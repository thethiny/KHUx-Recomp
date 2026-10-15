#include "RaidSetting.hpp"
#include "ParseUtilB.hpp"

namespace master {

using RaidSettingFunctions = TableFunctions<RaidSetting>;
RW_FUNCTION(0x78a610, "master::RaidSetting::getRaidSettingIds", RaidSettingFunctions::getIds);
RW_FUNCTION(0x78a62c, "master::RaidSetting::create", RaidSettingFunctions::create);
RW_FUNCTION(0x78ac58, "master::RaidSetting::getHash", RaidSettingFunctions::getHash);
RW_FUNCTION(0x78ad44, "master::RaidSetting::getID", RaidSettingFunctions::getID);
RW_FUNCTION(0x78ad90, "master::RaidSetting::~RaidSetting (deleting)", RaidSettingFunctions::destructDelete);

void RaidSetting::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe092c6, Kind::Int, RAID_SETTING_ID, 0x4, 0x70},              // "raidSettingId"
        {0xe092d4, Kind::Int, BOSS_ID, 0x4, 0x6c},                      // "bossId"
        {0xe092db, Kind::Int, BOSS_UP_TIME, 0x4, 0x68},                 // "bossUpTime"
        {0xdfaf47, Kind::Int, USE_AP, 0x4, 0x64},                       // "useAp"
        {0xe092e6, Kind::Int, RARE_BOSS_ID, 0x4, 0x60},                 // "rareBossId"
        {0xe092f1, Kind::Int, RARE_BOSS_UP_TIME, 0x4, 0x5c},            // "rareBossUpTime"
        {0xe09300, Kind::Int, RARE_USE_AP, 0x4, 0x58},                  // "rareUseAp"
        {0xe0930a, Kind::Int, RARE_UP_TYPE, 0x4, 0x54},                 // "rareUpType"
        {0xe09315, Kind::Int, RARE_UP_TYPE_ARG, 0x4, 0x50},             // "rareUpTypeArg"
        {0xe09323, Kind::Int, RARE_UP_TRIGGER_BOSS_ID, 0x4, 0x4c},      // "rareUpTriggerBossId"
        {0xe09337, Kind::Int, VALID_UNLOCK_RARE, 0x4, 0x48},            // "validUnlockRare"
        {0xe09347, Kind::IntArray, UNLOCK_RARE_BOSS_ID, 0xc, 0x74},     // "unlockRareBossId"
        {0xe09358, Kind::IntArray, UNLOCK_RARE_NUM, 0xc, 0x74},         // "unlockRareNum"
    };
    parse_b::parseRow(out, json, {0x98, 0x44, PARSE_VTABLE, 0x78ac05}, FIELDS);
}
RW_FUNCTION(0x78a7c0, "master::RaidSetting::parse", RaidSetting::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
