// AppState: the client's global app state singleton (a function-local static at 0x1003A48, 0x170 bytes).
// It holds the app version, the SafeBuffer-backed records (personal info = UUID, BRIDGE native session, login
// info), the running quest's StageState, the per-screen sort / filter settings, master / resource revisions,
// event resources, "new" / "will lock" ID lists, tutorial bookkeeping, and the save-file plumbing.
//
// Persistence: everything is serialized into a SaveBin (an unordered_map<string, shared_ptr<bg::Data>>) and stored
// both in cocos2d::UserDefault under "data" and in a file (SaveDataToFile); load() reads it back (UserDefault first,
// the file as fallback) and calls reset() when neither parses. save() writes at once on the main thread; other
// threads raise `saveRequested` and wait on a condition variable until update() (main thread, every frame) saved.
//
// The nested types (MedalSortFilterState, OptionState, Version, LoginInfo...) are in AppStateTypes.*; they are
// called through the guest (rw::call) so this file compiles on its own.
#pragma once

#include "Guest.hpp"
#include "SafeBuffer.hpp"

// libc++ std::unordered_map (32-bit): bucket array, bucket count, first node (before-begin's next), size,
// max_load_factor (float).
struct GuestHashMap {
    rw::Field<uint32_t> buckets;        // +0
    rw::Field<uint32_t> bucketCount;    // +4
    rw::Field<uint32_t> first;          // +8
    rw::Field<uint32_t> size;           // +12
    rw::Field<uint32_t> maxLoadFactor;  // +16 (float bits)
};
RW_SIZE(GuestHashMap, 20);

// libc++ std::function<void()> (32-bit): a 16-byte inline buffer, then __f_ (points at the buffer when the
// callable is stored inline, else at a heap object, null when empty). __base vtable: slot 4 destroy(),
// slot 5 destroy_deallocate().
struct GuestFunction {
    const uint8_t buffer[16];         // +0
    rw::Field<uint32_t> impl;   // +16 __f_
};
RW_SIZE(GuestFunction, 20);

struct AppState {
    // ---- the app version (AppState::Version, read from the bundled "info/version" text)
    rw::Field<int32_t> versionMajor;         // +0x00
    rw::Field<int32_t> versionMinor;         // +0x04
    rw::Field<int32_t> versionPatch;         // +0x08
    rw::Field<uint8_t> versionValid;         // +0x0C
    const uint8_t pad_0D[3];
    // ---- shared_ptr members
    guest::SharedPtr personalInfo;           // +0x10 PersonalInfo (SafeBuffer: the 16-byte UUID at 0)
    guest::SharedPtr bridgeInfo;             // +0x18 BRIDGEInfo (SafeBuffer: the 32-char native session ID at 0)
    guest::SharedPtr stageState;             // +0x20 StageState of the running quest
    guest::SharedPtr loginInfo;              // +0x28 LoginInfo (SafeBuffer: file size 0, md5 4..35, date 36)
    guest::SharedPtr medalSortFilter[6];     // +0x30 deck, mixed, evolve, sell, medal list, equip skill (MedalSortFilterState)
    guest::SharedPtr powerUpKeyBladeSortFilter;  // +0x60 KeyBladeSortFilterState
    guest::SharedPtr selectKeyBladeSortFilter;   // +0x68 KeyBladeSortFilterState
    guest::SharedPtr sellSkillSortFilter;    // +0x70 SkillSortFilterState
    guest::SharedPtr avatarBoardFilter;      // +0x78 AvatarBoardFilterState
    guest::SharedPtr adventureSelectFilter;  // +0x80 AdventureSelectFilterState
    guest::SharedPtr optionState;            // +0x88 OptionState
    // ---- revisions
    rw::Field<int32_t> masterRevision;       // +0x90
    guest::Vector masterFileRevisions;       // +0x94 std::vector<int>
    rw::Field<int32_t> resourceRevision;     // +0xA0
    rw::Field<uint32_t> resourceSize;        // +0xA4
    rw::Field<uint8_t> popBenefitStone;      // +0xA8
    const uint8_t pad_A9[3];
    GuestHashMap trackingEvents;             // +0xAC a SaveBin of tracked event names
    GuestHashMap eventResources;             // +0xC0 unordered_map<unsigned, unsigned>: event resource id -> size
    rw::Field<int32_t> avatarBoardPosNormal;     // +0xD4
    rw::Field<int32_t> avatarBoardPosRaid;       // +0xD8
    rw::Field<int32_t> avatarBoardPosColosseum;  // +0xDC
    rw::Field<int32_t> avatarBoardPosStage;      // +0xE0
    rw::Field<uint8_t> isStartTutorialStage; // +0xE4
    const uint8_t pad_E5[3];
    GuestFunction tutorialCloseFunc;         // +0xE8 std::function<void()>
    const uint8_t pad_FC[4];                       // +0xFC (never written)
    rw::Field<int32_t> tutorialCloseId;      // +0x100 tutorial popup the close function belongs to
    // ---- ID lists
    guest::Vector newUserMedalIDs;           // +0x104 std::vector<unsigned long long>
    guest::Vector willLockUserMedalIDs;      // +0x110 std::vector<unsigned long long>
    guest::Vector willUnlockUserMedalIDs;    // +0x11C std::vector<unsigned long long>
    guest::Vector clearStageOldSelects;      // +0x128 std::vector<int>
    guest::Vector clearTutorialPopupIDs;     // +0x134 std::vector<int>
    guest::Vector newAvatarPartsIDs;         // +0x140 std::vector<int>
    guest::Vector newAvatarPartsSetIDs;      // +0x14C std::vector<int>
    guest::Vector newAvatarBoardIDs;         // +0x158 std::vector<int>
    // ---- cross-thread save request
    rw::Field<uint8_t> saveRequested;        // +0x164
    const uint8_t pad_165[3];
    rw::Field<uint32_t> saveMutex;           // +0x168 std::mutex
    rw::Field<uint32_t> saveDone;            // +0x16C std::condition_variable

