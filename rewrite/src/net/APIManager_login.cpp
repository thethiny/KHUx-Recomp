// APIManager: the login request and the login chain (user/start, chat, party, stone ...). The chain requests
// skip the busy guard while APIManager::loginChain is set: they continue the running connection.
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t POOL_MALLOC = 0x49A2A3;            // rapidjson::MemoryPoolAllocator::Malloc(size_t)
constexpr uint32_t VFS_READ_DATA = 0x8DD46D;          // bg::vfs::readData(const char*) -> shared_ptr<Data>
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;      // StringBuffer::StringBuffer(const char* fmt, ...)
constexpr uint32_t MD5_CALCULATE = 0x4E0A89;          // md5::calculate(const void*, size_t) -> std::string (hex)
constexpr uint32_t CIPHER_CTOR = 0x4E0671;            // Cipher::Cipher(const std::array<uint8_t, 32>&)
constexpr uint32_t CIPHER_ENCRYPT = 0x4C678D;         // (Cipher&, const void*, size_t) -> std::string (encrypted)
constexpr uint32_t FILEUTILS_GET_INSTANCE = rwlib::cocos2d::FileUtils::getInstance; // cocos2d::FileUtils::getInstance
constexpr uint32_t DATE_MAKE_UTC = 0x49283D;          // Date::makeUTCDate() const
constexpr uint32_t STRING_SET_DTOR = 0x4D6DD1;        // std::unordered_set<std::string>::~ (__hash_table dtor)
constexpr uint32_t SAFEBUFFER_DTOR = 0x45A13D;

constexpr uint32_t STRINGBUFFER_LENGTH = 0x400;       // StringBuffer: 1024 chars, then the length

// strings in the image
constexpr uint32_t S_LENGTH = 0xDFBDC7;               // "length"
constexpr uint32_t S_DIGEST = 0xDFBDCE;               // "digest"
constexpr uint32_t S_CONFIG_FILE = 0xDFBDFC;          // "config/szWbU7hH"
constexpr uint32_t S_VERSION_FILE = 0xDFBAE0;         // "info/version"
constexpr uint32_t S_KEY_FORMAT = 0xDFBE0C;           // "%s%llu%s"
constexpr uint32_t S_UINT_FORMAT = 0xDFBE15;          // "%u"
constexpr uint32_t S_HOME = 0xDFBE18;                 // "home"
constexpr uint32_t S_COMMENT = 0xDFB036;              // "comment"
constexpr uint32_t S_GROUPS = 0xDFBE1D;               // "groups"
constexpr uint32_t S_DATE_FORMAT = 0xDFBE24;          // "%04d-%02d-%02d %02d:%02d:%02d"
constexpr uint32_t S_SHELL = 0xDFBE42;                // "shell"
constexpr uint32_t S_PARTY_ID = 0xDFAFD5;             // "partyId"
constexpr uint32_t S_GET_DETAIL = 0xDFBBD4;           // "getDetail"
constexpr uint32_t S_PLATFORM_TYPE = 0xDFA83B;        // "platformType"

constexpr uint32_t APPSTATE_ACCOUNT = 0x28;           // AppState: SafeBuffer* {u32 uuid length, 32-byte digest, Date}

void copyBytes(uint32_t dst, uint32_t src, uint32_t n) {
    for (uint32_t k = 0; k < n; k++) rw::store8(rw::host_ptr<uint8_t>(dst + k), load8(src + k));
}

}  // namespace

// ---------------------------------------------------------------------------------------------- login
// login: {"length": uuid length, "digest": the 32 digest bytes}, both from AppState's account SafeBuffer.
bool APIManager::executeLogin(APIManager *self, uint32_t cb) {
    Frame F(0xB0);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    uint32_t doc = newParams(F, 0x2C);
    uint32_t alloc = allocatorOf(doc);
    uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
    addU32(F, doc, S_LENGTH, sbWord(load32(app + APPSTATE_ACCOUNT), 0), alloc);
    // the digest as a copied string (kCopyStringFlag) in the document's pool
    uint32_t account = load32(app + APPSTATE_ACCOUNT);
    F.setWord(0x44, JSON_COPY_STRING);
    uint32_t chars = F.call(POOL_MALLOC, {alloc, 0x21});
    F.setWord(0x38, chars);
    F.setWord(0x3C, 0x20);
    for (uint32_t k = 0; k < 0x20; k++) rw::store8(rw::host_ptr<uint8_t>(chars + k), sbByte(account, 4 + k));
    rw::store8(rw::host_ptr<uint8_t>(chars + 0x20), 0);
    setNameValue(F, 0x48, S_DIGEST, 6);
    addMemberValue(F, doc, 0x48, 0x38, alloc);
    self->retryable = 1;
    uint32_t ctrl = keepParams(F, self, 0x2C);
    sendRequest(F, self, 59, cb, 0x58, 0x24);
    releaseIfSet(F, ctrl);
    return true;
}
RW_FUNCTION_NOVERIFY(0x4C52A8, "APIManager::executeLogin", APIManager::executeLogin);

