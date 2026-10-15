// SceneActionMap: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneActionMap_g5thunks {

// SceneActionMap::battleStartTutorial::$_42::lambda()
using battleStartTutorial_42_lv = thunk::Lambda<0xF5E284, 8, 0xF96F34, 0xE4F440, 4, thunk::cap(4, 4)>;
// SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej)
using battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej = thunk::Lambda<0xF5E308, 8, 0xF96F70, 0xE4F6F0, 4, thunk::cap(4, 4)>;
// SceneActionMap::toLoadingModeSetup::$_40::lambda()
using toLoadingModeSetup_40_lv = thunk::Lambda<0xF5E3B8, 12, 0xF96FC0, 0xE4F9F0, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda()
using toLoadingModeSetup_40_lv_lv = thunk::Lambda<0xF5E3E4, 12, 0xF96FD4, 0xE4FAB0, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneActionMap::makeTalkWindow::$_39::lambda()
using makeTalkWindow_39_lv = thunk::Lambda<0xF5E43C, 8, 0xF96FFC, 0xE4FC00, 4, thunk::cap(4, 4)>;
// SceneActionMap::stageFinishNotification::$_30::lambda()
using stageFinishNotification_30_lv = thunk::Lambda<0xF5E5F4, 8, 0xF970C4, 0xE502F0, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateBattleMode::$_26::lambda()
using updateBattleMode_26_lv = thunk::Lambda<0xF5E6D0, 8, 0xF97128, 0xE50630, 4, thunk::cap(4, 4)>;

}  // namespace SceneActionMap_g5thunks

