// SceneTakeOver: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneTakeOver_lambdas {

// SceneTakeOver::openWebViewInfoPopup::$_9 (captures a std::function at +24, copied with _ZNSt3__18functionIFvPN7cocos2d4NodeEEEC2ERKS5_)
using openWebViewInfoPopup_9 = thunk::LambdaFn<0xF6B144, 56, 0xFA4594, 0xE8F4C0, 8, 24, 0x474D99, thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(48, 4)>;
// SceneTakeOver::openWebViewInfoPopup::$_8
using openWebViewInfoPopup_8 = thunk::Lambda<0xF6B19C, 8, 0xFA45BC, 0xE8F7D0, 4, thunk::cap(4, 4)>;
// SceneTakeOver::openWebViewInfoPopup::$_6
using openWebViewInfoPopup_6 = thunk::Lambda<0xF6B220, 8, 0xFA45F8, 0xE8FCC0, 4>;
// SceneTakeOver::init::$_5
using init_5 = thunk::Lambda<0xF6B24C, 8, 0xFA460C, 0xE8FDF0, 4>;
// SceneTakeOver::init::$_10
using init_10 = thunk::Lambda<0xF6B2D0, 8, 0xFA4648, 0xE900E0, 4>;
// SceneTakeOver::init::$_3
using init_3 = thunk::Lambda<0xF6B2FC, 8, 0xFA465C, 0xE90180, 4, thunk::cap(4, 1)>;
// SceneTakeOver::init::$_2
using init_2 = thunk::Lambda<0xF6B430, 8, 0xFA46E8, 0xE90DA0, 4, thunk::cap(4, 4)>;
// SceneTakeOver::init::$_1
using init_1 = thunk::Lambda<0xF6B45C, 8, 0xFA46FC, 0xE90E20, 4, thunk::cap(4, 4)>;
// SceneTakeOver::init::$_0
using init_0 = thunk::Lambda<0xF6B488, 8, 0xFA4710, 0xE90EA0, 4, thunk::cap(4, 4)>;

}  // namespace SceneTakeOver_lambdas

