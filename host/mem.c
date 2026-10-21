/* Guest memory and the ELF loader: the library at its original addresses (no relocation base), page 0 left
 * unmapped, imports bound to slot addresses. Same layout and binding as emu/core.py. */
#include "host.h"

uint8_t *G;
uint32_t *init_array;
int init_count;
uint32_t exidx_addr, exidx_count;

/* The 3 GB guest heap is only reserved: its first access to each 1 MB chunk commits that chunk (zero-filled, as a
 * committed page would be). Committing all of it up front charged 3 GB against the system commit limit for the
 * whole session (other programs could not start) although the game touches a few hundred MB. */
#define HEAP_CHUNK 0x100000u
static void heap_chunk_committed(uint32_t chunk);
static LONG CALLBACK heap_fault(EXCEPTION_POINTERS *e) {
    EXCEPTION_RECORD *r = e->ExceptionRecord;
    if (r->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || r->NumberParameters < 2) return EXCEPTION_CONTINUE_SEARCH;
    uint8_t *a = (uint8_t *)r->ExceptionInformation[1];
    if (a < G + HEAP_BASE || a >= G + HEAP_BASE + HEAP_SIZE) return EXCEPTION_CONTINUE_SEARCH;
    uint32_t chunk = (uint32_t)(a - G) & ~(HEAP_CHUNK - 1);
    if (!VirtualAlloc(G + chunk, HEAP_CHUNK, MEM_COMMIT, PAGE_READWRITE)) return EXCEPTION_CONTINUE_SEARCH;
    heap_chunk_committed(chunk);
    return EXCEPTION_CONTINUE_EXECUTION;
}

void mem_init(void) {
    G = VirtualAlloc(NULL, (SIZE_T)1 << 32, MEM_RESERVE, PAGE_READWRITE);
    if (!G) fatal("cannot reserve the 4 GB guest space (error %lu)", GetLastError());
    mem_commit(IMAGE_LO, IMAGE_HI - IMAGE_LO);
    mem_commit(TRAP_BASE, TRAP_SIZE);
    AddVectoredExceptionHandler(1, heap_fault);
    mem_commit(HEAP_BASE, HEAP_CHUNK);
    mem_commit(STACK_BASE, STACK_SIZE);
    mem_commit(KUSER_PAGE, 0x1000);
    gw32(KUSER_PAGE + 0xFFC, 5);                         /* __kuser_helper_version */
}

void mem_commit(uint32_t lo, uint32_t size) {
    if (!VirtualAlloc(G + lo, size, MEM_COMMIT, PAGE_READWRITE))
        fatal("cannot commit guest 0x%08X+0x%X (error %lu)", lo, size, GetLastError());
}

/* ---------------------------------------------------------------- ELF32 */
#pragma pack(push, 1)
typedef struct { uint8_t ident[16]; uint16_t type, machine; uint32_t version, entry, phoff, shoff, flags;
                 uint16_t ehsize, phentsize, phnum, shentsize, shnum, shstrndx; } Ehdr;
typedef struct { uint32_t type, offset, vaddr, paddr, filesz, memsz, flags, align; } Phdr;
typedef struct { uint32_t name, value, size; uint8_t info, other; uint16_t shndx; } Sym;
typedef struct { uint32_t offset, info; } Rel;
typedef struct { int32_t tag; uint32_t val; } Dyn;
#pragma pack(pop)

#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_ARM_EXIDX 0x70000001
#define DT_NULL 0
#define DT_PLTRELSZ 2
#define DT_STRTAB 5
#define DT_SYMTAB 6
#define DT_REL 17
#define DT_RELSZ 18
#define DT_JMPREL 23
#define DT_INIT_ARRAY 25
#define DT_INIT_ARRAYSZ 27
#define DT_HASH 4
#define R_ARM_ABS32 2
#define R_ARM_GLOB_DAT 21
#define R_ARM_JUMP_SLOT 22
#define R_ARM_RELATIVE 23

/* defined dynamic symbols: open-addressing table name -> value */
#define SYM_CAP (1u << 17)
static struct { const char *name; uint32_t value; } symtab[SYM_CAP];

