// DamageNumber: the popping damage digits.
#include "DamageNumber.hpp"
#include "StageUI_Thunks.hpp"

using namespace scene;

namespace {
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Node_create = rwlib::cocos2d::Node::create;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__RKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE;           // (const std::string& file)
constexpr uint32_t MoveBy_create = rwlib::cocos2d::MoveBy::create;           // (float duration, const Vec2&)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t EaseBounceOut_create = rwlib::cocos2d::EaseBounceOut::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Spawn_create = rwlib::cocos2d::Spawn::create;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t FadeOut_create = rwlib::cocos2d::FadeOut::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformInt = 0x492225;   // (random, min, max, uint* state)
constexpr uint32_t libm_log10f = 0x41ABF0;             // (imports)
constexpr uint32_t libm_powf = 0x41ABFC;

constexpr uint32_t DIGIT_FORMATS = 0xF5DCC0;           // const char*[4]: "img/damage/dmg_num..._%d.png" per type
constexpr uint32_t DIGIT_SPACING = 0xE4DB90;           // float[4]: digit advance per type

constexpr uint32_t VT_Remove = 0xF5DCD8;               // init(...)::$_0 (remove from parent)

constexpr float RISE_TIME = 0.183333337f;
constexpr float BOUNCE_TIME = 0.75f;
constexpr float FADE_DELAY = 0.466666669f;
}  // namespace

DamageNumber *DamageNumber::create(int32_t value, uint32_t type, uint32_t scale) {
    uint32_t a = call(fn::operator_new, {SIZE});
    call(Node_ctor, {a});
    DamageNumber *d = rw::host_ptr<DamageNumber>(a);
    d->vtable = VTABLE;
    if (init(d, value, type, scale) == 1) {
        call(Ref_autorelease, {a});
        return d;
    }
    call(Ref_release, {a});
    return nullptr;
}
RW_FUNCTION(0x5EB064, "DamageNumber::create", DamageNumber::create);

// Lay the digits out centred in a holder node (scaled 0.8 / 1.0 over 1.2), then: a random jump (x -40..40,
// y 110..128) easing out, a small bounce, and after 0.47 s a fade-out ending in removal ($_0).
uint32_t DamageNumber::init(DamageNumber *self, int32_t value, uint32_t type, uint32_t scale) {
    if (type > 3) return 0;
    float size;
    if (scale == 0) size = 0.8f;
    else if (scale == 1) size = 1.0f;
    else return 0;
    uint32_t a = rw::guest_addr(self);
    uint32_t holder = call(Node_create, {});
    vcall(holder, node::setScale, {holder, scene::fbits(size / 1.2f)});
    vcall(a, node::addChild, {a, holder});
    float logv = bitsf(call(libm_log10f, {scene::fbits(float(value))}));
    int32_t digits = f2i(logv + 1.0f);
    float power = bitsf(call(libm_powf, {scene::fbits(10.0f), scene::fbits(float(digits - 1))}));
    if (digits >= 1) {
        float spacing = ldf(DIGIT_SPACING + 4 * type);
        float x = (spacing * float(1 - digits)) * 0.5f;
        uint32_t format = ld32(DIGIT_FORMATS + 4 * type);
        int32_t divisor = f2i(power);
        for (int32_t k = 0; k < digits; k++) {
            uint32_t sprite;
            {
                Frame f;
                uint32_t file = stringBuffer(f, {format, uint32_t(value / divisor % 10)});
                uint32_t s = newStr(f, file);
                sprite = call(Sprite_create, {s});
                strFree(s);
            }
            {
                Frame f;
                vcall(sprite, node::setPosition, {sprite, newVec2(f, x, 0.0f)});
            }
            vcall(holder, node::addChild, {holder, sprite});
            x = spacing + x;
            divisor /= 10;
        }
    }
    vcall(a, node::setCascadeOpacityEnabled, {a, 1});
    float rx = float(int32_t(call(Random_generateUniformInt, {call(Random_getInstance, {}), uint32_t(-40), 40, 0})));
    float ry = float(int32_t(call(Random_generateUniformInt, {call(Random_getInstance, {}), 110, 128, 0})));
    Frame f;
    uint32_t jump = newVec2(f, rx, ry);
    uint32_t rise = call(EaseSineOut_create, {call(MoveBy_create, {scene::fbits(RISE_TIME), jump})});
    uint32_t drop = newVec2(f, 0.0f, -6.0f);
    uint32_t bounce = call(EaseBounceOut_create, {call(MoveBy_create, {scene::fbits(BOUNCE_TIME), drop})});
    uint32_t move = call(Sequence_create, {rise, bounce, 0});
    uint32_t delay = call(DelayTime_create, {scene::fbits(FADE_DELAY)});
    uint32_t fade = call(FadeOut_create, {scene::fbits(FADE_DELAY)});
    uint32_t done = newFunction(f, VT_Remove, {a});
    uint32_t remove = call(CallFunc_create, {done});
    uint32_t vanish = call(Sequence_create, {delay, fade, remove, 0});
    call(Node_runAction, {a, call(Spawn_create, {move, vanish, 0})});
    functionDestroy(done);
    return 1;
}
RW_FUNCTION(0x5EB0E4, "DamageNumber::init", DamageNumber::init);

void DamageNumber::destroyDelete(DamageNumber *self) {
    uint32_t r0 = call(Node_dtor, {rw::guest_addr(self)});
    call(fn::operator_delete, {r0});
}
RW_FUNCTION(0x5EB6A8, "DamageNumber::~DamageNumber (deleting)", DamageNumber::destroyDelete);

// ------------------------------------------------------------------------------------------- lambdas
namespace damagenumber {
// init(...)::$_0: the number faded out: remove it
void remove(uint32_t self) {
    uint32_t node = ld32(self + 4);
    vcall(node, node::removeFromParent, {node});
}
constexpr uint32_t NAME_Remove = 0xE4DC20, TI_Remove = 0xF94CB4;
}  // namespace damagenumber

RW_FUNCTION(0x5EB604, "DamageNumber::init::$_0::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5EB60C, "DamageNumber::init::$_0::__clone", thunk::clone1<VT_Remove>);
RW_FUNCTION(0x5EB638, "DamageNumber::init::$_0::__clone(dst)", thunk::cloneTo1<VT_Remove>);
RW_FUNCTION(0x5EB658, "DamageNumber::init::$_0::destroy", thunk::nop);
RW_FUNCTION(0x5EB65A, "DamageNumber::init::$_0::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5EB662, "DamageNumber::init::$_0::operator()", damagenumber::remove);
RW_FUNCTION(0x5EB674, "DamageNumber::init::$_0::target", thunk::target<damagenumber::NAME_Remove>);
RW_FUNCTION(0x5EB694, "DamageNumber::init::$_0::target_type", thunk::targetType<damagenumber::TI_Remove>);
