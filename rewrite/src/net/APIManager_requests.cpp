// APIManager requests that follow one of the common shapes (see the helpers in APIManager.hpp): which action,
// which guard, and at most one parameter.
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t MASTER_DATA_MANAGER_GET_INSTANCE = 0x76C091;
constexpr uint32_t MASTER_DATA_MANAGER_RESET = 0x76D0B5;
constexpr uint32_t RESOURCE_DATA_MANAGER_GET_INSTANCE = 0x7728AD;
constexpr uint32_t RESOURCE_DATA_MANAGER_RESET = 0x77376D;

// the next request of the login/user-data chain: sent even while connecting (it is the running connection)
bool chainRequest(APIManager *self, int32_t action, uint32_t cb) {
    Frame F(0x60);
    self->retryable = 1;
    resetShared(F, self->params);
    sendRequest(F, self, action, cb, 0x28, 0x20);
    return true;
}

bool requestWithoutParams(APIManager *self, int32_t action, uint32_t cb, uint8_t retryable) {
    Frame F(0x60);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    self->retryable = retryable;
    resetShared(F, self->params);
    sendRequest(F, self, action, cb, 0x28, 0x20);
    return true;
}

bool requestWithEmptyParams(APIManager *self, int32_t action, uint32_t cb) {
    Frame F(0x70);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    newParams(F, 0x30);
    self->retryable = 1;
    uint32_t ctrl = keepParams(F, self, 0x30);
    sendRequest(F, self, action, cb, 0x38, 0x28);
    releaseIfSet(F, ctrl);
    return true;
}

bool requestWithId(APIManager *self, int32_t action, uint32_t key, uint64_t id, uint32_t cb) {
    Frame F(0x68);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    uint32_t doc = newParams(F, 0x28);
    addU64(F, doc, key, id, allocatorOf(doc));
    self->retryable = 0;
    uint32_t ctrl = keepParams(F, self, 0x28);
    sendRequest(F, self, action, cb, 0x30, 0x20);
    releaseIfSet(F, ctrl);
    return true;
}

// add: AddMember<int> / <unsigned> / <unsigned char> (the value register is passed through unchanged)
bool requestWithInt(const Frame &F, APIManager *self, int32_t action, uint32_t key, uint32_t value, uint32_t cb,
                    uint32_t add) {
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    uint32_t doc = newParams(F, 0x28);
    F.call(add, {doc, key, value, allocatorOf(doc)});
    self->retryable = 1;
    uint32_t ctrl = keepParams(F, self, 0x28);
    sendRequest(F, self, action, cb, 0x30, 0x20);
    releaseIfSet(F, ctrl);
    return true;
}

}  // namespace

// ---- chain requests (no guard, no parameters): the login/user-data chain
bool APIManager::executeTutorialUserCreate(APIManager *self, uint32_t cb) {
    return chainRequest(self, 61, cb);
}
RW_FUNCTION_NOVERIFY(0x4C5588, "APIManager::executeTutorialUserCreate", APIManager::executeTutorialUserCreate);

bool APIManager::executeUserShopGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 7, cb);
}
RW_FUNCTION_NOVERIFY(0x4C6CD4, "APIManager::executeUserShopGet", APIManager::executeUserShopGet);

bool APIManager::executeUserOptionGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 137, cb);
}
RW_FUNCTION_NOVERIFY(0x4C6DE4, "APIManager::executeUserOptionGet", APIManager::executeUserOptionGet);

bool APIManager::executeTutorialStatusGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 64, cb);
}
RW_FUNCTION_NOVERIFY(0x4C6EF4, "APIManager::executeTutorialStatusGet", APIManager::executeTutorialStatusGet);

bool APIManager::executeUserSphereGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 9, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7004, "APIManager::executeUserSphereGet", APIManager::executeUserSphereGet);

bool APIManager::executeUserMedalGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 13, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7114, "APIManager::executeUserMedalGet", APIManager::executeUserMedalGet);

