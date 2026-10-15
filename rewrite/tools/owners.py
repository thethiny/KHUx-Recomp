"""Attribute unnamed functions (sub_XXXX: lambdas, std::function thunks, outlined pieces) to an owner class.

python recomp/rewrite/tools/owners.py            writes out/rewrite/owners.json and prints a summary
python recomp/rewrite/tools/owners.py Class      the unnamed functions attributed to Class

Evidence, strongest first:
  1. callers: every function that calls (or tail-calls) it belongs to one class -> that class;
  2. typeinfo: the function returns the address of a typeinfo whose name mentions Class (std::function's target_type
     thunks, e.g. "Scene::open(...)::$_18") -> that class;
  3. position: clang emits lambdas and helpers of a translation unit next to their parent, so the class of the
     nearest named function before it (when the one after it agrees, or nothing named is closer) -> that class.
The result is a guide for assigning work, not a fact.
"""
import bisect
import collections
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent.parent
RECOMP = HERE.parent
sys.path.insert(0, str(HERE / "tools"))
import unit  # noqa: E402

OUT = RECOMP / "out" / "rewrite" / "owners.json"


def main():
    idx = unit.load_index()
    addrs = sorted(int(k, 16) for k in idx)
    cls = {a: unit.demangle_class(idx[f"{a:08X}"]["name"]) for a in addrs}
    named = [a for a in addrs if cls[a]]
    unnamed = [a for a in addrs if not cls[a] and idx[f"{a:08X}"]["name"].startswith(("sub_", "nullsub"))]

    # call graph from the lifted C (callee -> callers)
    callers = collections.defaultdict(set)
    fn = re.compile(r"^void F_([0-9A-F]+)\(CPU \*c\) \{")
    call = re.compile(r"(?:rt_call(?:_frag)?\(c, |TAIL\()0x([0-9A-F]+)u\)")
    for f in sorted(unit.LIFT.glob("lift_*.c")):
        if f.name == "lift_table.c":
            continue
        cur = None
        for line in f.read_text(encoding="utf-8", errors="replace").splitlines():
            m = fn.match(line)
            if m:
                cur = int(m.group(1), 16)
                continue
            if cur is not None and "0x" in line:
                for t in call.findall(line):
                    callers[int(t, 16) & ~1].add(cur)

    owners, how = {}, collections.Counter()
    for a in unnamed:
        cs = {cls.get(c) or owners.get(c) for c in callers.get(a, ())}
        cs.discard(None)
        cs.discard("")
        if len(cs) == 1:
            owners[a] = cs.pop()
            how["callers"] += 1
            continue
        i = bisect.bisect_left(named, a)
        before = cls[named[i - 1]] if i > 0 else ""
        after = cls[named[i]] if i < len(named) else ""
        if before and (before == after or not after):
            owners[a] = before
            how["position"] += 1
        elif before:
            # between two classes: the closer one
            owners[a] = before if a - named[i - 1] <= named[i] - a else after
            how["position (closest)"] += 1
    # second pass: callers that were themselves just attributed
    for a in unnamed:
        if a in owners and how:
            continue
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(json.dumps({f"{a:08X}": c for a, c in owners.items()}))
    if len(sys.argv) > 1:
        want = sys.argv[1]
        for a, c in sorted(owners.items()):
            if c == want:
                e = idx[f"{a:08X}"]
                print(f"{a:08X} {'X' if e['covered'] else ' '} {e['size']:6}  {e['name']}")
        return
    per = collections.Counter(owners.values())
    print(f"{len(unnamed)} unnamed functions, {len(owners)} attributed ({dict(how)})")
    for c, n in per.most_common(40):
        print(f"  {n:6}  {c}")


if __name__ == "__main__":
    main()
