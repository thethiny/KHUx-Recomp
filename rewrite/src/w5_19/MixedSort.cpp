// Level Up > Optimize (SceneMixedMedal::settingOmakaseMedals): its std::sort over the candidate medals
// (shared_ptr<UserMedalData>, 8 bytes) - the comparator sub_6A02C4 (the preferred keyblade type first, then medals
// that can be fused, then rarity, then the medal's +0x4C), libc++'s __sort3 / __sort4 / __sort5 and __sort. The
// sort moves elements as shared_ptrs do: a move leaves the source empty and a move assignment releases the old
// pointer; swaps exchange both words.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Less = 0x6A02C5;                 // the comparator
constexpr uint32_t Sort3 = 0x6A0425, Sort4 = 0x6A04FB, Sort5 = 0x6A059B, Sort = 0x69FE01;
constexpr uint32_t InsertionIncomplete = 0x6A0665;  // __insertion_sort_incomplete
}  // namespace g

constexpr uint32_t E = 8;

struct SortSP {
    uint32_t comp;
    bool less(uint32_t a, uint32_t b) const { return call(g::Less, {comp, a, b}) == 1; }
    static void swap(uint32_t a, uint32_t b) {
        const uint32_t a0 = ld32(a);
        st32(a, ld32(b));
        st32(b, a0);
        const uint32_t a1 = ld32(a + 4);
        st32(a + 4, ld32(b + 4));
        st32(b + 4, a1);
    }
};
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
}  // namespace

struct MixedSort {
    static uint32_t less(uint32_t comp, uint32_t a, uint32_t b);
    static uint32_t sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t comp);
    static uint32_t sort4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t comp);
    static uint32_t sort5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t comp);
    static void sort(uint32_t first, uint32_t last, uint32_t comp);
};

// {preferred keyblade type}: a's keyblade type (+0x458), fusable (+0x454), rarity (+0x470) against b's
uint32_t MixedSort::less(uint32_t comp, uint32_t a, uint32_t b) {
    Frame f;
    const uint32_t v = f.alloc(4);
    auto read = [&](uint32_t ref, uint32_t off) {
        const uint32_t sb = ld32(ld32(ref) + 0x58);
        rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, 4, off});
        return int32_t(ld32(v));
    };
    const int32_t typeA = read(a, 0x458), typeB = read(b, 0x458);
    const int32_t mixA = read(a, 0x454), mixB = read(b, 0x454);
    const int32_t rareA = read(a, 0x470), rareB = read(b, 0x470);
    if (typeA != typeB) {
        const int32_t preferred = int32_t(ld32(comp));
        return (preferred == typeA || preferred != typeB) ? 1 : 0;
    }
    if (mixA != mixB) return mixA > mixB ? 1 : 0;
    if (rareA == rareB) return int32_t(ld32(ld32(a) + 0x4C)) > int32_t(ld32(ld32(b) + 0x4C)) ? 1 : 0;
    return rareA > rareB ? 1 : 0;
}

uint32_t MixedSort::sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t comp) {
    const SortSP o{comp};
    const bool yx = o.less(y, x);
    const bool zy = o.less(z, y);
    if (!yx) {
        if (!zy) return 0;
        SortSP::swap(y, z);
        if (!o.less(y, x)) return 1;
        SortSP::swap(x, y);
        return 2;
    }
    if (zy) {
        SortSP::swap(x, z);
        return 1;
    }
    SortSP::swap(x, y);
    if (!o.less(z, y)) return 1;
    SortSP::swap(y, z);
    return 2;
}

uint32_t MixedSort::sort4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t comp) {
    const SortSP o{comp};
    uint32_t r = call(g::Sort3, {x1, x2, x3, comp});
    if (o.less(x4, x3)) {
        SortSP::swap(x3, x4);
        ++r;
        if (o.less(x3, x2)) {
            SortSP::swap(x2, x3);
            ++r;
            if (o.less(x2, x1)) {
                SortSP::swap(x1, x2);
                ++r;
            }
        }
    }
    return r;
}

uint32_t MixedSort::sort5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t comp) {
    const SortSP o{comp};
    uint32_t r = rw::call(g::Sort4, {x1, x2, x3, x4, comp});
    if (o.less(x5, x4)) {
        SortSP::swap(x4, x5);
        ++r;
        if (o.less(x4, x3)) {
            SortSP::swap(x3, x4);
            ++r;
            if (o.less(x3, x2)) {
                SortSP::swap(x2, x3);
                ++r;
                if (o.less(x2, x1)) {
                    SortSP::swap(x1, x2);
                    ++r;
                }
            }
        }
    }
    return r;
}

