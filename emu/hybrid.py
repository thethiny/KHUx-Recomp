"""Hybrid execution: lifted C functions (out/lift/lift.dll) run in place of their emulated originals.

Each lifted function's entry is overwritten with an SVC instruction. When the guest reaches it, the
interrupt hook copies the Unicorn registers into a CPU struct, runs the C function, copies the registers
back and continues at the address the function returned to. Guest memory is shared (core.Emu maps one
host block), so the C code reads and writes the same bytes as the emulator.

Verification (default on): the first call of every lifted function runs both versions from the same state.
  1. the C version, with every store logged (address, size, previous value),
  2. its stores are undone,
  3. the original code runs in Unicorn from the same registers (stores logged by a write hook).
The emulated result is kept either way. Registers, flags, return address and the bytes written must match;
otherwise the function is reported and reverted to emulation for the rest of the run.
"""
import ctypes as C
import json
import os
import struct
import time
from pathlib import Path

from unicorn import UC_HOOK_INTR, UC_HOOK_MEM_WRITE, UcError
from unicorn.arm_const import (UC_ARM_REG_CPSR, UC_ARM_REG_D0, UC_ARM_REG_FPSCR, UC_ARM_REG_LR, UC_ARM_REG_PC,
                               UC_ARM_REG_R0, UC_ARM_REG_SP)

from .. import paths
from .core import RETURN_MAGIC

REF_MAX_INSNS = 5_000_000      # a leaf that has not returned by then never will (or ran away)
SVC_IMM = 0xAB
THUMB_SVC = struct.pack("<H", 0xDF00 | SVC_IMM)
ARM_SVC = struct.pack("<I", 0xEF000000 | SVC_IMM)
WLOG_CAP = 1 << 20       # stores a logged run may make (16 bytes each)
VERIFY_CALLS = int(__import__("os").environ.get("KHUX_VERIFY_CALLS", "20"))   # calls compared per function
VERIFY_TRIES = 3
WORKER_NATIVE = os.environ.get("KHUX_WORKER_NATIVE", "1") == "1"   # speculative C on worker threads
REF_NATIVE = os.environ.get("KHUX_REF_NATIVE") == "1"   # reference runs call native imports (experimental)          # inconclusive checks (store log overflow) retried this many times


class WLog(C.Structure):
    _fields_ = [("addr", C.c_uint32), ("size", C.c_uint32), ("old_lo", C.c_uint32), ("old_hi", C.c_uint32)]


class CPU(C.Structure):
    _fields_ = [("r", C.c_uint32 * 16), ("n", C.c_uint32), ("z", C.c_uint32), ("c", C.c_uint32),
                ("v", C.c_uint32), ("pc", C.c_uint32), ("logging", C.c_uint32), ("wn", C.c_uint32),
                ("wcap", C.c_uint32), ("mem", C.c_void_p), ("wlog", C.POINTER(WLog)),
                ("emu", C.c_void_p), ("tail", C.c_uint32), ("lpc", C.c_uint32), ("wfull", C.c_uint32),
                ("icount", C.c_uint64), ("vfp_on", C.c_uint32), ("fpscr", C.c_uint32), ("uc", C.c_void_p),
                ("uc_read_batch", C.c_void_p), ("vf", C.c_uint64 * 32)]


LiftFn = C.CFUNCTYPE(None, C.POINTER(CPU))
EmuFn = C.CFUNCTYPE(None, C.POINTER(CPU), C.c_uint32)


class Entry(C.Structure):
    _fields_ = [("addr", C.c_uint32), ("flags", C.c_uint32), ("fn", C.c_void_p)]


REG_IDS = [UC_ARM_REG_R0 + k for k in range(13)] + [UC_ARM_REG_SP, UC_ARM_REG_LR]
VFP_IDS = [UC_ARM_REG_D0 + k for k in range(32)] + [UC_ARM_REG_FPSCR]


# Imports a speculative worker run may call (see Hybrid._worker_call). Each one can be rolled back:
#   "read"            reads memory or computes a value only
#   ("write", d, n)   writes n bytes at the address in r<d> (n = r<k> when given as "r<k>", else a constant);
#                     the range is logged first, so the undo restores it
#   "alloc"           allocates: an undo leaks the block
#   "free"            deferred until the run is kept
SPEC_IMPORTS = {
    **{n: "read" for n in (
        "strlen", "strnlen", "strcmp", "strncmp", "strcasecmp", "strncasecmp", "strchr", "strrchr", "strstr",
        "strpbrk", "strspn", "strcspn", "memcmp", "memchr", "pthread_getspecific", "pthread_self", "getenv",
        "__errno",
        "atoi", "atol", "atof", "strtol", "strtoul", "strtod", "strtof", "abs", "labs",
        "sinf", "cosf", "tanf", "asinf", "acosf", "atanf", "atan2f", "sqrtf", "powf", "expf", "logf", "log10f",
        "floorf", "ceilf", "fmodf", "roundf", "lroundf", "fabsf",
        "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "sqrt", "pow", "exp", "log", "log10",
        "floor", "ceil", "fmod", "round", "lround", "fabs", "ldexp", "frexp", "modf")},
    "memcpy": ("write", 0, "r2"), "memmove": ("write", 0, "r2"), "memset": ("write", 0, "r2"),
    "__aeabi_memcpy": ("write", 0, "r2"), "__aeabi_memcpy4": ("write", 0, "r2"),
    "__aeabi_memcpy8": ("write", 0, "r2"), "__aeabi_memmove": ("write", 0, "r2"),
    "__aeabi_memmove4": ("write", 0, "r2"), "__aeabi_memmove8": ("write", 0, "r2"),
    "__aeabi_memset": ("write", 0, "r1"), "__aeabi_memset4": ("write", 0, "r1"),
    "__aeabi_memset8": ("write", 0, "r1"), "__aeabi_memclr": ("write", 0, "r1"),
    "__aeabi_memclr4": ("write", 0, "r1"), "__aeabi_memclr8": ("write", 0, "r1"),
    "clock_gettime": ("write", 1, 8), "gettimeofday": ("write", 0, 8), "time": ("write", 0, 4),
    "malloc": "alloc", "calloc": "alloc", "strdup": "alloc", "_Znwj": "alloc", "_Znaj": "alloc",
    "free": "free", "_ZdlPv": "free", "_ZdaPv": "free",
    # scheduler state (mutex owners, condition counters): snapshotted on first use, restored by an undo; a
    # lock that would block undoes the run as usual
    **{n: "sched" for n in ("pthread_mutex_lock", "pthread_mutex_unlock", "pthread_mutex_trylock",
                            "pthread_cond_signal", "pthread_cond_broadcast")},
}


