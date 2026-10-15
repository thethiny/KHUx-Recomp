// SceneAvatarEdit: the std::function __func thunks of its lambdas (deleting destructor, __clone, __clone(dst), destroy,
// destroy_deallocate, target, target_type). A lambda captures plain words (this, flags...) right after the
// __func vtable; operator() is in the lambda file.
#include "SceneAvatarEdit.hpp"

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
constexpr Addr VT_backScene_L40 = 0xF6A5C0;   // backScene()::$_40
constexpr Addr VT_saveMyCoordinate_L39 = 0xF6A5EC;   // saveMyCoordinate()::$_39
constexpr Addr VT_saveMyCoordinate_L39_fnb = 0xF6A618;   // saveMyCoordinate()::$_39::lambda(bool)
constexpr Addr VT_openSavePopup_L38 = 0xF6A644;   // openSavePopup()::$_38
constexpr Addr VT_openSavePopup_L43_fnv = 0xF6A670;   // openSavePopup()::$_43::lambda()
constexpr Addr VT_openCreateCheckPopup_L37 = 0xF6A69C;   // openCreateCheckPopup()::$_37
constexpr Addr VT_openCreateCheckPopup_L37_fnb = 0xF6A6C8;   // openCreateCheckPopup()::$_37::lambda(bool)
constexpr Addr VT_openCreateCheckPopup_L37_fnv_fnb = 0xF6A6F4;   // openCreateCheckPopup()::$_37::lambda()::lambda(bool)
constexpr Addr VT_openCreateCheckPopup_L37_fnb2 = 0xF6A720;   // openCreateCheckPopup()::$_37::lambda(bool)#2
constexpr Addr VT_openCreateCheckPopup_L42_fnv = 0xF6A74C;   // openCreateCheckPopup()::$_42::lambda()
constexpr Addr VT_openCreateCheckPopup_L36 = 0xF6A778;   // openCreateCheckPopup()::$_36
constexpr Addr VT_openCreateCheckPopup_L35 = 0xF6A7A4;   // openCreateCheckPopup()::$_35
constexpr Addr VT_openCreateCheckPopup_L34 = 0xF6A7D0;   // openCreateCheckPopup()::$_34
constexpr Addr VT_openAccessoriesFilterPopup_L31 = 0xF6A7FC;   // openAccessoriesFilterPopup()::$_31
constexpr Addr VT_openAccessoriesFilterPopup_L41_fnv = 0xF6A828;   // openAccessoriesFilterPopup()::$_41::lambda()
constexpr Addr VT_openAccessoriesFilterPopup_L30 = 0xF6A854;   // openAccessoriesFilterPopup()::$_30
constexpr Addr VT_openAccessoriesFilterPopup_L29 = 0xF6A880;   // openAccessoriesFilterPopup()::$_29
constexpr Addr VT_openAccessoriesFilterPopup_L28 = 0xF6A8AC;   // openAccessoriesFilterPopup()::$_28
constexpr Addr VT_openAccessoriesFilterPopup_L27 = 0xF6A8D8;   // openAccessoriesFilterPopup()::$_27
constexpr Addr VT_openAccessoriesFilterPopup_L26 = 0xF6A904;   // openAccessoriesFilterPopup()::$_26
constexpr Addr VT_openAccessoriesFilterPopup_L25 = 0xF6A930;   // openAccessoriesFilterPopup()::$_25
constexpr Addr VT_openAccessoriesFilterPopup_L24 = 0xF6A95C;   // openAccessoriesFilterPopup()::$_24
constexpr Addr VT_openAccessoriesFilterPopup_L23 = 0xF6A988;   // openAccessoriesFilterPopup()::$_23
constexpr Addr VT_openAccessoriesFilterPopup_L22 = 0xF6A9B4;   // openAccessoriesFilterPopup()::$_22
constexpr Addr VT_openAccessoriesFilterPopup_L21 = 0xF6A9E0;   // openAccessoriesFilterPopup()::$_21
constexpr Addr VT_openAccessoriesFilterPopup_L20 = 0xF6AA0C;   // openAccessoriesFilterPopup()::$_20
constexpr Addr VT_openAccessoriesFilterPopup_L19 = 0xF6AA38;   // openAccessoriesFilterPopup()::$_19
constexpr Addr VT_openAccessoriesFilterPopup_L18 = 0xF6AA64;   // openAccessoriesFilterPopup()::$_18
constexpr Addr VT_initTabButton_L17 = 0xF6AA90;   // initTabButton()::$_17
constexpr Addr VT_initTabButton_L16 = 0xF6AABC;   // initTabButton()::$_16
constexpr Addr VT_initMyCordinateButton_L15 = 0xF6AAE8;   // initMyCordinateButton()::$_15
constexpr Addr VT_runOutAnimationMain_L11 = 0xF6AB14;   // runOutAnimationMain()::$_11
constexpr Addr VT_init_L10 = 0xF6AB40;   // init()::$_10
constexpr Addr VT_init_L9 = 0xF6AB6C;   // init()::$_9
constexpr Addr VT_init_L8 = 0xF6AB98;   // init()::$_8
constexpr Addr VT_init_L7 = 0xF6ABC4;   // init()::$_7
constexpr Addr VT_init_L6 = 0xF6ABF0;   // init()::$_6
constexpr Addr VT_init_L5 = 0xF6AC1C;   // init()::$_5
constexpr Addr VT_init_L4 = 0xF6AC48;   // init()::$_4
constexpr Addr VT_init_L4_fnb = 0xF6AC74;   // init()::$_4::lambda(bool)
constexpr Addr VT_init_L3 = 0xF6ACA0;   // init()::$_3
constexpr Addr VT_init_L2 = 0xF6ACCC;   // init()::$_2
constexpr Addr VT_init_L1 = 0xF6ACF8;   // init()::$_1
constexpr Addr VT_init_L0 = 0xF6AD24;   // init()::$_0

// backScene()::$_40  (operator() at 0x70A850)
void backScene_L40_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70A7FC, "SceneAvatarEdit::backScene()::$_40 __func::~__func (deleting)", backScene_L40_D0);
Addr backScene_L40_clone(Addr self) { return thunk::clone(self, VT_backScene_L40, 8, 0, false); }
RW_FUNCTION(0x70A804, "SceneAvatarEdit::backScene()::$_40 __func::__clone()", backScene_L40_clone);
void backScene_L40_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_backScene_L40, 0, false); }
RW_FUNCTION(0x70A828, "SceneAvatarEdit::backScene()::$_40 __func::__clone(__base*)", backScene_L40_cloneTo);
void backScene_L40_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70A844, "SceneAvatarEdit::backScene()::$_40 __func::destroy", backScene_L40_destroy);
void backScene_L40_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70A846, "SceneAvatarEdit::backScene()::$_40 __func::destroy_deallocate", backScene_L40_destroyDealloc);
Addr backScene_L40_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C250); }
RW_FUNCTION(0x70A970, "SceneAvatarEdit::backScene()::$_40 __func::target", backScene_L40_target);
Addr backScene_L40_targetType() { return 0xFA36DC; }
RW_FUNCTION(0x70A990, "SceneAvatarEdit::backScene()::$_40 __func::target_type", backScene_L40_targetType);

// saveMyCoordinate()::$_39  (operator() at 0x70AAF0)
Addr saveMyCoordinate_L39_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C300); }
RW_FUNCTION(0x70ABD4, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::target", saveMyCoordinate_L39_target);
Addr saveMyCoordinate_L39_targetType() { return 0xFA36F0; }
RW_FUNCTION(0x70ABF4, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::target_type", saveMyCoordinate_L39_targetType);

// saveMyCoordinate()::$_39::lambda(bool)  (operator() at 0x70AC5A)
void saveMyCoordinate_L39_fnb_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70AC08, "SceneAvatarEdit::saveMyCoordinate()::$_39::lambda(bool) __func::~__func (deleting)", saveMyCoordinate_L39_fnb_D0);
Addr saveMyCoordinate_L39_fnb_clone(Addr self) { return thunk::clone(self, VT_saveMyCoordinate_L39_fnb, 8, 0, false); }
RW_FUNCTION(0x70AC10, "SceneAvatarEdit::saveMyCoordinate()::$_39::lambda(bool) __func::__clone()", saveMyCoordinate_L39_fnb_clone);
void saveMyCoordinate_L39_fnb_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_saveMyCoordinate_L39_fnb, 0, false); }
RW_FUNCTION(0x70AC34, "SceneAvatarEdit::saveMyCoordinate()::$_39::lambda(bool) __func::__clone(__base*)", saveMyCoordinate_L39_fnb_cloneTo);
void saveMyCoordinate_L39_fnb_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70AC50, "SceneAvatarEdit::saveMyCoordinate()::$_39::lambda(bool) __func::destroy", saveMyCoordinate_L39_fnb_destroy);
void saveMyCoordinate_L39_fnb_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70AC52, "SceneAvatarEdit::saveMyCoordinate()::$_39::lambda(bool) __func::destroy_deallocate", saveMyCoordinate_L39_fnb_destroyDealloc);
Addr saveMyCoordinate_L39_fnb_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C3D0); }
RW_FUNCTION(0x70AC5C, "SceneAvatarEdit::saveMyCoordinate()::$_39::lambda(bool) __func::target", saveMyCoordinate_L39_fnb_target);
Addr saveMyCoordinate_L39_fnb_targetType() { return 0xFA3704; }
RW_FUNCTION(0x70AC7C, "SceneAvatarEdit::saveMyCoordinate()::$_39::lambda(bool) __func::target_type", saveMyCoordinate_L39_fnb_targetType);

