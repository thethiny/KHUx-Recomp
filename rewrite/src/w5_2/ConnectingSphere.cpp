// ConnectingSphere: the Avatar Board's open-state model (a cocos2d::Ref): the board id, the masu ids already
// opened on it (23 slots), the spheres picked to open (an unordered_set<int>), the player's points of the board's
// currency, and the request that opens them (/user/sphere/update).
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
namespace a {
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;               // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_ctor = 0x8FD025, Ref_dtor = 0x8FD059, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t IntSet_dtor = 0x5C0A99;                // __hash_table<int>::~__hash_table
constexpr uint32_t IntSet_clear = 0x7998FD;               // __hash_table<int>::clear
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_executeUserSphereUpdate = 0x4CCD69;   // (board, const set&, int, const function<void(bool)>&)
constexpr uint32_t SphereMap_find = 0x503479;             // map<int, UserSphere::Sphere const>::find(const int&)
constexpr uint32_t IntVector_copy = 0x4AF707, IntVector_dtor = 0x478961;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Sphere_create = 0x78CF21;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t FunctionVB_assign = 0x490109;          // function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVB_call = 0x474139;            // function<void(bool)>::operator()(bool)
}  // namespace a

constexpr uint32_t VTABLE = 0xFAC78C;
constexpr uint32_t SIZE = 0x94;
// fields
constexpr uint32_t F_PICKED = 0x14;        // unordered_set<int> (+0x14..+0x24: buckets, count, first, size, 1.0f)
constexpr uint32_t F_OPENED = 0x28;        // int[23]: masu ids opened on this board
constexpr uint32_t F_REQUEST = 0x84;       // set while picked spheres wait for the request
constexpr uint32_t F_POINTS = 0x88;        // the player's points of the board's currency
constexpr uint32_t F_BOARD = 0x8C;
constexpr uint32_t F_INCOMPLETE = 0x90;    // the board was not complete when loaded (status != 3)

void sbRead(uint32_t sb, uint32_t dst, uint32_t offset, uint32_t len) {
    rw::call(a::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, len, offset});
}
}  // namespace

struct ConnectingSphere {
    static uint32_t create();
    static void destructDelete(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t getUserData(uint32_t self, uint32_t board);
    static uint32_t getSphereStatus(uint32_t self, uint32_t board);
    static uint32_t executeSphereOpenlist(uint32_t self, uint32_t done);
};

uint32_t ConnectingSphere::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(a::Ref_ctor, {me});
    st32(me, VTABLE);
    for (uint32_t k = 0; k < 0x10; k += 4) st32(me + F_PICKED + k, 0);
    st32(me + F_PICKED + 0x10, fbits(1.0f));           // max_load_factor
    call(a::Ref_autorelease, {me});
    return me;
}

uint32_t ConnectingSphere::destruct(uint32_t self) {
    st32(self, VTABLE);
    call(a::IntSet_dtor, {self + F_PICKED});
    call(a::Ref_dtor, {self});
    return self;
}

void ConnectingSphere::destructDelete(uint32_t self) {
    destruct(self);
    call(fn::operator_delete, {self});
}

