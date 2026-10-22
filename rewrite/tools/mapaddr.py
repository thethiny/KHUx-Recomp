"""Name rewrite.dll offsets (the "rewrite.dll+OFF" frames of RW FAULT / crash lines) from out/rewrite/rewrite.map.

python recomp/rewrite/tools/mapaddr.py OFF [OFF ...]   (hex)
"""
import bisect
import re
import sys
from pathlib import Path
base = None; syms = []
for l in open(r'D:\Modding\git\KHUx\recomp\out\rewrite\rewrite.map', errors='replace'):
    m = re.search(r'Preferred load address is ([0-9a-fA-F]+)', l)
    if m: base = int(m.group(1), 16)
    p = l.split()
    if len(p) >= 3 and re.fullmatch(r'[0-9a-fA-F]{4}:[0-9a-fA-F]{8}', p[0]) and re.fullmatch(r'[0-9a-fA-F]{16}', p[2]):
        syms.append((int(p[2], 16) - base, p[1]))
syms.sort(); keys = [s[0] for s in syms]
for a in sys.argv[1:]:
    o = int(a, 16); i = bisect.bisect_right(keys, o) - 1
    print(a, syms[i][1], '+%X' % (o - syms[i][0]))
