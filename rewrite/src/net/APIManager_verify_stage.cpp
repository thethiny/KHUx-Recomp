// APIManager::verifyResponse: quests (stage), raids and the colosseum.
#include "APIManager_verify.hpp"

namespace apiv {

namespace {

constexpr uint32_t SAFEBUFFER_WRITE = 0x459FA1;            // SafeBuffer::write(dst, src, n, index)
constexpr uint32_t STRING_INIT = 0x47CD93;                 // std::string::__init(const char*, size_t)
constexpr uint32_t RAID_VECTOR_DTOR = 0x4BA87D;            // __vector_base<RaidBase>::~__vector_base
constexpr uint32_t RAID_VECTOR_DEALLOCATE = 0x4D5539;
constexpr uint32_t RAID_VECTOR_PUSH_SLOW = 0x4D7129;       // __push_back_slow_path<const RaidBase&>

constexpr uint32_t S_SUPPORT_USERS = 0xDFBC2A;             // "supportUsers"
constexpr uint32_t S_EVENT_CATEGORY = 0xDFBC4F;            // "eventCategory"
constexpr uint32_t S_RAIDS = 0xDFC0C7;                     // "raids"
constexpr uint32_t S_GET_REWARD_RANK_IDS = 0xDFC07C;       // "getRewardRankIds"
constexpr uint32_t EV_UPGRADE_QUEST = 0xDFBC37;            // "UpgradeQuest"
constexpr uint32_t EV_EVENT_QUEST = 0xDFBC44;              // "EventQuest"
constexpr uint32_t EV_QUESTS_COMPLETE_6 = 0xDFBC5D;
constexpr uint32_t EV_QUESTS_COMPLETE_34 = 0xDFBC6D;
constexpr uint32_t EV_QUESTS_COMPLETE_130 = 0xDFBC7E;
constexpr uint32_t EV_QUESTS_COMPLETE_200 = 0xDFBC90;
constexpr uint32_t EV_BOSS_HELP = 0xDFBCA2;                // "BossHelp"
constexpr uint32_t EV_COLISEUM_STAGE = 0xDFBCDA;           // "ColiseumStage"

// the stage request (APIManager::StageRequestData, a SafeBuffer): what the running quest was started with
constexpr uint32_t REQ_ACTION = 0x00;                      // i32
constexpr uint32_t REQ_STAGE_ID = 0x04;                    // i32
constexpr uint32_t REQ_SUPPORT_USER_ID = 0x08;             // u64
constexpr uint32_t REQ_KEYBLADE_ID = 0x10;                 // u64
// StageStart / RaidStart / ColosseumStart fields
constexpr uint32_t START_STAGE_ID = 0x30;                  // StageStart, ColosseumStart: i32
constexpr uint32_t START_SUPPORT_USER_ID = 0x38;           // StageStart, RaidStart: u64
constexpr uint32_t START_KEYBLADE_ID = 0x40;               // StageStart, RaidStart: u64
constexpr uint32_t COLOSSEUM_KEYBLADE_ID = 0x38;           // ColosseumStart: u64
constexpr uint32_t RAID_OWNER_ID = 0x60;                   // RaidStart: u64, the user whose boss it is
// StageEvent: std::vector<Event> (56 bytes each) at +0; Event: std::vector<stage entry> (24 bytes, stage id at +0)
constexpr uint32_t EVENT_SIZE = 0x38;
constexpr uint32_t EVENT_STAGE_SIZE = 0x18;
constexpr uint8_t CATEGORY_UPGRADE = 1;
constexpr uint8_t CATEGORY_EVENT = 2;
constexpr uint32_t MASTER_STAGE_NUMBER = 4;                // master::Stage data position 4: the quest number

// StageClear: the record lists it carries (shared_ptrs)
constexpr uint32_t STAGE_CLEAR_MATERIALS = 0x24;
constexpr uint32_t STAGE_CLEAR_MEDALS = 0x2C;
constexpr uint32_t STAGE_CLEAR_SKILLS = 0x34;
constexpr uint32_t STAGE_CLEAR_TITLES = 0x3C;
constexpr uint32_t STAGE_CLEAR_KEYBLADES = 0x44;
// RaidClear
constexpr uint32_t RAID_CLEAR_SKILLS = 0x08;
constexpr uint32_t RAID_CLEAR_TITLES = 0x10;
constexpr uint32_t RAID_CLEAR_KEYBLADES = 0x18;
// ColosseumClear
constexpr uint32_t COLOSSEUM_CLEAR_MATERIALS = 0x0C;
constexpr uint32_t COLOSSEUM_CLEAR_MEDALS = 0x14;
constexpr uint32_t COLOSSEUM_CLEAR_SKILLS = 0x1C;
constexpr uint32_t COLOSSEUM_CLEAR_TITLES = 0x24;
constexpr uint32_t COLOSSEUM_CLEAR_KEYBLADES = 0x2C;
// RaidReward: std::vector<entry> at +0; RaidReward151101 likewise; the record lists per entry
constexpr uint32_t RAID_REWARD_SIZE = 0x148;
constexpr uint32_t RAID_REWARD_MEDALS = 0x130;
constexpr uint32_t RAID_REWARD_SKILLS = 0x138;
constexpr uint32_t RAID_REWARD_MATERIALS = 0x140;
constexpr uint32_t RAID_REWARD_151101_SIZE = 0xB0;
constexpr uint32_t RAID_REWARD_151101_MEDALS = 0x98;
constexpr uint32_t RAID_REWARD_151101_SKILLS = 0xA0;
constexpr uint32_t RAID_REWARD_151101_MATERIALS = 0xA8;
// RaidBase (240 bytes): POD up to +0xB0, a std::string at +0xB0, POD after it
constexpr uint32_t RAID_BASE_SIZE = 0xF0;

// SafeBuffer::write into the manager's stage request at a data position
void writeStageRequest(const Ctx &c, uint32_t pos, uint32_t src, uint32_t n) {
    const uint32_t request = addr(&c.self->stageRequest);
    c.F.call(SAFEBUFFER_WRITE, {request, load32(request + 12) + pos, src, n, pos});
}

}  // namespace

uint32_t verifyStage(const Ctx &c) { return parseAndKeep(c, STAGE_PARSE, 0x7C0, off::STAGE); }
uint32_t verifyStage160310(const Ctx &c) { return parseAndKeep(c, STAGE_160310_PARSE, 0x7B8, off::STAGE_160310); }

// stage/event: the events; the request's eventCategory is remembered for stage/start
uint32_t verifyStageEvent(const Ctx &c) {
    if (!parse(c, STAGE_EVENT_PARSE, 0x7B0)) {
        drop(c, 0x7B0);
        return 0;
    }
    const uint32_t category = jsonMember(c, c.self->params.ptr, S_EVENT_CATEGORY);
    if (jsonFlags(category) & JSON_FLAG_INT) rw::set<uint8_t>(c.self, off::EVENT_CATEGORY, uint8_t(load32(category)));
    keep(c, 0x7B0, off::STAGE_EVENT);
    drop(c, 0x7B0);
    return 1;
}

uint32_t verifyStageSupportList(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, STAGE_SUPPORT_LIST_PARSE, 0x7A8, S_SUPPORT_USERS)) {
        keep(c, 0x7A8, off::SUPPORT_LIST);
        ok = 1;
    }
    drop(c, 0x7A8);
    return ok;
}

