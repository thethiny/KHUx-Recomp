// ImageButton::init $_0: the image button's touch-began handler (batch 3, named from its __func vtable's
// target_type). `functor` is the __func object: vtable, the button (+4), the touch target (+8), the node whose
// scale and anchor shape the hit rect (+0xC).
#include "SceneUi.hpp"

using namespace scene;

namespace {

constexpr uint32_t Touch_getLocation = 0x900E57;              // (Vec2* out, Touch*)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;        // (Vec2* out, node, const Vec2&)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B;                      // (Rect*, x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t FunctionTouch_call = 0x531C5D;             // std::function<void(Ref*, TouchEventType)>::operator()
constexpr uint32_t StringBuffer_ctor = 0x49140D;              // (buffer, fmt, ...)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t String_init = 0x47CD93;                    // std::string(const char*, size_t)
constexpr uint32_t ImageView_loadTexture = 0x839731;          // (view, const std::string&, TextureResType)
constexpr uint32_t S_ON_IMAGE = 0xDFE1A9;                     // "img/ui/%s_On.png"

// ImageButton fields
constexpr uint32_t IB_PRESSED = 0x270, IB_LOCKED = 0x271, IB_ENABLED = 0x272;
constexpr uint32_t IB_NAME = 0x274;                           // std::string: the image name
constexpr uint32_t IB_ON_TOUCH = 0x280;                       // std::function<void(Ref*, TouchEventType)>
constexpr uint32_t IB_IMAGE = 0x298;                          // ImageView* (also the target's first child)
constexpr uint32_t IB_TOUCH_X = 0x2A8, IB_TOUCH_Y = 0x2AC, IB_CLIP = 0x2B0;
constexpr uint32_t IB_NO_PRESS_IMAGE = 0x2C8, IB_KEEP_IMAGE = 0x2C9;
// the scale node's scale (+0x2B4) and anchor (+0x2A0, +0x2A4)
constexpr uint32_t N_SCALE = 0x2B4, N_ANCHOR_X = 0x2A0, N_ANCHOR_Y = 0x2A4;
constexpr uint32_t TARGET_IMAGE = 0x298, TARGET_IMAGE_2 = 0x29C;

uint32_t sizeOf(uint32_t node) { return vcall(node, node::getContentSize, {node}); }

}  // namespace

struct ImageButtonBatch3 {
    static uint32_t touchBegan(uint32_t functor, uint32_t touchPtr) {
        const uint32_t touch = ld32(touchPtr);
        const uint32_t self = ld32(functor + 4);
        Frame f;
        // the clipping node, if any, must contain the touch
        if (const uint32_t clip = ld32(self + IB_CLIP)) {
            const float w = ldf(sizeOf(clip));
            const float h = ldf(sizeOf(ld32(self + IB_CLIP)) + 4);
            const uint32_t rect = f.alloc(16);
            call(Rect_ctor, {rect, 0, 0, fbits(w), fbits(h)});
            const uint32_t clipNode = ld32(self + IB_CLIP);
            const uint32_t at = f.alloc(8), local = f.alloc(8);
            call(Touch_getLocation, {at, touch});
            call(Node_convertToNodeSpace, {local, clipNode, at});
            call(Vec2_dtor, {at});
            const uint32_t in = call(Rect_containsPoint, {rect, local});
            call(Vec2_dtor, {local});
            call(Vec2_dtor, {rect});
            if (!in) return 0;
        }
        {
            const uint32_t at = f.alloc(8);
            call(Touch_getLocation, {at, touch});
            st32(self + IB_TOUCH_Y, ld32(at + 4));
            st32(self + IB_TOUCH_X, ld32(at));
            call(Vec2_dtor, {at});
        }
        // the hit rect: the target's size scaled about its anchor
        uint32_t target = ld32(functor + 8);
        if (const uint32_t image = ld32(target + TARGET_IMAGE)) target = image;
        else if (const uint32_t image2 = ld32(target + TARGET_IMAGE_2)) target = image2;
        const uint32_t scaleNode = ld32(functor + 0xC);
        const float w0 = ldf(sizeOf(target));
        const float scaleX = ldf(scaleNode + N_SCALE);
        const float h0 = ldf(sizeOf(target) + 4);
        const float scaleY = ldf(scaleNode + N_SCALE);
        const float w1 = ldf(sizeOf(target));
        const float h1 = ldf(sizeOf(target) + 4);
        const float w2 = ldf(sizeOf(target));
        const float anchorX = ldf(scaleNode + N_ANCHOR_X);
        const float h2 = ldf(sizeOf(target) + 4);
        const float w = w0 * scaleX;
        const float x = (w1 - w) * 0.5f + w2 * anchorX;
        const float h = h0 * scaleY;
        const float y = (h1 - h) * 0.5f + h2 * ldf(scaleNode + N_ANCHOR_Y);
        const uint32_t rect = f.alloc(16);
        call(Rect_ctor, {rect, fbits(x), fbits(y), fbits(w), fbits(h)});
        const uint32_t local = f.alloc(8);
        {
            const uint32_t at = f.alloc(8);
            call(Touch_getLocation, {at, touch});
            call(Node_convertToNodeSpace, {local, target, at});
            call(Vec2_dtor, {at});
        }
        uint32_t taken = 0;
        if (ld8(self + IB_ENABLED) && !ld8(self + IB_LOCKED) && vcall(target, node::isVisible, {target}) == 1 &&
            call(Rect_containsPoint, {rect, local}) == 1) {
            if (ld32(self + IB_ON_TOUCH + 16)) call(FunctionTouch_call, {self + IB_ON_TOUCH, self, 0});
            st8(self + IB_PRESSED, 1);
            taken = 1;
            const uint32_t image = ld32(self + IB_IMAGE);
            if (image && !ld8(self + IB_LOCKED) && !(ld8(self + IB_KEEP_IMAGE) && ld8(self + IB_NO_PRESS_IMAGE))) {
                // the pressed image "img/ui/<name>_On.png"
                const uint32_t name = (ld8(self + IB_NAME) & 1) ? ld32(self + IB_NAME + 8) : self + IB_NAME + 1;
                const uint32_t buffer = f.alloc(STRING_BUFFER_SIZE);
                call(StringBuffer_ctor, {buffer, S_ON_IMAGE, name});
                const uint32_t path = f.alloc(12);
                call(String_init, {path, buffer, call(STRLEN, {buffer})});
                call(ImageView_loadTexture, {image, path, 0});
                strFree(path);
            }
        }
        call(Vec2_dtor, {local});
        call(Vec2_dtor, {rect});
        return taken;
    }
};

RW_FUNCTION(0x532160, "ImageButton::init::$_0::operator()", ImageButtonBatch3::touchBegan);
