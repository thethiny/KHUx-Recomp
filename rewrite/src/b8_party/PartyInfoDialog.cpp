// PartyInfoDialog (batch #8): the party info popups - a party's info (open / createLayout), the same with a Request
// button (openRequest / createRequestLayout) and the join conditions alert (openAlertRequest / createAlertLayout:
// which conditions the player meets, a circle or a cross per plate); close and the small button / answer lambdas.
// The lambdas' bodies and thunks are in battleui/RaidBossLwfDialog_PartyInfo.cpp and scene_social/thunks.
//
// PartyInfoDialog (0x298, a cocos2d::Node): +0x270 the layout root, +0x274 the parent it was opened on, +0x278 the
// shadow sprite, +0x27C request sent, +0x280 std::function<void(bool)> done.
// The party (APIManager +0x1E4): +0xC rank, +0x10 name, +0x1C play style, +0x20 join type, +0x24 message,
// +0x30 members, +0x7C the leader's name (C string), +0x158 the leader's avatar coordinate.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_ctor = 0x892531, Node_init = 0x892B69, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;
constexpr uint32_t FunctionVB_assign = 0x490109;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625, SceneUtil_createPartyRankImagePath = 0x46F08D;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t SM_openPopup = 0x61524D, SM_enableLoadingIndicator = 0x60D169, SM_updateHeader = 0x611169;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t APIManager_executePartyGet = 0x4C69BD, APIManager_executePaymentInfo = 0x4CE74D;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t Text_setTextHorizontalAlignment = 0x83A65F;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t String_assign = 0x48D811;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105, Avatar_create = 0x5C5883, Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D, SafeBuffer_read = 0x45A269;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t PartyInfoDialog_close = 0x596081, PartyInfoDialog_createAlertLayout = 0x594D61;
constexpr uint32_t GOT_VTABLE = 0xFD66C8;
}  // namespace g

constexpr uint32_t SIZE = 0x298;
constexpr uint32_t F_ROOT = 0x270, F_PARENT = 0x274, F_SHADOW = 0x278, F_SENT = 0x27C, F_DONE = 0x280;
constexpr uint32_t API_PARTY = 0x1E4, API_USER = 0x84, API_PAYMENT = 0x1D4, API_OFFLINE = 0x14;
constexpr uint32_t API_PARTY_STATE = 0x210, API_INVITES = 0x228;

constexpr uint32_t S_InfoJson = 0xE0002C, S_RequestJson = 0xE000CE, S_AlertJson = 0xE00102;
constexpr uint32_t S_CloseButton = 0xDF8D2F, S_ButtonRequest = 0xDFDACD, S_Txt = 0xDF94D1;
constexpr uint32_t S_TxtName = 0xDFDA10, S_RankIcon = 0xDFDA07, S_TxtLeader = 0xE00052, S_TxtLeaderLabel = 0xE0005D;
constexpr uint32_t S_TxtMessageLabel = 0xDFDA19, S_TxtMembers = 0xE0006E, S_TxtMembersLabel = 0xE0007A;
constexpr uint32_t S_MembersFormat = 0xDF8F35, S_TxtPlayStyle = 0xE0008C, S_TxtPlayStyleLabel = 0xE0009A;
constexpr uint32_t S_TxtJoinType = 0xE000AE, S_TxtJoinTypeLabel = 0xE000BB, S_Avatar = 0xDF8F56;
constexpr uint32_t S_AvatarNode = 0xDFE76A, S_Empty = 0xDF8A1A;
constexpr uint32_t S_Alert5 = 0xE00137, S_Wording[7] = {0, 0xDFF14C, 0xDF863B, 0xE0014B, 0xE00158, 0xE00165, 0xE0013E};
constexpr uint32_t S_PlateBlue[5] = {0xE00172, 0xE001A2, 0xE001D2, 0xE00202, 0xE00232};
constexpr uint32_t S_PlateRed[5] = {0xE0017E, 0xE001AE, 0xE001DE, 0xE0020E, 0xE0023E};
constexpr uint32_t S_IconCircle[5] = {0xE00189, 0xE001B9, 0xE001E9, 0xE00219, 0xE00249};
constexpr uint32_t S_IconCross[5] = {0xE00196, 0xE001C6, 0xE001F6, 0xE00226, 0xE00256};

