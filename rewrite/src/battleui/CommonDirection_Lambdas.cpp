// CommonDirection: the lambdas handed to cocos2d::CallFunc / std::function (their bodies and the libc++
// std::__function::__func thunks: deleting destructor, clone, clone into a buffer, destroy, destroy and free,
// target, target_type). Every lambda captures `this` (or the marker armature) and up to three more words.
#include "CommonDirection.hpp"

namespace {
using scene::call;
using scene::ld32;

constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t V_SET_VISIBLE = 0xB0;
constexpr uint32_t V_ARMATURE_GET_ANIMATION = 0x298;
constexpr uint32_t V_ANIMATION_PLAY_WITH_INDEX = 0x70;

// The fixed std::__function::__func<lambda, allocator, void()> thunks. VT = the __func vtable (+8), SIZE = the
// heap object size, WORDS = captured words copied after the vtable, TNAME = the lambda's type name (typeid name
// pointer), TINFO = its std::type_info.
template <uint32_t VT, uint32_t SIZE, uint32_t WORDS, uint32_t TNAME, uint32_t TINFO>
struct Thunks {
    static void deletingDtor(uint32_t self) { guest::deleteObject(self); }
    static uint32_t clone(uint32_t self) {
        uint32_t p = guest::newObject(SIZE);
        if (p) {
            rw::save<uint32_t>(p, VT);
            for (uint32_t k = 1; k <= WORDS; k++) rw::save<uint32_t>(p + 4 * k, ld32(self + 4 * k));
        }
        return p;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        rw::save<uint32_t>(dst, VT);
        for (uint32_t k = 1; k <= WORDS; k++) rw::save<uint32_t>(dst + 4 * k, ld32(self + 4 * k));
    }
    static void destroy(uint32_t) {}
    static void destroyDeallocate(uint32_t self) { guest::deleteObject(self); }
    static uint32_t target(uint32_t self, uint32_t typeInfo) { return ld32(typeInfo + 4) == TNAME ? self + 4 : 0; }
    static uint32_t targetType(uint32_t) { return TINFO; }
};

void setVisible(uint32_t node, uint32_t visible) { scene::vcall(node, V_SET_VISIBLE, {node, visible}); }
void playIndex(uint32_t armature, uint32_t index) {
    uint32_t anim = scene::vcall(armature, V_ARMATURE_GET_ANIMATION, {armature});
    scene::vcall(anim, V_ANIMATION_PLAY_WITH_INDEX, {anim, index, 0xFFFFFFFFu, 0xFFFFFFFFu});
}
CommonDirection *owner(uint32_t lambda) { return rw::host_ptr<CommonDirection>(ld32(lambda + 4)); }

// ------------------------------------------------------------------------------------------------ bodies
// battle_start $_12 / heal $_11: hide the fade layer after the flash
void hideFadeLayer(uint32_t lambda) { setVisible(owner(lambda)->fadeLayer.addr, 0); }
// show_boss_marker $_10 / hide_boss_marker $_9 (capture: the marker armature)
void markerShown(uint32_t lambda) { setVisible(ld32(lambda + 4), 1); }
void markerHidden(uint32_t lambda) { setVisible(ld32(lambda + 4), 0); }
// materialPanel_in $_8: the panel is gone
void materialHide(uint32_t lambda) {
    CommonDirection *self = owner(lambda);
    self->materialId = -1;
    setVisible(self->materialPanel.addr, 0);
}
// materialPanel_in $_7 / $_6: the panel's out (9) and hold (8) animations
void materialOut(uint32_t lambda) { playIndex(owner(lambda)->materialPanel.addr, 9); }
void materialStay(uint32_t lambda) { playIndex(owner(lambda)->materialPanel.addr, 8); }
// panel_update $_5 {this, slot, row, remaining}: keep holding for what is left of the 2 s
void panelMoved(uint32_t lambda) {
    CommonDirection::skilPanel_hold(owner(lambda), int32_t(ld32(lambda + 8)), rw::load<float>(lambda + 16));
}
// skilPanel_hold $_4 {this, slot, skillId}: the hold time is over
void holdOver(uint32_t lambda) { CommonDirection::skilPanel_out_common(owner(lambda), int32_t(ld32(lambda + 8))); }
// skilPanel_out_common $_3 {this, slot}: free the slot and its row
void outDone(uint32_t lambda) {
    CommonDirection *self = owner(lambda);
    setVisible(self->skillPanel[ld32(lambda + 8)].addr, 0);
    self->panelSkillId[ld32(lambda + 8)] = 0;
    self->panelState[ld32(lambda + 8)] = -1;
    for (uint32_t row = 0; row < 6; row++) {
        if (uint32_t(self->panelOrder[row].get()) == ld32(lambda + 8)) self->panelOrder[row] = -1;
    }
}
// skilPanel_in $_2 {this, slot, skillId}: the panel is in, hold it 2 s from now
void inDone(uint32_t lambda) {
    CommonDirection *self = owner(lambda);
    CommonDirection::skilPanel_hold(self, int32_t(ld32(lambda + 8)), 2.0f);
    self->panelHoldStart[ld32(lambda + 8)] = self->clock.get();
}
// init $_1 / $_0: stopUpdate / startUpdate
void stopUpdate(uint32_t lambda) { call(Node_unscheduleUpdate, {ld32(lambda + 4)}); }
void startUpdate(uint32_t lambda) { call(Node_scheduleUpdate, {ld32(lambda + 4)}); }

using BattleStart = Thunks<0xF6E644, 8, 1, 0xEA0590, 0xFA92DC>;    // battle_start $_12
using Heal = Thunks<0xF6E670, 8, 1, 0xEA0620, 0xFA92F0>;           // heal $_11
using ShowMarker = Thunks<0xF6E69C, 8, 1, 0xEA06C0, 0xFA9304>;     // show_boss_marker $_10
using HideMarker = Thunks<0xF6E6C8, 8, 1, 0xEA0770, 0xFA9318>;     // hide_boss_marker $_9
using MaterialHide = Thunks<0xF6E6F4, 8, 1, 0xEA0820, 0xFA932C>;   // materialPanel_in $_8
using MaterialOut = Thunks<0xF6E720, 8, 1, 0xEA08C0, 0xFA9340>;    // materialPanel_in $_7
using MaterialStay = Thunks<0xF6E74C, 8, 1, 0xEA0960, 0xFA9354>;   // materialPanel_in $_6
using PanelMoved = Thunks<0xF6E778, 20, 4, 0xEA09F0, 0xFA9368>;    // panel_update $_5
using HoldOver = Thunks<0xF6E7A4, 16, 3, 0xEA0A80, 0xFA937C>;      // skilPanel_hold $_4
using OutDone = Thunks<0xF6E7D0, 12, 2, 0xEA0B20, 0xFA9390>;       // skilPanel_out_common $_3
using InDone = Thunks<0xF6E7FC, 16, 3, 0xEA0BC0, 0xFA93A4>;        // skilPanel_in $_2
using StopUpdate = Thunks<0xF6E828, 8, 1, 0xEA0C50, 0xFA93B8>;     // init $_1
using StartUpdate = Thunks<0xF6E854, 8, 1, 0xEA0CD0, 0xFA93CC>;    // init $_0
}  // namespace