// user/start: the anti-tamper report. Every value is encrypted with a Cipher whose key is
// md5(config file + user id + app version): "home" (uuid length), "comment" (account digest), "groups" (the
// FileUtils search paths) and "shell" (the account's date as "YYYY-MM-DD hh:mm:ss").
bool APIManager::executeUserStart(APIManager *self, uint32_t cb) {
    Frame F(0xDE0);
    if (refuseUnlessChain(F, self, cb)) return false;

    // the key
    F.call(VFS_READ_DATA, {F.at(0xE8), S_CONFIG_FILE});
    F.call(VFS_READ_DATA, {F.at(0xE0), S_VERSION_FILE});
    uint32_t config = load32(F.word(0xE8) + 4);
    uint64_t userId = sbU64(self->user.ptr, 0);
    F.setWord(0xF0, uint32_t(userId));
    F.setWord(0xF4, uint32_t(userId >> 32));
    uint32_t version = load32(F.word(0xE0) + 4);
    F.call(STRINGBUFFER_CTOR, {F.at(0x9C0), S_KEY_FORMAT, config, 0, uint32_t(userId), uint32_t(userId >> 32), version});
    F.call(MD5_CALCULATE, {F.at(0xD4), F.at(0x9C0), F.word(0x9C0 + STRINGBUFFER_LENGTH)});
    copyBytes(F.at(0x9A0), cstr(F.at(0xD4)), 0x20);
    const uint32_t cipher = F.at(0xB0);
    F.call(CIPHER_CTOR, {cipher, F.at(0x9A0)});

    uint32_t doc = newParams(F, 0xA8);
    uint32_t alloc = allocatorOf(doc);
    uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});

    // an encrypted string member: encrypt into the frame string at strSlot, copy it into a value at valueSlot,
    // add it as `name`, free the string
    auto addEncrypted = [&](uint32_t strSlot, uint32_t valueSlot, uint32_t name, uint32_t nameLength,
                            uint32_t src, uint32_t length) {
        F.call(CIPHER_ENCRYPT, {F.at(strSlot), cipher, src, length});
        F.call(VALUE_FROM_STRING, {F.at(valueSlot), cstr(F.at(strSlot)), alloc});
        setNameValue(F, 0x150, name, nameLength);
        addMemberValue(F, doc, 0x150, valueSlot, alloc);
        if (load8(F.at(strSlot)) & 1) F.call(OPERATOR_DELETE, {F.word(strSlot + 8)});
    };

    uint32_t uuidLength = sbWord(load32(app + APPSTATE_ACCOUNT), 0);
    F.setWord(0xFC, uuidLength);
    F.call(STRINGBUFFER_CTOR, {F.at(0x59C), S_UINT_FORMAT, uuidLength});
    addEncrypted(0x9C, 0x140, S_HOME, 4, F.at(0x59C), F.word(0x59C + STRINGBUFFER_LENGTH));

    uint32_t account = load32(app + APPSTATE_ACCOUNT);
    for (uint32_t k = 0; k < 0x20; k++) rw::store8(rw::host_ptr<uint8_t>(F.at(0x57C + k)), sbByte(account, 4 + k));
    addEncrypted(0x90, 0x130, S_COMMENT, 7, F.at(0x57C), 0x20);

    // "groups": the search paths
    setEmptyArray(F, 0x120);
    uint32_t files = F.call(FILEUTILS_GET_INSTANCE, {});
    F.call(load32(load32(files) + 0x50), {F.at(0x7C), files});      // virtual: the search path set
    for (uint32_t node = F.word(0x84); node; node = load32(node)) { // the set's node list (string at +8)
        uint32_t s = node + 8;
        F.call(CIPHER_ENCRYPT, {F.at(0x70), cipher, cstr(s), strSize(s)});
        F.call(VALUE_FROM_STRING, {F.at(0x110), cstr(F.at(0x70)), alloc});
        F.call(PUSH_BACK_VALUE, {F.at(0x120), F.at(0x110), alloc});
        if (load8(F.at(0x70)) & 1) F.call(OPERATOR_DELETE, {F.word(0x78)});
    }
    setNameValue(F, 0x150, S_GROUPS, 6);
    addMemberValue(F, doc, 0x150, 0x120, alloc);

    // "shell": the account date
    account = load32(app + APPSTATE_ACCOUNT);
    for (uint32_t k = 0; k < 8; k++) rw::store8(rw::host_ptr<uint8_t>(F.at(0x68 + k)), sbByte(account, 0x24 + k));
    F.call(DATE_MAKE_UTC, {F.at(0x60), F.at(0x68)});
    uint32_t year = rw::load<uint16_t>(F.at(0x60));
    uint32_t monthDay = rw::load<uint16_t>(F.at(0x62));
    uint32_t hourMinute = rw::load<uint16_t>(F.at(0x64));
    uint32_t second = load8(F.at(0x66));
    F.call(STRINGBUFFER_CTOR, {F.at(0x178), S_DATE_FORMAT, year, monthDay & 0xFF, monthDay >> 8, hourMinute & 0xFF,
                               hourMinute >> 8, second});
    addEncrypted(0x54, 0x100, S_SHELL, 5, F.at(0x178), F.word(0x178 + STRINGBUFFER_LENGTH));

    self->retryable = 0;
    uint32_t ctrl = F.word(0xAC);
    assignShared(F, self->params, doc, ctrl);
    sendRequest(F, self, 2, cb, 0x160, 0x4C);
    F.call(STRING_SET_DTOR, {F.at(0x7C)});
    releaseIfSet(F, ctrl);
    F.call(SAFEBUFFER_DTOR, {cipher + 4});                          // ~Cipher
    if (load8(F.at(0xD4)) & 1) F.call(OPERATOR_DELETE, {F.word(0xDC)});
    releaseIfSet(F, F.word(0xE4));
    releaseIfSet(F, F.word(0xEC));
    return true;
}
RW_FUNCTION_NOVERIFY(0x4C61C4, "APIManager::executeUserStart", APIManager::executeUserStart);

