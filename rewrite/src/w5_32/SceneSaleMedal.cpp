// SceneSaleMedal: Medal List > Sell Medals (also reached from fusion, the shop and other screens - the back scene
// type). The user's medals (sorted / filtered by IMedalSortFilter, scene type 1) in a seven-column
// SellMedalScrollView; the selection's Munny and Avatar Coins (a rare medal warns), Sell (enabled with a
// selection) opens the confirm popup; back returns to the scene it came from.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_init = 0x88A8A1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t TextData_format = 0x492929;             // (out string, type, id, ...)
constexpr uint32_t MedalVector_dtor = 0x46B119;            // __vector_base<shared_ptr<UserMedalData>>::~__vector_base
constexpr uint32_t MedalVector_pushSlow = 0x487A39;        // vector<shared_ptr<UserMedalData>>::__push_back_slow_path
constexpr uint32_t FunctionVBVector_dtor = 0x60CA61;       // __vector_base<function<void(bool)>>::~__vector_base
constexpr uint32_t FunctionVBVector_pushSlow = 0x65E98D;   // vector<function<void(bool)>>::__push_back_slow_path(&&)
constexpr uint32_t VectorULL_copy = 0x468431, VectorULL_dtor = 0x468499;
constexpr uint32_t VectorULL_pushBackSlow = 0x4ECB6D;      // vector<ull>::__push_back_slow_path(const ull&)
constexpr uint32_t U64Set_dtor = 0x482619, U64Set_clear = 0x65E58F, U64Set_insertUnique = 0x482649;
constexpr uint32_t U64IntMap_find = 0x490275;              // unordered_map<ull, int>::find(const ull&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVB_move = 0x47BC69;             // std::function<void(bool)>(function&&)
constexpr uint32_t Swap_int = 0x5428E1;                    // std::function<void(int)>::swap
constexpr uint32_t Swap_ull = 0x65F595;                    // std::function<void(ull)>::swap
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B;     // (sm, std::function<void()> by value)
constexpr uint32_t SM_changeBackground = 0x6128ED;         // (sm, BackGroundType, const string&)
constexpr uint32_t SM_changeTickerMessage = 0x610B39;      // (sm, const string&)
constexpr uint32_t SM_setFinishSceneCallback = 0x613E3F;   // (sm, function<void(function<void(bool)>)> by value)
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t SM_getPopupLayer = 0x613E69, SM_openPopup = 0x61524D;
constexpr uint32_t SM_isOpenedPopup = 0x621519, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, std::function<void()> by value)
constexpr uint32_t IMedalSortFilter_runSortFilter = 0x66204D;    // (filter, SceneType)
constexpr uint32_t IMedalSortFilter_registSortButton = 0x664DA5; // (filter, widget, SceneType, function<void(bool)>)
constexpr uint32_t IMedalSortFilter_getShowType = 0x66527D;      // (filter, shared_ptr<MedalSortFilterState>, bool)
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D, IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t SellMedalScrollView_create = 0x4FEFD1, SellMedalScrollView_setEnabledTapItem = 0x4FFD1D;
constexpr uint32_t SellMedalScrollView_setMedalShowType = 0x5004D1, SellMedalScrollView_setMedalIDs = 0x4FFD25;
constexpr uint32_t SellMedalScrollView_getSelectedMedals = 0x5003B5;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int, UserMedal::Medal by value)
constexpr uint32_t Burst_create = 0x77A491;
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;           // (sret string, -, unsigned long long)
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;      // (button, text, bool, bool)
constexpr uint32_t SceneUtil_clearMedalNewMark = 0x46C4B5;
constexpr uint32_t SceneUtil_updateMedalPossessionLabel = 0x473A19;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;      // (node, const string& name, text id)
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;   // (Text*, const string& name)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;  // (function<void(bool)> by value)
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t SceneMixedMedal_callPrepareAPI = 0x69F475;   // (function<void(bool)> by value)
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;
constexpr uint32_t APIManager_executeRaidList = 0x4CC009;
constexpr uint32_t SceneSaleMedal_backScene2 = 0x65D7B1;
// GOT: the class vtable, IMedalSortFilter's, the __func vtable of a std::function<Layer*()> holding a function
// pointer, the scene factories, the popup fade time (&float)
constexpr uint32_t GOT_VTABLE = 0xFD60B8, GOT_VT_SORT_FILTER = 0xFD66B8, GOT_FUNC_LAYER_VT = 0xFD6074;
constexpr uint32_t GOT_CREATE_MYPAGE = 0xFD6078, GOT_CREATE_MEDAL_LIST = 0xFD60BC;
constexpr uint32_t GOT_CREATE_ADVENTURE = 0xFD669C, GOT_CREATE_RAID_LIST = 0xFD66A8, GOT_CREATE_SHOP_TOP = 0xFD66F8;
constexpr uint32_t GOT_POPUP_FADE = 0xFD65C0;
}  // namespace g

