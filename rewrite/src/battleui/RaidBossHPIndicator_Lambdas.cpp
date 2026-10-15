// RaidBossHPIndicator's translation unit, the rest: the lambdas it hands to std::function (bar percent setters
// for the AnimationNodes, animation-finished callbacks) with their std::function thunks, AnimationNode, and the
// anonymous Arc gauge class compiled into the same file.
#include "RaidBossHPIndicator.hpp"

using namespace scene;
using namespace raidhp;

// ------------------------------------------------------------------------------------------- std::function thunks
// libc++ __func<Lambda> vtable (from +8): ~__func (shared), deleting dtor, __clone(), __clone(dst), destroy(),
// destroy_deallocate(), operator(), target(type_info), target_type(). Every lambda here captures `this` (one word,
// an 8-byte __func: vtable, this).
namespace {
template <uint32_t VTABLE> struct Func1 {
    // deleting destructor / destroy_deallocate()
    static void deleteThis(uint32_t self) { rw::call(guest::OPERATOR_DELETE, {self}); }
    // __clone(): a heap copy (none when the allocation failed)
    static uint32_t clone(uint32_t self) {
        uint32_t p = rw::call(guest::OPERATOR_NEW, {8});
        if (p) {
            rw::save<uint32_t>(p, VTABLE);
            rw::save<uint32_t>(p + 4, rw::load<uint32_t>(self + 4));
        }
        return p;
    }
    // __clone(dst): copy into dst; r0 is the copied word (this when dst is null)
    static uint32_t cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return self;
        rw::save<uint32_t>(dst, VTABLE);
        uint32_t w = rw::load<uint32_t>(self + 4);
        rw::save<uint32_t>(dst + 4, w);
        return w;
    }
    // destroy(): the lambda is trivially destructible
    static void destroy(uint32_t) {}
};
// target(ti): the lambda when ti's name is the lambda's type name
template <uint32_t TYPE_NAME> uint32_t target(uint32_t self, uint32_t typeInfo) {
    return rw::load<uint32_t>(typeInfo + 4) == TYPE_NAME ? self + 4 : 0;
}
// target_type(): the lambda's typeinfo
template <uint32_t TYPE_INFO> uint32_t targetType() { return TYPE_INFO; }

constexpr uint32_t VT_ANIM_RED = 0xF5DC88;      // init()::$_0
constexpr uint32_t VT_ANIM_GREEN = 0xF5DC5C;    // init()::$_1
constexpr uint32_t VT_SWITCH_MODE = 0xF5DC30;   // switchModeFieldToBattle()::$_2
constexpr uint32_t VT_HP_DOWN = 0xF5DC04;       // setHP()::$_3
constexpr uint32_t VT_HP_UP = 0xF5DBD8;         // setHP()::$_4
constexpr uint32_t VT_NEXT_DISP = 0xF5DBAC;     // Next_Disp()::$_5

RaidBossHPIndicator *captured(uint32_t func) { return rw::host_ptr<RaidBossHPIndicator>(rw::load<uint32_t>(func + 4)); }
}  // namespace

// ------------------------------------------------------------------------------------------- lambda bodies
namespace {
struct RaidBossHPIndicatorLambdas {
    // init()::$_0 / $_1: an AnimationNode's scaleX is the bar's fill
    static void redBarRate(uint32_t func, const float *rate) {
        RaidBossHPIndicator *self = captured(func);
        setPercent(gauge(self, 4), *rate * 100.0f);
    }
    static void greenBarRate(uint32_t func, const float *rate) {
        RaidBossHPIndicator *self = captured(func);
        setPercent(gauge(self, 3), *rate * 100.0f);
    }

    // switchModeFieldToBattle()::$_2: the coerce effect finished
    static void coerceEffectEnd(uint32_t func) {
        RaidBossHPIndicator *self = captured(func);
        setVisible(self->coerceEffect, 0);
        self->coerceDirEnd = 1;
    }

