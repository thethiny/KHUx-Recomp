// SceneMedalList: Equipment > Medal List. The user's medals (sorted and filtered by IMedalSortFilter) in a
// seven-column MedalListScrollView, the sort button, the sell button and the medal count; back returns to MyPage.
// Also the two static helpers the screen uses: SceneUtil's count label (sub_473CAC) and the medal sort's
// std::__sort5 (sub_674B7A).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base, Layer_init = rwlib::cocos2d::Layer::init, Layer_dtor = rwlib::cocos2d::Layer::dtor_base;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t MedalVector_dtor = 0x46B119;            // __vector_base<shared_ptr<UserMedalData>>::~__vector_base
constexpr uint32_t MedalVector_pushSlow = 0x487A39;        // vector<shared_ptr<UserMedalData>>::__push_back_slow_path
constexpr uint32_t VectorULL_dtor = 0x468499;
constexpr uint32_t VectorULL_pushBackSlow = 0x4ECB6D;      // vector<ull>::__push_back_slow_path(const ull&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVB_move = 0x47BC69;             // std::function<void(bool)>(function&&)
constexpr uint32_t FunctionVB_call = 0x474139;             // std::function<void(bool)>::operator()(bool)
constexpr uint32_t Swap_int = 0x5428E1;                    // std::function<void(int)>::swap
constexpr uint32_t Swap_ull = 0x65F595;                    // std::function<void(ull)>::swap
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B;     // (sm, std::function<void()> by value)
constexpr uint32_t SM_setBackButtonType = 0x613E27;
constexpr uint32_t SM_showHeader = 0x6131ED;
constexpr uint32_t SM_changeBackground = 0x6128ED;         // (sm, BackGroundType, const string&)
constexpr uint32_t SM_changeTickerMessage = 0x610B39;      // (sm, const string&)
constexpr uint32_t SM_setFinishSceneCallback = 0x613E3F;   // (sm, function<void(function<void(bool)>)> by value)
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, std::function<void()> by value)
constexpr uint32_t IMedalSortFilter_runSortFilter = 0x66204D;    // (filter, SceneType)
constexpr uint32_t IMedalSortFilter_registSortButton = 0x664DA5; // (filter, widget, SceneType, function<void(bool)>)
constexpr uint32_t IMedalSortFilter_getShowType = 0x66527D;      // (filter, shared_ptr<MedalSortFilterState>, bool)
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D, IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t MedalListScrollView_create = 0x599F25, MedalListScrollView_setEnabledTapItem = 0x59AB11;
constexpr uint32_t MedalListScrollView_setMedalShowType = 0x59B2AD, MedalListScrollView_setMedalIDs = 0x59AB19;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int, UserMedal::Medal by value)
constexpr uint32_t SceneUtil_clearMedalNewMark = 0x46C4B5;
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;  // (function<void(bool)> by value)
constexpr uint32_t SceneUtil_updateMedalPossessionLabel = 0x473A19;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;      // (node, const string& name, text id)
constexpr uint32_t SortComparator = 0x674501, Sort4 = 0x674ADB;
// GOT: the class vtable, IMedalSortFilter's, the __func vtable of a std::function<Layer*()> holding a function
// pointer, createSceneLayer<SceneMyPage>, Color3B::RED / WHITE
constexpr uint32_t GOT_VTABLE = 0xFD60C0, GOT_VT_SORT_FILTER = 0xFD66B8;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_MYPAGE = 0xFD6078;
constexpr uint32_t GOT_COLOR_RED = 0xFD6064, GOT_COLOR_WHITE = 0xFD6060;
}  // namespace g

