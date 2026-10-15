// AvatarPartsScrollView: the std::function __func thunks of its lambdas (deleting destructor, __clone, __clone(dst), destroy,
// destroy_deallocate, target, target_type). A lambda captures plain words (this, flags...) right after the
// __func vtable; operator() is in the lambda file.
#include "AvatarPartsScrollView.hpp"

#include "../scene/SceneCommon.hpp"

namespace {
using Addr = uint32_t;
using namespace scene;

namespace thunk {
// operator delete(self)
inline void deleteSelf(Addr self) { call(fn::operator_delete, {self}); }
// copy `words` capture words (or one capture byte) from src+4 to dst+4
inline void copyCaptures(Addr src, Addr dst, uint32_t words, bool byte) {
    if (byte) {
        st8(dst + 4, ld8(src + 4));
        return;
    }
    for (uint32_t k = 0; k < words; k++) st32(dst + 4 + 4 * k, ld32(src + 4 + 4 * k));
}
// __clone(): a heap copy (null when the allocation failed)
inline Addr clone(Addr self, Addr vtable, uint32_t size, uint32_t words, bool byte) {
    Addr p = call(fn::operator_new, {size});
    if (p) {
        st32(p, vtable);
        copyCaptures(self, p, words, byte);
    }
    return p;
}
// __clone(dst): placement copy
inline void cloneTo(Addr self, Addr dst, Addr vtable, uint32_t words, bool byte) {
    if (!dst) return;
    st32(dst, vtable);
    copyCaptures(self, dst, words, byte);
}
// target(type_info): the lambda (self + 4) when the type name pointer matches, else null
inline Addr target(Addr self, Addr type, Addr name) { return ld32(type + 4) == name ? self + 4 : 0; }
}  // namespace thunk

// vtables
constexpr Addr VT_registItemTouch_L2 = 0xF58B1C;   // registItemTouch()::$_2
constexpr Addr VT_registItemTouch_L3 = 0xF58B48;   // registItemTouch()::$_3
constexpr Addr VT_registItemTouch_L1 = 0xF58B74;   // registItemTouch()::$_1
constexpr Addr VT_registItemTouch_L0 = 0xF58BA0;   // registItemTouch()::$_0

// registItemTouch()::$_2  (operator() at 0x554FAA)
void registItemTouch_L2_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x554F40, "AvatarPartsScrollView::registItemTouch()::$_2 __func::~__func (deleting)", registItemTouch_L2_D0);
Addr registItemTouch_L2_clone(Addr self) { return thunk::clone(self, VT_registItemTouch_L2, 16, 3, false); }
RW_FUNCTION(0x554F48, "AvatarPartsScrollView::registItemTouch()::$_2 __func::__clone()", registItemTouch_L2_clone);
void registItemTouch_L2_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_registItemTouch_L2, 3, false); }
RW_FUNCTION(0x554F78, "AvatarPartsScrollView::registItemTouch()::$_2 __func::__clone(__base*)", registItemTouch_L2_cloneTo);
void registItemTouch_L2_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x554FA0, "AvatarPartsScrollView::registItemTouch()::$_2 __func::destroy", registItemTouch_L2_destroy);
void registItemTouch_L2_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x554FA2, "AvatarPartsScrollView::registItemTouch()::$_2 __func::destroy_deallocate", registItemTouch_L2_destroyDealloc);
Addr registItemTouch_L2_target(Addr self, Addr type) { return thunk::target(self, type, 0xE309B0); }
RW_FUNCTION(0x554FB8, "AvatarPartsScrollView::registItemTouch()::$_2 __func::target", registItemTouch_L2_target);
Addr registItemTouch_L2_targetType() { return 0xF8814C; }
RW_FUNCTION(0x554FD8, "AvatarPartsScrollView::registItemTouch()::$_2 __func::target_type", registItemTouch_L2_targetType);