constexpr uint32_t SIZE = 0x318;
// fields
constexpr uint32_t F_SORT_FILTER = 0x288;  // IMedalSortFilter (second vtable)
constexpr uint32_t F_MEDALS = 0x28C;       // vector<shared_ptr<UserMedalData>>: all the user's medals
constexpr uint32_t F_SORTED = 0x298;       // vector<shared_ptr<UserMedalData>>: the sort filter's result
constexpr uint32_t F_INOUT = 0x2A4;        // IInOutAnimation (third vtable)
constexpr uint32_t F_BACK = 0x2A8;         // SceneUtil::BackScene: where back goes
constexpr uint32_t F_BACK2 = 0x2AC;        // SceneMixedMedal::BackScene (from fusion)
constexpr uint32_t F_EXCLUDED = 0x2B0;     // ull: the medal that is never sold (the fusion base)
constexpr uint32_t F_ROOT = 0x2B8;
constexpr uint32_t F_BOX_MAX = 0x2BC, F_BOX_NOW = 0x2C0;
constexpr uint32_t F_SCROLL = 0x2C4;       // SellMedalScrollView
constexpr uint32_t F_TOTAL_MONEY = 0x2C8, F_TOTAL_COINS = 0x2CC;
constexpr uint32_t F_SELL_BUTTON = 0x2D0, F_SORT_BUTTON = 0x2D4;
constexpr uint32_t F_MIXED_ID = 0x2D8;     // ull: SceneMixedMedal's selected medal (setSelectedMixedMedalItemID)
constexpr uint32_t F_SELECTED = 0x2E0;     // unordered_set<ull>: the selected medals (+0xC their count)
constexpr uint32_t F_SUM_MONEY = 0x2F4, F_SUM_COINS = 0x2F8;
constexpr uint32_t F_RARE = 0x2FC;         // a selected medal of rarity 3 and up
constexpr uint32_t F_INDEX = 0x300;        // the list's scroll index (setMedalListIndex)
constexpr uint32_t F_CAN_BACK = 0x304;
constexpr uint32_t F_POPUP_CLOSES = 0x308; // vector<function<void(bool)>>: the sell popups' close handlers
// SellMedalScrollView
constexpr uint32_t SCROLL_ITEM_IDS = 0x628, SCROLL_ON_TAP = 0x638, SCROLL_ON_LONG_PRESS = 0x650;

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_BackKey = 0xF62B54;      // init {this}
constexpr uint32_t VT_FinishScene = 0xF62B28;  // init {}
constexpr uint32_t VT_BackButton = 0xF62AFC;   // init {this}
constexpr uint32_t VT_SellButton = 0xF62AD0;   // init {this}
constexpr uint32_t VT_SortChanged = 0xF62AA4;  // init {this}
constexpr uint32_t VT_MedalTap = 0xF62A78;     // init {this} void(int)
constexpr uint32_t VT_LongPress = 0xF62A20;    // init {this} void(ull)
constexpr uint32_t VT_RunOutDone = 0xF629F4;   // runOutAnimationMain {std::function<void()>} (heap 0x20)
constexpr uint32_t VT_SellConfirm = 0xF62918;  // openSellPopup {sell, close, this, this, close} (heap 0x18)
constexpr uint32_t VT_CloseButton = 0xF628EC;  // openSellPopup {}
constexpr uint32_t VT_PopupClosed = 0xF62894;  // sub_65D71C {sell, close, window, layer, shadow, popup, this} (heap 0x20)
constexpr uint32_t VT_CanClose = 0xF62868;     // openSellPopup {close}
constexpr uint32_t VT_Back = 0xF627E4;         // backScene {this}
constexpr uint32_t VT_ToFusion = 0xF6283C;     // the back lambda (fusion) {back2}
constexpr uint32_t VT_ToEvolve = 0xF62810;     // the back lambda (evolution) {back2, -, mixed id} (heap 0x18)

// strings
constexpr uint32_t S_SceneJson = 0xE03D2F;     // "cocostudio/publish/MedalSellScene_ver130.json"
constexpr uint32_t S_Bg = 0xE03AF2;            // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_ScrollArea = 0xDF9B59, S_TxtMoney = 0xDFE7A3, S_TxtMoneyLabel = 0xDFE7AD;
constexpr uint32_t S_TxtACoin = 0xE03D5D, S_TxtACoinLabel = 0xE03D68, S_SellPanel = 0xE03D79, S_DeckBase2 = 0xE03D8A;
constexpr uint32_t S_TxtMoneyTotal = 0xE03D94, S_TxtMoneyTotalLavel = 0xE03DA4, S_ButtonBack = 0xE03DBA;
constexpr uint32_t S_Button = 0xDFD6D5, S_TxtSell = 0xDF87F3, S_ButtonSort = 0xE03DC6;
constexpr uint32_t S_BoxMaxLabel = 0xE03DD2, S_BoxNow = 0xE03DDE, S_PercentD = 0xDF86DE;
constexpr uint32_t S_CenterUI = 0xE02FBA, S_LeftUI = 0xDF85F0, S_RightUI = 0xDF85F7;
constexpr uint32_t S_PopupJson = 0xE03DE5;     // "cocostudio/publish/PopupNormal_MedalSell_Check.json"
constexpr uint32_t S_Window = 0xDF878E, S_ButtonSell = 0xDF87E7, S_ButtonClose = 0xDF8692, S_TxtCancel = 0xDF86A6;
constexpr uint32_t S_TxtSellTotal = 0xE03E19, S_TxtSellTotalLabel = 0xDF9B96;
constexpr uint32_t S_TxtRare1 = 0xE03E27, S_TxtRare2 = 0xE03E31, S_WarningPlate = 0xE03E3B;

