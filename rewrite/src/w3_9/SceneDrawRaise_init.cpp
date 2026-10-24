// SceneDrawRaise::init: collects the drawn medals of the last draw (APIManager's draw result), orders them for the
// show (a medal whose rarity score beats every earlier one goes to the front), then plays the draw demo movie
// (raid gacha / gacha result display, one variant per medal count) with a "load" movie event per drawn medal.
//
// The function keeps the original frame layout (0x1490 bytes): getMedalData takes its medal record by value on
// the stack, and the record's word at +0x2C is never initialised (it keeps whatever the stack held).
#include "SceneDrawRaise.hpp"

#include "core/SafeBuffer.hpp"

using namespace scene;
using Self = SceneDrawRaise;

namespace {
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;                // cocos2d::Layer::init
constexpr uint32_t Function_vv_assign = 0x4EB9CD;        // std::function<void()>::operator=(const function&)
constexpr uint32_t SceneManager_hideHeader = 0x61154D;   // (mgr, std::function<void()> by value)
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;   // (mgr, BackGroundType, const std::string&)
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, [sp] int, [sp+8] Medal)
constexpr uint32_t VectorU64_copy = 0x468431;            // std::vector<u64>::vector(const vector&)
constexpr uint32_t VectorU64_dtor = 0x468499;            // std::__vector_base<u64>::~__vector_base
constexpr uint32_t VectorU64_insert = 0x68FA39;          // std::vector<u64>::insert(pos, n, const u64&)
constexpr uint32_t VectorU64_pushBackSlow = 0x4ECB6D;    // std::vector<u64>::__push_back_slow_path
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;    // __vector_base_common::__throw_out_of_range (at())
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;       // (const char* path, void*)
constexpr uint32_t FlashMovieView_create = 0x5682A1;     // (LWFNode*, bool)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;                 // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t MovieHandlerMap_insertUnique = 0x530869;   // map<string, function<void(Movie*)>>::__insert_unique(hint, pair)
constexpr uint32_t MovieHandlerMap_destroy = 0x530811;        // its __tree::destroy(node)
constexpr uint32_t LWF_AddMovieEventHandler = rwlib::LWF::LWF::AddMovieEventHandler;       // LWF::AddMovieEventHandler(name, map)

constexpr uint32_t STR_DeckBG = 0xE03AF2;                // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t STR_RaidGachaDemo = 0xE04AE3;         // "lwf/raisedraw/demo/raidgacha_result_display_%02d/..."
constexpr uint32_t STR_GachaDemo = 0xE04B44;             // "lwf/raisedraw/demo/gacha_result_display_%02d/..."
constexpr uint32_t STR_drowMedal = 0xE04B36;             // "drowMedal%03d"

constexpr uint32_t VT_onEnter = 0xF654C8;                // init::$_0 [scene]: scheduleUpdate
constexpr uint32_t VT_onExit = 0xF6549C;                 // init::$_1 [scene]: unscheduleUpdate
constexpr uint32_t VT_RaidDemoDone = 0xF65444;           // init::$_3
constexpr uint32_t VT_RaidMedalLoad = 0xF65418;          // init::$_4
constexpr uint32_t VT_GachaDemoDone = 0xF653C0;          // init::$_5
constexpr uint32_t VT_GachaMedalLoad = 0xF65394;         // init::$_6

constexpr uint32_t APIMANAGER_DRAW_RESULT = 0x28C;       // APIManager: the last draw's result
constexpr uint32_t DRAW_RESULT_MEDALS = 0x00;            // SafeBuffer array of 72-byte UserMedal::Medal (count +0x20)
constexpr uint32_t DRAW_RESULT_TYPE = 0x18;              // 1: no medal show, 3: show only when ids are listed
constexpr uint32_t DRAW_RESULT_IDS = 0x1C;               // vector<u64>: medals not shown when the filter is on
constexpr uint32_t MEDAL_ARRAY_COUNT = 0x20;
constexpr uint32_t MEDAL_RECORD_SIZE = 0x48;             // hole::network::api::UserMedal::Medal
constexpr uint32_t SCENEMANAGER_DRAW_MODE = 0x294;       // 1 raid gacha, 2 gacha
constexpr uint32_t MEDAL_KIND = 0x45C;                   // master::Medal: 1 / 2 add 0.8 / 0.3 to the show score
constexpr uint32_t MEDAL_RARE = 0x470;
constexpr uint32_t USERMEDAL_MASTER = 0x58;
constexpr uint32_t NODE_ON_ENTER = 0x210;                // Node std::function<void()> hooks
constexpr uint32_t NODE_ON_EXIT = 0x228;
constexpr uint32_t LWFNODE_LWF = 0x3E0;                  // cocos2d::LWFNode: its LWF::LWF*
constexpr uint32_t FLASHVIEW_LOOP_STOP = 0x274;          // FlashMovieView: byte
constexpr uint32_t FLASHVIEW_ON_FINISH = 0x278;          // FlashMovieView: std::function<void()>

