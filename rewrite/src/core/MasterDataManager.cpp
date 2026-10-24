#include "MasterDataManager.hpp"

#include "SafeBuffer.hpp"

namespace {

// AppState
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;
constexpr uint32_t APPSTATE_GET_MASTER_FILE_REVISIONS = 0x4EAB4F;  // -> const vector<int>*
constexpr uint32_t APPSTATE_SET_MASTER_REVISIONS = 0x4EAB53;       // (state, revision, const vector<int>*)
// helpers next to the manager
constexpr uint32_t TABLE_PATH = 0x76CBBD;           // sub_76CBBC(out std::string, table): the table's file path
constexpr uint32_t TABLE_SIZE_OF_ROW_VTABLE = 0xF6EBF4;  // std::function impl of the constructor's lambda (row size)
// libc / libc++ / bg / cocos
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;         // __aeabi_memset(dst, n, c)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t MEMCMP = 0x40EF2C;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t STRTOL = 0x4101A4;
constexpr uint32_t REMOVE_FILE = 0x413C84;          // remove(path)
constexpr uint32_t FOPEN = 0x414158;
constexpr uint32_t FWRITE = 0x414164;
constexpr uint32_t FCLOSE = 0x414170;
constexpr uint32_t PTHREAD_CREATE = 0x410900;
constexpr uint32_t STRING_ASSIGN = 0x48D811;        // std::string::assign(const char*, size_t) (sub_48D810)
constexpr uint32_t STRING_APPEND = 0x47AC89;        // std::string::append(const char*, size_t) (sub_47AC88)
constexpr uint32_t STRING_COMPARE = 0x47CBD5;       // std::string::compare(const char*) (sub_47CBD4)
constexpr uint32_t VECTOR_STRING_DTOR = 0x47363F;   // std::__vector_base<std::string>::~__vector_base
constexpr uint32_t VECTOR_INT_DTOR = 0x478961;      // std::__vector_base<int>::~__vector_base
constexpr uint32_t VECTOR_INT_PUSH_BACK_SLOW = 0x47D709;  // std::vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t FUNCTION_ASSIGN_SIZE = 0x76F035; // std::function<unsigned(const shared_ptr<const SafeBuffer>&)>::operator=
constexpr uint32_t FUNCTION_CALL_HASH = 0x76C6D9;   // std::function<const char*()>::operator()
constexpr uint32_t FUNCTION_CALL_PARSE = 0x76DB45;  // std::function<shared_ptr<const master::Base>(const Value&)>::operator()
constexpr uint32_t OFFSETS_CLEAR = 0x76F201;        // unordered_map<int, long> clear
constexpr uint32_t OFFSETS_FIND = 0x76F5AF;         // unordered_map<int, long> find
constexpr uint32_t OFFSETS_AT = 0x76C869;           // unordered_map<int, long>::operator[]
constexpr uint32_t OFFSETS_INSERT_UNIQUE = 0x76F6B9;  // unordered_map<int, long> __insert_unique(pair<int, long>&&)
constexpr uint32_t OFFSETS_DTOR = 0x772431;
constexpr uint32_t ID_SET_INSERT_UNIQUE = 0x76F631; // std::set<int> __insert_unique(const int&)
constexpr uint32_t CACHE_FIND = 0x76CE55;           // LRUCache<unsigned long long, shared_ptr<const SafeBuffer>>::find
constexpr uint32_t CACHE_INSERT = 0x76CF35;         // LRUCache<...>::insert
constexpr uint32_t CACHE_MAP_CLEAR = 0x76EF45;      // the cache's unordered_map clear
constexpr uint32_t CACHE_MAP_DTOR = 0x77245F;
constexpr uint32_t KEY_LIST_CLEAR = 0x5CBD6D;       // std::__list_imp<unsigned long long>::clear
constexpr uint32_t QUEUE_DTOR = 0x76CD21;           // std::__deque_base<TableInfo>::~__deque_base
constexpr uint32_t QUEUE_CLEAR = 0x76E571;          // std::__deque_base<TableInfo>::clear
constexpr uint32_t QUEUE_PUSH_BACK = 0x76D35D;      // std::deque<TableInfo>::push_back(TableInfo&&)
constexpr uint32_t CIPHER_MAKE_SHARED = 0x4B975D;   // std::make_shared<Cipher>(std::array<uint8_t, 32>&)
constexpr uint32_t INPUT_STREAM_OPEN = 0x8DC555;    // bg::InputStreamFactory::open(out, path, bool)
constexpr uint32_t ARCHIVE_GET_INSTANCE = 0x8DC8F1; // bg::Archive::getInstance()
constexpr uint32_t ARCHIVE_GET_KEY = 0x8DC9CD;      // bg::Archive::getKey(out, archive, shared_ptr<IInputStream>, u16*)
constexpr uint32_t ARCHIVE_GET_DATA = 0x8DCBA9;     // bg::Archive::getData(out, archive, shared_ptr<IInputStream>, bool, u16*)
constexpr uint32_t ARCHIVE_CONVERT = 0x8DCDFD;      // bg::Archive::convert(out, archive, key, data, size)
constexpr uint32_t MEMORY_STREAM_MAKE_SHARED = 0x45A671;
constexpr uint32_t MEMORY_STREAM_WRITE = 0x491A79;  // bg::MemoryStream::write(src, size, count)
constexpr uint32_t STRINGBUFFER_FSET = 0x491501;    // StringBuffer::fset(fmt, ...)
constexpr uint32_t SAFEBUFFER_SERIALIZE = 0x45A555;
constexpr uint32_t FILEUTILS_GET_INSTANCE = rwlib::cocos2d::FileUtils::getInstance;
constexpr uint32_t REST_GET_INSTANCE = 0x497AE9;    // hole::network::RESTClient::getInstance
constexpr uint32_t REST_REQUEST = 0x497FE5;         // hole::network::RESTClient::request(...)
constexpr uint32_t PARSE_RESPONSE_BIND_VTABLE = 0xFA988C;  // std::function impl of bind(&parseResponse, this, _1)
constexpr uint32_t PARSE_RESPONSE = 0x76D6AD;       // MasterDataManager::parseResponse
constexpr uint32_t SAVE_THREAD_PROC = 0x76DB89;     // the download worker (thread entry, sub_76DB88)
constexpr uint32_t THROW_SYSTEM_ERROR = 0xC97965;   // std::__throw_system_error(int, const char*)
constexpr uint32_t THREAD_DETACH = 0xC97ADD;        // std::thread::detach (sub_C97ADC)
constexpr uint32_t THREAD_DTOR = 0xC97A3D;          // std::thread::~thread (sub_C97A3C)
// string literals
constexpr uint32_t STR_REVISION = 0xDFBF02, STR_URL = 0xDFBB13, STR_KEY = 0xE016E8, STR_MD5 = 0xE07BBD;
constexpr uint32_t STR_MASTER = 0xE07BC1, STR_COUNT = 0xDFB5EE, STR_M = 0xE07BCD, STR_HASH = 0xE07BC8;
constexpr uint32_t STR_PERCENT_D = 0xDF86DE, STR_WB = 0xDFC321, STR_THREAD_FAILED = 0xDFA2BA;
constexpr uint32_t DESTRUCTOR = 0x76C129;           // MasterDataManager::~MasterDataManager (for __cxa_atexit)

// rapidjson GenericValue: flags at +12 (type in the low bits; byte +13 bit 2 = number(int), byte +14 bit 4 = string)
constexpr uint32_t JSON_MEMBER = 0x4B1BB5;
uint32_t member(uint32_t value, uint32_t key) { return rw::call(JSON_MEMBER, {value, key}); }
bool isInt(uint32_t value) { return rw::load<uint8_t>(value + 13) & 4; }
bool isString(uint32_t value) { return rw::load<uint8_t>(value + 14) & 0x10; }
bool isObject(uint32_t value) { return rw::load<uint32_t>(value + 12) == 3; }

uint32_t archive() { return rw::call(ARCHIVE_GET_INSTANCE, {}); }
void releaseIf(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }
void addRefIf(uint32_t ctrl) { if (ctrl) guest::addRef(ctrl); }

// A shared_ptr copy in guest memory (the original passes shared_ptr arguments by value through its frame).
void copyShared(uint32_t dst, const guest::SharedPtr &src) {
    rw::save<uint32_t>(dst, src.obj);
    rw::save<uint32_t>(dst + 4, src.ctrl);
    addRefIf(src.ctrl);
}

// ~std::function for a function object in guest memory (`fn` = its __f_ pointer, `buffer` = its inline buffer).
void destroyFunction(uint32_t fn, uint32_t buffer) {
    if (fn == buffer) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(buffer) + 16), {buffer});   // destroy()
    else if (fn) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(fn) + 20), {fn});               // destroy_deallocate()
}

