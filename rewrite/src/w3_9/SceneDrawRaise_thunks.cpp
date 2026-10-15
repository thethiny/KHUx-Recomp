// SceneDrawRaise::init's lambdas: the std::function __func slots (destructors, clones, destroy, target,
// target_type). The lambda bodies (operator()) are in SceneDrawRaise_lambdas.cpp.
//
//   $_0 / $_1        [SceneDrawRaise*]  onEnter / onExit hooks: schedule / unschedule update()
//   $_3 / $_5        [layer, shadow, view, SceneDrawRaise*, vector<u64> ids, AudioLWFNode*] (0x24 bytes): the demo
//                    movie finished (raid gacha / gacha)
//   $_3/$_5.lambda   [layer, shadow, view, SceneDrawRaise*, vector<u64> ids] (0x20 bytes): after one frame, remove
//                    the demo and open the results popup
//   $_4 / $_6        [index, vector<u64> ids, AudioLWFNode*, rarity] (0x1C bytes): the "load" movie event of
//                    drowMedalNNN: attach the medal movie
#include "../scene_main/FunctionThunks.hpp"

namespace SceneDrawRaise_thunks {

constexpr uint32_t VectorU64_copy = 0x468431;     // std::vector<unsigned long long>::vector(const vector&)
constexpr uint32_t VectorU64_dtor = 0x468499;     // std::__vector_base<unsigned long long>::~__vector_base

inline uint32_t ld(uint32_t a) { return rw::load<uint32_t>(a); }
inline void st(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }

// The slots shared by the three hand-written shapes: the captured vector<u64> sits at VEC.
template <uint32_t VTABLE, uint32_t TYPEINFO, uint32_t TYPENAME, uint32_t VEC> struct VectorFunc {
    // ~__func(): resets the vtable, destroys the vector
    static uint32_t destructor(uint32_t self) {
        st(self, VTABLE);
        rw::call(VectorU64_dtor, {self + VEC});
        return self;
    }
    static void destructorDelete(uint32_t self) {
        st(self, VTABLE);
        rw::call(VectorU64_dtor, {self + VEC});
        guest::deleteObject(self);
    }
    static void destroy(uint32_t self) { rw::call(VectorU64_dtor, {self + VEC}); }
    static void destroyDeallocate(uint32_t self) {
        rw::call(VectorU64_dtor, {self + VEC});
        guest::deleteObject(self);
    }
    static uint32_t target(uint32_t self, uint32_t type) { return ld(type + 4) == TYPENAME ? self + 4 : 0; }
    static uint32_t targetType(uint32_t) { return TYPEINFO; }
};

// [layer, shadow, view, scene] + vector<u64> at +0x14 (+ node at +0x20 when SIZE is 0x24). The copies store the
// scene word again after copying the vector, as the compiler emitted them.
template <uint32_t VTABLE, uint32_t SIZE, uint32_t TYPEINFO, uint32_t TYPENAME>
struct DemoFunc : VectorFunc<VTABLE, TYPEINFO, TYPENAME, 0x14> {
    static void copyInto(uint32_t dst, uint32_t src) {
        st(dst, VTABLE);
        uint32_t a = ld(src + 4), b = ld(src + 8), c = ld(src + 0xC);
        st(dst + 4, a);
        st(dst + 8, b);
        st(dst + 0xC, c);
        st(dst + 0x10, ld(src + 0x10));
        rw::call(VectorU64_copy, {dst + 0x14, src + 0x14});
        st(dst + 0x10, ld(src + 0x10));
        if (SIZE == 0x24) st(dst + 0x20, ld(src + 0x20));
    }
    static uint32_t clone(uint32_t self) {
        uint32_t obj = guest::newObject(SIZE);
        if (obj) copyInto(obj, self);
        return obj;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (dst) copyInto(dst, self);
    }
};

// [index] + vector<u64> at +8 + [node, rarity]
template <uint32_t VTABLE, uint32_t TYPEINFO, uint32_t TYPENAME>
struct MedalHandlerFunc : VectorFunc<VTABLE, TYPEINFO, TYPENAME, 8> {
    static void copyInto(uint32_t dst, uint32_t src) {
        st(dst, VTABLE);
        st(dst + 4, ld(src + 4));
        rw::call(VectorU64_copy, {dst + 8, src + 8});
        uint32_t node = ld(src + 0x14), rarity = ld(src + 0x18);
        st(dst + 0x14, node);
        st(dst + 0x18, rarity);
    }
    static uint32_t clone(uint32_t self) {
        uint32_t obj = guest::newObject(0x1C);
        if (obj) copyInto(obj, self);
        return obj;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (dst) copyInto(dst, self);
    }
};

// SceneDrawRaise::init::$_0 / $_1 [SceneDrawRaise*]
using init_0 = thunk::Lambda<0xF654C8, 8, 0xF9D470, 0xE74350, 4, thunk::cap(4, 4)>;
using init_1 = thunk::Lambda<0xF6549C, 8, 0xF9D45C, 0xE742D0, 4, thunk::cap(4, 4)>;
using init_3 = DemoFunc<0xF65444, 0x24, 0xF9D434, 0xE741C0>;
using init_3_inner = DemoFunc<0xF65470, 0x20, 0xF9D448, 0xE74240>;
using init_5 = DemoFunc<0xF653C0, 0x24, 0xF9D3F8, 0xE74030>;
using init_5_inner = DemoFunc<0xF653EC, 0x20, 0xF9D40C, 0xE740B0>;
using init_4 = MedalHandlerFunc<0xF65418, 0xF9D420, 0xE74140>;
using init_6 = MedalHandlerFunc<0xF65394, 0xF9D3E4, 0xE73FB0>;

}  // namespace SceneDrawRaise_thunks

