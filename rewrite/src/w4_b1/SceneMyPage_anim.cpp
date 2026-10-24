// SceneMyPage: the in / out animations and the camera over the field.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5, SceneUtil_createRightInAnimation = 0x45B8BD;
constexpr uint32_t SceneUtil_createBottomInAnimation = 0x45B2F9, SceneUtil_createLeftOutAnimation = 0x45B735;
constexpr uint32_t SceneUtil_createBottomOutAnimation = 0x45B469, SceneUtil_createRightOutAnimation = 0x45BA3D;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneManager_getPopupLayer2 = 0x613E71;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FunctionVV_move = 0x47F905;
constexpr uint32_t M_setCameraPosition = 0x640851;
constexpr uint32_t VT_in_15 = 0xF6170C, VT_camera_20 = 0xF61630, VT_out_16 = 0xF616E0;
constexpr uint32_t ANCHOR_MIDDLE = 0xFEF5C0;
constexpr uint32_t S_CENTER_UI = 0xE02FBA, S_RIGHT_UI = 0xDF85F7, S_LEFT_UI = 0xDF85F0, S_RIGHT_BANNER = 0xE0374A;

// node->runAction(EaseSineOut(create<Side>Animation(node, 0, 0, done)))
void slide(uint32_t node, uint32_t create, uint32_t done) {
    uint32_t anim = call(create, {node, 0, 0, done});
    call(wl::Node_runAction, {node, call(EaseSineOut_create, {anim})});
}
void slideEmpty(uint32_t node, uint32_t create) {
    Frame f;
    uint32_t empty = newEmptyFunction(f);
    slide(node, create, empty);
    functionDestroy(empty);
}
}  // namespace

// The UI slides in, the field zooms to its scale ($_15 removes the shadow), the camera goes to the player
void SceneMyPage_runInAnimation(uint32_t self) {
    const uint32_t root = ld32(self + 0x290);
    slideEmpty(findNode(root, S_CENTER_UI, 8), SceneUtil_createLeftInAnimation);
    uint32_t right = findNode(ld32(self + 0x290), S_RIGHT_UI, 7);
    slideEmpty(right, SceneUtil_createRightInAnimation);
    slideEmpty(findNode(ld32(self + 0x290), S_RIGHT_BANNER, 14), SceneUtil_createBottomInAnimation);
    {
        uint32_t children = vcall(right, node::getChildren, {right});
        uint32_t first = ld32(ld32(children));
        vcall(first, 0x29C, {first, 0});   // setTouchEnabled(false)
    }
    uint32_t layer = call(SceneManager_getPopupLayer2, {call(SceneManager_getInstance, {})});
    uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    vcall(shadow, node::setOpacity, {shadow, 0});
    vcall(layer, node::addChild, {layer, shadow});
    st32(self + 0x374, ld32(self + OFF_SCALE_BASE));
    uint32_t panel = ld32(self + 0x298);
    vcall(panel, node::setAnchorPoint, {panel, ANCHOR_MIDDLE});
    {
        panel = ld32(self + 0x298);
        float w = ldf(vcall(panel, node::getContentSize, {panel}));
        float h = ldf(vcall(ld32(self + 0x298), node::getContentSize, {ld32(self + 0x298)}) + 4);
        Frame f;
        uint32_t v = f.alloc(8);
        call(wl::Vec2_ctorXY, {v, fb(w * 0.5f), fb(h * 0.5f)});
        vcall(panel, node::setPosition, {panel, v});
        call(wl::Vec2_dtor, {v});
    }
    {
        uint32_t target = ld32(self + 0x298);
        uint32_t delay = call(wl::DelayTime_create, {fb(0.2f)});
        uint32_t zoom = call(EaseSineOut_create, {call(fn::ScaleTo_create, {fb(1.2f), ld32(self + OFF_SCALE_BASE)})});
        Frame f;
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x24});
        if (heap) {
            st32(heap, VT_in_15);
            st32(heap + 4, layer);
            st32(heap + 8, shadow);
            for (uint32_t k = 0xC; k <= 0x1C; k += 4) st32(heap + k, self);
        }
        st32(fo + 16, heap);
        uint32_t cf = call(wl::CallFunc_create, {fo});
        call(wl::Node_runAction, {target, call(wl::Sequence_create, {delay, zoom, cf, 0})});
        functionDestroy(fo);
    }
    {
        uint32_t center = ld32(self + 0x294);
        uint32_t delay = call(wl::DelayTime_create, {fb(0.2f)});
        uint32_t zoom = call(EaseSineOut_create, {call(fn::ScaleTo_create, {fb(1.2f), ld32(self + OFF_SCALE_BASE)})});
        call(wl::Node_runAction, {center, call(wl::Sequence_create, {delay, zoom, 0})});
    }
    uint32_t me = ld32(self + 0x2BC);
    call(M_setCameraPosition, {self, vcall(me, node::getPosition, {me}), fb(1.2f)});
}

