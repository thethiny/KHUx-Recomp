"""Recompute the README "Progress" bars from the current inputs and outputs, and rewrite that section.

python recomp/progress.py [--dry-run]

- Decompiled: functions in the Hex-Rays export (Input/.../libcocos2dcpp_decompile/decompile) that have
  pseudocode, i.e. not marked "// decompile returned None".
- Lifted: analysed functions (out/analysis.json) the lifter translates to C. Runs the lifter over every
  function without compiling (about a minute). Also reports how many run in game (out/coverage.json) and the
  native instruction share of the last profiled hybrid run (out/lift/profile.json), when present.
- Rewritten: functions registered in rewrite/src (RW_FUNCTION). Run in game / Verified: the M3 verifier's results
  (rewrite/status.tsv: rewritten functions called while verifying; rewrite/verified.txt: those that matched the
  lifted original, after `rewrite/tools/merge_stats.py`).
- 1:1 binary match, Recompiled: read from progress.json ({"matching": n, "recompiled": n}) when it exists, else 0
  (Recompiled defaults to the full lift once the native host exists).
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
    seen = paths.OUT / "host" / "seen.txt"   # the native exe's coverage (KHUX_COVERAGE=1)
    if seen.exists():
        blocks |= {int(l, 16) for l in seen.read_text().split() if l.strip()}
    an = json.loads((paths.OUT / "analysis.json").read_text())
    return sum(1 for f in an["functions"] if int(f["addr"], 16) in blocks or int(f["addr"], 16) | 1 in blocks)


def native_share():
    prof = paths.OUT / "lift" / "profile.json"
    if not prof.exists():
        return None
    return json.loads(prof.read_text()).get("native_share")


def chunks():
    """{IDA chunk: [parents]} (rewrite/tools/chunks.py): pieces of other functions' bodies, counted with them."""
    path = HERE / "rewrite" / "tasks" / "chunks.tsv"
    out = {}
    if path.exists():
        for line in path.read_text(encoding="utf-8").splitlines():
            if line and not line.startswith("#"):
                a, ps = line.split("	")
                out[int(a, 16)] = [int(p, 16) for p in ps.split()]
    return out


def with_chunks(addrs):
    """addrs plus the chunks of any function in it (transitively: a chunk of a chunk counts too)."""
    out, cs = set(addrs), chunks()
    while True:
        more = {a for a, ps in cs.items() if a not in out and any(p in out for p in ps)}
        if not more:
            return out
        out |= more


def rewritten_set():
    pat = re.compile(r"^\s*RW_FUNCTION(?:_NOVERIFY)?\(\s*(0x[0-9A-Fa-f]+)", re.M)
    addrs = set()
    for f in (HERE / "rewrite" / "src").rglob("*.cpp"):
        addrs.update(int(a, 16) & ~1 for a in pat.findall(f.read_text(encoding="utf-8", errors="replace")))
    return with_chunks(addrs)


def verifier_sets():
    status, verified = HERE / "rewrite" / "status.tsv", HERE / "rewrite" / "verified.txt"
    ran = ({int(l.split("	")[0], 16) & ~1 for l in status.read_text(encoding="utf-8").splitlines()[1:] if l.strip()}
           if status.exists() else set())
    ok = ({int(l.split()[0], 16) & ~1 for l in verified.read_text().splitlines() if l.strip()}
          if verified.exists() else set())
    return with_chunks(ran), with_chunks(ok)


def split():
    """{address: True for open-source library code} (rewrite/tools/libsplit.py), or None without the index."""
    try:
        sys.path.insert(0, str(HERE / "rewrite" / "tools"))
        import libsplit
        return {a & ~1: lib is not None for a, lib in libsplit.groups().items()}
    except Exception:
        return None


def bar(pct):
    n = round(pct / 5)
    return "█" * n + "░" * (20 - n)


