// M3 rewrite runtime: registry, entry trampolines, guest calls and the differential verifier.
//
// The host (recomp/host) loads rewrite.dll, calls rw_init with its services, and for every entry binds the exact
// lifted address (Thumb bit included) and points that function's lift.dll override slot at rw_tramp(i).
// With KHUX_RW_VERIFY set, every call of a rewritten function is shadow-run: the lifted original runs with its
// guest stores logged, its result is recorded and undone, then the rewrite runs logged the same way, and the two
// are compared - the return registers, and the final value of every guest byte either one stored (outside the
// callee stack frames). On a mismatch the rewrite is undone, the lifted original runs for real, and the case is
// reported (the first few per function).
#include <algorithm>
#include <array>
#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <mutex>
#include <tuple>
#include <utility>
#include <vector>

#include "rw/rw.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

extern "C" {
typedef void (*LiftFn)(CPU *c);
struct RwHost {
    int version;                                           // RW_HOST_VERSION
    void (*run)(CPU *c, uint32_t target);                 // lift.dll rt_run: lifted code, tail calls followed
    void (*undo)(CPU *c);                                  // lift.dll rt_undo
    LiftFn *override_slots;                                // lift.dll rt_override (lift_count slots)
    uint32_t (*guest_call)(CPU *c, uint32_t fn, int n, const uint32_t *args);
    uint64_t (*guest_call64)(CPU *c, uint32_t fn, int n, const uint32_t *args);
    void (*log)(const char *line);
    int verify;                                            // KHUX_RW_VERIFY: shadow-run and compare every call
    const char *stats_path;                                // rw_report writes the per-function results here
    const char *verified_path;                             // addresses that passed verification (one hex per line)
    uint64_t (*guest_call_exact)(CPU *c, uint32_t fn, int n, const uint32_t *args);   // no realign, args at sp
    int (*shadow)(int phase);    // 0: begin (freeze clocks, save rand, record imports), 1: another run (restore
                                 // rand, replay imports), 2: end, 3: did the imports of this run diverge?
                                 // 4: did the first run call OpenGL ES (opaque: kept, not compared)?
                                 // 5 / 6: pause / resume the import recording (an effect's own imports)
                                 // 7 / 8: park the main thread for a shadow run on another thread (7: 0 = no)
    void (*debug)(char *out, int cap);   // a summary of the last shadow run's imports (mismatch reports)
    uint32_t ret_magic;                  // lr of a call made by the host itself (thread entries, callbacks)
    GTrack *gtrack;                      // rt.h: stale stack loads during shadow runs (shared with lift.dll)
    const volatile int *host_call_returns;   // 1 while the host's own calls return (the .init_array constructors)
};
}
#define RW_HOST_VERSION 9

// rt.h wlog: a full store log stops logging and the run goes on for real (the verifier keeps it)
extern "C" EXPORT int rt_log_full_continue = 1;
// rt.h GTrack: the host's tracker (the rewrite's own stores feed it too)
extern "C" EXPORT GTrack *rt_gtrack = nullptr;

