// HelpScrollView: the topic list of the Help popup (Other menu > Help). A BaseScrollView<int> of title rows
// (Help_Panel.json); the item id picks one of three titles (id % 3). A tap on a row (released within 10 pixels
// of where it went down) plays the tap animation; the animation's end ($_3, gen) opens the topic.
#include "TapRow.hpp"

using namespace scene;

namespace {
struct HelpScrollView : BaseScrollView {
    // +0x638 std::function (callable at +0x648), +0x650 the row template (retained), +0x654 rows take touches,
    // +0x655 a row is pressed, +0x658 the pressed item, +0x65C a tap is animating, +0x660 the touch's start y
    const uint8_t members[0x668 - 0x634];

    static constexpr uint32_t SIZE = 0x668;
    static constexpr uint32_t VTABLE = 0xF84D84, VTABLE2 = 0xF850DC, VTABLE3 = 0xF850F0;
    static constexpr uint32_t FUNCTION = 0x638;
    static constexpr uint32_t PANEL = 0x650;
    static constexpr uint32_t TOUCHABLE = 0x654;
    static constexpr uint32_t PRESSED = 0x655;
    static constexpr uint32_t PRESSED_ITEM = 0x658;
    static constexpr uint32_t TAPPED = 0x65C;
    static constexpr uint32_t TOUCH_Y = 0x660;
};
using Self = HelpScrollView;
RW_SIZE(HelpScrollView, HelpScrollView::SIZE);

namespace g {
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t Rect_ctor = 0x8DF18B;
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Touch_getLocation = 0x900E57;
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t string_ctor = 0x48D811;           // std::string(const char*, size_t)
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t MODSI3 = 0xCD6009;
constexpr uint32_t DIVSI3 = 0xCD5E8D;
}  // namespace g

constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
// __func vtables (+8)
constexpr uint32_t VT_TOUCH_BEGAN = 0xF56A1C;       // $_0 {view, row}
constexpr uint32_t VT_TOUCH_MOVED = 0xF569F0;       // $_1 {view, row} x3 (0x1C bytes, on the heap)
constexpr uint32_t VT_TOUCH_ENDED = 0xF56998;       // $_2 {view, row} x2 (0x14 bytes, on the heap)
constexpr uint32_t VT_TAP_DONE = 0xF569C4;          // $_3 {view, row}

constexpr uint32_t S_HELP_PANEL_JSON = 0xDFD902;    // "cocostudio/publish/Help_Panel.json"
constexpr uint32_t S_TXT_TITLE_LABEL = 0xDF9554;    // "Txt_Title_Label" (15)
constexpr uint32_t S_TITLE_0 = 0xDFD987, S_TITLE_1 = 0xDFD925, S_TITLE_2 = 0xDFD956, S_EMPTY = 0xDF8A1A;

constexpr uint32_t V_deleting = 0x04;
constexpr uint32_t V_retain = 0x0C;
constexpr uint32_t V_setPositionX = 0x64;
constexpr uint32_t V_setAnchorPoint = 0x9C;
constexpr uint32_t V_setContentSize = 0xA8;
constexpr uint32_t V_getContentSize = 0xAC;
constexpr uint32_t V_addChild = 0xF0;
constexpr uint32_t V_init = 0x248;
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;

inline uint32_t idCount(uint32_t me) { return (ld32(me + 0x62C) - ld32(me + 0x628)) >> 2; }
inline uint32_t fb(float f) { return rw::fbits(f); }

// the rect (0, 0, w, h) of a node
void localRect(uint32_t rect, uint32_t node) {
    float w = ldf(vcall(node, V_getContentSize, {node}));
    float h = ldf(vcall(node, V_getContentSize, {node}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, fb(w), fb(h)});
}
// is the touch inside `widget`?
bool touchInside(uint32_t touch, uint32_t widget) {
    Frame f;
    uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, widget, loc});
    call(g::Vec2_dtor, {loc});
    localRect(rect, widget);
    bool inside = call(g::Rect_containsPoint, {rect, p}) != 0;
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {p});
    return inside;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
