"""Recompute the README "Progress" bars from the current inputs and outputs, and rewrite that section.

python recomp/progress.py [--dry-run]

- Decompiled: functions in the Hex-Rays export (Input/.../libcocos2dcpp_decompile/decompile) that have
  pseudocode, i.e. not marked "// decompile returned None".
- Lifted: analysed functions (out/analysis.json) the lifter translates to C. Runs the lifter over every
  function without compiling (about a minute). Also reports how many run in game (out/coverage.json) and the
  native instruction share of the last profiled hybrid run (out/lift/profile.json), when present.
- Rewritten, 1:1 binary match, Recompiled: no tool measures these yet. They are read from progress.json
  ({"rewritten": n, "matching": n, "recompiled": n}, counts of functions) when it exists, else 0.
"""
import glob
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE / "lift"))
import paths  # noqa: E402

README = HERE / "README.md"
MANUAL = HERE / "progress.json"


def decompiled():
    folder = paths.decompile_index().parent / "decompile"
    total = failed = 0
    for f in sorted(glob.glob(str(folder / "libcocos2dcpp_*.c"))):
        lines = Path(f).read_text(encoding="utf-8", errors="replace").split("\n")
        for k, line in enumerate(lines):
            if line.startswith("// ==== "):
                total += 1
                if k + 1 < len(lines) and lines[k + 1].startswith("// decompile returned None"):
                    failed += 1
    if not total:
        sys.exit(f"no decompiled functions found in {folder}")
    return total - failed, total, failed


def lifted():
    import build
    from lifter import FuncLifter, Unsupported
    import analyze as AN
    img, d, funcs, fn_mode, plt, frags = build.candidates(everything=True)
    lf = FuncLifter(d.cs[AN.THUMB].reg_name, img, fn_mode, plt, frags)
    ok = escapes = 0
    for a, m, r, _ in funcs:
        try:
            lf.function(d, a, m, r)
        except Unsupported:
            continue
        ok += 1
        escapes += lf.escapes
    return ok, len(funcs), escapes


def in_game():
    cov = paths.OUT / "coverage.json"
    if not cov.exists():
        return None
    blocks = set(json.loads(cov.read_text())["blocks"])
    an = json.loads((paths.OUT / "analysis.json").read_text())
    return sum(1 for f in an["functions"] if int(f["addr"], 16) in blocks or int(f["addr"], 16) | 1 in blocks)


def native_share():
    prof = paths.OUT / "lift" / "profile.json"
    if not prof.exists():
        return None
    return json.loads(prof.read_text()).get("native_share")


def bar(pct):
    n = round(pct / 5)
    return "█" * n + "░" * (20 - n)


def main():
    dec_ok, dec_total, dec_failed = decompiled()
    lift_ok, lift_total, escapes = lifted()
    game = in_game()
    share = native_share()
    manual = json.loads(MANUAL.read_text()) if MANUAL.exists() else {}
    rewritten, matching, recompiled = (manual.get(k, 0) for k in ("rewritten", "matching", "recompiled"))
    # the native host links the full lift: every function in it runs natively, with no emulator
    full = HERE / "out" / "lift_all" / "lifted.json"
    if not recompiled and (HERE / "out" / "host" / "khux.exe").exists() and full.exists():
        recompiled = min(len(json.loads(full.read_text())), lift_total)

    game_txt = f" {game:,} of them run and are checked against the emulator in game." if game else ""
    share_txt = (f" In hybrid mode about {share * 100:.2f}% of the executed instructions already run as native C,"
                 if share else " Hybrid mode runs lifted C inside the emulator,")
    rows = [
        ("Decompiled", 100 * dec_ok / dec_total,
         f"{dec_ok:,} of {dec_total:,} functions have Hex-Rays pseudocode ({dec_failed:,} failed). Reference only."),
        ("Lifted", 100 * lift_ok / lift_total,
         f"{lift_ok:,} of {lift_total:,} analysed functions translate to C (machine-generated, one C function per "
         f"guest function, {escapes} instructions left to the emulator).{game_txt}"),
        ("Rewritten", 100 * rewritten / lift_total,
         "Readable, maintainable source (named types and variables, no register-level code)."
         + (f" {rewritten:,} functions." if rewritten else " Not started.")),
        ("1:1 binary match", 100 * matching / lift_total,
         "Source that rebuilds the original ARM binary byte for byte (a matching decompilation)."
         + (f" {matching:,} functions." if matching else " Not started.")),
        ("Recompiled", 100 * recompiled / lift_total,
         "A native executable that runs without the emulator." + (
             f" The native host (`host/`, Windows) runs all {recompiled:,} lifted functions natively, with real "
             "threads, sound, sockets, JNI and OpenGL ES, and no emulator or Python: it plays the game through "
             "login, the main menu and quests against the private server." if recompiled else
             share_txt + " and memory/string functions, math, the heap, stdio streams and OpenGL ES are native, but APK "
             "assets, threads, JNI, sockets and audio still go through the Python runtime (milestone M2).")),
    ]
    w = max(len(r[0]) for r in rows)
    out = ["## Progress", "", "```"]
    out += [f"{name.ljust(w)}  {bar(pct)}  {pct:5.1f}%" for name, pct, _ in rows]
    out += ["```", "", "Updated by `python recomp/progress.py`.", ""]
    out += [f"- **{name}**: {desc}" for name, _, desc in rows]
    section = "\n".join(out) + "\n\n"

    text = README.read_text(encoding="utf-8")
    m = re.search(r"## Progress\n.*?(?=\n## )", text, re.S)
    if not m:
        sys.exit("README.md has no '## Progress' section followed by another section")
    new = text[:m.start()] + section.rstrip("\n") + "\n" + text[m.end():]
    if "--dry-run" in sys.argv:
        sys.stdout.buffer.write(section.encode("utf-8"))
        return
    README.write_text(new, encoding="utf-8")
    sys.stdout.buffer.write(section.encode("utf-8"))


if __name__ == "__main__":
    main()
