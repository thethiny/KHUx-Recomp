// SceneManager: lifetime, scene changes, loading indicator, header visibility, callbacks and accessors.
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t Director_setDefaultScheduler = rwlib::cocos2d::Director::setDefaultScheduler;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;
constexpr uint32_t Layer_create = rwlib::cocos2d::Layer::create;
constexpr uint32_t LayerColor_create = rwlib::cocos2d::LayerColor::create;          // (const Color4B&)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v;
constexpr uint32_t Memset = 0x40E7A0;                     // __aeabi_memset(dst, n, value)
constexpr uint32_t LayerFactory_assignPointer = 0x60CE4D; // std::function<Layer*()>::operator=(Layer*(*)())
constexpr uint32_t LayerFactory_call = 0x6116E1;          // std::function<Layer*()>::operator()
constexpr uint32_t CreateSceneLayer_Splash = 0x60CED9;
constexpr uint32_t LoadingIndicator_create = 0x60CEE1;
constexpr uint32_t LoadingIndicator_show = 0x509CE1, LoadingIndicator_hide = 0x509D69;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t TouchBeganFunction_swap = 0x483E4D;    // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t TouchFunction_swap = 0x4F3699;         // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t FunctionV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionB_assign = 0x490109;           // std::function<void(bool)>::operator=
constexpr uint32_t FunctionBool_assign = 0x53327D;        // std::function<bool()>::operator=
constexpr uint32_t FunctionBool_call = 0x51022D;          // std::function<bool()>::operator()
constexpr uint32_t FinishFunction_assign = 0x6075E1;      // std::function<void(std::function<void(bool)>)>::operator=
constexpr uint32_t PopupDialog_create = 0x4F3C15, PopupDialog_open = 0x4F420D;
constexpr uint32_t PurchaseManager_getInstance = 0x4D7A71, PurchaseManager_abort = 0x4D8389;
constexpr uint32_t APIManager_clearAllResponses = 0x4C3141;
constexpr uint32_t OtherMenuDialog_create = 0x59E84D, OtherMenuDialog_open = 0x59E8E1;

constexpr uint32_t COLOR4B_BLACK = 0x1007EBC;
constexpr uint32_t STR_Empty = 0xDF8A1A, STR_Alerts = 0xE0311C;
constexpr int32_t TUTORIAL_PHASE_HEADER = 710;            // the header can be touched from this phase on

// lambdas
constexpr uint32_t VT_Init_FadeTouch = 0xF6094C;          // init $_0: the fade layer swallows touches
constexpr uint32_t VT_Init_EffectBegan = 0xF60920;        // $_1 [this]: touch effect at the touch
constexpr uint32_t VT_Init_EffectMoved = 0xF608F4;        // $_2 [this]
constexpr uint32_t VT_Init_OnEnter = 0xF608C8;            // $_3 [this]
constexpr uint32_t VT_Init_OnExit = 0xF6089C;             // $_4 [this]
constexpr uint32_t VT_Init_BackKey = 0xF60870;            // $_5 [this]
constexpr uint32_t VT_MoveToTitle_OnClose = 0xF60844;     // moveToTitleForcibly $_6: [callback, this] (heap)
constexpr uint32_t VT_PrevChange_Done = 0xF603A0;         // callPrevChangeNextSceneAPIs $_26: [this x3]
constexpr uint32_t VT_HideInfoHeader_Done = 0xF60374;     // $_27: [this, callback] (heap)
constexpr uint32_t VT_HideTitleHeader_Done = 0xF60348;    // $_28
constexpr uint32_t VT_HideHeader_Done = 0xF6031C;         // $_29: [this, header, callback] (heap)

uint32_t mgrAddr(SceneManager *self) { return rw::guest_addr(self); }
bool hasParent(uint32_t node) { return vcall(node, node::getParent, {node}) != 0; }

// pop every entry of a vector of 24-byte std::function slots (the callable pointer at +16)
void clearFunctionVector(rw::Field<uint32_t> *vec) {
    uint32_t begin = vec[0];
    uint32_t end = vec[1];
    while (end != begin) {
        uint32_t entry = end - 24;
        vec[1] = entry;
        functionDestroy(entry);
        end = vec[1];
    }
}

