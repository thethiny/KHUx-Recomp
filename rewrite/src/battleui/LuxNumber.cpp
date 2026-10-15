// LuxNumber: the Lux pop-up number (cocostudio armature with one bone per digit).
#include "LuxNumber.hpp"
#include "StageUI_Thunks.hpp"

using namespace scene;

namespace {
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Node_dtor = 0x8927ED;
constexpr uint32_t Node_init = 0x892B69;
constexpr uint32_t Node_removeAllChildrenWithCleanup = 0x894369;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Armature_create = 0x79F3C7;                 // (const std::string& name)
constexpr uint32_t ArmatureAnimation_setMovementEventCallFunc = 0x7A3C69;
constexpr uint32_t ArmatureAnimation_setFrameEventCallFunc = 0x7A3CE5;
constexpr uint32_t Bone_changeDisplayWithIndex = 0x7A2681;      // (bone, index, force)
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t libm_log10 = 0x419E7C;                       // double log10(double) (import)

// virtual slots (byte offsets)
constexpr uint32_t Armature_getAnimation = 0x298;
constexpr uint32_t Armature_getBone = 0x27C;                    // (const std::string&)
constexpr uint32_t Anim_play = 0x68;                            // (name, durationTo, loop)
constexpr uint32_t Anim_pause = 0x80;
constexpr uint32_t Anim_resume = 0x10;
constexpr uint32_t Anim_isPause = 0x1C;
constexpr uint32_t Anim_isComplete = 0x2C;
constexpr uint32_t Anim_isPlaying = 0x34;

constexpr uint32_t STR_ArmatureName = 0xE01F45;     // "ActionMapLuxNumAnim"
constexpr uint32_t STR_BoneFmt = 0xE01F59;          // "LuxNum%d"
constexpr uint32_t STR_ExportJsonFmt = 0xE01EED;    // "cocostudio/publish/%s.ExportJson"
constexpr uint32_t STYLE_MOVEMENTS = 0xF5D4B4;      // const char*[2]: the armature movement per Style
constexpr uint32_t STYLE_OFFSETS_X = 0xE4B8B0;      // float[2][5]: x offset per Style and digit count

constexpr uint32_t VT_MovementEvent = 0xF5D4F0;     // init(...)::$_0 (hide when the movement completes)
constexpr uint32_t VT_FrameEvent = 0xF5D4C4;        // init(...)::$_1 (no-op)

constexpr uint32_t MAX_VALUE = 99999;
constexpr uint32_t DIGITS = 5;

uint32_t animationOf(const LuxNumber *self) {
    uint32_t armature = self->armature;
    return vcall(armature, Armature_getAnimation, {armature});
}
}  // namespace

LuxNumber *LuxNumber::destroy(LuxNumber *self) {
    uint32_t a = rw::guest_addr(self);
    self->vtable = VTABLE;
    call(Node_removeAllChildrenWithCleanup, {a, 1});
    call(Node_dtor, {a});
    return self;
}
RW_FUNCTION(0x5D470C, "LuxNumber::~LuxNumber", LuxNumber::destroy);

void LuxNumber::destroyDelete(LuxNumber *self) {
    destroy(self);
    call(fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x5D4754, "LuxNumber::~LuxNumber (deleting)", LuxNumber::destroyDelete);

// Build the armature for `style` (0 / 1), show `value` (capped at 99999) on the digit bones (ones digit on
// LuxNum1; unused digits hidden), offset the armature by the digit count and start hidden and paused.
uint32_t LuxNumber::init(LuxNumber *self, uint32_t value, uint32_t style) {
    uint32_t a = rw::guest_addr(self);
    uint32_t failed = call(Node_init, {a}) ^ 1;
    if (style > 1 || failed) return 0;
    {
        Frame f;
        uint32_t name = newStr(f, STR_ArmatureName, 19);
        self->armature = call(Armature_create, {name});
        strFree(name);
    }
    uint32_t anim = animationOf(self);
    {
        Frame f;
        uint32_t movement = newStr(f, ld32(STYLE_MOVEMENTS + 4 * style));
        vcall(anim, Anim_play, {anim, movement, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(movement);
    }
    vcall(anim, Anim_pause, {anim, 0});
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_MovementEvent, {a});
        call(ArmatureAnimation_setMovementEventCallFunc, {anim, cb});
        functionDestroy(cb);
    }
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_FrameEvent, {});
        call(ArmatureAnimation_setFrameEventCallFunc, {anim, cb});
        functionDestroy(cb);
    }
    uint32_t shown = value > MAX_VALUE ? MAX_VALUE : value;
    int32_t digits = 1;
    if (shown) {
        uint64_t l = rw::call64(libm_log10, {uint32_t(bt::bits(double(shown))), uint32_t(bt::bits(double(shown)) >> 32)});
        digits = bt::d2iz(bt::dfromBits(l)) + 1;
    }
    int32_t rest = int32_t(shown);
    uint32_t result = 0;
    for (int32_t k = 0; k < int32_t(DIGITS);) {
        uint32_t armature = self->armature;
        uint32_t getBone = ld32(ld32(armature) + Armature_getBone);
        uint32_t bone;
        {
            Frame f;
            uint32_t buf = stringBuffer(f, {STR_BoneFmt, uint32_t(k + 1)});
            uint32_t s = newStr(f, buf);
            bone = call(getBone, {armature, s});
            strFree(s);
        }
        result = 1;
        if (bone) {
            if (k < digits) {
                call(Bone_changeDisplayWithIndex, {bone, uint32_t(rest % 10), 1});
                rest /= 10;
            } else {
                call(Bone_changeDisplayWithIndex, {bone, 0xFFFFFFFFu, 1});
            }
        }
        k++;
    }
    vcall(a, node::addChild, {a, self->armature});
    uint32_t offset = ld32(STYLE_OFFSETS_X + 20 * style + 4 * uint32_t(digits) - 4);
    uint32_t armature = self->armature;
    vcall(armature, node::setPositionX, {armature, offset});
    vcall(a, node::setVisible, {a, 0});
    return result;
}
RW_FUNCTION(0x5D4760, "LuxNumber::init", LuxNumber::init);

