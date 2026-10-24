// SceneManager::openChatPopup's fetch chain: $_43 (sub_61507C, called directly on the 0x50-byte capture block)
// requests the chat state (executeUserChatGet), its $_129 (__clone sub_6384E8, operator() sub_638558) asks
// ChatManager for the user, and $_130's operator() (sub_638804) stores the read / unread counts and the blocked
// users in SceneManager. Players below rank 13 get the chat button disabled.
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;
constexpr uint32_t Widget_isBright = rwlib::cocos2d::ui::Widget::isBright;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;   // (Button*, Text*, bool, bool)
constexpr uint32_t APIManager_executeUserChatGet = 0x4C6871;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;   // (api, const function<void(bool)>&, bool)
constexpr uint32_t ChatManager_getInstance = 0x4DB0E5;
constexpr uint32_t ChatManager_getUser = 0x4DB271;          // (cm, const function&)
constexpr uint32_t LoadingIndicator_hide = 0x509D69;
constexpr uint32_t ChatPopup_showError = 0x6317FD;          // sub_6317FC(error text target, const string&)
constexpr uint32_t Block_copy = 0x4DEB5F;
constexpr uint32_t BlockVector_pushSlow = 0x4DE87D;
constexpr uint32_t BoolVector_pushBack = 0x62161D;          // vector<bool>::push_back(const bool&)
constexpr uint32_t MEMCPY = 0x40E164;

constexpr uint32_t VT_GetUser = 0xF60978;      // $_129 (heap 0x48): the 0x44 capture words after the first three
constexpr uint32_t VT_GotUser = 0xF609A4;      // $_130 (heap 0x28)
constexpr uint32_t VT_ErrorClosed = 0xF609D0;  // $_129's error dialog lambda

constexpr uint32_t S_ChatWin = 0xE03314;
constexpr int32_t TXT_CHAT_ERROR = 104200028, TXT_CHAT_ERROR_1 = 105000001, TXT_CHAT_ERROR_2 = 105000002;
constexpr int32_t CHAT_MIN_RANK = 13;

constexpr uint32_t API_USER_SAFE = 0x1D4;      // APIManager: SafeBuffer* (+0: the player's rank) and its control
constexpr uint32_t SM_CHAT_CLOSE = 0x4B0;      // SceneManager: chat popup node
constexpr uint32_t SM_HEADER = 0x400;          // the header layout (Chat_Win)
constexpr uint32_t SM_LOADING = 0x4C8;         // LoadingIndicator*
constexpr uint32_t SM_CHAT_READ = 0x408, SM_CHAT_FIRST = 0x40C, SM_CHAT_READ2 = 0x410, SM_CHAT_READ3 = 0x414,
                   SM_CHAT_UNREAD = 0x41C;
constexpr uint32_t SM_BLOCKS = 0x454;          // vector<ChatManager::Block>
constexpr uint32_t SM_BLOCKS_SHOWN = 0x460;    // vector<bool>
constexpr uint32_t SM_CHAT_READY = 0x46C, SM_CHAT_READY2 = 0x46E, SM_CHAT_INTERVAL = 0x474;
constexpr uint32_t BLOCK_SIZE = 0x44;

void showError(uint32_t target, int32_t id) {
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t s = newStr(f, textDataStr(td));
    call(ChatPopup_showError, {target, s});
    strFree(s);
    sharedRelease(td);
}
}  // namespace

namespace chat_fetch {

// $_43 on the captures {button, text, SceneManager, ...}
void fetch(uint32_t caps) {
    const uint32_t sm = ld32(caps + 8);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_USER_SAFE + 4);
    const uint32_t user = ld32(api + API_USER_SAFE);
    if (ctrl) call(fn::shared_add, {ctrl});
    if (safe32(user, 0) <= CHAT_MIN_RANK - 1) {
        call(Widget_setBright, {ld32(caps), 0});
        const uint32_t button = ld32(caps), text = ld32(caps + 4);
        call(SceneUtil_setButtonEnabled, {button, text, call(Widget_isBright, {button}), 0});
        vcall(ld32(caps), node::setVisible, {ld32(caps), 0});
        const uint32_t popup = ld32(sm + SM_CHAT_CLOSE);
        vcall(popup, node::removeFromParentAndCleanup, {popup, 1});
        const uint32_t win = findNode(ld32(sm + SM_HEADER), S_ChatWin, 8);
        vcall(win, node::setVisible, {win, 0});
    }
    const uint32_t api2 = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t copy = f.alloc(0x44);
    call(MEMCPY, {copy, caps + 0xC, 0x44});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x48});
    if (heap) {
        st32(heap, VT_GetUser);
        call(MEMCPY, {heap + 4, copy, 0x44});
    }
    st32(fo + 16, heap);
    call(APIManager_executeUserChatGet, {api2, fo});
    functionDestroy(fo);
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x61507C, "SceneManager::openChatPopup $_43::operator() (sub_61507C)", chat_fetch::fetch);

