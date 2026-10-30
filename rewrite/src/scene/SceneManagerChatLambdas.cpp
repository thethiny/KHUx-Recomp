// SceneManager's chat popups' lambdas (delete / check / block / unblock / report post, block list): the popups'
// close handlers (buttons off, the popup shrinks, the shadow fades), their ends (the popup removed, a result
// dialog), Cancel, and the chat requests with their answers. The shapes repeat from popup to popup, so the common
// steps are the helpers below; each lambda passes its own captures.
#include "Captures.hpp"
#include "ChatMessage.hpp"
#include "SceneUi.hpp"


using namespace scene;
using caps::Cap;

namespace {
namespace cl {
// SceneManager
constexpr uint32_t CHAT_REQUESTING = 0x46D, CHAT_TIMER = 0x474;
constexpr uint32_t CHAT_DELETED_IDS = 0x43C;   // vector<int>: messages deleted here (hidden from the list)
// callees
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1, SM_setChatMessageScrollView = 0x617475;
constexpr uint32_t ChatManager_getInstance = 0x4DB0E5;
constexpr uint32_t ChatManager_deleteMessage = 0x4DC365;    // (cm, message id, const function<void(StatusCode)>&)
constexpr uint32_t IntVector_pushSlow = 0x47D709;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C;
constexpr uint32_t TOUCH_ENDED = 2;
constexpr int32_t TXT_DELETED = 104200010, TXT_REQUEST_FAILED = 104200019;
constexpr int32_t TXT_CHAT_ERROR_1 = 105000001, TXT_CHAT_ERROR_2 = 105000002;

void setTouchEnabled(uint32_t w, uint32_t on) { vcall(w, V_SET_TOUCH_ENABLED, {w, on}); }
// a heap __func `vt` holding `words` (from +4) in `fo`
void heapWords(uint32_t fo, uint32_t vt, std::initializer_list<uint32_t> words) {
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {uint32_t(4 + 4 * words.size())});
    if (h) {
        st32(h, vt);
        uint32_t k = 4;
        for (uint32_t w : words) {
            st32(h + k, w);
            k += 4;
        }
    }
    st32(fo + 16, h);
}
// a chat request's error 1 / 2: not requesting, the two buttons on, the error text
void chatError(uint32_t me, uint32_t first, uint32_t second, int32_t text) {
    st8(me + CHAT_REQUESTING, 0);
    setTouchEnabled(first, 1);
    setTouchEnabled(second, 1);
    okDialogText(text);
}
// Cancel {button}(sender, type): the cancel sound, the button on again, the popup closed
void cancelTouched(uint32_t button, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    call(fn::AudioPlaySystemSECancel, {});
    setTouchEnabled(button, 1);
    call(SM_closeCurrentPopup, {sceneManager(), 0});
}
// a popup's end {layer, shadow, popup, -}: shadow and popup removed
void removePopup(uint32_t f) {
    vcall(ld32(f + 4), node::removeChild, {ld32(f + 4), ld32(f + 8), 1});
    vcall(ld32(f + 4), node::removeChild, {ld32(f + 4), ld32(f + 0xC), 1});
}

// ------------------------------------------------------------------------------------------------ delete post
constexpr uint32_t VT_DeleteClosedEnd = 0xF5FC3C;   // $_64's end {layer, shadow, popup, deleted}
constexpr uint32_t VT_DeleteAnswer = 0xF5FCEC;      // $_61's request answer $_60 (heap 0x60)

// $_64 (closed) {delete, close, this, popup, layer, shadow}(deleted): both buttons off, no request, no poll; the
// popup shrinks (then the end below) and the shadow fades
void deleteClosed(uint32_t f, uint32_t deletedRef) {
    const uint8_t deleted = ld8(deletedRef);
    setTouchEnabled(ld32(f + 4), 0);
    setTouchEnabled(ld32(f + 8), 0);
    const uint32_t me = ld32(f + 0xC);
    st8(me + CHAT_REQUESTING, 0);
    st32(me + CHAT_TIMER, 0);
    const uint32_t popup = ld32(f + 0x10);
    Frame fr;
    const uint32_t end = fr.alloc(20);
    heapWords(end, VT_DeleteClosedEnd, {ld32(f + 0x14), ld32(f + 0x18), popup, deleted});
    shrinkPopup(popup, end, ld32(f + 0x18));
}
// $_64's end {layer, shadow, popup, deleted}: removed; deleted -> "the post was deleted"
void deleteClosedEnd(uint32_t f) {
    removePopup(f);
    if (ld8(f + 0x10)) okDialogText(TXT_DELETED);
}
// $_59 (Cancel) {close}
void deleteCancel(uint32_t f, uint32_t, uint32_t typeRef) { cancelTouched(ld32(f + 4), typeRef); }

// $_61 (Delete) {message +8, close, this, delete, close, $_60's {this, message, close, delete, close}}(sender,
// type): an empty post or a request running -> the failure dialog; else the buttons off and the post deleted
void deleteTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 0x4C);
    if (!strSize(f + 8 + chatmsg::MSG_TEXT) || ld8(me + CHAT_REQUESTING)) {
        failedDialog(TXT_REQUEST_FAILED, ld32(f + 0x48));
        return;
    }
    st8(me + CHAT_REQUESTING, 1);
    st32(me + CHAT_TIMER, 0);
    setTouchEnabled(ld32(f + 0x50), 0);
    setTouchEnabled(ld32(f + 0x54), 0);
    const uint32_t cm = call(ChatManager_getInstance, {});
    const uint32_t messageId = ld32(f + 0x30);
    Frame fr;
    const uint32_t message = fr.alloc(chatmsg::MSG_SIZE);
    const uint32_t owner = ld32(f + 0x58);
    chatmsg::msgCopy(message, f + 0x60);
    const uint32_t w0 = ld32(f + 0xA0), w1 = ld32(f + 0xA4), w2 = ld32(f + 0xA8);
    const uint32_t answer = fr.alloc(24);
    st32(answer + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x60});
    if (h) {
        st32(h, VT_DeleteAnswer);
        st32(h + 8, owner);
        chatmsg::msgMove(h + 0x10, message);
        st32(h + 0x50, w0);
        st32(h + 0x54, w1);
        st32(h + 0x58, w2);
    }
    st32(answer + 16, h);
    const uint32_t ok = call(ChatManager_deleteMessage, {cm, messageId, answer});
    functionDestroy(answer);
    chatmsg::msgDestroy(message);
    if (!ok) failedDialog(TXT_REQUEST_FAILED, ld32(f + 0x48));
}

// $_60 (deleted) {-, this, message +0x10, close, delete, close}(status): 0 -> the post hidden (its id kept), the
// list redrawn, the popup closed; 1 / 2 -> the chat error; 3 -> nothing; else -> the failure dialog
void deleteAnswered(uint32_t f, uint32_t statusRef) {
    const uint32_t status = ld32(statusRef);
    const uint32_t me = ld32(f + 8);
    if (status > 3) {
        failedDialog(TXT_REQUEST_FAILED, ld32(f + 0x58));
        return;
    }
    if (status == 0) {
        const uint32_t id = ld32(f + 0x38);
        const uint32_t ids = me + CHAT_DELETED_IDS;
        const uint32_t end = ld32(ids + 4);
        if (end < ld32(ids + 8)) {
            uint32_t n = 0;
            if (end) {
                st32(end, id);
                n = ld32(ids + 4);
            }
            st32(ids + 4, n + 4);
        } else {
            Frame fr;
            const uint32_t v = fr.alloc(4);
            st32(v, id);
            call(IntVector_pushSlow, {ids, v});
        }
        call(SM_setChatMessageScrollView, {me});
        call(fn::AudioPlaySystemSECancel, {});
        setTouchEnabled(ld32(f + 0x50), 1);
        call(SM_closeCurrentPopup, {sceneManager(), 1});
    } else if (status == 1 || status == 2) {
        chatError(me, ld32(f + 0x54), ld32(f + 0x50), status == 1 ? TXT_CHAT_ERROR_1 : TXT_CHAT_ERROR_2);
    }
}

