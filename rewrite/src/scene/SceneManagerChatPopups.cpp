// SceneManager: the chat popups opened from a chat message or the block list - delete / check (report, block)
// a post, the report reasons, the block list and unblocking a user.
//
// Every popup has the same opening: the popup sound, a shadow fading in on popup layer 2, the cocostudio layout
// added on the shadow and centred. The touch callbacks are lambdas; those capturing a ChatMessage (two strings)
// are too big for the std::function buffer and live in a heap __func: the message is copied into a stack
// temporary first, then moved into the heap object (its strings taken, the temporary's left empty).
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
// ChatScrollView::ChatMessage (0x40 bytes with its padding, 8-aligned)
constexpr uint32_t MSG_HEAD = 0x00;       // 16 bytes (ids)
constexpr uint32_t MSG_FLAG = 0x10;       // uint8
constexpr uint32_t MSG_NAME = 0x14;       // std::string
constexpr uint32_t MSG_INFO = 0x20;       // 12 bytes, copied raw
constexpr uint32_t MSG_TEXT = 0x2C;       // std::string
constexpr uint32_t MSG_FLAG2 = 0x38;      // uint8
constexpr uint32_t MSG_SIZE = 0x40;

constexpr uint32_t APIManager_chatShared = 0x84;          // a shared_ptr held while the popup is built (+0x88 control)

// guest functions
constexpr uint32_t Vec2_ctor_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;
constexpr uint32_t ChatScrollDelView_create = 0x558BE1;
constexpr uint32_t ChatScrollDelView_setMessages = 0x559FE5;   // (view, std::vector<ChatMessage> by value)
constexpr uint32_t MessageVector_pushBackSlow = 0x630CBD;      // vector<ChatMessage>::__push_back_slow_path(const&)
constexpr uint32_t MessageVector_copy = 0x617429;              // vector<ChatMessage>(const vector&)
constexpr uint32_t MessageVector_destroy = 0x5568AD;           // __vector_base<ChatMessage>::~__vector_base

// strings
constexpr uint32_t STR_ChatPostDeleteJson = 0xE033B9;     // "cocostudio/publish/Chat_Post_Delete.json"
constexpr uint32_t STR_ChatPostCheckJson = 0xE03415;      // "cocostudio/publish/Chat_Post_Check.json"
constexpr uint32_t STR_Txt_Post = 0xE033E2;
constexpr uint32_t STR_Txt_Wording = 0xDF862F;
constexpr uint32_t STR_Txt_Delete = 0xE033EB;
constexpr uint32_t STR_Txt_Cancel = 0xDF86A6;
constexpr uint32_t STR_Txt_BlockList = 0xE033F6;
constexpr uint32_t STR_Txt_Report = 0xE0343D;
constexpr uint32_t STR_Txt_Block = 0xE03448;
constexpr uint32_t STR_Button_Delete = 0xDFDB01;
constexpr uint32_t STR_Button_Close = 0xDF8692;
constexpr uint32_t STR_Button_BlockList = 0xE03404;
constexpr uint32_t STR_Button_Report = 0xE03452;
constexpr uint32_t STR_Button_Block = 0xE03460;
constexpr uint32_t STR_Close_Button = 0xDF8D2F;
constexpr uint32_t STR_Area = 0xDF9378;

// lambdas (vtables of the __func, +8)
constexpr uint32_t VT_DeletePost_Close = 0xF5FD18;        // $_59 [close]
constexpr uint32_t VT_DeletePost_Delete = 0xF5FCC0;       // $_61 [message, close, this, delete, close, $_60] (heap 0xB0)
constexpr uint32_t VT_DeletePost_BlockList = 0xF5FC94;    // $_62 [this]
constexpr uint32_t VT_DeletePost_CanClose = 0xF5FC68;     // $_63 [close] bool()
constexpr uint32_t VT_DeletePost_OnClose = 0xF5FC10;      // $_64 [delete, close, this, popup, layer, shadow] (heap 0x1C)
constexpr uint32_t VT_CheckPost_Close = 0xF5FBE4;         // $_65 []
constexpr uint32_t VT_CheckPost_Report = 0xF5FBB8;        // $_66 [this, message, flag] (heap 0x58)
constexpr uint32_t VT_CheckPost_Block = 0xF5FB60;         // $_67 [this, message, flag] (heap 0x58)
constexpr uint32_t VT_CheckPost_BlockList = 0xF5FB34;     // $_68 [this]
constexpr uint32_t VT_CheckPost_CanClose = 0xF5FB08;      // $_69 [close] bool()
constexpr uint32_t VT_CheckPost_OnClose = 0xF5FAB0;       // $_70 [popup, layer, shadow] void(bool)

uint32_t addrOf(SceneManager *self) { return rw::guest_addr(self); }
uint32_t addrOfMgr() { return rw::guest_addr(SceneManager::getInstance()); }

void copyWords(uint32_t dst, uint32_t src, uint32_t n) {
    for (uint32_t k = 0; k < n; k += 4) st32(dst + k, ld32(src + k));
}

// std::string copy constructor as libc++ inlines it
void strCopy(uint32_t dst, uint32_t src) {
    if (strIsLong(src)) strInit(dst, ld32(src + 8), ld32(src + 4));
    else copyWords(dst, src, 12);
}
// std::string move: take the three words, leave the source empty
void strMove(uint32_t dst, uint32_t src) {
    copyWords(dst, src, 12);
    st32(src, 0);
    st32(src + 4, 0);
    st32(src + 8, 0);
}

// ChatMessage copy / move constructors and destructor
void msgCopy(uint32_t dst, uint32_t src) {
    copyWords(dst + MSG_HEAD, src + MSG_HEAD, 16);
    st8(dst + MSG_FLAG, ld8(src + MSG_FLAG));
    strCopy(dst + MSG_NAME, src + MSG_NAME);
    copyWords(dst + MSG_INFO, src + MSG_INFO, 12);
    strCopy(dst + MSG_TEXT, src + MSG_TEXT);
    st8(dst + MSG_FLAG2, ld8(src + MSG_FLAG2));
}
void msgMove(uint32_t dst, uint32_t src) {
    copyWords(dst + MSG_HEAD, src + MSG_HEAD, 16);
    st8(dst + MSG_FLAG, ld8(src + MSG_FLAG));
    strMove(dst + MSG_NAME, src + MSG_NAME);
    copyWords(dst + MSG_INFO, src + MSG_INFO, 12);
    strMove(dst + MSG_TEXT, src + MSG_TEXT);
    st8(dst + MSG_FLAG2, ld8(src + MSG_FLAG2));
}
void msgDestroy(uint32_t m) {
    strFree(m + MSG_TEXT);
    strFree(m + MSG_NAME);
}

// The opening shared by the chat popups: sound, shadow fading in on a popup layer of the SceneManager (z `z`),
// the layout `json` on the shadow (z `z` + 1), anchored in the middle and centred on the visible area.
struct ChatPopup {
    uint32_t layer, shadow, popup;
};
constexpr uint32_t LAYER_POPUP2 = 0x3EC, LAYER_DERIVED = 0x3F4;   // SceneManager popupLayer2 / popupLayerDerived
// the sound and the shadow
ChatPopup beginChatPopup(uint32_t layerField, uint32_t z) {
    ChatPopup p;
    call(fn::AudioPlaySystemSEPopUp, {});
    p.layer = ld32(addrOfMgr() + layerField);
    p.shadow = SceneUtil::createShadowSprite();
    uint32_t opacity = vcall(p.shadow, node::getOpacity, {p.shadow});
    vcall(p.shadow, node::setOpacity, {p.shadow, 0});
    call(fn::Node_runAction, {p.shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(p.layer, node::addChildZ, {p.layer, p.shadow, z});
    p.popup = 0;
    return p;
}
// the layout on the shadow
void loadChatLayout(ChatPopup &p, uint32_t json, uint32_t z) {
    p.popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    uint32_t popup = p.popup;
    vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_MIDDLE});
    float px = bitsf(vcall(popup, node::getPositionX, {popup}));
    float w = ldf(vcall(popup, node::getContentSize, {popup}));
    float marginX = SceneUtil::getSideMargin();
    float py = bitsf(vcall(popup, node::getPositionY, {popup}));
    float h = ldf(vcall(popup, node::getContentSize, {popup}) + 4);
    float x = marginX + (px + w * 0.5f);
    float yHalf = py + h * 0.5f;
    float y = SceneUtil::getSideHeightMargin() + yHalf;
    {
        Frame f;
        vcall(popup, node::setPosition, {popup, newVec2(f, x, y)});
    }
    vcall(p.shadow, node::addChildZ, {p.shadow, popup, z + 1});
}
ChatPopup buildChatPopup(uint32_t json, uint32_t layerField = LAYER_POPUP2, uint32_t z = 2) {
    ChatPopup p = beginChatPopup(layerField, z);
    loadChatLayout(p, json, z);
    return p;
}

