// Scale9SpriteR: the game's Scale9Sprite whose centre slice is a fixed-size rectangle around a movable centre
// (+0x2F8, the content size's middle by default; +0x300 the centre slice size): the eight border sprites are
// placed and stretched around it.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Scale9SpriteR_ctor = 0x5EC079, Scale9SpriteR_initWithBatchNode = 0x5EC111;
constexpr uint32_t Scale9SpriteR_initWithWidgetVirtualRenderer = 0x5EC2C7, Scale9SpriteR_updatePositions = 0x5EC401;
constexpr uint32_t SpriteBatchNode_create = 0x8A8C69, SpriteBatchNode_createWithTexture = 0x8A8AB5;
constexpr uint32_t TextureAtlas_getTexture = 0x916359, Texture2D_getContentSize = 0x914ACB;
constexpr uint32_t Scale9Sprite_initWithBatchNode = 0x853081, Scale9Sprite_setContentSize = 0x853E05;
constexpr uint32_t Rect_ctor = 0x8DF18B, Vec2_ctor_ = 0x8E4B9D, Vec2_dtor_ = 0x8E4C17, Vec2_set = 0x8E530B;
constexpr uint32_t Size_copy = 0x8DF02D, Size_assign = 0x8DF041, Node_visit = 0x894711, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t GOT_Vec2_ZERO = 0xFD605C;
constexpr uint32_t V_release = 4, V_setScale = 0x48, V_setPosition = 0x5C, V_setAnchorPoint = 0x9C;
constexpr uint32_t V_getContentSize = 0xAC, V_getTexture640 = 0x280, V_getTexture624 = 0x270;
constexpr uint32_t F_CENTER = 0x2F8, F_CENTER_SIZE = 0x300, F_DIRTY = 0x298, F_CONTENT = 0x60;
// the nine sprites: 0x2A0 .. 0x2C0 (index 168 .. 176 as words)
constexpr uint32_t S(uint32_t i) { return 4 * i; }

float f32(uint32_t bits) { float f; std::memcpy(&f, &bits, 4); return f; }
uint32_t sprite(uint32_t self, uint32_t i) { return ld32(self + S(i)); }
void setPos(uint32_t self, uint32_t i, float x, float y) {
    const uint32_t s = sprite(self, i);
    vcall(s, V_setPosition, {s, fbits(x), fbits(y)});
}
void setScale(uint32_t self, uint32_t i, float x, float y) {
    const uint32_t s = sprite(self, i);
    vcall(s, V_setScale, {s, fbits(x), fbits(y)});
}
float sizeW(uint32_t self, uint32_t i) { const uint32_t s = sprite(self, i); return f32(ld32(vcall(s, V_getContentSize, {s}))); }
float sizeH(uint32_t self, uint32_t i) { const uint32_t s = sprite(self, i); return f32(ld32(vcall(s, V_getContentSize, {s}) + 4)); }
}  // namespace

