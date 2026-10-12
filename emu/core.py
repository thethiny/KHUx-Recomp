"""Unicorn-based Android loader for libcocos2dcpp.so v1.0.1.

Loads the ELF at its original addresses (so every address matches IDA / analysis.json),
leaving page 0 unmapped so NULL dereferences fault. Imports are bound to trap slots: each slot
holds an ARM `bx lr`, and a code hook on the trap region runs the Python handler first.
"""
import struct
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile
from unicorn import (UC_ARCH_ARM, UC_HOOK_CODE, UC_HOOK_MEM_FETCH_UNMAPPED, UC_HOOK_MEM_READ_UNMAPPED,
                     UC_HOOK_MEM_WRITE_UNMAPPED, UC_MODE_ARM, UC_PROT_ALL, Uc, UcError)
from unicorn.arm_const import (UC_ARM_REG_CPSR, UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0, UC_ARM_REG_R1,
                               UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_SP)

from .. import paths

R_ARM_ABS32, R_ARM_GLOB_DAT, R_ARM_JUMP_SLOT, R_ARM_RELATIVE = 2, 21, 22, 23

IMAGE_LO, IMAGE_HI = 0x1000, 0x1011000
TRAP_BASE, TRAP_SIZE = 0x0F000000, 0x40000      # import slots, 8 bytes each
RETURN_MAGIC = TRAP_BASE + TRAP_SIZE - 8          # LR for host->guest calls
HEAP_BASE, HEAP_SIZE = 0x20000000, 0xC0000000   # 3 GB: resource chunks (250 MB) are buffered in memory
STACK_BASE, STACK_SIZE = 0xE8000000, 0x800000
KUSER_PAGE = 0xFFFF0000
ARM_BX_LR = struct.pack("<I", 0xE12FFF1E)
REGS = [UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3]


class GuestFault(Exception):
    pass


class Heap:
    """Size-class free lists on top of a bump pointer; 16-byte gap before each block. The top block can
    grow in place (realloc) and is returned to the bump pointer when freed."""

    def __init__(self, emu, base, size):
        self.emu, self.ptr, self.end = emu, base, base + size
        self.free_lists = {}
        self.sizes = {}

    LARGE = 1 << 20

    def malloc(self, n):
        n = max(16, (n + 15) & ~15)
        lst = self.free_lists.get(n)
        if lst:
            p = lst.pop()
        elif n >= self.LARGE and (p := self._reuse_large(n)):
            return p
        else:
            p = self.ptr + 16
            self.ptr += n + 16
            if self.ptr > self.end:
                raise GuestFault("guest heap exhausted")
        self.sizes[p] = n
        return p

    def free(self, p):
        if not p:
            return
        n = self.sizes.pop(p, None)
        if n is None:
            self.emu.log(f"free of unknown pointer {p:#x} (lr={self.emu.reg(UC_ARM_REG_LR):#x})")
            return
        if p + n == self.ptr:      # top block: give it back to the bump pointer
            self.ptr = p - 16
            return
        self.free_lists.setdefault(n, []).append(p)

    def _reuse_large(self, n):
        """Best fit among freed large blocks (download buffers come in slightly different sizes, so exact-size
        free lists never reused them and the heap ran out). The block keeps its full size."""
        best = None
        for size, lst in self.free_lists.items():
            if size >= n and lst and (best is None or size < best):
                best = size
        if best is None or best > 2 * n:
            return 0
        p = self.free_lists[best].pop()
        self.sizes[p] = best
        return p

    def grow_in_place(self, p, n):
        """realloc fast path: the block is the most recent allocation, so it can be extended without copying
        (download buffers grow this way; copying them fragmented the heap)."""
        old = self.sizes.get(p)
        n = max(16, (n + 15) & ~15)
        if old is None or p + old != self.ptr or p + n > self.end:
            return False
        self.ptr = p + n
        self.sizes[p] = n
        return True

    def usable(self, p):
        return self.sizes.get(p, 0)


