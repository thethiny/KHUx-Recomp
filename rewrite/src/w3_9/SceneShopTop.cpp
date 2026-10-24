// SceneShopTop: life cycle, the shop API call, the layout (draw list, bottom buttons), in / out animations and back.
#include "SceneShopTop.hpp"

using namespace scene;
using namespace shop_top;
using Self = SceneShopTop;

namespace {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;                 // cocos2d::Layer::Layer
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;                 // cocos2d::Layer::~Layer
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;                 // cocos2d::Layer::init
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t Node_create = rwlib::cocos2d::Node::create;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;                  // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ImageView_create = rwlib::cocos2d::ui::ImageView::create;
constexpr uint32_t ImageButton_create = 0x531239;         // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;   // (std::function by value)
constexpr uint32_t DrawScrollView_create = 0x68CDBD;
constexpr uint32_t DrawScrollView_setCanTouch = 0x5B13AD;
constexpr uint32_t Function_vv_assign = 0x4EB9CD;         // std::function<void()>::operator=(const function&)
constexpr uint32_t Function_vv_swap = 0x4EE86D;           // std::function<void()>::swap
constexpr uint32_t Function_touch_swap = 0x483E4D;        // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;   // (mgr, BackGroundType, const string&)
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;    // (node, float, float, function<void()>)
constexpr uint32_t SceneUtil_createLeftOutAnimation = 0x45B735;
constexpr uint32_t APIManager_executeGachaList = 0x4CE3D5;     // (api, bool tutorial, const function<void(bool)>&)
constexpr uint32_t InOut_runOutAnimation = 0x6F24B5;      // IInOutAnimation::runOutAnimation(function<void()>)

constexpr uint32_t VSLOT_INIT = 0x250;                    // from the vtable start: init()
constexpr uint32_t VSLOT_DELETE = 0x04;                   // deleting destructor (through the object)
constexpr uint32_t VSLOT_IN_ANIMATION = 0x300;            // the scene's virtual slot called at the end of init
constexpr uint32_t VSLOT_LAYOUT_0x29C = 0x29C;            // ui::Layout virtual (Scroll_Area, false)
constexpr uint32_t APIMANAGER_GACHA_STATE = 0x44;         // APIManager: object whose byte +0x34 bit 0x40 shows Buy1
constexpr uint32_t VEC2_ANCHOR_BOTTOM_LEFT = 0x1006958;   // cocos2d::Vec2::ANCHOR_BOTTOM_LEFT

constexpr int32_t TEXT_TICKER = 100620008;
constexpr int32_t TEXT_SELL = 101100006, TEXT_BUY1 = 102600002, TEXT_BUY3 = 102600004;
constexpr int32_t TUTORIAL_PHASE_SHOP = 221;              // before this phase the gacha list is the tutorial one

constexpr uint32_t STR_DeckBG = 0xE03AF2;                 // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t STR_ShopDrawJson = 0xE049EC;           // "cocostudio/publish/ShopDraw_3Plate_ver120.json"
constexpr uint32_t STR_ShopDraw_All = 0xE04A1B;
constexpr uint32_t STR_Scroll_Area = 0xDF9B59;
constexpr uint32_t STR_Button_Jewel = 0xE04A28, STR_But_Spe01 = 0xE04A35;
constexpr uint32_t STR_ImageButton_JewelDummy = 0xE04A3F, STR_ImageButton_Jewel = 0xE04A56;
constexpr uint32_t STR_Button_Sell2 = 0xE04A68, STR_But03 = 0xE04A75, STR_ImageButton_Sell2 = 0xE04A7B;
constexpr uint32_t STR_Txt_Buy1 = 0xE04A8D, STR_Button_Buy1 = 0xE04A96, STR_But01 = 0xDFD027;
constexpr uint32_t STR_ImageButton_Buy1 = 0xE04AA2, STR_Button_Buy3 = 0xE04AB3, STR_ImageButton_Buy3 = 0xE04ABF;
constexpr uint32_t STR_Txt_Buy3 = 0xE04AD0;

// ImageButton::create(name, false) with a temporary std::string
uint32_t imageButton(uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    uint32_t button = call(ImageButton_create, {s, 0});
    strFree(s);
    return button;
}
// parent->addChild(child, z, name) with a temporary std::string
void addChildNamed(uint32_t parent, uint32_t child, uint32_t z, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    vcall(parent, node::addChildZName, {parent, child, z, s});
    strFree(s);
}
// button->addTouchEventListener(lambda [captures]) through ImageButton's own listener setter
void addButtonTouch(uint32_t button, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t cb = newFunction(f, vtable, captures);
    call(ImageButton_addTouchEventListener, {button, cb});
    functionDestroy(cb);
}
// An image button over the `buttonName` widget, added to it as `childName` with a touch lambda
uint32_t makeButton(uint32_t widget, uint32_t buttonName, uint32_t buttonLen, uint32_t image, uint32_t imageLen,
                    uint32_t childName, uint32_t childLen, uint32_t vtable, uint32_t self, uint32_t *buttonOut) {
    uint32_t target = findNode(widget, buttonName, buttonLen);
    uint32_t button = imageButton(image, imageLen);
    call(ImageButton_setTargetImageView, {button, target});
    addChildNamed(target, button, 0, childName, childLen);
    addButtonTouch(button, vtable, {self});
    *buttonOut = button;
    return target;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
SceneShopTop::Addr SceneShopTop::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->inOutVtable = INOUT_VTABLE;
    s->field_290 = 0;
    s->checkDuplicateMedals = 0;
    s->canTouch = 0;
    s->scrollView = 0;
    s->canBack = 0;
    if (call(ld32(VTABLE_START + VSLOT_INIT), {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, VSLOT_DELETE, {self});
    return 0;
}
RW_FUNCTION(0x5B27CC, "SceneShopTop::create", SceneShopTop::create);

SceneShopTop::Addr SceneShopTop::createScene() {
    uint32_t sceneObj = call(Scene_create, {});
    uint32_t layer = create();
    vcall(sceneObj, node::addChild, {sceneObj, layer});
    return sceneObj;
}
RW_FUNCTION(0x68BCC4, "SceneShopTop::createScene", SceneShopTop::createScene);

SceneShopTop::Addr SceneShopTop::destroy(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->inOutVtable = INOUT_VTABLE;
    return call(Layer_dtor, {self});
}
RW_FUNCTION(0x68BCE8, "SceneShopTop::~SceneShopTop", SceneShopTop::destroy);

SceneShopTop::Addr SceneShopTop::destroyThunk(Addr inOut) {
    Self *s = rw::host_ptr<Self>(inOut - INOUT_OFFSET);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->inOutVtable = INOUT_VTABLE;
    return call(Layer_dtor, {inOut - INOUT_OFFSET});
}
RW_FUNCTION(0x68BD18, "SceneShopTop::~SceneShopTop (thunk)", SceneShopTop::destroyThunk);

void SceneShopTop::destroyDelete(Addr self) { call(fn::operator_delete, {destroy(self)}); }
RW_FUNCTION(0x68BD4C, "SceneShopTop::~SceneShopTop (deleting)", SceneShopTop::destroyDelete);

void SceneShopTop::destroyDeleteThunk(Addr inOut) { call(fn::operator_delete, {destroyThunk(inOut)}); }
RW_FUNCTION(0x68BD80, "SceneShopTop::~SceneShopTop (deleting thunk)", SceneShopTop::destroyDeleteThunk);

// ------------------------------------------------------------------------------------------------ API
// The gacha list request (tutorial list before phase 221); `callback(true)` runs on success ($_0).
void SceneShopTop::callShopAPI(Addr callback) {
    uint32_t api = call(fn::APIManager_getInstance, {});
    int32_t phase = int32_t(call(SceneUtil_getNexTutorialphase, {}));
    Frame f;
    uint32_t captured = f.alloc(20);
    call(fn::function_vb_copy, {captured, callback});
    uint32_t onResult = f.alloc(20);
    heapFunction(onResult, VT_GachaListResult, 0x28, 0x10, fn::function_vb_move, captured);
    call(APIManager_executeGachaList, {api, phase < TUTORIAL_PHASE_SHOP ? 1u : 0u, onResult});
    functionDestroy(onResult);
    functionDestroy(captured);
}
RW_FUNCTION(0x68BB9C, "SceneShopTop::callShopAPI", SceneShopTop::callShopAPI);

// ------------------------------------------------------------------------------------------------ layout
uint32_t SceneShopTop::init(Addr self) {
    if (call(Layer_init, {self}) != 1) return 0;
    Self *s = rw::host_ptr<Self>(self);
    {
        Frame f;
        uint32_t onEnter = newFunction(f, VT_EnterScene, {self});
        call(Function_vv_assign, {self + 0x210, onEnter});
        functionDestroy(onEnter);
        uint32_t onExit = newFunction(f, VT_ExitScene, {self});
        call(Function_vv_assign, {self + 0x228, onExit});
        functionDestroy(onExit);
        uint32_t mgr = sceneManager();
        uint32_t onBack = newFunction(f, VT_BackKey, {self});
        call(SceneManager_setOnBackKeyCallback, {mgr, onBack});
        functionDestroy(onBack);
    }
    s->canBack = 1;
    call(SceneManager_showHeader, {sceneManager()});
    {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t bg = newStr(f, STR_DeckBG, 0x1A);
        call(SceneManager_changeBackground, {mgr, 0, bg});
        strFree(bg);
    }
    {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t text = textData(f, TEXT_TICKER);
        uint32_t s2 = newStr(f, textDataStr(text));
        call(SceneManager_changeTickerMessage, {mgr, s2});
        strFree(s2);
        sharedRelease(text);
    }
    // a swallowing touch listener with a "began -> true" handler ($_4); it is never added to the dispatcher
    {
        uint32_t listener = call(EventListenerTouchOneByOne_create, {});
        call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
        Frame f;
        uint32_t began = newFunction(f, VT_TouchBegan, {});
        call(Function_touch_swap, {began, listener + 0x50});
        functionDestroy(began);
    }
    uint32_t rootNode = call(Node_create, {});
    s->root = rootNode;
    vcall(self, node::addChild, {self, rootNode});
    uint32_t widget = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), STR_ShopDrawJson});
    vcall(widget, node::setAnchorPoint, {widget, VEC2_ANCHOR_MIDDLE});
    Frame f;
    uint32_t origin = f.alloc(8);
    call(fn::Director_getVisibleOrigin, {origin, director()});
    uint32_t size = f.alloc(8);
    call(fn::Director_getVisibleSize, {size, director()});
    float x = ldf(origin) + ldf(size) * 0.5f;
    float y = ldf(origin + 4) + ldf(size + 4) * 0.5f;
    {
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits(y)});
        vcall(widget, node::setPosition, {widget, pos});
        call(Vec2_dtor, {pos});
    }
    addChildNamed(s->root, widget, 0, STR_ShopDraw_All, 12);
    initShopTop(self, widget);
    initShopTopBottomButton(self, widget);
    vcall(self, VSLOT_IN_ANIMATION, {self});
    call(Vec2_dtor, {origin});
    return 1;
}
RW_FUNCTION(0x68BDB8, "SceneShopTop::init", SceneShopTop::init);

