// SceneTutorialDownload: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"
#include "../scene/SceneCommon.hpp"

namespace SceneTutorialDownload_lambdas {

// SceneTutorialDownload::openJewelPopup::$_8 (captures a std::function at +32, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openJewelPopup_8 = thunk::LambdaFn<0xF6D740, 56, 0xFA7394, 0xE9BC40, 8, 32, 0x45C84D, thunk::cap(8, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// SceneTutorialDownload::openDownloadPopup::$_6 (captures a std::function at +32, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openDownloadPopup_6 = thunk::LambdaFn<0xF6D7F0, 56, 0xFA73E4, 0xE9C0C0, 8, 32, 0x45C84D, thunk::cap(8, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// SceneTutorialDownload::finalize::$_4
using finalize_4 = thunk::Lambda<0xF6D848, 8, 0xFA740C, 0xE9C260, 4, thunk::cap(4, 4)>;
// SceneTutorialDownload::init::$_3
using init_3 = thunk::Lambda<0xF6D8A0, 12, 0xFA7434, 0xE9C3A0, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneTutorialDownload::init::$_2
using init_2 = thunk::Lambda<0xF6D8F8, 16, 0xFA745C, 0xE9C520, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneTutorialDownload::init::$_1
using init_1 = thunk::Lambda<0xF6D924, 8, 0xFA7470, 0xE9C5B0, 4, thunk::cap(4, 4)>;
// SceneTutorialDownload::init::$_0
using init_0 = thunk::Lambda<0xF6D950, 8, 0xFA7484, 0xE9C640, 4, thunk::cap(4, 4)>;

}  // namespace SceneTutorialDownload_lambdas

