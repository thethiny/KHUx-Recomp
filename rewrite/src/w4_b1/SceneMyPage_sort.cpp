// SceneMyPage: the ranking line-up re-sorted (changeRankingSort) and the std::function builders it uses.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t ActorVector_clear = 0x485BA3, ActorVector_pushBack = 0x6425D5;
constexpr uint32_t U64Vector_pushBackSlow = 0x4ECB6D;     // std::vector<unsigned long long>::__push_back_slow_path
constexpr uint32_t UIntVector_pushBackSlow = 0x4ED6F1;    // std::vector<unsigned>::__push_back_slow_path(unsigned&&)
constexpr uint32_t Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t FadeOut_create = 0x86B25D, FadeIn_create = 0x86B115;
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t SortIds = 0x64CA31;                    // std::sort internals over the ids with the mode comparator
constexpr uint32_t BuildFadeOutDone = 0x6461E5, BuildMoveDone = 0x646291;   // sub_6461E4 / sub_646290 below
constexpr uint32_t M_getAvatarLineupStartX = 0x6460F5, M_createPartyAvatar = 0x642395;
constexpr uint32_t M_setAvatrPosition = 0x6410D5, M_addChangeMotionEvent = 0x63FA8D, M_moveAvatar = 0x6411CD;
constexpr uint32_t M_setAvatarProfileImageVisible = 0x641EF5, M_resetAvatarAction = 0x643605;
constexpr uint32_t VT_sort_29 = 0xF61478, VT_sort_30 = 0xF614A4, VT_sort_31 = 0xF6144C;
constexpr uint32_t S_BUTTON_UPDATE = 0xDF9DFB, S_RIGHT_UI = 0xDF85F7, S_TXT_REQ = 0xE03808, S_AVATAR_ID = 0xE03794;
constexpr int32_t TXT_SORT_BASE = 101000017;              // the four sort captions
constexpr uint32_t MOTION_WALK = 0x1C;

uint32_t count8(uint32_t vec) { return uint32_t(int32_t(ld32(vec + 4) - ld32(vec)) >> 3); }

// vector<unsigned long long>::push_back(*src) as inlined (a null end pointer becomes 8)
void pushU64(uint32_t vec, uint32_t src) {
    uint32_t e = ld32(vec + 4);
    if (e == ld32(vec + 8)) {
        call(U64Vector_pushBackSlow, {vec, src});
        return;
    }
    uint32_t n = 0;
    if (e) {
        st32(e, ld32(src));
        st32(e + 4, ld32(src + 4));
        n = ld32(vec + 4);
    }
    st32(vec + 4, n + 8);
}

// the actor named "avatar_<id>" under the field layer (std::string built from a StringBuffer)
uint32_t avatarName(Frame &f, uint32_t lo, uint32_t hi) {
    uint32_t buf = stringBuffer(f, {S_AVATAR_ID, lo, hi});
    return newStr(f, buf);
}

// two std::strings equal (size, then bytes)
bool strEqual(uint32_t a, uint32_t b) {
    uint32_t n = strSize(a);
    if (n != strSize(b)) return false;
    uint32_t pa = strData(a), pb = strData(b);
    for (uint32_t k = 0; k < n; k++)
        if (ld8(pa + k) != ld8(pb + k)) return false;
    return true;
}

// the shared_ptr<SafeBuffer> at self+0x2C0 = APIManager's (sb at +sbOff, control at +sbOff+4); returns the buffer
uint32_t takeRanking(uint32_t self, uint32_t sbOff) {
    uint32_t api = call(APIManager_getInstance, {});
    uint32_t ctl = ld32(api + sbOff + 4);
    uint32_t sb = ld32(api + sbOff);
    addRef(ctl);
    st32(self + 0x2C0, sb);
    uint32_t old = ld32(self + 0x2C4);
    st32(self + 0x2C4, ctl);
    if (old) {
        release(old);
        sb = ld32(self + 0x2C0);
    }
    return sb;
}

