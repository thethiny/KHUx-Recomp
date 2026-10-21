// BirthRegisterScrollView: the year / month pull-down list of the birthday popup (BaseScrollView<int> of half-width
// cells "HalfContent" with a number). A tap on a row (without moving more than 10 points) picks its value: the
// row's tap animation, then the view's callback (+0x638) with the value.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_release = 0x8FD0BD, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Size_ctor = 0x8DF027, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B, Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Widget_create = 0x825875, Widget_clone = 0x8272B9;
constexpr uint32_t Touch_getLocation = 0x900E57, Node_convertToNodeSpace = 0x89599D, Node_runAction = 0x894AED;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionBTE_swap = 0x483E4D, FunctionVTE_swap = 0x4F3699, FunctionVI_call = 0x4F321D;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1, IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t BaseScrollViewInt_init = 0x4FA8A1;
constexpr uint32_t VectorInt_dtor = 0x478961, ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t aeabi_memset = 0x40E7A0, divsi3 = 0xCD5E8D;
constexpr uint32_t BirthRegisterScrollView_construct = 0x562311, BirthRegisterScrollView_assignEnded = 0x562AD9;
constexpr uint32_t BirthRegisterScrollView_tapped = 0x562C21;
constexpr uint32_t GOT_VT_BirthRegisterScrollView = 0xFD6664, GOT_VT_BaseScrollView_int = 0xFD6598;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054, GOT_ROW_SPACING = 0xFD659C;
constexpr uint32_t VT2_OFFSET = 0x360, VT3_OFFSET = 0x374;
constexpr uint32_t VT_Began = 0xF58E08;                    // $_1 {view, row}
constexpr uint32_t VT_Moved = 0xF58DDC;                    // $_2 {view, row} x3 (heap)
constexpr uint32_t VT_Ended = 0xF58D84;                    // $_3 {view, row, view, row} (heap)
constexpr uint32_t VT_TapDone = 0xF58DB0;                  // $_4 {view, row}
constexpr uint32_t VT_RowTouch = 0xF58E34;                 // changeRow $_0 {view, row widget, value}
constexpr uint32_t S_HalfContentJson = 0xDFF0D7, S_TxtNumLabel = 0xDFD8DC, S_IntFormat = 0xDF86DE;
constexpr uint32_t S_HalfContent = 0xDFEDEF, S_HalfContentImageButton = 0xDFEDFB, S_HalfContentBut = 0xDFEE13;
constexpr uint32_t V_deleting = 0x04, V_retainTouch = 0x0C, V_setAnchorPoint = 0x9C, V_setContentSize = 0xA8;
constexpr uint32_t V_getContentSize = 0xAC, V_isEnabled = 0xB4, V_addChild = 0xF0, V_addChildNamed = 0xFC;
constexpr uint32_t V_init = 0x248, V_changeRow = 0x340;
constexpr uint32_t SIZE = 0x668;
// BaseScrollView / IScrollProtocol fields
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518, F_ROWS = 0x51C, F_COLUMNS = 0x538, F_TOUCH_ON = 0x53C;
constexpr uint32_t F_SCROLL = 0x540, F_HALF_W = 0x570, F_ITEM_IDS = 0x628;
// BirthRegisterScrollView
constexpr uint32_t F_ON_PICK = 0x638, F_SELECTED = 0x650, F_TEMPLATE = 0x654, F_TAP_ENABLED = 0x658;
constexpr uint32_t F_PRESSED = 0x659, F_PRESS_INDEX = 0x65C, F_BUSY = 0x660, F_PRESS_Y = 0x664;

uint32_t count(uint32_t s) { return uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 2); }

// is the touch inside `node`'s content rect? `local` receives the point in node space (destroyed by the caller)
bool touchIn(uint32_t node, uint32_t touch, uint32_t local, uint32_t rect) {
    Frame f;
    const uint32_t at = f.alloc(8);
    call(Touch_getLocation, {at, touch});
    call(Node_convertToNodeSpace, {local, node, at});
    call(Vec2_dtor, {at});
    const uint32_t w = ld32(vcall(node, V_getContentSize, {node}));
    const uint32_t h = ld32(vcall(node, V_getContentSize, {node}) + 4);
    call(Rect_ctor, {rect, 0, 0, w, h});
    return call(Rect_containsPoint, {rect, local}) != 0;
}
}  // namespace

