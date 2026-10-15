#include "PlayerStatusIndicator.hpp"

#include "PlayerStatusIndicator_lib.hpp"

using namespace psi;
using namespace psi::lib;
using scene::Frame;
using scene::vcall;

// ------------------------------------------------------------------------------------------- life cycle
// Rendering the gauge textures (init) draws with OpenGL: create / init are not shadow-run.
PlayerStatusIndicator *PlayerStatusIndicator::create(uint32_t hpMax, uint32_t cpMax, uint32_t style) {
    uint32_t p = rw::call(guest::OPERATOR_NEW, {SIZE});
    rw::call(Node_ctor, {p});
    auto *self = rw::host_ptr<PlayerStatusIndicator>(p);
    rw::save<uint32_t>(p, VTABLE);
    self->alertSound = 0;
    self->partsCap = 0;
    self->partsEnd = 0;
    self->parts = 0;
    init(self, hpMax, cpMax, style);
    rw::call(Ref_autorelease, {p});
    return self;
}
RW_FUNCTION_NOVERIFY(0x5E143C, "PlayerStatusIndicator::create", PlayerStatusIndicator::create);

PlayerStatusIndicator *PlayerStatusIndicator::destruct(PlayerStatusIndicator *self) {
    uint32_t p = rw::guest_addr(self);
    rw::save<uint32_t>(p, VTABLE);
    stopAlert(self);
    rw::call(RefVector_dtor, {rw::guest_addr(&self->parts)});
    rw::call(Node_dtor, {p});
    return self;
}
RW_FUNCTION(0x5E27DC, "PlayerStatusIndicator::~PlayerStatusIndicator", PlayerStatusIndicator::destruct);

void PlayerStatusIndicator::destructDelete(PlayerStatusIndicator *self) {
    destruct(self);
    rw::call(guest::OPERATOR_DELETE, {rw::guest_addr(self)});
}
RW_FUNCTION(0x5E299C, "PlayerStatusIndicator::~PlayerStatusIndicator(deleting)", PlayerStatusIndicator::destructDelete);

// ------------------------------------------------------------------------------------------- HP
// Back to the normal HP display: white number with the current HP, alert number / animation hidden, the avatar
// icon's colour reset and the alert sound stopped.
void PlayerStatusIndicator::stopAlert(PlayerStatusIndicator *self) {
    uint32_t hpNum = self->part(HP_NUM);
    setAtlasNumber(hpNum, self->hp);
    vcall(hpNum, scene::node::setVisible, {hpNum, 1});
    uint32_t red = self->part(HP_RED_NUM);
    vcall(red, scene::node::setVisible, {red, 0});
    uint32_t alert = self->part(HP_ALERT);
    vcall(alert, scene::node::setVisible, {alert, 0});
    uint32_t icon = self->part(AVATAR_ICON);
    vcall(icon, scene::node::setColor, {icon, COLOR3B_WHITE});
    uint32_t sound = self->alertSound;
    if (sound != 0) {
        rw::call(audio_stop, {rw::call(audio_getInstance, {}), sound, 50});
        self->alertSound = 0;
    }
}
RW_FUNCTION(0x5E2838, "PlayerStatusIndicator::stopAlert", PlayerStatusIndicator::stopAlert);

