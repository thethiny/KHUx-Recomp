// RaidBossActor: the target markers (one BattleMarker per click point of the RaidBoss LWF), the whole-attack point,
// the status indicator and the prize parameters.
#include "RaidBossActor_common.hpp"

using namespace rba;

namespace {
constexpr uint32_t CLICK_POINT_NAMES = 0xFEF0BC;   // const char*[6]: "click_point_0".. per part
constexpr uint32_t VEC2_ANCHOR_MIDDLE = 0xFEF5C0;  // cocos2d::Vec2::ANCHOR_MIDDLE
constexpr uint32_t MARKER_TAP_LAMBDA = 0xF5DE90;   // __func vtable of make_marker's lambda (captures self, part, scene)
constexpr uint32_t MARKER_Z = 300;

uint32_t clickPointName(int32_t part) { return ld32(CLICK_POINT_NAMES + 4u * uint32_t(part)); }

// The RaidBoss's click points (a copy of RaidBoss::getClickPoints) and this node's position, as the original keeps
// them for the whole function. The copy's bucket array holds the address of the map object, so the callers give
// their frame the original's size (pushed registers + sub sp) and the map its original stack slot.
struct ClickPoints {
    uint32_t map, position;
    ClickPoints(RaidBossActor *self, uint32_t mapAt, uint32_t positionAt) : map(mapAt), position(positionAt) {
        uint32_t p = rw::vcall(self, node_slot::GET_POSITION, {addr(self)});
        rw::call(fn::Vec2_copy, {position, p});
        rw::call(fn::clickMap_copy, {map, rw::call(fn::RaidBoss_getClickPoints, {self->boss.addr})});
    }
    void destroy() const {
        rw::call(fn::clickMap_dtor, {map});
        rw::call(fn::Vec2_dtor, {position});
    }
    uint32_t first() const { return ld32(map + 8); }
    float x() const { return rw::load<float>(position); }
    float y() const { return rw::load<float>(position + 4); }
};
}  // namespace

// Move each marker to its click point (field coordinates: y down from this node).
void RaidBossActor::updateTargetPosition(RaidBossActor *self) {
    Frame f(0x58);
    ClickPoints points(self, f.addr(0x24), f.addr(0x38));
    for (uint32_t node = self->markerFirst; node; node = ld32(node)) {
        uint32_t name = clickPointName(ldi(node + 8));
        uint32_t len = rw::call(fn::strlen_, {name});
        uint32_t str = f.addr(0x18);
        rw::call(fn::string_init, {str, name, len});
        uint32_t found = rw::call(fn::clickMap_find, {points.map, str});
        if (ld8(str) & 1) guest::deleteObject(ld32(str + 8));
        if (!found) continue;
        uint32_t v = f.addr(0x10);
        rw::call(fn::Vec2_copy, {v, found + 20});
        rw::save<float>(v, points.x() + rw::load<float>(v));
        rw::save<float>(v + 4, points.y() - rw::load<float>(v + 4));
        uint32_t marker = ld32(node + 12);
        rw::vcall(rw::host_ptr<void>(marker), node_slot::SET_POSITION, {marker, v});
        rw::call(fn::Vec2_dtor, {v});
    }
    points.destroy();
}
RW_FUNCTION(0x5F34A0, "RaidBossActor::updateTargetPosition", RaidBossActor::updateTargetPosition);

// The marker of a part (0 = none).
uint32_t RaidBossActor::getGuage(RaidBossActor *self, int32_t part) {
    Frame f(0x10);
    uint32_t key = f.addr(8);
    rw::save<int32_t>(key, part);
    uint32_t node = findMarkerNode(self, key);
    return node ? ld32(node + 12) : 0;
}
RW_FUNCTION(0x5F35E8, "RaidBossActor::getGuage", RaidBossActor::getGuage);

// Show the markers of the live parts (hide the broken ones); `select` also shows the "select" caption.
void RaidBossActor::ShowTarget(RaidBossActor *self, uint32_t select) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node)) {
        uint32_t marker = ld32(node + 12);
        if (ld8(self->boss.addr + uint32_t(ldi(node + 8)) + BOSS_POINT_ENABLED)) {
            rw::call(fn::BattleMarker_hide, {marker});
        } else {
            rw::call(fn::BattleMarker_show_wait, {marker});
            if (select == 1) rw::call(fn::BattleMarker_disp_select_string, {ld32(node + 12)});
        }
    }
}
RW_FUNCTION(0x5F3640, "RaidBossActor::ShowTarget", RaidBossActor::ShowTarget);

