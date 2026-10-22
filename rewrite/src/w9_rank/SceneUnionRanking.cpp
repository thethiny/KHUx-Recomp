// SceneUnionRanking: last week's union standings, shown once at startup when /ranking/parade sends
// rankingUnionResult (APIManager +0x2EC: {unionId, rank, lux (u64)} per union). The player's own union gets the
// "unionRanking_<rank>" LWF; its marks (mc_mark1..) show the unions' ids in rank order and its score digits
// (score<n>_<digit>) roll until stopSelfRoll swaps in the "union_fix<digit>" pieces. A tap closes it.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t memcpy = 0x40E164;
constexpr uint32_t strlen = 0x40E08C;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t throw_out_of_range = 0x47CCE1;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t VectorInt_copy = 0x4AF707;            // vector<int>(const vector<int>&)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;         // function<void()>::operator=(const function&)
constexpr uint32_t VectorU64_pushBackSlow = 0x4ECB6D;
constexpr uint32_t VectorInt_insert = 0x516209;          // insert(const_iterator, const int&)
constexpr uint32_t MovieHandlerMap_destroy = 0x530811, MovieHandlerMap_insertUnique = 0x530869;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t FlashMovieView_create = 0x5682A1, FlashMovieView_gotoLastFrame = 0x568871;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;       // (const char* path, void*)
constexpr uint32_t SceneLoginBonus_openMemoryBonus = 0x6FA6F5;
constexpr uint32_t TextRendererFunction_assign = 0x6F6095;   // function<void(const string&, LWF::TextRenderer*)>::operator=
constexpr uint32_t VectorU64_insert = 0x6AD345;
constexpr uint32_t VectorMovie_resize = 0x7384D9;
constexpr uint32_t runOutAnimation = 0x738841;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t LWF_AddMovieEventHandler = 0x808645;
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_init = 0x88A8A1;
constexpr uint32_t Scene_create = 0x8A5F37;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t truncdfsf2 = 0xCD6B24;
constexpr uint32_t uldivmod = 0xCD733C, umoddi3 = 0xCD7805;
}  // namespace g

constexpr uint32_t SIZE = 0x2B8;
constexpr uint32_t VTABLE = 0xFA6CA0;
// fields
constexpr uint32_t F_ON_A = 0x210, F_ON_B = 0x228;      // std::function<void()> (Layer callbacks)
constexpr uint32_t F_MOVIES = 0x28C;                    // vector<LWF::Movie*>: the score digit movies
constexpr uint32_t F_STOPPED = 0x298;                   // bool: stopSelfRoll ran
constexpr uint32_t F_TAPPABLE = 0x2A4;                  // bool
constexpr uint32_t F_RANK_FRAMES = 0x29C;               // frames the roll runs (by the union's rank)
constexpr uint32_t F_DIGITS = 0x2A0;                    // score digits per union (12)
constexpr uint32_t F_LUX = 0x2A8;                       // vector<unsigned long long>: the unions' Lux, own first
constexpr uint32_t API_PARADE = 0x2EC;                  // APIManager: vector<{unionId, rank, lux}>
constexpr uint32_t API_USER = 0x94;                     // APIManager: the user's SafeBuffer (unionId at +0x30)
constexpr uint32_t LWFNODE_LWF = 0x3E0, LWF_TEXT_RENDERER = 0x110;
constexpr uint32_t FLASH_ON_END = 0x278, FLASH_AUTO_REMOVE = 0x274;
// lambda vtables
constexpr uint32_t VT_ON_A = 0xF6D664, VT_ON_B = 0xF6D638, VT_RUN_OUT = 0xF6D60C;
constexpr uint32_t VT_FLASH_END = 0xF6D588;            // $_7 {this, flash, lwf}
constexpr uint32_t VT_ROLL_STOP = 0xF6D5B4;            // $_7 inner {flash, this, lwf}
constexpr uint32_t VT_TAP = 0xF6D5E0;                  // {this, flash}
constexpr uint32_t VT_TEXT = 0xF6D714;                 // heap 0x10 {vector<int> union ids}
constexpr uint32_t VT_MARK_LOAD = 0xF6D6E8;            // {lwfNode, union id}
constexpr uint32_t VT_SCORE_LOAD = 0xF6D6BC;           // heap 0x28 {?, this, union index, lwfNode, digit (u64), j, j}
constexpr uint32_t VT_ENTER_FRAME = 0xF6D690;          // {this, lwfNode}
// strings
constexpr uint32_t S_Empty = 0xDF8A1A, S_Root = 0xDFE0ED, S_EnterFrame = 0xDFE0F3;
constexpr uint32_t S_MarkFmt = 0xE071B9, S_ScoreFmt = 0xE071C3, S_RankingLwf = 0xE071CE, S_FixLwf = 0xE07203;
constexpr uint32_t S_TapJson = 0xDFFCF2, S_Tap = 0xDFFB2B, S_Animation1 = 0xDFD0AF;

