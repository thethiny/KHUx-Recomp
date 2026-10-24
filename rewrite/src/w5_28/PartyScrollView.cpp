// PartyScrollView: the party lists of the Party screens (a BaseScrollView<int> of indexes into an APIManager party
// vector: 0 the search result, 1 the user's requests, 2 the invitations to him). Rows are PartyTop_InitialPanel
// clones with an AvatarIconCache for the leaders' avatars; each row's buttons (Withdraw / Accept / Delete / Details)
// call one of the view's std::function<void(int)> callbacks with the row after the scene manager's no-tap delay;
// the leader's avatar button ($_0) opens his info popup.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t MEMCPY = 0x40E164;                      // __aeabi_memcpy(dst, src, n)
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                   // Size(float, float)
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t ExpressionVector_allocate = 0x487635;   // vector<Avatar::Expression>::allocate(n)
constexpr uint32_t ExpressionVector_destroy = 0x46BE0D;
constexpr uint32_t AvatarIconCache_ctor = 0x5CA741;        // (Texture2D*, const Vec2&, float, float, const vector<Expression>&)
constexpr uint32_t AvatarIconCache_dtor = 0x5CA8FD;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;             // vector::at failure (does not return)
constexpr uint32_t VectorInt_dtor = 0x478961;
constexpr uint32_t VectorULL_dtor = 0x468499;
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1, IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t BaseScrollViewInt_init = 0x4FA8A1;
constexpr uint32_t Party_copy = 0x4B2E63;                  // hole::network::api::Party::Party(const Party&)
constexpr uint32_t FunctionBTE_swap = 0x483E4D;            // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVTE_swap = 0x4F3699;            // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_openNoTapScreen = 0x61FB25;          // (sm, float seconds)
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create, CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t GOT_VT_PartyScrollView = 0xFD65EC, GOT_VT_BaseScrollView_int = 0xFD6598;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
}  // namespace g

constexpr uint32_t LIST_SIZE = 0x6E0;
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518, F_ITEM_IDS = 0x628;
constexpr uint32_t F_ROW = 0x6B0;              // the row of the last tapped button
constexpr uint32_t F_TYPE = 0x6C0;             // PartyScrollViewType (0 search, 1 requests, 2 invitations)
constexpr uint32_t F_PANEL = 0x6C4;            // the PartyTop_InitialPanel row template (retained)
constexpr uint32_t F_TOUCH = 0x6C8;            // rows take touches
constexpr uint32_t F_ICONS = 0x6DC;            // AvatarIconCache*
constexpr uint32_t SM_NO_TAP_TIME = 0x300;     // SceneManager: float, the no-tap screen's seconds
constexpr uint32_t VT_RowTouchBegan = 0xF56ACC;   // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_RowTouchMoved = 0xF56AA0;   // $_1 {this, index} x 3 (heap 0x1C)
constexpr uint32_t VT_RowTouchEnded = 0xF56A48;   // $_2 {this, index, this, index} (heap 0x14)
constexpr uint32_t VT_RowParty = 0xF56C58;        // changeRow $_0 {-, -, words, Party, 0x128 bytes} (heap 0x1A8)
constexpr uint32_t S_InitialPanelJson = 0xDFD9B8; // "cocostudio/publish/PartyTop_InitialPanel.json"
constexpr uint32_t S_Avatar = 0xDF8F56;
constexpr uint32_t AVATAR_ICON_CACHE_SIZE = 0x60;
constexpr uint32_t V_deleting = 0x04, V_listInit = 0x350, V_changeRow = 0x340, V_widgetSetTouchEnabled = 0x29C;
constexpr uint32_t V_getVirtualRenderer = 0x2D8, V_getTexture = 0x280;

// The row buttons: the callback (std::function<void(int)>) and the delayed call's __func vtable
struct RowButton {
    uint32_t callback, vtDelayed;
};
constexpr RowButton BUTTON_5{0x698, 0xF56B24}, BUTTON_4{0x680, 0xF56B7C}, BUTTON_3{0x668, 0xF56BD4};
constexpr RowButton BUTTON_2{0x650, 0xF56C2C};