// registItemTouch()::$_3  (operator() at 0x55523C)
void registItemTouch_L3_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x5551DC, "AvatarPartsScrollView::registItemTouch()::$_3 __func::~__func (deleting)", registItemTouch_L3_D0);
Addr registItemTouch_L3_clone(Addr self) { return thunk::clone(self, VT_registItemTouch_L3, 8, 1, false); }
RW_FUNCTION(0x5551E4, "AvatarPartsScrollView::registItemTouch()::$_3 __func::__clone()", registItemTouch_L3_clone);
void registItemTouch_L3_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_registItemTouch_L3, 1, false); }
RW_FUNCTION(0x555210, "AvatarPartsScrollView::registItemTouch()::$_3 __func::__clone(__base*)", registItemTouch_L3_cloneTo);
void registItemTouch_L3_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x555230, "AvatarPartsScrollView::registItemTouch()::$_3 __func::destroy", registItemTouch_L3_destroy);
void registItemTouch_L3_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x555232, "AvatarPartsScrollView::registItemTouch()::$_3 __func::destroy_deallocate", registItemTouch_L3_destroyDealloc);
Addr registItemTouch_L3_target(Addr self, Addr type) { return thunk::target(self, type, 0xE30A80); }
RW_FUNCTION(0x5554C0, "AvatarPartsScrollView::registItemTouch()::$_3 __func::target", registItemTouch_L3_target);
Addr registItemTouch_L3_targetType() { return 0xF88160; }
RW_FUNCTION(0x5554E0, "AvatarPartsScrollView::registItemTouch()::$_3 __func::target_type", registItemTouch_L3_targetType);

// registItemTouch()::$_1  (operator() at 0x555564)
void registItemTouch_L1_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x5554F4, "AvatarPartsScrollView::registItemTouch()::$_1 __func::~__func (deleting)", registItemTouch_L1_D0);
Addr registItemTouch_L1_clone(Addr self) { return thunk::clone(self, VT_registItemTouch_L1, 24, 5, false); }
RW_FUNCTION(0x5554FC, "AvatarPartsScrollView::registItemTouch()::$_1 __func::__clone()", registItemTouch_L1_clone);
void registItemTouch_L1_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_registItemTouch_L1, 5, false); }
RW_FUNCTION(0x555530, "AvatarPartsScrollView::registItemTouch()::$_1 __func::__clone(__base*)", registItemTouch_L1_cloneTo);
void registItemTouch_L1_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x555558, "AvatarPartsScrollView::registItemTouch()::$_1 __func::destroy", registItemTouch_L1_destroy);
void registItemTouch_L1_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x55555A, "AvatarPartsScrollView::registItemTouch()::$_1 __func::destroy_deallocate", registItemTouch_L1_destroyDealloc);
Addr registItemTouch_L1_target(Addr self, Addr type) { return thunk::target(self, type, 0xE30B70); }
RW_FUNCTION(0x5556D0, "AvatarPartsScrollView::registItemTouch()::$_1 __func::target", registItemTouch_L1_target);
Addr registItemTouch_L1_targetType() { return 0xF88174; }
RW_FUNCTION(0x5556F0, "AvatarPartsScrollView::registItemTouch()::$_1 __func::target_type", registItemTouch_L1_targetType);

// registItemTouch()::$_0  (operator() at 0x55576C)
void registItemTouch_L0_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x555704, "AvatarPartsScrollView::registItemTouch()::$_0 __func::~__func (deleting)", registItemTouch_L0_D0);
Addr registItemTouch_L0_clone(Addr self) { return thunk::clone(self, VT_registItemTouch_L0, 12, 2, false); }
RW_FUNCTION(0x55570C, "AvatarPartsScrollView::registItemTouch()::$_0 __func::__clone()", registItemTouch_L0_clone);
void registItemTouch_L0_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_registItemTouch_L0, 2, false); }
RW_FUNCTION(0x55573C, "AvatarPartsScrollView::registItemTouch()::$_0 __func::__clone(__base*)", registItemTouch_L0_cloneTo);
void registItemTouch_L0_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x555760, "AvatarPartsScrollView::registItemTouch()::$_0 __func::destroy", registItemTouch_L0_destroy);
void registItemTouch_L0_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x555762, "AvatarPartsScrollView::registItemTouch()::$_0 __func::destroy_deallocate", registItemTouch_L0_destroyDealloc);
Addr registItemTouch_L0_target(Addr self, Addr type) { return thunk::target(self, type, 0xE30C60); }
RW_FUNCTION(0x555A00, "AvatarPartsScrollView::registItemTouch()::$_0 __func::target", registItemTouch_L0_target);
Addr registItemTouch_L0_targetType() { return 0xF88188; }
RW_FUNCTION(0x555A20, "AvatarPartsScrollView::registItemTouch()::$_0 __func::target_type", registItemTouch_L0_targetType);

}  // namespace
