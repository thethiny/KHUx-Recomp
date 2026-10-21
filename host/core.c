/* lift.dll binding, import dispatch, guest calls, heap helpers, logging. */
#include "host.h"

Lift L;
static CPU hcpu;                         /* for heap calls made by the host (never logs) */
static CRITICAL_SECTION log_lock;
static FILE *log_file;

/* ---------------------------------------------------------------- logging */
void hlog(const char *fmt, ...) {
    char buf[16384];
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

const char *rewrite_where(void);
int import_shadow_stats(int *recorded, int *replayed);
int import_shadow_divergence(const char **got);
void dump_cpu(CPU *c) {
    const char *w = rewrite_where();
    hlog("   logging=%u wn=%u%s%s", c->logging, c->wn, *w ? "; " : "", w);
    if (*w) {                                   /* inside the verifier: where the import replay stands */
        int rec, rep, div = import_shadow_stats(&rec, &rep);
        const char *got = NULL;
        int at = div ? import_shadow_divergence(&got) : 0;
        hlog("   imports recorded %d replayed %d%s%s%s", rec, rep, div ? "; diverged at " : "",
             div ? (got ? got : "?") : "", "");
        if (div) hlog("   (divergence at record #%d)", at);
    }
    hlog("   r0=%08X r1=%08X r2=%08X r3=%08X r4=%08X r5=%08X r6=%08X r7=%08X", c->r[0], c->r[1], c->r[2],
         c->r[3], c->r[4], c->r[5], c->r[6], c->r[7]);
    hlog("   r8=%08X r9=%08X r10=%08X r11=%08X r12=%08X sp=%08X lr=%08X lpc=%08X", c->r[8], c->r[9], c->r[10],
         c->r[11], c->r[12], c->r[13], c->r[14], c->lpc);
    /* Thumb return addresses on the stack: the call chain */
    uint32_t sp = c->r[13];
    int shown = 0;
    for (int k = 0; k < 1024 && shown < 24; k++) {
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
    GETP(heap_hold, "rt_heap_hold");
    GETP(files_allow, "rt_files_allow");
    { int *full = (int *)GetProcAddress(L.dll, "rt_log_full_continue"); if (full) *full = 1; }
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
    { glog(ctl, (uint32_t)(size)); memset(GP(ctl), 0, size); }
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
/* The CPU the heap stores are logged on: the calling guest thread's while it is in an M3 verifier shadow run (so
 * the run's undo also takes back what host code allocated or freed for it), the host's own otherwise. */
static CPU *heap_cpu(void) { return cur && cur->cpu && cur->cpu->logging ? cur->cpu : &hcpu; }
uint32_t galloc(uint32_t n) {
    uint32_t p = L.heap_malloc(heap_cpu(), n);
    if (!p) fatal("guest heap exhausted (%u bytes)", n);
    return p;
}
uint32_t gzalloc(uint32_t n) {
    uint32_t p = galloc(n ? n : 1);
    { glog(p, (uint32_t)(n)); memset(GP(p), 0, n); }
    return p;
}
uint32_t galloc_bytes(const void *src, uint32_t n) {
    uint32_t p = galloc(n ? n : 1);
    { glog(p, (uint32_t)(n)); memcpy(GP(p), src, n); }
    return p;
}
uint32_t galloc_str(const char *s) { return galloc_bytes(s, (uint32_t)strlen(s) + 1); }
void gfree(uint32_t p) {
    if (p && L.heap_free(heap_cpu(), p)) hlog("free of unknown pointer 0x%08X", p);
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
    /* In a logged run (the M3 verifier's shadow runs) the store is logged first, like the lifted STREX, so it is
     * undone with the rest of the run; with the log full the run is void and the store is skipped, as W32 does
     * (reported as done: failing would make the guest's retry loop spin forever). Every shared_ptr add_ref /
     * release goes through here. */
    case 0xFC0: {     /* int __kuser_cmpxchg(old, new, ptr): 0 and C set on success */
        uint32_t old = c->r[0];
        int ok = (c->logging && !wlog(c, c->r[2], 4)) ? 1
               : (uint32_t)_InterlockedCompareExchange((volatile long *)GP(c->r[2]), (long)c->r[1], (long)old) == old;
        c->r[0] = ok ? 0 : 1;
        c->c = (uint32_t)ok;
        break;
    }
    case 0xF60: {     /* int __kuser_cmpxchg64(const u64 *old, const u64 *new, u64 *ptr) */
        int64_t old = *(int64_t *)GP(c->r[0]);
        int ok = (c->logging && !(wlog(c, c->r[2], 4) && wlog(c, c->r[2] + 4, 4))) ? 1
               : _InterlockedCompareExchange64((volatile int64_t *)GP(c->r[2]), *(int64_t *)GP(c->r[1]), old) == old;
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
EXPORT int rt_log_full_continue = 1;   /* rt.h wlog in host code: a full log stops logging, the run goes on */

/* ---------------------------------------------------------------- M3 verifier: logged host writes */
void glog(uint32_t a, uint32_t n) {
    CPU *c = cur ? cur->cpu : NULL;
    if (!c || !c->logging || !n) return;
    if ((uint64_t)c->wn + n / 4 + 4 > c->wcap) { c->wfull = 1; c->logging = 0; return; }   /* as rt.h wlog */
    uint32_t k = 0;
    for (; k + 4 <= n; k += 4) {
        WLog *w = &c->wlog[c->wn++];
        w->addr = a + k; w->size = 4; w->old_lo = g32(a + k); w->old_hi = c->lpc;
    }
    for (; k < n; k++) {
        WLog *w = &c->wlog[c->wn++];
        w->addr = a + k; w->size = 1; w->old_lo = *(uint8_t *)GP(a + k); w->old_hi = c->lpc;
    }
}

/* ---------------------------------------------------------------- M3 verifier: import record / replay
 * A shadow run executes a function twice (the lifted original, then its rewrite) and undoes guest memory between
 * the runs. State the host keeps outside guest memory (mutexes, threads, files, GL, JNI, sockets, audio) cannot be
 * undone, so on the shadowed thread every such import runs for real only in the first run and is recorded (return
 * registers); the later runs get the recorded results without running it. Pure imports (string, formatting, math,
 * sorting: they only read and write guest memory) run every time. A replay that asks for a different import, or
 * more imports than were recorded, marks the shadow run as diverged (the verifier then reports a mismatch). */
typedef struct ImpRec { uint32_t addr, r0, r1, nbytes, args[4], sargs[6]; uint64_t *bytes; } ImpRec;   /* bytes: (addr << 8) | value; sargs: stack arguments (GL) */
static __declspec(thread) int imp_mode;                 /* 0 off, 1 record, 2 replay */
static int imp_replay_only;                            /* the replay sandbox (KHUX_RW_REPLAY) */
static __declspec(thread) ImpRec *imp_recs;
static __declspec(thread) uint32_t imp_div_got, imp_div_args[6];
static __declspec(thread) int imp_div_pos, imp_opaque, imp_paused, imp_saved_mode;
static __declspec(thread) int imp_n, imp_cap, imp_pos, imp_diverged;

static int import_pure(const char *n) {
    static const char *const prefixes[] = {"str", "mem", "wcs", "wmem", "ato", "sprintf", "snprintf", "vsnprintf",
        "vsprintf", "sscanf", "vsscanf", "is", "to", "abs", "labs", "llabs", "div", "ldiv", "sin", "cos", "tan",
        "asin", "acos", "atan", "sqrt", "pow", "exp", "log", "floor", "ceil", "fmod", "round", "lround", "trunc",
        "modf", "frexp", "ldexp", "fabs", "fmin", "fmax", "hypot", "cbrt", "qsort", "bsearch", "__aeabi_", "__ctype",
        "__errno", "__stack_chk", "bzero", "index", "rindex", "mblen", "mbtowc", "wctomb", "btowc", "wctob",
        /* call back into guest code: replaying would skip the callback's own (undone) stores, so they run again */
        "pthread_once",
        /* clocks: frozen during shadow runs (libc_shadow), so every run reads the same time */
        "gettimeofday", "time", "clock_gettime", "clock", 0};
    for (int k = 0; prefixes[k]; k++)
        if (!strncmp(n, prefixes[k], strlen(prefixes[k]))) return 1;
    return 0;
}

/* OpenGL ES in a shadow run (KHUX_RW_GLREPLAY, default on; 0 = every run that draws is opaque, as before): recorded
 * and replayed like the other imports. The GL calls run for real once, in the lifted run; the rewrite's run must
 * make the same calls with the same arguments (a value, or a pointer that is a stack address in both runs) and gets
 * the recorded results. lift.dll's GL code writes guest memory through host pointers, unlogged, so the outputs
 * (glGen*, glGet*) are logged here; calls whose outputs are not known (glReadPixels, buffer mapping) stay opaque. */
#include "gl_kinds.h"
static int gl_replay_on(void) {
    static int on = -1;
    if (on < 0) on = !getenv("KHUX_RW_GLREPLAY") || strcmp(getenv("KHUX_RW_GLREPLAY"), "0") != 0;
    return on;
}
static const char *gl_kinds(const char *n) {
    for (int k = 0; GL_KINDS[k].name; k++) if (!strcmp(GL_KINDS[k].name, n)) return GL_KINDS[k].kinds;
    return NULL;
}
static uint32_t gl_arg(const CPU *c, int k) { return k < 4 ? c->r[k] : g32(c->r[13] + 4u * (uint32_t)(k - 4)); }

/* glGet{Integer,Float,Boolean}v: how many values a pname returns (0 = not known here) */
static uint32_t gl_get_count(uint32_t pname) {
    switch (pname) {
    case 0x0BA2: case 0x0C10: case 0x0C22: case 0x0C23: case 0x8005: return 4;   /* viewport, scissor box, clear
                                                                                   * colour, colour mask, blend colour */
    case 0x0D3A: case 0x846D: case 0x846E: case 0x0B70: return 2;              /* max viewport dims, point / line
                                                                                   * ranges, depth range */
    case 0x86A2: case 0x8DF9: return 0;                                         /* format lists: any length */
    default: return 1;
    }
}

/* the guest buffers a GL call writes: {address, size} pairs, recorded whole; -1 = unknown (the run stays opaque) */
static int gl_outputs(const CPU *c, const char *n, uint32_t out[][2]) {
    int m = 0;
#define OUT_(k, size) do { out[m][0] = gl_arg(c, k); out[m][1] = (size); m++; } while (0)
    if (!strncmp(n, "glGen", 5) && strcmp(n, "glGenerateMipmap")) OUT_(1, 4u * c->r[0]);
    else if (!strcmp(n, "glGetIntegerv") || !strcmp(n, "glGetFloatv") || !strcmp(n, "glGetBooleanv")) {
        uint32_t k = gl_get_count(c->r[0]);
        if (!k) return -1;
        OUT_(1, k * (n[5] == 'B' ? 1u : 4u));
    }
    else if (!strcmp(n, "glGetShaderiv") || !strcmp(n, "glGetProgramiv") || !strcmp(n, "glGetBufferParameteriv")) OUT_(2, 4);
    else if (!strcmp(n, "glGetShaderInfoLog") || !strcmp(n, "glGetProgramInfoLog") || !strcmp(n, "glGetShaderSource")) {
        OUT_(2, 4); OUT_(3, c->r[1]);
    } else if (!strcmp(n, "glGetActiveAttrib") || !strcmp(n, "glGetActiveUniform")) {
        OUT_(3, 4); OUT_(4, 4); OUT_(5, 4); OUT_(6, c->r[2]);
    } else if (!strcmp(n, "glReadPixels") || !strcmp(n, "glMapBufferOES") || !strcmp(n, "glUnmapBufferOES")) return -1;
#undef OUT_
    for (int k = 0; k < m; k++) {
        if (!out[k][0]) out[k][1] = 0;
        if (out[k][1] > 0x10000u) return -1;
    }
    return m;
}

/* gl_outputs' text buffers: an info log / shader source (output 1), an active attribute / uniform name (output 3) */
static int n_is_text(const char *n, int k) {
    if (!strcmp(n, "glGetShaderInfoLog") || !strcmp(n, "glGetProgramInfoLog") || !strcmp(n, "glGetShaderSource")) return k == 1;
    if (!strcmp(n, "glGetActiveAttrib") || !strcmp(n, "glGetActiveUniform")) return k == 3;
    return 0;
}

/* the rewrite's GL call against the recorded one: every argument equal, or a stack address in both runs */
static int gl_args_match(const ImpRec *r, const CPU *c, const char *kinds) {
    uint32_t sp = c->r[13], sp0 = r->sargs[5];
    for (int k = 0; kinds[k] && k < 10; k++) {
        uint32_t was = k < 4 ? r->args[k] : r->sargs[k - 4], now = gl_arg(c, k);
        if (was == now) continue;
        if (kinds[k] == 'p' && was - (sp0 - 0x10000u) < 0x20000u && now - (sp - 0x10000u) < 0x20000u) continue;
        return 0;
    }
    return 1;
}

/* debugging: how many imports the first run recorded / the last replay consumed, and the names */
int import_shadow_stats(int *recorded, int *replayed) { *recorded = imp_n; *replayed = imp_pos; return imp_diverged; }
void import_shadow_opaque(void) { if (imp_mode != 2) imp_opaque = 1; }

/* where the replay diverged: the record index, and the import that came instead of the recorded one */
int import_shadow_divergence(const char **got) { *got = imp_div_got ? import_name(imp_div_got) : NULL; return imp_div_pos; }
/* the divergence's arguments (r0-r3, two stack words): recorded (at the record) and called */
void import_shadow_divergence_args(uint32_t rec[6], uint32_t got[6]) {
    for (int k = 0; k < 6; k++) {
        got[k] = imp_div_args[k];
        rec[k] = imp_div_pos < imp_n ? (k < 4 ? imp_recs[imp_div_pos].args[k] : imp_recs[imp_div_pos].sargs[k - 4]) : 0;
    }
}
const char *import_shadow_name(int k) { return k < imp_n ? import_name(imp_recs[k].addr) : NULL; }

/* Where a recorded store goes in a replay: a store into the buffer of a pointer argument (at most 64 KB past it)
 * moves with that argument, which differs when the caller's stack frame does (a rewrite's frame is not the original's
 * layout); any other store (heap blocks, globals) keeps its address. */
static uint32_t replay_addr(const ImpRec *r, const CPU *c, uint32_t a) {
    /* r0-r3, then a GL call's stack arguments (sargs[0..4]: glGetActiveUniform's size / type / name buffers are
     * its 5th-7th arguments - parseUniforms' alloca'd name moved with the frame and was never replayed into it) */
    int best = -1;
    uint32_t bp = 0;
    for (int k = 0; k < 9; k++) {
        uint32_t p = k < 4 ? r->args[k] : r->sargs[k - 4];
        if (p >= 0x1000u && a >= p && a - p < 0x10000u && (best < 0 || p > bp)) { best = k; bp = p; }
    }
    return best < 0 ? a : a - bp + gl_arg(c, best);
}

/* M3 verifier, a shadow run on another thread than the main one (the sound thread): the main thread parks at its
 * next frame boundary for the whole run, so it never sees the lifted run's stores before the undo (a half-undone
 * sound bank gave the main thread garbage pointers). Phase 7 asks (1 = parked, or this is the main thread; 0 = the
 * main thread did not park within 50 ms - it may wait on a lock this thread holds: run without a shadow), 8 ends. */
static DWORD main_tid;
static CRITICAL_SECTION excl_cs;
static volatile LONG excl_req, main_parked, cb_busy, cb_waiting;

/* the audio callback thread runs guest code too (it decodes the voices a shadow run starts): it waits while a
 * shadow run on another thread is in progress, and that run waits until no callback is running - on another
 * thread: a shadow run inside the callback itself (the game's mixer) does not wait for its own callback */
static __declspec(thread) LONG cb_own;
void shadow_guest_enter(void) {
    for (;;) {
        while (excl_req) SwitchToThread();
        InterlockedIncrement(&cb_busy);
        if (!excl_req) { cb_own++; return; }
        InterlockedDecrement(&cb_busy);
    }
}
void shadow_guest_leave(void) { cb_own--; InterlockedDecrement(&cb_busy); }

void shadow_main_thread(void) { main_tid = GetCurrentThreadId(); InitializeCriticalSection(&excl_cs); }

/* Guest threads that sleep (the SQEX sound thread's frame loop) park at their sleeps while a shadow run on another
 * thread is in progress: they hold no lock there, and they would change the state the run reads between its lifted
 * and rewritten sides (a source the mixer renders, then finds finished). */
static volatile LONG sleepers, sleepers_parked;
static __declspec(thread) int sleeper;
static volatile DWORD excl_tid;
void shadow_sleep_point(void) {
    if (!main_tid || GetCurrentThreadId() == main_tid) return;
    if (!sleeper) { sleeper = 1; InterlockedIncrement(&sleepers); }
    if (!excl_req || excl_tid == GetCurrentThreadId()) return;
    InterlockedIncrement(&sleepers_parked);
    while (excl_req) SwitchToThread();
    InterlockedDecrement(&sleepers_parked);
}
/* a thread that slept ends: no longer waited for */
void shadow_thread_exit(void) {
    if (sleeper) { sleeper = 0; InterlockedDecrement(&sleepers); }
}

static volatile LONG main_looping;   /* the main thread reached its park point (the frame loop): it can park */
void shadow_park_point(void) {
    main_looping = 1;
    if (!excl_req) return;
    InterlockedExchange(&main_parked, 1);
    while (excl_req) SwitchToThread();
    InterlockedExchange(&main_parked, 0);
}

/* M3 replay sandbox: while the main thread captures a call (mem_track), the other guest threads park the same way
 * (sleepers at their sleeps, the audio callback at its entry), so the saved pages are the state the call read and
 * not the sound thread's. At most 50 ms; when they do not park, the capture goes on (marked by its foreign pages). */
void shadow_capture_park(int on) {
    if (!main_tid || GetCurrentThreadId() != main_tid) return;
    if (on) {
        EnterCriticalSection(&excl_cs);
        excl_tid = main_tid;
        InterlockedExchange(&excl_req, 1);
        for (int k = 0; k < 50 && (cb_busy - cb_waiting > 0 || sleepers_parked < sleepers); k++) Sleep(1);
    } else {
        InterlockedExchange(&excl_req, 0);
        LeaveCriticalSection(&excl_cs);
    }
}

static int shadow_exclusive(int begin) {
    if (!main_tid || GetCurrentThreadId() == main_tid) return 1;
    /* by default other threads do not shadow-run at all (the original runs, not compared): the sound thread's voice
     * start-up raced with the undo even with the main and audio threads parked. KHUX_RW_THREADS=1 tries parking. */
    static int threads = -1;
    if (threads < 0) threads = getenv("KHUX_RW_THREADS") && strcmp(getenv("KHUX_RW_THREADS"), "0") != 0;
    if (!threads) return 0;
    /* before the frame loop the main thread never parks (boot waits on the sound thread, which a request would
     * keep parked at its sleeps): no request at all */
    if (begin && !main_looping) return 0;
    /* after a request that failed, none from this thread for 200 ms: the parked sleepers (the main thread may wait on
     * them) run */
    static __declspec(thread) DWORD retry_at;
    if (begin && (LONG)(GetTickCount() - retry_at) < 0) return 0;
    if (!begin) {
        InterlockedExchange(&excl_req, 0);
        while (main_parked) SwitchToThread();
        LeaveCriticalSection(&excl_cs);
        return 0;
    }
    /* waiting for another thread's request to end, this one runs nothing: it counts as parked */
    if (sleeper) InterlockedIncrement(&sleepers_parked);
    InterlockedAdd(&cb_waiting, cb_own);
    EnterCriticalSection(&excl_cs);
    InterlockedAdd(&cb_waiting, -cb_own);
    if (sleeper) InterlockedDecrement(&sleepers_parked);
    excl_tid = GetCurrentThreadId();
    InterlockedExchange(&excl_req, 1);
    for (int k = 0; k < 50 && (!main_parked || cb_busy - cb_waiting > cb_own || sleepers_parked < sleepers - sleeper); k++)
        Sleep(1);
    if (main_parked && cb_busy - cb_waiting == cb_own && sleepers_parked == sleepers - sleeper) return 1;
    { static int told; if (told++ < 5) hlog("verifier: thread %lu could not run alone (main parked %ld, callbacks %ld/%ld own, "
                                         "sleepers parked %ld of %ld, own %d)", GetCurrentThreadId(), main_parked,
                                         cb_busy - cb_waiting, cb_own, sleepers_parked, sleepers, sleeper); }
    retry_at = GetTickCount() + 200;
    InterlockedExchange(&excl_req, 0);
    while (main_parked) SwitchToThread();
    LeaveCriticalSection(&excl_cs);
    return 0;
}

/* M3 replay sandbox: the lifted run's import records as bytes (capture), and loading them back so that both runs of
 * a replayed call get them (KHUX_RW_REPLAY). Layout per record: addr r0 r1 nbytes args[4] sargs[6], bytes[nbytes]. */
void libc_shadow_state(double *real, double *mono, uint64_t *rnd);
void libc_shadow_force(double real, double mono, uint64_t rnd);
size_t import_shadow_export(uint8_t **out) {
    size_t n = 32;                                      /* "CLK1", pad, then the frozen clocks and the random state */
    for (int k = 0; k < imp_n; k++) n += 4u * 14 + 8u * imp_recs[k].nbytes;
    uint8_t *b = malloc(n), *p = b;
    {
        double real, mono; uint64_t rnd;
        libc_shadow_state(&real, &mono, &rnd);
        memcpy(p, "CLK1\0\0\0", 8); memcpy(p + 8, &real, 8); memcpy(p + 16, &mono, 8); memcpy(p + 24, &rnd, 8);
        p += 32;
    }
    for (int k = 0; k < imp_n; k++) {
        const ImpRec *r = &imp_recs[k];
        uint32_t h[14] = {r->addr, r->r0, r->r1, r->nbytes, r->args[0], r->args[1], r->args[2], r->args[3],
                          r->sargs[0], r->sargs[1], r->sargs[2], r->sargs[3], r->sargs[4], r->sargs[5]};
        memcpy(p, h, sizeof h); p += sizeof h;
        if (r->nbytes) { memcpy(p, r->bytes, 8u * r->nbytes); p += 8u * r->nbytes; }
    }
    *out = b;
    return n;
}
int import_shadow_import(const uint8_t *b, size_t n) {
    for (int k = 0; k < imp_n; k++) free(imp_recs[k].bytes);
    imp_n = 0;
    const uint8_t *p = b, *end = b + n;
    if (n >= 32 && !memcmp(p, "CLK1", 4)) {
        double real, mono; uint64_t rnd;
        memcpy(&real, p + 8, 8); memcpy(&mono, p + 16, 8); memcpy(&rnd, p + 24, 8);
        libc_shadow_force(real, mono, rnd);
        p += 32;
    }
    while (p + 56 <= end) {
        uint32_t h[14];
        memcpy(h, p, sizeof h); p += sizeof h;
        if (imp_n == imp_cap) { imp_cap = imp_cap ? imp_cap * 2 : 64; imp_recs = realloc(imp_recs, sizeof(ImpRec) * (size_t)imp_cap); }
        ImpRec *r = &imp_recs[imp_n++];
        r->addr = h[0]; r->r0 = h[1]; r->r1 = h[2]; r->nbytes = h[3];
        for (int k = 0; k < 4; k++) r->args[k] = h[4 + k];
        for (int k = 0; k < 6; k++) r->sargs[k] = h[8 + k];
        r->bytes = NULL;
        if (r->nbytes) { r->bytes = malloc(8u * r->nbytes); memcpy(r->bytes, p, 8u * r->nbytes); p += 8u * r->nbytes; }
    }
    imp_replay_only = 1;
    return imp_n;
}

/* phase 0: start recording; 1: replay from the first record; 2: end; 3: query (1 = the replay diverged) */
int import_shadow(int phase) {
    if (phase == 7) return shadow_exclusive(1);
    if (phase == 8) return shadow_exclusive(0);
    if (phase == 4) return imp_opaque;
    if (phase == 5) {                                     /* an effect runs: its imports are part of it (not recorded) */
        /* it runs for real, once (replayed after), so lift.dll's streams may run although the run logs: a save
         * reads UserDefault.xml (fopen declined in logged runs otherwise) */
        if (imp_paused++ == 0) { imp_saved_mode = imp_mode; imp_mode = 0; L.files_allow(1); }
        return 0;
    }
    if (phase == 6) {
        if (imp_paused && --imp_paused == 0) { imp_mode = imp_saved_mode; L.files_allow(0); }
        return 0;
    }
    if (phase == 0 && imp_replay_only) {                 /* replay sandbox: both runs replay the loaded record */
        imp_opaque = 0; imp_mode = 2; imp_pos = 0; imp_diverged = 0;
        return 0;
    }
    if (phase == 0) {
        imp_opaque = 0;
        for (int k = 0; k < imp_n; k++) free(imp_recs[k].bytes);
        imp_mode = 1; imp_n = 0; imp_diverged = 0;
    }
    else if (phase == 1) { imp_mode = 2; imp_pos = 0; imp_diverged = 0; }
    else if (phase == 2) { imp_mode = 0; }
    return phase == 3 ? (imp_diverged || (imp_mode == 2 && imp_pos != imp_n)) : 0;
}

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
    if (!fn && !imp_replay_only) {                /* the replay sandbox answers imports from the record */
        const char *name = import_name(a);
        if (name) fatal("unimplemented import %s called from 0x%08X (r0=%08X r1=%08X)", name, lr, c->r[0], c->r[1]);
        fatal("call to 0x%08X from 0x%08X: neither lifted code nor an import", target, lr);
    }
    if (imp_mode) {
        const char *name = import_name(a);
        const char *kinds = name && !strncmp(name, "gl", 2) && gl_replay_on() ? gl_kinds(name) : NULL;
        uint32_t outs[4][2];
        int nouts = kinds ? gl_outputs(c, name, outs) : -1;
        if (nouts < 0) kinds = NULL;
        if (imp_mode == 1 && name && !kinds && (!strncmp(name, "gl", 2) || !strncmp(name, "egl", 3))) {
            /* OpenGL ES: runs for real, once; its outputs are not logged, so this shadow run cannot be compared or
             * replayed: the verifier keeps it as the real call (imp_opaque) */
            imp_opaque = 1;
            {   /* once per import: which calls keep shadow runs opaque */
                static const char *seen[64];
                int k = 0;
                while (k < 64 && seen[k] && seen[k] != name) k++;
                if (k < 64 && !seen[k]) { seen[k] = name; hlog("verifier: %s is not replayable (the shadow run stays opaque)", name); }
            }
            int id = native_id(name), done = 0;
            if (id >= 0) { L.files_allow(1); done = L.native_call(c, id); L.files_allow(0); }
            if (!done) fn(c);
            c->pc = lr;
            return;
        }
        if (!name || !import_pure(name)) {
            if (imp_mode == 2) {                          /* replay: the recorded result, nothing runs */
                ImpRec *r = NULL;
                if (imp_pos < imp_n && imp_recs[imp_pos].addr == a && (!kinds || gl_args_match(&imp_recs[imp_pos], c, kinds)))
                    r = &imp_recs[imp_pos++];
                else {
                    if (!imp_diverged) {
                        imp_div_pos = imp_pos; imp_div_got = a;
                        for (int k = 0; k < 6; k++) imp_div_args[k] = k < 4 ? c->r[k] : g32(c->r[13] + 4u * (uint32_t)(k - 4));
                    }
                    imp_diverged = 1;
                    /* diverged (a mismatch either way): the rewrite run goes on with a recorded call of the same
                     * import and arguments when there is one (the next, else any), so it does not crash on a 0
                     * from an import such as pthread_getspecific; the next ones resync after it */
                    for (int pass = 0; pass < 2 && !r; pass++)
                        for (int k = pass ? 0 : imp_pos; k < imp_n && !r; k++) {
                            ImpRec *q = &imp_recs[k];
                            if (q->addr != a) continue;
                            if (kinds ? !gl_args_match(q, c, kinds) : memcmp(q->args, c->r, sizeof q->args)) continue;
                            r = q;
                            if (!pass) imp_pos = k + 1;
                        }
                }
                if (r) {
                    for (uint32_t k = 0; k < r->nbytes; k++)
                        gw8(replay_addr(r, c, (uint32_t)(r->bytes[k] >> 8)), (uint8_t)r->bytes[k]);
                    c->r[0] = r->r0;
                    c->r[1] = r->r1;
                } else c->r[0] = 0;
                c->pc = lr;
                return;
            }
            int mode = imp_mode;
            imp_mode = 0;                                 /* what the import calls back into runs normally */
            uint32_t outer = imp_target, w0 = c->wn, args_in[4] = {c->r[0], c->r[1], c->r[2], c->r[3]};
            imp_target = a;
            /* lift.dll's own stream functions decline inside logged runs: here they may run (once, recorded) */
            uint32_t sargs[6] = {0};
            uint8_t *pre[4] = {0};
            if (kinds) {
                for (int k = 4; kinds[k] && k < 9; k++) sargs[k - 4] = gl_arg(c, k);
                sargs[5] = c->r[13];
                for (int k = 0; k < nouts; k++)
                    if (outs[k][1]) { pre[k] = (uint8_t *)malloc(outs[k][1]); memcpy(pre[k], GP(outs[k][0]), outs[k][1]); }
            }
            int id = name ? native_id(name) : -1, done = 0;
            if (id >= 0) { L.files_allow(1); done = L.native_call(c, id); L.files_allow(0); }
            if (!done) fn(c);
            if (kinds)                                    /* GL's unlogged output stores, logged (undone, replayed) */
                for (int k = 0; k < nouts; k++) {
                    if (!pre[k]) continue;
                    /* every output byte (a replay into a buffer holding other bytes must still write them all);
                     * an info log or name: its text and terminator only */
                    uint32_t size = outs[k][1];
                    if (n_is_text(name, k)) {
                        uint32_t len = 0;
                        while (len + 1 < size && *(uint8_t *)GP(outs[k][0] + len)) len++;
                        size = len + 1 < size ? len + 1 : size;
                    }
                    for (uint32_t b = 0; b < size; b++) {
                        uint8_t v = *(uint8_t *)GP(outs[k][0] + b);
                        *(uint8_t *)GP(outs[k][0] + b) = pre[k][b];
                        gw8(outs[k][0] + b, v);
                    }
                    free(pre[k]);
                }
            imp_target = outer;
            imp_mode = mode;
            /* the guest bytes it stored (final values), replayed in the later runs */
            uint32_t w1 = c->wn < c->wcap ? c->wn : c->wcap, nb = 0;
            for (uint32_t k = w0; k < w1; k++) nb += c->wlog[k].size;
            uint64_t *bytes = nb ? (uint64_t *)malloc(8u * nb) : NULL;
            nb = 0;
            for (uint32_t k = w0; k < w1; k++)
                for (uint32_t b = 0; b < c->wlog[k].size; b++) {
                    uint32_t ga = c->wlog[k].addr + b;
                    bytes[nb++] = ((uint64_t)ga << 8) | *(uint8_t *)GP(ga);
                }
            if (imp_n == imp_cap) {
                imp_cap = imp_cap ? imp_cap * 2 : 256;
                imp_recs = (ImpRec *)realloc(imp_recs, sizeof(ImpRec) * (size_t)imp_cap);
            }
            imp_recs[imp_n].addr = a;
            imp_recs[imp_n].r0 = c->r[0];
            imp_recs[imp_n].r1 = c->r[1];
            imp_recs[imp_n].nbytes = nb;
            memcpy(imp_recs[imp_n].args, args_in, sizeof args_in);
            memcpy(imp_recs[imp_n].sargs, sargs, sizeof sargs);
            imp_recs[imp_n].bytes = bytes;
            imp_n++;
            c->pc = lr;
            return;
        }
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

/* Like a compiled `bl`: sp is left exactly as it is (no 8-byte realignment) and arguments 5+ are stored at
 * [sp, sp + 4 * (n - 4)) - the caller's outgoing-argument area, as the original code laid it out. Used by
 * rewritten code that reproduces the original frame layout exactly (rw::call_exact). */
uint64_t guest_call_exact(CPU *c, uint32_t fn, int n, const uint32_t *args) {
    uint32_t saved[16], n_ = c->n, z_ = c->z, c_ = c->c, v_ = c->v, pc_ = c->pc;
    memcpy(saved, c->r, sizeof(saved));
    uint32_t sp = c->r[13];
    for (int k = 4; k < n; k++) gw32(sp + 4u * (uint32_t)(k - 4), args[k]);
    for (int k = 0; k < n && k < 4; k++) c->r[k] = args[k];
    c->r[14] = RET_MAGIC;
    guest_run(c, fn, RET_MAGIC);
    uint64_t r = c->r[0] | ((uint64_t)c->r[1] << 32);
    memcpy(c->r, saved, sizeof(saved));
    c->n = n_; c->z = z_; c->c = c_; c->v = v_; c->pc = pc_;
    return r;
}
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
