// SceneManager: the chat window - polling the chat server (ChatManager), filtering the received messages (hidden,
// blocked users, special system messages), turning them into ChatScrollView rows, and the text helpers the chat
// uses (UTF-8 line wrapping, stamp message parsing, HTML entity decoding).
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
// ---- SceneManager chat fields (not in the layout yet)
constexpr uint32_t CHAT_BADGE = 0x3FC;           // cocos2d::Node*: the "new chat message" mark (shown while closed)
constexpr uint32_t CHAT_LAYOUT = 0x400;          // the chat popup's root widget
constexpr uint32_t CHAT_SCROLL = 0x404;          // ChatScrollView*
constexpr uint32_t CHAT_TOTAL = 0x408;           // uint32: messages on the server
constexpr uint32_t CHAT_START = 0x40C;           // uint32: first message requested (pages of 50)
constexpr uint32_t CHAT_LAST_SEEN_ID = 0x414;    // uint32: newest message id seen
constexpr uint32_t CHAT_SCROLL_TO_ID = 0x418;    // uint32: message id the scroll view keeps in place (0: none)
constexpr uint32_t CHAT_ROW_COUNT = 0x420;       // int32: rows given to the scroll view
constexpr uint32_t CHAT_42C = 0x42C;             // uint32: cleared when a message request starts
constexpr uint32_t CHAT_MESSAGES = 0x430;        // std::vector<ChatManager::Message>: the last received page
constexpr uint32_t CHAT_HIDDEN_IDS = 0x43C;      // std::vector<uint32>: message ids not shown
constexpr uint32_t CHAT_BLOCKED_IDS = 0x448;     // std::vector<std::string>: user ids blocked on this device
constexpr uint32_t CHAT_BLOCKS = 0x454;          // std::vector<ChatManager::Block>: users blocked on the server
constexpr uint32_t CHAT_REQUESTING = 0x46D;      // uint8: a message request is running
constexpr uint32_t CHAT_LOADING_PAGE = 0x46E;    // uint8: a page request (scrolled to an end) is running
constexpr uint32_t CHAT_RELOAD_VIEW = 0x46F;     // uint8: rebuild the scroll view (users of new messages unknown)
constexpr uint32_t CHAT_TIMER = 0x474;           // float: seconds to the next poll
constexpr uint32_t STAMP_MIGRATING = 0x498;      // uint8 (flag498): the stamp list pages
constexpr uint32_t STAMP_PAGE = 0x49C;           // int32 (field49C)
constexpr uint32_t STAMP_TIMER = 0x4A0;          // float (field4A0)
constexpr uint32_t STAMP_LEFT = 0x4A4;           // uint8: page to the left
constexpr uint32_t STAMP_LIST = 0x4A8;           // BaseStageScrollView*
constexpr uint32_t STAMP_MIGRATE_RESULT = 0x2FC; // uint8 (flag2FC): keeps paging while true

// ---- ChatManager::Message (76 bytes)
constexpr uint32_t MESSAGE_SIZE = 0x4C;
constexpr uint32_t MSG_ID = 0x00, MSG_NAME = 0x04, MSG_STR10 = 0x10, MSG_USER_ID = 0x1C, MSG_STR28 = 0x28;
constexpr uint32_t MSG_TEXT = 0x34, MSG_DELETED = 0x40, MSG_TIME = 0x44;     // MSG_TIME: two words
// ---- ChatManager::Block (68 bytes): the blocked user id at +0x18
constexpr uint32_t BLOCK_SIZE = 0x44, BLOCK_USER_ID = 0x18;
// ---- ChatScrollView::ChatMessage (64 bytes)
constexpr uint32_t ROW_SIZE = 0x40;
constexpr uint32_t ROW_USER_ID = 0x00;           // int64
constexpr uint32_t ROW_IS_STAMP = 0x08, ROW_STAMP_ID = 0x0C, ROW_IS_NEW = 0x10, ROW_TEXT = 0x14;
constexpr uint32_t ROW_TIME = 0x20, ROW_MESSAGE_ID = 0x28, ROW_NAME = 0x2C, ROW_38 = 0x38;

// ---- APIManager
constexpr uint32_t API_USER_STATUS = 0x1D4;      // shared_ptr<...> (SafeBuffer: chat level at 0)
constexpr uint32_t API_USER_HEADER = 0x84;       // shared_ptr<...> (SafeBuffer: chat enabled byte at 16)
constexpr uint32_t API_USER_MAP = 0x1FC;         // shared_ptr<...>: unordered_map<u64 user id, int> at +0x24
constexpr int32_t CHAT_MIN_LEVEL = 12;           // non-stamp messages show above this value

