// APIManager::verifyResponse: system, login and tutorial replies, the sphere board check, shop, gacha and purchase.
#include "APIManager_verify.hpp"
#include "master/DrawMedalType.hpp"

namespace apiv {

namespace {

constexpr uint32_t MASTER_DATA_GET_INSTANCE = 0x76C091;     // MasterDataManager::getInstance
constexpr uint32_t MASTER_DATA_UPDATE = 0x76D0C9;           // MasterDataManager::update(const Document&) -> bool
constexpr uint32_t RESOURCE_DATA_GET_INSTANCE = 0x7728AD;   // ResourceDataManager::getInstance
constexpr uint32_t RESOURCE_DATA_UPDATE = 0x773779;         // ResourceDataManager::update(const Document&)
constexpr uint32_t RESOURCE_DATA_DOWNLOAD_EVENT = 0x773A91; // ResourceDataManager::downloadEventResources

constexpr uint32_t S_USER_DATA = 0xDFA622;                  // "userData"
constexpr uint32_t S_USER_STONE = 0xDFA66C;                 // "userStone"
constexpr uint32_t S_USER_SPHERE = 0xDFA745;                // "userSphere"
constexpr uint32_t S_INFO_BODY = 0xDFBCB5;                  // "infoBody"
constexpr uint32_t S_ACTIVE_CAMPAIGN_IDS = 0xDFC06A;        // "activeCampaignIds"
constexpr uint32_t S_CHECK_TYPE = 0xDFBCAB;                 // "checkType"
constexpr uint32_t S_CLOSE_EVENT_SPHERE_BOARD_IDS = 0xDFC0AE; // "closeEventSphereBoardIds"
constexpr uint32_t S_PROGRESSION = 0xDFADEB;                // "progression"
constexpr uint32_t S_DRAW_MEDAL_TYPE_ID = 0xDFAE95;         // "drawMedalTypeId"
constexpr uint32_t S_RECEIPT_STATUS = 0xDFC08D;             // "receiptStatus"
constexpr uint32_t EV_5_SUCCESSIVE_DAYS = 0xDFBBBA;         // "5SuccessiveDays"
constexpr uint32_t EV_SUCCESSFUL_DL = 0xDFBBAD;             // "SuccessfulDL"
constexpr uint32_t EV_JEWELS_FOR_MEDALS = 0xDFBCBE;         // "JewelsForMedals"
constexpr uint32_t EV_BONUS_JEWELS = 0xDFBCCE;              // "BonusJewels"

constexpr uint32_t LOGIN_BONUS_DAYS = 0x0C;                 // LoginBonus: consecutive login days
constexpr uint32_t PROGRESSION_DOWNLOADED = 3;              // tutorial progression: the resources are downloaded
constexpr uint32_t DRAW_TYPE_SKIP = 5;                      // a draw type that never counts as jewels spent
constexpr uint32_t PAY_TYPE_JEWELS = 1;
// GachaDraw: the record lists it carries
constexpr uint32_t DRAW_MEDALS = 0x00;
constexpr uint32_t DRAW_SKILLS = 0x08;
constexpr uint32_t DRAW_MATERIALS = 0x10;
// UserShop: std::vector<Benefit> (12 bytes each) at +0
constexpr uint32_t BENEFIT_SIZE = 12;
// the sphere boards user/sphere/check reported (node + 0x18 of the opened spheres' map: the board id)
constexpr uint32_t SPHERE_NODE_SPHERE = 0x14;
constexpr uint32_t SPHERE_NODE_BOARD_ID = 0x18;

}  // namespace

uint32_t verifySystemNeedUrl(const Ctx &c) {
    return parseAndKeep(c, SYSTEM_NEED_URL_PARSE, 0x460, off::SYSTEM_NEED_URL);
}
uint32_t verifySystemCoppa(const Ctx &c) { return parseAndKeep(c, SYSTEM_COPPA_PARSE, 0x458, off::SYSTEM_COPPA); }

// system/master, system/resource, system/resourceEv: the data managers check (and store) the reply themselves
uint32_t verifySystemMaster(const Ctx &c) {
    return c.F.call(MASTER_DATA_UPDATE, {c.F.call(MASTER_DATA_GET_INSTANCE, {}), c.json});
}
uint32_t verifySystemResource(const Ctx &c) {
    return c.F.call(RESOURCE_DATA_UPDATE, {c.F.call(RESOURCE_DATA_GET_INSTANCE, {}), c.json});
}
uint32_t verifySystemResourceEv(const Ctx &c) {
    return c.F.call(RESOURCE_DATA_DOWNLOAD_EVENT, {c.F.call(RESOURCE_DATA_GET_INSTANCE, {}), c.json});
}

uint32_t verifySystemInformationList(const Ctx &c) {
    return parseAndKeep(c, SYSTEM_INFORMATION_LIST_PARSE, 0x450, off::SYSTEM_INFORMATION_LIST);
}
uint32_t verifySystemInformationList151203(const Ctx &c) {
    return parseAndKeep(c, SYSTEM_INFORMATION_LIST_151203_PARSE, 0x448, off::SYSTEM_INFORMATION_LIST_151203);
}

// system/information/detail: the reply's infoBody string is kept
uint32_t verifySystemInformationDetail(const Ctx &c) {
    if (jsonFlags(c.json) != JSON_OBJECT) return 0;
    const uint32_t body = jsonMember(c, c.json, S_INFO_BODY);
    if (!(jsonFlags(body) & JSON_FLAG_STRING)) return 0;
    const uint32_t chars = load32(body);
    const uint32_t n = c.F.call(STRLEN, {chars});
    c.F.call(STRING_ASSIGN_CHARS, {addr(c.self) + off::INFORMATION_BODY, chars, n});
    return 1;
}

// system/serialcode: the presents the code gave
uint32_t verifySystemSerialcode(const Ctx &c) { return parseAndKeep(c, USER_PRESENT_PARSE, 0x2A8, off::USER_PRESENT); }

// system/serialcode/campaign: activeCampaignIds (ints) replace the manager's list
uint32_t verifySystemSerialcodeCampaign(const Ctx &c) {
    return readIntList(c, S_ACTIVE_CAMPAIGN_IDS, 0x29C, 0xCE0, c.self->ints_270) ? 1 : 0;   // off::ACTIVE_CAMPAIGN_IDS
}

uint32_t verifySystemInvitecode(const Ctx &c) {
    return parseAndKeep(c, SYSTEM_INVITECODE_PARSE, 0x294, off::SYSTEM_INVITECODE);
}

// user/sphere/check: the request's checkType 1 / 2 reports opened spheres (set into the board, their board ids
// collected), anything else the closed event boards (removed). The board ids replace the manager's list and each
// is announced to AppState as a new avatar board.
uint32_t verifyUserSphereCheck(const Ctx &c) {
    const uint32_t checkType = jsonMember(c, c.self->params.ptr, S_CHECK_TYPE);
    if (!(jsonFlags(checkType) & JSON_FLAG_INT)) return 0;
    const uint32_t ids = c.F.at(0x5DC);                    // std::vector<int>
    c.F.setWord(0x5DC, 0);
    c.F.setWord(0x5E0, 0);
    c.F.setWord(0x5E4, 0);
    StdSharedPtr &board = member(c.self, off::USER_SPHERE);
    bool ok = true;
    if (load32(checkType) - 1 <= 1) {
        if (parse(c, USER_SPHERE_PARSE, 0x5D4, 1)) {
            const uint32_t spheres = c.F.word(0x5D4);
            const uint32_t end = spheres + 4;
            for (uint32_t node = load32(spheres); node != end; node = treeNext(node)) {
                c.F.call(USER_SPHERE_SET, {board.ptr, node + SPHERE_NODE_SPHERE});
                pushBackCopy(c.F, ids, node + SPHERE_NODE_BOARD_ID, 4, INT_VECTOR_PUSH_SLOW_COPY);
            }
        } else {
            ok = false;
        }
        drop(c, 0x5D4);
    } else {
        const uint32_t array = jsonMember(c, c.json, S_CLOSE_EVENT_SPHERE_BOARD_IDS);
        if (jsonFlags(array) != JSON_ARRAY) {
            ok = false;
        } else {
            const uint32_t n = jsonArraySize(array);
            for (uint32_t k = 0; k < n && ok; k++) {
                const uint32_t e = jsonArrayAt(array, k);
                if (!(jsonFlags(e) & JSON_FLAG_INT)) {
                    ok = false;
                    break;
                }
                c.F.setWord(0xCE4, load32(e));
                pushBackCopy(c.F, ids, c.F.at(0xCE4), 4, INT_VECTOR_PUSH_SLOW, true);
            }
            if (ok) {
                const uint32_t last = c.F.word(0x5E0);
                for (uint32_t p = c.F.word(0x5DC); p != last; p += 4)
                    c.F.call(USER_SPHERE_REMOVE, {board.ptr, load32(p)});
            }
        }
    }
    uint32_t result = 0;
    if (ok) {
        StdVector &kept = c.self->ints_308;                  // off::SPHERE_CHECK_BOARD_IDS
        c.F.call(INT_VECTOR_DEALLOCATE, {addr(&kept)});
        kept.begin = c.F.word(0x5DC);
        kept.end = c.F.word(0x5E0);
        kept.cap = c.F.word(0x5E4);
        c.F.setWord(0x5DC, 0);
        c.F.setWord(0x5E0, 0);
        c.F.setWord(0x5E4, 0);
        result = 1;
        if (kept.end != kept.begin) {
            for (uint32_t k = 0;;) {
                const uint32_t app = c.F.call(APPSTATE_GET_INSTANCE, {});
                if (uint32_t(int32_t(kept.end - kept.begin) >> 2) <= k) {   // ids.at(k)
                    c.F.call(VECTOR_THROW_OUT_OF_RANGE, {addr(&kept)});
                    break;
                }
                c.F.call(APPSTATE_ADD_NEW_AVATAR_BOARD_IDS, {app, load32(kept.begin + 4 * k)});
                k++;
                if (k >= uint32_t(int32_t(kept.end - kept.begin) >> 2)) break;
            }
        }
    }
    c.F.call(INT_VECTOR_DTOR, {ids});
    return result;
}

// login, tutorial/user/create
uint32_t verifyLogin(const Ctx &c) { return parseAndKeep(c, LOGIN_PARSE, 0xC50, off::LOGIN); }

// login/bonus: the fifth consecutive day sends "5SuccessiveDays"
uint32_t verifyLoginBonus(const Ctx &c) {
    const uint32_t bonus = parse(c, LOGIN_BONUS_PARSE, 0xC28);
    if (!bonus) {
        drop(c, 0xC28);
        return 0;
    }
    if (load32(bonus + LOGIN_BONUS_DAYS) == 5) sendEventOnce(c, EV_5_SUCCESSIVE_DAYS);
    keep(c, 0xC28, off::LOGIN_BONUS);
    drop(c, 0xC28);
    return 1;
}

// tutorial/progress: the request's progression 3 (resources downloaded) sends "SuccessfulDL"
uint32_t verifyTutorialProgress(const Ctx &c) {
    if (!parse(c, TUTORIAL_PROGRESS_PARSE, 0xC48)) {
        drop(c, 0xC48);
        return 0;
    }
    const uint32_t progression = jsonMember(c, c.self->params.ptr, S_PROGRESSION);
    if ((jsonFlags(progression) & JSON_FLAG_INT) && load32(progression) == PROGRESSION_DOWNLOADED)
        sendEventOnce(c, EV_SUCCESSFUL_DL);
    keep(c, 0xC48, off::TUTORIAL_PROGRESS);
    drop(c, 0xC48);
    return 1;
}

// tutorial/clear: the login and the clear rewards
uint32_t verifyTutorialClear(const Ctx &c) {
    parse(c, LOGIN_PARSE, 0xC40);
    parse(c, TUTORIAL_CLEAR_PARSE, 0xC38);
    uint32_t ok = 0;
    if (c.F.word(0xC40) && c.F.word(0xC38)) {
        keep(c, 0xC40, off::LOGIN);
        keep(c, 0xC38, off::TUTORIAL_CLEAR);
        ok = 1;
    }
    drop(c, 0xC38);
    drop(c, 0xC40);
    return ok;
}

uint32_t verifyTutorialStatus(const Ctx &c) { return parseAndKeep(c, TUTORIAL_STATUS_PARSE, 0xC30, off::TUTORIAL_STATUS); }

// two parts that must both parse: kept in order
static uint32_t keepBoth(const Ctx &c, uint32_t parserA, uint32_t slotA, uint32_t offA, uint32_t parserB, uint32_t slotB,
                         uint32_t offB) {
    parse(c, parserA, slotA);
    parse(c, parserB, slotB);
    uint32_t ok = 0;
    if (c.F.word(slotA) && c.F.word(slotB)) {
        keep(c, slotA, offA);
        keep(c, slotB, offB);
        ok = 1;
    }
    drop(c, slotB);
    drop(c, slotA);
    return ok;
}

// shop/medal/max: detail (medal box size) and stones
uint32_t verifyShopMedalMax(const Ctx &c) {
    return keepBoth(c, USER_DETAIL_PARSE, 0x440, off::USER_DETAIL, USER_STONE_PARSE, 0x438, off::USER_STONE);
}

// shop/ap/recover: points, stones and the keyblades (applied)
uint32_t verifyShopApRecover(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x430)) {
        if (parse(c, USER_STONE_PARSE, 0x428)) {
            if (parse(c, USER_KEYBLADE_PARSE, 0x420)) {
                keep(c, 0x430, off::USER_POINT);
                keep(c, 0x428, off::USER_STONE);
                applyRecords(c, 0x420, KEYBLADE_SIZE, 0xFD8, UPDATE_KEYBLADE);
                ok = 1;
            }
            drop(c, 0x420);
        }
        drop(c, 0x428);
    }
    drop(c, 0x430);
    return ok;
}

