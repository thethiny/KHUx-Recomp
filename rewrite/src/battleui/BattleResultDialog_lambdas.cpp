// BattleResultDialog: the std::function plumbing of its lambdas (libc++ __func slots: deleting destructor, clone, clone into a
// buffer, destroy, destroy_deallocate, target, target_type), generated from the binary's __func vtables (size,
// captured bytes, type_info, type name and target offset read from each slot's instructions), plus the lambda bodies.
#include "../scene_main/FunctionThunks.hpp"

namespace BattleResultDialog_w3 {

// BattleResultDialog::updateLuxLvNext::$_24
using updateLuxLvNext_24 = thunk::Lambda<0xF59DD8, 8, 0xF8B544, 0xE37060, 4, thunk::cap(4, 4)>;
// BattleResultDialog::updateLuxGauge::$_23
using updateLuxGauge_23 = thunk::Lambda<0xF59E04, 8, 0xF8B558, 0xE37100, 4, thunk::cap(4, 4)>;
// BattleResultDialog::popUpSkillLevelUp::$_22
using popUpSkillLevelUp_22 = thunk::Lambda<0xF59E30, 16, 0xF8B56C, 0xE371A0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// BattleResultDialog::popUpSkillLevelUp::$_22::(inner)
using popUpSkillLevelUp_22_inner = thunk::Lambda<0xF59E5C, 16, 0xF8B580, 0xE37260, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// BattleResultDialog::openRaidResult::$_7
using openRaidResult_7 = thunk::Lambda<0xF5A1A0, 8, 0xF8B6FC, 0xE382F0, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openRaidResult::$_6
using openRaidResult_6 = thunk::Lambda<0xF5A1CC, 8, 0xF8B710, 0xE38400, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openRaidResult::$_5
using openRaidResult_5 = thunk::Lambda<0xF5A1F8, 8, 0xF8B724, 0xE38510, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openStageResult::$_4
using openStageResult_4 = thunk::Lambda<0xF5A224, 8, 0xF8B738, 0xE38600, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openStageResult::$_4::(inner)
using openStageResult_4_inner = thunk::Lambda<0xF5A250, 8, 0xF8B74C, 0xE386C0, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openStageResult::$_4::(inner)
using openStageResult_4_inner_f5a27c = thunk::Lambda<0xF5A27C, 8, 0xF8B760, 0xE387A0, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openStageResult::$_4::(inner)
using openStageResult_4_inner_f5a2a8 = thunk::Lambda<0xF5A2A8, 8, 0xF8B774, 0xE388A0, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openStageResult::$_4::(inner)
using openStageResult_4_inner_f5a2d4 = thunk::Lambda<0xF5A2D4, 8, 0xF8B788, 0xE389C0, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openStageResult::$_3
using openStageResult_3 = thunk::Lambda<0xF5A300, 8, 0xF8B79C, 0xE38AE0, 4, thunk::cap(4, 4)>;
// BattleResultDialog::openStageResult::$_2
using openStageResult_2 = thunk::Lambda<0xF5A32C, 8, 0xF8B7B0, 0xE38BA0, 4, thunk::cap(4, 4)>;
// BattleResultDialog::init::$_1
using init_1_f5a358 = thunk::Lambda<0xF5A358, 8, 0xF8B7C4, 0xE38C40, 4, thunk::cap(4, 4)>;
// BattleResultDialog::init::$_0
using init_0 = thunk::Lambda<0xF5A384, 8, 0xF8B7D8, 0xE38CD0, 4, thunk::cap(4, 4)>;

}  // namespace BattleResultDialog_w3