// stack-frame scratch (not compared by the verifier)
inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
inline void fst8(uint32_t a, uint8_t v) { *rw::frame_ptr<uint8_t>(a) = v; }
inline void releaseIf(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}

// The zeroed UserMedal::Medal local (its word at +0x2C stays uninitialised) copied to the outgoing arguments:
// [sp] = 0 (medal id override), [sp + 8] = the record.
void medalArgs(uint32_t sp, uint32_t local) {
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4)
        if (k != 0x2C) fst32(local + k, 0);
    fst32(sp, 0);
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4) fst32(sp + 8 + k, ld32(local + k));
}

// shared_ptr<UserMedalData> out = MedalDataManager::getMedalData(id, 0, record)
void getMedalData(uint32_t sp, uint32_t out, uint32_t local, uint32_t lo, uint32_t hi) {
    uint32_t mgr = call(MedalDataManager_getInstance, {});
    medalArgs(sp, local);
    rw::call_exact(MedalDataManager_getMedalData, {out, mgr, lo, hi});
}

// node->setPosition(Vec2(0, shadow's height)) with the Vec2 temporary at `vec`
void placeAtShadowTop(uint32_t node, uint32_t shadow, uint32_t vec) {
    uint32_t size = vcall(shadow, node::getContentSize, {shadow});
    call(Vec2_ctor, {vec, 0, ld32(size + 4)});
    vcall(node, node::setPosition, {node, vec});
    call(Vec2_dtor, {vec});
}

