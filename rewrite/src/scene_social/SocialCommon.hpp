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
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;              // cocos2d::Layer::Layer()
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;              // cocos2d::Layer::~Layer() (returns this)
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;              // cocos2d::Layer::init()
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_release = 0x8FD0C1;
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;            // cocos2d::Scene::create()
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;               // cocos2d::Size::Size(float, float)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;               // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;               // cocos2d::Vec2::~Vec2()
constexpr uint32_t LayerColor_create = rwlib::cocos2d::LayerColor::create;       // cocos2d::LayerColor::create(const Color4B&)
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;  // (Size* out, Director*)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;         // (FiniteTimeAction*, ..., nullptr)
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;        // (float)
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;         // (const std::function<void()>&)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;      // (ActionInterval*)
constexpr uint32_t Label_createWithTTF = rwlib::cocos2d::Label::createWithTTF__RKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEES9_fRKNS_4SizeENS_14TextHAlignmentENS_14TextVAlignmentE;     // (text, font, float size, const Size&, hAlign, vAlign)
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
// cocostudio
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;   // (mgr, const string&)
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;         // (const string&)
constexpr uint32_t AnimationData_getMovement = rwlib::cocostudio::AnimationData::getMovement;  // (data, const string&) -> MovementData*
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