RW_FUNCTION(0x767162, "CommonDirection::battle_start::$_12::operator()", hideFadeLayer);
RW_FUNCTION(0x76720A, "CommonDirection::heal::$_11::operator()", hideFadeLayer);
RW_FUNCTION(0x7672B2, "CommonDirection::show_boss_marker::$_10::operator()", markerShown);
RW_FUNCTION(0x767356, "CommonDirection::hide_boss_marker::$_9::operator()", markerHidden);
RW_FUNCTION(0x7673FA, "CommonDirection::materialPanel_in::$_8::operator()", materialHide);
RW_FUNCTION(0x7674AA, "CommonDirection::materialPanel_in::$_7::operator()", materialOut);
RW_FUNCTION(0x767562, "CommonDirection::materialPanel_in::$_6::operator()", materialStay);
RW_FUNCTION(0x76762E, "CommonDirection::panel_update::$_5::operator()", panelMoved);
RW_FUNCTION(0x7676DA, "CommonDirection::skilPanel_hold::$_4::operator()", holdOver);
RW_FUNCTION(0x767786, "CommonDirection::skilPanel_out_common::$_3::operator()", outDone);
RW_FUNCTION(0x76787A, "CommonDirection::skilPanel_in::$_2::operator()", inDone);
RW_FUNCTION(0x767936, "CommonDirection::init::$_1::operator()", stopUpdate);
RW_FUNCTION(0x7679D2, "CommonDirection::init::$_0::operator()", startUpdate);

