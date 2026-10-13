/* lift.dll binding, import dispatch, guest calls, heap helpers, logging. */
#include "host.h"

Lift L;
static CPU hcpu;                         /* for heap calls made by the host (never logs) */
static CRITICAL_SECTION log_lock;
static FILE *log_file;

/* ---------------------------------------------------------------- logging */
void hlog(const char *fmt, ...) {
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    EnterCriticalSection(&log_lock);
    fputs(buf, stdout);
    fputc('\n', stdout);
    fflush(stdout);
    if (log_file) {
        fputs(buf, log_file);
        fputc('\n', log_file);
        fflush(log_file);
    }
    LeaveCriticalSection(&log_lock);
}

const char *thread_label(void) {
    static __declspec(thread) char buf[128];
    if (!cur) return "host";
    snprintf(buf, sizeof(buf), "thread %d (%s)", cur->tid, cur->name);
    return buf;
}

static void dump_cpu(CPU *c) {
    hlog("   r0=%08X r1=%08X r2=%08X r3=%08X r4=%08X r5=%08X r6=%08X r7=%08X", c->r[0], c->r[1], c->r[2],
         c->r[3], c->r[4], c->r[5], c->r[6], c->r[7]);
    hlog("   r8=%08X r9=%08X r10=%08X r11=%08X r12=%08X sp=%08X lr=%08X lpc=%08X", c->r[8], c->r[9], c->r[10],
         c->r[11], c->r[12], c->r[13], c->r[14], c->lpc);
    /* Thumb return addresses on the stack: the call chain */
    uint32_t sp = c->r[13];
    int shown = 0;
    for (int k = 0; k < 256 && shown < 16; k++) {
        uint32_t a = sp + 4u * (uint32_t)k;
        int mapped = (a >= HEAP_BASE && a < HEAP_BASE + HEAP_SIZE) || (a >= STACK_BASE && a < STACK_BASE + STACK_SIZE);
        if (!mapped) break;
        uint32_t w = g32(sp + 4u * k);
        if ((w & 1) && (w & ~1u) >= 0x44EFD0u && (w & ~1u) < 0xCF9C9Cu) {
            hlog("   [sp+0x%X] %08X", 4 * k, w);
            shown++;
        }
    }
}

void fatal(const char *fmt, ...) {
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    hlog("!! FATAL [%s]: %s", thread_label(), buf);
    if (cur && cur->cpu) dump_cpu(cur->cpu);
    ctl_event("crash", NULL);
    ExitProcess(1);
}

static LONG WINAPI on_crash(EXCEPTION_POINTERS *ep) {
    EXCEPTION_RECORD *er = ep->ExceptionRecord;
    uintptr_t at = er->NumberParameters >= 2 ? er->ExceptionInformation[1] : 0;
    char where[96] = "";
    if (G && at >= (uintptr_t)G && at < (uintptr_t)G + ((uintptr_t)1 << 32))
        snprintf(where, sizeof(where), " (guest address 0x%08X)", (uint32_t)(at - (uintptr_t)G));
    HMODULE mod = NULL;
    char mod_name[MAX_PATH] = "?";
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPCSTR)er->ExceptionAddress, &mod);
    if (mod) GetModuleFileNameA(mod, mod_name, MAX_PATH);
    hlog("!! CRASH [%s]: exception 0x%08lX at %s+0x%llX, data %p%s", thread_label(), er->ExceptionCode,
         strrchr(mod_name, 92) ? strrchr(mod_name, 92) + 1 : mod_name,
         (unsigned long long)((uintptr_t)er->ExceptionAddress - (uintptr_t)mod), (void *)at, where);
    if (cur && cur->cpu) dump_cpu(cur->cpu);
    ctl_event("crash", NULL);
    ExitProcess(2);
}

/* ---------------------------------------------------------------- lift.dll */
#define GETP(field, name) do { *(void **)&L.field = (void *)GetProcAddress(L.dll, name); \
    if (!L.field) fatal("lift.dll has no export %s (rebuild it with lift/build.py)", name); } while (0)

static uint32_t errno_get(void) { return cur ? cur->errno_ptr : 0; }