// openSavePopup()::$_38  (operator() at 0x70AD04)
void openSavePopup_L38_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70AC90, "SceneAvatarEdit::openSavePopup()::$_38 __func::~__func (deleting)", openSavePopup_L38_D0);
Addr openSavePopup_L38_clone(Addr self) { return thunk::clone(self, VT_openSavePopup_L38, 20, 4, false); }
RW_FUNCTION(0x70AC98, "SceneAvatarEdit::openSavePopup()::$_38 __func::__clone()", openSavePopup_L38_clone);
void openSavePopup_L38_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openSavePopup_L38, 4, false); }
RW_FUNCTION(0x70ACCC, "SceneAvatarEdit::openSavePopup()::$_38 __func::__clone(__base*)", openSavePopup_L38_cloneTo);
void openSavePopup_L38_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70ACF8, "SceneAvatarEdit::openSavePopup()::$_38 __func::destroy", openSavePopup_L38_destroy);
void openSavePopup_L38_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70ACFA, "SceneAvatarEdit::openSavePopup()::$_38 __func::destroy_deallocate", openSavePopup_L38_destroyDealloc);
Addr openSavePopup_L38_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C4C0); }
RW_FUNCTION(0x70ADE4, "SceneAvatarEdit::openSavePopup()::$_38 __func::target", openSavePopup_L38_target);
Addr openSavePopup_L38_targetType() { return 0xFA3718; }
RW_FUNCTION(0x70AE04, "SceneAvatarEdit::openSavePopup()::$_38 __func::target_type", openSavePopup_L38_targetType);

// openSavePopup()::$_43::lambda()  (operator() at 0x70AE82)
void openSavePopup_L43_fnv_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70AE18, "SceneAvatarEdit::openSavePopup()::$_43::lambda() __func::~__func (deleting)", openSavePopup_L43_fnv_D0);
Addr openSavePopup_L43_fnv_clone(Addr self) { return thunk::clone(self, VT_openSavePopup_L43_fnv, 16, 3, false); }
RW_FUNCTION(0x70AE20, "SceneAvatarEdit::openSavePopup()::$_43::lambda() __func::__clone()", openSavePopup_L43_fnv_clone);
void openSavePopup_L43_fnv_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openSavePopup_L43_fnv, 3, false); }
RW_FUNCTION(0x70AE50, "SceneAvatarEdit::openSavePopup()::$_43::lambda() __func::__clone(__base*)", openSavePopup_L43_fnv_cloneTo);
void openSavePopup_L43_fnv_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70AE78, "SceneAvatarEdit::openSavePopup()::$_43::lambda() __func::destroy", openSavePopup_L43_fnv_destroy);
void openSavePopup_L43_fnv_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70AE7A, "SceneAvatarEdit::openSavePopup()::$_43::lambda() __func::destroy_deallocate", openSavePopup_L43_fnv_destroyDealloc);
Addr openSavePopup_L43_fnv_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C560); }
RW_FUNCTION(0x70AEAC, "SceneAvatarEdit::openSavePopup()::$_43::lambda() __func::target", openSavePopup_L43_fnv_target);
Addr openSavePopup_L43_fnv_targetType() { return 0xFA372C; }
RW_FUNCTION(0x70AECC, "SceneAvatarEdit::openSavePopup()::$_43::lambda() __func::target_type", openSavePopup_L43_fnv_targetType);

// openCreateCheckPopup()::$_37  (operator() at 0x70AF50)
void openCreateCheckPopup_L37_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70AEE0, "SceneAvatarEdit::openCreateCheckPopup()::$_37 __func::~__func (deleting)", openCreateCheckPopup_L37_D0);
Addr openCreateCheckPopup_L37_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L37, 24, 5, false); }
RW_FUNCTION(0x70AEE8, "SceneAvatarEdit::openCreateCheckPopup()::$_37 __func::__clone()", openCreateCheckPopup_L37_clone);
void openCreateCheckPopup_L37_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L37, 5, false); }
RW_FUNCTION(0x70AF1C, "SceneAvatarEdit::openCreateCheckPopup()::$_37 __func::__clone(__base*)", openCreateCheckPopup_L37_cloneTo);
void openCreateCheckPopup_L37_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70AF44, "SceneAvatarEdit::openCreateCheckPopup()::$_37 __func::destroy", openCreateCheckPopup_L37_destroy);
void openCreateCheckPopup_L37_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70AF46, "SceneAvatarEdit::openCreateCheckPopup()::$_37 __func::destroy_deallocate", openCreateCheckPopup_L37_destroyDealloc);
Addr openCreateCheckPopup_L37_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C640); }
RW_FUNCTION(0x70B0E0, "SceneAvatarEdit::openCreateCheckPopup()::$_37 __func::target", openCreateCheckPopup_L37_target);
Addr openCreateCheckPopup_L37_targetType() { return 0xFA3740; }
RW_FUNCTION(0x70B100, "SceneAvatarEdit::openCreateCheckPopup()::$_37 __func::target_type", openCreateCheckPopup_L37_targetType);

// openCreateCheckPopup()::$_37::lambda(bool)  (operator() at 0x70B24E)
void openCreateCheckPopup_L37_fnb_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B1F0, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool) __func::~__func (deleting)", openCreateCheckPopup_L37_fnb_D0);
Addr openCreateCheckPopup_L37_fnb_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L37_fnb, 8, 1, true); }
RW_FUNCTION(0x70B1F8, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool) __func::__clone()", openCreateCheckPopup_L37_fnb_clone);
void openCreateCheckPopup_L37_fnb_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L37_fnb, 1, true); }
RW_FUNCTION(0x70B224, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool) __func::__clone(__base*)", openCreateCheckPopup_L37_fnb_cloneTo);
void openCreateCheckPopup_L37_fnb_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70B244, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool) __func::destroy", openCreateCheckPopup_L37_fnb_destroy);
void openCreateCheckPopup_L37_fnb_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B246, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool) __func::destroy_deallocate", openCreateCheckPopup_L37_fnb_destroyDealloc);
Addr openCreateCheckPopup_L37_fnb_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C720); }
RW_FUNCTION(0x70B25C, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool) __func::target", openCreateCheckPopup_L37_fnb_target);
Addr openCreateCheckPopup_L37_fnb_targetType() { return 0xFA3754; }
RW_FUNCTION(0x70B27C, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool) __func::target_type", openCreateCheckPopup_L37_fnb_targetType);

// openCreateCheckPopup()::$_37::lambda()::lambda(bool)  (operator() at 0x70B3D8)
void openCreateCheckPopup_L37_fnv_fnb_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B384, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda()::lambda(bool) __func::~__func (deleting)", openCreateCheckPopup_L37_fnv_fnb_D0);
Addr openCreateCheckPopup_L37_fnv_fnb_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L37_fnv_fnb, 8, 0, false); }
RW_FUNCTION(0x70B38C, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda()::lambda(bool) __func::__clone()", openCreateCheckPopup_L37_fnv_fnb_clone);
void openCreateCheckPopup_L37_fnv_fnb_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L37_fnv_fnb, 0, false); }
RW_FUNCTION(0x70B3B0, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda()::lambda(bool) __func::__clone(__base*)", openCreateCheckPopup_L37_fnv_fnb_cloneTo);
void openCreateCheckPopup_L37_fnv_fnb_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70B3CC, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda()::lambda(bool) __func::destroy", openCreateCheckPopup_L37_fnv_fnb_destroy);
void openCreateCheckPopup_L37_fnv_fnb_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B3CE, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda()::lambda(bool) __func::destroy_deallocate", openCreateCheckPopup_L37_fnv_fnb_destroyDealloc);
Addr openCreateCheckPopup_L37_fnv_fnb_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C840); }
RW_FUNCTION(0x70B468, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda()::lambda(bool) __func::target", openCreateCheckPopup_L37_fnv_fnb_target);
Addr openCreateCheckPopup_L37_fnv_fnb_targetType() { return 0xFA3768; }
RW_FUNCTION(0x70B488, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda()::lambda(bool) __func::target_type", openCreateCheckPopup_L37_fnv_fnb_targetType);

// openCreateCheckPopup()::$_37::lambda(bool)#2  (operator() at 0x70B504)
void openCreateCheckPopup_L37_fnb2_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B49C, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool)#2 __func::~__func (deleting)", openCreateCheckPopup_L37_fnb2_D0);
Addr openCreateCheckPopup_L37_fnb2_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L37_fnb2, 12, 2, false); }
RW_FUNCTION(0x70B4A4, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool)#2 __func::__clone()", openCreateCheckPopup_L37_fnb2_clone);
void openCreateCheckPopup_L37_fnb2_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L37_fnb2, 2, false); }
RW_FUNCTION(0x70B4D4, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool)#2 __func::__clone(__base*)", openCreateCheckPopup_L37_fnb2_cloneTo);
void openCreateCheckPopup_L37_fnb2_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70B4F8, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool)#2 __func::destroy", openCreateCheckPopup_L37_fnb2_destroy);
void openCreateCheckPopup_L37_fnb2_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B4FA, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool)#2 __func::destroy_deallocate", openCreateCheckPopup_L37_fnb2_destroyDealloc);
Addr openCreateCheckPopup_L37_fnb2_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8C970); }
RW_FUNCTION(0x70B5DC, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool)#2 __func::target", openCreateCheckPopup_L37_fnb2_target);
Addr openCreateCheckPopup_L37_fnb2_targetType() { return 0xFA377C; }
RW_FUNCTION(0x70B5FC, "SceneAvatarEdit::openCreateCheckPopup()::$_37::lambda(bool)#2 __func::target_type", openCreateCheckPopup_L37_fnb2_targetType);