// ------------------------------------------------------------------------------------------------ check post
constexpr uint32_t VT_CheckClosedEnd = 0xF5FADC;    // $_70's end {layer, shadow, popup}
constexpr uint32_t VT_CheckPostDone = 0xF5FB8C;     // the block / report popup's callback {}
constexpr uint32_t SM_openBlockPostPopup = 0x61AACD, SM_openReportPopup = 0x61C355;   // (this, message, function)

// $_70 (closed) {popup, layer, shadow}: the popup shrinks (its end removes it) and the shadow fades
void checkClosed(uint32_t f) {
    const uint32_t popup = ld32(f + 4);
    Frame fr;
    const uint32_t end = newFunction(fr, VT_CheckClosedEnd, {ld32(f + 8), ld32(f + 0xC), popup});
    shrinkPopup(popup, end, ld32(f + 0xC));
}
// $_67 / $_66 (Block / Report) {-, this, -, message +0x10}(sender, type): the decide sound, then the block /
// report popup for the message (passed by value)
void postAction(uint32_t f, uint32_t typeRef, uint32_t open) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 8);
    call(fn::AudioPlaySystemSEDecide, {});
    Frame fr;
    const uint32_t message = fr.alloc(chatmsg::MSG_SIZE);
    chatmsg::msgCopy(message, f + 0x10);
    const uint32_t done = newFunction(fr, VT_CheckPostDone, {});
    call(open, {me, message, done});
    functionDestroy(done);
    chatmsg::msgDestroy(message);
}
void checkBlock(uint32_t f, uint32_t, uint32_t typeRef) { postAction(f, typeRef, SM_openBlockPostPopup); }
void checkReport(uint32_t f, uint32_t, uint32_t typeRef) { postAction(f, typeRef, SM_openReportPopup); }

// ------------------------------------------------------------------------------------------------ failures, report
constexpr uint32_t SM_DIALOG_LAYER = 0x3F8;         // SceneManager: the layer of the request dialogs
constexpr uint32_t SM_REPORT_CANCELED = 0x4A5;      // byte
constexpr uint32_t SceneUtil_openOkDialog4LineSetLayer = 0x45C881;   // (layer, const string&, function<void(bool)>, const char*)
constexpr uint32_t Widget_isTouchEnabled = rwlib::cocos2d::ui::Widget::isTouchEnabled;
constexpr uint32_t FunctionVI_call = 0x4F321D;      // std::function<void(int)>::operator()

// TextData `text` in an OK dialog (no callback) on the dialog layer
void layerOkDialog(int32_t text) {
    const uint32_t dialogs = ld32(sceneManager() + SM_DIALOG_LAYER);
    Frame fr;
    const uint32_t td = textData(fr, text);
    const uint32_t message = newStr(fr, textDataStr(td));
    const uint32_t none = newEmptyFunction(fr);
    call(SceneUtil_openOkDialog4LineSetLayer, {dialogs, message, none, 0});
    functionDestroy(none);
    strFree(message);
    sharedRelease(td);
}

// a chat request failed with the server's `message` {button(s), callback +8}: the cancel sound, the button(s) on
// again, the popup closed (`closeArg`), the callback, then the message in an OK dialog on the dialog layer
void messageFailed(uint32_t cap, uint32_t message, bool twoButtons, uint32_t closeArg) {
    call(fn::AudioPlaySystemSECancel, {});
    setTouchEnabled(ld32(cap), 1);
    if (twoButtons) setTouchEnabled(ld32(cap + 4), 1);
    call(SM_closeCurrentPopup, {sceneManager(), closeArg});
    call(fn::function_vv_call, {cap + 8});
    const uint32_t layer = ld32(sceneManager() + SM_DIALOG_LAYER);
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(SceneUtil_openOkDialog4LineSetLayer, {layer, message, none, 0});
    functionDestroy(none);
}
void reportFailed(uint32_t cap, uint32_t message) { messageFailed(cap, message, false, 0); }
void unblockFailed(uint32_t cap, uint32_t message) { messageFailed(cap, message, true, 1); }
void blockFailed(uint32_t cap, uint32_t message) { messageFailed(cap, message, true, 0); }

// openReportPopup $_80 (Cancel) {this, button}(sender, type): pressed -> canceled; released -> the cancel sound,
// the button on, the popup closed; touch cancelled -> not canceled
void reportCancel(uint32_t f, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    const uint32_t me = ld32(f + 4);
    if (type == 3) {
        st8(me + SM_REPORT_CANCELED, 0);
    } else if (type == TOUCH_ENDED) {
        call(fn::AudioPlaySystemSECancel, {});
        st8(me + SM_REPORT_CANCELED, 1);
        setTouchEnabled(ld32(f + 8), 1);
        call(SM_closeCurrentPopup, {sceneManager(), 0});
    } else if (type == 0) {
        st8(me + SM_REPORT_CANCELED, 1);
    }
}
// openReportSelectPopup $_88 {-, button, -, picked function<void(int)> +0x10}(reason): picked (the button on)
// -> the callback with it, the popup closed
void reportReasonPicked(uint32_t f, uint32_t reasonRef) {
    const uint32_t reason = ld32(reasonRef);
    if (call(Widget_isTouchEnabled, {ld32(f + 8)}) != 1) return;
    call(FunctionVI_call, {f + 0x10, reason});
    call(SM_closeCurrentPopup, {sceneManager(), 1});
}

// ------------------------------------------------------------------------------------------------ closers
constexpr uint32_t VT_BlockClosedEnd = 0xF5F97C, VT_UnblockClosedEnd = 0xF5F504;   // {layer, shadow, popup}
constexpr uint32_t VT_ReportClosedEnd = 0xF5F7F0;   // {x, layer, shadow, popup, this, 0} (heap 0x1C)
constexpr uint32_t VT_ReasonClosedEnd = 0xF5F714;   // {layer, shadow, popup}
constexpr uint32_t SM_REPORT_REASON = 0x424;        // SceneManager: the picked reason (0 based)
constexpr uint32_t SM_chatLineUpdate = 0x615729;    // (sret string, -, const string&, max length)
constexpr uint32_t Text_setTextAreaSize = rwlib::cocos2d::ui::Text::setTextAreaSize;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t V_EDIT_SET_TEXT = 0x2A8, V_EDIT_GET_TEXT = 0x2AC, V_TEXT_SLOT_2D4 = 0x2D4;

