// sub_4B6DCC: std::string::replace(size_type pos, size_type n1, const char* s, size_type n2), a libc++ instance
// emitted next to the network code (used by the shop's URL building). In place when the capacity allows (handling a
// source inside the string itself), else through __grow_by_and_replace.
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t MEMMOVE = 0x40EF50;                    // memmove (import)
constexpr uint32_t String_growByAndReplace = 0x47AD19;    // __grow_by_and_replace(old_cap, delta, old_sz, pos, del, add, s)
constexpr uint32_t String_throwOutOfRange = 0x47CC75;     // (does not return)

void memmoveGuest(uint32_t dst, uint32_t src, uint32_t n) { call(MEMMOVE, {dst, src, n}); }

uint32_t stringReplace(uint32_t self, uint32_t pos, uint32_t n1, uint32_t s, uint32_t n2) {
    const uint32_t head = ld8(self);
    const bool isLong = (head & 1) != 0;
    const uint32_t size = isLong ? ld32(self + 4) : head >> 1;
    if (size < pos) call(String_throwOutOfRange, {self});
    const uint32_t after = size - pos;
    if (after < n1) n1 = after;
    const uint32_t cap = isLong ? (ld32(self) & ~1u) - 1 : 10;
    if (n1 - size + cap < n2) {
        call(String_growByAndReplace, {self, cap, size + n2 - n1 - cap, size, pos, n1, n2, s});
        return self;
    }
    const uint32_t p = isLong ? ld32(self + 8) : self + 1;
    uint32_t moveSrc, moveDst, moveLen;
    if (n1 == n2 || after == n1) {
        moveDst = p + pos;
        moveSrc = s;
        moveLen = n2;
    } else {
        const uint32_t tail = after - n1;
        if (n1 > n2) {
            memmoveGuest(p + pos, s, n2);
            moveDst = p + pos + n2;
            moveSrc = p + pos + n1;
            moveLen = tail;
        } else {
            uint32_t at = pos;
            if (p + pos < s && p + size > s) {
                if (p + pos + n1 <= s) {
                    s += n2 - n1;
                } else {
                    memmoveGuest(p + pos, s, n1);
                    at = pos + n1;
                    s += n2;
                    n2 -= n1;
                    n1 = 0;
                }
            }
            pos = at;
            memmoveGuest(p + pos + n2, p + pos + n1, tail);
            moveDst = p + pos;
            moveSrc = s;
            moveLen = n2;
        }
    }
    memmoveGuest(moveDst, moveSrc, moveLen);
    const uint32_t newSize = n2 - n1 + size;
    if (ld8(self) & 1) st32(self + 4, newSize);
    else st8(self, uint8_t(2 * newSize));
    st8(p + newSize, 0);
    return self;
}
}  // namespace

RW_FUNCTION(0x4B6DCC, "std::string::replace(pos, n1, const char*, n2) (sub_4B6DCC)", stringReplace);
