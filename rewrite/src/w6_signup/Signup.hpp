// Shared pieces of the new-account screens (w6_signup): popup placement, the darkening shadow, heap lambdas.
#pragma once

#include "../scene/SceneUi.hpp"

namespace signup {
using namespace scene;

constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create, Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;           // &Vec2::ANCHOR_MIDDLE

// the popup shadow on `layer`: transparent, then (when fadeIn) fading in to its opacity in 0.1 s
inline uint32_t addShadow(uint32_t layer, bool fadeIn) {
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x20C});
    const uint32_t opacity = vcall(shadow, 0x20C, {shadow});
    vcall(shadow, 0x214, {shadow, 0});
    if (fadeIn) call(Node_runAction, {shadow, call(FadeTo_create, {0x3DCCCCCD, opacity})});
    vcall(layer, 0xF0, {layer, shadow});
    return shadow;
}

// anchor in the middle, then moved by half its size plus the side margins of a screen wider / taller than the
// design size (each value read in the original's order)
inline void centerOnMargins(uint32_t node) {
    vcall(node, 0x9C, {node, ld32(GOT_ANCHOR_MIDDLE)});
    const float posX = bitsf(vcall(node, 0x68, {node}));
    const float w = ldf(vcall(node, 0xAC, {node}));
    const float margin = bitsf(call(SceneUtil_getSideMargin, {}));
    const float posY = bitsf(vcall(node, 0x70, {node}));
    const float h = ldf(vcall(node, 0xAC, {node}) + 4);
    const float x = margin + (posX + w * 0.5f);
    const float y0 = posY + h * 0.5f;
    const float hm = bitsf(call(SceneUtil_getSideHeightMargin, {fbits(y0)}));
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(Vec2_ctor, {pos, fbits(x), fbits(hm + y0)});
    vcall(node, 0x4C, {node, pos});
    call(Vec2_dtor, {pos});
}

// a std::function whose lambda lives on the heap ({vtable, words at +4...}); 0 when the allocation fails
inline uint32_t heapObject(uint32_t vt, uint32_t size) {
    const uint32_t obj = call(fn::operator_new, {size});
    if (obj) st32(obj, vt);
    return obj;
}

}  // namespace signup
