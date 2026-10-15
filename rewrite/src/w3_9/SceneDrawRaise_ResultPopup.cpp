// SceneDrawRaise::openDrawResultPopup: the result popup of one drawn medal (MedalInfoFlavorScene): medal info,
// a "tap the screen" animation and a full-screen button that closes it ($_8 -> $_7 lambda) and then runs `done`.
// The original never reads `this` (callers may pass null).
//
// The original frame (0x210 bytes) is kept: getMedalData and SceneUtil::updateMedalInfo take stack arguments and
// the medal record's word at +0x2C is never initialised.
#include "SceneDrawRaise.hpp"

#include "../scene_main/FunctionThunks.hpp"
#include "core/SafeBuffer.hpp"

using namespace scene;

namespace {
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_updateMedalInfo = 0x462195;   // (node, shared_ptr medal, bool, vector<Skill>*,
                                                          //  [sp] shared_ptr other, [sp+4] bool)
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;   // (node, std::function<void()> by value)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ImageButton_create = 0x531239;
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t VEC2_ZERO = 0x1006950;                 // cocos2d::Vec2::ZERO

constexpr uint32_t VSLOT_GET_ANIMATION = 0x298;           // cocostudio::Armature::getAnimation
constexpr uint32_t VSLOT_ANIMATION_PLAY = 0x68;           // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t VSLOT_ADD_CHILD_Z_NAME = 0xFC;
constexpr uint32_t APIMANAGER_USER = 0x94;                // APIManager: the user's SafeBuffer (byte +0x54: a flag)
constexpr uint32_t USER_FLAG_54 = 0x54;
constexpr uint32_t MEDAL_RECORD_SIZE = 0x48;

constexpr uint32_t VT_CloseTap = 0xF6533C;                // openDrawResultPopup::$_8 [node, layer, shadow, done]
constexpr uint32_t VT_CloseDone = 0xF65368;               // $_7::lambda [layer, shadow, node, done]

constexpr uint32_t STR_MedalInfoFlavorScene = 0xE04B8F;   // "cocostudio/publish/MedalInfoFlavorScene_ver106.json"
constexpr uint32_t STR_Close_Button = 0xDF8D2F;
constexpr uint32_t STR_MedalInfo = 0xDF8D25;
constexpr uint32_t STR_TapScreenJson = 0xDFFCF2;          // "cocostudio/publish/Tap_Screen_Animation.ExportJson"
constexpr uint32_t STR_Tap_Screen_Area = 0xE00317;
constexpr uint32_t STR_Tap_Screen_Animation = 0xDFFB2B;
constexpr uint32_t STR_Animation1 = 0xDFD0AF;
constexpr uint32_t STR_ShopBuy_But = 0xE00327;
constexpr uint32_t STR_CenterButton = 0xDFFB40;

inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
inline void releaseIf(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}

// SceneUtil::searchNodeByName(root, literal) with the temporary string at `tmp`
uint32_t findAt(uint32_t root, uint32_t tmp, uint32_t text, uint32_t len) {
    strInit(tmp, text, len);
    uint32_t node = call(SceneUtil_searchNodeByName, {root, tmp});
    strFree(tmp);
    return node;
}
}  // namespace