// c_str() of a std::string in guest memory
uint32_t cstr(uint32_t s) { return rw::host_ptr<guest::String>(s)->data(); }
void freeString(uint32_t s) {
    if (rw::load<uint8_t>(s) & 1) guest::deleteObject(rw::load<uint32_t>(s + 8));
}

}  // namespace

// ------------------------------------------------------------------------------------------ TableFile
uint32_t MasterDataManager::TableFile::init(TableFile *self, uint32_t table, uint32_t path) {
    // the original's frame (0x84 bytes): shared_ptr copies of the stream at 0x30/0x40/0x50/0x60, results at
    // 0x38/0x48/0x58/0x68, the stream at 0x70
    guest::Frame f(0x84);
    const uint32_t stream = f.addr(0x70);
    auto &S = *f.at<guest::SharedPtr>(0x70);
    uint32_t ok = 0;
    clear(self);
    rw::call(INPUT_STREAM_OPEN, {stream, path, 0});
    if (S.obj) {
        uint32_t a = archive();
        copyShared(f.addr(0x60), S);
        rw::call(ARCHIVE_GET_KEY, {f.addr(0x68), a, f.addr(0x60), 0});    // the first key: the table name
        releaseIf(rw::load<uint32_t>(f.addr(0x64)));
        auto &nameKey = *f.at<guest::SharedPtr>(0x68);
        if (nameKey.obj) {
            uint32_t name = rw::load<uint32_t>(nameKey.obj + 4);
            rw::call(STRING_ASSIGN, {rw::guest_addr(self), name, rw::call(STRLEN, {name})});
            rw::vcall(rw::host_ptr<void>(S.obj), 4, {S.obj, 0, 0});           // seek(0, 0)
            a = archive();
            copyShared(f.addr(0x50), S);
            rw::call(ARCHIVE_GET_DATA, {f.addr(0x58), a, f.addr(0x50), 0, 0});   // first entry: the revision
            releaseIf(rw::load<uint32_t>(f.addr(0x54)));
            auto &revisionData = *f.at<guest::SharedPtr>(0x58);
            if (!revisionData.obj) {
                clear(self);
                ok = 0;
            } else {
                uint32_t bytes = rw::load<uint32_t>(revisionData.obj + 4);
                uint8_t *rev = reinterpret_cast<uint8_t *>(&self->revision.raw);
                rw::store8(rev + 3, rw::load<uint8_t>(bytes + 3));
                rw::store8(rev + 2, rw::load<uint8_t>(bytes + 2));
                rw::store8(rev + 1, rw::load<uint8_t>(bytes + 1));
                rw::store8(rev + 0, rw::load<uint8_t>(bytes + 0));
                a = archive();
                copyShared(f.addr(0x40), S);
                ok = 0;
                rw::call(ARCHIVE_GET_DATA, {f.addr(0x48), a, f.addr(0x40), 0, 0});   // second entry: the hash
                releaseIf(rw::load<uint32_t>(f.addr(0x44)));
                auto &hashData = *f.at<guest::SharedPtr>(0x48);
                uint32_t fileHash = rw::load<uint32_t>(hashData.obj + 4);
                uint32_t hash = rw::call(FUNCTION_CALL_HASH, {TABLE_DESCRIPTORS + DESCRIPTOR_SIZE * table + 0x20});
                if (rw::call(MEMCMP, {fileHash, hash, rw::load<uint32_t>(hashData.obj + 8)}) != 0) {
                    clear(self);   // schema mismatch
                } else {
                    // index every row: key (row ID as text) -> stream offset of its entry
                    uint32_t offset = rw::vcall(rw::host_ptr<void>(S.obj), 3, {S.obj});   // tell()
                    uint32_t offsets = rw::guest_addr(self) + 24;
                    uint32_t prevCtrl = 0;
                    uint32_t keyCtrl;
                    for (;;) {
                        a = archive();
                        copyShared(f.addr(0x30), S);
                        rw::call(ARCHIVE_GET_KEY, {f.addr(0x38), a, f.addr(0x30), 0});
                        uint32_t key = rw::load<uint32_t>(f.addr(0x38));
                        keyCtrl = rw::load<uint32_t>(f.addr(0x3C));
                        rw::save<uint32_t>(f.addr(0x38), 0);
                        rw::save<uint32_t>(f.addr(0x3C), 0);
                        ok = 0;
                        bool valid;
                        if (prevCtrl) {
                            guest::release(prevCtrl);
                            valid = key != 0;
                            releaseIf(rw::load<uint32_t>(f.addr(0x3C)));
                        } else {
                            valid = key != 0;
                        }
                        releaseIf(rw::load<uint32_t>(f.addr(0x34)));
                        if (!valid) break;
                        uint32_t id = rw::call(STRTOL, {rw::load<uint32_t>(key + 4), f.addr(0x2C), 10});
                        if (rw::load<uint8_t>(rw::load<uint32_t>(f.addr(0x2C))) != 0) {   // not a number
                            clear(self);
                            goto release_key;
                        }
                        rw::save<uint32_t>(f.addr(0x24), id);
                        rw::save<uint32_t>(f.addr(0x28), offset);
                        rw::call(OFFSETS_INSERT_UNIQUE, {f.addr(0x78), offsets, f.addr(0x24)});
                        offset = rw::vcall(rw::host_ptr<void>(S.obj), 3, {S.obj});
                        prevCtrl = keyCtrl;
                    }
                    // the index must end exactly at the end of the file
                    if (offset == rw::vcall(rw::host_ptr<void>(S.obj), 6, {S.obj})) {
                        uint32_t obj = S.obj, ctrl = S.ctrl;
                        addRefIf(ctrl);
                        self->stream.obj = obj;
                        uint32_t old = self->stream.ctrl;
                        self->stream.ctrl = ctrl;
                        ok = 1;
                        releaseIf(old);
                    } else {
                        clear(self);
                    }
                release_key:
                    releaseIf(keyCtrl);
                }
                releaseIf(hashData.ctrl);
            }
            releaseIf(revisionData.ctrl);
        }
        releaseIf(nameKey.ctrl);
    }
    releaseIf(S.ctrl);
    return ok;
}
RW_FUNCTION(0x76C320, "MasterDataManager::TableFile::init", MasterDataManager::TableFile::init);