class NativeHeap:
    """The guest heap, kept by lift/rt_heap.c in guest memory and shared with lifted code (which calls it
    directly for malloc/free/calloc/realloc). Same interface as core.Heap, which it replaces at install."""

    def __init__(self, hybrid, old):
        self.h, dll, e = hybrid, hybrid.dll, hybrid.e
        for name in ("rt_heap_malloc", "rt_heap_free", "rt_heap_usable", "rt_heap_grow_in_place"):
            fn = getattr(dll, name)
            fn.argtypes = (C.POINTER(CPU), C.c_uint32) + ((C.c_uint32,) if name == "rt_heap_grow_in_place" else ())
            fn.restype = C.c_uint32
        self._malloc, self._free = dll.rt_heap_malloc, dll.rt_heap_free
        self._usable, self._grow = dll.rt_heap_usable, dll.rt_heap_grow_in_place
        # own CPU struct (no store logging) for calls from Python outside lifted runs
        self.cpu = CPU()
        self.cpu.mem = e.host_base
        self.cpu.wlog, self.cpu.wcap = hybrid.wlog, WLOG_CAP
        # hand the state over: control block from the old heap, then every live and free block gets a header
        size = dll.rt_heap_ctl_size()
        ctl = old.malloc(size)
        e.write(ctl, bytes(size))
        self.ctl, self.end = ctl, old.end
        used, free = 0x48454150, 0x45455246
        small_max = 65536
        e.write(ctl, struct.pack("<II", old.ptr, old.end))
        for p, n in old.sizes.items():
            e.write(p - 16, struct.pack("<II", n, used))
        for n, lst in old.free_lists.items():
            slot = ctl + 12 + 4 * (n // 16) if n <= small_max else ctl + 8
            for p in lst:
                e.write(p - 16, struct.pack("<II", n, free))
                e.write(p, e.read(slot, 4))
                e.write(slot, struct.pack("<I", p))
        C.c_uint32.in_dll(dll, "rt_heap_ctl").value = ctl
        self.e = e

    def _cpu(self):
        # inside a logged lifted run (speculative worker call), heap changes must be undone with it
        live = self.h.cpu
        return C.byref(live if live.logging else self.cpu)

    @property
    def ptr(self):
        return struct.unpack("<I", self.e.read(self.ctl, 4))[0]

    def malloc(self, n):
        from .core import GuestFault
        p = self._malloc(self._cpu(), n)
        if not p:
            raise GuestFault(f"guest heap exhausted (malloc {n})")
        return p

    def free(self, p):
        if p and self._free(self._cpu(), p):
            self.e.log(f"free of unknown pointer {p:#x} (lr={self.e.reg(UC_ARM_REG_LR):#x})")

    def usable(self, p):
        return self._usable(self._cpu(), p)

    def grow_in_place(self, p, n):
        return bool(self._grow(self._cpu(), p, n))


class VfpIO:
    """Batch transfer of d0..d31 + FPSCR between a Unicorn instance and a block of memory (the CPU struct's
    vf/fpscr fields, or a scratch buffer), one uc_reg_{read,write}_batch call each way."""

    def __init__(self, d_addr, fpscr_addr):
        from unicorn.unicorn_py3.unicorn import uclib
        self.read_fn, self.write_fn = uclib.uc_reg_read_batch, uclib.uc_reg_write_batch
        self.ids = (C.c_long * 33)(*VFP_IDS)
        self.ptrs = (C.c_void_p * 33)(*[d_addr + 8 * k for k in range(32)], fpscr_addr)

    def read(self, uc):
        self.read_fn(uc._uch, self.ids, self.ptrs, 33)

    def write(self, uc):
        self.write_fn(uc._uch, self.ids, self.ptrs, 33)


class Hybrid:
    def __init__(self, emu, dll=None, verify=True):
        self.e = emu
        self.uc = emu.uc
        self.verify = verify
        # load a private copy so the build can relink lift.dll while an emulator is running
        import os
        import shutil
        src = dll or os.environ.get("KHUX_LIFT_DLL") or paths.OUT / "lift" / "lift.dll"
        for stale in (paths.OUT / "lift").glob("lift_run_*.dll"):
            try:
                stale.unlink()         # copies of finished runs (a running emulator's copy is locked)
            except OSError:
                pass
        run_copy = paths.OUT / "lift" / f"lift_run_{os.getpid()}.dll"
        shutil.copyfile(src, run_copy)
        self.dll = C.CDLL(str(run_copy))
        n = C.c_int.in_dll(self.dll, "lift_count").value
        table = (Entry * n).in_dll(self.dll, "lift_table")
        self.enabled = (C.c_uint8 * n).in_dll(self.dll, "lift_enabled")
        self._rt_run = self.dll.rt_run
        self._rt_run.argtypes = (C.POINTER(CPU), C.c_uint32)
        self._rt_run.restype = None
        self.index = {}           # entry (even) -> index in lift_table / lift_enabled
        self.funcs = {}           # entry (even) -> (thumb, callable, leaf)
        self.orig = {}            # entry -> original bytes
        self.tramp = {}           # entry -> guest address of a trampoline running the original code
        from .coverage import load_blocks
        executed = load_blocks()
        skipped = 0
        # KHUX_LIFT_RANGE=lo:hi enables only non-leaf functions with lo <= rank < hi (rank = position among the
        # non-leaf entries); leaves are always on (they are verified). Used to bisect a bad lifted function.
        import os
        rng = os.environ.get("KHUX_LIFT_RANGE")
        lo_r, hi_r = (int(x) for x in rng.split(":")) if rng else (0, 1 << 30)
        rank = 0
        # KHUX_LIFT_DENY=file.json: list of entries (hex) kept emulated (bisecting a bad lifted function)
        deny = set()
        if os.environ.get("KHUX_LIFT_DENY"):
            deny = {int(x, 16) for x in json.loads(Path(os.environ["KHUX_LIFT_DENY"]).read_text())}
        for k, ent in enumerate(table):
            a = ent.addr & ~1
            self.index[a] = k
            if a in deny:
                continue
            if executed is not None and ent.addr not in executed:
                skipped += 1          # never seen executing in this mode: leave it alone
                continue
            if not ent.flags & 1:
                rank += 1
                if not (lo_r <= rank - 1 < hi_r):
                    continue
            self.funcs[a] = (bool(ent.addr & 1), LiftFn(ent.fn), bool(ent.flags & 1))
        # KHUX_LIFT_UNSEEN=1: lifted functions never seen executing are still callable from C (not patched, so
        # the emulator never enters them and the checker never compares them). Off by default: unchecked code.
        if os.environ.get("KHUX_LIFT_UNSEEN") == "1" and executed is not None:
            n_unseen = 0
            for k, ent in enumerate(table):
                if ent.addr not in executed:
                    self.enabled[k] = 1
                    n_unseen += 1
            emu.log(f"hybrid: {n_unseen} unseen lifted functions callable from C (KHUX_LIFT_UNSEEN)")
        if executed is None:
            emu.log("hybrid: no out/coverage.json - patching every lifted function (run once with --coverage)")
        else:
            emu.log(f"hybrid: {skipped} lifted functions never executed in recorded runs, left unpatched")
        self.cpu = CPU()
        self.wlog = (WLog * WLOG_CAP)()
        self.cpu.mem = emu.host_base
        self.cpu.wlog = self.wlog
        self.cpu.wcap = WLOG_CAP
        if hasattr(self.dll, "rt_heap_ctl_size") and os.environ.get("KHUX_NATIVE_HEAP", "1") == "1":
            emu.heap = NativeHeap(self, emu.heap)
            emu.log(f"hybrid: guest heap handed to lift.dll (control block {emu.heap.ctl:#x})")
        if hasattr(self.dll, "rt_native_call"):
            self._setup_native_imports()
        self._emu_cb = EmuFn(self._emu_call)              # keep a reference: C holds the pointer
        self.cpu.emu = C.cast(self._emu_cb, C.c_void_p)
        # VFP registers: the C code fetches them itself on first use (rt_vfp_load -> uc_reg_read_batch)
        from unicorn.unicorn_py3.unicorn import uclib
        self.cpu.uc = self.uc._uch
        self.cpu.uc_read_batch = C.cast(uclib.uc_reg_read_batch, C.c_void_p).value
        base = C.addressof(self.cpu)
        self.vfp_io = VfpIO(base + CPU.vf.offset, base + CPU.fpscr.offset)
        self.vfp_buf = (C.c_uint64 * 33)()              # scratch copy for reference runs
        self.vfp_buf_io = VfpIO(C.addressof(self.vfp_buf), C.addressof(self.vfp_buf) + 8 * 32)
        self.pending_fault = None
        self.watch_addr = int(os.environ["KHUX_WATCH"], 16) if os.environ.get("KHUX_WATCH") else 0
        self.watch_val = None
        if self.watch_addr:
            def on_watch_write(uc, access, addr, size, value, _):
                self.e.log(f"?? watch write {addr:#x} size {size} = {value:#x} by emulated pc "
                           f"{uc.reg_read(UC_ARM_REG_PC):#x} {self.e.sym(uc.reg_read(UC_ARM_REG_PC))}")
            self.uc.hook_add(UC_HOOK_MEM_WRITE, on_watch_write, begin=self.watch_addr, end=self.watch_addr + 3)
        self.esc = {} if os.environ.get("KHUX_PROFILE") else None   # (entry, continue pc) -> count
        self.tramp_hits = {}
        self.worker_bad = {}      # entry -> why its C code cannot run on worker threads (reaches an import, ...)
        self.worker_native = 0    # worker-thread calls completed in C
        self.spec = False         # a speculative worker run is in progress (see _worker_call)
        self.spec_frees = []      # frees deferred until that run is kept
        self.spec_sched = None    # scheduler state before that run touched it
        self.taint_block = False
        self.worker_blocks = {}   # entry -> speculative runs abandoned because the first import blocked
        self.verified, self.mismatch, self.tries = set(), {}, {}
        self.vcount = {}          # entry -> number of matching compared calls
        self.unverifiable = set() # call tree reaches imports: cannot be undone and re-run
        self.taint = False        # set when a verification run touched an import
        self.calls = 0
        self.native_calls = 0
        no_tramp = 0
        for a, (thumb, _, _) in list(self.funcs.items()):
            patch = THUMB_SVC if thumb else ARM_SVC
            self.orig[a] = emu.read(a, len(patch))
            t = self._make_trampoline(a, thumb)
            if t is None:
                no_tramp += 1             # first instruction is PC-relative: worker threads run C here too
            else:
                self.tramp[a] = t
            emu.write(a, patch)
            self.enabled[self.index[a]] = 1
        if no_tramp:
            emu.log(f"hybrid: {no_tramp} functions without a trampoline (worker threads run their C code)")
        self.uc.ctl_remove_cache(0x1000, 0x1011000)
        self.uc.hook_add(UC_HOOK_INTR, self._on_intr_safe)
        # addresses hooked with emu.on (control channel events, tracing) must run emulated to fire
        import bisect
        self._entries = sorted(self.index)
        self._bisect = bisect.bisect_right
        self.hook_disabled = set()
        for a in list(emu.hooked):
            self._keep_emulated(a)
        emu.hook_listeners.append(self._keep_emulated)
        emu.log(f"hybrid: {len(self.funcs)} lifted functions patched (verify={'on' if verify else 'off'})")
        if os.environ.get("KHUX_PROFILE"):
            self._start_profile()

    def _keep_emulated(self, addr):
        """An emu.on hook at addr only fires in emulated code: disable the lifted function containing it
        (the nearest entry at or below addr), so calls from C escape to the emulator there."""
        k = self._bisect(self._entries, addr) - 1
        if k < 0:
            return
        entry = self._entries[k]
        if entry in self.hook_disabled:
            return
        self.hook_disabled.add(entry)
        self.enabled[self.index[entry]] = 0
        if entry in self.funcs:
            del self.funcs[entry]
            self._poke(entry, self.orig[entry])
            self.uc.ctl_remove_cache(entry, entry + 4)
        self.e.log(f"hybrid: {entry:#x} {self.e.sym(entry)} kept emulated (hook at {addr:#x})")

    # ------------------------------------------------------------------ native imports
    def _setup_native_imports(self):
        """Imports implemented in lift.dll (libc, heap, OpenGL ES). Lifted code calls them directly; the
        emulator's Python handlers for the OpenGL ES ones are replaced by calls into the same C code, so both
        paths share one implementation and GL state is never tracked in two places."""
        e, dll = self.e, self.dll
        n = C.c_int.in_dll(dll, "rt_native_count").value
        names = (C.c_char_p * n).in_dll(dll, "rt_native_names")
        self.native_ids = {names[k].decode(): k for k in range(n)}
        dll.rt_native_call.argtypes = (C.POINTER(CPU), C.c_int)
        dll.rt_native_call.restype = C.c_int
        dll.rt_native_slot.argtypes = (C.c_uint32, C.c_int)
        errno_ptr = C.c_uint32.in_dll(dll, "rt_errno_ptr")
        errno_ptr.value = e.libc.errno_ptr
        sched = getattr(e, "sched", None)
        if sched is not None:
            sched.on_switch.append(lambda t: setattr(errno_ptr, "value", t.errno_ptr or 0))
        for addr, name in e.slots.items():      # imports reached through function pointers (GOT values)
            if name in self.native_ids and not name.startswith("gl"):
                dll.rt_native_slot(addr & ~1, self.native_ids[name])
        self.py_cpu = CPU()
        self.py_cpu.mem = e.host_base
        self.py_cpu.wlog, self.py_cpu.wcap = self.wlog, WLOG_CAP
        # stdio streams: the device file system root and the one virtual file (the library itself)
        if os.environ.get("KHUX_NATIVE_FILES", "1") == "1":
            from . import libc
            (C.c_char * 1024).in_dll(dll, "rt_fs_root").value = str(paths.FS_ROOT).replace("\\", "/").encode()
            (so_dev, so_host), = libc.VIRTUAL_FILES.items()
            (C.c_char * 256).in_dll(dll, "rt_so_guest").value = so_dev.encode()
            (C.c_char * 1024).in_dll(dll, "rt_so_host").value = str(so_host).replace("\\", "/").encode()
            n_files = 0
            for name in ("fopen", "fclose", "fread", "fwrite", "fgets", "fseek", "fseeko", "ftell", "ftello",
                         "fputs", "fputc", "fputwc"):
                if name in e.handlers and name in self.native_ids:
                    e.handlers[name] = self._native_handler(self.native_ids[name], e.handlers[name])
                    n_files += 1
            e.log(f"hybrid: native stdio streams ({n_files} calls; Python handlers route to lift.dll)")
        gl_state = getattr(e, "gl", None)
        if gl_state is None or os.environ.get("KHUX_NATIVE_GL", "1") != "1":
            return
        missing = dll.rt_gl_init()
        if missing:
            e.log(f"hybrid: native OpenGL ES off ({missing} entry points missing)")
            return
        from . import gles
        get_string = gles.gl.glGetString
        get_string.restype = C.c_char_p
        get_string.argtypes = (C.c_uint32,)
        strings = (C.c_uint32 * 8).in_dll(dll, "rt_gl_strings")
        for k in range(4):
            s = get_string(0x1F00 + k) or b""
            if k == 3:   # GL_EXTENSIONS: advertise what the runtime provides
                s += b" GL_OES_mapbuffer GL_OES_vertex_array_object"
            strings[k] = gl_state.guest_string(s)
        gl_state.native_draws = C.c_uint32.in_dll(dll, "rt_gl_draws")
        import importlib.util
        spec = importlib.util.spec_from_file_location("gl_table", Path(__file__).resolve().parent.parent /
                                                      "lift" / "gl_table.py")
        gl_table = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(gl_table)
        for addr, name in e.slots.items():
            if name in self.native_ids and name.startswith("gl"):
                dll.rt_native_slot(addr & ~1, self.native_ids[name])
        replaced = 0
        for name in gl_table.names():
            orig = gles.H.get(name)
            if orig is None:
                continue
            h = self._native_handler(self.native_ids[name], orig)
            gles.H[name] = h
            if name in e.handlers:
                e.handlers[name] = h
            replaced += 1
        orig_gpa = e.handlers.get("eglGetProcAddress")
        if orig_gpa is not None:
            def get_proc(em, orig_gpa=orig_gpa):
                name = em.cstr(em.reg(UC_ARM_REG_R0))
                orig_gpa(em)
                addr = em.reg(UC_ARM_REG_R0)
                if addr and name in self.native_ids:
                    dll.rt_native_slot(addr & ~1, self.native_ids[name])
            e.handlers["eglGetProcAddress"] = get_proc
            gles.H["eglGetProcAddress"] = get_proc
        e.log(f"hybrid: native OpenGL ES ({replaced} calls; Python handlers route to lift.dll)")

    def _native_handler(self, nid, orig):
        """Python import handler that runs native import `nid` on the trapped call's registers, falling back
        to the original handler when the C code declines it."""
        call, cpu = self.dll.rt_native_call, self.py_cpu
        reg = self.e.reg

        def handler(em):
            for k in range(4):
                cpu.r[k] = reg(UC_ARM_REG_R0 + k)
            cpu.r[13] = reg(UC_ARM_REG_SP)
            if call(C.byref(cpu), nid):
                em.ret(cpu.r[0])
            else:
                orig(em)
        return handler

    # ------------------------------------------------------------------ trampolines
    def _make_trampoline(self, entry, thumb):
        """Guest code that runs the original function: its first instruction (overwritten by the SVC) followed
        by a jump to the rest. Used for guest worker threads, whose slices cannot be suspended while a C frame
        is on the host stack. Returns None when the first instruction cannot be relocated."""
        from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs
        from capstone import arm as A
        n = 2 if thumb else 4
        code = self.e.read(entry, n)
        cs = Cs(CS_ARCH_ARM, CS_MODE_THUMB if thumb else CS_MODE_ARM)
        cs.detail = True
        ins = next(cs.disasm(code, entry), None)
        if ins is None or ins.size != n or ins.group(A.ARM_GRP_JUMP) or ins.id in (A.ARM_INS_BL, A.ARM_INS_BLX):
            return None
        for o in ins.operands:
            if (o.type == A.ARM_OP_REG and o.reg == A.ARM_REG_PC) or \
                    (o.type == A.ARM_OP_MEM and o.mem.base == A.ARM_REG_PC):
                return None
        blk = self.e.alloc(24)
        if thumb:
            t = (blk + 3) & ~3
            t += 2                                   # bx pc at t+2 lands on t+6, which must be word aligned
            body = code + struct.pack("<HH", 0x4778, 0x46C0)               # I; bx pc; nop
            body += struct.pack("<II", 0xE51FF004, (entry + 2) | 1)       # ldr pc,[pc,#-4]; .word entry+2|1
            self.e.write(t, body)
            return t | 1
        t = (blk + 3) & ~3
        self.e.write(t, code + struct.pack("<II", 0xE51FF004, entry + 4))
        return t

    # ------------------------------------------------------------------ calls from C into emulated code
    def _emu_call(self, cpu_p, target):
        """C called a function that is not lifted (or not enabled): run it in the emulator on the game CPU,
        e.call style (LR = RETURN_MAGIC, stopped by the trap hook), then hand the registers back."""
        e, cpu = self.e, self.cpu
        ret = cpu.r[14]
        spec = self.spec and cpu.logging
        if cpu.logging:
            pol = SPEC_IMPORTS.get(self._import_name(target)) if spec and not cpu.wfull else None
            if isinstance(pol, tuple):
                d, n = cpu.r[pol[1]], pol[2]
                n = cpu.r[int(n[1:])] if isinstance(n, str) else n
                if not (d == 0 or self._log_range(d, n)):
                    pol = None
            if pol is None:
                # verification run, or an import a speculative worker run cannot roll back: abandon the run
                self.taint = True
                self.taint_target = target
                cpu.pc = 0xFFFFFFFF
                return
            if pol == "free":
                self.spec_frees.append(cpu.r[0])         # applied when the run is kept
                cpu.pc = ret
                return
            if pol == "sched" and self.spec_sched is None:
                sch = e.sched
                self.spec_sched = (dict(sch.mutex_owner), dict(sch.cond_seq))
        # other guest threads may run lifted code while this call is in the emulator (the main thread waiting
        # on a worker, a worker waiting in place): they share self.cpu, so keep this frame's copy
        saved = C.string_at(C.addressof(cpu), C.sizeof(CPU))
        try:
            if cpu.vfp_on:
                self.vfp_io.write(self.uc)          # the callee sees the C code's VFP registers
            sched = getattr(e, "sched", None)
            worker = sched.current if sched is not None and sched.current.tid != 1 else None
            give_up = None
            while True:
                for k, rid in enumerate(REG_IDS):
                    e.set_reg(rid, cpu.r[k])
                cpsr = e.reg(UC_ARM_REG_CPSR)
                cpsr = (cpsr & 0x0FFFFFDF) | (cpu.n << 31) | (cpu.z << 30) | (cpu.c << 29) | (cpu.v << 28)
                e.set_reg(UC_ARM_REG_CPSR, cpsr | (0x20 if target & 1 else 0))
                e.set_reg(UC_ARM_REG_LR, RETURN_MAGIC)
                e.fault = None
                e.call_depth += 1
                try:
                    self.uc.emu_start(target, RETURN_MAGIC)
                except UcError as ex:
                    if not e.fault:
                        e.fault = f"{ex} at pc={e.reg(UC_ARM_REG_PC):#x} (called from lifted code)"
                finally:
                    e.call_depth -= 1
                if worker is None or worker.blocked is None or e.fault:
                    break
                # a worker thread blocked inside the import (mutex, condition, sleep, socket). A blocking
                # handler raises before doing anything, so the call had no effect yet.
                blk, worker.blocked = worker.blocked, None
                if spec:
                    # nothing has been committed: undo the C run and let the original code run this call
                    self.taint = self.taint_block = True
                    self.taint_target = target
                    cpu.pc = 0xFFFFFFFF
                    return
                # committed C frames are on the host stack: run the other workers here until it can proceed,
                # then call the import again (as the scheduler does for the main thread)
                give_up = give_up or time.time() + 30
                sched.suspended.add(worker)
                try:
                    while not blk.ready():
                        others = sched.runnable()
                        for t in others:
                            sched._run_slice(t)
                        if not others:
                            time.sleep(0.001)
                        if time.time() > give_up:
                            e.fault = f"worker {worker.tid} blocked inside lifted code on {blk.why}"
                            break
                finally:
                    sched.suspended.discard(worker)
                if e.fault:
                    break
                C.memmove(C.addressof(cpu), saved, len(saved))
            icount = cpu.icount                     # profiling counter: keep what nested runs added
            C.memmove(C.addressof(cpu), saved, len(saved))
            cpu.icount = icount
            for k, rid in enumerate(REG_IDS[:14]):
                cpu.r[k] = e.reg(rid)
            c = e.reg(UC_ARM_REG_CPSR)
            cpu.n, cpu.z, cpu.c, cpu.v = (c >> 31) & 1, (c >> 30) & 1, (c >> 29) & 1, (c >> 28) & 1
            cpu.r[14] = ret
            cpu.vfp_on = 0                          # the callee may have changed them: reload on next use
            if e.fault:
                self.pending_fault, e.fault = e.fault, None
                cpu.pc = 0xFFFFFFFF                 # not the caller's return address: the C chain unwinds
            else:
                cpu.pc = ret
        except Exception as ex:                     # never let an exception cross the ctypes callback
            import traceback
            self.pending_fault = f"host exception in emu_call({target:#x}): {ex!r}\n{traceback.format_exc()}"
            cpu.pc = 0xFFFFFFFF

    def _import_name(self, target):
        """Name of the import behind a call target: a PLT stub or a host function slot."""
        if not hasattr(self, "_plt_names"):
            an = json.loads((paths.OUT / "analysis.json").read_text())
            self._plt_names = {int(k, 16): v["name"] for k, v in an["plt"].items()}
        a = target & ~1
        name = self._plt_names.get(a) or self.e.slots.get(a) or ""
        return name.strip("<>")

    def _log_range(self, addr, n):
        """Add n bytes at addr to the store log (4-byte entries), as if the C code had written them."""
        cpu = self.cpu
        words = (n + 3) // 4
        if n > 1 << 24 or cpu.wn + words > WLOG_CAP:
            return False
        for k in range(words):
            a = addr + 4 * k
            w = self.wlog[cpu.wn]
            w.addr, w.size, w.old_hi = a, 4, 0
            try:
                w.old_lo = struct.unpack("<I", self._host(a, 4))[0]
            except OSError:               # not mapped: the import would fault anyway
                return False
            cpu.wn += 1
        return True

    # ------------------------------------------------------------------ helpers
    def _host(self, addr, n):
        return C.string_at(self.e.host_base + addr, n)

    def _poke(self, addr, data):
        C.memmove(self.e.host_base + addr, data, len(data))

    def _load_cpu(self, cpsr):
        reg = self.e.reg
        for k, rid in enumerate(REG_IDS):
            self.cpu.r[k] = reg(rid)
        self.cpu.n, self.cpu.z = (cpsr >> 31) & 1, (cpsr >> 30) & 1
        self.cpu.c, self.cpu.v = (cpsr >> 29) & 1, (cpsr >> 28) & 1
        self.cpu.vfp_on = 0

    def _regs_of_cpu(self):
        return [self.cpu.r[k] for k in range(15)], (self.cpu.n, self.cpu.z, self.cpu.c, self.cpu.v)

    def _store_cpu(self, cpsr):
        set_reg = self.e.set_reg
        for k, rid in enumerate(REG_IDS):
            set_reg(rid, self.cpu.r[k])
        cpsr = (cpsr & 0x0FFFFFDF) | (self.cpu.n << 31) | (self.cpu.z << 30) | (self.cpu.c << 29) | (self.cpu.v << 28)
        cpsr |= 0x20 if self.cpu.pc & 1 else 0
        set_reg(UC_ARM_REG_CPSR, cpsr)
        set_reg(UC_ARM_REG_PC, self.cpu.pc)
        if self.cpu.vfp_on:
            self.vfp_io.write(self.uc)

    # ------------------------------------------------------------------ interrupt hook
    def _on_intr_safe(self, uc, intno, user):
        try:
            self._on_intr(uc, intno, user)
        except Exception as ex:
            import traceback
            self.e.log(f"!! hybrid: host exception in the SVC hook: {ex!r} {traceback.format_exc()}")
            self.e.fault = f"hybrid hook exception: {ex!r}"
            uc.emu_stop()

    def _watch_check(self, where):
        """KHUX_WATCH=0xADDR: log every change of the word at ADDR seen around lifted-code transitions."""
        v = struct.unpack("<I", self._host(self.watch_addr, 4))[0]
        if v != self.watch_val:
            if self.watch_val is not None:
                self.e.log(f"?? watch {self.watch_addr:#x}: {self.watch_val:#x} -> {v:#x} at {where}")
            self.watch_val = v

    def _on_intr(self, uc, intno, _):
        if self.watch_addr:
            entry = self.e.reg(UC_ARM_REG_PC) - (2 if self.e.reg(UC_ARM_REG_CPSR) & 0x20 else 4)
            self._watch_check(f"before {entry:#x} {self.e.sym(entry)}")
            try:
                return self._on_intr_body(uc, intno, _)
            finally:
                self._watch_check(f"after {entry:#x} {self.e.sym(entry)} (pc now {self.e.reg(UC_ARM_REG_PC):#x})")
        return self._on_intr_body(uc, intno, _)

    def _on_intr_body(self, uc, intno, _):
        if intno != 2:
            return
        cpsr = self.e.reg(UC_ARM_REG_CPSR)
        thumb = bool(cpsr & 0x20)
        entry = self.e.reg(UC_ARM_REG_PC) - (2 if thumb else 4)
        f = self.funcs.get(entry)
        if f is None:
            self.e.log(f"!! hybrid: svc at {entry:#x} is not a lifted entry")
            uc.emu_stop()
            return
        sched = getattr(self.e, "sched", None)
        if sched is not None and sched.current.tid != 1 and entry in self.tramp:
            if entry not in self.worker_bad and WORKER_NATIVE:
                if self._worker_call(entry, f, cpsr):
                    return
            # guest worker thread: its slice must stay suspendable, so run the original code
            self.e.set_reg(UC_ARM_REG_PC, self.tramp[entry])
            if self.esc is not None:
                self.tramp_hits[entry] = self.tramp_hits.get(entry, 0) + 1
            return
        self.calls += 1
        if self.calls % 20000 == 0:
            self.report()
        if self.verify and entry not in self.verified and entry not in self.unverifiable:
            self._verified_call(entry, f, cpsr)
            return
        self._load_cpu(cpsr)
        self.cpu.logging = 0
        lr_in = self.cpu.r[14]
        try:
            self._rt_run(C.byref(self.cpu), entry | (1 if f[0] else 0))
        except OSError as ex:                      # access violation inside lifted code
            cpu = self.cpu
            self.e.log(f"!! hybrid: {entry:#x} {self.e.sym(entry)} crashed natively ({ex}) at guest "
                       f"{cpu.lpc:#x} {self.e.sym(cpu.lpc)} - stopping")
            self.e.log("   " + " ".join(f"r{k}={cpu.r[k]:#x}" for k in range(15)) + f" host_base={self.e.host_base:#x}")
            self.e.fault = f"native crash in lifted {entry:#x}: {ex}"
            uc.emu_stop()
            return
        self.native_calls += 1
        if self.esc is not None and self.cpu.pc != lr_in:
            k = (entry, self.cpu.pc)          # lifted code handed back to the emulator somewhere else
            self.esc[k] = self.esc.get(k, 0) + 1
        self._store_cpu(cpsr)
        if self.pending_fault:
            self.e.log(f"!! hybrid: fault below lifted {entry:#x}: {self.pending_fault}")
            self.e.fault, self.pending_fault = self.pending_fault, None
            uc.emu_stop()

    # ------------------------------------------------------------------ worker threads
    def _worker_call(self, entry, f, cpsr):
        # may run nested inside another thread's C frame (see _emu_call): keep that frame's state
        outer = (self.spec, self.spec_frees, self.spec_sched, self.taint, self.taint_block,
                 getattr(self, "taint_target", 0))
        self.spec, self.spec_frees, self.spec_sched = True, [], None
        try:
            return self._worker_run(entry, f, cpsr)
        finally:
            (self.spec, self.spec_frees, self.spec_sched, self.taint, self.taint_block,
             self.taint_target) = outer

    def _restore_entry(self, regs, vfp, cpsr):
        if self.spec_sched is not None:          # scheduler state changed by the undone run
            sch = self.e.sched
            sch.mutex_owner.clear()
            sch.mutex_owner.update(self.spec_sched[0])
            sch.cond_seq.clear()
            sch.cond_seq.update(self.spec_sched[1])
            self.spec_sched = None
        for rid, v in zip(REG_IDS, regs):
            self.e.set_reg(rid, v)
        self.e.set_reg(UC_ARM_REG_CPSR, cpsr)
        self.vfp_buf[:] = vfp
        self.vfp_buf_io.write(self.uc)

    def _worker_run(self, entry, f, cpsr):
        """Run a lifted function on a guest worker thread. A worker's slice must stay suspendable, and an import
        may block (mutex, condition wait), which cannot wait with a C frame on the host stack: the main thread
        could not run until the C code returned. So the C code runs speculatively, every store logged. Imports
        that can be rolled back (SPEC_IMPORTS) run normally; any other import, or a full store log, undoes the
        run and this call runs the original code instead. Returns True when the C result was kept."""
        self._load_cpu(cpsr)
        cpu = self.cpu
        entry_regs = list(cpu.r[:15])          # imports run during the attempt change the emulator's registers
        self.vfp_buf_io.read(self.uc)
        entry_vfp = list(self.vfp_buf)
        cpu.logging, cpu.wn, cpu.wfull = 1, 0, 0
        self.taint = self.taint_block = False
        crash = None
        try:
            self._rt_run(C.byref(cpu), entry | (1 if f[0] else 0))
        except OSError as ex:
            crash = str(ex)
        cpu.logging = 0
        if self.taint_block and not crash:
            # its first import would have blocked (a busy mutex, an empty queue): only this call falls back,
            # unless it keeps happening
            self.dll.rt_undo(C.byref(cpu))
            self._restore_entry(entry_regs, entry_vfp, cpsr)
            n = self.worker_blocks.get(entry, 0) + 1
            self.worker_blocks[entry] = n
            if n >= 8:
                self.worker_bad[entry] = f"blocks in {self.e.sym(self.taint_target)}"
            return False
        if self.taint or crash or cpu.wfull or self.pending_fault:
            self.dll.rt_undo(C.byref(cpu))
            self._restore_entry(entry_regs, entry_vfp, cpsr)
            why = (f"import {self._import_name(self.taint_target) or self.e.sym(self.taint_target)}" if self.taint else "crash " + crash if crash else
                   "store log full" if cpu.wfull else "fault")
            self.worker_bad[entry] = why
            self.pending_fault = None
            return False
        self.worker_native += 1
        for ptr in self.spec_frees:
            self.e.heap.free(ptr)
        self._store_cpu(cpsr)
        return True

    # ------------------------------------------------------------------ verification
    def _verified_call(self, entry, f, cpsr):
        thumb = f[0]
        start_regs = [self.e.reg(r) for r in REG_IDS]
        lr = start_regs[14]
        # 1. lifted version, stores logged
        self._load_cpu(cpsr)
        self.cpu.logging, self.cpu.wn, self.cpu.wfull = 1, 0, 0
        self.taint = False
        crash = None
        try:
            self._rt_run(C.byref(self.cpu), entry | (1 if thumb else 0))
        except OSError as ex:
            crash = str(ex)
        self.cpu.logging = 0
        wn = self.cpu.wn
        if self.watch_addr:
            self._watch_check(f"verify {entry:#x}: after C run (wn={wn}, taint={self.taint}, crash={crash}, "
                              f"c_pc={self.cpu.pc:#x})")
            wa = self.watch_addr
            hits = [(k, self.wlog[k].addr, self.wlog[k].size, self.wlog[k].old_lo, self.wlog[k].old_hi)
                    for k in range(min(wn, WLOG_CAP))
                    if self.wlog[k].addr <= wa + 3 and self.wlog[k].addr + self.wlog[k].size > wa]
            if hits:
                self.e.log(f"?? watch: logged stores over it: "
                           f"{[(k, hex(a), n, hex(o), hex(pc)) for k, a, n, o, pc in hits][:8]}")
                first = hits[0][0]
                self.e.log("?? watch: stores before it: " + ", ".join(
                    f"{self.wlog[k].addr:#x}@{self.wlog[k].old_hi:#x}" for k in range(max(0, first - 12), first)))
        c_regs, c_flags = self._regs_of_cpu()
        c_pc = self.cpu.pc
        c_vfp = list(self.cpu.vf) if self.cpu.vfp_on else None
        logs = [(self.wlog[k].addr, self.wlog[k].size, self.wlog[k].old_lo) for k in range(min(wn, WLOG_CAP))]
        c_final, orig = {}, {}
        for a, n, _ in logs:
            for b in range(n):
                c_final[a + b] = self._host(a + b, 1)[0]
        # 2. undo (reverse order restores the oldest value last)
        for a, n, old in reversed(logs):
            self._poke(a, struct.pack({1: "<B", 2: "<H", 4: "<I"}[n], old & ((1 << (8 * n)) - 1)))
            for b in range(n):
                orig[a + b] = (old >> (8 * b)) & 0xFF
        # 3. original code, in Unicorn
        if self.watch_addr:
            self._watch_check(f"verify {entry:#x}: after undo")
        for rid, v in zip(REG_IDS, start_regs):
            self.e.set_reg(rid, v)
        self.e.set_reg(UC_ARM_REG_CPSR, cpsr)
        if self.taint:
            self._give_up(entry, cpsr)
            return
        # stop where the C version returned: entries reached by a jump (IDA-split mid-function labels) return
        # through their parent's frame, not to LR
        self.ref_taint = False
        self.vfp_buf_io.read(self.uc)
        start_vfp = list(self.vfp_buf)
        # where the reference stops: a normal return (to LR) is the first arrival there at the C version's
        # stack depth (recursion through one call site returns to the same address from deeper frames); a piece
        # that ends elsewhere may pass that address several times first, so it must arrive with all of the C
        # version's registers
        if crash:
            want = None
        elif c_pc == lr:
            want = [(REG_IDS[13], c_regs[13])]
        else:
            want = [(REG_IDS[k], c_regs[k]) for k in (*range(12), 13)]
        e_written, ok = self._emulate(entry, cpsr, orig, c_pc if not crash else lr, want)
        if self.ref_taint or not ok:
            # the reference reached an import or failed: undo its stores, restore the registers and run this
            # call normally (a partial reference run must never leak into the game state)
            for b in e_written:
                if b in orig:
                    self._poke(b, bytes([orig[b]]))
            for rid, v in zip(REG_IDS, start_regs):
                self.e.set_reg(rid, v)
            self.e.set_reg(UC_ARM_REG_CPSR, cpsr)
            self.vfp_buf[:] = start_vfp
            self.vfp_buf_io.write(self.uc)
            if not self.ref_taint:
                self.e.log(f"!! hybrid: reference run of {entry:#x} {self.e.sym(entry)} failed; not verifiable")
            self._give_up(entry, cpsr)
            return
        if crash:
            self._revert(entry, f"native crash: {crash}")
            return
        if self.cpu.wfull:
            self.tries[entry] = self.tries.get(entry, 0) + 1
            if self.tries[entry] >= VERIFY_TRIES:
                self._revert(entry, "store log overflow")
            return
        # 4. compare
        e_regs = [self.e.reg(r) for r in REG_IDS]
        ec = self.e.reg(UC_ARM_REG_CPSR)
        e_flags = ((ec >> 31) & 1, (ec >> 30) & 1, (ec >> 29) & 1, (ec >> 28) & 1)
        diffs = []
        for k in range(15):
            if k in (12, 14):
                continue      # ip: call-clobbered scratch (PLT stubs set it); LR: compared via the return pc
            if c_regs[k] != e_regs[k]:
                diffs.append(f"r{k} C={c_regs[k]:#x} emu={e_regs[k]:#x}")
        if c_vfp is not None:
            self.vfp_buf_io.read(self.uc)            # the reference run's VFP result (copied to the game CPU)
            for k in range(32):
                if c_vfp[k] != self.vfp_buf[k]:
                    diffs.append(f"d{k} C={c_vfp[k]:#x} emu={self.vfp_buf[k]:#x}")
        # condition flags are not preserved across calls (AAPCS), so callers never read them: not compared
        # the return address matched: the reference run stopped exactly where the C version returned
        sp_end = e_regs[13]
        for b in set(c_final) | set(e_written):
            if sp_end - 0x10000 <= b < sp_end:
                continue      # dead stack below the final SP (e.g. a switch helper's push/pop scratch word)
            cv = c_final.get(b, orig.get(b))
            ev = self._host(b, 1)[0]
            if cv is None:
                cv = ev       # C never wrote it and the emulated store left it unchanged
            if cv != ev:
                diffs.append(f"mem {b:#x} C={cv:#x} emu={ev:#x}")
                if len(diffs) > 12:
                    break
        if diffs:
            self._revert(entry, "; ".join(diffs[:12]))
        else:
            n = self.vcount.get(entry, 0) + 1
            self.vcount[entry] = n
            if n >= VERIFY_CALLS:
                self.verified.add(entry)

    def _give_up(self, entry, cpsr):
        """The call tree reaches imports, so this function cannot be checked by undo/re-run. Mark it and run
        this call normally: the original code via its trampoline, or the C version when there is none."""
        self.unverifiable.add(entry)
        if entry in self.tramp:
            self.e.set_reg(UC_ARM_REG_PC, self.tramp[entry])
            return
        self._load_cpu(cpsr)
        self._rt_run(C.byref(self.cpu), entry | (1 if self.funcs[entry][0] else 0))
        self._store_cpu(cpsr)

    def _ref_cpu(self):
        """A second Unicorn instance over the same host memory, used only for reference runs. Running the
        original code there leaves the game's own emu_start untouched: a nested emu_start inside a hook would
        end the outer run early (a frame's game code cut off half-way)."""
        if getattr(self, "ref", None) is None:
            from unicorn import UC_ARCH_ARM, UC_MODE_ARM, Uc
            from unicorn.arm_const import UC_ARM_REG_C1_C0_2, UC_ARM_REG_FPEXC
            ref = Uc(UC_ARCH_ARM, UC_MODE_ARM)
            for lo, hi, perms in self.uc.mem_regions():
                ref.mem_map_ptr(lo, hi - lo + 1, perms, self.e.host_base + lo)
            ref.reg_write(UC_ARM_REG_C1_C0_2, ref.reg_read(UC_ARM_REG_C1_C0_2) | (0xF << 20))
            ref.reg_write(UC_ARM_REG_FPEXC, 0x40000000)
            self.ref_written, self.ref_orig = None, None

            def on_write(uc, access, addr, size, value, _):
                w, o = self.ref_written, self.ref_orig
                if w is None:
                    return
                for b in range(size):
                    if addr + b not in w:
                        w.add(addr + b)
                        if o is not None and addr + b not in o:
                            o[addr + b] = self._host(addr + b, 1)[0]
            ref.hook_add(UC_HOOK_MEM_WRITE, on_write)

            ncpu = CPU()                          # native imports called by the reference run
            ncpu.mem = self.e.host_base
            nlog = (WLog * WLOG_CAP)()
            ncpu.wlog, ncpu.wcap = nlog, WLOG_CAP

            def on_host_code(uc, addr, size, _):
                # an import slot: imports implemented natively (libc, math, heap; not GL) run here exactly as
                # they do for the C version, their stores recorded like the reference's own
                name = self.e.slots.get(addr & ~1) if addr < KUSER_PAGE else None
                nid = getattr(self, "native_ids", {}).get(name) if name and not name.startswith("gl") else None
                if nid is not None and self.ref_written is not None and REF_NATIVE:
                    for k in range(4):
                        ncpu.r[k] = uc.reg_read(UC_ARM_REG_R0 + k)
                    ncpu.r[13] = uc.reg_read(UC_ARM_REG_SP)
                    ncpu.r[14] = lr = uc.reg_read(UC_ARM_REG_LR)
                    ncpu.logging, ncpu.wn, ncpu.wfull = 1, 0, 0
                    ok = self.dll.rt_native_call(C.byref(ncpu), nid)
                    ncpu.logging = 0
                    if ok and not ncpu.wfull:
                        w, o = self.ref_written, self.ref_orig
                        for k in range(ncpu.wn):
                            ent = nlog[k]
                            for b in range(ent.size):
                                ba = ent.addr + b
                                if ba not in w:
                                    w.add(ba)
                                    if o is not None and ba not in o:
                                        o[ba] = (ent.old_lo >> (8 * b)) & 0xFF
                        uc.reg_write(UC_ARM_REG_R0, ncpu.r[0])
                        uc.reg_write(UC_ARM_REG_R0 + 1, ncpu.r[1])
                        uc.reg_write(UC_ARM_REG_PC, lr)
                        return
                    if ncpu.wn:                       # partly done: put its stores back
                        self.dll.rt_undo(C.byref(ncpu))
                self.ref_taint = True
                uc.emu_stop()
            from unicorn import UC_HOOK_CODE
            from .core import KUSER_PAGE, TRAP_BASE, TRAP_SIZE
            ref.hook_add(UC_HOOK_CODE, on_host_code, begin=TRAP_BASE, end=TRAP_BASE + TRAP_SIZE - 1)
            ref.hook_add(UC_HOOK_CODE, on_host_code, begin=KUSER_PAGE, end=KUSER_PAGE + 0xFFF)

            def on_ref_svc(uc, intno, _):
                # a callee's entry carries our SVC patch (shared memory): run its original code instead
                cpsr = uc.reg_read(UC_ARM_REG_CPSR)
                entry = uc.reg_read(UC_ARM_REG_PC) - (2 if cpsr & 0x20 else 4)
                if intno == 2 and entry in self.tramp:
                    uc.reg_write(UC_ARM_REG_PC, self.tramp[entry])
                else:
                    self.ref_taint = True
                    uc.emu_stop()
            ref.hook_add(UC_HOOK_INTR, on_ref_svc)
            self.ref = ref
        return self.ref

    def _emulate(self, entry, cpsr, orig, ret=None, want=None):
        """Run the original function on the reference CPU from the game CPU's current registers until it
        reaches `ret` (default LR), then copy the resulting registers back to the game CPU and continue there.
        With `want` ((reg id, value) pairs: the C version's final registers), only an arrival at `ret` with those
        registers stops it: an entry that loops through `ret` (a piece of a larger function) passes it several
        times before the C version's return. Returns (bytes written, ok)."""
        ref = self._ref_cpu()
        thumb = bool(cpsr & 0x20)
        lr = self.e.reg(UC_ARM_REG_LR)
        ret = lr if ret is None else ret
        stop = ret & ~1
        for rid in REG_IDS:
            ref.reg_write(rid, self.e.reg(rid))
        ref.reg_write(UC_ARM_REG_CPSR, cpsr)
        self.vfp_buf_io.read(self.uc)
        self.vfp_buf_io.write(ref)
        patch = self._host(entry, len(self.orig[entry]))
        self._poke(entry, self.orig[entry])
        ref.ctl_remove_cache(entry, entry + 4)
        ref.ctl_remove_cache(stop, stop + 4)
        self.ref_written, self.ref_orig = set(), orig
        ok = True
        arrivals = [0]

        def on_stop(uc, addr, size, _):
            arrivals[0] += 1
            if want is None or arrivals[0] > 100_000 or all(uc.reg_read(r) == v for r, v in want):
                uc.emu_stop()
        from unicorn import UC_HOOK_CODE
        hook = ref.hook_add(UC_HOOK_CODE, on_stop, begin=stop, end=stop)
        try:
            ref.emu_start(entry | (1 if thumb else 0), 0xFFFFFFF0, count=REF_MAX_INSNS)
            if ref.reg_read(UC_ARM_REG_PC) != stop and not getattr(self, "ref_taint", False):
                self.e.log(f"!! hybrid: emulated {entry:#x} did not return within {REF_MAX_INSNS} instructions")
                ok = False
        except UcError as ex:
            self.e.log(f"!! hybrid: emulated {entry:#x} faulted: {ex}")
            ok = False
        finally:
            ref.hook_del(hook)
            written, self.ref_written, self.ref_orig = self.ref_written, None, None
            if entry in self.funcs:
                self._poke(entry, patch)
            ref.ctl_remove_cache(entry, entry + 4)
            self.uc.ctl_remove_cache(entry, entry + 4)
        for rid in REG_IDS:
            self.e.set_reg(rid, ref.reg_read(rid))
        self.vfp_buf_io.read(ref)
        self.vfp_buf_io.write(self.uc)
        rc = ref.reg_read(UC_ARM_REG_CPSR)
        self.e.set_reg(UC_ARM_REG_CPSR, (cpsr & 0x0FFFFFDF) | (rc & 0xF0000000) | (0x20 if ret & 1 else 0))
        self.e.set_reg(UC_ARM_REG_PC, ret)
        return written, ok

    def _revert(self, entry, why):
        self.mismatch[entry] = why
        self.e.log(f"!! hybrid: {entry:#x} {self.e.sym(entry)} reverted: {why}")
        if entry in self.funcs:
            del self.funcs[entry]
            self.enabled[self.index[entry]] = 0
            self._poke(entry, self.orig[entry])
            self.uc.ctl_remove_cache(entry, entry + 4)
        self.report()

    # ------------------------------------------------------------------ profile
    def _start_profile(self):
        """KHUX_PROFILE=1: count guest instructions run natively (CPU.icount) and in the emulator (block hook,
        attributed to the containing function); written to out/lift/profile.json every few seconds."""
        import bisect
        import threading
        from unicorn import UC_HOOK_BLOCK
        from .core import IMAGE_HI, IMAGE_LO
        an = json.loads((paths.OUT / "analysis.json").read_text())
        starts = sorted(int(f["addr"], 16) for f in an["functions"])
        names = {int(f["addr"], 16): f.get("name") for f in an["functions"]}
        emu_insns = {}
        self.emu_total = 0

        who = {int(x, 16) for x in os.environ.get("KHUX_PROFILE_WHO", "").split(",") if x}
        self.who_seen = {}

        def on_block(uc, addr, size, _):
            n = size // (2 if self.e.reg(UC_ARM_REG_CPSR) & 0x20 else 4) or 1
            k = bisect.bisect_right(starts, addr) - 1
            f = starts[k] if k >= 0 else addr
            if f in who and self.who_seen.get(f, 0) < 5:
                # KHUX_PROFILE_WHO=0xADDR,..: log who runs these functions in the emulator
                self.who_seen[f] = self.who_seen.get(f, 0) + 1
                sched = getattr(self.e, "sched", None)
                tid = sched.current.tid if sched is not None else 1
                self.e.log(f"?? emulated {f:#x} block {addr:#x} thread {tid} lr={self.e.reg(UC_ARM_REG_LR):#x} "
                           f"{self.e.sym(self.e.reg(UC_ARM_REG_LR))} worker_bad={f in self.worker_bad} "
                           f"spec={self.spec} depth={self.e.call_depth}")
            emu_insns[f] = emu_insns.get(f, 0) + n
            self.emu_total += n
        self.uc.hook_add(UC_HOOK_BLOCK, on_block, begin=IMAGE_LO, end=IMAGE_HI)

        def writer():
            while True:
                time.sleep(5)
                native = self.cpu.icount
                top = sorted(emu_insns.items(), key=lambda kv: -kv[1])[:40]
                total = native + self.emu_total
                out = {"native_insns": native, "emulated_insns": self.emu_total,
                       "lifted_pc_now": f"{self.cpu.lpc:#x} {self.e.sym(self.cpu.lpc)}",
                       "escapes": [[f"{a:#x}", f"{pc:#x}", self.e.sym(pc), n] for (a, pc), n in
                                   sorted(self.esc.items(), key=lambda kv: -kv[1])[:30]],
                       "worker_trampolines": [[f"{a:#x}", self.e.sym(a), n] for a, n in
                                              sorted(self.tramp_hits.items(), key=lambda kv: -kv[1])[:15]],
                       "native_share": round(native / total, 4) if total else 0,
                       "top_emulated": [[f"{f:#x}", names.get(f), n, f in self.funcs] for f, n in top]}
                (paths.OUT / "lift" / "profile.json").write_text(json.dumps(out, indent=1))
        threading.Thread(target=writer, daemon=True).start()
        self.e.log("hybrid: profiling to out/lift/profile.json")

    # ------------------------------------------------------------------ report
    def report(self):
        r = {"lifted": len(self.funcs) + len(self.mismatch), "verified": len(self.verified),
             "compared_ok": len(self.vcount), "unverifiable": len(self.unverifiable),
             "reverted": len(self.mismatch), "calls": self.calls, "native_calls": self.native_calls,
             "worker_native_calls": self.worker_native, "worker_emulated": len(self.worker_bad),
             "worker_fallbacks": {f"{a:#x} {self.e.sym(a)}": w for a, w in sorted(self.worker_bad.items())},
             "mismatches": {f"{a:#x}": w for a, w in sorted(self.mismatch.items())}}
        (paths.OUT / "lift" / "hybrid_report.json").write_text(json.dumps(r, indent=1))
        return r


def install(emu, verify=True):
    emu.hybrid = Hybrid(emu, verify=verify)
    return emu.hybrid
