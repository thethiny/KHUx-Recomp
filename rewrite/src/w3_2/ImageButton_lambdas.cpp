// ImageButton: the std::function plumbing of its lambdas (libc++ __func slots: deleting destructor, clone, clone into a
// buffer, destroy, destroy_deallocate, target, target_type), generated from the binary's __func vtables (size,
// captured bytes, type_info, type name and target offset read from each slot's instructions), plus the lambda bodies.
#include "../scene_main/FunctionThunks.hpp"

namespace ImageButton_w3 {

// ImageButton::init::$_3
using init_3 = thunk::Lambda<0xF573E8, 8, 0xF868E8, 0xE286A0, 4, thunk::cap(4, 4)>;
// ImageButton::init::$_2
using init_2_f57414 = thunk::Lambda<0xF57414, 8, 0xF868FC, 0xE287A0, 4, thunk::cap(4, 4)>;
// ImageButton::init::$_1
using init_1 = thunk::Lambda<0xF57440, 16, 0xF86910, 0xE288A0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;

}  // namespace ImageButton_w3

RW_FUNCTION(0x531B98, "ImageButton::init::$_3 ~__func() (deleting)", ImageButton_w3::init_3::deleteThis);
RW_FUNCTION(0x531BA0, "ImageButton::init::$_3 __clone()", ImageButton_w3::init_3::clone);
RW_FUNCTION(0x531BCC, "ImageButton::init::$_3 __clone(__base*)", ImageButton_w3::init_3::cloneInto);
RW_FUNCTION(0x531BEC, "ImageButton::init::$_3 destroy()", ImageButton_w3::init_3::destroy);
RW_FUNCTION(0x531BEE, "ImageButton::init::$_3 destroy_deallocate()", ImageButton_w3::init_3::deleteThis);
RW_FUNCTION(0x531C28, "ImageButton::init::$_3 target(const type_info&)", ImageButton_w3::init_3::target);
RW_FUNCTION(0x531C48, "ImageButton::init::$_3 target_type()", ImageButton_w3::init_3::targetType);
RW_FUNCTION(0x531CE0, "ImageButton::init::$_2 ~__func() (deleting)", ImageButton_w3::init_2_f57414::deleteThis);
RW_FUNCTION(0x531CE8, "ImageButton::init::$_2 __clone()", ImageButton_w3::init_2_f57414::clone);
RW_FUNCTION(0x531D14, "ImageButton::init::$_2 __clone(__base*)", ImageButton_w3::init_2_f57414::cloneInto);
RW_FUNCTION(0x531D34, "ImageButton::init::$_2 destroy()", ImageButton_w3::init_2_f57414::destroy);
RW_FUNCTION(0x531D36, "ImageButton::init::$_2 destroy_deallocate()", ImageButton_w3::init_2_f57414::deleteThis);
RW_FUNCTION(0x531D88, "ImageButton::init::$_2 target(const type_info&)", ImageButton_w3::init_2_f57414::target);
RW_FUNCTION(0x531DA8, "ImageButton::init::$_2 target_type()", ImageButton_w3::init_2_f57414::targetType);
RW_FUNCTION(0x531DBC, "ImageButton::init::$_1 ~__func() (deleting)", ImageButton_w3::init_1::deleteThis);
RW_FUNCTION(0x531DC4, "ImageButton::init::$_1 __clone()", ImageButton_w3::init_1::clone);
RW_FUNCTION(0x531DF4, "ImageButton::init::$_1 __clone(__base*)", ImageButton_w3::init_1::cloneInto);
RW_FUNCTION(0x531E1C, "ImageButton::init::$_1 destroy()", ImageButton_w3::init_1::destroy);
RW_FUNCTION(0x531E1E, "ImageButton::init::$_1 destroy_deallocate()", ImageButton_w3::init_1::deleteThis);