// Centre the field on `pos` (clamped to the field), moving the field widget and then the avatar layer ($_20)
void SceneMyPage_setCameraPosition(uint32_t self, uint32_t pos, float duration) {
    Frame f;
    uint32_t target = f.alloc(8);
    call(fn::Vec2_ctor_copy, {target, pos});
    const uint32_t panelAt = self + 0x298, fieldAt = self + 0x29C;
    {
        uint32_t p = ld32(panelAt);
        stf(target, ldf(vcall(p, node::getContentSize, {p})) - ldf(target));
        float ratio = ldf(self + 0x370);
        p = ld32(panelAt);
        uint32_t size = vcall(p, node::getContentSize, {p});
        float a = -60.0f / ratio;
        stf(target + 4, (ldf(size + 4) - ldf(target + 4)) + a);
    }
    uint32_t vs1 = f.alloc(8), vs2 = f.alloc(8);
    call(fn::Director_getVisibleSize, {vs1, call(fn::Director_getInstance, {})});
    const float vw = ldf(vs1);
    call(fn::Director_getVisibleSize, {vs2, call(fn::Director_getInstance, {})});
    const float vh = ldf(vs2 + 4);
    float fieldW, fieldH;
    {
        uint32_t w = ld32(fieldAt);
        float cw = ldf(vcall(w, node::getContentSize, {w}));
        fieldW = cw * bf(vcall(ld32(fieldAt), node::getScale, {ld32(fieldAt)}));
        w = ld32(fieldAt);
        float ch = ldf(vcall(w, node::getContentSize, {w}) + 4);
        fieldH = ch * bf(vcall(ld32(fieldAt), node::getScale, {ld32(fieldAt)}));
    }
    uint32_t p = ld32(panelAt);
    float pw = ldf(vcall(p, node::getContentSize, {p}));
    const float cur = ldf(self + 0x374);
    p = ld32(panelAt);
    uint32_t psize = vcall(p, node::getContentSize, {p});
    const float a1 = (vw - pw * cur) * 0.5f / cur;
    const float b1 = fieldW * 0.5f;
    const float minX = b1 - a1;
    float tx = ldf(target);
    const bool keepX = minX >= tx || minX != minX || tx != tx;
    const float a2 = (vh - ldf(psize + 4) * cur) * 0.5f / cur;
    if (!keepX) {
        stf(target, minX);
        tx = minX;
    }
    const float maxX = vw / cur - b1 - a1;
    if (!(tx >= maxX || tx != tx || maxX != maxX)) stf(target, maxX);
    const float b2 = fieldH * 0.5f;
    const float minY = b2 - a2;
    float ty = ldf(target + 4);
    if (!(minY >= ty || minY != minY || ty != ty)) {
        stf(target + 4, minY);
        ty = minY;
    }
    const float maxY = vh / cur - b2 - a2;
    if (!(ty >= maxY || ty != ty || maxY != maxY)) stf(target + 4, maxY);

    call(smain::lib::Node_stopAllActions, {ld32(fieldAt)});
    {
        uint32_t w = ld32(fieldAt);
        uint32_t delay = call(wl::DelayTime_create, {fb(0.2f)});
        uint32_t move = call(EaseSineOut_create, {call(wl::MoveTo_create, {fb(duration), target})});
        call(wl::Node_runAction, {w, call(wl::Sequence_create, {delay, move, 0})});
    }
    // the avatar layer follows (panel size / 2 off), then $_20 stores the camera
    float lx, ly;
    {
        uint32_t pp = ld32(panelAt);
        float tx2 = ldf(target);
        float ty2 = ldf(target + 4);
        float w = ldf(vcall(pp, node::getContentSize, {pp}));
        pp = ld32(panelAt);
        uint32_t s2 = vcall(pp, node::getContentSize, {pp});
        lx = tx2 - w * 0.5f;
        ly = ty2 - ldf(s2 + 4) * 0.5f;
    }
    uint32_t layerPos = f.alloc(8);
    call(wl::Vec2_ctorXY, {layerPos, fb(lx), fb(ly)});
    uint32_t cap1 = f.alloc(8);
    call(fn::Vec2_ctor_copy, {cap1, layerPos});
    uint32_t move = call(EaseSineOut_create, {call(wl::MoveTo_create, {fb(duration), layerPos})});
    uint32_t cap2 = f.alloc(8);
    call(fn::Vec2_ctor_copy, {cap2, cap1});
    uint32_t fo = f.alloc(24);
    st32(fo + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x10});
    if (heap) {
        st32(heap, VT_camera_20);
        st32(heap + 4, self);
        call(fn::Vec2_ctor_copy, {heap + 8, cap2});
        st32(heap + 4, self);
    }
    st32(fo + 16, heap);
    uint32_t cf = call(wl::CallFunc_create, {fo});
    uint32_t seq = call(wl::Sequence_create, {move, cf, 0});
    functionDestroy(fo);
    call(wl::Vec2_dtor, {cap2});
    st8(self + 0x3CC, 1);
    call(smain::lib::Node_stopAllActions, {ld32(self + 0x2A4)});
    {
        uint32_t layer = ld32(self + 0x2A4);
        uint32_t delay = call(wl::DelayTime_create, {fb(0.2f)});
        call(wl::Node_runAction, {layer, call(wl::Sequence_create, {delay, seq, 0})});
    }
    call(wl::Vec2_dtor, {cap1});
    call(wl::Vec2_dtor, {layerPos});
    call(wl::Vec2_dtor, {target});
}

