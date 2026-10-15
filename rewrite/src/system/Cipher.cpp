// Cipher: AES-256-CBC with a zero IV and PKCS#7 padding (OpenSSL), the key kept in a SafeBuffer (Cipher::Info at
// +4). Used for the master data and the save files.
#include "core/SafeBuffer.hpp"
#include "SystemCommon.hpp"

namespace cipher {

namespace {
constexpr uint32_t AES_SET_ENCRYPT_KEY = 0xB8DE18;   // AES_set_encrypt_key(key, bits, schedule)
constexpr uint32_t AES_SET_DECRYPT_KEY = 0xB8DE1C;   // AES_set_decrypt_key(key, bits, schedule)
constexpr uint32_t AES_CBC_ENCRYPT = 0xB8EF88;       // AES_cbc_encrypt(in, out, len, schedule, iv, enc)
constexpr uint32_t MEMCPY = 0x40E164;                // memcpy (import)
constexpr uint32_t MEMSET = 0x40E7A0;                // memset (import)
constexpr uint32_t MAKE_SHARED_DATA = 0x4D7929;      // make_shared<bg::Data>(out, unsigned char*&, unsigned&, void(&)(void*))
constexpr uint32_t GOT_FREE = 0xFD64C0;              // &free
constexpr uint32_t GOT_INFO_VTABLE = 0xFD64F4;       // &_ZTVN6Cipher4InfoE
constexpr uint32_t GOT_STACK_GUARD = 0xFD6044;       // &__stack_chk_guard

enum Operation : uint32_t { ENCRYPT = 0, DECRYPT = 1 };
}  // namespace

// Cipher { vtable? +0; Info key at +4 (a SafeBuffer holding the 32 key bytes) }
struct Cipher {
    rw::Field<uint32_t> field_0;   // +0
    SafeBuffer key;                // +4 Cipher::Info

    static Cipher *construct(Cipher *self, const uint8_t *key32);
    // crypt(Operation, data, size) -> shared_ptr<bg::Data> (sret); a decrypt of a size that is not a multiple of
    // 16 gives an empty pointer.
    static void crypt(guest::SharedPtr *out, Cipher *self, uint32_t op, uint32_t data, uint32_t size);
    static void encrypt(guest::SharedPtr *out, Cipher *self, uint32_t data, uint32_t size);
    static void decrypt(guest::SharedPtr *out, Cipher *self, uint32_t data, uint32_t size);
    static void infoDeletingDestroy(SafeBuffer *info);
};
RW_LAYOUT(Cipher, key, 4);

void Cipher::crypt(guest::SharedPtr *out, Cipher *self, uint32_t op, uint32_t data, uint32_t size) {
    // the original frame: outLen +0x2C, outPtr +0x30, iv +0x34, key schedule +0x44, key +0x138
    guest::Frame f(0x160);
    const uint32_t outLen = f.addr(0x2C), outPtr = f.addr(0x30), iv = f.addr(0x34), sched = f.addr(0x44),
                   key = f.addr(0x138);
    rw::save<uint32_t>(f.addr(0x28), rw::load<uint32_t>(rw::load<uint32_t>(GOT_STACK_GUARD)));   // canary slot
    const SafeBuffer *info = SafeBuffer::read(&self->key, rw::host_ptr<uint8_t>(key), self->key.data.get(), 32, 0);
    uint32_t enc = 0;
    uint32_t in, len;
    if (op == DECRYPT) {
        if (size & 0xF) {
            out->obj = 0;
            out->ctrl = 0;
            return;
        }
        rw::call(AES_SET_DECRYPT_KEY, {key, 256, sched});
        len = size;
        in = data;
    } else if (op == ENCRYPT) {
        enc = 1;
        rw::call(AES_SET_ENCRYPT_KEY, {key, 256, sched});
        uint32_t pad = 16 - (size & 0xF);
        len = pad + size;
        in = rw::call(sys::lib::MALLOC, {len});
        rw::call(MEMCPY, {in, data, size});
        rw::call(MEMSET, {in + size, pad, pad});
    } else {
        // (not an Operation: the original encrypts its own stack slot with the read's result as the flag)
        enc = rw::guest_addr(info);
        len = op;
        in = f.addr(0x28);
    }
    uint32_t buf = rw::call(sys::lib::MALLOC, {len});
    rw::save<uint32_t>(outPtr, buf);
    for (uint32_t k = 0; k < 16; k += 4) rw::save<uint32_t>(iv + k, 0);
    rw::call(AES_CBC_ENCRYPT, {in, buf, len, sched, iv, enc});
    if (op == DECRYPT) {
        rw::save<uint32_t>(outLen, len - rw::load<uint8_t>(buf + len - 1));   // strip the padding
    } else if (op == ENCRYPT) {
        rw::save<uint32_t>(outLen, len);
        rw::call(sys::lib::FREE, {in});
    }
    rw::call(MAKE_SHARED_DATA, {rw::guest_addr(out), outPtr, outLen, rw::load<uint32_t>(GOT_FREE)});
}
RW_FUNCTION(0x4E0520, "Cipher::crypt", Cipher::crypt);

Cipher *Cipher::construct(Cipher *self, const uint8_t *key32) {
    SafeBuffer::construct(&self->key, 32, 0);
    self->key.vtable = rw::load<uint32_t>(GOT_INFO_VTABLE) + 8;
    SafeBuffer::write(&self->key, self->key.data.get(), key32, 32, 0);
    return self;
}
RW_FUNCTION(0x4E0670, "Cipher::Cipher(const std::array<unsigned char, 32>&)", Cipher::construct);

void Cipher::encrypt(guest::SharedPtr *out, Cipher *self, uint32_t data, uint32_t size) {
    crypt(out, self, ENCRYPT, data, size);
}
RW_FUNCTION(0x4E06D8, "Cipher::encrypt", Cipher::encrypt);

void Cipher::decrypt(guest::SharedPtr *out, Cipher *self, uint32_t data, uint32_t size) {
    crypt(out, self, DECRYPT, data, size);
}
RW_FUNCTION(0x4E06F2, "Cipher::decrypt", Cipher::decrypt);

void Cipher::infoDeletingDestroy(SafeBuffer *info) {
    guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(info)));
}
RW_FUNCTION(0x4E070C, "Cipher::Info::~Info (deleting)", Cipher::infoDeletingDestroy);

}  // namespace cipher
