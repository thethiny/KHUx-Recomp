// StageUI's in-battle menu (the pause popup: Close, Option, Retire - the colosseum variant shows the course and
// arrows), the retire confirmation, and the tutorial overlay widgets of the tutorial battles (Tutorial_<n>.json with a
// "Mask" hole through which taps pass).
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t PauseState_create = 0x5EDB31, StageUI_createBattleMenuPopup = 0x760295;
constexpr uint32_t StageUI_createColosseumMenuPopup = 0x760411, StageUI_deleteTutorialWidget = 0x75E5E1;
constexpr uint32_t TutorialWidget_create = 0x761431, TutorialWidget_ctor = 0x7638F9;
constexpr uint32_t AdvStageScrollView_setSubMisstionInfo = 0x56B301, AdvStageScrollView_setColosseumInfo = 0x56D65D;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_setButtonEnabled = 0x468625;
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951, SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t Widget_setBright = 0x825A09, EaseSineOut_create = 0x860D09, SafeBuffer_read = 0x45A269;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t FileUtils_getInstance = 0xA3F44D, Layout_create = 0x82861D, Size_ctor = 0x8DF027;
constexpr uint32_t Size_copy = 0x8DF02D, Vec2_copy = 0x8E4C03, Vec2_add = 0x8E4D2D, Rect_setRect = 0x8DF183;
constexpr uint32_t Rect_containsPoint = 0x8DF2A9, Touch_getLocation = 0x900E57, RectTouchFn_call = 0x7616A1;
constexpr uint32_t ImageView_isScale9Enabled = 0x839B71, Scale9SpriteR_create = 0x5EC25D;
constexpr uint32_t Scale9SpriteR_setContentCenter = 0x5EC361;
constexpr uint32_t ActionManagerEx_getInstance = 0x79B535, ActionManagerEx_playActionByName = 0x79BE43;
constexpr uint32_t AppGetGrameFrameWidth = 0x799B31, AppGetGrameFrameHeight = 0x799D21;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t GestureRecognizer_dtor = 0x4F3621, Layout_dtor = 0x8289C5;
constexpr uint32_t function_vb_copy = 0x45C84D;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t VT_RetireAnswer = 0xF6E568;    // {pad, this, pad, answer at +0x10} (heap 0x28)
constexpr uint32_t VT_MenuClose = 0xF6E408, VT_MenuOption = 0xF6E3DC, VT_MenuRetire = 0xF6E384;   // {this}
constexpr uint32_t VT_MenuCanClose = 0xF6E358;    // {this}
constexpr uint32_t VT_MenuClosed = 0xF6E300;      // {pad, popup, layer, shadow, pad, answer at +0x18} (heap 0x30)
constexpr uint32_t VT_TutorialWidget = 0xFA885C, VT_TutorialWidget2 = 0xFA8BA0, VT_TutorialWidget3 = 0xFA8BB4;
constexpr uint32_t S_BattleMenuJson = 0xE075F4, S_ColosseumMenuJson = 0xE07625, S_ArrowFile = 0xE00B10;
constexpr uint32_t S_PointLeft = 0xDFF5F1, S_ArrowAnim = 0xE00B43, S_Left = 0xE033B4, S_PointRight = 0xDFF602;
constexpr uint32_t S_Right = 0xE00B59, S_CloseButton = 0xDF8D2F, S_OptionButton = 0xE00575, S_Txt = 0xDF94D1;
constexpr uint32_t S_RetiredButton = 0xE07651, S_TutorialFmt = 0xE07675, S_PublishFmt = 0xE0765F, S_Mask = 0xDF99A1;
constexpr uint32_t S_Animation0 = 0xDF9996, S_TxtTutorial = 0xDF99A6;
constexpr uint32_t V_release = 0x04, V_getScaleX = 0x2C, V_getScaleY = 0x34, V_setPosition = 0x4C;
constexpr uint32_t V_getPosition = 0x54, V_setPositionXY = 0x5C, V_setAnchorPoint = 0x9C, V_setContentSize = 0xA8;
constexpr uint32_t V_getContentSize = 0xAC, V_setVisible = 0xB0, V_addChild = 0xF0, V_addChildZ = 0xF4;
constexpr uint32_t V_getChildByName = 0x104, V_getOpacity = 0x20C, V_setOpacity = 0x214, V_init = 0x248;
constexpr uint32_t V_getAnimation = 0x298, V_setTouchEnabled = 0x29C, V_getLayoutSize = 0x2B8;
constexpr uint32_t V_getVirtualRenderer = 0x2D8, V_playAnimation = 0x68, V_isFileExist = 0x58;
// StageUI: +0x70 menu open (byte), +0x90 the tutorial widget
constexpr uint32_t F_MENU_FLAG = 0x70, F_TUTORIAL = 0x90;
// TutorialWidget: +0x14C the touch layout, +0x500 GestureRecognizer, +0x528 / +0x538 the hit test callback
// (function<bool(Rect&, Touch*)>), +0x528 blocked flag (+0x528 = word 330), +0x534 the action, +0x550 the hole
constexpr uint32_t TW_LAYOUT = 0x14C, TW_GESTURE = 0x500, TW_BLOCKED = 0x528, TW_ACTION = 0x534, TW_HIT = 0x538;
constexpr uint32_t TW_HOLE = 0x550, TW_SIZE = 0x560;
// APIManager: +0x13C StageStart (+0x30 the stage, +0x4C the sub missions), +0x260 ColosseumStart (+0x28 course),
// +0x64 TutorialStatus (+0x10 finished), +0x94 UserDetail (+0x40 the retire allowance)
constexpr uint32_t API_STAGE_START = 0x13C, API_COLOSSEUM_START = 0x260, API_TUTORIAL = 0x64, API_USER_DETAIL = 0x94;