// every 0x128-byte ranking record's id (its first 8 bytes) appended to the line-up ids
void readRecordIds(uint32_t self, uint32_t sb) {
    Frame f;
    uint32_t rec = f.alloc(0x128);
    uint32_t off = 0;
    uint32_t i = 0;
    do {
        st8(rec + 0x11F, ld8(rec + 0x11F) & 0xFD);
        call(SafeBuffer_read, {sb, rec, ld32(sb + 0xC) + off, 0x128, off});
        pushU64(self + OFF_IDS2, rec);
        off += 0x128;
        i++;
        sb = ld32(self + 0x2C0);
    } while (i < ld32(sb + 0x20));
}
}  // namespace

// Builds the std::function of changeRankingSort's $_29 (heap-stored: 14 captured words) into `out`
uint32_t SceneMyPage_buildFadeOutDone(uint32_t out, uint32_t, uint32_t self, uint32_t actor, uint32_t a0, uint32_t a1,
                                      uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7,
                                      uint32_t a8, uint32_t a9, uint32_t a10, uint32_t a11, uint32_t a12,
                                      uint32_t a13) {
    st32(out + 16, 0);
    uint32_t p = call(fn::operator_new, {0x48});
    if (p) {
        st32(p, VT_sort_29);
        st32(p + 0xC, actor);
        st32(p + 8, self);
        st32(p + 0x14, a1);
        st32(p + 0x10, a0);
        st32(p + 0x1C, a3);
        st32(p + 0x18, a2);
        st32(p + 0x20, a4);
        st32(p + 0x24, a5);
        st32(p + 0x28, a6);
        st32(p + 0x2C, a7);
        st32(p + 0x30, a8);
        st32(p + 0x34, a9);
        st32(p + 0x38, a10);
        st32(p + 0x3C, a11);
        st32(p + 0x40, a12);
        st32(p + 0x44, a13);
    }
    st32(out + 16, p);
    return out;
}

// Builds the std::function of changeRankingSort's $_30 (heap-stored: 8 captured words) into `out`
uint32_t SceneMyPage_buildMoveDone(uint32_t out, uint32_t, uint32_t self, uint32_t actor, uint32_t a0, uint32_t a1,
                                   uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5) {
    st32(out + 16, 0);
    uint32_t p = call(fn::operator_new, {0x28});
    if (p) {
        st32(p, VT_sort_30);
        st32(p + 8, self);
        st32(p + 0xC, actor);
        st32(p + 0x10, a0);
        st32(p + 0x14, a1);
        st32(p + 0x18, a2);
        st32(p + 0x1C, a3);
        st32(p + 0x20, a4);
        st32(p + 0x24, a5);
    }
    st32(out + 16, p);
    return out;
}