// ---- guest functions
constexpr uint32_t ChatManager_getInstance = 0x4DB0E5;
constexpr uint32_t ChatManager_getUser = 0x4DB271;          // (cm, const function<void(StatusCode,u,u,const vector<Block>&)>&)
constexpr uint32_t ChatManager_getMessage = 0x4DB7E1;       // (cm, count, MessageGetType, start, const function&)
constexpr uint32_t Message_copy = 0x4DE205;                 // ChatManager::Message(const Message&)
constexpr uint32_t Block_copy = 0x4DEB5F;                   // ChatManager::Block(const Block&)
constexpr uint32_t MessageVector_pushSlow = 0x4DDF05;       // vector<Message>::__push_back_slow_path(const&)
constexpr uint32_t RowVector_pushSlow = 0x630CBD;           // vector<ChatScrollView::ChatMessage>::__push_back_slow_path
constexpr uint32_t RowVector_copy = 0x617429;               // vector<ChatScrollView::ChatMessage>(const vector&)
constexpr uint32_t RowVector_destroy = 0x5568AD;            // __vector_base<ChatScrollView::ChatMessage>::~
constexpr uint32_t StringVector_pushSlow = 0x484465;        // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t StringVector_destroy = 0x47363F;         // __vector_base<string>::~
constexpr uint32_t ChatScrollView_setMessages = 0x558711;   // (view, vector by value, uint32 keep id)
constexpr uint32_t ChatScrollView_getMessageId = 0x558A35;  // (view, int index)
constexpr uint32_t ChatScrollView_getMessageMaxNum = 0x558A75;
constexpr uint32_t IScrollProtocol_getScrollIndex = 0x4F7B79;
constexpr uint32_t BaseStageScrollView_migrationLeft = 0x59DA99, BaseStageScrollView_migrationRight = 0x59DAB3;
constexpr uint32_t LoadingIndicator_hide = 0x509D69;
constexpr uint32_t FunctionB_assign = 0x490109;             // std::function<void(bool)>::operator=
constexpr uint32_t UserMap_find = 0x490275;                 // unordered_map<unsigned long long, int>::find
constexpr uint32_t Stoi = 0xC93E8B;                         // std::stoi(const string&, size_t*, int base)
constexpr uint32_t StringCompare = 0x47CBD5;                // std::string::compare(const char*)
constexpr uint32_t StringReplace = 0x4B6DCD;                // std::string::replace(pos, n, const char*, n2)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t ImageView_getCapInsets = rwlib::cocos2d::ui::ImageView::getCapInsets;
constexpr uint32_t Widget_getCustomSize = rwlib::cocos2d::ui::Widget::getCustomSize;
constexpr uint32_t Size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_, Size_assign = rwlib::cocos2d::Size::op_assign__RKS0_;
constexpr uint32_t SceneManager_getChatSpMessageCheck = 0x615B19;
constexpr uint32_t SceneManager_getChatStampMessageCheck = 0x615EA5;
constexpr uint32_t SceneManager_getChatStampId = 0x616249;
constexpr uint32_t SceneManager_getStringHtmlEncodeUtf = 0x616439;
constexpr uint32_t WIDGET_SET_CONTENT_SIZE = 0x2B0;         // vtable byte offset used on the chat widgets

// lambdas of chatDataUpdate
constexpr uint32_t VT_ChatData_IdleUser = 0xF601E8;         // $_34 [this]: getUser while the chat is closed
constexpr uint32_t VT_ChatData_Messages = 0xF60190;         // $_36 [this, this]: getMessage result
constexpr uint32_t VT_ChatData_User = 0xF60164;             // $_37 [this, this, this]: getUser result

// strings
constexpr uint32_t STR_Empty = 0xDF8A1A, STR_NewLine = 0xDF9E14, STR_Space = 0xDF9E16;
constexpr uint32_t STR_ChatWin = 0xE03314, STR_Panel = 0xDFD420;
constexpr uint32_t STR_TagC = 0xE0331D, STR_TagU = 0xE03321, STR_Gt = 0xE03325, STR_TagE = 0xE03327;
constexpr uint32_t STR_TagL = 0xE0332B;
constexpr uint32_t STR_StampMedal = 0xE0332F, STR_StampTalks = 0xE0333D;     // "ISTAMP:MEDAL:", "ISTAMP:TALKS:"
constexpr uint32_t STR_Medal = 0xE0334B, STR_Talks = 0xDFED49;               // "MEDAL:", "TALKS:"

uint32_t mgrAddr(SceneManager *self) { return rw::guest_addr(self); }
uint32_t field(SceneManager *self, uint32_t off) { return ld32(mgrAddr(self) + off); }
void setField(SceneManager *self, uint32_t off, uint32_t v) { st32(mgrAddr(self) + off, v); }
uint8_t flag(SceneManager *self, uint32_t off) { return ld8(mgrAddr(self) + off); }
void setFlag(SceneManager *self, uint32_t off, uint8_t v) { st8(mgrAddr(self) + off, v); }

// ------------------------------------------------------------------------------------------------ strings
// equal contents (the inlined operator==)
bool strEq(uint32_t a, uint32_t b) {
    uint32_t n = strSize(a);
    if (n != strSize(b)) return false;
    return std::memcmp(rw::host_ptr<uint8_t>(strData(a)), rw::host_ptr<uint8_t>(strData(b)), n) == 0;
}
// string::find(needle, from) as libc++ inlines it (std::__search with char equality)
uint32_t strFind(uint32_t s, uint32_t needle, uint32_t from) {
    uint32_t n = strSize(s), m = strSize(needle);
    if (from > n || n - from < m) return 0xFFFFFFFFu;
    if (m == 0) return from;
    const uint8_t *d = rw::host_ptr<uint8_t>(strData(s));
    const uint8_t *k = rw::host_ptr<uint8_t>(strData(needle));
    for (uint32_t p = from; p + m <= n; p++)
        if (std::memcmp(d + p, k, m) == 0) return p;
    return 0xFFFFFFFFu;
}
// bytes of the UTF-8 sequence starting with `c` (the chat's own table: an invalid lead byte counts 3)
uint32_t utf8Length(int8_t c) {
    if (c >= 0) return 1;
    uint8_t b = uint8_t(c);
    if ((b >> 5) < 7) return 2;
    if ((b >> 4) < 0xF) return 3;
    if ((b >> 3) < 0x1F) return 4;
    if ((b >> 2) < 0x3F) return 5;
    if ((b >> 1) < 0x7F) return 6;
    return 3;
}
int8_t strByte(uint32_t s, uint32_t pos) { return int8_t(ld8(strData(s) + pos)); }

