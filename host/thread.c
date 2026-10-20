/* pthreads on host threads. Every guest thread runs lifted code on its own CPU struct and guest stack.
 * Mutexes and condition variables are host objects found by the guest object's address (bionic's are plain
 * words, zero when statically initialised). Like the emulator, every mutex is recursive. */
#include "host.h"
#include <process.h>
#include <mmsystem.h>

__declspec(thread) HThread *cur;
static HThread *threads[4096];
static volatile LONG next_tid = 100;
static SRWLOCK threads_lock = SRWLOCK_INIT;
static volatile LONG next_key = 1;
extern void host_emu(CPU *c, uint32_t target);

#define G_ETIMEDOUT 110
#define G_EBUSY 16
#define G_ESRCH 3

CPU *cpu_new(uint32_t sp) {
    CPU *c = calloc(1, sizeof(CPU));
    c->mem = G;
    c->emu = host_emu;
    c->vfp_on = 1;           /* the VFP registers live in the struct; nothing to load */
    c->r[13] = sp;
    return c;
}

void dump_threads(void) {
    for (int k = 0; k < 4096; k++) {
        HThread *t = threads[k];
        if (!t || !t->cpu || t->done) continue;
        hlog("-- thread %d (%s)", t->tid, t->name);
        dump_cpu(t->cpu);
    }
}

HThread *thread_main(void) {
    HThread *t = calloc(1, sizeof(HThread));
    t->tid = 1;
    strcpy(t->name, "main");
    /* 64 KB above the first frame (Android has argv/env/auxv there): guest code reading a little past its
       caller's frame (varargs, unterminated stack strings) must not run off the mapping when rewritten C++
       callers leave the guest stack almost empty */
    t->cpu = cpu_new(STACK_BASE + STACK_SIZE - 0x10000);
    t->errno_ptr = gzalloc(4);
    threads[1] = t;
    cur = t;
    return t;
}

/* A host thread that runs guest code (the audio callback thread): its own CPU, guest stack and errno. */
HThread *thread_host(const char *name) {
    HThread *t = calloc(1, sizeof(HThread));
    t->tid = (int)InterlockedIncrement(&next_tid);
    if (t->tid >= 4096) fatal("too many threads");
    t->stack = galloc(THREAD_STACK);
    t->cpu = cpu_new((t->stack + THREAD_STACK - 64) & ~7u);
    t->errno_ptr = gzalloc(4);
    snprintf(t->name, sizeof(t->name), "%s", name);
    AcquireSRWLockExclusive(&threads_lock);
    threads[t->tid] = t;
    ReleaseSRWLockExclusive(&threads_lock);
    cur = t;
    return t;
}

/* ---------------------------------------------------------------- creation, exit */
static unsigned __stdcall thread_entry(void *p) {
    HThread *t = p;
    cur = t;
    uint32_t args[1] = {t->arg};
    t->retval = guest_call(t->cpu, t->fn, 1, args);
    hlog("  thread %d exited", t->tid);
    shadow_thread_exit();
    InterlockedExchange(&t->done, 1);
    return 0;
}

static void i_pthread_create(CPU *c) {
    uint32_t th = c->r[0], fn = c->r[2], arg = c->r[3];
    HThread *t = calloc(1, sizeof(HThread));
    t->tid = (int)InterlockedIncrement(&next_tid);
    if (t->tid >= 4096) fatal("too many threads");
    t->stack = galloc(THREAD_STACK);
    t->cpu = cpu_new((t->stack + THREAD_STACK - 64) & ~7u);
    t->errno_ptr = gzalloc(4);
    t->fn = fn;
    t->arg = arg;
    snprintf(t->name, sizeof(t->name), "0x%08X", fn);
    AcquireSRWLockExclusive(&threads_lock);
    threads[t->tid] = t;
    ReleaseSRWLockExclusive(&threads_lock);
    gw32(th, (uint32_t)t->tid);
    hlog("  thread %d created: 0x%08X", t->tid, fn);
    t->handle = (HANDLE)_beginthreadex(NULL, 64u << 20, thread_entry, t, 0, NULL);
    if (!t->handle) fatal("cannot start a host thread");
    RET(c, 0);
}

