// Shop > Sell Materials, the sale itself: SceneSellOutMaterialItem::openSellCheckDialog's result lambda $_17 (the
// API answer: refresh the header, rebuild and sort the material list, "no materials" when it is empty, the
// "Sold for n munny" popup SceneUtil::openSellOkPopup, the list field), the lambdas that remove the dialogs
// ($_24 UlvE_, openSellDetailPopup's $_23 UlvE_) and runOutAnimation's $_8 std::function slots.
// $_17's sort (std::__sort / __sort3 / the comparator) is a second copy of the instantiation init uses (another
// lambda type, the same code): the rewrite runs that one (w5_13/MaterialSale.cpp).
#include "../scene/SceneUi.hpp"
#include "../scene_main/FunctionThunks.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t SceneManager_openPopup = 0x61524D;      // (mgr, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t BaseScrollViewULL_setItemIDs = 0x4FD231;
constexpr uint32_t VectorULL_pushBackSlow = 0x4ECB6D;
constexpr uint32_t VectorULL_dtor = 0x468499;              // ~__vector_base<unsigned long long>
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_openSellOkPopup = 0x474DCD;   // (int munny, bool)
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;       // (string out, unsigned long long)
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_setShadowText = 0x45BD15;
constexpr uint32_t Scene_updateListField = 0x6869B1;       // SceneSellOutMaterialItem::updateListField
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD, audio_Manager_getLayer = 0x5BF33D;
constexpr uint32_t audio_Layer_playSE = 0x5BF785;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
// the instantiation init uses (w5_13/MaterialSale.cpp)
constexpr uint32_t MaterialSort = 0x68AED9, MaterialSort3 = 0x68B495, MaterialLess = 0x68B319;
}  // namespace g

// SceneSellOutMaterialItem
constexpr uint32_t F_ROOT = 0x288, F_SCROLL = 0x28C;
constexpr uint32_t API_USER_MATERIALS = 0xE4;   // shared_ptr<SafeBuffer>: {u64 id, material id, count} (16 bytes)
constexpr uint32_t SB_COUNT = 0x20;
constexpr uint32_t MATERIAL_PRICE = 0xD0;       // master::Material: the sale price
constexpr uint32_t API_LOGGED_IN = 0x14;

constexpr int32_t TXT_NO_MATERIALS = 101100011;
constexpr int32_t TXT_SOLD = 100300009, TXT_SOLD_2 = 100300008, TXT_OK = 100100001;
constexpr uint32_t SE_SOLD = 0x1E56B9B8, SE_LAYER = 10000;

constexpr uint32_t S_Txt_None = 0xE04850;
constexpr uint32_t S_SellOkJson = 0xDF9B65;     // "cocostudio/publish/PopupNormal_MedalSell_Ok.json"
constexpr uint32_t S_NormalText_Window01 = 0xDF878E, S_Button_OK = 0xDF8688, S_Txt_OK = 0xDF869F;
constexpr uint32_t S_Txt_SellTotal_Label = 0xDF9B96, S_Txt_Sell_Ok1 = 0xDF9BAA, S_Txt_Sell_Ok2 = 0xDF9BB7;

// __func vtables (+8)
constexpr uint32_t VT_SellError = 0xF649F4;     // $_17 UlbE_ {}
constexpr uint32_t VT_OkTouch = 0xF542A0;       // openSellOkPopup $_81 {}
constexpr uint32_t VT_OkCanClose = 0xF54274;    // $_82 bool() {button}
constexpr uint32_t VT_OkClose = 0xF5421C;       // $_83 {button, window, layer, shadow, popup}

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }

// text->setString(TextData `id`)
void setTextData(uint32_t text, int32_t id) {
    Frame f;
    uint32_t sp = textData(f, id);
    setTextC(text, textDataStr(sp));
    sharedRelease(sp);
}

