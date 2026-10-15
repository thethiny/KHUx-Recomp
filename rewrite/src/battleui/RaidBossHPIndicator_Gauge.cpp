// RaidBossHPIndicator: the HP gauge animation (setHP and the direction steps its lambdas run when an animation
// ends) and the buff / debuff icon.
#include "RaidBossHPIndicator.hpp"

using namespace scene;
using namespace raidhp;

namespace {
constexpr uint32_t VT_HP_DOWN = 0xF5DC04;        // setHP()::$_3  damage animation finished
constexpr uint32_t VT_HP_UP = 0xF5DBD8;          // setHP()::$_4  heal animation finished
constexpr uint32_t VT_NEXT_DISP = 0xF5DBAC;      // Next_Disp()::$_5  show the next buff
constexpr uint32_t BUFF_NONE = 20;               // buffTurns value of an inactive buff
constexpr uint32_t ANIM_LOOP_DEFAULT = 0xFFFFFFFFu;

// the fill of the current gauge for `hp`: the remainder of a box, a full gauge when hp is a non-zero multiple
float boxRate(uint32_t hp, uint32_t perBox) {
    uint32_t rem = umod(hp, perBox);
    if (hp >= perBox && rem == 0) return 1.0f;
    return float(rem) / float(perBox);
}
}  // namespace

// ------------------------------------------------------------------------------------------- setHP
// Show `newHp`: the green bar jumps / the red bar drains after a delay on damage, the bar fills on healing.
// Crossing a box boundary breaks a box (the gauge refills from the next box).
uint32_t RaidBossHPIndicator::setHP(RaidBossHPIndicator *self, uint32_t newHp) {
    constexpr uint32_t RESULT = 0x8CC;     // r0 at return: the frame size the epilogue loaded
    uint32_t me = rw::guest_addr(self);
    uint32_t cur = self->hp;
    if (cur == newHp) return RESULT;

    uint32_t per = self->hpPerBox;
    float fper = float(per);
    uint32_t rem = umod(cur, per);
    float fromRate = float(rem) / fper;
    bool partial = cur == 0 || rem != 0;           // the current gauge is not a full box
    if (!partial) fromRate = 1.0f;
    float fromShown = atLeastSliver(fromRate);
    uint32_t remNew = umod(newHp, per);
    float toRate = float(remNew) / fper;
    if (newHp != 0 && remNew == 0) toRate = 1.0f;
    float target = atLeastSliver(toRate);

    uint32_t g = self->gauges;
    uint32_t greenBar = rw::load<uint32_t>(g + 0xC);
    uint32_t redBar = rw::load<uint32_t>(g + 0x10);
    // HP at the bottom of the current gauge and at its top
    uint32_t base = 0;
    if (cur >= per) {
        base = cur - rem;
        if (!partial) base -= per;
    }
    uint32_t top = per + base;

    if (cur > newHp && base >= newHp && int32_t(base) >= 1) {
        // the damage empties this gauge: break a box
        target = 0.0f;
        setPercent(greenBar, 0.0f);
        self->hpTarget = base;
        uint32_t maxFrame;
        {
            TempStr s(str::RAID_GAUGE_HPMAX, 16);
            maxFrame = search(me, s);
        }
        if (maxFrame) setVisible(maxFrame, 0);
        self->boxBroken = 1;
        uint32_t perNow = self->hpPerBox;
        if (self->hpMax > perNow) {
            uint32_t boxes = boxesFor(newHp, perNow);
            setBoxHpGauge(self, self->boxLeft, boxes);
            self->boxLeft = boxes;
            setBoxCountText(self, childByName(me, str::PANEL_4, 7));
            target = 0.0f;
        }
    } else if (top > newHp || cur >= newHp) {
        setPercent(greenBar, target * 100.0f);
        self->hpTarget = newHp;
    } else {
        // healing past this gauge: fill it first
        self->hpTarget = top;
        target = 1.0f;
    }

    if (self->hp > newHp) {
        // damage: the red bar follows the green one after a second
        uint32_t done = callFunc(VT_HP_DOWN, me);
        uint8_t broken = self->boxBroken;
        if (self->gaugeResetPending) {
            if (broken) {
                {
                    TempStr s(str::RAID_GAUGE_HP, 13);
                    setGaugeLength(self, search(me, s), 1.0f);
                }
                self->boxBroken = 0;
                self->gaugeResetPending = 0;
                self->dropCount = 0;
            }
        } else if (broken) {
            self->gaugeResetPending = 1;
        }
        if (self->dropCount == 0) setPercent(redBar, fromShown * 100.0f);
        uint32_t redAnim = gauge(self, 0);
        stopAllActions(redAnim);
        if (self->dropCount == 0) setScale(redAnim, fromShown);
        NumSync(self, redAnim, target);
        self->dropCount += 1;
        uint32_t timer = gauge(self, 1);
        stopAllActions(timer);
        runAfter(timer, 1.0f, done);
    } else {
        uint32_t cur2 = self->hp;
        int32_t bytes = int32_t(self->gaugesEnd - self->gauges);
        bool hasNumber = bytes >= 21;              // a 6th element: the HP number
        if (cur2 < newHp && !hasNumber) {
            // healing: the red bar (and the green one with it) fills after a second
            uint32_t done = callFunc(VT_HP_UP, me);
            uint32_t greenAnim = gauge(self, 2);
            stopAllActions(greenAnim);
            setScale(greenAnim, fromShown);
            setPercent(greenBar, fromShown * 100.0f);
            NumSync(self, greenAnim, target);
            uint32_t timer = gauge(self, 1);
            stopAllActions(timer);
            runAfter(timer, 1.0f, done);
        } else if (hasNumber) {
            setAtlasNumber(gauge(self, 5), int32_t(newHp));
        }
    }
    self->hp = newHp;
    return RESULT;
}
RW_FUNCTION(0x5E6304, "RaidBossHPIndicator::setHP", RaidBossHPIndicator::setHP);

