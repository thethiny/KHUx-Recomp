// SceneDrawRaise::openDrawOmakePopup: the draw bonus ("omake") popup (MedalInfo_DrowBonus). The bonus is a medal
// (draw result type 3: its card, "<name> x<count>"), a keyblade (type 13) or another item (memory icon + label).
// OK ($_14) closes it ($_13's lambda) and runs `done`; the open animation's end ($_15) adds the bonus LWF effect.
// The original never reads `this`.
//
// The original frame (0x6D8 bytes) is kept: getMedalData / createDrawMedal / setItemImageData take stack
// arguments and the medal record's word at +0x2C is never initialised.
#include "SceneDrawRaise.hpp"

#include "../scene_main/FunctionThunks.hpp"
#include "core/SafeBuffer.hpp"

using namespace scene;

namespace {
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;     // (root, const string& name, int textId)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;      // (string& label, string& image, int type, int id,
                                                               //  [sp] bool)
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t VectorU64_copy = 0x468431;
constexpr uint32_t VectorU64_dtor = 0x468499;
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;
constexpr uint32_t AEABI_MEMCPY = 0x40E164;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;

constexpr uint32_t APIMANAGER_DRAW_RESULT = 0x28C;             // shared_ptr<draw result> (object, control block)
constexpr uint32_t RESULT_BONUS_CTRL = 0x14;                   // draw result: a shared_ptr's control block
constexpr uint32_t RESULT_BONUS_TYPE = 0x18;                   // 3 medal, 13 keyblade, else an item
constexpr uint32_t RESULT_BONUS_IDS = 0x1C;                    // vector<u64>
constexpr uint32_t USERMEDAL_MASTER = 0x58;
constexpr uint32_t MEDAL_NAME = 0x10;                          // master::Medal: name (0x41 bytes)
constexpr uint32_t MEDAL_RECORD_SIZE = 0x48;

constexpr int32_t TEXT_OMAKE_TITLE = 0x61D8D69;
constexpr int32_t TEXT_MEDAL_COUNT = 0x61D8D76;                // "%s x%d"-like format
constexpr int32_t TEXT_OK = 0x5F767A1;

constexpr uint32_t VT_OkTap = 0xF65100;                        // openDrawOmakePopup::$_14 [popup, layer, shadow, done]
constexpr uint32_t VT_OkDone = 0xF6512C;                       // $_13::lambda [popup, layer, shadow, done]
constexpr uint32_t VT_Opened = 0xF650D4;                       // openDrawOmakePopup::$_15 [popup]

constexpr uint32_t STR_DrowBonusJson = 0xE04C5F;               // "cocostudio/publish/MedalInfo_DrowBonus_ver106.json"
constexpr uint32_t STR_Txt_Omake = 0xE04C92;
constexpr uint32_t STR_Medal_Area = 0xE00CB0;
constexpr uint32_t STR_Memory = 0xE04C9C;
constexpr uint32_t STR_KB = 0xDFEF39;
constexpr uint32_t STR_Txt_MemoryNum_Label = 0xE04CA3;
constexpr uint32_t STR_Txt_KB_Label = 0xE047CD;
constexpr uint32_t STR_Txt_Medal_Label = 0xDFD5CC;
constexpr uint32_t STR_Panel01 = 0xE04CB7;                     // "Result_Clear_Challenge_Panel01"
constexpr uint32_t STR_Icon_New = 0xDFDD8A;
constexpr uint32_t STR_Button_OK = 0xDF8688;
constexpr uint32_t STR_Txt_OK = 0xDF869F;
constexpr uint32_t STR_Area_Point = 0xDF9FB4;
constexpr uint32_t STR_DrawBonusLwf = 0xE04D94;                // "lwf/raisedraw/demo/Draw_Bonus/Draw_Bonus.lwf"

inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
inline void releaseIf(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}
uint32_t findAt(uint32_t root, uint32_t tmp, uint32_t text, uint32_t len) {
    strInit(tmp, text, len);
    uint32_t node = call(SceneUtil_searchNodeByName, {root, tmp});
    strFree(tmp);
    return node;
}
void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
// a std::string at `tmp` from the characters of string `src`, handed to `use`, then freed
void loadTextureFrom(uint32_t image, uint32_t tmp, uint32_t chars) {
    strInit(tmp, chars, host_strlen(chars));
    call(ImageView_loadTexture, {image, tmp, 0});
    strFree(tmp);
}
void setStringFrom(uint32_t text, uint32_t tmp, uint32_t chars) {
    strInit(tmp, chars, host_strlen(chars));
    call(fn::Text_setString, {text, tmp});
    strFree(tmp);
}
}  // namespace

