"""Fold the verifier's per-run results (out/rewrite/stats/*.tsv) into rewrite/status.tsv and rewrite/verified.txt.

python recomp/rewrite/tools/merge_stats.py [--reset ADDR ...]

Each verifying run of the native host rewrites its own stats file with its running totals, so the status is
recomputed from all of them every time. status.tsv: one line per rewritten function called while verifying:
addr, name, calls, verified, mismatches, unverifiable. verified.txt: addresses with at least one verified call and
no mismatch - the native host installs only these when it is not verifying. --reset ADDR (after fixing a function)
ignores that function's results from the runs so far (kept in rewrite/reset.tsv).

rewrite/accepted.txt lists functions reviewed by hand whose mismatches are known and harmless ("ADDR reason" per
line, # comments): the original stores bytes it never initialised (stale stack contents), or another thread races
with the shadow run. They count as verified when they have verified calls, mismatches or not. "ADDR!" (one-time
exceptions, reviewed: every call differs only in such bytes, so none is ever clean) counts without a verified call.
"""
import argparse
from pathlib import Path

HERE = Path(__file__).resolve().parent.parent
RECOMP = HERE.parent
STATS = RECOMP / "out" / "rewrite" / "stats"
STATUS = HERE / "status.tsv"
VERIFIED = HERE / "verified.txt"
RESET = HERE / "reset.tsv"
ACCEPTED = HERE / "accepted.txt"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--reset", nargs="*", default=[], help="forget the results of these addresses so far")
    a = ap.parse_args()
    files = sorted(STATS.glob("*.tsv")) if STATS.exists() else []
    cutoff = {}
    if RESET.exists():
        for line in RESET.read_text().splitlines():
            k, f = line.split("\t")
            cutoff[k] = f
    for addr in a.reset:
        cutoff[f"{int(addr, 16):08X}"] = files[-1].name if files else ""
    RESET.write_text("".join(f"{k}\t{v}\n" for k, v in sorted(cutoff.items())))
    status = {}
    for f in files:
        for line in f.read_text(encoding="utf-8").splitlines():
            p = line.split("\t")
            if len(p) != 6 or f.name <= cutoff.get(p[0], ""):
                continue
            rec = status.setdefault(p[0], [p[1], 0, 0, 0, 0])
            for k in range(4):
                rec[k + 1] += int(p[k + 2])
    lines = ["addr\tname\tcalls\tverified\tmismatches\tunverifiable"]
    lines += [f"{k}\t{v[0]}\t{v[1]}\t{v[2]}\t{v[3]}\t{v[4]}" for k, v in sorted(status.items())]
    STATUS.write_text("\n".join(lines) + "\n", encoding="utf-8")
    accepted, always = set(), set()
    if ACCEPTED.exists():
        for line in ACCEPTED.read_text(encoding="utf-8").splitlines():
            if line.strip() and not line.startswith("#"):
                tok = line.split()[0]
                addr = f"{int(tok.rstrip('!'), 16):08X}"
                accepted.add(addr)
                if tok.endswith("!"):
                    always.add(addr)
    ok = sorted(k for k, v in status.items()
                if (v[2] > 0 and (v[3] == 0 or k in accepted)) or (k in always and v[3] > 0))
    VERIFIED.write_text("".join(f"{k}\n" for k in ok))
    bad = sorted((k, v[0], v[3]) for k, v in status.items() if v[3] and k not in accepted)
    print(f"{len(status)} functions with results, {len(ok)} verified, {len(bad)} with mismatches")
    for k, name, n in bad[:60]:
        print(f"  MISMATCH {k} {name} ({n})")


if __name__ == "__main__":
    main()
