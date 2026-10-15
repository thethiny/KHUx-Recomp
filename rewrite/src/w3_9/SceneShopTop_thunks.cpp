// SceneShopTop: the std::function slots of its lambdas (libc++ __func vtables). Each vtable's slots were read from
// the binary and checked against the lifted code: sizes and captured words from __clone, the type name pointer
// from target() and the typeinfo from target_type(). The lambda bodies (operator()) are in SceneShopTop_lambdas.cpp.
#include "../scene_main/FunctionThunks.hpp"

namespace SceneShopTop_lambdas {

// SceneShopTop::backScene::$_13
using backScene_13 = thunk::Lambda<0xF64D38, 8, 0xF9CD9C, 0xE71CD0, 4>;
// SceneShopTop::initShopTopBottomButton::$_12
using bottom_12 = thunk::Lambda<0xF64D64, 8, 0xF9CDB0, 0xE71DA0, 4, thunk::cap(4, 4)>;
// SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1}
using bottom_12_inner = thunk::Lambda<0xF64D90, 8, 0xF9CDC4, 0xE71EA0, 4>;
// SceneShopTop::initShopTopBottomButton::$_11
using bottom_11 = thunk::Lambda<0xF64DBC, 8, 0xF9CDD8, 0xE71FC0, 4, thunk::cap(4, 4)>;
// SceneShopTop::initShopTopBottomButton::$_10
using bottom_10 = thunk::Lambda<0xF64DE8, 8, 0xF9CDEC, 0xE720B0, 4, thunk::cap(4, 4)>;
// SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1}
using bottom_10_inner = thunk::Lambda<0xF64E14, 8, 0xF9CE00, 0xE721B0, 4>;
// SceneShopTop::initShopTopBottomButton::$_8
using bottom_8 = thunk::Lambda<0xF64E40, 24, 0xF9CE14, 0xE722D0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4)>;
// SceneShopTop::initShopTopBottomButton::$_9
using bottom_9 = thunk::Lambda<0xF64E6C, 16, 0xF9CE28, 0xE723A0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneShopTop::initShopTopBottomButton::$_14
using bottom_14 = thunk::Lambda<0xF64E98, 8, 0xF9CE3C, 0xE72470, 4, thunk::cap(4, 4)>;
// SceneShopTop::initShopTopBottomButton::$_7
using bottom_7 = thunk::Lambda<0xF64EC4, 8, 0xF9CE50, 0xE72560, 4, thunk::cap(4, 4)>;
// SceneShopTop::initShopTop::$_6
using initShopTop_6 = thunk::Lambda<0xF64EF0, 8, 0xF9CE64, 0xE72620, 4, thunk::cap(4, 4)>;
// SceneShopTop::initShopTop::$_6::{lambda()#1}
using initShopTop_6_inner = thunk::Lambda<0xF64F1C, 8, 0xF9CE78, 0xE726E0, 4>;
// SceneShopTop::runInAnimation::$_5
using runIn_5 = thunk::Lambda<0xF64F48, 8, 0xF9CE8C, 0xE72790, 4, thunk::cap(4, 4)>;
// SceneShopTop::runInAnimation::$_5::{lambda()#2}
using runIn_5_inner2 = thunk::Lambda<0xF64F74, 8, 0xF9CEA0, 0xE72830, 4, thunk::cap(4, 4)>;
// SceneShopTop::init::$_4
using init_4 = thunk::Lambda<0xF64FA0, 8, 0xF9CEB4, 0xE728E0, 4>;
// SceneShopTop::init::$_3
using init_3 = thunk::Lambda<0xF64FCC, 8, 0xF9CEC8, 0xE72960, 4, thunk::cap(4, 4)>;
// SceneShopTop::init::$_2
using init_2 = thunk::Lambda<0xF64FF8, 8, 0xF9CEDC, 0xE729E0, 4, thunk::cap(4, 4)>;
// SceneShopTop::init::$_1
using init_1 = thunk::Lambda<0xF65024, 8, 0xF9CEF0, 0xE72A60, 4, thunk::cap(4, 4)>;
// SceneShopTop::callShopAPI::$_0 (captures a std::function<void(bool)> at +16, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using callShopAPI_0 = thunk::LambdaFn<0xF65050, 0x28, 0xF9CF04, 0xE72AF0, 8, 16, 0x45C84D>;

}  // namespace SceneShopTop_lambdas