// a post popup closed {button, button, this, popup, layer, shadow}: both buttons off, no request, no poll, the
// popup shrinks (its end `vt` {layer, shadow, popup} removes it) and the shadow fades
void closedSmall(uint32_t f, uint32_t vt) {
    setTouchEnabled(ld32(f + 4), 0);
    setTouchEnabled(ld32(f + 8), 0);
    const uint32_t me = ld32(f + 0xC);
    st8(me + CHAT_REQUESTING, 0);
    st32(me + CHAT_TIMER, 0);
    const uint32_t popup = ld32(f + 0x10);
    Frame fr;
    const uint32_t end = newFunction(fr, vt, {ld32(f + 0x14), ld32(f + 0x18), popup});
    shrinkPopup(popup, end, ld32(f + 0x18));
}
void blockClosed(uint32_t f) { closedSmall(f, VT_BlockClosedEnd); }
void unblockClosed(uint32_t f) { closedSmall(f, VT_UnblockClosedEnd); }
// openReportPopup $_86 (closed) {report, close, this, popup, x, layer, shadow}: as closedSmall, the end on the
// heap {x, layer, shadow, popup, this, 0}
void reportClosed(uint32_t f) {
    setTouchEnabled(ld32(f + 4), 0);
    setTouchEnabled(ld32(f + 8), 0);
    const uint32_t me = ld32(f + 0xC);
    st8(me + CHAT_REQUESTING, 0);
    st32(me + CHAT_TIMER, 0);
    const uint32_t popup = ld32(f + 0x10);
    Frame fr;
    const uint32_t end = fr.alloc(20);
    heapWords(end, VT_ReportClosedEnd, {ld32(f + 0x14), ld32(f + 0x18), ld32(f + 0x1C), popup, me, 0});
    shrinkPopup(popup, end, ld32(f + 0x1C));
}
// openReportSelectPopup $_90 (closed) {-, picked function<void(int)> +8, popup, layer, shadow, button}(ok): not
// picked -> picked(-1); the popup shrinks, the shadow fades, the button off
void reasonClosed(uint32_t f, uint32_t okRef) {
    if (!ld8(okRef)) call(FunctionVI_call, {f + 8, 0xFFFFFFFFu});
    const uint32_t popup = ld32(f + 0x20);
    {
        Frame fr;
        const uint32_t end = newFunction(fr, VT_ReasonClosedEnd, {ld32(f + 0x24), ld32(f + 0x28), popup});
        shrinkPopup(popup, end, ld32(f + 0x28));
    }
    setTouchEnabled(ld32(f + 0x2C), 0);
}
// openReportPopup $_138 {report, close, this, label, reason texts...}(reason): both buttons on, not canceled; a
// reason (1 based) kept and its text shown
void reportReasonShown(uint32_t f, uint32_t reasonRef) {
    const int32_t reason = int32_t(ld32(reasonRef));
    const uint32_t me = ld32(f + 0xC);
    setTouchEnabled(ld32(f + 4), 1);
    setTouchEnabled(ld32(f + 8), 1);
    st8(me + SM_REPORT_CANCELED, 0);
    if (reason < 0) return;
    st32(me + SM_REPORT_REASON, uint32_t(reason - 1));
    setTextId(ld32(f + 0x10), int32_t(ld32(f + 0x14 + 4 * uint32_t(reason - 1))));
}
// openReportPopup $_79 (the comment edited) {edit, -, -, max length, label}: the text cut to the allowed lines,
// shown in the label (470 x 131)
void reportTextChanged(uint32_t f) {
    const uint32_t edit = ld32(f + 4);
    const uint32_t setText = ld32(ld32(edit) + V_EDIT_SET_TEXT);
    const uint32_t text = vcall(edit, V_EDIT_GET_TEXT, {edit});
    Frame fr;
    {
        const uint32_t line = fr.alloc(12);
        call(SM_chatLineUpdate, {line, text, text, ld32(f + 0xC)});
        call(setText, {edit, line});
        strFree(line);
    }
    const uint32_t label = ld32(f + 0x10);
    vcall(label, V_TEXT_SLOT_2D4, {label, 0});
    call(fn::Text_setString, {ld32(f + 0x10), vcall(ld32(f + 4), V_EDIT_GET_TEXT, {ld32(f + 4)})});
    const uint32_t size = fr.alloc(8), size2 = fr.alloc(8);
    call(Size_ctor, {size, fbits(470.0f), fbits(131.0f)});
    call(Text_setTextAreaSize, {ld32(f + 0x10), size});
    const uint32_t l2 = ld32(f + 0x10);
    call(Size_ctor, {size2, fbits(470.0f), fbits(131.0f)});
    vcall(l2, node::setContentSize, {l2, size2});
}

// ------------------------------------------------------------------------------------------------ block list, report end
constexpr uint32_t VT_BlockListClosedEnd = 0xF5F638;   // {layer, shadow, popup}
constexpr uint32_t VT_ReportSelectPicked = 0xF5F8CC;   // $_138 {report, close, this, label, 7 reason texts} (heap 0x30)
constexpr uint32_t SM_BLOCK_UNCHECKED = 0x460;         // SceneManager: vector<bool> {words, size}, a row unchecked
constexpr uint32_t SM_openReportSelectPopup = 0x61D6E5;   // (-, reason, function<void(int)>)
constexpr int32_t TXT_REPORTED = 104200017;

// the block list closed {this, list, this, popup, layer, shadow}(changed): unchanged -> every row checked again;
// the list off, no request, no poll, the popup shrinks (its end removes it) and the shadow fades
void blockListClosed(uint32_t f, uint32_t changedRef) {
    if (!ld8(changedRef)) {
        const uint32_t bits = ld32(f + 4) + SM_BLOCK_UNCHECKED;
        for (uint32_t i = 0; i < ld32(bits + 4); i++) {
            const uint32_t w = ld32(bits) + ((i >> 3) & 0x1FFFFFFC);
            st32(w, ld32(w) | (1u << (i & 0x1F)));
        }
    }
    setTouchEnabled(ld32(f + 8), 0);
    const uint32_t me = ld32(f + 0xC);
    st8(me + CHAT_REQUESTING, 0);
    st32(me + CHAT_TIMER, 0);
    const uint32_t popup = ld32(f + 0x10);
    Frame fr;
    const uint32_t end = newFunction(fr, VT_BlockListClosedEnd, {ld32(f + 0x14), ld32(f + 0x18), popup});
    shrinkPopup(popup, end, ld32(f + 0x18));
}
// openReportPopup's close end {layer, a, b, c, this, reported}: a, b, c removed from the layer, not canceled;
// reported -> "thank you for the report" on the dialog layer
void reportClosedEnd(uint32_t f) {
    const uint32_t layer = ld32(f + 4), me = ld32(f + 0x14);
    vcall(layer, node::removeChild, {layer, ld32(f + 8), 1});
    vcall(ld32(f + 4), node::removeChild, {ld32(f + 4), ld32(f + 0xC), 1});
    vcall(ld32(f + 4), node::removeChild, {ld32(f + 4), ld32(f + 0x10), 1});
    st8(me + SM_REPORT_CANCELED, 0);
    if (ld8(f + 0x18)) layerOkDialog(TXT_REPORTED);
}
// openReportPopup $_81 (Reason) {this, report, close, label + 7 reason texts +0x10..}(sender, type): pressed ->
// canceled off; released -> the decide sound, both buttons off, canceled, the reason select popup (its answer
// $_138 gets the captures); touch cancelled -> not canceled
void reportReasonTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    const uint32_t me = ld32(f + 4);
    if (type == 3) {
        st8(me + SM_REPORT_CANCELED, 0);
        return;
    }
    if (type != TOUCH_ENDED) {
        if (type == 0) st8(me + SM_REPORT_CANCELED, 1);
        return;
    }
    call(fn::AudioPlaySystemSEDecide, {});
    setTouchEnabled(ld32(f + 8), 0);
    setTouchEnabled(ld32(f + 0xC), 0);
    st8(me + SM_REPORT_CANCELED, 1);
    const uint32_t reason = ld32(me + SM_REPORT_REASON);
    Frame fr;
    const uint32_t picked = fr.alloc(24);
    heapWords(picked, VT_ReportSelectPicked, {ld32(f + 0x10), ld32(f + 0x14), ld32(f + 0x18), ld32(f + 0x1C),
                                              ld32(f + 0x20), ld32(f + 0x24), ld32(f + 0x28), ld32(f + 0x2C),
                                              ld32(f + 0x30), ld32(f + 0x34), ld32(f + 0x38)});
    call(SM_openReportSelectPopup, {ld32(picked + 16), reason, picked});   // r0: the new object (unused)
    functionDestroy(picked);
}

