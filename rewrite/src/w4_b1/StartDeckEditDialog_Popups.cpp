// StartDeckEditDialog: the cost-over popup and the ability-max (burst) popup.
#include "StartDeckEditDialog_lib.hpp"

using namespace sded;

namespace {
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_getPopupLayer2 = 0x613E71;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t FunctionVB_copy = 0x45C84D;          // std::function<void(bool)>::function(const function&)
constexpr uint32_t Misc_create = 0x786D5D;
constexpr uint32_t ANCHOR_MIDDLE = 0xFEF5C0;
constexpr uint32_t IN_OUT_TIME = 0xDEC3D0;
constexpr uint32_t VS_setAnchorPoint = 0x9C, VS_getPositionY = 0x70;

constexpr uint32_t S_COST_OVER_JSON = 0xDFF992, S_ABILITY_MAX_JSON = 0xDFF962;
constexpr uint32_t S_4LINE_LABEL = 0xDF87A2, S_BUTTON_EQUIP = 0xDFF9C0, S_TXT_EQUIP = 0xDFF9CD;
constexpr uint32_t S_CLOSE_BUTTON = 0xDF8D2F, S_TXT_WORDING = 0xDF862F, S_TXT_CROWN = 0xDF8658;
constexpr uint32_t S_TXT_CROWN_LABEL = 0xDF866B, S_FMT_D = 0xDF86DE, S_TXT_CANCEL = 0xDF86A6;

constexpr uint32_t VT_cost_36 = 0xF595C4, VT_cost_37 = 0xF5956C;
constexpr uint32_t VT_max_29 = 0xF5977C, VT_max_30 = 0xF59750, VT_max_32 = 0xF59724, VT_max_33 = 0xF5961C;
constexpr uint32_t VT_max_34 = 0xF595F0;

// shadow sprite faded in on the popup layer 2; returns {layer, shadow}
void shadowIn(uint32_t &layer, uint32_t &shadow) {
    layer = call(SceneManager_getPopupLayer2, {call(SceneManager_getInstance, {})});
    shadow = call(SceneUtil_createShadowSprite, {});
    uint32_t op = vcall(shadow, VS_getOpacity, {shadow});
    vcall(shadow, VS_setOpacity, {shadow, 0});
    call(wl::Node_runAction, {shadow, call(FadeTo_create, {ld32(IN_OUT_TIME), op})});
    vcall(layer, VS_addChild, {layer, shadow});
}

// the popup json, centred (side margins added) and put on the layer
uint32_t popupWidget(uint32_t layer, uint32_t json) {
    uint32_t popup = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), json});
    vcall(popup, VS_setAnchorPoint, {popup, ANCHOR_MIDDLE});
    float x = bf(vcall(popup, VS_getPositionX, {popup}));
    float w = ldf(vcall(popup, VS_getContentSize, {popup}));
    float margin = bf(call(SceneUtil_getSideMargin, {}));
    float y = bf(vcall(popup, VS_getPositionY, {popup}));
    float h = ldf(vcall(popup, VS_getContentSize, {popup}) + 4);
    float px = margin + (x + w * 0.5f);
    float py = y + h * 0.5f;
    float hm = bf(call(SceneUtil_getSideHeightMargin, {}));
    Frame f;
    uint32_t pos = f.alloc(8);
    call(wl::Vec2_ctorXY, {pos, fb(px), fb(hm + py)});
    vcall(popup, VS_setPosition, {popup, pos});
    call(wl::Vec2_dtor, {pos});
    vcall(layer, VS_addChild, {layer, popup});
    return popup;
}

// a std::function on the guest stack holding a heap __func: vtable, words at +8.., a copied function at `fnAt`
uint32_t heapFunc(Frame &f, uint32_t size, uint32_t vtable, std::initializer_list<uint32_t> words, uint32_t fnAt,
                  uint32_t src) {
    uint32_t fo = f.alloc(24);
    st32(fo + 16, 0);
    uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, vtable);
        uint32_t k = 8;
        for (uint32_t w : words) { st32(heap + k, w); k += 4; }
        functionCopy(heap + fnAt, src);
    }
    st32(fo + 16, heap);
    return fo;
}
}  // namespace

