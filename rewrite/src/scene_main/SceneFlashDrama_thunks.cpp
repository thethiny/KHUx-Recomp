// SceneFlashDrama: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneFlashDrama_lambdas {

// SceneFlashDrama::next_scene::$_11
using next_scene_11 = thunk::Lambda<0xF69CD0, 16, 0xFA28B4, 0xE89B90, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneFlashDrama::update::$_10
using update_10 = thunk::Lambda<0xF69CFC, 16, 0xFA28C8, 0xE89C30, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneFlashDrama::init::$_9
using init_9 = thunk::Lambda<0xF69D28, 8, 0xFA28DC, 0xE89CC0, 4, thunk::cap(4, 4)>;
// SceneFlashDrama::init::$_8
using init_8 = thunk::Lambda<0xF69D54, 8, 0xFA28F0, 0xE89D40, 4, thunk::cap(4, 4)>;
// SceneFlashDrama::init::$_7
using init_7 = thunk::Lambda<0xF69D80, 8, 0xFA2904, 0xE89DD0, 4, thunk::cap(4, 4)>;
// SceneFlashDrama::init::$_6
using init_6 = thunk::Lambda<0xF69DAC, 8, 0xFA2918, 0xE89E60, 4, thunk::cap(4, 4)>;

}  // namespace SceneFlashDrama_lambdas