RW_FUNCTION(0x767104, "CommonDirection::battle_start::$_12 __func::~__func (deleting)", BattleStart::deletingDtor);
RW_FUNCTION(0x76710C, "CommonDirection::battle_start::$_12 __func::__clone", BattleStart::clone);
RW_FUNCTION(0x767138, "CommonDirection::battle_start::$_12 __func::__clone(dst)", BattleStart::cloneInto);
RW_FUNCTION(0x767158, "CommonDirection::battle_start::$_12 __func::destroy", BattleStart::destroy);
RW_FUNCTION(0x76715A, "CommonDirection::battle_start::$_12 __func::destroy_deallocate", BattleStart::destroyDeallocate);
RW_FUNCTION(0x767178, "CommonDirection::battle_start::$_12 __func::target", BattleStart::target);
RW_FUNCTION(0x767198, "CommonDirection::battle_start::$_12 __func::target_type", BattleStart::targetType);

RW_FUNCTION(0x7671AC, "CommonDirection::heal::$_11 __func::~__func (deleting)", Heal::deletingDtor);
RW_FUNCTION(0x7671B4, "CommonDirection::heal::$_11 __func::__clone", Heal::clone);
RW_FUNCTION(0x7671E0, "CommonDirection::heal::$_11 __func::__clone(dst)", Heal::cloneInto);
RW_FUNCTION(0x767200, "CommonDirection::heal::$_11 __func::destroy", Heal::destroy);
RW_FUNCTION(0x767202, "CommonDirection::heal::$_11 __func::destroy_deallocate", Heal::destroyDeallocate);
RW_FUNCTION(0x767220, "CommonDirection::heal::$_11 __func::target", Heal::target);
RW_FUNCTION(0x767240, "CommonDirection::heal::$_11 __func::target_type", Heal::targetType);

RW_FUNCTION(0x767254, "CommonDirection::show_boss_marker::$_10 __func::~__func (deleting)", ShowMarker::deletingDtor);
RW_FUNCTION(0x76725C, "CommonDirection::show_boss_marker::$_10 __func::__clone", ShowMarker::clone);
RW_FUNCTION(0x767288, "CommonDirection::show_boss_marker::$_10 __func::__clone(dst)", ShowMarker::cloneInto);
RW_FUNCTION(0x7672A8, "CommonDirection::show_boss_marker::$_10 __func::destroy", ShowMarker::destroy);
RW_FUNCTION(0x7672AA, "CommonDirection::show_boss_marker::$_10 __func::destroy_deallocate", ShowMarker::destroyDeallocate);
RW_FUNCTION(0x7672C4, "CommonDirection::show_boss_marker::$_10 __func::target", ShowMarker::target);
RW_FUNCTION(0x7672E4, "CommonDirection::show_boss_marker::$_10 __func::target_type", ShowMarker::targetType);

RW_FUNCTION(0x7672F8, "CommonDirection::hide_boss_marker::$_9 __func::~__func (deleting)", HideMarker::deletingDtor);
RW_FUNCTION(0x767300, "CommonDirection::hide_boss_marker::$_9 __func::__clone", HideMarker::clone);
RW_FUNCTION(0x76732C, "CommonDirection::hide_boss_marker::$_9 __func::__clone(dst)", HideMarker::cloneInto);
RW_FUNCTION(0x76734C, "CommonDirection::hide_boss_marker::$_9 __func::destroy", HideMarker::destroy);
RW_FUNCTION(0x76734E, "CommonDirection::hide_boss_marker::$_9 __func::destroy_deallocate", HideMarker::destroyDeallocate);
RW_FUNCTION(0x767368, "CommonDirection::hide_boss_marker::$_9 __func::target", HideMarker::target);
RW_FUNCTION(0x767388, "CommonDirection::hide_boss_marker::$_9 __func::target_type", HideMarker::targetType);

