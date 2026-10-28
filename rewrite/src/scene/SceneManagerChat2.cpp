// SceneManager: the chat popup (message list, input box, send / stamp / close buttons) and the stamp list popup.
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
// ---- SceneManager chat fields (not in the layout yet; see SceneManagerChat.cpp)
constexpr uint32_t CHAT_BADGE = 0x3FC;           // cocos2d::Node*: the "new chat message" mark
constexpr uint32_t CHAT_LAYOUT = 0x400;          // the chat popup's input panel (Panel_1)
constexpr uint32_t CHAT_SCROLL = 0x404;          // ChatScrollView*
constexpr uint32_t CHAT_TOTAL = 0x408;           // uint32
constexpr uint32_t CHAT_START = 0x40C;           // uint32
constexpr uint32_t CHAT_410 = 0x410;             // 0x14 bytes cleared on open (0x410..0x423: last seen id, scroll id...)
constexpr uint32_t CHAT_42C = 0x42C;
constexpr uint32_t CHAT_HIDDEN_IDS = 0x43C;      // std::vector<uint32>
constexpr uint32_t CHAT_BLOCKED_IDS = 0x448;     // std::vector<std::string>
constexpr uint32_t CHAT_POPUP_SHOWN = 0x46C;     // uint8 (chatPopupDisp)
constexpr uint32_t CHAT_REQUESTING = 0x46D, CHAT_LOADING_PAGE = 0x46E, CHAT_RELOAD_VIEW = 0x46F;
constexpr uint32_t CHAT_OPENING = 0x470;         // uint8: set when the popup opens
constexpr uint32_t CHAT_TEXT_FIELD = 0x478;      // ui::TextField* Txt_Chat_Label (cleared once replaced by the edit box)
constexpr uint32_t CHAT_ON_CLOSE = 0x480;        // std::function<void()>: closes the chat popup
constexpr uint32_t CHAT_TIMER = 0x474;           // float: seconds to the next poll
constexpr uint32_t CHAT_EDITING = 0x4A5;         // uint8: the edit box is being edited
constexpr uint32_t CHAT_EDIT_BOX = 0x4AC;        // cocos2d::extension::EditBox* (from textFieldToEditBox)
constexpr uint32_t CHAT_EDIT_LAYER = 0x4B0;      // the edit box's layer (textFieldToEditBox output)
constexpr uint32_t CHAT_SEND_DELEGATE = 0x4B4;   // ChatEditBoxMessageSend* (created once)

// ChatEditBoxMessageSend
constexpr uint32_t SEND_EDIT_BOX_REF = 0x2A8, SEND_LABEL_REF = 0x2AC;   // EditBox** / Label** for textFieldToEditBox
constexpr uint32_t SEND_DELEGATE_BASE = 0x288;                          // its EditBoxDelegate base
constexpr uint32_t SEND_ON_CHANGE = 0x290;                              // std::function<void()>

// APIManager
constexpr uint32_t API_USER_HEADER = 0x84;       // shared_ptr<user header>: SafeBuffer (chat enabled byte at 16),
                                                 // rank at +0x2C, name std::string at +0x30
constexpr uint32_t API_USER_MAP = 0x1FC;         // shared_ptr<...>
constexpr uint32_t API_USER_STATUS = 0x1D4;      // shared_ptr<...>
constexpr uint32_t HEADER_RANK = 0x2C, HEADER_NAME = 0x30;

constexpr uint32_t Memset = 0x40E7A0;            // __aeabi_memset(dst, n, value)
constexpr uint32_t Memcpy = 0x40E164;            // __aeabi_memcpy(dst, src, n)
constexpr uint32_t ChatScrollView_create = 0x556961;
constexpr uint32_t ChatEditBoxMessageSend_create = 0x55C6E9;
constexpr uint32_t FunctionV_swap = 0x4EE86D;    // std::function<void()>::swap
constexpr uint32_t FunctionB_assign = 0x490109;  // std::function<void(bool)>::operator=
constexpr uint32_t TextField_setMaxLength = rwlib::cocos2d::ui::TextField::setMaxLength;
constexpr uint32_t Label_setAlignment = rwlib::cocos2d::Label::setAlignment;
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright, Widget_isBright = rwlib::cocos2d::ui::Widget::isBright;
constexpr uint32_t Text_setTextHorizontalAlignment = rwlib::cocos2d::ui::Text::setTextHorizontalAlignment;
constexpr uint32_t Text_getStringLength = rwlib::cocos2d::ui::Text::getStringLength;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;   // Vec2(float, float), ~Vec2
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                         // Size(float, float)
constexpr uint32_t APIManager_executePaymentInfo = 0x4CE74D;
constexpr uint32_t ChatPopup_fetch = 0x61507D;   // openChatPopup $_43::operator() (on the captures, called directly)
constexpr uint32_t ChatPopup_close = 0x631931;   // the chat popup's close ({this, root, layer, shadow}; chatPopupClose below)
constexpr uint32_t LoadingIndicator_hide = 0x509D69;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;   // (mgr, bool) -> bool
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t COLOR3B_GRAY = 0x1007EA0, COLOR3B_BLACK = 0x1007E9A;
constexpr uint32_t TEXTFIELD_PLACEHOLDER = 0x150;   // vtable byte offset: returns the place holder (sret)
constexpr uint32_t WIDGET_SET_TOUCH_ENABLED = 0x29C;