// stage/start: points, the start reply and the supporters; the quest's request data is recorded and its keyblade
// becomes the favorite. A stage listed by one of the cached events sends "UpgradeQuest" / "EventQuest".
uint32_t verifyStageStart(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x7D8)) {
        if (parse(c, STAGE_START_PARSE, 0x7D0)) {
            if (parse(c, STAGE_SUPPORT_LIST_PARSE, 0x7C8, S_SUPPORT_USERS)) {
                keep(c, 0x7D8, off::USER_POINT);
                keep(c, 0x7D0, off::STAGE_START);
                keep(c, 0x7C8, off::SUPPORT_LIST);
                const uint32_t start = member(c.self, off::STAGE_START).ptr;
                writeStageRequest(c, REQ_ACTION, c.F.at(0xC58), 4);
                writeStageRequest(c, REQ_STAGE_ID, start + START_STAGE_ID, 4);
                writeStageRequest(c, REQ_KEYBLADE_ID, start + START_KEYBLADE_ID, 8);
                writeStageRequest(c, REQ_SUPPORT_USER_ID, start + START_SUPPORT_USER_ID, 8);
                c.F.call(UPDATE_FAVORITE_KEYBLADE, {addr(c.self), 0, load32(start + START_KEYBLADE_ID),
                                                    load32(start + START_KEYBLADE_ID + 4)});
                ok = 1;
                const uint32_t events = member(c.self, off::STAGE_EVENT).ptr;
                if (events) {
                    uint32_t e = load32(events);
                    const uint32_t last = load32(events + 4);
                    for (; e != last; e += EVENT_SIZE) {
                        uint32_t s = load32(e);
                        const uint32_t stagesEnd = load32(e + 4);
                        if (s != stagesEnd) {
                            const uint32_t stageId = load32(start + START_STAGE_ID);
                            while (load32(s) != stageId && (s += EVENT_STAGE_SIZE) != stagesEnd) {
                            }
                        }
                        if (s != stagesEnd) break;
                    }
                    if (e != last) {
                        const uint8_t category = rw::get<uint8_t>(c.self, off::EVENT_CATEGORY);
                        if (category == CATEGORY_UPGRADE) sendEventOnce(c, EV_UPGRADE_QUEST);
                        else if (category == CATEGORY_EVENT) sendEventOnce(c, EV_EVENT_QUEST);
                    }
                }
            }
            drop(c, 0x7C8);
        }
        drop(c, 0x7D0);
    }
    drop(c, 0x7D8);
    return ok;
}