RW_FUNCTION(0x68D0AC, "SceneShopTop::backScene::$_13 ~__func() (deleting)", SceneShopTop_lambdas::backScene_13::deleteThis);
RW_FUNCTION(0x68D0B4, "SceneShopTop::backScene::$_13 __clone()", SceneShopTop_lambdas::backScene_13::clone);
RW_FUNCTION(0x68D0D8, "SceneShopTop::backScene::$_13 __clone(__base*)", SceneShopTop_lambdas::backScene_13::cloneInto);
RW_FUNCTION(0x68D0F4, "SceneShopTop::backScene::$_13 destroy()", SceneShopTop_lambdas::backScene_13::destroy);
RW_FUNCTION(0x68D0F6, "SceneShopTop::backScene::$_13 destroy_deallocate()", SceneShopTop_lambdas::backScene_13::deleteThis);
RW_FUNCTION(0x68D220, "SceneShopTop::backScene::$_13 target(const type_info&)", SceneShopTop_lambdas::backScene_13::target);
RW_FUNCTION(0x68D240, "SceneShopTop::backScene::$_13 target_type()", SceneShopTop_lambdas::backScene_13::targetType);
RW_FUNCTION(0x68D254, "SceneShopTop::initShopTopBottomButton::$_12 ~__func() (deleting)", SceneShopTop_lambdas::bottom_12::deleteThis);
RW_FUNCTION(0x68D25C, "SceneShopTop::initShopTopBottomButton::$_12 __clone()", SceneShopTop_lambdas::bottom_12::clone);
RW_FUNCTION(0x68D288, "SceneShopTop::initShopTopBottomButton::$_12 __clone(__base*)", SceneShopTop_lambdas::bottom_12::cloneInto);
RW_FUNCTION(0x68D2A8, "SceneShopTop::initShopTopBottomButton::$_12 destroy()", SceneShopTop_lambdas::bottom_12::destroy);
RW_FUNCTION(0x68D2AA, "SceneShopTop::initShopTopBottomButton::$_12 destroy_deallocate()", SceneShopTop_lambdas::bottom_12::deleteThis);
RW_FUNCTION(0x68D364, "SceneShopTop::initShopTopBottomButton::$_12 target(const type_info&)", SceneShopTop_lambdas::bottom_12::target);
RW_FUNCTION(0x68D384, "SceneShopTop::initShopTopBottomButton::$_12 target_type()", SceneShopTop_lambdas::bottom_12::targetType);
RW_FUNCTION(0x68D398, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} ~__func()", SceneShopTop_lambdas::bottom_12_inner::destructor);
RW_FUNCTION(0x68D39A, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} ~__func() (deleting)", SceneShopTop_lambdas::bottom_12_inner::deleteThis);
RW_FUNCTION(0x68D3A4, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} __clone()", SceneShopTop_lambdas::bottom_12_inner::clone);
RW_FUNCTION(0x68D3C8, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} __clone(__base*)", SceneShopTop_lambdas::bottom_12_inner::cloneInto);
RW_FUNCTION(0x68D3E4, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} destroy()", SceneShopTop_lambdas::bottom_12_inner::destroy);
RW_FUNCTION(0x68D3E6, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} destroy_deallocate()", SceneShopTop_lambdas::bottom_12_inner::deleteThis);
RW_FUNCTION(0x68D480, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} target(const type_info&)", SceneShopTop_lambdas::bottom_12_inner::target);
RW_FUNCTION(0x68D4A0, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1} target_type()", SceneShopTop_lambdas::bottom_12_inner::targetType);
RW_FUNCTION(0x68D4B4, "SceneShopTop::initShopTopBottomButton::$_11 ~__func() (deleting)", SceneShopTop_lambdas::bottom_11::deleteThis);
RW_FUNCTION(0x68D4BC, "SceneShopTop::initShopTopBottomButton::$_11 __clone()", SceneShopTop_lambdas::bottom_11::clone);
RW_FUNCTION(0x68D4E8, "SceneShopTop::initShopTopBottomButton::$_11 __clone(__base*)", SceneShopTop_lambdas::bottom_11::cloneInto);
RW_FUNCTION(0x68D508, "SceneShopTop::initShopTopBottomButton::$_11 destroy()", SceneShopTop_lambdas::bottom_11::destroy);
RW_FUNCTION(0x68D50A, "SceneShopTop::initShopTopBottomButton::$_11 destroy_deallocate()", SceneShopTop_lambdas::bottom_11::deleteThis);
RW_FUNCTION(0x68D5B0, "SceneShopTop::initShopTopBottomButton::$_11 target(const type_info&)", SceneShopTop_lambdas::bottom_11::target);
RW_FUNCTION(0x68D5D0, "SceneShopTop::initShopTopBottomButton::$_11 target_type()", SceneShopTop_lambdas::bottom_11::targetType);
RW_FUNCTION(0x68D5E4, "SceneShopTop::initShopTopBottomButton::$_10 ~__func() (deleting)", SceneShopTop_lambdas::bottom_10::deleteThis);
RW_FUNCTION(0x68D5EC, "SceneShopTop::initShopTopBottomButton::$_10 __clone()", SceneShopTop_lambdas::bottom_10::clone);
RW_FUNCTION(0x68D618, "SceneShopTop::initShopTopBottomButton::$_10 __clone(__base*)", SceneShopTop_lambdas::bottom_10::cloneInto);
RW_FUNCTION(0x68D638, "SceneShopTop::initShopTopBottomButton::$_10 destroy()", SceneShopTop_lambdas::bottom_10::destroy);
RW_FUNCTION(0x68D63A, "SceneShopTop::initShopTopBottomButton::$_10 destroy_deallocate()", SceneShopTop_lambdas::bottom_10::deleteThis);
RW_FUNCTION(0x68D700, "SceneShopTop::initShopTopBottomButton::$_10 target(const type_info&)", SceneShopTop_lambdas::bottom_10::target);
RW_FUNCTION(0x68D720, "SceneShopTop::initShopTopBottomButton::$_10 target_type()", SceneShopTop_lambdas::bottom_10::targetType);
RW_FUNCTION(0x68D734, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1} ~__func() (deleting)", SceneShopTop_lambdas::bottom_10_inner::deleteThis);
RW_FUNCTION(0x68D73C, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1} __clone()", SceneShopTop_lambdas::bottom_10_inner::clone);
RW_FUNCTION(0x68D760, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1} __clone(__base*)", SceneShopTop_lambdas::bottom_10_inner::cloneInto);
RW_FUNCTION(0x68D77C, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1} destroy()", SceneShopTop_lambdas::bottom_10_inner::destroy);
RW_FUNCTION(0x68D77E, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1} destroy_deallocate()", SceneShopTop_lambdas::bottom_10_inner::deleteThis);
RW_FUNCTION(0x68D830, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1} target(const type_info&)", SceneShopTop_lambdas::bottom_10_inner::target);
RW_FUNCTION(0x68D850, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1} target_type()", SceneShopTop_lambdas::bottom_10_inner::targetType);
RW_FUNCTION(0x68D86C, "SceneShopTop::initShopTopBottomButton::$_8 ~__func() (deleting)", SceneShopTop_lambdas::bottom_8::deleteThis);
RW_FUNCTION(0x68D874, "SceneShopTop::initShopTopBottomButton::$_8 __clone()", SceneShopTop_lambdas::bottom_8::clone);
RW_FUNCTION(0x68D8A8, "SceneShopTop::initShopTopBottomButton::$_8 __clone(__base*)", SceneShopTop_lambdas::bottom_8::cloneInto);
RW_FUNCTION(0x68D8D0, "SceneShopTop::initShopTopBottomButton::$_8 destroy()", SceneShopTop_lambdas::bottom_8::destroy);
RW_FUNCTION(0x68D8D2, "SceneShopTop::initShopTopBottomButton::$_8 destroy_deallocate()", SceneShopTop_lambdas::bottom_8::deleteThis);
RW_FUNCTION(0x68DA0C, "SceneShopTop::initShopTopBottomButton::$_8 target(const type_info&)", SceneShopTop_lambdas::bottom_8::target);
RW_FUNCTION(0x68DA2C, "SceneShopTop::initShopTopBottomButton::$_8 target_type()", SceneShopTop_lambdas::bottom_8::targetType);
RW_FUNCTION(0x68DA40, "SceneShopTop::initShopTopBottomButton::$_9 ~__func() (deleting)", SceneShopTop_lambdas::bottom_9::deleteThis);
RW_FUNCTION(0x68DA48, "SceneShopTop::initShopTopBottomButton::$_9 __clone()", SceneShopTop_lambdas::bottom_9::clone);
RW_FUNCTION(0x68DA78, "SceneShopTop::initShopTopBottomButton::$_9 __clone(__base*)", SceneShopTop_lambdas::bottom_9::cloneInto);
RW_FUNCTION(0x68DAA0, "SceneShopTop::initShopTopBottomButton::$_9 destroy()", SceneShopTop_lambdas::bottom_9::destroy);
RW_FUNCTION(0x68DAA2, "SceneShopTop::initShopTopBottomButton::$_9 destroy_deallocate()", SceneShopTop_lambdas::bottom_9::deleteThis);
RW_FUNCTION(0x68DAB4, "SceneShopTop::initShopTopBottomButton::$_9 target(const type_info&)", SceneShopTop_lambdas::bottom_9::target);
RW_FUNCTION(0x68DAD4, "SceneShopTop::initShopTopBottomButton::$_9 target_type()", SceneShopTop_lambdas::bottom_9::targetType);
RW_FUNCTION(0x68DAE8, "SceneShopTop::initShopTopBottomButton::$_14 ~__func() (deleting)", SceneShopTop_lambdas::bottom_14::deleteThis);
RW_FUNCTION(0x68DAF0, "SceneShopTop::initShopTopBottomButton::$_14 __clone()", SceneShopTop_lambdas::bottom_14::clone);
RW_FUNCTION(0x68DB1C, "SceneShopTop::initShopTopBottomButton::$_14 __clone(__base*)", SceneShopTop_lambdas::bottom_14::cloneInto);
RW_FUNCTION(0x68DB3C, "SceneShopTop::initShopTopBottomButton::$_14 destroy()", SceneShopTop_lambdas::bottom_14::destroy);
RW_FUNCTION(0x68DB3E, "SceneShopTop::initShopTopBottomButton::$_14 destroy_deallocate()", SceneShopTop_lambdas::bottom_14::deleteThis);
RW_FUNCTION(0x68DB64, "SceneShopTop::initShopTopBottomButton::$_14 target(const type_info&)", SceneShopTop_lambdas::bottom_14::target);
RW_FUNCTION(0x68DB84, "SceneShopTop::initShopTopBottomButton::$_14 target_type()", SceneShopTop_lambdas::bottom_14::targetType);
RW_FUNCTION(0x68DB98, "SceneShopTop::initShopTopBottomButton::$_7 ~__func() (deleting)", SceneShopTop_lambdas::bottom_7::deleteThis);
RW_FUNCTION(0x68DBA0, "SceneShopTop::initShopTopBottomButton::$_7 __clone()", SceneShopTop_lambdas::bottom_7::clone);
RW_FUNCTION(0x68DBCC, "SceneShopTop::initShopTopBottomButton::$_7 __clone(__base*)", SceneShopTop_lambdas::bottom_7::cloneInto);
RW_FUNCTION(0x68DBEC, "SceneShopTop::initShopTopBottomButton::$_7 destroy()", SceneShopTop_lambdas::bottom_7::destroy);
RW_FUNCTION(0x68DBEE, "SceneShopTop::initShopTopBottomButton::$_7 destroy_deallocate()", SceneShopTop_lambdas::bottom_7::deleteThis);
RW_FUNCTION(0x68DC28, "SceneShopTop::initShopTopBottomButton::$_7 target(const type_info&)", SceneShopTop_lambdas::bottom_7::target);
RW_FUNCTION(0x68DC48, "SceneShopTop::initShopTopBottomButton::$_7 target_type()", SceneShopTop_lambdas::bottom_7::targetType);
RW_FUNCTION(0x68DC5C, "SceneShopTop::initShopTop::$_6 ~__func() (deleting)", SceneShopTop_lambdas::initShopTop_6::deleteThis);
RW_FUNCTION(0x68DC64, "SceneShopTop::initShopTop::$_6 __clone()", SceneShopTop_lambdas::initShopTop_6::clone);
RW_FUNCTION(0x68DC90, "SceneShopTop::initShopTop::$_6 __clone(__base*)", SceneShopTop_lambdas::initShopTop_6::cloneInto);
RW_FUNCTION(0x68DCB0, "SceneShopTop::initShopTop::$_6 destroy()", SceneShopTop_lambdas::initShopTop_6::destroy);
RW_FUNCTION(0x68DCB2, "SceneShopTop::initShopTop::$_6 destroy_deallocate()", SceneShopTop_lambdas::initShopTop_6::deleteThis);
RW_FUNCTION(0x68DD8C, "SceneShopTop::initShopTop::$_6 target(const type_info&)", SceneShopTop_lambdas::initShopTop_6::target);
RW_FUNCTION(0x68DDAC, "SceneShopTop::initShopTop::$_6 target_type()", SceneShopTop_lambdas::initShopTop_6::targetType);
RW_FUNCTION(0x68DDC0, "SceneShopTop::initShopTop::$_6::{lambda()#1} ~__func() (deleting)", SceneShopTop_lambdas::initShopTop_6_inner::deleteThis);
RW_FUNCTION(0x68DDC8, "SceneShopTop::initShopTop::$_6::{lambda()#1} __clone()", SceneShopTop_lambdas::initShopTop_6_inner::clone);
RW_FUNCTION(0x68DDEC, "SceneShopTop::initShopTop::$_6::{lambda()#1} __clone(__base*)", SceneShopTop_lambdas::initShopTop_6_inner::cloneInto);
RW_FUNCTION(0x68DE08, "SceneShopTop::initShopTop::$_6::{lambda()#1} destroy()", SceneShopTop_lambdas::initShopTop_6_inner::destroy);
RW_FUNCTION(0x68DE0A, "SceneShopTop::initShopTop::$_6::{lambda()#1} destroy_deallocate()", SceneShopTop_lambdas::initShopTop_6_inner::deleteThis);
RW_FUNCTION(0x68DEC0, "SceneShopTop::initShopTop::$_6::{lambda()#1} target(const type_info&)", SceneShopTop_lambdas::initShopTop_6_inner::target);
RW_FUNCTION(0x68DEE0, "SceneShopTop::initShopTop::$_6::{lambda()#1} target_type()", SceneShopTop_lambdas::initShopTop_6_inner::targetType);
RW_FUNCTION(0x68DFC8, "SceneShopTop::runInAnimation::$_5 ~__func() (deleting)", SceneShopTop_lambdas::runIn_5::deleteThis);
RW_FUNCTION(0x68DFD0, "SceneShopTop::runInAnimation::$_5 __clone()", SceneShopTop_lambdas::runIn_5::clone);
RW_FUNCTION(0x68DFFC, "SceneShopTop::runInAnimation::$_5 __clone(__base*)", SceneShopTop_lambdas::runIn_5::cloneInto);
RW_FUNCTION(0x68E01C, "SceneShopTop::runInAnimation::$_5 destroy()", SceneShopTop_lambdas::runIn_5::destroy);
RW_FUNCTION(0x68E01E, "SceneShopTop::runInAnimation::$_5 destroy_deallocate()", SceneShopTop_lambdas::runIn_5::deleteThis);
RW_FUNCTION(0x68E2FC, "SceneShopTop::runInAnimation::$_5 target(const type_info&)", SceneShopTop_lambdas::runIn_5::target);
RW_FUNCTION(0x68E31C, "SceneShopTop::runInAnimation::$_5 target_type()", SceneShopTop_lambdas::runIn_5::targetType);
RW_FUNCTION(0x68E330, "SceneShopTop::runInAnimation::$_5::{lambda()#2} ~__func() (deleting)", SceneShopTop_lambdas::runIn_5_inner2::deleteThis);
RW_FUNCTION(0x68E338, "SceneShopTop::runInAnimation::$_5::{lambda()#2} __clone()", SceneShopTop_lambdas::runIn_5_inner2::clone);
RW_FUNCTION(0x68E364, "SceneShopTop::runInAnimation::$_5::{lambda()#2} __clone(__base*)", SceneShopTop_lambdas::runIn_5_inner2::cloneInto);
RW_FUNCTION(0x68E384, "SceneShopTop::runInAnimation::$_5::{lambda()#2} destroy()", SceneShopTop_lambdas::runIn_5_inner2::destroy);
RW_FUNCTION(0x68E386, "SceneShopTop::runInAnimation::$_5::{lambda()#2} destroy_deallocate()", SceneShopTop_lambdas::runIn_5_inner2::deleteThis);
RW_FUNCTION(0x68E3B0, "SceneShopTop::runInAnimation::$_5::{lambda()#2} target(const type_info&)", SceneShopTop_lambdas::runIn_5_inner2::target);
RW_FUNCTION(0x68E3D0, "SceneShopTop::runInAnimation::$_5::{lambda()#2} target_type()", SceneShopTop_lambdas::runIn_5_inner2::targetType);
RW_FUNCTION(0x68E3E4, "SceneShopTop::init::$_4 ~__func() (deleting)", SceneShopTop_lambdas::init_4::deleteThis);
RW_FUNCTION(0x68E3EC, "SceneShopTop::init::$_4 __clone()", SceneShopTop_lambdas::init_4::clone);
RW_FUNCTION(0x68E410, "SceneShopTop::init::$_4 __clone(__base*)", SceneShopTop_lambdas::init_4::cloneInto);
RW_FUNCTION(0x68E42C, "SceneShopTop::init::$_4 destroy()", SceneShopTop_lambdas::init_4::destroy);
RW_FUNCTION(0x68E42E, "SceneShopTop::init::$_4 destroy_deallocate()", SceneShopTop_lambdas::init_4::deleteThis);
RW_FUNCTION(0x68E43C, "SceneShopTop::init::$_4 target(const type_info&)", SceneShopTop_lambdas::init_4::target);
RW_FUNCTION(0x68E45C, "SceneShopTop::init::$_4 target_type()", SceneShopTop_lambdas::init_4::targetType);
RW_FUNCTION(0x68E470, "SceneShopTop::init::$_3 ~__func() (deleting)", SceneShopTop_lambdas::init_3::deleteThis);
RW_FUNCTION(0x68E478, "SceneShopTop::init::$_3 __clone()", SceneShopTop_lambdas::init_3::clone);
RW_FUNCTION(0x68E4A4, "SceneShopTop::init::$_3 __clone(__base*)", SceneShopTop_lambdas::init_3::cloneInto);
RW_FUNCTION(0x68E4C4, "SceneShopTop::init::$_3 destroy()", SceneShopTop_lambdas::init_3::destroy);
RW_FUNCTION(0x68E4C6, "SceneShopTop::init::$_3 destroy_deallocate()", SceneShopTop_lambdas::init_3::deleteThis);
RW_FUNCTION(0x68E4D8, "SceneShopTop::init::$_3 target(const type_info&)", SceneShopTop_lambdas::init_3::target);
RW_FUNCTION(0x68E4F8, "SceneShopTop::init::$_3 target_type()", SceneShopTop_lambdas::init_3::targetType);
RW_FUNCTION(0x68E50C, "SceneShopTop::init::$_2 ~__func() (deleting)", SceneShopTop_lambdas::init_2::deleteThis);
RW_FUNCTION(0x68E514, "SceneShopTop::init::$_2 __clone()", SceneShopTop_lambdas::init_2::clone);
RW_FUNCTION(0x68E540, "SceneShopTop::init::$_2 __clone(__base*)", SceneShopTop_lambdas::init_2::cloneInto);
RW_FUNCTION(0x68E560, "SceneShopTop::init::$_2 destroy()", SceneShopTop_lambdas::init_2::destroy);
RW_FUNCTION(0x68E562, "SceneShopTop::init::$_2 destroy_deallocate()", SceneShopTop_lambdas::init_2::deleteThis);
RW_FUNCTION(0x68E574, "SceneShopTop::init::$_2 target(const type_info&)", SceneShopTop_lambdas::init_2::target);
RW_FUNCTION(0x68E594, "SceneShopTop::init::$_2 target_type()", SceneShopTop_lambdas::init_2::targetType);
RW_FUNCTION(0x68E5A8, "SceneShopTop::init::$_1 ~__func() (deleting)", SceneShopTop_lambdas::init_1::deleteThis);
RW_FUNCTION(0x68E5B0, "SceneShopTop::init::$_1 __clone()", SceneShopTop_lambdas::init_1::clone);
RW_FUNCTION(0x68E5DC, "SceneShopTop::init::$_1 __clone(__base*)", SceneShopTop_lambdas::init_1::cloneInto);
RW_FUNCTION(0x68E5FC, "SceneShopTop::init::$_1 destroy()", SceneShopTop_lambdas::init_1::destroy);
RW_FUNCTION(0x68E5FE, "SceneShopTop::init::$_1 destroy_deallocate()", SceneShopTop_lambdas::init_1::deleteThis);
RW_FUNCTION(0x68E620, "SceneShopTop::init::$_1 target(const type_info&)", SceneShopTop_lambdas::init_1::target);
RW_FUNCTION(0x68E640, "SceneShopTop::init::$_1 target_type()", SceneShopTop_lambdas::init_1::targetType);
RW_FUNCTION(0x68E654, "SceneShopTop::callShopAPI::$_0 ~__func()", SceneShopTop_lambdas::callShopAPI_0::destructor);
RW_FUNCTION(0x68E690, "SceneShopTop::callShopAPI::$_0 ~__func() (deleting)", SceneShopTop_lambdas::callShopAPI_0::destructorDelete);
RW_FUNCTION(0x68E6D0, "SceneShopTop::callShopAPI::$_0 __clone()", SceneShopTop_lambdas::callShopAPI_0::clone);
RW_FUNCTION(0x68E720, "SceneShopTop::callShopAPI::$_0 __clone(__base*)", SceneShopTop_lambdas::callShopAPI_0::cloneInto);
RW_FUNCTION(0x68E754, "SceneShopTop::callShopAPI::$_0 destroy()", SceneShopTop_lambdas::callShopAPI_0::destroy);
RW_FUNCTION(0x68E774, "SceneShopTop::callShopAPI::$_0 destroy_deallocate()", SceneShopTop_lambdas::callShopAPI_0::destroyDeallocate);
RW_FUNCTION(0x68E7B8, "SceneShopTop::callShopAPI::$_0 target(const type_info&)", SceneShopTop_lambdas::callShopAPI_0::target);
RW_FUNCTION(0x68E7D8, "SceneShopTop::callShopAPI::$_0 target_type()", SceneShopTop_lambdas::callShopAPI_0::targetType);
