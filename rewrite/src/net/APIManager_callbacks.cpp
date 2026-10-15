// APIManager callbacks: what runs after a reply was verified (onRespond dispatches the login chain itself; the
// others are bound by the scenes). Most simply end the connection successfully; the user-data chain callbacks
// start the next request of the chain.
#include "APIManager.hpp"

using namespace apim;

namespace {
constexpr uint32_t FINISH_CONNECTION = 0x4BAEB1;
constexpr uint32_t EXECUTE_TUTORIAL_USER_CREATE = 0x4C5589;
constexpr uint32_t EXECUTE_USER_START = 0x4C61C5;
constexpr uint32_t EXECUTE_PARTY_GET = 0x4C69BD;
constexpr uint32_t EXECUTE_USER_STONE_GET = 0x4C6B89;
constexpr uint32_t EXECUTE_USER_OPTION_GET = 0x4C6DE5;
constexpr uint32_t EXECUTE_PARTY_MEMBER_LIST = 0x4C7AA5;

// finishConnection(true) through the dispatcher, from a frame of the original's size
void finishOk(APIManager *self) {
    Frame F(8);
    F.call(FINISH_CONNECTION, {addr(self), 1});
}
// the next request of a chain with the stored callback: execute*(callback)
void chain(APIManager *self, uint32_t execute) {
    Frame F(8);
    F.call(execute, {addr(self), addr(&self->callback)});
}
}  // namespace

// ---------------------------------------------------------------------------------------------- login chain
// login: a new user (Login byte 0) creates the tutorial user first.
void APIManager::callbackLogin(APIManager *self, int32_t status) {
    Frame F(0x28);
    if (status != 0) return;
    if (sbByte(self->login.ptr, 0)) {
        F.call(EXECUTE_TUTORIAL_USER_CREATE, {addr(self), addr(&self->callback)});
    } else {
        F.call(FINISH_CONNECTION, {addr(self), 1});
    }
}
RW_FUNCTION_NOVERIFY(0x4C2A1C, "APIManager::callbackLogin", APIManager::callbackLogin);

void APIManager::callbackUserGet(APIManager *self) {
    Frame F(8);
    self->loginChain = 1;
    F.call(EXECUTE_USER_START, {addr(self), addr(&self->callback)});
}
RW_FUNCTION_NOVERIFY(0x4C2AF4, "APIManager::callbackUserGet", APIManager::callbackUserGet);

void APIManager::callbackUserChatGet(APIManager *self) {
    Frame F(16);
    if (self->loginChain) {
        // executePartyGet(partyId 0, callback): r1 is the unused register before the 64-bit argument
        F.call(EXECUTE_PARTY_GET, {addr(self), addr(&self->callback), 0, 0, addr(&self->callback)});
    } else {
        F.call(FINISH_CONNECTION, {addr(self), 1});
    }
}
RW_FUNCTION_NOVERIFY(0x4C2B2A, "APIManager::callbackUserChatGet", APIManager::callbackUserChatGet);

void APIManager::callbackPartyGet(APIManager *self) {
    Frame F(8);
    if (self->loginChain) F.call(EXECUTE_USER_STONE_GET, {addr(self), addr(&self->callback)});
    else F.call(FINISH_CONNECTION, {addr(self), 1});
}
RW_FUNCTION_NOVERIFY(0x4C2B56, "APIManager::callbackPartyGet", APIManager::callbackPartyGet);

void APIManager::callbackUserShopGet(APIManager *self) {
    Frame F(8);
    if (self->loginChain) {
        self->loginChain = 0;
        F.call(EXECUTE_USER_OPTION_GET, {addr(self), addr(&self->callback)});
    } else {
        F.call(FINISH_CONNECTION, {addr(self), 1});
    }
}
RW_FUNCTION_NOVERIFY(0x4C2B82, "APIManager::callbackUserShopGet", APIManager::callbackUserShopGet);

// user/support/get: a user in a party also loads its member list.
void APIManager::callbackUserSupportGet(APIManager *self) {
    Frame F(0x28);
    if (sbU64(self->userDetail.ptr, 40) != 0) {  // UserDetail: party id
        self->allowWhileBusy = 1;
        F.call(EXECUTE_PARTY_MEMBER_LIST, {addr(self), 1, addr(&self->callback), 1});
    } else {
        F.call(FINISH_CONNECTION, {addr(self), 1});
    }
}
RW_FUNCTION_NOVERIFY(0x4C2C30, "APIManager::callbackUserSupportGet", APIManager::callbackUserSupportGet);