// ------------------------------------------------------------------------------------------- direction steps
void RaidBossHPIndicator::setHPDownDir(RaidBossHPIndicator *self) {
    uint32_t g = self->gauges;
    uint32_t greenBar = rw::load<uint32_t>(g + 0xC);
    uint32_t redBar = rw::load<uint32_t>(g + 0x10);
    uint32_t redAnim = rw::load<uint32_t>(g);
    setPercent(redBar, 100.0f);
    stopAllActions(redAnim);
    uint32_t cur = self->hp;
    uint32_t per = self->hpPerBox;
    float rate = boxRate(cur, per);
    self->hpTarget = cur;
    float shown = atLeastSliver(rate);
    if (cur != 0) {
        setPercent(greenBar, shown * 100.0f);
        setScale(redAnim, 1.0f);
        NumSync(self, redAnim, shown);
        self->dropCount = 0;
        if (self->hpMax > self->hpPerBox) setBoxHpGauge(self, self->boxLeft, self->boxLeft);
    } else {
        self->boxLeft = 0;
        setPercent(redBar, 0.0f);
        setPercent(greenBar, 0.0f);
        self->dropCount = 0;
        if (self->hpMax > self->hpPerBox) setBoxHpGauge(self, self->boxLeft, self->boxLeft);
    }
}
RW_FUNCTION(0x5E78CC, "RaidBossHPIndicator::setHPDownDir", RaidBossHPIndicator::setHPDownDir);

void RaidBossHPIndicator::setHPDirEnd(RaidBossHPIndicator *self) {
    self->dropCount = 0;
    if (self->hpMax > self->hpPerBox) setBoxHpGauge(self, self->boxLeft, self->boxLeft);
}
RW_FUNCTION(0x5E7A1C, "RaidBossHPIndicator::setHPDirEnd", RaidBossHPIndicator::setHPDirEnd);

void RaidBossHPIndicator::setHPUpDir(RaidBossHPIndicator *self) {
    uint32_t me = rw::guest_addr(self);
    uint32_t g = self->gauges;
    uint32_t greenAnim = rw::load<uint32_t>(g + 8);
    uint32_t greenBar = rw::load<uint32_t>(g + 0xC);
    setPercent(rw::load<uint32_t>(g + 0x10), 0.0f);
    stopAllActions(greenAnim);
    uint32_t cur = self->hp;
    float shown = atLeastSliver(boxRate(cur, self->hpPerBox));
    if (self->boxLeft == self->boxTotal) {
        // back in the top box: show the max-HP frame again and the gauge at its full length
        uint32_t maxFrame;
        {
            TempStr s(str::RAID_GAUGE_HPMAX, 16);
            maxFrame = search(me, s);
        }
        if (maxFrame) setVisible(maxFrame, 1);
        uint32_t hpMax = self->hpMax;
        uint32_t per = self->hpPerBox;
        TempStr s(str::RAID_GAUGE_HP, 13);
        uint32_t rem = umod(hpMax, per);
        uint32_t image = search(me, s);
        float rate = float(rem) / float(per);
        setGaugeLength(self, image, rem ? rate : 1.0f);
    }
    self->hpTarget = self->hp;
    setPercent(greenBar, 2.0f);
    setScale(greenAnim, 0.0f);
    NumSync(self, greenAnim, shown);
}
RW_FUNCTION(0x5E7A48, "RaidBossHPIndicator::setHPUpDir", RaidBossHPIndicator::setHPUpDir);

