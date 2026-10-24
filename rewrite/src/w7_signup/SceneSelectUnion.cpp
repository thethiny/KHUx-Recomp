// SceneSelectUnion: the Union transfer (Other menu > Union transfer, after the description popup). The five Union
// emblems (the current one greyed; a press shows the "on" emblem, a release opens "Transfer to <Union>?" with the
// jewel price and the jewels held); OK pays and sends the change (or offers the jewel shop when short), then back
// to MyPage. Shares UnionScene.json with SceneUnionRegister.
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base, Layer_init = rwlib::cocos2d::Layer::init, Scene_create = rwlib::cocos2d::Scene::create, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t SceneSelectUnion_create = 0x5AC5D1, SceneSelectUnion_init = 0x6C8E09;
constexpr uint32_t SceneSelectUnion_runInAnimation = 0x6C9555, SceneSelectUnion_runOutAnimation = 0x6C960D;
constexpr uint32_t SceneSelectUnion_assignEnded = 0x6C94DD;
constexpr uint32_t function_vv_assign = 0x4EB9CD, function_vb_assign = 0x490109;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_move = 0x47F905;
constexpr uint32_t function_vb_copy = 0x45C84D, function_vb_move = 0x47BC69;
constexpr uint32_t FunctionBTE_swap = 0x483E4D, FunctionVTE_swap = 0x4F3699;
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B, SM_setBackButtonType = 0x613E27, SM_showHeader = 0x6131ED;
constexpr uint32_t SM_hideTitleHeader = 0x6130B1, SM_changeTickerMessage = 0x610B39, SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1, SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49, SceneUtil_textToCustomTextView = 0x45BF15;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_createUnionImagePath = 0x46968D;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775, SceneUtil_openOkCancelDialog4Line = 0x45C951;
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;
constexpr uint32_t SceneUtil_createBottomInAnimation = 0x45B2F9, SceneUtil_createBottomOutAnimation = 0x45B469;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create, ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t SafeBuffer_read = 0x45A269, shared_addRef = 0xC8DBB5;
constexpr uint32_t APIManager_executeUnionChange = 0x4CE225, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63, BuyMagicStoneDialog_open = 0x588CAD;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t GOT_VT_SceneSelectUnion = 0xFD66E8, GOT_POPUP_FADE = 0xFD65C0;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_MYPAGE = 0xFD6078;
constexpr uint32_t VT_OnEnter = 0xF67700, VT_OnExit = 0xF676D4, VT_BackKey = 0xF676A8;   // {this}
constexpr uint32_t VT_EmblemBegan = 0xF6767C;     // {current Union, index, emblem, emblem, "on" emblem} (heap 0x18)
constexpr uint32_t VT_EmblemMoved = 0xF67650;     // {emblem, emblem, "on" emblem}
constexpr uint32_t VT_EmblemEnded = 0xF675F8;     // {"on" emblem, this, index}
constexpr uint32_t VT_OutDone = 0xF675CC;         // {pad, done} (heap 0x20)
constexpr uint32_t VT_BackOutDone = 0xF672E0;     // {}
constexpr uint32_t VT_DescOk = 0xF675A0, VT_DescCancel = 0xF67574;          // {}
constexpr uint32_t VT_DescCanClose = 0xF67548;    // {Close}
constexpr uint32_t VT_DescClosed = 0xF674F0;      // {pad, root, pad, answer function, layer, shadow} (heap 0x30)
constexpr uint32_t VT_DescCloseDone = 0xF6751C;   // {pad, answer function, ok, layer, shadow, root} (heap 0x30)
constexpr uint32_t VT_JoinOk = 0xF673BC;          // {Close, OK, root, this} (heap 0x14)
constexpr uint32_t VT_JoinCancel = 0xF67390;      // {Close, OK}
constexpr uint32_t VT_JoinCanClose = 0xF67364;    // {Close}
constexpr uint32_t VT_JoinClosed = 0xF6730C;      // {root, layer, shadow}
constexpr uint32_t VT_JoinCloseDone = 0xF67338;   // {layer, shadow, root}
constexpr uint32_t VT_ShortOfJewels = 0xF673E8;   // {}
constexpr uint32_t VT_ChangeAnswer = 0xF67414;    // {this}
constexpr uint32_t VT_ChangedOk = 0xF67440, VT_ChangeError = 0xF67498;      // {this}
constexpr uint32_t VT_ChangedOut = 0xF6746C, VT_ErrorOut = 0xF674C4;        // {}
constexpr uint32_t S_Json = 0xE05902, S_UnionBase = 0xE05925, S_TxtUnionLabel = 0xE0594D;
constexpr uint32_t S_EmblemOnFmt = 0xE05930, S_EmblemFmt = 0xE05940;
constexpr uint32_t S_DescJson = 0xE0595D, S_JoinJson = 0xE05997;
constexpr uint32_t S_ButtonOK = 0xDF8688, S_ButtonClose = 0xDF8692, S_TxtOK = 0xDF869F, S_TxtCancel = 0xDF86A6;
constexpr uint32_t S_TxtWording1 = 0xDFF14C, S_TxtWording2 = 0xDF863B, S_Txt10Line = 0xE05982;
constexpr uint32_t S_Txt2Line = 0xE059BC, S_TxtCrown = 0xDF8658, S_TxtCrownLabel = 0xDF866B, S_Fmt7d = 0xDF8684;
constexpr uint32_t S_UnionIcon = 0xE059CC;
constexpr uint32_t V_release = 0x04, V_setLocalZOrder = 0x0C, V_setPosition = 0x4C, V_getPosition = 0x54;
constexpr uint32_t V_setAnchorPoint = 0x9C, V_setVisible = 0xB0, V_addChild = 0xF0, V_getOpacity = 0x20C;
constexpr uint32_t V_setOpacity = 0x214, V_setColor = 0x22C, V_setTouchEnabled = 0x29C;
constexpr uint32_t SIZE = 0x298;
// SceneSelectUnion: +0x210 / +0x228 enter / exit callbacks, +0x288 the chosen Union (1..5), +0x28C the layout,
// +0x290 Union_Base, +0x294 can leave (byte)
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_UNION = 0x288, F_ROOT = 0x28C, F_BASE = 0x290;
constexpr uint32_t F_CAN_LEAVE = 0x294;
// APIManager: +0x94 the user data (SafeBuffer, +0x30 the Union), +0xA4 the jewels (SafeBuffer: +0 paid, +4 free)
constexpr uint32_t API_USER_SB = 0x94, API_JEWELS_SB = 0xA4, API_ERROR_SHOWN = 0x14;
constexpr uint32_t USER_UNION = 0x30;
constexpr uint32_t SM_RETRY_FLAG = 0x4CC, SM_RETRY_FN = 0x548;
constexpr uint32_t MISC_UNION_CHANGE = 114;      // value: the jewel price
constexpr uint32_t RET_OPEN_UNION = 0xA0C;