// The text `name` of the popup: shadow and TextData text `textId`
void label(uint32_t popup, uint32_t name, uint32_t len, int32_t textId) {
    uint32_t text = findNode(popup, name, len);
    SceneUtil::setShadow(text);
    setTextId(text, textId);
}

// widget->addTouchEventListener(fo) with a std::function already built, destroyed after
void addTouchFunction(uint32_t widget, uint32_t fo) {
    call(fn::Widget_addTouchEventListener, {widget, fo});
    functionDestroy(fo);
}

// A ChatScrollDelView showing the one message, filling the layout's "Area" node. `keepPosition`: re-set the
// view's own position (the delete popup does). `messages` (a stack vector<ChatMessage>) is left for the caller to
// destroy.
uint32_t addMessageView(uint32_t popup, uint32_t message, uint32_t messages, bool keepPosition, Frame &f) {
    uint32_t area = findNode(popup, STR_Area, 4);
    uint32_t view = call(ChatScrollDelView_create, {});
    uint32_t size = vcall(area, node::getContentSize, {area});
    vcall(view, node::setContentSize, {view, size});
    if (keepPosition) {
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor_copy, {pos, vcall(view, node::getPosition, {view})});
        vcall(view, node::setPosition, {view, pos});
    }
    vcall(area, node::addChild, {area, view});
    st32(messages, 0);
    st32(messages + 4, 0);
    st32(messages + 8, 0);
    call(MessageVector_pushBackSlow, {messages, message});
    uint32_t byValue = f.alloc(12);
    call(MessageVector_copy, {byValue, messages});
    call(ChatScrollDelView_setMessages, {view, byValue});
    call(MessageVector_destroy, {byValue});
    return view;
}

void runOpenAnimation(uint32_t popup) {
    Frame f;
    uint32_t none = newEmptyFunction(f);
    call(fn::Node_runAction, {popup, SceneUtil::createOpenPopupAnimation(popup, none)});
    functionDestroy(none);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ delete post

// "Delete this post?" for one of the user's own messages: delete, cancel, or open the block list.
void SceneManager::openDeletePostPopup(SceneManager *self, Addr message) {
    const uint32_t me = addrOf(self);
    ChatPopup p = buildChatPopup(STR_ChatPostDeleteJson);
    const uint32_t popup = p.popup;
    Frame f;
    label(popup, STR_Txt_Post, 8, 104200026);
    label(popup, STR_Txt_Wording, 11, 104200009);
    label(popup, STR_Txt_Delete, 10, 104200003);
    label(popup, STR_Txt_Cancel, 10, 100100002);
    label(popup, STR_Txt_BlockList, 13, 104200005);
    uint32_t del = findNode(popup, STR_Button_Delete, 13);
    uint32_t close = findNode(popup, STR_Button_Close, 12);
    uint32_t blockList = findNode(popup, STR_Button_BlockList, 16);

    addTouch(close, VT_DeletePost_Close, {close});

    // $_60 [this, message, close, delete, close]: the deletion itself, captured by the delete button's lambda
    uint32_t inner = f.alloc(0x58);
    st32(inner, me);
    msgCopy(inner + 8, message);
    st32(inner + 0x48, close);
    st32(inner + 0x4C, del);
    st32(inner + 0x50, close);
    // $_61 [message, close, this, delete, close, $_60], built on the stack then moved into a heap __func
    uint32_t outer = f.alloc(0xA8);
    msgCopy(outer, message);
    st32(outer + 0x40, close);
    st32(outer + 0x44, me);
    st32(outer + 0x48, del);
    st32(outer + 0x4C, close);
    st32(outer + 0x50, ld32(inner));
    msgCopy(outer + 0x58, inner + 8);
    copyWords(outer + 0x98, inner + 0x48, 12);
    uint32_t onDelete = f.alloc(20);
    st32(onDelete + 16, 0);
    uint32_t heap = call(fn::operator_new, {0xB0});
    if (heap) {
        st32(heap, VT_DeletePost_Delete);
        msgMove(heap + 8, outer);
        copyWords(heap + 0x48, outer + 0x40, 0x14);
        msgMove(heap + 0x60, outer + 0x58);
        copyWords(heap + 0xA0, outer + 0x98, 12);
    }
    st32(onDelete + 16, heap);
    addTouchFunction(del, onDelete);
    msgDestroy(outer + 0x58);
    msgDestroy(outer);

    addTouch(blockList, VT_DeletePost_BlockList, {me});

    uint32_t chatCtl = ld32(call(fn::APIManager_getInstance, {}) + APIManager_chatShared + 4);
    if (chatCtl) call(fn::shared_add, {chatCtl});
    uint32_t messages = f.alloc(12);
    addMessageView(popup, message, messages, true, f);
    runOpenAnimation(popup);

    uint32_t mgr = addrOf(getInstance());
    uint32_t canClose = newFunction(f, VT_DeletePost_CanClose, {close});
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t closeHeap = call(fn::operator_new, {0x1C});
    if (closeHeap) {
        st32(closeHeap, VT_DeletePost_OnClose);
        st32(closeHeap + 0x04, del);
        st32(closeHeap + 0x08, close);
        st32(closeHeap + 0x0C, me);
        st32(closeHeap + 0x10, popup);
        st32(closeHeap + 0x14, p.layer);
        st32(closeHeap + 0x18, p.shadow);
    }
    st32(onClose + 16, closeHeap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(canClose);
    call(MessageVector_destroy, {messages});
    if (chatCtl) call(fn::shared_release, {chatCtl});
    msgDestroy(inner + 8);
}
RW_FUNCTION(0x619144, "SceneManager::openDeletePostPopup", SceneManager::openDeletePostPopup);

// ------------------------------------------------------------------------------------------------ check post

namespace {
// The report / block button lambdas of openCheckPostPopup: [this, message, flag] in a heap __func (0x58). The flag
// byte is never initialised by the original: it copies whatever its stack frame held at `staleFlag`.
void addMessageTouch(uint32_t widget, uint32_t vtable, uint32_t me, uint32_t message, uint32_t staleFlag, Frame &f) {
    uint32_t tmp = f.alloc(0x50);
    st32(tmp, me);
    msgCopy(tmp + 8, message);
    uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x58});
    if (heap) {
        st32(heap, vtable);
        st32(heap + 8, ld32(tmp));
        msgMove(heap + 0x10, tmp + 8);
        st8(heap + 0x50, ld8(staleFlag));
    }
    st32(fo + 16, heap);
    addTouchFunction(widget, fo);
    msgDestroy(tmp + 8);
}
}  // namespace