// openCreateCheckPopup()::$_42::lambda()  (operator() at 0x70B67A)
void openCreateCheckPopup_L42_fnv_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B610, "SceneAvatarEdit::openCreateCheckPopup()::$_42::lambda() __func::~__func (deleting)", openCreateCheckPopup_L42_fnv_D0);
Addr openCreateCheckPopup_L42_fnv_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L42_fnv, 16, 3, false); }
RW_FUNCTION(0x70B618, "SceneAvatarEdit::openCreateCheckPopup()::$_42::lambda() __func::__clone()", openCreateCheckPopup_L42_fnv_clone);
void openCreateCheckPopup_L42_fnv_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L42_fnv, 3, false); }
RW_FUNCTION(0x70B648, "SceneAvatarEdit::openCreateCheckPopup()::$_42::lambda() __func::__clone(__base*)", openCreateCheckPopup_L42_fnv_cloneTo);
void openCreateCheckPopup_L42_fnv_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70B670, "SceneAvatarEdit::openCreateCheckPopup()::$_42::lambda() __func::destroy", openCreateCheckPopup_L42_fnv_destroy);
void openCreateCheckPopup_L42_fnv_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B672, "SceneAvatarEdit::openCreateCheckPopup()::$_42::lambda() __func::destroy_deallocate", openCreateCheckPopup_L42_fnv_destroyDealloc);
Addr openCreateCheckPopup_L42_fnv_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CA60); }
RW_FUNCTION(0x70B6A4, "SceneAvatarEdit::openCreateCheckPopup()::$_42::lambda() __func::target", openCreateCheckPopup_L42_fnv_target);
Addr openCreateCheckPopup_L42_fnv_targetType() { return 0xFA3790; }
RW_FUNCTION(0x70B6C4, "SceneAvatarEdit::openCreateCheckPopup()::$_42::lambda() __func::target_type", openCreateCheckPopup_L42_fnv_targetType);

// openCreateCheckPopup()::$_36  (operator() at 0x70B74A)
void openCreateCheckPopup_L36_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B6D8, "SceneAvatarEdit::openCreateCheckPopup()::$_36 __func::~__func (deleting)", openCreateCheckPopup_L36_D0);
Addr openCreateCheckPopup_L36_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L36, 20, 4, false); }
RW_FUNCTION(0x70B6E0, "SceneAvatarEdit::openCreateCheckPopup()::$_36 __func::__clone()", openCreateCheckPopup_L36_clone);
void openCreateCheckPopup_L36_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L36, 4, false); }
RW_FUNCTION(0x70B714, "SceneAvatarEdit::openCreateCheckPopup()::$_36 __func::__clone(__base*)", openCreateCheckPopup_L36_cloneTo);
void openCreateCheckPopup_L36_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70B740, "SceneAvatarEdit::openCreateCheckPopup()::$_36 __func::destroy", openCreateCheckPopup_L36_destroy);
void openCreateCheckPopup_L36_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B742, "SceneAvatarEdit::openCreateCheckPopup()::$_36 __func::destroy_deallocate", openCreateCheckPopup_L36_destroyDealloc);
Addr openCreateCheckPopup_L36_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CB10); }
RW_FUNCTION(0x70B764, "SceneAvatarEdit::openCreateCheckPopup()::$_36 __func::target", openCreateCheckPopup_L36_target);
Addr openCreateCheckPopup_L36_targetType() { return 0xFA37A4; }
RW_FUNCTION(0x70B784, "SceneAvatarEdit::openCreateCheckPopup()::$_36 __func::target_type", openCreateCheckPopup_L36_targetType);

// openCreateCheckPopup()::$_35  (operator() at 0x70B7F6)
void openCreateCheckPopup_L35_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B798, "SceneAvatarEdit::openCreateCheckPopup()::$_35 __func::~__func (deleting)", openCreateCheckPopup_L35_D0);
Addr openCreateCheckPopup_L35_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L35, 8, 1, false); }
RW_FUNCTION(0x70B7A0, "SceneAvatarEdit::openCreateCheckPopup()::$_35 __func::__clone()", openCreateCheckPopup_L35_clone);
void openCreateCheckPopup_L35_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L35, 1, false); }
RW_FUNCTION(0x70B7CC, "SceneAvatarEdit::openCreateCheckPopup()::$_35 __func::__clone(__base*)", openCreateCheckPopup_L35_cloneTo);
void openCreateCheckPopup_L35_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70B7EC, "SceneAvatarEdit::openCreateCheckPopup()::$_35 __func::destroy", openCreateCheckPopup_L35_destroy);
void openCreateCheckPopup_L35_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B7EE, "SceneAvatarEdit::openCreateCheckPopup()::$_35 __func::destroy_deallocate", openCreateCheckPopup_L35_destroyDealloc);
Addr openCreateCheckPopup_L35_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CBC0); }
RW_FUNCTION(0x70B800, "SceneAvatarEdit::openCreateCheckPopup()::$_35 __func::target", openCreateCheckPopup_L35_target);
Addr openCreateCheckPopup_L35_targetType() { return 0xFA37B8; }
RW_FUNCTION(0x70B820, "SceneAvatarEdit::openCreateCheckPopup()::$_35 __func::target_type", openCreateCheckPopup_L35_targetType);

// openCreateCheckPopup()::$_34  (operator() at 0x70B886)
void openCreateCheckPopup_L34_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B834, "SceneAvatarEdit::openCreateCheckPopup()::$_34 __func::~__func (deleting)", openCreateCheckPopup_L34_D0);
Addr openCreateCheckPopup_L34_clone(Addr self) { return thunk::clone(self, VT_openCreateCheckPopup_L34, 8, 0, false); }
RW_FUNCTION(0x70B83C, "SceneAvatarEdit::openCreateCheckPopup()::$_34 __func::__clone()", openCreateCheckPopup_L34_clone);
void openCreateCheckPopup_L34_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openCreateCheckPopup_L34, 0, false); }
RW_FUNCTION(0x70B860, "SceneAvatarEdit::openCreateCheckPopup()::$_34 __func::__clone(__base*)", openCreateCheckPopup_L34_cloneTo);
void openCreateCheckPopup_L34_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70B87C, "SceneAvatarEdit::openCreateCheckPopup()::$_34 __func::destroy", openCreateCheckPopup_L34_destroy);
void openCreateCheckPopup_L34_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70B87E, "SceneAvatarEdit::openCreateCheckPopup()::$_34 __func::destroy_deallocate", openCreateCheckPopup_L34_destroyDealloc);
Addr openCreateCheckPopup_L34_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CCA0); }
RW_FUNCTION(0x70B89C, "SceneAvatarEdit::openCreateCheckPopup()::$_34 __func::target", openCreateCheckPopup_L34_target);
Addr openCreateCheckPopup_L34_targetType() { return 0xFA37CC; }
RW_FUNCTION(0x70B8BC, "SceneAvatarEdit::openCreateCheckPopup()::$_34 __func::target_type", openCreateCheckPopup_L34_targetType);

