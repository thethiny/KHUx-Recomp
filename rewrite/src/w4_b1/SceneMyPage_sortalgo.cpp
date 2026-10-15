// SceneMyPage::changeRankingSort: libc++ std::sort over the line-up ids (uint64) with the $_28 comparator (the
// ranking records looked up by id), as instantiated in the game: __sort, __insertion_sort_incomplete, __sort3/4/5.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t COMP = 0x64CE71, SORT3 = 0x64D13D, SORT4 = 0x64D1FF, SORT5 = 0x64D297;
constexpr uint32_t INSERTION_INCOMPLETE = 0x64D359, SORT = 0x64CA31;
constexpr uint32_t RankIndex_find = 0x490275;   // std::unordered_map<unsigned long long, int>::find(const key&)
constexpr uint32_t RECORD_SIZE = 0x98;

bool less(uint32_t comp, uint32_t a, uint32_t b) { return call(COMP, {comp, a, b}) != 0; }
void swap8(uint32_t x, uint32_t y) {
    uint32_t x0 = ld32(x), x1 = ld32(x + 4), y0 = ld32(y), y1 = ld32(y + 4);
    st32(x, y0);
    st32(x + 4, y1);
    st32(y, x0);
    st32(y + 4, x1);
}
uint32_t sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t c) { return call(SORT3, {x, y, z, c}); }

// the ranking record of the id at `key` (the record list at rank+0x3C, its index from the map at rank+0x24);
// a copy of its two strings is made and freed as the original copies the whole record
struct RecordCopy {
    uint32_t rec;
    uint32_t s7C, s88;   // frame strings (long ones re-allocated)
};
uint32_t findRecord(uint32_t self, uint32_t keyPtr, Frame &f, RecordCopy &out) {
    uint32_t rank = ld32(self + 0x2C0);
    uint32_t key = f.alloc(8);
    st32(key, ld32(keyPtr));
    st32(key + 4, ld32(keyPtr + 4));
    uint32_t node = call(RankIndex_find, {rank + 0x24, key});
    uint32_t idx = node ? ld32(node + 0x10) : 0xFFFFFFFFu;
    uint32_t b = ld32(rank + 0x3C);
    uint32_t count = uint32_t(int32_t(ld32(rank + 0x40) - b) >> 3) * 0x286BCA1Bu;
    if (!(count > idx)) {
        call(wl::VECTOR_AT_THROW, {rank + 0x3C});
        return 0;
    }
    out.rec = b + idx * RECORD_SIZE;
    out.s7C = f.alloc(12);
    out.s88 = f.alloc(12);
    for (uint32_t k = 0; k < 2; k++) {
        uint32_t src = out.rec + (k ? 0x88 : 0x7C), dst = k ? out.s88 : out.s7C;
        if (strIsLong(src)) {
            strInit(dst, ld32(src + 8), ld32(src + 4));
        } else {
            st32(dst, ld32(src));
            st32(dst + 4, ld32(src + 4));
            st32(dst + 8, ld32(src + 8));
        }
    }
    return 1;
}
}  // namespace

// $_28 {self, &mode}: a before b in the line-up: mode 0 by record +0x78, 1 by +0x44 + +0x48 (both descending,
// signed), 2 by the uint64 at +0x58 (descending), otherwise by id (ascending)
uint32_t SceneMyPage_sortComp(uint32_t comp, uint32_t a, uint32_t b) {
    const uint32_t self = ld32(comp);
    Frame f;
    RecordCopy ra{}, rb{};
    if (!findRecord(self, a, f, ra)) return 0;
    if (!findRecord(self, b, f, rb)) return 0;
    uint32_t result;
    const uint32_t mode = ld8(ld32(comp + 4));
    if (mode == 0) {
        result = int32_t(ld32(ra.rec + 0x78)) > int32_t(ld32(rb.rec + 0x78));
    } else if (mode == 1) {
        int32_t va = int32_t(ld32(ra.rec + 0x44) + ld32(ra.rec + 0x48));
        int32_t vb = int32_t(ld32(rb.rec + 0x44) + ld32(rb.rec + 0x48));
        result = va > vb;
    } else if (mode == 2) {
        uint64_t va = uint64_t(ld32(ra.rec + 0x58)) | (uint64_t(ld32(ra.rec + 0x5C)) << 32);
        uint64_t vb = uint64_t(ld32(rb.rec + 0x58)) | (uint64_t(ld32(rb.rec + 0x5C)) << 32);
        result = va > vb;
    } else {
        uint64_t va = uint64_t(ld32(a)) | (uint64_t(ld32(a + 4)) << 32);
        uint64_t vb = uint64_t(ld32(b)) | (uint64_t(ld32(b + 4)) << 32);
        result = va < vb;
    }
    strFree(rb.s88);
    strFree(rb.s7C);
    strFree(ra.s88);
    strFree(ra.s7C);
    return result;
}

