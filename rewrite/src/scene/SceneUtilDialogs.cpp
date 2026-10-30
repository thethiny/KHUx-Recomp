// SceneUtil: message dialogs (PopupDialog) and the AP-recovery popup.
#include "../core/SafeBuffer.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t PopupDialog_create = 0x4F3C15;        // (ButtonType, WindowType)
constexpr uint32_t PopupDialog_setOkLabel = 0x4F4569;
constexpr uint32_t PopupDialog_setCancelLabel = 0x4F4605;
constexpr uint32_t PopupDialog_open = 0x4F420D;          // (dialog, parent, const string& message, function<void(bool)>)
constexpr uint32_t FunctionBB_copy = 0x45CD55;            // std::function<void(bool,bool)>(const function&)
constexpr uint32_t AppState_getCurrentAP = 0x4EA5E9;      // (state, long* out)

// PopupDialog::ButtonType / WindowType
constexpr uint32_t BUTTONS_OK = 0, BUTTONS_OK_CANCEL = 1;
constexpr uint32_t WINDOW_4_LINES = 0, WINDOW_8_LINES = 1;

// APIManager: shared_ptr<UserPoint> (a SafeBuffer: paid jewels at 0, free jewels at 4)
constexpr uint32_t APIManager_userPoint = 0xA4;
constexpr uint32_t USERPOINT_PAID_JEWELS = 0, USERPOINT_FREE_JEWELS = 4;
constexpr uint32_t SceneManager_apRecoveryText = 0x304;  // SceneManager field: the AP countdown label

// lambdas
constexpr uint32_t VT_ChargeApDialog_3 = 0xF55110;        // openChargeApDialog: wraps the (bool,bool) callback
constexpr uint32_t VT_ChargeApPopup_OkTouch = 0xF550E4;   // $_4
constexpr uint32_t VT_ChargeApPopup_CloseTouch = 0xF550B8;   // $_5
constexpr uint32_t VT_ChargeApPopup_CanClose = 0xF5508C;  // $_6 (bool()), captures the close button
constexpr uint32_t VT_ChargeApPopup_OnClose = 0xF55034;   // $_7 (void(bool)), heap stored

// strings
constexpr uint32_t STR_PopupApRecoveryJson = 0xDF85FF;    // "cocostudio/publish/PopupNormal_AP_Recovery.json"
constexpr uint32_t STR_Txt_Wording = 0xDF862F;
constexpr uint32_t STR_Txt_Wording2 = 0xDF863B;
constexpr uint32_t STR_Txt_AP_Recovery = 0xDF8648;
constexpr uint32_t STR_Txt_CrownPossessed = 0xDF8658;
constexpr uint32_t STR_Txt_CrownPossessed_Label = 0xDF866B;
constexpr uint32_t STR_7d = 0xDF8684;                     // "%7d"
constexpr uint32_t STR_Button_OK = 0xDF8688;
constexpr uint32_t STR_Button_Close = 0xDF8692;
constexpr uint32_t STR_Txt_OK = 0xDF869F;
constexpr uint32_t STR_Txt_Cancel = 0xDF86A6;

constexpr uint32_t MISC_AP_RECOVERY = 112;                // master Misc row: AP recovered per jewel

// open a PopupDialog with `message` on `parent` (0: SceneManager popup layer 2)
void openDialog(uint32_t buttons, uint32_t window, uint32_t parent, uint32_t message, uint32_t callback,
                uint32_t okLabel, uint32_t cancelLabel, bool hasCancel) {
    uint32_t dialog = call(PopupDialog_create, {buttons, window});
    if (okLabel) call(PopupDialog_setOkLabel, {dialog, okLabel});
    if (hasCancel && cancelLabel) call(PopupDialog_setCancelLabel, {dialog, cancelLabel});
    if (!parent) parent = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, callback});
    call(PopupDialog_open, {dialog, parent, message, cb});
    functionDestroy(cb);
    call(fn::AudioPlaySystemSEPopUp, {});
}

// the user's jewels (paid + free)
int32_t jewels(uint32_t userPoint) {
    const SafeBuffer *point = rw::host_ptr<SafeBuffer>(userPoint);
    int32_t freeJewels = point->get32(USERPOINT_FREE_JEWELS);
    return int32_t(uint32_t(point->get32(USERPOINT_PAID_JEWELS)) + uint32_t(freeJewels));
}
}  // namespace

