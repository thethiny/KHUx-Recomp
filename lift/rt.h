/* Runtime support for code lifted from the ARMv5TE (ARM + Thumb-1) game library.
 *
 * Guest memory is one flat 4 GB block: guest address a is host address mem + a. The emulator maps the same
 * block, so lifted code and emulated code share every byte. Registers and flags live in CPU, which the
 * caller fills before a lifted function runs and reads back afterwards.
 *
 * A lifted function returns by storing the guest return address (Thumb bit included) in c->pc.
 */
#ifndef KHUX_LIFT_RT_H
#define KHUX_LIFT_RT_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#define INLINE static __forceinline
#else
#define EXPORT __attribute__((visibility("default")))
#define INLINE static inline __attribute__((always_inline))
#endif

typedef struct WLog {
    uint32_t addr, size, old_lo, old_hi;   /* store of `size` bytes at addr; previous contents */
} WLog;

typedef struct CPU {
    uint32_t r[16];        /* r0..r12, sp, lr, pc (pc unused while running) */
    uint32_t n, z, c, v;   /* condition flags, 0 or 1 */
    uint32_t pc;           /* guest address the function returned to */
    uint32_t logging;      /* nonzero: record every store in wlog (differential testing) */
    uint32_t wn, wcap;     /* used / capacity of wlog */
    uint8_t *mem;          /* host address of guest address 0 */
    WLog *wlog;
    /* Run guest code at `target` in the emulator (callee not lifted or not enabled). On return the
     * registers are back in the struct and c->pc is the return address, or anything else after a fault. */
    void (*emu)(struct CPU *c, uint32_t target);
    uint32_t tail;         /* set with c->pc by a tail call: continue at c->pc with the same LR */
    uint32_t lpc;          /* guest address of the instruction being executed (crash reports) */
    uint32_t wfull;        /* the store log overflowed: the run is void (wn still counts valid entries) */
    uint64_t icount;       /* guest instructions executed in lifted code (profiling) */
    /* VFP state, loaded from the emulator on first use (vfp_on = 0 means vf/fpscr are stale). The host writes
     * it back to the emulator when vfp_on is set, whenever control returns there. */
    uint32_t vfp_on;
    uint32_t fpscr;
    void *uc;              /* emulator handle and its uc_reg_read_batch, for rt_vfp_load */
    void *uc_read_batch;
    union {
        uint64_t d[32];    /* d0..d31 */
        uint32_t s[64];    /* s0..s31 alias d0..d15 (s[2k] is the low half of d[k]) */
        float f[64];
        double df[32];
    } vf;
} CPU;

/* Call guest function `target` (Thumb bit set for Thumb code): a lifted, enabled function runs directly,
 * anything else through c->emu. Defined in the generated lift_table.c. Callers set LR first and afterwards
 * check that c->pc is their return address; otherwise they return too (the emulator continues there). */
void rt_call(CPU *c, uint32_t target);
/* Entry from the host hook: like rt_call, but a tail call into code that is not lifted is not run in the
 * emulator - it returns with c->pc = target so the emulator simply jumps there. */
EXPORT void rt_run(CPU *c, uint32_t target);
/* A lifted function's tail call: tail-call chains (IDA-split loops) run in rt_call's loop, not recursively. */
#define TAIL(t) do { c->pc = (t); c->tail = 1; return; } while (0)
/* Call into a frame fragment (an IDA-split piece of a function that returns through its parent's frame, not to
 * LR): run it in C when lifted, otherwise escape to it - the emulator cannot run it as a call, since it never
 * returns to the call site. */
void rt_call_frag(CPU *c, uint32_t target);

/* Undo every logged store, newest first (defined in lift_table.c). */
EXPORT void rt_undo(CPU *c);

/* Fetch the VFP registers and FPSCR from the emulator (defined in lift_table.c). */
void rt_vfp_load(CPU *c);
#define VFP_USE() do { if (!c->vfp_on) rt_vfp_load(c); } while (0)