// Someone else's message: report the post, block the user, or open the block list.
void SceneManager::openCheckPostPopup(SceneManager *self, Addr message) {
    // the original frame (push 0x14 + 0x29C) at its address: two lambda flags are read from it uninitialised
    Frame frame;
    const uint32_t base = frame.allocExact(0x2B0);
    const uint32_t me = addrOf(self);
    ChatPopup p = buildChatPopup(STR_ChatPostCheckJson);
    const uint32_t popup = p.popup;
    Frame f;
    label(popup, STR_Txt_Post, 8, 104200026);
    label(popup, STR_Txt_Report, 10, 104200008);
    label(popup, STR_Txt_Block, 9, 104200004);
    label(popup, STR_Txt_BlockList, 13, 104200005);
    uint32_t report = findNode(popup, STR_Button_Report, 13);
    uint32_t close = findNode(popup, STR_Close_Button, 12);
    uint32_t blockList = findNode(popup, STR_Button_BlockList, 16);
    uint32_t block = findNode(popup, STR_Button_Block, 12);

    addTouch(close, VT_CheckPost_Close, {});
    addMessageTouch(report, VT_CheckPost_Report, me, message, base + 0x1E0, f);
    addMessageTouch(block, VT_CheckPost_Block, me, message, base + 0x190, f);
    addTouch(blockList, VT_CheckPost_BlockList, {me});

    uint32_t messages = f.alloc(12);
    addMessageView(popup, message, messages, false, f);
    runOpenAnimation(popup);

    uint32_t mgr = addrOf(getInstance());
    uint32_t canClose = newFunction(f, VT_CheckPost_CanClose, {close});
    uint32_t onClose = newFunction(f, VT_CheckPost_OnClose, {popup, p.layer, p.shadow});
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(canClose);
    call(MessageVector_destroy, {messages});
}
RW_FUNCTION(0x619F04, "SceneManager::openCheckPostPopup", SceneManager::openCheckPostPopup);

// ------------------------------------------------------------------------------------------------ block post

namespace {
constexpr uint32_t STR_ChatPostBlockJson = 0xE0346D;      // "cocostudio/publish/Chat_Post_Block.json"
constexpr uint32_t STR_Txt_Wording1 = 0xDFF14C;
constexpr uint32_t STR_Txt_Wording2 = 0xDF863B;
constexpr uint32_t STR_Txt_Name_Label = 0xDFDBB8;
constexpr uint32_t STR_Avatar = 0xDF8F56;
constexpr uint32_t STR_AvatarUser = 0xDFED1D;
constexpr uint32_t STR_imageButton = 0xDF8F6D;
constexpr uint32_t STR_Empty = 0xDF8A1A;

// APIManager: shared_ptr<UserAvatar> (a SafeBuffer of 296-byte avatar records, indexed by user id)
constexpr uint32_t APIManager_userAvatars = 0x1FC;
constexpr uint32_t UserAvatar_index = 0x24;               // unordered_map<u64 user id, int slot>
constexpr uint32_t AVATAR_SIZE = 0x128;
constexpr uint32_t AVATAR_NAME = 0x0C;                    // char[] user name
constexpr uint32_t AVATAR_COORDINATE = 0xE8;              // hole::network::api::UserAvatar::Coordinate

constexpr uint32_t AvatarIndex_find = 0x490275;           // unordered_map<unsigned long long, int>::find
constexpr uint32_t SafeBuffer_read = 0x45A269;            // (buffer, dst, src, n, index)
constexpr uint32_t SafeBuffer_destroy = 0x45A13D;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;       // (Avatar::Style out, const Coordinate&, int)
constexpr uint32_t ExpressionVector_allocate = 0x487635;  // vector<Avatar::Expression>::allocate(n)
constexpr uint32_t ExpressionVector_destroy = 0x46BE0D;
constexpr uint32_t AvatarIcon_create = 0x5C9DB5;          // (style, texture, const Vec2&, float, float, const vector<Expression>&)
constexpr uint32_t ImageButton_create = 0x531239;         // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;  // (button, node, float)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Node_getLocalZOrder = 0x18;            // vtable byte offsets
constexpr uint32_t Widget_getVirtualRenderer = 0x2D8;
constexpr uint32_t Sprite_getTexture = 0x280;

constexpr uint32_t VT_BlockPost_Avatar = 0xF5FA84;        // $_71 [] (the avatar image button)
constexpr uint32_t VT_BlockPost_Close = 0xF5FA58;         // $_72 []
constexpr uint32_t VT_BlockPost_Block = 0xF5F9D4;         // $_76 (heap 0x130), see openBlockPostPopup
constexpr uint32_t VT_BlockPost_CanClose = 0xF5F9A8;      // $_77 [close] bool()
constexpr uint32_t VT_BlockPost_OnClose = 0xF5F950;       // $_78 [block, close, this, popup, layer, shadow] (heap 0x1C)

// Where the avatar code keeps its temporaries in the caller's original frame
struct AvatarTemps {
    uint32_t key, record, style, position, expressions, expression;
};
constexpr AvatarTemps BLOCK_POST_TEMPS{0x230, 0x518, 0x170, 0x168, 0x15C, 0x23C};
constexpr AvatarTemps UNBLOCK_POST_TEMPS{0x1C0, 0x418, 0x114, 0x10C, 0x100, 0x1CC};

// The user's avatar icon (mirrored) in place of the layout's "Avatar" node and the user's name, when the user
// `userId` is in the avatar list; otherwise the node is hidden and the name label says so. `frame` is the original
// frame (the avatar record and the icon's temporaries at their original offsets). `imageButton`: also make the
// icon touchable (an ImageButton over it).
void showPostUser(uint32_t frame, const AvatarTemps &t, uint32_t popup, uint32_t nameLabel, uint32_t avatarNode,
                  uint32_t avatars, uint32_t userLo, uint32_t userHi, bool imageButton) {
    const uint32_t key = frame + t.key, record = frame + t.record;
    const uint32_t style = frame + t.style, position = frame + t.position, expressions = frame + t.expressions;
    const uint32_t expression = frame + t.expression;
    st32(key, userLo);
    st32(key + 4, userHi);
    uint32_t node = call(AvatarIndex_find, {avatars + UserAvatar_index, key});
    int32_t slot = node ? int32_t(ld32(node + 0x10)) : -1;
    if (slot < 0) {
        uint32_t parent = vcall(avatarNode, node::getParent, {avatarNode});
        uint32_t old = findNode(parent, STR_AvatarUser, 10);
        if (old) vcall(old, node::removeFromParentAndCleanup, {old, 1});
        vcall(avatarNode, node::setVisible, {avatarNode, 0});
        setTextId(nameLabel, 104200024);
        return;
    }
    st8(record + 0x11F, uint8_t(ld8(record + 0x11F) & 0xFD));
    uint32_t at = AVATAR_SIZE * uint32_t(slot);
    call(SafeBuffer_read, {avatars, record, ld32(avatars + 12) + at, AVATAR_SIZE, at});
    uint32_t renderer = vcall(avatarNode, Widget_getVirtualRenderer, {avatarNode});
    uint32_t texture = vcall(renderer, Sprite_getTexture, {renderer});
    call(AvatarUtil_makeStyle, {style, record + AVATAR_COORDINATE, 0});
    stf(position, -80.0f);
    stf(position + 4, 130.0f);
    // vector<Avatar::Expression>{1}
    st32(expression, 1);
    st32(expressions, 0);
    st32(expressions + 4, 0);
    st32(expressions + 8, 0);
    call(ExpressionVector_allocate, {expressions, 1});
    uint32_t end = ld32(expressions + 4);
    if (end) {
        st32(end, ld32(expression));
        end = ld32(expressions + 4);
    }
    st32(expressions + 4, end + 4);
    uint32_t icon = call(AvatarIcon_create, {style, texture, position, fbits(1.6f), fbits(1.6f), expressions});
    call(ExpressionVector_destroy, {expressions});
    call(SafeBuffer_destroy, {style});
    vcall(icon, node::setPosition, {icon, vcall(avatarNode, node::getPosition, {avatarNode})});
    vcall(icon, node::setScaleX, {icon, fbits(-1.0f)});
    uint32_t parent = vcall(avatarNode, node::getParent, {avatarNode});
    uint32_t old = findNode(parent, STR_AvatarUser, 10);
    if (old) vcall(old, node::removeFromParentAndCleanup, {old, 1});
    {
        uint32_t z = vcall(avatarNode, Node_getLocalZOrder, {avatarNode});
        Frame f;
        uint32_t name = newStr(f, STR_AvatarUser, 10);
        vcall(parent, node::addChildZName, {parent, icon, z, name});
        strFree(name);
    }
    vcall(avatarNode, node::setVisible, {avatarNode, 0});
    if (!imageButton) {
        setTextC(nameLabel, record + AVATAR_NAME);
        return;
    }
    uint32_t button = findNode(popup, STR_imageButton, 11);
    if (!button) {
        Frame f;
        uint32_t empty = newStr(f, STR_Empty, 0);
        button = call(ImageButton_create, {empty, 0});
        strFree(empty);
        call(ImageButton_setTargetView, {button, parent, fbits(1.0f)});
        uint32_t name = newStr(f, STR_imageButton, 11);
        vcall(parent, node::addChildZName, {parent, button, 0, name});
        strFree(name);
    }
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_BlockPost_Avatar, {});
        call(ImageButton_addTouchEventListener, {button, cb});
        functionDestroy(cb);
    }
    setTextC(nameLabel, record + AVATAR_NAME);
}
}  // namespace

