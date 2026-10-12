"""M0 analyzer for the v1.0.1 libcocos2dcpp.so static recompilation.

Reads the ELF + IDA export and produces:
  recomp/out/analysis.json  - functions (addr, mode, end), imports, PLT map,
                              init_array, JNI exports, switch tables, indirect sites
  recomp/out/report.md      - coverage report for the lifter

Function mode (ARM/Thumb) is not recorded by IDA's index nor by .ARM.exidx,
so it is resolved from: dynsym low bit -> pointer relocations -> call graph
propagation (BL keeps mode, BLX imm switches) -> trial decode.

Usage: python recomp/analyze.py [path/to/libcocos2dcpp.so]
"""
import bisect
import json
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs
from capstone import arm as A
from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
import paths  # noqa: E402

SO = Path(sys.argv[1]) if len(sys.argv) > 1 else paths.so()
IDA = paths.decompile_dir()
OUT = paths.OUT

ARM, THUMB = "arm", "thumb"
R_ARM_ABS32, R_ARM_GLOB_DAT, R_ARM_JUMP_SLOT, R_ARM_RELATIVE = 2, 21, 22, 23
CC_NONE = (A.ARM_CC_AL, A.ARM_CC_INVALID)

# libgcc Thumb-1 switch helpers: BL helper; table follows the BL inline.
# value = (entry size, signed)
CASE_HELPERS = {
    "__gnu_thumb1_case_sqi": (1, True),
    "__gnu_thumb1_case_uqi": (1, False),
    "__gnu_thumb1_case_shi": (2, True),
    "__gnu_thumb1_case_uhi": (2, False),
    "__gnu_thumb1_case_si": (4, True),
}


# calls that never return; the compiler puts padding/literals right after them
NORETURN = {"__stack_chk_fail", "_Unwind_Resume", "__cxa_throw", "__cxa_rethrow", "abort", "exit",
            "_exit", "longjmp", "siglongjmp", "_ZSt9terminatev", "__assert2", "png_error",
            "png_chunk_error", "png_fixed_error", "__cxa_bad_cast", "__cxa_bad_typeid",
            "__cxa_call_unexpected", "__cxa_pure_virtual", "_ZSt17__throw_bad_allocv"}