struct MaterialSale25 {
    // openSellCheckDialog $_17 (bool ok) {scene, shared_ptr<material row> (+8, +0xC), count (+0x10)}
    static void sold(uint32_t functor, uint32_t okRef) {
        uint32_t ok = ld8(okRef);
        uint32_t scene = ld32(functor + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (!ok) {
            if (!ld8(call(fn::APIManager_getInstance, {}) + API_LOGGED_IN)) return;
            uint32_t api = call(fn::APIManager_getInstance, {});
            Frame f;
            uint32_t cb = newFunction(f, VT_SellError, {});
            call(g::APIManager_openErrorDialog, {api, cb, 1});
            functionDestroy(cb);
            return;
        }
        call(g::SceneManager_updateHeader, {sceneManager()});
        Frame f;
        uint32_t ids = f.alloc(12);              // vector<unsigned long long>: the user material ids
        st32(ids, 0);
        st32(ids + 4, 0);
        st32(ids + 8, 0);
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t materials = f.alloc(8);         // shared_ptr copy of the user materials
        st32(materials, ld32(api + API_USER_MATERIALS));
        st32(materials + 4, ld32(api + API_USER_MATERIALS + 4));
        if (ld32(materials + 4)) call(fn::shared_add, {ld32(materials + 4)});
        uint32_t rec = f.alloc(16);
        for (uint32_t i = 0, at = 0; i < ld32(ld32(materials) + SB_COUNT); i++, at += 16) {
            uint32_t sb = ld32(materials);
            const SafeBuffer *s = safe(sb);
            SafeBuffer::read(s, rw::frame_ptr<uint8_t>(rec), s->data.get() + at, 16, at);
            uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(g::VectorULL_pushBackSlow, {ids, rec});
            } else {
                uint32_t next = 0;
                if (end) {
                    st32(end, ld32(rec));
                    st32(end + 4, ld32(rec + 4));
                    next = ld32(ids + 4);
                }
                st32(ids + 4, next + 8);
            }
        }
        uint32_t comp = f.alloc(4);              // the comparator: {&materials}
        st32(comp, materials);
        call(g::MaterialSort, {ld32(ids), ld32(ids + 4), comp});
        call(g::BaseScrollViewULL_setItemIDs, {ld32(scene + F_SCROLL), ids});
        setVisible(findNode(ld32(scene + F_ROOT), S_Txt_None, 8), 0);
        if (ld32(ids + 4) == ld32(ids)) {
            uint32_t none = findNode(ld32(scene + F_ROOT), S_Txt_None, 8);
            call(g::SceneUtil_setShadow, {none});
            setVisible(none, 1);
            setTextData(none, TXT_NO_MATERIALS);
        }
        int32_t price = safe32(ld32(functor + 8), MATERIAL_PRICE);
        call(g::SceneUtil_openSellOkPopup, {ld32(functor + 0x10) * uint32_t(price), 1});
        call(g::Scene_updateListField, {scene});
        sharedRelease(materials);
        call(g::VectorULL_dtor, {ids});
    }

    // $_24 UlvE_ / $_23 UlvE_ {parent, shadow, popup}: remove the dialog
    static void removeDialog(uint32_t functor) {
        uint32_t parent = ld32(functor + 4);
        vcall(parent, node::removeChild, {parent, ld32(functor + 8), 1});
        parent = ld32(functor + 4);
        vcall(parent, node::removeChild, {parent, ld32(functor + 0xC), 1});
    }

    // $_17 __clone(): {scene, shared_ptr<material row>, count}
    static uint32_t cloneSold(uint32_t self) {
        uint32_t obj = call(fn::operator_new, {0x14});
        if (obj) {
            st32(obj, 0xF649C8);
            st32(obj + 4, ld32(self + 4));
            st32(obj + 8, ld32(self + 8));
            uint32_t ctrl = ld32(self + 0xC);
            st32(obj + 0xC, ctrl);
            if (ctrl) call(fn::shared_add, {ctrl});
            st32(obj + 0x10, ld32(self + 0x10));
        }
        return obj;
    }

    // runOutAnimation $_8 {function<void()>}: __clone() and destroy_deallocate()
    static uint32_t cloneRunOut(uint32_t self) {
        uint32_t obj = call(fn::operator_new, {0x20});
        if (obj) {
            st32(obj, 0xF64C04);
            call(fn::function_vv_copy, {obj + 8, self + 8});
        }
        return obj;
    }
    static void destroyRunOut(uint32_t self) {
        functionDestroy(self + 8);
        call(fn::operator_delete, {self});
    }