bool APIManager::executeUserSkillGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 11, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7224, "APIManager::executeUserSkillGet", APIManager::executeUserSkillGet);

bool APIManager::executeUserMaterialGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 14, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7334, "APIManager::executeUserMaterialGet", APIManager::executeUserMaterialGet);

bool APIManager::executeUserKeybladeGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 10, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7444, "APIManager::executeUserKeybladeGet", APIManager::executeUserKeybladeGet);

bool APIManager::executeUserAvatarAllGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 20, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7554, "APIManager::executeUserAvatarAllGet", APIManager::executeUserAvatarAllGet);

bool APIManager::executeUserAvatarPartsGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 21, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7664, "APIManager::executeUserAvatarPartsGet", APIManager::executeUserAvatarPartsGet);

bool APIManager::executeUserTitleGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 16, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7774, "APIManager::executeUserTitleGet", APIManager::executeUserTitleGet);

bool APIManager::executeUserLinkGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 140, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7884, "APIManager::executeUserLinkGet", APIManager::executeUserLinkGet);

bool APIManager::executeUserSupportGet(APIManager *self, uint32_t cb) {
    return chainRequest(self, 17, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7994, "APIManager::executeUserSupportGet", APIManager::executeUserSupportGet);

// ---- guarded requests without parameters (can be resent)
bool APIManager::executeTutorialClear(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 63, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4C5C88, "APIManager::executeTutorialClear", APIManager::executeTutorialClear);

bool APIManager::executeUserAlbumGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 15, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4C7C74, "APIManager::executeUserAlbumGet", APIManager::executeUserAlbumGet);

bool APIManager::executeStageGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 99, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CAC74, "APIManager::executeStageGet", APIManager::executeStageGet);

bool APIManager::executeStageGet160310(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 100, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CADB8, "APIManager::executeStageGet160310", APIManager::executeStageGet160310);

bool APIManager::executeRaidGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 107, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CBEC4, "APIManager::executeRaidGet", APIManager::executeRaidGet);

bool APIManager::executeRaidList(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 108, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CC008, "APIManager::executeRaidList", APIManager::executeRaidList);

bool APIManager::executeRaidRewardGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 109, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CC14C, "APIManager::executeRaidRewardGet", APIManager::executeRaidRewardGet);

bool APIManager::executeRaidRewardGet151101(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 110, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CC290, "APIManager::executeRaidRewardGet151101", APIManager::executeRaidRewardGet151101);

bool APIManager::executeUserPresentGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 18, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CCF3C, "APIManager::executeUserPresentGet", APIManager::executeUserPresentGet);

bool APIManager::executeUserBirthdayGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 8, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CD254, "APIManager::executeUserBirthdayGet", APIManager::executeUserBirthdayGet);

bool APIManager::executeShopMedalMax(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 66, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CDDF4, "APIManager::executeShopMedalMax", APIManager::executeShopMedalMax);

bool APIManager::executePaymentInfo(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 22, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CE74C, "APIManager::executePaymentInfo", APIManager::executePaymentInfo);

bool APIManager::executePartyNoticeUpdate(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 76, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CED10, "APIManager::executePartyNoticeUpdate", APIManager::executePartyNoticeUpdate);

bool APIManager::executePartyLogin(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 77, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CEE54, "APIManager::executePartyLogin", APIManager::executePartyLogin);

bool APIManager::executePartyApplicationList(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 83, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4CF4BC, "APIManager::executePartyApplicationList", APIManager::executePartyApplicationList);

bool APIManager::executePartyOfferList(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 96, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D0E88, "APIManager::executePartyOfferList", APIManager::executePartyOfferList);

bool APIManager::executeColosseumGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 114, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D1324, "APIManager::executeColosseumGet", APIManager::executeColosseumGet);

bool APIManager::executeColosseumRewardList(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 115, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D1468, "APIManager::executeColosseumRewardList", APIManager::executeColosseumRewardList);