void MasterDataManager::TableFile::clear(TableFile *self) {
    // name.clear()
    if (self->name.isLong()) {
        rw::store8(rw::host_ptr<uint8_t>(rw::get<uint32_t>(self->name.raw, 8)), 0);
        rw::set<uint32_t>(self->name.raw, 4, 0);
    } else {
        rw::store8(self->name.raw + 1, 0);
        rw::store8(self->name.raw, 0);
    }
    self->revision = 0;
    self->stream.obj = 0;
    uint32_t ctrl = self->stream.ctrl;
    self->stream.ctrl = 0;
    releaseIf(ctrl);
    rw::call(OFFSETS_CLEAR, {rw::guest_addr(self) + 24});
}
RW_FUNCTION(0x76C69C, "MasterDataManager::TableFile::clear", MasterDataManager::TableFile::clear);

void MasterDataManager::TableFile::getIDs(uint32_t outSet, const TableFile *self) {
    // std::set<int>: begin node = &end node, end node's left child = null, size = 0
    rw::save<uint32_t>(outSet, outSet + 4);
    rw::save<uint32_t>(outSet + 4, 0);
    rw::save<uint32_t>(outSet + 8, 0);
    guest::Frame f(8);
    for (uint32_t node = self->offsetFirst; node; node = rw::load<uint32_t>(node))
        rw::call(ID_SET_INSERT_UNIQUE, {f.addr(), outSet, node + 8});
}
RW_FUNCTION(0x76C71C, "MasterDataManager::TableFile::getIDs", MasterDataManager::TableFile::getIDs);

void MasterDataManager::TableFile::getData(guest::SharedPtr *out, TableFile *self, int32_t id) {
    guest::Frame f(0x10);
    const uint32_t key = f.addr(8), streamCopy = f.addr(0);
    rw::save<int32_t>(key, id);
    uint32_t offsets = rw::guest_addr(self) + 24;
    if (rw::call(OFFSETS_FIND, {offsets, key})) {
        uint32_t s = self->stream.obj;
        uint32_t seek = rw::load<uint32_t>(rw::load<uint32_t>(s) + 16);
        uint32_t offset = rw::load<uint32_t>(rw::call(OFFSETS_AT, {offsets, key}));
        rw::call(seek, {s, offset, 0});
        uint32_t a = archive();
        copyShared(streamCopy, self->stream);
        rw::call(ARCHIVE_GET_DATA, {rw::guest_addr(out), a, streamCopy, 0, 0});
        releaseIf(rw::load<uint32_t>(streamCopy + 4));
    } else {
        out->obj = 0;
        out->ctrl = 0;
    }
}
RW_FUNCTION(0x76C798, "MasterDataManager::TableFile::getData", MasterDataManager::TableFile::getData);

