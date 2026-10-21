// SceneAvatarEdit::openCreateCheckPopup: "Begin with this avatar?" after creating the avatar of a new account (the
// avatar shown in the popup). OK sends the avatar with the tutorial progress (phase 5) and goes on to the Union
// choice; Cancel closes the popup.
#include "Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t AvatarActor_create = 0x73FD21, AvatarActor_setStyle = 0x740635;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105, SafeBuffer_dtor = 0x45A13D, Avatar_preloadResources = 0x5C7ED1;
constexpr uint32_t SceneAvatarEdit_getEquipKeybladeID = 0x70715D;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D, APIManager_executeTutorialClear = 0x4C5C89;
constexpr uint32_t function_vb_assign = 0x490109;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t AvatarCreateCheck_close = 0x70B115;
constexpr uint32_t GOT_UNION_REGISTER_FACTORY = 0xFD6968;
constexpr uint32_t VT_CloseTouch = 0xF6A7D0;               // {}
constexpr uint32_t VT_CanClose = 0xF6A7A4;                 // {Close}
constexpr uint32_t VT_Closed = 0xF6A778;                   // {window, layer, shadow, root} (heap)
constexpr uint32_t VT_OkTouch = 0xF6A69C;                  // {window, layer, shadow, root, scene} (heap)
constexpr uint32_t VT_CloseEnd = 0xF6A74C;                 // {layer, shadow, root}
constexpr uint32_t VT_Retry = 0xF6A720;                    // {scene, byte}
constexpr uint32_t VT_Sent = 0xF6A6C8;                     // {byte}
constexpr uint32_t VT_Cleared = 0xF6A6F4;                  // {}
constexpr uint32_t S_Json = 0xE06958, S_Win04 = 0xE06981, S_Wording = 0xDF862F, S_Wording2 = 0xDF863B;
constexpr uint32_t S_ButtonClose = 0xDF8692, S_TxtCancel = 0xDF86A6, S_ButtonOK = 0xDF8688, S_TxtOK = 0xDF869F;
constexpr uint32_t S_Avatar = 0xDF8F56;
constexpr uint32_t V_getLocalZOrder = 0x18, V_setScale = 0x40, V_setPosition = 0x4C, V_getPositionX = 0x68;
constexpr uint32_t V_getPositionY = 0x70, V_setVisible = 0xB0, V_addChild = 0xF0, V_addChildZ = 0xF4;
constexpr uint32_t V_getParent = 0x11C, V_removeFromParent = 0x124;
constexpr uint32_t AVATAR_LWF = 0x288;
// SceneAvatarEdit: the parts sets (vector of 48-byte entries) and the selected one, the gender byte, the avatar
constexpr uint32_t F_SELECTED = 0x2E4, F_GENDER = 0x2ED, F_SETS = 0x300, F_AVATAR_DATA = 0x30C;
// SceneManager / APIManager
constexpr uint32_t SM_RETRY_FLAG = 0x4CC, SM_RETRY_FN = 0x548, API_USER = 0x54, USER_NAME = 0x4C;
constexpr uint32_t PHASE_AVATAR = 5;

void titleLabelId(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}
void retryCallback(uint32_t fo) {
    const uint32_t sm = sceneManager();
    st8(sm + SM_RETRY_FLAG, 1);
    call(function_vb_assign, {sm + SM_RETRY_FN, fo});
}
}  // namespace