// ChatManager::Message destructor (its strings, last to first)
void messageFree(uint32_t m) {
    strFree(m + MSG_TEXT);
    strFree(m + MSG_STR28);
    strFree(m + MSG_USER_ID);
    strFree(m + MSG_STR10);
    strFree(m + MSG_NAME);
}

// the message id is in the hidden list
bool isHiddenMessage(SceneManager *self, uint32_t id) {
    uint32_t begin = field(self, CHAT_HIDDEN_IDS), end = field(self, CHAT_HIDDEN_IDS + 4);
    if (end == begin) return false;
    uint32_t n = (end - begin) >> 2;
    bool found = false;
    for (uint32_t k = 0; k < n; k++) found |= ld32(begin + 4 * k) == id;
    return found;
}

// the user's chat value (SafeBuffer int at 0 of the APIManager user status)
int32_t chatLevel(uint32_t statusBuffer) {
    Frame f;
    return int32_t(ld32(safeBytes(f, statusBuffer, 0, 4)));
}

// Fill the ChatScrollView row at `row` (the caller's stack) from the message and push a copy onto `rows`
// (std::vector<ChatScrollView::ChatMessage>); frees the row's strings and the decoded text `text` after.
void pushRow(SceneManager *self, uint32_t rows, uint32_t row, uint32_t msg, int32_t userId, uint32_t stamp,
             uint32_t stampId, bool isNew, uint32_t text) {
    st32(row + ROW_USER_ID, uint32_t(userId));
    st32(row + ROW_USER_ID + 4, uint32_t(userId >> 31));
    st8(row + ROW_IS_STAMP, uint8_t(stamp));
    st32(row + ROW_STAMP_ID, stampId);
    st8(row + ROW_IS_NEW, isNew ? 1 : 0);
    strCopy(row + ROW_TEXT, text);
    st32(row + ROW_TIME, ld32(msg + MSG_TIME));
    st32(row + ROW_TIME + 4, ld32(msg + MSG_TIME + 4));
    st32(row + ROW_MESSAGE_ID, ld32(msg + MSG_ID));
    strCopy(row + ROW_NAME, msg + MSG_NAME);
    st8(row + ROW_38, 0);
    // push_back(row), inlined: copy-construct at the end
    uint32_t end = ld32(rows + 4);
    if (end == ld32(rows + 8)) {
        call(RowVector_pushSlow, {rows, row});
    } else {
        if (end) {
            for (uint32_t k = 0; k < 16; k += 4) st32(end + k, ld32(row + k));   // the stamp byte's padding too
            st8(end + ROW_IS_NEW, ld8(row + ROW_IS_NEW));
            strCopy(end + ROW_TEXT, row + ROW_TEXT);
            for (uint32_t k = 0; k < 12; k += 4) st32(end + ROW_TIME + k, ld32(row + ROW_TIME + k));
            strCopy(end + ROW_NAME, row + ROW_NAME);
            st8(end + ROW_38, 0);
            end = ld32(rows + 4);
        }
        st32(rows + 4, end + ROW_SIZE);
    }
    setField(self, CHAT_ROW_COUNT, field(self, CHAT_ROW_COUNT) + 1);
    strFree(row + ROW_NAME);
    strFree(row + ROW_TEXT);
    strFree(text);
}

// A message the chat shows: not deleted, not hidden, has a user, not a special message, user not blocked.
// Returns the user id (stoi of the user id string) through *userId.
bool passesFilters(SceneManager *self, uint32_t msg, int32_t *userId) {
    if (ld32(msg + MSG_DELETED)) return false;
    if (isHiddenMessage(self, ld32(msg + MSG_ID))) return false;
    if (strSize(msg + MSG_USER_ID) == 0) return false;
    *userId = int32_t(call(Stoi, {msg + MSG_USER_ID, 0, 10}));
    // the original passes the user id as `this` (the member does not use it)
    if (call(SceneManager_getChatSpMessageCheck, {uint32_t(*userId), msg})) return false;
    if (SceneManager::chatUserBlockCheck(self, msg + MSG_USER_ID)) return false;
    return true;
}

// Give the scroll view a copy of the rows (setMessages takes the vector by value).
void setScrollMessages(SceneManager *self, uint32_t rows, uint32_t copy) {
    uint32_t view = field(self, CHAT_SCROLL);
    call(RowVector_copy, {copy, rows});
    call(ChatScrollView_setMessages, {view, copy, field(self, CHAT_SCROLL_TO_ID)});
    call(RowVector_destroy, {copy});
}

