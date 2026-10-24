// Helpers shared by the social / party / ranking / chat scene rewrites (scene_social): the cocos2d layer life
// cycle, guest temporaries (std::string, std::function lambdas), and the library / game functions these scenes call.
// Everything is a 32-bit guest address (Addr); the scene namespace (src/scene/SceneCommon.hpp, SceneUi.hpp) supplies
// Frame, newStr / strFree, vcall by vtable byte offset and the cocos2d::Node slot offsets.
#pragma once

#include "../core/Guest.hpp"
#include "../scene/SceneUi.hpp"

namespace social {
using namespace scene;
using Addr = uint32_t;

// ------------------------------------------------------------------------------------------------ functions
namespace sfn {
// cocos2d
constexpr uint32_t Layer_ctor = 0x88A7D1;              // cocos2d::Layer::Layer()
constexpr uint32_t Layer_dtor = 0x88A88D;              // cocos2d::Layer::~Layer() (returns this)
constexpr uint32_t Layer_init = 0x88A8A1;              // cocos2d::Layer::init()
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0C1;
constexpr uint32_t Scene_create = 0x8A5F37;            // cocos2d::Scene::create()
constexpr uint32_t Size_ctor = 0x8DF027;               // cocos2d::Size::Size(float, float)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;               // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = 0x8E4C17;               // cocos2d::Vec2::~Vec2()
constexpr uint32_t LayerColor_create = 0x88B539;       // cocos2d::LayerColor::create(const Color4B&)
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t Director_getVisibleSize = 0x8F0675;  // (Size* out, Director*)
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Sequence_create = 0x867999;         // (FiniteTimeAction*, ..., nullptr)
constexpr uint32_t DelayTime_create = 0x86852D;        // (float)
constexpr uint32_t CallFunc_create = 0x866B39;         // (const std::function<void()>&)
constexpr uint32_t EaseSineOut_create = 0x860D09;      // (ActionInterval*)
constexpr uint32_t Label_createWithTTF = 0x883825;     // (text, font, float size, const Size&, hAlign, vAlign)
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
// cocostudio
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;   // (mgr, const string&)
constexpr uint32_t Armature_create = 0x79F3C7;         // (const string&)
constexpr uint32_t AnimationData_getMovement = 0x7A767D;  // (data, const string&) -> MovementData*
// libc++
constexpr uint32_t function_vv_assign = 0x4EB9CD;      // std::function<void()>::operator=(const function&)
constexpr uint32_t function_vb_assign = 0x490109;      // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t function_vv_call = 0x470E69;        // std::function<void()>::operator()() const
constexpr uint32_t function_touchBool_swap = 0x483E4D; // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t function_touchVoid_swap = 0x4F3699; // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t vector_uint_dtor = 0x485ABB;        // std::__vector_base<unsigned>::~__vector_base
// game
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_createTopInAnimation = 0x45B03D;
constexpr uint32_t SceneUtil_createBottomInAnimation = 0x45B2F9;
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;   // (mgr, std::function<void()> by value)
constexpr uint32_t bg_vfs_readData = 0x8DD46D;         // (shared_ptr<bg::Data>* out, const char* path)
}  // namespace sfn

// cocos2d::Node / Layer virtual slots used here that SceneCommon.hpp does not name (vtable byte offsets)
namespace slot {
constexpr uint32_t release = 0x04;           // deleting destructor
constexpr uint32_t init = 0x248;   // init() (from the vtable pointer; _ZTV + 0x250)
constexpr uint32_t setPosition = 0x4C;
constexpr uint32_t setOpacity = 0x214;
constexpr uint32_t setColor = 0x22C;
constexpr uint32_t setTouchEnabled = 0x29C;  // ui::Widget::setTouchEnabled(bool)
constexpr uint32_t getAnimation = 0x298;     // cocostudio::Armature::getAnimation()
constexpr uint32_t getAnimationData = 0x88;  // ArmatureAnimation::getAnimationData()
constexpr uint32_t isPlaying = 0x3C;         // ProcessBase::isPlaying()
constexpr uint32_t setSpeedScale = 0x70;     // ArmatureAnimation::setSpeedScale(float)
}  // namespace slot

// ------------------------------------------------------------------------------------------------ values
inline Addr dir() { return call(sfn::Director_getInstance, {}); }

// ------------------------------------------------------------------------------------------------ nodes
inline void setVisible(Addr n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline float getPosY(Addr n) { return bitsf(vcall(n, node::getPositionY, {n})); }
inline void setPosY(Addr n, float y) { vcall(n, node::setPositionY, {n, fbits(y)}); }
inline float getPosX(Addr n) { return bitsf(vcall(n, node::getPositionX, {n})); }
inline void setPosX(Addr n, float x) { vcall(n, node::setPositionX, {n, fbits(x)}); }
inline void addChild(Addr parent, Addr child) { vcall(parent, node::addChild, {parent, child}); }
inline void addChildZ(Addr parent, Addr child, int32_t z) { vcall(parent, node::addChildZ, {parent, child, uint32_t(z)}); }

// a guest std::string temporary from a literal: built, passed to `use`, freed
template <class F> inline auto withStr(Addr text, uint32_t len, F use) {
    Frame f;
    Addr s = newStr(f, text, len);
    if constexpr (std::is_void_v<decltype(use(s))>) {
        use(s);
        strFree(s);
    } else {
        auto r = use(s);
        strFree(s);
        return r;
    }
}
// SceneUtil::searchNodeByName(root, "literal")
inline Addr search(Addr root, Addr name, uint32_t len) {
    return withStr(name, len, [&](Addr s) { return call(sfn::SceneUtil_searchNodeByName, {root, s}); });
}

// a std::function temporary holding an inline lambda (vtable + captured words): built, passed to `use`, destroyed
template <class F> inline auto withLambda(Addr vtable, std::initializer_list<uint32_t> captures, F use) {
    Frame f;
    Addr fo = newFunction(f, vtable, captures);
    if constexpr (std::is_void_v<decltype(use(fo))>) {
        use(fo);
        functionDestroy(fo);
    } else {
        auto r = use(fo);
        functionDestroy(fo);
        return r;
    }
}
// an empty std::function temporary (callable pointer null)
template <class F> inline auto withEmptyFunction(F use) {
    Frame f;
    Addr fo = newEmptyFunction(f);
    if constexpr (std::is_void_v<decltype(use(fo))>) {
        use(fo);
        functionDestroy(fo);
    } else {
        auto r = use(fo);
        functionDestroy(fo);
        return r;
    }
}

// ------------------------------------------------------------------------------------------------ layers
// The usual static create(): operator new, Layer::Layer, vtable, the subclass's field initialisers (`fields`),
// then init() (virtual): autorelease on success, delete and return null on failure.
template <class F> inline Addr createLayer(uint32_t size, uint32_t vtable, F fields) {
    Addr self = call(fn::operator_new, {size});
    call(sfn::Layer_ctor, {self});
    st32(self, vtable);
    fields(self);
    if (vcall(self, slot::init, {self}) == 1) {
        call(sfn::Ref_autorelease, {self});
        return self;
    }
    vcall(self, slot::release, {self});
    return 0;
}
// createScene(): a cocos2d::Scene holding one layer
inline Addr sceneWith(Addr layer, Addr sceneObj) {
    vcall(sceneObj, node::addChild, {sceneObj, layer});
    return sceneObj;
}

}  // namespace social
