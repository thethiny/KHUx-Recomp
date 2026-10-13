"""Lift functions of the game library to C and build lift.dll (MSVC).

python recomp/lift/build.py [--limit N] [--no-compile] [--leaves] [--all]

Default: every function whose entry executed in a recorded run (out/coverage.json, see emu/coverage.py).
--leaves: only leaf functions (no calls, indirect jumps, switch tables or tail calls). --all: every function.
Instructions the lifter cannot translate become escapes to the emulator (see lifter.py); functions the lifter
rejects outright are listed in out/lift/unsupported.txt and stay emulated.
"""
import argparse
import json
import subprocess
import sys
import time
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
sys.path.insert(0, str(HERE))
import analyze as AN  # noqa: E402
import paths  # noqa: E402
from lifter import FuncLifter, Unsupported  # noqa: E402

OUT = paths.OUT / "lift"
PER_FILE = 400                 # small files keep every core busy under cl /MP
CRLF = chr(13) + chr(10)
VCVARS = Path(r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat")

# Imports implemented in C (pure memory and string functions): lifted code calls them directly instead of
# going through the emulator's Python handlers (emu/libc.py, whose results these match). Writes go through
# MEMMOVE / MEMSET, so verification and speculative worker runs log them like any other store.
NATIVE_IMPORTS = {
    **{n: "MEMMOVE(c, c->r[0], c->r[1], c->r[2]);" for n in (
        "memcpy", "memmove", "__aeabi_memcpy", "__aeabi_memcpy4", "__aeabi_memcpy8",
        "__aeabi_memmove", "__aeabi_memmove4", "__aeabi_memmove8")},
    "memset": "MEMSET(c, c->r[0], c->r[1], c->r[2]);",
    **{n: "MEMSET(c, c->r[0], c->r[2], c->r[1]);" for n in ("__aeabi_memset", "__aeabi_memset4", "__aeabi_memset8")},
    **{n: "MEMSET(c, c->r[0], 0, c->r[1]);" for n in ("__aeabi_memclr", "__aeabi_memclr4", "__aeabi_memclr8",
                                                       "bzero")},
    "memcmp": "c->r[0] = c->r[2] ? SIGN3(memcmp(HA(c->r[0]), HA(c->r[1]), c->r[2])) : 0;",
    "memchr": ("{ const uint8_t *p = c->r[2] ? memchr(HA(c->r[0]), (int)(c->r[1] & 0xFF), c->r[2]) : 0; "
               "c->r[0] = p ? c->r[0] + (uint32_t)(p - HA(c->r[0])) : 0; }"),
    "strlen": "c->r[0] = (uint32_t)strlen((const char *)HA(c->r[0]));",
    "strcmp": "c->r[0] = SIGN3(strcmp((const char *)HA(c->r[0]), (const char *)HA(c->r[1])));",
    "strncmp": "c->r[0] = c->r[2] ? SIGN3(strncmp((const char *)HA(c->r[0]), (const char *)HA(c->r[1]), c->r[2])) : 0;",
    "strchr": ("{ const char *s = (const char *)HA(c->r[0]); uint32_t ch = c->r[1] & 0xFF; "
               "const char *p = ch ? strchr(s, (int)ch) : s + strlen(s); "
               "c->r[0] = p ? c->r[0] + (uint32_t)(p - s) : 0; }"),
    "strrchr": ("{ const char *s = (const char *)HA(c->r[0]); uint32_t ch = c->r[1] & 0xFF; "
                "const char *p = ch ? strrchr(s, (int)ch) : s + strlen(s); "
                "c->r[0] = p ? c->r[0] + (uint32_t)(p - s) : 0; }"),
    "strcpy": "MEMMOVE(c, c->r[0], c->r[1], (uint32_t)strlen((const char *)HA(c->r[1])) + 1);",
    "strcat": ("{ uint32_t d = c->r[0] + (uint32_t)strlen((const char *)HA(c->r[0])); "
               "MEMMOVE(c, d, c->r[1], (uint32_t)strlen((const char *)HA(c->r[1])) + 1); }"),
    "strncpy": ("{ uint32_t n = c->r[2], k = (uint32_t)strnlen((const char *)HA(c->r[1]), n); "
                "MEMMOVE(c, c->r[0], c->r[1], k); MEMSET(c, c->r[0] + k, 0, n - k); }"),
    # math (emu/libc.py computes the float versions in double precision and rounds: same here)
    **{n: f"RET_D(c, {n}(D64(c->r[0], c->r[1])));" for n in (
        "sin", "cos", "tan", "asin", "acos", "atan", "exp", "log", "log10", "sqrt", "fabs", "floor")},
    **{n: f"RET_D(c, {n}(D64(c->r[0], c->r[1]), D64(c->r[2], c->r[3])));" for n in ("atan2", "pow", "fmod")},
    **{n: f"c->r[0] = FBITS((float){n[:-1]}((double)F32(c->r[0])));" for n in (
        "sinf", "cosf", "tanf", "asinf", "acosf", "expf", "logf", "log10f", "sqrtf", "floorf", "ceilf")},
    **{n: f"c->r[0] = FBITS((float){n[:-1]}((double)F32(c->r[0]), (double)F32(c->r[1])));"
       for n in ("atan2f", "powf", "fmodf")},
    "qsort": "return native_qsort(c);",
    # stdio streams (lift/rt_files.c)
    **{n: f"return native_{n.rstrip('o') if n in ('fseeko', 'ftello') else n}(c);" for n in (
        "fopen", "fclose", "fread", "fwrite", "fgets", "fseek", "fseeko", "ftell", "ftello", "feof", "fputs",
        "fputc")},
    "fputwc": "return native_fputc(c);",
    "__errno": "if (!rt_errno_ptr) return 0; c->r[0] = rt_errno_ptr;",
    # guest heap (lift/rt_heap.c), once the host has handed it over (rt_heap_ctl set)
    "malloc": "if (!rt_heap_ctl) return 0; c->r[0] = rt_heap_malloc(c, c->r[0]);",
    "calloc": "if (!rt_heap_ctl) return 0; c->r[0] = rt_heap_calloc(c, c->r[0], c->r[1]);",
    "realloc": "if (!rt_heap_ctl) return 0; c->r[0] = rt_heap_realloc(c, c->r[0], c->r[1]);",
    "free": "if (!rt_heap_ctl) return 0; rt_heap_free(c, c->r[0]);",
}


def native_import_c(plt):
    """C for the native imports: native_by_id (one case per import: libc, heap, OpenGL ES), native_import
    (PLT stub address or a registered host slot -> id), and the exports the host uses to call them by id and to
    register slots (eglGetProcAddress results)."""
    import gl_table
    names = list(NATIVE_IMPORTS) + gl_table.names()
    ids = {n: k for k, n in enumerate(names)}
    gl_src, gl_cases = gl_table.c_code(len(NATIVE_IMPORTS))
    lib_cases = [f"    case {ids[n]}: /* {n} */ {code} return 1;" for n, code in NATIVE_IMPORTS.items()]
    plt_cases = [f"    case 0x{a:X}u: return native_by_id(c, {ids[n]}); /* {n} */"
                 for a, n in sorted(plt.items()) if n in ids]
    return chr(10).join([
        "#define WIN32_LEAN_AND_MEAN", "#include <windows.h>", "#include <math.h>", gl_src,
        f"EXPORT const int rt_native_count = {len(names)};",
        "EXPORT const char *rt_native_names[] = {" + ", ".join(f'"{n}"' for n in names) + "};",
        "/* Run native import `id` on the registers in c (arguments in r0-r3 and on the stack, result in r0).",
        " * Returns 0 when it is not handled here (the caller falls back to the emulator's implementation). */",
        "static int native_by_id(CPU *c, int id) {", "    switch (id) {", *lib_cases, *gl_cases,
        "    default: return 0;", "    }", "}",
        "EXPORT int rt_native_call(CPU *c, int id) { return native_by_id(c, id); }",
        "static uint32_t slot_addr[256]; static int slot_id[256], slot_n;",
        "/* host import slots (function pointers handed to the guest, e.g. by eglGetProcAddress) */",
        "EXPORT void rt_native_slot(uint32_t addr, int id) {",
        "    for (int k = 0; k < slot_n; k++) if (slot_addr[k] == addr) { slot_id[k] = id; return; }",
        "    if (slot_n < 256) { slot_addr[slot_n] = addr; slot_id[slot_n++] = id; }", "}",
        "static int native_import(CPU *c, uint32_t target) {", "    switch (target & ~1u) {", *plt_cases,
        "    default: break;", "    }",
        "    for (int k = 0; k < slot_n; k++) if (slot_addr[k] == (target & ~1u)) return native_by_id(c, slot_id[k]);",
        "    return 0;", "}", ""])


RT_CALL = """
static LiftFn lookup(uint32_t target) {
    uint32_t a = target & ~1u;
    int lo = 0, hi = lift_count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        uint32_t m = lift_table[mid].addr & ~1u;
        if (m == a) return (lift_enabled[mid] && lift_table[mid].addr == target) ? lift_table[mid].fn : 0;
        if (m < a) lo = mid + 1; else hi = mid - 1;
    }
    return 0;
}

/* Outside the game's own code (PLT stubs -> imports, host function slots): proper functions that always return
 * to LR, safe to run synchronously in the emulator. */
static int external(uint32_t target) {
    uint32_t a = target & ~1u;
    return a < TEXT_LO || a >= TEXT_HI;
}

void rt_call(CPU *c, uint32_t target) {
    for (;;) {
        LiftFn f = lookup(target);
        c->tail = 0;
        if (!f) {
            if (external(target)) {
                if (native_import(c, target)) c->pc = c->r[14];
                else c->emu(c, target);
                return;
            }
            /* Game code that is not lifted is never run as a nested call: IDA-split pieces of a function
             * continue their parent's body and return through its frame, never to this call site. Escape
             * instead - the C chain unwinds and the emulator jumps there with LR already set. */
            c->pc = target;
            return;
        }
        f(c);
        if (!c->tail) return;
        target = c->pc;
    }
}

void rt_call_frag(CPU *c, uint32_t target) {
    LiftFn f = lookup(target);
    if (!f) { c->pc = target; c->tail = 0; return; }
    rt_call(c, target);
}

EXPORT void rt_undo(CPU *c) {
    uint32_t n = c->wn < c->wcap ? c->wn : c->wcap;
    while (n--) {
        WLog *w = &c->wlog[n];
        if (w->size == 1) R8(w->addr) = (uint8_t)w->old_lo;
        else if (w->size == 2) R16(w->addr) = (uint16_t)w->old_lo;
        else R32(w->addr) = w->old_lo;
    }
    c->wn = 0;
}

typedef int (*UcBatch)(void *uc, const int *regs, void **vals, int count);

void rt_vfp_load(CPU *c) {
    int ids[33];
    void *vals[33];
    for (int k = 0; k < 32; k++) { ids[k] = UC_REG_D0 + k; vals[k] = &c->vf.d[k]; }
    ids[32] = UC_REG_FPSCR;
    vals[32] = &c->fpscr;
    ((UcBatch)c->uc_read_batch)(c->uc, ids, vals, 33);
    c->vfp_on = 1;
}

EXPORT void rt_run(CPU *c, uint32_t target) {
    for (;;) {
        LiftFn f = lookup(target);
        c->tail = 0;
        if (!f) { c->pc = target; return; }
        f(c);
        if (!c->tail) return;
        target = c->pc;
    }
}
"""


def is_leaf(r):
    return not (r["calls"] or r["indirect_calls"] or r["indirect_jumps"] or r["switches"] or r["issues"]
                or r["tail_calls"] or r["far_branches"])


def thumb_pieces(d, fn, fn_starts, frags, case_helpers, lo, hi, plt_range, noret):
    """Thumb "functions" (IDA split points) that are really pieces of another function: Thumb-1 reaches them
    with bl used as a long branch. A piece never saves LR and never returns through it (it may call other
    functions), and it only leaves through other pieces or by popping its parent's frame. Absorbing them (like fragments) turns the jumps
    between pieces into gotos. Thunks (tail calls into real functions, LR live) are not pieces."""
    from capstone import arm as CA
    cands = []
    for a, m in fn:
        if m != AN.THUMB or a in frags:
            continue
        i = d.one(a, m)
        if i is None or (i.id == CA.ARM_INS_PUSH and any(o.reg == CA.ARM_REG_LR for o in i.operands)):
            continue                                  # an ordinary prologue: a real function
        cands.append(a)
    # local test once per candidate (tails = jumps into other function starts); then the largest consistent
    # set: drop candidates with a tail outside the set until stable (pieces jump into each other in cycles)
    starts = fn_starts - frags
    tails = {}
    for a in cands:
        r = AN.analyze_function(d, a, AN.THUMB, lo, hi, case_helpers, starts, plt_range, noret, frags)
        # a piece decoded on its own may reach its parent's switch dispatch without the bounding cmp, or with
        # an unrelated cmp whose bound makes the table overrun into code: both are artifacts of the context
        issues = [x for x in r["issues"]
                  if not x.startswith(("thumb switch without bound", "flow into literal"))]
        if r["push_lr"] or r["indirect_jumps"] or issues or not r["sizes"]:
            continue
        uses_lr = False
        for pc in r["sizes"]:
            i = d.one(pc, AN.THUMB)
            if i is not None and i.id in (CA.ARM_INS_BX, CA.ARM_INS_MOV) and                     any(o.type == CA.ARM_OP_REG and o.reg == CA.ARM_REG_LR for o in i.operands):
                uses_lr = True
                break
        if not uses_lr:
            tails[a] = set(r["tail_calls"])
    pieces = set(tails)
    while True:
        drop = {a for a in pieces if any(t not in pieces and t not in frags for t in tails[a])}
        if not drop:
            return pieces
        pieces -= drop


def candidates(only_leaves=False, everything=False):
    """(entry, mode, decode result, is_leaf) of the functions to lift, plus the function-mode and PLT maps."""
    an = json.loads((paths.OUT / "analysis.json").read_text())
    img = AN.Image(paths.so())
    d = AN.Disasm(img)
    fn = [(int(f["addr"], 16), f["mode"]) for f in an["functions"]]
    fn_starts = {a for a, _ in fn}
    fn_mode = dict(fn)
    frags = {int(x, 16) for x in an["fragments"]}
    plt = {int(k, 16): (int(v["target"], 16) if isinstance(v["target"], str) else v["target"])
           for k, v in an["plt"].items()}
    text_lo, text_hi = img.sec[".text"]
    noret = {int(x, 16) for x in an["noreturn"]}
    frags = {int(x, 16) for x in an["fragments"]}
    executed = None
    if not everything and not only_leaves:
        cov = paths.OUT / "coverage.json"
        if not cov.exists():
            sys.exit("no out/coverage.json: run the emulator once with --coverage (or pass --leaves / --all)")
        executed = set(json.loads(cov.read_text())["blocks"])
    starts_nofrag = fn_starts - frags
    # __gnu_thumb1_case_* switch helpers (direct or through PLT stubs), as analyze.py finds them: their call
    # sites become native switch dispatch
    by_name = {f.get("name"): int(f["addr"], 16) for f in an["functions"] if f.get("name")}
    case_helpers = {by_name[n] & ~1: v for n, v in AN.CASE_HELPERS.items() if n in by_name}
    case_helpers.update({int(k, 16): AN.CASE_HELPERS[v["name"]] for k, v in an["plt"].items()
                         if v["name"] in AN.CASE_HELPERS and v["target"]})
    # an IDA "function" that starts with a switch-helper call is the dispatch half of its parent's switch (the
    # bounding cmp is in the parent): absorb it like a fragment
    from capstone import arm as CA
    for a, m in fn:
        if m != AN.THUMB:
            continue
        i = d.one(a, m)
        if i is not None and i.id in (CA.ARM_INS_BL, CA.ARM_INS_BLX) and i.operands and                 i.operands[0].type == CA.ARM_OP_IMM and i.operands[0].imm in case_helpers:
            frags.add(a)
    frags |= thumb_pieces(d, fn, fn_starts, frags, case_helpers, text_lo, text_hi, img.sec[".plt"], noret)
    starts_nofrag = fn_starts - frags
    out = []
    for a, m in fn:
        if executed is not None:
            seen = [mm for mm, bit in ((AN.ARM, 0), (AN.THUMB, 1)) if (a | bit) in executed]
            if not seen:
                continue
            if m not in seen:
                m = seen[0]              # the static analysis guessed the mode wrong: trust what ran
                fn_mode[a] = m
        # fragments (IDA-split pieces that run in their parent's frame) are absorbed: decoding continues into
        # them, so a function and its pieces become one C function whose jumps between pieces are gotos
        r = AN.analyze_function(d, a, m, text_lo, text_hi, case_helpers, starts_nofrag, img.sec[".plt"], noret,
                                frags)
        leaf = is_leaf(r)
        if only_leaves and not leaf:
            continue
        out.append((a, m, r, leaf))
    return img, d, out, fn_mode, plt, frags


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--no-compile", action="store_true")
    ap.add_argument("--leaves", action="store_true")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--out", help="output folder (default out/lift; use another for --all so a running "
                                  "coverage build or emulator keeps its files)")
    args = ap.parse_args()
    t0 = time.time()
    global OUT
    if args.out:
        OUT = Path(args.out).resolve()
    img, d, funcs, fn_mode, plt, frags = candidates(args.leaves, args.all)
    if args.limit:
        funcs = funcs[:args.limit]
    print(f"{len(funcs)} candidate functions ({time.time() - t0:.0f}s)", flush=True)

    lf = FuncLifter(d.cs[AN.THUMB].reg_name, img, fn_mode, plt, frags)
    lifted, unsupported, reasons = [], [], Counter()
    escapes = 0
    for a, m, r, leaf in funcs:
        try:
            code = lf.function(d, a, m, r)
        except Unsupported as e:
            unsupported.append(f"{a:#x} {m} {e}")
            reasons[str(e).split(" ")[0]] += 1
            continue
        if lf.escapes:
            leaf = False                 # an escape hands control to the emulator: not verifiable on its own
        escapes += lf.escapes
        lifted.append((a, m, code, leaf))
    print(f"lifted {len(lifted)} ({sum(1 for x in lifted if x[3])} leaves, {escapes} escape points), "
          f"unsupported {len(unsupported)}: {reasons.most_common(12)}", flush=True)

    OUT.mkdir(parents=True, exist_ok=True)
    for old in OUT.glob("lift_*.c"):
        old.unlink()
    (OUT / "unsupported.txt").write_text("\n".join(unsupported))
    files = []
    for k in range(0, len(lifted), PER_FILE):
        part = lifted[k:k + PER_FILE]
        src = ['#include "rt.h"', "#include <math.h>", "#pragma warning(disable: 4102 4101 4146)"]
        src += [code for _, _, code, _ in part]
        f = OUT / f"lift_{k // PER_FILE:03d}.c"
        f.write_text("\n\n".join(src) + "\n")
        files.append(f)
    table = ['#include "rt.h"']
    table += [f"void F_{a:X}(CPU *c);" for a, _, _, _ in lifted]
    table.append("EXPORT const LiftEntry lift_table[] = {")
    table += [f"  {{0x{a | (1 if m == AN.THUMB else 0):X}u, {1 if leaf else 0}u, F_{a:X}}},"
              for a, m, _, leaf in lifted]
    table.append("};")
    table.append(f"EXPORT const int lift_count = {len(lifted)};")
    table.append(f"EXPORT uint8_t lift_enabled[{max(len(lifted), 1)}];   /* set by the host */")
    text_lo, text_hi = img.sec[".text"]
    table.append(f"#define TEXT_LO 0x{text_lo:X}u")
    table.append(f"#define TEXT_HI 0x{text_hi:X}u")
    from unicorn import arm_const as UA
    table.append(f"#define UC_REG_D0 {UA.UC_ARM_REG_D0}")
    table.append(f"#define UC_REG_FPSCR {UA.UC_ARM_REG_FPSCR}")
    an_plt = json.loads((paths.OUT / "analysis.json").read_text())["plt"]
    table.append('#include "rt_heap.c"')
    table.append('#include "rt_files.c"')
    table.append(native_import_c({int(k, 16): v["name"] for k, v in an_plt.items() if v["target"] is None}))
    table.append(RT_CALL)
    tf = OUT / "lift_table.c"
    tf.write_text("\n".join(table) + "\n")
    files.append(tf)
    json.dump([[a, m, leaf] for a, m, _, leaf in lifted], open(OUT / "lifted.json", "w"))
    print(f"wrote {len(files)} C files ({time.time() - t0:.0f}s)", flush=True)
    if args.no_compile:
        return

    srcs = " ".join(f'"{f.name}"' for f in files)
    bat = OUT / "build.bat"
    bat.write_text(CRLF.join([
        "@echo off",
        f'call "{VCVARS}" >nul',
        f'cl /nologo /O1 /LD /MP /W2 /wd4102 /wd4101 /wd4146 /I "{HERE}" {srcs} /Fe:lift.dll /link /NOLOGO',
        ""]))
    r = subprocess.run(["cmd", "/c", str(bat)], cwd=str(OUT), capture_output=True, text=True)
    log = (r.stdout + r.stderr)
    (OUT / "build.log").write_text(log)
    errs = [l for l in log.splitlines() if " error " in l or "fatal error" in l]
    print(f"cl exit {r.returncode}, {len(errs)} errors ({time.time() - t0:.0f}s)")
    for l in errs[:20]:
        print("  " + l)


if __name__ == "__main__":
    main()
