#include "SceneRaidList.hpp"

#include "FuncThunks.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layer_ctor = 0x88A7D1;
constexpr uint32_t Layer_init = 0x88A8A1;
constexpr uint32_t Layer_dtor = 0x88A88D;
constexpr uint32_t Scene_create = 0x8A5F37;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t VoidFunction_assign = 0x4EB9CD;       // std::function<void()>::operator=
constexpr uint32_t VoidFunction_copy = 0x4683CD;         // std::function<void()>(const function&)
constexpr uint32_t VoidFunction_move = 0x47F905;         // std::function<void()>(function&&)
constexpr uint32_t VoidFunction_call = 0x470E69;
constexpr uint32_t BoolFunction_assign = 0x490109;       // std::function<void(bool)>::operator=
constexpr uint32_t IntFunction_swap = 0x5428E1;          // std::function<void(int)>::swap
constexpr uint32_t BoolIntFunction_swap = 0x6C7529;      // std::function<bool(int)>::swap
constexpr uint32_t IntVector_push_back_slow = 0x4D6CC1;  // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t IntVector_base_dtor = 0x478961;
constexpr uint32_t RaidVector_copy = 0x6C6CC5;           // vector<hole::network::api::RaidBase>(const vector&)
constexpr uint32_t RaidVector_base_dtor = 0x4BA87D;
constexpr uint32_t Vector_throw_out_of_range = 0x47CCE1;
constexpr uint32_t BaseScrollView_setItemIDs = 0x478929; // BaseScrollView<int>::setItemIDs(const vector<int>&)
constexpr uint32_t RaidBossScrollView_create = 0x6C6C71;
constexpr uint32_t SM_getInstance = 0x60C26D;
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B;  // (sm, std::function<void()> by value)
constexpr uint32_t SM_setBackButtonType = 0x613E27;
constexpr uint32_t SM_showHeader = 0x6131ED;
constexpr uint32_t SM_setHeaderRaidAlertsBtn = 0x6132D5;
constexpr uint32_t SM_changeBackground = 0x6128ED;      // (sm, BackGroundType, const string&)
constexpr uint32_t SM_changeTickerMessage = 0x610B39;
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_isOpenedPopup = 0x621519;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_getPopupLayer = 0x613E69;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;      // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;   // (node, float, float, function by value)
constexpr uint32_t SceneUtil_createLeftOutAnimation = 0x45B735;
constexpr uint32_t SceneUtil_getCampaignPoint = 0x4741BD;  // (int, int, int, const vector<int>&, int, bool)
constexpr uint32_t SceneUtil_isOverMedalSlot = 0x46A085;
constexpr uint32_t SceneUtil_isMaxApOk = 0x469FF9;
constexpr uint32_t SceneUtil_isApOk = 0x46A061;
constexpr uint32_t SceneUtil_openMedalOverDialog = 0x460251;   // (bool, BackScene, function<void(bool)>)
constexpr uint32_t SceneUtil_openChargeApDialog = 0x45CC01;    // (function<void(bool,bool)>)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;     // (const string&, function<void(bool)>, const char*)
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;   // (function<void(bool)>)
constexpr uint32_t StageUtil_setupRaidGlobalVariables = 0x764269;
constexpr uint32_t StartDeckEditDialog_create = 0x5735DF;
constexpr uint32_t StartDeckEditDialog_open = 0x573851;  // (dialog, parent, StageType, int, function<void(bool)>)
constexpr uint32_t BuyMagicStoneDialog_open = 0x588CAD;  // (const function<void()>&, bool)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t createSceneLayer_AdventureSelect = 0x579EB1;  // createSceneLayer<SceneAdventureSelect>()
constexpr uint32_t VT_FuncPtrLayer = 0xF7E130;           // __func<Layer*(*)()> + 8

constexpr uint32_t STR_bg = 0xE03AF2;                    // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t STR_sceneJson = 0xE058AC;             // "cocostudio/publish/AdvRaidListScene_ver130.json"
constexpr uint32_t STR_LeftUI = 0xDF85F0, STR_Button_Back = 0xE03DBA, STR_UnderUI = 0xDFE20F;
constexpr uint32_t STR_Win_Bottom = 0xE058DC, STR_ScrollArea = 0xE058E7, STR_Txt_Title = 0xDFF142;
constexpr int32_t TEXT_TICKER = 100620020;
constexpr int32_t TEXT_TITLE = 103200033;
constexpr int32_t TEXT_MAX_AP_NG = 102610001;