RW_FUNCTION(0x73B4D8, "SceneTutorialDownload::openJewelPopup::$_8 ~__func()", SceneTutorialDownload_lambdas::openJewelPopup_8::destructor);
RW_FUNCTION(0x73B514, "SceneTutorialDownload::openJewelPopup::$_8 ~__func() (deleting)", SceneTutorialDownload_lambdas::openJewelPopup_8::destructorDelete);
RW_FUNCTION(0x73B554, "SceneTutorialDownload::openJewelPopup::$_8 __clone()", SceneTutorialDownload_lambdas::openJewelPopup_8::clone);
RW_FUNCTION(0x73B5B8, "SceneTutorialDownload::openJewelPopup::$_8 __clone(__base*)", SceneTutorialDownload_lambdas::openJewelPopup_8::cloneInto);
RW_FUNCTION(0x73B604, "SceneTutorialDownload::openJewelPopup::$_8 destroy()", SceneTutorialDownload_lambdas::openJewelPopup_8::destroy);
RW_FUNCTION(0x73B624, "SceneTutorialDownload::openJewelPopup::$_8 destroy_deallocate()", SceneTutorialDownload_lambdas::openJewelPopup_8::destroyDeallocate);
RW_FUNCTION(0x73B850, "SceneTutorialDownload::openJewelPopup::$_8 target(const type_info&)", SceneTutorialDownload_lambdas::openJewelPopup_8::target);
RW_FUNCTION(0x73B874, "SceneTutorialDownload::openJewelPopup::$_8 target_type()", SceneTutorialDownload_lambdas::openJewelPopup_8::targetType);
RW_FUNCTION(0x73BE74, "SceneTutorialDownload::openDownloadPopup::$_6 ~__func()", SceneTutorialDownload_lambdas::openDownloadPopup_6::destructor);
RW_FUNCTION(0x73BEB0, "SceneTutorialDownload::openDownloadPopup::$_6 ~__func() (deleting)", SceneTutorialDownload_lambdas::openDownloadPopup_6::destructorDelete);
RW_FUNCTION(0x73BEF0, "SceneTutorialDownload::openDownloadPopup::$_6 __clone()", SceneTutorialDownload_lambdas::openDownloadPopup_6::clone);
RW_FUNCTION(0x73BF54, "SceneTutorialDownload::openDownloadPopup::$_6 __clone(__base*)", SceneTutorialDownload_lambdas::openDownloadPopup_6::cloneInto);
RW_FUNCTION(0x73BFA0, "SceneTutorialDownload::openDownloadPopup::$_6 destroy()", SceneTutorialDownload_lambdas::openDownloadPopup_6::destroy);
RW_FUNCTION(0x73BFC0, "SceneTutorialDownload::openDownloadPopup::$_6 destroy_deallocate()", SceneTutorialDownload_lambdas::openDownloadPopup_6::destroyDeallocate);
RW_FUNCTION(0x73C150, "SceneTutorialDownload::openDownloadPopup::$_6 target(const type_info&)", SceneTutorialDownload_lambdas::openDownloadPopup_6::target);
RW_FUNCTION(0x73C174, "SceneTutorialDownload::openDownloadPopup::$_6 target_type()", SceneTutorialDownload_lambdas::openDownloadPopup_6::targetType);
RW_FUNCTION(0x73C360, "SceneTutorialDownload::finalize::$_4 ~__func() (deleting)", SceneTutorialDownload_lambdas::finalize_4::deleteThis);
RW_FUNCTION(0x73C368, "SceneTutorialDownload::finalize::$_4 __clone()", SceneTutorialDownload_lambdas::finalize_4::clone);
RW_FUNCTION(0x73C394, "SceneTutorialDownload::finalize::$_4 __clone(__base*)", SceneTutorialDownload_lambdas::finalize_4::cloneInto);
RW_FUNCTION(0x73C3B4, "SceneTutorialDownload::finalize::$_4 destroy()", SceneTutorialDownload_lambdas::finalize_4::destroy);
RW_FUNCTION(0x73C3B6, "SceneTutorialDownload::finalize::$_4 destroy_deallocate()", SceneTutorialDownload_lambdas::finalize_4::deleteThis);
RW_FUNCTION(0x73C450, "SceneTutorialDownload::finalize::$_4 target(const type_info&)", SceneTutorialDownload_lambdas::finalize_4::target);
RW_FUNCTION(0x73C470, "SceneTutorialDownload::finalize::$_4 target_type()", SceneTutorialDownload_lambdas::finalize_4::targetType);
RW_FUNCTION(0x73C5C0, "SceneTutorialDownload::init::$_3 ~__func() (deleting)", SceneTutorialDownload_lambdas::init_3::deleteThis);
RW_FUNCTION(0x73C5C8, "SceneTutorialDownload::init::$_3 __clone()", SceneTutorialDownload_lambdas::init_3::clone);
RW_FUNCTION(0x73C5F8, "SceneTutorialDownload::init::$_3 __clone(__base*)", SceneTutorialDownload_lambdas::init_3::cloneInto);
RW_FUNCTION(0x73C61C, "SceneTutorialDownload::init::$_3 destroy()", SceneTutorialDownload_lambdas::init_3::destroy);
RW_FUNCTION(0x73C61E, "SceneTutorialDownload::init::$_3 destroy_deallocate()", SceneTutorialDownload_lambdas::init_3::deleteThis);
RW_FUNCTION(0x73C944, "SceneTutorialDownload::init::$_3 target(const type_info&)", SceneTutorialDownload_lambdas::init_3::target);
RW_FUNCTION(0x73C964, "SceneTutorialDownload::init::$_3 target_type()", SceneTutorialDownload_lambdas::init_3::targetType);
RW_FUNCTION(0x73CA84, "SceneTutorialDownload::init::$_2 ~__func() (deleting)", SceneTutorialDownload_lambdas::init_2::deleteThis);
RW_FUNCTION(0x73CA8C, "SceneTutorialDownload::init::$_2 __clone()", SceneTutorialDownload_lambdas::init_2::clone);
RW_FUNCTION(0x73CABC, "SceneTutorialDownload::init::$_2 __clone(__base*)", SceneTutorialDownload_lambdas::init_2::cloneInto);
RW_FUNCTION(0x73CAE4, "SceneTutorialDownload::init::$_2 destroy()", SceneTutorialDownload_lambdas::init_2::destroy);
RW_FUNCTION(0x73CAE6, "SceneTutorialDownload::init::$_2 destroy_deallocate()", SceneTutorialDownload_lambdas::init_2::deleteThis);
RW_FUNCTION(0x73CB60, "SceneTutorialDownload::init::$_2 target(const type_info&)", SceneTutorialDownload_lambdas::init_2::target);
RW_FUNCTION(0x73CB80, "SceneTutorialDownload::init::$_2 target_type()", SceneTutorialDownload_lambdas::init_2::targetType);
RW_FUNCTION(0x73CB94, "SceneTutorialDownload::init::$_1 ~__func() (deleting)", SceneTutorialDownload_lambdas::init_1::deleteThis);
RW_FUNCTION(0x73CB9C, "SceneTutorialDownload::init::$_1 __clone()", SceneTutorialDownload_lambdas::init_1::clone);
RW_FUNCTION(0x73CBC8, "SceneTutorialDownload::init::$_1 __clone(__base*)", SceneTutorialDownload_lambdas::init_1::cloneInto);
RW_FUNCTION(0x73CBE8, "SceneTutorialDownload::init::$_1 destroy()", SceneTutorialDownload_lambdas::init_1::destroy);
RW_FUNCTION(0x73CBEA, "SceneTutorialDownload::init::$_1 destroy_deallocate()", SceneTutorialDownload_lambdas::init_1::deleteThis);
RW_FUNCTION(0x73CBFC, "SceneTutorialDownload::init::$_1 target(const type_info&)", SceneTutorialDownload_lambdas::init_1::target);
RW_FUNCTION(0x73CC1C, "SceneTutorialDownload::init::$_1 target_type()", SceneTutorialDownload_lambdas::init_1::targetType);
RW_FUNCTION(0x73CC30, "SceneTutorialDownload::init::$_0 ~__func() (deleting)", SceneTutorialDownload_lambdas::init_0::deleteThis);
RW_FUNCTION(0x73CC38, "SceneTutorialDownload::init::$_0 __clone()", SceneTutorialDownload_lambdas::init_0::clone);
RW_FUNCTION(0x73CC64, "SceneTutorialDownload::init::$_0 __clone(__base*)", SceneTutorialDownload_lambdas::init_0::cloneInto);
RW_FUNCTION(0x73CC84, "SceneTutorialDownload::init::$_0 destroy()", SceneTutorialDownload_lambdas::init_0::destroy);
RW_FUNCTION(0x73CC86, "SceneTutorialDownload::init::$_0 destroy_deallocate()", SceneTutorialDownload_lambdas::init_0::deleteThis);
RW_FUNCTION(0x73CC98, "SceneTutorialDownload::init::$_0 target(const type_info&)", SceneTutorialDownload_lambdas::init_0::target);
RW_FUNCTION(0x73CCB8, "SceneTutorialDownload::init::$_0 target_type()", SceneTutorialDownload_lambdas::init_0::targetType);