// The HP number: below the alert threshold (and not 0) the red number, the alert animation, a reddened icon and
// the alert sound (started once); otherwise the normal display. The icon's expression is reset either way.
void PlayerStatusIndicator::setupHPText(PlayerStatusIndicator *self, uint32_t hp) {
    uint32_t alertBelow = self->hpAlert;
    uint32_t icon = self->part(AVATAR_ICON);
    uint32_t hpNum = self->part(HP_NUM);
    if (alertBelow > hp && self->hp != 0) {
        uint32_t alert = self->part(HP_ALERT);
        uint32_t red = self->part(HP_RED_NUM);
        setAtlasNumber(red, hp);
        vcall(hpNum, scene::node::setVisible, {hpNum, 0});
        vcall(red, scene::node::setVisible, {red, 1});
        vcall(alert, scene::node::setVisible, {alert, 1});
        {
            Frame f;
            uint32_t color = f.alloc(4);
            rw::call(Color3B_ctor, {color, 0xFF, 0x80, 0x80});
            vcall(icon, scene::node::setColor, {icon, color});
        }
        if (self->alertSound == 0) {
            Frame f;
            uint32_t layer = f.alloc(8);   // std::shared_ptr<audio::Layer>
            rw::call(audio_getLayer, {layer, rw::call(audio_getInstance, {}), 0});
            uint32_t sound = rw::call(audioLayer_playSE, {scene::ld32(layer), SE_HP_ALERT, 0, 0});
            scene::sharedRelease(layer);
            self->alertSound = sound;
        }
    } else {
        stopAlert(self);
        setAtlasNumber(hpNum, hp);
    }
    rw::call(AvatarIcon_changeExpression, {icon, 1, 0});
}
RW_FUNCTION(0x5E3980, "PlayerStatusIndicator::setupHPText", PlayerStatusIndicator::setupHPText);

// circle = the circle gauge's rate for an HP rate, side = the side gauge's (only when there is one).
uint32_t PlayerStatusIndicator::calculateFractions(PlayerStatusIndicator *self, float rate, const float *circle,
                                                   const float *side) {
    float share = self->circleShare;
    float c = share <= rate ? 1.0f : rate / share;
    rw::store32(circle, rw::fbits(c));
    share = self->circleShare;
    if (share >= 1.0f || std::isnan(share)) return 1;   // (r0 = the comparison result)
    float s = (share > rate || std::isnan(rate)) ? 0.0f : (rate - share) / (1.0f - share);
    rw::store32(side, rw::fbits(s));
    return rw::fbits(s);
}
RW_FUNCTION(0x5E38C8, "PlayerStatusIndicator::calculateFractions", PlayerStatusIndicator::calculateFractions);

// A new HP value. Gaining HP (or no animation running below it) sets both gauge layers and the number at once;
// losing HP shakes the indicator, sets the front layer and lets the HP animation node scale down to the new rate
// (its function moves the back layer and the number along).
void PlayerStatusIndicator::setHP(PlayerStatusIndicator *self, uint32_t hp) {
    if (self->hp == hp) return;
    float rate = float(hp) / float(self->hpMax.get());
    float share = self->circleShare;
    float circleRate = share <= rate ? 1.0f : rate / share;
    float sideRate = 0.0f;
    if (!(share >= 1.0f || std::isnan(share)) && !(share > rate || std::isnan(rate)))
        sideRate = (rate - share) / (1.0f - share);
    uint32_t circle = self->part(CIRCLE_GAUGE);
    if (!(ldf(circle + psi::circle::RATE_03) == circleRate)) {
        arcSetRate(scene::ld32(circle + psi::circle::ARC_03), circleRate * ldf(circle + psi::circle::SHARE));
        stf(circle + psi::circle::RATE_03, circleRate);
        share = self->circleShare;
    }
    if (!(share >= 1.0f || std::isnan(share))) {
        uint32_t side = self->part(SIDE_GAUGE);
        if (!(ldf(side + psi::side::RATE_03) == sideRate)) {
            uint32_t sprite = scene::ld32(side + psi::side::SPRITE_03);
            float scale = (sideRate * ldf(side + psi::side::LENGTH_HALF)) / ldf(side + psi::side::WIDTH_03);
            vcall(sprite, scene::node::setScaleX, {sprite, rw::fbits(scale)});
            stf(side + psi::side::RATE_03, sideRate);
        }
    }
    uint32_t animNode = self->part(HP_ANIMATION);
    rw::call(Node_stopAllActions, {animNode});
    float shown = rw::bitsf(vcall(animNode, scene::node::getScale, {animNode}));
    if (rate >= shown || std::isnan(rate) || std::isnan(shown)) {
        if (!(ldf(circle + psi::circle::RATE_04) == circleRate)) {
            arcSetRate(scene::ld32(circle + psi::circle::ARC_04), circleRate * ldf(circle + psi::circle::SHARE));
            stf(circle + psi::circle::RATE_04, circleRate);
        }
        float share2 = self->circleShare;
        if (!(share2 >= 1.0f || std::isnan(share2))) {
            uint32_t side = self->part(SIDE_GAUGE);
            if (!(ldf(side + psi::side::RATE_04) == sideRate)) {
                uint32_t sprite = scene::ld32(side + psi::side::SPRITE_04);
                float scale = (sideRate * ldf(side + psi::side::LENGTH_HALF)) / ldf(side + psi::side::WIDTH_04);
                vcall(sprite, scene::node::setScaleX, {sprite, rw::fbits(scale)});
                stf(side + psi::side::RATE_04, sideRate);
            }
        }
        vcall(animNode, scene::node::setScale, {animNode, rw::fbits(rate)});
        self->hp = hp;
        setupHPText(self, hp);
        return;
    }
    // damage: shake, then animate down from the larger of the shown and the previous rate
    rw::call(AvatarIcon_changeExpression, {self->part(AVATAR_ICON), 5, 0});
    uint32_t root = self->part(ROOT);
    rw::call(Node_stopAllActions, {root});
    {
        Frame f;
        uint32_t zero = f.alloc(8);
        rw::call(Vec2_ctor, {zero, 0, 0});
        vcall(root, scene::node::setPosition, {root, zero});
        rw::call(Vec2_dtor, {zero});
    }
    rw::call(Node_runAction, {root, self->part(VIBRATE)});
    float previous = float(self->hp.get()) / float(self->hpMax.get());
    float from = shown < previous ? previous : shown;
    vcall(animNode, scene::node::setScale, {animNode, rw::fbits(from)});
    uint32_t delay = rw::call(DelayTime_create, {rw::fbits(0.5f)});
    uint32_t scale = rw::call(EaseSineOut_create, {rw::call(ScaleTo_create, {rw::fbits(0.5f), rw::fbits(rate)})});
    uint32_t seq = rw::call(Sequence_create, {delay, scale, 0});
    rw::call(Node_runAction, {animNode, seq});
    self->hp = hp;
}
RW_FUNCTION(0x5E3118, "PlayerStatusIndicator::setHP", PlayerStatusIndicator::setHP);