void assignVV(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}

// a decoded 32-bit field of a SafeBuffer, read the way the original does (SafeBuffer::read into a stack slot)
uint32_t sbRead(uint32_t sb, uint32_t index) {
    Frame f;
    const uint32_t v = f.alloc(4);
    call(SafeBuffer_read, {sb, v, ld32(sb + 0xC) + index, 4, index});
    return ld32(v);
}

// misc 114's value: the price of a Union transfer
uint32_t unionChangePrice() {
    Frame f;
    const uint32_t row = masterRow(f, fn::Misc_create, MISC_UNION_CHANGE);
    const uint32_t price = sbRead(ld32(row), 4);
    sharedRelease(row);
    return price;
}

// the popup shadow: transparent, fading in to its opacity, on `layer`
uint32_t popupShadow(uint32_t layer) {
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x83});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), opacity})});
    vcall(layer, V_addChild, {layer, shadow});
    return shadow;
}

// the popup layout on `layer`, centred on the shadow
uint32_t popupRoot(uint32_t layer, uint32_t shadow, uint32_t json) {
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    vcall(layer, V_addChild, {layer, root});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(root, V_setAnchorPoint, {root, anchor});
        call(Vec2_dtor, {anchor});
    }
    vcall(root, V_setPosition, {root, vcall(shadow, V_getPosition, {shadow})});
    return root;
}

void buttonLabel(uint32_t button, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {button, s, uint32_t(id)});
    strFree(s);
}

// a text with a shadow showing text `id`
void shadowText(uint32_t root, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(root, name, len);
    SceneUtil::setShadow(text);
    setTextId(text, id);
}

