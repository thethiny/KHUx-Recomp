// APIManager: the client's server-API manager (singleton at guest 0x1003558). Builds the JSON request
// parameters for every server action (execute*), hands them to hole::network::api::Client, and on the reply
// (onRespond) verifies/parses the response into the shared_ptr members below, then runs the follow-up request
// of a request chain (login -> user/start -> chat -> party -> stone -> shop -> option -> tutorial status ...)
// or finishes the connection by invoking the caller's std::function<void(bool)> callback.
//
// Only one request runs at a time: `connecting` is set while a request is in flight and the guarded execute*
// methods refuse (calling the callback with false) while connecting (unless `allowWhileBusy`) or after an
// error (`failed`).
#pragma once

#include <initializer_list>

#include "rw/rw.hpp"
#include "core/SafeBuffer.hpp"

// ------------------------------------------------------------------------------------------ library layouts
// libc++ (32-bit) objects embedded in APIManager.
struct StdSharedPtr {                  // std::shared_ptr<T>
    rw::Field<uint32_t> ptr;           // +0 the object
    rw::Field<uint32_t> ctrl;          // +4 std::__shared_weak_count*
};
RW_SIZE(StdSharedPtr, 8);

struct StdString {                     // std::string; byte0 & 1 == 0: short (size byte0 >> 1, chars at +1)
    rw::Field<uint32_t> cap;           // +0 long form: capacity | 1
    rw::Field<uint32_t> size;          // +4 long form: size
    rw::Field<uint32_t> data;          // +8 long form: chars
};
RW_SIZE(StdString, 12);

struct StdVector {                     // std::vector<T>
    rw::Field<uint32_t> begin;
    rw::Field<uint32_t> end;
    rw::Field<uint32_t> cap;
};
RW_SIZE(StdVector, 12);

struct StdFunction {                   // std::function<...>: small-object buffer, then the callable (__f_)
    rw::Field<uint32_t> buf[4];        // +0  (== __f_ when the callable is stored in place)
    rw::Field<uint32_t> f;             // +16 __base* (null = empty)
};
RW_SIZE(StdFunction, 20);

// ------------------------------------------------------------------------------------------ APIManager
struct APIManager {
    // connection state (onRespond): 0 ok, 2 maintenance, 3 app update needed, 4 data update needed,
    // 5 session expired (relogin); isBusy() = state in 1..3
    rw::Field<int32_t> state;                  // +0x000
    rw::Field<uint8_t> connecting;             // +0x004 a request is in flight
    rw::Field<uint8_t> allowWhileBusy;         // +0x005 next guarded execute may start while connecting
    rw::Field<uint8_t> retryable;              // +0x006 the request can be resent (needsRetry)
    const uint8_t pad_007[1];
    StdString errorMessage;                    // +0x008 text for the error dialog (handleError)
    rw::Field<uint8_t> failed;                 // +0x014 the last request failed: requests are refused
    const uint8_t pad_015[3];
    rw::Field<int32_t> status;                 // +0x018 Client::StatusCode of the last reply
    rw::Field<int32_t> action;                 // +0x01C hole::network::api::Action being requested
    StdSharedPtr params;                       // +0x020 shared_ptr<rapidjson::Document>: request parameters
    StdFunction callback;                      // +0x028 std::function<void(bool)>: called when the request ends
    const uint8_t pad_03c[4];
    rw::Field<uint8_t> loginChain;             // +0x040 running the login chain (user/start -> ... -> option)
    const uint8_t pad_041[3];
    // the parsed responses (hole::network::api::*), kept until replaced
    StdSharedPtr ret;                          // +0x044 shared_ptr<Ret>: the reply header
    StdSharedPtr login;                        // +0x04C Login (a SafeBuffer: byte 0 = new user)
    StdSharedPtr resp_054[2];                  // +0x054, +0x05C
    StdSharedPtr tutorialStatus;               // +0x064 TutorialStatus (u32 phase, u64 popup flags at 8, finished at 0x10)
    StdSharedPtr resp_06c;                     // +0x06C
    StdSharedPtr user;                         // +0x074 User (u64 user id at 0)
    StdSharedPtr resp_07c[2];                  // +0x07C, +0x084
    StdSharedPtr userPoint;                    // +0x08C UserPoint (money at 0)
    StdSharedPtr userDetail;                   // +0x094 UserDetail (u64 party id at 40)
    StdSharedPtr stageResumption;              // +0x09C StageResumption
    StdSharedPtr userStone;                    // +0x0A4 UserStone
    StdSharedPtr resp_0ac[3];                  // +0x0AC .. +0x0BC
    StdSharedPtr userAvatarAll;                // +0x0C4 UserAvatarAll (coordinates; id -> slot map at +0x24)
    StdSharedPtr userAvatarParts;              // +0x0CC UserAvatarParts (24-byte parts)
    StdSharedPtr userMedal;                    // +0x0D4 UserMedal (72-byte medals)
    StdSharedPtr userSkill;                    // +0x0DC UserSkill (40-byte skills)
    StdSharedPtr userMaterial;                 // +0x0E4 UserMaterial (16-byte materials)
    StdSharedPtr userKeyblade;                 // +0x0EC UserKeyblade (88-byte keyblades)
    StdSharedPtr userAlbum;                    // +0x0F4 UserAlbum
    StdSharedPtr resp_0fc[2];                  // +0x0FC, +0x104
    StdSharedPtr userTitle;                    // +0x10C UserTitle (16-byte titles)
    StdSharedPtr resp_114[5];                  // +0x114 .. +0x134
    StdSharedPtr stageStart;                   // +0x13C StageStart
    StdSharedPtr resp_144[2];                  // +0x144, +0x14C
    StdVector raids;                           // +0x154 std::vector<RaidBase> (240 bytes each)
    StdSharedPtr resp_160[9];                  // +0x160 .. +0x1A0 (+0x188 is not cleared by clearAllResponses)
    StdSharedPtr systemNeedUrl;                // +0x1A8 SystemNeedUrl (also kept by onRespond)
    StdSharedPtr resp_1b0[3];                  // +0x1B0 .. +0x1C0
    StdString string_1c8;                      // +0x1C8
    StdSharedPtr resp_1d4[6];                  // +0x1D4 .. +0x1FC
    StdVector partyApplications;               // +0x204 std::vector<PartyApplicationList::PartyEx> (408 bytes each)
    StdSharedPtr resp_210[7];                  // +0x210 .. +0x240
    rw::Field<uint32_t> word_248;              // +0x248 (not initialized by the constructor)
    StdSharedPtr resp_24c;                     // +0x24C
    StdVector ints_254;                        // +0x254 std::vector<int>
    StdSharedPtr colosseumStart;               // +0x260 ColosseumStart (rank/stage count/stage id at +0x28, keyblade +0x38)
    StdSharedPtr resp_268;                     // +0x268
    StdVector ints_270;                        // +0x270 std::vector<int>
    StdSharedPtr resp_27c[16];                 // +0x27C .. +0x2F4
    StdVector ints_2fc;                        // +0x2FC std::vector<int>
    StdVector ints_308;                        // +0x308 std::vector<int>
    StdSharedPtr resp_314[5];                  // +0x314 .. +0x334
    SafeBuffer stageRequest;                   // +0x33C StageRequestData (a 0x18-byte SafeBuffer)
    const uint8_t pad_35c[4];
    StdString string_360;                      // +0x360

