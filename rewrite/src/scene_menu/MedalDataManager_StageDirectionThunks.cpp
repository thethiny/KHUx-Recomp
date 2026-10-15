// The std::function thunks of StageDirection's lambdas (cocos_start $_3..$_7, stage_start $_2, init $_0/$_1).
// They sit in the binary right before MedalDataManager and were attributed to it; StageDirection itself is not
// rewritten yet, so they live here. Each __func is a vtable plus the captured words (StageDirection* at +4, and
// for stage_start's $_2 the stage number at +8); the lambda bodies are the small operator() functions below.
#include "../scene_main/FunctionThunks.hpp"

namespace {
// guest functions
constexpr uint32_t AUDIO_MANAGER_GET_INSTANCE = 0x5BECFD;  // audio::Manager::getInstance()
constexpr uint32_t AUDIO_MANAGER_GET_LAYER = 0x5BF33D;     // audio::Manager::getLayer(shared_ptr<Layer>* out, mgr, int)
constexpr uint32_t AUDIO_LAYER_PLAY_SE = 0x5BF785;         // audio::Layer::playSE(layer, ResourceID, int, int)
constexpr uint32_t STAGE_DIRECTION_COCOS_START = 0x7682D1; // StageDirection::cocos_start(int)
constexpr uint32_t NODE_SCHEDULE_UPDATE = 0x894BA7;        // cocos2d::Node::scheduleUpdate()
constexpr uint32_t NODE_UNSCHEDULE_UPDATE = 0x894CB3;      // cocos2d::Node::unscheduleUpdate()

// StageDirection fields the lambdas touch
constexpr uint32_t SD_NODE_A = 0x280;       // child nodes removed when the start direction ends
constexpr uint32_t SD_NODE_B = 0x284;
constexpr uint32_t SD_NODE_C = 0x294;
constexpr uint32_t SD_STATE_FLAG = 0x29C;   // int: set to 1 by cocos_start $_4
constexpr uint32_t SD_FINISHED = 0x2A0;     // byte
constexpr uint32_t SD_PHASE = 0x2A4;        // int phase (2: cocos start running, 4: start done)
constexpr uint32_t NODE_REMOVE_CHILD = 0x12C;   // cocos2d::Node::removeChild(child, cleanup) vtable byte offset

using thunk::cap;
using Start7 = thunk::Lambda<0xF6EA3C, 8, 0xFA9718, 0xEA1410, 4>;
using Start6 = thunk::Lambda<0xF6EA68, 8, 0xFA972C, 0xEA14A0, 4>;
using Start5 = thunk::Lambda<0xF6EA94, 8, 0xFA9740, 0xEA1530, 4, cap(4, 4)>;
using Start4 = thunk::Lambda<0xF6EAC0, 8, 0xFA9754, 0xEA15C0, 4, cap(4, 4)>;
using Start3 = thunk::Lambda<0xF6EAEC, 8, 0xFA9768, 0xEA1650, 4>;
using StageStart2 = thunk::Lambda<0xF6EB18, 12, 0xFA977C, 0xEA16E0, 4, cap(8, 4), cap(4, 4)>;
using Init1 = thunk::Lambda<0xF6EB44, 8, 0xFA9790, 0xEA1770, 4, cap(4, 4)>;
using Init0 = thunk::Lambda<0xF6EB70, 8, 0xFA97A4, 0xEA17F0, 4, cap(4, 4)>;

// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
void playSE(uint32_t id) {
    guest::Frame f(0x10);
    uint32_t mgr = rw::call(AUDIO_MANAGER_GET_INSTANCE, {});
    uint32_t layer = f.addr(4);   // shared_ptr<audio::Layer>
    rw::call(AUDIO_MANAGER_GET_LAYER, {layer, mgr, 0});
    rw::call(AUDIO_LAYER_PLAY_SE, {rw::load<uint32_t>(layer), id, 0, 0});
    uint32_t ctrl = rw::load<uint32_t>(layer + 4);
    if (ctrl) guest::release(ctrl);
}

struct StageDirectionLambdas {
    // cocos_start $_7 / $_6 / $_3: sound effects of the start direction
    static void start7(uint32_t) { playSE(509000550); }
    static void start6(uint32_t) { playSE(509000540); }
    static void start3(uint32_t) { playSE(509000200); }
    // cocos_start $_5: the direction is over - remove its three nodes, mark finished, phase 4
    static void start5(uint32_t self) {
        uint32_t sd = rw::load<uint32_t>(self + 4);
        for (uint32_t off : {SD_NODE_A, SD_NODE_B, SD_NODE_C}) {
            uint32_t child = rw::load<uint32_t>(sd + off);
            rw::call(rw::load<uint32_t>(rw::load<uint32_t>(sd) + NODE_REMOVE_CHILD), {sd, child, 1});
        }
        rw::save<uint8_t>(sd + SD_FINISHED, 1);
        rw::save<uint32_t>(sd + SD_PHASE, 4);
    }
    // cocos_start $_4
    static void start4(uint32_t self) { rw::save<uint32_t>(rw::load<uint32_t>(self + 4) + SD_STATE_FLAG, 1); }
    // stage_start $_2: run the cocos start direction for the captured stage, phase 2
    static void stageStart2(uint32_t self) {
        uint32_t sd = rw::load<uint32_t>(self + 4);
        rw::call(STAGE_DIRECTION_COCOS_START, {sd, rw::load<uint32_t>(self + 8)});
        rw::save<uint32_t>(sd + SD_PHASE, 2);
    }
    // init $_1 / $_0: stop / start the per-frame update
    static void init1(uint32_t self) { rw::call(NODE_UNSCHEDULE_UPDATE, {rw::load<uint32_t>(self + 4)}); }
    static void init0(uint32_t self) { rw::call(NODE_SCHEDULE_UPDATE, {rw::load<uint32_t>(self + 4)}); }
};
}  // namespace

