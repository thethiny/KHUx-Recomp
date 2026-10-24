// SceneSphereBoardSelect::setSphereBoardList: the Avatar Board list. The board ids of every sphere (master)
// are collected; for each board the user has, its kind (sphere word +0x98: 1 / 2 normal, 3 colosseum, 4 stage,
// 5 raid) decides whether it shows in this mode; a normal board avaterBoardDispCheck accepts is only counted
// (as locked) in mode 1. The shown boards go to the scroll view with the remembered board selected; the two
// counters, the filter label (mode 1) and the arrows (more than one board) are updated.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_getAvatarBoardPos[4] = {0x4EABB3, 0x4EABD7, 0x4EABE9, 0x4EABC5};   // normal,
                                                                                               // colosseum (3),
                                                                                               // stage (4), raid (5)
constexpr uint32_t AppState_getAvatarBoardFilterState = 0x4EB189;
constexpr uint32_t tree_int_destroy = 0x48D9B5, tree_int_insert = 0x6BCBD5, tree_int_insert_hint = 0x6BCA05;
constexpr uint32_t Sphere_getSphereBoardIds = 0x78CF05, Sphere_create = 0x78CF21, SafeBuffer_read = 0x45A269;
constexpr uint32_t map_sphere_find = 0x503479, vector_int_copy = 0x4AF707, vector_int_dtor = 0x478961;
constexpr uint32_t avaterBoardDispCheck = 0x6B8551;
constexpr uint32_t vector_sphere_push_back_slow = 0x6BC885, vector_sphere_copy = 0x6B8929;
constexpr uint32_t vector_sphere_dtor = 0x50248D, SphereScrollView_setSphereBoard = 0x502F99;
constexpr uint32_t StringBuffer_ctor = 0x49140D, Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t S_FmtD = 0xDF86DE, S_LeftArrow = 0xE05629, S_RightUI = 0xDF85F7;
constexpr uint32_t V_setVisible = 0xB0;
// SceneSphereBoardSelect: +0x2A0 / +0x2A4 the two counters, +0x2A8 the filter label, +0x2AC the remembered board,
// +0x2B0 mode (1 normal, 3 colosseum, 4 stage, 5 raid), +0x2B4 the scroll view (+0x3B4 its board count),
// +0x2B8 the layout, +0x2BC vector<shared_ptr<Sphere>> (end +0x2C0, capacity +0x2C4), +0x2C8 set<int> of board
// ids (end node +0x2CC, size +0x2D0)
constexpr uint32_t F_COUNT = 0x2A0, F_TOTAL = 0x2A4, F_FILTER = 0x2A8, F_POS = 0x2AC, F_MODE = 0x2B0;
constexpr uint32_t F_VIEW = 0x2B4, F_ROOT = 0x2B8, F_LIST = 0x2BC, F_IDS = 0x2C8, VIEW_COUNT = 0x3B4;

// std::set<int> (libc++ __tree): node {left, right, parent, color, key +0x10}
uint32_t next(uint32_t node) {
    if (ld32(node + 4)) {
        node = ld32(node + 4);
        while (ld32(node)) node = ld32(node);
        return node;
    }
    uint32_t p;
    for (;;) {
        p = ld32(node + 8);
        if (ld32(p) == node) return p;
        node = p;
    }
}
void setNumber(uint32_t text, uint32_t n) {
    Frame f;
    const uint32_t buf = f.alloc(0x404);
    call(StringBuffer_ctor, {buf, S_FmtD, n});
    const uint32_t s = newStr(f, buf);
    call(Text_setString, {text, s});
    strFree(s);
}
}  // namespace