constexpr uint32_t SM_BACK_KEY_ENABLED = 0x30A;          // SceneManager byte flags
constexpr uint32_t SM_FLAG_308 = 0x308;
constexpr uint32_t SM_FLAG_4CC = 0x4CC;
constexpr uint32_t SM_HEADER_CALLBACK = 0x548;           // std::function<void(bool)>
constexpr uint32_t APIMANAGER_RAIDS = 340;               // vector<RaidBase>
constexpr uint32_t APIMANAGER_CAMPAIGNS = 764;           // vector<int>
constexpr uint32_t RAID_BASE_SIZE = 240;
constexpr uint32_t SCROLL_ON_SELECT = 1592;              // BaseScrollView: std::function<void(int)>
constexpr uint32_t SCROLL_ON_FILTER = 1616;              // BaseScrollView: std::function<bool(int)>
constexpr uint32_t WIDGET_SET_ENABLED = 668;             // ui::Widget::setEnabled / setTouchEnabled slot

// lambdas: vtable (+8), type name, type_info
constexpr uint32_t VT_Enter0 = 0xF67180, TS_Enter0 = 0xE7CC50, TI_Enter0 = 0xF9F4F8;
constexpr uint32_t VT_Exit1 = 0xF67154, TS_Exit1 = 0xE7CBD0, TI_Exit1 = 0xF9F4E4;
constexpr uint32_t VT_Select2 = 0xF670D0, TS_Select2 = 0xE7CA20, TI_Select2 = 0xF9F4A8;
constexpr uint32_t VT_Filter3 = 0xF670A4, TS_Filter3 = 0xE7C9A0, TI_Filter3 = 0xF9F494;
constexpr uint32_t VT_Out4 = 0xF67078, TS_Out4 = 0xE7C900, TI_Out4 = 0xF9F480;
constexpr uint32_t VT_Back5 = 0xF6704C, TS_Back5 = 0xE7C860, TI_Back5 = 0xF9F46C;
constexpr uint32_t VT_ApNg = 0xF670FC, TS_ApNg = 0xE7CAB0, TI_ApNg = 0xF9F4BC;     // $_2::{lambda(bool,bool)}
constexpr uint32_t VT_Deck = 0xF67128, TS_Deck = 0xE7CB40, TI_Deck = 0xF9F4D0;     // $_2::{lambda(bool)}

uint32_t sm() { return call(SM_getInstance, {}); }