namespace rw {

thread_local CPU *cpu = nullptr;

static std::vector<Entry> &entries() {
    static std::vector<Entry> v;
    return v;
}

int add(const Entry &e) {
    entries().push_back(e);
    return int(entries().size()) - 1;
}

static const RwHost *H = nullptr;

struct Bound {
    uint32_t target = 0;     // exact lifted address (mode bit included)
    int slot = -1;           // lift.dll index
};
static std::vector<Bound> bound;

struct Stats {
    std::atomic<uint64_t> calls{0}, verified{0}, mismatches{0}, unverifiable{0};
    std::atomic<uint64_t> stale_words{0};   // differing words accepted as stale stack bytes (rt.h GTrack)
    std::atomic<bool> too_big{false};   // a shadow run filled the store log: later calls run lifted, unshadowed
};
static std::vector<Stats> *stats;

static void logf(const char *fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    if (H && H->log) H->log(buf);
}

// set while a rewrite calls a guest function: the host's guest_call gives the callee RET_MAGIC as lr, like a call
// the host itself makes (a thread entry, a callback), but a rewritten callee must still be verified (verify)
static thread_local bool from_rewrite = false;

// crash reports: on this thread, the rewritten functions entered through their trampolines and the guest functions
// rewrites called, innermost last. Rewritten C++ frames are not on the guest stack, so this is the only call chain
// a crash inside them has. Direct C++ calls between rewrites are not listed.
struct Link { uint32_t v; uint8_t kind; };   // kind: 0 entered entry v, 1 called guest address v, 2/3 left them
static thread_local Link chain[256];
static thread_local int chain_n = 0;
// and the last 32 enter / leave events (a crash right after a call returned shows what ran just before)
static thread_local Link hist[32];
static thread_local uint32_t hist_n = 0;
struct Chain {
    Link l;
    Chain(uint32_t v, uint8_t kind) : l{v, kind} {
        if (chain_n < 256) chain[chain_n] = l;
        chain_n++;
        hist[hist_n++ & 31] = l;
    }
    ~Chain() { chain_n--; hist[hist_n++ & 31] = Link{l.v, uint8_t(l.kind + 2)}; }
};

uint32_t call(uint32_t fn, std::initializer_list<uint32_t> args) {
    Chain in_chain(fn, 1);
    std::vector<uint32_t> a(args);
    CPU *c = cpu;
    uint32_t r = (from_rewrite = true, H->guest_call(c, fn, int(a.size()), a.data()));
    from_rewrite = false;
    cpu = c;                                               // a nested rewritten call set it to the same CPU
    return r;
}

uint64_t call_exact(uint32_t fn, std::initializer_list<uint32_t> args) {
    Chain in_chain(fn, 1);
    std::vector<uint32_t> a(args);
    CPU *c = cpu;
    uint64_t r = (from_rewrite = true, H->guest_call_exact(c, fn, int(a.size()), a.data()));
    from_rewrite = false;
    cpu = c;
    return r;
}

uint64_t call64(uint32_t fn, std::initializer_list<uint32_t> args) {
    Chain in_chain(fn, 1);
    std::vector<uint32_t> a(args);
    CPU *c = cpu;
    uint64_t r = (from_rewrite = true, H->guest_call64(c, fn, int(a.size()), a.data()));
    from_rewrite = false;
    cpu = c;
    return r;
}

// ------------------------------------------------------------------------------------------------- verifier
// 64M stores (1 GB of address space per verifying thread; only the pages a run touches get memory). A run that
// fills it skips its later stores and is void, so it must be rare: constructing a singleton (big zero-filled
// tables) took more than the 1M of the first version.
static constexpr uint32_t LOG_CAP = 1u << 26;
static constexpr uint32_t STACK_WINDOW = 1u << 20;         // callee frames live in [sp - 1 MB, sp)

struct Regs {
    uint32_t r[16], n, z, c, v;
};
static Regs save(const CPU *c) {
    Regs s;
    std::memcpy(s.r, c->r, sizeof s.r);
    s.n = c->n; s.z = c->z; s.c = c->c; s.v = c->v;
    return s;
}
static void restore(CPU *c, const Regs &s) {
    std::memcpy(c->r, s.r, sizeof s.r);
    c->n = s.n; c->z = s.z; c->c = s.c; c->v = s.v;
}

// Final bytes of every logged store outside the callee stack frames: address -> byte.
static std::map<uint32_t, uint8_t> stored(const CPU *c, uint32_t sp) {
    std::map<uint32_t, uint8_t> out;
    uint32_t n = c->wn < c->wcap ? c->wn : c->wcap;
    for (uint32_t k = 0; k < n; k++) {
        const WLog &w = c->wlog[k];
        for (uint32_t b = 0; b < w.size; b++) {
            uint32_t a = w.addr + b;
            if (a < sp && sp - a <= STACK_WINDOW) continue;
            out[a] = c->mem[a];
        }
    }
    return out;
}

static uint32_t cur32(const CPU *c, uint32_t a) { uint32_t v; std::memcpy(&v, c->mem + a, 4); return v; }

// ---- stale stack bytes (rt.h GTrack): the values each run loaded from stack words no live frame had stored
static void gt_begin(uint32_t sp) {
    GTrack *g = rt_gtrack;
    if (!g) return;
    g->hi = sp;
    g->lo = sp - GT_WINDOW;
    g->clock = 0;
    g->nret = 0;
    g->overflow = 0;
    g->has_zero = 0;
    std::memset(g->wtime, 0, GT_WINDOW);
    std::memset(g->vals, 0, GT_VALS * 4);
    g->on = 1;
}
static void gt_end() { if (rt_gtrack) rt_gtrack->on = 0; }
static thread_local std::vector<uint32_t> gt_vals_l;       // the lifted run's set (GTrack.vals layout)
static thread_local bool gt_zero_l = false, gt_valid_l = false;
static void gt_snapshot() {
    GTrack *g = rt_gtrack;
    gt_valid_l = g && !g->overflow;
    if (!gt_valid_l) return;
    gt_vals_l.assign(g->vals, g->vals + GT_VALS);
    gt_zero_l = g->has_zero != 0;
}
// a differing word is garbage on both sides: the lifted value and the rewrite's were both loaded from stale words
// (off unless KHUX_RW_STALE=1: experimental - the rewrite side cannot see stale loads made by rewritten C++)
static bool stale_pair(uint32_t lifted, uint32_t rewrite) {
    static const bool enabled = getenv("KHUX_RW_STALE") && strcmp(getenv("KHUX_RW_STALE"), "0") != 0;
    if (!enabled) return false;
    GTrack *g = rt_gtrack;
    if (!g || !gt_valid_l || g->overflow) return false;
    GTrack l = *g;
    l.vals = gt_vals_l.data();
    l.has_zero = gt_zero_l;
    return gt_has(&l, lifted) && gt_has(g, rewrite);
}

static void run_lifted(CPU *c, int i) {
    LiftFn keep = H->override_slots[bound[i].slot];
    H->override_slots[bound[i].slot] = nullptr;            // other threads run the lifted code meanwhile: fine
    H->run(c, bound[i].target);
    H->override_slots[bound[i].slot] = keep;
}


static void run_rewrite(CPU *c, int i) {
    CPU *outer = cpu;
    cpu = c;
    Chain in_chain(uint32_t(i), 0);
    entries()[i].run(c);
    cpu = outer;
    c->pc = c->r[14];                                      // returned to the caller, like a lifted function
}

// the store log of this thread's shadow runs (1 GB committed): released when the thread ends - the network code
// starts a thread per request, and the logs of finished threads used up the commit limit ("cannot start a host
// thread")
struct LogBuf {
    WLog *p = nullptr;
    ~LogBuf() { if (p) VirtualFree(p, 0, MEM_RELEASE); }
};
static thread_local LogBuf logbuf_;
#define logbuf (logbuf_.p)

// ------------------------------------------------------------------------------------------------- effects
// Functions whose effects reach outside guest memory (a request to the server, a file or UserDefault write,
// analytics, a sound). In a shadow run they would happen twice, so the verifier treats them as boundaries:
// in the lifted run each one runs for real (once) and is recorded - its arguments, its return registers and the
// final value of every guest byte it stored; in the rewrite run the
// same call is matched against the record and replayed - the recorded stores are applied, nothing runs.
// A call that does not match the record (other function, other arguments, or one too many / too few) fails the
// comparison. Outside shadow runs they are plain calls.
// A handoff effect gives work to another thread (a request goes to the network thread's queue, which that thread
// pops at once): its stores cannot be undone and replayed, so a shadow run that makes one is kept as the real
// call, like a run that draws (not compared).
// args: the argument registers the effect reads (the others are leftovers of the caller and not compared)
struct EffectDef { uint32_t addr; const char *name; bool handoff = false; int args = 4; };
static const EffectDef EFFECTS[] = {
    {0x4B7200, "hole::network::api::Client::call", true},
    {0x7E4408, "cocos2d::network::HttpClient::send", true},
    {0x4E8DC8, "AppState::save", false, 1},
    {0x4E8E8C, "AppState::saveMain", false, 1},
    {0x4E2A5C, "TrackingSDKUtil::sendEventInternal", false, 2},
    {0x8D4B4C, "cocos2d::FileUtils::writeToFile"},
    {0x901234, "cocos2d::UserDefault::setBoolForKey"},
    {0x90129C, "cocos2d::UserDefault::flush"},
    {0x9013A0, "cocos2d::UserDefault::setIntegerForKey"},
    {0x9014BC, "cocos2d::UserDefault::setFloatForKey"},
    {0x9015EC, "cocos2d::UserDefault::setDoubleForKey"},
    {0x901794, "cocos2d::UserDefault::setStringForKey"},
    {0x901988, "cocos2d::UserDefault::setDataForKey"},
    {0x937820, "CocosDenshion::SimpleAudioEngine::playBackgroundMusic"},
    {0x93783E, "CocosDenshion::SimpleAudioEngine::playEffect"},
};
static constexpr int EFFECT_COUNT = int(sizeof(EFFECTS) / sizeof(EFFECTS[0]));

enum class Phase { Off, Record, Replay };
struct EffectCall {
    uint32_t fn;
    uint32_t args[4];
    uint32_t ret0, ret1;
    uint32_t sp;                                        // at the call
    std::vector<std::pair<uint32_t, uint8_t>> stores;   // guest byte, final value (not the effect's own frames)
};

// Where a recorded effect store goes in the replay: the rewrite's stack frames are not laid out like the
// original's. A store into the buffer of a pointer argument (at most 64 KB past it) moves with that argument; any
// other store into the caller's stack moves with sp; anything else (heap, globals) keeps its address.
static uint32_t effect_store_addr(const EffectCall &rec, const CPU *c, uint32_t a) {
    int best = -1;
    for (int k = 0; k < 4; k++) {
        uint32_t p = rec.args[k];
        if (p >= 0x1000u && a >= p && a - p < 0x10000u && (best < 0 || p > rec.args[best])) best = k;
    }
    if (best >= 0) return a - rec.args[best] + c->r[best];
    if (a >= rec.sp && a - rec.sp < STACK_WINDOW) return a - rec.sp + c->r[13];
    return a;
}
static thread_local Phase phase = Phase::Off;
static thread_local std::vector<EffectCall> effect_calls;
static thread_local size_t effect_pos;
static thread_local bool effect_diverged;

// a word that points into the current thread's stack (frames may legitimately sit elsewhere in the rewrite)
static bool stackish(CPU *c, uint32_t v) {
    uint32_t sp = c->r[13];
    return v >= sp - STACK_WINDOW && v < sp + STACK_WINDOW;
}

static void run_lifted(CPU *c, int i);

static thread_local bool effect_handoff;   // the lifted run made a handoff effect

static void effect_enter(CPU *c, int i) {
    const uint32_t addr = entries()[i].addr;
    if (phase == Phase::Record) {
        for (const EffectDef &d : EFFECTS)
            if (d.addr == addr && d.handoff) effect_handoff = true;
        EffectCall rec{addr, {c->r[0], c->r[1], c->r[2], c->r[3]}, 0, 0, c->r[13], {}};
        uint32_t w0 = c->wn;
        phase = Phase::Off;                              // effects inside an effect are part of it
        H->shadow(5);                                    // and so are its imports: replayed with it
        run_lifted(c, i);
        H->shadow(6);
        phase = Phase::Record;
        uint32_t n = c->wn < c->wcap ? c->wn : c->wcap;
        std::map<uint32_t, uint8_t> bytes;
        for (uint32_t k = w0; k < n; k++)
            for (uint32_t b = 0; b < c->wlog[k].size; b++) {
                uint32_t a = c->wlog[k].addr + b;
                if (a < rec.sp && rec.sp - a <= STACK_WINDOW) continue;     // the effect's own (dead) frames
                bytes[a] = c->mem[a];
            }
        rec.stores.assign(bytes.begin(), bytes.end());
        rec.ret0 = c->r[0];
        rec.ret1 = c->r[1];
        effect_calls.push_back(std::move(rec));
        return;
    }
    if (phase == Phase::Replay) {
        bool ok = effect_pos < effect_calls.size() && effect_calls[effect_pos].fn == addr;
        if (ok) {
            const EffectCall &rec = effect_calls[effect_pos];
            int nargs = 4;
            for (const EffectDef &d : EFFECTS)
                if (d.addr == addr) nargs = d.args;
            for (int k = 0; k < nargs && ok; k++)
                ok = rec.args[k] == c->r[k] || (stackish(c, rec.args[k]) && stackish(c, c->r[k]));
        }
        if (!ok) {
            effect_diverged = true;
            c->r[0] = 0;
            c->pc = c->r[14];
            return;
        }
        const EffectCall &rec = effect_calls[effect_pos++];
        CPU *outer = cpu;                                // log_store logs on rw::cpu (unset in lifted runs)
        cpu = c;
        for (auto &sb : rec.stores) {
            uint32_t a = effect_store_addr(rec, c, sb.first);
            if (log_store(c->mem + a, 1)) c->mem[a] = sb.second;
        }
        cpu = outer;
        c->r[0] = rec.ret0;
        c->r[1] = rec.ret1;
        c->pc = c->r[14];
        return;
    }
    run_lifted(c, i);                                    // not in a shadow run: a plain call
}

static thread_local bool lifted_run = false;   // the current shadow run is the lifted side
// rewritten functions the lifted side of the current shadow run passed through: when the comparison matches, the
// rewrite side ran their rewrites too (through rw::call or as direct C++ calls), so each gets a verified call
static thread_local std::vector<int> nested;
// crash reports: the function being verified on this thread and the side running ("lifted", "rewrite")
static thread_local const char *where_fn = nullptr, *where_side = "";

static void verify(CPU *c, int i) {
    const Entry &e = entries()[i];
    Stats &st = (*stats)[i];
    // inside the lifted side of an outer shadow run: the original all the way down, so
    // the comparison is lifted code against rewritten code at every level
    const bool by_rewrite = from_rewrite;      // called through rw::call (consumed here: deeper calls are not)
    from_rewrite = false;
    if (lifted_run) { if (nested.size() < 4096) nested.push_back(i); run_lifted(c, i); return; }
    if (st.too_big) { if (c->logging) run_rewrite(c, i); else run_lifted(c, i); return; }
    // called by the host itself (a thread's entry, a JNI or GL callback): a thread entry never returns, so it
    // cannot be shadow-run; the original runs, and every call it makes is verified on its own. The constructors
    // (the host flags them) return and are shadow-run like any call.
    if (!c->logging && !by_rewrite && (c->r[14] & ~1u) == H->ret_magic && !*H->host_call_returns) {
        run_lifted(c, i);
        return;
    }
    // inside the rewrite side of an outer shadow run: its comparison covers this call
    if (c->logging) { run_rewrite(c, i); return; }
    // verified often enough with no mismatch (KHUX_RW_CAP calls, default 200; 0 = never stop): the rewrite runs
    // without a shadow run, so long sessions stay playable and reach further into the game
    static const uint64_t cap = getenv("KHUX_RW_CAP") ? strtoull(getenv("KHUX_RW_CAP"), nullptr, 10) : 200;
    if (cap && st.verified.load() >= cap && st.mismatches.load() == 0) { run_rewrite(c, i); return; }
    // on another thread than the main one, the main thread parks for the whole shadow run (it must not see the
    // lifted run's stores before the undo); when it cannot park in time, the original runs without a comparison
    if (!H->shadow(7)) { run_lifted(c, i); return; }
    struct Exclusive { ~Exclusive() { H->shadow(8); } } exclusive;
    if (!logbuf) {
        logbuf = static_cast<WLog *>(VirtualAlloc(nullptr, size_t(LOG_CAP) * sizeof(WLog), MEM_RESERVE | MEM_COMMIT,
                                                  PAGE_READWRITE));
        if (!logbuf) { logf("RW: no memory for the store log"); run_lifted(c, i); return; }
    }
    WLog *old_log = c->wlog;
    uint32_t old_cap = c->wcap;
    c->wlog = logbuf;
    c->wcap = LOG_CAP;
    const Regs in = save(c);
    const uint32_t sp = c->r[13];

    // heap bump pointer before / after each run (mismatch reports: allocation differences)
    const uint32_t heap0 = *reinterpret_cast<const uint32_t *>(c->mem + 0x20000010);
    // 1. the lifted original (clocks frozen and random numbers replayed for both runs; effects run and recorded)
    // KHUX_RW_WATCH=addr (with KHUX_RW_DUMPFN): the word there at each step of the shadow run
    static const uint32_t watch = getenv("KHUX_RW_WATCH") ? uint32_t(strtoul(getenv("KHUX_RW_WATCH"), nullptr, 16)) : 0;
    static const uint32_t watch_fn = getenv("KHUX_RW_DUMPFN") ? uint32_t(strtoul(getenv("KHUX_RW_DUMPFN"), nullptr, 16)) : 0;
    uint32_t wv[4] = {};
    auto peek = [&](int k) { if (watch) memcpy(&wv[k], c->mem + watch, 4); };
    peek(0);
    H->shadow(0);
    effect_calls.clear();
    effect_handoff = false;
    phase = Phase::Record;
    c->logging = 1; c->wn = 0; c->wfull = 0;
    gt_begin(sp);
    lifted_run = true;
    where_fn = e.name; where_side = "lifted";
    nested.clear();
    run_lifted(c, i);
    lifted_run = false;
    phase = Phase::Off;
    const Regs out_l = save(c);
    const uint32_t pc_l = c->pc;
    const bool full_l = c->wfull != 0;
    if (H->shadow(4) || full_l || effect_handoff) {
        // it called OpenGL ES, handed work to another thread, or it filled the store log (logging stopped, the rest ran for real): either way
        // this run cannot be undone or replayed, so it is the real call
        if (full_l && !st.too_big.exchange(true))
            logf("RW LOG FULL %s @%08X: kept as the real call; not shadowed from now on", e.name, e.addr);
        c->logging = 0;
        gt_end();
        st.unverifiable++;
        where_fn = nullptr;
        H->shadow(2);
        c->wlog = old_log;
        c->wcap = old_cap;
        c->wn = 0;
        return;
    }
    auto mem_l = stored(c, sp);
    // the lifted run's final bytes, its dead frames included: a mismatch puts these back instead of running the
    // original a second time (a rerun after two undone runs replayed its imports against a different path)
    std::vector<std::pair<uint32_t, uint8_t>> post_l;
    {
        uint32_t n = c->wn < c->wcap ? c->wn : c->wcap;
        for (uint32_t k = 0; k < n; k++)
            for (uint32_t b = 0; b < c->wlog[k].size; b++) {
                uint32_t a = c->wlog[k].addr + b;
                post_l.emplace_back(a, c->mem[a]);
            }
    }
    const uint32_t heap_l = *reinterpret_cast<const uint32_t *>(c->mem + 0x20000010);
    auto dump_log = [&](const char *tag) {               // KHUX_RW_DUMPLOG: every store of both runs, appended
        static const char *path = getenv("KHUX_RW_DUMPLOG");
        static const uint32_t only = getenv("KHUX_RW_DUMPFN") ? uint32_t(strtoul(getenv("KHUX_RW_DUMPFN"), nullptr, 16)) : 0;
        if (!path || (only && e.addr != only)) return;
        FILE *f = fopen(path, "a");
        if (!f) return;
        uint32_t n = c->wn < c->wcap ? c->wn : c->wcap;
        fprintf(f, "== %s %s %u\n", e.name, tag, n);
        for (uint32_t k = 0; k < n; k++) {
            const WLog &w = c->wlog[k];
            uint32_t now = 0;
            memcpy(&now, c->mem + w.addr, w.size);
            fprintf(f, "%08X %u old %08X now %08X pc %08X\n", w.addr, w.size, w.old_lo, now, w.old_hi);
        }
        fclose(f);
    };
    dump_log("lifted");
    auto heap_trace = [&](std::string &out) {             // each store to the heap pointer: old value @ guest pc
        out.clear();
        uint32_t n = c->wn < c->wcap ? c->wn : c->wcap;
        char b[40];
        for (uint32_t k = 0; k < n && out.size() < 600; k++)
            if (c->wlog[k].addr == 0x20000010) { snprintf(b, sizeof b, " %08X@%08X", c->wlog[k].old_lo, c->wlog[k].old_hi); out += b; }
    };
    static thread_local std::string trace_l, trace_r;
    heap_trace(trace_l);
    // KHUX_RW_STOREPC=1: keep the lifted run's store log, so a mismatch report can name the storing instructions
    static const bool keep_pcs = getenv("KHUX_RW_STOREPC") != nullptr;
    static thread_local std::vector<WLog> log_l;
    if (keep_pcs) log_l.assign(c->wlog, c->wlog + (c->wn < c->wcap ? c->wn : c->wcap));
    gt_snapshot();                                        // the lifted run's stale values; the rewrite run's next
    gt_begin(sp);
    peek(1);
    H->undo(c);
    peek(2);

    // 2. the rewrite, from the same state
    H->shadow(1);
    restore(c, in);
    c->wn = 0; c->wfull = 0;
    phase = Phase::Replay; effect_pos = 0; effect_diverged = false;
    // KHUX_RW_SELFTEST=1: the lifted code against itself - any mismatch is the verifier's (nondeterminism)
    static const bool selftest = getenv("KHUX_RW_SELFTEST") != nullptr;
    where_side = "rewrite";
    if (selftest) { lifted_run = true; run_lifted(c, i); lifted_run = false; }
    else run_rewrite(c, i);
    phase = Phase::Off;
    if (rt_gtrack) rt_gtrack->on = 0;
    const bool effects_ok = !effect_diverged && effect_pos == effect_calls.size() && !H->shadow(3);
    const uint32_t heap_r = *reinterpret_cast<const uint32_t *>(c->mem + 0x20000010);
    heap_trace(trace_r);
    peek(3);
    if (watch && e.addr == watch_fn)
        logf("RW WATCH %08X: before %08X, after lifted %08X, after undo %08X, after rewrite %08X", watch, wv[0], wv[1],
             wv[2], wv[3]);
    dump_log("rewrite");
    const Regs out_r = save(c);
    const uint32_t pc_r = c->pc;
    const bool full_r = c->wfull != 0;
    auto mem_r = stored(c, sp);

    const char *why = "";
    bool same = !full_l && !full_r;
    if (same && !effects_ok) { same = false; why = "effect calls"; }
    if (same && pc_l != pc_r) { same = false; why = "return address"; }
    if (same && out_r.r[13] != out_l.r[13]) { same = false; why = "sp"; }
    for (int k = 0; k < e.ret_words && same; k++) if (out_l.r[k] != out_r.r[k]) { same = false; why = "return value"; }
    if (same) why = "memory";
    if (same) {
        // every byte either one stored must end equal (a byte only one of them wrote keeps its old value in
        // the other run, which the undo restored - so compare against the current memory for those)
        // a word both runs stored whole that points into this thread's stack in both is the address of a local
        // (callee frames sit elsewhere in the rewrite): equal when both are stack addresses
        auto word = [](const std::map<uint32_t, uint8_t> &m, uint32_t a, uint32_t &v) {
            v = 0;
            for (uint32_t b = 0; b < 4; b++) {
                auto it = m.find(a + b);
                if (it == m.end()) return false;
                v |= uint32_t(it->second) << (8 * b);
            }
            return true;
        };
        auto on_stack = [&](uint32_t v) { return v >= sp - STACK_WINDOW && v < sp + STACK_WINDOW; };
        for (auto &kv : mem_l) {
            auto it = mem_r.find(kv.first);
            uint8_t r = it != mem_r.end() ? it->second : c->mem[kv.first];
            if (r == kv.second) continue;
            uint32_t wl, wr, a = kv.first & ~3u;
            if (word(mem_l, a, wl) && word(mem_r, a, wr) && on_stack(wl) && on_stack(wr)) continue;
            // both runs stored a value read from stale stack bytes (rt.h GTrack): garbage either way
            if (word(mem_l, a, wl) && stale_pair(wl, cur32(c, a))) {
                st.stale_words++;
                if (st.stale_words <= 2)
                    logf("RW STALE %s @%08X: word %08X lifted %08X, rewrite %08X (both from stale stack bytes)",
                         e.name, e.addr, a, wl, cur32(c, a));
                continue;
            }
            same = false;
            break;
        }
        if (same) for (auto &kv : mem_r) {
            if (mem_l.count(kv.first)) continue;
            // the lifted run did not store here: the byte must be unchanged by the rewrite, but the
            // rewrite's value is what memory holds now; the original value is in the log
            same = false;
            uint32_t n = c->wn < c->wcap ? c->wn : c->wcap;
            for (uint32_t k = 0; k < n; k++) {
                const WLog &w = c->wlog[k];
                if (kv.first >= w.addr && kv.first < w.addr + w.size) {
                    uint8_t old = uint8_t(w.old_lo >> (8 * (kv.first - w.addr)));
                    same = old == kv.second;               // stored the value it already had
                    break;
                }
            }
            if (!same) break;
        }
    }
    c->logging = 0;
    if (full_r) {
        // the rewrite filled the log: it went on unlogged and cannot be undone, so its result stays
        st.unverifiable++;
        if (!st.too_big.exchange(true))
            logf("RW LOG FULL %s @%08X (rewrite run): its result is kept; not shadowed from now on", e.name, e.addr);
    } else if (same) {
        st.verified++;
        std::sort(nested.begin(), nested.end());
        nested.erase(std::unique(nested.begin(), nested.end()), nested.end());
        for (int k : nested) if (k != i) (*stats)[k].verified++;
    } else {
        uint64_t n = ++st.mismatches;
        if (n <= 3) {
            logf("RW MISMATCH %s @%08X (#%llu, %s): args r0=%08X r1=%08X r2=%08X r3=%08X sp=%08X | lifted r0=%08X "
                 "r1=%08X sp=%08X pc=%08X stores=%zu | rewrite r0=%08X r1=%08X sp=%08X pc=%08X stores=%zu",
                 e.name, e.addr, (unsigned long long)n, why, in.r[0], in.r[1], in.r[2], in.r[3], in.r[13],
                 out_l.r[0], out_l.r[1], out_l.r[13], pc_l, mem_l.size(), out_r.r[0], out_r.r[1], out_r.r[13],
                 pc_r, mem_r.size());
            char dbg[1536];
            H->debug(dbg, sizeof dbg);
            logf("    heap %08X -> lifted %08X, rewrite %08X; effects %zu/%zu; %s", heap0, heap_l, heap_r, effect_pos,
                 effect_calls.size(), dbg);
            {   // stale stack tracking (rt.h GTrack): the sets, and the first differing word against them
                size_t nl = 0, nr = 0;
                for (uint32_t v : gt_vals_l) nl += v != 0;
                if (rt_gtrack) for (uint32_t k = 0; k < GT_VALS; k++) nr += rt_gtrack->vals[k] != 0;
                std::string probe;
                for (auto &kv : mem_l) {
                    auto it = mem_r.find(kv.first);
                    if (it != mem_r.end() && it->second == kv.second) continue;
                    uint32_t a = kv.first & ~3u, wl = 0;
                    for (uint32_t b = 0; b < 4; b++) { auto j = mem_l.find(a + b); wl |= uint32_t(j != mem_l.end() ? j->second : 0) << (8 * b); }
                    uint32_t wr = cur32(c, a);
                    GTrack l = rt_gtrack ? *rt_gtrack : GTrack{};
                    l.vals = gt_vals_l.data(); l.has_zero = gt_zero_l;
                    char b[160];
                    snprintf(b, sizeof b, "; word %08X lifted %08X (stale: %d) rewrite %08X (stale: %d)", a, wl,
                             gt_valid_l && gt_has(&l, wl), wr, rt_gtrack && gt_has(rt_gtrack, wr));
                    probe = b;
                    break;
                }
                logf("    stale values: lifted %zu%s%s, rewrite %zu%s, returns %u%s", nl, gt_zero_l ? " +0" : "",
                     gt_valid_l ? "" : " (overflow)", nr, rt_gtrack && rt_gtrack->has_zero ? " +0" : "",
                     rt_gtrack ? rt_gtrack->nret : 0, probe.c_str());
            }
            if (keep_pcs) {   // the last store to each of the first differing words, in each run: guest pc
                auto last_pc = [](const WLog *w, size_t n, uint32_t a) -> uint32_t {
                    for (size_t k = n; k-- > 0;) if (a >= w[k].addr && a < w[k].addr + w[k].size) return w[k].old_hi;
                    return 0;
                };
                int shown_pc = 0;
                for (auto &kv : mem_l) {
                    auto it = mem_r.find(kv.first);
                    if (it != mem_r.end() && it->second == kv.second) continue;
                    logf("    store pc %08X: lifted %08X, rewrite %08X", kv.first,
                         last_pc(log_l.data(), log_l.size(), kv.first),
                         last_pc(c->wlog, c->wn < c->wcap ? c->wn : c->wcap, kv.first));
                    if (++shown_pc >= 4) break;
                }
            }
            logf("    heap stores lifted:%s", trace_l.c_str());
            logf("    heap stores rewrite:%s", trace_r.c_str());
            int shown = 0;
            for (auto &kv : mem_l) {
                auto it = mem_r.find(kv.first);
                if (it == mem_r.end() || it->second != kv.second) {
                    if (it == mem_r.end()) logf("    byte %08X: lifted %02X, rewrite did not store", kv.first, kv.second);
                    else logf("    byte %08X: lifted %02X, rewrite %02X", kv.first, kv.second, it->second);
                    if (++shown >= 8) break;
                }
            }
            for (auto &kv : mem_r) {
                if (mem_l.count(kv.first)) continue;
                logf("    byte %08X: only the rewrite stored %02X", kv.first, kv.second);
                if (++shown >= 12) break;
            }
        }
    }
    if (full_r || (same && !full_l)) {
        restore(c, out_r);
        c->pc = pc_r;
    } else {
        // keep the game on the original: undo the rewrite, then put back what the lifted run left - its stores,
        // registers and return address. Its imports and effects already happened for real, once, in that run.
        c->logging = 1;
        H->undo(c);
        c->logging = 0;
        for (auto &ab : post_l) c->mem[ab.first] = ab.second;
        restore(c, out_l);
        c->pc = pc_l;
    }
    H->shadow(2);
    c->wlog = old_log;
    c->wcap = old_cap;
    c->wn = 0;
    where_fn = nullptr;
}

void enter(CPU *c, int i) {
    if (entries()[i].effect) { effect_enter(c, i); return; }
    (*stats)[i].calls++;
    if (H->verify && entries()[i].verify) verify(c, i);
    else run_rewrite(c, i);
}

static std::vector<uint32_t> verified_set;     // sorted guest addresses from verified.txt

// effect boundaries get registry entries (after the rewritten functions) with their own trampolines
static int effect_base = -1;
template <int N> static void effect_tramp(CPU *c) { enter(c, effect_base + N); }
template <int... N> static std::array<void (*)(CPU *), sizeof...(N)> effect_tramps(std::integer_sequence<int, N...>) {
    return {{&effect_tramp<N>...}};
}
static void add_effects() {
    static const auto tr = effect_tramps(std::make_integer_sequence<int, EFFECT_COUNT>{});
    effect_base = int(entries().size());
    for (int k = 0; k < EFFECT_COUNT; k++) {
        Entry e{EFFECTS[k].addr, EFFECTS[k].name, nullptr, 0, false, tr[k]};
        e.effect = true;
        add(e);
    }
}

}  // namespace rw

