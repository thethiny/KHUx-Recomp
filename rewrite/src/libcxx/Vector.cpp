// std::vector members over the game's element types that the compiler emitted out of line: allocate / deallocate,
// copy constructors and __construct_at_end. The vector's own bookkeeping is libc++'s (Libcxx.hpp); what differs per
// instance is the element size and how one element is copied or destroyed - the inlined copy constructors below
// copy exactly the bytes the compiled code copies (padding stays untouched), strings through basic_string::__init.
#include "libcxx/Libcxx.hpp"

using guest::Vector;
using libcxx::copyBytes;
using libcxx::copyShared;
using libcxx::copyString;
using libcxx::copyWords;
using libcxx::destroyFunction;
using libcxx::destroyShared;
using libcxx::destroyString;
using libcxx::destroyVector;
using libcxx::maxSize;
using libcxx::THROW_LENGTH_ERROR;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t VectorInt_copy = 0x4AF707;          // vector<int>::vector(const vector&)
constexpr uint32_t VectorU64Base_dtor = 0x468499;      // __vector_base<unsigned long long>::~__vector_base
constexpr uint32_t VectorStory_copy = 0x4B5BC9;        // vector<Stage::Story>::vector(const vector&)
constexpr uint32_t Party_copy = 0x4B2E63;              // hole::network::api::Party::Party(const Party&)
constexpr uint32_t Gacha_copy = 0x4B37E5;              // hole::network::api::GachaList::Gacha::Gacha(const Gacha&)
constexpr uint32_t SafeBuffer_copy = 0x45A29D;         // SafeBuffer::SafeBuffer(const SafeBuffer&)
constexpr uint32_t AvatarStyle_vtable = 0xF7E554;      // _ZTVN6Avatar5StyleE + 8
constexpr uint32_t DEQUE_BLOCK_BYTES = 0x1000;         // a deque<shared_ptr<...>> block: 512 elements

Vector *vec(uint32_t v) { return rw::host_ptr<Vector>(v); }
void copyByte(uint32_t d, uint32_t s, uint32_t off) { save<uint8_t>(d + off, load<uint8_t>(s + off)); }

// ------------------------------------------------------------------------------------------ allocate / deallocate
// allocate(n): a buffer for n elements, begin = end; more than max_size throws length_error
template <uint32_t Size> void allocate(uint32_t v, uint32_t n) {
    if (n > maxSize<Size>()) {
        rw::call(THROW_LENGTH_ERROR, {v});
        return;
    }
    const uint32_t p = guest::newObject(n * Size);
    save<uint32_t>(v, p);
    save<uint32_t>(v + 4, p);
    save<uint32_t>(v + 8, p + n * Size);
}

void forgetBuffer(uint32_t v) {
    save<uint32_t>(v, 0);
    save<uint32_t>(v + 4, 0);
    save<uint32_t>(v + 8, 0);
}
// deallocate(): the elements destroyed (Destroy, from the back), the buffer freed, the vector emptied
template <uint32_t Size> void deallocate(uint32_t v) {
    if (!load<uint32_t>(v)) return;
    destroyVector<Size>(vec(v));
    forgetBuffer(v);
}
template <uint32_t Size, void (*Destroy)(uint32_t)> void deallocateWith(uint32_t v) {
    if (!load<uint32_t>(v)) return;
    destroyVector<Size>(vec(v), Destroy);
    forgetBuffer(v);
}

// ------------------------------------------------------------------------------------------ element destructors
void stringElement(uint32_t e) { destroyString(e); }
void sharedElement(uint32_t e) { destroyShared(e); }
void raidBaseDestroy(uint32_t e) { destroyString(e + 0xB0); }
void partyExDestroy(uint32_t e) {
    destroyString(e + 0x60);
    rw::call(VectorU64Base_dtor, {e + 0x40});
    destroyString(e + 0x24);
    destroyString(e + 0x10);
}
void chatStampDestroy(uint32_t e) { destroyString(e + 8); }
void chatBlockDestroy(uint32_t e) { destroyString(e + 0x10); }
void chatMessageDestroy(uint32_t e) {
    destroyString(e + 0x2C);
    destroyString(e + 0x14);
}
void movieHandlerDestroy(uint32_t e) { destroyFunction(e + 8); }   // pair<int, function<void(LWF::Movie*)>>

