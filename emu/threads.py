"""Cooperative guest threads on a single Unicorn instance.

Every guest thread has its own stack, saved CPU context, errno, pthread TLS and kuser TLS. Worker threads
run in instruction-count slices from the main loop (Scheduler.run_workers). A handler that would block
raises Block(predicate): the thread's PC is left on the import's trap slot, so when the predicate becomes
true the import is simply executed again and completes.

If the *main* thread blocks (e.g. on a mutex a preempted worker holds), the workers are run nested from
inside the handler until the predicate holds; then the main thread retries the import.
"""
import time

from unicorn.arm_const import UC_ARM_REG_CPSR, UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0, UC_ARM_REG_SP

from .core import GuestFault
from .libc import imp

SLICE_INSNS = 200_000
STACK_SIZE = 1 << 20
ETIMEDOUT, EBUSY = 110, 16


class Block(Exception):
    def __init__(self, ready, why):
        self.ready, self.why = ready, why


class Thread:
    def __init__(self, tid, name):
        self.tid, self.name = tid, name
        self.ctx = None
        self.done = False
        self.blocked = None      # Block while waiting
        self.errno_ptr = None
        self.tls_keys = {}
        self.kuser_tls = None
        self.wait = None         # per-import resumable wait state
        self.retval = 0
        self.start = None
        self.slices = 0


class Scheduler:
    def __init__(self, emu):
        self.emu = emu
        main = Thread(1, "main")
        main.errno_ptr = emu.libc.errno_ptr
        main.tls_keys = emu.libc.tls_keys
        self.threads = {1: main}
        self.current = main
        self.next_tid = 100
        self.exit_slot = emu.host_function("<thread exit>", self._thread_exit)
        self.mutex_owner = {}    # mutex addr -> (tid, depth)
        self.cond_seq = {}       # cond addr -> signal counter
        self.nested = False

    # ------------------------------------------------------------------ per-thread state
    def _activate(self, t):
        e = self.emu
        self.current = t
        e.libc.errno_ptr = t.errno_ptr
        e.libc.tls_keys = t.tls_keys
        e.tls = t.kuser_tls

    def _deactivate(self, t):
        t.kuser_tls = self.emu.tls

    # ------------------------------------------------------------------ creation / exit
    def create(self, fn, arg):
        e = self.emu
        t = Thread(self.next_tid, e.sym(fn))
        self.next_tid += 1
        stack = e.heap.malloc(STACK_SIZE)
        t.errno_ptr = e.alloc(4)
        # build the new thread's context by temporarily editing registers; PC is not touched here
        # (writing PC from inside a hook acts as a jump), it is patched into the saved context instead
        regs = (UC_ARM_REG_SP, UC_ARM_REG_R0, UC_ARM_REG_LR, UC_ARM_REG_CPSR)
        saved = [e.reg(r) for r in regs]
        cpsr = saved[3]
        e.set_reg(UC_ARM_REG_SP, stack + STACK_SIZE - 64)
        e.set_reg(UC_ARM_REG_R0, arg)
        e.set_reg(UC_ARM_REG_LR, self.exit_slot)
        e.set_reg(UC_ARM_REG_CPSR, (cpsr | 0x20) if fn & 1 else (cpsr & ~0x20))
        t.ctx = e.uc.context_save()
        t.start = fn          # first slice starts here (emu_start's begin sets PC and Thumb state)
        for r, v in zip(regs, saved):
            e.set_reg(r, v)
        self.threads[t.tid] = t
        e.log(f"  thread {t.tid} created: {e.sym(fn)}")
        return t.tid

    def _thread_exit(self, e):
        t = self.current
        t.done = True
        t.retval = e.reg(UC_ARM_REG_R0)
        e.log(f"  thread {t.tid} exited")
        e.uc.emu_stop()

    # ------------------------------------------------------------------ running
    def runnable(self):
        return [t for t in self.threads.values()
                if t.tid != 1 and not t.done and (not t.blocked or t.blocked.ready())]

    def run_workers(self, budget_s=0.03):
        """Main loop, between frames: run workers round-robin until they all block or the time budget is
        used (CPU-heavy work such as MasterDataManager::save needs many slices)."""
        deadline = time.perf_counter() + budget_s
        while time.perf_counter() < deadline:
            ready = self.runnable()
            if not ready:
                break
            for t in ready:
                self._run_slice(t)
                if time.perf_counter() >= deadline:
                    break

    def status(self):
        out = []
        for t in self.threads.values():
            st = "done" if t.done else ("blocked: " + t.blocked.why if t.blocked and not t.blocked.ready()
                                        else "runnable")
            pc = None
            if t.ctx is not None and t.tid != 1 and not t.done:
                try:
                    pc = self.emu.sym(t.ctx.reg_read(UC_ARM_REG_PC))
                except Exception:
                    pc = None
            out.append({"tid": t.tid, "entry": t.name, "state": st, "slices": t.slices, "pc": pc})
        return out

    def _run_slice(self, t):
        e = self.emu
        prev = self.current
        main_ctx = e.uc.context_save()
        self._deactivate(prev)
        self._activate(t)
        t.blocked = None
        t.slices += 1
        e.uc.context_restore(t.ctx)
        if t.start is not None:
            pc, t.start = t.start, None
        else:
            pc = e.reg(UC_ARM_REG_PC) | (1 if e.reg(UC_ARM_REG_CPSR) & 0x20 else 0)
        e.fault = None
        try:
            e.uc.emu_start(pc, 0xFFFFFFFF, count=SLICE_INSNS)
        except Exception as ex:
            if not e.fault:
                e.fault = f"{ex} at pc={e.reg(UC_ARM_REG_PC):#x}"
        t.ctx = e.uc.context_save()
        self._deactivate(t)
        self._activate(prev)
        e.uc.context_restore(main_ctx)
        if e.fault:
            msg = e.fault
            e.fault = None
            raise GuestFault(f"[thread {t.tid}] {msg}")

    def block(self, e, blk):
        """Called by the trap hook when a handler raised Block."""
        t = self.current
        if t.tid != 1:
            # worker: park on the trap slot (PC still points at it) and stop the slice
            t.blocked = blk
            e.uc.emu_stop()
            return
        # main thread: run the workers until the condition holds, then retry the import
        if self.nested:
            raise GuestFault(f"main thread blocked re-entrantly on {blk.why}")
        self.nested = True
        try:
            give_up = time.time() + 30
            while not blk.ready():
                runnable = self.runnable()
                if not runnable:
                    time.sleep(0.001)
                for w in runnable:
                    self._run_slice(w)
                if time.time() > give_up:
                    raise GuestFault(f"main thread deadlocked on {blk.why}")
        finally:
            self.nested = False
        # PC is the trap slot; writing it re-executes the import, which now succeeds
        e.set_reg(UC_ARM_REG_PC, e.reg(UC_ARM_REG_PC))