namespace draw_raise_omake {
void openDrawOmakePopup(uint32_t, uint32_t done) {
    Frame f;
    const uint32_t sp = f.allocExact(0x6D8);

    call(fn::AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    vcall(shadow, node::setOpacity, {shadow, 0});
    vcall(layer, node::addChild, {layer, shadow});
    uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), STR_DrowBonusJson});
    vcall(layer, node::addChild, {layer, popup});
    uint32_t setAnchor = ld32(ld32(popup) + node::setAnchorPoint);
    call(Vec2_ctor, {sp + 0x1A0, fbits(0.5f), fbits(0.5f)});
    call(setAnchor, {popup, sp + 0x1A0});
    call(Vec2_dtor, {sp + 0x1A0});
    uint32_t setPosition = ld32(ld32(popup) + node::setPosition);
    call(setPosition, {popup, vcall(shadow, node::getPosition, {shadow})});

    strInit(sp + 0x194, STR_Txt_Omake, 9);
    call(SceneUtil_settingTitleLabel, {popup, sp + 0x194, uint32_t(TEXT_OMAKE_TITLE)});
    strFree(sp + 0x194);
    uint32_t medalArea = findAt(popup, sp + 0x188, STR_Medal_Area, 0xA);
    uint32_t memory = findAt(popup, sp + 0x17C, STR_Memory, 6);
    uint32_t keyblade = findAt(popup, sp + 0x170, STR_KB, 2);
    uint32_t memoryNum = findAt(popup, sp + 0x164, STR_Txt_MemoryNum_Label, 0x13);
    uint32_t keybladeLabel = findAt(popup, sp + 0x158, STR_Txt_KB_Label, 0xC);
    uint32_t medalLabel = findAt(popup, sp + 0x14C, STR_Txt_Medal_Label, 0xF);
    uint32_t panel = findAt(popup, sp + 0x140, STR_Panel01, 0x1E);
    call(SceneUtil_setShadow, {memoryNum});
    call(SceneUtil_setShadow, {keybladeLabel});
    call(SceneUtil_setShadow, {medalLabel});
    setVisible(medalArea, 0);
    setVisible(memory, 0);
    setVisible(memoryNum, 0);
    setVisible(keyblade, 0);
    setVisible(keybladeLabel, 0);
    setVisible(medalLabel, 0);

    // the bonus of the last draw (copied: shared_ptr references, type, ids)
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t resultCtrl = ld32(api + APIMANAGER_DRAW_RESULT + 4);
    uint32_t result = ld32(api + APIMANAGER_DRAW_RESULT);
    if (resultCtrl) call(fn::shared_add, {resultCtrl});
    uint32_t bonusCtrl = ld32(result + RESULT_BONUS_CTRL);
    if (bonusCtrl) call(fn::shared_add, {bonusCtrl});
    fst32(sp + 0x130, ld32(result + RESULT_BONUS_TYPE));
    call(VectorU64_copy, {sp + 0x134, result + RESULT_BONUS_IDS});
    uint32_t type = ld32(result + RESULT_BONUS_TYPE);
    fst32(sp + 0x130, type);

    if (type == 3) {
        // a medal: its card (createDrawMedal(id, small, 0); `this` gets whatever the registers held) and its name
        uint32_t begin = ld32(sp + 0x134), end = ld32(sp + 0x138);
        if (end == begin) {
            call(Vector_throwOutOfRange, {sp + 0x134});
            return;
        }
        fst32(sp, 0);
        fst32(sp + 4, 0);
        uint32_t card = uint32_t(rw::call_exact(draw_raise::CREATE_DRAW_MEDAL,
                                                {begin + 8, end, ld32(begin), ld32(begin + 4)}));
        uint32_t mgr = call(MedalDataManager_getInstance, {});
        begin = ld32(sp + 0x134);
        end = ld32(sp + 0x138);
        if (end == begin) {
            call(Vector_throwOutOfRange, {sp + 0x134});
            return;
        }
        uint32_t lo = ld32(begin), hi = ld32(begin + 4);
        const uint32_t local = sp + 0x670;
        for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4)
            if (k != 0x2C) fst32(local + k, 0);
        fst32(sp, 0);
        for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4) fst32(sp + 8 + k, ld32(local + k));
        rw::call_exact(MedalDataManager_getMedalData, {sp + 0x128, mgr, lo, hi});
        strInit(sp + 0x11C, STR_Icon_New, 8);
        setVisible(call(SceneUtil_searchNodeByName, {card, sp + 0x11C}), 0);
        strFree(sp + 0x11C);
        setVisible(panel, 1);
        vcall(medalArea, node::addChild, {medalArea, card});
        setVisible(medalArea, 1);
        call(fn::TextData_get, {sp + 0x114, 0, uint32_t(TEXT_MEDAL_COUNT)});
        uint32_t format = ld32(ld32(sp + 0x114) + 4);
        const SafeBuffer *row = rw::host_ptr<SafeBuffer>(ld32(ld32(sp + 0x128) + USERMEDAL_MASTER));
        SafeBuffer::read(row, rw::host_ptr<uint8_t>(sp + 0x228), row->data.get() + MEDAL_NAME, 0x41, MEDAL_NAME);
        uint32_t count = uint32_t(int32_t(ld32(sp + 0x138) - ld32(sp + 0x134)) >> 3);
        call(fn::StringBuffer_ctor, {sp + 0x26C, format, sp + 0x228, count});
        releaseIf(ld32(sp + 0x118));
        setStringFrom(medalLabel, sp + 0x108, sp + 0x26C);
        setVisible(medalLabel, 1);
        releaseIf(ld32(sp + 0x12C));
    } else {
        // an item: SceneUtil::setItemImageData(label, image, type, 0, false)
        for (uint32_t k = 0xF0; k < 0x108; k += 4) fst32(sp + k, 0);
        fst32(sp, 0);
        rw::call_exact(SceneUtil_setItemImageData, {sp + 0xFC, sp + 0xF0, type, 0});
        uint32_t image = strData(sp + 0xF0);
        if (type == 0xD) {
            loadTextureFrom(keyblade, sp + 0xE4, image);
            setStringFrom(keybladeLabel, sp + 0xD8, strData(sp + 0xFC));
            setVisible(keyblade, 1);
            setVisible(keybladeLabel, 1);
        } else {
            loadTextureFrom(memory, sp + 0xCC, image);
            setStringFrom(memoryNum, sp + 0xC0, strData(sp + 0xFC));
            setVisible(memory, 1);
            setVisible(memoryNum, 1);
        }
        strFree(sp + 0xF0);
        strFree(sp + 0xFC);
    }

    // OK: $_14{popup, layer, shadow, done}
    fst32(sp + 0x200, popup);
    fst32(sp + 0x204, layer);
    fst32(sp + 0x208, shadow);
    call(fn::function_vv_copy, {sp + 0x210, done});
    uint32_t ok = findAt(popup, sp + 0xB4, STR_Button_OK, 9);
    fst32(sp + 0x1C0, ld32(sp + 0x200));
    fst32(sp + 0x1C4, ld32(sp + 0x204));
    fst32(sp + 0x1C8, ld32(sp + 0x208));
    functionCopy(sp + 0x1D0, sp + 0x210);
    fst32(sp + 0x1F8, 0);
    uint32_t obj = guest::newObject(0x30);
    if (obj) {
        st32(obj, VT_OkTap);
        st32(obj + 8, ld32(sp + 0x1C0));
        st32(obj + 0xC, ld32(sp + 0x1C4));
        st32(obj + 0x10, ld32(sp + 0x1C8));
        call(fn::function_vv_move, {obj + 0x18, sp + 0x1D0});
    }
    fst32(sp + 0x1F8, obj);
    call(fn::Widget_addTouchEventListener, {ok, sp + 0x1E8});
    functionDestroy(sp + 0x1E8);
    functionDestroy(sp + 0x1D0);
    strInit(sp + 0xA8, STR_Txt_OK, 6);
    call(SceneUtil_settingTitleLabel, {ok, sp + 0xA8, uint32_t(TEXT_OK)});
    strFree(sp + 0xA8);

    // open animation, then $_15{popup}
    fst32(sp + 0x1B8, sp + 0x1A8);
    fst32(sp + 0x1A8, VT_Opened);
    fst32(sp + 0x1AC, popup);
    uint32_t action = call(SceneUtil_createOpenPopupAnimation, {popup, sp + 0x1A8});
    call(fn::Node_runAction, {popup, call(EaseSineOut_create, {action})});
    functionDestroy(sp + 0x1A8);
    functionDestroy(sp + 0x210);
    call(VectorU64_dtor, {sp + 0x134});
    releaseIf(bonusCtrl);
    releaseIf(resultCtrl);
}
}  // namespace draw_raise_omake
RW_FUNCTION(0x691B54, "SceneDrawRaise::openDrawOmakePopup", draw_raise_omake::openDrawOmakePopup);