// ------------------------------------------------------------------------------------------ TableInfo
uint32_t MasterDataManager::TableInfo::parse(TableInfo *self, uint32_t json) {
    uint32_t revision = member(json, STR_REVISION);
    if (!isInt(revision)) return 0;
    uint32_t url = member(json, STR_URL);
    if (!isString(url)) return 0;
    uint32_t key = member(json, STR_KEY);
    if (!isString(key)) return 0;
    uint32_t md5 = member(json, STR_MD5);
    if (!isString(md5)) return 0;
    self->revision = rw::load<int32_t>(revision);
    uint32_t s = rw::load<uint32_t>(url);
    rw::call(STRING_ASSIGN, {rw::guest_addr(&self->url), s, rw::call(STRLEN, {s})});
    s = rw::load<uint32_t>(md5);
    rw::call(STRING_ASSIGN, {rw::guest_addr(&self->md5), s, rw::call(STRLEN, {s})});
    guest::Frame f(0x28);
    rw::call(MEMCPY, {f.addr(8), rw::load<uint32_t>(key), 0x20});     // std::array<uint8_t, 32> cipher key
    rw::call(CIPHER_MAKE_SHARED, {f.addr(0), f.addr(8)});
    uint32_t obj = rw::load<uint32_t>(f.addr(0)), ctrl = rw::load<uint32_t>(f.addr(4));
    rw::save<uint32_t>(f.addr(0), 0);
    rw::save<uint32_t>(f.addr(4), 0);
    self->cipher.obj = obj;
    uint32_t old = self->cipher.ctrl;
    self->cipher.ctrl = ctrl;
    releaseIf(old);
    return 1;
}
RW_FUNCTION(0x76C1D4, "MasterDataManager::TableInfo::parse", MasterDataManager::TableInfo::parse);

// ------------------------------------------------------------------------------------------ the manager
MasterDataManager *MasterDataManager::getInstance() {
    if (guest::staticInitBegin(INSTANCE_GUARD)) {
        rw::call(0x76C8F5, {INSTANCE});   // MasterDataManager::MasterDataManager()
        rw::call(guest::CXA_ATEXIT, {DESTRUCTOR, INSTANCE, guest::DSO_HANDLE});
        guest::staticInitDone(INSTANCE_GUARD);
    }
    return rw::host_ptr<MasterDataManager>(INSTANCE);
}
RW_FUNCTION(0x76C090, "MasterDataManager::getInstance", MasterDataManager::getInstance);

MasterDataManager *MasterDataManager::destruct(MasterDataManager *self) {
    const uint32_t base = rw::guest_addr(self);
    releaseIf(self->connection.ctrl);
    destroyFunction(rw::load<uint32_t>(base + CACHE_SIZE_FN + 16), base + CACHE_SIZE_FN);
    rw::call(KEY_LIST_CLEAR, {base + CACHE_LIST});
    rw::call(CACHE_MAP_DTOR, {base + CACHE});
    rw::call(QUEUE_DTOR, {base + 2168});
    for (int k = int(TABLE_COUNT) - 1; k >= 0; k--) {
        TableFile &t = self->files[k];
        rw::call(OFFSETS_DTOR, {rw::guest_addr(&t) + 24});
        releaseIf(t.stream.ctrl);
        if (t.name.isLong()) guest::deleteObject(rw::get<uint32_t>(t.name.raw, 8));
    }
    return self;
}
RW_FUNCTION(0x76C128, "MasterDataManager::~MasterDataManager", MasterDataManager::destruct);

MasterDataManager *MasterDataManager::construct(MasterDataManager *self) {
    const uint32_t base = rw::guest_addr(self);
    self->masterRevision = 0;
    self->status = 0;
    self->downloadTotal = 0;
    for (uint32_t k = 0; k < TABLE_COUNT; k++) {
        rw::call(AEABI_MEMSET, {rw::guest_addr(&self->files[k]), 0x28, 0});
        self->files[k].offsetMaxLoad = 1.0f;
    }
    // the download queue and the first part of the row cache
    rw::call(AEABI_MEMSET, {base + 2168, 0x28, 0});
    rw::save<float>(base + 2208, 1.0f);                // cache map max load factor
    rw::save<uint32_t>(base + CACHE_LIST, base + CACHE_LIST);      // empty list: prev = next = end node
    rw::save<uint32_t>(base + CACHE_LIST + 4, base + CACHE_LIST);
    rw::save<uint32_t>(base + CACHE_LIST + 8, 0);
    rw::save<uint32_t>(base + CACHE_USED, 0);
    rw::save<uint32_t>(base + CACHE_SIZE_FN + 16, 0);  // empty std::function
    self->downloading = 0;
    self->connection.obj = 0;
    self->connection.ctrl = 0;
    {
        // cache capacity 256 KiB; row size = a lambda (std::function assigned from a temporary)
        guest::Frame f(0x18);
        const uint32_t fn = f.addr(0);
        rw::save<uint32_t>(f.addr(0x10), fn);
        rw::save<uint32_t>(fn, TABLE_SIZE_OF_ROW_VTABLE);
        rw::save<uint32_t>(base + CACHE_CAPACITY, 0x40000);
        rw::call(FUNCTION_ASSIGN_SIZE, {base + CACHE_SIZE_FN, fn});
        destroyFunction(rw::load<uint32_t>(f.addr(0x10)), fn);
    }
    // open every table file; any failure (or a revision / name mismatch) forgets the master revisions
    uint32_t state = rw::call(APPSTATE_GET_INSTANCE, {});
    uint32_t revisions = rw::call(APPSTATE_GET_MASTER_FILE_REVISIONS, {state});
    uint32_t count = uint32_t(int32_t(rw::load<uint32_t>(revisions + 4) - rw::load<uint32_t>(revisions)) >> 2);
    bool reset = count > TABLE_COUNT;
    if (!reset && count != 0) {
        uint32_t ok = 1;
        guest::Frame f(0x10);
        const uint32_t path = f.addr(0);
        for (uint32_t k = 0; k < count; k++) {
            rw::call(TABLE_PATH, {path, k});
            TableFile &t = self->files[k];
            if (TableFile::init(&t, k, cstr(path)) != 1 ||
                t.revision != rw::load<int32_t>(rw::load<uint32_t>(revisions) + 4 * k) ||
                rw::call(STRING_COMPARE, {rw::guest_addr(&t), rw::load<uint32_t>(TABLE_DESCRIPTORS + DESCRIPTOR_SIZE * k)}) != 0)
                ok = 0;
            freeString(path);
        }
        reset = (ok & 1) == 0;
    }
    if (reset) rw::call(APPSTATE_SET_MASTER_REVISIONS, {rw::call(APPSTATE_GET_INSTANCE, {}), 0, 0});
    return self;
}
RW_FUNCTION(0x76C8F4, "MasterDataManager::MasterDataManager", MasterDataManager::construct);

