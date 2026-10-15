// AvatarTitleScrollView: the list of player titles (a BaseScrollView<int> over master::Title ids; id 0 = "no
// title"). Each row shows the title board image and its name; tapping a row (a touch that stays within 20 px) plays
// the tap animation and reports the row through onSelect. The std::function thunks of its lambdas are in
// scene_social/thunks/AvatarTitleScrollView_thunks.cpp and w3_4/AvatarTitleScrollView_ThunksW3.cpp.
#include "Common.hpp"

using namespace w36;

struct AvatarTitleScrollView {
    const uint8_t layout[0x4FC];               // cocos2d::ui::Layout (vtables at +0, +0x280, +0x350)
    const uint8_t scroll[0x628 - 0x4FC];       // IScrollProtocol (see scene_main/IScrollProtocol.hpp)
    guest::Vector itemIDs;                     // +0x628 BaseScrollView<int>: the title ids
    const uint8_t pad_634[4];
    const uint8_t onSelect[24];                // +0x638 std::function<void(int)>: a row was tapped (row index)
    rw::Field<uint32_t> panel;                 // +0x650 retained row template (AvatarTitle_PanelButton.json in a Widget)
    rw::Field<uint8_t> touching;               // +0x654 a touch started on a row and stayed on it
    const uint8_t pad_655[3];
    rw::Field<int32_t> selected;               // +0x658 the touched row
    rw::Field<uint8_t> tapped;                 // +0x65C
    const uint8_t pad_65D[3];
    rw::Field<float> touchStartY;              // +0x660
    rw::Field<uint8_t> tapEnabled;             // +0x664 setEnabledTapItem
    const uint8_t pad_665[3];

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x668;
    static constexpr uint32_t VTABLE = 0xF83B90;     // _ZTV21AvatarTitleScrollView + 8
    static constexpr uint32_t VTABLE2 = 0xF83EE8;    // + 0x360 (at +0x280)
    static constexpr uint32_t VTABLE3 = 0xF83EFC;    // + 0x374 (at +0x350)

    static AvatarTitleScrollView *destroy(AvatarTitleScrollView *self);
    static void destroyDelete(AvatarTitleScrollView *self);
    static Addr create();
    static AvatarTitleScrollView *construct(AvatarTitleScrollView *self);
    static uint32_t init(AvatarTitleScrollView *self);
    static void update(AvatarTitleScrollView *self, float dt);
    static Addr getLayoutPanel(AvatarTitleScrollView *self, int32_t row);
    static Addr createRow(AvatarTitleScrollView *self, uint32_t index, int32_t column);
    static uint32_t changeRow(AvatarTitleScrollView *self, Addr row, uint32_t index, int32_t column);
    static void registItemTouchEvent(AvatarTitleScrollView *self, Addr widget, int32_t index);
    static void setEnabledTapItem(AvatarTitleScrollView *self, uint32_t enabled);
};
RW_LAYOUT(AvatarTitleScrollView, itemIDs, 0x628);
RW_LAYOUT(AvatarTitleScrollView, panel, 0x650);
RW_LAYOUT(AvatarTitleScrollView, selected, 0x658);
RW_LAYOUT(AvatarTitleScrollView, touchStartY, 0x660);
RW_LAYOUT(AvatarTitleScrollView, tapEnabled, 0x664);
RW_SIZE(AvatarTitleScrollView, AvatarTitleScrollView::SIZE);

using Self = AvatarTitleScrollView;

namespace {
using Addr = uint32_t;
namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Layout_ctor = 0x828671;                 // cocos2d::ui::Layout::Layout()
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;
constexpr uint32_t IScrollProtocol_update = 0x4F5CFD;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t BaseScrollView_int_init = 0x4FA8A1;     // BaseScrollView<int>::init()
constexpr uint32_t vector_int_dtor = 0x478961;
constexpr uint32_t throwOutOfRange = 0x47CCE1;             // vector::at out of range (throws)
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Size_ctor = 0x8DF027;                   // (out, float w, float h)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;                   // (out, float x, float y)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B;                   // (out, x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Touch_getLocation = 0x900E57;           // (Vec2* out, touch)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;     // (Vec2* out, node, const Vec2&)
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t ImageView_loadTexture = 0x839731;       // (image, const string&, TextureResType)
constexpr uint32_t TextData_get = 0x4928AD;                // (shared_ptr out, Type, id)
constexpr uint32_t STRING_ASSIGN = 0x48D811;               // std::string::assign(const char*, size_t)
constexpr uint32_t Title_create = 0x78FA8D;                // master::Title::create(shared_ptr out, int id)
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, dst, src, n, index)
constexpr uint32_t SceneUtil_createUserTitleImagePath = 0x46ED99;   // (string* out, int imageId)
constexpr uint32_t SceneUtil_setUserTitleOutline = 0x45BD95;        // (ui::Text*)
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;         // (node, std::function<void()> by value)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FUNC_TOUCH_BOOL_SWAP = 0x483E4D;        // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FUNC_TOUCH_VOID_SWAP = 0x4F3699;        // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FUNC_VI_CALL = 0x4F321D;                // std::function<void(int)>::operator()(int)
constexpr uint32_t DIVSI3 = 0xCD5E8D;                      // __divsi3
}  // namespace g

