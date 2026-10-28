import sys, re
sys.path.insert(0, r'C:/Users/Admin/AppData/Local/Temp/claude/d--Modding-git-KHUx/2976a5c7-9c73-4541-9f83-9fe44b6ce564/scratchpad')
sys.argv = [sys.argv[0], 'NUL'] + sys.argv[1:]
exec(open(r'C:/Users/Admin/AppData/Local/Temp/claude/d--Modding-git-KHUx/2976a5c7-9c73-4541-9f83-9fe44b6ce564/scratchpad/vtname.py').read().split('for l in open')[0])
import glob, os
done = set()
for f in glob.glob(r'D:/Modding/git/KHUx/recomp/rewrite/src/**/*.cpp', recursive=True):
    for m in re.finditer(r'RW_FUNCTION\(0x([0-9A-Fa-f]+)', open(f, encoding='utf8', errors='replace').read()):
        done.add(int(m.group(1), 16))
for a in sys.argv[2:]:
    vt = int(a, 16)
    ops = [rd(vt + k) for k in (8, 0xC, 0x10, 0x14, 0x18)]
    print(hex(vt), ' '.join('%X%s' % (o & ~1, '*' if (o & ~1) in done else '') for o in ops))