    // ---- statics (guest globals)
    static constexpr uint32_t INSTANCE = 0x1003A48;         // getInstance()::instance
    static constexpr uint32_t INSTANCE_GUARD = 0x1003BB8;   // its guard
    static constexpr uint32_t MAIN_THREAD_ID = 0x1003A44;   // AppState::_mainThreadID (pthread_t)
    static constexpr uint32_t MAIN_THREAD_ID_SET = 0x1003A40;  // AppState::_isMainThreadIDInitialized (bool)

    static AppState *getInstance();
    static AppState *construct(AppState *self);
    static AppState *destruct(AppState *self);
    static void setMainThread();
    static uint32_t isMainThread();

    static void load(AppState *self);
    static void reset(AppState *self);
    static void save(AppState *self);
    static void update(AppState *self, float dt);
    static void saveMain(AppState *self);

    static int32_t getCurrentAP(const AppState *self, const int32_t *secondsToNext);
    static int32_t getFullChargeApSec(const AppState *self);
    static void createStageState(AppState *self, uint32_t playable);
    static void destroyStageState(AppState *self);

    static void getUUIDString(guest::String *out, const AppState *self);
    static void getNativeSessionIDString(guest::String *out, const AppState *self);
    static void setNativeSessionID(AppState *self, uint32_t id);

    static int32_t getMasterRevision(const AppState *self) { return self->masterRevision; }
    static guest::Vector *getMasterFileRevisions(AppState *self) { return &self->masterFileRevisions; }
    static void setMasterRevisions(AppState *self, int32_t revision, const guest::Vector *fileRevisions);
    static int32_t getResourceRevision(const AppState *self) { return self->resourceRevision; }
    static uint32_t getResourceSize(const AppState *self) { return self->resourceSize; }
    static void setResourceRevisionAndSize(AppState *self, int32_t revision, uint32_t size);
    static void setAvatarBoardPosNormal(AppState *self, int32_t pos);
    static int32_t getAvatarBoardPosNormal(const AppState *self) { return self->avatarBoardPosNormal; }
    static void setAvatarBoardPosRaid(AppState *self, int32_t pos);
    static int32_t getAvatarBoardPosRaid(const AppState *self) { return self->avatarBoardPosRaid; }
    static void setAvatarBoardPosColosseum(AppState *self, int32_t pos);
    static int32_t getAvatarBoardPosColosseum(const AppState *self) { return self->avatarBoardPosColosseum; }
    static void setAvatarBoardPosStage(AppState *self, int32_t pos);
    static int32_t getAvatarBoardPosStage(const AppState *self) { return self->avatarBoardPosStage; }

    static void setEventResourceIDAndSize(AppState *self, uint32_t id, uint32_t size);
    static GuestHashMap *getEventResources(AppState *self) { return &self->eventResources; }
    static uint32_t isStartTutorialStageGet(const AppState *self) { return self->isStartTutorialStage; }
    static void setIsStartTutorialStage(AppState *self, uint32_t value);
    static void enumerateEventResourceIDsToDownload(guest::Vector *out, AppState *self);
    static uint32_t needsUpdateData(AppState *self);
    static uint32_t needsUpdateApp(AppState *self, uint32_t version);

