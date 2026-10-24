// SceneBattleResult: create, destructor, init, initScene, nextResult, openNewLuxPopup, addResultRewardKeyBlade.
// (openLevelUpPopup and startResult are in SceneBattleResult_LevelUp.cpp / SceneBattleResult_Start.cpp.)
#include "SceneBattleResult.hpp"

#include "B2sCommon.hpp"

using namespace b2s;

namespace {
constexpr Addr GOT_vtable = 0xFD686C;                // vtable block of SceneBattleResult (+8: the vtable)
constexpr Addr Layer_ctor = rwlib::cocos2d::Layer::ctor_base;                // cocos2d::Layer::Layer()
constexpr Addr Layer_dtor = rwlib::cocos2d::Layer::dtor_base;                // cocos2d::Layer::~Layer()
constexpr Addr Layer_init = rwlib::cocos2d::Layer::init;                // cocos2d::Layer::init()
constexpr Addr VectorBase_ResutData_dtor = 0x511C85; // std::__vector_base<BattleResultScrollView::ResutData>::~__vector_base
constexpr Addr ResutData_pushBackSlow = 0x51517D;    // std::vector<ResutData>::__push_back_slow_path
constexpr Addr ResutData_copyCtor = 0x5153E7;        // BattleResultScrollView::ResutData::ResutData(const ResutData&)
constexpr Addr FunctionVoid_assign = 0x4EB9CD;       // std::function<void()>::operator=(const function&)
constexpr Addr FunctionBool_assign = 0x490109;       // std::function<void(bool)>::operator=(const function&)
constexpr Addr SceneManager_getPopupLayer2 = 0x613E71;
constexpr Addr SceneManager_changeBackground = 0x6128ED;   // (mgr, BackGroundType, const std::string&)
constexpr Addr SceneManager_changeSceneLayer = 0x612B99;   // (mgr, const function<Layer*()>&, bool, bool, FadeType)
constexpr Addr RaidBossLwfDialog_create = 0x59734D;
constexpr Addr RaidBossLwfDialog_open = 0x5973ED;    // (dialog, Node*, RaidBossLwfType, int, int, function<void()>)
constexpr Addr BattleResultDialog_setCloseEvent = 0x582B81;
constexpr Addr BattleResultDialog_openGettingItem = 0x581805;
constexpr Addr BattleResultDialog_openClearRank = 0x5821DD;
constexpr Addr SceneBattleResult_initScene = 0x6816ED;
constexpr Addr SceneBattleResult_afterInit = 0x681785;      // sub_681784(&this): the non-raid-boss start of the scene
constexpr Addr SceneBattleResult_openNewKeyBladePopup = 0x681CC5;
constexpr Addr SceneBattleResult_openLevelUpPopup = 0x681E2D;
constexpr Addr SceneBattleResult_openNewLuxPopup = 0x682AB1;
constexpr Addr SceneBattleResult_openUserTitlePopup = 0x682B49;
constexpr Addr SceneUtil_openNewLuxPopup = 0x46FDC5;
constexpr Addr SceneUtil_getNexTutorialphase = 0x472B31;
constexpr Addr SceneUtil_openTutorialPopup = 0x4684C9;
constexpr Addr SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;
constexpr Addr APIManager_executeUserSphereCheck = 0x4CCBC1;
constexpr Addr DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr Addr CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr Addr Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr Addr Reward_create = 0x78B5C1;             // master::Reward::create(shared_ptr out, id)
constexpr Addr SceneUtil_setItemImageData = 0x46C71D;

// lambda __func vtables created here
constexpr Addr VT_onInitA = 0xF647E4, VT_onInitB = 0xF647B8;           // init(): the two scene callbacks
constexpr Addr VT_raidBossClose = 0xF6478C;                            // init(): RaidBossLwfDialog close
constexpr Addr VT_closeGettingItem = 0xF64760, VT_closeClearRank = 0xF64734;
constexpr Addr VT_tutorialDone = 0xF64684;
constexpr Addr VT_sphereCheckDone = 0xF646DC;
constexpr Addr VT_adventureSelectDone = 0xF64708;
constexpr Addr VT_changeSceneLayerFn = 0xF646B0;
constexpr Addr VT_delayedCall = 0xF64810;
constexpr Addr VT_newLuxDone = 0xF645D4;

constexpr Addr STR_DeckBG = 0xE03AF2;                // "img/ui/Equip_DeckBG_01.png" (26)

// ResutData (0x44 bytes): the reward list entry the scroll view shows
constexpr uint32_t RD_SIZE = 0x44;
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
SceneBattleResult *SceneBattleResult::create() {
    guest::Frame F(16);
    const Addr obj = cx(OPERATOR_NEW, {0x2D8});
    cx(Layer_ctor, {obj});
    const Addr vt = ld32(GOT_vtable);
    st32(obj, vt + 8);
    cx(AEABI_MEMSET, {obj + 0x28C, 0x36, 0});
    cx(AEABI_MEMSET, {obj + 0x2C4, 0x14, 0});
    const Addr initFn = ld32(vt + 0x250);                  // vtable slot of init, read from the class vtable
    if (cx(initFn, {obj}) == 1) {
        cx(REF_AUTORELEASE, {obj});
        return rw::host_ptr<SceneBattleResult>(obj);
    }
    vcx(obj, 4, {obj});                                    // virtual slot 1: deleting destructor
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------
SceneBattleResult *SceneBattleResult::destruct(SceneBattleResult *self) {
    guest::Frame F(8);
    const Addr me = rw::guest_addr(self);
    self->vtable = ld32(GOT_vtable) + 8;
    cx(VectorBase_ResutData_dtor, {me + 0x2CC});
    // the five Property shared_ptrs and the clear data: release the control blocks, last one first
    for (Addr off : {0x2B8u, 0x2B0u, 0x2A8u, 0x2A0u, 0x298u, 0x290u}) {
        const Addr ctrl = ld32(me + off);
        if (ctrl) cx(SHARED_RELEASE, {ctrl});
    }
    cx(Layer_dtor, {me});
    return self;
}

// ---------------------------------------------------------------------------------------------------------------
uint32_t SceneBattleResult::init(SceneBattleResult *self) {
    guest::Frame F(0x130);
    const Addr me = rw::guest_addr(self);
    if (cx(Layer_init, {me}) != 1) return 0;

    // two scene callbacks (lambdas capturing this), assigned to the std::function members
    const Addr fnA = F.addr(0xF8);
    st32(fnA + 16, fnA);
    st32(fnA, VT_onInitA);
    st32(fnA + 4, me);
    cx(FunctionVoid_assign, {me + 0x210, fnA});
    fnDestroy(fnA);
    const Addr fnB = F.addr(0xE0);
    st32(fnB + 16, fnB);
    st32(fnB, VT_onInitB);
    st32(fnB + 4, me);
    cx(FunctionVoid_assign, {me + 0x228, fnB});
    fnDestroy(fnB);

    // the result kind from the current scene mode: 5 -> raid (2), 6 / 8 -> colosseum (1), else stage (0)
    const Addr mgr = cx(SCENEMANAGER_GETINSTANCE, {});
    const uint32_t mode = ld32(mgr + 0x28C);
    uint32_t kind = 0;
    if (mode == 5) kind = 2;
    else if (mode == 6 || mode == 8) kind = 1;
    self->resultKind = kind;
    cx(SceneBattleResult_initScene, {0x288});              // initScene ignores its argument (r0 = 0x288 here)

    st32(F.addr(0x1C), me);
    if (self->resultKind != 1 || ld32(ld32(cx(APIMANAGER_GETINSTANCE, {}) + 0x180)) != 3) {
        cx(SceneBattleResult_afterInit, {F.addr(0x1C)});
        return 1;
    }
    // raid boss intro: the dialog data (0xA8 bytes) comes from a SafeBuffer
    const Addr api = cx(APIMANAGER_GETINSTANCE, {});
    const Addr sb = ld32(api + 0x178);
    const Addr data = F.addr(0x38);
    cx(SAFEBUFFER_READ, {sb, data, ld32(sb + 0xC), 0xA8, 0});
    const Addr dialog = cx(RaidBossLwfDialog_create, {});
    const Addr popupLayer = cx(SceneManager_getPopupLayer2, {cx(SCENEMANAGER_GETINSTANCE, {})});
    const uint32_t bossA = ld32(F.addr(0x5C));
    const uint32_t bossB = ld32(F.addr(0x64));
    const Addr onClose = F.addr(0x20);
    st32(onClose + 16, onClose);
    st32(onClose, VT_raidBossClose);
    st32(onClose + 4, me);
    cx(RaidBossLwfDialog_open, {dialog, popupLayer, 2, bossA, bossB, onClose});
    fnDestroy(onClose);
    return 1;
}

// ---------------------------------------------------------------------------------------------------------------
void SceneBattleResult::initScene(SceneBattleResult *) {
    guest::Frame F(0x28);
    const Addr mgr = cx(SCENEMANAGER_GETINSTANCE, {});
    const Addr name = F.addr(4);
    strInit(name, STR_DeckBG, 0x1A);
    cx(SceneManager_changeBackground, {mgr, 0, name});
    strFree(name);
}

// ---------------------------------------------------------------------------------------------------------------
// Open the next pending result dialog, or leave the result screen when none is left.
void SceneBattleResult::nextResult(SceneBattleResult *self) {
    guest::Frame F(0x110);
    const Addr me = rw::guest_addr(self);
    const Addr dialogSlot = me + 0x2C8;

    if (self->showGettingItem) {
        self->showGettingItem = 0;
        const Addr fn = F.addr(0xC0);
        st32(fn + 16, fn);
        st32(fn, VT_closeGettingItem);
        st32(fn + 4, me);
        cx(BattleResultDialog_setCloseEvent, {ld32(dialogSlot), fn});
        fnDestroy(fn);
        const Addr dialog = ld32(dialogSlot);
        const Addr layer = cx(SCENEMANAGER_GETPOPUPLAYER, {cx(SCENEMANAGER_GETINSTANCE, {})});
        cx(BattleResultDialog_openGettingItem, {dialog, layer});
        return;
    }
    if (self->showClearRank) {
        self->showClearRank = 0;
        const Addr fn = F.addr(0xA8);
        st32(fn + 16, fn);
        st32(fn, VT_closeClearRank);
        st32(fn + 4, me);
        cx(BattleResultDialog_setCloseEvent, {ld32(dialogSlot), fn});
        fnDestroy(fn);
        const Addr dialog = ld32(dialogSlot);
        const Addr layer = cx(SCENEMANAGER_GETPOPUPLAYER, {cx(SCENEMANAGER_GETINSTANCE, {})});
        cx(BattleResultDialog_openClearRank, {dialog, layer});
        return;
    }
    if (self->showNewKeyblade) {
        cx(SceneBattleResult_openNewKeyBladePopup, {me});
        const uint32_t index = self->keybladeIndex + 1;
        self->keybladeIndex = index;
        // number of entries: (end - begin) / 0x44, as the compiler's multiplication by the inverse of 17
        const uint32_t count = uint32_t(int32_t(self->rewardKeyblades.end - self->rewardKeyblades.begin) >> 2) * 0xF0F0F0F1u;
        if (!(count > index)) self->showNewKeyblade = 0;
        return;
    }
    if (self->showLevelUp) {
        self->showLevelUp = 0;
        cx(SceneBattleResult_openLevelUpPopup, {me});
        return;
    }
    if (self->showNewLux) {
        self->showNewLux = 0;
        cx(SceneBattleResult_openNewLuxPopup, {me, 0});
        return;
    }
    if (self->showUserTitle) {
        self->showUserTitle = 0;
        cx(SceneBattleResult_openUserTitlePopup, {me, 0});
        return;
    }

    // nothing left: leave the result screen
    st8(cx(SCENEMANAGER_GETINSTANCE, {}) + 0x2F4, 1);
    const uint32_t mode = ld32(cx(SCENEMANAGER_GETINSTANCE, {}) + 0x28C);
    if (mode <= 7 && ((1u << mode) & 0x8E) != 0) {         // modes 1, 2, 3, 7
        const Addr mgr = cx(SCENEMANAGER_GETINSTANCE, {});
        const Addr emptyFn = F.addr(0x90);                 // an empty std::function<void(bool)>
        st32(emptyFn + 16, 0);
        st8(mgr + 0x4CC, 1);
        cx(FunctionBool_assign, {mgr + 0x548, emptyFn});
        fnDestroy(emptyFn);
        if (cx(SceneUtil_getNexTutorialphase, {}) == 995) {
            const Addr api = cx(APIMANAGER_GETINSTANCE, {});
            const Addr fn = F.addr(0x60);
            st32(fn + 16, fn);
            st32(fn, VT_sphereCheckDone);
            st16(fn + 4, 0);
            cx(APIManager_executeUserSphereCheck, {api, 3, fn});
            fnDestroy(fn);
        } else {
            const Addr fn = F.addr(0x78);
            st32(fn + 16, fn);
            st32(fn, VT_adventureSelectDone);
            cx(SceneAdventureSelect_callAdventureSelectAPI, {fn});
            fnDestroy(fn);
        }
        const Addr mgr2 = cx(SCENEMANAGER_GETINSTANCE, {});
        const Addr layerFn = F.addr(0x48);
        st32(layerFn + 16, layerFn);
        st32(layerFn, VT_changeSceneLayerFn);
        st32(layerFn + 4, me);
        cx(SceneManager_changeSceneLayer, {mgr2, layerFn, 1, 1, 1});
        fnDestroy(layerFn);
        return;
    }
    const uint32_t mode2 = ld32(cx(SCENEMANAGER_GETINSTANCE, {}) + 0x28C);
    if (mode2 != 6 && mode2 != 8) {
        // a short delay, then the call
        const Addr delay = cx(DelayTime_create, {0x3DCCCCCD});
        const Addr fn = F.addr(0xD8);
        st32(fn + 16, fn);
        st32(fn, VT_delayedCall);
        st16(fn + 4, 0);
        const Addr call = cx(CallFunc_create, {fn});
        const Addr seq = cx(Sequence_create, {delay, call, 0});
        cx(RUN_ACTION, {me, seq});
        fnDestroy(fn);
        return;
    }
    // colosseum: the tutorial popup 21
    const Addr callback = F.addr(0x18);
    st32(callback + 16, callback);
    st32(callback, VT_tutorialDone);
    st32(callback + 4, 0);
    st32(callback + 8, me);
    const Addr result = F.addr(0x30);
    cx(SceneUtil_openTutorialPopup, {result, 21, callback, 0, 0, 0, 0});
    fnDestroy(result);
    fnDestroy(callback);
}

// ---------------------------------------------------------------------------------------------------------------
void SceneBattleResult::openNewLuxPopup(SceneBattleResult *self, int32_t rank) {
    guest::Frame F(0x40);
    const Addr me = rw::guest_addr(self);
    const Addr fn = F.addr(8);
    st32(fn + 16, fn);
    st32(fn, VT_newLuxDone);
    st32(fn + 4, me);
    cx(SceneUtil_openNewLuxPopup, {uint32_t(rank), fn});
    fnDestroy(fn);
}

// ---------------------------------------------------------------------------------------------------------------
// Add the reward `index` of reward row `rewardId` to the keyblade list when it is a keyblade (type 13): the
// ResutData (0x44 bytes: three strings, ids, counts) is built in the frame and copied into the vector.
void SceneBattleResult::addResultRewardKeyBlade(SceneBattleResult *self, int32_t rewardId, int32_t index) {
    guest::Frame F(0xD0);
    const Addr me = rw::guest_addr(self);
    const Addr B = F.addr(0x28);                 // the ResutData under construction
    const Addr reward = F.addr(0x6C);            // shared_ptr<master::Reward>
    cx(Reward_create, {reward, uint32_t(rewardId)});
    {
        const Addr sb = ld32(reward);
        cx(SAFEBUFFER_READ, {sb, F.addr(0x74), ld32(sb + 0xC) + 4, 4, 4});     // number of reward slots
    }
    if (int32_t(ld32(F.addr(0x74))) > index) {
        st8(B + 0x34, 0);
        st32(B + 0x3C, 0);
        st32(B + 0x40, 0);
        st32(B + 0x38, 0);                       // third string: empty
        cx(AEABI_MEMSET, {B + 4, 0x18, 0});      // first and second string: empty
        const uint32_t slot = uint32_t(index) * 4;
        {
            const Addr sb = ld32(B + 0x44);
            cx(SAFEBUFFER_READ, {sb, F.addr(0xA8), ld32(sb + 0xC) + 0x18, 0x10, 0x18});   // item types
        }
        const uint32_t itemType = ld32(F.addr(0xA8) + slot);
        st32(B, itemType);
        if (itemType == 13) {                    // a keyblade
            {
                const Addr sb = ld32(B + 0x44);
                cx(SAFEBUFFER_READ, {sb, F.addr(0x98), ld32(sb + 0xC) + 0x28, 0x10, 0x28});   // item ids
            }
            const uint32_t itemId = ld32(F.addr(0x98) + slot);
            cx(SceneUtil_setItemImageData, {B + 0x10, B + 4, 13, itemId, 0});
            st32(B + 0x1C, uint32_t(rewardId));
            st8(B + 0x20, 0);
            st8(B + 0x21, 1);
            st32(B + 0x24, uint32_t(index));
            {
                const Addr sb = ld32(B + 0x44);
                cx(SAFEBUFFER_READ, {sb, F.addr(0x88), ld32(sb + 0xC) + 8, 0x10, 8});         // counts
            }
            const uint32_t countValue = ld32(F.addr(0x88) + slot);
            st32(B + 0x28, 1);
            st32(B + 0x2C, countValue);
            {
                const Addr sb = ld32(B + 0x44);
                cx(SAFEBUFFER_READ, {sb, F.addr(0x78), ld32(sb + 0xC) + 0x68, 0x10, 0x68});   // amounts
            }
            st32(B + 0x30, ld32(F.addr(0x78) + slot));

            // list.push_back(entry)
            const Addr endSlot = me + 0x2D0;
            const uint32_t end = ld32(endSlot);
            if (end == ld32(me + 0x2D4)) {
                cx(ResutData_pushBackSlow, {me + 0x2CC, B});
            } else {
                uint32_t base = 0;
                if (end != 0) {
                    cx(ResutData_copyCtor, {end, B});
                    base = ld32(endSlot);
                }
                st32(endSlot, base + RD_SIZE);
            }
        }
        // the three strings, last first
        if (ld8(B + 0x38) & 1) cx(OPERATOR_DELETE, {ld32(B + 0x40)});
        if (ld8(B + 0x10) & 1) cx(OPERATOR_DELETE, {ld32(B + 0x18)});
        if (ld8(B + 4) & 1) cx(OPERATOR_DELETE, {ld32(B + 0xC)});
    }
    const Addr ctrl = ld32(B + 0x48);            // the Reward row's control block
    if (ctrl) cx(SHARED_RELEASE, {ctrl});
}

RW_FUNCTION(0x6053F0, "SceneBattleResult::create()", SceneBattleResult::create);
RW_FUNCTION(0x681464, "SceneBattleResult::~SceneBattleResult()", SceneBattleResult::destruct);
RW_FUNCTION(0x6814F8, "SceneBattleResult::init()", SceneBattleResult::init);
RW_FUNCTION(0x6816EC, "SceneBattleResult::initScene()", SceneBattleResult::initScene);
RW_FUNCTION(0x6818B0, "SceneBattleResult::nextResult()", SceneBattleResult::nextResult);
RW_FUNCTION(0x682AB0, "SceneBattleResult::openNewLuxPopup(int)", SceneBattleResult::openNewLuxPopup);
RW_FUNCTION(0x683E64, "SceneBattleResult::addResultRewardKeyBlade(int, int)", SceneBattleResult::addResultRewardKeyBlade);
