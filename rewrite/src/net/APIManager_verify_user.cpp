// APIManager::verifyResponse: user data (user/*, items, medal and skill operations, keyblades, presents).
#include "APIManager_verify.hpp"
#include "master/Medal.hpp"

namespace apiv {

namespace {

constexpr uint32_t S_USER_DATA = 0xDFA622;               // "userData"
constexpr uint32_t S_USER_STONE = 0xDFA66C;              // "userStone"
constexpr uint32_t S_USER_KEYBLADE = 0xDFA7AC;           // "userKeyblade"
constexpr uint32_t S_USER_MATERIAL = 0xDFBC01;           // "userMaterial"
constexpr uint32_t S_USER_POP_UP = 0xDFBBCA;             // "userPopUp"
constexpr uint32_t S_IS_POP_BENEFIT_STONE = 0xDFC117;    // "isPopBenefitStone"
constexpr uint32_t S_LAST_USER_PRESENT_IDS = 0xDFC09B;   // "lastUserPresentIds"
constexpr uint32_t S_SELL_USER_MEDAL_IDS = 0xDFC0FC;     // "sellUserMedalIds"
constexpr uint32_t S_SELL_USER_SKILL_IDS = 0xDFC0EB;     // "sellUserSkillIds"
constexpr uint32_t S_COMPONENT_USER_MEDAL_IDS = 0xDFBEA2;// "componentUserMedalIds"
constexpr uint32_t S_ATTACHMENT_MEDAL_IDS = 0xDFBEC9;    // "attachmentMedalIds"
constexpr uint32_t S_USER_SKILL_IDS = 0xDFBE85;          // "userSkillIds"
// tracking events
constexpr uint32_t EV_SHARE_MEDAL = 0xDFBBE9;            // "ShareMedal": the support medal changed
constexpr uint32_t EV_CHANGE_OUTFIT = 0xDFBBF4;          // "ChangeOutfit"
constexpr uint32_t EV_SP_ATTACK_BONUS = 0xDFBC0E;        // "SPAttackBonus"
constexpr uint32_t EV_ENHANCE_6_STARS = 0xDFBC1C;        // "Enhance6Stars"

constexpr uint32_t SUPPORT_MEDAL_ID = 0x58;              // UserSupport: u64 at data position 0x58
constexpr uint32_t MEDAL_RARITY = master::Medal::RARE;
constexpr uint32_t ENHANCE_GREAT = 0x28;                 // UserMedalEnhance (+0x28 == 0 and +0x70 != 0: SP bonus)
constexpr uint32_t ENHANCE_SP_BONUS = 0x70;
constexpr uint32_t ENHANCE_MEDAL = 0x48;                 // UserMedalEnhance / UserMedalEvolve: the resulting medal
constexpr uint32_t ENHANCE_SKILLS = 0x94;                // UserMedalEnhance: std::vector<Skill> (40-byte skills)
constexpr uint32_t EVOLVE_MEDAL_ID = 0x50;               // UserMedalEvolve: the medal's master id

uint32_t keepDetail(const Ctx &c, uint32_t slot) { return parseAndKeep(c, USER_DETAIL_PARSE, slot, off::USER_DETAIL); }

}  // namespace

// user: User, UserPoint, UserDetail and StageResumption must all parse, and userPopUp must be an object with the
// integer isPopBenefitStone; non-zero raises AppState's benefit stone popup.
uint32_t verifyUser(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_PARSE, 0xC20, 1, S_USER_DATA)) {
        if (parse(c, USER_POINT_PARSE, 0xC18)) {
            if (parse(c, USER_DETAIL_PARSE, 0xC10)) {
                if (parse(c, STAGE_RESUMPTION_PARSE, 0xC08)) {
                    const uint32_t popUp = jsonMember(c, c.json, S_USER_POP_UP);
                    if (jsonFlags(popUp) == JSON_OBJECT) {
                        const uint32_t stone = jsonMember(c, popUp, S_IS_POP_BENEFIT_STONE);
                        if (jsonFlags(stone) & JSON_FLAG_INT) {
                            const uint32_t popBenefitStone = load32(stone);
                            keep(c, 0xC20, off::USER);
                            keep(c, 0xC18, off::USER_POINT);
                            keep(c, 0xC10, off::USER_DETAIL);
                            keep(c, 0xC08, off::STAGE_RESUMPTION);
                            if (popBenefitStone)
                                c.F.call(APPSTATE_SET_POP_BENEFIT_STONE, {c.F.call(APPSTATE_GET_INSTANCE, {}), 1});
                            ok = 1;
                        }
                    }
                }
                drop(c, 0xC08);
            }
            drop(c, 0xC10);
        }
        drop(c, 0xC18);
    }
    drop(c, 0xC20);
    return ok;
}