// The draw demo (the raid gacha and gacha branches of the original are the same code with their own locals): a modal shadow, the demo movie for `count` medals on popup layer 2, its finish callback
// (DemoDone: remove it and open the results popup), and a "load" event handler per medal (MedalLoad: attach the
// medal movie of medal i to drowMedal<i+1>).
void playDemo(uint32_t self, uint32_t sp, bool raid) {
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    vcall(shadow, node::setOpacity, {shadow, 0});
    vcall(layer, node::addChild, {layer, shadow});

    uint32_t idsBegin = ld32(sp + 0x1C8), idsEnd = ld32(sp + 0x1CC);
    int32_t bytes = int32_t(idsEnd - idsBegin);
    uint32_t count = uint32_t(bytes >> 3);

    // locals of the two variants
    const uint32_t pathBuf = raid ? 0xF4C : 0x6B4;
    const uint32_t vecTmp = raid ? 0x1A0 : 0x120;
    const uint32_t cap1 = raid ? 0x180 : 0x100;     // [layer, shadow, view, self, ids, node]
    const uint32_t cap2 = raid ? 0x160 : 0xE0;      // the same, moved into the callback
    const uint32_t doneFn = raid ? 0xF30 : 0x698;   // std::function<void()> (callable pointer at +0x10)

    call(fn::StringBuffer_ctor, {sp + pathBuf, raid ? STR_RaidGachaDemo : STR_GachaDemo, count, count});
    uint32_t node = call(AudioLWFNode_create, {sp + pathBuf, 0});
    placeAtShadowTop(node, shadow, sp + vecTmp);
    uint32_t view = call(FlashMovieView_create, {node, 0});
    vcall(layer, node::addChild, {layer, view});

    fst32(sp + cap1, layer);
    fst32(sp + cap1 + 4, shadow);
    fst32(sp + cap1 + 8, view);
    fst32(sp + cap1 + 0xC, self);
    call(VectorU64_copy, {sp + cap1 + 0x10, sp + 0x1AC});
    fst32(sp + cap1 + 0xC, self);
    fst32(sp + cap1 + 0x1C, node);
    fst32(sp + cap2, layer);
    fst32(sp + cap2 + 4, shadow);
    fst32(sp + cap2 + 8, view);
    fst32(sp + cap2 + 0xC, self);
    call(VectorU64_copy, {sp + cap2 + 0x10, sp + cap1 + 0x10});
    fst32(sp + cap2 + 0xC, self);
    fst32(sp + cap2 + 0x1C, node);

    // view->onFinish = DemoDone{layer, shadow, view, self, ids, node}
    fst32(sp + doneFn + 0x10, 0);
    uint32_t obj = guest::newObject(0x24);
    if (obj) {
        st32(obj, raid ? VT_RaidDemoDone : VT_GachaDemoDone);
        st32(obj + 4, ld32(sp + cap2));
        st32(obj + 8, ld32(sp + cap2 + 4));
        st32(obj + 0xC, ld32(sp + cap2 + 8));
        st32(obj + 0x14, ld32(sp + cap2 + 0x10));
        st32(obj + 0x18, ld32(sp + cap2 + 0x14));
        st32(obj + 0x1C, ld32(sp + cap2 + 0x18));
        fst32(sp + cap2 + 0x10, 0);
        fst32(sp + cap2 + 0x14, 0);
        fst32(sp + cap2 + 0x18, 0);
        st32(obj + 0x10, ld32(sp + cap2 + 0xC));
        st32(obj + 0x20, ld32(sp + cap2 + 0x1C));
    }
    fst32(sp + doneFn + 0x10, obj);
    call(Function_vv_assign, {view + FLASHVIEW_ON_FINISH, sp + doneFn});
    functionDestroy(sp + doneFn);
    call(VectorU64_dtor, {sp + cap2 + 0x10});
    st8(view + FLASHVIEW_LOOP_STOP, 1);

    // per medal locals
    const uint32_t recordLocal = raid ? 0xEE8 : 0x650;  // the zeroed UserMedal::Medal
    const uint32_t medalOut = raid ? 0x158 : 0xD8;      // shared_ptr<UserMedalData>
    const uint32_t nameBuf = raid ? 0xAE4 : 0x24C;      // StringBuffer "drowMedal%03d"
    const uint32_t name = raid ? 0x14C : 0xCC;          // std::string of it
    const uint32_t cap3 = raid ? 0x128 : 0xA8;          // [i, ids, node, rarity]
    const uint32_t cap4 = raid ? 0x204 : 0x1E8;         // the same, moved into the handler
    const uint32_t pair = raid ? 0xAB8 : 0x220;         // pair<const string, function<void(Movie*)>> ("load", handler)
    const uint32_t map = raid ? 0x140 : 0xC0;           // map<string, function<void(Movie*)>>

    if (raid ? bytes < 1 : idsEnd == idsBegin) {
        call(VectorU64_dtor, {sp + cap1 + 0x10});
        return;
    }
    for (uint32_t i = 0;;) {
        uint32_t begin = ld32(sp + 0x1C8), end = ld32(sp + 0x1CC);
        uint32_t mgr = call(MedalDataManager_getInstance, {});
        if (!(uint32_t(int32_t(end - begin) >> 3) > i)) {
            call(Vector_throwOutOfRange, {sp + 0x1C8});
            return;
        }
        uint32_t lo = ld32(begin + i * 8), hi = ld32(begin + i * 8 + 4);
        medalArgs(sp, sp + recordLocal);
        rw::call_exact(MedalDataManager_getMedalData, {sp + medalOut, mgr, lo, hi});
        uint32_t row = ld32(ld32(sp + medalOut) + USERMEDAL_MASTER);
        int32_t rarity = safe32(row, MEDAL_RARE);
        call(fn::StringBuffer_ctor, {sp + nameBuf, STR_drowMedal, i + 1});
        uint32_t lwf = ld32(node + LWFNODE_LWF);
        strInit(sp + name, sp + nameBuf, host_strlen(sp + nameBuf));

        fst32(sp + cap3, i);
        call(VectorU64_copy, {sp + cap3 + 4, sp + 0x1C8});
        fst32(sp + cap3 + 0x10, node);
        fst32(sp + cap3 + 0x14, uint32_t(rarity));
        // the pair's key: "load" (short string)
        fst8(sp + pair, 8);
        fst8(sp + pair + 4, 'd');
        fst8(sp + pair + 3, 'a');
        fst8(sp + pair + 2, 'o');
        fst8(sp + pair + 1, 'l');
        fst8(sp + pair + 5, 0);
        fst32(sp + cap4, ld32(sp + cap3));
        fst32(sp + cap4 + 4, ld32(sp + cap3 + 4));
        fst32(sp + cap4 + 8, ld32(sp + cap3 + 8));
        fst32(sp + cap4 + 0xC, ld32(sp + cap3 + 0xC));
        fst32(sp + cap3 + 4, 0);
        fst32(sp + cap3 + 8, 0);
        fst32(sp + cap3 + 0xC, 0);
        fst32(sp + cap4 + 0x10, ld32(sp + cap3 + 0x10));
        fst32(sp + cap4 + 0x14, ld32(sp + cap3 + 0x14));
        // the pair's value: MedalLoad{i, ids, node, rarity}
        fst32(sp + pair + 0x20, 0);
        uint32_t handler = guest::newObject(0x1C);
        if (handler) {
            st32(handler, raid ? VT_RaidMedalLoad : VT_GachaMedalLoad);
            st32(handler + 4, ld32(sp + cap4));
            st32(handler + 8, ld32(sp + cap4 + 4));
            st32(handler + 0xC, ld32(sp + cap4 + 8));
            st32(handler + 0x10, ld32(sp + cap4 + 0xC));
            fst32(sp + cap4 + 4, 0);
            fst32(sp + cap4 + 8, 0);
            fst32(sp + cap4 + 0xC, 0);
            st32(handler + 0x18, ld32(sp + cap4 + 0x14));
            st32(handler + 0x14, ld32(sp + cap4 + 0x10));
        }
        fst32(sp + pair + 0x20, handler);
        call(VectorU64_dtor, {sp + cap4 + 4});
        // map{ pair }: an empty tree (begin = end node, no root), one insert
        fst32(sp + map, sp + map + 4);
        fst32(sp + map + 4, 0);
        fst32(sp + map + 8, 0);
        call(MovieHandlerMap_insertUnique, {sp + map, sp + map + 4, sp + pair});
        call(LWF_AddMovieEventHandler, {lwf, sp + name, sp + map});
        call(MovieHandlerMap_destroy, {sp + map, ld32(sp + map + 4)});
        functionDestroy(sp + pair + 0x10);
        strFree(sp + pair);
        call(VectorU64_dtor, {sp + cap3 + 4});
        strFree(sp + name);
        releaseIf(ld32(sp + medalOut + 4));

        ++i;
        if (raid) {
            if (int32_t(i) >= int32_t(count)) break;      // the count taken before the loop
        } else {
            uint32_t n = uint32_t(int32_t(ld32(sp + 0x1CC) - ld32(sp + 0x1C8)) >> 3);
            if (i >= n) break;
        }
    }
    call(VectorU64_dtor, {sp + cap1 + 0x10});
}
}  // namespace

