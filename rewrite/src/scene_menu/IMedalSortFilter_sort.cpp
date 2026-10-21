// IMedalSortFilter::runSortFilter's std::sort over the filtered medals (shared_ptr<UserMedalData>, 8 bytes) with
// the comparator sub_674500, as libc++ instantiated it: __sort (sub_67403C), __sort3 (sub_674A04), __sort4
// (sub_674ADA). __sort5 (sub_674B7A) and __insertion_sort_incomplete (sub_674C44) are called as they are.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t COMP = 0x674501, SORT3 = 0x674A05, SORT4 = 0x674ADB, SORT5 = 0x674B7B;
constexpr uint32_t INSERTION_INCOMPLETE = 0x674C45, SORT = 0x67403D;

bool less(uint32_t comp, uint32_t a, uint32_t b) { return call(COMP, {comp, a, b}) != 0; }
void swap8(uint32_t x, uint32_t y) {
    const uint32_t x0 = ld32(x), x1 = ld32(x + 4), y0 = ld32(y), y1 = ld32(y + 4);
    st32(x, y0);
    st32(x + 4, y1);
    st32(y, x0);
    st32(y + 4, x1);
}
uint32_t sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t c) { return call(SORT3, {x, y, z, c}); }
// *dst = std::move(*src) for shared_ptrs: src emptied, dst's old reference released
void moveShared(uint32_t dst, uint32_t src) {
    const uint32_t ptr = ld32(src), ctrl = ld32(src + 4);
    st32(src, 0);
    st32(src + 4, 0);
    st32(dst, ptr);
    const uint32_t old = ld32(dst + 4);
    st32(dst + 4, ctrl);
    if (old) call(fn::shared_release, {old});
}
}  // namespace

namespace medal_sort {

// std::__sort3: returns the swaps made
uint32_t sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t c) {
    const bool yx = less(c, y, x);
    const bool zy = less(c, z, y);
    if (!yx) {
        if (!zy) return 0;
        swap8(y, z);
        if (!less(c, y, x)) return 1;
        swap8(x, y);
        return 2;
    }
    if (zy) {
        swap8(x, z);
        return 1;
    }
    swap8(x, y);
    if (!less(c, z, y)) return 1;
    swap8(y, z);
    return 2;
}
RW_FUNCTION(0x674A04, "std::__sort3<runSortFilter comparator&, shared_ptr<UserMedalData>*> (sub_674A04)",
            medal_sort::sort3);

// std::__sort4
uint32_t sort4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t c) {
    const uint32_t r = ::sort3(x1, x2, x3, c);
    if (!less(c, x4, x3)) return r;
    swap8(x3, x4);
    if (!less(c, x3, x2)) return r + 1;
    swap8(x2, x3);
    if (!less(c, x2, x1)) return r + 2;
    swap8(x1, x2);
    return r + 3;
}
RW_FUNCTION(0x674ADA, "std::__sort4<runSortFilter comparator&, shared_ptr<UserMedalData>*> (sub_674ADA)",
            medal_sort::sort4);

