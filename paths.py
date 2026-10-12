"""Where recomp finds its inputs and writes its outputs.

Inputs (not part of this repository) live in one folder that holds the v1.0.1 build:
    libcocos2dcpp.so                         the armeabi native library
    khux_1.0.1_shortmovie_aligned.apk        the APK (assets, Java side)
    libcocos2dcpp_decompile/index.txt        function index from the IDA export (optional, tracing only)

Set KHUX_INPUT to that folder. Without it, the folder is looked up relative to this repository at
../Input/1.0.1 (the layout used when recomp is checked out as a submodule). If neither exists, every
lookup fails with an error saying what is missing - nothing silently falls back.

Outputs stay inside this repository: fs/ (the emulated device filesystem), out/ (analysis), logs.
"""
import os
from pathlib import Path

RECOMP = Path(__file__).resolve().parent
_RELATIVE_INPUT = RECOMP.parent / "Input" / "1.0.1"

FS_ROOT = RECOMP / "fs"
OUT = RECOMP / "out"
EVENTS_FILE = RECOMP / "emu_events.jsonl"
SERVER_LOG = RECOMP / "server_emu.log"


def input_dir() -> Path:
    env = os.environ.get("KHUX_INPUT", "").strip()
    if env:
        p = Path(env).expanduser().resolve()
        if not p.is_dir():
            raise FileNotFoundError(f"KHUX_INPUT={env!r} is not a folder")
        return p
    if _RELATIVE_INPUT.is_dir():
        return _RELATIVE_INPUT
    raise FileNotFoundError(
        f"v1.0.1 input folder not found: set KHUX_INPUT, or provide {_RELATIVE_INPUT} "
        "(it must contain libcocos2dcpp.so and khux_1.0.1_shortmovie_aligned.apk)")


def require(name: str) -> Path:
    """A file or folder inside the input folder; raises if it does not exist."""
    p = input_dir() / name
    if not p.exists():
        raise FileNotFoundError(f"missing input: {p}")
    return p


def so() -> Path:
    return require("libcocos2dcpp.so")


def apk() -> Path:
    return require("khux_1.0.1_shortmovie_aligned.apk")


def decompile_dir() -> Path:
    return require("libcocos2dcpp_decompile")


def decompile_index() -> Path:
    return require("libcocos2dcpp_decompile/index.txt")
