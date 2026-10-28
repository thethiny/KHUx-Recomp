// std::sort instances (libc++ 3.5's introsort: std::__sort with __sort3 / __sort4 / __sort5, __insertion_sort_3
// and __insertion_sort_incomplete). With std::less (int, unsigned long long, APIManager::Timezone) the comparison is
// inlined and the comparator reference is passed along unused; with a stateless lambda (the stamp and avatar-parts
// lists) every comparison calls the lambda with the two elements' addresses, no comparator is passed, and __sort
// inlines __insertion_sort_3. The helpers call one another out of line as the compiled code does.
#include "libcxx/Libcxx.hpp"

using rw::load;
using rw::save;

namespace {

// element kinds: Size bytes, read / written as Value
// (Limit: the longest range __sort leaves to insertion sort - 30 for trivially copyable values, else 6)
struct IntElem {
    using Value = int32_t;
    static constexpr uint32_t Size = 4;
    static constexpr int32_t Limit = 30;
    static Value get(uint32_t p) { return int32_t(load<uint32_t>(p)); }
    static void put(uint32_t p, Value v) { save<uint32_t>(p, uint32_t(v)); }
};
struct U64Elem {
    using Value = uint64_t;
    static constexpr uint32_t Size = 8;
    static constexpr int32_t Limit = 30;
    static Value get(uint32_t p) { return load<uint32_t>(p) | uint64_t(load<uint32_t>(p + 4)) << 32; }
    static void put(uint32_t p, Value v) {
        save<uint32_t>(p, uint32_t(v));
        save<uint32_t>(p + 4, uint32_t(v >> 32));
    }
};

// shared_ptr<T>: moved and swapped as its two words
struct SharedElem {
    using Value = uint64_t;
    static constexpr uint32_t Size = 8;
    static constexpr int32_t Limit = 6;
    static Value get(uint32_t p) { return load<uint32_t>(p) | uint64_t(load<uint32_t>(p + 4)) << 32; }
    static void put(uint32_t p, Value v) {
        save<uint32_t>(p, uint32_t(v));
        save<uint32_t>(p + 4, uint32_t(v >> 32));
    }
};

// comparators: std::less inlined (a comparator reference is passed to the helpers), or a lambda called out of line
template <class E> struct InlineLess {
    static constexpr bool PassesComparator = true;
    static bool less(uint32_t a, uint32_t b) { return E::get(a) < E::get(b); }
};
template <uint32_t Lambda> struct CallLess {
    static constexpr bool PassesComparator = false;
    static bool less(uint32_t a, uint32_t b) { return rw::call(Lambda, {a, b}) != 0; }
};

// the helpers' addresses (Ins3 0: __insertion_sort_3 is inlined in __sort)
template <uint32_t Sort, uint32_t Sort3, uint32_t Sort4, uint32_t Sort5, uint32_t Ins3, uint32_t InsIncomplete> struct SortAddrs {
    static constexpr uint32_t sort = Sort, sort3 = Sort3, sort4 = Sort4, sort5 = Sort5, ins3 = Ins3, insIncomplete = InsIncomplete;
};

// one instance: the element kind, the comparator and the helpers
template <class E, class C, class A> struct Sorter {
    static constexpr uint32_t S = E::Size;
    static bool less(uint32_t a, uint32_t b) { return C::less(a, b); }
    static void swapAt(uint32_t a, uint32_t b) {
        const typename E::Value t = E::get(a);
        E::put(a, E::get(b));
        E::put(b, t);
    }
    // a call to a helper: the comparator reference appended when the instance passes one
    static uint32_t call(uint32_t fn, std::initializer_list<uint32_t> args, uint32_t comp) {
        uint32_t v[7];
        uint32_t n = 0;
        for (uint32_t a : args) v[n++] = a;
        if (C::PassesComparator) v[n++] = comp;
        switch (n) {
        case 2: return rw::call(fn, {v[0], v[1]});
        case 3: return rw::call(fn, {v[0], v[1], v[2]});
        case 4: return rw::call(fn, {v[0], v[1], v[2], v[3]});
        case 5: return rw::call(fn, {v[0], v[1], v[2], v[3], v[4]});
        default: return rw::call(fn, {v[0], v[1], v[2], v[3], v[4], v[5]});
        }
    }
    static uint32_t sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t c) { return call(A::sort3, {x, y, z}, c); }
    static uint32_t sort4(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t comp) { return call(A::sort4, {a, b, c, d}, comp); }
    static uint32_t sort5(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t comp) {
        return call(A::sort5, {a, b, c, d, e}, comp);
    }

