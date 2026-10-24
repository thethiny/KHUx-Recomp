// LoadingIndicator: the touch-swallowing loading overlay.
#include "LoadingIndicator.hpp"
#include "StageUI_Thunks.hpp"

using namespace scene;

namespace {
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t LayerColor_create = rwlib::cocos2d::LayerColor::create;                 // (const Color4B&)
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t TouchFunction_swap = 0x483E4D;               // std::function<bool(Touch*,Event*)>::swap
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;                    // (float duration, uint8 opacity)
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;

constexpr uint32_t COLOR4B_BLACK = 0x1007EBC;                    // cocos2d::Color4B::BLACK
constexpr uint32_t VEC2_ANCHOR_BOTTOM_LEFT = 0x1006958;          // cocos2d::Vec2::ANCHOR_BOTTOM_LEFT
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
constexpr uint32_t LISTENER_ON_TOUCH_BEGAN = 0x50;               // EventListenerTouchOneByOne::onTouchBegan

constexpr uint32_t Armature_getAnimation = 0x298;               // virtual slots (byte offsets)
constexpr uint32_t Anim_playWithIndex = 0x70;                   // (index, durationTo, loop)

constexpr uint32_t STR_ExportJson = 0xDFD4D1;    // "cocostudio/publish/LoadingAnimation.ExportJson"
constexpr uint32_t STR_LoadingAnimation = 0xDFD4C0;

constexpr uint32_t VT_SwallowTouch = 0xF5612C;   // init()::$_0 (bool(Touch*, Event*): true)
constexpr uint32_t VT_HideDone = 0xF56100;       // hide()::$_1 (remove from parent)
}  // namespace