// Re-sort the ranking line-up by `mode` (0-3): reload the ids (ranking records, or the own id alone), sort them,
// then line the first columns up - members already on the field walk over, missing ones are created on a random
// point or recycle the actor of a member that dropped out; the actors left over fade out. $_31 runs after 0.6 s.
uint32_t SceneMyPage_changeRankingSort(uint32_t self, uint32_t mode) {
    Frame frame;
    const uint32_t modeByte = frame.alloc(4);
    st8(modeByte, uint8_t(mode));
    const uint32_t button = findNode(ld32(self + 0x290), S_BUTTON_UPDATE, 0xD);
    st8(self + 0x3A9, 0);
    vcall(button, 0x29C, {button, 0});
    st8(self + 0x2F1, 0);
    st32(self + 0x388, 0);
    st32(self + 0x2F4, 0);
    st8(self + 0x3A8, ld8(modeByte));
    const uint32_t rightUi = findNode(ld32(self + 0x290), S_RIGHT_UI, 7);
    const uint32_t caption = findNode(rightUi, S_TXT_REQ, 7);
    call(SceneUtil_setShadow, {caption, 1});
    if (ld8(modeByte) <= 3) setTextId(caption, TXT_SORT_BASE + int32_t(ld8(modeByte)));

    const uint32_t ids2 = self + OFF_IDS2;
    {
        uint32_t e = ld32(ids2 + 4), b = ld32(ids2);
        if (e != b) st32(ids2 + 4, e + (~7u & ~((e - 8) - b)));
    }
    const uint32_t kind = ld32(self + 0x28C);
    if (kind == 1) {
        uint32_t sb = takeRanking(self, 0x32C);
        if (sb && ld32(sb + 0x20)) readRecordIds(self, sb);
    } else if (kind == 0) {
        uint32_t sb = takeRanking(self, 0x1FC);
        if (!sb) pushU64(ids2, self + 0x3A0);
        else if (ld32(sb + 0x20)) readRecordIds(self, sb);
    }
    {
        uint32_t comp = frame.alloc(8);
        st32(comp, self);
        st32(comp + 4, modeByte);
        call(SortIds, {ld32(ids2), ld32(ids2 + 4), comp});
    }
    const uint32_t lineup = self + OFF_ACTORS_B;
    call(ActorVector_clear, {lineup});

    // the field slots whose member is not among the first columns of the new order
    const uint32_t freeSlots = frame.alloc(16);   // counter, begin, end, cap
    const uint32_t freeVec = freeSlots + 4;
    st32(freeSlots, 0);
    st32(freeVec, 0);
    st32(freeVec + 4, 0);
    st32(freeVec + 8, 0);
    const uint32_t ids = self + OFF_IDS;
    if (ld32(ids + 4) != ld32(ids)) {
        uint32_t i = 0;
        do {
            uint32_t at = ld32(ids) + i * 8;
            uint32_t lo = ld32(at), hi = ld32(at + 4);
            bool kept = false;
            uint32_t b2 = ld32(ids2), e2 = ld32(ids2 + 4);
            if ((lo | hi) && e2 != b2) {
                int32_t cols = int32_t(ld32(self + OFF_COLUMNS));
                uint32_t j = 0;
                uint32_t p = b2;
                while (cols > int32_t(j)) {
                    uint32_t vlo = ld32(p), vhi = ld32(p + 4);
                    j++;
                    if (vlo == lo && vhi == hi) {
                        kept = true;
                        break;
                    }
                    p += 8;
                    if (!(j < uint32_t(int32_t(e2 - b2) >> 3))) break;
                }
            }
            if (!kept) {
                uint32_t e = ld32(freeVec + 4);
                if (e == ld32(freeVec + 8)) {
                    call(wl::IntVector_pushBackSlow, {freeVec, freeSlots});
                } else {
                    uint32_t n = 0;
                    if (e) {
                        st32(e, i);
                        n = ld32(freeVec + 4);
                    }
                    st32(freeVec + 4, n + 4);
                }
            }
            i++;
            st32(freeSlots, i);
        } while (i < count8(ids));
    }

    st32(self + 0x380, 0x3F800000u);
    const uint32_t startX = call(M_getAvatarLineupStartX, {self});
    const uint32_t layerAt = self + 0x2A4;
    if (ld32(ids2 + 4) != ld32(ids2)) {
        uint32_t i = 0;
        while (int32_t(ld32(self + OFF_COLUMNS)) > int32_t(i)) {
            Frame f;
            uint32_t at = ld32(ids2) + i * 8;
            const uint32_t lo = ld32(at), hi = ld32(at + 4);
            const uint32_t idSlot = f.alloc(8);
            st32(idSlot, lo);
            st32(idSlot + 4, hi);
            const uint32_t name = avatarName(f, lo, hi);
            uint32_t layer = ld32(layerAt);
            const uint32_t child = vcall(layer, 0x104, {layer, name});
            // an actor of the field slots with that name, else the layer's child
            uint32_t actor = child;
            {
                const uint32_t a = self + OFF_ACTORS_A;
                if (int32_t(ld32(a + 4) - ld32(a)) > 0) {
                    int32_t k = 0;
                    do {
                        uint32_t cand = ld32(ld32(a) + uint32_t(k) * 4);
                        Frame g;
                        uint32_t got = g.alloc(12);
                        vcall(cand, 0x150, {got, cand});
                        bool eq = strEqual(name, got);
                        strFree(got);
                        k++;
                        if (eq) {
                            actor = cand;
                            break;
                        }
                        actor = child;
                    } while (k < int32_t(ld32(a + 4) - ld32(a)) >> 2);
                }
            }
            bool reused = false;
            bool skip = false;
            if (!actor) {
                const uint32_t count = count8(ids);
                const uint32_t fb0 = ld32(freeVec), fe0 = ld32(freeVec + 4);
                if (count < ld32(self + OFF_COLUMNS) && fe0 == fb0) {
                    // a new actor on a random field point
                    actor = call(M_createPartyAvatar, {self, count, lo, hi});
                    uint32_t lay = ld32(layerAt);
                    {
                        Frame g;
                        uint32_t name2 = avatarName(g, lo, hi);
                        vcall(lay, 0xFC, {lay, actor, 0, name2});
                        strFree(name2);
                    }
                    call(ActorVector_pushBack, {self + OFF_ACTORS_A, actor});
                    {
                        uint32_t e = ld32(ids + 4);
                        if (e == ld32(self + 0x2D0)) {
                            call(U64Vector_pushBackSlow, {ids, idSlot});
                        } else {
                            uint32_t n = 0;
                            if (e) {
                                st32(e, lo);
                                st32(e + 4, hi);
                                n = ld32(ids + 4);
                            }
                            st32(ids + 4, n + 8);
                        }
                    }
                    uint32_t rnd = call(Random_getInstance, {});
                    uint32_t r = call(Random_generateUniformInt,
                                      {rnd, 0, count8(self + OFF_POINTS) - 1, 0});
                    uint32_t rSlot = f.alloc(4);
                    st32(rSlot, r);
                    {
                        uint32_t route = self + OFF_ROUTE;
                        uint32_t e = ld32(route + 4);
                        if (e >= ld32(route + 8)) {
                            call(UIntVector_pushBackSlow, {route, rSlot});
                        } else {
                            uint32_t n = 0;
                            if (e) {
                                st32(e, r);
                                n = ld32(route + 4);
                            }
                            st32(route + 4, n + 4);
                        }
                    }
                    if (!(count8(self + OFF_POINTS) > r)) {
                        call(wl::VECTOR_AT_THROW, {self + OFF_POINTS});
                        return 0;
                    }
                    uint32_t pos = f.alloc(8);
                    call(Vec2_copy, {pos, ld32(self + OFF_POINTS) + r * 8});
                    call(M_setAvatrPosition, {self, actor, pos});
                    call(wl::Vec2_dtor, {pos});
                    if (!actor) skip = true;
                } else if (fe0 == fb0) {
                    skip = true;
                } else {
                    // recycle the actor of the first free slot
                    uint32_t slot = ld32(fb0);
                    actor = ld32(ld32(self + OFF_ACTORS_A) + slot * 4);
                    uint32_t n = fe0 - (fb0 + 4);
                    call(wl::MEMMOVE, {fb0, fb0 + 4, n});
                    uint32_t newEnd = fb0 + (n & ~3u);
                    uint32_t e = ld32(freeVec + 4);
                    if (e != newEnd) st32(freeVec + 4, e + (~3u & ~((e - 4) - newEnd)));
                    uint32_t at2 = ld32(ids) + slot * 8;
                    st32(at2, lo);
                    st32(at2 + 4, hi);
                    {
                        Frame g;
                        uint32_t at3 = ld32(ids) + slot * 8;
                        uint32_t name3 = avatarName(g, ld32(at3), ld32(at3 + 4));
                        vcall(actor, 0x154, {actor, name3});
                        strFree(name3);
                    }
                    vcall(actor, node::setVisible, {actor, 1});
                    reused = true;
                }
            }
            if (!skip) {
                call(ActorVector_pushBack, {lineup, actor});
                call(M_addChangeMotionEvent, {self, actor, MOTION_WALK});
                const int32_t si = int32_t(i);
                const float rowMul = float(si % 2) * -10.0f;
                if (reused) {
                    call(M_setAvatarProfileImageVisible, {self, actor, 0});
                    const bool own = ld32(self + 0x3A0) == lo && ld32(self + 0x3A4) == hi;
                    uint32_t avatar = ld32(actor + 0x288);
                    if (!own || ld32(self + 0x2C0)) {
                        uint32_t fade = call(FadeOut_create, {fb(0.3f)});
                        Frame g;
                        uint32_t fo = g.alloc(20);
                        call(BuildFadeOutDone, {fo, startX, self, actor, lo, hi, startX, i, fb(rowMul), 0, self, actor,
                                                0x3F800000u, 1, lo, hi, i, button});
                        uint32_t cf = call(wl::CallFunc_create, {fo});
                        call(wl::Node_runAction, {avatar, call(wl::Sequence_create, {fade, cf, 0})});
                        functionDestroy(fo);
                    } else {
                        uint32_t fade = call(FadeOut_create, {0x3D99999Au});
                        uint32_t delay = call(wl::DelayTime_create, {0x3E666667u});
                        Frame g;
                        uint32_t fo = g.alloc(20);
                        call(BuildFadeOutDone, {fo, startX, self, actor, lo, hi, startX, i, fb(rowMul), 0, self, actor,
                                                0x3F800000u, 1, lo, hi, i, button});
                        uint32_t cf = call(wl::CallFunc_create, {fo});
                        call(wl::Node_runAction, {avatar, call(wl::Sequence_create, {fade, delay, cf, 0})});
                        functionDestroy(fo);
                    }
                } else {
                    call(M_setAvatarProfileImageVisible, {self, actor, 1});
                    uint32_t avatar = ld32(actor + 0x288);
                    call(wl::Node_runAction, {avatar, call(FadeIn_create, {fb(0.6f)})});
                    if (ld32(self + 0x2C0)) {
                        Frame g;
                        uint32_t pos = g.alloc(8);
                        float x = bf(startX) + float(si) * 60.0f;
                        call(wl::Vec2_ctorXY, {pos, fb(x), fb(rowMul + 70.0f)});
                        uint32_t fo = g.alloc(20);
                        call(BuildMoveDone, {fo, button, self, actor, 0x3F800000u, 0, lo, hi, i, button});
                        call(M_moveAvatar, {self, actor, pos, fo, fb(0.6f)});
                        functionDestroy(fo);
                        call(wl::Vec2_dtor, {pos});
                    } else {
                        uint32_t node = ld32(actor + 0x288);
                        uint32_t delay = call(wl::DelayTime_create, {fb(0.3f)});
                        Frame g;
                        uint32_t fo = g.alloc(20);
                        call(BuildMoveDone, {fo, button, self, actor, 0x3F800000u, 0, lo, hi, i, button});
                        uint32_t cf = call(wl::CallFunc_create, {fo});
                        call(wl::Node_runAction, {node, call(wl::Sequence_create, {delay, cf, 0})});
                        functionDestroy(fo);
                    }
                }
            }
            strFree(name);
            i++;
            if (i >= count8(ids2)) break;
        }
    }

    // the members that dropped out fade away
    {
        uint32_t p = ld32(freeVec), end = ld32(freeVec + 4);
        while (p != end) {
            Frame f;
            uint32_t layer = ld32(layerAt);
            uint32_t at = ld32(ids) + ld32(p) * 8;
            uint32_t name = avatarName(f, ld32(at), ld32(at + 4));
            uint32_t actor = vcall(layer, 0x104, {layer, name});
            uint32_t flag = ld8(name);
            strFree(name);
            if (actor) {
                call(M_resetAvatarAction, {flag, actor});
                call(M_setAvatarProfileImageVisible, {self, actor, 0});
                uint32_t avatar = ld32(actor + 0x288);
                call(wl::Node_runAction, {avatar, call(FadeOut_create, {fb(0.6f)})});
            }
            p += 4;
        }
    }
    {
        uint32_t delay = call(wl::DelayTime_create, {fb(0.6f)});
        Frame f;
        uint32_t fo = newFunction(f, VT_sort_31, {self});
        uint32_t cf = call(wl::CallFunc_create, {fo});
        call(wl::Node_runAction, {self, call(wl::Sequence_create, {delay, cf, 0})});
        functionDestroy(fo);
    }
    call(IntVector_baseDtor, {freeVec});
    return 0;
}

RW_FUNCTION(0x645140, "SceneMyPage::changeRankingSort", SceneMyPage_changeRankingSort);
RW_FUNCTION(0x6461E4, "sub_6461E4 (changeRankingSort $_29 std::function)", SceneMyPage_buildFadeOutDone);
RW_FUNCTION(0x646290, "sub_646290 (changeRankingSort $_30 std::function)", SceneMyPage_buildMoveDone);
