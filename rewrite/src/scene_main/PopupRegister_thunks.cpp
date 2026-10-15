// PopupRegister: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace PopupRegister_lambdas {

// PopupRegister::openNameRegisterPopup::$_11 (captures a std::function at +40, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using openNameRegisterPopup_11 = thunk::LambdaFn<0xF58E60, 64, 0xF8A2EC, 0xE31D70, 8, 40, 0x4683CD, thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4)>;
// PopupRegister::openBirthRegisterPopup::$_9 (captures a std::function at +48, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using openBirthRegisterPopup_9 = thunk::LambdaFn<0xF58F10, 72, 0xF8A33C, 0xE32290, 8, 48, 0x4683CD, thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4), thunk::cap(40, 4)>;
// PopupRegister::openBirthRegisterPopup::$_8 (captures a std::function at +32, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using openBirthRegisterPopup_8 = thunk::LambdaFn<0xF58F3C, 56, 0xF8A350, 0xE32360, 8, 32, 0x4683CD, thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4)>;
// PopupRegister::openBirthRegisterPopup::$_7
using openBirthRegisterPopup_7 = thunk::Lambda<0xF58FC0, 8, 0xF8A38C, 0xE32640, 4>;
// PopupRegister::openBirthRegisterPopup::$_6
using openBirthRegisterPopup_6 = thunk::Lambda<0xF58FEC, 28, 0xF8A3A0, 0xE32740, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4)>;
// PopupRegister::openBirthRegisterPopup::$_5
using openBirthRegisterPopup_5 = thunk::Lambda<0xF59070, 32, 0xF8A3DC, 0xE32AB0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// PopupRegister::openBirthRegisterPopup::$_4
using openBirthRegisterPopup_4 = thunk::Lambda<0xF5909C, 32, 0xF8A3F0, 0xE32BB0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// PopupRegister::openBirthRegisterPopup::$_3
using openBirthRegisterPopup_3 = thunk::Lambda<0xF59120, 8, 0xF8A42C, 0xE32F50, 4>;
// PopupRegister::openBirthRegisterPopup::$_2
using openBirthRegisterPopup_2 = thunk::Lambda<0xF5914C, 32, 0xF8A440, 0xE33020, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// PopupRegister::openBirthRegisterPopup::$_1
using openBirthRegisterPopup_1 = thunk::Lambda<0xF59178, 32, 0xF8A454, 0xE33120, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// PopupRegister::openBirthRegisterPopup::$_0
using openBirthRegisterPopup_0 = thunk::Lambda<0xF591FC, 8, 0xF8A490, 0xE334C0, 4>;

}  // namespace PopupRegister_lambdas