namespace draw_raise_result {
void openDrawResultPopup(uint32_t, uint32_t, uint32_t idLo, uint32_t idHi, uint32_t, uint32_t openAnimation,
                         uint32_t done) {
    Frame f;
    const uint32_t sp = f.allocExact(0x210);

    call(fn::AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    vcall(layer, node::addChild, {layer, shadow});
    strInit(sp + 0x118, STR_MedalInfoFlavorScene, 0x33);
    uint32_t popup = call(SceneUtil_createNodeWithSceneFile, {sp + 0x118});
    strFree(sp + 0x118);
    vcall(layer, node::addChild, {layer, popup});
    call(Vec2_ctor, {sp + 0x110, fbits(0.5f), fbits(0.5f)});
    vcall(popup, node::setAnchorPoint, {popup, sp + 0x110});
    call(Vec2_dtor, {sp + 0x110});
    uint32_t setPosition = ld32(ld32(popup) + node::setPosition);
    call(setPosition, {popup, vcall(shadow, node::getPosition, {shadow})});

    uint32_t close = findAt(popup, sp + 0x104, STR_Close_Button, 0xC);
    vcall(close, node::setVisible, {close, 0});
    uint32_t info = findAt(popup, sp + 0xF8, STR_MedalInfo, 9);
    info = findAt(info, sp + 0xEC, STR_MedalInfo, 9);
    vcall(info, node::setAnchorPoint, {info, VEC2_ZERO});
    // centre the medal info: (width / 2, height / 2) of the popup
    uint32_t infoSetPosition = ld32(ld32(info) + node::setPosition);
    float width = ldf(vcall(popup, node::getContentSize, {popup}));
    uint32_t size = vcall(popup, node::getContentSize, {popup});
    float x = width * 0.5f;
    float y = ldf(size + 4) * 0.5f;
    call(Vec2_ctor, {sp + 0xE4, fbits(x), fbits(y)});
    call(infoSetPosition, {info, sp + 0xE4});
    call(Vec2_dtor, {sp + 0xE4});

    // the medal: getMedalData(id, 0, zeroed record) -> updateMedalInfo(popup, medal, user flag, null, null, false)
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t mgr = call(MedalDataManager_getInstance, {});
    const uint32_t local = sp + 0x1A8;
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4)
        if (k != 0x2C) fst32(local + k, 0);
    fst32(sp, 0);
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4) fst32(sp + 8 + k, ld32(local + k));
    rw::call_exact(MedalDataManager_getMedalData, {sp + 0xDC, mgr, idLo, idHi});
    fst32(sp + 0xD4, ld32(sp + 0xDC));                 // a copy of the shared_ptr (passed by value)
    uint32_t ctrl = ld32(sp + 0xE0);
    fst32(sp + 0xD8, ctrl);
    if (ctrl) call(fn::shared_add, {ctrl});
    const SafeBuffer *user = rw::host_ptr<SafeBuffer>(ld32(api + APIMANAGER_USER));
    SafeBuffer::read(user, rw::host_ptr<uint8_t>(sp + 0x127), user->data.get() + USER_FLAG_54, 1, USER_FLAG_54);
    uint8_t userFlag = ld8(sp + 0x127);
    fst32(sp + 0xCC, 0);                               // a null shared_ptr
    fst32(sp + 0xD0, 0);
    fst32(sp + 4, 0);
    fst32(sp, sp + 0xCC);
    rw::call_exact(SceneUtil_updateMedalInfo, {popup, sp + 0xD4, userFlag ? 1u : 0u, 0});
    releaseIf(ld32(sp + 0xD0));
    releaseIf(ld32(sp + 0xD8));

    // captures of the close button: node, layer, shadow, done
    fst32(sp + 0x180, popup);
    fst32(sp + 0x184, layer);
    fst32(sp + 0x188, shadow);
    call(fn::function_vv_copy, {sp + 0x190, done});

    // the "tap the screen" animation
    uint32_t adm = call(ArmatureDataManager_getInstance, {});
    strInit(sp + 0xC0, STR_TapScreenJson, 0x32);
    call(ArmatureDataManager_addArmatureFileInfo, {adm, sp + 0xC0});
    strFree(sp + 0xC0);
    uint32_t area = findAt(popup, sp + 0xB4, STR_Tap_Screen_Area, 0xF);
    strInit(sp + 0xA8, STR_Tap_Screen_Animation, 0x14);
    uint32_t armature = call(Armature_create, {sp + 0xA8});
    strFree(sp + 0xA8);
    vcall(area, node::addChild, {area, armature});
    uint32_t animation = vcall(armature, VSLOT_GET_ANIMATION, {armature});
    uint32_t play = ld32(ld32(animation) + VSLOT_ANIMATION_PLAY);
    strInit(sp + 0x9C, STR_Animation1, 0xA);
    call(play, {animation, sp + 0x9C, 0xFFFFFFFFu, 0xFFFFFFFFu});
    strFree(sp + 0x9C);