// left in / out slide of a node with no callback, eased
void slide(uint32_t anim, uint32_t node, uint32_t done) {
    uint32_t act = call(anim, {node, 0, 0, done});
    call(Node_runAction, {node, call(EaseSineOut_create, {act})});
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
SceneRaidList *SceneRaidList::create() {
    uint32_t p = call(guest::OPERATOR_NEW, {SIZE});
    call(Layer_ctor, {p});
    SceneRaidList *s = rw::host_ptr<SceneRaidList>(p);
    s->vtable = VTABLE;
    s->root = 0;
    s->canGoBack = 0;
    s->leaving = 0;
    if (vcall(p, 0x248, {p}) == 1) {                // init() (read from the static vtable + 0x250)
        call(Ref_autorelease, {p});
        return s;
    }
    vcall(p, 4, {p});
    return nullptr;
}
RW_FUNCTION(0x57A73C, "SceneRaidList::create", SceneRaidList::create);

uint32_t SceneRaidList::createScene() {
    uint32_t scene = call(Scene_create, {});
    uint32_t layer = rw::guest_addr(create());
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x6C6544, "SceneRaidList::createScene", SceneRaidList::createScene);

SceneRaidList *SceneRaidList::destroy(SceneRaidList *self) {
    self->vtable = VTABLE;
    return rw::host_ptr<SceneRaidList>(call(Layer_dtor, {rw::guest_addr(self)}));
}
RW_FUNCTION(0x6C6568, "SceneRaidList::~SceneRaidList", SceneRaidList::destroy);

void SceneRaidList::deletingDestroy(SceneRaidList *self) {
    self->vtable = VTABLE;
    call(guest::OPERATOR_DELETE, {call(Layer_dtor, {rw::guest_addr(self)})});
}
RW_FUNCTION(0x6C6588, "SceneRaidList::~SceneRaidList (deleting)", SceneRaidList::deletingDestroy);

void SceneRaidList::update(SceneRaidList *, float) {}
RW_FUNCTION(0x6C6ED8, "SceneRaidList::update", SceneRaidList::update);

// ------------------------------------------------------------------------------------------- init
uint32_t SceneRaidList::init(SceneRaidList *self) {
    uint32_t a = rw::guest_addr(self);
    if (call(Layer_init, {a}) != 1) return 0;
    Frame f;
    {
        uint32_t fn = newFunction(f, VT_Enter0, {a});
        call(VoidFunction_assign, {a + 528, fn});
        functionDestroy(fn);
        fn = newFunction(f, VT_Exit1, {a});
        call(VoidFunction_assign, {a + 552, fn});
        functionDestroy(fn);
    }
    self->canGoBack = 0;
    {
        uint32_t m = sm();
        uint32_t fn = newEmptyFunction(f);
        call(SM_setOnBackKeyCallback, {m, fn});
        functionDestroy(fn);
    }
    rw::save<uint8_t>(sm() + SM_BACK_KEY_ENABLED, 0);
    call(SM_setBackButtonType, {sm(), 0});
    call(SM_showHeader, {sm()});
    call(SM_setHeaderRaidAlertsBtn, {sm(), 0});
    {
        uint32_t m = sm();
        uint32_t s = newStr(f, STR_bg, 0x1A);
        call(SM_changeBackground, {m, 0, s});
        strFree(s);
    }
    {
        uint32_t m = sm();
        uint32_t td = textData(f, TEXT_TICKER);
        uint32_t s = newStr(f, textDataStr(td));
        call(SM_changeTickerMessage, {m, s});
        strFree(s);
        sharedRelease(td);
    }
    {
        uint32_t s = newStr(f, STR_sceneJson, 0x2F);
        self->root = SceneUtil::createNodeWithSceneFile(s);
        strFree(s);
    }
    vcall(a, node::addChild, {a, self->root});
    uint32_t left = findNode(self->root, STR_LeftUI, 6);
    uint32_t back = findNode(left, STR_Button_Back, 11);
    SceneUtil::backBtnSetPosY(back);
    vcall(back, node::setVisible, {back, 0});
    uint32_t under = findNode(self->root, STR_UnderUI, 7);
    uint32_t winBottom = findNode(under, STR_Win_Bottom, 10);
    if (winBottom) vcall(winBottom, WIDGET_SET_ENABLED, {winBottom, 1});
    uint32_t area = findNode(under, STR_ScrollArea, 0x1A);
    vcall(under, WIDGET_SET_ENABLED, {under, 0});
    vcall(area, WIDGET_SET_ENABLED, {area, 0});
    uint32_t sv = call(RaidBossScrollView_create, {});
    self->scrollView = sv;
    vcall(sv, node::setContentSize, {sv, vcall(area, node::getContentSize, {area})});
    titleLabel(under, STR_Txt_Title, 9, TEXT_TITLE);

    // item ids 0 .. raids - 1
    uint32_t ids = f.alloc(12);
    st32(ids, 0); st32(ids + 4, 0); st32(ids + 8, 0);
    uint32_t raids = f.alloc(12);
    call(RaidVector_copy, {raids, call(APIManager_getInstance, {}) + APIMANAGER_RAIDS});
    uint32_t value = f.alloc(4);
    st32(value, 0);
    if (int32_t(ld32(raids + 4) - ld32(raids)) >= 1) {
        for (int32_t k = 1;; k++) {
            uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(IntVector_push_back_slow, {ids, value});
            } else {
                uint32_t slot = 0;
                if (end) { st32(end, uint32_t(k - 1)); slot = ld32(ids + 4); }
                st32(ids + 4, slot + 4);
            }
            st32(value, uint32_t(k));
            int32_t count = int32_t(uint32_t(int32_t(ld32(raids + 4) - ld32(raids)) >> 4) * 0xEEEEEEEFu);
            if (!(k < count)) break;
        }
    }
    call(BaseScrollView_setItemIDs, {self->scrollView, ids});
    {
        // onSelect = [this, raids](int index) {...}
        uint32_t svAddr = self->scrollView;
        uint32_t copy = f.alloc(12);
        call(RaidVector_copy, {copy, raids});
        uint32_t moved = f.alloc(12);
        st32(moved, ld32(copy)); st32(moved + 4, ld32(copy + 4)); st32(moved + 8, ld32(copy + 8));
        st32(copy, 0); st32(copy + 4, 0); st32(copy + 8, 0);
        uint32_t fn = f.alloc(20);
        st32(fn + 16, 0);
        uint32_t heap = call(guest::OPERATOR_NEW, {20});
        if (heap) {
            rw::save<uint32_t>(heap, VT_Select2);
            rw::save<uint32_t>(heap + 8, ld32(moved));
            rw::save<uint32_t>(heap + 12, ld32(moved + 4));
            rw::save<uint32_t>(heap + 16, ld32(moved + 8));
            st32(moved, 0); st32(moved + 4, 0); st32(moved + 8, 0);
            rw::save<uint32_t>(heap + 4, a);
        }
        st32(fn + 16, heap);
        call(IntFunction_swap, {fn, svAddr + SCROLL_ON_SELECT});
        functionDestroy(fn);
        call(RaidVector_base_dtor, {moved});
        call(RaidVector_base_dtor, {copy});
    }
    {
        uint32_t fn = newFunction(f, VT_Filter3, {});
        call(BoolIntFunction_swap, {fn, self->scrollView + SCROLL_ON_FILTER});
        functionDestroy(fn);
    }
    vcall(area, node::addChild, {area, self->scrollView});
    runInAnimation(self);
    call(RaidVector_base_dtor, {raids});
    call(IntVector_base_dtor, {ids});
    return 1;
}
RW_FUNCTION(0x6C65AC, "SceneRaidList::init", SceneRaidList::init);