namespace {
// $_14 [popup, layer, shadow at +8, done at +0x18]: OK tapped (touch ended) -> decide sound, eased close animation,
// then $_13's lambda
void okTap(uint32_t self, const uint32_t *, const uint32_t *type) {
    if (*type != 2) return;
    call(AudioPlaySystemSEDecide, {});
    Frame f;
    uint32_t popup = ld32(self + 8);
    uint32_t caps = f.alloc(0x48);                     // [layer, shadow, popup] + done copy at +0x10
    fst32(caps, ld32(self + 0xC));
    fst32(caps + 4, ld32(self + 0x10));
    fst32(caps + 8, popup);
    call(fn::function_vv_copy, {caps + 0x10, self + 0x18});
    uint32_t cb = caps + 0x28;
    fst32(cb + 0x10, 0);
    uint32_t obj = guest::newObject(0x30);
    if (obj) {
        st32(obj, VT_OkDone);
        st32(obj + 8, ld32(caps));
        st32(obj + 0xC, ld32(caps + 4));
        st32(obj + 0x10, ld32(caps + 8));
        call(fn::function_vv_move, {obj + 0x18, caps + 0x10});
    }
    fst32(cb + 0x10, obj);
    uint32_t action = call(SceneUtil_createClosePopupAnimation, {popup, cb});
    call(fn::Node_runAction, {popup, call(EaseSineOut_create, {action})});
    functionDestroy(cb);
    functionDestroy(caps + 0x10);
}

// $_13's lambda [layer, shadow, popup at +8, done at +0x18]: remove the shadow and the popup, run `done`
void okDone(uint32_t self) {
    uint32_t layer = ld32(self + 8);
    vcall(layer, node::removeChild, {layer, ld32(self + 0xC), 1});
    layer = ld32(self + 8);
    vcall(layer, node::removeChild, {layer, ld32(self + 0x10), 1});
    if (ld32(self + 0x28)) call(fn::function_vv_call, {self + 0x18});
}

// $_15 [popup]: the popup is open: the bonus LWF effect on Area_Point
void opened(uint32_t self) {
    Frame f;
    uint32_t popup = ld32(self + 4);
    uint32_t name = f.alloc(12);                       // "Area_Point" (short string)
    *rw::frame_ptr<uint8_t>(name) = 0x14;
    call(AEABI_MEMCPY, {name + 1, STR_Area_Point, 0xA});
    *rw::frame_ptr<uint8_t>(name + 0xB) = 0;
    uint32_t area = call(SceneUtil_searchNodeByName, {popup, name});
    strFree(name);
    uint32_t lwf = call(AudioLWFNode_create, {STR_DrawBonusLwf, 0});
    vcall(area, node::addChild, {area, lwf});
}

using OkTapFunc = thunk::LambdaFn<VT_OkTap, 0x30, 0xF9D2B8, 0xE72EB0, 8, 0x18, fn::function_vv_copy,
                                  thunk::cap(8, 4), thunk::cap(0xC, 4), thunk::cap(0x10, 4)>;
using OkDoneFunc = thunk::LambdaFn<VT_OkDone, 0x30, 0xF9D2CC, 0xE72F90, 8, 0x18, fn::function_vv_copy,
                                   thunk::cap(8, 4), thunk::cap(0xC, 4), thunk::cap(0x10, 4)>;
using OpenedFunc = thunk::Lambda<VT_Opened, 8, 0xF9D2A4, 0xE72DB0, 4, thunk::cap(4, 4)>;
}  // namespace