// ------------------------------------------------------------------------------------------ element copies
// copy(dst, src): one element copy-constructed into raw memory
template <uint32_t Bytes> void words(uint32_t d, uint32_t s) { libcxx::copyElement<Bytes>(d, s); }
void stringCopy(uint32_t d, uint32_t s) { copyString(d, s); }
void sharedCopy(uint32_t d, uint32_t s) { copyShared(d, s); }
void storyCopy(uint32_t d, uint32_t s) {   // Stage::Story: 3 words, a vector<int>
    copyWords(d, s, 12);
    rw::call(VectorInt_copy, {d + 0xC, s + 0xC});
}
void raidBaseCopy(uint32_t d, uint32_t s) {   // RaidBase: 0xA8 bytes, 2 words, a string, 13 words (the first part twice)
    copyBytes(d, s, 0xA8);
    copyWords(d + 0xA8, s + 0xA8, 8);
    copyString(d + 0xB0, s + 0xB0);
    copyWords(d + 0xBC, s + 0xBC, 0x34);
    copyBytes(d, s, 0xA8);
}
void partyExCopy(uint32_t d, uint32_t s) {   // PartyApplicationList::PartyEx: the Party, then 0x128 plain bytes
    rw::call(Party_copy, {d, s});
    copyBytes(d + 0x70, s + 0x70, 0x128);
}
void applicantCopy(uint32_t d, uint32_t s) {   // PartyApplication::Applicant / PartyInviteList::Offer
    copyWords(d, s, 16);
    copyString(d + 0x10, s + 0x10);
    copyWords(d + 0x1C, s + 0x1C, 16);
    copyWords(d + 0x2C, s + 0x2C, 12);
}
void chatStampCopy(uint32_t d, uint32_t s) {   // a byte at 4, a string at 8, 8 single bytes at 0x14
    copyByte(d, s, 4);
    copyWords(d, s, 4);
    copyString(d + 8, s + 8);
    for (uint32_t off = 0x14; off < 0x1C; off++) copyByte(d, s, off);
}
void chatBlockCopy(uint32_t d, uint32_t s) {   // 3 words, a halfword, a string at 0x10
    copyWords(d, s, 12);
    save<uint16_t>(d + 0xC, load<uint16_t>(s + 0xC));
    copyString(d + 0x10, s + 0x10);
}
void chatMessageCopy(uint32_t d, uint32_t s) {
    copyWords(d, s, 16);
    copyByte(d, s, 0x10);
    copyString(d + 0x14, s + 0x14);
    copyBytes(d + 0x20, s + 0x20, 12);
    copyString(d + 0x2C, s + 0x2C);
    copyByte(d, s, 0x38);
}
void cooperatorCopy(uint32_t d, uint32_t s) {   // RaidMember::Cooperator
    copyWords(d, s, 8);
    copyString(d + 8, s + 8);
    copyWords(d + 0x14, s + 0x14, 0x38);
}
void playerCopy(uint32_t d, uint32_t s) {   // RaidStart::Player
    copyWords(d, s, 0x34);
    copyString(d + 0x34, s + 0x34);
    copyWords(d + 0x40, s + 0x40, 8);
}
void eventCopy(uint32_t d, uint32_t s) {   // StageEvent::Event: a vector<Story>, then plain members and a string
    rw::call(VectorStory_copy, {d, s});
    copyByte(d, s, 0x10);
    copyWords(d + 0xC, s + 0xC, 4);
    copyString(d + 0x14, s + 0x14);
    copyWords(d + 0x20, s + 0x20, 16);
    copyByte(d, s, 0x34);
    copyWords(d + 0x30, s + 0x30, 4);
}
void styleCopy(uint32_t d, uint32_t s) {   // Avatar::Style: a SafeBuffer
    rw::call(SafeBuffer_copy, {d, s});
    save<uint32_t>(d, AvatarStyle_vtable);
}
void rankerCopy(uint32_t d, uint32_t s) {   // UnionRanking::Ranker: two shared_ptrs, 0x60 plain bytes
    copyShared(d, s);
    copyShared(d + 8, s + 8);
    copyBytes(d + 0x10, s + 0x10, 0x60);
}
void gachaCopy(uint32_t d, uint32_t s) { rw::call(Gacha_copy, {d, s}); }

// ------------------------------------------------------------------------------------------ construction
// __construct_at_end(first, last): each element copied into the slot at end (none when end is null), end advanced
template <uint32_t Size, void (*Copy)(uint32_t, uint32_t)> void constructAtEnd(uint32_t v, uint32_t first, uint32_t last) {
    for (; first != last; first += Size) {
        if (const uint32_t slot = load<uint32_t>(v + 4)) Copy(slot, first);
        save<uint32_t>(v + 4, load<uint32_t>(v + 4) + Size);
    }
}

// __construct_at_end(deque iterator first, last) of shared_ptr<RESTClient::Connection>: the iterators are
// {block-map pointer, element}; only the elements are compared
void connectionsFromDeque(uint32_t v, uint32_t map, uint32_t p, uint32_t, uint32_t last) {
    while (p != last) {
        if (const uint32_t slot = load<uint32_t>(v + 4)) copyShared(slot, p);
        save<uint32_t>(v + 4, load<uint32_t>(v + 4) + 8);
        p += 8;
        if (p - load<uint32_t>(map) == DEQUE_BLOCK_BYTES) {
            map += 4;
            p = load<uint32_t>(map);
        }
    }
}

// vector(const vector&) through the instance's allocate and __construct_at_end
template <uint32_t Size, uint32_t Allocate, uint32_t ConstructAtEnd> uint32_t copyConstruct(uint32_t v, uint32_t src) {
    forgetBuffer(v);
    const uint32_t begin = load<uint32_t>(src), end = load<uint32_t>(src + 4);
    if (end != begin) {
        rw::call(Allocate, {v, (end - begin) / Size});
        rw::call(ConstructAtEnd, {v, load<uint32_t>(src), load<uint32_t>(src + 4)});
    }
    return v;
}

