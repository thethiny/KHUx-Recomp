// BGFacebook: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "../scene_main/FunctionThunks.hpp"

namespace BGFacebook_g5thunks {

// BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0
using hideConnectingShadow_0 = thunk::Lambda<0xF55868, 8, 0xF7FB5C, 0xE1E950, 4, thunk::cap(4, 4)>;

}  // namespace BGFacebook_g5thunks

RW_FUNCTION(0x4E633A, "BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0 ~__func() (deleting)", BGFacebook_g5thunks::hideConnectingShadow_0::deleteThis);
RW_FUNCTION(0x4E6344, "BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0 __clone()", BGFacebook_g5thunks::hideConnectingShadow_0::clone);
RW_FUNCTION(0x4E6370, "BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0 __clone(__base*)", BGFacebook_g5thunks::hideConnectingShadow_0::cloneInto);
RW_FUNCTION(0x4E6390, "BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0 destroy()", BGFacebook_g5thunks::hideConnectingShadow_0::destroy);
RW_FUNCTION(0x4E6392, "BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0 destroy_deallocate()", BGFacebook_g5thunks::hideConnectingShadow_0::deleteThis);
RW_FUNCTION(0x4E63B4, "BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0 target(const type_info&)", BGFacebook_g5thunks::hideConnectingShadow_0::target);
RW_FUNCTION(0x4E63D4, "BGFacebook::FacebookManagerImpl::hideConnectingShadow::$_0 target_type()", BGFacebook_g5thunks::hideConnectingShadow_0::targetType);