// ------------------------------------------------------------------------------------------------ dialogs

void SceneUtil::openOkDialog4Line(Addr message, Addr callback, Addr okLabel) {
    openDialog(BUTTONS_OK, WINDOW_4_LINES, 0, message, callback, okLabel, 0, false);
}
RW_FUNCTION(0x45C774, "SceneUtil::openOkDialog4Line", SceneUtil::openOkDialog4Line);

void SceneUtil::openOkDialog4LineSetLayer(Addr layer, Addr message, Addr callback, Addr okLabel) {
    uint32_t dialog = call(PopupDialog_create, {BUTTONS_OK, WINDOW_4_LINES});
    if (okLabel) call(PopupDialog_setOkLabel, {dialog, okLabel});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, callback});
    call(PopupDialog_open, {dialog, layer, message, cb});
    functionDestroy(cb);
    call(fn::AudioPlaySystemSEPopUp, {});
}
RW_FUNCTION(0x45C880, "SceneUtil::openOkDialog4LineSetLayer", SceneUtil::openOkDialog4LineSetLayer);

void SceneUtil::openOkCancelDialog4Line(Addr message, Addr callback, Addr okLabel, Addr cancelLabel) {
    openDialog(BUTTONS_OK_CANCEL, WINDOW_4_LINES, 0, message, callback, okLabel, cancelLabel, true);
}
RW_FUNCTION(0x45C950, "SceneUtil::openOkCancelDialog4Line", SceneUtil::openOkCancelDialog4Line);

void SceneUtil::openOkDialog8Line(Addr message, Addr callback, Addr okLabel) {
    openDialog(BUTTONS_OK, WINDOW_8_LINES, 0, message, callback, okLabel, 0, false);
}
RW_FUNCTION(0x45CA3C, "SceneUtil::openOkDialog8Line", SceneUtil::openOkDialog8Line);

void SceneUtil::openOkCancelDialog8Line(Addr message, Addr callback, Addr okLabel, Addr cancelLabel) {
    openDialog(BUTTONS_OK_CANCEL, WINDOW_8_LINES, 0, message, callback, okLabel, cancelLabel, true);
}
RW_FUNCTION(0x45CB14, "SceneUtil::openOkCancelDialog8Line", SceneUtil::openOkCancelDialog8Line);

// The AP-recovery popup, with a (bool, bool) callback adapted to the popup's (bool) one.
void SceneUtil::openChargeApDialog(Addr callback) {
    Frame f;
    uint32_t cbCopy = f.alloc(20);
    call(FunctionBB_copy, {cbCopy, callback});
    // the lambda [cb = cbCopy](bool) {...}, built in a stack temporary, then moved into a heap __func
    uint32_t captured = f.alloc(24);
    functionCopy(captured, cbCopy);
    uint32_t adapter = f.alloc(20);
    st32(adapter + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x20});
    if (heap) {
        st32(heap, VT_ChargeApDialog_3);
        call(fn::function_vbb_move, {heap + 8, captured});
    }
    st32(adapter + 16, heap);
    openChargeApPopup(adapter);
    functionDestroy(adapter);
    functionDestroy(captured);
    functionDestroy(cbCopy);
}
RW_FUNCTION(0x45CC00, "SceneUtil::openChargeApDialog", SceneUtil::openChargeApDialog);