class Image:
    def __init__(self, path):
        self.raw = path.read_bytes()
        self.elf = ELFFile(open(path, "rb"))
        self.loads = [(s["p_vaddr"], s["p_offset"], s["p_filesz"])
                      for s in self.elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        self.sec = {s.name: (s["sh_addr"], s["sh_addr"] + s["sh_size"])
                    for s in self.elf.iter_sections() if s["sh_addr"]}

    def off(self, va):
        for v, o, n in self.loads:
            if v <= va < v + n:
                return o + va - v
        return None

    def read(self, va, n):
        o = self.off(va)
        return None if o is None else self.raw[o:o + n]

    def u32(self, va):
        b = self.read(va, 4)
        return None if b is None or len(b) < 4 else struct.unpack("<I", b)[0]

    def in_sec(self, va, name):
        lo, hi = self.sec[name]
        return lo <= va < hi


def load_ida_functions():
    funcs = {}
    for line in (IDA / "index.txt").read_text().splitlines():
        if line.startswith("#") or not line.strip():
            continue
        _, ea, name = line.split(None, 2)
        funcs[int(ea, 16)] = name
    names = {}
    for line in (IDA / "names.txt").read_text(errors="replace").splitlines():
        if line.startswith("0x"):
            ea, name = line.split(None, 1)
            names.setdefault(name, int(ea, 16))
    return funcs, names


def arm_imm(op_str):
    """Last immediate of an operand string; capstone prints some modified
    immediates as '#imm8, #rot' which means imm8 ROR rot."""
    parts = [int(p.split()[0].rstrip("]!,"), 0) for p in op_str.split("#")[1:]]
    if len(parts) == 2:
        v, r = parts
        return ((v >> r) | (v << (32 - r))) & 0xFFFFFFFF
    return parts[-1]


def parse_dynamic(img):
    syms = list(img.elf.get_section_by_name(".dynsym").iter_symbols())
    defined, imports = {}, {}
    for s in syms:
        if not s.name:
            continue
        if s["st_shndx"] == "SHN_UNDEF":
            imports[s.name] = s["st_info"]["type"]
        elif s["st_info"]["type"] == "STT_FUNC":
            defined[s.name] = s["st_value"]

    # PLT stubs: header 20 bytes, then 12-byte stubs "add ip,pc,#a; add ip,ip,#b; ldr pc,[ip,#c]!"
    got_to_sym = {}
    for r in img.elf.get_section_by_name(".rel.plt").iter_relocations():
        assert r["r_info_type"] == R_ARM_JUMP_SLOT
        got_to_sym[r["r_offset"]] = syms[r["r_info_sym"]]
    md = Cs(CS_ARCH_ARM, CS_MODE_ARM)
    plt_lo, plt_hi = img.sec[".plt"]
    plt = {}  # stub addr -> {"name", "target" (internal, with thumb bit) | None}
    for stub in range(plt_lo + 20, plt_hi, 12):
        ins = list(md.disasm(img.read(stub, 12), stub))
        assert [i.mnemonic for i in ins] == ["add", "add", "ldr"], hex(stub)
        imm = [arm_imm(i.op_str) for i in ins]
        got = (stub + 8 + imm[0] + imm[1] + imm[2]) & 0xFFFFFFFF
        s = got_to_sym[got]
        internal = s["st_shndx"] != "SHN_UNDEF"
        plt[stub] = {"name": s.name, "got": got, "target": s["st_value"] if internal else None}

    reloc = Counter()
    fptrs = {}  # data address -> code pointer (with thumb bit) from RELATIVE/ABS32
    text_lo, text_hi = img.sec[".text"]
    for r in img.elf.get_section_by_name(".rel.dyn").iter_relocations():
        t = r["r_info_type"]
        reloc[t] += 1
        if t == R_ARM_RELATIVE:
            v = img.u32(r["r_offset"])
        elif t in (R_ARM_ABS32, R_ARM_GLOB_DAT):
            s = syms[r["r_info_sym"]]
            if s["st_shndx"] == "SHN_UNDEF" or s["st_info"]["type"] != "STT_FUNC":
                continue
            v = s["st_value"] + (img.u32(r["r_offset"]) if t == R_ARM_ABS32 else 0)
        else:
            continue
        if v is not None and text_lo <= (v & ~1) < text_hi:
            fptrs[r["r_offset"]] = v

    ia_lo, ia_hi = img.sec[".init_array"]
    init_array = [img.u32(a) for a in range(ia_lo, ia_hi, 4)]
    return defined, imports, plt, reloc, fptrs, init_array


class Disasm:
    """Recursive-descent decoder over one function."""

    def __init__(self, img):
        self.img = img
        self.cs = {}
        for mode, m in ((ARM, CS_MODE_ARM), (THUMB, CS_MODE_THUMB)):
            c = Cs(CS_ARCH_ARM, m)
            c.detail = True
            self.cs[mode] = c

    def one(self, addr, mode):
        b = self.img.read(addr, 4)
        if not b:
            return None
        for i in self.cs[mode].disasm(b, addr, count=1):
            return i
        return None


def pc_written(i):
    if i.id in (A.ARM_INS_POP, A.ARM_INS_LDM, A.ARM_INS_LDMIB, A.ARM_INS_LDMDA, A.ARM_INS_LDMDB):
        return any(o.type == A.ARM_OP_REG and o.reg == A.ARM_REG_PC for o in i.operands)
    return bool(i.operands) and i.operands[0].type == A.ARM_OP_REG and \
        i.operands[0].reg == A.ARM_REG_PC and i.id not in (A.ARM_INS_STR, A.ARM_INS_PUSH, A.ARM_INS_STM,
                                                          A.ARM_INS_CMP, A.ARM_INS_TST)


def analyze_function(d, entry, mode, lo, hi, case_helpers, fn_starts, plt_range, noreturn=frozenset(),
                     fragments=frozenset()):
    """Recursive descent over one function. lo/hi = function boundary."""
    res = {"insns": 0, "calls": [], "switches": [], "indirect_jumps": [], "indirect_calls": 0,
           "tail_calls": [], "issues": [], "mnem": Counter(), "data": set(), "max": entry,
           "noreturn_callees": [], "far_branches": [], "sizes": {}, "returns": False,
           "push_lr": False, "pop_pc": False,
           "code_refs": []}
    seen = set()
    work = [(entry, None)]
    plt_lo, plt_hi = plt_range
    while work:
        pc, cmp_state = work.pop()
        popped = ()        # registers loaded by the previous pop (vararg "pop {r3}; bx r3" returns)
        adr = None         # (reg, value) of the last adr (thumb inline switch)
        after_call = None  # callee of the call that immediately precedes pc
        lits = {}          # reg -> literal-pool word loaded into it (PIC "ldr rX,=off; add rX,pc")
        while pc not in seen:
            stop = None
            i = None
            if pc != entry and pc in fn_starts or plt_lo <= pc < plt_hi:
                res["tail_calls"].append(pc)
                if pc in fragments:
                    res["returns"] = True
                break
            if not (lo <= pc < hi):
                stop = "leave"
            elif pc in res["data"]:
                stop = "flow into literal"
            else:
                i = d.one(pc, mode)
                if i is None:
                    stop = "undecodable"
            if stop:
                if after_call is not None:
                    res["noreturn_callees"].append(after_call)
                elif stop == "leave":
                    if pc in fn_starts or plt_lo <= pc < plt_hi:
                        res["tail_calls"].append(pc)
                    else:
                        res["issues"].append(f"flow leaves function at {pc:#x}")
                else:
                    res["issues"].append(f"{stop} at {pc:#x}")
                break
            if not (i.id == A.ARM_INS_MOV and i.op_str == "r8, r8"):  # thumb nop padding
                after_call = None
            seen.add(pc)
            res["sizes"][pc] = i.size
            res["insns"] += 1
            res["max"] = max(res["max"], pc + i.size)
            res["mnem"][i.insn_name()] += 1
            if mode == THUMB and i.size == 4 and i.id not in (A.ARM_INS_BL, A.ARM_INS_BLX):
                res["issues"].append(f"thumb2 insn {i.mnemonic} {i.op_str} at {pc:#x}")
            nxt = pc + i.size
            cond = i.cc not in CC_NONE
            ops = i.operands
            prev_popped, popped = popped, ()

            # literal pool references
            for o in ops:
                if o.type == A.ARM_OP_MEM and o.mem.base == A.ARM_REG_PC and o.mem.index == 0 \
                        and i.id in (A.ARM_INS_LDR, A.ARM_INS_LDRB, A.ARM_INS_LDRH, A.ARM_INS_LDRD):
                    base = (pc + 4) & ~3 if mode == THUMB else pc + 8
                    lit = base + o.mem.disp
                    for k in range(0, 8 if i.id == A.ARM_INS_LDRD else 4, 2):
                        res["data"].add(lit + k)
            if i.id == A.ARM_INS_CMP and len(ops) == 2 and ops[1].type == A.ARM_OP_IMM:
                cmp_state = (ops[0].reg, ops[1].imm)
            if i.id == A.ARM_INS_ADD and ops and ops[0].type == A.ARM_OP_REG and ops[0].reg in lits and                     any(o.type == A.ARM_OP_REG and o.reg == A.ARM_REG_PC for o in ops[1:]):
                res["code_refs"].append((lits[ops[0].reg] + pc + (4 if mode == THUMB else 8)) & 0xFFFFFFFF)
            if ops and ops[0].type == A.ARM_OP_REG and i.id not in (
                    A.ARM_INS_STR, A.ARM_INS_STRB, A.ARM_INS_STRH, A.ARM_INS_CMP, A.ARM_INS_CMN, A.ARM_INS_TST):
                lits.pop(ops[0].reg, None)
            if i.id == A.ARM_INS_LDR and len(ops) == 2 and ops[1].type == A.ARM_OP_MEM and                     ops[1].mem.base == A.ARM_REG_PC and ops[1].mem.index == 0:
                base = (pc + 4) & ~3 if mode == THUMB else pc + 8
                v = d.img.u32(base + ops[1].mem.disp)
                if v is not None:
                    lits[ops[0].reg] = v
            if i.id == A.ARM_INS_ADR:
                adr = (ops[0].reg, ops[1].imm if mode == ARM else ((pc + 4) & ~3) + ops[1].imm)
            if i.id == A.ARM_INS_POP and not pc_written(i):
                popped = tuple(o.reg for o in ops)
            if i.id not in (A.ARM_INS_BX, A.ARM_INS_MOV) and                     any(o.type == A.ARM_OP_REG and o.reg == A.ARM_REG_LR for o in ops):
                res["push_lr"] = True  # lr saved (push/stmdb/str lr,[sp,#-4]!) or otherwise used
            if i.id in (A.ARM_INS_POP, A.ARM_INS_LDM) and pc_written(i):
                res["pop_pc"] = True

            if i.id == A.ARM_INS_B:
                tgt = ops[0].imm
                if cond:
                    work.append((tgt, cmp_state))
                    pc = nxt
                else:
                    pc = tgt
                continue
            if i.id in (A.ARM_INS_BL, A.ARM_INS_BLX) and ops[0].type == A.ARM_OP_IMM:
                tgt = ops[0].imm
                tmode = mode if i.id == A.ARM_INS_BL else (THUMB if mode == ARM else ARM)
                if mode == THUMB and tgt in case_helpers:
                    size, signed = case_helpers[tgt]
                    if cmp_state is None:
                        res["issues"].append(f"thumb switch without bound at {pc:#x}")
                        break
                    n = cmp_state[1] + 1
                    tbl = (nxt + 3) & ~3 if size == 4 else nxt  # case_si aligns its table
                    fmt = {1: "b", 2: "h", 4: "i"}[size]
                    fmt = fmt if signed else fmt.upper()
                    vals = struct.unpack(f"<{n}{fmt}", d.img.read(tbl, n * size))
                    if size == 4:  # si: offsets relative to table start
                        tgts = [tbl + v for v in vals]
                    else:  # offsets in halfwords relative to table start
                        tgts = [tbl + 2 * v for v in vals]
                    res["switches"].append({"at": pc, "kind": f"thumb_{size}", "n": n, "targets": tgts})
                    end = tbl + n * size
                    for a in range(tbl, (end + 1) & ~1, 2):
                        res["data"].add(a)
                    work.extend((t, None) for t in tgts)
                    break
                if lo <= tgt < hi and tgt not in fn_starts:
                    # Thumb-1 far branch: BL into the same function, LR is dead
                    res["far_branches"].append(pc)
                    pc = tgt
                    continue
                res["calls"].append((tgt, tmode))
                # a conditional call (ARM bleq, ...) to a fragment or noreturn function still falls through
                # when the condition fails, e.g. __divdf3's `bleq` into the shared special-case handler
                if tgt in fragments:  # far branch into our own epilogue: it returns for us
                    res["returns"] = True
                    if not cond:
                        break
                    pc = nxt
                    continue
                if tgt in noreturn:
                    if not cond:
                        break
                    pc = nxt
                    continue
                after_call = tgt
                pc = nxt
                continue
            if i.id == A.ARM_INS_BLX:  # register
                res["indirect_calls"] += 1
                pc = nxt
                continue
            if i.id == A.ARM_INS_BX:
                r = ops[0].reg
                if r != A.ARM_REG_LR and r not in prev_popped:
                    res["indirect_jumps"].append(pc)
                else:
                    res["returns"] = True
                if cond:
                    pc = nxt
                    continue
                break
            if pc_written(i):
                # ARM jump tables
                if mode == ARM and i.id == A.ARM_INS_LDR and ops[1].type == A.ARM_OP_MEM \
                        and ops[1].mem.base == A.ARM_REG_PC and ops[1].mem.index and cmp_state:
                    n = cmp_state[1] + 1
                    tbl = pc + 8
                    tgts = [d.img.u32(tbl + 4 * k) for k in range(n)]
                    res["switches"].append({"at": pc, "kind": "arm_ldr", "n": n, "targets": tgts})
                    for k in range(n):
                        res["data"].update(range(tbl + 4 * k, tbl + 4 * k + 4, 2))
                    work.extend((t & ~1, None) for t in tgts)
                    if cond:
                        res["issues"].append(f"conditional ldr-pc switch fallthrough at {pc:#x}")
                    break
                if mode == ARM and i.id == A.ARM_INS_ADD and len(ops) == 3 and ops[1].reg == A.ARM_REG_PC \
                        and ops[2].type == A.ARM_OP_REG and cmp_state:
                    # addls pc, pc, rX, lsl #2 ; b default ; b case0 ; b case1 ...
                    n = cmp_state[1] + 1
                    tgts = [pc + 8 + 4 * k for k in range(n)]
                    res["switches"].append({"at": pc, "kind": "arm_add", "n": n, "targets": tgts})
                    work.extend((t, None) for t in tgts)
                    if cond:
                        pc = nxt
                        continue
                    break
                if mode == THUMB and i.id == A.ARM_INS_MOV and adr and cmp_state:
                    # lsls r0,#2; adr r1,T; adds r0,r1; ldr r0,[r0]; adds r0,r1; mov pc,r0
                    n = cmp_state[1] + 1
                    tbl = adr[1]
                    vals = struct.unpack(f"<{n}i", d.img.read(tbl, 4 * n))
                    tgts = [(tbl + v) & ~1 for v in vals]
                    res["switches"].append({"at": pc, "kind": "thumb_adr", "n": n, "table": tbl,
                                            "targets": tgts})
                    res["data"].update(range(tbl, tbl + 4 * n, 2))
                    work.extend((t, None) for t in tgts)
                    break
                is_ret = i.id in (A.ARM_INS_POP, A.ARM_INS_LDM) or \
                    (i.id == A.ARM_INS_MOV and ops[1].type == A.ARM_OP_REG and
                     (ops[1].reg == A.ARM_REG_LR or ops[1].reg in prev_popped)) or \
                    (i.id == A.ARM_INS_LDR and ops[1].mem.base == A.ARM_REG_SP)
                if not is_ret:
                    res["indirect_jumps"].append(pc)
                else:
                    res["returns"] = True
                if cond:
                    pc = nxt
                    continue
                break
            pc = nxt
    return res


def dump_readable(analysis):
    """JSON with one record per line and addresses as hex strings (readable in an editor)."""
    h = hex
    a = dict(analysis)
    a["functions"] = [{**f, "addr": h(f["addr"]), "end": h(f["end"])} for f in a["functions"]]
    a["plt"] = {k: {**v, "got": h(v["got"]), "target": h(v["target"]) if v["target"] is not None else None}
                for k, v in a["plt"].items()}
    a["init_array"] = [h(x) for x in a["init_array"]]
    a["jni"] = {k: h(v) for k, v in a["jni"].items()}
    a["switches"] = [{**sw, "at": h(sw["at"]), "targets": [h(t) for t in sw["targets"]],
                      **({"table": h(sw["table"])} if "table" in sw else {})} for sw in a["switches"]]
    a["indirect_jumps"] = [[h(f), h(j)] for f, j in a["indirect_jumps"]]
    a["noreturn"] = [h(x) for x in a["noreturn"]]
    a["fragments"] = [h(x) for x in a["fragments"]]
    if "prologue_gaps" in a:
        a["prologue_gaps"] = [[h(x), h(n)] for x, n in a["prologue_gaps"]]
    out = ["{"]
    keys = list(a)
    for ki, k in enumerate(keys):
        v = a[k]
        tail = "," if ki < len(keys) - 1 else ""
        if isinstance(v, list) and v:
            out.append(f" {json.dumps(k)}: [")
            out += [f"  {json.dumps(x)}{',' if i < len(v) - 1 else ''}" for i, x in enumerate(v)]
            out.append(f" ]{tail}")
        elif isinstance(v, dict) and v:
            out.append(f" {json.dumps(k)}: {{")
            items = list(v.items())
            out += [f"  {json.dumps(kk)}: {json.dumps(x)}{',' if i < len(items) - 1 else ''}"
                    for i, (kk, x) in enumerate(items)]
            out.append(f" }}{tail}")
        else:
            out.append(f" {json.dumps(k)}: {json.dumps(v)}{tail}")
    out.append("}")
    return "\n".join(out) + "\n"


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    img = Image(SO)
    ida_funcs, ida_names = load_ida_functions()
    defined, imports, plt, reloc, fptrs, init_array = parse_dynamic(img)
    text_lo, text_hi = img.sec[".text"]

    # ---- function set: IDA functions inside .text, plus any symbol/pointer target IDA missed
    mode = {}
    src = {}
    for name, v in defined.items():
        a = v & ~1
        if text_lo <= a < text_hi:
            mode[a] = THUMB if v & 1 else ARM
            src[a] = "dynsym"
    for v in list(fptrs.values()) + init_array + [p["target"] for p in plt.values() if p["target"]]:
        a = v & ~1
        if text_lo <= a < text_hi and a not in mode:
            mode[a] = THUMB if v & 1 else ARM
            src[a] = "pointer"
    starts = {a for a in ida_funcs if text_lo <= a < text_hi}
    missing_from_ida = sorted(a for a in mode if a not in starts)
    starts |= set(mode)
    order = sorted(starts)
    fn_starts = set(order)

    def bound(a):
        k = bisect.bisect_right(order, a)
        return order[k] if k < len(order) else text_hi

    case_helpers = {}
    for name, (sz, sg) in CASE_HELPERS.items():
        a = defined.get(name, ida_names.get(name))
        if a is not None:
            case_helpers[a & ~1] = (sz, sg)
            # calls may go through PLT stubs too
    for stub, p in plt.items():
        if p["name"] in CASE_HELPERS and p["target"]:
            case_helpers[stub] = CASE_HELPERS[p["name"]]

    d = Disasm(img)
    results = {}

    noreturn = set()
    fragments = set()
    for name in NORETURN:
        if name in defined:
            noreturn.add(defined[name] & ~1)
        if name in ida_names:
            noreturn.add(ida_names[name])
    noreturn |= {stub for stub, p in plt.items() if p["name"] in NORETURN}

    def descend(a, m):
        return analyze_function(d, a, m, text_lo, text_hi, case_helpers, fn_starts, img.sec[".plt"], noreturn,
                                fragments)

    def run(a, m):
        r = descend(a, m)
        results[a] = r
        return r

    # ---- propagate mode through the call graph
    pcrel_added = 0
    queue = [a for a in order if a in mode]
    done = set()
    while True:
        callers_mode = defaultdict(Counter)
        plt_lo, plt_hi = img.sec[".plt"]
        while True:
            while queue:
                a = queue.pop()
                if a in done:
                    continue
                done.add(a)
                r = run(a, mode[a])
                for tgt, tm in r["calls"]:
                    if plt_lo <= tgt < plt_hi:
                        continue
                    if tgt in fn_starts and tgt not in mode:
                        callers_mode[tgt][tm] += 1
                for tgt in r["tail_calls"]:
                    if tgt not in mode:
                        callers_mode[tgt][mode[a]] += 1
                for sw in r["switches"]:
                    pass
                for tgt, c in list(callers_mode.items()):
                    if tgt not in mode:
                        mode[tgt] = c.most_common(1)[0][0]
                        src[tgt] = "callgraph"
                        queue.append(tgt)
                callers_mode.clear()
            # ---- trial decode for the rest
            rest = [a for a in order if a not in mode]
            if not rest:
                break
            print(f"trial-decoding {len(rest)} functions with unknown mode", flush=True)
            for a in rest:
                best = None
                for m in ((THUMB, ARM) if a % 4 == 0 else (THUMB,)):
                    r = descend(a, m)
                    score = len(r["issues"]) * 1000 - r["insns"]
                    if best is None or score < best[0]:
                        best = (score, m, r)
                mode[a] = best[1]
                src[a] = "trial"
                results[a] = best[2]
                done.add(a)
                for tgt, tm in best[2]["calls"]:
                    if tgt in fn_starts and tgt not in mode:
                        mode[tgt] = tm
                        src[tgt] = "callgraph"
                        queue.append(tgt)
            if not queue:
                break

        # functions only reachable through PIC address computations ("ldr rX,=off; add rX,pc")
        new_refs = {}
        for r in results.values():
            for v in r["code_refs"]:
                t = v & ~1
                if text_lo <= t < text_hi and t not in fn_starts:
                    new_refs[t] = v
        if not new_refs:
            break
        for t, v in new_refs.items():
            mode[t] = THUMB if v & 1 else ARM
            src[t] = "pcrel"
            fn_starts.add(t)
            queue.append(t)
        pcrel_added += len(new_refs)
        order[:] = sorted(fn_starts)
        print(f"added {len(new_refs)} functions from pc-relative code references", flush=True)

    # ---- noreturn inference (fixpoint): a function never returns if no path reaches a
    # return, an indirect jump, or a tail call into a returning function.
    stub_target = {stub: p["target"] & ~1 for stub, p in plt.items() if p["target"]}

    def resolve(t):
        return stub_target.get(t, t)

    callers = defaultdict(set)
    for a, r in results.items():
        for t, _ in r["calls"]:
            callers[resolve(t)].add(a)
        for t in r["tail_calls"]:
            callers[resolve(t)].add(a)
    def is_fragment(r):
        return not r["push_lr"] and (r["pop_pc"] or any(resolve(t) in fragments for t in r["tail_calls"]))

    inferred = 0
    changed = True
    while changed:
        changed = False
        new = []
        for a, r in results.items():
            if a in noreturn or a in fragments:
                continue
            if is_fragment(r):
                # frame fragment: IDA split a far-branch target of a big Thumb function
                # into its own "function"; it returns to its parent's caller, never to us
                new.append(a)
                continue
            if r["returns"] or r["indirect_jumps"] or r["issues"]:
                continue
            if all(resolve(t) in noreturn for t in r["tail_calls"]):
                new.append(a)
        if not new:
            break
        for a in new:
            r = results[a]
            if is_fragment(r):
                fragments.add(a)
            else:
                noreturn.add(a)
            inferred += 1
        noreturn |= {stub for stub, t in stub_target.items() if t in noreturn}
        fragments |= {stub for stub, t in stub_target.items() if t in fragments}
        rerun = set().union(*(callers[a] for a in new))
        for a in rerun:
            r = run(a, mode[a])
            for t, _ in r["calls"]:
                callers[resolve(t)].add(a)
            for t in r["tail_calls"]:
                callers[resolve(t)].add(a)
        changed = True
    print(f"noreturn: {inferred} inferred ({len(fragments)} frame fragments), {len(noreturn)} total", flush=True)

    # ---- report
    total = Counter()
    issues = Counter()
    issue_funcs = []
    switches = []
    ind_jumps = []
    ind_calls = 0
    coverage_bytes = 0
    noret = Counter()
    code_bytes = {}
    shared = 0
    issue_samples = defaultdict(list)
    for a in order:
        r = results[a]
        total.update(r["mnem"])
        ind_calls += r["indirect_calls"]
        ind_jumps += [(a, j) for j in r["indirect_jumps"]]
        switches += r["switches"]
        for k, v in r["sizes"].items():
            if k in code_bytes:
                shared += 1
            code_bytes[k] = v
        r["sizes"] = None
        for t in r["noreturn_callees"]:
            noret[plt[t]["name"] if t in plt else ida_funcs.get(t, hex(t))] += 1
        for s in r["issues"]:
            k = "thumb2" if s.startswith("thumb2") else s.split(" at ")[0]
            issues[k] += 1
            if len(issue_samples[k]) < 15:
                issue_samples[k].append(f"{a:#x} {ida_funcs.get(a, '?')[:60]} ({mode[a]}): {s}")
        if r["issues"]:
            issue_funcs.append((a, ida_funcs.get(a, "?"), mode[a], r["issues"][:3]))

    jni = {n: v for n, v in defined.items() if n.startswith("Java_") or n == "JNI_OnLoad"}
    real_imports = sorted({p["name"] for p in plt.values() if p["target"] is None})
    analysis = {
        "binary": str(SO.name),
        "sections": {k: [hex(v[0]), hex(v[1])] for k, v in img.sec.items()},
        "functions": [{"addr": a, "mode": mode[a], "end": bound(a), "src": src.get(a, "ida"),
                       "name": ida_funcs.get(a)} for a in order],
        "plt": {hex(k): v for k, v in plt.items()},
        "imports": real_imports,
        "data_imports": sorted(n for n, t in imports.items() if t != "STT_FUNC"),
        "init_array": init_array,
        "jni": jni,
        "switches": switches,
        "indirect_jumps": [[a, j] for a, j in ind_jumps],
        "noreturn": sorted(noreturn),
        "fragments": sorted(fragments),
    }
    (OUT / "analysis.json").write_text(dump_readable(analysis))

    # ---- gaps: .text ranges that are neither decoded code nor known data
    data_all = set()
    for r in results.values():
        data_all |= r["data"]
    covered = bytearray(text_hi - text_lo)
    for k, v in code_bytes.items():
        covered[k - text_lo:k - text_lo + v] = b"" * v
    for k in data_all:
        if text_lo <= k < text_hi - 1:
            covered[k - text_lo:k - text_lo + 2] = b""
    gaps = []
    k = 0
    n = len(covered)
    while k < n:
        if covered[k]:
            k += 1
            continue
        j = k
        while j < n and not covered[j]:
            j += 1
        gaps.append((text_lo + k, j - k))
        k = j
    big = sorted(gaps, key=lambda g: -g[1])[:40]
    # gaps that start (after alignment padding) with a function prologue = likely dead or missed functions
    prologue_gaps = []
    for ga, gn in gaps:
        for off in (0, 2):
            h = img.read(ga + off, 2)
            if h and gn > off + 4 and h[1] == 0xB5:  # thumb push {..., lr}
                prologue_gaps.append((ga + off, gn - off))
                break
            w = img.u32(ga + off) if (ga + off) % 4 == 0 else None
            if w and gn > off + 4 and (w & 0xFFFF4000) == 0xE92D4000:  # arm stmdb sp!, {..., lr}
                prologue_gaps.append((ga + off, gn - off))
                break
    analysis["prologue_gaps"] = prologue_gaps
    (OUT / "analysis.json").write_text(dump_readable(analysis))
    gap_lines = [f"- data bytes (literals/tables): {covered.count(2)}; unaccounted: {sum(g[1] for g in gaps)} "
                 f"in {len(gaps)} gaps ({sum(1 for g in gaps if g[1] <= 2)} are <=2-byte alignment padding)"]
    gap_lines += [f"- gaps starting with a push-lr prologue (unreferenced/dead or missed functions): "
                  f"{len(prologue_gaps)}, {sum(n for _, n in prologue_gaps)} bytes"]
    gap_lines += [f"  - {a:#x} +{n:#x} (in/after {ida_funcs.get(order[max(0, bisect.bisect_right(order, a) - 1)], '?')[:70]})"
                  for a, n in big]

    src_c = Counter(src.get(a, "ida") for a in order)
    mode_c = Counter(mode[a] for a in order)
    lines = [
        "# M0 analysis report — libcocos2dcpp.so v1.0.1", "",
        f"- functions: **{len(order)}** (IDA {len(ida_funcs)}; {len(missing_from_ida)} added from symbols/pointers, {pcrel_added} from PIC address computations)",
        f"- mode: {dict(mode_c)}; resolved by {dict(src_c)}",
        f"- instructions decoded: **{sum(total.values())}**, unique code bytes {sum(code_bytes.values())} of .text {text_hi - text_lo}; {shared} insn decodes shared between functions (IDA chunks/shared tails)",
        f"- PLT stubs: {len(plt)} ({sum(1 for p in plt.values() if p['target'])} internal, {len(real_imports)} real imports)",
        f"- relocations: { {k: v for k, v in reloc.items()} } (2=ABS32, 21=GLOB_DAT, 23=RELATIVE); code pointers in data: {len(fptrs)}",
        f"- init_array: {len(init_array)} ctors; JNI exports: {len(jni)}",
        f"- switch tables: {len(switches)} ({dict(Counter(s['kind'] for s in switches))})",
        f"- indirect calls (blx reg): {ind_calls}; unresolved indirect jumps: {len(ind_jumps)}",
        f"- noreturn: {len(noreturn)}; IDA frame fragments (far-branch targets split off as functions): {len(fragments)}",
        f"- functions with issues: {len(issue_funcs)}", "",
        "## Issue classes", "",
    ]
    lines += [f"- {n} x {k}" for k, n in issues.most_common()]
    lines += ["", "## Coverage gaps (largest 40)", ""] + gap_lines
    lines += ["", "## Callees treated as noreturn (call followed by non-code)", ""]
    lines += [f"- {n} x {k}" for k, n in noret.most_common(60)]
    for k, v in issue_samples.items():
        lines += ["", f"## Samples: {k}", ""] + [f"- {x}" for x in v]
    lines += ["", "## Instruction histogram", "", "| insn | count |", "|---|---|"]
    lines += [f"| {k} | {n} |" for k, n in total.most_common()]
    lines += ["", "## Real imports", "", ", ".join(real_imports)]
    lines += ["", "## First 200 functions with issues", ""]
    lines += [f"- {a:#x} {n} ({m}): {'; '.join(s)}" for a, n, m, s in issue_funcs[:200]]
    lines += ["", "## Unresolved indirect jumps (first 200)", ""]
    lines += [f"- in {a:#x} {ida_funcs.get(a, '?')}: at {j:#x} `{(lambda i: i.mnemonic + ' ' + i.op_str)(d.one(j, mode[a]))}`"
              for a, j in ind_jumps[:200]]
    (OUT / "report.md").write_text("\n".join(lines), encoding="utf-8")
    print("\n".join(lines[:14]))


if __name__ == "__main__":
    main()
