// <algorithm> / <array> / <bitset> / <memory> instances over the game's types (libc++ 3.5's algorithms).
#include "libcxx/Libcxx.hpp"

using libcxx::copyBytes;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t AEABI_MEMSET = 0x40E7A0;     // __aeabi_memset(dest, n, c) (import)
constexpr uint32_t OPERATOR_NEW_NOTHROW = 0xC98F4D;   // operator new(size_t, const nothrow_t&)
constexpr uint32_t NOTHROW = 0xF43DF3;          // std::nothrow
constexpr uint32_t TOUPPER_TAB = 0xFD614C;      // _toupper_tab_ (bionic's toupper table, a GOT import)

inline uint32_t shr(uint32_t v, uint32_t s) { return s >= 32 ? 0 : v >> s; }   // ARM register shifts
inline uint32_t shl(uint32_t v, uint32_t s) { return s >= 32 ? 0 : v << s; }

// __fill_n_false(bit_iterator first {word, bit}, n) on a bitset<3>'s storage: clear n bits from first; the
// iterator (passed by reference) is advanced over the words it touches
void fillNFalse(uint32_t it, uint32_t n) {
    const uint32_t ctz = load<uint32_t>(it + 4);
    if (ctz) {
        const uint32_t clz = 32 - ctz;
        const uint32_t dn = clz > n ? n : clz;
        const uint32_t m = shl(~0u, ctz) & shr(~0u, clz - dn);
        const uint32_t seg = load<uint32_t>(it);
        save<uint32_t>(seg, load<uint32_t>(seg) & ~m);
        save<uint32_t>(it, load<uint32_t>(it) + 4);
        n -= dn;
    }
    const uint32_t nw = n / 32;
    rw::call(AEABI_MEMSET, {load<uint32_t>(it), nw * 4, 0});
    if (nw * 32 != n) {
        const uint32_t seg = load<uint32_t>(it) + nw * 4;
        save<uint32_t>(it, seg);
        save<uint32_t>(seg, load<uint32_t>(seg) & ~shr(~0u, 32 - (n - nw * 32)));
    }
}

// get_temporary_buffer<UnionRanking::Ranker>(n) -> {buffer, count} in `result`: nothrow new, halving on failure
void rankerTemporaryBuffer(uint32_t result, int32_t n) {
    save<uint32_t>(result, 0);
    save<uint32_t>(result + 4, 0);
    if (n > 0x1249249) n = 0x1249249;   // PTRDIFF_MAX / sizeof(Ranker)
    while (n >= 1) {
        const uint32_t p = rw::call(OPERATOR_NEW_NOTHROW, {uint32_t(n) * 0x70, NOTHROW});
        if (p) {
            save<uint32_t>(result, p);
            save<uint32_t>(result + 4, uint32_t(n));
            return;
        }
        const bool more = n > 1;
        n /= 2;
        if (!more) break;
    }
    save<uint32_t>(result, 0);
}

// swap(Ranker&, Ranker&) by moves: two shared_ptrs and 0x60 plain bytes (a temporary on the stack)
void moveShared(uint32_t d, uint32_t s) {   // shared_ptr move assignment: the old owner released
    const uint32_t obj = load<uint32_t>(s), ctrl = load<uint32_t>(s + 4);
    save<uint32_t>(s, 0);
    save<uint32_t>(s + 4, 0);
    save<uint32_t>(d, obj);
    const uint32_t old = load<uint32_t>(d + 4);
    save<uint32_t>(d + 4, ctrl);
    if (old) guest::release(old);
}
void swapRankers(uint32_t a, uint32_t b) {
    guest::Frame frame(0x70);
    const uint32_t tmp = frame.addr();
    for (uint32_t k = 0; k < 16; k += 4) save<uint32_t>(tmp + k, load<uint32_t>(a + k));   // move-construct
    for (uint32_t k = 0; k < 16; k += 4) save<uint32_t>(a + k, 0);
    copyBytes(tmp + 0x10, a + 0x10, 0x60);
    moveShared(a, b);
    moveShared(a + 8, b + 8);
    copyBytes(a + 0x10, b + 0x10, 0x60);
    save<uint32_t>(b, load<uint32_t>(tmp));
    uint32_t old = load<uint32_t>(b + 4);
    save<uint32_t>(b + 4, load<uint32_t>(tmp + 4));
    if (old) guest::release(old);
    save<uint32_t>(b + 8, load<uint32_t>(tmp + 8));
    old = load<uint32_t>(b + 0xC);
    save<uint32_t>(b + 0xC, load<uint32_t>(tmp + 0xC));
    if (old) guest::release(old);
    copyBytes(b + 0x10, tmp + 0x10, 0x60);
}