// "Recover AP with a jewel?" popup: the recovery amount, the time to the next AP, the jewels owned.
void SceneUtil::openChargeApPopup(Addr callback) {
    Frame f;
    // AP recovered by one jewel (Misc table)
    uint32_t misc = f.alloc(8);
    call(fn::Misc_create, {misc, MISC_AP_RECOVERY});
    int32_t recovery = rw::host_ptr<SafeBuffer>(ld32(misc))->get32(4);
    sharedRelease(misc);

    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t userPointCtl = ld32(api + APIManager_userPoint + 4);
    uint32_t userPoint = ld32(api + APIManager_userPoint);
    if (userPointCtl) call(fn::shared_add, {userPointCtl});

    PopupBase base = loadPopup(STR_PopupApRecoveryJson);
    uint32_t layer = base.layer, shadow = base.shadow, popup = base.popup;

    // message: recovery amount
    uint32_t wording = findNode(popup, STR_Txt_Wording, 11);
    uint32_t viewName = newStr(f, STR_Txt_Wording, 11);
    uint32_t view = textToCustomTextView(wording, viewName);
    strFree(viewName);
    {
        Frame g;
        uint32_t text = textData(g, 102600018);
        uint32_t buf = stringBuffer(g, {textDataStr(text), uint32_t(recovery)});
        uint32_t xml = newStr(g, buf);
        call(fn::CustomTextView_setXml, {view, xml});
        strFree(xml);
        sharedRelease(text);
    }
    uint32_t wording2 = findNode(popup, STR_Txt_Wording2, 12);
    setTextId(wording2, 102600047);
    setShadow(wording2);

    // time to the next AP (mm:ss)
    uint32_t apOut = f.alloc(4);
    call(AppState_getCurrentAP, {call(fn::AppState_getInstance, {}), apOut});
    int32_t ap = int32_t(ld32(apOut));
    uint32_t recoveryText = findNode(popup, STR_Txt_AP_Recovery, 15);
    st32(sceneManager() + SceneManager_apRecoveryText, recoveryText);
    recoveryText = ld32(sceneManager() + SceneManager_apRecoveryText);
    {
        Frame g;
        uint32_t text = textData(g, 102600051);
        uint32_t buf = stringBuffer(g, {textDataStr(text), uint32_t(ap / 60), uint32_t(ap % 60)});
        setTextC(recoveryText, buf);
        sharedRelease(text);
    }
    setShadow(ld32(sceneManager() + SceneManager_apRecoveryText));

    // jewels owned
    uint32_t crown = findNode(popup, STR_Txt_CrownPossessed, 18);
    setShadow(crown);
    setTextId(crown, 100310002);
    uint32_t crownCount = findNode(popup, STR_Txt_CrownPossessed_Label, 24);
    setShadow(crownCount);
    {
        Frame g;
        uint32_t buf = stringBuffer(g, {STR_7d, uint32_t(jewels(userPoint))});
        setTextC(crownCount, buf);
    }

    uint32_t ok = findNode(popup, STR_Button_OK, 9);
    uint32_t close = findNode(popup, STR_Button_Close, 12);
    addTouch(ok, VT_ChargeApPopup_OkTouch, {});
    titleLabel(ok, STR_Txt_OK, 6, 100100001);
    addTouch(close, VT_ChargeApPopup_CloseTouch, {});
    titleLabel(close, STR_Txt_Cancel, 10, 100100002);

    // SceneManager::openPopup(canClose = [close], onClose = [callback, popup, layer, shadow, ok, close])
    uint32_t cbCopy = f.alloc(20);
    call(fn::function_vb_copy, {cbCopy, callback});
    uint32_t mgr = sceneManager();
    uint32_t canClose = newFunction(f, VT_ChargeApPopup_CanClose, {close});
    uint32_t captured = f.alloc(24);
    functionCopy(captured, cbCopy);
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x38});
    if (heap) {
        st32(heap, VT_ChargeApPopup_OnClose);
        call(fn::function_vb_move, {heap + 8, captured});
        st32(heap + 0x2C, ok);
        st32(heap + 0x28, shadow);
        st32(heap + 0x24, layer);
        st32(heap + 0x20, popup);
        st32(heap + 0x30, close);
    }
    st32(onClose + 16, heap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(captured);
    functionDestroy(canClose);

    uint32_t none = newEmptyFunction(f);
    call(fn::Node_runAction, {popup, createOpenPopupAnimation(popup, none)});
    functionDestroy(none);
    functionDestroy(cbCopy);
    if (userPointCtl) call(fn::shared_release, {userPointCtl});
}
RW_FUNCTION(0x45CD88, "SceneUtil::openChargeApPopup", SceneUtil::openChargeApPopup);

