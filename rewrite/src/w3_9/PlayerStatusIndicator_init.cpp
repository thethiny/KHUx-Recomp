// PlayerStatusIndicator::init (building every part from the cocostudio layout), the anonymous helper classes it
// creates (Arc::create, Vibrate), and the std::function plumbing of its lambda $_0 (the HP drop animation).
#include "PlayerStatusIndicator_lib.hpp"

#include "../scene_main/FunctionThunks.hpp"

using namespace psi;
using namespace psi::lib;
using scene::Frame;
using scene::vcall;
using scene::ld32;
using scene::st32;

namespace {
constexpr uint32_t HP_LAMBDA_VTABLE = 0xF5DAD0;   // __func<PlayerStatusIndicator::init $_0, ..., void(float)> + 8
constexpr uint32_t HP_LAMBDA_TYPEINFO = 0xF939EC;
constexpr uint32_t HP_LAMBDA_TYPENAME = 0xE4D3E0;

// A long std::string built inline the way the compiler emitted it for some literals: a 0x40 / 0x30 byte heap
// block (capacity | 1, size, data), the characters copied with memcpy and terminated.
uint32_t inlineLongString(Frame &f, uint32_t literal, uint32_t len, uint32_t block) {
    uint32_t data = rw::call(guest::OPERATOR_NEW, {block});
    uint32_t s = f.alloc(12);
    st32(s, block | 1);
    st32(s + 4, len);
    st32(s + 8, data);
    rw::call(memcpy_, {data, literal, len});
    rw::save<uint8_t>(data + len, 0);
    return s;
}

// SceneUtil::searchNodeByName(root, std::string(literal))
uint32_t search(uint32_t root, uint32_t name, uint32_t len) { return scene::findNode(root, name, len); }

// widget->getVirtualRenderer() as a Label: setAdditionalKerning
void kerning(uint32_t widget, float k) {
    uint32_t label = vcall(widget, WIDGET_GET_VIRTUAL_RENDERER, {widget});
    rw::call(Label_setAdditionalKerning, {label, rw::fbits(k)});
}

uint32_t createArmature() {
    Frame f;
    uint32_t s = scene::newStr(f, STR_ACTION_MAP_ANIMATION, 18);
    uint32_t a = rw::call(Armature_create, {s});
    scene::strFree(s);
    return a;
}

// node->setPosition(other->getPosition())
void copyPosition(uint32_t node, uint32_t other) {
    uint32_t setPosition = ld32(ld32(node) + scene::node::setPosition);
    rw::call(setPosition, {node, vcall(other, scene::node::getPosition, {other})});
}
void setVisible(uint32_t node, uint32_t v) { vcall(node, scene::node::setVisible, {node, v}); }
void addChild(uint32_t parent, uint32_t child) { vcall(parent, scene::node::addChild, {parent, child}); }
void addChildZ(uint32_t parent, uint32_t child, int32_t z) {
    vcall(parent, scene::node::addChildZ, {parent, child, uint32_t(z)});
}
}  // namespace

