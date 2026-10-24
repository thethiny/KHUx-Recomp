// BattleResultDialog: lambda bodies and std::function slots that run in game but were left after batch 2 (batch 3,
// named from their __func vtables' target_type). `functor` is the __func object: vtable at +0, the dialog at +4.
#include "BattleResultDialog.hpp"

using namespace brd;
using namespace brd::g;

namespace {

constexpr uint32_t BRD_close = 0x582AAD;
constexpr uint32_t BRD_playLevelUpAnimation = 0x58362D;
constexpr uint32_t BRD_updateLuxGauge = 0x57DF21;
constexpr uint32_t BRD_updateLuxLvNext = 0x58379D;
constexpr uint32_t BRD_initResultLwf = 0x582B91;          // (dialog, Node*, ResultLwfType, std::function<void()>)
constexpr uint32_t BRD_initSubMissionLwf = 0x582EF9;      // (dialog, index, std::function<void()>)
constexpr uint32_t ResultPointLayer_skipUpdate = 0x584AF1;   // (layer, bool)
constexpr uint32_t Audio_stop = 0x5BF3AD;                 // audio::Manager::stop(handle, fade ms)
constexpr uint32_t CustomTextView_setXml = 0x4F931D;      // (view, const std::string&)
constexpr uint32_t String_init = 0x47CD93;                // std::string(const char*, size_t)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t Misc_create = 0x786D5D;                // master::Misc::create(shared_ptr out, id)
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                   // (Size*, w, h)
constexpr uint32_t MoveTo_create = rwlib::cocos2d::MoveTo::create;              // (float, const Vec2&)
constexpr uint32_t Spawn_create = rwlib::cocos2d::Spawn::create;               // (action, ..., nullptr)
constexpr uint32_t V_getAnimation = 0x298;                // cocostudio::Armature::getAnimation
constexpr uint32_t V_animPlay = 0x68;                     // ArmatureAnimation::play(name, durationTo, loop)

// BattleResultDialog fields (BattleResultDialog.hpp)
constexpr uint32_t ROOT = 0x278;
constexpr uint32_t CAN_CLOSE = 0x298, LUX_RANK = 0x29C, TAP_ANIMATION = 0x2D0, NEXT_TEXT = 0x2D4, NEXT_PLATE = 0x2D8;
constexpr uint32_t LUX_POINTS = 0x318, RESULT_SE = 0x3C0, RESULT_DONE = 0x3C8;
constexpr uint32_t API_USER = 0x94;                       // APIManager: SafeBuffer, the user's Lux rank at +8
constexpr uint32_t S_TIP_LUX = 0xDFE849, S_AREA_POINT = 0xDF9FB4, S_ANIMATION1 = 0xDFD0AF;
constexpr int32_t TEXT_LUX_NEXT = 0x6252E89;

constexpr uint32_t VT_subMissionEnd = 0xF59E88;           // initSubMissionLwf $_21 [?, this, done]
constexpr uint32_t VT_stage_4a = 0xF5A250, VT_stage_4b = 0xF5A27C, VT_stage_4c = 0xF5A2A8, VT_stage_4d = 0xF5A2D4;
constexpr uint32_t Audio_getLayer = 0x5BF33D;             // audio::Manager::getLayer(int) (sret shared_ptr<Layer>)
constexpr uint32_t Layer_playSE = 0x5BF785;               // audio::Layer::playSE(ResourceID, int, int)
constexpr uint32_t SE_RESULT_LOOP = 0x1E56B9C2;
constexpr uint32_t COUNT_STARTED = 0x2E4;                 // byte
constexpr uint32_t LUX_POINTS_RUN = LUX_POINTS + 4, LUX_POINTS_DONE = LUX_POINTS + 5;   // ResultPointLayer bytes

int32_t userLuxRank() { return safe32(ld32(call(APIManager_getInstance, {}) + API_USER), 8); }

}  // namespace

