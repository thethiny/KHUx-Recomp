// ScenePartyCreate: construction, the EditBoxDelegate / IInOutAnimation entry points and the lambda bodies of
// openPopUpCreateParty (which is in ScenePartyCreate_popup.cpp; the std::function thunks are in
// ScenePartyCreate_thunks.cpp).
#include "ScenePartyCreate.hpp"

#include "../SocialCommon.hpp"

using namespace social;
using Self = ScenePartyCreate;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t Node_unscheduleUpdate = rwlib::cocos2d::Node::unscheduleUpdate;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState;
constexpr uint32_t Widget_isTouchEnabled = rwlib::cocos2d::ui::Widget::isTouchEnabled;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t EditBox_getText = rwlib::cocos2d::extension::EditBox::getText;             // cocos2d::extension::EditBox::getText() -> const char*
constexpr uint32_t string_compare = 0x47CBD5;              // std::string::compare(const char*)
constexpr uint32_t function_vv_assign = 0x4EB9CD;
constexpr uint32_t function_vv_call = 0x470E69;
constexpr uint32_t function_vb_assign = 0x490109;          // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SceneManager_setBackButtonType = 0x613E27;
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_executePartyCreate = 0x4CE891;   // (api, name, message, Frequency, AgreeType, cb)
constexpr uint32_t APIManager_executePartyUpdate = 0x4CEAD1;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_createLeftOutAnimation = 0x45B735;   // (node, float, float, function by value)
constexpr uint32_t SceneUtil_editBoxEditingDidEnd = 0x45C61D;     // (Node** ref, Label*, const char*)
constexpr uint32_t SceneUtil_splitSpaceCheck = 0x47898D;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;      // (std::function<void(bool)> by value)
constexpr uint32_t ScenePartyCreate_create = 0x657DF1;
constexpr uint32_t ScenePartyCreate_openPopUpCreateParty = 0x6D2321;
constexpr uint32_t ScenePartyCreate_editBoxEditingDidEnd = 0x6D4131;
constexpr uint32_t setPlayStyle = 0x6D3FB3;                // the play style check box lambda (called directly)
constexpr uint32_t closePopup = 0x6D4911;                  // $_14 (called directly): close the edit popup
constexpr uint32_t makeCreatedCallback = 0x6D5659;         // std::function<void(bool)>($_2::{lambda(bool)#1})
}  // namespace g

constexpr uint32_t createSceneLayer_ScenePartyTop = 0x53E305;
constexpr uint32_t VT_LayerFactory = 0xF7E130;      // __func<Layer*(*)(), ..., Layer*()> + 8
constexpr uint32_t TXTFIELD_MESSAGE = 0xFEF170;     // const char* global: "TextField_Message"

// strings
constexpr uint32_t S_DeckBG = 0xE03AF2;            // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_PartyTopScene = 0xE03B0D;     // "cocostudio/publish/PartyTopScene.json"
constexpr uint32_t S_RightUI = 0xDF85F7;
constexpr uint32_t S_CenterUI = 0xE02FBA;
constexpr uint32_t S_empty = 0xDF8A1A;
constexpr uint32_t S_shadowCreateParty = 0xE05AE8;
constexpr int32_t TEXT_INVALID_TEXT = 103810019;

// lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_init0 = 0xF67E38;            // init $_0 {this}: scheduleUpdate
constexpr uint32_t VT_init1 = 0xF67E0C;            // init $_1 {this}: unscheduleUpdate
constexpr uint32_t VT_closeShadow = 0xF67CAC;      // $_14::{lambda()#1} {layer}: remove the shadow
constexpr uint32_t VT_closeOut = 0xF67C54;         // $_14::{lambda()#2} {this, layer, popup}: after the slide out
constexpr uint32_t VT_closeDone = 0xF67C80;        // $_14::{lambda()#2}::{lambda()#1} {layer, popup}
constexpr uint32_t VT_okNameEmpty = 0xF67DE0;      // $_2::{lambda()#1} {this}
constexpr uint32_t VT_okMessageEmpty = 0xF67DB4;   // $_2::{lambda()#2} {this}
constexpr uint32_t VT_okError = 0xF67D88;          // $_2::{lambda(bool)#1}::{lambda(bool)#1}

constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t V_getName = 0x150;              // Node::getName() (std::string by value)
constexpr uint32_t V_removeChildByName = 0x134;
constexpr uint32_t V_runInAnimation = 0x310;
constexpr uint32_t V_editBoxString = 0x2AC;        // const std::string& of the edit box (party name / message)
constexpr uint32_t SM_NO_TAP_TIME = 0x300;
constexpr uint32_t SM_PARTY_DIRTY = 0x4CC;         // SceneManager byte
constexpr uint32_t SM_PARTY_CALLBACK = 0x548;      // SceneManager std::function<void(bool)>
constexpr uint32_t API_PARTY = 0x84;               // shared_ptr<SafeBuffer>: the player's party (byte +0x10: exists)

// libc++ std::string copy constructor, inline: a long string is re-initialised, a short one copied as is
void strCopy(Addr dst, Addr src) {
    if (strIsLong(src)) {
        call(fn::string_init, {dst, ld32(src + 8), ld32(src + 4)});
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}

// openOkDialog4Line(TextData TEXT_INVALID_TEXT, no callback)
void invalidTextDialog() {
    Frame f;
    Addr text = textData(f, TEXT_INVALID_TEXT);
    Addr s = newStr(f, textDataStr(text));
    withEmptyFunction([&](Addr fo) { call(g::SceneUtil_openOkDialog4Line, {s, fo, 0}); });
    strFree(s);
    sharedRelease(text);
}

// a check box group of three (play style / join type): enable all three, then select `value`'s box and disable it
void enableAll(Addr a, Addr b, Addr c) {
    vcall(a, V_setTouchEnabled, {a, 1});
    vcall(b, V_setTouchEnabled, {b, 1});
    vcall(c, V_setTouchEnabled, {c, 1});
}
void unselect(Addr box) { call(g::CheckBox_setSelectedState, {box, 0}); }
void disable(Addr box) { vcall(box, V_setTouchEnabled, {box, 0}); }
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Addr Self::create() {
    Addr scene = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {scene});
    Self *self = rw::host_ptr<Self>(scene);
    rw::set<uint32_t>(self, 0, VTABLE);
    self->vtableEditBoxDelegate = VTABLE2;
    self->vtableInOut = VTABLE3;
    self->editNode = 0;
    self->editLabel = 0;
    self->editing = 0;
    self->messageText = 0;
    if (call(ld32(VTABLE_BASE + 0x250), {scene}) == 1) {   // init(), through the class vtable
        call(g::Ref_autorelease, {scene});
        return scene;
    }
    vcall(scene, 0x04, {scene});
    return 0;
}
RW_FUNCTION(0x657DF0, "ScenePartyCreate::create", ScenePartyCreate::create);