// ------------------------------------------------------------------------------------------- init
// hpMax below 4000 gets a ring only part of the way round (3000 HP and less: 128 px of the 256.8 px ring; then
// +379.4 px per 1000 HP); a bar longer than the ring continues in the side gauge (251 px per unit).
uint32_t PlayerStatusIndicator::init(PlayerStatusIndicator *self, uint32_t hpMaxIn, uint32_t cpMax, uint32_t style) {
    uint32_t me = rw::guest_addr(self);
    rw::call(audio_load, {rw::call(audio_getInstance, {}), SE_HP_ALERT});
    uint32_t root = rw::call(Node_create, {});
    addChild(me, root);
    uint32_t parts = rw::guest_addr(&self->parts);
    rw::call(RefVector_pushBack, {parts, root});
    uint32_t layout = rw::call(GUIReader_widgetFromJsonFile, {rw::call(GUIReader_getInstance, {}), STR_LAYOUT_JSON});
    addChild(root, layout);
    vcall(layout, WIDGET_29C, {layout, 0});
    uint32_t circleBody = search(layout, STR_CIRCLE_BODY, 18);
    uint32_t circleParent = vcall(circleBody, scene::node::getParent, {circleBody});
    uint32_t sideBody = search(layout, STR_SIDE_BODY, 16);
    uint32_t sideParent = vcall(sideBody, scene::node::getParent, {sideBody});

    // ---- gauge geometry
    uint32_t hpMax;
    float hpFactor = 1.0f;
    if (hpMaxIn > 4000) {
        hpMax = hpMaxIn;
    } else {
        hpMax = hpMaxIn < 3000 ? 3000 : hpMaxIn;
        hpFactor = float(hpMax - 3000) / 1000.0f;
    }
    float length = hpFactor * rw::bitsf(0x43BDB4D0) + rw::bitsf(0x430069A0);   // * 379.413 + 128.413
    const float RING = rw::bitsf(0x438069A0);                                   // 256.825
    float sideShare = 0.0f, circleShare;
    if (length > RING || std::isnan(length)) {
        sideShare = (length + rw::bitsf(0xC38069A0)) / 251.0f;
        circleShare = 1.0f;
    } else {
        circleShare = length / RING;
    }
    self->gaugeLength = length;
    self->circleShare = (circleShare * RING) / length;

    // ---- the circular gauge
    uint32_t circle = rw::call(guest::OPERATOR_NEW, {psi::circle::SIZE});
    rw::call(Node_ctor, {circle});
    st32(circle, psi::circle::VTABLE);
    stf(circle + psi::circle::RATE_04, -1.0f);
    stf(circle + psi::circle::RATE_03, -1.0f);
    stf(circle + psi::circle::SHARE, circleShare);
    uint32_t cache = textureCache();
    uint32_t maskTex;
    {
        Frame f;
        uint32_t s = inlineLongString(f, STR_CIRCLE_MSK, 50, 0x40);
        maskTex = rw::call(TextureCache_addImage, {cache, s});
        scene::strFree(s);
    }
    uint32_t clip = rw::call(ClippingNode_create, {});
    {
        uint32_t setContentSize = ld32(ld32(clip) + scene::node::setContentSize);
        Frame f;
        uint32_t size = f.alloc(8);
        rw::call(Texture2D_getContentSize, {size, maskTex});
        rw::call(setContentSize, {clip, size});
    }
    vcall(clip, NODE_SET_CASCADE_OPACITY, {clip, 1});
    rw::call(ClippingNode_setAlphaThreshold, {clip, 0});
    rw::call(ClippingNode_setInverted, {clip, 1});
    addChild(circle, clip);
    stf(circle + psi::circle::SCALE, 1.0f);
    stf(circle + psi::circle::MASK_ROTATION, 180.0f);
    stf(circle + psi::circle::ANGLE, -270.0f);
    uint32_t stencil = rw::call(Node_create, {});
    uint32_t maskSprite = rw::call(Sprite_createWithTexture, {maskTex});
    vcall(maskSprite, scene::node::setRotation, {maskSprite, ld32(circle + psi::circle::MASK_ROTATION)});
    addChild(stencil, maskSprite);
    rw::call(ClippingNode_setStencil, {clip, stencil});
    st32(circle + psi::circle::ARC_04, arc::create(STR_CIRCLE_04));
    st32(circle + psi::circle::ARC_03, arc::create(STR_CIRCLE_03));
    arcSetRate(ld32(circle + psi::circle::ARC_04), 0.0f);
    addChild(clip, ld32(circle + psi::circle::ARC_04));
    addChild(clip, ld32(circle + psi::circle::ARC_03));
    rw::call(Ref_autorelease, {circle});
    copyPosition(circle, circleBody);
    vcall(circle, scene::node::setScale, {circle, rw::fbits(-1.0f)});
    addChildZ(circleParent, circle, -1);
    if (!(ldf(circle + psi::circle::RATE_04) == 1.0f)) {
        arcSetRate(ld32(circle + psi::circle::ARC_04), ldf(circle + psi::circle::SHARE));
        stf(circle + psi::circle::RATE_04, 1.0f);
    }
    if (!(ldf(circle + psi::circle::RATE_03) == 1.0f)) {
        arcSetRate(ld32(circle + psi::circle::ARC_03), ldf(circle + psi::circle::SHARE));
        stf(circle + psi::circle::RATE_03, 1.0f);
    }

    // ---- the HP animation node: its scaleX change runs lambda $_0
    uint32_t animNode = rw::call(guest::OPERATOR_NEW, {psi::anim::SIZE});
    rw::call(Node_ctor, {animNode});
    st32(animNode, psi::anim::VTABLE);
    st32(animNode + psi::anim::FUNCTION_PTR, 0);
    if (rw::call(Node_init, {animNode}) == 1) {
        rw::call(Ref_autorelease, {animNode});
    } else {
        vcall(animNode, 4, {animNode});
        animNode = 0;
    }
    {
        Frame f;
        uint32_t fn = scene::newFunction(f, HP_LAMBDA_VTABLE, {me});
        rw::call(FloatFunction_swap, {fn, animNode + psi::anim::FUNCTION});
        scene::functionDestroy(fn);
    }
    addChild(root, animNode);
    rw::call(RefVector_pushBack, {parts, animNode});

    // ---- numbers
    uint32_t hpNum = search(layout, STR_HP_NUM, 13);
    kerning(hpNum, -8.0f);
    rw::call(RefVector_pushBack, {parts, hpNum});
    uint32_t hpRed = search(layout, STR_HP_RED_NUM, 16);
    kerning(hpRed, -8.0f);
    setVisible(hpRed, 0);
    rw::call(RefVector_pushBack, {parts, hpRed});
    uint32_t defNum = search(layout, STR_MEDAL_DECK_LABEL, 16);
    kerning(defNum, -8.0f);
    rw::call(RefVector_pushBack, {parts, defNum});

    // ---- armatures
    {
        uint32_t mgr = rw::call(ArmatureDataManager_getInstance, {});
        Frame f;
        uint32_t s = scene::newStr(f, STR_ANIM_JSON, 48);
        rw::call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        scene::strFree(s);
    }
    uint32_t hpAlert = createArmature();
    copyPosition(hpAlert, circleBody);
    playNames(hpAlert, STR_HP_ALERT, 7, 1);
    addChildZ(root, hpAlert, -3);
    rw::call(RefVector_pushBack, {parts, hpAlert});
    uint32_t burstMaxNode = search(layout, STR_BUERST_MAX, 10);
    setVisible(burstMaxNode, 0);
    uint32_t charge = createArmature();
    copyPosition(charge, burstMaxNode);
    setVisible(charge, 0);
    addChild(root, charge);
    rw::call(RefVector_pushBack, {parts, charge});
    uint32_t use = createArmature();
    setVisible(use, 0);
    addChild(root, use);
    rw::call(RefVector_pushBack, {parts, use});
    uint32_t burstMax = createArmature();
    copyPosition(burstMax, burstMaxNode);
    playNames(burstMax, STR_BURST_MAX, 9, 1);
    addChild(root, burstMax);
    rw::call(RefVector_pushBack, {parts, burstMax});

    // ---- the avatar icon in place of the "avatar_icontest" placeholder
    uint32_t placeholder = search(layout, STR_AVATAR_ICONTEST, 15);
    uint32_t renderer = vcall(placeholder, WIDGET_GET_VIRTUAL_RENDERER, {placeholder});
    uint32_t texture = vcall(renderer, SPRITE_GET_TEXTURE, {renderer});
    uint32_t icon;
    {
        Frame f;
        uint32_t offset = f.alloc(8);
        rw::call(Vec2_ctor, {offset, rw::fbits(-75.0f), rw::fbits(113.0f)});
        // std::vector<Avatar::Expression>{1, 5}, filled the way the inlined initializer-list constructor does
        uint32_t init = f.alloc(8);
        st32(init, 1);
        st32(init + 4, 5);
        uint32_t vec = f.alloc(12);
        st32(vec, 0);
        st32(vec + 4, 0);
        st32(vec + 8, 0);
        rw::call(exprvec_allocate, {vec, 2});
        uint32_t end = ld32(vec + 4);
        for (uint32_t k = 0; k < 2; k++) {
            uint32_t at = 0;
            if (end != 0) {
                st32(end, ld32(init + 4 * k));
                at = ld32(vec + 4);
            }
            end = at + 4;
            st32(vec + 4, end);
        }
        icon = rw::call(AvatarIcon_create,
                        {style, texture, offset, rw::fbits(-1.3f), rw::fbits(1.3f), vec});
        rw::call(exprvec_baseDtor, {vec});
        rw::call(Vec2_dtor, {offset});
    }
    copyPosition(icon, placeholder);
    uint32_t iconParent = vcall(placeholder, scene::node::getParent, {placeholder});
    uint32_t addChildZFn = ld32(ld32(iconParent) + scene::node::addChildZ);
    uint32_t z = vcall(placeholder, NODE_GET_LOCAL_Z_ORDER, {placeholder});
    rw::call(addChildZFn, {iconParent, icon, z});
    vcall(placeholder, scene::node::removeFromParent, {placeholder});
    rw::call(RefVector_pushBack, {parts, icon});

    // ---- hit reaction, burst number / gauge, cost cancel effect
    {
        Frame f;
        uint32_t shake = f.alloc(8);
        rw::call(Vec2_ctor, {shake, 0, rw::fbits(10.0f)});
        uint32_t vib = vibrate::create(rw::bitsf(0x3F2AAAAB), 2.5f, shake);
        rw::call(RefVector_pushBack, {parts, vib});
        rw::call(Vec2_dtor, {shake});
    }
    uint32_t burstNum = search(layout, STR_BURST_NUM, 16);
    kerning(burstNum, -30.0f);
    rw::call(RefVector_pushBack, {parts, burstNum});
    uint32_t costCancel = createArmature();
    setVisible(costCancel, 0);
    addChildZ(root, costCancel, 1);
    rw::call(RefVector_pushBack, {parts, costCancel});
    uint32_t burstGauge = search(layout, STR_BURST_GAUGE, 11);
    rw::call(RefVector_pushBack, {parts, burstGauge});
    rw::call(RefVector_pushBack, {parts, circle});

    // ---- the side gauge for bars longer than the ring
    float sideLength = 0.0f;
    if (!(sideShare <= 0.0f || std::isnan(sideShare))) {
        uint32_t side = rw::call(guest::OPERATOR_NEW, {psi::side::SIZE});
        rw::call(Node_ctor, {side});
        st32(side, psi::side::VTABLE);
        stf(side + psi::side::RATE_04, -1.0f);
        stf(side + psi::side::RATE_03, -1.0f);
        st32(side + psi::side::SPRITES, 0);
        st32(side + psi::side::SPRITES + 4, 0);
        st32(side + psi::side::SPRITES + 8, 0);
        rw::call(Node_setAnchorPoint, {side, VEC2_ANCHOR_MIDDLE_RIGHT});
        stf(side + psi::side::SHARE, sideShare);
        uint32_t tex04, tex03;
        {
            uint32_t c = textureCache();
            Frame f;
            uint32_t s = inlineLongString(f, STR_SIDE_04, 47, 0x30);
            tex04 = rw::call(TextureCache_addImage, {c, s});
            scene::strFree(s);
        }
        {
            uint32_t c = textureCache();
            Frame f;
            uint32_t s = inlineLongString(f, STR_SIDE_03, 47, 0x30);
            tex03 = rw::call(TextureCache_addImage, {c, s});
            scene::strFree(s);
        }
        Frame f;
        uint32_t size04 = f.alloc(8), size03 = f.alloc(8);
        rw::call(Size_copy, {size04, rw::call(Texture2D_getContentSizeInPixels, {tex04})});
        rw::call(Size_copy, {size03, rw::call(Texture2D_getContentSizeInPixels, {tex03})});
        float width04 = ldf(size04) + -2.0f;
        stf(side + psi::side::WIDTH_04, width04);
        stf(side + psi::side::WIDTH_03, ldf(size03) + -2.0f);
        uint32_t rect = f.alloc(16);
        rw::call(Rect_ctor, {rect, rw::fbits(1.0f), 0, rw::fbits(width04), ld32(size04 + 4)});
        st32(side + psi::side::SPRITE_04, rw::call(Sprite_createWithTextureRect, {tex04, rect, 0}));
        rw::call(Vec2_dtor, {rect});
        uint32_t rect2 = f.alloc(16);
        rw::call(Rect_ctor, {rect2, rw::fbits(1.0f), 0, ld32(side + psi::side::WIDTH_03), ld32(size03 + 4)});
        st32(side + psi::side::SPRITE_03, rw::call(Sprite_createWithTextureRect, {tex03, rect2, 0}));
        rw::call(Vec2_dtor, {rect2});
        uint32_t s04 = ld32(side + psi::side::SPRITE_04);
        vcall(s04, scene::node::setAnchorPoint, {s04, VEC2_ANCHOR_MIDDLE_RIGHT});
        uint32_t s03 = ld32(side + psi::side::SPRITE_03);
        vcall(s03, scene::node::setAnchorPoint, {s03, VEC2_ANCHOR_MIDDLE_RIGHT});
        s04 = ld32(side + psi::side::SPRITE_04);
        vcall(s04, scene::node::setPositionX, {s04, rw::fbits(0.5f)});
        s03 = ld32(side + psi::side::SPRITE_03);
        vcall(s03, scene::node::setPositionX, {s03, rw::fbits(0.5f)});
        float len = rw::callf(roundf_, {rw::fbits(ldf(side + psi::side::SHARE) * 251.0f)});
        stf(side + psi::side::LENGTH, len);
        stf(side + psi::side::LENGTH_HALF, len + 0.5f);
        s04 = ld32(side + psi::side::SPRITE_04);
        vcall(s04, scene::node::setScaleX, {s04, 0});
        rw::call(SpriteVector_pushBack, {side + psi::side::SPRITES, ld32(side + psi::side::SPRITE_04)});
        rw::call(SpriteVector_pushBack, {side + psi::side::SPRITES, ld32(side + psi::side::SPRITE_03)});
        addChild(side, ld32(side + psi::side::SPRITE_04));
        addChild(side, ld32(side + psi::side::SPRITE_03));
        rw::call(Ref_autorelease, {side});
        copyPosition(side, sideBody);
        vcall(side, scene::node::setScaleX, {side, rw::fbits(-1.0f)});
        addChildZ(sideParent, side, -1);
        if (!(ldf(side + psi::side::RATE_04) == 1.0f)) {
            uint32_t sprite = ld32(side + psi::side::SPRITE_04);
            float scale = ldf(side + psi::side::LENGTH_HALF) / ldf(side + psi::side::WIDTH_04);
            vcall(sprite, scene::node::setScaleX, {sprite, rw::fbits(scale)});
            stf(side + psi::side::RATE_04, 1.0f);
        }
        if (!(ldf(side + psi::side::RATE_03) == 1.0f)) {
            uint32_t sprite = ld32(side + psi::side::SPRITE_03);
            float scale = ldf(side + psi::side::LENGTH_HALF) / ldf(side + psi::side::WIDTH_03);
            vcall(sprite, scene::node::setScaleX, {sprite, rw::fbits(scale)});
            stf(side + psi::side::RATE_03, 1.0f);
        }
        rw::call(RefVector_pushBack, {parts, side});
        sideLength = ldf(side + psi::side::LENGTH);
    }

    // ---- the static gauge frame texture; the layout placeholders go away
    uint32_t statusField = search(layout, STR_STATUS_FIELD, 17);
    float x = ldf(vcall(circle, scene::node::getPosition, {circle}));
    uint32_t rt = renderGaugeTexture(self, x, (1.0f - circleShare) * 270.0f, sideLength);
    addChildZ(root, rt, -2);
    vcall(circleBody, scene::node::removeFromParent, {circleBody});
    vcall(sideBody, scene::node::removeFromParent, {sideBody});
    vcall(statusField, scene::node::removeFromParent, {statusField});

    self->hpMax = hpMax;
    self->hpAlert = bt::f2uiz(float(hpMax) * rw::bitsf(0x3E4CCCCD));   // * 0.2
    self->cpMax = cpMax;
    self->hp = 0;
    setHP(self, hpMax);
    self->cp = 0;
    setCP(self, 0);
    return 1;
}
RW_FUNCTION_NOVERIFY(0x5E14C4, "PlayerStatusIndicator::init", PlayerStatusIndicator::init);