struct BirthRegisterScrollView {
    static uint32_t create(uint32_t);
    static uint32_t construct(uint32_t s);
    static uint32_t destruct(uint32_t s);
    static uint32_t init(uint32_t s);
    static uint32_t createRow(uint32_t s, uint32_t index, uint32_t arg);
    static uint32_t changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t arg);
    static uint32_t registItemTouchEvent(uint32_t s, uint32_t widget, uint32_t index);
    static uint32_t assignEnded(uint32_t dst, uint32_t captures);
    static uint32_t tapped(uint32_t captures, uint32_t touch);
    static uint32_t tapDone(uint32_t functor);
    static uint32_t moved(uint32_t functor, uint32_t touchRef);
    static uint32_t began(uint32_t functor, uint32_t touchRef);
    static uint32_t rowTouched(uint32_t functor, uint32_t, uint32_t typeRef);
};

uint32_t BirthRegisterScrollView::create(uint32_t) {
    const uint32_t s = call(fn::operator_new, {SIZE});
    call(BirthRegisterScrollView_construct, {s});
    if (vcall(s, V_init, {s}) == 1) {
        call(Ref_autorelease, {s});
        return s;
    }
    vcall(s, V_deleting, {s});
    return 0;
}

uint32_t BirthRegisterScrollView::construct(uint32_t s) {
    auto *base = rw::host_ptr<BaseScrollView>(s);
    call(Layout_ctor, {s});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    const uint32_t vt = ld32(GOT_VT_BirthRegisterScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    st32(s + F_ON_PICK + 0x10, 0);
    st32(s + F_SELECTED, 0);
    st8(s + F_TAP_ENABLED, 0);
    st8(s + F_PRESSED, 0);
    st32(s + F_PRESS_INDEX, 0);
    st8(s + F_BUSY, 0);
    st32(s + F_PRESS_Y, 0);
    return s;
}

uint32_t BirthRegisterScrollView::destruct(uint32_t s) {
    const uint32_t vt = ld32(GOT_VT_BirthRegisterScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    if (ld32(s + F_TEMPLATE)) call(Ref_release, {ld32(s + F_TEMPLATE)});
    functionDestroy(s + F_ON_PICK);
    const uint32_t bvt = ld32(GOT_VT_BaseScrollView_int);
    st32(s, bvt + 8);
    st32(s + 0x280, bvt + VT2_OFFSET);
    st32(s + 0x350, bvt + VT3_OFFSET);
    call(VectorInt_dtor, {s + F_ITEM_IDS});
    call(IScrollProtocol_dtor, {s});
    return s;
}

// cells 330x70; the template is a widget of that size holding BirthRegister_HalfContent
uint32_t BirthRegisterScrollView::init(uint32_t s) {
    stf(s + F_ITEM_W, 330.0f);
    stf(s + F_ITEM_H, 70.0f);
    call(IScrollProtocol_setEnabledScrollBar, {s, 1});
    call(IScrollProtocol_setEnabledScroll, {s, 0});
    const float half = ldf(s + F_ITEM_W) * 0.5f;
    stf(s + F_HALF_W, half);
    const uint32_t widget = call(Widget_create, {fbits(half)});
    st32(s + F_TEMPLATE, widget);
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(Size_ctor, {size, ld32(s + F_ITEM_W), ld32(s + F_ITEM_H)});
        vcall(widget, V_setContentSize, {widget, size});
    }
    vcall(ld32(s + F_TEMPLATE), V_setAnchorPoint, {ld32(s + F_TEMPLATE), ld32(GOT_ANCHOR_MIDDLE)});
    call(Ref_retain, {ld32(s + F_TEMPLATE)});
    const uint32_t content =
        call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_HalfContentJson});
    vcall(ld32(s + F_TEMPLATE), V_addChild, {ld32(s + F_TEMPLATE), content});
    return call(BaseScrollViewInt_init, {s});
}

uint32_t BirthRegisterScrollView::createRow(uint32_t s, uint32_t index, uint32_t arg) {
    if (count(s) <= index) return call(ThrowOutOfRange, {s + F_ITEM_IDS});
    const uint32_t row = call(Widget_clone, {ld32(s + F_TEMPLATE)});
    vcall(s, V_changeRow, {s, row, index, arg});
    return row;
}

// a row: its number, and the image button over the cell (made once) that picks it
uint32_t BirthRegisterScrollView::changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
    const uint32_t ids = ld32(s + F_ITEM_IDS);
    if (count(s) <= index) return call(ThrowOutOfRange, {s + F_ITEM_IDS});
    if (!findNode(widget, S_TxtNumLabel, 0xD)) return 1;
    SceneUtil::setShadow(findNode(widget, S_TxtNumLabel, 0xD));
    const uint32_t text = findNode(widget, S_TxtNumLabel, 0xD);
    SceneUtil::setShadow(text);
    const uint32_t value = ids + 4 * index;
    {
        Frame f;
        setTextC(text, stringBuffer(f, {S_IntFormat, ld32(value)}));
    }
    const uint32_t content = findNode(widget, S_HalfContent, 0xB);
    uint32_t button = findNode(widget, S_HalfContentImageButton, 0x17);
    if (!button) {
        {
            Frame f;
            const uint32_t name = newStr(f, S_HalfContentBut, 0xF);
            button = call(ImageButton_create, {name, 0});
            strFree(name);
        }
        call(ImageButton_setTargetView, {button, content, 0x3F800000});
        Frame f;
        const uint32_t name = newStr(f, S_HalfContentImageButton, 0x17);
        vcall(content, V_addChildNamed, {content, button, 0, name});
        strFree(name);
    }
    Frame f;
    const uint32_t cb = newFunction(f, VT_RowTouch, {s, widget, ld32(value)});
    call(ImageButton_addTouchEventListener, {button, cb});
    functionDestroy(cb);
    return 0;
}

