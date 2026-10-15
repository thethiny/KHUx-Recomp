// MyRecordScrollBar: the scroll bar of the "my record" list (MyRecordScrollView): a cocos2d::Node with a
// background (Scale9Sprite scrollbar_bg.png, hidden by default) and a bar (scrl_bar.png) whose length is the
// visible share of the content and whose position follows the scroll position. Type 0 lays it out along y,
// any other type along x.
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t SIZE = 0x290;
constexpr uint32_t VTABLE = 0xF84B00;            // _ZTV17MyRecordScrollBar + 8
// fields
constexpr uint32_t TYPE = 0x270;                 // ScrollBarType: 0 along y, else along x
constexpr uint32_t VIEW_WIDTH = 0x274;           // cocos2d::Size: the visible area (width, height)
constexpr uint32_t VIEW_HEIGHT = 0x278;
constexpr uint32_t MAX_POSITION = 0x27C;         // float: the content's scrollable length
constexpr uint32_t SCROLL_POSITION = 0x280;      // float
constexpr uint32_t BAR = 0x284;                  // Scale9Sprite*
constexpr uint32_t BACKGROUND = 0x288;           // Scale9Sprite*

constexpr uint32_t AEABI_MEMSET = 0x40E7A0;
constexpr uint32_t NODE_CTOR = 0x892531;
constexpr uint32_t NODE_DTOR = 0x8927ED;
constexpr uint32_t REF_AUTORELEASE = 0x8FD0D5;
constexpr uint32_t SIZE_CTOR_DEFAULT = 0x8DF01F;            // cocos2d::Size::Size()
constexpr uint32_t SIZE_CTOR = 0x8DF027;                    // cocos2d::Size::Size(float, float)
constexpr uint32_t SIZE_ASSIGN = 0x8DF041;                  // cocos2d::Size::operator=(const Size&)
constexpr uint32_t RECT_CTOR = 0x8DF18B;                    // cocos2d::Rect::Rect(x, y, w, h)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;                    // cocos2d::Vec2::~Vec2 (Rect's inlined destructor)
constexpr uint32_t SCALE9_CREATE = 0x854589;                // Scale9Sprite::create(const string&)
constexpr uint32_t SCALE9_CREATE_CAP = 0x8543E9;            // Scale9Sprite::create(const string&, const Rect&, const Rect& capInsets)
constexpr uint32_t STR_BG = 0xDFD8EA;                       // "img/ui/scrollbar_bg.png"
constexpr uint32_t STR_BAR = 0xDFCF02;                      // "img/ui/scrl_bar.png"

float f(uint32_t self, uint32_t off) { return ldf(self + off); }
uint32_t size(Frame &fr, float w, float h) {
    uint32_t s = fr.alloc(8);
    call(SIZE_CTOR, {s, fbits(w), fbits(h)});
    return s;
}
uint32_t vfn(uint32_t obj, uint32_t slot) { return ld32(ld32(obj) + slot); }
}  // namespace

struct MyRecordScrollBar {
    // move the bar to the scroll position
    static void setScrollPosition(uint32_t self, float position) {
        st32(self + SCROLL_POSITION, fbits(position));
        uint32_t type = ld32(self + TYPE);
        float max = f(self, MAX_POSITION);
        uint32_t barSize = vcall(ld32(self + BAR), node::getContentSize, {ld32(self + BAR)});
        float ratio = position / max;
        uint32_t bg = ld32(self + BACKGROUND);
        uint32_t bgGetSize = vfn(bg, node::getContentSize);
        uint32_t bar = ld32(self + BAR);
        if (type) {
            uint32_t setPositionX = vfn(bar, node::setPositionX);
            float barWidth = ldf(barSize);
            float bgWidth = ldf(call(bgGetSize, {bg}));
            uint32_t barSize2 = vcall(ld32(self + BAR), node::getContentSize, {ld32(self + BAR)});
            float x = barWidth * 0.5f + ratio * (bgWidth - ldf(barSize2));
            call(setPositionX, {bar, fbits(x)});
        } else {
            uint32_t setPositionY = vfn(bar, node::setPositionY);
            float barHeight = ldf(barSize + 4);
            uint32_t bgSize = call(bgGetSize, {bg});
            float y = barHeight * 0.5f + (1.0f - ratio) * ldf(bgSize + 4);
            call(setPositionY, {bar, fbits(y)});
        }
    }