namespace {
Self *construct(Self *self) {
    uint32_t me = self->self();
    call(g::Layout_ctor, {me});
    BaseScrollView::initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = Self::VTABLE;
    self->vtable2 = Self::VTABLE2;
    self->vtable3 = Self::VTABLE3;
    st32(me + Self::FUNCTION + 16, 0);
    st8(me + Self::TOUCHABLE, 0);
    st8(me + Self::PRESSED, 0);
    st32(me + Self::PRESSED_ITEM, 0);
    st8(me + Self::TAPPED, 0);
    st32(me + Self::TOUCH_Y, 0);
    return self;
}

uint32_t create() {
    uint32_t p = call(fn::operator_new, {Self::SIZE});
    construct(rw::host_ptr<Self>(p));
    if (vcall(p, V_init, {p}) == 1) {
        call(g::Ref_autorelease, {p});
        return p;
    }
    vcall(p, V_deleting, {p});
    return 0;
}

Self *destroy(Self *self) {
    uint32_t me = self->self();
    self->vtable = Self::VTABLE;
    self->vtable2 = Self::VTABLE2;
    self->vtable3 = Self::VTABLE3;
    if (uint32_t panel = ld32(me + Self::PANEL)) vcall(panel, V_deleting, {panel});
    st32(me + Self::PANEL, 0);
    functionDestroy(me + Self::FUNCTION);
    self->vtable = BaseScrollView::VTABLE_INT;
    self->vtable2 = BaseScrollView::VTABLE2_INT;
    self->vtable3 = BaseScrollView::VTABLE3_INT;
    call(g::VectorBaseInt_dtor, {me + 0x628});
    IScrollProtocol::destroy(self);
    return self;
}

// rows of 900 x 100 with a scroll bar; the row template is loaded once and kept
uint32_t init(Self *self) {
    uint32_t me = self->self();
    self->itemWidth = 900.0f;
    self->itemHeight = 100.0f;
    IScrollProtocol::setEnabledScrollBar(self, 1);
    self->offsetX = self->itemWidth * 0.5f;
    uint32_t reader = call(g::GUIReader_getInstance, {});
    uint32_t panel = call(g::GUIReader_widgetFromJsonFile, {reader, S_HELP_PANEL_JSON});
    st32(me + Self::PANEL, panel);
    call(g::Ref_retain, {panel});
    uint32_t p = ld32(me + Self::PANEL);
    vcall(p, V_widgetSetTouchEnabled, {p, 0});
    return BaseScrollView::initInt(self);
}

// ------------------------------------------------------------------------------------------------ rows
// a row: an empty widget (the item's size, centred) holding a clone of the template
uint32_t getLayoutPanel(Self *self) {
    uint32_t me = self->self();
    uint32_t widget = call(g::Widget_create, {});
    uint32_t clone = call(g::Widget_clone, {ld32(me + Self::PANEL)});
    vcall(widget, V_setAnchorPoint, {widget, ld32(GOT_ANCHOR_MIDDLE)});
    vcall(widget, V_addChild, {widget, clone});
    {
        Frame f;
        uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fb(self->itemWidth), fb(self->itemHeight)});
        vcall(widget, V_setContentSize, {widget, size});
    }
    vcall(widget, V_setPositionX, {widget, fb(self->itemWidth * 0.5f)});
    return widget;
}

uint32_t createRow(Self *self, uint32_t index, int32_t arg) {
    uint32_t me = self->self();
    if (idCount(me) <= index) call(g::THROW_OUT_OF_RANGE, {});
    uint32_t row = getLayoutPanel(self);
    vcall(me, IScrollProtocol::V_isReplaceItem, {me, row, index, uint32_t(arg)});
    return row;
}

// the row's title by item id % 3; 1 when the widget has no title label
uint32_t changeRow(Self *self, uint32_t widget, uint32_t index, int32_t) {
    uint32_t me = self->self();
    if (idCount(me) <= index) call(g::THROW_OUT_OF_RANGE, {});
    Frame f;
    uint32_t title = f.alloc(12);
    switch (int32_t(call(g::MODSI3, {ld32(self->itemIDs.begin + 4 * index), 3}))) {
    case 0: call(g::string_ctor, {title, S_TITLE_0, 0x30}); break;
    case 1: call(g::string_ctor, {title, S_TITLE_1, 0x30}); break;
    case 2: call(g::string_ctor, {title, S_TITLE_2, 0x30}); break;
    default: call(g::string_ctor, {title, S_EMPTY, 0}); break;
    }
    uint32_t label = findNode(widget, S_TXT_TITLE_LABEL, 15);
    uint32_t result = 1;
    if (label) {
        call(g::SceneUtil_setShadow, {label});
        call(g::Text_setString, {label, title});
        result = 0;
    }
    strFree(title);
    return result;
}

// sub_51C1C0: a touch-ended / cancelled slot gets $_2 {view, row, view, row}
void assignTouchEnded(uint32_t slot, uint32_t captures) { tap_row::assignTouchEnded(VT_TOUCH_ENDED, slot, captures); }

