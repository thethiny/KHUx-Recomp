// SceneDrawRaise::init's lambda bodies (operator() of the std::function __func objects; `self` is the __func,
// the captures follow its vtable). The other slots are in SceneDrawRaise_thunks.cpp.
#include "SceneDrawRaise.hpp"

using namespace scene;

namespace {
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t TRUNC_DF_SF = 0xCD6B24;               // __truncdfsf2 (double -> float)
constexpr uint32_t VectorU64_copy = 0x468431;
constexpr uint32_t VectorU64_dtor = 0x468499;
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, [sp] int, [sp+8] Medal)

constexpr uint32_t DIRECTOR_ANIMATION_INTERVAL = 0xA8;   // cocos2d::Director: double
constexpr uint32_t VSLOT_LWFNODE_ATTACH = 0x2F0;          // LWFNode virtual: attach a movie (sret shared_ptr,
                                                          // path, Movie*, [sp] name, [sp+4] function<void(...)>)
constexpr uint32_t MEDAL_KIND = 0x45C;
constexpr uint32_t USERMEDAL_MASTER = 0x58;
constexpr uint32_t MEDAL_RECORD_SIZE = 0x48;

constexpr uint32_t VT_RaidDemoClose = 0xF65470;           // init::$_3::operator()::lambda
constexpr uint32_t VT_GachaDemoClose = 0xF653EC;          // init::$_5::operator()::lambda

constexpr uint32_t STR_drowMedal = 0xE04B36;              // "drowMedal%03d"
constexpr uint32_t STR_MedalUp = 0xE04DEF;                // "lwf/raisedraw/Medal/Medal_D_Up%d/Medal_D_Up%d.lwf"
constexpr uint32_t STR_MedalRev = 0xE04E21;               // "lwf/raisedraw/Medal/Medal_D_Rev%d/Medal_D_Rev%d.lwf"

inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
inline void releaseIf(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}

// $_0 / $_1 [scene]: the onEnter / onExit hooks
void onEnter(uint32_t self) { call(Node_scheduleUpdate, {ld32(self + 4)}); }
void onExit(uint32_t self) { call(Node_unscheduleUpdate, {ld32(self + 4)}); }

// $_3 / $_5 [layer, shadow, view, scene, ids, node]: the demo movie finished. One frame later (a DelayTime of the
// animation interval on the movie node) the inner lambda removes the demo and opens the results popup.
void demoDone(uint32_t self, uint32_t innerVtable) {
    Frame f;
    uint32_t copy = f.alloc(0x24);           // the lambda, copied ([+4] layer .. [+0x20] node)
    fst32(copy + 4, ld32(self + 4));
    fst32(copy + 8, ld32(self + 8));
    fst32(copy + 0xC, ld32(self + 0xC));
    fst32(copy + 0x10, ld32(self + 0x10));
    call(VectorU64_copy, {copy + 0x14, self + 0x14});
    fst32(copy + 0x10, ld32(self + 0x10));
    uint32_t director = call(fn::Director_getInstance, {});
    uint32_t interval = call(TRUNC_DF_SF, {ld32(director + DIRECTOR_ANIMATION_INTERVAL),
                                           ld32(director + DIRECTOR_ANIMATION_INTERVAL + 4)});
    uint32_t node = ld32(self + 0x20);
    uint32_t delay = call(fn::DelayTime_create, {interval});
    uint32_t inner = f.alloc(0x20);          // [layer, shadow, view, scene, ids] for the inner lambda
    fst32(inner + 4, ld32(copy + 4));
    fst32(inner + 8, ld32(copy + 8));
    fst32(inner + 0xC, ld32(copy + 0xC));
    fst32(inner + 0x10, ld32(copy + 0x10));
    call(VectorU64_copy, {inner + 0x14, copy + 0x14});
    fst32(inner + 0x10, ld32(copy + 0x10));
    uint32_t done = f.alloc(0x18);           // std::function<void()> (callable pointer at +0x10)
    fst32(done + 0x10, 0);
    uint32_t obj = guest::newObject(0x20);
    if (obj) {
        st32(obj, innerVtable);
        st32(obj + 4, ld32(inner + 4));
        st32(obj + 8, ld32(inner + 8));
        st32(obj + 0xC, ld32(inner + 0xC));
        st32(obj + 0x14, ld32(inner + 0x14));
        st32(obj + 0x18, ld32(inner + 0x18));
        st32(obj + 0x1C, ld32(inner + 0x1C));
        fst32(inner + 0x1C, 0);
        fst32(inner + 0x18, 0);
        fst32(inner + 0x14, 0);
        st32(obj + 0x10, ld32(inner + 0x10));
    }
    fst32(done + 0x10, obj);
    uint32_t callFunc = call(fn::CallFunc_create, {done});
    uint32_t seq = call(fn::Sequence_create, {delay, callFunc, 0});
    call(fn::Node_runAction, {node, seq});
    functionDestroy(done);
    call(VectorU64_dtor, {inner + 0x14});
    call(VectorU64_dtor, {copy + 0x14});
}
void raidDemoDone(uint32_t self) { demoDone(self, VT_RaidDemoClose); }
void gachaDemoDone(uint32_t self) { demoDone(self, VT_GachaDemoClose); }