// The draw list: a DrawScrollView filling Scroll_Area; choosing a draw runs $_6.
void SceneShopTop::initShopTop(Addr self, Addr widget) {
    Self *s = rw::host_ptr<Self>(self);
    uint32_t area = findNode(widget, STR_Scroll_Area, 11);
    vcall(area, VSLOT_LAYOUT_0x29C, {area, 0});
    uint32_t scroll = call(DrawScrollView_create, {});
    s->scrollView = scroll;
    vcall(scroll, node::setContentSize, {scroll, vcall(area, node::getContentSize, {area})});
    call(DrawScrollView_setCanTouch, {s->scrollView, 0});
    {
        Frame f;
        uint32_t onOpen = newFunction(f, VT_OpenDraw, {self});
        call(Function_vv_swap, {onOpen, s->scrollView + SCROLL_ON_OPEN_DRAW});
        functionDestroy(onOpen);
    }
    vcall(area, node::addChild, {area, s->scrollView});
}
RW_FUNCTION(0x68C1B0, "SceneShopTop::initShopTop", SceneShopTop::initShopTop);

// The bottom buttons: jewels (with a dummy image button over its icon), sell, buy one, buy three.
void SceneShopTop::initShopTopBottomButton(Addr self, Addr widget) {
    uint32_t jewelArea = findNode(widget, STR_Button_Jewel, 12);
    uint32_t icon = call(ImageView_create, {});
    {
        float w = ldf(vcall(jewelArea, node::getContentSize, {jewelArea}));
        float h = ldf(vcall(jewelArea, node::getContentSize, {jewelArea}) + 4);
        Frame f;
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, fbits(w), fbits(h)});
        vcall(icon, node::setPosition, {icon, pos});
        call(Vec2_dtor, {pos});
    }
    vcall(icon, node::setAnchorPoint, {icon, VEC2_ANCHOR_BOTTOM_LEFT});
    vcall(jewelArea, node::addChildZ, {jewelArea, icon, 0});
    uint32_t dummy = imageButton(STR_But_Spe01, 9);
    call(ImageButton_setTargetImageView, {dummy, icon});
    addChildNamed(jewelArea, dummy, 2, STR_ImageButton_JewelDummy, 0x16);
    st8(dummy + IMAGEBUTTON_ENABLED, 1);
    vcall(icon, node::setOpacity, {icon, 0});
    uint32_t jewelButton = imageButton(STR_But_Spe01, 9);
    call(ImageButton_setTargetImageView, {jewelButton, jewelArea});
    addChildNamed(jewelArea, jewelButton, 3, STR_ImageButton_Jewel, 0x11);
    Frame f;
    uint32_t refresh = f.alloc(12);                     // the $_9 lambda object, called in place below
    st32(refresh, jewelArea);
    st32(refresh + 4, jewelButton);
    st32(refresh + 8, self);
    addButtonTouch(dummy, VT_TapJewelDummy, {self});
    {
        Frame g;
        uint32_t onTap = g.alloc(20);
        st32(onTap + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_TapJewel);
            st32(heap + 4, self);
            st32(heap + 8, self);
            st32(heap + 12, jewelArea);
            st32(heap + 16, jewelButton);
            st32(heap + 20, self);
        }
        st32(onTap + 16, heap);
        call(ImageButton_addTouchEventListener, {jewelButton, onTap});
        functionDestroy(onTap);
    }
    refreshJewelButton(refresh);

    uint32_t button;
    uint32_t sellArea = makeButton(widget, STR_Button_Sell2, 12, STR_But03, 5, STR_ImageButton_Sell2, 0x11,
                                   VT_TapSell, self, &button);
    titleLabel(sellArea, STR_Txt_Buy1, 8, TEXT_SELL);

    uint32_t buy1Area = makeButton(widget, STR_Button_Buy1, 11, STR_But01, 5, STR_ImageButton_Buy1, 0x10,
                                   VT_TapBuy1, self, &button);
    titleLabel(buy1Area, STR_Txt_Buy1, 8, TEXT_BUY1);
    uint32_t state = ld32(call(fn::APIManager_getInstance, {}) + APIMANAGER_GACHA_STATE);
    if (state == 0 || (ld8(state + 0x34) & 0x40) == 0) vcall(button, node::setVisible, {button, 0});

    uint32_t buy3Area = makeButton(widget, STR_Button_Buy3, 11, STR_But01, 5, STR_ImageButton_Buy3, 0x10,
                                   VT_TapBuy3, self, &button);
    titleLabel(buy3Area, STR_Txt_Buy3, 8, TEXT_BUY3);
}
RW_FUNCTION(0x68C2E4, "SceneShopTop::initShopTopBottomButton", SceneShopTop::initShopTopBottomButton);