// cocos_start $_7
RW_FUNCTION(0x76A6D0, "StageDirection::cocos_start $_7 __func::~__func (deleting)", Start7::deleteThis);
RW_FUNCTION(0x76A6D8, "StageDirection::cocos_start $_7 __func::__clone()", Start7::clone);
RW_FUNCTION(0x76A6FC, "StageDirection::cocos_start $_7 __func::__clone(__base*)", Start7::cloneInto);
RW_FUNCTION(0x76A718, "StageDirection::cocos_start $_7 __func::destroy", Start7::destroy);
RW_FUNCTION(0x76A71A, "StageDirection::cocos_start $_7 __func::destroy_deallocate", Start7::deleteThis);
RW_FUNCTION(0x76A724, "StageDirection::cocos_start $_7 __func::operator()", StageDirectionLambdas::start7);
RW_FUNCTION(0x76A7A8, "StageDirection::cocos_start $_7 __func::target", Start7::target);
RW_FUNCTION(0x76A7C8, "StageDirection::cocos_start $_7 __func::target_type", Start7::targetType);
// cocos_start $_6
RW_FUNCTION(0x76A7DC, "StageDirection::cocos_start $_6 __func::~__func (deleting)", Start6::deleteThis);
RW_FUNCTION(0x76A7E4, "StageDirection::cocos_start $_6 __func::__clone()", Start6::clone);
RW_FUNCTION(0x76A808, "StageDirection::cocos_start $_6 __func::__clone(__base*)", Start6::cloneInto);
RW_FUNCTION(0x76A824, "StageDirection::cocos_start $_6 __func::destroy", Start6::destroy);
RW_FUNCTION(0x76A826, "StageDirection::cocos_start $_6 __func::destroy_deallocate", Start6::deleteThis);
RW_FUNCTION(0x76A830, "StageDirection::cocos_start $_6 __func::operator()", StageDirectionLambdas::start6);
RW_FUNCTION(0x76A8B4, "StageDirection::cocos_start $_6 __func::target", Start6::target);
RW_FUNCTION(0x76A8D4, "StageDirection::cocos_start $_6 __func::target_type", Start6::targetType);
// cocos_start $_5
RW_FUNCTION(0x76A8E8, "StageDirection::cocos_start $_5 __func::~__func (deleting)", Start5::deleteThis);
RW_FUNCTION(0x76A8F0, "StageDirection::cocos_start $_5 __func::__clone()", Start5::clone);
RW_FUNCTION(0x76A91C, "StageDirection::cocos_start $_5 __func::__clone(__base*)", Start5::cloneInto);
RW_FUNCTION(0x76A93C, "StageDirection::cocos_start $_5 __func::destroy", Start5::destroy);
RW_FUNCTION(0x76A93E, "StageDirection::cocos_start $_5 __func::destroy_deallocate", Start5::deleteThis);
RW_FUNCTION(0x76A946, "StageDirection::cocos_start $_5 __func::operator()", StageDirectionLambdas::start5);
RW_FUNCTION(0x76A99C, "StageDirection::cocos_start $_5 __func::target", Start5::target);
RW_FUNCTION(0x76A9BC, "StageDirection::cocos_start $_5 __func::target_type", Start5::targetType);
// cocos_start $_4
RW_FUNCTION(0x76A9D0, "StageDirection::cocos_start $_4 __func::~__func (deleting)", Start4::deleteThis);
RW_FUNCTION(0x76A9D8, "StageDirection::cocos_start $_4 __func::__clone()", Start4::clone);
RW_FUNCTION(0x76AA04, "StageDirection::cocos_start $_4 __func::__clone(__base*)", Start4::cloneInto);
RW_FUNCTION(0x76AA24, "StageDirection::cocos_start $_4 __func::destroy", Start4::destroy);
RW_FUNCTION(0x76AA26, "StageDirection::cocos_start $_4 __func::destroy_deallocate", Start4::deleteThis);
RW_FUNCTION(0x76AA2E, "StageDirection::cocos_start $_4 __func::operator()", StageDirectionLambdas::start4);
RW_FUNCTION(0x76AA3C, "StageDirection::cocos_start $_4 __func::target", Start4::target);
RW_FUNCTION(0x76AA5C, "StageDirection::cocos_start $_4 __func::target_type", Start4::targetType);
// cocos_start $_3
RW_FUNCTION(0x76AA70, "StageDirection::cocos_start $_3 __func::~__func (deleting)", Start3::deleteThis);
RW_FUNCTION(0x76AA78, "StageDirection::cocos_start $_3 __func::__clone()", Start3::clone);
RW_FUNCTION(0x76AA9C, "StageDirection::cocos_start $_3 __func::__clone(__base*)", Start3::cloneInto);
RW_FUNCTION(0x76AAB8, "StageDirection::cocos_start $_3 __func::destroy", Start3::destroy);
RW_FUNCTION(0x76AABA, "StageDirection::cocos_start $_3 __func::destroy_deallocate", Start3::deleteThis);
RW_FUNCTION(0x76AAC4, "StageDirection::cocos_start $_3 __func::operator()", StageDirectionLambdas::start3);
RW_FUNCTION(0x76AB48, "StageDirection::cocos_start $_3 __func::target", Start3::target);
RW_FUNCTION(0x76AB68, "StageDirection::cocos_start $_3 __func::target_type", Start3::targetType);
// stage_start $_2
RW_FUNCTION(0x76AB7C, "StageDirection::stage_start $_2 __func::~__func (deleting)", StageStart2::deleteThis);
RW_FUNCTION(0x76AB84, "StageDirection::stage_start $_2 __func::__clone()", StageStart2::clone);
RW_FUNCTION(0x76ABB4, "StageDirection::stage_start $_2 __func::__clone(__base*)", StageStart2::cloneInto);
RW_FUNCTION(0x76ABD8, "StageDirection::stage_start $_2 __func::destroy", StageStart2::destroy);
RW_FUNCTION(0x76ABDA, "StageDirection::stage_start $_2 __func::destroy_deallocate", StageStart2::deleteThis);
RW_FUNCTION(0x76ABE2, "StageDirection::stage_start $_2 __func::operator()", StageDirectionLambdas::stageStart2);
RW_FUNCTION(0x76ABFC, "StageDirection::stage_start $_2 __func::target", StageStart2::target);
RW_FUNCTION(0x76AC1C, "StageDirection::stage_start $_2 __func::target_type", StageStart2::targetType);
// init $_1
RW_FUNCTION(0x76AC30, "StageDirection::init $_1 __func::~__func (deleting)", Init1::deleteThis);
RW_FUNCTION(0x76AC38, "StageDirection::init $_1 __func::__clone()", Init1::clone);
RW_FUNCTION(0x76AC64, "StageDirection::init $_1 __func::__clone(__base*)", Init1::cloneInto);
RW_FUNCTION(0x76AC84, "StageDirection::init $_1 __func::destroy", Init1::destroy);
RW_FUNCTION(0x76AC86, "StageDirection::init $_1 __func::destroy_deallocate", Init1::deleteThis);
RW_FUNCTION(0x76AC8E, "StageDirection::init $_1 __func::operator()", StageDirectionLambdas::init1);
RW_FUNCTION(0x76AC98, "StageDirection::init $_1 __func::target", Init1::target);
RW_FUNCTION(0x76ACB8, "StageDirection::init $_1 __func::target_type", Init1::targetType);
// init $_0
RW_FUNCTION(0x76ACCC, "StageDirection::init $_0 __func::~__func (deleting)", Init0::deleteThis);
RW_FUNCTION(0x76ACD4, "StageDirection::init $_0 __func::__clone()", Init0::clone);
RW_FUNCTION(0x76AD00, "StageDirection::init $_0 __func::__clone(__base*)", Init0::cloneInto);
RW_FUNCTION(0x76AD20, "StageDirection::init $_0 __func::destroy", Init0::destroy);
RW_FUNCTION(0x76AD22, "StageDirection::init $_0 __func::destroy_deallocate", Init0::deleteThis);
RW_FUNCTION(0x76AD2A, "StageDirection::init $_0 __func::operator()", StageDirectionLambdas::init0);
RW_FUNCTION(0x76AD34, "StageDirection::init $_0 __func::target", Init0::target);
RW_FUNCTION(0x76AD54, "StageDirection::init $_0 __func::target_type", Init0::targetType);

// cocos_start $_8: only its target / target_type sit in this range
namespace {
using Start8 = thunk::Lambda<0xF6EA10, 8, 0xFA9704, 0xEA1380, 4>;
}
RW_FUNCTION(0x76A69C, "StageDirection::cocos_start $_8 __func::target", Start8::target);
RW_FUNCTION(0x76A6BC, "StageDirection::cocos_start $_8 __func::target_type", Start8::targetType);