// ChatManager::getMessage(100, MessageGetType 2, start, $_36{this, this}); the result says if it was sent
bool requestMessages(SceneManager *self, uint32_t cm, uint32_t start) {
    Frame f;
    uint32_t cb = newFunction(f, VT_ChatData_Messages, {mgrAddr(self), mgrAddr(self)});
    uint32_t sent = call(ChatManager_getMessage, {cm, 100, 2, start, cb});
    functionDestroy(cb);
    return sent != 0;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ polling

// Every frame. Chat closed: poll the user state every 60 s while the badge is hidden (the header shows); chat
// open: page the stamp list, rebuild the scroll view when asked, poll new messages every 15 s, or load the
// previous / next page of 50 when the scroll view reached an end.
void SceneManager::chatDataUpdate(SceneManager *self, float dt) {
    uint32_t m = mgrAddr(self);
    if (!self->chatPopupDisp) {
        if (!field(self, CHAT_BADGE) || !self->headerRequested) return;
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t ctl = ld32(api + API_USER_HEADER + 4), header = ld32(api + API_USER_HEADER);
        if (ctl) call(fn::shared_add, {ctl});
        uint8_t chatEnabled;
        {
            Frame f;
            chatEnabled = ld8(safeBytes(f, header, 16, 1));
        }
        uint32_t badge = field(self, CHAT_BADGE);
        if (chatEnabled) {
            if (!vcall(badge, node::isVisible, {badge})) {
                float t = rw::get<float>(self, CHAT_TIMER) - dt;
                rw::set<float>(self, CHAT_TIMER, t);
                if (t <= 0.0f) {
                    uint32_t cm = call(ChatManager_getInstance, {});
                    Frame f;
                    uint32_t cb = newFunction(f, VT_ChatData_IdleUser, {m});
                    call(ChatManager_getUser, {cm, cb});
                    functionDestroy(cb);
                    rw::set<float>(self, CHAT_TIMER, 60.0f);
                }
            }
        } else if (badge && vcall(badge, node::isVisible, {badge}) == 1) {
            vcall(field(self, CHAT_BADGE), node::setVisible, {field(self, CHAT_BADGE), 0});
        }
        if (ctl) call(fn::shared_release, {ctl});
        return;
    }

    // the stamp list pages by itself while migrating
    if (flag(self, STAMP_MIGRATING) && flag(self, STAMP_MIGRATE_RESULT)) {
        float t = rw::get<float>(self, STAMP_TIMER);
        if (!(t < 0.3f)) {
            int32_t page = int32_t(field(self, STAMP_PAGE));
            if (page <= 4) setField(self, STAMP_PAGE, uint32_t(++page));
            uint32_t list = field(self, STAMP_LIST);
            uint32_t moved = flag(self, STAMP_LEFT) ? call(BaseStageScrollView_migrationLeft, {list, uint32_t(page)})
                                                    : call(BaseStageScrollView_migrationRight, {list, uint32_t(page)});
            setFlag(self, STAMP_MIGRATE_RESULT, uint8_t(moved));
        } else {
            rw::set<float>(self, STAMP_TIMER, t + dt);
        }
    }

    if (flag(self, CHAT_RELOAD_VIEW) && !flag(self, CHAT_REQUESTING)) {
        // rebuild the scroll view from the last page, behind the loading indicator
        SceneManager *mgr = getInstance();
        {
            Frame f;
            uint32_t none = newEmptyFunction(f);
            mgr->loadingBlocked = 1;
            call(FunctionB_assign, {mgrAddr(mgr) + 0x548 /* onFinishLoading */, none});
            functionDestroy(none);
        }
        enableLoadingIndicator(getInstance(), 1);
        SceneManager *mgr2 = getInstance();
        uint32_t indicator = mgr2->loadingIndicator;
        if (vcall(indicator, node::getParent, {indicator})) call(LoadingIndicator_hide, {mgr2->loadingIndicator});
        setChatMessageScrollView(self);
        setFlag(self, CHAT_RELOAD_VIEW, 0);
        return;
    }

    float t = rw::get<float>(self, CHAT_TIMER) + dt;
    rw::set<float>(self, CHAT_TIMER, t);
    if (!(t >= 15.0f)) {
        // between polls: load another page when the view was scrolled to one of its ends
        if (flag(self, CHAT_REQUESTING) || flag(self, CHAT_LOADING_PAGE)) return;
        if (field(self, CHAT_TOTAL) < 51) return;
        uint32_t view = field(self, CHAT_SCROLL);
        if (!view) return;
        uint32_t index = call(IScrollProtocol_getScrollIndex, {view});
        if (index == 0xFFFFFFFFu && field(self, CHAT_START) >= 2) {
            // top: the previous (older) page
            setField(self, CHAT_SCROLL_TO_ID, call(ChatScrollView_getMessageId, {field(self, CHAT_SCROLL), 1}));
            uint32_t start = field(self, CHAT_START);
            setField(self, CHAT_START, start <= 50 ? 1 : start - 50);
        } else {
            if (index != 0xFFFFFFFFu && index < 51) return;
            uint32_t total = field(self, CHAT_TOTAL);
            if (total < 101) return;
            if (total == field(self, CHAT_START) + 50) return;
            // bottom: the next (newer) page
            int32_t at = 48;
            if (index == 0xFFFFFFFEu) at = int32_t(call(ChatScrollView_getMessageMaxNum, {field(self, CHAT_SCROLL)}) - 2);
            uint32_t id = call(ChatScrollView_getMessageId, {field(self, CHAT_SCROLL), uint32_t(at)});
            setField(self, CHAT_SCROLL_TO_ID, id);
            if (field(self, CHAT_LAST_SEEN_ID) < id) setField(self, CHAT_LAST_SEEN_ID, id);
            uint32_t old = field(self, CHAT_START);
            setField(self, CHAT_START, old + 50);
            total = field(self, CHAT_TOTAL);
            if (total <= old + 100) setField(self, CHAT_START, total - 50);
            if (total <= 50) setField(self, CHAT_START, 1);
        }
        setField(self, CHAT_42C, 0);
        setFlag(self, CHAT_LOADING_PAGE, 1);
        rw::set<float>(self, CHAT_TIMER, 15.0f);
        uint32_t cm = call(ChatManager_getInstance, {});
        if (requestMessages(self, cm, field(self, CHAT_START))) {
            setFlag(self, CHAT_REQUESTING, 1);
        } else if (flag(self, CHAT_LOADING_PAGE)) {
            enableLoadingIndicator(getInstance(), 1);
        }
        return;
    }

    // poll
    rw::set<uint32_t>(self, CHAT_TIMER, 0);
    uint8_t loadingPage = flag(self, CHAT_LOADING_PAGE);
    uint32_t cm = call(ChatManager_getInstance, {});
    if (loadingPage) {
        if (requestMessages(self, cm, field(self, CHAT_START))) {
            setField(self, CHAT_42C, 0);
            setFlag(self, CHAT_REQUESTING, 1);
        } else if (flag(self, CHAT_LOADING_PAGE)) {
            rw::set<float>(self, CHAT_TIMER, 16.0f);
        }
    } else {
        Frame f;
        uint32_t cb = newFunction(f, VT_ChatData_User, {m, m, m});
        call(ChatManager_getUser, {cm, cb});
        functionDestroy(cb);
    }
}
RW_FUNCTION(0x60E110, "SceneManager::chatDataUpdate", SceneManager::chatDataUpdate);

// ------------------------------------------------------------------------------------------------ rows

// Rebuild the scroll view from the last received page (CHAT_MESSAGES).
void SceneManager::setChatMessageScrollView(SceneManager *self) {
    // the original's frame: the rows copied to the heap carry the stack bytes after the stamp flag
    Frame frame;
    const uint32_t base = frame.allocExact(0x148);
    const uint32_t copy = base + 0x74, text = base + 0x80, msg = base + 0x8C, rows = base + 0xD8;
    const uint32_t row = base + 0xE8;
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctl = ld32(api + API_USER_STATUS + 4), status = ld32(api + API_USER_STATUS);
    if (ctl) call(fn::shared_add, {ctl});
    st32(rows, 0);
    st32(rows + 4, 0);
    st32(rows + 8, 0);
    setField(self, CHAT_ROW_COUNT, 0);
    uint32_t begin = field(self, CHAT_MESSAGES), end = field(self, CHAT_MESSAGES + 4);
    if (end != begin) {
        for (uint32_t i = 0;; i++) {
            if (((end - begin) >> 2) * 0x286BCA1Bu <= i) call(ThrowOutOfRange, {mgrAddr(self) + CHAT_MESSAGES});
            call(Message_copy, {msg, begin + MESSAGE_SIZE * i});
            int32_t userId;
            if (passesFilters(self, msg, &userId)) {
                uint32_t lastSeen = field(self, CHAT_LAST_SEEN_ID);
                uint32_t id = ld32(msg + MSG_ID);
                // `this` = the message id in the original
                uint32_t stamp = call(SceneManager_getChatStampMessageCheck, {id, msg + MSG_TEXT});
                int32_t level = chatLevel(status);
                uint32_t shown = (level > CHAT_MIN_LEVEL ? 1u : 0u) | stamp;
                if (shown) {
                    uint32_t stampId = 0;
                    if (stamp == 1) stampId = call(SceneManager_getChatStampId, {shown, msg + MSG_TEXT});
                    call(SceneManager_getStringHtmlEncodeUtf, {text, stamp, msg + MSG_TEXT});
                    pushRow(self, rows, row, msg, userId, stamp, stampId, id > lastSeen, text);
                }
            }
            messageFree(msg);
            begin = field(self, CHAT_MESSAGES);
            end = field(self, CHAT_MESSAGES + 4);
            if (i + 1 >= ((end - begin) >> 2) * 0x286BCA1Bu) break;
        }
    }
    setScrollMessages(self, rows, copy);
    setField(self, CHAT_SCROLL_TO_ID, 0);
    call(RowVector_destroy, {rows});
    if (ctl) call(fn::shared_release, {ctl});
}
RW_FUNCTION(0x617474, "SceneManager::setChatMessageScrollView()", SceneManager::setChatMessageScrollView);

// Keep a received page (CHAT_MESSAGES = messages) and show it. The first new message (id above the last seen)
// becomes the one the view scrolls to; a new message from a user the client does not know asks for a rebuild.
void SceneManager::setChatMessageScrollViewFrom(SceneManager *self, Addr messages) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x168);
    const uint32_t copy = base + 0x8C, text = base + 0x98, msg = base + 0xA4, rows = base + 0xF0;
    const uint32_t key = base + 0x100, row = base + 0x108;
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctl = ld32(api + API_USER_STATUS + 4), status = ld32(api + API_USER_STATUS);
    if (ctl) call(fn::shared_add, {ctl});
    bool scrollSet = false;
    st32(rows, 0);
    st32(rows + 4, 0);
    st32(rows + 8, 0);
    // clear the kept page
    const uint32_t kept = mgrAddr(self) + CHAT_MESSAGES;
    for (uint32_t e = ld32(kept + 4); e != ld32(kept); e = ld32(kept + 4)) {
        st32(kept + 4, e - MESSAGE_SIZE);
        messageFree(e - MESSAGE_SIZE);
    }
    setField(self, CHAT_ROW_COUNT, 0);
    if (ld32(messages + 4) != ld32(messages)) {
        for (uint32_t i = 0;; i++) {
            if (((ld32(messages + 4) - ld32(messages)) >> 2) * 0x286BCA1Bu <= i) call(ThrowOutOfRange, {messages});
            call(Message_copy, {msg, ld32(messages) + MESSAGE_SIZE * i});
            // kept.push_back(msg)
            uint32_t end = ld32(kept + 4);
            if (end == ld32(kept + 8)) {
                call(MessageVector_pushSlow, {kept, msg});
            } else {
                uint32_t at = 0;
                if (end) {
                    call(Message_copy, {end, msg});
                    at = ld32(kept + 4);
                }
                st32(kept + 4, at + MESSAGE_SIZE);
            }
            int32_t userId;
            if (passesFilters(self, msg, &userId)) {
                uint32_t lastSeen = field(self, CHAT_LAST_SEEN_ID);
                uint32_t id = ld32(msg + MSG_ID);
                uint32_t stamp = call(SceneManager_getChatStampMessageCheck, {id, msg + MSG_TEXT});
                int32_t level = chatLevel(status);
                if ((level > CHAT_MIN_LEVEL ? 1u : 0u) | stamp) {
                    uint32_t stampId = 0;
                    if (stamp == 1) stampId = call(SceneManager_getChatStampId, {stamp, msg + MSG_TEXT});
                    if (id > lastSeen && (field(self, CHAT_SCROLL_TO_ID) | uint32_t(scrollSet)) == 0) {
                        uint32_t first = ld32(msg + MSG_ID);
                        setField(self, CHAT_SCROLL_TO_ID, first);
                        if (first) scrollSet = true;
                    }
                    // a new message from a user missing in the user map: rebuild later
                    uint32_t api2 = call(fn::APIManager_getInstance, {});
                    uint32_t mapCtl = ld32(api2 + API_USER_MAP + 4), users = ld32(api2 + API_USER_MAP);
                    if (mapCtl) call(fn::shared_add, {mapCtl});
                    if (users) {
                        st32(key, uint32_t(userId));
                        st32(key + 4, uint32_t(userId >> 31));
                        uint32_t node = call(UserMap_find, {users + 0x24, key});
                        int32_t slot = node ? int32_t(ld32(node + 0x10)) : -1;
                        bool old = !(id > lastSeen);
                        if (slot < 0 && !old) setFlag(self, CHAT_RELOAD_VIEW, 1);
                    }
                    call(SceneManager_getStringHtmlEncodeUtf, {text, 0, msg + MSG_TEXT});
                    pushRow(self, rows, row, msg, userId, stamp, stampId, id > lastSeen, text);
                    if (mapCtl) call(fn::shared_release, {mapCtl});
                }
            }
            messageFree(msg);
            if (i + 1 >= ((ld32(messages + 4) - ld32(messages)) >> 2) * 0x286BCA1Bu) break;
        }
    }
    setScrollMessages(self, rows, copy);
    if (!flag(self, CHAT_RELOAD_VIEW)) setField(self, CHAT_SCROLL_TO_ID, 0);
    call(RowVector_destroy, {rows});
    if (ctl) call(fn::shared_release, {ctl});
}
RW_FUNCTION(0x616DC8,
            "SceneManager::setChatMessageScrollView(const std::vector<ChatManager::Message>&)",
            SceneManager::setChatMessageScrollViewFrom);