    static constexpr uint32_t INSTANCE = 0x1003558;   // the singleton
    static constexpr uint32_t INSTANCE_GUARD = 0x10038C8;

    // ---- lifetime / connection (APIManager_core.cpp)
    static APIManager *construct(APIManager *self);
    static APIManager *destruct(APIManager *self);
    static APIManager *getInstance();
    static void abort(APIManager *self);
    static void finishConnection(APIManager *self, uint32_t ok);
    static int32_t onRespond(APIManager *self, int32_t action, int32_t status, uint32_t retRef, uint32_t doc);
    static void handleError(APIManager *self, int32_t status, uint32_t retRef);
    static bool isBusy(const APIManager *self);
    static bool needsRetry(const APIManager *self);
    static void clearAllResponses(APIManager *self);
    static void reallocate(APIManager *self);
    static uint32_t openErrorDialog(APIManager *self, uint32_t cb, uint32_t retry);   // APIManager_dialog.cpp
    static void StageRequestData_deletingDestructor(uint32_t self);
    static bool executeUserGet(APIManager *self, uint32_t cb);

    // ---- callbacks (APIManager_callbacks.cpp): run after a verified reply
    static void callbackLogin(APIManager *self, int32_t status);
    static void callbackUserGet(APIManager *self);
    static void callbackUserChatGet(APIManager *self);
    static void callbackPartyGet(APIManager *self);
    static void callbackUserShopGet(APIManager *self);
    static void callbackUserSupportGet(APIManager *self);
    static void callbackTutorialStatusGet(APIManager *self);
    static void callbackUserStart(APIManager *self);
    static void callbackUserStoneGet(APIManager *self);
    static void callbackUserSphereGet(APIManager *self);
    static void callbackUserMedalGet(APIManager *self);
    static void callbackUserSkillGet(APIManager *self);
    static void callbackUserMaterialGet(APIManager *self);
    static void callbackUserKeybladeGet(APIManager *self);
    static void callbackUserAvatarAllGet(APIManager *self);
    static void callbackUserAvatarPartsGet(APIManager *self);
    static void callbackUserTitleGet(APIManager *self);
    static void callbackUserOptionGet(APIManager *self);
    static void callbackUserLinkGet(APIManager *self);
    static void callbackLoginBonus(APIManager *self);
    static void callbackTutorialUserCreate(APIManager *self);
    static void callbackTutorialProgress(APIManager *self);
    static void callbackTutorialClear(APIManager *self);
    static void callbackTutorialStatusUpdate(APIManager *self);
    static void callbackUserDetailGet(APIManager *self);
    static void callbackUserProfileGet(APIManager *self);
    static void callbackUserAlbumGet(APIManager *self);
    static void callbackUserAvatarGet(APIManager *self);
    static void callbackUserUpdate(APIManager *self);
    static void callbackUserGenderUpdate(APIManager *self);
    static void callbackUserPlaystyleUpdate(APIManager *self);
    static void callbackUserTitleUpdate(APIManager *self);
    static void callbackUserAvatarUpdate(APIManager *self);
    static void callbackUserKeybladeUpdate(APIManager *self);
    static void callbackUserMedalLock(APIManager *self);
    static void callbackUserSupportUpdate(APIManager *self);
    static void callbackMedalSkillUpdate(APIManager *self);
    static void callbackUserMaterialSell(APIManager *self);
    static void callbackUserMedalSell(APIManager *self);
    static void callbackSkillSell(APIManager *self);
    static void callbackKeybladeBurstMax(APIManager *self);
    static void callbackKeybladeEnhance(APIManager *self);
    static void callbackUserMedalEnhance(APIManager *self);
    static void callbackUserMedalEvolve(APIManager *self);
    static void callbackSkillEvolve(APIManager *self);
    static void callbackSystemMaster(APIManager *self);
    static void callbackSystemResource(APIManager *self);
    static void callbackSystemResourceEv(APIManager *self);
    static void callbackStageGet(APIManager *self);
    static void callbackStageGet160310(APIManager *self);
    static void callbackStageEventGet(APIManager *self);
    static void callbackStageSupportList(APIManager *self);
    static void callbackStageStart(APIManager *self);
    static void callbackStageRetire(APIManager *self);
    static void callbackStageContinue(APIManager *self);
    static void callbackStageClear(APIManager *self);
    static void callbackRaidGet(APIManager *self);
    static void callbackRaidList(APIManager *self);
    static void callbackRaidRewardGet(APIManager *self);
    static void callbackRaidRewardGet151101(APIManager *self);
    static void callbackRaidMemberGet(APIManager *self);
    static void callbackRaidStart(APIManager *self);
    static void callbackRaidClear(APIManager *self);
    static void callbackUserSphereCheck(APIManager *self);
    static void callbackUserSphereUpdate(APIManager *self);
    static void callbackUserPresentGet(APIManager *self);
    static void callbackUserPresentReceive(APIManager *self);
    static void callbackUserBirthdayGet(APIManager *self);
    static void callbackUserBirthdayCreate(APIManager *self);
    static void callbackSystemNeedUrl(APIManager *self);
    static void callbackSystemCoppaGet(APIManager *self);
    static void callbackSystemInformationList(APIManager *self);
    static void callbackSystemInformationList151203(APIManager *self);
    static void callbackSystemInformationDetailGet(APIManager *self);
    static void callbackShopMedalMax(APIManager *self);
    static void callbackShopApRecover(APIManager *self);
    static void callbackUserSphereBuy(APIManager *self);
    static void callbackUnionChange(APIManager *self);
    static void callbackGachaList(APIManager *self);
    static void callbackGachaDraw(APIManager *self);
    static void callbackPurchasePrepare(APIManager *self);
    static void callbackPurchaseFinish(APIManager *self);
    static void callbackPaymentInfo(APIManager *self);
    static void callbackPartyCreate(APIManager *self);
    static void callbackPartyUpdate(APIManager *self);
    static void callbackPartyNoticeUpdate(APIManager *self);
    static void callbackPartyLogin(APIManager *self);
    static void callbackPartyLeave(APIManager *self);
    static void callbackPartyMemberList(APIManager *self);
    static void callbackPartySearchActive(APIManager *self);
    static void callbackPartyApplication(APIManager *self);
    static void callbackPartyApplicationList(APIManager *self);
    static void callbackPartyApplicationCancel(APIManager *self);
    static void callbackPartyApplicantList(APIManager *self);
    static void callbackPartyApplicantAgree(APIManager *self);
    static void callbackPartyApplicantReject(APIManager *self);
    static void callbackPartyLeaderChange(APIManager *self);
    static void callbackPartyAdminAppoint(APIManager *self);
    static void callbackPartyAdminDismiss(APIManager *self);
    static void callbackPartyMemberExpel(APIManager *self);
    static void callbackPartyInviteSearch(APIManager *self);
    static void callbackPartyInviteOffer(APIManager *self);
    static void callbackPartyInviteList(APIManager *self);
    static void callbackPartyInviteCancel(APIManager *self);
    static void callbackPartyOfferList(APIManager *self);
    static void callbackPartyOfferAgree(APIManager *self);
    static void callbackPartyOfferReject(APIManager *self);
    static void callbackColosseumGet(APIManager *self);
    static void callbackColosseumRewardList(APIManager *self);
    static void callbackColosseumStart(APIManager *self);
    static void callbackColosseumRetire(APIManager *self);
    static void callbackColosseumClear(APIManager *self);
    static void callbackSystemSerialcode(APIManager *self);
    static void callbackSystemSerialcodeCampaignGet(APIManager *self);
    static void callbackSystemInvitecode(APIManager *self);
    static void callbackSystemPushRegist(APIManager *self);
    static void callbackSystemPushHelp(APIManager *self);
    static void callbackRankingWeeklyUser(APIManager *self);
    static void callbackRankingMonthlyUser(APIManager *self);
    static void callbackRankingWeeklyUserOwn(APIManager *self);
    static void callbackRankingMonthlyUserOwn(APIManager *self);
    static void callbackRankingLux(APIManager *self);
    static void callbackRankingLuxOwn(APIManager *self);
    static void callbackRankingParty(APIManager *self);
    static void callbackRankingPartyOwn(APIManager *self);
    static void callbackRankingColosseum(APIManager *self);
    static void callbackRankingColosseumOwn(APIManager *self);
    static void callbackRankingParadeGet(APIManager *self);
    static void callbackRankingRewardCheck(APIManager *self);
    static void callbackCampaignGet(APIManager *self);
    static void callbackMypageInfoGet(APIManager *self);
    static void callbackCampaignPincodeGet(APIManager *self);
    static void callbackCampaignPincodeCreate(APIManager *self);
    static void callbackCampaignPincodeUpdate(APIManager *self);
    static void callbackCampaignPincodeDelete(APIManager *self);
    static void callbackCampaignPincodeStatusUpdate(APIManager *self);
    static void callbackUserOptionUpdate(APIManager *self);
    static void callbackLinkMembersGet(APIManager *self);
    static void callbackUserLinkUpdate(APIManager *self);
    static void callbackUserLinkDelete(APIManager *self);
    static void callbackSystemUuidUpdate(APIManager *self);
    static void callbackUserAlbumChallengeUpdate(APIManager *self);