// ------------------------------------------------------------------------------------------- animations
void SceneRaidList::runInAnimation(SceneRaidList *self) {
    Frame f;
    uint32_t under = findNode(self->root, STR_UnderUI, 7);
    {
        uint32_t fn = newEmptyFunction(f);
        slide(SceneUtil_createLeftInAnimation, under, fn);
        functionDestroy(fn);
    }
    uint32_t children = vcall(under, node::getChildren, {under});
    uint32_t first = ld32(ld32(children));
    vcall(first, WIDGET_SET_ENABLED, {first, 0});
    uint32_t left = findNode(self->root, STR_LeftUI, 6);
    {
        uint32_t fn = newEmptyFunction(f);
        slide(SceneUtil_createLeftInAnimation, left, fn);
        functionDestroy(fn);
    }
}
RW_FUNCTION(0x6C6D18, "SceneRaidList::runInAnimation", SceneRaidList::runInAnimation);

void SceneRaidList::runOutAnimation(SceneRaidList *self, uint32_t done) {
    call(SM_setCanTouchHeader, {sm(), 0});
    self->leaving = 1;
    Frame f;
    uint32_t under = findNode(self->root, STR_UnderUI, 7);
    {
        uint32_t copy = f.alloc(20);
        call(VoidFunction_copy, {copy, done});
        uint32_t fn = f.alloc(20);                    // [done] { done(); } on the heap
        st32(fn + 16, 0);
        uint32_t heap = call(guest::OPERATOR_NEW, {0x20});
        if (heap) {
            rw::save<uint32_t>(heap, VT_Out4);
            call(VoidFunction_move, {heap + 8, copy});
        }
        st32(fn + 16, heap);
        slide(SceneUtil_createLeftOutAnimation, under, fn);
        functionDestroy(fn);
        functionDestroy(copy);
    }
    uint32_t left = findNode(self->root, STR_LeftUI, 6);
    {
        uint32_t fn = newEmptyFunction(f);
        slide(SceneUtil_createLeftOutAnimation, left, fn);
        functionDestroy(fn);
    }
}
RW_FUNCTION(0x6C6EDC, "SceneRaidList::runOutAnimation", SceneRaidList::runOutAnimation);