/* ---------------------------------------------------------------- memory */
#define HA(a) (c->mem + (uint32_t)(a))
/* raw access (stores, the log, the runtime's own reads) */
#define M8(a)  (*(uint8_t *)HA(a))
#define M16(a) (*(uint16_t *)HA(a))
#define M32(a) (*(uint32_t *)HA(a))

/* ---------------------------------------------------------------- stale stack (M3 verifier)
 * The verifier compares every byte a function stores outside its stack with what its rewrite stores. Compiled code
 * sometimes copies stack bytes it never initialised into memory that outlives the call (a struct built on the stack
 * with unset fields): those bytes are whatever an earlier, returned call left there (its pushes, its locals), so they
 * differ with any other code that ran before - including a rewrite, whose frames and pushes differ. While a shadow
 * run logs, GTrack follows the stack window below the run's entry sp: each word's last store (clock), and each
 * return (the sp after it). A load from a word that was never stored in this run, or whose storing frame has since
 * returned (a later return left sp above it), reads stale bytes: its value goes into `vals`. The verifier then
 * accepts a differing word when both runs' values came from stale loads. */
typedef struct GTrack {
    uint32_t on;                      /* tracking this run */
    uint32_t lo, hi;                  /* window [lo, hi): hi = the run's entry sp */
    uint32_t clock;
    uint32_t nret, overflow;          /* returns: a stack of (time, sp) with sp decreasing (dominated ones dropped) */
    uint32_t *wtime;                  /* (hi - lo) / 4 words: time of the last store, 0 = not stored in this run */
    uint32_t *ret_t, *ret_sp;         /* GT_RETS entries */
    uint32_t *vals;                   /* GT_VALS-slot open hash of stale values (0 stored as GT_ZERO_KEY flag) */
    uint32_t has_zero;
} GTrack;
#define GT_WINDOW 0x20000u
#define GT_RETS 65536u
#define GT_VALS 16384u
/* the active tracker (lift.dll: the host sets it; rewrite.dll: its copy, from the host) */
EXPORT extern GTrack *rt_gtrack;

#ifdef _MSC_VER
#define GT_COLD static __declspec(noinline)
#else
#define GT_COLD static __attribute__((noinline, unused))
#endif
/* (the tracking bodies are out of line: they only run while the verifier logs, and every load site stays small) */
INLINE int gt_active(CPU *c) { return c->logging && rt_gtrack && rt_gtrack->on; }
GT_COLD void gt_store(CPU *c, uint32_t a, uint32_t size) {
    GTrack *g = rt_gtrack;
    if (a - g->lo < g->hi - g->lo) {
        g->wtime[(a - g->lo) >> 2] = ++g->clock;
        uint32_t e = a + size - 1;
        if (e - g->lo < g->hi - g->lo) g->wtime[(e - g->lo) >> 2] = g->clock;
    }
}
/* a lifted call returned with sp: every word below sp is free again */
GT_COLD void gt_return(CPU *c) {
    GTrack *g = rt_gtrack;
    uint32_t sp = c->r[13];
    while (g->nret && g->ret_sp[g->nret - 1] <= sp) g->nret--;
    if (g->nret >= GT_RETS) { g->overflow = 1; return; }
    g->ret_t[g->nret] = ++g->clock;
    g->ret_sp[g->nret] = sp;
    g->nret++;
}
INLINE void gt_add(GTrack *g, uint32_t v) {
    if (!v) { g->has_zero = 1; return; }
    uint32_t h = (v * 2654435761u) & (GT_VALS - 1);
    for (uint32_t k = 0; k < 64; k++, h = (h + 1) & (GT_VALS - 1)) {
        if (g->vals[h] == v) return;
        if (!g->vals[h]) { g->vals[h] = v; return; }
    }
}
INLINE int gt_has(const GTrack *g, uint32_t v) {
    if (!v) return (int)g->has_zero;
    uint32_t h = (v * 2654435761u) & (GT_VALS - 1);
    for (uint32_t k = 0; k < 64; k++, h = (h + 1) & (GT_VALS - 1)) {
        if (g->vals[h] == v) return 1;
        if (!g->vals[h]) return 0;
    }
    return 0;
}
/* a load of the word at a (aligned): stale when never stored in this run, or a return since its store left sp
 * above it */