namespace avatar_create_check {

uint32_t openCreateCheckPopup(uint32_t self) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = addShadow(layer, true);
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
    centerOnMargins(root);
    vcall(shadow, V_addChild, {shadow, root});
    const uint32_t win = findNode(root, S_Win04, 6);
    titleLabelId(win, S_Wording, 0xB, 102500015);
    titleLabelId(win, S_Wording2, 0xC, 102500022);
    const uint32_t close = findNode(root, S_ButtonClose, 0xC);
    addTouch(close, VT_CloseTouch, {});
    titleLabelId(close, S_TxtCancel, 0xA, 100100002);
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t canClose = newFunction(f, VT_CanClose, {close});
        const uint32_t closed = f.alloc(20);
        const uint32_t obj = heapObject(VT_Closed, 0x14);
        if (obj) {
            st32(obj + 4, win);
            st32(obj + 8, layer);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, root);
        }
        st32(closed + 16, obj);
        call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
        functionDestroy(closed);
        functionDestroy(canClose);
    }
    const uint32_t ok = findNode(root, S_ButtonOK, 9);
    {
        Frame f;
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapObject(VT_OkTouch, 0x18);
        if (obj) {
            st32(obj + 4, win);
            st32(obj + 8, layer);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, root);
            st32(obj + 0x14, self);
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
    }
    titleLabelId(ok, S_TxtOK, 6, 100100001);

    // the avatar in place of the "Avatar" node: the selected parts set, scale 2, 50 points lower
    const uint32_t spot = findNode(win, S_Avatar, 6);
    const uint32_t actor = call(AvatarActor_create, {0, 1});
    const uint32_t kb = call(SceneAvatarEdit_getEquipKeybladeID, {self, ld8(self + F_GENDER)});
    const uint32_t sets = ld32(self + F_SETS);
    const uint32_t count = ((ld32(self + F_SETS + 4) - sets) >> 4) * 0xAAAAAAABu;
    const uint32_t index = ld32(self + F_SELECTED);
    if (count <= index) return call(ThrowOutOfRange, {self + F_SETS});
    {
        Frame f;
        const uint32_t style = f.alloc(0x40);
        call(AvatarUtil_makeStyle, {style, sets + 48 * index, kb});
        call(AvatarActor_setStyle, {actor, style});
        call(SafeBuffer_dtor, {style});
    }
    vcall(actor, V_setVisible, {actor, 1});
    vcall(actor, V_setScale, {actor, 0x40000000});
    {
        const float x = bitsf(vcall(spot, V_getPositionX, {spot}));
        const float y1 = bitsf(vcall(spot, V_getPositionY, {spot}));
        const float y2 = bitsf(vcall(spot, V_getPositionY, {spot}));
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits((y1 + y2 * -0.5f) + -50.0f)});
        vcall(actor, V_setPosition, {actor, pos});
        call(Vec2_dtor, {pos});
    }
    call(Avatar_preloadResources, {ld32(actor + AVATAR_LWF)});
    {
        const uint32_t parent = vcall(spot, V_getParent, {spot});
        vcall(parent, V_addChildZ, {parent, actor, vcall(spot, V_getLocalZOrder, {spot})});
    }
    vcall(spot, V_removeFromParent, {spot});
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        const uint32_t open = call(SceneUtil_createOpenPopupAnimation, {win, none});
        call(Node_runAction, {win, call(EaseSineOut_create, {open})});
        functionDestroy(none);
    }
    return 0;
}
RW_FUNCTION(0x709668, "SceneAvatarEdit::openCreateCheckPopup", avatar_create_check::openCreateCheckPopup);

// sub_70B114 (captures {window, layer, shadow, root}): the window's close animation, the shadow fades out
uint32_t closePopup(uint32_t captures) {
    const uint32_t win = ld32(captures);
    Frame f;
    const uint32_t fo = newFunction(f, VT_CloseEnd, {ld32(captures + 4), ld32(captures + 8), ld32(captures + 0xC)});
    const uint32_t close = call(SceneUtil_createClosePopupAnimation, {win, fo});
    call(Node_runAction, {win, call(EaseSineOut_create, {close})});
    functionDestroy(fo);
    const uint32_t shadow = ld32(captures + 8);
    call(Node_runAction, {shadow, call(FadeTo_create, {0x3DCCCCCD, 0})});
    return 0;
}
RW_FUNCTION(0x70B114, "SceneAvatarEdit::openCreateCheckPopup close (sub_70B114)", avatar_create_check::closePopup);

// the popup closed with "no": Cancel sound and the close animation
uint32_t closed(uint32_t self, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    if (ok) return ok;
    call(AudioPlaySystemSECancel, {});
    return call(AvatarCreateCheck_close, {self + 4});
}
RW_FUNCTION(0x70B74A, "SceneAvatarEdit::openCreateCheckPopup closed operator()", avatar_create_check::closed);

// OK: the popup closes, the avatar is sent (phase 5), then the Union choice
uint32_t okTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t scene = ld32(self + 0x14);
    call(AudioPlaySystemSEDecide, {});
    call(AvatarCreateCheck_close, {self + 4});
    call(SM_closeCurrentPopup, {sceneManager(), 1});
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t retry = newFunction(f, VT_Retry, {scene, 0});
        st8(sm + SM_RETRY_FLAG, 1);
        call(function_vb_assign, {sm + SM_RETRY_FN, retry});
        functionDestroy(retry);
    }
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t avatar = ld32(scene + F_AVATAR_DATA);
        Frame f;
        const uint32_t sent = newFunction(f, VT_Sent, {0});
        call(APIManager_executeTutorialProgress, {api, PHASE_AVATAR, 0, 0, 0, 0, 0, avatar, sent});
        functionDestroy(sent);
    }
    smain::changeSceneLayer(ld32(GOT_UNION_REGISTER_FACTORY), 1);
    return 0;
}
RW_FUNCTION(0x70AF50, "SceneAvatarEdit::openCreateCheckPopup OK operator()", avatar_create_check::okTouched);

// the avatar sent: with a name on the user, the tutorial cleared
uint32_t avatarSent(uint32_t) {
    const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
    if (!strSize(user + USER_NAME)) return 0;
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        retryCallback(none);
        functionDestroy(none);
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_Cleared, {});
    call(APIManager_executeTutorialClear, {api, fo, VT_Cleared});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x70B290, "SceneAvatarEdit::openCreateCheckPopup sent operator()", avatar_create_check::avatarSent);

}  // namespace avatar_create_check