static uint32_t hash_str(const char *s) {
    uint32_t h = 2166136261u;
    while (*s) h = (h ^ (uint8_t)*s++) * 16777619u;
    return h;
}

static void sym_add(const char *name, uint32_t v) {
    uint32_t k = hash_str(name) & (SYM_CAP - 1);
    while (symtab[k].name) {
        if (!strcmp(symtab[k].name, name)) return;
        k = (k + 1) & (SYM_CAP - 1);
    }
    symtab[k].name = name;
    symtab[k].value = v;
}

uint32_t sym(const char *name) {
    uint32_t k = hash_str(name) & (SYM_CAP - 1);
    while (symtab[k].name) {
        if (!strcmp(symtab[k].name, name)) return symtab[k].value;
        k = (k + 1) & (SYM_CAP - 1);
    }
    return 0;
}

static void relocate(const Rel *rel, uint32_t n, const Sym *syms, const char *strs) {
    for (uint32_t k = 0; k < n; k++) {
        uint32_t type = rel[k].info & 0xFF, si = rel[k].info >> 8, off = rel[k].offset, v;
        if (type == R_ARM_RELATIVE) continue;            /* loaded at the original addresses: base 0 */
        const Sym *s = &syms[si];
        const char *name = strs + s->name;
        if (s->shndx != 0) v = s->value;
        else if ((s->info & 0xF) == 2 /* STT_FUNC */ || type == R_ARM_JUMP_SLOT) v = slot_for(name);
        else v = data_import(name);
        if (type == R_ARM_ABS32) v += g32(off);
        else if (type != R_ARM_GLOB_DAT && type != R_ARM_JUMP_SLOT)
            fatal("unsupported relocation type %u at 0x%08X", type, off);
        gw32(off, v);
    }
}

/* the file bytes behind a virtual address (through the PT_LOAD segments) */
static const void *file_at(const uint8_t *raw, long size, const Ehdr *eh, uint32_t va) {
    const Phdr *ph = (const Phdr *)(raw + eh->phoff);
    for (int k = 0; k < eh->phnum; k++)
        if (ph[k].type == PT_LOAD && va >= ph[k].vaddr && va < ph[k].vaddr + ph[k].filesz &&
            ph[k].offset + (va - ph[k].vaddr) < (uint32_t)size)
            return raw + ph[k].offset + (va - ph[k].vaddr);
    fatal("ELF: address 0x%08X is not in the file", va);
}

