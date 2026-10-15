// LWFNodeGroup: the std::function plumbing of its lambdas (libc++ __func slots: deleting destructor, clone, clone into a
// buffer, destroy, destroy_deallocate, target, target_type), generated from the binary's __func vtables (size,
// captured bytes, type_info, type name and target offset read from each slot's instructions), plus the lambda bodies.
#include "../scene_main/FunctionThunks.hpp"

namespace LWFNodeGroup_w3 {

// LWFNodeGroup::LWFNodeGroup::$_1
using LWFNodeGroup_1 = thunk::Lambda<0xF5D548, 8, 0xF91134, 0xE4BBC0, 4, thunk::cap(4, 4)>;
// LWFNodeGroup::LWFNodeGroup::$_0
using LWFNodeGroup_0 = thunk::Lambda<0xF5D574, 8, 0xF91148, 0xE4BC50, 4, thunk::cap(4, 4)>;

}  // namespace LWFNodeGroup_w3

RW_FUNCTION(0x5D5430, "LWFNodeGroup::LWFNodeGroup::$_1 destroy()", LWFNodeGroup_w3::LWFNodeGroup_1::destroy);
RW_FUNCTION(0x5D5432, "LWFNodeGroup::LWFNodeGroup::$_1 destroy_deallocate()", LWFNodeGroup_w3::LWFNodeGroup_1::deleteThis);
RW_FUNCTION(0x5D5444, "LWFNodeGroup::LWFNodeGroup::$_1 target(const type_info&)", LWFNodeGroup_w3::LWFNodeGroup_1::target);
RW_FUNCTION(0x5D5464, "LWFNodeGroup::LWFNodeGroup::$_1 target_type()", LWFNodeGroup_w3::LWFNodeGroup_1::targetType);
RW_FUNCTION(0x5D5478, "LWFNodeGroup::LWFNodeGroup::$_0 ~__func() (deleting)", LWFNodeGroup_w3::LWFNodeGroup_0::deleteThis);
RW_FUNCTION(0x5D5480, "LWFNodeGroup::LWFNodeGroup::$_0 __clone()", LWFNodeGroup_w3::LWFNodeGroup_0::clone);
RW_FUNCTION(0x5D54AC, "LWFNodeGroup::LWFNodeGroup::$_0 __clone(__base*)", LWFNodeGroup_w3::LWFNodeGroup_0::cloneInto);
RW_FUNCTION(0x5D54CC, "LWFNodeGroup::LWFNodeGroup::$_0 destroy()", LWFNodeGroup_w3::LWFNodeGroup_0::destroy);
RW_FUNCTION(0x5D54CE, "LWFNodeGroup::LWFNodeGroup::$_0 destroy_deallocate()", LWFNodeGroup_w3::LWFNodeGroup_0::deleteThis);
RW_FUNCTION(0x5D54E0, "LWFNodeGroup::LWFNodeGroup::$_0 target(const type_info&)", LWFNodeGroup_w3::LWFNodeGroup_0::target);
RW_FUNCTION(0x5D5500, "LWFNodeGroup::LWFNodeGroup::$_0 target_type()", LWFNodeGroup_w3::LWFNodeGroup_0::targetType);

// LWFNodeGroup::LWFNodeGroup lambdas {this}: the group's update schedule follows its LWF nodes
namespace LWFNodeGroup_w3 {
// $_1: stop updating
void unscheduleUpdate(uint32_t lambda) { rw::call(0x894CB3, {rw::load<uint32_t>(lambda + 4)}); }   // Node::unscheduleUpdate
// $_0: start updating
void scheduleUpdate(uint32_t lambda) { rw::call(0x894BA7, {rw::load<uint32_t>(lambda + 4)}); }     // Node::scheduleUpdate
}  // namespace LWFNodeGroup_w3

RW_FUNCTION(0x5D543A, "LWFNodeGroup::LWFNodeGroup::$_1 operator()", LWFNodeGroup_w3::unscheduleUpdate);
RW_FUNCTION(0x5D54D6, "LWFNodeGroup::LWFNodeGroup::$_0 operator()", LWFNodeGroup_w3::scheduleUpdate);