namespace scale9r {

uint32_t create(uint32_t path, uint32_t inset) {
    const uint32_t self = call(fn::operator_new, {0x308});
    call(Scale9SpriteR_ctor, {self});
    const uint32_t batch = call(SpriteBatchNode_create, {path, 9});
    if (call(Scale9SpriteR_initWithBatchNode, {self, batch, inset})) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x5EC010, "Scale9SpriteR::create(const std::string&, float)", scale9r::create);

// the whole texture, the inner rectangle inset by `inset`, the centre in the middle; the batch node's sprites
uint32_t initWithBatchNode(uint32_t self, uint32_t batch, uint32_t insetBits) {
    const float inset = f32(insetBits);
    Frame f;
    const uint32_t size = f.alloc(8), whole = f.alloc(16), inner = f.alloc(16), mid = f.alloc(8);
    call(Texture2D_getContentSize, {size, call(TextureAtlas_getTexture, {ld32(batch + 0x274)})});
    call(Rect_ctor, {whole, 0, 0, ld32(size), ld32(size + 4)});
    const float twice = inset + inset;
    const float h = f32(ld32(whole + 0xC)) - twice;
    const float w = f32(ld32(whole + 8)) - twice;
    call(Rect_ctor, {inner, insetBits, insetBits, fbits(w), fbits(h)});
    call(Vec2_ctor_, {mid, fbits(f32(ld32(size)) * 0.5f), fbits(f32(ld32(size + 4)) * 0.5f)});
    st32(self + F_CENTER + 4, ld32(mid + 4));
    st32(self + F_CENTER, ld32(mid));
    call(Vec2_dtor_, {mid});
    call(Size_assign, {self + F_CENTER_SIZE, inner + 8});
    const uint32_t ok = call(Scale9Sprite_initWithBatchNode, {self, batch, whole, inner});
    call(Vec2_dtor_, {inner});
    call(Vec2_dtor_, {whole});
    return ok ? self : 0;
}
RW_FUNCTION(0x5EC110, "Scale9SpriteR::initWithBatchNode", scale9r::initWithBatchNode);

uint32_t createFromWidget(uint32_t node, uint32_t flag, uint32_t inset) {
    const uint32_t self = call(fn::operator_new, {0x308});
    call(Scale9SpriteR_ctor, {self});
    if (call(Scale9SpriteR_initWithWidgetVirtualRenderer, {self, node, uint32_t(int32_t(int8_t(flag))), inset})) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x5EC25C, "Scale9SpriteR::create(cocos2d::Node*, bool, float)", scale9r::createFromWidget);

// the texture of a widget's renderer (flag 1: of its +0x29C child) as a 9-sprite batch
uint32_t initWithWidgetVirtualRenderer(uint32_t self, uint32_t node, uint32_t flag, uint32_t inset) {
    uint32_t tex;
    if (flag == 1) {
        const uint32_t child = ld32(node + 0x29C);
        tex = vcall(child, V_getTexture624, {child});
    } else {
        tex = vcall(node, V_getTexture640, {node});
    }
    if (!tex) return 0;
    const uint32_t batch = call(SpriteBatchNode_createWithTexture, {tex, 9});
    return call(Scale9SpriteR_initWithBatchNode, {self, batch, inset});
}
RW_FUNCTION(0x5EC2C6, "Scale9SpriteR::initWithWidgetVirtualRenderer", scale9r::initWithWidgetVirtualRenderer);

uint32_t setContentSize(uint32_t self, uint32_t size) {
    call(Vec2_set, {self + F_CENTER, fbits(f32(ld32(size)) * 0.5f), fbits(f32(ld32(size + 4)) * 0.5f)});
    return call(Scale9Sprite_setContentSize, {self, size});
}
RW_FUNCTION(0x5EC3B4, "Scale9SpriteR::setContentSize", scale9r::setContentSize);

// the borders around the centre slice: columns 0 | left | left + slice width, rows 0 | bottom | bottom + slice
// height (left / bottom clamped at 0), each sprite stretched to fill its cell
uint32_t updatePositions(uint32_t self) {
    if (!sprite(self, 172)) return 0;
    Frame f;
    const uint32_t content = f.alloc(8);
    call(Size_copy, {content, self + F_CONTENT});
    const float cy = f32(ld32(self + F_CENTER + 4)), sh = f32(ld32(self + F_CENTER_SIZE + 4));
    const float cx = f32(ld32(self + F_CENTER)), sw = f32(ld32(self + F_CENTER_SIZE));
    const float h = f32(ld32(content + 4)), w = f32(ld32(content));
    const float w168 = sizeW(self, 168), h168 = sizeH(self, 168);
    const float w172 = sizeW(self, 172), h172 = sizeH(self, 172);
    const float w176 = sizeW(self, 176), h176 = sizeH(self, 176);
    for (uint32_t i : {174u, 175u, 176u, 171u, 172u, 173u, 168u, 169u, 170u}) {
        const uint32_t s = sprite(self, i);
        vcall(s, V_setAnchorPoint, {s, ld32(GOT_Vec2_ZERO)});
    }
    setPos(self, 174, 0.0f, 0.0f);
    const float lx = cx - sw * 0.5f, left = lx >= 0.0f ? lx : 0.0f;
    setPos(self, 175, left, 0.0f);
    const float right = sw + left;
    setPos(self, 176, right, 0.0f);
    const float by = cy - sh * 0.5f, bottom = by >= 0.0f ? by : 0.0f;
    setPos(self, 171, 0.0f, bottom);
    setPos(self, 172, left, bottom);
    setPos(self, 173, right, bottom);
    const float top = sh + bottom;
    setPos(self, 168, 0.0f, top);
    setPos(self, 169, left, top);
    setPos(self, 170, right, top);
    const float sx0 = left / w168, sy0 = bottom / h176;
    setScale(self, 174, sx0, sy0);
    const float sx1 = sw / w172;
    setScale(self, 175, sx1, sy0);
    const float sx2 = (w - right) / w176;
    setScale(self, 176, sx2, sy0);
    const float sy1 = sh / h172;
    setScale(self, 171, sx0, sy1);
    setScale(self, 172, sx1, sy1);
    setScale(self, 173, sx2, sy1);
    const float sy2 = (h - top) / h168;
    setScale(self, 168, sx0, sy2);
    setScale(self, 169, sx1, sy2);
    setScale(self, 170, sx2, sy2);
    return 0;
}
RW_FUNCTION(0x5EC400, "Scale9SpriteR::updatePositions", scale9r::updatePositions);

uint32_t visit(uint32_t self, uint32_t renderer, uint32_t transform, uint32_t flags) {
    if (ld8(self + F_DIRTY)) {
        call(Scale9SpriteR_updatePositions, {self});
        st8(self + F_DIRTY, 0);
    }
    return call(Node_visit, {self, renderer, transform, flags});
}
RW_FUNCTION(0x5EC7AC, "Scale9SpriteR::visit", scale9r::visit);

}  // namespace scale9r
