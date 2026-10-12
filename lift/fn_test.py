"""Run one lifted function and the original side by side, outside the game (for chasing a lifter bug).

python recomp/lift/fn_test.py 0xADDR [arm|thumb] r0 r1 r2 r3 ...

The original runs in a bare Unicorn with the library's load segments and a stack; the lifted version runs
from out/lift/lift.dll on a copy of the same memory. Prints both register sets and, with --trace, the
emulated instruction trace (address, mnemonic, registers after) to compare against the C code by hand.
"""
import ctypes as C
import struct
import sys
from pathlib import Path

from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs
from elftools.elf.elffile import ELFFile
from unicorn import UC_ARCH_ARM, UC_HOOK_CODE, UC_MODE_ARM, UC_PROT_ALL, Uc
from unicorn.arm_const import (UC_ARM_REG_C1_C0_2, UC_ARM_REG_CPSR, UC_ARM_REG_FPEXC, UC_ARM_REG_LR,
                               UC_ARM_REG_PC, UC_ARM_REG_R0, UC_ARM_REG_SP)

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import paths  # noqa: E402

IMG_LO, IMG_HI = 0x1000, 0x1011000
STACK_LO, STACK_SIZE = 0x40000000, 0x100000
STOP = 0x50000000
REGS = [UC_ARM_REG_R0 + k for k in range(13)] + [UC_ARM_REG_SP, UC_ARM_REG_LR]


def image_bytes():
    path = paths.so()
    raw = path.read_bytes()
    img = bytearray(IMG_HI - IMG_LO)
    for seg in ELFFile(open(path, "rb")).iter_segments():
        if seg["p_type"] != "PT_LOAD":
            continue
        va, off, fs = seg["p_vaddr"], seg["p_offset"], seg["p_filesz"]
        lo = max(va, IMG_LO)
        data = raw[off + lo - va:off + fs]
        img[lo - IMG_LO:lo - IMG_LO + len(data)] = data
    return bytes(img)


def run_emulated(img, entry, thumb, args, trace):
    uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    uc.mem_map(IMG_LO, IMG_HI - IMG_LO, UC_PROT_ALL)
    uc.mem_write(IMG_LO, img)
    uc.mem_map(STACK_LO, STACK_SIZE, UC_PROT_ALL)
    uc.mem_map(STOP, 0x1000, UC_PROT_ALL)
    uc.reg_write(UC_ARM_REG_C1_C0_2, uc.reg_read(UC_ARM_REG_C1_C0_2) | (0xF << 20))
    uc.reg_write(UC_ARM_REG_FPEXC, 0x40000000)
    for k, v in enumerate(args):
        uc.reg_write(UC_ARM_REG_R0 + k, v)
    uc.reg_write(UC_ARM_REG_SP, STACK_LO + STACK_SIZE - 0x100)
    uc.reg_write(UC_ARM_REG_LR, STOP)
    lines = []
    if trace:
        cs = {False: Cs(CS_ARCH_ARM, CS_MODE_ARM), True: Cs(CS_ARCH_ARM, CS_MODE_THUMB)}

        def on_code(u, addr, size, _):
            t = bool(u.reg_read(UC_ARM_REG_CPSR) & 0x20)
            ins = next(cs[t].disasm(bytes(u.mem_read(addr, size)), addr), None)
            regs = " ".join(f"{u.reg_read(UC_ARM_REG_R0 + k):08x}" for k in range(8))
            c = u.reg_read(UC_ARM_REG_CPSR) >> 28
            lines.append(f"{addr:#x} {ins.mnemonic if ins else '?'} {ins.op_str if ins else ''} | before r0-7 {regs} nzcv {c:04b}")
        uc.hook_add(UC_HOOK_CODE, on_code)
    uc.emu_start(entry | (1 if thumb else 0), STOP, count=1_000_000)
    return [uc.reg_read(r) for r in REGS], uc.reg_read(UC_ARM_REG_PC), lines


def run_lifted(img, entry, thumb, args):
    sys.path.insert(0, str(HERE.parent / "emu"))
    import importlib.util
    spec = importlib.util.spec_from_file_location("hyb", HERE.parent / "emu" / "hybrid.py")
    # reuse the struct definitions without importing the emulator package
    src = (HERE.parent / "emu" / "hybrid.py").read_text(encoding="utf-8")
    ns = {}
    ns["C"] = C
    exec(src[src.index("class WLog"):src.index("REG_IDS")], ns)
    CPU, Entry, EmuFn = ns["CPU"], ns["Entry"], ns["EmuFn"]
    dll = C.CDLL(str(paths.OUT / "lift" / "lift.dll"))
    n = C.c_int.in_dll(dll, "lift_count").value
    enabled = (C.c_uint8 * n).in_dll(dll, "lift_enabled")
    for k in range(n):
        enabled[k] = 1
    # a private 4 GB-addressable block: reserve, then commit the image and the stack
    k32 = C.WinDLL("kernel32")
    k32.VirtualAlloc.restype = C.c_void_p
    k32.VirtualAlloc.argtypes = (C.c_void_p, C.c_size_t, C.c_uint32, C.c_uint32)
    base = k32.VirtualAlloc(None, 1 << 32, 0x2000, 0x04)
    for lo, size in ((IMG_LO, IMG_HI - IMG_LO), (STACK_LO, STACK_SIZE), (STOP, 0x1000)):
        k32.VirtualAlloc(base + lo, size, 0x1000, 0x04)
    C.memmove(base + IMG_LO, img, len(img))
    cpu = CPU()
    cpu.mem = base
    cpu.vfp_on = 1            # VFP registers start at zero, as in the bare Unicorn
    for k, v in enumerate(args):
        cpu.r[k] = v
    cpu.r[13] = STACK_LO + STACK_SIZE - 0x100
    cpu.r[14] = STOP
    calls = []

    def emu(cpu_p, target):
        calls.append(target)
        cpu_p.contents.pc = 0xFFFFFFFF
    cb = EmuFn(emu)
    cpu.emu = C.cast(cb, C.c_void_p)
    dll.rt_run(C.byref(cpu), entry | (1 if thumb else 0))
    return [cpu.r[k] for k in range(15)], cpu.pc, calls


def main():
    entry = int(sys.argv[1], 16)
    thumb = len(sys.argv) > 2 and sys.argv[2] == "thumb"
    args = [int(a, 0) for a in sys.argv[3:] if a not in ("arm", "thumb", "--trace")]
    trace = "--trace" in sys.argv
    img = image_bytes()
    e_regs, e_pc, lines = run_emulated(img, entry, thumb, args, trace)
    c_regs, c_pc, calls = run_lifted(img, entry, thumb, args)
    print(f"emulated: pc={e_pc:#x} " + " ".join(f"r{k}={v:#x}" for k, v in enumerate(e_regs)))
    print(f"lifted:   pc={c_pc:#x} " + " ".join(f"r{k}={v:#x}" for k, v in enumerate(c_regs)))
    if calls:
        print("lifted code called the emulator for:", [hex(t) for t in calls])
    for k in range(13):
        if e_regs[k] != c_regs[k]:
            print(f"  r{k} differs: emu {e_regs[k]:#x} C {c_regs[k]:#x}")
    if trace:
        print("\n".join(lines))


if __name__ == "__main__":
    main()