// lambdas
constexpr uint32_t VT_Chat_Close = 0xF60138;     // $_38 (heap 0x20): [this, root, shadow, opacity, send, stamp, close]
constexpr uint32_t VT_Chat_CloseTouch = 0xF6010C;   // $_39 [this]
constexpr uint32_t VT_Chat_TextChanged = 0xF600E0;  // $_40 (heap 0x18): [this, maxLength, label2, send, sendText]
constexpr uint32_t VT_Chat_SendTouch = 0xF60030; // $_41 (heap 0x88)
constexpr uint32_t VT_Chat_StampTouch = 0xF60004;   // $_42 [this]
constexpr uint32_t VT_Chat_Payment = 0xF5FFAC;   // $_44 (heap 0x54): the 0x50-byte capture block
constexpr uint32_t VT_Chat_CanClose = 0xF5FF80;  // $_46 [close button]
constexpr uint32_t VT_Chat_OnClose = 0xF5FF54;   // $_45 (heap 0x18): [this, this, root, layer, shadow]
constexpr uint32_t VT_Chat_Closed = 0xF60054;    // the close animation's end (heap 0x14): [layer, this, shadow, root]

constexpr uint32_t STR_Empty = 0xDF8A1A;
constexpr uint32_t STR_ChatSceneJson = 0xE032CB;  // "cocostudio/publish/ChatScene.json"
constexpr uint32_t STR_CloseButton = 0xDF8D2F, STR_CenterUIInput = 0xDFF18F, STR_Panel1 = 0xE02A0D;
constexpr uint32_t STR_TxtChatLabel = 0xDFED6A, STR_TxtStamp = 0xE032ED, STR_TxtSend = 0xE0064F;
constexpr uint32_t STR_TxtLimit = 0xDF9EED, STR_Area = 0xDF9378, STR_ButtonSend = 0xE00637;
constexpr uint32_t STR_ButtonStamp = 0xE032F7, STR_TxtChatLabel2 = 0xE03304, STR_TxtName = 0xDFDA10;
constexpr uint32_t STR_RankIcon = 0xDFDA07;
constexpr uint32_t STR_CalligraphyFont = 0xDF9F1F;   // "cocostudio/publish/Cinema_Calligraphy_Web.ttf"

constexpr int32_t TEXT_CHAT_UNAVAILABLE = 101000027;
constexpr int32_t TEXT_STAMP = 104200002, TEXT_SEND = 104200001, TEXT_LIMIT = 104200031;
constexpr uint32_t MISC_CHAT_MAX_LENGTH = 809;

uint32_t mgrAddr(SceneManager *self) { return rw::guest_addr(self); }

// node->setXxx(Vec2(x, y)) through a temporary cocos2d::Vec2 (constructed and destroyed like the original)
void vcallVec2(uint32_t node, uint32_t slot, float x, float y) {
    uint32_t fnp = ld32(ld32(node) + slot);
    Frame f;
    uint32_t v = f.alloc(8);
    call(Vec2_ctor, {v, fbits(x), fbits(y)});
    call(fnp, {node, v});
    call(Vec2_dtor, {v});
}

}  // namespace