uint32_t MasterDataManager::remove(MasterDataManager *self) {
    const uint32_t base = rw::guest_addr(self);
    rw::call(CACHE_MAP_CLEAR, {base + CACHE});
    rw::call(KEY_LIST_CLEAR, {base + CACHE_LIST});
    rw::save<uint32_t>(base + CACHE_USED, 0);
    guest::Frame f(0x10);
    const uint32_t path = f.addr(0);
    for (uint32_t k = 0; k < TABLE_COUNT; k++) {
        TableFile::clear(&self->files[k]);
        rw::call(TABLE_PATH, {path, k});
        if (rw::load<uint8_t>(path) & 1) {
            uint32_t data = rw::load<uint32_t>(path + 8);
            rw::call(REMOVE_FILE, {data});
            guest::deleteObject(data);
        } else {
            rw::call(REMOVE_FILE, {path + 1});
        }
    }
    uint32_t state = rw::call(APPSTATE_GET_INSTANCE, {});
    return rw::call(APPSTATE_SET_MASTER_REVISIONS, {state, 0, 0}), 0;
}
RW_FUNCTION(0x76CD4C, "MasterDataManager::remove", MasterDataManager::remove);

// The cache key: table in the high word (or-ed with the sign of the id), id in the low word.
static void cacheKey(uint32_t at, uint32_t table, int32_t id) {
    rw::save<int32_t>(at, id);
    rw::save<uint32_t>(at + 4, uint32_t(id >> 31) | table);
}

void MasterDataManager::find(guest::SharedPtr *out, MasterDataManager *self, uint32_t table, int32_t id) {
    guest::Frame f(8);
    cacheKey(f.addr(), table, id);
    rw::call(CACHE_FIND, {rw::guest_addr(out), rw::guest_addr(self) + CACHE, f.addr()});
}
RW_FUNCTION(0x76CE04, "MasterDataManager::find", MasterDataManager::find);

void MasterDataManager::insert(MasterDataManager *self, uint32_t table, int32_t id, const guest::SharedPtr *row) {
    if (!row->obj) return;
    guest::Frame f(0x10);
    cacheKey(f.addr(8), table, id);
    copyShared(f.addr(0), *row);
    rw::call(CACHE_INSERT, {rw::guest_addr(self) + CACHE, f.addr(8), f.addr(0)});
    releaseIf(rw::load<uint32_t>(f.addr(4)));
}
RW_FUNCTION(0x76CEA8, "MasterDataManager::insert", MasterDataManager::insert);

void MasterDataManager::getIDs(uint32_t outSet, const MasterDataManager *self, uint32_t table) {
    TableFile::getIDs(outSet, rw::host_ptr<TableFile>(rw::guest_addr(self) + 44 * table + 12));
}
RW_FUNCTION(0x76D090, "MasterDataManager::getIDs", MasterDataManager::getIDs);

void MasterDataManager::getData(guest::SharedPtr *out, MasterDataManager *self, uint32_t table, int32_t id) {
    TableFile::getData(out, rw::host_ptr<TableFile>(rw::guest_addr(self) + 44 * table + 12), id);
}
RW_FUNCTION(0x76D0A0, "MasterDataManager::getData", MasterDataManager::getData);

uint32_t MasterDataManager::reset(MasterDataManager *self) {
    self->downloadTotal = 0;
    return rw::call(QUEUE_CLEAR, {rw::guest_addr(self) + 2168});
}
RW_FUNCTION(0x76D0B4, "MasterDataManager::reset", MasterDataManager::reset);

uint32_t MasterDataManager::update(MasterDataManager *self, uint32_t document) {
    if (!isObject(document)) return 0;
    uint32_t master = member(document, STR_MASTER);
    if (!isObject(master)) return 0;
    uint32_t revision = member(master, STR_REVISION);
    if (!isInt(revision)) return 0;
    uint32_t count = member(master, STR_COUNT);
    if (!isInt(count)) return 0;
    const uint32_t base = rw::guest_addr(self);
    guest::Frame f(0x74);
    const uint32_t info = f.addr(0x3C);             // TableInfo
    auto &ti = *f.at<TableInfo>(0x3C);
    int32_t listed = 0;
    for (uint32_t k = 0; k < TABLE_COUNT; k++) {
        uint32_t entry = member(document, rw::load<uint32_t>(TABLE_DESCRIPTORS + DESCRIPTOR_SIZE * k));
        if (!isObject(entry)) continue;
        rw::call(AEABI_MEMSET, {info + 8, 0x20, 0});
        if (TableInfo::parse(&ti, entry) == 1) {
            if (ti.revision > self->files[k].revision) {    // newer than the file: queue it
                ti.table = k;
                rw::call(QUEUE_PUSH_BACK, {base + 2168, info});
            }
            listed++;
        }
        if (ti.md5.isLong()) guest::deleteObject(rw::get<uint32_t>(ti.md5.raw, 8));
        releaseIf(ti.cipher.ctrl);
        if (ti.url.isLong()) guest::deleteObject(rw::get<uint32_t>(ti.url.raw, 8));
    }
    if (listed >= 1 && rw::load<int32_t>(count) == listed) {
        // the server listed every table: drop the cached rows
        rw::call(CACHE_MAP_CLEAR, {base + CACHE});
        rw::call(KEY_LIST_CLEAR, {base + CACHE_LIST});
        rw::save<uint32_t>(base + CACHE_USED, 0);
    }
    self->masterRevision = rw::load<int32_t>(revision);
    self->downloadTotal = self->queueSize;
    // FileUtils::createDirectory(getWritablePath() + "m")
    uint32_t files = rw::call(FILEUTILS_GET_INSTANCE, {});
    uint32_t createDirectory = rw::load<uint32_t>(rw::load<uint32_t>(files) + 0x54);
    uint32_t files2 = rw::call(FILEUTILS_GET_INSTANCE, {});
    rw::call(rw::load<uint32_t>(rw::load<uint32_t>(files2) + 0x44), {f.addr(0x64), files2});   // getWritablePath
    uint32_t appended = rw::call(STRING_APPEND, {f.addr(0x64), STR_M, 1});
    // move the result into a new string at 0x30
    rw::save<uint32_t>(f.addr(0x38), rw::load<uint32_t>(appended + 8));
    rw::save<uint32_t>(f.addr(0x34), rw::load<uint32_t>(appended + 4));
    rw::save<uint32_t>(f.addr(0x30), rw::load<uint32_t>(appended));
    rw::save<uint32_t>(appended, 0);
    rw::save<uint32_t>(appended + 4, 0);
    rw::save<uint32_t>(appended + 8, 0);
    if (rw::load<uint8_t>(f.addr(0x64)) & 1) guest::deleteObject(rw::load<uint32_t>(f.addr(0x6C)));
    rw::call(createDirectory, {files, f.addr(0x30), 0});
    if (rw::load<uint8_t>(f.addr(0x30)) & 1) guest::deleteObject(rw::load<uint32_t>(f.addr(0x38)));
    startNextDownload(self);
    return 1;
}
RW_FUNCTION(0x76D0C8, "MasterDataManager::update", MasterDataManager::update);