    // ---- requests of the common shapes (APIManager_requests.cpp); true = sent
    static bool executeTutorialUserCreate(APIManager *self, uint32_t cb);
    static bool executeUserShopGet(APIManager *self, uint32_t cb);
    static bool executeUserOptionGet(APIManager *self, uint32_t cb);
    static bool executeTutorialStatusGet(APIManager *self, uint32_t cb);
    static bool executeUserSphereGet(APIManager *self, uint32_t cb);
    static bool executeUserMedalGet(APIManager *self, uint32_t cb);
    static bool executeUserSkillGet(APIManager *self, uint32_t cb);
    static bool executeUserMaterialGet(APIManager *self, uint32_t cb);
    static bool executeUserKeybladeGet(APIManager *self, uint32_t cb);
    static bool executeUserAvatarAllGet(APIManager *self, uint32_t cb);
    static bool executeUserAvatarPartsGet(APIManager *self, uint32_t cb);
    static bool executeUserTitleGet(APIManager *self, uint32_t cb);
    static bool executeUserLinkGet(APIManager *self, uint32_t cb);
    static bool executeUserSupportGet(APIManager *self, uint32_t cb);
    static bool executeTutorialClear(APIManager *self, uint32_t cb);
    static bool executeUserAlbumGet(APIManager *self, uint32_t cb);
    static bool executeStageGet(APIManager *self, uint32_t cb);
    static bool executeStageGet160310(APIManager *self, uint32_t cb);
    static bool executeRaidGet(APIManager *self, uint32_t cb);
    static bool executeRaidList(APIManager *self, uint32_t cb);
    static bool executeRaidRewardGet(APIManager *self, uint32_t cb);
    static bool executeRaidRewardGet151101(APIManager *self, uint32_t cb);
    static bool executeUserPresentGet(APIManager *self, uint32_t cb);
    static bool executeUserBirthdayGet(APIManager *self, uint32_t cb);
    static bool executeShopMedalMax(APIManager *self, uint32_t cb);
    static bool executePaymentInfo(APIManager *self, uint32_t cb);
    static bool executePartyNoticeUpdate(APIManager *self, uint32_t cb);
    static bool executePartyLogin(APIManager *self, uint32_t cb);
    static bool executePartyApplicationList(APIManager *self, uint32_t cb);
    static bool executePartyOfferList(APIManager *self, uint32_t cb);
    static bool executeColosseumGet(APIManager *self, uint32_t cb);
    static bool executeColosseumRewardList(APIManager *self, uint32_t cb);
    static bool executeSystemSerialcodeCampaignGet(APIManager *self, uint32_t cb);
    static bool executeRankingWeeklyUser(APIManager *self, uint32_t cb);
    static bool executeRankingMonthlyUser(APIManager *self, uint32_t cb);
    static bool executeRankingWeeklyUserOwn(APIManager *self, uint32_t cb);
    static bool executeRankingMonthlyUserOwn(APIManager *self, uint32_t cb);
    static bool executeRankingLux(APIManager *self, uint32_t cb);
    static bool executeRankingLuxOwn(APIManager *self, uint32_t cb);
    static bool executeRankingParty(APIManager *self, uint32_t cb);
    static bool executeRankingPartyOwn(APIManager *self, uint32_t cb);
    static bool executeRankingColosseum(APIManager *self, uint32_t cb);
    static bool executeRankingColosseumOwn(APIManager *self, uint32_t cb);
    static bool executeRankingParadeGet(APIManager *self, uint32_t cb);
    static bool executeCampaignGet(APIManager *self, uint32_t cb);
    static bool executeMypageInfoGet(APIManager *self, uint32_t cb);
    static bool executeShopApRecover(APIManager *self, uint32_t cb);
    static bool executePartyLeave(APIManager *self, uint32_t cb);
    static bool executePartyApplicantList(APIManager *self, uint32_t cb);
    static bool executePartyInviteList(APIManager *self, uint32_t cb);
    static bool executeSystemInvitecode(APIManager *self, uint32_t cb);
    static bool executeSystemPushHelp(APIManager *self, uint32_t cb);
    static bool executeCampaignPincodeGet(APIManager *self, uint32_t cb);
    static bool executeCampaignPincodeCreate(APIManager *self, uint32_t cb);
    static bool executeCampaignPincodeDelete(APIManager *self, uint32_t cb);
    static bool executeCampaignPincodeStatusUpdate(APIManager *self, uint32_t cb);
    static bool executeLoginBonus(APIManager *self, uint32_t cb);
    static bool executeStageSupportList(APIManager *self, uint32_t cb);
    static bool executeStageContinue(APIManager *self, uint32_t cb);
    static bool executeSystemNeedUrl(APIManager *self, uint32_t cb);
    static bool executeSystemCoppaGet(APIManager *self, uint32_t cb);
    static bool executeSystemInformationList151203(APIManager *self, uint32_t cb);
    static bool executeUserDetailGet(APIManager *self, uint64_t userId, uint32_t cb);
    static bool executeKeybladeBurstMax(APIManager *self, uint64_t userKeybladeId, uint32_t cb);
    static bool executeRaidMemberGet(APIManager *self, uint64_t raidId, uint32_t cb);
    static bool executePartyApplicationCancel(APIManager *self, uint64_t partyApplicantId, uint32_t cb);
    static bool executePartyApplicantAgree(APIManager *self, uint64_t partyApplicantId, uint32_t cb);
    static bool executePartyApplicantReject(APIManager *self, uint64_t partyApplicantId, uint32_t cb);
    static bool executePartyLeaderChange(APIManager *self, uint64_t appointId, uint32_t cb);
    static bool executePartyAdminAppoint(APIManager *self, uint64_t appointId, uint32_t cb);
    static bool executePartyAdminDismiss(APIManager *self, uint64_t dismissId, uint32_t cb);
    static bool executePartyMemberExpel(APIManager *self, uint64_t expelId, uint32_t cb);
    static bool executePartyInviteCancel(APIManager *self, uint64_t partyOfferId, uint32_t cb);
    static bool executePartyOfferAgree(APIManager *self, uint64_t partyOfferId, uint32_t cb);
    static bool executePartyOfferReject(APIManager *self, uint64_t partyOfferId, uint32_t cb);
    static bool executeUserSphereCheck(APIManager *self, uint32_t checkType, uint32_t cb);
    static bool executeSystemInformationList(APIManager *self, uint32_t type, uint32_t cb);
    static bool executeSystemInformationDetailGet(APIManager *self, uint32_t infoId, uint32_t cb);
    static bool executeUserSphereBuy(APIManager *self, uint32_t sphereBoardId, uint32_t cb);
    static bool executeRankingRewardCheck(APIManager *self, uint32_t type, uint32_t cb);
    static bool executeUserAlbumChallengeUpdate(APIManager *self, uint32_t challengeId, uint32_t cb);
    static bool executeSystemMaster(APIManager *self, uint32_t revision, uint32_t cb);
    static bool executeSystemResource(APIManager *self, uint32_t revision, uint32_t cb);
    static bool executeStageEventGet(APIManager *self, uint32_t eventCategory, uint32_t cb);