// ------------------------------------------------------------------------------------------- burst gauge / def
// The burst gauge: number of full gauges, the use / charge animation when it changed, the bar (percent of the
// current gauge) and the MAX animation when full.
void PlayerStatusIndicator::setCP(PlayerStatusIndicator *self, uint32_t cp) {
    uint32_t value = self->cpMax.get() < cp ? self->cpMax.get() : cp;
    uint32_t points = value / 10000;
    uint32_t burstNum = self->part(BURST_NUM);
    uint32_t oldPoints = self->cp;
    Frame f;
    uint32_t text = scene::stringBuffer(f, {STR_D, points});
    {
        uint32_t s = scene::newStr(f, text);
        rw::call(TextAtlas_setString, {burstNum, s});
        scene::strFree(s);
    }
    oldPoints /= 10000;
    if (points < oldPoints) {
        uint32_t use = self->part(BURST_USE);
        playNames(use, STR_BURST_USE, 9, 0);
        // placed right of the number: 70 px per digit, overlapping by 30
        uint32_t digits = scene::ld32(text + 1024);    // StringBuffer length
        float width = float(digits) * 70.0f + float(digits - 1) * -30.0f;
        placeRightOf(use, burstNum, width);
        vcall(use, scene::node::setVisible, {use, 1});
    } else if (points > oldPoints) {
        uint32_t charge = self->part(BURST_CHARGE);
        playNames(charge, STR_BURST_CHARGE, 12, 0);
        vcall(charge, scene::node::setVisible, {charge, 1});
    }
    uint32_t burstMax = self->part(BURST_MAX);
    uint32_t bar = self->part(BURST_GAUGE);
    if (value == self->cpMax) {
        rw::call(LoadingBar_setPercent, {bar, rw::fbits(100.0f)});
        vcall(burstMax, scene::node::setVisible, {burstMax, 1});
    } else {
        rw::call(LoadingBar_setPercent, {bar, rw::fbits(float(value % 10000 / 100))});
        vcall(burstMax, scene::node::setVisible, {burstMax, 0});
    }
    self->cp = value;
}
RW_FUNCTION(0x5E34D8, "PlayerStatusIndicator::setCP", PlayerStatusIndicator::setCP);