RW_FUNCTION(0x6F3DDC, "SceneFlashDrama::next_scene::$_11 ~__func() (deleting)", SceneFlashDrama_lambdas::next_scene_11::deleteThis);
RW_FUNCTION(0x6F3DE4, "SceneFlashDrama::next_scene::$_11 __clone()", SceneFlashDrama_lambdas::next_scene_11::clone);
RW_FUNCTION(0x6F3E14, "SceneFlashDrama::next_scene::$_11 __clone(__base*)", SceneFlashDrama_lambdas::next_scene_11::cloneInto);
RW_FUNCTION(0x6F3E3C, "SceneFlashDrama::next_scene::$_11 destroy()", SceneFlashDrama_lambdas::next_scene_11::destroy);
RW_FUNCTION(0x6F3E3E, "SceneFlashDrama::next_scene::$_11 destroy_deallocate()", SceneFlashDrama_lambdas::next_scene_11::deleteThis);
RW_FUNCTION(0x6F3E54, "SceneFlashDrama::next_scene::$_11 target(const type_info&)", SceneFlashDrama_lambdas::next_scene_11::target);
RW_FUNCTION(0x6F3E74, "SceneFlashDrama::next_scene::$_11 target_type()", SceneFlashDrama_lambdas::next_scene_11::targetType);
RW_FUNCTION(0x6F3E88, "SceneFlashDrama::update::$_10 ~__func() (deleting)", SceneFlashDrama_lambdas::update_10::deleteThis);
RW_FUNCTION(0x6F3E90, "SceneFlashDrama::update::$_10 __clone()", SceneFlashDrama_lambdas::update_10::clone);
RW_FUNCTION(0x6F3EC0, "SceneFlashDrama::update::$_10 __clone(__base*)", SceneFlashDrama_lambdas::update_10::cloneInto);
RW_FUNCTION(0x6F3EE8, "SceneFlashDrama::update::$_10 destroy()", SceneFlashDrama_lambdas::update_10::destroy);
RW_FUNCTION(0x6F3EEA, "SceneFlashDrama::update::$_10 destroy_deallocate()", SceneFlashDrama_lambdas::update_10::deleteThis);
RW_FUNCTION(0x6F3F00, "SceneFlashDrama::update::$_10 target(const type_info&)", SceneFlashDrama_lambdas::update_10::target);
RW_FUNCTION(0x6F3F20, "SceneFlashDrama::update::$_10 target_type()", SceneFlashDrama_lambdas::update_10::targetType);
RW_FUNCTION(0x6F3F34, "SceneFlashDrama::init::$_9 ~__func() (deleting)", SceneFlashDrama_lambdas::init_9::deleteThis);
RW_FUNCTION(0x6F3F3C, "SceneFlashDrama::init::$_9 __clone()", SceneFlashDrama_lambdas::init_9::clone);
RW_FUNCTION(0x6F3F68, "SceneFlashDrama::init::$_9 __clone(__base*)", SceneFlashDrama_lambdas::init_9::cloneInto);
RW_FUNCTION(0x6F3F88, "SceneFlashDrama::init::$_9 destroy()", SceneFlashDrama_lambdas::init_9::destroy);
RW_FUNCTION(0x6F3F8A, "SceneFlashDrama::init::$_9 destroy_deallocate()", SceneFlashDrama_lambdas::init_9::deleteThis);
RW_FUNCTION(0x6F3F9C, "SceneFlashDrama::init::$_9 target(const type_info&)", SceneFlashDrama_lambdas::init_9::target);
RW_FUNCTION(0x6F3FBC, "SceneFlashDrama::init::$_9 target_type()", SceneFlashDrama_lambdas::init_9::targetType);
RW_FUNCTION(0x6F3FD0, "SceneFlashDrama::init::$_8 ~__func() (deleting)", SceneFlashDrama_lambdas::init_8::deleteThis);
RW_FUNCTION(0x6F3FD8, "SceneFlashDrama::init::$_8 __clone()", SceneFlashDrama_lambdas::init_8::clone);
RW_FUNCTION(0x6F4004, "SceneFlashDrama::init::$_8 __clone(__base*)", SceneFlashDrama_lambdas::init_8::cloneInto);
RW_FUNCTION(0x6F4024, "SceneFlashDrama::init::$_8 destroy()", SceneFlashDrama_lambdas::init_8::destroy);
RW_FUNCTION(0x6F4026, "SceneFlashDrama::init::$_8 destroy_deallocate()", SceneFlashDrama_lambdas::init_8::deleteThis);
RW_FUNCTION(0x6F4038, "SceneFlashDrama::init::$_8 target(const type_info&)", SceneFlashDrama_lambdas::init_8::target);
RW_FUNCTION(0x6F4058, "SceneFlashDrama::init::$_8 target_type()", SceneFlashDrama_lambdas::init_8::targetType);
RW_FUNCTION(0x6F406C, "SceneFlashDrama::init::$_7 ~__func() (deleting)", SceneFlashDrama_lambdas::init_7::deleteThis);
RW_FUNCTION(0x6F4074, "SceneFlashDrama::init::$_7 __clone()", SceneFlashDrama_lambdas::init_7::clone);
RW_FUNCTION(0x6F40A0, "SceneFlashDrama::init::$_7 __clone(__base*)", SceneFlashDrama_lambdas::init_7::cloneInto);
RW_FUNCTION(0x6F40C0, "SceneFlashDrama::init::$_7 destroy()", SceneFlashDrama_lambdas::init_7::destroy);
RW_FUNCTION(0x6F40C2, "SceneFlashDrama::init::$_7 destroy_deallocate()", SceneFlashDrama_lambdas::init_7::deleteThis);
RW_FUNCTION(0x6F41E8, "SceneFlashDrama::init::$_7 target(const type_info&)", SceneFlashDrama_lambdas::init_7::target);
RW_FUNCTION(0x6F4208, "SceneFlashDrama::init::$_7 target_type()", SceneFlashDrama_lambdas::init_7::targetType);
RW_FUNCTION(0x6F421C, "SceneFlashDrama::init::$_6 ~__func() (deleting)", SceneFlashDrama_lambdas::init_6::deleteThis);
RW_FUNCTION(0x6F4224, "SceneFlashDrama::init::$_6 __clone()", SceneFlashDrama_lambdas::init_6::clone);
RW_FUNCTION(0x6F4250, "SceneFlashDrama::init::$_6 __clone(__base*)", SceneFlashDrama_lambdas::init_6::cloneInto);
RW_FUNCTION(0x6F4270, "SceneFlashDrama::init::$_6 destroy()", SceneFlashDrama_lambdas::init_6::destroy);
RW_FUNCTION(0x6F4272, "SceneFlashDrama::init::$_6 destroy_deallocate()", SceneFlashDrama_lambdas::init_6::deleteThis);
RW_FUNCTION(0x6F4298, "SceneFlashDrama::init::$_6 target(const type_info&)", SceneFlashDrama_lambdas::init_6::target);
RW_FUNCTION(0x6F42B8, "SceneFlashDrama::init::$_6 target_type()", SceneFlashDrama_lambdas::init_6::targetType);