Addr Self::createScene() {
    Addr scene = call(g::Scene_create, {});
    Addr layer = call(g::ScenePartyCreate_create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x6D1EC8, "ScenePartyCreate::createScene", ScenePartyCreate::createScene);

namespace {
Addr destroyAt(Addr base) {
    st32(base, Self::VTABLE);
    st32(base + 0x288, Self::VTABLE2);
    st32(base + 0x28C, Self::VTABLE3);
    return call(g::Layer_dtor, {base});
}
}  // namespace

Addr Self::destroy(Self *self) { return destroyAt(rw::guest_addr(self)); }
RW_FUNCTION(0x6D1EEC, "ScenePartyCreate::~ScenePartyCreate", ScenePartyCreate::destroy);
Addr Self::destroyThunk648(Addr base) { return destroyAt(base - 0x288); }
RW_FUNCTION(0x6D1F28, "non-virtual thunk (-0x288) to ScenePartyCreate::~ScenePartyCreate", ScenePartyCreate::destroyThunk648);
Addr Self::destroyThunk652(Addr base) { return destroyAt(base - 0x28C); }
RW_FUNCTION(0x6D1F64, "non-virtual thunk (-0x28C) to ScenePartyCreate::~ScenePartyCreate", ScenePartyCreate::destroyThunk652);
void Self::destroyDelete(Self *self) { call(fn::operator_delete, {destroyAt(rw::guest_addr(self))}); }
RW_FUNCTION(0x6D1FA4, "ScenePartyCreate::~ScenePartyCreate (deleting)", ScenePartyCreate::destroyDelete);
void Self::destroyDeleteThunk648(Addr base) { call(fn::operator_delete, {destroyAt(base - 0x288)}); }
RW_FUNCTION(0x6D1FE4, "non-virtual thunk (-0x288) to ScenePartyCreate::~ScenePartyCreate (deleting)", ScenePartyCreate::destroyDeleteThunk648);
void Self::destroyDeleteThunk652(Addr base) { call(fn::operator_delete, {destroyAt(base - 0x28C)}); }
RW_FUNCTION(0x6D2024, "non-virtual thunk (-0x28C) to ScenePartyCreate::~ScenePartyCreate (deleting)", ScenePartyCreate::destroyDeleteThunk652);

uint32_t Self::init(Self *self) {
    Addr me = rw::guest_addr(self);
    if (call(g::Layer_init, {me}) != 1) return 0;
    withLambda(VT_init0, {me}, [&](Addr f) { call(g::function_vv_assign, {me + 0x210, f}); });
    withLambda(VT_init1, {me}, [&](Addr f) { call(g::function_vv_assign, {me + 0x228, f}); });
    call(g::SceneManager_setBackButtonType, {sceneManager(), 2});
    call(g::SceneManager_showHeader, {sceneManager()});
    {
        Addr mgr = sceneManager();
        withStr(S_DeckBG, 26, [&](Addr s) { call(g::SceneManager_changeBackground, {mgr, 0, s}); });
    }
    Addr node_ = withStr(S_PartyTopScene, 37, [&](Addr s) {
        Addr n = call(g::SceneUtil_createNodeWithSceneFile, {s});
        self->sceneNode = n;
        return n;
    });
    node_ = self->sceneNode;
    vcall(me, node::addChild, {me, node_});
    Addr right = findNode(self->sceneNode, S_RightUI, 7);
    vcall(right, node::setVisible, {right, 0});
    Addr center = findNode(self->sceneNode, S_CenterUI, 8);
    vcall(center, node::setVisible, {center, 0});
    call(g::ScenePartyCreate_openPopUpCreateParty, {me});
    vcall(me, V_runInAnimation, {me});
    return 1;
}
RW_FUNCTION(0x6D2068, "ScenePartyCreate::init", ScenePartyCreate::init);

void Self::update(Self *, float) {}
RW_FUNCTION(0x6D3F78, "ScenePartyCreate::update", ScenePartyCreate::update);
void Self::runInAnimation(Self *) {}
RW_FUNCTION(0x6D3F7A, "ScenePartyCreate::runInAnimation", ScenePartyCreate::runInAnimation);
void Self::runInAnimationThunk652(Addr) {}
RW_FUNCTION(0x6D3F7C, "non-virtual thunk (-0x28C) to ScenePartyCreate::runInAnimation", ScenePartyCreate::runInAnimationThunk652);

// leaving: the header buttons stop reacting, then `done`
void Self::runOutAnimationMain(Self *, Addr done) {
    call(g::SceneManager_setCanTouchHeader, {sceneManager(), 0});
    call(g::function_vv_call, {done});
}
RW_FUNCTION(0x6D3F7E, "ScenePartyCreate::runOutAnimationMain", ScenePartyCreate::runOutAnimationMain);
void Self::runOutAnimationMainThunk652(Addr, Addr done) {
    call(g::SceneManager_setCanTouchHeader, {sceneManager(), 0});
    call(g::function_vv_call, {done});
}
RW_FUNCTION(0x6D3F98, "non-virtual thunk (-0x28C) to ScenePartyCreate::runOutAnimationMain", ScenePartyCreate::runOutAnimationMainThunk652);

// An edit box closed: the message box also updates the message text; then SceneUtil's common handling.
void Self::editBoxEditingDidEnd(Self *self, Addr editBox) {
    Frame f;
    Addr name = f.alloc(12);
    vcall(editBox, V_getName, {name, editBox});
    if (call(g::string_compare, {name, ld32(TXTFIELD_MESSAGE)}) == 0 && self->messageText) {
        Addr text = call(g::EditBox_getText, {editBox});
        Addr copy = newStr(f, text);
        bool isLong = strIsLong(copy);
        Addr label = self->messageText;
        Addr s = newStr(f, strData(copy));
        call(g::Text_setString, {label, s});
        strFree(s);
        if (isLong) call(fn::operator_delete, {ld32(copy + 8)});
    }
    Addr labelNode = self->editLabel;
    Addr text = call(g::EditBox_getText, {editBox});
    call(g::SceneUtil_editBoxEditingDidEnd, {rw::guest_addr(self) + 0x29C, labelNode, text});
    strFree(name);
}
RW_FUNCTION(0x6D4130, "ScenePartyCreate::editBoxEditingDidEnd", ScenePartyCreate::editBoxEditingDidEnd);
void Self::editBoxEditingDidEndThunk648(Addr base, Addr editBox) {
    call(g::ScenePartyCreate_editBoxEditingDidEnd, {base - 0x288, editBox});
}
RW_FUNCTION(0x6D42A8, "non-virtual thunk (-0x288) to ScenePartyCreate::editBoxEditingDidEnd", ScenePartyCreate::editBoxEditingDidEndThunk648);

void Self::editBoxEditingDidBegin(Self *, Addr) {}
RW_FUNCTION(0x6D42B8, "ScenePartyCreate::editBoxEditingDidBegin", ScenePartyCreate::editBoxEditingDidBegin);
void Self::editBoxTextChanged(Self *, Addr, Addr) {}
RW_FUNCTION(0x6D42BA, "ScenePartyCreate::editBoxTextChanged", ScenePartyCreate::editBoxTextChanged);
void Self::editBoxReturn(Self *, Addr) {}
RW_FUNCTION(0x6D42BC, "ScenePartyCreate::editBoxReturn", ScenePartyCreate::editBoxReturn);

namespace {
void emptyDelegate(Addr, Addr) {}
void emptyDelegate3(Addr, Addr, Addr) {}
}  // namespace
RW_FUNCTION(0x6D42BE, "non-virtual thunk (-0x288) to ScenePartyCreate::editBoxEditingDidBegin", emptyDelegate);
RW_FUNCTION(0x6D42C0, "non-virtual thunk (-0x288) to ScenePartyCreate::editBoxTextChanged", emptyDelegate3);
RW_FUNCTION(0x6D42C2, "non-virtual thunk (-0x288) to ScenePartyCreate::editBoxReturn", emptyDelegate);

// ------------------------------------------------------------------------------------------------ lambdas
struct ScenePartyCreateLambdas {
    // init $_0 / $_1 {this}
    static void onEnter(Addr func) { call(g::Node_scheduleUpdate, {ld32(func + 4)}); }
    static void onExit(Addr func) { call(g::Node_unscheduleUpdate, {ld32(func + 4)}); }

    // the play style group {this, box3, box2, box1} (called directly): select `style` (3: box3, 2: box2, 1: box1)
    static void setPlayStyle(const uint32_t *group, int32_t style) {
        Self *self = rw::host_ptr<Self>(group[0]);
        if (self->playStyle == style) return;
        self->playStyle = style;
        enableAll(group[1], group[2], group[3]);
        int32_t now = self->playStyle;
        if (now == 3) {
            unselect(group[3]);
            unselect(group[2]);
            disable(group[1]);
            return;
        }
        if (now == 2) {
            unselect(group[3]);
            disable(group[2]);
        } else if (now == 1) {
            disable(group[3]);
            unselect(group[2]);
        } else {
            return;
        }
        unselect(group[1]);
    }
    // the join type group {this, box3, box2, box1} (called directly): select `type` (1: box1, 2: box2, 3: box3)
    static void setJoinType(const uint32_t *group, int32_t type) {
        Self *self = rw::host_ptr<Self>(group[0]);
        if (self->joinType == type) return;
        self->joinType = type;
        enableAll(group[1], group[2], group[3]);
        if (type == 1) {
            unselect(group[1]);
            unselect(group[2]);
            disable(group[3]);
            return;
        }
        if (type == 2) {
            unselect(group[1]);
            disable(group[2]);
        } else if (type == 3) {
            disable(group[1]);
            unselect(group[2]);
        } else {
            return;
        }
        unselect(group[3]);
    }
    // $_7 / $_8 / $_9 {this, box3, box2, box1}(touch): play style 1 / 2 / 3
    static void playStyleTouch(Addr func, const uint32_t *type, int32_t style) {
        if (*type != TOUCH_ENDED) return;
        call(g::AudioPlaySystemSEDecide, {});
        call(g::setPlayStyle, {func + 4, uint32_t(style)});
    }
    static void playStyle1(Addr func, const uint32_t *, const uint32_t *type) { playStyleTouch(func, type, 1); }
    static void playStyle2(Addr func, const uint32_t *, const uint32_t *type) { playStyleTouch(func, type, 2); }
    static void playStyle3(Addr func, const uint32_t *, const uint32_t *type) { playStyleTouch(func, type, 3); }
    // $_11 / $_12 / $_13 {this, box3, box2, box1}(touch): join type 3 / 2 / 1 (setJoinType inlined)
    static void joinTypeTouch(Addr func, const uint32_t *type, int32_t value) {
        if (*type != TOUCH_ENDED) return;
        call(g::AudioPlaySystemSEDecide, {});
        Self *self = rw::host_ptr<Self>(ld32(func + 4));
        if (self->joinType == value) return;
        self->joinType = value;
        Addr a = ld32(func + 8), b = ld32(func + 12), c = ld32(func + 16);
        (void)a;
        (void)b;
        (void)c;
        vcall(ld32(func + 8), V_setTouchEnabled, {ld32(func + 8), 1});
        vcall(ld32(func + 12), V_setTouchEnabled, {ld32(func + 12), 1});
        vcall(ld32(func + 16), V_setTouchEnabled, {ld32(func + 16), 1});
        if (value == 1) {
            unselect(ld32(func + 8));
            unselect(ld32(func + 12));
            disable(ld32(func + 16));
        } else if (value == 2) {
            unselect(ld32(func + 8));
            disable(ld32(func + 12));
            unselect(ld32(func + 16));
        } else {
            disable(ld32(func + 8));
            unselect(ld32(func + 12));
            unselect(ld32(func + 16));
        }
    }
    static void joinType1(Addr func, const uint32_t *, const uint32_t *type) { joinTypeTouch(func, type, 1); }
    static void joinType2(Addr func, const uint32_t *, const uint32_t *type) { joinTypeTouch(func, type, 2); }
    static void joinType3(Addr func, const uint32_t *, const uint32_t *type) { joinTypeTouch(func, type, 3); }

    // $_3 (close button): release closes the popup
    static void closeTouch(Addr, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        call(g::SceneManager_closeCurrentPopup, {sceneManager(), 0});
    }
    // $_4 {closeButton}: can the popup close
    static uint32_t canClose(Addr func) { return call(g::Widget_isTouchEnabled, {ld32(func + 4)}); }
    // $_5 {this, $_14 = layer, shadow, popup, this}(closed by OK?): closed otherwise - cancel sound, close it
    static void popupClosed(Addr func, const uint32_t *okArg) {
        if (uint8_t(*okArg)) return;
        Self *self = rw::host_ptr<Self>(ld32(func + 4));
        call(g::AudioPlaySystemSECancel, {});
        self->editing = 1;
        call(g::closePopup, {func + 8});
    }
    // $_14 {layer, shadow, popup, this} (called directly): fade out the shadow (removed after 0.35 s) and slide the
    // popup out to the left
    static void close(const uint32_t *capture) {
        Addr layer = capture[0];
        Addr callFunc;
        Addr self = capture[3];
        {
            Frame f;
            Addr fo = newFunction(f, VT_closeShadow, {layer});
            callFunc = call(g::CallFunc_create, {fo});
            functionDestroy(fo);
        }
        Addr shadow = capture[1];
        Addr delay = call(g::DelayTime_create, {fbits(0.35f)});
        call(g::Node_runAction, {shadow, call(g::Sequence_create, {delay, callFunc, 0})});
        Addr popup = capture[2];
        withLambda(VT_closeOut, {self, capture[0], popup}, [&](Addr fo) {
            Addr anim = call(g::SceneUtil_createLeftOutAnimation, {popup, 0, 0, fo});
            call(g::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
        });
    }
    // $_14::{lambda()#1} {layer}: remove the shadow
    static void removeShadow(Addr func) {
        Addr layer = ld32(func + 4);
        uint32_t removeByName = ld32(ld32(layer) + V_removeChildByName);
        withStr(S_shadowCreateParty, 17, [&](Addr s) { call(removeByName, {layer, s, 1}); });
    }
    // $_14::{lambda()#2} {this, layer, popup}: the scene's out animation, then remove the popup
    static void slidOut(Addr func) {
        Addr self = ld32(func + 4);
        withLambda(VT_closeDone, {ld32(func + 8), ld32(func + 12)},
                   [&](Addr fo) { call(g::IInOutAnimation_runOutAnimation, {self + 0x28C, fo}); });
    }
    // $_14::{lambda()#2}::{lambda()#1} {layer, popup}: remove the popup, reload the party top and go back to it
    static void backToPartyTop(Addr func) {
        Addr layer = ld32(func + 4);
        vcall(layer, node::removeChild, {layer, ld32(func + 8), 1});
        Addr mgr = sceneManager();
        Frame f;
        Addr empty = newEmptyFunction(f);
        rw::save<uint8_t>(mgr + SM_PARTY_DIRTY, 1);
        call(g::function_vb_assign, {mgr + SM_PARTY_CALLBACK, empty});
        functionDestroy(empty);
        withEmptyFunction([&](Addr fo) { call(g::ScenePartyTop_callPartyTopAPI, {fo}); });
        Addr mgr2 = sceneManager();
        withLambda(VT_LayerFactory, {createSceneLayer_ScenePartyTop},
                   [&](Addr factory) { call(g::SceneManager_changeSceneLayer, {mgr2, factory, 1, 1, 1}); });
    }

    // $_2 {this, layer, shadow, popup, this, nameBox, messageBox}(touch): OK - validate the texts, then create or
    // update the party
    static void okTouch(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        Addr self = ld32(func + 4);
        call(g::AudioPlaySystemSEDecide, {});
        Addr layer = ld32(func + 8), shadow = ld32(func + 12), popup = ld32(func + 16), self2 = ld32(func + 20);
        Frame f;
        Addr name = f.alloc(12);
        Addr message = f.alloc(12);
        {
            Addr box = ld32(func + 24);
            strCopy(name, vcall(box, V_editBoxString, {box}));
        }
        {
            Addr box = ld32(func + 28);
            strCopy(message, vcall(box, V_editBoxString, {box}));
        }
        if (call(g::string_compare, {name, S_empty}) == 0) {
            delayedRetry(self, VT_okNameEmpty);
        } else if (call(g::string_compare, {message, S_empty}) == 0) {
            delayedRetry(self, VT_okMessageEmpty);
        } else if (strSize(name) && call(g::SceneUtil_splitSpaceCheck, {name}) == 0) {
            invalidTextDialog();
        } else if (strSize(message) && call(g::SceneUtil_splitSpaceCheck, {message}) == 0) {
            invalidTextDialog();
        } else {
            Self *s = rw::host_ptr<Self>(self);
            s->editing = 1;
            Addr api = call(g::APIManager_getInstance, {});
            uint32_t ctrl = ld32(api + API_PARTY + 4);
            Addr party = ld32(api + API_PARTY);
            if (ctrl) guest::addRef(ctrl);
            bool update = false;
            if (party) {
                Addr flag = f.alloc(8);
                call(g::SafeBuffer_read, {party, flag + 3, ld32(party + 0xC) + 0x10, 1, 0x10});
                update = ld8(flag + 3) != 0;
            }
            call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
            Addr api2 = call(g::APIManager_getInstance, {});
            Addr nameText = strData(name), messageText = strData(message);
            int32_t agree = s->joinType;
            int32_t frequency = s->playStyle;
            Addr cb = f.alloc(24);
            call(g::makeCreatedCallback, {cb, self, layer, shadow, popup, self2});
            call(update ? g::APIManager_executePartyUpdate : g::APIManager_executePartyCreate,
                 {api2, nameText, messageText, uint32_t(frequency), uint32_t(agree), cb});
            functionDestroy(cb);
            if (ctrl) guest::release(ctrl);
        }
        strFree(message);
        strFree(name);
    }
    // block touches for the scene change time, then run `vtable`'s lambda {this} (an empty text was entered)
    static void delayedRetry(Addr self, uint32_t vtable) {
        Addr mgr = sceneManager();
        call(g::SceneManager_openNoTapScreen, {mgr, ld32(sceneManager() + SM_NO_TAP_TIME)});
        Addr delay = call(g::DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
        withLambda(vtable, {self}, [&](Addr fo) {
            Addr callFunc = call(g::CallFunc_create, {fo});
            call(g::Node_runAction, {self, call(g::Sequence_create, {delay, callFunc, 0})});
        });
    }
    // the std::function<void(bool)> of $_2::{lambda(bool)#1} {this, layer, shadow, popup, this} (an out-of-line
    // constructor: the lambda arrives by value in r1..r3 and on the stack)
    static Addr makeCallback(Addr out, Addr self, Addr layer, Addr shadow, Addr popup, Addr self2) {
        st32(out + 16, 0);
        Addr heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, 0xF67D5C);
            st32(heap + 4, self);
            st32(heap + 8, layer);
            st32(heap + 12, shadow);
            st32(heap + 16, popup);
            st32(heap + 20, self2);
        }
        st32(out + 16, heap);
        return out;
    }
    // $_2::{lambda(bool)#1} {this, $_14...}(ok): created - refresh the header and close; failed - the API error
    // dialog, and unless the request is retried the popup may be used again
    static void created(Addr func, const uint32_t *okArg) {
        uint8_t ok = uint8_t(*okArg);
        Addr self = ld32(func + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            call(g::SceneManager_updateHeader, {sceneManager()});
            call(g::SceneManager_closeCurrentPopup, {sceneManager(), 1});
            call(g::closePopup, {func + 8});
            return;
        }
        if (ld8(call(g::APIManager_getInstance, {}) + 0x14)) {
            Addr api = call(g::APIManager_getInstance, {});
            withLambda(VT_okError, {}, [&](Addr fo) { call(g::APIManager_openErrorDialog, {api, fo, 1}); });
        }
        if (call(g::APIManager_needsRetry, {call(g::APIManager_getInstance, {})}) == 0)
            rw::save<uint8_t>(self + 0x2A4, 0);
    }
    static void nothing(Addr, const uint32_t *) {}
    // $_2::{lambda()#1} / {lambda()#2} {this}: an empty name / message
    static void emptyText(Addr) { invalidTextDialog(); }
};

RW_FUNCTION(0x6D5CDE, "ScenePartyCreate::init $_0 __func::operator()", ScenePartyCreateLambdas::onEnter);
RW_FUNCTION(0x6D5C42, "ScenePartyCreate::init $_1 __func::operator()", ScenePartyCreateLambdas::onExit);
RW_FUNCTION(0x6D3FB2, "ScenePartyCreate::openPopUpCreateParty $_6::operator() (play style)", ScenePartyCreateLambdas::setPlayStyle);
RW_FUNCTION(0x6D4070, "ScenePartyCreate::openPopUpCreateParty $_10::operator() (join type)", ScenePartyCreateLambdas::setJoinType);
RW_FUNCTION(0x6D467A, "ScenePartyCreate::openPopUpCreateParty $_9 __func::operator()", ScenePartyCreateLambdas::playStyle1);
RW_FUNCTION(0x6D473A, "ScenePartyCreate::openPopUpCreateParty $_8 __func::operator()", ScenePartyCreateLambdas::playStyle2);
RW_FUNCTION(0x6D47FA, "ScenePartyCreate::openPopUpCreateParty $_7 __func::operator()", ScenePartyCreateLambdas::playStyle3);
RW_FUNCTION(0x6D4336, "ScenePartyCreate::openPopUpCreateParty $_13 __func::operator()", ScenePartyCreateLambdas::joinType1);
RW_FUNCTION(0x6D444A, "ScenePartyCreate::openPopUpCreateParty $_12 __func::operator()", ScenePartyCreateLambdas::joinType2);
RW_FUNCTION(0x6D4562, "ScenePartyCreate::openPopUpCreateParty $_11 __func::operator()", ScenePartyCreateLambdas::joinType3);
RW_FUNCTION(0x6D4F96, "ScenePartyCreate::openPopUpCreateParty $_3 __func::operator()", ScenePartyCreateLambdas::closeTouch);
RW_FUNCTION(0x6D4F06, "ScenePartyCreate::openPopUpCreateParty $_4 __func::operator()", ScenePartyCreateLambdas::canClose);
RW_FUNCTION(0x6D48B6, "ScenePartyCreate::openPopUpCreateParty $_5 __func::operator()", ScenePartyCreateLambdas::popupClosed);
RW_FUNCTION(0x6D4910, "ScenePartyCreate::openPopUpCreateParty $_14::operator()", ScenePartyCreateLambdas::close);
RW_FUNCTION(0x6D4DC0, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::operator()", ScenePartyCreateLambdas::removeShadow);
RW_FUNCTION(0x6D4AB0, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::operator()", ScenePartyCreateLambdas::slidOut);
RW_FUNCTION(0x6D4BF0, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::operator()", ScenePartyCreateLambdas::backToPartyTop);
RW_FUNCTION(0x6D5060, "ScenePartyCreate::openPopUpCreateParty $_2 __func::operator()", ScenePartyCreateLambdas::okTouch);
RW_FUNCTION(0x6D5658, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ std::function(lambda&&)", ScenePartyCreateLambdas::makeCallback);
RW_FUNCTION(0x6D5720, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::operator()", ScenePartyCreateLambdas::created);
RW_FUNCTION(0x6D58AE, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::operator()", ScenePartyCreateLambdas::nothing);
RW_FUNCTION(0x6D5944, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::operator()", ScenePartyCreateLambdas::emptyText);
RW_FUNCTION(0x6D5AC4, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::operator()", ScenePartyCreateLambdas::emptyText);