RW_FUNCTION(0x5838B8, "BattleResultDialog::updateLuxLvNext::$_24 ~__func() (deleting)", BattleResultDialog_w3::updateLuxLvNext_24::deleteThis);
RW_FUNCTION(0x5838C0, "BattleResultDialog::updateLuxLvNext::$_24 __clone()", BattleResultDialog_w3::updateLuxLvNext_24::clone);
RW_FUNCTION(0x5838EC, "BattleResultDialog::updateLuxLvNext::$_24 __clone(__base*)", BattleResultDialog_w3::updateLuxLvNext_24::cloneInto);
RW_FUNCTION(0x58390C, "BattleResultDialog::updateLuxLvNext::$_24 destroy()", BattleResultDialog_w3::updateLuxLvNext_24::destroy);
RW_FUNCTION(0x58390E, "BattleResultDialog::updateLuxLvNext::$_24 destroy_deallocate()", BattleResultDialog_w3::updateLuxLvNext_24::deleteThis);
RW_FUNCTION(0x583AAC, "BattleResultDialog::updateLuxLvNext::$_24 target(const type_info&)", BattleResultDialog_w3::updateLuxLvNext_24::target);
RW_FUNCTION(0x583ACC, "BattleResultDialog::updateLuxLvNext::$_24 target_type()", BattleResultDialog_w3::updateLuxLvNext_24::targetType);
RW_FUNCTION(0x583AE0, "BattleResultDialog::updateLuxGauge::$_23 ~__func() (deleting)", BattleResultDialog_w3::updateLuxGauge_23::deleteThis);
RW_FUNCTION(0x583AE8, "BattleResultDialog::updateLuxGauge::$_23 __clone()", BattleResultDialog_w3::updateLuxGauge_23::clone);
RW_FUNCTION(0x583B14, "BattleResultDialog::updateLuxGauge::$_23 __clone(__base*)", BattleResultDialog_w3::updateLuxGauge_23::cloneInto);
RW_FUNCTION(0x583B34, "BattleResultDialog::updateLuxGauge::$_23 destroy()", BattleResultDialog_w3::updateLuxGauge_23::destroy);
RW_FUNCTION(0x583B36, "BattleResultDialog::updateLuxGauge::$_23 destroy_deallocate()", BattleResultDialog_w3::updateLuxGauge_23::deleteThis);
RW_FUNCTION(0x583C74, "BattleResultDialog::updateLuxGauge::$_23 target(const type_info&)", BattleResultDialog_w3::updateLuxGauge_23::target);
RW_FUNCTION(0x583C94, "BattleResultDialog::updateLuxGauge::$_23 target_type()", BattleResultDialog_w3::updateLuxGauge_23::targetType);
RW_FUNCTION(0x583DEC, "BattleResultDialog::popUpSkillLevelUp::$_22 ~__func() (deleting)", BattleResultDialog_w3::popUpSkillLevelUp_22::deleteThis);
RW_FUNCTION(0x583DF4, "BattleResultDialog::popUpSkillLevelUp::$_22 __clone()", BattleResultDialog_w3::popUpSkillLevelUp_22::clone);
RW_FUNCTION(0x583E24, "BattleResultDialog::popUpSkillLevelUp::$_22 __clone(__base*)", BattleResultDialog_w3::popUpSkillLevelUp_22::cloneInto);
RW_FUNCTION(0x583E4C, "BattleResultDialog::popUpSkillLevelUp::$_22 destroy()", BattleResultDialog_w3::popUpSkillLevelUp_22::destroy);
RW_FUNCTION(0x583E4E, "BattleResultDialog::popUpSkillLevelUp::$_22 destroy_deallocate()", BattleResultDialog_w3::popUpSkillLevelUp_22::deleteThis);
RW_FUNCTION(0x583F24, "BattleResultDialog::popUpSkillLevelUp::$_22 target(const type_info&)", BattleResultDialog_w3::popUpSkillLevelUp_22::target);
RW_FUNCTION(0x583F44, "BattleResultDialog::popUpSkillLevelUp::$_22 target_type()", BattleResultDialog_w3::popUpSkillLevelUp_22::targetType);
RW_FUNCTION(0x583F58, "BattleResultDialog::popUpSkillLevelUp::$_22::(inner) ~__func() (deleting)", BattleResultDialog_w3::popUpSkillLevelUp_22_inner::deleteThis);
RW_FUNCTION(0x583F60, "BattleResultDialog::popUpSkillLevelUp::$_22::(inner) __clone()", BattleResultDialog_w3::popUpSkillLevelUp_22_inner::clone);
RW_FUNCTION(0x583F90, "BattleResultDialog::popUpSkillLevelUp::$_22::(inner) __clone(__base*)", BattleResultDialog_w3::popUpSkillLevelUp_22_inner::cloneInto);
RW_FUNCTION(0x583FB8, "BattleResultDialog::popUpSkillLevelUp::$_22::(inner) destroy()", BattleResultDialog_w3::popUpSkillLevelUp_22_inner::destroy);
RW_FUNCTION(0x583FBA, "BattleResultDialog::popUpSkillLevelUp::$_22::(inner) destroy_deallocate()", BattleResultDialog_w3::popUpSkillLevelUp_22_inner::deleteThis);
RW_FUNCTION(0x58400C, "BattleResultDialog::popUpSkillLevelUp::$_22::(inner) target(const type_info&)", BattleResultDialog_w3::popUpSkillLevelUp_22_inner::target);
RW_FUNCTION(0x58402C, "BattleResultDialog::popUpSkillLevelUp::$_22::(inner) target_type()", BattleResultDialog_w3::popUpSkillLevelUp_22_inner::targetType);
RW_FUNCTION(0x585854, "BattleResultDialog::openRaidResult::$_7 target_type()", BattleResultDialog_w3::openRaidResult_7::targetType);
RW_FUNCTION(0x585868, "BattleResultDialog::openRaidResult::$_6 ~__func() (deleting)", BattleResultDialog_w3::openRaidResult_6::deleteThis);
RW_FUNCTION(0x585870, "BattleResultDialog::openRaidResult::$_6 __clone()", BattleResultDialog_w3::openRaidResult_6::clone);
RW_FUNCTION(0x58589C, "BattleResultDialog::openRaidResult::$_6 __clone(__base*)", BattleResultDialog_w3::openRaidResult_6::cloneInto);
RW_FUNCTION(0x5858BC, "BattleResultDialog::openRaidResult::$_6 destroy()", BattleResultDialog_w3::openRaidResult_6::destroy);
RW_FUNCTION(0x5858BE, "BattleResultDialog::openRaidResult::$_6 destroy_deallocate()", BattleResultDialog_w3::openRaidResult_6::deleteThis);
RW_FUNCTION(0x585978, "BattleResultDialog::openRaidResult::$_6 target(const type_info&)", BattleResultDialog_w3::openRaidResult_6::target);
RW_FUNCTION(0x585998, "BattleResultDialog::openRaidResult::$_6 target_type()", BattleResultDialog_w3::openRaidResult_6::targetType);
RW_FUNCTION(0x5859AC, "BattleResultDialog::openRaidResult::$_5 ~__func() (deleting)", BattleResultDialog_w3::openRaidResult_5::deleteThis);
RW_FUNCTION(0x5859B4, "BattleResultDialog::openRaidResult::$_5 __clone()", BattleResultDialog_w3::openRaidResult_5::clone);
RW_FUNCTION(0x5859E0, "BattleResultDialog::openRaidResult::$_5 __clone(__base*)", BattleResultDialog_w3::openRaidResult_5::cloneInto);
RW_FUNCTION(0x585A00, "BattleResultDialog::openRaidResult::$_5 destroy()", BattleResultDialog_w3::openRaidResult_5::destroy);
RW_FUNCTION(0x585A02, "BattleResultDialog::openRaidResult::$_5 destroy_deallocate()", BattleResultDialog_w3::openRaidResult_5::deleteThis);
RW_FUNCTION(0x585A18, "BattleResultDialog::openRaidResult::$_5 target(const type_info&)", BattleResultDialog_w3::openRaidResult_5::target);
RW_FUNCTION(0x585A38, "BattleResultDialog::openRaidResult::$_5 target_type()", BattleResultDialog_w3::openRaidResult_5::targetType);
RW_FUNCTION(0x585A4C, "BattleResultDialog::openStageResult::$_4 ~__func() (deleting)", BattleResultDialog_w3::openStageResult_4::deleteThis);
RW_FUNCTION(0x585A54, "BattleResultDialog::openStageResult::$_4 __clone()", BattleResultDialog_w3::openStageResult_4::clone);
RW_FUNCTION(0x585A80, "BattleResultDialog::openStageResult::$_4 __clone(__base*)", BattleResultDialog_w3::openStageResult_4::cloneInto);
RW_FUNCTION(0x585AA0, "BattleResultDialog::openStageResult::$_4 destroy()", BattleResultDialog_w3::openStageResult_4::destroy);
RW_FUNCTION(0x585AA2, "BattleResultDialog::openStageResult::$_4 destroy_deallocate()", BattleResultDialog_w3::openStageResult_4::deleteThis);
RW_FUNCTION(0x585BA8, "BattleResultDialog::openStageResult::$_4 target(const type_info&)", BattleResultDialog_w3::openStageResult_4::target);
RW_FUNCTION(0x585BC8, "BattleResultDialog::openStageResult::$_4 target_type()", BattleResultDialog_w3::openStageResult_4::targetType);
RW_FUNCTION(0x585BDC, "BattleResultDialog::openStageResult::$_4::(inner) ~__func() (deleting)", BattleResultDialog_w3::openStageResult_4_inner::deleteThis);
RW_FUNCTION(0x585BE4, "BattleResultDialog::openStageResult::$_4::(inner) __clone()", BattleResultDialog_w3::openStageResult_4_inner::clone);
RW_FUNCTION(0x585C10, "BattleResultDialog::openStageResult::$_4::(inner) __clone(__base*)", BattleResultDialog_w3::openStageResult_4_inner::cloneInto);
RW_FUNCTION(0x585C30, "BattleResultDialog::openStageResult::$_4::(inner) destroy()", BattleResultDialog_w3::openStageResult_4_inner::destroy);
RW_FUNCTION(0x585C32, "BattleResultDialog::openStageResult::$_4::(inner) destroy_deallocate()", BattleResultDialog_w3::openStageResult_4_inner::deleteThis);
RW_FUNCTION(0x585CD0, "BattleResultDialog::openStageResult::$_4::(inner) target(const type_info&)", BattleResultDialog_w3::openStageResult_4_inner::target);
RW_FUNCTION(0x585CF0, "BattleResultDialog::openStageResult::$_4::(inner) target_type()", BattleResultDialog_w3::openStageResult_4_inner::targetType);
RW_FUNCTION(0x585D04, "BattleResultDialog::openStageResult::$_4::(inner) ~__func() (deleting)", BattleResultDialog_w3::openStageResult_4_inner_f5a27c::deleteThis);
RW_FUNCTION(0x585D0C, "BattleResultDialog::openStageResult::$_4::(inner) __clone()", BattleResultDialog_w3::openStageResult_4_inner_f5a27c::clone);
RW_FUNCTION(0x585D38, "BattleResultDialog::openStageResult::$_4::(inner) __clone(__base*)", BattleResultDialog_w3::openStageResult_4_inner_f5a27c::cloneInto);
RW_FUNCTION(0x585D58, "BattleResultDialog::openStageResult::$_4::(inner) destroy()", BattleResultDialog_w3::openStageResult_4_inner_f5a27c::destroy);
RW_FUNCTION(0x585D5A, "BattleResultDialog::openStageResult::$_4::(inner) destroy_deallocate()", BattleResultDialog_w3::openStageResult_4_inner_f5a27c::deleteThis);
RW_FUNCTION(0x585DF8, "BattleResultDialog::openStageResult::$_4::(inner) target(const type_info&)", BattleResultDialog_w3::openStageResult_4_inner_f5a27c::target);
RW_FUNCTION(0x585E18, "BattleResultDialog::openStageResult::$_4::(inner) target_type()", BattleResultDialog_w3::openStageResult_4_inner_f5a27c::targetType);
RW_FUNCTION(0x585E2C, "BattleResultDialog::openStageResult::$_4::(inner) ~__func() (deleting)", BattleResultDialog_w3::openStageResult_4_inner_f5a2a8::deleteThis);
RW_FUNCTION(0x585E34, "BattleResultDialog::openStageResult::$_4::(inner) __clone()", BattleResultDialog_w3::openStageResult_4_inner_f5a2a8::clone);
RW_FUNCTION(0x585E60, "BattleResultDialog::openStageResult::$_4::(inner) __clone(__base*)", BattleResultDialog_w3::openStageResult_4_inner_f5a2a8::cloneInto);
RW_FUNCTION(0x585E80, "BattleResultDialog::openStageResult::$_4::(inner) destroy()", BattleResultDialog_w3::openStageResult_4_inner_f5a2a8::destroy);
RW_FUNCTION(0x585E82, "BattleResultDialog::openStageResult::$_4::(inner) destroy_deallocate()", BattleResultDialog_w3::openStageResult_4_inner_f5a2a8::deleteThis);
RW_FUNCTION(0x585F20, "BattleResultDialog::openStageResult::$_4::(inner) target(const type_info&)", BattleResultDialog_w3::openStageResult_4_inner_f5a2a8::target);
RW_FUNCTION(0x585F40, "BattleResultDialog::openStageResult::$_4::(inner) target_type()", BattleResultDialog_w3::openStageResult_4_inner_f5a2a8::targetType);
RW_FUNCTION(0x585F54, "BattleResultDialog::openStageResult::$_4::(inner) ~__func() (deleting)", BattleResultDialog_w3::openStageResult_4_inner_f5a2d4::deleteThis);
RW_FUNCTION(0x585F5C, "BattleResultDialog::openStageResult::$_4::(inner) __clone()", BattleResultDialog_w3::openStageResult_4_inner_f5a2d4::clone);
RW_FUNCTION(0x585F88, "BattleResultDialog::openStageResult::$_4::(inner) __clone(__base*)", BattleResultDialog_w3::openStageResult_4_inner_f5a2d4::cloneInto);
RW_FUNCTION(0x585FA8, "BattleResultDialog::openStageResult::$_4::(inner) destroy()", BattleResultDialog_w3::openStageResult_4_inner_f5a2d4::destroy);
RW_FUNCTION(0x585FAA, "BattleResultDialog::openStageResult::$_4::(inner) destroy_deallocate()", BattleResultDialog_w3::openStageResult_4_inner_f5a2d4::deleteThis);
RW_FUNCTION(0x5860F8, "BattleResultDialog::openStageResult::$_4::(inner) target(const type_info&)", BattleResultDialog_w3::openStageResult_4_inner_f5a2d4::target);
RW_FUNCTION(0x586118, "BattleResultDialog::openStageResult::$_4::(inner) target_type()", BattleResultDialog_w3::openStageResult_4_inner_f5a2d4::targetType);
RW_FUNCTION(0x58612C, "BattleResultDialog::openStageResult::$_3 ~__func() (deleting)", BattleResultDialog_w3::openStageResult_3::deleteThis);
RW_FUNCTION(0x586134, "BattleResultDialog::openStageResult::$_3 __clone()", BattleResultDialog_w3::openStageResult_3::clone);
RW_FUNCTION(0x586160, "BattleResultDialog::openStageResult::$_3 __clone(__base*)", BattleResultDialog_w3::openStageResult_3::cloneInto);
RW_FUNCTION(0x586180, "BattleResultDialog::openStageResult::$_3 destroy()", BattleResultDialog_w3::openStageResult_3::destroy);
RW_FUNCTION(0x586182, "BattleResultDialog::openStageResult::$_3 destroy_deallocate()", BattleResultDialog_w3::openStageResult_3::deleteThis);
RW_FUNCTION(0x586288, "BattleResultDialog::openStageResult::$_3 target(const type_info&)", BattleResultDialog_w3::openStageResult_3::target);
RW_FUNCTION(0x5862A8, "BattleResultDialog::openStageResult::$_3 target_type()", BattleResultDialog_w3::openStageResult_3::targetType);
RW_FUNCTION(0x5862BC, "BattleResultDialog::openStageResult::$_2 ~__func() (deleting)", BattleResultDialog_w3::openStageResult_2::deleteThis);
RW_FUNCTION(0x5862C4, "BattleResultDialog::openStageResult::$_2 __clone()", BattleResultDialog_w3::openStageResult_2::clone);
RW_FUNCTION(0x5862F0, "BattleResultDialog::openStageResult::$_2 __clone(__base*)", BattleResultDialog_w3::openStageResult_2::cloneInto);
RW_FUNCTION(0x586310, "BattleResultDialog::openStageResult::$_2 destroy()", BattleResultDialog_w3::openStageResult_2::destroy);
RW_FUNCTION(0x586312, "BattleResultDialog::openStageResult::$_2 destroy_deallocate()", BattleResultDialog_w3::openStageResult_2::deleteThis);
RW_FUNCTION(0x58641C, "BattleResultDialog::openStageResult::$_2 target(const type_info&)", BattleResultDialog_w3::openStageResult_2::target);
RW_FUNCTION(0x58643C, "BattleResultDialog::openStageResult::$_2 target_type()", BattleResultDialog_w3::openStageResult_2::targetType);
RW_FUNCTION(0x586450, "BattleResultDialog::init::$_1 ~__func() (deleting)", BattleResultDialog_w3::init_1_f5a358::deleteThis);
RW_FUNCTION(0x586458, "BattleResultDialog::init::$_1 __clone()", BattleResultDialog_w3::init_1_f5a358::clone);
RW_FUNCTION(0x586484, "BattleResultDialog::init::$_1 __clone(__base*)", BattleResultDialog_w3::init_1_f5a358::cloneInto);
RW_FUNCTION(0x5864A4, "BattleResultDialog::init::$_1 destroy()", BattleResultDialog_w3::init_1_f5a358::destroy);
RW_FUNCTION(0x5864A6, "BattleResultDialog::init::$_1 destroy_deallocate()", BattleResultDialog_w3::init_1_f5a358::deleteThis);
RW_FUNCTION(0x5864B8, "BattleResultDialog::init::$_1 target(const type_info&)", BattleResultDialog_w3::init_1_f5a358::target);
RW_FUNCTION(0x5864D8, "BattleResultDialog::init::$_1 target_type()", BattleResultDialog_w3::init_1_f5a358::targetType);
RW_FUNCTION(0x5864EC, "BattleResultDialog::init::$_0 ~__func() (deleting)", BattleResultDialog_w3::init_0::deleteThis);
RW_FUNCTION(0x5864F4, "BattleResultDialog::init::$_0 __clone()", BattleResultDialog_w3::init_0::clone);
RW_FUNCTION(0x586520, "BattleResultDialog::init::$_0 __clone(__base*)", BattleResultDialog_w3::init_0::cloneInto);
RW_FUNCTION(0x586540, "BattleResultDialog::init::$_0 destroy()", BattleResultDialog_w3::init_0::destroy);
RW_FUNCTION(0x586542, "BattleResultDialog::init::$_0 destroy_deallocate()", BattleResultDialog_w3::init_0::deleteThis);
RW_FUNCTION(0x586554, "BattleResultDialog::init::$_0 target(const type_info&)", BattleResultDialog_w3::init_0::target);
RW_FUNCTION(0x586574, "BattleResultDialog::init::$_0 target_type()", BattleResultDialog_w3::init_0::targetType);