    // __sort3(x, y, z) -> the number of swaps
    static uint32_t doSort3(uint32_t x, uint32_t y, uint32_t z, uint32_t) {
        const bool yx = less(y, x), zy = less(z, y);
        if (!yx) {
            if (!zy) return 0;
            swapAt(y, z);
            if (less(y, x)) {
                swapAt(x, y);
                return 2;
            }
            return 1;
        }
        if (zy) {
            swapAt(x, z);
            return 1;
        }
        swapAt(x, y);
        if (less(z, y)) {
            swapAt(y, z);
            return 2;
        }
        return 1;
    }

    static uint32_t doSort4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t comp) {
        uint32_t r = sort3(x1, x2, x3, comp);
        if (less(x4, x3)) {
            swapAt(x3, x4);
            ++r;
            if (less(x3, x2)) {
                swapAt(x2, x3);
                ++r;
                if (less(x2, x1)) {
                    swapAt(x1, x2);
                    ++r;
                }
            }
        }
        return r;
    }

    static uint32_t doSort5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t comp) {
        uint32_t r = sort4(x1, x2, x3, x4, comp);
        if (less(x5, x4)) {
            swapAt(x4, x5);
            ++r;
            if (less(x4, x3)) {
                swapAt(x3, x4);
                ++r;
                if (less(x3, x2)) {
                    swapAt(x2, x3);
                    ++r;
                    if (less(x2, x1)) {
                        swapAt(x1, x2);
                        ++r;
                    }
                }
            }
        }
        return r;
    }
    // the lambda instances' helpers take no comparator
    static uint32_t doSort3NoComp(uint32_t x, uint32_t y, uint32_t z) { return doSort3(x, y, z, 0); }
    static uint32_t doSort4NoComp(uint32_t a, uint32_t b, uint32_t c, uint32_t d) { return doSort4(a, b, c, d, 0); }
    static uint32_t doSort5NoComp(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) { return doSort5(a, b, c, d, e, 0); }

    // the insertion of *i into the sorted run before it (j is the run's last element); the moved-out element is
    // kept on the stack, where a comparator lambda can take its address
    static void insert(uint32_t first, uint32_t i, uint32_t j) {
        guest::Frame frame(8);
        const uint32_t t = frame.addr();
        E::put(t, E::get(i));
        uint32_t k = j;
        j = i;
        do {
            E::put(j, E::get(k));
            j = k;
        } while (j != first && less(t, k -= S));
        E::put(j, E::get(t));
    }

    static void insertionSort3(uint32_t first, uint32_t last, uint32_t comp) {
        uint32_t j = first + 2 * S;
        sort3(first, first + S, j, comp);
        for (uint32_t i = j + S; i != last; i += S) {
            if (less(i, j)) insert(first, i, j);
            j = i;
        }
    }

    // -> whether [first, last) ended sorted (gives up after 8 insertions)
    static uint32_t insertionSortIncomplete(uint32_t first, uint32_t last, uint32_t comp) {
        switch ((last - first) / S) {
        case 0:
        case 1: return 1;
        case 2:
            if (less(last - S, first)) swapAt(first, last - S);
            return 1;
        case 3: sort3(first, first + S, last - S, comp); return 1;
        case 4: sort4(first, first + S, first + 2 * S, last - S, comp); return 1;
        case 5: sort5(first, first + S, first + 2 * S, first + 3 * S, last - S, comp); return 1;
        }
        uint32_t j = first + 2 * S;
        sort3(first, first + S, j, comp);
        uint32_t count = 0;
        for (uint32_t i = j + S; i != last; i += S) {
            if (less(i, j)) {
                insert(first, i, j);
                if (++count == 8) return i + S == last;
            }
            j = i;
        }
        return 1;
    }
    static uint32_t insertionSortIncompleteNoComp(uint32_t first, uint32_t last) { return insertionSortIncomplete(first, last, 0); }

    static void sort(uint32_t first, uint32_t last, uint32_t comp) {
        for (;;) {
        restart:
            const int32_t len = int32_t(last - first) / int32_t(S);
            switch (len) {
            case 0:
            case 1: return;
            case 2:
                if (less(last - S, first)) swapAt(first, last - S);
                return;
            case 3: sort3(first, first + S, last - S, comp); return;
            case 4: sort4(first, first + S, first + 2 * S, last - S, comp); return;
            case 5: sort5(first, first + S, first + 2 * S, first + 3 * S, last - S, comp); return;
            }
            if (len <= E::Limit) {
                if (A::ins3) call(A::ins3, {first, last}, comp);
                else insertionSort3(first, last, comp);
                return;
            }
            uint32_t m = first;
            const uint32_t lm1 = last - S;
            uint32_t swaps;
            int32_t delta = len / 2;
            m += uint32_t(delta) * S;
            if (len >= 1000) {
                delta /= 2;
                swaps = sort5(first, first + uint32_t(delta) * S, m, m + uint32_t(delta) * S, lm1, comp);
            } else {
                swaps = sort3(first, m, lm1, comp);
            }
            uint32_t i = first, j = lm1;
            if (!less(i, m)) {
                for (;;) {
                    if (i == (j -= S)) {
                        i += S;
                        j = last;
                        if (!less(first, j -= S)) {
                            for (;;) {
                                if (i == j) return;
                                if (less(first, i)) {
                                    swapAt(i, j);
                                    ++swaps;
                                    i += S;
                                    break;
                                }
                                i += S;
                            }
                        }
                        if (i == j) return;
                        for (;;) {
                            while (!less(first, i)) i += S;
                            while (less(first, j -= S)) {}
                            if (i >= j) break;
                            swapAt(i, j);
                            ++swaps;
                            i += S;
                        }
                        first = i;
                        goto restart;
                    }
                    if (less(j, m)) {
                        swapAt(i, j);
                        ++swaps;
                        break;
                    }
                }
            }
            i += S;
            if (i < j) {
                for (;;) {
                    while (less(i, m)) i += S;
                    while (!less(j -= S, m)) {}
                    if (i > j) break;
                    swapAt(i, j);
                    ++swaps;
                    if (m == i) m = j;
                    i += S;
                }
            }
            if (i != m && less(m, i)) {
                swapAt(i, m);
                ++swaps;
            }
            if (!swaps) {
                const bool fs = call(A::insIncomplete, {first, i}, comp) != 0;
                if (call(A::insIncomplete, {i + S, last}, comp) == 1) {
                    if (fs) return;
                    last = i;
                    continue;
                }
                if (fs) {
                    first = i + S;
                    continue;
                }
            }
            if (int32_t(i - first) < int32_t(last - i)) {
                call(A::sort, {first, i}, comp);
                first = i + S;
            } else {
                call(A::sort, {i + S, last}, comp);
                last = i;
            }
        }
    }
    static void sortNoComp(uint32_t first, uint32_t last) { sort(first, last, 0); }
};