// changeRow
constexpr uint32_t STRING_ASSIGN = 0x498A1D;               // std::string::operator=(const string&)
constexpr uint32_t STRING_ASSIGN_N = 0x48D811;             // std::string::assign(const char*, size_t)
constexpr uint32_t VECTOR_ULL_ASSIGN = 0x4D7461;           // vector<ull>::assign(first, last)
constexpr uint32_t VECTOR_PARTY_EX_COPY = 0x51EE91, VECTOR_PARTY_EX_DTOR = 0x4B3DD1;
constexpr uint32_t DATE_MAKE_LOCAL = 0x4927CD, TEXT_DATA_FORMAT = 0x492929;
constexpr uint32_t ICON_CACHE_FIND = 0x5CA9A9, ICON_CACHE_ADD = 0x5CAA57;
constexpr uint32_t AVATAR_MAKE_STYLE = 0x497105, SAFE_BUFFER_DTOR = 0x45A13D;
constexpr uint32_t IMAGE_BUTTON_CREATE = 0x531239, IMAGE_BUTTON_SET_TARGET_VIEW = 0x5318C1;
constexpr uint32_t IMAGE_BUTTON_SET_TARGET_IMAGE_VIEW = 0x531631, IMAGE_BUTTON_ADD_TOUCH = 0x5315B1;
constexpr uint32_t RANK_IMAGE_PATH = 0x46F08D, IMAGE_VIEW_LOAD_TEXTURE = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t OPEN_AVATAR_INFO_POPUP = 0x46A229;      // (ull user, bool, function<void(bool)>, function<bool()>)
constexpr uint32_t TOUCH_GET_LOCATION = rwlib::cocos2d::Touch::getLocation, NODE_CONVERT_TO_NODE_SPACE = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t RECT_CTOR = rwlib::cocos2d::Rect::ctor_base__ffff, RECT_CONTAINS = rwlib::cocos2d::Rect::containsPoint, TAP_ANIMATION = 0x45BBA9;
constexpr uint32_t GOT_ROW_SPACING = 0xFD659C;
constexpr uint32_t API_PARTIES = 0x204, API_REQUESTS = 0x218, API_INVITES = 0x238;
constexpr uint32_t PARTY = 0x70, PARTY_EX = 0x198, RECORD = 0x38;
constexpr uint32_t F_AVATAR_USER = 0x6B8, F_TOUCH_INSIDE = 0x6C9, F_TAP_INDEX = 0x6CC, F_TAP_RUNNING = 0x6D0;
constexpr uint32_t F_TOUCH_Y = 0x6D4, F_AVATAR_OPENING = 0x6D8;
constexpr uint32_t VT_Button2 = 0xF56C00, VT_Button3 = 0xF56BA8, VT_Button4 = 0xF56B50, VT_Button5 = 0xF56AF8;
constexpr uint32_t VT_TapDone = 0xF56A74, VT_AvatarDelayed = 0xF56C84, VT_AvatarClosed = 0xF56CB0;
constexpr uint32_t V_GET_LOCAL_Z = 0x18;
constexpr uint32_t S_PlateInitial = 0xDFD9E6, S_AvatarIconObj = 0xDF8F5D, S_ImageButtonAvatar = 0xDFD9F4;
constexpr uint32_t S_RankIcon = 0xDFDA07, S_TxtName = 0xDFDA10, S_TxtMessageLabel = 0xDFDA19;
constexpr uint32_t S_PartyInfo1Label = 0xDFDA2B, S_PartyInfo1 = 0xDFDA41, S_PartyInfo2Label = 0xDFDA51;
constexpr uint32_t S_PartyInfo2 = 0xDFDA67, S_PartyInfo3Label = 0xDFDA77, S_PartyInfo3 = 0xDFDA8D;
constexpr uint32_t S_RequestDateLabel = 0xDFDA9D, S_ReceptionLabel = 0xDFDAB3, S_TwoStrings = 0xDFDAC7;
constexpr uint32_t S_IntFormat = 0xDF86DE, S_But01 = 0xDFD027, S_EMPTY = 0xDF8A1A;
constexpr uint32_t S_ButtonRequest = 0xDFDACD, S_ButtonCancelRequest = 0xDFDADC, S_ButtonApproval = 0xDFDAF1;
constexpr uint32_t S_ButtonDelete = 0xDFDB01, S_ImageButtonRequest = 0xDFDB0F, S_ImageButtonCancel = 0xDFDB23;
constexpr uint32_t S_ImageButtonApproval = 0xDFDB40, S_ImageButtonDelete = 0xDFDB55;
constexpr int32_t TXT_DATE = 103801063;                     // year, month, day, hour, minute, second
constexpr int32_t TXT_MEMBERS = 103800023, TXT_PLAY_STYLE = 102400003, TXT_JOIN_TYPE = 103800019;
constexpr int32_t TXT_REQUEST_DATE = 103800051, TXT_RECEPTION = 103800052;
constexpr int32_t TXT_BUTTON_REQUEST = 103800029, TXT_BUTTON_CANCEL = 103810024, TXT_BUTTON_APPROVE = 103800030;
constexpr int32_t TXT_BUTTON_DELETE = 103800031;
constexpr int32_t TXT_STYLES[3] = {102400004, 102400005, 102400006};
constexpr int32_t TXT_JOINS[3] = {103800049, 103800021, 103800020};
}  // namespace

struct PartyScrollView {
    static uint32_t create(uint32_t type) {
        const uint32_t me = call(fn::operator_new, {LIST_SIZE});
        construct(me);
        if (vcall(me, V_listInit, {me, type}) == 1) {
            call(g::Ref_autorelease, {me});
            return me;
        }
        vcall(me, V_deleting, {me});
        return 0;
    }

    static uint32_t construct(uint32_t s) {
        auto *base = rw::host_ptr<BaseScrollView>(s);
        call(g::Layout_ctor, {s});
        BaseScrollView::initScrollDefaults(base);
        base->itemIDs.begin = 0;
        base->itemIDs.end = 0;
        base->itemIDs.cap = 0;
        const uint32_t vt = ld32(g::GOT_VT_PartyScrollView);
        st32(s, vt + 8);
        st32(s + 0x280, vt + 0x364);
        st32(s + 0x350, vt + 0x378);
        for (uint32_t fo = 0x638; fo <= 0x698; fo += 0x18) st32(s + fo + 0x10, 0);   // five empty std::functions
        st32(s + F_ROW, 0);
        st32(s + 0x6BC, 0);
        st32(s + 0x6B8, 0);
        st8(s + F_TOUCH, 0);
        st8(s + 0x6C9, 0);
        st32(s + 0x6CC, 0);
        st8(s + 0x6D0, 0);
        st32(s + 0x6D4, 0);
        st8(s + 0x6D8, 0);
        st32(s + F_ICONS, 0);
        return s;
    }

    static uint32_t destruct(uint32_t s) {
        const uint32_t vt = ld32(g::GOT_VT_PartyScrollView);
        st32(s, vt + 8);
        st32(s + 0x280, vt + 0x364);
        st32(s + 0x350, vt + 0x378);
        if (const uint32_t panel = ld32(s + F_PANEL)) vcall(panel, V_deleting, {panel});
        st32(s + F_PANEL, 0);
        if (const uint32_t icons = ld32(s + F_ICONS)) {
            call(g::AvatarIconCache_dtor, {icons});
            call(fn::operator_delete, {icons});
        }
        st32(s + F_ICONS, 0);
        for (uint32_t fo = 0x698; fo >= 0x638; fo -= 0x18) functionDestroy(s + fo);
        const uint32_t bvt = ld32(g::GOT_VT_BaseScrollView_int);
        st32(s, bvt + 8);
        st32(s + 0x280, bvt + 0x360);
        st32(s + 0x350, bvt + 0x374);
        call(g::VectorInt_dtor, {s + F_ITEM_IDS});
        call(g::IScrollProtocol_dtor, {s});
        return s;
    }

