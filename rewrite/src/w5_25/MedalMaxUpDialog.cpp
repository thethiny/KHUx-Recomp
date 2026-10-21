// SceneUtil::openMedalMaxUpDialog(onDone): "raise the medal box limit" (from the medal-full dialog's Expand).
// At the top limit already: an OK dialog ($_16). With the Jewels for it: PopupNormal_MedalPossessed1 with the
// cost (Misc 111) and step (Misc 103), the current limit (userDetail +0x34), the cap (Misc 104) and the Jewels held
// (userPoint paid + free); OK ($_14) buys, Cancel ($_15 on close) gives up. Without them: an OK / Cancel dialog
// offering the shop ($_17).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_isMaxMedalSlot = 0x45F9DD;
constexpr uint32_t SceneUtil_hasMagicStone = 0x45DD29;
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;         // (const string&, function<void(bool)>, const char*)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;   // (..., const char*, const char*)
constexpr uint32_t function_vbb_copy = 0x45CD55;                   // std::function<void(bool,bool)>(const&)
constexpr uint32_t function_vbb_move = 0x48B1E9;                   // std::function<void(bool,bool)>(&&)
constexpr uint32_t memcpy = 0x40E164;
}  // namespace g

constexpr uint32_t S_Json = 0xDFA026;   // "cocostudio/publish/PopupNormal_MedalPossessed1.json"
constexpr uint32_t S_Txt_Wording = 0xDF862F, S_Txt_UpperLimit1 = 0xDF8715, S_Txt_UpperLimit_before = 0xDF873A,
                   S_Txt_UpperLimit2 = 0xDF8750, S_Txt_CrownPossessed = 0xDF8658,
                   S_Txt_CrownPossessed_Label = 0xDF866B;
constexpr uint32_t S_Button_OK = 0xDF8688, S_Button_Close = 0xDF8692, S_Txt_OK = 0xDF869F, S_Txt_Cancel = 0xDF86A6;
constexpr uint32_t S_7d = 0xDF8684;     // "%7d"
constexpr int32_t TXT_AT_MAX = 102600022, TXT_NO_JEWELS = 102600016, TXT_WORDING = 102600020;
constexpr int32_t TXT_LIMIT_NOW = 102600042, TXT_LIMIT_CAP = 102600043, TXT_JEWELS = 100310002;
constexpr int32_t TXT_OK = 100100001, TXT_CANCEL = 100100002;
constexpr int32_t MISC_STEP = 103, MISC_CAP = 104, MISC_COST = 111;
constexpr uint32_t VT_AtMax = 0xF54EA8;      // $_16 {function<void(bool,bool)> at +8}
constexpr uint32_t VT_Buy = 0xF55504;        // $_14 {ok, close, popup, layer, shadow, step, limit; fn at +0x28}
constexpr uint32_t VT_Cancel = 0xF554D8;     // Button_Close: no captures
constexpr uint32_t VT_CanClose = 0xF554AC;   // {close}
constexpr uint32_t VT_Closed = 0xF55454;     // $_15 {ok, close, popup, layer, shadow; fn at +0x20}
constexpr uint32_t VT_NoJewels = 0xF55428;   // $_17 {fn at +8}
constexpr uint32_t API_USER_DETAIL = 0x94, API_USER_POINT = 0xA4;   // shared_ptr<SafeBuffer> in APIManager
constexpr uint32_t UD_MAX_MEDAL = 0x34;
constexpr uint32_t V_SET_VISIBLE = 0xB0;

void release(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}

int32_t miscValue(int32_t id) {
    Frame f;
    uint32_t row = masterRow(f, fn::Misc_create, uint32_t(id));
    int32_t v = safe32(ld32(row), 4);
    sharedRelease(row);
    return v;
}