bool APIManager::executeSystemSerialcodeCampaignGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 32, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D1F28, "APIManager::executeSystemSerialcodeCampaignGet", APIManager::executeSystemSerialcodeCampaignGet);

bool APIManager::executeRankingWeeklyUser(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 119, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D24D8, "APIManager::executeRankingWeeklyUser", APIManager::executeRankingWeeklyUser);

bool APIManager::executeRankingMonthlyUser(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 120, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D261C, "APIManager::executeRankingMonthlyUser", APIManager::executeRankingMonthlyUser);

bool APIManager::executeRankingWeeklyUserOwn(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 121, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D2760, "APIManager::executeRankingWeeklyUserOwn", APIManager::executeRankingWeeklyUserOwn);

bool APIManager::executeRankingMonthlyUserOwn(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 122, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D28A4, "APIManager::executeRankingMonthlyUserOwn", APIManager::executeRankingMonthlyUserOwn);

bool APIManager::executeRankingLux(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 123, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D29E8, "APIManager::executeRankingLux", APIManager::executeRankingLux);

bool APIManager::executeRankingLuxOwn(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 124, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D2B2C, "APIManager::executeRankingLuxOwn", APIManager::executeRankingLuxOwn);

bool APIManager::executeRankingParty(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 125, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D2C70, "APIManager::executeRankingParty", APIManager::executeRankingParty);

bool APIManager::executeRankingPartyOwn(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 126, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D2DB4, "APIManager::executeRankingPartyOwn", APIManager::executeRankingPartyOwn);

bool APIManager::executeRankingColosseum(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 127, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D2EF8, "APIManager::executeRankingColosseum", APIManager::executeRankingColosseum);

bool APIManager::executeRankingColosseumOwn(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 128, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D303C, "APIManager::executeRankingColosseumOwn", APIManager::executeRankingColosseumOwn);

bool APIManager::executeRankingParadeGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 129, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D3180, "APIManager::executeRankingParadeGet", APIManager::executeRankingParadeGet);

bool APIManager::executeCampaignGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 131, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D346C, "APIManager::executeCampaignGet", APIManager::executeCampaignGet);

bool APIManager::executeMypageInfoGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 56, cb, 1);
}
RW_FUNCTION_NOVERIFY(0x4D35B0, "APIManager::executeMypageInfoGet", APIManager::executeMypageInfoGet);

// ---- guarded requests without parameters (not resent)
bool APIManager::executeShopApRecover(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 67, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4CDF38, "APIManager::executeShopApRecover", APIManager::executeShopApRecover);

bool APIManager::executePartyLeave(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 79, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4CEF98, "APIManager::executePartyLeave", APIManager::executePartyLeave);

bool APIManager::executePartyApplicantList(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 85, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4CF7AC, "APIManager::executePartyApplicantList", APIManager::executePartyApplicantList);

bool APIManager::executePartyInviteList(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 94, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4D0B98, "APIManager::executePartyInviteList", APIManager::executePartyInviteList);

bool APIManager::executeSystemInvitecode(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 33, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4D206C, "APIManager::executeSystemInvitecode", APIManager::executeSystemInvitecode);

bool APIManager::executeSystemPushHelp(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 35, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4D2394, "APIManager::executeSystemPushHelp", APIManager::executeSystemPushHelp);

bool APIManager::executeCampaignPincodeGet(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 132, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4D36F4, "APIManager::executeCampaignPincodeGet", APIManager::executeCampaignPincodeGet);

bool APIManager::executeCampaignPincodeCreate(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 133, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4D3838, "APIManager::executeCampaignPincodeCreate", APIManager::executeCampaignPincodeCreate);

bool APIManager::executeCampaignPincodeDelete(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 135, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4D3B40, "APIManager::executeCampaignPincodeDelete", APIManager::executeCampaignPincodeDelete);

