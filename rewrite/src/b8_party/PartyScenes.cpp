// Batch #8: the party scenes' createScene functions and ScenePartyTop's leftover lambdas (copy the party's invite
// text to the clipboard, back out of the screen, the party top reload after an API answer).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t ScenePartyTop_create = 0x53E30D, ScenePartyMember_create = 0x659E6D;
constexpr uint32_t ScenePartyManagement_create = 0x657635;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t BoardCopy_copy = 0x4E2C81;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1, SM_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t PartyTopReset = 0x656B45;                    // sub_656B44
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t FunctionVB_assign = 0x490109;
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_PARTY_TOP = 0xFD6620;
}  // namespace g

constexpr uint32_t VT_TopOut = 0xF62104;
constexpr uint32_t SM_POPUP_FLAG = 0x4CC, SM_POPUP_DONE = 0x548;
constexpr uint32_t F_INOUT = 0x288;
constexpr int32_t TXT_COPY_FORMAT = 104010006, TXT_COPIED = 104010002;
constexpr uint32_t MISC_INVITE_URL = 115;

uint32_t createScene(uint32_t create) {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
uint32_t partyTopCreateScene() { return createScene(g::ScenePartyTop_create); }
uint32_t partyMemberCreateScene() { return createScene(g::ScenePartyMember_create); }
uint32_t partyManagementCreateScene() { return createScene(g::ScenePartyManagement_create); }

// the copy button {-, string code (+4), reset (+0x10)}(sender, type): the invite text (TextData format with the
// code and Misc 115) to the clipboard, the popup closed, "copied"
uint32_t copyInvite(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t fmt = textData(f, TXT_COPY_FORMAT);
    const uint32_t fmtStr = textDataStr(fmt);
    const uint32_t code = strData(self + 4);
    const uint32_t misc = masterRow(f, fn::Misc_create, MISC_INVITE_URL);
    const int32_t value = safe32(ld32(misc), 4);
    const uint32_t buf = stringBuffer(f, {fmtStr, code, uint32_t(value)});
    call(g::BoardCopy_copy, {buf});
    sharedRelease(misc);
    sharedRelease(fmt);
    call(g::SM_closeCurrentPopup, {sceneManager(), 1});
    call(g::PartyTopReset, {self + 0x10});
    const uint32_t msgText = textData(f, TXT_COPIED);
    const uint32_t msg = newStr(f, textDataStr(msgText));
    {
        Frame t;
        const uint32_t none = newEmptyFunction(t);
        call(g::SceneUtil_openOkDialog4Line, {msg, none, 0});
        functionDestroy(none);
    }
    strFree(msg);
    sharedRelease(msgText);
    return 0;
}

// the back button {scene}(sender, type): the out animation
uint32_t backOut(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    const uint32_t scene = ld32(self + 4);
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_TopOut, {});
    call(g::IInOutAnimation_runOutAnimation, {scene + F_INOUT, fo});
    functionDestroy(fo);
    return 0;
}

void reloadPartyTop() {
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(g::ScenePartyTop_callPartyTopAPI, {none});
        functionDestroy(none);
    }
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(g::GOT_CREATE_PARTY_TOP)});
    call(g::SM_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
}
// the popup flag set, its callback cleared, then the party top reloaded
uint32_t reloadAfterPopup() {
    const uint32_t sm = sceneManager();
    st8(sm + SM_POPUP_FLAG, 1);
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(g::FunctionVB_assign, {sm + SM_POPUP_DONE, none});
        functionDestroy(none);
    }
    reloadPartyTop();
    return 0;
}
// an API answer: the party top reloaded unless the request is retried
uint32_t reloadUnlessRetry() {
    if (!call(g::APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) reloadPartyTop();
    return 0;
}
}  // namespace

RW_FUNCTION(0x652A4C, "ScenePartyTop::createScene()", partyTopCreateScene);
RW_FUNCTION(0x6F0A20, "ScenePartyMember::createScene()", partyMemberCreateScene);
RW_FUNCTION(0x6E6554, "ScenePartyManagement::createScene()", partyManagementCreateScene);
RW_FUNCTION(0x656F6C, "ScenePartyTop copy invite (sub_656F6C)", copyInvite);
RW_FUNCTION(0x6577E0, "ScenePartyTop back button (sub_6577E0)", backOut);
RW_FUNCTION(0x658E90, "ScenePartyTop reload after a popup (sub_658E90)", reloadAfterPopup);
RW_FUNCTION(0x659838, "ScenePartyTop reload unless retried (sub_659838)", reloadUnlessRetry);
RW_FUNCTION(0x65A2D8, "ScenePartyTop reload unless retried (sub_65A2D8)", reloadUnlessRetry);