// user/detail: another user's detail (kept apart from the player's)
uint32_t verifyUserDetailOther(const Ctx &c) { return parseAndKeep(c, USER_DETAIL_PARSE, 0xC00, off::USER_DETAIL_OTHER); }
uint32_t verifyUserProfile(const Ctx &c) { return parseAndKeep(c, USER_PROFILE_PARSE, 0xBF8, off::USER_PROFILE); }
uint32_t verifyUserChat(const Ctx &c) { return parseAndKeep(c, USER_CHAT_PARSE, 0xBF0, off::USER_CHAT); }
uint32_t verifyUserStone(const Ctx &c) { return parseAndKeep(c, USER_STONE_PARSE, 0xA70, off::USER_STONE); }
uint32_t verifyUserShop(const Ctx &c) { return parseAndKeep(c, USER_SHOP_PARSE, 0xA68, off::USER_SHOP); }
uint32_t verifyUserBirthday(const Ctx &c) { return parseAndKeep(c, USER_BIRTHDAY_PARSE, 0x468, off::USER_BIRTHDAY); }

uint32_t verifyUserSphere(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_SPHERE_PARSE, 0xA60, 0)) {
        keep(c, 0xA60, off::USER_SPHERE);
        ok = 1;
    }
    drop(c, 0xA60);
    return ok;
}

uint32_t verifyUserKeyblade(const Ctx &c) { return parseAndKeep(c, USER_KEYBLADE_PARSE, 0xA28, off::USER_KEYBLADE); }
uint32_t verifyUserSkill(const Ctx &c) { return parseAndKeep(c, USER_SKILL_PARSE, 0xA38, off::USER_SKILL); }
uint32_t verifyUserMedal(const Ctx &c) { return parseAndKeep(c, USER_MEDAL_PARSE, 0xA40, off::USER_MEDAL); }
uint32_t verifyUserMaterial(const Ctx &c) { return parseAndKeep(c, USER_MATERIAL_PARSE, 0xA30, off::USER_MATERIAL); }

// user/album: the album and its challenges
uint32_t verifyUserAlbum(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_ALBUM_PARSE, 0xA20)) {
        if (parse(c, USER_ALBUM_CHALLENGE_PARSE, 0xA18)) {
            keep(c, 0xA20, off::USER_ALBUM);
            keep(c, 0xA18, off::USER_ALBUM_CHALLENGE);
            ok = 1;
        }
        drop(c, 0xA18);
    }
    drop(c, 0xA20);
    return ok;
}

uint32_t verifyUserTitle(const Ctx &c) { return parseAndKeep(c, USER_TITLE_PARSE, 0xA08, off::USER_TITLE); }

// user/support (+ support/update): a different support medal than the one cached sends "ShareMedal"
uint32_t verifyUserSupport(const Ctx &c) {
    const uint32_t support = parse(c, USER_SUPPORT_PARSE, 0xA10);
    if (!support) {
        drop(c, 0xA10);
        return 0;
    }
    StdSharedPtr &cached = member(c.self, off::USER_SUPPORT);
    if (cached.ptr) {
        c.F.call(SAFEBUFFER_READ, {support, c.F.at(0xC78), load32(support + 12) + SUPPORT_MEDAL_ID, 8, SUPPORT_MEDAL_ID});
        const uint32_t old = cached.ptr;
        c.F.call(SAFEBUFFER_READ, {old, c.F.at(0xC80), load32(old + 12) + SUPPORT_MEDAL_ID, 8, SUPPORT_MEDAL_ID});
        if (((c.F.word(0xC78) ^ c.F.word(0xC80)) | (c.F.word(0xC7C) ^ c.F.word(0xC84))) != 0)
            sendEventOnce(c, EV_SHARE_MEDAL);
    }
    keep(c, 0xA10, off::USER_SUPPORT);
    drop(c, 0xA10);
    return 1;
}