// ------------------------------------------------------------------------------------------------ block / unblock
// The block request's captures (0xB8; $_75 holds them at +0x58, its answer at +8): the SceneManager, the message,
// then {words, function} pairs, the failure pair {button, button, function} and the closing buttons.
namespace blockcap {
constexpr uint32_t OWNER = 0x00, MSG = 0x08, W3 = 0x48, W_B = 0x90, W_C = 0xB0, SIZE = 0xB8;
constexpr caps::Layout LAYOUT = {{Cap::Words, OWNER, 4}, {Cap::Msg, MSG, 0}, {Cap::Words, W3, 12}, {Cap::Fn, 0x58, 0},
                              {Cap::Words, 0x70, 8}, {Cap::Fn, 0x78, 0}, {Cap::Words, W_B, 8}, {Cap::Fn, 0x98, 0},
                              {Cap::Words, W_C, 8}};
}  // namespace blockcap
// The unblock request's (0x90; $_98 at +0x30, its answer at +8): the SceneManager and 3 words, a halfword, the
// user id, then the same pairs as above.
namespace unblockcap {
constexpr uint32_t OWNER = 0x00, USER = 0x14, W3 = 0x20, W_B = 0x68, W_C = 0x88, SIZE = 0x90;
constexpr caps::Layout LAYOUT = {{Cap::Words, OWNER, 16}, {Cap::Half, 0x10, 0}, {Cap::Str, USER, 0},
                              {Cap::Words, W3, 12}, {Cap::Fn, 0x30, 0}, {Cap::Words, 0x48, 8}, {Cap::Fn, 0x50, 0},
                              {Cap::Words, W_B, 8}, {Cap::Fn, 0x70, 0}, {Cap::Words, W_C, 8}};
}  // namespace unblockcap
// the user fetch after a (un)block: {3 words, function, 2 words, function}, as the requests hold them from W3
constexpr caps::Layout USER_FETCH = {{Cap::Words, 0x00, 12}, {Cap::Fn, 0x10, 0}, {Cap::Words, 0x28, 8},
                                  {Cap::Fn, 0x30, 0}};
constexpr uint32_t USER_FETCH_SIZE = 0x48;

constexpr uint32_t VT_BlockAnswer = 0xF5FA00, VT_UnblockAnswer = 0xF5F588;    // the request answers
constexpr uint32_t VT_BlockUserGot = 0xF5FA2C, VT_UnblockUserGot = 0xF5F5B4;  // the user fetches' answers
constexpr uint32_t ChatManager_block = 0x4DC611;    // (cm, const char* user id, const function<void(StatusCode)>&)
constexpr uint32_t ChatManager_unblock = 0x4DC901;  // the same
constexpr uint32_t ChatManager_getUser = 0x4DB271;  // (cm, const function<...>&) -> bool
constexpr uint32_t StringVector_pushSlow = 0x48D891;
constexpr uint32_t BlockPost_failed = 0x62B189, UnblockPost_failed = 0x6261E9;   // blockFailed / unblockFailed above
constexpr uint32_t CHAT_BLOCKED_IDS = 0x448;        // SceneManager: vector<string>, users blocked here
constexpr uint32_t AEABI_MEMCMP = 0x40EF2C;         // memcmp (import)
constexpr int32_t TXT_BLOCK_BUSY = 104200021, TXT_UNBLOCK_BUSY = 104200022, TXT_FETCH_FAILED = 104200028;

// a request failed {button, button, callback}: TextData `id` through the popup's failure (`failed`, out of line)
void failedText(uint32_t failed, uint32_t cap, int32_t id) {
    Frame fr;
    const uint32_t td = textData(fr, id);
    const uint32_t message = newStr(fr, textDataStr(td));
    call(failed, {cap, message});
    strFree(message);
    sharedRelease(td);
}
// a chat request's error 1 / 2 on the dialog layer: not requesting, the two buttons on (the report: not
// canceled), the error text
void chatErrorOnLayer(uint32_t me, uint32_t first, uint32_t second, int32_t text, bool notCanceled = false) {
    st8(me + CHAT_REQUESTING, 0);
    setTouchEnabled(first, 1);
    setTouchEnabled(second, 1);
    if (notCanceled) st8(me + SM_REPORT_CANCELED, 0);
    layerOkDialog(text);
}
// the user fetched again (its answer `vt` gets the pairs at `pairs`); not sent -> the failure (`failed` on `cap`)
void fetchUser(uint32_t vt, uint32_t pairs, uint32_t failed, uint32_t cap) {
    const uint32_t cm = call(ChatManager_getInstance, {});
    const uint32_t got = caps::sendWith(USER_FETCH, USER_FETCH_SIZE, vt, pairs,
                                      [&](uint32_t answer) { return call(ChatManager_getUser, {cm, answer}); });
    if (!got) failedText(failed, cap, TXT_FETCH_FAILED);
}

// $_75 / $_98 (Block / Unblock) {-, -, this, button, button, ..., user id string, captures, failure cap}(sender,
// type): a request running -> the failure; else requesting, the buttons off, the loading indicator, the request
// (its answer gets the captures); not sent -> the indicator hidden and the failure
struct PostRequest {
    uint32_t userId, caps;
    caps::Layout layout;
    uint32_t capsSize, answerVt, request, failed, failCap;
    int32_t busyText;
};
void postRequest(uint32_t f, uint32_t typeRef, const PostRequest &r) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 8);
    if (ld8(me + CHAT_REQUESTING)) {
        failedText(r.failed, f + r.failCap, r.busyText);
        return;
    }
    st8(me + CHAT_REQUESTING, 1);
    st32(me + CHAT_TIMER, 0);
    setTouchEnabled(ld32(f + 0xC), 0);
    setTouchEnabled(ld32(f + 0x10), 0);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t cm = call(ChatManager_getInstance, {});
    const uint32_t userId = strData(f + r.userId);
    const uint32_t sent = caps::sendWith(r.layout, r.capsSize, r.answerVt, f + r.caps,
                                       [&](uint32_t answer) { return call(r.request, {cm, userId, answer}); });
    if (sent) return;
    hideLoadingIndicator();
    failedText(r.failed, f + r.failCap, r.busyText);
}
void blockTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    postRequest(f, typeRef, {0x44, 0x58, blockcap::LAYOUT, blockcap::SIZE, VT_BlockAnswer, ChatManager_block,
                             BlockPost_failed, 0x110, TXT_BLOCK_BUSY});
}
void unblockTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    postRequest(f, typeRef, {0x24, 0x30, unblockcap::LAYOUT, unblockcap::SIZE, VT_UnblockAnswer,
                             ChatManager_unblock, UnblockPost_failed, 0xC0, TXT_UNBLOCK_BUSY});
}

// an answer's common part (captures `c`, status): the indicator hidden; 1 / 2 -> the chat error; else -> the
// failure; true for 0 (3 does nothing)
bool answerStatus(uint32_t c, uint32_t status, uint32_t wB, uint32_t wC, uint32_t failed, int32_t busyText) {
    hideLoadingIndicator();
    if (status > 3) {
        failedText(failed, c + wB, busyText);
        return false;
    }
    if (status == 1 || status == 2) {
        const uint32_t me = ld32(c);
        chatErrorOnLayer(me, ld32(c + wC), ld32(c + wC + 4), status == 1 ? TXT_CHAT_ERROR_1 : TXT_CHAT_ERROR_2);
        return false;
    }
    return status == 0;
}