bool APIManager::executeCampaignPincodeStatusUpdate(APIManager *self, uint32_t cb) {
    return requestWithoutParams(self, 136, cb, 0);
}
RW_FUNCTION_NOVERIFY(0x4D3C84, "APIManager::executeCampaignPincodeStatusUpdate", APIManager::executeCampaignPincodeStatusUpdate);

// ---- guarded requests with an empty parameter object
bool APIManager::executeLoginBonus(APIManager *self, uint32_t cb) {
    return requestWithEmptyParams(self, 60, cb);
}
RW_FUNCTION_NOVERIFY(0x4C5698, "APIManager::executeLoginBonus", APIManager::executeLoginBonus);

bool APIManager::executeStageSupportList(APIManager *self, uint32_t cb) {
    return requestWithEmptyParams(self, 102, cb);
}
RW_FUNCTION_NOVERIFY(0x4CB0A8, "APIManager::executeStageSupportList", APIManager::executeStageSupportList);

bool APIManager::executeStageContinue(APIManager *self, uint32_t cb) {
    return requestWithEmptyParams(self, 105, cb);
}
RW_FUNCTION_NOVERIFY(0x4CB5EC, "APIManager::executeStageContinue", APIManager::executeStageContinue);

bool APIManager::executeSystemNeedUrl(APIManager *self, uint32_t cb) {
    return requestWithEmptyParams(self, 23, cb);
}
RW_FUNCTION_NOVERIFY(0x4CD618, "APIManager::executeSystemNeedUrl", APIManager::executeSystemNeedUrl);

bool APIManager::executeSystemCoppaGet(APIManager *self, uint32_t cb) {
    return requestWithEmptyParams(self, 24, cb);
}
RW_FUNCTION_NOVERIFY(0x4CD79C, "APIManager::executeSystemCoppaGet", APIManager::executeSystemCoppaGet);

bool APIManager::executeSystemInformationList151203(APIManager *self, uint32_t cb) {
    return requestWithEmptyParams(self, 29, cb);
}
RW_FUNCTION_NOVERIFY(0x4CDAC8, "APIManager::executeSystemInformationList151203", APIManager::executeSystemInformationList151203);

// ---- guarded requests with one 64-bit id parameter (not resent)
bool APIManager::executeUserDetailGet(APIManager *self, uint64_t userId, uint32_t cb) {
    return requestWithId(self, 3, 0xDFA977 /* "userId" */, userId, cb);
}
RW_FUNCTION_NOVERIFY(0x4C7DB8, "APIManager::executeUserDetailGet", APIManager::executeUserDetailGet);

bool APIManager::executeKeybladeBurstMax(APIManager *self, uint64_t userKeybladeId, uint32_t cb) {
    return requestWithId(self, 51, 0xDFAEE6 /* "userKeybladeId" */, userKeybladeId, cb);
}
RW_FUNCTION_NOVERIFY(0x4C9DC8, "APIManager::executeKeybladeBurstMax", APIManager::executeKeybladeBurstMax);

bool APIManager::executeRaidMemberGet(APIManager *self, uint64_t raidId, uint32_t cb) {
    return requestWithId(self, 111, 0xDFB326 /* "raidId" */, raidId, cb);
}
RW_FUNCTION_NOVERIFY(0x4CC3D4, "APIManager::executeRaidMemberGet", APIManager::executeRaidMemberGet);

bool APIManager::executePartyApplicationCancel(APIManager *self, uint64_t partyApplicantId, uint32_t cb) {
    return requestWithId(self, 84, 0xDFB015 /* "partyApplicantId" */, partyApplicantId, cb);
}
RW_FUNCTION_NOVERIFY(0x4CF600, "APIManager::executePartyApplicationCancel", APIManager::executePartyApplicationCancel);

bool APIManager::executePartyApplicantAgree(APIManager *self, uint64_t partyApplicantId, uint32_t cb) {
    return requestWithId(self, 86, 0xDFB015 /* "partyApplicantId" */, partyApplicantId, cb);
}
RW_FUNCTION_NOVERIFY(0x4CF8F0, "APIManager::executePartyApplicantAgree", APIManager::executePartyApplicantAgree);

