"""One interactive step: act, wait for the game to react, report what is on screen now.

  python -m recomp.emu.step                       # just report
  python -m recomp.emu.step tap text=Collect      # any ctl action: tap X Y | tap name=.. | text Sora | swipe ..
  python -m recomp.emu.step --wait 30 tap 568 320 # max seconds to wait for a reaction (default 20)

After the action it waits until the screen changes, a new idle/api/crash event arrives, or the timeout
passes, then prints the new events and the visible widgets (touchable ones marked *).
"""
import sys
import time

from .ctl import get


def summary():
    ui = get("ui")
    if "error" in ui:
        return f"ui error: {ui['error']}"
    lines = [f"screen: {ui.get('screen')}"]
    for w in ui["widgets"]:
        cap = w.get("text") or w.get("label") or ""
        if not (w["touch"] or cap):
            continue
        lines.append(f"  {'*' if w['touch'] else ' '} {w['class'].split('::')[-1]:<10} {w['name'][:28]:<28} "
                     f"{cap[:50]!r:<52} {w['rect']}")
    return "\n".join(lines)


def main(argv):
    sys.stdout.reconfigure(encoding="utf-8")
    wait = 20.0
    if argv[:1] == ["--wait"]:
        wait, argv = float(argv[1]), argv[2:]
    since = get("state")["events"]
    before = get("state").get("screen")
    if argv:
        cmd, rest = argv[0], argv[1:]
        if cmd == "tap":
            r = get("tap", **dict([rest[0].split("=", 1)])) if "=" in rest[0] else get("tap", x=rest[0], y=rest[1])
        elif cmd == "text":
            r = get("text", s=rest[0], enter=rest[1] if len(rest) > 1 else 1)
        elif cmd == "swipe":
            r = get("swipe", x0=rest[0], y0=rest[1], x1=rest[2], y1=rest[3],
                    **dict(a.split("=", 1) for a in rest[4:]))
        else:
            sys.exit(f"unknown action {cmd}")
        if "error" in r:
            print("action error:", r["error"])
        t0 = time.time()
        while time.time() - t0 < wait:
            time.sleep(0.5)
            evs = get("events", since=since)
            if any(e["kind"] in ("screen", "api", "crash", "master_save") for e in evs):
                time.sleep(1.5)   # let the new screen settle
                break
            if get("state").get("screen") != before:
                break
    for e in get("events", since=since):
        data = {k: v for k, v in e.items() if k not in ("seq", "t", "kind", "frame")}
        print(f"event {e['kind']}: {data}")
    print(summary())


if __name__ == "__main__":
    main(sys.argv[1:])
