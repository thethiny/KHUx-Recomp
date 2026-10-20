// Other > Album: SceneAlbum, the medal album. The header shows the medals collected / all and the completion rate;
// AlbumScrollView lists every collectable medal in album order (seven columns): owned ones with their image,
// the rest as "?" over the blank medal. Also the side menu's album button (sub_5AC894: request the album, then
// change scene through sub_5AAB40) and the album order sort (std::__sort over medal master ids, comparator
// sub_6CEFFC: the album number of the medal).
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"

#include <cstring>
#include <limits>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_init = 0x88A8A1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Size_ctor = 0x8DF027;                   // Size(float, float)
constexpr uint32_t Widget_create = 0x825875, Widget_clone = 0x8272B9;
constexpr uint32_t Text_create = 0x839DED, Text_setFontSize = 0x83A35D, Text_setString = 0x83A2DD;
constexpr uint32_t ImageView_loadTexture = 0x839731;       // (image, const string&, TextureResType)
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionBTE_swap = 0x483E4D;            // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVTE_swap = 0x4F3699;            // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVI_swap = 0x5428E1;             // std::function<void(int)>::swap
constexpr uint32_t FunctionLayer_copy = 0x5AAC65;          // std::function<Layer*()>(const function&)
constexpr uint32_t FunctionLayer_move = 0x5AAE3D;          // std::function<Layer*()>(function&&)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;             // vector::at failure (does not return)
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t VectorInt_dtor = 0x478961;              // __vector_base<int>::~__vector_base
constexpr uint32_t VectorInt_pushSlow = 0x47D709;          // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t VectorInt_pushSlowConst = 0x4D6CC1;     // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t VectorInt_assign = 0x47D5BD;            // vector<int>::assign(int*, int*)
constexpr uint32_t TreeInt_destroy = 0x48D9B5;             // __tree<int>::destroy(node)
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9, IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t BaseScrollViewInt_init = 0x4FA8A1, BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t AlbumScrollView_create = 0x50B7B9, AlbumScrollView_setEnabledTapItem = 0x50C235;
constexpr uint32_t AlbumScrollView_setAlbumItemIDs = 0x50C241;
constexpr uint32_t Medal_getMedalIds = 0x785855;           // master::Medal::getMedalIds() -> std::set<int>
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int master id, UserMedal::Medal)
constexpr uint32_t MedalDataManager_getMedalSmallImagePath = 0x76B17D;   // (mgr, shared_ptr<UserMedalData>) -> char*
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B;     // (sm, std::function<void()> by value)
constexpr uint32_t SM_setBackButtonType = 0x613E27;
constexpr uint32_t SM_showHeader = 0x6131ED;
constexpr uint32_t SM_hideTitleHeader = 0x6130B1;          // (sm, function<void()>)
constexpr uint32_t SM_changeBackground = 0x6128ED;         // (sm, BackGroundType, const string&)
constexpr uint32_t SM_changeTickerMessage = 0x610B39;      // (sm, const string&)
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_canFinishScene = 0x6214F5;
constexpr uint32_t SM_callCurrentSceneRunOutAnimation = 0x62186D;   // (sm, function<void()>)
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;        // (sm, bool)
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, std::function<void()> by value)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t APIManager_executeUserAlbumGet = 0x4C7C75;    // (api, function<void(bool)>)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;   // (function<void(bool)> by value)
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;       // (node, const string& name, text id)
constexpr uint32_t SceneAlbum_menuChange = 0x5AAB41;             // sub_5AAB40
constexpr uint32_t Sort = 0x6CEC8D, Compare = 0x6CEFFD, Sort3 = 0x6CF159, Sort4 = 0x6CF1F5, Sort5 = 0x6CF26D;
constexpr uint32_t InsertionSortIncomplete = 0x6CF305;
// GOT: the class vtables, the __func vtable of a std::function<Layer*()> holding a function pointer, and
// createSceneLayer<SceneMyPage> / <SceneAlbum>
constexpr uint32_t GOT_VT_AlbumScrollView = 0xFD65CC, GOT_VT_BaseScrollView_int = 0xFD6598;
constexpr uint32_t GOT_VT_SceneAlbum = 0xFD66F0;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_MYPAGE = 0xFD6078, GOT_CREATE_ALBUM = 0xFD66EC;
}  // namespace g