bool APIManager::executePartyApplicantReject(APIManager *self, uint64_t partyApplicantId, uint32_t cb) {
    return requestWithId(self, 87, 0xDFB015 /* "partyApplicantId" */, partyApplicantId, cb);
}
RW_FUNCTION_NOVERIFY(0x4CFA9C, "APIManager::executePartyApplicantReject", APIManager::executePartyApplicantReject);

bool APIManager::executePartyLeaderChange(APIManager *self, uint64_t appointId, uint32_t cb) {
    return requestWithId(self, 88, 0xDFBFFB /* "appointId" */, appointId, cb);
}
RW_FUNCTION_NOVERIFY(0x4D008C, "APIManager::executePartyLeaderChange", APIManager::executePartyLeaderChange);

bool APIManager::executePartyAdminAppoint(APIManager *self, uint64_t appointId, uint32_t cb) {
    return requestWithId(self, 89, 0xDFBFFB /* "appointId" */, appointId, cb);
}
RW_FUNCTION_NOVERIFY(0x4D0238, "APIManager::executePartyAdminAppoint", APIManager::executePartyAdminAppoint);

bool APIManager::executePartyAdminDismiss(APIManager *self, uint64_t dismissId, uint32_t cb) {
    return requestWithId(self, 90, 0xDFC005 /* "dismissId" */, dismissId, cb);
}
RW_FUNCTION_NOVERIFY(0x4D03E4, "APIManager::executePartyAdminDismiss", APIManager::executePartyAdminDismiss);

bool APIManager::executePartyMemberExpel(APIManager *self, uint64_t expelId, uint32_t cb) {
    return requestWithId(self, 91, 0xDFC00F /* "expelId" */, expelId, cb);
}
RW_FUNCTION_NOVERIFY(0x4D0590, "APIManager::executePartyMemberExpel", APIManager::executePartyMemberExpel);

bool APIManager::executePartyInviteCancel(APIManager *self, uint64_t partyOfferId, uint32_t cb) {
    return requestWithId(self, 95, 0xDFAFAC /* "partyOfferId" */, partyOfferId, cb);
}
RW_FUNCTION_NOVERIFY(0x4D0CDC, "APIManager::executePartyInviteCancel", APIManager::executePartyInviteCancel);

bool APIManager::executePartyOfferAgree(APIManager *self, uint64_t partyOfferId, uint32_t cb) {
    return requestWithId(self, 97, 0xDFAFAC /* "partyOfferId" */, partyOfferId, cb);
}
RW_FUNCTION_NOVERIFY(0x4D0FCC, "APIManager::executePartyOfferAgree", APIManager::executePartyOfferAgree);

bool APIManager::executePartyOfferReject(APIManager *self, uint64_t partyOfferId, uint32_t cb) {
    return requestWithId(self, 98, 0xDFAFAC /* "partyOfferId" */, partyOfferId, cb);
}
RW_FUNCTION_NOVERIFY(0x4D1178, "APIManager::executePartyOfferReject", APIManager::executePartyOfferReject);

// ---- guarded requests with one integer parameter
bool APIManager::executeUserSphereCheck(APIManager *self, uint32_t checkType, uint32_t cb) {
    Frame F(0x68);
    return requestWithInt(F, self, 57, 0xDFBCAB /* "checkType" */, checkType, cb, ADD_MEMBER_I32);
}
RW_FUNCTION_NOVERIFY(0x4CCBC0, "APIManager::executeUserSphereCheck", APIManager::executeUserSphereCheck);

bool APIManager::executeSystemInformationList(APIManager *self, uint32_t type, uint32_t cb) {
    Frame F(0x68);
    return requestWithInt(F, self, 28, 0xDFA938 /* "type" */, type, cb, ADD_MEMBER_I32);
}
RW_FUNCTION_NOVERIFY(0x4CD920, "APIManager::executeSystemInformationList", APIManager::executeSystemInformationList);