using namespace SceneDrawRaise_thunks;

// $_0
RW_FUNCTION(0x69597C, "SceneDrawRaise::init::$_0 ~__func() (deleting)", init_0::deleteThis);
RW_FUNCTION(0x695984, "SceneDrawRaise::init::$_0 __clone()", init_0::clone);
RW_FUNCTION(0x6959B0, "SceneDrawRaise::init::$_0 __clone(__base*)", init_0::cloneInto);
RW_FUNCTION(0x6959D0, "SceneDrawRaise::init::$_0 destroy()", init_0::destroy);
RW_FUNCTION(0x6959D2, "SceneDrawRaise::init::$_0 destroy_deallocate()", init_0::deleteThis);
RW_FUNCTION(0x6959E4, "SceneDrawRaise::init::$_0 target(const type_info&)", init_0::target);
RW_FUNCTION(0x695A04, "SceneDrawRaise::init::$_0 target_type()", init_0::targetType);
// $_1
RW_FUNCTION(0x6958E0, "SceneDrawRaise::init::$_1 ~__func() (deleting)", init_1::deleteThis);
RW_FUNCTION(0x6958E8, "SceneDrawRaise::init::$_1 __clone()", init_1::clone);
RW_FUNCTION(0x695914, "SceneDrawRaise::init::$_1 __clone(__base*)", init_1::cloneInto);
RW_FUNCTION(0x695934, "SceneDrawRaise::init::$_1 destroy()", init_1::destroy);
RW_FUNCTION(0x695936, "SceneDrawRaise::init::$_1 destroy_deallocate()", init_1::deleteThis);
RW_FUNCTION(0x695948, "SceneDrawRaise::init::$_1 target(const type_info&)", init_1::target);
RW_FUNCTION(0x695968, "SceneDrawRaise::init::$_1 target_type()", init_1::targetType);
// $_3
RW_FUNCTION(0x695480, "SceneDrawRaise::init::$_3 ~__func()", init_3::destructor);
RW_FUNCTION(0x6954AC, "SceneDrawRaise::init::$_3 ~__func() (deleting)", init_3::destructorDelete);
RW_FUNCTION(0x6954DC, "SceneDrawRaise::init::$_3 __clone()", init_3::clone);
RW_FUNCTION(0x695540, "SceneDrawRaise::init::$_3 __clone(__base*)", init_3::cloneInto);
RW_FUNCTION(0x69558C, "SceneDrawRaise::init::$_3 destroy()", init_3::destroy);
RW_FUNCTION(0x695596, "SceneDrawRaise::init::$_3 destroy_deallocate()", init_3::destroyDeallocate);
RW_FUNCTION(0x69571C, "SceneDrawRaise::init::$_3 target(const type_info&)", init_3::target);
RW_FUNCTION(0x69573C, "SceneDrawRaise::init::$_3 target_type()", init_3::targetType);
// $_3::operator()::lambda
RW_FUNCTION(0x695750, "SceneDrawRaise::init::$_3::lambda ~__func()", init_3_inner::destructor);
RW_FUNCTION(0x69577C, "SceneDrawRaise::init::$_3::lambda ~__func() (deleting)", init_3_inner::destructorDelete);
RW_FUNCTION(0x6957AC, "SceneDrawRaise::init::$_3::lambda __clone()", init_3_inner::clone);
RW_FUNCTION(0x69580C, "SceneDrawRaise::init::$_3::lambda __clone(__base*)", init_3_inner::cloneInto);
RW_FUNCTION(0x695854, "SceneDrawRaise::init::$_3::lambda destroy()", init_3_inner::destroy);
RW_FUNCTION(0x69585E, "SceneDrawRaise::init::$_3::lambda destroy_deallocate()", init_3_inner::destroyDeallocate);
RW_FUNCTION(0x6958AC, "SceneDrawRaise::init::$_3::lambda target(const type_info&)", init_3_inner::target);
RW_FUNCTION(0x6958CC, "SceneDrawRaise::init::$_3::lambda target_type()", init_3_inner::targetType);
// $_5
RW_FUNCTION(0x694C14, "SceneDrawRaise::init::$_5 ~__func()", init_5::destructor);
RW_FUNCTION(0x694C40, "SceneDrawRaise::init::$_5 ~__func() (deleting)", init_5::destructorDelete);
RW_FUNCTION(0x694C70, "SceneDrawRaise::init::$_5 __clone()", init_5::clone);
RW_FUNCTION(0x694CD4, "SceneDrawRaise::init::$_5 __clone(__base*)", init_5::cloneInto);
RW_FUNCTION(0x694D20, "SceneDrawRaise::init::$_5 destroy()", init_5::destroy);
RW_FUNCTION(0x694D2A, "SceneDrawRaise::init::$_5 destroy_deallocate()", init_5::destroyDeallocate);
RW_FUNCTION(0x694EB0, "SceneDrawRaise::init::$_5 target(const type_info&)", init_5::target);
RW_FUNCTION(0x694ED0, "SceneDrawRaise::init::$_5 target_type()", init_5::targetType);
// $_5::operator()::lambda
RW_FUNCTION(0x694EE4, "SceneDrawRaise::init::$_5::lambda ~__func()", init_5_inner::destructor);
RW_FUNCTION(0x694F10, "SceneDrawRaise::init::$_5::lambda ~__func() (deleting)", init_5_inner::destructorDelete);
RW_FUNCTION(0x694F40, "SceneDrawRaise::init::$_5::lambda __clone()", init_5_inner::clone);
RW_FUNCTION(0x694FA0, "SceneDrawRaise::init::$_5::lambda __clone(__base*)", init_5_inner::cloneInto);
RW_FUNCTION(0x694FE8, "SceneDrawRaise::init::$_5::lambda destroy()", init_5_inner::destroy);
RW_FUNCTION(0x694FF2, "SceneDrawRaise::init::$_5::lambda destroy_deallocate()", init_5_inner::destroyDeallocate);
RW_FUNCTION(0x695040, "SceneDrawRaise::init::$_5::lambda target(const type_info&)", init_5_inner::target);
RW_FUNCTION(0x695060, "SceneDrawRaise::init::$_5::lambda target_type()", init_5_inner::targetType);
// $_4
RW_FUNCTION(0x695074, "SceneDrawRaise::init::$_4 ~__func()", init_4::destructor);
RW_FUNCTION(0x6950A0, "SceneDrawRaise::init::$_4 ~__func() (deleting)", init_4::destructorDelete);
RW_FUNCTION(0x6950D0, "SceneDrawRaise::init::$_4 __clone()", init_4::clone);
RW_FUNCTION(0x69512C, "SceneDrawRaise::init::$_4 __clone(__base*)", init_4::cloneInto);
RW_FUNCTION(0x695170, "SceneDrawRaise::init::$_4 destroy()", init_4::destroy);
RW_FUNCTION(0x69517A, "SceneDrawRaise::init::$_4 destroy_deallocate()", init_4::destroyDeallocate);
RW_FUNCTION(0x69544C, "SceneDrawRaise::init::$_4 target(const type_info&)", init_4::target);
RW_FUNCTION(0x69546C, "SceneDrawRaise::init::$_4 target_type()", init_4::targetType);
// $_6
RW_FUNCTION(0x694808, "SceneDrawRaise::init::$_6 ~__func()", init_6::destructor);
RW_FUNCTION(0x694834, "SceneDrawRaise::init::$_6 ~__func() (deleting)", init_6::destructorDelete);
RW_FUNCTION(0x694864, "SceneDrawRaise::init::$_6 __clone()", init_6::clone);
RW_FUNCTION(0x6948C0, "SceneDrawRaise::init::$_6 __clone(__base*)", init_6::cloneInto);
RW_FUNCTION(0x694904, "SceneDrawRaise::init::$_6 destroy()", init_6::destroy);
RW_FUNCTION(0x69490E, "SceneDrawRaise::init::$_6 destroy_deallocate()", init_6::destroyDeallocate);
RW_FUNCTION(0x694BE0, "SceneDrawRaise::init::$_6 target(const type_info&)", init_6::target);
RW_FUNCTION(0x694C00, "SceneDrawRaise::init::$_6 target_type()", init_6::targetType);
