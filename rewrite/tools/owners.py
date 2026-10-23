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


def first_name(mangled, start):
    """The first <length><identifier> at `start` (skipping an N nesting prefix and qualifiers)."""
    i = start
    while i < len(mangled) and mangled[i] in "NKVZ":
        i += 1
    m = re.match(r"(\d+)", mangled[i:])
    if not m:
        return ""
    n = int(m.group(1))
    i += len(m.group(1))
    return mangled[i:i + n]


def vtable_owners():
    """Functions stored in a vtable -> the class named by the vtable's typeinfo. For a libc++ std::function
    __func<lambda> the typeinfo names the lambda's enclosing function ("...__funcIZN13SceneDeckEdit22run...E4$_38...")
    and that function's class is taken; for a class vtable ("13SceneDeckEdit") the class itself."""
    sys.path.insert(0, str(RECOMP))
    import struct
    import paths  # noqa: E402
    from elftools.elf.elffile import ELFFile
    so = paths.so()
    data = open(so, "rb").read()
    with open(so, "rb") as fh:
        segs = [(s["p_vaddr"], s["p_filesz"], s["p_offset"]) for s in ELFFile(fh).iter_segments()
                if s["p_type"] == "PT_LOAD"]

    def word(va):
        for v, sz, off in segs:
            if v <= va < v + sz - 3:
                return struct.unpack_from("<I", data, off + va - v)[0]
        return None

    def cstr(va):
        for v, sz, off in segs:
            if v <= va < v + sz:
                o = off + va - v
                return data[o:data.index(b"\0", o)].decode("ascii", "replace")
        return ""

    text_end = min(v for v, _, _ in segs if v >= 0xF00000)
    out = {}
    for v, sz, off in segs:
        if v < 0xF00000:
            continue
        i = 0
        while i < sz - 8:
            # a vtable: 0 (offset to top), a typeinfo in the data segment, then code pointers
            top = struct.unpack_from("<I", data, off + i)[0]
            ti = struct.unpack_from("<I", data, off + i + 4)[0]
            first = struct.unpack_from("<I", data, off + i + 8)[0] if i + 12 <= sz else 0
            if top == 0 and v <= ti < v + sz and 0x400000 <= first < text_end:
                name = cstr(word(ti + 4) or 0) if word(ti + 4) else ""
                owner = ""
                if "__funcIZ" in name:
                    owner = first_name(name, name.index("__funcIZ") + len("__funcIZ"))
                elif name and (name[0].isdigit() or name[0] == "N"):
                    owner = first_name(name, 0)
                k = i + 8
                while k < sz - 3:
                    w = struct.unpack_from("<I", data, off + k)[0]
                    if not (0x400000 <= w < text_end):
                        break
                    if owner:
                        out.setdefault(w & ~1, owner)
                    k += 4
                i = k
                continue
            i += 4
    return out


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
    vt_owner = vtable_owners()
    for a in unnamed:
        if a in vt_owner:
            owners[a] = vt_owner[a]
            how["vtable"] += 1
            continue
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