    // Rows 946x128 (the scroll bar on); the row template PartyTop_InitialPanel (hidden) and its avatar icon cache.
    static uint32_t init(uint32_t s, uint32_t type) {
        st32(s + F_TYPE, type);
        stf(s + F_ITEM_W, 946.0f);
        stf(s + F_ITEM_H, 128.0f);
        call(g::IScrollProtocol_setEnabledScrollBar, {s, 1});
        stf(s + 0x570, ldf(s + F_ITEM_W) * 0.5f);
        stf(s + 0x574, -93.0f);
        stf(s + 0x57C, type == 1 ? 163.0f : 103.0f);
        const uint32_t panel =
            call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_InitialPanelJson});
        st32(s + F_PANEL, panel);
        call(g::Ref_retain, {panel});
        vcall(ld32(s + F_PANEL), V_widgetSetTouchEnabled, {ld32(s + F_PANEL), 0});
        const uint32_t avatar = findNode(ld32(s + F_PANEL), S_Avatar, 6);
        const uint32_t renderer = vcall(avatar, V_getVirtualRenderer, {avatar});
        const uint32_t texture = vcall(renderer, V_getTexture, {renderer});
        const uint32_t cache = call(fn::operator_new, {AVATAR_ICON_CACHE_SIZE});
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctorXY, {pos, fbits(-103.0f), fbits(122.0f)});
        // vector<Avatar::Expression>{1}
        const uint32_t expression = f.alloc(4), expressions = f.alloc(12);
        st32(expression, 1);
        st32(expressions, 0);
        st32(expressions + 4, 0);
        st32(expressions + 8, 0);
        call(g::ExpressionVector_allocate, {expressions, 1});
        uint32_t end = ld32(expressions + 4);
        if (end) {
            st32(end, ld32(expression));
            end = ld32(expressions + 4);
        }
        st32(expressions + 4, end + 4);
        call(g::AvatarIconCache_ctor, {cache, texture, pos, fbits(-1.43f), fbits(1.43f), expressions});
        st32(s + F_ICONS, cache);
        call(g::ExpressionVector_destroy, {expressions});
        call(g::Vec2_dtor, {pos});
        return call(g::BaseScrollViewInt_init, {s});
    }

    // A row: an empty widget of the row size holding a clone of the template.
    static uint32_t getLayoutPanel(uint32_t s, uint32_t) {
        const uint32_t panel = call(g::Widget_create, {});
        const uint32_t row = call(g::Widget_clone, {ld32(s + F_PANEL)});
        vcall(panel, node::setAnchorPoint, {panel, ld32(g::GOT_ANCHOR_MIDDLE)});
        vcall(panel, node::addChild, {panel, row});
        {
            Frame f;
            const uint32_t size = f.alloc(8);
            call(g::Size_ctor, {size, ld32(s + F_ITEM_W), ld32(s + F_ITEM_H)});
            vcall(panel, node::setContentSize, {panel, size});
        }
        vcall(panel, node::setPositionX, {panel, fbits(ldf(s + F_ITEM_W) * 0.5f)});
        return panel;
    }

    static uint32_t createRow(uint32_t s, uint32_t index, uint32_t arg) {
        const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 2);
        if (count <= index) {
            call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
            return 0;
        }
        const uint32_t panel = getLayoutPanel(s, s + F_ITEM_IDS);
        vcall(s, V_changeRow, {s, panel, index, arg});
        return panel;
    }

    // Touch listener for a row (registered on the list itself): began / moved / ended / cancelled with (this, index).
    static uint32_t registItemTouchEvent(uint32_t s, uint32_t, uint32_t index) {
        if (!ld8(s + F_TOUCH)) return 0;
        const uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
        const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
        call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
        Frame f;
        const uint32_t began = newFunction(f, VT_RowTouchBegan, {s, index});
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
            st32(obj, VT_RowTouchMoved);
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
        call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, s});
        return 0;
    }

    // sub_51F02C: function<void(Touch*, Event*)> = $_2 {this, index, this, index}
    static uint32_t assignTouchEnded(uint32_t dst, uint32_t captures) {
        Frame f;
        const uint32_t fn = f.alloc(20);
        st32(fn + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x14});
        if (obj)
            for (uint32_t k = 0; k < 0x14; k += 4) st32(obj + k, k ? ld32(captures + k - 4) : VT_RowTouchEnded);
        st32(fn + 16, obj);
        call(g::FunctionVTE_swap, {fn, dst});
        functionDestroy(fn);
        return 0;
    }

    // changeRow $_5 / $_4 / $_3 {button, view, row}: on touch end of an enabled button with a callback set, keep the
    // row, block taps for the no-tap time and call the callback after it.
    static void rowButton(const RowButton &b, uint32_t functor, uint32_t typeRef) {
        const uint32_t type = ld32(typeRef);
        const uint32_t view = ld32(functor + 8);
        const uint32_t button = ld32(functor + 4);
        const uint32_t visible = vcall(button, node::isVisible, {button});
        if (type != 2 || visible != 1) return;
        call(g::AudioPlaySystemSEDecide, {});
        if (!ld32(view + b.callback + 0x10)) return;
        st32(view + F_ROW, ld32(functor + 12));
        const uint32_t sm = sceneManager();
        call(g::SM_openNoTapScreen, {sm, ld32(sceneManager() + SM_NO_TAP_TIME)});
        const uint32_t delay = call(g::DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
        Frame f;
        const uint32_t then = newFunction(f, b.vtDelayed, {view});
        const uint32_t cf = call(g::CallFunc_create, {then});
        call(g::Node_runAction, {view, call(g::Sequence_create, {delay, cf, 0})});
        functionDestroy(then);
    }
    static void rowButton5(uint32_t f, uint32_t, uint32_t t) { rowButton(BUTTON_5, f, t); }
    static void rowButton4(uint32_t f, uint32_t, uint32_t t) { rowButton(BUTTON_4, f, t); }
    static void rowButton3(uint32_t f, uint32_t, uint32_t t) { rowButton(BUTTON_3, f, t); }
    // their lambda() {view}: the callback with the kept row
    static void delayed(uint32_t functor, uint32_t callback) {
        const uint32_t view = ld32(functor + 4);
        call(g::FunctionVI_call, {view + callback, ld32(view + F_ROW)});
    }
    static void delayed5(uint32_t f) { delayed(f, BUTTON_5.callback); }
    static void delayed4(uint32_t f) { delayed(f, BUTTON_4.callback); }
    static void delayed3(uint32_t f) { delayed(f, BUTTON_3.callback); }

    // changeRow $_0 (0x1A8: two words, a Party, 0x128 bytes): __clone() / destroy_deallocate()
    static uint32_t partyClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x1A8});
        if (obj) {
            st32(obj, VT_RowParty);
            st32(obj + 8, ld32(self + 8));
            st32(obj + 12, ld32(self + 12));
            call(g::Party_copy, {obj + 0x10, self + 0x10});
            call(g::MEMCPY, {obj + 0x80, self + 0x80, 0x128});
        }
        return obj;
    }
    static void partyDestroyDeallocate(uint32_t self) {
        strFree(self + 0x70);
        call(g::VectorULL_dtor, {self + 0x50});
        strFree(self + 0x34);
        strFree(self + 0x20);
        call(fn::operator_delete, {self});
    }

    // ------------------------------------------------------------------------------------------- changeRow
    // hole::network::api::Party (0x70): +0 4 words (id), +0xC rank, +0x10 name, +0x1C play style, +0x20 join type,
    // +0x24 message, +0x30 members, +0x40 vector<ull>, +0x4C 0x14 bytes (a Date first), +0x60 string; a PartyEx adds
    // 0x128 bytes (the leader's UserAvatar: +0 user id, +0xE8 coordinate).
    static void partyFree(uint32_t p) {
        strFree(p + 0x60);
        call(g::VectorULL_dtor, {p + 0x40});
        strFree(p + 0x24);
        strFree(p + 0x10);
    }
    // PartyEx::operator= as inlined (0x124 bytes of the avatar part)
    static void partyAssign(uint32_t dst, uint32_t src) {
        for (uint32_t k = 0; k < 0x10; k += 4) st32(dst + k, ld32(src + k));
        call(STRING_ASSIGN, {dst + 0x10, src + 0x10});
        st32(dst + 0x1C, ld32(src + 0x1C));
        st32(dst + 0x20, ld32(src + 0x20));
        call(STRING_ASSIGN, {dst + 0x24, src + 0x24});
        for (uint32_t k = 0x30; k < 0x40; k += 4) st32(dst + k, ld32(src + k));
        if (dst != src) call(VECTOR_ULL_ASSIGN, {dst + 0x40, ld32(src + 0x40), ld32(src + 0x44)});
        call(g::MEMCPY, {dst + 0x4C, src + 0x4C, 0x14});
        call(STRING_ASSIGN, {dst + 0x60, src + 0x60});
        call(g::MEMCPY, {dst + 0x70, src + 0x70, 0x124});
    }
    // a Party moved (its strings and vector taken)
    static void partyMove(uint32_t dst, uint32_t src) {
        auto take = [&](uint32_t o, uint32_t n, bool clear) {
            for (uint32_t k = 0; k < n; k += 4) {
                st32(dst + o + k, ld32(src + o + k));
                if (clear) st32(src + o + k, 0);
            }
        };
        take(0, 0x10, false);
        take(0x10, 12, true);
        take(0x1C, 8, false);
        take(0x24, 12, true);
        take(0x30, 0x10, false);
        take(0x40, 12, true);
        call(g::MEMCPY, {dst + 0x4C, src + 0x4C, 0x14});
        take(0x60, 12, true);
    }
    // the "Txt" name as the compiler builds it inline (a short string)
    static uint32_t inlineTxt(Frame &f) {
        const uint32_t s = f.alloc(12);
        st8(s, 6);
        st8(s + 1, 'T');
        st8(s + 2, 'x');
        st8(s + 3, 't');
        st8(s + 4, 0);
        return s;
    }
    // a row button's ImageButton (created on first use: "But01" over the button, its "Txt" label)
    static uint32_t rowImageButton(uint32_t row, uint32_t button, uint32_t name, uint32_t len, int32_t label) {
        uint32_t ib = findNode(row, name, len);
        if (ib) return ib;
        {
            Frame f;
            const uint32_t file = newStr(f, S_But01, 5);
            ib = call(IMAGE_BUTTON_CREATE, {file, 0});
            strFree(file);
        }
        call(IMAGE_BUTTON_SET_TARGET_IMAGE_VIEW, {ib, button});
        {
            Frame f;
            const uint32_t tag = newStr(f, name, len);
            vcall(button, node::addChildZName, {button, ib, 0, tag});
            strFree(tag);
        }
        Frame f;
        const uint32_t txt = inlineTxt(f);
        SceneUtil::settingTitleLabel(button, txt, label);
        strFree(txt);
        return ib;
    }
    static void imageButtonTouch(uint32_t ib, uint32_t vt, std::initializer_list<uint32_t> captures) {
        Frame f;
        const uint32_t fo = newFunction(f, vt, captures);
        call(IMAGE_BUTTON_ADD_TOUCH, {ib, fo});
        functionDestroy(fo);
    }
    // a label's text from a choice (TextData ids[v - 1] for v in 1..3, else empty) through the string `s`
    static void choiceText(uint32_t label, uint32_t value, const int32_t ids[3], uint32_t s) {
        if (value >= 1 && value <= 3) {
            Frame f;
            const uint32_t td = textData(f, ids[value - 1]);
            const uint32_t text = textDataStr(td);
            call(STRING_ASSIGN_N, {s, text, host_strlen(text)});
            sharedRelease(td);
        }
        call(fn::Text_setString, {label, s});
    }
    // a date label: "%s %s" of TextData `id` and the date
    static void dateLabel(uint32_t label, int32_t id, uint32_t dateText) {
        Frame f;
        const uint32_t td = textData(f, id);
        setTextC(label, stringBuffer(f, {S_TwoStrings, textDataStr(td), strData(dateText)}));
        sharedRelease(td);
    }

    // Fill `row` with party itemIDs[index] of the view's list (0 search results: APIManager's party list; 1 the
    // user's requests; 2 the invitations - their party found by id): the leader's avatar icon (its button carries
    // the party: $_0), rank, name, message, members, play style, join type, the request / reception date, and the
    // list's button (Request $_2 / Cancel $_3 / Approve $_4 + Delete $_5). 1 when the row has no Plate_Initial.
    static uint32_t changeRow(uint32_t s, uint32_t row, uint32_t index, uint32_t) {
        const uint32_t ids = ld32(s + F_ITEM_IDS);
        if (uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ids) >> 2) <= index) call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        if (!findNode(row, S_PlateInitial, 0xD)) return 1;
        Frame f;
        const uint32_t party = f.alloc(PARTY_EX);
        for (uint32_t k = 0; k < PARTY_EX; k += 4) st32(party + k, 0);
        const uint32_t dateText = newStr(f, S_EMPTY, 0);
        const uint32_t date = f.alloc(8);
        for (uint32_t k = 0; k < 8; k += 4) st32(date + k, 0);
        const uint32_t type = ld32(s + F_TYPE);
        const uint32_t id = ld32(ids + 4 * index);
        if (type == 0) {
            const uint32_t parties = f.alloc(12);
            call(VECTOR_PARTY_EX_COPY, {parties, call(fn::APIManager_getInstance, {}) + API_PARTIES});
            if ((ld32(parties + 4) - ld32(parties)) / PARTY_EX <= id) call(g::ThrowOutOfRange, {parties});
            partyAssign(party, ld32(parties) + PARTY_EX * id);
            call(DATE_MAKE_LOCAL, {date, party + 0x4C});
            call(VECTOR_PARTY_EX_DTOR, {parties});
        } else if (type == 1 || type == 2) {
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t list = type == 1 ? API_REQUESTS : API_INVITES;
            const uint32_t ctrl = ld32(api + list + 4), x = ld32(api + list);
            if (ctrl) call(fn::shared_add, {ctrl});
            const uint32_t parties = f.alloc(12);
            call(VECTOR_PARTY_EX_COPY, {parties, x + (type == 1 ? 0xC : 0x14)});
            if ((ld32(x + 4) - ld32(x)) / RECORD <= id) call(g::ThrowOutOfRange, {x});
            const uint32_t entry = ld32(x) + RECORD * id;
            if (type == 1) {
                call(DATE_MAKE_LOCAL, {date, entry + 0x20});
                if ((ld32(parties + 4) - ld32(parties)) / PARTY_EX <= id) call(g::ThrowOutOfRange, {parties});
                partyAssign(party, ld32(parties) + PARTY_EX * id);
            } else {
                // the invitation (copied), its party by id
                const uint32_t rec = f.alloc(RECORD);
                for (uint32_t k = 0; k < 0x10; k += 4) st32(rec + k, ld32(entry + k));
                if (ld8(entry + 0x10) & 1) strInit(rec + 0x10, ld32(entry + 0x18), ld32(entry + 0x14));
                else for (uint32_t k = 0; k < 12; k += 4) st32(rec + 0x10 + k, ld32(entry + 0x10 + k));
                for (uint32_t k = 0x1C; k < RECORD; k += 4) st32(rec + k, ld32(entry + k));
                call(DATE_MAKE_LOCAL, {date, rec + 0x20});
                const uint32_t n = (ld32(parties + 4) - ld32(parties)) / PARTY_EX;
                for (uint32_t k = 0; k < n; k++) {
                    const uint32_t ex = ld32(parties) + PARTY_EX * k;
                    if (ld32(ex) == ld32(rec + 8) && ld32(ex + 4) == ld32(rec + 0xC)) {
                        partyAssign(party, ex);
                        break;
                    }
                }
                strFree(rec + 0x10);
            }
            call(VECTOR_PARTY_EX_DTOR, {parties});
            if (ctrl) call(fn::shared_release, {ctrl});
        }
        {
            const uint32_t ymd = rw::load<uint16_t>(date + 2), hm = rw::load<uint16_t>(date + 4);
            Frame h;
            const uint32_t text = h.alloc(12);
            call(TEXT_DATA_FORMAT, {text, 0, uint32_t(TXT_DATE), rw::load<uint16_t>(date), ymd & 0xFF, ymd >> 8,
                                    hm & 0xFF, hm >> 8, ld8(date + 6)});
            const uint32_t c = strData(text);
            call(STRING_ASSIGN_N, {dateText, c, host_strlen(c)});
            strFree(text);
        }
        // the leader's avatar icon over the Avatar (or in place of the previous one)
        const uint32_t user = f.alloc(0x128);
        call(g::MEMCPY, {user, party + PARTY, 0x128});
        const uint32_t avatar = findNode(row, S_Avatar, 6);
        const uint32_t old = findNode(row, S_AvatarIconObj, 0xF);
        auto iconFor = [&]() {
            uint32_t icon = call(ICON_CACHE_FIND, {ld32(s + F_ICONS), 0, ld32(user), ld32(user + 4)});
            if (!icon) {
                const uint32_t cache = ld32(s + F_ICONS);
                Frame h;
                const uint32_t style = h.alloc(0x40);
                call(AVATAR_MAKE_STYLE, {style, user + 0xE8, 0});
                icon = call(ICON_CACHE_ADD, {cache, 0, ld32(user), ld32(user + 4), style});
                call(SAFE_BUFFER_DTOR, {style});
            }
            return icon;
        };
        auto addIcon = [&](uint32_t parent, uint32_t icon, uint32_t z) {
            Frame h;
            const uint32_t tag = newStr(h, S_AvatarIconObj, 0xF);
            vcall(parent, node::addChildZName, {parent, icon, z, tag});
            strFree(tag);
        };
        if (old) {
            const uint32_t icon = iconFor();
            if (icon != old) {
                vcall(icon, node::setPosition, {icon, vcall(old, node::getPosition, {old})});
                const uint32_t parent = vcall(old, node::getParent, {old});
                vcall(icon, node::removeFromParent, {icon});
                addIcon(parent, icon, vcall(old, V_GET_LOCAL_Z, {old}));
                vcall(old, node::removeFromParent, {old});
            }
        } else {
            const uint32_t icon = iconFor();
            vcall(icon, node::setPosition, {icon, vcall(avatar, node::getPosition, {avatar})});
            vcall(icon, node::removeFromParent, {icon});
            const uint32_t parent = vcall(avatar, node::getParent, {avatar});
            addIcon(parent, icon, vcall(avatar, V_GET_LOCAL_Z, {avatar}));
            vcall(avatar, node::setOpacity, {avatar, 0});
        }
        uint32_t avatarButton = findNode(avatar, S_ImageButtonAvatar, 0x12);
        if (!avatarButton) {
            {
                Frame h;
                const uint32_t file = newStr(h, S_EMPTY, 0);
                avatarButton = call(IMAGE_BUTTON_CREATE, {file, 0});
                strFree(file);
            }
            call(IMAGE_BUTTON_SET_TARGET_VIEW, {avatarButton, avatar, fbits(0.8f)});
            Frame h;
            const uint32_t tag = newStr(h, S_ImageButtonAvatar, 0x12);
            vcall(avatar, node::addChildZName, {avatar, avatarButton, 10, tag});
            strFree(tag);
        }
        {
            // $_0 {row, view, Party, 0x128 bytes}: built on the stack, moved into the heap
            Frame h;
            const uint32_t lambda = h.alloc(0x80 + 0x128);
            st32(lambda, row);
            st32(lambda + 4, s);
            call(g::Party_copy, {lambda + 8, party});
            call(g::MEMCPY, {lambda + 0x78, party + PARTY, 0x128});
            const uint32_t fo = h.alloc(20);
            st32(fo + 16, 0);
            const uint32_t heap = call(fn::operator_new, {0x1A8});
            if (heap) {
                st32(heap, VT_RowParty);
                st32(heap + 0xC, ld32(lambda + 4));
                st32(heap + 8, ld32(lambda));
                partyMove(heap + 0x10, lambda + 8);
                call(g::MEMCPY, {heap + 0x80, lambda + 0x78, 0x128});
            }
            st32(fo + 16, heap);
            call(IMAGE_BUTTON_ADD_TOUCH, {avatarButton, fo});
            functionDestroy(fo);
            partyFree(lambda + 8);
        }
        {
            const uint32_t rank = findNode(row, S_RankIcon, 8);
            Frame h;
            const uint32_t path = h.alloc(12);
            call(RANK_IMAGE_PATH, {path, ld32(party + 0xC)});
            const uint32_t copy = newStr(h, strData(path));
            call(IMAGE_VIEW_LOAD_TEXTURE, {rank, copy, 0});
            strFree(copy);
            strFree(path);
        }
        {
            const uint32_t name = findNode(row, S_TxtName, 8);
            SceneUtil::setShadow(name);
            setTextC(name, strData(party + 0x10));
        }
        setTextC(findNode(row, S_TxtMessageLabel, 0x11), strData(party + 0x24));
        {
            const uint32_t members = findNode(row, S_PartyInfo1Label, 0x15);
            SceneUtil::setShadow(members);
            Frame h;
            setTextC(members, stringBuffer(h, {S_IntFormat, ld32(party + 0x30)}));
        }
        titleLabel(row, S_PartyInfo1, 0xF, TXT_MEMBERS);
        const uint32_t style = findNode(row, S_PartyInfo2Label, 0x15);
        SceneUtil::setShadow(style);
        const uint32_t styleText = newStr(f, S_EMPTY, 0);
        choiceText(style, ld32(party + 0x1C), TXT_STYLES, styleText);
        titleLabel(row, S_PartyInfo2, 0xF, TXT_PLAY_STYLE);
        const uint32_t join = findNode(row, S_PartyInfo3Label, 0x15);
        SceneUtil::setShadow(join);
        const uint32_t joinText = newStr(f, S_EMPTY, 0);
        choiceText(join, ld32(party + 0x20), TXT_JOINS, joinText);
        titleLabel(row, S_PartyInfo3, 0xF, TXT_JOIN_TYPE);
        const uint32_t requestDate = findNode(row, S_RequestDateLabel, 0x15);
        const uint32_t reception = findNode(row, S_ReceptionLabel, 0x13);
        SceneUtil::setShadow(requestDate);
        SceneUtil::setShadow(reception);
        dateLabel(requestDate, TXT_REQUEST_DATE, dateText);
        dateLabel(reception, TXT_RECEPTION, dateText);
        if (type == 0) {
            vcall(requestDate, node::setVisible, {requestDate, 0});
            vcall(reception, node::setVisible, {reception, 0});
        } else if (type == 1) {
            vcall(requestDate, node::setVisible, {requestDate, 1});
            vcall(reception, node::setVisible, {reception, 0});
        } else if (type == 2) {
            vcall(requestDate, node::setVisible, {requestDate, 0});
            vcall(reception, node::setVisible, {reception, 1});
        }
        const uint32_t request = findNode(row, S_ButtonRequest, 0xE);
        const uint32_t cancel = findNode(row, S_ButtonCancelRequest, 0x14);
        const uint32_t approve = findNode(row, S_ButtonApproval, 0xF);
        const uint32_t remove = findNode(row, S_ButtonDelete, 0xD);
        for (const uint32_t b : {request, cancel, approve, remove}) vcall(b, node::setVisible, {b, 0});
        const uint32_t ibRequest = rowImageButton(row, request, S_ImageButtonRequest, 0x13, TXT_BUTTON_REQUEST);
        const uint32_t ibCancel = rowImageButton(row, cancel, S_ImageButtonCancel, 0x1C, TXT_BUTTON_CANCEL);
        const uint32_t ibApprove = rowImageButton(row, approve, S_ImageButtonApproval, 0x14, TXT_BUTTON_APPROVE);
        const uint32_t ibRemove = rowImageButton(row, remove, S_ImageButtonDelete, 0x12, TXT_BUTTON_DELETE);
        imageButtonTouch(ibRequest, VT_Button2, {row, s, ld32(ids + 4 * index)});
        imageButtonTouch(ibCancel, VT_Button3, {row, s, ld32(ids + 4 * index)});
        imageButtonTouch(ibApprove, VT_Button4, {row, s, ld32(ids + 4 * index)});
        imageButtonTouch(ibRemove, VT_Button5, {row, s, ld32(ids + 4 * index)});
        if (type == 0) {
            vcall(request, node::setVisible, {request, 1});
        } else if (type == 1) {
            vcall(cancel, node::setVisible, {cancel, 1});
        } else if (type == 2) {
            vcall(approve, node::setVisible, {approve, 1});
            vcall(remove, node::setVisible, {remove, 1});
        }
        strFree(joinText);
        strFree(styleText);
        strFree(dateText);
        partyFree(party);
        return 0;
    }

    // ------------------------------------------------------------------------------------------- row touches
    // the tap {view, row, 2 words}(touch): the touched index from the row and the scroll position; an entry there
    // and the touch still near its start: the row's tap animation
    static uint32_t tapRow(uint32_t captures, uint32_t touch) {
        const uint32_t view = ld32(captures);
        const float rowStep = ldf(view + F_ITEM_H) + ldf(ld32(GOT_ROW_SPACING));
        const int32_t rows = f2i(ldf(view + 0x540)) / f2i(rowStep);
        const uint32_t index = ld32(captures + 4) + ld32(view + 0x538) * uint32_t(rows) - 1;
        st32(view + F_TAP_INDEX, index);
        const uint32_t count = uint32_t(int32_t(ld32(view + F_ITEM_IDS + 4) - ld32(view + F_ITEM_IDS)) >> 2);
        if (count > index && ld8(view + F_TOUCH_INSIDE)) {
            Frame f;
            const uint32_t loc = f.alloc(8);
            call(TOUCH_GET_LOCATION, {loc, touch});
            if (std::fabs(ldf(loc + 4) - ldf(view + F_TOUCH_Y)) < 10.0f) {
                const uint32_t at = ld32(view + F_TAP_INDEX);
                if (uint32_t(int32_t(ld32(view + F_ITEM_IDS + 4) - ld32(view + F_ITEM_IDS)) >> 2) <= at)
                    call(g::ThrowOutOfRange, {view + F_ITEM_IDS});
                const uint32_t entry = ld32(ld32(view + F_ITEM_IDS) + 4 * at);
                call(g::Vec2_dtor, {loc});
                if (entry) {
                    st8(view + F_TAP_RUNNING, 1);
                    const uint32_t row = ld32(ld32(view + 0x51C) + 4 * ld32(captures + 4));
                    vcall(row, node::setLocalZOrder, {row, 1});
                    Frame h;
                    const uint32_t done = newFunction(h, VT_TapDone, {ld32(captures + 8), ld32(captures + 0xC)});
                    const uint32_t action = call(TAP_ANIMATION, {row, done});
                    call(g::Node_runAction, {row, action});
                    functionDestroy(done);
                    return 0;
                }
            } else {
                call(g::Vec2_dtor, {loc});
            }
        }
        st8(view + F_TOUCH_INSIDE, 0);
        return 0;
    }
    // touch moved {-, view, row, tap captures (+0xC)}(touch*): leaving the row ends the tap
    static uint32_t touchMoved(uint32_t self, uint32_t touchp) {
        const uint32_t touch = ld32(touchp);
        const uint32_t view = ld32(self + 4);
        if (!ld8(view + F_TOUCH_INSIDE)) return 0;
        const uint32_t row = ld32(ld32(view + 0x51C) + 4 * ld32(self + 8));
        Frame f;
        const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
        call(TOUCH_GET_LOCATION, {loc, touch});
        call(NODE_CONVERT_TO_NODE_SPACE, {p, row, loc});
        call(g::Vec2_dtor, {loc});
        const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
        const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
        call(RECT_CTOR, {rect, 0, 0, w, h});
        if (!call(RECT_CONTAINS, {rect, p})) {
            st8(view + F_TOUCH_INSIDE, 0);
            tapRow(self + 0xC, touch);
        }
        call(g::Vec2_dtor, {rect});
        call(g::Vec2_dtor, {p});
        return 0;
    }
    // touch began {-, view, row}(touch*): the touch's y kept; inside the row: marked, 1
    static uint32_t touchBegan(uint32_t self, uint32_t touchp) {
        const uint32_t view = ld32(self + 4);
        if (ld8(view + F_TAP_RUNNING)) return 0;
        const uint32_t touch = ld32(touchp);
        {
            Frame f;
            const uint32_t loc = f.alloc(8);
            call(TOUCH_GET_LOCATION, {loc, touch});
            st32(view + F_TOUCH_Y, ld32(loc + 4));
            call(g::Vec2_dtor, {loc});
        }
        const uint32_t row = ld32(ld32(view + 0x51C) + 4 * ld32(self + 8));
        Frame f;
        const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
        call(TOUCH_GET_LOCATION, {loc, touch});
        call(NODE_CONVERT_TO_NODE_SPACE, {p, row, loc});
        call(g::Vec2_dtor, {loc});
        const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
        const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
        call(RECT_CTOR, {rect, 0, 0, w, h});
        uint32_t r = 0;
        if (call(RECT_CONTAINS, {rect, p})) {
            r = 1;
            st8(view + F_TOUCH_INSIDE, 1);
        }
        call(g::Vec2_dtor, {rect});
        call(g::Vec2_dtor, {p});
        return r;
    }

    // $_2 (Request) {row, view, row id}: the type checked first, then the row's visibility (unlike $_3..$_5)
    static uint32_t rowButton2(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return 0;
        const uint32_t button = ld32(functor + 4), view = ld32(functor + 8);
        if (vcall(button, node::isVisible, {button}) != 1) return 0;
        call(g::AudioPlaySystemSEDecide, {});
        if (!ld32(view + BUTTON_2.callback + 0x10)) return 0;
        st32(view + F_ROW, ld32(functor + 12));
        const uint32_t sm = sceneManager();
        call(g::SM_openNoTapScreen, {sm, ld32(sceneManager() + SM_NO_TAP_TIME)});
        const uint32_t delay = call(g::DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
        Frame f;
        const uint32_t then = newFunction(f, BUTTON_2.vtDelayed, {view});
        const uint32_t cf = call(g::CallFunc_create, {then});
        call(g::Node_runAction, {view, call(g::Sequence_create, {delay, cf, 0})});
        functionDestroy(then);
        return 0;
    }

    // $_0 {row, view, Party, 0x128 bytes}: the avatar's button - the leader's info after the no-tap delay (once)
    static uint32_t avatarTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return 0;
        const uint32_t row = ld32(functor + 8), view = ld32(functor + 0xC);
        if (vcall(row, node::isVisible, {row}) != 1) return 0;
        if (ld8(view + F_AVATAR_OPENING)) return 0;
        st8(view + F_AVATAR_OPENING, 1);
        call(g::AudioPlaySystemSEDecide, {});
        st32(view + F_AVATAR_USER + 4, ld32(functor + 0x84));
        st32(view + F_AVATAR_USER, ld32(functor + 0x80));
        const uint32_t sm = sceneManager();
        call(g::SM_openNoTapScreen, {sm, ld32(sceneManager() + SM_NO_TAP_TIME)});
        const uint32_t delay = call(g::DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
        Frame f;
        const uint32_t then = newFunction(f, VT_AvatarDelayed, {view});
        const uint32_t cf = call(g::CallFunc_create, {then});
        call(g::Node_runAction, {view, call(g::Sequence_create, {delay, cf, 0})});
        functionDestroy(then);
        return 0;
    }
    // its delayed {view}: the leader's avatar info popup (closed: {view})
    static uint32_t avatarDelayed(uint32_t functor) {
        const uint32_t view = ld32(functor + 4);
        const uint32_t hi = ld32(view + F_AVATAR_USER + 4), lo = ld32(view + F_AVATAR_USER);
        Frame f;
        const uint32_t closed = newFunction(f, VT_AvatarClosed, {view});
        const uint32_t check = f.alloc(20);
        st32(check + 16, 0);
        call(OPEN_AVATAR_INFO_POPUP, {lo, hi, 0, closed, check});
        functionDestroy(check);
        functionDestroy(closed);
        return 0;
    }
    // $_0's __func: ~__func / deleting / __clone(p) / destroy
    static uint32_t partyLambdaDtor(uint32_t self) {
        st32(self, VT_RowParty);
        partyFree(self + 0x10);
        return self;
    }
    static void partyLambdaDelete(uint32_t self) {
        partyLambdaDtor(self);
        call(fn::operator_delete, {self});
    }
    static uint32_t partyCloneInto(uint32_t self, uint32_t p) {
        if (!p) return self;
        st32(p, VT_RowParty);
        st32(p + 8, ld32(self + 8));
        st32(p + 0xC, ld32(self + 0xC));
        call(g::Party_copy, {p + 0x10, self + 0x10});
        return call(g::MEMCPY, {p + 0x80, self + 0x80, 0x128});
    }
    static void partyDestroy(uint32_t self) { partyFree(self + 0x10); }
};