// ------------------------------------------------------------------------------------------- Arc / Vibrate
// Arc::create(path): a node with the texture as three sprites: the whole ring, its right half (anchored
// middle-left) and the upper-right quarter (anchored top-left); Arc::setRate lights one of them.
uint32_t psi::arc::create(uint32_t path) {
    uint32_t node = rw::call(guest::OPERATOR_NEW, {SIZE});
    rw::call(Node_ctor, {node});
    st32(node, VTABLE);
    st32(node + SPRITES, 0);
    st32(node + SPRITES + 4, 0);
    st32(node + SPRITES + 8, 0);
    uint32_t cache = textureCache();
    uint32_t tex;
    {
        Frame f;
        uint32_t s = scene::newStr(f, path);
        tex = rw::call(TextureCache_addImage, {cache, s});
        scene::strFree(s);
    }
    Frame f;
    uint32_t size = f.alloc(8);
    rw::call(Size_copy, {size, rw::call(Texture2D_getContentSizeInPixels, {tex})});
    vcall(node, scene::node::setContentSize, {node, size});
    uint32_t whole = rw::call(Sprite_createWithTexture, {tex});
    float halfW = ldf(size) * 0.5f;
    uint32_t rect = f.alloc(16);
    rw::call(Rect_ctor, {rect, rw::fbits(halfW), 0, rw::fbits(halfW), ld32(size + 4)});
    uint32_t right = rw::call(Sprite_createWithTextureRect, {tex, rect, 0});
    rw::call(Vec2_dtor, {rect});
    vcall(right, scene::node::setAnchorPoint, {right, VEC2_ANCHOR_MIDDLE_LEFT});
    float a = ldf(size) * 0.5f;
    float b = ldf(size + 4) * 0.5f;
    uint32_t rect2 = f.alloc(16);
    rw::call(Rect_ctor, {rect2, rw::fbits(a), rw::fbits(b), rw::fbits(a), rw::fbits(b)});
    uint32_t quarter = rw::call(Sprite_createWithTextureRect, {tex, rect2, 0});
    rw::call(Vec2_dtor, {rect2});
    vcall(quarter, scene::node::setAnchorPoint, {quarter, VEC2_ANCHOR_TOP_LEFT});
    addChild(node, whole);
    addChild(node, right);
    addChild(node, quarter);
    rw::call(SpriteVector_pushBack, {node + SPRITES, whole});
    rw::call(SpriteVector_pushBack, {node + SPRITES, right});
    rw::call(SpriteVector_pushBack, {node + SPRITES, quarter});
    rw::call(Ref_autorelease, {node});
    return node;
}
RW_FUNCTION(0x5E4C98, "(anonymous)::Arc::create (sub_5E4C98)", psi::arc::create);

