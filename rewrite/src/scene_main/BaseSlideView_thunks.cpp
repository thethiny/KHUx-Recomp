// BaseSlideView: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace BaseSlideView_lambdas {

// BaseSlideView::runRightSlide::$_5
using runRightSlide_5 = thunk::Lambda<0xF5704C, 8, 0xF85EE8, 0xE26EA0, 4, thunk::cap(4, 4)>;
// BaseSlideView::runLeftSlide::$_4
using runLeftSlide_4 = thunk::Lambda<0xF57078, 8, 0xF85EFC, 0xE26F30, 4, thunk::cap(4, 4)>;
// BaseSlideView::onCompleteAnimation::$_3
using onCompleteAnimation_3 = thunk::Lambda<0xF570A4, 8, 0xF85F10, 0xE26FD0, 4, thunk::cap(4, 4)>;
// BaseSlideView::init::$_2
using init_2 = thunk::Lambda<0xF570D0, 12, 0xF85F24, 0xE27070, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// BaseSlideView::init::$_6
using init_6 = thunk::Lambda<0xF570FC, 8, 0xF85F38, 0xE270F0, 4, thunk::cap(4, 4)>;
// BaseSlideView::init::$_1
using init_1 = thunk::Lambda<0xF57128, 8, 0xF85F4C, 0xE27180, 4, thunk::cap(4, 4)>;
// BaseSlideView::init::$_0
using init_0 = thunk::Lambda<0xF57154, 8, 0xF85F60, 0xE27210, 4, thunk::cap(4, 4)>;

}  // namespace BaseSlideView_lambdas