uint32_t BirthRegisterScrollView::registItemTouchEvent(uint32_t s, uint32_t widget, uint32_t index) {
    if (!ld8(s + F_TAP_ENABLED)) return 0;
    const uint32_t dispatcher = ld32(call(Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    const uint32_t beganFn = newFunction(f, VT_Began, {s, index});
    call(FunctionBTE_swap, {beganFn, listener + 0x50});
    functionDestroy(beganFn);
    const uint32_t captures = f.alloc(16);
    st32(captures, s);
    st32(captures + 4, index);
    st32(captures + 8, s);
    st32(captures + 12, index);
    const uint32_t movedFn = f.alloc(20);
    st32(movedFn + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, VT_Moved);
        for (uint32_t w = 4; w < 0x1C; w += 8) {
            st32(obj + w, s);
            st32(obj + w + 4, index);
        }
    }
    st32(movedFn + 16, obj);
    call(FunctionVTE_swap, {movedFn, listener + 0x68});
    functionDestroy(movedFn);
    call(BirthRegisterScrollView_assignEnded, {listener + 0x80, captures});
    call(BirthRegisterScrollView_assignEnded, {listener + 0x98, captures});
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, s});
    return 0;
}

// sub_562AD8: std::function<void(Touch*, Event*)> = $_3 {view, row, view, row} (heap, by swap)
uint32_t BirthRegisterScrollView::assignEnded(uint32_t dst, uint32_t captures) {
    Frame f;
    const uint32_t tmp = f.alloc(20);
    st32(tmp + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, VT_Ended);
        for (uint32_t w = 0; w < 16; w += 4) st32(obj + 4 + w, ld32(captures + w));
    }
    st32(tmp + 16, obj);
    call(FunctionVTE_swap, {tmp, dst});
    functionDestroy(tmp);
    return 0;
}

// sub_562C20 (captures [view, row, view, row], touch): released - the pressed item (row + the scrolled rows);
// unless the touch moved 10 points or more, or the item is 0, the row plays its tap animation, then $_4
uint32_t BirthRegisterScrollView::tapped(uint32_t captures, uint32_t touch) {
    const uint32_t s = ld32(captures);
    const float rowH = ldf(s + F_ITEM_H) + ldf(ld32(GOT_ROW_SPACING));
    const int32_t scrolled = f2i(ldf(s + F_SCROLL));
    const int32_t rows = int32_t(call(divsi3, {uint32_t(scrolled), uint32_t(f2i(rowH))}));
    const uint32_t item = ld32(captures + 4) + ld32(s + F_COLUMNS) * uint32_t(rows) - 1;
    st32(s + F_PRESS_INDEX, item);
    auto clear = [&] {
        st8(s + F_PRESSED, 0);
        return 0u;
    };
    if (count(s) <= item || !ld8(s + F_PRESSED)) return clear();
    Frame f;
    const uint32_t at = f.alloc(8);
    call(Touch_getLocation, {at, touch});
    const float dy = ldf(at + 4) - ldf(s + F_PRESS_Y);
    if (!(bitsf(fbits(dy) & 0x7FFFFFFFu) < 10.0f)) {
        call(Vec2_dtor, {at});
        return clear();
    }
    const uint32_t pressed = ld32(s + F_PRESS_INDEX);
    if (count(s) <= pressed) return call(ThrowOutOfRange, {s + F_ITEM_IDS});
    const uint32_t value = ld32(ld32(s + F_ITEM_IDS) + 4 * pressed);
    call(Vec2_dtor, {at});
    if (!value) return clear();
    st8(s + F_BUSY, 1);
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(captures + 4));
    vcall(row, V_retainTouch, {row, 1});
    const uint32_t done = newFunction(f, VT_TapDone, {ld32(captures + 8), ld32(captures + 12)});
    call(Node_runAction, {row, call(SceneUtil_createTapAnimation, {row, done})});
    functionDestroy(done);
    return 0;
}