// Vibrate(): Action base, duration 0, its two vtables, the two Vec2 members (+0x34 offset, +0x3C start)
uint32_t psi::vibrate::construct(uint32_t self) {
    rw::call(Action_ctor, {self});
    st32(self + 0x24, 0);
    st32(self, VTABLE);
    st32(self + 0x14, CLONABLE_VTABLE);
    rw::call(Vec2_ctor0, {self + 0x34});
    rw::call(Vec2_ctor0, {self + 0x3C});
    return self;
}
RW_FUNCTION(0x5E47EC, "(anonymous)::Vibrate::Vibrate (sub_5E47EC)", psi::vibrate::construct);

// Vibrate::create(duration, strength, offset); null when initWithDuration fails
uint32_t psi::vibrate::create(float duration, float strength, uint32_t offset) {
    uint32_t a = rw::call(guest::OPERATOR_NEW, {SIZE});
    construct(a);
    if (rw::call(ActionInterval_initWithDuration, {a, rw::fbits(duration)}) != 1) {
        rw::call(Ref_release, {a});
        return 0;
    }
    rw::save<float>(a + 0x30, strength);
    st32(a + 0x38, ld32(offset + 4));
    st32(a + 0x34, ld32(offset));
    rw::call(Ref_autorelease, {a});
    return a;
}
RW_FUNCTION(0x5E2A10, "(anonymous)::Vibrate::create (sub_5E2A10)", psi::vibrate::create);