// stage/retire, colosseum/retire: the keyblades (applied) and the resumption state
uint32_t verifyRetire(const Ctx &c) {
    if (!parse(c, USER_KEYBLADE_PARSE, 0x790)) {
        drop(c, 0x790);
        return 0;
    }
    uint32_t ok = 0;
    if (parse(c, STAGE_RESUMPTION_PARSE, 0x788)) {
        keep(c, 0x788, off::STAGE_RESUMPTION);
        applyRecords(c, 0x790, KEYBLADE_SIZE, 0x12F8, UPDATE_KEYBLADE);
        ok = 1;
    }
    drop(c, 0x788);
    drop(c, 0x790);
    return ok;
}

// stage/continue: stones and the keyblades (applied)
uint32_t verifyStageContinue(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_STONE_PARSE, 0x7A0)) {
        if (parse(c, USER_KEYBLADE_PARSE, 0x798)) {
            keep(c, 0x7A0, off::USER_STONE);
            applyRecords(c, 0x798, KEYBLADE_SIZE, 0x1350, UPDATE_KEYBLADE);
            ok = 1;
        }
        drop(c, 0x798);
    }
    drop(c, 0x7A0);
    return ok;
}

// stage/clear: points, detail, stones, the clear result (its items applied) and the resumption state. The quest's
// number sends the "QuestsComplete" milestones, the new level the "AvatarLevel" ones.
uint32_t verifyStageClear(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x780)) {
        if (parse(c, USER_DETAIL_PARSE, 0x778)) {
            if (parse(c, USER_STONE_PARSE, 0x770)) {
                if (parse(c, STAGE_CLEAR_PARSE, 0x768)) {
                    if (parse(c, STAGE_RESUMPTION_PARSE, 0x760)) {
                        const uint32_t request = addr(&c.self->stageRequest);
                        c.F.call(SAFEBUFFER_READ, {request, c.F.at(0xCC0), load32(request + 12) + REQ_STAGE_ID, 4,
                                                   REQ_STAGE_ID});
                        c.F.call(MASTER_STAGE_CREATE, {c.F.at(0x758), c.F.word(0xCC0)});
                        const uint32_t stage = c.F.word(0x758);
                        if (stage) {
                            c.F.call(SAFEBUFFER_READ, {stage, c.F.at(0xCC4), load32(stage + 12) + MASTER_STAGE_NUMBER, 4,
                                                       MASTER_STAGE_NUMBER});
                            const int32_t number = int32_t(c.F.word(0xCC4));
                            if (number > 199) {
                                if (number == 200) sendEventOnce(c, EV_QUESTS_COMPLETE_200);
                            } else if (number == 6) {
                                sendEventOnce(c, EV_QUESTS_COMPLETE_6);
                            } else if (number == 34) {
                                sendEventOnce(c, EV_QUESTS_COMPLETE_34);
                            } else if (number == 130) {
                                sendEventOnce(c, EV_QUESTS_COMPLETE_130);
                            }
                        }
                        sendLevelEvents(c, 0x778, 0xCC8, 0xCCC);
                        keep(c, 0x780, off::USER_POINT);
                        keep(c, 0x778, off::USER_DETAIL);
                        keep(c, 0x770, off::USER_STONE);
                        keep(c, 0x768, off::STAGE_CLEAR);
                        keep(c, 0x760, off::STAGE_RESUMPTION);
                        const HeldList materials = holdList(c, c.F.word(0x768) + STAGE_CLEAR_MATERIALS);
                        applyHeld(c, materials, MATERIAL_SIZE, 0x748, UPDATE_MATERIAL);
                        const HeldList medals = holdList(c, c.F.word(0x768) + STAGE_CLEAR_MEDALS);
                        applyHeld(c, medals, MEDAL_SIZE, 0x12B0, UPDATE_MEDAL);
                        saveAppState(c);
                        const HeldList skills = holdList(c, c.F.word(0x768) + STAGE_CLEAR_SKILLS);
                        applyHeld(c, skills, SKILL_SIZE, 0x720, UPDATE_SKILL);
                        const HeldList titles = holdList(c, c.F.word(0x768) + STAGE_CLEAR_TITLES);
                        applyHeld(c, titles, TITLE_SIZE, 0x710, UPDATE_TITLE);
                        const HeldList keyblades = holdList(c, c.F.word(0x768) + STAGE_CLEAR_KEYBLADES);
                        applyHeld(c, keyblades, KEYBLADE_SIZE, 0x1258, UPDATE_KEYBLADE);
                        releaseHeld(c, keyblades);
                        releaseHeld(c, titles);
                        releaseHeld(c, skills);
                        releaseHeld(c, medals);
                        releaseHeld(c, materials);
                        ok = 1;
                        drop(c, 0x758);                     // the master stage
                    }
                    drop(c, 0x760);
                }
                drop(c, 0x768);
            }
            drop(c, 0x770);
        }
        drop(c, 0x778);
    }
    drop(c, 0x780);
    return ok;
}