// openAccessoriesFilterPopup()::$_31  (operator() at 0x70BAC8)
void openAccessoriesFilterPopup_L31_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BA50, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_31 __func::~__func (deleting)", openAccessoriesFilterPopup_L31_D0);
Addr openAccessoriesFilterPopup_L31_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L31, 28, 6, false); }
RW_FUNCTION(0x70BA58, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_31 __func::__clone()", openAccessoriesFilterPopup_L31_clone);
void openAccessoriesFilterPopup_L31_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L31, 6, false); }
RW_FUNCTION(0x70BA90, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_31 __func::__clone(__base*)", openAccessoriesFilterPopup_L31_cloneTo);
void openAccessoriesFilterPopup_L31_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70BABC, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_31 __func::destroy", openAccessoriesFilterPopup_L31_destroy);
void openAccessoriesFilterPopup_L31_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BABE, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_31 __func::destroy_deallocate", openAccessoriesFilterPopup_L31_destroyDealloc);
Addr openAccessoriesFilterPopup_L31_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CD50); }
RW_FUNCTION(0x70BBC4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_31 __func::target", openAccessoriesFilterPopup_L31_target);
Addr openAccessoriesFilterPopup_L31_targetType() { return 0xFA37E0; }
RW_FUNCTION(0x70BBE4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_31 __func::target_type", openAccessoriesFilterPopup_L31_targetType);

// openAccessoriesFilterPopup()::$_41::lambda()  (operator() at 0x70BC62)
void openAccessoriesFilterPopup_L41_fnv_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BBF8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_41::lambda() __func::~__func (deleting)", openAccessoriesFilterPopup_L41_fnv_D0);
Addr openAccessoriesFilterPopup_L41_fnv_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L41_fnv, 16, 3, false); }
RW_FUNCTION(0x70BC00, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_41::lambda() __func::__clone()", openAccessoriesFilterPopup_L41_fnv_clone);
void openAccessoriesFilterPopup_L41_fnv_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L41_fnv, 3, false); }
RW_FUNCTION(0x70BC30, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_41::lambda() __func::__clone(__base*)", openAccessoriesFilterPopup_L41_fnv_cloneTo);
void openAccessoriesFilterPopup_L41_fnv_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70BC58, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_41::lambda() __func::destroy", openAccessoriesFilterPopup_L41_fnv_destroy);
void openAccessoriesFilterPopup_L41_fnv_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BC5A, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_41::lambda() __func::destroy_deallocate", openAccessoriesFilterPopup_L41_fnv_destroyDealloc);
Addr openAccessoriesFilterPopup_L41_fnv_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CE10); }
RW_FUNCTION(0x70BC8C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_41::lambda() __func::target", openAccessoriesFilterPopup_L41_fnv_target);
Addr openAccessoriesFilterPopup_L41_fnv_targetType() { return 0xFA37F4; }
RW_FUNCTION(0x70BCAC, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_41::lambda() __func::target_type", openAccessoriesFilterPopup_L41_fnv_targetType);

// openAccessoriesFilterPopup()::$_30  (operator() at 0x70BD1E)
void openAccessoriesFilterPopup_L30_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BCC0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_30 __func::~__func (deleting)", openAccessoriesFilterPopup_L30_D0);
Addr openAccessoriesFilterPopup_L30_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L30, 8, 1, false); }
RW_FUNCTION(0x70BCC8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_30 __func::__clone()", openAccessoriesFilterPopup_L30_clone);
void openAccessoriesFilterPopup_L30_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L30, 1, false); }
RW_FUNCTION(0x70BCF4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_30 __func::__clone(__base*)", openAccessoriesFilterPopup_L30_cloneTo);
void openAccessoriesFilterPopup_L30_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70BD14, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_30 __func::destroy", openAccessoriesFilterPopup_L30_destroy);
void openAccessoriesFilterPopup_L30_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BD16, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_30 __func::destroy_deallocate", openAccessoriesFilterPopup_L30_destroyDealloc);
Addr openAccessoriesFilterPopup_L30_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CED0); }
RW_FUNCTION(0x70BD28, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_30 __func::target", openAccessoriesFilterPopup_L30_target);
Addr openAccessoriesFilterPopup_L30_targetType() { return 0xFA3808; }
RW_FUNCTION(0x70BD48, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_30 __func::target_type", openAccessoriesFilterPopup_L30_targetType);

// openAccessoriesFilterPopup()::$_29  (operator() at 0x70BDBA)
void openAccessoriesFilterPopup_L29_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BD5C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_29 __func::~__func (deleting)", openAccessoriesFilterPopup_L29_D0);
Addr openAccessoriesFilterPopup_L29_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L29, 8, 1, false); }
RW_FUNCTION(0x70BD64, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_29 __func::__clone()", openAccessoriesFilterPopup_L29_clone);
void openAccessoriesFilterPopup_L29_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L29, 1, false); }
RW_FUNCTION(0x70BD90, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_29 __func::__clone(__base*)", openAccessoriesFilterPopup_L29_cloneTo);
void openAccessoriesFilterPopup_L29_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70BDB0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_29 __func::destroy", openAccessoriesFilterPopup_L29_destroy);
void openAccessoriesFilterPopup_L29_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BDB2, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_29 __func::destroy_deallocate", openAccessoriesFilterPopup_L29_destroyDealloc);
Addr openAccessoriesFilterPopup_L29_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8CFB0); }
RW_FUNCTION(0x70BDD8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_29 __func::target", openAccessoriesFilterPopup_L29_target);
Addr openAccessoriesFilterPopup_L29_targetType() { return 0xFA381C; }
RW_FUNCTION(0x70BDF8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_29 __func::target_type", openAccessoriesFilterPopup_L29_targetType);

// openAccessoriesFilterPopup()::$_28  (operator() at 0x70BE6C)
void openAccessoriesFilterPopup_L28_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BE0C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_28 __func::~__func (deleting)", openAccessoriesFilterPopup_L28_D0);
Addr openAccessoriesFilterPopup_L28_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L28, 8, 1, false); }
RW_FUNCTION(0x70BE14, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_28 __func::__clone()", openAccessoriesFilterPopup_L28_clone);
void openAccessoriesFilterPopup_L28_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L28, 1, false); }
RW_FUNCTION(0x70BE40, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_28 __func::__clone(__base*)", openAccessoriesFilterPopup_L28_cloneTo);
void openAccessoriesFilterPopup_L28_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70BE60, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_28 __func::destroy", openAccessoriesFilterPopup_L28_destroy);
void openAccessoriesFilterPopup_L28_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BE62, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_28 __func::destroy_deallocate", openAccessoriesFilterPopup_L28_destroyDealloc);
Addr openAccessoriesFilterPopup_L28_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D090); }
RW_FUNCTION(0x70BE8C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_28 __func::target", openAccessoriesFilterPopup_L28_target);
Addr openAccessoriesFilterPopup_L28_targetType() { return 0xFA3830; }
RW_FUNCTION(0x70BEAC, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_28 __func::target_type", openAccessoriesFilterPopup_L28_targetType);

// openAccessoriesFilterPopup()::$_27  (operator() at 0x70BF20)
void openAccessoriesFilterPopup_L27_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BEC0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_27 __func::~__func (deleting)", openAccessoriesFilterPopup_L27_D0);
Addr openAccessoriesFilterPopup_L27_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L27, 8, 1, false); }
RW_FUNCTION(0x70BEC8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_27 __func::__clone()", openAccessoriesFilterPopup_L27_clone);
void openAccessoriesFilterPopup_L27_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L27, 1, false); }
RW_FUNCTION(0x70BEF4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_27 __func::__clone(__base*)", openAccessoriesFilterPopup_L27_cloneTo);
void openAccessoriesFilterPopup_L27_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70BF14, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_27 __func::destroy", openAccessoriesFilterPopup_L27_destroy);
void openAccessoriesFilterPopup_L27_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BF16, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_27 __func::destroy_deallocate", openAccessoriesFilterPopup_L27_destroyDealloc);
Addr openAccessoriesFilterPopup_L27_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D170); }
RW_FUNCTION(0x70BF40, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_27 __func::target", openAccessoriesFilterPopup_L27_target);
Addr openAccessoriesFilterPopup_L27_targetType() { return 0xFA3844; }
RW_FUNCTION(0x70BF60, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_27 __func::target_type", openAccessoriesFilterPopup_L27_targetType);

// openAccessoriesFilterPopup()::$_26  (operator() at 0x70BFD4)
void openAccessoriesFilterPopup_L26_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BF74, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_26 __func::~__func (deleting)", openAccessoriesFilterPopup_L26_D0);
Addr openAccessoriesFilterPopup_L26_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L26, 8, 1, false); }
RW_FUNCTION(0x70BF7C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_26 __func::__clone()", openAccessoriesFilterPopup_L26_clone);
void openAccessoriesFilterPopup_L26_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L26, 1, false); }
RW_FUNCTION(0x70BFA8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_26 __func::__clone(__base*)", openAccessoriesFilterPopup_L26_cloneTo);
void openAccessoriesFilterPopup_L26_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70BFC8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_26 __func::destroy", openAccessoriesFilterPopup_L26_destroy);
void openAccessoriesFilterPopup_L26_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70BFCA, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_26 __func::destroy_deallocate", openAccessoriesFilterPopup_L26_destroyDealloc);
Addr openAccessoriesFilterPopup_L26_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D250); }
RW_FUNCTION(0x70BFF4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_26 __func::target", openAccessoriesFilterPopup_L26_target);
Addr openAccessoriesFilterPopup_L26_targetType() { return 0xFA3858; }
RW_FUNCTION(0x70C014, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_26 __func::target_type", openAccessoriesFilterPopup_L26_targetType);

// openAccessoriesFilterPopup()::$_25  (operator() at 0x70C086)
void openAccessoriesFilterPopup_L25_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C028, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_25 __func::~__func (deleting)", openAccessoriesFilterPopup_L25_D0);
Addr openAccessoriesFilterPopup_L25_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L25, 8, 1, false); }
RW_FUNCTION(0x70C030, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_25 __func::__clone()", openAccessoriesFilterPopup_L25_clone);
void openAccessoriesFilterPopup_L25_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L25, 1, false); }
RW_FUNCTION(0x70C05C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_25 __func::__clone(__base*)", openAccessoriesFilterPopup_L25_cloneTo);
void openAccessoriesFilterPopup_L25_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C07C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_25 __func::destroy", openAccessoriesFilterPopup_L25_destroy);
void openAccessoriesFilterPopup_L25_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C07E, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_25 __func::destroy_deallocate", openAccessoriesFilterPopup_L25_destroyDealloc);
Addr openAccessoriesFilterPopup_L25_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D330); }
RW_FUNCTION(0x70C0A4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_25 __func::target", openAccessoriesFilterPopup_L25_target);
Addr openAccessoriesFilterPopup_L25_targetType() { return 0xFA386C; }
RW_FUNCTION(0x70C0C4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_25 __func::target_type", openAccessoriesFilterPopup_L25_targetType);

// openAccessoriesFilterPopup()::$_24  (operator() at 0x70C138)
void openAccessoriesFilterPopup_L24_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C0D8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_24 __func::~__func (deleting)", openAccessoriesFilterPopup_L24_D0);
Addr openAccessoriesFilterPopup_L24_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L24, 8, 1, false); }
RW_FUNCTION(0x70C0E0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_24 __func::__clone()", openAccessoriesFilterPopup_L24_clone);
void openAccessoriesFilterPopup_L24_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L24, 1, false); }
RW_FUNCTION(0x70C10C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_24 __func::__clone(__base*)", openAccessoriesFilterPopup_L24_cloneTo);
void openAccessoriesFilterPopup_L24_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C12C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_24 __func::destroy", openAccessoriesFilterPopup_L24_destroy);
void openAccessoriesFilterPopup_L24_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C12E, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_24 __func::destroy_deallocate", openAccessoriesFilterPopup_L24_destroyDealloc);
Addr openAccessoriesFilterPopup_L24_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D410); }
RW_FUNCTION(0x70C158, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_24 __func::target", openAccessoriesFilterPopup_L24_target);
Addr openAccessoriesFilterPopup_L24_targetType() { return 0xFA3880; }
RW_FUNCTION(0x70C178, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_24 __func::target_type", openAccessoriesFilterPopup_L24_targetType);

// openAccessoriesFilterPopup()::$_23  (operator() at 0x70C1EC)
void openAccessoriesFilterPopup_L23_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C18C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_23 __func::~__func (deleting)", openAccessoriesFilterPopup_L23_D0);
Addr openAccessoriesFilterPopup_L23_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L23, 8, 1, false); }
RW_FUNCTION(0x70C194, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_23 __func::__clone()", openAccessoriesFilterPopup_L23_clone);
void openAccessoriesFilterPopup_L23_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L23, 1, false); }
RW_FUNCTION(0x70C1C0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_23 __func::__clone(__base*)", openAccessoriesFilterPopup_L23_cloneTo);
void openAccessoriesFilterPopup_L23_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C1E0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_23 __func::destroy", openAccessoriesFilterPopup_L23_destroy);
void openAccessoriesFilterPopup_L23_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C1E2, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_23 __func::destroy_deallocate", openAccessoriesFilterPopup_L23_destroyDealloc);
Addr openAccessoriesFilterPopup_L23_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D4F0); }
RW_FUNCTION(0x70C20C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_23 __func::target", openAccessoriesFilterPopup_L23_target);
Addr openAccessoriesFilterPopup_L23_targetType() { return 0xFA3894; }
RW_FUNCTION(0x70C22C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_23 __func::target_type", openAccessoriesFilterPopup_L23_targetType);

// openAccessoriesFilterPopup()::$_22  (operator() at 0x70C2A0)
void openAccessoriesFilterPopup_L22_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C240, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_22 __func::~__func (deleting)", openAccessoriesFilterPopup_L22_D0);
Addr openAccessoriesFilterPopup_L22_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L22, 8, 1, false); }
RW_FUNCTION(0x70C248, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_22 __func::__clone()", openAccessoriesFilterPopup_L22_clone);
void openAccessoriesFilterPopup_L22_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L22, 1, false); }
RW_FUNCTION(0x70C274, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_22 __func::__clone(__base*)", openAccessoriesFilterPopup_L22_cloneTo);
void openAccessoriesFilterPopup_L22_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C294, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_22 __func::destroy", openAccessoriesFilterPopup_L22_destroy);
void openAccessoriesFilterPopup_L22_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C296, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_22 __func::destroy_deallocate", openAccessoriesFilterPopup_L22_destroyDealloc);
Addr openAccessoriesFilterPopup_L22_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D5D0); }
RW_FUNCTION(0x70C2C0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_22 __func::target", openAccessoriesFilterPopup_L22_target);
Addr openAccessoriesFilterPopup_L22_targetType() { return 0xFA38A8; }
RW_FUNCTION(0x70C2E0, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_22 __func::target_type", openAccessoriesFilterPopup_L22_targetType);

// openAccessoriesFilterPopup()::$_21  (operator() at 0x70C352)
void openAccessoriesFilterPopup_L21_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C2F4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_21 __func::~__func (deleting)", openAccessoriesFilterPopup_L21_D0);
Addr openAccessoriesFilterPopup_L21_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L21, 8, 1, false); }
RW_FUNCTION(0x70C2FC, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_21 __func::__clone()", openAccessoriesFilterPopup_L21_clone);
void openAccessoriesFilterPopup_L21_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L21, 1, false); }
RW_FUNCTION(0x70C328, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_21 __func::__clone(__base*)", openAccessoriesFilterPopup_L21_cloneTo);
void openAccessoriesFilterPopup_L21_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C348, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_21 __func::destroy", openAccessoriesFilterPopup_L21_destroy);
void openAccessoriesFilterPopup_L21_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C34A, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_21 __func::destroy_deallocate", openAccessoriesFilterPopup_L21_destroyDealloc);
Addr openAccessoriesFilterPopup_L21_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D6B0); }
RW_FUNCTION(0x70C370, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_21 __func::target", openAccessoriesFilterPopup_L21_target);
Addr openAccessoriesFilterPopup_L21_targetType() { return 0xFA38BC; }
RW_FUNCTION(0x70C390, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_21 __func::target_type", openAccessoriesFilterPopup_L21_targetType);

// openAccessoriesFilterPopup()::$_20  (operator() at 0x70C404)
void openAccessoriesFilterPopup_L20_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C3A4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_20 __func::~__func (deleting)", openAccessoriesFilterPopup_L20_D0);
Addr openAccessoriesFilterPopup_L20_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L20, 8, 1, false); }
RW_FUNCTION(0x70C3AC, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_20 __func::__clone()", openAccessoriesFilterPopup_L20_clone);
void openAccessoriesFilterPopup_L20_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L20, 1, false); }
RW_FUNCTION(0x70C3D8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_20 __func::__clone(__base*)", openAccessoriesFilterPopup_L20_cloneTo);
void openAccessoriesFilterPopup_L20_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C3F8, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_20 __func::destroy", openAccessoriesFilterPopup_L20_destroy);
void openAccessoriesFilterPopup_L20_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C3FA, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_20 __func::destroy_deallocate", openAccessoriesFilterPopup_L20_destroyDealloc);
Addr openAccessoriesFilterPopup_L20_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D790); }
RW_FUNCTION(0x70C424, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_20 __func::target", openAccessoriesFilterPopup_L20_target);
Addr openAccessoriesFilterPopup_L20_targetType() { return 0xFA38D0; }
RW_FUNCTION(0x70C444, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_20 __func::target_type", openAccessoriesFilterPopup_L20_targetType);

// openAccessoriesFilterPopup()::$_19  (operator() at 0x70C4B8)
void openAccessoriesFilterPopup_L19_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C458, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_19 __func::~__func (deleting)", openAccessoriesFilterPopup_L19_D0);
Addr openAccessoriesFilterPopup_L19_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L19, 8, 1, false); }
RW_FUNCTION(0x70C460, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_19 __func::__clone()", openAccessoriesFilterPopup_L19_clone);
void openAccessoriesFilterPopup_L19_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L19, 1, false); }
RW_FUNCTION(0x70C48C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_19 __func::__clone(__base*)", openAccessoriesFilterPopup_L19_cloneTo);
void openAccessoriesFilterPopup_L19_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C4AC, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_19 __func::destroy", openAccessoriesFilterPopup_L19_destroy);
void openAccessoriesFilterPopup_L19_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C4AE, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_19 __func::destroy_deallocate", openAccessoriesFilterPopup_L19_destroyDealloc);
Addr openAccessoriesFilterPopup_L19_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D870); }
RW_FUNCTION(0x70C608, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_19 __func::target", openAccessoriesFilterPopup_L19_target);
Addr openAccessoriesFilterPopup_L19_targetType() { return 0xFA38E4; }
RW_FUNCTION(0x70C628, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_19 __func::target_type", openAccessoriesFilterPopup_L19_targetType);

// openAccessoriesFilterPopup()::$_18  (operator() at 0x70C68E)
void openAccessoriesFilterPopup_L18_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C63C, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_18 __func::~__func (deleting)", openAccessoriesFilterPopup_L18_D0);
Addr openAccessoriesFilterPopup_L18_clone(Addr self) { return thunk::clone(self, VT_openAccessoriesFilterPopup_L18, 8, 0, false); }
RW_FUNCTION(0x70C644, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_18 __func::__clone()", openAccessoriesFilterPopup_L18_clone);
void openAccessoriesFilterPopup_L18_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_openAccessoriesFilterPopup_L18, 0, false); }
RW_FUNCTION(0x70C668, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_18 __func::__clone(__base*)", openAccessoriesFilterPopup_L18_cloneTo);
void openAccessoriesFilterPopup_L18_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C684, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_18 __func::destroy", openAccessoriesFilterPopup_L18_destroy);
void openAccessoriesFilterPopup_L18_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C686, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_18 __func::destroy_deallocate", openAccessoriesFilterPopup_L18_destroyDealloc);
Addr openAccessoriesFilterPopup_L18_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8D950); }
RW_FUNCTION(0x70C6A4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_18 __func::target", openAccessoriesFilterPopup_L18_target);
Addr openAccessoriesFilterPopup_L18_targetType() { return 0xFA38F8; }
RW_FUNCTION(0x70C6C4, "SceneAvatarEdit::openAccessoriesFilterPopup()::$_18 __func::target_type", openAccessoriesFilterPopup_L18_targetType);

// initTabButton()::$_17  (operator() at 0x70C87C)
void initTabButton_L17_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C814, "SceneAvatarEdit::initTabButton()::$_17 __func::~__func (deleting)", initTabButton_L17_D0);
Addr initTabButton_L17_clone(Addr self) { return thunk::clone(self, VT_initTabButton_L17, 12, 2, false); }
RW_FUNCTION(0x70C81C, "SceneAvatarEdit::initTabButton()::$_17 __func::__clone()", initTabButton_L17_clone);
void initTabButton_L17_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_initTabButton_L17, 2, false); }
RW_FUNCTION(0x70C84C, "SceneAvatarEdit::initTabButton()::$_17 __func::__clone(__base*)", initTabButton_L17_cloneTo);
void initTabButton_L17_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C870, "SceneAvatarEdit::initTabButton()::$_17 __func::destroy", initTabButton_L17_destroy);
void initTabButton_L17_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C872, "SceneAvatarEdit::initTabButton()::$_17 __func::destroy_deallocate", initTabButton_L17_destroyDealloc);
Addr initTabButton_L17_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DA20); }
RW_FUNCTION(0x70C904, "SceneAvatarEdit::initTabButton()::$_17 __func::target", initTabButton_L17_target);
Addr initTabButton_L17_targetType() { return 0xFA390C; }
RW_FUNCTION(0x70C924, "SceneAvatarEdit::initTabButton()::$_17 __func::target_type", initTabButton_L17_targetType);