RW_FUNCTION(0x526614, "BaseSlideView::runRightSlide::$_5 ~__func() (deleting)", BaseSlideView_lambdas::runRightSlide_5::deleteThis);
RW_FUNCTION(0x52661C, "BaseSlideView::runRightSlide::$_5 __clone()", BaseSlideView_lambdas::runRightSlide_5::clone);
RW_FUNCTION(0x526648, "BaseSlideView::runRightSlide::$_5 __clone(__base*)", BaseSlideView_lambdas::runRightSlide_5::cloneInto);
RW_FUNCTION(0x526668, "BaseSlideView::runRightSlide::$_5 destroy()", BaseSlideView_lambdas::runRightSlide_5::destroy);
RW_FUNCTION(0x52666A, "BaseSlideView::runRightSlide::$_5 destroy_deallocate()", BaseSlideView_lambdas::runRightSlide_5::deleteThis);
RW_FUNCTION(0x526680, "BaseSlideView::runRightSlide::$_5 target(const type_info&)", BaseSlideView_lambdas::runRightSlide_5::target);
RW_FUNCTION(0x5266A0, "BaseSlideView::runRightSlide::$_5 target_type()", BaseSlideView_lambdas::runRightSlide_5::targetType);
RW_FUNCTION(0x5266B4, "BaseSlideView::runLeftSlide::$_4 ~__func() (deleting)", BaseSlideView_lambdas::runLeftSlide_4::deleteThis);
RW_FUNCTION(0x5266BC, "BaseSlideView::runLeftSlide::$_4 __clone()", BaseSlideView_lambdas::runLeftSlide_4::clone);
RW_FUNCTION(0x5266E8, "BaseSlideView::runLeftSlide::$_4 __clone(__base*)", BaseSlideView_lambdas::runLeftSlide_4::cloneInto);
RW_FUNCTION(0x526708, "BaseSlideView::runLeftSlide::$_4 destroy()", BaseSlideView_lambdas::runLeftSlide_4::destroy);
RW_FUNCTION(0x52670A, "BaseSlideView::runLeftSlide::$_4 destroy_deallocate()", BaseSlideView_lambdas::runLeftSlide_4::deleteThis);
RW_FUNCTION(0x526720, "BaseSlideView::runLeftSlide::$_4 target(const type_info&)", BaseSlideView_lambdas::runLeftSlide_4::target);
RW_FUNCTION(0x526740, "BaseSlideView::runLeftSlide::$_4 target_type()", BaseSlideView_lambdas::runLeftSlide_4::targetType);
RW_FUNCTION(0x526754, "BaseSlideView::onCompleteAnimation::$_3 ~__func() (deleting)", BaseSlideView_lambdas::onCompleteAnimation_3::deleteThis);
RW_FUNCTION(0x52675C, "BaseSlideView::onCompleteAnimation::$_3 __clone()", BaseSlideView_lambdas::onCompleteAnimation_3::clone);
RW_FUNCTION(0x526788, "BaseSlideView::onCompleteAnimation::$_3 __clone(__base*)", BaseSlideView_lambdas::onCompleteAnimation_3::cloneInto);
RW_FUNCTION(0x5267A8, "BaseSlideView::onCompleteAnimation::$_3 destroy()", BaseSlideView_lambdas::onCompleteAnimation_3::destroy);
RW_FUNCTION(0x5267AA, "BaseSlideView::onCompleteAnimation::$_3 destroy_deallocate()", BaseSlideView_lambdas::onCompleteAnimation_3::deleteThis);
RW_FUNCTION(0x5267E4, "BaseSlideView::onCompleteAnimation::$_3 target(const type_info&)", BaseSlideView_lambdas::onCompleteAnimation_3::target);
RW_FUNCTION(0x526804, "BaseSlideView::onCompleteAnimation::$_3 target_type()", BaseSlideView_lambdas::onCompleteAnimation_3::targetType);
RW_FUNCTION(0x526954, "BaseSlideView::init::$_2 ~__func() (deleting)", BaseSlideView_lambdas::init_2::deleteThis);
RW_FUNCTION(0x52695C, "BaseSlideView::init::$_2 __clone()", BaseSlideView_lambdas::init_2::clone);
RW_FUNCTION(0x52698C, "BaseSlideView::init::$_2 __clone(__base*)", BaseSlideView_lambdas::init_2::cloneInto);
RW_FUNCTION(0x5269B0, "BaseSlideView::init::$_2 destroy()", BaseSlideView_lambdas::init_2::destroy);
RW_FUNCTION(0x5269B2, "BaseSlideView::init::$_2 destroy_deallocate()", BaseSlideView_lambdas::init_2::deleteThis);
RW_FUNCTION(0x526C74, "BaseSlideView::init::$_2 target(const type_info&)", BaseSlideView_lambdas::init_2::target);
RW_FUNCTION(0x526C94, "BaseSlideView::init::$_2 target_type()", BaseSlideView_lambdas::init_2::targetType);
RW_FUNCTION(0x526CA8, "BaseSlideView::init::$_6 ~__func() (deleting)", BaseSlideView_lambdas::init_6::deleteThis);
RW_FUNCTION(0x526CB0, "BaseSlideView::init::$_6 __clone()", BaseSlideView_lambdas::init_6::clone);
RW_FUNCTION(0x526CDC, "BaseSlideView::init::$_6 __clone(__base*)", BaseSlideView_lambdas::init_6::cloneInto);
RW_FUNCTION(0x526CFC, "BaseSlideView::init::$_6 destroy()", BaseSlideView_lambdas::init_6::destroy);
RW_FUNCTION(0x526CFE, "BaseSlideView::init::$_6 destroy_deallocate()", BaseSlideView_lambdas::init_6::deleteThis);
RW_FUNCTION(0x526D2C, "BaseSlideView::init::$_6 target(const type_info&)", BaseSlideView_lambdas::init_6::target);
RW_FUNCTION(0x526D4C, "BaseSlideView::init::$_6 target_type()", BaseSlideView_lambdas::init_6::targetType);
RW_FUNCTION(0x526D60, "BaseSlideView::init::$_1 ~__func() (deleting)", BaseSlideView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x526D68, "BaseSlideView::init::$_1 __clone()", BaseSlideView_lambdas::init_1::clone);
RW_FUNCTION(0x526D94, "BaseSlideView::init::$_1 __clone(__base*)", BaseSlideView_lambdas::init_1::cloneInto);
RW_FUNCTION(0x526DB4, "BaseSlideView::init::$_1 destroy()", BaseSlideView_lambdas::init_1::destroy);
RW_FUNCTION(0x526DB6, "BaseSlideView::init::$_1 destroy_deallocate()", BaseSlideView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x526F34, "BaseSlideView::init::$_1 target(const type_info&)", BaseSlideView_lambdas::init_1::target);
RW_FUNCTION(0x526F54, "BaseSlideView::init::$_1 target_type()", BaseSlideView_lambdas::init_1::targetType);
RW_FUNCTION(0x526F68, "BaseSlideView::init::$_0 ~__func() (deleting)", BaseSlideView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x526F70, "BaseSlideView::init::$_0 __clone()", BaseSlideView_lambdas::init_0::clone);
RW_FUNCTION(0x526F9C, "BaseSlideView::init::$_0 __clone(__base*)", BaseSlideView_lambdas::init_0::cloneInto);
RW_FUNCTION(0x526FBC, "BaseSlideView::init::$_0 destroy()", BaseSlideView_lambdas::init_0::destroy);
RW_FUNCTION(0x526FBE, "BaseSlideView::init::$_0 destroy_deallocate()", BaseSlideView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x5270FC, "BaseSlideView::init::$_0 target(const type_info&)", BaseSlideView_lambdas::init_0::target);
RW_FUNCTION(0x52711C, "BaseSlideView::init::$_0 target_type()", BaseSlideView_lambdas::init_0::targetType);
