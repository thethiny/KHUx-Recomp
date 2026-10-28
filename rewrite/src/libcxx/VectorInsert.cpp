// std::vector::insert instances (one element, n copies, a forward range), libc++ 3.5's algorithms: in place when
// the capacity allows (__move_range opens the gap; an inserted value that lived inside the moved range is read from
// its new place), else through a __split_buffer of the recommended capacity, swapped in by
// __swap_out_circular_buffer. For trivially copyable elements the compiler inlined __move_range and
// __swap_out_circular_buffer (memmove / memcpy); for shared_ptr it calls the instance's own.
#include "libcxx/Libcxx.hpp"

using guest::Vector;
using libcxx::copyBytes;
using libcxx::copyShared;
using libcxx::copyWords;
using libcxx::maxSize;
using libcxx::moveShared;
using libcxx::THROW_LENGTH_ERROR;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t AEABI_MEMMOVE = 0x40EF50;   // __aeabi_memmove(dst, src, n) (import)
constexpr uint32_t FIRST = 0, BEGIN = 4, END = 8, CAP = 0xC, ALLOC = 0x10;   // __split_buffer<T, allocator<T>&>

Vector *vec(uint32_t v) { return rw::host_ptr<Vector>(v); }

// ------------------------------------------------------------------------------------------ element policies
// trivially copyable elements of Size bytes
template <uint32_t Size> struct Trivial {
    static void copy(uint32_t d, uint32_t s) { libcxx::copyElement<Size>(d, s); }
    static void move(uint32_t d, uint32_t s) { libcxx::copyElement<Size>(d, s); }
    static void assign(uint32_t d, uint32_t s) { libcxx::copyElement<Size>(d, s); }

    // __move_range(from_s, from_e, to): the tail past the current end constructed there, the rest moved back
    static void moveRange(uint32_t v, uint32_t fromS, uint32_t fromE, uint32_t to) {
        const uint32_t oldLast = load<uint32_t>(v + 4);
        const uint32_t n = oldLast - to;
        for (uint32_t i = fromS + n; i < fromE; i += Size) {
            if (const uint32_t slot = load<uint32_t>(v + 4)) move(slot, i);
            save<uint32_t>(v + 4, load<uint32_t>(v + 4) + Size);
        }
        rw::call(AEABI_MEMMOVE, {oldLast - n, fromS, n});
    }

    // __swap_out_circular_buffer(sb, p): [begin, p) copied in front of sb.begin, [p, end) after sb.end, the
    // buffers swapped -> the split buffer's old begin (where p's element now is)
    static uint32_t swapOut(uint32_t v, uint32_t sb, uint32_t p) {
        const uint32_t r = load<uint32_t>(sb + BEGIN);
        const uint32_t front = p - load<uint32_t>(v);
        const uint32_t newBegin = r - (front & ~(Size - 1));
        save<uint32_t>(sb + BEGIN, newBegin);
        copyBytes(newBegin, load<uint32_t>(v), front);
        const uint32_t back = load<uint32_t>(v + 4) - p;
        copyBytes(load<uint32_t>(sb + END), p, back);
        for (uint32_t k = 0; k < 12; k += 4) {
            const uint32_t mine = load<uint32_t>(v + k);
            const uint32_t theirs = k == 4 ? load<uint32_t>(sb + END) + (back & ~(Size - 1)) : load<uint32_t>(sb + 4 + k);
            save<uint32_t>(v + k, theirs);
            save<uint32_t>(sb + 4 + k, mine);
        }
        save<uint32_t>(sb + FIRST, load<uint32_t>(sb + BEGIN));
        return r;
    }
};

// shared_ptr<T> (8 bytes), with the instance's out-of-line __move_range / __swap_out_circular_buffer
template <uint32_t MoveRange, uint32_t SwapOut> struct Shared {
    static void copy(uint32_t d, uint32_t s) { copyShared(d, s); }
    static void move(uint32_t d, uint32_t s) { moveShared(d, s); }
    static void assign(uint32_t d, uint32_t s) {   // operator=(const shared_ptr&)
        const uint32_t obj = load<uint32_t>(s), ctrl = load<uint32_t>(s + 4);
        if (ctrl) guest::addRef(ctrl);
        save<uint32_t>(d, obj);
        const uint32_t old = load<uint32_t>(d + 4);
        save<uint32_t>(d + 4, ctrl);
        if (old) guest::release(old);
    }
    static void moveRange(uint32_t v, uint32_t fromS, uint32_t fromE, uint32_t to) { rw::call(MoveRange, {v, fromS, fromE, to}); }
    static uint32_t swapOut(uint32_t v, uint32_t sb, uint32_t p) { return rw::call(SwapOut, {v, sb, p}); }
};