bool APIManager::executeSystemInformationDetailGet(APIManager *self, uint32_t infoId, uint32_t cb) {
    Frame F(0x68);
    return requestWithInt(F, self, 30, 0xDFBFC3 /* "infoId" */, infoId, cb, ADD_MEMBER_I32);
}
RW_FUNCTION_NOVERIFY(0x4CDC4C, "APIManager::executeSystemInformationDetailGet", APIManager::executeSystemInformationDetailGet);

bool APIManager::executeUserSphereBuy(APIManager *self, uint32_t sphereBoardId, uint32_t cb) {
    Frame F(0x68);
    return requestWithInt(F, self, 68, 0xDFB29F /* "sphereBoardId" */, sphereBoardId, cb, ADD_MEMBER_I32);
}
RW_FUNCTION_NOVERIFY(0x4CE07C, "APIManager::executeUserSphereBuy", APIManager::executeUserSphereBuy);

bool APIManager::executeRankingRewardCheck(APIManager *self, uint32_t type, uint32_t cb) {
    Frame F(0x68);
    return requestWithInt(F, self, 130, 0xDFA938 /* "type" */, type, cb, ADD_MEMBER_I32);
}
RW_FUNCTION_NOVERIFY(0x4D32C4, "APIManager::executeRankingRewardCheck", APIManager::executeRankingRewardCheck);

bool APIManager::executeUserAlbumChallengeUpdate(APIManager *self, uint32_t challengeId, uint32_t cb) {
    Frame F(0x68);
    return requestWithInt(F, self, 144, 0xDFB5F4 /* "challengeId" */, challengeId, cb, ADD_MEMBER_U32);
}
RW_FUNCTION_NOVERIFY(0x4D4778, "APIManager::executeUserAlbumChallengeUpdate", APIManager::executeUserAlbumChallengeUpdate);

// ---- master / resource downloads: the data manager is reset first
bool APIManager::executeSystemMaster(APIManager *self, uint32_t revision, uint32_t cb) {
    Frame F(0x68);
    F.call(MASTER_DATA_MANAGER_RESET, {F.call(MASTER_DATA_MANAGER_GET_INSTANCE, {})});
    return requestWithInt(F, self, 25, 0xDFBF02 /* "revision" */, revision, cb, ADD_MEMBER_I32);
}
RW_FUNCTION_NOVERIFY(0x4CA720, "APIManager::executeSystemMaster", APIManager::executeSystemMaster);

bool APIManager::executeSystemResource(APIManager *self, uint32_t revision, uint32_t cb) {
    Frame F(0x68);
    F.call(RESOURCE_DATA_MANAGER_RESET, {F.call(RESOURCE_DATA_MANAGER_GET_INSTANCE, {})});
    return requestWithInt(F, self, 26, 0xDFBF02 /* "revision" */, revision, cb, ADD_MEMBER_I32);
}
RW_FUNCTION_NOVERIFY(0x4CA8CC, "APIManager::executeSystemResource", APIManager::executeSystemResource);

bool APIManager::executeStageEventGet(APIManager *self, uint32_t eventCategory, uint32_t cb) {
    Frame F(0x68);
    F.call(MASTER_DATA_MANAGER_RESET, {F.call(MASTER_DATA_MANAGER_GET_INSTANCE, {})});
    return requestWithInt(F, self, 101, 0xDFBC4F /* "eventCategory" */, eventCategory, cb, ADD_MEMBER_U8);
}
RW_FUNCTION_NOVERIFY(0x4CAEFC, "APIManager::executeStageEventGet", APIManager::executeStageEventGet);

// user/get (action 1): guarded, no parameters; its reply starts the login chain (onRespond)
bool APIManager::executeUserGet(APIManager *self, uint32_t cb) { return requestWithoutParams(self, 1, cb, 1); }
RW_FUNCTION_NOVERIFY(0x4C6080, "APIManager::executeUserGet", APIManager::executeUserGet);