void lift_load(const char *path) {
    L.dll = LoadLibraryA(path);
    if (!L.dll) fatal("cannot load %s (error %lu)", path, GetLastError());
    GETP(table, "lift_table");
    int *count;
    *(void **)&count = (void *)GetProcAddress(L.dll, "lift_count");
    if (!count) fatal("lift.dll has no lift_count");
    L.count = *count;
    GETP(enabled, "lift_enabled");
    GETP(run, "rt_run");
    GETP(lookup, "rt_lookup");
    GETP(native_call, "rt_native_call");
    GETP(native_slot, "rt_native_slot");
    int *ncount;
    *(void **)&ncount = (void *)GetProcAddress(L.dll, "rt_native_count");
    if (!ncount) fatal("lift.dll has no rt_native_count");
    L.native_count = *ncount;
    GETP(native_names, "rt_native_names");
    GETP(heap_malloc, "rt_heap_malloc");
    GETP(heap_free, "rt_heap_free");
    GETP(heap_realloc, "rt_heap_realloc");
    GETP(heap_usable, "rt_heap_usable");
    GETP(heap_ctl_size, "rt_heap_ctl_size");
    GETP(heap_ctl, "rt_heap_ctl");
    GETP(errno_fn, "rt_errno_fn");
    GETP(hook, "rt_hook");
    GETP(fs_root, "rt_fs_root");
    GETP(so_guest, "rt_so_guest");
    GETP(so_host, "rt_so_host");
    GETP(gl_init, "rt_gl_init");
    GETP(gl_strings, "rt_gl_strings");
    GETP(gl_draws, "rt_gl_draws");
    GETP(plt, "rt_plt");
    int *pcount;
    *(void **)&pcount = (void *)GetProcAddress(L.dll, "rt_plt_count");
    if (!pcount) fatal("lift.dll has no rt_plt_count");
    L.plt_count = *pcount;
    memset(L.enabled, 1, (size_t)L.count);       /* every lifted function runs; there is nothing else */
    *L.errno_fn = errno_get;
    hlog("lift.dll: %d functions, %d native imports, %d PLT imports", L.count, L.native_count, L.plt_count);
}

void heap_init(void) {
    uint32_t ctl = HEAP_BASE, size = L.heap_ctl_size();
    memset(GP(ctl), 0, size);
    gw32(ctl, (HEAP_BASE + size + 15u) & ~15u);           /* bump pointer */
    gw32(ctl + 4, HEAP_BASE + HEAP_SIZE - 0x1000u);      /* end (the last page stays clear of the 4 GB edge) */
    *L.heap_ctl = ctl;
    hcpu.mem = G;
}

void core_init(void) {
    InitializeCriticalSection(&log_lock);
    log_file = fopen("host.log", "w");
    SetUnhandledExceptionFilter(on_crash);
}

/* ---------------------------------------------------------------- heap */
uint32_t galloc(uint32_t n) {
    uint32_t p = L.heap_malloc(&hcpu, n);
    if (!p) fatal("guest heap exhausted (%u bytes)", n);
    return p;
}
uint32_t gzalloc(uint32_t n) {
    uint32_t p = galloc(n ? n : 1);
    memset(GP(p), 0, n);
    return p;
}
uint32_t galloc_bytes(const void *src, uint32_t n) {
    uint32_t p = galloc(n ? n : 1);
    memcpy(GP(p), src, n);
    return p;
}
uint32_t galloc_str(const char *s) { return galloc_bytes(s, (uint32_t)strlen(s) + 1); }
void gfree(uint32_t p) {
    if (p && L.heap_free(&hcpu, p)) hlog("free of unknown pointer 0x%08X", p);
}

void set_errno(int e) {
    if (cur && cur->errno_ptr) gw32(cur->errno_ptr, (uint32_t)e);
}

/* ---------------------------------------------------------------- import registry */
#define IMP_CAP 2048
static struct { const char *name; ImportFn fn; } imps[IMP_CAP];
static int n_imps;
#define SLOT_N (TRAP_SIZE / 8)
static const char *slot_name[SLOT_N];
static ImportFn slot_fn[SLOT_N];
static int n_slots;
static ImportFn *plt_fn;

void imp(const char *name, ImportFn fn) {
    for (int k = 0; k < n_imps; k++) if (!strcmp(imps[k].name, name)) { imps[k].fn = fn; return; }
    if (n_imps >= IMP_CAP) fatal("too many imports");
    imps[n_imps].name = name;
    imps[n_imps++].fn = fn;
}

