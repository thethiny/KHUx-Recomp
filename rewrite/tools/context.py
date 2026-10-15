"""Where things are, for a rewrite task: one command instead of searching the tree (keeps agent runs cheap).

python recomp/rewrite/tools/context.py Class [Class ...]

Per class: the functions still to rewrite (X = executed in game - out/host/seen.txt from KHUX_COVERAGE=1 sessions,
or the emulator's coverage: do these first; size in bytes), where its layout
is already declared and which files already register its functions. Then, for everything those functions call:
the classes, each with the header that declares its layout (include it, do not redeclare) and how many of the
called functions are already rewritten. Last, the shared helper headers and the helpers they offer.
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import unit  # noqa: E402

SRC = unit.HERE / "src"
HELPERS = [
    ("include/rw/rw.hpp", "runtime: Field/PtrField, load/save/get/set, call/call_exact/call64/vcall, fbits/bitsf"),
    ("src/core/Guest.hpp", "guest::Frame (stack scratch, original frame sizes), Vector/String/SharedPtr layouts, "
                           "operator new/delete, static guards"),
    ("src/scene/SceneCommon.hpp", "scene helpers: Frame, Vec2/Size, director(), visibleSize(), strings, functions"),
    ("src/scene_main/MainCommon.hpp", "ld32/st32/ldf/stf, fbits/bitsf, f2i, vcall(obj, byte offset), widgets"),
    ("src/core/SafeBuffer.hpp", "SafeBuffer (encrypted fields: read/write through it)"),
]


def headers_declaring(cls, hpps):
    short = cls.split("::")[-1]
    pat = re.compile(r"^\s*(?:struct|class)\s+" + re.escape(short) + r"\b[^;]*$", re.M)
    return [h for h, text in hpps.items() if pat.search(text)]


def main():
    classes = sys.argv[1:]
    if not classes:
        sys.exit(__doc__)
    idx = unit.load_index()
    done = {a & ~1 for a in unit.rewritten()}
    # executed in game: the native host's coverage (KHUX_COVERAGE=1 -> out/host/seen.txt), else the old emulator's
    seen_file = unit.RECOMP / "out" / "host" / "seen.txt"
    if seen_file.exists():
        seen = {int(l, 16) for l in seen_file.read_text().split() if l.strip()}
        for k, e in idx.items():
            e["covered"] = e["covered"] or int(k, 16) in seen
    hpps = {p.relative_to(unit.HERE).as_posix(): p.read_text(encoding="utf-8", errors="replace")
            for p in SRC.rglob("*.hpp")}
    cpps = {p.relative_to(unit.HERE).as_posix(): p.read_text(encoding="utf-8", errors="replace")
            for p in SRC.rglob("*.cpp")}
    by_class = defaultdict(list)
    for k, e in idx.items():
        c = unit.demangle_class(e["name"])
        if c in classes or c.split("::")[-1] in classes:
            by_class[c.split("::")[-1]].append((int(k, 16), e))

    callees = defaultdict(set)
    for cls in classes:
        fns = sorted(by_class.get(cls, []), key=lambda t: (not t[1]["covered"], t[0]))
        todo = [(a, e) for a, e in fns if (a & ~1) not in done]
        print(f"=== {cls}: {len(todo)} to do of {len(fns)}")
        for a, e in todo:
            print(f"  {a:08X} {'X' if e['covered'] else ' '} {e['size']:5}  {e['name'][:110]}")
            if e["lifted"]:
                blk = "\n".join(unit.text_block(unit.LIFT / e["lifted"][0], e["lifted"][1],
                                                lambda l: l.startswith("void F_")))
                for t in re.findall(r"rt_call(?:_frag)?\(c, 0x([0-9A-F]+)u\)|TAIL\(0x([0-9A-F]+)u\)", blk):
                    t = int(t[0] or t[1], 16) & ~1
                    ce = idx.get(f"{t:08X}")
                    if ce:
                        callees[unit.demangle_class(ce["name"]) or "(free functions / sub_)"].add(t)
        decl = headers_declaring(cls, hpps)
        print(f"  layout declared in: {', '.join(decl) or '(nowhere yet)'}")
        reg = sorted(p for p, text in cpps.items() if re.search(r'RW_FUNCTION\(0x[0-9A-Fa-f]+,\s*"' + re.escape(cls) + "::", text))
        print(f"  functions already registered in: {', '.join(reg) or '(none)'}")
        print()

    print("=== called classes (header to include; rewritten / called)")
    for c, ts in sorted(callees.items(), key=lambda kv: -len(kv[1])):
        if c in classes:
            continue
        n_done = sum(1 for t in ts if t in done)
        decl = headers_declaring(c, hpps) if not c.startswith("(") else []
        print(f"  {c[:40]:40} {n_done:3}/{len(ts):<3} {', '.join(decl[:2]) or '-'}")
    print()
    print("=== shared helpers")
    for path, what in HELPERS:
        p = unit.HERE / path
        if not p.exists():
            continue
        names = sorted(set(re.findall(r"^\s*(?:inline|static inline|constexpr)?\s*[\w:<>\*& ]+?\s(\w+)\s*\(",
                                      p.read_text(encoding="utf-8", errors="replace"), re.M)) - {"if", "for", "while", "return", "switch"})
        print(f"  {path}: {what}")
        if names:
            print(f"      {', '.join(names[:40])}")


if __name__ == "__main__":
    main()