// user/present: also the unreceived count shown on my page
uint32_t verifyUserPresent(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_PRESENT_PARSE, 0xA00)) {
        keep(c, 0xA00, off::USER_PRESENT);
        const uint32_t mypage = member(c.self, off::MYPAGE_INFO).ptr;
        ok = 1;
        if (mypage) rw::save<uint32_t>(mypage + off::MYPAGE_PRESENT_COUNT, load32(member(c.self, off::USER_PRESENT).ptr + 8));
    }
    drop(c, 0xA00);
    return ok;
}

uint32_t verifyUserAvatar(const Ctx &c) { return parseAndKeep(c, USER_AVATAR_PARSE, 0xA58, off::USER_AVATAR); }
uint32_t verifyUserAvatarAll(const Ctx &c) { return parseAndKeep(c, USER_AVATAR_ALL_PARSE, 0xA50, off::USER_AVATAR_ALL); }
uint32_t verifyUserAvatarParts(const Ctx &c) {
    return parseAndKeep(c, USER_AVATAR_PARTS_PARSE, 0xA48, off::USER_AVATAR_PARTS);
}
uint32_t verifyPaymentInfo(const Ctx &c) { return parseAndKeep(c, PAYMENT_INFO_PARSE, 0x380, off::PAYMENT_INFO); }

// user/update, user/gender/update: the user (userData)
uint32_t verifyUserUpdate(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_PARSE, 0x9F8, 1, S_USER_DATA)) {
        keep(c, 0x9F8, off::USER);
        ok = 1;
    }
    drop(c, 0x9F8);
    return ok;
}

// user/playstyle/update, user/title/update: the user detail
uint32_t verifyUserDetail(const Ctx &c) { return keepDetail(c, 0x9F0); }

// user/avatar/update: the coordinates (applied one by one) and the user detail
uint32_t verifyUserAvatarUpdate(const Ctx &c) {
    parse(c, USER_AVATAR_ALL_PARSE, 0x9E8);
    parse(c, USER_DETAIL_PARSE, 0x9E0);
    uint32_t ok = 0;
    if (c.F.word(0x9E8) && c.F.word(0x9E0)) {
        sendEventOnce(c, EV_CHANGE_OUTFIT);
        applyRecords(c, 0x9E8, COORDINATE_SIZE, 0x15D0, UPDATE_COORDINATE);
        keep(c, 0x9E0, off::USER_DETAIL);
        ok = 1;
    }
    drop(c, 0x9E0);
    drop(c, 0x9E8);
    return ok;
}