// The thunks of the nested lambdas (generated from the lifted code by a scratch tool, gen_thunks.py, and checked
// against it).
namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::~__func
static uint32_t openJewelPopup_L8_Ub_dtor(uint32_t self) {
    st32(self, 0xf6d76cu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x73B888, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::~__func", openJewelPopup_L8_Ub_dtor);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::~__func (deleting)
static void openJewelPopup_L8_Ub_deletingDtor(uint32_t self) {
    st32(self, 0xf6d76cu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73B8C4, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::~__func (deleting)", openJewelPopup_L8_Ub_deletingDtor);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::__clone()
static uint32_t openJewelPopup_L8_Ub_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf6d76cu);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        uint32_t v3 = ld32(self + 16 + 0);
        uint32_t v4 = ld32(self + 16 + 4);
        uint32_t v5 = ld32(self + 16 + 8);
        uint32_t v6 = ld32(self + 16 + 12);
        st32(v1 + 16, v3);
        st32(v1 + 20, v4);
        st32(v1 + 24, v5);
        st32(v1 + 28, v6);
        call(0x45c84d, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v7 = ld32(self + 8);
        st32(v1 + 8, v7);
    }
    return v1;
}
RW_FUNCTION(0x73B904, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::__clone()", openJewelPopup_L8_Ub_clone);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::__clone(__base*)
static void openJewelPopup_L8_Ub_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d76cu);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        uint32_t v2 = ld32(self + 16 + 0);
        uint32_t v3 = ld32(self + 16 + 4);
        uint32_t v4 = ld32(self + 16 + 8);
        uint32_t v5 = ld32(self + 16 + 12);
        st32(p + 16 + 0, v2);
        st32(p + 16 + 4, v3);
        st32(p + 16 + 8, v4);
        st32(p + 16 + 12, v5);
        call(0x45c84d, {p + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v6 = ld32(self + 8);
        st32(p + 8, v6);
    }
}
RW_FUNCTION(0x73B968, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::__clone(__base*)", openJewelPopup_L8_Ub_cloneInto);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::destroy
static void openJewelPopup_L8_Ub_destroy(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x73B9B4, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::destroy", openJewelPopup_L8_Ub_destroy);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::destroy_deallocate
static void openJewelPopup_L8_Ub_destroyDeallocate(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73B9D4, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::destroy_deallocate", openJewelPopup_L8_Ub_destroyDeallocate);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::target
static uint32_t openJewelPopup_L8_Ub_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9bd40u ? self + 8 : 0; }
RW_FUNCTION(0x73BBDC, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::target", openJewelPopup_L8_Ub_target);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_: __func::target_type
static uint32_t openJewelPopup_L8_Ub_targetType() { return 0xfa73a8u; }
RW_FUNCTION(0x73BC00, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ __func::target_type", openJewelPopup_L8_Ub_targetType);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::~__func
static uint32_t openJewelPopup_L7_Uv_dtor(uint32_t self) {
    st32(self, 0xf6d798u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x73BC14, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::~__func", openJewelPopup_L7_Uv_dtor);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::~__func (deleting)
static void openJewelPopup_L7_Uv_deletingDtor(uint32_t self) {
    st32(self, 0xf6d798u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73BC50, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::~__func (deleting)", openJewelPopup_L7_Uv_deletingDtor);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::__clone()
static uint32_t openJewelPopup_L7_Uv_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf6d798u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x73BC90, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::__clone()", openJewelPopup_L7_Uv_clone);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::__clone(__base*)
static void openJewelPopup_L7_Uv_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d798u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x73BCEC, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::__clone(__base*)", openJewelPopup_L7_Uv_cloneInto);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::destroy
static void openJewelPopup_L7_Uv_destroy(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x73BD2C, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::destroy", openJewelPopup_L7_Uv_destroy);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::destroy_deallocate
static void openJewelPopup_L7_Uv_destroyDeallocate(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73BD4C, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::destroy_deallocate", openJewelPopup_L7_Uv_destroyDeallocate);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::target
static uint32_t openJewelPopup_L7_Uv_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9be60u ? self + 8 : 0; }
RW_FUNCTION(0x73BDB4, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::target", openJewelPopup_L7_Uv_target);

// SceneTutorialDownload::openJewelPopup $_7 UlvE_: __func::target_type
static uint32_t openJewelPopup_L7_Uv_targetType() { return 0xfa73bcu; }
RW_FUNCTION(0x73BDD8, "SceneTutorialDownload::openJewelPopup $_7 UlvE_ __func::target_type", openJewelPopup_L7_Uv_targetType);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_: __func::~__func (deleting)
static void openJewelPopup_L8_Ub_Ub_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73BDEC, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_ __func::~__func (deleting)", openJewelPopup_L8_Ub_Ub_deletingDtor);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_: __func::__clone()
static uint32_t openJewelPopup_L8_Ub_Ub_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d7c4u);
    }
    return v1;
}
RW_FUNCTION(0x73BDF4, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_ __func::__clone()", openJewelPopup_L8_Ub_Ub_clone);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_: __func::__clone(__base*)
static void openJewelPopup_L8_Ub_Ub_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d7c4u);
    }
}
RW_FUNCTION(0x73BE18, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_ __func::__clone(__base*)", openJewelPopup_L8_Ub_Ub_cloneInto);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_: __func::destroy
static void openJewelPopup_L8_Ub_Ub_destroy(uint32_t self) {
}
RW_FUNCTION(0x73BE34, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_ __func::destroy", openJewelPopup_L8_Ub_Ub_destroy);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_: __func::destroy_deallocate
static void openJewelPopup_L8_Ub_Ub_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73BE36, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_ __func::destroy_deallocate", openJewelPopup_L8_Ub_Ub_destroyDeallocate);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_: __func::target
static uint32_t openJewelPopup_L8_Ub_Ub_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9bf80u ? self + 4 : 0; }
RW_FUNCTION(0x73BE40, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_ __func::target", openJewelPopup_L8_Ub_Ub_target);

// SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_: __func::target_type
static uint32_t openJewelPopup_L8_Ub_Ub_targetType() { return 0xfa73d0u; }
RW_FUNCTION(0x73BE60, "SceneTutorialDownload::openJewelPopup $_8 UlbE_ UlbE_ __func::target_type", openJewelPopup_L8_Ub_Ub_targetType);

}  // namespace
