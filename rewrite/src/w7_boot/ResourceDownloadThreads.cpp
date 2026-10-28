// ResourceDataManager's download worker threads (std::thread __thread_proxy instances over its lambdas; the
// threads are started in ResourceDownload.cpp). Both install the thread's __thread_struct first, as std::thread
// does, and free their capture block at the end.
#include "core/Guest.hpp"
#include "libcxx/Libcxx.hpp"

namespace {

using rw::load;
using rw::save;

constexpr uint32_t THREAD_LOCAL_DATA = 0xC97C4D;       // std::__thread_local_data()
constexpr uint32_t THREAD_STRUCT_CTOR = 0xC97ED9;      // std::__thread_struct::__thread_struct()
constexpr uint32_t THREAD_SPECIFIC_RESET = 0x499749;   // __thread_specific_ptr<__thread_struct>::reset(p)
constexpr uint32_t START_NEXT_DOWNLOAD = 0x773CF5;     // ResourceDataManager::startNextDownloadThread()
constexpr uint32_t STRING_INIT_RANGE = 0x49A4A7;       // basic_string::__init(char* first, char* last)
constexpr uint32_t MD5_CALCULATE = 0x4E0A89;           // md5::calculate(string* out, const void*, size_t)
constexpr uint32_t SEARCH_PRED = 0x49A42B;             // std::__search<bool (*&)(char, char)>(...)
constexpr uint32_t CHAR_PRED = 0x49A419;               // the case-folding char comparison (sub_49A418)
constexpr uint32_t OCTET_STREAM = 0xDFA30E;            // "application/octet-stream" (24 chars)
constexpr uint32_t CACHE_GET_INSTANCE = 0x4D7571;      // hole::network::CacheManager::getInstance()
constexpr uint32_t CACHE_FILE_PATH = 0x4D7585;         // CacheManager::convertURLToCacheFilePath(string*, this, const string&)
constexpr uint32_t REF_RELEASE = 0x8FD0BD;             // cocos2d::Ref::release()
constexpr uint32_t ITEMS_POP_FRONT = 0x77453D;         // std::deque<const void*>::pop_front()
constexpr uint32_t FOPEN = 0x414158, FWRITE = 0x414164, FCLOSE = 0x414170, MEMCMP = 0x40EF2C;   // imports
constexpr uint32_t LIT_WB = 0xDFC321;                  // "wb"

// ResourceDataManager fields (see w3_1/ResourceDataManager.cpp)
constexpr uint32_t RDM_ERROR = 0x00, RDM_DONE = 0x08, RDM_PROCESSING = 0x0D, RDM_ITEMS = 0x30, RDM_ITEM_MAP_BEGIN = 0x34,
                   RDM_ITEM_START = 0x40, RDM_PIECE_TOTAL = 0x50;
// a queued piece (ResourceInfo item): its md5 string +0xC, its size +0x18; HttpResponse: the body (vector<char>)
// +0x1C / +0x20, the content type (vector<char>) +0x28 / +0x2C
constexpr uint32_t ITEM_MD5 = 0xC, ITEM_SIZE = 0x18, RESP_BODY = 0x1C, RESP_TYPE = 0x28;

void installThreadStruct() {
    const uint32_t tld = rw::call(THREAD_LOCAL_DATA, {});
    const uint32_t ts = guest::newObject(4);
    rw::call(THREAD_STRUCT_CTOR, {ts});
    rw::call(THREAD_SPECIFIC_RESET, {tld, ts});
}

uint32_t strSize(uint32_t s) { return (load<uint8_t>(s) & 1) ? load<uint32_t>(s + 4) : uint32_t(load<uint8_t>(s) >> 1); }
uint32_t strData(uint32_t s) { return (load<uint8_t>(s) & 1) ? load<uint32_t>(s + 8) : s + 1; }

// [this] { startNextDownloadThread(); processing = false; } (captures: the manager)
uint32_t nextDownloadThread(uint32_t captures) {
    installThreadStruct();
    const uint32_t m = load<uint32_t>(captures);
    rw::call(START_NEXT_DOWNLOAD, {m});
    save<uint8_t>(m + RDM_PROCESSING, 0);
    guest::deleteObject(captures);
    return 0;
}

// The answer to a piece (captures: the HttpResponse, the manager): the body must have the piece's size and md5 and
// the content type must be application/octet-stream (else error 1); it is written to the piece's cache file
// (error 2 when that fails). On success the piece is popped and the next download started.
uint32_t pieceResponseThread(uint32_t captures) {
    installThreadStruct();
    const uint32_t m = load<uint32_t>(captures + 4);
    guest::Frame frame(0x30);
    const uint32_t type = frame.addr(0x0), md5 = frame.addr(0xC), path = frame.addr(0x18), pred = frame.addr(0x24);
    const uint32_t response = load<uint32_t>(captures);
    rw::call(STRING_INIT_RANGE, {type, load<uint32_t>(response + RESP_TYPE), load<uint32_t>(response + RESP_TYPE + 4)});
    const uint32_t start = load<uint32_t>(m + RDM_ITEM_START);
    const uint32_t item = load<uint32_t>(load<uint32_t>(load<uint32_t>(m + RDM_ITEM_MAP_BEGIN) + start / 1024 * 4) + start % 1024 * 4);
    const uint32_t body = load<uint32_t>(response + RESP_BODY), bodySize = load<uint32_t>(response + RESP_BODY + 4) - body;
    if (load<uint32_t>(item + ITEM_SIZE) != bodySize) {
        save<uint32_t>(m + RDM_ERROR, 1);
    } else {
        rw::call(MD5_CALCULATE, {md5, body, bodySize});
        bool differs = true;
        const uint32_t expected = item + ITEM_MD5;
        uint32_t n = strSize(expected);
        if (n == strSize(md5)) {
            const uint32_t a = strData(expected), b = strData(md5);
            if (load<uint8_t>(expected) & 1) {
                differs = rw::call(MEMCMP, {a, b, n}) != 0;
            } else {
                uint32_t k = 0;
                while (k < n && load<uint8_t>(a + k) == load<uint8_t>(b + k)) k++;
                differs = k < n;
            }
        }
        libcxx::destroyString(md5);
        if (differs) {
            save<uint32_t>(m + RDM_ERROR, 1);
        } else {
            const uint32_t typeSize = strSize(type), typeData = strData(type);
            bool found = false;
            if (typeSize >= 0x18) {
                save<uint32_t>(pred, CHAR_PRED);
                const uint32_t at = rw::call(SEARCH_PRED, {typeData, typeData + typeSize, OCTET_STREAM, OCTET_STREAM + 0x18, pred});
                found = at != typeData + typeSize && at - typeData != 0xFFFFFFFFu;
            }
            if (!found) {
                save<uint32_t>(m + RDM_ERROR, 1);
            } else {
                bool written = false;
                const uint32_t cache = rw::call(CACHE_GET_INSTANCE, {});
                rw::call(CACHE_FILE_PATH, {path, cache, item});
                const uint32_t file = rw::call(FOPEN, {strData(path), LIT_WB});
                if (file) {
                    const uint32_t wrote = rw::call(FWRITE, {load<uint32_t>(response + RESP_BODY),
                                                             load<uint32_t>(response + RESP_BODY + 4) - load<uint32_t>(response + RESP_BODY), 1, file});
                    rw::call(FCLOSE, {file});
                    written = wrote == 1;
                }
                libcxx::destroyString(path);
                if (!written) save<uint32_t>(m + RDM_ERROR, 2);
            }
        }
    }
    rw::call(REF_RELEASE, {load<uint32_t>(captures)});
    if (!load<uint32_t>(m + RDM_ERROR)) {
        save<uint32_t>(m + RDM_PIECE_TOTAL, 0);
        save<uint32_t>(m + RDM_DONE, load<uint32_t>(m + RDM_DONE) + load<uint32_t>(item + ITEM_SIZE));
        rw::call(ITEMS_POP_FRONT, {m + RDM_ITEMS});
        save<uint8_t>(m + RDM_PROCESSING, 1);
        rw::call(START_NEXT_DOWNLOAD, {m});
        save<uint8_t>(m + RDM_PROCESSING, 0);
    }
    libcxx::destroyString(type);
    guest::deleteObject(captures);
    return 0;
}

}  // namespace

RW_FUNCTION_NOVERIFY(0x775658, "ResourceDataManager next-download thread (__thread_proxy, sub_775658)", nextDownloadThread);
RW_FUNCTION_NOVERIFY(0x774A1C, "ResourceDataManager piece-response thread (__thread_proxy, sub_774A1C)", pieceResponseThread);
