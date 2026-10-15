// SceneUtil: medal box popups (box limit raised, box full).
#include "../core/SafeBuffer.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
// APIManager: shared_ptr<UserData> (a SafeBuffer; medal box extension at 0x34)
constexpr uint32_t APIManager_userData = 0x94;
constexpr uint32_t USERDATA_MEDAL_SLOT_EXTENSION = 0x34;
constexpr uint32_t MISC_MEDAL_SLOT_BASE = 103;            // medal box size before extensions
constexpr uint32_t MISC_MEDAL_SLOT_MAX = 104;             // largest medal box

constexpr uint32_t VT_MaxUpResult_OkTouch = 0xF54E50;     // $_17: [callback, popup, layer, shadow]
constexpr uint32_t VT_MedalOver_ExtensionTouch = 0xF54DF8;   // $_19
constexpr uint32_t VT_MedalOver_MixTouch = 0xF54DCC;      // $_20
constexpr uint32_t VT_MedalOver_SellTouch = 0xF54D48;     // $_21 (+ BackScene)

constexpr uint32_t STR_PopupMedalPossessed2Json = 0xDF86E1;  // "cocostudio/publish/PopupNormal_MedalPossessed2.json"
constexpr uint32_t STR_PopupMedalOverJson = 0xDF8760;        // "cocostudio/publish/PopupNormal_MedalOver.json"
constexpr uint32_t STR_Txt_Wording = 0xDF862F;
constexpr uint32_t STR_Txt_UpperLimit1 = 0xDF8715;
constexpr uint32_t STR_Txt_UpperLimit_after = 0xDF8725;
constexpr uint32_t STR_Txt_UpperLimit_before = 0xDF873A;
constexpr uint32_t STR_Txt_UpperLimit2 = 0xDF8750;
constexpr uint32_t STR_Button_OK = 0xDF8688;
constexpr uint32_t STR_Txt_OK = 0xDF869F;
constexpr uint32_t STR_d = 0xDF86DE;                          // "%d"
constexpr uint32_t STR_NormalText_Window01 = 0xDF878E;
constexpr uint32_t STR_4Line_Label = 0xDF87A2;
constexpr uint32_t STR_s_nn_s = 0xDF87AE;                     // "%s\n\n%s"
constexpr uint32_t STR_Button_Extension = 0xDF87B5;
constexpr uint32_t STR_Txt_Extension = 0xDF87C6;
constexpr uint32_t STR_Button_Mix = 0xDF87D4;
constexpr uint32_t STR_Txt_Mix = 0xDF87DF;
constexpr uint32_t STR_Button_Sell = 0xDF87E7;
constexpr uint32_t STR_Txt_Sell = 0xDF87F3;

int32_t miscValue(uint32_t id) {
    Frame f;
    uint32_t misc = f.alloc(8);
    call(fn::Misc_create, {misc, id});
    int32_t v = rw::host_ptr<SafeBuffer>(ld32(misc))->get32(4);
    sharedRelease(misc);
    return v;
}

// widget->addTouchEventListener(lambda) for a heap-stored lambda: vtable, four captured words at +8, a copy
// of the std::function<void(bool)> `callback` at +0x18 and optionally one more word at +0x30.
void addTouchWithCallback(uint32_t widget, uint32_t vtable, uint32_t size, const uint32_t (&words)[4],
                          uint32_t callback, bool hasExtra = false, uint32_t extra = 0) {
    Frame f;
    uint32_t captured = f.alloc(24);
    functionCopy(captured, callback);
    uint32_t touch = f.alloc(20);
    uint32_t heap = heapFunction(touch, vtable, size, 0x18, fn::function_vb_move, captured);
    if (heap) {
        for (uint32_t k = 0; k < 4; k++) st32(heap + 8 + 4 * k, words[k]);
        if (hasExtra) st32(heap + 0x30, extra);
    }
    call(fn::Widget_addTouchEventListener, {widget, touch});
    functionDestroy(touch);
    functionDestroy(captured);
}
}  // namespace

// Is the medal box already at its largest size (base + bought extensions > maximum)?
bool SceneUtil::isMaxMedalSlot() {
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctl = ld32(api + APIManager_userData + 4);
    uint32_t userData = ld32(api + APIManager_userData);
    if (ctl) call(fn::shared_add, {ctl});
    int32_t base = miscValue(MISC_MEDAL_SLOT_BASE);
    int32_t extension = rw::host_ptr<SafeBuffer>(userData)->get32(USERDATA_MEDAL_SLOT_EXTENSION);
    int32_t max = miscValue(MISC_MEDAL_SLOT_MAX);
    int32_t slots = int32_t(uint32_t(extension) + uint32_t(base));
    if (ctl) call(fn::shared_release, {ctl});
    return max < slots;
}
RW_FUNCTION(0x45F9DC, "SceneUtil::isMaxMedalSlot", SceneUtil::isMaxMedalSlot);

