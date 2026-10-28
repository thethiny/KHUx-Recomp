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
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright, EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create, SafeBuffer_read = 0x45A269;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance, ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t FileUtils_getInstance = rwlib::cocos2d::FileUtils::getInstance, Layout_create = rwlib::cocos2d::ui::Layout::create, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t Size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_, Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_, Vec2_add = rwlib::cocos2d::Vec2::add, Rect_setRect = rwlib::cocos2d::Rect::setRect;
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint, Touch_getLocation = rwlib::cocos2d::Touch::getLocation, RectTouchFn_call = 0x7616A1;
constexpr uint32_t ImageView_isScale9Enabled = rwlib::cocos2d::ui::ImageView::isScale9Enabled, Scale9SpriteR_create = 0x5EC25D;
constexpr uint32_t Scale9SpriteR_setContentCenter = 0x5EC361;
constexpr uint32_t ActionManagerEx_getInstance = rwlib::cocostudio::ActionManagerEx::getInstance, ActionManagerEx_playActionByName = rwlib::cocostudio::ActionManagerEx::playActionByName;
constexpr uint32_t AppGetGrameFrameWidth = 0x799B31, AppGetGrameFrameHeight = 0x799D21;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t GestureRecognizer_dtor = 0x4F3621, Layout_dtor = rwlib::cocos2d::ui::Layout::dtor_base;
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
// TutorialWidget: +0x530 the touch layout, +0x500 GestureRecognizer, +0x528 / +0x538 the hit test callback
// (function<bool(Rect&, Touch*)>), +0x528 blocked flag (+0x528 = word 330), +0x534 the action, +0x550 the hole
constexpr uint32_t TW_LAYOUT = 0x530, TW_GESTURE = 0x500, TW_BLOCKED = 0x528, TW_ACTION = 0x534, TW_HIT = 0x538;
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

// ------------------------------------------------------------------------------------------------ menu / continue / retire lambdas
namespace {
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t StageUI_openRetireDialog = 0x75FB9D, StageUI_openContinueDialog = 0x75FDB9;
constexpr uint32_t APIManager_executeStageContinue = 0x4CB5ED, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;
constexpr uint32_t BuyMagicStoneDialog_open = 0x588CAD;          // (const function<void()>&, bool)
constexpr uint32_t FunctionFn_call = 0x634989;                   // function<void(function<void(bool)>)>::operator()
constexpr uint32_t function_vv_copy = 0x4683CD, function_vb_call = 0x474139;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t MakeContinueDone = 0x7627F5;                  // sub_7627F4 (makeContinueDone below)
constexpr uint32_t RetireDone = 0x763665;                        // sub_763664 (retireDone below)
constexpr uint32_t Misc_create = 0x786D5D;
constexpr uint32_t GOT_CREATE_ADVENTURE_SELECT = 0xFD669C;
constexpr uint32_t VT_MenuCloseEnd = 0xF6E32C;      // the battle menu's close animation end {layer, shadow, popup}
constexpr uint32_t VT_MenuRetireAnswer = 0xF6E3B0;  // the battle menu's retire answer (no captures)
constexpr uint32_t VT_ContinueDialog = 0xF6E434;    // openContinueDialog $_3 {continue +8, this +0x20, give up +0x28} (0x40)
constexpr uint32_t VT_ContinueRetire = 0xF6E460;    // $_3's retire answer {this +8, give up +0x10} (0x28)
constexpr uint32_t VT_ContinueBuyAsk = 0xF6E48C;    // $_3's "buy Jewels?" answer {this +8, give up +0x10} (0x28)
constexpr uint32_t VT_ContinueBuyClosed = 0xF6E4C0; // the Buy Jewels dialog closed {this +8, give up +0x10} (0x28)
constexpr uint32_t VT_ContinueDone = 0xF6E4E4;      // executeStageContinue's answer {continue +8} (0x20)
constexpr uint32_t VT_ContinueError = 0xF6E510;     // its error dialog (no captures)
constexpr uint32_t VT_RetireDoneFn = 0xF6E5C0;      // the retire hook's answer (no captures; body sub_763664)
constexpr uint32_t VT_RetireError = 0xF6E594;       // its error dialog (no captures)
constexpr uint32_t F_RETIRE_HOOK = 0x78;            // StageUI: function<void(function<void(bool)>)>
constexpr uint32_t API_JEWELS = 0xA4;               // APIManager: SafeBuffer (free Jewels at 0, paid at 4)
constexpr uint32_t API_SHOW_ERROR = 0x14;
constexpr uint32_t MISC_CONTINUE_COST = 113;
constexpr int32_t TXT_BUY_JEWELS_TO_CONTINUE = 102600016;
constexpr uint32_t TOUCH_ENDED = 2;

// a heap __func `vt` (0x28 bytes) {this +8, a clone of the std::function at `fn` +0x10} into the std::function `fo`
uint32_t heapThisFn(uint32_t fo, uint32_t vt, uint32_t self, uint32_t fn) {
    st32(fo + 16, 0);
    const uint32_t obj = heapObject(vt, 0x28);
    if (obj) {
        st32(obj + 8, self);
        functionCopy(obj + 0x10, fn);
        st32(obj + 8, self);
    }
    st32(fo + 16, obj);
    return obj;
}
void apiErrorDialog(uint32_t vt) {
    if (!ld8(call(fn::APIManager_getInstance, {}) + API_SHOW_ERROR)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, vt, {});
    call(APIManager_openErrorDialog, {api, fo, 1});
    functionDestroy(fo);
}
}  // namespace