// ------------------------------------------------------------------------------------------------ AlbumScrollView
constexpr uint32_t SCROLL_SIZE = 0x680;
constexpr uint32_t VT2_OFFSET = 0x360, VT3_OFFSET = 0x374;
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518;
constexpr uint32_t F_ITEM_IDS = 0x628;        // BaseScrollView<int> item ids (indices into the album list)
constexpr uint32_t F_ON_TAP = 0x638;          // std::function<void(int)>
constexpr uint32_t F_PANEL = 0x650;           // the cell template (Album_Medal.json)
constexpr uint32_t F_OWNED = 0x654;           // vector<int>: the user's album (medal ids)
constexpr uint32_t F_MEDALS = 0x660;          // vector<int>: medal master ids in album order
constexpr uint32_t F_TAP = 0x66C;             // cells take touches
// lambda vtables (+8)
constexpr uint32_t VT_TouchBegan = 0xF56234;  // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_TouchMoved = 0xF56208;  // $_1 {this, index, this, index, this, index} (heap 0x1C)
constexpr uint32_t VT_TouchEnded = 0xF561B0;  // $_2 {this, index, this, index} (heap 0x14)
// strings
constexpr uint32_t S_AlbumMedalJson = 0xDFD5A8;   // "cocostudio/publish/Album_Medal.json"
constexpr uint32_t S_TxtMedalLabel = 0xDFD5CC, S_TxtMedalNumLabel = 0xDFD5DC, S_MedalS = 0xDF8F79;
constexpr uint32_t S_FmtNo = 0xDFD5F0;            // "%s%d"
constexpr uint32_t S_Question = 0xDFD5F5;         // "?"
constexpr uint32_t S_BlankMedal = 0xDFD5F7;       // "img/medal/Medal_S_0.png"
constexpr int32_t TXT_NO = 103600003;
constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_changeRow = 0x340, V_setName = 0x154;
constexpr uint32_t V_setTouchEnabled = 0x29C;

// master::Medal row (SafeBuffer behind UserMedalData +0x58): +0 id, +0xC album number, +0x4B8 not in the album
constexpr uint32_t MEDAL_ROW = 0x58, ROW_ID = 0, ROW_NO = 0xC, ROW_NOT_IN_ALBUM = 0x4B8;

// ------------------------------------------------------------------------------------------------ SceneAlbum
constexpr uint32_t SIZE = 0x2A0;
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228;   // Layer std::function<void()> members
constexpr uint32_t F_INOUT = 0x288;           // IInOutAnimation (second vtable)
constexpr uint32_t F_ROOT = 0x28C, F_CENTER = 0x290, F_SCROLL = 0x294;
constexpr uint32_t F_CAN_BACK = 0x29C;
constexpr uint32_t VT_OnEnter = 0xF67860;     // init {this}
constexpr uint32_t VT_OnExit = 0xF67834;      // init {this}
constexpr uint32_t VT_BackKey = 0xF67808;     // init {this}
constexpr uint32_t VT_MedalTap = 0xF677DC;    // init {this} void(int)
constexpr uint32_t VT_RunOutDone = 0xF677B0;  // runOutAnimationMain {std::function<void()>} (heap, 0x20)
constexpr uint32_t VT_Back = 0xF6772C;        // backScene {}: sub_6CE6E4
constexpr uint32_t VT_MenuChange = 0xF5BFB4;  // sub_5AAB40 {std::function<Layer*()>} (heap, 0x20)
constexpr uint32_t S_SceneJson = 0xE059D6;    // "cocostudio/publish/AlbumScene_ver130.json"
constexpr uint32_t S_Bg = 0xE03AF2;           // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_CenterUI = 0xE02FBA, S_WinBottom = 0xE058DC, S_ScrollArea = 0xDF9B59;
constexpr uint32_t S_TxtMedalGet = 0xDF9A4C, S_TxtMedalGetNum = 0xDF9A83, S_TxtMedalGetSlash = 0xDF9A70;
constexpr uint32_t S_TxtMedalGetAll = 0xDF9A59, S_TxtMedalComp = 0xE05A00, S_TxtMedalCompNum = 0xE05A0E;
constexpr uint32_t S_TxtMedalCompRate = 0xE05A26, S_TxtChallenge = 0xE05A3E, S_ButtonChallenge = 0xE05A4C;
constexpr uint32_t S_FmtInt = 0xDF86DE;       // "%d"
constexpr uint32_t S_FmtRate = 0xE05A39;      // "%.1f"
constexpr int32_t TXT_TICKER = 100620026, TXT_GET = 103600001, TXT_COMP = 103600002, TXT_RATE = 100450003;
constexpr int32_t TXT_CHALLENGE = 103600004;
constexpr uint32_t API_USER_ALBUM = 0xF4;     // shared_ptr<SafeBuffer>: owned medal ids; +0x20 all, +0x24 count
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548;
constexpr uint32_t V_scene_init = 0x248, V_runInAnimation = 0x300;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
void sbRead(uint32_t sb, uint32_t dst, uint32_t offset) {
    rw::call(g::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, 4, offset});
}
// MedalDataManager::getMedalData(out, manager, 0, master id, UserMedal::Medal{})
uint32_t medalData(Frame &f, uint32_t masterId) {
    const uint32_t mgr = call(g::MedalDataManager_getInstance, {});
    const uint32_t out = f.alloc(8);
    call(g::MedalDataManager_getMedalData, {out, mgr, 0, 0, masterId, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0});
    return out;
}
// the 4-byte field at `offset` of the master row of a UserMedalData shared_ptr
uint32_t medalRowInt(uint32_t medal, uint32_t offset) {
    Frame f;
    const uint32_t v = f.alloc(4);
    sbRead(ld32(ld32(medal) + MEDAL_ROW), v, offset);
    return ld32(v);
}
// vector<int>::push_back(value): `less` = the end < capacity test of push_back(T&&) (else end != capacity)
void pushInt(uint32_t vec, uint32_t valueAddr, bool rvalue) {
    const uint32_t end = ld32(vec + 4);
    if (rvalue ? end >= ld32(vec + 8) : end == ld32(vec + 8)) {
        call(rvalue ? g::VectorInt_pushSlow : g::VectorInt_pushSlowConst, {vec, valueAddr});
        return;
    }
    uint32_t n = 0;
    if (end) {
        st32(end, ld32(valueAddr));
        n = ld32(vec + 4);
    }
    st32(vec + 4, n + 4);
}
// node->runAction(EaseSineOut(make(node, 0, 0, done)))
using MakeAnim = SceneUtil::Addr (*)(SceneUtil::Addr, float, float, SceneUtil::Addr);
void runEased(MakeAnim make, uint32_t node, uint32_t done) {
    const uint32_t action = make(node, 0.0f, 0.0f, done);
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
inline void swapI(uint32_t a, uint32_t b) {
    const uint32_t t = ld32(a);
    st32(a, ld32(b));
    st32(b, t);
}
inline bool less(uint32_t a, uint32_t b) { return call(g::Compare, {a, b}) == 1; }
}  // namespace

