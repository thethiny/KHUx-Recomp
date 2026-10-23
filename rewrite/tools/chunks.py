"""IDA chunks: index entries whose code is part of another function's body, not functions of their own.

python recomp/rewrite/tools/chunks.py      writes rewrite/tasks/chunks.tsv (chunk, parents) and prints a summary

IDA splits some functions (shared epilogues, outlined tails, alternate entries) into pieces with their own entry;
the lifter emits each piece both on its own and inside the function that runs into it (the parent's F_ body holds
the piece's L_ label). A piece is listed when some other lifted function contains its label, nothing calls it
(no rt_call to it in the lifted code), no word of the .so points at it (no vtable / function pointer) and no
hand-written rewrite calls it (a parent's tail call written as a call makes the piece a function after all).
progress.py counts a listed piece as rewritten / run / verified when one of its parents is: the parent's rewrite
runs the piece's code as part of its own body.
"""
import glob
import re
import struct
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REWRITE = HERE.parent
RECOMP = REWRITE.parent
sys.path.insert(0, str(RECOMP))
import paths  # noqa: E402

OUT = REWRITE / "tasks" / "chunks.tsv"


def main():
    fre = re.compile(r"^void F_([0-9A-F]+)\(CPU")
    lre = re.compile(r"^L_([0-9A-F]+):")
    cre = re.compile(r"rt_call\(c, 0x([0-9A-F]+)u\)")
    entries, labels, called, called_from = set(), {}, set(), {}
    files = sorted(glob.glob(str(RECOMP / "out" / "lift_all" / "lift_*.c")))
    for f in files:
        cur = None
        for line in open(f, encoding="utf-8", errors="replace"):
            m = fre.match(line)
            if m:
                cur = int(m.group(1), 16)
                entries.add(cur)
                continue
            if line.startswith("L_"):
                m = lre.match(line)
                if m and cur is not None:
                    labels.setdefault(int(m.group(1), 16), set()).add(cur)
            for m in cre.finditer(line):
                called_from.setdefault(int(m.group(1), 16) & ~1, set()).add(cur)
    data = Path(paths.so()).read_bytes()
    words = set(struct.unpack_from("<%dI" % (len(data) // 4), data))
    hexre = re.compile(r"0x([0-9A-Fa-f]{6,8})")
    for f in (REWRITE / "src").rglob("*.cpp"):
        called.update(int(h, 16) & ~1 for h in hexre.findall(f.read_text(encoding="utf-8", errors="replace")))
    # a function pointer to Thumb code has bit 0 set: for a Thumb piece only that word counts (the even value is
    # often just data that happens to match)
    import json
    thumb = {int(fn["addr"], 16) for fn in json.load(open(RECOMP / "out" / "analysis.json"))["functions"]
             if fn.get("mode") == "thumb"}
    rows = []
    for a in sorted(entries):
        parents = labels.get(a, set()) - {a}
        # a piece called (bl) only from inside its own parents is still part of them: IDA's split epilogues and
        # tails, which run in the parent's frame and return for it
        foreign = called_from.get(a, set()) - parents - {a}
        if not parents or foreign or a in called or (a | 1) in words or (a not in thumb and a in words):
            continue
        rows.append((a, sorted(parents)))
    OUT.write_text("# chunk\tparents (rewrite/tools/chunks.py)\n" +
                   "".join(f"{a:08X}\t{' '.join(f'{p:08X}' for p in ps)}\n" for a, ps in rows), encoding="utf-8")
    print(f"{len(rows)} chunks -> {OUT}")


if __name__ == "__main__":
    main()