RW_FUNCTION(0x6930FC, "SceneDrawRaise::openDrawOmakePopup::$_14::operator()", okTap);
RW_FUNCTION(0x692F98, "SceneDrawRaise::openDrawOmakePopup::$_14 ~__func()", OkTapFunc::destructor);
RW_FUNCTION(0x692FD4, "SceneDrawRaise::openDrawOmakePopup::$_14 ~__func() (deleting)", OkTapFunc::destructorDelete);
RW_FUNCTION(0x693014, "SceneDrawRaise::openDrawOmakePopup::$_14 __clone()", OkTapFunc::clone);
RW_FUNCTION(0x693070, "SceneDrawRaise::openDrawOmakePopup::$_14 __clone(__base*)", OkTapFunc::cloneInto);
RW_FUNCTION(0x6930B0, "SceneDrawRaise::openDrawOmakePopup::$_14 destroy()", OkTapFunc::destroy);
RW_FUNCTION(0x6930D0, "SceneDrawRaise::openDrawOmakePopup::$_14 destroy_deallocate()", OkTapFunc::destroyDeallocate);
RW_FUNCTION(0x693234, "SceneDrawRaise::openDrawOmakePopup::$_14 target(const type_info&)", OkTapFunc::target);
RW_FUNCTION(0x693258, "SceneDrawRaise::openDrawOmakePopup::$_14 target_type()", OkTapFunc::targetType);