struct AlbumScrollView {
    static uint32_t create();
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static uint32_t getLayoutPanel(uint32_t self, uint32_t);
    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t arg);
    static uint32_t changeRow(uint32_t self, uint32_t widget, uint32_t index, uint32_t arg);
    static uint32_t registItemTouchEvent(uint32_t self, uint32_t widget, uint32_t index);
    static uint32_t assignTouchEnded(uint32_t dst, uint32_t lambda);
};

struct SceneAlbum {
    static uint32_t create();
    static void destroyDelete(uint32_t self);
    static uint32_t init(uint32_t self);
    static void runInAnimation(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static void albumTouched(uint32_t functor, uint32_t sender, uint32_t type);
    static void menuChange(uint32_t layer);
    static uint32_t compare(uint32_t a, uint32_t b);
    static uint32_t sort3(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t insertionSortIncomplete(uint32_t first, uint32_t last);
    static void sort(uint32_t first, uint32_t last);
};

// ================================================================================================ AlbumScrollView
uint32_t AlbumScrollView::create() {
    const uint32_t me = call(fn::operator_new, {SCROLL_SIZE});
    construct(me);
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t AlbumScrollView::construct(uint32_t s) {
    auto *base = rw::host_ptr<BaseScrollView>(s);
    call(g::Layout_ctor, {s});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    const uint32_t vt = ld32(g::GOT_VT_AlbumScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    st32(s + F_ON_TAP + 0x10, 0);
    st32(s + 0x670, 0);
    st8(s + 0x674, 0);
    st32(s + 0x678, 0);
    call(g::aeabi_memset, {s + F_OWNED, 0x1A, 0});
    return s;
}

uint32_t AlbumScrollView::destruct(uint32_t s) {
    const uint32_t vt = ld32(g::GOT_VT_AlbumScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    const uint32_t panel = ld32(s + F_PANEL);
    if (panel) vcall(panel, V_deleting, {panel});
    st32(s + F_PANEL, 0);
    call(g::VectorInt_dtor, {s + F_MEDALS});
    call(g::VectorInt_dtor, {s + F_OWNED});
    functionDestroy(s + F_ON_TAP);
    const uint32_t bvt = ld32(g::GOT_VT_BaseScrollView_int);
    st32(s, bvt + 8);
    st32(s + 0x280, bvt + VT2_OFFSET);
    st32(s + 0x350, bvt + VT3_OFFSET);
    call(g::VectorInt_dtor, {s + F_ITEM_IDS});
    call(g::IScrollProtocol_dtor, {s});
    return s;
}

// Cells 126x148 with the scroll bar; the cell template gets a hidden-by-default "Txt_Medal_Label" text (size 30).
uint32_t AlbumScrollView::init(uint32_t s) {
    stf(s + F_ITEM_W, 126.0f);
    stf(s + F_ITEM_H, 148.0f);
    call(g::IScrollProtocol_setEnabledScrollBar, {s, 1});
    stf(s + 0x570, 30.0f);
    stf(s + 0x574, -64.0f);
    stf(s + 0x57C, 74.0f);
    const uint32_t panel =
        call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_AlbumMedalJson});
    st32(s + F_PANEL, panel);
    call(g::Ref_retain, {panel});
    vcall(ld32(s + F_PANEL), V_setTouchEnabled, {ld32(s + F_PANEL), 0});
    const uint32_t text = call(g::Text_create, {});
    call(g::Text_setFontSize, {text, 30});
    {
        Frame f;
        const uint32_t name = newStr(f, S_TxtMedalLabel, 0xF);
        vcall(text, V_setName, {text, name});
        strFree(name);
    }
    vcall(ld32(s + F_PANEL), node::addChild, {ld32(s + F_PANEL), text});
    return call(g::BaseScrollViewInt_init, {s});
}

// A cell: an empty widget of the cell size holding a clone of the template.
uint32_t AlbumScrollView::getLayoutPanel(uint32_t s, uint32_t) {
    const uint32_t panel = call(g::Widget_create, {});
    const uint32_t cell = call(g::Widget_clone, {ld32(s + F_PANEL)});
    vcall(panel, node::setAnchorPoint, {panel, VEC2_ANCHOR_MIDDLE});
    vcall(panel, node::addChild, {panel, cell});
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, ld32(s + F_ITEM_W), ld32(s + F_ITEM_H)});
        vcall(panel, node::setContentSize, {panel, size});
    }
    vcall(panel, node::setPositionX, {panel, fbits(ldf(s + F_ITEM_W) * 0.5f)});
    return panel;
}