constexpr uint32_t BASE_VTABLE = 0xF811D0;                 // _ZTV14BaseScrollViewIiE
// (cocos2d::Vec2::ANCHOR_MIDDLE: VEC2_ANCHOR_MIDDLE of scene/SceneUi.hpp, 0xFEF5C0)
constexpr uint32_t ITEM_MARGIN = 0xE1F834;                 // IScrollProtocol::ITEM_MARGIN (float)
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
// EventListenerTouchOneByOne callbacks (std::function members)
constexpr uint32_t ON_TOUCH_BEGAN = 0x50, ON_TOUCH_MOVED = 0x68, ON_TOUCH_ENDED = 0x80, ON_TOUCH_CANCELLED = 0x98;
// IScrollProtocol members
constexpr uint32_t ITEM_WIDTH = 0x514, ITEM_HEIGHT = 0x518, ROW_WIDGETS = 0x51C, POSITION = 0x540;

constexpr uint32_t S_PanelJson = 0xDFD6FA;      // "cocostudio/publish/AvatarTitle_PanelButton.json"
constexpr uint32_t S_mainPanel = 0xDFD1FC;
constexpr uint32_t S_Button = 0xDFD6D5;
constexpr uint32_t S_RankTitle_Board = 0xDF940E;
constexpr uint32_t S_RankTitle_Name = 0xDF941E;
constexpr int32_t TEXT_NO_TITLE = 102200009;
constexpr uint32_t TITLE_NAME = 8;              // master::Title data positions: name (65 bytes)
constexpr uint32_t TITLE_IMAGE = 0x154;         // board image id

// lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_touchBegan = 0xF56578;    // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_touchMoved = 0xF5654C;    // $_1 {this, index, $_2 {this, index, this}} (heap, 24 bytes)
constexpr uint32_t VT_touchEnded = 0xF564F4;    // $_2 {this, index, this}
constexpr uint32_t VT_tapDone = 0xF56520;       // $_3 {this}: the tap animation finished

constexpr uint32_t SLOT_init = 0x248;
constexpr uint32_t SLOT_release = 0x04;
constexpr uint32_t SLOT_changeRow = 0x340;
constexpr uint32_t SLOT_setTouchEnabled = 0x29C;

uint32_t itemCount(const Self *s) { return uint32_t(int32_t(s->itemIDs.end - s->itemIDs.begin) >> 2); }

// std::function<void(Touch*, Event*)> member = lambda $_2 {captures[0..2]} (sub_5112D0: operator=(F&&))
void assignTouchEnded(Addr member, Addr captures) {
    withLambda(VT_touchEnded, {ld32(captures), ld32(captures + 4), ld32(captures + 8)},
               [&](Addr fo) { call(g::FUNC_TOUCH_VOID_SWAP, {fo, member}); });
}

