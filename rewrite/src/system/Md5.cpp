// md5: hex MD5 digests of a memory block or a file (OpenSSL's MD5), used to check downloads and requests.
#include "StringBuffer.hpp"

namespace md5 {

namespace {
constexpr uint32_t MD5 = 0xB80CF0;           // MD5(data, len, digest[16])
constexpr uint32_t MD5_INIT = 0xB7FE10;      // MD5_Init(ctx)
constexpr uint32_t MD5_UPDATE = 0xB80A8C;    // MD5_Update(ctx, data, len)
constexpr uint32_t MD5_FINAL = 0xB80BA4;     // MD5_Final(digest, ctx)
constexpr uint32_t STRLEN = 0x40E08C;        // strlen (import)
constexpr uint32_t FOPEN = 0x414158;         // fopen (import)
constexpr uint32_t FCLOSE = 0x414170;        // fclose (import)
constexpr uint32_t FREAD = 0x414308;         // fread (import)
constexpr uint32_t STRING_INIT = 0x47CD93;   // sub_47CD92: std::string(this)::__init(const char*, size)
constexpr uint32_t STRING_ASSIGN = 0x48D811; // sub_48D810: std::string::assign(const char*, size)
constexpr uint32_t HEX_BYTE = 0xDFC3A1;      // "%02x"
constexpr uint32_t MODE_RB = 0xDFC3A6;       // "rb"
constexpr uint32_t FCONCAT = 0x491571;       // StringBuffer::fconcat(fmt, ...)

// The 16 digest bytes as 32 lowercase hex digits in the StringBuffer at buf; returns its strlen.
uint32_t hexDigest(uint32_t buf, uint32_t digest) {
    rw::save<uint32_t>(buf + StringBuffer::CAPACITY, 0);
    rw::save<uint8_t>(buf, 0);
    for (uint32_t k = 0; k < 16; k++) rw::call(FCONCAT, {buf, HEX_BYTE, rw::load<uint8_t>(digest + k)});
    return rw::call(STRLEN, {buf});
}
}  // namespace

// md5::calculate(const void* data, size_t len) -> std::string (sret)
void calculate(guest::String *out, uint32_t data, uint32_t len) {
    // the original frame (5 saved registers + 0x424): MD5's own frame must sit where it did, since OPENSSL_cleanse
    // folds the address of what it wipes into a global (cleanse_ctr)
    guest::Frame f(0x438);
    const uint32_t buf = f.addr(0xC), digest = f.addr(0x410);
    rw::call_exact(MD5, {data, len, digest});
    uint32_t n = hexDigest(buf, digest);
    rw::call(STRING_INIT, {rw::guest_addr(out), buf, n});
}
RW_FUNCTION(0x4E0A88, "md5::calculate(const void*, unsigned)", calculate);

// md5::calculate(const char* path, std::string& out, unsigned* size): 1 when the file was read.
uint32_t calculateFile(uint32_t path, guest::String *out, const uint32_t *size) {
    guest::Frame f(0x884);
    const uint32_t buf = f.addr(0x10), digest = f.addr(0x414), chunk = f.addr(0x424), ctx = f.addr(0x824);
    uint32_t fp = rw::call(FOPEN, {path, MODE_RB});
    if (!fp) return 0;
    rw::call(MD5_INIT, {ctx});
    if (size) rw::store32(size, 0);
    for (;;) {
        uint32_t n = rw::call(FREAD, {chunk, 1, 1024, fp});
        if (!n) break;
        if (rw::call(MD5_UPDATE, {ctx, chunk, n}) != 1) {
            rw::call(FCLOSE, {fp});
            return 0;
        }
        if (size) rw::store32(size, *size + n);
    }
    rw::call(FCLOSE, {fp});
    rw::call(MD5_FINAL, {digest, ctx});
    uint32_t n = hexDigest(buf, digest);
    rw::call(STRING_ASSIGN, {rw::guest_addr(out), buf, n});
    return 1;
}
RW_FUNCTION(0x4E0B28, "md5::calculate(const char*, std::string&, unsigned*)", calculateFile);

}  // namespace md5