RW_FUNCTION(0x7133C8, "SceneTakeOver::openWebViewInfoPopup::$_9 ~__func()", SceneTakeOver_lambdas::openWebViewInfoPopup_9::destructor);
RW_FUNCTION(0x713404, "SceneTakeOver::openWebViewInfoPopup::$_9 ~__func() (deleting)", SceneTakeOver_lambdas::openWebViewInfoPopup_9::destructorDelete);
RW_FUNCTION(0x713444, "SceneTakeOver::openWebViewInfoPopup::$_9 __clone()", SceneTakeOver_lambdas::openWebViewInfoPopup_9::clone);
RW_FUNCTION(0x7134A4, "SceneTakeOver::openWebViewInfoPopup::$_9 __clone(__base*)", SceneTakeOver_lambdas::openWebViewInfoPopup_9::cloneInto);
RW_FUNCTION(0x7134EC, "SceneTakeOver::openWebViewInfoPopup::$_9 destroy()", SceneTakeOver_lambdas::openWebViewInfoPopup_9::destroy);
RW_FUNCTION(0x71350C, "SceneTakeOver::openWebViewInfoPopup::$_9 destroy_deallocate()", SceneTakeOver_lambdas::openWebViewInfoPopup_9::destroyDeallocate);
RW_FUNCTION(0x7136B8, "SceneTakeOver::openWebViewInfoPopup::$_9 target(const type_info&)", SceneTakeOver_lambdas::openWebViewInfoPopup_9::target);
RW_FUNCTION(0x7136DC, "SceneTakeOver::openWebViewInfoPopup::$_9 target_type()", SceneTakeOver_lambdas::openWebViewInfoPopup_9::targetType);
RW_FUNCTION(0x7138D4, "SceneTakeOver::openWebViewInfoPopup::$_8 ~__func() (deleting)", SceneTakeOver_lambdas::openWebViewInfoPopup_8::deleteThis);
RW_FUNCTION(0x7138DC, "SceneTakeOver::openWebViewInfoPopup::$_8 __clone()", SceneTakeOver_lambdas::openWebViewInfoPopup_8::clone);
RW_FUNCTION(0x713908, "SceneTakeOver::openWebViewInfoPopup::$_8 __clone(__base*)", SceneTakeOver_lambdas::openWebViewInfoPopup_8::cloneInto);
RW_FUNCTION(0x713928, "SceneTakeOver::openWebViewInfoPopup::$_8 destroy()", SceneTakeOver_lambdas::openWebViewInfoPopup_8::destroy);
RW_FUNCTION(0x71392A, "SceneTakeOver::openWebViewInfoPopup::$_8 destroy_deallocate()", SceneTakeOver_lambdas::openWebViewInfoPopup_8::deleteThis);
RW_FUNCTION(0x71393C, "SceneTakeOver::openWebViewInfoPopup::$_8 target(const type_info&)", SceneTakeOver_lambdas::openWebViewInfoPopup_8::target);
RW_FUNCTION(0x71395C, "SceneTakeOver::openWebViewInfoPopup::$_8 target_type()", SceneTakeOver_lambdas::openWebViewInfoPopup_8::targetType);
RW_FUNCTION(0x713C50, "SceneTakeOver::openWebViewInfoPopup::$_6 ~__func() (deleting)", SceneTakeOver_lambdas::openWebViewInfoPopup_6::deleteThis);
RW_FUNCTION(0x713C58, "SceneTakeOver::openWebViewInfoPopup::$_6 __clone()", SceneTakeOver_lambdas::openWebViewInfoPopup_6::clone);
RW_FUNCTION(0x713C7C, "SceneTakeOver::openWebViewInfoPopup::$_6 __clone(__base*)", SceneTakeOver_lambdas::openWebViewInfoPopup_6::cloneInto);
RW_FUNCTION(0x713C98, "SceneTakeOver::openWebViewInfoPopup::$_6 destroy()", SceneTakeOver_lambdas::openWebViewInfoPopup_6::destroy);
RW_FUNCTION(0x713C9A, "SceneTakeOver::openWebViewInfoPopup::$_6 destroy_deallocate()", SceneTakeOver_lambdas::openWebViewInfoPopup_6::deleteThis);
RW_FUNCTION(0x713CEC, "SceneTakeOver::openWebViewInfoPopup::$_6 target(const type_info&)", SceneTakeOver_lambdas::openWebViewInfoPopup_6::target);
RW_FUNCTION(0x713D0C, "SceneTakeOver::openWebViewInfoPopup::$_6 target_type()", SceneTakeOver_lambdas::openWebViewInfoPopup_6::targetType);
RW_FUNCTION(0x713D20, "SceneTakeOver::init::$_5 ~__func() (deleting)", SceneTakeOver_lambdas::init_5::deleteThis);
RW_FUNCTION(0x713D28, "SceneTakeOver::init::$_5 __clone()", SceneTakeOver_lambdas::init_5::clone);
RW_FUNCTION(0x713D4C, "SceneTakeOver::init::$_5 __clone(__base*)", SceneTakeOver_lambdas::init_5::cloneInto);
RW_FUNCTION(0x713D68, "SceneTakeOver::init::$_5 destroy()", SceneTakeOver_lambdas::init_5::destroy);
RW_FUNCTION(0x713D6A, "SceneTakeOver::init::$_5 destroy_deallocate()", SceneTakeOver_lambdas::init_5::deleteThis);
RW_FUNCTION(0x713E28, "SceneTakeOver::init::$_5 target(const type_info&)", SceneTakeOver_lambdas::init_5::target);
RW_FUNCTION(0x713E48, "SceneTakeOver::init::$_5 target_type()", SceneTakeOver_lambdas::init_5::targetType);
RW_FUNCTION(0x71413A, "SceneTakeOver::init::$_10 ~__func() (deleting)", SceneTakeOver_lambdas::init_10::deleteThis);
RW_FUNCTION(0x714144, "SceneTakeOver::init::$_10 __clone()", SceneTakeOver_lambdas::init_10::clone);
RW_FUNCTION(0x714168, "SceneTakeOver::init::$_10 __clone(__base*)", SceneTakeOver_lambdas::init_10::cloneInto);
RW_FUNCTION(0x714184, "SceneTakeOver::init::$_10 destroy()", SceneTakeOver_lambdas::init_10::destroy);
RW_FUNCTION(0x714186, "SceneTakeOver::init::$_10 destroy_deallocate()", SceneTakeOver_lambdas::init_10::deleteThis);
RW_FUNCTION(0x714244, "SceneTakeOver::init::$_10 target(const type_info&)", SceneTakeOver_lambdas::init_10::target);
RW_FUNCTION(0x714264, "SceneTakeOver::init::$_10 target_type()", SceneTakeOver_lambdas::init_10::targetType);
RW_FUNCTION(0x714278, "SceneTakeOver::init::$_3 ~__func() (deleting)", SceneTakeOver_lambdas::init_3::deleteThis);
RW_FUNCTION(0x714280, "SceneTakeOver::init::$_3 __clone()", SceneTakeOver_lambdas::init_3::clone);
RW_FUNCTION(0x7142AC, "SceneTakeOver::init::$_3 __clone(__base*)", SceneTakeOver_lambdas::init_3::cloneInto);
RW_FUNCTION(0x7142CC, "SceneTakeOver::init::$_3 destroy()", SceneTakeOver_lambdas::init_3::destroy);
RW_FUNCTION(0x7142CE, "SceneTakeOver::init::$_3 destroy_deallocate()", SceneTakeOver_lambdas::init_3::deleteThis);
RW_FUNCTION(0x714380, "SceneTakeOver::init::$_3 target(const type_info&)", SceneTakeOver_lambdas::init_3::target);
RW_FUNCTION(0x7143A0, "SceneTakeOver::init::$_3 target_type()", SceneTakeOver_lambdas::init_3::targetType);
RW_FUNCTION(0x714CB8, "SceneTakeOver::init::$_2 ~__func() (deleting)", SceneTakeOver_lambdas::init_2::deleteThis);
RW_FUNCTION(0x714CC0, "SceneTakeOver::init::$_2 __clone()", SceneTakeOver_lambdas::init_2::clone);
RW_FUNCTION(0x714CEC, "SceneTakeOver::init::$_2 __clone(__base*)", SceneTakeOver_lambdas::init_2::cloneInto);
RW_FUNCTION(0x714D0C, "SceneTakeOver::init::$_2 destroy()", SceneTakeOver_lambdas::init_2::destroy);
RW_FUNCTION(0x714D0E, "SceneTakeOver::init::$_2 destroy_deallocate()", SceneTakeOver_lambdas::init_2::deleteThis);
RW_FUNCTION(0x714D18, "SceneTakeOver::init::$_2 target(const type_info&)", SceneTakeOver_lambdas::init_2::target);
RW_FUNCTION(0x714D38, "SceneTakeOver::init::$_2 target_type()", SceneTakeOver_lambdas::init_2::targetType);
RW_FUNCTION(0x714D4C, "SceneTakeOver::init::$_1 ~__func() (deleting)", SceneTakeOver_lambdas::init_1::deleteThis);
RW_FUNCTION(0x714D54, "SceneTakeOver::init::$_1 __clone()", SceneTakeOver_lambdas::init_1::clone);
RW_FUNCTION(0x714D80, "SceneTakeOver::init::$_1 __clone(__base*)", SceneTakeOver_lambdas::init_1::cloneInto);
RW_FUNCTION(0x714DA0, "SceneTakeOver::init::$_1 destroy()", SceneTakeOver_lambdas::init_1::destroy);
RW_FUNCTION(0x714DA2, "SceneTakeOver::init::$_1 destroy_deallocate()", SceneTakeOver_lambdas::init_1::deleteThis);
RW_FUNCTION(0x714DB4, "SceneTakeOver::init::$_1 target(const type_info&)", SceneTakeOver_lambdas::init_1::target);
RW_FUNCTION(0x714DD4, "SceneTakeOver::init::$_1 target_type()", SceneTakeOver_lambdas::init_1::targetType);
RW_FUNCTION(0x714DE8, "SceneTakeOver::init::$_0 ~__func() (deleting)", SceneTakeOver_lambdas::init_0::deleteThis);
RW_FUNCTION(0x714DF0, "SceneTakeOver::init::$_0 __clone()", SceneTakeOver_lambdas::init_0::clone);
RW_FUNCTION(0x714E1C, "SceneTakeOver::init::$_0 __clone(__base*)", SceneTakeOver_lambdas::init_0::cloneInto);
RW_FUNCTION(0x714E3C, "SceneTakeOver::init::$_0 destroy()", SceneTakeOver_lambdas::init_0::destroy);
RW_FUNCTION(0x714E3E, "SceneTakeOver::init::$_0 destroy_deallocate()", SceneTakeOver_lambdas::init_0::deleteThis);
RW_FUNCTION(0x714E50, "SceneTakeOver::init::$_0 target(const type_info&)", SceneTakeOver_lambdas::init_0::target);
RW_FUNCTION(0x714E70, "SceneTakeOver::init::$_0 target_type()", SceneTakeOver_lambdas::init_0::targetType);