// a std::function slot set from a stack lambda (operator= copies it), then the stack copy destroyed
void setFunction(uint32_t slot, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    const uint32_t fo = newFunction(f, vtable, captures);
    call(g::FunctionVV_assign, {slot, fo});
    functionDestroy(fo);
}
// "load" as libc++ builds it inline (short string: size byte 8, the characters, NUL)
void shortLoad(uint32_t s) {
    st8(s, 8);
    st8(s + 4, 'd');
    st8(s + 3, 'a');
    st8(s + 2, 'o');
    st8(s + 1, 'l');
    st8(s + 5, 0);
}
// map<string, function<void(Movie*)>> {pair} -> LWF::AddMovieEventHandler(name, map); then the map and the pair go
void addHandler(uint32_t lwf, uint32_t name, uint32_t map, uint32_t pair) {
    st32(map, map + 4);
    st32(map + 4, 0);
    st32(map + 8, 0);
    call(g::MovieHandlerMap_insertUnique, {map, map + 4, pair});
    call(g::LWF_AddMovieEventHandler, {lwf, name, map});
    call(g::MovieHandlerMap_destroy, {map, ld32(map + 4)});
    functionDestroy(pair + 0x10);
    strFree(pair);
    strFree(name);
}
}  // namespace

struct SceneUnionRanking {
    static uint32_t create();
    static uint32_t createScene();
    static uint32_t init(uint32_t self);
    static void settingUnionRankingLwf(uint32_t self, uint32_t lwfNode, uint32_t ids, uint32_t lux, int32_t rank);
    static void stopSelfRoll(uint32_t self, uint32_t lwfNode);
    static void flashEnd(uint32_t fo);
    static void rollStop(uint32_t fo);
    static void memoryBonus(uint32_t fo);
};

uint32_t SceneUnionRanking::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    st32(me, VTABLE);
    st32(me + 0x29C, 0);
    st32(me + 0x2A0, 0);
    st8(me + F_TAPPABLE, 0);
    st32(me + 0x2A8, 0);
    st32(me + 0x2AC, 0);
    st32(me + 0x2B0, 0);
    st8(me + F_STOPPED, 0);
    st32(me + 0x294, 0);
    st32(me + 0x290, 0);
    st32(me + 0x28C, 0);
    if (init(me) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

uint32_t SceneUnionRanking::createScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = create();
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}

// The own union's LWF (by its rank); the other unions' ids and everyone's Lux collected, the own union first.
// Without the own union in the list the screen leaves at once (runOutAnimation).
uint32_t SceneUnionRanking::init(uint32_t self) {
    if (call(g::Layer_init, {self}) != 1) return 0;
    setFunction(self + F_ON_A, VT_ON_A, {self});
    setFunction(self + F_ON_B, VT_ON_B, {self});
    Frame f;
    const uint32_t ids = f.alloc(16);                      // vector<int> + the user's union id after it
    {
        const uint32_t api = call(g::APIManager_getInstance, {});
        const uint32_t sb = ld32(api + API_USER);
        rw::call(g::SafeBuffer_read, {sb, ids + 0xC, ld32(sb + 0xC) + 0x30, 4, 0x30});
    }
    const uint32_t own = ld32(ids + 0xC);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t lwfNode = 0;
    int32_t rank = 1;
    const uint32_t list = ld32(call(g::APIManager_getInstance, {}) + API_PARADE);
    const uint32_t begin = ld32(list), end = ld32(list + 4);
    const uint32_t luxes = self + F_LUX;
    for (uint32_t e = begin; e != end; e += 16) {
        if (ld32(e) == own) {
            rank = int32_t(ld32(e + 4));
            Frame g2;
            const uint32_t path = g2.alloc(0x404);
            call(g::StringBuffer_ctor, {path, S_RankingLwf, uint32_t(rank), uint32_t(rank)});
            lwfNode = call(g::AudioLWFNode_create, {path, 0});
            call(g::VectorInt_insert, {ids, ld32(ids), e});
            call(g::VectorU64_insert, {luxes, ld32(luxes), e + 8});
            continue;
        }
        const uint32_t iend = ld32(ids + 4);
        if (iend == ld32(ids + 8)) {
            call(g::VectorInt_pushBackSlow, {ids, e});
        } else {
            if (iend) st32(iend, ld32(e));
            st32(ids + 4, (iend ? iend : 0) + 4);
        }
        const uint32_t lend = ld32(luxes + 4);
        if (lend == ld32(luxes + 8)) {
            call(g::VectorU64_pushBackSlow, {luxes, e + 8});
        } else {
            uint32_t at = 0;
            if (lend) {
                st32(lend, ld32(e + 8));
                st32(lend + 4, ld32(e + 12));
                at = ld32(luxes + 4);
            }
            st32(luxes + 4, at + 8);
        }
    }
    if (!lwfNode) {
        Frame g2;
        const uint32_t fo = newFunction(g2, VT_RUN_OUT, {});
        call(g::runOutAnimation, {self, fo});
        functionDestroy(fo);
    } else {
        settingUnionRankingLwf(self, lwfNode, ids, luxes, rank);
        const uint32_t flash = call(g::FlashMovieView_create, {lwfNode, 0});
        {
            Frame g2;
            const uint32_t fo = newFunction(g2, VT_FLASH_END, {self, flash, lwfNode});
            call(g::FunctionVV_assign, {flash + FLASH_ON_END, fo});
            functionDestroy(fo);
        }
        st8(flash + FLASH_AUTO_REMOVE, 0);
        vcall(self, node::addChild, {self, flash});
    }
    call(g::VectorBaseInt_dtor, {ids});
    return 1;
}