static HThread *thread_by_tid(uint32_t tid) {
    HThread *t = NULL;
    AcquireSRWLockShared(&threads_lock);
    if (tid < 4096) t = threads[tid];
    ReleaseSRWLockShared(&threads_lock);
    return t;
}

static void i_pthread_join(CPU *c) {
    HThread *t = thread_by_tid(c->r[0]);
    if (!t || t->tid == 1) { RET(c, G_ESRCH); return; }
    WaitForSingleObject(t->handle, INFINITE);
    if (c->r[1]) gw32(c->r[1], t->retval);
    RET(c, 0);
}

static void i_pthread_self(CPU *c) { RET(c, (uint32_t)cur->tid); }
static void i_pthread_equal(CPU *c) { RET(c, c->r[0] == c->r[1]); }
static void i_ok(CPU *c) { RET(c, 0); }

/* ---------------------------------------------------------------- sync objects by guest address */
typedef struct Sync { uint32_t key; CRITICAL_SECTION cs; CONDITION_VARIABLE cv; } Sync;
#define SYNC_CAP (1u << 16)
static Sync *sync_tab[SYNC_CAP];
static SRWLOCK sync_lock = SRWLOCK_INIT;

static Sync *sync_get(uint32_t addr) {
    uint32_t h = (addr * 2654435761u) >> 16;
    AcquireSRWLockShared(&sync_lock);
    for (uint32_t k = h;; k = (k + 1) & (SYNC_CAP - 1)) {
        Sync *s = sync_tab[k];
        if (!s) break;
        if (s->key == addr) { ReleaseSRWLockShared(&sync_lock); return s; }
    }
    ReleaseSRWLockShared(&sync_lock);
    AcquireSRWLockExclusive(&sync_lock);
    uint32_t k = h, used = 0;
    for (;; k = (k + 1) & (SYNC_CAP - 1)) {
        Sync *s = sync_tab[k];
        if (!s) break;
        if (s->key == addr) { ReleaseSRWLockExclusive(&sync_lock); return s; }
        if (++used >= SYNC_CAP) fatal("too many mutexes / condition variables");
    }
    Sync *s = calloc(1, sizeof(Sync));
    s->key = addr;
    InitializeCriticalSection(&s->cs);
    InitializeConditionVariable(&s->cv);
    sync_tab[k] = s;
    ReleaseSRWLockExclusive(&sync_lock);
    return s;
}

static void i_mutex_lock(CPU *c) {
    Sync *s = sync_get(c->r[0]);
    if (!TryEnterCriticalSection(&s->cs)) {
        shadow_wait_begin();                              /* M3 verifier: do not wait holding the heap */
        EnterCriticalSection(&s->cs);
        shadow_wait_end();
    }
    RET(c, 0);
}

static void i_mutex_trylock(CPU *c) {
    RET(c, TryEnterCriticalSection(&sync_get(c->r[0])->cs) ? 0 : G_EBUSY);
}

static void i_mutex_unlock(CPU *c) {
    Sync *s = sync_get(c->r[0]);
    if (s->cs.OwningThread == (HANDLE)(uintptr_t)GetCurrentThreadId()) LeaveCriticalSection(&s->cs);
    RET(c, 0);
}

/* The wait releases the mutex completely (it is recursive) and takes it back to the same depth. */
static int cond_wait(uint32_t cond, uint32_t mutex, DWORD ms) {
    Sync *cv = sync_get(cond), *m = sync_get(mutex);
    LONG depth = m->cs.OwningThread == (HANDLE)(uintptr_t)GetCurrentThreadId() ? m->cs.RecursionCount : 0;
    if (!depth) {                                         /* waiting without holding the mutex */
        EnterCriticalSection(&m->cs);
        depth = 1;
    }
    for (LONG k = 1; k < depth; k++) LeaveCriticalSection(&m->cs);
    shadow_wait_begin();
    BOOL ok = SleepConditionVariableCS(&cv->cv, &m->cs, ms);
    shadow_wait_end();
    for (LONG k = 1; k < depth; k++) EnterCriticalSection(&m->cs);
    return ok ? 0 : G_ETIMEDOUT;
}

static void i_cond_wait(CPU *c) { RET(c, (uint32_t)cond_wait(c->r[0], c->r[1], INFINITE)); }

