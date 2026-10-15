"""List the imports (PLT stubs) the rewrite calls that neither lift.dll nor the native host implements.

python recomp/rewrite/tools/check_imports.py

Rewritten code calls library functions through their PLT stubs (rw::call(0x40xxxx...)). The lifted code reaches
many imports only through lift.dll's native fast paths, so the host may lack an implementation the rewrite now
needs: such a call ends the game with "unimplemented import". This scans rewrite/src for call targets that are
PLT stubs and checks each import name against lift/build.py NATIVE_IMPORTS and the host's imp("...") table.
"""
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent.parent
RECOMP = HERE.parent


def main():
    a = json.loads((RECOMP / "out" / "analysis.json").read_text())
    plt = {}
    for k, v in (a["plt"].items() if isinstance(a["plt"], dict) else enumerate(a["plt"])):
        if isinstance(v, dict):
            addr = int(str(v.get("addr", k)), 16) if isinstance(v.get("addr", k), str) else int(v.get("addr", k))
            plt[addr & ~1] = v.get("name") or v.get("symbol")
        else:
            plt[int(str(k), 16) & ~1] = v
    native = set(re.findall(r'"([A-Za-z_][\w]*)"', (RECOMP / "lift" / "build.py").read_text()))
    host = set()
    for f in (RECOMP / "host").glob("*.c"):
        host.update(re.findall(r'imp\("([^"]+)"', f.read_text(encoding="utf-8", errors="replace")))
    used = {}
    for f in (HERE / "src").rglob("*.[ch]pp"):
        for m in re.finditer(r"0x([0-9A-Fa-f]{6,8})", f.read_text(encoding="utf-8")):
            t = int(m.group(1), 16) & ~1
            if t in plt:
                used.setdefault(plt[t], set()).add(f.relative_to(HERE).as_posix())
    missing = {n: fs for n, fs in used.items() if n and n not in native and n not in host}
    print(f"{len(used)} imports used by the rewrite, {len(missing)} not implemented")
    for n, fs in sorted(missing.items()):
        print(f"  {n:28} {', '.join(sorted(fs))[:120]}")
    sys.exit(1 if missing else 0)


if __name__ == "__main__":
    main()
