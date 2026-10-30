# python addrcheck.py [FILE ...] : function-address constants named Class_method (or Class_method_x) that do not
# point at a symbol of that class and method (Input/1.0.1 names.txt; Thumb bit allowed). Default: all of rewrite/src
# except gen/. Catches guessed or mistyped callee addresses.
import glob, re, sys

NAMES = r'D:\Modding\git\KHUx\Input\1.0.1\libcocos2dcpp_decompile\names.txt'
sym = {}
for line in open(NAMES, encoding='utf8', errors='replace'):
    m = re.match(r'0x([0-9A-F]{8})\s+(\S+)', line)
    if m and not m.group(2).startswith('j_') and not m.group(2).endswith('_ptr'):
        sym.setdefault(int(m.group(1), 16), m.group(2))
# the identifiers in the mangled names (length-prefixed): a constant's class part is checked only when it is one
idents = set()
for s in sym.values():
    i = 0
    for m in re.finditer(r'(\d+)([A-Za-z_])', s):
        n = int(m.group(1))
        if 2 <= n <= 60:
            idents.add(s[m.start(2):m.start(2) + n])
files = sys.argv[1:] or [f for f in glob.glob(r'D:\Modding\git\KHUx\recomp\rewrite\src\**\*.[ch]pp', recursive=True)
                         if 'gen' not in f.replace(chr(92), '/').split('/')]
bad = 0
for f in files:
    for n, line in enumerate(open(f, encoding='utf8', errors='replace'), 1):
        for m in re.finditer(r'\b([A-Z][A-Za-z0-9]+)_([a-z][A-Za-z0-9]+)\s*=\s*0x([0-9A-Fa-f]{6,7})\b', line):
            cls, meth, a = m.group(1), m.group(2), int(m.group(3), 16) & ~1
            if a < 0x400000 or a >= 0xD00000:
                continue
            s = sym.get(a)
            if s is None:
                continue                      # unnamed (sub_) or a data address: not checked
            if cls not in idents:
                continue
            low = s.lower().replace('_', '')
            special = meth in ('ctor', 'dtor', 'copy', 'construct', 'destruct', 'destroy', 'move') and                 re.search(re.escape(cls) + r'(?:[CD][0-3]E|E[CD][0-3]E)', s)
            if cls in s and (meth.lower() in low or special):
                continue
            print(f"{f.replace(chr(92), '/').split('/rewrite/src/')[-1]}:{n} {cls}_{meth} = {m.group(3)} is {s}")
            bad += 1
print(bad, 'suspicious')
