"""Find a lifted function that breaks a quest start (bisection with KHUX_LIFT_DENY).

python recomp/lift/bisect_quest.py [--verify]   (--verify: with the checker on, for checker-induced faults)

Each test boots the hybrid emulator with half of the suspects kept emulated, gets to MyPage, starts a quest
(recomp.emu.campaign 1 <short battle>) and checks the log for a failed frame. A crash that goes away when a
half is denied puts the culprit in that half. Assumes one culprit (or at least one in the half kept).
Progress is saved to out/lift/bisect_quest.json, so a run can be resumed.
"""
import json
import subprocess
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import paths  # noqa: E402

ROOT = HERE.parent.parent              # the folder that contains recomp/
STATE = paths.OUT / "lift" / "bisect_quest.json"
DENY = paths.OUT / "lift" / "bisect_deny.json"
LOG = paths.OUT / "lift" / "bisect_run.log"
ENV_APK = "khux_1.0.1_shortmovie_aligned.apk"


def kill_emulators():
    subprocess.run(["powershell", "-NoProfile", "-Command",
                    "Get-CimInstance Win32_Process | Where-Object { $_.Name -eq 'python.exe' -and "
                    "$_.CommandLine -like '*recomp.emu*' } | ForEach-Object { Stop-Process -Id $_.ProcessId "
                    "-Force }"], capture_output=True)


def wait_for(pattern, seconds):
    end = time.time() + seconds
    while time.time() < end:
        text = LOG.read_text(encoding="utf-8", errors="replace") if LOG.exists() else ""
        if any(p in text for p in pattern):
            return text
        time.sleep(2)
    return LOG.read_text(encoding="utf-8", errors="replace") if LOG.exists() else ""


def crashes(deny):
    """True when the quest start fails with these entries kept emulated."""
    import os
    kill_emulators()
    DENY.write_text(json.dumps([f"{a:#x}" for a in sorted(deny)]))
    env = dict(os.environ, KHUX_APK=ENV_APK, KHUX_LIFT_DENY=str(DENY))
    log = open(LOG, "w", encoding="utf-8")
    flags = ["--hybrid"] + ([] if "--verify" in sys.argv else ["--no-verify"])
    proc = subprocess.Popen([sys.executable, "-u", "-m", "recomp.emu.run", *flags],
                            cwd=ROOT, env=env, stdout=log, stderr=subprocess.STDOUT)
    try:
        text = wait_for(["SceneTitle", "failed:", ">>> crash"], 600)
        if "SceneTitle" not in text:
            return True
        subprocess.run([sys.executable, "-m", "recomp.emu.boot_to_title"], cwd=ROOT, capture_output=True,
                       timeout=300)
        text = wait_for(["SceneNotice", "SceneMyPage", "failed:"], 300)
        if "SceneMyPage" not in text and "failed:" not in text:
            subprocess.run([sys.executable, "-m", "recomp.emu.step", "tap", "name=Button_Close"], cwd=ROOT,
                           capture_output=True, timeout=120)
        text = wait_for(["SceneMyPage", "failed:"], 600)
        if "failed:" in text:
            return True
        try:
            subprocess.run([sys.executable, "-m", "recomp.emu.campaign", "1", "40"], cwd=ROOT,
                           capture_output=True, timeout=420)
        except subprocess.TimeoutExpired:
            pass
        text = LOG.read_text(encoding="utf-8", errors="replace")
        return "failed:" in text or ">>> crash" in text
    finally:
        proc.kill()
        log.close()
        kill_emulators()


def main():
    lifted = json.loads((paths.OUT / "lift" / "lifted.json").read_text())
    blocks = set(json.loads((paths.OUT / "coverage.json").read_text())["blocks"])
    all_entries = sorted(a for a, m, _ in lifted if (a | (1 if m == "thumb" else 0)) in blocks)
    # --keep 0xA,0xB: always lifted, never suspects (the other half of a fault that needs two functions)
    keep = set()
    if "--keep" in sys.argv:
        keep = {int(x, 16) for x in sys.argv[sys.argv.index("--keep") + 1].split(",")}
    all_entries = [a for a in all_entries if a not in keep]
    if STATE.exists():
        st = json.loads(STATE.read_text())
        suspects = [int(x, 16) for x in st["suspects"]]
        print(f"resuming with {len(suspects)} suspects", flush=True)
    else:
        suspects = all_entries
        t0 = time.time()
        if not crashes(set()):
            sys.exit("no crash with every function lifted: nothing to bisect")
        print(f"baseline crashes ({time.time() - t0:.0f}s per test)", flush=True)
    while len(suspects) > 1:
        half = suspects[:len(suspects) // 2]
        rest = suspects[len(suspects) // 2:]
        # keep `half` emulated (plus everything already cleared stays lifted)
        bad = crashes(set(half))
        suspects = rest if bad else half
        STATE.write_text(json.dumps({"suspects": [f"{a:#x}" for a in suspects]}))
        print(f"{'crash' if bad else 'ok'} with {len(half)} denied -> {len(suspects)} suspects "
              f"({suspects[0]:#x}..{suspects[-1]:#x})", flush=True)
    a = suspects[0]
    print(f"culprit: {a:#x}", flush=True)
    print("confirm (only it denied):", "ok" if not crashes({a}) else "still crashes", flush=True)


if __name__ == "__main__":
    main()
