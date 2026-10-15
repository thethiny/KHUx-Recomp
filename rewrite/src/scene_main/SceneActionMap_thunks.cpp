// SceneActionMap: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneActionMap_lambdas {

// SceneActionMap::executingTutorial1010::$_50
using executingTutorial1010_50 = thunk::Lambda<0xF5E17C, 8, 0xF96EBC, 0xE4EFD0, 4, thunk::cap(4, 4)>;
// SceneActionMap::executingTutorial1006::$_49
using executingTutorial1006_49 = thunk::Lambda<0xF5E1A8, 8, 0xF96ED0, 0xE4F080, 4, thunk::cap(4, 4)>;
// SceneActionMap::changedMapTutorial::$_48
using changedMapTutorial_48 = thunk::Lambda<0xF5E1D4, 8, 0xF96EE4, 0xE4F130, 4, thunk::cap(4, 4)>;
// SceneActionMap::changedMapTutorial::$_47
using changedMapTutorial_47 = thunk::Lambda<0xF5E200, 8, 0xF96EF8, 0xE4F1E0, 4, thunk::cap(4, 4)>;
// SceneActionMap::stageFinishTutorial::$_46 (captures a std::function at +8, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using stageFinishTutorial_46 = thunk::LambdaFn<0xF5E22C, 32, 0xF96F0C, 0xE4F290, 8, 8, 0x4683CD>;
// SceneActionMap::battleStartTutorial::$_45 (captures a std::function at +40, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using battleStartTutorial_45 = thunk::LambdaFn<0xF5E258, 64, 0xF96F20, 0xE4F360, 8, 40, 0x4683CD, thunk::cap(8, 4), thunk::cap(16, 4), thunk::cap(24, 4), thunk::cap(32, 4)>;
// SceneActionMap::battleStartTutorial::$_42 (captures a std::function at +16, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using battleStartTutorial_42 = thunk::LambdaFn<0xF5E2B0, 40, 0xF96F48, 0xE4F520, 8, 16, 0x4683CD, thunk::cap(8, 4)>;
// SceneActionMap::battleStartTutorial::$_43 (captures a std::function at +24, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using battleStartTutorial_43 = thunk::LambdaFn<0xF5E2DC, 48, 0xF96F5C, 0xE4F5F0, 8, 24, 0x4683CD, thunk::cap(8, 4), thunk::cap(16, 4)>;
// SceneActionMap::battleStartTutorial::$_44 (captures a std::function at +32, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using battleStartTutorial_44 = thunk::LambdaFn<0xF5E334, 56, 0xF96F84, 0xE4F7F0, 8, 32, 0x4683CD, thunk::cap(8, 4), thunk::cap(16, 4), thunk::cap(24, 4)>;
// SceneActionMap::toFieldModeSetup::$_41
using toFieldModeSetup_41 = thunk::Lambda<0xF5E360, 8, 0xF96F98, 0xE4F8B0, 4, thunk::cap(4, 4)>;
// SceneActionMap::toLoadingModeSetup::$_40
using toLoadingModeSetup_40 = thunk::Lambda<0xF5E38C, 12, 0xF96FAC, 0xE4F950, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneActionMap::makeTalkWindow::$_39
using makeTalkWindow_39 = thunk::Lambda<0xF5E410, 8, 0xF96FE8, 0xE4FB60, 4, thunk::cap(4, 4)>;
// SceneActionMap::onTouchEndedTalk::$_38
using onTouchEndedTalk_38 = thunk::Lambda<0xF5E468, 8, 0xF97010, 0xE4FCC0, 4, thunk::cap(4, 4)>;
// SceneActionMap::onTouchEndedTalk::$_37
using onTouchEndedTalk_37 = thunk::Lambda<0xF5E494, 8, 0xF97024, 0xE4FD90, 4, thunk::cap(4, 4)>;
// SceneActionMap::onTouchEndedTalk::$_36
using onTouchEndedTalk_36 = thunk::Lambda<0xF5E4C0, 8, 0xF97038, 0xE4FE60, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateTalkMode::$_35
using updateTalkMode_35 = thunk::Lambda<0xF5E4EC, 8, 0xF9704C, 0xE4FF10, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateTalkMode::$_34
using updateTalkMode_34 = thunk::Lambda<0xF5E518, 8, 0xF97060, 0xE4FFA0, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateIntrludeMode::$_33
using updateIntrludeMode_33 = thunk::Lambda<0xF5E544, 8, 0xF97074, 0xE50040, 4>;
// SceneActionMap::updateIntrludeMode::$_32
using updateIntrludeMode_32 = thunk::Lambda<0xF5E570, 8, 0xF97088, 0xE500E0, 4>;
// SceneActionMap::stageFinishNotification::$_31
using stageFinishNotification_31 = thunk::Lambda<0xF5E59C, 8, 0xF9709C, 0xE50180, 4, thunk::cap(4, 4)>;
// SceneActionMap::stageFinishNotification::$_30
using stageFinishNotification_30 = thunk::Lambda<0xF5E5C8, 8, 0xF970B0, 0xE50230, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateBattleMode::$_29
using updateBattleMode_29 = thunk::Lambda<0xF5E620, 8, 0xF970D8, 0xE503B0, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateBattleMode::$_28
using updateBattleMode_28 = thunk::Lambda<0xF5E64C, 16, 0xF970EC, 0xE50450, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneActionMap::updateBattleMode::$_27
using updateBattleMode_27 = thunk::Lambda<0xF5E678, 8, 0xF97100, 0xE504F0, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateBattleMode::$_26
using updateBattleMode_26 = thunk::Lambda<0xF5E6A4, 8, 0xF97114, 0xE50590, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateBattleMode::$_25
using updateBattleMode_25 = thunk::Lambda<0xF5E6FC, 8, 0xF9713C, 0xE506E0, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateBattleMode::$_24
using updateBattleMode_24 = thunk::Lambda<0xF5E728, 8, 0xF97150, 0xE50780, 4, thunk::cap(4, 4)>;
// SceneActionMap::updateBattleMode::$_23
using updateBattleMode_23 = thunk::Lambda<0xF5E754, 8, 0xF97164, 0xE50820, 4, thunk::cap(4, 4)>;
// SceneActionMap::update::$_22
using update_22 = thunk::Lambda<0xF5E780, 8, 0xF97178, 0xE508B0, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_21
using init_21 = thunk::Lambda<0xF5E7AC, 8, 0xF9718C, 0xE50940, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_20
using init_20 = thunk::Lambda<0xF5E7D8, 8, 0xF971A0, 0xE509F0, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_19
using init_19 = thunk::Lambda<0xF5E804, 8, 0xF971B4, 0xE50A90, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_18
using init_18 = thunk::Lambda<0xF5E830, 8, 0xF971C8, 0xE50B30, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_17
using init_17 = thunk::Lambda<0xF5E85C, 8, 0xF971DC, 0xE50BC0, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_16
using init_16 = thunk::Lambda<0xF5E888, 8, 0xF971F0, 0xE50C40, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_15
using init_15 = thunk::Lambda<0xF5E8B4, 8, 0xF97204, 0xE50CC0, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_14
using init_14 = thunk::Lambda<0xF5E8E0, 8, 0xF97218, 0xE50D40, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_13
using init_13 = thunk::Lambda<0xF5E90C, 8, 0xF9722C, 0xE50DC0, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_12
using init_12 = thunk::Lambda<0xF5E938, 8, 0xF97240, 0xE50E40, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_11
using init_11 = thunk::Lambda<0xF5E964, 8, 0xF97254, 0xE50EE0, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_10
using init_10 = thunk::Lambda<0xF5E990, 8, 0xF97268, 0xE50F60, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_9
using init_9 = thunk::Lambda<0xF5E9BC, 8, 0xF9727C, 0xE50FF0, 4>;
// SceneActionMap::init::$_8
using init_8 = thunk::Lambda<0xF5E9E8, 8, 0xF97290, 0xE51070, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_7
using init_7 = thunk::Lambda<0xF5EA14, 8, 0xF972A4, 0xE51100, 4>;
// SceneActionMap::init::$_6
using init_6 = thunk::Lambda<0xF5EA40, 8, 0xF972B8, 0xE51180, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_5
using init_5 = thunk::Lambda<0xF5EA6C, 8, 0xF972CC, 0xE51200, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_4
using init_4 = thunk::Lambda<0xF5EA98, 8, 0xF972E0, 0xE51280, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_3
using init_3 = thunk::Lambda<0xF5EAC4, 8, 0xF972F4, 0xE51300, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_2
using init_2 = thunk::Lambda<0xF5EAF0, 8, 0xF97308, 0xE51390, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_1
using init_1 = thunk::Lambda<0xF5EB1C, 8, 0xF9731C, 0xE51410, 4, thunk::cap(4, 4)>;
// SceneActionMap::init::$_0
using init_0 = thunk::Lambda<0xF5EB48, 8, 0xF97330, 0xE51490, 4, thunk::cap(4, 4)>;

}  // namespace SceneActionMap_lambdas