// vector(const vector&) with __construct_at_end inlined
template <uint32_t Size, uint32_t Allocate, void (*Copy)(uint32_t, uint32_t)> uint32_t copyConstructInline(uint32_t v, uint32_t src) {
    forgetBuffer(v);
    const uint32_t n = (load<uint32_t>(src + 4) - load<uint32_t>(src)) / Size;
    if (n) {
        rw::call(Allocate, {v, n});
        constructAtEnd<Size, Copy>(v, load<uint32_t>(src), load<uint32_t>(src + 4));
    }
    return v;
}

// vector<MapJumpInfo*>(n): n null pointers
uint32_t nullPointers(uint32_t v, uint32_t n) {
    forgetBuffer(v);
    if (n) {
        rw::call(0x75D6FF, {v, n});   // allocate
        for (; n; n--) {
            if (const uint32_t slot = load<uint32_t>(v + 4)) save<uint32_t>(slot, 0);
            save<uint32_t>(v + 4, load<uint32_t>(v + 4) + 4);
        }
    }
    return v;
}

// vector<shared_ptr<MedalDataManager::UserMedalData>>::__move_range(from_s, from_e, to): the tail past the old end
// move-constructed there, the rest move-assigned backwards (move_backward: each assignment releases the old owner)
void sharedMoveRange(uint32_t v, uint32_t fromS, uint32_t fromE, uint32_t to) {
    const uint32_t oldLast = load<uint32_t>(v + 4);
    const uint32_t n = oldLast - to;
    for (uint32_t i = fromS + (n & ~7u); i < fromE; i += 8) {
        if (const uint32_t slot = load<uint32_t>(v + 4)) libcxx::moveShared(slot, i);
        save<uint32_t>(v + 4, load<uint32_t>(v + 4) + 8);
    }
    const int32_t count = int32_t(n) >> 3;
    for (int32_t m = 0; m < count; m++) {
        const uint32_t src = fromS + uint32_t(count - 1 - m) * 8, dst = oldLast - 8 - uint32_t(m) * 8;
        const uint32_t ctrl = load<uint32_t>(src + 4), obj = load<uint32_t>(src);
        save<uint32_t>(src, 0);
        save<uint32_t>(src + 4, 0);
        save<uint32_t>(dst, obj);
        const uint32_t old = load<uint32_t>(dst + 4);
        save<uint32_t>(dst + 4, ctrl);
        if (old) guest::release(old);
    }
}

// ...::__swap_out_circular_buffer(sb, p): the elements before p moved in front of sb.begin, those from p on after
// sb.end, the buffers swapped -> where p's element now is (the split buffer's old begin)
uint32_t sharedSwapOut(uint32_t v, uint32_t sb, uint32_t p) {
    const uint32_t r = load<uint32_t>(sb + 4);
    const uint32_t begin = load<uint32_t>(v);
    for (uint32_t s = p; s != begin;) {
        s -= 8;
        libcxx::moveShared(load<uint32_t>(sb + 4) - 8, s);
        save<uint32_t>(sb + 4, load<uint32_t>(sb + 4) - 8);
    }
    const uint32_t end = load<uint32_t>(v + 4);
    for (uint32_t s = p; s != end; s += 8) {
        if (const uint32_t slot = load<uint32_t>(sb + 8)) libcxx::moveShared(slot, s);
        save<uint32_t>(sb + 8, load<uint32_t>(sb + 8) + 8);
    }
    for (uint32_t k = 0; k < 12; k += 4) {
        const uint32_t mine = load<uint32_t>(v + k);
        save<uint32_t>(v + k, load<uint32_t>(sb + 4 + k));
        save<uint32_t>(sb + 4 + k, mine);
    }
    save<uint32_t>(sb, load<uint32_t>(sb + 4));
    return r;
}

// reserve(n) of pointers (SplitDtor: ~__split_buffer<T*, allocator<T*>&>)
template <uint32_t SplitDtor> void reservePointers(uint32_t v, uint32_t n) { libcxx::reserveRelocating<4>(vec(v), n, SplitDtor); }

// ------------------------------------------------------------------------------------------ assign(first, last)
constexpr uint32_t AEABI_MEMMOVE = 0x40EF50;    // __aeabi_memmove(dst, src, n) (import)
constexpr uint32_t STRING_ASSIGN = 0x498A1D;    // basic_string::operator=(const basic_string&) (sub_498A1C)

// more than the capacity: the buffer replaced (the instance's deallocate, __recommend, allocate)
template <uint32_t Size, uint32_t Deallocate, uint32_t Allocate> bool reallocateFor(uint32_t v, uint32_t n) {
    rw::call(Deallocate, {v});
    if (n > maxSize<Size>()) {
        rw::call(THROW_LENGTH_ERROR, {v});
        return false;
    }
    rw::call(Allocate, {v, libcxx::recommend<Size>(vec(v), n)});
    return true;
}

