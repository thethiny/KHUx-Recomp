// ImageButton: an invisible cocos2d::Node that turns an image (a ui::ImageView, or any node) into a button. It owns a
// one-by-one touch listener; while pressed it swaps the image to "img/ui/<name>_On.png" (selectable buttons) and
// reports the touch phases to a std::function<void(Ref*, Widget::TouchEventType)> like a ui::Widget would.
// The hit area is the target's content size times targetScale, centred, shifted by the target's anchor point.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace {
using scene::call;
using scene::fbits;
using scene::ld32;
using scene::ldf;

constexpr uint32_t SIZE = 0x2D0;
constexpr uint32_t VTABLE = 0xF86660;                 // _ZTV11ImageButton + 8

constexpr uint32_t AEABI_MEMSET = 0x40E7A0;           // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t STRING_INIT = 0x47CD93;            // std::string::__init(const char*, size_t)
constexpr uint32_t STRING_ASSIGN = 0x498A1D;          // std::string::operator=(const string&)
constexpr uint32_t StringBuffer_ctor = 0x49140D;      // StringBuffer(fmt, ...): a 1 KB formatted buffer
constexpr uint32_t STRING_BUFFER_SIZE = 0x404;
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Node_dtor = 0x8927ED;
constexpr uint32_t Node_setVisible = 0x893341;
constexpr uint32_t Node_convertToNodeSpace = 0x89599D; // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t Vec2_ctor = 0x8E4B95;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B;              // Rect(x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Touch_getLocation = 0x900E57;      // (Vec2* out, Touch*)
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t ImageView_loadTexture = 0x839731;  // (const string&, TextureResType)
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t TouchOneByOne_create = 0x8FACA5;
constexpr uint32_t TouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t FunctionTouchBegan_swap = 0x483E4D;   // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouch_swap = 0x4F3699;        // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouchEvent_assign = 0x5315C1; // std::function<void(Ref*, TouchEventType)>::operator=
constexpr uint32_t FunctionTouchEvent_call = 0x531C5D;   // std::function<void(Ref*, TouchEventType)>::operator()
constexpr uint32_t GOT_ANCHOR_BOTTOM_LEFT = 0xFD6080;    // GOT slot: &cocos2d::Vec2::ANCHOR_BOTTOM_LEFT

// EventListenerTouchOneByOne callbacks (std::function members)
constexpr uint32_t LISTENER_ON_BEGAN = 0x50, LISTENER_ON_MOVED = 0x68, LISTENER_ON_ENDED = 0x80,
                   LISTENER_ON_CANCELLED = 0x98;
// ImageButton::init lambdas (__func vtables + 8)
constexpr uint32_t VT_ON_BEGAN = 0xF5746C;      // $_0 {this, this, this}
constexpr uint32_t VT_ON_MOVED = 0xF57440;      // $_1 {this, this, this}
constexpr uint32_t VT_ON_ENDED = 0xF57414;      // $_2 {this}
constexpr uint32_t VT_ON_CANCELLED = 0xF573E8;  // $_3 {this}

// Widget::TouchEventType
constexpr uint32_t TOUCH_MOVED = 1, TOUCH_ENDED = 2, TOUCH_CANCELED = 3;

constexpr uint32_t STR_DISABLE_PNG = 0xDFE193;  // "img/ui/%s_Disable.png"
constexpr uint32_t STR_ON_PNG = 0xDFE1A9;       // "img/ui/%s_On.png"
constexpr uint32_t STR_OFF_PNG = 0xDFE1BA;      // "img/ui/%s_Off.png"

constexpr uint32_t V_GET_CONTENT_SIZE = 0xAC, V_SET_VISIBLE = 0xB0;

uint32_t contentSize(uint32_t node) { return scene::vcall(node, V_GET_CONTENT_SIZE, {node}); }

}  // namespace

struct ImageButton {
    rw::Field<uint32_t> vtable;                   // +0x000 cocos2d::Node
    const uint8_t node[0x26C];
    rw::Field<uint8_t> pressed;                   // +0x270 a touch began on it and has not left
    rw::Field<uint8_t> disabled;                  // +0x271 shows "<name>_Disable.png"
    rw::Field<uint8_t> field_272;                 // +0x272 (1)
    const uint8_t pad_273[1];
    guest::String fileName;                       // +0x274 image name: img/ui/<name>_On/Off/Disable.png
    const uint8_t onTouchEvent[0x18];             // +0x280 std::function<void(Ref*, Widget::TouchEventType)>
    rw::Field<uint32_t> imageView;                // +0x298 ui::ImageView* whose texture follows the state
    rw::Field<uint32_t> targetView;               // +0x29C Node* whose area is the hit area (no image view)
    rw::Field<float> anchorX;                     // +0x2A0 target anchor point (ANCHOR_BOTTOM_LEFT)
    rw::Field<float> anchorY;                     // +0x2A4
    rw::Field<float> touchStartX;                 // +0x2A8 where the touch began
    rw::Field<float> touchStartY;                 // +0x2AC
    rw::Field<uint32_t> field_2B0;                // +0x2B0
    rw::Field<float> targetScale;                 // +0x2B4 hit area scale (1.0)
    rw::Field<float> moveTolerance;               // +0x2B8 pixels a touch may move before it cancels (50)
    guest::String field_2BC;                      // +0x2BC std::string
    rw::Field<uint8_t> selected;                  // +0x2C8 shows "<name>_On.png" when selectable
    rw::Field<uint8_t> selectEnabled;             // +0x2C9 selectable: On / Off images
    const uint8_t pad_2CA[6];

