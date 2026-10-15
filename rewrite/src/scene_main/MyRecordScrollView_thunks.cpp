// MyRecordScrollView: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace MyRecordScrollView_lambdas {

// MyRecordScrollView::init::$_1
using init_1 = thunk::Lambda<0xF56940, 8, 0xF84ADC, 0xE24840, 4, thunk::cap(4, 4)>;
// MyRecordScrollView::init::$_0
using init_0 = thunk::Lambda<0xF5696C, 8, 0xF84AF0, 0xE248D0, 4, thunk::cap(4, 4)>;

}  // namespace MyRecordScrollView_lambdas

RW_FUNCTION(0x51B968, "MyRecordScrollView::init::$_1 ~__func() (deleting)", MyRecordScrollView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x51B970, "MyRecordScrollView::init::$_1 __clone()", MyRecordScrollView_lambdas::init_1::clone);
RW_FUNCTION(0x51B99C, "MyRecordScrollView::init::$_1 __clone(__base*)", MyRecordScrollView_lambdas::init_1::cloneInto);
RW_FUNCTION(0x51B9BC, "MyRecordScrollView::init::$_1 destroy()", MyRecordScrollView_lambdas::init_1::destroy);
RW_FUNCTION(0x51B9BE, "MyRecordScrollView::init::$_1 destroy_deallocate()", MyRecordScrollView_lambdas::init_1::deleteThis);
RW_FUNCTION(0x51B9D0, "MyRecordScrollView::init::$_1 target(const type_info&)", MyRecordScrollView_lambdas::init_1::target);
RW_FUNCTION(0x51B9F0, "MyRecordScrollView::init::$_1 target_type()", MyRecordScrollView_lambdas::init_1::targetType);
RW_FUNCTION(0x51BA04, "MyRecordScrollView::init::$_0 ~__func() (deleting)", MyRecordScrollView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x51BA0C, "MyRecordScrollView::init::$_0 __clone()", MyRecordScrollView_lambdas::init_0::clone);
RW_FUNCTION(0x51BA38, "MyRecordScrollView::init::$_0 __clone(__base*)", MyRecordScrollView_lambdas::init_0::cloneInto);
RW_FUNCTION(0x51BA58, "MyRecordScrollView::init::$_0 destroy()", MyRecordScrollView_lambdas::init_0::destroy);
RW_FUNCTION(0x51BA5A, "MyRecordScrollView::init::$_0 destroy_deallocate()", MyRecordScrollView_lambdas::init_0::deleteThis);
RW_FUNCTION(0x51BA6C, "MyRecordScrollView::init::$_0 target(const type_info&)", MyRecordScrollView_lambdas::init_0::target);
RW_FUNCTION(0x51BA8C, "MyRecordScrollView::init::$_0 target_type()", MyRecordScrollView_lambdas::init_0::targetType);
