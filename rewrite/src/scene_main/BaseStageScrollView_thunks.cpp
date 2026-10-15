// BaseStageScrollView: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace BaseStageScrollView_lambdas {

// BaseStageScrollView::init::$_2
using init_2 = thunk::Lambda<0xF5B2FC, 12, 0xF8D15C, 0xE40A50, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// BaseStageScrollView::init::$_1
using init_1 = thunk::Lambda<0xF5B328, 12, 0xF8D170, 0xE40B00, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// BaseStageScrollView::init::$_0
using init_0 = thunk::Lambda<0xF5B354, 8, 0xF8D184, 0xE40BB0, 4, thunk::cap(4, 4)>;

}  // namespace BaseStageScrollView_lambdas

RW_FUNCTION(0x59DD98, "BaseStageScrollView::init::$_2 ~__func() (deleting)", BaseStageScrollView_lambdas::init_2::deleteThis);
RW_FUNCTION(0x59DDA0, "BaseStageScrollView::init::$_2 __clone()", BaseStageScrollView_lambdas::init_2::clone);
RW_FUNCTION(0x59DDD0, "BaseStageScrollView::init::$_2 __clone(__base*)", BaseStageScrollView_lambdas::init_2::cloneInto);
RW_FUNCTION(0x59DDF4, "BaseStageScrollView::init::$_2 destroy()", BaseStageScrollView_lambdas::init_2::destroy);
RW_FUNCTION(0x59DDF6, "BaseStageScrollView::init::$_2 destroy_deallocate()", BaseStageScrollView_lambdas::init_2::deleteThis);
RW_FUNCTION(0x59E140, "BaseStageScrollView::init::$_2 target(const type_info&)", BaseStageScrollView_lambdas::init_2::target);
RW_FUNCTION(0x59E160, "BaseStageScrollView::init::$_2 target_type()", BaseStageScrollView_lambdas::init_2::targetType);
RW_FUNCTION(0x59E174, "BaseStageScrollView::init::$_1 ~__func() (deleting)", BaseStageScrollView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x59E17C, "BaseStageScrollView::init::$_1 __clone()", BaseStageScrollView_lambdas::init_1::clone);
RW_FUNCTION(0x59E1AC, "BaseStageScrollView::init::$_1 __clone(__base*)", BaseStageScrollView_lambdas::init_1::cloneInto);
RW_FUNCTION(0x59E1D0, "BaseStageScrollView::init::$_1 destroy()", BaseStageScrollView_lambdas::init_1::destroy);
RW_FUNCTION(0x59E1D2, "BaseStageScrollView::init::$_1 destroy_deallocate()", BaseStageScrollView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x59E5E4, "BaseStageScrollView::init::$_1 target(const type_info&)", BaseStageScrollView_lambdas::init_1::target);
RW_FUNCTION(0x59E604, "BaseStageScrollView::init::$_1 target_type()", BaseStageScrollView_lambdas::init_1::targetType);
RW_FUNCTION(0x59E618, "BaseStageScrollView::init::$_0 ~__func() (deleting)", BaseStageScrollView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x59E620, "BaseStageScrollView::init::$_0 __clone()", BaseStageScrollView_lambdas::init_0::clone);
RW_FUNCTION(0x59E64C, "BaseStageScrollView::init::$_0 __clone(__base*)", BaseStageScrollView_lambdas::init_0::cloneInto);
RW_FUNCTION(0x59E66C, "BaseStageScrollView::init::$_0 destroy()", BaseStageScrollView_lambdas::init_0::destroy);
RW_FUNCTION(0x59E66E, "BaseStageScrollView::init::$_0 destroy_deallocate()", BaseStageScrollView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x59E818, "BaseStageScrollView::init::$_0 target(const type_info&)", BaseStageScrollView_lambdas::init_0::target);
RW_FUNCTION(0x59E838, "BaseStageScrollView::init::$_0 target_type()", BaseStageScrollView_lambdas::init_0::targetType);
