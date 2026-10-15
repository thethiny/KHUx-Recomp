// StageManager: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "../scene_main/FunctionThunks.hpp"

namespace StageManager_g5thunks {

// StageManager::isFullSetTargetRarity::$_2
using isFullSetTargetRarity_2 = thunk::Lambda<0xF6E214, 16, 0xFA84C8, 0xE9EFC0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// StageManager::getMostHigherRarity::$_1
using getMostHigherRarity_1 = thunk::Lambda<0xF6E240, 12, 0xFA84DC, 0xE9F080, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// StageManager::countOnDeckMedalAttribute::$_0
using countOnDeckMedalAttribute_0 = thunk::Lambda<0xF6E26C, 20, 0xFA84F0, 0xE9F150, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;

}  // namespace StageManager_g5thunks

RW_FUNCTION(0x75C8B4, "StageManager::isFullSetTargetRarity::$_2 ~__func() (deleting)", StageManager_g5thunks::isFullSetTargetRarity_2::deleteThis);
RW_FUNCTION(0x75C8BC, "StageManager::isFullSetTargetRarity::$_2 __clone()", StageManager_g5thunks::isFullSetTargetRarity_2::clone);
RW_FUNCTION(0x75C8EC, "StageManager::isFullSetTargetRarity::$_2 __clone(__base*)", StageManager_g5thunks::isFullSetTargetRarity_2::cloneInto);
RW_FUNCTION(0x75C914, "StageManager::isFullSetTargetRarity::$_2 destroy()", StageManager_g5thunks::isFullSetTargetRarity_2::destroy);
RW_FUNCTION(0x75C916, "StageManager::isFullSetTargetRarity::$_2 destroy_deallocate()", StageManager_g5thunks::isFullSetTargetRarity_2::deleteThis);
RW_FUNCTION(0x75C998, "StageManager::isFullSetTargetRarity::$_2 target(const type_info&)", StageManager_g5thunks::isFullSetTargetRarity_2::target);
RW_FUNCTION(0x75C9B8, "StageManager::isFullSetTargetRarity::$_2 target_type()", StageManager_g5thunks::isFullSetTargetRarity_2::targetType);
RW_FUNCTION(0x75C9CC, "StageManager::getMostHigherRarity::$_1 ~__func() (deleting)", StageManager_g5thunks::getMostHigherRarity_1::deleteThis);
RW_FUNCTION(0x75C9D4, "StageManager::getMostHigherRarity::$_1 __clone()", StageManager_g5thunks::getMostHigherRarity_1::clone);
RW_FUNCTION(0x75CA04, "StageManager::getMostHigherRarity::$_1 __clone(__base*)", StageManager_g5thunks::getMostHigherRarity_1::cloneInto);
RW_FUNCTION(0x75CA28, "StageManager::getMostHigherRarity::$_1 destroy()", StageManager_g5thunks::getMostHigherRarity_1::destroy);
RW_FUNCTION(0x75CA2A, "StageManager::getMostHigherRarity::$_1 destroy_deallocate()", StageManager_g5thunks::getMostHigherRarity_1::deleteThis);
RW_FUNCTION(0x75CAA8, "StageManager::getMostHigherRarity::$_1 target(const type_info&)", StageManager_g5thunks::getMostHigherRarity_1::target);
RW_FUNCTION(0x75CAC8, "StageManager::getMostHigherRarity::$_1 target_type()", StageManager_g5thunks::getMostHigherRarity_1::targetType);
RW_FUNCTION(0x75CADC, "StageManager::countOnDeckMedalAttribute::$_0 ~__func() (deleting)", StageManager_g5thunks::countOnDeckMedalAttribute_0::deleteThis);
RW_FUNCTION(0x75CAE4, "StageManager::countOnDeckMedalAttribute::$_0 __clone()", StageManager_g5thunks::countOnDeckMedalAttribute_0::clone);
RW_FUNCTION(0x75CB18, "StageManager::countOnDeckMedalAttribute::$_0 __clone(__base*)", StageManager_g5thunks::countOnDeckMedalAttribute_0::cloneInto);
RW_FUNCTION(0x75CB44, "StageManager::countOnDeckMedalAttribute::$_0 destroy()", StageManager_g5thunks::countOnDeckMedalAttribute_0::destroy);
RW_FUNCTION(0x75CB46, "StageManager::countOnDeckMedalAttribute::$_0 destroy_deallocate()", StageManager_g5thunks::countOnDeckMedalAttribute_0::deleteThis);
RW_FUNCTION(0x75CBD4, "StageManager::countOnDeckMedalAttribute::$_0 target(const type_info&)", StageManager_g5thunks::countOnDeckMedalAttribute_0::target);
RW_FUNCTION(0x75CBF4, "StageManager::countOnDeckMedalAttribute::$_0 target_type()", StageManager_g5thunks::countOnDeckMedalAttribute_0::targetType);
