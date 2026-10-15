#include "StringBuffer.hpp"

namespace {
uint32_t vsnprintf(uint32_t dst, uint32_t size, uint32_t fmt, uint32_t va) {
    return rw::call(sys::lib::VSNPRINTF, {dst, size, fmt, va});
}
}  // namespace

StringBuffer *StringBuffer::construct(StringBuffer *self, uint32_t fmt) {
    sys::VaSpill va(2);
    self->length = 0;
    self->length = int32_t(vsnprintf(sys::A(self), CAPACITY, fmt, va.va()));
    return self;
}
RW_FUNCTION(0x49140C, "StringBuffer::StringBuffer(const char*, ...)", StringBuffer::construct);

StringBuffer *StringBuffer::vfset(StringBuffer *self, uint32_t fmt, uint32_t va) {
    self->length = int32_t(vsnprintf(sys::A(self), CAPACITY, fmt, va));
    return self;
}
RW_FUNCTION(0x491480, "StringBuffer::vfset", StringBuffer::vfset);

StringBuffer *StringBuffer::set(StringBuffer *self, uint32_t str) {
    self->length = 0;
    concat(self, str);
    return self;
}
RW_FUNCTION(0x4914A6, "StringBuffer::set", StringBuffer::set);

StringBuffer *StringBuffer::concat(StringBuffer *self, uint32_t str) {
    uint32_t len = uint32_t(int32_t(self->length));
    // at most CAPACITY - 1 - length characters
    for (uint32_t left = CAPACITY - 1 - len; left != 0; left--) {
        uint8_t ch = rw::load<uint8_t>(str++);
        if (!ch) break;
        rw::store8(self->text + len, ch);
        len = uint32_t(int32_t(self->length)) + 1;
        self->length = int32_t(len);
    }
    rw::store8(self->text + len, 0);
    return self;
}
RW_FUNCTION(0x4914C4, "StringBuffer::concat", StringBuffer::concat);

StringBuffer *StringBuffer::fset(StringBuffer *self, uint32_t fmt) {
    sys::VaSpill va(2);
    self->length = int32_t(vsnprintf(sys::A(self), CAPACITY, fmt, va.va()));
    return self;
}
RW_FUNCTION(0x491500, "StringBuffer::fset", StringBuffer::fset);

StringBuffer *StringBuffer::fconcat(StringBuffer *self, uint32_t fmt) {
    sys::VaSpill va(2);
    uint32_t len = uint32_t(int32_t(self->length));
    uint32_t n = vsnprintf(sys::A(self) + len, CAPACITY - len, fmt, va.va());
    self->length = int32_t(uint32_t(int32_t(self->length)) + n);
    return self;
}
RW_FUNCTION(0x491570, "StringBuffer::fconcat", StringBuffer::fconcat);

StringBuffer *StringBuffer::vfconcat(StringBuffer *self, uint32_t fmt, uint32_t va) {
    uint32_t len = uint32_t(int32_t(self->length));
    uint32_t n = vsnprintf(sys::A(self) + len, CAPACITY - len, fmt, va);
    self->length = int32_t(uint32_t(int32_t(self->length)) + n);
    return self;
}
RW_FUNCTION(0x4915E0, "StringBuffer::vfconcat", StringBuffer::vfconcat);

void StringBuffer::replace(StringBuffer *self, uint32_t index, uint32_t c) {
    rw::store8(self->text + index, uint8_t(c));
}
RW_FUNCTION(0x491608, "StringBuffer::replace", StringBuffer::replace);

namespace {
// sub_49160E: an outlined malloc(size) call.
uint32_t mallocThunk(uint32_t size) { return rw::call(sys::lib::MALLOC, {size}); }
}  // namespace
RW_FUNCTION(0x49160E, "sub_49160E (malloc thunk)", mallocThunk);