uint32_t SceneDrawRaise::init(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    const uint32_t sp = f.allocExact(0x1490);

    if (call(Layer_init, {self}) != 1) return 0;

    // onEnter / onExit: schedule / unschedule update()
    fst32(sp + 0x1458, VT_onEnter);
    fst32(sp + 0x145C, self);
    fst32(sp + 0x1468, sp + 0x1458);
    call(Function_vv_assign, {self + NODE_ON_ENTER, sp + 0x1458});
    functionDestroy(sp + 0x1458);
    fst32(sp + 0x1440, VT_onExit);
    fst32(sp + 0x1444, self);
    fst32(sp + 0x1450, sp + 0x1440);
    call(Function_vv_assign, {self + NODE_ON_EXIT, sp + 0x1440});
    functionDestroy(sp + 0x1440);

    uint8_t showFilter = 1;
    s->flag_2AE = 1;
    uint32_t mgr = sceneManager();
    fst32(sp + 0x1438, 0);                          // an empty std::function<void()>
    call(SceneManager_hideHeader, {mgr, sp + 0x1428});
    functionDestroy(sp + 0x1428);
    mgr = sceneManager();
    strInit(sp + 0x1D4, STR_DeckBG, 0x1A);
    call(SceneManager_changeBackground, {mgr, 0, sp + 0x1D4});
    strFree(sp + 0x1D4);

    uint32_t api = call(fn::APIManager_getInstance, {});
    if (ld32(ld32(api + APIMANAGER_DRAW_RESULT) + DRAW_RESULT_TYPE) == 1) showFilter = 0;
    s->flag_2AC = showFilter;
    uint32_t result = ld32(api + APIMANAGER_DRAW_RESULT);
    if (ld32(result + DRAW_RESULT_TYPE) == 3 && ld32(result + DRAW_RESULT_IDS + 4) == ld32(result + DRAW_RESULT_IDS)) {
        showFilter = 0;
        s->flag_2AC = 0;
    }

    // ids (vector<u64> at sp+0x1C8): the drawn medals in show order
    fst32(sp + 0x1C8, 0);
    fst32(sp + 0x1CC, 0);
    fst32(sp + 0x1D0, 0);
    uint32_t drawResult = ld32(api + APIMANAGER_DRAW_RESULT);
    uint32_t count = ld32(ld32(drawResult + DRAW_RESULT_MEDALS) + MEDAL_ARRAY_COUNT);
    if (count) {
        float best = 0.0f;
        for (uint32_t i = 0;;) {
            if (i) showFilter = s->flag_2AC;
            uint32_t offset = 72 * i;
            bool skip = false;
            if (showFilter) {
                const SafeBuffer *medals = rw::host_ptr<SafeBuffer>(ld32(drawResult + DRAW_RESULT_MEDALS));
                SafeBuffer::read(medals, rw::host_ptr<uint8_t>(sp + 0x13E0), medals->data.get() + offset, 72, offset);
                uint32_t r = ld32(api + APIMANAGER_DRAW_RESULT);
                uint32_t p = ld32(r + DRAW_RESULT_IDS), e = ld32(r + DRAW_RESULT_IDS + 4);
                uint32_t lo = ld32(sp + 0x13E0), hi = ld32(sp + 0x13E4);
                for (; p != e; p += 8) {
                    if (ld32(p) == lo && ld32(p + 4) == hi) {
                        skip = true;
                        break;
                    }
                }
            }
            if (!skip) {
                const SafeBuffer *medals = rw::host_ptr<SafeBuffer>(ld32(drawResult + DRAW_RESULT_MEDALS));
                SafeBuffer::read(medals, rw::host_ptr<uint8_t>(sp + 0x1398), medals->data.get() + offset, 72, offset);
                uint32_t lo = ld32(sp + 0x1398), hi = ld32(sp + 0x139C);
                fst32(sp + 0x1C0, lo);                 // the id (u64) pushed below
                fst32(sp + 0x1C4, hi);
                getMedalData(sp, sp + 0x1B8, sp + 0x1350, lo, hi);
                uint32_t row = ld32(ld32(sp + 0x1B8) + USERMEDAL_MASTER);
                int32_t kind = safe32(row, MEDAL_KIND);
                int32_t rarity = safe32(row, MEDAL_RARE);
                double bonus = kind == 1 ? 0.8 : kind == 2 ? 0.3 : 0.0;
                float score = float(double(float(rarity)) + bonus);
                bool better = score > best;
                float newBest = better ? score : best;
                uint32_t begin = ld32(sp + 0x1C8), end = ld32(sp + 0x1CC);
                if (end != begin && better) {
                    call(VectorU64_insert, {sp + 0x1C8, begin, 1, sp + 0x1C0});
                } else if (end == ld32(sp + 0x1D0)) {
                    call(VectorU64_pushBackSlow, {sp + 0x1C8, sp + 0x1C0});
                } else {
                    uint32_t next = 0;
                    if (end) {
                        st32(end, ld32(sp + 0x1C0));
                        st32(end + 4, ld32(sp + 0x1C4));
                        next = ld32(sp + 0x1CC);
                    }
                    fst32(sp + 0x1CC, next + 8);
                }
                releaseIf(ld32(sp + 0x1BC));
                best = newBest;
            }
            if (++i >= count) break;
        }
    }

    s->field_2A0 = 0;
    fst32(sp + 0x1A8, self);
    call(VectorU64_copy, {sp + 0x1AC, sp + 0x1C8});
    uint32_t mode = ld32(sceneManager() + SCENEMANAGER_DRAW_MODE);
    if (mode == 1) playDemo(self, sp, true);
    else if (mode == 2) playDemo(self, sp, false);
    call(VectorU64_dtor, {sp + 0x1AC});
    call(VectorU64_dtor, {sp + 0x1C8});
    return 1;
}
RW_FUNCTION(0x68EB20, "SceneDrawRaise::init", SceneDrawRaise::init);
