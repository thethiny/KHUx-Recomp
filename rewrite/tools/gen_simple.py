"""Generate faithful C++ rewrites of small, fixed-shape functions (forwarders, thunks, getters, setters,
constant returns, tail calls, std::function slots...) from the lifted C.

python recomp/rewrite/tools/gen_simple.py [--max-size N] [--stats]

The generator does not match text patterns: it parses every instruction's lifted C (the exact semantics) into a
small register-level IR and runs it symbolically - incoming registers, constants (literal pools read from the
.so when they live in read-only memory), memory loads, stores and calls as ordered statements, condition flags
of cmp/tst/movs for forward branches. A function is accepted only when every instruction is understood and the
result can be expressed with the rewrite runtime: it returns through the incoming lr with sp and r4-r11
restored, it never reads a register it does not know (a scratch register clobbered by a call, r12 at entry...),
and calls get exactly the argument registers the original passes. Anything else is skipped, never guessed.

Output: rewrite/src/gen/gen_<range>.cpp (regenerated from scratch each run; addresses registered with
RW_FUNCTION anywhere else in rewrite/src are skipped).

Calls keep their kind: a direct bl/blx calls the exact target the lifted C calls (rw::call / rw::call64), a call
through a vtable stays a virtual call (rw::vcall), a tail call (b / bx) runs with the caller's sp unchanged
(rw::call_exact, so stack arguments 5+ reach the callee as in the original). A function whose r0:r1 is the
callee's r0:r1 returns uint64_t so the verifier compares both words.
"""
import argparse
import json
import re
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

HERE = Path(__file__).resolve().parent
REWRITE = HERE.parent
RECOMP = REWRITE.parent
sys.path.insert(0, str(RECOMP))
import paths  # noqa: E402

OUT_DIR = REWRITE / "src" / "gen"
LIFT = RECOMP / "out" / "lift_all"
INDEX = RECOMP / "out" / "rewrite" / "index.json"
DECOMP = paths.decompile_index().parent / "decompile"
NAMES = paths.decompile_index().parent / "names.txt"
SO = paths.input_dir() / "libcocos2dcpp.so"
M32 = 0xFFFFFFFF
PLT = (0x40DCC4, 0x44EFD0)          # .plt: stubs of real imports (libc, GL, ...), run by the host
# imports whose effects stay in guest memory or are deterministic / replayed by the verifier (clocks frozen,
# random numbers replayed): a function calling one of them can be verified. Any other import (GL, files, logging,
# locks, JNI...) makes the function RW_FUNCTION_NOVERIFY.
PURE_IMPORTS = set("""strlen free memcpy memset memcmp memmove memchr malloc calloc realloc strcmp strncmp strchr strrchr
strspn strcspn strstr strcpy strncpy strcat strncat strcasecmp strncasecmp strtok_r strdup atoi atol atof strtol strtoul
strtoll strtoull strtod strtof sscanf snprintf sprintf vsnprintf toupper tolower isspace isdigit isalpha isalnum
__errno qsort bsearch ceilf floorf roundf sinf cosf tanf sqrtf powf expf exp2f logf log10f atan2f atanf acosf asinf fabsf
fmodf ceil floor round sin cos tan sqrt pow exp log log10 atan2 atan acos asin fabs fmod lround lroundf
lrand48 rand random srand48 time gettimeofday clock_gettime
__aeabi_memcpy __aeabi_memcpy4 __aeabi_memcpy8 __aeabi_memset __aeabi_memset4 __aeabi_memset8 __aeabi_memclr
__aeabi_memclr4 __aeabi_memclr8 __aeabi_memmove __aeabi_memmove4 __aeabi_memmove8""".split())


class Reject(Exception):
    pass


