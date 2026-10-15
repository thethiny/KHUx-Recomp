// AimTarget: the std::function slots of its lambdas, generated from the binary's __func vtables
// (each thunk's shape was checked by emulating the original instructions). The lambda bodies
// (operator()) are written by hand.
#include "../scene_main/FunctionThunks.hpp"

namespace AimTarget_w31_thunks {

// AimTarget::AimMarker::init::$_0
using init_0 = thunk::Lambda<0xF6DCC0, 8, 0xFA7C24, 0xE9D530, 4, thunk::cap(4, 4)>;

}  // namespace AimTarget_w31_thunks

RW_FUNCTION(0x73FC7C, "AimTarget::AimMarker::init::$_0 ~__func() (deleting)", AimTarget_w31_thunks::init_0::deleteThis);
RW_FUNCTION(0x73FC84, "AimTarget::AimMarker::init::$_0 __clone()", AimTarget_w31_thunks::init_0::clone);
RW_FUNCTION(0x73FCB0, "AimTarget::AimMarker::init::$_0 __clone(__base*)", AimTarget_w31_thunks::init_0::cloneInto);
RW_FUNCTION(0x73FCD0, "AimTarget::AimMarker::init::$_0 destroy()", AimTarget_w31_thunks::init_0::destroy);
RW_FUNCTION(0x73FCD2, "AimTarget::AimMarker::init::$_0 destroy_deallocate()", AimTarget_w31_thunks::init_0::deleteThis);
RW_FUNCTION(0x73FCEC, "AimTarget::AimMarker::init::$_0 target(const type_info&)", AimTarget_w31_thunks::init_0::target);
RW_FUNCTION(0x73FD0C, "AimTarget::AimMarker::init::$_0 target_type()", AimTarget_w31_thunks::init_0::targetType);