// Open the chat: reset the chat state, build ChatScene.json on the popup layer (message list, input edit box,
// send / stamp / close buttons, the player's name and rank), then either fetch the chat data behind the loading
// indicator (chat enabled) or show it directly. Without a user map or with the chat disabled: a notice.
void SceneManager::openChatPopup(SceneManager *self) {
    const uint32_t m = mgrAddr(self);
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t headerCtl = ld32(api + API_USER_HEADER + 4);
    uint32_t header = ld32(api + API_USER_HEADER);
    if (headerCtl) call(fn::shared_add, {headerCtl});
    uint32_t api2 = call(fn::APIManager_getInstance, {});
    uint32_t mapCtl = ld32(api2 + API_USER_MAP + 4);
    uint32_t users = ld32(api2 + API_USER_MAP);
    if (mapCtl) call(fn::shared_add, {mapCtl});
    uint8_t chatEnabled;
    {
        Frame f;
        chatEnabled = ld8(safeBytes(f, header, 16, 1));
    }
    if (!users || !chatEnabled) {
        Frame f;
        uint32_t text = textData(f, TEXT_CHAT_UNAVAILABLE);
        uint32_t message = newStr(f, textDataStr(text));
        uint32_t none = newEmptyFunction(f);
        SceneUtil::openOkDialog4Line(message, none, 0);
        functionDestroy(none);
        strFree(message);
        sharedRelease(text);
        if (mapCtl) call(fn::shared_release, {mapCtl});
        if (headerCtl) call(fn::shared_release, {headerCtl});
        return;
    }

    // ---- reset the chat state
    st8(m + CHAT_OPENING, 1);
    uint32_t badge = ld32(m + CHAT_BADGE);
    if (badge && vcall(badge, node::isVisible, {badge}) == 1)
        vcall(ld32(m + CHAT_BADGE), node::setVisible, {ld32(m + CHAT_BADGE), 0});
    st32(m + CHAT_START, 1);
    st32(m + CHAT_TOTAL, 0);
    st8(m + CHAT_REQUESTING, 0);
    st8(m + CHAT_POPUP_SHOWN, 0);
    call(Memset, {m + CHAT_410, 0x14, 0});
    st8(m + CHAT_RELOAD_VIEW, 0);
    st8(m + CHAT_LOADING_PAGE, 1);
    {
        // hidden ids: clear (the inlined vector<uint32>::clear arithmetic)
        uint32_t end = ld32(m + CHAT_HIDDEN_IDS + 4), begin = ld32(m + CHAT_HIDDEN_IDS);
        if (end != begin) st32(m + CHAT_HIDDEN_IDS + 4, end + (~(end - 4 - begin) & 0xFFFFFFFCu));
        // blocked ids: destroy the strings from the back
        uint32_t ids = m + CHAT_BLOCKED_IDS;
        uint32_t e = ld32(ids + 4), b = ld32(ids);
        while (e != b) {
            uint32_t s = e - 12;
            st32(ids + 4, s);
            if (strIsLong(s)) {
                call(fn::operator_delete, {ld32(s + 8)});
                s = ld32(ids + 4);
            }
            e = s;
        }
    }
    st32(m + CHAT_42C, 0);
    st8(m + CHAT_EDITING, 0);

    // ---- the popup: a shadow and ChatScene.json on the popup layer
    uint32_t layer = getInstance()->popupLayer;
    uint32_t shadow = SceneUtil::createShadowSprite();
    uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    vcall(layer, node::addChildZ, {layer, shadow, 0});
    uint32_t root;
    {
        Frame f;
        uint32_t json = newStr(f, STR_ChatSceneJson, 0x21);
        root = SceneUtil::createNodeWithSceneFile(json);
        strFree(json);
    }
    vcall(root, node::setVisible, {root, 0});
    vcall(layer, node::addChildZ, {layer, root, 1});
    // the captures of the show lambda: {this, root, layer, shadow} after a leading `this`
    Frame keep;
    const uint32_t shown = keep.alloc(20);
    st32(shown, m);
    st32(shown + 4, m);
    st32(shown + 8, root);
    st32(shown + 12, layer);
    st32(shown + 16, shadow);

    uint32_t closeBase = findNode(root, STR_CloseButton, 12);
    uint32_t centerInput = findNode(root, STR_CenterUIInput, 14);
    {
        Frame f;
        uint32_t name = newStr(f, STR_Panel1, 7);
        st32(m + CHAT_LAYOUT, SceneUtil::searchNodeByName(centerInput, name));
        strFree(name);
    }
    // move the input panel from its parent to CenterUI_Input (z 2, below)
    uint32_t panelParent = vcall(ld32(m + CHAT_LAYOUT), node::getParent, {ld32(m + CHAT_LAYOUT)});
    vcall(panelParent, node::removeChild, {panelParent, ld32(m + CHAT_LAYOUT), 1});
    {
        Frame f;
        uint32_t name = newStr(f, STR_TxtChatLabel, 14);
        st32(m + CHAT_TEXT_FIELD, SceneUtil::searchNodeByName(ld32(m + CHAT_LAYOUT), name));
        strFree(name);
    }
    int32_t maxLength;
    {
        Frame f;
        uint32_t row = masterRow(f, fn::Misc_create, MISC_CHAT_MAX_LENGTH);
        maxLength = int32_t(ld32(safeBytes(f, ld32(row), 4, 4)));
        sharedRelease(row);
    }
    call(TextField_setMaxLength, {ld32(m + CHAT_TEXT_FIELD), uint32_t(maxLength)});
    uint32_t stampText = findNode(ld32(m + CHAT_LAYOUT), STR_TxtStamp, 9);
    SceneUtil::setShadow(stampText);
    setTextId(stampText, TEXT_STAMP);
    uint32_t sendText = findNode(ld32(m + CHAT_LAYOUT), STR_TxtSend, 8);
    SceneUtil::setShadow(sendText);
    setTextId(sendText, TEXT_SEND);
    vcall(sendText, node::setColor, {sendText, COLOR3B_GRAY});
    uint32_t limitText = findNode(root, STR_TxtLimit, 9);
    SceneUtil::setShadow(limitText);
    {
        Frame f;
        uint32_t text = textData(f, TEXT_LIMIT);
        uint32_t buf = stringBuffer(f, {textDataStr(text), uint32_t(maxLength)});
        setTextC(limitText, buf);
        sharedRelease(text);
    }
    vcall(centerInput, node::addChildZ, {centerInput, ld32(m + CHAT_LAYOUT), 2});

    // ---- the message list
    uint32_t area = findNode(root, STR_Area, 4);
    uint32_t view = call(ChatScrollView_create, {});
    st32(m + CHAT_SCROLL, view);
    uint32_t setSize = ld32(ld32(view) + node::setContentSize);
    call(setSize, {view, vcall(area, node::getContentSize, {area})});
    vcall(area, node::addChild, {area, ld32(m + CHAT_SCROLL)});
    if (!ld32(m + CHAT_SEND_DELEGATE)) {
        uint32_t delegate = call(ChatEditBoxMessageSend_create, {});
        st32(m + CHAT_SEND_DELEGATE, delegate);
        vcall(layer, node::addChildZ, {layer, delegate, 4});
    }
    uint32_t sendButton = findNode(ld32(m + CHAT_LAYOUT), STR_ButtonSend, 11);
    uint32_t stampButton = findNode(ld32(m + CHAT_LAYOUT), STR_ButtonStamp, 12);
    uint32_t closeButton = findNode(closeBase, STR_CloseButton, 12);
    vcall(closeButton, WIDGET_SET_TOUCH_ENABLED, {closeButton, 0});
    {
        // this->onClose (+0x480) = $_38
        Frame f;
        uint32_t fo = f.alloc(20);
        heapLambda(fo, 0x20, VT_Chat_Close, {m, root, shadow, opacity, sendButton, stampButton, closeButton});
        call(FunctionV_swap, {fo, m + CHAT_ON_CLOSE});
        functionDestroy(fo);
    }
    addTouch(closeButton, VT_Chat_CloseTouch, {m});
    uint32_t label2 = findNode(root, STR_TxtChatLabel2, 15);
    setTextC(label2, STR_Empty);

    // ---- the input: the text field becomes an edit box (ChatEditBoxMessageSend is its delegate)
    {
        uint32_t delegate = ld32(m + CHAT_SEND_DELEGATE);
        uint32_t labelRef = delegate + SEND_LABEL_REF, editRef = delegate + SEND_EDIT_BOX_REF;
        uint32_t field = ld32(m + CHAT_TEXT_FIELD);
        Frame f;
        uint32_t placeholder = f.alloc(12);
        call(ld32(ld32(field) + TEXTFIELD_PLACEHOLDER), {placeholder, field});
        uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fbits(500.0f), fbits(52.0f)});
        uint32_t d = ld32(m + CHAT_SEND_DELEGATE);
        uint32_t delegateBase = d ? d + SEND_DELEGATE_BASE : 0;
        uint32_t editBox = SceneUtil::textFieldToEditBox(editRef, labelRef, field, placeholder, STR_Empty,
                                                          STR_CalligraphyFont, size, delegateBase, m + CHAT_EDITING,
                                                          m + CHAT_EDIT_LAYER, maxLength);
        st32(m + CHAT_EDIT_BOX, editBox);
        strFree(placeholder);
    }
    vcall(ld32(m + CHAT_EDIT_BOX), node::setColor, {ld32(m + CHAT_EDIT_BOX), COLOR3B_BLACK});
    vcall(ld32(m + CHAT_EDIT_BOX), node::setVisible, {ld32(m + CHAT_EDIT_BOX), 0});
    st32(m + CHAT_TEXT_FIELD, 0);
    {
        // delegate->onChange = $_40
        uint32_t target = ld32(m + CHAT_SEND_DELEGATE) + SEND_ON_CHANGE;
        Frame f;
        uint32_t fo = f.alloc(20);
        heapLambda(fo, 0x18, VT_Chat_TextChanged, {m, uint32_t(maxLength), label2, sendButton, sendText});
        call(FunctionV_swap, {fo, target});
        functionDestroy(fo);
    }
    {
        uint32_t editBox = ld32(m + CHAT_EDIT_BOX);
        call(Label_setAlignment, {editBox, 0, ld32(editBox + 0x3E0)});
    }
    call(Widget_setBright, {sendButton, 0});
    SceneUtil::setButtonEnabled(sendButton, sendText, call(Widget_isBright, {sendButton}), 0);
    {
        // send button: $_41 (heap 0x88), the captures of the nested send / close / show lambdas
        Frame f;
        uint32_t fo = f.alloc(20);
        heapLambda(fo, 0x88, VT_Chat_SendTouch,
                   {m, sendButton, stampButton, closeButton, m, label2, sendButton, sendText, m, root, shadow,
                    opacity, sendButton, stampButton, closeButton, stampButton, closeButton, m, m, root, layer, shadow,
                    sendButton, stampButton, closeButton, m, m, root, layer, shadow, sendButton, stampButton,
                    closeButton});
        call(fn::Widget_addTouchEventListener, {sendButton, fo});
        functionDestroy(fo);
    }
    addTouch(stampButton, VT_Chat_StampTouch, {m});

    // ---- the player's name and rank icon
    uint32_t nameText = findNode(root, STR_TxtName, 8);
    SceneUtil::setShadow(nameText);
    call(fn::Text_setString, {nameText, header + HEADER_NAME});
    call(Text_setTextHorizontalAlignment, {nameText, 0});
    vcallVec2(nameText, node::setAnchorPoint, 0.0f, 0.5f);
    {
        uint32_t setPos = ld32(ld32(nameText) + node::setPosition);
        float x = float(int32_t(call(Text_getStringLength, {nameText}) - 1u)) * -11.0f + 22.0f;
        Frame f;
        uint32_t v = f.alloc(8);
        call(Vec2_ctor, {v, fbits(x), 0});
        call(setPos, {nameText, v});
        call(Vec2_dtor, {v});
    }
    call(fn::Text_setString, {nameText, header + HEADER_NAME});
    uint32_t rankIcon = findNode(root, STR_RankIcon, 8);
    {
        Frame f;
        uint32_t path = f.alloc(12);
        SceneUtil::createPartyRankImagePath(path, int32_t(ld32(header + HEADER_RANK)));
        uint32_t s = newStr(f, strData(path));
        call(fn::ImageView_loadTexture, {rankIcon, s, 0});
        strFree(s);
        strFree(path);
    }
    {
        uint32_t setPos = ld32(ld32(rankIcon) + node::setPosition);
        float x = float(int32_t(call(Text_getStringLength, {nameText}) - 1u)) * -11.0f + -10.0f;
        Frame f;
        uint32_t v = f.alloc(8);
        call(Vec2_ctor, {v, fbits(x), 0});
        call(setPos, {rankIcon, v});
        call(Vec2_dtor, {v});
    }

    // ---- the fetch lambda's captures (0x50 bytes), copied by value
    const uint32_t fetch = keep.alloc(0x50), fetchCopy = keep.alloc(0x50);
    {
        const uint32_t words[20] = {sendButton, sendText, m, m, m, m, root, layer, shadow, sendButton, stampButton,
                                    closeButton, m, m, root, layer, shadow, sendButton, stampButton, closeButton};
        for (uint32_t k = 0; k < 20; k++) st32(fetch + 4 * k, words[k]);
    }
    call(Memcpy, {fetchCopy, fetch, 0x50});
    uint8_t enabledNow;
    {
        Frame f;
        enabledNow = ld8(safeBytes(f, header, 16, 1));
    }
    if (enabledNow) {
        uint32_t api3 = call(fn::APIManager_getInstance, {});
        uint32_t statusCtl = ld32(api3 + API_USER_STATUS + 4);
        uint32_t status = ld32(api3 + API_USER_STATUS);
        if (statusCtl) call(fn::shared_add, {statusCtl});
        SceneManager *mgr = getInstance();
        {
            Frame f;
            uint32_t none = newEmptyFunction(f);
            mgr->loadingBlocked = 1;
            call(FunctionB_assign, {mgrAddr(mgr) + 0x548 /* onFinishLoading */, none});
            functionDestroy(none);
        }
        enableLoadingIndicator(getInstance(), 1);
        if (status) {
            call(ChatPopup_fetch, {fetch});
        } else {
            // fetch after the payment info request ($_44 holds a copy of the captures)
            uint32_t api4 = call(fn::APIManager_getInstance, {});
            Frame f;
            uint32_t copy = f.alloc(0x50);
            call(Memcpy, {copy, fetchCopy, 0x50});
            uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            uint32_t heap = call(fn::operator_new, {0x54});
            if (heap) {
                st32(heap, VT_Chat_Payment);
                call(Memcpy, {heap + 4, copy, 0x50});
            }
            st32(fo + 16, heap);
            call(APIManager_executePaymentInfo, {api4, fo});
            functionDestroy(fo);
        }
        if (statusCtl) call(fn::shared_release, {statusCtl});
        SceneManager *popupMgr = getInstance();
        Frame f;
        uint32_t canClose = newFunction(f, VT_Chat_CanClose, {closeButton});
        uint32_t onClose = f.alloc(20);
        heapLambda(onClose, 0x18, VT_Chat_OnClose,
                   {ld32(shown), ld32(shown + 4), ld32(shown + 8), ld32(shown + 12), ld32(shown + 16)});
        call(fn::SceneManager_openPopup, {mgrAddr(popupMgr), canClose, onClose, 1});
        functionDestroy(onClose);
        functionDestroy(canClose);
    } else {
        st8(ld32(shown) + CHAT_EDITING, 1);
        call(ChatPopup_close, {shown + 4});
    }
    if (mapCtl) call(fn::shared_release, {mapCtl});
    if (headerCtl) call(fn::shared_release, {headerCtl});
}
RW_FUNCTION(0x613E9C, "SceneManager::openChatPopup", SceneManager::openChatPopup);

