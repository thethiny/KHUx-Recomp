"""Wait for a (re)started game to reach the title and tap through it (dev helper).

python -m recomp.emu.boot_to_title
Then continue with: python -m recomp.emu.step ...
"""
import sys
import time

from .drive_to_download import get, screen, tap_until, wait_for


def main():
    if not wait_for(lambda s: s, 60):
        sys.exit("control channel not up")
    if not wait_for(lambda s: "SceneTitle" in s, 30):
        tap_until(lambda s: "SceneTitle" in s, {"x": 568, "y": 320}, settle=0)
    # the title animation idles a long time before it takes a tap: a tap after 1 s skips it, a second one 3 s
    # later (once the skip animation has played) leaves the title; tap_until is the slow fallback
    time.sleep(1)
    get("tap", x=568, y=320)
    time.sleep(3)
    get("tap", x=568, y=320)
    tap_until(lambda s: "SceneTitle" not in s and s, {"x": 568, "y": 320}, settle=0)
    print("past title:", screen())


if __name__ == "__main__":
    main()
