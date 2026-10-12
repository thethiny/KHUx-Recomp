"""Wait for a (re)started game to reach the title and tap through it (dev helper).

python -m recomp.emu.boot_to_title
Then continue with: python -m recomp.emu.step ...
"""
import sys

from .drive_to_download import screen, tap_until, wait_for


def main():
    if not wait_for(lambda s: s, 60):
        sys.exit("control channel not up")
    if not wait_for(lambda s: "SceneTitle" in s, 30):
        tap_until(lambda s: "SceneTitle" in s, {"x": 568, "y": 320}, settle=0)
    tap_until(lambda s: "SceneTitle" not in s and s, {"x": 568, "y": 320})
    print("past title:", screen())


if __name__ == "__main__":
    main()