// only when rows take touches: began / moved / ended+cancelled on the view itself
void registItemTouchEvent(Self *self, uint32_t, int32_t index) {
    uint32_t me = self->self();
    if (!ld8(me + Self::TOUCHABLE)) return;
    uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    uint32_t row = uint32_t(index);
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_TOUCH_BEGAN, {me, row});
        call(g::FunctionTouchBool_swap, {fo, listener + 0x50});
        functionDestroy(fo);
    }
    Frame f;
    uint32_t caps = f.alloc(16);
    st32(caps, me);
    st32(caps + 4, row);
    st32(caps + 8, me);
    st32(caps + 12, row);
    {
        Frame g2;
        uint32_t fo = tap_row::newHeapFunction(g2, VT_TOUCH_MOVED, {me, row, me, row, me, row});
        call(g::FunctionTouchVoid_swap, {fo, listener + 0x68});
        functionDestroy(fo);
    }
    assignTouchEnded(listener + 0x80, caps);
    assignTouchEnded(listener + 0x98, caps);
    call(g::EventDispatcher_addListenerWithSceneGraphPriority, {dispatcher, listener, me});
}

// ------------------------------------------------------------------------------------------------ touch lambdas
// $_2's body (sub_51C314, captures {view, row, view, row}): the pressed item from the row and the scroll position;
// a release within 10 pixels of the start on an item with a non-zero id plays the row's tap animation ($_3 at
// its end), anything else ends the press
constexpr tap_row::Layout TAP{Self::PRESSED_ITEM, Self::PRESSED, Self::TAPPED, Self::TOUCH_Y, 4, VT_TAP_DONE};
void touchEndedBody(uint32_t captures, uint32_t touch) { tap_row::touchEnded(TAP, captures, touch); }

// $_0 {view, row}: unless a tap is animating, remembers where the touch went down; a touch inside the row
// presses it (handled)
uint32_t touchBegan(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t touch = ld32(touchRef);
    uint32_t me = ld32(functor + 4);
    if (ld8(me + Self::TAPPED)) return 0;
    {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        st32(me + Self::TOUCH_Y, ld32(loc + 4));
        call(g::Vec2_dtor, {loc});
    }
    uint32_t widget = ld32(ld32(me + 0x51C) + 4 * ld32(functor + 8));
    if (!touchInside(touch, widget)) return 0;
    st8(me + Self::PRESSED, 1);
    return 1;
}

// $_1 {view, row, view, row, view, row}: a pressed touch leaving its row ends the press (through $_2's body)
void touchMoved(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t touch = ld32(touchRef);
    uint32_t me = ld32(functor + 4);
    if (!ld8(me + Self::PRESSED)) return;
    uint32_t widget = ld32(ld32(me + 0x51C) + 4 * ld32(functor + 8));
    if (touchInside(touch, widget)) return;
    st8(me + Self::PRESSED, 0);
    touchEndedBody(functor + 0xC, touch);
}
}  // namespace

RW_FUNCTION(0x51BC0C, "HelpScrollView::HelpScrollView()", construct);
RW_FUNCTION(0x51BBB8, "HelpScrollView::create()", create);
RW_FUNCTION(0x51BAAC, "HelpScrollView::~HelpScrollView()", destroy);
RW_FUNCTION(0x51BD64, "HelpScrollView::init()", init);
RW_FUNCTION(0x51BDDC, "HelpScrollView::getLayoutPanel(int)", getLayoutPanel);
RW_FUNCTION(0x51BEA4, "HelpScrollView::createRow(int, int)", createRow);
RW_FUNCTION(0x51BEFC, "HelpScrollView::changeRow(Widget*, int, int)", changeRow);
RW_FUNCTION(0x51C078, "HelpScrollView::registItemTouchEvent(Widget*, int)", registItemTouchEvent);
RW_FUNCTION(0x51C1C0, "HelpScrollView::registItemTouchEvent touch ended setter (sub_51C1C0)", assignTouchEnded);
RW_FUNCTION(0x51C314, "HelpScrollView::registItemTouchEvent $_2 body (sub_51C314)", touchEndedBody);
RW_FUNCTION(0x51C7F4, "HelpScrollView::registItemTouchEvent $_0 operator() (sub_51C7F4)", touchBegan);
RW_FUNCTION(0x51C650, "HelpScrollView::registItemTouchEvent $_1 operator() (sub_51C650)", touchMoved);
