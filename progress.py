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
- Images (docs/progress/, linked from the section): treemaps of the game code by class and of the libraries by
  library, one cell per function (verified / rewritten / not rewritten). Not written with --dry-run.
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


# ---- README image (docs/progress/): a treemap of the game code, self-made SVG, stdlib only ----

DOCS = HERE / "docs" / "progress"
# light theme first, then the dark overrides (GitHub renders README images with the viewer's color scheme)
SVG_STYLE = """<style>
.bg{fill:#ffffff}.v{fill:#2da44e}.r{fill:#d4a72c}.t{fill:#d0d7de}.grid{stroke:#ffffff}.edge{stroke:#ffffff}
.txt{fill:#1f2328;font-family:-apple-system,'Segoe UI',Helvetica,Arial,sans-serif}
.halo{stroke:#ffffff;stroke-width:3px;paint-order:stroke;stroke-linejoin:round}.dim{fill:#59636e}
@media (prefers-color-scheme: dark){
.bg{fill:#0d1117}.v{fill:#238636}.r{fill:#9e6a03}.t{fill:#30363d}.grid{stroke:#0d1117}.edge{stroke:#0d1117}
.txt{fill:#e6edf3}.halo{stroke:#0d1117}.dim{fill:#9198a1}}
</style>"""


def esc(s):
    return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace('"', "&quot;")


def thunk_target(name):
    """"_ZThn640_N9Scene..." (a this-adjusting thunk) -> "_ZN9Scene...", so the class reads like the target's."""
    m = re.match(r"_ZTh[nv]?\d+_(?:n?\d+_)?", name)
    return "_Z" + name[m.end():] if m else name


def game_owner(name, owner):
    """The block a game function goes into: its class (mangled name, or rewrite/tools/owners.py's guess for an
    unnamed one); master:: tables, the sound engine's C API, free functions and Java glue as one block each."""
    import unit
    cls = unit.demangle_class(thunk_target(name)) or owner or ""
    if cls.startswith("master::"):
        return "master (data tables)"
    if cls:
        return cls
    if name.startswith("SdSoundSystem"):
        return "SQEX"
    if name.startswith("Java_"):
        return "JNI entry points"
    if re.match(r"_Z\d", name):
        return "free functions"
    return "unattributed"


# the small classes go into one block per kind, so labels stay readable
SMALL = 50
KINDS = [("other scenes", r"^Scene"), ("other scroll views", r"(ScrollView|View)$"),
         ("other dialogs", r"(Dialog|^[Pp]op[Uu]p)"),
         ("other battle / stage", r"(Stage|Battle|Raid|Actor|Enemy|Boss|Medal|Deck)")]


def game_blocks(is_lib, rw_set, ok_set):
    """[(label, tooltip name, verified, rewritten-not-verified, total)] for the game code, largest first."""
    sys.path.insert(0, str(HERE / "rewrite" / "tools"))
    import unit
    idx = unit.load_index()
    owners_path = paths.OUT / "rewrite" / "owners.json"
    owners = json.loads(owners_path.read_text()) if owners_path.exists() else {}
    by_cls = {}
    for a, lib in is_lib.items():
        if lib:
            continue
        e = idx.get(f"{a:08X}") or idx.get(f"{a | 1:08X}") or {}
        cls = game_owner(e.get("name", ""), owners.get(f"{a:08X}"))
        s = by_cls.setdefault(cls, [0, 0, 0])
        s[0] += a in ok_set
        s[1] += a in rw_set and a not in ok_set
        s[2] += 1
    blocks, merged = [], {}
    for cls, (v, r, n) in by_cls.items():
        if n >= SMALL:
            blocks.append((cls, cls, v, r, n))
            continue
        kind = next((k for k, p in KINDS if re.search(p, cls)), "other classes")
        merged.setdefault(kind, []).append((cls, v, r, n))
    # a kind too small for a block of its own joins "other classes"
    for kind in sorted(merged):
        if kind != "other classes" and sum(m[3] for m in merged[kind]) < SMALL:
            merged.setdefault("other classes", []).extend(merged.pop(kind))
    for kind, members in sorted(merged.items()):
        v, r, n = (sum(m[i] for m in members) for i in (1, 2, 3))
        if len(members) == 1:
            blocks.append((members[0][0], members[0][0], v, r, n))
        else:
            blocks.append((kind, f"{kind} ({len(members)} classes under {SMALL} functions each)", v, r, n))
    return sorted(blocks, key=lambda b: (-b[4], b[0]))