static void i_cond_timedwait(CPU *c) {
    /* bionic: absolute CLOCK_REALTIME */
    int32_t sec = (int32_t)g32(c->r[2]), nsec = (int32_t)g32(c->r[2] + 4);
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    int64_t now_100ns = (((int64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime) - 116444736000000000LL;
    int64_t until_100ns = (int64_t)sec * 10000000 + nsec / 100;
    int64_t ms = (until_100ns - now_100ns) / 10000;
    RET(c, (uint32_t)cond_wait(c->r[0], c->r[1], ms <= 0 ? 0 : (DWORD)ms));
}

static void i_cond_signal(CPU *c) { WakeConditionVariable(&sync_get(c->r[0])->cv); RET(c, 0); }
static void i_cond_broadcast(CPU *c) { WakeAllConditionVariable(&sync_get(c->r[0])->cv); RET(c, 0); }

/* pthread_once: 0 = not run, 1 = running, 2 = done */
static void i_pthread_once(CPU *c) {
    uint32_t ctl = c->r[0], fn = c->r[1];
    volatile long *w = (volatile long *)GP(ctl);
    glog(ctl, 4);                                         /* both stores below, undone together */
    if (_InterlockedCompareExchange(w, 1, 0) == 0) {
        GCALL0(c, fn);
        glog(ctl, 4);
        _InterlockedExchange(w, 2);
    } else {
        while (*w != 2) SwitchToThread();
    }
    RET(c, 0);
}

static void i_key_create(CPU *c) {
    LONG k = InterlockedIncrement(&next_key) - 1;
    if (k >= MAX_KEYS) fatal("too many pthread keys");
    gw32(c->r[0], (uint32_t)k);
    RET(c, 0);
}

static void i_getspecific(CPU *c) { RET(c, c->r[0] < MAX_KEYS ? cur->keys[c->r[0]] : 0); }
static void i_setspecific(CPU *c) {
    if (c->r[0] < MAX_KEYS) cur->keys[c->r[0]] = c->r[1];
    RET(c, 0);
}

/* ---------------------------------------------------------------- sleeping */
static void sleep_us(uint64_t us) {
    shadow_sleep_point();
    if (us >= 1000) { shadow_wait_begin(); Sleep((DWORD)(us / 1000)); shadow_wait_end(); }
    else SwitchToThread();
}
static void i_usleep(CPU *c) { sleep_us(c->r[0]); RET(c, 0); }
static void i_nanosleep(CPU *c) {
    uint32_t s = g32(c->r[0]), ns = g32(c->r[0] + 4);
    sleep_us((uint64_t)s * 1000000u + ns / 1000u);
    RET(c, 0);
}
static void i_sched_yield(CPU *c) { SwitchToThread(); RET(c, 0); }

void threads_init(void) {
    imp("pthread_create", i_pthread_create);
    imp("pthread_join", i_pthread_join);
    imp("pthread_self", i_pthread_self);
    imp("pthread_equal", i_pthread_equal);
    imp("pthread_detach", i_ok);
    imp("pthread_mutex_init", i_ok);
    imp("pthread_mutex_destroy", i_ok);
    imp("pthread_mutexattr_init", i_ok);
    imp("pthread_mutexattr_destroy", i_ok);
    imp("pthread_mutexattr_settype", i_ok);
    imp("pthread_cond_init", i_ok);
    imp("pthread_cond_destroy", i_ok);
    imp("pthread_mutex_lock", i_mutex_lock);
    imp("pthread_mutex_trylock", i_mutex_trylock);
    imp("pthread_mutex_unlock", i_mutex_unlock);
    imp("pthread_cond_wait", i_cond_wait);
    imp("pthread_cond_timedwait", i_cond_timedwait);
    imp("pthread_cond_signal", i_cond_signal);
    imp("pthread_cond_broadcast", i_cond_broadcast);
    imp("pthread_once", i_pthread_once);
    imp("pthread_key_create", i_key_create);
    imp("pthread_key_delete", i_ok);
    imp("pthread_getspecific", i_getspecific);
    imp("pthread_setspecific", i_setspecific);
    imp("usleep", i_usleep);
    imp("nanosleep", i_nanosleep);
    imp("sched_yield", i_sched_yield);
    timeBeginPeriod(1);
}