namespace {
// the loading indicator hidden when it is shown (on a parent)
void hideLoadingIndicator() {
    SceneManager *mgr = SceneManager::getInstance();
    const uint32_t indicator = mgr->loadingIndicator;
    if (vcall(indicator, node::getParent, {indicator})) call(LoadingIndicator_hide, {mgr->loadingIndicator});
}

// The chat popup's close {this, root, layer, shadow}: the chat flags and poll timer cleared, the loading indicator
// hidden, the popup shrinks (a heap lambda {layer, this, shadow, root} ends the close) and the shadow fades out.
void chatPopupClose(uint32_t cap) {
    const uint32_t self = ld32(cap);
    st8(self + CHAT_POPUP_SHOWN, 0);
    st8(self + CHAT_REQUESTING, 0);
    st32(self + CHAT_TIMER, 0);
    hideLoadingIndicator();
    const uint32_t root = ld32(cap + 4), layer = ld32(cap + 8), shadow = ld32(cap + 12);
    Frame f;
    const uint32_t fo = f.alloc(20);
    heapLambda(fo, 0x14, VT_Chat_Closed, {layer, self, shadow, root});
    call(fn::Node_runAction, {root, SceneUtil::createClosePopupAnimation(root, fo)});
    functionDestroy(fo);
    call(fn::Node_runAction, {ld32(cap + 12), call(fn::FadeTo_create, {fbits(0.1f), 0})});
}

// A chat request failed {this, this, root, layer, shadow, send, stamp, close}(message): the loading indicator
// hidden; with the popup shown its buttons are enabled again, else the popup (when it is the current one) closes;
// then the message in an OK dialog.
void chatRequestFailed(uint32_t cap, uint32_t message) {
    const uint32_t self = ld32(cap);
    st8(self + CHAT_REQUESTING, 0);
    hideLoadingIndicator();
    if (ld8(self + CHAT_POPUP_SHOWN)) {
        for (uint32_t k = 0x14; k <= 0x1C; k += 4) vcall(ld32(cap + k), WIDGET_SET_TOUCH_ENABLED, {ld32(cap + k), 1});
        st8(self + CHAT_EDITING, 0);
    } else if (!call(SceneManager_closeCurrentPopup, {mgrAddr(SceneManager::getInstance()), 1})) {
        st8(self + CHAT_EDITING, 1);
        call(ChatPopup_close, {cap + 4});
    }
    call(AudioPlaySystemSECancel, {});
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    SceneUtil::openOkDialog4Line(message, none, 0);
    functionDestroy(none);
}
}  // namespace

