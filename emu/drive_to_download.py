"""Drive a fresh boot to the master-data download and watch its progress (dev helper).

python -m recomp.emu.drive_to_download [watch_seconds]

Animated screens (the title) never go "idle", so input is driven by screen changes instead:
tap, and if the screen has not changed within RETRY_S, tap again.
"""
import sys
import time
from pathlib import Path

from .. import paths
from .ctl import get

SERVER_LOG = paths.SERVER_LOG
RETRY_S = 10


def screen():
    try:
        return get("state").get("screen") or []
    except OSError:
        return []


def wait_for(cond, timeout):
    t0 = time.time()
    while time.time() - t0 < timeout:
        if cond(screen()):
            return True
        time.sleep(0.5)
    return False


def tap_until(cond, tap, settle=2.0, tries=12):
    """Tap (dict for /tap) until cond(screen) holds; retries every RETRY_S seconds."""
    time.sleep(settle)
    for _ in range(tries):
        if cond(screen()):
            return True
        r = get("tap", **tap)
        if "error" in r:
            print("  tap error:", r["error"])
        if wait_for(cond, RETRY_S):
            return True
    return False


def main():
    watch = float(sys.argv[1]) if len(sys.argv) > 1 else 180
    if not wait_for(lambda s: s, 60):
        sys.exit("control channel not up")
    # splash: usually continues by itself, sometimes needs a tap (the phone does the same)
    if not wait_for(lambda s: "SceneTitle" in s, 30):
        tap_until(lambda s: "SceneTitle" in s, {"x": 568, "y": 320}, settle=0)
    print("title")
    tap_until(lambda s: "SceneAgreement" in s, {"x": 568, "y": 320})
    print("agreement")
    tap_until(lambda s: "SceneTutorialDownload" in s, {"text": "Accept"})
    print("download screen")
    time.sleep(2)
    print("download:", get("tap", text="Download").get("tap"))
    t0 = time.time()
    while time.time() - t0 < watch:
        time.sleep(15)
        st = get("state")
        served = SERVER_LOG.read_text(errors="replace").count("SERVING MASTER")
        busy = [f"{t['tid']}:{t['entry'][:24]}={t['state'][:24]}/{t['slices']}" for t in st["threads"]
                if t["state"] != "done" and t["tid"] != 1]
        print(f"{time.time() - t0:5.0f}s screen {st['screen']} served {served} | {busy}", flush=True)
        if "SceneTutorialDownload" not in (st["screen"] or []):
            break


if __name__ == "__main__":
    main()