constexpr uint32_t SIZE = 0x2C8;
// fields
constexpr uint32_t F_SORT_FILTER = 0x288;  // IMedalSortFilter (second vtable)
constexpr uint32_t F_MEDALS = 0x28C;       // vector<shared_ptr<UserMedalData>>: all the user's medals
constexpr uint32_t F_SORTED = 0x298;       // vector<shared_ptr<UserMedalData>>: the sort filter's result
constexpr uint32_t F_INOUT = 0x2A4;        // IInOutAnimation (third vtable)
constexpr uint32_t F_ROOT = 0x2A8;
constexpr uint32_t F_SCROLL = 0x2B4;       // MedalListScrollView
constexpr uint32_t F_SELL_BUTTON = 0x2B8, F_SORT_BUTTON = 0x2BC;
constexpr uint32_t F_CAN_BACK = 0x2C0;
// MedalListScrollView members (std::function)
constexpr uint32_t SCROLL_ON_TAP = 0x638, SCROLL_ON_LONG_PRESS = 0x650;

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_BackKey = 0xF6DC68;     // init {this}
constexpr uint32_t VT_FinishScene = 0xF6DC3C; // init {}: sub_73F2B0
constexpr uint32_t VT_SortChanged = 0xF6DC10; // init {this}
constexpr uint32_t VT_MedalTap = 0xF6DB8C;    // init {this} void(int)
constexpr uint32_t VT_SellButton = 0xF6DB08;  // init {this}
constexpr uint32_t VT_LongPress = 0xF6DA84;   // init {this} void(ull)
constexpr uint32_t VT_RunOutDone = 0xF6DA58;  // runOutAnimationMain {std::function<void()>} (heap, 0x20)
constexpr uint32_t VT_Back = 0xF6D97C;        // backScene {}: sub_73DDA0

// strings
constexpr uint32_t S_SceneJson = 0xE07316;    // "cocostudio/publish/MedalListScene_ver130.json"
constexpr uint32_t S_Bg = 0xE03AF2;           // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_WinBottom = 0xE058DC, S_ScrollArea = 0xDF9B59, S_SellPanel = 0xE03D79, S_ButtonBack = 0xE03DBA;
constexpr uint32_t S_ButtonSort = 0xE03DC6, S_TxtSell1 = 0xE04F0E, S_ButtonSell1 = 0xE04F01;
constexpr uint32_t S_CenterUI = 0xE02FBA, S_LeftUI = 0xDF85F0;

constexpr int32_t TXT_TICKER = 100620045, TXT_SELL = 101100004;
constexpr uint32_t SORT_SCENE_MEDAL_LIST = 2;   // IMedalSortFilter::SceneType
constexpr uint32_t API_USER_MEDALS = 0xD4;      // SafeBuffer of UserMedal::Medal records (0x48 bytes), count at +0x20
constexpr uint32_t MEDAL_RECORD = 0x48;
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548;

constexpr uint32_t V_init = 0x250 - 8, V_deleting = 0x04, V_runInAnimation = 0x300, V_getSortFilterState = 0x308;
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
// swap the member std::function at `member` with a new lambda (the old one is destroyed)
void swapInto(uint32_t member, uint32_t swap, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, captures);
    call(swap, {fo, member});
    functionDestroy(fo);
}
// node->runAction(EaseSineOut(make(node, 0, 0, done)))
using MakeAnim = SceneUtil::Addr (*)(SceneUtil::Addr, float, float, SceneUtil::Addr);
void runEased(MakeAnim make, uint32_t node, uint32_t done) {
    uint32_t action = make(node, 0.0f, 0.0f, done);
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
void runEasedEmpty(MakeAnim make, uint32_t node) {
    Frame f;
    uint32_t done = newEmptyFunction(f);
    runEased(make, node, done);
    functionDestroy(done);
}
// vector<shared_ptr<T>>::push_back(const shared_ptr&)
void pushShared(uint32_t vec, uint32_t sp) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(g::MedalVector_pushSlow, {vec, sp});
        return;
    }
    if (end) {
        st32(end, ld32(sp));
        st32(end + 4, ld32(sp + 4));
        addRefIf(ld32(sp + 4));
    }
    st32(vec + 4, ld32(vec + 4) + 8);
}
}  // namespace

struct SceneMedalList {
    static uint32_t create();
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static void runInAnimation(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static void finishScene(uint32_t functor, uint32_t next);
    static void setCountLabel(uint32_t root, uint32_t name, uint32_t text, uint32_t warning);
    static uint32_t sort5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t comp);
};

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t SceneMedalList::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    call(g::aeabi_memset, {me + F_MEDALS, 0x18, 0});
    const uint32_t vt = ld32(g::GOT_VTABLE);
    st32(me, vt + 8);
    st32(me + F_SORT_FILTER, vt + 0x31C);
    st32(me + F_INOUT, vt + 0x330);
    st32(me + F_SELL_BUTTON, 0);
    st32(me + F_SORT_BUTTON, 0);
    st8(me + F_CAN_BACK, 0);
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t SceneMedalList::destruct(uint32_t me) {
    const uint32_t vt = ld32(g::GOT_VTABLE);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x330);
    st32(me + F_SORT_FILTER, ld32(g::GOT_VT_SORT_FILTER) + 8);
    call(g::MedalVector_dtor, {me + F_SORTED});
    call(g::MedalVector_dtor, {me + F_MEDALS});
    call(g::Layer_dtor, {me});
    return me;
}