RW_FUNCTION(0x631930, "SceneManager chat popup close (sub_631930)", chatPopupClose);
RW_FUNCTION(0x6317FC, "SceneManager chat request failed (sub_6317FC)", chatRequestFailed);

// ------------------------------------------------------------------------------------------------ input and send
namespace {
constexpr uint32_t VT_Chat_PaymentError = 0xF5FFD8;   // $_44's error dialog callback (no captures)
constexpr uint32_t VT_Chat_Created = 0xF60088;        // createMessage's callback $_128 (heap 0x58): $_41's captures from +0x14
constexpr uint32_t VT_Chat_Opened = 0xF600B4;         // $_38's open animation end {send, stamp, close}
constexpr uint32_t CHAT_LAST_ID = 0x414;              // uint32: the newest message id seen
constexpr uint32_t CHAT_SENT = 0x41C;                 // uint32: messages sent
constexpr uint32_t EDITBOX_SET_TEXT = 0x2A8, EDITBOX_GET_TEXT = 0x2AC;   // vtable byte offsets (const std::string&)
constexpr uint32_t API_SHOW_ERROR = 0x14;             // APIManager: uint8
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;   // (api, const function<void()>&, bool)
constexpr uint32_t SceneManager_chatWindowUpdate = 0x615311;   // (mgr, int lines)
constexpr uint32_t SceneManager_chatLineUpdate = 0x615729;     // (sret string, mgr, const string&, int maxLength)
constexpr uint32_t SceneManager_getChatStampMessageCheck = 0x615EA5;   // (mgr, const string&): a stamp message
constexpr uint32_t SceneUtil_splitSpaceCheck = 0x47898D;       // (const string&): not only spaces
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t ChatManager_getInstance = 0x4DB0E5, ChatManager_createMessage = 0x4DC021;   // (cm, const char*, const function&)
constexpr uint32_t ChatScrollView_getTextLineCount = 0x55852D;   // (const string&, int columns, int maxLines)
constexpr uint32_t Text_setTextAreaSize = rwlib::cocos2d::ui::Text::setTextAreaSize;
constexpr uint32_t ChatPopup_reopen = 0x63204D;                  // $_38's body (on its captures)
constexpr int32_t TEXT_CHAT_SEND_FAILED = 104200029, TEXT_CHAT_ONLY_SPACES = 104200032;
constexpr int32_t TEXT_CHAT_STATUS_1 = 105000001, TEXT_CHAT_STATUS_2 = 105000002;
constexpr uint32_t CHAT_COLUMNS = 36, CHAT_MAX_LINES = 3, CHAT_LINE_HEIGHT = 26;
constexpr float CHAT_INPUT_WIDTH = 470.0f, CHAT_LAYER_WIDTH = 500.0f, CHAT_LAYER_HEIGHT = 52.0f;
constexpr float CHAT_POLL_SECONDS = 15.0f;
constexpr uint32_t CHAT_RECENT = 50;                  // the window starts this many messages before the newest
constexpr uint32_t TOUCH_BEGAN = 0, TOUCH_ENDED = 2, TOUCH_CANCELED = 3;

uint32_t editText(uint32_t m) {
    const uint32_t box = ld32(m + CHAT_EDIT_BOX);
    return vcall(box, EDITBOX_GET_TEXT, {box});
}
void setTouch(uint32_t widget, uint32_t on) { vcall(widget, WIDGET_SET_TOUCH_ENABLED, {widget, on}); }
// a stamp message that cannot go out while no request runs
bool stampBlocked(uint32_t m) {
    const uint32_t text = editText(m);
    if (call(SceneManager_getChatStampMessageCheck, {text, text}) != 1) return false;
    return strSize(editText(m)) != 0 && !ld8(m + CHAT_REQUESTING);
}
// chatRequestFailed(cap, TextData text `id`)
void chatFailedText(uint32_t cap, int32_t id) {
    Frame f;
    const uint32_t text = textData(f, id);
    const uint32_t message = newStr(f, textDataStr(text));
    chatRequestFailed(cap, message);
    strFree(message);
    sharedRelease(text);
}
}  // namespace