void labelOf(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

// the field menu layout, centred on a 960x640 frame with the side margins (sizes read the original's way)
uint32_t fieldMenu(uint32_t json) {
    const uint32_t w = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    vcall(w, V_setAnchorPoint, {w, ld32(GOT_ANCHOR_MIDDLE)});
    const float w1 = ldf(vcall(w, V_getLayoutSize, {w}));
    const float m = bitsf(call(SceneUtil_getSideMargin, {}));
    const float w2 = ldf(vcall(w, V_getLayoutSize, {w}));
    const float h1 = ldf(vcall(w, V_getLayoutSize, {w}) + 4);
    const float hm = bitsf(call(SceneUtil_getSideHeightMargin, {fbits(h1)}));
    const float h2 = ldf(vcall(w, V_getLayoutSize, {w}) + 4);
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(Vec2_ctor, {pos, fbits(((960.0f - w1) + m) + w2 * 0.5f), fbits(((640.0f - h1) + hm) + h2 * 0.5f)});
    vcall(w, V_setPosition, {w, pos});
    call(Vec2_dtor, {pos});
    vcall(w, V_setTouchEnabled, {w, 0});
    return w;
}

// an ArrowAnim armature playing `move`, added to node `point`
void arrow(uint32_t root, uint32_t point, uint32_t len, uint32_t move, uint32_t moveLen) {
    const uint32_t at = findNode(root, point, len);
    uint32_t armature;
    {
        Frame f;
        const uint32_t s = newStr(f, S_ArrowAnim, 9);
        armature = call(Armature_create, {s});
        strFree(s);
    }
    const uint32_t anim = vcall(armature, V_getAnimation, {armature});
    {
        Frame f;
        const uint32_t s = newStr(f, move, moveLen);
        vcall(anim, V_playAnimation, {anim, s, 0xFFFFFFFF, 0xFFFFFFFF});
        strFree(s);
    }
    vcall(at, V_addChild, {at, armature});
}
}  // namespace

