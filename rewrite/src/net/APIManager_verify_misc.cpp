// APIManager::verifyResponse: rankings, campaigns, account links, user options and album challenges.
#include "APIManager_verify.hpp"

namespace apiv {

namespace {

constexpr uint32_t STRING_RESERVE = 0x48D705;              // std::string::reserve(size_t) (0: shrink to fit)
constexpr uint32_t LINK_MAP_INDEX = 0x4C4FE1;              // unordered_map<PlatformType, Info>::operator[](const key&)
constexpr uint32_t LINK_MAP_ERASE = 0x4D524D;              // __hash_table<...>::__erase_unique<PlatformType>

constexpr uint32_t S_USER_DATA = 0xDFA622;                 // "userData"
constexpr uint32_t S_USER_STONE = 0xDFA66C;                // "userStone"
constexpr uint32_t S_PLATFORM_TYPE = 0xDFA83B;             // "platformType"
constexpr uint32_t S_CAMPAIGNS = 0xDFAEC9;                 // "campaigns"
constexpr uint32_t S_LUX_RANKING_WEEKLY = 0xDFBCE8;        // "luxRankingWeekly"
constexpr uint32_t S_LUX_RANKING_MONTHLY = 0xDFBCF9;       // "luxRankingMonthly"
constexpr uint32_t S_LUX_RANKING_WEEKLY_OWN = 0xDFBD0B;    // "luxRankingWeeklyOwn"
constexpr uint32_t S_LUX_RANKING_MONTHLY_OWN = 0xDFBD1F;   // "luxRankingMonthlyOwn"
constexpr uint32_t S_LUX_RANKING = 0xDFBD34;               // "luxRanking" (ranking/lux and ranking/lux/own)
constexpr uint32_t S_PARTY_RANKING = 0xDFBD3F;             // "partyRanking"
constexpr uint32_t S_PARTY_RANKING_OWN = 0xDFBD4C;         // "partyRankingOwn"
constexpr uint32_t S_COLOSSEUM_RANKING = 0xDFBD5C;         // "colosseumRanking"
constexpr uint32_t S_COLOSSEUM_RANKING_OWN = 0xDFBD6D;     // "colosseumRankingOwn"
constexpr uint32_t S_RANKING_REWARD = 0xDFBD81;            // "rankingReward"
constexpr uint32_t S_AVATAR_SYNC = 0xDFBD8F;               // "avatarSync"
constexpr uint32_t S_SRC_ACCEPT = 0xDFC060;                // "srcAccept"
constexpr uint32_t S_DEST_ACCEPT = 0xDFC055;               // "destAccept"
constexpr uint32_t S_PIN_CODE = 0xDFC035;                  // "pinCode"
constexpr uint32_t S_LINK_USER_LIST = 0xDFBD9A;            // "linkUserList"
constexpr uint32_t EV_FACEBOOK_LINK = 0xDFBDA7;            // "FacebookLink"

constexpr uint32_t JSON_FLAG_UINT = 0x800;

// a ranking (X::parse(out, response, key)) moved into its member
uint32_t ranking(const Ctx &c, uint32_t parser, uint32_t slot, uint32_t key, uint32_t offset) {
    parse(c, parser, slot, key);
    return moveParsed(c, slot, offset);
}

// std::string::clear() as inlined: the first char and the size zeroed
void clearString(uint32_t s) {
    if (load8(s) & 1) {
        rw::store8(rw::host_ptr<uint8_t>(load32(s + 8)), 0);
        rw::save<uint32_t>(s + 4, 0);
    } else {
        rw::store8(rw::host_ptr<uint8_t>(s + 1), 0);
        rw::store8(rw::host_ptr<uint8_t>(s), 0);
    }
}

}  // namespace

uint32_t verifyRankingWeeklyUser(const Ctx &c) {
    return ranking(c, LUX_RANKING_PARSE, 0x28C, S_LUX_RANKING_WEEKLY, off::LUX_RANKING_WEEKLY);
}
uint32_t verifyRankingMonthlyUser(const Ctx &c) {
    return ranking(c, LUX_RANKING_PARSE, 0x284, S_LUX_RANKING_MONTHLY, off::LUX_RANKING_MONTHLY);
}
uint32_t verifyRankingWeeklyUserOwn(const Ctx &c) {
    return ranking(c, LUX_RANKING_PARSE, 0x27C, S_LUX_RANKING_WEEKLY_OWN, off::LUX_RANKING_WEEKLY_OWN);
}
uint32_t verifyRankingMonthlyUserOwn(const Ctx &c) {
    return ranking(c, LUX_RANKING_PARSE, 0x274, S_LUX_RANKING_MONTHLY_OWN, off::LUX_RANKING_MONTHLY_OWN);
}
uint32_t verifyRankingLux(const Ctx &c) { return ranking(c, LUX_RANKING_PARSE, 0x26C, S_LUX_RANKING, off::LUX_RANKING); }
uint32_t verifyRankingLuxOwn(const Ctx &c) {
    return ranking(c, LUX_RANKING_PARSE, 0x264, S_LUX_RANKING, off::LUX_RANKING_OWN);
}
uint32_t verifyRankingParty(const Ctx &c) {
    return ranking(c, PARTY_RANKING_PARSE, 0x25C, S_PARTY_RANKING, off::PARTY_RANKING);
}
uint32_t verifyRankingPartyOwn(const Ctx &c) {
    return ranking(c, PARTY_RANKING_PARSE, 0x254, S_PARTY_RANKING_OWN, off::PARTY_RANKING_OWN);
}
uint32_t verifyRankingColosseum(const Ctx &c) {
    return ranking(c, COLOSSEUM_RANKING_PARSE, 0x24C, S_COLOSSEUM_RANKING, off::COLOSSEUM_RANKING);
}
uint32_t verifyRankingColosseumOwn(const Ctx &c) {
    return ranking(c, COLOSSEUM_RANKING_PARSE, 0x244, S_COLOSSEUM_RANKING_OWN, off::COLOSSEUM_RANKING_OWN);
}
uint32_t verifyRankingParade(const Ctx &c) {
    parse(c, UNION_RANKING_PARSE, 0x23C);
    return moveParsed(c, 0x23C, off::UNION_RANKING);
}

// ranking/reward/check: the reward check; a non-empty rankingReward carries the items given (parsed from that
// member: medals, skills, avatar parts, materials, keyblades, titles - and points / stones, required when the
// reply's root has userData / userStone)
uint32_t verifyRankingRewardCheck(const Ctx &c) {
    if (!parse(c, RANKING_REWARD_PARSE, 0x234)) {
        drop(c, 0x234);
        return 0;
    }
    const uint32_t reward = jsonMember(c, c.json, S_RANKING_REWARD);
    if (!(jsonFlags(reward) == JSON_ARRAY && jsonArraySize(reward) == 0)) {
        struct Part { uint32_t parser, slot; };
        static constexpr Part parts[] = {
            {USER_MEDAL_PARSE, 0x22C}, {USER_SKILL_PARSE, 0x224}, {USER_AVATAR_PARTS_PARSE, 0x21C},
            {USER_MATERIAL_PARSE, 0x214}, {USER_KEYBLADE_PARSE, 0x20C}, {USER_TITLE_PARSE, 0x204},
            {USER_POINT_PARSE, 0x1FC}, {USER_STONE_PARSE, 0x1F4},
        };
        constexpr int N = int(sizeof(parts) / sizeof(parts[0]));
        int k = 0;
        for (; k < N; k++) {
            c.F.call(parts[k].parser, {c.F.at(parts[k].slot), reward});
            if (c.F.word(parts[k].slot)) continue;
            // points / stones may be missing when the root does not carry them either
            if (k == 6 && jsonFlags(jsonMember(c, c.json, S_USER_DATA)) == 0) continue;
            if (k == 7 && jsonFlags(jsonMember(c, c.json, S_USER_STONE)) == 0) continue;
            break;
        }
        const bool parsed = k == N;
        if (parsed) {
            applyRecords(c, 0x22C, MEDAL_SIZE, 0xEA8, UPDATE_MEDAL);
            saveAppState(c);
            applyRecords(c, 0x224, SKILL_SIZE, 0x1C8, UPDATE_SKILL);
            applyRecords(c, 0x21C, AVATAR_PART_SIZE, 0x1B0, UPDATE_AVATAR_PART);
            applyRecords(c, 0x214, MATERIAL_SIZE, 0x1A0, UPDATE_MATERIAL);
            applyRecords(c, 0x20C, KEYBLADE_SIZE, 0xE50, UPDATE_KEYBLADE);
            applyRecords(c, 0x204, TITLE_SIZE, 0x190, UPDATE_TITLE);
            if (c.F.word(0x1FC)) keep(c, 0x1FC, off::USER_POINT);
            if (c.F.word(0x1F4)) keep(c, 0x1F4, off::USER_STONE);
        }
        for (int j = parsed ? N - 1 : k; j >= 0; j--) drop(c, parts[j].slot);
        if (!parsed) {
            drop(c, 0x234);
            return 0;
        }
    }
    moveParsed(c, 0x234, off::RANKING_REWARD);
    drop(c, 0x234);
    return 1;
}

// campaign: campaigns (ints) replace the manager's list
uint32_t verifyCampaign(const Ctx &c) {
    return readIntList(c, S_CAMPAIGNS, 0x184, 0xCBC, c.self->ints_2fc) ? 1 : 0;   // off::CAMPAIGN_IDS
}

// campaign/pincode (get), campaign/pincode/status: avatarSync.srcAccept / destAccept (bools)
uint32_t verifyCampaignPincodeGet(const Ctx &c) {
    rw::set<uint8_t>(c.self, off::AVATAR_SYNC_DEST, 0);
    rw::set<uint8_t>(c.self, off::AVATAR_SYNC_SRC, 0);
    const uint32_t sync = jsonMember(c, c.json, S_AVATAR_SYNC);
    if (jsonFlags(sync) != JSON_OBJECT) return 0;
    const uint32_t src = jsonFlags(jsonMember(c, sync, S_SRC_ACCEPT));
    if (!(src & JSON_FLAG_BOOL)) return 0;
    const uint32_t dest = jsonFlags(jsonMember(c, sync, S_DEST_ACCEPT));
    if (!(dest & JSON_FLAG_BOOL)) return 0;
    rw::set<uint8_t>(c.self, off::AVATAR_SYNC_SRC, src == JSON_TRUE ? 1 : 0);
    rw::set<uint8_t>(c.self, off::AVATAR_SYNC_DEST, dest == JSON_TRUE ? 1 : 0);
    return 1;
}

// campaign/pincode (create): avatarSync.pinCode replaces the kept pin code (cleared first in any case)
uint32_t verifyCampaignPincodeCreate(const Ctx &c) {
    const uint32_t pin = addr(c.self) + off::PIN_CODE;
    clearString(pin);
    const uint32_t sync = jsonMember(c, c.json, S_AVATAR_SYNC);
    if (jsonFlags(sync) != JSON_OBJECT) return 0;
    const uint32_t code = c.F.at(0x170);                  // std::string
    c.F.setWord(0x170, 0);
    c.F.setWord(0x174, 0);
    c.F.setWord(0x178, 0);
    const uint32_t value = jsonMember(c, sync, S_PIN_CODE);
    if (!(jsonFlags(value) & JSON_FLAG_STRING)) return 0;
    const uint32_t chars = load32(value);
    c.F.call(STRING_ASSIGN_CHARS, {code, chars, c.F.call(STRLEN, {chars})});
    // pin = std::move(code)
    clearString(pin);
    c.F.call(STRING_RESERVE, {pin, 0});
    copyGuest(pin, code, 12);
    c.F.setWord(0x170, 0);
    c.F.setWord(0x174, 0);
    c.F.setWord(0x178, 0);
    return 1;
}

uint32_t verifyUserOption(const Ctx &c) { return parseAndKeep(c, USER_OPTION_PARSE, 0x168, off::USER_OPTION); }

// link/members: the linked users (a PartyMemberList)
uint32_t verifyLinkMembers(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, PARTY_MEMBER_LIST_PARSE, 0xB0, S_LINK_USER_LIST)) {
        keep(c, 0xB0, off::LINK_MEMBERS);
        ok = 1;
    }
    drop(c, 0xB0);
    return ok;
}