// $_2 body {this, index, this}: the finger lifted: work out the touched row; a touch that moved less than 20 px
// vertically is a tap (decide SE, tap animation on the row's board, onSelect when it ends).
void touchEndedBody(Addr lambda, Addr touch) {
    Self *self = rw::host_ptr<Self>(ld32(lambda));
    const Addr me = rw::guest_addr(self);
    float itemPixel = rw::bitsf(rw::fbits(rw::load<float>(me + ITEM_HEIGHT) + rw::load<float>(ITEM_MARGIN)));
    int32_t pos = f2i(rw::load<float>(me + POSITION));
    int32_t first = int32_t(call(g::DIVSI3, {uint32_t(pos), uint32_t(f2i(itemPixel))}));
    int32_t row = first + int32_t(ld32(lambda + 4)) - 1;
    self->selected = row;
    uint32_t count = itemCount(self);
    if (!(count > uint32_t(row))) self->selected = int32_t(count - 1);
    if (!self->touching) {
        self->touching = 0;
        return;
    }
    Frame f;
    Addr loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    float startY = self->touchStartY;
    float y = ldf(loc + 4);
    call(g::Vec2_dtor, {loc});
    float moved = std::fabs(y - startY);
    if (moved >= 20.0f || moved != moved) {
        self->touching = 0;
        return;
    }
    self->tapped = 1;
    call(g::AudioPlaySystemSEDecide, {});
    Addr widget = ld32(ld32(me + ROW_WIDGETS) + 4 * ld32(lambda + 4));
    Addr board = search(widget, S_RankTitle_Board, 15);
    if (!board) return;
    withLambda(VT_tapDone, {ld32(lambda + 8)}, [&](Addr fo) {
        call(fn::Node_runAction, {board, call(g::SceneUtil_createTapAnimation, {board, fo})});
    });
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::destroy(Self *self) {
    Addr me = rw::guest_addr(self);
    rw::set<uint32_t>(self, 0, VTABLE);
    rw::set<uint32_t>(self, 0x280, VTABLE2);
    rw::set<uint32_t>(self, 0x350, VTABLE3);
    if (Addr panel = self->panel) vcall(panel, SLOT_release, {panel});
    self->panel = 0;
    functionDestroy(me + 0x638);
    rw::set<uint32_t>(self, 0, BASE_VTABLE + 8);
    rw::set<uint32_t>(self, 0x280, BASE_VTABLE + 0x360);
    rw::set<uint32_t>(self, 0x350, BASE_VTABLE + 0x374);
    call(g::vector_int_dtor, {me + 0x628});
    call(g::IScrollProtocol_dtor, {me});
    return self;
}
RW_FUNCTION(0x510938, "AvatarTitleScrollView::~AvatarTitleScrollView", AvatarTitleScrollView::destroy);

void Self::destroyDelete(Self *self) { call(fn::operator_delete, {rw::guest_addr(destroy(self))}); }
RW_FUNCTION(0x510A10, "AvatarTitleScrollView::~AvatarTitleScrollView (deleting)", AvatarTitleScrollView::destroyDelete);

Addr Self::create() {
    Addr view = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(view));
    if (vcall(view, SLOT_init, {view}) == 1) {
        call(sfn::Ref_autorelease, {view});
        return view;
    }
    vcall(view, SLOT_release, {view});
    return 0;
}
RW_FUNCTION(0x510A44, "AvatarTitleScrollView::create", AvatarTitleScrollView::create);

Self *Self::construct(Self *self) {
    Addr me = rw::guest_addr(self);
    call(g::Layout_ctor, {me});
    // IScrollProtocol
    rw::set<float>(self, 0x4FC, 0.1f);
    rw::set<float>(self, 0x500, 0.3f);
    rw::set<uint32_t>(self, 0x504, 0);
    rw::set<uint32_t>(self, 0x508, 0);
    rw::set<uint32_t>(self, 0x50C, 0);
    rw::set<float>(self, 0x510, 200.0f);
    call(g::aeabi_memset, {me + 0x514, 0x24, 0});
    rw::set<uint32_t>(self, 0x538, 1);
    rw::set<uint8_t>(self, 0x53C, 1);
    rw::set<uint32_t>(self, 0x590, 0);
    rw::set<uint8_t>(self, 0x594, 0);
    rw::set<uint32_t>(self, 0x5C4, 0);
    rw::set<uint32_t>(self, 0x5D8, 0);
    rw::set<uint32_t>(self, 0x5F0, 0);
    rw::set<uint32_t>(self, 0x608, 0);
    rw::set<uint32_t>(self, 0x620, 0);
    call(g::aeabi_memset, {me + 0x540, 0x12, 0});
    call(g::aeabi_memset, {me + 0x554, 0x12, 0});
    call(g::aeabi_memset, {me + 0x568, 0x25, 0});
    call(g::aeabi_memset, {me + 0x598, 0x2A, 0});
    // BaseScrollView<int>
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    // AvatarTitleScrollView
    rw::set<uint32_t>(self, 0, VTABLE);
    rw::set<uint32_t>(self, 0x280, VTABLE2);
    rw::set<uint32_t>(self, 0x350, VTABLE3);
    rw::set<uint32_t>(self, 0x648, 0);      // empty onSelect
    self->touching = 0;
    self->selected = 0;
    self->tapped = 0;
    rw::set<uint32_t>(self, 0x660, 0);
    self->tapEnabled = 0;
    return self;
}
RW_FUNCTION(0x510A98, "AvatarTitleScrollView::AvatarTitleScrollView", AvatarTitleScrollView::construct);