    // the second copy of the material sort: the same code as init's instantiation
    static uint32_t less(uint32_t comp, uint32_t a, uint32_t b) { return call(g::MaterialLess, {comp, a, b}); }
    static uint32_t sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t comp) {
        return call(g::MaterialSort3, {x, y, z, comp});
    }
    static void sort(uint32_t first, uint32_t last, uint32_t comp) { call(g::MaterialSort, {first, last, comp}); }

    // SceneUtil::openSellOkPopup(munny, bool): "Sold for <munny>" with an OK button, on the popup layer
    static void openSellOkPopup(int32_t munny, uint32_t) {
        {
            Frame f;
            uint32_t layer = f.alloc(8);
            call(g::audio_Manager_getLayer, {layer, call(g::audio_Manager_getInstance, {}), SE_LAYER});
            call(g::audio_Layer_playSE, {ld32(layer), SE_SOLD, 0, 0});
            sharedRelease(layer);
        }
        uint32_t layer = call(g::SceneManager_getPopupLayer, {sceneManager()});
        uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
        uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.15f), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
        uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_SellOkJson});
        // centred on screens wider / taller than the 960x640 design size
        float x = bitsf(vcall(popup, node::getPositionX, {popup}));
        vcall(popup, node::setPositionX, {popup, fbits(x + (visibleSize().width + -960.0f) * 0.5f)});
        float y = bitsf(vcall(popup, node::getPositionY, {popup}));
        vcall(popup, node::setPositionY, {popup, fbits(y + (visibleSize().height + -640.0f) * 0.5f)});
        vcall(shadow, node::addChild, {shadow, popup});
        uint32_t window = findNode(popup, S_NormalText_Window01, 19);
        uint32_t ok = findNode(popup, S_Button_OK, 9);
        addTouch(ok, VT_OkTouch, {});
        titleLabel(ok, S_Txt_OK, 6, TXT_OK);
        uint32_t total = findNode(popup, S_Txt_SellTotal_Label, 19);
        call(g::SceneUtil_setShadowText, {total});
        {
            Frame f;
            uint32_t money = f.alloc(12);
            call(g::SceneUtil_getMoneyStr, {money, 0, uint32_t(munny), uint32_t(munny >> 31)});
            setTextC(total, strData(money));
            strFree(money);
        }
        uint32_t line1 = findNode(popup, S_Txt_Sell_Ok1, 12);
        call(g::SceneUtil_setShadowText, {line1});
        setTextData(line1, TXT_SOLD);
        uint32_t line2 = findNode(popup, S_Txt_Sell_Ok2, 12);
        call(g::SceneUtil_setShadowText, {line2});
        setTextData(line2, TXT_SOLD_2);
        Frame f;
        {
            uint32_t opened = newEmptyFunction(f);
            call(fn::Node_runAction, {window, call(g::SceneUtil_createOpenPopupAnimation, {window, opened})});
            functionDestroy(opened);
        }
        uint32_t mgr = sceneManager();
        uint32_t canClose = newFunction(f, VT_OkCanClose, {ok});
        uint32_t cb = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_OkClose);
            st32(heap + 4, ok);
            st32(heap + 8, window);
            st32(heap + 0xC, layer);
            st32(heap + 0x10, shadow);
            st32(heap + 0x14, popup);
        }
        st32(cb + 16, heap);
        call(g::SceneManager_openPopup, {mgr, canClose, cb, 0});
        functionDestroy(cb);
        functionDestroy(canClose);
    }
};

using thunk::cap;
using CloneDetailDone = thunk::Lambda<0xF64AD0, 0x10, 0, 0, 4, cap(4, 4), cap(8, 4), cap(0xC, 4)>;
}  // namespace

RW_FUNCTION(0x689250, "SceneSellOutMaterialItem::openSellCheckDialog $_17 __func::operator()(bool) (sub_689250)",
            MaterialSale25::sold);
RW_FUNCTION(0x6891A4, "SceneSellOutMaterialItem::openSellCheckDialog $_17 __func::__clone() (sub_6891A4)",
            MaterialSale25::cloneSold);
RW_FUNCTION(0x6890EA, "SceneSellOutMaterialItem::openSellCheckDialog $_24 UlvE_ __func::operator() (sub_6890EA)",
            MaterialSale25::removeDialog);
RW_FUNCTION(0x68A416, "SceneSellOutMaterialItem::openSellDetailPopup $_23 UlvE_ __func::operator() (sub_68A416)",
            MaterialSale25::removeDialog);
RW_FUNCTION(0x68A3E4, "SceneSellOutMaterialItem::openSellDetailPopup $_23 UlvE_ __func::__clone(__base*) (sub_68A3E4)",
            CloneDetailDone::cloneInto);
RW_FUNCTION(0x68AD28, "SceneSellOutMaterialItem::runOutAnimation $_8 __func::__clone() (sub_68AD28)",
            MaterialSale25::cloneRunOut);
RW_FUNCTION(0x68ADCC, "SceneSellOutMaterialItem::runOutAnimation $_8 __func::destroy_deallocate() (sub_68ADCC)",
            MaterialSale25::destroyRunOut);
RW_FUNCTION(0x689598, "std::__sort<$_17 material comparator&, ull*> (sub_689598)", MaterialSale25::sort);
RW_FUNCTION(0x6899D8, "SceneSellOutMaterialItem::openSellCheckDialog $_17 material comparator (sub_6899D8)",
            MaterialSale25::less);
RW_FUNCTION(0x689B54, "std::__sort3<$_17 material comparator&, ull*> (sub_689B54)", MaterialSale25::sort3);
RW_FUNCTION(0x474DCC, "SceneUtil::openSellOkPopup(int, bool)", MaterialSale25::openSellOkPopup);
