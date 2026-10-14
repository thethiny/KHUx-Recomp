"""Play story quests (and any raid boss that shows up) back to back (dev helper).

python -m recomp.emu.campaign [rounds] [battle_max_seconds]

Each round: get to the story quest detail screen (from MyPage via Quests / a guide's "Quests" button),
then python -m recomp.emu.play_quest, which plays whatever the screen offers - the newest quest, or the raid
boss when one is up. Stops when a round ends somewhere unexpected (error popup, crash) so it can be looked at.
"""
import subprocess
import sys
import time

from .ctl import STORY_SCENES, advance_story, get, is_story
from .play_quest import button, screen, tap_w, widgets


def text_of(name):
    """Text of the first on-screen widget called `name` (panels for other modes sit off screen)."""
    return next((w.get("text") for w in widgets() if w["name"] == name and w.get("text")
                 and 0 <= w["rect"][1] < 640 and -100 < w["rect"][0] < 1136), None)


def select_newest():
    """The detail screen opens on the last played quest; pick the highest quest number on the strip
    (Txt_Stage_Num_Label under each quest icon), unless a raid boss panel is showing."""
    hp = text_of("Raid_Boss_HpFix_Label")
    if hp and hp != "999,999,999":
        return
    nums = [w for w in widgets() if w["name"] == "Txt_Stage_Num_Label" and (w.get("text") or "").isdigit()
            and 0 <= w["rect"][0] <= 1136]
    if nums:
        r = max(nums, key=lambda w: int(w["text"]))["rect"]
        get("tap", x=(r[0] + r[2]) / 2, y=r[1] - 30)
        time.sleep(4)


def to_quest_select(timeout=240):
    """From MyPage (or quest select already) to the story quest detail screen."""
    t0 = time.time()
    while time.time() - t0 < timeout:
        s = screen()
        if s in ("SceneActionMap", "SceneRaidBattle"):     # a battle (re)started: play it out
            subprocess.run([sys.executable, "-u", "-m", "recomp.emu.auto_battle", "2400"])
            continue
        if "FlashMovieView" in (get("state").get("screen") or []):
            get("tap", x=568, y=320)            # movie over a scene (new world, boss appears, ...)
            time.sleep(3)
            continue
        ok = button("OK")
        if ok and s != "SceneBattleResult":     # guide popups block everything under them
            print("  popup:", text_of("Txt"))
            tap_w(ok)
            time.sleep(3)
            continue
        if s == "SceneAdventureSelect":
            # map with the quest buttons -> story quests (QuestBut1); the detail view has Txt_Story_Label
            if text_of("Txt_Story_Label") or text_of("Raid_Boss_HpFix_Label"):
                return True
            b = next((w for w in widgets() if w["touch"] and w["name"] == "QuestBut1"), None)
            if b:
                tap_w(b)
                time.sleep(6)
                continue
        elif s == "SceneMyPage":
            b = button(name="Button_Goto") or next(
                (w for w in widgets() if w["touch"] and w["name"] == "Button_World"), None)
            if b:
                tap_w(b)
                time.sleep(8)
                continue
        ok = button("OK")
        if s == "SceneBattleResult":
            b = button(name="Button_Ok")
            if b:
                tap_w(b)
            get("tap", x=568, y=600)
        elif ok:                       # guide popups / notices
            tap_w(ok)
        elif s == "SceneNotice" and (close := button(name="Button_Close")):
            tap_w(close)               # startup notice WebView popup
        elif is_story(s) or s in ("SceneNotice", ""):
            advance_story()
        time.sleep(3)
    return False


def main():
    rounds = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    limit = sys.argv[2] if len(sys.argv) > 2 else "2400"
    for r in range(rounds):
        if screen() in ("SceneActionMap", "SceneRaidBattle") + STORY_SCENES:
            print(f"round {r}: finishing the battle in progress", flush=True)
            subprocess.run([sys.executable, "-u", "-m", "recomp.emu.auto_battle", limit])
        if not to_quest_select():
            print(f"round {r}: could not reach the quest screen (at {screen()})")
            return
        select_newest()
        title = text_of("Txt_Story_Label")
        hp = text_of("Raid_Boss_HpFix_Label")
        print(f"round {r}: {'raid boss HP ' + hp if hp and hp != '999,999,999' else title}", flush=True)
        subprocess.run([sys.executable, "-u", "-m", "recomp.emu.play_quest", limit])
        if button("OK") and screen() in ("", "SceneManager"):
            print("error popup after the round - stopping")
            return


if __name__ == "__main__":
    main()