uint32_t AlbumScrollView::createRow(uint32_t s, uint32_t index, uint32_t arg) {
    const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 2);
    if (count <= index) {
        call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint32_t panel = getLayoutPanel(s, s + F_ITEM_IDS);
    vcall(s, V_changeRow, {s, panel, index, arg});
    return panel;
}

// Fill a cell with album entry `index`: its number ("No." + album number) and, when the user has the medal, its
// image; otherwise "?" over the blank medal.
uint32_t AlbumScrollView::changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
    const uint32_t ids = ld32(s + F_ITEM_IDS);
    if (!(uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ids) >> 2) > index)) {
        call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint32_t medalS = findNode(widget, S_MedalS, 7);
    if (!medalS) return 1;
    const uint32_t entry = ld32(ids + 4 * index);
    const uint32_t medals = ld32(s + F_MEDALS);
    if (!(uint32_t(int32_t(ld32(s + F_MEDALS + 4) - medals) >> 2) > entry)) {
        call(g::ThrowOutOfRange, {s + F_MEDALS});
        return 0;
    }
    Frame f;
    const uint32_t medal = medalData(f, ld32(medals + 4 * entry));
    {
        const uint32_t label = findNode(widget, S_TxtMedalNumLabel, 0x13);
        SceneUtil::setShadow(label);
        const uint32_t td = textData(f, TXT_NO);
        const uint32_t prefix = textDataStr(td);
        const uint32_t no = medalRowInt(medal, ROW_NO);
        Frame b;
        setTextC(label, stringBuffer(b, {S_FmtNo, prefix, no}));
        sharedRelease(td);
    }
    bool owned = false;
    const uint32_t ownedBegin = ld32(s + F_OWNED);
    if (ld32(s + F_OWNED + 4) != ownedBegin) {
        for (uint32_t i = 0;;) {
            const uint32_t id = ld32(ld32(s + F_OWNED) + 4 * i);
            const uint32_t rowId = medalRowInt(medal, ROW_ID);
            i++;
            if (id == rowId) {
                owned = true;
                break;
            }
            if (!(i < uint32_t(int32_t(ld32(s + F_OWNED + 4) - ld32(s + F_OWNED)) >> 2))) break;
        }
    }
    const uint32_t label = findNode(widget, S_TxtMedalLabel, 0xF);
    setVisible(label, owned ? 0 : 1);
    if (owned) {
        const uint32_t mgr = call(g::MedalDataManager_getInstance, {});
        const uint32_t copy = f.alloc(8);
        st32(copy, ld32(medal));
        st32(copy + 4, ld32(medal + 4));
        addRefIf(ld32(copy + 4));
        const uint32_t path = call(g::MedalDataManager_getMedalSmallImagePath, {mgr, copy});
        {
            Frame t;
            const uint32_t str = newStr(t, path);
            call(g::ImageView_loadTexture, {medalS, str, 0});
            strFree(str);
        }
        releaseIf(ld32(copy + 4));
    } else {
        {
            Frame t;
            const uint32_t str = newStr(t, S_Question, 1);
            call(g::Text_setString, {label, str});
            strFree(str);
        }
        vcall(label, node::setPosition, {label, vcall(medalS, node::getPosition, {medalS})});
        Frame t;
        const uint32_t str = newStr(t, S_BlankMedal, 0x17);
        call(g::ImageView_loadTexture, {medalS, str, 0});
        strFree(str);
    }
    releaseIf(ld32(medal + 4));
    return 0;
}

// Touch listener for a cell: began / moved / ended / cancelled lambdas capturing (this, index).
uint32_t AlbumScrollView::registItemTouchEvent(uint32_t s, uint32_t widget, uint32_t index) {
    if (!ld8(s + F_TAP)) return 0;
    const uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    const uint32_t began = newFunction(f, VT_TouchBegan, {s, index});
    call(g::FunctionBTE_swap, {began, listener + 0x50});
    functionDestroy(began);
    const uint32_t captures = f.alloc(16);
    st32(captures, s);
    st32(captures + 4, index);
    st32(captures + 8, s);
    st32(captures + 12, index);
    const uint32_t moved = f.alloc(20);
    st32(moved + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, VT_TouchMoved);
        for (uint32_t w = 4; w < 0x1C; w += 8) {
            st32(obj + w, s);
            st32(obj + w + 4, index);
        }
    }
    st32(moved + 16, obj);
    call(g::FunctionVTE_swap, {moved, listener + 0x68});
    functionDestroy(moved);
    assignTouchEnded(listener + 0x80, captures);
    assignTouchEnded(listener + 0x98, captures);
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, widget});
    return 0;
}

