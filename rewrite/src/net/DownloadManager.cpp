// DownloadManager: downloads of server files (shop banners, ...) with a file cache. start() serves a cached copy
// (a SaveBin {expires, expiration_date, data} in CacheManager's cache path) while it has not expired, else sends an
// HttpRequest whose response (onRespond) is cached for `ttl` seconds; either way the caller's callback gets the
// data (finalize), and a callback answering false drops the cache file. Requests are numbered; the number -> URL
// map (an unordered_map<unsigned, std::string> at +4) lets cancel() forget one.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t CacheManager_getInstance = 0x4D7571;
constexpr uint32_t CacheManager_convertURLToCacheFilePath = 0x4D7585;   // (string out, cache, const string& url)
constexpr uint32_t CacheManager_removeCache = 0x4D78C1;                 // (cache, const string& url)
constexpr uint32_t InputStreamFactory_open = 0x8DC555;      // (shared_ptr<InputStream> out, const char*, bool)
constexpr uint32_t Data_makeSharedBytes = 0x4D7929;         // make_shared<bg::Data>(uchar*&, unsigned&, void(&)(void*))
constexpr uint32_t Data_makeSharedVoid = 0x4DF885;          // make_shared<bg::Data>(void*&, unsigned&, void(&)(void*))
constexpr uint32_t SaveBin_deserialize = 0x4F0FE9;          // (savebin, const shared_ptr<Data>&)
constexpr uint32_t SaveBin_serialize = 0x4F0E95;            // (shared_ptr<Data> out, savebin)
constexpr uint32_t SaveBin_getU8 = 0x4E0335;                // (savebin, key, uchar&) -> bool
constexpr uint32_t SaveBin_getLong = 0x4E03E9;              // (savebin, key, long&) -> bool
constexpr uint32_t SaveBin_getData = 0x4F0DED;              // (shared_ptr<Data> out, savebin, key)
constexpr uint32_t SaveBin_setU8 = 0x4E00FD;                // (savebin, key, const uchar&)
constexpr uint32_t SaveBin_setLong = 0x4E0219;              // (savebin, key, const long&)
constexpr uint32_t SaveBin_setData = 0x4F0CE1;              // (savebin, key, const shared_ptr<Data>&)
constexpr uint32_t SaveBinTable_dtor = 0x4DAB71;            // unordered_map<string, shared_ptr<Data>>::~
constexpr uint32_t UrlMap_find = 0x4DF691;                  // unordered_map<unsigned, string>::find(const unsigned&)
constexpr uint32_t UrlMap_erase = 0x4DF715;                 // erase(const_iterator)
constexpr uint32_t UrlMap_eraseKey = 0x4DF671;              // __erase_unique(const unsigned&)
constexpr uint32_t UrlMap_insert = 0x4DFC5D;                // __insert_unique(pair<unsigned, const char*>&&) (sret)
constexpr uint32_t Date_setIntervalFrom1970 = 0x492665;
constexpr uint32_t Date_setIntervalFromNow = 0x4926D1;
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t Date_calculateIntervalSince1970 = 0x492745;
constexpr uint32_t DataCallback_call = 0x4DF5E9;            // std::function<bool(const shared_ptr<Data>&)>::operator()
constexpr uint32_t DataCallback_copy = 0x4DFBF5;            // its copy constructor
constexpr uint32_t ResponseFunction_fromBind = 0x4DF585;    // function<void(HttpClient*, HttpResponse*)>(bind, 0)
constexpr uint32_t ResponseFunction_assign = 0x49D419;      // operator=(const function&)
constexpr uint32_t Ref_ctor = 0x8FD025;
constexpr uint32_t HttpClient_getInstance = 0x7E3FCD;
constexpr uint32_t HttpClient_send = 0x7E4409;
constexpr uint32_t string_assign = 0x48D811;                // std::string::assign(const char*, size_t)
constexpr uint32_t string_reserve = 0x48D705;
// libc (imports)
constexpr uint32_t MALLOC = 0x40FF58, FREE = 0x40FF64, MEMCPY = 0x40E164;
constexpr uint32_t FOPEN = 0x414158, FWRITE = 0x414164, FCLOSE = 0x414170;
}  // namespace g

