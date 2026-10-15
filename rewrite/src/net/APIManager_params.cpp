// APIManager requests with a few scalar or string parameters. All of them are guarded (refused while another
// request runs or after a failure) and build a JSON object as the request parameters.
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t S_ACCESS_TOKEN = 0xDFC049;       // "accessToken"
constexpr uint32_t S_AGREE_TYPE = 0xDFB12E;         // "agreeType"
constexpr uint32_t S_CHANGE_GENDER = 0xDFBE55;      // "changeGender"
constexpr uint32_t S_COUNT = 0xDFB5EE;              // "count"
constexpr uint32_t S_DEVICE_TOKEN = 0xDFC029;       // "deviceToken"
constexpr uint32_t S_DEVICE_TYPE = 0xDFB94F;        // "deviceType"
constexpr uint32_t S_DRAW_MEDAL_TYPE_ID = 0xDFAE95; // "drawMedalTypeId"
constexpr uint32_t S_IS_NOTICE_AP = 0xDFA863;       // "isNoticeAp"
constexpr uint32_t S_IS_NOTICE_EVENT = 0xDFA848;    // "isNoticeEvent"
constexpr uint32_t S_IS_NOTICE_HELP = 0xDFA856;     // "isNoticeHelp"
constexpr uint32_t S_MEMBER_COUNT = 0xDFB122;       // "memberCount"
constexpr uint32_t S_MESSAGE = 0xDFAFCD;            // "message"
constexpr uint32_t S_NUMBER = 0xDFB260;             // "number"
constexpr uint32_t S_PARTY_ID = 0xDFAFD5;           // "partyId"
constexpr uint32_t S_PIN_CODE = 0xDFC035;           // "pinCode"
constexpr uint32_t S_PLATFORM_TYPE = 0xDFA83B;      // "platformType"
constexpr uint32_t S_PLAY_STYLE = 0xDFB138;         // "playStyle"
constexpr uint32_t S_RAID_ID = 0xDFB326;            // "raidId"
constexpr uint32_t S_RANK = 0xDFAA03;               // "rank"
constexpr uint32_t S_RELEASE_MASU_ID = 0xDFBF8E;    // "releaseMasuId"
constexpr uint32_t S_SERIAL_CODE = 0xDFC01E;        // "serialCode"
constexpr uint32_t S_SPHERE_BOARD_ID = 0xDFB29F;    // "sphereBoardId"
constexpr uint32_t S_STAGE_COUNT = 0xDFAEBE;        // "stageCount"
constexpr uint32_t S_STAGE_ID = 0xDFAEF5;           // "stageId"
constexpr uint32_t S_SUPPORT_MEDAL_ID = 0xDFBE76;   // "supportMedalId"
constexpr uint32_t S_SUPPORT_USER_ID = 0xDFB318;    // "supportUserId"
constexpr uint32_t S_TARGET_USER_ID = 0xDFBE48;     // "targetUserId"
constexpr uint32_t S_TITLE_LEFT_ID = 0xDFB431;      // "titleLeftId"
constexpr uint32_t S_TITLE_PLATE_ID = 0xDFB417;     // "titlePlateId"
constexpr uint32_t S_TITLE_RIGHT_ID = 0xDFB424;     // "titleRightId"
constexpr uint32_t S_TUTORIAL = 0xDFA7E4;           // "tutorial"
constexpr uint32_t S_UNION_ID = 0xDFAA08;           // "unionId"
constexpr uint32_t S_USER_ID = 0xDFA977;            // "userId"
constexpr uint32_t S_USER_KEYBLADE_ID = 0xDFAEE6;   // "userKeybladeId"
constexpr uint32_t S_USER_MATERIAL_ID = 0xDFB657;   // "userMaterialId"

// the common frame layouts
constexpr RequestSlots SLOTS_68{0x28, 0x30, 0x20};    // 0x68-byte frame
constexpr RequestSlots SLOTS_70{0x30, 0x38, 0x28};    // 0x70-byte frame
constexpr RequestSlots SLOTS_78{0x38, 0x40, 0x30};    // 0x78-byte frame
constexpr RequestSlots SLOTS_STR{0x30, 0x58, 0x28};   // 0x90-byte frame (string values at 0x38, names at 0x48)

constexpr uint32_t STR_VALUE = 0x38, STR_NAME = 0x48;