    static uint32_t create(uint32_t type, uint32_t viewSize) {
        uint32_t self = call(fn::operator_new, {SIZE});
        call(AEABI_MEMSET, {self, SIZE, 0});
        call(NODE_CTOR, {self});
        st32(self, VTABLE);
        call(SIZE_CTOR_DEFAULT, {self + VIEW_WIDTH});
        st32(self + BACKGROUND, 0);
        st32(self + BAR, 0);
        st32(self + SCROLL_POSITION, 0);
        st32(self + MAX_POSITION, 0);
        init(self, type, viewSize);
        call(REF_AUTORELEASE, {self});
        return self;
    }

    static void setBackGroundVisible(uint32_t self, uint32_t visible) {
        uint32_t bg = ld32(self + BACKGROUND);
        vcall(bg, node::setVisible, {bg, visible});
    }

    static void setMaxPosition(uint32_t self, float max) { st32(self + MAX_POSITION, fbits(max)); }

    // size the node, the background and the bar (bar length = view / (view + max) of the view)
    static void updateLayout(uint32_t self) {
        uint32_t setContentSize = vfn(self, node::setContentSize);
        Frame fr;
        if (ld32(self + TYPE)) {
            float w = f(self, VIEW_WIDTH);
            uint32_t bg = ld32(self + BACKGROUND);
            float bgHeight = ldf(vcall(bg, node::getContentSize, {bg}) + 4);
            call(setContentSize, {self, size(fr, w, bgHeight)});
            bg = ld32(self + BACKGROUND);
            uint32_t bgSetSize = vfn(bg, node::setContentSize);
            float w2 = f(self, VIEW_WIDTH);
            bgHeight = ldf(call(vfn(bg, node::getContentSize), {bg}) + 4);
            call(bgSetSize, {bg, size(fr, w2, bgHeight)});
            bg = ld32(self + BACKGROUND);
            uint32_t setPositionX = vfn(bg, node::setPositionX);
            float bgWidth = ldf(call(vfn(bg, node::getContentSize), {bg}));
            call(setPositionX, {bg, fbits(bgWidth * 0.5f)});
            uint32_t bar = ld32(self + BAR);
            uint32_t barSetSize = vfn(bar, node::setContentSize);
            float max = f(self, MAX_POSITION);
            float view = f(self, VIEW_WIDTH);
            uint32_t barSize = call(vfn(bar, node::getContentSize), {bar});
            float length = view * (view / (view + max));
            call(barSetSize, {bar, size(fr, length, ldf(barSize + 4))});
        } else {
            uint32_t bg = ld32(self + BACKGROUND);
            float bgWidth = ldf(vcall(bg, node::getContentSize, {bg}));
            call(setContentSize, {self, size(fr, bgWidth, f(self, VIEW_HEIGHT))});
            bg = ld32(self + BACKGROUND);
            uint32_t bgSetSize = vfn(bg, node::setContentSize);
            bgWidth = ldf(call(vfn(bg, node::getContentSize), {bg}));
            call(bgSetSize, {bg, size(fr, bgWidth, f(self, VIEW_HEIGHT))});
            bg = ld32(self + BACKGROUND);
            uint32_t setPositionY = vfn(bg, node::setPositionY);
            float bgHeight = ldf(call(vfn(bg, node::getContentSize), {bg}) + 4);
            call(setPositionY, {bg, fbits(bgHeight * 0.5f)});
            uint32_t bar = ld32(self + BAR);
            uint32_t barSetSize = vfn(bar, node::setContentSize);
            float max = f(self, MAX_POSITION);
            float view = f(self, VIEW_HEIGHT);
            uint32_t barSize = call(vfn(bar, node::getContentSize), {bar});
            float length = (view / (view + max)) * f(self, VIEW_HEIGHT);
            call(barSetSize, {bar, size(fr, ldf(barSize), length)});
        }
        setScrollPosition(self, f(self, SCROLL_POSITION));
    }

