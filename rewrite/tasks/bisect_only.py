# python bisect_only.py DIR KEEP_NAME : find the installed rewrite (DIR/only.txt) that makes the record(s) in DIR
# mismatch. KEEP_NAME (a line of only.txt, e.g. the recorded function's) stays installed in every trial.
import os, subprocess, sys
from pathlib import Path

RECOMP = Path(r'D:\Modding\git\KHUx\recomp')
rec_dir = Path(sys.argv[1]).resolve()
keep = sys.argv[2]
lines = [l for l in (rec_dir / 'only.txt').read_text(encoding='utf8').splitlines() if l.strip()]
keep_lines = [l for l in lines if keep in l]
rest = [l for l in lines if keep not in l]
trial_file = rec_dir.parent / (rec_dir.name + '_trial_only.txt')


def mismatches(subset):
    trial_file.write_text('\n'.join(keep_lines + subset) + '\n', encoding='utf8')
    subprocess.run([sys.executable, str(RECOMP / 'rewrite/tools/replay.py'), str(rec_dir), '--only', str(trial_file)],
                   capture_output=True, text=True, timeout=900)
    log = (RECOMP / 'out/host/replay.log').read_text(encoding='utf8', errors='replace')
    return 'RW MISMATCH' in log


print('all:', mismatches(rest), 'none:', mismatches([]), flush=True)
cand = rest
while len(cand) > 1:
    half = cand[:len(cand) // 2]
    if mismatches(half):
        cand = half
    else:
        cand = cand[len(cand) // 2:]
    print(len(cand), cand[:3], flush=True)
print('culprit:', cand)