// $_44 (heap 0x54) {the fetch captures}(ok): the payment info for the chat arrived -> fetch; failed: the loading
// indicator hidden and, when wanted, the API error dialog
static void chatPaymentInfo(uint32_t functor, uint32_t okp) {
    if (ld8(okp)) {
        call(ChatPopup_fetch, {functor + 4});
        return;
    }
    hideLoadingIndicator();
    if (!ld8(call(fn::APIManager_getInstance, {}) + API_SHOW_ERROR)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_Chat_PaymentError, {});
    call(APIManager_openErrorDialog, {api, fo, 1});
    functionDestroy(fo);
}

// $_41 (heap 0x88) Button_Send {this, send, stamp, close, $_128's captures (+0x14, 0x54 bytes), the failure
// captures (+0x68)}(sender, type): began / cancelled mark the edit box (+0x4A5); ended sends the edit box's text
// (not a blocked stamp, not empty, not only spaces) with the buttons off until createMessage answers
static void chatSendTouch(uint32_t functor, uint32_t, uint32_t typep) {
    const uint32_t type = ld32(typep);
    const uint32_t m = ld32(functor + 4);
    if (type == TOUCH_CANCELED) {
        st8(m + CHAT_EDITING, 0);
        return;
    }
    if (type != TOUCH_ENDED) {
        if (type == TOUCH_BEGAN) st8(m + CHAT_EDITING, 1);
        return;
    }
    Frame f;
    const uint32_t message = f.alloc(12);
    strCopy(message, editText(m));
    if (stampBlocked(m) || strSize(editText(m)) == 0) {
        st8(m + CHAT_EDITING, 0);
        strFree(message);
        return;
    }
    if (strSize(editText(m)) && !call(SceneUtil_splitSpaceCheck, {editText(m)})) {
        okDialogText(TEXT_CHAT_ONLY_SPACES);
        st8(m + CHAT_EDITING, 0);
        strFree(message);
        return;
    }
    call(fn::AudioPlaySystemSEDecide, {});
    st8(m + CHAT_REQUESTING, 1);
    st32(m + CHAT_TIMER, 0);
    setTouch(ld32(functor + 8), 0);
    setTouch(ld32(functor + 0xC), 0);
    setTouch(ld32(functor + 0x10), 0);
    const uint32_t cm = call(ChatManager_getInstance, {});
    const uint32_t captures = f.alloc(0x54);
    call(Memcpy, {captures, functor + 0x14, 0x54});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x58});
    if (heap) {
        st32(heap, VT_Chat_Created);
        call(Memcpy, {heap + 4, captures, 0x54});
    }
    st32(fo + 16, heap);
    const uint32_t sent = call(ChatManager_createMessage, {cm, strData(message), fo});
    functionDestroy(fo);
    if (!sent) chatFailedText(functor + 0x68, TEXT_CHAT_SEND_FAILED);
    strFree(message);
}