// GOT slots (read at run time)
constexpr uint32_t GOT_FREE = 0xFD64C0;                 // &free
constexpr uint32_t GOT_ON_RESPOND = 0xFD64E4;           // &DownloadManager::onRespond
constexpr uint32_t GOT_HTTP_REQUEST_VTABLE = 0xFD6140;  // &_ZTVN7cocos2d7network11HttpRequestE
constexpr uint32_t GOT_DESTRUCTOR = 0xFD64E0;           // &DownloadManager::~DownloadManager

constexpr uint32_t INSTANCE_GUARD = 0x10039C0;
constexpr uint32_t INSTANCE = 0x10039C4;
constexpr uint32_t URL_MAP = 4;                         // unordered_map<unsigned, std::string>

constexpr uint32_t S_Expires = 0xDFC339;                // "expires"
constexpr uint32_t S_ExpirationDate = 0xDFC329;         // "expiration_date"
constexpr uint32_t S_Data = 0xDFC324;                   // "data"
constexpr uint32_t S_Wb = 0xDFC321;                     // "wb"

// HttpResponse
constexpr uint32_t RESPONSE_SUCCEEDED = 0x18;           // bool
constexpr uint32_t RESPONSE_DATA = 0x1C;                // std::vector<char>

// an empty SaveBin (its unordered_map<string, shared_ptr<Data>>, max load factor 1)
uint32_t newSaveBin(Frame &f) {
    uint32_t sb = f.alloc(20);
    for (uint32_t k = 0; k < 16; k += 4) st32(sb + k, 0);
    stf(sb + 16, 1.0f);
    return sb;
}

// The cache file path of `url`: CacheManager::convertURLToCacheFilePath(string(url)) into `out`.
void cachePath(uint32_t out, uint32_t url) {
    const uint32_t cache = call(g::CacheManager_getInstance, {});
    Frame f;
    const uint32_t s = newStr(f, url);
    call(g::CacheManager_convertURLToCacheFilePath, {out, cache, s});
    strFree(s);
}

}  // namespace