// std::__sort3: returns the swaps made
uint32_t SceneMyPage_sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t c) {
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

// std::__sort4
uint32_t SceneMyPage_sort4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t c) {
    uint32_t r = sort3(x1, x2, x3, c);
    if (!less(c, x4, x3)) return r;
    swap8(x3, x4);
    if (!less(c, x3, x2)) return r + 1;
    swap8(x2, x3);
    if (!less(c, x2, x1)) return r + 2;
    swap8(x1, x2);
    return r + 3;
}

// std::__sort5
uint32_t SceneMyPage_sort5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t c) {
    uint32_t r = call(SORT4, {x1, x2, x3, x4, c});
    if (!less(c, x5, x4)) return r;
    swap8(x4, x5);
    if (!less(c, x4, x3)) return r + 1;
    swap8(x3, x4);
    if (!less(c, x3, x2)) return r + 2;
    swap8(x2, x3);
    if (!less(c, x2, x1)) return r + 3;
    swap8(x1, x2);
    return r + 4;
}

namespace {
// the 0..5 element cases shared by __sort and __insertion_sort_incomplete; false when longer
bool smallSort(uint32_t first, uint32_t last, uint32_t c) {
    uint32_t n = uint32_t(int32_t(last - first) >> 3);
    switch (n) {
    case 0:
    case 1:
        return true;
    case 2:
        if (less(c, last - 8, first)) swap8(first, last - 8);
        return true;
    case 3:
        sort3(first, first + 8, last - 8, c);
        return true;
    case 4:
        call(SORT4, {first, first + 8, first + 16, last - 8, c});
        return true;
    case 5:
        call(SORT5, {first, first + 8, first + 16, first + 24, last - 8, c});
        return true;
    default:
        return false;
    }
}
// one insertion step: *i before *j -> shift it down; returns true when it moved
bool insertOne(uint32_t first, uint32_t i, uint32_t j, uint32_t c) {
    if (!less(c, i, j)) return false;
    Frame f;
    uint32_t t = f.alloc(8);
    st32(t, ld32(i));
    st32(t + 4, ld32(i + 4));
    uint32_t hole = i, k = j;
    while (true) {
        st32(hole, ld32(k));
        st32(hole + 4, ld32(k + 4));
        hole = k;
        if (hole == first) break;
        if (!less(c, t, k - 8)) break;
        k -= 8;
    }
    st32(hole, ld32(t));
    st32(hole + 4, ld32(t + 4));
    return true;
}
}  // namespace

// std::__insertion_sort_incomplete: gives up after 8 moved elements; true when [first, last) is sorted
uint32_t SceneMyPage_insertionSortIncomplete(uint32_t first, uint32_t last, uint32_t c) {
    if (uint32_t(int32_t(last - first) >> 3) <= 5) {
        smallSort(first, last, c);
        return 1;
    }
    uint32_t j = first + 16;
    sort3(first, first + 8, j, c);
    uint32_t count = 0;
    for (uint32_t i = j + 8; i != last; i += 8) {
        if (insertOne(first, i, j, c)) {
            if (++count == 8) return i + 8 == last;
        }
        j = i;
    }
    return 1;
}

// std::__sort (introsort without the depth limit, as libc++ had it)
void SceneMyPage_sortIds(uint32_t first, uint32_t last, uint32_t c) {
    while (true) {
    restart:
        const int32_t bytes = int32_t(last - first);
        const int32_t len = bytes >> 3;
        if (uint32_t(len) <= 5) {
            smallSort(first, last, c);
            return;
        }
        if (bytes <= 0xF7) {
            // __insertion_sort_3
            uint32_t j = first + 16;
            sort3(first, first + 8, j, c);
            for (uint32_t i = j + 8; i != last; i += 8) {
                insertOne(first, i, j, c);
                j = i;
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
            swaps = sort3(first, m, lm1, c);
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

RW_FUNCTION(0x64CE70, "SceneMyPage::changeRankingSort::$_28::operator()", SceneMyPage_sortComp);
RW_FUNCTION(0x64D13C, "std::__sort3<changeRankingSort::$_28&, unsigned long long*>", SceneMyPage_sort3);
RW_FUNCTION(0x64D1FE, "std::__sort4<changeRankingSort::$_28&, unsigned long long*>", SceneMyPage_sort4);
RW_FUNCTION(0x64D296, "std::__sort5<changeRankingSort::$_28&, unsigned long long*>", SceneMyPage_sort5);
RW_FUNCTION(0x64D358, "std::__insertion_sort_incomplete<changeRankingSort::$_28&, unsigned long long*>",
            SceneMyPage_insertionSortIncomplete);
RW_FUNCTION(0x64CA30, "std::__sort<changeRankingSort::$_28&, unsigned long long*>", SceneMyPage_sortIds);