// __rotate_forward(first, middle, last) over Rankers -> the new position of first's element
uint32_t rotateRankers(uint32_t first, uint32_t middle, uint32_t last) {
    constexpr uint32_t S = 0x70;
    uint32_t i = middle;
    for (;;) {
        swapRankers(first, i);
        first += S;
        i += S;
        if (i == last) break;
        if (first == middle) middle = i;
    }
    const uint32_t r = first;
    if (first != middle) {
        i = middle;
        for (;;) {
            swapRankers(first, i);
            first += S;
            i += S;
            if (i == last) {
                if (first == middle) break;
                i = middle;
            } else if (first == middle) {
                middle = i;
            }
        }
    }
    return r;
}

// array<RaidBase::Info::Part, 6>::at(i) const
uint32_t raidPartAt(uint32_t self, uint32_t i) {
    if (i < 6) return self + i * 0x14;
    const uint32_t e = rw::call(0xC98131, {8});            // __cxa_allocate_exception
    rw::call(0xC8E86D, {e, 0xDFD494});                      // logic_error::logic_error("array::at")
    save<uint32_t>(e, 0xFD5848);                            // vtable for std::out_of_range
    rw::call(0xC983D5, {e, 0xFD585C, 0xC9A9D5});            // __cxa_throw(e, &typeid(out_of_range), ~out_of_range)
    return 0;
}

// FindSubstringInsensitive's std::search: chars compared through bionic's toupper table (negative chars as is)
int32_t upper(uint32_t p) {
    const int32_t c = int8_t(load<uint8_t>(p));
    return c < 0 ? c : int16_t(load<uint16_t>(load<uint32_t>(TOUPPER_TAB) + uint32_t(c) * 2 + 2));
}
uint32_t searchInsensitive(uint32_t first1, uint32_t last1, uint32_t first2, uint32_t last2) {
    const int32_t len2 = int32_t(last2 - first2);
    if (!len2) return first1;
    if (int32_t(last1 - first1) < len2) return last1;
    const uint32_t s = last1 + 1 - uint32_t(len2);
    for (; first1 != s; first1++) {
        if (upper(first1) != upper(first2)) continue;
        int32_t k = 1;
        for (; k != len2 && upper(first1 + k) == upper(first2 + k); k++) {}
        if (k == len2) return first1;
    }
    return last1;
}

// std::__search / std::__find_end over chars with a bool(*)(char, char) predicate (passed by reference)
bool charPred(uint32_t pred, uint32_t a, uint32_t b) {
    const uint32_t ca = uint32_t(int32_t(int8_t(load<uint8_t>(a)))), cb = uint32_t(int32_t(int8_t(load<uint8_t>(b))));
    return rw::call(load<uint32_t>(pred), {ca, cb}) != 0;
}
uint32_t searchPred(uint32_t first1, uint32_t last1, uint32_t first2, uint32_t last2, uint32_t pred) {
    const int32_t len2 = int32_t(last2 - first2);
    if (!len2) return first1;
    if (int32_t(last1 - first1) < len2) return last1;
    const uint32_t s = last1 - uint32_t(len2 - 1);
    for (;;) {
        for (;;) {
            if (first1 == s) return last1;
            if (charPred(pred, first1, first2)) break;
            ++first1;
        }
        uint32_t m1 = first1, m2 = first2;
        for (;;) {
            if (++m2 == last2) return first1;
            ++m1;
            if (!charPred(pred, m1, m2)) {
                ++first1;
                break;
            }
        }
    }
}
uint32_t findEndPred(uint32_t first1, uint32_t last1, uint32_t first2, uint32_t last2, uint32_t pred) {
    const int32_t len2 = int32_t(last2 - first2);
    if (!len2) return last1;
    if (int32_t(last1 - first1) < len2) return last1;
    const uint32_t s = first1 + uint32_t(len2 - 1);
    uint32_t l1 = last1;
    const uint32_t l2 = last2 - 1;
    for (;;) {
        for (;;) {
            if (s == l1) return last1;
            if (charPred(pred, --l1, l2)) break;
        }
        uint32_t m1 = l1, m2 = l2;
        for (;;) {
            if (m2 == first2) return m1;
            if (!charPred(pred, --m1, --m2)) break;
        }
    }
}