namespace sphere_board_list {

uint32_t setSphereBoardList(uint32_t self) {
    for (int k = 0; k < 4; k++) call(fn::APIManager_getInstance, {});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t mode = ld32(self + F_MODE);
    const uint32_t which = mode == 3 ? 1 : mode == 4 ? 2 : mode == 5 ? 3 : 0;
    st32(self + F_POS, call(AppState_getAvatarBoardPos[which], {call(AppState_getInstance, {})}));
    st32(ld32(self + F_VIEW) + VIEW_COUNT, 0);
    call(tree_int_destroy, {self + F_IDS, ld32(self + F_IDS + 4)});
    st32(self + F_IDS + 8, 0);
    st32(self + F_IDS, self + F_IDS + 4);
    st32(self + F_IDS + 4, 0);
    Frame f;
    const uint32_t boards = f.alloc(12);
    call(Sphere_getSphereBoardIds, {boards});
    for (uint32_t it = ld32(boards); it != boards + 4; it = next(it)) {
        Frame g;
        const uint32_t sp = g.alloc(8), v = g.alloc(4), key = g.alloc(4), ret = g.alloc(8);
        call(Sphere_create, {sp, ld32(it + 0x10)});
        call(SafeBuffer_read, {ld32(sp), v, ld32(ld32(sp) + 0xC), 4, 0});
        st32(key, ld32(v));
        call(tree_int_insert, {ret, self + F_IDS, key});
        if (ld32(sp + 4)) call(fn::shared_release, {ld32(sp + 4)});
    }
    if (ld32(api + 0xB4)) {
        // the shown list emptied
        const uint32_t begin = ld32(self + F_LIST);
        for (uint32_t end = ld32(self + F_LIST + 4); end != begin;) {
            end -= 8;
            st32(self + F_LIST + 4, end);
            if (ld32(end + 4)) call(fn::shared_release, {ld32(end + 4)});
            end = ld32(self + F_LIST + 4);
        }
        // a copy of the board ids
        const uint32_t ids = f.alloc(12);
        st32(ids + 4, 0);
        st32(ids + 8, 0);
        st32(ids, ids + 4);
        for (uint32_t it = ld32(self + F_IDS); it != self + F_IDS + 4; it = next(it))
            call(tree_int_insert_hint, {ids, ids + 4, it + 0x10});      // insert(end(), key)
        uint32_t selected = 0, locked = 0;
        for (uint32_t it = ld32(ids); it != ids + 4; it = next(it)) {
            const uint32_t map = ld32(api + 0xB4);
            const uint32_t found = call(map_sphere_find, {map, it + 0x10});
            if (found == map + 4) continue;
            Frame g;
            const uint32_t sp = g.alloc(8), kindV = g.alloc(4);
            call(Sphere_create, {sp, ld32(it + 0x10)});
            call(SafeBuffer_read, {ld32(sp), kindV, ld32(ld32(sp) + 0xC) + 0x98, 4, 0x98});
            const uint32_t kind = ld32(kindV) - 1;
            bool counted = false;
            if (kind <= 1) {
                // a normal board: avaterBoardDispCheck(shared_ptr<Sphere>, the user's sphere)
                const uint32_t sphere = g.alloc(8), user = g.alloc(0x20);
                st32(sphere, ld32(sp));
                st32(sphere + 4, ld32(sp + 4));
                if (ld32(sphere + 4)) call(fn::shared_add, {ld32(sphere + 4)});
                for (uint32_t k = 0; k < 4; k++) st32(user + 4 * k, ld32(found + 0x14 + 4 * k));
                st32(user + 0x10, ld32(found + 0x24));
                call(vector_int_copy, {user + 0x14, found + 0x28});
                const uint32_t ok = call(avaterBoardDispCheck, {self, sphere, user});
                call(vector_int_dtor, {user + 0x14});
                if (ld32(sphere + 4)) call(fn::shared_release, {ld32(sphere + 4)});
                if (ok == 1) {
                    if (ld32(self + F_MODE) == 1) locked++;
                    counted = true;
                }
            }
            if (!counted && kind <= 4) {
                static const uint32_t MODE_FOR[5] = {1, 1, 3, 4, 5};
                if (ld32(self + F_MODE) == MODE_FOR[kind]) {
                    const uint32_t end = ld32(self + F_LIST + 4);
                    if (end == ld32(self + F_LIST + 8)) {
                        call(vector_sphere_push_back_slow, {self + F_LIST, sp});
                    } else {
                        if (end) {
                            st32(end, ld32(sp));
                            st32(end + 4, ld32(sp + 4));
                            if (ld32(sp + 4)) call(fn::shared_add, {ld32(sp + 4)});
                        }
                        st32(self + F_LIST + 4, ld32(self + F_LIST + 4) + 8);
                    }
                    const uint32_t view = ld32(self + F_VIEW);
                    const uint32_t n = ld32(view + VIEW_COUNT);
                    st32(view + VIEW_COUNT, n + 1);
                    if (ld32(self + F_POS) == ld32(it + 0x10)) selected = n;
                }
            }
            if (ld32(sp + 4)) call(fn::shared_release, {ld32(sp + 4)});
        }
        const uint32_t n = ld32(ld32(self + F_VIEW) + VIEW_COUNT);
        setNumber(ld32(self + F_COUNT), n);
        setNumber(ld32(self + F_TOTAL), ld32(ld32(self + F_VIEW) + VIEW_COUNT) + locked);
        {
            Frame g;
            const uint32_t copy = g.alloc(12);
            const uint32_t view = ld32(self + F_VIEW);
            call(vector_sphere_copy, {copy, self + F_LIST});
            call(SphereScrollView_setSphereBoard, {view, copy, selected});
            call(vector_sphere_dtor, {copy});
        }
        call(tree_int_destroy, {ids, ld32(ids + 4)});
    }
    if (ld32(self + F_MODE) == 1) {
        Frame g;
        const uint32_t state = g.alloc(8);
        call(AppState_getAvatarBoardFilterState, {state, call(AppState_getInstance, {})});
        const int32_t id = ld8(ld32(state)) ? 100100012 : 100100035;
        setTextId(ld32(self + F_FILTER), id);
        if (ld32(state + 4)) call(fn::shared_release, {ld32(state + 4)});
    }
    const uint32_t left = findNode(ld32(self + F_ROOT), S_LeftArrow, 0xC);
    const uint32_t right = findNode(ld32(self + F_ROOT), S_RightUI, 7);
    if (left) {
        const uint32_t many = int32_t(ld32(ld32(self + F_VIEW) + VIEW_COUNT)) > 1 ? 1 : 0;
        vcall(left, V_setVisible, {left, many});
    }
    if (right) {
        const uint32_t many = int32_t(ld32(ld32(self + F_VIEW) + VIEW_COUNT)) > 1 ? 1 : 0;
        vcall(right, V_setVisible, {right, many});
    }
    call(tree_int_destroy, {boards, ld32(boards + 4)});
    return 0x91C;
}
RW_FUNCTION(0x6B8974, "SceneSphereBoardSelect::setSphereBoardList", sphere_board_list::setSphereBoardList);

}  // namespace sphere_board_list