// The LWF's handlers: the text renderer gets the union ids; mc_mark<k> shows union k's mark, score<k>_<j> its
// j-th Lux digit (from the right; 12 digits), and the root's enterFrame drives the roll.
// The original keeps its locals in one 0x994-byte frame; the movie-event maps sit on it at their original offsets
// (a map's root node keeps a pointer to the map's end node, a frame address, on the heap).
void SceneUnionRanking::settingUnionRankingLwf(uint32_t self, uint32_t lwfNode, uint32_t ids, uint32_t lux,
                                               int32_t rank) {
    Frame top;
    const uint32_t sp = top.allocExact(0x994 + 0x14);
    const uint32_t textFn = sp + 0x970;
    const uint32_t lwfField = lwfNode + LWFNODE_LWF;
    {
        const uint32_t lwf = ld32(lwfField);
        const uint32_t copy = sp + 0xDC;
        call(g::VectorInt_copy, {copy, ids});
        st32(textFn + 0x10, 0);
        const uint32_t obj = call(fn::operator_new, {0x10});
        if (obj) {
            st32(obj, VT_TEXT);
            st32(obj + 4, ld32(copy));
            st32(obj + 8, ld32(copy + 4));
            st32(obj + 0xC, ld32(copy + 8));
            st32(copy, 0);
            st32(copy + 4, 0);
            st32(copy + 8, 0);
        }
        st32(textFn + 0x10, obj);
        call(g::TextRendererFunction_assign, {lwf + LWF_TEXT_RENDERER, textFn});
        functionDestroy(textFn);
        call(g::VectorBaseInt_dtor, {copy});
    }
    st32(self + F_RANK_FRAMES, 0);
    switch (rank) {
    case 1: case 2: st32(self + F_RANK_FRAMES, 143); break;
    case 3: case 4: st32(self + F_RANK_FRAMES, 119); break;
    case 5: st32(self + F_RANK_FRAMES, 109); break;
    default: break;
    }
    st32(self + F_DIGITS, 12);
    {
        const uint32_t b = ld32(self + F_MOVIES), e = ld32(self + F_MOVIES + 4);
        if (e != b) st32(self + F_MOVIES + 4, e + (~(e - 4 - b) & 0xFFFFFFFCu));
    }
    call(g::VectorMovie_resize, {self + F_MOVIES, 12});
    st8(self + F_STOPPED, 0);
    for (uint32_t k = 0; k < (ld32(ids + 4) - ld32(ids)) >> 2; k++) {
        const uint32_t id = ld32(ld32(ids) + 4 * k);
        {
            const uint32_t lwf = ld32(lwfField);
            const uint32_t buf = sp + 0x56C, name = sp + 0xD0, pair = sp + 0x540, map = sp + 0xC4;
            call(g::StringBuffer_ctor, {buf, S_MarkFmt, k + 1});
            strInit(name, buf, call(g::strlen, {buf}));
            shortLoad(pair);
            st32(pair + 0x20, pair + 0x10);
            st32(pair + 0x10, VT_MARK_LOAD);
            st32(pair + 0x14, lwfNode);
            st32(pair + 0x18, id);
            addHandler(lwf, name, map, pair);
        }
        if ((ld32(lux + 4) - ld32(lux)) >> 3 <= k) call(g::throw_out_of_range, {lux});
        if (int32_t(ld32(self + F_DIGITS)) < 1) continue;
        const uint32_t at = ld32(lux) + 8 * k;
        uint64_t value = uint64_t(ld32(at)) | uint64_t(ld32(at + 4)) << 32;
        int32_t j = 0;
        do {
            const uint32_t lwf = ld32(lwfField);
            const uint32_t buf = sp + 0x13C, name = sp + 0xB8, pair = sp + 0x110, map = sp + 0xAC;
            call(g::StringBuffer_ctor, {buf, S_ScoreFmt, k + 1, uint32_t(j + 1)});
            strInit(name, buf, call(g::strlen, {buf}));
            shortLoad(pair);
            st32(pair + 0x20, 0);
            const uint64_t digit = value % 10;
            const uint32_t obj = call(fn::operator_new, {0x28});
            if (obj) {
                st32(obj, VT_SCORE_LOAD);
                st32(obj + 8, self);
                st32(obj + 0xC, k);
                st32(obj + 0x10, lwfNode);
                st32(obj + 0x18, uint32_t(digit));
                st32(obj + 0x1C, uint32_t(digit >> 32));
                st32(obj + 0x20, uint32_t(j));
                st32(obj + 0x24, uint32_t(j));
            }
            st32(pair + 0x20, obj);
            addHandler(lwf, name, map, pair);
            value /= 10;
            j++;
        } while (j < int32_t(ld32(self + F_DIGITS)));
    }
    {
        const uint32_t lwf = ld32(lwfField);
        const uint32_t name = sp + 0xA0, pair = sp + 0xE8, map = sp + 0x94;
        strInit(name, S_Root, 5);
        st8(pair, 20);
        call(g::memcpy, {pair + 1, S_EnterFrame, 10});
        st8(pair + 0xB, 0);
        st32(pair + 0x20, pair + 0x10);
        st32(pair + 0x10, VT_ENTER_FRAME);
        st32(pair + 0x18, lwfNode);
        st32(pair + 0x14, self);
        addHandler(lwf, name, map, pair);
    }
}