// user/sphere/buy: stones and the bought sphere (userSphere, set into the board)
uint32_t verifyUserSphereBuy(const Ctx &c) {
    parse(c, USER_STONE_PARSE, 0x418);
    const uint32_t sphere = c.F.at(0x3F8);                 // UserSphere::Sphere, its std::vector<int> at +0x14
    c.F.setWord(0x40C, 0);
    c.F.setWord(0x410, 0);
    c.F.setWord(0x414, 0);
    const uint32_t initFailed = c.F.call(SPHERE_INIT, {sphere, jsonMember(c, c.json, S_USER_SPHERE)}) ^ 1;
    uint32_t ok = 0;
    if (c.F.word(0x418) && !initFailed) {
        keep(c, 0x418, off::USER_STONE);
        c.F.call(USER_SPHERE_SET, {member(c.self, off::USER_SPHERE).ptr, sphere});
        ok = 1;
    }
    c.F.call(INT_VECTOR_DTOR, {sphere + 0x14});
    drop(c, 0x418);
    return ok;
}

// union/change: detail and stones
uint32_t verifyUnionChange(const Ctx &c) {
    return keepBoth(c, USER_DETAIL_PARSE, 0x3F0, off::USER_DETAIL, USER_STONE_PARSE, 0x3E8, off::USER_STONE);
}

