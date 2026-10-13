/* Guest heap (malloc/free/realloc) in C, shared by lifted code and the emulator's Python runtime.
 *
 * Every piece of heap state lives in guest memory and is changed through the logged stores (W32), so an
 * undone run (verification, speculative worker call) rolls the heap back together with everything else.
 *
 *   HeapCtl (at ctl): bump pointer, end, one free list per small size class, one list of larger blocks.
 *   Block p: 16-byte header at p - 16 = { size, tag }; a free block's first word links to the next one.
 *
 * Same policy as the Python heap it replaces (emu/core.py Heap): 16-byte granularity, exact-size reuse of
 * small blocks, best fit (at most twice the request) for large ones, the top block grows in place and
 * returns to the bump pointer when freed. Included by the generated lift_table.c.
 */
#define HEAP_SMALL_MAX 65536u                /* sizes up to this have their own free list */
#define HEAP_CLASSES (HEAP_SMALL_MAX / 16 + 1)
#define HEAP_TAG_USED 0x48454150u            /* "HEAP" */
#define HEAP_TAG_FREE 0x45455246u            /* "FREE" */

/* HeapCtl layout (guest words) */
#define HC_PTR 0u
#define HC_END 4u
#define HC_LARGE 8u
#define HC_SMALL 12u                          /* HEAP_CLASSES words */
#define HC_SIZE (HC_SMALL + 4u * HEAP_CLASSES)

EXPORT uint32_t rt_heap_ctl;                  /* guest address of the HeapCtl, set by the host */

/* One recursive lock for the whole heap: guest threads are host threads in the standalone host. Uncontended in
 * the emulator (one host thread). */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
static volatile long heap_owner;
static int heap_depth;
static void heap_lock(void) {
    long me = (long)GetCurrentThreadId();
    if (heap_owner == me) { heap_depth++; return; }
    while (_InterlockedCompareExchange(&heap_owner, me, 0) != 0) YieldProcessor();
    heap_depth = 1;
}
static void heap_unlock(void) {
    if (--heap_depth == 0) _InterlockedExchange(&heap_owner, 0);
}
#define LOCKED(T, call) { T r_; heap_lock(); r_ = call; heap_unlock(); return r_; }

static uint32_t heap_size(uint32_t n) { n = (n + 15u) & ~15u; return n < 16u ? 16u : n; }

INLINE uint32_t HDR_SIZE(CPU *c, uint32_t p) { return R32(p - 16u); }
INLINE uint32_t HDR_TAG(CPU *c, uint32_t p) { return R32(p - 12u); }

static uint32_t heap_usable(CPU *c, uint32_t p) {
    if (!p || HDR_TAG(c, p) != HEAP_TAG_USED) return 0;
    return HDR_SIZE(c, p);
}

static uint32_t heap_malloc(CPU *c, uint32_t req) {
    uint32_t ctl = rt_heap_ctl, n = heap_size(req), p;
    if (req > 0xF0000000u) return 0;
    if (n <= HEAP_SMALL_MAX) {
        uint32_t slot = ctl + HC_SMALL + 4u * (n / 16u);
        p = R32(slot);
        if (p) {
            W32(c, slot, R32(p));
            W32(c, p - 12u, HEAP_TAG_USED);
            return p;
        }
    } else {
        /* best fit among freed large blocks, at most twice the request; the block keeps its size */
        uint32_t prev = ctl + HC_LARGE, best_prev = 0, best = 0, best_n = 0;
        for (uint32_t q = R32(prev); q; prev = q, q = R32(q)) {
            uint32_t qn = HDR_SIZE(c, q);
            if (qn >= n && qn <= 2u * n && (!best || qn < best_n)) { best = q; best_prev = prev; best_n = qn; }
        }
        if (best) {
            W32(c, best_prev, R32(best));
            W32(c, best - 12u, HEAP_TAG_USED);
            return best;
        }
    }
    uint32_t ptr = R32(ctl + HC_PTR);
    if ((uint64_t)ptr + 16u + n > R32(ctl + HC_END)) return 0;     /* exhausted */
    p = ptr + 16u;
    W32(c, ctl + HC_PTR, p + n);
    W32(c, p - 16u, n);
    W32(c, p - 12u, HEAP_TAG_USED);
    return p;
}

/* Returns 0, or 1 when p was not a live block (the host reports it). */
static uint32_t heap_free(CPU *c, uint32_t p) {
    uint32_t ctl = rt_heap_ctl;
    if (!p) return 0;
    if (HDR_TAG(c, p) != HEAP_TAG_USED) return 1;
    uint32_t n = HDR_SIZE(c, p);
    W32(c, p - 12u, HEAP_TAG_FREE);
    if (p + n == R32(ctl + HC_PTR)) {        /* top block: give it back to the bump pointer */
        W32(c, ctl + HC_PTR, p - 16u);
        return 0;
    }
    uint32_t slot = n <= HEAP_SMALL_MAX ? ctl + HC_SMALL + 4u * (n / 16u) : ctl + HC_LARGE;
    W32(c, p, R32(slot));
    W32(c, slot, p);
    return 0;
}

/* realloc fast path: the most recent allocation grows without copying. */
static uint32_t heap_grow_in_place(CPU *c, uint32_t p, uint32_t req) {
    uint32_t ctl = rt_heap_ctl, n = heap_size(req);
    if (!p || HDR_TAG(c, p) != HEAP_TAG_USED) return 0;
    uint32_t old = HDR_SIZE(c, p);
    if (p + old != R32(ctl + HC_PTR) || (uint64_t)p + n > R32(ctl + HC_END)) return 0;
    W32(c, ctl + HC_PTR, p + n);
    W32(c, p - 16u, n);
    return 1;
}