// $_74 (blocked) {-, blockcap +8}(status): 0 -> the user kept as blocked, then fetched again
void blockAnswered(uint32_t f, uint32_t statusRef) {
    using namespace blockcap;
    const uint32_t c = f + 8;
    if (!answerStatus(c, ld32(statusRef), W_B, W_C, BlockPost_failed, TXT_BLOCK_BUSY)) return;
    const uint32_t ids = ld32(c + OWNER) + CHAT_BLOCKED_IDS;
    const uint32_t user = c + MSG + chatmsg::MSG_TEXT;
    const uint32_t end = ld32(ids + 4);
    if (end == ld32(ids + 8)) {
        call(StringVector_pushSlow, {ids, user});
    } else {
        if (end) strCopy(end, user);
        st32(ids + 4, ld32(ids + 4) + 12);
    }
    fetchUser(VT_BlockUserGot, c + W3, BlockPost_failed, c + W_B);
}

// whether the strings `a` (a copy) and `b` are equal, as the inline operator== compares them (memcmp for a long a)
bool strEqual(uint32_t a, uint32_t b) {
    uint32_t n = strSize(a);
    if (n != strSize(b)) return false;
    uint32_t pa = strData(a), pb = strData(b);
    if (strIsLong(a)) return call(AEABI_MEMCMP, {pa, pb, n}) == 0;
    for (; n; n--, pa++, pb++)
        if (ld8(pa) != ld8(pb)) return false;
    return true;
}
// vector<string>::erase(begin + index), inline: the later strings moved down, the last one destroyed
void eraseString(uint32_t vec, uint32_t index) {
    uint32_t p = ld32(vec) + 12 * index;
    for (; p + 12 != ld32(vec + 4); p += 12) strMoveAssign(p, p + 12);
    for (uint32_t end = ld32(vec + 4); end != p; end = ld32(vec + 4)) {
        st32(vec + 4, end - 12);
        strFree(end - 12);
    }
}
// $_97 (unblocked) {-, unblockcap +8}(status): 0 -> the user no longer kept as blocked, then fetched again
void unblockAnswered(uint32_t f, uint32_t statusRef) {
    using namespace unblockcap;
    const uint32_t c = f + 8;
    if (!answerStatus(c, ld32(statusRef), W_B, W_C, UnblockPost_failed, TXT_UNBLOCK_BUSY)) return;
    const uint32_t ids = ld32(c + OWNER) + CHAT_BLOCKED_IDS;
    if (ld32(ids + 4) != ld32(ids)) {
        Frame fr;
        const uint32_t copy = fr.alloc(12);
        for (uint32_t i = 0;;) {
            strCopy(copy, ld32(ids) + 12 * i);
            if (strEqual(copy, c + USER)) {
                eraseString(ids, i);
                strFree(copy);
                break;
            }
            strFree(copy);
            if (++i >= ((ld32(ids + 4) - ld32(ids)) >> 2) * 0xAAAAAAABu) break;
        }
    }
    fetchUser(VT_UnblockUserGot, c + W3, UnblockPost_failed, c + W_B);
}

// ------------------------------------------------------------------------------------------------ report post
// the report request's captures (0x48; $_81's at +0x58, its answer $_82 at +8): a word, the done function, the
// SceneManager, a button and the failure cap {button, button, function}
constexpr caps::Layout REPORT_CAPS = {{Cap::Words, 0x00, 4}, {Cap::Fn, 0x08, 0}, {Cap::Words, 0x20, 12},
                                   {Cap::Fn, 0x30, 0}};
constexpr uint32_t REPORT_CAPS_SIZE = 0x48;
constexpr uint32_t VT_ReportAnswer = 0xF5F874;
constexpr uint32_t ChatManager_report = 0x4DCBA9;   // (cm, target, reason (1 based), const char* comment, answer)
constexpr uint32_t SceneUtil_splitSpaceCheck = 0x47898D;   // (const string&) -> false when only blanks
constexpr uint32_t ReportPost_failed = 0x629599;    // sub_629598 (reportFailed above), called out of line
constexpr int32_t TXT_REPORT_BUSY = 104200023, TXT_REPORT_EMPTY = 104200033;

// the comment as the edit box holds it now (its getText)
uint32_t editText(uint32_t edit) { return vcall(edit, V_EDIT_GET_TEXT, {edit}); }

// $_81 (Report) {-, -, this, edit, report, close, ..., target +0x40, captures +0x58, failure cap +0xA0}(sender,
// type): pressed -> canceled; touch cancelled -> not canceled; released -> a request running: the failure; an
// empty or blank comment: "enter a comment"; else requesting, the buttons off, the report sent (its answer gets the
// captures); not sent -> the indicator hidden and the failure
void reportTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    const uint32_t me = ld32(f + 8);
    if (type == 3 || type == 0) {
        st8(me + SM_REPORT_CANCELED, type == 0);
        return;
    }
    if (type != TOUCH_ENDED) return;
    if (ld8(me + CHAT_REQUESTING)) {
        st8(me + SM_REPORT_CANCELED, 0);
        failedText(ReportPost_failed, f + 0xA0, TXT_REPORT_BUSY);
        return;
    }
    const uint32_t edit = ld32(f + 0xC);
    bool blank = !strSize(editText(edit));
    if (!blank && strSize(editText(edit))) blank = !call(SceneUtil_splitSpaceCheck, {editText(edit)});
    if (blank) {
        layerOkDialog(TXT_REPORT_EMPTY);
        st8(me + SM_REPORT_CANCELED, 0);
        return;
    }
    st8(me + CHAT_REQUESTING, 1);
    st32(me + CHAT_TIMER, 0);
    setTouchEnabled(ld32(f + 0x10), 0);
    setTouchEnabled(ld32(f + 0x14), 0);
    st8(me + SM_REPORT_CANCELED, 1);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t cm = call(ChatManager_getInstance, {});
    const uint32_t reason = ld32(me + SM_REPORT_REASON), target = ld32(f + 0x40);
    const uint32_t comment = strData(editText(edit));
    const uint32_t sent = caps::sendWith(REPORT_CAPS, REPORT_CAPS_SIZE, VT_ReportAnswer, f + 0x58, [&](uint32_t answer) {
        return call(ChatManager_report, {cm, target, reason + 1, comment, answer});
    });
    if (sent) return;
    hideLoadingIndicator();
    failedText(ReportPost_failed, f + 0xA0, TXT_REPORT_BUSY);
}

// $_82 (reported) {-, -, close, -, done function +0x10, this +0x28, report +0x2C, failure cap +0x30}(status): the
// indicator hidden; 0 -> the cancel sound, the button on, the popup closed, done(); 1 / 2 -> the chat error (not
// canceled); 3 -> nothing; else -> the failure
void reportAnswered(uint32_t f, uint32_t statusRef) {
    const uint32_t status = ld32(statusRef);
    hideLoadingIndicator();
    if (status > 3) {
        failedText(ReportPost_failed, f + 0x30, TXT_REPORT_BUSY);
    } else if (status == 0) {
        call(fn::AudioPlaySystemSECancel, {});
        setTouchEnabled(ld32(f + 8), 1);
        call(SM_closeCurrentPopup, {sceneManager(), 1});
        call(fn::function_vv_call, {f + 0x10});
    } else if (status != 3) {
        chatErrorOnLayer(ld32(f + 0x28), ld32(f + 0x2C), ld32(f + 8), status == 1 ? TXT_CHAT_ERROR_1 : TXT_CHAT_ERROR_2,
                         true);
    }
}