// Back: close an open popup, else return to the adventure select scene after the out animation.
void SceneRaidList::backScene(SceneRaidList *self) {
    if (!self->canGoBack) return;
    uint32_t opened = call(SM_isOpenedPopup, {sm()});
    uint32_t m = sm();
    if (opened == 1) {
        call(SM_closeCurrentPopup, {m, 0});
        return;
    }
    rw::save<uint8_t>(m + SM_FLAG_308, 0);
    self->canGoBack = 0;
    Frame f;
    {
        uint32_t m2 = sm();
        uint32_t fn = newEmptyFunction(f);
        rw::save<uint8_t>(m2 + SM_FLAG_4CC, 1);
        call(BoolFunction_assign, {m2 + SM_HEADER_CALLBACK, fn});
        functionDestroy(fn);
    }
    {
        uint32_t fn = newEmptyFunction(f);
        call(SceneAdventureSelect_callAdventureSelectAPI, {fn});
        functionDestroy(fn);
    }
    {
        uint32_t fn = newFunction(f, VT_Back5, {});
        runOutAnimation(self, fn);
        functionDestroy(fn);
    }
}
RW_FUNCTION(0x6C7100, "SceneRaidList::backScene", SceneRaidList::backScene);

// ------------------------------------------------------------------------------------------- lambdas
namespace SceneRaidListLambdas {
using namespace bui::thunk;

// init $_0 / $_1 (Node onEnter / onExit hooks): schedule / unschedule update
void enterCall(uint32_t self) { call(Node_scheduleUpdate, {ld32(self + 4)}); }
void exitCall(uint32_t self) { call(Node_unscheduleUpdate, {ld32(self + 4)}); }

// init $_3 (list filter): no raid is filtered out
uint32_t filterCall(uint32_t, uint32_t) { return 0; }

// backScene $_5 (after the out animation): back to the adventure select scene
void backCall(uint32_t) {
    Frame f;
    uint32_t m = sm();
    uint32_t fn = newFunction(f, VT_FuncPtrLayer, {createSceneLayer_AdventureSelect});
    call(SM_changeSceneLayer, {m, fn, 1, 1, 1});      // (layer factory, true, true, FadeType 1)
    functionDestroy(fn);
    call(SM_setHeaderRaidAlertsBtn, {sm(), 1});
    rw::save<uint8_t>(sm() + SM_BACK_KEY_ENABLED, 1);
}

// init $_2 (a raid tapped; captures scene, copy of the raid list): check medal slots and AP, then open the deck
// dialog for the raid
void selectCall(uint32_t self, uint32_t indexRef) {
    uint32_t scene = ld32(self + 4);
    if (rw::load<uint8_t>(scene + 656)) return;
    uint32_t index = ld32(indexRef);
    uint32_t begin = ld32(self + 8);
    uint32_t count = uint32_t(int32_t(ld32(self + 12) - begin) >> 4) * 0xEEEEEEEFu;
    if (!(count > index)) {
        call(Vector_throw_out_of_range, {self + 8});
        return;
    }
    uint32_t raid = begin + RAID_BASE_SIZE * index;
    uint32_t a7 = ld32(raid + 0x2C), a4 = ld32(raid + 0xC);
    uint32_t api = call(APIManager_getInstance, {});
    uint32_t point = call(SceneUtil_getCampaignPoint, {a4, 4, a7, api + APIMANAGER_CAMPAIGNS, 7, 0});
    Frame f;
    if (call(SceneUtil_isOverMedalSlot, {0}) == 1) {
        uint32_t fn = newEmptyFunction(f);
        call(SceneUtil_openMedalOverDialog, {0, 6, fn});
        functionDestroy(fn);
        return;
    }
    if (call(SceneUtil_isMaxApOk, {point}) == 1) {
        if (call(SceneUtil_isApOk, {point}) == 1) {
            call(StageUtil_setupRaidGlobalVariables, {raid});
            uint32_t dialog = call(StartDeckEditDialog_create, {});
            uint32_t layer = call(SM_getPopupLayer, {sm()});
            uint32_t fn = newFunction(f, VT_Deck, {scene});
            call(StartDeckEditDialog_open, {dialog, layer, 4, index, fn});
            functionDestroy(fn);
        } else {
            uint32_t fn = newFunction(f, VT_ApNg, {});
            call(SceneUtil_openChargeApDialog, {fn});
            functionDestroy(fn);
        }
        return;
    }
    uint32_t td = textData(f, TEXT_MAX_AP_NG);
    uint32_t s = newStr(f, textDataStr(td));
    uint32_t fn = newEmptyFunction(f);
    call(SceneUtil_openOkDialog4Line, {s, fn, 0});
    functionDestroy(fn);
    strFree(s);
    sharedRelease(td);
}

// $_2::{lambda(bool,bool)} (charge AP dialog closed): open the jewel shop when asked
void apChargeCall(uint32_t, uint32_t, uint32_t buyRef) {
    if (!rw::load<uint8_t>(buyRef)) return;
    Frame f;
    uint32_t fn = newEmptyFunction(f);
    call(BuyMagicStoneDialog_open, {fn, 0});
    functionDestroy(fn);
}

// $_2::{lambda(bool)} (deck dialog closed): leave the scene when the raid starts
void deckCall(uint32_t self, uint32_t startRef) {
    if (!rw::load<uint8_t>(startRef)) return;
    Frame f;
    uint32_t fn = newEmptyFunction(f);
    SceneRaidList::runOutAnimation(rw::host_ptr<SceneRaidList>(ld32(self + 4)), fn);
    functionDestroy(fn);
}

// $_2 holds a vector<RaidBase> at +8
uint32_t select2Dtor(uint32_t self) {
    rw::save<uint32_t>(self, VT_Select2);
    call(RaidVector_base_dtor, {self + 8});
    return self;
}
void select2DeletingDtor(uint32_t self) {
    rw::save<uint32_t>(self, VT_Select2);
    call(RaidVector_base_dtor, {self + 8});
    deleteSelf(self);
}
uint32_t select2Clone(uint32_t self) {
    uint32_t p = call(guest::OPERATOR_NEW, {20});
    if (p) {
        rw::save<uint32_t>(p, VT_Select2);
        rw::save<uint32_t>(p + 4, ld32(self + 4));
        call(RaidVector_copy, {p + 8, self + 8});
        rw::save<uint32_t>(p + 4, ld32(self + 4));
    }
    return p;
}
void select2CloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return;
    rw::save<uint32_t>(dst, VT_Select2);
    rw::save<uint32_t>(dst + 4, ld32(self + 4));
    call(RaidVector_copy, {dst + 8, self + 8});
    rw::save<uint32_t>(dst + 4, ld32(self + 4));
}
void select2Destroy(uint32_t self) { call(RaidVector_base_dtor, {self + 8}); }
void select2DestroyDealloc(uint32_t self) {
    call(RaidVector_base_dtor, {self + 8});
    deleteSelf(self);
}