// The defense number (at most 99999).
void PlayerStatusIndicator::setDef(PlayerStatusIndicator *self, uint32_t def) {
    uint32_t label = self->part(DEF_NUM);
    setAtlasNumber(label, def > 99999 ? 99999 : def);
}
RW_FUNCTION(0x5E3B98, "PlayerStatusIndicator::setDef", PlayerStatusIndicator::setDef);

// The "cost cancel" effect next to the burst number (one or two digits wide).
void PlayerStatusIndicator::playEffectCostCancel(PlayerStatusIndicator *self) {
    uint32_t burstNum = self->part(BURST_NUM);
    uint32_t arm = self->part(COST_CANCEL);
    vcall(arm, scene::node::setVisible, {arm, 1});
    uint32_t anim = vcall(arm, ARMATURE_GET_ANIMATION, {arm});
    uint32_t play = scene::ld32(scene::ld32(anim) + ANIM_PLAY);
    {
        Frame f;
        uint32_t s = scene::newStr(f, STR_SKILL_COST_CANCEL, 16);
        rw::call(play, {anim, s, 0xFFFFFFFFu, 0xFFFFFFFFu});
        scene::strFree(s);
    }
    int32_t digits = self->cp >= 100000 ? 2 : 1;
    float width = float(digits) * 70.0f + float(digits - 1) * -30.0f;
    placeRightOf(arm, burstNum, width);
}
RW_FUNCTION(0x5E3C64, "PlayerStatusIndicator::playEffectCostCancel", PlayerStatusIndicator::playEffectCostCancel);

// ------------------------------------------------------------------------------------------- textures
// A size x size render texture with a white wedge: the stencil that cuts the gauge ring at `angle` degrees
// (more than 30: two triangles covering the quarter; else one triangle up to tan(45 deg)). OpenGL drawing.
uint32_t PlayerStatusIndicator::renderMaskTexture(PlayerStatusIndicator *, int32_t size, float angle) {
    uint32_t rt = rw::call(RenderTexture_create, {uint32_t(size), uint32_t(size)});
    vcall(rt, RT_BEGIN_WITH_CLEAR, {rt, 0, 0, 0, 0});
    bool wide = angle > 30.0f || std::isnan(angle);
    float half = float(int32_t(uint32_t(size) + (uint32_t(size) >> 31)) >> 1);
    uint32_t dn = rw::call(DrawNode_create, {});
    Frame f;
    uint32_t a = f.alloc(8), b = f.alloc(8), c = f.alloc(8);
    if (wide) {
        rw::call(Vec2_ctor, {a, rw::fbits(half), 0});
        rw::call(Vec2_ctor, {b, 0, 0});
        rw::call(Vec2_ctor, {c, rw::fbits(half), rw::fbits(half)});
        rw::call(DrawNode_drawTriangle, {dn, a, b, c, COLOR4F_WHITE});
        rw::call(Vec2_dtor, {c});
        rw::call(Vec2_dtor, {b});
        rw::call(Vec2_dtor, {a});
        rw::call(Vec2_ctor, {a, rw::fbits(half), rw::fbits(half)});
        rw::call(Vec2_ctor, {b, 0, rw::fbits(half)});
        rw::call(Vec2_ctor, {c, 0, 0});
        rw::call(DrawNode_drawTriangle, {dn, a, b, c, COLOR4F_WHITE});
        rw::call(Vec2_dtor, {c});
        rw::call(Vec2_dtor, {b});
    } else {
        rw::call(Vec2_ctor, {a, rw::fbits(half), 0});
        rw::call(Vec2_ctor, {b, 0, 0});
        float t = rw::callf(tanf_, {QUARTER_PI});
        rw::call(Vec2_ctor, {c, rw::fbits(half), rw::fbits(half * t)});
        rw::call(DrawNode_drawTriangle, {dn, a, b, c, COLOR4F_WHITE});
        rw::call(Vec2_dtor, {c});
        rw::call(Vec2_dtor, {b});
    }
    rw::call(Vec2_dtor, {a});
    uint32_t setPosition = scene::ld32(scene::ld32(dn) + scene::node::setPosition);
    uint32_t center = f.alloc(8);
    rw::call(Vec2_ctor, {center, rw::fbits(half), rw::fbits(half)});
    rw::call(setPosition, {dn, center});
    rw::call(Vec2_dtor, {center});
    rw::call(Node_visit, {dn});
    vcall(rt, RT_END, {rt});
    return rt;
}
RW_FUNCTION_NOVERIFY(0x5E3DEC, "PlayerStatusIndicator::renderMaskTexture", PlayerStatusIndicator::renderMaskTexture);

