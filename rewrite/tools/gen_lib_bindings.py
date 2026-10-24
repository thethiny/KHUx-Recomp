"""Name the library functions the game code calls (rewrite/include/rw/lib.hpp) and use those names in the sources.

python recomp/rewrite/tools/gen_lib_bindings.py [--write]

Every hex constant in the game-code sources (files whose rewrites are game functions, and headers) that is the
exact call address of a library function (libsplit groups other than libc++ / C++ runtime) becomes
`rwlib::<qualified name>`: rwlib::cocos2d::Node::runAction, rwlib::ov_read. Constructors are `ctor` / `ctor_base`,
destructors `dtor` / `dtor_base` / `dtor_delete`; overloads get the mangled parameter list as a suffix. Names the
script cannot read (templates, substitutions) keep their hex value. Without --write: the report only.

rw/rw.hpp includes the header at its end (no source gains a line: RW_REGISTER names its registration
objects by __LINE__). The names have the same values as the literals they replace, so the compiled code must not change: build, then
compare the objects (the change is a rename only). The header is the list of entry points a native build maps to
the real libraries.
"""
import collections, re, sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
RECOMP = HERE.parents[1]
sys.path.insert(0, str(HERE))
import libsplit, unit

SRC = RECOMP / 'rewrite' / 'src'
HEADER = RECOMP / 'rewrite' / 'include' / 'rw' / 'lib.hpp'
SKIP_GROUPS = {'libc++ / C++ runtime'}
KEYWORDS = set('''alignas alignof and asm auto bool break case catch char class const constexpr const_cast continue
decltype default delete do double dynamic_cast else enum explicit export extern false float for friend goto if
inline int long mutable namespace new noexcept not nullptr operator or private protected public register
reinterpret_cast return short signed sizeof static static_assert static_cast struct switch template this throw true
try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor'''.split())
OPS = {'aS': 'op_assign', 'eq': 'op_eq', 'ne': 'op_ne', 'lt': 'op_lt', 'gt': 'op_gt', 'le': 'op_le', 'ge': 'op_ge',
       'pl': 'op_plus', 'mi': 'op_minus', 'ml': 'op_mul', 'dv': 'op_div', 'pL': 'op_plus_assign',
       'mI': 'op_minus_assign', 'mL': 'op_mul_assign', 'dV': 'op_div_assign', 'ix': 'op_index', 'cl': 'op_call',
       'ng': 'op_neg', 'nt': 'op_not', 'nw': 'op_new', 'dl': 'op_delete', 'na': 'op_new_array', 'da': 'op_delete_array'}
SPECIAL = {'C1': 'ctor', 'C2': 'ctor_base', 'C3': 'ctor_alloc', 'D0': 'dtor_delete', 'D1': 'dtor', 'D2': 'dtor_base'}


def parse(mangled):
    """(scope parts, leaf, mangled parameters) of a non-template mangled name; None when it cannot tell."""
    if not mangled.startswith('_Z'):
        return ([], mangled, '') if re.fullmatch(r'[A-Za-z_]\w*', mangled) else None
    s = mangled[2:]
    nested = s.startswith('N')
    if nested:
        s = s[1:]
        while s and s[0] in 'KVr': s = s[1:]                 # cv-qualified member function
    parts, i = [], 0
    if s.startswith('St'): parts.append('std'); i = 2
    while i < len(s):
        if s[i].isdigit():
            n = re.match(r'\d+', s[i:]).group(0); i += len(n)
            parts.append(s[i:i + int(n)]); i += int(n)
        elif s[i:i + 2] in SPECIAL: parts.append(SPECIAL[s[i:i + 2]]); i += 2
        elif s[i:i + 2] in OPS: parts.append(OPS[s[i:i + 2]]); i += 2
        else: break
        if not nested: break
    rest = s[i:]
    if nested:
        if not rest.startswith('E'): return None                # template args, substitutions, ...
        rest = rest[1:]
    if not parts or any(p.startswith('_') and p[1:2].isupper() for p in parts): return None
    return parts[:-1], parts[-1], rest


def ident(x):
    x = re.sub(r'\W', '_', x)
    return x + '_' if x in KEYWORDS else x


def is_game_file(text, group):
    regs = [int(a, 16) & ~1 for a in re.findall(r'RW_FUNCTION(?:_NOVERIFY)?\(\s*(0x[0-9A-Fa-f]+)', text)]
    return not regs or sum(group(a) != 'game' for a in regs) * 2 <= len(regs)


