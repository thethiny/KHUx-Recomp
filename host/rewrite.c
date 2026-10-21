/* M3: load the rewrite (out/rewrite/rewrite.dll, built from recomp/rewrite) and point lift.dll's override slots at
 * its functions, so rewritten C++ runs instead of the lifted code at those guest addresses.
 *   KHUX_REWRITE=0     run only the lifted code
 *   KHUX_RW_VERIFY=1   install every rewritten function and shadow-run each call against the lifted original
 *                      (a mismatch keeps the lifted result); results go to out/rewrite/stats/<time>.tsv, which
 *                      rewrite/tools/merge_stats.py folds into rewrite/verified.txt
 * Without KHUX_RW_VERIFY only the functions listed in rewrite/verified.txt are installed.
 */
#include "host.h"

typedef void (*LiftFn)(CPU *c);
typedef struct RwHost {
    int version;
    void (*run)(CPU *c, uint32_t target);
    void (*undo)(CPU *c);
    LiftFn *override_slots;
    uint32_t (*guest_call)(CPU *c, uint32_t fn, int n, const uint32_t *args);
    uint64_t (*guest_call64)(CPU *c, uint32_t fn, int n, const uint32_t *args);
    void (*log)(const char *line);
    int verify;
    const char *stats_path;
    const char *verified_path;
    uint64_t (*guest_call_exact)(CPU *c, uint32_t fn, int n, const uint32_t *args);
    int (*shadow)(int phase);
    void (*debug)(char *out, int cap);
    uint32_t ret_magic;                                   /* lr of host -> guest calls */
    GTrack *gtrack;                                       /* rt.h stale stack tracking, shared with lift.dll */
    const volatile int *host_call_returns;                /* 1 while the host's calls return (constructors) */
    /* replay sandbox (version 10): page capture, import records, guest memory writes */
    int (*track)(int on);
    int (*track_pages)(const uint32_t **addrs, const uint8_t **data);
    int (*track_zero)(const uint32_t **chunks);
    size_t (*imports_export)(uint8_t **out);
    int (*imports_import)(const uint8_t *b, size_t n);
    void (*put)(uint32_t addr, const void *bytes, uint32_t size);
} RwHost;
int mem_track(int on);
int mem_track_pages(const uint32_t **addrs, const uint8_t **data);
int mem_track_zero(const uint32_t **chunks);
void mem_put(uint32_t addr, const void *bytes, uint32_t size);
size_t import_shadow_export(uint8_t **out);
int import_shadow_import(const uint8_t *b, size_t n);
static int (*rw_replay_fn)(CPU *c, const char *dir);
/* KHUX_RW_REPLAY: run the recorded calls in a directory against their rewrites (rewrite.dll rw_replay) */
int rewrite_replay(CPU *c, const char *dir) {
    if (!rw_replay_fn) fatal("rewrite.dll has no rw_replay (rebuild it)");
    return rw_replay_fn(c, dir);
}

static RwHost host;
/* the host's own calls into guest code normally are not shadow-run (a thread entry never returns); the
 * constructors of .init_array return, so main.c sets this around them and they are verified like any call */
volatile int rewrite_host_call_returns;
/* rt.h GTrack: the host's own guest stores (imports writing guest memory) feed the shared tracker too */
EXPORT GTrack *rt_gtrack;
static const char *(*rw_where_fn)(void);   /* crash reports: what the verifier is running */

/* a prefix list from the environment: inline, or "@file" (one prefix per line, for long lists) */
static const char *read_list(const char *v) {
    if (!v || v[0] != '@') return v;
    FILE *f = fopen(v + 1, "rb");
    if (!f) fatal("cannot read %s", v + 1);
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char *)malloc((size_t)n + 2);
    buf[0] = 10;                                          /* marks a line list (see name_listed) */
    n = (long)fread(buf + 1, 1, (size_t)n, f) + 1;
    fclose(f);
    buf[n] = 0;
    for (char *p = buf; *p; p++) if (*p == 13) *p = 10;   /* one name (prefix) per line: names hold commas */
    return buf;
}