// a std::string literal longer than the inline form (libc++: heap block of 16 / 32 bytes)
uint32_t longStr(Frame &f, uint32_t text, uint32_t len) {
    uint32_t s = f.alloc(12);
    uint32_t cap = (len + 16) & ~15u;
    uint32_t buf = call(fn::operator_new, {cap});
    st32(s, cap | 1);
    st32(s + 4, len);
    st32(s + 8, buf);
    call(g::memcpy, {buf, text, len});
    st8(buf + len, 0);
    return s;
}
uint32_t findLong(uint32_t root, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = longStr(f, name, len);
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    strFree(s);
    return node;
}
uint32_t customTextView(uint32_t text, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = longStr(f, name, len);
    uint32_t view = call(g::SceneUtil_textToCustomTextView, {text, s});
    strFree(s);
    return view;
}
// a std::function whose callable is a heap __func `vt` (size bytes) holding a function<void(bool,bool)> moved from
// `from` at `fnOffset`, with `words` from +8
uint32_t heapLambda(Frame &f, uint32_t vt, uint32_t size, uint32_t fnOffset, uint32_t from,
                    std::initializer_list<uint32_t> words) {
    uint32_t fo = newEmptyFunction(f);
    uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, vt);
        uint32_t k = 8;
        for (uint32_t w : words) { st32(heap + k, w); k += 4; }
        call(g::function_vbb_move, {heap + fnOffset, from});
    }
    st32(fo + 16, heap);
    return fo;
}

struct MedalMaxUpDialog {
    static void open(uint32_t onDone) {
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t detail = ld32(api + API_USER_DETAIL), detailCtrl = ld32(api + API_USER_DETAIL + 4);
        if (detailCtrl) call(fn::shared_add, {detailCtrl});
        int32_t step = miscValue(MISC_STEP);
        int32_t limit = safe32(detail, UD_MAX_MEDAL);
        int32_t cap = miscValue(MISC_CAP);
        int32_t cost = miscValue(MISC_COST);
        Frame f;
        uint32_t done = f.alloc(20), done2 = f.alloc(20);
        call(g::function_vbb_copy, {done, onDone});
        call(g::function_vbb_copy, {done2, onDone});
        if (call(g::SceneUtil_isMaxMedalSlot, {})) {
            Frame t;
            uint32_t text = textData(t, TXT_AT_MAX);
            uint32_t msg = newStr(t, textDataStr(text));
            uint32_t tmp = t.alloc(20);
            call(g::function_vbb_copy, {tmp, onDone});
            uint32_t cb = heapLambda(t, VT_AtMax, 0x20, 8, tmp, {});
            call(g::SceneUtil_openOkDialog4Line, {msg, cb, 0});
            functionDestroy(cb);
            functionDestroy(tmp);
            strFree(msg);
            sharedRelease(text);
        } else if (call(g::SceneUtil_hasMagicStone, {uint32_t(cost)}) == 1) {
            popup(done, cost, step, limit, cap);
        } else {
            Frame t;
            uint32_t copy = t.alloc(20);
            functionCopy(copy, done2);
            uint32_t text = textData(t, TXT_NO_JEWELS);
            uint32_t msg = newStr(t, textDataStr(text));
            uint32_t tmp = t.alloc(20);
            functionCopy(tmp, copy);
            uint32_t cb = heapLambda(t, VT_NoJewels, 0x20, 8, tmp, {});
            call(g::SceneUtil_openOkCancelDialog4Line, {msg, cb, 0, 0});
            functionDestroy(cb);
            functionDestroy(tmp);
            strFree(msg);
            sharedRelease(text);
            functionDestroy(copy);
        }
        functionDestroy(done2);
        functionDestroy(done);
        release(detailCtrl);
    }

    // a TextData format with arguments, into a string
    static void setFormatted(uint32_t target, bool xml, int32_t id, std::initializer_list<uint32_t> args) {
        Frame f;
        uint32_t text = textData(f, id);
        uint32_t buf;
        if (args.size() == 1) buf = stringBuffer(f, {textDataStr(text), *args.begin()});
        else buf = stringBuffer(f, {textDataStr(text), *args.begin(), *(args.begin() + 1)});
        uint32_t s = newStr(f, buf);
        if (xml) call(fn::CustomTextView_setXml, {target, s});
        else call(fn::Text_setString, {target, s});
        strFree(s);
        sharedRelease(text);
    }