// raid: kept when it parsed; never an error
uint32_t verifyRaid(const Ctx &c) {
    if (parse(c, RAID_PARSE, 0x708)) keep(c, 0x708, off::RAID);
    drop(c, 0x708);
    return 1;
}

// raid/list: raids (objects) into RaidBase records that replace the manager's list
uint32_t verifyRaidList(const Ctx &c) {
    const uint32_t raids = c.F.at(0x6FC);                // std::vector<RaidBase>
    c.F.setWord(0x6FC, 0);
    c.F.setWord(0x700, 0);
    c.F.setWord(0x704, 0);
    uint32_t ok = 0;
    const uint32_t list = jsonMember(c, c.json, S_RAIDS);
    if (jsonFlags(list) == JSON_ARRAY) {
        const uint32_t n = jsonArraySize(list);
        const uint32_t raid = c.F.at(0x18C0);            // the RaidBase being read
        bool good = true;
        for (uint32_t k = 0; k < n; k++) {
            const uint32_t e = jsonArrayAt(list, k);
            if (jsonFlags(e) != JSON_OBJECT) {
                good = false;
                break;
            }
            frameAnd8(c, 0x18C0 + 0x17, 0xFD);
            frameAnd8(c, 0x18C0 + 0x21, 0xFD);
            c.F.setWord(0x1970, 0);                       // its std::string at +0xB0
            c.F.setWord(0x1974, 0);
            c.F.setWord(0x1978, 0);
            if (!c.F.call(RAID_BASE_INIT, {raid, e})) {
                destroyString(c, raid + 0xB0);
                good = false;
                break;
            }
            // raids.push_back(raid): RaidBase's copy constructor as inlined
            const uint32_t end = load32(raids + 4);
            if (end == load32(raids + 8)) {
                c.F.call(RAID_VECTOR_PUSH_SLOW, {raids, raid});
            } else {
                uint32_t at = 0;
                if (end) {
                    c.F.call(MEMCPY, {end, raid, 0xA8});
                    rw::save<uint32_t>(end + 0xAC, load32(raid + 0xAC));
                    rw::save<uint32_t>(end + 0xA8, load32(raid + 0xA8));
                    if (load8(raid + 0xB0) & 1)
                        c.F.call(STRING_INIT, {end + 0xB0, load32(raid + 0xB8), load32(raid + 0xB4)});
                    else
                        copyGuest(end + 0xB0, raid + 0xB0, 12);
                    for (uint32_t block = 0xBC; block < 0xEC; block += 16)   // each 16 bytes stored high word first
                        for (int w = 3; w >= 0; w--)
                            rw::save<uint32_t>(end + block + 4 * w, load32(raid + block + 4 * w));
                    rw::save<uint32_t>(end + 0xEC, load32(raid + 0xEC));
                    c.F.call(MEMCPY, {end, raid, 0xA8});
                    at = load32(raids + 4);
                }
                rw::save<uint32_t>(raids + 4, at + RAID_BASE_SIZE);
            }
            destroyString(c, raid + 0xB0);
        }
        if (good) {
            StdVector &kept = c.self->raids;                 // off::RAIDS
            c.F.call(RAID_VECTOR_DEALLOCATE, {addr(&kept)});
            kept.begin = c.F.word(0x6FC);
            kept.end = c.F.word(0x700);
            kept.cap = c.F.word(0x704);
            c.F.setWord(0x6FC, 0);
            c.F.setWord(0x700, 0);
            c.F.setWord(0x704, 0);
            ok = 1;
        }
    }
    c.F.call(RAID_VECTOR_DTOR, {raids});
    return ok;
}