// ------------------------------------------------------------------------------------------------ block list
// SceneManager's chat window: the total / newest id, the first id of the page (total - 50), the next request's
// start, the first id seen, the new messages' count
constexpr uint32_t SM_CHAT_TOTAL = 0x408, SM_CHAT_START = 0x40C, SM_CHAT_FROM = 0x410, SM_CHAT_FIRST = 0x414;
constexpr uint32_t SM_CHAT_NEW = 0x41C;
constexpr uint32_t SM_BLOCKS = 0x454;               // vector<ChatManager::Block> (0x44 each: five strings first)
constexpr uint32_t BLOCK_SIZE = 0x44;
constexpr uint32_t Block_copy = 0x4DEB5F, BlockVector_pushSlow = 0x4DE87D, BoolVector_push = 0x62161D;

// ChatBlockListScrollView::ChatBlock (0x1C): the user number, checked, the Block's two last words as halfwords at
// +6, the name; a vector<ChatBlock> is {begin, end, cap}
constexpr uint32_t CB_SIZE = 0x1C, CB_CHECKED = 4, CB_WORDS = 6, CB_NAME = 0x10;
constexpr uint32_t BLOCK_USER = 0x18, BLOCK_WORDS = 0x3C;   // ChatManager::Block: the user number (a string)
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;       // sub_47CCE0 (vector::at's throw)
constexpr uint32_t std_stoi = 0xC93E8B;                     // (const string&, size_t*, base)
constexpr uint32_t ChatBlockVector_pushSlow = 0x627CD1, ChatBlockVector_copy = 0x61E6A5;
constexpr uint32_t ChatBlockVector_dtor = 0x55A989;
constexpr uint32_t ChatBlockListScrollView_setBlockList = 0x55B7A1;   // (view, vector<ChatBlock> by value)

// a ChatManager::Block's five strings destroyed (the inline destructor)
void blockDestroy(uint32_t b) {
    for (uint32_t s = 0x30;; s -= 0xC) {
        strFree(b + s);
        if (!s) break;
    }
}
// the block list shown {-, this, view}: no rows -> the cancel sound and the popup closed; else each Block as a
// ChatBlock (checked from the bits at +0x460) and the list handed to the view
void blockListShow(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    const uint32_t list = me + SM_BLOCKS;
    if (int32_t(ld32(list + 4) - ld32(list)) < 1) {
        call(fn::AudioPlaySystemSECancel, {});
        call(SM_closeCurrentPopup, {sceneManager(), 1});
        return;
    }
    Frame fr;
    const uint32_t rows = fr.alloc(12);
    st32(rows, 0);
    st32(rows + 4, 0);
    st32(rows + 8, 0);
    if (ld32(list + 4) != ld32(list)) {
        const uint32_t row = fr.alloc(CB_SIZE), block = fr.alloc(BLOCK_SIZE);
        const uint32_t bits = me + SM_BLOCK_UNCHECKED;
        for (uint32_t i = 0, off = 0;; off += BLOCK_SIZE) {
            if (((ld32(list + 4) - ld32(list)) >> 2) * 0xF0F0F0F1u <= i) call(Vector_throwOutOfRange, {list});
            call(Block_copy, {block, ld32(list) + off});
            if (ld32(bits + 4) <= i) call(Vector_throwOutOfRange, {bits});
            const uint32_t word = ld32(ld32(bits) + 4 * (i >> 5));
            st32(row, strSize(block + BLOCK_USER) ? call(std_stoi, {block + BLOCK_USER, 0, 10}) : 0);
            st8(row + CB_CHECKED, (word & (1u << (i & 0x1F))) ? 1 : 0);
            const uint32_t w0 = ld32(block + BLOCK_WORDS), w1 = ld32(block + BLOCK_WORDS + 4);
            rw::save<uint16_t>(row + CB_WORDS + 4, uint16_t(w1));
            rw::save<uint16_t>(row + CB_WORDS, uint16_t(w0));
            rw::save<uint16_t>(row + CB_WORDS + 6, uint16_t(w1 >> 16));
            rw::save<uint16_t>(row + CB_WORDS + 2, uint16_t(w0 >> 16));
            strCopy(row + CB_NAME, block);
            const uint32_t end = ld32(rows + 4);
            if (end == ld32(rows + 8)) {
                call(ChatBlockVector_pushSlow, {rows, row});
            } else {
                if (end) {
                    copyWords(end, row, 12);
                    rw::save<uint16_t>(end + 12, rw::load<uint16_t>(row + 12));
                    strCopy(end + CB_NAME, row + CB_NAME);
                }
                st32(rows + 4, ld32(rows + 4) + CB_SIZE);
            }
            strFree(row + CB_NAME);
            blockDestroy(block);
            if (++i >= ((ld32(list + 4) - ld32(list)) >> 2) * 0xF0F0F0F1u) break;
        }
    }
    const uint32_t view = ld32(f + 8);
    const uint32_t copy = fr.alloc(12);
    call(ChatBlockVector_copy, {copy, rows});
    call(ChatBlockListScrollView_setBlockList, {view, copy});
    call(ChatBlockVector_dtor, {copy});
    call(ChatBlockVector_dtor, {rows});
}

// the SceneManager's blocked users replaced by the server's `blocks` (vector<Block>), each one checked
void replaceBlocks(uint32_t me, uint32_t blocks) {
    // clear(): the rows destroyed from the back
    const uint32_t list = me + SM_BLOCKS;
    const uint32_t begin = ld32(list);
    for (uint32_t end = ld32(list + 4); end != begin; end = ld32(list + 4)) {
        st32(list + 4, end - BLOCK_SIZE);
        blockDestroy(end - BLOCK_SIZE);
    }
    st32(me + SM_BLOCK_UNCHECKED + 4, 0);
    Frame fr;
    const uint32_t yes = fr.alloc(4);
    if (ld32(blocks + 4) != ld32(blocks)) {
        for (uint32_t i = 0, off = 0;; off += BLOCK_SIZE) {
            const uint32_t src = ld32(blocks) + off;
            const uint32_t end = ld32(list + 4);
            if (end == ld32(list + 8)) {
                call(BlockVector_pushSlow, {list, src});
            } else {
                uint32_t next = 0;
                if (end) {
                    call(Block_copy, {end, src});
                    next = ld32(list + 4);
                }
                st32(list + 4, next + BLOCK_SIZE);
            }
            st8(yes, 1);
            call(BoolVector_push, {me + SM_BLOCK_UNCHECKED, yes});
            if (++i >= ((ld32(blocks + 4) - ld32(blocks)) >> 2) * 0xF0F0F0F1u) break;
        }
    }
}
// a block list fetched {-, this, button, button, -, done function<void()> +0x18, failure cap +0x30}(status,
// total, offset, blocks): 0 -> the counts kept, the list replaced (each row checked), redrawn, the cancel sound,
// the buttons on, the popup closed, done(); 1 / 2 -> the chat error; 3 -> nothing; else -> the failure
void blockListAnswer(uint32_t f, uint32_t statusRef, uint32_t totalRef, uint32_t offsetRef, uint32_t blocks,
                     uint32_t closeArg, uint32_t failed) {
    const uint32_t status = ld32(statusRef);
    if (status > 3) {
        failedText(failed, f + 0x30, TXT_FETCH_FAILED);
        return;
    }
    const uint32_t me = ld32(f + 8);
    if (status == 1 || status == 2) {
        chatErrorOnLayer(me, ld32(f + 0xC), ld32(f + 0x10), status == 1 ? TXT_CHAT_ERROR_1 : TXT_CHAT_ERROR_2);
        return;
    }
    if (status == 3) return;
    const uint32_t offset = ld32(offsetRef), total = ld32(totalRef);
    st32(me + SM_CHAT_TOTAL, total);
    st32(me + SM_CHAT_NEW, offset);
    st32(me + SM_CHAT_START, total > 50 ? total - 50 : 1);
    replaceBlocks(me, blocks);
    call(SM_setChatMessageScrollView, {me});
    call(fn::AudioPlaySystemSECancel, {});
    setTouchEnabled(ld32(f + 0xC), 1);
    setTouchEnabled(ld32(f + 0x10), 1);
    call(SM_closeCurrentPopup, {sceneManager(), closeArg});
    call(fn::function_vv_call, {f + 0x18});
}
// the block list's two fetches: after blocking (the popup closed with 0) and after unblocking (with 1)
void blockListFetched(uint32_t f, uint32_t s, uint32_t total, uint32_t offset, uint32_t blocks) {
    blockListAnswer(f, s, total, offset, blocks, 0, BlockPost_failed);
}
void unblockListFetched(uint32_t f, uint32_t s, uint32_t total, uint32_t offset, uint32_t blocks) {
    blockListAnswer(f, s, total, offset, blocks, 1, UnblockPost_failed);
}