namespace stageui_menu {

// "Retire?": OK / Cancel, the answer through a heap lambda {this, answer}
uint32_t openRetireDialog(uint32_t self, uint32_t answer) {
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, answer});
    const uint32_t td = textData(f, 103100007);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t inner = f.alloc(20);
    functionCopy(inner, copy);
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(VT_RetireAnswer, 0x28);
    if (obj) {
        st32(obj + 8, self);
        functionCopy(obj + 0x10, inner);
        st32(obj + 8, self);
    }
    st32(fo + 16, obj);
    call(SceneUtil_openOkCancelDialog4Line, {s, fo, 0, 0});
    functionDestroy(fo);
    functionDestroy(inner);
    strFree(s);
    sharedRelease(td);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x75FB9C, "StageUI::openRetireDialog", stageui_menu::openRetireDialog);

// the quest's pause menu: the stage's sub missions
uint32_t createBattleMenuPopup(uint32_t) {
    const uint32_t w = fieldMenu(S_BattleMenuJson);
    const uint32_t start = ld32(call(fn::APIManager_getInstance, {}) + API_STAGE_START);
    call(AdvStageScrollView_setSubMisstionInfo, {w, ld32(start + 0x30), start + 0x4C});
    return w;
}
RW_FUNCTION(0x760294, "StageUI::createBattleMenuPopup", stageui_menu::createBattleMenuPopup);

// the colosseum's pause menu: the course, with arrows
uint32_t createColosseumMenuPopup(uint32_t) {
    const uint32_t w = fieldMenu(S_ColosseumMenuJson);
    {
        const uint32_t mgr = call(ArmatureDataManager_getInstance, {});
        Frame f;
        const uint32_t s = newStr(f, S_ArrowFile, 0x27);
        call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        strFree(s);
    }
    arrow(w, S_PointLeft, 0x10, S_Left, 4);
    arrow(w, S_PointRight, 0x11, S_Right, 5);
    const uint32_t start = ld32(call(fn::APIManager_getInstance, {}) + API_COLOSSEUM_START);
    Frame f;
    const uint32_t course = f.alloc(16);
    st32(course, ld32(start + 0x28));
    st32(course + 4, ld32(start + 0x2C));
    st32(course + 8, ld32(start + 0x30));
    call(AdvStageScrollView_setColosseumInfo, {w, course});
    return w;
}
RW_FUNCTION(0x760410, "StageUI::createColosseumMenuPopup", stageui_menu::createColosseumMenuPopup);

// the pause popup (`style` 1: the colosseum's) over a PauseState; Retire is greyed out in the tutorial and when the
// user data allows no retiring; closing runs `answer`
uint32_t openBattleMenuPopup(uint32_t self, uint32_t style, uint32_t answer) {
    const uint32_t pause = call(PauseState_create, {});
    st8(self + F_MENU_FLAG, 0);
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = addShadow(layer, true);
    const uint32_t popup = call(style == 1 ? StageUI_createColosseumMenuPopup : StageUI_createBattleMenuPopup, {style});
    vcall(popup, V_addChild, {popup, pause});
    vcall(layer, V_addChild, {layer, popup});
    addTouch(findNode(popup, S_CloseButton, 0xC), VT_MenuClose, {self});
    const uint32_t option = findNode(popup, S_OptionButton, 0xC);
    addTouch(option, VT_MenuOption, {self});
    labelOf(option, S_Txt, 3, 101000008);
    const uint32_t retire = findNode(popup, S_RetiredButton, 0xD);
    addTouch(retire, VT_MenuRetire, {self});
    labelOf(retire, S_Txt, 3, 103100010);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t tutorial = ld32(call(fn::APIManager_getInstance, {}) + API_TUTORIAL);
    bool off = tutorial && !ld8(tutorial + 0x10);
    if (!off) {
        const uint32_t sb = ld32(api + API_USER_DETAIL);
        Frame f;
        const uint32_t v = f.alloc(4);
        call(SafeBuffer_read, {sb, v, ld32(sb + 0xC) + 0x40, 4, 0x40});
        off = !ld32(v);
    }
    if (off) {
        vcall(retire, V_setTouchEnabled, {retire, 0});
        call(Widget_setBright, {retire, 0});
        const uint32_t text = findNode(retire, S_Txt, 3);
        call(SceneUtil_setButtonEnabled, {retire, text, 0, 0});
    }
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(Node_runAction, {popup, call(EaseSineOut_create, {call(SceneUtil_createOpenPopupAnimation, {popup, none})})});
        functionDestroy(none);
    }
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t canClose = newFunction(f, VT_MenuCanClose, {self});
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, answer});
    const uint32_t closed = f.alloc(20);
    const uint32_t obj = heapObject(VT_MenuClosed, 0x30);
    if (obj) {
        st32(obj + 8, popup);
        st32(obj + 0xC, layer);
        st32(obj + 0x10, shadow);
        functionCopy(obj + 0x18, copy);
        st32(obj + 8, popup);
        st32(obj + 0xC, layer);
        st32(obj + 0x10, shadow);
    }
    st32(closed + 16, obj);
    call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
    functionDestroy(closed);
    functionDestroy(copy);
    functionDestroy(canClose);
    return 0;
}
RW_FUNCTION(0x760814, "StageUI::openBattleMenuPopup", stageui_menu::openBattleMenuPopup);