struct StartDeckEditDialogPopups {
    // openCostOverDeckPopup(callback): `this` is unused (the callers leave a lambda vtable in r0)
    static uint32_t openCostOverDeckPopup(uint32_t, uint32_t callback) {
        call(AudioPlaySystemSEPopUp, {});
        uint32_t layer, shadow;
        shadowIn(layer, shadow);
        uint32_t popup = popupWidget(layer, S_COST_OVER_JSON);
        titleLabel(popup, S_4LINE_LABEL, 11, 103000014);
        uint32_t equip = find(popup, S_BUTTON_EQUIP, 12);
        titleLabel(equip, S_TXT_EQUIP, 9, 100100006);
        uint32_t close = find(popup, S_CLOSE_BUTTON, 12);
        Frame f;
        uint32_t cb = f.alloc(24);                 // $_35's captured callback
        call(FunctionVB_copy, {cb, callback});
        for (int k = 0; k < 2; k++) {
            Frame g;
            uint32_t tmp = g.alloc(24);
            functionCopy(tmp, cb);
            uint32_t fo = heapFunc(g, 0x38, k == 0 ? VT_cost_36 : VT_cost_37, {equip, close, shadow, popup, layer},
                                   0x20, tmp);
            call(Widget_addTouchEventListener, {k == 0 ? equip : close, fo});
            functionDestroy(fo);
            functionDestroy(tmp);
        }
        {
            Frame g;
            uint32_t empty = newEmptyFunction(g);
            call(wl::Node_runAction, {popup, call(SceneUtil_createOpenPopupAnimation, {popup, empty})});
            functionDestroy(empty);
        }
        functionDestroy(cb);
        return 0;
    }