// ------------------------------------------------------------------------------------------------ chat data update
constexpr uint32_t SM_CHAT_RETRIES = 0x42C, SM_CHAT_FETCHING = 0x46E;
constexpr uint32_t VT_ChatMessagesGot = 0xF60190;   // chatDataUpdate's message answer {a, b} (inline)
constexpr uint32_t ChatManager_getMessage = 0x4DB7E1;   // (cm, count, type, index, const function&)
constexpr uint32_t CHAT_RETRY_DELAY = 0x41800000;   // 16.0f: seconds to the next poll

// chatDataUpdate's block list answer {-, this, a, b}(status, -, unread, blocks): not 0 -> nothing; else the blocked
// users replaced; unread (or a third quiet poll) -> the messages fetched (100 from the page at +0x40C): sent ->
// requesting, else the next poll in 16 s
void chatBlocksUpdated(uint32_t f, uint32_t statusRef, uint32_t, uint32_t unreadRef, uint32_t blocks) {
    if (ld32(statusRef)) return;
    const uint32_t unread = ld32(unreadRef);
    const uint32_t me = ld32(f + 4);
    replaceBlocks(me, blocks);
    const uint32_t retries = me + SM_CHAT_RETRIES;
    if (unread) {
        st32(retries, 0);
    } else {
        st32(retries, ld32(retries) + 1);
        if (ld32(retries) < 3) return;
    }
    st8(me + SM_CHAT_FETCHING, 1);
    const uint32_t cm = call(ChatManager_getInstance, {});
    const uint32_t page = ld32(me + SM_CHAT_START);
    uint32_t sent;
    {
        Frame fr;
        const uint32_t got = newFunction(fr, VT_ChatMessagesGot, {ld32(f + 8), ld32(f + 0xC)});
        sent = call(ChatManager_getMessage, {cm, 100, 2, page, got});
        functionDestroy(got);
    }
    if (sent)
        st8(me + CHAT_REQUESTING, 1);
    else
        st32(me + CHAT_TIMER, CHAT_RETRY_DELAY);
}

// ------------------------------------------------------------------------------------------------ chat poll answer
// ChatManager::Message (0x4C): its id first, five strings from +4
constexpr uint32_t MESSAGE_SIZE = 0x4C;
constexpr uint32_t Message_copy = 0x4DE205;
constexpr uint32_t SM_CHAT_OPEN = 0x46C, SM_CHAT_MORE = 0x46F;
constexpr uint32_t SM_CHAT_VIEW = 0x404;
constexpr uint32_t SM_CHAT_SHOWN = 0x430;           // vector<Message>: the messages on screen
constexpr uint32_t SM_CHAT_DONE = 0x480;            // function<void()>: runs once when the poll ends
constexpr uint32_t SM_setChatMessageUpdate = 0x617971, SM_setChatMessageScrollViewWith = 0x616DC9;
constexpr uint32_t ChatScrollView_getMessageId = 0x558A35;
constexpr uint32_t ChatManager_update = 0x4DBC89;   // (cm, from id, const function&)
constexpr uint32_t VT_ChatUpdated = 0xF601BC;       // the update's answer {callback} (inline)
constexpr uint32_t CHAT_PAGE_DELAY = 0x41700000;    // 15.0f

void messageDestroy(uint32_t m) {
    for (uint32_t s = 0x34; s >= 4; s -= 0xC) strFree(m + s);
}
uint32_t messageCount(uint32_t vec) { return ((ld32(vec + 4) - ld32(vec)) >> 2) * 0x286BCA1Bu; }
// the poll ended: the done function (when set) runs once and is dropped
void chatDone(uint32_t me) {
    const uint32_t done = me + SM_CHAT_DONE;
    if (!ld32(done + 16)) return;
    call(fn::function_vv_call, {done});
    functionDestroy(done);
    st32(done + 16, 0);
}
// not requesting, the indicator hidden, done
void chatEnd(uint32_t me) {
    st8(me + CHAT_REQUESTING, 0);
    hideLoadingIndicator();
    chatDone(me);
}
// messages from `from` on requested (their answer gets the callback); not sent -> the poll ends
void chatUpdateFrom(uint32_t f, uint32_t me) {
    const uint32_t cm = call(ChatManager_getInstance, {});
    uint32_t sent;
    {
        Frame fr;
        const uint32_t got = newFunction(fr, VT_ChatUpdated, {ld32(f + 8)});
        sent = call(ChatManager_update, {cm, ld32(me + SM_CHAT_FROM), got});
        functionDestroy(got);
    }
    if (!sent) chatEnd(me);
}

// the poll's messages on screen: from the last one's id, the next request (older pages while new ones are counted,
// else from the newest) or the end
void chatShownStep(uint32_t f, uint32_t me, uint32_t last) {
    st8(me + SM_CHAT_FETCHING, 0);
    const uint32_t shown = me + SM_CHAT_SHOWN;
    if (messageCount(shown) <= last) call(Vector_throwOutOfRange, {shown});
    Frame fr;
    const uint32_t msg = fr.alloc(MESSAGE_SIZE);
    call(Message_copy, {msg, ld32(shown) + MESSAGE_SIZE * last});
    const uint32_t fresh = ld32(me + SM_CHAT_NEW), from = ld32(me + SM_CHAT_FROM);
    const uint32_t id = ld32(msg);
    uint32_t total;
    if (id >= fresh + from) {
        st32(me + SM_CHAT_TOTAL, id);
        total = id;
    } else if (fresh) {
        st8(me + SM_CHAT_MORE, 0);
        const uint32_t next = ld32(me + SM_CHAT_TOTAL) + fresh;
        st32(me + SM_CHAT_FROM, next);
        st32(me + SM_CHAT_TOTAL, next);
        st32(me + SM_CHAT_NEW, 0);
        chatUpdateFrom(f, me);
        messageDestroy(msg);
        return;
    } else {
        total = ld32(me + SM_CHAT_TOTAL);
    }
    if (!fresh && total <= from) {
        chatEnd(me);
    } else {
        st32(me + SM_CHAT_NEW, 0);
        st32(me + SM_CHAT_START, total > 50 ? total - 50 : 1);
        st32(me + SM_CHAT_FROM, total);
        chatUpdateFrom(f, me);
    }
    messageDestroy(msg);
}

