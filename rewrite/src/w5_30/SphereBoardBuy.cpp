// SceneSphereBoardSelect::openBoardBuyDialog: buying a Jewel board (an unbought type-2 board of the list). With
// enough Jewels, SphereBoard_Account_ver120.json on popup layer 2 (the board's incentive, board and kind images,
// the price text, the Jewels held) as a SceneManager popup: OK sends /user/sphere/buy (68), then the popup closes
// and the caller's callback(true, false) runs; Cancel / closing calls it with (false, false). Without enough
// Jewels the OK / Cancel dialog of the shop. The callback is a std::function<void(bool, bool)> copied by value
// into each lambda, as the original does (each copy clones the caller's functor).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t StringBuffer_ctor = 0x49140D;           // (buf, fmt, ...)
constexpr uint32_t Misc_create = 0x786D5D, Sphere_create = 0x78CF21;
constexpr uint32_t FunctionVBB_copy = 0x45CD55;            // std::function<void(bool,bool)>(const function&)
constexpr uint32_t FunctionVBB_move = 0x48B1E9;            // std::function<void(bool,bool)>(function&&)
constexpr uint32_t FunctionVBB_call = 0x48B159;            // std::function<void(bool,bool)>::operator()(bool, bool)
constexpr uint32_t SceneUtil_hasMagicStone = 0x45DD29;     // (int price)
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;   // (Text*, const string& name)
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;      // (node, const string& name, text id)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;      // (const string&, const function<void()>&, bool)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t Director_getInstance = 0x8EE519, Director_getTextureCache = 0x8EFE69;
constexpr uint32_t TextureCache_addImage = 0x917DA9;       // (cache, const string&)
constexpr uint32_t ImageView_loadTextureTex = 0x8398FD;    // ImageView::loadTexture(Texture2D*)
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t SM_getPopupLayer2 = 0x613E71, SM_openPopup = 0x61524D, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_updateHeader = 0x611169;
constexpr uint32_t APIManager_executeUserSphereBuy = 0x4CE07D;  // (api, int board, const function<void(bool)>&)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;       // (api, const function<void(bool)>&, bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
}  // namespace g

constexpr uint32_t API_HOLD = 0x94;            // APIManager: a shared_ptr held during the dialog (control at +4)
constexpr uint32_t API_STONE = 0xA4;           // APIManager: the Jewels SafeBuffer (+0 free, +4 paid; control +4)
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t V_SET_SIZE = 0x2B0;         // ui::Widget::setSize(const Size&)
constexpr uint32_t VT_Ok = 0xF66968;           // OK {popup, layer, shadow, -, callback, board} (heap 0x38)
constexpr uint32_t VT_Cancel = 0xF6693C;       // Cancel {}
constexpr uint32_t VT_CanClose = 0xF66910;     // the popup's can-close {close button}
constexpr uint32_t VT_OnClose = 0xF668B8;      // the popup's on-close {popup, layer, shadow, -, callback} (heap 0x30)
constexpr uint32_t VT_ShortOfJewels = 0xF6688C;   // the OK / Cancel dialog {callback} (heap 0x20)
constexpr uint32_t VT_BuyDone = 0xF66994;      // OK's request callback {popup, layer, shadow, -, callback} (heap 0x30)
constexpr uint32_t VT_RemoveNodes = 0xF668E4;  // the close animation's end {layer, shadow, popup}
constexpr uint32_t VT_BuyError = 0xF669C0;     // the error dialog's callback {}
constexpr uint32_t S_AccountJson = 0xE05636, S_SelectBase = 0xDF94D5, S_SelectBoard = 0xDF94E1;
constexpr uint32_t S_SelectKind = 0xDF94EE, S_IncentiveFmt = 0xDF94FA, S_BoardFmt = 0xDF9516, S_KindFmt = 0xDF9536;
constexpr uint32_t S_TxtWording = 0xDF862F, S_TxtCrown = 0xDF8658, S_TxtCrownLabel = 0xDF866B, S_Fmt7d = 0xDF8684;
constexpr uint32_t S_ButtonOK = 0xDF8688, S_TxtOK = 0xDF869F, S_ButtonClose = 0xDF8692, S_TxtCancel = 0xDF86A6;
constexpr int32_t TXT_PRICE = 103400023, TXT_HELD = 100310002, TXT_OK = 100100001, TXT_CANCEL = 100100002;
constexpr int32_t TXT_SHORT = 102600016, TXT_BOUGHT = 103400061;
constexpr uint32_t MISC_111 = 111, DEFAULT_INCENTIVE = 2100001;