uint32_t LoadingIndicator::init(LoadingIndicator *self) {
    uint32_t a = rw::guest_addr(self);
    Vec2 origin = visibleOrigin();
    Size visible = visibleSize();
    uint32_t shade = call(LayerColor_create, {COLOR4B_BLACK});
    self->shade = shade;
    vcall(shade, node::setOpacity, {shade, 127});
    vcall(a, node::addChildZ, {a, self->shade, 0});
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    {
        Frame f;
        uint32_t began = newFunction(f, VT_SwallowTouch, {});
        call(TouchFunction_swap, {began, listener + LISTENER_ON_TOUCH_BEGAN});
        functionDestroy(began);        // now holds the listener's previous callback
    }
    uint32_t dispatcher = ld32(director() + DIRECTOR_EVENT_DISPATCHER);
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self->shade});
    uint32_t manager = call(ArmatureDataManager_getInstance, {});
    {
        Frame f;
        uint32_t s = newStr(f, STR_ExportJson, 0x2E);
        call(ArmatureDataManager_addArmatureFileInfo, {manager, s});
        strFree(s);
    }
    uint32_t armature;
    {
        Frame f;
        uint32_t s = newStr(f, STR_LoadingAnimation, 0x10);
        armature = call(Armature_create, {s});
        strFree(s);
    }
    vcall(armature, node::setAnchorPoint, {armature, VEC2_ANCHOR_BOTTOM_LEFT});
    {
        Frame f;
        uint32_t pos = newVec2(f, (origin.x + visible.width) + -84.0f, origin.y + 10.0f);
        vcall(armature, node::setPosition, {armature, pos});
    }
    uint32_t anim = vcall(armature, Armature_getAnimation, {armature});
    vcall(anim, Anim_playWithIndex, {anim, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
    vcall(a, node::addChildZ, {a, armature, 100});
    self->animation = armature;
    vcall(a, node::setOpacity, {a, 0});
    vcall(a, node::setCascadeOpacityEnabled, {a, 1});
    return 1;
}
RW_FUNCTION(0x509A3C, "LoadingIndicator::init", LoadingIndicator::init);

LoadingIndicator *LoadingIndicator::destroy(LoadingIndicator *self) {
    return rw::host_ptr<LoadingIndicator>(call(Node_dtor, {rw::guest_addr(self)}));
}
RW_FUNCTION(0x509CCC, "LoadingIndicator::~LoadingIndicator", LoadingIndicator::destroy);

void LoadingIndicator::destroyDelete(LoadingIndicator *self) {
    uint32_t r0 = call(Node_dtor, {rw::guest_addr(self)});
    call(fn::operator_delete, {r0});   // Node::~Node returns this
}
RW_FUNCTION(0x509CD4, "LoadingIndicator::~LoadingIndicator (deleting)", LoadingIndicator::destroyDelete);

// Fade in (from the current opacity; from 0 after a half second delay). Returns the runAction result, or the
// address of `showing` when already shown.
uint32_t LoadingIndicator::show(LoadingIndicator *self) {
    if (self->showing) return rw::guest_addr(&self->showing);
    self->showing = 1;
    uint32_t a = rw::guest_addr(self);
    call(Node_stopAllActions, {a});
    int32_t opacity = int32_t(vcall(a, node::getOpacity, {a}));
    uint32_t action;
    if (opacity) {
        float t = (float(opacity ^ 0xFF) / 255.0f) * 0.1f;
        action = call(FadeTo_create, {scene::fbits(t), 0xFF});
    } else {
        uint32_t delay = call(DelayTime_create, {scene::fbits(0.5f)});
        uint32_t fade = call(FadeTo_create, {scene::fbits(0.1f), 0xFF});
        action = call(Sequence_create, {delay, fade, 0});
    }
    return call(Node_runAction, {a, action});
}
RW_FUNCTION(0x509CE0, "LoadingIndicator::show", LoadingIndicator::show);

// Fade out, then remove the node ($_1).
uint32_t LoadingIndicator::hide(LoadingIndicator *self) {
    uint32_t a = rw::guest_addr(self);
    self->showing = 0;
    uint32_t opacity = vcall(a, node::getOpacity, {a});
    call(Node_stopAllActions, {a});
    float t = (float(opacity) / 255.0f) * 0.1f;
    uint32_t fade = call(FadeTo_create, {scene::fbits(t), 0});
    Frame f;
    uint32_t done = newFunction(f, VT_HideDone, {a});
    uint32_t callback = call(CallFunc_create, {done});
    call(Node_runAction, {a, call(Sequence_create, {fade, callback, 0})});
    functionDestroy(done);
    return 0;
}
RW_FUNCTION(0x509D68, "LoadingIndicator::hide", LoadingIndicator::hide);

LoadingIndicator *LoadingIndicator::create() {
    uint32_t a = call(fn::operator_new, {SIZE});
    call(Node_ctor, {a});
    LoadingIndicator *l = rw::host_ptr<LoadingIndicator>(a);
    l->vtable = VTABLE;
    l->shade = 0;
    l->animation = 0;
    l->showing = 0;
    if (init(l) == 1) {                    // a direct (non-virtual) call in the original
        call(Ref_autorelease, {a});
        return l;
    }
    vcall(a, 0x04, {a});                  // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x60CEE0, "LoadingIndicator::create", LoadingIndicator::create);

// ------------------------------------------------------------------------------------------- lambdas
namespace loading {
// hide()::$_1: the fade-out finished: remove the indicator
void hideDone(uint32_t self) {
    uint32_t node = ld32(self + 4);
    vcall(node, node::removeFromParent, {node});
}
// init()::$_0: swallow every touch
uint32_t swallowTouch(uint32_t, uint32_t, uint32_t) { return 1; }

constexpr uint32_t NAME_HideDone = 0xE21DE0, TI_HideDone = 0xF83290;
constexpr uint32_t NAME_SwallowTouch = 0xE21E90, TI_SwallowTouch = 0xF832A4;
}  // namespace loading

RW_FUNCTION(0x509E70, "LoadingIndicator::hide::$_1::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x509E78, "LoadingIndicator::hide::$_1::__clone", thunk::clone1<VT_HideDone>);
RW_FUNCTION(0x509EA4, "LoadingIndicator::hide::$_1::__clone(dst)", thunk::cloneTo1<VT_HideDone>);
RW_FUNCTION(0x509EC4, "LoadingIndicator::hide::$_1::destroy", thunk::nop);
RW_FUNCTION(0x509EC6, "LoadingIndicator::hide::$_1::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x509ECE, "LoadingIndicator::hide::$_1::operator()", loading::hideDone);
RW_FUNCTION(0x509EE0, "LoadingIndicator::hide::$_1::target", thunk::target<loading::NAME_HideDone>);
RW_FUNCTION(0x509F00, "LoadingIndicator::hide::$_1::target_type", thunk::targetType<loading::TI_HideDone>);

RW_FUNCTION(0x509F14, "LoadingIndicator::init::$_0::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x509F1C, "LoadingIndicator::init::$_0::__clone", thunk::cloneEmpty<VT_SwallowTouch>);
RW_FUNCTION(0x509F40, "LoadingIndicator::init::$_0::__clone(dst)", thunk::cloneToEmpty<VT_SwallowTouch>);
RW_FUNCTION(0x509F5C, "LoadingIndicator::init::$_0::destroy", thunk::nop);
RW_FUNCTION(0x509F5E, "LoadingIndicator::init::$_0::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x509F66, "LoadingIndicator::init::$_0::operator()", loading::swallowTouch);
RW_FUNCTION(0x509F6C, "LoadingIndicator::init::$_0::target", thunk::target<loading::NAME_SwallowTouch>);
RW_FUNCTION(0x509F8C, "LoadingIndicator::init::$_0::target_type", thunk::targetType<loading::TI_SwallowTouch>);