// assign(first, last) of trivially copyable elements: memmove over the live ones, the rest copied in at the end
template <uint32_t Size, uint32_t Deallocate, uint32_t Allocate> void assignTrivial(uint32_t v, uint32_t first, uint32_t last) {
    const uint32_t n = (last - first) / Size;
    const uint32_t begin = load<uint32_t>(v);
    if (n > (load<uint32_t>(v + 8) - begin) / Size) {
        if (reallocateFor<Size, Deallocate, Allocate>(v, n)) constructAtEnd<Size, words<Size>>(v, first, last);
        return;
    }
    const uint32_t liveBytes = load<uint32_t>(v + 4) - begin;
    if (n > liveBytes / Size) {
        rw::call(AEABI_MEMMOVE, {begin, first, liveBytes});
        constructAtEnd<Size, words<Size>>(v, first + liveBytes / Size * Size, last);
        return;
    }
    rw::call(AEABI_MEMMOVE, {begin, first, last - first});
    const uint32_t m = begin + ((last - first) & ~(Size - 1));
    const uint32_t end = load<uint32_t>(v + 4);
    if (end != m) save<uint32_t>(v + 4, end + (~(end - Size - m) & ~(Size - 1)));
}

// assign(first, last) of other elements: assigned over the live ones (assign(dst, src)), then the rest constructed
// by the instance's __construct_at_end, or the surplus destroyed from the back
template <uint32_t Size, uint32_t Deallocate, uint32_t Allocate, uint32_t ConstructAtEnd, void (*Assign)(uint32_t, uint32_t),
          void (*Destroy)(uint32_t)>
void assignElements(uint32_t v, uint32_t first, uint32_t last) {
    const uint32_t n = (last - first) / Size;
    const uint32_t begin = load<uint32_t>(v);
    if (n > (load<uint32_t>(v + 8) - begin) / Size) {
        if (reallocateFor<Size, Deallocate, Allocate>(v, n)) rw::call(ConstructAtEnd, {v, first, last});
        return;
    }
    const uint32_t size = (load<uint32_t>(v + 4) - begin) / Size;
    const bool growing = n > size;
    const uint32_t mid = growing ? first + size * Size : last;
    uint32_t m = begin;
    for (uint32_t i = first; i != mid; i += Size, m += Size) Assign(m, i);
    if (growing) {
        rw::call(ConstructAtEnd, {v, mid, last});
        return;
    }
    while (load<uint32_t>(v + 4) != m) {
        const uint32_t e = load<uint32_t>(v + 4) - Size;
        save<uint32_t>(v + 4, e);
        Destroy(e);
    }
}

// element assignments (operator=, inlined)
void stringAssign(uint32_t d, uint32_t s) { rw::call(STRING_ASSIGN, {d, s}); }
void sharedAssign(uint32_t d, uint32_t s) {   // shared_ptr::operator=(const&): one more owner, the old one released
    const uint32_t obj = load<uint32_t>(s), ctrl = load<uint32_t>(s + 4);
    if (ctrl) guest::addRef(ctrl);
    save<uint32_t>(d, obj);
    const uint32_t old = load<uint32_t>(d + 4);
    save<uint32_t>(d + 4, ctrl);
    if (old) guest::release(old);
}
void raidBaseAssign(uint32_t d, uint32_t s) {   // 0xA4 bytes (not the padding word at 0xA4), 2 words, a string, 13 words
    copyBytes(d, s, 0xA4);
    copyWords(d + 0xA8, s + 0xA8, 8);
    stringAssign(d + 0xB0, s + 0xB0);
    copyWords(d + 0xBC, s + 0xBC, 0x34);
}
void chatStampAssign(uint32_t d, uint32_t s) {
    copyByte(d, s, 4);
    copyWords(d, s, 4);
    stringAssign(d + 8, s + 8);
    for (uint32_t off = 0x14; off < 0x1C; off++) copyByte(d, s, off);
}
void chatBlockAssign(uint32_t d, uint32_t s) {
    copyWords(d, s, 12);
    save<uint16_t>(d + 0xC, load<uint16_t>(s + 0xC));
    stringAssign(d + 0x10, s + 0x10);
}
void chatMessageAssign(uint32_t d, uint32_t s) {
    copyWords(d, s, 16);
    copyByte(d, s, 0x10);
    stringAssign(d + 0x14, s + 0x14);
    copyBytes(d + 0x20, s + 0x20, 12);
    stringAssign(d + 0x2C, s + 0x2C);
    copyByte(d, s, 0x38);
}

// vector<shared_ptr<RESTClient::Connection>>(deque iterator first, last): {block-map pointer, element} pairs
uint32_t connectionsFromDequeRange(uint32_t v, uint32_t firstMap, uint32_t first, uint32_t lastMap, uint32_t last) {
    forgetBuffer(v);
    if (last == first) return v;
    const int32_t n = int32_t(lastMap - firstMap) * 128 + int32_t(last - load<uint32_t>(lastMap)) / 8;
    const int32_t skip = int32_t(first - load<uint32_t>(firstMap)) / 8;
    if (n == skip) return v;
    rw::call(0x49A6E9, {v, uint32_t(n - skip)});                       // allocate
    rw::call(0x49A711, {v, firstMap, first, lastMap, last});           // __construct_at_end(first, last)
    return v;
}

// vector<const char*>(first, last)
uint32_t cstringsFromRange(uint32_t v, uint32_t first, uint32_t last) {
    forgetBuffer(v);
    if (const uint32_t n = (last - first) / 4) {
        rw::call(0x5D5515, {v, n});   // allocate
        constructAtEnd<4, words<4>>(v, first, last);
    }
    return v;
}