struct BattleResultDialogBatch3 {
    // updateLuxLvNext $_24 [this](float, value): "next level in N" and the plate sliding in
    static void luxNextText(uint32_t functor, uint32_t /*ratioPtr*/, uint32_t valuePtr) {
        const uint32_t self = ld32(functor + 4);
        if (const uint32_t view = ld32(self + NEXT_TEXT)) {
            const uint32_t value = ld32(valuePtr);
            Frame f;
            const uint32_t td = f.alloc(8);
            call(TextData_get, {td, 0, uint32_t(TEXT_LUX_NEXT)});
            const uint32_t buffer = f.alloc(STRING_BUFFER_SIZE);
            call(StringBuffer_ctor, {buffer, ld32(ld32(td) + 4), value});
            const uint32_t text = f.alloc(12);
            call(String_init, {text, buffer, call(STRLEN, {buffer})});
            call(CustomTextView_setXml, {view, text});
            strFree(text);
            if (ld32(td + 4)) call(fn::shared_release, {ld32(td + 4)});
        }
        const uint32_t plate = ld32(self + NEXT_PLATE);
        if (!plate) return;
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(Vec2_copy, {pos, vcall(plate, node::getPosition, {plate})});
        vcall(ld32(self + NEXT_PLATE), node::setPositionY, {ld32(self + NEXT_PLATE), fbits(ldf(pos + 4) + 24.0f)});
        vcall(ld32(self + NEXT_PLATE), node::setOpacity, {ld32(self + NEXT_PLATE), 0});
        const uint32_t move = call(MoveTo_create, {fbits(0.15f), pos});
        const uint32_t spawn = call(Spawn_create, {move, call(FadeTo_create, {fbits(0.15f), 0xFF}), 0});
        const uint32_t seq = call(Sequence_create, {spawn, call(DelayTime_create, {fbits(0.032f)}), 0, 0});
        const uint32_t target = ld32(self + NEXT_PLATE);
        call(Node_runAction, {target, call(EaseSineOut_create, {seq})});
        setVisible(ld32(self + NEXT_PLATE), 1);
        call(Vec2_dtor, {pos});
    }

    // updateLuxGauge $_23 [this](float ratio, int left): at 0 left, a new Lux rank; the gauge width
    static void luxGauge(uint32_t functor, uint32_t ratioPtr, uint32_t leftPtr) {
        const int32_t left = int32_t(ld32(leftPtr));
        const float ratio = ldf(ratioPtr);
        const uint32_t self = ld32(functor + 4);
        Frame f;
        const uint32_t misc = f.alloc(8);
        call(Misc_create, {misc, 100});
        if (left <= 0) {
            const int32_t maxRank = safe32(ld32(misc), 4);
            const int32_t next = int32_t(ld32(self + LUX_RANK)) + 1;
            if (maxRank >= next) {
                st32(self + LUX_RANK, uint32_t(next));
                call(BRD_playLevelUpAnimation, {self});
            }
        }
        if (const uint32_t tip = find(ld32(self + ROOT), S_TIP_LUX, 7)) {
            const uint32_t setContentSize = ld32(ld32(tip) + node::setContentSize);
            const uint32_t size = vcall(tip, node::getContentSize, {tip});
            const uint32_t newSize = f.alloc(8);
            call(Size_ctor, {newSize, fbits(ratio * 263.0f), ld32(size + 4)});
            call(setContentSize, {tip, newSize});
        }
        if (ld32(misc + 4)) call(fn::shared_release, {ld32(misc + 4)});
    }

