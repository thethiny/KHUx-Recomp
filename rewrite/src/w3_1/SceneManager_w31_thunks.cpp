// SceneManager: the std::function slots of its lambdas, generated from the binary's __func vtables
// (each thunk's shape was checked by emulating the original instructions). The lambda bodies
// (operator()) are written by hand.
#include "../scene_main/FunctionThunks.hpp"

namespace SceneManager_w31_thunks {

// SceneManager::createHeader::$_11 UlvE_
using createHeader_11_UlvE_ = thunk::Lambda<0xF6073C, 8, 0xF98A2C, 0xE59EA0, 4, thunk::cap(4, 4)>;
// SceneManager::createHeader::$_10
using createHeader_10 = thunk::Lambda<0xF60768, 12, 0xF98A40, 0xE59FA0, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneManager::createHeader::$_10 UlvE_
using createHeader_10_UlvE_ = thunk::Lambda<0xF60794, 8, 0xF98A54, 0xE5A070, 4, thunk::cap(4, 4)>;
// SceneManager::createHeader::$_9
using createHeader_9 = thunk::Lambda<0xF607C0, 12, 0xF98A68, 0xE5A170, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneManager::update::$_8
using update_8 = thunk::Lambda<0xF607EC, 8, 0xF98A7C, 0xE5A200, 4>;
// SceneManager::update::$_7
using update_7 = thunk::Lambda<0xF60818, 8, 0xF98A90, 0xE5A280, 4, thunk::cap(4, 4)>;
// SceneManager::moveToTitleForcibly::$_6 (captures a std::function at +8, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using moveToTitleForcibly_6 = thunk::LambdaFn<0xF60844, 40, 0xF98AA4, 0xE5A320, 8, 8, 0x4683CD, thunk::cap(32, 4)>;
// SceneManager::init::$_5
using init_5 = thunk::Lambda<0xF60870, 8, 0xF98AB8, 0xE5A3D0, 4, thunk::cap(4, 4)>;
// SceneManager::init::$_4
using init_4 = thunk::Lambda<0xF6089C, 8, 0xF98ACC, 0xE5A450, 4, thunk::cap(4, 4)>;
// SceneManager::init::$_3
using init_3 = thunk::Lambda<0xF608C8, 8, 0xF98AE0, 0xE5A4D0, 4, thunk::cap(4, 4)>;
// SceneManager::openChatPopup::$_130
using openChatPopup_130 = thunk::Lambda<0xF609A4, 40, 0xF98B44, 0xE5A7D0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;
// SceneManager::openChatPopup::$_129 UlbE_
using openChatPopup_129_UlbE_ = thunk::Lambda<0xF609D0, 8, 0xF98B58, 0xE5A870, 4>;
// SceneManager::closeGlobalMenu::$_20 UlvE_
using closeGlobalMenu_20_UlvE_ = thunk::Lambda<0xF609FC, 8, 0xF98B6C, 0xE5A940, 4, thunk::cap(4, 4)>;

}  // namespace SceneManager_w31_thunks

