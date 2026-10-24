// SceneEvolveMedal::init: the Evolve screen. Callbacks (two scene functions, back key, scene finish, Back button),
// the MedalEvoScene layout and its panels, the medal (the one picked in the Medal List) and its evolved form (its
// UserMedal row with level 1 under the evolved medal id): images, rarity, the comparison panel, the cost and the
// player's munny. The materials: the evolve medal ids (up to 5) grouped by id, the owned medals that can be used
// for each (not the medal itself, not favorites, not the protected medal, not in a deck), sorted; the first of each
// group is taken (its id kept on the scene, flags for skills / burst / bonuses it carries). One cell per material
// with the owned count and a touch listener, then the Evolve button (enabled at max level with every material and
// enough munny), Sell and the hint text.
#include "../scene/SceneUi.hpp"

#include <cstring>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t function_vv_assign = 0x4EB9CD;          // std::function<void()>::operator=(const function&)
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneManager_setFinishSceneCallback = 0x613E3F;
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_backBtnSetPosY = 0x45AFB5;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_updateMedalRarePanel = 0x4619F5;
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;
constexpr uint32_t SceneUtil_getMedalMaxRare = 0x462049;
constexpr uint32_t SceneUtil_setStatusChangeColor = 0x4659F1;
constexpr uint32_t SceneUtil_setWarningColor = 0x4686A9;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x474615;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t HashULLInt_find = 0x490275;              // unordered_map<ull, int>::find
constexpr uint32_t UllSet_copy = 0x4740A9, UllSet_move = 0x6A5035, UllSet_destroy = 0x482619;
constexpr uint32_t UllSet_insert = 0x482649;                // __insert_unique(pair out, set, const ull&)
constexpr uint32_t VecInt_pushSlow = 0x4D6CC1;
constexpr uint32_t VecInt_destroy = 0x478961;
constexpr uint32_t VecUll_pushSlowMove = 0x4B33F9, VecUll_pushSlowCopy = 0x4ECB6D, VecUll_insert = 0x6AD345;
constexpr uint32_t VecSp_pushSlow = 0x487A39;               // vector<shared_ptr<UserMedalData>>::push_back slow
constexpr uint32_t VecSp_destroy = 0x46B119;
constexpr uint32_t VecVecSp_pushSlow = 0x6B5C69, VecVecSp_destroy = 0x6ADAB1;
constexpr uint32_t MapInt_index = 0x6AD2C5, MapInt_find = 0x6B5C25, MapInt_at = 0x6AD5CD, MapInt_destroy = 0x5BFE0D;
constexpr uint32_t throw_out_of_range = 0x47CCE1;
constexpr uint32_t sortMaterials = 0x6B3909;                // sub_6B3908(first, last)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t MedalDataManager_getMedalLargeImagePath = 0x76B1E5;
constexpr uint32_t MedalDataManager_isFavoriteMedal = 0x76B249;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageButton_setTargetImageView = 0x531631, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t initEvolutLayer = 0x6AC519, createMaterialMedal = 0x6AD681;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_copy = rwlib::cocos2d::Rect::ctor_base__RKS0_, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addSceneGraph = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t function_touchBegan_swap = 0x483E4D, function_touchEnded_swap = 0x4F3699;
constexpr uint32_t operator_new_array = 0xC98F6F, operator_delete_array = 0xC98FB3;
constexpr uint32_t floatsisf = 0xCD6D68;
}  // namespace g

// lambda vtables (__func<...>)
constexpr uint32_t VT_SceneFn210 = 0xF66574, VT_SceneFn228 = 0xF66548, VT_BackKey = 0xF6651C;
constexpr uint32_t VT_Finish = 0xF664F0, VT_BackButton = 0xF664C4, VT_MedalTap = 0xF6646C;
constexpr uint32_t VT_CellBegan = 0xF66440, VT_CellEnded = 0xF66414, VT_Evolve = 0xF662B4, VT_Sell = 0xF66230;
constexpr uint32_t G_targetScale = 0xFD65F4;                // GOT: the ImageButton target scale float