/* is `name` prefixed by one of the comma-separated prefixes in `list`? ("name$": that exact name) */
static int name_listed(const char *list, const char *name) {
    char sep = list[0] == 10 ? 10 : ',';                  /* a list from a file: one per line */
    for (const char *p = list; *p;) {
        const char *e = strchr(p, sep);
        size_t n = e ? (size_t)(e - p) : strlen(p);
        if (n > 1 && p[n - 1] == '$') {
            if (strlen(name) == n - 1 && !strncmp(name, p, n - 1)) return 1;
        } else if (n && !strncmp(name, p, n)) return 1;
        if (!e) break;
        p = e + 1;
    }
    return 0;
}

int import_shadow_stats(int *recorded, int *replayed);
const char *import_shadow_name(int k);
int import_shadow_divergence(const char **got);
void import_shadow_divergence_args(uint32_t rec[6], uint32_t got[6]);
/* rewrite.dll debug hook: a one-line summary of the last shadow run's imports */
static void rw_debug(char *out, int cap) {
    int rec, rep, div = import_shadow_stats(&rec, &rep);
    const char *got = NULL;
    int at = import_shadow_divergence(&got);
    int n = snprintf(out, (size_t)cap, "imports recorded %d replayed %d diverged %d", rec, rep, div);
    if (div) n += snprintf(out + n, (size_t)(cap - n), " (at #%d: recorded %s, called %s)", at,
                           at < rec ? import_shadow_name(at) : "(none)", got ? got : "?");
    if (div) {
        uint32_t ra[6], ga[6];
        import_shadow_divergence_args(ra, ga);
        n += snprintf(out + n, (size_t)(cap - n), " [args recorded %X %X %X %X %X %X, called %X %X %X %X %X %X]", ra[0], ra[1],
                      ra[2], ra[3], ra[4], ra[5], ga[0], ga[1], ga[2], ga[3], ga[4], ga[5]);
    }
    n += snprintf(out + n, (size_t)(cap - n), ":");
    for (int k = 0; k < rec && n < cap - 40; k++) n += snprintf(out + n, (size_t)(cap - n), " %s", import_shadow_name(k));
}
static void (*rw_report_fn)(void);

static void rw_log(const char *line) { hlog("%s", line); }