// std::__sort (introsort without the depth limit, as libc++ had it)
void sort(uint32_t first, uint32_t last, uint32_t c) {
    while (true) {
    restart:
        const int32_t bytes = int32_t(last - first);
        const int32_t len = bytes >> 3;
        switch (len) {
        case 0:
        case 1:
            return;
        case 2:
            if (less(c, last - 8, first)) swap8(first, last - 8);
            return;
        case 3:
            ::sort3(first, first + 8, last - 8, c);
            return;
        case 4:
            call(SORT4, {first, first + 8, first + 16, last - 8, c});
            return;
        case 5:
            call(SORT5, {first, first + 8, first + 16, first + 24, last - 8, c});
            return;
        default:
            break;
        }
        if (bytes <= 55) {
            // __insertion_sort_3, moving the shared_ptrs
            uint32_t j = first + 16;
            ::sort3(first, first + 8, j, c);
            for (uint32_t i = j + 8; i != last; j = i, i += 8) {
                if (!less(c, i, j)) continue;
                Frame f;
                const uint32_t t = f.alloc(8);
                st32(t, ld32(i));
                st32(t + 4, ld32(i + 4));
                st32(i, 0);
                st32(i + 4, 0);
                uint32_t hole = i, k = j;
                while (true) {
                    moveShared(hole, k);
                    hole = k;
                    if (hole == first) break;
                    if (!less(c, t, k - 8)) break;
                    k -= 8;
                }
                moveShared(hole, t);
                if (ld32(t + 4)) call(fn::shared_release, {ld32(t + 4)});
            }
            return;
        }
        const int32_t half = len / 2;
        uint32_t m = first + uint32_t(half) * 8;
        const uint32_t lm1 = last - 8;
        uint32_t swaps;
        if (bytes >= 0x1F39) {
            const int32_t delta = len / 4;
            swaps = call(SORT5, {first, first + uint32_t(delta) * 8, m, first + uint32_t(delta + half) * 8, lm1, c});
        } else {
            swaps = ::sort3(first, m, lm1, c);
        }
        uint32_t i = first, j = lm1;
        if (!less(c, i, m)) {
            while (true) {
                j -= 8;
                if (i == j) {
                    // *first is the smallest: put the elements equal to it first
                    i += 8;
                    j = last;
                    j -= 8;
                    if (!less(c, first, j)) {
                        while (true) {
                            if (i == j) return;
                            if (less(c, first, i)) {
                                swap8(i, j);
                                i += 8;
                                break;
                            }
                            i += 8;
                        }
                    }
                    if (i == j) return;
                    while (true) {
                        while (!less(c, first, i)) i += 8;
                        do {
                            j -= 8;
                        } while (less(c, first, j));
                        if (!(i < j)) break;
                        swap8(i, j);
                        i += 8;
                    }
                    first = i;
                    goto restart;
                }
                if (less(c, j, m)) {
                    swap8(i, j);
                    swaps++;
                    break;
                }
            }
        }
        i += 8;
        if (i < j) {
            while (true) {
                while (less(c, i, m)) i += 8;
                do {
                    j -= 8;
                } while (!less(c, j, m));
                if (i > j) break;
                swap8(i, j);
                swaps++;
                if (m == i) m = j;
                i += 8;
            }
        }
        if (i != m && less(c, m, i)) {
            swap8(i, m);
            swaps++;
        }
        if (swaps == 0) {
            const bool fs = call(INSERTION_INCOMPLETE, {first, i, c}) != 0;
            if (call(INSERTION_INCOMPLETE, {i + 8, last, c}) == 1) {
                if (fs) return;
                last = i;
                continue;
            }
            if (fs) {
                first = i + 8;
                continue;
            }
        }
        if (int32_t(i - first) < int32_t(last - i)) {
            call(SORT, {first, i, c});
            first = i + 8;
        } else {
            call(SORT, {i + 8, last, c});
            last = i;
        }
    }
}
RW_FUNCTION(0x67403C, "std::__sort<runSortFilter comparator&, shared_ptr<UserMedalData>*> (sub_67403C)",
            medal_sort::sort);

// std::__insertion_sort_incomplete: sorts short ranges outright; longer ones by insertion, giving up (false)
// after eight moved elements unless that was the last one
uint32_t insertionSortIncomplete(uint32_t first, uint32_t last, uint32_t c) {
    switch (int32_t(last - first) >> 3) {
    case 0:
    case 1:
        return 1;
    case 2:
        if (less(c, last - 8, first)) swap8(first, last - 8);
        return 1;
    case 3:
        ::sort3(first, first + 8, last - 8, c);
        return 1;
    case 4:
        call(SORT4, {first, first + 8, first + 16, last - 8, c});
        return 1;
    case 5:
        call(SORT5, {first, first + 8, first + 16, first + 24, last - 8, c});
        return 1;
    default:
        break;
    }
    uint32_t j = first + 16;
    ::sort3(first, first + 8, j, c);
    uint32_t count = 0;
    for (uint32_t i = j + 8; i != last; j = i, i += 8) {
        if (!less(c, i, j)) continue;
        Frame f;
        const uint32_t t = f.alloc(8);
        st32(t, ld32(i));
        st32(t + 4, ld32(i + 4));
        st32(i, 0);
        st32(i + 4, 0);
        uint32_t hole = i, k = j;
        while (true) {
            moveShared(hole, k);
            hole = k;
            if (hole == first) break;
            k -= 8;
            if (!less(c, t, k)) break;
        }
        moveShared(hole, t);
        const bool done = ++count == 8;
        if (ld32(t + 4)) call(fn::shared_release, {ld32(t + 4)});
        if (done) return i + 8 == last ? 1 : 0;
    }
    return 1;
}
RW_FUNCTION(0x674C44, "std::__insertion_sort_incomplete<runSortFilter comparator&, shared_ptr<UserMedalData>*> (sub_674C44)",
            medal_sort::insertionSortIncomplete);

}  // namespace medal_sort