constexpr uint32_t S_Bg = 0xE03AF2, S_Json = 0xE0541B, S_LeftUI = 0xDF85F0, S_UnderUI = 0xDFE20F;
constexpr uint32_t S_CenterLeft = 0xE05448, S_CenterRight = 0xE04ED9, S_ButtonBack = 0xE03DBA;
constexpr uint32_t S_DummyMedal1 = 0xE05456, S_StarArea1 = 0xE05463, S_DummyMedal2 = 0xE0546E;
constexpr uint32_t S_StarArea2 = 0xDFDF71, S_Empty = 0xDF8A1A, S_BaseMoney1 = 0xE0502E, S_BaseMoney2 = 0xE0503A;
constexpr uint32_t S_TxtMoney = 0xDFE7A3, S_TxtMoneyLabel = 0xDFE7AD, S_Area = 0xDF9378;
constexpr uint32_t S_ButtonLimitCut = 0xDF8D54, S_TxtLimitCut = 0xDF8D64, S_But01 = 0xDFD027;
constexpr uint32_t S_ButtonSell1 = 0xE04F01, S_TxtSell1 = 0xE04F0E, S_But06 = 0xE04F18, S_TxtWording = 0xDF862F;

// SceneEvolveMedal fields
constexpr uint32_t F_FN210 = 0x210, F_FN228 = 0x228, F_ROOT = 0x28C, F_LEFT = 0x290, F_UNDER = 0x294;
constexpr uint32_t F_RIGHT = 0x298, F_CENTER = 0x29C, F_MEDAL = 0x2A0, F_MATERIAL_IDS = 0x2A8;
constexpr uint32_t F_MATERIALS = 0x2B4, F_HAS_SKILL = 0x2E0, F_HAS_BURST = 0x2E1, F_HAS_BONUS = 0x2E2;
constexpr uint32_t F_FLAG_2E3 = 0x2E3;
constexpr uint32_t V_runIn = 0x300;
// UserMedalData: level, skill count, burst level, cost-down / attack / defense bonus, a protection byte; its row
constexpr uint32_t M_LEVEL = 0x8, M_SB = 0x58, M_RARE = 0x470, M_MAX_LEVEL = 0x47C, M_EVOLVED_ID = 0x4DC;
constexpr uint32_t M_EVOLVE_COST = 0x4E0, M_MATERIAL_COUNT = 0x4E4, M_MATERIAL_IDS = 0x4E8;

struct Ctx {
    Frame &f;
    uint32_t read(uint32_t sb, uint32_t off, uint32_t len) {
        const uint32_t dst = f.alloc((len + 3) & ~3u);
        rw::call(g::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + off, len, off});
        return dst;
    }
    int32_t read32(uint32_t sb, uint32_t off) { return int32_t(ld32(read(sb, off, 4))); }
    int32_t medal32(uint32_t medal, uint32_t off) { return read32(ld32(medal + M_SB), off); }
};
inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void shadow(uint32_t text) { call(g::SceneUtil_setShadow, {text}); }
inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// MedalDataManager::getMedalData(out, mgr, user medal id, medal id, UserMedal::Medal row) - a zero row when none
void medalData(uint32_t out, uint32_t mgr, uint32_t lo, uint32_t hi, uint32_t medalId, const uint32_t *row) {
    uint32_t w[18] = {};
    if (row) std::memcpy(w, row, sizeof w);
    call(g::MedalDataManager_getMedalData,
         {out, mgr, lo, hi, medalId, 0, w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11],
          w[12], w[13], w[14], w[15], w[16], w[17]});
}
// a shared_ptr copy at `dst`
void spCopy(uint32_t dst, uint32_t src) {
    st32(dst, ld32(src));
    st32(dst + 4, ld32(src + 4));
    addRef(ld32(dst + 4));
}
// the medal's large image into a widget
void medalImage(Frame &f, uint32_t image, uint32_t medalRef) {
    const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
    const uint32_t copy = f.alloc(8);
    spCopy(copy, medalRef);
    const uint32_t path = call(g::MedalDataManager_getMedalLargeImagePath, {mdm, copy});
    Frame g2;
    const uint32_t s = newStr(g2, path, host_strlen(path));
    call(g::ImageView_loadTexture, {image, s, 0});
    strFree(s);
    release(ld32(copy + 4));
}
void rarePanel(uint32_t panel, int32_t rare, int32_t maxRare) {
    call(g::SceneUtil_updateMedalRarePanel, {panel, uint32_t(rare), 1, fbits(1.0f), 0, uint32_t(maxRare), 0});
}
void moneyLabel(uint32_t label, int32_t value) {
    Frame f;
    const uint32_t s = f.alloc(12);
    call(g::SceneUtil_getMoneyStr, {s, 0, uint32_t(value), uint32_t(value >> 31)});
    call(fn::Text_setString, {label, s});
    strFree(s);
}
// libc++ vector<shared_ptr<T>>::erase(begin()): move the rest down one, destroy the last
void popFront(uint32_t vec) {
    const uint32_t begin = ld32(vec), end = ld32(vec + 4);
    for (uint32_t d = begin, s = begin + 8; s != end; d = s, s += 8) {
        const uint32_t obj = ld32(s), ctl = ld32(s + 4);
        st32(s, 0);
        st32(s + 4, 0);
        st32(d, obj);
        const uint32_t old = ld32(d + 4);
        st32(d + 4, ctl);
        release(old);
    }
    const uint32_t newEnd = end - 8;
    for (uint32_t e = ld32(vec + 4); e != newEnd;) {
        e -= 8;
        st32(vec + 4, e);
        release(ld32(e + 4));
        e = ld32(vec + 4);
    }
}
}  // namespace