uint32_t MasterDataManager::startNextDownload(MasterDataManager *self) {
    if (self->queueSize == 0) {
        finishDownload(self);
        self->downloading = 0;
        return 0;
    }
    self->downloading = 1;
    self->status = 0;
    // the queue front (deque blocks of 102 TableInfo)
    uint32_t start = self->queueStart;
    uint32_t block = rw::load<uint32_t>(self->queueMapBegin + 4 * (start / 102));
    uint32_t front = block + 40 * (start % 102);
    uint32_t rest = rw::call(REST_GET_INSTANCE, {});
    uint32_t url = cstr(front + 8);
    guest::Frame f(0x48);
    const uint32_t connection = f.addr(0x00);   // shared_ptr<Connection> (empty)
    const uint32_t body = f.addr(0x08);         // shared_ptr<rapidjson::Document> (empty)
    const uint32_t headers = f.addr(0x10);      // vector<string> (empty)
    const uint32_t result = f.addr(0x1C);       // shared_ptr<Connection> returned
    const uint32_t callback = f.addr(0x24);     // std::function: bind(&parseResponse, this, _1), __f_ at +0x10
    for (uint32_t k = 0; k < 3; k++) rw::save<uint32_t>(headers + 4 * k, 0);
    rw::save<uint32_t>(body, 0);
    rw::save<uint32_t>(body + 4, 0);
    rw::save<uint32_t>(callback + 0x10, callback);
    rw::save<uint32_t>(callback, PARSE_RESPONSE_BIND_VTABLE);
    rw::save<uint32_t>(callback + 4, PARSE_RESPONSE);
    rw::save<uint32_t>(callback + 8, 0);
    rw::save<uint32_t>(callback + 12, rw::guest_addr(self));
    rw::save<uint32_t>(connection, 0);
    rw::save<uint32_t>(connection + 4, 0);
    // request(url, GET, headers, body, callback, cipher, false, connection, md5)
    rw::call(REST_REQUEST, {result, rest, url, 0, headers, body, callback, front + 20, 0, connection, front + 28});
    uint32_t obj = rw::load<uint32_t>(result), ctrl = rw::load<uint32_t>(result + 4);
    rw::save<uint32_t>(result, 0);
    rw::save<uint32_t>(result + 4, 0);
    self->connection.obj = obj;
    uint32_t old = self->connection.ctrl;
    self->connection.ctrl = ctrl;
    if (old) {
        guest::release(old);
        releaseIf(rw::load<uint32_t>(result + 4));
    }
    releaseIf(rw::load<uint32_t>(connection + 4));
    destroyFunction(rw::load<uint32_t>(callback + 0x10), callback);
    releaseIf(rw::load<uint32_t>(body + 4));
    rw::call(VECTOR_STRING_DTOR, {headers});
    return 1;
}
RW_FUNCTION(0x76D40C, "MasterDataManager::startNextDownload", MasterDataManager::startNextDownload);

uint8_t MasterDataManager::isDownloading(const MasterDataManager *self) { return self->downloading; }
RW_FUNCTION(0x76D5E0, "MasterDataManager::isDownloading", MasterDataManager::isDownloading);

void MasterDataManager::finishDownload(MasterDataManager *self) {
    // the revisions of all table files, as the AppState's master file revisions
    guest::Frame f(0x10);
    const uint32_t value = f.addr(0), vec = f.addr(4);
    auto &v = *f.at<guest::Vector>(4);
    v.begin = 0;
    v.end = 0;
    v.cap = 0;
    for (uint32_t k = 0; k < TABLE_COUNT; k++) {
        int32_t rev = self->files[k].revision;
        rw::save<int32_t>(value, rev);
        if (v.end < v.cap) {
            uint32_t end = 0;
            if (v.end) {
                rw::save<int32_t>(v.end, rev);
                end = v.end;
            }
            v.end = end + 4;
        } else {
            rw::call(VECTOR_INT_PUSH_BACK_SLOW, {vec, value});
        }
    }
    uint32_t state = rw::call(APPSTATE_GET_INSTANCE, {});
    rw::call(APPSTATE_SET_MASTER_REVISIONS, {state, uint32_t(self->masterRevision.get()), vec});
    rw::call(VECTOR_INT_DTOR, {vec});
}
RW_FUNCTION(0x76D5E8, "MasterDataManager::finishDownload", MasterDataManager::finishDownload);