RW_FUNCTION(0x602374, "SceneActionMap::executingTutorial1010::$_50 ~__func() (deleting)", SceneActionMap_lambdas::executingTutorial1010_50::deleteThis);
RW_FUNCTION(0x60237C, "SceneActionMap::executingTutorial1010::$_50 __clone()", SceneActionMap_lambdas::executingTutorial1010_50::clone);
RW_FUNCTION(0x6023A8, "SceneActionMap::executingTutorial1010::$_50 __clone(__base*)", SceneActionMap_lambdas::executingTutorial1010_50::cloneInto);
RW_FUNCTION(0x6023C8, "SceneActionMap::executingTutorial1010::$_50 destroy()", SceneActionMap_lambdas::executingTutorial1010_50::destroy);
RW_FUNCTION(0x6023CA, "SceneActionMap::executingTutorial1010::$_50 destroy_deallocate()", SceneActionMap_lambdas::executingTutorial1010_50::deleteThis);
RW_FUNCTION(0x6023E0, "SceneActionMap::executingTutorial1010::$_50 target(const type_info&)", SceneActionMap_lambdas::executingTutorial1010_50::target);
RW_FUNCTION(0x602400, "SceneActionMap::executingTutorial1010::$_50 target_type()", SceneActionMap_lambdas::executingTutorial1010_50::targetType);
RW_FUNCTION(0x602414, "SceneActionMap::executingTutorial1006::$_49 ~__func() (deleting)", SceneActionMap_lambdas::executingTutorial1006_49::deleteThis);
RW_FUNCTION(0x60241C, "SceneActionMap::executingTutorial1006::$_49 __clone()", SceneActionMap_lambdas::executingTutorial1006_49::clone);
RW_FUNCTION(0x602448, "SceneActionMap::executingTutorial1006::$_49 __clone(__base*)", SceneActionMap_lambdas::executingTutorial1006_49::cloneInto);
RW_FUNCTION(0x602468, "SceneActionMap::executingTutorial1006::$_49 destroy()", SceneActionMap_lambdas::executingTutorial1006_49::destroy);
RW_FUNCTION(0x60246A, "SceneActionMap::executingTutorial1006::$_49 destroy_deallocate()", SceneActionMap_lambdas::executingTutorial1006_49::deleteThis);
RW_FUNCTION(0x60247C, "SceneActionMap::executingTutorial1006::$_49 target(const type_info&)", SceneActionMap_lambdas::executingTutorial1006_49::target);
RW_FUNCTION(0x60249C, "SceneActionMap::executingTutorial1006::$_49 target_type()", SceneActionMap_lambdas::executingTutorial1006_49::targetType);
RW_FUNCTION(0x6024B0, "SceneActionMap::changedMapTutorial::$_48 ~__func() (deleting)", SceneActionMap_lambdas::changedMapTutorial_48::deleteThis);
RW_FUNCTION(0x6024B8, "SceneActionMap::changedMapTutorial::$_48 __clone()", SceneActionMap_lambdas::changedMapTutorial_48::clone);
RW_FUNCTION(0x6024E4, "SceneActionMap::changedMapTutorial::$_48 __clone(__base*)", SceneActionMap_lambdas::changedMapTutorial_48::cloneInto);
RW_FUNCTION(0x602504, "SceneActionMap::changedMapTutorial::$_48 destroy()", SceneActionMap_lambdas::changedMapTutorial_48::destroy);
RW_FUNCTION(0x602506, "SceneActionMap::changedMapTutorial::$_48 destroy_deallocate()", SceneActionMap_lambdas::changedMapTutorial_48::deleteThis);
RW_FUNCTION(0x602518, "SceneActionMap::changedMapTutorial::$_48 target(const type_info&)", SceneActionMap_lambdas::changedMapTutorial_48::target);
RW_FUNCTION(0x602538, "SceneActionMap::changedMapTutorial::$_48 target_type()", SceneActionMap_lambdas::changedMapTutorial_48::targetType);
RW_FUNCTION(0x60254C, "SceneActionMap::changedMapTutorial::$_47 ~__func()", SceneActionMap_lambdas::changedMapTutorial_47::destructor);
RW_FUNCTION(0x60254E, "SceneActionMap::changedMapTutorial::$_47 ~__func() (deleting)", SceneActionMap_lambdas::changedMapTutorial_47::deleteThis);
RW_FUNCTION(0x602558, "SceneActionMap::changedMapTutorial::$_47 __clone()", SceneActionMap_lambdas::changedMapTutorial_47::clone);
RW_FUNCTION(0x602584, "SceneActionMap::changedMapTutorial::$_47 __clone(__base*)", SceneActionMap_lambdas::changedMapTutorial_47::cloneInto);
RW_FUNCTION(0x6025A4, "SceneActionMap::changedMapTutorial::$_47 destroy()", SceneActionMap_lambdas::changedMapTutorial_47::destroy);
RW_FUNCTION(0x6025A6, "SceneActionMap::changedMapTutorial::$_47 destroy_deallocate()", SceneActionMap_lambdas::changedMapTutorial_47::deleteThis);
RW_FUNCTION(0x6026A0, "SceneActionMap::changedMapTutorial::$_47 target(const type_info&)", SceneActionMap_lambdas::changedMapTutorial_47::target);
RW_FUNCTION(0x6026C0, "SceneActionMap::changedMapTutorial::$_47 target_type()", SceneActionMap_lambdas::changedMapTutorial_47::targetType);
RW_FUNCTION(0x6026D4, "SceneActionMap::stageFinishTutorial::$_46 ~__func()", SceneActionMap_lambdas::stageFinishTutorial_46::destructor);
RW_FUNCTION(0x602710, "SceneActionMap::stageFinishTutorial::$_46 ~__func() (deleting)", SceneActionMap_lambdas::stageFinishTutorial_46::destructorDelete);
RW_FUNCTION(0x602750, "SceneActionMap::stageFinishTutorial::$_46 __clone()", SceneActionMap_lambdas::stageFinishTutorial_46::clone);
RW_FUNCTION(0x6027A0, "SceneActionMap::stageFinishTutorial::$_46 __clone(__base*)", SceneActionMap_lambdas::stageFinishTutorial_46::cloneInto);
RW_FUNCTION(0x6027D4, "SceneActionMap::stageFinishTutorial::$_46 destroy()", SceneActionMap_lambdas::stageFinishTutorial_46::destroy);
RW_FUNCTION(0x6027F4, "SceneActionMap::stageFinishTutorial::$_46 destroy_deallocate()", SceneActionMap_lambdas::stageFinishTutorial_46::destroyDeallocate);
RW_FUNCTION(0x60282C, "SceneActionMap::stageFinishTutorial::$_46 target(const type_info&)", SceneActionMap_lambdas::stageFinishTutorial_46::target);
RW_FUNCTION(0x60284C, "SceneActionMap::stageFinishTutorial::$_46 target_type()", SceneActionMap_lambdas::stageFinishTutorial_46::targetType);
RW_FUNCTION(0x602860, "SceneActionMap::battleStartTutorial::$_45 ~__func()", SceneActionMap_lambdas::battleStartTutorial_45::destructor);
RW_FUNCTION(0x60289C, "SceneActionMap::battleStartTutorial::$_45 ~__func() (deleting)", SceneActionMap_lambdas::battleStartTutorial_45::destructorDelete);
RW_FUNCTION(0x6028DC, "SceneActionMap::battleStartTutorial::$_45 __clone()", SceneActionMap_lambdas::battleStartTutorial_45::clone);
RW_FUNCTION(0x60294C, "SceneActionMap::battleStartTutorial::$_45 __clone(__base*)", SceneActionMap_lambdas::battleStartTutorial_45::cloneInto);
RW_FUNCTION(0x6029A4, "SceneActionMap::battleStartTutorial::$_45 destroy()", SceneActionMap_lambdas::battleStartTutorial_45::destroy);
RW_FUNCTION(0x6029C4, "SceneActionMap::battleStartTutorial::$_45 destroy_deallocate()", SceneActionMap_lambdas::battleStartTutorial_45::destroyDeallocate);
RW_FUNCTION(0x602C4C, "SceneActionMap::battleStartTutorial::$_45 target(const type_info&)", SceneActionMap_lambdas::battleStartTutorial_45::target);
RW_FUNCTION(0x602C70, "SceneActionMap::battleStartTutorial::$_45 target_type()", SceneActionMap_lambdas::battleStartTutorial_45::targetType);
RW_FUNCTION(0x6031A8, "SceneActionMap::battleStartTutorial::$_42 ~__func()", SceneActionMap_lambdas::battleStartTutorial_42::destructor);
RW_FUNCTION(0x6031E4, "SceneActionMap::battleStartTutorial::$_42 ~__func() (deleting)", SceneActionMap_lambdas::battleStartTutorial_42::destructorDelete);
RW_FUNCTION(0x603224, "SceneActionMap::battleStartTutorial::$_42 __clone()", SceneActionMap_lambdas::battleStartTutorial_42::clone);
RW_FUNCTION(0x60327C, "SceneActionMap::battleStartTutorial::$_42 __clone(__base*)", SceneActionMap_lambdas::battleStartTutorial_42::cloneInto);
RW_FUNCTION(0x6032BC, "SceneActionMap::battleStartTutorial::$_42 destroy()", SceneActionMap_lambdas::battleStartTutorial_42::destroy);
RW_FUNCTION(0x6032DC, "SceneActionMap::battleStartTutorial::$_42 destroy_deallocate()", SceneActionMap_lambdas::battleStartTutorial_42::destroyDeallocate);
RW_FUNCTION(0x603314, "SceneActionMap::battleStartTutorial::$_42 target(const type_info&)", SceneActionMap_lambdas::battleStartTutorial_42::target);
RW_FUNCTION(0x603338, "SceneActionMap::battleStartTutorial::$_42 target_type()", SceneActionMap_lambdas::battleStartTutorial_42::targetType);
RW_FUNCTION(0x60334C, "SceneActionMap::battleStartTutorial::$_43 ~__func()", SceneActionMap_lambdas::battleStartTutorial_43::destructor);
RW_FUNCTION(0x603388, "SceneActionMap::battleStartTutorial::$_43 ~__func() (deleting)", SceneActionMap_lambdas::battleStartTutorial_43::destructorDelete);
RW_FUNCTION(0x6033C8, "SceneActionMap::battleStartTutorial::$_43 __clone()", SceneActionMap_lambdas::battleStartTutorial_43::clone);
RW_FUNCTION(0x603428, "SceneActionMap::battleStartTutorial::$_43 __clone(__base*)", SceneActionMap_lambdas::battleStartTutorial_43::cloneInto);
RW_FUNCTION(0x603470, "SceneActionMap::battleStartTutorial::$_43 destroy()", SceneActionMap_lambdas::battleStartTutorial_43::destroy);
RW_FUNCTION(0x603490, "SceneActionMap::battleStartTutorial::$_43 destroy_deallocate()", SceneActionMap_lambdas::battleStartTutorial_43::destroyDeallocate);
RW_FUNCTION(0x6034C8, "SceneActionMap::battleStartTutorial::$_43 target(const type_info&)", SceneActionMap_lambdas::battleStartTutorial_43::target);
RW_FUNCTION(0x6034EC, "SceneActionMap::battleStartTutorial::$_43 target_type()", SceneActionMap_lambdas::battleStartTutorial_43::targetType);
RW_FUNCTION(0x6035D8, "SceneActionMap::battleStartTutorial::$_44 ~__func()", SceneActionMap_lambdas::battleStartTutorial_44::destructor);
RW_FUNCTION(0x603614, "SceneActionMap::battleStartTutorial::$_44 ~__func() (deleting)", SceneActionMap_lambdas::battleStartTutorial_44::destructorDelete);
RW_FUNCTION(0x603654, "SceneActionMap::battleStartTutorial::$_44 __clone()", SceneActionMap_lambdas::battleStartTutorial_44::clone);
RW_FUNCTION(0x6036BC, "SceneActionMap::battleStartTutorial::$_44 __clone(__base*)", SceneActionMap_lambdas::battleStartTutorial_44::cloneInto);
RW_FUNCTION(0x60370C, "SceneActionMap::battleStartTutorial::$_44 destroy()", SceneActionMap_lambdas::battleStartTutorial_44::destroy);
RW_FUNCTION(0x60372C, "SceneActionMap::battleStartTutorial::$_44 destroy_deallocate()", SceneActionMap_lambdas::battleStartTutorial_44::destroyDeallocate);
RW_FUNCTION(0x603764, "SceneActionMap::battleStartTutorial::$_44 target(const type_info&)", SceneActionMap_lambdas::battleStartTutorial_44::target);
RW_FUNCTION(0x603788, "SceneActionMap::battleStartTutorial::$_44 target_type()", SceneActionMap_lambdas::battleStartTutorial_44::targetType);
RW_FUNCTION(0x60379C, "SceneActionMap::toFieldModeSetup::$_41 ~__func() (deleting)", SceneActionMap_lambdas::toFieldModeSetup_41::deleteThis);
RW_FUNCTION(0x6037A4, "SceneActionMap::toFieldModeSetup::$_41 __clone()", SceneActionMap_lambdas::toFieldModeSetup_41::clone);
RW_FUNCTION(0x6037D0, "SceneActionMap::toFieldModeSetup::$_41 __clone(__base*)", SceneActionMap_lambdas::toFieldModeSetup_41::cloneInto);
RW_FUNCTION(0x6037F0, "SceneActionMap::toFieldModeSetup::$_41 destroy()", SceneActionMap_lambdas::toFieldModeSetup_41::destroy);
RW_FUNCTION(0x6037F2, "SceneActionMap::toFieldModeSetup::$_41 destroy_deallocate()", SceneActionMap_lambdas::toFieldModeSetup_41::deleteThis);
RW_FUNCTION(0x60380C, "SceneActionMap::toFieldModeSetup::$_41 target(const type_info&)", SceneActionMap_lambdas::toFieldModeSetup_41::target);
RW_FUNCTION(0x60382C, "SceneActionMap::toFieldModeSetup::$_41 target_type()", SceneActionMap_lambdas::toFieldModeSetup_41::targetType);
RW_FUNCTION(0x603840, "SceneActionMap::toLoadingModeSetup::$_40 ~__func() (deleting)", SceneActionMap_lambdas::toLoadingModeSetup_40::deleteThis);
RW_FUNCTION(0x603848, "SceneActionMap::toLoadingModeSetup::$_40 __clone()", SceneActionMap_lambdas::toLoadingModeSetup_40::clone);
RW_FUNCTION(0x603878, "SceneActionMap::toLoadingModeSetup::$_40 __clone(__base*)", SceneActionMap_lambdas::toLoadingModeSetup_40::cloneInto);
RW_FUNCTION(0x60389C, "SceneActionMap::toLoadingModeSetup::$_40 destroy()", SceneActionMap_lambdas::toLoadingModeSetup_40::destroy);
RW_FUNCTION(0x60389E, "SceneActionMap::toLoadingModeSetup::$_40 destroy_deallocate()", SceneActionMap_lambdas::toLoadingModeSetup_40::deleteThis);
RW_FUNCTION(0x6039D4, "SceneActionMap::toLoadingModeSetup::$_40 target(const type_info&)", SceneActionMap_lambdas::toLoadingModeSetup_40::target);
RW_FUNCTION(0x6039F4, "SceneActionMap::toLoadingModeSetup::$_40 target_type()", SceneActionMap_lambdas::toLoadingModeSetup_40::targetType);
RW_FUNCTION(0x604924, "SceneActionMap::makeTalkWindow::$_39 ~__func() (deleting)", SceneActionMap_lambdas::makeTalkWindow_39::deleteThis);
RW_FUNCTION(0x60492C, "SceneActionMap::makeTalkWindow::$_39 __clone()", SceneActionMap_lambdas::makeTalkWindow_39::clone);
RW_FUNCTION(0x604958, "SceneActionMap::makeTalkWindow::$_39 __clone(__base*)", SceneActionMap_lambdas::makeTalkWindow_39::cloneInto);
RW_FUNCTION(0x604978, "SceneActionMap::makeTalkWindow::$_39 destroy()", SceneActionMap_lambdas::makeTalkWindow_39::destroy);
RW_FUNCTION(0x60497A, "SceneActionMap::makeTalkWindow::$_39 destroy_deallocate()", SceneActionMap_lambdas::makeTalkWindow_39::deleteThis);
RW_FUNCTION(0x604ACC, "SceneActionMap::makeTalkWindow::$_39 target(const type_info&)", SceneActionMap_lambdas::makeTalkWindow_39::target);
RW_FUNCTION(0x604AEC, "SceneActionMap::makeTalkWindow::$_39 target_type()", SceneActionMap_lambdas::makeTalkWindow_39::targetType);
RW_FUNCTION(0x604C90, "SceneActionMap::onTouchEndedTalk::$_38 ~__func() (deleting)", SceneActionMap_lambdas::onTouchEndedTalk_38::deleteThis);
RW_FUNCTION(0x604C98, "SceneActionMap::onTouchEndedTalk::$_38 __clone()", SceneActionMap_lambdas::onTouchEndedTalk_38::clone);
RW_FUNCTION(0x604CC4, "SceneActionMap::onTouchEndedTalk::$_38 __clone(__base*)", SceneActionMap_lambdas::onTouchEndedTalk_38::cloneInto);
RW_FUNCTION(0x604CE4, "SceneActionMap::onTouchEndedTalk::$_38 destroy()", SceneActionMap_lambdas::onTouchEndedTalk_38::destroy);
RW_FUNCTION(0x604CE6, "SceneActionMap::onTouchEndedTalk::$_38 destroy_deallocate()", SceneActionMap_lambdas::onTouchEndedTalk_38::deleteThis);
RW_FUNCTION(0x604D10, "SceneActionMap::onTouchEndedTalk::$_38 target(const type_info&)", SceneActionMap_lambdas::onTouchEndedTalk_38::target);
RW_FUNCTION(0x604D30, "SceneActionMap::onTouchEndedTalk::$_38 target_type()", SceneActionMap_lambdas::onTouchEndedTalk_38::targetType);
RW_FUNCTION(0x604D44, "SceneActionMap::onTouchEndedTalk::$_37 ~__func() (deleting)", SceneActionMap_lambdas::onTouchEndedTalk_37::deleteThis);
RW_FUNCTION(0x604D4C, "SceneActionMap::onTouchEndedTalk::$_37 __clone()", SceneActionMap_lambdas::onTouchEndedTalk_37::clone);
RW_FUNCTION(0x604D78, "SceneActionMap::onTouchEndedTalk::$_37 __clone(__base*)", SceneActionMap_lambdas::onTouchEndedTalk_37::cloneInto);
RW_FUNCTION(0x604D98, "SceneActionMap::onTouchEndedTalk::$_37 destroy()", SceneActionMap_lambdas::onTouchEndedTalk_37::destroy);
RW_FUNCTION(0x604D9A, "SceneActionMap::onTouchEndedTalk::$_37 destroy_deallocate()", SceneActionMap_lambdas::onTouchEndedTalk_37::deleteThis);
RW_FUNCTION(0x604DC4, "SceneActionMap::onTouchEndedTalk::$_37 target(const type_info&)", SceneActionMap_lambdas::onTouchEndedTalk_37::target);
RW_FUNCTION(0x604DE4, "SceneActionMap::onTouchEndedTalk::$_37 target_type()", SceneActionMap_lambdas::onTouchEndedTalk_37::targetType);
RW_FUNCTION(0x604DF8, "SceneActionMap::onTouchEndedTalk::$_36 ~__func() (deleting)", SceneActionMap_lambdas::onTouchEndedTalk_36::deleteThis);
RW_FUNCTION(0x604E00, "SceneActionMap::onTouchEndedTalk::$_36 __clone()", SceneActionMap_lambdas::onTouchEndedTalk_36::clone);
RW_FUNCTION(0x604E2C, "SceneActionMap::onTouchEndedTalk::$_36 __clone(__base*)", SceneActionMap_lambdas::onTouchEndedTalk_36::cloneInto);
RW_FUNCTION(0x604E4C, "SceneActionMap::onTouchEndedTalk::$_36 destroy()", SceneActionMap_lambdas::onTouchEndedTalk_36::destroy);
RW_FUNCTION(0x604E4E, "SceneActionMap::onTouchEndedTalk::$_36 destroy_deallocate()", SceneActionMap_lambdas::onTouchEndedTalk_36::deleteThis);
RW_FUNCTION(0x604EAC, "SceneActionMap::onTouchEndedTalk::$_36 target(const type_info&)", SceneActionMap_lambdas::onTouchEndedTalk_36::target);
RW_FUNCTION(0x604ECC, "SceneActionMap::onTouchEndedTalk::$_36 target_type()", SceneActionMap_lambdas::onTouchEndedTalk_36::targetType);
RW_FUNCTION(0x605132, "SceneActionMap::updateTalkMode::$_35 ~__func() (deleting)", SceneActionMap_lambdas::updateTalkMode_35::deleteThis);
RW_FUNCTION(0x60513C, "SceneActionMap::updateTalkMode::$_35 __clone()", SceneActionMap_lambdas::updateTalkMode_35::clone);
RW_FUNCTION(0x605168, "SceneActionMap::updateTalkMode::$_35 __clone(__base*)", SceneActionMap_lambdas::updateTalkMode_35::cloneInto);
RW_FUNCTION(0x605188, "SceneActionMap::updateTalkMode::$_35 destroy()", SceneActionMap_lambdas::updateTalkMode_35::destroy);
RW_FUNCTION(0x60518A, "SceneActionMap::updateTalkMode::$_35 destroy_deallocate()", SceneActionMap_lambdas::updateTalkMode_35::deleteThis);
RW_FUNCTION(0x6051B0, "SceneActionMap::updateTalkMode::$_35 target(const type_info&)", SceneActionMap_lambdas::updateTalkMode_35::target);
RW_FUNCTION(0x6051D0, "SceneActionMap::updateTalkMode::$_35 target_type()", SceneActionMap_lambdas::updateTalkMode_35::targetType);
RW_FUNCTION(0x6051E4, "SceneActionMap::updateTalkMode::$_34 ~__func() (deleting)", SceneActionMap_lambdas::updateTalkMode_34::deleteThis);
RW_FUNCTION(0x6051EC, "SceneActionMap::updateTalkMode::$_34 __clone()", SceneActionMap_lambdas::updateTalkMode_34::clone);
RW_FUNCTION(0x605218, "SceneActionMap::updateTalkMode::$_34 __clone(__base*)", SceneActionMap_lambdas::updateTalkMode_34::cloneInto);
RW_FUNCTION(0x605238, "SceneActionMap::updateTalkMode::$_34 destroy()", SceneActionMap_lambdas::updateTalkMode_34::destroy);
RW_FUNCTION(0x60523A, "SceneActionMap::updateTalkMode::$_34 destroy_deallocate()", SceneActionMap_lambdas::updateTalkMode_34::deleteThis);
RW_FUNCTION(0x605250, "SceneActionMap::updateTalkMode::$_34 target(const type_info&)", SceneActionMap_lambdas::updateTalkMode_34::target);
RW_FUNCTION(0x605270, "SceneActionMap::updateTalkMode::$_34 target_type()", SceneActionMap_lambdas::updateTalkMode_34::targetType);
RW_FUNCTION(0x6052E4, "SceneActionMap::updateIntrludeMode::$_33 ~__func() (deleting)", SceneActionMap_lambdas::updateIntrludeMode_33::deleteThis);
RW_FUNCTION(0x6052EC, "SceneActionMap::updateIntrludeMode::$_33 __clone()", SceneActionMap_lambdas::updateIntrludeMode_33::clone);
RW_FUNCTION(0x605310, "SceneActionMap::updateIntrludeMode::$_33 __clone(__base*)", SceneActionMap_lambdas::updateIntrludeMode_33::cloneInto);
RW_FUNCTION(0x60532C, "SceneActionMap::updateIntrludeMode::$_33 destroy()", SceneActionMap_lambdas::updateIntrludeMode_33::destroy);
RW_FUNCTION(0x60532E, "SceneActionMap::updateIntrludeMode::$_33 destroy_deallocate()", SceneActionMap_lambdas::updateIntrludeMode_33::deleteThis);
RW_FUNCTION(0x6053BC, "SceneActionMap::updateIntrludeMode::$_33 target(const type_info&)", SceneActionMap_lambdas::updateIntrludeMode_33::target);
RW_FUNCTION(0x6053DC, "SceneActionMap::updateIntrludeMode::$_33 target_type()", SceneActionMap_lambdas::updateIntrludeMode_33::targetType);
RW_FUNCTION(0x605480, "SceneActionMap::updateIntrludeMode::$_32 ~__func() (deleting)", SceneActionMap_lambdas::updateIntrludeMode_32::deleteThis);
RW_FUNCTION(0x605488, "SceneActionMap::updateIntrludeMode::$_32 __clone()", SceneActionMap_lambdas::updateIntrludeMode_32::clone);
RW_FUNCTION(0x6054AC, "SceneActionMap::updateIntrludeMode::$_32 __clone(__base*)", SceneActionMap_lambdas::updateIntrludeMode_32::cloneInto);
RW_FUNCTION(0x6054C8, "SceneActionMap::updateIntrludeMode::$_32 destroy()", SceneActionMap_lambdas::updateIntrludeMode_32::destroy);
RW_FUNCTION(0x6054CA, "SceneActionMap::updateIntrludeMode::$_32 destroy_deallocate()", SceneActionMap_lambdas::updateIntrludeMode_32::deleteThis);
RW_FUNCTION(0x605564, "SceneActionMap::updateIntrludeMode::$_32 target(const type_info&)", SceneActionMap_lambdas::updateIntrludeMode_32::target);
RW_FUNCTION(0x605584, "SceneActionMap::updateIntrludeMode::$_32 target_type()", SceneActionMap_lambdas::updateIntrludeMode_32::targetType);
RW_FUNCTION(0x605598, "SceneActionMap::stageFinishNotification::$_31 ~__func() (deleting)", SceneActionMap_lambdas::stageFinishNotification_31::deleteThis);
RW_FUNCTION(0x6055A0, "SceneActionMap::stageFinishNotification::$_31 __clone()", SceneActionMap_lambdas::stageFinishNotification_31::clone);
RW_FUNCTION(0x6055CC, "SceneActionMap::stageFinishNotification::$_31 __clone(__base*)", SceneActionMap_lambdas::stageFinishNotification_31::cloneInto);
RW_FUNCTION(0x6055EC, "SceneActionMap::stageFinishNotification::$_31 destroy()", SceneActionMap_lambdas::stageFinishNotification_31::destroy);
RW_FUNCTION(0x6055EE, "SceneActionMap::stageFinishNotification::$_31 destroy_deallocate()", SceneActionMap_lambdas::stageFinishNotification_31::deleteThis);
RW_FUNCTION(0x605600, "SceneActionMap::stageFinishNotification::$_31 target(const type_info&)", SceneActionMap_lambdas::stageFinishNotification_31::target);
RW_FUNCTION(0x605620, "SceneActionMap::stageFinishNotification::$_31 target_type()", SceneActionMap_lambdas::stageFinishNotification_31::targetType);
RW_FUNCTION(0x605634, "SceneActionMap::stageFinishNotification::$_30 ~__func() (deleting)", SceneActionMap_lambdas::stageFinishNotification_30::deleteThis);
RW_FUNCTION(0x60563C, "SceneActionMap::stageFinishNotification::$_30 __clone()", SceneActionMap_lambdas::stageFinishNotification_30::clone);
RW_FUNCTION(0x605668, "SceneActionMap::stageFinishNotification::$_30 __clone(__base*)", SceneActionMap_lambdas::stageFinishNotification_30::cloneInto);
RW_FUNCTION(0x605688, "SceneActionMap::stageFinishNotification::$_30 destroy()", SceneActionMap_lambdas::stageFinishNotification_30::destroy);
RW_FUNCTION(0x60568A, "SceneActionMap::stageFinishNotification::$_30 destroy_deallocate()", SceneActionMap_lambdas::stageFinishNotification_30::deleteThis);
RW_FUNCTION(0x60573C, "SceneActionMap::stageFinishNotification::$_30 target(const type_info&)", SceneActionMap_lambdas::stageFinishNotification_30::target);
RW_FUNCTION(0x60575C, "SceneActionMap::stageFinishNotification::$_30 target_type()", SceneActionMap_lambdas::stageFinishNotification_30::targetType);
RW_FUNCTION(0x60580C, "SceneActionMap::updateBattleMode::$_29 ~__func() (deleting)", SceneActionMap_lambdas::updateBattleMode_29::deleteThis);
RW_FUNCTION(0x605814, "SceneActionMap::updateBattleMode::$_29 __clone()", SceneActionMap_lambdas::updateBattleMode_29::clone);
RW_FUNCTION(0x605840, "SceneActionMap::updateBattleMode::$_29 __clone(__base*)", SceneActionMap_lambdas::updateBattleMode_29::cloneInto);
RW_FUNCTION(0x605860, "SceneActionMap::updateBattleMode::$_29 destroy()", SceneActionMap_lambdas::updateBattleMode_29::destroy);
RW_FUNCTION(0x605862, "SceneActionMap::updateBattleMode::$_29 destroy_deallocate()", SceneActionMap_lambdas::updateBattleMode_29::deleteThis);
RW_FUNCTION(0x605874, "SceneActionMap::updateBattleMode::$_29 target(const type_info&)", SceneActionMap_lambdas::updateBattleMode_29::target);
RW_FUNCTION(0x605894, "SceneActionMap::updateBattleMode::$_29 target_type()", SceneActionMap_lambdas::updateBattleMode_29::targetType);
RW_FUNCTION(0x6058A8, "SceneActionMap::updateBattleMode::$_28 ~__func() (deleting)", SceneActionMap_lambdas::updateBattleMode_28::deleteThis);
RW_FUNCTION(0x6058B0, "SceneActionMap::updateBattleMode::$_28 __clone()", SceneActionMap_lambdas::updateBattleMode_28::clone);
RW_FUNCTION(0x6058E0, "SceneActionMap::updateBattleMode::$_28 __clone(__base*)", SceneActionMap_lambdas::updateBattleMode_28::cloneInto);
RW_FUNCTION(0x605908, "SceneActionMap::updateBattleMode::$_28 destroy()", SceneActionMap_lambdas::updateBattleMode_28::destroy);
RW_FUNCTION(0x60590A, "SceneActionMap::updateBattleMode::$_28 destroy_deallocate()", SceneActionMap_lambdas::updateBattleMode_28::deleteThis);
RW_FUNCTION(0x6059F8, "SceneActionMap::updateBattleMode::$_28 target(const type_info&)", SceneActionMap_lambdas::updateBattleMode_28::target);
RW_FUNCTION(0x605A18, "SceneActionMap::updateBattleMode::$_28 target_type()", SceneActionMap_lambdas::updateBattleMode_28::targetType);
RW_FUNCTION(0x605A2C, "SceneActionMap::updateBattleMode::$_27 ~__func() (deleting)", SceneActionMap_lambdas::updateBattleMode_27::deleteThis);
RW_FUNCTION(0x605A34, "SceneActionMap::updateBattleMode::$_27 __clone()", SceneActionMap_lambdas::updateBattleMode_27::clone);
RW_FUNCTION(0x605A60, "SceneActionMap::updateBattleMode::$_27 __clone(__base*)", SceneActionMap_lambdas::updateBattleMode_27::cloneInto);
RW_FUNCTION(0x605A80, "SceneActionMap::updateBattleMode::$_27 destroy()", SceneActionMap_lambdas::updateBattleMode_27::destroy);
RW_FUNCTION(0x605A82, "SceneActionMap::updateBattleMode::$_27 destroy_deallocate()", SceneActionMap_lambdas::updateBattleMode_27::deleteThis);
RW_FUNCTION(0x605AF0, "SceneActionMap::updateBattleMode::$_27 target(const type_info&)", SceneActionMap_lambdas::updateBattleMode_27::target);
RW_FUNCTION(0x605B10, "SceneActionMap::updateBattleMode::$_27 target_type()", SceneActionMap_lambdas::updateBattleMode_27::targetType);
RW_FUNCTION(0x605B24, "SceneActionMap::updateBattleMode::$_26 ~__func() (deleting)", SceneActionMap_lambdas::updateBattleMode_26::deleteThis);
RW_FUNCTION(0x605B2C, "SceneActionMap::updateBattleMode::$_26 __clone()", SceneActionMap_lambdas::updateBattleMode_26::clone);
RW_FUNCTION(0x605B58, "SceneActionMap::updateBattleMode::$_26 __clone(__base*)", SceneActionMap_lambdas::updateBattleMode_26::cloneInto);
RW_FUNCTION(0x605B78, "SceneActionMap::updateBattleMode::$_26 destroy()", SceneActionMap_lambdas::updateBattleMode_26::destroy);
RW_FUNCTION(0x605B7A, "SceneActionMap::updateBattleMode::$_26 destroy_deallocate()", SceneActionMap_lambdas::updateBattleMode_26::deleteThis);
RW_FUNCTION(0x605C48, "SceneActionMap::updateBattleMode::$_26 target(const type_info&)", SceneActionMap_lambdas::updateBattleMode_26::target);
RW_FUNCTION(0x605C68, "SceneActionMap::updateBattleMode::$_26 target_type()", SceneActionMap_lambdas::updateBattleMode_26::targetType);
RW_FUNCTION(0x605D28, "SceneActionMap::updateBattleMode::$_25 ~__func() (deleting)", SceneActionMap_lambdas::updateBattleMode_25::deleteThis);
RW_FUNCTION(0x605D30, "SceneActionMap::updateBattleMode::$_25 __clone()", SceneActionMap_lambdas::updateBattleMode_25::clone);
RW_FUNCTION(0x605D5C, "SceneActionMap::updateBattleMode::$_25 __clone(__base*)", SceneActionMap_lambdas::updateBattleMode_25::cloneInto);
RW_FUNCTION(0x605D7C, "SceneActionMap::updateBattleMode::$_25 destroy()", SceneActionMap_lambdas::updateBattleMode_25::destroy);
RW_FUNCTION(0x605D7E, "SceneActionMap::updateBattleMode::$_25 destroy_deallocate()", SceneActionMap_lambdas::updateBattleMode_25::deleteThis);
RW_FUNCTION(0x605D9C, "SceneActionMap::updateBattleMode::$_25 target(const type_info&)", SceneActionMap_lambdas::updateBattleMode_25::target);
RW_FUNCTION(0x605DBC, "SceneActionMap::updateBattleMode::$_25 target_type()", SceneActionMap_lambdas::updateBattleMode_25::targetType);
RW_FUNCTION(0x605DD0, "SceneActionMap::updateBattleMode::$_24 ~__func() (deleting)", SceneActionMap_lambdas::updateBattleMode_24::deleteThis);
RW_FUNCTION(0x605DD8, "SceneActionMap::updateBattleMode::$_24 __clone()", SceneActionMap_lambdas::updateBattleMode_24::clone);
RW_FUNCTION(0x605E04, "SceneActionMap::updateBattleMode::$_24 __clone(__base*)", SceneActionMap_lambdas::updateBattleMode_24::cloneInto);
RW_FUNCTION(0x605E24, "SceneActionMap::updateBattleMode::$_24 destroy()", SceneActionMap_lambdas::updateBattleMode_24::destroy);
RW_FUNCTION(0x605E26, "SceneActionMap::updateBattleMode::$_24 destroy_deallocate()", SceneActionMap_lambdas::updateBattleMode_24::deleteThis);
RW_FUNCTION(0x605E38, "SceneActionMap::updateBattleMode::$_24 target(const type_info&)", SceneActionMap_lambdas::updateBattleMode_24::target);
RW_FUNCTION(0x605E58, "SceneActionMap::updateBattleMode::$_24 target_type()", SceneActionMap_lambdas::updateBattleMode_24::targetType);
RW_FUNCTION(0x605E6C, "SceneActionMap::updateBattleMode::$_23 ~__func() (deleting)", SceneActionMap_lambdas::updateBattleMode_23::deleteThis);
RW_FUNCTION(0x605E74, "SceneActionMap::updateBattleMode::$_23 __clone()", SceneActionMap_lambdas::updateBattleMode_23::clone);
RW_FUNCTION(0x605EA0, "SceneActionMap::updateBattleMode::$_23 __clone(__base*)", SceneActionMap_lambdas::updateBattleMode_23::cloneInto);
RW_FUNCTION(0x605EC0, "SceneActionMap::updateBattleMode::$_23 destroy()", SceneActionMap_lambdas::updateBattleMode_23::destroy);
RW_FUNCTION(0x605EC2, "SceneActionMap::updateBattleMode::$_23 destroy_deallocate()", SceneActionMap_lambdas::updateBattleMode_23::deleteThis);
RW_FUNCTION(0x605EF4, "SceneActionMap::updateBattleMode::$_23 target(const type_info&)", SceneActionMap_lambdas::updateBattleMode_23::target);
RW_FUNCTION(0x605F14, "SceneActionMap::updateBattleMode::$_23 target_type()", SceneActionMap_lambdas::updateBattleMode_23::targetType);
RW_FUNCTION(0x605F28, "SceneActionMap::update::$_22 ~__func() (deleting)", SceneActionMap_lambdas::update_22::deleteThis);
RW_FUNCTION(0x605F30, "SceneActionMap::update::$_22 __clone()", SceneActionMap_lambdas::update_22::clone);
RW_FUNCTION(0x605F5C, "SceneActionMap::update::$_22 __clone(__base*)", SceneActionMap_lambdas::update_22::cloneInto);
RW_FUNCTION(0x605F7C, "SceneActionMap::update::$_22 destroy()", SceneActionMap_lambdas::update_22::destroy);
RW_FUNCTION(0x605F7E, "SceneActionMap::update::$_22 destroy_deallocate()", SceneActionMap_lambdas::update_22::deleteThis);
RW_FUNCTION(0x605F98, "SceneActionMap::update::$_22 target(const type_info&)", SceneActionMap_lambdas::update_22::target);
RW_FUNCTION(0x605FB8, "SceneActionMap::update::$_22 target_type()", SceneActionMap_lambdas::update_22::targetType);
RW_FUNCTION(0x605FCC, "SceneActionMap::init::$_21 ~__func() (deleting)", SceneActionMap_lambdas::init_21::deleteThis);
RW_FUNCTION(0x605FD4, "SceneActionMap::init::$_21 __clone()", SceneActionMap_lambdas::init_21::clone);
RW_FUNCTION(0x606000, "SceneActionMap::init::$_21 __clone(__base*)", SceneActionMap_lambdas::init_21::cloneInto);
RW_FUNCTION(0x606020, "SceneActionMap::init::$_21 destroy()", SceneActionMap_lambdas::init_21::destroy);
RW_FUNCTION(0x606022, "SceneActionMap::init::$_21 destroy_deallocate()", SceneActionMap_lambdas::init_21::deleteThis);
RW_FUNCTION(0x606058, "SceneActionMap::init::$_21 target(const type_info&)", SceneActionMap_lambdas::init_21::target);
RW_FUNCTION(0x606078, "SceneActionMap::init::$_21 target_type()", SceneActionMap_lambdas::init_21::targetType);
RW_FUNCTION(0x60608C, "SceneActionMap::init::$_20 ~__func() (deleting)", SceneActionMap_lambdas::init_20::deleteThis);
RW_FUNCTION(0x606094, "SceneActionMap::init::$_20 __clone()", SceneActionMap_lambdas::init_20::clone);
RW_FUNCTION(0x6060C0, "SceneActionMap::init::$_20 __clone(__base*)", SceneActionMap_lambdas::init_20::cloneInto);
RW_FUNCTION(0x6060E0, "SceneActionMap::init::$_20 destroy()", SceneActionMap_lambdas::init_20::destroy);
RW_FUNCTION(0x6060E2, "SceneActionMap::init::$_20 destroy_deallocate()", SceneActionMap_lambdas::init_20::deleteThis);
RW_FUNCTION(0x606104, "SceneActionMap::init::$_20 target(const type_info&)", SceneActionMap_lambdas::init_20::target);
RW_FUNCTION(0x606124, "SceneActionMap::init::$_20 target_type()", SceneActionMap_lambdas::init_20::targetType);
RW_FUNCTION(0x60613A, "SceneActionMap::init::$_19 ~__func() (deleting)", SceneActionMap_lambdas::init_19::deleteThis);
RW_FUNCTION(0x606144, "SceneActionMap::init::$_19 __clone()", SceneActionMap_lambdas::init_19::clone);
RW_FUNCTION(0x606170, "SceneActionMap::init::$_19 __clone(__base*)", SceneActionMap_lambdas::init_19::cloneInto);
RW_FUNCTION(0x606190, "SceneActionMap::init::$_19 destroy()", SceneActionMap_lambdas::init_19::destroy);
RW_FUNCTION(0x606192, "SceneActionMap::init::$_19 destroy_deallocate()", SceneActionMap_lambdas::init_19::deleteThis);
RW_FUNCTION(0x606224, "SceneActionMap::init::$_19 target(const type_info&)", SceneActionMap_lambdas::init_19::target);
RW_FUNCTION(0x606244, "SceneActionMap::init::$_19 target_type()", SceneActionMap_lambdas::init_19::targetType);
RW_FUNCTION(0x606258, "SceneActionMap::init::$_18 ~__func() (deleting)", SceneActionMap_lambdas::init_18::deleteThis);
RW_FUNCTION(0x606260, "SceneActionMap::init::$_18 __clone()", SceneActionMap_lambdas::init_18::clone);
RW_FUNCTION(0x60628C, "SceneActionMap::init::$_18 __clone(__base*)", SceneActionMap_lambdas::init_18::cloneInto);
RW_FUNCTION(0x6062AC, "SceneActionMap::init::$_18 destroy()", SceneActionMap_lambdas::init_18::destroy);
RW_FUNCTION(0x6062AE, "SceneActionMap::init::$_18 destroy_deallocate()", SceneActionMap_lambdas::init_18::deleteThis);
RW_FUNCTION(0x606340, "SceneActionMap::init::$_18 target(const type_info&)", SceneActionMap_lambdas::init_18::target);
RW_FUNCTION(0x606360, "SceneActionMap::init::$_18 target_type()", SceneActionMap_lambdas::init_18::targetType);
RW_FUNCTION(0x606374, "SceneActionMap::init::$_17 ~__func()", SceneActionMap_lambdas::init_17::destructor);
RW_FUNCTION(0x606376, "SceneActionMap::init::$_17 ~__func() (deleting)", SceneActionMap_lambdas::init_17::deleteThis);
RW_FUNCTION(0x606380, "SceneActionMap::init::$_17 __clone()", SceneActionMap_lambdas::init_17::clone);
RW_FUNCTION(0x6063AC, "SceneActionMap::init::$_17 __clone(__base*)", SceneActionMap_lambdas::init_17::cloneInto);
RW_FUNCTION(0x6063CC, "SceneActionMap::init::$_17 destroy()", SceneActionMap_lambdas::init_17::destroy);
RW_FUNCTION(0x6063CE, "SceneActionMap::init::$_17 destroy_deallocate()", SceneActionMap_lambdas::init_17::deleteThis);
RW_FUNCTION(0x6063E8, "SceneActionMap::init::$_17 target(const type_info&)", SceneActionMap_lambdas::init_17::target);
RW_FUNCTION(0x606408, "SceneActionMap::init::$_17 target_type()", SceneActionMap_lambdas::init_17::targetType);
RW_FUNCTION(0x60641C, "SceneActionMap::init::$_16 ~__func() (deleting)", SceneActionMap_lambdas::init_16::deleteThis);
RW_FUNCTION(0x606424, "SceneActionMap::init::$_16 __clone()", SceneActionMap_lambdas::init_16::clone);
RW_FUNCTION(0x606450, "SceneActionMap::init::$_16 __clone(__base*)", SceneActionMap_lambdas::init_16::cloneInto);
RW_FUNCTION(0x606470, "SceneActionMap::init::$_16 destroy()", SceneActionMap_lambdas::init_16::destroy);
RW_FUNCTION(0x606472, "SceneActionMap::init::$_16 destroy_deallocate()", SceneActionMap_lambdas::init_16::deleteThis);
RW_FUNCTION(0x606484, "SceneActionMap::init::$_16 target(const type_info&)", SceneActionMap_lambdas::init_16::target);
RW_FUNCTION(0x6064A4, "SceneActionMap::init::$_16 target_type()", SceneActionMap_lambdas::init_16::targetType);
RW_FUNCTION(0x6064B8, "SceneActionMap::init::$_15 ~__func() (deleting)", SceneActionMap_lambdas::init_15::deleteThis);
RW_FUNCTION(0x6064C0, "SceneActionMap::init::$_15 __clone()", SceneActionMap_lambdas::init_15::clone);
RW_FUNCTION(0x6064EC, "SceneActionMap::init::$_15 __clone(__base*)", SceneActionMap_lambdas::init_15::cloneInto);
RW_FUNCTION(0x60650C, "SceneActionMap::init::$_15 destroy()", SceneActionMap_lambdas::init_15::destroy);
RW_FUNCTION(0x60650E, "SceneActionMap::init::$_15 destroy_deallocate()", SceneActionMap_lambdas::init_15::deleteThis);
RW_FUNCTION(0x60652C, "SceneActionMap::init::$_15 target(const type_info&)", SceneActionMap_lambdas::init_15::target);
RW_FUNCTION(0x60654C, "SceneActionMap::init::$_15 target_type()", SceneActionMap_lambdas::init_15::targetType);
RW_FUNCTION(0x606560, "SceneActionMap::init::$_14 ~__func() (deleting)", SceneActionMap_lambdas::init_14::deleteThis);
RW_FUNCTION(0x606568, "SceneActionMap::init::$_14 __clone()", SceneActionMap_lambdas::init_14::clone);
RW_FUNCTION(0x606594, "SceneActionMap::init::$_14 __clone(__base*)", SceneActionMap_lambdas::init_14::cloneInto);
RW_FUNCTION(0x6065B4, "SceneActionMap::init::$_14 destroy()", SceneActionMap_lambdas::init_14::destroy);
RW_FUNCTION(0x6065B6, "SceneActionMap::init::$_14 destroy_deallocate()", SceneActionMap_lambdas::init_14::deleteThis);
RW_FUNCTION(0x6065EC, "SceneActionMap::init::$_14 target(const type_info&)", SceneActionMap_lambdas::init_14::target);
RW_FUNCTION(0x60660C, "SceneActionMap::init::$_14 target_type()", SceneActionMap_lambdas::init_14::targetType);
RW_FUNCTION(0x606620, "SceneActionMap::init::$_13 ~__func() (deleting)", SceneActionMap_lambdas::init_13::deleteThis);
RW_FUNCTION(0x606628, "SceneActionMap::init::$_13 __clone()", SceneActionMap_lambdas::init_13::clone);
RW_FUNCTION(0x606654, "SceneActionMap::init::$_13 __clone(__base*)", SceneActionMap_lambdas::init_13::cloneInto);
RW_FUNCTION(0x606674, "SceneActionMap::init::$_13 destroy()", SceneActionMap_lambdas::init_13::destroy);
RW_FUNCTION(0x606676, "SceneActionMap::init::$_13 destroy_deallocate()", SceneActionMap_lambdas::init_13::deleteThis);
RW_FUNCTION(0x606688, "SceneActionMap::init::$_13 target(const type_info&)", SceneActionMap_lambdas::init_13::target);
RW_FUNCTION(0x6066A8, "SceneActionMap::init::$_13 target_type()", SceneActionMap_lambdas::init_13::targetType);
RW_FUNCTION(0x6066BC, "SceneActionMap::init::$_12 ~__func() (deleting)", SceneActionMap_lambdas::init_12::deleteThis);
RW_FUNCTION(0x6066C4, "SceneActionMap::init::$_12 __clone()", SceneActionMap_lambdas::init_12::clone);
RW_FUNCTION(0x6066F0, "SceneActionMap::init::$_12 __clone(__base*)", SceneActionMap_lambdas::init_12::cloneInto);
RW_FUNCTION(0x606710, "SceneActionMap::init::$_12 destroy()", SceneActionMap_lambdas::init_12::destroy);
RW_FUNCTION(0x606712, "SceneActionMap::init::$_12 destroy_deallocate()", SceneActionMap_lambdas::init_12::deleteThis);
RW_FUNCTION(0x606724, "SceneActionMap::init::$_12 target(const type_info&)", SceneActionMap_lambdas::init_12::target);
RW_FUNCTION(0x606744, "SceneActionMap::init::$_12 target_type()", SceneActionMap_lambdas::init_12::targetType);
RW_FUNCTION(0x606854, "SceneActionMap::init::$_11 ~__func() (deleting)", SceneActionMap_lambdas::init_11::deleteThis);
RW_FUNCTION(0x60685C, "SceneActionMap::init::$_11 __clone()", SceneActionMap_lambdas::init_11::clone);
RW_FUNCTION(0x606888, "SceneActionMap::init::$_11 __clone(__base*)", SceneActionMap_lambdas::init_11::cloneInto);
RW_FUNCTION(0x6068A8, "SceneActionMap::init::$_11 destroy()", SceneActionMap_lambdas::init_11::destroy);
RW_FUNCTION(0x6068AA, "SceneActionMap::init::$_11 destroy_deallocate()", SceneActionMap_lambdas::init_11::deleteThis);
RW_FUNCTION(0x6068CC, "SceneActionMap::init::$_11 target(const type_info&)", SceneActionMap_lambdas::init_11::target);
RW_FUNCTION(0x6068EC, "SceneActionMap::init::$_11 target_type()", SceneActionMap_lambdas::init_11::targetType);
RW_FUNCTION(0x606A88, "SceneActionMap::init::$_10 ~__func() (deleting)", SceneActionMap_lambdas::init_10::deleteThis);
RW_FUNCTION(0x606A90, "SceneActionMap::init::$_10 __clone()", SceneActionMap_lambdas::init_10::clone);
RW_FUNCTION(0x606ABC, "SceneActionMap::init::$_10 __clone(__base*)", SceneActionMap_lambdas::init_10::cloneInto);
RW_FUNCTION(0x606ADC, "SceneActionMap::init::$_10 destroy()", SceneActionMap_lambdas::init_10::destroy);
RW_FUNCTION(0x606ADE, "SceneActionMap::init::$_10 destroy_deallocate()", SceneActionMap_lambdas::init_10::deleteThis);
RW_FUNCTION(0x606B7C, "SceneActionMap::init::$_10 target(const type_info&)", SceneActionMap_lambdas::init_10::target);
RW_FUNCTION(0x606B9C, "SceneActionMap::init::$_10 target_type()", SceneActionMap_lambdas::init_10::targetType);
RW_FUNCTION(0x606BB0, "SceneActionMap::init::$_9 ~__func() (deleting)", SceneActionMap_lambdas::init_9::deleteThis);
RW_FUNCTION(0x606BB8, "SceneActionMap::init::$_9 __clone()", SceneActionMap_lambdas::init_9::clone);
RW_FUNCTION(0x606BDC, "SceneActionMap::init::$_9 __clone(__base*)", SceneActionMap_lambdas::init_9::cloneInto);
RW_FUNCTION(0x606BF8, "SceneActionMap::init::$_9 destroy()", SceneActionMap_lambdas::init_9::destroy);
RW_FUNCTION(0x606BFA, "SceneActionMap::init::$_9 destroy_deallocate()", SceneActionMap_lambdas::init_9::deleteThis);
RW_FUNCTION(0x606C8C, "SceneActionMap::init::$_9 target(const type_info&)", SceneActionMap_lambdas::init_9::target);
RW_FUNCTION(0x606CAC, "SceneActionMap::init::$_9 target_type()", SceneActionMap_lambdas::init_9::targetType);
RW_FUNCTION(0x606CC0, "SceneActionMap::init::$_8 ~__func() (deleting)", SceneActionMap_lambdas::init_8::deleteThis);
RW_FUNCTION(0x606CC8, "SceneActionMap::init::$_8 __clone()", SceneActionMap_lambdas::init_8::clone);
RW_FUNCTION(0x606CF4, "SceneActionMap::init::$_8 __clone(__base*)", SceneActionMap_lambdas::init_8::cloneInto);
RW_FUNCTION(0x606D14, "SceneActionMap::init::$_8 destroy()", SceneActionMap_lambdas::init_8::destroy);
RW_FUNCTION(0x606D16, "SceneActionMap::init::$_8 destroy_deallocate()", SceneActionMap_lambdas::init_8::deleteThis);
RW_FUNCTION(0x606D2C, "SceneActionMap::init::$_8 target(const type_info&)", SceneActionMap_lambdas::init_8::target);
RW_FUNCTION(0x606D4C, "SceneActionMap::init::$_8 target_type()", SceneActionMap_lambdas::init_8::targetType);
RW_FUNCTION(0x606D62, "SceneActionMap::init::$_7 ~__func() (deleting)", SceneActionMap_lambdas::init_7::deleteThis);
RW_FUNCTION(0x606D6C, "SceneActionMap::init::$_7 __clone()", SceneActionMap_lambdas::init_7::clone);
RW_FUNCTION(0x606D90, "SceneActionMap::init::$_7 __clone(__base*)", SceneActionMap_lambdas::init_7::cloneInto);
RW_FUNCTION(0x606DAC, "SceneActionMap::init::$_7 destroy()", SceneActionMap_lambdas::init_7::destroy);
RW_FUNCTION(0x606DAE, "SceneActionMap::init::$_7 destroy_deallocate()", SceneActionMap_lambdas::init_7::deleteThis);
RW_FUNCTION(0x606E40, "SceneActionMap::init::$_7 target(const type_info&)", SceneActionMap_lambdas::init_7::target);
RW_FUNCTION(0x606E60, "SceneActionMap::init::$_7 target_type()", SceneActionMap_lambdas::init_7::targetType);
RW_FUNCTION(0x606E74, "SceneActionMap::init::$_6 ~__func() (deleting)", SceneActionMap_lambdas::init_6::deleteThis);
RW_FUNCTION(0x606E7C, "SceneActionMap::init::$_6 __clone()", SceneActionMap_lambdas::init_6::clone);
RW_FUNCTION(0x606EA8, "SceneActionMap::init::$_6 __clone(__base*)", SceneActionMap_lambdas::init_6::cloneInto);
RW_FUNCTION(0x606EC8, "SceneActionMap::init::$_6 destroy()", SceneActionMap_lambdas::init_6::destroy);
RW_FUNCTION(0x606ECA, "SceneActionMap::init::$_6 destroy_deallocate()", SceneActionMap_lambdas::init_6::deleteThis);
RW_FUNCTION(0x606EE0, "SceneActionMap::init::$_6 target(const type_info&)", SceneActionMap_lambdas::init_6::target);
RW_FUNCTION(0x606F00, "SceneActionMap::init::$_6 target_type()", SceneActionMap_lambdas::init_6::targetType);
RW_FUNCTION(0x606F14, "SceneActionMap::init::$_5 ~__func() (deleting)", SceneActionMap_lambdas::init_5::deleteThis);
RW_FUNCTION(0x606F1C, "SceneActionMap::init::$_5 __clone()", SceneActionMap_lambdas::init_5::clone);
RW_FUNCTION(0x606F48, "SceneActionMap::init::$_5 __clone(__base*)", SceneActionMap_lambdas::init_5::cloneInto);
RW_FUNCTION(0x606F68, "SceneActionMap::init::$_5 destroy()", SceneActionMap_lambdas::init_5::destroy);
RW_FUNCTION(0x606F6A, "SceneActionMap::init::$_5 destroy_deallocate()", SceneActionMap_lambdas::init_5::deleteThis);
RW_FUNCTION(0x606F88, "SceneActionMap::init::$_5 target(const type_info&)", SceneActionMap_lambdas::init_5::target);
RW_FUNCTION(0x606FA8, "SceneActionMap::init::$_5 target_type()", SceneActionMap_lambdas::init_5::targetType);
RW_FUNCTION(0x606FBC, "SceneActionMap::init::$_4 ~__func() (deleting)", SceneActionMap_lambdas::init_4::deleteThis);
RW_FUNCTION(0x606FC4, "SceneActionMap::init::$_4 __clone()", SceneActionMap_lambdas::init_4::clone);
RW_FUNCTION(0x606FF0, "SceneActionMap::init::$_4 __clone(__base*)", SceneActionMap_lambdas::init_4::cloneInto);
RW_FUNCTION(0x607010, "SceneActionMap::init::$_4 destroy()", SceneActionMap_lambdas::init_4::destroy);
RW_FUNCTION(0x607012, "SceneActionMap::init::$_4 destroy_deallocate()", SceneActionMap_lambdas::init_4::deleteThis);
RW_FUNCTION(0x607028, "SceneActionMap::init::$_4 target(const type_info&)", SceneActionMap_lambdas::init_4::target);
RW_FUNCTION(0x607048, "SceneActionMap::init::$_4 target_type()", SceneActionMap_lambdas::init_4::targetType);
RW_FUNCTION(0x60705E, "SceneActionMap::init::$_3 ~__func() (deleting)", SceneActionMap_lambdas::init_3::deleteThis);
RW_FUNCTION(0x607068, "SceneActionMap::init::$_3 __clone()", SceneActionMap_lambdas::init_3::clone);
RW_FUNCTION(0x607094, "SceneActionMap::init::$_3 __clone(__base*)", SceneActionMap_lambdas::init_3::cloneInto);
RW_FUNCTION(0x6070B4, "SceneActionMap::init::$_3 destroy()", SceneActionMap_lambdas::init_3::destroy);
RW_FUNCTION(0x6070B6, "SceneActionMap::init::$_3 destroy_deallocate()", SceneActionMap_lambdas::init_3::deleteThis);
RW_FUNCTION(0x6070CC, "SceneActionMap::init::$_3 target(const type_info&)", SceneActionMap_lambdas::init_3::target);
RW_FUNCTION(0x6070EC, "SceneActionMap::init::$_3 target_type()", SceneActionMap_lambdas::init_3::targetType);
RW_FUNCTION(0x607100, "SceneActionMap::init::$_2 ~__func()", SceneActionMap_lambdas::init_2::destructor);
RW_FUNCTION(0x607102, "SceneActionMap::init::$_2 ~__func() (deleting)", SceneActionMap_lambdas::init_2::deleteThis);
RW_FUNCTION(0x60710C, "SceneActionMap::init::$_2 __clone()", SceneActionMap_lambdas::init_2::clone);
RW_FUNCTION(0x607138, "SceneActionMap::init::$_2 __clone(__base*)", SceneActionMap_lambdas::init_2::cloneInto);
RW_FUNCTION(0x607158, "SceneActionMap::init::$_2 destroy()", SceneActionMap_lambdas::init_2::destroy);
RW_FUNCTION(0x60715A, "SceneActionMap::init::$_2 destroy_deallocate()", SceneActionMap_lambdas::init_2::deleteThis);
RW_FUNCTION(0x607174, "SceneActionMap::init::$_2 target(const type_info&)", SceneActionMap_lambdas::init_2::target);
RW_FUNCTION(0x607194, "SceneActionMap::init::$_2 target_type()", SceneActionMap_lambdas::init_2::targetType);
RW_FUNCTION(0x6071AA, "SceneActionMap::init::$_1 ~__func() (deleting)", SceneActionMap_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6071B4, "SceneActionMap::init::$_1 __clone()", SceneActionMap_lambdas::init_1::clone);
RW_FUNCTION(0x6071E0, "SceneActionMap::init::$_1 __clone(__base*)", SceneActionMap_lambdas::init_1::cloneInto);
RW_FUNCTION(0x607200, "SceneActionMap::init::$_1 destroy()", SceneActionMap_lambdas::init_1::destroy);
RW_FUNCTION(0x607202, "SceneActionMap::init::$_1 destroy_deallocate()", SceneActionMap_lambdas::init_1::deleteThis);
RW_FUNCTION(0x607220, "SceneActionMap::init::$_1 target(const type_info&)", SceneActionMap_lambdas::init_1::target);
RW_FUNCTION(0x607240, "SceneActionMap::init::$_1 target_type()", SceneActionMap_lambdas::init_1::targetType);
RW_FUNCTION(0x607254, "SceneActionMap::init::$_0 ~__func() (deleting)", SceneActionMap_lambdas::init_0::deleteThis);
RW_FUNCTION(0x60725C, "SceneActionMap::init::$_0 __clone()", SceneActionMap_lambdas::init_0::clone);
RW_FUNCTION(0x607288, "SceneActionMap::init::$_0 __clone(__base*)", SceneActionMap_lambdas::init_0::cloneInto);
RW_FUNCTION(0x6072A8, "SceneActionMap::init::$_0 destroy()", SceneActionMap_lambdas::init_0::destroy);
RW_FUNCTION(0x6072AA, "SceneActionMap::init::$_0 destroy_deallocate()", SceneActionMap_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6072C0, "SceneActionMap::init::$_0 target(const type_info&)", SceneActionMap_lambdas::init_0::target);
RW_FUNCTION(0x6072E0, "SceneActionMap::init::$_0 target_type()", SceneActionMap_lambdas::init_0::targetType);