// std::function<void(Touch*, Event*)>::operator=($_2&&): a heap __func with the four captures, swapped in
uint32_t AlbumScrollView::assignTouchEnded(uint32_t dst, uint32_t lambda) {
    Frame f;
    const uint32_t tmp = f.alloc(20);
    st32(tmp + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, VT_TouchEnded);
        for (uint32_t w = 0; w < 16; w += 4) st32(obj + 4 + w, ld32(lambda + w));
    }
    st32(tmp + 16, obj);
    call(g::FunctionVTE_swap, {tmp, dst});
    functionDestroy(tmp);
    return 0;
}

// ================================================================================================ SceneAlbum
uint32_t SceneAlbum::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    const uint32_t vt = ld32(g::GOT_VT_SceneAlbum);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    call(g::aeabi_memset, {me + F_ROOT, 0x11, 0});
    if (vcall(me, V_scene_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

void SceneAlbum::destroyDelete(uint32_t self) {
    const uint32_t vt = ld32(g::GOT_VT_SceneAlbum);
    st32(self, vt + 8);
    st32(self + F_INOUT, vt + 0x318);
    call(g::Layer_dtor, {self});
    call(fn::operator_delete, {self});
}

uint32_t SceneAlbum::init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    Frame f;
    for (const auto &[member, vtable] : {std::pair{F_ON_ENTER, VT_OnEnter}, std::pair{F_ON_EXIT, VT_OnExit}}) {
        const uint32_t fo = newFunction(f, vtable, {me});
        call(g::FunctionVV_assign, {me + member, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t m = sceneManager();
        const uint32_t fo = newFunction(f, VT_BackKey, {me});
        call(g::SM_setOnBackKeyCallback, {m, fo});
        functionDestroy(fo);
    }
    st8(me + F_CAN_BACK, 1);
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
        const uint32_t none = newEmptyFunction(f);
        call(g::SM_hideTitleHeader, {m, none});
        functionDestroy(none);
    }
    {
        const uint32_t s = newStr(f, S_SceneJson, 0x29);
        st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
        strFree(s);
    }
    vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
    st32(me + F_CENTER, findNode(ld32(me + F_ROOT), S_CenterUI, 8));
    {
        const uint32_t bottom = findNode(ld32(me + F_CENTER), S_WinBottom, 0xA);
        if (bottom) vcall(bottom, V_setTouchEnabled, {bottom, 1});
    }
    for (const auto &[name, len] : {std::pair{S_TxtMedalGet, 0xCu}, std::pair{S_TxtMedalGetNum, 0x16u},
                                    std::pair{S_TxtMedalGetSlash, 0x12u}, std::pair{S_TxtMedalGetAll, 0x16u},
                                    std::pair{S_TxtMedalComp, 0xDu}, std::pair{S_TxtMedalCompNum, 0x17u},
                                    std::pair{S_TxtMedalCompRate, 0x12u}})
        SceneUtil::setShadow(findNode(ld32(me + F_ROOT), name, len));

    // the counts: medals collected / all, and the completion rate
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t albumCtl = ld32(api + API_USER_ALBUM + 4), album = ld32(api + API_USER_ALBUM);
    addRefIf(albumCtl);
    {
        const uint32_t get = findNode(ld32(me + F_ROOT), S_TxtMedalGet, 0xC);
        const uint32_t num = findNode(ld32(me + F_ROOT), S_TxtMedalGetNum, 0x16);
        const uint32_t all = findNode(ld32(me + F_ROOT), S_TxtMedalGetAll, 0x16);
        setTextId(get, TXT_GET);
        {
            Frame b;
            setTextC(num, stringBuffer(b, {S_FmtInt, ld32(album + 0x24)}));
        }
        {
            Frame b;
            setTextC(all, stringBuffer(b, {S_FmtInt, ld32(album + 0x20)}));
        }
        SceneUtil::setShadow(num);
        SceneUtil::setShadow(all);
    }
    {
        const uint32_t comp = findNode(ld32(me + F_ROOT), S_TxtMedalComp, 0xD);
        const uint32_t num = findNode(ld32(me + F_ROOT), S_TxtMedalCompNum, 0x17);
        setTextId(comp, TXT_COMP);
        SceneUtil::setShadow(comp);
        float rate = float(ld32(album + 0x24)) / float(int32_t(ld32(album + 0x20))) * 100.0f;
        if (rate != rate) rate = std::numeric_limits<float>::quiet_NaN();   // the soft-float NaN is positive
        uint64_t bits;
        const double d = rate;
        std::memcpy(&bits, &d, 8);
        {
            Frame b;
            setTextC(num, stringBuffer(b, {S_FmtRate, uint32_t(bits), uint32_t(bits >> 32)}));
        }
        SceneUtil::setShadow(num);
    }
    {
        const uint32_t s = newStr(f, S_TxtMedalCompRate, 0x12);
        call(g::SceneUtil_settingTitleLabel, {ld32(me + F_ROOT), s, uint32_t(TXT_RATE)});
        strFree(s);
    }
    const uint32_t area = findNode(ld32(me + F_ROOT), S_ScrollArea, 0xB);

    // the album: the medals in the album (not flagged out of it, id 1 excluded), sorted by album number
    const uint32_t items = f.alloc(12), medals = f.alloc(12), owned = f.alloc(12);
    for (uint32_t v : {items, medals, owned}) {
        st32(v, 0);
        st32(v + 4, 0);
        st32(v + 8, 0);
    }
    const uint32_t set = f.alloc(12);
    call(g::Medal_getMedalIds, {set});
    const uint32_t value = f.alloc(4);
    uint32_t count = 0;
    for (uint32_t node = ld32(set); node != set + 4;) {
        const uint32_t key = node + 0x10;
        {
            Frame m;
            const uint32_t medal = medalData(m, ld32(key));
            if (ld32(key) != 1 && medalRowInt(medal, ROW_NOT_IN_ALBUM) == 0) {
                st32(value, count);
                pushInt(items, value, true);
                count++;
                pushInt(medals, key, false);
            }
            releaseIf(ld32(medal + 4));
        }
        // in-order successor
        if (ld32(node + 4)) {
            node = ld32(node + 4);
            while (ld32(node)) node = ld32(node);
        } else {
            uint32_t child;
            do {
                child = node;
                node = ld32(node + 8);
            } while (ld32(node) != child);
        }
    }
    call(g::Sort, {ld32(medals), ld32(medals + 4)});
    for (uint32_t i = 0, at = 0; i < ld32(album + 0x24); i++, at += 4) {
        sbRead(album, value, at);
        pushInt(owned, value, true);
    }

    st32(me + F_SCROLL, call(g::AlbumScrollView_create, {}));
    call(g::IScrollProtocol_setMaxColumn, {ld32(me + F_SCROLL), 7});
    call(g::AlbumScrollView_setEnabledTapItem, {ld32(me + F_SCROLL), 1});
    {
        const uint32_t scroll = ld32(me + F_SCROLL);
        vcall(scroll, node::setContentSize, {scroll, vcall(area, node::getContentSize, {area})});
    }
    if (ld32(me + F_SCROLL) + F_MEDALS != medals)
        call(g::VectorInt_assign, {ld32(me + F_SCROLL) + F_MEDALS, ld32(medals), ld32(medals + 4)});
    call(g::AlbumScrollView_setAlbumItemIDs, {ld32(me + F_SCROLL), owned});
    call(g::BaseScrollViewInt_setItemIDs, {ld32(me + F_SCROLL), items});
    {
        const uint32_t fo = newFunction(f, VT_MedalTap, {me});
        call(g::FunctionVI_swap, {fo, ld32(me + F_SCROLL) + F_ON_TAP});
        functionDestroy(fo);
    }
    vcall(area, node::addChild, {area, ld32(me + F_SCROLL)});
    {
        const uint32_t challenge = findNode(ld32(me + F_ROOT), S_TxtChallenge, 0xD);
        setTextId(challenge, TXT_CHALLENGE);
        SceneUtil::setShadow(challenge);
    }
    {
        const uint32_t button = findNode(ld32(me + F_ROOT), S_ButtonChallenge, 0x10);
        setVisible(button, 0);
        vcall(button, V_setTouchEnabled, {button, 0});
    }
    vcall(me, V_runInAnimation, {me});
    call(g::TreeInt_destroy, {set, ld32(set + 4)});
    call(g::VectorInt_dtor, {owned});
    call(g::VectorInt_dtor, {medals});
    call(g::VectorInt_dtor, {items});
    releaseIf(albumCtl);
    return 1;
}

// ------------------------------------------------------------------------------------------------ animations
void SceneAlbum::runInAnimation(uint32_t me) {
    const uint32_t center = ld32(me + F_CENTER);
    vcall(center, node::setLocalZOrder, {center, 2});
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    runEased(SceneUtil::createLeftInAnimation, ld32(me + F_CENTER), none);
    functionDestroy(none);
}

// Slide the centre panel out; `done` (std::function<void()>) runs when it is gone.
void SceneAlbum::runOutAnimationMain(uint32_t me, uint32_t done) {
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    const uint32_t center = ld32(me + F_CENTER);
    Frame f;
    const uint32_t captured = f.alloc(20);
    call(fn::function_vv_copy, {captured, done});
    const uint32_t onDone = f.alloc(20);
    heapFunction(onDone, VT_RunOutDone, 0x20, 8, fn::function_vv_move, captured);
    runEased(SceneUtil::createLeftOutAnimation, center, onDone);
    functionDestroy(onDone);
    functionDestroy(captured);
}

// ------------------------------------------------------------------------------------------------ back
void SceneAlbum::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    st8(me + F_CAN_BACK, 0);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Back, {});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// backScene's lambda: refresh the MyPage data and return there (fade).
void SceneAlbum::backDone(uint32_t) {
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

// ------------------------------------------------------------------------------------------------ menu
// The side menu's album button (touch ended): request the user's album, then change to the Album screen.
void SceneAlbum::albumTouched(uint32_t, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    call(g::AudioPlaySystemSEDecide, {});
    if (call(g::SM_canFinishScene, {sceneManager()}) != 1) return;
    Frame f;
    {
        const uint32_t m = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    }
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t none = newEmptyFunction(f);
        call(g::APIManager_executeUserAlbumGet, {api, none});
        functionDestroy(none);
    }
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(g::GOT_CREATE_ALBUM)});
    call(g::SceneAlbum_menuChange, {layer});
    functionDestroy(layer);
}