def install(emu):
    emu.sched = Scheduler(emu)


def _blocking(e, ready, why):
    if not ready():
        raise Block(ready, why)


# ---------------------------------------------------------------------------------------- pthreads
@imp("pthread_create")
def _pthread_create(e):
    th, attr, fn, arg = e.args(4)
    tid = e.sched.create(fn, arg)
    e.w32(th, tid)
    e.ret(0)


@imp("pthread_self")
def _pthread_self(e):
    e.ret(e.sched.current.tid)


@imp("pthread_equal")
def _pthread_equal(e):
    a, b = e.args(2)
    e.ret(int(a == b))


@imp("pthread_detach")
def _pthread_detach(e):
    e.ret(0)


@imp("pthread_join")
def _pthread_join(e):
    tid, out = e.args(2)
    t = e.sched.threads.get(tid)
    if t is None:
        e.ret(3)  # ESRCH
        return
    _blocking(e, lambda: t.done, f"join {tid}")
    if out:
        e.w32(out, t.retval)
    e.ret(0)


# mutexes: every mutex is treated as recursive (never self-deadlocks)
@imp("pthread_mutex_init", "pthread_mutex_destroy", "pthread_mutexattr_init", "pthread_mutexattr_destroy",
     "pthread_mutexattr_settype", "pthread_cond_init", "pthread_cond_destroy")
def _noop(e):
    e.ret(0)


def _mutex_free_for(s, m, tid):
    owner = s.mutex_owner.get(m)
    return owner is None or owner[0] == tid or s.threads.get(owner[0], None) is None \
        or s.threads[owner[0]].done


def _lock(s, m, tid):
    owner = s.mutex_owner.get(m)
    if owner and owner[0] == tid:
        s.mutex_owner[m] = (tid, owner[1] + 1)
    else:
        s.mutex_owner[m] = (tid, 1)