// "Block this user?" for the author of a message: the author's avatar and name, block or cancel. `callback` runs
// after blocking.
//
// The block button's lambda nests copies of the callback four levels deep (the API call, its result, the dialog):
//   L1 [block, close, callback]                                   (0x20)
//   L2 [this, block, close, callback, L1]                         (0x48)
//   L3 [this, message, L2, L1, block, close]                      (0xB8)
//   L4 [this, block, close, message, L3, L1]                      (0x128, the heap __func at +8)
// Each level is copied from the previous ones on the stack (string and std::function copies, in member order), L4 is
// moved into the heap. The original never initialises L4's copy of L3's last two words (block, close): the heap
// gets the stale bytes of its frame there.
void SceneManager::openBlockPostPopup(SceneManager *self, Addr message, Addr callback) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x660);     // push 0x14 + 0x64C: the lambdas at their original offsets
    const uint32_t L1 = base + 0x4C8, L2 = base + 0x480, L3 = base + 0x3C8, L4 = base + 0x288;
    const uint32_t me = addrOf(self);
    ChatPopup p = buildChatPopup(STR_ChatPostBlockJson, LAYER_DERIVED, 4);
    const uint32_t popup = p.popup;
    label(popup, STR_Txt_Wording1, 12, 104200012);
    label(popup, STR_Txt_Wording2, 12, 104200013);
    label(popup, STR_Txt_Block, 9, 104200004);
    label(popup, STR_Txt_Cancel, 10, 100100002);

    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t avatarsCtl = ld32(api + APIManager_userAvatars + 4);
    uint32_t avatars = ld32(api + APIManager_userAvatars);
    if (avatarsCtl) call(fn::shared_add, {avatarsCtl});
    uint32_t nameLabel = findNode(popup, STR_Txt_Name_Label, 14);
    SceneUtil::setShadow(nameLabel);
    uint32_t avatarNode = findNode(popup, STR_Avatar, 6);
    showPostUser(base, BLOCK_POST_TEMPS, popup, nameLabel, avatarNode, avatars, ld32(message + MSG_HEAD),
                 ld32(message + MSG_HEAD + 4), true);

    uint32_t close = findNode(popup, STR_Button_Close, 12);
    uint32_t block = findNode(popup, STR_Button_Block, 12);
    addTouch(close, VT_BlockPost_Close, {});

    // L1, L2
    st32(L1, block);
    st32(L1 + 4, close);
    call(fn::function_vv_copy, {L1 + 8, callback});
    st32(L2, me);
    st32(L2 + 4, block);
    st32(L2 + 8, close);
    call(fn::function_vv_copy, {L2 + 0x10, callback});
    copyWords(L2 + 0x28, L1, 8);
    functionCopy(L2 + 0x30, L1 + 8);
    // L3
    st32(L3, me);
    msgCopy(L3 + 8, message);
    copyWords(L3 + 0x48, L2, 12);
    functionCopy(L3 + 0x58, L2 + 0x10);
    copyWords(L3 + 0x70, L2 + 0x28, 8);
    functionCopy(L3 + 0x78, L2 + 0x30);
    copyWords(L3 + 0x90, L1, 8);
    functionCopy(L3 + 0x98, L1 + 8);
    st32(L3 + 0xB0, block);
    st32(L3 + 0xB4, close);
    // L4 (L4 + 0x100, L3's block / close, is left as it is)
    st32(L4, me);
    st32(L4 + 4, block);
    st32(L4 + 8, close);
    msgCopy(L4 + 0x10, message);
    st32(L4 + 0x50, ld32(L3));
    msgCopy(L4 + 0x58, L3 + 8);
    copyWords(L4 + 0x98, L3 + 0x48, 12);
    functionCopy(L4 + 0xA8, L3 + 0x58);
    copyWords(L4 + 0xC0, L3 + 0x70, 8);
    functionCopy(L4 + 0xC8, L3 + 0x78);
    copyWords(L4 + 0xE0, L3 + 0x90, 8);
    functionCopy(L4 + 0xE8, L3 + 0x98);
    copyWords(L4 + 0x108, L1, 8);
    functionCopy(L4 + 0x110, L1 + 8);
    // the block button: L4 moved into a heap __func
    Frame f;
    uint32_t onBlock = f.alloc(20);
    st32(onBlock + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x130});
    if (heap) {
        const uint32_t h = heap + 8;
        st32(heap, VT_BlockPost_Block);
        copyWords(h, L4, 12);
        msgMove(h + 0x10, L4 + 0x10);
        st32(h + 0x50, ld32(L4 + 0x50));
        msgMove(h + 0x58, L4 + 0x58);
        copyWords(h + 0x98, L4 + 0x98, 12);
        call(fn::function_vv_move, {h + 0xA8, L4 + 0xA8});
        copyWords(h + 0xC0, L4 + 0xC0, 8);
        call(fn::function_vv_move, {h + 0xC8, L4 + 0xC8});
        st32(h + 0x50, ld32(L4 + 0x50));
        copyWords(h + 0xE0, L4 + 0xE0, 8);
        call(fn::function_vv_move, {h + 0xE8, L4 + 0xE8});
        copyWords(h + 0x100, L4 + 0x100, 8);
        copyWords(h + 0x108, L4 + 0x108, 8);
        call(fn::function_vv_move, {h + 0x110, L4 + 0x110});
    }
    st32(onBlock + 16, heap);
    addTouchFunction(block, onBlock);
    functionDestroy(L4 + 0x110);
    functionDestroy(L4 + 0xE8);
    functionDestroy(L4 + 0xC8);
    functionDestroy(L4 + 0xA8);
    msgDestroy(L4 + 0x58);
    msgDestroy(L4 + 0x10);

    runOpenAnimation(popup);
    uint32_t mgr = addrOf(getInstance());
    uint32_t canClose = newFunction(f, VT_BlockPost_CanClose, {close});
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t closeHeap = call(fn::operator_new, {0x1C});
    if (closeHeap) {
        st32(closeHeap, VT_BlockPost_OnClose);
        st32(closeHeap + 0x04, block);
        st32(closeHeap + 0x08, close);
        st32(closeHeap + 0x0C, me);
        st32(closeHeap + 0x10, popup);
        st32(closeHeap + 0x14, p.layer);
        st32(closeHeap + 0x18, p.shadow);
    }
    st32(onClose + 16, closeHeap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(canClose);
    // L3, L2, L1
    functionDestroy(L3 + 0x98);
    functionDestroy(L3 + 0x78);
    functionDestroy(L3 + 0x58);
    msgDestroy(L3 + 8);
    functionDestroy(L2 + 0x30);
    functionDestroy(L2 + 0x10);
    functionDestroy(L1 + 8);
    if (avatarsCtl) call(fn::shared_release, {avatarsCtl});
}
RW_FUNCTION(0x61AACC, "SceneManager::openBlockPostPopup", SceneManager::openBlockPostPopup);

// ------------------------------------------------------------------------------------------------ report