// The roll ends: each of the own union's 12 score digit movies gets its "union_fix<digit>" piece (the digits of
// the first Lux in the list, the own union's).
void SceneUnionRanking::stopSelfRoll(uint32_t self, uint32_t lwfNode) {
    st8(self + F_STOPPED, 1);
    const uint32_t lux = ld32(self + F_LUX);
    if (ld32(self + F_LUX + 4) == lux) call(g::throw_out_of_range, {self + F_LUX});
    if (int32_t(ld32(self + F_DIGITS)) < 1) return;
    uint64_t value = uint64_t(ld32(lux)) | uint64_t(ld32(lux + 4)) << 32;
    for (int32_t k = 0; k < int32_t(ld32(self + F_DIGITS)); k++) {
        const uint64_t digit = value % 10;
        Frame f;
        const uint32_t path = f.alloc(0x404);
        call(g::StringBuffer_ctor, {path, S_FixLwf, uint32_t(digit), uint32_t(digit >> 32), uint32_t(digit),
                                    uint32_t(digit >> 32)});
        const uint32_t movie = ld32(ld32(self + F_MOVIES) + 4 * uint32_t(k));
        const uint32_t fo = newEmptyFunction(f);
        const uint32_t out = f.alloc(8);
        vcall(lwfNode, 0x2F0, {out, lwfNode, path, movie, S_Empty, fo});    // attachLWF
        if (ld32(out + 4)) call(fn::shared_release, {ld32(out + 4)});
        functionDestroy(fo);
        value /= 10;
    }
}

// $_7 (the LWF finished, {this, flash, lwf}): once the roll stopped, wait one frame and stop it for good
void SceneUnionRanking::flashEnd(uint32_t fo) {
    const uint32_t self = ld32(fo + 4);
    if (!ld8(self + F_TAPPABLE)) return;
    const uint32_t flash = ld32(fo + 8), lwf = ld32(fo + 0xC);
    const uint32_t dir = call(fn::Director_getInstance, {});
    const uint32_t interval = call(g::truncdfsf2, {ld32(dir + 0xA8), ld32(dir + 0xAC)});
    const uint32_t delay = call(fn::DelayTime_create, {interval});
    Frame f;
    const uint32_t stop = newFunction(f, VT_ROLL_STOP, {flash, self, lwf});
    const uint32_t cb = call(fn::CallFunc_create, {stop});
    const uint32_t seq = call(fn::Sequence_create, {delay, cb, 0});
    call(fn::Node_runAction, {lwf, seq});
    functionDestroy(stop);
}