// raid/reward: points and the rewards; each reward's medals (then saved), skills and materials applied
uint32_t verifyRaidReward(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x6F4)) {
        if (parse(c, RAID_REWARD_PARSE, 0x6EC)) {
            keep(c, 0x6F4, off::USER_POINT);
            keep(c, 0x6EC, off::RAID_REWARD);
            const uint32_t rewards = member(c.self, off::RAID_REWARD).ptr;
            const uint32_t last = load32(rewards + 4);
            for (uint32_t e = load32(rewards); e != last; e += RAID_REWARD_SIZE) {
                applyRecordsAt(c, e + RAID_REWARD_MEDALS, MEDAL_SIZE, 0x1210, UPDATE_MEDAL);
                saveAppState(c);
                applyRecordsAt(c, e + RAID_REWARD_SKILLS, SKILL_SIZE, 0x6C0, UPDATE_SKILL);
                applyRecordsAt(c, e + RAID_REWARD_MATERIALS, MATERIAL_SIZE, 0x6B0, UPDATE_MATERIAL);
            }
            ok = 1;
        }
        drop(c, 0x6EC);
    }
    drop(c, 0x6F4);
    return ok;
}

uint32_t verifyRaidReward151101(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x6A8)) {
        if (parse(c, RAID_REWARD_151101_PARSE, 0x6A0)) {
            keep(c, 0x6A8, off::USER_POINT);
            keep(c, 0x6A0, off::RAID_REWARD_151101);
            const uint32_t rewards = member(c.self, off::RAID_REWARD_151101).ptr;
            const uint32_t last = load32(rewards + 4);
            for (uint32_t e = load32(rewards); e != last; e += RAID_REWARD_151101_SIZE) {
                applyRecordsAt(c, e + RAID_REWARD_151101_MEDALS, MEDAL_SIZE, 0x11C8, UPDATE_MEDAL);
                saveAppState(c);
                applyRecordsAt(c, e + RAID_REWARD_151101_SKILLS, SKILL_SIZE, 0x678, UPDATE_SKILL);
                applyRecordsAt(c, e + RAID_REWARD_151101_MATERIALS, MATERIAL_SIZE, 0x668, UPDATE_MATERIAL);
            }
            ok = 1;
        }
        drop(c, 0x6A0);
    }
    drop(c, 0x6A8);
    return ok;
}

uint32_t verifyRaidMember(const Ctx &c) { return parseAndKeep(c, RAID_MEMBER_PARSE, 0x660, off::RAID_MEMBER); }