    // openAbilityMaxPopup(done): the crown cost wording, the crowns owned, OK / Cancel; opened through
    // SceneManager::openPopup, then $_34 runs after 0.1 s
    static uint32_t openAbilityMaxPopup(uint32_t self, uint32_t done) {
        uint32_t layer, shadow;
        shadowIn(layer, shadow);
        uint32_t popup = popupWidget(layer, S_ABILITY_MAX_JSON);
        Frame f;
        int32_t cost;
        {
            uint32_t misc = masterRow(f, Misc_create, 120);
            cost = scene::safe32(ld32(misc), 4);
            release(ld32(misc + 4));
        }
        uint32_t a = api();
        uint32_t status = ld32(a + 0xA4), statusCtrl = ld32(a + 0xA8);
        addRef(statusCtrl);
        {
            uint32_t text = find(popup, S_TXT_WORDING, 11);
            Frame g;
            uint32_t name = newStr(g, S_TXT_WORDING, 11);
            uint32_t ctv = call(SceneUtil_textToCustomTextView, {text, name});
            strFree(name);
            uint32_t td = textData(g, 103000012);
            uint32_t buf = stringBuffer(g, {textDataStr(td), uint32_t(cost)});
            uint32_t xml = newStr(g, buf);
            call(CustomTextView_setXml, {ctv, xml});
            strFree(xml);
            release(ld32(td + 4));
        }
        titleLabel(popup, S_TXT_CROWN, 18, 100310002);
        {
            uint32_t label = find(popup, S_TXT_CROWN_LABEL, 24);
            uint32_t w[2];
            safeWords(status, 0, w, 2);
            Frame g;
            uint32_t buf = stringBuffer(g, {S_FMT_D, w[0] + w[1]});
            uint32_t str = newStr(g, buf);
            call(Text_setString, {label, str});
            strFree(str);
            call(SceneUtil_setShadow, {label});
        }
        uint32_t ok = find(popup, S_BUTTON_OK, 9);
        uint32_t cancel = find(popup, S_BUTTON_CLOSE, 12);
        titleLabel(ok, S_TXT_OK, 6, 100100001);
        titleLabel(cancel, S_TXT_CANCEL, 10, 100100002);
        uint32_t a2 = api();
        uint32_t slide = ld32(self + 0x2B0);
        uint32_t ids = ld32(slide + 0x578);
        uint32_t count = uint32_t(int32_t(ld32(slide + 0x57C) - ids) >> 2);
        uint32_t kbs = ld32(a2 + API_KEYBLADES);
        uint32_t idx = ld32(slide + 0x5AC);
        if (!(count > idx)) {
            call(wl::VECTOR_AT_THROW, {slide + 0x578});
            return 0;
        }
        uint32_t kb[22];
        safeWords(kbs, ld32(ids + idx * 4) * 0x58, kb, 22);
        uint32_t doneCopy = f.alloc(24);
        call(FunctionVV_copy, {doneCopy, done});
        {
            Frame g;
            uint32_t fo = inlineFn(g, VT_max_29, {});
            call(Widget_addTouchEventListener, {ok, fo});
            functionDestroy(fo);
        }
        {
            Frame g;
            uint32_t fo = inlineFn(g, VT_max_30, {});
            call(Widget_addTouchEventListener, {cancel, fo});
            functionDestroy(fo);
        }
        // the 0x70-byte capture {ok, cancel, shadow, popup, layer, cost, keyblade} + function<void()> at +0x70
        uint32_t cap = f.alloc(0x88);
        st32(cap, ok);
        st32(cap + 4, cancel);
        st32(cap + 8, shadow);
        st32(cap + 0xC, popup);
        st32(cap + 0x10, layer);
        st32(cap + 0x14, uint32_t(cost));
        for (uint32_t k = 0; k < 22; k++) st32(cap + 0x18 + k * 4, kb[k]);
        functionCopy(cap + 0x70, doneCopy);
        uint32_t sm = call(SceneManager_getInstance, {});
        {
            Frame g;
            uint32_t check = inlineFn(g, VT_max_32, {ok, cancel});
            uint32_t tmp = g.alloc(0x88);
            call(wl::MEMCPY, {tmp, cap, 0x70});
            functionCopy(tmp + 0x70, cap + 0x70);
            uint32_t fo = g.alloc(24);
            st32(fo + 16, 0);
            uint32_t heap = call(fn::operator_new, {0x90});
            if (heap) {
                st32(heap, VT_max_33);
                call(wl::MEMCPY, {heap + 8, tmp, 0x70});
                functionCopy(heap + 0x78, tmp + 0x70);
            }
            st32(fo + 16, heap);
            call(SceneManager_openPopup, {sm, check, fo, 1});
            functionDestroy(fo);
            functionDestroy(tmp + 0x70);
            functionDestroy(check);
        }
        {
            Frame g;
            uint32_t empty = newEmptyFunction(g);
            call(wl::Node_runAction, {popup, call(SceneUtil_createOpenPopupAnimation, {popup, empty})});
            functionDestroy(empty);
        }
        {
            uint32_t delay = call(wl::DelayTime_create, {fb(0.1f)});
            Frame g;
            uint32_t fo = inlineFn(g, VT_max_34, {self});
            uint32_t cf = call(wl::CallFunc_create, {fo});
            call(wl::Node_runAction, {self, call(wl::Sequence_create, {delay, cf, 0})});
            functionDestroy(fo);
        }
        functionDestroy(cap + 0x70);
        functionDestroy(doneCopy);
        release(statusCtrl);
        return 0xB14;
    }

};

using P = StartDeckEditDialogPopups;
RW_FUNCTION(0x57818C, "StartDeckEditDialog::openCostOverDeckPopup", P::openCostOverDeckPopup);
RW_FUNCTION(0x577550, "StartDeckEditDialog::openAbilityMaxPopup", P::openAbilityMaxPopup);