// $_3 / $_5 inner lambda [layer, shadow, view, scene, ids]: remove the demo, open the results popup
void demoClose(uint32_t self) {
    uint32_t layer = ld32(self + 4);
    vcall(layer, node::removeChild, {layer, ld32(self + 8), 1});
    layer = ld32(self + 4);
    vcall(layer, node::removeChild, {layer, ld32(self + 0xC), 1});
    call(draw_raise::OPEN_DRAW_RESULTS_POPUP, {ld32(self + 0x10), self + 0x14});
}

// $_4 / $_6 [index, ids, node, rarity]: the "load" event of movie drowMedal<index+1>: attach the medal movie for
// medal `index` (kind 1: Medal_D_Up<rarity>, kind 2: Medal_D_Rev<rarity>, else Medal_D_Up<rarity + 6>). The
// original frame (0x1160 bytes) is kept: getMedalData and the attach call take stack arguments.
void medalLoad(uint32_t self, const uint32_t *moviePtr) {
    Frame f;
    const uint32_t sp = f.allocExact(0x1160);
    uint32_t movie = *moviePtr;
    call(fn::StringBuffer_ctor, {sp + 0xD40, STR_drowMedal, ld32(self + 4) + 1});
    uint32_t mgr = call(MedalDataManager_getInstance, {});
    uint32_t index = ld32(self + 4), begin = ld32(self + 8);
    if (!(uint32_t(int32_t(ld32(self + 0xC) - begin) >> 3) > index)) {
        call(Vector_throwOutOfRange, {self + 8});
        return;
    }
    uint32_t lo = ld32(begin + index * 8), hi = ld32(begin + index * 8 + 4);
    // the zeroed UserMedal::Medal local at sp+0xCF8 (word +0x2C uninitialised) -> outgoing [sp]=0, [sp+8]=record
    const uint32_t local = sp + 0xCF8;
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4)
        if (k != 0x2C) fst32(local + k, 0);
    fst32(sp, 0);
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4) fst32(sp + 8 + k, ld32(local + k));
    rw::call_exact(MedalDataManager_getMedalData, {sp + 0x8C, mgr, lo, hi});
    int32_t kind = safe32(ld32(ld32(sp + 0x8C) + USERMEDAL_MASTER), MEDAL_KIND);

    uint32_t node = ld32(self + 0x14), rarity = ld32(self + 0x18);
    uint32_t attach = ld32(ld32(node) + VSLOT_LWFNODE_ATTACH);
    uint32_t path, fnObj, out;
    if (kind == 1) {
        path = sp + 0x8F4, fnObj = sp + 0x8D8, out = sp + 0x84;
        call(fn::StringBuffer_ctor, {path, STR_MedalUp, rarity, rarity});
    } else if (kind == 2) {
        path = sp + 0x4D4, fnObj = sp + 0x4B8, out = sp + 0x7C;
        call(fn::StringBuffer_ctor, {path, STR_MedalRev, rarity, rarity});
    } else {
        path = sp + 0xB4, fnObj = sp + 0x98, out = sp + 0x74;
        call(fn::StringBuffer_ctor, {path, STR_MedalUp, rarity + 6, rarity + 6});
    }
    fst32(fnObj + 0x10, 0);                 // an empty std::function
    fst32(sp + 4, fnObj);
    fst32(sp, sp + 0xD40);                  // the movie name
    rw::call_exact(attach, {out, node, path, movie});
    releaseIf(ld32(out + 4));
    functionDestroy(fnObj);
    releaseIf(ld32(sp + 0x90));
}
}  // namespace

RW_FUNCTION(0x6959DA, "SceneDrawRaise::init::$_0::operator()", onEnter);
RW_FUNCTION(0x69593E, "SceneDrawRaise::init::$_1::operator()", onExit);
RW_FUNCTION(0x6955AC, "SceneDrawRaise::init::$_3::operator()", raidDemoDone);
RW_FUNCTION(0x694D40, "SceneDrawRaise::init::$_5::operator()", gachaDemoDone);
RW_FUNCTION(0x695874, "SceneDrawRaise::init::$_3::operator()::lambda::operator()", demoClose);
RW_FUNCTION(0x695008, "SceneDrawRaise::init::$_5::operator()::lambda::operator()", demoClose);
RW_FUNCTION(0x695190, "SceneDrawRaise::init::$_4::operator()(LWF::Movie*)", medalLoad);
RW_FUNCTION(0x694924, "SceneDrawRaise::init::$_6::operator()(LWF::Movie*)", medalLoad);