// user/sphere/update: points, detail, the sphere board (each changed sphere set) and the items the board gave
uint32_t verifyUserSphereUpdate(const Ctx &c) {
    struct Part { uint32_t parser, slot; };
    // parsed in this order; the first one missing ends the check (the parsed ones are dropped in reverse)
    static constexpr Part parts[] = {
        {USER_POINT_PARSE, 0x5CC}, {USER_DETAIL_PARSE, 0x5C4}, {USER_SPHERE_PARSE, 0x5BC},
        {USER_MATERIAL_PARSE, 0x5B4}, {USER_MEDAL_PARSE, 0x5AC}, {USER_SKILL_PARSE, 0x5A4},
        {USER_KEYBLADE_PARSE, 0x59C}, {USER_AVATAR_PARTS_PARSE, 0x594}, {USER_TITLE_PARSE, 0x58C},
    };
    constexpr int N = int(sizeof(parts) / sizeof(parts[0]));
    int parsed = 0;
    uint32_t ok = 0;
    for (; parsed < N; parsed++) {
        const Part &p = parts[parsed];
        const uint32_t v = p.parser == USER_SPHERE_PARSE ? parse(c, p.parser, p.slot, 0) : parse(c, p.parser, p.slot);
        if (!v) break;
    }
    if (parsed == N) {
        keep(c, 0x5CC, off::USER_POINT);
        keep(c, 0x5C4, off::USER_DETAIL);
        // the spheres that changed (a std::map; the Sphere at node + 0x14) go into the cached board
        const uint32_t spheres = c.F.word(0x5BC);
        const uint32_t end = spheres + 4;
        for (uint32_t node = load32(spheres); node != end; node = treeNext(node))
            c.F.call(USER_SPHERE_SET, {member(c.self, off::USER_SPHERE).ptr, node + 0x14});
        applyRecords(c, 0x5B4, MATERIAL_SIZE, 0x578, UPDATE_MATERIAL);
        applyRecords(c, 0x5AC, MEDAL_SIZE, 0x1128, UPDATE_MEDAL);
        saveAppState(c);
        applyRecords(c, 0x5A4, SKILL_SIZE, 0x550, UPDATE_SKILL);
        applyRecords(c, 0x59C, KEYBLADE_SIZE, 0x10D0, UPDATE_KEYBLADE);
        applyRecords(c, 0x594, AVATAR_PART_SIZE, 0x538, UPDATE_AVATAR_PART);
        applyRecords(c, 0x58C, TITLE_SIZE, 0x528, UPDATE_TITLE);
        ok = 1;
    }
    // the slot of the part that failed was parsed too (empty): it is dropped with the others
    for (int k = parsed < N ? parsed : N - 1; k >= 0; k--) drop(c, parts[k].slot);
    return ok;
}

// user/keyblade/update: the keyblades are applied (not kept)
uint32_t verifyUserKeybladeUpdate(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_KEYBLADE_PARSE, 0x9D8)) {
        applyRecords(c, 0x9D8, KEYBLADE_SIZE, 0x1578, UPDATE_KEYBLADE);
        ok = 1;
    }
    drop(c, 0x9D8);
    return ok;
}

// user/medal/lock: the medals are applied and saved
uint32_t verifyUserMedalLock(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_MEDAL_PARSE, 0x9D0)) {
        applyRecords(c, 0x9D0, MEDAL_SIZE, 0x1530, UPDATE_MEDAL);
        saveAppState(c);
        ok = 1;
    }
    drop(c, 0x9D0);
    return ok;
}

// medal/skill/update: medals (then saved) and skills
uint32_t verifyMedalSkillUpdate(const Ctx &c) {
    parse(c, USER_MEDAL_PARSE, 0x9C8);
    parse(c, USER_SKILL_PARSE, 0x9C0);
    uint32_t ok = 0;
    if (c.F.word(0x9C8) && c.F.word(0x9C0)) {
        applyRecords(c, 0x9C8, MEDAL_SIZE, 0x14E8, UPDATE_MEDAL);
        saveAppState(c);
        applyRecords(c, 0x9C0, SKILL_SIZE, 0x998, UPDATE_SKILL);
        ok = 1;
    }
    drop(c, 0x9C0);
    drop(c, 0x9C8);
    return ok;
}