    // setHP()::$_3: the red bar finished draining
    static void hpDownEnd(uint32_t func) {
        RaidBossHPIndicator *self = captured(func);
        uint32_t me = rw::guest_addr(self);
        if (self->boxBroken && self->gaugeResetPending) {
            {
                TempStr s(str::RAID_GAUGE_HP, 13);
                RaidBossHPIndicator::setGaugeLength(self, search(me, s), 1.0f);
            }
            self->boxBroken = 0;
            self->gaugeResetPending = 0;
            self->dropCount = 0;
        }
        uint32_t target = self->hpTarget;
        uint32_t cur = self->hp;
        if (cur < target) {
            RaidBossHPIndicator::setHPDownDir(self);          // more of the damage to show (next box)
            return;
        }
        uint32_t per = self->hpPerBox;
        uint32_t rem = umod(cur, per);
        if (cur == target && cur != 0 && rem == 0) {
            RaidBossHPIndicator::setHPDownJustDir(self);
        } else {
            self->dropCount = 0;
            if (self->hpMax > per) RaidBossHPIndicator::setBoxHpGauge(self, self->boxLeft, self->boxLeft);
        }
    }

    // setHP()::$_4: the heal animation of one gauge finished
    static void hpUpEnd(uint32_t func) {
        RaidBossHPIndicator *self = captured(func);
        uint32_t me = rw::guest_addr(self);
        uint32_t per = self->hpPerBox;
        uint32_t target = self->hpTarget;
        uint32_t cur = self->hp;
        bool refill = cur > target;
        if (!refill) {
            uint32_t rem = umod(cur, per);
            refill = cur == target && cur != 0 && rem == 0;
        }
        if (refill && self->hpMax > per) {
            // the heal filled a box: one more box
            uint32_t boxes = boxesFor(cur, per);
            RaidBossHPIndicator::setBoxHpGauge(self, self->boxLeft, boxes);
            self->boxLeft = boxes;
            RaidBossHPIndicator::setBoxCountText(self, childByName(me, str::PANEL_4, 7));
        }
        target = self->hpTarget;
        cur = self->hp;
        if (cur > target) {
            RaidBossHPIndicator::setHPUpDir(self);
        } else {
            uint32_t rem = umod(cur, self->hpPerBox);
            if (cur == target && cur != 0 && rem == 0) RaidBossHPIndicator::setHPUpJustDir(self);
            else RaidBossHPIndicator::setHPUpDirEnd(self);
        }
    }

    // Next_Disp()::$_5: show the next active buff (hide the icon when none is left)
    static void nextBuff(uint32_t func) {
        RaidBossHPIndicator *self = captured(func);
        uint32_t flags = self->buffFlags;
        if (!flags) {
            setVisible(self->buffIcon, 0);
            return;
        }
        uint32_t idx = self->buffShown;
        do {
            uint32_t next = idx + 1;
            idx = next <= 19 ? next : 0;
        } while (!(bit(idx + 1) & flags));
        self->buffShown = idx;
        RaidBossHPIndicator::Next_Disp(self);
    }
};

using L = RaidBossHPIndicatorLambdas;
}  // namespace
RW_FUNCTION(0x5EAF48, "RaidBossHPIndicator::init()::$_0::operator()", L::redBarRate);
RW_FUNCTION(0x5EAE90, "RaidBossHPIndicator::init()::$_1::operator()", L::greenBarRate);
RW_FUNCTION(0x5EADDE, "RaidBossHPIndicator::switchModeFieldToBattle()::$_2::operator()", L::coerceEffectEnd);
RW_FUNCTION(0x5EABF8, "RaidBossHPIndicator::setHP()::$_3::operator()", L::hpDownEnd);
RW_FUNCTION(0x5EA9D0, "RaidBossHPIndicator::setHP()::$_4::operator()", L::hpUpEnd);
RW_FUNCTION(0x5EA8F6, "RaidBossHPIndicator::Next_Disp()::$_5::operator()", L::nextBuff);

