// SceneBattleResult: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "../scene_main/FunctionThunks.hpp"

namespace SceneBattleResult_g5thunks {

// SceneBattleResult::openGetKeyBladePopup
using openGetKeyBladePopup = thunk::Lambda<0xF64474, 8, 0xF9C338, 0xE6F700, 4, thunk::cap(4, 4)>;
// SceneBattleResult::openGetKeyBladePopup (captures a std::function at +24, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using openGetKeyBladePopup_ = thunk::LambdaFn<0xF644A0, 48, 0xF9C34C, 0xE6F870, 8, 24, 0x4683CD, thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;
// SceneBattleResult::openGetKeyBladePopup (captures a std::function at +24, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using openGetKeyBladePopup__ = thunk::LambdaFn<0xF644CC, 48, 0xF9C360, 0xE6F9F0, 8, 24, 0x4683CD, thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;
// SceneBattleResult::startResult::$_17
using startResult_17 = thunk::Lambda<0xF644F8, 8, 0xF9C374, 0xE6FB20, 4, thunk::cap(4, 4)>;
// SceneBattleResult::startResult::$_16
using startResult_16 = thunk::Lambda<0xF64524, 8, 0xF9C388, 0xE6FBB0, 4, thunk::cap(4, 4)>;
// SceneBattleResult::startResult::$_15
using startResult_15 = thunk::Lambda<0xF64550, 8, 0xF9C39C, 0xE6FC40, 4, thunk::cap(4, 4)>;
// SceneBattleResult::openNewKeyBladePopup::$_14
using openNewKeyBladePopup_14 = thunk::Lambda<0xF6457C, 8, 0xF9C3B0, 0xE6FCE0, 4, thunk::cap(4, 4)>;
// SceneBattleResult::openUserTitlePopup::$_13
using openUserTitlePopup_13 = thunk::Lambda<0xF645A8, 8, 0xF9C3C4, 0xE6FD90, 4, thunk::cap(4, 4)>;
// SceneBattleResult::openNewLuxPopup::$_12
using openNewLuxPopup_12 = thunk::Lambda<0xF645D4, 8, 0xF9C3D8, 0xE6FE40, 4, thunk::cap(4, 4)>;
// SceneBattleResult::openLevelUpPopup::$_11
using openLevelUpPopup_11 = thunk::Lambda<0xF64600, 12, 0xF9C3EC, 0xE6FEE0, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneBattleResult::openLevelUpPopup::$_10
using openLevelUpPopup_10 = thunk::Lambda<0xF6462C, 20, 0xF9C400, 0xE6FFB0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;
// SceneBattleResult::openLevelUpPopup::$_10::lambda()
using openLevelUpPopup_10_lv = thunk::Lambda<0xF64658, 20, 0xF9C414, 0xE70080, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;
// SceneBattleResult::nextResult::$_9
using nextResult_9 = thunk::Lambda<0xF64684, 12, 0xF9C428, 0xE70150, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneBattleResult::nextResult::$_8
using nextResult_8 = thunk::Lambda<0xF646B0, 8, 0xF9C43C, 0xE701F0, 4, thunk::cap(4, 4)>;
// SceneBattleResult::nextResult::$_7
using nextResult_7 = thunk::Lambda<0xF646DC, 8, 0xF9C450, 0xE70280, 4, thunk::cap(4, 1), thunk::cap(5, 1)>;
// SceneBattleResult::nextResult::$_6
using nextResult_6 = thunk::Lambda<0xF64708, 8, 0xF9C464, 0xE70310, 4>;
// SceneBattleResult::nextResult::$_5
using nextResult_5 = thunk::Lambda<0xF64734, 8, 0xF9C478, 0xE703A0, 4, thunk::cap(4, 4)>;
// SceneBattleResult::nextResult::$_4
using nextResult_4 = thunk::Lambda<0xF64760, 8, 0xF9C48C, 0xE70430, 4, thunk::cap(4, 4)>;
// SceneBattleResult::init::$_2
using init_2 = thunk::Lambda<0xF6478C, 8, 0xF9C4A0, 0xE704C0, 4, thunk::cap(4, 4)>;
// SceneBattleResult::init::$_1
using init_1 = thunk::Lambda<0xF647B8, 8, 0xF9C4B4, 0xE70550, 4, thunk::cap(4, 4)>;
// SceneBattleResult::init::$_0
using init_0 = thunk::Lambda<0xF647E4, 8, 0xF9C4C8, 0xE705E0, 4, thunk::cap(4, 4)>;
// SceneBattleResult::nextResult::$_9::lambda()
using nextResult_9_lv = thunk::Lambda<0xF64810, 8, 0xF9C4DC, 0xE70680, 4, thunk::cap(4, 1), thunk::cap(5, 1)>;
// SceneBattleResult::nextResult::$_9::lambda()::lambda()
using nextResult_9_lv_lv = thunk::Lambda<0xF6483C, 8, 0xF9C4F0, 0xE70750, 4>;
// SceneBattleResult::nextResult::$_9::lambda()::lambda(bool)
using nextResult_9_lv_lb = thunk::Lambda<0xF64868, 8, 0xF9C504, 0xE70820, 4, thunk::cap(4, 1), thunk::cap(5, 1)>;
// SceneBattleResult::init::$_21::lambda(bool)
using init_21_lb = thunk::Lambda<0xF64894, 8, 0xF9C518, 0xE708E0, 4, thunk::cap(4, 4)>;
// SceneBattleResult::init::$_21::lambda(bool)::lambda(bool)
using init_21_lb_lb = thunk::Lambda<0xF648C0, 8, 0xF9C52C, 0xE70990, 4, thunk::cap(4, 4)>;
// SceneBattleResult::init::$_3::lambda(bool)
using init_3_lb = thunk::Lambda<0xF648EC, 8, 0xF9C540, 0xE70A40, 4, thunk::cap(4, 4)>;
// SceneBattleResult::init::$_3::lambda(bool)::lambda(bool)
using init_3_lb_lb = thunk::Lambda<0xF64918, 8, 0xF9C554, 0xE70AE0, 4>;

}  // namespace SceneBattleResult_g5thunks