// The menu's scene change (sub_5AAB40): when the scene may finish, run the current scene out, then change to
// `layer`; close the open popup.
void SceneAlbum::menuChange(uint32_t layer) {
    if (call(g::SM_canFinishScene, {sceneManager()}) != 1) return;
    const uint32_t m = sceneManager();
    Frame f;
    const uint32_t copy = f.alloc(20), fo = f.alloc(20);
    call(g::FunctionLayer_copy, {copy, layer});
    heapFunction(fo, VT_MenuChange, 0x20, 8, g::FunctionLayer_move, copy);
    call(g::SM_callCurrentSceneRunOutAnimation, {m, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    call(g::SM_closeCurrentPopup, {sceneManager(), 0});
}

// ------------------------------------------------------------------------------------------------ album order
// The sort comparator (medal master ids by pointer): album number of a < album number of b.
uint32_t SceneAlbum::compare(uint32_t a, uint32_t b) {
    int32_t na, nb;
    {
        Frame f;
        const uint32_t medal = medalData(f, ld32(a));
        na = int32_t(medalRowInt(medal, ROW_NO));
        releaseIf(ld32(medal + 4));
    }
    {
        Frame f;
        const uint32_t medal = medalData(f, ld32(b));
        nb = int32_t(medalRowInt(medal, ROW_NO));
        releaseIf(ld32(medal + 4));
    }
    return na < nb ? 1 : 0;
}

// std::__sort3: returns the swaps made
uint32_t SceneAlbum::sort3(uint32_t x, uint32_t y, uint32_t z) {
    const bool yx = less(y, x);
    const bool zy = less(z, y);
    if (!yx) {
        if (!zy) return 0;
        swapI(y, z);
        if (!less(y, x)) return 1;
        swapI(x, y);
        return 2;
    }
    if (zy) {
        swapI(x, z);
        return 1;
    }
    swapI(x, y);
    if (!less(z, y)) return 1;
    swapI(y, z);
    return 2;
}

// std::__insertion_sort_incomplete: sorts [first, last) unless more than 8 elements move; true when sorted
uint32_t SceneAlbum::insertionSortIncomplete(uint32_t first, uint32_t last) {
    switch (int32_t(last - first) >> 2) {
    case 0:
    case 1:
        return 1;
    case 2:
        if (less(last - 4, first)) swapI(first, last - 4);
        return 1;
    case 3:
        call(g::Sort3, {first, first + 4, last - 4});
        return 1;
    case 4:
        call(g::Sort4, {first, first + 4, first + 8, last - 4});
        return 1;
    case 5:
        call(g::Sort5, {first, first + 4, first + 8, first + 12, last - 4});
        return 1;
    }
    call(g::Sort3, {first, first + 4, first + 8});
    Frame f;
    const uint32_t t = f.alloc(4);
    uint32_t moved = 0;
    for (uint32_t j = first + 8, i = j + 4; i != last; j = i, i += 4) {
        if (!less(i, j)) continue;
        st32(t, ld32(i));
        uint32_t k = j, hole = i;
        for (;;) {
            st32(hole, ld32(k));
            hole = k;
            if (hole == first) break;
            k -= 4;
            if (!less(t, k)) break;
        }
        st32(hole, ld32(t));
        if (++moved == 8) return i + 4 == last ? 1 : 0;
    }
    return 1;
}

// std::__sort (libc++ introsort without the depth limit): insertion sort up to 30 elements
void SceneAlbum::sort(uint32_t first, uint32_t last) {
    for (;;) {
    restart:
        const uint32_t len = uint32_t(int32_t(last - first) >> 2);
        const uint32_t lm1 = last - 4;
        switch (len) {
        case 0:
        case 1:
            return;
        case 2:
            if (less(lm1, first)) swapI(first, lm1);
            return;
        case 3:
            call(g::Sort3, {first, first + 4, lm1});
            return;
        case 4:
            call(g::Sort4, {first, first + 4, first + 8, lm1});
            return;
        case 5:
            call(g::Sort5, {first, first + 4, first + 8, first + 12, lm1});
            return;
        }
        if (len <= 30) {
            // __insertion_sort_3
            call(g::Sort3, {first, first + 4, first + 8});
            Frame f;
            const uint32_t t = f.alloc(4);
            for (uint32_t j = first + 8, i = j + 4; i != last; j = i, i += 4) {
                if (!less(i, j)) continue;
                st32(t, ld32(i));
                uint32_t k = j, hole = i;
                for (;;) {
                    st32(hole, ld32(k));
                    hole = k;
                    if (hole == first) break;
                    k -= 4;
                    if (!less(t, k)) break;
                }
                st32(hole, ld32(t));
            }
            return;
        }
        uint32_t m = first + 4 * (len / 2);
        uint32_t swaps;
        if (len >= 1000) {
            const uint32_t delta = len / 4;
            swaps = call(g::Sort5, {first, first + 4 * delta, m, m + 4 * delta, lm1});
        } else {
            swaps = call(g::Sort3, {first, m, lm1});
        }
        uint32_t i = first, j = lm1;
        if (!less(i, m)) {
            // *first == *m: guard the downward j against i
            bool found = false;
            while (j - 4 != i) {
                j -= 4;
                if (less(j, m)) {
                    swapI(i, j);
                    swaps++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                // *m <= all others: partition into [first, i) == *first and *first < [i, last)
                i = first + 4;
                j = lm1;
                if (!less(first, j)) {
                    for (;;) {
                        if (i == j) return;
                        if (less(first, i)) {
                            swapI(i, j);
                            swaps++;
                            i += 4;
                            break;
                        }
                        i += 4;
                    }
                }
                if (i == j) return;
                for (;;) {
                    while (!less(first, i)) i += 4;
                    do j -= 4;
                    while (less(first, j));
                    if (i >= j) break;
                    swapI(i, j);
                    swaps++;
                    i += 4;
                }
                first = i;
                goto restart;
            }
        }
        i += 4;
        if (i < j) {
            for (;;) {
                while (less(i, m)) i += 4;
                do j -= 4;
                while (!less(j, m));
                if (i > j) break;
                swapI(i, j);
                swaps++;
                if (m == i) m = j;
                i += 4;
            }
        }
        if (i != m && less(m, i)) {
            swapI(i, m);
            swaps++;
        }
        if (swaps == 0) {
            const bool fs = call(g::InsertionSortIncomplete, {first, i}) != 0;
            if (call(g::InsertionSortIncomplete, {i + 4, last}) == 1) {
                if (fs) return;
                last = i;
                continue;
            }
            if (fs) {
                first = i + 4;
                continue;
            }
        }
        if (int32_t(i - first) < int32_t(last - i)) {
            call(g::Sort, {first, i});
            first = i + 4;
        } else {
            call(g::Sort, {i + 4, last});
            last = i;
        }
    }
}

RW_FUNCTION(0x50B694, "AlbumScrollView::~AlbumScrollView", AlbumScrollView::destruct);
RW_FUNCTION(0x50B7B8, "AlbumScrollView::create", AlbumScrollView::create);
RW_FUNCTION(0x50B80C, "AlbumScrollView::AlbumScrollView", AlbumScrollView::construct);
RW_FUNCTION(0x50B968, "AlbumScrollView::init", AlbumScrollView::init);
RW_FUNCTION(0x50BAA4, "AlbumScrollView::getLayoutPanel", AlbumScrollView::getLayoutPanel);
RW_FUNCTION(0x50BB6C, "AlbumScrollView::createRow", AlbumScrollView::createRow);
RW_FUNCTION(0x50BBC4, "AlbumScrollView::changeRow", AlbumScrollView::changeRow);
RW_FUNCTION(0x50C048, "AlbumScrollView::registItemTouchEvent", AlbumScrollView::registItemTouchEvent);
RW_FUNCTION(0x50C1A0, "AlbumScrollView::registItemTouchEvent $_2 std::function::operator=(&&) (sub_50C1A0)",
            AlbumScrollView::assignTouchEnded);
RW_FUNCTION(0x5AC9FC, "SceneAlbum::create", SceneAlbum::create);
RW_FUNCTION(0x6CCD24, "SceneAlbum::~SceneAlbum (deleting)", SceneAlbum::destroyDelete);
RW_FUNCTION(0x6CCD90, "SceneAlbum::init", SceneAlbum::init);
RW_FUNCTION(0x6CDD40, "SceneAlbum::runInAnimation", SceneAlbum::runInAnimation);
RW_FUNCTION(0x6CDE04, "SceneAlbum::runOutAnimationMain", SceneAlbum::runOutAnimationMain);
RW_FUNCTION(0x6CE5E4, "SceneAlbum::backScene", SceneAlbum::backScene);
RW_FUNCTION(0x6CE6E4, "SceneAlbum::backScene lambda __func::operator() (sub_6CE6E4)", SceneAlbum::backDone);
RW_FUNCTION(0x5AC894, "side menu album button lambda __func::operator() (sub_5AC894)", SceneAlbum::albumTouched);
RW_FUNCTION(0x5AAB40, "side menu scene change (sub_5AAB40)", SceneAlbum::menuChange);
RW_FUNCTION(0x6CEFFC, "SceneAlbum::init album order comparator (sub_6CEFFC)", SceneAlbum::compare);
RW_FUNCTION(0x6CF158, "std::__sort3<album order comparator&, int*> (sub_6CF158)", SceneAlbum::sort3);
RW_FUNCTION(0x6CF304, "std::__insertion_sort_incomplete<album order comparator&, int*> (sub_6CF304)",
            SceneAlbum::insertionSortIncomplete);
RW_FUNCTION(0x6CEC8C, "std::__sort<album order comparator&, int*> (sub_6CEC8C)", SceneAlbum::sort);
