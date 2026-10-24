// llvm::ConvertUTF wrappers (llvm/Support/ConvertUTFWrapper.cpp) compiled into the game for its text input:
// UTF-8 <-> wide / UTF-16 conversions over the ConvertUTF.c core (library code, called).
#include "SystemCommon.hpp"

namespace llvm_utf {

namespace {
constexpr uint32_t IS_LEGAL_UTF8_STRING = rwlib::isLegalUTF8String;   // isLegalUTF8String(const UTF8** src, const UTF8* end)
constexpr uint32_t CONVERT_UTF8_TO_UTF16 = rwlib::ConvertUTF8toUTF16;  // ConvertUTF8toUTF16(src**, srcEnd, dst**, dstEnd, flags)
constexpr uint32_t CONVERT_UTF8_TO_UTF32 = rwlib::ConvertUTF8toUTF32;  // ConvertUTF8toUTF32(...)
constexpr uint32_t CONVERT_UTF32_TO_UTF8 = rwlib::ConvertUTF32toUTF8;  // ConvertUTF32toUTF8(...)
constexpr uint32_t CONVERT_UTF16_TO_UTF8 = rwlib::ConvertUTF16toUTF8;  // ConvertUTF16toUTF8(...)
constexpr uint32_t MEMCPY = 0x40E164;                 // memcpy (import)
constexpr uint32_t STRING_RESIZE = 0x7EA449;          // sub_7EA448: std::string::resize(n, ch)
constexpr uint32_t U16VECTOR_INSERT = 0x92DE49;       // std::vector<unsigned short>::insert(pos, first, last)
constexpr uint32_t U16VECTOR_BASE_DTOR = 0x8CAC23;    // std::__vector_base<unsigned short>::~__vector_base

// a libc++ string's characters and length (char or char16_t: the short form's data starts at +1 / +2)
struct Chars {
    uint32_t data, size;
};
Chars chars(uint32_t str, uint32_t shortDataOffset) {
    uint8_t b0 = sys::ld8(str);
    if (b0 & 1) return {sys::ld32(str + 8), sys::ld32(str + 4)};
    return {str + shortDataOffset, uint32_t(b0 >> 1)};
}
}  // namespace

// ConvertUTF8toWide(unsigned WideCharWidth, const std::string& source, char*& resultPtr, const UTF8*& errorPtr)
uint32_t ConvertUTF8toWide(uint32_t width, uint32_t source, const uint32_t *resultPtr, const uint32_t *errorPtr) {
    guest::Frame f(0x30);
    uint32_t result = 0;
    if (width == 4 || width == 2) {
        const bool utf32 = width == 4;
        const uint32_t dstSlot = f.addr(utf32 ? 0x14 : 0x1C), srcSlot = f.addr(utf32 ? 0x18 : 0x20);
        Chars s = chars(source, 1);
        sys::st32(srcSlot, s.data);
        uint32_t dst = *resultPtr;
        sys::st32(dstSlot, dst);
        uint32_t dstEnd = dst + (s.size << (utf32 ? 4 : 2));
        result = rw::call(utf32 ? CONVERT_UTF8_TO_UTF32 : CONVERT_UTF8_TO_UTF16,
                          {srcSlot, s.data + s.size, dstSlot, dstEnd, 0});
        if (result) rw::store32(errorPtr, sys::ld32(srcSlot));
        else rw::store32(resultPtr, sys::ld32(dstSlot));
    } else if (width == 1) {
        const uint32_t srcSlot = f.addr(0x24);
        Chars s = chars(source, 1);
        sys::st32(srcSlot, s.data);
        if (rw::call(IS_LEGAL_UTF8_STRING, {srcSlot, s.data + s.size})) {
            Chars t = chars(source, 1);
            rw::call(MEMCPY, {*resultPtr, t.data, t.size});
            uint32_t n = chars(source, 1).size;
            rw::store32(resultPtr, *resultPtr + n);
        } else {
            rw::store32(errorPtr, sys::ld32(srcSlot));
            result = 3;   // sourceIllegal
        }
    }
    return result == 0 ? 1 : 0;
}
RW_FUNCTION(0x92DAF4, "llvm::ConvertUTF8toWide", ConvertUTF8toWide);

// ConvertCodePointToUTF8(unsigned source, char*& resultPtr)
uint32_t ConvertCodePointToUTF8(uint32_t codePoint, const uint32_t *resultPtr) {
    guest::Frame f(0x1C);
    const uint32_t dstSlot = f.addr(0xC), srcSlot = f.addr(0x10), cp = f.addr(0x14);
    sys::st32(cp, codePoint);
    sys::st32(srcSlot, cp);
    uint32_t dst = *resultPtr;
    sys::st32(dstSlot, dst);
    if (rw::call(CONVERT_UTF32_TO_UTF8, {srcSlot, cp + 4, dstSlot, dst + 4, 0}) != 0) return 0;
    rw::store32(resultPtr, sys::ld32(dstSlot));
    return 1;
}
RW_FUNCTION(0x92DC50, "llvm::ConvertCodePointToUTF8", ConvertCodePointToUTF8);

uint32_t hasUTF16ByteOrderMark(const uint8_t *bytes, uint32_t length) {
    if (length < 2) return 0;
    if (bytes[0] == 0xFE) return bytes[1] == 0xFF ? 1 : 0;
    if (bytes[0] == 0xFF) return bytes[1] == 0xFE ? 1 : 0;
    return 0;
}
RW_FUNCTION(0x92DCB8, "llvm::hasUTF16ByteOrderMark", hasUTF16ByteOrderMark);

// convertUTF16ToUTF8String(const std::u16string& src, std::string& out): byte-swapped input (BOM FFFE) is
// swapped into a copy first, a BOM is skipped.
uint32_t convertUTF16ToUTF8String(uint32_t src, guest::String *out) {
    Chars s = chars(src, 2);
    if (s.size == 0) return 1;
    guest::Frame f(0x30);
    const uint32_t dstSlot = f.addr(0x14), swapped = f.addr(0x18), srcSlot = f.addr(0x24);
    uint32_t begin = s.data;
    sys::st32(srcSlot, begin);
    sys::st32(swapped, 0);
    sys::st32(swapped + 4, 0);
    sys::st32(swapped + 8, 0);
    uint32_t end = s.data + s.size * 2;
    uint32_t first = sys::ld16(begin);
    if (first == 0xFFFE) {
        rw::call(U16VECTOR_INSERT, {swapped, 0, begin, end});
        uint32_t vb = sys::ld32(swapped);
        uint32_t n = (sys::ld32(swapped + 4) - vb) >> 1;
        for (uint32_t k = 0; k < n; k++) {
            uint32_t p = sys::ld32(swapped) + 2 * k;
            uint16_t v = sys::ld16(p);
            sys::st16(p, uint16_t((v << 8) | (v >> 8)));
        }
        begin = sys::ld32(swapped);
        end = sys::ld32(swapped + 4);
        sys::st32(srcSlot, begin);
        first = sys::ld16(begin);
    }
    if (first == 0xFEFF) sys::st32(srcSlot, begin + 2);
    uint32_t units = chars(src, 2).size;
    uint32_t outAddr = rw::guest_addr(out);
    rw::call(STRING_RESIZE, {outAddr, (units << 2) | 1, 0});
    Chars o = chars(outAddr, 1);
    sys::st32(dstSlot, o.data);
    uint32_t ok;
    if (rw::call(CONVERT_UTF16_TO_UTF8, {srcSlot, end, dstSlot, o.data + o.size, 0}) != 0) {
        // failed: out becomes empty
        if (sys::ld8(outAddr) & 1) {
            sys::st8(sys::ld32(outAddr + 8), 0);
            sys::st32(outAddr + 4, 0);
        } else {
            sys::st8(outAddr + 1, 0);
            sys::st8(outAddr, 0);
        }
        ok = 0;
    } else {
        uint32_t dataNow = (sys::ld8(outAddr) & 1) ? sys::ld32(outAddr + 8) : outAddr + 1;
        rw::call(STRING_RESIZE, {outAddr, sys::ld32(dstSlot) - dataNow, 0});
        ok = 1;
    }
    rw::call(U16VECTOR_BASE_DTOR, {swapped});
    return ok;
}
RW_FUNCTION(0x92DCF0, "llvm::convertUTF16ToUTF8String", convertUTF16ToUTF8String);

}  // namespace llvm_utf
