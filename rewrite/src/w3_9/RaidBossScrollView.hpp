// RaidBossScrollView: the raid list of the raid list scene (SceneRaidList): one Adv_Raid_Boss_Block row per raid
// the player can join (the boss card with its level, AP, HP and time left, and who found it). A
// BaseScrollView<int> whose item ids index the raid copies it keeps (std::vector<RaidBase>, 240 bytes each, from
// APIManager +340). Tapping a row calls onSelect(id); holding one for 0.8 s opens the raid boss info popup.
#pragma once

#include "BaseScrollView.hpp"

struct RaidBossScrollView : BaseScrollView {
    // +0x638 std::function<void(int)> onSelect (callable at +0x648), +0x650 a second std::function (callable at
    // +0x660), +0x668 a row was pressed, +0x670 the pressed row's item, +0x678 the press lasts, +0x679 the long
    // press fired, +0x67C its timer, +0x680 shared_ptr<AvatarIconCache>, +0x688 std::vector<RaidBase>
    const uint8_t members[0x698 - 0x634];

    static constexpr uint32_t SIZE = 0x698;
    static constexpr uint32_t VTABLE = 0xF82B8C, VTABLE2 = 0xF82EE4, VTABLE3 = 0xF82EF8;
    static constexpr uint32_t ON_SELECT = 0x638;
    static constexpr uint32_t FUNCTION_2 = 0x650;
    static constexpr uint32_t PRESSED = 0x668;
    static constexpr uint32_t F_66C = 0x66C;
    static constexpr uint32_t PRESSED_ITEM = 0x670;
    static constexpr uint32_t F_674 = 0x674;
    static constexpr uint32_t HOLDING = 0x678;
    static constexpr uint32_t LONG_PRESSED = 0x679;
    static constexpr uint32_t HOLD_TIME = 0x67C;
    static constexpr uint32_t ICON_CACHE = 0x680;    // shared_ptr<AvatarIconCache>
    static constexpr uint32_t RAIDS = 0x688;         // std::vector<RaidBase>
    static constexpr uint32_t RAID_SIZE = 240;
    static constexpr uint32_t RAID_FINDER = 168;     // RaidBase: the discoverer (RaidBase::User)

    static Addr create();
    static RaidBossScrollView *construct(RaidBossScrollView *self);
    static RaidBossScrollView *destroy(RaidBossScrollView *self);
    static uint32_t init(RaidBossScrollView *self);
    static void update(RaidBossScrollView *self, float dt);
    static void updateScroll(RaidBossScrollView *self, float delta);
    static Addr createRow(RaidBossScrollView *self, uint32_t index, int32_t arg);
    static uint32_t changeRow(RaidBossScrollView *self, Addr widget, uint32_t index, int32_t arg);
    static void registItemTouchEvent(RaidBossScrollView *self, Addr widget, int32_t index);
    static void setRaidBossFrame(RaidBossScrollView *self, Addr widget, uint32_t raid);
    static uint32_t setBossFrame(Addr widget, Addr info);
    static void setUserFrame(Addr out, Addr widget, Addr user, Addr cache);
};
RW_SIZE(RaidBossScrollView, RaidBossScrollView::SIZE);
