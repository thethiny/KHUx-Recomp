"""DEV/TEST HARNESS ONLY: drive game code directly through the native exe's control channel (khux.exe started
with KHUX_DEV=1), without walking the UI.

  python -m recomp.emu.dev scene SceneRanking            # SceneManager::changeSceneLayer(createSceneLayer<T>)
  python -m recomp.emu.dev scenes                        # the scene names createSceneLayer exists for
  python -m recomp.emu.dev call NAME|0xADDR [arg ...]    # a direct call (args as the ctl "call" endpoint takes)
  python -m recomp.emu.dev only NAME$ PREFIX ...         # install only these rewrites now (KHUX_RW_ONLY live)
  python -m recomp.emu.dev only @file                    # ... from a file, one name (prefix) per line
  python -m recomp.emu.dev only                          # all rewrites again

A scene jump is the bare scene change: scenes whose menu first loads server data (party, ranking, raid list, ...)
read it at once and crash without it (ScenePartyTop::runInJoinLayout) - enter those through their menu button.

NAME is a rewritten function's name as registered (RW_FUNCTION, e.g. "SceneRanking::initRankingTitle()"), or an
index name (mangled / sub_XXXXXX). Calls run on the main thread and, with KHUX_RW_VERIFY=1, go through the
verifier like any call. They act on the live game: save / restore the account around a session.
"""
import json
import re
import sys
from pathlib import Path

from recomp.emu.ctl import get

RECOMP = Path(__file__).resolve().parent.parent
SM_GET_INSTANCE = 0x60C26C          # SceneManager::getInstance
SM_CHANGE_SCENE_LAYER = 0x612B98    # SceneManager::changeSceneLayer(std::function<Layer*()>, bool, bool, int)
VT_LAYER_FACTORY = 0xF7E130         # __func<Layer*(*)(), ...> vtable + 8: a std::function over a function pointer

_names = None


def _index():
    sys.path.insert(0, str(RECOMP / "rewrite" / "tools"))
    import unit
    return unit.load_index()


def names():
    """{name: (address, thumb)} from the rewrite's registrations and the function index."""
    global _names
    if _names is None:
        idx = _index()
        _names = {e["name"]: (int(a, 16), e["mode"] == "thumb") for a, e in idx.items()}
        pat = re.compile(r'RW_FUNCTION(?:_NOVERIFY)?\(\s*(0x[0-9A-Fa-f]+),\s*"([^"]+)"')
        for f in (RECOMP / "rewrite" / "src").rglob("*.cpp"):
            for a, n in pat.findall(f.read_text(encoding="utf-8", errors="replace")):
                e = idx.get(f"{int(a, 16) & ~1:08X}")
                _names.setdefault(n, (int(a, 16) & ~1, bool(e and e["mode"] == "thumb")))
    return _names


def addr(fn):
    """A function's address (int or "0x.." passes through) for the ctl "call" endpoint."""
    if isinstance(fn, int):
        return fn
    if re.fullmatch(r"0x[0-9A-Fa-f]+", fn):
        return int(fn, 16)
    if fn not in names():
        raise SystemExit(f"unknown function {fn!r}")
    return names()[fn][0]


def call(fn, *args):
    """r0 of a direct call; args: ints, or the ctl tokens ("zero", "c:text", "w:a|b", "fn:VT|a", ...)."""
    a = ",".join(hex(x) if isinstance(x, int) else str(x) for x in args)
    r = get("call", fn=hex(addr(fn)), args=a)
    if "error" in r:
        raise SystemExit(f"call {fn}: {r['error']}")
    return int(r["r0"], 16)


def scenes():
    """{scene name: createSceneLayer<T> entry (Thumb bit set)}."""
    out = {}
    for n, (a, thumb) in names().items():
        m = re.fullmatch(r"_Z16createSceneLayerI\d+(\w+?)EPN7cocos2d5LayerEv", n)
        if m:
            out[m.group(1)] = a | thumb
    return out


def scene(name):
    """Change to scene `name` the way the menus do (fade out / in)."""
    factory = scenes().get(name)
    if factory is None:
        raise SystemExit(f"no createSceneLayer<{name}>; known: {', '.join(sorted(scenes()))}")
    mgr = call(SM_GET_INSTANCE)
    return call(SM_CHANGE_SCENE_LAYER, mgr, f"fn:{VT_LAYER_FACTORY:#x}|{factory:#x}", 1, 1, 1)


def only(*names_):
    """Install only these rewrites (names, "name$" for an exact one, or "@file"); none: all again."""
    r = get("rwonly", set=",".join(names_)) if names_ else get("rwonly")
    if "error" in r:
        raise SystemExit(f"rwonly: {r['error']}")
    return r["installed"]


def main():
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    cmd, rest = sys.argv[1], sys.argv[2:]
    if cmd == "scene":
        scene(rest[0])
        print(json.dumps({"scene": rest[0]}))
    elif cmd == "scenes":
        print("\n".join(sorted(scenes())))
    elif cmd == "call":
        print(json.dumps({"r0": hex(call(rest[0], *rest[1:]))}))
    elif cmd == "only":
        print(json.dumps({"installed": only(*rest)}))
    else:
        raise SystemExit(__doc__)


if __name__ == "__main__":
    main()