// Vibrate::clone(): a new Vibrate with this one's duration, strength and offset (kept when initWithDuration fails:
// the copy is autoreleased either way)
static uint32_t vibrateClone(uint32_t self) {
    const uint32_t a = rw::call(guest::OPERATOR_NEW, {psi::vibrate::SIZE});
    rw::call(0x5E47ED, {a});   // Vibrate::Vibrate
    const uint32_t strength = ld32(self + 0x30);
    if (rw::call(ActionInterval_initWithDuration, {a, ld32(self + 0x24)}) == 1) {
        st32(a + 0x30, strength);
        st32(a + 0x38, ld32(self + 0x38));
        st32(a + 0x34, ld32(self + 0x34));
    }
    rw::call(Ref_autorelease, {a});
    return a;
}
RW_FUNCTION(0x5E48E4, "(anonymous)::Vibrate::clone (sub_5E48E4)", vibrateClone);

// ------------------------------------------------------------------------------------------- lambda $_0
// init $_0 [this](float scale): the HP animation node's scaleX is the HP rate shown; move the front gauge layers
// (arc04 / side sprite04) and the HP number along.
namespace PlayerStatusIndicator_lambdas {
void hpAnimation(uint32_t lambda, const float *scaleArg) {
    auto *self = rw::host_ptr<PlayerStatusIndicator>(ld32(lambda + 4));
    float share = self->circleShare;
    float scale = *scaleArg;
    float circleRate, sideRate;
    if (!(share <= scale)) {
        circleRate = scale / share;
        sideRate = 0.0f;
    } else {
        circleRate = 1.0f;
        sideRate = (share >= 1.0f || std::isnan(share)) ? 0.0f : (scale - share) / (1.0f - share);
    }
    uint32_t circle = self->part(PlayerStatusIndicator::CIRCLE_GAUGE);
    if (!(ldf(circle + psi::circle::RATE_04) == circleRate)) {
        arcSetRate(ld32(circle + psi::circle::ARC_04), circleRate * ldf(circle + psi::circle::SHARE));
        stf(circle + psi::circle::RATE_04, circleRate);
        share = self->circleShare;
    }
    if (!(share >= 1.0f || std::isnan(share))) {
        uint32_t side = self->part(PlayerStatusIndicator::SIDE_GAUGE);
        if (!(ldf(side + psi::side::RATE_04) == sideRate)) {
            uint32_t sprite = ld32(side + psi::side::SPRITE_04);
            float s = (sideRate * ldf(side + psi::side::LENGTH_HALF)) / ldf(side + psi::side::WIDTH_04);
            vcall(sprite, scene::node::setScaleX, {sprite, rw::fbits(s)});
            stf(side + psi::side::RATE_04, sideRate);
        }
    }
    PlayerStatusIndicator::setupHPText(self, bt::f2uiz(scale * float(self->hpMax.get())));
}

using HpLambda = thunk::Lambda<HP_LAMBDA_VTABLE, 8, HP_LAMBDA_TYPEINFO, HP_LAMBDA_TYPENAME, 4, thunk::cap(4, 4)>;
}  // namespace PlayerStatusIndicator_lambdas

namespace PL = PlayerStatusIndicator_lambdas;
RW_FUNCTION(0x5E41B8, "PlayerStatusIndicator::init $_0 __func::~__func (deleting)", PL::HpLambda::deleteThis);
RW_FUNCTION(0x5E41C0, "PlayerStatusIndicator::init $_0 __func::__clone()", PL::HpLambda::clone);
RW_FUNCTION(0x5E41EC, "PlayerStatusIndicator::init $_0 __func::__clone(__base*)", PL::HpLambda::cloneInto);
RW_FUNCTION(0x5E420C, "PlayerStatusIndicator::init $_0 __func::destroy", PL::HpLambda::destroy);
RW_FUNCTION(0x5E420E, "PlayerStatusIndicator::init $_0 __func::destroy_deallocate", PL::HpLambda::deleteThis);
RW_FUNCTION(0x5E4216, "PlayerStatusIndicator::init $_0 (HP animation step)", PL::hpAnimation);
RW_FUNCTION(0x5E4380, "PlayerStatusIndicator::init $_0 __func::target", PL::HpLambda::target);
RW_FUNCTION(0x5E43A0, "PlayerStatusIndicator::init $_0 __func::target_type", PL::HpLambda::targetType);