void sbRead(uint32_t sb, uint32_t out, uint32_t offset, uint32_t len) {
    rw::call(g::SafeBuffer_read, {sb, out, ld32(sb + 0xC) + offset, len, offset});
}
void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// a function<void(bool,bool)> copied into a new stack object
uint32_t copyFn(Frame &f, uint32_t from) {
    const uint32_t to = f.alloc(20);
    call(g::FunctionVBB_copy, {to, from});
    return to;
}
// a {vt, popup, layer, shadow, -, function moved in} lambda on the heap (0x30)
uint32_t heapPopupLambda(uint32_t vt, uint32_t popup, uint32_t layer, uint32_t shadow, uint32_t fnFrom) {
    const uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, vt);
        st32(obj + 8, popup);
        st32(obj + 12, layer);
        st32(obj + 16, shadow);
        call(g::FunctionVBB_move, {obj + 0x18, fnFrom});
        st32(obj + 8, popup);
        st32(obj + 12, layer);
        st32(obj + 16, shadow);
    }
    return obj;
}
// a texture of the cache by a printf path, or the fallback number's
uint32_t texture(uint32_t fmt, uint32_t value, uint32_t fallback) {
    for (const uint32_t v : {value, fallback}) {
        const uint32_t cache = call(g::Director_getTextureCache, {call(g::Director_getInstance, {})});
        Frame f;
        const uint32_t buf = f.alloc(0x404);
        call(g::StringBuffer_ctor, {buf, fmt, v});
        const uint32_t s = newStr(f, buf);
        const uint32_t tex = call(g::TextureCache_addImage, {cache, s});
        strFree(s);
        if (tex) return tex;
    }
    return 0;
}
// the popup's close animation (then its shadow and layout removed) and the shadow fading out
void closePopup(uint32_t popup, uint32_t layer, uint32_t shadow) {
    Frame f;
    const uint32_t removed = newFunction(f, VT_RemoveNodes, {layer, shadow, popup});
    call(fn::Node_runAction, {popup, call(g::SceneUtil_createClosePopupAnimation, {popup, removed})});
    functionDestroy(removed);
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), 0})});
}
}  // namespace