uint32_t MasterDataManager::getProgress(const MasterDataManager *self, const uint32_t *total) {
    if (total) rw::store32(total, self->downloadTotal);
    return self->downloadTotal - self->queueSize;
}
RW_FUNCTION(0x76D694, "MasterDataManager::getProgress", MasterDataManager::getProgress);

void MasterDataManager::parseResponse(MasterDataManager *self, const guest::SharedPtr *result) {
    uint32_t res = result->obj;     // RESTClient::Result: +0 error, +20 state (4 = done)
    self->connection.obj = 0;
    uint32_t old = self->connection.ctrl;
    self->connection.ctrl = 0;
    releaseIf(old);
    if (rw::load<uint32_t>(res) != 0 || rw::load<uint32_t>(res + 20) != 4) {
        self->status = rw::load<uint32_t>(res) != 0 ? 1 : 3;
        self->downloading = 0;
        return;
    }
    // save on a detached worker thread: arg = {this, shared_ptr<Result>}
    uint32_t obj = result->obj, ctrl = result->ctrl;
    addRefIf(ctrl);
    uint32_t arg = guest::newObject(12);
    if (ctrl) {
        guest::addRef(ctrl);
        rw::save<uint32_t>(arg, rw::guest_addr(self));
        rw::save<uint32_t>(arg + 4, obj);
        rw::save<uint32_t>(arg + 8, ctrl);
        guest::addRef(ctrl);
        rw::save<uint32_t>(arg, rw::guest_addr(self));
        guest::release(ctrl);
    } else {
        rw::save<uint32_t>(arg, rw::guest_addr(self));
        rw::save<uint32_t>(arg + 4, obj);
        rw::save<uint32_t>(arg + 8, 0);
    }
    guest::Frame f(8);
    const uint32_t thread = f.addr(0);
    uint32_t err = rw::call(PTHREAD_CREATE, {thread, 0, SAVE_THREAD_PROC, arg});
    if (err) rw::call(THROW_SYSTEM_ERROR, {err, STR_THREAD_FAILED});
    releaseIf(ctrl);
    rw::call(THREAD_DETACH, {thread});
    rw::call(THREAD_DTOR, {thread});
}
RW_FUNCTION(0x76D6AC, "MasterDataManager::parseResponse", MasterDataManager::parseResponse);

uint32_t MasterDataManager::save(MasterDataManager *self, const TableInfo *info, uint32_t rows) {
    // the original's frame is 0x474 bytes: a 1 KiB StringBuffer at 0x6C (length word at +0x400)
    guest::Frame f(0x474);
    const uint32_t ms = f.addr(0x48);           // shared_ptr<bg::MemoryStream>
    rw::call(MEMORY_STREAM_MAKE_SHARED, {ms});
    uint32_t table = info->table;
    uint32_t descriptor = TABLE_DESCRIPTORS + DESCRIPTOR_SIZE * table;
    uint32_t name = rw::load<uint32_t>(descriptor);
    rw::save<int32_t>(f.addr(0x58), info->revision);
    uint32_t a = archive();
    rw::call(ARCHIVE_CONVERT, {f.addr(0x50), a, name, f.addr(0x58), 4});      // name -> revision
    uint32_t stream = rw::load<uint32_t>(ms);
    uint32_t entry = rw::load<uint32_t>(f.addr(0x50));
    rw::call(MEMORY_STREAM_WRITE, {stream, rw::load<uint32_t>(entry + 4), rw::load<uint32_t>(entry + 8), 1});
    releaseIf(rw::load<uint32_t>(f.addr(0x54)));
    uint32_t hash = rw::call(FUNCTION_CALL_HASH, {TABLE_DESCRIPTORS + DESCRIPTOR_SIZE * info->table + 0x20});
    a = archive();
    uint32_t hashLength = rw::call(STRLEN, {hash});
    rw::call(ARCHIVE_CONVERT, {f.addr(0x5C), a, STR_HASH, hash, hashLength});   // "hash" -> schema hash
    entry = rw::load<uint32_t>(f.addr(0x5C));
    rw::call(MEMORY_STREAM_WRITE, {stream, rw::load<uint32_t>(entry + 4), rw::load<uint32_t>(entry + 8), 1});
    releaseIf(rw::load<uint32_t>(f.addr(0x60)));
    const uint32_t key = f.addr(0x6C);
    rw::save<uint32_t>(key + 0x400, 0);
    rw::store8(rw::host_ptr<uint8_t>(key), 0);
    table = info->table;
    int32_t count = rw::load<int32_t>(rows + 4);
    uint32_t ok = 0;
    bool failed = false;
    for (int32_t j = 0; j < count; j++) {
        // row = descriptor.parse(rows[j]); key = "%d" % row->getID(); value = row->serialize()
        rw::call(FUNCTION_CALL_PARSE, {f.addr(0x40), TABLE_DESCRIPTORS + DESCRIPTOR_SIZE * table + 8,
                                       rw::load<uint32_t>(rows) + 16 * uint32_t(j)});
        uint32_t row = rw::load<uint32_t>(f.addr(0x40));
        if (!row) {
            self->status = 3;
            ok = 0;
            releaseIf(rw::load<uint32_t>(f.addr(0x44)));
            failed = true;
            break;
        }
        uint32_t id = rw::vcall(rw::host_ptr<void>(row), 2, {row});
        rw::call(STRINGBUFFER_FSET, {key, STR_PERCENT_D, id});
        rw::call(SAFEBUFFER_SERIALIZE, {f.addr(0x38), row});
        a = archive();
        uint32_t data = rw::load<uint32_t>(f.addr(0x38));
        rw::call(ARCHIVE_CONVERT, {f.addr(0x64), a, key, rw::load<uint32_t>(data + 4), rw::load<uint32_t>(data + 8)});
        entry = rw::load<uint32_t>(f.addr(0x64));
        rw::call(MEMORY_STREAM_WRITE, {stream, rw::load<uint32_t>(entry + 4), rw::load<uint32_t>(entry + 8), 1});
        releaseIf(rw::load<uint32_t>(f.addr(0x68)));
        releaseIf(rw::load<uint32_t>(f.addr(0x3C)));
        releaseIf(rw::load<uint32_t>(f.addr(0x44)));
        table = info->table;
    }
    if (!failed) {
        const uint32_t path = f.addr(0x2C);
        rw::call(TABLE_PATH, {path, table});
        uint32_t file = rw::call(FOPEN, {cstr(path), STR_WB});
        bool written = false;
        if (file) {
            uint32_t bytes = rw::vcall(rw::host_ptr<void>(stream), 7, {stream});   // data()
            uint32_t size = rw::vcall(rw::host_ptr<void>(stream), 6, {stream});    // size()
            ok = 1;
            uint32_t n = rw::call(FWRITE, {bytes, size, 1, file});
            rw::call(FCLOSE, {file});
            if (n == 1) {
                uint32_t t = info->table;
                written = TableFile::init(rw::host_ptr<TableFile>(rw::guest_addr(self) + 44 * t + 12), t, cstr(path)) != 0;
            }
        }
        if (!written) {
            self->status = 2;
            ok = 0;
        }
        freeString(path);
    }
    releaseIf(rw::load<uint32_t>(ms + 4));
    return ok;
}
RW_FUNCTION(0x76D81C, "MasterDataManager::save", MasterDataManager::save);