    static constexpr uint32_t ON_TOUCH_EVENT = 0x280;
    uint32_t self() const { return rw::guest_addr(this); }
    bool hasTouchEvent() const { return ld32(self() + ON_TOUCH_EVENT + 16) != 0; }
    void fireTouchEvent(uint32_t button, uint32_t type) const {
        call(FunctionTouchEvent_call, {self() + ON_TOUCH_EVENT, button, type});
    }

    static ImageButton *construct(ImageButton *b) {
        uint32_t s = b->self();
        call(Node_ctor, {s});
        b->vtable = VTABLE;
        b->disabled = 0;
        b->pressed = 0;
        b->field_272 = 1;
        scene::st32(s + 0x27C, 0);   // fileName (empty)
        scene::st32(s + 0x278, 0);
        scene::st32(s + 0x274, 0);
        scene::st32(s + ON_TOUCH_EVENT + 16, 0);   // onTouchEvent (empty)
        b->imageView = 0;
        b->targetView = 0;
        call(Vec2_ctor, {s + 0x2A0});
        call(Vec2_ctor, {s + 0x2A8});
        b->field_2B0 = 0;
        b->targetScale = 1.0f;
        b->moveTolerance = 50.0f;
        rw::store16(rw::host_ptr<uint8_t>(s + 0x2C8), 0);   // selected, selectEnabled (one halfword store)
        scene::st32(s + 0x2C4, 0);  // field_2BC (empty)
        scene::st32(s + 0x2C0, 0);
        scene::st32(s + 0x2BC, 0);
        return b;
    }

    // create(name, swallowTouches)
    static uint32_t create(const guest::String *name, uint32_t swallow) {
        uint32_t p = call(scene::fn::operator_new, {SIZE});
        call(AEABI_MEMSET, {p, SIZE, 0});
        construct(rw::host_ptr<ImageButton>(p));
        scene::Frame f;
        uint32_t copy = f.alloc(12);   // the by-value std::string argument
        uint32_t src = rw::guest_addr(name);
        if (name->isLong()) {
            call(STRING_INIT, {copy, ld32(src + 8), ld32(src + 4)});
        } else {
            rw::store32(rw::frame_ptr<uint32_t>(copy), ld32(src));
            rw::store32(rw::frame_ptr<uint32_t>(copy + 4), ld32(src + 4));
            rw::store32(rw::frame_ptr<uint32_t>(copy + 8), ld32(src + 8));
        }
        init(rw::host_ptr<ImageButton>(p), rw::host_ptr<guest::String>(copy), swallow);
        scene::strFree(copy);
        call(Ref_autorelease, {p});
        return p;
    }

    static uint32_t init(ImageButton *b, const guest::String *name, uint32_t swallow) {
        uint32_t s = b->self();
        call(STRING_ASSIGN, {s + 0x274, rw::guest_addr(name)});
        uint32_t anchor = ld32(GOT_ANCHOR_BOTTOM_LEFT);
        uint32_t ax = ld32(anchor), ay = ld32(anchor + 4);
        scene::st32(s + 0x2A4, ay);
        scene::st32(s + 0x2A0, ax);
        uint32_t listener = call(TouchOneByOne_create, {});
        call(TouchOneByOne_setSwallowTouches, {listener, swallow});
        {
            scene::Frame f;
            uint32_t fo = scene::newFunction(f, VT_ON_BEGAN, {s, s, s});
            call(FunctionTouchBegan_swap, {fo, listener + LISTENER_ON_BEGAN});
            scene::functionDestroy(fo);
        }
        {
            scene::Frame f;
            uint32_t fo = scene::newFunction(f, VT_ON_MOVED, {s, s, s});
            call(FunctionTouch_swap, {fo, listener + LISTENER_ON_MOVED});
            scene::functionDestroy(fo);
        }
        {
            scene::Frame f;
            uint32_t fo = scene::newFunction(f, VT_ON_ENDED, {s});
            call(FunctionTouch_swap, {fo, listener + LISTENER_ON_ENDED});
            scene::functionDestroy(fo);
        }
        {
            scene::Frame f;
            uint32_t fo = scene::newFunction(f, VT_ON_CANCELLED, {s});
            call(FunctionTouch_swap, {fo, listener + LISTENER_ON_CANCELLED});
            scene::functionDestroy(fo);
        }
        uint32_t dispatcher = ld32(call(Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
        call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, s});
        return 1;
    }

