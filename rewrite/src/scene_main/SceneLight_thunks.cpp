// SceneLight: the std::function slots of its lambdas, generated from the binary's __func vtables (each
// thunk was checked against the original instructions by emulating it). The lambda bodies (operator())
// are written by hand next to the functions that create them.
#include "FunctionThunks.hpp"

namespace SceneLight_lambdas {

// SceneLight::cmd_move::$_21
using cmd_move_21 = thunk::Lambda<0xF66C84, 8, 0xF9EFA0, 0xE7BBF0, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_20
using cmd_move_20 = thunk::Lambda<0xF66CB0, 8, 0xF9EFB4, 0xE7BC80, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_19
using cmd_move_19 = thunk::Lambda<0xF66CDC, 8, 0xF9EFC8, 0xE7BD10, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_18
using cmd_move_18 = thunk::Lambda<0xF66D08, 8, 0xF9EFDC, 0xE7BDA0, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_17
using cmd_move_17 = thunk::Lambda<0xF66D34, 8, 0xF9EFF0, 0xE7BE30, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_16
using cmd_move_16 = thunk::Lambda<0xF66D60, 8, 0xF9F004, 0xE7BEC0, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_15
using cmd_move_15 = thunk::Lambda<0xF66D8C, 8, 0xF9F018, 0xE7BF50, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_14
using cmd_move_14 = thunk::Lambda<0xF66DB8, 8, 0xF9F02C, 0xE7BFE0, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_13
using cmd_move_13 = thunk::Lambda<0xF66DE4, 8, 0xF9F040, 0xE7C070, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_12
using cmd_move_12 = thunk::Lambda<0xF66E10, 8, 0xF9F054, 0xE7C100, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_move::$_11
using cmd_move_11 = thunk::Lambda<0xF66E3C, 8, 0xF9F068, 0xE7C190, 4, thunk::cap(4, 4)>;
// SceneLight::runSpeech::$_10
using runSpeech_10 = thunk::Lambda<0xF66E68, 12, 0xF9F07C, 0xE7C230, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// SceneLight::cmd_text::$_9
using cmd_text_9 = thunk::Lambda<0xF66E94, 8, 0xF9F090, 0xE7C2C0, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_text::$_8
using cmd_text_8 = thunk::Lambda<0xF66EC0, 8, 0xF9F0A4, 0xE7C350, 4, thunk::cap(4, 4)>;
// SceneLight::cmd_bind::$_7
using cmd_bind_7 = thunk::Lambda<0xF66EEC, 20, 0xF9F0B8, 0xE7C3E0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4)>;
// SceneLight::next_scene::$_6
using next_scene_6 = thunk::Lambda<0xF66F18, 16, 0xF9F0CC, 0xE7C480, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneLight::init::$_5
using init_5 = thunk::Lambda<0xF66F44, 8, 0xF9F0E0, 0xE7C520, 4, thunk::cap(4, 4)>;
// SceneLight::init::$_4
using init_4 = thunk::Lambda<0xF66F70, 8, 0xF9F0F4, 0xE7C5B0, 4, thunk::cap(4, 4)>;
// SceneLight::init::$_3
using init_3 = thunk::Lambda<0xF66F9C, 8, 0xF9F108, 0xE7C640, 4>;
// SceneLight::init::$_2
using init_2 = thunk::Lambda<0xF66FC8, 8, 0xF9F11C, 0xE7C6D0, 4, thunk::cap(4, 4)>;
// SceneLight::init::$_1
using init_1 = thunk::Lambda<0xF66FF4, 8, 0xF9F130, 0xE7C740, 4, thunk::cap(4, 4)>;
// SceneLight::init::$_0
using init_0 = thunk::Lambda<0xF67020, 8, 0xF9F144, 0xE7C7B0, 4, thunk::cap(4, 4)>;

}  // namespace SceneLight_lambdas