// ------------------------------------------------------------------------------------------ helpers (unnamed)
namespace {
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;      // StringBuffer::StringBuffer(fmt, ...)
constexpr uint32_t STR_TABLE_FILE_FORMAT = 0xE07BCF;  // "m%03d.jpg"
constexpr uint32_t STR_SLASH = 0xDF8C66;              // "/"
constexpr uint32_t FILEUTILS_VSLOT_GET_WRITABLE_PATH = 0x44 / 4;
constexpr uint32_t THREAD_LOCAL_DATA = 0xC97C4D;      // std::__thread_local_data()
constexpr uint32_t THREAD_STRUCT_CTOR = 0xC97ED9;     // std::__thread_struct::__thread_struct()
constexpr uint32_t THREAD_SPECIFIC_RESET = 0x499749;  // __thread_specific_ptr<__thread_struct>::reset(p)
constexpr uint32_t QUEUE_POP_FRONT = 0x76DC5D;        // std::deque<TableInfo>::pop_front()

// dst = std::move(*src) of a temporary std::string the original returns by reference (append): take the three
// words, leave src empty
void takeString(uint32_t dst, uint32_t src) {
    uint32_t w0 = rw::load<uint32_t>(src), w1 = rw::load<uint32_t>(src + 4), w2 = rw::load<uint32_t>(src + 8);
    rw::save<uint32_t>(dst, w0);
    rw::save<uint32_t>(dst + 4, w1);
    rw::save<uint32_t>(dst + 8, w2);
    rw::save<uint32_t>(src, 0);
    rw::save<uint32_t>(src + 4, 0);
    rw::save<uint32_t>(src + 8, 0);
}
}  // namespace

// sub_76CBBC(out, table): the file path of a table, FileUtils::getWritablePath() + "m" + "/" + "m%03d.jpg".
static void tablePath(uint32_t out, int32_t table) {
    guest::Frame f(0x460);
    const uint32_t fileName = f.addr(0x3C);   // StringBuffer (its characters first)
    const uint32_t writable = f.addr(0x30), dir = f.addr(0x18), dirSlash = f.addr(0x24);
    rw::call(STRINGBUFFER_CTOR, {fileName, STR_TABLE_FILE_FORMAT, uint32_t(table)});
    uint32_t fileUtils = rw::call(FILEUTILS_GET_INSTANCE, {});
    rw::vcall(rw::host_ptr<void>(fileUtils), FILEUTILS_VSLOT_GET_WRITABLE_PATH, {writable, fileUtils});
    takeString(dir, rw::call(STRING_APPEND, {writable, STR_M, 1}));
    freeString(writable);
    takeString(dirSlash, rw::call(STRING_APPEND, {dir, STR_SLASH, 1}));
    uint32_t n = rw::call(STRLEN, {fileName});
    takeString(out, rw::call(STRING_APPEND, {dirSlash, fileName, n}));
    freeString(dirSlash);
    freeString(dir);
}
RW_FUNCTION(0x76CBBC, "MasterDataManager table file path (sub_76CBBC)", tablePath);

// sub_76DB88: the download worker thread (std::thread proxy of startNextDownload's lambda, captures {this,
// shared_ptr<document>}): save the front table of the queue; on success drop it and start the next download,
// else stop downloading. NOVERIFY: it installs the thread's __thread_struct (pthread_setspecific: host state the
// verifier cannot undo, a shadow run would free the first run's struct) and writes the table file.
static uint32_t saveThreadProc(uint32_t captures) {
    uint32_t tld = rw::call(THREAD_LOCAL_DATA, {});
    uint32_t ts = guest::newObject(4);
    rw::call(THREAD_STRUCT_CTOR, {ts});
    rw::call(THREAD_SPECIFIC_RESET, {tld, ts});
    auto *self = rw::host_ptr<MasterDataManager>(rw::load<uint32_t>(captures));
    uint32_t start = self->queueStart;
    uint32_t front = rw::load<uint32_t>(self->queueMapBegin + (start / 102) * 4) + (start % 102) * 40;
    uint32_t rows = rw::load<uint32_t>(captures + 4) + 8;
    if (MasterDataManager::save(self, rw::host_ptr<MasterDataManager::TableInfo>(front), rows)) {
        rw::call(QUEUE_POP_FRONT, {rw::guest_addr(&self->queueMapFirst)});
        MasterDataManager::startNextDownload(self);
    } else {
        self->downloading = 0;
    }
    releaseIf(rw::load<uint32_t>(captures + 8));
    guest::deleteObject(captures);
    return 0;
}
RW_FUNCTION_NOVERIFY(0x76DB88, "MasterDataManager::startNextDownload worker thread (sub_76DB88)", saveThreadProc);