    // ---- login and the login chain (APIManager_login.cpp)
    static bool executeLogin(APIManager *self, uint32_t cb);
    static bool executeUserStart(APIManager *self, uint32_t cb);
    static bool executeUserChatGet(APIManager *self, uint32_t cb);
    static bool executeUserStoneGet(APIManager *self, uint32_t cb);
    static bool executePartyGet(APIManager *self, uint64_t partyId, uint32_t cb);
    static bool executePartyMemberList(APIManager *self, uint32_t detail, uint32_t cb, uint32_t platformType);

    // ---- quests (APIManager_stage.cpp)
    static bool executeStageStart(APIManager *self, int32_t stageId, uint64_t supportUserId, uint64_t userKeybladeId,
                                  uint32_t skip, uint32_t cb);
    static bool executeStageClear(APIManager *self, uint32_t cb);

    // ---- cached user data updates (APIManager_userdata.cpp); item arguments are guest addresses
    static void updateKeyblade(APIManager *self, uint32_t keyblade);
    static void updateMedal(APIManager *self, uint32_t medal);
    static void updateSkill(APIManager *self, uint32_t skill);
    static void updateMaterial(APIManager *self, uint32_t material);
    static void removeMedals(APIManager *self, uint32_t ids);
    static void removeSkills(APIManager *self, uint32_t ids);
    static void updateTitle(APIManager *self, uint32_t title);
    static void updateFavoriteKeyblade(APIManager *self, uint64_t id);
    static void updateAvatarPart(APIManager *self, uint32_t part);
    static void updateCoordinate(APIManager *self, uint32_t coordinate);
    static void updateMoney(APIManager *self, int32_t money);
    static int32_t findMedalSetKeybladeIndex(APIManager *self, uint64_t medalId);