RW_FUNCTION(0x6030FC, "SceneActionMap::battleStartTutorial::$_42::lambda() ~__func() (deleting)", SceneActionMap_g5thunks::battleStartTutorial_42_lv::deleteThis);
RW_FUNCTION(0x603104, "SceneActionMap::battleStartTutorial::$_42::lambda() __clone()", SceneActionMap_g5thunks::battleStartTutorial_42_lv::clone);
RW_FUNCTION(0x603130, "SceneActionMap::battleStartTutorial::$_42::lambda() __clone(__base*)", SceneActionMap_g5thunks::battleStartTutorial_42_lv::cloneInto);
RW_FUNCTION(0x603150, "SceneActionMap::battleStartTutorial::$_42::lambda() destroy()", SceneActionMap_g5thunks::battleStartTutorial_42_lv::destroy);
RW_FUNCTION(0x603152, "SceneActionMap::battleStartTutorial::$_42::lambda() destroy_deallocate()", SceneActionMap_g5thunks::battleStartTutorial_42_lv::deleteThis);
RW_FUNCTION(0x603174, "SceneActionMap::battleStartTutorial::$_42::lambda() target(const type_info&)", SceneActionMap_g5thunks::battleStartTutorial_42_lv::target);
RW_FUNCTION(0x603194, "SceneActionMap::battleStartTutorial::$_42::lambda() target_type()", SceneActionMap_g5thunks::battleStartTutorial_42_lv::targetType);
RW_FUNCTION(0x603502, "SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej) ~__func() (deleting)", SceneActionMap_g5thunks::battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej::deleteThis);
RW_FUNCTION(0x60350C, "SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej) __clone()", SceneActionMap_g5thunks::battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej::clone);
RW_FUNCTION(0x603538, "SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej) __clone(__base*)", SceneActionMap_g5thunks::battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej::cloneInto);
RW_FUNCTION(0x603558, "SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej) destroy()", SceneActionMap_g5thunks::battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej::destroy);
RW_FUNCTION(0x60355A, "SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej) destroy_deallocate()", SceneActionMap_g5thunks::battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej::deleteThis);
RW_FUNCTION(0x6035A4, "SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej) target(const type_info&)", SceneActionMap_g5thunks::battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej::target);
RW_FUNCTION(0x6035C4, "SceneActionMap::battleStartTutorial::$_45::lambda(P17GestureRecognizerRKN7cocos2d4Vec2Ej) target_type()", SceneActionMap_g5thunks::battleStartTutorial_45_lP17GestureRecognizerRKN7cocos2d4Vec2Ej::targetType);
RW_FUNCTION(0x603A08, "SceneActionMap::toLoadingModeSetup::$_40::lambda() ~__func() (deleting)", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv::deleteThis);
RW_FUNCTION(0x603A10, "SceneActionMap::toLoadingModeSetup::$_40::lambda() __clone()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv::clone);
RW_FUNCTION(0x603A40, "SceneActionMap::toLoadingModeSetup::$_40::lambda() __clone(__base*)", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv::cloneInto);
RW_FUNCTION(0x603A64, "SceneActionMap::toLoadingModeSetup::$_40::lambda() destroy()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv::destroy);
RW_FUNCTION(0x603A66, "SceneActionMap::toLoadingModeSetup::$_40::lambda() destroy_deallocate()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv::deleteThis);
RW_FUNCTION(0x603B10, "SceneActionMap::toLoadingModeSetup::$_40::lambda() target(const type_info&)", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv::target);
RW_FUNCTION(0x603B30, "SceneActionMap::toLoadingModeSetup::$_40::lambda() target_type()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv::targetType);
RW_FUNCTION(0x603B44, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() ~__func() (deleting)", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv_lv::deleteThis);
RW_FUNCTION(0x603B4C, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() __clone()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv_lv::clone);
RW_FUNCTION(0x603B7C, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() __clone(__base*)", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv_lv::cloneInto);
RW_FUNCTION(0x603BA0, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() destroy()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv_lv::destroy);
RW_FUNCTION(0x603BA2, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() destroy_deallocate()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv_lv::deleteThis);
RW_FUNCTION(0x603BD4, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() target(const type_info&)", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv_lv::target);
RW_FUNCTION(0x603BF4, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() target_type()", SceneActionMap_g5thunks::toLoadingModeSetup_40_lv_lv::targetType);
RW_FUNCTION(0x604B00, "SceneActionMap::makeTalkWindow::$_39::lambda() ~__func() (deleting)", SceneActionMap_g5thunks::makeTalkWindow_39_lv::deleteThis);
RW_FUNCTION(0x604B08, "SceneActionMap::makeTalkWindow::$_39::lambda() __clone()", SceneActionMap_g5thunks::makeTalkWindow_39_lv::clone);
RW_FUNCTION(0x604B34, "SceneActionMap::makeTalkWindow::$_39::lambda() __clone(__base*)", SceneActionMap_g5thunks::makeTalkWindow_39_lv::cloneInto);
RW_FUNCTION(0x604B54, "SceneActionMap::makeTalkWindow::$_39::lambda() destroy()", SceneActionMap_g5thunks::makeTalkWindow_39_lv::destroy);
RW_FUNCTION(0x604B56, "SceneActionMap::makeTalkWindow::$_39::lambda() destroy_deallocate()", SceneActionMap_g5thunks::makeTalkWindow_39_lv::deleteThis);
RW_FUNCTION(0x604C5C, "SceneActionMap::makeTalkWindow::$_39::lambda() target(const type_info&)", SceneActionMap_g5thunks::makeTalkWindow_39_lv::target);
RW_FUNCTION(0x604C7C, "SceneActionMap::makeTalkWindow::$_39::lambda() target_type()", SceneActionMap_g5thunks::makeTalkWindow_39_lv::targetType);
RW_FUNCTION(0x605770, "SceneActionMap::stageFinishNotification::$_30::lambda() ~__func() (deleting)", SceneActionMap_g5thunks::stageFinishNotification_30_lv::deleteThis);
RW_FUNCTION(0x605778, "SceneActionMap::stageFinishNotification::$_30::lambda() __clone()", SceneActionMap_g5thunks::stageFinishNotification_30_lv::clone);
RW_FUNCTION(0x6057A4, "SceneActionMap::stageFinishNotification::$_30::lambda() __clone(__base*)", SceneActionMap_g5thunks::stageFinishNotification_30_lv::cloneInto);
RW_FUNCTION(0x6057C4, "SceneActionMap::stageFinishNotification::$_30::lambda() destroy()", SceneActionMap_g5thunks::stageFinishNotification_30_lv::destroy);
RW_FUNCTION(0x6057C6, "SceneActionMap::stageFinishNotification::$_30::lambda() destroy_deallocate()", SceneActionMap_g5thunks::stageFinishNotification_30_lv::deleteThis);
RW_FUNCTION(0x6057D8, "SceneActionMap::stageFinishNotification::$_30::lambda() target(const type_info&)", SceneActionMap_g5thunks::stageFinishNotification_30_lv::target);
RW_FUNCTION(0x6057F8, "SceneActionMap::stageFinishNotification::$_30::lambda() target_type()", SceneActionMap_g5thunks::stageFinishNotification_30_lv::targetType);
RW_FUNCTION(0x605C7C, "SceneActionMap::updateBattleMode::$_26::lambda() ~__func() (deleting)", SceneActionMap_g5thunks::updateBattleMode_26_lv::deleteThis);
RW_FUNCTION(0x605C84, "SceneActionMap::updateBattleMode::$_26::lambda() __clone()", SceneActionMap_g5thunks::updateBattleMode_26_lv::clone);
RW_FUNCTION(0x605CB0, "SceneActionMap::updateBattleMode::$_26::lambda() __clone(__base*)", SceneActionMap_g5thunks::updateBattleMode_26_lv::cloneInto);
RW_FUNCTION(0x605CD0, "SceneActionMap::updateBattleMode::$_26::lambda() destroy()", SceneActionMap_g5thunks::updateBattleMode_26_lv::destroy);
RW_FUNCTION(0x605CD2, "SceneActionMap::updateBattleMode::$_26::lambda() destroy_deallocate()", SceneActionMap_g5thunks::updateBattleMode_26_lv::deleteThis);
RW_FUNCTION(0x605CF4, "SceneActionMap::updateBattleMode::$_26::lambda() target(const type_info&)", SceneActionMap_g5thunks::updateBattleMode_26_lv::target);
RW_FUNCTION(0x605D14, "SceneActionMap::updateBattleMode::$_26::lambda() target_type()", SceneActionMap_g5thunks::updateBattleMode_26_lv::targetType);