// runOutAnimation $_4 holds the caller's std::function<void()> at +8
uint32_t out4Dtor(uint32_t self) {
    rw::save<uint32_t>(self, VT_Out4);
    functionDestroy(self + 8);
    return self;
}
void out4DeletingDtor(uint32_t self) {
    rw::save<uint32_t>(self, VT_Out4);
    functionDestroy(self + 8);
    deleteSelf(self);
}
uint32_t out4Clone(uint32_t self) {
    uint32_t p = call(guest::OPERATOR_NEW, {0x20});
    if (p) {
        rw::save<uint32_t>(p, VT_Out4);
        call(VoidFunction_copy, {p + 8, self + 8});
    }
    return p;
}
void out4CloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return;
    rw::save<uint32_t>(dst, VT_Out4);
    call(VoidFunction_copy, {dst + 8, self + 8});
}
void out4Destroy(uint32_t self) { functionDestroy(self + 8); }
void out4DestroyDealloc(uint32_t self) {
    functionDestroy(self + 8);
    deleteSelf(self);
}
void out4Call(uint32_t self) {
    if (ld32(self + 0x18)) call(VoidFunction_call, {self + 8});
}

void funcDelete(uint32_t self) { deleteSelf(self); }
void funcNop(uint32_t) {}
#define SRL_THUNKS(N, VT, TS, TI, SIZE, WORDS)                                                               \
    uint32_t N##Clone(uint32_t self) { return cloneNew(self, VT, SIZE, WORDS); }                               \
    void N##CloneInto(uint32_t self, uint32_t dst) { cloneInto(self, dst, VT, WORDS); }                       \
    uint32_t N##Target(uint32_t self, uint32_t ti) { return target(self, ti, TS); }                           \
    uint32_t N##TargetType(uint32_t) { return TI; }
