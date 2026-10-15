// StageUtil: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace StageUtil_lambdas {

// StageUtil::getTreasurePrizes::$_1
using getTreasurePrizes_1 = thunk::Lambda<0xF6E5EC, 8, 0xFA8D40, 0xEA0350, 4>;
// StageUtil::getTreasurePrizes::$_0
using getTreasurePrizes_0 = thunk::Lambda<0xF6E618, 8, 0xFA8D54, 0xEA0460, 4>;

}  // namespace StageUtil_lambdas

RW_FUNCTION(0x764836, "StageUtil::getTreasurePrizes::$_1 ~__func() (deleting)", StageUtil_lambdas::getTreasurePrizes_1::deleteThis);
RW_FUNCTION(0x764840, "StageUtil::getTreasurePrizes::$_1 __clone()", StageUtil_lambdas::getTreasurePrizes_1::clone);
RW_FUNCTION(0x764864, "StageUtil::getTreasurePrizes::$_1 __clone(__base*)", StageUtil_lambdas::getTreasurePrizes_1::cloneInto);
RW_FUNCTION(0x764880, "StageUtil::getTreasurePrizes::$_1 destroy()", StageUtil_lambdas::getTreasurePrizes_1::destroy);
RW_FUNCTION(0x764882, "StageUtil::getTreasurePrizes::$_1 destroy_deallocate()", StageUtil_lambdas::getTreasurePrizes_1::deleteThis);
RW_FUNCTION(0x7648B8, "StageUtil::getTreasurePrizes::$_1 target(const type_info&)", StageUtil_lambdas::getTreasurePrizes_1::target);
RW_FUNCTION(0x7648D8, "StageUtil::getTreasurePrizes::$_1 target_type()", StageUtil_lambdas::getTreasurePrizes_1::targetType);
RW_FUNCTION(0x7648EC, "StageUtil::getTreasurePrizes::$_0 ~__func() (deleting)", StageUtil_lambdas::getTreasurePrizes_0::deleteThis);
RW_FUNCTION(0x7648F4, "StageUtil::getTreasurePrizes::$_0 __clone()", StageUtil_lambdas::getTreasurePrizes_0::clone);
RW_FUNCTION(0x764918, "StageUtil::getTreasurePrizes::$_0 __clone(__base*)", StageUtil_lambdas::getTreasurePrizes_0::cloneInto);
RW_FUNCTION(0x764934, "StageUtil::getTreasurePrizes::$_0 destroy()", StageUtil_lambdas::getTreasurePrizes_0::destroy);
RW_FUNCTION(0x764936, "StageUtil::getTreasurePrizes::$_0 destroy_deallocate()", StageUtil_lambdas::getTreasurePrizes_0::deleteThis);
RW_FUNCTION(0x764974, "StageUtil::getTreasurePrizes::$_0 target(const type_info&)", StageUtil_lambdas::getTreasurePrizes_0::target);
RW_FUNCTION(0x764994, "StageUtil::getTreasurePrizes::$_0 target_type()", StageUtil_lambdas::getTreasurePrizes_0::targetType);