// The text node `name` under `root`, with a shadow and the TextData text `textId`; returns the text node.
SceneUtil::Addr SceneUtil::settingTitleLabel(Addr root, Addr name, int32_t textId) {
    Frame f;
    uint32_t key = newStr(f, strData(name));
    uint32_t text = searchNodeByName(root, key);
    strFree(key);
    setShadow(text);
    setTextId(text, textId);
    return text;
}
RW_FUNCTION(0x45D8D0, "SceneUtil::settingTitleLabel", SceneUtil::settingTitleLabel);

// does the user own at least `count` jewels
bool SceneUtil::hasMagicStone(int32_t count) {
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctl = ld32(api + APIManager_userPoint + 4);
    uint32_t userPoint = ld32(api + APIManager_userPoint);
    if (ctl) call(fn::shared_add, {ctl});
    int32_t owned = jewels(userPoint);
    if (ctl) call(fn::shared_release, {ctl});
    return owned >= count;
}
RW_FUNCTION(0x45DD28, "SceneUtil::hasMagicStone", SceneUtil::hasMagicStone);

// ------------------------------------------------------------------------------------------------ continue

namespace {
constexpr uint32_t MISC_CONTINUE_COST = 113;             // master Misc row: jewels to continue a quest
constexpr uint32_t VT_ContinueDialog_8 = 0xF54FDC;        // wraps the (bool,bool) callback for openContinuPopup
constexpr uint32_t VT_ContinueDialog_9 = 0xF54FB0;        // wraps it for the "not enough jewels" dialog
constexpr uint32_t VT_ContinuPopup_OkTouch = 0xF54F84;    // $_10
constexpr uint32_t VT_ContinuPopup_CloseTouch = 0xF54F58; // $_11
constexpr uint32_t VT_ContinuPopup_CanClose = 0xF54F2C;   // $_12 (bool())
constexpr uint32_t VT_ContinuPopup_OnClose = 0xF54ED4;    // $_13 (void(bool)), heap stored
constexpr uint32_t STR_PopupContinueJson = 0xDF86B1;      // "cocostudio/publish/PopupNormal_Continue.json"
constexpr uint32_t STR_d = 0xDF86DE;                      // "%d"

int32_t miscValue(uint32_t id) {
    Frame f;
    uint32_t misc = f.alloc(8);
    call(fn::Misc_create, {misc, id});
    int32_t v = rw::host_ptr<SafeBuffer>(ld32(misc))->get32(4);
    sharedRelease(misc);
    return v;
}
}  // namespace

// Continue a failed quest: the jewel popup when the user can pay, else a "not enough jewels" dialog.
void SceneUtil::openContinueDialog(Addr callback) {
    int32_t cost = miscValue(MISC_CONTINUE_COST);
    Frame f;
    if (hasMagicStone(cost) == 1) {
        uint32_t cbCopy = f.alloc(20);
        call(FunctionBB_copy, {cbCopy, callback});
        uint32_t captured = f.alloc(24);
        functionCopy(captured, cbCopy);
        uint32_t adapter = f.alloc(20);
        heapFunction(adapter, VT_ContinueDialog_8, 0x20, 8, fn::function_vbb_move, captured);
        openContinuPopup(adapter);
        functionDestroy(adapter);
        functionDestroy(captured);
        functionDestroy(cbCopy);
    } else {
        uint32_t cbCopy = f.alloc(20);
        call(FunctionBB_copy, {cbCopy, callback});
        uint32_t text = textData(f, 102600016);
        uint32_t message = newStr(f, textDataStr(text));
        uint32_t captured = f.alloc(24);
        functionCopy(captured, cbCopy);
        uint32_t adapter = f.alloc(20);
        heapFunction(adapter, VT_ContinueDialog_9, 0x20, 8, fn::function_vbb_move, captured);
        openOkCancelDialog4Line(message, adapter, 0, 0);
        functionDestroy(adapter);
        functionDestroy(captured);
        strFree(message);
        sharedRelease(text);
        functionDestroy(cbCopy);
    }
}
RW_FUNCTION(0x45D9F0, "SceneUtil::openContinueDialog", SceneUtil::openContinueDialog);