constexpr int32_t TXT_TICKER = 100620006, TXT_MONEY = 100300003, TXT_A_COIN = 101500006, TXT_TOTAL = 100300005;
constexpr int32_t TXT_SELL = 100100029, TXT_CANCEL = 100100002, TXT_SELL_ASK = 101502002;
constexpr int32_t TXT_RARE1 = 101400008, TXT_RARE2 = 101400009;
constexpr uint32_t SORT_SCENE_SALE = 1;          // IMedalSortFilter::SceneType
constexpr uint32_t API_USER_MEDALS = 0xD4;       // SafeBuffer of UserMedal::Medal records (0x48 bytes), count at +0x20
constexpr uint32_t API_MONEY = 0x8C;             // SafeBuffer: +0 Munny
constexpr uint32_t API_USER_DETAIL = 0x94;       // shared_ptr<UserDetail> (SafeBuffer: +0x34 medal box size)
constexpr uint32_t MEDAL_RECORD = 0x48;
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548, SM_FLAG_308 = 0x308, SM_FLAG_30A = 0x30A;
constexpr uint32_t SM_MEDAL = 0x2A0;             // SceneManager: ull, the medal the next screen works on
// UserMedalData: +0x14 sell price, +0x50 burst level, +0x58 master::Medal SafeBuffer (+0x470 rarity, +0x4AC
// burst id, +0x4BC Avatar Coins)
constexpr uint32_t UMD_PRICE = 0x14, UMD_BURST_LV = 0x50, UMD_MASTER = 0x58;

constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_runInAnimation = 0x300, V_getSortFilterState = 0x308;
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
inline void setTouchEnabled(uint32_t node, uint32_t on) { vcall(node, V_widgetSetTouchEnabled, {node, on}); }
void setTextChars(uint32_t text, uint32_t chars) {
    Frame f;
    const uint32_t s = f.alloc(12);
    strInit(s, chars, call(g::STRLEN, {chars}));
    call(fn::Text_setString, {text, s});
    strFree(s);
}
// text = SceneUtil::getMoneyStr(value)
void setMoney(uint32_t text, int32_t value) {
    Frame f;
    const uint32_t s = f.alloc(12);
    call(g::SceneUtil_getMoneyStr, {s, 0, uint32_t(value), uint32_t(value >> 31)});
    call(fn::Text_setString, {text, s});
    strFree(s);
}
// MedalDataManager::getMedalData(out, manager, id) (the record passed as zeros)
uint32_t medalData(Frame &f, uint32_t lo, uint32_t hi) {
    const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
    const uint32_t out = f.alloc(8);
    call(g::MedalDataManager_getMedalData, {out, mdm, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    return out;
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
// vector<ull> of the sorted medals' ids
void sortedIds(uint32_t me, uint32_t ids) {
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    if (ld32(me + F_SORTED + 4) == ld32(me + F_SORTED)) return;
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
// SceneManager::changeSceneLayer(fo, true, false, 0)
void changeScene(uint32_t fo) { rw::call(g::SM_changeSceneLayer, {sceneManager(), fo, 1, 0, 0}); }
void changeSceneTo(uint32_t gotFactory) {
    Frame f;
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(gotFactory)});
    changeScene(layer);
    functionDestroy(layer);
}
// sm+0x4CC = 1, the header callback cleared
void resetHeaderCallback() {
    Frame f;
    const uint32_t sm = sceneManager();
    const uint32_t none = newEmptyFunction(f);
    st8(sm + SM_FLAG_4CC, 1);
    call(g::FunctionVB_assign, {sm + SM_CALLBACK_548, none});
    functionDestroy(none);
}
// fnAddr(fn<void(bool)> by value) with an empty callback; fnAddr(api, fn) when `api` is given
void callWithEmpty(uint32_t fnAddr, uint32_t api = 0) {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    if (api) call(fnAddr, {api, none});
    else call(fnAddr, {none});
    functionDestroy(none);
}
}  // namespace

struct SceneSaleMedal {
    // ------------------------------------------------------------------------------------------------ life cycle
    static uint32_t create() {
        const uint32_t me = call(fn::operator_new, {SIZE});
        call(g::Layer_ctor, {me});
        call(g::aeabi_memset, {me + F_MEDALS, 0x18, 0});
        const uint32_t vt = ld32(g::GOT_VTABLE);
        st32(me, vt + 8);
        st32(me + F_SORT_FILTER, vt + 0x31C);
        st32(me + F_INOUT, vt + 0x330);
        call(g::aeabi_memset, {me + F_BACK, 0x48, 0});
        stf(me + 0x2F0, 1.0f);                  // the selected set's max load factor
        st32(me + F_SUM_MONEY, 0);
        st32(me + F_SUM_COINS, 0);
        st8(me + F_RARE, 0);
        st32(me + F_INDEX, 0);
        st8(me + F_CAN_BACK, 0);
        st32(me + F_POPUP_CLOSES, 0);
        st32(me + F_POPUP_CLOSES + 4, 0);
        st32(me + F_POPUP_CLOSES + 8, 0);
        if (vcall(me, V_init, {me}) == 1) {
            call(g::Ref_autorelease, {me});
            return me;
        }
        vcall(me, V_deleting, {me});
        return 0;
    }

    static uint32_t destruct(uint32_t me) {
        const uint32_t vt = ld32(g::GOT_VTABLE);
        st32(me, vt + 8);
        st32(me + F_SORT_FILTER, vt + 0x31C);
        st32(me + F_INOUT, vt + 0x330);
        call(g::FunctionVBVector_dtor, {me + F_POPUP_CLOSES});
        call(g::U64Set_dtor, {me + F_SELECTED});
        st32(me + F_SORT_FILTER, ld32(g::GOT_VT_SORT_FILTER) + 8);
        call(g::MedalVector_dtor, {me + F_SORTED});
        call(g::MedalVector_dtor, {me + F_MEDALS});
        call(g::Layer_dtor, {me});
        return me;
    }

    // ------------------------------------------------------------------------------------------------ init
    static uint32_t init(uint32_t me) {
        if (call(g::Layer_init, {me}) != 1) return 0;
        st8(me + F_CAN_BACK, 1);
        Frame f;
        {
            const uint32_t sm = sceneManager();
            const uint32_t fo = newFunction(f, VT_BackKey, {me});
            call(g::SM_setOnBackKeyCallback, {sm, fo});
            functionDestroy(fo);
        }
        {
            const uint32_t s = newStr(f, S_SceneJson, 0x2D);
            st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
            strFree(s);
        }
        {
            const uint32_t sm = sceneManager();
            const uint32_t s = newStr(f, S_Bg, 0x1A);
            call(g::SM_changeBackground, {sm, 0, s});
            strFree(s);
        }
        {
            const uint32_t sm = sceneManager();
            const uint32_t td = textData(f, TXT_TICKER);
            const uint32_t s = newStr(f, textDataStr(td));
            call(g::SM_changeTickerMessage, {sm, s});
            strFree(s);
            sharedRelease(td);
        }
        {
            const uint32_t sm = sceneManager();
            const uint32_t fo = newFunction(f, VT_FinishScene, {});
            call(g::SM_setFinishSceneCallback, {sm, fo});
            functionDestroy(fo);
        }
        const uint32_t area = findNode(ld32(me + F_ROOT), S_ScrollArea, 0xB);
        st32(me + F_SCROLL, call(g::SellMedalScrollView_create, {}));
        call(g::IScrollProtocol_setMaxColumn, {ld32(me + F_SCROLL), 7});
        call(g::SellMedalScrollView_setEnabledTapItem, {ld32(me + F_SCROLL), 1});
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
                call(g::MedalDataManager_getMedalData, {medal, mdm, ld32(record), ld32(record + 4), 0, 0, 0, 0, 0, 0, 0,
                                                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
                pushShared(me + F_MEDALS, medal);
                releaseIf(ld32(medal + 4));
                if (!(++n < ld32(medals + 0x20))) break;
            }
        }
        call(g::IMedalSortFilter_runSortFilter, {me + F_SORT_FILTER, SORT_SCENE_SALE});
        const uint32_t ids = f.alloc(12);
        sortedIds(me, ids);
        uint32_t showType;
        {
            const uint32_t state = f.alloc(8);
            vcall(me, V_getSortFilterState, {state, me});
            showType = call(g::IMedalSortFilter_getShowType, {me + F_SORT_FILTER, state, 1});
            releaseIf(ld32(state + 4));
        }
        call(g::SellMedalScrollView_setMedalShowType, {ld32(me + F_SCROLL), showType});
        st32(me + F_INDEX, 0);
        call(g::IScrollProtocol_setIndex, {ld32(me + F_SCROLL), 0});
        call(g::SellMedalScrollView_setMedalIDs, {ld32(me + F_SCROLL), ids, ld32(me + F_EXCLUDED), ld32(me + F_EXCLUDED + 4)});
        vcall(me, V_runInAnimation, {me});
        vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
        call(g::SceneUtil_updateMedalPossessionLabel, {ld32(me + F_ROOT)});
        titleLabel(ld32(me + F_ROOT), S_TxtMoney, 9, TXT_MONEY);
        updateHaveMoneynPanel(me);
        titleLabel(ld32(me + F_ROOT), S_TxtACoin, 0xA, TXT_A_COIN);
        st32(me + F_TOTAL_COINS, findNode(ld32(me + F_ROOT), S_TxtACoinLabel, 0x10));
        const uint32_t panel = findNode(ld32(me + F_ROOT), S_SellPanel, 0x10);
        setTouchEnabled(panel, 1);
        setTouchEnabled(findNode(ld32(me + F_ROOT), S_DeckBase2, 9), 1);
        {
            const uint32_t total = findNode(panel, S_TxtMoneyTotal, 0xF);
            SceneUtil::setShadow(total);
            setTextId(total, TXT_TOTAL);
        }
        st32(me + F_TOTAL_MONEY, findNode(ld32(me + F_ROOT), S_TxtMoneyTotalLavel, 0x15));
        SceneUtil::setShadow(ld32(me + F_TOTAL_MONEY));
        updateSellInfo(me);
        {
            const uint32_t back = findNode(ld32(me + F_ROOT), S_ButtonBack, 0xB);
            SceneUtil::backBtnSetPosY(back);
            addTouch(back, VT_BackButton, {me});
        }
        st32(me + F_SELL_BUTTON, findNode(panel, S_Button, 6));
        addTouch(ld32(me + F_SELL_BUTTON), VT_SellButton, {me});
        {
            const uint32_t text = findNode(ld32(me + F_SELL_BUTTON), S_TxtSell, 8);
            SceneUtil::setShadow(text);
            setTextId(text, TXT_SELL);
        }
        updateSellButton(me);
        {
            const uint32_t panel2 = findNode(ld32(me + F_ROOT), S_SellPanel, 0x10);
            st32(me + F_SORT_BUTTON, findNode(panel2, S_ButtonSort, 0xB));
            const uint32_t cb = newFunction(f, VT_SortChanged, {me});
            call(g::IMedalSortFilter_registSortButton, {me + F_SORT_FILTER, panel2, SORT_SCENE_SALE, cb});
            functionDestroy(cb);
        }
        swapInto(ld32(me + F_SCROLL) + SCROLL_ON_TAP, g::Swap_int, VT_MedalTap, {me});
        swapInto(ld32(me + F_SCROLL) + SCROLL_ON_LONG_PRESS, g::Swap_ull, VT_LongPress, {me});
        st32(me + F_BOX_MAX, findNode(ld32(me + F_ROOT), S_BoxMaxLabel, 0xB));
        st32(me + F_BOX_NOW, findNode(ld32(me + F_ROOT), S_BoxNow, 6));
        {
            const uint32_t a = call(fn::APIManager_getInstance, {});
            const uint32_t detailCtl = ld32(a + API_USER_DETAIL + 4), detail = ld32(a + API_USER_DETAIL);
            addRefIf(detailCtl);
            const uint32_t boxMax = ld32(me + F_BOX_MAX);
            const uint32_t size = f.alloc(4);
            rw::call(g::SafeBuffer_read, {detail, size, ld32(detail + 0xC) + 0x34, 4, 0x34});
            const uint32_t buf = f.alloc(0x404);
            call(g::StringBuffer_ctor, {buf, S_PercentD, ld32(size)});
            setTextChars(boxMax, buf);
            const uint32_t boxNow = ld32(me + F_BOX_NOW);
            const uint32_t scroll = ld32(me + F_SCROLL);
            const uint32_t buf2 = f.alloc(0x404);
            call(g::StringBuffer_ctor,
                 {buf2, S_PercentD, uint32_t(int32_t(ld32(scroll + SCROLL_ITEM_IDS + 4) - ld32(scroll + SCROLL_ITEM_IDS)) >> 3)});
            setTextChars(boxNow, buf2);
            releaseIf(detailCtl);
        }
        call(g::VectorULL_dtor, {ids});
        releaseIf(medalsCtl);
        return 1;
    }

    // ------------------------------------------------------------------------------------------------ panel
    // The Munny the user has.
    static void updateHaveMoneynPanel(uint32_t me) {
        const uint32_t label = findNode(ld32(me + F_ROOT), S_TxtMoneyLabel, 0xF);
        SceneUtil::setShadow(label);
        const uint32_t sb = ld32(call(fn::APIManager_getInstance, {}) + API_MONEY);
        Frame f;
        const uint32_t money = f.alloc(4);
        rw::call(g::SafeBuffer_read, {sb, money, ld32(sb + 0xC), 4, 0});
        setMoney(label, int32_t(ld32(money)));
    }

    // The selected medals: their ids (the set), the Munny they sell for, the Avatar Coins (a medal whose burst
    // is at its burst's top level), whether one is rare (3 and up).
    static void updateSellInfo(uint32_t me) {
        call(g::U64Set_clear, {me + F_SELECTED});
        st32(me + F_SUM_MONEY, 0);
        st32(me + F_SUM_COINS, 0);
        st8(me + F_RARE, 0);
        Frame f;
        const uint32_t idsCopy = f.alloc(12), selected = f.alloc(12);
        call(g::VectorULL_copy, {idsCopy, ld32(me + F_SCROLL) + SCROLL_ITEM_IDS});
        call(g::SellMedalScrollView_getSelectedMedals, {selected, ld32(me + F_SCROLL)});
        if (ld32(selected + 4) != ld32(selected)) {
            for (uint32_t i = 0; i < uint32_t(int32_t(ld32(selected + 4) - ld32(selected)) >> 3); i++) {
                const uint32_t at = ld32(selected) + 8 * i;
                Frame m;
                const uint32_t medal = medalData(m, ld32(at), ld32(at + 4));
                const uint32_t inserted = m.alloc(8);
                call(g::U64Set_insertUnique, {inserted, me + F_SELECTED, ld32(medal)});
                const uint32_t data = ld32(medal);
                st32(me + F_SUM_MONEY, ld32(me + F_SUM_MONEY) + ld32(data + UMD_PRICE));
                const uint32_t master = ld32(data + UMD_MASTER);
                const uint32_t rarity = m.alloc(4), burstId = m.alloc(4), burstTop = m.alloc(4), coins = m.alloc(4);
                rw::call(g::SafeBuffer_read, {master, rarity, ld32(master + 0xC) + 0x470, 4, 0x470});
                if (int32_t(ld32(rarity)) >= 3) st8(me + F_RARE, 1);
                const uint32_t master2 = ld32(ld32(medal) + UMD_MASTER);
                rw::call(g::SafeBuffer_read, {master2, burstId, ld32(master2 + 0xC) + 0x4AC, 4, 0x4AC});
                const uint32_t burst = m.alloc(8);
                call(g::Burst_create, {burst, ld32(burstId)});
                if (ld32(burst)) {
                    rw::call(g::SafeBuffer_read, {ld32(burst), burstTop, ld32(ld32(burst) + 0xC) + 0x158, 4, 0x158});
                    if (int32_t(ld32(burstTop)) >= 1 && ld32(ld32(medal) + UMD_BURST_LV) == ld32(burstTop)) {
                        const uint32_t master3 = ld32(ld32(medal) + UMD_MASTER);
                        rw::call(g::SafeBuffer_read, {master3, coins, ld32(master3 + 0xC) + 0x4BC, 4, 0x4BC});
                        st32(me + F_SUM_COINS, ld32(me + F_SUM_COINS) + ld32(coins));
                    }
                }
                releaseIf(ld32(burst + 4));
                releaseIf(ld32(medal + 4));
            }
        }
        setMoney(ld32(me + F_TOTAL_MONEY), int32_t(ld32(me + F_SUM_MONEY)));
        setMoney(ld32(me + F_TOTAL_COINS), int32_t(ld32(me + F_SUM_COINS)));
        call(g::VectorULL_dtor, {selected});
        call(g::VectorULL_dtor, {idsCopy});
    }

    // Sell is enabled with a selection.
    static void updateSellButton(uint32_t me) {
        const uint32_t count = ld32(me + F_SELECTED + 0xC);
        const uint32_t text = findNode(ld32(me + F_SELL_BUTTON), S_TxtSell, 8);
        call(g::SceneUtil_setButtonEnabled, {ld32(me + F_SELL_BUTTON), text, count ? 1u : 0u, 0});
    }

    // Where back goes; from fusion / evolution (1, 2) the medal being worked on is never sold and the list is
    // set again.
    static void setBackScene(uint32_t me, uint32_t type) {
        st32(me + F_BACK, type);
        if (type - 1 > 1) return;
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t medal = medalData(f, ld32(sm + SM_MEDAL), ld32(sm + SM_MEDAL + 4));
        st32(me + F_EXCLUDED + 4, ld32(ld32(medal) + 4));
        st32(me + F_EXCLUDED, ld32(ld32(medal)));
        const uint32_t ids = f.alloc(12);
        sortedIds(me, ids);
        st32(me + F_INDEX, 0);
        call(g::IScrollProtocol_setIndex, {ld32(me + F_SCROLL), 0});
        call(g::SellMedalScrollView_setMedalIDs, {ld32(me + F_SCROLL), ids, ld32(me + F_EXCLUDED), ld32(me + F_EXCLUDED + 4)});
        call(g::VectorULL_dtor, {ids});
        releaseIf(ld32(medal + 4));
    }

    // ------------------------------------------------------------------------------------------------ animations
    // The centre and left panels slide in from the left, the right one from the right (and comes to the front);
    // their first child does not take touches.
    static void runInAnimation(uint32_t me) {
        for (const auto &[name, len] : {std::pair{S_CenterUI, 8u}, std::pair{S_LeftUI, 6u}}) {
            const uint32_t node = findNode(ld32(me + F_ROOT), name, len);
            runEasedEmpty(SceneUtil::createLeftInAnimation, node);
            const uint32_t child = ld32(ld32(vcall(node, node::getChildren, {node})));
            setTouchEnabled(child, 0);
        }
        const uint32_t right = findNode(ld32(me + F_ROOT), S_RightUI, 7);
        runEasedEmpty(SceneUtil::createRightInAnimation, right);
        vcall(right, node::setLocalZOrder, {right, 2});
        const uint32_t child = ld32(ld32(vcall(right, node::getChildren, {right})));
        setTouchEnabled(child, 0);
    }

    // Slide the three panels out; `done` (std::function<void()>) runs when the centre panel is gone.
    static void runOutAnimationMain(uint32_t me, uint32_t done) {
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
        runEasedEmpty(SceneUtil::createRightOutAnimation, findNode(ld32(me + F_ROOT), S_RightUI, 7));
    }

    // ------------------------------------------------------------------------------------------------ sell popup
    // "Sell n medals?" with the Munny total and the rare-medal warning, Sell / Cancel; a SceneManager popup whose
    // close handler is also kept in the scene's list.
    static void openSellPopup(uint32_t me) {
        call(fn::AudioPlaySystemSEPopUp, {});
        const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
        const uint32_t shadow = SceneUtil::createShadowSprite();
        const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(g::Node_runAction, {shadow, call(fn::FadeTo_create, {ld32(ld32(g::GOT_POPUP_FADE)), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
        const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PopupJson});
        {
            const float x = bitsf(vcall(popup, node::getPositionX, {popup}));
            vcall(popup, node::setPositionX, {popup, fbits(x + SceneUtil::getSideMargin())});
            const float y = bitsf(vcall(popup, node::getPositionY, {popup}));
            vcall(popup, node::setPositionY, {popup, fbits(y + SceneUtil::getSideHeightMargin())});
        }
        vcall(layer, node::addChild, {layer, popup});
        const uint32_t window = findNode(popup, S_Window, 0x13);
        const uint32_t sell = findNode(popup, S_ButtonSell, 0xB);
        const uint32_t close = findNode(popup, S_ButtonClose, 0xC);
        Frame f;
        {
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x18});
            if (obj) {
                st32(obj, VT_SellConfirm);
                st32(obj + 4, sell);
                st32(obj + 8, close);
                st32(obj + 12, me);
                st32(obj + 16, me);
                st32(obj + 20, close);
            }
            st32(fo + 16, obj);
            call(fn::Widget_addTouchEventListener, {sell, fo});
            functionDestroy(fo);
        }
        titleLabel(sell, S_TxtSell, 8, TXT_SELL);
        addTouch(close, VT_CloseButton, {});
        titleLabel(close, S_TxtCancel, 0xA, TXT_CANCEL);
        {
            const uint32_t text = findNode(popup, S_TxtSell, 8);
            const uint32_t name = newStr(f, S_TxtSell, 8);
            const uint32_t view = call(g::SceneUtil_textToCustomTextView, {text, name});
            strFree(name);
            const uint32_t out = f.alloc(12);
            call(g::TextData_format, {out, 0, uint32_t(TXT_SELL_ASK), ld32(me + F_SELECTED + 0xC)});
            const uint32_t s = newStr(f, strData(out));
            call(g::CustomTextView_setXml, {view, s});
            strFree(s);
            strFree(out);
        }
        titleLabel(popup, S_TxtSellTotal, 0xD, TXT_TOTAL);
        {
            const uint32_t label = findNode(popup, S_TxtSellTotalLabel, 0x13);
            SceneUtil::setShadow(label);
            setMoney(label, int32_t(ld32(me + F_SUM_MONEY)));
        }
        titleLabel(popup, S_TxtRare1, 9, TXT_RARE1);
        titleLabel(popup, S_TxtRare2, 9, TXT_RARE2);
        setVisible(findNode(popup, S_WarningPlate, 0xD), ld8(me + F_RARE));
        {
            const uint32_t none = newEmptyFunction(f);
            call(g::Node_runAction, {window, SceneUtil::createOpenPopupAnimation(window, none)});
            functionDestroy(none);
        }
        // the close handler, kept
        {
            const uint32_t closed = f.alloc(24);
            popupClosedFunction(closed, sell, close, window, layer, shadow, popup, me);
            const uint32_t end = ld32(me + F_POPUP_CLOSES + 4);
            if (end >= ld32(me + F_POPUP_CLOSES + 8)) {
                call(g::FunctionVBVector_pushSlow, {me + F_POPUP_CLOSES, closed});
            } else {
                uint32_t n = 0;
                if (end) {
                    call(g::FunctionVB_move, {end, closed});
                    n = ld32(me + F_POPUP_CLOSES + 4);
                }
                st32(me + F_POPUP_CLOSES + 4, n + 24);
            }
            functionDestroy(closed);
        }
        {
            const uint32_t sm = sceneManager();
            const uint32_t canClose = newFunction(f, VT_CanClose, {close});
            const uint32_t closed = f.alloc(24);
            popupClosedFunction(closed, sell, close, window, layer, shadow, popup, me);
            call(g::SM_openPopup, {sm, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(canClose);
        }
    }

    // sub_65D71C: a std::function<void(bool)> at `out` holding {sell, close, window, layer, shadow, popup, this}
    static uint32_t popupClosedFunction(uint32_t out, uint32_t sell, uint32_t close, uint32_t window, uint32_t layer,
                                        uint32_t shadow, uint32_t popup, uint32_t me) {
        st32(out + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x20});
        if (obj) {
            st32(obj, VT_PopupClosed);
            st32(obj + 4, sell);
            st32(obj + 8, close);
            st32(obj + 12, window);
            st32(obj + 16, layer);
            st32(obj + 20, shadow);
            st32(obj + 24, popup);
            st32(obj + 28, me);
        }
        st32(out + 16, obj);
        return out;
    }

    // ------------------------------------------------------------------------------------------------ back
    // With an open popup: close it; otherwise (once) the list's callbacks dropped, the "new" marks cleared and
    // the scene runs out (then the back lambda).
    static void backScene(uint32_t me) {
        if (!ld8(me + F_CAN_BACK)) return;
        if (call(g::SM_isOpenedPopup, {sceneManager()}) == 1) {
            call(g::SM_closeCurrentPopup, {sceneManager(), 0});
            return;
        }
        st8(me + F_CAN_BACK, 0);
        st8(sceneManager() + SM_FLAG_308, 0);
        finalize(me);
        call(g::SceneUtil_clearMedalNewMark, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_Back, {me});
        call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
        functionDestroy(fo);
    }

    // The buttons and the list's taps off, its tap / long-press callbacks dropped.
    static uint32_t finalize(uint32_t me) {
        setTouchEnabled(ld32(me + F_SELL_BUTTON), 0);
        setTouchEnabled(ld32(me + F_SORT_BUTTON), 0);
        call(g::SellMedalScrollView_setEnabledTapItem, {ld32(me + F_SCROLL), 0});
        const uint32_t scroll = ld32(me + F_SCROLL);
        functionDestroy(scroll + SCROLL_ON_TAP);
        st32(scroll + SCROLL_ON_TAP + 16, 0);
        const uint32_t scroll2 = ld32(me + F_SCROLL);
        const uint32_t p = ld32(scroll2 + SCROLL_ON_LONG_PRESS + 16);
        uint32_t result = p;
        if (p == scroll2 + SCROLL_ON_LONG_PRESS) result = vcall(p, 0x10, {p});
        else if (p) result = vcall(p, 0x14, {p});
        st32(scroll2 + SCROLL_ON_LONG_PRESS + 16, 0);
        return result;
    }

    // backScene's lambda {this}: to the back scene - fusion / evolution (if the medal is still the user's; else
    // backScene2), MyPage, quests, raids, the medal list, or the shop (the default)
    static void backDone(uint32_t functor) {
        const uint32_t me = ld32(functor + 4);
        const uint32_t type = ld32(me + F_BACK);
        const auto medalKept = [&]() {
            const uint32_t medals = ld32(call(fn::APIManager_getInstance, {}) + API_USER_MEDALS);
            const uint32_t sm = sceneManager();
            Frame f;
            const uint32_t key = f.alloc(8);
            st32(key, ld32(sm + SM_MEDAL));
            st32(key + 4, ld32(sm + SM_MEDAL + 4));
            const uint32_t node = call(g::U64IntMap_find, {medals + 0x24, key});
            return node && int32_t(ld32(node + 16)) >= 0;
        };
        Frame f;
        switch (type) {
        case 1: {
            if (!medalKept()) {
                call(g::SceneSaleMedal_backScene2, {me});
                return;
            }
            const uint32_t back2 = ld32(me + F_BACK2);
            resetHeaderCallback();
            callWithEmpty(g::SceneMixedMedal_callPrepareAPI);
            const uint32_t fo = newFunction(f, VT_ToFusion, {back2});
            changeScene(fo);
            functionDestroy(fo);
            return;
        }
        case 2: {
            if (!medalKept()) {
                call(g::SceneSaleMedal_backScene2, {me});
                return;
            }
            const uint32_t hi = ld32(me + F_MIXED_ID + 4), lo = ld32(me + F_MIXED_ID), back2 = ld32(me + F_BACK2);
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x18});
            if (obj) {
                st32(obj, VT_ToEvolve);
                st32(obj + 8, back2);
                st32(obj + 12, 0);
                st32(obj + 16, lo);
                st32(obj + 20, hi);
            }
            st32(fo + 16, obj);
            changeScene(fo);
            functionDestroy(fo);
            return;
        }
        case 4:
            resetHeaderCallback();
            callWithEmpty(g::SceneUtil_callPrepareMypageAPIs);
            changeSceneTo(g::GOT_CREATE_MYPAGE);
            return;
        case 5:
            resetHeaderCallback();
            callWithEmpty(g::SceneAdventureSelect_callAdventureSelectAPI);
            changeSceneTo(g::GOT_CREATE_ADVENTURE);
            st8(sceneManager() + SM_FLAG_30A, 1);
            return;
        case 6:
            resetHeaderCallback();
            callWithEmpty(g::APIManager_executeRaidList, call(fn::APIManager_getInstance, {}));
            changeSceneTo(g::GOT_CREATE_RAID_LIST);
            return;
        case 7: {
            const uint32_t sm = sceneManager();
            st32(sm + SM_MEDAL + 4, 0);
            st32(sm + SM_MEDAL, 0);
            changeSceneTo(g::GOT_CREATE_MEDAL_LIST);
            return;
        }
        default:
            resetHeaderCallback();
            callWithEmpty(g::SceneShopTop_callShopAPI);
            changeSceneTo(g::GOT_CREATE_SHOP_TOP);
            return;
        }
    }
};