// True when the page holds a message the chat would show that is newer than the last seen one.
bool SceneManager::setChatMessageUpdate(SceneManager *self, Addr messages) {
    Frame frame;
    const uint32_t msg = frame.alloc(MESSAGE_SIZE);
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctl = ld32(api + API_USER_STATUS + 4), status = ld32(api + API_USER_STATUS);
    if (ctl) call(fn::shared_add, {ctl});
    bool found = false;
    if (ld32(messages + 4) != ld32(messages)) {
        for (uint32_t i = 0;; i++) {
            if (((ld32(messages + 4) - ld32(messages)) >> 2) * 0x286BCA1Bu <= i) call(ThrowOutOfRange, {messages});
            call(Message_copy, {msg, ld32(messages) + MESSAGE_SIZE * i});
            bool isNew = false;
            int32_t userId;
            if (passesFilters(self, msg, &userId)) {
                uint32_t lastSeen = field(self, CHAT_LAST_SEEN_ID);
                uint32_t id = ld32(msg + MSG_ID);
                uint32_t stamp = call(SceneManager_getChatStampMessageCheck, {id, msg + MSG_TEXT});
                int32_t level = chatLevel(status);
                if ((stamp | (level > CHAT_MIN_LEVEL ? 1u : 0u)) && id > lastSeen) isNew = true;
            }
            messageFree(msg);
            if (isNew) {
                found = true;
                break;
            }
            if (i + 1 >= ((ld32(messages + 4) - ld32(messages)) >> 2) * 0x286BCA1Bu) break;
        }
    }
    if (ctl) call(fn::shared_release, {ctl});
    return found;
}
RW_FUNCTION(0x617970, "SceneManager::setChatMessageUpdate", SceneManager::setChatMessageUpdate);