// $_128 (heap 0x58) {this, label2, send, sendText, $_38's captures (+0x14), stamp (+0x30), close (+0x34), the
// failure captures (+0x38)}(ChatManager::StatusCode): 0 sent -> the input cleared and shrunk, the message window
// moved to the newest messages, the popup reopened and polled again in 15 s; 1 / 2 refused -> the buttons back
// and a notice; 3 nothing; else the request failed
static void chatCreated(uint32_t functor, uint32_t statusp) {
    const uint32_t status = ld32(statusp);
    if (status > 3) {
        chatFailedText(functor + 0x38, TEXT_CHAT_SEND_FAILED);
        return;
    }
    const uint32_t m = ld32(functor + 4);
    if (status == 0) {
        {
            Frame f;
            const uint32_t box = ld32(m + CHAT_EDIT_BOX);
            const uint32_t empty = f.alloc(12);
            st8(empty, 0);
            st8(empty + 1, 0);
            vcall(box, EDITBOX_SET_TEXT, {box, empty});
            strFree(empty);
            const uint32_t empty2 = f.alloc(12);
            st8(empty2, 0);
            st8(empty2 + 1, 0);
            call(fn::Text_setString, {ld32(functor + 8), empty2});
            strFree(empty2);
        }
        const uint32_t send = ld32(functor + 0xC);
        call(Widget_setBright, {send, 0});
        SceneUtil::setButtonEnabled(ld32(functor + 0xC), ld32(functor + 0x10),
                                    call(Widget_isBright, {ld32(functor + 0xC)}), 0);
        {
            const uint32_t layer = ld32(m + CHAT_EDIT_LAYER);
            const uint32_t setSize = ld32(ld32(layer) + node::setContentSize);
            Frame f;
            const uint32_t size = f.alloc(8);
            call(Size_ctor, {size, fbits(CHAT_LAYER_WIDTH), fbits(CHAT_LAYER_HEIGHT)});
            call(setSize, {layer, size});
        }
        call(SceneManager_chatWindowUpdate, {m, 1});
        st32(m + CHAT_SENT, ld32(m + CHAT_SENT) + 1);
        st32(m + CHAT_START, 1);
        const uint32_t total = ld32(m + CHAT_TOTAL);
        if (total > CHAT_RECENT) st32(m + CHAT_START, total - CHAT_RECENT);
        if (ld32(m + CHAT_LAST_ID) < total) st32(m + CHAT_LAST_ID, total);
        call(ChatPopup_reopen, {functor + 0x14});
        st8(m + CHAT_LOADING_PAGE, 1);
        stf(m + CHAT_TIMER, CHAT_POLL_SECONDS);
        call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        return;
    }
    if (status == 3) return;
    st8(m + CHAT_REQUESTING, 0);
    hideLoadingIndicator();
    setTouch(ld32(functor + 0xC), 1);
    setTouch(ld32(functor + 0x30), 1);
    setTouch(ld32(functor + 0x34), 1);
    st8(m + CHAT_EDITING, 0);
    okDialogText(status == 1 ? TEXT_CHAT_STATUS_1 : TEXT_CHAT_STATUS_2);
}