def code_spans(text):
    """Spans of code outside comments and string / char literals."""
    spans, i, start, n = [], 0, 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            spans.append((start, i)); i = text.find('\n', i); i = n if i < 0 else i; start = i
        elif text.startswith('/*', i):
            spans.append((start, i)); j = text.find('*/', i + 2); i = n if j < 0 else j + 2; start = i
        elif c in '"\'':
            spans.append((start, i)); j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == '\\' else 1
            i = j + 1; start = i
        else:
            i += 1
    spans.append((start, n))
    return spans


def main():
    write = '--write' in sys.argv
    idx = unit.load_index()
    groups = libsplit.groups()
    group = lambda a: groups.get(a) or 'game'
    entry = {}                                                   # call address (Thumb bit set) -> mangled name
    for a, e in idx.items():
        a = int(a, 16)
        if group(a) in ('game',) or group(a) in SKIP_GROUPS: continue
        entry[a | (1 if e['mode'] == 'thumb' else 0)] = e['name']
    hexre = re.compile(r'\b0x([0-9A-Fa-f]{6,7})[uU]?\b')
    files = sorted(p for p in SRC.rglob('*.[ch]pp'))
    used = collections.Counter()
    texts = {}
    for f in files:
        t = f.read_text(encoding='utf-8', errors='replace')
        if not is_game_file(t, group): continue
        texts[f] = t
        for a, b in code_spans(t):
            for m in hexre.finditer(t, a, b):
                v = int(m.group(1), 16)
                if v in entry: used[v] += 1
    # names
    parsed, unreadable = {}, []
    for v in used:
        p = parse(entry[v])
        if p is None: unreadable.append(entry[v]); continue
        parsed[v] = p
    by_leaf = collections.defaultdict(list)
    for v, (scope, leaf, params) in parsed.items(): by_leaf[(tuple(scope), leaf)].append(v)
    scopes = {tuple(s[:k]) for s, _, _ in parsed.values() for k in range(1, len(s) + 1)}
    name = {}
    for (scope, leaf), vs in by_leaf.items():
        for v in vs:
            l = ident(leaf)
            if len(vs) > 1: l += '__' + ident(parsed[v][2] or 'v')
            if scope + (leaf,) in scopes: l += '_fn'                 # a leaf named like a nested scope
            name[v] = '::'.join(['rwlib'] + [ident(x) for x in scope] + [l])
    dup = [n for n, c in collections.Counter(name.values()).items() if c > 1]
    for v in list(name):
        if name[v] in dup: name[v] += f'_{v:X}'
    # header
    tree = collections.defaultdict(list)
    for v, n in name.items():
        parts = n.split('::')
        tree[tuple(parts[1:-1])].append((parts[-1], v))
    out = ['// Generated by rewrite/tools/gen_lib_bindings.py - do not edit.',
           '// The library functions the game code calls, by name, at their call addresses in the original image',
           '// (Thumb bit set). Recomp calls them there (lifted code); a native build maps the same names to the',
           '// real libraries.',
           '#pragma once', '', '#include <cstdint>', '']
    for scope in sorted(tree):
        ns = '::'.join(['rwlib'] + list(scope))
        out.append(f'namespace {ns} {{')
        for leaf, v in sorted(tree[scope]):
            out.append(f'inline constexpr uint32_t {leaf} = 0x{v:X};   // {entry[v]}')
        out.append('}')
        out.append('')
    header = '\n'.join(out)
    # sources
    changed, reps = 0, 0
    for f, t in texts.items():
        pieces, last, n = [], 0, 0
        for a, b in code_spans(t):
            for m in hexre.finditer(t, a, b):
                v = int(m.group(1), 16)
                line = t[t.rfind('\n', 0, m.start()) + 1:m.start()]
                if v in name and 'RW_FUNCTION' not in line and 'RW_REGISTER' not in line:   # registrations keep literals
                    pieces.append(t[last:m.start()]); pieces.append(name[v]); last = m.end(); n += 1
        if not n: continue
        new = ''.join(pieces) + t[last:]
        changed += 1; reps += n
        if write:
            with open(f, 'w', encoding='utf-8', newline='') as fh: fh.write(new)
    if write:
        with open(HEADER, 'w', encoding='utf-8', newline='\n') as fh: fh.write(header + '\n')
    print(f'{len(used)} library call targets used by game code; {len(name)} named, {len(unreadable)} kept as hex')
    print(f'{reps} literals replaced in {changed} files' + ('' if write else ' (dry run)'))
    by_ns = collections.Counter(n.split('::')[1] if n.count('::') > 1 else '(C functions)' for n in name.values())
    print('top scopes:', ', '.join(f'{k} {c}' for k, c in by_ns.most_common(8)))
    for u in unreadable[:15]: print('  kept:', u)


if __name__ == '__main__':
    main()