// EvolveInit's material medals, [](shared_ptr<UserMedalData> a, shared_ptr<UserMedalData> b) inlined: a before b
// when b has a skill (+0x48) and a has none, else a burst (+0x50), else a bonus (+0x28 + 0x2C + 0x30), else by
// +8. The shared_ptrs are taken by value: copies (an owner more) for the comparison, released after, b first.
struct MaterialLess {
    static constexpr bool PassesComparator = false;
    static bool flagOrder(bool fa, bool fb, bool &decided) {
        decided = fa != fb;
        return fb && !fa;
    }
    static bool less(uint32_t a, uint32_t b) {
        const uint32_t ao = load<uint32_t>(a), ac = load<uint32_t>(a + 4);
        if (ac) guest::addRef(ac);
        const uint32_t bo = load<uint32_t>(b), bc = load<uint32_t>(b + 4);
        if (bc) guest::addRef(bc);
        auto bonus = [](uint32_t o) { return load<uint32_t>(o + 0x28) + load<uint32_t>(o + 0x2C) + load<uint32_t>(o + 0x30) != 0; };
        bool decided;
        bool r = flagOrder(load<uint32_t>(ao + 0x48) != 0, load<uint32_t>(bo + 0x48) != 0, decided);
        if (!decided) r = flagOrder(load<uint32_t>(ao + 0x50) != 0, load<uint32_t>(bo + 0x50) != 0, decided);
        if (!decided) r = flagOrder(bonus(ao), bonus(bo), decided);
        if (!decided) r = int32_t(load<uint32_t>(ao + 8)) < int32_t(load<uint32_t>(bo + 8));
        if (bc) guest::release(bc);
        if (ac) guest::release(ac);
        return r;
    }
};