    // a button over the whole shadow closes the popup
    strInit(sp + 0x90, STR_ShopBuy_But, 0xB);
    uint32_t button = call(ImageButton_create, {sp + 0x90, 0});
    strFree(sp + 0x90);
    call(ImageButton_setTargetView, {button, shadow, 0x3F800000});
    uint32_t addChild = ld32(ld32(popup) + VSLOT_ADD_CHILD_Z_NAME);
    strInit(sp + 0x84, STR_CenterButton, 0xC);
    call(addChild, {popup, button, 0, sp + 0x84});
    strFree(sp + 0x84);
    fst32(sp + 0x140, ld32(sp + 0x180));
    fst32(sp + 0x144, ld32(sp + 0x184));
    fst32(sp + 0x148, ld32(sp + 0x188));
    functionCopy(sp + 0x150, sp + 0x190);
    fst32(sp + 0x178, 0);
    uint32_t obj = guest::newObject(0x30);
    if (obj) {
        st32(obj, VT_CloseTap);
        st32(obj + 8, ld32(sp + 0x140));
        st32(obj + 0xC, ld32(sp + 0x144));
        st32(obj + 0x10, ld32(sp + 0x148));
        call(fn::function_vv_move, {obj + 0x18, sp + 0x150});
    }
    fst32(sp + 0x178, obj);
    call(ImageButton_addTouchEventListener, {button, sp + 0x168});
    functionDestroy(sp + 0x168);
    functionDestroy(sp + 0x150);

    if (openAnimation == 1) {
        fst32(sp + 0x138, 0);                          // an empty std::function
        uint32_t action = call(SceneUtil_createOpenPopupAnimation, {popup, sp + 0x128});
        call(fn::Node_runAction, {popup, action});
        functionDestroy(sp + 0x128);
    }
    functionDestroy(sp + 0x190);
    releaseIf(ld32(sp + 0xE0));
}
}  // namespace draw_raise_result
RW_FUNCTION(0x68FEB0, "SceneDrawRaise::openDrawResultPopup", draw_raise_result::openDrawResultPopup);

namespace {
// $_8 [node, layer, shadow at +8, done at +0x18]: touch ended -> cancel sound, close animation, then $_7's lambda
void closeTap(uint32_t self, const uint32_t *, const uint32_t *type) {
    if (*type != 2) return;
    call(AudioPlaySystemSECancel, {});
    Frame f;
    uint32_t popup = ld32(self + 8);
    uint32_t caps = f.alloc(0x48);                     // [layer, shadow, node] + done copy at +0x10
    fst32(caps, ld32(self + 0xC));
    fst32(caps + 4, ld32(self + 0x10));
    fst32(caps + 8, popup);
    call(fn::function_vv_copy, {caps + 0x10, self + 0x18});
    uint32_t cb = caps + 0x28;                         // std::function<void()> (callable pointer at +0x10)
    fst32(cb + 0x10, 0);
    uint32_t obj = guest::newObject(0x30);
    if (obj) {
        st32(obj, VT_CloseDone);
        st32(obj + 8, ld32(caps));
        st32(obj + 0xC, ld32(caps + 4));
        st32(obj + 0x10, ld32(caps + 8));
        call(fn::function_vv_move, {obj + 0x18, caps + 0x10});
    }
    fst32(cb + 0x10, obj);
    uint32_t action = call(SceneUtil_createClosePopupAnimation, {popup, cb});
    call(fn::Node_runAction, {popup, action});
    functionDestroy(cb);
    functionDestroy(caps + 0x10);
}

// $_7's lambda [layer, shadow, node at +8, done at +0x18]: remove the shadow and the popup, run `done`
void closeDone(uint32_t self) {
    uint32_t layer = ld32(self + 8);
    vcall(layer, node::removeChild, {layer, ld32(self + 0xC), 1});
    layer = ld32(self + 8);
    vcall(layer, node::removeChild, {layer, ld32(self + 0x10), 1});
    if (ld32(self + 0x28)) call(fn::function_vv_call, {self + 0x18});
}

using CloseTapFunc = thunk::LambdaFn<VT_CloseTap, 0x30, 0xF9D3BC, 0xE73E10, 8, 0x18, fn::function_vv_copy,
                                     thunk::cap(8, 4), thunk::cap(0xC, 4), thunk::cap(0x10, 4)>;
using CloseDoneFunc = thunk::LambdaFn<VT_CloseDone, 0x30, 0xF9D3D0, 0xE73EF0, 8, 0x18, fn::function_vv_copy,
                                      thunk::cap(8, 4), thunk::cap(0xC, 4), thunk::cap(0x10, 4)>;
}  // namespace