// ------------------------------------------------------------------------------------------------ text

// Size the chat input background (Chat_Win) and its Panel for `lines` lines of 26 points.
void SceneManager::chatWindowUpdate(SceneManager *self, int32_t lines) {
    Frame f;
    uint32_t size = f.alloc(8);
    float extra = float(int32_t(uint32_t(lines) - 1u)) * 26.0f;
    uint32_t win = findNode(field(self, CHAT_LAYOUT), STR_ChatWin, 8);
    call(ImageView_getCapInsets, {win});
    call(Size_copy, {size, call(Widget_getCustomSize, {win})});
    stf(size + 4, extra + 52.0f);
    vcall(win, WIDGET_SET_CONTENT_SIZE, {win, size});
    uint32_t panel = findNode(field(self, CHAT_LAYOUT), STR_Panel, 5);
    call(ImageView_getCapInsets, {panel});
    call(Size_assign, {size, call(Widget_getCustomSize, {panel})});
    stf(size + 4, extra + 65.0f);
    vcall(panel, WIDGET_SET_CONTENT_SIZE, {panel, size});
}
RW_FUNCTION(0x615310, "SceneManager::chatWindowUpdate", SceneManager::chatWindowUpdate);

// `text` wrapped for the chat input: a line breaks after `maxWidth` width units (a multi-byte character counts
// 2), at most `maxLines` lines. out: the returned std::string.
void SceneManager::chatNewLineUpdate(Addr out, SceneManager *self, Addr text, int32_t maxWidth, int32_t maxLines) {
    (void)self;
    strInit(out, STR_Empty, 0);
    uint32_t n = strSize(text);
    if (!n) return;
    Frame f;
    uint32_t next = f.alloc(12), ch = f.alloc(12);
    uint32_t pos = 0;
    int32_t width = 0, line = 0;
    for (;;) {
        uint32_t len = utf8Length(strByte(text, pos));
        call(fn::string_substr_init, {ch, text, pos, len, text});
        int32_t newLine;
        if (call(StringCompare, {ch, STR_NewLine})) {
            call(fn::string_append, {out, strData(ch), strSize(ch)});
            width += len > 1 ? 2 : 1;
            newLine = line;
            if (width > maxWidth) {
                // wrap: look at the next character
                uint32_t nextLen = utf8Length(strByte(text, len + pos));
                newLine = line + 1;
                call(fn::string_substr_init, {next, text, len + pos, nextLen, text});
                width = 0;
                strMoveAssign(ch, next);
                if (newLine < maxLines) {
                    if (call(StringCompare, {ch, STR_NewLine})) {
                        call(fn::string_append, {out, STR_NewLine, 1});
                    } else {
                        // the next character is a line break itself: it starts the new line
                        if (call(StringCompare, {ch, STR_NewLine})) line = newLine;
                        newLine = line;
                    }
                }
            }
        } else {
            newLine = line + 1;
            width = 0;
            if (newLine < maxLines) call(fn::string_append, {out, strData(ch), strSize(ch)});
        }
        strFree(ch);
        pos += len;
        if (newLine >= maxLines || pos >= n) return;
        line = newLine;
    }
}
RW_FUNCTION(0x615498, "SceneManager::chatNewLineUpdate", SceneManager::chatNewLineUpdate);

