"""Find a lifted non-leaf function that breaks the game, by bisecting KHUX_LIFT_RANGE.

python recomp/lift/find_bad.py [--until "== nativeInit ->"] [--fail "!!"] [--timeout 240] [--lo 0 --hi N]

Each probe starts the emulator in hybrid mode with only a range of non-leaf functions enabled and waits for
the --until marker (pass) or a line containing a --fail marker before it (fail).
"""
import argparse
import json
import os
import subprocess
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import paths  # noqa: E402

LOG = paths.OUT / "lift" / "bisect_run.log"


def probe(lo, hi, until, fail, timeout):
    env = dict(os.environ, KHUX_LIFT_RANGE=f"{lo}:{hi}", KHUX_VERIFY_CALLS="1")
    with open(LOG, "wb") as f:
        p = subprocess.Popen([sys.executable, "-m", "recomp.emu.run", "--hybrid"], cwd=str(HERE.parent.parent),
                             stdout=f, stderr=subprocess.STDOUT, env=env)
    t0 = time.time()
    result = None
    try:
        while time.time() - t0 < timeout and p.poll() is None:
            time.sleep(2)
            text = LOG.read_bytes().replace(b"\0", b"").decode("utf-8", "replace")
            for line in text.splitlines():
                if until in line:
                    result = True
                    break
                if any(m in line for m in fail) and "hybrid:" not in line:
                    result = False
                    break
            if result is not None:
                break
        if result is None:
            result = False if p.poll() is not None else None
    finally:
        p.kill()
        p.wait()
    return result


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--until", default="== nativeInit ->")
    ap.add_argument("--fail", nargs="*", default=["!! nativeInit failed", "unmapped", "native crash"])
    ap.add_argument("--timeout", type=int, default=240)
    ap.add_argument("--lo", type=int, default=0)
    ap.add_argument("--hi", type=int, default=0)
    args = ap.parse_args()
    lifted = json.loads((paths.OUT / "lift" / "lifted.json").read_text())
    nonleaf = [(a, m) for a, m, leaf in lifted if not leaf]
    lo, hi = args.lo, args.hi or len(nonleaf)
    print(f"{len(nonleaf)} non-leaf functions; checking range {lo}:{hi}", flush=True)
    r = probe(lo, hi, args.until, args.fail, args.timeout)
    print(f"  {lo}:{hi} -> {r}", flush=True)
    if r is not False:
        print("the full range does not fail - nothing to bisect")
        return
    while hi - lo > 1:
        mid = (lo + hi) // 2
        r = probe(lo, mid, args.until, args.fail, args.timeout)
        print(f"  {lo}:{mid} -> {r}", flush=True)
        if r is False:
            hi = mid
        elif r is True:
            lo = mid
        else:
            print("  inconclusive (timeout) - stopping")
            break
    a, m = nonleaf[lo]
    print(f"culprit candidate: rank {lo} -> {a:#x} ({m})")


if __name__ == "__main__":
    main()
