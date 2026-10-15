// SceneSplash: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneSplash_lambdas {

// SceneSplash::init::$_2
using init_2 = thunk::Lambda<0xF643F0, 8, 0xF9BFE8, 0xE6F520, 4, thunk::cap(4, 4)>;
// SceneSplash::init::$_1
using init_1 = thunk::Lambda<0xF6441C, 8, 0xF9BFFC, 0xE6F590, 4, thunk::cap(4, 4)>;
// SceneSplash::init::$_0
using init_0 = thunk::Lambda<0xF64448, 8, 0xF9C010, 0xE6F600, 4, thunk::cap(4, 4)>;

}  // namespace SceneSplash_lambdas

RW_FUNCTION(0x681268, "SceneSplash::init::$_2 ~__func() (deleting)", SceneSplash_lambdas::init_2::deleteThis);
RW_FUNCTION(0x681270, "SceneSplash::init::$_2 __clone()", SceneSplash_lambdas::init_2::clone);
RW_FUNCTION(0x68129C, "SceneSplash::init::$_2 __clone(__base*)", SceneSplash_lambdas::init_2::cloneInto);
RW_FUNCTION(0x6812BC, "SceneSplash::init::$_2 destroy()", SceneSplash_lambdas::init_2::destroy);
RW_FUNCTION(0x6812BE, "SceneSplash::init::$_2 destroy_deallocate()", SceneSplash_lambdas::init_2::deleteThis);
RW_FUNCTION(0x6812D4, "SceneSplash::init::$_2 target(const type_info&)", SceneSplash_lambdas::init_2::target);
RW_FUNCTION(0x6812F4, "SceneSplash::init::$_2 target_type()", SceneSplash_lambdas::init_2::targetType);
RW_FUNCTION(0x681308, "SceneSplash::init::$_1 ~__func() (deleting)", SceneSplash_lambdas::init_1::deleteThis);
RW_FUNCTION(0x681310, "SceneSplash::init::$_1 __clone()", SceneSplash_lambdas::init_1::clone);
RW_FUNCTION(0x68133C, "SceneSplash::init::$_1 __clone(__base*)", SceneSplash_lambdas::init_1::cloneInto);
RW_FUNCTION(0x68135C, "SceneSplash::init::$_1 destroy()", SceneSplash_lambdas::init_1::destroy);
RW_FUNCTION(0x68135E, "SceneSplash::init::$_1 destroy_deallocate()", SceneSplash_lambdas::init_1::deleteThis);
RW_FUNCTION(0x681370, "SceneSplash::init::$_1 target(const type_info&)", SceneSplash_lambdas::init_1::target);
RW_FUNCTION(0x681390, "SceneSplash::init::$_1 target_type()", SceneSplash_lambdas::init_1::targetType);
RW_FUNCTION(0x6813A4, "SceneSplash::init::$_0 ~__func() (deleting)", SceneSplash_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6813AC, "SceneSplash::init::$_0 __clone()", SceneSplash_lambdas::init_0::clone);
RW_FUNCTION(0x6813D8, "SceneSplash::init::$_0 __clone(__base*)", SceneSplash_lambdas::init_0::cloneInto);
RW_FUNCTION(0x6813F8, "SceneSplash::init::$_0 destroy()", SceneSplash_lambdas::init_0::destroy);
RW_FUNCTION(0x6813FA, "SceneSplash::init::$_0 destroy_deallocate()", SceneSplash_lambdas::init_0::deleteThis);
RW_FUNCTION(0x68140C, "SceneSplash::init::$_0 target(const type_info&)", SceneSplash_lambdas::init_0::target);
RW_FUNCTION(0x68142C, "SceneSplash::init::$_0 target_type()", SceneSplash_lambdas::init_0::targetType);