RW_FUNCTION(0x565CBC, "PopupRegister::openNameRegisterPopup::$_11 ~__func()", PopupRegister_lambdas::openNameRegisterPopup_11::destructor);
RW_FUNCTION(0x565CF8, "PopupRegister::openNameRegisterPopup::$_11 ~__func() (deleting)", PopupRegister_lambdas::openNameRegisterPopup_11::destructorDelete);
RW_FUNCTION(0x565D38, "PopupRegister::openNameRegisterPopup::$_11 __clone()", PopupRegister_lambdas::openNameRegisterPopup_11::clone);
RW_FUNCTION(0x565DA0, "PopupRegister::openNameRegisterPopup::$_11 __clone(__base*)", PopupRegister_lambdas::openNameRegisterPopup_11::cloneInto);
RW_FUNCTION(0x565DEC, "PopupRegister::openNameRegisterPopup::$_11 destroy()", PopupRegister_lambdas::openNameRegisterPopup_11::destroy);
RW_FUNCTION(0x565E0C, "PopupRegister::openNameRegisterPopup::$_11 destroy_deallocate()", PopupRegister_lambdas::openNameRegisterPopup_11::destroyDeallocate);
RW_FUNCTION(0x566218, "PopupRegister::openNameRegisterPopup::$_11 target(const type_info&)", PopupRegister_lambdas::openNameRegisterPopup_11::target);
RW_FUNCTION(0x56623C, "PopupRegister::openNameRegisterPopup::$_11 target_type()", PopupRegister_lambdas::openNameRegisterPopup_11::targetType);
RW_FUNCTION(0x5666C0, "PopupRegister::openBirthRegisterPopup::$_9 ~__func()", PopupRegister_lambdas::openBirthRegisterPopup_9::destructor);
RW_FUNCTION(0x5666FC, "PopupRegister::openBirthRegisterPopup::$_9 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_9::destructorDelete);
RW_FUNCTION(0x56673C, "PopupRegister::openBirthRegisterPopup::$_9 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_9::clone);
RW_FUNCTION(0x5667AC, "PopupRegister::openBirthRegisterPopup::$_9 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_9::cloneInto);
RW_FUNCTION(0x5667FC, "PopupRegister::openBirthRegisterPopup::$_9 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_9::destroy);
RW_FUNCTION(0x56681C, "PopupRegister::openBirthRegisterPopup::$_9 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_9::destroyDeallocate);
RW_FUNCTION(0x5669CC, "PopupRegister::openBirthRegisterPopup::$_9 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_9::target);
RW_FUNCTION(0x5669F0, "PopupRegister::openBirthRegisterPopup::$_9 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_9::targetType);
RW_FUNCTION(0x566A04, "PopupRegister::openBirthRegisterPopup::$_8 ~__func()", PopupRegister_lambdas::openBirthRegisterPopup_8::destructor);
RW_FUNCTION(0x566A40, "PopupRegister::openBirthRegisterPopup::$_8 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_8::destructorDelete);
RW_FUNCTION(0x566A80, "PopupRegister::openBirthRegisterPopup::$_8 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_8::clone);
RW_FUNCTION(0x566AE0, "PopupRegister::openBirthRegisterPopup::$_8 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_8::cloneInto);
RW_FUNCTION(0x566B24, "PopupRegister::openBirthRegisterPopup::$_8 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_8::destroy);
RW_FUNCTION(0x566B44, "PopupRegister::openBirthRegisterPopup::$_8 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_8::destroyDeallocate);
RW_FUNCTION(0x566D78, "PopupRegister::openBirthRegisterPopup::$_8 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_8::target);
RW_FUNCTION(0x566D9C, "PopupRegister::openBirthRegisterPopup::$_8 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_8::targetType);
RW_FUNCTION(0x567010, "PopupRegister::openBirthRegisterPopup::$_7 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_7::deleteThis);
RW_FUNCTION(0x567018, "PopupRegister::openBirthRegisterPopup::$_7 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_7::clone);
RW_FUNCTION(0x56703C, "PopupRegister::openBirthRegisterPopup::$_7 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_7::cloneInto);
RW_FUNCTION(0x567058, "PopupRegister::openBirthRegisterPopup::$_7 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_7::destroy);
RW_FUNCTION(0x56705A, "PopupRegister::openBirthRegisterPopup::$_7 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_7::deleteThis);
RW_FUNCTION(0x567078, "PopupRegister::openBirthRegisterPopup::$_7 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_7::target);
RW_FUNCTION(0x567098, "PopupRegister::openBirthRegisterPopup::$_7 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_7::targetType);
RW_FUNCTION(0x5670AC, "PopupRegister::openBirthRegisterPopup::$_6 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_6::deleteThis);
RW_FUNCTION(0x5670B4, "PopupRegister::openBirthRegisterPopup::$_6 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_6::clone);
RW_FUNCTION(0x5670EC, "PopupRegister::openBirthRegisterPopup::$_6 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_6::cloneInto);
RW_FUNCTION(0x567118, "PopupRegister::openBirthRegisterPopup::$_6 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_6::destroy);
RW_FUNCTION(0x56711A, "PopupRegister::openBirthRegisterPopup::$_6 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_6::deleteThis);
RW_FUNCTION(0x5672E0, "PopupRegister::openBirthRegisterPopup::$_6 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_6::target);
RW_FUNCTION(0x567300, "PopupRegister::openBirthRegisterPopup::$_6 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_6::targetType);
RW_FUNCTION(0x567474, "PopupRegister::openBirthRegisterPopup::$_5 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_5::deleteThis);
RW_FUNCTION(0x56747C, "PopupRegister::openBirthRegisterPopup::$_5 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_5::clone);
RW_FUNCTION(0x5674B8, "PopupRegister::openBirthRegisterPopup::$_5 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_5::cloneInto);
RW_FUNCTION(0x5674E8, "PopupRegister::openBirthRegisterPopup::$_5 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_5::destroy);
RW_FUNCTION(0x5674EA, "PopupRegister::openBirthRegisterPopup::$_5 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_5::deleteThis);
RW_FUNCTION(0x567644, "PopupRegister::openBirthRegisterPopup::$_5 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_5::target);
RW_FUNCTION(0x567664, "PopupRegister::openBirthRegisterPopup::$_5 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_5::targetType);
RW_FUNCTION(0x567678, "PopupRegister::openBirthRegisterPopup::$_4 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_4::deleteThis);
RW_FUNCTION(0x567680, "PopupRegister::openBirthRegisterPopup::$_4 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_4::clone);
RW_FUNCTION(0x5676BC, "PopupRegister::openBirthRegisterPopup::$_4 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_4::cloneInto);
RW_FUNCTION(0x5676EC, "PopupRegister::openBirthRegisterPopup::$_4 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_4::destroy);
RW_FUNCTION(0x5676EE, "PopupRegister::openBirthRegisterPopup::$_4 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_4::deleteThis);
RW_FUNCTION(0x567894, "PopupRegister::openBirthRegisterPopup::$_4 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_4::target);
RW_FUNCTION(0x5678B4, "PopupRegister::openBirthRegisterPopup::$_4 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_4::targetType);
RW_FUNCTION(0x567A70, "PopupRegister::openBirthRegisterPopup::$_3 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_3::deleteThis);
RW_FUNCTION(0x567A78, "PopupRegister::openBirthRegisterPopup::$_3 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_3::clone);
RW_FUNCTION(0x567A9C, "PopupRegister::openBirthRegisterPopup::$_3 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_3::cloneInto);
RW_FUNCTION(0x567AB8, "PopupRegister::openBirthRegisterPopup::$_3 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_3::destroy);
RW_FUNCTION(0x567ABA, "PopupRegister::openBirthRegisterPopup::$_3 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_3::deleteThis);
RW_FUNCTION(0x567AD8, "PopupRegister::openBirthRegisterPopup::$_3 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_3::target);
RW_FUNCTION(0x567AF8, "PopupRegister::openBirthRegisterPopup::$_3 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_3::targetType);
RW_FUNCTION(0x567B0C, "PopupRegister::openBirthRegisterPopup::$_2 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_2::deleteThis);
RW_FUNCTION(0x567B14, "PopupRegister::openBirthRegisterPopup::$_2 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_2::clone);
RW_FUNCTION(0x567B50, "PopupRegister::openBirthRegisterPopup::$_2 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_2::cloneInto);
RW_FUNCTION(0x567B80, "PopupRegister::openBirthRegisterPopup::$_2 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_2::destroy);
RW_FUNCTION(0x567B82, "PopupRegister::openBirthRegisterPopup::$_2 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_2::deleteThis);
RW_FUNCTION(0x567CDC, "PopupRegister::openBirthRegisterPopup::$_2 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_2::target);
RW_FUNCTION(0x567CFC, "PopupRegister::openBirthRegisterPopup::$_2 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_2::targetType);
RW_FUNCTION(0x567D10, "PopupRegister::openBirthRegisterPopup::$_1 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_1::deleteThis);
RW_FUNCTION(0x567D18, "PopupRegister::openBirthRegisterPopup::$_1 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_1::clone);
RW_FUNCTION(0x567D54, "PopupRegister::openBirthRegisterPopup::$_1 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_1::cloneInto);
RW_FUNCTION(0x567D84, "PopupRegister::openBirthRegisterPopup::$_1 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_1::destroy);
RW_FUNCTION(0x567D86, "PopupRegister::openBirthRegisterPopup::$_1 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_1::deleteThis);
RW_FUNCTION(0x567F2C, "PopupRegister::openBirthRegisterPopup::$_1 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_1::target);
RW_FUNCTION(0x567F4C, "PopupRegister::openBirthRegisterPopup::$_1 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_1::targetType);
RW_FUNCTION(0x568108, "PopupRegister::openBirthRegisterPopup::$_0 ~__func() (deleting)", PopupRegister_lambdas::openBirthRegisterPopup_0::deleteThis);
RW_FUNCTION(0x568110, "PopupRegister::openBirthRegisterPopup::$_0 __clone()", PopupRegister_lambdas::openBirthRegisterPopup_0::clone);
RW_FUNCTION(0x568134, "PopupRegister::openBirthRegisterPopup::$_0 __clone(__base*)", PopupRegister_lambdas::openBirthRegisterPopup_0::cloneInto);
RW_FUNCTION(0x568150, "PopupRegister::openBirthRegisterPopup::$_0 destroy()", PopupRegister_lambdas::openBirthRegisterPopup_0::destroy);
RW_FUNCTION(0x568152, "PopupRegister::openBirthRegisterPopup::$_0 destroy_deallocate()", PopupRegister_lambdas::openBirthRegisterPopup_0::deleteThis);
RW_FUNCTION(0x568170, "PopupRegister::openBirthRegisterPopup::$_0 target(const type_info&)", PopupRegister_lambdas::openBirthRegisterPopup_0::target);
RW_FUNCTION(0x568190, "PopupRegister::openBirthRegisterPopup::$_0 target_type()", PopupRegister_lambdas::openBirthRegisterPopup_0::targetType);