GT_COLD void gt_load(CPU *c, uint32_t a) {
    GTrack *g = rt_gtrack;
    a &= ~3u;
    if (a - g->lo >= g->hi - g->lo || g->overflow) return;
    uint32_t t = g->wtime[(a - g->lo) >> 2];
    int stale = t == 0;
    if (!stale) {                       /* the first return after t has the highest sp of those after t */
        uint32_t lo = 0, hi = g->nret;
        while (lo < hi) { uint32_t m = (lo + hi) / 2; if (g->ret_t[m] > t) hi = m; else lo = m + 1; }
        stale = lo < g->nret && g->ret_sp[lo] > a;
    }
    if (stale) gt_add(g, M32(a));
}
GT_COLD uint32_t LD32(CPU *c, uint32_t a) { if (rt_gtrack && rt_gtrack->on) gt_load(c, a); return M32(a); }
GT_COLD uint32_t LD16(CPU *c, uint32_t a) { if (rt_gtrack && rt_gtrack->on) gt_load(c, a); return M16(a); }
GT_COLD uint32_t LD8(CPU *c, uint32_t a)  { if (rt_gtrack && rt_gtrack->on) gt_load(c, a); return M8(a); }
/* loads (lifted code) */
#define R8(a)  (c->logging ? LD8(c, (uint32_t)(a)) : (uint32_t)M8(a))
#define R16(a) (c->logging ? LD16(c, (uint32_t)(a)) : (uint32_t)M16(a))
#define R32(a) (c->logging ? LD32(c, (uint32_t)(a)) : M32(a))
#define RS8(a)  ((uint32_t)(int32_t)(int8_t)R8(a))
#define RS16(a) ((uint32_t)(int32_t)(int16_t)R16(a))

/* Record a store about to happen. When the log is full, wfull is set and logging stops: the store and the rest of
 * the run happen for real, unlogged, so the run cannot be undone any more and whoever logs keeps it as the real
 * execution (the M3 verifier does), if rt_log_full_continue; otherwise the store is skipped (returns 0) and the run
 * is void and gets undone. wn always counts
 * only the entries written by this run (older entries past it are stale). */
/* 1: a full log stops logging and the run goes on for real (the native host's M3 verifier); 0: the store is
 * skipped and the run is void (the emulator's speculative runs undo it). Defined in lift_table.c / rewrite.dll. */
EXPORT extern int rt_log_full_continue;

INLINE int wlog(CPU *c, uint32_t a, uint32_t size) {
    if (c->wn < c->wcap) {
        WLog *w = &c->wlog[c->wn++];
        w->addr = a;
        w->size = size;
        w->old_lo = size == 1 ? M8(a) : size == 2 ? M16(a) : M32(a);
        w->old_hi = c->lpc;    /* guest pc of the store (diagnostics) */
        if (rt_gtrack && rt_gtrack->on) gt_store(c, a, size);
        return 1;
    }
    c->wfull = 1;
    if (!rt_log_full_continue) return 0;
    c->logging = 0;
    return 1;
}

INLINE void W8(CPU *c, uint32_t a, uint32_t v)  { if (c->logging && !wlog(c, a, 1)) return; M8(a) = (uint8_t)v; }
INLINE void W16(CPU *c, uint32_t a, uint32_t v) { if (c->logging && !wlog(c, a, 2)) return; M16(a) = (uint16_t)v; }
INLINE void W32(CPU *c, uint32_t a, uint32_t v) { if (c->logging && !wlog(c, a, 4)) return; M32(a) = v; }

/* Make room in the store log for a block write of n bytes at a (4-byte entries, bytes for the tail).
 * When it does not fit: as in wlog (returns 0 when the write must be skipped). */