// The header / info header / title header slide out with an animation whose callback (a heap lambda holding
// `self`, optionally the node, and the caller's callback) finishes the hide.
void slideOut(SceneManager *self, uint32_t widget, uint32_t vtable, bool withWidget, uint32_t callback, bool left) {
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    uint32_t done = f.alloc(20);
    st32(done + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, vtable);
        st32(heap + 8, mgrAddr(self));
        if (withWidget) st32(heap + 12, widget);
        call(fn::function_vv_move, {heap + 16, cb});
        if (!withWidget) st32(heap + 8, mgrAddr(self));
    }
    st32(done + 16, heap);
    uint32_t action = left ? SceneUtil::createLeftOutAnimation(widget, 0.0f, 0.0f, done)
                           : SceneUtil::createTopOutAnimation(widget, 0.0f, 0.0f, done);
    call(fn::Node_runAction, {widget, action});
    functionDestroy(done);
    functionDestroy(cb);
}

void callIfSet(uint32_t function) {
    if (ld32(function + 16)) call(fn::function_vv_call, {function});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ lifetime

// A new cocos2d::Scene holding the SceneManager layer.
SceneManager::Addr SceneManager::createScene() {
    uint32_t scene = call(Scene_create, {});
    uint32_t mgr = mgrAddr(getInstance());
    vcall(scene, node::addChild, {scene, mgr});
    return scene;
}
RW_FUNCTION(0x60C248, "SceneManager::createScene", SceneManager::createScene);

// The instance, created on first use with the default scheduler priority raised meanwhile.
SceneManager *SceneManager::getInstance() {
    if (!rw::load<uint32_t>(INSTANCE)) {
        uint32_t old = call(Director_setDefaultScheduler, {director(), 0x7FFFFFFF});
        rw::save<uint32_t>(INSTANCE, mgrAddr(create()));
        call(Ref_retain, {rw::load<uint32_t>(INSTANCE)});
        call(Director_setDefaultScheduler, {director(), old});
    }
    return instance();
}
RW_FUNCTION(0x60C26C, "SceneManager::getInstance", SceneManager::getInstance);

SceneManager *SceneManager::create() {
    uint32_t mgr = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<SceneManager>(mgr));
    if (vcall(mgr, 0x248 /* init */, {mgr}) == 1) {
        call(Ref_autorelease, {mgr});
        return rw::host_ptr<SceneManager>(mgr);
    }
    vcall(mgr, 0x04 /* deleting destructor */, {mgr});
    return nullptr;
}
RW_FUNCTION(0x60C2BC, "SceneManager::create", SceneManager::create);