// chatDataUpdate's message answer {-, this, callback}(status, messages): chat closed -> nothing; 1 / 2 -> the chat
// error, the poll ends; no messages -> it ends; else the newest kept, the list redrawn and older pages fetched while
// the screen is short of them; else -> it ends
void chatMessagesGot(uint32_t f, uint32_t statusRef, uint32_t messages) {
    const uint32_t status = ld32(statusRef);
    const uint32_t me = ld32(f + 4);
    if (!ld8(me + SM_CHAT_OPEN)) return;
    if (status > 3) {
        chatEnd(me);
        return;
    }
    if (status == 1 || status == 2) {
        okDialogText(status == 1 ? TXT_CHAT_ERROR_1 : TXT_CHAT_ERROR_2);
        st8(me + SM_CHAT_FETCHING, 0);
        st8(me + CHAT_REQUESTING, 0);
        hideLoadingIndicator();
        chatDone(me);
        return;
    }
    if (status == 3) return;
    if (ld32(messages + 4) == ld32(messages)) {
        chatEnd(me);
        return;
    }
    const int32_t bytes = int32_t(ld32(messages + 4) - ld32(messages));
    const uint32_t last = messageCount(messages) - 1;
    if (!(last < messageCount(messages))) call(Vector_throwOutOfRange, {messages});
    Frame fr;
    const uint32_t msg = fr.alloc(MESSAGE_SIZE);
    call(Message_copy, {msg, ld32(messages) + MESSAGE_SIZE * last});
    if (bytes >= 0x4D) {
        if (ld32(messages + 4) == ld32(messages)) call(Vector_throwOutOfRange, {messages});
        const uint32_t first = ld32(me + SM_CHAT_FIRST), id0 = ld32(ld32(messages));
        if (first < id0) {
            st32(me + SM_CHAT_FIRST, id0);
        } else {
            const uint32_t oldest = ld32(me + SM_CHAT_FROM);
            if (first < oldest && oldest <= ld32(msg)) st32(me + SM_CHAT_FIRST, oldest);
        }
    }
    const uint32_t retries = me + SM_CHAT_RETRIES;
    bool ended = false;
    if (ld32(retries) >= 3) {
        st32(retries, 0);
        st8(me + SM_CHAT_FETCHING, 0);
        if (!call(SM_setChatMessageUpdate, {me, messages})) {
            chatEnd(me);
            ended = true;
        }
    }
    if (!ended) {
        if (ld32(msg) <= ld32(me + SM_CHAT_TOTAL) && !ld8(me + SM_CHAT_FETCHING)) {
            hideLoadingIndicator();
            st8(me + CHAT_REQUESTING, 0);
            chatDone(me);
        } else {
            call(SM_setChatMessageScrollViewWith, {me, messages});
            const uint32_t pages = me + SM_CHAT_START;
            if (ld32(pages) >= 2 && !call(ChatScrollView_getMessageId, {ld32(me + SM_CHAT_VIEW), 0})) {
                st32(pages, ld32(pages) > 50 ? ld32(pages) - 50 : 1);
                st32(me + CHAT_TIMER, CHAT_PAGE_DELAY);
            } else {
                chatShownStep(f, me, last);
            }
        }
    }
    messageDestroy(msg);
}
}  // namespace cl
}  // namespace

RW_FUNCTION(0x62D3A4, "SceneManager::openDeletePostPopup $_64::operator()(bool) (closed)", cl::deleteClosed);
RW_FUNCTION(0x62D574, "SceneManager::openDeletePostPopup $_133 close end::operator()", cl::deleteClosedEnd);
RW_FUNCTION(0x62E872, "SceneManager::openDeletePostPopup $_59::operator() (Cancel)", cl::deleteCancel);
RW_FUNCTION(0x62DCAC, "SceneManager::openDeletePostPopup $_61::operator() (Delete)", cl::deleteTouched);
RW_FUNCTION(0x62E4A4, "SceneManager::openDeletePostPopup $_60::operator()(StatusCode)", cl::deleteAnswered);
RW_FUNCTION(0x62C678, "SceneManager::openCheckPostPopup $_70::operator() (closed)", cl::checkClosed);
RW_FUNCTION(0x62CC14, "SceneManager::openCheckPostPopup $_67::operator() (Block)", cl::checkBlock);
RW_FUNCTION(0x62D0E4, "SceneManager::openCheckPostPopup $_66::operator() (Report)", cl::checkReport);
RW_FUNCTION(0x629598, "SceneManager::openReportPopup request failed (sub_629598)", cl::reportFailed);
RW_FUNCTION(0x6261E8, "SceneManager::openUnBlockPostPopup request failed (sub_6261E8)", cl::unblockFailed);
RW_FUNCTION(0x62B188, "SceneManager::openBlockPostPopup request failed (sub_62B188)", cl::blockFailed);
RW_FUNCTION(0x62A078, "SceneManager::openReportPopup $_80::operator() (Cancel)", cl::reportCancel);
RW_FUNCTION(0x6284D0, "SceneManager::openReportSelectPopup $_88::operator()(int)", cl::reportReasonPicked);
RW_FUNCTION(0x62A344, "SceneManager::openBlockPostPopup $_78::operator() (closed)", cl::blockClosed);
RW_FUNCTION(0x625618, "SceneManager::openUnBlockPostPopup $_101::operator() (closed)", cl::unblockClosed);
RW_FUNCTION(0x628654, "SceneManager::openReportPopup $_86::operator() (closed)", cl::reportClosed);
RW_FUNCTION(0x6280E4, "SceneManager::openReportSelectPopup $_90::operator()(bool) (closed)", cl::reasonClosed);
RW_FUNCTION(0x629EE0, "SceneManager::openReportPopup $_138::operator()(int)", cl::reportReasonShown);
RW_FUNCTION(0x62A170, "SceneManager::openReportPopup $_79::operator() (text changed)", cl::reportTextChanged);
RW_FUNCTION(0x627674, "SceneManager::openBlockListPopup closed::operator()(bool) (sub_627674)", cl::blockListClosed);
RW_FUNCTION(0x628830, "SceneManager::openReportPopup close end::operator() (sub_628830)", cl::reportClosedEnd);
RW_FUNCTION(0x629CB4, "SceneManager::openReportPopup $_81::operator() (Reason)", cl::reportReasonTouched);
RW_FUNCTION(0x62ABFC, "SceneManager::openBlockPostPopup $_75::operator() (Block)", cl::blockTouched);
RW_FUNCTION(0x62B7F4, "SceneManager::openBlockPostPopup $_74::operator()(StatusCode)", cl::blockAnswered);
RW_FUNCTION(0x62BFA8, "SceneManager::openBlockListPopup block list answer::operator() (sub_62BFA8)", cl::blockListFetched);
RW_FUNCTION(0x625D20, "SceneManager::openUnBlockPostPopup $_98::operator() (Unblock)", cl::unblockTouched);
RW_FUNCTION(0x62673C, "SceneManager::openUnBlockPostPopup $_97::operator()(StatusCode)", cl::unblockAnswered);
RW_FUNCTION(0x627040, "SceneManager::openUnBlockPostPopup block list answer::operator() (sub_627040)", cl::unblockListFetched);
RW_FUNCTION(0x6279A8, "SceneManager::openBlockListPopup list shown::operator() (sub_6279A8)", cl::blockListShow);
RW_FUNCTION(0x629000, "SceneManager::openReportPopup $_81::operator() (Report)", cl::reportTouched);
RW_FUNCTION(0x6298BC, "SceneManager::openReportPopup $_82::operator()(StatusCode)", cl::reportAnswered);
RW_FUNCTION(0x6327A4, "SceneManager::chatDataUpdate block list answer::operator() (sub_6327A4)", cl::chatBlocksUpdated);
RW_FUNCTION(0x632F58, "SceneManager::chatDataUpdate message answer::operator() (sub_632F58)", cl::chatMessagesGot);