void RaidBossHPIndicator::setHPDownJustDir(RaidBossHPIndicator *self) {
    uint32_t g = self->gauges;
    uint32_t redAnim = rw::load<uint32_t>(g);
    uint32_t greenBar = rw::load<uint32_t>(g + 0xC);
    setPercent(rw::load<uint32_t>(g + 0x10), 100.0f);
    setPercent(greenBar, 100.0f);
    stopAllActions(redAnim);
    self->dropCount = 0;
    if (self->hpMax > self->hpPerBox) setBoxHpGauge(self, self->boxLeft, self->boxLeft);
}
RW_FUNCTION(0x5E7C88, "RaidBossHPIndicator::setHPDownJustDir", RaidBossHPIndicator::setHPDownJustDir);

void RaidBossHPIndicator::setHPUpJustDir(RaidBossHPIndicator *self) {
    uint32_t g = self->gauges;
    uint32_t redAnim = rw::load<uint32_t>(g);
    uint32_t greenBar = rw::load<uint32_t>(g + 0xC);
    setPercent(rw::load<uint32_t>(g + 0x10), 100.0f);
    setPercent(greenBar, 100.0f);
    stopAllActions(redAnim);
    self->dropCount = 0;
}
RW_FUNCTION(0x5E7CEC, "RaidBossHPIndicator::setHPUpJustDir", RaidBossHPIndicator::setHPUpJustDir);

void RaidBossHPIndicator::setHPUpDirEnd(RaidBossHPIndicator *self) {
    stopAllActions(gauge(self, 2));
    uint32_t per = self->hpPerBox;
    float rate = float(umod(self->hp, per)) / float(per);
    setPercent(gauge(self, 4), rate * 100.0f);
}
RW_FUNCTION(0x5E7D30, "RaidBossHPIndicator::setHPUpDirEnd", RaidBossHPIndicator::setHPUpDirEnd);

// ------------------------------------------------------------------------------------------- buffs / debuffs
void RaidBossHPIndicator::set_buf_debuf(RaidBossHPIndicator *self, int32_t idx, int32_t turns) {
    if (idx == 20) return;
    self->buffFlags = self->buffFlags | bit(uint32_t(idx) + 1);
    rw::set<int32_t>(self, 0x2DC + 4 * uint32_t(idx), int32_t(uint32_t(turns) - 1));
    Next_Disp(self);
}
RW_FUNCTION(0x5E7D90, "RaidBossHPIndicator::set_buf_debuf", RaidBossHPIndicator::set_buf_debuf);

// Show the buff at buffShown (moving on to the next active one when it is not active), then the next one
// a second later; hide the icon when no buff is active.
void RaidBossHPIndicator::Next_Disp(RaidBossHPIndicator *self) {
    uint32_t me = rw::guest_addr(self);
    uint32_t icon = self->buffIcon;
    uint32_t setVisibleFn = rw::load<uint32_t>(rw::load<uint32_t>(icon) + slot::setVisible);
    if (self->buffFlags == 0) {
        rw::call(setVisibleFn, {icon, 0});
        return;
    }
    rw::call(setVisibleFn, {icon, 1});
    uint32_t idx = self->buffShown;
    uint32_t flags = self->buffFlags;
    if (!(bit(idx + 1) & flags)) {
        do {
            uint32_t next = idx + 1;
            idx = next <= 19 ? next : 0;
        } while (!(bit(idx + 1) & flags));
        self->buffShown = idx;
    }
    rw::call(lib::Node_stopAllActions, {me});
    uint32_t anim = vcall(self->buffIcon, slot::getAnimation, {self->buffIcon});
    uint32_t playWithIndex = rw::load<uint32_t>(rw::load<uint32_t>(anim) + slot::anim_playWithIndex);
    rw::call(playWithIndex, {anim, self->buffShown, ANIM_LOOP_DEFAULT, ANIM_LOOP_DEFAULT});
    anim = vcall(self->buffTurnIcon, slot::getAnimation, {self->buffTurnIcon});
    playWithIndex = rw::load<uint32_t>(rw::load<uint32_t>(anim) + slot::anim_playWithIndex);
    uint32_t turns = rw::get<uint32_t>(self, 0x2DC + 4 * self->buffShown);
    rw::call(playWithIndex, {anim, turns, ANIM_LOOP_DEFAULT, ANIM_LOOP_DEFAULT});
    uint32_t delay = rw::call(lib::DelayTime_create, {fbits(1.0f)});
    Frame f;
    uint32_t fo = newFunction(f, VT_NEXT_DISP, {me});
    uint32_t next = rw::call(lib::CallFunc_create, {fo});
    uint32_t seq = rw::call(lib::Sequence_create, {delay, next, 0});
    rw::call(lib::Node_runAction, {me, seq});
    functionDestroy(fo);
}
RW_FUNCTION(0x5E7DB8, "RaidBossHPIndicator::Next_Disp", RaidBossHPIndicator::Next_Disp);