    static void addTouchEventListener(ImageButton *b, uint32_t function) {
        call(FunctionTouchEvent_assign, {b->self() + ON_TOUCH_EVENT, function});
    }

    // the image view's texture for the current state: Disable, On (selected) or Off
    static void updateImage(ImageButton *b) {
        uint32_t view = b->imageView;
        if (!view) return;
        uint32_t fmt;
        if (b->disabled) fmt = STR_DISABLE_PNG;
        else if (b->selectEnabled) fmt = b->selected ? STR_ON_PNG : STR_OFF_PNG;
        else fmt = STR_OFF_PNG;
        uint32_t name = b->fileName.data();
        scene::Frame f;
        uint32_t buffer = f.alloc(STRING_BUFFER_SIZE);
        uint32_t path = f.alloc(12);
        call(StringBuffer_ctor, {buffer, fmt, name});
        call(STRING_INIT, {path, buffer, scene::host_strlen(buffer)});
        call(ImageView_loadTexture, {view, path, 0});
        scene::strFree(path);
    }

    static void setTargetImageView(ImageButton *b, uint32_t view) {
        b->imageView = view;
        updateImage(b);
    }
    static ImageButton *setTargetView(ImageButton *b, uint32_t view, float scale) {
        b->targetView = view;
        b->targetScale = scale;
        return b;
    }
    static ImageButton *setTargetAnchorPoint(ImageButton *b, const float *anchor) {
        float x = anchor[0], y = anchor[1];
        b->anchorY = y;
        b->anchorX = x;
        return b;
    }
    static void setIsDisabled(ImageButton *b, uint32_t disabled) {
        b->disabled = uint8_t(disabled);
        updateImage(b);
    }
    static ImageButton *setSelectEnabled(ImageButton *b, uint32_t enabled) {
        b->selectEnabled = uint8_t(enabled);
        return b;
    }
    static void setIsSelected(ImageButton *b, uint32_t selected) {
        if (uint32_t(b->selected) == selected) return;   // the byte against the whole argument register
        b->selected = uint8_t(selected);
        updateImage(b);
    }
    static void setFileName(ImageButton *b, const guest::String *name) {
        call(STRING_ASSIGN, {b->self() + 0x274, rw::guest_addr(name)});
        updateImage(b);
    }
    // Node::setVisible, and the image view follows
    static void setVisible(ImageButton *b, uint32_t visible) {
        call(Node_setVisible, {b->self(), visible});
        uint32_t view = b->imageView;
        if (view) scene::vcall(view, V_SET_VISIBLE, {view, visible});
    }

    static uint32_t destruct(ImageButton *b) {
        uint32_t s = b->self();
        b->vtable = VTABLE;
        scene::strFree(s + 0x2BC);
        call(Vec2_dtor, {s + 0x2A8});
        call(Vec2_dtor, {s + 0x2A0});
        scene::functionDestroy(s + ON_TOUCH_EVENT);
        scene::strFree(s + 0x274);
        call(Node_dtor, {s});
        return s;
    }
    static void destructDelete(ImageButton *b) {
        uint32_t s = destruct(b);
        call(scene::fn::operator_delete, {s});
    }

