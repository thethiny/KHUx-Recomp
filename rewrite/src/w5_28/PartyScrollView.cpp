// PartyScrollView: the party lists of the Party screens (a BaseScrollView<int> of indexes into an APIManager party
// vector: 0 the search result, 1 the user's requests, 2 the invitations to him). Rows are PartyTop_InitialPanel
// clones with an AvatarIconCache for the leaders' avatars; each row's buttons (Withdraw / Accept / Delete / Details)
// call one of the view's std::function<void(int)> callbacks with the row after the scene manager's no-tap delay.
// changeRow (0x51D018) is not rewritten.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t MEMCPY = 0x40E164;                      // __aeabi_memcpy(dst, src, n)
constexpr uint32_t Widget_create = 0x825875, Widget_clone = 0x8272B9;
constexpr uint32_t Size_ctor = 0x8DF027;                   // Size(float, float)
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
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
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_openNoTapScreen = 0x61FB25;          // (sm, float seconds)
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t Node_runAction = 0x894AED;
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