// ------------------------------------------------------------------------------------------ the split buffer
// __split_buffer(cap, start, alloc): `cap` elements, empty at element `start`
template <uint32_t Size> void splitInit(uint32_t sb, uint32_t v, uint32_t cap, uint32_t start) {
    save<uint32_t>(sb + CAP, 0);
    save<uint32_t>(sb + ALLOC, v + 8);
    const uint32_t first = cap ? guest::newObject(cap * Size) : 0;
    save<uint32_t>(sb + FIRST, first);
    save<uint32_t>(sb + BEGIN, first + start * Size);
    save<uint32_t>(sb + END, first + start * Size);
    save<uint32_t>(sb + CAP, first + cap * Size);
}

// __split_buffer::push_back(x) on the freshly made (empty) buffer, inlined: when its end is at the capacity the
// empty range slides to the middle, or (at the very front) the buffer is regrown into a temporary and swapped
template <uint32_t Size, class E, uint32_t SplitDtor> void splitPushBack(uint32_t sb, uint32_t x) {
    if (load<uint32_t>(sb + END) == load<uint32_t>(sb + CAP)) {
        const uint32_t first = load<uint32_t>(sb + FIRST), begin = load<uint32_t>(sb + BEGIN);
        if (begin > first) {
            const uint32_t d = ((begin - first) / Size + 1) / 2;
            save<uint32_t>(sb + BEGIN, begin - d * Size);
            save<uint32_t>(sb + END, begin - d * Size);
        } else {
            guest::Frame frame(20);
            const uint32_t t = frame.addr();
            save<uint32_t>(t + ALLOC, load<uint32_t>(sb + ALLOC));
            uint32_t c = 2 * ((load<uint32_t>(sb + CAP) - first) / Size);
            if (!c) c = 1;
            const uint32_t tFirst = guest::newObject(c * Size), tBegin = tFirst + c / 4 * Size;
            save<uint32_t>(t + FIRST, tFirst);
            save<uint32_t>(t + BEGIN, tBegin);
            save<uint32_t>(t + END, tBegin);
            save<uint32_t>(t + CAP, tFirst + c * Size);
            for (uint32_t p = load<uint32_t>(sb + BEGIN); p != load<uint32_t>(sb + END); p += Size) {
                if (const uint32_t slot = load<uint32_t>(t + END)) E::move(slot, p);
                save<uint32_t>(t + END, load<uint32_t>(t + END) + Size);
            }
            for (uint32_t k = 0; k < 16; k += 4) {
                const uint32_t mine = load<uint32_t>(sb + k);
                save<uint32_t>(sb + k, load<uint32_t>(t + k));
                save<uint32_t>(t + k, mine);
            }
            rw::call(SplitDtor, {t});
        }
    }
    if (const uint32_t slot = load<uint32_t>(sb + END)) E::copy(slot, x);
    save<uint32_t>(sb + END, load<uint32_t>(sb + END) + Size);
}

// ------------------------------------------------------------------------------------------ insert
// insert(pos, x) -> the inserted element
template <uint32_t Size, class E, uint32_t SplitDtor> uint32_t insertOne(uint32_t v, uint32_t pos, uint32_t x) {
    const uint32_t begin = load<uint32_t>(v);
    const uint32_t p = begin + ((pos - begin) & ~(Size - 1));
    if (load<uint32_t>(v + 4) < load<uint32_t>(v + 8)) {
        if (p == load<uint32_t>(v + 4)) {
            if (p) E::copy(p, x);
            save<uint32_t>(v + 4, load<uint32_t>(v + 4) + Size);
        } else {
            E::moveRange(v, p, load<uint32_t>(v + 4), p + Size);
            uint32_t xr = x;
            if (p <= xr && xr < load<uint32_t>(v + 4)) xr += Size;
            E::assign(p, xr);
        }
        return p;
    }
    const uint32_t size = (load<uint32_t>(v + 4) - begin) / Size;
    if (size + 1 > maxSize<Size>()) {
        rw::call(THROW_LENGTH_ERROR, {v});
        return 0;
    }
    guest::Frame frame(20);
    const uint32_t sb = frame.addr();
    splitInit<Size>(sb, v, libcxx::recommend<Size>(vec(v), size + 1), (p - begin) / Size);
    splitPushBack<Size, E, SplitDtor>(sb, x);
    const uint32_t r = E::swapOut(v, sb, p);
    rw::call(SplitDtor, {sb});
    return r;
}