// raid/start: points, the start reply and the supporters; another user's boss sends "BossHelp". The request data
// records the raid (stage id 0).
uint32_t verifyRaidStart(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x658)) {
        if (parse(c, RAID_START_PARSE, 0x650)) {
            if (parse(c, STAGE_SUPPORT_LIST_PARSE, 0x648, S_SUPPORT_USERS)) {
                const uint32_t user = member(c.self, off::USER).ptr;
                if (user) {
                    const uint32_t start = c.F.word(0x650);
                    const uint32_t ownerLo = load32(start + RAID_OWNER_ID), ownerHi = load32(start + RAID_OWNER_ID + 4);
                    c.F.call(SAFEBUFFER_READ, {user, c.F.at(0xCD0), load32(user + 12), 8, 0});
                    if (((ownerLo ^ c.F.word(0xCD0)) | (ownerHi ^ c.F.word(0xCD4))) != 0) sendEventOnce(c, EV_BOSS_HELP);
                }
                keep(c, 0x658, off::USER_POINT);
                keep(c, 0x650, off::RAID_START);
                keep(c, 0x648, off::SUPPORT_LIST);
                const uint32_t start = member(c.self, off::RAID_START).ptr;
                writeStageRequest(c, REQ_ACTION, c.F.at(0xC58), 4);
                c.F.setWord(0x644, 0);
                writeStageRequest(c, REQ_STAGE_ID, c.F.at(0x644), 4);
                writeStageRequest(c, REQ_KEYBLADE_ID, start + START_KEYBLADE_ID, 8);
                writeStageRequest(c, REQ_SUPPORT_USER_ID, start + START_SUPPORT_USER_ID, 8);
                ok = 1;
                c.F.call(UPDATE_FAVORITE_KEYBLADE, {addr(c.self), 0, load32(start + START_KEYBLADE_ID),
                                                    load32(start + START_KEYBLADE_ID + 4)});
            }
            drop(c, 0x648);
        }
        drop(c, 0x650);
    }
    drop(c, 0x658);
    return ok;
}

// raid/clear: points, detail, the clear result (its skills, titles and keyblades applied) and the resumption
// state; the new level sends the "AvatarLevel" milestones
uint32_t verifyRaidClear(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x63C)) {
        if (parse(c, USER_DETAIL_PARSE, 0x634)) {
            if (parse(c, RAID_CLEAR_PARSE, 0x62C)) {
                if (parse(c, STAGE_RESUMPTION_PARSE, 0x624)) {
                    sendLevelEvents(c, 0x634, 0xCD8, 0xCDC);
                    keep(c, 0x63C, off::USER_POINT);
                    keep(c, 0x634, off::USER_DETAIL);
                    keep(c, 0x62C, off::RAID_CLEAR);
                    keep(c, 0x624, off::STAGE_RESUMPTION);
                    const HeldList skills = holdList(c, c.F.word(0x62C) + RAID_CLEAR_SKILLS);
                    applyHeld(c, skills, SKILL_SIZE, 0x5F8, UPDATE_SKILL);
                    const HeldList titles = holdList(c, c.F.word(0x62C) + RAID_CLEAR_TITLES);
                    applyHeld(c, titles, TITLE_SIZE, 0x5E8, UPDATE_TITLE);
                    const HeldList keyblades = holdList(c, c.F.word(0x62C) + RAID_CLEAR_KEYBLADES);
                    applyHeld(c, keyblades, KEYBLADE_SIZE, 0x1170, UPDATE_KEYBLADE);
                    releaseHeld(c, keyblades);
                    releaseHeld(c, titles);
                    releaseHeld(c, skills);
                    ok = 1;
                }
                drop(c, 0x624);
            }
            drop(c, 0x62C);
        }
        drop(c, 0x634);
    }
    drop(c, 0x63C);
    return ok;
}

uint32_t verifyColosseum(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, COLOSSEUM_PARSE, 0x348, 0)) {
        keep(c, 0x348, off::COLOSSEUM);
        ok = 1;
    }
    drop(c, 0x348);
    return ok;
}