    static uint32_t init(uint32_t self, uint32_t type, uint32_t viewSize) {
        st32(self + TYPE, type);
        call(SIZE_ASSIGN, {self + VIEW_WIDTH, viewSize});
        Frame fr;
        uint32_t name = fr.alloc(12);
        strInit(name, STR_BG, 23);
        uint32_t bg = call(SCALE9_CREATE, {name});
        st32(self + BACKGROUND, bg);
        strFree(name);
        bg = ld32(self + BACKGROUND);
        vcall(bg, node::setVisible, {bg, 0});
        bg = ld32(self + BACKGROUND);
        uint32_t vtable = ld32(bg);
        if (type) {
            float half = ldf(viewSize) * 0.5f;
            call(ld32(vtable + node::setPositionX), {bg, fbits(half)});
        } else {
            float half = ldf(viewSize + 4) * 0.5f;
            call(ld32(vtable + node::setPositionY), {bg, fbits(half)});
        }
        vcall(self, node::addChild, {self, ld32(self + BACKGROUND)});
        uint32_t barName = fr.alloc(12), rect = fr.alloc(16), capInsets = fr.alloc(16);
        strInit(barName, STR_BAR, 19);
        call(RECT_CTOR, {rect, 0, 0, fbits(12.0f), fbits(16.0f)});
        call(RECT_CTOR, {capInsets, fbits(6.0f), fbits(7.0f), fbits(1.0f), fbits(4.0f)});
        st32(self + BAR, call(SCALE9_CREATE_CAP, {barName, rect, capInsets}));
        call(VEC2_DTOR, {capInsets});
        call(VEC2_DTOR, {rect});
        strFree(barName);
        vcall(self, node::addChild, {self, ld32(self + BAR)});
        setPixel(self, 12.0f);
        return 1;
    }

    // the bar's thickness
    static void setPixel(uint32_t self, float pixel) {
        uint32_t setContentSize = vfn(self, node::setContentSize);
        Frame fr;
        const bool alongX = ld32(self + TYPE) != 0;
        if (alongX) call(setContentSize, {self, size(fr, f(self, VIEW_WIDTH), pixel)});
        else call(setContentSize, {self, size(fr, pixel, f(self, VIEW_HEIGHT))});
        uint32_t bg = ld32(self + BACKGROUND);
        uint32_t bgSetSize = vfn(bg, node::setContentSize);
        uint32_t bgSize = call(vfn(bg, node::getContentSize), {bg});
        if (alongX) call(bgSetSize, {bg, size(fr, ldf(bgSize), pixel)});
        else call(bgSetSize, {bg, size(fr, pixel, ldf(bgSize + 4))});
        bg = ld32(self + BACKGROUND);
        const uint32_t slot = alongX ? node::setPositionY : node::setPositionX;
        uint32_t setPosition = vfn(bg, slot);
        float half = pixel * 0.5f;
        call(setPosition, {bg, fbits(half)});
        uint32_t bar = ld32(self + BAR);
        uint32_t barSetSize = vfn(bar, node::setContentSize);
        uint32_t barSize = call(vfn(bar, node::getContentSize), {bar});
        if (alongX) call(barSetSize, {bar, size(fr, ldf(barSize), pixel)});
        else call(barSetSize, {bar, size(fr, pixel, ldf(barSize + 4))});
        bar = ld32(self + BAR);
        call(vfn(bar, slot), {bar, fbits(half)});
    }

    static void deletingDestruct(uint32_t self) {
        call(NODE_DTOR, {self});
        call(fn::operator_delete, {self});
    }
};

RW_FUNCTION(0x519F8C, "MyRecordScrollBar::setScrollPosition", MyRecordScrollBar::setScrollPosition);
RW_FUNCTION(0x51B110, "MyRecordScrollBar::create", MyRecordScrollBar::create);
RW_FUNCTION(0x51B1B8, "MyRecordScrollBar::setBackGroundVisible", MyRecordScrollBar::setBackGroundVisible);
RW_FUNCTION(0x51B1CA, "MyRecordScrollBar::setMaxPosition", MyRecordScrollBar::setMaxPosition);
RW_FUNCTION(0x51B1D4, "MyRecordScrollBar::updateLayout", MyRecordScrollBar::updateLayout);
RW_FUNCTION(0x51B408, "MyRecordScrollBar::init", MyRecordScrollBar::init);
RW_FUNCTION(0x51B5C8, "MyRecordScrollBar::setPixel", MyRecordScrollBar::setPixel);
RW_FUNCTION(0x51BAA0, "MyRecordScrollBar::~MyRecordScrollBar (deleting)", MyRecordScrollBar::deletingDestruct);
