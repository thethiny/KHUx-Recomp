// SceneMovie: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneMovie_lambdas {

// SceneMovie::runOutAnimation::$_4 (captures a std::function at +8, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using runOutAnimation_4 = thunk::LambdaFn<0xF60A28, 32, 0xF98EB4, 0xE5AB60, 8, 8, 0x4683CD>;
// SceneMovie::init::$_3
using init_3 = thunk::Lambda<0xF60A54, 12, 0xF98EC8, 0xE5AC30, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneMovie::init::$_2
using init_2 = thunk::Lambda<0xF60A80, 8, 0xF98EDC, 0xE5ACD0, 4, thunk::cap(4, 1)>;
// SceneMovie::init::$_1
using init_1 = thunk::Lambda<0xF60AAC, 8, 0xF98EF0, 0xE5AD40, 4, thunk::cap(4, 4)>;
// SceneMovie::init::$_0
using init_0 = thunk::Lambda<0xF60AD8, 8, 0xF98F04, 0xE5ADB0, 4, thunk::cap(4, 4)>;

}  // namespace SceneMovie_lambdas

RW_FUNCTION(0x639204, "SceneMovie::runOutAnimation::$_4 ~__func()", SceneMovie_lambdas::runOutAnimation_4::destructor);
RW_FUNCTION(0x639240, "SceneMovie::runOutAnimation::$_4 ~__func() (deleting)", SceneMovie_lambdas::runOutAnimation_4::destructorDelete);
RW_FUNCTION(0x639280, "SceneMovie::runOutAnimation::$_4 __clone()", SceneMovie_lambdas::runOutAnimation_4::clone);
RW_FUNCTION(0x6392D0, "SceneMovie::runOutAnimation::$_4 __clone(__base*)", SceneMovie_lambdas::runOutAnimation_4::cloneInto);
RW_FUNCTION(0x639304, "SceneMovie::runOutAnimation::$_4 destroy()", SceneMovie_lambdas::runOutAnimation_4::destroy);
RW_FUNCTION(0x639324, "SceneMovie::runOutAnimation::$_4 destroy_deallocate()", SceneMovie_lambdas::runOutAnimation_4::destroyDeallocate);
RW_FUNCTION(0x63935C, "SceneMovie::runOutAnimation::$_4 target(const type_info&)", SceneMovie_lambdas::runOutAnimation_4::target);
RW_FUNCTION(0x63937C, "SceneMovie::runOutAnimation::$_4 target_type()", SceneMovie_lambdas::runOutAnimation_4::targetType);
RW_FUNCTION(0x639390, "SceneMovie::init::$_3 ~__func() (deleting)", SceneMovie_lambdas::init_3::deleteThis);
RW_FUNCTION(0x639398, "SceneMovie::init::$_3 __clone()", SceneMovie_lambdas::init_3::clone);
RW_FUNCTION(0x6393C8, "SceneMovie::init::$_3 __clone(__base*)", SceneMovie_lambdas::init_3::cloneInto);
RW_FUNCTION(0x6393EC, "SceneMovie::init::$_3 destroy()", SceneMovie_lambdas::init_3::destroy);
RW_FUNCTION(0x6393EE, "SceneMovie::init::$_3 destroy_deallocate()", SceneMovie_lambdas::init_3::deleteThis);
RW_FUNCTION(0x639520, "SceneMovie::init::$_3 target(const type_info&)", SceneMovie_lambdas::init_3::target);
RW_FUNCTION(0x639540, "SceneMovie::init::$_3 target_type()", SceneMovie_lambdas::init_3::targetType);
RW_FUNCTION(0x639554, "SceneMovie::init::$_2 ~__func() (deleting)", SceneMovie_lambdas::init_2::deleteThis);
RW_FUNCTION(0x63955C, "SceneMovie::init::$_2 __clone()", SceneMovie_lambdas::init_2::clone);
RW_FUNCTION(0x639588, "SceneMovie::init::$_2 __clone(__base*)", SceneMovie_lambdas::init_2::cloneInto);
RW_FUNCTION(0x6395A8, "SceneMovie::init::$_2 destroy()", SceneMovie_lambdas::init_2::destroy);
RW_FUNCTION(0x6395AA, "SceneMovie::init::$_2 destroy_deallocate()", SceneMovie_lambdas::init_2::deleteThis);
RW_FUNCTION(0x63966C, "SceneMovie::init::$_2 target(const type_info&)", SceneMovie_lambdas::init_2::target);
RW_FUNCTION(0x63968C, "SceneMovie::init::$_2 target_type()", SceneMovie_lambdas::init_2::targetType);
RW_FUNCTION(0x6396A0, "SceneMovie::init::$_1 ~__func() (deleting)", SceneMovie_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6396A8, "SceneMovie::init::$_1 __clone()", SceneMovie_lambdas::init_1::clone);
RW_FUNCTION(0x6396D4, "SceneMovie::init::$_1 __clone(__base*)", SceneMovie_lambdas::init_1::cloneInto);
RW_FUNCTION(0x6396F4, "SceneMovie::init::$_1 destroy()", SceneMovie_lambdas::init_1::destroy);
RW_FUNCTION(0x6396F6, "SceneMovie::init::$_1 destroy_deallocate()", SceneMovie_lambdas::init_1::deleteThis);
RW_FUNCTION(0x639708, "SceneMovie::init::$_1 target(const type_info&)", SceneMovie_lambdas::init_1::target);
RW_FUNCTION(0x639728, "SceneMovie::init::$_1 target_type()", SceneMovie_lambdas::init_1::targetType);
RW_FUNCTION(0x63973C, "SceneMovie::init::$_0 ~__func() (deleting)", SceneMovie_lambdas::init_0::deleteThis);
RW_FUNCTION(0x639744, "SceneMovie::init::$_0 __clone()", SceneMovie_lambdas::init_0::clone);
RW_FUNCTION(0x639770, "SceneMovie::init::$_0 __clone(__base*)", SceneMovie_lambdas::init_0::cloneInto);
RW_FUNCTION(0x639790, "SceneMovie::init::$_0 destroy()", SceneMovie_lambdas::init_0::destroy);
RW_FUNCTION(0x639792, "SceneMovie::init::$_0 destroy_deallocate()", SceneMovie_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6397A4, "SceneMovie::init::$_0 target(const type_info&)", SceneMovie_lambdas::init_0::target);
RW_FUNCTION(0x6397C4, "SceneMovie::init::$_0 target_type()", SceneMovie_lambdas::init_0::targetType);