SRL_THUNKS(back5, VT_Back5, TS_Back5, TI_Back5, 8, 0)
SRL_THUNKS(filter3, VT_Filter3, TS_Filter3, TI_Filter3, 8, 0)
SRL_THUNKS(apNg, VT_ApNg, TS_ApNg, TI_ApNg, 8, 0)
SRL_THUNKS(deck, VT_Deck, TS_Deck, TI_Deck, 8, 1)
SRL_THUNKS(exit1, VT_Exit1, TS_Exit1, TI_Exit1, 8, 1)
SRL_THUNKS(enter0, VT_Enter0, TS_Enter0, TI_Enter0, 8, 1)
#undef SRL_THUNKS
uint32_t select2Target(uint32_t self, uint32_t ti) { return target(self, ti, TS_Select2); }
uint32_t select2TargetType(uint32_t) { return TI_Select2; }
uint32_t out4Target(uint32_t self, uint32_t ti) { return target(self, ti, TS_Out4); }
uint32_t out4TargetType(uint32_t) { return TI_Out4; }
}  // namespace SceneRaidListLambdas

using namespace SceneRaidListLambdas;
// backScene $_5
RW_FUNCTION(0x6C7248, "SceneRaidList::backScene $_5 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x6C7250, "SceneRaidList::backScene $_5 __clone()", back5Clone);
RW_FUNCTION(0x6C7274, "SceneRaidList::backScene $_5 __clone(dst)", back5CloneInto);
RW_FUNCTION(0x6C7290, "SceneRaidList::backScene $_5 destroy", funcNop);
RW_FUNCTION(0x6C7292, "SceneRaidList::backScene $_5 destroy_deallocate", funcDelete);
RW_FUNCTION(0x6C729C, "SceneRaidList::backScene $_5 operator()", backCall);
RW_FUNCTION(0x6C7364, "SceneRaidList::backScene $_5 target", back5Target);
RW_FUNCTION(0x6C7384, "SceneRaidList::backScene $_5 target_type", back5TargetType);
// runOutAnimation $_4
RW_FUNCTION(0x6C7398, "SceneRaidList::runOutAnimation $_4 ~__func", out4Dtor);
RW_FUNCTION(0x6C73D4, "SceneRaidList::runOutAnimation $_4 ~__func (deleting)", out4DeletingDtor);
RW_FUNCTION(0x6C7414, "SceneRaidList::runOutAnimation $_4 __clone()", out4Clone);
RW_FUNCTION(0x6C7464, "SceneRaidList::runOutAnimation $_4 __clone(dst)", out4CloneInto);
RW_FUNCTION(0x6C7498, "SceneRaidList::runOutAnimation $_4 destroy", out4Destroy);
RW_FUNCTION(0x6C74B8, "SceneRaidList::runOutAnimation $_4 destroy_deallocate", out4DestroyDealloc);
RW_FUNCTION(0x6C74E4, "SceneRaidList::runOutAnimation $_4 operator()", out4Call);
RW_FUNCTION(0x6C74F4, "SceneRaidList::runOutAnimation $_4 target", out4Target);
RW_FUNCTION(0x6C7514, "SceneRaidList::runOutAnimation $_4 target_type", out4TargetType);
// init $_3 (filter)
RW_FUNCTION(0x6C7624, "SceneRaidList::init $_3 ~__func", funcNop);
RW_FUNCTION(0x6C7626, "SceneRaidList::init $_3 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x6C7630, "SceneRaidList::init $_3 __clone()", filter3Clone);
RW_FUNCTION(0x6C7654, "SceneRaidList::init $_3 __clone(dst)", filter3CloneInto);
RW_FUNCTION(0x6C7670, "SceneRaidList::init $_3 destroy", funcNop);
RW_FUNCTION(0x6C7672, "SceneRaidList::init $_3 destroy_deallocate", funcDelete);
RW_FUNCTION(0x6C767A, "SceneRaidList::init $_3 operator()", filterCall);
RW_FUNCTION(0x6C7680, "SceneRaidList::init $_3 target", filter3Target);
RW_FUNCTION(0x6C76A0, "SceneRaidList::init $_3 target_type", filter3TargetType);
// init $_2 (select)
RW_FUNCTION(0x6C76B4, "SceneRaidList::init $_2 ~__func", select2Dtor);
RW_FUNCTION(0x6C76E0, "SceneRaidList::init $_2 ~__func (deleting)", select2DeletingDtor);
RW_FUNCTION(0x6C7710, "SceneRaidList::init $_2 __clone()", select2Clone);
RW_FUNCTION(0x6C7768, "SceneRaidList::init $_2 __clone(dst)", select2CloneInto);
RW_FUNCTION(0x6C77A8, "SceneRaidList::init $_2 destroy", select2Destroy);
RW_FUNCTION(0x6C77B2, "SceneRaidList::init $_2 destroy_deallocate", select2DestroyDealloc);
RW_FUNCTION(0x6C77C8, "SceneRaidList::init $_2 operator()", selectCall);
RW_FUNCTION(0x6C7A2C, "SceneRaidList::init $_2 target", select2Target);
RW_FUNCTION(0x6C7A4C, "SceneRaidList::init $_2 target_type", select2TargetType);
// init $_2::{lambda(bool,bool)}
RW_FUNCTION(0x6C7A60, "SceneRaidList::init $_2 charge AP lambda ~__func (deleting)", funcDelete);
RW_FUNCTION(0x6C7A68, "SceneRaidList::init $_2 charge AP lambda __clone()", apNgClone);
RW_FUNCTION(0x6C7A8C, "SceneRaidList::init $_2 charge AP lambda __clone(dst)", apNgCloneInto);
RW_FUNCTION(0x6C7AA8, "SceneRaidList::init $_2 charge AP lambda destroy", funcNop);
RW_FUNCTION(0x6C7AAA, "SceneRaidList::init $_2 charge AP lambda destroy_deallocate", funcDelete);
RW_FUNCTION(0x6C7AB4, "SceneRaidList::init $_2 charge AP lambda operator()", apChargeCall);
RW_FUNCTION(0x6C7B40, "SceneRaidList::init $_2 charge AP lambda target", apNgTarget);
RW_FUNCTION(0x6C7B60, "SceneRaidList::init $_2 charge AP lambda target_type", apNgTargetType);
// init $_2::{lambda(bool)}
RW_FUNCTION(0x6C7B74, "SceneRaidList::init $_2 deck lambda ~__func (deleting)", funcDelete);
RW_FUNCTION(0x6C7B7C, "SceneRaidList::init $_2 deck lambda __clone()", deckClone);
RW_FUNCTION(0x6C7BA8, "SceneRaidList::init $_2 deck lambda __clone(dst)", deckCloneInto);
RW_FUNCTION(0x6C7BC8, "SceneRaidList::init $_2 deck lambda destroy", funcNop);
RW_FUNCTION(0x6C7BCA, "SceneRaidList::init $_2 deck lambda destroy_deallocate", funcDelete);
RW_FUNCTION(0x6C7BD4, "SceneRaidList::init $_2 deck lambda operator()", deckCall);
RW_FUNCTION(0x6C7C60, "SceneRaidList::init $_2 deck lambda target", deckTarget);
RW_FUNCTION(0x6C7C80, "SceneRaidList::init $_2 deck lambda target_type", deckTargetType);
// init $_1 (onExit)
RW_FUNCTION(0x6C7C94, "SceneRaidList::init $_1 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x6C7C9C, "SceneRaidList::init $_1 __clone()", exit1Clone);
RW_FUNCTION(0x6C7CC8, "SceneRaidList::init $_1 __clone(dst)", exit1CloneInto);
RW_FUNCTION(0x6C7CE8, "SceneRaidList::init $_1 destroy", funcNop);
RW_FUNCTION(0x6C7CEA, "SceneRaidList::init $_1 destroy_deallocate", funcDelete);
RW_FUNCTION(0x6C7CF2, "SceneRaidList::init $_1 operator()", exitCall);
RW_FUNCTION(0x6C7CFC, "SceneRaidList::init $_1 target", exit1Target);
RW_FUNCTION(0x6C7D1C, "SceneRaidList::init $_1 target_type", exit1TargetType);
// init $_0 (onEnter)
RW_FUNCTION(0x6C7D30, "SceneRaidList::init $_0 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x6C7D38, "SceneRaidList::init $_0 __clone()", enter0Clone);
RW_FUNCTION(0x6C7D64, "SceneRaidList::init $_0 __clone(dst)", enter0CloneInto);
RW_FUNCTION(0x6C7D84, "SceneRaidList::init $_0 destroy", funcNop);
RW_FUNCTION(0x6C7D86, "SceneRaidList::init $_0 destroy_deallocate", funcDelete);
RW_FUNCTION(0x6C7D8E, "SceneRaidList::init $_0 operator()", enterCall);
RW_FUNCTION(0x6C7D98, "SceneRaidList::init $_0 target", enter0Target);
RW_FUNCTION(0x6C7DB8, "SceneRaidList::init $_0 target_type", enter0TargetType);