using IntSort = Sorter<IntElem, InlineLess<IntElem>, SortAddrs<0x4EC65D, 0x4EC8D5, 0x4EC91F, 0x4EC96F, 0x4EC9DD, 0x4ECA49>>;
using U64Sort = Sorter<U64Elem, InlineLess<U64Elem>, SortAddrs<0x4ECC81, 0x4ED119, 0x4ED243, 0x4ED31B, 0x4ED43F, 0x4ED511>>;
using TimezoneSort = Sorter<IntElem, InlineLess<IntElem>, SortAddrs<0x54D231, 0x54D4A9, 0x54D4F3, 0x54D543, 0x54D5B1, 0x54D61D>>;
using MedalStampSort = Sorter<IntElem, CallLess<0x62FDA1>, SortAddrs<0x62FA31, 0x62FE59, 0x62FEF5, 0x62FF6D, 0, 0x630005>>;
using TalkStampSort = Sorter<IntElem, CallLess<0x630785>, SortAddrs<0x630415, 0x63083D, 0x6308D9, 0x630951, 0, 0x6309E9>>;
using AvatarPartSort = Sorter<IntElem, CallLess<0x70D085>, SortAddrs<0x70CD15, 0x70D189, 0x70D225, 0x70D29D, 0, 0x70D335>>;
using MaterialSort = Sorter<SharedElem, MaterialLess, SortAddrs<0x6B3909, 0x6B497D, 0x6B4F39, 0x6B52D1, 0, 0x6B5791>>;