RW_FUNCTION(0x51CB04, "PartyScrollView::create", PartyScrollView::create);
RW_FUNCTION(0x51CB60, "PartyScrollView::PartyScrollView", PartyScrollView::construct);
RW_FUNCTION(0x51C958, "PartyScrollView::~PartyScrollView", PartyScrollView::destruct);
RW_FUNCTION(0x51CCF0, "PartyScrollView::init", PartyScrollView::init);
RW_FUNCTION(0x51CEF4, "PartyScrollView::getLayoutPanel", PartyScrollView::getLayoutPanel);
RW_FUNCTION(0x51CFC0, "PartyScrollView::createRow", PartyScrollView::createRow);
RW_FUNCTION(0x51EEE4, "PartyScrollView::registItemTouchEvent", PartyScrollView::registItemTouchEvent);
RW_FUNCTION(0x51F02C, "PartyScrollView::registItemTouchEvent touch-ended assign (sub_51F02C)",
            PartyScrollView::assignTouchEnded);
RW_FUNCTION(0x51F844, "PartyScrollView::changeRow::$_5 operator()", PartyScrollView::rowButton5);
RW_FUNCTION(0x51FA9C, "PartyScrollView::changeRow::$_4 operator()", PartyScrollView::rowButton4);
RW_FUNCTION(0x51FCF4, "PartyScrollView::changeRow::$_3 operator()", PartyScrollView::rowButton3);
RW_FUNCTION(0x51F9E6, "PartyScrollView::changeRow::$_5::lambda() operator()", PartyScrollView::delayed5);
RW_FUNCTION(0x51FC3E, "PartyScrollView::changeRow::$_4::lambda() operator()", PartyScrollView::delayed4);
RW_FUNCTION(0x51FE96, "PartyScrollView::changeRow::$_3::lambda() operator()", PartyScrollView::delayed3);
RW_FUNCTION(0x5201E8, "PartyScrollView::changeRow::$_0 __clone()", PartyScrollView::partyClone);
RW_FUNCTION(0x5202EA, "PartyScrollView::changeRow::$_0 destroy_deallocate()", PartyScrollView::partyDestroyDeallocate);
RW_FUNCTION(0x51D018, "PartyScrollView::changeRow", PartyScrollView::changeRow);
RW_FUNCTION(0x51F19C, "PartyScrollView::registItemTouchEvent row tap (sub_51F19C)", PartyScrollView::tapRow);
RW_FUNCTION(0x51F4D8, "PartyScrollView::registItemTouchEvent touch moved (sub_51F4D8)", PartyScrollView::touchMoved);
RW_FUNCTION(0x51F67C, "PartyScrollView::registItemTouchEvent touch began (sub_51F67C)", PartyScrollView::touchBegan);
RW_FUNCTION(0x51FF4C, "PartyScrollView::changeRow::$_2 operator()", PartyScrollView::rowButton2);
RW_FUNCTION(0x520330, "PartyScrollView::changeRow::$_0 operator()", PartyScrollView::avatarTouched);
RW_FUNCTION(0x5204E8, "PartyScrollView::changeRow::$_0::lambda() operator()", PartyScrollView::avatarDelayed);
RW_FUNCTION(0x520134, "PartyScrollView::changeRow::$_0 __func::~__func", PartyScrollView::partyLambdaDtor);
RW_FUNCTION(0x52018C, "PartyScrollView::changeRow::$_0 __func::~__func (deleting)", PartyScrollView::partyLambdaDelete);
RW_FUNCTION(0x520258, "PartyScrollView::changeRow::$_0 __clone(p)", PartyScrollView::partyCloneInto);
RW_FUNCTION(0x5202AC, "PartyScrollView::changeRow::$_0 destroy()", PartyScrollView::partyDestroy);