// "Continue with jewels?" popup: the cost and the jewels owned.
void SceneUtil::openContinuPopup(Addr callback) {
    int32_t cost = miscValue(MISC_CONTINUE_COST);
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t userPointCtl = ld32(api + APIManager_userPoint + 4);
    uint32_t userPoint = ld32(api + APIManager_userPoint);
    if (userPointCtl) call(fn::shared_add, {userPointCtl});
    PopupBase base = loadPopup(STR_PopupContinueJson);
    uint32_t popup = base.popup;
    Frame f;

    uint32_t wording = findNode(popup, STR_Txt_Wording, 11);
    uint32_t viewName = newStr(f, STR_Txt_Wording, 11);
    uint32_t view = textToCustomTextView(wording, viewName);
    strFree(viewName);
    {
        Frame g;
        uint32_t text = textData(g, 103100005);
        uint32_t buf = stringBuffer(g, {textDataStr(text), uint32_t(cost)});
        uint32_t xml = newStr(g, buf);
        call(fn::CustomTextView_setXml, {view, xml});
        strFree(xml);
        sharedRelease(text);
    }
    uint32_t wording2 = findNode(popup, STR_Txt_Wording2, 12);
    setTextId(wording2, 103100039);
    setShadow(wording2);
    uint32_t crown = findNode(popup, STR_Txt_CrownPossessed, 18);
    setTextId(crown, 100310002);
    setShadow(crown);
    uint32_t crownCount = findNode(popup, STR_Txt_CrownPossessed_Label, 24);
    {
        Frame g;
        uint32_t buf = stringBuffer(g, {STR_d, uint32_t(jewels(userPoint))});
        setTextC(crownCount, buf);
    }
    setShadow(crownCount);

    uint32_t ok = findNode(popup, STR_Button_OK, 9);
    addTouch(ok, VT_ContinuPopup_OkTouch, {});
    titleLabel(ok, STR_Txt_OK, 6, 100100001);
    uint32_t close = findNode(popup, STR_Button_Close, 12);
    addTouch(close, VT_ContinuPopup_CloseTouch, {});
    titleLabel(close, STR_Txt_Cancel, 10, 100100002);
    {
        Frame g;
        uint32_t none = newEmptyFunction(g);
        call(fn::Node_runAction, {popup, createOpenPopupAnimation(popup, none)});
        functionDestroy(none);
    }

    // SceneManager::openPopup(canClose, onClose = [popup, layer, shadow, callback])
    uint32_t mgr = sceneManager();
    uint32_t canClose = newFunction(f, VT_ContinuPopup_CanClose, {});
    uint32_t captured = f.alloc(20);
    call(fn::function_vb_copy, {captured, callback});
    uint32_t onClose = f.alloc(20);
    uint32_t heap = heapFunction(onClose, VT_ContinuPopup_OnClose, 0x30, 0x18, fn::function_vb_move, captured);
    if (heap) {
        st32(heap + 8, popup);
        st32(heap + 12, base.layer);
        st32(heap + 16, base.shadow);
    }
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(captured);
    functionDestroy(canClose);
    if (userPointCtl) call(fn::shared_release, {userPointCtl});
}
RW_FUNCTION(0x45DDE0, "SceneUtil::openContinuPopup", SceneUtil::openContinuPopup);