// vector<Avatar::Style>(first, last)
uint32_t stylesFromRange(uint32_t v, uint32_t first, uint32_t last) {
    forgetBuffer(v);
    if (const uint32_t n = (last - first) / 0x20) {
        rw::call(0x736785, {v, n});                 // allocate
        rw::call(0x7367AD, {v, first, last});       // __construct_at_end
    }
    return v;
}

// vector<LWF::Movie*>::resize(n): __append the missing nulls, or forget the surplus
void moviesResize(uint32_t v, uint32_t n) {
    const uint32_t begin = load<uint32_t>(v), end = load<uint32_t>(v + 4);
    const uint32_t size = (end - begin) / 4;
    if (size < n) {
        rw::call(0x7398ED, {v, n - size});   // __append
    } else if (size > n) {
        const uint32_t m = begin + n * 4;
        if (end != m) save<uint32_t>(v + 4, end + (~(end - 4 - m) & ~3u));
    }
}

// vector<LWF::Movie*>::__append(n): n null pointers, through a split buffer when they do not fit
void moviesAppend(uint32_t v, uint32_t n) {
    if ((load<uint32_t>(v + 8) - load<uint32_t>(v + 4)) / 4 >= n) {
        for (; n; n--) {
            if (const uint32_t slot = load<uint32_t>(v + 4)) save<uint32_t>(slot, 0);
            save<uint32_t>(v + 4, load<uint32_t>(v + 4) + 4);
        }
        return;
    }
    const uint32_t size = (load<uint32_t>(v + 4) - load<uint32_t>(v)) / 4;
    if (size + n > maxSize<4>()) {
        rw::call(THROW_LENGTH_ERROR, {v});
        return;
    }
    const uint32_t cap = libcxx::recommend<4>(vec(v), size + n);
    guest::Frame frame(20);
    const uint32_t sb = frame.addr();
    save<uint32_t>(sb + 0xC, 0);
    save<uint32_t>(sb + 0x10, v + 8);
    const uint32_t buf = cap ? guest::newObject(cap * 4) : 0;
    save<uint32_t>(sb, buf);
    save<uint32_t>(sb + 4, buf + size * 4);
    save<uint32_t>(sb + 8, buf + size * 4);
    save<uint32_t>(sb + 0xC, buf + cap * 4);
    for (; n; n--) {
        if (const uint32_t slot = load<uint32_t>(sb + 8)) save<uint32_t>(slot, 0);
        save<uint32_t>(sb + 8, load<uint32_t>(sb + 8) + 4);
    }
    libcxx::swapOutRelocating<4>(vec(v), sb, buf, size, load<uint32_t>(sb + 8), 0x739A29);
}

// vector<ResourceInfo record>(const vector&): the buffer allocated inline; a record is two strings and a word
uint32_t recordsCopy(uint32_t v, uint32_t src) {
    forgetBuffer(v);
    const uint32_t begin = load<uint32_t>(src), end = load<uint32_t>(src + 4);
    if (end == begin) return v;
    const uint32_t n = (end - begin) / 0x1C;
    if (n > maxSize<0x1C>()) {
        rw::call(THROW_LENGTH_ERROR, {v});
        return v;
    }
    const uint32_t p = guest::newObject(end - begin);
    save<uint32_t>(v, p);
    save<uint32_t>(v + 4, p);
    save<uint32_t>(v + 8, p + n * 0x1C);
    const uint32_t last = load<uint32_t>(src + 4);
    for (uint32_t s = load<uint32_t>(src); s != last; s += 0x1C) {
        if (const uint32_t slot = load<uint32_t>(v + 4)) {
            copyString(slot, s);
            copyString(slot + 0xC, s + 0xC);
            save<uint32_t>(slot + 0x18, load<uint32_t>(s + 0x18));
        }
        save<uint32_t>(v + 4, load<uint32_t>(v + 4) + 0x1C);
    }
    return v;
}

}  // namespace