SceneManager *SceneManager::construct(SceneManager *self) {
    uint32_t m = mgrAddr(self);
    call(Layer_ctor, {m});
    st32(m, 0xF97C60);                                   // SceneManager vtable
    self->field2A4 = 0;
    self->field2A0 = 0;
    self->flag2A8 = 0;
    self->nextB = 0;
    self->nextA = 0;
    self->field2B8 = 0;
    self->nextD = 0;
    self->nextC = 0;
    self->nextE = 0;
    self->field2E4 = 0;
    self->field2E8 = 0;
    self->flag2EC = 0;
    call(Memset, {m + 0x288, 0x14, 0});
    call(Memset, {m + 0x2D0, 0x13, 0});
    st32(m + 0x2F4, 0);                                  // field2F4, flag2F6, avatarBoardDirty
    self->field2F0 = 0;
    self->avatarBoardMode = 1;
    self->flag2FC = 0;
    self->field300 = 0.4f;
    self->apRecoveryText = 0;
    self->flag308 = 0;
    self->flag309 = 0;
    self->flag30A = 1;
    self->flag30B = 1;
    self->field30C = 0;
    self->raidAlertsShown = 1;
    self->infoHeaderOffset = 0.0f;
    self->sceneState = 2;
    st32(m + 0x324, 0);
    self->flag329 = 0;
    self->headerRequested = 0;
    self->abortOnNextScene = 0;
    self->flag31E = 0;
    self->flag31D = 0;
    self->flag31C = 0;
    self->touchEffectEnabled = 0;
    call(Vec2_ctor, {m + 0x32C});
    self->backButtonType = 0;
    self->header = 0;
    self->infoHeaderShown = 0;
    call(Memset, {m + 0x340, 0x24, 0});
    self->globalMenuSelected = 0xFFFFFFFFu;
    self->globalMenuOpen = 0;
    self->globalMenuClosed = 0;
    st32(self->addr() + 0x380, 0);   // globalMenuSceneFactory.__f_
    self->titleHeaderShown = 0;
    st32(m + 0x3D8, 0);                                  // nextSceneFactory: empty
    self->field474 = 0;
    self->field478 = 0;
    st32(self->addr() + 0x490, 0);   // chatClose.__f_
    self->flag498 = 0;
    st32(m + 0x4E0, 0);                                  // prepareFinishSceneCallback: empty
    st32(m + 0x4F8, 0);                                  // finishSceneCallback
    st32(m + 0x510, 0);                                  // function500
    st32(m + 0x528, 0);                                  // chatButtonCallback
    st32(m + 0x540, 0);                                  // onChangeProfile
    st32(m + 0x558, 0);                                  // onFinishLoading
    call(Memset, {m + 0x38C, 0x24, 0});
    call(Memset, {m + 0x3E0, 0x91, 0});
    self->field4A4 = 0;
    self->field4A0 = 0;
    self->field49C = 0;
    call(Memset, {m + 0x4A8, 0x19, 0});
    rw::save<uint16_t>(m + 0x4CC, 0);                    // loadingBlocked, flag4CD
    self->loadingIndicator = 0;
    self->field4C4 = 0;
    self->background = 0;
    st32(m + 0x568, 0);                                  // backgroundName: empty string
    st32(m + 0x564, 0);
    st32(m + 0x560, 0);
    self->apShown = 0xFFFFFFFFu;
    self->apMaxShown = 0xFFFFFFFFu;
    self->apSeconds = 0xFFFFFFFFu;
    st32(m + 0x5C0, 0);                                  // onBackKey: empty
    self->barButtonCopy = 0;
    self->faceImageUrls[1] = 0;                          // map: no root
    self->faceImageUrls[2] = 0;                          // size 0
    self->flag588 = 0;
    st32(self->addr() + 0x584, 0);   // pushToken: empty std::string (zeroed words)
    st32(self->addr() + 0x580, 0);
    st32(self->addr() + 0x57C, 0);
    call(Memset, {m + 0x58C, 0x24, 0});
    self->faceImageUrls[0] = m + 0x5D0;                  // begin = the end node
    return self;
}
RW_FUNCTION(0x60C310, "SceneManager::SceneManager", SceneManager::construct);

// Build the layers (scene, popups, fade, touch effects), the loading indicator and the touch / back key hooks.
bool SceneManager::init(SceneManager *self) {
    uint32_t m = mgrAddr(self);
    call(LayerFactory_assignPointer, {m + 0x3C8, CreateSceneLayer_Splash});
    self->fadeLayer = call(LayerColor_create, {COLOR4B_BLACK});
    call(Ref_retain, {self->fadeLayer});
    vcall(m, node::addChildZ, {m, self->fadeLayer, 11});
    Frame f;
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    {
        uint32_t began = newFunction(f, VT_Init_FadeTouch, {});
        call(TouchBeganFunction_swap, {began, listener + 0x50});
        functionDestroy(began);
    }
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {ld32(director() + 0x84), listener, self->fadeLayer});
    self->popupLayer = call(Layer_create, {});
    vcall(m, node::addChildZ, {m, self->popupLayer, 6});
    self->popupLayer2 = call(Layer_create, {});
    vcall(m, node::addChildZ, {m, self->popupLayer2, 7});
    self->popupLayer3 = call(Layer_create, {});
    vcall(m, node::addChildZ, {m, self->popupLayer3, 13});
    self->popupLayerDerived = call(Layer_create, {});
    vcall(m, node::addChildZ, {m, self->popupLayerDerived, 8});
    self->popupLayerDerived2 = call(Layer_create, {});
    vcall(m, node::addChildZ, {m, self->popupLayerDerived2, 9});
    self->loadingIndicator = call(LoadingIndicator_create, {});
    call(Ref_retain, {self->loadingIndicator});
    uint32_t effects = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {effects, 0});
    {
        Frame g;
        uint32_t began = newFunction(g, VT_Init_EffectBegan, {m});
        call(TouchBeganFunction_swap, {began, effects + 0x50});
        functionDestroy(began);
        uint32_t moved = newFunction(g, VT_Init_EffectMoved, {m});
        call(TouchFunction_swap, {moved, effects + 0x68});
        functionDestroy(moved);
    }
    self->touchEffectLayer = call(Layer_create, {});
    vcall(m, node::addChildZ, {m, self->touchEffectLayer, 10});
    call(EventDispatcher_addEventListenerWithSceneGraphPriority,
         {ld32(director() + 0x84), effects, self->touchEffectLayer});
    self->touchEffectEnabled = 1;
    vcall(self->touchEffectLayer, node::setVisible, {self->touchEffectLayer, 1});
    {
        Frame g;
        uint32_t onEnter = newFunction(g, VT_Init_OnEnter, {m});
        call(FunctionV_assign, {m + NODE_ON_ENTER_CALLBACK, onEnter});
        functionDestroy(onEnter);
        uint32_t onExit = newFunction(g, VT_Init_OnExit, {m});
        call(FunctionV_assign, {m + NODE_ON_EXIT_CALLBACK, onExit});
        functionDestroy(onExit);
    }
    call(fn::string_assign, {m + 0x350, STR_Empty, 0});
    {
        Frame g;
        uint32_t back = newFunction(g, VT_Init_BackKey, {m});
        SceneUtil::addBackKeyCallback(m, back);
        functionDestroy(back);
    }
    return true;
}
RW_FUNCTION(0x60CB00, "SceneManager::init", SceneManager::init);