// $_129 __func::__clone()
uint32_t cloneGetUser(uint32_t self) {
    const uint32_t p = call(fn::operator_new, {0x48});
    if (p) {
        st32(p, VT_GetUser);
        call(MEMCPY, {p + 4, self + 4, 0x44});
    }
    return p;
}
RW_FUNCTION(0x6384E8, "SceneManager::openChatPopup $_129 __func::__clone() (sub_6384E8)", chat_fetch::cloneGetUser);

// $_129(bool ok): ask ChatManager for the user ($_130), else hide the indicator and show the error dialog.
void getUser(uint32_t self, uint32_t ok) {
    if (ld8(ok)) {
        const uint32_t cm = call(ChatManager_getInstance, {});
        Frame f;
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x28});
        if (heap) {
            st32(heap, VT_GotUser);
            for (uint32_t k = 4; k < 0x28; k += 4) st32(heap + k, ld32(self + k));
        }
        st32(fo + 16, heap);
        const uint32_t requested = call(ChatManager_getUser, {cm, fo});
        functionDestroy(fo);
        if (!requested) showError(self + 0x28, TXT_CHAT_ERROR);
        return;
    }
    const uint32_t sm = call(fn::SceneManager_getInstance, {});
    const uint32_t indicator = ld32(sm + SM_LOADING);
    if (vcall(indicator, node::getParent, {indicator})) call(LoadingIndicator_hide, {ld32(sm + SM_LOADING)});
    if (ld8(call(fn::APIManager_getInstance, {}) + 0x14)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t cb = newFunction(f, VT_ErrorClosed, {});
        call(APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
    }
}
RW_FUNCTION(0x638558, "SceneManager::openChatPopup $_129::operator()(bool) (sub_638558)", chat_fetch::getUser);

// $_130(status, read, unread, blocks): status 0 stores the counts and the blocked users; 1 / 2 / other show errors;
// 3 (throttled) does nothing.
void gotUser(uint32_t self, uint32_t statusPtr, uint32_t readPtr, uint32_t unreadPtr, uint32_t blocks) {
    const uint32_t status = ld32(statusPtr);
    if (status == 3) return;
    if (status == 1) {
        showError(self + 8, TXT_CHAT_ERROR_1);
        return;
    }
    if (status != 0) {
        showError(self + 8, status == 2 ? TXT_CHAT_ERROR_2 : TXT_CHAT_ERROR);
        return;
    }
    const uint32_t unread = ld32(unreadPtr);
    const uint32_t read = ld32(readPtr);
    const uint32_t sm = ld32(self + 4);
    st32(sm + SM_CHAT_READ, read);
    st32(sm + SM_CHAT_READ2, read);
    st32(sm + SM_CHAT_FIRST, read > 50 ? read - 50 : 1);
    st32(sm + SM_CHAT_UNREAD, unread);
    st32(sm + SM_CHAT_READ3, read);
    // clear the blocked users
    while (ld32(sm + SM_BLOCKS + 4) != ld32(sm + SM_BLOCKS)) {
        const uint32_t block = ld32(sm + SM_BLOCKS + 4) - BLOCK_SIZE;
        st32(sm + SM_BLOCKS + 4, block);
        for (uint32_t o : {0x30u, 0x24u, 0x18u, 0xCu, 0u}) strFree(block + o);
    }
    st32(sm + SM_BLOCKS_SHOWN + 4, 0);
    const uint32_t n = (ld32(blocks + 4) - ld32(blocks)) / BLOCK_SIZE;
    for (uint32_t i = 0; i < n; i++) {
        const uint32_t src = ld32(blocks) + BLOCK_SIZE * i;
        const uint32_t end = ld32(sm + SM_BLOCKS + 4);
        if (end == ld32(sm + SM_BLOCKS + 8)) {
            call(BlockVector_pushSlow, {sm + SM_BLOCKS, src});
        } else {
            if (end) call(Block_copy, {end, src});
            st32(sm + SM_BLOCKS + 4, ld32(sm + SM_BLOCKS + 4) + BLOCK_SIZE);
        }
        Frame f;
        const uint32_t yes = f.alloc(4);
        st8(yes, 1);
        call(BoolVector_pushBack, {sm + SM_BLOCKS_SHOWN, yes});
    }
    st8(sm + SM_CHAT_READY, 1);
    st8(sm + SM_CHAT_READY2, 1);
    stf(sm + SM_CHAT_INTERVAL, 15.0f);
}
RW_FUNCTION(0x638804, "SceneManager::openChatPopup $_130::operator() (sub_638804)", chat_fetch::gotUser);

}  // namespace chat_fetch