RW_FUNCTION(0x6944C4, "SceneDrawRaise::openDrawResultPopup::$_8::operator()", closeTap);
RW_FUNCTION(0x694360, "SceneDrawRaise::openDrawResultPopup::$_8 ~__func()", CloseTapFunc::destructor);
RW_FUNCTION(0x69439C, "SceneDrawRaise::openDrawResultPopup::$_8 ~__func() (deleting)", CloseTapFunc::destructorDelete);
RW_FUNCTION(0x6943DC, "SceneDrawRaise::openDrawResultPopup::$_8 __clone()", CloseTapFunc::clone);
RW_FUNCTION(0x694438, "SceneDrawRaise::openDrawResultPopup::$_8 __clone(__base*)", CloseTapFunc::cloneInto);
RW_FUNCTION(0x694478, "SceneDrawRaise::openDrawResultPopup::$_8 destroy()", CloseTapFunc::destroy);
RW_FUNCTION(0x694498, "SceneDrawRaise::openDrawResultPopup::$_8 destroy_deallocate()", CloseTapFunc::destroyDeallocate);
RW_FUNCTION(0x6945F8, "SceneDrawRaise::openDrawResultPopup::$_8 target(const type_info&)", CloseTapFunc::target);
RW_FUNCTION(0x69461C, "SceneDrawRaise::openDrawResultPopup::$_8 target_type()", CloseTapFunc::targetType);

RW_FUNCTION(0x694794, "SceneDrawRaise::openDrawResultPopup::$_7::lambda::operator()", closeDone);
RW_FUNCTION(0x694630, "SceneDrawRaise::openDrawResultPopup::$_7::lambda ~__func()", CloseDoneFunc::destructor);
RW_FUNCTION(0x69466C, "SceneDrawRaise::openDrawResultPopup::$_7::lambda ~__func() (deleting)", CloseDoneFunc::destructorDelete);
RW_FUNCTION(0x6946AC, "SceneDrawRaise::openDrawResultPopup::$_7::lambda __clone()", CloseDoneFunc::clone);
RW_FUNCTION(0x694708, "SceneDrawRaise::openDrawResultPopup::$_7::lambda __clone(__base*)", CloseDoneFunc::cloneInto);
RW_FUNCTION(0x694748, "SceneDrawRaise::openDrawResultPopup::$_7::lambda destroy()", CloseDoneFunc::destroy);
RW_FUNCTION(0x694768, "SceneDrawRaise::openDrawResultPopup::$_7::lambda destroy_deallocate()", CloseDoneFunc::destroyDeallocate);
RW_FUNCTION(0x6947D0, "SceneDrawRaise::openDrawResultPopup::$_7::lambda target(const type_info&)", CloseDoneFunc::target);
RW_FUNCTION(0x6947F4, "SceneDrawRaise::openDrawResultPopup::$_7::lambda target_type()", CloseDoneFunc::targetType);
