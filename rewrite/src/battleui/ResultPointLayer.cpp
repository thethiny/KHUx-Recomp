#include "ResultPointLayer.hpp"

using namespace scene;

namespace {
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;   // std::function<void()>::operator=(const function&)
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t StringBuffer_ctor = 0x49140D;   // StringBuffer(this, fmt, ...)
constexpr uint32_t STR_d = 0xDF86DE;               // "%d"
// libgcc conversions between float and uint64 (called rather than re-derived: their rounding and the
// saturation of out-of-range values are the guest's)
constexpr uint32_t floatundisf = 0xCD6D88;         // (uint64) -> float
constexpr uint32_t fixunssfdi = 0xCD7429;          // (float) -> uint64

float u64ToFloat(uint32_t lo, uint32_t hi) { return rw::callf(floatundisf, {lo, hi}); }
uint64_t floatToU64(float f) { return rw::call64(fixunssfdi, {rw::fbits(f)}); }

// label->setString(text)
void setLabel(uint32_t label, uint32_t text) {
    Frame f;
    uint32_t s = newStr(f, text);
    call(Text_setString, {label, s});
    strFree(s);
}
}  // namespace

void ResultPointLayer::update(ResultPointLayer *self, float dt, uint32_t withComma) {
    if (!self->running || self->finished) return;
    float t = self->elapsed + dt;
    self->elapsed = t;
    if (t <= self->duration || t != t || self->duration != self->duration) updateLabel(self, withComma);
    else skipUpdate(self, withComma);
}
RW_FUNCTION(0x57DEBC, "ResultPointLayer::update", ResultPointLayer::update);

void ResultPointLayer::set(ResultPointLayer *self, uint32_t label, uint64_t add, float duration, uint32_t onDone) {
    self->label = label;
    self->baseLo = 0;
    self->baseHi = 0;
    self->addLo = 0;
    self->addHi = 0;
    Frame f;
    uint32_t copy = f.alloc(24);
    call(fn::function_vv_copy, {copy, onDone});
    uint64_t base = (uint64_t(self->baseHi) << 32 | self->baseLo) + (uint64_t(self->addHi) << 32 | self->addLo);
    self->baseHi = uint32_t(base >> 32);
    self->baseLo = uint32_t(base);
    self->running = 0;
    self->finished = 0;
    self->addHi = uint32_t(add >> 32);
    self->addLo = uint32_t(add);
    self->elapsed = 0.0f;
    self->duration = duration;
    call(FunctionVV_assign, {rw::guest_addr(self->onDone), copy});
    functionDestroy(copy);
}
RW_FUNCTION(0x57F860, "ResultPointLayer::set", ResultPointLayer::set);

void ResultPointLayer::updateLabel(ResultPointLayer *self, uint32_t withComma) {
    uint32_t label = self->label;
    if (!label) return;
    float base = u64ToFloat(self->baseLo, self->baseHi);
    float add = u64ToFloat(self->addLo, self->addHi);
    uint64_t value = floatToU64(base + add * self->elapsed / self->duration);
    uint32_t lo = uint32_t(value), hi = uint32_t(value >> 32);
    if (withComma == 1) {
        // "1,234,567"
        Frame f;
        uint32_t money = f.alloc(12);
        SceneUtil::getMoneyStr(money, value);
        setLabel(label, strData(money));
        strFree(money);
    } else {
        Frame f;
        uint32_t buf = stringBuffer(f, {STR_d, lo, hi});
        setLabel(label, buf);
    }
}
RW_FUNCTION(0x57F930, "ResultPointLayer::updateLabel", ResultPointLayer::updateLabel);

void ResultPointLayer::skipUpdate(ResultPointLayer *self, uint32_t withComma) {
    self->elapsed = self->duration;
    self->finished = 1;
    updateLabel(self, withComma);
    if (self->onDone_f) call(fn::function_vv_call, {rw::guest_addr(self->onDone)});
}
RW_FUNCTION(0x584AF0, "ResultPointLayer::skipUpdate", ResultPointLayer::skipUpdate);