void elf_load(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) fatal("cannot open the game library %s", path);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *raw = malloc((size_t)size);
    if (!raw || fread(raw, 1, (size_t)size, f) != (size_t)size) fatal("cannot read %s", path);
    fclose(f);
    const Ehdr *eh = (const Ehdr *)raw;
    if (memcmp(eh->ident, "\x7f" "ELF", 4) || eh->ident[4] != 1 || eh->machine != 40)
        fatal("%s is not a 32-bit ARM ELF", path);
    const Phdr *ph = (const Phdr *)(raw + eh->phoff);
    uint32_t dyn_addr = 0;
    for (int k = 0; k < eh->phnum; k++) {
        if (ph[k].type == PT_LOAD) {
            uint32_t va = ph[k].vaddr, lo = va < IMAGE_LO ? IMAGE_LO : va;     /* page 0 stays unmapped */
            if (va + ph[k].memsz > IMAGE_HI) fatal("segment 0x%08X+0x%X outside the image", va, ph[k].memsz);
            if (va + ph[k].filesz > lo) memcpy(GP(lo), raw + ph[k].offset + (lo - va), va + ph[k].filesz - lo);
        } else if (ph[k].type == PT_DYNAMIC) {
            dyn_addr = ph[k].vaddr;
        } else if (ph[k].type == PT_ARM_EXIDX) {
            exidx_addr = ph[k].vaddr;
            exidx_count = ph[k].memsz / 8;
        }
    }
    if (!dyn_addr) fatal("%s has no dynamic segment", path);
    uint32_t symtab_a = 0, strtab_a = 0, rel = 0, relsz = 0, jmprel = 0, pltrelsz = 0, ia = 0, iasz = 0, hash = 0;
    for (const Dyn *d = GP(dyn_addr); d->tag != DT_NULL; d++) {
        switch (d->tag) {
        case DT_SYMTAB: symtab_a = d->val; break;
        case DT_STRTAB: strtab_a = d->val; break;
        case DT_REL: rel = d->val; break;
        case DT_RELSZ: relsz = d->val; break;
        case DT_JMPREL: jmprel = d->val; break;
        case DT_PLTRELSZ: pltrelsz = d->val; break;
        case DT_INIT_ARRAY: ia = d->val; break;
        case DT_INIT_ARRAYSZ: iasz = d->val; break;
        case DT_HASH: hash = d->val; break;
        }
    }
    if (!symtab_a || !strtab_a || !hash) fatal("%s: incomplete dynamic section", path);
    /* the dynamic symbol table starts inside page 0, which is not mapped: read the tables from the file
     * (the raw buffer stays allocated: symbol names point into it) */
    const Sym *syms = file_at(raw, size, eh, symtab_a);
    const char *strs = file_at(raw, size, eh, strtab_a);
    uint32_t nsyms = *(const uint32_t *)file_at(raw, size, eh, hash + 4);    /* nchain = number of symbols */
    for (uint32_t k = 1; k < nsyms; k++)
        if (syms[k].shndx != 0 && syms[k].name) sym_add(strs + syms[k].name, syms[k].value);
    relocate(file_at(raw, size, eh, rel), relsz / 8, syms, strs);
    relocate(file_at(raw, size, eh, jmprel), pltrelsz / 8, syms, strs);
    init_count = (int)(iasz / 4);
    init_array = malloc(sizeof(uint32_t) * (init_count + 1));
    for (int k = 0; k < init_count; k++) init_array[k] = g32(ia + 4u * k);
    hlog("loaded %s: %u symbols, %d constructors", path, nsyms, init_count);
}

/* ---------------------------------------------------------------- data imports (as emu/libc.py) */
#define DATA_CAP 128
static struct { char name[64]; uint32_t addr; } data_imports[DATA_CAP];
static int n_data;
uint32_t sl_iid(const char *itf);       /* sl.c */

uint32_t data_import(const char *name) {
    for (int k = 0; k < n_data; k++) if (!strcmp(data_imports[k].name, name)) return data_imports[k].addr;
    uint32_t a;
    if (!strcmp(name, "__stack_chk_guard")) {
        a = gzalloc(4);
        gw32(a, 0xC0FFEE00u);
    } else if (!strcmp(name, "__sF")) {
        a = gzalloc(84 * 3);                             /* bionic FILE is 84 bytes: stdin, stdout, stderr */
    } else if (!strcmp(name, "_ctype_")) {
        /* bionic: a pointer to the table one before index 0 (index -1 = EOF); BSD flag bits */
        uint8_t tbl[260] = {0};
        for (int ch = 0; ch < 128; ch++) {
            uint8_t fl = 0;
            if (ch >= 'A' && ch <= 'Z') fl |= 1;
            if (ch >= 'a' && ch <= 'z') fl |= 2;
            if (ch >= '0' && ch <= '9') fl |= 4;
            if (ch == ' ' || (ch >= 9 && ch <= 13)) fl |= 8;
            if (ch > 32 && ch < 127 && !(fl & 7)) fl |= 0x10;
            if (ch < 32 || ch == 127) fl |= 0x20;
            if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) fl |= 0x40;
            if (ch == 32) fl |= 0x80;
            tbl[ch + 1] = fl;
        }
        uint32_t t = galloc_bytes(tbl, sizeof(tbl));
        a = gzalloc(4);
        gw32(a, t);
    } else if (!strcmp(name, "_tolower_tab_") || !strcmp(name, "_toupper_tab_")) {
        int lower = name[1] == 't' && name[3] == 'l';
        uint16_t tbl[257];
        tbl[0] = 0xFFFF;
        for (int ch = 0; ch < 256; ch++) {
            int v = ch;
            if (lower && ch >= 'A' && ch <= 'Z') v = ch + 32;
            if (!lower && ch >= 'a' && ch <= 'z') v = ch - 32;
            tbl[ch + 1] = (uint16_t)v;
        }
        uint32_t t = galloc_bytes(tbl, sizeof(tbl));
        a = gzalloc(4);
        gw32(a, t);
    } else if (!strncmp(name, "SL_IID_", 7)) {
        a = gzalloc(4);
        gw32(a, sl_iid(name + 7));
    } else {
        hlog("?? unknown data import %s, giving 256 zero bytes", name);
        a = gzalloc(256);
    }
    if (n_data >= DATA_CAP) fatal("too many data imports");
    strncpy(data_imports[n_data].name, name, 63);
    data_imports[n_data++].addr = a;
    return a;
}