class Emu:
    def __init__(self, so=None, trace_imports=False):
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        self.trace_imports = trace_imports
        self.handlers = {}       # import name -> handler
        self.slots = {}          # slot addr -> import name
        self.slot_of = {}        # import name -> slot addr
        self.next_slot = TRAP_BASE
        self.data_imports = {}   # name -> guest addr
        self.missing = set()
        self.call_depth = 0
        self.logfile = None

        uc = self.uc
        uc.mem_map(IMAGE_LO, IMAGE_HI - IMAGE_LO, UC_PROT_ALL)
        uc.mem_map(TRAP_BASE, TRAP_SIZE, UC_PROT_ALL)
        uc.mem_write(TRAP_BASE, ARM_BX_LR * (TRAP_SIZE // 4))
        uc.mem_map(HEAP_BASE, HEAP_SIZE, UC_PROT_ALL)
        uc.mem_map(STACK_BASE, STACK_SIZE, UC_PROT_ALL)
        self.heap = Heap(self, HEAP_BASE, HEAP_SIZE)
        # enable VFP/NEON: the game is soft-float, but the SQEX sound library uses VFP instructions
        from unicorn.arm_const import UC_ARM_REG_C1_C0_2, UC_ARM_REG_FPEXC
        uc.reg_write(UC_ARM_REG_C1_C0_2, uc.reg_read(UC_ARM_REG_C1_C0_2) | (0xF << 20))
        uc.reg_write(UC_ARM_REG_FPEXC, 0x40000000)
        uc.reg_write(UC_ARM_REG_SP, STACK_BASE + STACK_SIZE - 0x100)

        self.reg, self.set_reg = self._fast_regs()
        uc.hook_add(UC_HOOK_CODE, self._on_trap, begin=TRAP_BASE, end=TRAP_BASE + TRAP_SIZE - 1)

        # Linux ARM kuser helpers (ARMv5 has no atomics): fixed addresses in the vector page
        uc.mem_map(KUSER_PAGE, 0x1000, UC_PROT_ALL)
        uc.mem_write(KUSER_PAGE, ARM_BX_LR * 0x400)
        uc.mem_write(KUSER_PAGE + 0xFFC, struct.pack("<I", 5))   # __kuser_helper_version
        self.tls = self.heap_tls = None
        uc.hook_add(UC_HOOK_CODE, self._on_kuser, begin=KUSER_PAGE + 0xF00, end=KUSER_PAGE + 0xFFB)
        uc.hook_add(UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED,
                    self._on_unmapped)
        self.load(so or paths.so())

    # ------------------------------------------------------------------ logging
    def log(self, msg):
        print(msg, flush=True)
        if self.logfile:
            self.logfile.write(msg + "\n")

    # ------------------------------------------------------------------ registers / memory
    def _fast_regs(self):
        """Direct uc_reg_read/uc_reg_write for the 32-bit core registers. The Python binding's reg_read
        resolves the register class per call (_select_reg_class), which was ~20% of a battle frame."""
        import ctypes
        from unicorn.unicorn_py3.unicorn import uclib
        from unicorn import arm_const as A
        core = {getattr(A, f"UC_ARM_REG_R{k}") for k in range(13)} | {
            A.UC_ARM_REG_SP, A.UC_ARM_REG_LR, A.UC_ARM_REG_PC, A.UC_ARM_REG_CPSR}
        buf = ctypes.c_uint32()
        ref = ctypes.byref(buf)
        h = self.uc._uch
        rd, wr = uclib.uc_reg_read, uclib.uc_reg_write
        slow_r, slow_w = self.uc.reg_read, self.uc.reg_write

        def reg(r):
            if r in core:
                rd(h, r, ref)
                return buf.value
            return slow_r(r)

        def set_reg(r, v):
            if r in core:
                buf.value = v & 0xFFFFFFFF
                wr(h, r, ref)
            else:
                slow_w(r, v & 0xFFFFFFFF)
        return reg, set_reg

    def reg(self, r):
        return self.uc.reg_read(r)

    def set_reg(self, r, v):
        self.uc.reg_write(r, v & 0xFFFFFFFF)

    def read(self, addr, n):
        return bytes(self.uc.mem_read(addr, n))

    def write(self, addr, data):
        self.uc.mem_write(addr, bytes(data))

    def u32(self, addr):
        return struct.unpack("<I", self.read(addr, 4))[0]

    def w32(self, addr, v):
        self.write(addr, struct.pack("<I", v & 0xFFFFFFFF))

    def cstr(self, addr, limit=1 << 20):
        if not addr:
            return None
        out = bytearray()
        while len(out) < limit:
            chunk = self.read(addr + len(out), 256)
            k = chunk.find(b"\0")
            if k >= 0:
                out += chunk[:k]
                return out.decode("utf-8", "replace")
            out += chunk
        return out.decode("utf-8", "replace")

    def cbytes(self, addr):
        out = bytearray()
        while True:
            chunk = self.read(addr + len(out), 256)
            k = chunk.find(b"\0")
            if k >= 0:
                return bytes(out + chunk[:k])
            out += chunk

    def alloc(self, n, data=None):
        p = self.heap.malloc(n)
        self.write(p, data if data is not None else b"\0" * n)
        return p

    def alloc_str(self, s):
        b = s.encode() if isinstance(s, str) else s
        return self.alloc(len(b) + 1, b + b"\0")

    # ------------------------------------------------------------------ loading
    def load(self, path):
        raw = Path(path).read_bytes()
        elf = ELFFile(open(path, "rb"))
        for seg in elf.iter_segments():
            if seg["p_type"] != "PT_LOAD":
                continue
            va, off, fs = seg["p_vaddr"], seg["p_offset"], seg["p_filesz"]
            lo = max(va, IMAGE_LO)   # page 0 (ELF header) stays unmapped
            self.write(lo, raw[off + lo - va:off + fs])
        syms = list(elf.get_section_by_name(".dynsym").iter_symbols())
        self.symbols = {s.name: s["st_value"] for s in syms
                        if s.name and s["st_shndx"] != "SHN_UNDEF"}
        rels = list(elf.get_section_by_name(".rel.dyn").iter_relocations()) + \
            list(elf.get_section_by_name(".rel.plt").iter_relocations())
        for r in rels:
            t, off = r["r_info_type"], r["r_offset"]
            if t == R_ARM_RELATIVE:
                continue  # loaded at original addresses: base 0, addend already correct
            s = syms[r["r_info_sym"]]
            if s["st_shndx"] != "SHN_UNDEF":
                v = s["st_value"]
            elif s["st_info"]["type"] == "STT_FUNC" or t == R_ARM_JUMP_SLOT:
                v = self.slot(s.name)
            else:
                v = self.data_import(s.name)
            if t == R_ARM_ABS32:
                v += self.u32(off)
            self.w32(off, v)
        ia = elf.get_section_by_name(".init_array")
        self.init_array = [self.u32(a) for a in range(ia["sh_addr"], ia["sh_addr"] + ia["sh_size"], 4)]

    def slot(self, name):
        if name not in self.slot_of:
            a = self.next_slot
            self.next_slot += 8
            assert self.next_slot < RETURN_MAGIC
            self.slot_of[name] = a
            self.slots[a] = name
        return self.slot_of[name]

    def data_import(self, name):
        if name not in self.data_imports:
            from . import libc
            self.data_imports[name] = libc.make_data_import(self, name)
        return self.data_imports[name]

    def host_function(self, name, fn):
        """Expose a Python function to the guest (JNI tables, callbacks); returns its guest address."""
        a = self.slot(name)
        self.handlers[name] = fn
        return a

    # ------------------------------------------------------------------ traps
    def _on_trap(self, uc, addr, size, _):
        if addr == RETURN_MAGIC:
            uc.emu_stop()
            return
        name = self.slots.get(addr)
        fn = self.handlers.get(name)
        if fn is None:
            lr = self.reg(UC_ARM_REG_LR)
            self.missing.add(name)
            self.log(f"!! unimplemented import {name} called from {lr:#x} "
                     f"(r0={self.reg(UC_ARM_REG_R0):#x} r1={self.reg(UC_ARM_REG_R1):#x})")
            self.fault = f"unimplemented import {name}"
            uc.emu_stop()
            return
        if self.trace_imports:
            self.log(f"  [{name}] r0={self.reg(UC_ARM_REG_R0):#x} r1={self.reg(UC_ARM_REG_R1):#x} "
                     f"lr={self.reg(UC_ARM_REG_LR):#x}")
        try:
            fn(self)
        except GuestFault as e:
            self.fault = str(e)
            uc.emu_stop()
        except Exception as e:
            from .threads import Block
            if isinstance(e, Block):
                try:
                    self.sched.block(self, e)
                except GuestFault as g:
                    self.fault = str(g)
                    uc.emu_stop()
                return
            self._host_exception(uc, name, e)

    def _host_exception(self, uc, name, e):
        """A bug in a Python handler: report it with guest context and stop."""
        import traceback
        self.fault = (f"host exception in {name} (lr={self.reg(UC_ARM_REG_LR):#x}): {e!r}\n"
                      + traceback.format_exc())
        self.dump()
        uc.emu_stop()

    def _set_carry(self, c):
        cpsr = self.reg(UC_ARM_REG_CPSR)
        self.set_reg(UC_ARM_REG_CPSR, (cpsr | (1 << 29)) if c else (cpsr & ~(1 << 29)))

    def _on_kuser(self, uc, addr, size, _):
        off = addr - KUSER_PAGE
        r0, r1, r2 = self.reg(UC_ARM_REG_R0), self.reg(UC_ARM_REG_R1), self.reg(UC_ARM_REG_R2)
        if off == 0xFC0:    # int __kuser_cmpxchg(old, new, ptr): 0 + C set on success
            ok = self.u32(r2) == r0
            if ok:
                self.w32(r2, r1)
            self.set_reg(UC_ARM_REG_R0, 0 if ok else 1)
            self._set_carry(ok)
        elif off == 0xF60:  # int __kuser_cmpxchg64(const u64 *old, const u64 *new, u64 *ptr)
            ok = self.read(r2, 8) == self.read(r0, 8)
            if ok:
                self.write(r2, self.read(r1, 8))
            self.set_reg(UC_ARM_REG_R0, 0 if ok else 1)
            self._set_carry(ok)
        elif off == 0xFE0:  # void *__kuser_get_tls(void)
            if self.tls is None:
                self.tls = self.heap.malloc(0x400)
                self.write(self.tls, b"\0" * 0x400)
            self.set_reg(UC_ARM_REG_R0, self.tls)
        elif off == 0xFA0:  # __kuser_memory_barrier
            pass
        else:
            self.fault = f"unknown kuser helper {addr:#x}"
            uc.emu_stop()

    def _on_unmapped(self, uc, access, addr, size, value, _):
        pc = self.reg(UC_ARM_REG_PC)
        kind = {19: "read", 20: "write", 21: "fetch"}.get(access, str(access))
        self.fault = f"unmapped {kind} at {addr:#x} (pc={pc:#x} {self.sym(pc)}, lr={self.reg(UC_ARM_REG_LR):#x})"
        self.log("!! " + self.fault)
        self.dump()
        return False

    # ------------------------------------------------------------------ debugging helpers
    def on(self, addr, cb):
        """Call cb(emu) whenever guest execution reaches addr (Thumb bit ignored)."""
        a = addr & ~1
        self.uc.hook_add(UC_HOOK_CODE, lambda uc, pc, size, _: cb(self), begin=a, end=a)

    def std_string(self, p):
        """libc++ (32-bit) std::string at guest address p."""
        b = self.read(p, 12)
        if b[0] & 1:
            size, data = struct.unpack_from("<II", b, 4)
            return self.read(data, size).decode("utf-8", "replace")
        return b[1:1 + (b[0] >> 1)].decode("utf-8", "replace")

    # ------------------------------------------------------------------ diagnostics
    def sym(self, addr):
        """'name+off' for a guest code address, using IDA's function list."""
        if not hasattr(self, "_funcs"):
            import bisect
            import re
            names = {}
            idx = paths.decompile_index()
            for line in idx.read_text().splitlines():
                m = re.match(r"\s*\d+\s+0x([0-9A-Fa-f]+)\s+(\S+)", line)
                if m:
                    names[int(m.group(1), 16)] = m.group(2)
            self._funcs = sorted(names)
            self._names = names
            self._bisect = bisect
        a = addr & ~1
        if a >= IMAGE_HI:
            name = self.slots.get(a)
            return f"<{name}>" if name else ""
        k = self._bisect.bisect_right(self._funcs, a) - 1
        if k < 0:
            return ""
        f = self._funcs[k]
        return f"{self._names[f][:70]}+{a - f:#x}"

    def dump(self):
        from unicorn.arm_const import UC_ARM_REG_R4
        regs = [self.reg(UC_ARM_REG_R0 + k) for k in range(13)]
        self.log("   " + " ".join(f"r{k}={v:#x}" for k, v in enumerate(regs)))
        sp = self.reg(UC_ARM_REG_SP)
        self.log(f"   sp={sp:#x} lr={self.reg(UC_ARM_REG_LR):#x} {self.sym(self.reg(UC_ARM_REG_LR))}")
        self.log(f"   pc={self.reg(UC_ARM_REG_PC):#x} {self.sym(self.reg(UC_ARM_REG_PC))}")
        try:
            words = struct.unpack("<256I", self.read(sp, 1024))
        except UcError:
            return
        shown = 0
        for k, w in enumerate(words):
            if 0x44EFD0 <= (w & ~1) < 0xCF9C9C and (w & 1):   # thumb return addresses
                self.log(f"   [sp+{4 * k:#x}] {w:#x} {self.sym(w)}")
                shown += 1
                if shown >= 16:
                    break

    # ------------------------------------------------------------------ calling
    def call(self, addr, *args):
        """Call guest function `addr` (Thumb bit honoured) with up to N int args; returns r0.
        Re-entrant: handlers may call back into the guest (qsort comparators, callbacks)."""
        uc = self.uc
        saved = [self.reg(r) for r in REGS] + [self.reg(UC_ARM_REG_LR), self.reg(UC_ARM_REG_PC),
                                              self.reg(UC_ARM_REG_SP), self.reg(UC_ARM_REG_CPSR)]
        sp = self.reg(UC_ARM_REG_SP)
        stack_args = list(args[4:])
        if stack_args:
            sp -= 4 * len(stack_args)
        sp &= ~7
        for k, v in enumerate(stack_args):
            self.w32(sp + 4 * k, v)
        self.set_reg(UC_ARM_REG_SP, sp)
        for r, v in zip(REGS, args[:4]):
            self.set_reg(r, v)
        self.set_reg(UC_ARM_REG_LR, RETURN_MAGIC)
        self.fault = None
        self.call_depth += 1
        try:
            uc.emu_start(addr, RETURN_MAGIC)
        except UcError as e:
            if not self.fault:
                self.fault = f"{e} at pc={self.reg(UC_ARM_REG_PC):#x} lr={self.reg(UC_ARM_REG_LR):#x}"
        finally:
            self.call_depth -= 1
        ret = self.reg(UC_ARM_REG_R0)
        if self.fault:
            raise GuestFault(self.fault)
        for r, v in zip(REGS + [UC_ARM_REG_LR], saved[:5]):
            self.set_reg(r, v)
        self.set_reg(UC_ARM_REG_SP, saved[6])
        self.set_reg(UC_ARM_REG_CPSR, saved[7])
        if self.call_depth:
            # Nested call from inside an import handler. Writing PC from a hook acts as a jump, so
            # resuming at the trap slot would re-run the handler; jump straight to the import's
            # return address instead (exactly what the slot's `bx lr` does, Thumb bit included).
            self.set_reg(UC_ARM_REG_PC, saved[4])
        return ret

    def caller(self, addr):
        """Fast repeated calls of one 2-arg guest function (qsort/bsearch comparators): registers are saved
        and restored once by the returned `done()`, each call only sets r0/r1/lr. Returns (fn, done);
        fn(a, b) gives the signed r0."""
        uc = self.uc
        saved = [self.reg(r) for r in REGS] + [self.reg(UC_ARM_REG_LR), self.reg(UC_ARM_REG_PC),
                                              self.reg(UC_ARM_REG_SP), self.reg(UC_ARM_REG_CPSR)]
        self.set_reg(UC_ARM_REG_SP, saved[6] & ~7)
        rw, rr = self.set_reg, self.reg

        def fn(a, b):
            rw(UC_ARM_REG_R0, a)
            rw(UC_ARM_REG_R1, b)
            rw(UC_ARM_REG_LR, RETURN_MAGIC)
            self.fault = None
            self.call_depth += 1
            try:
                uc.emu_start(addr, RETURN_MAGIC)
            except UcError as e:
                if not self.fault:
                    self.fault = f"{e} at pc={rr(UC_ARM_REG_PC):#x} lr={rr(UC_ARM_REG_LR):#x}"
            finally:
                self.call_depth -= 1
            if self.fault:
                raise GuestFault(self.fault)
            r = rr(UC_ARM_REG_R0)
            return r - (1 << 32) if r & 0x80000000 else r

        def done():
            for r, v in zip(REGS + [UC_ARM_REG_LR], saved[:5]):
                self.set_reg(r, v)
            self.set_reg(UC_ARM_REG_SP, saved[6])
            self.set_reg(UC_ARM_REG_CPSR, saved[7])
            if self.call_depth:
                self.set_reg(UC_ARM_REG_PC, saved[4])   # see call()
        return fn, done

    # ------------------------------------------------------------------ args / returns for handlers
    def args(self, n):
        out = [self.reg(r) for r in REGS[:min(n, 4)]]
        sp = self.reg(UC_ARM_REG_SP)
        for k in range(4, n):
            out.append(self.u32(sp + 4 * (k - 4)))
        return out

    def ret(self, v):
        self.set_reg(UC_ARM_REG_R0, v)

    def ret64(self, v):
        v &= 0xFFFFFFFFFFFFFFFF
        self.set_reg(UC_ARM_REG_R0, v)
        self.set_reg(UC_ARM_REG_R1, v >> 32)