// initTabButton()::$_16  (operator() at 0x70C996)
void initTabButton_L16_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C938, "SceneAvatarEdit::initTabButton()::$_16 __func::~__func (deleting)", initTabButton_L16_D0);
Addr initTabButton_L16_clone(Addr self) { return thunk::clone(self, VT_initTabButton_L16, 8, 1, false); }
RW_FUNCTION(0x70C940, "SceneAvatarEdit::initTabButton()::$_16 __func::__clone()", initTabButton_L16_clone);
void initTabButton_L16_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_initTabButton_L16, 1, false); }
RW_FUNCTION(0x70C96C, "SceneAvatarEdit::initTabButton()::$_16 __func::__clone(__base*)", initTabButton_L16_cloneTo);
void initTabButton_L16_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70C98C, "SceneAvatarEdit::initTabButton()::$_16 __func::destroy", initTabButton_L16_destroy);
void initTabButton_L16_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C98E, "SceneAvatarEdit::initTabButton()::$_16 __func::destroy_deallocate", initTabButton_L16_destroyDealloc);
Addr initTabButton_L16_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DAE0); }
RW_FUNCTION(0x70C9A8, "SceneAvatarEdit::initTabButton()::$_16 __func::target", initTabButton_L16_target);
Addr initTabButton_L16_targetType() { return 0xFA3920; }
RW_FUNCTION(0x70C9C8, "SceneAvatarEdit::initTabButton()::$_16 __func::target_type", initTabButton_L16_targetType);