// back to MyPage: the MyPage data, then the scene change
void toMyPage() {
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneUtil_callPrepareMypageAPIs, {none});
        functionDestroy(none);
    }
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t layerFn = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(GOT_CREATE_MYPAGE)});
    call(SM_changeSceneLayer, {sm, layerFn, 1, 1, 1});
    functionDestroy(layerFn);
}

void runOut(uint32_t self, uint32_t vt) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {});
    call(SceneSelectUnion_runOutAnimation, {self, fo});
    functionDestroy(fo);
}
}  // namespace

namespace scene_select_union {

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneSelectUnion_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x6C8DA0, "SceneSelectUnion::createScene", scene_select_union::createScene);

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    st32(self, ld32(GOT_VT_SceneSelectUnion) + 8);
    st8(self + F_CAN_LEAVE, 0);
    st32(self + F_BASE, 0);
    st32(self + F_ROOT, 0);
    st32(self + F_UNION, 0);
    if (call(SceneSelectUnion_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x5AC5D0, "SceneSelectUnion::create", scene_select_union::create);

uint32_t init(uint32_t self) {
    if (call(Layer_init, {self}) != 1) return 0;
    assignVV(self + F_ON_ENTER, VT_OnEnter, self);
    assignVV(self + F_ON_EXIT, VT_OnExit, self);
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t fo = newFunction(f, VT_BackKey, {self});
        call(SM_setOnBackKeyCallback, {sm, fo});
        functionDestroy(fo);
    }
    st8(self + F_CAN_LEAVE, 1);
    {
        Frame f;
        const uint32_t s = newStr(f, S_Json, 0x22);
        st32(self + F_ROOT, call(SceneUtil_createNodeWithSceneFile, {s}));
        strFree(s);
    }
    vcall(self, V_addChild, {self, ld32(self + F_ROOT)});
    call(SM_setBackButtonType, {sceneManager(), 0});
    call(SM_showHeader, {sceneManager()});
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SM_hideTitleHeader, {sm, none});
        functionDestroy(none);
    }
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t td = textData(f, 100620023);
        const uint32_t s = newStr(f, textDataStr(td));
        call(SM_changeTickerMessage, {sm, s});
        strFree(s);
        sharedRelease(td);
    }
    st32(self + F_BASE, findNode(ld32(self + F_ROOT), S_UnionBase, 0xA));
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_USER_SB + 4);
    const uint32_t sb = ld32(api + API_USER_SB);
    if (ctrl) call(shared_addRef, {ctrl});
    const uint32_t current = sbRead(sb, USER_UNION);
    for (uint32_t i = 1; i < 6; i++) {
        uint32_t on, emblem;
        {
            Frame f;
            const uint32_t name = stringBuffer(f, {S_EmblemOnFmt, i});
            on = findNode(ld32(self + F_ROOT), name, host_strlen(name));
        }
        vcall(on, V_setVisible, {on, 0});
        vcall(on, V_setTouchEnabled, {on, 0});
        {
            Frame f;
            const uint32_t name = stringBuffer(f, {S_EmblemFmt, i});
            emblem = findNode(ld32(self + F_ROOT), name, host_strlen(name));
        }
        if (current == i) {
            Frame f;
            const uint32_t grey = f.alloc(4);
            call(fn::Color3B_ctor, {grey, 0x46, 0x46, 0x46});
            vcall(emblem, V_setColor, {emblem, grey});
        }
        const uint32_t dispatcher = ld32(call(Director_getInstance, {}) + 0x84);
        const uint32_t listener = call(EventListenerTouchOneByOne_create, {});
        call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
        Frame f;
        {
            const uint32_t began = f.alloc(20);
            const uint32_t obj = heapObject(VT_EmblemBegan, 0x18);
            if (obj) {
                st32(obj + 4, current);
                st32(obj + 8, i);
                st32(obj + 0xC, emblem);
                st32(obj + 0x10, emblem);
                st32(obj + 0x14, on);
            }
            st32(began + 16, obj);
            call(FunctionBTE_swap, {began, listener + 0x50});
            functionDestroy(began);
        }
        const uint32_t captures = f.alloc(12);
        st32(captures, on);
        st32(captures + 4, self);
        st32(captures + 8, i);
        {
            const uint32_t moved = newFunction(f, VT_EmblemMoved, {emblem, emblem, on});
            call(FunctionVTE_swap, {moved, listener + 0x68});
            functionDestroy(moved);
        }
        call(SceneSelectUnion_assignEnded, {listener + 0x80, captures});
        call(SceneSelectUnion_assignEnded, {listener + 0x98, captures});
        call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self});
    }
    shadowText(ld32(self + F_ROOT), S_TxtUnionLabel, 0xF, 100620023);
    call(SceneSelectUnion_runInAnimation, {self});
    if (ctrl) call(fn::shared_release, {ctrl});
    return 1;
}
RW_FUNCTION(0x6C8E08, "SceneSelectUnion::init", scene_select_union::init);

