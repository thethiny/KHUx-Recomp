"""Replay recorded calls (KHUX_RW_CAPTURE records, *.rwr) against their rewrites, without the game.

python recomp/rewrite/tools/replay.py DIR [--only FILE]

Runs khux.exe in replay mode (KHUX_RW_REPLAY=DIR): each record's registers, imports and guest pages are put back
and the call goes through the verifier again - the lifted original against the rewrite - and the outcome is
compared with the one the live session had. A record that crashes the replay is counted and skipped (the run
resumes after it, KHUX_RW_REPLAY_FROM). --only: a KHUX_RW_ONLY list (names) to install; default: DIR/only.txt,
which the capture writes when its session had a KHUX_RW_ONLY list (callees that ran lifted when the call was
recorded must run lifted in the replay too), else all.
Prints the summary: agree / differ (each listed) / crashed / skipped, and the time.
"""
import os
import re
import subprocess
import sys
import time
from pathlib import Path

RECOMP = Path(__file__).resolve().parents[2]
EXE = RECOMP / "out" / "host" / "khux.exe"


def main():
    args = sys.argv[1:]
    if not args:
        sys.exit(__doc__)
    rec_dir = Path(args[0]).resolve()
    only = args[args.index("--only") + 1] if "--only" in args else None
    if not only and (rec_dir / "only.txt").exists():     # the capture session's installed set
        only = str(rec_dir / "only.txt")
    files = sorted(p.name for p in rec_dir.glob("*.rwr"))
    log = RECOMP / "out" / "host" / "replay.log"
    start, agree, differ, crashed, skipped = 0, [], [], [], 0
    t0 = time.time()
    while start < len(files):
        env = dict(os.environ, KHUX_RW_REPLAY=str(rec_dir), KHUX_RW_VERIFY="1", KHUX_RW_CAP="0",
                   KHUX_RW_REPLAY_FROM=str(start))
        if only:
            env["KHUX_RW_ONLY"] = "@" + only
        with open(log, "w") as f:
            subprocess.run([str(EXE)], cwd=str(RECOMP), stdout=f, stderr=subprocess.STDOUT, env=env, timeout=3600)
        text = log.read_text(errors="replace")
        done = 0
        last_started = None
        for line in text.splitlines():
            m = re.match(r"RW REPLAY start .*[/\\](\S+\.rwr)", line)
            if m:
                last_started = m.group(1)
                continue
            m = re.match(r"RW REPLAY .*[/\\](\S+\.rwr) (.*): (verified|MISMATCH|not compared), live (verified|MISMATCH)", line)
            if m:
                (agree if m.group(3) == m.group(4) else differ).append(f"{m.group(1)} {m.group(2)}: replay {m.group(3)}, live {m.group(4)}")
                done += 1
            elif re.match(r"RW REPLAY .*: skipped", line):
                skipped += 1
                done += 1
        if "RW REPLAY:" in text:                          # reached the end
            break
        # crashed in the record after the last one that finished
        bad = last_started or (files[start + done] if start + done < len(files) else "?")
        crashed.append(bad)
        start += done + 1
    dt = time.time() - t0
    print(f"{len(files)} records: {len(agree)} agree with the live outcome, {len(differ)} differ, "
          f"{len(crashed)} crashed, {skipped} skipped - {dt:.1f} s")
    for d in differ:
        print("  differs:", d)
    for c in crashed:
        print("  crashed:", c)


if __name__ == "__main__":
    main()