// ---------------------------------------------------------------------------------------------- user data chain

void APIManager::callbackTutorialStatusGet(APIManager *self) { chain(self, 0x4C7005); }  // -> executeUserSphereGet
RW_FUNCTION_NOVERIFY(0x4C2ADC, "APIManager::callbackTutorialStatusGet", APIManager::callbackTutorialStatusGet);
void APIManager::callbackUserStart(APIManager *self) { chain(self, 0x4C6871); }  // -> executeUserChatGet
RW_FUNCTION_NOVERIFY(0x4C2B08, "APIManager::callbackUserStart", APIManager::callbackUserStart);
void APIManager::callbackUserStoneGet(APIManager *self) { chain(self, 0x4C6CD5); }  // -> executeUserShopGet
RW_FUNCTION_NOVERIFY(0x4C2B74, "APIManager::callbackUserStoneGet", APIManager::callbackUserStoneGet);
void APIManager::callbackUserSphereGet(APIManager *self) { chain(self, 0x4C7115); }  // -> executeUserMedalGet
RW_FUNCTION_NOVERIFY(0x4C2BAA, "APIManager::callbackUserSphereGet", APIManager::callbackUserSphereGet);
void APIManager::callbackUserMedalGet(APIManager *self) { chain(self, 0x4C7225); }  // -> executeUserSkillGet
RW_FUNCTION_NOVERIFY(0x4C2BB8, "APIManager::callbackUserMedalGet", APIManager::callbackUserMedalGet);
void APIManager::callbackUserSkillGet(APIManager *self) { chain(self, 0x4C7335); }  // -> executeUserMaterialGet
RW_FUNCTION_NOVERIFY(0x4C2BC6, "APIManager::callbackUserSkillGet", APIManager::callbackUserSkillGet);
void APIManager::callbackUserMaterialGet(APIManager *self) { chain(self, 0x4C7445); }  // -> executeUserKeybladeGet
RW_FUNCTION_NOVERIFY(0x4C2BD4, "APIManager::callbackUserMaterialGet", APIManager::callbackUserMaterialGet);
void APIManager::callbackUserKeybladeGet(APIManager *self) { chain(self, 0x4C7555); }  // -> executeUserAvatarAllGet
RW_FUNCTION_NOVERIFY(0x4C2BE2, "APIManager::callbackUserKeybladeGet", APIManager::callbackUserKeybladeGet);
void APIManager::callbackUserAvatarAllGet(APIManager *self) { chain(self, 0x4C7665); }  // -> executeUserAvatarPartsGet
RW_FUNCTION_NOVERIFY(0x4C2C04, "APIManager::callbackUserAvatarAllGet", APIManager::callbackUserAvatarAllGet);
void APIManager::callbackUserAvatarPartsGet(APIManager *self) { chain(self, 0x4C7775); }  // -> executeUserTitleGet
RW_FUNCTION_NOVERIFY(0x4C2C12, "APIManager::callbackUserAvatarPartsGet", APIManager::callbackUserAvatarPartsGet);
void APIManager::callbackUserTitleGet(APIManager *self) { chain(self, 0x4C7885); }  // -> executeUserLinkGet
RW_FUNCTION_NOVERIFY(0x4C2C20, "APIManager::callbackUserTitleGet", APIManager::callbackUserTitleGet);
void APIManager::callbackUserOptionGet(APIManager *self) { chain(self, 0x4C6EF5); }  // -> executeTutorialStatusGet
RW_FUNCTION_NOVERIFY(0x4C30E8, "APIManager::callbackUserOptionGet", APIManager::callbackUserOptionGet);
void APIManager::callbackUserLinkGet(APIManager *self) { chain(self, 0x4C7995); }  // -> executeUserSupportGet
RW_FUNCTION_NOVERIFY(0x4C310A, "APIManager::callbackUserLinkGet", APIManager::callbackUserLinkGet);