@imp("pthread_mutex_lock")
def _mutex_lock(e):
    m = e.args(1)[0]
    s, tid = e.sched, e.sched.current.tid
    _blocking(e, lambda: _mutex_free_for(s, m, tid), f"mutex {m:#x}")
    _lock(s, m, tid)
    e.ret(0)


@imp("pthread_mutex_trylock")
def _mutex_trylock(e):
    m = e.args(1)[0]
    s, tid = e.sched, e.sched.current.tid
    if not _mutex_free_for(s, m, tid):
        e.ret(EBUSY)
        return
    _lock(s, m, tid)
    e.ret(0)


@imp("pthread_mutex_unlock")
def _mutex_unlock(e):
    m = e.args(1)[0]
    s = e.sched
    owner = s.mutex_owner.get(m)
    if owner:
        if owner[1] > 1:
            s.mutex_owner[m] = (owner[0], owner[1] - 1)
        else:
            del s.mutex_owner[m]
    e.ret(0)


def _cond_wait(e, cond, m, deadline):
    s, t = e.sched, e.sched.current
    if t.wait is None or t.wait[0] != ("cond", cond):
        # first entry: release the mutex fully and remember the signal counter
        owner = s.mutex_owner.pop(m, None)
        t.wait = (("cond", cond), s.cond_seq.get(cond, 0), owner[1] if owner else 1, deadline)
    _, seq, depth, dl = t.wait
    signaled = lambda: s.cond_seq.get(cond, 0) != seq
    timed_out = lambda: dl is not None and time.time() >= dl
    _blocking(e, lambda: (signaled() or timed_out()) and _mutex_free_for(s, m, t.tid), f"cond {cond:#x}")
    t.wait = None
    s.mutex_owner[m] = (t.tid, depth)
    return 0 if signaled() else ETIMEDOUT


@imp("pthread_cond_wait")
def _pthread_cond_wait(e):
    cond, m = e.args(2)
    e.ret(_cond_wait(e, cond, m, None))


@imp("pthread_cond_timedwait")
def _pthread_cond_timedwait(e):
    import struct
    cond, m, ts = e.args(3)
    sec, nsec = struct.unpack("<ii", e.read(ts, 8))
    # bionic: absolute CLOCK_REALTIME
    e.ret(_cond_wait(e, cond, m, sec + nsec / 1e9))


@imp("pthread_cond_signal", "pthread_cond_broadcast")
def _pthread_cond_signal(e):
    c = e.args(1)[0]
    e.sched.cond_seq[c] = e.sched.cond_seq.get(c, 0) + 1
    e.ret(0)


@imp("pthread_once")
def _pthread_once(e):
    ctl, fn = e.args(2)
    if e.u32(ctl) == 0:
        e.w32(ctl, 1)
        e.call(fn)
    e.ret(0)


@imp("pthread_key_create")
def _pthread_key_create(e):
    keyp, dtor = e.args(2)
    st = e.libc
    k = st.next_key
    st.next_key += 1
    e.w32(keyp, k)
    e.ret(0)


@imp("pthread_key_delete")
def _pthread_key_delete(e):
    e.ret(0)


@imp("pthread_getspecific")
def _pthread_getspecific(e):
    e.ret(e.libc.tls_keys.get(e.args(1)[0], 0))


@imp("pthread_setspecific")
def _pthread_setspecific(e):
    k, v = e.args(2)
    e.libc.tls_keys[k] = v
    e.ret(0)


# ---------------------------------------------------------------------------------------- sleeping
def _sleep(e, seconds):
    t = e.sched.current
    if t.tid == 1:
        time.sleep(min(seconds, 0.05))   # the main thread never really sleeps in the render loop
        return
    if t.wait is None or t.wait[0] != "sleep":
        t.wait = ("sleep", time.time() + seconds)
    dl = t.wait[1]
    _blocking(e, lambda: time.time() >= dl, "sleep")
    t.wait = None


@imp("usleep")
def _usleep(e):
    _sleep(e, e.args(1)[0] / 1e6)
    e.ret(0)


@imp("nanosleep")
def _nanosleep(e):
    import struct
    req = e.args(1)[0]
    s, ns = struct.unpack("<II", e.read(req, 8))
    _sleep(e, s + ns / 1e9)
    e.ret(0)


@imp("sched_yield")
def _sched_yield(e):
    _sleep(e, 0)
    e.ret(0)