static ImportFn imp_find(const char *name) {
    for (int k = 0; k < n_imps; k++) if (!strcmp(imps[k].name, name)) return imps[k].fn;
    return NULL;
}

static int native_id(const char *name) {
    for (int k = 0; k < L.native_count; k++) if (!strcmp(L.native_names[k], name)) return k;
    return -1;
}

int import_exists(const char *name) { return imp_find(name) != NULL || native_id(name) >= 0; }

uint32_t slot_for(const char *name) {
    for (int k = 0; k < n_slots; k++) if (!strcmp(slot_name[k], name)) return TRAP_BASE + 8u * k;
    if (n_slots >= SLOT_N - 2) fatal("out of import slots");
    slot_name[n_slots] = _strdup(name);
    return TRAP_BASE + 8u * n_slots++;
}

uint32_t host_fn(const char *name, ImportFn fn) {
    uint32_t a = slot_for(name);
    slot_fn[(a - TRAP_BASE) / 8] = fn;
    return a;
}

/* Every slot and PLT import gets its implementation; natives in lift.dll are registered for slot calls. */
void imports_bind(void) {
    int missing = 0, natives = 0;
    for (int k = 0; k < n_slots; k++) {
        int id = native_id(slot_name[k]);
        if (id >= 0) {
            L.native_slot(TRAP_BASE + 8u * k, id);
            natives++;
        }
        if (!slot_fn[k]) slot_fn[k] = imp_find(slot_name[k]);
    }
    plt_fn = calloc((size_t)L.plt_count, sizeof(ImportFn));
    for (int k = 0; k < L.plt_count; k++) {
        plt_fn[k] = imp_find(L.plt[k].name);
        if (!plt_fn[k] && native_id(L.plt[k].name) < 0) {
            if (missing < 40) hlog("   no implementation: %s", L.plt[k].name);
            missing++;
        }
    }
    hlog("imports: %d slots (%d native in lift.dll), %d host implementations, %d imports without one "
         "(calling them stops the game)", n_slots, natives, n_imps, missing);
}

const char *import_name(uint32_t target) {
    uint32_t a = target & ~1u;
    if (a >= TRAP_BASE && a < TRAP_BASE + TRAP_SIZE) {
        uint32_t k = (a - TRAP_BASE) / 8;
        return k < (uint32_t)n_slots ? slot_name[k] : "?";
    }
    int lo = 0, hi = L.plt_count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (L.plt[mid].addr == a) return L.plt[mid].name;
        if (L.plt[mid].addr < a) lo = mid + 1; else hi = mid - 1;
    }
    return NULL;
}

static ImportFn import_fn(uint32_t a) {
    if (a >= TRAP_BASE && a < TRAP_BASE + TRAP_SIZE) {
        uint32_t k = (a - TRAP_BASE) / 8;
        if (k >= (uint32_t)n_slots) return NULL;
        if (!slot_fn[k]) slot_fn[k] = imp_find(slot_name[k]);     /* a slot made after imports_bind */
        return slot_fn[k];
    }
    int lo = 0, hi = L.plt_count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (L.plt[mid].addr == a) return plt_fn[mid];
        if (L.plt[mid].addr < a) lo = mid + 1; else hi = mid - 1;
    }
    return NULL;
}

/* Linux ARM kuser helpers (ARMv5 has no atomic instructions) */
static void kuser(CPU *c, uint32_t a) {
    switch (a - KUSER_PAGE) {
    case 0xFC0: {     /* int __kuser_cmpxchg(old, new, ptr): 0 and C set on success */
        uint32_t old = c->r[0];
        int ok = (uint32_t)_InterlockedCompareExchange((volatile long *)GP(c->r[2]), (long)c->r[1], (long)old) == old;
        c->r[0] = ok ? 0 : 1;
        c->c = (uint32_t)ok;
        break;
    }
    case 0xF60: {     /* int __kuser_cmpxchg64(const u64 *old, const u64 *new, u64 *ptr) */
        int64_t old = *(int64_t *)GP(c->r[0]);
        int ok = _InterlockedCompareExchange64((volatile int64_t *)GP(c->r[2]), *(int64_t *)GP(c->r[1]), old) == old;
        c->r[0] = ok ? 0 : 1;
        c->c = (uint32_t)ok;
        break;
    }
    case 0xFE0:       /* void *__kuser_get_tls(void) */
        if (!cur->kuser_tls) cur->kuser_tls = gzalloc(0x400);
        c->r[0] = cur->kuser_tls;
        break;
    case 0xFA0:       /* __kuser_memory_barrier */
        MemoryBarrier();
        break;
    default:
        fatal("unknown kuser helper 0x%08X", a);
    }
}