uint32_t verifyGachaList(const Ctx &c) { return parseAndKeep(c, GACHA_LIST_PARSE, 0x3E0, off::GACHA_LIST); }

// gacha/draw: the drawn medals, skills and materials (applied); points / stones when the reply has them. A draw
// paid with jewels sends "JewelsForMedals".
uint32_t verifyGachaDraw(const Ctx &c) {
    parse(c, USER_POINT_PARSE, 0x3D8);
    parse(c, USER_STONE_PARSE, 0x3D0);
    parse(c, GACHA_DRAW_PARSE, 0x3C8);
    const uint32_t userDataFlags = jsonFlags(jsonMember(c, c.json, S_USER_DATA));
    const uint32_t userStone = jsonMember(c, c.json, S_USER_STONE);
    uint32_t ok = 0;
    if (c.F.word(0x3C8) && !(userDataFlags == JSON_OBJECT && !c.F.word(0x3D8)) &&
        !(jsonFlags(userStone) == JSON_OBJECT && !c.F.word(0x3D0))) {
        const uint32_t typeId = jsonMember(c, c.self->params.ptr, S_DRAW_MEDAL_TYPE_ID);
        if (jsonFlags(typeId) & JSON_FLAG_INT) {
            c.F.call(MASTER_DRAW_MEDAL_TYPE_CREATE, {c.F.at(0x3C0), load32(typeId)});
            const uint32_t type = c.F.word(0x3C0);
            if (type) {
                using master::DrawMedalType;
                c.F.call(SAFEBUFFER_READ, {type, c.F.at(0xD00), load32(type + 12) + DrawMedalType::DRAW_TYPE, 4,
                                           DrawMedalType::DRAW_TYPE});
                if (c.F.word(0xD00) != DRAW_TYPE_SKIP) {
                    const uint32_t t = c.F.word(0x3C0);
                    c.F.call(SAFEBUFFER_READ, {t, c.F.at(0xD04), load32(t + 12) + DrawMedalType::PAY_TYPE, 4,
                                               DrawMedalType::PAY_TYPE});
                    if (c.F.word(0xD04) == PAY_TYPE_JEWELS) sendEventOnce(c, EV_JEWELS_FOR_MEDALS);
                }
            }
            drop(c, 0x3C0);
        }
        if (c.F.word(0x3D8)) keep(c, 0x3D8, off::USER_POINT);
        if (c.F.word(0x3D0)) keep(c, 0x3D0, off::USER_STONE);
        keep(c, 0x3C8, off::GACHA_DRAW);
        const uint32_t draw = c.F.word(0x3C8);
        applyRecordsAt(c, draw + DRAW_MATERIALS, MATERIAL_SIZE, 0x3B0, UPDATE_MATERIAL);
        applyRecordsAt(c, c.F.word(0x3C8) + DRAW_MEDALS, MEDAL_SIZE, 0xF90, UPDATE_MEDAL);
        saveAppState(c);
        applyRecordsAt(c, c.F.word(0x3C8) + DRAW_SKILLS, SKILL_SIZE, 0x388, UPDATE_SKILL);
        ok = 1;
    }
    drop(c, 0x3C8);
    drop(c, 0x3D0);
    drop(c, 0x3D8);
    return ok;
}

