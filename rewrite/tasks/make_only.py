# python make_only.py OUT ADDR... : a KHUX_RW_ONLY list (name$ per line) of the rewrites at these addresses
import glob, re, sys
want = {int(a, 16) for a in sys.argv[2:]}
names = {}
for f in glob.glob(r'D:\Modding\git\KHUx\recomp\rewrite\src\**\*.cpp', recursive=True):
    t = open(f, encoding='utf-8', errors='replace').read()
    for m in re.finditer(r'RW_FUNCTION(?:_NOVERIFY)?\(\s*0x([0-9A-Fa-f]+),\s*"((?:[^"\\]|\\.)*)"', t):
        a = int(m.group(1), 16)
        if a in want: names[a] = m.group(2).replace('\\"', '"')
missing = sorted(want - set(names))
if missing: print('missing:', ' '.join(f'{a:X}' for a in missing))
with open(sys.argv[1], 'w', encoding='utf-8') as f:
    for a in sorted(names): f.write(names[a] + '$\n')
print(len(names), 'names')