// the player's state on board `board`: the masu ids opened (from the user sphere map; the rest of the 23 slots
// cleared), whether the board was incomplete, and the points of its currency (the board's kind: 3, 4, 5 or other)
uint32_t ConnectingSphere::getUserData(uint32_t self, uint32_t board) {
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, board);
    st32(self + F_BOARD, board);
    const uint32_t api = call(a::APIManager_getInstance, {});
    const uint32_t mapCtrl = ld32(api + 0xB8), map = ld32(api + 0xB4);
    if (mapCtrl) call(fn::shared_add, {mapCtrl});
    if (map) {
        const uint32_t node = call(a::SphereMap_find, {map, key});
        uint32_t filled = 0;
        bool clear = true;
        if (node != map + 4) {
            const uint32_t ids = f.alloc(12);
            call(a::IntVector_copy, {ids, node + 0x28});
            const uint32_t begin = ld32(ids), bytes = ld32(ids + 4) - begin;
            const int32_t n = int32_t(bytes) >> 2;
            if (int32_t(bytes) >= 1) {
                int32_t k = 0;
                do {
                    st32(self + F_OPENED + 4 * uint32_t(k), ld32(begin + 4 * uint32_t(k)));
                } while (++k < n);
                filled = 1;
                if (int32_t(bytes) > 4) {
                    call(a::IntVector_dtor, {ids});
                    filled = uint32_t(n);
                    if (int32_t(bytes) > 0x5B) clear = false;
                } else {
                    call(a::IntVector_dtor, {ids});
                }
            } else {
                call(a::IntVector_dtor, {ids});
            }
        }
        if (clear) call(a::AEABI_MEMSET, {self + F_OPENED + 4 * filled, 0x5C - 4 * filled, 0});
    }
    const uint32_t r = call(a::IntSet_clear, {self + F_PICKED});
    const uint32_t status = getSphereStatus(r, ld32(self + F_BOARD));
    st8(self + F_INCOMPLETE, status == 3 ? 0 : 1);
    const uint32_t api2 = call(a::APIManager_getInstance, {});
    const uint32_t itemsCtrl = ld32(api2 + 0x90), items = ld32(api2 + 0x8C);
    if (itemsCtrl) call(fn::shared_add, {itemsCtrl});
    const uint32_t sphere = f.alloc(8), kind = f.alloc(4), v = f.alloc(4);
    call(a::Sphere_create, {sphere, ld32(self + F_BOARD)});
    sbRead(ld32(sphere), kind, 0x98, 4);
    if (items) {
        switch (ld32(kind)) {
        case 3: sbRead(items, v, 0x50, 4); break;
        case 4: sbRead(items, v, 0x48, 4); break;
        case 5: sbRead(items, v, 0x4C, 4); break;
        default: sbRead(items, v, 0x18, 4); break;
        }
        st32(self + F_POINTS, ld32(v));
    } else {
        st32(self + F_POINTS, 0);
    }
    st32(self + F_REQUEST, 0);
    sharedRelease(sphere);
    if (itemsCtrl) call(fn::shared_release, {itemsCtrl});
    if (mapCtrl) call(fn::shared_release, {mapCtrl});
    return 1;
}

// the board's status in the user sphere map (3 = complete)
uint32_t ConnectingSphere::getSphereStatus(uint32_t self, uint32_t board) {
    (void)self;
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, board);
    const uint32_t api = call(a::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + 0xB8), map = ld32(api + 0xB4);
    if (ctrl) call(fn::shared_add, {ctrl});
    const uint32_t status = ld32(call(a::SphereMap_find, {map, key}) + 0x20);
    if (ctrl) call(fn::shared_release, {ctrl});
    return status;
}

// sends the picked spheres (/user/sphere/update) with `done` as the callback; nothing picked: done(true) now
uint32_t ConnectingSphere::executeSphereOpenlist(uint32_t self, uint32_t done) {
    if (!ld32(self + F_REQUEST)) {
        call(a::FunctionVB_call, {done, 1});
        return 1;
    }
    const uint32_t sm = call(a::SceneManager_getInstance, {});
    Frame f;
    const uint32_t empty = newEmptyFunction(f);
    st8(sm + 0x4CC, 1);
    call(a::FunctionVB_assign, {sm + 0x548, empty});
    functionDestroy(empty);
    const uint32_t api = call(a::APIManager_getInstance, {});
    rw::call(a::APIManager_executeUserSphereUpdate, {api, ld32(self + F_BOARD), self + F_PICKED, ld32(self + F_REQUEST), done});
    call(a::IntSet_clear, {self + F_PICKED});
    return 1;
}

RW_FUNCTION(0x793A64, "ConnectingSphere::create", ConnectingSphere::create);
RW_FUNCTION(0x798B5C, "ConnectingSphere::~ConnectingSphere (deleting)", ConnectingSphere::destructDelete);
RW_FUNCTION(0x798B28, "ConnectingSphere::~ConnectingSphere", ConnectingSphere::destruct);
RW_FUNCTION(0x798B98, "ConnectingSphere::get_user_data", ConnectingSphere::getUserData);
RW_FUNCTION(0x798DC0, "ConnectingSphere::getSphereStatus", ConnectingSphere::getSphereStatus);
RW_FUNCTION(0x798EC8, "ConnectingSphere::executeSphereOpenlist", ConnectingSphere::executeSphereOpenlist);