// purchase/prepare
uint32_t verifyPurchasePrepare(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, PURCHASE_PREPARE_PARSE, 0x378, S_USER_DATA)) {
        keep(c, 0x378, off::PURCHASE_PREPARE);
        ok = 1;
    }
    drop(c, 0x378);
    return ok;
}

// purchase/finish: points, stones, payment info, the receipt status; the shop's new benefits are added to the
// cached shop (more benefits than before sends "BonusJewels")
uint32_t verifyPurchaseFinish(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, USER_POINT_PARSE, 0x370)) {
        if (parse(c, USER_STONE_PARSE, 0x368)) {
            if (parse(c, PAYMENT_INFO_PARSE, 0x360)) {
                if (parse(c, USER_SHOP_PARSE, 0x358)) {
                    const uint32_t status = jsonMember(c, c.json, S_RECEIPT_STATUS);
                    if (jsonFlags(status) & JSON_FLAG_INT) {
                        c.self->word_248 = load32(status);       // off::RECEIPT_STATUS
                        keep(c, 0x370, off::USER_POINT);
                        keep(c, 0x368, off::USER_STONE);
                        keep(c, 0x360, off::PAYMENT_INFO);
                        StdSharedPtr &shop = member(c.self, off::USER_SHOP);
                        const uint32_t had = load32(shop.ptr + 4) - load32(shop.ptr);
                        const uint32_t parsed = c.F.word(0x358);
                        if (load32(parsed + 4) - load32(parsed) > had) c.F.call(TRACKING_SEND, {EV_BONUS_JEWELS, 0});
                        const uint32_t first = load32(c.F.word(0x358)), last = load32(c.F.word(0x358) + 4);
                        for (uint32_t b = first; b != last; b += BENEFIT_SIZE)
                            c.F.call(USER_SHOP_ADD_BENEFIT, {shop.ptr, b});
                        ok = 1;
                    }
                }
                drop(c, 0x358);
            }
            drop(c, 0x360);
        }
        drop(c, 0x368);
    }
    drop(c, 0x370);
    return ok;
}

}  // namespace apiv
