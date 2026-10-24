// MissBoard: the "MISS" popup of the battle screen (see MissBoard.hpp).
#include "MissBoard.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base, Node_dtor = rwlib::cocos2d::Node::dtor_base, Node_init = rwlib::cocos2d::Node::init;
constexpr uint32_t Node_removeAllChildrenWithCleanup = rwlib::cocos2d::Node::removeAllChildrenWithCleanup;
constexpr uint32_t Node_convertToWorldSpace = rwlib::cocos2d::Node::convertToWorldSpace;   // (Vec2* out, node, const Vec2&)
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v, Vec2_ctor_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t VEC2_ZERO = 0x1006950;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;            // (const std::string&)
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t ArmatureAnimation_setMovementEventCallFunc = rwlib::cocostudio::ArmatureAnimation::setMovementEventCallFunc;
constexpr uint32_t ArmatureAnimation_setFrameEventCallFunc = rwlib::cocostudio::ArmatureAnimation::setFrameEventCallFunc;

constexpr uint32_t STR_ActionMapLuxNumAnim = 0xE01F45;
constexpr uint32_t STR_SkillMiss = 0xE01EE3;
constexpr uint32_t STR_FMT_EXPORT_JSON = 0xE01EED;        // "cocostudio/publish/%s.ExportJson"

// lambdas of init (std::function __func vtables + 8)
constexpr uint32_t VT_LAMBDA_MOVEMENT = 0xF5D3E0;         // $_4: hide the board when the movement completes
constexpr uint32_t VT_LAMBDA_FRAME = 0xF5D3B4;            // $_5: frame events (nothing)

// virtual slots (byte offsets)
constexpr uint32_t VT_INIT = 0x248;                       // Node::init
constexpr uint32_t VT_GET_ANIMATION = 0x298;              // Armature::getAnimation
constexpr uint32_t VT_ANIM_RESUME = 0x10;                 // ArmatureAnimation::resume
constexpr uint32_t VT_ANIM_GET_CURRENT_FRAME = 0x1C;      // ProcessBase::getCurrentFrameIndex
constexpr uint32_t VT_ANIM_IS_PAUSE = 0x2C;               // ProcessBase::isPause
constexpr uint32_t VT_ANIM_IS_COMPLETE = 0x34;            // ProcessBase::isComplete
constexpr uint32_t VT_ANIM_PLAY = 0x68;                   // play(name, durationTo, loop)
constexpr uint32_t VT_ANIM_GOTO_AND_PAUSE = 0x80;         // gotoAndPause(frame)

constexpr uint32_t MOVEMENT_COMPLETE = 1;                 // cocostudio::MovementEventType::COMPLETE

uint32_t animationOf(uint32_t armature) { return vcall(armature, VT_GET_ANIMATION, {armature}); }
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
uint32_t MissBoard::create() {
    uint32_t obj = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<MissBoard>(obj));
    if (vcall(obj, VT_INIT, {obj}) == 1) {
        call(Ref_autorelease, {obj});
        return obj;
    }
    vcall(obj, 4, {obj});    // deleting destructor
    return 0;
}
RW_FUNCTION(0x5D4224, "MissBoard::create", MissBoard::create);

MissBoard *MissBoard::construct(MissBoard *self) {
    const uint32_t me = rw::guest_addr(self);
    call(Node_ctor, {me});
    self->vtable = VTABLE;
    self->armature = 0;
    call(Vec2_ctor, {rw::guest_addr(&self->pos)});
    call(Vec2_ctor, {rw::guest_addr(&self->uiPos)});
    return self;
}
RW_FUNCTION(0x5D46A4, "MissBoard::MissBoard", MissBoard::construct);

MissBoard *MissBoard::destruct(MissBoard *self) {
    const uint32_t me = rw::guest_addr(self);
    self->vtable = VTABLE;
    call(Node_removeAllChildrenWithCleanup, {me, 1});
    call(Vec2_dtor, {rw::guest_addr(&self->uiPos)});
    call(Vec2_dtor, {rw::guest_addr(&self->pos)});
    call(Node_dtor, {me});
    return self;
}
RW_FUNCTION(0x5D3D04, "MissBoard::~MissBoard", MissBoard::destruct);

void MissBoard::destructDelete(MissBoard *self) {
    call(fn::operator_delete, {rw::guest_addr(destruct(self))});
}
RW_FUNCTION(0x5D3D74, "MissBoard::~MissBoard (deleting)", MissBoard::destructDelete);

// The armature (stopped on frame 0 of "SkillMiss", hidden by its movement lambda when done), the board hidden.
bool MissBoard::init(MissBoard *self) {
    const uint32_t me = rw::guest_addr(self);
    if (call(Node_init, {me}) != 1) return false;
    {
        Frame f;
        uint32_t name = newStr(f, STR_ActionMapLuxNumAnim, 19);
        self->armature = call(Armature_create, {name});
        strFree(name);
    }
    uint32_t anim = animationOf(self->armature);
    {
        Frame f;
        uint32_t movement = newStr(f, STR_SkillMiss, 9);
        vcall(anim, VT_ANIM_PLAY, {anim, movement, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(movement);
    }
    vcall(anim, VT_ANIM_GOTO_AND_PAUSE, {anim, 0});
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_LAMBDA_MOVEMENT, {me});
        call(ArmatureAnimation_setMovementEventCallFunc, {anim, cb});
        functionDestroy(cb);
    }
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_LAMBDA_FRAME, {me});
        call(ArmatureAnimation_setFrameEventCallFunc, {anim, cb});
        functionDestroy(cb);
    }
    vcall(me, node::addChild, {me, self->armature});
    vcall(me, node::setVisible, {me, 0});
    return true;
}
RW_FUNCTION(0x5D3D80, "MissBoard::init", MissBoard::init);