// insert(pos, n, x) of trivially copyable elements -> the first inserted element
template <uint32_t Size, uint32_t SplitDtor> uint32_t insertFill(uint32_t v, uint32_t pos, uint32_t n, uint32_t x) {
    using E = Trivial<Size>;
    const uint32_t begin = load<uint32_t>(v);
    const uint32_t p = begin + ((pos - begin) & ~(Size - 1));
    if (!n) return p;
    if (n <= (load<uint32_t>(v + 8) - load<uint32_t>(v + 4)) / Size) {
        const uint32_t oldN = n, oldLast = load<uint32_t>(v + 4);
        const uint32_t dx = (load<uint32_t>(v + 4) - p) / Size;
        if (n > dx) {
            for (uint32_t k = n - dx; k; k--) {   // __construct_at_end(n - dx, x)
                if (const uint32_t slot = load<uint32_t>(v + 4)) E::copy(slot, x);
                save<uint32_t>(v + 4, load<uint32_t>(v + 4) + Size);
            }
            n = dx;
        }
        if (n) {
            E::moveRange(v, p, oldLast, p + oldN * Size);
            uint32_t xr = x;
            if (p <= xr && xr < load<uint32_t>(v + 4)) xr += oldN * Size;
            for (uint32_t k = 0; k < n; k++) E::assign(p + k * Size, xr);   // fill_n
        }
        return p;
    }
    const uint32_t newSize = (load<uint32_t>(v + 4) - begin) / Size + n;
    if (newSize > maxSize<Size>()) {
        rw::call(THROW_LENGTH_ERROR, {v});
        return 0;
    }
    guest::Frame frame(20);
    const uint32_t sb = frame.addr();
    splitInit<Size>(sb, v, libcxx::recommend<Size>(vec(v), newSize), (p - begin) / Size);
    for (; n; n--) {   // __split_buffer::__construct_at_end(n, x)
        if (const uint32_t slot = load<uint32_t>(sb + END)) E::copy(slot, x);
        save<uint32_t>(sb + END, load<uint32_t>(sb + END) + Size);
    }
    const uint32_t r = E::swapOut(v, sb, p);
    rw::call(SplitDtor, {sb});
    return r;
}

// insert(pos, first, last) of trivially copyable elements -> the first inserted element (its __recommend has no
// length check)
template <uint32_t Size, uint32_t SplitDtor> uint32_t insertRange(uint32_t v, uint32_t pos, uint32_t first, uint32_t last) {
    using E = Trivial<Size>;
    const uint32_t begin = load<uint32_t>(v);
    const uint32_t p = begin + ((pos - begin) & ~(Size - 1));
    int32_t n = int32_t(last - first) / int32_t(Size);
    if (n <= 0) return p;
    if (n <= int32_t(load<uint32_t>(v + 8) - load<uint32_t>(v + 4)) / int32_t(Size)) {
        const int32_t oldN = n;
        const uint32_t oldLast = load<uint32_t>(v + 4);
        uint32_t m = last;
        const int32_t dx = int32_t(load<uint32_t>(v + 4) - p) / int32_t(Size);
        if (n > dx) {
            m = first + uint32_t(dx) * Size;
            for (uint32_t i = m; i != last; i += Size) {   // __construct_at_end(m, last)
                if (const uint32_t slot = load<uint32_t>(v + 4)) E::copy(slot, i);
                save<uint32_t>(v + 4, load<uint32_t>(v + 4) + Size);
            }
            n = dx;
        }
        if (n > 0) {
            E::moveRange(v, p, oldLast, p + uint32_t(oldN) * Size);
            rw::call(AEABI_MEMMOVE, {p, first, m - first});   // copy(first, m, p)
        }
        return p;
    }
    const uint32_t size = (load<uint32_t>(v + 4) - begin) / Size;
    const uint32_t newSize = size + uint32_t(n);
    const uint32_t cap = (load<uint32_t>(v + 8) - begin) / Size;
    const uint32_t recommended = cap >= maxSize<Size>() / 2 ? maxSize<Size>() : (2 * cap < newSize ? newSize : 2 * cap);
    guest::Frame frame(20);
    const uint32_t sb = frame.addr();
    splitInit<Size>(sb, v, recommended, (p - begin) / Size);
    for (uint32_t i = first; i != last; i += Size) {   // __split_buffer::__construct_at_end(first, last)
        if (const uint32_t slot = load<uint32_t>(sb + END)) E::copy(slot, i);
        save<uint32_t>(sb + END, load<uint32_t>(sb + END) + Size);
    }
    const uint32_t r = E::swapOut(v, sb, p);
    rw::call(SplitDtor, {sb});
    return r;
}

using MedalDataElems = Shared<0x52FF45, 0x52FFD3>;   // vector<shared_ptr<MedalDataManager::UserMedalData>>

}  // namespace

RW_FUNCTION(0x516208, "std::vector<int>::insert(const_iterator, const int&)", (insertOne<4, Trivial<4>, 0x47D819>));
RW_FUNCTION(0x64A374, "std::vector<AvatarActor*>::insert(const_iterator, AvatarActor* const&)", (insertOne<4, Trivial<4>, 0x5F95BD>));
RW_FUNCTION(0x6AD344, "std::vector<unsigned long long>::insert(const_iterator, const unsigned long long&)", (insertOne<8, Trivial<8>, 0x4B350D>));
RW_FUNCTION(0x52DE2C, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::insert(const_iterator, const shared_ptr&)", (insertOne<8, MedalDataElems, 0x487B7B>));
RW_FUNCTION(0x68FA38, "std::vector<unsigned long long>::insert(const_iterator, size_t, const unsigned long long&)", (insertFill<8, 0x4B350D>));
RW_FUNCTION(0x92DE48, "std::vector<unsigned short>::insert(const_iterator, const unsigned short*, const unsigned short*)", (insertRange<2, 0x8C7D01>));