    // ---- requests with scalar / string parameters (APIManager_params.cpp)
    static bool executeUserSupportUpdate(APIManager *self, uint64_t supportMedalId, uint32_t cb);
    static bool executeKeybladeEnhance(APIManager *self, uint64_t userKeybladeId, uint32_t cb);
    static bool executeUserAvatarGet(APIManager *self, uint64_t targetUserId, uint32_t cb);
    static bool executeUserProfileGet(APIManager *self, uint64_t userId, uint32_t platformType, uint32_t cb);
    static bool executeUserGenderUpdate(APIManager *self, int32_t gender, uint32_t cb);
    static bool executeUserTitleUpdate(APIManager *self, uint64_t left, uint64_t right, uint64_t plate, uint32_t cb);
    static bool executeUserMaterialSell(APIManager *self, uint64_t userMaterialId, int32_t number, uint32_t cb);
    static bool executeUserSphereUpdate(APIManager *self, int32_t sphereBoardId, uint32_t masuIds, uint32_t unused, uint32_t cb);
    static bool executeUserOptionUpdate(APIManager *self, uint32_t option, uint32_t cb);
    static bool executeUnionChange(APIManager *self, int32_t unionId, uint32_t cb);
    static bool executeGachaList(APIManager *self, uint32_t tutorial, uint32_t cb);
    static bool executeGachaDraw(APIManager *self, int32_t drawMedalTypeId, int32_t count, uint32_t cb);
    static bool executeRaidStart(APIManager *self, uint64_t raidId, uint64_t supportUserId, uint64_t userKeybladeId, uint32_t cb);
    static bool executeColosseumStart(APIManager *self, int32_t rank, int32_t stageCount, int32_t stageId, uint64_t userKeybladeId, uint32_t cb);
    static bool executePartySearchActive(APIManager *self, int32_t playStyle, int32_t rank, int32_t agreeType, int32_t memberCount, uint32_t cb);
    static bool executePartyApplication(APIManager *self, uint64_t partyId, uint32_t message, uint32_t cb);
    static bool executePartyInviteOffer(APIManager *self, uint64_t userId, uint32_t message, uint32_t cb);
    static bool executeSystemSerialcode(APIManager *self, uint32_t serialCode, uint32_t cb);
    static bool executeCampaignPincodeUpdate(APIManager *self, uint32_t pinCode, uint32_t cb);
    static bool executeSystemPushRegist(APIManager *self, uint32_t deviceToken, uint32_t cb);
    static bool executeUserLinkUpdate(APIManager *self, uint32_t accessToken, uint32_t platformType, uint32_t cb);
    static bool executeUserLinkDelete(APIManager *self, uint32_t accessToken, uint32_t platformType, uint32_t cb);
    static bool executeSystemUuidUpdate(APIManager *self, uint32_t accessToken, uint32_t platformType, uint32_t cb);

    // ---- requests with list / state parameters (APIManager_lists.cpp)
    static bool executeUserMedalSell(APIManager *self, uint32_t userMedalIds, uint32_t cb);
    static bool executeSkillSell(APIManager *self, uint32_t userSkillIds, uint32_t cb);
    static bool executeUserPresentReceive(APIManager *self, uint32_t presentIds, uint32_t cb);
    static bool executeUserMedalEnhance(APIManager *self, uint64_t baseUserMedalId, uint32_t componentIds, uint32_t overwriteSkill, uint32_t cb);
    static bool executeUserMedalEvolve(APIManager *self, uint64_t baseUserMedalId, uint32_t attachmentIds, uint32_t cb);
    static bool executeSkillEvolve(APIManager *self, uint64_t baseUserSkillId, uint32_t componentIds, uint32_t cb);
    static bool executeUserMedalLock(APIManager *self, uint32_t locked, uint32_t unlocked, uint32_t cb);
    static bool executeSystemResourceEv(APIManager *self, uint32_t resourceIds, uint32_t cb);
    static bool executeLinkMembersGet(APIManager *self, uint32_t platformIds, uint32_t platformType, uint32_t cb);
    static bool executeUserPlaystyleUpdate(APIManager *self, uint32_t timezones, int32_t frequency, uint32_t cb);
    static bool executePartyInviteSearch(APIManager *self, int32_t level, uint32_t timezones, int32_t frequency, int32_t luxRank, uint32_t cb);
    static bool executeUserUpdate(APIManager *self, uint32_t name, uint32_t comment, uint32_t cb);
    static bool executePartyCreate(APIManager *self, uint32_t name, uint32_t message, int32_t playStyle, int32_t agreeType, uint32_t cb);
    static bool executePartyUpdate(APIManager *self, uint32_t name, uint32_t message, int32_t playStyle, int32_t agreeType, uint32_t cb);
    static bool executePurchasePrepare(APIManager *self, uint32_t productId, uint32_t priceLocale, uint32_t cb);
    static bool executePurchaseFinish(APIManager *self, uint32_t transaction, uint32_t cb);
    static bool executeUserBirthdayCreate(APIManager *self, int32_t year, int32_t month, uint32_t cb);
    static bool executeTutorialStatusUpdate(APIManager *self, uint32_t phase, uint64_t popupFlag, uint32_t finished, uint32_t cb);
    static bool executeStageRetire(APIManager *self, uint32_t cb);
    static bool executeColosseumRetire(APIManager *self, uint32_t cb);