// the inner lambda {flash, this, lwf}: the LWF on its last frame, the fixed digits in, a "tap the screen" armature
// and a full-screen button that closes the screen
void SceneUnionRanking::rollStop(uint32_t fo) {
    const uint32_t flash = ld32(fo + 4), self = ld32(fo + 8);
    call(g::FlashMovieView_gotoLastFrame, {flash});
    stopSelfRoll(self, ld32(fo + 0xC));
    const uint32_t mgr = call(g::ArmatureDataManager_getInstance, {});
    Frame f;
    {
        const uint32_t s = f.alloc(12);
        const uint32_t data = call(fn::operator_new, {0x40});
        st32(s, 0x41);
        st32(s + 4, 50);
        st32(s + 8, data);
        call(g::memcpy, {data, S_TapJson, 50});
        st8(data + 50, 0);
        call(g::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        strFree(s);
    }
    uint32_t armature;
    {
        const uint32_t s = f.alloc(12);
        const uint32_t data = call(fn::operator_new, {0x20});
        st32(s, 0x21);
        st32(s + 4, 20);
        st32(s + 8, data);
        call(g::memcpy, {data, S_Tap, 20});
        st8(data + 20, 0);
        armature = call(g::Armature_create, {s});
        strFree(s);
    }
    vcall(self, node::addChildZ, {self, armature, 3});
    {
        const uint32_t size = f.alloc(8);
        call(fn::Director_getVisibleSize, {size, call(fn::Director_getInstance, {})});
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(ldf(size) * 0.5f), fbits(30.0f)});
        vcall(armature, node::setPosition, {armature, pos});
        call(g::Vec2_dtor, {pos});
    }
    {
        const uint32_t animation = vcall(armature, 0x298, {armature});    // getAnimation
        const uint32_t s = f.alloc(12);
        st8(s, 20);
        call(g::memcpy, {s + 1, S_Animation1, 10});
        st8(s + 0xB, 0);
        vcall(animation, 0x68, {animation, s, 0xFFFFFFFFu, 0xFFFFFFFFu});   // play(name, -1, -1)
        strFree(s);
    }
    uint32_t button;
    {
        const uint32_t s = f.alloc(12);
        st8(s, 0);
        st8(s + 1, 0);
        button = call(g::ImageButton_create, {s, 0});
        strFree(s);
    }
    call(g::ImageButton_setTargetView, {button, self, fbits(1.0f)});
    vcall(self, node::addChildZ, {self, button, 3});
    const uint32_t tap = newFunction(f, VT_TAP, {self, flash});
    call(g::ImageButton_addTouchEventListener, {button, tap});
    functionDestroy(tap);
}

// SceneLoginBonus lambda {scene}: openMemoryBonus with an empty callback
void SceneUnionRanking::memoryBonus(uint32_t fo) {
    Frame f;
    const uint32_t cb = newEmptyFunction(f);
    call(g::SceneLoginBonus_openMemoryBonus, {ld32(fo + 4), cb});
    functionDestroy(cb);
}

RW_FUNCTION(0x6FAE08, "SceneUnionRanking::create", SceneUnionRanking::create);
RW_FUNCTION(0x737CFC, "SceneUnionRanking::createScene", SceneUnionRanking::createScene);
RW_FUNCTION(0x738508, "SceneUnionRanking::init", SceneUnionRanking::init);
RW_FUNCTION(0x737DD0, "SceneUnionRanking::settingUnionRankingLwf", SceneUnionRanking::settingUnionRankingLwf);
RW_FUNCTION(0x73888C, "SceneUnionRanking::stopSelfRoll", SceneUnionRanking::stopSelfRoll);
RW_FUNCTION(0x738A68, "SceneUnionRanking::init $_7 __func::operator() (sub_738A68)", SceneUnionRanking::flashEnd);
RW_FUNCTION(0x738BF8, "SceneUnionRanking::init $_7 inner lambda __func::operator() (sub_738BF8)",
            SceneUnionRanking::rollStop);
RW_FUNCTION(0x6FB25C, "SceneLoginBonus openMemoryBonus lambda __func::operator() (sub_6FB25C)",
            SceneUnionRanking::memoryBonus);