struct EvolveInit {
    static uint32_t init(uint32_t self);
};

uint32_t EvolveInit::init(uint32_t self) {
    if (call(g::Layer_init, {self}) != 1) return 0;
    Frame f;
    Ctx c{f};

    // ---------------------------------------------------------------- callbacks
    for (auto [field, vt] : {std::pair<uint32_t, uint32_t>{F_FN210, VT_SceneFn210}, {F_FN228, VT_SceneFn228}}) {
        Frame g2;
        const uint32_t fo = newFunction(g2, vt, {self});
        call(g::function_vv_assign, {self + field, fo});
        functionDestroy(fo);
    }
    st8(self + F_FLAG_2E3, 1);
    {
        const uint32_t sm = sceneManager();
        Frame g2;
        const uint32_t fo = newFunction(g2, VT_BackKey, {self});
        call(g::SceneManager_setOnBackKeyCallback, {sm, fo});
        functionDestroy(fo);
    }
    call(g::SceneManager_showHeader, {sceneManager()});
    {
        const uint32_t sm = sceneManager();
        Frame g2;
        const uint32_t s = newStr(g2, S_Bg, 0x1A);
        call(g::SceneManager_changeBackground, {sm, 0, s});
        strFree(s);
    }
    {
        const uint32_t sm = sceneManager();
        Frame g2;
        const uint32_t td = textData(g2, 100620004);
        const uint32_t text = textDataStr(td);
        const uint32_t s = newStr(g2, text, host_strlen(text));
        call(g::SceneManager_changeTickerMessage, {sm, s});
        strFree(s);
        sharedRelease(td);
    }
    {
        const uint32_t sm = sceneManager();
        Frame g2;
        const uint32_t fo = newFunction(g2, VT_Finish, {});
        call(g::SceneManager_setFinishSceneCallback, {sm, fo});
        functionDestroy(fo);
    }

    // ---------------------------------------------------------------- layout
    {
        Frame g2;
        const uint32_t s = newStr(g2, S_Json, 0x2C);
        st32(self + F_ROOT, call(g::SceneUtil_createNodeWithSceneFile, {s}));
        strFree(s);
    }
    vcall(self, node::addChild, {self, ld32(self + F_ROOT)});
    st32(self + F_LEFT, find(ld32(self + F_ROOT), S_LeftUI, 6));
    st32(self + F_UNDER, find(ld32(self + F_ROOT), S_UnderUI, 7));
    st32(self + F_CENTER, find(ld32(self + F_ROOT), S_CenterLeft, 0xD));
    st32(self + F_RIGHT, find(ld32(self + F_ROOT), S_CenterRight, 0xE));
    {
        const uint32_t back = find(ld32(self + F_ROOT), S_ButtonBack, 0xB);
        call(g::SceneUtil_backBtnSetPosY, {back});
        addTouch(back, VT_BackButton, {self});
    }

    // ---------------------------------------------------------------- the medal
    {
        const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
        const uint32_t sm = sceneManager();
        const uint32_t hi = ld32(sm + 0x2A4), lo = ld32(sm + 0x2A0);
        const uint32_t out = f.alloc(8);
        medalData(out, mdm, lo, hi, 0, nullptr);
        const uint32_t obj = ld32(out), ctl = ld32(out + 4);
        st32(out, 0);
        st32(out + 4, 0);
        st32(self + F_MEDAL, obj);
        const uint32_t old = ld32(self + F_MEDAL + 4);
        st32(self + F_MEDAL + 4, ctl);
        release(old);
    }
    const uint32_t medalRef = self + F_MEDAL;
    auto medal = [&] { return ld32(medalRef); };
    const int32_t maxLevel = c.medal32(medal(), M_MAX_LEVEL);
    const int32_t level = int32_t(ld32(medal() + M_LEVEL));
    const int32_t cost = c.medal32(medal(), M_EVOLVE_COST);
    const uint32_t api = call(g::APIManager_getInstance, {});
    const int32_t munny = c.read32(ld32(api + 0x8C), 0);
    int32_t maxRare;
    {
        const uint32_t copy = f.alloc(8);
        spCopy(copy, medalRef);
        maxRare = int32_t(call(g::SceneUtil_getMedalMaxRare, {copy}));
        release(ld32(copy + 4));
    }
    const uint32_t image1 = find(ld32(self + F_CENTER), S_DummyMedal1, 0xC);
    medalImage(f, image1, medalRef);
    {
        const uint32_t stars = find(ld32(self + F_CENTER), S_StarArea1, 0xA);
        rarePanel(stars, c.medal32(medal(), M_RARE), maxRare);
    }
    {
        Frame g2;
        const uint32_t s = newStr(g2, S_Empty, 0);
        const uint32_t button = call(g::ImageButton_create, {s, 0});
        strFree(s);
        call(g::ImageButton_setTargetView, {button, image1, ld32(ld32(G_targetScale))});
        vcall(image1, node::addChild, {image1, button});
        Frame g3;
        const uint32_t fo = newFunction(g3, VT_MedalTap, {self});
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }

    // ---------------------------------------------------------------- the evolved medal: its row at level 1
    uint32_t row[18] = {};
    {
        const uint32_t rows = ld32(call(g::APIManager_getInstance, {}) + 0xD4);
        const uint32_t key = f.alloc(8);
        st32(key, ld32(medal()));
        st32(key + 4, ld32(medal() + 4));
        const uint32_t node = call(g::HashULLInt_find, {rows + 0x24, key});
        if (node && int32_t(ld32(node + 0x10)) >= 0) {
            const uint32_t index = ld32(node + 0x10);
            const uint32_t r = c.read(rows, 72 * index, 0x48);
            for (uint32_t k = 0; k < 18; ++k) row[k] = ld32(r + 4 * k);
        }
    }
    const uint32_t evolved = f.alloc(8);
    {
        const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
        const int32_t evolvedId = c.medal32(medal(), M_EVOLVED_ID);
        row[3] = 1;
        medalData(evolved, mdm, 0, 0, uint32_t(evolvedId), row);
    }
    {
        const uint32_t image2 = find(ld32(self + F_CENTER), S_DummyMedal2, 0xC);
        medalImage(f, image2, evolved);
        const uint32_t stars = find(ld32(self + F_CENTER), S_StarArea2, 0xA);
        rarePanel(stars, c.medal32(ld32(evolved), M_RARE), maxRare);
    }
    {
        const uint32_t right = ld32(self + F_RIGHT);
        const uint32_t a = f.alloc(8), b = f.alloc(8);
        spCopy(a, evolved);
        spCopy(b, medalRef);
        call(g::initEvolutLayer, {right, a, b, 1});
        release(ld32(b + 4));
        release(ld32(a + 4));
    }

    // ---------------------------------------------------------------- cost and munny
    uint32_t costLabel;
    {
        const uint32_t base = find(ld32(self + F_UNDER), S_BaseMoney1, 0xB);
        const uint32_t title = find(base, S_TxtMoney, 9);
        shadow(title);
        setTextId(title, 100300002);
        costLabel = find(base, S_TxtMoneyLabel, 0xF);
        shadow(costLabel);
        moneyLabel(costLabel, c.medal32(medal(), M_EVOLVE_COST));
    }
    {
        const uint32_t base = find(ld32(self + F_UNDER), S_BaseMoney2, 0xB);
        const uint32_t title = find(base, S_TxtMoney, 9);
        shadow(title);
        setTextId(title, 100300003);
        const uint32_t label = find(base, S_TxtMoneyLabel, 0xF);
        shadow(label);
        const uint32_t api2 = call(g::APIManager_getInstance, {});
        moneyLabel(label, c.read32(ld32(api2 + 0x8C), 0));
    }

    // ---------------------------------------------------------------- materials: group the ids
    const uint32_t types = f.alloc(12), groups = f.alloc(12);   // vector<int>, vector<vector<shared_ptr>>
    for (uint32_t k = 0; k < 12; k += 4) {
        st32(types + k, 0);
        st32(groups + k, 0);
    }
    const int32_t count = c.medal32(medal(), M_MATERIAL_COUNT);
    const uint32_t owned = call(g::operator_new_array, {(count < 0 || count > 0x3FFFFFFF) ? 0xFFFFFFFFu
                                                                                          : uint32_t(count) * 4});
    {
        int32_t prev = -1;
        const uint32_t tmpId = f.alloc(4), tmpVec = f.alloc(12);
        for (int32_t i = 0; i < count; ++i) {
            const int32_t id = int32_t(ld32(c.read(ld32(medal() + M_SB), M_MATERIAL_IDS, 0x14) + 4 * i));
            st32(tmpId, uint32_t(id));
            st32(owned + 4 * i, 0);
            if (prev == id) continue;
            const uint32_t end = ld32(types + 4);
            if (end == ld32(types + 8)) {
                call(g::VecInt_pushSlow, {types, tmpId});
            } else {
                if (end) st32(end, uint32_t(id));
                st32(types + 4, end + 4);
            }
            st32(tmpVec, 0);
            st32(tmpVec + 4, 0);
            st32(tmpVec + 8, 0);
            const uint32_t gend = ld32(groups + 4);
            if (gend >= ld32(groups + 8)) {
                call(g::VecVecSp_pushSlow, {groups, tmpVec});
            } else {
                if (gend) {
                    st32(gend, 0);
                    st32(gend + 4, 0);
                    st32(gend + 8, 0);
                    st32(gend, ld32(tmpVec));
                    st32(gend + 4, ld32(tmpVec + 4));
                    st32(gend + 8, ld32(tmpVec + 8));
                    st32(tmpVec, 0);
                    st32(tmpVec + 4, 0);
                    st32(tmpVec + 8, 0);
                }
                st32(groups + 4, ld32(groups + 4) + 12);
            }
            call(g::VecSp_destroy, {tmpVec});
            prev = id;
        }
    }
    // the protected medal (a user data id) is never offered
    uint32_t keepLo, keepHi;
    {
        const uint32_t api2 = call(g::APIManager_getInstance, {});
        const uint32_t v = c.read(ld32(api2 + 0x114), 0x58, 8);
        keepLo = ld32(v);
        keepHi = ld32(v + 4);
    }
    // map<int, int> owned count per medal id, unordered_set<ull> of the taken medals
    const uint32_t counts = f.alloc(12), taken = f.alloc(20);
    st32(counts + 4, 0);
    st32(counts + 8, 0);
    st32(counts, counts + 4);
    for (uint32_t k = 0; k < 16; k += 4) st32(taken + k, 0);
    st32(taken + 16, fbits(1.0f));
    {
        const uint32_t rows = ld32(call(g::APIManager_getInstance, {}) + 0xD4);
        const uint32_t m = f.alloc(8), key = f.alloc(4), favCopy = f.alloc(8);
        for (uint32_t j = 0; j < ld32(rows + 0x20); ++j) {
            const uint32_t r = c.read(rows, 72 * j, 0x48);
            const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
            medalData(m, mdm, ld32(r), ld32(r + 4), 0, nullptr);
            const uint32_t obj = ld32(m);
            if (ld32(obj) != ld32(medal()) || ld32(obj + 4) != ld32(medal() + 4)) {
                const int32_t id = c.read32(ld32(obj + M_SB), 0);
                st32(key, uint32_t(id));
                // counts[id] += 1 (a lower_bound walk first, then operator[])
                uint32_t node = ld32(counts + 4), hit = counts + 4;
                while (node) {
                    if (int32_t(ld32(node + 0x10)) < id) {
                        node = ld32(node + 4);
                    } else {
                        hit = node;
                        node = ld32(node);
                    }
                }
                if (hit != counts + 4 && !(id < int32_t(ld32(hit + 0x10)))) {
                    const uint32_t n = ld32(call(g::MapInt_index, {counts, key}));
                    st32(call(g::MapInt_index, {counts, key}), n + 1);
                } else {
                    st32(call(g::MapInt_index, {counts, key}), 1);
                }
                for (uint32_t idx = 0; idx < (ld32(types + 4) - ld32(types)) >> 2; ++idx) {
                    if (int32_t(ld32(ld32(types) + 4 * idx)) != id) continue;
                    const uint32_t mdm2 = call(g::MedalDataManager_getInstance, {});
                    spCopy(favCopy, m);
                    const uint32_t fav = call(g::MedalDataManager_isFavoriteMedal, {mdm2, favCopy});
                    release(ld32(favCopy + 4));
                    if (fav) continue;
                    const uint32_t mo = ld32(m);
                    if ((ld32(mo) == keepLo && ld32(mo + 4) == keepHi) || ld8(mo + 0x26)) continue;
                    if ((ld32(groups + 4) - ld32(groups)) / 12 <= idx) call(g::throw_out_of_range, {groups});
                    const uint32_t vec = ld32(groups) + 12 * idx;
                    const uint32_t end = ld32(vec + 4);
                    if (end == ld32(vec + 8)) {
                        call(g::VecSp_pushSlow, {vec, m});
                    } else {
                        if (end) {
                            st32(end, mo);
                            st32(end + 4, ld32(m + 4));
                            addRef(ld32(m + 4));
                        }
                        st32(vec + 4, ld32(vec + 4) + 8);
                    }
                    break;
                }
            }
            release(ld32(m + 4));
        }
    }
    for (uint32_t idx = 0; idx < (ld32(groups + 4) - ld32(groups)) / 12; ++idx) {
        const uint32_t vec = ld32(groups) + 12 * idx;
        call(g::sortMaterials, {ld32(vec), ld32(vec + 4)});
    }
    // take the first medal of each material's group
    {
        const uint32_t idTmp = f.alloc(8), insertOut = f.alloc(8);
        for (int32_t i = 0; i < count; ++i) {
            const int32_t id = int32_t(ld32(c.read(ld32(medal() + M_SB), M_MATERIAL_IDS, 0x14) + 4 * i));
            const uint32_t ntypes = (ld32(types + 4) - ld32(types)) >> 2;
            uint32_t vec = 0;
            for (uint32_t idx = 0; idx < ntypes; ++idx) {
                if (int32_t(ld32(ld32(types) + 4 * idx)) != id) continue;
                if ((ld32(groups + 4) - ld32(groups)) / 12 <= idx) call(g::throw_out_of_range, {groups});
                const uint32_t v = ld32(groups) + 12 * idx;
                if (ld32(v + 4) != ld32(v)) {
                    vec = v;
                    break;
                }
            }
            if (!vec) continue;
            const uint32_t first = ld32(vec);
            const uint32_t mo = ld32(first);
            if (int32_t(ld32(mo + 0x48)) >= 1) st8(self + F_HAS_SKILL, 1);
            if (int32_t(ld32(mo + 0x50)) >= 1) st8(self + F_HAS_BURST, 1);
            if (int32_t(ld32(mo + 0x28) + ld32(mo + 0x2C) + ld32(mo + 0x30)) >= 1) st8(self + F_HAS_BONUS, 1);
            call(g::UllSet_insert, {insertOut, taken, mo});
            st32(idTmp, uint32_t(id));
            st32(idTmp + 4, uint32_t(id >> 31));
            {
                const uint32_t ids = self + F_MATERIAL_IDS;
                const uint32_t end = ld32(ids + 4);
                if (end >= ld32(ids + 8)) {
                    call(g::VecUll_pushSlowMove, {ids, idTmp});
                } else {
                    if (end) {
                        st32(end, uint32_t(id));
                        st32(end + 4, uint32_t(id >> 31));
                    }
                    st32(ids + 4, ld32(ids + 4) + 8);
                }
            }
            {
                const uint32_t mats = self + F_MATERIALS;
                const uint32_t begin = ld32(mats), end = ld32(mats + 4);
                if (uint32_t(i) < (end - begin) >> 3) {
                    call(g::VecUll_insert, {mats, begin + 8 * uint32_t(i), ld32(first)});
                } else {
                    const uint32_t src = ld32(first);
                    if (end == ld32(mats + 8)) {
                        call(g::VecUll_pushSlowCopy, {mats, src});
                    } else {
                        if (end) {
                            st32(end, ld32(src));
                            st32(end + 4, ld32(src + 4));
                        }
                        st32(mats + 4, ld32(mats + 4) + 8);
                    }
                }
            }
            st32(owned + 4 * i, 1);
            popFront(vec);
        }
    }

    // ---------------------------------------------------------------- material cells
    const uint32_t area = find(ld32(self + F_UNDER), S_Area, 4);
    const uint32_t unusedMap = f.alloc(12);
    st32(unusedMap + 4, 0);
    st32(unusedMap + 8, 0);
    st32(unusedMap, unusedMap + 4);
    bool allOwned = true;
    {
        uint32_t taking = 0;
        int32_t x = 0;
        for (int32_t i = 0; i < count; ++i) {
            const int32_t id = int32_t(ld32(c.read(ld32(medal() + M_SB), M_MATERIAL_IDS, 0x14) + 4 * i));
            const uint32_t keyA = c.read(ld32(medal() + M_SB), M_MATERIAL_IDS, 0x14) + 4 * i;
            int32_t have = 0;
            if (call(g::MapInt_find, {counts, keyA}) != counts + 4) {
                const uint32_t keyB = c.read(ld32(medal() + M_SB), M_MATERIAL_IDS, 0x14) + 4 * i;
                have = int32_t(ld32(call(g::MapInt_at, {counts, keyB})));
            }
            const uint32_t isOwned = ld32(owned + 4 * i);
            const uint32_t out = f.alloc(8);
            if (isOwned) {
                const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
                const uint32_t mats = self + F_MATERIALS;
                if (((ld32(mats + 4) - ld32(mats)) >> 3) <= taking) call(g::throw_out_of_range, {mats});
                const uint32_t idp = ld32(mats) + 8 * taking;
                medalData(out, mdm, ld32(idp), ld32(idp + 4), 0, nullptr);
                ++taking;
            } else {
                const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
                medalData(out, mdm, 0, 0, uint32_t(id), nullptr);
                allOwned = false;
            }
            const uint32_t mo = ld32(out), mctl = ld32(out + 4);
            st32(out, 0);
            st32(out + 4, 0);
            const uint32_t arg = f.alloc(8);
            st32(arg, mo);
            st32(arg + 4, mctl);
            addRef(mctl);
            const uint32_t cell =
                call(g::createMaterialMedal, {self, arg, uint32_t(have), isOwned ? 1u : 0u});
            release(mctl);
            vcall(cell, node::setPositionX, {cell, call(g::floatsisf, {uint32_t(x)})});
            vcall(area, node::addChild, {area, cell});
            const uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + 0x84);
            const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
            call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
            const uint32_t w = ld32(vcall(cell, node::getContentSize, {cell}));
            const uint32_t h = ld32(vcall(cell, node::getContentSize, {cell}) + 4);
            const uint32_t rect = f.alloc(16);
            call(g::Rect_ctor, {rect, 0, 0, w, h});
            // onTouchBegan {cell, rect}
            const uint32_t cap1 = f.alloc(20), cap2 = f.alloc(20);
            st32(cap1, cell);
            call(g::Rect_copy, {cap1 + 4, rect});
            st32(cap2, cell);
            call(g::Rect_copy, {cap2 + 4, cap1 + 4});
            st32(cap2, cell);
            {
                const uint32_t fo = newEmptyFunction(f);
                const uint32_t heap = call(fn::operator_new, {0x18});
                if (heap) {
                    st32(heap, VT_CellBegan);
                    st32(heap + 4, ld32(cap2));
                    call(g::Rect_copy, {heap + 8, cap2 + 4});
                    st32(heap + 4, ld32(cap2));
                }
                st32(fo + 16, heap);
                call(g::function_touchBegan_swap, {fo, listener + 0x50});
                functionDestroy(fo);
            }
            call(g::Vec2_dtor, {cap2 + 4});
            call(g::Vec2_dtor, {cap1 + 4});
            // onTouchEnded {the material medal}
            addRef(mctl);
            {
                Frame g2;
                const uint32_t fo = newFunction(g2, VT_CellEnded, {mo, mctl});
                call(g::function_touchEnded_swap, {fo, listener + 0x80});
                functionDestroy(fo);
            }
            call(g::EventDispatcher_addSceneGraph, {dispatcher, listener, cell});
            call(g::Vec2_dtor, {rect});
            release(mctl);
            x += 110;
        }
    }
    call(g::operator_delete_array, {owned});

    // ---------------------------------------------------------------- Evolve, Sell, hint
    {
        const uint32_t image = find(ld32(self + F_UNDER), S_ButtonLimitCut, 0xF);
        const uint32_t text = find(image, S_TxtLimitCut, 0xC);
        shadow(text);
        setTextId(text, 101400001);
        Frame g2;
        const uint32_t s = newStr(g2, S_But01, 5);
        const uint32_t button = call(g::ImageButton_create, {s, 0});
        strFree(s);
        call(g::ImageButton_setTargetImageView, {button, image});
        vcall(image, node::addChild, {image, button});
        const bool atMax = maxLevel <= level, affordable = cost <= munny;
        // the lambda {at max level, every material, enough munny, the taken medal ids, this x3}
        const uint32_t lam = g2.alloc(36);
        st8(lam, atMax ? 1 : 0);
        st8(lam + 1, allOwned ? 1 : 0);
        st8(lam + 2, affordable ? 1 : 0);
        call(g::UllSet_copy, {lam + 4, taken});
        st32(lam + 0x18, self);
        st32(lam + 0x1C, self);
        st32(lam + 0x20, self);
        const uint32_t fo = newEmptyFunction(g2);
        const uint32_t heap = call(fn::operator_new, {0x28});
        if (heap) {
            st32(heap, VT_Evolve);
            st8(heap + 4, ld8(lam));
            st8(heap + 6, ld8(lam + 2));
            st8(heap + 5, ld8(lam + 1));
            call(g::UllSet_move, {heap + 8, lam + 4});
            st32(heap + 0x1C, self);
            st32(heap + 0x20, self);
            st32(heap + 0x24, self);
        }
        st32(fo + 16, heap);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        call(g::UllSet_destroy, {lam + 4});
        call(g::SceneUtil_setButtonEnabled, {button, text, (atMax && affordable && allOwned) ? 1u : 0u, 0});
        if (cost > munny) call(g::SceneUtil_setWarningColor, {costLabel});
        else call(g::SceneUtil_setStatusChangeColor, {costLabel});
    }
    {
        const uint32_t image = find(ld32(self + F_UNDER), S_ButtonSell1, 0xC);
        const uint32_t text = find(image, S_TxtSell1, 9);
        shadow(text);
        setTextId(text, 101100004);
        Frame g2;
        const uint32_t s = newStr(g2, S_But06, 5);
        const uint32_t button = call(g::ImageButton_create, {s, 0});
        strFree(s);
        call(g::ImageButton_setTargetImageView, {button, image});
        vcall(image, node::addChild, {image, button});
        const uint32_t fo = newFunction(g2, VT_Sell, {self});
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t text = find(ld32(self + F_UNDER), S_TxtWording, 0xB);
        shadow(text);
        setTextId(text, 101400007);
    }
    vcall(self, V_runIn, {self});

    call(g::MapInt_destroy, {unusedMap, ld32(unusedMap + 4)});
    call(g::UllSet_destroy, {taken});
    call(g::MapInt_destroy, {counts, ld32(counts + 4)});
    call(g::VecInt_destroy, {types});
    call(g::VecVecSp_destroy, {groups});
    release(ld32(evolved + 4));
    return 1;
}

RW_FUNCTION(0x6AA434, "SceneEvolveMedal::init", EvolveInit::init);