// The body marker (part 0) only: select / selected (random) / back to waiting.
void RaidBossActor::ReselectTarget(RaidBossActor *self) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node))
        if (!ld32(node + 8)) { rw::call(fn::BattleMarker_show_select, {ld32(node + 12)}); return; }
}
RW_FUNCTION(0x5F3688, "RaidBossActor::ReselectTarget", RaidBossActor::ReselectTarget);

void RaidBossActor::SelectedTarget(RaidBossActor *self) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node))
        if (!ld32(node + 8)) { rw::call(fn::BattleMarker_show_selected_random, {ld32(node + 12)}); return; }
}
RW_FUNCTION(0x5F36A8, "RaidBossActor::SelectedTarget", RaidBossActor::SelectedTarget);

void RaidBossActor::CancelTarget(RaidBossActor *self) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node))
        if (!ld32(node + 8)) { rw::call(fn::BattleMarker_show_wait, {ld32(node + 12)}); return; }
}
RW_FUNCTION(0x5F36C8, "RaidBossActor::CancelTarget", RaidBossActor::CancelTarget);

// One part's marker: selected, or hidden when the part is broken.
void RaidBossActor::ShowTargetInex(RaidBossActor *self, int32_t part) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node)) {
        if (ldi(node + 8) != part) continue;
        uint8_t broken = ld8(self->boss.addr + uint32_t(part) + BOSS_POINT_ENABLED);
        uint32_t marker = ld32(node + 12);
        rw::call(broken ? fn::BattleMarker_hide : fn::BattleMarker_show_selected, {marker});
        return;
    }
}
RW_FUNCTION(0x5F36E8, "RaidBossActor::ShowTargetInex", RaidBossActor::ShowTargetInex);

void RaidBossActor::HideTargetInex(RaidBossActor *self, int32_t part) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node))
        if (ldi(node + 8) == part) { rw::call(fn::BattleMarker_hide, {ld32(node + 12)}); return; }
}
RW_FUNCTION(0x5F3724, "RaidBossActor::HideTargetInex", RaidBossActor::HideTargetInex);

// Every marker hides completely (not just its ring) from now on.
void RaidBossActor::marker_mode_chamge(RaidBossActor *self) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node))
        rw::save<uint8_t>(ld32(node + 12) + MARKER_RING_ONLY, 0);
}
RW_FUNCTION(0x5F3744, "RaidBossActor::marker_mode_chamge", RaidBossActor::marker_mode_chamge);

void RaidBossActor::HideTarget(RaidBossActor *self) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node))
        rw::call(fn::BattleMarker_hide, {ld32(node + 12)});
}
RW_FUNCTION(0x5F3760, "RaidBossActor::HideTarget", RaidBossActor::HideTarget);

void RaidBossActor::HideTargetNoSelected(RaidBossActor *self, int32_t part) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node))
        if (ldi(node + 8) != part) rw::call(fn::BattleMarker_hide, {ld32(node + 12)});
}
RW_FUNCTION(0x5F377C, "RaidBossActor::HideTargetNoSelected", RaidBossActor::HideTargetNoSelected);

// How many of the parts 1..5 are not broken.
int32_t RaidBossActor::get_active_parts(RaidBossActor *self) {
    int32_t n = 0;
    for (uint32_t k = 1; k <= 5; k++) n += ld8(self->boss.addr + BOSS_POINT_ENABLED + k) ^ 1;
    return n;
}
RW_FUNCTION(0x5F37A4, "RaidBossActor::get_active_parts", RaidBossActor::get_active_parts);