LuxNumber *LuxNumber::create(uint32_t value, uint32_t style) {
    uint32_t a = call(fn::operator_new, {SIZE});
    call(Node_ctor, {a});
    LuxNumber *n = rw::host_ptr<LuxNumber>(a);
    n->vtable = VTABLE;
    n->armature = 0;
    if (init(n, value, style) == 1) {
        call(Ref_autorelease, {a});
        return n;
    }
    vcall(a, 0x04, {a});      // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5D4AC4, "LuxNumber::create", LuxNumber::create);

// Start the pop-up: resume the paused animation and show the node (only when it was paused and not done).
uint32_t LuxNumber::play(LuxNumber *self) {
    uint32_t anim = animationOf(self);
    uint32_t r = vcall(anim, Anim_isPause, {anim});
    if (r != 0) return r;
    r = vcall(anim, Anim_isComplete, {anim});
    if (r != 1) return r;
    vcall(anim, Anim_resume, {anim});
    uint32_t a = rw::guest_addr(self);
    return vcall(a, node::setVisible, {a, 1});
}
RW_FUNCTION(0x5D4B48, "LuxNumber::play", LuxNumber::play);

uint32_t LuxNumber::isFinish(LuxNumber *self) {
    uint32_t anim = animationOf(self);
    if (vcall(anim, Anim_isPause, {anim}) == 0) return 0;
    return vcall(anim, Anim_isPlaying, {anim});
}
RW_FUNCTION(0x5D4B98, "LuxNumber::isFinish", LuxNumber::isFinish);

// Register the armature's export file (static, once per battle).
uint32_t LuxNumber::firstCall() {
    uint32_t manager = call(ArmatureDataManager_getInstance, {});
    Frame f;
    uint32_t buf = stringBuffer(f, {STR_ExportJsonFmt, STR_ArmatureName});
    uint32_t s = newStr(f, buf);
    call(ArmatureDataManager_addArmatureFileInfo, {manager, s});
    strFree(s);
    return 0;
}
RW_FUNCTION(0x5D4BD0, "LuxNumber::firstCall", LuxNumber::firstCall);

void LuxNumber::finalCall() {}
RW_FUNCTION(0x5D4C9C, "LuxNumber::finalCall", LuxNumber::finalCall);

// ------------------------------------------------------------------------------------------- lambdas
namespace luxnumber {
// init(...)::$_0 (void(Armature*, MovementEventType, const std::string&)): hide the number once the movement
// completes (type 1).
void onMovementEvent(uint32_t self, uint32_t, const int32_t *type, uint32_t) {
    if (*type != 1) return;
    uint32_t node = ld32(self + 4);
    vcall(node, node::setVisible, {node, 0});
}
// init(...)::$_1 (void(Bone*, const std::string&, int, int)): nothing
void onFrameEvent(uint32_t, uint32_t, uint32_t, uint32_t) {}

constexpr uint32_t NAME_FrameEvent = 0xE4B990, TI_FrameEvent = 0xF90E74;
constexpr uint32_t NAME_MovementEvent = 0xE4BA80, TI_MovementEvent = 0xF90E88;
}  // namespace luxnumber

// init(...)::$_1
RW_FUNCTION(0x5D4C9E, "LuxNumber::init::$_1::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D4CA8, "LuxNumber::init::$_1::__clone", thunk::cloneEmpty<VT_FrameEvent>);
RW_FUNCTION(0x5D4CCC, "LuxNumber::init::$_1::__clone(dst)", thunk::cloneToEmpty<VT_FrameEvent>);
RW_FUNCTION(0x5D4CE8, "LuxNumber::init::$_1::destroy", thunk::nop);
RW_FUNCTION(0x5D4CEA, "LuxNumber::init::$_1::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D4CF2, "LuxNumber::init::$_1::operator()", luxnumber::onFrameEvent);
RW_FUNCTION(0x5D4CF4, "LuxNumber::init::$_1::target", thunk::target<luxnumber::NAME_FrameEvent>);
RW_FUNCTION(0x5D4D14, "LuxNumber::init::$_1::target_type", thunk::targetType<luxnumber::TI_FrameEvent>);

// init(...)::$_0
RW_FUNCTION(0x5D4D28, "LuxNumber::init::$_0::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D4D30, "LuxNumber::init::$_0::__clone", thunk::clone1<VT_MovementEvent>);
RW_FUNCTION(0x5D4D5C, "LuxNumber::init::$_0::__clone(dst)", thunk::cloneTo1<VT_MovementEvent>);
RW_FUNCTION(0x5D4D7C, "LuxNumber::init::$_0::destroy", thunk::nop);
RW_FUNCTION(0x5D4D7E, "LuxNumber::init::$_0::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D4D86, "LuxNumber::init::$_0::operator()", luxnumber::onMovementEvent);
RW_FUNCTION(0x5D4D9C, "LuxNumber::init::$_0::target", thunk::target<luxnumber::NAME_MovementEvent>);
RW_FUNCTION(0x5D4DBC, "LuxNumber::init::$_0::target_type", thunk::targetType<luxnumber::TI_MovementEvent>);
