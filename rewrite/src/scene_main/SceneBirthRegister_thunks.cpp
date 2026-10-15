// SceneBirthRegister: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneBirthRegister_lambdas {

// SceneBirthRegister::init::$_3
using init_3 = thunk::Lambda<0xF6B094, 8, 0xFA4230, 0xE8F200, 4>;
// SceneBirthRegister::init::$_2
using init_2 = thunk::Lambda<0xF6B0C0, 8, 0xFA4244, 0xE8F290, 4, thunk::cap(4, 4)>;
// SceneBirthRegister::init::$_1
using init_1 = thunk::Lambda<0xF6B0EC, 8, 0xFA4258, 0xE8F320, 4, thunk::cap(4, 4)>;
// SceneBirthRegister::init::$_0
using init_0 = thunk::Lambda<0xF6B118, 8, 0xFA426C, 0xE8F3B0, 4, thunk::cap(4, 4)>;

}  // namespace SceneBirthRegister_lambdas

RW_FUNCTION(0x7121A8, "SceneBirthRegister::init::$_3 ~__func() (deleting)", SceneBirthRegister_lambdas::init_3::deleteThis);
RW_FUNCTION(0x7121B0, "SceneBirthRegister::init::$_3 __clone()", SceneBirthRegister_lambdas::init_3::clone);
RW_FUNCTION(0x7121D4, "SceneBirthRegister::init::$_3 __clone(__base*)", SceneBirthRegister_lambdas::init_3::cloneInto);
RW_FUNCTION(0x7121F0, "SceneBirthRegister::init::$_3 destroy()", SceneBirthRegister_lambdas::init_3::destroy);
RW_FUNCTION(0x7121F2, "SceneBirthRegister::init::$_3 destroy_deallocate()", SceneBirthRegister_lambdas::init_3::deleteThis);
RW_FUNCTION(0x71220C, "SceneBirthRegister::init::$_3 target(const type_info&)", SceneBirthRegister_lambdas::init_3::target);
RW_FUNCTION(0x71222C, "SceneBirthRegister::init::$_3 target_type()", SceneBirthRegister_lambdas::init_3::targetType);
RW_FUNCTION(0x712240, "SceneBirthRegister::init::$_2 ~__func() (deleting)", SceneBirthRegister_lambdas::init_2::deleteThis);
RW_FUNCTION(0x712248, "SceneBirthRegister::init::$_2 __clone()", SceneBirthRegister_lambdas::init_2::clone);
RW_FUNCTION(0x712274, "SceneBirthRegister::init::$_2 __clone(__base*)", SceneBirthRegister_lambdas::init_2::cloneInto);
RW_FUNCTION(0x712294, "SceneBirthRegister::init::$_2 destroy()", SceneBirthRegister_lambdas::init_2::destroy);
RW_FUNCTION(0x712296, "SceneBirthRegister::init::$_2 destroy_deallocate()", SceneBirthRegister_lambdas::init_2::deleteThis);
RW_FUNCTION(0x7122AC, "SceneBirthRegister::init::$_2 target(const type_info&)", SceneBirthRegister_lambdas::init_2::target);
RW_FUNCTION(0x7122CC, "SceneBirthRegister::init::$_2 target_type()", SceneBirthRegister_lambdas::init_2::targetType);
RW_FUNCTION(0x7122E0, "SceneBirthRegister::init::$_1 ~__func() (deleting)", SceneBirthRegister_lambdas::init_1::deleteThis);
RW_FUNCTION(0x7122E8, "SceneBirthRegister::init::$_1 __clone()", SceneBirthRegister_lambdas::init_1::clone);
RW_FUNCTION(0x712314, "SceneBirthRegister::init::$_1 __clone(__base*)", SceneBirthRegister_lambdas::init_1::cloneInto);
RW_FUNCTION(0x712334, "SceneBirthRegister::init::$_1 destroy()", SceneBirthRegister_lambdas::init_1::destroy);
RW_FUNCTION(0x712336, "SceneBirthRegister::init::$_1 destroy_deallocate()", SceneBirthRegister_lambdas::init_1::deleteThis);
RW_FUNCTION(0x712348, "SceneBirthRegister::init::$_1 target(const type_info&)", SceneBirthRegister_lambdas::init_1::target);
RW_FUNCTION(0x712368, "SceneBirthRegister::init::$_1 target_type()", SceneBirthRegister_lambdas::init_1::targetType);
RW_FUNCTION(0x71237C, "SceneBirthRegister::init::$_0 ~__func() (deleting)", SceneBirthRegister_lambdas::init_0::deleteThis);
RW_FUNCTION(0x712384, "SceneBirthRegister::init::$_0 __clone()", SceneBirthRegister_lambdas::init_0::clone);
RW_FUNCTION(0x7123B0, "SceneBirthRegister::init::$_0 __clone(__base*)", SceneBirthRegister_lambdas::init_0::cloneInto);
RW_FUNCTION(0x7123D0, "SceneBirthRegister::init::$_0 destroy()", SceneBirthRegister_lambdas::init_0::destroy);
RW_FUNCTION(0x7123D2, "SceneBirthRegister::init::$_0 destroy_deallocate()", SceneBirthRegister_lambdas::init_0::deleteThis);
RW_FUNCTION(0x7123E4, "SceneBirthRegister::init::$_0 target(const type_info&)", SceneBirthRegister_lambdas::init_0::target);
RW_FUNCTION(0x712404, "SceneBirthRegister::init::$_0 target_type()", SceneBirthRegister_lambdas::init_0::targetType);
