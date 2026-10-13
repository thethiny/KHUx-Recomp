"""Client for the emulator control channel (see control.py).

  python -m recomp.emu.ctl state
  python -m recomp.emu.ctl events [since]
  python -m recomp.emu.ctl wait [idle|scene|scene=Name|api|api=62] [timeout_s]
  python -m recomp.emu.ctl ui [all]           # touchable widgets with window-pixel rects
  python -m recomp.emu.ctl tap X Y            # window pixels (origin top-left)
  python -m recomp.emu.ctl tap name=Button_OK | text=OK
  python -m recomp.emu.ctl swipe X0 Y0 X1 Y1 [frames=N] [hold=N]
  python -m recomp.emu.ctl text Sora [0]      # IME text into the focused TextField (+Enter unless 0)
"""
import json
import sys
import urllib.parse
import urllib.request

BASE = "http://127.0.0.1:8765"


def get(path, **q):
    url = f"{BASE}/{path}" + ("?" + urllib.parse.urlencode(q) if q else "")
    timeout = float(q.get("timeout", 30)) + 10
    with urllib.request.urlopen(url, timeout=timeout) as r:
        return json.loads(r.read())


def skip_story():
    """Tap the story's SKIP control if one is on screen (FlashSkipAnimation, top left of SceneLight / dramas).
    Returns True when it tapped."""
    for w in get("ui", all=1).get("widgets", []):
        r = w["rect"]
        if "skip" in w["name"].lower() and r[2] > r[0] and r[3] > r[1] and 0 <= r[0] < 1136 and 0 <= r[1] < 640:
            get("tap", x=(r[0] + r[2]) / 2, y=(r[1] + r[3]) / 2)
            return True
    return False


def main(argv):
    sys.stdout.reconfigure(encoding="utf-8")
    if not argv:
        print(__doc__)
        return 1
    cmd, rest = argv[0], argv[1:]
    if cmd == "state":
        out = get("state")
    elif cmd == "events":
        out = get("events", since=rest[0] if rest else 0)
    elif cmd == "wait":
        q = {"for": rest[0] if rest else "idle", "timeout": rest[1] if len(rest) > 1 else 60}
        out = get("wait", **q)
    elif cmd == "ui":
        out = get("ui", **({"all": 1} if rest[:1] == ["all"] else {}))
    elif cmd == "tap":
        if "=" in rest[0]:
            k, v = rest[0].split("=", 1)
            out = get("tap", **{k: v})
        else:
            out = get("tap", x=rest[0], y=rest[1])
    elif cmd == "swipe":
        extra = dict(a.split("=", 1) for a in rest[4:])          # frames=N hold=N
        out = get("swipe", x0=rest[0], y0=rest[1], x1=rest[2], y1=rest[3], **extra)
    elif cmd == "text":
        out = get("text", s=rest[0], enter=rest[1] if len(rest) > 1 else 1)
    else:
        print(__doc__)
        return 1
    print(json.dumps(out, ensure_ascii=False, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
