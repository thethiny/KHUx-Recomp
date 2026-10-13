"""Build the native host (out/host/khux.exe) with MSVC.

python recomp/host/build.py
The recompiled game itself is lift.dll (python recomp/lift/build.py --all --out out/lift_all).
"""
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
RECOMP = HERE.parent
OUT = RECOMP / "out" / "host"
VCVARS = Path(r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat")
CRLF = chr(13) + chr(10)
SOURCES = ["main.c", "core.c", "mem.c", "thread.c", "libc.c", "fs.c", "net.c", "jni.c", "gl.c", "sl.c", "ctl.c", "ui.c", "webview.c"]
LIBS = ["ws2_32.lib", "winmm.lib", "user32.lib", "gdi32.lib", "kernel32.lib", "ole32.lib", "advapi32.lib",
        "shell32.lib", "version.lib", r"third_party\webview2\WebView2LoaderStatic.lib"]
WV2_DIR = HERE / "third_party" / "webview2"
WV2_URL = "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2"   # Microsoft WebView2 SDK (NuGet)


def fetch_webview2():
    """WebView2.h and the static loader from Microsoft's NuGet package (not kept in the repository)."""
    if (WV2_DIR / "WebView2.h").exists() and (WV2_DIR / "WebView2LoaderStatic.lib").exists():
        return
    import io
    import urllib.request
    import zipfile
    print("fetching the WebView2 SDK from nuget.org")
    z = zipfile.ZipFile(io.BytesIO(urllib.request.urlopen(WV2_URL).read()))
    WV2_DIR.mkdir(parents=True, exist_ok=True)
    for src, dst in (("build/native/include/WebView2.h", "WebView2.h"),
                     ("build/native/x64/WebView2LoaderStatic.lib", "WebView2LoaderStatic.lib"),
                     ("LICENSE.txt", "LICENSE.txt")):
        if src in z.namelist():
            (WV2_DIR / dst).write_bytes(z.read(src))


def main():
    fetch_webview2()
    OUT.mkdir(parents=True, exist_ok=True)
    srcs = " ".join(f'"{HERE / s}"' for s in SOURCES)
    bat = OUT / "build.bat"
    bat.write_text(CRLF.join([
        "@echo off",
        f'call "{VCVARS}" >nul',
        f'cl /nologo /O2 /W3 /MD /Zi /wd4996 /wd4244 /wd4267 /D_CRT_SECURE_NO_WARNINGS {srcs} /Fe:khux.exe '
        f'/link /NOLOGO /DEBUG /MAP /STACK:134217728 {" ".join(chr(34) + str(HERE / l) + chr(34) if l.startswith("third_party") else l for l in LIBS)}',
        ""]))
    r = subprocess.run(["cmd", "/c", str(bat)], cwd=str(OUT), capture_output=True, text=True)
    log = r.stdout + r.stderr
    (OUT / "build.log").write_text(log)
    errs = [l for l in log.splitlines() if " error " in l or "fatal error" in l or ": warning" in l]
    for l in errs[:60]:
        print(l)
    print(f"cl exit {r.returncode}: {OUT / 'khux.exe'}")
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
