// SceneUnionRegister's invitation popups: an invited new player joined the inviter's party ("Join" - the party and
// the inviter's names) or could not ("NotJoin"), and the jewel notices for an invitation and for a party (misc 115's
// value). Each has one OK; closing runs the callback with true.
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t function_vb_copy = 0x45C84D, function_vb_move = 0x47BC69, function_vb_call = 0x474139;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t GOT_POPUP_FADE = 0xFD65C0;
constexpr uint32_t VT_JoinOk = 0xF6B958, VT_NotJoinOk = 0xF6B900;           // {pad, root, layer, shadow, pad, answer}
constexpr uint32_t VT_InvitationJewelOk = 0xF6B8A8, VT_PartyJewelOk = 0xF6B850;
constexpr uint32_t VT_JoinCloseDone = 0xF6B984, VT_NotJoinCloseDone = 0xF6B92C;   // {layer, shadow, root}
constexpr uint32_t VT_InvitationJewelCloseDone = 0xF6B8D4, VT_PartyJewelCloseDone = 0xF6B87C;
constexpr uint32_t S_JoinJson = 0xE06B9C, S_NotJoinJson = 0xE06BF8, S_InvitationJewelJson = 0xE06C2F;
constexpr uint32_t S_PartyJewelJson = 0xE06C64;
constexpr uint32_t S_ButtonOK = 0xDF8688, S_TxtOK = 0xDF869F;
constexpr uint32_t S_Wording1 = 0xDFF14C, S_Wording2 = 0xDF863B, S_Wording3 = 0xE0014B, S_Wording4 = 0xE00158;
constexpr uint32_t S_Wording5 = 0xE00165, S_TxtParty = 0xDF8DC4, S_TxtPartyLabel = 0xE06BD0, S_TxtUser = 0xE06BE0;
constexpr uint32_t S_TxtUserLabel = 0xE06BE9;
constexpr uint32_t V_setPosition = 0x4C, V_getPosition = 0x54, V_setPositionX = 0x64, V_getPositionX = 0x68;
constexpr uint32_t V_setPositionY = 0x6C, V_getPositionY = 0x70, V_setAnchorPoint = 0x9C, V_addChild = 0xF0;
constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214;
// APIManager +0x5C: the invitation result (a SafeBuffer: +0x14 the inviter's name, 0x21 bytes; +0x18 the party name)
constexpr uint32_t API_INVITATION = 0x5C, INV_PARTY_NAME = 0x18, INV_USER_NAME = 0x14, INV_USER_NAME_LEN = 0x21;
constexpr uint32_t MISC_INVITATION_JEWELS = 115;

void labelOf(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

struct Popup { uint32_t layer, shadow, root, ok, answer; };

// the shadow, the layout shifted by the side margins and centred on the shadow, OK with the closing lambda (a heap
// __func `vt` holding {root, layer, shadow, a copy of the answer}); the caller destroys `answer` (a copy) last
Popup open(Frame &f, uint32_t json, uint32_t vt, uint32_t answer) {
    Popup p;
    call(fn::AudioPlaySystemSEPopUp, {});
    p.layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    p.shadow = call(SceneUtil_createShadowSprite, {0x20C});
    const uint32_t opacity = vcall(p.shadow, V_getOpacity, {p.shadow});
    vcall(p.shadow, V_setOpacity, {p.shadow, 0});
    call(Node_runAction, {p.shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), opacity})});
    vcall(p.layer, V_addChild, {p.layer, p.shadow});
    p.root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    {
        const float x = bitsf(vcall(p.root, V_getPositionX, {p.root}));
        const float m = bitsf(call(SceneUtil_getSideMargin, {fbits(x)}));
        vcall(p.root, V_setPositionX, {p.root, fbits(x + m)});
        const float y = bitsf(vcall(p.root, V_getPositionY, {p.root}));
        const float hm = bitsf(call(SceneUtil_getSideHeightMargin, {fbits(y)}));
        vcall(p.root, V_setPositionY, {p.root, fbits(y + hm)});
    }
    vcall(p.layer, V_addChild, {p.layer, p.root});
    {
        Frame g;
        const uint32_t anchor = g.alloc(8);
        call(Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(p.root, V_setAnchorPoint, {p.root, anchor});
        call(Vec2_dtor, {anchor});
    }
    vcall(p.root, V_setPosition, {p.root, vcall(p.shadow, V_getPosition, {p.shadow})});
    p.answer = f.alloc(20);
    call(function_vb_copy, {p.answer, answer});
    p.ok = findNode(p.root, S_ButtonOK, 9);
    {
        Frame g;
        const uint32_t copy = g.alloc(20);
        functionCopy(copy, p.answer);
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(vt, 0x30);
        if (obj) {
            st32(obj + 8, p.root);
            st32(obj + 0xC, p.layer);
            st32(obj + 0x10, p.shadow);
            call(function_vb_move, {obj + 0x18, copy});
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {p.ok, fo});
        functionDestroy(fo);
        functionDestroy(copy);
    }
    return p;
}

void finish(const Popup &p) {
    {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(Node_runAction, {p.root, call(SceneUtil_createOpenPopupAnimation, {p.root, none})});
        functionDestroy(none);
    }
    functionDestroy(p.answer);
}

// "%d jewels" style text `id` with misc 115's value, in Txt_Wording1
void jewelText(uint32_t root, int32_t id) {
    const uint32_t text = findNode(root, S_Wording1, 0xC);
    SceneUtil::setShadow(text);
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t row = masterRow(f, fn::Misc_create, MISC_INVITATION_JEWELS);
    const uint32_t v = f.alloc(4);
    call(SafeBuffer_read, {ld32(row), v, ld32(ld32(row) + 0xC) + 4, 4, 4});
    setTextC(text, stringBuffer(f, {textDataStr(td), ld32(v)}));
    sharedRelease(row);
    sharedRelease(td);
}