// Create a marker for each click point of the boss (type from the part, HP gauge of the part's max HP), placed at
// the click point, tappable (the lambda selects the part in the scene), and hidden; the whole-attack point is the
// mean of the marker positions.
void RaidBossActor::make_marker(RaidBossActor *self, uint32_t scene, uint32_t parent, float scale) {
    uint32_t a = addr(self);
    Frame f(0xB8);
    ClickPoints points(self, f.addr(0x54), f.addr(0x68));
    self->targetPart = -1;
    self->field_924 = -1;
    for (uint32_t node = points.first(); node; node = ld32(node)) {
        uint32_t name = node + 8;
        float px = rw::load<float>(node + 20), py = rw::load<float>(node + 24);
        for (int32_t part = 0; part < 6; part++) {
            if (rw::call(fn::string_eq_cstr, {name, clickPointName(part)}) != 0) continue;
            int32_t type = 3;
            if (part) type = partGet(self, part, RaidParts::TYPE) - 1;
            int32_t maxHp = partGet(self, part, RaidParts::MAX_HP);
            uint32_t marker = rw::call(fn::BattleMarker_create, {uint32_t(type), uint32_t(maxHp)});
            void *m = rw::host_ptr<void>(marker);
            rw::vcall(m, node_slot::SET_ANCHOR_POINT, {marker, VEC2_ANCHOR_MIDDLE});
            rw::call(fn::BattleMarker_set_scene, {marker, scene});
            float x = px + points.x();
            float y = points.y() - py;
            if (part) {
                rw::call(fn::BattleMarker_show, {marker, rw::fbits(x), rw::fbits(y), 1});
            } else {
                rw::call(fn::BattleMarker_show, {marker, rw::fbits(x), rw::fbits(y), 0});
                uint32_t pos = rw::vcall(m, node_slot::GET_POSITION, {marker});
                rw::save<uint32_t>(a + 904, ld32(pos));
                rw::save<uint32_t>(a + 908, ld32(pos + 4));
            }
            rw::vcall(m, node_slot::SET_SCALE, {marker, rw::fbits(scale)});
            rw::vcall(rw::host_ptr<void>(parent), node_slot::ADD_CHILD_Z, {parent, marker, MARKER_Z});
            // marker->onTap = [self, part, scene](BattleMarker*) {...}
            uint32_t tap = f.addr(0x80);
            rw::save<uint32_t>(tap, MARKER_TAP_LAMBDA);
            rw::save<uint32_t>(tap + 4, a);
            rw::save<int32_t>(tap + 8, part);
            rw::save<uint32_t>(tap + 12, scene);
            rw::save<uint32_t>(tap + 16, tap);
            rw::call(fn::markerFunction_swap, {tap, marker + MARKER_ON_TAP});
            uint32_t held = ld32(tap + 16);
            if (held == tap) rw::call(ld32(ld32(tap) + 0x10), {tap});
            else if (held) rw::call(ld32(ld32(held) + 0x14), {held});
            rw::call(fn::Vec2_add, {a + 912, rw::vcall(m, node_slot::GET_POSITION, {marker})});
            uint32_t pair = f.addr(0x4C);
            rw::save<int32_t>(pair, part);
            rw::save<uint32_t>(pair + 4, marker);
            rw::call(fn::markers_insert, {f.addr(0x78), a + MARKERS, pair});
            rw::call(fn::BattleMarker_hide, {marker});
        }
    }
    rw::call(fn::Vec2_scale, {a + 912, rw::fbits(1.0f / float(uint32_t(self->markerCount)))});
    points.destroy();
}
RW_FUNCTION(0x5F37D0, "RaidBossActor::make_marker", RaidBossActor::make_marker);

// The mean position of the live parts' click points.
void RaidBossActor::makeWholeAttackPoint(RaidBossActor *self) {
    uint32_t a = addr(self);
    Frame f(0x68);
    ClickPoints points(self, f.addr(0x30), f.addr(0x44));
    self->wholeAttackPoint.x = 0.0f;
    self->wholeAttackPoint.y = 0.0f;
    int32_t count = 0;
    for (uint32_t node = points.first(); node; node = ld32(node)) {
        float px = rw::load<float>(node + 20), py = rw::load<float>(node + 24);
        for (int32_t part = 0; part < 6; part++) {
            if (rw::call(fn::string_eq_cstr, {node + 8, clickPointName(part)}) != 0) continue;
            if (partGet(self, part, RaidParts::HP)) {
                float x = px + points.x();
                self->wholeAttackPoint.x = self->wholeAttackPoint.x + x;
                float y = points.y() - py;
                self->wholeAttackPoint.y = self->wholeAttackPoint.y + y;
                count++;
            }
            break;
        }
    }
    if (count) rw::call(fn::Vec2_scale, {a + 912, rw::fbits(1.0f / float(count))});
    points.destroy();
}
RW_FUNCTION(0x5F3B64, "RaidBossActor::makeWholeAttackPoint", RaidBossActor::makeWholeAttackPoint);

