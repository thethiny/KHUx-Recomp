// MedalListScrollView: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "../scene_main/FunctionThunks.hpp"

namespace MedalListScrollView_g5thunks {

// MedalListScrollView::registItemTouchEvent::$_2
using registItemTouchEvent_2 = thunk::Lambda<0xF5B24C, 20, 0xF8CD74, 0xE40690, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;
// MedalListScrollView::registItemTouchEvent::$_3
using registItemTouchEvent_3 = thunk::Lambda<0xF5B278, 12, 0xF8CD88, 0xE40760, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// MedalListScrollView::registItemTouchEvent::$_1
using registItemTouchEvent_1 = thunk::Lambda<0xF5B2A4, 28, 0xF8CD9C, 0xE40850, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4)>;

}  // namespace MedalListScrollView_g5thunks

RW_FUNCTION(0x59B2CC, "MedalListScrollView::registItemTouchEvent::$_2 ~__func() (deleting)", MedalListScrollView_g5thunks::registItemTouchEvent_2::deleteThis);
RW_FUNCTION(0x59B2D4, "MedalListScrollView::registItemTouchEvent::$_2 __clone()", MedalListScrollView_g5thunks::registItemTouchEvent_2::clone);
RW_FUNCTION(0x59B308, "MedalListScrollView::registItemTouchEvent::$_2 __clone(__base*)", MedalListScrollView_g5thunks::registItemTouchEvent_2::cloneInto);
RW_FUNCTION(0x59B334, "MedalListScrollView::registItemTouchEvent::$_2 destroy()", MedalListScrollView_g5thunks::registItemTouchEvent_2::destroy);
RW_FUNCTION(0x59B336, "MedalListScrollView::registItemTouchEvent::$_2 destroy_deallocate()", MedalListScrollView_g5thunks::registItemTouchEvent_2::deleteThis);
RW_FUNCTION(0x59B34C, "MedalListScrollView::registItemTouchEvent::$_2 target(const type_info&)", MedalListScrollView_g5thunks::registItemTouchEvent_2::target);
RW_FUNCTION(0x59B36C, "MedalListScrollView::registItemTouchEvent::$_2 target_type()", MedalListScrollView_g5thunks::registItemTouchEvent_2::targetType);
RW_FUNCTION(0x59B56C, "MedalListScrollView::registItemTouchEvent::$_3 ~__func() (deleting)", MedalListScrollView_g5thunks::registItemTouchEvent_3::deleteThis);
RW_FUNCTION(0x59B574, "MedalListScrollView::registItemTouchEvent::$_3 __clone()", MedalListScrollView_g5thunks::registItemTouchEvent_3::clone);
RW_FUNCTION(0x59B5A4, "MedalListScrollView::registItemTouchEvent::$_3 __clone(__base*)", MedalListScrollView_g5thunks::registItemTouchEvent_3::cloneInto);
RW_FUNCTION(0x59B5C8, "MedalListScrollView::registItemTouchEvent::$_3 destroy()", MedalListScrollView_g5thunks::registItemTouchEvent_3::destroy);
RW_FUNCTION(0x59B5CA, "MedalListScrollView::registItemTouchEvent::$_3 destroy_deallocate()", MedalListScrollView_g5thunks::registItemTouchEvent_3::deleteThis);
RW_FUNCTION(0x59B5F0, "MedalListScrollView::registItemTouchEvent::$_3 target(const type_info&)", MedalListScrollView_g5thunks::registItemTouchEvent_3::target);
RW_FUNCTION(0x59B610, "MedalListScrollView::registItemTouchEvent::$_3 target_type()", MedalListScrollView_g5thunks::registItemTouchEvent_3::targetType);
RW_FUNCTION(0x59B624, "MedalListScrollView::registItemTouchEvent::$_1 ~__func() (deleting)", MedalListScrollView_g5thunks::registItemTouchEvent_1::deleteThis);
RW_FUNCTION(0x59B62C, "MedalListScrollView::registItemTouchEvent::$_1 __clone()", MedalListScrollView_g5thunks::registItemTouchEvent_1::clone);
RW_FUNCTION(0x59B664, "MedalListScrollView::registItemTouchEvent::$_1 __clone(__base*)", MedalListScrollView_g5thunks::registItemTouchEvent_1::cloneInto);
RW_FUNCTION(0x59B690, "MedalListScrollView::registItemTouchEvent::$_1 destroy()", MedalListScrollView_g5thunks::registItemTouchEvent_1::destroy);
RW_FUNCTION(0x59B692, "MedalListScrollView::registItemTouchEvent::$_1 destroy_deallocate()", MedalListScrollView_g5thunks::registItemTouchEvent_1::deleteThis);