INLINE int LOGRANGE(CPU *c, uint32_t a, uint32_t n) {
    if (!c->logging || !n) return 1;
    if ((uint64_t)c->wn + n / 4 + 3 > c->wcap) {
        c->wfull = 1;
        if (!rt_log_full_continue) return 0;
        c->logging = 0;
        return 1;
    }
    uint32_t k = 0;
    for (; k + 4 <= n; k += 4) wlog(c, a + k, 4);
    for (; k < n; k++) wlog(c, a + k, 1);
    return 1;
}
INLINE void MEMMOVE(CPU *c, uint32_t d, uint32_t s, uint32_t n) {
    if (n && gt_active(c)) for (uint32_t k = s & ~3u; k < s + n; k += 4) gt_load(c, k);
    if (n && LOGRANGE(c, d, n)) memmove(HA(d), HA(s), n);
}
INLINE void MEMSET(CPU *c, uint32_t d, uint32_t v, uint32_t n) {
    if (n && LOGRANGE(c, d, n)) memset(HA(d), (int)(v & 0xFF), n);
}
INLINE uint32_t SIGN3(int v) { return v < 0 ? 0xFFFFFFFFu : v > 0 ? 1u : 0u; }

/* ---------------------------------------------------------------- exclusives, barriers
 * strex succeeds when memory still holds the value its ldrex read: a host compare-exchange, so it stays atomic
 * once guest threads are host threads. Returns 0 on success, 1 on failure (the strex status). */
#ifdef _MSC_VER
#include <intrin.h>
#define FENCE() _mm_mfence()
INLINE uint32_t STREX32(CPU *c, uint32_t a, uint32_t old, uint32_t v) {
    if (c->logging && !wlog(c, a, 4)) return 1;
    return (uint32_t)_InterlockedCompareExchange((volatile long *)HA(a), (long)v, (long)old) != old;
}
INLINE uint32_t STREX16(CPU *c, uint32_t a, uint32_t old, uint32_t v) {
    if (c->logging && !wlog(c, a, 2)) return 1;
    return (uint16_t)_InterlockedCompareExchange16((volatile short *)HA(a), (short)v, (short)old) != (uint16_t)old;
}
INLINE uint32_t STREX8(CPU *c, uint32_t a, uint32_t old, uint32_t v) {
    if (c->logging && !wlog(c, a, 1)) return 1;
    return (uint8_t)_InterlockedCompareExchange8((volatile char *)HA(a), (char)v, (char)old) != (uint8_t)old;
}
#else
#define FENCE() __sync_synchronize()
#define STREX_GEN(n, T) INLINE uint32_t STREX##n(CPU *c, uint32_t a, uint32_t old, uint32_t v) { \
    if (c->logging && !wlog(c, a, n / 8)) return 1; \
    return !__sync_bool_compare_and_swap((volatile T *)HA(a), (T)old, (T)v); }
STREX_GEN(32, uint32_t)
STREX_GEN(16, uint16_t)
STREX_GEN(8, uint8_t)
#endif

/* ---------------------------------------------------------------- flags */
INLINE uint32_t ADC_F(CPU *c, uint32_t a, uint32_t b, uint32_t cin) {
    uint64_t s = (uint64_t)a + b + cin;
    uint32_t r = (uint32_t)s;
    c->c = (uint32_t)(s >> 32) & 1;
    c->v = ((~(a ^ b)) & (a ^ r)) >> 31;
    c->n = r >> 31;
    c->z = r == 0;
    return r;
}
#define ADD_F(a, b)      ADC_F(c, (a), (b), 0)
#define SUB_F(a, b)      ADC_F(c, (a), ~(uint32_t)(b), 1)
#define SBC_F(a, b, cin) ADC_F(c, (a), ~(uint32_t)(b), (cin))

INLINE uint32_t NZ(CPU *c, uint32_t r) { c->n = r >> 31; c->z = r == 0; return r; }

/* ---------------------------------------------------------------- barrel shifter
 * type: 0 LSL, 1 LSR, 2 ASR, 3 ROR, 4 RRX. amt is the shift amount (immediate, or the low byte of a
 * register). *co receives the shifter carry-out (unchanged cin when the amount is 0). */
