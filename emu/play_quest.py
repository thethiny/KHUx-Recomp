"""Play the quest (or raid boss) shown on the quest detail screen (SceneAdventureSelect) end to end (dev helper).

python -m recomp.emu.play_quest [battle_max_seconds]

Begin -> keyblade Confirm -> support -> Start (103) -> auto_battle -> result screens -> back to quest select.
Guide popups (Txt + Button_OK 'OK') are acknowledged wherever they appear.
"""
import subprocess
import sys
import time

from .ctl import get


def screen():
    s = get("state").get("screen") or []
    return s[1] if len(s) > 1 else ""


def widgets():
    ui = get("ui")
    return ui.get("widgets", []) if "error" not in ui else []


def button(label=None, name="Button_OK"):
    for w in widgets():
        if w["touch"] and w["name"] == name and (label is None or w.get("label") == label) \
                and w["rect"][0] != w["rect"][2]:
            return w
    return None


def tap_w(w):
    r = w["rect"]
    get("tap", x=(r[0] + r[2]) / 2, y=(r[1] + r[3]) / 2)


def wait_api(action, since, timeout=60):
    actions = action if isinstance(action, tuple) else (action,)
    t0 = time.time()
    while time.time() - t0 < timeout:
        for ev in get("events", since=since):
            if ev["kind"] == "api" and ev.get("action") in actions:
                return True
            if ev["kind"] == "crash":
                sys.exit(f"crash: {ev}")
        time.sleep(1)
    return False


def ack_popups(rounds=3):
    for _ in range(rounds):
        ok = button("OK")
        if not ok:
            return
        print("  popup:", next((w.get("text") for w in widgets() if w["name"] == "Txt" and w.get("text")), ""))
        tap_w(ok)
        time.sleep(2)


def begin():
    """Tap Begin. On a raid panel the boss icon (Raid_Boss_Icon, touchable: opens the reward list) overlaps
    most of Button_RaidStart, so tap the strip of the button right of the icon."""
    allw = get("ui", all=1).get("widgets", [])
    close = next((w for w in allw if w["name"] == "Close_Button" and "/Panel_28/" in w["path"]), None)
    if close:                                     # the raid reward list is open over the panel: close it
        tap_w(close)
        time.sleep(2)
        allw = get("ui", all=1).get("widgets", [])
    btn = next((w for w in allw if w["name"] == "Button_RaidStart" and 0 <= w["rect"][1] < 640), None)
    icon = next((w for w in allw if w["name"] == "Raid_Boss_Icon" and 0 <= w["rect"][1] < 640), None)
    if btn and icon and icon["rect"][2] < btn["rect"][2]:
        get("tap", x=(icon["rect"][2] + btn["rect"][2]) / 2, y=(btn["rect"][1] + btn["rect"][3]) / 2)
    else:
        get("tap", x=773, y=399)


def main():
    limit = sys.argv[1] if len(sys.argv) > 1 else "1200"
    since = get("state")["events"]
    ack_popups()
    begin()
    if not wait_api(102, since, 30):
        ack_popups()
        begin()
        if not wait_api(102, since, 30):
            sys.exit("no 102 after Begin")
    time.sleep(3)
    for _ in range(10):                           # keyblade dialog: Confirm
        b = button("Confirm")
        if b:
            tap_w(b)
            break
        time.sleep(1)
    time.sleep(3)
    ack_popups()
    get("tap", x=850, y=160)                      # first support row
    time.sleep(2)
    since = get("state")["events"]
    for _ in range(5):
        b = button("Start")
        if b:
            tap_w(b)
        if wait_api((103, 112), since, 15):      # /stage/start, or /raid/start for a raid boss
            break
    else:
        sys.exit("no 103/112 after Start")
    print("stage started")
    for _ in range(150):                          # stage intro: loading, then maybe a story drama (tap on)
        sc = screen()
        if sc in ("SceneActionMap", "SceneRaidBattle"):
            break
        if sc == "SceneFlashDrama":
            get("tap", x=568, y=320)
        time.sleep(2)
    subprocess.run([sys.executable, "-u", "-m", "recomp.emu.auto_battle", limit])
    for _ in range(30):                           # result screens
        s = screen()
        if s not in ("SceneBattleResult", "LoadingIndicator", "SceneActionMap", "SceneRaidBattle",
                     "SceneFlashDrama", ""):
            break
        if s == "SceneFlashDrama":                # after-quest story drama
            get("tap", x=568, y=320)
        if s == "SceneBattleResult":
            b = button(name="Button_Ok")
            if b:
                tap_w(b)
            get("tap", x=568, y=600)
        time.sleep(4)
    print("now at:", screen())


if __name__ == "__main__":
    main()