// The first `maxChars` + 1 characters (UTF-8) of `text`. out: the returned std::string.
void SceneManager::chatLineUpdate(Addr out, SceneManager *self, Addr text, int32_t maxChars) {
    (void)self;
    strInit(out, STR_Empty, 0);
    uint32_t n = strSize(text);
    if (!n) return;
    Frame f;
    uint32_t ch = f.alloc(12);
    uint32_t pos = 0;
    for (int32_t count = 1;; count++) {
        uint32_t len = utf8Length(strByte(text, pos));
        call(fn::string_substr_init, {ch, text, pos, len, text});
        call(fn::string_append, {out, strData(ch), strSize(ch)});
        strFree(ch);
        pos += len;
        if (count >= maxChars || pos >= n) return;
    }
}
RW_FUNCTION(0x615728, "SceneManager::chatLineUpdate", SceneManager::chatLineUpdate);

// The user (id string) is blocked: on the server (CHAT_BLOCKS) or on this device (CHAT_BLOCKED_IDS).
bool SceneManager::chatUserBlockCheck(SceneManager *self, Addr userId) {
    Frame f;
    uint32_t block = f.alloc(BLOCK_SIZE);
    uint32_t begin = field(self, CHAT_BLOCKS), end = field(self, CHAT_BLOCKS + 4);
    if (end != begin) {
        for (uint32_t i = 0;; i++) {
            if (((end - begin) >> 2) * 0xF0F0F0F1u <= i) call(ThrowOutOfRange, {mgrAddr(self) + CHAT_BLOCKS});
            call(Block_copy, {block, begin + BLOCK_SIZE * i});
            bool match = strEq(block + BLOCK_USER_ID, userId);
            strFree(block + 0x30);
            strFree(block + 0x24);
            strFree(block + 0x18);
            strFree(block + 0x0C);
            strFree(block + 0x00);
            if (match) return true;
            begin = field(self, CHAT_BLOCKS);
            end = field(self, CHAT_BLOCKS + 4);
            if (i + 1 >= ((end - begin) >> 2) * 0xF0F0F0F1u) break;
        }
    }
    uint32_t ids = field(self, CHAT_BLOCKED_IDS);
    if (field(self, CHAT_BLOCKED_IDS + 4) == ids) return false;
    uint32_t id = f.alloc(12);
    for (uint32_t i = 0;; i++) {
        strCopy(id, ids + 12 * i);
        bool match = strEq(id, userId);
        strFree(id);
        if (match) return true;
        ids = field(self, CHAT_BLOCKED_IDS);
        if (i + 1 >= ((field(self, CHAT_BLOCKED_IDS + 4) - ids) >> 2) * 0xAAAAAAABu) return false;
    }
}
RW_FUNCTION(0x615878, "SceneManager::chatUserBlockCheck", SceneManager::chatUserBlockCheck);

// A system message not shown as chat: "<C>", "<U>", "<E:user id>" or "<L:user id>" (the message's own user).
bool SceneManager::getChatSpMessageCheck(SceneManager *self, Addr message) {
    (void)self;
    uint32_t text = message + MSG_TEXT;
    Frame f;
    uint32_t tag = f.alloc(12);
    strInit(tag, STR_TagC, 3);
    if (strEq(text, tag)) {
        strFree(tag);
        return true;
    }
    call(fn::string_assign, {tag, STR_TagU, 3});
    if (strEq(text, tag)) {
        strFree(tag);
        return true;
    }
    uint32_t gt = f.alloc(12);
    strInit(gt, STR_Gt, 1);
    call(fn::string_assign, {tag, STR_TagE, 3});
    call(fn::string_append, {tag, strData(message + MSG_USER_ID), strSize(message + MSG_USER_ID)});
    call(fn::string_append, {tag, strData(gt), strSize(gt)});
    bool special = strEq(text, tag);
    if (!special) {
        call(fn::string_assign, {tag, STR_TagL, 3});
        call(fn::string_append, {tag, strData(message + MSG_USER_ID), strSize(message + MSG_USER_ID)});
        call(fn::string_append, {tag, strData(gt), strSize(gt)});
        special = strEq(text, tag);
    }
    strFree(gt);
    strFree(tag);
    return special;
}
RW_FUNCTION(0x615B18, "SceneManager::getChatSpMessageCheck", SceneManager::getChatSpMessageCheck);