namespace {
constexpr uint32_t STR_ChatPostReportJson = 0xE03495;     // "cocostudio/publish/Chat_Post_Report.json"
constexpr uint32_t STR_Txt_Section = 0xE034BE;
constexpr uint32_t STR_Txt_Send = 0xE0064F;
constexpr uint32_t STR_Txt_Section_Label = 0xE034CA;
constexpr uint32_t STR_Txt_Content = 0xE034DC;
constexpr uint32_t STR_Txt_Content_Label = 0xE034E8;
constexpr uint32_t STR_Txt_Limit = 0xDF9EED;
constexpr uint32_t STR_Button_Send = 0xE00637;
constexpr uint32_t STR_Button_List = 0xE00643;
constexpr uint32_t STR_CalligraphyFont = 0xDF9F1F;        // "cocostudio/publish/Cinema_Calligraphy_Web.ttf"

// SceneManager fields outside the declared layout
constexpr uint32_t SM_reportReason = 0x424;               // int: the selected report reason (index into REPORT_REASON_TEXTS)
constexpr uint32_t SM_reportEditing = 0x4A5;              // uint8: the report text box is being edited
constexpr uint32_t REPORT_REASON_TEXTS = 0xE52960;        // int[7]: TextData ids of the report reasons

constexpr uint32_t TextField_getMaxLength = rwlib::cocos2d::ui::TextField::getMaxLength;
constexpr uint32_t TextField_getPlaceHolder = 0x150;      // vtable byte offset: std::string (sret)
constexpr uint32_t ChatEditBoxMessageSend_create = 0x55C6E9;
constexpr uint32_t EditBox_node = 0x2A8;                  // ChatEditBoxMessageSend: Node* (textFieldToEditBox out)
constexpr uint32_t EditBox_label = 0x2AC;                 // Label* (out)
constexpr uint32_t EditBox_delegate = 0x288;              // the EditBoxDelegate base
constexpr uint32_t EditBox_onSend = 0x290;                // std::function<void()>
constexpr uint32_t FunctionV_swap = 0x4EE86D;             // std::function<void()>::swap
constexpr uint32_t Label_setAlignment = rwlib::cocos2d::Label::setAlignment;         // (label, TextHAlignment, TextVAlignment)
constexpr uint32_t Label_vAlignment = 0x3E0;

constexpr uint32_t VT_Report_EditDone = 0xF5F924;         // $_79 [label, this, maxLength, uiText] (heap 0x14)
constexpr uint32_t VT_Report_Close = 0xF5F8F8;            // $_80 [this, close]
constexpr uint32_t VT_Report_List = 0xF5F8A0;             // $_82 [this, send, close, send, close, this, sectionLabel, ids[7]] (heap 0x3C)
constexpr uint32_t VT_Report_Send = 0xF5F848;             // $_84 (heap 0xC0), see openReportPopup
constexpr uint32_t VT_Report_CanClose = 0xF5F81C;         // $_85 [close] bool()
constexpr uint32_t VT_Report_OnClose = 0xF5F7C4;          // $_86 [send, close, this, popup, layer, editBox, shadow] (heap 0x20)
}  // namespace

// "Report this post": the reason (picked from a list), a free text, send or cancel. `callback` runs after the
// report was sent.
//
// The send button's lambda LD [this, label, send, close, message, LB, LC, LA] holds three callback holders built
// first on the stack:
//   LA [close, callback]                  (0x20)
//   LB [close, callback]                  (0x20)
//   LC [this, send, LA]                   (0x28)
// LD (0xB8) is copied from them (strings and std::function copies in member order) and moved into a heap __func.
void SceneManager::openReportPopup(SceneManager *self, Addr message, Addr callback) {
    const uint32_t me = addrOf(self);
    Frame f;
    ChatPopup p = beginChatPopup(LAYER_DERIVED, 4);
    rw::set<uint8_t>(self, SM_reportEditing, 0);
    loadChatLayout(p, STR_ChatPostReportJson, 4);
    const uint32_t popup = p.popup;
    rw::set<int32_t>(self, SM_reportReason, 0);
    label(popup, STR_Txt_Wording, 11, 104200014);
    label(popup, STR_Txt_Post, 8, 104200026);
    label(popup, STR_Txt_Section, 11, 104200015);
    label(popup, STR_Txt_Send, 8, 104200001);
    label(popup, STR_Txt_Cancel, 10, 100100002);
    uint32_t sectionLabel = findNode(popup, STR_Txt_Section_Label, 17);
    SceneUtil::setShadow(sectionLabel);
    setTextId(sectionLabel, int32_t(ld32(REPORT_REASON_TEXTS + 4 * ld32(me + SM_reportReason))));
    label(popup, STR_Txt_Content, 11, 104200016);

    // the free text: a TextField turned into an edit box, with its length limit shown
    uint32_t field = findNode(popup, STR_Txt_Content_Label, 17);
    int32_t maxLength = int32_t(call(TextField_getMaxLength, {field}));
    uint32_t limit = findNode(popup, STR_Txt_Limit, 9);
    SceneUtil::setShadow(limit);
    {
        Frame g;
        uint32_t text = textData(g, 100100037);
        setTextC(limit, stringBuffer(g, {textDataStr(text), uint32_t(maxLength)}));
        sharedRelease(text);
    }
    uint32_t editBox = call(ChatEditBoxMessageSend_create, {});
    vcall(p.layer, node::addChildZ, {p.layer, editBox, 4});
    uint32_t uiText = SceneUtil::textFieldToUiText(field, STR_Empty, STR_CalligraphyFont);
    uint32_t editLabel = 0;
    {
        Frame g;
        uint32_t placeholder = g.alloc(12);
        vcall(field, TextField_getPlaceHolder, {placeholder, field});
        uint32_t size = g.alloc(8);
        stf(size, 470.0f);
        stf(size + 4, 130.0f);
        editLabel = SceneUtil::textFieldToEditBox(editBox + EditBox_node, editBox + EditBox_label, field, placeholder,
                                              STR_Empty, STR_CalligraphyFont, size,
                                              editBox ? editBox + EditBox_delegate : 0, me + SM_reportEditing, 0, 0);
        strFree(placeholder);
    }
    vcall(editLabel, node::setVisible, {editLabel, 0});
    {
        // editBox->onSend = [label, this, maxLength, uiText]
        Frame g;
        uint32_t fo = g.alloc(20);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_Report_EditDone);
            st32(heap + 4, editLabel);
            st32(heap + 8, me);
            st32(heap + 12, uint32_t(maxLength));
            st32(heap + 16, uiText);
        }
        st32(fo + 16, heap);
        call(FunctionV_swap, {fo, editBox + EditBox_onSend});
        functionDestroy(fo);
    }
    call(Label_setAlignment, {editLabel, 0, ld32(editLabel + Label_vAlignment)});

    uint32_t messages = f.alloc(12);
    addMessageView(popup, message, messages, false, f);
    uint32_t send = findNode(popup, STR_Button_Send, 11);
    uint32_t close = findNode(popup, STR_Button_Close, 12);

    uint32_t LA = f.alloc(0x20), LB = f.alloc(0x20), LC = f.alloc(0x28), LD = f.alloc(0xB8);
    st32(LA, close);
    call(fn::function_vv_copy, {LA + 8, callback});
    addTouch(close, VT_Report_Close, {me, close});
    st32(LB, close);
    call(fn::function_vv_copy, {LB + 8, callback});
    st32(LC, me);
    st32(LC + 4, send);
    st32(LC + 8, ld32(LA));
    functionCopy(LC + 0x10, LA + 8);

    // the reason list button
    uint32_t list = findNode(popup, STR_Button_List, 11);
    {
        Frame g;
        uint32_t fo = g.alloc(20);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x3C});
        if (heap) {
            st32(heap, VT_Report_List);
            st32(heap + 0x04, me);
            st32(heap + 0x08, send);
            st32(heap + 0x0C, close);
            st32(heap + 0x10, send);
            st32(heap + 0x14, close);
            st32(heap + 0x18, me);
            st32(heap + 0x1C, sectionLabel);
            for (uint32_t k = 0; k < 7; k++) st32(heap + 0x20 + 4 * k, 104210001 + k);
        }
        st32(fo + 16, heap);
        addTouchFunction(list, fo);
    }

    // the send button: LD
    st32(LD, me);
    st32(LD + 4, editLabel);
    st32(LD + 8, send);
    st32(LD + 0xC, close);
    msgCopy(LD + 0x10, message);
    st32(LD + 0x50, ld32(LB));
    functionCopy(LD + 0x58, LB + 8);
    copyWords(LD + 0x70, LC, 12);
    functionCopy(LD + 0x80, LC + 0x10);
    st32(LD + 0x98, ld32(LA));
    functionCopy(LD + 0xA0, LA + 8);
    uint32_t onSend = f.alloc(20);
    st32(onSend + 16, 0);
    uint32_t heap = call(fn::operator_new, {0xC0});
    if (heap) {
        const uint32_t h = heap + 8;
        st32(heap, VT_Report_Send);
        copyWords(h, LD, 16);
        msgMove(h + 0x10, LD + 0x10);
        st32(h + 0x50, ld32(LD + 0x50));
        call(fn::function_vv_move, {h + 0x58, LD + 0x58});
        copyWords(h + 0x70, LD + 0x70, 12);
        call(fn::function_vv_move, {h + 0x80, LD + 0x80});
        st32(h + 0x78, ld32(LD + 0x78));
        st32(h + 0x98, ld32(LD + 0x98));
        call(fn::function_vv_move, {h + 0xA0, LD + 0xA0});
        st32(h + 0x98, ld32(LD + 0x98));
    }
    st32(onSend + 16, heap);
    addTouchFunction(send, onSend);
    functionDestroy(LD + 0xA0);
    functionDestroy(LD + 0x80);
    functionDestroy(LD + 0x58);
    msgDestroy(LD + 0x10);

    runOpenAnimation(popup);
    uint32_t mgr = addrOf(getInstance());
    uint32_t canClose = newFunction(f, VT_Report_CanClose, {close});
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t closeHeap = call(fn::operator_new, {0x20});
    if (closeHeap) {
        st32(closeHeap, VT_Report_OnClose);
        st32(closeHeap + 0x04, send);
        st32(closeHeap + 0x08, close);
        st32(closeHeap + 0x0C, me);
        st32(closeHeap + 0x10, popup);
        st32(closeHeap + 0x14, p.layer);
        st32(closeHeap + 0x18, editBox);
        st32(closeHeap + 0x1C, p.shadow);
    }
    st32(onClose + 16, closeHeap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(canClose);
    functionDestroy(LC + 0x10);
    functionDestroy(LB + 8);
    functionDestroy(LA + 8);
    call(MessageVector_destroy, {messages});
}
RW_FUNCTION(0x61C354, "SceneManager::openReportPopup", SceneManager::openReportPopup);