RW_FUNCTION(0x684224, "SceneBattleResult::openGetKeyBladePopup ~__func() (deleting)", SceneBattleResult_g5thunks::openGetKeyBladePopup::deleteThis);
RW_FUNCTION(0x68422C, "SceneBattleResult::openGetKeyBladePopup __clone()", SceneBattleResult_g5thunks::openGetKeyBladePopup::clone);
RW_FUNCTION(0x684258, "SceneBattleResult::openGetKeyBladePopup __clone(__base*)", SceneBattleResult_g5thunks::openGetKeyBladePopup::cloneInto);
RW_FUNCTION(0x684278, "SceneBattleResult::openGetKeyBladePopup destroy()", SceneBattleResult_g5thunks::openGetKeyBladePopup::destroy);
RW_FUNCTION(0x68427A, "SceneBattleResult::openGetKeyBladePopup destroy_deallocate()", SceneBattleResult_g5thunks::openGetKeyBladePopup::deleteThis);
RW_FUNCTION(0x6843B4, "SceneBattleResult::openGetKeyBladePopup target(const type_info&)", SceneBattleResult_g5thunks::openGetKeyBladePopup::target);
RW_FUNCTION(0x6843D4, "SceneBattleResult::openGetKeyBladePopup target_type()", SceneBattleResult_g5thunks::openGetKeyBladePopup::targetType);
RW_FUNCTION(0x6843E8, "SceneBattleResult::openGetKeyBladePopup ~__func()", SceneBattleResult_g5thunks::openGetKeyBladePopup_::destructor);
RW_FUNCTION(0x684424, "SceneBattleResult::openGetKeyBladePopup ~__func() (deleting)", SceneBattleResult_g5thunks::openGetKeyBladePopup_::destructorDelete);
RW_FUNCTION(0x684464, "SceneBattleResult::openGetKeyBladePopup __clone()", SceneBattleResult_g5thunks::openGetKeyBladePopup_::clone);
RW_FUNCTION(0x6844C0, "SceneBattleResult::openGetKeyBladePopup __clone(__base*)", SceneBattleResult_g5thunks::openGetKeyBladePopup_::cloneInto);
RW_FUNCTION(0x684500, "SceneBattleResult::openGetKeyBladePopup destroy()", SceneBattleResult_g5thunks::openGetKeyBladePopup_::destroy);
RW_FUNCTION(0x684520, "SceneBattleResult::openGetKeyBladePopup destroy_deallocate()", SceneBattleResult_g5thunks::openGetKeyBladePopup_::destroyDeallocate);
RW_FUNCTION(0x684684, "SceneBattleResult::openGetKeyBladePopup target(const type_info&)", SceneBattleResult_g5thunks::openGetKeyBladePopup_::target);
RW_FUNCTION(0x6846A8, "SceneBattleResult::openGetKeyBladePopup target_type()", SceneBattleResult_g5thunks::openGetKeyBladePopup_::targetType);
RW_FUNCTION(0x6846BC, "SceneBattleResult::openGetKeyBladePopup ~__func()", SceneBattleResult_g5thunks::openGetKeyBladePopup__::destructor);
RW_FUNCTION(0x6846F8, "SceneBattleResult::openGetKeyBladePopup ~__func() (deleting)", SceneBattleResult_g5thunks::openGetKeyBladePopup__::destructorDelete);
RW_FUNCTION(0x684738, "SceneBattleResult::openGetKeyBladePopup __clone()", SceneBattleResult_g5thunks::openGetKeyBladePopup__::clone);
RW_FUNCTION(0x684794, "SceneBattleResult::openGetKeyBladePopup __clone(__base*)", SceneBattleResult_g5thunks::openGetKeyBladePopup__::cloneInto);
RW_FUNCTION(0x6847D4, "SceneBattleResult::openGetKeyBladePopup destroy()", SceneBattleResult_g5thunks::openGetKeyBladePopup__::destroy);
RW_FUNCTION(0x6847F4, "SceneBattleResult::openGetKeyBladePopup destroy_deallocate()", SceneBattleResult_g5thunks::openGetKeyBladePopup__::destroyDeallocate);
RW_FUNCTION(0x68485C, "SceneBattleResult::openGetKeyBladePopup target(const type_info&)", SceneBattleResult_g5thunks::openGetKeyBladePopup__::target);
RW_FUNCTION(0x684880, "SceneBattleResult::openGetKeyBladePopup target_type()", SceneBattleResult_g5thunks::openGetKeyBladePopup__::targetType);
RW_FUNCTION(0x684894, "SceneBattleResult::startResult::$_17 ~__func() (deleting)", SceneBattleResult_g5thunks::startResult_17::deleteThis);
RW_FUNCTION(0x68489C, "SceneBattleResult::startResult::$_17 __clone()", SceneBattleResult_g5thunks::startResult_17::clone);
RW_FUNCTION(0x6848C8, "SceneBattleResult::startResult::$_17 __clone(__base*)", SceneBattleResult_g5thunks::startResult_17::cloneInto);
RW_FUNCTION(0x6848E8, "SceneBattleResult::startResult::$_17 destroy()", SceneBattleResult_g5thunks::startResult_17::destroy);
RW_FUNCTION(0x6848EA, "SceneBattleResult::startResult::$_17 destroy_deallocate()", SceneBattleResult_g5thunks::startResult_17::deleteThis);
RW_FUNCTION(0x6848FC, "SceneBattleResult::startResult::$_17 target(const type_info&)", SceneBattleResult_g5thunks::startResult_17::target);
RW_FUNCTION(0x68491C, "SceneBattleResult::startResult::$_17 target_type()", SceneBattleResult_g5thunks::startResult_17::targetType);
RW_FUNCTION(0x684930, "SceneBattleResult::startResult::$_16 ~__func() (deleting)", SceneBattleResult_g5thunks::startResult_16::deleteThis);
RW_FUNCTION(0x684938, "SceneBattleResult::startResult::$_16 __clone()", SceneBattleResult_g5thunks::startResult_16::clone);
RW_FUNCTION(0x684964, "SceneBattleResult::startResult::$_16 __clone(__base*)", SceneBattleResult_g5thunks::startResult_16::cloneInto);
RW_FUNCTION(0x684984, "SceneBattleResult::startResult::$_16 destroy()", SceneBattleResult_g5thunks::startResult_16::destroy);
RW_FUNCTION(0x684986, "SceneBattleResult::startResult::$_16 destroy_deallocate()", SceneBattleResult_g5thunks::startResult_16::deleteThis);
RW_FUNCTION(0x684998, "SceneBattleResult::startResult::$_16 target(const type_info&)", SceneBattleResult_g5thunks::startResult_16::target);
RW_FUNCTION(0x6849B8, "SceneBattleResult::startResult::$_16 target_type()", SceneBattleResult_g5thunks::startResult_16::targetType);
RW_FUNCTION(0x6849CC, "SceneBattleResult::startResult::$_15 ~__func() (deleting)", SceneBattleResult_g5thunks::startResult_15::deleteThis);
RW_FUNCTION(0x6849D4, "SceneBattleResult::startResult::$_15 __clone()", SceneBattleResult_g5thunks::startResult_15::clone);
RW_FUNCTION(0x684A00, "SceneBattleResult::startResult::$_15 __clone(__base*)", SceneBattleResult_g5thunks::startResult_15::cloneInto);
RW_FUNCTION(0x684A20, "SceneBattleResult::startResult::$_15 destroy()", SceneBattleResult_g5thunks::startResult_15::destroy);
RW_FUNCTION(0x684A22, "SceneBattleResult::startResult::$_15 destroy_deallocate()", SceneBattleResult_g5thunks::startResult_15::deleteThis);
RW_FUNCTION(0x684A34, "SceneBattleResult::startResult::$_15 target(const type_info&)", SceneBattleResult_g5thunks::startResult_15::target);
RW_FUNCTION(0x684A54, "SceneBattleResult::startResult::$_15 target_type()", SceneBattleResult_g5thunks::startResult_15::targetType);
RW_FUNCTION(0x684A68, "SceneBattleResult::openNewKeyBladePopup::$_14 ~__func() (deleting)", SceneBattleResult_g5thunks::openNewKeyBladePopup_14::deleteThis);
RW_FUNCTION(0x684A70, "SceneBattleResult::openNewKeyBladePopup::$_14 __clone()", SceneBattleResult_g5thunks::openNewKeyBladePopup_14::clone);
RW_FUNCTION(0x684A9C, "SceneBattleResult::openNewKeyBladePopup::$_14 __clone(__base*)", SceneBattleResult_g5thunks::openNewKeyBladePopup_14::cloneInto);
RW_FUNCTION(0x684ABC, "SceneBattleResult::openNewKeyBladePopup::$_14 destroy()", SceneBattleResult_g5thunks::openNewKeyBladePopup_14::destroy);
RW_FUNCTION(0x684ABE, "SceneBattleResult::openNewKeyBladePopup::$_14 destroy_deallocate()", SceneBattleResult_g5thunks::openNewKeyBladePopup_14::deleteThis);
RW_FUNCTION(0x684AD0, "SceneBattleResult::openNewKeyBladePopup::$_14 target(const type_info&)", SceneBattleResult_g5thunks::openNewKeyBladePopup_14::target);
RW_FUNCTION(0x684AF0, "SceneBattleResult::openNewKeyBladePopup::$_14 target_type()", SceneBattleResult_g5thunks::openNewKeyBladePopup_14::targetType);
RW_FUNCTION(0x684B04, "SceneBattleResult::openUserTitlePopup::$_13 ~__func() (deleting)", SceneBattleResult_g5thunks::openUserTitlePopup_13::deleteThis);
RW_FUNCTION(0x684B0C, "SceneBattleResult::openUserTitlePopup::$_13 __clone()", SceneBattleResult_g5thunks::openUserTitlePopup_13::clone);
RW_FUNCTION(0x684B38, "SceneBattleResult::openUserTitlePopup::$_13 __clone(__base*)", SceneBattleResult_g5thunks::openUserTitlePopup_13::cloneInto);
RW_FUNCTION(0x684B58, "SceneBattleResult::openUserTitlePopup::$_13 destroy()", SceneBattleResult_g5thunks::openUserTitlePopup_13::destroy);
RW_FUNCTION(0x684B5A, "SceneBattleResult::openUserTitlePopup::$_13 destroy_deallocate()", SceneBattleResult_g5thunks::openUserTitlePopup_13::deleteThis);
RW_FUNCTION(0x684B6C, "SceneBattleResult::openUserTitlePopup::$_13 target(const type_info&)", SceneBattleResult_g5thunks::openUserTitlePopup_13::target);
RW_FUNCTION(0x684B8C, "SceneBattleResult::openUserTitlePopup::$_13 target_type()", SceneBattleResult_g5thunks::openUserTitlePopup_13::targetType);
RW_FUNCTION(0x684BA0, "SceneBattleResult::openNewLuxPopup::$_12 ~__func() (deleting)", SceneBattleResult_g5thunks::openNewLuxPopup_12::deleteThis);
RW_FUNCTION(0x684BA8, "SceneBattleResult::openNewLuxPopup::$_12 __clone()", SceneBattleResult_g5thunks::openNewLuxPopup_12::clone);
RW_FUNCTION(0x684BD4, "SceneBattleResult::openNewLuxPopup::$_12 __clone(__base*)", SceneBattleResult_g5thunks::openNewLuxPopup_12::cloneInto);
RW_FUNCTION(0x684BF4, "SceneBattleResult::openNewLuxPopup::$_12 destroy()", SceneBattleResult_g5thunks::openNewLuxPopup_12::destroy);
RW_FUNCTION(0x684BF6, "SceneBattleResult::openNewLuxPopup::$_12 destroy_deallocate()", SceneBattleResult_g5thunks::openNewLuxPopup_12::deleteThis);
RW_FUNCTION(0x684C08, "SceneBattleResult::openNewLuxPopup::$_12 target(const type_info&)", SceneBattleResult_g5thunks::openNewLuxPopup_12::target);
RW_FUNCTION(0x684C28, "SceneBattleResult::openNewLuxPopup::$_12 target_type()", SceneBattleResult_g5thunks::openNewLuxPopup_12::targetType);
RW_FUNCTION(0x684C3C, "SceneBattleResult::openLevelUpPopup::$_11 ~__func() (deleting)", SceneBattleResult_g5thunks::openLevelUpPopup_11::deleteThis);
RW_FUNCTION(0x684C44, "SceneBattleResult::openLevelUpPopup::$_11 __clone()", SceneBattleResult_g5thunks::openLevelUpPopup_11::clone);
RW_FUNCTION(0x684C74, "SceneBattleResult::openLevelUpPopup::$_11 __clone(__base*)", SceneBattleResult_g5thunks::openLevelUpPopup_11::cloneInto);
RW_FUNCTION(0x684C98, "SceneBattleResult::openLevelUpPopup::$_11 destroy()", SceneBattleResult_g5thunks::openLevelUpPopup_11::destroy);
RW_FUNCTION(0x684C9A, "SceneBattleResult::openLevelUpPopup::$_11 destroy_deallocate()", SceneBattleResult_g5thunks::openLevelUpPopup_11::deleteThis);
RW_FUNCTION(0x684DF0, "SceneBattleResult::openLevelUpPopup::$_11 target(const type_info&)", SceneBattleResult_g5thunks::openLevelUpPopup_11::target);
RW_FUNCTION(0x684E10, "SceneBattleResult::openLevelUpPopup::$_11 target_type()", SceneBattleResult_g5thunks::openLevelUpPopup_11::targetType);
RW_FUNCTION(0x684E24, "SceneBattleResult::openLevelUpPopup::$_10 ~__func() (deleting)", SceneBattleResult_g5thunks::openLevelUpPopup_10::deleteThis);
RW_FUNCTION(0x684E2C, "SceneBattleResult::openLevelUpPopup::$_10 __clone()", SceneBattleResult_g5thunks::openLevelUpPopup_10::clone);
RW_FUNCTION(0x684E60, "SceneBattleResult::openLevelUpPopup::$_10 __clone(__base*)", SceneBattleResult_g5thunks::openLevelUpPopup_10::cloneInto);
RW_FUNCTION(0x684E8C, "SceneBattleResult::openLevelUpPopup::$_10 destroy()", SceneBattleResult_g5thunks::openLevelUpPopup_10::destroy);
RW_FUNCTION(0x684E8E, "SceneBattleResult::openLevelUpPopup::$_10 destroy_deallocate()", SceneBattleResult_g5thunks::openLevelUpPopup_10::deleteThis);
RW_FUNCTION(0x684F7C, "SceneBattleResult::openLevelUpPopup::$_10 target(const type_info&)", SceneBattleResult_g5thunks::openLevelUpPopup_10::target);
RW_FUNCTION(0x684F9C, "SceneBattleResult::openLevelUpPopup::$_10 target_type()", SceneBattleResult_g5thunks::openLevelUpPopup_10::targetType);
RW_FUNCTION(0x684FB0, "SceneBattleResult::openLevelUpPopup::$_10::lambda() ~__func() (deleting)", SceneBattleResult_g5thunks::openLevelUpPopup_10_lv::deleteThis);
RW_FUNCTION(0x684FB8, "SceneBattleResult::openLevelUpPopup::$_10::lambda() __clone()", SceneBattleResult_g5thunks::openLevelUpPopup_10_lv::clone);
RW_FUNCTION(0x684FEC, "SceneBattleResult::openLevelUpPopup::$_10::lambda() __clone(__base*)", SceneBattleResult_g5thunks::openLevelUpPopup_10_lv::cloneInto);
RW_FUNCTION(0x685018, "SceneBattleResult::openLevelUpPopup::$_10::lambda() destroy()", SceneBattleResult_g5thunks::openLevelUpPopup_10_lv::destroy);
RW_FUNCTION(0x68501A, "SceneBattleResult::openLevelUpPopup::$_10::lambda() destroy_deallocate()", SceneBattleResult_g5thunks::openLevelUpPopup_10_lv::deleteThis);
RW_FUNCTION(0x68505C, "SceneBattleResult::openLevelUpPopup::$_10::lambda() target(const type_info&)", SceneBattleResult_g5thunks::openLevelUpPopup_10_lv::target);
RW_FUNCTION(0x68507C, "SceneBattleResult::openLevelUpPopup::$_10::lambda() target_type()", SceneBattleResult_g5thunks::openLevelUpPopup_10_lv::targetType);
RW_FUNCTION(0x685090, "SceneBattleResult::nextResult::$_9 ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_9::deleteThis);
RW_FUNCTION(0x685098, "SceneBattleResult::nextResult::$_9 __clone()", SceneBattleResult_g5thunks::nextResult_9::clone);
RW_FUNCTION(0x6850C8, "SceneBattleResult::nextResult::$_9 __clone(__base*)", SceneBattleResult_g5thunks::nextResult_9::cloneInto);
RW_FUNCTION(0x6850EC, "SceneBattleResult::nextResult::$_9 destroy()", SceneBattleResult_g5thunks::nextResult_9::destroy);
RW_FUNCTION(0x6850EE, "SceneBattleResult::nextResult::$_9 destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_9::deleteThis);
RW_FUNCTION(0x6851B8, "SceneBattleResult::nextResult::$_9 target(const type_info&)", SceneBattleResult_g5thunks::nextResult_9::target);
RW_FUNCTION(0x6851D8, "SceneBattleResult::nextResult::$_9 target_type()", SceneBattleResult_g5thunks::nextResult_9::targetType);
RW_FUNCTION(0x6851EC, "SceneBattleResult::nextResult::$_8 ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_8::deleteThis);
RW_FUNCTION(0x6851F4, "SceneBattleResult::nextResult::$_8 __clone()", SceneBattleResult_g5thunks::nextResult_8::clone);
RW_FUNCTION(0x685220, "SceneBattleResult::nextResult::$_8 __clone(__base*)", SceneBattleResult_g5thunks::nextResult_8::cloneInto);
RW_FUNCTION(0x685240, "SceneBattleResult::nextResult::$_8 destroy()", SceneBattleResult_g5thunks::nextResult_8::destroy);
RW_FUNCTION(0x685242, "SceneBattleResult::nextResult::$_8 destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_8::deleteThis);
RW_FUNCTION(0x685258, "SceneBattleResult::nextResult::$_8 target(const type_info&)", SceneBattleResult_g5thunks::nextResult_8::target);
RW_FUNCTION(0x685278, "SceneBattleResult::nextResult::$_8 target_type()", SceneBattleResult_g5thunks::nextResult_8::targetType);
RW_FUNCTION(0x68528C, "SceneBattleResult::nextResult::$_7 ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_7::deleteThis);
RW_FUNCTION(0x685294, "SceneBattleResult::nextResult::$_7 __clone()", SceneBattleResult_g5thunks::nextResult_7::clone);
RW_FUNCTION(0x6852C4, "SceneBattleResult::nextResult::$_7 __clone(__base*)", SceneBattleResult_g5thunks::nextResult_7::cloneInto);
RW_FUNCTION(0x6852E8, "SceneBattleResult::nextResult::$_7 destroy()", SceneBattleResult_g5thunks::nextResult_7::destroy);
RW_FUNCTION(0x6852EA, "SceneBattleResult::nextResult::$_7 destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_7::deleteThis);
RW_FUNCTION(0x685390, "SceneBattleResult::nextResult::$_7 target(const type_info&)", SceneBattleResult_g5thunks::nextResult_7::target);
RW_FUNCTION(0x6853B0, "SceneBattleResult::nextResult::$_7 target_type()", SceneBattleResult_g5thunks::nextResult_7::targetType);
RW_FUNCTION(0x6853C4, "SceneBattleResult::nextResult::$_6 ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_6::deleteThis);
RW_FUNCTION(0x6853CC, "SceneBattleResult::nextResult::$_6 __clone()", SceneBattleResult_g5thunks::nextResult_6::clone);
RW_FUNCTION(0x6853F0, "SceneBattleResult::nextResult::$_6 __clone(__base*)", SceneBattleResult_g5thunks::nextResult_6::cloneInto);
RW_FUNCTION(0x68540C, "SceneBattleResult::nextResult::$_6 destroy()", SceneBattleResult_g5thunks::nextResult_6::destroy);
RW_FUNCTION(0x68540E, "SceneBattleResult::nextResult::$_6 destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_6::deleteThis);
RW_FUNCTION(0x68543C, "SceneBattleResult::nextResult::$_6 target(const type_info&)", SceneBattleResult_g5thunks::nextResult_6::target);
RW_FUNCTION(0x68545C, "SceneBattleResult::nextResult::$_6 target_type()", SceneBattleResult_g5thunks::nextResult_6::targetType);
RW_FUNCTION(0x685470, "SceneBattleResult::nextResult::$_5 ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_5::deleteThis);
RW_FUNCTION(0x685478, "SceneBattleResult::nextResult::$_5 __clone()", SceneBattleResult_g5thunks::nextResult_5::clone);
RW_FUNCTION(0x6854A4, "SceneBattleResult::nextResult::$_5 __clone(__base*)", SceneBattleResult_g5thunks::nextResult_5::cloneInto);
RW_FUNCTION(0x6854C4, "SceneBattleResult::nextResult::$_5 destroy()", SceneBattleResult_g5thunks::nextResult_5::destroy);
RW_FUNCTION(0x6854C6, "SceneBattleResult::nextResult::$_5 destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_5::deleteThis);
RW_FUNCTION(0x6854D8, "SceneBattleResult::nextResult::$_5 target(const type_info&)", SceneBattleResult_g5thunks::nextResult_5::target);
RW_FUNCTION(0x6854F8, "SceneBattleResult::nextResult::$_5 target_type()", SceneBattleResult_g5thunks::nextResult_5::targetType);
RW_FUNCTION(0x68550C, "SceneBattleResult::nextResult::$_4 ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_4::deleteThis);
RW_FUNCTION(0x685514, "SceneBattleResult::nextResult::$_4 __clone()", SceneBattleResult_g5thunks::nextResult_4::clone);
RW_FUNCTION(0x685540, "SceneBattleResult::nextResult::$_4 __clone(__base*)", SceneBattleResult_g5thunks::nextResult_4::cloneInto);
RW_FUNCTION(0x685560, "SceneBattleResult::nextResult::$_4 destroy()", SceneBattleResult_g5thunks::nextResult_4::destroy);
RW_FUNCTION(0x685562, "SceneBattleResult::nextResult::$_4 destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_4::deleteThis);
RW_FUNCTION(0x685574, "SceneBattleResult::nextResult::$_4 target(const type_info&)", SceneBattleResult_g5thunks::nextResult_4::target);
RW_FUNCTION(0x685594, "SceneBattleResult::nextResult::$_4 target_type()", SceneBattleResult_g5thunks::nextResult_4::targetType);
RW_FUNCTION(0x6855A8, "SceneBattleResult::init::$_2 ~__func() (deleting)", SceneBattleResult_g5thunks::init_2::deleteThis);
RW_FUNCTION(0x6855B0, "SceneBattleResult::init::$_2 __clone()", SceneBattleResult_g5thunks::init_2::clone);
RW_FUNCTION(0x6855DC, "SceneBattleResult::init::$_2 __clone(__base*)", SceneBattleResult_g5thunks::init_2::cloneInto);
RW_FUNCTION(0x6855FC, "SceneBattleResult::init::$_2 destroy()", SceneBattleResult_g5thunks::init_2::destroy);
RW_FUNCTION(0x6855FE, "SceneBattleResult::init::$_2 destroy_deallocate()", SceneBattleResult_g5thunks::init_2::deleteThis);
RW_FUNCTION(0x685624, "SceneBattleResult::init::$_2 target(const type_info&)", SceneBattleResult_g5thunks::init_2::target);
RW_FUNCTION(0x685644, "SceneBattleResult::init::$_2 target_type()", SceneBattleResult_g5thunks::init_2::targetType);
RW_FUNCTION(0x685658, "SceneBattleResult::init::$_1 ~__func() (deleting)", SceneBattleResult_g5thunks::init_1::deleteThis);
RW_FUNCTION(0x685660, "SceneBattleResult::init::$_1 __clone()", SceneBattleResult_g5thunks::init_1::clone);
RW_FUNCTION(0x68568C, "SceneBattleResult::init::$_1 __clone(__base*)", SceneBattleResult_g5thunks::init_1::cloneInto);
RW_FUNCTION(0x6856AC, "SceneBattleResult::init::$_1 destroy()", SceneBattleResult_g5thunks::init_1::destroy);
RW_FUNCTION(0x6856AE, "SceneBattleResult::init::$_1 destroy_deallocate()", SceneBattleResult_g5thunks::init_1::deleteThis);
RW_FUNCTION(0x6856C0, "SceneBattleResult::init::$_1 target(const type_info&)", SceneBattleResult_g5thunks::init_1::target);
RW_FUNCTION(0x6856E0, "SceneBattleResult::init::$_1 target_type()", SceneBattleResult_g5thunks::init_1::targetType);
RW_FUNCTION(0x6856F4, "SceneBattleResult::init::$_0 ~__func() (deleting)", SceneBattleResult_g5thunks::init_0::deleteThis);
RW_FUNCTION(0x6856FC, "SceneBattleResult::init::$_0 __clone()", SceneBattleResult_g5thunks::init_0::clone);
RW_FUNCTION(0x685728, "SceneBattleResult::init::$_0 __clone(__base*)", SceneBattleResult_g5thunks::init_0::cloneInto);
RW_FUNCTION(0x685748, "SceneBattleResult::init::$_0 destroy()", SceneBattleResult_g5thunks::init_0::destroy);
RW_FUNCTION(0x68574A, "SceneBattleResult::init::$_0 destroy_deallocate()", SceneBattleResult_g5thunks::init_0::deleteThis);
RW_FUNCTION(0x6857F0, "SceneBattleResult::init::$_0 target(const type_info&)", SceneBattleResult_g5thunks::init_0::target);
RW_FUNCTION(0x685810, "SceneBattleResult::init::$_0 target_type()", SceneBattleResult_g5thunks::init_0::targetType);
RW_FUNCTION(0x685824, "SceneBattleResult::nextResult::$_9::lambda() ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_9_lv::deleteThis);
RW_FUNCTION(0x68582C, "SceneBattleResult::nextResult::$_9::lambda() __clone()", SceneBattleResult_g5thunks::nextResult_9_lv::clone);
RW_FUNCTION(0x68585C, "SceneBattleResult::nextResult::$_9::lambda() __clone(__base*)", SceneBattleResult_g5thunks::nextResult_9_lv::cloneInto);
RW_FUNCTION(0x685880, "SceneBattleResult::nextResult::$_9::lambda() destroy()", SceneBattleResult_g5thunks::nextResult_9_lv::destroy);
RW_FUNCTION(0x685882, "SceneBattleResult::nextResult::$_9::lambda() destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_9_lv::deleteThis);
RW_FUNCTION(0x6859B8, "SceneBattleResult::nextResult::$_9::lambda() target(const type_info&)", SceneBattleResult_g5thunks::nextResult_9_lv::target);
RW_FUNCTION(0x6859D8, "SceneBattleResult::nextResult::$_9::lambda() target_type()", SceneBattleResult_g5thunks::nextResult_9_lv::targetType);
RW_FUNCTION(0x6859EC, "SceneBattleResult::nextResult::$_9::lambda()::lambda() ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_9_lv_lv::deleteThis);
RW_FUNCTION(0x6859F4, "SceneBattleResult::nextResult::$_9::lambda()::lambda() __clone()", SceneBattleResult_g5thunks::nextResult_9_lv_lv::clone);
RW_FUNCTION(0x685A18, "SceneBattleResult::nextResult::$_9::lambda()::lambda() __clone(__base*)", SceneBattleResult_g5thunks::nextResult_9_lv_lv::cloneInto);
RW_FUNCTION(0x685A34, "SceneBattleResult::nextResult::$_9::lambda()::lambda() destroy()", SceneBattleResult_g5thunks::nextResult_9_lv_lv::destroy);
RW_FUNCTION(0x685A36, "SceneBattleResult::nextResult::$_9::lambda()::lambda() destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_9_lv_lv::deleteThis);
RW_FUNCTION(0x685A54, "SceneBattleResult::nextResult::$_9::lambda()::lambda() target(const type_info&)", SceneBattleResult_g5thunks::nextResult_9_lv_lv::target);
RW_FUNCTION(0x685A74, "SceneBattleResult::nextResult::$_9::lambda()::lambda() target_type()", SceneBattleResult_g5thunks::nextResult_9_lv_lv::targetType);
RW_FUNCTION(0x685A88, "SceneBattleResult::nextResult::$_9::lambda()::lambda(bool) ~__func() (deleting)", SceneBattleResult_g5thunks::nextResult_9_lv_lb::deleteThis);
RW_FUNCTION(0x685A90, "SceneBattleResult::nextResult::$_9::lambda()::lambda(bool) __clone()", SceneBattleResult_g5thunks::nextResult_9_lv_lb::clone);
RW_FUNCTION(0x685AC0, "SceneBattleResult::nextResult::$_9::lambda()::lambda(bool) __clone(__base*)", SceneBattleResult_g5thunks::nextResult_9_lv_lb::cloneInto);
RW_FUNCTION(0x685AE4, "SceneBattleResult::nextResult::$_9::lambda()::lambda(bool) destroy()", SceneBattleResult_g5thunks::nextResult_9_lv_lb::destroy);
RW_FUNCTION(0x685AE6, "SceneBattleResult::nextResult::$_9::lambda()::lambda(bool) destroy_deallocate()", SceneBattleResult_g5thunks::nextResult_9_lv_lb::deleteThis);
RW_FUNCTION(0x685B8C, "SceneBattleResult::nextResult::$_9::lambda()::lambda(bool) target(const type_info&)", SceneBattleResult_g5thunks::nextResult_9_lv_lb::target);
RW_FUNCTION(0x685BAC, "SceneBattleResult::nextResult::$_9::lambda()::lambda(bool) target_type()", SceneBattleResult_g5thunks::nextResult_9_lv_lb::targetType);
RW_FUNCTION(0x685D90, "SceneBattleResult::init::$_21::lambda(bool) ~__func() (deleting)", SceneBattleResult_g5thunks::init_21_lb::deleteThis);
RW_FUNCTION(0x685D98, "SceneBattleResult::init::$_21::lambda(bool) __clone()", SceneBattleResult_g5thunks::init_21_lb::clone);
RW_FUNCTION(0x685DC4, "SceneBattleResult::init::$_21::lambda(bool) __clone(__base*)", SceneBattleResult_g5thunks::init_21_lb::cloneInto);
RW_FUNCTION(0x685DE4, "SceneBattleResult::init::$_21::lambda(bool) destroy()", SceneBattleResult_g5thunks::init_21_lb::destroy);
RW_FUNCTION(0x685DE6, "SceneBattleResult::init::$_21::lambda(bool) destroy_deallocate()", SceneBattleResult_g5thunks::init_21_lb::deleteThis);
RW_FUNCTION(0x685EBC, "SceneBattleResult::init::$_21::lambda(bool) target(const type_info&)", SceneBattleResult_g5thunks::init_21_lb::target);
RW_FUNCTION(0x685EDC, "SceneBattleResult::init::$_21::lambda(bool) target_type()", SceneBattleResult_g5thunks::init_21_lb::targetType);
RW_FUNCTION(0x685EF0, "SceneBattleResult::init::$_21::lambda(bool)::lambda(bool) ~__func() (deleting)", SceneBattleResult_g5thunks::init_21_lb_lb::deleteThis);
RW_FUNCTION(0x685EF8, "SceneBattleResult::init::$_21::lambda(bool)::lambda(bool) __clone()", SceneBattleResult_g5thunks::init_21_lb_lb::clone);
RW_FUNCTION(0x685F24, "SceneBattleResult::init::$_21::lambda(bool)::lambda(bool) __clone(__base*)", SceneBattleResult_g5thunks::init_21_lb_lb::cloneInto);
RW_FUNCTION(0x685F44, "SceneBattleResult::init::$_21::lambda(bool)::lambda(bool) destroy()", SceneBattleResult_g5thunks::init_21_lb_lb::destroy);
RW_FUNCTION(0x685F46, "SceneBattleResult::init::$_21::lambda(bool)::lambda(bool) destroy_deallocate()", SceneBattleResult_g5thunks::init_21_lb_lb::deleteThis);
RW_FUNCTION(0x685F58, "SceneBattleResult::init::$_21::lambda(bool)::lambda(bool) target(const type_info&)", SceneBattleResult_g5thunks::init_21_lb_lb::target);
RW_FUNCTION(0x685F78, "SceneBattleResult::init::$_21::lambda(bool)::lambda(bool) target_type()", SceneBattleResult_g5thunks::init_21_lb_lb::targetType);
RW_FUNCTION(0x685F8C, "SceneBattleResult::init::$_3::lambda(bool) ~__func() (deleting)", SceneBattleResult_g5thunks::init_3_lb::deleteThis);
RW_FUNCTION(0x685F94, "SceneBattleResult::init::$_3::lambda(bool) __clone()", SceneBattleResult_g5thunks::init_3_lb::clone);
RW_FUNCTION(0x685FC0, "SceneBattleResult::init::$_3::lambda(bool) __clone(__base*)", SceneBattleResult_g5thunks::init_3_lb::cloneInto);
RW_FUNCTION(0x685FE0, "SceneBattleResult::init::$_3::lambda(bool) destroy()", SceneBattleResult_g5thunks::init_3_lb::destroy);
RW_FUNCTION(0x685FE2, "SceneBattleResult::init::$_3::lambda(bool) destroy_deallocate()", SceneBattleResult_g5thunks::init_3_lb::deleteThis);
RW_FUNCTION(0x6860C0, "SceneBattleResult::init::$_3::lambda(bool) target(const type_info&)", SceneBattleResult_g5thunks::init_3_lb::target);
RW_FUNCTION(0x6860E0, "SceneBattleResult::init::$_3::lambda(bool) target_type()", SceneBattleResult_g5thunks::init_3_lb::targetType);
RW_FUNCTION(0x6860F4, "SceneBattleResult::init::$_3::lambda(bool)::lambda(bool) ~__func() (deleting)", SceneBattleResult_g5thunks::init_3_lb_lb::deleteThis);
RW_FUNCTION(0x6860FC, "SceneBattleResult::init::$_3::lambda(bool)::lambda(bool) __clone()", SceneBattleResult_g5thunks::init_3_lb_lb::clone);
RW_FUNCTION(0x686120, "SceneBattleResult::init::$_3::lambda(bool)::lambda(bool) __clone(__base*)", SceneBattleResult_g5thunks::init_3_lb_lb::cloneInto);
RW_FUNCTION(0x68613C, "SceneBattleResult::init::$_3::lambda(bool)::lambda(bool) destroy()", SceneBattleResult_g5thunks::init_3_lb_lb::destroy);
RW_FUNCTION(0x68613E, "SceneBattleResult::init::$_3::lambda(bool)::lambda(bool) destroy_deallocate()", SceneBattleResult_g5thunks::init_3_lb_lb::deleteThis);
RW_FUNCTION(0x686154, "SceneBattleResult::init::$_3::lambda(bool)::lambda(bool) target(const type_info&)", SceneBattleResult_g5thunks::init_3_lb_lb::target);
RW_FUNCTION(0x686174, "SceneBattleResult::init::$_3::lambda(bool)::lambda(bool) target_type()", SceneBattleResult_g5thunks::init_3_lb_lb::targetType);
