// SceneNameRegister: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneNameRegister_lambdas {

// SceneNameRegister::init::$_2
using init_2 = thunk::Lambda<0xF6B6F0, 8, 0xFA520C, 0xE917B0, 4>;
// SceneNameRegister::init::$_1
using init_1 = thunk::Lambda<0xF6B71C, 8, 0xFA5220, 0xE91840, 4, thunk::cap(4, 4)>;
// SceneNameRegister::init::$_0
using init_0 = thunk::Lambda<0xF6B748, 8, 0xFA5234, 0xE918D0, 4, thunk::cap(4, 4)>;

}  // namespace SceneNameRegister_lambdas

RW_FUNCTION(0x717052, "SceneNameRegister::init::$_2 ~__func() (deleting)", SceneNameRegister_lambdas::init_2::deleteThis);
RW_FUNCTION(0x71705C, "SceneNameRegister::init::$_2 __clone()", SceneNameRegister_lambdas::init_2::clone);
RW_FUNCTION(0x717080, "SceneNameRegister::init::$_2 __clone(__base*)", SceneNameRegister_lambdas::init_2::cloneInto);
RW_FUNCTION(0x71709C, "SceneNameRegister::init::$_2 destroy()", SceneNameRegister_lambdas::init_2::destroy);
RW_FUNCTION(0x71709E, "SceneNameRegister::init::$_2 destroy_deallocate()", SceneNameRegister_lambdas::init_2::deleteThis);
RW_FUNCTION(0x71715C, "SceneNameRegister::init::$_2 target(const type_info&)", SceneNameRegister_lambdas::init_2::target);
RW_FUNCTION(0x71717C, "SceneNameRegister::init::$_2 target_type()", SceneNameRegister_lambdas::init_2::targetType);
RW_FUNCTION(0x717190, "SceneNameRegister::init::$_1 ~__func() (deleting)", SceneNameRegister_lambdas::init_1::deleteThis);
RW_FUNCTION(0x717198, "SceneNameRegister::init::$_1 __clone()", SceneNameRegister_lambdas::init_1::clone);
RW_FUNCTION(0x7171C4, "SceneNameRegister::init::$_1 __clone(__base*)", SceneNameRegister_lambdas::init_1::cloneInto);
RW_FUNCTION(0x7171E4, "SceneNameRegister::init::$_1 destroy()", SceneNameRegister_lambdas::init_1::destroy);
RW_FUNCTION(0x7171E6, "SceneNameRegister::init::$_1 destroy_deallocate()", SceneNameRegister_lambdas::init_1::deleteThis);
RW_FUNCTION(0x7171F8, "SceneNameRegister::init::$_1 target(const type_info&)", SceneNameRegister_lambdas::init_1::target);
RW_FUNCTION(0x717218, "SceneNameRegister::init::$_1 target_type()", SceneNameRegister_lambdas::init_1::targetType);
RW_FUNCTION(0x71722C, "SceneNameRegister::init::$_0 ~__func() (deleting)", SceneNameRegister_lambdas::init_0::deleteThis);
RW_FUNCTION(0x717234, "SceneNameRegister::init::$_0 __clone()", SceneNameRegister_lambdas::init_0::clone);
RW_FUNCTION(0x717260, "SceneNameRegister::init::$_0 __clone(__base*)", SceneNameRegister_lambdas::init_0::cloneInto);
RW_FUNCTION(0x717280, "SceneNameRegister::init::$_0 destroy()", SceneNameRegister_lambdas::init_0::destroy);
RW_FUNCTION(0x717282, "SceneNameRegister::init::$_0 destroy_deallocate()", SceneNameRegister_lambdas::init_0::deleteThis);
RW_FUNCTION(0x71729C, "SceneNameRegister::init::$_0 target(const type_info&)", SceneNameRegister_lambdas::init_0::target);
RW_FUNCTION(0x7172BC, "SceneNameRegister::init::$_0 target_type()", SceneNameRegister_lambdas::init_0::targetType);
