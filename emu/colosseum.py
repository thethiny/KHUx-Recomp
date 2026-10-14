"""Play Coliseum rounds back to back (dev helper, native exe: uses insta-win).

python -m recomp.emu.colosseum [rounds]

From MyPage or quest select: open the Coliseum (the fourth mode button on the adventure screen, QuestBut4 ->
/colosseum 114), then per round: Begin -> keyblade Start (/colosseum/start 116) -> insta-win (every enemy
defeated, then the stage finish -> /colosseum/clear 118) -> result screens. Stops on anything unexpected.
"""
import sys
import time

from .campaign import to_quest_select
from .ctl import advance_story, get, is_story
from .play_quest import ack_popups, button, instawin, screen, tap_w, wait_api


def labels():
    return {w["name"]: w.get("text") for w in get("ui")["widgets"]
            if w["name"] in ("Txt_Class_Label", "Txt_Round_Num_Label") and 0 <= w["rect"][0] < 1136}


def open_coliseum():
    if screen() != "SceneAdventureSelect":
        to_quest_select()
    since = get("state")["events"]
    touchable = {w["name"] for w in get("ui")["widgets"] if w["touch"]}
    if "Button_Ranking" in touchable:                  # already on the Coliseum
        return True
    if "QuestBut4" not in touchable:                   # quest detail -> mode buttons
        get("tap", name="Button_Back")
        time.sleep(3)
    get("tap", name="QuestBut4")                       # (a hidden Coliseum panel keeps placeholder labels)
    return wait_api(114, since, 30)


def play_round():
    b = None
    for _ in range(3):                                 # a popup (level up, ...) can swallow the first tap
        ack_popups()
        get("tap", x=773, y=397)                       # Begin (Txt_Start)
        time.sleep(4)
        b = button(name="Button_OK")                   # keyblade dialog: "Start"
        if b:
            break
    if not b:
        return "no keyblade dialog"
    tap_w(b)
    if not instawin(90):
        return "no clear"
    for _ in range(25):                                # result screens
        s = screen()
        if s not in ("SceneBattleResult", "LoadingIndicator", "SceneActionMap", "") and not is_story(s):
            break
        if is_story(s):
            advance_story()
        if s == "SceneBattleResult":
            b = button(name="Button_Ok")
            if b:
                tap_w(b)
            get("tap", x=568, y=600)
        time.sleep(3)
    return None if screen() == "SceneAdventureSelect" else f"ended at {screen()}"


def main():
    sys.stdout.reconfigure(encoding="utf-8")
    rounds = int(sys.argv[1]) if len(sys.argv) > 1 else 1
    if not open_coliseum():
        sys.exit("could not open the Coliseum")
    for k in range(rounds):
        print(f"round {k}: {labels()}")
        err = play_round()
        if err:
            sys.exit(f"stopped: {err}")
    print("now:", labels())


if __name__ == "__main__":
    main()
