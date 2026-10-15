// SceneSphereBoard: the std::function slots of its lambdas, generated from the binary's __func vtables
// (each thunk's shape was checked by emulating the original instructions). The lambda bodies
// (operator()) are written by hand.
#include "../scene_main/FunctionThunks.hpp"

namespace SceneSphereBoard_w31_thunks {

// SceneSphereBoard::backScene::$_8
using backScene_8 = thunk::Lambda<0xF66AA0, 8, 0xF9EBAC, 0xE7B440, 4>;
// SceneSphereBoard::runOutAnimationMain::$_7 (captures a std::function at +8, copied with _ZNSt3__18functionIFvvEEC2ERKS2_)
using runOutAnimationMain_7 = thunk::LambdaFn<0xF66ACC, 32, 0xF9EBC0, 0xE7B4F0, 8, 8, 0x4683CD>;
// SceneSphereBoard::changeSceneNext::$_6
using changeSceneNext_6 = thunk::Lambda<0xF66AF8, 8, 0xF9EBD4, 0xE7B5B0, 4>;
// SceneSphereBoard::changeSceneNext::$_6 UlvE_
using changeSceneNext_6_UlvE_ = thunk::Lambda<0xF66B24, 8, 0xF9EBE8, 0xE7B660, 4>;
// SceneSphereBoard::init::$_5
using init_5 = thunk::Lambda<0xF66B50, 8, 0xF9EBFC, 0xE7B730, 4, thunk::cap(4, 4)>;
// SceneSphereBoard::init::$_4
using init_4 = thunk::Lambda<0xF66B7C, 8, 0xF9EC10, 0xE7B7F0, 4, thunk::cap(4, 4)>;

}  // namespace SceneSphereBoard_w31_thunks