RW_FUNCTION(0x6C50C2, "SceneLight::cmd_move::$_21 ~__func() (deleting)", SceneLight_lambdas::cmd_move_21::deleteThis);
RW_FUNCTION(0x6C50CC, "SceneLight::cmd_move::$_21 __clone()", SceneLight_lambdas::cmd_move_21::clone);
RW_FUNCTION(0x6C50F8, "SceneLight::cmd_move::$_21 __clone(__base*)", SceneLight_lambdas::cmd_move_21::cloneInto);
RW_FUNCTION(0x6C5118, "SceneLight::cmd_move::$_21 destroy()", SceneLight_lambdas::cmd_move_21::destroy);
RW_FUNCTION(0x6C511A, "SceneLight::cmd_move::$_21 destroy_deallocate()", SceneLight_lambdas::cmd_move_21::deleteThis);
RW_FUNCTION(0x6C5130, "SceneLight::cmd_move::$_21 target(const type_info&)", SceneLight_lambdas::cmd_move_21::target);
RW_FUNCTION(0x6C5150, "SceneLight::cmd_move::$_21 target_type()", SceneLight_lambdas::cmd_move_21::targetType);
RW_FUNCTION(0x6C5164, "SceneLight::cmd_move::$_20 ~__func() (deleting)", SceneLight_lambdas::cmd_move_20::deleteThis);
RW_FUNCTION(0x6C516C, "SceneLight::cmd_move::$_20 __clone()", SceneLight_lambdas::cmd_move_20::clone);
RW_FUNCTION(0x6C5198, "SceneLight::cmd_move::$_20 __clone(__base*)", SceneLight_lambdas::cmd_move_20::cloneInto);
RW_FUNCTION(0x6C51B8, "SceneLight::cmd_move::$_20 destroy()", SceneLight_lambdas::cmd_move_20::destroy);
RW_FUNCTION(0x6C51BA, "SceneLight::cmd_move::$_20 destroy_deallocate()", SceneLight_lambdas::cmd_move_20::deleteThis);
RW_FUNCTION(0x6C51D0, "SceneLight::cmd_move::$_20 target(const type_info&)", SceneLight_lambdas::cmd_move_20::target);
RW_FUNCTION(0x6C51F0, "SceneLight::cmd_move::$_20 target_type()", SceneLight_lambdas::cmd_move_20::targetType);
RW_FUNCTION(0x6C5204, "SceneLight::cmd_move::$_19 ~__func() (deleting)", SceneLight_lambdas::cmd_move_19::deleteThis);
RW_FUNCTION(0x6C520C, "SceneLight::cmd_move::$_19 __clone()", SceneLight_lambdas::cmd_move_19::clone);
RW_FUNCTION(0x6C5238, "SceneLight::cmd_move::$_19 __clone(__base*)", SceneLight_lambdas::cmd_move_19::cloneInto);
RW_FUNCTION(0x6C5258, "SceneLight::cmd_move::$_19 destroy()", SceneLight_lambdas::cmd_move_19::destroy);
RW_FUNCTION(0x6C525A, "SceneLight::cmd_move::$_19 destroy_deallocate()", SceneLight_lambdas::cmd_move_19::deleteThis);
RW_FUNCTION(0x6C5270, "SceneLight::cmd_move::$_19 target(const type_info&)", SceneLight_lambdas::cmd_move_19::target);
RW_FUNCTION(0x6C5290, "SceneLight::cmd_move::$_19 target_type()", SceneLight_lambdas::cmd_move_19::targetType);
RW_FUNCTION(0x6C52A4, "SceneLight::cmd_move::$_18 ~__func() (deleting)", SceneLight_lambdas::cmd_move_18::deleteThis);
RW_FUNCTION(0x6C52AC, "SceneLight::cmd_move::$_18 __clone()", SceneLight_lambdas::cmd_move_18::clone);
RW_FUNCTION(0x6C52D8, "SceneLight::cmd_move::$_18 __clone(__base*)", SceneLight_lambdas::cmd_move_18::cloneInto);
RW_FUNCTION(0x6C52F8, "SceneLight::cmd_move::$_18 destroy()", SceneLight_lambdas::cmd_move_18::destroy);
RW_FUNCTION(0x6C52FA, "SceneLight::cmd_move::$_18 destroy_deallocate()", SceneLight_lambdas::cmd_move_18::deleteThis);
RW_FUNCTION(0x6C5310, "SceneLight::cmd_move::$_18 target(const type_info&)", SceneLight_lambdas::cmd_move_18::target);
RW_FUNCTION(0x6C5330, "SceneLight::cmd_move::$_18 target_type()", SceneLight_lambdas::cmd_move_18::targetType);
RW_FUNCTION(0x6C5344, "SceneLight::cmd_move::$_17 ~__func() (deleting)", SceneLight_lambdas::cmd_move_17::deleteThis);
RW_FUNCTION(0x6C534C, "SceneLight::cmd_move::$_17 __clone()", SceneLight_lambdas::cmd_move_17::clone);
RW_FUNCTION(0x6C5378, "SceneLight::cmd_move::$_17 __clone(__base*)", SceneLight_lambdas::cmd_move_17::cloneInto);
RW_FUNCTION(0x6C5398, "SceneLight::cmd_move::$_17 destroy()", SceneLight_lambdas::cmd_move_17::destroy);
RW_FUNCTION(0x6C539A, "SceneLight::cmd_move::$_17 destroy_deallocate()", SceneLight_lambdas::cmd_move_17::deleteThis);
RW_FUNCTION(0x6C53B0, "SceneLight::cmd_move::$_17 target(const type_info&)", SceneLight_lambdas::cmd_move_17::target);
RW_FUNCTION(0x6C53D0, "SceneLight::cmd_move::$_17 target_type()", SceneLight_lambdas::cmd_move_17::targetType);
RW_FUNCTION(0x6C53E4, "SceneLight::cmd_move::$_16 ~__func() (deleting)", SceneLight_lambdas::cmd_move_16::deleteThis);
RW_FUNCTION(0x6C53EC, "SceneLight::cmd_move::$_16 __clone()", SceneLight_lambdas::cmd_move_16::clone);
RW_FUNCTION(0x6C5418, "SceneLight::cmd_move::$_16 __clone(__base*)", SceneLight_lambdas::cmd_move_16::cloneInto);
RW_FUNCTION(0x6C5438, "SceneLight::cmd_move::$_16 destroy()", SceneLight_lambdas::cmd_move_16::destroy);
RW_FUNCTION(0x6C543A, "SceneLight::cmd_move::$_16 destroy_deallocate()", SceneLight_lambdas::cmd_move_16::deleteThis);
RW_FUNCTION(0x6C5464, "SceneLight::cmd_move::$_16 target(const type_info&)", SceneLight_lambdas::cmd_move_16::target);
RW_FUNCTION(0x6C5484, "SceneLight::cmd_move::$_16 target_type()", SceneLight_lambdas::cmd_move_16::targetType);
RW_FUNCTION(0x6C5498, "SceneLight::cmd_move::$_15 ~__func() (deleting)", SceneLight_lambdas::cmd_move_15::deleteThis);
RW_FUNCTION(0x6C54A0, "SceneLight::cmd_move::$_15 __clone()", SceneLight_lambdas::cmd_move_15::clone);
RW_FUNCTION(0x6C54CC, "SceneLight::cmd_move::$_15 __clone(__base*)", SceneLight_lambdas::cmd_move_15::cloneInto);
RW_FUNCTION(0x6C54EC, "SceneLight::cmd_move::$_15 destroy()", SceneLight_lambdas::cmd_move_15::destroy);
RW_FUNCTION(0x6C54EE, "SceneLight::cmd_move::$_15 destroy_deallocate()", SceneLight_lambdas::cmd_move_15::deleteThis);
RW_FUNCTION(0x6C5504, "SceneLight::cmd_move::$_15 target(const type_info&)", SceneLight_lambdas::cmd_move_15::target);
RW_FUNCTION(0x6C5524, "SceneLight::cmd_move::$_15 target_type()", SceneLight_lambdas::cmd_move_15::targetType);
RW_FUNCTION(0x6C5538, "SceneLight::cmd_move::$_14 ~__func() (deleting)", SceneLight_lambdas::cmd_move_14::deleteThis);
RW_FUNCTION(0x6C5540, "SceneLight::cmd_move::$_14 __clone()", SceneLight_lambdas::cmd_move_14::clone);
RW_FUNCTION(0x6C556C, "SceneLight::cmd_move::$_14 __clone(__base*)", SceneLight_lambdas::cmd_move_14::cloneInto);
RW_FUNCTION(0x6C558C, "SceneLight::cmd_move::$_14 destroy()", SceneLight_lambdas::cmd_move_14::destroy);
RW_FUNCTION(0x6C558E, "SceneLight::cmd_move::$_14 destroy_deallocate()", SceneLight_lambdas::cmd_move_14::deleteThis);
RW_FUNCTION(0x6C55A4, "SceneLight::cmd_move::$_14 target(const type_info&)", SceneLight_lambdas::cmd_move_14::target);
RW_FUNCTION(0x6C55C4, "SceneLight::cmd_move::$_14 target_type()", SceneLight_lambdas::cmd_move_14::targetType);
RW_FUNCTION(0x6C55D8, "SceneLight::cmd_move::$_13 ~__func() (deleting)", SceneLight_lambdas::cmd_move_13::deleteThis);
RW_FUNCTION(0x6C55E0, "SceneLight::cmd_move::$_13 __clone()", SceneLight_lambdas::cmd_move_13::clone);
RW_FUNCTION(0x6C560C, "SceneLight::cmd_move::$_13 __clone(__base*)", SceneLight_lambdas::cmd_move_13::cloneInto);
RW_FUNCTION(0x6C562C, "SceneLight::cmd_move::$_13 destroy()", SceneLight_lambdas::cmd_move_13::destroy);
RW_FUNCTION(0x6C562E, "SceneLight::cmd_move::$_13 destroy_deallocate()", SceneLight_lambdas::cmd_move_13::deleteThis);
RW_FUNCTION(0x6C5644, "SceneLight::cmd_move::$_13 target(const type_info&)", SceneLight_lambdas::cmd_move_13::target);
RW_FUNCTION(0x6C5664, "SceneLight::cmd_move::$_13 target_type()", SceneLight_lambdas::cmd_move_13::targetType);
RW_FUNCTION(0x6C5678, "SceneLight::cmd_move::$_12 ~__func() (deleting)", SceneLight_lambdas::cmd_move_12::deleteThis);
RW_FUNCTION(0x6C5680, "SceneLight::cmd_move::$_12 __clone()", SceneLight_lambdas::cmd_move_12::clone);
RW_FUNCTION(0x6C56AC, "SceneLight::cmd_move::$_12 __clone(__base*)", SceneLight_lambdas::cmd_move_12::cloneInto);
RW_FUNCTION(0x6C56CC, "SceneLight::cmd_move::$_12 destroy()", SceneLight_lambdas::cmd_move_12::destroy);
RW_FUNCTION(0x6C56CE, "SceneLight::cmd_move::$_12 destroy_deallocate()", SceneLight_lambdas::cmd_move_12::deleteThis);
RW_FUNCTION(0x6C56E4, "SceneLight::cmd_move::$_12 target(const type_info&)", SceneLight_lambdas::cmd_move_12::target);
RW_FUNCTION(0x6C5704, "SceneLight::cmd_move::$_12 target_type()", SceneLight_lambdas::cmd_move_12::targetType);
RW_FUNCTION(0x6C5718, "SceneLight::cmd_move::$_11 ~__func() (deleting)", SceneLight_lambdas::cmd_move_11::deleteThis);
RW_FUNCTION(0x6C5720, "SceneLight::cmd_move::$_11 __clone()", SceneLight_lambdas::cmd_move_11::clone);
RW_FUNCTION(0x6C574C, "SceneLight::cmd_move::$_11 __clone(__base*)", SceneLight_lambdas::cmd_move_11::cloneInto);
RW_FUNCTION(0x6C576C, "SceneLight::cmd_move::$_11 destroy()", SceneLight_lambdas::cmd_move_11::destroy);
RW_FUNCTION(0x6C576E, "SceneLight::cmd_move::$_11 destroy_deallocate()", SceneLight_lambdas::cmd_move_11::deleteThis);
RW_FUNCTION(0x6C5784, "SceneLight::cmd_move::$_11 target(const type_info&)", SceneLight_lambdas::cmd_move_11::target);
RW_FUNCTION(0x6C57A4, "SceneLight::cmd_move::$_11 target_type()", SceneLight_lambdas::cmd_move_11::targetType);
RW_FUNCTION(0x6C57B8, "SceneLight::runSpeech::$_10 ~__func() (deleting)", SceneLight_lambdas::runSpeech_10::deleteThis);
RW_FUNCTION(0x6C57C0, "SceneLight::runSpeech::$_10 __clone()", SceneLight_lambdas::runSpeech_10::clone);
RW_FUNCTION(0x6C57F0, "SceneLight::runSpeech::$_10 __clone(__base*)", SceneLight_lambdas::runSpeech_10::cloneInto);
RW_FUNCTION(0x6C5814, "SceneLight::runSpeech::$_10 destroy()", SceneLight_lambdas::runSpeech_10::destroy);
RW_FUNCTION(0x6C5816, "SceneLight::runSpeech::$_10 destroy_deallocate()", SceneLight_lambdas::runSpeech_10::deleteThis);
RW_FUNCTION(0x6C587C, "SceneLight::runSpeech::$_10 target(const type_info&)", SceneLight_lambdas::runSpeech_10::target);
RW_FUNCTION(0x6C589C, "SceneLight::runSpeech::$_10 target_type()", SceneLight_lambdas::runSpeech_10::targetType);
RW_FUNCTION(0x6C58B0, "SceneLight::cmd_text::$_9 ~__func() (deleting)", SceneLight_lambdas::cmd_text_9::deleteThis);
RW_FUNCTION(0x6C58B8, "SceneLight::cmd_text::$_9 __clone()", SceneLight_lambdas::cmd_text_9::clone);
RW_FUNCTION(0x6C58E4, "SceneLight::cmd_text::$_9 __clone(__base*)", SceneLight_lambdas::cmd_text_9::cloneInto);
RW_FUNCTION(0x6C5904, "SceneLight::cmd_text::$_9 destroy()", SceneLight_lambdas::cmd_text_9::destroy);
RW_FUNCTION(0x6C5906, "SceneLight::cmd_text::$_9 destroy_deallocate()", SceneLight_lambdas::cmd_text_9::deleteThis);
RW_FUNCTION(0x6C591C, "SceneLight::cmd_text::$_9 target(const type_info&)", SceneLight_lambdas::cmd_text_9::target);
RW_FUNCTION(0x6C593C, "SceneLight::cmd_text::$_9 target_type()", SceneLight_lambdas::cmd_text_9::targetType);
RW_FUNCTION(0x6C5950, "SceneLight::cmd_text::$_8 ~__func() (deleting)", SceneLight_lambdas::cmd_text_8::deleteThis);
RW_FUNCTION(0x6C5958, "SceneLight::cmd_text::$_8 __clone()", SceneLight_lambdas::cmd_text_8::clone);
RW_FUNCTION(0x6C5984, "SceneLight::cmd_text::$_8 __clone(__base*)", SceneLight_lambdas::cmd_text_8::cloneInto);
RW_FUNCTION(0x6C59A4, "SceneLight::cmd_text::$_8 destroy()", SceneLight_lambdas::cmd_text_8::destroy);
RW_FUNCTION(0x6C59A6, "SceneLight::cmd_text::$_8 destroy_deallocate()", SceneLight_lambdas::cmd_text_8::deleteThis);
RW_FUNCTION(0x6C59BC, "SceneLight::cmd_text::$_8 target(const type_info&)", SceneLight_lambdas::cmd_text_8::target);
RW_FUNCTION(0x6C59DC, "SceneLight::cmd_text::$_8 target_type()", SceneLight_lambdas::cmd_text_8::targetType);
RW_FUNCTION(0x6C59F0, "SceneLight::cmd_bind::$_7 ~__func() (deleting)", SceneLight_lambdas::cmd_bind_7::deleteThis);
RW_FUNCTION(0x6C59F8, "SceneLight::cmd_bind::$_7 __clone()", SceneLight_lambdas::cmd_bind_7::clone);
RW_FUNCTION(0x6C5A2C, "SceneLight::cmd_bind::$_7 __clone(__base*)", SceneLight_lambdas::cmd_bind_7::cloneInto);
RW_FUNCTION(0x6C5A58, "SceneLight::cmd_bind::$_7 destroy()", SceneLight_lambdas::cmd_bind_7::destroy);
RW_FUNCTION(0x6C5A5A, "SceneLight::cmd_bind::$_7 destroy_deallocate()", SceneLight_lambdas::cmd_bind_7::deleteThis);
RW_FUNCTION(0x6C5A78, "SceneLight::cmd_bind::$_7 target(const type_info&)", SceneLight_lambdas::cmd_bind_7::target);
RW_FUNCTION(0x6C5A98, "SceneLight::cmd_bind::$_7 target_type()", SceneLight_lambdas::cmd_bind_7::targetType);
RW_FUNCTION(0x6C5AAC, "SceneLight::next_scene::$_6 ~__func() (deleting)", SceneLight_lambdas::next_scene_6::deleteThis);
RW_FUNCTION(0x6C5AB4, "SceneLight::next_scene::$_6 __clone()", SceneLight_lambdas::next_scene_6::clone);
RW_FUNCTION(0x6C5AE4, "SceneLight::next_scene::$_6 __clone(__base*)", SceneLight_lambdas::next_scene_6::cloneInto);
RW_FUNCTION(0x6C5B0C, "SceneLight::next_scene::$_6 destroy()", SceneLight_lambdas::next_scene_6::destroy);
RW_FUNCTION(0x6C5B0E, "SceneLight::next_scene::$_6 destroy_deallocate()", SceneLight_lambdas::next_scene_6::deleteThis);
RW_FUNCTION(0x6C5B24, "SceneLight::next_scene::$_6 target(const type_info&)", SceneLight_lambdas::next_scene_6::target);
RW_FUNCTION(0x6C5B44, "SceneLight::next_scene::$_6 target_type()", SceneLight_lambdas::next_scene_6::targetType);
RW_FUNCTION(0x6C5DA4, "SceneLight::init::$_5 ~__func() (deleting)", SceneLight_lambdas::init_5::deleteThis);
RW_FUNCTION(0x6C5DAC, "SceneLight::init::$_5 __clone()", SceneLight_lambdas::init_5::clone);
RW_FUNCTION(0x6C5DD8, "SceneLight::init::$_5 __clone(__base*)", SceneLight_lambdas::init_5::cloneInto);
RW_FUNCTION(0x6C5DF8, "SceneLight::init::$_5 destroy()", SceneLight_lambdas::init_5::destroy);
RW_FUNCTION(0x6C5DFA, "SceneLight::init::$_5 destroy_deallocate()", SceneLight_lambdas::init_5::deleteThis);
RW_FUNCTION(0x6C5E10, "SceneLight::init::$_5 target(const type_info&)", SceneLight_lambdas::init_5::target);
RW_FUNCTION(0x6C5E30, "SceneLight::init::$_5 target_type()", SceneLight_lambdas::init_5::targetType);
RW_FUNCTION(0x6C5E44, "SceneLight::init::$_4 ~__func() (deleting)", SceneLight_lambdas::init_4::deleteThis);
RW_FUNCTION(0x6C5E4C, "SceneLight::init::$_4 __clone()", SceneLight_lambdas::init_4::clone);
RW_FUNCTION(0x6C5E78, "SceneLight::init::$_4 __clone(__base*)", SceneLight_lambdas::init_4::cloneInto);
RW_FUNCTION(0x6C5E98, "SceneLight::init::$_4 destroy()", SceneLight_lambdas::init_4::destroy);
RW_FUNCTION(0x6C5E9A, "SceneLight::init::$_4 destroy_deallocate()", SceneLight_lambdas::init_4::deleteThis);
RW_FUNCTION(0x6C60A0, "SceneLight::init::$_4 target(const type_info&)", SceneLight_lambdas::init_4::target);
RW_FUNCTION(0x6C60C0, "SceneLight::init::$_4 target_type()", SceneLight_lambdas::init_4::targetType);
RW_FUNCTION(0x6C60D4, "SceneLight::init::$_3 ~__func() (deleting)", SceneLight_lambdas::init_3::deleteThis);
RW_FUNCTION(0x6C60DC, "SceneLight::init::$_3 __clone()", SceneLight_lambdas::init_3::clone);
RW_FUNCTION(0x6C6100, "SceneLight::init::$_3 __clone(__base*)", SceneLight_lambdas::init_3::cloneInto);
RW_FUNCTION(0x6C611C, "SceneLight::init::$_3 destroy()", SceneLight_lambdas::init_3::destroy);
RW_FUNCTION(0x6C611E, "SceneLight::init::$_3 destroy_deallocate()", SceneLight_lambdas::init_3::deleteThis);
RW_FUNCTION(0x6C6128, "SceneLight::init::$_3 target(const type_info&)", SceneLight_lambdas::init_3::target);
RW_FUNCTION(0x6C6148, "SceneLight::init::$_3 target_type()", SceneLight_lambdas::init_3::targetType);
RW_FUNCTION(0x6C615C, "SceneLight::init::$_2 ~__func() (deleting)", SceneLight_lambdas::init_2::deleteThis);
RW_FUNCTION(0x6C6164, "SceneLight::init::$_2 __clone()", SceneLight_lambdas::init_2::clone);
RW_FUNCTION(0x6C6190, "SceneLight::init::$_2 __clone(__base*)", SceneLight_lambdas::init_2::cloneInto);
RW_FUNCTION(0x6C61B0, "SceneLight::init::$_2 destroy()", SceneLight_lambdas::init_2::destroy);
RW_FUNCTION(0x6C61B2, "SceneLight::init::$_2 destroy_deallocate()", SceneLight_lambdas::init_2::deleteThis);
RW_FUNCTION(0x6C61C8, "SceneLight::init::$_2 target(const type_info&)", SceneLight_lambdas::init_2::target);
RW_FUNCTION(0x6C61E8, "SceneLight::init::$_2 target_type()", SceneLight_lambdas::init_2::targetType);
RW_FUNCTION(0x6C61FC, "SceneLight::init::$_1 ~__func() (deleting)", SceneLight_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6C6204, "SceneLight::init::$_1 __clone()", SceneLight_lambdas::init_1::clone);
RW_FUNCTION(0x6C6230, "SceneLight::init::$_1 __clone(__base*)", SceneLight_lambdas::init_1::cloneInto);
RW_FUNCTION(0x6C6250, "SceneLight::init::$_1 destroy()", SceneLight_lambdas::init_1::destroy);
RW_FUNCTION(0x6C6252, "SceneLight::init::$_1 destroy_deallocate()", SceneLight_lambdas::init_1::deleteThis);
RW_FUNCTION(0x6C6264, "SceneLight::init::$_1 target(const type_info&)", SceneLight_lambdas::init_1::target);
RW_FUNCTION(0x6C6284, "SceneLight::init::$_1 target_type()", SceneLight_lambdas::init_1::targetType);
RW_FUNCTION(0x6C6298, "SceneLight::init::$_0 ~__func() (deleting)", SceneLight_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6C62A0, "SceneLight::init::$_0 __clone()", SceneLight_lambdas::init_0::clone);
RW_FUNCTION(0x6C62CC, "SceneLight::init::$_0 __clone(__base*)", SceneLight_lambdas::init_0::cloneInto);
RW_FUNCTION(0x6C62EC, "SceneLight::init::$_0 destroy()", SceneLight_lambdas::init_0::destroy);
RW_FUNCTION(0x6C62EE, "SceneLight::init::$_0 destroy_deallocate()", SceneLight_lambdas::init_0::deleteThis);
RW_FUNCTION(0x6C6300, "SceneLight::init::$_0 target(const type_info&)", SceneLight_lambdas::init_0::target);
RW_FUNCTION(0x6C6320, "SceneLight::init::$_0 target_type()", SceneLight_lambdas::init_0::targetType);
