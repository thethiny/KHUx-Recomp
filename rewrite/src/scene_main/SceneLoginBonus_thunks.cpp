// SceneLoginBonus: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneLoginBonus_lambdas {

// SceneLoginBonus::openMemoryBonus::$_11
using openMemoryBonus_11 = thunk::Lambda<0xF69EE0, 8, 0xFA2D1C, 0xE8A430, 4, thunk::cap(4, 4)>;
// SceneLoginBonus::openLoginBonus::$_9
using openLoginBonus_9 = thunk::Lambda<0xF69F0C, 16, 0xFA2D30, 0xE8A4F0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneLoginBonus::skipEvent::$_7
using skipEvent_7 = thunk::Lambda<0xF69F64, 8, 0xFA2D58, 0xE8A640, 4, thunk::cap(4, 4)>;
// SceneLoginBonus::runOutAnimationMain::$_6 (captures a std::function at +8, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using runOutAnimationMain_6 = thunk::LambdaFn<0xF69F90, 32, 0xFA2D6C, 0xE8A6F0, 8, 8, 0x4683CD>;
// SceneLoginBonus::runOutAnimationMain::$_5
using runOutAnimationMain_5 = thunk::Lambda<0xF69FBC, 8, 0xFA2D80, 0xE8A7C0, 4, thunk::cap(4, 4)>;
// SceneLoginBonus::runInAnimation::$_4
using runInAnimation_4 = thunk::Lambda<0xF69FE8, 8, 0xFA2D94, 0xE8A870, 4, thunk::cap(4, 4)>;
// SceneLoginBonus::runInAnimation::$_3
using runInAnimation_3 = thunk::Lambda<0xF6A014, 8, 0xFA2DA8, 0xE8A900, 4, thunk::cap(4, 4)>;
// SceneLoginBonus::init::$_2
using init_2 = thunk::Lambda<0xF6A040, 8, 0xFA2DBC, 0xE8A9C0, 4, thunk::cap(4, 4)>;
// SceneLoginBonus::init::$_1
using init_1 = thunk::Lambda<0xF6A06C, 8, 0xFA2DD0, 0xE8AA40, 4, thunk::cap(4, 4)>;
// SceneLoginBonus::init::$_0
using init_0 = thunk::Lambda<0xF6A098, 8, 0xFA2DE4, 0xE8AAC0, 4, thunk::cap(4, 4)>;

}  // namespace SceneLoginBonus_lambdas