__declspec(thread) uint32_t imp_target;       /* the import being run (generic handlers read their name) */

/* lift.dll calls this for a call target outside the game's code that it does not run natively. On return
 * c->pc is the caller's return address (LR). */
void host_emu(CPU *c, uint32_t target) {
    uint32_t a = target & ~1u, lr = c->r[14];
    if (a >= KUSER_PAGE) {
        kuser(c, a);
        c->pc = lr;
        return;
    }
    if (a == RET_MAGIC) {                         /* tail call to the host's return address */
        c->pc = RET_MAGIC;
        return;
    }
    ImportFn fn = import_fn(a);
    if (!fn) {
        const char *name = import_name(a);
        if (name) fatal("unimplemented import %s called from 0x%08X (r0=%08X r1=%08X)", name, lr, c->r[0], c->r[1]);
        fatal("call to 0x%08X from 0x%08X: neither lifted code nor an import", target, lr);
    }
    uint32_t outer = imp_target;
    imp_target = a;
    fn(c);
    imp_target = outer;
    c->pc = lr;
}

/* ---------------------------------------------------------------- calls into the guest */
static int external(uint32_t a) {
    return (a >= 0x40DCC4u && a < 0x44EFD0u) || (a >= TRAP_BASE && a < TRAP_BASE + TRAP_SIZE) || a >= KUSER_PAGE;
}

/* Run guest code from target until control returns to `stop`. */
void guest_run(CPU *c, uint32_t target, uint32_t stop) {
    L.run(c, target);
    while (c->pc != stop) {
        uint32_t a = c->pc & ~1u;
        if (external(a)) {                        /* a top-level tail call into an import */
            host_emu(c, c->pc);
            continue;
        }
        if (L.lookup(c->pc)) {
            L.run(c, c->pc);
            continue;
        }
        fatal("control reached 0x%08X, which is not lifted code (last lifted instruction 0x%08X)", c->pc, c->lpc);
    }
}

static uint64_t call_impl(CPU *c, uint32_t fn, int n, const uint32_t *args) {
    uint32_t saved[16], n_ = c->n, z_ = c->z, c_ = c->c, v_ = c->v, pc_ = c->pc;
    memcpy(saved, c->r, sizeof(saved));
    uint32_t sp = c->r[13];
    if (n > 4) sp -= 4u * (uint32_t)(n - 4);
    sp &= ~7u;
    for (int k = 4; k < n; k++) gw32(sp + 4u * (uint32_t)(k - 4), args[k]);
    c->r[13] = sp;
    for (int k = 0; k < n && k < 4; k++) c->r[k] = args[k];
    c->r[14] = RET_MAGIC;
    guest_run(c, fn, RET_MAGIC);
    uint64_t r = c->r[0] | ((uint64_t)c->r[1] << 32);
    memcpy(c->r, saved, sizeof(saved));
    c->n = n_; c->z = z_; c->c = c_; c->v = v_; c->pc = pc_;
    return r;
}

uint32_t guest_call(CPU *c, uint32_t fn, int n, const uint32_t *args) { return (uint32_t)call_impl(c, fn, n, args); }
uint64_t guest_call64(CPU *c, uint32_t fn, int n, const uint32_t *args) { return call_impl(c, fn, n, args); }

/* ---------------------------------------------------------------- argument readers */
VA va_regs(CPU *c, int start) { VA a = {c, start, 0, 0}; return a; }
VA va_list_at(CPU *c, uint32_t va) { VA a = {c, 0, va, 1}; return a; }

uint32_t va_u32(VA *a) {
    if (a->use_va) {
        uint32_t v = g32(a->va);
        a->va += 4;
        return v;
    }
    return ARG(a->c, a->idx++);
}

uint64_t va_u64(VA *a) {
    if (a->use_va) a->va = (a->va + 7u) & ~7u;
    else if (a->idx & 1) a->idx++;
    uint64_t lo = va_u32(a);
    return lo | ((uint64_t)va_u32(a) << 32);
}

double va_f64(VA *a) { return U2D(va_u64(a)); }