def main():
    dec_ok, dec_total, dec_failed = decompiled()
    lift_ok, lift_total, escapes = lifted()
    game = in_game()
    share = native_share()
    manual = json.loads(MANUAL.read_text()) if MANUAL.exists() else {}
    matching, recompiled = (manual.get(k, 0) for k in ("matching", "recompiled"))
    rw_set = rewritten_set()
    ran_set, ok_set = verifier_sets()
    rewritten, ran, verified = len(rw_set), len(ran_set), len(ok_set)
    is_lib = split()
    # the native host links the full lift: every function in it runs natively, with no emulator
    full = HERE / "out" / "lift_all" / "lifted.json"
    if not recompiled and (HERE / "out" / "host" / "khux.exe").exists() and full.exists():
        recompiled = min(len(json.loads(full.read_text())), lift_total)

    game_txt = f" {game:,} of them run and are checked against the emulator in game." if game else ""
    share_txt = (f" In hybrid mode about {share * 100:.2f}% of the executed instructions already run as native C,"
                 if share else " Hybrid mode runs lifted C inside the emulator,")
    # (group, [(row, done, total)]), then the notes printed under the block
    groups = [("Translation", [
        ("Decompiled", dec_ok, dec_total),
        ("Lifted", lift_ok, lift_total),
        ("Recompiled", recompiled, lift_total),
    ])]

    def rewrite_rows(rw, ran_, ok, total):
        return [("Rewritten", len(rw), total), ("Run under the verifier", len(ran_), total),
                ("Verified", len(ok), total)]

    lib_txt = ""
    if is_lib:
        n_lib = sum(1 for v in is_lib.values() if v)
        n_game = len(is_lib) - n_lib

        def part(addrs, lib):
            return {a for a in addrs if is_lib.get(a) is lib}
        groups.append(("Rewrite: game code", rewrite_rows(part(rw_set, False), part(ran_set, False),
                                                          part(ok_set, False), n_game)))
        groups.append(("Rewrite: open-source libraries", rewrite_rows(part(rw_set, True), part(ran_set, True),
                                                                      part(ok_set, True), n_lib)))
        lib_txt = (f" **Game code** is the game's own {n_game:,} functions, Square Enix's sound engine included; "
                   f"**open-source libraries** are the other {n_lib:,} (cocos2d-x, libc++, OpenSSL, FreeType, libpng "
                   "and others; `rewrite/tools/libsplit.py`), which a native build can take from upstream source "
                   "instead.")
    groups.append(("Rewrite: all functions", rewrite_rows(rw_set, ran_set, ok_set, lift_total)))
    groups.append(("Later, in a separate repository", [("1:1 binary match", matching, lift_total)]))

    notes = [
        ("Decompiled", f"functions with Hex-Rays pseudocode ({dec_failed:,} failed). Reference only."),
        ("Lifted", f"functions the lifter translates to C (machine-generated, one C function per guest function, "
                   f"{escapes} instructions left to the emulator).{game_txt}"),
        ("Recompiled", "a native executable that runs without the emulator." + (
            f" The native host (`host/`, Windows) runs every lifted function natively, with real threads, sound, "
            "sockets, JNI and OpenGL ES, and no emulator or Python: it plays the game through login, the main "
            "menu, quests, raids and multiplayer against the private server." if recompiled else
            share_txt + " and memory/string functions, math, the heap, stdio streams and OpenGL ES are native, but "
            "APK assets, threads, JNI, sockets and audio still go through the Python runtime (milestone M2).")),
        ("Rewritten", "readable C++ (named types and variables, no register-level code) in `rewrite/`." + lib_txt),
        ("Run under the verifier", "rewritten functions the game has called while the M3 verifier compared them "
                                   "with the lifted original."),
        ("Verified", "rewritten functions that matched the lifted original on every checked call. Normal play "
                     "installs only these."),
        ("1:1 binary match", "source that rebuilds the original ARM binary byte for byte (a matching "
                             "decompilation). Planned, with the cleanup and the native build, for a fork of the "
                             "finished rewrite."),
    ]

    w = max(len(name) for _, rows in groups for name, _, _ in rows)
    out = ["## Progress", "", "```"]
    for k, (g, rows) in enumerate(groups):
        out += ([""] if k else []) + [g]
        out += [f"  {name.ljust(w)}  {bar(100 * n / t)}  {100 * n / t:5.1f}%  {n:>6,} / {t:,}"
                for name, n, t in rows]
    out += ["```", "", "Updated by `python recomp/progress.py`.", ""]
    out += [f"- **{name}**: {desc}" for name, desc in notes]
    out += [""]
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