// sub_6C94DC: a listener callback = the release lambda {"on" emblem, this, index}
uint32_t assignEnded(uint32_t dst, uint32_t captures) {
    Frame f;
    const uint32_t fo = newFunction(f, VT_EmblemEnded, {ld32(captures), ld32(captures + 4), ld32(captures + 8)});
    call(FunctionVTE_swap, {fo, dst});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6C94DC, "SceneSelectUnion::init ended assign (sub_6C94DC)", scene_select_union::assignEnded);

uint32_t runInAnimation(uint32_t self) {
    vcall(ld32(self + F_BASE), V_setLocalZOrder, {ld32(self + F_BASE), 2});
    const uint32_t base = ld32(self + F_BASE);
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    const uint32_t in = call(SceneUtil_createBottomInAnimation, {base, 0, 0, none});
    call(Node_runAction, {base, call(EaseSineOut_create, {in})});
    functionDestroy(none);
    return 0;
}
RW_FUNCTION(0x6C9554, "SceneSelectUnion::runInAnimation", scene_select_union::runInAnimation);

uint32_t runOutAnimation(uint32_t self, uint32_t done) {
    call(SM_setCanTouchHeader, {sceneManager(), 0});
    const uint32_t base = ld32(self + F_BASE);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vv_copy, {copy, done});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(VT_OutDone, 0x20);
    if (obj) call(function_vv_move, {obj + 8, copy});
    st32(fo + 16, obj);
    const uint32_t out = call(SceneUtil_createBottomOutAnimation, {base, 0, 0, fo});
    call(Node_runAction, {base, call(EaseSineOut_create, {out})});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x6C960C, "SceneSelectUnion::runOutAnimation", scene_select_union::runOutAnimation);

uint32_t backScene(uint32_t self) {
    if (!ld8(self + F_CAN_LEAVE)) return 0;
    st8(self + F_CAN_LEAVE, 0);
    runOut(self, VT_BackOutDone);
    return 0;
}
RW_FUNCTION(0x6CAB10, "SceneSelectUnion::backScene", scene_select_union::backScene);