RW_FUNCTION(0x48A0EC, "SceneSaleMedal::create", SceneSaleMedal::create);
RW_FUNCTION(0x65B430, "SceneSaleMedal::~SceneSaleMedal", SceneSaleMedal::destruct);
RW_FUNCTION(0x65B500, "SceneSaleMedal::init", SceneSaleMedal::init);
RW_FUNCTION(0x65C374, "SceneSaleMedal::updateHaveMoneynPanel", SceneSaleMedal::updateHaveMoneynPanel);
RW_FUNCTION(0x65C460, "SceneSaleMedal::updateSellInfo", SceneSaleMedal::updateSellInfo);
RW_FUNCTION(0x65C704, "SceneSaleMedal::updateSellButton", SceneSaleMedal::updateSellButton);
RW_FUNCTION(0x65C7C8, "SceneSaleMedal::setBackScene", SceneSaleMedal::setBackScene);
RW_FUNCTION(0x65C968, "SceneSaleMedal::runInAnimation", SceneSaleMedal::runInAnimation);
RW_FUNCTION(0x65CC1C, "SceneSaleMedal::runOutAnimationMain", SceneSaleMedal::runOutAnimationMain);
RW_FUNCTION(0x65CEF0, "SceneSaleMedal::openSellPopup", SceneSaleMedal::openSellPopup);
RW_FUNCTION(0x65D71C, "SceneSaleMedal::openSellPopup close-handler builder (sub_65D71C)", SceneSaleMedal::popupClosedFunction);
RW_FUNCTION(0x65D928, "SceneSaleMedal::backScene", SceneSaleMedal::backScene);
RW_FUNCTION(0x65DA10, "SceneSaleMedal::finalize", SceneSaleMedal::finalize);
RW_FUNCTION(0x65DB00, "SceneSaleMedal::backScene::$_12 operator() (sub_65DB00)", SceneSaleMedal::backDone);