uint32_t Self::init(Self *self) {
    Addr me = rw::guest_addr(self);
    rw::set<float>(self, ITEM_WIDTH, 469.0f);
    rw::set<float>(self, ITEM_HEIGHT, 67.0f);
    call(g::IScrollProtocol_setEnabledScrollBar, {me, 1});
    self->tapEnabled = 1;
    Addr panel = call(g::Widget_create, {});
    self->panel = panel;
    {
        Frame f;
        Addr size = f.alloc(8);
        call(g::Size_ctor, {size, ld32(me + ITEM_WIDTH), ld32(me + ITEM_HEIGHT)});
        vcall(panel, node::setContentSize, {panel, size});
    }
    panel = self->panel;
    vcall(panel, node::setAnchorPoint, {panel, VEC2_ANCHOR_MIDDLE});
    call(sfn::Ref_retain, {self->panel});
    Addr layout = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_PanelJson});
    {
        Frame f;
        Addr pos = f.alloc(8);
        float x = rw::load<float>(me + ITEM_WIDTH) * 0.5f;
        call(g::Vec2_ctor, {pos, rw::fbits(x), 0});
        vcall(layout, node::setPosition, {layout, pos});
        call(g::Vec2_dtor, {pos});
    }
    Addr p = self->panel;
    withStr(S_mainPanel, 9, [&](Addr name) { vcall(p, node::addChildZName, {p, layout, 0, name}); });
    Addr button = search(layout, S_Button, 6);
    vcall(button, SLOT_setTouchEnabled, {button, 0});
    return call(g::BaseScrollView_int_init, {me});
}
RW_FUNCTION(0x510BF0, "AvatarTitleScrollView::init", AvatarTitleScrollView::init);

void Self::update(Self *self, float dt) { call(g::IScrollProtocol_update, {rw::guest_addr(self), rw::fbits(dt)}); }
RW_FUNCTION(0x510DE4, "AvatarTitleScrollView::update", AvatarTitleScrollView::update);

Addr Self::getLayoutPanel(Self *self, int32_t) { return call(g::Widget_clone, {self->panel}); }
RW_FUNCTION(0x510DEC, "AvatarTitleScrollView::getLayoutPanel", AvatarTitleScrollView::getLayoutPanel);

Addr Self::createRow(Self *self, uint32_t index, int32_t column) {
    Addr me = rw::guest_addr(self);
    if (!(itemCount(self) > index)) call(g::throwOutOfRange, {me + 0x628});
    Addr row = call(g::Widget_clone, {self->panel});
    vcall(me, SLOT_changeRow, {me, row, index, uint32_t(column)});
    return row;
}
RW_FUNCTION(0x510DFC, "AvatarTitleScrollView::createRow", AvatarTitleScrollView::createRow);

// Fill a row: the title's board image and name ("no title" text and image 0 for id 0). 1 = the row has no board.
uint32_t Self::changeRow(Self *self, Addr row, uint32_t index, int32_t) {
    Addr me = rw::guest_addr(self);
    Addr ids = self->itemIDs.begin;
    if (!(itemCount(self) > index)) call(g::throwOutOfRange, {me + 0x628});
    Frame f;
    Addr name = f.alloc(12);
    {
        Addr text = f.alloc(8);
        call(g::TextData_get, {text, 0, uint32_t(TEXT_NO_TITLE)});
        Addr chars = ld32(ld32(text) + 4);
        strInit(name, chars, host_strlen(chars));
        sharedRelease(text);
    }
    uint32_t titleId = ld32(ids + 4 * index);
    uint32_t image = 0;
    if (titleId) {
        Addr title = f.alloc(8);
        call(g::Title_create, {title, titleId});
        Addr buf = f.alloc(0x48);
        Addr sb = ld32(title);
        rw::call_exact(g::SafeBuffer_read, {sb, buf, ld32(sb + 12) + TITLE_NAME, 0x41, TITLE_NAME});
        call(g::STRING_ASSIGN, {name, buf, host_strlen(buf)});
        Addr imageBuf = f.alloc(4);
        sb = ld32(title);
        rw::call_exact(g::SafeBuffer_read, {sb, imageBuf, ld32(sb + 12) + TITLE_IMAGE, 4, TITLE_IMAGE});
        image = ld32(imageBuf);
        sharedRelease(title);
    }
    uint32_t result = 1;
    Addr board = search(row, S_RankTitle_Board, 15);
    if (board) {
        vcall(board, node::setVisible, {board, 1});
        Addr path = f.alloc(12);
        call(g::SceneUtil_createUserTitleImagePath, {path, image});
        withStr(strData(path), host_strlen(strData(path)),
                [&](Addr copy) { call(g::ImageView_loadTexture, {board, copy, 0}); });
        strFree(path);
        Addr label = search(row, S_RankTitle_Name, 14);
        call(g::SceneUtil_setUserTitleOutline, {label});
        result = 0;
        call(fn::Text_setString, {label, name});
    }
    strFree(name);
    return result;
}
RW_FUNCTION(0x510E54, "AvatarTitleScrollView::changeRow", AvatarTitleScrollView::changeRow);