// The UI slides out; `done` runs when the centre UI is out ($_16)
void SceneMyPage_runOutAnimationMain(uint32_t self, uint32_t done) {
    st8(self + 0x410, 0);
    call(SceneManager_setCanTouchHeader, {call(SceneManager_getInstance, {}), 0});
    uint32_t center = findNode(ld32(self + 0x290), S_CENTER_UI, 8);
    {
        Frame f;
        uint32_t copy = f.alloc(24);
        call(fn::function_vv_copy, {copy, done});
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x20});
        if (heap) {
            st32(heap, VT_out_16);
            call(FunctionVV_move, {heap + 8, copy});
        }
        st32(fo + 16, heap);
        slide(center, SceneUtil_createLeftOutAnimation, fo);
        functionDestroy(fo);
        functionDestroy(copy);
    }
    slideEmpty(findNode(ld32(self + 0x290), S_LEFT_UI, 6), SceneUtil_createBottomOutAnimation);
    slideEmpty(findNode(ld32(self + 0x290), S_RIGHT_BANNER, 14), SceneUtil_createBottomOutAnimation);
    slideEmpty(findNode(ld32(self + 0x290), S_RIGHT_UI, 7), SceneUtil_createRightOutAnimation);
}

RW_FUNCTION(0x640404, "SceneMyPage::runInAnimation", SceneMyPage_runInAnimation);
RW_FUNCTION(0x640850, "SceneMyPage::setCameraPosition", SceneMyPage_setCameraPosition);
RW_FUNCTION(0x640D4C, "SceneMyPage::runOutAnimationMain", SceneMyPage_runOutAnimationMain);