void SceneShopTop::update(Addr, float) {}
RW_FUNCTION(0x68CB78, "SceneShopTop::update", SceneShopTop::update);

// ------------------------------------------------------------------------------------------------ animations
// The layout slides in from the left; $_5 runs at the end.
void SceneShopTop::runInAnimation(Addr self) {
    uint32_t all = findNode(rw::host_ptr<Self>(self)->root, STR_ShopDraw_All, 12);
    Frame f;
    uint32_t done = newFunction(f, VT_InAnimationDone, {self});
    uint32_t action = call(SceneUtil_createLeftInAnimation, {all, 0, 0, done});
    call(fn::Node_runAction, {all, call(EaseSineOut_create, {action})});
    functionDestroy(done);
}
RW_FUNCTION(0x68CB7C, "SceneShopTop::runInAnimation", SceneShopTop::runInAnimation);

void SceneShopTop::runInAnimationThunk(Addr inOut) { runInAnimation(inOut - INOUT_OFFSET); }
RW_FUNCTION(0x68CC84, "SceneShopTop::runInAnimation (thunk)", SceneShopTop::runInAnimationThunk);

// Lock the buttons and the list, slide out to the left; `done` runs at the end.
void SceneShopTop::runOutAnimationMain(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    s->canTouch = 0;
    call(DrawScrollView_setCanTouch, {s->scrollView, 0});
    uint32_t all = findNode(s->root, STR_ShopDraw_All, 12);
    Frame f;
    uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    uint32_t action = call(SceneUtil_createLeftOutAnimation, {all, 0, 0, copy});
    call(fn::Node_runAction, {all, call(EaseSineOut_create, {action})});
    functionDestroy(copy);
}
RW_FUNCTION(0x68CC94, "SceneShopTop::runOutAnimationMain", SceneShopTop::runOutAnimationMain);

void SceneShopTop::runOutAnimationMainThunk(Addr inOut, Addr done) {
    runOutAnimationMain(inOut - INOUT_OFFSET, done);
}
RW_FUNCTION(0x68CDAC, "SceneShopTop::runOutAnimationMain (thunk)", SceneShopTop::runOutAnimationMainThunk);

// Back (once): slide out, then My Page ($_13).
void SceneShopTop::backScene(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->canBack == 0) return;
    s->canBack = 0;
    Frame f;
    uint32_t next = newFunction(f, VT_BackToMyPage, {});
    call(InOut_runOutAnimation, {self + INOUT_OFFSET, next});
    functionDestroy(next);
}
RW_FUNCTION(0x68D000, "SceneShopTop::backScene", SceneShopTop::backScene);