    // ---- requests with nested parameters (APIManager_nested.cpp)
    static bool executeMedalSkillUpdate(APIManager *self, uint32_t medalSkills, uint32_t cb);
    static bool executeUserKeybladeUpdate(APIManager *self, uint32_t keyblades, uint32_t cb);
    static bool executeUserAvatarUpdate(APIManager *self, uint32_t coordinates, int32_t defaultNo, uint32_t cb);
    static bool executeTutorialProgress(APIManager *self, int32_t progression, int32_t year, int32_t month, uint32_t name, uint32_t inviteCode, int32_t unionId, uint32_t coordinate, uint32_t cb);
    static bool executeColosseumClear(APIManager *self, uint32_t cb);
    static bool executeRaidClear(APIManager *self, int32_t count, uint32_t parts, uint32_t cb);
};
RW_LAYOUT(APIManager, connecting, 0x04);
RW_LAYOUT(APIManager, errorMessage, 0x08);
RW_LAYOUT(APIManager, failed, 0x14);
RW_LAYOUT(APIManager, status, 0x18);
RW_LAYOUT(APIManager, action, 0x1C);
RW_LAYOUT(APIManager, params, 0x20);
RW_LAYOUT(APIManager, callback, 0x28);
RW_LAYOUT(APIManager, loginChain, 0x40);
RW_LAYOUT(APIManager, ret, 0x44);
RW_LAYOUT(APIManager, login, 0x4C);
RW_LAYOUT(APIManager, user, 0x74);
RW_LAYOUT(APIManager, userPoint, 0x8C);
RW_LAYOUT(APIManager, userDetail, 0x94);
RW_LAYOUT(APIManager, userAvatarAll, 0xC4);
RW_LAYOUT(APIManager, userMedal, 0xD4);
RW_LAYOUT(APIManager, userKeyblade, 0xEC);
RW_LAYOUT(APIManager, userTitle, 0x10C);
RW_LAYOUT(APIManager, stageStart, 0x13C);
RW_LAYOUT(APIManager, raids, 0x154);
RW_LAYOUT(APIManager, resp_160, 0x160);
RW_LAYOUT(APIManager, string_1c8, 0x1C8);
RW_LAYOUT(APIManager, resp_1d4, 0x1D4);
RW_LAYOUT(APIManager, partyApplications, 0x204);
RW_LAYOUT(APIManager, resp_210, 0x210);
RW_LAYOUT(APIManager, resp_24c, 0x24C);
RW_LAYOUT(APIManager, ints_254, 0x254);
RW_LAYOUT(APIManager, colosseumStart, 0x260);
RW_LAYOUT(APIManager, systemNeedUrl, 0x1A8);
RW_LAYOUT(APIManager, tutorialStatus, 0x64);
RW_LAYOUT(APIManager, ints_270, 0x270);
RW_LAYOUT(APIManager, resp_27c, 0x27C);
RW_LAYOUT(APIManager, ints_2fc, 0x2FC);
RW_LAYOUT(APIManager, ints_308, 0x308);
RW_LAYOUT(APIManager, resp_314, 0x314);
RW_LAYOUT(APIManager, stageRequest, 0x33C);
RW_LAYOUT(APIManager, string_360, 0x360);
RW_SIZE(APIManager, 0x36C);

// ------------------------------------------------------------------------------------------ helpers
namespace apim {

// Guest functions used throughout (Thumb addresses).
constexpr uint32_t SHARED_ADD = 0xC8DBB5;          // std::__shared_weak_count::__add_shared
constexpr uint32_t SHARED_RELEASE = 0xC8DBD1;      // std::__shared_weak_count::__release_shared
constexpr uint32_t OPERATOR_DELETE = 0xC98F9B;     // operator delete(void*)
constexpr uint32_t OPERATOR_NEW = 0xC98EF1;        // operator new(unsigned)
constexpr uint32_t FN_BOOL_COPY = 0x45C84D;        // std::function<void(bool)>::function(const function&)
constexpr uint32_t FN_BOOL_ASSIGN = 0x490109;      // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FN_BOOL_CALL = 0x474139;        // std::function<void(bool)>::operator()(bool)
constexpr uint32_t FN_BIND_RESPOND = 0x4C553D;     // std::function<StatusCode(...)>(std::bind(&APIManager::onRespond, ...))
constexpr uint32_t CLIENT_GET_INSTANCE = 0x4B6F15; // hole::network::api::Client::getInstance
constexpr uint32_t CLIENT_CALL = 0x4B7201;         // Client::call(Action, const function&, const shared_ptr<Document>&, bool, bool)
constexpr uint32_t MAKE_SHARED_DOCUMENT = 0x4B9345;// std::make_shared<rapidjson::Document>()
constexpr uint32_t ADD_MEMBER_VALUE = 0x49A225;    // GenericValue::AddMember(GenericValue& name, GenericValue& value, Allocator&)
constexpr uint32_t ADD_MEMBER_U64 = 0x4C5FE5;      // GenericValue::AddMember<unsigned long long>(const char*, T, Allocator&)
constexpr uint32_t ADD_MEMBER_I32 = 0x499801;      // GenericValue::AddMember<int>
constexpr uint32_t ADD_MEMBER_U32 = 0x4C54C9;      // GenericValue::AddMember<unsigned>
constexpr uint32_t ADD_MEMBER_U8 = 0x4C8149;       // GenericValue::AddMember<unsigned char>
constexpr uint32_t ADD_MEMBER_I8 = 0x4D1CF1;       // GenericValue::AddMember<signed char>
constexpr uint32_t VALUE_FROM_STRING = 0x4B90F9;   // GenericValue::GenericValue(const char*, Allocator&) (copied string)
constexpr uint32_t PUSH_BACK_VALUE = 0x4C8E91;     // GenericValue::PushBack(GenericValue&, Allocator&)
constexpr uint32_t PUSH_BACK_U64 = 0x4C9175;       // GenericValue::PushBack<unsigned long long>(T, Allocator&)
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;
constexpr uint32_t ONRESPOND_GOT = 0xFD6488;       // GOT slot holding &APIManager::onRespond

constexpr uint32_t STACK_GUARD_GOT = 0xFD6044;     // GOT slot of __stack_chk_guard

// rapidjson GenericValue type flags (the 4th word of a value)
constexpr uint32_t JSON_OBJECT = 3;
constexpr uint32_t JSON_ARRAY = 4;

// The function's own guest stack frame (the same size as the original's, so its locals sit at the same
// addresses): lowers sp for the frame's lifetime. Calls through the frame put stack arguments at sp+0, where
// the original's outgoing-argument area is, so the callee runs on the same stack as under the lifted code.
class Frame {
public:
    explicit Frame(uint32_t size) : saved_(rw::cpu->r[13]), base_(saved_ - size) { rw::cpu->r[13] = base_; }
    ~Frame() { rw::cpu->r[13] = saved_; }
    Frame(const Frame &) = delete;
    Frame &operator=(const Frame &) = delete;

    uint32_t at(uint32_t off) const { return base_ + off; }          // guest address sp + off
    uint32_t word(uint32_t off) const { return rw::load<uint32_t>(base_ + off); }
    void setWord(uint32_t off, uint32_t v) const { rw::save<uint32_t>(base_ + off, v); }

