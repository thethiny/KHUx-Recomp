// SceneDownload: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneDownload_lambdas {

// SceneDownload::finalize::$_3
using finalize_3 = thunk::Lambda<0xF671AC, 8, 0xF9F828, 0xE7CD10, 4>;
// SceneDownload::init::$_2
using init_2 = thunk::Lambda<0xF6725C, 8, 0xF9F878, 0xE7CFC0, 4, thunk::cap(4, 4)>;
// SceneDownload::init::$_1
using init_1 = thunk::Lambda<0xF67288, 8, 0xF9F88C, 0xE7D040, 4, thunk::cap(4, 4)>;
// SceneDownload::init::$_0
using init_0 = thunk::Lambda<0xF672B4, 8, 0xF9F8A0, 0xE7D0C0, 4>;

}  // namespace SceneDownload_lambdas

RW_FUNCTION(0x6C8768, "SceneDownload::finalize::$_3 ~__func() (deleting)", SceneDownload_lambdas::finalize_3::deleteThis);
RW_FUNCTION(0x6C8770, "SceneDownload::finalize::$_3 __clone()", SceneDownload_lambdas::finalize_3::clone);
RW_FUNCTION(0x6C8794, "SceneDownload::finalize::$_3 __clone(__base*)", SceneDownload_lambdas::finalize_3::cloneInto);
RW_FUNCTION(0x6C87B0, "SceneDownload::finalize::$_3 destroy()", SceneDownload_lambdas::finalize_3::destroy);
RW_FUNCTION(0x6C87B2, "SceneDownload::finalize::$_3 destroy_deallocate()", SceneDownload_lambdas::finalize_3::deleteThis);
RW_FUNCTION(0x6C8858, "SceneDownload::finalize::$_3 target(const type_info&)", SceneDownload_lambdas::finalize_3::target);
RW_FUNCTION(0x6C8878, "SceneDownload::finalize::$_3 target_type()", SceneDownload_lambdas::finalize_3::targetType);
RW_FUNCTION(0x6C8BE0, "SceneDownload::init::$_2 ~__func() (deleting)", SceneDownload_lambdas::init_2::deleteThis);
RW_FUNCTION(0x6C8BE8, "SceneDownload::init::$_2 __clone()", SceneDownload_lambdas::init_2::clone);
RW_FUNCTION(0x6C8C14, "SceneDownload::init::$_2 __clone(__base*)", SceneDownload_lambdas::init_2::cloneInto);
RW_FUNCTION(0x6C8C34, "SceneDownload::init::$_2 destroy()", SceneDownload_lambdas::init_2::destroy);
RW_FUNCTION(0x6C8C36, "SceneDownload::init::$_2 destroy_deallocate()", SceneDownload_lambdas::init_2::deleteThis);
RW_FUNCTION(0x6C8C48, "SceneDownload::init::$_2 target(const type_info&)", SceneDownload_lambdas::init_2::target);
RW_FUNCTION(0x6C8C68, "SceneDownload::init::$_2 target_type()", SceneDownload_lambdas::init_2::targetType);
RW_FUNCTION(0x6C8C7C, "SceneDownload::init::$_1 ~__func() (deleting)", SceneDownload_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6C8C84, "SceneDownload::init::$_1 __clone()", SceneDownload_lambdas::init_1::clone);
RW_FUNCTION(0x6C8CB0, "SceneDownload::init::$_1 __clone(__base*)", SceneDownload_lambdas::init_1::cloneInto);
RW_FUNCTION(0x6C8CD0, "SceneDownload::init::$_1 destroy()", SceneDownload_lambdas::init_1::destroy);
RW_FUNCTION(0x6C8CD2, "SceneDownload::init::$_1 destroy_deallocate()", SceneDownload_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6C8CE4, "SceneDownload::init::$_1 target(const type_info&)", SceneDownload_lambdas::init_1::target);
RW_FUNCTION(0x6C8D04, "SceneDownload::init::$_1 target_type()", SceneDownload_lambdas::init_1::targetType);
RW_FUNCTION(0x6C8D18, "SceneDownload::init::$_0 ~__func() (deleting)", SceneDownload_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6C8D20, "SceneDownload::init::$_0 __clone()", SceneDownload_lambdas::init_0::clone);
RW_FUNCTION(0x6C8D44, "SceneDownload::init::$_0 __clone(__base*)", SceneDownload_lambdas::init_0::cloneInto);
RW_FUNCTION(0x6C8D60, "SceneDownload::init::$_0 destroy()", SceneDownload_lambdas::init_0::destroy);
RW_FUNCTION(0x6C8D62, "SceneDownload::init::$_0 destroy_deallocate()", SceneDownload_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6C8D6C, "SceneDownload::init::$_0 target(const type_info&)", SceneDownload_lambdas::init_0::target);
RW_FUNCTION(0x6C8D8C, "SceneDownload::init::$_0 target_type()", SceneDownload_lambdas::init_0::targetType);