constexpr int32_t TXT_LEADER = 103800040, TXT_MEMBERS = 103800023, TXT_PLAY_STYLE = 102400003;
constexpr int32_t TXT_JOIN_TYPE = 103800019, TXT_ANY = 103800011, TXT_REQUEST = 103800064;
constexpr int32_t TXT_STYLES[3] = {102400004, 102400005, 102400006};
constexpr int32_t TXT_JOINS[3] = {103800049, 103800021, 103800020};
constexpr uint32_t MAX_MEMBERS = 30;
constexpr uint32_t AVATAR_MOTION = 27;

constexpr uint32_t VT_InfoGot = 0xF5AFB8, VT_RequestGot = 0xF5AE84;       // executePartyGet's answer {dialog, flag}
constexpr uint32_t VT_Closed = 0xF5AC74;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C, V_SET_SCALE_XY = 0x48, V_GET_LOCAL_Z = 0x18;
constexpr uint32_t AVATAR_NODE = 0x29C;                                    // Avatar: its armature node

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }
uint32_t root(uint32_t self) { return ld32(self + F_ROOT); }

// the shadow on the parent (z 100) with the dialog in it, the layout loaded into the shadow, centred, hidden
void openShadowAndLayout(uint32_t self, uint32_t parent, uint32_t done, uint32_t json) {
    st32(self + F_PARENT, parent);
    call(g::FunctionVB_assign, {self + F_DONE, done});
    const uint32_t shadow = SceneUtil::createShadowSprite();
    st32(self + F_SHADOW, shadow);
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(ld32(self + F_SHADOW), node::setOpacity, {ld32(self + F_SHADOW), 0});
    call(fn::Node_runAction, {ld32(self + F_SHADOW), call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(ld32(self + F_SHADOW), node::addChild, {ld32(self + F_SHADOW), self});
    vcall(ld32(self + F_PARENT), node::addChildZ, {ld32(self + F_PARENT), ld32(self + F_SHADOW), 100});
    const uint32_t layout = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    st32(self + F_ROOT, layout);
    vcall(layout, node::setAnchorPoint, {layout, VEC2_ANCHOR_MIDDLE});
    const uint32_t r = root(self);
    const float px = bitsf(vcall(r, node::getPositionX, {r}));
    const float w = ldf(vcall(root(self), node::getContentSize, {root(self)}));
    const float side = bitsf(call(g::SceneUtil_getSideMargin, {}));
    const float py = bitsf(vcall(root(self), node::getPositionY, {root(self)}));
    const float h = ldf(vcall(root(self), node::getContentSize, {root(self)}) + 4);
    const float x = side + (px + w * 0.5f);
    const float y0 = py + h * 0.5f;
    const float y = bitsf(call(g::SceneUtil_getSideHeightMargin, {})) + y0;
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(g::Vec2_ctor, {pos, fbits(x), fbits(y)});
    vcall(r, node::setPosition, {r, pos});
    call(g::Vec2_dtor, {pos});
    vcall(ld32(self + F_SHADOW), node::addChild, {ld32(self + F_SHADOW), root(self)});
    setVisible(root(self), 0);
}

// open / openRequest(parent, party id, done, flag): the layout, then /party/get with {dialog, flag}
uint32_t openWith(uint32_t self, uint32_t parent, uint32_t lo, uint32_t hi, uint32_t done, uint32_t flag,
                  uint32_t json, uint32_t vt) {
    if (ld32(self + F_PARENT)) return 0;
    openShadowAndLayout(self, parent, done, json);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self, flag});
    call(g::APIManager_executePartyGet, {api, fo, lo, hi, fo});
    functionDestroy(fo);
    return 1;
}
uint32_t open(uint32_t self, uint32_t parent, uint32_t lo, uint32_t hi, uint32_t done, uint32_t flag) {
    return openWith(self, parent, lo, hi, done, flag, S_InfoJson, VT_InfoGot);
}
uint32_t openRequest(uint32_t self, uint32_t parent, uint32_t lo, uint32_t hi, uint32_t done, uint32_t flag) {
    return openWith(self, parent, lo, hi, done, flag, S_RequestJson, VT_RequestGot);
}
// openAlertRequest(parent, done, flag): the layout and the alert at once
uint32_t openAlertRequest(uint32_t self, uint32_t parent, uint32_t done, uint32_t flag) {
    if (ld32(self + F_PARENT)) return 0;
    openShadowAndLayout(self, parent, done, S_AlertJson);
    call(g::PartyInfoDialog_createAlertLayout, {self, flag & 0xFF});
    return 1;
}