void Self::registItemTouchEvent(Self *self, Addr, int32_t index) {
    Addr me = rw::guest_addr(self);
    Addr dispatcher = ld32(call(g::Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
    Addr listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    withLambda(VT_touchBegan, {me, uint32_t(index)},
               [&](Addr fo) { call(g::FUNC_TOUCH_BOOL_SWAP, {fo, listener + ON_TOUCH_BEGAN}); });
    Frame f;
    Addr captures = f.alloc(12);        // $_2's captures {this, index, this}
    st32(captures, me);
    st32(captures + 4, uint32_t(index));
    st32(captures + 8, me);
    {
        Addr moved = f.alloc(24);
        st32(moved + 16, 0);
        Addr heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_touchMoved);
            st32(heap + 4, me);
            st32(heap + 8, uint32_t(index));
            st32(heap + 0xC, me);
            st32(heap + 0x10, uint32_t(index));
            st32(heap + 0x14, me);
        }
        st32(moved + 16, heap);
        call(g::FUNC_TOUCH_VOID_SWAP, {moved, listener + ON_TOUCH_MOVED});
        functionDestroy(moved);
    }
    assignTouchEnded(listener + ON_TOUCH_ENDED, captures);
    assignTouchEnded(listener + ON_TOUCH_CANCELLED, captures);
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, me});
}
RW_FUNCTION(0x511190, "AvatarTitleScrollView::registItemTouchEvent", AvatarTitleScrollView::registItemTouchEvent);

void Self::setEnabledTapItem(Self *self, uint32_t enabled) { self->tapEnabled = uint8_t(enabled); }
RW_FUNCTION(0x511348, "AvatarTitleScrollView::setEnabledTapItem", AvatarTitleScrollView::setEnabledTapItem);

// ------------------------------------------------------------------------------------------------ lambdas
static void assignTouchEndedFn(Addr member, Addr captures) { assignTouchEnded(member, captures); }
RW_FUNCTION(0x5112D0, "AvatarTitleScrollView::registItemTouchEvent $_2 function<void(Touch*, Event*)>::operator=",
            assignTouchEndedFn);

static void touchEndedLambda(Addr func, const uint32_t *touchArg) { touchEndedBody(func + 4, *touchArg); }
RW_FUNCTION(0x5113BE, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::operator()", touchEndedLambda);

static void touchEndedBodyFn(Addr lambda, Addr touch) { touchEndedBody(lambda, touch); }
RW_FUNCTION(0x511400, "AvatarTitleScrollView::registItemTouchEvent $_2::operator()", touchEndedBodyFn);

// $_3 {this}: the tap animation ended: report the selected row (when a row was touched and taps are enabled)
static void tapDoneLambda(Addr func) {
    Self *self = rw::host_ptr<Self>(ld32(func + 4));
    Addr me = rw::guest_addr(self);
    if (ld32(me + 0x648) && self->touching && self->tapEnabled)
        call(g::FUNC_VI_CALL, {me + 0x638, uint32_t(self->selected)});
    self->tapped = 0;
    self->touching = 0;
}
RW_FUNCTION(0x511668, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::operator()", tapDoneLambda);

// $_1 {this, index, $_2}: the finger moved: leaving the row's main panel cancels the touch (as if lifted).
static void touchMovedLambda(Addr func, const uint32_t *touchArg) {
    const Addr touch = *touchArg;
    Self *self = rw::host_ptr<Self>(ld32(func + 4));
    Addr me = rw::guest_addr(self);
    if (!self->touching) return;
    Addr widget = ld32(ld32(me + ROW_WIDGETS) + 4 * ld32(func + 8));
    Addr panel = search(widget, S_mainPanel, 9);
    if (!panel) return;
    Frame f;
    Addr loc = f.alloc(8);
    Addr local = f.alloc(8);
    Addr rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {local, panel, loc});
    call(g::Vec2_dtor, {loc});
    uint32_t width = ld32(vcall(panel, node::getContentSize, {panel}));
    uint32_t height = ld32(vcall(panel, node::getContentSize, {panel}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, width, height});
    if (!call(g::Rect_containsPoint, {rect, local})) {
        self->touching = 0;
        touchEndedBody(func + 0xC, touch);
    }
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {local});
}
RW_FUNCTION(0x511754, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::operator()", touchMovedLambda);