// ================================================================================================ the lambdas
namespace {
namespace dialogs {
constexpr uint32_t FunctionBB_call = 0x48B159;             // std::function<void(bool,bool)>::operator()
constexpr uint32_t SceneUtil_openContinueDialog = 0x45D9F1; // (function<void(bool,bool)> by value)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;
constexpr uint32_t VT_ContinueJewels = 0xF55008;           // $_8's dialog answer {callback (bool,bool)} (heap 0x20)
constexpr uint32_t VT_ContinuPopupEnd = 0xF54F00, VT_ChargeApPopupEnd = 0xF55060;   // {layer, shadow, popup}
constexpr int32_t TXT_CONTINUE_JEWELS = 103100007;

// $_13 (openContinuPopup closed) {popup, layer, shadow, -, callback (bool) +0x18}(ok): the popup shrinks, the
// shadow fades, callback(ok)
void continuPopupClosed(uint32_t f, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t popup = ld32(f + 8);
    {
        Frame fr;
        const uint32_t end = newFunction(fr, VT_ContinuPopupEnd, {ld32(f + 0xC), ld32(f + 0x10), popup});
        shrinkPopup(popup, end, ld32(f + 0x10));
    }
    call(fn::function_vb_call, {f + 0x18, ok != 0});
}
// $_7 (openChargeApPopup closed) {-, callback (bool) +8, popup, layer, shadow, OK, Close}(ok): callback(ok), the
// decide / cancel sound, the popup shrinks, the shadow fades, both buttons off
void chargeApPopupClosed(uint32_t f, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(fn::function_vb_call, {f + 8, ok});
    call(ok ? fn::AudioPlaySystemSEDecide : fn::AudioPlaySystemSECancel, {});
    const uint32_t popup = ld32(f + 0x20);
    {
        Frame fr;
        const uint32_t end = newFunction(fr, VT_ChargeApPopupEnd, {ld32(f + 0x24), ld32(f + 0x28), popup});
        shrinkPopup(popup, end, ld32(f + 0x28));
    }
    vcall(ld32(f + 0x2C), node::setTouchEnabled, {ld32(f + 0x2C), 0});
    vcall(ld32(f + 0x30), node::setTouchEnabled, {ld32(f + 0x30), 0});
}
// $_8 (openContinueDialog's answer) {-, callback (bool,bool) +8}(ok): ok -> callback(true, false); else "use
// jewels to continue?" (its answer gets the callback)
void continueAnswered(uint32_t f, uint32_t okRef) {
    if (ld8(okRef)) {
        call(FunctionBB_call, {f + 8, 1, 0});
        return;
    }
    Frame fr;
    const uint32_t td = textData(fr, TXT_CONTINUE_JEWELS);
    const uint32_t message = newStr(fr, textDataStr(td));
    const uint32_t cb = fr.alloc(24);
    call(FunctionBB_copy, {cb, f + 8});
    const uint32_t answer = fr.alloc(24);
    st32(answer + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x20});
    if (h) {
        st32(h, VT_ContinueJewels);
        call(fn::function_vbb_move, {h + 8, cb});
    }
    st32(answer + 16, h);
    call(SceneUtil_openOkCancelDialog4Line, {message, answer, 0, 0});
    functionDestroy(answer);
    functionDestroy(cb);
    strFree(message);
    sharedRelease(td);
}
// $_8's answer {-, callback (bool,bool) +8}(yes): yes -> callback(false, false); no -> the continue dialog again
void continueJewelsAnswered(uint32_t f, uint32_t yesRef) {
    if (ld8(yesRef)) {
        call(FunctionBB_call, {f + 8, 0, 0});
        return;
    }
    Frame fr;
    const uint32_t cb = fr.alloc(24);
    call(FunctionBB_copy, {cb, f + 8});
    call(SceneUtil_openContinueDialog, {cb});
    functionDestroy(cb);
}
// openChargeApPopup $_135's AP countdown end {layer, a, b}: the countdown label forgotten, a and b removed
void apCountdownEnd(uint32_t f) {
    st32(sceneManager() + SceneManager_apRecoveryText, 0);
    vcall(ld32(f + 4), node::removeChild, {ld32(f + 4), ld32(f + 8), 1});
    vcall(ld32(f + 4), node::removeChild, {ld32(f + 4), ld32(f + 0xC), 1});
}
}  // namespace dialogs
}  // namespace

RW_FUNCTION(0x48B3A8, "SceneUtil::openContinuPopup $_13::operator()(bool) (closed)", dialogs::continuPopupClosed);
RW_FUNCTION(0x48BF80, "SceneUtil::openChargeApPopup $_7::operator()(bool) (closed)", dialogs::chargeApPopupClosed);
RW_FUNCTION(0x48BA34, "SceneUtil::openContinueDialog $_8::operator()(bool)", dialogs::continueAnswered);
RW_FUNCTION(0x48BD38, "SceneUtil::openContinueDialog $_8 lambda(bool)#1::operator()", dialogs::continueJewelsAnswered);
RW_FUNCTION(0x48C126, "SceneUtil::openChargeApPopup $_135 lambda()#1::operator()", dialogs::apCountdownEnd);