/* ---------------------------------------------------------------- M3 replay sandbox: page capture
 * KHUX_RW_CAPTURE (rewrite.dll asks through RwHost.track): for one shadow run every committed guest page is made
 * inaccessible; the first access to each (any thread, lifted code or the host's own imports) saves the page as it
 * was and opens it again. The saved pages are the call's input memory: rewrite.dll writes them with the call's
 * registers and imports to a record that the replay mode (KHUX_RW_REPLAY) runs again without the game.
 * Heap chunks committed during the capture start zero-filled: they are listed as zero chunks. */
static volatile LONG trk_on;
static uint8_t *trk_bits;                            /* 1 bit per guest page: inaccessible, not yet saved */
static CRITICAL_SECTION trk_cs;
static uint32_t *trk_addr; static uint8_t *trk_data; static int trk_n, trk_cap;
static uint32_t (*trk_regions)[2]; static int trk_nreg, trk_regcap;   /* the protected ranges (guest lo, size) */
static uint8_t heap_committed[HEAP_SIZE / HEAP_CHUNK];        /* chunks committed so far */
static uint32_t trk_zero[4096]; static int trk_nzero;
static DWORD trk_tid; static int trk_foreign;                /* the capturing thread; pages first touched by others */

static LONG CALLBACK trk_fault(EXCEPTION_POINTERS *e) {
    EXCEPTION_RECORD *r = e->ExceptionRecord;
    if (!trk_on || r->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || r->NumberParameters < 2) return EXCEPTION_CONTINUE_SEARCH;
    uint8_t *a = (uint8_t *)r->ExceptionInformation[1];
    if (a < G || a >= G + ((SIZE_T)1 << 32)) return EXCEPTION_CONTINUE_SEARCH;
    uint32_t page = (uint32_t)(a - G) >> 12;
    EnterCriticalSection(&trk_cs);
    /* an access that straddles two pages can report the first (open) one: the closed one is the next */
    if (!(trk_bits[page >> 3] & (1u << (page & 7))) && page + 1 < (1u << 20) &&
        (trk_bits[(page + 1) >> 3] & (1u << ((page + 1) & 7))))
        page++;
    if (!(trk_bits[page >> 3] & (1u << (page & 7)))) {
        LeaveCriticalSection(&trk_cs);
        /* another thread opened it meanwhile: retry; not ours at all: the next handler */
        MEMORY_BASIC_INFORMATION mi;
        if (VirtualQuery(a, &mi, sizeof mi) && mi.State == MEM_COMMIT && mi.Protect == PAGE_READWRITE)
            return EXCEPTION_CONTINUE_EXECUTION;
        return EXCEPTION_CONTINUE_SEARCH;
    }
    DWORD old;
    VirtualProtect(G + ((size_t)page << 12), 0x1000, PAGE_READWRITE, &old);
    if (trk_n == trk_cap) {
        trk_cap = trk_cap ? trk_cap * 2 : 256;
        trk_addr = realloc(trk_addr, sizeof(uint32_t) * (size_t)trk_cap);
        trk_data = realloc(trk_data, (size_t)0x1000 * (size_t)trk_cap);
    }
    if (GetCurrentThreadId() != trk_tid) trk_foreign++;
    trk_addr[trk_n] = page << 12;
    memcpy(trk_data + (size_t)0x1000 * (size_t)trk_n, G + ((size_t)page << 12), 0x1000);
    trk_n++;
    trk_bits[page >> 3] &= (uint8_t)~(1u << (page & 7));
    LeaveCriticalSection(&trk_cs);
    return EXCEPTION_CONTINUE_EXECUTION;
}

