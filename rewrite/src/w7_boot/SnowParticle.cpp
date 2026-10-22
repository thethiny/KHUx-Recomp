// SnowParticle: a stage's snow weather (StageFieldManager weather 2 / 3). Style 0: one particle system; style 1
// (a snow storm): a white veil (opacity 76) and four particle systems whose gravity changes in gusts. Each
// system is a ParticleSystemQuad subclass with its own rectangle and per-style parameters (positions and gravity
// scaled to the frame size).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t SnowParticle_ctor = 0x5EC9E1, SnowParticle_setAnimationSnowStorm = 0x5ED3AD;
constexpr uint32_t SnowParticle_updateSubtractionGravity = 0x5ED7AD;
constexpr uint32_t newSystem_ = 0x5ECDF1, setupSystem_ = 0x5ECF3D;
constexpr uint32_t Node_create = 0x89372D, Sprite_create = 0x8A64DD, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t AppGetGrameFrameWidth = 0x799B31, AppGetGrameFrameHeight = 0x799D21;
constexpr uint32_t Rect_ctor = 0x8DF18B, Color3B_ctor = 0x9062E7, StringBuffer_ctor = 0x49140D;
constexpr uint32_t Vec2_ctor_ = 0x8E4B9D, Vec2_ctor_default = 0x8E4B95, Vec2_copy = 0x8E4C03, Vec2_dtor_ = 0x8E4C17;
constexpr uint32_t Vec2_scale = 0x8E50F5, Vec2_subtract = 0x8E530F, Size_ctor = 0x8DF01F, Size_assignVec2 = 0x8DF04F;
constexpr uint32_t ParticleSystemQuad_ctor = 0x8A09F9, ParticleSystem_initWithFile = 0x89D981;
constexpr uint32_t Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t GOT_Vec2_ZERO = 0xFD605C;
constexpr uint32_t S_root = 0xE02D8B, S_PlistFmt = 0xE02D78;   // "img/stage/%s.plist"
// per style: the plist name and the 12 parameter words (style 0 one entry at 0x1003E18; style 1 four entries of
// 0x30 bytes from 0x1003D20), the position / size factors (style 0: 0x1003DE0 / DE8, style 1: DF0 / DF8)
constexpr uint32_t T_STYLE0 = 0x1003E18, T_STYLE1 = 0x1003D20, T_POS0 = 0x1003DE0, T_SIZE0 = 0x1003DE8;
constexpr uint32_t T_POS1 = 0x1003DF0, T_SIZE1 = 0x1003DF8;
// the storm: gravity x / y and gust length range (frames) per strength 0..2
constexpr uint32_t T_GRAVITY_X = 0xE4DF50, T_GRAVITY_Y = 0xE4DF5C, T_GUST_MIN = 0xE4DF68, T_GUST_MAX = 0xE4DF74;
constexpr uint32_t VT_System = 0xF9585C, VT_System2 = 0xF95B8C;
constexpr uint32_t VT_GustDone = 0xF5DDE0;                 // the storm's CallFunc lambda {this}
constexpr uint32_t V_init = 0x270, V_release = 4, V_addChildZ = 0xF4, V_setName = 0x154, V_getChildByName = 0x104;
constexpr uint32_t V_getChildren = 0x10C, V_getTag = 0x148, V_setTextureRect = 0x284, V_setColor = 0x22C;
constexpr uint32_t V_setOpacity = 0x214, V_setPosition = 0x4C, V_getPosition = 0x54, V_setTag = 0x14C;
constexpr uint32_t V_getGravity = 0x284, V_setGravity = 0x288, V_setScale_ = 0x1C4;
// SnowParticle: +0x270 the position, +0x278 gust length, +0x27C gust time, +0x288 style, +0x28C storm strength
constexpr uint32_t F_POS = 0x270, F_GUST_LEN = 0x278, F_GUST_T = 0x27C, F_STYLE = 0x288, F_STRENGTH = 0x28C;
// the systems: +0x2F8..+0x304 start / end positions, +0x37C life, +0x398 flag, +0x438 rect offset, +0x440 size,
// +0x448.. the style parameters (+0x44C..+0x454 lives, +0x458..+0x474 four positions)