namespace DownloadManager {

uint32_t getInstance() {
    if (guest::staticInitBegin(INSTANCE_GUARD)) {
        for (uint32_t k = 0; k < 0x14; k += 4) st32(INSTANCE + k, 0);
        stf(INSTANCE + 0x14, 1.0f);
        rw::call(guest::CXA_ATEXIT, {ld32(GOT_DESTRUCTOR), INSTANCE, guest::DSO_HANDLE});
        guest::staticInitDone(INSTANCE_GUARD);
    }
    return INSTANCE;
}
// the first call constructs and registers the destructor: not shadow-run
RW_FUNCTION_NOVERIFY(0x4DECB0, "DownloadManager::getInstance", DownloadManager::getInstance);

// Hand `data` to the callback; when it answers false the cache file of `key` goes.
void finalize(uint32_t self, uint32_t key, uint32_t callback, uint32_t data) {
    (void)self;
    if (call(g::DataCallback_call, {callback, data})) return;
    const uint32_t cache = call(g::CacheManager_getInstance, {});
    Frame f;
    const uint32_t s = newStr(f, key);
    call(g::CacheManager_removeCache, {cache, s});
    strFree(s);
}
RW_FUNCTION(0x4DF1F4, "DownloadManager::finalize", DownloadManager::finalize);

// Download `url` (cached under `key`, the URL when null): -1 when the cache answered, else the request number.
int32_t start(uint32_t self, uint32_t url, uint32_t callback, int32_t ttl, uint32_t key) {
    if (!key) key = url;
    Frame f;
    const uint32_t path = f.alloc(12);
    cachePath(path, key);
    const uint32_t data = f.alloc(8);                 // shared_ptr<bg::Data> from the cache
    const uint32_t stream = f.alloc(8);
    call(g::InputStreamFactory_open, {stream, strData(path), 0});
    const uint32_t in = ld32(stream);
    if (!in || !vcall(in, 0x14, {in})) {
        st32(data, 0);
        st32(data + 4, 0);
    } else {
        const uint32_t sizeSlot = f.alloc(4), bufSlot = f.alloc(4);
        const uint32_t size = vcall(in, 0x18, {in});
        st32(sizeSlot, size);
        const uint32_t buf = call(g::MALLOC, {size});
        st32(bufSlot, buf);
        if (vcall(in, 0x08, {in, buf, size, 1}) != 1) {
            call(g::FREE, {buf});
            st32(data, 0);
            st32(data + 4, 0);
        } else {
            const uint32_t raw = f.alloc(8);
            call(g::Data_makeSharedBytes, {raw, bufSlot, sizeSlot, ld32(GOT_FREE)});
            const uint32_t sb = newSaveBin(f);
            call(g::SaveBin_deserialize, {sb, raw});
            const uint32_t expires = f.alloc(4), expiration = f.alloc(4);
            bool fresh = false;
            if (call(g::SaveBin_getU8, {sb, S_Expires, expires})) {
                if (!ld8(expires)) {
                    fresh = true;
                } else if (call(g::SaveBin_getLong, {sb, S_ExpirationDate, expiration}) == 1) {
                    const uint32_t date = f.alloc(8);
                    call(g::Date_setIntervalFrom1970, {date, ld32(expiration)});
                    fresh = int32_t(call(g::Date_calculateIntervalSinceNow, {date})) >= 0;
                }
            }
            bool have = false;
            if (fresh) {
                call(g::SaveBin_getData, {data, sb, S_Data});
                if (ld32(data)) have = true;
                else if (ld32(data + 4)) call(fn::shared_release, {ld32(data + 4)});
            }
            if (!have) {
                st32(data, 0);
                st32(data + 4, 0);
            }
            call(g::SaveBinTable_dtor, {sb});
            sharedRelease(raw);
        }
    }
    sharedRelease(stream);
    strFree(path);
    int32_t result;
    if (ld32(data)) {
        result = -1;
        finalize(self, key, callback, data);
    } else {
        const uint32_t id = ld32(self);
        st32(self, id + 1 == 0xFFFFFFFFu ? 0 : id + 1);
        result = int32_t(id);
        const uint32_t pair = f.alloc(8);
        st32(pair, id);
        st32(pair + 4, key);
        const uint32_t inserted = f.alloc(8);
        call(g::UrlMap_insert, {inserted, self + URL_MAP, pair});
        {
            const uint32_t cache = call(g::CacheManager_getInstance, {});
            Frame h;
            const uint32_t s = newStr(h, key);
            call(g::CacheManager_removeCache, {cache, s});
            strFree(s);
        }
        // new HttpRequest(url), its callback bind(&onRespond, this, id, callback, ttl, _1, _2)
        const uint32_t req = call(fn::operator_new, {0x88});
        call(g::Ref_ctor, {req});
        st32(req, ld32(GOT_HTTP_REQUEST_VTABLE) + 8);
        st32(req + 0x64, 0);
        st32(req + 0x68, 0);
        st32(req + 0x6C, 0);
        for (uint32_t k = 0; k < 0x24; k += 4) st32(req + 0x18 + k, 0);
        for (uint32_t off : {0x3Cu, 0x44u, 0x40u, 0x58u, 0x60u, 0x80u, 0x14u}) st32(req + off, 0);
        call(g::string_assign, {req + 0x18, url, host_strlen(url)});
        const uint32_t bind = f.alloc(0x2C);
        st32(bind, ld32(GOT_ON_RESPOND));
        st32(bind + 4, 0);
        st32(bind + 8, self);
        st32(bind + 0xC, id);
        call(g::DataCallback_copy, {bind + 0x10, callback});
        st32(bind + 0x28, uint32_t(ttl));
        const uint32_t fn = f.alloc(20);
        call(g::ResponseFunction_fromBind, {fn, bind, 0});
        call(g::ResponseFunction_assign, {req + 0x48, fn});
        functionDestroy(fn);
        functionDestroy(bind + 0x10);
        call(g::HttpClient_send, {call(g::HttpClient_getInstance, {}), req});
    }
    sharedRelease(data);
    return result;
}
RW_FUNCTION(0x4DED64, "DownloadManager::start", DownloadManager::start);

// The response of request `id`: on success cache the bytes (expiring after `ttl` seconds when > 0) and hand them on;
// on failure hand on an empty pointer. A request already cancelled is dropped.
void onRespond(uint32_t self, uint32_t id, uint32_t callback, int32_t ttl, uint32_t client, uint32_t response) {
    (void)client;
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, id);
    const uint32_t node = call(g::UrlMap_find, {self + URL_MAP, key});
    const uint32_t url = f.alloc(12);                 // the request's URL, moved out of the map
    for (uint32_t k = 0; k < 12; k += 4) st32(url + k, 0);
    if (node) {
        st8(url + 1, 0);
        st8(url, 0);
        call(g::string_reserve, {url, 0});
        for (uint32_t k = 0; k < 12; k += 4) {
            st32(url + k, ld32(node + 0xC + k));
            st32(node + 0xC + k, 0);
        }
        call(g::UrlMap_erase, {self + URL_MAP, node});
    }
    const uint32_t urlText = strData(url);
    if (ld8(response + RESPONSE_SUCCEEDED)) {
        const uint32_t from = ld32(response + RESPONSE_DATA);
        const uint32_t size = ld32(response + RESPONSE_DATA + 4) - from;
        const uint32_t sizeSlot = f.alloc(4), bufSlot = f.alloc(4);
        st32(sizeSlot, size);
        const uint32_t buf = call(g::MALLOC, {size});
        st32(bufSlot, buf);
        call(g::MEMCPY, {buf, from, size});
        const uint32_t data = f.alloc(8);
        call(g::Data_makeSharedVoid, {data, bufSlot, sizeSlot, ld32(GOT_FREE)});
        if (node) {
            const uint32_t path = f.alloc(12);
            cachePath(path, urlText);
            const uint32_t sb = newSaveBin(f);
            const uint32_t expires = f.alloc(4);
            st8(expires, ttl > 0 ? 1 : 0);
            call(g::SaveBin_setU8, {sb, S_Expires, expires});
            if (ld8(expires)) {
                const uint32_t date = f.alloc(8), when = f.alloc(4);
                call(g::Date_setIntervalFromNow, {date, uint32_t(ttl)});
                st32(when, call(g::Date_calculateIntervalSince1970, {date}));
                call(g::SaveBin_setLong, {sb, S_ExpirationDate, when});
            }
            call(g::SaveBin_setData, {sb, S_Data, data});
            const uint32_t file = call(g::FOPEN, {strData(path), S_Wb});
            if (file) {
                const uint32_t bytes = f.alloc(8);
                call(g::SaveBin_serialize, {bytes, sb});
                const uint32_t b = ld32(bytes);
                call(g::FWRITE, {ld32(b + 4), ld32(b + 8), 1, file});
                call(g::FCLOSE, {file});
                sharedRelease(bytes);
            }
            call(g::SaveBinTable_dtor, {sb});
            strFree(path);
            finalize(self, urlText, callback, data);
        }
        sharedRelease(data);
    } else if (node) {
        const uint32_t none = f.alloc(8);
        st32(none, 0);
        st32(none + 4, 0);
        finalize(self, urlText, callback, none);
    }
    strFree(url);
}
RW_FUNCTION(0x4DF2A0, "DownloadManager::onRespond", DownloadManager::onRespond);

void cancel(uint32_t self, uint32_t id) {
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, id);
    call(g::UrlMap_eraseKey, {self + URL_MAP, key});
}
RW_FUNCTION(0x4DF62C, "DownloadManager::cancel", DownloadManager::cancel);

}  // namespace DownloadManager