uint32_t create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::AEABI_MEMSET, {me, SIZE, 0});
    call(g::Node_ctor, {me});
    st32(me, ld32(g::GOT_VTABLE) + 8);
    st32(me + F_DONE + 0x10, 0);
    st8(me + F_SENT, 0);
    st32(me + F_SHADOW, 0);
    st32(me + F_PARENT, 0);
    st32(me + F_ROOT, 0);
    if (call(g::Node_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

// the layout shown with its open animation (its end: `vtOpened`)
void showLayout(uint32_t self, uint32_t vtOpened) {
    setVisible(root(self), 1);
    const uint32_t r = root(self);
    Frame f;
    const uint32_t fo = newFunction(f, vtOpened, {});
    const uint32_t anim = call(g::SceneUtil_createOpenPopupAnimation, {r, fo});
    call(fn::Node_runAction, {r, call(g::EaseSineOut_create, {anim})});
    functionDestroy(fo);
}
// SceneManager::openPopup(can close {close}, result {captures})
void openPopup(uint32_t vtCanClose, uint32_t close, uint32_t vtResult, std::initializer_list<uint32_t> captures) {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t canClose = newFunction(f, vtCanClose, {close});
    const uint32_t result = newFunction(f, vtResult, captures);
    call(g::SM_openPopup, {sm, canClose, result, 1});
    functionDestroy(result);
    functionDestroy(canClose);
}
void textIdThenShadow(uint32_t root, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(root, name, len);
    setTextId(text, id);
    SceneUtil::setShadow(text);
}
uint32_t shadowedText(uint32_t root, uint32_t name, uint32_t len) {
    const uint32_t text = findNode(root, name, len);
    SceneUtil::setShadow(text);
    return text;
}
// a label's text: TextData `ids[v - 1]` for v in 1..3, else "any"
void choiceText(uint32_t label, uint32_t value, const int32_t ids[3], uint32_t s) {
    Frame f;
    const uint32_t td = textData(f, value >= 1 && value <= 3 ? ids[value - 1] : TXT_ANY);
    const uint32_t text = textDataStr(td);
    call(g::String_assign, {s, text, host_strlen(text)});
    sharedRelease(td);
    call(fn::Text_setString, {label, s});
}

// the party block of the info / request layouts: name, rank, leader, message, members, play style, join type and
// the leader's avatar (motion 27, scale 1.8) in place of the Avatar node
void fillParty(uint32_t self) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t partyRef = api + API_PARTY;
    {
        const uint32_t name = findNode(root(self), S_TxtName, 8);
        SceneUtil::setShadow(name);
        call(fn::Text_setString, {name, ld32(partyRef) + 0x10});
    }
    {
        const uint32_t icon = findNode(root(self), S_RankIcon, 8);
        Frame f;
        const uint32_t path = f.alloc(12);
        call(g::SceneUtil_createPartyRankImagePath, {path, ld32(ld32(partyRef) + 0xC)});
        const uint32_t copy = newStr(f, strData(path));
        call(g::ImageView_loadTexture, {icon, copy, 0});
        strFree(copy);
        strFree(path);
    }
    textIdThenShadow(root(self), S_TxtLeader, 0xA, TXT_LEADER);
    setTextC(shadowedText(root(self), S_TxtLeaderLabel, 0x10), ld32(partyRef) + 0x7C);
    {
        const uint32_t message = findNode(root(self), S_TxtMessageLabel, 0x11);
        call(g::Text_setTextHorizontalAlignment, {message, 3});
        call(fn::Text_setString, {message, ld32(partyRef) + 0x24});
    }
    textIdThenShadow(root(self), S_TxtMembers, 0xB, TXT_MEMBERS);
    {
        const uint32_t label = shadowedText(root(self), S_TxtMembersLabel, 0x11);
        Frame f;
        const uint32_t buf = stringBuffer(f, {S_MembersFormat, ld32(ld32(partyRef) + 0x30), MAX_MEMBERS});
        setTextC(label, buf);
    }
    textIdThenShadow(root(self), S_TxtPlayStyle, 0xD, TXT_PLAY_STYLE);
    Frame f;
    const uint32_t style = shadowedText(root(self), S_TxtPlayStyleLabel, 0x13);
    const uint32_t styleText = newStr(f, S_Empty, 0);
    choiceText(style, ld32(ld32(partyRef) + 0x1C), TXT_STYLES, styleText);
    textIdThenShadow(root(self), S_TxtJoinType, 0xC, TXT_JOIN_TYPE);
    const uint32_t join = shadowedText(root(self), S_TxtJoinTypeLabel, 0x12);
    const uint32_t joinText = newStr(f, S_Empty, 0);
    choiceText(join, ld32(ld32(partyRef) + 0x20), TXT_JOINS, joinText);
    {
        const uint32_t avatar = findNode(root(self), S_Avatar, 6);
        setVisible(avatar, 0);
        const uint32_t styleBuf = f.alloc(0x40);
        call(g::AvatarUtil_makeStyle, {styleBuf, ld32(partyRef) + 0x158, 0});
        const uint32_t a = call(g::Avatar_create, {styleBuf, 0});
        call(g::Avatar_changeMotion, {a, AVATAR_MOTION});
        vcall(a, V_SET_SCALE_XY, {a, fbits(1.8f), fbits(1.8f)});
        vcall(a, node::setPosition, {a, vcall(avatar, node::getPosition, {avatar})});
        const uint32_t parent = vcall(avatar, node::getParent, {avatar});
        const uint32_t z = vcall(avatar, V_GET_LOCAL_Z, {avatar});
        {
            Frame h;
            const uint32_t tag = newStr(h, S_AvatarNode, 0xA);
            vcall(parent, node::addChildZName, {parent, a, z, tag});
            strFree(tag);
        }
        call(g::Node_unscheduleUpdate, {ld32(a + AVATAR_NODE)});
        call(g::SafeBuffer_dtor, {styleBuf});
    }
    strFree(joinText);
    strFree(styleText);
}

// the info layout: Close, the popup (result: the dialog), the party
uint32_t createLayout(uint32_t self, uint32_t) {
    showLayout(self, 0xF5AF8C);
    const uint32_t close = findNode(root(self), S_CloseButton, 0xC);
    addTouch(close, 0xF5AF60, {});
    openPopup(0xF5AF34, close, 0xF5AF08, {self});
    fillParty(self);
    return 0x634;
}

// the request layout: Request (off while the user already requested: user byte 0x10) and Close, the popup, the party
uint32_t createRequestLayout(uint32_t self, uint32_t) {
    showLayout(self, 0xF5AE58);
    const uint32_t request = findNode(root(self), S_ButtonRequest, 0xE);
    const uint32_t close = findNode(root(self), S_CloseButton, 0xC);
    addTouch(close, 0xF5AE2C, {close, request});
    const uint32_t text = findNode(request, S_Txt, 3);
    SceneUtil::setShadow(text);
    setTextId(text, TXT_REQUEST);
    {
        const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
        Frame f;
        const uint32_t b = f.alloc(4);
        call(g::SafeBuffer_read, {user, b, ld32(user + 0xC) + 0x10, 1, 0x10});
        call(g::SceneUtil_setButtonEnabled, {request, text, ld8(b) ? 0u : 1u, 0});
    }
    addTouch(request, 0xF5ADA8, {self});
    openPopup(0xF5AD7C, close, 0xF5AD50, {self, self});
    fillParty(self);
    return 0x68C;
}

// a "%d"-style wording: TextData `id` formatted with Misc `misc`'s value
void miscWording(uint32_t root, uint32_t name, int32_t id, uint32_t misc) {
    const uint32_t text = shadowedText(root, name, 0xC);
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t m = masterRow(f, fn::Misc_create, misc);
    const int32_t v = safe32(ld32(m), 4);
    const uint32_t buf = stringBuffer(f, {textDataStr(td), uint32_t(v)});
    setTextC(text, buf);
    sharedRelease(m);
    sharedRelease(td);
}
// a condition's plate: met (bit clear): blue + circle, else red + cross
void plate(uint32_t root, uint32_t k, bool met) {
    const uint32_t blue = findNode(root, S_PlateBlue[k], 0xB);
    const uint32_t red = findNode(root, S_PlateRed[k], 0xA);
    const uint32_t circle = findNode(root, S_IconCircle[k], 0xC);
    const uint32_t cross = findNode(root, S_IconCross[k], 0xB);
    setVisible(blue, met ? 1 : 0);
    setVisible(circle, met ? 1 : 0);
    setVisible(red, met ? 0 : 1);
    setVisible(cross, met ? 0 : 1);
}

// the join conditions alert (true: joining a party - the party's flags; false: inviting - the invite flags and a
// fifth condition)
uint32_t createAlertLayout(uint32_t self, uint32_t joining) {
    showLayout(self, 0xF5AD24);
    const uint32_t close = findNode(root(self), S_CloseButton, 0xC);
    addTouch(close, 0xF5ACF8, {close});
    openPopup(0xF5ACCC, close, 0xF5ACA0, {self});
    const uint32_t alert5 = findNode(root(self), S_Alert5, 6);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t flags;
    bool fifthMet = false;
    if (joining == 1) {
        titleLabel(root(self), S_Wording[6], 0xC, 103870001);
        titleLabel(root(self), S_Wording[1], 0xC, 103870002);
        miscWording(root(self), S_Wording[2], 103870003, 800);
        titleLabel(root(self), S_Wording[3], 0xC, 103870004);
        miscWording(root(self), S_Wording[4], 103870005, 803);
        setVisible(alert5, 0);
        flags = ld32(ld32(api + API_PARTY_STATE) + 0x44);
    } else {
        titleLabel(root(self), S_Wording[6], 0xC, 103870101);
        titleLabel(root(self), S_Wording[1], 0xC, 103870102);
        miscWording(root(self), S_Wording[2], 103870103, 800);
        titleLabel(root(self), S_Wording[3], 0xC, 103870104);
        miscWording(root(self), S_Wording[4], 103870105, 801);
        setVisible(alert5, 1);
        titleLabel(root(self), S_Wording[5], 0xC, 103870106);
        flags = ld32(ld32(api + API_INVITES));
        fifthMet = (flags & 0x10) == 0;
    }
    for (uint32_t k = 0; k < 4; k++) plate(root(self), k, (flags & (1u << k)) == 0);
    if (!joining) plate(root(self), 4, fifthMet);
    return 0x12CC;
}

// close: the eased close animation (its end: {dialog}), the shadow fades out
uint32_t close(uint32_t self) {
    const uint32_t r = root(self);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Closed, {self});
    call(fn::Node_runAction, {r, call(g::SceneUtil_createClosePopupAnimation, {r, fo})});
    functionDestroy(fo);
    call(fn::Node_runAction, {ld32(self + F_SHADOW), call(fn::FadeTo_create, {fbits(0.1f), 0})});
    return 0;
}