// ------------------------------------------------------------------------------------------------ init
uint32_t SceneMedalList::init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    Frame f;
    {
        const uint32_t s = newStr(f, S_SceneJson, 0x2D);
        st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
        strFree(s);
    }
    {
        const uint32_t bottom = findNode(ld32(me + F_ROOT), S_WinBottom, 0xA);
        if (bottom) vcall(bottom, V_widgetSetTouchEnabled, {bottom, 1});
    }
    {
        const uint32_t m = sceneManager();
        const uint32_t fo = newFunction(f, VT_BackKey, {me});
        call(g::SM_setOnBackKeyCallback, {m, fo});
        functionDestroy(fo);
    }
    call(g::SM_setBackButtonType, {sceneManager(), 0});
    call(g::SM_showHeader, {sceneManager()});
    {
        const uint32_t m = sceneManager();
        const uint32_t s = newStr(f, S_Bg, 0x1A);
        call(g::SM_changeBackground, {m, 0, s});
        strFree(s);
    }
    {
        const uint32_t m = sceneManager();
        const uint32_t td = textData(f, TXT_TICKER);
        const uint32_t s = newStr(f, textDataStr(td));
        call(g::SM_changeTickerMessage, {m, s});
        strFree(s);
        sharedRelease(td);
    }
    {
        const uint32_t m = sceneManager();
        const uint32_t fo = newFunction(f, VT_FinishScene, {});
        call(g::SM_setFinishSceneCallback, {m, fo});
        functionDestroy(fo);
    }
    const uint32_t area = findNode(ld32(me + F_ROOT), S_ScrollArea, 0xB);
    st32(me + F_SCROLL, call(g::MedalListScrollView_create, {}));
    call(g::IScrollProtocol_setMaxColumn, {ld32(me + F_SCROLL), 7});
    call(g::MedalListScrollView_setEnabledTapItem, {ld32(me + F_SCROLL), 1});
    {
        const uint32_t scroll = ld32(me + F_SCROLL);
        vcall(scroll, node::setContentSize, {scroll, vcall(area, node::getContentSize, {area})});
    }
    vcall(area, node::addChild, {area, ld32(me + F_SCROLL)});

    // the user's medals, sorted by the filter
    while (ld32(me + F_MEDALS + 4) != ld32(me + F_MEDALS)) {
        const uint32_t last = ld32(me + F_MEDALS + 4) - 8;
        st32(me + F_MEDALS + 4, last);
        releaseIf(ld32(last + 4));
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t medalsCtl = ld32(api + API_USER_MEDALS + 4), medals = ld32(api + API_USER_MEDALS);
    addRefIf(medalsCtl);
    if (ld32(medals + 0x20)) {
        const uint32_t record = f.alloc(MEDAL_RECORD);
        for (uint32_t n = 0, at = 0;; at += MEDAL_RECORD) {
            const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
            rw::call(g::SafeBuffer_read, {medals, record, ld32(medals + 0xC) + at, MEDAL_RECORD, at});
            Frame m;
            const uint32_t medal = m.alloc(8);
            call(g::MedalDataManager_getMedalData, {medal, mdm, ld32(record), ld32(record + 4), 0, 0, 0, 0, 0, 0, 0, 0,
                                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
            pushShared(me + F_MEDALS, medal);
            releaseIf(ld32(medal + 4));
            if (!(++n < ld32(medals + 0x20))) break;
        }
    }
    call(g::IMedalSortFilter_runSortFilter, {me + F_SORT_FILTER, SORT_SCENE_MEDAL_LIST});
    const uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    if (ld32(me + F_SORTED + 4) != ld32(me + F_SORTED)) {
        for (uint32_t i = 0;;) {
            const uint32_t data = ld32(ld32(me + F_SORTED) + 8 * i);
            const uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(g::VectorULL_pushBackSlow, {ids, data});
            } else {
                uint32_t n = 0;
                if (end) {
                    st32(end, ld32(data));
                    st32(end + 4, ld32(data + 4));
                    n = ld32(ids + 4);
                }
                st32(ids + 4, n + 8);
            }
            i++;
            if (i >= uint32_t(int32_t(ld32(me + F_SORTED + 4) - ld32(me + F_SORTED)) >> 3)) break;
        }
    }
    uint32_t showType;
    {
        const uint32_t state = f.alloc(8);
        vcall(me, V_getSortFilterState, {state, me});
        showType = call(g::IMedalSortFilter_getShowType, {me + F_SORT_FILTER, state, 1});
        releaseIf(ld32(state + 4));
    }
    call(g::MedalListScrollView_setMedalShowType, {ld32(me + F_SCROLL), showType});
    call(g::IScrollProtocol_setIndex, {ld32(me + F_SCROLL), 0});
    call(g::MedalListScrollView_setMedalIDs, {ld32(me + F_SCROLL), ids});
    st8(me + F_CAN_BACK, 1);
    vcall(me, V_runInAnimation, {me});
    vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
    setVisible(findNode(ld32(me + F_ROOT), S_SellPanel, 0x10), 1);
    {
        const uint32_t back = findNode(ld32(me + F_ROOT), S_ButtonBack, 0xB);
        SceneUtil::backBtnSetPosY(back);
        setVisible(back, 0);
    }
    {
        const uint32_t panel = findNode(ld32(me + F_ROOT), S_SellPanel, 0x10);
        st32(me + F_SORT_BUTTON, findNode(panel, S_ButtonSort, 0xB));
        const uint32_t cb = newFunction(f, VT_SortChanged, {me});
        call(g::IMedalSortFilter_registSortButton, {me + F_SORT_FILTER, panel, SORT_SCENE_MEDAL_LIST, cb});
        functionDestroy(cb);
    }
    swapInto(ld32(me + F_SCROLL) + SCROLL_ON_TAP, g::Swap_int, VT_MedalTap, {me});
    {
        const uint32_t root = ld32(me + F_ROOT);
        const uint32_t s = newStr(f, S_TxtSell1, 9);
        call(g::SceneUtil_settingTitleLabel, {root, s, uint32_t(TXT_SELL)});
        strFree(s);
    }
    st32(me + F_SELL_BUTTON, findNode(ld32(me + F_ROOT), S_ButtonSell1, 0xC));
    addTouch(ld32(me + F_SELL_BUTTON), VT_SellButton, {me});
    swapInto(ld32(me + F_SCROLL) + SCROLL_ON_LONG_PRESS, g::Swap_ull, VT_LongPress, {me});
    call(g::SceneUtil_updateMedalPossessionLabel, {ld32(me + F_ROOT)});
    call(g::VectorULL_dtor, {ids});
    releaseIf(medalsCtl);
    return 1;
}

// ------------------------------------------------------------------------------------------------ animations
// The centre and left panels slide in; their first child does not take touches.
void SceneMedalList::runInAnimation(uint32_t me) {
    for (const auto &[name, len] : {std::pair{S_CenterUI, 8u}, std::pair{S_LeftUI, 6u}}) {
        const uint32_t node = findNode(ld32(me + F_ROOT), name, len);
        runEasedEmpty(SceneUtil::createLeftInAnimation, node);
        const uint32_t child = ld32(ld32(vcall(node, node::getChildren, {node})));
        vcall(child, V_widgetSetTouchEnabled, {child, 0});
    }
}

// Slide both panels out; `done` (std::function<void()>) runs when the centre panel is gone.
void SceneMedalList::runOutAnimationMain(uint32_t me, uint32_t done) {
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    const uint32_t center = findNode(ld32(me + F_ROOT), S_CenterUI, 8);
    {
        Frame f;
        const uint32_t captured = f.alloc(20);
        call(fn::function_vv_copy, {captured, done});
        const uint32_t onDone = f.alloc(20);
        heapFunction(onDone, VT_RunOutDone, 0x20, 8, fn::function_vv_move, captured);
        runEased(SceneUtil::createLeftOutAnimation, center, onDone);
        functionDestroy(onDone);
        functionDestroy(captured);
    }
    runEasedEmpty(SceneUtil::createLeftOutAnimation, findNode(ld32(me + F_ROOT), S_LeftUI, 6));
}

// ------------------------------------------------------------------------------------------------ back
void SceneMedalList::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    st8(me + F_CAN_BACK, 0);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Back, {});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// backScene's lambda: refresh the MyPage data and return there (fade).
void SceneMedalList::backDone(uint32_t) {
    Frame f;
    {
        const uint32_t m = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    }
    {
        const uint32_t none = newEmptyFunction(f);
        call(g::SceneUtil_callPrepareMypageAPIs, {none});
        functionDestroy(none);
    }
    const uint32_t m = sceneManager();
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(g::GOT_CREATE_MYPAGE)});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 1, 1});
    functionDestroy(layer);
}