static uint32_t heap_realloc(CPU *c, uint32_t p, uint32_t n) {
    if (!p) return heap_malloc(c, n);
    if (!n) { heap_free(c, p); return 0; }
    uint32_t old = heap_usable(c, p);
    if (old >= n || heap_grow_in_place(c, p, n)) return p;
    uint32_t q = heap_malloc(c, n);
    if (!q) return 0;
    MEMMOVE(c, q, p, old);
    heap_free(c, p);
    return q;
}

static uint32_t heap_calloc(CPU *c, uint32_t count, uint32_t size) {
    uint64_t total = (uint64_t)count * size;
    if (total > 0xF0000000u) return 0;
    uint32_t p = heap_malloc(c, (uint32_t)total);
    if (p) MEMSET(c, p, 0, (uint32_t)total);
    return p;
}

EXPORT uint32_t rt_heap_usable(CPU *c, uint32_t p) LOCKED(uint32_t, heap_usable(c, p))
EXPORT uint32_t rt_heap_malloc(CPU *c, uint32_t n) LOCKED(uint32_t, heap_malloc(c, n))
/* Returns 0, or 1 when p was not a live block (the host reports it). */
EXPORT uint32_t rt_heap_free(CPU *c, uint32_t p) LOCKED(uint32_t, heap_free(c, p))
EXPORT uint32_t rt_heap_grow_in_place(CPU *c, uint32_t p, uint32_t n) LOCKED(uint32_t, heap_grow_in_place(c, p, n))
EXPORT uint32_t rt_heap_realloc(CPU *c, uint32_t p, uint32_t n) LOCKED(uint32_t, heap_realloc(c, p, n))
EXPORT uint32_t rt_heap_calloc(CPU *c, uint32_t count, uint32_t size) LOCKED(uint32_t, heap_calloc(c, count, size))

/* ---------------------------------------------------------------- qsort, errno
 * qsort as emu/libc.py does it: a stable sort of the elements of a copy (the comparator reads the copy, never
 * the array being reordered), then the array rewritten in sorted order. The comparator is guest code and runs
 * through the lifted code; when it is not lifted the call is declined and the host's implementation runs. */
static LiftFn lookup(uint32_t target);
#define QS_RET 0xFFFFFFF1u

static int qs_less(CPU *c, uint32_t fn, uint32_t a, uint32_t b, int *bad) {
    c->r[0] = a; c->r[1] = b; c->r[14] = QS_RET;
    rt_call(c, fn);
    if (c->pc != QS_RET) { *bad = 1; return 0; }
    return (int32_t)c->r[0] < 0;
}

static void qs_merge(CPU *c, uint32_t fn, uint32_t copy, uint32_t size, uint32_t *idx, uint32_t *tmp,
                     uint32_t n, int *bad) {
    if (n < 2 || *bad) return;
    uint32_t h = n / 2;
    qs_merge(c, fn, copy, size, idx, tmp, h, bad);
    qs_merge(c, fn, copy, size, idx + h, tmp, n - h, bad);
    uint32_t i = 0, j = h, k = 0;
    while (i < h && j < n && !*bad) {
        /* take the right element only when it is strictly smaller: stable */
        if (qs_less(c, fn, copy + idx[j] * size, copy + idx[i] * size, bad)) tmp[k++] = idx[j++];
        else tmp[k++] = idx[i++];
    }
    while (i < h) tmp[k++] = idx[i++];
    while (j < n) tmp[k++] = idx[j++];
    memcpy(idx, tmp, n * sizeof(uint32_t));
}

static int native_qsort(CPU *c) {
    uint32_t base = c->r[0], n = c->r[1], size = c->r[2], fn = c->r[3], lr = c->r[14];
    if (n < 2 || !size) { c->r[0] = 0; return 1; }
    if (!rt_heap_ctl || !lookup(fn) || (uint64_t)n * size > 0x10000000u) return 0;
    uint32_t copy = rt_heap_malloc(c, n * size);
    uint32_t *idx = (uint32_t *)malloc(2u * n * sizeof(uint32_t));
    if (!copy || !idx) { if (copy) rt_heap_free(c, copy); free(idx); return 0; }
    MEMMOVE(c, copy, base, n * size);
    for (uint32_t k = 0; k < n; k++) idx[k] = k;
    uint32_t keep[13];
    memcpy(keep, c->r, sizeof(keep));               /* r0-r12: the comparator may clobber the scratch ones */
    int bad = 0;
    qs_merge(c, fn, copy, size, idx, idx + n, n, &bad);
    memcpy(c->r, keep, sizeof(keep));
    if (!bad)
        for (uint32_t k = 0; k < n; k++) MEMMOVE(c, base + k * size, copy + idx[k] * size, size);
    rt_heap_free(c, copy);
    free(idx);
    c->r[14] = lr;
    if (bad) return 0;     /* the comparator escaped: nothing changed, the host's qsort runs instead */
    c->r[0] = 0;
    return 1;
}

EXPORT uint32_t rt_errno_ptr;                    /* the current guest thread's errno, set by the host */
EXPORT uint32_t (*rt_errno_fn)(void);            /* or this: per host thread (standalone host) */

/* Host side: the HeapCtl size, so it can be allocated and handed over. */
EXPORT uint32_t rt_heap_ctl_size(void) { return HC_SIZE; }