struct SphereBoardBuy {
    // Returns what the epilogue leaves in r0 (the frame size, 0x2394).
    static uint32_t openBoardBuyDialog(uint32_t /*self*/, uint32_t callback, uint32_t board, uint32_t price) {
        constexpr uint32_t EPILOGUE_R0 = 0x2394;
        const uint32_t holdCtl = ld32(call(fn::APIManager_getInstance, {}) + API_HOLD + 4);
        if (holdCtl) call(fn::shared_add, {holdCtl});
        Frame f;
        {
            const uint32_t row = f.alloc(8), out = f.alloc(4);
            call(g::Misc_create, {row, MISC_111});
            sbRead(ld32(row), out, 4, 4);
            release(ld32(row + 4));
        }
        const uint32_t cbA = copyFn(f, callback), cbB = copyFn(f, callback);
        if (call(g::SceneUtil_hasMagicStone, {price}) == 1) {
            call(fn::AudioPlaySystemSEPopUp, {});
            const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
            const uint32_t shadow = SceneUtil::createShadowSprite();
            const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
            vcall(shadow, node::setOpacity, {shadow, 0});
            call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
            vcall(layer, node::addChild, {layer, shadow});
            const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_AccountJson});
            vcall(layer, node::addChild, {layer, popup});
            vcall(popup, node::setAnchorPoint, {popup, ld32(GOT_ANCHOR_MIDDLE)});
            {
                const uint32_t size = f.alloc(8);
                call(g::Size_ctor, {size, fbits(960.0f), fbits(640.0f)});
                vcall(popup, V_SET_SIZE, {popup, size});
                const float px = bitsf(vcall(popup, node::getPositionX, {popup}));
                const float w = ldf(vcall(popup, node::getContentSize, {popup}));
                const float x = SceneUtil::getSideMargin() + (px + w * 0.5f);
                const float py = bitsf(vcall(popup, node::getPositionY, {popup}));
                const float h = ldf(vcall(popup, node::getContentSize, {popup}) + 4);
                const float y = SceneUtil::getSideHeightMargin() + (py + h * 0.5f);
                vcall(popup, node::setPosition, {popup, newVec2(f, x, y)});
            }
            // the board's images
            const uint32_t sphere = f.alloc(8), incentive = f.alloc(4), boardImage = f.alloc(4), kind = f.alloc(4);
            call(g::Sphere_create, {sphere, board});
            sbRead(ld32(sphere), incentive, 0, 4);
            sbRead(ld32(sphere), boardImage, 0x8C, 4);
            sbRead(ld32(sphere), kind, 0x88, 4);
            const uint32_t selectBase = findNode(popup, S_SelectBase, 11);
            const uint32_t selectBoard = findNode(popup, S_SelectBoard, 12);
            const uint32_t selectKind = findNode(popup, S_SelectKind, 11);
            call(g::ImageView_loadTextureTex, {selectBase, texture(S_IncentiveFmt, ld32(incentive), DEFAULT_INCENTIVE)});
            call(g::ImageView_loadTextureTex, {selectBoard, texture(S_BoardFmt, ld32(boardImage), 1)});
            if (ld32(kind)) {
                call(g::ImageView_loadTextureTex, {selectKind, texture(S_KindFmt, ld32(kind), 1)});
                vcall(selectKind, node::setVisible, {selectKind, 1});
            } else {
                vcall(selectKind, node::setVisible, {selectKind, 0});
            }
            // the price, the Jewels held
            {
                const uint32_t wording = findNode(popup, S_TxtWording, 11);
                const uint32_t name = newStr(f, S_TxtWording, 11);
                const uint32_t view = call(g::SceneUtil_textToCustomTextView, {wording, name});
                strFree(name);
                const uint32_t td = textData(f, TXT_PRICE);
                const uint32_t buf = f.alloc(0x404);
                call(g::StringBuffer_ctor, {buf, textDataStr(td), price});
                const uint32_t s = newStr(f, buf);
                call(g::CustomTextView_setXml, {view, s});
                strFree(s);
                sharedRelease(td);
            }
            {
                const uint32_t held = findNode(popup, S_TxtCrown, 18);
                SceneUtil::setShadow(held);
                setTextId(held, TXT_HELD);
            }
            const uint32_t heldLabel = findNode(popup, S_TxtCrownLabel, 24);
            SceneUtil::setShadow(heldLabel);
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t stoneCtl = ld32(api + API_STONE + 4), stone = ld32(api + API_STONE);
            if (stoneCtl) call(fn::shared_add, {stoneCtl});
            {
                const uint32_t paid = f.alloc(4), free = f.alloc(4);
                sbRead(stone, paid, 4, 4);
                sbRead(stone, free, 0, 4);
                const uint32_t buf = f.alloc(0x404);
                call(g::StringBuffer_ctor, {buf, S_Fmt7d, ld32(free) + ld32(paid)});
                setTextC(heldLabel, buf);
            }
            // OK: the purchase
            const uint32_t ok = findNode(popup, S_ButtonOK, 9);
            {
                const uint32_t cb = copyFn(f, cbA);
                const uint32_t fo = f.alloc(20);
                st32(fo + 16, 0);
                const uint32_t obj = call(fn::operator_new, {0x38});
                if (obj) {
                    st32(obj, VT_Ok);
                    st32(obj + 8, popup);
                    st32(obj + 12, layer);
                    st32(obj + 16, shadow);
                    call(g::FunctionVBB_move, {obj + 0x18, cb});
                    st32(obj + 0x30, board);
                }
                st32(fo + 16, obj);
                call(fn::Widget_addTouchEventListener, {ok, fo});
                functionDestroy(fo);
                functionDestroy(cb);
            }
            {
                const uint32_t name = newStr(f, S_TxtOK, 6);
                call(g::SceneUtil_settingTitleLabel, {ok, name, uint32_t(TXT_OK)});
                strFree(name);
            }
            const uint32_t close = findNode(popup, S_ButtonClose, 12);
            {
                const uint32_t fo = newFunction(f, VT_Cancel, {});
                call(fn::Widget_addTouchEventListener, {close, fo});
                functionDestroy(fo);
                const uint32_t name = newStr(f, S_TxtCancel, 10);
                call(g::SceneUtil_settingTitleLabel, {close, name, uint32_t(TXT_CANCEL)});
                strFree(name);
            }
            // the SceneManager popup: can close {close}, on close {popup, layer, shadow, callback}
            const uint32_t cbC = copyFn(f, cbA), cbD = copyFn(f, cbC);
            const uint32_t sm = sceneManager();
            const uint32_t canClose = newFunction(f, VT_CanClose, {close});
            const uint32_t cbE = copyFn(f, cbD);
            const uint32_t onClose = f.alloc(20);
            st32(onClose + 16, 0);
            st32(onClose + 16, heapPopupLambda(VT_OnClose, popup, layer, shadow, cbE));
            call(g::SM_openPopup, {sm, canClose, onClose, 1});
            functionDestroy(onClose);
            functionDestroy(cbE);
            functionDestroy(canClose);
            {
                const uint32_t none = newEmptyFunction(f);
                call(fn::Node_runAction, {popup, SceneUtil::createOpenPopupAnimation(popup, none)});
                functionDestroy(none);
            }
            functionDestroy(cbD);
            functionDestroy(cbC);
            release(stoneCtl);
            release(ld32(sphere + 4));
        } else {
            // not enough Jewels: the shop's OK / Cancel dialog with the callback
            const uint32_t cb = copyFn(f, cbB);
            const uint32_t td = textData(f, TXT_SHORT);
            const uint32_t s = newStr(f, textDataStr(td));
            const uint32_t cb2 = copyFn(f, cb);
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x20});
            if (obj) {
                st32(obj, VT_ShortOfJewels);
                call(g::FunctionVBB_move, {obj + 8, cb2});
            }
            st32(fo + 16, obj);
            call(g::SceneUtil_openOkCancelDialog4Line, {s, fo, 0, 0});
            functionDestroy(fo);
            functionDestroy(cb2);
            strFree(s);
            sharedRelease(td);
            functionDestroy(cb);
        }
        functionDestroy(cbB);
        functionDestroy(cbA);
        release(holdCtl);
        return EPILOGUE_R0;
    }

    // OK {popup, layer, shadow, -, callback, board}: /user/sphere/buy, the popup closed first
    static void okTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        call(g::SM_closeCurrentPopup, {sceneManager(), 1});
        call(g::AudioPlaySystemSEDecide, {});
        const uint32_t popup = ld32(functor + 8), layer = ld32(functor + 12), shadow = ld32(functor + 16);
        Frame f;
        const uint32_t cb = copyFn(f, functor + 0x18);
        call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t board = ld32(functor + 0x30);
        const uint32_t cb2 = copyFn(f, cb);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        st32(fo + 16, heapPopupLambda(VT_BuyDone, popup, layer, shadow, cb2));
        call(g::APIManager_executeUserSphereBuy, {api, board, fo});
        functionDestroy(fo);
        functionDestroy(cb2);
        functionDestroy(cb);
    }

    // the request callback's __clone()
    static uint32_t buyDoneClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, VT_BuyDone);
            for (uint32_t k = 8; k < 0x14; k += 4) st32(obj + k, ld32(self + k));
            call(g::FunctionVBB_copy, {obj + 0x18, self + 0x18});
            for (uint32_t k = 8; k < 0x14; k += 4) st32(obj + k, ld32(self + k));
        }
        return obj;
    }

    // the request callback (bool ok): the header updated, the popup closed, callback(true, false), "bought";
    // or the error dialog
    static void buyDone(uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef);
        call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
        Frame f;
        if (!ok) {
            if (!ld8(call(fn::APIManager_getInstance, {}) + 0x14)) return;
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t cb = newFunction(f, VT_BuyError, {});
            call(g::APIManager_openErrorDialog, {api, cb, 1});
            functionDestroy(cb);
            return;
        }
        call(g::SM_updateHeader, {sceneManager()});
        closePopup(ld32(functor + 8), ld32(functor + 12), ld32(functor + 16));
        call(g::FunctionVBB_call, {functor + 0x18, 1, 0});
        const uint32_t td = textData(f, TXT_BOUGHT);
        const uint32_t s = newStr(f, textDataStr(td));
        const uint32_t none = newEmptyFunction(f);
        call(g::SceneUtil_openOkDialog4Line, {s, none, 0});
        functionDestroy(none);
        strFree(s);
        sharedRelease(td);
    }

    // the popup's on-close (bool by the manager): unless the purchase closed it, the close animation and
    // callback(false, false)
    static void popupClosed(uint32_t functor, uint32_t byManagerRef) {
        if (ld8(byManagerRef)) return;
        call(g::AudioPlaySystemSECancel, {});
        closePopup(ld32(functor + 8), ld32(functor + 12), ld32(functor + 16));
        call(g::FunctionVBB_call, {functor + 0x18, 0, 0});
    }

    // the close animation's end {layer, shadow, popup}
    static void removeNodes(uint32_t functor) {
        const uint32_t layer = ld32(functor + 4);
        vcall(layer, node::removeChild, {layer, ld32(functor + 8), 1});
        vcall(ld32(functor + 4), node::removeChild, {ld32(functor + 4), ld32(functor + 12), 1});
    }
};

RW_FUNCTION(0x6BA138, "SceneSphereBoardSelect::openBoardBuyDialog", SphereBoardBuy::openBoardBuyDialog);
RW_FUNCTION(0x6BDA34, "SceneSphereBoardSelect::openBoardBuyDialog::$_12::lambda(Ref*, TouchEventType) operator() (OK)",
            SphereBoardBuy::okTouched);
RW_FUNCTION(0x6BDCCC, "SceneSphereBoardSelect::openBoardBuyDialog OK request callback __clone() (sub_6BDCCC)",
            SphereBoardBuy::buyDoneClone);
RW_FUNCTION(0x6BDDD8, "SceneSphereBoardSelect::openBoardBuyDialog OK request callback operator() (sub_6BDDD8)",
            SphereBoardBuy::buyDone);
RW_FUNCTION(0x6BD58C, "SceneSphereBoardSelect::openBoardBuyDialog popup on-close operator() (sub_6BD58C)",
            SphereBoardBuy::popupClosed);
RW_FUNCTION(0x6BD72E, "SceneSphereBoardSelect::openBoardBuyDialog close-animation end operator() (sub_6BD72E)",
            SphereBoardBuy::removeNodes);
