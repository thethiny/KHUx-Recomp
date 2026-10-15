"""Everything needed to rewrite a guest function (or a whole class): the work unit for one rewrite task.

python recomp/rewrite/tools/unit.py 0x4EEB88            one function
python recomp/rewrite/tools/unit.py StageState          every function of a class (list, then each unit)
python recomp/rewrite/tools/unit.py StageState --list   the class's functions only
python recomp/rewrite/tools/unit.py --classes [N]       classes by function count (game code first)

Per function: address, mode, size, executed in game (coverage), already rewritten, the demangled signature and
Hex-Rays pseudocode (reference: it can be wrong about types and stack layouts), the lifted C (exact semantics:
this is what the verifier compares against), and the guest functions it calls, named, with their rewrite status.
An index is cached in out/rewrite/index.json (rebuilt when analysis.json or the lift output changes).
"""
import bisect
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent.parent
RECOMP = HERE.parent
sys.path.insert(0, str(RECOMP))
import paths  # noqa: E402

OUT = RECOMP / "out"
LIFT = OUT / "lift_all"
INDEX = OUT / "rewrite" / "index.json"
DECOMP = paths.decompile_index().parent / "decompile"
NAMES = paths.decompile_index().parent / "names.txt"


def build_index():
    funcs = json.loads((OUT / "analysis.json").read_text())["functions"]
    names = {}
    for line in NAMES.read_text(encoding="utf-8", errors="replace").splitlines():
        p = line.split()
        if len(p) >= 2 and p[0].startswith("0x") and not p[1].startswith("j_"):
            names.setdefault(int(p[0], 16), p[1])
    decomp = {}
    head = re.compile(r"^// ==== (\S+) \(ea=0x([0-9A-Fa-f]+)\) ====")
    for f in sorted(DECOMP.glob("libcocos2dcpp_*.c")):
        for k, line in enumerate(f.read_text(encoding="utf-8", errors="replace").splitlines()):
            m = head.match(line)
            if m:
                decomp[int(m.group(2), 16)] = [f.name, k + 1, m.group(1)]
    lifted = {}
    fn = re.compile(r"^void F_([0-9A-F]+)\(CPU \*c\) \{")
    for f in sorted(LIFT.glob("lift_*.c")):
        if f.name == "lift_table.c":
            continue
        for k, line in enumerate(f.read_text(encoding="utf-8", errors="replace").splitlines()):
            m = fn.match(line)
            if m:
                lifted[int(m.group(1), 16)] = [f.name, k + 1]
    cov = set()
    covf = OUT / "coverage.json"
    if covf.exists():
        blocks = sorted(json.loads(covf.read_text())["blocks"])
        starts = sorted(int(f["addr"], 16) for f in funcs)
        for b in blocks:
            i = bisect.bisect_right(starts, b & ~1) - 1
            if i >= 0:
                cov.add(starts[i])
    idx = {}
    for f in funcs:
        a = int(f["addr"], 16)
        d = decomp.get(a)
        idx[f"{a:08X}"] = {
            "mode": f["mode"], "size": int(f["end"], 16) - a,
            "name": names.get(a) or (d[2] if d else f.get("name") or f"sub_{a:X}"),
            "decomp": d[:2] if d else None, "lifted": lifted.get(a), "covered": a in cov,
        }
    INDEX.parent.mkdir(parents=True, exist_ok=True)
    INDEX.write_text(json.dumps(idx))
    return idx


def load_index():
    src = [OUT / "analysis.json", LIFT / "lift_table.c"]
    if INDEX.exists() and all(INDEX.stat().st_mtime >= s.stat().st_mtime for s in src if s.exists()):
        return json.loads(INDEX.read_text())
    return build_index()