// Loads cocostudio/publish/ActionMapLuxNumAnim.ExportJson.
void MissBoard::firstCall() {
    uint32_t mgr = call(ArmatureDataManager_getInstance, {});
    Frame f;
    uint32_t path = stringBuffer(f, {STR_FMT_EXPORT_JSON, STR_ActionMapLuxNumAnim});
    uint32_t s = newStr(f, path);
    call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
    strFree(s);
}
RW_FUNCTION(0x5D406C, "MissBoard::firstCall", MissBoard::firstCall);

void MissBoard::finalCall() {}
RW_FUNCTION(0x5D4138, "MissBoard::finalCall", MissBoard::finalCall);

// ------------------------------------------------------------------------------------------- display
// Show the board; an animation paused on frame 0 is resumed and the armature shown again.
void MissBoard::play(MissBoard *self, uint32_t) {
    const uint32_t me = rw::guest_addr(self);
    uint32_t anim = animationOf(self->armature);
    if (vcall(anim, VT_ANIM_GET_CURRENT_FRAME, {anim}) == 0 && vcall(anim, VT_ANIM_IS_PAUSE, {anim}) == 1) {
        vcall(anim, VT_ANIM_RESUME, {anim});
        vcall(self->armature, node::setVisible, {self->armature, 1});
    }
    vcall(me, node::setVisible, {me, 1});
}
RW_FUNCTION(0x5D3F68, "MissBoard::play", MissBoard::play);

// Always false (asks isComplete past frame 0 and drops the answer).
bool MissBoard::isFinish(MissBoard *self) {
    uint32_t anim = animationOf(self->armature);
    if (vcall(anim, VT_ANIM_GET_CURRENT_FRAME, {anim})) vcall(anim, VT_ANIM_IS_COMPLETE, {anim});
    return false;
}
RW_FUNCTION(0x5D3FC6, "MissBoard::isFinish", MissBoard::isFinish);

// The board's world position (stored in uiPos; returns its address).
uint32_t MissBoard::getUIPos(MissBoard *self) {
    Frame f;
    uint32_t out = f.alloc(8);
    call(Node_convertToWorldSpace, {out, rw::guest_addr(self), VEC2_ZERO});
    float x = ldf(out), y = ldf(out + 4);
    self->uiPos[1] = y;
    self->uiPos[0] = x;
    call(Vec2_dtor, {out});
    return rw::guest_addr(&self->uiPos);
}
RW_FUNCTION(0x5D3FF8, "MissBoard::getUIPos", MissBoard::getUIPos);

void MissBoard::setCompleteVisible(MissBoard *, uint32_t armature) {
    vcall(armature, node::setVisible, {armature, 0});
    uint32_t anim = animationOf(armature);
    vcall(anim, VT_ANIM_GOTO_AND_PAUSE, {anim, 0});
}
RW_FUNCTION(0x5D413A, "MissBoard::setCompleteVisible", MissBoard::setCompleteVisible);

// A new board on `parent` (z 2) at pos + (0, yOffset), scaled, playing.
void MissBoard::disp_miss(uint32_t parent, uint32_t pos, float yOffset, float scale) {
    uint32_t board = create();
    Frame f;
    uint32_t p = f.alloc(8);
    call(Vec2_ctor_copy, {p, pos});
    stf(p + 4, ldf(p + 4) + yOffset);
    vcall(board, node::setPosition, {board, p});
    vcall(board, node::setScale, {board, fbits(scale)});
    vcall(parent, node::addChildZ, {parent, board, 2});
    play(rw::host_ptr<MissBoard>(board), 0);
    call(Vec2_dtor, {p});
}
RW_FUNCTION(0x5D4170, "MissBoard::disp_miss", MissBoard::disp_miss);

// ------------------------------------------------------------------------------------------- lambdas of init
namespace {
// init $_4 (movement event): when the movement completes, hide the armature and stop it on frame 0.
void initMovementLambda(uint32_t func, uint32_t armature, uint32_t type, uint32_t) {
    (void)armature;
    if (ld32(type) != MOVEMENT_COMPLETE) return;
    uint32_t arm = rw::host_ptr<MissBoard>(ld32(func + 4))->armature;
    vcall(arm, node::setVisible, {arm, 0});
    uint32_t anim = animationOf(arm);
    vcall(anim, VT_ANIM_GOTO_AND_PAUSE, {anim, 0});
}
RW_FUNCTION(0x5D436A, "MissBoard::init $_4 __func::operator()", initMovementLambda);

// init $_5 (frame event): nothing.
void initFrameLambda() {}
RW_FUNCTION(0x5D42D6, "MissBoard::init $_5 __func::operator()", initFrameLambda);
}  // namespace