RW_FUNCTION(0x6933D0, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda::operator()", okDone);
RW_FUNCTION(0x69326C, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda ~__func()", OkDoneFunc::destructor);
RW_FUNCTION(0x6932A8, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda ~__func() (deleting)", OkDoneFunc::destructorDelete);
RW_FUNCTION(0x6932E8, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda __clone()", OkDoneFunc::clone);
RW_FUNCTION(0x693344, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda __clone(__base*)", OkDoneFunc::cloneInto);
RW_FUNCTION(0x693384, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda destroy()", OkDoneFunc::destroy);
RW_FUNCTION(0x6933A4, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda destroy_deallocate()", OkDoneFunc::destroyDeallocate);
RW_FUNCTION(0x69340C, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda target(const type_info&)", OkDoneFunc::target);
RW_FUNCTION(0x693430, "SceneDrawRaise::openDrawOmakePopup::$_13::lambda target_type()", OkDoneFunc::targetType);

RW_FUNCTION(0x692EA8, "SceneDrawRaise::openDrawOmakePopup::$_15::operator()", opened);
RW_FUNCTION(0x692E48, "SceneDrawRaise::openDrawOmakePopup::$_15 ~__func() (deleting)", OpenedFunc::deleteThis);
RW_FUNCTION(0x692E50, "SceneDrawRaise::openDrawOmakePopup::$_15 __clone()", OpenedFunc::clone);
RW_FUNCTION(0x692E7C, "SceneDrawRaise::openDrawOmakePopup::$_15 __clone(__base*)", OpenedFunc::cloneInto);
RW_FUNCTION(0x692E9C, "SceneDrawRaise::openDrawOmakePopup::$_15 destroy()", OpenedFunc::destroy);
RW_FUNCTION(0x692E9E, "SceneDrawRaise::openDrawOmakePopup::$_15 destroy_deallocate()", OpenedFunc::deleteThis);
RW_FUNCTION(0x692F64, "SceneDrawRaise::openDrawOmakePopup::$_15 target(const type_info&)", OpenedFunc::target);
RW_FUNCTION(0x692F84, "SceneDrawRaise::openDrawOmakePopup::$_15 target_type()", OpenedFunc::targetType);