void rewrite_load(const char *root) {
    const char *off = getenv("KHUX_REWRITE");
    if (off && !strcmp(off, "0")) { hlog("rewrite: off (KHUX_REWRITE=0)"); return; }
    char path[1024];
    snprintf(path, sizeof(path), "%s\\out\\rewrite\\rewrite.dll", root);
    if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) { hlog("rewrite: no %s (lifted code only)", path); return; }
    HMODULE dll = LoadLibraryA(path);
    if (!dll) fatal("cannot load %s (error %lu)", path, GetLastError());
    int (*init)(const RwHost *) = (int (*)(const RwHost *))GetProcAddress(dll, "rw_init");
    uint32_t (*entry_addr)(int) = (uint32_t (*)(int))GetProcAddress(dll, "rw_entry_addr");
    const char *(*entry_name)(int) = (const char *(*)(int))GetProcAddress(dll, "rw_entry_name");
    int (*entry_effect)(int) = (int (*)(int))GetProcAddress(dll, "rw_entry_effect");
    int (*bind)(int, uint32_t, int) = (int (*)(int, uint32_t, int))GetProcAddress(dll, "rw_bind");
    rw_report_fn = (void (*)(void))GetProcAddress(dll, "rw_report");
    rw_where_fn = (const char *(*)(void))GetProcAddress(dll, "rw_where");
    rw_replay_fn = (int (*)(CPU *, const char *))GetProcAddress(dll, "rw_replay");
    LiftFn **slot_ptr = (LiftFn **)GetProcAddress(L.dll, "rt_override");
    int (*index)(uint32_t) = (int (*)(uint32_t))GetProcAddress(L.dll, "rt_index");
    void (*undo)(CPU *) = (void (*)(CPU *))GetProcAddress(L.dll, "rt_undo");
    if (!init || !entry_addr || !entry_name || !bind) fatal("%s is not a rewrite.dll", path);
    if (!slot_ptr || !index || !undo) fatal("lift.dll has no rt_override / rt_index: rebuild it (lift/build.py --all)");

    *slot_ptr = (LiftFn *)calloc((size_t)L.count, sizeof(LiftFn));
    const char *v = getenv("KHUX_RW_VERIFY");
    static char stats_path[1024], verified_path[1024];
    char stats_dir[1024];
    snprintf(stats_dir, sizeof(stats_dir), "%s/out/rewrite/stats", root);
    CreateDirectoryA(stats_dir, NULL);
    SYSTEMTIME t;
    GetLocalTime(&t);
    snprintf(stats_path, sizeof(stats_path), "%s/%04d%02d%02d_%02d%02d%02d.tsv", stats_dir, t.wYear, t.wMonth,
             t.wDay, t.wHour, t.wMinute, t.wSecond);
    snprintf(verified_path, sizeof(verified_path), "%s/rewrite/verified.txt", root);
    host.version = 10;
    host.track = mem_track;
    host.track_pages = mem_track_pages;
    host.track_zero = mem_track_zero;
    host.imports_export = import_shadow_export;
    host.imports_import = import_shadow_import;
    host.put = mem_put;
    host.host_call_returns = &rewrite_host_call_returns;
    /* the stale stack tracker (rt.h GTrack): lift.dll's loads and stores feed it during shadow runs */
    GTrack **gt_slot = (GTrack **)GetProcAddress(L.dll, "rt_gtrack");
    if (!gt_slot) fatal("lift.dll has no rt_gtrack: rebuild it (lift/build.py --all)");
    static GTrack gt;
    gt.wtime = (uint32_t *)calloc(GT_WINDOW / 4, 4);
    gt.ret_t = (uint32_t *)calloc(GT_RETS, 4);
    gt.ret_sp = (uint32_t *)calloc(GT_RETS, 4);
    gt.vals = (uint32_t *)calloc(GT_VALS, 4);
    *gt_slot = &gt;
    rt_gtrack = &gt;
    host.gtrack = &gt;
    host.ret_magic = RET_MAGIC;
    host.debug = rw_debug;
    host.shadow = libc_shadow;
    host.guest_call_exact = guest_call_exact;
    host.stats_path = stats_path;
    host.verified_path = verified_path;
    host.run = L.run;
    host.undo = undo;
    host.override_slots = *slot_ptr;
    host.guest_call = guest_call;
    host.guest_call64 = guest_call64;
    host.log = rw_log;
    host.verify = v && strcmp(v, "0") != 0;
    int n = init(&host);
    if (n < 0) fatal("rewrite.dll does not match this host (rebuild both)");
    int bound = 0;
    /* bisecting: KHUX_RW_ONLY / KHUX_RW_SKIP = comma-separated name prefixes ("SafeBuffer::,AppState::") */
    const char *only = read_list(getenv("KHUX_RW_ONLY")), *skip = read_list(getenv("KHUX_RW_SKIP"));
    for (int i = 0; i < n; i++) {
        uint32_t a = entry_addr(i);
        int effect = entry_effect && entry_effect(i);     /* effect boundaries stay with any filter */
        if (!effect && only && !name_listed(only, entry_name(i))) continue;
        if (!effect && skip && name_listed(skip, entry_name(i))) continue;
        int k = index(a);
        if (k < 0) { hlog("!! rewrite: %s at %08X is not a lifted function", entry_name(i), a); continue; }
        bound += bind(i, L.table[k].addr, k);
    }
    hlog("rewrite.dll: %d of %d rewritten functions installed%s", bound, n,
         host.verify ? " (verifying every call)" : " (the verified ones)");
}

