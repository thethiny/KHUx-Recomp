# python recomp/rewrite/tools/replay_bisect.py REC_DIR TARGET_NAME : which other installed rewrite makes TARGET's record mismatch.
# Replays the record(s) with only.txt = TARGET + a subset of the capture's set (REC_DIR/only_full.txt), halving.
import os, re, subprocess, sys
RECOMP = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
rec, target = sys.argv[1], sys.argv[2]
if not os.path.exists(os.path.join(rec, 'only_full.txt')):      # keep the capture's set (only.txt is the default)
    open(os.path.join(rec, 'only_full.txt'), 'w', encoding='utf-8').write(open(os.path.join(rec, 'only.txt'), encoding='utf-8').read())
full = [l.strip() for l in open(os.path.join(rec, 'only_full.txt'), encoding='utf-8') if l.strip()]
others = [n for n in full if n.rstrip('$') != target.rstrip('$')]
tgt = target if target.endswith('$') else target + '$'
lst = os.path.join(rec, 'only_bisect.txt')

def mismatch(subset):
    open(lst, 'w', encoding='utf-8').write(''.join(n + '\n' for n in [tgt] + subset))
    env = dict(os.environ, KHUX_RW_REPLAY=rec, KHUX_RW_VERIFY='1', KHUX_RW_CAP='0', KHUX_RW_ONLY='@' + lst)
    log = os.path.join(RECOMP, 'out', 'host', 'replay_bisect.log')
    with open(log, 'w') as f:
        subprocess.run([os.path.join(RECOMP, 'out', 'host', 'khux.exe')], cwd=RECOMP, stdout=f,
                       stderr=subprocess.STDOUT, env=env, timeout=600)
    text = open(log, errors='replace').read()
    res = re.findall(r'RW REPLAY .*: (verified|MISMATCH|not compared), live', text)
    return 'MISMATCH' in res, res

ok, res = mismatch(others)
print('all:', res, flush=True)
ok0, res0 = mismatch([])
print('target alone:', res0, flush=True)
if not ok or ok0:
    sys.exit(0)
cand = others
while len(cand) > 1:
    half = cand[:len(cand) // 2]
    m, _ = mismatch(half)
    cand = half if m else cand[len(cand) // 2:]
    print(len(cand), 'left', '(first half)' if m else '(second half)', flush=True)
print('culprit:', cand)
m, r = mismatch(cand)
print('check alone with culprit:', r)