    // initSubMissionLwf $_21 __clone(): the dialog (stored before and after copying `done`) and `done`
    static uint32_t subMissionEndClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x28});
        if (obj) {
            st32(obj, VT_subMissionEnd);
            st32(obj + 8, ld32(self + 8));
            call(FunctionVV_copy, {obj + 0x10, self + 0x10});
            st32(obj + 8, ld32(self + 8));
        }
        return obj;
    }

    // openStageResult $_4 [this]: the clear LWF on Area_Point, then sub mission LWFs 1 and 2
    static void stageResultLwf(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        const uint32_t point = find(ld32(self + ROOT), S_AREA_POINT, 10);
        Frame f;
        const uint32_t next = newFunction(f, VT_stage_4a, {self});
        call(BRD_initResultLwf, {self, point, 0, next});
        functionDestroy(next);
    }
    static void stageSubMission1(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        Frame f;
        const uint32_t next = newFunction(f, VT_stage_4b, {self});
        call(BRD_initSubMissionLwf, {self, 1, next});
        functionDestroy(next);
    }
    static void stageSubMission2(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        Frame f;
        const uint32_t next = newFunction(f, VT_stage_4c, {self});
        call(BRD_initSubMissionLwf, {self, 2, next});
        functionDestroy(next);
    }

    static void stageSubMission3(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        Frame f;
        const uint32_t next = newFunction(f, VT_stage_4d, {self});
        call(BRD_initSubMissionLwf, {self, 3, next});
        functionDestroy(next);
    }
    // the LWFs are done: start the Lux count-up (with its SE unless already done) and, when closable, the tap
    // animation
    static void stageLwfsDone(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        st8(self + RESULT_DONE, 1);
        st8(self + COUNT_STARTED, 1);
        st8(self + LUX_POINTS_RUN, 1);
        if (!ld8(self + LUX_POINTS_DONE)) {
            Frame f;
            const uint32_t layer = f.alloc(8);
            call(Audio_getLayer, {layer, call(Audio_getInstance, {}), 0});
            const uint32_t handle = call(Layer_playSE, {ld32(layer), SE_RESULT_LOOP, 0, 0});
            if (ld32(layer + 4)) call(fn::shared_release, {ld32(layer + 4)});
            st32(self + RESULT_SE, handle);
        }
        if (!ld8(self + CAN_CLOSE)) return;
        const uint32_t armature = ld32(self + TAP_ANIMATION);
        const uint32_t animation = vcall(armature, V_getAnimation, {armature});
        const uint32_t play = ld32(ld32(animation) + V_animPlay);
        Frame f;
        const uint32_t name = newStr(f, S_ANIMATION1, 10);
        call(play, {animation, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
        setVisible(ld32(self + TAP_ANIMATION), 1);
    }

    // openStageResult $_3 [this]: a tap - close when done, else skip the Lux count-up to its end
    static void stageResultTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        if (ld32(typePtr) != 2) return;
        const uint32_t self = ld32(functor + 4);
        const uint8_t canClose = ld8(self + CAN_CLOSE);
        call(Audio_stop, {call(Audio_getInstance, {}), ld32(self + RESULT_SE), 50});
        if (canClose) {
            if (ld8(self + RESULT_DONE)) call(BRD_close, {self});
            return;
        }
        if (int32_t(ld32(self + LUX_RANK)) < userLuxRank()) {
            st32(self + LUX_RANK, uint32_t(userLuxRank()));
            call(BRD_playLevelUpAnimation, {self});
        }
        call(ResultPointLayer_skipUpdate, {self + LUX_POINTS, 1});
        st32(self + LUX_RANK, uint32_t(userLuxRank()));
        call(BRD_updateLuxGauge, {self});
    }

    // openStageResult $_2 [this]: the count-up ended - the next-level text, closable, the tap animation
    static void stageResultShown(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        call(BRD_updateLuxLvNext, {self});
        st8(self + CAN_CLOSE, 1);
        call(Audio_stop, {call(Audio_getInstance, {}), ld32(self + RESULT_SE), 50});
        if (!ld8(self + RESULT_DONE)) return;
        const uint32_t armature = ld32(self + TAP_ANIMATION);
        const uint32_t animation = vcall(armature, V_getAnimation, {armature});
        const uint32_t play = ld32(ld32(animation) + V_animPlay);
        Frame f;
        const uint32_t name = newStr(f, S_ANIMATION1, 10);
        call(play, {animation, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
        setVisible(ld32(self + TAP_ANIMATION), 1);
    }
};

using L = BattleResultDialogBatch3;
RW_FUNCTION(0x583918, "BattleResultDialog::updateLuxLvNext::$_24::operator()", L::luxNextText);
RW_FUNCTION(0x583B40, "BattleResultDialog::updateLuxGauge::$_23::operator()", L::luxGauge);
RW_FUNCTION(0x5840BC, "BattleResultDialog::initSubMissionLwf::$_21 __clone()", L::subMissionEndClone);
RW_FUNCTION(0x585AAC, "BattleResultDialog::openStageResult::$_4::operator()", L::stageResultLwf);
RW_FUNCTION(0x585C3C, "BattleResultDialog::openStageResult::$_4::{lambda()#1}::operator()", L::stageSubMission1);
RW_FUNCTION(0x585D64, "BattleResultDialog::openStageResult::$_4::{lambda()#1}::{lambda()#1}::operator()",
            L::stageSubMission2);
RW_FUNCTION(0x585E8C, "BattleResultDialog::openStageResult::$_4 (sub_585E8C): sub mission LWF 3", L::stageSubMission3);
RW_FUNCTION(0x585FB4, "BattleResultDialog::openStageResult::$_4 (sub_585FB4): the LWFs are done", L::stageLwfsDone);
RW_FUNCTION(0x58618C, "BattleResultDialog::openStageResult::$_3::operator()", L::stageResultTouched);
RW_FUNCTION(0x58631C, "BattleResultDialog::openStageResult::$_2::operator()", L::stageResultShown);