// initMyCordinateButton()::$_15  (operator() at 0x70CA44)
void initMyCordinateButton_L15_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70C9DC, "SceneAvatarEdit::initMyCordinateButton()::$_15 __func::~__func (deleting)", initMyCordinateButton_L15_D0);
Addr initMyCordinateButton_L15_clone(Addr self) { return thunk::clone(self, VT_initMyCordinateButton_L15, 12, 2, false); }
RW_FUNCTION(0x70C9E4, "SceneAvatarEdit::initMyCordinateButton()::$_15 __func::__clone()", initMyCordinateButton_L15_clone);
void initMyCordinateButton_L15_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_initMyCordinateButton_L15, 2, false); }
RW_FUNCTION(0x70CA14, "SceneAvatarEdit::initMyCordinateButton()::$_15 __func::__clone(__base*)", initMyCordinateButton_L15_cloneTo);
void initMyCordinateButton_L15_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70CA38, "SceneAvatarEdit::initMyCordinateButton()::$_15 __func::destroy", initMyCordinateButton_L15_destroy);
void initMyCordinateButton_L15_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70CA3A, "SceneAvatarEdit::initMyCordinateButton()::$_15 __func::destroy_deallocate", initMyCordinateButton_L15_destroyDealloc);
Addr initMyCordinateButton_L15_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DBB0); }
RW_FUNCTION(0x70CB60, "SceneAvatarEdit::initMyCordinateButton()::$_15 __func::target", initMyCordinateButton_L15_target);
Addr initMyCordinateButton_L15_targetType() { return 0xFA3934; }
RW_FUNCTION(0x70CB80, "SceneAvatarEdit::initMyCordinateButton()::$_15 __func::target_type", initMyCordinateButton_L15_targetType);

// runOutAnimationMain()::$_11  (operator() at 0x70D5FC)
Addr runOutAnimationMain_L11_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DC70); }
RW_FUNCTION(0x70D608, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::target", runOutAnimationMain_L11_target);
Addr runOutAnimationMain_L11_targetType() { return 0xFA3948; }
RW_FUNCTION(0x70D628, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::target_type", runOutAnimationMain_L11_targetType);

// init()::$_10  (operator() at 0x70D69C)
void init_L10_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70D63C, "SceneAvatarEdit::init()::$_10 __func::~__func (deleting)", init_L10_D0);
Addr init_L10_clone(Addr self) { return thunk::clone(self, VT_init_L10, 8, 1, false); }
RW_FUNCTION(0x70D644, "SceneAvatarEdit::init()::$_10 __func::__clone()", init_L10_clone);
void init_L10_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L10, 1, false); }
RW_FUNCTION(0x70D670, "SceneAvatarEdit::init()::$_10 __func::__clone(__base*)", init_L10_cloneTo);
void init_L10_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70D690, "SceneAvatarEdit::init()::$_10 __func::destroy", init_L10_destroy);
void init_L10_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70D692, "SceneAvatarEdit::init()::$_10 __func::destroy_deallocate", init_L10_destroyDealloc);
Addr init_L10_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DD30); }
RW_FUNCTION(0x70D784, "SceneAvatarEdit::init()::$_10 __func::target", init_L10_target);
Addr init_L10_targetType() { return 0xFA395C; }
RW_FUNCTION(0x70D7A4, "SceneAvatarEdit::init()::$_10 __func::target_type", init_L10_targetType);

// init()::$_9  (operator() at 0x70D818)
void init_L9_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70D7B8, "SceneAvatarEdit::init()::$_9 __func::~__func (deleting)", init_L9_D0);
Addr init_L9_clone(Addr self) { return thunk::clone(self, VT_init_L9, 8, 1, false); }
RW_FUNCTION(0x70D7C0, "SceneAvatarEdit::init()::$_9 __func::__clone()", init_L9_clone);
void init_L9_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L9, 1, false); }
RW_FUNCTION(0x70D7EC, "SceneAvatarEdit::init()::$_9 __func::__clone(__base*)", init_L9_cloneTo);
void init_L9_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70D80C, "SceneAvatarEdit::init()::$_9 __func::destroy", init_L9_destroy);
void init_L9_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70D80E, "SceneAvatarEdit::init()::$_9 __func::destroy_deallocate", init_L9_destroyDealloc);
Addr init_L9_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DDF0); }
RW_FUNCTION(0x70D8F4, "SceneAvatarEdit::init()::$_9 __func::target", init_L9_target);
Addr init_L9_targetType() { return 0xFA3970; }
RW_FUNCTION(0x70D914, "SceneAvatarEdit::init()::$_9 __func::target_type", init_L9_targetType);