    uint32_t call(uint32_t fn, std::initializer_list<uint32_t> args) const {
        rw::cpu->r[13] = base_ + (args.size() > 4 ? uint32_t(4 * (args.size() - 4)) : 0u);
        uint32_t r = rw::call(fn, args);
        rw::cpu->r[13] = base_;
        return r;
    }
    uint64_t call64(uint32_t fn, std::initializer_list<uint32_t> args) const {
        rw::cpu->r[13] = base_ + (args.size() > 4 ? uint32_t(4 * (args.size() - 4)) : 0u);
        uint64_t r = rw::call64(fn, args);
        rw::cpu->r[13] = base_;
        return r;
    }
    // virtual call: the function pointer at vtable + slot * 4 of the object at guest address obj
    uint32_t vcall(uint32_t obj, uint32_t slot, std::initializer_list<uint32_t> args) const {
        uint32_t vtable = rw::load<uint32_t>(obj);
        return call(rw::load<uint32_t>(vtable + slot * 4), args);
    }

private:
    uint32_t saved_, base_;
};

inline uint32_t addr(const void *p) { return rw::guest_addr(p); }
inline uint32_t load32(uint32_t a) { return rw::load<uint32_t>(a); }
inline uint8_t load8(uint32_t a) { return rw::load<uint8_t>(a); }

inline void addShared(const Frame &F, uint32_t ctrl) { F.call(SHARED_ADD, {ctrl}); }
inline void releaseShared(const Frame &F, uint32_t ctrl) { F.call(SHARED_RELEASE, {ctrl}); }
inline void releaseIfSet(const Frame &F, uint32_t ctrl) { if (ctrl) releaseShared(F, ctrl); }

// libc++ ~function(): destroy() in place (slot 4) or destroy_deallocate() on the heap copy (slot 5).
inline void destroyFunction(const Frame &F, uint32_t fn) {
    uint32_t f = load32(fn + 16);
    if (f == fn) F.vcall(f, 4, {f});
    else if (f) F.vcall(f, 5, {f});
}

// std::shared_ptr<T>::operator=(const shared_ptr&) as inlined: add a reference to the new control block,
// store, release the old one.
inline void assignShared(const Frame &F, StdSharedPtr &dst, uint32_t ptr, uint32_t ctrl) {
    if (ctrl) addShared(F, ctrl);
    dst.ptr = ptr;
    uint32_t old = dst.ctrl;
    dst.ctrl = ctrl;
    releaseIfSet(F, old);
}

// std::shared_ptr reset(): null the pointer, release the old control block.
inline void resetShared(const Frame &F, StdSharedPtr &sp) {
    sp.ptr = 0;
    uint32_t old = sp.ctrl;
    sp.ctrl = 0;
    releaseIfSet(F, old);
}

// One decoded byte / 64-bit value of a SafeBuffer at data position index (SafeBuffer::read without the copy).
inline uint8_t sbByte(uint32_t sb, uint32_t index) {
    const SafeBuffer *b = rw::host_ptr<SafeBuffer>(sb);
    return uint8_t(rw::load<uint8_t>(b->data.addr + index) ^ SafeBuffer::mask(b->key, index));
}
inline uint32_t sbWord(uint32_t sb, uint32_t index) {
    uint32_t v = 0;
    for (uint32_t k = 0; k < 4; k++) v |= uint32_t(sbByte(sb, index + k)) << (8 * k);
    return v;
}
inline uint64_t sbU64(uint32_t sb, uint32_t index) {
    return uint64_t(sbWord(sb, index)) | (uint64_t(sbWord(sb, index + 4)) << 32);
}

// C string of a libc++ std::string at guest address s.
inline uint32_t cstr(uint32_t s) { return (load8(s) & 1) ? load32(s + 8) : s + 1; }
inline uint32_t strSize(uint32_t s) { uint8_t b = load8(s); return (b & 1) ? load32(s + 4) : uint32_t(b >> 1); }

}  // namespace apim

// ------------------------------------------------------------------------------------------ request building
namespace apim {

inline void put32(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }

// The guard of every request a scene starts: refuse while another request runs (unless allowWhileBusy) or
// after a failure; the callback then gets false.
inline bool refuseRequest(const Frame &F, APIManager *self, uint32_t cb) {
    if (self->failed || (!self->allowWhileBusy && self->connecting)) {
        if (load32(cb + 16)) F.call(FN_BOOL_CALL, {cb, 0});
        return true;
    }
    return false;
}

// make_shared<rapidjson::Document>() into the frame slot (shared_ptr: document, control block), made an empty
// JSON object (SetObject). Returns the document.
inline uint32_t newParams(const Frame &F, uint32_t slot) {
    F.call(MAKE_SHARED_DOCUMENT, {F.at(slot)});
    uint32_t doc = F.word(slot);
    if (doc) {
        put32(doc + 0, 0);
        put32(doc + 4, 0);
        put32(doc + 8, 0);
        put32(doc + 12, JSON_OBJECT);
    }
    return doc;
}
// The document's MemoryPoolAllocator (Document::allocator_).
inline uint32_t allocatorOf(uint32_t doc) { return load32(doc + 0x10); }

// self->params = the frame's document (a copy). Returns the control block: the frame's own reference, released
// when the request was sent.
inline uint32_t keepParams(const Frame &F, APIManager *self, uint32_t slot) {
    uint32_t ctrl = F.word(slot + 4);
    assignShared(F, self->params, F.word(slot), ctrl);
    return ctrl;
}

// Send self->params as `action`: remember the callback, mark the connection busy and Client::call with
// std::bind(&APIManager::onRespond, self, action, _1, _2, _3). fnSlot / resultSlot: the frame's std::function
// and the returned shared_ptr<Request>.
inline void sendRequest(const Frame &F, APIManager *self, int32_t action, uint32_t cb, uint32_t fnSlot,
                        uint32_t resultSlot) {
    self->action = action;
    F.call(FN_BOOL_ASSIGN, {addr(&self->callback), cb});
    self->connecting = 1;
    uint32_t client = F.call(CLIENT_GET_INSTANCE, {});
    uint32_t act = uint32_t(self->action.get());
    const uint32_t fn = F.at(fnSlot), result = F.at(resultSlot);
    F.call(FN_BIND_RESPOND, {fn, load32(ONRESPOND_GOT), 0, addr(self), act, 0});
    F.call(CLIENT_CALL, {result, client, act, fn, addr(&self->params), 0, 0});
    releaseIfSet(F, load32(result + 4));
    destroyFunction(F, fn);
}

// rapidjson GenericValue::AddMember<T>(name, value, allocator) on the object `obj`.
inline void addI32(const Frame &F, uint32_t obj, uint32_t name, uint32_t value, uint32_t alloc) {
    F.call(ADD_MEMBER_I32, {obj, name, value, alloc});
}
inline void addU32(const Frame &F, uint32_t obj, uint32_t name, uint32_t value, uint32_t alloc) {
    F.call(ADD_MEMBER_U32, {obj, name, value, alloc});
}
inline void addU8(const Frame &F, uint32_t obj, uint32_t name, uint32_t value, uint32_t alloc) {
    F.call(ADD_MEMBER_U8, {obj, name, value, alloc});
}
inline void addI8(const Frame &F, uint32_t obj, uint32_t name, uint32_t value, uint32_t alloc) {
    F.call(ADD_MEMBER_I8, {obj, name, value, alloc});
}
inline void addU64(const Frame &F, uint32_t obj, uint32_t name, uint64_t value, uint32_t alloc) {
    F.call(ADD_MEMBER_U64, {obj, name, uint32_t(value), uint32_t(value >> 32), alloc});
}

}  // namespace apim