// the description popup before the Union screen (static): what a transfer is and its jewel price. OK / Cancel
// close it; `answer(OK)` runs once it has closed.
uint32_t openPopUpDescriptionUnion(uint32_t answer) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = popupShadow(layer);
    const uint32_t root = popupRoot(layer, shadow, S_DescJson);
    Frame f;
    // the lambda {root, answer, layer, shadow}, copied twice on the way into the closed callback
    const uint32_t fn1 = f.alloc(20);
    call(function_vb_copy, {fn1, answer});
    const uint32_t close = findNode(root, S_ButtonClose, 0xC);
    const uint32_t ok = findNode(root, S_ButtonOK, 9);
    addTouch(ok, VT_DescOk, {});
    buttonLabel(ok, S_TxtOK, 6, 100100001);
    addTouch(close, VT_DescCancel, {});
    buttonLabel(close, S_TxtCancel, 0xA, 100100002);
    const uint32_t fn2 = f.alloc(20);
    functionCopy(fn2, fn1);
    {
        const uint32_t sm = sceneManager();
        Frame g;
        const uint32_t canClose = newFunction(g, VT_DescCanClose, {close});
        const uint32_t fn3 = g.alloc(20);
        functionCopy(fn3, fn2);
        const uint32_t closed = g.alloc(20);
        const uint32_t obj = heapObject(VT_DescClosed, 0x30);
        if (obj) {
            st32(obj + 8, root);
            call(function_vb_move, {obj + 0x10, fn3});
            st32(obj + 0x28, layer);
            st32(obj + 0x2C, shadow);
        }
        st32(closed + 16, obj);
        call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
        functionDestroy(closed);
        functionDestroy(fn3);
        functionDestroy(canClose);
    }
    shadowText(root, S_TxtWording1, 0xC, 103700002);
    shadowText(root, S_TxtWording2, 0xC, 103700004);
    {
        const uint32_t text = findNode(root, S_Txt10Line, 0x14);
        Frame g;
        const uint32_t name = newStr(g, S_Txt10Line, 0x14);
        const uint32_t view = call(SceneUtil_textToCustomTextView, {text, name});
        strFree(name);
        const uint32_t td = textData(g, 103700003);
        const uint32_t row = masterRow(g, fn::Misc_create, MISC_UNION_CHANGE);
        const uint32_t price = sbRead(ld32(row), 4);
        const uint32_t xml = stringBuffer(g, {textDataStr(td), price});
        const uint32_t s = newStr(g, xml);
        call(fn::CustomTextView_setXml, {view, s});
        strFree(s);
        sharedRelease(row);
        sharedRelease(td);
    }
    {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(Node_runAction, {root, call(EaseSineOut_create, {call(SceneUtil_createOpenPopupAnimation, {root, none})})});
        functionDestroy(none);
    }
    functionDestroy(fn2);
    functionDestroy(fn1);
    return 0;
}
RW_FUNCTION(0x6C9740, "SceneSelectUnion::openPopUpDescriptionUnion", scene_select_union::openPopUpDescriptionUnion);

// the description popup closed (lambda {root, answer, layer, shadow} at `lambda`): the closing animation, whose end
// runs the answer, and the shadow fading out
uint32_t descriptionClosed(uint32_t lambda, uint32_t ok) {
    const uint32_t root = ld32(lambda);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, lambda + 8});
    const uint32_t rest = f.alloc(16);
    st32(rest, ok & 0xFF);
    st32(rest + 4, ld32(lambda + 0x20));
    st32(rest + 8, ld32(lambda + 0x24));
    st32(rest + 0xC, ld32(lambda));
    const uint32_t done = f.alloc(20);
    const uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, VT_DescCloseDone);
        call(function_vb_move, {obj + 8, copy});
        for (uint32_t k = 0; k < 16; k += 4) st32(obj + 0x20 + k, ld32(rest + k));
    }
    st32(done + 16, obj);
    call(Node_runAction, {root, call(EaseSineOut_create, {call(SceneUtil_createClosePopupAnimation, {root, done})})});
    functionDestroy(done);
    functionDestroy(copy);
    const uint32_t shadow = ld32(lambda + 0x24);
    call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), 0})});
    return 0;
}
RW_FUNCTION(0x6CBE9C, "SceneSelectUnion::openPopUpDescriptionUnion closed lambda operator()(bool) (sub_6CBE9C)",
            scene_select_union::descriptionClosed);

