#include "SceneSplash.hpp"

using namespace smain;

namespace {
constexpr uint32_t STR_SplashScene = 0xE046A2;     // "cocostudio/publish/SplashScreenScene.json"
constexpr uint32_t STR_SQEX_Logo = 0xE046CC;       // "SQEX_Logo"
constexpr uint32_t STR_DisneyLogo = 0xE046D6;      // "img/logo/Disney_logo.png"
constexpr uint32_t VT_OnEnter = 0xF64448;          // init()::$_0
constexpr uint32_t VT_OnExit = 0xF6441C;           // init()::$_1
constexpr uint32_t VT_OnTouchBegan = 0xF643F0;     // init()::$_2
constexpr uint32_t CreateSceneLayer_SceneMovie = 0x681261;
constexpr float FADE_TIME = 0.15f;
constexpr float WAIT_TIME = 2.0f;

SceneSplash *self_of(uint32_t func) { return rw::host_ptr<SceneSplash>(ld32(func + 4)); }
}  // namespace

// the original calls SceneSplash::init directly (devirtualized: the type is known after operator new)
uint32_t SceneSplash::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    call(lib::Layer_ctor, {self});
    st32(self, VTABLE);
    if (call(0x680DA1, {self}) == 1) {   // SceneSplash::init
        call(lib::Ref_autorelease, {self});
        return self;
    }
    vcall(self, vslot::release, {self});
    return 0;
}
RW_FUNCTION(0x638474, "SceneSplash::create", SceneSplash::create);

uint32_t SceneSplash::createScene() {
    uint32_t sceneObj = call(lib::Scene_create, {});
    uint32_t layer = create();
    vcall(sceneObj, node::addChild, {sceneObj, layer});
    return sceneObj;
}
RW_FUNCTION(0x680D38, "SceneSplash::createScene", SceneSplash::createScene);

SceneSplash *SceneSplash::destroy(SceneSplash *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    return rw::host_ptr<SceneSplash>(call(lib::Layer_dtor, {rw::guest_addr(self)}));
}
RW_FUNCTION(0x680D5C, "SceneSplash::~SceneSplash", SceneSplash::destroy);

void SceneSplash::destroyDelete(SceneSplash *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    call(fn::operator_delete, {call(lib::Layer_dtor, {rw::guest_addr(self)})});
}
RW_FUNCTION(0x680D7C, "SceneSplash::~SceneSplash (deleting)", SceneSplash::destroyDelete);

uint32_t SceneSplash::init(SceneSplash *self) {
    uint32_t me = rw::guest_addr(self);
    if (call(lib::Layer_init, {me}) != 1) return 0;
    assignFunction(me + NODE_ONENTER_CALLBACK, VT_OnEnter, {me});
    assignFunction(me + NODE_ONEXIT_CALLBACK, VT_OnExit, {me});
    uint32_t black = call(lib::LayerColor_create, {lib::Color4B_BLACK});
    vcall(me, node::addChild, {me, black});
    uint32_t root;
    {
        Frame f;
        uint32_t file = newStr(f, STR_SplashScene, 41);
        root = SceneUtil::createNodeWithSceneFile(file);
        strFree(file);
    }
    vcall(me, node::addChild, {me, root});
    self->logo = findNode(root, STR_SQEX_Logo, 9);
    {
        Frame f;
        uint32_t file = newStr(f, STR_DisneyLogo, 24);
        call(fn::ImageView_loadTexture, {self->logo, file, 0});
        strFree(file);
    }
    self->state = 0;
    self->logoIndex = 0;
    self->tapped = 0;
    uint32_t dispatcher = ld32(director() + lib::Director_eventDispatcher);
    uint32_t listener = call(lib::EventListenerTouchOneByOne_create, {});
    call(lib::EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_OnTouchBegan, {me});
        call(lib::FunctionTouch_swap, {fo, listener + TOUCH_ONTOUCHBEGAN});
        functionDestroy(fo);
    }
    uint32_t touchLayer = call(lib::Layer_create, {});
    vcall(me, node::addChildZ, {me, touchLayer, 1000});
    call(lib::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, touchLayer});
    return 1;
}
RW_FUNCTION(0x680DA0, "SceneSplash::init", SceneSplash::init);

void SceneSplash::update(SceneSplash *self, float) {
    if (call(lib::Node_getNumberOfRunningActions, {self->logo})) {
        if (!self->tapped) return;
        call(lib::Node_stopAllActions, {self->logo});
    }
    switch (self->state) {
    case 0:   // show the logo, then wait
        self->state = 1;
        vcall(self->logo, node::setOpacity, {self->logo, 255});
        call(fn::Node_runAction, {self->logo, call(fn::DelayTime_create, {fbits(WAIT_TIME)})});
        self->tapped = 0;
        break;
    case 1:   // waited: the movie after the last logo, else fade out
        if (self->logoIndex > 0) {
            self->state = 3;
            changeSceneLayer(CreateSceneLayer_SceneMovie);
            return;
        }
        self->state = 2;
        if (self->tapped) return;
        call(fn::Node_runAction, {self->logo, call(lib::FadeOut_create, {fbits(FADE_TIME)})});
        break;
    case 2: {   // next logo, fading in
        self->state = 0;
        self->logoIndex = self->logoIndex + 1;
        uint32_t file = ld32(LOGO_FILES + uint32_t(self->logoIndex) * 4);
        uint32_t logo = self->logo;
        {
            Frame f;
            uint32_t s = newStr(f, file);
            call(fn::ImageView_loadTexture, {logo, s, 0});
            strFree(s);
        }
        vcall(self->logo, node::setOpacity, {self->logo, 0});
        call(fn::Node_runAction, {self->logo, call(lib::FadeIn_create, {fbits(FADE_TIME)})});
        break;
    }
    default:
        break;
    }
}
RW_FUNCTION(0x681070, "SceneSplash::update", SceneSplash::update);

void SceneSplash::onEnter(uint32_t func) { call(lib::Node_scheduleUpdate, {ld32(func + 4)}); }
RW_FUNCTION(0x681402, "SceneSplash::init()::$_0::operator()", SceneSplash::onEnter);

void SceneSplash::onExit(uint32_t func) { call(lib::Node_unscheduleUpdate, {ld32(func + 4)}); }
RW_FUNCTION(0x681366, "SceneSplash::init()::$_1::operator()", SceneSplash::onExit);

uint32_t SceneSplash::onTouchBegan(uint32_t func, uint32_t, uint32_t) {
    self_of(func)->tapped = 1;
    return 1;
}
RW_FUNCTION(0x6812C6, "SceneSplash::init()::$_2::operator()", SceneSplash::onTouchBegan);