namespace {
namespace chargeap {
constexpr uint32_t Misc_create = 0x786D5D, SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_hasMagicStone = 0x45DD29;          // (jewels needed) -> 1 when the player has them
constexpr uint32_t APIManager_executeShopApRecover = 0x4CDF39;  // (api, function<void(bool)>)
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951, SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t VT_Recovered = 0xF55168;      // $_3's request answer {callback (bool,bool)} (heap 0x20)
constexpr uint32_t VT_NoJewels = 0xF5513C;       // the "not enough jewels" dialog's answer (heap 0x20)
constexpr uint32_t VT_RecoveredOk = 0xF55194;    // the "AP recovered" dialog's answer (heap 0x20)
constexpr uint32_t VT_RecoverError = 0xF551C0;
constexpr uint32_t SE_ERROR = 509000180;
constexpr int32_t TXT_NO_JEWELS = 102600016, TXT_RECOVERED = 102600019;

// a heap __func `vt` {callback (bool,bool) moved from `cb`} into `answer`
void callbackAnswer(uint32_t answer, uint32_t vt, uint32_t cb) {
    st32(answer + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x20});
    if (h) {
        st32(h, vt);
        call(fn::function_vbb_move, {h + 8, cb});
    }
    st32(answer + 16, h);
}

// $_3 (the AP popup closed) {-, callback (bool,bool) +8}(ok): ok -> with the jewels (master Misc 112): the AP
// recovered (its answer gets the callback); without: the error sound and "not enough jewels" (likewise)
void popupClosed(uint32_t f, uint32_t okRef) {
    if (!ld8(okRef)) return;
    uint32_t price;
    {
        Frame fr;
        const uint32_t row = fr.alloc(8), value = fr.alloc(4);
        call(Misc_create, {row, MISC_AP_RECOVERY});
        const uint32_t sb = ld32(row);
        call(SafeBuffer_read, {sb, value, ld32(sb + 0xC) + 4, 4, 4});
        price = ld32(value);
        sharedRelease(row);
    }
    Frame fr;
    const uint32_t cb = fr.alloc(24);
    if (call(SceneUtil_hasMagicStone, {price}) == 1) {
        call(FunctionBB_copy, {cb, f + 8});
        call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t tmp = fr.alloc(24), answer = fr.alloc(24);
        functionCopy(tmp, cb);
        callbackAnswer(answer, VT_Recovered, tmp);
        call(APIManager_executeShopApRecover, {api, answer});
        functionDestroy(answer);
        functionDestroy(tmp);
    } else {
        call(FunctionBB_copy, {cb, f + 8});
        playLayerSE(SE_ERROR);
        const uint32_t td = textData(fr, TXT_NO_JEWELS);
        const uint32_t message = newStr(fr, textDataStr(td));
        const uint32_t tmp = fr.alloc(24), answer = fr.alloc(24);
        functionCopy(tmp, cb);
        callbackAnswer(answer, VT_NoJewels, tmp);
        call(SceneUtil_openOkCancelDialog4Line, {message, answer, 0, 0});
        functionDestroy(answer);
        functionDestroy(tmp);
        strFree(message);
        sharedRelease(td);
    }
    functionDestroy(cb);
}
// $_3's request answer {-, callback (bool,bool) +8}(ok): the indicator off; ok -> the header updated and "AP
// recovered" (its answer gets the callback); else the API error
void recovered(uint32_t f, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        apiErrorDialogIfShown(VT_RecoverError);
        return;
    }
    call(SceneManager_updateHeader, {sceneManager()});
    Frame fr;
    const uint32_t td = textData(fr, TXT_RECOVERED);
    const uint32_t message = newStr(fr, textDataStr(td));
    const uint32_t cb = fr.alloc(24), answer = fr.alloc(24);
    call(FunctionBB_copy, {cb, f + 8});
    callbackAnswer(answer, VT_RecoveredOk, cb);
    call(SceneUtil_openOkDialog4Line, {message, answer, 0});
    functionDestroy(answer);
    functionDestroy(cb);
    strFree(message);
    sharedRelease(td);
}
}  // namespace chargeap
}  // namespace

RW_FUNCTION(0x48C4B0, "SceneUtil::openChargeApDialog $_3::operator()(bool)", chargeap::popupClosed);
RW_FUNCTION(0x48CB44, "SceneUtil::openChargeApDialog $_3 lambda(bool)#1::operator()", chargeap::recovered);
