// AvatarActor: the std::function slots of its lambdas, generated from the binary's __func vtables
// (each thunk's shape was checked by emulating the original instructions). The lambda bodies
// (operator()) are written by hand.
#include "../scene_main/FunctionThunks.hpp"

namespace AvatarActor_w31_thunks {

// AvatarActor::addPrize::$_1
using addPrize_1 = thunk::Lambda<0xF6DD4C, 8, 0xFA7EC0, 0xE9D5D0, 4, thunk::cap(4, 4)>;

}  // namespace AvatarActor_w31_thunks

RW_FUNCTION(0x7416E6, "AvatarActor::addPrize::$_1 ~__func() (deleting)", AvatarActor_w31_thunks::addPrize_1::deleteThis);
RW_FUNCTION(0x7416F0, "AvatarActor::addPrize::$_1 __clone()", AvatarActor_w31_thunks::addPrize_1::clone);
RW_FUNCTION(0x74171C, "AvatarActor::addPrize::$_1 __clone(__base*)", AvatarActor_w31_thunks::addPrize_1::cloneInto);
RW_FUNCTION(0x74173C, "AvatarActor::addPrize::$_1 destroy()", AvatarActor_w31_thunks::addPrize_1::destroy);
RW_FUNCTION(0x74173E, "AvatarActor::addPrize::$_1 destroy_deallocate()", AvatarActor_w31_thunks::addPrize_1::deleteThis);
RW_FUNCTION(0x741754, "AvatarActor::addPrize::$_1 target(const type_info&)", AvatarActor_w31_thunks::addPrize_1::target);
RW_FUNCTION(0x741774, "AvatarActor::addPrize::$_1 target_type()", AvatarActor_w31_thunks::addPrize_1::targetType);
