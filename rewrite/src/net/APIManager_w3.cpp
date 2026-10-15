// APIManager.cpp's file-local helper: encrypt a block with the request Cipher and return it base64-encoded as a
// std::string (the encrypted login / user-start parameters).
#include "core/Guest.hpp"

namespace {

constexpr uint32_t CIPHER_ENCRYPT = 0x4E06D9;      // Cipher::encrypt(const void*, size_t) -> shared_ptr<bg::Data>
constexpr uint32_t BASE64_ENCODE = 0x905B2D;       // base64Encode(const uint8_t*, size_t, char** out) (malloc'ed)
constexpr uint32_t STRLEN = 0x40E08C;              // strlen (import)
constexpr uint32_t FREE = 0x40FF64;                // free (import)
constexpr uint32_t STRING_INIT = 0x47CD93;         // std::string::__init(const char*, size_t)
constexpr uint32_t STRING_RESERVE = 0x48D705;      // std::string::reserve(size_t)

// bg::Data: vtable, bytes, size
constexpr uint32_t DATA_BYTES = 4, DATA_SIZE = 8;

// The original frame: 5 pushed registers + 0x24 bytes of locals (Cipher::encrypt reaches OpenSSL, which folds
// stack addresses into a global, so callee frames must sit where they did).
constexpr uint32_t FRAME_SIZE = 20 + 0x24;
constexpr uint32_t TMP_STRING = 0x08;       // std::string built from the base64 text
constexpr uint32_t ENCRYPTED = 0x14;        // shared_ptr<bg::Data> (object, control block)
constexpr uint32_t BASE64 = 0x1C;           // char* filled by base64Encode

uint32_t ld(uint32_t a) { return rw::load<uint32_t>(a); }

}  // namespace

namespace APIManagerHelpers {

// out = base64(cipher.encrypt(data, length)); empty when the encryption or the encoding failed. r0 = 0 (the
// stack-guard difference the original leaves in r0).
uint32_t encryptToBase64(const guest::String *out, uint32_t cipher, uint32_t data, uint32_t length) {
    guest::Frame f(FRAME_SIZE);
    uint32_t sp = f.addr();
    uint32_t o = rw::guest_addr(out);
    rw::save<uint32_t>(o, 0);
    rw::save<uint32_t>(o + 4, 0);
    rw::save<uint32_t>(o + 8, 0);
    rw::save<uint32_t>(sp + BASE64, 0);
    rw::call_exact(CIPHER_ENCRYPT, {sp + ENCRYPTED, cipher, data, length});
    uint32_t text = 0;
    uint32_t encrypted = ld(sp + ENCRYPTED);
    if (encrypted) {
        rw::save<uint32_t>(sp, sp + TMP_STRING + 4);   // the original's spilled pointer (fifth-argument slot)
        rw::call_exact(BASE64_ENCODE, {ld(encrypted + DATA_BYTES), ld(encrypted + DATA_SIZE), sp + BASE64});
        uint32_t b64 = ld(sp + BASE64);
        if (b64) {
            uint32_t n = uint32_t(rw::call_exact(STRLEN, {b64}));
            rw::call_exact(STRING_INIT, {sp + TMP_STRING, b64, n});
            // out = std::move(tmp): clear out (short, empty), shrink it, take tmp's words, leave tmp empty
            rw::save<uint8_t>(o + 1, 0);
            rw::save<uint8_t>(o, 0);
            rw::call_exact(STRING_RESERVE, {o, 0});
            uint32_t w0 = ld(sp + TMP_STRING), w1 = ld(sp + TMP_STRING + 4), w2 = ld(sp + TMP_STRING + 8);
            rw::save<uint32_t>(o, w0);
            rw::save<uint32_t>(o + 4, w1);
            rw::save<uint32_t>(o + 8, w2);
            rw::save<uint32_t>(sp + TMP_STRING + 4, 0);
            rw::save<uint32_t>(sp + TMP_STRING + 8, 0);
            rw::save<uint32_t>(sp + TMP_STRING, 0);
            text = ld(sp + BASE64);
        }
    }
    rw::call_exact(FREE, {text});
    uint32_t ctrl = ld(sp + ENCRYPTED + 4);
    if (ctrl) rw::call_exact(guest::SHARED_RELEASE, {ctrl});
    return 0;
}

}  // namespace APIManagerHelpers

RW_FUNCTION(0x4C678C, "APIManager.cpp encryptToBase64(std::string*, Cipher&, const void*, size_t)",
            APIManagerHelpers::encryptToBase64);