    static void popup(uint32_t done, int32_t cost, int32_t step, int32_t limit, int32_t cap) {
        PopupBase p = loadPopup(S_Json);
        uint32_t wording = findLong(p.popup, S_Txt_Wording, 11);
        uint32_t view = customTextView(wording, S_Txt_Wording, 11);
        setFormatted(view, true, TXT_WORDING, {uint32_t(cost), uint32_t(step)});
        uint32_t limit1 = findLong(p.popup, S_Txt_UpperLimit1, 15);
        view = customTextView(limit1, S_Txt_UpperLimit1, 15);
        setFormatted(view, true, TXT_LIMIT_NOW, {uint32_t(limit)});
        uint32_t before = findLong(p.popup, S_Txt_UpperLimit_before, 21);
        vcall(before, V_SET_VISIBLE, {before, 0});
        uint32_t limit2 = findLong(p.popup, S_Txt_UpperLimit2, 15);
        call(g::SceneUtil_setShadow, {limit2});
        setFormatted(limit2, false, TXT_LIMIT_CAP, {uint32_t(cap)});
        uint32_t crown = findLong(p.popup, S_Txt_CrownPossessed, 18);
        call(g::SceneUtil_setShadow, {crown});
        setTextId(crown, TXT_JEWELS);
        uint32_t jewels = findLong(p.popup, S_Txt_CrownPossessed_Label, 24);
        call(g::SceneUtil_setShadow, {jewels});
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t point = ld32(api + API_USER_POINT), pointCtrl = ld32(api + API_USER_POINT + 4);
        if (pointCtrl) call(fn::shared_add, {pointCtrl});
        {
            int32_t free = safe32(point, 4), paid = safe32(point, 0);
            Frame f;
            uint32_t buf = stringBuffer(f, {S_7d, uint32_t(paid + free)});
            setTextC(jewels, buf);
        }
        uint32_t ok = findNode(p.popup, S_Button_OK, 9);
        uint32_t close = findLong(p.popup, S_Button_Close, 12);
        {
            Frame f;
            uint32_t copy = f.alloc(20);
            functionCopy(copy, done);
            uint32_t cb = heapLambda(f, VT_Buy, 0x40, 0x28, copy,
                                     {ok, close, p.popup, p.layer, p.shadow, uint32_t(step), uint32_t(limit)});
            call(fn::Widget_addTouchEventListener, {ok, cb});
            functionDestroy(cb);
            functionDestroy(copy);
        }
        titleLabel(ok, S_Txt_OK, 6, TXT_OK);
        addTouch(close, VT_Cancel, {});
        titleLabel(close, S_Txt_Cancel, 10, TXT_CANCEL);
        Frame f;
        uint32_t copy = f.alloc(20);
        functionCopy(copy, done);
        uint32_t mgr = sceneManager();
        {
            Frame t;
            uint32_t canClose = newFunction(t, VT_CanClose, {close});
            uint32_t tmp = t.alloc(20);
            functionCopy(tmp, copy);
            uint32_t closed = heapLambda(t, VT_Closed, 0x38, 0x20, tmp, {ok, close, p.popup, p.layer, p.shadow});
            call(fn::SceneManager_openPopup, {mgr, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(tmp);
            functionDestroy(canClose);
        }
        {
            Frame t;
            uint32_t none = newEmptyFunction(t);
            call(fn::Node_runAction, {p.popup, call(g::SceneUtil_createOpenPopupAnimation, {p.popup, none})});
            functionDestroy(none);
        }
        functionDestroy(copy);
        release(pointCtrl);
    }
};
}  // namespace

RW_FUNCTION(0x45E798, "SceneUtil::openMedalMaxUpDialog(function<void(bool,bool)>)", MedalMaxUpDialog::open);