// ---------------------------------------------------------------------------------------------- allocate
RW_FUNCTION(0x47B5FC, "std::vector<string>::allocate", allocate<12>);
RW_FUNCTION(0x47D6DE, "std::vector<int>::allocate", allocate<4>);
RW_FUNCTION(0x487634, "std::vector<Avatar::Expression>::allocate", allocate<4>);
RW_FUNCTION(0x48D7E8, "std::vector<unsigned long long>::allocate", allocate<8>);
RW_FUNCTION(0x496602, "std::vector<MapNonPlayableCharacter*>::allocate", allocate<4>);
RW_FUNCTION(0x49662A, "std::vector<MapPlacementObject*>::allocate", allocate<4>);
RW_FUNCTION(0x49A6E8, "std::vector<shared_ptr<RESTClient::Connection>>::allocate", allocate<8>);
RW_FUNCTION(0x49D822, "std::vector<char>::allocate", allocate<1>);
RW_FUNCTION(0x4B5C1C, "std::vector<Stage::Story>::allocate", allocate<0x18>);
RW_FUNCTION(0x503450, "std::vector<shared_ptr<const master::Sphere>>::allocate", allocate<8>);
RW_FUNCTION(0x509868, "std::vector<RaidBase>::allocate", allocate<0xF0>);
RW_FUNCTION(0x51B908, "std::vector<vector<unsigned long long>>::allocate", allocate<12>);
RW_FUNCTION(0x51B938, "std::vector<vector<int>>::allocate", allocate<12>);
RW_FUNCTION(0x52069C, "std::vector<PartyApplicationList::PartyEx>::allocate", allocate<0x198>);
RW_FUNCTION(0x5254E8, "std::vector<PartyApplication::Applicant>::allocate", allocate<0x38>);
RW_FUNCTION(0x525590, "std::vector<PartyInviteList::Offer>::allocate", allocate<0x38>);
RW_FUNCTION(0x52E618, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::allocate", allocate<8>);
RW_FUNCTION(0x54F238, "std::vector<Colosseum::Stage>::allocate", allocate<16>);
RW_FUNCTION(0x55CA2C, "std::vector<ChatStampListScrollView::ChatStamp>::allocate", allocate<0x1C>);
RW_FUNCTION(0x55CF3C, "std::vector<ChatBlockListScrollView::ChatBlock>::allocate", allocate<0x1C>);
RW_FUNCTION(0x55D57C, "std::vector<ChatScrollView::ChatMessage>::allocate", allocate<0x40>);
RW_FUNCTION(0x599BD4, "std::vector<shared_ptr<const master::EvCampaign>>::allocate", allocate<8>);
RW_FUNCTION(0x5B39CC, "std::vector<GachaList::Gacha>::allocate", allocate<0x48>);
RW_FUNCTION(0x5B9B48, "std::vector<RaidMember::Cooperator>::allocate", allocate<0x50>);
RW_FUNCTION(0x5D5514, "std::vector<const char*>::allocate", allocate<4>);
RW_FUNCTION(0x5F7070, "std::vector<RaidStart::Player>::allocate", allocate<0x48>);
RW_FUNCTION(0x60B13E, "std::vector<MapPrizes*>::allocate", allocate<4>);
RW_FUNCTION(0x701B9C, "std::vector<RankingReward::Result>::allocate", allocate<12>);
RW_FUNCTION(0x72DA84, "std::vector<StageEvent::Event>::allocate", allocate<0x38>);
RW_FUNCTION(0x736784, "std::vector<Avatar::Style>::allocate", allocate<0x20>);
RW_FUNCTION(0x737BD8, "std::vector<UnionRanking::Ranker>::allocate", allocate<0x70>);
RW_FUNCTION(0x75D6FE, "std::vector<MapJumpInfo*>::allocate", allocate<4>);

// ---------------------------------------------------------------------------------------------- deallocate
RW_FUNCTION(0x47D6B0, "std::vector<int>::deallocate", deallocate<4>);
RW_FUNCTION(0x4965D4, "std::vector<MapNonPlayableCharacter*>::deallocate", deallocate<4>);
RW_FUNCTION(0x49D228, "std::vector<string>::deallocate", (deallocateWith<12, stringElement>));
RW_FUNCTION(0x49D800, "std::vector<char>::deallocate", deallocate<1>);
RW_FUNCTION(0x4B33C4, "std::vector<unsigned long long>::deallocate", deallocate<8>);
RW_FUNCTION(0x4D5538, "std::vector<RaidBase>::deallocate", (deallocateWith<0xF0, raidBaseDestroy>));
RW_FUNCTION(0x4D5588, "std::vector<PartyApplicationList::PartyEx>::deallocate", (deallocateWith<0x198, partyExDestroy>));
RW_FUNCTION(0x50340A, "std::vector<shared_ptr<const master::Sphere>>::deallocate", (deallocateWith<8, sharedElement>));
RW_FUNCTION(0x52E5D2, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::deallocate", (deallocateWith<8, sharedElement>));
RW_FUNCTION(0x55C9DE, "std::vector<ChatStampListScrollView::ChatStamp>::deallocate", (deallocateWith<0x1C, chatStampDestroy>));
RW_FUNCTION(0x55CEEE, "std::vector<ChatBlockListScrollView::ChatBlock>::deallocate", (deallocateWith<0x1C, chatBlockDestroy>));
RW_FUNCTION(0x55D51A, "std::vector<ChatScrollView::ChatMessage>::deallocate", (deallocateWith<0x40, chatMessageDestroy>));
RW_FUNCTION(0x599B8E, "std::vector<shared_ptr<const master::EvCampaign>>::deallocate", (deallocateWith<8, sharedElement>));
RW_FUNCTION(0x60B110, "std::vector<MapPrizes*>::deallocate", deallocate<4>);
RW_FUNCTION(0x6A2E64, "std::vector<cocos2d::ui::Widget*>::deallocate", deallocate<4>);
RW_FUNCTION(0x80A74C, "std::vector<pair<int, function<void(LWF::Movie*)>>>::deallocate", (deallocateWith<0x20, movieHandlerDestroy>));

// ---------------------------------------------------------------------------------------------- __construct_at_end
RW_FUNCTION(0x47B62C, "std::vector<string>::__construct_at_end(string*, string*)", (constructAtEnd<12, stringCopy>));
RW_FUNCTION(0x5E407C, "std::vector<string>::__construct_at_end(const string*, const string*)", (constructAtEnd<12, stringCopy>));
RW_FUNCTION(0x49A710, "std::vector<shared_ptr<RESTClient::Connection>>::__construct_at_end(deque iterator)", connectionsFromDeque);
RW_FUNCTION(0x4B5C4C, "std::vector<Stage::Story>::__construct_at_end", (constructAtEnd<0x18, storyCopy>));
RW_FUNCTION(0x5033D4, "std::vector<shared_ptr<const master::Sphere>>::__construct_at_end", (constructAtEnd<8, sharedCopy>));
RW_FUNCTION(0x50977C, "std::vector<RaidBase>::__construct_at_end", (constructAtEnd<0xF0, raidBaseCopy>));
RW_FUNCTION(0x5206CC, "std::vector<PartyApplicationList::PartyEx>::__construct_at_end", (constructAtEnd<0x198, partyExCopy>));
RW_FUNCTION(0x525518, "std::vector<PartyApplication::Applicant>::__construct_at_end", (constructAtEnd<0x38, applicantCopy>));
RW_FUNCTION(0x5255C0, "std::vector<PartyInviteList::Offer>::__construct_at_end", (constructAtEnd<0x38, applicantCopy>));
RW_FUNCTION(0x52E59C, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::__construct_at_end", (constructAtEnd<8, sharedCopy>));
RW_FUNCTION(0x55C958, "std::vector<ChatStampListScrollView::ChatStamp>::__construct_at_end", (constructAtEnd<0x1C, chatStampCopy>));
RW_FUNCTION(0x55CE9C, "std::vector<ChatBlockListScrollView::ChatBlock>::__construct_at_end", (constructAtEnd<0x1C, chatBlockCopy>));
RW_FUNCTION(0x55D45C, "std::vector<ChatScrollView::ChatMessage>::__construct_at_end", (constructAtEnd<0x40, chatMessageCopy>));
RW_FUNCTION(0x599B58, "std::vector<shared_ptr<const master::EvCampaign>>::__construct_at_end", (constructAtEnd<8, sharedCopy>));
RW_FUNCTION(0x5B9B78, "std::vector<RaidMember::Cooperator>::__construct_at_end", (constructAtEnd<0x50, cooperatorCopy>));
RW_FUNCTION(0x5F70A0, "std::vector<RaidStart::Player>::__construct_at_end", (constructAtEnd<0x48, playerCopy>));
RW_FUNCTION(0x72DAB4, "std::vector<StageEvent::Event>::__construct_at_end", (constructAtEnd<0x38, eventCopy>));
RW_FUNCTION(0x7367AC, "std::vector<Avatar::Style>::__construct_at_end", (constructAtEnd<0x20, styleCopy>));
RW_FUNCTION(0x737C08, "std::vector<UnionRanking::Ranker>::__construct_at_end", (constructAtEnd<0x70, rankerCopy>));

// ---------------------------------------------------------------------------------------------- constructors
RW_FUNCTION(0x4AF706, "std::vector<int>::vector(const vector&)", (copyConstructInline<4, 0x47D6DF, words<4>>));
RW_FUNCTION(0x495E74, "std::vector<MapPlacementObject*>::vector(const vector&)", (copyConstructInline<4, 0x49662B, words<4>>));
RW_FUNCTION(0x5CA800, "std::vector<Avatar::Expression>::vector(const vector&)", (copyConstructInline<4, 0x487635, words<4>>));
RW_FUNCTION(0x54E488, "std::vector<Colosseum::Stage>::vector(const vector&)", (copyConstructInline<16, 0x54F239, words<16>>));
RW_FUNCTION(0x700BD0, "std::vector<RankingReward::Result>::vector(const vector&)", (copyConstructInline<12, 0x701B9D, words<12>>));
RW_FUNCTION(0x5AEBF0, "std::vector<GachaList::Gacha>::vector(const vector&)", (copyConstructInline<0x48, 0x5B39CD, gachaCopy>));
RW_FUNCTION(0x4B5BC8, "std::vector<Stage::Story>::vector(const vector&)", (copyConstruct<0x18, 0x4B5C1D, 0x4B5C4D>));
RW_FUNCTION(0x51EE90, "std::vector<PartyApplicationList::PartyEx>::vector(const vector&)", (copyConstruct<0x198, 0x52069D, 0x5206CD>));
RW_FUNCTION(0x5232D0, "std::vector<PartyInviteList::Offer>::vector(const vector&)", (copyConstruct<0x38, 0x525591, 0x5255C1>));
RW_FUNCTION(0x523324, "std::vector<PartyApplication::Applicant>::vector(const vector&)", (copyConstruct<0x38, 0x5254E9, 0x525519>));
RW_FUNCTION(0x5B9AF4, "std::vector<RaidMember::Cooperator>::vector(const vector&)", (copyConstruct<0x50, 0x5B9B49, 0x5B9B79>));
RW_FUNCTION(0x5F4A04, "std::vector<RaidStart::Player>::vector(const vector&)", (copyConstruct<0x48, 0x5F7071, 0x5F70A1>));
RW_FUNCTION(0x617428, "std::vector<ChatScrollView::ChatMessage>::vector(const vector&)", (copyConstruct<0x40, 0x55D57D, 0x55D45D>));
RW_FUNCTION(0x6190F0, "std::vector<ChatStampListScrollView::ChatStamp>::vector(const vector&)", (copyConstruct<0x1C, 0x55CA2D, 0x55C959>));
RW_FUNCTION(0x61E6A4, "std::vector<ChatBlockListScrollView::ChatBlock>::vector(const vector&)", (copyConstruct<0x1C, 0x55CF3D, 0x55CE9D>));
RW_FUNCTION(0x665538, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::vector(const vector&)", (copyConstruct<8, 0x52E619, 0x52E59D>));
RW_FUNCTION(0x6B8928, "std::vector<shared_ptr<const master::Sphere>>::vector(const vector&)", (copyConstruct<8, 0x503451, 0x5033D5>));
RW_FUNCTION(0x723C98, "std::vector<StageEvent::Event>::vector(const vector&)", (copyConstruct<0x38, 0x72DA85, 0x72DAB5>));
RW_FUNCTION(0x735FB0, "std::vector<UnionRanking::Ranker>::vector(const vector&)", (copyConstruct<0x70, 0x737BD9, 0x737C09>));
RW_FUNCTION(0x75C1A8, "std::vector<MapJumpInfo*>::vector(size_t)", nullPointers);

// ---------------------------------------------------------------------------------------------- reserve, insert helpers
RW_FUNCTION(0x75BC00, "std::vector<MapEnemyTeamInfo*>::reserve", reservePointers<0x75DD29>);
RW_FUNCTION(0x75BE94, "std::vector<MapTransferInfo*>::reserve", reservePointers<0x75DBED>);
RW_FUNCTION(0x75BF54, "std::vector<MapTreasure::Info*>::reserve", reservePointers<0x75DAB1>);
RW_FUNCTION(0x75C014, "std::vector<MapMaterial::Info*>::reserve", reservePointers<0x75D975>);
RW_FUNCTION(0x75C0E8, "std::vector<MapNonPlayableCharacter::Info*>::reserve", reservePointers<0x75D839>);
RW_FUNCTION(0x52FF44, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::__move_range", sharedMoveRange);
RW_FUNCTION(0x52FFD2, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::__swap_out_circular_buffer(sb, p)", sharedSwapOut);

// ---------------------------------------------------------------------------------------------- assign(first, last)
RW_FUNCTION(0x47D5BC, "std::vector<int>::assign(int*, int*)", (assignTrivial<4, 0x47D6B1, 0x47D6DF>));
RW_FUNCTION(0x4964E0, "std::vector<MapNonPlayableCharacter*>::assign", (assignTrivial<4, 0x4965D5, 0x496603>));
RW_FUNCTION(0x60B01C, "std::vector<MapPrizes*>::assign", (assignTrivial<4, 0x60B111, 0x60B13F>));
RW_FUNCTION(0x49D724, "std::vector<char>::assign(const char*, const char*)", (assignTrivial<1, 0x49D801, 0x49D823>));
RW_FUNCTION(0x4D7460, "std::vector<unsigned long long>::assign", (assignTrivial<8, 0x4B33C5, 0x48D7E9>));
RW_FUNCTION(0x49D100, "std::vector<string>::assign", (assignElements<12, 0x49D229, 0x47B5FD, 0x47B62D, stringAssign, stringElement>));
RW_FUNCTION(0x5032B8, "std::vector<shared_ptr<const master::Sphere>>::assign", (assignElements<8, 0x50340B, 0x503451, 0x5033D5, sharedAssign, sharedElement>));
RW_FUNCTION(0x52E480, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::assign", (assignElements<8, 0x52E5D3, 0x52E619, 0x52E59D, sharedAssign, sharedElement>));
RW_FUNCTION(0x599A3C, "std::vector<shared_ptr<const master::EvCampaign>>::assign", (assignElements<8, 0x599B8F, 0x599BD5, 0x599B59, sharedAssign, sharedElement>));
RW_FUNCTION(0x5095C4, "std::vector<RaidBase>::assign", (assignElements<0xF0, 0x4D5539, 0x509869, 0x50977D, raidBaseAssign, raidBaseDestroy>));
RW_FUNCTION(0x55C7E8, "std::vector<ChatStampListScrollView::ChatStamp>::assign", (assignElements<0x1C, 0x55C9DF, 0x55CA2D, 0x55C959, chatStampAssign, chatStampDestroy>));
RW_FUNCTION(0x55CD60, "std::vector<ChatBlockListScrollView::ChatBlock>::assign", (assignElements<0x1C, 0x55CEEF, 0x55CF3D, 0x55CE9D, chatBlockAssign, chatBlockDestroy>));
RW_FUNCTION(0x55D308, "std::vector<ChatScrollView::ChatMessage>::assign", (assignElements<0x40, 0x55D51B, 0x55D57D, 0x55D45D, chatMessageAssign, chatMessageDestroy>));
RW_FUNCTION(0x498F6C, "std::vector<shared_ptr<RESTClient::Connection>>::vector(deque iterator, deque iterator)", connectionsFromDequeRange);
RW_FUNCTION(0x5D4F74, "std::vector<const char*>::vector(const char**, const char**)", cstringsFromRange);
RW_FUNCTION(0x736604, "std::vector<Avatar::Style>::vector(Style*, Style*)", stylesFromRange);
RW_FUNCTION(0x7384D8, "std::vector<LWF::Movie*>::resize", moviesResize);
RW_FUNCTION(0x7398EC, "std::vector<LWF::Movie*>::__append", moviesAppend);
RW_FUNCTION(0x7761AC, "std::vector<ResourceInfo record>::vector(const vector&)", recordsCopy);
