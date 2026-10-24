// ScrollBar: the thin bar drawn along an IScrollProtocol list. A cocos2d::Node holding a background (an empty
// Scale9Sprite as long as the list) and the bar ("img/ui/scrl_bar.png", sized by the visible share of the list),
// laid out across X for horizontal lists and across Y for vertical ones.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"
#include "../scene_main/IScrollProtocol.hpp"

namespace {
using scene::call;
using scene::fbits;
using scene::ld32;
using scene::ldf;

constexpr uint32_t SIZE = 0x280;
constexpr uint32_t VTABLE = 0xF80484;                 // _ZTV9ScrollBar + 8

constexpr uint32_t AEABI_MEMSET = 0x40E7A0;           // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Scale9Sprite_create = rwlib::cocos2d::extension::Scale9Sprite::create__v;    // Scale9Sprite::create()
constexpr uint32_t Scale9Sprite_createFile = rwlib::cocos2d::extension::Scale9Sprite::create__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEERKNS_4RectESD_;  // Scale9Sprite::create(const string&, const Rect&, const Rect& capInsets)
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;              // Rect(x, y, w, h)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;              // Size(w, h)
constexpr uint32_t STRING_INIT = 0x47CD93;
constexpr uint32_t IScrollProtocol_getMaxPosition = 0x4F6511;
constexpr uint32_t STR_SCROLL_BAR_PNG = 0xDFCF02;     // "img/ui/scrl_bar.png" (19 chars)

// cocos2d::Node virtual slots (byte offsets)
constexpr uint32_t V_SET_POSITION_X = 0x64, V_SET_POSITION_Y = 0x6C;
constexpr uint32_t V_SET_CONTENT_SIZE = 0xA8, V_GET_CONTENT_SIZE = 0xAC, V_SET_VISIBLE = 0xB0, V_ADD_CHILD = 0xF0;

uint32_t vfn(uint32_t obj, uint32_t slot) { return ld32(ld32(obj) + slot); }
// getContentSize(): a pointer to the node's Size {width, height}
uint32_t contentSize(uint32_t node) { return call(vfn(node, V_GET_CONTENT_SIZE), {node}); }
float width(uint32_t node) { return ldf(contentSize(node)); }
float height(uint32_t node) { return ldf(contentSize(node) + 4); }
// node->setContentSize(Size(w, h)) with the setter read before the Size is built, as the code does
void setContentSize(scene::Frame &f, uint32_t node, uint32_t setter, float w, float h) {
    uint32_t size = f.alloc(8);
    call(Size_ctor, {size, fbits(w), fbits(h)});
    call(setter, {node, size});
}

}  // namespace

struct ScrollBar {
    rw::Field<uint32_t> vtable;                     // +0x000 cocos2d::Node
    const uint8_t node[0x26C];
    rw::Field<float> scrollPosition;                // +0x270 the list position last shown
    rw::Field<uint32_t> list;                       // +0x274 IScrollProtocol* (the list it belongs to)
    rw::Field<uint32_t> bar;                        // +0x278 Scale9Sprite*: the bar
    rw::Field<uint32_t> background;                 // +0x27C Scale9Sprite*: the track

    uint32_t self() const { return rw::guest_addr(this); }
    bool horizontal() const { return rw::host_ptr<IScrollProtocol>(list)->scrollType != 0; }

    static uint32_t create(uint32_t list) {
        uint32_t p = call(scene::fn::operator_new, {SIZE});
        call(AEABI_MEMSET, {p, SIZE, 0});
        call(Node_ctor, {p});
        ScrollBar *s = rw::host_ptr<ScrollBar>(p);
        s->vtable = VTABLE;
        s->background = 0;
        s->bar = 0;
        s->list = 0;
        s->scrollPosition = 0.0f;
        init(s, list);
        call(Ref_autorelease, {p});
        return p;
    }