namespace stageui_menu {

// openBattleMenuPopup's closed {-, popup, layer, shadow, -, answer +0x18}(bool): the popup's close animation
// (removing popup and shadow at its end), the shadow fading out, the answer
uint32_t menuClosed(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t popup = ld32(functor + 8);
    {
        Frame f;
        const uint32_t end = newFunction(f, VT_MenuCloseEnd, {ld32(functor + 0xC), ld32(functor + 0x10), popup});
        call(Node_runAction, {popup, call(SceneUtil_createClosePopupAnimation, {popup, end})});
        functionDestroy(end);
    }
    call(Node_runAction, {ld32(functor + 0x10), call(fn::FadeTo_create, {fbits(0.1f), 0})});
    call(function_vb_call, {functor + 0x18, ok ? 1u : 0u});
    return 0;
}
// the close animation's end {layer, shadow, popup}
uint32_t menuCloseEnd(uint32_t functor) {
    const uint32_t layer = ld32(functor + 4);
    vcall(layer, node::removeChild, {layer, ld32(functor + 8), 1});
    vcall(ld32(functor + 4), node::removeChild, {ld32(functor + 4), ld32(functor + 0xC), 1});
    return 0;
}
// Retire {this}: the retire question (not while the menu is closing)
uint32_t menuRetireTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return 0;
    const uint32_t self = ld32(functor + 4);
    if (ld8(self + F_MENU_FLAG)) return 0;
    Frame f;
    const uint32_t answer = newFunction(f, VT_MenuRetireAnswer, {});
    call(StageUI_openRetireDialog, {self, answer});
    functionDestroy(answer);
    return 0;
}
// Close {this}: the menu closes
uint32_t menuCloseTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return 0;
    st8(ld32(functor + 4) + F_MENU_FLAG, 1);
    call(AudioPlaySystemSECancel, {});
    call(SM_closeCurrentPopup, {sceneManager(), 0});
    return 0;
}

// openContinueDialog $_3 {continue +8, this +0x20, give up +0x28}(yes): yes with enough Jewels (misc 113's cost,
// free + paid) -> the continue request; yes without -> "buy Jewels?"; no -> the retire question
uint32_t continueAnswer(uint32_t functor, uint32_t yesRef) {
    const uint32_t self = ld32(functor + 0x20);
    if (!ld8(yesRef)) {
        Frame f;
        const uint32_t copy = f.alloc(20), inner = f.alloc(20), answer = f.alloc(20);
        call(function_vv_copy, {copy, functor + 0x28});
        functionCopy(inner, copy);
        heapThisFn(answer, VT_ContinueRetire, self, inner);
        call(StageUI_openRetireDialog, {self, answer});
        functionDestroy(answer);
        functionDestroy(inner);
        functionDestroy(copy);
        return 0;
    }
    int32_t cost;
    {
        Frame f;
        const uint32_t misc = masterRow(f, Misc_create, MISC_CONTINUE_COST);
        cost = safe32(ld32(misc), 4);
        const uint32_t jewels = ld32(call(fn::APIManager_getInstance, {}) + API_JEWELS);
        const int32_t free = safe32(jewels, 0);
        const int32_t paid = safe32(ld32(call(fn::APIManager_getInstance, {}) + API_JEWELS), 4);
        sharedRelease(misc);
        if (cost > paid + free) {
            const uint32_t copy = f.alloc(20);
            call(function_vv_copy, {copy, functor + 0x28});
            const uint32_t td = textData(f, TXT_BUY_JEWELS_TO_CONTINUE);
            const uint32_t s = newStr(f, textDataStr(td));
            const uint32_t inner = f.alloc(20), answer = f.alloc(20);
            functionCopy(inner, copy);
            heapThisFn(answer, VT_ContinueBuyAsk, self, inner);
            call(SceneUtil_openOkCancelDialog4Line, {s, answer, 0, 0});
            functionDestroy(answer);
            functionDestroy(inner);
            strFree(s);
            sharedRelease(td);
            functionDestroy(copy);
            return 0;
        }
    }
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vv_copy, {copy, functor + 8});
    const uint32_t done = f.alloc(20);
    call(MakeContinueDone, {done, copy});
    call(APIManager_executeStageContinue, {api, done});
    functionDestroy(done);
    functionDestroy(copy);
    return 0;
}
// $_3's clone
uint32_t continueClone(uint32_t functor) {
    const uint32_t obj = heapObject(VT_ContinueDialog, 0x40);
    if (obj) {
        call(function_vv_copy, {obj + 8, functor + 8});
        st32(obj + 0x20, ld32(functor + 0x20));
        call(function_vv_copy, {obj + 0x28, functor + 0x28});
        st32(obj + 0x20, ld32(functor + 0x20));
    }
    return obj;
}
// "buy Jewels?" {this, give up}(yes): yes -> the Buy Jewels dialog (its close back here); no -> the continue
// question again
uint32_t continueBuyAnswer(uint32_t functor, uint32_t yesRef) {
    const uint32_t self = ld32(functor + 8);
    if (!ld8(yesRef)) return call(StageUI_openContinueDialog, {self, functor + 0x10});
    Frame f;
    const uint32_t copy = f.alloc(20), closed = f.alloc(20);
    call(function_vv_copy, {copy, functor + 0x10});
    heapThisFn(closed, VT_ContinueBuyClosed, self, copy);
    call(BuyMagicStoneDialog_open, {closed, 0});
    functionDestroy(closed);
    functionDestroy(copy);
    return 0;
}
// sub_7627F4(out, continue): executeStageContinue's answer, a heap $_1 {copy of continue}
uint32_t makeContinueDone(uint32_t out, uint32_t fn) {
    st32(out + 16, 0);
    const uint32_t obj = heapObject(VT_ContinueDone, 0x20);
    if (obj) call(function_vv_copy, {obj + 8, fn});
    st32(out + 16, obj);
    return out;
}
// $_1 {continue}(ok): continued -> continue(); failed -> the API error dialog
uint32_t continueDone(uint32_t functor, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) call(fn::function_vv_call, {functor + 8});
    else apiErrorDialog(VT_ContinueError);
    return 0;
}
// $_2 {continue}: the continue request again (the error dialog's retry)
uint32_t continueRetry(uint32_t functor) {
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vv_copy, {copy, functor + 8});
    const uint32_t done = f.alloc(20);
    call(MakeContinueDone, {done, copy});
    call(APIManager_executeStageContinue, {api, done});
    functionDestroy(done);
    functionDestroy(copy);
    return 0;
}