// user/present/receive: whatever the presents gave (points, stones, materials, medals, skills, keyblades, avatar
// parts, titles); the presents listed in lastUserPresentIds stay, the others are marked received.
uint32_t verifyUserPresentReceive(const Ctx &c) {
    // userData / userStone are only required when the reply has them (as objects)
    parse(c, USER_POINT_PARSE, 0x520);
    if (jsonFlags(jsonMember(c, c.json, S_USER_DATA)) == JSON_OBJECT && !c.F.word(0x520)) {
        drop(c, 0x520);
        return 0;
    }
    parse(c, USER_STONE_PARSE, 0x518);
    uint32_t ok = 0;
    int dropFrom;                                   // the slots to drop: 0x518 .. down to this one
    if (jsonFlags(jsonMember(c, c.json, S_USER_STONE)) == JSON_OBJECT && !c.F.word(0x518)) {
        dropFrom = 0x518;
    } else if (!parse(c, USER_MATERIAL_PARSE, 0x510)) {
        dropFrom = 0x510;
    } else if (!parse(c, USER_MEDAL_PARSE, 0x508)) {
        dropFrom = 0x508;
    } else if (!parse(c, USER_SKILL_PARSE, 0x500)) {
        dropFrom = 0x500;
    } else if (!parse(c, USER_KEYBLADE_PARSE, 0x4F8)) {
        dropFrom = 0x4F8;
    } else if (!parse(c, USER_AVATAR_PARTS_PARSE, 0x4F0)) {
        dropFrom = 0x4F0;
    } else if (!parse(c, USER_TITLE_PARSE, 0x4E8)) {
        dropFrom = 0x4E8;
    } else {
        dropFrom = 0x4E8;
        newIdSet(c, 0x4D4);
        if (readIdSet(c, S_LAST_USER_PRESENT_IDS, 0x4D4, 0xCF0, 0xDA8)) {
            if (c.F.word(0x520)) keep(c, 0x520, off::USER_POINT);
            if (c.F.word(0x518)) keep(c, 0x518, off::USER_STONE);
            applyRecords(c, 0x510, MATERIAL_SIZE, 0x4C0, UPDATE_MATERIAL);
            applyRecords(c, 0x508, MEDAL_SIZE, 0x1088, UPDATE_MEDAL);
            saveAppState(c);
            applyRecords(c, 0x500, SKILL_SIZE, 0x498, UPDATE_SKILL);
            applyRecords(c, 0x4F8, KEYBLADE_SIZE, 0x1030, UPDATE_KEYBLADE);
            applyRecords(c, 0x4F0, AVATAR_PART_SIZE, 0x480, UPDATE_AVATAR_PART);
            // every cached present (a std::set; the present id at node + 0x18) not in the list was received
            StdSharedPtr &presents = member(c.self, off::USER_PRESENT);
            const uint32_t end = presents.ptr + 4;
            for (uint32_t node = load32(presents.ptr); node != end; node = treeNext(node)) {
                const uint32_t id = node + 0x18;
                if (!c.F.call(ID_SET_FIND, {c.F.at(0x4D4), id}))
                    c.F.call(USER_PRESENT_SET_RECEIVED, {presents.ptr, 0, load32(id), load32(id + 4)});
            }
            const uint32_t mypage = member(c.self, off::MYPAGE_INFO).ptr;
            if (mypage) rw::save<uint32_t>(mypage + off::MYPAGE_PRESENT_COUNT, c.F.word(0x4D4 + 12));   // set size
            applyRecords(c, 0x4E8, TITLE_SIZE, 0x470, UPDATE_TITLE);
            ok = 1;
        }
        deleteIdSet(c, 0x4D4);
    }
    for (uint32_t slot = uint32_t(dropFrom); slot <= 0x518; slot += 8) drop(c, slot);
    drop(c, 0x520);
    return ok;
}

// user/material/sell: points and the sold material (userMaterial, one record)
uint32_t verifyUserMaterialSell(const Ctx &c) {
    parse(c, USER_POINT_PARSE, 0x990);
    const uint32_t material = jsonMember(c, c.json, S_USER_MATERIAL);
    uint32_t ok = 0;
    if (jsonFlags(material) == JSON_OBJECT) {
        const uint32_t initFailed = c.F.call(MATERIAL_INIT, {c.F.at(0x980), material}) ^ 1;
        if (c.F.word(0x990) && !initFailed) {
            keep(c, 0x990, off::USER_POINT);
            c.F.call(UPDATE_MATERIAL, {addr(c.self), c.F.at(0x980)});
            ok = 1;
        }
    }
    drop(c, 0x990);
    return ok;
}

// user/medal/sell: points, the skills the medals carried, and the sold medals removed
uint32_t verifyUserMedalSell(const Ctx &c) {
    if (!parse(c, USER_POINT_PARSE, 0x978)) {
        drop(c, 0x978);
        return 0;
    }
    uint32_t ok = 0;
    if (parse(c, USER_SKILL_PARSE, 0x970)) {
        newIdSet(c, 0x95C);
        if (readIdSet(c, S_SELL_USER_MEDAL_IDS, 0x95C, 0xC88, 0xD78)) {
            keep(c, 0x978, off::USER_POINT);
            applyRecords(c, 0x970, SKILL_SIZE, 0x930, UPDATE_SKILL);
            c.F.call(REMOVE_MEDALS, {addr(c.self), c.F.at(0x95C)});
            ok = 1;
        }
        deleteIdSet(c, 0x95C);
    }
    drop(c, 0x970);
    drop(c, 0x978);
    return ok;
}