    static uint32_t init(ScrollBar *s, uint32_t list) {
        s->list = list;
        uint32_t bg = call(Scale9Sprite_create, {});
        s->background = bg;
        // the track sits on the list's centre line
        if (s->horizontal()) {
            uint32_t setX = vfn(bg, V_SET_POSITION_X);
            call(setX, {bg, fbits(width(s->list) * 0.5f)});
        } else {
            uint32_t setY = vfn(bg, V_SET_POSITION_Y);
            call(setY, {bg, fbits(height(s->list) * 0.5f)});
        }
        scene::vcall(s->self(), V_ADD_CHILD, {s->self(), s->background});
        {
            scene::Frame f;
            uint32_t path = f.alloc(12), rect = f.alloc(16), insets = f.alloc(16);
            call(STRING_INIT, {path, STR_SCROLL_BAR_PNG, 19});
            call(Rect_ctor, {rect, fbits(0.0f), fbits(0.0f), fbits(12.0f), fbits(16.0f)});
            call(Rect_ctor, {insets, fbits(6.0f), fbits(7.0f), fbits(1.0f), fbits(4.0f)});
            s->bar = call(Scale9Sprite_createFile, {path, rect, insets});
            call(Vec2_dtor, {insets});
            call(Vec2_dtor, {rect});
            scene::strFree(path);
        }
        scene::vcall(s->self(), V_ADD_CHILD, {s->self(), s->bar});
        setPixel(s, 12.0f);
        return 1;
    }

    // thickness of the bar: the node, the track and the bar get it across the scroll direction
    static void setPixel(ScrollBar *s, float pixel) {
        uint32_t self = s->self();
        uint32_t setSelf = vfn(self, V_SET_CONTENT_SIZE);
        bool horiz = s->horizontal();
        uint32_t listSize = contentSize(s->list);
        scene::Frame f;
        if (horiz) {
            setContentSize(f, self, setSelf, ldf(listSize), pixel);
            uint32_t bg = s->background;
            uint32_t setBg = vfn(bg, V_SET_CONTENT_SIZE);
            setContentSize(f, bg, setBg, width(bg), pixel);
            uint32_t track = s->background;
            uint32_t setY = vfn(track, V_SET_POSITION_Y);
            float half = pixel * 0.5f;
            call(setY, {track, fbits(half)});
            uint32_t bar = s->bar;
            uint32_t setBar = vfn(bar, V_SET_CONTENT_SIZE);
            setContentSize(f, bar, setBar, width(bar), pixel);
            uint32_t b = s->bar;
            call(vfn(b, V_SET_POSITION_Y), {b, fbits(half)});
        } else {
            setContentSize(f, self, setSelf, pixel, ldf(listSize + 4));
            uint32_t bg = s->background;
            uint32_t setBg = vfn(bg, V_SET_CONTENT_SIZE);
            setContentSize(f, bg, setBg, pixel, height(bg));
            uint32_t track = s->background;
            uint32_t setX = vfn(track, V_SET_POSITION_X);
            float half = pixel * 0.5f;
            call(setX, {track, fbits(half)});
            uint32_t bar = s->bar;
            uint32_t setBar = vfn(bar, V_SET_CONTENT_SIZE);
            setContentSize(f, bar, setBar, pixel, height(bar));
            uint32_t b = s->bar;
            call(vfn(b, V_SET_POSITION_X), {b, fbits(half)});
        }
    }