// ------------------------------------------------------------------------------------------------ report reasons

namespace {
constexpr uint32_t STR_ChatPostReportWinJson = 0xE034FA;  // "cocostudio/publish/Chat_Post_ReportWin.json"
constexpr uint32_t STR_Scroll_Area = 0xDF9B59;
constexpr uint32_t LAYER_DERIVED2 = 0x3F8;                // SceneManager popupLayerDerived2

constexpr uint32_t FunctionI_copy = 0x539C99;             // std::function<void(int)>(const function&)
constexpr uint32_t FunctionI_move = 0x543045;             // std::function<void(int)>(function&&)
constexpr uint32_t FunctionI_swap = 0x5428E1;             // std::function<void(int)>::swap
constexpr uint32_t ChatReportScrollView_create = 0x55A211;
constexpr uint32_t ChatReportScrollView_setIds = 0x55A7A9;  // (view, count)
constexpr uint32_t ReportView_onSelect = 0x638;           // ChatReportScrollView: std::function<void(int)>
constexpr uint32_t ReportView_selected = 0x650;           // int: the selected reason

constexpr uint32_t VT_ReportSelect_Close = 0xF5F798;      // $_87 []
constexpr uint32_t VT_ReportSelect_Select = 0xF5F76C;     // $_88 [close, callback] (heap 0x28)
constexpr uint32_t VT_ReportSelect_CanClose = 0xF5F740;   // $_89 [close] bool()
constexpr uint32_t VT_ReportSelect_OnClose = 0xF5F6E8;    // $_90 [callback, popup, layer, shadow, close] (heap 0x30)
}  // namespace

// The list of report reasons (7), `selected` highlighted; picking one calls `callback` with its index.
void SceneManager::openReportSelectPopup(SceneManager *, int32_t selected, Addr callback) {
    ChatPopup p = buildChatPopup(STR_ChatPostReportWinJson, LAYER_DERIVED2, 6);
    const uint32_t popup = p.popup;
    Frame f;
    uint32_t close = findNode(popup, STR_Close_Button, 12);
    // the close callback's captures [callback, popup, layer, shadow, close], copied into it at the end
    uint32_t closing = f.alloc(0x28);
    call(FunctionI_copy, {closing, callback});
    st32(closing + 0x18, popup);
    st32(closing + 0x1C, p.layer);
    st32(closing + 0x20, p.shadow);
    st32(closing + 0x24, close);
    addTouch(close, VT_ReportSelect_Close, {});

    uint32_t area = findNode(popup, STR_Scroll_Area, 11);
    uint32_t view = call(ChatReportScrollView_create, {});
    uint32_t size = vcall(area, node::getContentSize, {area});
    vcall(view, node::setContentSize, {view, size});
    call(ChatReportScrollView_setIds, {view, 7});
    rw::set<int32_t>(rw::host_ptr<uint8_t>(view), ReportView_selected, selected);
    vcall(area, node::addChild, {area, view});

    // view->onSelect = [close, callback]: built as a temporary, moved into a heap __func, swapped in
    uint32_t select = f.alloc(0x20);
    st32(select, close);
    call(FunctionI_copy, {select + 8, callback});
    uint32_t moved = f.alloc(0x20);
    st32(moved, ld32(select));
    call(FunctionI_move, {moved + 8, select + 8});
    st32(moved, ld32(select));
    {
        Frame g;
        uint32_t fo = g.alloc(20);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x28});
        if (heap) {
            st32(heap, VT_ReportSelect_Select);
            st32(heap + 8, ld32(moved));
            call(FunctionI_move, {heap + 0x10, moved + 8});
            st32(heap + 8, ld32(moved));
        }
        st32(fo + 16, heap);
        call(FunctionI_swap, {fo, view + ReportView_onSelect});
        functionDestroy(fo);
    }
    functionDestroy(moved + 8);
    functionDestroy(select + 8);

    runOpenAnimation(popup);
    uint32_t mgr = addrOf(getInstance());
    uint32_t canClose = newFunction(f, VT_ReportSelect_CanClose, {close});
    uint32_t captured = f.alloc(0x28);
    functionCopy(captured, closing);
    copyWords(captured + 0x18, closing + 0x18, 16);
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x30});
    if (heap) {
        st32(heap, VT_ReportSelect_OnClose);
        call(FunctionI_move, {heap + 8, captured});
        copyWords(heap + 0x20, captured + 0x18, 16);
    }
    st32(onClose + 16, heap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(captured);
    functionDestroy(canClose);
    functionDestroy(closing);
}
RW_FUNCTION(0x61D6E4, "SceneManager::openReportSelectPopup", SceneManager::openReportSelectPopup);

// ------------------------------------------------------------------------------------------------ block list

