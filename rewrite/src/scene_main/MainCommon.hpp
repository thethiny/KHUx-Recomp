// Shared by the scene_main rewrites (the main scenes - My Page, adventure select, action map, boot / register
// scenes - and their scroll views): library entry points and virtual slots on top of scene/SceneCommon.hpp and
// scene/SceneUi.hpp.
#pragma once

#include "../scene/SceneUi.hpp"

namespace smain {

using namespace scene;

namespace lib {
// cocos2d
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;                  // cocos2d::Layer::Layer()
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;                  // cocos2d::Layer::~Layer()
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;                  // cocos2d::Layer::init()
constexpr uint32_t Layer_create = rwlib::cocos2d::Layer::create;                // cocos2d::Layer::create()
constexpr uint32_t LayerColor_create = rwlib::cocos2d::LayerColor::create;           // cocos2d::LayerColor::create(const Color4B&)
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;                // cocos2d::Scene::create()
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;             // cocos2d::Ref::autorelease()
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;                  // cocos2d::Ref::retain()
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;                 // cocos2d::Ref::release()
constexpr uint32_t Node_getNumberOfRunningActions = rwlib::cocos2d::Node::getNumberOfRunningActions;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t Node_unscheduleUpdate = rwlib::cocos2d::Node::unscheduleUpdate;
constexpr uint32_t FadeIn_create = rwlib::cocos2d::FadeIn::create;               // (float duration)
constexpr uint32_t FadeOut_create = rwlib::cocos2d::FadeOut::create;              // (float duration)
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t Director_eventDispatcher = 0x84;        // Director field: EventDispatcher*
constexpr uint32_t Color4B_BLACK = 0x1007EBC;
// libc++ std::function
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionTouch_swap = 0x483E4D;          // std::function<bool(Touch*, Event*)>::swap
// scene manager
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;   // (mgr, function<Layer*()>*, bool, bool, FadeType)
constexpr uint32_t VT_LayerFactory = 0xF7E130;             // __func<Layer*(*)(), ...> + 8 (a plain function pointer)
}  // namespace lib

namespace lib {
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;           // (Vec2* out, Touch*)
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;     // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;                   // (Rect*, x, y, w, h)
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;          // (const Rect*, const Vec2&)
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create;               // cocos2d::ui::Widget::create()
constexpr uint32_t Widget_setContentSize = rwlib::cocos2d::ui::Widget::setContentSize;       // ui::Widget::setContentSize(const Size&)
constexpr uint32_t Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t WidgetVector_pushBack = 0x4F6B5D;       // cocos2d::Vector<ui::Widget*>::pushBack
constexpr uint32_t WidgetVector_clear = 0x485B45;
constexpr uint32_t WidgetVector_erase = 0x4F64B1;          // erase(iterator)
constexpr uint32_t WidgetVector_dtor = 0x485A8F;
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t FunctionVF_call = 0x4F8E89;             // std::function<void(float)>::operator()(float)
constexpr uint32_t AEABI_MEMMOVE = 0x40EF50;               // __aeabi_memmove(dst, src, n) (import)
constexpr uint32_t DIVSI3 = 0xCD5E8D;                      // __divsi3
constexpr uint32_t MODSI3 = 0xCD6009;                      // __modsi3
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;          // std::vector .at() failure (never returns)
}  // namespace lib

// Integer division / remainder like libgcc's __divsi3 / __modsi3 (division by zero goes to the guest helper)
inline int32_t idiv(int32_t a, int32_t b) {
    if (b == 0) return int32_t(call(lib::DIVSI3, {uint32_t(a), 0}));
    if (b == -1) return int32_t(0u - uint32_t(a));
    return a / b;
}
inline int32_t imod(int32_t a, int32_t b) {
    if (b == 0) return int32_t(call(lib::MODSI3, {uint32_t(a), 0}));
    if (b == -1) return 0;
    return a % b;
}

// Touch::getLocation() as a host value
inline Vec2 touchLocation(uint32_t touch) {
    Frame f;
    uint32_t v = f.alloc(8);
    call(lib::Touch_getLocation, {v, touch});
    return readVec2(v);
}

// cocos2d::Node virtual slots (byte offsets) missing from scene::node
namespace vslot {
constexpr uint32_t release = 0x04;                         // deleting destructor
constexpr uint32_t init = 0x248;                           // init() (from the vtable pointer; _ZTV + 0x250)
}  // namespace vslot

// EventListenerTouchOneByOne: onTouchBegan std::function at +0x50
constexpr uint32_t TOUCH_ONTOUCHBEGAN = 0x50;
// cocos2d::Node: the onEnter / onExit std::function<void()> callbacks
constexpr uint32_t NODE_ONENTER_CALLBACK = 0x210;
constexpr uint32_t NODE_ONEXIT_CALLBACK = 0x228;

// node->setOnEnterCallback-style assignment of an inline lambda {vtable, captures} to a std::function member
inline void assignFunction(uint32_t member, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, captures);
    call(lib::FunctionVV_assign, {member, fo});
    functionDestroy(fo);
}

// SceneManager::changeSceneLayer(createSceneLayer<T>, true, true, fade) with the factory std::function inline
inline void changeSceneLayer(uint32_t factory, uint32_t fade = 1) {
    uint32_t mgr = sceneManager();
    Frame f;
    uint32_t fo = newFunction(f, lib::VT_LayerFactory, {factory});
    call(lib::SceneManager_changeSceneLayer, {mgr, fo, 1, 1, fade});
    functionDestroy(fo);
}

// The usual T::create(): operator new, constructor (Layer + vtable), virtual init(), autorelease (or delete).
inline uint32_t createLayer(uint32_t size, uint32_t vtable) {
    uint32_t self = call(fn::operator_new, {size});
    call(lib::Layer_ctor, {self});
    st32(self, vtable);
    if (vcall(self, vslot::init, {self}) == 1) {
        call(lib::Ref_autorelease, {self});
        return self;
    }
    vcall(self, vslot::release, {self});
    return 0;
}

}  // namespace smain
