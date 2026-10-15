"""Build the M3 rewrite (out/rewrite/rewrite.dll) with MSVC, and record the rewritten-function count.

python recomp/rewrite/build.py

Every .cpp under rewrite/src (and the runtime) is compiled; functions registered with RW_FUNCTION replace their
lifted versions when the native host loads rewrite.dll (KHUX_REWRITE=0 turns that off, KHUX_RW_VERIFY=1 checks
every call against the lifted code). progress.json "rewritten" = the number of RW_FUNCTION registrations.
"""
import json
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
RECOMP = HERE.parent
OUT = RECOMP / "out" / "rewrite"
VCVARS = Path(r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat")
CRLF = chr(13) + chr(10)
PROGRESS = RECOMP / "progress.json"


def sources():
    """runtime + src; KHUX_RW_EXCLUDE=scene,net leaves those src folders (or files, by name without .cpp) out (modules being written)."""
    import os
    skip = [x for x in os.environ.get("KHUX_RW_EXCLUDE", "").split(",") if x]
    srcs = [f for f in sorted((HERE / "src").rglob("*.cpp"))
            if not any(f.relative_to(HERE / "src").parts[0] == x or f.stem == x for x in skip)]
    return sorted((HERE / "runtime").glob("*.cpp")) + srcs


def count_rewritten():
    pat = re.compile(r"^\s*RW_FUNCTION(?:_NOVERIFY)?\(\s*(0x[0-9A-Fa-f]+)", re.M)
    addrs = set()
    for f in (HERE / "src").rglob("*.cpp"):
        addrs.update(int(a, 16) for a in pat.findall(f.read_text(encoding="utf-8")))
    return addrs


def check(files):
    """Compile the given sources only (no link): a quick check for one module, safe to run in parallel."""
    import tempfile
    tmp = Path(tempfile.mkdtemp(prefix="rwcheck_"))
    inc = f'/I"{HERE / "include"}" /I"{RECOMP / "lift"}" /I"{HERE / "src"}"'
    srcs = " ".join(f'"{Path(f).resolve()}"' for f in files)
    bat = tmp / "check.bat"
    bat.write_text(CRLF.join([
        "@echo off", f'call "{VCVARS}" >nul',
        f"cl /nologo /c /std:c++17 /O2 /W3 /MD /EHsc /bigobj /utf-8 /wd4996 /wd4244 /wd4267 /wd4146 "
        f"/D_CRT_SECURE_NO_WARNINGS {inc} {srcs}", ""]))
    r = subprocess.run(["cmd", "/c", str(bat)], cwd=str(tmp), capture_output=True, text=True)
    log = r.stdout + r.stderr
    for line in [l for l in log.splitlines() if " error " in l or "fatal error" in l or ": warning" in l][:80]:
        print(line)
    print(f"check: cl exit {r.returncode}")
    sys.exit(r.returncode)


def main():
    if len(sys.argv) > 2 and sys.argv[1] == "--check":
        check(sys.argv[2:])
    OUT.mkdir(parents=True, exist_ok=True)
    obj_dir = OUT / "obj"
    obj_dir.mkdir(exist_ok=True)
    # incremental: a source recompiles when it or any header of the rewrite / rt.h is newer than its object
    headers = list((HERE / "include").rglob("*.hpp")) + list((HERE / "src").rglob("*.hpp")) + [RECOMP / "lift" / "rt.h"]
    newest_header = max(h.stat().st_mtime for h in headers)
    objs, stale = [], []
    for src in sources():
        rel = src.relative_to(HERE).with_suffix("")
        obj = obj_dir / ("_".join(rel.parts) + ".obj")
        objs.append(obj)
        if not obj.exists() or obj.stat().st_mtime < max(src.stat().st_mtime, newest_header):
            stale.append((src, obj))
    for o in obj_dir.glob("*.obj"):                      # objects of deleted sources
        if o not in objs:
            o.unlink()
    inc = f'/I"{HERE / "include"}" /I"{RECOMP / "lift"}" /I"{HERE / "src"}"'
    flags = ("/nologo /std:c++17 /O2 /W3 /MD /Zi /EHsc /bigobj /utf-8 /wd4996 /wd4244 /wd4267 /wd4146 "
             f"/D_CRT_SECURE_NO_WARNINGS {inc}")
    lines = ["@echo off", f'call "{VCVARS}" >nul']
    # cl /MP names objects after their sources: each source is compiled through a one-line wrapper named after
    # its path (src/core/A.cpp -> src_core_A.cpp), so equal file names in different folders cannot collide
    wrap_dir = OUT / "wrap"
    wrap_dir.mkdir(exist_ok=True)
    wraps = []
    for src, obj in stale:
        w = wrap_dir / (obj.stem + ".cpp")
        w.write_text(f'#include "{src.as_posix()}"\n')
        wraps.append(w)
    if wraps:
        # the sources go in a response file: a full rebuild's list is far over cmd's 8191-character line limit
        cl_rsp = OUT / "sources.rsp"
        cl_rsp.write_text("".join(f'"{w}"\n' for w in wraps))
        lines.append(f'cl /c /MP {flags} /Fo"{obj_dir.as_posix()}/" /Fd"{OUT / "rewrite_c.pdb"}" @"{cl_rsp}"')
        lines.append("if errorlevel 1 exit /b 1")
    rsp = OUT / "objs.rsp"
    rsp.write_text("\n".join(f'"{o}"' for o in objs) + "\n")
    lines.append(f'link /NOLOGO /DLL /DEBUG /MAP /OUT:rewrite.dll @"{rsp}"')
    bat = OUT / "build.bat"
    bat.write_text(CRLF.join(lines + [""]))
    print(f"compiling {len(stale)} of {len(objs)} files")
    r = subprocess.run(["cmd", "/c", str(bat)], cwd=str(OUT), capture_output=True, text=True)
    log = r.stdout + r.stderr
    (OUT / "build.log").write_text(log)
    for line in [l for l in log.splitlines() if " error " in l or "fatal error" in l or ": warning" in l
                 or "LNK" in l][:80]:
        print(line)
    addrs = count_rewritten()
    prog = json.loads(PROGRESS.read_text()) if PROGRESS.exists() else {}
    prog["rewritten"] = len(addrs)
    PROGRESS.write_text(json.dumps(prog, indent=1) + "\n")
    print(f"cl exit {r.returncode}: {OUT / 'rewrite.dll'} ({len(addrs)} functions rewritten)")
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