    static void createKeyBladeSortFilterState(guest::SharedPtr *out, AppState *self);
    static void createSkillSortFilterState(guest::SharedPtr *out, AppState *self);
    static void copyShared(guest::SharedPtr *out, const guest::SharedPtr &member);

    static guest::Vector *getNewUserMedalIDs(AppState *self) { return &self->newUserMedalIDs; }
    static guest::Vector *getWillLockUserMedalIDs(AppState *self) { return &self->willLockUserMedalIDs; }
    static guest::Vector *getWillUnLockUserMedalIDs(AppState *self) { return &self->willUnlockUserMedalIDs; }
    static guest::Vector *getClearStageOldSelects(AppState *self) { return &self->clearStageOldSelects; }
    static guest::Vector *getClearTutorialPopupIDs(AppState *self) { return &self->clearTutorialPopupIDs; }
    static guest::Vector *getNewAvatarPartsIDs(AppState *self) { return &self->newAvatarPartsIDs; }
    static guest::Vector *getNewAvatarPartsSetIDs(AppState *self) { return &self->newAvatarPartsSetIDs; }
    static guest::Vector *getNewAvatarBoardIDs(AppState *self) { return &self->newAvatarBoardIDs; }

    static uint32_t isClearTutorialPopupIDs(AppState *self, int32_t id);
    static uint32_t isClearTutorialPopupIDsInPhase(AppState *self, int32_t id);
    static void resetClearTutorialPopupIDs(AppState *self);
    static void setTutorialCloseFunc(GuestFunction *out, AppState *self, GuestFunction *func, int32_t id);
    static void getTutorialCloseFunc(GuestFunction *out, AppState *self, int32_t id);
    static void execTutorialCloseFunc(AppState *self, int32_t id);

    static void setPopBenefitStone(AppState *self, uint32_t value);
    static uint32_t checkTrackingEvent(AppState *self, uint32_t name);
    static void saveTrackingEvent(AppState *self, uint32_t name);
};
RW_LAYOUT(AppState, personalInfo, 0x10);
RW_LAYOUT(AppState, bridgeInfo, 0x18);
RW_LAYOUT(AppState, stageState, 0x20);
RW_LAYOUT(AppState, loginInfo, 0x28);
RW_LAYOUT(AppState, medalSortFilter, 0x30);
RW_LAYOUT(AppState, powerUpKeyBladeSortFilter, 0x60);
RW_LAYOUT(AppState, selectKeyBladeSortFilter, 0x68);
RW_LAYOUT(AppState, sellSkillSortFilter, 0x70);
RW_LAYOUT(AppState, avatarBoardFilter, 0x78);
RW_LAYOUT(AppState, adventureSelectFilter, 0x80);
RW_LAYOUT(AppState, optionState, 0x88);
RW_LAYOUT(AppState, masterRevision, 0x90);
RW_LAYOUT(AppState, masterFileRevisions, 0x94);
RW_LAYOUT(AppState, resourceRevision, 0xA0);
RW_LAYOUT(AppState, resourceSize, 0xA4);
RW_LAYOUT(AppState, popBenefitStone, 0xA8);
RW_LAYOUT(AppState, trackingEvents, 0xAC);
RW_LAYOUT(AppState, eventResources, 0xC0);
RW_LAYOUT(AppState, avatarBoardPosNormal, 0xD4);
RW_LAYOUT(AppState, avatarBoardPosStage, 0xE0);
RW_LAYOUT(AppState, isStartTutorialStage, 0xE4);
RW_LAYOUT(AppState, tutorialCloseFunc, 0xE8);
RW_LAYOUT(AppState, tutorialCloseId, 0x100);
RW_LAYOUT(AppState, newUserMedalIDs, 0x104);
RW_LAYOUT(AppState, willLockUserMedalIDs, 0x110);
RW_LAYOUT(AppState, willUnlockUserMedalIDs, 0x11C);
RW_LAYOUT(AppState, clearStageOldSelects, 0x128);
RW_LAYOUT(AppState, clearTutorialPopupIDs, 0x134);
RW_LAYOUT(AppState, newAvatarPartsIDs, 0x140);
RW_LAYOUT(AppState, newAvatarPartsSetIDs, 0x14C);
RW_LAYOUT(AppState, newAvatarBoardIDs, 0x158);
RW_LAYOUT(AppState, saveRequested, 0x164);
RW_LAYOUT(AppState, saveMutex, 0x168);
RW_LAYOUT(AppState, saveDone, 0x16C);
RW_SIZE(AppState, 0x170);
