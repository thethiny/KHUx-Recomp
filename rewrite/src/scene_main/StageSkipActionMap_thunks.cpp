// StageSkipActionMap: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace StageSkipActionMap_lambdas {

// StageSkipActionMap::init::$_1
using init_1 = thunk::Lambda<0xF6E298, 8, 0xFA8818, 0xE9F210, 4, thunk::cap(4, 4)>;
// StageSkipActionMap::init::$_0
using init_0 = thunk::Lambda<0xF6E2C4, 8, 0xFA882C, 0xE9F2A0, 4, thunk::cap(4, 4)>;

}  // namespace StageSkipActionMap_lambdas

RW_FUNCTION(0x75E3BA, "StageSkipActionMap::init::$_1 ~__func() (deleting)", StageSkipActionMap_lambdas::init_1::deleteThis);
RW_FUNCTION(0x75E3C4, "StageSkipActionMap::init::$_1 __clone()", StageSkipActionMap_lambdas::init_1::clone);
RW_FUNCTION(0x75E3F0, "StageSkipActionMap::init::$_1 __clone(__base*)", StageSkipActionMap_lambdas::init_1::cloneInto);
RW_FUNCTION(0x75E410, "StageSkipActionMap::init::$_1 destroy()", StageSkipActionMap_lambdas::init_1::destroy);
RW_FUNCTION(0x75E412, "StageSkipActionMap::init::$_1 destroy_deallocate()", StageSkipActionMap_lambdas::init_1::deleteThis);
RW_FUNCTION(0x75E424, "StageSkipActionMap::init::$_1 target(const type_info&)", StageSkipActionMap_lambdas::init_1::target);
RW_FUNCTION(0x75E444, "StageSkipActionMap::init::$_1 target_type()", StageSkipActionMap_lambdas::init_1::targetType);
RW_FUNCTION(0x75E458, "StageSkipActionMap::init::$_0 ~__func() (deleting)", StageSkipActionMap_lambdas::init_0::deleteThis);
RW_FUNCTION(0x75E460, "StageSkipActionMap::init::$_0 __clone()", StageSkipActionMap_lambdas::init_0::clone);
RW_FUNCTION(0x75E48C, "StageSkipActionMap::init::$_0 __clone(__base*)", StageSkipActionMap_lambdas::init_0::cloneInto);
RW_FUNCTION(0x75E4AC, "StageSkipActionMap::init::$_0 destroy()", StageSkipActionMap_lambdas::init_0::destroy);
RW_FUNCTION(0x75E4AE, "StageSkipActionMap::init::$_0 destroy_deallocate()", StageSkipActionMap_lambdas::init_0::deleteThis);
RW_FUNCTION(0x75E4C0, "StageSkipActionMap::init::$_0 target(const type_info&)", StageSkipActionMap_lambdas::init_0::target);
RW_FUNCTION(0x75E4E0, "StageSkipActionMap::init::$_0 target_type()", StageSkipActionMap_lambdas::init_0::targetType);