// ------------------------------------------------------------------------------------------------ scene changes

void SceneManager::setEnabledTouchEffect(SceneManager *self, uint32_t enabled) {
    self->touchEffectEnabled = uint8_t(enabled);
    vcall(self->touchEffectLayer, node::setVisible, {self->touchEffectLayer, enabled});
}
RW_FUNCTION(0x60CF64, "SceneManager::setEnabledTouchEffect", SceneManager::setEnabledTouchEffect);

// An error dialog (text `textId`) whose OK goes back to the title screen; the API manager is marked to restart.
void SceneManager::moveToTitleForcibly(SceneManager *self, int32_t textId, Addr callback) {
    if (hasParent(self->loadingIndicator)) call(LoadingIndicator_hide, {self->loadingIndicator});
    vcall(self->popupLayer3, node::removeAllChildren, {self->popupLayer3});
    uint32_t dialog = call(PopupDialog_create, {0, 0});
    uint32_t parent = self->popupLayer3;
    Frame f;
    uint32_t text = textData(f, textId);
    uint32_t message = newStr(f, textDataStr(text));
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, VT_MoveToTitle_OnClose);
        functionCopy(heap + 8, cb);
        st32(heap + 0x20, mgrAddr(self));
    }
    st32(onClose + 16, heap);
    call(PopupDialog_open, {dialog, parent, message, onClose});
    functionDestroy(onClose);
    functionDestroy(cb);
    strFree(message);
    sharedRelease(text);
    call(fn::AudioPlaySystemSEPopUp, {});
    st32(call(fn::APIManager_getInstance, {}), 1);
}
RW_FUNCTION(0x60CF7C, "SceneManager::moveToTitleForcibly", SceneManager::moveToTitleForcibly);

void SceneManager::enableLoadingIndicator(SceneManager *self, uint32_t enable) {
    bool shown = hasParent(self->loadingIndicator);
    if (enable == 1) {
        if (!shown) vcall(mgrAddr(self), node::addChildZ, {mgrAddr(self), self->loadingIndicator, 12});
        call(LoadingIndicator_show, {self->loadingIndicator});
    } else if (shown) {
        call(LoadingIndicator_hide, {self->loadingIndicator});
    }
}
RW_FUNCTION(0x60D168, "SceneManager::enableLoadingIndicator", SceneManager::enableLoadingIndicator);

static bool noChildren(uint32_t layer) {
    uint32_t children = vcall(layer, node::getChildren, {layer});
    return ld32(children) == ld32(children + 4);
}