// HP of the targeted part (the body when none).
int32_t RaidBossActor::getEnemyHp(RaidBossActor *self) {
    Frame f(0x18);
    uint32_t key = f.addr(8);
    int32_t target = self->targetPart;
    rw::save<int32_t>(key, target >= 0 ? target : 0);
    uint32_t node = findMarkerNode(self, key);
    if (!node) return 0;
    return partGet(self, ldi(node + 8), RaidParts::HP);
}
RW_FUNCTION(0x5F3CE8, "RaidBossActor::getEnemyHp", RaidBossActor::getEnemyHp);

// The position of a part's marker ((0, 0) when there is none).
void RaidBossActor::find_marker(const bt::Vec2 *out, RaidBossActor *self, int32_t part) {
    Frame f(0x10);
    uint32_t key = f.addr(8);
    rw::save<int32_t>(key, part);
    uint32_t node = findMarkerNode(self, key);
    if (node) {
        uint32_t marker = ld32(node + 12);
        rw::call(fn::Vec2_copy, {addr(out), rw::vcall(rw::host_ptr<void>(marker), node_slot::GET_POSITION, {marker})});
    } else {
        rw::call(fn::Vec2_ctor_ff, {addr(out), 0, 0});
    }
}
RW_FUNCTION(0x5F3D78, "RaidBossActor::find_marker", RaidBossActor::find_marker);

// The boss's large HP gauge (HP per box from master::Misc 203), retained, at the top centre (x = width / 2 - 84).
uint32_t RaidBossActor::createRaidBossStatusField(RaidBossActor *self, uint32_t name, uint32_t hp, uint32_t hpMax,
                                                  int32_t level, int32_t type) {
    Frame f(0x40);
    uint32_t misc = f.addr(0x24);
    rw::call(fn::master_Misc_create, {misc, 203});
    uint32_t hpPerBox = getu(ld32(misc), 4);
    uint32_t indicator = rw::call(fn::RaidBossStatusIndicator_create,
                                  {name, hp, hpMax, uint32_t(level), uint32_t(type), hpPerBox});
    releaseShared(ld32(misc + 4));
    rw::call(fn::Ref_retain, {indicator});
    uint32_t size = f.addr(0x1C);
    rw::call(fn::Director_getVisibleSize, {size, rw::call(fn::Director_getInstance, {})});
    float x = rw::load<float>(size) * 0.5f + -84.0f;
    uint32_t setPosition = ld32(ld32(indicator) + 4 * node_slot::SET_POSITION);
    uint32_t pos = f.addr(0x14);
    rw::call(fn::Vec2_ctor_ff, {pos, rw::fbits(x), 0});
    rw::call(setPosition, {indicator, pos});
    rw::call(fn::Vec2_dtor, {pos});
    self->statusIndicator = indicator;
    return indicator;
}
RW_FUNCTION(0x5F3DF0, "RaidBossActor::createRaidBossStatusField", RaidBossActor::createRaidBossStatusField);

// A part's two prize rates, and the skill whose prize bonuses apply (one for the whole boss).
void RaidBossActor::setPrizeParam(RaidBossActor *self, int32_t part, float rateA, float rateB,
                                  const guest::SharedPtr *skill) {
    uint32_t p = addr(self) + 1988 + 32u * uint32_t(part);
    set32(p, PrizeParam::RATE_A, int32_t(rw::fbits(rateA)));
    set32(p, PrizeParam::RATE_B, int32_t(rw::fbits(rateB)));
    uint32_t obj = skill->obj, ctrl = skill->ctrl;
    addRef(ctrl);
    self->skill.obj = obj;
    uint32_t old = self->skill.ctrl;
    self->skill.ctrl = ctrl;
    releaseShared(old);
}
RW_FUNCTION(0x5F3EF8, "RaidBossActor::setPrizeParam", RaidBossActor::setPrizeParam);