// colosseum/reward/list: getRewardRankIds (ints) replace the manager's list
uint32_t verifyColosseumRewardList(const Ctx &c) {
    return readIntList(c, S_GET_REWARD_RANK_IDS, 0x33C, 0xCF8, c.self->ints_254) ? 1 : 0;   // off::COLOSSEUM_REWARD_RANK_IDS
}

// colosseum/start: points, the colosseum, the start reply; the request data records the battle
uint32_t verifyColosseumStart(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x334)) {
        if (parse(c, COLOSSEUM_PARSE, 0x32C, 1)) {
            if (parse(c, COLOSSEUM_START_PARSE, 0x324)) {
                sendEventOnce(c, EV_COLISEUM_STAGE);
                keep(c, 0x334, off::USER_POINT);
                keep(c, 0x32C, off::COLOSSEUM);
                keep(c, 0x324, off::COLOSSEUM_START);
                const uint32_t start = member(c.self, off::COLOSSEUM_START).ptr;
                writeStageRequest(c, REQ_ACTION, c.F.at(0xC58), 4);
                writeStageRequest(c, REQ_STAGE_ID, start + START_STAGE_ID, 4);
                c.F.setWord(0x318, 0);                    // no supporter
                c.F.setWord(0x31C, 0);
                writeStageRequest(c, REQ_SUPPORT_USER_ID, c.F.at(0x318), 8);
                writeStageRequest(c, REQ_KEYBLADE_ID, start + COLOSSEUM_KEYBLADE_ID, 8);
                ok = 1;
                c.F.call(UPDATE_FAVORITE_KEYBLADE, {addr(c.self), 0, load32(start + COLOSSEUM_KEYBLADE_ID),
                                                    load32(start + COLOSSEUM_KEYBLADE_ID + 4)});
            }
            drop(c, 0x324);
        }
        drop(c, 0x32C);
    }
    drop(c, 0x334);
    return ok;
}

// colosseum/clear: points, detail, the clear result (its items applied) and the resumption state
uint32_t verifyColosseumClear(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x310)) {
        if (parse(c, USER_DETAIL_PARSE, 0x308)) {
            if (parse(c, COLOSSEUM_CLEAR_PARSE, 0x300)) {
                if (parse(c, STAGE_RESUMPTION_PARSE, 0x2F8)) {
                    sendLevelEvents(c, 0x308, 0xCE8, 0xCEC);
                    keep(c, 0x310, off::USER_POINT);
                    keep(c, 0x308, off::USER_DETAIL);
                    keep(c, 0x300, off::COLOSSEUM_CLEAR);
                    keep(c, 0x2F8, off::STAGE_RESUMPTION);
                    const HeldList materials = holdList(c, c.F.word(0x300) + COLOSSEUM_CLEAR_MATERIALS);
                    applyHeld(c, materials, MATERIAL_SIZE, 0x2E8, UPDATE_MATERIAL);
                    const HeldList medals = holdList(c, c.F.word(0x300) + COLOSSEUM_CLEAR_MEDALS);
                    applyHeld(c, medals, MEDAL_SIZE, 0xF48, UPDATE_MEDAL);
                    saveAppState(c);
                    const HeldList skills = holdList(c, c.F.word(0x300) + COLOSSEUM_CLEAR_SKILLS);
                    applyHeld(c, skills, SKILL_SIZE, 0x2C0, UPDATE_SKILL);
                    const HeldList titles = holdList(c, c.F.word(0x300) + COLOSSEUM_CLEAR_TITLES);
                    applyHeld(c, titles, TITLE_SIZE, 0x2B0, UPDATE_TITLE);
                    const HeldList keyblades = holdList(c, c.F.word(0x300) + COLOSSEUM_CLEAR_KEYBLADES);
                    applyHeld(c, keyblades, KEYBLADE_SIZE, 0xEF0, UPDATE_KEYBLADE);
                    releaseHeld(c, keyblades);
                    releaseHeld(c, titles);
                    releaseHeld(c, skills);
                    releaseHeld(c, medals);
                    releaseHeld(c, materials);
                    ok = 1;
                }
                drop(c, 0x2F8);
            }
            drop(c, 0x300);
        }
        drop(c, 0x308);
    }
    drop(c, 0x310);
    return ok;
}

}  // namespace apiv
