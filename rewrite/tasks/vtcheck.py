# python vtcheck.py : vtable-range literals in rewrite/src holding a vtable SYMBOL address where objects store symbol + 8
# (a std::function __func / object vptr points 8 past offset-to-top and typeinfo). Found 2026-10-29 twice.
import glob, re, struct, sys
sys.path.insert(0, r'D:\Modding\git\KHUx\recomp')
import paths
from elftools.elf.elffile import ELFFile
so = open(paths.so(), 'rb').read()
e = ELFFile(open(paths.so(), 'rb'))
segs = [(s['p_vaddr'], s['p_filesz'], s['p_offset']) for s in e.iter_segments() if s['p_type'] == 'PT_LOAD']
text = e.get_section_by_name('.text')
t0, t1 = text['sh_addr'], text['sh_addr'] + text['sh_size']
def rd(va):
    for v, sz, off in segs:
        if v <= va < v + sz and va + 4 <= v + sz: return struct.unpack_from('<I', so, off + va - v)[0]
    return None
def is_vtable(x):
    a, b, c = rd(x - 8), rd(x - 4), rd(x)
    return a == 0 and b is not None and not (t0 <= b < t1) and b != 0 and c is not None and (t0 <= (c & ~1) < t1 or c == 0)
bad = 0
for f in sorted(glob.glob(r'D:\Modding\git\KHUx\recomp\rewrite\src\**\*.[ch]pp', recursive=True)):
    if 'gen' in f.replace(chr(92), '/').split('/'): continue
    for n, line in enumerate(open(f, encoding='utf8', errors='replace'), 1):
        # every literal in the vtable range (named VT_* constants and vtables passed inline), unless written "+ 8"
        for m in re.finditer(r'0x(F[5-9A-C][0-9A-Fa-f]{4})\b(?!\s*\+\s*8)', line):
            x = int(m.group(1), 16)
            # the bug this looks for: the vtable symbol (offset-to-top) stored where the object holds symbol + 8
            if not is_vtable(x) and is_vtable(x + 8):
                print(f"{f.replace(chr(92), '/').split('/rewrite/src/')[-1]}:{n} {x:X} "
                      f"(stored vtables point 8 further: {x + 8:X}): {line.strip()[:90]}")
                bad += 1
print(bad, 'suspicious')