// "Medal box enlarged" result: jewels spent, new limit.
void SceneUtil::openMedalMaxUpResultDialog(int32_t spent, int32_t added, int32_t newLimit, Addr callback) {
    PopupBase base = loadPopup(STR_PopupMedalPossessed2Json);
    uint32_t popup = base.popup;
    Frame f;
    uint32_t wording = findNode(popup, STR_Txt_Wording, 11);
    uint32_t viewName = newStr(f, STR_Txt_Wording, 11);
    uint32_t view = textToCustomTextView(wording, viewName);
    strFree(viewName);
    {
        Frame g;
        uint32_t text = textData(g, 102600021);
        uint32_t buf = stringBuffer(g, {textDataStr(text), uint32_t(spent)});
        uint32_t xml = newStr(g, buf);
        call(fn::CustomTextView_setXml, {view, xml});
        strFree(xml);
        sharedRelease(text);
    }
    uint32_t limit1 = findNode(popup, STR_Txt_UpperLimit1, 15);
    setShadow(limit1);
    {
        Frame g;
        uint32_t text = textData(g, 102600040);
        uint32_t buf = stringBuffer(g, {textDataStr(text), uint32_t(added)});
        setTextC(limit1, buf);
        sharedRelease(text);
    }
    uint32_t after = findNode(popup, STR_Txt_UpperLimit_after, 20);
    setShadow(after);
    {
        Frame g;
        setTextC(after, stringBuffer(g, {STR_d, uint32_t(newLimit)}));
    }
    uint32_t before = findNode(popup, STR_Txt_UpperLimit_before, 21);
    vcall(before, node::setVisible, {before, 0});
    uint32_t limit2 = findNode(popup, STR_Txt_UpperLimit2, 15);
    vcall(limit2, node::setVisible, {limit2, 0});

    uint32_t ok = findNode(popup, STR_Button_OK, 9);
    {
        // [callback, popup, layer, shadow]: the function is copied straight into the lambda temporary
        Frame g;
        uint32_t captured = g.alloc(20);
        call(fn::function_vb_copy, {captured, callback});
        uint32_t touch = g.alloc(20);
        uint32_t heap = heapFunction(touch, VT_MaxUpResult_OkTouch, 0x30, 8, fn::function_vb_move, captured);
        if (heap) {
            st32(heap + 0x20, popup);
            st32(heap + 0x24, base.layer);
            st32(heap + 0x28, base.shadow);
        }
        call(fn::Widget_addTouchEventListener, {ok, touch});
        functionDestroy(touch);
        functionDestroy(captured);
    }
    titleLabel(ok, STR_Txt_OK, 6, 100100001);
    uint32_t none = newEmptyFunction(f);
    call(fn::Node_runAction, {popup, createOpenPopupAnimation(popup, none)});
    functionDestroy(none);
}
RW_FUNCTION(0x45FAE8, "SceneUtil::openMedalMaxUpResultDialog", SceneUtil::openMedalMaxUpResultDialog);

// "Medal box full": enlarge the box, fuse medals or sell medals.
void SceneUtil::openMedalOverDialog(uint32_t altMessage, uint32_t backScene, Addr callback) {
    call(fn::AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t shadow = createShadowSprite();
    uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}),
                                                             STR_PopupMedalOverJson});
    centerPopup(popup);
    vcall(shadow, node::addChild, {shadow, popup});     // this popup sits on the shadow
    uint32_t window = findNode(popup, STR_NormalText_Window01, 19);

    // every button's lambda captures [window, layer, shadow, popup] and a copy of the callback
    Frame f;
    const uint32_t words[4] = {window, layer, shadow, popup};
    uint32_t cbCopy = f.alloc(20);
    call(fn::function_vb_copy, {cbCopy, callback});
    int32_t messageId = altMessage ? 102610003 : 102610002;
    uint32_t label = findNode(popup, STR_4Line_Label, 11);
    uint32_t viewName = newStr(f, STR_4Line_Label, 11);
    uint32_t view = textToCustomTextView(label, viewName);
    strFree(viewName);
    {
        Frame g;
        uint32_t text1 = textData(g, messageId);
        uint32_t text2 = textData(g, 102610008);
        uint32_t buf = stringBuffer(g, {STR_s_nn_s, textDataStr(text1), textDataStr(text2)});
        uint32_t xml = newStr(g, buf);
        call(fn::CustomTextView_setXml, {view, xml});
        strFree(xml);
        sharedRelease(text2);
        sharedRelease(text1);
    }
    uint32_t extension = findNode(popup, STR_Button_Extension, 16);
    addTouchWithCallback(extension, VT_MedalOver_ExtensionTouch, 0x30, words, cbCopy);
    titleLabel(extension, STR_Txt_Extension, 13, 102610007);
    uint32_t mix = findNode(popup, STR_Button_Mix, 10);
    addTouchWithCallback(mix, VT_MedalOver_MixTouch, 0x30, words, cbCopy);
    titleLabel(mix, STR_Txt_Mix, 7, 101300001);
    uint32_t sell = findNode(popup, STR_Button_Sell, 11);
    addTouchWithCallback(sell, VT_MedalOver_SellTouch, 0x38, words, cbCopy, true, backScene);
    titleLabel(sell, STR_Txt_Sell, 8, 100100029);

    uint32_t none = newEmptyFunction(f);
    call(fn::Node_runAction, {window, createOpenPopupAnimation(window, none)});
    functionDestroy(none);
    functionDestroy(cbCopy);
}
RW_FUNCTION(0x460250, "SceneUtil::openMedalOverDialog", SceneUtil::openMedalOverDialog);