// init's finish-scene lambda (function<void(bool)>&& next): clear the medals' "new" marks, then next(true).
void SceneMedalList::finishScene(uint32_t, uint32_t next) {
    Frame f;
    const uint32_t fo = f.alloc(20);
    call(g::FunctionVB_move, {fo, next});
    call(g::SceneUtil_clearMedalNewMark, {});
    call(g::FunctionVB_call, {fo, 1});
    functionDestroy(fo);
}

// ------------------------------------------------------------------------------------------------ helpers
// SceneUtil's count label: node `name` under root gets `text`, a shadow, and red (warning) or white.
void SceneMedalList::setCountLabel(uint32_t root, uint32_t name, uint32_t text, uint32_t warning) {
    uint32_t label;
    {
        Frame f;
        const uint32_t s = newStr(f, strData(name));
        label = SceneUtil::searchNodeByName(root, s);
        strFree(s);
    }
    {
        Frame f;
        const uint32_t s = newStr(f, strData(text));
        call(g::Text_setString, {label, s});
        strFree(s);
    }
    SceneUtil::setShadow(label);
    vcall(label, node::setColor, {label, ld32(warning == 1 ? g::GOT_COLOR_RED : g::GOT_COLOR_WHITE)});
}

// std::__sort5 of the medal sort (shared_ptrs, comparator sub_674500): returns the swaps made
uint32_t SceneMedalList::sort5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t comp) {
    const auto less = [&](uint32_t a, uint32_t b) { return call(g::SortComparator, {comp, a, b}) == 1; };
    const auto swap8 = [](uint32_t x, uint32_t y) {
        const uint32_t x0 = ld32(x), x1v = ld32(x + 4);
        st32(x, ld32(y));
        st32(y, x0);
        st32(x + 4, ld32(y + 4));
        st32(y + 4, x1v);
    };
    uint32_t r = call(g::Sort4, {x1, x2, x3, x4, comp});
    if (!less(x5, x4)) return r;
    swap8(x4, x5);
    r++;
    if (!less(x4, x3)) return r;
    swap8(x3, x4);
    r++;
    if (!less(x3, x2)) return r;
    swap8(x2, x3);
    r++;
    if (!less(x2, x1)) return r;
    swap8(x1, x2);
    return r + 1;
}

