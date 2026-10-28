# for each to-do body: the gen thunk that forwards to it, the __func vtable holding that thunk, and its thunk name
import glob, re, struct, sys
sys.path.insert(0, r'D:\Modding\git\KHUx\recomp')
import paths
from elftools.elf.elffile import ELFFile
so = open(paths.so(), 'rb').read()
e = ELFFile(open(paths.so(), 'rb'))
segs = [(s['p_vaddr'], s['p_filesz'], s['p_offset']) for s in e.iter_segments() if s['p_type'] == 'PT_LOAD']
todo = [int(l.split('\t')[0], 16) for l in open(sys.argv[1])]
fwd = {}   # body -> [thunk addr]
for f in glob.glob(r'D:\Modding\git\KHUx\recomp\rewrite\src\gen\*.cpp'):
    t = open(f, encoding='utf-8', errors='replace').read()
    for m in re.finditer(r'static \w+ (\w+)\([^)]*\) \{\r?\n(.*?)\r?\n\}\r?\nRW_FUNCTION\(0x([0-9A-F]+)', t, re.S):
        for c in re.findall(r'rw::call\(0x([0-9A-F]+), ', m.group(2)):
            fwd.setdefault(int(c, 16) & ~1, []).append(int(m.group(3), 16))
def refs(v):
    out = []
    pat = struct.pack('<I', v)
    for va, sz, off in segs:
        i = so.find(pat, off, off + sz)
        while i >= 0:
            out.append(va + i - off); i = so.find(pat, i + 4, off + sz)
    return out
names = {}
for f in glob.glob(r'D:\Modding\git\KHUx\recomp\rewrite\src\**\*thunks*.cpp', recursive=True):
    t = open(f, encoding='utf-8', errors='replace').read()
    for m in re.finditer(r'// (.+)\r?\nusing \w+ = thunk::\w+<0x([0-9A-F]+)', t):
        names[int(m.group(2), 16)] = m.group(1)
for a in todo:
    th = fwd.get(a, []) + [a]
    vts = []
    for t in th:
        for r in refs(t | 1):
            vts.append(r - 0x18)
    nm = [names.get(v, '?') for v in vts]
    print(f'{a:X}\tthunk={",".join("%X" % x for x in th)}\tvt={",".join("%X" % v for v in vts)}\t{" | ".join(nm)}')