namespace {
constexpr uint32_t STR_ChatPostBlockListJson = 0xE03526;  // "cocostudio/publish/Chat_Post_BlockList.json"
constexpr uint32_t STR_Txt_Save = 0xE03552;
constexpr uint32_t STR_Button_Save = 0xE0355B;

// SceneManager fields outside the declared layout
constexpr uint32_t SM_blocks = 0x454;                     // std::vector<ChatManager::Block> (begin, end, capacity)
constexpr uint32_t SM_blockedFlags = 0x460;               // std::vector<bool> (words, bit count, capacity): still blocked

// ChatManager::Block (0x44 bytes): five strings, then 8 bytes
constexpr uint32_t BLOCK_SIZE = 0x44;
constexpr uint32_t BLOCK_NAME = 0x00, BLOCK_STR_0C = 0x0C, BLOCK_USER_ID = 0x18, BLOCK_STR_24 = 0x24,
                   BLOCK_STR_30 = 0x30, BLOCK_TAIL = 0x3C;
// ChatBlockListScrollView::ChatBlock (0x1C bytes)
constexpr uint32_t CHATBLOCK_SIZE = 0x1C;
constexpr uint32_t CHATBLOCK_USER_ID = 0x00;              // int (the Block's user id string, parsed)
constexpr uint32_t CHATBLOCK_BLOCKED = 0x04;              // uint8
constexpr uint32_t CHATBLOCK_TAIL = 0x06;                 // the Block's last 8 bytes
constexpr uint32_t CHATBLOCK_NAME = 0x10;                 // std::string

constexpr uint32_t Block_copy = 0x4DEB5F;                 // ChatManager::Block(const Block&)
constexpr uint32_t Stoi = 0xC93E8B;                       // std::stoi(const string&, size_t*, int base)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;            // throws std::out_of_range("vector")
constexpr uint32_t ChatBlockVector_pushBackSlow = 0x627CD1;
constexpr uint32_t ChatBlockVector_copy = 0x61E6A5;
constexpr uint32_t ChatBlockVector_destroy = 0x55A989;
constexpr uint32_t ChatBlockListScrollView_create = 0x55AA29;
constexpr uint32_t ChatBlockListScrollView_setBlockList = 0x55B7A1;   // (view, vector<ChatBlock> by value)
constexpr uint32_t BlockListView_onChange = 0x638;        // std::function<void()>

constexpr uint32_t VT_BlockList_Close = 0xF5F6BC;         // $_91 []
constexpr uint32_t VT_BlockList_Change = 0xF5F690;        // $_92 [this, view]
constexpr uint32_t VT_BlockList_CanClose = 0xF5F664;      // $_93 [close] bool()
constexpr uint32_t VT_BlockList_OnClose = 0xF5F60C;       // $_94 [this, close, this, popup, layer, shadow] (heap 0x1C)
}  // namespace

