// Library code that sits among the game's static constructors (0x453000..0x45A000) and is counted with the game:
// a libpng helper, cocos2d's and libc++'s exit-time array destructors, and the C runtime's atexit (ARM code from
// crtbegin_so).
#include "Guest.hpp"

namespace {

using rw::call;
using rw::load;

constexpr uint32_t ATTRIBUTE_NAMES_GOT = 0xFD8114;  // &cocos2d::s_attributeNames (six std::string)
constexpr uint32_t STRING_DTOR = 0xC8F01F;          // std::string::~basic_string (sub_C8F01E)
constexpr uint32_t WSTRING_DTOR = 0xC91675;         // std::wstring::~basic_string (sub_C91674)
constexpr uint32_t STRING_SIZE = 0xC;
// libc++'s __time_get_c_storage tables (24 strings each, filled on first use)
constexpr uint32_t WIDE_AM_PM = 0x100DA90, AM_PM = 0x100DBB0, WIDE_MONTHS = 0x100DCD0, MONTHS = 0x100DDF0;
constexpr uint32_t ATEXIT_HANDLER_WRAPPER = 0x459DFC;  // ARM

}  // namespace

struct LibraryStatics {
    // libpng's png_colorspace_endpoints_match, the part the compiler split off: green and blue (png_xy +8 .. +0x14)
    // each within delta of the other's
    static uint32_t endpointsMatch(uint32_t xy1, uint32_t xy2, uint32_t delta) {
        for (uint32_t at = 8; at <= 0x14; at += 4) {
            const int32_t a = int32_t(load<uint32_t>(xy1 + at)), b = int32_t(load<uint32_t>(xy2 + at));
            if (a < int32_t(uint32_t(b) - delta) || a > int32_t(uint32_t(b) + delta)) return 0;
        }
        return 1;
    }

    // cocos2d::s_attributeNames' exit destructor: the six names, last first (a long string frees its buffer)
    static void attributeNames() {
        const uint32_t names = load<uint32_t>(ATTRIBUTE_NAMES_GOT);
        for (uint32_t k = 6; k-- > 0;) {
            const uint32_t s = names + k * STRING_SIZE;
            if (load<uint8_t>(s) & 1) guest::deleteObject(load<uint32_t>(s + 8));
        }
    }

    // the time tables' exit destructors: 24 strings, last first
    static void strings(uint32_t table, uint32_t dtor) {
        for (uint32_t k = 24; k-- > 0;) call(dtor, {table + k * STRING_SIZE});
    }
    static void wideAmPm() { strings(WIDE_AM_PM, WSTRING_DTOR); }
    static void amPm() { strings(AM_PM, STRING_DTOR); }
    static void wideMonths() { strings(WIDE_MONTHS, WSTRING_DTOR); }
    static void months() { strings(MONTHS, STRING_DTOR); }

    // __atexit_handler_wrapper(func): calls func if there is one
    static uint32_t atexitHandler(uint32_t func) { return func ? call(func, {}) : 0; }
    // atexit(func) = __cxa_atexit(__atexit_handler_wrapper, func, &__dso_handle)
    static uint32_t atexit(uint32_t func) {
        return call(guest::CXA_ATEXIT, {ATEXIT_HANDLER_WRAPPER, func, guest::DSO_HANDLE});
    }
};
RW_FUNCTION(0x453224, "png_colorspace_endpoints_match, green and blue (sub_453224)", LibraryStatics::endpointsMatch);
RW_FUNCTION(0x457884, "cocos2d::s_attributeNames static destructor (sub_457884)", LibraryStatics::attributeNames);
RW_FUNCTION(0x459078, "libc++ wide am/pm table static destructor (sub_459078)", LibraryStatics::wideAmPm);
RW_FUNCTION(0x45917C, "libc++ am/pm table static destructor (sub_45917C)", LibraryStatics::amPm);
RW_FUNCTION(0x459280, "libc++ wide month table static destructor (sub_459280)", LibraryStatics::wideMonths);
RW_FUNCTION(0x459384, "libc++ month table static destructor (sub_459384)", LibraryStatics::months);
// not verified: ARM code the lifter decoded as Thumb (its "original" jumps into another function); nothing calls them
RW_FUNCTION_NOVERIFY(0x459DFC, "__atexit_handler_wrapper (sub_459DFC)", LibraryStatics::atexitHandler);
RW_FUNCTION_NOVERIFY(0x459E10, "atexit (sub_459E10)", LibraryStatics::atexit);
