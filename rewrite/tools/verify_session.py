"""Run the native exe with every rewritten function shadow-verified, play a little, and merge the results.

python recomp/rewrite/tools/verify_session.py [quests] [--fs fs2]

Boots (KHUX_RW_VERIFY=1), clears the startup notices to MyPage, plays `quests` story quests with rush +
insta-win (battle load, win, results, quest select; default 2; the account is too far ahead to win real fights), closes the game and runs merge_stats.py. Needs the private server
running. Prints the rewrite lines of the run log (mismatches, the per-function report).
"""
import os
import subprocess
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent.parent
RECOMP = HERE.parent
ROOT = RECOMP.parent
LOG = RECOMP / "out" / "host" / "rwverify.log"


def kill_exe():
    subprocess.run(["powershell", "-NoProfile", "-Command",
                    "Get-Process khux -ErrorAction SilentlyContinue | Stop-Process -Force"], capture_output=True)


def main():
    args = sys.argv[1:]
    quests = int(args[0]) if args and args[0].isdigit() else 2
    fs = args[args.index("--fs") + 1] if "--fs" in args else None
    kill_exe()
    time.sleep(1)
    env = dict(os.environ, KHUX_RW_VERIFY="1")
    cmd = [str(RECOMP / "out" / "host" / "khux.exe")] + (["--fs", fs] if fs else [])
    with open(LOG, "w") as log:
        exe = subprocess.Popen(cmd, cwd=str(RECOMP), stdout=log, stderr=subprocess.STDOUT, env=env)
    try:
        subprocess.run([sys.executable, "-m", "recomp.emu.boot_to_title"], cwd=str(ROOT), timeout=400)
        sys.path.insert(0, str(ROOT))
        from recomp.emu.ctl import advance_story, get, is_story
        from recomp.emu.play_quest import button, screen, tap_w
        for _ in range(30):                          # notices / login bonus / story -> MyPage
            s = screen()
            if s == "SceneMyPage" and not button():
                break
            if s == "SceneActionMap":                # a quest resumed after a killed session: win it
                from recomp.emu.play_quest import instawin
                os.environ["KHUX_INSTAWIN"] = "1"
                instawin(120)
            b = button() or button(name="Button_Close")
            if b:
                tap_w(b)
            elif is_story(s):
                advance_story()
            else:
                get("tap", x=568, y=600)
            time.sleep(3)
        if quests:
            try:
                subprocess.run([sys.executable, "-u", "-m", "recomp.emu.campaign", str(quests), "300"],
                               cwd=str(ROOT), env=dict(os.environ, KHUX_RUSH="1", KHUX_INSTAWIN="1"), timeout=600 * quests)
            except subprocess.TimeoutExpired:
                print("campaign took too long; keeping the results so far")
        time.sleep(15)                               # one more report (every 1200 frames)
    except Exception as e:                           # the play part failed: the results so far still count
        print("session stopped early:", e)
    finally:
        kill_exe()
    lines = [l for l in LOG.read_text(encoding="utf-8", errors="replace").splitlines()
             if l.startswith(("rewrite.dll", "RW ", "    byte", "!! FATAL", "!! CRASH"))]
    print("\n".join(lines[-80:]))
    subprocess.run([sys.executable, str(HERE / "tools" / "merge_stats.py")])


if __name__ == "__main__":
    main()
