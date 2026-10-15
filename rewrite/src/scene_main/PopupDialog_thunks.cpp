// PopupDialog: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace PopupDialog_lambdas {

// PopupDialog::close::$_4
using close_4 = thunk::Lambda<0xF55944, 8, 0xF7FEF4, 0xE1F280, 4, thunk::cap(4, 4)>;
// PopupDialog::open::$_3
using open_3 = thunk::Lambda<0xF55970, 8, 0xF7FF08, 0xE1F350, 4, thunk::cap(4, 4)>;
// PopupDialog::open::$_2
using open_2 = thunk::Lambda<0xF5599C, 8, 0xF7FF1C, 0xE1F490, 4, thunk::cap(4, 4)>;
// PopupDialog::init::$_1
using init_1 = thunk::Lambda<0xF559C8, 8, 0xF7FF30, 0xE1F5C0, 4, thunk::cap(4, 4)>;
// PopupDialog::init::$_0
using init_0 = thunk::Lambda<0xF559F4, 8, 0xF7FF44, 0xE1F6A0, 4, thunk::cap(4, 4)>;

}  // namespace PopupDialog_lambdas

RW_FUNCTION(0x4F46A0, "PopupDialog::close::$_4 ~__func() (deleting)", PopupDialog_lambdas::close_4::deleteThis);
RW_FUNCTION(0x4F46A8, "PopupDialog::close::$_4 __clone()", PopupDialog_lambdas::close_4::clone);
RW_FUNCTION(0x4F46D4, "PopupDialog::close::$_4 __clone(__base*)", PopupDialog_lambdas::close_4::cloneInto);
RW_FUNCTION(0x4F46F4, "PopupDialog::close::$_4 destroy()", PopupDialog_lambdas::close_4::destroy);
RW_FUNCTION(0x4F46F6, "PopupDialog::close::$_4 destroy_deallocate()", PopupDialog_lambdas::close_4::deleteThis);
RW_FUNCTION(0x4F4714, "PopupDialog::close::$_4 target(const type_info&)", PopupDialog_lambdas::close_4::target);
RW_FUNCTION(0x4F4734, "PopupDialog::close::$_4 target_type()", PopupDialog_lambdas::close_4::targetType);
RW_FUNCTION(0x4F4748, "PopupDialog::open::$_3 ~__func() (deleting)", PopupDialog_lambdas::open_3::deleteThis);
RW_FUNCTION(0x4F4750, "PopupDialog::open::$_3 __clone()", PopupDialog_lambdas::open_3::clone);
RW_FUNCTION(0x4F477C, "PopupDialog::open::$_3 __clone(__base*)", PopupDialog_lambdas::open_3::cloneInto);
RW_FUNCTION(0x4F479C, "PopupDialog::open::$_3 destroy()", PopupDialog_lambdas::open_3::destroy);
RW_FUNCTION(0x4F479E, "PopupDialog::open::$_3 destroy_deallocate()", PopupDialog_lambdas::open_3::deleteThis);
RW_FUNCTION(0x4F47D0, "PopupDialog::open::$_3 target(const type_info&)", PopupDialog_lambdas::open_3::target);
RW_FUNCTION(0x4F47F0, "PopupDialog::open::$_3 target_type()", PopupDialog_lambdas::open_3::targetType);
RW_FUNCTION(0x4F4804, "PopupDialog::open::$_2 ~__func() (deleting)", PopupDialog_lambdas::open_2::deleteThis);
RW_FUNCTION(0x4F480C, "PopupDialog::open::$_2 __clone()", PopupDialog_lambdas::open_2::clone);
RW_FUNCTION(0x4F4838, "PopupDialog::open::$_2 __clone(__base*)", PopupDialog_lambdas::open_2::cloneInto);
RW_FUNCTION(0x4F4858, "PopupDialog::open::$_2 destroy()", PopupDialog_lambdas::open_2::destroy);
RW_FUNCTION(0x4F485A, "PopupDialog::open::$_2 destroy_deallocate()", PopupDialog_lambdas::open_2::deleteThis);
RW_FUNCTION(0x4F4878, "PopupDialog::open::$_2 target(const type_info&)", PopupDialog_lambdas::open_2::target);
RW_FUNCTION(0x4F4898, "PopupDialog::open::$_2 target_type()", PopupDialog_lambdas::open_2::targetType);
RW_FUNCTION(0x4F48AC, "PopupDialog::init::$_1 ~__func() (deleting)", PopupDialog_lambdas::init_1::deleteThis);
RW_FUNCTION(0x4F48B4, "PopupDialog::init::$_1 __clone()", PopupDialog_lambdas::init_1::clone);
RW_FUNCTION(0x4F48E0, "PopupDialog::init::$_1 __clone(__base*)", PopupDialog_lambdas::init_1::cloneInto);
RW_FUNCTION(0x4F4900, "PopupDialog::init::$_1 destroy()", PopupDialog_lambdas::init_1::destroy);
RW_FUNCTION(0x4F4902, "PopupDialog::init::$_1 destroy_deallocate()", PopupDialog_lambdas::init_1::deleteThis);
RW_FUNCTION(0x4F492C, "PopupDialog::init::$_1 target(const type_info&)", PopupDialog_lambdas::init_1::target);
RW_FUNCTION(0x4F494C, "PopupDialog::init::$_1 target_type()", PopupDialog_lambdas::init_1::targetType);
RW_FUNCTION(0x4F4960, "PopupDialog::init::$_0 ~__func() (deleting)", PopupDialog_lambdas::init_0::deleteThis);
RW_FUNCTION(0x4F4968, "PopupDialog::init::$_0 __clone()", PopupDialog_lambdas::init_0::clone);
RW_FUNCTION(0x4F4994, "PopupDialog::init::$_0 __clone(__base*)", PopupDialog_lambdas::init_0::cloneInto);
RW_FUNCTION(0x4F49B4, "PopupDialog::init::$_0 destroy()", PopupDialog_lambdas::init_0::destroy);
RW_FUNCTION(0x4F49B6, "PopupDialog::init::$_0 destroy_deallocate()", PopupDialog_lambdas::init_0::deleteThis);
RW_FUNCTION(0x4F49E0, "PopupDialog::init::$_0 target(const type_info&)", PopupDialog_lambdas::init_0::target);
RW_FUNCTION(0x4F4A00, "PopupDialog::init::$_0 target_type()", PopupDialog_lambdas::init_0::targetType);