// The guarded start of a request with parameters: false when refused; else the parameter document.
bool begin(const Frame &F, APIManager *self, uint32_t cb, uint32_t docSlot, uint32_t &doc) {
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    doc = newParams(F, docSlot);
    return true;
}

}  // namespace

// ---------------------------------------------------------------------------------------------- user
bool APIManager::executeUserSupportUpdate(APIManager *self, uint64_t supportMedalId, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    addU64(F, doc, S_SUPPORT_MEDAL_ID, supportMedalId, allocatorOf(doc));
    return sendWithParams(F, self, 45, cb, 1, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4C9454, "APIManager::executeUserSupportUpdate", APIManager::executeUserSupportUpdate);

bool APIManager::executeKeybladeEnhance(APIManager *self, uint64_t userKeybladeId, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    addU64(F, doc, S_USER_KEYBLADE_ID, userKeybladeId, allocatorOf(doc));
    return sendWithParams(F, self, 52, cb, 1, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4C9F74, "APIManager::executeKeybladeEnhance", APIManager::executeKeybladeEnhance);

// user/avatar/get: another user's avatar ({"targetUserId"}), or the own one (no parameter) when 0.
bool APIManager::executeUserAvatarGet(APIManager *self, uint64_t targetUserId, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    if (targetUserId != 0) addU64(F, doc, S_TARGET_USER_ID, targetUserId, alloc);
    return sendWithParams(F, self, 19, cb, 1, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4C81B0, "APIManager::executeUserAvatarGet", APIManager::executeUserAvatarGet);

// user/profile/get: {"userId"} when not 0, {"platformType"} when not 0.
bool APIManager::executeUserProfileGet(APIManager *self, uint64_t userId, uint32_t platformType, uint32_t cb) {
    Frame F(0x70);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_70.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    if (userId != 0) addU64(F, doc, S_USER_ID, userId, alloc);
    if (platformType != 0) addU8(F, doc, S_PLATFORM_TYPE, platformType, alloc);
    return sendWithParams(F, self, 4, cb, 0, SLOTS_70);
}
RW_FUNCTION_NOVERIFY(0x4C7F64, "APIManager::executeUserProfileGet", APIManager::executeUserProfileGet);

// user/gender/update: {"changeGender": 2 for 2, else 1}
bool APIManager::executeUserGenderUpdate(APIManager *self, int32_t gender, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    addI32(F, doc, S_CHANGE_GENDER, gender == 2 ? 2 : 1, allocatorOf(doc));
    return sendWithParams(F, self, 37, cb, 1, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4C8584, "APIManager::executeUserGenderUpdate", APIManager::executeUserGenderUpdate);

// user/title/update: the left, right and plate titles
bool APIManager::executeUserTitleUpdate(APIManager *self, uint64_t left, uint64_t right, uint64_t plate, uint32_t cb) {
    Frame F(0x78);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_78.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addU64(F, doc, S_TITLE_LEFT_ID, left, alloc);
    addU64(F, doc, S_TITLE_RIGHT_ID, right, alloc);
    addU64(F, doc, S_TITLE_PLATE_ID, plate, alloc);
    return sendWithParams(F, self, 39, cb, 1, SLOTS_78);
}
RW_FUNCTION_NOVERIFY(0x4C896C, "APIManager::executeUserTitleUpdate", APIManager::executeUserTitleUpdate);

bool APIManager::executeUserMaterialSell(APIManager *self, uint64_t userMaterialId, int32_t number, uint32_t cb) {
    Frame F(0x70);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_70.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addU64(F, doc, S_USER_MATERIAL_ID, userMaterialId, alloc);
    addI32(F, doc, S_NUMBER, uint32_t(number), alloc);
    return sendWithParams(F, self, 48, cb, 1, SLOTS_70);
}
RW_FUNCTION_NOVERIFY(0x4C986C, "APIManager::executeUserMaterialSell", APIManager::executeUserMaterialSell);

// user/sphere/update: the board and the first cell of the set to release (the int argument is unused)
bool APIManager::executeUserSphereUpdate(APIManager *self, int32_t sphereBoardId, uint32_t masuIds, uint32_t unused,
                                         uint32_t cb) {
    (void)unused;
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_SPHERE_BOARD_ID, uint32_t(sphereBoardId), alloc);
    uint32_t firstNode = load32(masuIds + 8);                 // unordered_set<int>: first node, value at +8
    addI32(F, doc, S_RELEASE_MASU_ID, load32(firstNode + 8), alloc);
    return sendWithParams(F, self, 42, cb, 1, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4CCD68, "APIManager::executeUserSphereUpdate", APIManager::executeUserSphereUpdate);

// user/option/update: the three notification switches of a UserOption (bytes 0..2)
bool APIManager::executeUserOptionUpdate(APIManager *self, uint32_t option, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_IS_NOTICE_AP, load8(option + 0), alloc);
    addI32(F, doc, S_IS_NOTICE_HELP, load8(option + 1), alloc);
    addI32(F, doc, S_IS_NOTICE_EVENT, load8(option + 2), alloc);
    return sendWithParams(F, self, 138, cb, 1, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4D3DC8, "APIManager::executeUserOptionUpdate", APIManager::executeUserOptionUpdate);

bool APIManager::executeUnionChange(APIManager *self, int32_t unionId, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    addI32(F, doc, S_UNION_ID, uint32_t(unionId), allocatorOf(doc));
    return sendWithParams(F, self, 69, cb, 0, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4CE224, "APIManager::executeUnionChange", APIManager::executeUnionChange);

// ---------------------------------------------------------------------------------------------- gacha
// gacha/list: {"tutorial": 1} during the tutorial
bool APIManager::executeGachaList(APIManager *self, uint32_t tutorial, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    if (tutorial == 1) addI32(F, doc, S_TUTORIAL, 1, alloc);
    return sendWithParams(F, self, 70, cb, 1, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4CE3D4, "APIManager::executeGachaList", APIManager::executeGachaList);

bool APIManager::executeGachaDraw(APIManager *self, int32_t drawMedalTypeId, int32_t count, uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_68.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_DRAW_MEDAL_TYPE_ID, uint32_t(drawMedalTypeId), alloc);
    addI32(F, doc, S_COUNT, uint32_t(count), alloc);
    return sendWithParams(F, self, 71, cb, 0, SLOTS_68);
}
RW_FUNCTION_NOVERIFY(0x4CE578, "APIManager::executeGachaDraw", APIManager::executeGachaDraw);

// ---------------------------------------------------------------------------------------------- raid / colosseum
bool APIManager::executeRaidStart(APIManager *self, uint64_t raidId, uint64_t supportUserId, uint64_t userKeybladeId,
                                  uint32_t cb) {
    Frame F(0x80);
    uint32_t doc;
    const RequestSlots S{0x40, 0x48, 0x38};
    if (!begin(F, self, cb, S.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addU64(F, doc, S_RAID_ID, raidId, alloc);
    addU64(F, doc, S_SUPPORT_USER_ID, supportUserId, alloc);
    addU64(F, doc, S_USER_KEYBLADE_ID, userKeybladeId, alloc);
    return sendWithParams(F, self, 112, cb, 0, S);
}
RW_FUNCTION_NOVERIFY(0x4CC580, "APIManager::executeRaidStart", APIManager::executeRaidStart);

bool APIManager::executeColosseumStart(APIManager *self, int32_t rank, int32_t stageCount, int32_t stageId,
                                       uint64_t userKeybladeId, uint32_t cb) {
    Frame F(0x78);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_78.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_RANK, uint32_t(rank), alloc);
    addI32(F, doc, S_STAGE_COUNT, uint32_t(stageCount), alloc);
    addI32(F, doc, S_STAGE_ID, uint32_t(stageId), alloc);
    addU64(F, doc, S_USER_KEYBLADE_ID, userKeybladeId, alloc);
    return sendWithParams(F, self, 116, cb, 0, SLOTS_78);
}
RW_FUNCTION_NOVERIFY(0x4D15AC, "APIManager::executeColosseumStart", APIManager::executeColosseumStart);

// ---------------------------------------------------------------------------------------------- party
bool APIManager::executePartySearchActive(APIManager *self, int32_t playStyle, int32_t rank, int32_t agreeType,
                                          int32_t memberCount, uint32_t cb) {
    Frame F(0x70);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_70.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_PLAY_STYLE, uint32_t(playStyle), alloc);
    addI32(F, doc, S_RANK, uint32_t(rank), alloc);
    addI32(F, doc, S_AGREE_TYPE, uint32_t(agreeType), alloc);
    addI32(F, doc, S_MEMBER_COUNT, uint32_t(memberCount), alloc);
    return sendWithParams(F, self, 81, cb, 1, SLOTS_70);
}
RW_FUNCTION_NOVERIFY(0x4CF0DC, "APIManager::executePartySearchActive", APIManager::executePartySearchActive);

bool APIManager::executePartyApplication(APIManager *self, uint64_t partyId, uint32_t message, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_STR.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addU64(F, doc, S_PARTY_ID, partyId, alloc);
    addString(F, doc, S_MESSAGE, 7, message, alloc, STR_VALUE, STR_NAME);
    return sendWithParams(F, self, 82, cb, 0, SLOTS_STR);
}
RW_FUNCTION_NOVERIFY(0x4CF2C8, "APIManager::executePartyApplication", APIManager::executePartyApplication);

bool APIManager::executePartyInviteOffer(APIManager *self, uint64_t userId, uint32_t message, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_STR.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addU64(F, doc, S_USER_ID, userId, alloc);
    addString(F, doc, S_MESSAGE, 7, message, alloc, STR_VALUE, STR_NAME);
    return sendWithParams(F, self, 93, cb, 0, SLOTS_STR);
}
RW_FUNCTION_NOVERIFY(0x4D09A4, "APIManager::executePartyInviteOffer", APIManager::executePartyInviteOffer);

// ---------------------------------------------------------------------------------------------- system / account
bool APIManager::executeSystemSerialcode(APIManager *self, uint32_t serialCode, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_STR.doc, doc)) return false;
    addString(F, doc, S_SERIAL_CODE, 10, serialCode, allocatorOf(doc), STR_VALUE, STR_NAME);
    return sendWithParams(F, self, 31, cb, 0, SLOTS_STR);
}
RW_FUNCTION_NOVERIFY(0x4D1D64, "APIManager::executeSystemSerialcode", APIManager::executeSystemSerialcode);

bool APIManager::executeCampaignPincodeUpdate(APIManager *self, uint32_t pinCode, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_STR.doc, doc)) return false;
    addString(F, doc, S_PIN_CODE, 7, pinCode, allocatorOf(doc), STR_VALUE, STR_NAME);
    return sendWithParams(F, self, 134, cb, 0, SLOTS_STR);
}
RW_FUNCTION_NOVERIFY(0x4D397C, "APIManager::executeCampaignPincodeUpdate", APIManager::executeCampaignPincodeUpdate);

// system/push/regist: {"deviceType": 2 (Android), "deviceToken"}
bool APIManager::executeSystemPushRegist(APIManager *self, uint32_t deviceToken, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_STR.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_DEVICE_TYPE, 2, alloc);
    addString(F, doc, S_DEVICE_TOKEN, 11, deviceToken, alloc, STR_VALUE, STR_NAME);
    return sendWithParams(F, self, 34, cb, 0, SLOTS_STR);
}
RW_FUNCTION_NOVERIFY(0x4D21B0, "APIManager::executeSystemPushRegist", APIManager::executeSystemPushRegist);