    // fit the node and the track to the list, size the bar to the visible share, then place it
    static void updateLayout(ScrollBar *s) {
        uint32_t self = s->self();
        uint32_t setSelf = vfn(self, V_SET_CONTENT_SIZE);
        uint32_t list = s->list;
        scene::Frame f;
        if (rw::host_ptr<IScrollProtocol>(list)->scrollType != 0) {
            float listW = width(s->list);
            setContentSize(f, self, setSelf, listW, height(s->background));
            uint32_t bg = s->background;
            uint32_t setBg = vfn(bg, V_SET_CONTENT_SIZE);
            float w = width(s->list);
            setContentSize(f, bg, setBg, w, height(s->background));
            uint32_t track = s->background;
            uint32_t setX = vfn(track, V_SET_POSITION_X);
            call(setX, {track, fbits(width(track) * 0.5f)});
            float view = width(s->list);
            float view2 = width(s->list);
            float maxPos = rw::callf(IScrollProtocol_getMaxPosition, {s->list});
            uint32_t bar = s->bar;
            uint32_t setBar = vfn(bar, V_SET_CONTENT_SIZE);
            float listW2 = width(s->list);
            uint32_t barSize = contentSize(s->bar);
            float len = (view / (view2 + maxPos)) * listW2;
            setContentSize(f, bar, setBar, len, ldf(barSize + 4));
        } else {
            float bgW = width(s->background);
            setContentSize(f, self, setSelf, bgW, height(s->list));
            uint32_t bg = s->background;
            uint32_t setBg = vfn(bg, V_SET_CONTENT_SIZE);
            float w = width(bg);
            setContentSize(f, bg, setBg, w, height(s->list));
            uint32_t track = s->background;
            uint32_t setY = vfn(track, V_SET_POSITION_Y);
            call(setY, {track, fbits(height(track) * 0.5f)});
            float view = height(s->list);
            float view2 = height(s->list);
            float maxPos = rw::callf(IScrollProtocol_getMaxPosition, {s->list});
            uint32_t bar = s->bar;
            uint32_t setBar = vfn(bar, V_SET_CONTENT_SIZE);
            float barW = width(bar);
            uint32_t listSize = contentSize(s->list);
            float len = (view / (view2 + maxPos)) * ldf(listSize + 4);
            setContentSize(f, bar, setBar, barW, len);
        }
        setScrollPosition(s, s->scrollPosition);
    }

    // place the bar for a list position (0 .. getMaxPosition): left to right, or top to bottom
    static void setScrollPosition(ScrollBar *s, float position) {
        s->scrollPosition = position;
        float maxPos = rw::callf(IScrollProtocol_getMaxPosition, {s->list});
        int32_t type = rw::host_ptr<IScrollProtocol>(s->list)->scrollType;
        uint32_t barSize = contentSize(s->bar);
        float ratio = position / maxPos;
        uint32_t bar = s->bar;
        if (type != 0) {
            uint32_t setX = vfn(bar, V_SET_POSITION_X);
            float barW = ldf(barSize);
            float trackW = width(s->background);
            uint32_t barSize2 = contentSize(s->bar);
            float half = barW * 0.5f;
            call(setX, {bar, fbits(half + ratio * (trackW - ldf(barSize2)))});
        } else {
            uint32_t setY = vfn(bar, V_SET_POSITION_Y);
            float barH = ldf(barSize + 4);
            float trackH = height(s->background);
            uint32_t barSize2 = contentSize(s->bar);
            float half = barH * 0.5f;
            float span = trackH - ldf(barSize2 + 4);
            call(setY, {bar, fbits(half + (1.0f - ratio) * span)});
        }
    }

    static void setBackgroundVisible(ScrollBar *s, uint32_t visible) {
        uint32_t bg = s->background;
        call(vfn(bg, V_SET_VISIBLE), {bg, visible});
    }

    static void destructDelete(ScrollBar *s) {
        uint32_t p = call(Node_dtor, {s->self()});
        call(scene::fn::operator_delete, {p});
    }
};
RW_LAYOUT(ScrollBar, scrollPosition, 0x270);
RW_LAYOUT(ScrollBar, background, 0x27C);
RW_SIZE(ScrollBar, 0x280);

RW_FUNCTION(0x4F5024, "ScrollBar::create(IScrollProtocol*)", ScrollBar::create);
RW_FUNCTION(0x4F50B0, "ScrollBar::init(IScrollProtocol*)", ScrollBar::init);
RW_FUNCTION(0x4F5220, "ScrollBar::setPixel(float)", ScrollBar::setPixel);
RW_FUNCTION(0x4F53F8, "ScrollBar::updateLayout()", ScrollBar::updateLayout);
RW_FUNCTION(0x4F56A4, "ScrollBar::setScrollPosition(float)", ScrollBar::setScrollPosition);
RW_FUNCTION(0x4F57C0, "ScrollBar::setBackgroundVisible(bool)", ScrollBar::setBackgroundVisible);
RW_FUNCTION(0x4F57D2, "ScrollBar::~ScrollBar (deleting)", ScrollBar::destructDelete);