// $_4: the tap animation ended - the value goes to the callback
uint32_t BirthRegisterScrollView::tapDone(uint32_t functor) {
    const uint32_t s = ld32(functor + 4);
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(functor + 8));
    vcall(row, V_retainTouch, {row, 0});
    if (ld32(s + F_ON_PICK + 0x10) && ld8(s + F_PRESSED)) call(FunctionVI_call, {s + F_ON_PICK, ld32(s + F_PRESS_INDEX)});
    st8(s + F_BUSY, 0);
    st8(s + F_PRESSED, 0);
    return F_BUSY;
}

// $_2: moved off the row - the press ends there
uint32_t BirthRegisterScrollView::moved(uint32_t functor, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(functor + 4);
    if (!ld8(s + F_PRESSED)) return 0;
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(functor + 8));
    Frame f;
    const uint32_t local = f.alloc(8), rect = f.alloc(16);
    if (!touchIn(row, touch, local, rect)) {
        st8(s + F_PRESSED, 0);
        call(BirthRegisterScrollView_tapped, {functor + 0xC, touch});
    }
    call(Vec2_dtor, {rect});
    call(Vec2_dtor, {local});
    return 0;
}

// $_1: a press on the row (when no tap is running) - its height is kept, the press is on
uint32_t BirthRegisterScrollView::began(uint32_t functor, uint32_t touchRef) {
    const uint32_t s = ld32(functor + 4);
    if (ld8(s + F_BUSY)) return 0;
    const uint32_t touch = ld32(touchRef);
    {
        Frame f;
        const uint32_t at = f.alloc(8);
        call(Touch_getLocation, {at, touch});
        st32(s + F_PRESS_Y, ld32(at + 4));
        call(Vec2_dtor, {at});
    }
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(functor + 8));
    Frame f;
    const uint32_t local = f.alloc(8), rect = f.alloc(16);
    uint32_t result = 0;
    if (touchIn(row, touch, local, rect)) {
        result = 1;
        st8(s + F_PRESSED, 1);
    }
    call(Vec2_dtor, {rect});
    call(Vec2_dtor, {local});
    return result;
}

// changeRow $_0: the row's button released (touch on, button enabled) - the value selected and passed on. r0 is
// what the original leaves there on each path
uint32_t BirthRegisterScrollView::rowTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    const uint32_t s = ld32(functor + 4);
    if (ld32(typeRef) != 2) return F_TOUCH_ON;
    if (!ld8(s + F_TOUCH_ON)) return 0;
    const uint32_t button = ld32(functor + 8);
    const uint32_t enabled = vcall(button, V_isEnabled, {button});
    if (enabled != 1) return enabled;
    st32(s + F_SELECTED, ld32(functor + 0xC));
    if (!ld32(s + F_ON_PICK + 0x10)) return 0;
    return call(FunctionVI_call, {s + F_ON_PICK, ld32(functor + 0xC)});
}

RW_FUNCTION(0x562158, "BirthRegisterScrollView::~BirthRegisterScrollView", BirthRegisterScrollView::destruct);
RW_FUNCTION(0x5622BC, "BirthRegisterScrollView::create", BirthRegisterScrollView::create);
RW_FUNCTION(0x562310, "BirthRegisterScrollView::BirthRegisterScrollView", BirthRegisterScrollView::construct);
RW_FUNCTION(0x562470, "BirthRegisterScrollView::init", BirthRegisterScrollView::init);
RW_FUNCTION(0x562584, "BirthRegisterScrollView::createRow", BirthRegisterScrollView::createRow);
RW_FUNCTION(0x5625E0, "BirthRegisterScrollView::changeRow", BirthRegisterScrollView::changeRow);
RW_FUNCTION(0x562990, "BirthRegisterScrollView::registItemTouchEvent", BirthRegisterScrollView::registItemTouchEvent);
RW_FUNCTION(0x562AD8, "BirthRegisterScrollView::registItemTouchEvent $_3 assign (sub_562AD8)",
            BirthRegisterScrollView::assignEnded);
RW_FUNCTION(0x562C20, "BirthRegisterScrollView::registItemTouchEvent tapped (sub_562C20)", BirthRegisterScrollView::tapped);
RW_FUNCTION(0x562E5C, "BirthRegisterScrollView::registItemTouchEvent::$_4 operator()", BirthRegisterScrollView::tapDone);
RW_FUNCTION(0x562F5C, "BirthRegisterScrollView::registItemTouchEvent::$_2 operator()", BirthRegisterScrollView::moved);
RW_FUNCTION(0x563100, "BirthRegisterScrollView::registItemTouchEvent::$_1 operator()", BirthRegisterScrollView::began);
RW_FUNCTION(0x5632C8, "BirthRegisterScrollView::changeRow::$_0 operator()", BirthRegisterScrollView::rowTouched);