static void trk_protect(uint32_t lo, uint32_t size) {
    DWORD old;
    if (!size || !VirtualProtect(G + lo, size, PAGE_NOACCESS, &old)) return;
    if (trk_nreg == trk_regcap) {
        trk_regcap = trk_regcap ? trk_regcap * 2 : 256;
        trk_regions = realloc(trk_regions, sizeof(uint32_t[2]) * (size_t)trk_regcap);
    }
    trk_regions[trk_nreg][0] = lo; trk_regions[trk_nreg][1] = size; trk_nreg++;
    for (uint32_t p = lo >> 12; p < (lo + size) >> 12; p++) trk_bits[p >> 3] |= (uint8_t)(1u << (p & 7));
}

/* 1: start capturing (every committed guest page inaccessible); 0: stop (all accessible again) */
void shadow_capture_park(int on);
int mem_track(int on) {
    static int init;
    if (!init) {
        init = 1;
        InitializeCriticalSection(&trk_cs);
        trk_bits = calloc((size_t)1 << 17, 1);
        AddVectoredExceptionHandler(1, trk_fault);      /* first: before heap_fault */
    }
    if (on) {
        if (trk_on) return -1;                          /* one capture at a time */
        shadow_capture_park(1);
        EnterCriticalSection(&trk_cs);
        trk_n = 0; trk_nreg = 0; trk_nzero = 0; trk_foreign = 0; trk_tid = GetCurrentThreadId();
        /* on before the first page closes: another thread's access to a closed page waits in trk_fault for the
         * lock, then finds its page saved and opened */
        InterlockedExchange(&trk_on, 1);
        /* the committed runs of the guest space (the heap's chunks are listed by heap_fault) */
        for (uint8_t *p = G; p < G + ((SIZE_T)1 << 32);) {
            MEMORY_BASIC_INFORMATION mi;
            if (!VirtualQuery(p, &mi, sizeof mi)) break;
            uint8_t *end = (uint8_t *)mi.BaseAddress + mi.RegionSize;
            uint32_t lo = (uint32_t)((uint8_t *)mi.BaseAddress - G);
            if (mi.State == MEM_COMMIT && mi.Protect == PAGE_READWRITE && !(lo >= TRAP_BASE && lo < TRAP_BASE + TRAP_SIZE))
                trk_protect(lo, (uint32_t)mi.RegionSize);
            p = end;
        }
        LeaveCriticalSection(&trk_cs);
    } else {
        EnterCriticalSection(&trk_cs);
        DWORD old;
        for (int k = 0; k < trk_nreg; k++) {
            VirtualProtect(G + trk_regions[k][0], trk_regions[k][1], PAGE_READWRITE, &old);
            for (uint32_t p = trk_regions[k][0] >> 12; p < (trk_regions[k][0] + trk_regions[k][1]) >> 12; p++)
                trk_bits[p >> 3] &= (uint8_t)~(1u << (p & 7));
        }
        InterlockedExchange(&trk_on, 0);                /* off once every page is open again */
        LeaveCriticalSection(&trk_cs);
        shadow_capture_park(0);
        return trk_foreign;                             /* stop: how many pages another thread touched first */
    }
    return trk_n;
}
/* the pages saved by the last capture, and the heap chunks committed (zero-filled) during it */
int mem_track_pages(const uint32_t **addrs, const uint8_t **data) { *addrs = trk_addr; *data = trk_data; return trk_n; }
int mem_track_zero(const uint32_t **chunks) { *chunks = trk_zero; return trk_nzero; }
/* replay: put guest memory in a page's state (committing it first when needed) */
void mem_put(uint32_t addr, const void *bytes, uint32_t size) {
    VirtualAlloc(G + addr, size, MEM_COMMIT, PAGE_READWRITE);
    memcpy(G + addr, bytes, size);
}
static void heap_chunk_committed(uint32_t chunk) {
    uint32_t k = (chunk - HEAP_BASE) / HEAP_CHUNK;
    if (heap_committed[k]) return;
    heap_committed[k] = 1;
    if (trk_on && trk_nzero < 4096) trk_zero[trk_nzero++] = chunk;    /* zero-filled: its state before the call */
}