// __thread_specific_ptr<__thread_struct>::reset(p): p installed for this thread, the previous one destroyed
constexpr uint32_t PTHREAD_GETSPECIFIC = 0x410A44, PTHREAD_SETSPECIFIC = 0x410A50;   // imports
void threadStructReset(uint32_t self, uint32_t p) {
    const uint32_t old = rw::call(PTHREAD_GETSPECIFIC, {load<uint32_t>(self)});
    rw::call(PTHREAD_SETSPECIFIC, {load<uint32_t>(self), p});
    if (old) {
        rw::call(0xC97EFD, {old});   // __thread_struct::~__thread_struct
        guest::deleteObject(old);
    }
}

// operator+(const string&, const string&): the left side's chars with room for both, then the right side appended
constexpr uint32_t STRING_INIT_RESERVE = 0x48461F;   // basic_string::__init(const char*, size_t size, size_t reserve)
constexpr uint32_t STRING_APPEND = 0x47AC89;         // basic_string::append(const char*, size_t)
inline uint32_t strSize(uint32_t s) { return (load<uint8_t>(s) & 1) ? load<uint32_t>(s + 4) : uint32_t(load<uint8_t>(s) >> 1); }
inline uint32_t strData(uint32_t s) { return (load<uint8_t>(s) & 1) ? load<uint32_t>(s + 8) : s + 1; }
void stringConcat(uint32_t result, uint32_t l, uint32_t r) {
    save<uint32_t>(result, 0);
    save<uint32_t>(result + 4, 0);
    save<uint32_t>(result + 8, 0);
    const uint32_t lsize = strSize(l), rsize = strSize(r);
    rw::call(STRING_INIT_RESERVE, {result, strData(l), lsize, lsize + rsize});
    rw::call(STRING_APPEND, {result, strData(r), rsize});
}

// basic_string::__init(__wrap_iter<char*> first, last): a short (up to 10 chars) or heap buffer, the chars, a 0
void stringInitRange(uint32_t s, uint32_t first, uint32_t last) {
    const uint32_t size = last - first;
    if (size >= 0xFFFFFFF0u) {
        rw::call(0x47ADED, {s});   // __basic_string_common::__throw_length_error
        return;
    }
    uint32_t p;
    if (size <= 10) {
        save<uint8_t>(s, uint8_t(size * 2));
        p = s + 1;
    } else {
        const uint32_t cap = (size + 16) & ~15u;
        p = guest::newObject(cap);
        save<uint32_t>(s, cap | 1);
        save<uint32_t>(s + 4, size);
        save<uint32_t>(s + 8, p);
    }
    for (uint32_t k = 0; first + k != last; k++) save<uint8_t>(p + k, load<uint8_t>(first + k));
    save<uint8_t>(p + size, 0);
}

}  // namespace

RW_FUNCTION(0x5F7A22, "std::__fill_n_false<__bitset<1, 3>>", fillNFalse);
RW_FUNCTION(0x7367FC, "std::get_temporary_buffer<UnionRanking::Ranker>", rankerTemporaryBuffer);
RW_FUNCTION(0x737A10, "std::__rotate_forward<__wrap_iter<UnionRanking::Ranker*>>", rotateRankers);
RW_FUNCTION(0x56CB50, "std::array<RaidBase::Info::Part, 6>::at const", raidPartAt);
RW_FUNCTION(0x49A35C, "std::__search<FindSubstringInsensitive lambda, __wrap_iter<const char*>>", searchInsensitive);
RW_FUNCTION(0x49A42A, "std::__search<bool (*&)(char, char), const char*, const char*>", searchPred);
RW_FUNCTION(0x7C9370, "std::__find_end<bool (*&)(char, char), const char*, const char*>", findEndPred);
RW_FUNCTION(0x499748, "std::__thread_specific_ptr<__thread_struct>::reset", threadStructReset);
RW_FUNCTION(0x470EAC, "std::operator+(const string&, const string&)", stringConcat);
RW_FUNCTION(0x49A4A6, "std::string::__init<__wrap_iter<char*>>", stringInitRange);
