// ChatManager::getUser's response lambda (batch 3, named from its __func vtable's target_type): the chat user
// state - the blocked users and this user's read / unread counts - handed to the caller's callback.
// `functor`: vtable, ?, the ChatManager action words (+8, +0xC), the callback std::function<void(StatusCode,
// unsigned, unsigned, const std::vector<Block>&)> at +0x10.
#include "SceneUi.hpp"

using namespace scene;

namespace {

constexpr uint32_t ChatManager_updateInterval = 0x4DB225;    // (action words, const Document&)
constexpr uint32_t Json_member = 0x4B1BB5;                   // rapidjson GenericValue::operator[](const char*)
constexpr uint32_t Block_init = 0x4DAE71;                    // ChatManager::Block::init(block, const Value&)
constexpr uint32_t Block_copy = 0x4DEB5F;                    // ChatManager::Block(const Block&)
constexpr uint32_t BlockVector_pushSlow = 0x4DE87D;          // vector<Block>::__push_back_slow_path(const Block&)
constexpr uint32_t BlockVector_dtor = 0x4DB601;              // __vector_base<Block>::~__vector_base
constexpr uint32_t Callback_call = 0x4DB575;                 // the callback's operator()(code, a, b, blocks)
constexpr uint32_t STRTOUMAX = 0x413F90;                     // (const char*, char**, base) (import)
constexpr uint32_t aeabi_memset = 0x40E7A0;                  // (dst, n, c)
constexpr uint32_t S_CHAT_ENTRY = 0xDFC2A9, S_BLOCK_ENTRY = 0xDFC2C3, S_CHAT_ID = 0xDFB0E2;
constexpr uint32_t S_READ_NUMBER = 0xDFC1F6, S_UNREAD_COUNT = 0xDFC2B3;
constexpr uint32_t API_USER = 0x84;                          // APIManager: the user (+0x7C: the chat id)

// rapidjson value: data +0 (string pointer / int), size +4, flags +0xC (type in the low byte)
constexpr uint32_t J_FLAGS = 0xC, J_STRING = 0x100000, J_UINT = 0x2000;
constexpr uint32_t T_OBJECT = 3, T_ARRAY = 4;
constexpr uint32_t BLOCK_SIZE = 0x44;

uint32_t member(uint32_t value, uint32_t name) { return call(Json_member, {value, name}); }

// an unsigned number that may come as a string; false when it is neither
bool readUnsigned(uint32_t value, uint32_t &out) {
    const uint32_t flags = ld32(value + J_FLAGS);
    if (flags & J_STRING) {
        out = call(STRTOUMAX, {ld32(value), 0, 10});
        return true;
    }
    if (flags & J_UINT) {
        out = ld32(value);
        return true;
    }
    return false;
}

void destroyBlockStrings(uint32_t block) {
    for (uint32_t o : {0x30u, 0x24u, 0x18u, 0xCu, 0u}) strFree(block + o);
}

}  // namespace

struct ChatManagerBatch3 {
    static void userGot(uint32_t functor, uint32_t responsePtr) {
        const uint32_t response = ld32(responsePtr);
        const uint32_t doc = response + 8;
        call(ChatManager_updateInterval, {ld32(functor + 8), ld32(functor + 0xC), doc});
        Frame f;
        const uint32_t blocks = f.alloc(12);
        st32(blocks, 0);
        st32(blocks + 4, 0);
        st32(blocks + 8, 0);
        // code 2 (and leftover register values for the counts) unless the whole parse succeeds
        uint32_t code = 2, read = response, unread = functor;
        do {
            if (ld32(response + 0x14) != 3) break;
            const uint32_t chats = member(doc, S_CHAT_ENTRY);
            if (ld32(chats + J_FLAGS) != T_ARRAY) break;
            const uint32_t blockList = member(doc, S_BLOCK_ENTRY);
            if (ld32(blockList + J_FLAGS) != T_ARRAY) break;
            read = blocks;
            // the blocked users
            const uint32_t n = ld32(blockList + 4);
            bool ok = true;
            for (uint32_t i = 0; i < n && ok; i++) {
                const uint32_t elem = ld32(blockList) + i * 16;
                if (ld32(elem + J_FLAGS) != T_OBJECT) {
                    ok = false;
                    break;
                }
                const uint32_t block = f.alloc(BLOCK_SIZE);
                call(aeabi_memset, {block, 0x3C, 0});
                st8(block + 0x43, uint8_t(ld8(block + 0x43) & 0xFD));
                if (!call(Block_init, {block, elem})) {
                    ok = false;
                } else {
                    const uint32_t end = ld32(blocks + 4);
                    if (end != ld32(blocks + 8)) {
                        if (end) call(Block_copy, {end, block});
                        st32(blocks + 4, ld32(blocks + 4) + BLOCK_SIZE);
                    } else {
                        call(BlockVector_pushSlow, {blocks, block});
                    }
                }
                destroyBlockStrings(block);
            }
            if (!ok) break;
            // this user's chat entry: the read message number and the unread count
            const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
            const uint32_t m = ld32(chats + 4);
            for (uint32_t i = 0; i < m; i++) {
                const uint32_t entry = ld32(chats) + i * 16;
                uint32_t id;
                if (!readUnsigned(member(entry, S_CHAT_ID), id)) break;
                if (id != ld32(user + 0x7C)) continue;
                if (readUnsigned(member(entry, S_READ_NUMBER), read) &&
                    readUnsigned(member(entry, S_UNREAD_COUNT), unread))
                    code = 0;
                break;
            }
        } while (false);
        // the response status (1 / 2) overrides the parse result
        const uint32_t status = ld32(response);
        uint32_t override = 0;
        if (status == 1) override = 1;
        if (status == 2) override = 2;
        if (ld32(functor + 0x20)) call(Callback_call, {functor + 0x10, override ? override : code, read, unread, blocks});
        call(BlockVector_dtor, {blocks});
    }
};

RW_FUNCTION(0x4DE4C4, "ChatManager::getUser::$_N::operator()", ChatManagerBatch3::userGot);