RW_FUNCTION(0x76739C, "CommonDirection::materialPanel_in::$_8 __func::~__func (deleting)", MaterialHide::deletingDtor);
RW_FUNCTION(0x7673A4, "CommonDirection::materialPanel_in::$_8 __func::__clone", MaterialHide::clone);
RW_FUNCTION(0x7673D0, "CommonDirection::materialPanel_in::$_8 __func::__clone(dst)", MaterialHide::cloneInto);
RW_FUNCTION(0x7673F0, "CommonDirection::materialPanel_in::$_8 __func::destroy", MaterialHide::destroy);
RW_FUNCTION(0x7673F2, "CommonDirection::materialPanel_in::$_8 __func::destroy_deallocate", MaterialHide::destroyDeallocate);
RW_FUNCTION(0x767418, "CommonDirection::materialPanel_in::$_8 __func::target", MaterialHide::target);
RW_FUNCTION(0x767438, "CommonDirection::materialPanel_in::$_8 __func::target_type", MaterialHide::targetType);

RW_FUNCTION(0x76744C, "CommonDirection::materialPanel_in::$_7 __func::~__func (deleting)", MaterialOut::deletingDtor);
RW_FUNCTION(0x767454, "CommonDirection::materialPanel_in::$_7 __func::__clone", MaterialOut::clone);
RW_FUNCTION(0x767480, "CommonDirection::materialPanel_in::$_7 __func::__clone(dst)", MaterialOut::cloneInto);
RW_FUNCTION(0x7674A0, "CommonDirection::materialPanel_in::$_7 __func::destroy", MaterialOut::destroy);
RW_FUNCTION(0x7674A2, "CommonDirection::materialPanel_in::$_7 __func::destroy_deallocate", MaterialOut::destroyDeallocate);
RW_FUNCTION(0x7674D0, "CommonDirection::materialPanel_in::$_7 __func::target", MaterialOut::target);
RW_FUNCTION(0x7674F0, "CommonDirection::materialPanel_in::$_7 __func::target_type", MaterialOut::targetType);

RW_FUNCTION(0x767504, "CommonDirection::materialPanel_in::$_6 __func::~__func (deleting)", MaterialStay::deletingDtor);
RW_FUNCTION(0x76750C, "CommonDirection::materialPanel_in::$_6 __func::__clone", MaterialStay::clone);
RW_FUNCTION(0x767538, "CommonDirection::materialPanel_in::$_6 __func::__clone(dst)", MaterialStay::cloneInto);
RW_FUNCTION(0x767558, "CommonDirection::materialPanel_in::$_6 __func::destroy", MaterialStay::destroy);
RW_FUNCTION(0x76755A, "CommonDirection::materialPanel_in::$_6 __func::destroy_deallocate", MaterialStay::destroyDeallocate);
RW_FUNCTION(0x767588, "CommonDirection::materialPanel_in::$_6 __func::target", MaterialStay::target);
RW_FUNCTION(0x7675A8, "CommonDirection::materialPanel_in::$_6 __func::target_type", MaterialStay::targetType);

RW_FUNCTION(0x7675BC, "CommonDirection::panel_update::$_5 __func::~__func (deleting)", PanelMoved::deletingDtor);
RW_FUNCTION(0x7675C4, "CommonDirection::panel_update::$_5 __func::__clone", PanelMoved::clone);
RW_FUNCTION(0x7675F8, "CommonDirection::panel_update::$_5 __func::__clone(dst)", PanelMoved::cloneInto);
RW_FUNCTION(0x767624, "CommonDirection::panel_update::$_5 __func::destroy", PanelMoved::destroy);
RW_FUNCTION(0x767626, "CommonDirection::panel_update::$_5 __func::destroy_deallocate", PanelMoved::destroyDeallocate);
RW_FUNCTION(0x76763C, "CommonDirection::panel_update::$_5 __func::target", PanelMoved::target);
RW_FUNCTION(0x76765C, "CommonDirection::panel_update::$_5 __func::target_type", PanelMoved::targetType);

RW_FUNCTION(0x767670, "CommonDirection::skilPanel_hold::$_4 __func::~__func (deleting)", HoldOver::deletingDtor);
RW_FUNCTION(0x767678, "CommonDirection::skilPanel_hold::$_4 __func::__clone", HoldOver::clone);
RW_FUNCTION(0x7676A8, "CommonDirection::skilPanel_hold::$_4 __func::__clone(dst)", HoldOver::cloneInto);
RW_FUNCTION(0x7676D0, "CommonDirection::skilPanel_hold::$_4 __func::destroy", HoldOver::destroy);
RW_FUNCTION(0x7676D2, "CommonDirection::skilPanel_hold::$_4 __func::destroy_deallocate", HoldOver::destroyDeallocate);
RW_FUNCTION(0x7676EC, "CommonDirection::skilPanel_hold::$_4 __func::target", HoldOver::target);
RW_FUNCTION(0x76770C, "CommonDirection::skilPanel_hold::$_4 __func::target_type", HoldOver::targetType);