// $_0
RW_FUNCTION(0x5EAEE8, "__func<init()::$_0>::~__func (deleting)", Func1<VT_ANIM_RED>::deleteThis);
RW_FUNCTION(0x5EAEF0, "__func<init()::$_0>::__clone()", Func1<VT_ANIM_RED>::clone);
RW_FUNCTION(0x5EAF1C, "__func<init()::$_0>::__clone(dst)", Func1<VT_ANIM_RED>::cloneInto);
RW_FUNCTION(0x5EAF3C, "__func<init()::$_0>::destroy", Func1<VT_ANIM_RED>::destroy);
RW_FUNCTION(0x5EAF3E, "__func<init()::$_0>::destroy_deallocate", Func1<VT_ANIM_RED>::deleteThis);
RW_FUNCTION(0x5EAF6C, "__func<init()::$_0>::target", target<0xE4DB40>);
RW_FUNCTION(0x5EAF8C, "__func<init()::$_0>::target_type", targetType<0xF94A1C>);
// $_1
RW_FUNCTION(0x5EAE30, "__func<init()::$_1>::~__func (deleting)", Func1<VT_ANIM_GREEN>::deleteThis);
RW_FUNCTION(0x5EAE38, "__func<init()::$_1>::__clone()", Func1<VT_ANIM_GREEN>::clone);
RW_FUNCTION(0x5EAE64, "__func<init()::$_1>::__clone(dst)", Func1<VT_ANIM_GREEN>::cloneInto);
RW_FUNCTION(0x5EAE84, "__func<init()::$_1>::destroy", Func1<VT_ANIM_GREEN>::destroy);
RW_FUNCTION(0x5EAE86, "__func<init()::$_1>::destroy_deallocate", Func1<VT_ANIM_GREEN>::deleteThis);
RW_FUNCTION(0x5EAEB4, "__func<init()::$_1>::target", target<0xE4DA80>);
RW_FUNCTION(0x5EAED4, "__func<init()::$_1>::target_type", targetType<0xF94A08>);
// $_2
RW_FUNCTION(0x5EAD80, "__func<switchModeFieldToBattle()::$_2>::~__func (deleting)", Func1<VT_SWITCH_MODE>::deleteThis);
RW_FUNCTION(0x5EAD88, "__func<switchModeFieldToBattle()::$_2>::__clone()", Func1<VT_SWITCH_MODE>::clone);
RW_FUNCTION(0x5EADB4, "__func<switchModeFieldToBattle()::$_2>::__clone(dst)", Func1<VT_SWITCH_MODE>::cloneInto);
RW_FUNCTION(0x5EADD4, "__func<switchModeFieldToBattle()::$_2>::destroy", Func1<VT_SWITCH_MODE>::destroy);
RW_FUNCTION(0x5EADD6, "__func<switchModeFieldToBattle()::$_2>::destroy_deallocate", Func1<VT_SWITCH_MODE>::deleteThis);
RW_FUNCTION(0x5EADFC, "__func<switchModeFieldToBattle()::$_2>::target", target<0xE4D9C0>);
RW_FUNCTION(0x5EAE1C, "__func<switchModeFieldToBattle()::$_2>::target_type", targetType<0xF949F4>);
// $_3
RW_FUNCTION(0x5EAB98, "__func<setHP()::$_3>::~__func (deleting)", Func1<VT_HP_DOWN>::deleteThis);
RW_FUNCTION(0x5EABA0, "__func<setHP()::$_3>::__clone()", Func1<VT_HP_DOWN>::clone);
RW_FUNCTION(0x5EABCC, "__func<setHP()::$_3>::__clone(dst)", Func1<VT_HP_DOWN>::cloneInto);
RW_FUNCTION(0x5EABEC, "__func<setHP()::$_3>::destroy", Func1<VT_HP_DOWN>::destroy);
RW_FUNCTION(0x5EABEE, "__func<setHP()::$_3>::destroy_deallocate", Func1<VT_HP_DOWN>::deleteThis);
RW_FUNCTION(0x5EAD4C, "__func<setHP()::$_3>::target", target<0xE4D920>);
RW_FUNCTION(0x5EAD6C, "__func<setHP()::$_3>::target_type", targetType<0xF949E0>);
// $_4
RW_FUNCTION(0x5EA970, "__func<setHP()::$_4>::~__func (deleting)", Func1<VT_HP_UP>::deleteThis);
RW_FUNCTION(0x5EA978, "__func<setHP()::$_4>::__clone()", Func1<VT_HP_UP>::clone);
RW_FUNCTION(0x5EA9A4, "__func<setHP()::$_4>::__clone(dst)", Func1<VT_HP_UP>::cloneInto);
RW_FUNCTION(0x5EA9C4, "__func<setHP()::$_4>::destroy", Func1<VT_HP_UP>::destroy);
RW_FUNCTION(0x5EA9C6, "__func<setHP()::$_4>::destroy_deallocate", Func1<VT_HP_UP>::deleteThis);
RW_FUNCTION(0x5EAB64, "__func<setHP()::$_4>::target", target<0xE4D890>);
RW_FUNCTION(0x5EAB84, "__func<setHP()::$_4>::target_type", targetType<0xF949CC>);
// $_5
RW_FUNCTION(0x5EA898, "__func<Next_Disp()::$_5>::~__func (deleting)", Func1<VT_NEXT_DISP>::deleteThis);
RW_FUNCTION(0x5EA8A0, "__func<Next_Disp()::$_5>::__clone()", Func1<VT_NEXT_DISP>::clone);
RW_FUNCTION(0x5EA8CC, "__func<Next_Disp()::$_5>::__clone(dst)", Func1<VT_NEXT_DISP>::cloneInto);
RW_FUNCTION(0x5EA8EC, "__func<Next_Disp()::$_5>::destroy", Func1<VT_NEXT_DISP>::destroy);
RW_FUNCTION(0x5EA8EE, "__func<Next_Disp()::$_5>::destroy_deallocate", Func1<VT_NEXT_DISP>::deleteThis);
RW_FUNCTION(0x5EA93C, "__func<Next_Disp()::$_5>::target", target<0xE4D800>);
RW_FUNCTION(0x5EA95C, "__func<Next_Disp()::$_5>::target_type", targetType<0xF949B8>);