// Close {dialog}: the header refreshed, closed
uint32_t closeTouched(uint32_t functor) {
    call(g::AudioPlaySystemSECancel, {});
    const uint32_t dialog = ld32(functor + 4);
    call(g::SM_updateHeader, {sceneManager()});
    return call(g::PartyInfoDialog_close, {dialog});
}
// a Close button {button}(sender, type): the popup closed, the button off
uint32_t closeButton(uint32_t functor, uint32_t, uint32_t typep) {
    const uint32_t type = ld32(typep);
    if (type != 2) return type;
    call(g::SM_closeCurrentPopup, {sceneManager(), 0});
    const uint32_t button = ld32(functor + 4);
    return vcall(button, V_SET_TOUCH_ENABLED, {button, 0});
}
// the request popup's result (bool sent) {dialog, dialog}: cancelled: the done callback dropped; then closed
uint32_t requestResult(uint32_t functor, uint32_t okp) {
    if (!ld8(okp)) {
        const uint32_t dialog = ld32(functor + 4);
        call(g::AudioPlaySystemSECancel, {});
        functionDestroy(dialog + F_DONE);
        st32(dialog + F_DONE + 0x10, 0);
    }
    const uint32_t dialog = ld32(functor + 8);
    call(g::SM_updateHeader, {sceneManager()});
    return call(g::PartyInfoDialog_close, {dialog});
}
// Request {dialog}(sender, type): sent; with the payment info loaded: refresh it ({}), else the popup closes
uint32_t requestTouched(uint32_t functor, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    const uint32_t dialog = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    st8(dialog + F_SENT, 1);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_PAYMENT + 4), info = ld32(api + API_PAYMENT);
    if (ctrl) call(fn::shared_add, {ctrl});
    if (info) {
        const uint32_t a = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, 0xF5ADD4, {});
        call(g::APIManager_executePaymentInfo, {a, fo});
        functionDestroy(fo);
    } else {
        call(g::SM_closeCurrentPopup, {sceneManager(), 1});
    }
    if (ctrl) call(fn::shared_release, {ctrl});
    return 0;
}
// the payment info's answer (bool ok): the popup closes, or the error dialog
uint32_t paymentAnswered(uint32_t, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    const uint32_t sm = sceneManager();
    if (ok) {
        call(g::SM_closeCurrentPopup, {sm, 1});
        return 0;
    }
    call(g::SM_enableLoadingIndicator, {sm, 0});
    if (!ld8(call(fn::APIManager_getInstance, {}) + API_OFFLINE)) return 0;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t cb = newFunction(f, 0xF5AE00, {});
    call(g::APIManager_openErrorDialog, {api, cb, 1});
    functionDestroy(cb);
    return 0;
}
}  // namespace

