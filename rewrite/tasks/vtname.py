import struct, sys, subprocess, re
sys.path.insert(0, r'D:\Modding\git\KHUx\recomp')
import paths
from elftools.elf.elffile import ELFFile
so = open(paths.so(), 'rb').read()
e = ELFFile(open(paths.so(), 'rb'))
segs = [(s['p_vaddr'], s['p_filesz'], s['p_offset']) for s in e.iter_segments() if s['p_type'] == 'PT_LOAD']
def rd(va):
    for v, sz, off in segs:
        if v <= va < v + sz: return struct.unpack_from('<I', so, off + va - v)[0]
    return None
def cstr(va):
    for v, sz, off in segs:
        if v <= va < v + sz:
            o = off + va - v; return so[o:so.index(b'\0', o)].decode('latin1')
# relocations for typeinfo pointers (R_ARM_ABS32 / RELATIVE)
rel = {}
from elftools.elf.relocation import RelocationSection
for s in e.iter_sections():
    if isinstance(s, RelocationSection):
        symtab = e.get_section(s['sh_link'])
        for r in s.iter_relocations():
            if r['r_info_sym']:
                rel[r['r_offset']] = symtab.get_symbol(r['r_info_sym']).name
def name_of(vt):
    ti = rd(vt - 4)
    if not ti: return rel.get(vt - 4, '?')
    nm = rd(ti + 4)
    s = cstr(nm) if nm else None
    return s or '?'
def demangle(s):
    try:
        return subprocess.run(['c++filt', '-_' if False else '', s], capture_output=True, text=True).stdout.strip()
    except Exception:
        return s
for l in open(sys.argv[1]):
    p = l.split('\t'); vt = re.search(r'vt=([0-9A-F]+)', l)
    if not vt: print(p[0], '?'); continue
    n = name_of(int(vt.group(1), 16))
    m = re.search(r'Z(N?\d+\w+?E)?.*?(\$_\d+|Ul[^E]*E_?\d*)', n)
    print(p[0], vt.group(1), n[:160])
