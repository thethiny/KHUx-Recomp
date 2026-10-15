// MarkFader: the std::function slots of its lambdas, generated from the binary's __func vtables
// (each thunk's shape was checked by emulating the original instructions). The lambda bodies
// (operator()) are written by hand.
#include "../scene_main/FunctionThunks.hpp"

namespace MarkFader_w31_thunks {

// MarkFader::fadeIn::$_2 (captures a std::function at +8, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using fadeIn_2 = thunk::LambdaFn<0xF5D8A0, 32, 0xF91CCC, 0xE4C870, 8, 8, 0x4683CD>;
// MarkFader::fadeIn::$_1
using fadeIn_1 = thunk::Lambda<0xF5D8CC, 8, 0xF91CE0, 0xE4C910, 4, thunk::cap(4, 4)>;
// MarkFader::init::$_0
using init_0 = thunk::Lambda<0xF5D8F8, 8, 0xF91CF4, 0xE4C9F0, 4, thunk::cap(4, 4)>;

}  // namespace MarkFader_w31_thunks

RW_FUNCTION(0x5DA000, "MarkFader::fadeIn::$_2 ~__func() (deleting)", MarkFader_w31_thunks::fadeIn_2::destructorDelete);
RW_FUNCTION(0x5DA040, "MarkFader::fadeIn::$_2 __clone()", MarkFader_w31_thunks::fadeIn_2::clone);
RW_FUNCTION(0x5DA090, "MarkFader::fadeIn::$_2 __clone(__base*)", MarkFader_w31_thunks::fadeIn_2::cloneInto);
RW_FUNCTION(0x5DA0C4, "MarkFader::fadeIn::$_2 destroy()", MarkFader_w31_thunks::fadeIn_2::destroy);
RW_FUNCTION(0x5DA0E4, "MarkFader::fadeIn::$_2 destroy_deallocate()", MarkFader_w31_thunks::fadeIn_2::destroyDeallocate);
RW_FUNCTION(0x5DA140, "MarkFader::fadeIn::$_2 target(const type_info&)", MarkFader_w31_thunks::fadeIn_2::target);
RW_FUNCTION(0x5DA160, "MarkFader::fadeIn::$_2 target_type()", MarkFader_w31_thunks::fadeIn_2::targetType);
RW_FUNCTION(0x5DA174, "MarkFader::fadeIn::$_1 ~__func() (deleting)", MarkFader_w31_thunks::fadeIn_1::deleteThis);
RW_FUNCTION(0x5DA17C, "MarkFader::fadeIn::$_1 __clone()", MarkFader_w31_thunks::fadeIn_1::clone);
RW_FUNCTION(0x5DA1A8, "MarkFader::fadeIn::$_1 __clone(__base*)", MarkFader_w31_thunks::fadeIn_1::cloneInto);
RW_FUNCTION(0x5DA1C8, "MarkFader::fadeIn::$_1 destroy()", MarkFader_w31_thunks::fadeIn_1::destroy);
RW_FUNCTION(0x5DA1CA, "MarkFader::fadeIn::$_1 destroy_deallocate()", MarkFader_w31_thunks::fadeIn_1::deleteThis);
RW_FUNCTION(0x5DA208, "MarkFader::fadeIn::$_1 target(const type_info&)", MarkFader_w31_thunks::fadeIn_1::target);
RW_FUNCTION(0x5DA228, "MarkFader::fadeIn::$_1 target_type()", MarkFader_w31_thunks::fadeIn_1::targetType);
RW_FUNCTION(0x5DA23C, "MarkFader::init::$_0 ~__func() (deleting)", MarkFader_w31_thunks::init_0::deleteThis);
RW_FUNCTION(0x5DA244, "MarkFader::init::$_0 __clone()", MarkFader_w31_thunks::init_0::clone);
RW_FUNCTION(0x5DA270, "MarkFader::init::$_0 __clone(__base*)", MarkFader_w31_thunks::init_0::cloneInto);
RW_FUNCTION(0x5DA290, "MarkFader::init::$_0 destroy()", MarkFader_w31_thunks::init_0::destroy);
RW_FUNCTION(0x5DA292, "MarkFader::init::$_0 destroy_deallocate()", MarkFader_w31_thunks::init_0::deleteThis);
RW_FUNCTION(0x5DA2A0, "MarkFader::init::$_0 target(const type_info&)", MarkFader_w31_thunks::init_0::target);
RW_FUNCTION(0x5DA2C0, "MarkFader::init::$_0 target_type()", MarkFader_w31_thunks::init_0::targetType);