// ------------------------------------------------------------------------------------------------- host API
extern "C" {

__declspec(dllexport) int rw_init(const RwHost *host) {
    if (!host || host->version != RW_HOST_VERSION) return -1;
    rw::H = host;
    rt_gtrack = host->gtrack;
    if (host->verify) rw::add_effects();             // effect boundaries matter only in shadow runs
    int n = int(rw::entries().size());
    rw::bound.assign(n, rw::Bound{});
    rw::stats = new std::vector<rw::Stats>(n);
    if (host->verified_path) {
        if (FILE *f = fopen(host->verified_path, "r")) {
            char line[256];
            while (fgets(line, sizeof line, f)) {
                unsigned a = 0;
                if (sscanf(line, "%x", &a) == 1) rw::verified_set.push_back(a & ~1u);
            }
            fclose(f);
        }
        std::sort(rw::verified_set.begin(), rw::verified_set.end());
    }
    return n;
}

__declspec(dllexport) uint32_t rw_entry_addr(int i) { return rw::entries()[i].addr; }
__declspec(dllexport) const char *rw_entry_name(int i) { return rw::entries()[i].name; }
// the verifier's effect boundaries (not rewrites: KHUX_RW_ONLY / SKIP never filter them out)
__declspec(dllexport) int rw_entry_effect(int i) { return rw::entries()[i].effect ? 1 : 0; }

// The host found the lifted function for entry i (exact address, its lift.dll slot). Install the override when
// verifying (a mismatch falls back to the lifted code) or when the function passed verification before.
// Returns 1 when installed.
__declspec(dllexport) const char *rw_where(void) {
    static thread_local char buf[8192];
    size_t n = 0;
    buf[0] = 0;
    if (rw::where_fn) n = snprintf(buf, sizeof buf, "verifying %s (%s side)", rw::where_fn, rw::where_side);
    // the innermost 16 links of the chain
    auto link = [&](const rw::Link &l) -> std::string {
        if (l.kind == 0) return rw::entries()[l.v].name;
        if (l.kind == 1) { char t[24]; snprintf(t, sizeof t, "call 0x%X", l.v); return t; }
        return "";
    };
    int top = rw::chain_n < 256 ? rw::chain_n : 256;
    const int first = top > 16 ? top - 16 : 0;
    for (int k = first; k < top && n < sizeof buf - 300; k++)
        n += snprintf(buf + n, sizeof buf - n, "%s%s", k == first ? "\n   rewrite chain: " : " > ",
                      link(rw::chain[k]).c_str());
    // recent history, oldest first: "+name" / "+call 0x.." entered, "-name" / "-" left
    uint32_t hn = rw::hist_n, from = hn > 24 ? hn - 24 : 0;
    for (uint32_t k = from; k < hn && n < sizeof buf - 300; k++) {
        const rw::Link &l = rw::hist[k & 31];
        const char *sep = k == from ? "\n   recent: " : " ";
        if (l.kind == 3) n += snprintf(buf + n, sizeof buf - n, "%s-", sep);
        else n += snprintf(buf + n, sizeof buf - n, "%s%c%s", sep, l.kind < 2 ? '+' : '-',
                           link(rw::Link{l.v, uint8_t(l.kind & 1)}).c_str());
    }
    return buf;
}

__declspec(dllexport) int rw_bind(int i, uint32_t target, int slot) {
    rw::bound[i] = rw::Bound{target, slot};
    uint32_t a = rw::entries()[i].addr & ~1u;
    // a NOVERIFY function cannot be shadow-checked: while verifying, the lifted code runs instead
    // (effect boundaries are installed only while verifying)
    bool ok = rw::entries()[i].effect ? rw::H->verify != 0
            : rw::H->verify ? rw::entries()[i].verify
                            : std::binary_search(rw::verified_set.begin(), rw::verified_set.end(), a);
    if (ok) rw::H->override_slots[slot] = rw::entries()[i].tramp;
    return ok ? 1 : 0;
}

// Per-function results so far: to the log, and (all of them) to stats_path as TSV
// addr  name  calls  verified  mismatches  unverifiable.
__declspec(dllexport) void rw_report(void) {
    FILE *f = rw::H->stats_path ? fopen(rw::H->stats_path, "w") : nullptr;
    for (size_t i = 0; i < rw::bound.size(); i++) {
        auto &s = (*rw::stats)[i];
        if (!s.calls) continue;
        if (f) fprintf(f, "%08X\t%s\t%llu\t%llu\t%llu\t%llu\n", rw::entries()[i].addr, rw::entries()[i].name,
                       (unsigned long long)s.calls.load(), (unsigned long long)s.verified.load(),
                       (unsigned long long)s.mismatches.load(), (unsigned long long)s.unverifiable.load());
        if (s.mismatches.load())
            rw::logf("RW %-48s calls %llu verified %llu MISMATCHES %llu", rw::entries()[i].name,
                     (unsigned long long)s.calls.load(), (unsigned long long)s.verified.load(),
                     (unsigned long long)s.mismatches.load());
    }
    if (f) fclose(f);
}

}  // extern "C"