# ------------------------------------------------------------------------------------------------ the binary
class Image:
    """Read-only view of the .so as loaded (base 0): which words are constants (code, rodata, RELRO without an
    import relocation) and their values."""

    def __init__(self, path):
        self.f = open(path, "rb")
        self.elf = ELFFile(self.f)
        self.loads = [(s["p_vaddr"], s["p_memsz"], s["p_offset"], s["p_filesz"], s["p_flags"])
                      for s in self.elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        self.relro = [(s["p_vaddr"], s["p_memsz"]) for s in self.elf.iter_segments()
                      if s["p_type"] == "PT_GNU_RELRO"]
        self.reloc = {}          # address -> (type, symbol name) for relocations the loader resolves
        symtab = self.elf.get_section_by_name(".dynsym")
        for sec in self.elf.iter_sections():
            if isinstance(sec, RelocationSection):
                for r in sec.iter_relocations():
                    t = r["r_info_type"]
                    if t == 23:                                   # R_ARM_RELATIVE: base 0, file value stands
                        continue
                    sym = symtab.get_symbol(r["r_info_sym"]).name if r["r_info_sym"] else ""
                    self.reloc[r["r_offset"]] = (t, sym)

    def const_word(self, addr):
        """The 32-bit word at addr if it can never change at run time, else None."""
        for k in range(4):
            if addr + k in self.reloc:
                return None
        for va, msz, off, fsz, flags in self.loads:
            if va <= addr and addr + 4 <= va + fsz:
                ro = not (flags & 2) or any(rv <= addr and addr + 4 <= rv + rs for rv, rs in self.relro)
                if not ro:
                    return None
                self.f.seek(off + addr - va)
                return int.from_bytes(self.f.read(4), "little")
        return None

    def const_bytes(self, addr, n, signed):
        """Narrow read-only load (1/2 bytes), or None."""
        w = self.const_word(addr & ~3)
        if w is None or (addr & 3) + n > 4:
            return None
        v = (w >> (8 * (addr & 3))) & ((1 << (8 * n)) - 1)
        if signed and v >> (8 * n - 1):
            v = (v - (1 << (8 * n))) & M32
        return v


# ------------------------------------------------------------------------------------------------ names
def split_top(s, sep):
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch in "(<[":
            depth += 1
        elif ch in ")>]":
            depth -= 1
        if ch == sep and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    out.append(cur)
    return out


def parse_proto(proto):
    """Hex-Rays prototype -> (qualified name, [param types] or None, return type text)."""
    p = proto.strip()
    if not p.endswith(")"):
        return None, None, ""
    depth = 0
    for i in range(len(p) - 1, -1, -1):
        if p[i] == ")":
            depth += 1
        elif p[i] == "(":
            depth -= 1
            if depth == 0:
                break
    else:
        return None, None, ""
    head, params = p[:i], p[i + 1:-1]
    # the name is the last top-level token of head (templates may contain spaces)
    depth, j = 0, len(head)
    while j > 0:
        ch = head[j - 1]
        if ch in ">)":
            depth += 1
        elif ch in "<(":
            depth -= 1
        elif ch == " " and depth == 0:
            break
        j -= 1
    if "`" in head:                                # `non-virtual thunk to'X::f
        j = head.index("`")
    name, ret = head[j:], head[:j].strip()
    if name.startswith("`") and "'" in name:
        name = name[name.index("'") + 1:]
    while name[:1] in "*&":
        ret += name[0]
        name = name[1:]
    ret = ret.replace("__fastcall", "").replace("__cdecl", "").strip()
    if "@<" in p or "__usercall" in p or "__spoils" in p:
        return name, None, ret
    ps = [x.strip() for x in split_top(params, ",")] if params.strip() not in ("", "void") else []
    return name, ps, ret


WORD1 = {"int", "unsigned int", "_DWORD", "char", "_BYTE", "bool", "__int16", "_WORD", "unsigned __int8",
         "float", "signed int", "unsigned __int16", "__int8", "_BOOL4", "_BOOL1", "_BOOL2", "unsigned __int32",
         "__int32", "short", "unsigned short", "unsigned char", "signed char", "long", "unsigned long", "size_t",
         "wchar_t", "char16_t", "char32_t", "signed __int16", "signed __int8", "unsigned int8_t", "int32_t",
         "uint32_t", "uint8_t", "int8_t", "uint16_t", "int16_t", "_UNKNOWN"}
WORD2 = {"__int64", "unsigned __int64", "double", "_QWORD", "long long", "unsigned long long", "signed __int64"}
# functions that return in r0:r1 although IDA types them 32-bit (a forwarder typed uint32_t loses r1: 2026-10-17,
# frexp's forwarder broke printf's %f in a shadow run): frexp, sub_CD57CC (double -> double), and the libm imports
# llrint (long long), nearbyint, tgamma, exp2 (double)
WIDE_RETURN = {0xCA4984, 0xCD57CC, 0x44D548, 0x44D56C, 0x44D5A8, 0x44D5B4}


def param_type(p):
    p = re.sub(r"\s+[A-Za-z_]\w*$", "", p.strip()) if not p.strip().endswith(("*", "&")) else p.strip()
    return p.replace("const ", "").replace("volatile ", "").strip()


def proto_words(params):
    """Argument words a parameter list occupies (AAPCS), or None when unknown (struct by value, varargs...)."""
    if params is None:
        return None
    k = 0
    for p in params:
        if p == "..." or not p:
            return None
        t = param_type(p)
        if "*" in t or "&" in t or "(" in t:
            k += 1
        elif t in WORD1:
            k += 1
        elif t in WORD2:
            k += (k & 1) + 2
        else:
            return None
    return k


class Names:
    def __init__(self, idx):
        self.idx = idx
        self.data = {}
        for line in NAMES.read_text(encoding="utf-8", errors="replace").splitlines():
            p = line.split()
            if len(p) >= 2 and p[0].startswith("0x"):
                self.data.setdefault(int(p[0], 16), p[1])
        self.files = {}
        self.cache = {}
        self.plt = {a: n for a, n in self.data.items() if PLT[0] <= a < PLT[1]}

    def is_function(self, t):
        """A call / tail target: a function of the library, or an import stub."""
        return f"{t & ~1:08X}" in self.idx or t in self.plt

    def lines(self, f):
        if f not in self.files:
            self.files[f] = (DECOMP / f).read_text(encoding="utf-8", errors="replace").split("\n")
        return self.files[f]

    def proto(self, a):
        """(qualified name, params or None, return type, raw prototype) of the function at a."""
        if a in self.cache:
            return self.cache[a]
        e = self.idx.get(f"{a:08X}")
        res = (None, None, "", "")
        if e and e["decomp"]:
            L = self.lines(e["decomp"][0])
            k = e["decomp"][1]
            raw = ""
            while k < len(L) and not L[k].startswith("{") and len(raw) < 4000:
                raw += L[k].strip() + " "
                k += 1
            raw = re.sub(r"\(\s+", "(", raw.strip())
            name, params, ret = parse_proto(raw)
            res = (name, params, ret, raw)
        if a in WIDE_RETURN:
            res = (res[0], res[1], "double", res[3])
        self.cache[a] = res
        return res

    def mangled(self, a):
        e = self.idx.get(f"{a:08X}")
        return e["name"] if e else self.data.get(a, "")

    def pretty(self, a):
        """Readable (demangled) name of the function at a."""
        if a in self.plt:
            return f"{self.plt[a]} (import)"
        name = self.proto(a)[0]
        m = self.mangled(a)
        if not name or name.startswith("sub_") or not m:
            return m or f"sub_{a:X}"
        kind = special_kind(m)
        if m.startswith("_ZTh") or m.startswith("_ZTv"):
            name = "thunk to " + name
        return name + (f" [{kind}]" if kind else "")

    def symbol(self, v):
        """Name for a constant that is an address (function, typeinfo, vtable...), or ''."""
        for a in (v, v & ~1):
            if f"{a:08X}" in self.idx and (a == v or v & 1):
                return self.pretty(a)
        n = self.data.get(v)
        if n and not n.startswith(("unk_", "byte_", "word_", "dword_", "off_", "loc_", "locret_", "stru_")):
            return n
        return ""


def special_kind(m):
    m2 = re.search(r"(C1|C2|C3|D0|D1|D2)E[vP]", m) or re.search(r"(C1|C2|C3|D0|D1|D2)E.*$", m)
    if not m.startswith("_Z") or not m2:
        return ""
    return {"D0": "deleting destructor", "D1": "complete destructor", "D2": "base destructor",
            "C1": "complete constructor", "C2": "base constructor", "C3": "allocating constructor"}[m2.group(1)]


def strip_templates(s):
    out, depth = "", 0
    for ch in s:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out += ch
    return out


OPS = {"()": "call", "==": "eq", "!=": "ne", "=": "assign", "<": "lt", ">": "gt", "<=": "le", ">=": "ge",
       "+": "add", "-": "sub", "*": "mul", "/": "div", "[]": "index", "->": "arrow", "!": "not", "<<": "shl",
       ">>": "shr", "+=": "add_assign", "-=": "sub_assign", "&&": "and", "||": "or", "new": "new",
       "delete": "delete", "new[]": "new_array", "delete[]": "delete_array", "bool": "bool", "&": "bitand",
       "|": "bitor", "^": "xor", "~": "compl", "%": "mod", "++": "inc", "--": "dec"}


def ident(name, kind=""):
    """C++ identifier from a demangled qualified name."""
    s = name
    s = re.sub(r"operator\s*(\(\)|==|!=|<=|>=|<<|>>|\+=|-=|&&|\|\||->|\[\]|new\[\]|delete\[\]|new|delete|[=<>+\-*/!&|^~%]|\+\+|--)",
               lambda m: "op_" + OPS.get(m.group(1), "x"), s)
    s = strip_templates(s)
    s = s.replace("{lambda", "lambda").replace("(anonymous namespace)", "anon")
    s = re.sub(r"~(\w+)", r"\1_dtor", s)
    s = s.replace("std::__1::", "std::").replace("std::__function::", "")
    parts = [p for p in re.split(r"::|[^A-Za-z0-9_]+", s) if p]
    if len(parts) > 4:
        parts = parts[-4:]
    s = "_".join(parts)
    if kind == "deleting destructor":
        s += "_deleting"
    s = re.sub(r"_+", "_", s).strip("_")
    return s[:80] or "fn"


# ------------------------------------------------------------------------------------------------ IR
# expressions: ('in', r) incoming register r (13 sp excluded) / stack argument (r >= 16: word r-16 above sp)
#              ('const', v) / ('temp', id) / ('bin', op, a, b) / ('not', a) / ('clob',) unknown value
def C(v):
    return ("const", v & M32)


def is_const(e):
    return e[0] == "const"


def add(a, b):
    if is_const(a) and is_const(b):
        return C(a[1] + b[1])
    if is_const(a):
        a, b = b, a
    if is_const(b) and b[1] == 0:
        return a
    if is_const(b) and a[0] == "bin" and a[1] == "+" and is_const(a[3]):
        return add(a[2], C(a[3][1] + b[1]))
    return ("bin", "+", a, b)


def sub(a, b):
    if is_const(b):
        return add(a, C(-b[1]))
    if is_const(a) and is_const(b):
        return C(a[1] - b[1])
    return ("bin", "-", a, b)


def binop(op, a, b):
    if a[0] == "clob" or b[0] == "clob":
        return ("clob",)
    if op == "+":
        return add(a, b)
    if op == "-":
        return sub(a, b)
    if is_const(a) and is_const(b):
        x, y = a[1], b[1]
        r = {"&": x & y, "|": x | y, "^": x ^ y, "*": x * y, "<<": (x << y) if y < 32 else 0,
             ">>": (x >> y) if y < 32 else 0,
             "asr": ((x - (1 << 32) if x >> 31 else x) >> min(y, 31))}[op]
        return C(r)
    if op in ("<<", ">>", "asr") and is_const(b) and b[1] == 0:
        return a
    return ("bin", op, a, b)


def uses(e, acc):
    if e[0] == "temp":
        acc.append(e[1])
    elif e[0] == "bin":
        uses(e[2], acc)
        uses(e[3], acc)
    elif e[0] == "not":
        uses(e[1], acc)
    return acc


def has(e, pred):
    if pred(e):
        return True
    if e[0] == "bin":
        return has(e[2], pred) or has(e[3], pred)
    if e[0] == "not":
        return has(e[1], pred)
    return False


# ------------------------------------------------------------------------------------------------ lifted C parser
RG = r"c->r\[(\d+)\]"
K = r"0x([0-9A-F]+)u"
OPND = rf"(?:{RG}|{K})"
LINE = re.compile(r"^L_([0-9A-F]+): /\* (.*?) \*/ c->lpc = 0x[0-9A-F]+u; c->icount\+\+; (.*)$")
P_LOAD = re.compile(rf"^\{{ uint32_t dc_ = 0; \(void\)dc_; uint32_t a_ = {OPND} \+ (-?)\(uint32_t\)(?:(\d+)|{RG}); "
                    rf"uint32_t v_ = (R32|R16|R8|RS16|RS8)\(a_\); {RG} = v_; \}}$")
P_STORE = re.compile(rf"^\{{ uint32_t dc_ = 0; \(void\)dc_; uint32_t a_ = {RG} \+ (-?)\(uint32_t\)(?:(\d+)|{RG}); "
                     rf"W(32|16|8)\(c, a_, {RG}\);  \}}$")
P_MOV = re.compile(rf"^\{{ uint32_t r_ = (NZ\(c, )?(~)?{OPND}\)?; {RG} = r_; \}}$")
P_ARITH_F = re.compile(rf"^\{{ uint32_t r_ = (ADD|SUB)_F\({OPND}, {OPND}\); (?:{RG} = r_;|\(void\)r_;) \}}$")
P_ARITH = re.compile(rf"^\{{ uint32_t r_ = (NZ\(c, )?\({OPND} ([-+&|^]) (~?){OPND}\)\)?; (?:{RG} = r_;|\(void\)r_;) \}}$")
P_SHIFT = re.compile(rf"^\{{ uint32_t sc_ = c->c; uint32_t sv_ = SHIFT\({RG}, ([0-3]), (\d+), c->c, &sc_\); "
                     rf"(NZ\(c, sv_\); c->c = sc_; )?{RG} = sv_; \}}$")
P_MUL = re.compile(rf"^\{{ uint32_t mv_ = {RG} \* {RG}; (NZ\(c, mv_\); )?{RG} = mv_; \}}$")
P_PUSH = re.compile(r"^\{ uint32_t b_ = c->r\[13\]; uint32_t a_ = b_ - (\d+); ((?:W32\(c, a_ \+ \d+, c->r\[\d+\]\); )+)"
                    r"c->r\[13\] = b_ - (\d+); \}$")
P_POP = re.compile(r"^\{ uint32_t b_ = c->r\[13\]; uint32_t a_ = b_; ((?:uint32_t l\d+_ = R32\(a_ \+ \d+\); )+)"
                   r"c->r\[13\] = b_ \+ (\d+); ((?:c->r\[\d+\] = l\d+_; )*)(?:c->pc = \((l\d+_)\); return; )?\}$")
P_LDM = re.compile(r"^\{ uint32_t b_ = c->r\[(\d+)\]; uint32_t a_ = b_; ((?:uint32_t l\d+_ = R32\(a_ \+ \d+\); )+)"
                   r"(?:c->r\[(\d+)\] = b_ \+ (\d+); )?((?:c->r\[\d+\] = l\d+_; )+)\}$")
P_STM = re.compile(r"^\{ uint32_t b_ = c->r\[(\d+)\]; uint32_t a_ = b_; ((?:W32\(c, a_ \+ \d+, c->r\[\d+\]\); )+)"
                   r"(?:c->r\[(\d+)\] = b_ \+ (\d+); )?\}$")
P_CALL = re.compile(rf"^c->r\[14\] = {K}; rt_call\(c, {K}\); if \(c->pc != {K}\) return;$")
P_CALLR = re.compile(rf"^\{{ uint32_t t_ = {RG}; c->r\[14\] = {K}; rt_call\(c, t_\); if \(c->pc != {K}\) return; \}}$")
P_RET = re.compile(r"^c->pc = \(c->r\[14\]\); return;$")
P_TAIL = re.compile(rf"^TAIL\((?:{K}|\({RG}\))\);$")
P_GOTO = re.compile(r"^goto L_([0-9A-F]+);$")
P_IF = re.compile(r"^if \((.*?)\) \{ (.*) \}$")
CONDS = {"c->z": "eq", "!c->z": "ne", "c->c": "hs", "!c->c": "lo", "c->n": "mi", "!c->n": "pl",
         "c->v": "vs", "!c->v": "vc", "c->c && !c->z": "hi", "!c->c || c->z": "ls", "c->n == c->v": "ge",
         "c->n != c->v": "lt", "!c->z && c->n == c->v": "gt", "c->z || c->n != c->v": "le"}
NEG = {"eq": "ne", "ne": "eq", "hs": "lo", "lo": "hs", "mi": "pl", "pl": "mi", "hi": "ls", "ls": "hi",
       "ge": "lt", "lt": "ge", "gt": "le", "le": "gt", "vs": "vc", "vc": "vs"}


def opnd(reg, k):
    return ("r", int(reg)) if reg is not None else ("k", int(k, 16))


def parse_insn(code):
    """One instruction's lifted C -> IR op tuple, or raise Reject."""
    m = P_IF.match(code)
    if m and m.group(1) not in CONDS and m.group(1).startswith("(") and m.group(1)[1:-1] in CONDS:
        code = f"if ({m.group(1)[1:-1]}) {{ {m.group(2)} }}"
        m = P_IF.match(code)
    if m and m.group(1) in CONDS:
        inner = m.group(2)
        g = P_GOTO.match(inner)
        if g:
            return ("bcond", CONDS[m.group(1)], int(g.group(1), 16))
        return ("cond", CONDS[m.group(1)], parse_insn(inner.strip()))
    m = P_LOAD.match(code)
    if m:
        breg, bk, neg, off, oreg, kind, dst = m.groups()
        return ("ld", kind, opnd(breg, bk), neg == "-", ("k", int(off)) if off is not None else ("r", int(oreg)),
                int(dst))
    m = P_STORE.match(code)
    if m:
        breg, neg, off, oreg, bits, src = m.groups()
        return ("st", int(bits), ("r", int(breg)), neg == "-",
                ("k", int(off)) if off is not None else ("r", int(oreg)), int(src))
    m = P_MOV.match(code)
    if m:
        nz, inv, reg, k, dst = m.groups()
        return ("mov", bool(nz), bool(inv), opnd(reg, k), int(dst))
    m = P_ARITH_F.match(code)
    if m:
        op, r1, k1, r2, k2, dst = m.groups()
        return ("arithf", "+" if op == "ADD" else "-", opnd(r1, k1), opnd(r2, k2), int(dst) if dst else None)
    m = P_ARITH.match(code)
    if m:
        nz, r1, k1, op, inv, r2, k2, dst = m.groups()
        return ("arith", bool(nz), op, opnd(r1, k1), bool(inv), opnd(r2, k2), int(dst) if dst else None)
    m = P_SHIFT.match(code)
    if m:
        src, typ, amt, nz, dst = m.groups()
        return ("shift", int(typ), int(amt), int(src), bool(nz), int(dst))
    m = P_MUL.match(code)
    if m:
        a, b, nz, dst = m.groups()
        return ("mul", int(a), int(b), bool(nz), int(dst))
    m = P_PUSH.match(code)
    if m:
        n, ws, n2 = m.groups()
        if n != n2:
            raise Reject("push form")
        stores = [(int(o), int(r)) for o, r in re.findall(r"W32\(c, a_ \+ (\d+), c->r\[(\d+)\]\)", ws)]
        return ("push", int(n), stores)
    m = P_POP.match(code)
    if m:
        lds, n, assigns, pcl = m.groups()
        loads = {l: int(o) for l, o in re.findall(r"uint32_t (l\d+_) = R32\(a_ \+ (\d+)\)", lds)}
        regs = [(int(r), loads[l]) for r, l in re.findall(r"c->r\[(\d+)\] = (l\d+_)", assigns)]
        return ("pop", int(n), regs, loads[pcl] if pcl else None)
    m = P_LDM.match(code)
    if m:
        base, lds, wb, n, assigns = m.groups()
        if wb is not None and wb != base:
            raise Reject("ldm form")
        loads = {l: int(o) for l, o in re.findall(r"uint32_t (l\d+_) = R32\(a_ \+ (\d+)\)", lds)}
        regs = [(int(r), loads[l]) for r, l in re.findall(r"c->r\[(\d+)\] = (l\d+_)", assigns)]
        return ("ldm", int(base), regs, int(n) if wb is not None else None)
    m = P_STM.match(code)
    if m:
        base, sts, wb, n = m.groups()
        if wb is not None and wb != base:
            raise Reject("stm form")
        stores = [(int(o), int(r)) for o, r in re.findall(r"W32\(c, a_ \+ (\d+), c->r\[(\d+)\]\)", sts)]
        return ("stm", int(base), stores, int(n) if wb is not None else None)
    m = P_CALL.match(code)
    if m:
        lr, tgt, chk = m.groups()
        if lr != chk:
            raise Reject("call form")
        return ("call", ("k", int(tgt, 16)))
    m = P_CALLR.match(code)
    if m:
        reg, lr, chk = m.groups()
        if lr != chk:
            raise Reject("call form")
        return ("call", ("r", int(reg)))
    if P_RET.match(code):
        return ("ret",)
    m = P_TAIL.match(code)
    if m:
        k, reg = m.groups()
        return ("tail", opnd(reg, k))
    m = P_GOTO.match(code)
    if m:
        return ("goto", int(m.group(1), 16))
    raise Reject("insn: " + code[:80])


def lifted_insns(lines, start):
    out = []
    i = start
    if not lines[i - 1].startswith("void F_"):
        raise Reject("lift index")
    while i < len(lines) and not lines[i].startswith("}"):
        m = LINE.match(lines[i])
        if m:
            out.append((int(m.group(1), 16), m.group(2), m.group(3)))
        elif lines[i].strip() and not lines[i].strip().startswith("goto L_"):
            raise Reject("line: " + lines[i][:60])
        i += 1
    return out


# ------------------------------------------------------------------------------------------------ symbolic run
LOADTY = {"R32": ("uint32_t", 4, False), "R16": ("uint16_t", 2, False), "R8": ("uint8_t", 1, False),
          "RS16": ("int16_t", 2, True), "RS8": ("int8_t", 1, True)}
STORETY = {32: "uint32_t", 16: "uint16_t", 8: "uint8_t"}
MAX_PATHS = 12


class Fn:
    def __init__(self, addr, insns, img, names):
        self.addr = addr
        self.insns = insns
        self.at = {a: k for k, (a, _, _) in enumerate(insns)}
        self.ops = [parse_insn(c) for _, _, c in insns]
        self.img = img
        self.names = names
        self.temps = {}          # id -> statement
        self.ntemp = 0
        self.paths = 0

    def new_temp(self, st):
        t = self.ntemp
        self.ntemp += 1
        self.temps[t] = st
        return ("temp", t)

    # state: regs (list of 16 expr; 13 = ('sp', delta); 14 = lr), stack {delta: expr}, flags, stmts
    def run(self):
        regs = [("in", k) if k < 4 else ("cs", k) for k in range(16)]     # cs: r4-r12 at entry (not usable)
        regs[13] = ("sp", 0)
        regs[14] = ("lr",)
        return self.walk(0, regs, {}, None, [])

    def val(self, regs, o):
        if o[0] == "k":
            return C(o[1])
        if o[1] == 15:
            raise Reject("pc operand")
        return regs[o[1]]

    def load(self, stmts, kind, addr, regs):
        ty, n, signed = LOADTY[kind]
        if addr[0] == "sp":                              # stack: a frame slot, or an incoming stack argument
            d = addr[1]
            if kind != "R32" or d % 4:
                raise Reject("stack read")
            if d in self.stack:
                return self.stack[d]
            if d >= 0:
                return ("in", 4 + d // 4)
            raise Reject("stack read")
        if addr[0] == "clob" or has(addr, lambda e: e[0] in ("clob", "sp", "lr")):
            raise Reject("unknown address")
        if is_const(addr):
            v = self.img.const_word(addr[1]) if n == 4 else self.img.const_bytes(addr[1], n, signed)
            if v is not None:
                return ("const", v, ("ro", addr[1]))
        t = self.new_temp(("load", ty, addr, signed))
        stmts.append(t[1])
        return t

    def walk(self, k, regs, stack, flags, stmts):
        """Run from instruction k; returns a tree node {stmts, term}."""
        self.paths += 1
        if self.paths > MAX_PATHS:
            raise Reject("too many paths")
        regs = list(regs)
        stack = dict(stack)
        stmts = list(stmts)
        steps = 0
        while True:
            steps += 1
            if k >= len(self.ops) or steps > 64:
                raise Reject("falls off")
            op = self.ops[k]
            self.stack = stack
            if op[0] == "bcond":
                tgt = self.at.get(op[2])
                if tgt is None or tgt <= k:
                    raise Reject("branch target")
                cond = self.cond(op[1], flags)
                t = self.walk(tgt, regs, stack, flags, [])
                f = self.walk(k + 1, regs, stack, flags, [])
                return {"stmts": stmts, "term": ("if", cond, t, f)}
            if op[0] == "cond":
                inner = op[2]
                if inner[0] not in ("mov", "ld", "arith", "ret") or (inner[0] in ("mov", "arith") and inner[1]):
                    raise Reject("cond insn")
                cond = self.cond(op[1], flags)
                r2, s2, st2 = list(regs), dict(stack), []
                _, done = self.step(inner, r2, s2, flags, st2)
                if done:
                    done["stmts"] = st2 + done["stmts"]
                    t = done
                else:
                    t = self.walk(k + 1, r2, s2, flags, st2)
                f = self.walk(k + 1, regs, stack, flags, [])
                return {"stmts": stmts, "term": ("if", cond, t, f)}
            if op[0] == "goto":
                tgt = self.at.get(op[1])
                if tgt is None or tgt <= k:
                    raise Reject("goto target")
                k = tgt
                continue
            flags, done = self.step(op, regs, stack, flags, stmts)
            if done:
                done["stmts"] = stmts + done["stmts"]
                return done
            k += 1

    def cond(self, cc, flags):
        if flags is None:
            raise Reject("flags unknown")
        kind = flags[0]
        if kind == "sub":
            a, b = flags[1], flags[2]
            m = {"eq": "==", "ne": "!=", "hs": ">=u", "lo": "<u", "hi": ">u", "ls": "<=u", "ge": ">=s", "lt": "<s",
                 "gt": ">s", "le": "<=s"}
            if cc in m:
                return ("cmp", m[cc], a, b)
            if cc in ("mi", "pl"):
                return ("cmp", "<s" if cc == "mi" else ">=s", sub(a, b), C(0))
        elif kind in ("nz", "add"):
            r = flags[1] if kind == "nz" else add(flags[1], flags[2])
            if cc in ("eq", "ne"):
                return ("cmp", "==" if cc == "eq" else "!=", r, C(0))
            if cc in ("mi", "pl"):
                return ("cmp", "<s" if cc == "mi" else ">=s", r, C(0))
        raise Reject("condition " + cc)

    def need(self, e):
        if e[0] == "clob" or has(e, lambda x: x[0] == "clob"):
            raise Reject("clobbered value")
        return e

    def step(self, op, regs, stack, flags, stmts):
        """Apply one op; returns (flags, terminal node or None)."""
        kind = op[0]
        if kind == "ld":
            _, lk, base, neg, off, dst = op
            if base == ("r", 15) or dst in (13, 15):
                raise Reject("pc/sp load")
            b = self.val(regs, base)
            o = self.val(regs, off)
            if b[0] == "sp":
                if o[0] != "const" or neg:
                    raise Reject("sp index")
                v = self.load(stmts, lk, ("sp", b[1] + o[1]), regs)
            else:
                a = sub(b, o) if neg else add(self.need(b), self.need(o))
                v = self.load(stmts, lk, a, regs)
            regs[dst] = v
            return flags, None
        if kind == "st":
            _, bits, base, neg, off, src = op
            b = self.val(regs, base)
            o = self.val(regs, off)
            if src in (13, 15) or base[1] == 15:
                raise Reject("stores sp/pc")
            v = self.need(regs[src])
            if b[0] == "sp":                               # a slot of the own frame (outgoing arguments...)
                if o[0] != "const" or neg or bits != 32 or (b[1] + o[1]) % 4 or b[1] + o[1] >= 0:
                    raise Reject("stack store")
                stack[b[1] + o[1]] = v
                stack[("st", b[1] + o[1])] = True
                return flags, None
            a = sub(b, o) if neg else add(self.need(b), self.need(o))
            if v[0] in ("sp", "lr") or has(a, lambda e: e[0] in ("sp", "lr")) or has(v, lambda e: e[0] in ("sp", "lr")):
                raise Reject("stores sp/lr")
            stmts.append(("store", STORETY[bits], a, v))
            return flags, None
        if kind == "ldm":
            _, base, rl, wb = op
            b = regs[base]
            if base in (13, 15) or b[0] == "sp" or any(r in (13, 15) for r, _ in rl):
                raise Reject("ldm sp/pc")
            b = self.need(b)
            vals = [(r, self.load(stmts, "R32", add(b, C(o)), regs)) for r, o in rl]
            if wb is not None:
                regs[base] = add(b, C(wb))
            for r, v in vals:
                regs[r] = v
            return flags, None
        if kind == "stm":
            _, base, sl, wb = op
            b = regs[base]
            if base in (13, 15) or b[0] == "sp" or any(r in (13, 14, 15) for _, r in sl):
                raise Reject("stm sp/pc")
            b = self.need(b)
            for o, r in sl:
                v = self.need(regs[r])
                if has(v, lambda e: e[0] in ("sp", "lr")):
                    raise Reject("stores sp/lr")
                stmts.append(("store", "uint32_t", add(b, C(o)), v))
            if wb is not None:
                regs[base] = add(b, C(wb))
            return flags, None
        if kind == "mov":
            _, nz, inv, src, dst = op
            if dst == 15 or src == ("r", 15):
                raise Reject("mov pc")
            v = self.val(regs, src)
            if v[0] == "sp" or dst == 13:                  # mov rX, sp / mov sp, rX (frame pointer)
                if inv or nz or v[0] != "sp":
                    raise Reject("mov sp")
            elif inv:
                v = ("not", self.need(v)) if not is_const(v) else C(~v[1])
            regs[dst] = v
            return (("nz", v) if nz else flags), None
        if kind == "arithf":
            _, o, a, b, dst = op
            if dst in (13, 15) or ("r", 13) in (a, b) or ("r", 15) in (a, b):
                raise Reject("sp arith")
            x, y = self.need(self.val(regs, a)), self.need(self.val(regs, b))
            if x[0] == "sp" or y[0] == "sp":
                raise Reject("sp arith")
            if dst is not None:
                regs[dst] = binop(o, x, y)
            return (("sub", x, y) if o == "-" else ("add", x, y)), None
        if kind == "arith":
            _, nz, o, a, inv, b, dst = op
            if dst == 15 or ("r", 15) in (a, b):
                raise Reject("pc arith")
            x, y = self.val(regs, a), self.val(regs, b)
            if x[0] == "sp" or y[0] == "sp" or dst == 13:
                # add/sub with sp: frame adjust (sub sp, #n), frame pointer (add r7, sp, #8)
                if x[0] == "sp" and is_const(y) and not nz and not inv and o in "+-" and y[1] < 0x10000:
                    regs[dst] = ("sp", x[1] + (y[1] if o == "+" else -y[1]))
                    return flags, None
                raise Reject("sp arith")
            x, y = self.need(x), self.need(y)
            if inv:
                y = C(~y[1]) if is_const(y) else ("not", y)
            r = binop(o, x, y)
            if dst is not None:
                regs[dst] = r
            return (("nz", r) if nz else flags), None
        if kind == "shift":
            _, typ, amt, src, nz, dst = op
            if typ == 3 or src in (13, 15) or dst in (13, 15):
                raise Reject("shift")
            x = self.need(regs[src])
            if amt == 0 and typ != 0:
                raise Reject("shift #0")
            if typ != 2 and amt >= 32:                  # lsl/lsr #32: zero (a C++ shift by 32 is undefined)
                r = C(0)
            else:
                r = binop({0: "<<", 1: ">>", 2: "asr"}[typ], x, C(min(amt, 31)))
            regs[dst] = r
            return (("nz", r) if nz else flags), None
        if kind == "mul":
            _, a, b, nz, dst = op
            r = binop("*", self.need(regs[a]), self.need(regs[b]))
            regs[dst] = r
            return (("nz", r) if nz else flags), None
        if kind == "push":
            _, n, stores = op
            sp = regs[13]
            for o, r in stores:
                stack[(sp[1] - n + o)] = regs[r]
            regs[13] = ("sp", sp[1] - n)
            return flags, None
        if kind == "pop":
            _, n, rl, pco = op
            sp = regs[13][1]
            vals = {}
            for r, o in rl:
                if sp + o not in stack:
                    raise Reject("pop unknown slot")
                vals[r] = stack[sp + o]
            for r, v in vals.items():
                if r == 13:
                    raise Reject("pop sp")
                regs[r] = v
            regs[13] = ("sp", sp + n)
            if pco is not None:
                if sp + pco not in stack:
                    raise Reject("pop pc slot")
                return flags, self.leave(regs, stack.get(sp + pco), stmts)
            return flags, None
        if kind == "call":
            tgt = op[1]
            if tgt[0] == "k":
                t = C(tgt[1])
                if not self.names.is_function(tgt[1]):
                    raise Reject("call target not a function")
            else:
                t = self.need(regs[tgt[1]])
            args = self.call_args(t, regs, stack)
            res = self.new_temp(("call", t, args, regs[13][1]))
            stmts.append(res[1])
            for k in range(4, len(args)):                  # the callee owns its stack arguments
                stack[regs[13][1] + 4 * (k - 4)] = ("clob",)
            regs[0] = ("lo", res[1])
            regs[1] = ("hi", res[1])
            for r in (2, 3, 12, 14):
                regs[r] = ("clob",)
            return None, None
        if kind == "ret":
            return flags, self.leave(regs, regs[14], stmts)
        if kind == "tail":
            tgt = op[1]
            if tgt[0] == "k":
                t = C(tgt[1])
                if not self.names.is_function(tgt[1]):
                    raise Reject("tail target not a function")
            else:
                t = self.need(regs[tgt[1]])
            self.check_exit(regs, regs[14])
            args = self.call_args(t, regs, None)
            res = self.new_temp(("call", t, args, "tail"))
            return flags, {"stmts": [res[1]], "term": ("ret", ("lo", res[1]), ("hi", res[1]))}
        raise Reject("op " + kind)

    def check_exit(self, regs, pc):
        if pc != ("lr",):
            raise Reject("returns elsewhere")
        if regs[13] != ("sp", 0):
            raise Reject("sp not restored")
        for r in range(4, 12):
            if regs[r] != ("cs", r):
                raise Reject(f"r{r} not restored")

    def leave(self, regs, pc, stmts):
        self.check_exit(regs, pc)
        return {"stmts": [], "term": ("ret", regs[0], regs[1])}

    def call_args(self, target, regs, stack):
        """Arguments the call passes: r0..r(n-1), then stack words (stack None: a tail call, whose stack
        arguments are the caller's and reach the callee unchanged)."""
        tw = trusted = None
        named = False
        if is_const(target):
            ta = target[1] & ~1
            if target[1] in self.names.plt:
                proto = (None, None, "", "")
                trusted = 1 if self.names.plt[target[1]] == "free" else None
            else:
                proto = self.names.proto(ta)
                trusted = trusted_args(self.names.mangled(ta), proto)
            tw = proto_words(proto[1])
            named = bool(proto[0]) and not proto[0].startswith(("sub_", "nullsub_", "j_"))

        def bad(v):
            return has(v, lambda e: e[0] in ("clob", "sp", "lr", "cs"))
        args = []
        for r in range(4):
            v = regs[r]
            if bad(v):
                # a register the callee does not take (a scratch value the original leaves there): the callee
                # is a destructor / operator delete, or a named function whose prototype has fewer words
                if trusted is not None and r >= trusted:
                    break
                if named and tw is not None and r >= tw:
                    break
                raise Reject(f"call arg r{r} unknown")
            args.append(v)
        if stack is not None:
            sp = regs[13][1]
            stored = [d for d in stack if isinstance(d, tuple) and sp <= d[1] < sp + 64]
            if tw is not None and tw > 4:
                if len(args) < 4:
                    raise Reject("stack args after unknown register")
                for k in range(tw - 4):
                    v = stack.get(sp + 4 * k)
                    if v is None or ("st", sp + 4 * k) not in stack or bad(v):
                        raise Reject("stack arg unknown")
                    args.append(v)
                return args
            if stored:
                raise Reject("stack args, callee prototype unknown")
        # drop trailing arguments the callee does not take (per its prototype) that are just the incoming
        # registers - they reach the callee unchanged either way (rw::call leaves other registers as they are)
        while args and tw is not None and len(args) > max(tw, 1) and args[-1] == ("in", len(args) - 1):
            args.pop()
        # a destructor / operator delete / free takes one register: what the others hold is scratch
        if trusted is not None:
            args = args[:trusted]
        # a previous call's r1 left in a register the (named) callee does not take: scratch as well
        while named and tw is not None and len(args) > max(tw, 1) and args[-1][0] == "hi":
            args.pop()
        return args


def trusted_args(mangled, proto):
    """Number of argument registers a callee certainly reads at most (for calls after a call, where r2/r3 hold
    the previous callee's leftovers), or None."""
    if mangled in ("_ZdlPv", "_ZdaPv", "free", "_ZdlPvRKSt9nothrow_t"):
        return 1
    if re.search(r"D[01]Ev$", mangled):
        return 1
    if re.search(r"D2Ev$", mangled) and proto[1] is not None and proto_words(proto[1]) == 1:
        return 1
    return None


# ------------------------------------------------------------------------------------------------ emission
def is_side(st):
    return st[0] in ("store", "call")


def render_const(v, hint=None):
    if v < 10:
        return f"{v}u"
    if v >= 0x80000000 and (v - (1 << 32)) > -0x10000:
        return f"0x{v:X}u"
    return f"0x{v:X}u"


class Writer:
    """Renders one function's tree."""

    def __init__(self, fn, names, pnames, ret_kind):
        self.fn = fn
        self.names = names
        self.pnames = pnames
        self.ret_kind = ret_kind
        self.lines = []
        self.uses = {}
        self.vnames = {}
        self.inl = {}
        self.wide = set()
        self.notes = []

    def note(self, line):
        """line + the notes gathered while rendering it."""
        out = line
        uniq = []
        for n in self.notes:
            if n not in uniq:
                uniq.append(n)
        if uniq:
            out += "  // " + "; ".join(uniq)
        self.notes = []
        return out

    def ex(self, e, prec=0):
        k = e[0]
        if k == "in":
            return self.pnames[e[1]]
        if k == "const":
            n = self.names.symbol(e[1]) if e[1] >= 0x1000 else ""
            if len(e) > 2 and n:
                self.notes.append(f"0x{e[1]:X} = {n} (read from 0x{e[2][1]:X})")
            elif n:
                self.notes.append(f"0x{e[1]:X} = {n}")
            return render_const(e[1])
        if k == "lo":
            if e[1] in self.inl:
                self.notes += self.inl[e[1]][1]
                return self.inl[e[1]][0]
            n = self.vnames[e[1]]
            return f"uint32_t({n})" if e[1] in self.wide else n
        if k == "hi":
            return f"uint32_t({self.vnames[e[1]]} >> 32)"
        if k == "temp":
            if e[1] in self.inl:
                self.notes += self.inl[e[1]][1]
                return self.inl[e[1]][0]
            return self.vnames[e[1]]
        if k == "not":
            return "~" + self.ex(e[1], 9)
        if k == "bin":
            op, a, b = e[1], e[2], e[3]
            if op == "+" and is_const(b) and b[1] >= 0x80000000:
                s = f"{self.ex(a, 5)} - {render_const((1 << 32) - b[1])}"
                p = 5
            elif op == "asr":
                s = f"uint32_t(int32_t({self.ex(a)}) >> {b[1] if is_const(b) else self.ex(b)})"
                p = 10
            else:
                p = {"*": 6, "+": 5, "-": 5, "<<": 4, ">>": 4, "&": 3, "^": 2, "|": 1}[op]
                lp = 6 if op in ("<<", ">>") else p            # (a + b) << n: parenthesised for clarity
                s = f"{self.ex(a, lp)} {op} {self.ex(b, p + 1)}"
            return f"({s})" if p < prec else s
        raise Reject("render " + k)


def cond_str(w, c):
    _, op, a, b = c
    if op.endswith("s"):
        return f"int32_t({w.ex(a)}) {op[:-1]} int32_t({w.ex(b)})"
    # operands bind tighter than any binary operator but * (relational/equality sit below shifts, above &^|)
    return f"{w.ex(a, 6)} {op.rstrip('u')} {w.ex(b, 6)}"


# ------------------------------------------------------------------------------------------------ driver
def registered_elsewhere():
    pat = re.compile(r"^\s*RW_FUNCTION(?:_NOVERIFY)?\(\s*0x([0-9A-Fa-f]+)", re.M)
    out = set()
    for f in (REWRITE / "src").rglob("*.cpp"):
        if f.parent == OUT_DIR:
            continue
        out.update(int(a, 16) & ~1 for a in pat.findall(f.read_text(encoding="utf-8")))
    return out


def all_terms(node, acc):
    t = node["term"]
    if t[0] == "ret":
        acc.append((node, t))
    else:
        all_terms(t[2], acc), all_terms(t[3], acc)
    return acc


def all_exprs(node, fn, acc):
    for t in node["stmts"]:
        if isinstance(t, tuple):
            continue
        st = fn.temps[t]
        if st[0] == "load":
            acc.append(st[2])
        elif st[0] == "call":
            acc.append(st[1])
            acc.extend(st[2])
    t = node["term"]
    if t[0] == "if":
        acc.extend([t[1][2], t[1][3]])
        all_exprs(t[2], fn, acc), all_exprs(t[3], fn, acc)
    return acc


def stores_in(node, fn, acc):
    for t in node["stmts"]:
        if isinstance(t, tuple):
            acc.extend([t[2], t[3]])
    t = node["term"]
    if t[0] == "if":
        stores_in(t[2], fn, acc), stores_in(t[3], fn, acc)
    return acc


def max_in(exprs):
    m = -1
    for e in exprs:
        def walk(x):
            nonlocal m
            if x[0] == "in":
                m = max(m, x[1])
            elif x[0] == "bin":
                walk(x[2]), walk(x[3])
            elif x[0] == "not":
                walk(x[1])
            elif x[0] == "cmp":
                walk(x[2]), walk(x[3])
        walk(e)
    return m


class Generator:
    def __init__(self, idx, img, names):
        self.idx = idx
        self.img = img
        self.names = names

    def analyze(self, addr, insns):
        fn = Fn(addr, insns, self.img, self.names)
        tree = fn.run()
        # normalise statement lists: temps are ints, stores are tuples
        return fn, tree

    def generate(self, addr, insns):
        fn, tree = self.analyze(addr, insns)
        name, params, ret, raw = self.names.proto(addr)
        mangled = self.names.mangled(addr)
        terms = all_terms(tree, [])
        exprs = all_exprs(tree, fn, []) + stores_in(tree, fn, [])
        if any(has(e, lambda x: x[0] in ("sp", "lr", "clob")) for e in exprs):
            raise Reject("sp/lr/clobbered value used")
        if any(has(e, lambda x: x[0] == "cs") for e in exprs):
            raise Reject("reads r4+ at entry")
        # return kind
        # a callee's r0:r1 is passed on as a 64-bit result only when that callee is declared to return 64 bits
        # (its r1 is otherwise a leftover: a rewritten callee leaves a different one, and no caller reads it)
        def wide_callee(tid):
            target = fn.temps[tid][1]
            if not (isinstance(target, tuple) and target[0] == "const"):
                return False
            callee_ret = self.names.proto(target[1] & ~1)[2].replace("const ", "").strip()
            return any(x in callee_ret for x in WORD2) and "*" not in callee_ret
        calls_pair = any(t[1][0] == "lo" and t[2] == ("hi", t[1][1]) and wide_callee(t[1][1]) for _, t in terms)
        r_words = None
        rt = ret.replace("const ", "").strip()
        if any(x in rt for x in WORD2) and "*" not in rt:
            r_words = 2
        # declared void (IDA's prototype, or a std::function<void(...)> slot): whatever a callee leaves in r0/r1
        # is not a result - callers never read it, and a rewritten callee leaves other leftovers (the verifier
        # would report them as mismatches)
        declared_void = rt == "void" or re.search(r",\s*void \(\)\([^)]*\)>::operator\(\)", name or "") is not None
        # cocos2d's core virtuals are void in the engine source; IDA often types them int (CallFunc::update)
        declared_void = declared_void or re.fullmatch(
            r"cocos2d::(?:\w+::)*(?:update|step|startWithTarget|stop|execute|onEnter|onExit|onEnterTransitionDidFinish|"
            r"onExitTransitionDidStart|cleanup|visit|draw)", name or "") is not None
        # a game class's update(float) overrides cocos2d::Node::update (void); IDA types many of them int
        # (SupportCharacterScrollView::update returned IScrollProtocol::update's leftover r0)
        declared_void = declared_void or (re.fullmatch(r"(?:\w+::)+update", name or "") is not None
                                          and re.search(r"\(\w+ \*\*?this, float \w+\)", raw or "") is not None)
        # every value r0 can have at a return is what a void callee left there (e.g. a wrapper around free)
        def void_callee(tid):
            target = fn.temps[tid][1]
            return (isinstance(target, tuple) and target[0] == "const"
                    and self.names.proto(target[1] & ~1)[2].strip() == "void")
        only_void_leftovers = bool(terms) and all(
            t[1] == ("in", 0) or (t[1][0] == "lo" and void_callee(t[1][1])) for _, t in terms) and any(
            t[1][0] == "lo" for _, t in terms)
        if declared_void or only_void_leftovers:
            ret_kind = "void"
        elif calls_pair or r_words == 2:
            ret_kind = "u64"
        elif all(t[1] == ("in", 0) for _, t in terms) and (rt == "void" or not rt or raw == ""):
            ret_kind = "void"
        elif all(t[1] == ("in", 0) for _, t in terms) and rt == "void":
            ret_kind = "void"
        else:
            ret_kind = "u32"
        for _, t in terms:
            vals = [t[1]] + ([t[2]] if ret_kind == "u64" else [])
            for v in vals:
                if has(v, lambda x: x[0] in ("clob", "sp", "lr", "cs")):
                    raise Reject("return value unknown")
        # parameters
        own = proto_words(params)
        used = max_in(exprs + ([t[1] for _, t in terms] if ret_kind != "void" else [])
                      + ([t[2] for _, t in terms] if ret_kind == "u64" else []))
        np_ = max(used + 1, min(own, 4) if own is not None else 0)
        if own is None and np_ < 4:
            np_ = max(np_, used + 1)
        member = bool(params) and params[0].strip().endswith("this")
        pnames = ["self" if member else "a0"] + [f"a{k}" for k in range(1, 16)]
        return fn, tree, ret_kind, np_, pnames, (name, params, ret, raw, mangled)

    def render(self, addr, fn, tree, ret_kind, np_, pnames, info):
        name, params, ret, raw, mangled = info
        w = Writer(fn, self.names, pnames, ret_kind)
        # uses of temps across the whole tree (to choose what to inline)
        count = {}

        def cnt(e):
            if e[0] in ("temp", "lo", "hi"):
                count[e[1]] = count.get(e[1], 0) + 1
                if e[0] == "hi":
                    w.wide.add(e[1])
            elif e[0] == "bin":
                cnt(e[2]), cnt(e[3])
            elif e[0] == "not":
                cnt(e[1])
            elif e[0] == "cmp":
                cnt(e[2]), cnt(e[3])

        def cnt_node(n):
            for t in n["stmts"]:
                if isinstance(t, tuple):
                    cnt(t[2]), cnt(t[3])
                else:
                    st = fn.temps[t]
                    if st[0] == "load":
                        cnt(st[2])
                    else:
                        cnt(st[1])
                        for a in st[2]:
                            cnt(a)
            t = n["term"]
            if t[0] == "ret":
                cnt(t[1])
                if ret_kind == "u64":
                    cnt(t[2])
            else:
                cnt(t[1])
                cnt_node(t[2]), cnt_node(t[3])
        cnt_node(tree)
        shape = classify(fn, tree, ret_kind)
        body = []
        self.emit_node(w, tree, count, body, 1, ret_kind)
        # function
        cname = self.cname(addr, name, mangled, shape, fn)
        rtype = {"u64": "uint64_t", "u32": "uint32_t", "void": "void"}[ret_kind]
        ps = ", ".join(f"uint32_t {pnames[k]}" for k in range(np_))
        pretty = self.names.pretty(addr)
        out = [f"// {pretty}"]
        if raw and not raw.startswith(("int __fastcall sub_", "void __fastcall sub_", "int sub_", "int **sub_")) \
                and len(raw) < 400:
            out.append(f"// IDA: {raw}")
        out.append(f"static {rtype} {cname}({ps}) {{")
        out += body
        out.append("}")
        q = pretty.replace("\\", "\\\\").replace('"', '\\"')
        impure = sorted({self.names.plt[st[1][1]] for st in fn.temps.values()
                         if st[0] == "call" and is_const(st[1]) and st[1][1] in self.names.plt
                         and self.names.plt[st[1][1]] not in PURE_IMPORTS})
        if impure:
            out.append(f"// not verified: calls {', '.join(impure)} (effects outside guest memory)")
            out.append(f'RW_FUNCTION_NOVERIFY(0x{addr:X}, "{q}", {cname});')
        else:
            out.append(f'RW_FUNCTION(0x{addr:X}, "{q}", {cname});')
        return "\n".join(out), shape, cname

    def cname(self, addr, name, mangled, shape, fn):
        if name and not name.startswith("sub_") and not mangled.startswith("sub_"):
            base = ident(name, special_kind(mangled))
            if mangled.startswith(("_ZTh", "_ZTv")):
                base = "thunk_" + base
        else:
            base = shape_name(shape, fn, self.names)
        return f"{base}_{addr:X}"

    def emit_node(self, w, node, count, out, depth, ret_kind):
        ind = "    " * depth
        stmts = node["stmts"]
        term = node["term"]
        for i, t in enumerate(stmts):
            if isinstance(t, tuple):                                  # store
                ty, a, v = t[1], t[2], t[3]
                vs = w.ex(v)
                if ty != "uint32_t":
                    vs = f"{ty}({vs})"
                out.append(w.note(f"{ind}rw::save<{ty}>({w.ex(a)}, {vs});"))
                continue
            st = w.fn.temps[t]
            if st[0] == "load":
                ty, a = st[1], st[2]
                e = f"rw::load<{ty}>({w.ex(a)})"
                if ty != "uint32_t":
                    e = f"uint32_t({e})"
                if self.next_use_ok(w, stmts, i, t, count, node):
                    w.inl[t] = (e, w.notes)
                    w.notes = []
                else:
                    w.vnames[t] = f"v{t}"
                    out.append(w.note(f"{ind}const uint32_t v{t} = {e};"))
                continue
            # call
            target, args, where = st[1], st[2], st[3]
            n = count.get(t, 0)
            last = i == len(stmts) - 1
            pair = last and term[0] == "ret" and ret_kind == "u64" and term[1] == ("lo", t) and term[2] == ("hi", t)
            if pair:
                w.wide.add(t)
            call = self.call_expr(w, target, args, where, t in w.wide)
            if pair:
                out.append(w.note(f"{ind}return {call};"))
                return
            if last and term[0] == "ret" and ret_kind == "u32" and term[1] == ("lo", t) and n == 1:
                out.append(w.note(f"{ind}return {call};"))
                return
            if n == 0:
                out.append(w.note(f"{ind}{call};"))
            elif n == 1 and t not in w.wide and i + 1 < len(stmts) and not isinstance(stmts[i + 1], tuple)                     and w.fn.temps[stmts[i + 1]][0] == "call" and ("lo", t) in w.fn.temps[stmts[i + 1]][2]                     and not any(uses(x, []) for x in w.fn.temps[stmts[i + 1]][2])                     and is_const(w.fn.temps[stmts[i + 1]][1]):
                w.inl[t] = (call, w.notes)                            # nested: g(f(x)), nothing in between
                w.notes = []
            else:
                w.vnames[t] = f"r{t}"
                ty = "uint64_t" if t in w.wide else "uint32_t"
                out.append(w.note(f"{ind}const {ty} r{t} = {call};"))
        if term[0] == "ret":
            if ret_kind == "void":
                if depth > 1:
                    out.append(f"{ind}return;")
                return
            if ret_kind == "u32":
                out.append(w.note(f"{ind}return {w.ex(term[1])};"))
            else:
                lo, hi = term[1], term[2]
                if lo[0] == "lo" and hi == ("hi", lo[1]) and lo[1] in w.vnames:
                    out.append(f"{ind}return {w.vnames[lo[1]]};")
                else:
                    out.append(w.note(f"{ind}return uint64_t({w.ex(lo)}) | uint64_t({w.ex(hi)}) << 32;"))
            return
        out.append(w.note(f"{ind}if ({cond_str(w, term[1])}) {{"))
        self.emit_node(w, term[2], count, out, depth + 1, ret_kind)
        out.append(f"{ind}}} else {{")
        self.emit_node(w, term[3], count, out, depth + 1, ret_kind)
        out.append(f"{ind}}}")

    def next_use_ok(self, w, stmts, i, t, count, node):
        """A load temp can be inlined when it is used once, by the next store/call of this node or by the
        terminal, with only loads in between (so moving the read later changes nothing)."""
        if count.get(t, 0) != 1:
            return False
        for j in range(i + 1, len(stmts)):
            s = stmts[j]
            if isinstance(s, tuple):
                return t in uses(s[2], []) + uses(s[3], [])
            st = w.fn.temps[s]
            if st[0] == "load":
                if t in uses(st[2], []):
                    return True
                continue
            return t in uses(st[1], []) + sum((uses(a, []) for a in st[2]), [])
        term = node["term"]
        if term[0] == "ret":
            return t in uses(term[1], []) + uses(term[2], [])
        return t in uses(term[1][2], []) + uses(term[1][3], [])

    def call_expr(self, w, target, args, where, wide):
        a = ", ".join(w.ex(x) for x in args)
        tail = where == "tail"
        kind = "rw::call_exact" if tail else ("rw::call64" if wide else "rw::call")
        if is_const(target):
            w.notes.insert(0, self.names.pretty(target[1] & ~1) or f"0x{target[1]:X}")
            return f"{kind}(0x{target[1]:X}, {{{a}}})"
        vt = vslot(w, target)
        if vt:
            obj, slot = vt
            if not tail and not wide:
                return f"rw::vcall(rw::host_ptr<uint32_t>({w.ex(obj)}), {slot}, {{{a}}})"
            w.notes.insert(0, f"virtual slot {slot}")
            return f"{kind}(rw::load<uint32_t>(rw::load<uint32_t>({w.ex(obj)}) + {slot * 4}u), {{{a}}})"
        w.notes.insert(0, "call through a function pointer")
        return f"{kind}({w.ex(target)}, {{{a}}})"


def vslot(w, target):
    """(object expr, slot) when the call target is *(*(obj) + 4 * slot), both loads inlined into this call."""
    if target[0] != "temp" or target[1] not in w.inl:
        return None
    st = w.fn.temps[target[1]]
    if st[0] != "load" or st[1] != "uint32_t":
        return None
    a = st[2]
    if a[0] == "temp":
        t0, k = a[1], 0
    elif a[0] == "bin" and a[1] == "+" and a[2][0] == "temp" and is_const(a[3]):
        t0, k = a[2][1], a[3][1]
    else:
        return None
    if t0 not in w.inl or k % 4 or k > 0x4000:
        return None
    st0 = w.fn.temps[t0]
    if st0[0] != "load" or st0[1] != "uint32_t":
        return None
    return st0[2], k // 4


def classify(fn, tree, ret_kind):
    """Shape label (for statistics and generated names of unnamed functions)."""
    terms = all_terms(tree, [])
    if tree["term"][0] == "if":
        return "branching"
    stmts = tree["stmts"]
    kinds = []
    for t in stmts:
        if isinstance(t, tuple):
            kinds.append("store")
        else:
            st = fn.temps[t]
            kinds.append("tail" if st[0] == "call" and st[3] == "tail" else st[0])
    r0 = tree["term"][1]
    if not kinds:
        if ret_kind == "void" or r0 == ("in", 0):
            return "empty"
        if is_const(r0):
            return "return_constant"
        return "return_expr"
    if kinds == ["tail"]:
        return "tail_call"
    if kinds == ["call"]:
        return "forward_call"
    if all(k == "call" for k in kinds):
        return "call_chain"
    if all(k == "load" for k in kinds):
        return "getter"
    if all(k == "store" for k in kinds):
        return "setter"
    if "call" in kinds or "tail" in kinds:
        return "load_and_call" if "store" not in kinds else "mixed"
    return "load_store"


def shape_name(shape, fn, names):
    tree_calls = [fn.temps[t] for t in fn.temps if fn.temps[t][0] == "call"]
    if shape in ("forward_call", "tail_call", "call_chain", "load_and_call") and tree_calls:
        tgt = tree_calls[-1][1]
        if is_const(tgt):
            n = names.proto(tgt[1] & ~1)[0] or names.mangled(tgt[1] & ~1)
            pre = {"forward_call": "forward_to", "tail_call": "tail_to", "call_chain": "calls",
                   "load_and_call": "calls"}[shape]
            return f"{pre}_{ident(n)}"
    return {"empty": "empty", "return_constant": "return_constant", "return_expr": "compute", "getter": "get",
            "setter": "set"}.get(shape, "fn")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--max-size", type=int, default=32)
    ap.add_argument("--per-file", type=int, default=700)
    ap.add_argument("--stats", action="store_true", help="print rejection reasons")
    ap.add_argument("--only", help="comma-separated hex addresses (print, do not write)")
    args = ap.parse_args()
    idx = json.loads(INDEX.read_text())
    img = Image(SO)
    names = Names(idx)
    gen = Generator(idx, img, names)
    done = registered_elsewhere()
    files = {}
    rejects = {}
    results = []
    only = {int(x, 16) for x in args.only.split(",")} if args.only else None
    for key, e in sorted(idx.items()):
        a = int(key, 16)
        if only is not None and a not in only:
            continue
        if e["size"] > args.max_size or a in done or not e["lifted"]:
            continue
        if re.match(r"__aeabi_c[fd]r?cmp", e["name"]):
            # the ARM EABI float comparisons return their result in the condition flags, which a rewrite does
            # not reproduce (their callers branch on them)
            rejects.setdefault("returns in flags", []).append(a)
            continue
        f, line = e["lifted"]
        if f not in files:
            files[f] = (LIFT / f).read_text(encoding="utf-8").split("\n")
        try:
            insns = lifted_insns(files[f], line)
            if not insns or insns[0][0] != a or any(not (a <= x < a + e["size"]) for x, _, _ in insns):
                raise Reject("instruction range")
            fn, tree, rk, np_, pn, info = gen.generate(a, insns)
            code, shape, cname = gen.render(a, fn, tree, rk, np_, pn, info)
            results.append((a, code, shape, e["covered"]))
        except Reject as r:
            reason = str(r).split(":")[0]
            rejects.setdefault(reason, []).append(key)
    if only is not None:
        for a, code, shape, cov in results:
            print(f"// shape: {shape}\n{code}\n")
        for k, v in rejects.items():
            print("REJECT", k, v)
        return
    # write files
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    for old in OUT_DIR.glob("gen_*.cpp"):
        old.unlink()
    for n in range(0, len(results), args.per_file):
        chunk = results[n:n + args.per_file]
        lo, hi = chunk[0][0], chunk[-1][0]
        path = OUT_DIR / f"gen_{lo:07X}.cpp"
        head = [f"// Generated by rewrite/tools/gen_simple.py - do not edit (re-run the generator instead).",
                f"// Small fixed-shape functions 0x{lo:X}..0x{hi:X}, each replacing its lifted original.",
                '#include "rw/rw.hpp"', ""]
        path.write_text("\n".join(head) + "\n\n".join(c for _, c, _, _ in chunk) + "\n", encoding="utf-8")
    shapes = {}
    for a, code, shape, cov in results:
        s = shapes.setdefault(shape, [0, 0])
        s[0] += 1
        s[1] += bool(cov)
    print(f"generated {len(results)} functions ({sum(1 for r in results if r[3])} executed in game) into "
          f"{(len(results) + args.per_file - 1) // args.per_file} files")
    for s, (n, c) in sorted(shapes.items(), key=lambda x: -x[1][0]):
        print(f"  {s:16} {n:6}  (executed {c})")
    print(f"skipped {sum(len(v) for v in rejects.values())}")
    if args.stats:
        for k, v in sorted(rejects.items(), key=lambda x: -len(x[1]))[:40]:
            print(f"  {len(v):6}  {k}  e.g. {' '.join(v[:3])}")


if __name__ == "__main__":
    main()