void RaidBossHPIndicator::clr_buf_debuf(RaidBossHPIndicator *self, int32_t idx) {
    if (idx == 20) return;
    self->buffFlags = self->buffFlags & ~bit(uint32_t(idx) + 1);
    rw::set<int32_t>(self, 0x2DC + 4 * uint32_t(idx), int32_t(BUFF_NONE));
}
RW_FUNCTION(0x5E7F40, "RaidBossHPIndicator::clr_buf_debuf", RaidBossHPIndicator::clr_buf_debuf);

void RaidBossHPIndicator::clr_buf_debuf_all(RaidBossHPIndicator *self) {
    setVisible(self->buffIcon, 0);
    self->buffFlags = 0;
}
RW_FUNCTION(0x5E7F64, "RaidBossHPIndicator::clr_buf_debuf_all", RaidBossHPIndicator::clr_buf_debuf_all);

namespace {
// One up / down pair of buffs (slots up = 2k, down = 2k + 1 of a group): a positive level shows the up buff
// (frame level + 19), a negative one the down buff (frame 22 - level), 0 clears both.
void setBuffPair(RaidBossHPIndicator *self, uint32_t up, int32_t level) {
    uint32_t down = up + 1;
    if (level >= 1) {
        self->buffFlags = self->buffFlags | bit(up + 1);
        self->buffTurns[up] = int32_t(uint32_t(level) + 19);
        RaidBossHPIndicator::Next_Disp(self);
        self->buffFlags = self->buffFlags & ~bit(down + 1);
        self->buffTurns[down] = int32_t(BUFF_NONE);
    } else if (level == 0) {
        self->buffTurns[up] = int32_t(BUFF_NONE);
        self->buffFlags = self->buffFlags & ~(bit(up + 1) | bit(down + 1));
        self->buffTurns[down] = int32_t(BUFF_NONE);
    } else {
        self->buffFlags = self->buffFlags | bit(down + 1);
        self->buffTurns[down] = int32_t(22u - uint32_t(level));
        RaidBossHPIndicator::Next_Disp(self);
        self->buffFlags = self->buffFlags & ~bit(up + 1);
        self->buffTurns[up] = int32_t(BUFF_NONE);
    }
}
}  // namespace

// buffs 0..7: attack up / down for four kinds
void RaidBossHPIndicator::set_attack_buff_status(RaidBossHPIndicator *self, int32_t a, int32_t b, int32_t c,
                                                 int32_t d) {
    setBuffPair(self, 0, a);
    setBuffPair(self, 2, b);
    setBuffPair(self, 4, c);
    setBuffPair(self, 6, d);
}
RW_FUNCTION(0x5E7F88, "RaidBossHPIndicator::set_attack_buff_status", RaidBossHPIndicator::set_attack_buff_status);

// buffs 8..15: defence up / down for four kinds
void RaidBossHPIndicator::set_defence_buff_status(RaidBossHPIndicator *self, int32_t a, int32_t b, int32_t c,
                                                  int32_t d) {
    setBuffPair(self, 8, a);
    setBuffPair(self, 10, b);
    setBuffPair(self, 12, c);
    setBuffPair(self, 14, d);
}
RW_FUNCTION(0x5E8188, "RaidBossHPIndicator::set_defence_buff_status", RaidBossHPIndicator::set_defence_buff_status);