namespace {
// {"accessToken", "platformType"}: user/link/update, user/link/delete, system/uuid/update
bool accountLinkRequest(APIManager *self, int32_t action, uint32_t accessToken, uint32_t platformType, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, SLOTS_STR.doc, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addString(F, doc, S_ACCESS_TOKEN, 11, accessToken, alloc, STR_VALUE, STR_NAME);
    addU8(F, doc, S_PLATFORM_TYPE, platformType, alloc);
    return sendWithParams(F, self, action, cb, 0, SLOTS_STR);
}
}  // namespace

bool APIManager::executeUserLinkUpdate(APIManager *self, uint32_t accessToken, uint32_t platformType, uint32_t cb) {
    return accountLinkRequest(self, 141, accessToken, platformType, cb);
}
RW_FUNCTION_NOVERIFY(0x4D41CC, "APIManager::executeUserLinkUpdate", APIManager::executeUserLinkUpdate);

bool APIManager::executeUserLinkDelete(APIManager *self, uint32_t accessToken, uint32_t platformType, uint32_t cb) {
    return accountLinkRequest(self, 142, accessToken, platformType, cb);
}
RW_FUNCTION_NOVERIFY(0x4D43B0, "APIManager::executeUserLinkDelete", APIManager::executeUserLinkDelete);

bool APIManager::executeSystemUuidUpdate(APIManager *self, uint32_t accessToken, uint32_t platformType, uint32_t cb) {
    return accountLinkRequest(self, 143, accessToken, platformType, cb);
}
RW_FUNCTION_NOVERIFY(0x4D4594, "APIManager::executeSystemUuidUpdate", APIManager::executeSystemUuidUpdate);