// init()::$_8  (operator() at 0x70D988)
void init_L8_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70D928, "SceneAvatarEdit::init()::$_8 __func::~__func (deleting)", init_L8_D0);
Addr init_L8_clone(Addr self) { return thunk::clone(self, VT_init_L8, 8, 1, false); }
RW_FUNCTION(0x70D930, "SceneAvatarEdit::init()::$_8 __func::__clone()", init_L8_clone);
void init_L8_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L8, 1, false); }
RW_FUNCTION(0x70D95C, "SceneAvatarEdit::init()::$_8 __func::__clone(__base*)", init_L8_cloneTo);
void init_L8_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70D97C, "SceneAvatarEdit::init()::$_8 __func::destroy", init_L8_destroy);
void init_L8_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70D97E, "SceneAvatarEdit::init()::$_8 __func::destroy_deallocate", init_L8_destroyDealloc);
Addr init_L8_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DE70); }
RW_FUNCTION(0x70DA50, "SceneAvatarEdit::init()::$_8 __func::target", init_L8_target);
Addr init_L8_targetType() { return 0xFA3984; }
RW_FUNCTION(0x70DA70, "SceneAvatarEdit::init()::$_8 __func::target_type", init_L8_targetType);

// init()::$_7  (operator() at 0x70DAE4)
void init_L7_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70DA84, "SceneAvatarEdit::init()::$_7 __func::~__func (deleting)", init_L7_D0);
Addr init_L7_clone(Addr self) { return thunk::clone(self, VT_init_L7, 8, 1, false); }
RW_FUNCTION(0x70DA8C, "SceneAvatarEdit::init()::$_7 __func::__clone()", init_L7_clone);
void init_L7_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L7, 1, false); }
RW_FUNCTION(0x70DAB8, "SceneAvatarEdit::init()::$_7 __func::__clone(__base*)", init_L7_cloneTo);
void init_L7_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70DAD8, "SceneAvatarEdit::init()::$_7 __func::destroy", init_L7_destroy);
void init_L7_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70DADA, "SceneAvatarEdit::init()::$_7 __func::destroy_deallocate", init_L7_destroyDealloc);
Addr init_L7_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DEF0); }
RW_FUNCTION(0x70DC90, "SceneAvatarEdit::init()::$_7 __func::target", init_L7_target);
Addr init_L7_targetType() { return 0xFA3998; }
RW_FUNCTION(0x70DCB0, "SceneAvatarEdit::init()::$_7 __func::target_type", init_L7_targetType);

// init()::$_6  (operator() at 0x70DEC8)
void init_L6_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70DE68, "SceneAvatarEdit::init()::$_6 __func::~__func (deleting)", init_L6_D0);
Addr init_L6_clone(Addr self) { return thunk::clone(self, VT_init_L6, 8, 1, false); }
RW_FUNCTION(0x70DE70, "SceneAvatarEdit::init()::$_6 __func::__clone()", init_L6_clone);
void init_L6_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L6, 1, false); }
RW_FUNCTION(0x70DE9C, "SceneAvatarEdit::init()::$_6 __func::__clone(__base*)", init_L6_cloneTo);
void init_L6_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70DEBC, "SceneAvatarEdit::init()::$_6 __func::destroy", init_L6_destroy);
void init_L6_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70DEBE, "SceneAvatarEdit::init()::$_6 __func::destroy_deallocate", init_L6_destroyDealloc);
Addr init_L6_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8DFA0); }
RW_FUNCTION(0x70DF00, "SceneAvatarEdit::init()::$_6 __func::target", init_L6_target);
Addr init_L6_targetType() { return 0xFA39AC; }
RW_FUNCTION(0x70DF20, "SceneAvatarEdit::init()::$_6 __func::target_type", init_L6_targetType);

// init()::$_5  (operator() at 0x70DF94)
void init_L5_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70DF34, "SceneAvatarEdit::init()::$_5 __func::~__func (deleting)", init_L5_D0);
Addr init_L5_clone(Addr self) { return thunk::clone(self, VT_init_L5, 8, 1, false); }
RW_FUNCTION(0x70DF3C, "SceneAvatarEdit::init()::$_5 __func::__clone()", init_L5_clone);
void init_L5_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L5, 1, false); }
RW_FUNCTION(0x70DF68, "SceneAvatarEdit::init()::$_5 __func::__clone(__base*)", init_L5_cloneTo);
void init_L5_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70DF88, "SceneAvatarEdit::init()::$_5 __func::destroy", init_L5_destroy);
void init_L5_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70DF8A, "SceneAvatarEdit::init()::$_5 __func::destroy_deallocate", init_L5_destroyDealloc);
Addr init_L5_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8E050); }
RW_FUNCTION(0x70DFCC, "SceneAvatarEdit::init()::$_5 __func::target", init_L5_target);
Addr init_L5_targetType() { return 0xFA39C0; }
RW_FUNCTION(0x70DFEC, "SceneAvatarEdit::init()::$_5 __func::target_type", init_L5_targetType);

// init()::$_4  (operator() at 0x70E060)
void init_L4_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E000, "SceneAvatarEdit::init()::$_4 __func::~__func (deleting)", init_L4_D0);
Addr init_L4_clone(Addr self) { return thunk::clone(self, VT_init_L4, 8, 1, false); }
RW_FUNCTION(0x70E008, "SceneAvatarEdit::init()::$_4 __func::__clone()", init_L4_clone);
void init_L4_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L4, 1, false); }
RW_FUNCTION(0x70E034, "SceneAvatarEdit::init()::$_4 __func::__clone(__base*)", init_L4_cloneTo);
void init_L4_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70E054, "SceneAvatarEdit::init()::$_4 __func::destroy", init_L4_destroy);
void init_L4_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E056, "SceneAvatarEdit::init()::$_4 __func::destroy_deallocate", init_L4_destroyDealloc);
Addr init_L4_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8E100); }
RW_FUNCTION(0x70E11C, "SceneAvatarEdit::init()::$_4 __func::target", init_L4_target);
Addr init_L4_targetType() { return 0xFA39D4; }
RW_FUNCTION(0x70E13C, "SceneAvatarEdit::init()::$_4 __func::target_type", init_L4_targetType);

// init()::$_4::lambda(bool)  (operator() at 0x70E1AE)
void init_L4_fnb_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E150, "SceneAvatarEdit::init()::$_4::lambda(bool) __func::~__func (deleting)", init_L4_fnb_D0);
Addr init_L4_fnb_clone(Addr self) { return thunk::clone(self, VT_init_L4_fnb, 8, 1, false); }
RW_FUNCTION(0x70E158, "SceneAvatarEdit::init()::$_4::lambda(bool) __func::__clone()", init_L4_fnb_clone);
void init_L4_fnb_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L4_fnb, 1, false); }
RW_FUNCTION(0x70E184, "SceneAvatarEdit::init()::$_4::lambda(bool) __func::__clone(__base*)", init_L4_fnb_cloneTo);
void init_L4_fnb_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70E1A4, "SceneAvatarEdit::init()::$_4::lambda(bool) __func::destroy", init_L4_fnb_destroy);
void init_L4_fnb_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E1A6, "SceneAvatarEdit::init()::$_4::lambda(bool) __func::destroy_deallocate", init_L4_fnb_destroyDealloc);
Addr init_L4_fnb_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8E1B0); }
RW_FUNCTION(0x70E1C0, "SceneAvatarEdit::init()::$_4::lambda(bool) __func::target", init_L4_fnb_target);
Addr init_L4_fnb_targetType() { return 0xFA39E8; }
RW_FUNCTION(0x70E1E0, "SceneAvatarEdit::init()::$_4::lambda(bool) __func::target_type", init_L4_fnb_targetType);

// init()::$_3  (operator() at 0x70E254)
void init_L3_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E1F4, "SceneAvatarEdit::init()::$_3 __func::~__func (deleting)", init_L3_D0);
Addr init_L3_clone(Addr self) { return thunk::clone(self, VT_init_L3, 8, 1, false); }
RW_FUNCTION(0x70E1FC, "SceneAvatarEdit::init()::$_3 __func::__clone()", init_L3_clone);
void init_L3_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L3, 1, false); }
RW_FUNCTION(0x70E228, "SceneAvatarEdit::init()::$_3 __func::__clone(__base*)", init_L3_cloneTo);
void init_L3_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70E248, "SceneAvatarEdit::init()::$_3 __func::destroy", init_L3_destroy);
void init_L3_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E24A, "SceneAvatarEdit::init()::$_3 __func::destroy_deallocate", init_L3_destroyDealloc);
Addr init_L3_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8E2A0); }
RW_FUNCTION(0x70E280, "SceneAvatarEdit::init()::$_3 __func::target", init_L3_target);
Addr init_L3_targetType() { return 0xFA39FC; }
RW_FUNCTION(0x70E2A0, "SceneAvatarEdit::init()::$_3 __func::target_type", init_L3_targetType);