// "Transfer to <Union>?": the Union's name and emblem, the price and the jewels held; OK / Cancel
uint32_t openPopUpUnion(uint32_t self) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = popupShadow(layer);
    const uint32_t root = popupRoot(layer, shadow, S_JoinJson);
    const uint32_t ok = findNode(root, S_ButtonOK, 9);
    const uint32_t close = findNode(root, S_ButtonClose, 0xC);
    Frame f;
    {
        Frame g;
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(VT_JoinOk, 0x14);
        if (obj) {
            st32(obj + 4, close);
            st32(obj + 8, ok);
            st32(obj + 0xC, root);
            st32(obj + 0x10, self);
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
    }
    buttonLabel(ok, S_TxtOK, 6, 100100001);
    addTouch(close, VT_JoinCancel, {close, ok});
    buttonLabel(close, S_TxtCancel, 0xA, 100100002);
    // the five Union names
    const uint32_t names = f.alloc(60);
    {
        uint32_t td[5];
        for (uint32_t k = 0; k < 5; k++) {
            td[k] = textData(f, 103710001 + int32_t(k));
            strInitC(names + 12 * k, textDataStr(td[k]));
        }
        for (int k = 4; k >= 0; k--) sharedRelease(td[k]);
    }
    {
        const uint32_t text = findNode(root, S_Txt2Line, 0xF);
        Frame g;
        const uint32_t name = newStr(g, S_Txt2Line, 0xF);
        const uint32_t view = call(SceneUtil_textToCustomTextView, {text, name});
        strFree(name);
        const uint32_t td = textData(g, 103700011);
        const uint32_t row = masterRow(g, fn::Misc_create, MISC_UNION_CHANGE);
        const uint32_t price = sbRead(ld32(row), 4);
        const uint32_t chosen = names + 12 * (ld32(self + F_UNION) - 1);
        const uint32_t xml = stringBuffer(g, {textDataStr(td), price, strData(chosen)});
        const uint32_t s = newStr(g, xml);
        call(fn::CustomTextView_setXml, {view, s});
        strFree(s);
        sharedRelease(row);
        sharedRelease(td);
    }
    shadowText(root, S_TxtCrown, 0x12, 100310002);
    const uint32_t held = findNode(root, S_TxtCrownLabel, 0x18);
    SceneUtil::setShadow(held);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_JEWELS_SB + 4);
    const uint32_t jewels = ld32(api + API_JEWELS_SB);
    if (ctrl) call(shared_addRef, {ctrl});
    {
        const uint32_t free = sbRead(jewels, 4);
        const uint32_t paid = sbRead(jewels, 0);
        Frame g;
        setTextC(held, stringBuffer(g, {S_Fmt7d, paid + free}));
    }
    {
        const uint32_t icon = findNode(root, S_UnionIcon, 9);
        Frame g;
        const uint32_t path = g.alloc(12);
        call(SceneUtil_createUnionImagePath, {path, ld32(self + F_UNION), 1, 0});
        const uint32_t s = newStr(g, strData(path));
        call(ImageView_loadTexture, {icon, s, 0});
        strFree(s);
        strFree(path);
    }
    {
        const uint32_t sm = sceneManager();
        Frame g;
        const uint32_t canClose = newFunction(g, VT_JoinCanClose, {close});
        const uint32_t closed = newFunction(g, VT_JoinClosed, {root, layer, shadow});
        call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
        functionDestroy(closed);
        functionDestroy(canClose);
    }
    {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(Node_runAction, {root, call(EaseSineOut_create, {call(SceneUtil_createOpenPopupAnimation, {root, none})})});
        functionDestroy(none);
    }
    if (ctrl) call(fn::shared_release, {ctrl});
    for (int k = 4; k >= 0; k--) strFree(names + 12 * uint32_t(k));
    return RET_OPEN_UNION;
}
RW_FUNCTION(0x6CA060, "SceneSelectUnion::openPopUpUnion", scene_select_union::openPopUpUnion);

// backScene's out animation done: MyPage
uint32_t backOutDone(uint32_t) {
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        st8(sm + SM_RETRY_FLAG, 1);
        call(function_vb_assign, {sm + SM_RETRY_FN, none});
        functionDestroy(none);
    }
    toMyPage();
    return 0;
}
RW_FUNCTION(0x6CAC0C, "SceneSelectUnion::backScene $_16 operator() (sub_6CAC0C)", scene_select_union::backOutDone);

// the transfer popup closed: the closing animation, the shadow fading out
uint32_t joinClosed(uint32_t functor, uint32_t okRef) {
    if (ld8(okRef)) call(AudioPlaySystemSEDecide, {});
    else call(AudioPlaySystemSECancel, {});
    const uint32_t root = ld32(functor + 4);
    Frame f;
    const uint32_t done = newFunction(f, VT_JoinCloseDone, {ld32(functor + 8), ld32(functor + 0xC), root});
    call(Node_runAction, {root, call(EaseSineOut_create, {call(SceneUtil_createClosePopupAnimation, {root, done})})});
    functionDestroy(done);
    const uint32_t shadow = ld32(functor + 0xC);
    call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), 0})});
    return 0;
}
RW_FUNCTION(0x6CADCC, "SceneSelectUnion::openPopUpUnion closed operator() (sub_6CADCC)", scene_select_union::joinClosed);

// Cancel: the popup closes, both buttons stop taking touches
uint32_t joinCancel(uint32_t functor, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    call(SM_closeCurrentPopup, {sceneManager(), 0});
    vcall(ld32(functor + 4), V_setTouchEnabled, {ld32(functor + 4), 0});
    return vcall(ld32(functor + 8), V_setTouchEnabled, {ld32(functor + 8), 0});
}
RW_FUNCTION(0x6CB0B6, "SceneSelectUnion::openPopUpUnion Cancel operator() (sub_6CB0B6)", scene_select_union::joinCancel);