INLINE uint32_t SHIFT(uint32_t v, int type, uint32_t amt, uint32_t cin, uint32_t *co) {
    *co = cin;
    switch (type) {
    case 0:
        if (amt == 0) return v;
        if (amt < 32) { *co = (v >> (32 - amt)) & 1; return v << amt; }
        *co = amt == 32 ? (v & 1) : 0;
        return 0;
    case 1:
        if (amt == 0) return v;
        if (amt < 32) { *co = (v >> (amt - 1)) & 1; return v >> amt; }
        *co = amt == 32 ? (v >> 31) : 0;
        return 0;
    case 2:
        if (amt == 0) return v;
        if (amt < 32) { *co = (v >> (amt - 1)) & 1; return (uint32_t)((int32_t)v >> amt); }
        *co = v >> 31;
        return (uint32_t)((int32_t)v >> 31);
    case 3:
        if (amt == 0) return v;
        amt &= 31;
        if (amt == 0) { *co = v >> 31; return v; }
        v = (v >> amt) | (v << (32 - amt));
        *co = v >> 31;
        return v;
    default: /* RRX */
        *co = v & 1;
        return (cin << 31) | (v >> 1);
    }
}

INLINE uint32_t CLZ(uint32_t v) {
    uint32_t n = 0;
    if (!v) return 32;
    while (!(v & 0x80000000u)) { v <<= 1; n++; }
    return n;
}

/* ---------------------------------------------------------------- VFP
 * Arithmetic runs in host single/double precision with round-to-nearest, which matches the VFP's default mode
 * (no flush-to-zero; Android starts threads with FPSCR = 0). Products of vmla/vmls are rounded before the add,
 * as on the VFP (no fused multiply-add). */
INLINE void FCMP(CPU *c, double a, double b) {
    uint32_t f;
    if (a != a || b != b) f = 0x3u;          /* unordered: C, V */
    else if (a == b) f = 0x6u;               /* Z, C */
    else if (a < b) f = 0x8u;                /* N */
    else f = 0x2u;                           /* C */
    c->fpscr = (c->fpscr & 0x0FFFFFFFu) | (f << 28);
}
INLINE void FPFLAGS(CPU *c) {                /* vmrs APSR_nzcv, fpscr */
    c->n = c->fpscr >> 31; c->z = (c->fpscr >> 30) & 1; c->c = (c->fpscr >> 29) & 1; c->v = (c->fpscr >> 28) & 1;
}
/* float/double to integer, rounding toward zero, saturating; NaN gives 0 */
INLINE uint32_t CVT_S32(double v) {
    if (v != v) return 0;
    if (v >= 2147483648.0) return 0x7FFFFFFFu;
    if (v <= -2147483649.0) return 0x80000000u;
    return (uint32_t)(int32_t)v;
}
INLINE uint32_t CVT_U32(double v) {
    if (v != v || v <= -1.0) return 0;
    if (v >= 4294967296.0) return 0xFFFFFFFFu;
    return (uint32_t)(int64_t)v;
}
INLINE float F32(uint32_t bits) { union { uint32_t u; float f; } x; x.u = bits; return x.f; }
/* soft-float ABI values in core registers (double in r0:r1, float in r0) */
INLINE double D64(uint32_t lo, uint32_t hi) { union { uint64_t u; double d; } x; x.u = lo | ((uint64_t)hi << 32); return x.d; }
INLINE void RET_D(CPU *c, double v) { union { uint64_t u; double d; } x; x.d = v; c->r[0] = (uint32_t)x.u; c->r[1] = (uint32_t)(x.u >> 32); }
INLINE uint32_t FBITS(float f) { union { uint32_t u; float f; } x; x.f = f; return x.u; }
INLINE uint64_t RD64(CPU *c, uint32_t a) { return (uint64_t)R32(a) | ((uint64_t)R32(a + 4) << 32); }

/* ---------------------------------------------------------------- export table */
typedef void (*LiftFn)(CPU *);
typedef struct LiftEntry {
    uint32_t addr;   /* guest entry, Thumb bit set for Thumb functions */
    uint32_t flags;  /* bit 0: leaf (no calls, no escapes) - verifiable against the emulator */
    LiftFn fn;
} LiftEntry;

#endif