uint32_t verifyUserLink(const Ctx &c) { return parseAndKeep(c, USER_LINK_PARSE, 0xA8, off::USER_LINK); }

// user/link update: the reply's link (UserLink::Info: platform type, token) stored in the cached links
uint32_t verifyUserLinkUpdate(const Ctx &c) {
    const uint32_t info = c.F.at(0x98);                   // the platform type (u8), its token string at +4
    c.F.setWord(0x9C, 0);
    c.F.setWord(0xA0, 0);
    c.F.setWord(0xA4, 0);
    uint32_t ok = 0;
    if (c.F.call(LINK_INFO_INIT, {info, c.json})) {
        sendEventOnce(c, EV_FACEBOOK_LINK);
        const uint32_t entry = c.F.call(LINK_MAP_INDEX, {member(c.self, off::USER_LINK).ptr, info});
        rw::store8(rw::host_ptr<uint8_t>(entry), load8(info));
        ok = 1;
        assignString(c, entry + 4, info + 4);
    }
    destroyString(c, info + 4);
    return ok;
}

// user/link delete: the request's platformType link is removed from the cached links
uint32_t verifyUserLinkDelete(const Ctx &c) {
    const uint32_t platform = jsonMember(c, c.self->params.ptr, S_PLATFORM_TYPE);
    if (!(jsonFlags(platform) & JSON_FLAG_UINT)) return 0;
    *rw::frame_ptr<uint8_t>(c.F.at(0x97)) = uint8_t(load32(platform));
    c.F.call(LINK_MAP_ERASE, {member(c.self, off::USER_LINK).ptr, c.F.at(0x97)});
    return 1;
}