// OK on one of the popups: the window's closing animation, the shadow fading out, the answer with true
uint32_t closed(uint32_t functor, uint32_t typeRef, uint32_t vt) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t root = ld32(functor + 8);
    {
        Frame f;
        const uint32_t done = newFunction(f, vt, {ld32(functor + 0xC), ld32(functor + 0x10), root});
        call(Node_runAction, {root, call(SceneUtil_createClosePopupAnimation, {root, done})});
        functionDestroy(done);
    }
    const uint32_t shadow = ld32(functor + 0x10);
    call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), 0})});
    if (ld32(functor + 0x28)) call(function_vb_call, {functor + 0x18, 1});
    return 0;
}
}  // namespace

namespace invitation_popups {

uint32_t openInvitationResultPop(uint32_t, uint32_t answer) {
    Frame f;
    const Popup p = open(f, S_JoinJson, VT_JoinOk, answer);
    labelOf(p.ok, S_TxtOK, 6, 100100001);
    const uint32_t invitation = ld32(call(fn::APIManager_getInstance, {}) + API_INVITATION);
    labelOf(p.root, S_Wording1, 0xC, 109130007);
    labelOf(p.root, S_Wording2, 0xC, 109130010);
    labelOf(p.root, S_TxtParty, 9, 109130008);
    {
        const uint32_t text = findNode(p.root, S_TxtPartyLabel, 0xF);
        SceneUtil::setShadow(text);
        setTextC(text, strData(invitation + INV_PARTY_NAME));
    }
    labelOf(p.root, S_TxtUser, 8, 109130009);
    {
        const uint32_t text = findNode(p.root, S_TxtUserLabel, 0xE);
        SceneUtil::setShadow(text);
        const uint32_t sb = ld32(invitation);
        Frame g;
        const uint32_t name = g.alloc(0x24);
        call(SafeBuffer_read, {sb, name, ld32(sb + 0xC) + INV_USER_NAME, INV_USER_NAME_LEN, INV_USER_NAME});
        setTextC(text, name);
    }
    finish(p);
    return 0;
}
RW_FUNCTION(0x718740, "SceneUnionRegister::openInvitationResultPop", invitation_popups::openInvitationResultPop);

uint32_t openInvitationNotJoinPop(uint32_t, uint32_t answer) {
    Frame f;
    const Popup p = open(f, S_NotJoinJson, VT_NotJoinOk, answer);
    labelOf(p.ok, S_TxtOK, 6, 100100001);
    labelOf(p.root, S_Wording1, 0xC, 109130013);
    labelOf(p.root, S_Wording2, 0xC, 109130014);
    labelOf(p.root, S_Wording3, 0xC, 109130015);
    labelOf(p.root, S_Wording4, 0xC, 109130016);
    labelOf(p.root, S_Wording5, 0xC, 109130010);
    finish(p);
    return 0;
}
RW_FUNCTION(0x718DFC, "SceneUnionRegister::openInvitationNotJoinPop", invitation_popups::openInvitationNotJoinPop);

uint32_t openInvitationJewelPop(uint32_t, uint32_t answer) {
    Frame f;
    const Popup p = open(f, S_InvitationJewelJson, VT_InvitationJewelOk, answer);
    labelOf(p.ok, S_TxtOK, 6, 100100001);
    jewelText(p.root, 109130012);
    labelOf(p.root, S_Wording2, 0xC, 109130018);
    finish(p);
    return 0;
}
RW_FUNCTION(0x719370, "SceneUnionRegister::openInvitationJewelPop", invitation_popups::openInvitationJewelPop);

uint32_t openPartyJewelPop(uint32_t, uint32_t answer) {
    Frame f;
    const Popup p = open(f, S_PartyJewelJson, VT_PartyJewelOk, answer);
    labelOf(p.ok, S_TxtOK, 6, 100100001);
    jewelText(p.root, 109130011);
    labelOf(p.root, S_Wording2, 0xC, 109130018);
    finish(p);
    return 0;
}
RW_FUNCTION(0x7198D4, "SceneUnionRegister::openPartyJewelPop", invitation_popups::openPartyJewelPop);

uint32_t partyJewelClosed(uint32_t functor, uint32_t, uint32_t typeRef) {
    return closed(functor, typeRef, VT_PartyJewelCloseDone);
}
RW_FUNCTION(0x71A928, "SceneUnionRegister::openPartyJewelPop OK operator() (sub_71A928)", invitation_popups::partyJewelClosed);

uint32_t invitationJewelClosed(uint32_t functor, uint32_t, uint32_t typeRef) {
    return closed(functor, typeRef, VT_InvitationJewelCloseDone);
}
RW_FUNCTION(0x71AC84, "SceneUnionRegister::openInvitationJewelPop OK operator() (sub_71AC84)",
            invitation_popups::invitationJewelClosed);

uint32_t notJoinClosed(uint32_t functor, uint32_t, uint32_t typeRef) {
    return closed(functor, typeRef, VT_NotJoinCloseDone);
}
RW_FUNCTION(0x71AFE0, "SceneUnionRegister::openInvitationNotJoinPop OK operator() (sub_71AFE0)", invitation_popups::notJoinClosed);

uint32_t joinClosed(uint32_t functor, uint32_t, uint32_t typeRef) {
    return closed(functor, typeRef, VT_JoinCloseDone);
}
RW_FUNCTION(0x71B33C, "SceneUnionRegister::openInvitationResultPop OK operator() (sub_71B33C)", invitation_popups::joinClosed);

}  // namespace invitation_popups