// skill/sell: points, the medals that carried the skills, and the sold skills removed
uint32_t verifySkillSell(const Ctx &c) {
    if (!parse(c, USER_POINT_PARSE, 0x928)) {
        drop(c, 0x928);
        return 0;
    }
    uint32_t ok = 0;
    if (parse(c, USER_MEDAL_PARSE, 0x920)) {
        newIdSet(c, 0x90C);
        if (readIdSet(c, S_SELL_USER_SKILL_IDS, 0x90C, 0xC90, 0xD80)) {
            keep(c, 0x928, off::USER_POINT);
            applyRecords(c, 0x920, MEDAL_SIZE, 0x14A0, UPDATE_MEDAL);
            saveAppState(c);
            c.F.call(REMOVE_SKILLS, {addr(c.self), c.F.at(0x90C)});
            ok = 1;
        }
        deleteIdSet(c, 0x90C);
    }
    drop(c, 0x920);
    drop(c, 0x928);
    return ok;
}

// keyblade/burst/max: stones and the keyblade (userKeyblade, one record)
uint32_t verifyKeybladeBurstMax(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_STONE_PARSE, 0x904)) {
        const uint32_t keyblade = jsonMember(c, c.json, S_USER_KEYBLADE);
        if (jsonFlags(keyblade) == JSON_OBJECT && c.F.call(KEYBLADE_INIT, {c.F.at(0x1448), keyblade}) == 1) {
            keep(c, 0x904, off::USER_STONE);
            ok = 1;
            c.F.call(UPDATE_KEYBLADE, {addr(c.self), c.F.at(0x1448)});
        }
    }
    drop(c, 0x904);
    return ok;
}

// keyblade/enhance: keyblades, the materials spent and the titles it gave (kept and applied)
uint32_t verifyKeybladeEnhance(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_KEYBLADE_PARSE, 0x8FC)) {
        if (parse(c, USER_MATERIAL_PARSE, 0x8F4)) {
            if (parse(c, USER_TITLE_PARSE, 0x8EC)) {
                keep(c, 0x8EC, off::KEYBLADE_ENHANCE_TITLE);
                applyRecords(c, 0x8FC, KEYBLADE_SIZE, 0x13F0, UPDATE_KEYBLADE);
                applyRecords(c, 0x8F4, MATERIAL_SIZE, 0x8D8, UPDATE_MATERIAL);
                applyRecords(c, 0x8EC, TITLE_SIZE, 0x8C8, UPDATE_TITLE);
                ok = 1;
            }
            drop(c, 0x8EC);
        }
        drop(c, 0x8F4);
    }
    drop(c, 0x8FC);
    return ok;
}

