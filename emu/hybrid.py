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

from unicorn import UC_HOOK_INTR, UC_HOOK_MEM_WRITE, UcError
from unicorn.arm_const import (UC_ARM_REG_CPSR, UC_ARM_REG_D0, UC_ARM_REG_FPSCR, UC_ARM_REG_LR, UC_ARM_REG_PC,
                               UC_ARM_REG_R0, UC_ARM_REG_SP)

from .. import paths
from .core import RETURN_MAGIC

REF_MAX_INSNS = 5_000_000      # a leaf that has not returned by then never will (or ran away)
SVC_IMM = 0xAB
THUMB_SVC = struct.pack("<H", 0xDF00 | SVC_IMM)
ARM_SVC = struct.pack("<I", 0xEF000000 | SVC_IMM)
WLOG_CAP = 1 << 16
VERIFY_CALLS = int(__import__("os").environ.get("KHUX_VERIFY_CALLS", "20"))   # calls compared per function
VERIFY_TRIES = 3          # inconclusive checks (store log overflow) retried this many times


class WLog(C.Structure):
    _fields_ = [("addr", C.c_uint32), ("size", C.c_uint32), ("old_lo", C.c_uint32), ("old_hi", C.c_uint32)]


class CPU(C.Structure):
    _fields_ = [("r", C.c_uint32 * 16), ("n", C.c_uint32), ("z", C.c_uint32), ("c", C.c_uint32),
                ("v", C.c_uint32), ("pc", C.c_uint32), ("logging", C.c_uint32), ("wn", C.c_uint32),
                ("wcap", C.c_uint32), ("mem", C.c_void_p), ("wlog", C.POINTER(WLog)),
                ("emu", C.c_void_p), ("tail", C.c_uint32), ("lpc", C.c_uint32), ("icount_pad", C.c_uint32),
                ("icount", C.c_uint64), ("vfp_on", C.c_uint32), ("fpscr", C.c_uint32), ("uc", C.c_void_p),
                ("uc_read_batch", C.c_void_p), ("vf", C.c_uint64 * 32)]


LiftFn = C.CFUNCTYPE(None, C.POINTER(CPU))
EmuFn = C.CFUNCTYPE(None, C.POINTER(CPU), C.c_uint32)


class Entry(C.Structure):
    _fields_ = [("addr", C.c_uint32), ("flags", C.c_uint32), ("fn", C.c_void_p)]


REG_IDS = [UC_ARM_REG_R0 + k for k in range(13)] + [UC_ARM_REG_SP, UC_ARM_REG_LR]
VFP_IDS = [UC_ARM_REG_D0 + k for k in range(32)] + [UC_ARM_REG_FPSCR]


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
        src = dll or paths.OUT / "lift" / "lift.dll"
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
        for k, ent in enumerate(table):
            a = ent.addr & ~1
            self.index[a] = k
            if executed is not None and ent.addr not in executed:
                skipped += 1          # never seen executing in this mode: leave it alone
                continue
            if not ent.flags & 1:
                rank += 1
                if not (lo_r <= rank - 1 < hi_r):
                    continue
            self.funcs[a] = (bool(ent.addr & 1), LiftFn(ent.fn), bool(ent.flags & 1))
        if executed is None:
            emu.log("hybrid: no out/coverage.json - patching every lifted function (run once with --coverage)")
        else:
            emu.log(f"hybrid: {skipped} lifted functions never executed in recorded runs, left unpatched")
        self.cpu = CPU()
        self.wlog = (WLog * WLOG_CAP)()
        self.cpu.mem = emu.host_base
        self.cpu.wlog = self.wlog
        self.cpu.wcap = WLOG_CAP
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
        self.esc = {} if os.environ.get("KHUX_PROFILE") else None   # (entry, continue pc) -> count
        self.tramp_hits = {}
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
        emu.log(f"hybrid: {len(self.funcs)} lifted functions patched (verify={'on' if verify else 'off'})")
        if os.environ.get("KHUX_PROFILE"):
            self._start_profile()

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
        if cpu.logging:
            self.taint = True                       # verification run would touch an import: abandon it
            cpu.pc = 0xFFFFFFFF
            return
        try:
            if cpu.vfp_on:
                self.vfp_io.write(self.uc)          # the callee sees the C code's VFP registers
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

    def _on_intr(self, uc, intno, _):
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

    # ------------------------------------------------------------------ verification
    def _verified_call(self, entry, f, cpsr):
        thumb = f[0]
        start_regs = [self.e.reg(r) for r in REG_IDS]
        lr = start_regs[14]
        # 1. lifted version, stores logged
        self._load_cpu(cpsr)
        self.cpu.logging, self.cpu.wn = 1, 0
        self.taint = False
        crash = None
        try:
            self._rt_run(C.byref(self.cpu), entry | (1 if thumb else 0))
        except OSError as ex:
            crash = str(ex)
        self.cpu.logging = 0
        wn = self.cpu.wn
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
        e_written, ok = self._emulate(entry, cpsr, orig, c_pc if not crash else lr)
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
        if wn > WLOG_CAP:
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
        for b in set(c_final) | set(e_written):
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

            def on_host_code(uc, addr, size, _):
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

    def _emulate(self, entry, cpsr, orig, ret=None):
        """Run the original function on the reference CPU from the game CPU's current registers until it
        reaches `ret` (default LR), then copy the resulting registers back to the game CPU and continue there.
        Returns the set of guest bytes written (first previous values recorded into `orig`), or None."""
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
        try:
            ref.emu_start(entry | (1 if thumb else 0), stop, count=REF_MAX_INSNS)
            if ref.reg_read(UC_ARM_REG_PC) != stop and not getattr(self, "ref_taint", False):
                self.e.log(f"!! hybrid: emulated {entry:#x} did not return within {REF_MAX_INSNS} instructions")
                ok = False
        except UcError as ex:
            self.e.log(f"!! hybrid: emulated {entry:#x} faulted: {ex}")
            ok = False
        finally:
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

        def on_block(uc, addr, size, _):
            n = size // (2 if self.e.reg(UC_ARM_REG_CPSR) & 0x20 else 4) or 1
            k = bisect.bisect_right(starts, addr) - 1
            f = starts[k] if k >= 0 else addr
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
             "mismatches": {f"{a:#x}": w for a, w in sorted(self.mismatch.items())}}
        (paths.OUT / "lift" / "hybrid_report.json").write_text(json.dumps(r, indent=1))
        return r


def install(emu, verify=True):
    emu.hybrid = Hybrid(emu, verify=verify)
    return emu.hybrid
