// MaterialScrollView: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "../scene_main/FunctionThunks.hpp"

namespace MaterialScrollView_g5thunks {

// MaterialScrollView::registItemTouchEvent::$_2
using registItemTouchEvent_2 = thunk::Lambda<0xF55DBC, 20, 0xF8247C, 0xE20A70, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;
// MaterialScrollView::registItemTouchEvent::$_1
using registItemTouchEvent_1 = thunk::Lambda<0xF55DE8, 28, 0xF82490, 0xE20B60, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4)>;

}  // namespace MaterialScrollView_g5thunks

RW_FUNCTION(0x501DA8, "MaterialScrollView::registItemTouchEvent::$_2 ~__func() (deleting)", MaterialScrollView_g5thunks::registItemTouchEvent_2::deleteThis);
RW_FUNCTION(0x501DB0, "MaterialScrollView::registItemTouchEvent::$_2 __clone()", MaterialScrollView_g5thunks::registItemTouchEvent_2::clone);
RW_FUNCTION(0x501DE4, "MaterialScrollView::registItemTouchEvent::$_2 __clone(__base*)", MaterialScrollView_g5thunks::registItemTouchEvent_2::cloneInto);
RW_FUNCTION(0x501E10, "MaterialScrollView::registItemTouchEvent::$_2 destroy()", MaterialScrollView_g5thunks::registItemTouchEvent_2::destroy);
RW_FUNCTION(0x501E12, "MaterialScrollView::registItemTouchEvent::$_2 destroy_deallocate()", MaterialScrollView_g5thunks::registItemTouchEvent_2::deleteThis);
RW_FUNCTION(0x501E28, "MaterialScrollView::registItemTouchEvent::$_2 target(const type_info&)", MaterialScrollView_g5thunks::registItemTouchEvent_2::target);
RW_FUNCTION(0x501E48, "MaterialScrollView::registItemTouchEvent::$_2 target_type()", MaterialScrollView_g5thunks::registItemTouchEvent_2::targetType);
RW_FUNCTION(0x502018, "MaterialScrollView::registItemTouchEvent::$_1 ~__func() (deleting)", MaterialScrollView_g5thunks::registItemTouchEvent_1::deleteThis);
RW_FUNCTION(0x502020, "MaterialScrollView::registItemTouchEvent::$_1 __clone()", MaterialScrollView_g5thunks::registItemTouchEvent_1::clone);
RW_FUNCTION(0x502058, "MaterialScrollView::registItemTouchEvent::$_1 __clone(__base*)", MaterialScrollView_g5thunks::registItemTouchEvent_1::cloneInto);
RW_FUNCTION(0x502084, "MaterialScrollView::registItemTouchEvent::$_1 destroy()", MaterialScrollView_g5thunks::registItemTouchEvent_1::destroy);
RW_FUNCTION(0x502086, "MaterialScrollView::registItemTouchEvent::$_1 destroy_deallocate()", MaterialScrollView_g5thunks::registItemTouchEvent_1::deleteThis);