RW_FUNCTION(0x767720, "CommonDirection::skilPanel_out_common::$_3 __func::~__func (deleting)", OutDone::deletingDtor);
RW_FUNCTION(0x767728, "CommonDirection::skilPanel_out_common::$_3 __func::__clone", OutDone::clone);
RW_FUNCTION(0x767758, "CommonDirection::skilPanel_out_common::$_3 __func::__clone(dst)", OutDone::cloneInto);
RW_FUNCTION(0x76777C, "CommonDirection::skilPanel_out_common::$_3 __func::destroy", OutDone::destroy);
RW_FUNCTION(0x76777E, "CommonDirection::skilPanel_out_common::$_3 __func::destroy_deallocate", OutDone::destroyDeallocate);
RW_FUNCTION(0x7677DC, "CommonDirection::skilPanel_out_common::$_3 __func::target", OutDone::target);
RW_FUNCTION(0x7677FC, "CommonDirection::skilPanel_out_common::$_3 __func::target_type", OutDone::targetType);

RW_FUNCTION(0x767810, "CommonDirection::skilPanel_in::$_2 __func::~__func (deleting)", InDone::deletingDtor);
RW_FUNCTION(0x767818, "CommonDirection::skilPanel_in::$_2 __func::__clone", InDone::clone);
RW_FUNCTION(0x767848, "CommonDirection::skilPanel_in::$_2 __func::__clone(dst)", InDone::cloneInto);
RW_FUNCTION(0x767870, "CommonDirection::skilPanel_in::$_2 __func::destroy", InDone::destroy);
RW_FUNCTION(0x767872, "CommonDirection::skilPanel_in::$_2 __func::destroy_deallocate", InDone::destroyDeallocate);
RW_FUNCTION(0x7678A4, "CommonDirection::skilPanel_in::$_2 __func::target", InDone::target);
RW_FUNCTION(0x7678C4, "CommonDirection::skilPanel_in::$_2 __func::target_type", InDone::targetType);

RW_FUNCTION(0x7678D8, "CommonDirection::init::$_1 __func::~__func (deleting)", StopUpdate::deletingDtor);
RW_FUNCTION(0x7678E0, "CommonDirection::init::$_1 __func::__clone", StopUpdate::clone);
RW_FUNCTION(0x76790C, "CommonDirection::init::$_1 __func::__clone(dst)", StopUpdate::cloneInto);
RW_FUNCTION(0x76792C, "CommonDirection::init::$_1 __func::destroy", StopUpdate::destroy);
RW_FUNCTION(0x76792E, "CommonDirection::init::$_1 __func::destroy_deallocate", StopUpdate::destroyDeallocate);
RW_FUNCTION(0x767940, "CommonDirection::init::$_1 __func::target", StopUpdate::target);
RW_FUNCTION(0x767960, "CommonDirection::init::$_1 __func::target_type", StopUpdate::targetType);

RW_FUNCTION(0x767974, "CommonDirection::init::$_0 __func::~__func (deleting)", StartUpdate::deletingDtor);
RW_FUNCTION(0x76797C, "CommonDirection::init::$_0 __func::__clone", StartUpdate::clone);
RW_FUNCTION(0x7679A8, "CommonDirection::init::$_0 __func::__clone(dst)", StartUpdate::cloneInto);
RW_FUNCTION(0x7679C8, "CommonDirection::init::$_0 __func::destroy", StartUpdate::destroy);
RW_FUNCTION(0x7679CA, "CommonDirection::init::$_0 __func::destroy_deallocate", StartUpdate::destroyDeallocate);
RW_FUNCTION(0x7679DC, "CommonDirection::init::$_0 __func::target", StartUpdate::target);
RW_FUNCTION(0x7679FC, "CommonDirection::init::$_0 __func::target_type", StartUpdate::targetType);