// ---------------------------------------------------------------------------------------------- finish
void APIManager::callbackLoginBonus(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2AB4, "APIManager::callbackLoginBonus", APIManager::callbackLoginBonus);
void APIManager::callbackTutorialUserCreate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2ABE, "APIManager::callbackTutorialUserCreate", APIManager::callbackTutorialUserCreate);
void APIManager::callbackTutorialProgress(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2AC8, "APIManager::callbackTutorialProgress", APIManager::callbackTutorialProgress);
void APIManager::callbackTutorialClear(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2AD2, "APIManager::callbackTutorialClear", APIManager::callbackTutorialClear);
void APIManager::callbackTutorialStatusUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2AEA, "APIManager::callbackTutorialStatusUpdate", APIManager::callbackTutorialStatusUpdate);
void APIManager::callbackUserDetailGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2B16, "APIManager::callbackUserDetailGet", APIManager::callbackUserDetailGet);
void APIManager::callbackUserProfileGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2B20, "APIManager::callbackUserProfileGet", APIManager::callbackUserProfileGet);
void APIManager::callbackUserAlbumGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2BF0, "APIManager::callbackUserAlbumGet", APIManager::callbackUserAlbumGet);
void APIManager::callbackUserAvatarGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2BFA, "APIManager::callbackUserAvatarGet", APIManager::callbackUserAvatarGet);
void APIManager::callbackUserUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CB0, "APIManager::callbackUserUpdate", APIManager::callbackUserUpdate);
void APIManager::callbackUserGenderUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CBA, "APIManager::callbackUserGenderUpdate", APIManager::callbackUserGenderUpdate);
void APIManager::callbackUserPlaystyleUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CC4, "APIManager::callbackUserPlaystyleUpdate", APIManager::callbackUserPlaystyleUpdate);
void APIManager::callbackUserTitleUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CCE, "APIManager::callbackUserTitleUpdate", APIManager::callbackUserTitleUpdate);
void APIManager::callbackUserAvatarUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CD8, "APIManager::callbackUserAvatarUpdate", APIManager::callbackUserAvatarUpdate);
void APIManager::callbackUserKeybladeUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CE2, "APIManager::callbackUserKeybladeUpdate", APIManager::callbackUserKeybladeUpdate);
void APIManager::callbackUserMedalLock(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CEC, "APIManager::callbackUserMedalLock", APIManager::callbackUserMedalLock);
void APIManager::callbackUserSupportUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2CF6, "APIManager::callbackUserSupportUpdate", APIManager::callbackUserSupportUpdate);
void APIManager::callbackMedalSkillUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D00, "APIManager::callbackMedalSkillUpdate", APIManager::callbackMedalSkillUpdate);
void APIManager::callbackUserMaterialSell(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D0A, "APIManager::callbackUserMaterialSell", APIManager::callbackUserMaterialSell);
void APIManager::callbackUserMedalSell(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D14, "APIManager::callbackUserMedalSell", APIManager::callbackUserMedalSell);
void APIManager::callbackSkillSell(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D1E, "APIManager::callbackSkillSell", APIManager::callbackSkillSell);
void APIManager::callbackKeybladeBurstMax(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D28, "APIManager::callbackKeybladeBurstMax", APIManager::callbackKeybladeBurstMax);
void APIManager::callbackKeybladeEnhance(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D32, "APIManager::callbackKeybladeEnhance", APIManager::callbackKeybladeEnhance);
void APIManager::callbackUserMedalEnhance(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D3C, "APIManager::callbackUserMedalEnhance", APIManager::callbackUserMedalEnhance);
void APIManager::callbackUserMedalEvolve(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D46, "APIManager::callbackUserMedalEvolve", APIManager::callbackUserMedalEvolve);
void APIManager::callbackSkillEvolve(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D50, "APIManager::callbackSkillEvolve", APIManager::callbackSkillEvolve);
void APIManager::callbackSystemMaster(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D5A, "APIManager::callbackSystemMaster", APIManager::callbackSystemMaster);
void APIManager::callbackSystemResource(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D64, "APIManager::callbackSystemResource", APIManager::callbackSystemResource);
void APIManager::callbackSystemResourceEv(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D6E, "APIManager::callbackSystemResourceEv", APIManager::callbackSystemResourceEv);
void APIManager::callbackStageGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D78, "APIManager::callbackStageGet", APIManager::callbackStageGet);
void APIManager::callbackStageGet160310(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D82, "APIManager::callbackStageGet160310", APIManager::callbackStageGet160310);
void APIManager::callbackStageEventGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D8C, "APIManager::callbackStageEventGet", APIManager::callbackStageEventGet);
void APIManager::callbackStageSupportList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2D96, "APIManager::callbackStageSupportList", APIManager::callbackStageSupportList);
void APIManager::callbackStageStart(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DA0, "APIManager::callbackStageStart", APIManager::callbackStageStart);
void APIManager::callbackStageRetire(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DAA, "APIManager::callbackStageRetire", APIManager::callbackStageRetire);
void APIManager::callbackStageContinue(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DB4, "APIManager::callbackStageContinue", APIManager::callbackStageContinue);
void APIManager::callbackStageClear(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DBE, "APIManager::callbackStageClear", APIManager::callbackStageClear);
void APIManager::callbackRaidGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DC8, "APIManager::callbackRaidGet", APIManager::callbackRaidGet);
void APIManager::callbackRaidList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DD2, "APIManager::callbackRaidList", APIManager::callbackRaidList);
void APIManager::callbackRaidRewardGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DDC, "APIManager::callbackRaidRewardGet", APIManager::callbackRaidRewardGet);
void APIManager::callbackRaidRewardGet151101(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DE6, "APIManager::callbackRaidRewardGet151101", APIManager::callbackRaidRewardGet151101);
void APIManager::callbackRaidMemberGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DF0, "APIManager::callbackRaidMemberGet", APIManager::callbackRaidMemberGet);
void APIManager::callbackRaidStart(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2DFA, "APIManager::callbackRaidStart", APIManager::callbackRaidStart);
void APIManager::callbackRaidClear(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E04, "APIManager::callbackRaidClear", APIManager::callbackRaidClear);
void APIManager::callbackUserSphereCheck(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E0E, "APIManager::callbackUserSphereCheck", APIManager::callbackUserSphereCheck);
void APIManager::callbackUserSphereUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E18, "APIManager::callbackUserSphereUpdate", APIManager::callbackUserSphereUpdate);
void APIManager::callbackUserPresentGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E22, "APIManager::callbackUserPresentGet", APIManager::callbackUserPresentGet);
void APIManager::callbackUserPresentReceive(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E2C, "APIManager::callbackUserPresentReceive", APIManager::callbackUserPresentReceive);
void APIManager::callbackUserBirthdayGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E36, "APIManager::callbackUserBirthdayGet", APIManager::callbackUserBirthdayGet);
void APIManager::callbackUserBirthdayCreate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E40, "APIManager::callbackUserBirthdayCreate", APIManager::callbackUserBirthdayCreate);
void APIManager::callbackSystemNeedUrl(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E4A, "APIManager::callbackSystemNeedUrl", APIManager::callbackSystemNeedUrl);
void APIManager::callbackSystemCoppaGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E54, "APIManager::callbackSystemCoppaGet", APIManager::callbackSystemCoppaGet);
void APIManager::callbackSystemInformationList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E5E, "APIManager::callbackSystemInformationList", APIManager::callbackSystemInformationList);
void APIManager::callbackSystemInformationList151203(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E68, "APIManager::callbackSystemInformationList151203", APIManager::callbackSystemInformationList151203);
void APIManager::callbackSystemInformationDetailGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E72, "APIManager::callbackSystemInformationDetailGet", APIManager::callbackSystemInformationDetailGet);
void APIManager::callbackShopMedalMax(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E7C, "APIManager::callbackShopMedalMax", APIManager::callbackShopMedalMax);
void APIManager::callbackShopApRecover(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E86, "APIManager::callbackShopApRecover", APIManager::callbackShopApRecover);
void APIManager::callbackUserSphereBuy(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E90, "APIManager::callbackUserSphereBuy", APIManager::callbackUserSphereBuy);
void APIManager::callbackUnionChange(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2E9A, "APIManager::callbackUnionChange", APIManager::callbackUnionChange);
void APIManager::callbackGachaList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EA4, "APIManager::callbackGachaList", APIManager::callbackGachaList);
void APIManager::callbackGachaDraw(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EAE, "APIManager::callbackGachaDraw", APIManager::callbackGachaDraw);
void APIManager::callbackPurchasePrepare(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EB8, "APIManager::callbackPurchasePrepare", APIManager::callbackPurchasePrepare);
void APIManager::callbackPurchaseFinish(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EC2, "APIManager::callbackPurchaseFinish", APIManager::callbackPurchaseFinish);
void APIManager::callbackPaymentInfo(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2ECC, "APIManager::callbackPaymentInfo", APIManager::callbackPaymentInfo);
void APIManager::callbackPartyCreate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2ED6, "APIManager::callbackPartyCreate", APIManager::callbackPartyCreate);
void APIManager::callbackPartyUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EE0, "APIManager::callbackPartyUpdate", APIManager::callbackPartyUpdate);
void APIManager::callbackPartyNoticeUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EEA, "APIManager::callbackPartyNoticeUpdate", APIManager::callbackPartyNoticeUpdate);
void APIManager::callbackPartyLogin(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EF4, "APIManager::callbackPartyLogin", APIManager::callbackPartyLogin);
void APIManager::callbackPartyLeave(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2EFE, "APIManager::callbackPartyLeave", APIManager::callbackPartyLeave);
void APIManager::callbackPartyMemberList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F08, "APIManager::callbackPartyMemberList", APIManager::callbackPartyMemberList);
void APIManager::callbackPartySearchActive(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F12, "APIManager::callbackPartySearchActive", APIManager::callbackPartySearchActive);
void APIManager::callbackPartyApplication(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F1C, "APIManager::callbackPartyApplication", APIManager::callbackPartyApplication);
void APIManager::callbackPartyApplicationList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F26, "APIManager::callbackPartyApplicationList", APIManager::callbackPartyApplicationList);
void APIManager::callbackPartyApplicationCancel(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F30, "APIManager::callbackPartyApplicationCancel", APIManager::callbackPartyApplicationCancel);
void APIManager::callbackPartyApplicantList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F3A, "APIManager::callbackPartyApplicantList", APIManager::callbackPartyApplicantList);
void APIManager::callbackPartyApplicantAgree(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F44, "APIManager::callbackPartyApplicantAgree", APIManager::callbackPartyApplicantAgree);
void APIManager::callbackPartyApplicantReject(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F4E, "APIManager::callbackPartyApplicantReject", APIManager::callbackPartyApplicantReject);
void APIManager::callbackPartyLeaderChange(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F58, "APIManager::callbackPartyLeaderChange", APIManager::callbackPartyLeaderChange);
void APIManager::callbackPartyAdminAppoint(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F62, "APIManager::callbackPartyAdminAppoint", APIManager::callbackPartyAdminAppoint);
void APIManager::callbackPartyAdminDismiss(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F6C, "APIManager::callbackPartyAdminDismiss", APIManager::callbackPartyAdminDismiss);
void APIManager::callbackPartyMemberExpel(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F76, "APIManager::callbackPartyMemberExpel", APIManager::callbackPartyMemberExpel);
void APIManager::callbackPartyInviteSearch(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F80, "APIManager::callbackPartyInviteSearch", APIManager::callbackPartyInviteSearch);
void APIManager::callbackPartyInviteOffer(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F8A, "APIManager::callbackPartyInviteOffer", APIManager::callbackPartyInviteOffer);
void APIManager::callbackPartyInviteList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F94, "APIManager::callbackPartyInviteList", APIManager::callbackPartyInviteList);
void APIManager::callbackPartyInviteCancel(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2F9E, "APIManager::callbackPartyInviteCancel", APIManager::callbackPartyInviteCancel);
void APIManager::callbackPartyOfferList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FA8, "APIManager::callbackPartyOfferList", APIManager::callbackPartyOfferList);
void APIManager::callbackPartyOfferAgree(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FB2, "APIManager::callbackPartyOfferAgree", APIManager::callbackPartyOfferAgree);
void APIManager::callbackPartyOfferReject(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FBC, "APIManager::callbackPartyOfferReject", APIManager::callbackPartyOfferReject);
void APIManager::callbackColosseumGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FC6, "APIManager::callbackColosseumGet", APIManager::callbackColosseumGet);
void APIManager::callbackColosseumRewardList(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FD0, "APIManager::callbackColosseumRewardList", APIManager::callbackColosseumRewardList);
void APIManager::callbackColosseumStart(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FDA, "APIManager::callbackColosseumStart", APIManager::callbackColosseumStart);
void APIManager::callbackColosseumRetire(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FE4, "APIManager::callbackColosseumRetire", APIManager::callbackColosseumRetire);
void APIManager::callbackColosseumClear(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FEE, "APIManager::callbackColosseumClear", APIManager::callbackColosseumClear);
void APIManager::callbackSystemSerialcode(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C2FF8, "APIManager::callbackSystemSerialcode", APIManager::callbackSystemSerialcode);
void APIManager::callbackSystemSerialcodeCampaignGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3002, "APIManager::callbackSystemSerialcodeCampaignGet", APIManager::callbackSystemSerialcodeCampaignGet);
void APIManager::callbackSystemInvitecode(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C300C, "APIManager::callbackSystemInvitecode", APIManager::callbackSystemInvitecode);
void APIManager::callbackSystemPushRegist(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3016, "APIManager::callbackSystemPushRegist", APIManager::callbackSystemPushRegist);
void APIManager::callbackSystemPushHelp(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3020, "APIManager::callbackSystemPushHelp", APIManager::callbackSystemPushHelp);
void APIManager::callbackRankingWeeklyUser(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C302A, "APIManager::callbackRankingWeeklyUser", APIManager::callbackRankingWeeklyUser);
void APIManager::callbackRankingMonthlyUser(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3034, "APIManager::callbackRankingMonthlyUser", APIManager::callbackRankingMonthlyUser);
void APIManager::callbackRankingWeeklyUserOwn(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C303E, "APIManager::callbackRankingWeeklyUserOwn", APIManager::callbackRankingWeeklyUserOwn);
void APIManager::callbackRankingMonthlyUserOwn(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3048, "APIManager::callbackRankingMonthlyUserOwn", APIManager::callbackRankingMonthlyUserOwn);
void APIManager::callbackRankingLux(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3052, "APIManager::callbackRankingLux", APIManager::callbackRankingLux);
void APIManager::callbackRankingLuxOwn(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C305C, "APIManager::callbackRankingLuxOwn", APIManager::callbackRankingLuxOwn);
void APIManager::callbackRankingParty(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3066, "APIManager::callbackRankingParty", APIManager::callbackRankingParty);
void APIManager::callbackRankingPartyOwn(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3070, "APIManager::callbackRankingPartyOwn", APIManager::callbackRankingPartyOwn);
void APIManager::callbackRankingColosseum(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C307A, "APIManager::callbackRankingColosseum", APIManager::callbackRankingColosseum);
void APIManager::callbackRankingColosseumOwn(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3084, "APIManager::callbackRankingColosseumOwn", APIManager::callbackRankingColosseumOwn);
void APIManager::callbackRankingParadeGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C308E, "APIManager::callbackRankingParadeGet", APIManager::callbackRankingParadeGet);
void APIManager::callbackRankingRewardCheck(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3098, "APIManager::callbackRankingRewardCheck", APIManager::callbackRankingRewardCheck);
void APIManager::callbackCampaignGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30A2, "APIManager::callbackCampaignGet", APIManager::callbackCampaignGet);
void APIManager::callbackMypageInfoGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30AC, "APIManager::callbackMypageInfoGet", APIManager::callbackMypageInfoGet);
void APIManager::callbackCampaignPincodeGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30B6, "APIManager::callbackCampaignPincodeGet", APIManager::callbackCampaignPincodeGet);
void APIManager::callbackCampaignPincodeCreate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30C0, "APIManager::callbackCampaignPincodeCreate", APIManager::callbackCampaignPincodeCreate);
void APIManager::callbackCampaignPincodeUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30CA, "APIManager::callbackCampaignPincodeUpdate", APIManager::callbackCampaignPincodeUpdate);
void APIManager::callbackCampaignPincodeDelete(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30D4, "APIManager::callbackCampaignPincodeDelete", APIManager::callbackCampaignPincodeDelete);
void APIManager::callbackCampaignPincodeStatusUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30DE, "APIManager::callbackCampaignPincodeStatusUpdate", APIManager::callbackCampaignPincodeStatusUpdate);
void APIManager::callbackUserOptionUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C30F6, "APIManager::callbackUserOptionUpdate", APIManager::callbackUserOptionUpdate);
void APIManager::callbackLinkMembersGet(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3100, "APIManager::callbackLinkMembersGet", APIManager::callbackLinkMembersGet);
void APIManager::callbackUserLinkUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3118, "APIManager::callbackUserLinkUpdate", APIManager::callbackUserLinkUpdate);
void APIManager::callbackUserLinkDelete(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3122, "APIManager::callbackUserLinkDelete", APIManager::callbackUserLinkDelete);
void APIManager::callbackSystemUuidUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C312C, "APIManager::callbackSystemUuidUpdate", APIManager::callbackSystemUuidUpdate);
void APIManager::callbackUserAlbumChallengeUpdate(APIManager *self) { finishOk(self); }
RW_FUNCTION_NOVERIFY(0x4C3136, "APIManager::callbackUserAlbumChallengeUpdate", APIManager::callbackUserAlbumChallengeUpdate);
