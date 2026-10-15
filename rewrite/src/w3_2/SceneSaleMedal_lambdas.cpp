// SceneSaleMedal: the std::function plumbing of its lambdas (libc++ __func slots: deleting destructor, clone, clone into a
// buffer, destroy, destroy_deallocate, target, target_type), generated from the binary's __func vtables (size,
// captured bytes, type_info, type name and target offset read from each slot's instructions), plus the lambda bodies.
#include "../scene_main/FunctionThunks.hpp"

namespace SceneSaleMedal_w3 {

// SceneSaleMedal::backScene::$_12
using backScene_12 = thunk::Lambda<0xF627E4, 8, 0xF9A654, 0xE63C60, 4, thunk::cap(4, 4)>;

}  // namespace SceneSaleMedal_w3

RW_FUNCTION(0x65DA9E, "SceneSaleMedal::backScene::$_12 ~__func() (deleting)", SceneSaleMedal_w3::backScene_12::deleteThis);
RW_FUNCTION(0x65DAA8, "SceneSaleMedal::backScene::$_12 __clone()", SceneSaleMedal_w3::backScene_12::clone);
RW_FUNCTION(0x65DAD4, "SceneSaleMedal::backScene::$_12 __clone(__base*)", SceneSaleMedal_w3::backScene_12::cloneInto);
RW_FUNCTION(0x65DAF4, "SceneSaleMedal::backScene::$_12 destroy()", SceneSaleMedal_w3::backScene_12::destroy);
RW_FUNCTION(0x65DAF6, "SceneSaleMedal::backScene::$_12 destroy_deallocate()", SceneSaleMedal_w3::backScene_12::deleteThis);