def demangle_class(name):
    """Class of a mangled member function (_ZN<len>Class...), or ''."""
    m = re.match(r"_ZNK?(\d+)([A-Za-z_]\w*)", name)
    if not m:
        return ""
    n = int(m.group(1))
    cls = m.group(2)[:n]
    rest = m.group(2)[n:]
    if cls in ("cocos2d", "cocostudio", "hole", "network", "api", "master", "rapidjson", "LWF", "tinyxml2"):
        m2 = re.match(r"(\d+)(\w+)", rest)
        if m2:
            k = int(m2.group(1))
            return cls + "::" + m2.group(2)[:k]
    return cls


def rewritten():
    pat = re.compile(r"^\s*RW_FUNCTION(?:_NOVERIFY)?\(\s*0x([0-9A-Fa-f]+)", re.M)
    out = set()
    for f in (HERE / "src").rglob("*.cpp"):
        out.update(int(a, 16) for a in pat.findall(f.read_text(encoding="utf-8")))
    return out


def text_block(path, line, stop):
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    out = []
    for k in range(line - 1, len(lines)):
        if k > line - 1 and stop(lines[k]):
            break
        out.append(lines[k])
    return out


def show(addr, idx, done):
    key = f"{addr:08X}"
    e = idx.get(key)
    if not e:
        print(f"!! {key}: not an analysed function")
        return
    print("=" * 110)
    print(f"{key}  {e['name']}  ({e['mode']}, {e['size']} bytes, executed in game: {'yes' if e['covered'] else 'no'}, "
          f"rewritten: {'yes' if addr in done else 'no'})")
    if e["decomp"]:
        print("--- Hex-Rays pseudocode (reference only)")
        blk = text_block(DECOMP / e["decomp"][0], e["decomp"][1], lambda l: l.startswith("// ==== "))
        print("\n".join(blk).rstrip())
    if e["lifted"]:
        blk = text_block(LIFT / e["lifted"][0], e["lifted"][1], lambda l: l.startswith("void F_"))
        calls = sorted({int(x, 16) for x in re.findall(r"rt_call(?:_frag)?\(c, 0x([0-9A-F]+)u\)", "\n".join(blk))}
                       | {int(x, 16) for x in re.findall(r"TAIL\(0x([0-9A-F]+)u\)", "\n".join(blk))})
        if calls:
            print("--- calls")
            for t in calls:
                ce = idx.get(f"{t & ~1:08X}")
                nm = ce["name"] if ce else "(import / PLT)"
                print(f"    {t:08X}  {nm}{'  [rewritten]' if (t & ~1) in done else ''}")
        print(f"--- lifted C ({len(blk)} lines; exact semantics)")
        print("\n".join(blk).rstrip())


def main():
    a = sys.argv[1:]
    idx = load_index()
    done = rewritten()
    if not a:
        sys.exit(__doc__)
    if a[0] == "--classes":
        n = int(a[1]) if len(a) > 1 else 60
        cnt, cov, rw = {}, {}, {}
        for k, e in idx.items():
            c = demangle_class(e["name"])
            if not c:
                continue
            cnt[c] = cnt.get(c, 0) + 1
            cov[c] = cov.get(c, 0) + int(e["covered"])
            rw[c] = rw.get(c, 0) + int(int(k, 16) in done)
        for c, v in sorted(cnt.items(), key=lambda kv: -kv[1])[:n]:
            print(f"{v:6} functions  {cov[c]:5} executed  {rw[c]:5} rewritten  {c}")
        return
    if re.fullmatch(r"(0x)?[0-9A-Fa-f]{5,8}", a[0]):
        show(int(a[0], 16), idx, done)
        return
    cls = a[0]
    members = sorted((int(k, 16), e) for k, e in idx.items() if demangle_class(e["name"]) == cls)
    for addr, e in members:
        print(f"{addr:08X} {'X' if e['covered'] else ' '} {'R' if addr in done else ' '} {e['size']:6}  {e['name']}")
    print(f"{len(members)} functions (X = executed in game, R = rewritten)")
    if "--list" in a:
        return
    for addr, e in members:
        show(addr, idx, done)


if __name__ == "__main__":
    main()