def squarify(values, x, y, w, h):
    """Squarified treemap (Bruls et al.): rectangles for values (sorted largest first) filling x, y, w, h."""
    total = sum(values)
    areas = [v * w * h / total for v in values]
    rects, row = [], []

    def worst(row, side):
        s = sum(row)
        return max(max(side * side * a / s ** 2, s ** 2 / (side * side * a)) for a in row)

    def place(row, x, y, w, h):
        thick = sum(row) / min(w, h)
        off = 0
        for a in row:
            length = a / thick
            rects.append((x, y + off, thick, length) if w >= h else (x + off, y, length, thick))
            off += length
        return (x + thick, y, w - thick, h) if w >= h else (x, y + thick, w, h - thick)

    while areas:
        if not row or worst(row + [areas[0]], min(w, h)) <= worst(row, min(w, h)):
            row.append(areas.pop(0))
            continue
        x, y, w, h = place(row, x, y, w, h)
        row = []
    if row:
        place(row, x, y, w, h)
    return rects


def block_cells(v, r, n, x, y, w, h):
    """One cell per function on a cols x rows grid (row-major: verified, then rewritten, then not rewritten).
    Each state's run is drawn as at most three rectangles and the grid as one path, which keeps the file small."""
    cols = max(1, min(n, round((n * w / h) ** 0.5)))
    rows = -(-n // cols)
    cw, ch = w / cols, h / rows
    out = []

    def run(lo, hi, cls):
        while lo < hi:
            row, col = divmod(lo, cols)
            if col == 0 and hi - lo >= cols:   # whole rows at once
                k = (hi - lo) // cols
                out.append((cls, x, y + row * ch, w, k * ch))
                lo += k * cols
            else:
                end = min(hi, (row + 1) * cols)
                out.append((cls, x + col * cw, y + row * ch, (end - lo) * cw, ch))
                lo = end
    run(0, v, "v")
    run(v, v + r, "r")
    run(v + r, n, "t")
    parts = [f'<rect class="{c}" x="{rx:.1f}" y="{ry:.1f}" width="{rw:.1f}" height="{rh:.1f}"/>'
             for c, rx, ry, rw, rh in out if rw > 0 and rh > 0]
    if min(cw, ch) >= 3:   # cell borders only where cells are big enough to see
        d = "".join(f"M{x + c * cw:.1f} {y:.1f}v{h:.1f}" for c in range(1, cols))
        d += "".join(f"M{x:.1f} {y + k * ch:.1f}h{w:.1f}" for k in range(1, rows))
        if d:
            parts.append(f'<path class="grid" d="{d}" stroke-width="0.6" fill="none"/>')
    return parts


def lib_blocks(rw_set, ok_set):
    """[(label, tooltip name, verified, rewritten-not-verified, total)] for the open-source libraries, by library."""
    sys.path.insert(0, str(HERE / "rewrite" / "tools"))
    import libsplit
    by_lib = {}
    for a, lib in libsplit.groups().items():
        if lib is None:
            continue
        a &= ~1
        s = by_lib.setdefault(lib, [0, 0, 0])
        s[0] += a in ok_set
        s[1] += a in rw_set and a not in ok_set
        s[2] += 1
    return sorted(((k, k, v, r, n) for k, (v, r, n) in by_lib.items()), key=lambda b: (-b[4], b[0]))


def treemap(blocks, title="Game code", grouping="grouped by class", width=1000, height=640):
    head, foot = 46, 0
    v_all, r_all, n_all = (sum(b[i] for b in blocks) for i in (2, 3, 4))
    parts = [f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {width} {height + head + foot}" '
             f'width="{width}" height="{height + head + foot}" role="img" '
             f'aria-label="{esc(title)}: {v_all:,} of {n_all:,} functions verified">',
             SVG_STYLE, f'<rect class="bg" width="{width}" height="{height + head + foot}"/>',
             f'<text class="txt" x="4" y="19" font-size="16" font-weight="600">{esc(title)}: '
             f'{100 * v_all / n_all:.1f}% verified, {100 * (v_all + r_all) / n_all:.1f}% rewritten '
             f'({n_all:,} functions, one cell each, {grouping})</text>']
    lx = 4
    for cls, label, n in (("v", "verified", v_all), ("r", "rewritten, not verified yet", r_all),
                          ("t", "not rewritten", n_all - v_all - r_all)):
        text = f"{label} ({n:,})"
        parts.append(f'<rect class="{cls}" x="{lx}" y="29" width="11" height="11" rx="2"/>'
                     f'<text class="txt dim" x="{lx + 15}" y="39" font-size="12">{esc(text)}</text>')
        lx += 15 + 7 * len(text) + 18
    for (label, name, v, r, n), (x, y, w, h) in zip(blocks, squarify([b[4] for b in blocks], 0, head, width,
                                                                       height)):
        parts.append(f'<g><title>{esc(name)}: {v}/{n} verified ({100 * v / n:.0f}%), '
                     f'{v + r}/{n} rewritten</title>')
        parts += block_cells(v, r, n, x + 1, y + 1, w - 2, h - 2)
        parts.append(f'<rect class="edge" x="{x:.1f}" y="{y:.1f}" width="{w:.1f}" height="{h:.1f}" fill="none" '
                     f'stroke-width="2"/>')
        fit = int((w - 8) / 6.5)   # characters that fit at 11px
        if h > 18 and fit >= 3:
            short = label if len(label) <= fit else label[:fit - 1] + "…"
            parts.append(f'<text class="txt halo" x="{x + 4:.1f}" y="{y + 14:.1f}" font-size="11">{esc(short)}'
                         '</text>')
            if h > 32 and fit >= 4:
                parts.append(f'<text class="txt halo" x="{x + 4:.1f}" y="{y + 27:.1f}" font-size="10">'
                             f'{100 * v / n:.0f}%</text>')
        parts.append("</g>")
    parts.append("</svg>")
    return "\n".join(parts) + "\n"


def write_images(is_lib, rw_set, ok_set, dry):
    """The game and library treemaps into docs/progress/; returns the README lines that show them."""
    files = {"game-map.svg": treemap(game_blocks(is_lib, rw_set, ok_set)),
             "libraries-map.svg": treemap(lib_blocks(rw_set, ok_set), "Open-source libraries", "grouped by library")}
    if not dry:
        DOCS.mkdir(parents=True, exist_ok=True)
        for name, svg in files.items():
            (DOCS / name).write_bytes(svg.encode("utf-8"))   # bytes: no CRLF on Windows, so no diff
    for name, svg in files.items():
        print(f"docs/progress/{name}: {len(svg.encode('utf-8')):,} bytes" + (" (dry run, not written)" if dry else ""),
              file=sys.stderr)
    return ["![Game code by class: verified / rewritten / not rewritten](docs/progress/game-map.svg)", "",
            "![Open-source libraries by library: verified / rewritten / not rewritten](docs/progress/libraries-map.svg)",
            ""]


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
    groups.append(("Binary match", [("1:1 binary match", matching, lift_total)]))

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
                             "decompilation). Not started."),
    ]

    w = max(len(name) for _, rows in groups for name, _, _ in rows)
    out = ["## Progress", ""]
    if is_lib:
        out += write_images(is_lib, rw_set, ok_set, "--dry-run" in sys.argv)
    out += ["```"]
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