// openRetireDialog $_0 {-, this, -, answer +0x10}(yes): yes -> through the scene's retire hook (or straight to the
// adventure select); the answer either way
uint32_t retireAnswer(uint32_t functor, uint32_t yesRef) {
    const uint8_t yes = ld8(yesRef);
    if (yes) {
        const uint32_t self = ld32(functor + 8);
        call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        if (ld32(self + F_RETIRE_HOOK + 16)) {
            Frame f;
            const uint32_t done = newFunction(f, VT_RetireDoneFn, {});
            call(FunctionFn_call, {self + F_RETIRE_HOOK, done});
            functionDestroy(done);
        } else {
            call(RetireDone, {1});
        }
    }
    call(function_vb_call, {functor + 0x10, yes ? 1u : 0u});
    return 0;
}
// sub_763664(ok): retired -> the adventure select (its API first); failed -> the API error dialog
uint32_t retireDone(uint32_t ok) {
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        apiErrorDialog(VT_RetireError);
        return 0;
    }
    apiBusyDropPayment();
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneAdventureSelect_callAdventureSelectAPI, {none});
        functionDestroy(none);
    }
    changeSceneLayerTo(GOT_CREATE_ADVENTURE_SELECT, 1, 1, 1);
    return 0;
}

}  // namespace stageui_menu

RW_FUNCTION(0x761AFC, "StageUI::openBattleMenuPopup $_8::operator()(bool) (closed)", stageui_menu::menuClosed);
RW_FUNCTION(0x761C7A, "StageUI::openBattleMenuPopup $_8 close animation end::operator()", stageui_menu::menuCloseEnd);
RW_FUNCTION(0x761DD0, "StageUI::openBattleMenuPopup $_6::operator() (Retire)", stageui_menu::menuRetireTouched);
RW_FUNCTION(0x762056, "StageUI::openBattleMenuPopup $_4::operator() (Close)", stageui_menu::menuCloseTouched);
RW_FUNCTION(0x7622F8, "StageUI::openContinueDialog $_3::operator()(bool)", stageui_menu::continueAnswer);
RW_FUNCTION(0x762160, "StageUI::openContinueDialog $_3 __clone()", stageui_menu::continueClone);
RW_FUNCTION(0x762B60, "StageUI::openContinueDialog $_3 buy Jewels answer::operator()(bool)", stageui_menu::continueBuyAnswer);
RW_FUNCTION(0x7627F4, "StageUI::openContinueDialog continue answer maker (sub_7627F4)", stageui_menu::makeContinueDone);
RW_FUNCTION(0x762FDC, "StageUI::openContinueDialog $_1::operator()(bool)", stageui_menu::continueDone);
RW_FUNCTION(0x7632BC, "StageUI::openContinueDialog $_2::operator()", stageui_menu::continueRetry);
RW_FUNCTION(0x763544, "StageUI::openRetireDialog $_0::operator()(bool)", stageui_menu::retireAnswer);
RW_FUNCTION(0x763664, "StageUI::openRetireDialog $_0 retire done (sub_763664)", stageui_menu::retireDone);
