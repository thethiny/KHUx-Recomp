"""Install the game files the native host needs from your own copy of the KHUx v1.0.1 APK.

python recomp/host/install.py <KHUx v1.0.1 APK> [--to recomp/game]

Extracts lib/armeabi/libcocos2dcpp.so and assets/ into the game folder. The recompiled code is tied to one exact
build of the library, so its MD5 is checked; any other build is refused.
"""
import argparse
import hashlib
import sys
import zipfile
from pathlib import Path

LIB = "lib/armeabi/libcocos2dcpp.so"
LIB_MD5 = "df23475901cd0f0ba6ac587d2315d84d"     # KHUx v1.0.1 (the build lift.dll was recompiled from)
DEFAULT_TO = Path(__file__).resolve().parent.parent / "game"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("apk")
    ap.add_argument("--to", default=str(DEFAULT_TO))
    a = ap.parse_args()
    apk, to = Path(a.apk), Path(a.to)
    if not apk.is_file():
        sys.exit(f"{apk}: no such file")
    z = zipfile.ZipFile(apk)
    try:
        lib = z.read(LIB)
    except KeyError:
        sys.exit(f"{apk} has no {LIB}: not a KHUx v1.0.1 APK")
    md5 = hashlib.md5(lib).hexdigest()
    if md5 != LIB_MD5:
        sys.exit(f"{LIB} has MD5 {md5}, expected {LIB_MD5} (KHUx v1.0.1)")
    to.mkdir(parents=True, exist_ok=True)
    (to / "libcocos2dcpp.so").write_bytes(lib)
    n = 0
    for name in z.namelist():
        if name.startswith("assets/") and not name.endswith("/"):
            dest = to / name
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_bytes(z.read(name))
            n += 1
    print(f"installed into {to}: libcocos2dcpp.so and {n} assets")


if __name__ == "__main__":
    main()