namespace apim {

// The guard of the login chain's requests: skipped while the chain runs (it is the running connection).
inline bool refuseUnlessChain(const Frame &F, APIManager *self, uint32_t cb) {
    if (self->loginChain) return false;
    if (refuseRequest(F, self, cb)) return true;
    self->allowWhileBusy = 0;
    return false;
}

// rapidjson GenericValue words at a frame slot: a constant-string name (StringRef: kConstStringFlag) and an
// empty array (kArrayType).
constexpr uint32_t JSON_CONST_STRING = 0x100005;
constexpr uint32_t JSON_COPY_STRING = 0x300005;
inline void setNameValue(const Frame &F, uint32_t slot, uint32_t chars, uint32_t length) {
    F.setWord(slot + 12, JSON_CONST_STRING);
    F.setWord(slot + 0, chars);
    F.setWord(slot + 4, length);
}
inline void setEmptyArray(const Frame &F, uint32_t slot) {
    F.setWord(slot + 0, 0);
    F.setWord(slot + 4, 0);
    F.setWord(slot + 8, 0);
    F.setWord(slot + 12, JSON_ARRAY);
}
// GenericValue::AddMember(GenericValue& name, GenericValue& value, Allocator&)
inline void addMemberValue(const Frame &F, uint32_t obj, uint32_t nameSlot, uint32_t valueSlot, uint32_t alloc) {
    F.call(ADD_MEMBER_VALUE, {obj, F.at(nameSlot), F.at(valueSlot), alloc});
}

}  // namespace apim

namespace apim {

// memcpy between guest addresses (the import), as logged stores
inline void copyGuest(uint32_t dst, uint32_t src, uint32_t n) {
    uint32_t k = 0;
    for (; k + 4 <= n; k += 4) rw::save<uint32_t>(dst + k, load32(src + k));
    for (; k < n; k++) rw::store8(rw::host_ptr<uint8_t>(dst + k), load8(src + k));
}

// std::vector<T>::push_back(const T&) of a trivially copyable T as libc++ inlines it: copy at end (when the
// storage has room), else __push_back_slow_path. atLeast: the room test is end >= cap instead of end == cap.
inline void pushBackCopy(const Frame &F, uint32_t vec, uint32_t src, uint32_t size, uint32_t slowPath,
                         bool atLeast = false) {
    uint32_t end = load32(vec + 4), cap = load32(vec + 8);
    if (atLeast ? end >= cap : end == cap) {
        F.call(slowPath, {vec, src});
        return;
    }
    uint32_t p = 0;
    if (end) {
        copyGuest(end, src, size);
        p = end;
    }
    rw::save<uint32_t>(vec + 4, p + size);
}

}  // namespace apim

namespace apim {

// The frame slots of a request with parameters (the original's stack layout): the shared_ptr<Document>, the
// std::function bound to onRespond and the shared_ptr<Request> Client::call returns.
struct RequestSlots {
    uint32_t doc, fn, result;
};

// The end of a request with parameters: retryable flag, self->params = the frame's document, send, then the
// frame's own reference is dropped. Returns true (sent).
inline bool sendWithParams(const Frame &F, APIManager *self, int32_t action, uint32_t cb, uint8_t retryable,
                           const RequestSlots &S) {
    self->retryable = retryable;
    uint32_t ctrl = keepParams(F, self, S.doc);
    sendRequest(F, self, action, cb, S.fn, S.result);
    releaseIfSet(F, ctrl);
    return true;
}

// A copied-string member: GenericValue(chars, allocator) at valueSlot, the constant name at nameSlot.
inline void addString(const Frame &F, uint32_t obj, uint32_t name, uint32_t nameLength, uint32_t chars,
                      uint32_t alloc, uint32_t valueSlot, uint32_t nameSlot) {
    F.call(VALUE_FROM_STRING, {F.at(valueSlot), chars, alloc});
    setNameValue(F, nameSlot, name, nameLength);
    addMemberValue(F, obj, nameSlot, valueSlot, alloc);
}

}  // namespace apim

namespace apim {

// rapidjson number flags (the 4th word of a GenericValue)
constexpr uint32_t JSON_INT_NEGATIVE = 0x1606;     // GenericValue(int) with a negative value
constexpr uint32_t JSON_UINT_HIGH_BIT = 0x3A06;    // GenericValue(unsigned) with bit 31 set
constexpr uint32_t JSON_INT_POSITIVE = 0x3E06;     // a value that fits int, unsigned, int64 and uint64

// GenericValue(int) built in place at a frame slot (+8 is left as it is, like the original)
inline void setIntValue(const Frame &F, uint32_t slot, int32_t v) {
    F.setWord(slot + 12, JSON_INT_NEGATIVE);
    F.setWord(slot + 4, uint32_t(v >> 31));
    F.setWord(slot + 0, uint32_t(v));
    if (v >= 0) F.setWord(slot + 12, JSON_INT_POSITIVE);
}

// SetObject() on a value at a frame slot
inline void setObjectAt(const Frame &F, uint32_t slot) {
    F.setWord(slot + 0, 0);
    F.setWord(slot + 4, 0);
    F.setWord(slot + 8, 0);
    F.setWord(slot + 12, JSON_OBJECT);
}

// libc++ __tree_next: the in-order successor in a std::set / std::map (left +0, right +4, parent +8)
inline uint32_t treeNext(uint32_t node) {
    uint32_t right = load32(node + 4);
    if (right) {
        node = right;
        while (load32(node)) node = load32(node);
        return node;
    }
    for (;;) {
        uint32_t parent = load32(node + 8);
        bool isLeft = load32(parent) == node;
        node = parent;
        if (isLeft) return parent;
    }
}

}  // namespace apim