// $_38's body {this, root, shadow, opacity, send, stamp, close}: the request done; a shown popup gets its buttons
// back, a hidden one is shown again (shadow fade, open animation that enables the buttons at its end)
static void chatReopen(uint32_t cap) {
    const uint32_t m = ld32(cap);
    st8(m + CHAT_REQUESTING, 0);
    st32(m + CHAT_TIMER, 0);
    const uint32_t root = ld32(cap + 4);
    if (vcall(root, node::isVisible, {root})) {
        setTouch(ld32(cap + 0x10), 1);
        setTouch(ld32(cap + 0x14), 1);
        setTouch(ld32(cap + 0x18), 1);
        st8(m + CHAT_EDITING, 0);
        return;
    }
    call(fn::AudioPlaySystemSEPopUp, {});
    call(fn::Node_runAction, {ld32(cap + 8), call(fn::FadeTo_create, {fbits(0.1f), ld8(cap + 0xC)})});
    st32(m + CHAT_TIMER, 0);
    vcall(ld32(cap + 4), node::setVisible, {ld32(cap + 4), 1});
    const uint32_t popup = ld32(cap + 4);
    Frame f;
    const uint32_t opened = newFunction(f, VT_Chat_Opened, {ld32(cap + 0x10), ld32(cap + 0x14), ld32(cap + 0x18)});
    call(fn::Node_runAction, {popup, call(SceneUtil_createOpenPopupAnimation, {popup, opened})});
    functionDestroy(opened);
}

// $_40 (heap 0x18) {this, maxLength, label2, send, sendText}: the edit box's text changed -> cut to maxLength,
// mirrored in label2, Send lit when there is text (and it is no blocked stamp), the input areas sized to the
// message's lines
static void chatTextChanged(uint32_t functor) {
    const uint32_t m = ld32(functor + 4);
    {
        const uint32_t box = ld32(m + CHAT_EDIT_BOX);
        const uint32_t setText = ld32(ld32(box) + EDITBOX_SET_TEXT);
        const uint32_t text = vcall(box, EDITBOX_GET_TEXT, {box});
        Frame f;
        const uint32_t cut = f.alloc(12);
        call(SceneManager_chatLineUpdate, {cut, text, text, ld32(functor + 8)});
        call(setText, {box, cut});
        strFree(cut);
    }
    {
        const uint32_t label2 = ld32(functor + 0xC);
        Frame f;
        const uint32_t s = newStr(f, strData(editText(m)));
        call(fn::Text_setString, {label2, s});
        strFree(s);
    }
    const uint32_t lines = call(ChatScrollView_getTextLineCount, {editText(m), CHAT_COLUMNS, CHAT_MAX_LINES});
    const uint32_t send = ld32(functor + 0x10);
    call(Widget_setBright, {send, strSize(editText(m)) ? 1u : 0u});
    if (stampBlocked(m)) call(Widget_setBright, {ld32(functor + 0x10), 0});
    SceneUtil::setButtonEnabled(ld32(functor + 0x10), ld32(functor + 0x14), call(Widget_isBright, {ld32(functor + 0x10)}), 0);
    const float height = float(int32_t(lines * CHAT_LINE_HEIGHT));
    Frame f;
    {
        const uint32_t box = ld32(m + CHAT_EDIT_BOX);
        const uint32_t setSize = ld32(ld32(box) + node::setContentSize);
        const uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fbits(CHAT_INPUT_WIDTH), fbits(height)});
        call(setSize, {box, size});
    }
    {
        const uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fbits(CHAT_INPUT_WIDTH), fbits(height)});
        call(Text_setTextAreaSize, {ld32(functor + 0xC), size});
    }
    {
        const uint32_t label2 = ld32(functor + 0xC);
        const uint32_t setSize = ld32(ld32(label2) + node::setContentSize);
        const uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fbits(CHAT_INPUT_WIDTH), fbits(height)});
        call(setSize, {label2, size});
    }
    {
        const uint32_t layer = ld32(m + CHAT_EDIT_LAYER);
        const uint32_t setSize = ld32(ld32(layer) + node::setContentSize);
        const uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fbits(CHAT_LAYER_WIDTH), fbits(float(int32_t(lines * CHAT_LINE_HEIGHT + CHAT_LINE_HEIGHT)))});
        call(setSize, {layer, size});
    }
    call(SceneManager_chatWindowUpdate, {m, lines});
}

// $_39 Close_Button {this}(sender, type): ended -> the current popup closes; began / cancelled mark the edit box
static void chatCloseTouch(uint32_t functor, uint32_t, uint32_t typep) {
    const uint32_t type = ld32(typep);
    const uint32_t m = ld32(functor + 4);
    if (type == TOUCH_CANCELED) {
        st8(m + CHAT_EDITING, 0);
    } else if (type == TOUCH_ENDED) {
        call(AudioPlaySystemSECancel, {});
        call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
    } else if (type == TOUCH_BEGAN) {
        st8(m + CHAT_EDITING, 1);
    }
}

RW_FUNCTION(0x631150, "SceneManager::openChatPopup $_44::operator() (payment info)", chatPaymentInfo);
RW_FUNCTION(0x631418, "SceneManager::openChatPopup $_41::operator() (Button_Send)", chatSendTouch);
RW_FUNCTION(0x631BDC, "SceneManager::openChatPopup $_128::operator() (message created)", chatCreated);
RW_FUNCTION(0x63204C, "SceneManager::openChatPopup $_38 body (sub_63204C): reopen", chatReopen);
RW_FUNCTION(0x6322D4, "SceneManager::openChatPopup $_40::operator() (text changed)", chatTextChanged);
RW_FUNCTION(0x632610, "SceneManager::openChatPopup $_39::operator() (Close_Button)", chatCloseTouch);