// ---------------------------------------------------------------------------------------------- chain steps
namespace {
bool chainOrGuardedRequest(APIManager *self, int32_t action, uint32_t cb) {
    Frame F(0x60);
    if (refuseUnlessChain(F, self, cb)) return false;
    self->retryable = 1;
    resetShared(F, self->params);
    sendRequest(F, self, action, cb, 0x28, 0x20);
    return true;
}
}  // namespace

bool APIManager::executeUserChatGet(APIManager *self, uint32_t cb) { return chainOrGuardedRequest(self, 5, cb); }
RW_FUNCTION_NOVERIFY(0x4C6870, "APIManager::executeUserChatGet", APIManager::executeUserChatGet);

bool APIManager::executeUserStoneGet(APIManager *self, uint32_t cb) { return chainOrGuardedRequest(self, 6, cb); }
RW_FUNCTION_NOVERIFY(0x4C6B88, "APIManager::executeUserStoneGet", APIManager::executeUserStoneGet);

// party/get: {"partyId"} when given (0 = the user's own party); only the parameterless form is resent.
bool APIManager::executePartyGet(APIManager *self, uint64_t partyId, uint32_t cb) {
    Frame F(0x68);
    if (refuseUnlessChain(F, self, cb)) return false;
    uint32_t doc = newParams(F, 0x28);
    uint32_t alloc = allocatorOf(doc);
    if (partyId != 0) addU64(F, doc, S_PARTY_ID, partyId, alloc);
    self->retryable = partyId == 0 ? 1 : 0;
    uint32_t ctrl = keepParams(F, self, 0x28);
    sendRequest(F, self, 78, cb, 0x30, 0x20);
    releaseIfSet(F, ctrl);
    return true;
}
RW_FUNCTION_NOVERIFY(0x4C69BC, "APIManager::executePartyGet", APIManager::executePartyGet);

// party/member/list: {"getDetail": 1} when detail, {"platformType"} when not 0.
bool APIManager::executePartyMemberList(APIManager *self, uint32_t detail, uint32_t cb, uint32_t platformType) {
    Frame F(0x68);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    uint32_t doc = newParams(F, 0x28);
    uint32_t alloc = allocatorOf(doc);
    if (detail == 1) addI32(F, doc, S_GET_DETAIL, 1, alloc);
    if (platformType != 0) addU8(F, doc, S_PLATFORM_TYPE, platformType, alloc);
    self->retryable = 1;
    uint32_t ctrl = keepParams(F, self, 0x28);
    sendRequest(F, self, 80, cb, 0x30, 0x20);
    releaseIfSet(F, ctrl);
    return true;
}
RW_FUNCTION_NOVERIFY(0x4C7AA4, "APIManager::executePartyMemberList", APIManager::executePartyMemberList);