// ------------------------------------------------------------------------------------------- AnimationNode
AnimationNode *AnimationNode::create() {
    uint32_t p = guest::newObject(SIZE);
    rw::call(lib::Node_ctor, {p});
    auto *self = rw::host_ptr<AnimationNode>(p);
    rw::set<uint32_t>(self, 0, VTABLE);
    self->onScaleXFn = 0;
    if (rw::call(lib::Node_init, {p}) == 1) {
        rw::call(lib::Ref_autorelease, {p});
        return self;
    }
    vcall(p, slot::deletingDtor, {p});
    return nullptr;
}
RW_FUNCTION(0x5E6294, "AnimationNode::create", AnimationNode::create);

namespace {
// ~std::function<void(float)> of onScaleX, inlined
void destroyOnScaleX(AnimationNode *self) {
    uint32_t fnBuf = rw::guest_addr(self->onScaleX);
    uint32_t f = self->onScaleXFn;
    if (f == fnBuf) vcall(f, 0x10, {f});
    else if (f) vcall(f, 0x14, {f});
}
}  // namespace

AnimationNode *AnimationNode::destroy(AnimationNode *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    destroyOnScaleX(self);
    rw::call(lib::Node_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x5EAFA0, "AnimationNode::~AnimationNode", AnimationNode::destroy);

void AnimationNode::destroyDelete(AnimationNode *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    destroyOnScaleX(self);
    rw::call(lib::Node_dtor, {rw::guest_addr(self)});
    rw::call(guest::OPERATOR_DELETE, {rw::guest_addr(self)});
}
RW_FUNCTION(0x5EAFEC, "AnimationNode::~AnimationNode (deleting)", AnimationNode::destroyDelete);

void AnimationNode::setScaleX(AnimationNode *self, float scaleX) {
    uint32_t me = rw::guest_addr(self);
    rw::call(lib::Node_setScaleX, {me, fbits(scaleX)});
    if (self->onScaleXFn) rw::call(lib::FloatFunction_call, {me + 0x270, fbits(scaleX)});
}
RW_FUNCTION(0x5EB03C, "AnimationNode::setScaleX", AnimationNode::setScaleX);

// ------------------------------------------------------------------------------------------- Arc (anonymous)
// An anonymous-namespace Node of the same file: a ring of sprites (cocos2d::Vector<Sprite*> at +0x274) of
// which one is lit, the node rotated with the rate (rate at +0x270). A twin of AnimationNode::setScaleX
// (sub_5E4BC4) belongs to another anonymous Node with a std::function<void(float)> at +0x270.
namespace {
struct RaidHpArc {
    static constexpr uint32_t VTABLE = 0xF93F54;   // _ZTVN12_GLOBAL__N_13ArcE + 8

    static void setScaleXNotify(uint32_t self, float scaleX) {
        rw::call(lib::Node_setScaleX, {self, fbits(scaleX)});
        if (rw::load<uint32_t>(self + 0x280)) rw::call(lib::FloatFunction_call, {self + 0x270, fbits(scaleX)});
    }

    // light sprite int(3 - 3 * rate) and rotate the ring to (1 - rate) * -270 degrees
    static void setRate(uint32_t self, float rate) {
        uint32_t sprites = rw::load<uint32_t>(self + 0x274);
        int32_t bytes = int32_t(rw::load<uint32_t>(self + 0x278) - sprites);
        int32_t count = bytes >> 2;
        int32_t lit = f2i(3.0f - rate * 3.0f);
        if (bytes >= 1) {
            for (int32_t k = 0; k < count; k++) {
                if (k > 0) sprites = rw::load<uint32_t>(self + 0x274);
                uint32_t sprite = rw::load<uint32_t>(sprites + 4 * uint32_t(k));
                vcall(sprite, slot::setVisible, {sprite, lit == k ? 1u : 0u});
            }
        }
        vcall(self, slot::setRotation, {self, fbits((1.0f - rate) * -270.0f)});
        rw::save<float>(self + 0x270, rate);
    }

    static uint32_t destroy(uint32_t self) {
        rw::save<uint32_t>(self, VTABLE);
        rw::call(lib::SpriteVector_dtor, {self + 0x274});
        rw::call(lib::Node_dtor, {self});
        return self;
    }
    static void destroyDelete(uint32_t self) {
        rw::save<uint32_t>(self, VTABLE);
        rw::call(lib::SpriteVector_dtor, {self + 0x274});
        rw::call(lib::Node_dtor, {self});
        rw::call(guest::OPERATOR_DELETE, {self});
    }
    // deleting destructor of a member-less Node subclass of the file
    static void nodeDestroyDelete(uint32_t self) {
        rw::call(lib::Node_dtor, {self});
        rw::call(guest::OPERATOR_DELETE, {self});
    }
};
}  // namespace
RW_FUNCTION(0x5E4BC4, "(anonymous Node)::setScaleX (sub_5E4BC4)", RaidHpArc::setScaleXNotify);
RW_FUNCTION(0x5E4BEC, "(anonymous)::Arc::setRate (sub_5E4BEC)", RaidHpArc::setRate);
RW_FUNCTION(0x5E4F08, "(anonymous)::Arc::~Arc (sub_5E4F08)", RaidHpArc::destroy);
RW_FUNCTION(0x5E4F3C, "(anonymous)::Arc::~Arc deleting (sub_5E4F3C)", RaidHpArc::destroyDelete);
RW_FUNCTION(0x5E4F74, "(anonymous Node)::~Node deleting (sub_5E4F74)", RaidHpArc::nodeDestroyDelete);