// user/medal/enhance: points, the enhance result (kept), the component medals removed, the skills replaced
uint32_t verifyUserMedalEnhance(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_SKILL_PARSE, 0x8C0)) {
        if (parse(c, USER_POINT_PARSE, 0x8B8)) {
            if (parse(c, USER_MEDAL_ENHANCE_PARSE, 0x8B0)) {
                newIdSet(c, 0x89C);
                if (readIdSet(c, S_COMPONENT_USER_MEDAL_IDS, 0x89C, 0xC98, 0xD88)) {
                    const uint32_t enhance = c.F.word(0x8B0);
                    if (load32(enhance + ENHANCE_GREAT) == 0 && load32(enhance + ENHANCE_SP_BONUS) != 0)
                        sendEventOnce(c, EV_SP_ATTACK_BONUS);
                    keep(c, 0x8B8, off::USER_POINT);
                    keep(c, 0x8B0, off::USER_MEDAL_ENHANCE);
                    // the skills of the parsed list are removed (by id), then the result's skills applied
                    const uint32_t skills = c.F.word(0x8C0);
                    const uint32_t count = load32(skills + 0x20);
                    newIdSet(c, 0x888);
                    for (uint32_t k = 0, index = 0; k < count; k++, index += SKILL_SIZE) {
                        const uint32_t list = c.F.word(0x8C0);
                        c.F.call(SAFEBUFFER_READ, {list, c.F.at(0x860), load32(list + 12) + index, SKILL_SIZE, index});
                        c.F.call(ID_SET_INSERT_COPY, {c.F.at(0xD90), c.F.at(0x888), c.F.at(0x860)});
                    }
                    c.F.call(REMOVE_SKILLS, {addr(c.self), c.F.at(0x888)});
                    const uint32_t result = c.F.word(0x8B0);
                    const uint32_t last = load32(result + ENHANCE_SKILLS + 4);
                    for (uint32_t s = load32(result + ENHANCE_SKILLS); s != last; s += SKILL_SIZE)
                        c.F.call(UPDATE_SKILL, {addr(c.self), s});
                    c.F.call(UPDATE_MEDAL, {addr(c.self), c.F.word(0x8B0) + ENHANCE_MEDAL});
                    saveAppState(c);
                    c.F.call(REMOVE_MEDALS, {addr(c.self), c.F.at(0x89C)});
                    deleteIdSet(c, 0x888);
                    ok = 1;
                }
                deleteIdSet(c, 0x89C);
            }
            drop(c, 0x8B0);
        }
        drop(c, 0x8B8);
    }
    drop(c, 0x8C0);
    return ok;
}

// user/medal/evolve: points, the evolve result (kept), the attachment medals removed; a 6-star result sends
// "Enhance6Stars"
uint32_t verifyUserMedalEvolve(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x858)) {
        if (parse(c, USER_MEDAL_EVOLVE_PARSE, 0x850)) {
            newIdSet(c, 0x83C);
            if (readIdSet(c, S_ATTACHMENT_MEDAL_IDS, 0x83C, 0xCA0, 0xD98)) {
                c.F.call(MASTER_MEDAL_CREATE, {c.F.at(0x834), load32(c.F.word(0x850) + EVOLVE_MEDAL_ID)});
                const uint32_t medal = c.F.word(0x834);
                if (medal) {
                    c.F.call(SAFEBUFFER_READ, {medal, c.F.at(0xCAC), load32(medal + 12) + MEDAL_RARITY, 4, MEDAL_RARITY});
                    if (c.F.word(0xCAC) == 6) sendEventOnce(c, EV_ENHANCE_6_STARS);
                }
                keep(c, 0x858, off::USER_POINT);
                keep(c, 0x850, off::USER_MEDAL_EVOLVE);
                c.F.call(UPDATE_MEDAL, {addr(c.self), c.F.word(0x850) + ENHANCE_MEDAL});
                saveAppState(c);
                c.F.call(REMOVE_MEDALS, {addr(c.self), c.F.at(0x83C)});
                ok = 1;
                drop(c, 0x834);
            }
            deleteIdSet(c, 0x83C);
        }
        drop(c, 0x850);
    }
    drop(c, 0x858);
    return ok;
}

// skill/evolve: points, the medals and skills it changed, the consumed skills removed
uint32_t verifySkillEvolve(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x82C)) {
        if (parse(c, USER_MEDAL_PARSE, 0x824)) {
            if (parse(c, USER_SKILL_PARSE, 0x81C)) {
                newIdSet(c, 0x808);
                if (readIdSet(c, S_USER_SKILL_IDS, 0x808, 0xCB0, 0xDA0)) {
                    keep(c, 0x82C, off::USER_POINT);
                    applyRecords(c, 0x81C, SKILL_SIZE, 0x7E0, UPDATE_SKILL);
                    applyRecords(c, 0x824, MEDAL_SIZE, 0x13A8, UPDATE_MEDAL);
                    saveAppState(c);
                    c.F.call(REMOVE_SKILLS, {addr(c.self), c.F.at(0x808)});
                    ok = 1;
                }
                deleteIdSet(c, 0x808);
            }
            drop(c, 0x81C);
        }
        drop(c, 0x824);
    }
    drop(c, 0x82C);
    return ok;
}

uint32_t verifyMypage(const Ctx &c) { return parseAndKeep(c, MYPAGE_INFO_PARSE, 0x17C, off::MYPAGE_INFO); }

}  // namespace apiv
