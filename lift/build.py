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
PER_FILE = 1500
CRLF = chr(13) + chr(10)
VCVARS = Path(r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat")

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
            if (external(target)) { c->emu(c, target); return; }
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
    out = []
    for a, m in fn:
        if executed is not None:
            seen = [mm for mm, bit in ((AN.ARM, 0), (AN.THUMB, 1)) if (a | bit) in executed]
            if not seen:
                continue
            if m not in seen:
                m = seen[0]              # the static analysis guessed the mode wrong: trust what ran
                fn_mode[a] = m
        r = AN.analyze_function(d, a, m, text_lo, text_hi, {}, fn_starts, img.sec[".plt"], noret, frags)
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
    args = ap.parse_args()
    t0 = time.time()
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