float f32(uint32_t bits) { float f; std::memcpy(&f, &bits, 4); return f; }
uint32_t root(uint32_t self) {
    Frame f;
    const uint32_t s = newStr(f, S_root, 4);
    const uint32_t r = vcall(self, V_getChildByName, {self, s});
    strFree(s);
    return r;
}
// a system from img/stage/<name>.plist
uint32_t systemFor(uint32_t name) {
    Frame f;
    const uint32_t buf = f.alloc(0x404);
    call(StringBuffer_ctor, {buf, S_PlistFmt, name});
    const uint32_t path = newStr(f, buf);
    const uint32_t ps = call(newSystem_, {path});
    strFree(path);
    return ps;
}
// the systems under root that have a tag
template <class Fn> void eachSystem(uint32_t node, Fn fn) {
    const uint32_t v = vcall(node, V_getChildren, {node});
    const uint32_t end = ld32(v + 4);
    for (uint32_t it = ld32(v); it != end; it += 4) {
        const uint32_t ps = ld32(it);
        if (vcall(ps, V_getTag, {ps}) != 0xFFFFFFFF) fn(ps);
    }
}
}  // namespace

namespace snow_particle {

uint32_t init(uint32_t self, uint32_t style) {
    st32(self + F_STYLE, style);
    const uint32_t r = call(Node_create, {});
    {
        Frame f;
        const uint32_t s = newStr(f, S_root, 4);
        vcall(r, V_setName, {r, s});
        strFree(s);
    }
    vcall(self, V_addChildZ, {self, r, 0});
    if (style == 0) {
        const uint32_t ps = systemFor(ld32(T_STYLE0));
        vcall(r, V_addChildZ, {r, ps, 0xFFFFFFFF});
        call(setupSystem_, {ps, T_STYLE0, T_POS0, T_SIZE0});
        vcall(ps, V_setTag, {ps, 0});
        return 1;
    }
    if (style != 1) return 0;
    const uint32_t veil = call(Sprite_create, {});
    {
        Frame f;
        const uint32_t w = call(AppGetGrameFrameWidth, {}), h = call(AppGetGrameFrameHeight, {});
        const uint32_t rect = f.alloc(16);
        call(Rect_ctor, {rect, 0, 0, w, h});
        vcall(veil, V_setTextureRect, {veil, rect});
        call(Vec2_dtor_, {rect});
        const uint32_t white = f.alloc(4);
        call(Color3B_ctor, {white, 0xFF, 0xFF, 0xFF});
        vcall(veil, V_setColor, {veil, white});
    }
    vcall(veil, V_setOpacity, {veil, 76});
    vcall(self, V_addChildZ, {self, veil, 1});
    uint32_t entry = T_STYLE1;
    int32_t z = -1;
    for (uint32_t k = 0; k < 4; k++, z--, entry += 0x30) {
        const uint32_t ps = systemFor(ld32(entry));
        vcall(r, V_addChildZ, {r, ps, uint32_t(z)});
        call(setupSystem_, {ps, entry, T_POS1, T_SIZE1});
        vcall(ps, V_setTag, {ps, k});
    }
    return 1;
}
RW_FUNCTION(0x5ECAEC, "SnowParticle::init", snow_particle::init);

// the particle system (a ParticleSystemQuad subclass) from a plist
uint32_t newSystem(uint32_t path) {
    const uint32_t ps = call(fn::operator_new, {0x478});
    call(ParticleSystemQuad_ctor, {ps});
    st32(ps, VT_System);
    st32(ps + 0x270, VT_System2);
    call(Vec2_ctor_default, {ps + 0x438});
    call(Size_ctor, {ps + 0x440});
    st32(ps + 0x448, 0);
    for (uint32_t off : {0x458u, 0x460u, 0x468u, 0x470u}) call(Vec2_copy, {ps + off, ld32(GOT_Vec2_ZERO)});
    if (call(ParticleSystem_initWithFile, {ps, path}) == 1) {
        call(Ref_autorelease, {ps});
        return ps;
    }
    vcall(ps, V_release, {ps});
    return 0;
}
RW_FUNCTION(0x5ECDF0, "SnowParticle: the particle system from a plist (sub_5ECDF0)", snow_particle::newSystem);

// its rectangle (position and size factors times the frame size) and the 12 style parameter words
uint32_t setupSystem(uint32_t ps, uint32_t params, uint32_t pos, uint32_t size) {
    Frame f;
    const uint32_t frame = f.alloc(8), p = f.alloc(8), s = f.alloc(8);
    const uint32_t w = call(AppGetGrameFrameWidth, {}), h = call(AppGetGrameFrameHeight, {});
    call(Vec2_ctor_, {frame, w, h});
    const float fw = f32(ld32(frame)), fh = f32(ld32(frame + 4));
    call(Vec2_ctor_, {p, fbits(fw * f32(ld32(pos))), fbits(fh * f32(ld32(pos + 4)))});
    call(Vec2_ctor_, {s, fbits(fw * f32(ld32(size))), fbits(fh * f32(ld32(size + 4)))});
    st32(ps + 0x43C, ld32(p + 4));
    st32(ps + 0x438, ld32(p));
    call(Size_assignVec2, {ps + 0x440, s});
    for (uint32_t k = 0; k < 12; k++) st32(ps + 0x448 + 4 * k, ld32(params + 4 * k));
    st32(ps + 0x398, 1);
    call(Vec2_dtor_, {s});
    call(Vec2_dtor_, {p});
    call(Vec2_dtor_, {frame});
    return 0;
}
RW_FUNCTION(0x5ECF3C, "SnowParticle: a system's rectangle and parameters (sub_5ECF3C)", snow_particle::setupSystem);

uint32_t create(uint32_t style) {
    const uint32_t self = call(fn::operator_new, {0x290});
    call(SnowParticle_ctor, {self});
    if (vcall(self, V_init, {self, style}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x5ED0BC, "SnowParticle::create", snow_particle::create);

// the systems placed for the frame: a positive first life (+0x44C) starts them from the first positions pair
// (scaled) with that life and scale 1; then the second pair with the life at +0x454; style 1 starts the storm
uint32_t initCoordinate(uint32_t self, uint32_t pos) {
    st32(self + F_POS + 4, ld32(pos + 4));
    st32(self + F_POS, ld32(pos));
    const uint32_t r = root(self);
    if (!r) return 0;
    eachSystem(r, [&](uint32_t ps) {
        Frame f;
        const uint32_t frame = f.alloc(8);
        call(Vec2_ctor_, {frame, call(AppGetGrameFrameWidth, {}), call(AppGetGrameFrameHeight, {})});
        const uint32_t life = ld32(ps + 0x44C);
        if (f32(life) > 0.0f) {
            const uint32_t a = f.alloc(8), b = f.alloc(8);
            call(Vec2_copy, {a, ps + 0x458});
            call(Vec2_copy, {b, ps + 0x460});
            call(Vec2_scale, {a, frame});
            call(Vec2_scale, {b, frame});
            st32(ps + 0x2FC, ld32(a + 4));
            st32(ps + 0x2F8, ld32(a));
            st32(ps + 0x304, ld32(b + 4));
            st32(ps + 0x300, ld32(b));
            vcall(ps, V_setPosition, {ps, ld32(GOT_Vec2_ZERO)});
            st32(ps + 0x37C, life);
            vcall(ps, V_setScale_, {ps, fbits(1.0f)});
            call(Vec2_dtor_, {b});
            call(Vec2_dtor_, {a});
        }
        const uint32_t a = f.alloc(8), b = f.alloc(8);
        call(Vec2_copy, {a, ps + 0x468});
        call(Vec2_copy, {b, ps + 0x470});
        call(Vec2_scale, {a, frame});
        call(Vec2_scale, {b, frame});
        st32(ps + 0x2FC, ld32(a + 4));
        st32(ps + 0x2F8, ld32(a));
        st32(ps + 0x304, ld32(b + 4));
        st32(ps + 0x300, ld32(b));
        vcall(ps, V_setPosition, {ps, ld32(GOT_Vec2_ZERO)});
        st32(ps + 0x37C, ld32(ps + 0x454));
        call(Vec2_dtor_, {b});
        call(Vec2_dtor_, {a});
        call(Vec2_dtor_, {frame});
    });
    if (ld32(self + F_STYLE) == 1) call(SnowParticle_setAnimationSnowStorm, {self});
    return 0;
}
RW_FUNCTION(0x5ED118, "SnowParticle::initCoordinate", snow_particle::initCoordinate);

// a gust: strength 1 (calm) after a gust, else 2 (strong, 30%) or 0 at random; every system's gravity and life
// for it, then a random length (frames / 30) before the next gust
uint32_t setAnimationSnowStorm(uint32_t self) {
    const uint32_t r = root(self);
    if (!r) return 0;
    uint32_t strength = 1;
    if (ld32(self + F_STRENGTH) == 1) {
        const uint32_t n = call(Random_generateUniformInt, {call(Random_getInstance, {}), 1, 100, 0});
        strength = n < 31 ? 2 : 0;
    }
    st32(self + F_STRENGTH, strength);
    const uint32_t lifeIndex = strength ? 2 : 1;
    eachSystem(r, [&](uint32_t ps) {
        Frame f;
        const uint32_t g = f.alloc(8);
        call(Vec2_ctor_, {g, ld32(T_GRAVITY_X + 4 * strength), ld32(T_GRAVITY_Y + 4 * strength)});
        vcall(ps, V_setGravity, {ps, g});
        call(Vec2_dtor_, {g});
        st32(ps + 0x37C, ld32(ps + 0x44C + 4 * lifeIndex));
    });
    const int32_t lo = int32_t(f32(ld32(T_GUST_MIN + 4 * strength)));
    const int32_t hi = int32_t(f32(ld32(T_GUST_MAX + 4 * strength)));
    const uint32_t n = call(Random_generateUniformInt, {call(Random_getInstance, {}), uint32_t(lo), uint32_t(hi), 0});
    const float len = float(int32_t(n)) / 30.0f;
    st32(self + F_GUST_LEN, fbits(len));
    st32(self + F_GUST_T, 0);
    const uint32_t delay = call(DelayTime_create, {fbits(len)});
    Frame f;
    const uint32_t fo = newFunction(f, VT_GustDone, {self});
    const uint32_t then = call(CallFunc_create, {fo});
    const uint32_t seq = call(Sequence_create, {delay, then, 0});
    functionDestroy(fo);
    call(fn::Node_runAction, {r, seq});
    return 0;
}
RW_FUNCTION(0x5ED3AC, "SnowParticle::setAnimationSnowStorm", snow_particle::setAnimationSnowStorm);

// the view moved: the new position kept (root's shifted position is computed but not applied, as in the game);
// during a calm gust every system's gravity eases; the gust time grows by dt
uint32_t updateCoordinate(uint32_t self, uint32_t pos, uint32_t dt) {
    Frame f;
    const uint32_t d = f.alloc(8);
    call(Vec2_copy, {d, self + F_POS});
    call(Vec2_subtract, {d, pos});
    st32(self + F_POS, ld32(pos));
    st32(self + F_POS + 4, ld32(pos + 4));
    const uint32_t r = root(self);
    if (r) {
        const uint32_t p = f.alloc(8);
        call(Vec2_copy, {p, vcall(r, V_getPosition, {r})});
        st32(p, fbits(f32(ld32(d)) + f32(ld32(p))));
        st32(p + 4, fbits(f32(ld32(p + 4)) - f32(ld32(d + 4))));
        eachSystem(r, [&](uint32_t ps) {
            if (ps && ld32(self + F_STYLE) == 1 && ld32(self + F_STRENGTH) == 0)
                call(SnowParticle_updateSubtractionGravity, {self, ps});
        });
        st32(self + F_GUST_T, fbits(f32(ld32(self + F_GUST_T)) + f32(dt)));
        call(Vec2_dtor_, {p});
    }
    call(Vec2_dtor_, {d});
    return 0;
}
RW_FUNCTION(0x5ED628, "SnowParticle::updateCoordinate", snow_particle::updateCoordinate);

uint32_t updateSubtractionGravity(uint32_t self, uint32_t ps) {
    Frame f;
    const uint32_t g = f.alloc(8);
    call(Vec2_copy, {g, vcall(ps, V_getGravity, {ps})});
    const float t = f32(ld32(self + F_GUST_T)) / f32(ld32(self + F_GUST_LEN));
    st32(g, fbits(t * 790.0f + -800.0f));
    st32(g + 4, fbits(t * 190.0f + -200.0f));
    vcall(ps, V_setGravity, {ps, g});
    call(Vec2_dtor_, {g});
    return 0;
}
RW_FUNCTION(0x5ED7AC, "SnowParticle::updateSubtractionGravity", snow_particle::updateSubtractionGravity);

}  // namespace snow_particle