// ------------------------------------------------------------------------------------------ comparator lambdas
// Each creates the master records of both ids and reads a 4-byte field from each through SafeBuffer::read.
constexpr uint32_t Stamp_create = 0x7919E5, AvatarParts_create = 0x778DE1, SafeBuffer_read = 0x45A269;
int32_t readField(uint32_t record, uint32_t offset) {   // SafeBuffer::read(dst, data + offset, 4, offset)
    guest::Frame out(8);
    rw::call(SafeBuffer_read, {record, out.addr(), load<uint32_t>(record + 0xC) + offset, 4, offset});
    return int32_t(load<uint32_t>(out.addr()));
}
// [](const int& a, const int& b) { return Stamp(a).field10 < Stamp(b).field10; } (each record released at once)
uint32_t stampLess(uint32_t a, uint32_t b) {
    guest::Frame frame(8);
    const uint32_t sp = frame.addr();
    rw::call(Stamp_create, {sp, load<uint32_t>(a)});
    const int32_t fa = readField(load<uint32_t>(sp), 0x10);
    libcxx::destroyShared(sp);
    rw::call(Stamp_create, {sp, load<uint32_t>(b)});
    const int32_t fb = readField(load<uint32_t>(sp), 0x10);
    libcxx::destroyShared(sp);
    return fa < fb;
}
// [](const int& a, const int& b): by the AvatarParts field 0x98, then field 0 (both records released at the end)
uint32_t avatarPartLess(uint32_t a, uint32_t b) {
    guest::Frame frame(16);
    const uint32_t pa = frame.addr(8), pb = frame.addr(0);
    rw::call(AvatarParts_create, {pa, load<uint32_t>(a)});
    const int32_t a98 = readField(load<uint32_t>(pa), 0x98);
    rw::call(AvatarParts_create, {pb, load<uint32_t>(b)});
    const int32_t b98 = readField(load<uint32_t>(pb), 0x98);
    bool r;
    if (a98 != b98) {
        r = a98 < b98;
    } else {
        const int32_t a0 = readField(load<uint32_t>(pa), 0);
        r = a0 < readField(load<uint32_t>(pb), 0);
    }
    libcxx::destroyShared(pb);
    libcxx::destroyShared(pa);
    return r;
}

}  // namespace

