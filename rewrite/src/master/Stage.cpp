#include "Stage.hpp"
#include "ParseUtilD.hpp"

namespace master {

using StageFunctions = TableFunctions<Stage>;
RW_FUNCTION(0x78e4cc, "master::Stage::getStageIds", StageFunctions::getIds);
RW_FUNCTION(0x78e4e8, "master::Stage::create", StageFunctions::create);
RW_FUNCTION(0x78f92c, "master::Stage::getHash", StageFunctions::getHash);
RW_FUNCTION(0x78fa18, "master::Stage::getID", StageFunctions::getID);
RW_FUNCTION(0x78fa64, "master::Stage::~Stage (deleting)", StageFunctions::destructDelete);

void Stage::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xdfaef5, Kind::Int, STAGE_ID, 0x4, 0xd8},                         // "stageId"
        {0xdfa935, Kind::Int, ID, 0x4, 0xd4},                               // "id"
        {0xdfade6, Kind::String, NAME, 0x81, 0xdc},                         // "name"
        {0xe09680, Kind::String, MAP_NAME, 0x81, 0xdc},                     // "mapName"
        {0xdfaf47, Kind::Int, USE_AP, 0x4, 0xd0},                           // "useAp"
        {0xe08488, Kind::Int, CHAPTER_ID, 0x4, 0xcc},                       // "chapterId"
        {0xe07c36, Kind::Int, WORLD_ID, 0x4, 0xc8},                         // "worldId"
        {0xe08f2c, Kind::Int, THUMB_ID, 0x4, 0xc4},                         // "thumbId"
        {0xe09688, Kind::Int, STAGE_KIND, 0x4, 0xc0},                       // "stageKind"
        {0xdfb48b, Kind::Int, SHOW_ICON, 0x4, 0xbc},                        // "showIcon"
        {0xe09692, Kind::Int, VALID_BEFORE_DRAMA, 0x4, 0xb8},               // "validBeforeDrama"
        {0xe096a3, Kind::IntArray, BEFORE_DRAMA_ID, 0x14, 0xdc},            // "beforeDramaId"
        {0xe096b1, Kind::IntArray, BEFORE_DRAMA_TYPE, 0x14, 0xdc},          // "beforeDramaType"
        {0xe096c1, Kind::Int, VALID_AFTER_DRAMA, 0x4, 0xb4},                // "validAfterDrama"
        {0xe096d1, Kind::IntArray, AFTER_DRAMA_ID, 0x14, 0xdc},             // "afterDramaId"
        {0xe096de, Kind::IntArray, AFTER_DRAMA_TYPE, 0x14, 0xdc},           // "afterDramaType"
        {0xe096ed, Kind::Int, VALID_CLEAR_GET_TITLE, 0x4, 0xb0},            // "validClearGetTitle"
        {0xe09700, Kind::Int, CLEAR_GET_TITLE, 0x4, 0xac},                  // "clearGetTitle"
        {0xe0970e, Kind::Int, VALID_CLEAR_GET_SPHERE, 0x4, 0xa8},           // "validClearGetSphere"
        {0xe09722, Kind::Int, CLEAR_GET_SPHERE, 0x4, 0xa4},                 // "clearGetSphere"
        {0xe09731, Kind::Int, RESET_CLEAR_GET, 0x4, 0xa0},                  // "resetClearGet"
        {0xe0973f, Kind::Int, VALID_CLEAR_GET_ITEM, 0x4, 0x9c},             // "validClearGetItem"
        {0xe09751, Kind::IntArray, CLEAR_GET_ITEM_TYPE, 0xc, 0xdc},         // "clearGetItemType"
        {0xe09762, Kind::IntArray, CLEAR_GET_ITEM_ID, 0xc, 0xdc},           // "clearGetItemId"
        {0xe09771, Kind::IntArray, CLEAR_GET_ASSIGN_SKILL_TYPE, 0xc, 0xdc}, // "clearGetAssignSkillType"
        {0xe09789, Kind::IntArray, CLEAR_GET_ASSIGN_SKILL_ID, 0xc, 0xdc},   // "clearGetAssignSkillId"
        {0xe0979f, Kind::IntArray, CLEAR_GET_ASSIGN_SKILL_LV, 0xc, 0xdc},   // "clearGetAssignSkillLv"
        {0xe097b5, Kind::IntArray, CLEAR_GET_ITEM_NUM, 0xc, 0xdc},          // "clearGetItemNum"
        {0xe097c5, Kind::Int, RESET_SUB_MISSION, 0x4, 0x98},                // "resetSubMission"
        {0xe097d5, Kind::Int, VALID_SUBMISSION, 0x4, 0x94},                 // "validSubmission"
        {0xe097e5, Kind::IntArray, SUBMISSION_REQUIRE, 0xc, 0xdc},          // "submissionRequire"
        {0xe07d34, Kind::StringArray, SUBMISSION_NAME, 0x183, 0xdc, 0x81},  // "submissionName"
        {0xe097f7, Kind::IntArray, SUBMISSION_DATA_TYPE, 0xc, 0xdc},        // "submissionDataType"
        {0xe0980a, Kind::IntArray, SUBMISSION_ID_TYPE, 0xc, 0xdc},          // "submissionIdType"
        {0xe0981b, Kind::IntArray, SUBMISSION_ID, 0xc, 0xdc},               // "submissionId"
        {0xe09828, Kind::IntArray, SUBMISSION_NUM, 0xc, 0xdc},              // "submissionNum"
        {0xe09836, Kind::IntArray, SUBMISSION_REWARD_TYPE, 0xc, 0xdc},      // "submissionRewardType"
        {0xe0984b, Kind::IntArray, SUBMISSION_ITEM_ID, 0xc, 0xdc},          // "submissionItemId"
        {0xe0985c, Kind::IntArray, SUBMISSION_SKILL_TYPE, 0xc, 0xdc},       // "submissionSkillType"
        {0xe09870, Kind::IntArray, SUBMISSION_SKILL_ID, 0xc, 0xdc},         // "submissionSkillId"
        {0xe09882, Kind::IntArray, SUBMISSION_SKILL_LV, 0xc, 0xdc},         // "submissionSkillLv"
        {0xe09894, Kind::IntArray, SUBMISSION_ITEM_NUM, 0xc, 0xdc},         // "submissionItemNum"
        {0xe098a6, Kind::Int, RAID_BOSS, 0x4, 0x90},                        // "raidBoss"
        {0xe098af, Kind::Int, BGM_FIELD, 0x4, 0x8c},                        // "bgmField"
        {0xe098b8, Kind::Int, BGM_BATTLE, 0x4, 0x88},                       // "bgmBattle"
        {0xe098c2, Kind::Int, BGM_BOSS, 0x4, 0x84},                         // "bgmBoss"
    };
    parse_d::parseRow(out, json, {0x278, 0x3fc, PARSE_VTABLE, 0x78f8d9}, FIELDS);
}
RW_FUNCTION(0x78e67c, "master::Stage::parse", Stage::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