const char *rewrite_where(void) { return rw_where_fn ? rw_where_fn() : ""; }

void rewrite_report(void) {
    if (!rw_report_fn || !host.verify) return;            /* only while verifying */
    LARGE_INTEGER f, t0, t1;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&t0);
    rw_report_fn();
    QueryPerformanceCounter(&t1);
    hlog("rewrite: report took %.0f ms", 1000.0 * (double)(t1.QuadPart - t0.QuadPart) / (double)f.QuadPart);
}

/* KHUX_COVERAGE=1: which lifted functions the game calls. lift.dll sets a byte per function in rt_seen; every
 * report merges the called ones into out/host/seen.txt (one hex address per line, kept across sessions): the
 * "executed in game" list for planning rewrites (rewrite/tools/context.py). */
static uint8_t *cov_seen, *cov_all;   /* cov_seen: since the last coverage_take; cov_all: taken before */
static char cov_path[1100];

void coverage_init(const char *root) {
    const char *v = getenv("KHUX_COVERAGE");
    if (!v || !strcmp(v, "0")) return;
    uint8_t **slot = (uint8_t **)GetProcAddress(L.dll, "rt_seen");
    if (!slot) { hlog("coverage: lift.dll has no rt_seen (rebuild it)"); return; }
    cov_seen = calloc((size_t)L.count, 1);
    cov_all = calloc((size_t)L.count, 1);
    *slot = cov_seen;
    snprintf(cov_path, sizeof(cov_path), "%s\\out\\host\\seen.txt", root);
    hlog("coverage: recording called functions into %s", cov_path);
}

void coverage_report(void) {
    if (!cov_seen) return;
    uint8_t *known = calloc((size_t)L.count, 1);
    FILE *f = fopen(cov_path, "r");
    if (f) {
        char line[32];
        while (fgets(line, sizeof(line), f)) {
            uint32_t a = (uint32_t)strtoul(line, NULL, 16);
            for (int lo = 0, hi = L.count - 1; lo <= hi;) {
                int mid = (lo + hi) / 2;
                uint32_t at = L.table[mid].addr & ~1u;
                if (at == a) { known[mid] = 1; break; }
                if (at < a) lo = mid + 1; else hi = mid - 1;
            }
        }
        fclose(f);
    }
    f = fopen(cov_path, "w");
    if (!f) { free(known); return; }
    int n = 0;
    for (int k = 0; k < L.count; k++)
        if (known[k] || cov_seen[k] || cov_all[k]) { fprintf(f, "%08X\n", L.table[k].addr & ~1u); n++; }
    fclose(f);
    free(known);
    hlog("coverage: %d functions called so far (all sessions)", n);
}

/* ctl "coverage": the functions called since the previous call (JSON list of hex addresses), for tagging
 * functions with the screen they run on. Called from the control thread; a byte the game sets while it is
 * being cleared here may be missed, which is fine for planning. */
char *coverage_take(void) {
    if (!cov_seen) return _strdup("{\"error\": \"run with KHUX_COVERAGE=1\"}");
    size_t cap = 64, len = 0;
    for (int k = 0; k < L.count; k++) if (cov_seen[k]) cap += 11;
    char *out = malloc(cap + 16);
    len += sprintf(out, "{\"fns\": [");
    int first = 1;
    for (int k = 0; k < L.count && len + 12 < cap; k++)
        if (cov_seen[k]) {
            cov_seen[k] = 0;
            cov_all[k] = 1;
            len += sprintf(out + len, "%s\"%08X\"", first ? "" : ",", L.table[k].addr & ~1u);
            first = 0;
        }
    sprintf(out + len, "]}");
    return out;
}