// std::__sort (libc++ for a type that is not trivially copyable: insertion sort up to 6 elements, median of 3 / 5
// pivots, the equal-pivot partition, the insertion-sort shortcut after a swap-free partition, recursion into the
// smaller side)
void MixedSort::sort(uint32_t first, uint32_t last, uint32_t comp) {
    const SortSP o{comp};
    for (;;) {
    restart:
        const uint32_t len = uint32_t(int32_t(last - first) >> 3);
        const uint32_t lm1 = last - E;
        switch (len) {
        case 0:
        case 1:
            return;
        case 2:
            if (o.less(lm1, first)) SortSP::swap(first, lm1);
            return;
        case 3:
            call(g::Sort3, {first, first + E, lm1, comp});
            return;
        case 4:
            rw::call(g::Sort4, {first, first + E, first + 2 * E, lm1, comp});
            return;
        case 5:
            rw::call(g::Sort5, {first, first + E, first + 2 * E, first + 3 * E, lm1, comp});
            return;
        }
        if (len <= 6) {
            call(g::Sort3, {first, first + E, first + 2 * E, comp});
            Frame f;
            const uint32_t t = f.alloc(8);
            for (uint32_t j = first + 2 * E, i = j + E; i != last; j = i, i += E) {
                if (!o.less(i, j)) continue;
                st32(t, ld32(i));
                st32(t + 4, ld32(i + 4));
                st32(i, 0);
                st32(i + 4, 0);
                uint32_t k = j, hole = i;
                for (;;) {
                    // *hole = std::move(*k)
                    const uint32_t obj = ld32(k), ctl = ld32(k + 4);
                    st32(k, 0);
                    st32(k + 4, 0);
                    st32(hole, obj);
                    const uint32_t old = ld32(hole + 4);
                    st32(hole + 4, ctl);
                    release(old);
                    hole = k;
                    if (hole == first) break;
                    if (!o.less(t, k - E)) break;
                    k -= E;
                }
                // *hole = std::move(t)
                const uint32_t obj = ld32(t), ctl = ld32(t + 4);
                st32(t, 0);
                st32(t + 4, 0);
                st32(hole, obj);
                const uint32_t old = ld32(hole + 4);
                st32(hole + 4, ctl);
                release(old);
                release(ld32(t + 4));
            }
            return;
        }
        uint32_t m = first + E * uint32_t(int32_t(len) / 2);
        uint32_t swaps;
        if (int32_t(last - first) >= 0x1F39) {
            const uint32_t delta = uint32_t(int32_t(len) / 4);
            swaps = rw::call(g::Sort5, {first, first + E * delta, m, m + E * delta, lm1, comp});
        } else {
            swaps = call(g::Sort3, {first, m, lm1, comp});
        }
        uint32_t i = first, j = lm1;
        if (!o.less(i, m)) {
            bool found = false;
            while (j - E != i) {
                j -= E;
                if (o.less(j, m)) {
                    SortSP::swap(i, j);
                    swaps++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                i = first + E;
                j = lm1;
                if (!o.less(first, j)) {
                    for (;;) {
                        if (i == j) return;
                        if (o.less(first, i)) {
                            SortSP::swap(i, j);
                            swaps++;
                            i += E;
                            break;
                        }
                        i += E;
                    }
                }
                if (i == j) return;
                for (;;) {
                    while (!o.less(first, i)) i += E;
                    do j -= E;
                    while (o.less(first, j));
                    if (i >= j) break;
                    SortSP::swap(i, j);
                    swaps++;
                    i += E;
                }
                first = i;
                goto restart;
            }
        }
        i += E;
        if (i < j) {
            for (;;) {
                while (o.less(i, m)) i += E;
                do j -= E;
                while (!o.less(j, m));
                if (i > j) break;
                SortSP::swap(i, j);
                swaps++;
                if (m == i) m = j;
                i += E;
            }
        }
        if (i != m && o.less(m, i)) {
            SortSP::swap(i, m);
            swaps++;
        }
        if (swaps == 0) {
            const bool fs = call(g::InsertionIncomplete, {first, i, comp}) != 0;
            if (call(g::InsertionIncomplete, {i + E, last, comp}) == 1) {
                if (fs) return;
                last = i;
                continue;
            }
            if (fs) {
                first = i + E;
                continue;
            }
        }
        if (int32_t(i - first) < int32_t(last - i)) {
            call(g::Sort, {first, i, comp});
            first = i + E;
        } else {
            call(g::Sort, {i + E, last, comp});
            last = i;
        }
    }
}

RW_FUNCTION(0x6A02C4, "SceneMixedMedal::settingOmakaseMedals comparator (sub_6A02C4)", MixedSort::less);
RW_FUNCTION(0x6A0424, "std::__sort3<SceneMixedMedal medal compare&, shared_ptr<UserMedalData>*> (sub_6A0424)", MixedSort::sort3);
RW_FUNCTION(0x6A04FA, "std::__sort4<SceneMixedMedal medal compare&, shared_ptr<UserMedalData>*> (sub_6A04FA)", MixedSort::sort4);
RW_FUNCTION(0x6A059A, "std::__sort5<SceneMixedMedal medal compare&, shared_ptr<UserMedalData>*> (sub_6A059A)", MixedSort::sort5);
RW_FUNCTION(0x69FE00, "std::__sort<SceneMixedMedal medal compare&, shared_ptr<UserMedalData>*> (sub_69FE00)", MixedSort::sort);