// init()::$_2  (operator() at 0x70E312)
void init_L2_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E2B4, "SceneAvatarEdit::init()::$_2 __func::~__func (deleting)", init_L2_D0);
Addr init_L2_clone(Addr self) { return thunk::clone(self, VT_init_L2, 8, 1, false); }
RW_FUNCTION(0x70E2BC, "SceneAvatarEdit::init()::$_2 __func::__clone()", init_L2_clone);
void init_L2_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L2, 1, false); }
RW_FUNCTION(0x70E2E8, "SceneAvatarEdit::init()::$_2 __func::__clone(__base*)", init_L2_cloneTo);
void init_L2_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70E308, "SceneAvatarEdit::init()::$_2 __func::destroy", init_L2_destroy);
void init_L2_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E30A, "SceneAvatarEdit::init()::$_2 __func::destroy_deallocate", init_L2_destroyDealloc);
Addr init_L2_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8E320); }
RW_FUNCTION(0x70E31C, "SceneAvatarEdit::init()::$_2 __func::target", init_L2_target);
Addr init_L2_targetType() { return 0xFA3A10; }
RW_FUNCTION(0x70E33C, "SceneAvatarEdit::init()::$_2 __func::target_type", init_L2_targetType);

// init()::$_1  (operator() at 0x70E3AE)
void init_L1_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E350, "SceneAvatarEdit::init()::$_1 __func::~__func (deleting)", init_L1_D0);
Addr init_L1_clone(Addr self) { return thunk::clone(self, VT_init_L1, 8, 1, false); }
RW_FUNCTION(0x70E358, "SceneAvatarEdit::init()::$_1 __func::__clone()", init_L1_clone);
void init_L1_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L1, 1, false); }
RW_FUNCTION(0x70E384, "SceneAvatarEdit::init()::$_1 __func::__clone(__base*)", init_L1_cloneTo);
void init_L1_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70E3A4, "SceneAvatarEdit::init()::$_1 __func::destroy", init_L1_destroy);
void init_L1_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E3A6, "SceneAvatarEdit::init()::$_1 __func::destroy_deallocate", init_L1_destroyDealloc);
Addr init_L1_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8E3A0); }
RW_FUNCTION(0x70E3B8, "SceneAvatarEdit::init()::$_1 __func::target", init_L1_target);
Addr init_L1_targetType() { return 0xFA3A24; }
RW_FUNCTION(0x70E3D8, "SceneAvatarEdit::init()::$_1 __func::target_type", init_L1_targetType);

// init()::$_0  (operator() at 0x70E44A)
void init_L0_D0(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E3EC, "SceneAvatarEdit::init()::$_0 __func::~__func (deleting)", init_L0_D0);
Addr init_L0_clone(Addr self) { return thunk::clone(self, VT_init_L0, 8, 1, false); }
RW_FUNCTION(0x70E3F4, "SceneAvatarEdit::init()::$_0 __func::__clone()", init_L0_clone);
void init_L0_cloneTo(Addr self, Addr dst) { thunk::cloneTo(self, dst, VT_init_L0, 1, false); }
RW_FUNCTION(0x70E420, "SceneAvatarEdit::init()::$_0 __func::__clone(__base*)", init_L0_cloneTo);
void init_L0_destroy(Addr self) { (void)self; }
RW_FUNCTION(0x70E440, "SceneAvatarEdit::init()::$_0 __func::destroy", init_L0_destroy);
void init_L0_destroyDealloc(Addr self) { thunk::deleteSelf(self); }
RW_FUNCTION(0x70E442, "SceneAvatarEdit::init()::$_0 __func::destroy_deallocate", init_L0_destroyDealloc);
Addr init_L0_target(Addr self, Addr type) { return thunk::target(self, type, 0xE8E420); }
RW_FUNCTION(0x70E454, "SceneAvatarEdit::init()::$_0 __func::target", init_L0_target);
Addr init_L0_targetType() { return 0xFA3A38; }
RW_FUNCTION(0x70E474, "SceneAvatarEdit::init()::$_0 __func::target_type", init_L0_targetType);

// ---- lambdas capturing a std::function at +8 (heap __func of 0x20 bytes):
// saveMyCoordinate()::$_39 (std::function<void(bool)>) and runOutAnimationMain()::$_11 (std::function<void()>)
namespace fnwrap {
constexpr Addr function_vb_copy = 0x45C84D;   // std::function<void(bool)>::function(const function&)
constexpr Addr function_vv_copy = 0x4683CD;   // std::function<void()>::function(const function&)
constexpr Addr function_vv_call = 0x470E69;   // std::function<void()>::operator()
// ~function of the captured std::function at self+8
inline void destroyCaptured(Addr self) {
    Addr f = ld32(self + 0x18);
    if (f == self + 8) vcall(f, 0x10, {f});
    else if (f) vcall(f, 0x14, {f});
}
inline Addr dtor(Addr self, Addr vtable) {
    st32(self, vtable);
    destroyCaptured(self);
    return self;
}
inline Addr clone(Addr self, Addr vtable, Addr copyCtor) {
    Addr p = call(fn::operator_new, {0x20});
    if (p) {
        st32(p, vtable);
        call(copyCtor, {p + 8, self + 8});
    }
    return p;
}
inline void cloneTo(Addr self, Addr dst, Addr vtable, Addr copyCtor) {
    if (!dst) return;
    st32(dst, vtable);
    call(copyCtor, {dst + 8, self + 8});
}
}  // namespace fnwrap

Addr saveMyCoordinate_L39_D1(Addr self) { return fnwrap::dtor(self, VT_saveMyCoordinate_L39); }
RW_FUNCTION(0x70A9A4, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::~__func", saveMyCoordinate_L39_D1);
void saveMyCoordinate_L39_D0(Addr self) { thunk::deleteSelf(fnwrap::dtor(self, VT_saveMyCoordinate_L39)); }
RW_FUNCTION(0x70A9E0, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::~__func (deleting)", saveMyCoordinate_L39_D0);
Addr saveMyCoordinate_L39_clone(Addr self) {
    return fnwrap::clone(self, VT_saveMyCoordinate_L39, fnwrap::function_vb_copy);
}
RW_FUNCTION(0x70AA20, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::__clone()", saveMyCoordinate_L39_clone);
void saveMyCoordinate_L39_cloneTo(Addr self, Addr dst) {
    fnwrap::cloneTo(self, dst, VT_saveMyCoordinate_L39, fnwrap::function_vb_copy);
}
RW_FUNCTION(0x70AA70, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::__clone(__base*)", saveMyCoordinate_L39_cloneTo);
void saveMyCoordinate_L39_destroy(Addr self) { fnwrap::destroyCaptured(self); }
RW_FUNCTION(0x70AAA4, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::destroy", saveMyCoordinate_L39_destroy);
void saveMyCoordinate_L39_destroyDealloc(Addr self) {
    fnwrap::destroyCaptured(self);
    thunk::deleteSelf(self);
}
RW_FUNCTION(0x70AAC4, "SceneAvatarEdit::saveMyCoordinate()::$_39 __func::destroy_deallocate",
            saveMyCoordinate_L39_destroyDealloc);

Addr runOutAnimationMain_L11_D1(Addr self) { return fnwrap::dtor(self, VT_runOutAnimationMain_L11); }
RW_FUNCTION(0x70D4B0, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::~__func", runOutAnimationMain_L11_D1);
void runOutAnimationMain_L11_D0(Addr self) { thunk::deleteSelf(fnwrap::dtor(self, VT_runOutAnimationMain_L11)); }
RW_FUNCTION(0x70D4EC, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::~__func (deleting)",
            runOutAnimationMain_L11_D0);
Addr runOutAnimationMain_L11_clone(Addr self) {
    return fnwrap::clone(self, VT_runOutAnimationMain_L11, fnwrap::function_vv_copy);
}
RW_FUNCTION(0x70D52C, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::__clone()", runOutAnimationMain_L11_clone);
void runOutAnimationMain_L11_cloneTo(Addr self, Addr dst) {
    fnwrap::cloneTo(self, dst, VT_runOutAnimationMain_L11, fnwrap::function_vv_copy);
}
RW_FUNCTION(0x70D57C, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::__clone(__base*)",
            runOutAnimationMain_L11_cloneTo);
void runOutAnimationMain_L11_destroy(Addr self) { fnwrap::destroyCaptured(self); }
RW_FUNCTION(0x70D5B0, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::destroy", runOutAnimationMain_L11_destroy);
void runOutAnimationMain_L11_destroyDealloc(Addr self) {
    fnwrap::destroyCaptured(self);
    thunk::deleteSelf(self);
}
RW_FUNCTION(0x70D5D0, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::destroy_deallocate",
            runOutAnimationMain_L11_destroyDealloc);
// operator(): run the captured callback
void runOutAnimationMain_L11_call(Addr self) { call(fnwrap::function_vv_call, {self + 8}); }
RW_FUNCTION(0x70D5FC, "SceneAvatarEdit::runOutAnimationMain()::$_11 __func::operator()", runOutAnimationMain_L11_call);

}  // namespace