// OK: enough jewels - the transfer is sent; else "not enough jewels, buy some?"
uint32_t joinOk(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t self = ld32(functor + 0x10);
    call(SM_closeCurrentPopup, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_JEWELS_SB + 4);
    const uint32_t jewels = ld32(api + API_JEWELS_SB);
    if (ctrl) call(shared_addRef, {ctrl});
    const uint32_t free = sbRead(jewels, 4);
    const int32_t held = int32_t(sbRead(jewels, 0) + free);
    const int32_t price = int32_t(unionChangePrice());
    if (held >= price) {
        vcall(ld32(functor + 4), V_setTouchEnabled, {ld32(functor + 4), 0});
        vcall(ld32(functor + 8), V_setTouchEnabled, {ld32(functor + 8), 0});
        findNode(ld32(functor + 0xC), S_ButtonClose, 0xC);
        call(SM_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api2 = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_ChangeAnswer, {self});
        call(APIManager_executeUnionChange, {api2, ld32(self + F_UNION), fo});
        functionDestroy(fo);
    } else {
        Frame f;
        const uint32_t td = textData(f, 102600016);
        const uint32_t s = newStr(f, textDataStr(td));
        const uint32_t fo = newFunction(f, VT_ShortOfJewels, {});
        call(SceneUtil_openOkCancelDialog4Line, {s, fo, 0, 0});
        functionDestroy(fo);
        strFree(s);
        sharedRelease(td);
    }
    if (ctrl) call(fn::shared_release, {ctrl});
    return 0;
}
RW_FUNCTION(0x6CB198, "SceneSelectUnion::openPopUpUnion OK operator() (sub_6CB198)", scene_select_union::joinOk);

// "buy jewels?" answered yes: the jewel shop
uint32_t shortOfJewels(uint32_t, uint32_t yesRef) {
    if (!ld8(yesRef)) return 0;
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(BuyMagicStoneDialog_open, {none, 0});
    functionDestroy(none);
    return 0;
}
RW_FUNCTION(0x6CB4D8, "SceneSelectUnion::openPopUpUnion buy jewels operator() (sub_6CB4D8)", scene_select_union::shortOfJewels);

// the transfer's answer: "transferred" then MyPage; an error dialog (when the request shows one) then MyPage
uint32_t changeAnswer(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t self = ld32(functor + 4);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        Frame f;
        const uint32_t td = textData(f, 103700007);
        const uint32_t s = newStr(f, textDataStr(td));
        const uint32_t fo = newFunction(f, VT_ChangedOk, {self});
        call(SceneUtil_openOkDialog4Line, {s, fo, 0});
        functionDestroy(fo);
        strFree(s);
        sharedRelease(td);
    } else if (ld8(call(fn::APIManager_getInstance, {}) + API_ERROR_SHOWN)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_ChangeError, {self});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    return 0;
}
RW_FUNCTION(0x6CB5F8, "SceneSelectUnion::openPopUpUnion change answer operator() (sub_6CB5F8)", scene_select_union::changeAnswer);

uint32_t changedOk(uint32_t functor) {
    runOut(ld32(functor + 4), VT_ChangedOut);
    return 0;
}
RW_FUNCTION(0x6CB804, "SceneSelectUnion::openPopUpUnion transferred operator() (sub_6CB804)", scene_select_union::changedOk);

uint32_t outToMyPage(uint32_t) {
    toMyPage();
    return 0;
}
RW_FUNCTION(0x6CB91C, "SceneSelectUnion::openPopUpUnion transferred out operator() (sub_6CB91C)", scene_select_union::outToMyPage);
RW_FUNCTION(0x6CBBC0, "SceneSelectUnion::openPopUpUnion error out operator() (sub_6CBBC0)", scene_select_union::outToMyPage);

uint32_t changeError(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    if (call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) return 0;
    runOut(self, VT_ErrorOut);
    return 0;
}
RW_FUNCTION(0x6CBA90, "SceneSelectUnion::openPopUpUnion change error operator() (sub_6CBA90)", scene_select_union::changeError);

}  // namespace scene_select_union