// moveToTitleForcibly, but only when nothing else is going on (a header, no scene change, no loading, no popups,
// no pending API call).
void SceneManager::moveToTitleIfPossible(SceneManager *self, int32_t textId, Addr callback) {
    if (!self->header || self->globalMenuOpen) return;
    if (hasParent(self->loadingIndicator)) return;
    if (!noChildren(self->popupLayer) || !noChildren(self->popupLayer2) || !noChildren(self->popupLayer3) ||
        !noChildren(self->popupLayerDerived) || !noChildren(self->popupLayerDerived2))
        return;
    if (ld8(call(fn::APIManager_getInstance, {}) + 4)) return;
    if (self->flag4CD) return;
    moveToTitleForcibly(self, textId, callback);
}
RW_FUNCTION(0x60D1B0, "SceneManager::moveToTitleIfPossible", SceneManager::moveToTitleIfPossible);

// Before a scene change: send the pending medal locks (without the loading indicator), then go on.
void SceneManager::callPrevChangeNextSceneAPIs(SceneManager *self) {
    SceneManager *mgr = getInstance();
    Frame f;
    uint32_t empty = newEmptyFunction(f);
    mgr->loadingBlocked = 1;
    call(FunctionB_assign, {mgrAddr(mgr) + 0x548, empty});
    functionDestroy(empty);
    uint32_t m = mgrAddr(self);
    uint32_t done = newFunction(f, VT_PrevChange_Done, {m, m, m});
    SceneUtil::callUpdateLockMedalAPI(done);
    functionDestroy(done);
}
RW_FUNCTION(0x60E78C, "SceneManager::callPrevChangeNextSceneAPIs", SceneManager::callPrevChangeNextSceneAPIs);

void SceneManager::setCanTouchHeader(SceneManager *self, uint32_t canTouch) {
    uint32_t v = SceneUtil::getNexTutorialphase() >= TUTORIAL_PHASE_HEADER ? canTouch : 0;
    self->flag588 = uint8_t(v);
}
RW_FUNCTION(0x60E86C, "SceneManager::setCanTouchHeader", SceneManager::setCanTouchHeader);

// Create the next scene layer from nextSceneFactory (after a reset of popups, header and background when
// abortOnNextScene is set).
void SceneManager::createNextScene(SceneManager *self) {
    uint32_t m = mgrAddr(self);
    if (self->abortOnNextScene) {
        self->abortOnNextScene = 0;
        call(PurchaseManager_abort, {call(PurchaseManager_getInstance, {})});
        call(APIManager_clearAllResponses, {call(fn::APIManager_getInstance, {})});
        clearFunctionVector(self->popupCanClose);
        clearFunctionVector(self->popupOnClose);
        self->popupFlags[1] = 0;
        vcall(self->popupLayer, node::removeAllChildren, {self->popupLayer});
        vcall(self->popupLayer2, node::removeAllChildren, {self->popupLayer2});
        vcall(self->popupLayerDerived, node::removeAllChildren, {self->popupLayerDerived});
        vcall(self->popupLayerDerived2, node::removeAllChildren, {self->popupLayerDerived2});
        {
            Frame f;
            uint32_t none = newEmptyFunction(f);
            hideHeader(self, none);
            functionDestroy(none);
        }
        removeBackground(self);
        st32(call(fn::APIManager_getInstance, {}), 0);
    }
    uint32_t layer = call(LayerFactory_call, {m + 0x3C8});
    if (layer) {
        vcall(m, node::addChildZ, {m, layer, 2});
        self->sceneLayer = layer;
        call(Ref_retain, {layer});
    }
    functionDestroy(m + 0x3C8);
    st32(m + 0x3D8, 0);
}
RW_FUNCTION(0x60E894, "SceneManager::createNextScene", SceneManager::createNextScene);

void SceneManager::removeBackground(SceneManager *self) {
    uint32_t m = mgrAddr(self);
    if (self->background) vcall(m, node::removeChild, {m, self->background, 1});
    self->background = 0;
    call(fn::string_assign, {m + 0x560, STR_Empty, 0});
    self->backgroundType = 0;
}
RW_FUNCTION(0x611690, "SceneManager::removeBackground", SceneManager::removeBackground);

// ------------------------------------------------------------------------------------------------ header

void SceneManager::changeTitleHeader(SceneManager *, uint32_t, uint32_t, uint32_t) {}
RW_FUNCTION(0x610D9C, "SceneManager::changeTitleHeader", SceneManager::changeTitleHeader);

void SceneManager::hideHeader(SceneManager *self, Addr callback) {
    self->headerRequested = 0;
    uint32_t header = self->header;
    if (!header) {
        callIfSet(callback);
        return;
    }
    self->header = 0;
    slideOut(self, header, VT_HideHeader_Done, true, callback, false);
}
RW_FUNCTION(0x61154C, "SceneManager::hideHeader", SceneManager::hideHeader);