RW_FUNCTION(0x6FA91C, "SceneLoginBonus::openMemoryBonus::$_11 ~__func() (deleting)", SceneLoginBonus_lambdas::openMemoryBonus_11::deleteThis);
RW_FUNCTION(0x6FA924, "SceneLoginBonus::openMemoryBonus::$_11 __clone()", SceneLoginBonus_lambdas::openMemoryBonus_11::clone);
RW_FUNCTION(0x6FA950, "SceneLoginBonus::openMemoryBonus::$_11 __clone(__base*)", SceneLoginBonus_lambdas::openMemoryBonus_11::cloneInto);
RW_FUNCTION(0x6FA970, "SceneLoginBonus::openMemoryBonus::$_11 destroy()", SceneLoginBonus_lambdas::openMemoryBonus_11::destroy);
RW_FUNCTION(0x6FA972, "SceneLoginBonus::openMemoryBonus::$_11 destroy_deallocate()", SceneLoginBonus_lambdas::openMemoryBonus_11::deleteThis);
RW_FUNCTION(0x6FAA5C, "SceneLoginBonus::openMemoryBonus::$_11 target(const type_info&)", SceneLoginBonus_lambdas::openMemoryBonus_11::target);
RW_FUNCTION(0x6FAA7C, "SceneLoginBonus::openMemoryBonus::$_11 target_type()", SceneLoginBonus_lambdas::openMemoryBonus_11::targetType);
RW_FUNCTION(0x6FAA90, "SceneLoginBonus::openLoginBonus::$_9 ~__func() (deleting)", SceneLoginBonus_lambdas::openLoginBonus_9::deleteThis);
RW_FUNCTION(0x6FAA98, "SceneLoginBonus::openLoginBonus::$_9 __clone()", SceneLoginBonus_lambdas::openLoginBonus_9::clone);
RW_FUNCTION(0x6FAAC8, "SceneLoginBonus::openLoginBonus::$_9 __clone(__base*)", SceneLoginBonus_lambdas::openLoginBonus_9::cloneInto);
RW_FUNCTION(0x6FAAF0, "SceneLoginBonus::openLoginBonus::$_9 destroy()", SceneLoginBonus_lambdas::openLoginBonus_9::destroy);
RW_FUNCTION(0x6FAAF2, "SceneLoginBonus::openLoginBonus::$_9 destroy_deallocate()", SceneLoginBonus_lambdas::openLoginBonus_9::deleteThis);
RW_FUNCTION(0x6FAB04, "SceneLoginBonus::openLoginBonus::$_9 target(const type_info&)", SceneLoginBonus_lambdas::openLoginBonus_9::target);
RW_FUNCTION(0x6FAB24, "SceneLoginBonus::openLoginBonus::$_9 target_type()", SceneLoginBonus_lambdas::openLoginBonus_9::targetType);
RW_FUNCTION(0x6FAC8C, "SceneLoginBonus::skipEvent::$_7 ~__func() (deleting)", SceneLoginBonus_lambdas::skipEvent_7::deleteThis);
RW_FUNCTION(0x6FAC94, "SceneLoginBonus::skipEvent::$_7 __clone()", SceneLoginBonus_lambdas::skipEvent_7::clone);
RW_FUNCTION(0x6FACC0, "SceneLoginBonus::skipEvent::$_7 __clone(__base*)", SceneLoginBonus_lambdas::skipEvent_7::cloneInto);
RW_FUNCTION(0x6FACE0, "SceneLoginBonus::skipEvent::$_7 destroy()", SceneLoginBonus_lambdas::skipEvent_7::destroy);
RW_FUNCTION(0x6FACE2, "SceneLoginBonus::skipEvent::$_7 destroy_deallocate()", SceneLoginBonus_lambdas::skipEvent_7::deleteThis);
RW_FUNCTION(0x6FADCC, "SceneLoginBonus::skipEvent::$_7 target(const type_info&)", SceneLoginBonus_lambdas::skipEvent_7::target);
RW_FUNCTION(0x6FADEC, "SceneLoginBonus::skipEvent::$_7 target_type()", SceneLoginBonus_lambdas::skipEvent_7::targetType);
RW_FUNCTION(0x6FAEB4, "SceneLoginBonus::runOutAnimationMain::$_6 ~__func()", SceneLoginBonus_lambdas::runOutAnimationMain_6::destructor);
RW_FUNCTION(0x6FAEF0, "SceneLoginBonus::runOutAnimationMain::$_6 ~__func() (deleting)", SceneLoginBonus_lambdas::runOutAnimationMain_6::destructorDelete);
RW_FUNCTION(0x6FAF30, "SceneLoginBonus::runOutAnimationMain::$_6 __clone()", SceneLoginBonus_lambdas::runOutAnimationMain_6::clone);
RW_FUNCTION(0x6FAF80, "SceneLoginBonus::runOutAnimationMain::$_6 __clone(__base*)", SceneLoginBonus_lambdas::runOutAnimationMain_6::cloneInto);
RW_FUNCTION(0x6FAFB4, "SceneLoginBonus::runOutAnimationMain::$_6 destroy()", SceneLoginBonus_lambdas::runOutAnimationMain_6::destroy);
RW_FUNCTION(0x6FAFD4, "SceneLoginBonus::runOutAnimationMain::$_6 destroy_deallocate()", SceneLoginBonus_lambdas::runOutAnimationMain_6::destroyDeallocate);
RW_FUNCTION(0x6FB00C, "SceneLoginBonus::runOutAnimationMain::$_6 target(const type_info&)", SceneLoginBonus_lambdas::runOutAnimationMain_6::target);
RW_FUNCTION(0x6FB02C, "SceneLoginBonus::runOutAnimationMain::$_6 target_type()", SceneLoginBonus_lambdas::runOutAnimationMain_6::targetType);
RW_FUNCTION(0x6FB040, "SceneLoginBonus::runOutAnimationMain::$_5 ~__func() (deleting)", SceneLoginBonus_lambdas::runOutAnimationMain_5::deleteThis);
RW_FUNCTION(0x6FB048, "SceneLoginBonus::runOutAnimationMain::$_5 __clone()", SceneLoginBonus_lambdas::runOutAnimationMain_5::clone);
RW_FUNCTION(0x6FB074, "SceneLoginBonus::runOutAnimationMain::$_5 __clone(__base*)", SceneLoginBonus_lambdas::runOutAnimationMain_5::cloneInto);
RW_FUNCTION(0x6FB094, "SceneLoginBonus::runOutAnimationMain::$_5 destroy()", SceneLoginBonus_lambdas::runOutAnimationMain_5::destroy);
RW_FUNCTION(0x6FB096, "SceneLoginBonus::runOutAnimationMain::$_5 destroy_deallocate()", SceneLoginBonus_lambdas::runOutAnimationMain_5::deleteThis);
RW_FUNCTION(0x6FB0B0, "SceneLoginBonus::runOutAnimationMain::$_5 target(const type_info&)", SceneLoginBonus_lambdas::runOutAnimationMain_5::target);
RW_FUNCTION(0x6FB0D0, "SceneLoginBonus::runOutAnimationMain::$_5 target_type()", SceneLoginBonus_lambdas::runOutAnimationMain_5::targetType);
RW_FUNCTION(0x6FB0E4, "SceneLoginBonus::runInAnimation::$_4 ~__func() (deleting)", SceneLoginBonus_lambdas::runInAnimation_4::deleteThis);
RW_FUNCTION(0x6FB0EC, "SceneLoginBonus::runInAnimation::$_4 __clone()", SceneLoginBonus_lambdas::runInAnimation_4::clone);
RW_FUNCTION(0x6FB118, "SceneLoginBonus::runInAnimation::$_4 __clone(__base*)", SceneLoginBonus_lambdas::runInAnimation_4::cloneInto);
RW_FUNCTION(0x6FB138, "SceneLoginBonus::runInAnimation::$_4 destroy()", SceneLoginBonus_lambdas::runInAnimation_4::destroy);
RW_FUNCTION(0x6FB13A, "SceneLoginBonus::runInAnimation::$_4 destroy_deallocate()", SceneLoginBonus_lambdas::runInAnimation_4::deleteThis);
RW_FUNCTION(0x6FB1C8, "SceneLoginBonus::runInAnimation::$_4 target(const type_info&)", SceneLoginBonus_lambdas::runInAnimation_4::target);
RW_FUNCTION(0x6FB1E8, "SceneLoginBonus::runInAnimation::$_4 target_type()", SceneLoginBonus_lambdas::runInAnimation_4::targetType);
RW_FUNCTION(0x6FB1FC, "SceneLoginBonus::runInAnimation::$_3 ~__func() (deleting)", SceneLoginBonus_lambdas::runInAnimation_3::deleteThis);
RW_FUNCTION(0x6FB204, "SceneLoginBonus::runInAnimation::$_3 __clone()", SceneLoginBonus_lambdas::runInAnimation_3::clone);
RW_FUNCTION(0x6FB230, "SceneLoginBonus::runInAnimation::$_3 __clone(__base*)", SceneLoginBonus_lambdas::runInAnimation_3::cloneInto);
RW_FUNCTION(0x6FB250, "SceneLoginBonus::runInAnimation::$_3 destroy()", SceneLoginBonus_lambdas::runInAnimation_3::destroy);
RW_FUNCTION(0x6FB252, "SceneLoginBonus::runInAnimation::$_3 destroy_deallocate()", SceneLoginBonus_lambdas::runInAnimation_3::deleteThis);
RW_FUNCTION(0x6FB2E0, "SceneLoginBonus::runInAnimation::$_3 target(const type_info&)", SceneLoginBonus_lambdas::runInAnimation_3::target);
RW_FUNCTION(0x6FB300, "SceneLoginBonus::runInAnimation::$_3 target_type()", SceneLoginBonus_lambdas::runInAnimation_3::targetType);
RW_FUNCTION(0x6FB58C, "SceneLoginBonus::init::$_2 ~__func() (deleting)", SceneLoginBonus_lambdas::init_2::deleteThis);
RW_FUNCTION(0x6FB594, "SceneLoginBonus::init::$_2 __clone()", SceneLoginBonus_lambdas::init_2::clone);
RW_FUNCTION(0x6FB5C0, "SceneLoginBonus::init::$_2 __clone(__base*)", SceneLoginBonus_lambdas::init_2::cloneInto);
RW_FUNCTION(0x6FB5E0, "SceneLoginBonus::init::$_2 destroy()", SceneLoginBonus_lambdas::init_2::destroy);
RW_FUNCTION(0x6FB5E2, "SceneLoginBonus::init::$_2 destroy_deallocate()", SceneLoginBonus_lambdas::init_2::deleteThis);
RW_FUNCTION(0x6FB608, "SceneLoginBonus::init::$_2 target(const type_info&)", SceneLoginBonus_lambdas::init_2::target);
RW_FUNCTION(0x6FB628, "SceneLoginBonus::init::$_2 target_type()", SceneLoginBonus_lambdas::init_2::targetType);
RW_FUNCTION(0x6FB63C, "SceneLoginBonus::init::$_1 ~__func() (deleting)", SceneLoginBonus_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6FB644, "SceneLoginBonus::init::$_1 __clone()", SceneLoginBonus_lambdas::init_1::clone);
RW_FUNCTION(0x6FB670, "SceneLoginBonus::init::$_1 __clone(__base*)", SceneLoginBonus_lambdas::init_1::cloneInto);
RW_FUNCTION(0x6FB690, "SceneLoginBonus::init::$_1 destroy()", SceneLoginBonus_lambdas::init_1::destroy);
RW_FUNCTION(0x6FB692, "SceneLoginBonus::init::$_1 destroy_deallocate()", SceneLoginBonus_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6FB6A4, "SceneLoginBonus::init::$_1 target(const type_info&)", SceneLoginBonus_lambdas::init_1::target);
RW_FUNCTION(0x6FB6C4, "SceneLoginBonus::init::$_1 target_type()", SceneLoginBonus_lambdas::init_1::targetType);
RW_FUNCTION(0x6FB6D8, "SceneLoginBonus::init::$_0 ~__func() (deleting)", SceneLoginBonus_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6FB6E0, "SceneLoginBonus::init::$_0 __clone()", SceneLoginBonus_lambdas::init_0::clone);
RW_FUNCTION(0x6FB70C, "SceneLoginBonus::init::$_0 __clone(__base*)", SceneLoginBonus_lambdas::init_0::cloneInto);
RW_FUNCTION(0x6FB72C, "SceneLoginBonus::init::$_0 destroy()", SceneLoginBonus_lambdas::init_0::destroy);
RW_FUNCTION(0x6FB72E, "SceneLoginBonus::init::$_0 destroy_deallocate()", SceneLoginBonus_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6FB740, "SceneLoginBonus::init::$_0 target(const type_info&)", SceneLoginBonus_lambdas::init_0::target);
RW_FUNCTION(0x6FB760, "SceneLoginBonus::init::$_0 target_type()", SceneLoginBonus_lambdas::init_0::targetType);