RW_FUNCTION(0x48A634, "SceneMedalList::create", SceneMedalList::create);
RW_FUNCTION(0x73CCF0, "SceneMedalList::~SceneMedalList", SceneMedalList::destruct);
RW_FUNCTION(0x73CDA4, "SceneMedalList::init", SceneMedalList::init);
RW_FUNCTION(0x73D6E4, "SceneMedalList::runInAnimation", SceneMedalList::runInAnimation);
RW_FUNCTION(0x73D8D0, "SceneMedalList::runOutAnimationMain", SceneMedalList::runOutAnimationMain);
RW_FUNCTION(0x73DCA0, "SceneMedalList::backScene", SceneMedalList::backScene);
RW_FUNCTION(0x73DDA0, "SceneMedalList::backScene lambda __func::operator() (sub_73DDA0)", SceneMedalList::backDone);
RW_FUNCTION(0x73F2B0, "SceneMedalList::init finish-scene lambda __func::operator() (sub_73F2B0)", SceneMedalList::finishScene);
RW_FUNCTION(0x473CAC, "SceneUtil count label helper (sub_473CAC)", SceneMedalList::setCountLabel);
RW_FUNCTION(0x674B7A, "std::__sort5<runSortFilter comparator&, shared_ptr<UserMedalData>*> (sub_674B7A)",
            SceneMedalList::sort5);