// The static gauge background as one texture: the ring frame (Avatar_Circle_01 / 05 / 06 by the angle the HP
// bar covers: below 90, below 180, more) clipped by the wedge mask over avatar_gauge.png, and, for long bars,
// the side frame (Avatar_Side_01 stretched to sideLength, Avatar_Side_02 as its end cap). OpenGL drawing.
uint32_t PlayerStatusIndicator::renderGaugeTexture(PlayerStatusIndicator *, float x, float angle,
                                                   float sideLength) {
    uint32_t frameTex;
    if (!(angle >= 90.0f || std::isnan(angle))) {
        frameTex = addImage(STR_CIRCLE_01, 49);
    } else {
        bool full = angle >= 180.0f || std::isnan(angle);
        uint32_t cache = textureCache();
        frameTex = full ? addImageTo(cache, STR_CIRCLE_06, 49) : addImageTo(cache, STR_CIRCLE_05, 49);
    }
    float size = ldf(rw::call(Texture2D_getContentSizeInPixels, {frameTex}));
    // the wedge angle: angle mod 90, 90 when it divides evenly (a double fmod like the original)
    double d = double(angle);
    uint64_t m = rw::call64(fmod_, {uint32_t(rw_dbits(d)), uint32_t(rw_dbits(d) >> 32), 0, 0x40568000u});
    double md = rw_bitsd(m);
    bool even = md == 0.0;
    float wedge = even ? 90.0f : float(md);
    double wedgeD = even ? 90.0 : md;
    uint32_t mask = renderMaskTexture(nullptr, bt::f2iz(size), wedge);
    uint32_t cache = textureCache();
    uint32_t gaugeTex;
    float halfSize;
    {
        Frame f;
        uint32_t s = scene::newStr(f, STR_AVATAR_GAUGE, 35);
        halfSize = size * 0.5f;
        gaugeTex = rw::call(TextureCache_addImage, {cache, s});
        scene::strFree(s);
    }
    Frame f;
    uint32_t gaugeSize = f.alloc(8);
    rw::call(Size_copy, {gaugeSize, rw::call(Texture2D_getContentSizeInPixels, {gaugeTex})});
    uint32_t rt = rw::call(RenderTexture_create_fmt,
                           {uint32_t(bt::f2iz(ldf(gaugeSize))), uint32_t(bt::f2iz(ldf(gaugeSize + 4))), 2, 0x88F0});
    uint32_t rtSprite = scene::ld32(rt + RT_SPRITE);
    vcall(rtSprite, scene::node::setAnchorPoint, {rtSprite, VEC2_ANCHOR_BOTTOM_LEFT});
    vcall(rt, RT_BEGIN_WITH_CLEAR, {rt, 0, 0, 0, 0});
    rw::call(Texture2D_setAliasTexParameters, {gaugeTex});
    uint32_t gauge = rw::call(Sprite_createWithTexture, {gaugeTex});
    vcall(gauge, scene::node::setAnchorPoint, {gauge, VEC2_ANCHOR_BOTTOM_LEFT});
    rw::call(Node_visit, {gauge});
    // the ring frame, clipped by the wedge
    uint32_t clip = rw::call(ClippingNode_create, {});
    uint32_t clipSize = f.alloc(8);
    rw::call(Size_ctor, {clipSize, rw::fbits(size), rw::fbits(size)});
    vcall(clip, scene::node::setContentSize, {clip, clipSize});
    vcall(clip, NODE_SET_CASCADE_OPACITY, {clip, 1});
    rw::call(ClippingNode_setAlphaThreshold, {clip, 0});
    rw::call(ClippingNode_setInverted, {clip, 1});
    uint32_t pos = f.alloc(8);
    rw::call(Vec2_ctor, {pos, rw::fbits(x), rw::fbits(halfSize)});
    vcall(clip, scene::node::setPosition, {clip, pos});
    rw::call(Vec2_dtor, {pos});
    rw::call(ClippingNode_setStencil, {clip, mask});
    if (wedgeD == 90.0) rw::call(Texture2D_setAliasTexParameters, {frameTex});
    uint32_t frame = rw::call(Sprite_createWithTexture, {frameTex});
    vcall(frame, scene::node::setRotation, {frame, rw::fbits(180.0f - angle)});
    vcall(clip, scene::node::addChild, {clip, frame});
    rw::call(Node_visit, {clip});
    if (!(sideLength <= 0.0f || std::isnan(sideLength))) {
        uint32_t sideCache = textureCache();
        uint32_t sideTex;
        float sideY;
        {
            Frame g;
            uint32_t s = scene::newStr(g, STR_SIDE_01, 47);
            sideY = halfSize + 54.0f;
            sideTex = rw::call(TextureCache_addImage, {sideCache, s});
            scene::strFree(s);
        }
        uint32_t sideSize = f.alloc(8);
        rw::call(Size_copy, {sideSize, rw::call(Texture2D_getContentSizeInPixels, {sideTex})});
        float width = ldf(sideSize) + -2.0f;
        uint32_t rect = f.alloc(16);
        rw::call(Rect_ctor, {rect, rw::fbits(1.0f), 0, rw::fbits(width), scene::ld32(sideSize + 4)});
        uint32_t bar = rw::call(Sprite_createWithTextureRect, {sideTex, rect, 0});
        rw::call(Vec2_dtor, {rect});
        rw::call(Texture2D_setAliasTexParameters, {vcall(bar, SPRITE_GET_TEXTURE, {bar})});
        vcall(bar, scene::node::setAnchorPoint, {bar, VEC2_ANCHOR_MIDDLE_LEFT});
        vcall(bar, scene::node::setScaleX, {bar, rw::fbits(sideLength / width)});
        uint32_t p1 = f.alloc(8);
        rw::call(Vec2_ctor, {p1, rw::fbits(x), rw::fbits(sideY)});
        vcall(bar, scene::node::setPosition, {bar, p1});
        rw::call(Vec2_dtor, {p1});
        rw::call(Node_visit, {bar});
        uint32_t cap;
        {
            Frame g;
            uint32_t s = scene::newStr(g, STR_SIDE_02, 47);
            cap = rw::call(Sprite_create, {s});
            scene::strFree(s);
        }
        rw::call(Texture2D_setAliasTexParameters, {vcall(cap, SPRITE_GET_TEXTURE, {cap})});
        vcall(cap, scene::node::setAnchorPoint, {cap, VEC2_ANCHOR_MIDDLE_LEFT});
        float capX = x + sideLength;
        uint32_t p2 = f.alloc(8);
        rw::call(Vec2_ctor, {p2, rw::fbits(capX), rw::fbits(sideY)});
        vcall(cap, scene::node::setPosition, {cap, p2});
        rw::call(Vec2_dtor, {p2});
        rw::call(Node_visit, {cap});
    }
    vcall(rt, RT_END, {rt});
    uint32_t sprite = scene::ld32(rt + RT_SPRITE);
    rw::call(Texture2D_setAntiAliasTexParameters, {vcall(sprite, SPRITE_GET_TEXTURE, {sprite})});
    return rt;
}
RW_FUNCTION_NOVERIFY(0x5E2A78, "PlayerStatusIndicator::renderGaugeTexture", PlayerStatusIndicator::renderGaugeTexture);