// the tutorial overlay `n` (Tutorial_<n>.json, when the file exists): a full-screen TutorialWidget holding the
// layout and a touch layout; the "Mask" image becomes a 9-slice hole (+0x550 its rect); the layout's Animation0 plays
uint32_t createTutorialWidget(uint32_t self, uint32_t n) {
    Frame f;
    const uint32_t name = stringBuffer(f, {S_TutorialFmt, n});
    const uint32_t path = stringBuffer(f, {S_PublishFmt, name});
    if (ld32(self + F_TUTORIAL)) call(StageUI_deleteTutorialWidget, {self});
    const uint32_t files = call(FileUtils_getInstance, {});
    uint32_t exists;
    {
        Frame g;
        const uint32_t s = newStr(g, path);
        exists = vcall(files, V_isFileExist, {files, s});
        strFree(s);
    }
    if (exists != 1) return 0;
    uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), path});
    if (!root) return 0;
    const uint32_t size = f.alloc(8);
    {
        const uint32_t fw = call(AppGetGrameFrameWidth, {});
        const uint32_t fh = call(AppGetGrameFrameHeight, {});
        call(Size_ctor, {size, fw, fh});
    }
    const uint32_t widget = call(TutorialWidget_create, {});
    vcall(widget, V_setContentSize, {widget, size});
    vcall(widget, V_setTouchEnabled, {widget, 1});
    vcall(widget, V_addChild, {widget, root});
    st32(widget + TW_LAYOUT, call(Layout_create, {}));
    vcall(ld32(widget + TW_LAYOUT), V_setContentSize, {ld32(widget + TW_LAYOUT), size});
    vcall(ld32(widget + TW_LAYOUT), V_setTouchEnabled, {ld32(widget + TW_LAYOUT), 1});
    vcall(widget, V_addChildZ, {widget, ld32(widget + TW_LAYOUT), 0xFFFFFFFF});
    const uint32_t rootSize = vcall(root, V_getContentSize, {root});
    if (ldf(rootSize) < bitsf(call(AppGetGrameFrameWidth, {})) ||
        ldf(rootSize + 4) < bitsf(call(AppGetGrameFrameHeight, {}))) {
        const float x = (bitsf(call(AppGetGrameFrameWidth, {})) - ldf(rootSize)) * 0.5f;
        const float y = bitsf(call(AppGetGrameFrameHeight, {})) - ldf(rootSize + 4);
        Frame g;
        const uint32_t pos = g.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits(y * 0.5f)});
        vcall(root, V_setPosition, {root, pos});
        call(Vec2_dtor, {pos});
    }
    uint32_t mask;
    {
        Frame g;
        const uint32_t s = newStr(g, S_Mask, 4);
        mask = vcall(root, V_getChildByName, {root, s});
        strFree(s);
    }
    if (mask) {
        const uint32_t renderer = vcall(mask, V_getVirtualRenderer, {mask});
        const uint32_t scale9 = call(ImageView_isScale9Enabled, {mask});
        const uint32_t hole = call(Scale9SpriteR_create, {renderer, scale9, fbits(1.0f)});
        const uint32_t maskPos = vcall(mask, V_getPosition, {mask});
        const uint32_t rootPos = vcall(root, V_getPosition, {root});
        Frame g;
        const uint32_t at = g.alloc(8);
        call(Vec2_copy, {at, maskPos});
        call(Vec2_add, {at, rootPos});
        const uint32_t hs = g.alloc(8);
        call(Size_copy, {hs, vcall(mask, V_getContentSize, {mask})});
        stf(hs, bitsf(vcall(mask, V_getScaleX, {mask})) * ldf(hs));
        stf(hs + 4, bitsf(vcall(mask, V_getScaleY, {mask})) * ldf(hs + 4));
        call(Scale9SpriteR_setContentCenter, {hole, at, hs, vcall(widget, V_getContentSize, {widget})});
        vcall(hole, V_setOpacity, {hole, vcall(mask, V_getOpacity, {mask})});
        vcall(hole, V_setPositionXY, {hole, fbits(ldf(rootSize) * 0.5f), fbits(ldf(rootSize + 4) * 0.5f)});
        vcall(root, V_addChildZ, {root, hole, 0xFFFFFFFF});
        vcall(mask, V_setVisible, {mask, 0});
        call(Rect_setRect, {widget + TW_HOLE, fbits(ldf(at) - ldf(hs) * 0.5f), fbits(ldf(at + 4) - ldf(hs + 4) * 0.5f),
                            ld32(hs), ld32(hs + 4)});
        call(Vec2_dtor, {at});
    }
    const uint32_t mgr = call(ActionManagerEx_getInstance, {});
    st32(widget + TW_ACTION, call(ActionManagerEx_playActionByName, {mgr, name, S_Animation0}));
    st32(self + F_TUTORIAL, widget);
    call(Ref_retain, {widget});
    if (n == 103) labelOf(root, S_TxtTutorial, 0xC, 104820103);
    else if (n == 102) labelOf(root, S_TxtTutorial, 0xC, 104820102);
    return widget;
}
RW_FUNCTION(0x760EC8, "StageUI::createTutorialWidget", stageui_menu::createTutorialWidget);

