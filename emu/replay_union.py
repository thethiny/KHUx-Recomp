"""From a boot that resumes at the union selection, replay it and wait for the tutorial battle (dev helper).

python -m recomp.emu.replay_union
(Server account must be at tutorial progression 5; the battle follows the union step in the same session.)
"""
import sys
import time

from .boot_to_title import main as boot_to_title
from .ctl import get
from .drive_to_download import screen, tap_until, wait_for


def main():
    boot_to_title()
    if not wait_for(lambda s: "SceneUnionRegister" in s, 240):
        sys.exit(f"not at union selection: {screen()}")
    # story drama -> skip, then the explanation popup -> OK
    tap_until(lambda s: "FlashDrama" not in s, {"x": 59, "y": 30}, settle=3)
    tap_until(lambda s: any(w["name"] == "Union_L_03" for w in get("ui", all=1)["widgets"])
              and not any(w["name"] == "Button_OK" and w["touch"] for w in get("ui")["widgets"]),
              {"name": "Button_OK"}, settle=2, tries=4)
    get("tap", x=568, y=185)            # Union_L_03 (Unicornis)
    time.sleep(3)
    get("tap", name="Button_OK")        # "Join Unicornis?"
    if not wait_for(lambda s: "FlashDrama" in s, 30):
        sys.exit(f"no union drama: {screen()}")
    tap_until(lambda s: "FlashDrama" not in s, {"x": 59, "y": 30}, settle=2)
    if not wait_for(lambda s: "SceneActionMap" in s, 180):
        sys.exit(f"no battle: {screen()}")
    print("in battle:", screen())


if __name__ == "__main__":
    main()
