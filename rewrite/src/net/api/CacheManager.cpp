#include "CacheManager.hpp"

namespace hole::network {

using api::GuestFrame;
using api::SharedPtr;
using api::StdString;

namespace {
constexpr uint32_t FileUtils_getInstance = rwlib::cocos2d::FileUtils::getInstance;  // cocos2d::FileUtils::getInstance()
constexpr uint32_t FileUtils_getWritablePath = 18;    // vtable slot (+0x48), returns a std::string (hidden r0)
constexpr uint32_t new_array = 0xC98F6F;              // operator new[]
constexpr uint32_t delete_array = 0xC98FB3;           // operator delete[]
constexpr uint32_t string_append = 0x47AC89;          // std::string::append(const char*, size_t), returns this
constexpr uint32_t CONVERT = 0x4D7585;                // CacheManager::convertURLToCacheFilePath
constexpr uint32_t InputStreamFactory_open = 0x8DC555;  // bg::InputStreamFactory::open(const char*, bool) (sret)
constexpr uint32_t md5_calculate = 0x4E0A89;          // md5::calculate(const void*, unsigned) -> std::string (sret)
constexpr uint32_t Data_make_shared = 0x4D7929;       // shared_ptr<bg::Data>::make_shared(unsigned char*&, unsigned&, free)
constexpr uint32_t GOT_free = 0xFD64C0;               // GOT slot of free
constexpr uint32_t malloc_ = 0x40FF58;                // malloc (import)
constexpr uint32_t free_ = 0x40FF64;                  // free (import)
constexpr uint32_t memcmp_ = 0x40EF2C;                // memcmp (import)
constexpr uint32_t remove_ = 0x413C84;                // remove (import)
// InputStream vtable slots
constexpr uint32_t Stream_read = 2;                   // read(void*, unsigned, int) -> 1 when complete
constexpr uint32_t Stream_isOpen = 5;
constexpr uint32_t Stream_size = 6;
}  // namespace

uint32_t CacheManager::getInstance() { return INSTANCE; }
RW_FUNCTION(0x4D7570, "hole::network::CacheManager::getInstance", CacheManager::getInstance);

uint32_t CacheManager::convertURLToCacheFilePath(StdString *out, uint32_t, const StdString *url) {
    GuestFrame frame(0x50);                   // original frame: the hex name at +0x20, the writable path at +0x2c
    uint32_t files = rw::call(FileUtils_getInstance, {});
    rw::vcall(rw::host_ptr<uint32_t>(files), FileUtils_getWritablePath, {frame.addr(0x2C), files});

    uint32_t n = url->size();
    uint32_t src = url->data();
    uint32_t hexChars = rw::call(new_array, {2 * n});
    uint32_t key = 0x6D5379;
    for (uint32_t k = 0; k < n; k++) {
        key = key * 214013u + 2531011u;
        uint8_t b = uint8_t(rw::load<uint8_t>(src + k) ^ key);
        uint8_t hi = b >> 4, lo = b & 0xF;
        rw::store8(rw::host_ptr<uint8_t>(hexChars + 2 * k), uint8_t(hi >= 10 ? hi + 0x57 : hi | 0x30));
        rw::store8(rw::host_ptr<uint8_t>(hexChars + 2 * k + 1), uint8_t(lo >= 10 ? lo + 0x57 : lo | 0x30));
    }
    rw::call(api::guest::string_init, {frame.addr(0x20), hexChars, 2 * n});
    rw::call(delete_array, {hexChars});

    // *out = std::move(path.append(hex))
    StdString *hex = frame.at<StdString>(0x20);
    bool hexLong = hex->isLong();
    StdString *joined = rw::host_ptr<StdString>(rw::call(string_append, {frame.addr(0x2C), hex->data(), hex->size()}));
    out->w0 = joined->w0.get();
    out->w1 = joined->w1.get();
    out->w2 = joined->w2.get();
    joined->clear();
    if (hexLong) api::guestDelete(hex->w2);
    StdString *path = frame.at<StdString>(0x2C);
    if (path->isLong()) api::guestDelete(path->w2);   // never: moved out above
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4D7584, "hole::network::CacheManager::convertURLToCacheFilePath",
            CacheManager::convertURLToCacheFilePath);

uint32_t CacheManager::validate(SharedPtr *out, uint32_t, const StdString *url, uint32_t size, const StdString *md5) {
    // original frame 0x58: the md5 string at +0x18, the buffer at +0x24, the stream size at +0x28, the stream
    // shared_ptr at +0x2c, the path at +0x34
    GuestFrame frame(0x58);
    rw::call(CONVERT, {frame.addr(0x34), 0xFFFFCBF8u, rw::guest_addr(url)});   // r1 holds a GOT offset here
    StdString *path = frame.at<StdString>(0x34);
    bool pathLong = path->isLong();
    rw::call(InputStreamFactory_open, {frame.addr(0x2C), path->data(), 0});
    SharedPtr *stream = frame.at<SharedPtr>(0x2C);
    uint32_t *bufSlot = frame.at<uint32_t>(0x24);
    uint32_t *sizeSlot = frame.at<uint32_t>(0x28);

    bool made = false;
    uint32_t s = stream->ptr;
    if (s && rw::vcall(rw::host_ptr<uint32_t>(s), Stream_isOpen, {s})) {
        *sizeSlot = rw::vcall(rw::host_ptr<uint32_t>(s), Stream_size, {s});
        if (*sizeSlot == size) {
            uint32_t buf = rw::call(malloc_, {size + 1});
            *bufSlot = buf;
            if (rw::vcall(rw::host_ptr<uint32_t>(s), Stream_read, {s, buf, size, 1}) != 1) {
                rw::call(free_, {buf});
            } else {
                rw::store8(rw::host_ptr<uint8_t>(buf + size), 0);
                rw::call(md5_calculate, {frame.addr(0x18), buf, size});
                StdString *sum = frame.at<StdString>(0x18);
                bool sumLong = sum->isLong();
                bool differ = true;
                uint32_t n = md5->size();
                if (n == sum->size()) {
                    uint32_t a = md5->data(), b = sum->data();
                    if (md5->isLong()) {
                        differ = rw::call(memcmp_, {a, b, n}) != 0;
                    } else {
                        differ = false;
                        for (uint32_t k = 0; k < n; k++) {
                            if (rw::load<uint8_t>(a + k) != rw::load<uint8_t>(b + k)) { differ = true; break; }
                        }
                    }
                }
                if (sumLong) api::guestDelete(sum->w2);
                if (!differ) {
                    rw::call(Data_make_shared, {rw::guest_addr(out), frame.addr(0x24), frame.addr(0x28),
                                                rw::load<uint32_t>(GOT_free)});
                    made = true;
                }
            }
        }
    }
    if (!made) out->reset();
    if (stream->ctrl) rw::call(api::guest::shared_release, {stream->ctrl});
    if (pathLong) api::guestDelete(path->w2);
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4D76F0, "hole::network::CacheManager::validate", CacheManager::validate);

uint32_t CacheManager::removeCache(uint32_t self, const StdString *url) {
    GuestFrame frame(0x28);                   // original frame: the path at +8
    rw::call(CONVERT, {frame.addr(8), self, rw::guest_addr(url)});
    StdString *path = frame.at<StdString>(8);
    if (path->isLong()) {
        uint32_t p = path->w2;
        rw::call(remove_, {p});
        api::guestDelete(p);
    } else {
        rw::call(remove_, {frame.addr(9)});
    }
    return 0;   // stack-protector check
}
// deletes a file: not run twice by the verifier
RW_FUNCTION_NOVERIFY(0x4D78C0, "hole::network::CacheManager::removeCache", CacheManager::removeCache);

}  // namespace hole::network