// The block list (users this user blocked), each with its blocked flag; a notice dialog when there is none.
void SceneManager::openBlockListPopup(SceneManager *self) {
    // the original frame (push 0x14 + 0x1FC): a ChatBlock temporary at its original offset, whose padding byte
    // (+5, never written) is copied into the vector
    Frame frame;
    const uint32_t base = frame.allocExact(0x210);
    const uint32_t entry = base + 0x74, block = base + 0x90, blocks = base + 0xD4, byValue = base + 0x68;
    const uint32_t me = addrOf(self);
    if (ld32(me + SM_blocks + 4) == ld32(me + SM_blocks)) {
        Frame f;
        uint32_t text = textData(f, 104200011);
        uint32_t message = newStr(f, textDataStr(text));
        uint32_t none = newEmptyFunction(f);
        SceneUtil::openOkDialog4Line(message, none, 0);
        functionDestroy(none);
        strFree(message);
        sharedRelease(text);
        return;
    }
    ChatPopup p = beginChatPopup(LAYER_DERIVED, 4);
    self->chatBlockCount = 0;
    loadChatLayout(p, STR_ChatPostBlockListJson, 4);
    const uint32_t popup = p.popup;
    Frame f;
    label(popup, STR_Txt_Wording, 11, 104200013);
    label(popup, STR_Txt_Save, 8, 104200006);
    uint32_t close = findNode(popup, STR_Close_Button, 12);
    addTouch(close, VT_BlockList_Close, {});
    uint32_t save = findNode(popup, STR_Button_Save, 11);
    vcall(save, node::setVisible, {save, 0});

    uint32_t area = findNode(popup, STR_Area, 4);
    uint32_t view = call(ChatBlockListScrollView_create, {});
    uint32_t size = vcall(area, node::getContentSize, {area});
    vcall(view, node::setContentSize, {view, size});

    // the list entries: one ChatBlock per ChatManager::Block
    st32(blocks, 0);
    st32(blocks + 4, 0);
    st32(blocks + 8, 0);
    uint32_t begin = ld32(me + SM_blocks), end = ld32(me + SM_blocks + 4);
    if (end != begin) {
        for (uint32_t index = 0, offset = 0;; index++, offset += BLOCK_SIZE) {
            if (((end - begin) >> 2) * 0xF0F0F0F1u <= index) call(ThrowOutOfRange, {me + SM_blocks});
            call(Block_copy, {block, begin + offset});
            if (ld32(me + SM_blockedFlags + 4) <= index) call(ThrowOutOfRange, {me + SM_blockedFlags});
            uint32_t mask = 1u << (index & 31);
            uint32_t flags = ld32(me + SM_blockedFlags);
            int32_t userId = 0;
            if (strSize(block + BLOCK_USER_ID)) userId = int32_t(call(Stoi, {block + BLOCK_USER_ID, 0, 10}));
            st32(entry + CHATBLOCK_USER_ID, uint32_t(userId));
            st8(entry + CHATBLOCK_BLOCKED, (ld32(flags + 4 * (index >> 5)) & mask) ? 1 : 0);
            uint32_t tail0 = ld32(block + BLOCK_TAIL), tail1 = ld32(block + BLOCK_TAIL + 4);
            rw::save<uint16_t>(entry + CHATBLOCK_TAIL + 4, uint16_t(tail1));
            rw::save<uint16_t>(entry + CHATBLOCK_TAIL, uint16_t(tail0));
            rw::save<uint16_t>(entry + CHATBLOCK_TAIL + 6, uint16_t(tail1 >> 16));
            rw::save<uint16_t>(entry + CHATBLOCK_TAIL + 2, uint16_t(tail0 >> 16));
            strCopy(entry + CHATBLOCK_NAME, block + BLOCK_NAME);
            // blocks.push_back(entry)
            uint32_t last = ld32(blocks + 4);
            if (last == ld32(blocks + 8)) {
                call(ChatBlockVector_pushBackSlow, {blocks, entry});
            } else {
                if (last) {
                    copyWords(last, entry, 12);
                    rw::save<uint16_t>(last + 12, rw::load<uint16_t>(entry + 12));
                    strCopy(last + CHATBLOCK_NAME, entry + CHATBLOCK_NAME);
                }
                st32(blocks + 4, ld32(blocks + 4) + CHATBLOCK_SIZE);
            }
            strFree(entry + CHATBLOCK_NAME);
            strFree(block + BLOCK_STR_30);
            strFree(block + BLOCK_STR_24);
            strFree(block + BLOCK_USER_ID);
            strFree(block + BLOCK_STR_0C);
            strFree(block + BLOCK_NAME);
            begin = ld32(me + SM_blocks);
            end = ld32(me + SM_blocks + 4);
            if (index + 1 >= ((end - begin) >> 2) * 0xF0F0F0F1u) break;
        }
    }
    call(ChatBlockVector_copy, {byValue, blocks});
    call(ChatBlockListScrollView_setBlockList, {view, byValue});
    call(ChatBlockVector_destroy, {byValue});
    vcall(area, node::addChild, {area, view});
    {
        // view->onChange = [this, view]
        Frame g;
        uint32_t fo = newFunction(g, VT_BlockList_Change, {me, view});
        call(FunctionV_swap, {fo, view + BlockListView_onChange});
        functionDestroy(fo);
    }

    runOpenAnimation(popup);
    uint32_t mgr = addrOf(getInstance());
    uint32_t canClose = newFunction(f, VT_BlockList_CanClose, {close});
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t closeHeap = call(fn::operator_new, {0x1C});
    if (closeHeap) {
        st32(closeHeap, VT_BlockList_OnClose);
        st32(closeHeap + 0x04, me);
        st32(closeHeap + 0x08, close);
        st32(closeHeap + 0x0C, me);
        st32(closeHeap + 0x10, popup);
        st32(closeHeap + 0x14, p.layer);
        st32(closeHeap + 0x18, p.shadow);
    }
    st32(onClose + 16, closeHeap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(canClose);
    call(ChatBlockVector_destroy, {blocks});
}
RW_FUNCTION(0x61DCE0, "SceneManager::openBlockListPopup", SceneManager::openBlockListPopup);

// ------------------------------------------------------------------------------------------------ unblock

namespace {
constexpr uint32_t VT_UnBlock_Close = 0xF5F5E0;           // $_95 []
constexpr uint32_t VT_UnBlock_Block = 0xF5F55C;           // $_99 (heap 0xE0), see openUnBlockPostPopup
constexpr uint32_t VT_UnBlock_CanClose = 0xF5F530;        // $_100 [close] bool()
constexpr uint32_t VT_UnBlock_OnClose = 0xF5F4D8;         // $_101 [block, close, this, popup, layer, shadow] (heap 0x1C)

// ChatBlockListScrollView::ChatBlock copy / move / destructor as inlined (3 words, a halfword, the name; the last
// two bytes before the name are padding)
void chatBlockCopy(uint32_t dst, uint32_t src) {
    copyWords(dst, src, 12);
    rw::save<uint16_t>(dst + 12, rw::load<uint16_t>(src + 12));
    strCopy(dst + CHATBLOCK_NAME, src + CHATBLOCK_NAME);
}
void chatBlockMove(uint32_t dst, uint32_t src) {
    copyWords(dst, src, 12);
    rw::save<uint16_t>(dst + 12, rw::load<uint16_t>(src + 12));
    strMove(dst + CHATBLOCK_NAME, src + CHATBLOCK_NAME);
}
}  // namespace

// "Unblock this user?" from the block list: the user's avatar and name, then the same nested callback holders as
// openBlockPostPopup with a ChatBlock in place of the message:
//   L1 [block, close, callback]                                   (0x20)
//   L2 [this, block, close, callback, L1]                         (0x48)
//   L3 [this, chatBlock, L2, L1, block, close]                    (0x90)
//   L4 [this, block, close, chatBlock, L3, L1]                    (0xD8, the heap __func at +8)
void SceneManager::openUnBlockPostPopup(SceneManager *self, Addr chatBlock, Addr callback) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x560);     // push 0x14 + 0x54C: temporaries at their original offsets
    const uint32_t L1 = base + 0x3E0, L2 = base + 0x398, L3 = base + 0x308, L4 = base + 0x218;
    const uint32_t me = addrOf(self);
    ChatPopup p = buildChatPopup(STR_ChatPostBlockJson, LAYER_DERIVED, 6);
    const uint32_t popup = p.popup;
    label(popup, STR_Txt_Wording1, 12, 104200030);
    uint32_t wording2 = findNode(popup, STR_Txt_Wording2, 12);
    vcall(wording2, node::setVisible, {wording2, 0});
    label(popup, STR_Txt_Block, 9, 100100001);
    label(popup, STR_Txt_Cancel, 10, 100100002);

    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t avatarsCtl = ld32(api + APIManager_userAvatars + 4);
    uint32_t avatars = ld32(api + APIManager_userAvatars);
    if (avatarsCtl) call(fn::shared_add, {avatarsCtl});
    uint32_t nameLabel = findNode(popup, STR_Txt_Name_Label, 14);
    SceneUtil::setShadow(nameLabel);
    uint32_t avatarNode = findNode(popup, STR_Avatar, 6);
    int32_t userId = int32_t(ld32(chatBlock + CHATBLOCK_USER_ID));
    showPostUser(base, UNBLOCK_POST_TEMPS, popup, nameLabel, avatarNode, avatars, uint32_t(userId),
                 uint32_t(userId >> 31), false);

    uint32_t close = findNode(popup, STR_Button_Close, 12);
    uint32_t block = findNode(popup, STR_Button_Block, 12);
    addTouch(close, VT_UnBlock_Close, {});

    // L1, L2
    st32(L1, block);
    st32(L1 + 4, close);
    call(fn::function_vv_copy, {L1 + 8, callback});
    st32(L2, me);
    st32(L2 + 4, block);
    st32(L2 + 8, close);
    call(fn::function_vv_copy, {L2 + 0x10, callback});
    copyWords(L2 + 0x28, L1, 8);
    functionCopy(L2 + 0x30, L1 + 8);
    // L3
    st32(L3, me);
    chatBlockCopy(L3 + 4, chatBlock);
    copyWords(L3 + 0x20, L2, 12);
    functionCopy(L3 + 0x30, L2 + 0x10);
    copyWords(L3 + 0x48, L2 + 0x28, 8);
    functionCopy(L3 + 0x50, L2 + 0x30);
    copyWords(L3 + 0x68, L1, 8);
    functionCopy(L3 + 0x70, L1 + 8);
    st32(L3 + 0x88, block);
    st32(L3 + 0x8C, close);
    // L4
    st32(L4, me);
    st32(L4 + 4, block);
    st32(L4 + 8, close);
    chatBlockCopy(L4 + 0xC, chatBlock);
    st32(L4 + 0x28, ld32(L3));
    chatBlockCopy(L4 + 0x2C, L3 + 4);
    st32(L4 + 0x28, ld32(L3));
    copyWords(L4 + 0x48, L3 + 0x20, 12);
    functionCopy(L4 + 0x58, L3 + 0x30);
    copyWords(L4 + 0x70, L3 + 0x48, 8);
    functionCopy(L4 + 0x78, L3 + 0x50);
    st32(L4 + 0x28, ld32(L3));
    copyWords(L4 + 0x90, L3 + 0x68, 8);
    functionCopy(L4 + 0x98, L3 + 0x70);
    copyWords(L4 + 0xB0, L3 + 0x88, 8);
    copyWords(L4 + 0xB8, L1, 8);
    functionCopy(L4 + 0xC0, L1 + 8);
    // the block button: L4 moved into a heap __func
    Frame f;
    uint32_t onBlock = f.alloc(20);
    st32(onBlock + 16, 0);
    uint32_t heap = call(fn::operator_new, {0xE0});
    if (heap) {
        const uint32_t h = heap + 8;
        st32(heap, VT_UnBlock_Block);
        copyWords(h, L4, 12);
        chatBlockMove(h + 0xC, L4 + 0xC);
        st32(h + 0x28, ld32(L4 + 0x28));
        chatBlockMove(h + 0x2C, L4 + 0x2C);
        copyWords(h + 0x48, L4 + 0x48, 12);
        call(fn::function_vv_move, {h + 0x58, L4 + 0x58});
        copyWords(h + 0x70, L4 + 0x70, 8);
        call(fn::function_vv_move, {h + 0x78, L4 + 0x78});
        st32(h + 0x28, ld32(L4 + 0x28));
        copyWords(h + 0x90, L4 + 0x90, 8);
        call(fn::function_vv_move, {h + 0x98, L4 + 0x98});
        copyWords(h + 0xB0, L4 + 0xB0, 8);
        copyWords(h + 0xB8, L4 + 0xB8, 8);
        call(fn::function_vv_move, {h + 0xC0, L4 + 0xC0});
    }
    st32(onBlock + 16, heap);
    addTouchFunction(block, onBlock);
    functionDestroy(L4 + 0xC0);
    functionDestroy(L4 + 0x98);
    functionDestroy(L4 + 0x78);
    functionDestroy(L4 + 0x58);
    strFree(L4 + 0x2C + CHATBLOCK_NAME);
    strFree(L4 + 0xC + CHATBLOCK_NAME);

    runOpenAnimation(popup);
    uint32_t mgr = addrOf(getInstance());
    uint32_t canClose = newFunction(f, VT_UnBlock_CanClose, {close});
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t closeHeap = call(fn::operator_new, {0x1C});
    if (closeHeap) {
        st32(closeHeap, VT_UnBlock_OnClose);
        st32(closeHeap + 0x04, block);
        st32(closeHeap + 0x08, close);
        st32(closeHeap + 0x0C, me);
        st32(closeHeap + 0x10, popup);
        st32(closeHeap + 0x14, p.layer);
        st32(closeHeap + 0x18, p.shadow);
    }
    st32(onClose + 16, closeHeap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(canClose);
    // L3, L2, L1
    functionDestroy(L3 + 0x70);
    functionDestroy(L3 + 0x50);
    functionDestroy(L3 + 0x30);
    strFree(L3 + 4 + CHATBLOCK_NAME);
    functionDestroy(L2 + 0x30);
    functionDestroy(L2 + 0x10);
    functionDestroy(L1 + 8);
    if (avatarsCtl) call(fn::shared_release, {avatarsCtl});
}
RW_FUNCTION(0x61E704, "SceneManager::openUnBlockPostPopup", SceneManager::openUnBlockPostPopup);