    // ------------------------------------------------------------------------------------------ touch lambdas
    // init::$_1 (onTouchMoved): moving further than moveTolerance from the start, or leaving the target's hit area,
    // cancels the press
    static void onTouchMoved(uint32_t lambda, const uint32_t *touchArg, uint32_t) {
        ImageButton *b = rw::host_ptr<ImageButton>(ld32(lambda + 4));
        if (!b->pressed) return;
        uint32_t touch = *touchArg;
        scene::Frame f;
        uint32_t start = f.alloc(8);
        call(Touch_getLocation, {start, touch});
        float tol = b->moveTolerance;
        bool moved = tol < std::fabs(b->touchStartX - ldf(start)) || tol < std::fabs(b->touchStartY - ldf(start + 4));
        if (b->hasTouchEvent()) b->fireTouchEvent(b->self(), TOUCH_MOVED);
        // the node whose area counts: the image view, else the target view, else the button itself
        uint32_t target = ld32(lambda + 8);
        ImageButton *t = rw::host_ptr<ImageButton>(target);
        if (t->imageView) target = t->imageView;
        else if (t->targetView) target = t->targetView;
        ImageButton *g = rw::host_ptr<ImageButton>(ld32(lambda + 12));
        float w = ldf(contentSize(target)) * g->targetScale;
        float h = ldf(contentSize(target) + 4) * g->targetScale;
        float dw = ldf(contentSize(target)) - w;
        float dh = ldf(contentSize(target) + 4) - h;
        uint32_t s5 = contentSize(target);
        float ax = g->anchorX;
        float w5 = ldf(s5);
        uint32_t s6 = contentSize(target);
        float x = dw * 0.5f + w5 * ax;
        float y = dh * 0.5f + ldf(s6 + 4) * g->anchorY;
        uint32_t rect = f.alloc(16), loc = f.alloc(8), local = f.alloc(8);
        call(Rect_ctor, {rect, fbits(x), fbits(y), fbits(w), fbits(h)});
        call(Touch_getLocation, {loc, touch});
        call(Node_convertToNodeSpace, {local, target, loc});
        call(Vec2_dtor, {loc});
        if (moved || !call(Rect_containsPoint, {rect, local})) {
            b->pressed = 0;
            updateImage(b);
            if (b->hasTouchEvent()) b->fireTouchEvent(b->self(), TOUCH_CANCELED);
        }
        call(Vec2_dtor, {local});
        call(Vec2_dtor, {rect});
        call(Vec2_dtor, {start});
    }

    // init::$_2 (onTouchEnded): a press that is still on reports ENDED (kept alive for the callback)
    static void onTouchEnded(uint32_t lambda, uint32_t, uint32_t) {
        ImageButton *b = rw::host_ptr<ImageButton>(ld32(lambda + 4));
        if (!b->pressed) return;
        call(Ref_retain, {b->self()});
        call(Ref_autorelease, {b->self()});
        if (b->hasTouchEvent()) b->fireTouchEvent(b->self(), TOUCH_ENDED);
        b->pressed = 0;
        updateImage(b);
    }

    // init::$_3 (onTouchCancelled)
    static void onTouchCancelled(uint32_t lambda, uint32_t, uint32_t) {
        ImageButton *b = rw::host_ptr<ImageButton>(ld32(lambda + 4));
        b->pressed = 0;
        updateImage(b);
        if (b->hasTouchEvent()) b->fireTouchEvent(b->self(), TOUCH_CANCELED);
    }
};
RW_LAYOUT(ImageButton, fileName, 0x274);
RW_LAYOUT(ImageButton, imageView, 0x298);
RW_LAYOUT(ImageButton, targetScale, 0x2B4);
RW_LAYOUT(ImageButton, selected, 0x2C8);
RW_SIZE(ImageButton, SIZE);

RW_FUNCTION(0x531238, "ImageButton::create(std::string, bool)", ImageButton::create);
RW_FUNCTION(0x531300, "ImageButton::ImageButton()", ImageButton::construct);
RW_FUNCTION(0x531414, "ImageButton::init(std::string, bool)", ImageButton::init);
RW_FUNCTION(0x5315B0, "ImageButton::addTouchEventListener(std::function)", ImageButton::addTouchEventListener);
RW_FUNCTION(0x531630, "ImageButton::setTargetImageView(ui::ImageView*)", ImageButton::setTargetImageView);
RW_FUNCTION(0x531640, "ImageButton::updateImage()", ImageButton::updateImage);
RW_FUNCTION(0x5318C0, "ImageButton::setTargetView(Node*, float)", ImageButton::setTargetView);
RW_FUNCTION(0x5318CE, "ImageButton::setTargetAnchorPoint(const Vec2&)", ImageButton::setTargetAnchorPoint);
RW_FUNCTION(0x5318E0, "ImageButton::setIsDisabled(bool)", ImageButton::setIsDisabled);
RW_FUNCTION(0x5318F0, "ImageButton::setSelectEnabled(bool)", ImageButton::setSelectEnabled);
RW_FUNCTION(0x5318FC, "ImageButton::setIsSelected(bool)", ImageButton::setIsSelected);
RW_FUNCTION(0x531912, "ImageButton::setFileName(std::string)", ImageButton::setFileName);
RW_FUNCTION(0x53192C, "ImageButton::setVisible(bool)", ImageButton::setVisible);
RW_FUNCTION(0x531954, "ImageButton::~ImageButton", ImageButton::destruct);
RW_FUNCTION(0x5319DC, "ImageButton::~ImageButton (deleting)", ImageButton::destructDelete);
RW_FUNCTION(0x531E28, "ImageButton::init::$_1 operator() (onTouchMoved)", ImageButton::onTouchMoved);
RW_FUNCTION(0x531D3E, "ImageButton::init::$_2 operator() (onTouchEnded)", ImageButton::onTouchEnded);
RW_FUNCTION(0x531BF6, "ImageButton::init::$_3 operator() (onTouchCancelled)", ImageButton::onTouchCancelled);
