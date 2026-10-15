// FlashMovieView: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace FlashMovieView_lambdas {

// FlashMovieView::init::$_2
using init_2 = thunk::Lambda<0xF59228, 8, 0xF8A728, 0xE335C0, 4, thunk::cap(4, 4)>;
// FlashMovieView::init::$_1
using init_1 = thunk::Lambda<0xF59254, 8, 0xF8A73C, 0xE33680, 4>;
// FlashMovieView::init::$_0
using init_0 = thunk::Lambda<0xF59280, 8, 0xF8A750, 0xE33740, 4, thunk::cap(4, 4)>;

}  // namespace FlashMovieView_lambdas

RW_FUNCTION(0x568928, "FlashMovieView::init::$_2 ~__func() (deleting)", FlashMovieView_lambdas::init_2::deleteThis);
RW_FUNCTION(0x568930, "FlashMovieView::init::$_2 __clone()", FlashMovieView_lambdas::init_2::clone);
RW_FUNCTION(0x56895C, "FlashMovieView::init::$_2 __clone(__base*)", FlashMovieView_lambdas::init_2::cloneInto);
RW_FUNCTION(0x56897C, "FlashMovieView::init::$_2 destroy()", FlashMovieView_lambdas::init_2::destroy);
RW_FUNCTION(0x56897E, "FlashMovieView::init::$_2 destroy_deallocate()", FlashMovieView_lambdas::init_2::deleteThis);
RW_FUNCTION(0x5689CC, "FlashMovieView::init::$_2 target(const type_info&)", FlashMovieView_lambdas::init_2::target);
RW_FUNCTION(0x5689EC, "FlashMovieView::init::$_2 target_type()", FlashMovieView_lambdas::init_2::targetType);
RW_FUNCTION(0x568A00, "FlashMovieView::init::$_1 ~__func() (deleting)", FlashMovieView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x568A08, "FlashMovieView::init::$_1 __clone()", FlashMovieView_lambdas::init_1::clone);
RW_FUNCTION(0x568A2C, "FlashMovieView::init::$_1 __clone(__base*)", FlashMovieView_lambdas::init_1::cloneInto);
RW_FUNCTION(0x568A48, "FlashMovieView::init::$_1 destroy()", FlashMovieView_lambdas::init_1::destroy);
RW_FUNCTION(0x568A4A, "FlashMovieView::init::$_1 destroy_deallocate()", FlashMovieView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x568A58, "FlashMovieView::init::$_1 target(const type_info&)", FlashMovieView_lambdas::init_1::target);
RW_FUNCTION(0x568A78, "FlashMovieView::init::$_1 target_type()", FlashMovieView_lambdas::init_1::targetType);
RW_FUNCTION(0x568A8C, "FlashMovieView::init::$_0 ~__func() (deleting)", FlashMovieView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x568A94, "FlashMovieView::init::$_0 __clone()", FlashMovieView_lambdas::init_0::clone);
RW_FUNCTION(0x568AC0, "FlashMovieView::init::$_0 __clone(__base*)", FlashMovieView_lambdas::init_0::cloneInto);
RW_FUNCTION(0x568AE0, "FlashMovieView::init::$_0 destroy()", FlashMovieView_lambdas::init_0::destroy);
RW_FUNCTION(0x568AE2, "FlashMovieView::init::$_0 destroy_deallocate()", FlashMovieView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x568B34, "FlashMovieView::init::$_0 target(const type_info&)", FlashMovieView_lambdas::init_0::target);
RW_FUNCTION(0x568B54, "FlashMovieView::init::$_0 target_type()", FlashMovieView_lambdas::init_0::targetType);