RW_FUNCTION(0x592994, "PartyInfoDialog::create", create);
RW_FUNCTION(0x592A38, "PartyInfoDialog::open", open);
RW_FUNCTION(0x592CA0, "PartyInfoDialog::createLayout", createLayout);
RW_FUNCTION(0x5939E0, "PartyInfoDialog::openRequest", openRequest);
RW_FUNCTION(0x593C48, "PartyInfoDialog::createRequestLayout", createRequestLayout);
RW_FUNCTION(0x594B84, "PartyInfoDialog::openAlertRequest", openAlertRequest);
RW_FUNCTION(0x594D60, "PartyInfoDialog::createAlertLayout", createAlertLayout);
RW_FUNCTION(0x596080, "PartyInfoDialog::close", close);
RW_FUNCTION(0x59632E, "PartyInfoDialog Close operator() (sub_59632E)", closeTouched);
RW_FUNCTION(0x59647A, "PartyInfoDialog close button operator() (sub_59647A)", closeButton);
RW_FUNCTION(0x5965CA, "PartyInfoDialog request popup result operator() (sub_5965CA)", requestResult);
RW_FUNCTION(0x59674C, "PartyInfoDialog Request operator() (sub_59674C)", requestTouched);
RW_FUNCTION(0x5968D4, "PartyInfoDialog payment info answer operator() (sub_5968D4)", paymentAnswered);