uint32_t tutorialWidgetCreate() {
    const uint32_t w = call(fn::operator_new, {TW_SIZE});
    call(TutorialWidget_ctor, {w});
    if (vcall(w, V_init, {w}) == 1) {
        call(Ref_autorelease, {w});
        return w;
    }
    vcall(w, V_release, {w});
    return 0;
}
RW_FUNCTION(0x761430, "StageUI::TutorialWidget::create", stageui_menu::tutorialWidgetCreate);

// a touch passes through the hole (when the hit test, if any, allows); elsewhere it is swallowed
uint32_t tutorialWidgetTouchBegan(uint32_t self, uint32_t touch, uint32_t) {
    if (ld32(self + TW_BLOCKED)) return 0;
    Frame f;
    const uint32_t at = f.alloc(8);
    call(Touch_getLocation, {at, touch});
    const uint32_t inside = call(Rect_containsPoint, {self + TW_HOLE, at});
    call(Vec2_dtor, {at});
    if (inside != 1) return 1;
    if (!ld32(self + TW_HIT + 0x10)) return 0;
    return call(RectTouchFn_call, {self + TW_HIT, self + TW_HOLE, touch}) ^ 1;
}
RW_FUNCTION(0x7615EC, "StageUI::TutorialWidget::onTouchBegan", stageui_menu::tutorialWidgetTouchBegan);

uint32_t tutorialWidgetDestroy(uint32_t self) {
    st32(self, VT_TutorialWidget);
    st32(self + 0x280, VT_TutorialWidget2);
    st32(self + 0x350, VT_TutorialWidget3);
    call(Vec2_dtor, {self + TW_HOLE});
    functionDestroy(self + TW_HIT);
    call(GestureRecognizer_dtor, {self + TW_GESTURE});
    return call(Layout_dtor, {self});
}
RW_FUNCTION(0x761740, "StageUI::TutorialWidget::~TutorialWidget", stageui_menu::tutorialWidgetDestroy);

}  // namespace stageui_menu
