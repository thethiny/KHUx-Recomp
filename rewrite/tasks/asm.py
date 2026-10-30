# python asm.py ADDR [ADDR ...] : the lifted function at ADDR as "ADDR insn [-> call]" lines (out/lift_all), up to
# the end of its lifted C function; pc-relative literal loads show the loaded word
import glob, re, struct, sys
sys.path.insert(0, r'D:\Modding\git\KHUx\recomp')
import paths
from elftools.elf.elffile import ELFFile
so = open(paths.so(), 'rb').read()
segs = [(s['p_vaddr'], s['p_filesz'], s['p_offset']) for s in ELFFile(open(paths.so(), 'rb')).iter_segments() if s['p_type'] == 'PT_LOAD']
def rd(va):
    for v, sz, off in segs:
        if v <= va < v + sz: return struct.unpack_from('<I', so, off + va - v)[0]
files = sorted(glob.glob(r'D:\Modding\git\KHUx\recomp\out\lift_all\*.c'))
for a in sys.argv[1:]:
    tag = 'L_%X:' % int(a, 16)
    for f in files:
        with open(f, encoding='utf8', errors='replace') as fh:
            lines = None
            for line in fh:
                if lines is None:
                    if line.startswith(tag): lines = [line]
                    continue
                if line.startswith('}'): break
                lines.append(line)
        if lines is not None: break
    else:
        print('not found', a); continue
    print('==', a)
    for line in lines:
        m = re.match(r'L_([0-9A-F]+): /\* (.*?) \*/', line)
        if not m: continue
        pc, ins = int(m.group(1), 16), m.group(2)
        extra = ''
        c = re.search(r'rt_call\(c, 0x([0-9A-F]+)u\)', line)
        if c: extra = ' -> ' + c.group(1)
        l = re.match(r'ldr r\d, \[pc, #0x([0-9a-f]+)\]', ins)
        if l:
            w = rd(((pc + 4) & ~3) + int(l.group(1), 16))
            extra = ' = %08X' % w + (' (%g)' % struct.unpack('<f', struct.pack('<I', w))[0] if 0x30000000 < w < 0x50000000 or 0xB0000000 < w < 0xD0000000 else '')
        print('%X %s%s' % (pc, ins, extra))