RW_FUNCTION(0x63768C, "SceneManager::createHeader::$_11 UlvE_ target_type()", SceneManager_w31_thunks::createHeader_11_UlvE_::targetType);
RW_FUNCTION(0x6376A0, "SceneManager::createHeader::$_10 ~__func() (deleting)", SceneManager_w31_thunks::createHeader_10::deleteThis);
RW_FUNCTION(0x6376A8, "SceneManager::createHeader::$_10 __clone()", SceneManager_w31_thunks::createHeader_10::clone);
RW_FUNCTION(0x6376D8, "SceneManager::createHeader::$_10 __clone(__base*)", SceneManager_w31_thunks::createHeader_10::cloneInto);
RW_FUNCTION(0x6376FC, "SceneManager::createHeader::$_10 destroy()", SceneManager_w31_thunks::createHeader_10::destroy);
RW_FUNCTION(0x6376FE, "SceneManager::createHeader::$_10 destroy_deallocate()", SceneManager_w31_thunks::createHeader_10::deleteThis);
RW_FUNCTION(0x6377FC, "SceneManager::createHeader::$_10 target(const type_info&)", SceneManager_w31_thunks::createHeader_10::target);
RW_FUNCTION(0x63781C, "SceneManager::createHeader::$_10 target_type()", SceneManager_w31_thunks::createHeader_10::targetType);
RW_FUNCTION(0x637830, "SceneManager::createHeader::$_10 UlvE_ ~__func() (deleting)", SceneManager_w31_thunks::createHeader_10_UlvE_::deleteThis);
RW_FUNCTION(0x637838, "SceneManager::createHeader::$_10 UlvE_ __clone()", SceneManager_w31_thunks::createHeader_10_UlvE_::clone);
RW_FUNCTION(0x637864, "SceneManager::createHeader::$_10 UlvE_ __clone(__base*)", SceneManager_w31_thunks::createHeader_10_UlvE_::cloneInto);
RW_FUNCTION(0x637884, "SceneManager::createHeader::$_10 UlvE_ destroy()", SceneManager_w31_thunks::createHeader_10_UlvE_::destroy);
RW_FUNCTION(0x637886, "SceneManager::createHeader::$_10 UlvE_ destroy_deallocate()", SceneManager_w31_thunks::createHeader_10_UlvE_::deleteThis);
RW_FUNCTION(0x6378A0, "SceneManager::createHeader::$_10 UlvE_ target(const type_info&)", SceneManager_w31_thunks::createHeader_10_UlvE_::target);
RW_FUNCTION(0x6378C0, "SceneManager::createHeader::$_10 UlvE_ target_type()", SceneManager_w31_thunks::createHeader_10_UlvE_::targetType);
RW_FUNCTION(0x6378D4, "SceneManager::createHeader::$_9 ~__func() (deleting)", SceneManager_w31_thunks::createHeader_9::deleteThis);
RW_FUNCTION(0x6378DC, "SceneManager::createHeader::$_9 __clone()", SceneManager_w31_thunks::createHeader_9::clone);
RW_FUNCTION(0x63790C, "SceneManager::createHeader::$_9 __clone(__base*)", SceneManager_w31_thunks::createHeader_9::cloneInto);
RW_FUNCTION(0x637930, "SceneManager::createHeader::$_9 destroy()", SceneManager_w31_thunks::createHeader_9::destroy);
RW_FUNCTION(0x637932, "SceneManager::createHeader::$_9 destroy_deallocate()", SceneManager_w31_thunks::createHeader_9::deleteThis);
RW_FUNCTION(0x6379B8, "SceneManager::createHeader::$_9 target(const type_info&)", SceneManager_w31_thunks::createHeader_9::target);
RW_FUNCTION(0x6379D8, "SceneManager::createHeader::$_9 target_type()", SceneManager_w31_thunks::createHeader_9::targetType);
RW_FUNCTION(0x6379EC, "SceneManager::update::$_8 ~__func() (deleting)", SceneManager_w31_thunks::update_8::deleteThis);
RW_FUNCTION(0x6379F4, "SceneManager::update::$_8 __clone()", SceneManager_w31_thunks::update_8::clone);
RW_FUNCTION(0x637A18, "SceneManager::update::$_8 __clone(__base*)", SceneManager_w31_thunks::update_8::cloneInto);
RW_FUNCTION(0x637A34, "SceneManager::update::$_8 destroy()", SceneManager_w31_thunks::update_8::destroy);
RW_FUNCTION(0x637A36, "SceneManager::update::$_8 destroy_deallocate()", SceneManager_w31_thunks::update_8::deleteThis);
RW_FUNCTION(0x637B28, "SceneManager::update::$_8 target(const type_info&)", SceneManager_w31_thunks::update_8::target);
RW_FUNCTION(0x637B48, "SceneManager::update::$_8 target_type()", SceneManager_w31_thunks::update_8::targetType);
RW_FUNCTION(0x637B5C, "SceneManager::update::$_7 ~__func() (deleting)", SceneManager_w31_thunks::update_7::deleteThis);
RW_FUNCTION(0x637B64, "SceneManager::update::$_7 __clone()", SceneManager_w31_thunks::update_7::clone);
RW_FUNCTION(0x637B90, "SceneManager::update::$_7 __clone(__base*)", SceneManager_w31_thunks::update_7::cloneInto);
RW_FUNCTION(0x637BB0, "SceneManager::update::$_7 destroy()", SceneManager_w31_thunks::update_7::destroy);
RW_FUNCTION(0x637BB2, "SceneManager::update::$_7 destroy_deallocate()", SceneManager_w31_thunks::update_7::deleteThis);
RW_FUNCTION(0x637BE8, "SceneManager::update::$_7 target(const type_info&)", SceneManager_w31_thunks::update_7::target);
RW_FUNCTION(0x637C08, "SceneManager::update::$_7 target_type()", SceneManager_w31_thunks::update_7::targetType);
RW_FUNCTION(0x637C1C, "SceneManager::moveToTitleForcibly::$_6 ~__func()", SceneManager_w31_thunks::moveToTitleForcibly_6::destructor);
RW_FUNCTION(0x637C58, "SceneManager::moveToTitleForcibly::$_6 ~__func() (deleting)", SceneManager_w31_thunks::moveToTitleForcibly_6::destructorDelete);
RW_FUNCTION(0x637C98, "SceneManager::moveToTitleForcibly::$_6 __clone()", SceneManager_w31_thunks::moveToTitleForcibly_6::clone);
RW_FUNCTION(0x637CEC, "SceneManager::moveToTitleForcibly::$_6 __clone(__base*)", SceneManager_w31_thunks::moveToTitleForcibly_6::cloneInto);
RW_FUNCTION(0x637D28, "SceneManager::moveToTitleForcibly::$_6 destroy()", SceneManager_w31_thunks::moveToTitleForcibly_6::destroy);
RW_FUNCTION(0x637D48, "SceneManager::moveToTitleForcibly::$_6 destroy_deallocate()", SceneManager_w31_thunks::moveToTitleForcibly_6::destroyDeallocate);
RW_FUNCTION(0x637E84, "SceneManager::moveToTitleForcibly::$_6 target(const type_info&)", SceneManager_w31_thunks::moveToTitleForcibly_6::target);
RW_FUNCTION(0x637EA4, "SceneManager::moveToTitleForcibly::$_6 target_type()", SceneManager_w31_thunks::moveToTitleForcibly_6::targetType);
RW_FUNCTION(0x637F58, "SceneManager::init::$_5 ~__func() (deleting)", SceneManager_w31_thunks::init_5::deleteThis);
RW_FUNCTION(0x637F60, "SceneManager::init::$_5 __clone()", SceneManager_w31_thunks::init_5::clone);
RW_FUNCTION(0x637F8C, "SceneManager::init::$_5 __clone(__base*)", SceneManager_w31_thunks::init_5::cloneInto);
RW_FUNCTION(0x637FAC, "SceneManager::init::$_5 destroy()", SceneManager_w31_thunks::init_5::destroy);
RW_FUNCTION(0x637FAE, "SceneManager::init::$_5 destroy_deallocate()", SceneManager_w31_thunks::init_5::deleteThis);
RW_FUNCTION(0x638064, "SceneManager::init::$_5 target(const type_info&)", SceneManager_w31_thunks::init_5::target);
RW_FUNCTION(0x638084, "SceneManager::init::$_5 target_type()", SceneManager_w31_thunks::init_5::targetType);
RW_FUNCTION(0x638098, "SceneManager::init::$_4 ~__func() (deleting)", SceneManager_w31_thunks::init_4::deleteThis);
RW_FUNCTION(0x6380A0, "SceneManager::init::$_4 __clone()", SceneManager_w31_thunks::init_4::clone);
RW_FUNCTION(0x6380CC, "SceneManager::init::$_4 __clone(__base*)", SceneManager_w31_thunks::init_4::cloneInto);
RW_FUNCTION(0x6380EC, "SceneManager::init::$_4 destroy()", SceneManager_w31_thunks::init_4::destroy);
RW_FUNCTION(0x6380EE, "SceneManager::init::$_4 destroy_deallocate()", SceneManager_w31_thunks::init_4::deleteThis);
RW_FUNCTION(0x638100, "SceneManager::init::$_4 target(const type_info&)", SceneManager_w31_thunks::init_4::target);
RW_FUNCTION(0x638120, "SceneManager::init::$_4 target_type()", SceneManager_w31_thunks::init_4::targetType);
RW_FUNCTION(0x638134, "SceneManager::init::$_3 ~__func() (deleting)", SceneManager_w31_thunks::init_3::deleteThis);
RW_FUNCTION(0x63813C, "SceneManager::init::$_3 __clone()", SceneManager_w31_thunks::init_3::clone);
RW_FUNCTION(0x638168, "SceneManager::init::$_3 __clone(__base*)", SceneManager_w31_thunks::init_3::cloneInto);
RW_FUNCTION(0x638188, "SceneManager::init::$_3 destroy()", SceneManager_w31_thunks::init_3::destroy);
RW_FUNCTION(0x63818A, "SceneManager::init::$_3 destroy_deallocate()", SceneManager_w31_thunks::init_3::deleteThis);
RW_FUNCTION(0x638B1C, "SceneManager::openChatPopup::$_130 target(const type_info&)", SceneManager_w31_thunks::openChatPopup_130::target);
RW_FUNCTION(0x638B3C, "SceneManager::openChatPopup::$_130 target_type()", SceneManager_w31_thunks::openChatPopup_130::targetType);
RW_FUNCTION(0x638B50, "SceneManager::openChatPopup::$_129 UlbE_ ~__func() (deleting)", SceneManager_w31_thunks::openChatPopup_129_UlbE_::deleteThis);
RW_FUNCTION(0x638B58, "SceneManager::openChatPopup::$_129 UlbE_ __clone()", SceneManager_w31_thunks::openChatPopup_129_UlbE_::clone);
RW_FUNCTION(0x638B7C, "SceneManager::openChatPopup::$_129 UlbE_ __clone(__base*)", SceneManager_w31_thunks::openChatPopup_129_UlbE_::cloneInto);
RW_FUNCTION(0x638B98, "SceneManager::openChatPopup::$_129 UlbE_ destroy()", SceneManager_w31_thunks::openChatPopup_129_UlbE_::destroy);
RW_FUNCTION(0x638B9A, "SceneManager::openChatPopup::$_129 UlbE_ destroy_deallocate()", SceneManager_w31_thunks::openChatPopup_129_UlbE_::deleteThis);
RW_FUNCTION(0x638BB0, "SceneManager::openChatPopup::$_129 UlbE_ target(const type_info&)", SceneManager_w31_thunks::openChatPopup_129_UlbE_::target);
RW_FUNCTION(0x638BD0, "SceneManager::openChatPopup::$_129 UlbE_ target_type()", SceneManager_w31_thunks::openChatPopup_129_UlbE_::targetType);
RW_FUNCTION(0x638BE4, "SceneManager::closeGlobalMenu::$_20 UlvE_ ~__func() (deleting)", SceneManager_w31_thunks::closeGlobalMenu_20_UlvE_::deleteThis);
RW_FUNCTION(0x638BEC, "SceneManager::closeGlobalMenu::$_20 UlvE_ __clone()", SceneManager_w31_thunks::closeGlobalMenu_20_UlvE_::clone);
RW_FUNCTION(0x638C18, "SceneManager::closeGlobalMenu::$_20 UlvE_ __clone(__base*)", SceneManager_w31_thunks::closeGlobalMenu_20_UlvE_::cloneInto);
RW_FUNCTION(0x638C38, "SceneManager::closeGlobalMenu::$_20 UlvE_ destroy()", SceneManager_w31_thunks::closeGlobalMenu_20_UlvE_::destroy);
RW_FUNCTION(0x638C3A, "SceneManager::closeGlobalMenu::$_20 UlvE_ destroy_deallocate()", SceneManager_w31_thunks::closeGlobalMenu_20_UlvE_::deleteThis);
RW_FUNCTION(0x638C88, "SceneManager::closeGlobalMenu::$_20 UlvE_ target(const type_info&)", SceneManager_w31_thunks::closeGlobalMenu_20_UlvE_::target);
RW_FUNCTION(0x638CA8, "SceneManager::closeGlobalMenu::$_20 UlvE_ target_type()", SceneManager_w31_thunks::closeGlobalMenu_20_UlvE_::targetType);