RW_FUNCTION(0x4EC65C, "std::__sort<__less<int>&, int*>", IntSort::sort);
RW_FUNCTION(0x4EC8D4, "std::__sort3<__less<int>&, int*>", IntSort::doSort3);
RW_FUNCTION(0x4EC91E, "std::__sort4<__less<int>&, int*>", IntSort::doSort4);
RW_FUNCTION(0x4EC96E, "std::__sort5<__less<int>&, int*>", IntSort::doSort5);
RW_FUNCTION(0x4EC9DC, "std::__insertion_sort_3<__less<int>&, int*>", IntSort::insertionSort3);
RW_FUNCTION(0x4ECA48, "std::__insertion_sort_incomplete<__less<int>&, int*>", IntSort::insertionSortIncomplete);
RW_FUNCTION(0x4ECC80, "std::__sort<__less<unsigned long long>&, unsigned long long*>", U64Sort::sort);
RW_FUNCTION(0x4ED118, "std::__sort3<__less<unsigned long long>&, unsigned long long*>", U64Sort::doSort3);
RW_FUNCTION(0x4ED242, "std::__sort4<__less<unsigned long long>&, unsigned long long*>", U64Sort::doSort4);
RW_FUNCTION(0x4ED31A, "std::__sort5<__less<unsigned long long>&, unsigned long long*>", U64Sort::doSort5);
RW_FUNCTION(0x4ED43E, "std::__insertion_sort_3<__less<unsigned long long>&, unsigned long long*>", U64Sort::insertionSort3);
RW_FUNCTION(0x4ED510, "std::__insertion_sort_incomplete<__less<unsigned long long>&, unsigned long long*>", U64Sort::insertionSortIncomplete);
RW_FUNCTION(0x54D230, "std::__sort<__less<APIManager::Timezone>&, Timezone*>", TimezoneSort::sort);
RW_FUNCTION(0x54D4A8, "std::__sort3<__less<APIManager::Timezone>&, Timezone*>", TimezoneSort::doSort3);
RW_FUNCTION(0x54D4F2, "std::__sort4<__less<APIManager::Timezone>&, Timezone*>", TimezoneSort::doSort4);
RW_FUNCTION(0x54D542, "std::__sort5<__less<APIManager::Timezone>&, Timezone*>", TimezoneSort::doSort5);
RW_FUNCTION(0x54D5B0, "std::__insertion_sort_3<__less<APIManager::Timezone>&, Timezone*>", TimezoneSort::insertionSort3);
RW_FUNCTION(0x54D61C, "std::__insertion_sort_incomplete<__less<APIManager::Timezone>&, Timezone*>", TimezoneSort::insertionSortIncomplete);
RW_FUNCTION(0x62FA30, "std::__sort<medal stamp lambda&, int*>", MedalStampSort::sortNoComp);
RW_FUNCTION(0x62FDA0, "medal stamp sort lambda (Stamp field 0x10)", stampLess);
RW_FUNCTION(0x62FE58, "std::__sort3<medal stamp lambda&, int*>", MedalStampSort::doSort3NoComp);
RW_FUNCTION(0x62FEF4, "std::__sort4<medal stamp lambda&, int*>", MedalStampSort::doSort4NoComp);
RW_FUNCTION(0x62FF6C, "std::__sort5<medal stamp lambda&, int*>", MedalStampSort::doSort5NoComp);
RW_FUNCTION(0x630004, "std::__insertion_sort_incomplete<medal stamp lambda&, int*>", MedalStampSort::insertionSortIncompleteNoComp);
RW_FUNCTION(0x630414, "std::__sort<talk stamp lambda&, int*>", TalkStampSort::sortNoComp);
RW_FUNCTION(0x630784, "talk stamp sort lambda (Stamp field 0x10)", stampLess);
RW_FUNCTION(0x63083C, "std::__sort3<talk stamp lambda&, int*>", TalkStampSort::doSort3NoComp);
RW_FUNCTION(0x6308D8, "std::__sort4<talk stamp lambda&, int*>", TalkStampSort::doSort4NoComp);
RW_FUNCTION(0x630950, "std::__sort5<talk stamp lambda&, int*>", TalkStampSort::doSort5NoComp);
RW_FUNCTION(0x6309E8, "std::__insertion_sort_incomplete<talk stamp lambda&, int*>", TalkStampSort::insertionSortIncompleteNoComp);
RW_FUNCTION(0x70CD14, "std::__sort<avatar parts lambda&, int*>", AvatarPartSort::sortNoComp);
RW_FUNCTION(0x70D084, "avatar parts sort lambda (AvatarParts fields 0x98, 0)", avatarPartLess);
RW_FUNCTION(0x70D188, "std::__sort3<avatar parts lambda&, int*>", AvatarPartSort::doSort3NoComp);
RW_FUNCTION(0x70D224, "std::__sort4<avatar parts lambda&, int*>", AvatarPartSort::doSort4NoComp);
RW_FUNCTION(0x70D29C, "std::__sort5<avatar parts lambda&, int*>", AvatarPartSort::doSort5NoComp);
RW_FUNCTION(0x70D334, "std::__insertion_sort_incomplete<avatar parts lambda&, int*>", AvatarPartSort::insertionSortIncompleteNoComp);
RW_FUNCTION(0x6B3908, "std::__sort<EvolveInit material lambda&, shared_ptr<UserMedalData>*>", MaterialSort::sortNoComp);
RW_FUNCTION(0x6B497C, "std::__sort3<EvolveInit material lambda&, shared_ptr<UserMedalData>*>", MaterialSort::doSort3NoComp);
RW_FUNCTION(0x6B4F38, "std::__sort4<EvolveInit material lambda&, shared_ptr<UserMedalData>*>", MaterialSort::doSort4NoComp);
RW_FUNCTION(0x6B52D0, "std::__sort5<EvolveInit material lambda&, shared_ptr<UserMedalData>*>", MaterialSort::doSort5NoComp);
RW_FUNCTION(0x6B5790, "std::__insertion_sort_incomplete<EvolveInit material lambda&, shared_ptr<UserMedalData>*>", MaterialSort::insertionSortIncompleteNoComp);
