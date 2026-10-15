// PopupRegister: the std::function plumbing of its lambdas (libc++ __func slots: deleting destructor, clone, clone into a
// buffer, destroy, destroy_deallocate, target, target_type), generated from the binary's __func vtables (size,
// captured bytes, type_info, type name and target offset read from each slot's instructions), plus the lambda bodies.
#include "FunctionThunks.hpp"

namespace PopupRegister_w3 {

// PopupRegister::openNameRegisterPopup::$_10::(inner)
using openNameRegisterPopup_10_inner = thunk::Lambda<0xF58EB8, 16, 0xF8A314, 0xE31FE0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// PopupRegister::openNameRegisterPopup::$_11::(inner)
using openNameRegisterPopup_11_inner_f58ee4 = thunk::Lambda<0xF58EE4, 8, 0xF8A328, 0xE32130, 4>;

}  // namespace PopupRegister_w3

RW_FUNCTION(0x566570, "PopupRegister::openNameRegisterPopup::$_10::(inner) ~__func() (deleting)", PopupRegister_w3::openNameRegisterPopup_10_inner::deleteThis);
RW_FUNCTION(0x566578, "PopupRegister::openNameRegisterPopup::$_10::(inner) __clone()", PopupRegister_w3::openNameRegisterPopup_10_inner::clone);
RW_FUNCTION(0x5665A8, "PopupRegister::openNameRegisterPopup::$_10::(inner) __clone(__base*)", PopupRegister_w3::openNameRegisterPopup_10_inner::cloneInto);
RW_FUNCTION(0x5665D0, "PopupRegister::openNameRegisterPopup::$_10::(inner) destroy()", PopupRegister_w3::openNameRegisterPopup_10_inner::destroy);
RW_FUNCTION(0x5665D2, "PopupRegister::openNameRegisterPopup::$_10::(inner) destroy_deallocate()", PopupRegister_w3::openNameRegisterPopup_10_inner::deleteThis);
RW_FUNCTION(0x566604, "PopupRegister::openNameRegisterPopup::$_10::(inner) target(const type_info&)", PopupRegister_w3::openNameRegisterPopup_10_inner::target);
RW_FUNCTION(0x566624, "PopupRegister::openNameRegisterPopup::$_10::(inner) target_type()", PopupRegister_w3::openNameRegisterPopup_10_inner::targetType);
RW_FUNCTION(0x566638, "PopupRegister::openNameRegisterPopup::$_11::(inner) ~__func() (deleting)", PopupRegister_w3::openNameRegisterPopup_11_inner_f58ee4::deleteThis);
RW_FUNCTION(0x566640, "PopupRegister::openNameRegisterPopup::$_11::(inner) __clone()", PopupRegister_w3::openNameRegisterPopup_11_inner_f58ee4::clone);
RW_FUNCTION(0x566664, "PopupRegister::openNameRegisterPopup::$_11::(inner) __clone(__base*)", PopupRegister_w3::openNameRegisterPopup_11_inner_f58ee4::cloneInto);
RW_FUNCTION(0x566680, "PopupRegister::openNameRegisterPopup::$_11::(inner) destroy()", PopupRegister_w3::openNameRegisterPopup_11_inner_f58ee4::destroy);
RW_FUNCTION(0x566682, "PopupRegister::openNameRegisterPopup::$_11::(inner) destroy_deallocate()", PopupRegister_w3::openNameRegisterPopup_11_inner_f58ee4::deleteThis);
RW_FUNCTION(0x56668C, "PopupRegister::openNameRegisterPopup::$_11::(inner) target(const type_info&)", PopupRegister_w3::openNameRegisterPopup_11_inner_f58ee4::target);
RW_FUNCTION(0x5666AC, "PopupRegister::openNameRegisterPopup::$_11::(inner) target_type()", PopupRegister_w3::openNameRegisterPopup_11_inner_f58ee4::targetType);