// user/album/challenge/update: the challenges updated in the cached album, the rewards applied
uint32_t verifyUserAlbumChallengeUpdate(const Ctx &c) {
    struct Part { uint32_t parser, slot; };
    static constexpr Part parts[] = {
        {USER_ALBUM_CHALLENGE_PARSE, 0x160}, {USER_MATERIAL_PARSE, 0x158}, {USER_MEDAL_PARSE, 0x150},
        {USER_SKILL_PARSE, 0x148}, {USER_AVATAR_PARTS_PARSE, 0x140}, {USER_KEYBLADE_PARSE, 0x138},
        {USER_POINT_PARSE, 0x130}, {USER_DETAIL_PARSE, 0x128}, {USER_TITLE_PARSE, 0x120},
        {USER_STONE_PARSE, 0x118},
    };
    constexpr int N = int(sizeof(parts) / sizeof(parts[0]));
    int k = 0;
    while (k < N && parse(c, parts[k].parser, parts[k].slot)) k++;
    uint32_t ok = 0;
    if (k == N) {
        c.F.call(USER_ALBUM_CHALLENGE_UPDATE, {member(c.self, off::USER_ALBUM_CHALLENGE).ptr, c.F.word(0x160)});
        keep(c, 0x130, off::USER_POINT);
        keep(c, 0x128, off::USER_DETAIL);
        keep(c, 0x118, off::USER_STONE);
        applyRecords(c, 0x158, MATERIAL_SIZE, 0x108, UPDATE_MATERIAL);
        applyRecords(c, 0x150, MEDAL_SIZE, 0xE08, UPDATE_MEDAL);
        saveAppState(c);
        applyRecords(c, 0x148, SKILL_SIZE, 0xE0, UPDATE_SKILL);
        applyRecords(c, 0x140, AVATAR_PART_SIZE, 0xC8, UPDATE_AVATAR_PART);
        applyRecords(c, 0x138, KEYBLADE_SIZE, 0xDB0, UPDATE_KEYBLADE);
        applyRecords(c, 0x120, TITLE_SIZE, 0xB8, UPDATE_TITLE);
        ok = 1;
    }
    for (int j = k < N ? k : N - 1; j >= 0; j--) drop(c, parts[j].slot);
    return ok;
}

}  // namespace apiv