// A stamp message: "ISTAMP:MEDAL:<id>" or "ISTAMP:TALKS:<id>" (the prefix once, no space).
bool SceneManager::getChatStampMessageCheck(SceneManager *self, Addr text) {
    (void)self;
    if (strSize(text) < 13) return false;
    Frame f;
    uint32_t medal = f.alloc(12), talks = f.alloc(12), space = f.alloc(12);
    strInit(medal, STR_StampMedal, 13);
    strInit(talks, STR_StampTalks, 13);
    strInit(space, STR_Space, 1);
    auto isStamp = [&](uint32_t prefix) {
        return strFind(text, prefix, 0) == 0 && strFind(text, prefix, 13) == 0xFFFFFFFFu &&
               strFind(text, space, 0) == 0xFFFFFFFFu;
    };
    bool stamp = isStamp(medal) || isStamp(talks);
    strFree(space);
    strFree(talks);
    strFree(medal);
    return stamp;
}
RW_FUNCTION(0x615EA4, "SceneManager::getChatStampMessageCheck", SceneManager::getChatStampMessageCheck);

// The stamp id of a stamp message (the number after "ISTAMP:MEDAL:" / "ISTAMP:TALKS:"), 0 otherwise.
int32_t SceneManager::getChatStampId(SceneManager *self, Addr text) {
    (void)self;
    Frame f;
    uint32_t rest = f.alloc(12), tail = f.alloc(12);
    call(fn::string_substr_init, {rest, text, 7, 0xFFFFFFFFu, text});     // after "ISTAMP:"
    int32_t id = 0;
    // find("MEDAL:") / find("TALKS:") on the rest (a const char* needle of 6)
    auto contains = [&](uint32_t literal) {
        uint32_t n = strSize(rest);
        if (n < 6) return false;
        const uint8_t *d = rw::host_ptr<uint8_t>(strData(rest));
        const uint8_t *k = rw::host_ptr<uint8_t>(literal);
        for (uint32_t p = 0; p + 6 <= n; p++)
            if (std::memcmp(d + p, k, 6) == 0) return true;
        return false;
    };
    if (contains(STR_Medal) || contains(STR_Talks)) {
        call(fn::string_substr_init, {tail, rest, 6, 0xFFFFFFFFu, rest});
        strMoveAssign(rest, tail);
        id = int32_t(call(Stoi, {rest, 0, 10}));
    }
    strFree(rest);
    return id;
}
RW_FUNCTION(0x616248, "SceneManager::getChatStampId", SceneManager::getChatStampId);

// `text` with the HTML entities the chat server sends decoded (&quot; &amp; &#39; &lt; &gt; &#96; &#123;
// &#125;), each replaced everywhere in turn. out: the returned std::string.
void SceneManager::getStringHtmlEncodeUtf(Addr out, SceneManager *self, Addr text) {
    (void)self;
    static constexpr struct { uint32_t entity, entityLen, plain; } TABLE[] = {
        {0xE03352, 6, 0xE03383}, {0xE03359, 5, 0xE03385}, {0xE0335F, 5, 0xE03387}, {0xE03365, 4, 0xE03389},
        {0xE0336A, 4, 0xE03325}, {0xE0336F, 5, 0xE0338B}, {0xE03375, 6, 0xE0338D}, {0xE0337C, 6, 0xE0338F},
    };
    strCopy(out, text);
    Frame f;
    uint32_t entities = f.alloc(12), plains = f.alloc(12), tmp = f.alloc(12), entity = f.alloc(12);
    // vector<string>::push_back(string&&), inlined
    auto push = [&](uint32_t vec, uint32_t literal, uint32_t len) {
        strInit(tmp, literal, len);
        uint32_t end = ld32(vec + 4);
        if (end >= ld32(vec + 8)) {
            call(StringVector_pushSlow, {vec, tmp});
        } else {
            uint32_t at = 0;
            if (end) {
                st32(end, ld32(tmp));
                st32(end + 4, ld32(tmp + 4));
                st32(end + 8, ld32(tmp + 8));
                st32(tmp, 0);
                st32(tmp + 4, 0);
                st32(tmp + 8, 0);
                at = ld32(vec + 4);
            }
            st32(vec + 4, at + 12);
        }
        strFree(tmp);
    };
    st32(entities, 0);
    st32(entities + 4, 0);
    st32(entities + 8, 0);
    for (const auto &e : TABLE) push(entities, e.entity, e.entityLen);
    st32(plains, 0);
    st32(plains + 4, 0);
    st32(plains + 8, 0);
    for (const auto &e : TABLE) push(plains, e.plain, 1);
    if (ld32(entities + 4) != ld32(entities)) {
        for (uint32_t i = 0;; i++) {
            strCopy(entity, ld32(entities) + 12 * i);
            for (uint32_t pos = strFind(out, entity, 0); pos != 0xFFFFFFFFu; pos = strFind(out, entity, 0)) {
                if (((ld32(plains + 4) - ld32(plains)) >> 2) * 0xAAAAAAABu <= i) call(ThrowOutOfRange, {plains});
                uint32_t plain = ld32(plains) + 12 * i;
                call(StringReplace, {out, pos, strSize(entity), strData(plain), strSize(plain)});
            }
            strFree(entity);
            if (i + 1 >= ((ld32(entities + 4) - ld32(entities)) >> 2) * 0xAAAAAAABu) break;
        }
    }
    call(StringVector_destroy, {plains});
    call(StringVector_destroy, {entities});
}
RW_FUNCTION(0x616438, "SceneManager::getStringHtmlEncodeUtf", SceneManager::getStringHtmlEncodeUtf);
