// BattleResultDialog lambda bodies that sit next to ResultPointLayer in the binary (owners.py attributes them to
// ResultPointLayer): the "popup opened" callbacks of the result screens (start the result LWF on "Area_Point" and,
// when it ends, start the counters), the centre-button taps (speed up the item list / skip the counters / close),
// the OK buttons, the "count ended" callbacks, and the sub-mission LWF delay. Each runs on the __func object
// (r0): the captured dialog is at +4 (+8 for the 8-aligned $_21).
#include "BattleResultDialog.hpp"

using namespace brd;

namespace {
constexpr uint32_t Audio_getLayer = 0x5BF33D;        // audio::Manager::getLayer(shared_ptr<Layer> out, mgr, int)
constexpr uint32_t AudioLayer_playSE = 0x5BF785;     // audio::Layer::playSE(layer, ResourceID, int, int)
constexpr uint32_t Audio_stop = 0x5BF3AD;            // audio::Manager::stop(mgr, int sound, int fadeFrames)
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Node_removeChild = 0x12C;         // vtable byte offsets
constexpr uint32_t Armature_getAnimation = 0x298;
constexpr uint32_t ArmatureAnimation_play = 0x68;    // play(const std::string&, int durationTo, int loop)

constexpr uint32_t STR_Area_Point = 0xDF9FB4;        // "Area_Point"
constexpr uint32_t STR_Animation1 = 0xDFD0AF;        // "Animation1"

constexpr uint32_t SE_COUNT_UP = 0x1E56B9C2;         // the counter sound (colosseum / raid results)
constexpr uint32_t SE_MUNNY_COUNT = 0x1E56B9CC;      // the munny counter sound (items obtained)

// lambdas created by these bodies (vtable + 8)
constexpr uint32_t VT_RankLwfEnd = 0xF59F0C;         // openClearRank $_19::(lambda)
constexpr uint32_t VT_ItemLwfEnd = 0xF59FBC;         // openGettingItem $_16::(lambda)
constexpr uint32_t VT_ColoLwfEnd = 0xF5A098;         // openColosseumResult $_12::(lambda)
constexpr uint32_t VT_RaidLwfEnd = 0xF5A148;         // openRaidResult $_9::(lambda)

constexpr int32_t TOUCH_BEGAN = 0, TOUCH_ENDED = 2;  // cocos2d::ui::Widget::TouchEventType
constexpr uint32_t API_USER_STATUS = 0x94;           // APIManager: shared_ptr<SafeBuffer> (Lux rank at data +8)

BattleResultDialog *captured(uint32_t func) { return dialog(ld32(func + 4)); }

// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
uint32_t playSE(uint32_t id) {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(Audio_getLayer, {layer, call(g::Audio_getInstance, {}), 0});
    uint32_t sound = call(AudioLayer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
    return sound;
}
void stopSound(uint32_t sound) { call(Audio_stop, {call(g::Audio_getInstance, {}), sound, 50}); }

// The result LWF over the screen's "Area_Point" node; `vtable` is the lambda (capturing the dialog) run when it ends.
void startResultLwf(BattleResultDialog *self, int32_t type, uint32_t vtable) {
    uint32_t area = find(self->root, STR_Area_Point, 10);
    Frame f;
    uint32_t onEnd = newFunction(f, vtable, {addr(self)});
    BattleResultDialog::initResultLwf(self, area, type, onEnd);
    functionDestroy(onEnd);
}

// BattleResultScrollView animation speeds: the item list played slowly, or hurried by a touch
void setListSpeed(BattleResultDialog *self, float rareDrop, float step) {
    call(g::ScrollView_setRareDropSpeed, {self->scrollView, fbits(rareDrop)});
    call(g::ScrollView_setAddRightInTime, {self->scrollView, fbits(step)});
    call(g::ScrollView_setDelayTime, {self->scrollView, fbits(step)});
    call(g::ScrollView_setListMoveTime, {self->scrollView, fbits(step)});
}

// The Lux rank of the user status (SafeBuffer data position 8)
int32_t userLuxRank(uint32_t api) {
    Frame f;
    uint32_t out = f.alloc(8);
    uint32_t sb = ld32(api + API_USER_STATUS);
    call(g::SafeBuffer_read, {sb, out, ld32(sb + 12) + 8, 4, 8});
    return int32_t(ld32(out));
}

// Skip the Lux counter: play the level-up animation when the rank grew, end the count, show the new rank.
void skipLuxCount(BattleResultDialog *self) {
    uint32_t api = call(g::APIManager_getInstance, {});
    int32_t shown = self->luxRank;
    if (shown < userLuxRank(api)) {
        self->luxRank = userLuxRank(api);
        BattleResultDialog::playLevelUpAnimation(self);
    }
    ResultPointLayer::skipUpdate(&self->luxPoints, 1);
    self->luxRank = userLuxRank(api);
    BattleResultDialog::updateLuxGauge(self);
}

// The Lux count ended: next-level text, the screen may close, the count sound stops, "tap the screen" plays.
void luxCountEnded(BattleResultDialog *self) {
    BattleResultDialog::updateLuxLvNext(self);
    self->canClose = 1;
    stopSound(self->field_3C0);
    uint32_t tap = self->tapAnimation;
    uint32_t anim = vcall(tap, Armature_getAnimation, {tap});
    uint32_t play = ld32(ld32(anim) + ArmatureAnimation_play);
    {
        Frame f;
        uint32_t name = newStr(f, STR_Animation1, 10);
        call(play, {anim, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
    }
    uint32_t tap2 = self->tapAnimation;
    vcall(tap2, node::setVisible, {tap2, 1});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ initSubMissionLwf
// $_21 [self, onEnd]: after the sub-mission LWF, wait subMissionDelay seconds, then run onEnd.
static void subMissionLwfEnded(uint32_t func) {
    BattleResultDialog *self = dialog(ld32(func + 8));
    uint32_t delay = call(g::DelayTime_create, {fbits(float(self->subMissionDelay))});
    uint32_t then = call(g::CallFunc_create, {func + 16});
    uint32_t seq = call(g::Sequence_create, {delay, then, 0});
    call(g::Node_runAction, {addr(self), seq});
}
RW_FUNCTION(0x5841A0, "BattleResultDialog::initSubMissionLwf $_21 operator()", subMissionLwfEnded);

// ------------------------------------------------------------------------------------------------ close
// $_20: detach the dialog from its parent (kept alive until the frame ends), then the close event.
static void closeDetach(uint32_t func) {
    BattleResultDialog *self = captured(func);
    if (!self->parent) return;
    call(Ref_retain, {addr(self)});
    call(g::Ref_autorelease, {addr(self)});
    uint32_t parent = self->parent;
    vcall(parent, Node_removeChild, {parent, self->shadow, 1});
    self->parent = 0;
    if (self->closeEvent_f) call(g::FunctionVV_call, {addr(self->closeEvent)});
}
RW_FUNCTION(0x584272, "BattleResultDialog::close $_20 operator()", closeDetach);

// ------------------------------------------------------------------------------------------------ openClearRank
// $_19: the popup is open - the result LWF.
static void rankOpened(uint32_t func) { startResultLwf(captured(func), BattleResultDialog::LWF_RESULT_REWARD, VT_RankLwfEnd); }
RW_FUNCTION(0x584354, "BattleResultDialog::openClearRank $_19 operator()", rankOpened);

// $_19::(lambda): the LWF ended - the item list animates in.
static void rankLwfEnded(uint32_t func) { captured(func)->scrollAnimating = 1; }
RW_FUNCTION(0x5844E2, "BattleResultDialog::openClearRank $_19 (lambda) operator()", rankLwfEnded);

// $_18: centre button: a touch speeds the list up; the release slows it down again (cancel sound while it may
// not close yet).
static void rankTap(uint32_t func, const uint32_t * /*ref*/, const int32_t *type) {
    BattleResultDialog *self = captured(func);
    if (*type == TOUCH_ENDED) {
        setListSpeed(self, 1.0f, 0.2f);
        if (!self->canClose) call(AudioPlaySystemSECancel, {});
    } else if (*type == TOUCH_BEGAN) {
        setListSpeed(self, 2.0f, 0.05f);
    }
}
RW_FUNCTION(0x584584, "BattleResultDialog::openClearRank $_18 operator()", rankTap);

// $_17: OK button
static void rankOk(uint32_t func, const uint32_t * /*ref*/, const int32_t *type) {
    if (*type != TOUCH_ENDED) return;
    BattleResultDialog *self = captured(func);
    if (self->canClose) BattleResultDialog::close(self);
}
RW_FUNCTION(0x58469E, "BattleResultDialog::openClearRank $_17 operator()", rankOk);

// ------------------------------------------------------------------------------------------------ openGettingItem
// $_16: the popup is open - the result LWF.
static void itemOpened(uint32_t func) { startResultLwf(captured(func), BattleResultDialog::LWF_RESULT_REWARD, VT_ItemLwfEnd); }
RW_FUNCTION(0x58474C, "BattleResultDialog::openGettingItem $_16 operator()", itemOpened);

// $_16::(lambda): the LWF ended - the item list and the munny counter start (with its sound).
static void itemLwfEnded(uint32_t func) {
    BattleResultDialog *self = captured(func);
    self->scrollAnimating = 1;
    self->points3.running = 1;
    if (!self->points3.finished) self->field_3C0 = playSE(SE_MUNNY_COUNT);
}
RW_FUNCTION(0x5848DC, "BattleResultDialog::openGettingItem $_16 (lambda) operator()", itemLwfEnded);

// $_15: centre button: speeds the list up while touched; the release ends the munny count when the screen may
// not close yet.
static void itemTap(uint32_t func, const uint32_t * /*ref*/, const int32_t *type) {
    BattleResultDialog *self = captured(func);
    if (*type == TOUCH_ENDED) {
        setListSpeed(self, 1.0f, 0.2f);
        if (!self->canClose) {
            stopSound(self->field_3C0);
            ResultPointLayer::skipUpdate(&self->points3, 1);
        }
    } else if (*type == TOUCH_BEGAN) {
        setListSpeed(self, 2.0f, 0.05f);
    }
}
RW_FUNCTION(0x584A18, "BattleResultDialog::openGettingItem $_15 operator()", itemTap);

// $_14: the munny count ended - its sound stops.
static void itemMoneyDone(uint32_t func) { stopSound(captured(func)->field_3C0); }
RW_FUNCTION(0x584B76, "BattleResultDialog::openGettingItem $_14 operator()", itemMoneyDone);

// $_13: OK button
static void itemOk(uint32_t func, const uint32_t * /*ref*/, const int32_t *type) {
    if (*type != TOUCH_ENDED) return;
    BattleResultDialog *self = captured(func);
    if (!self->canClose) return;
    stopSound(self->field_3C0);
    BattleResultDialog::close(self);
}
RW_FUNCTION(0x584C22, "BattleResultDialog::openGettingItem $_13 operator()", itemOk);

// ------------------------------------------------------------------------------------------------ openColosseumResult
// $_12: the popup is open - the result LWF.
static void coloOpened(uint32_t func) { startResultLwf(captured(func), BattleResultDialog::LWF_RESULT, VT_ColoLwfEnd); }
RW_FUNCTION(0x584CE8, "BattleResultDialog::openColosseumResult $_12 operator()", coloOpened);

// $_12::(lambda): the LWF ended - the point and Lux counters start (with the count sound).
static void coloLwfEnded(uint32_t func) {
    BattleResultDialog *self = captured(func);
    self->points0.running = 1;
    self->luxPoints.running = 1;
    if (!self->luxPoints.finished) self->field_3C0 = playSE(SE_COUNT_UP);
}
RW_FUNCTION(0x584E78, "BattleResultDialog::openColosseumResult $_12 (lambda) operator()", coloLwfEnded);

// $_11: centre button released: close, or skip the Lux count.
static void coloTap(uint32_t func, const uint32_t * /*ref*/, const int32_t *type) {
    if (*type != TOUCH_ENDED) return;
    BattleResultDialog *self = captured(func);
    if (self->canClose) {
        BattleResultDialog::close(self);
        return;
    }
    stopSound(self->field_3C0);
    skipLuxCount(self);
}
RW_FUNCTION(0x584FB4, "BattleResultDialog::openColosseumResult $_11 operator()", coloTap);

// $_10: the Lux count ended.
static void coloLuxDone(uint32_t func) { luxCountEnded(captured(func)); }
RW_FUNCTION(0x585138, "BattleResultDialog::openColosseumResult $_10 operator()", coloLuxDone);

// ------------------------------------------------------------------------------------------------ openRaidResult
// $_9 [self, type]: the popup is open - the result LWF of the captured type.
static void raidOpened(uint32_t func) {
    BattleResultDialog *self = captured(func);
    int32_t type = int32_t(ld32(func + 8));
    uint32_t area = find(self->root, STR_Area_Point, 10);
    Frame f;
    uint32_t onEnd = newFunction(f, VT_RaidLwfEnd, {addr(self)});
    BattleResultDialog::initResultLwf(self, area, type, onEnd);
    functionDestroy(onEnd);
}
RW_FUNCTION(0x5852CC, "BattleResultDialog::openRaidResult $_9 operator()", raidOpened);

// $_9::(lambda): the LWF ended - the raid point counter starts (with the count sound).
static void raidLwfEnded(uint32_t func) {
    BattleResultDialog *self = captured(func);
    self->points2.running = 1;
    if (!self->points2.finished) self->field_3C0 = playSE(SE_COUNT_UP);
}
RW_FUNCTION(0x585460, "BattleResultDialog::openRaidResult $_9 (lambda) operator()", raidLwfEnded);

// $_8: centre button released: close, or skip the point / munny / Lux counts.
static void raidTap(uint32_t func, const uint32_t * /*ref*/, const int32_t *type) {
    if (*type != TOUCH_ENDED) return;
    BattleResultDialog *self = captured(func);
    if (self->canClose) {
        BattleResultDialog::close(self);
        return;
    }
    stopSound(self->field_3C0);
    ResultPointLayer::skipUpdate(&self->points2, 1);
    ResultPointLayer::skipUpdate(&self->points3, 1);
    skipLuxCount(self);
}
RW_FUNCTION(0x585598, "BattleResultDialog::openRaidResult $_8 operator()", raidTap);

// $_7: the Lux count ended.
static void raidLuxDone(uint32_t func) { luxCountEnded(captured(func)); }
RW_FUNCTION(0x58573C, "BattleResultDialog::openRaidResult $_7 operator()", raidLuxDone);
