# game functions (libsplit) not rewritten and not IDA chunks of a rewritten parent
import glob, re, sys, collections
sys.path.insert(0, r'D:\Modding\git\KHUx\recomp\rewrite\tools')
import libsplit, unit
g = libsplit.groups()
idx = unit.load_index(); funcs = idx.get('functions', idx)
name = {int(k, 16): v['name'] for k, v in funcs.items() if isinstance(v, dict) and 'name' in v}
size = {int(k, 16): v.get('size', 0) for k, v in funcs.items() if isinstance(v, dict)}
rw = set()
for f in glob.glob(r'D:\Modding\git\KHUx\recomp\rewrite\src\**\*.cpp', recursive=True):
    for m in re.finditer(r'RW_FUNCTION(?:_NOVERIFY)?\(0x([0-9A-Fa-f]+)', open(f, encoding='utf-8', errors='replace').read()):
        rw.add(int(m.group(1), 16))
chunks = {}
for l in open(r'D:\Modding\git\KHUx\recomp\rewrite\tasks\chunks.tsv'):
    m = re.match(r'([0-9A-Fa-f]+)\t(.*)', l)
    if m: chunks[int(m.group(1), 16)] = [int(x, 16) for x in re.findall(r'[0-9A-Fa-f]{5,}', m.group(2))]
todo = []
for a, lib in g.items():
    if lib is not None or a in rw: continue
    def covered(x, seen):
        if x in rw: return True
        if x in seen or x not in chunks: return False
        seen.add(x)
        return any(covered(p, seen) for p in chunks[x])
    if a in chunks and covered(a, set()): continue
    todo.append(a)
todo.sort()
print('game', sum(1 for v in g.values() if v is None), 'todo', len(todo), 'bytes', sum(size.get(a, 0) for a in todo))
with open(sys.argv[1], 'w') as f:
    for a in todo: f.write(f'{a:X}\t{size.get(a,0)}\t{name.get(a,"")}\n')