RW_FUNCTION(0x6BFF94, "SceneSphereBoard::backScene::$_8 ~__func() (deleting)", SceneSphereBoard_w31_thunks::backScene_8::deleteThis);
RW_FUNCTION(0x6BFF9C, "SceneSphereBoard::backScene::$_8 __clone()", SceneSphereBoard_w31_thunks::backScene_8::clone);
RW_FUNCTION(0x6BFFC0, "SceneSphereBoard::backScene::$_8 __clone(__base*)", SceneSphereBoard_w31_thunks::backScene_8::cloneInto);
RW_FUNCTION(0x6BFFDC, "SceneSphereBoard::backScene::$_8 destroy()", SceneSphereBoard_w31_thunks::backScene_8::destroy);
RW_FUNCTION(0x6BFFDE, "SceneSphereBoard::backScene::$_8 destroy_deallocate()", SceneSphereBoard_w31_thunks::backScene_8::deleteThis);
RW_FUNCTION(0x6C0094, "SceneSphereBoard::backScene::$_8 target(const type_info&)", SceneSphereBoard_w31_thunks::backScene_8::target);
RW_FUNCTION(0x6C00B4, "SceneSphereBoard::backScene::$_8 target_type()", SceneSphereBoard_w31_thunks::backScene_8::targetType);
RW_FUNCTION(0x6C00C8, "SceneSphereBoard::runOutAnimationMain::$_7 ~__func()", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::destructor);
RW_FUNCTION(0x6C0104, "SceneSphereBoard::runOutAnimationMain::$_7 ~__func() (deleting)", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::destructorDelete);
RW_FUNCTION(0x6C0144, "SceneSphereBoard::runOutAnimationMain::$_7 __clone()", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::clone);
RW_FUNCTION(0x6C0194, "SceneSphereBoard::runOutAnimationMain::$_7 __clone(__base*)", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::cloneInto);
RW_FUNCTION(0x6C01C8, "SceneSphereBoard::runOutAnimationMain::$_7 destroy()", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::destroy);
RW_FUNCTION(0x6C01E8, "SceneSphereBoard::runOutAnimationMain::$_7 destroy_deallocate()", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::destroyDeallocate);
RW_FUNCTION(0x6C0224, "SceneSphereBoard::runOutAnimationMain::$_7 target(const type_info&)", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::target);
RW_FUNCTION(0x6C0244, "SceneSphereBoard::runOutAnimationMain::$_7 target_type()", SceneSphereBoard_w31_thunks::runOutAnimationMain_7::targetType);
RW_FUNCTION(0x6C0258, "SceneSphereBoard::changeSceneNext::$_6 ~__func() (deleting)", SceneSphereBoard_w31_thunks::changeSceneNext_6::deleteThis);
RW_FUNCTION(0x6C0260, "SceneSphereBoard::changeSceneNext::$_6 __clone()", SceneSphereBoard_w31_thunks::changeSceneNext_6::clone);
RW_FUNCTION(0x6C0284, "SceneSphereBoard::changeSceneNext::$_6 __clone(__base*)", SceneSphereBoard_w31_thunks::changeSceneNext_6::cloneInto);
RW_FUNCTION(0x6C02A0, "SceneSphereBoard::changeSceneNext::$_6 destroy()", SceneSphereBoard_w31_thunks::changeSceneNext_6::destroy);
RW_FUNCTION(0x6C02A2, "SceneSphereBoard::changeSceneNext::$_6 destroy_deallocate()", SceneSphereBoard_w31_thunks::changeSceneNext_6::deleteThis);
RW_FUNCTION(0x6C0348, "SceneSphereBoard::changeSceneNext::$_6 target(const type_info&)", SceneSphereBoard_w31_thunks::changeSceneNext_6::target);
RW_FUNCTION(0x6C0368, "SceneSphereBoard::changeSceneNext::$_6 target_type()", SceneSphereBoard_w31_thunks::changeSceneNext_6::targetType);
RW_FUNCTION(0x6C037C, "SceneSphereBoard::changeSceneNext::$_6 UlvE_ ~__func() (deleting)", SceneSphereBoard_w31_thunks::changeSceneNext_6_UlvE_::deleteThis);
RW_FUNCTION(0x6C0384, "SceneSphereBoard::changeSceneNext::$_6 UlvE_ __clone()", SceneSphereBoard_w31_thunks::changeSceneNext_6_UlvE_::clone);
RW_FUNCTION(0x6C03A8, "SceneSphereBoard::changeSceneNext::$_6 UlvE_ __clone(__base*)", SceneSphereBoard_w31_thunks::changeSceneNext_6_UlvE_::cloneInto);
RW_FUNCTION(0x6C03C4, "SceneSphereBoard::changeSceneNext::$_6 UlvE_ destroy()", SceneSphereBoard_w31_thunks::changeSceneNext_6_UlvE_::destroy);
RW_FUNCTION(0x6C03C6, "SceneSphereBoard::changeSceneNext::$_6 UlvE_ destroy_deallocate()", SceneSphereBoard_w31_thunks::changeSceneNext_6_UlvE_::deleteThis);
RW_FUNCTION(0x6C03D8, "SceneSphereBoard::changeSceneNext::$_6 UlvE_ target(const type_info&)", SceneSphereBoard_w31_thunks::changeSceneNext_6_UlvE_::target);
RW_FUNCTION(0x6C03F8, "SceneSphereBoard::changeSceneNext::$_6 UlvE_ target_type()", SceneSphereBoard_w31_thunks::changeSceneNext_6_UlvE_::targetType);
RW_FUNCTION(0x6C040C, "SceneSphereBoard::init::$_5 ~__func() (deleting)", SceneSphereBoard_w31_thunks::init_5::deleteThis);
RW_FUNCTION(0x6C0414, "SceneSphereBoard::init::$_5 __clone()", SceneSphereBoard_w31_thunks::init_5::clone);
RW_FUNCTION(0x6C0440, "SceneSphereBoard::init::$_5 __clone(__base*)", SceneSphereBoard_w31_thunks::init_5::cloneInto);
RW_FUNCTION(0x6C0460, "SceneSphereBoard::init::$_5 destroy()", SceneSphereBoard_w31_thunks::init_5::destroy);
RW_FUNCTION(0x6C0462, "SceneSphereBoard::init::$_5 destroy_deallocate()", SceneSphereBoard_w31_thunks::init_5::deleteThis);
RW_FUNCTION(0x6C0484, "SceneSphereBoard::init::$_5 target(const type_info&)", SceneSphereBoard_w31_thunks::init_5::target);
RW_FUNCTION(0x6C04A4, "SceneSphereBoard::init::$_5 target_type()", SceneSphereBoard_w31_thunks::init_5::targetType);
RW_FUNCTION(0x6C04B8, "SceneSphereBoard::init::$_4 ~__func() (deleting)", SceneSphereBoard_w31_thunks::init_4::deleteThis);
RW_FUNCTION(0x6C04C0, "SceneSphereBoard::init::$_4 __clone()", SceneSphereBoard_w31_thunks::init_4::clone);
RW_FUNCTION(0x6C04EC, "SceneSphereBoard::init::$_4 __clone(__base*)", SceneSphereBoard_w31_thunks::init_4::cloneInto);
RW_FUNCTION(0x6C050C, "SceneSphereBoard::init::$_4 destroy()", SceneSphereBoard_w31_thunks::init_4::destroy);
RW_FUNCTION(0x6C050E, "SceneSphereBoard::init::$_4 destroy_deallocate()", SceneSphereBoard_w31_thunks::init_4::deleteThis);
