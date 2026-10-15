#include "Random.hpp"

namespace {
constexpr uint32_t NOW_MICROS = 0xC60645;        // sub_C60644(uint64_t *out): gettimeofday in microseconds
constexpr uint32_t UDIVSI3 = 0xCD5DF9;           // __udivsi3 (used for a zero divisor)
constexpr uint32_t GOT_SAFEBUFFER_DTOR = 0xFD6104;   // &SafeBuffer::~SafeBuffer
constexpr uint32_t GOT_RANDOM_VTABLE = 0xFD6108;     // &_ZTV6Random
constexpr uint32_t GOT_DSO_HANDLE = 0xFD604C;        // &__dso_handle

constexpr uint32_t LCG_MUL = 1103515245u, LCG_ADD = 12345u;

uint32_t udiv(uint32_t a, uint32_t b) { return b ? a / b : rw::call(UDIVSI3, {a, b}); }
}  // namespace

Random *Random::getInstance() {
    if (guest::staticInitBegin(INSTANCE_GUARD)) {
        construct(rw::host_ptr<Random>(INSTANCE));
        rw::call(guest::CXA_ATEXIT, {rw::load<uint32_t>(GOT_SAFEBUFFER_DTOR), INSTANCE, rw::load<uint32_t>(GOT_DSO_HANDLE)});
        guest::staticInitDone(INSTANCE_GUARD);
    }
    return rw::host_ptr<Random>(INSTANCE);
}
RW_FUNCTION(0x492054, "Random::getInstance", Random::getInstance);

Random *Random::construct(Random *self) {
    SafeBuffer::construct(self, 4, 1);
    self->vtable = rw::load<uint32_t>(GOT_RANDOM_VTABLE) + 8;
    guest::Frame f(16);
    rw::call(NOW_MICROS, {f.addr()});
    uint32_t seed = rw::load<uint32_t>(f.addr());   // low word of the microsecond clock
    SafeBuffer::write(self, self->data.get(), reinterpret_cast<const uint8_t *>(&seed), 4, 0);
    return self;
}
RW_FUNCTION(0x4920EC, "Random::Random", Random::construct);

uint32_t Random::getMaxInt(Random *) { return MAX; }
RW_FUNCTION(0x492184, "Random::getMaxInt", Random::getMaxInt);

uint32_t Random::generate(Random *self, const uint32_t *seed) {
    uint32_t next;
    if (seed) {
        next = LCG_MUL * *seed + LCG_ADD;
        rw::store32(seed, next);
    } else {
        next = LCG_MUL * uint32_t(self->get32(0)) + LCG_ADD;
        SafeBuffer::write(self, self->data.get(), reinterpret_cast<const uint8_t *>(&next), 4, 0);
    }
    return next & MAX;
}
RW_FUNCTION(0x49218C, "Random::generate", Random::generate);

int32_t Random::generateUniformInt(Random *self, int32_t lo, int32_t hi, const uint32_t *seed) {
    uint32_t bucket = udiv(MAX, 1u - uint32_t(lo) + uint32_t(hi));
    uint32_t r = generate(self, seed);
    return int32_t(udiv(r, bucket + 1) + uint32_t(lo));
}
RW_FUNCTION(0x492224, "Random::generateUniformInt", Random::generateUniformInt);

float Random::generateUniformFloat(Random *self, float lo, float hi, const uint32_t *seed) {
    float r = float(int32_t(generate(self, seed)));
    float span = hi - lo;
    float scaled = span * r;
    scaled = scaled * rw::bitsf(0x30000000u);   // 2^-31
    return scaled + lo;
}
RW_FUNCTION(0x492260, "Random::generateUniformFloat", Random::generateUniformFloat);

void Random::deletingDestroy(Random *self) { guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self))); }
RW_FUNCTION(0x4922A0, "Random::~Random (deleting)", Random::deletingDestroy);