// Show the info header: the header moves down by the info header's height.
void SceneManager::showInfoHeader(SceneManager *self) {
    self->infoHeaderShown = 1;
    if (self->infoHeader) {
        uint32_t info = self->infoHeader;
        self->infoHeaderOffset = -0.0f - ldf(vcall(info, node::getContentSize, {info}) + 4);
    }
    uint32_t header = self->header;
    if (header) {
        float visibleHeight = visibleSize().height;
        float height = ldf(vcall(header, node::getContentSize, {header}) + 4);
        vcall(header, node::setPositionY, {header, fbits((visibleHeight - height) + float(self->infoHeaderOffset))});
    }
}
RW_FUNCTION(0x612E98, "SceneManager::showInfoHeader", SceneManager::showInfoHeader);

void SceneManager::hideInfoHeader(SceneManager *self, Addr callback) {
    self->infoHeaderShown = 0;
    self->infoHeaderOffset = 0.0f;
    uint32_t info = self->infoHeader;
    if (!info) {
        callIfSet(callback);
        return;
    }
    slideOut(self, info, VT_HideInfoHeader_Done, false, callback, false);
}
RW_FUNCTION(0x612F4C, "SceneManager::hideInfoHeader", SceneManager::hideInfoHeader);

void SceneManager::hideHeaderRight(SceneManager *) {}
RW_FUNCTION(0x613090, "SceneManager::hideHeaderRight", SceneManager::hideHeaderRight);

void SceneManager::showTitleHeader(SceneManager *self, uint32_t main, uint32_t sub1, uint32_t sub2) {
    self->titleMain = main;
    self->titleSub1 = sub1;
    self->titleSub2 = sub2;
    self->titleHeaderShown = 1;
}
RW_FUNCTION(0x613092, "SceneManager::showTitleHeader", SceneManager::showTitleHeader);

void SceneManager::hideTitleHeader(SceneManager *self, Addr callback) {
    self->titleHeaderShown = 0;
    uint32_t title = self->titleHeader;
    if (!title) {
        callIfSet(callback);
        return;
    }
    slideOut(self, title, VT_HideTitleHeader_Done, false, callback, true);
}
RW_FUNCTION(0x6130B0, "SceneManager::hideTitleHeader", SceneManager::hideTitleHeader);

void SceneManager::showHeader(SceneManager *self) { self->headerRequested = 1; }
RW_FUNCTION(0x6131EC, "SceneManager::showHeader", SceneManager::showHeader);

// The header's raid alert button shows when raids are listed (and alerts were enabled).
void SceneManager::updateHeaderRaidAlertsBtn(SceneManager *self) {
    uint32_t header = self->header;
    if (!header) return;
    uint32_t alerts = findNode(header, STR_Alerts, 6);
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t show = 0;
    if (ld32(api + 0x158) != ld32(api + 0x154)) show = self->raidAlertsShown ? 1 : 0;
    vcall(alerts, node::setVisible, {alerts, show});
    self->raidAlertsShown = 1;
}
RW_FUNCTION(0x6131F8, "SceneManager::updateHeaderRaidAlertsBtn", SceneManager::updateHeaderRaidAlertsBtn);

void SceneManager::setHeaderRaidAlertsBtn(SceneManager *self, uint32_t shown) { self->raidAlertsShown = uint8_t(shown); }
RW_FUNCTION(0x6132D4, "SceneManager::setHeaderRaidAlertsBtn", SceneManager::setHeaderRaidAlertsBtn);

void SceneManager::showHeaderRight(SceneManager *) {}
RW_FUNCTION(0x613E24, "SceneManager::showHeaderRight", SceneManager::showHeaderRight);

void SceneManager::setBackButtonType(SceneManager *self, uint32_t type) { self->backButtonType = type; }
RW_FUNCTION(0x613E26, "SceneManager::setBackButtonType", SceneManager::setBackButtonType);

void SceneManager::setBackButtonTypeVisible(SceneManager *, uint32_t) {}
RW_FUNCTION(0x613E2E, "SceneManager::setBackButtonTypeVisible", SceneManager::setBackButtonTypeVisible);

// The "other" menu (options, help...) on the popup layer.
void SceneManager::openOtherPopup(SceneManager *) {
    uint32_t dialog = call(OtherMenuDialog_create, {});
    call(OtherMenuDialog_open, {dialog, getInstance()->popupLayer});
}
RW_FUNCTION(0x61255C, "SceneManager::openOtherPopup", SceneManager::openOtherPopup);

// ------------------------------------------------------------------------------------------------ callbacks

void SceneManager::setPrepareFinishSceneCallback(SceneManager *self, Addr function) {
    call(FunctionBool_assign, {mgrAddr(self) + 0x4D0, function});
}
RW_FUNCTION(0x613E30, "SceneManager::setPrepareFinishSceneCallback", SceneManager::setPrepareFinishSceneCallback);

void SceneManager::setFinishSceneCallback(SceneManager *self, Addr function) {
    call(FinishFunction_assign, {mgrAddr(self) + 0x4E8, function});
}
RW_FUNCTION(0x613E3E, "SceneManager::setFinishSceneCallback", SceneManager::setFinishSceneCallback);

void SceneManager::setChatButtonCallback(SceneManager *self, Addr function) {
    call(FunctionBool_assign, {mgrAddr(self) + 0x518, function});
}
RW_FUNCTION(0x613E4C, "SceneManager::setChatButtonCallback", SceneManager::setChatButtonCallback);

void SceneManager::setOnBackKeyCallback(SceneManager *self, Addr function) {
    call(FunctionV_assign, {mgrAddr(self) + 0x5B0, function});
}
RW_FUNCTION(0x613E5A, "SceneManager::setOnBackKeyCallback", SceneManager::setOnBackKeyCallback);

void SceneManager::setOnChangeProfile(SceneManager *self, Addr function) {
    call(FunctionV_assign, {mgrAddr(self) + 0x530, function});
}
RW_FUNCTION(0x62160E, "SceneManager::setOnChangeProfile", SceneManager::setOnChangeProfile);

void SceneManager::onChangeProfile(SceneManager *self) { callIfSet(mgrAddr(self) + 0x530); }
RW_FUNCTION(0x6215F6, "SceneManager::onChangeProfile", SceneManager::onChangeProfile);

// The scene may finish unless its prepare-finish callback says no.
bool SceneManager::canFinishScene(SceneManager *self) {
    uint32_t m = mgrAddr(self);
    if (!ld32(m + 0x4E0)) return true;
    return call(FunctionBool_call, {m + 0x4D0}) == 1;
}
RW_FUNCTION(0x6214F4, "SceneManager::canFinishScene", SceneManager::canFinishScene);

SceneManager::Addr SceneManager::getPopupLayer(SceneManager *self) { return self->popupLayer; }
RW_FUNCTION(0x613E68, "SceneManager::getPopupLayer", SceneManager::getPopupLayer);

SceneManager::Addr SceneManager::getPopupLayer2(SceneManager *self) { return self->popupLayer2; }
RW_FUNCTION(0x613E70, "SceneManager::getPopupLayer2", SceneManager::getPopupLayer2);

SceneManager::Addr SceneManager::getPopupLayer3(SceneManager *self) { return self->popupLayer3; }
RW_FUNCTION(0x613E78, "SceneManager::getPopupLayer3", SceneManager::getPopupLayer3);

SceneManager::Addr SceneManager::getPopupLayerDerived(SceneManager *self) { return self->popupLayerDerived; }
RW_FUNCTION(0x613E80, "SceneManager::getPopupLayerDerived", SceneManager::getPopupLayerDerived);

SceneManager::Addr SceneManager::getPopupLayerDerived2(SceneManager *self) { return self->popupLayerDerived2; }
RW_FUNCTION(0x613E88, "SceneManager::getPopupLayerDerived2", SceneManager::getPopupLayerDerived2);

uint8_t SceneManager::getChatPopupDisp(SceneManager *self) { return self->chatPopupDisp; }
RW_FUNCTION(0x613E90, "SceneManager::getChatPopupDisp", SceneManager::getChatPopupDisp);

void SceneManager::chatBlockListAdd(SceneManager *self, int32_t count) { self->chatBlockCount += count; }
RW_FUNCTION(0x61E6F8, "SceneManager::chatBlockListAdd", SceneManager::chatBlockListAdd);
