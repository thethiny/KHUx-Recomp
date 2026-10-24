// SceneBattleResult: lambda bodies that run in game but were left after batch 2 (batch 3, named from their __func
// vtables' target_type). `functor` is the __func object: vtable at +0, then the captures.
#include "SceneUi.hpp"

using namespace scene;

namespace {

constexpr uint32_t SBR_nextResult = 0x6818B1;
constexpr uint32_t SBR_resultLoaded = 0x685BC1;                   // sub_685BC0(captures): the results, loaded
constexpr uint32_t SAS_callAdventureSelectAPI = 0x71D261;         // (std::function<void(bool)>)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;         // (api, const std::function<void(bool)>&, bool)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;                // (const char* path, void*)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;            // (ResourceID, fade ms)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t V_getAnimation = 0x298;                        // cocostudio::Armature::getAnimation
constexpr uint32_t V_animPlay = 0x68;                             // ArmatureAnimation::play(name, durationTo, loop)

constexpr uint32_t API_REQUEST_FAILED = 0x14;
constexpr uint32_t API_RESULT_STATE = 0x14C;                      // APIManager: the stage result (+0xF0: its kind)
constexpr uint32_t API_RAID_STATE = 0x180;                        // APIManager: pointer to the raid state (int)
constexpr uint32_t SM_ADVENTURE = 0x28C;
constexpr uint32_t SM_RESULT_FLAG = 0x2F5;
constexpr uint32_t APP_FLAGS = 0x20;                              // AppState: SafeBuffer, +8 a BGM switch
constexpr uint32_t BGM_RESULT = 0x1DDCAACE, BGM_RESULT_ALT = 0x1DDCB6F1;
constexpr uint32_t S_AREA_POINT = 0xDF9FB4, S_LUX_LEVEL_UP_LWF = 0xE047DA, S_ANIMATION1 = 0xDFD0AF;

constexpr uint32_t SBR_startResult = 0x683265;
constexpr uint32_t APIManager_executeSystemPushHelp = 0x4D2395;   // (api, const std::function<void(bool)>&)
constexpr uint32_t RaidEnemy_create = 0x787745;                   // master::RaidEnemy::create(shared_ptr out, id)
constexpr uint32_t API_RAID = 0x178;                              // APIManager: the raid (SafeBuffer, 0xA8 bytes)
constexpr uint32_t API_USER_ID = 0x74;                            // APIManager: SafeBuffer, the user id (8 bytes) at 0
constexpr uint32_t VT_pushHelpDone = 0xF64894;                    // [this]
constexpr uint32_t TutorialMisc_create = 0x777BB5;                // (shared_ptr out, id)
constexpr uint32_t APIManager_executeUserSphereCheck = 0x4CCBC1;  // (api, SphereLockType, const std::function&)
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;         // (std::string&, std::string&, type, id, bool)
constexpr uint32_t String_assign = 0x48D811;                      // std::string::assign(const char*, size_t)
constexpr uint32_t ResutData_copy = 0x5153E7;                     // BattleResultScrollView::ResutData(const&)
constexpr uint32_t ResutVector_pushSlow = 0x51517D;               // vector<ResutData>::__push_back_slow_path
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t aeabi_memset = 0x40E7A0;                      // (dst, n, c)
constexpr uint32_t API_USER_RANK = 0x94;                          // APIManager: SafeBuffer, the Lux rank at +0x40
constexpr uint32_t SBR_ITEMS = 0x2CC;                             // std::vector<ResutData>
constexpr uint32_t RESUT_SIZE = 0x44, ITEM_KEYBLADE = 13;
constexpr int32_t TEXT_ITEM_GOT = 0x623CED1, TEXT_ITEM_GOT_FIRST = 0x623CED2;
constexpr uint32_t VT_sphereChecked = 0xF648EC;                   // [this]
constexpr uint32_t VT_levelUpClosed = 0xF64658;   // openLevelUpPopup $_10::{lambda()#1} [layer, panel, popup, this]
constexpr uint32_t VT_selectApiDone = 0xF64708;   // nextResult $_7::{lambda(bool)#1}
constexpr uint32_t VT_retry = 0xF64918;           // init $_3::{lambda(bool)#1}::{lambda(bool)#1}

}  // namespace

struct SceneBattleResultBatch3 {
    // openLevelUpPopup $_11 [popup, armature]: the Lux level-up LWF on Area_Point and the armature's animation
    static void levelUpShown(uint32_t functor) {
        const uint32_t point = findNode(ld32(functor + 4), S_AREA_POINT, 10);
        const uint32_t lwf = call(AudioLWFNode_create, {S_LUX_LEVEL_UP_LWF, 0});
        vcall(point, node::addChild, {point, lwf});
        const uint32_t armature = ld32(functor + 8);
        const uint32_t animation = vcall(armature, V_getAnimation, {armature});
        const uint32_t play = ld32(ld32(animation) + V_animPlay);
        Frame f;
        const uint32_t name = newStr(f, S_ANIMATION1, 10);
        call(play, {animation, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
        const uint32_t again = ld32(functor + 8);
        vcall(again, node::setVisible, {again, 1});
    }

    // openLevelUpPopup $_10 [popup, layer, panel, this]: the OK button - shrink the popup, then (closed) remove
    // it and go on with the next result
    static void levelUpOk(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        if (ld32(typePtr) != 2) return;
        const uint32_t self = ld32(functor + 0x10);
        call(AudioPlaySystemSEDecide, {});
        const uint32_t popup = ld32(functor + 4), layer = ld32(functor + 8), panel = ld32(functor + 0xC);
        Frame f;
        const uint32_t closed = f.alloc(24);
        st32(closed + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x14});
        if (obj) {
            st32(obj, VT_levelUpClosed);
            st32(obj + 4, layer);
            st32(obj + 8, panel);
            st32(obj + 0xC, popup);
            st32(obj + 0x10, self);
        }
        st32(closed + 16, obj);
        const uint32_t shrink = call(SceneUtil_createClosePopupAnimation, {popup, closed});
        call(Node_runAction, {popup, call(EaseSineOut_create, {shrink})});
        functionDestroy(closed);
    }
    static void levelUpClosed(uint32_t functor) {
        const uint32_t parent = ld32(functor + 4);
        vcall(parent, node::removeChild, {parent, ld32(functor + 8), 1});
        vcall(ld32(functor + 4), node::removeChild, {ld32(functor + 4), ld32(functor + 0xC), 1});
        call(SBR_nextResult, {ld32(functor + 0x10)});
    }

    // nextResult $_7 (bool): back to the quest select's APIs
    static void nextResultDone(uint32_t /*functor*/, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        Frame f;
        const uint32_t done = newFunction(f, VT_selectApiDone, {});
        call(SAS_callAdventureSelectAPI, {done});
        functionDestroy(done);
    }
    // nextResult $_6: unless the result was a raid's, flag the result for the scene manager
    static void nextResultFlag(uint32_t /*functor*/) {
        if (ld32(ld32(call(fn::APIManager_getInstance, {}) + API_RESULT_STATE) + 0xF0) == 3) return;
        st8(call(fn::SceneManager_getInstance, {}) + SM_RESULT_FLAG, 1);
    }

    // init $_0 [this]: start updating and the result BGM
    static void resultEnter(uint32_t functor) {
        call(Node_scheduleUpdate, {ld32(functor + 4)});
        const int32_t adventure = int32_t(ld32(call(fn::SceneManager_getInstance, {}) + SM_ADVENTURE));
        uint32_t bgm = BGM_RESULT;
        if (adventure == 6 || adventure == 8) {
            if (ld32(ld32(call(fn::APIManager_getInstance, {}) + API_RAID_STATE)) == 3) return;
        } else if (safe32(ld32(call(AppState_getInstance, {}) + APP_FLAGS), 8)) {
            bgm = BGM_RESULT_ALT;
        }
        call(AudioReplaceGameMainBGM, {bgm, 500});
    }

    // sub_685BC0(captures [this]): the result screen starts - after the own raid of the player (boss up, help
    // not turned off) the help request screen first
    static void resultReady(uint32_t captures) {
        const uint32_t self = ld32(captures);
        auto start = [&]() {
            call(SceneManager_enableLoadingIndicator, {call(fn::SceneManager_getInstance, {}), 0});
            call(SBR_startResult, {self});
        };
        if (int32_t(ld32(call(fn::SceneManager_getInstance, {}) + SM_ADVENTURE)) != 6) return start();
        const uint32_t raidState = call(fn::APIManager_getInstance, {}) + API_RAID_STATE;
        if (ld32(ld32(raidState)) != 1) return start();
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t raid = ld32(api + API_RAID);
        const uint32_t ownerLo = ld32(raid + 0x60), ownerHi = ld32(raid + 0x64);
        const SafeBuffer *userId = safe(ld32(call(fn::APIManager_getInstance, {}) + API_USER_ID));
        const uint32_t idLo = uint32_t(userId->get32(0)), idHi = uint32_t(userId->get32(4));
        if (((idHi ^ ownerHi) | (idLo ^ ownerLo)) != 0) return start();
        if (!ld8(ld32(raidState) + 4)) return start();
        if (safe32(ld32(call(AppState_getInstance, {}) + APP_FLAGS), 8) != 0) return start();
        const int32_t enemyId = safe32(ld32(call(fn::APIManager_getInstance, {}) + API_RAID), 0x2C);
        Frame f;
        const uint32_t enemy = f.alloc(8);
        call(RaidEnemy_create, {enemy, uint32_t(enemyId)});
        const int32_t value = safe32(ld32(enemy), 0x8C);
        if (value != 1) {
            call(SceneManager_enableLoadingIndicator, {call(fn::SceneManager_getInstance, {}), 1});
            const uint32_t api2 = call(fn::APIManager_getInstance, {});
            const uint32_t done = newFunction(f, VT_pushHelpDone, {self});
            call(APIManager_executeSystemPushHelp, {api2, done});
            functionDestroy(done);
            if (ld32(enemy + 4)) call(fn::shared_release, {ld32(enemy + 4)});
            return;
        }
        if (ld32(enemy + 4)) call(fn::shared_release, {ld32(enemy + 4)});
        start();
    }

    // sub_681784(captures [this]): from the Lux rank of TutorialMisc 8 on, the sphere lock check first
    static void sphereCheck(uint32_t captures) {
        Frame f;
        const uint32_t misc = f.alloc(8);
        call(TutorialMisc_create, {misc, 8});
        if (!ld32(misc)) {
            call(SBR_resultLoaded, {captures});
        } else {
            const int32_t rank = safe32(ld32(call(fn::APIManager_getInstance, {}) + API_USER_RANK), 0x40);
            const int32_t from = safe32(ld32(misc), 4);
            if (rank < from) {
                call(SBR_resultLoaded, {captures});
            } else {
                call(SceneManager_enableLoadingIndicator, {call(fn::SceneManager_getInstance, {}), 1});
                const uint32_t api = call(fn::APIManager_getInstance, {});
                const uint32_t done = newFunction(f, VT_sphereChecked, {ld32(captures)});
                call(APIManager_executeUserSphereCheck, {api, 1, done});
                functionDestroy(done);
            }
        }
        sharedRelease(misc);
    }

    // sub_684098(&this, type, id, count, first): a keyblade among the got items (other kinds are not listed)
    static void addResultItem(uint32_t selfPtr, uint32_t type, uint32_t id, uint32_t count, uint32_t first) {
        const uint32_t self = ld32(selfPtr);
        Frame f;
        const uint32_t item = f.alloc(RESUT_SIZE);
        st8(item + 0x34, 0);
        st32(item + 0x38, 0);
        st32(item + 0x3C, 0);
        st32(item + 0x40, 0);
        call(aeabi_memset, {item + 4, 0x18, 0});
        st32(item, type);
        if (type == ITEM_KEYBLADE) {
            call(SceneUtil_setItemImageData, {item + 0x10, item + 4, ITEM_KEYBLADE, id, 0});
            st32(item + 0x1C, id);
            st8(item + 0x20, 0);
            st8(item + 0x21, 0);
            st32(item + 0x24, 0);
            st32(item + 0x28, 1);
            st32(item + 0x2C, 1);
            st32(item + 0x30, count);
            st8(item + 0x34, 1);
            const uint32_t td = f.alloc(8);
            call(fn::TextData_get, {td, 0, uint32_t(first ? TEXT_ITEM_GOT_FIRST : TEXT_ITEM_GOT)});
            const uint32_t text = ld32(ld32(td) + 4);
            call(String_assign, {item + 0x38, text, call(STRLEN, {text})});
            sharedRelease(td);
            const uint32_t end = ld32(self + SBR_ITEMS + 4);
            if (end != ld32(self + SBR_ITEMS + 8)) {
                if (end) call(ResutData_copy, {end, item});
                st32(self + SBR_ITEMS + 4, ld32(self + SBR_ITEMS + 4) + RESUT_SIZE);
            } else {
                call(ResutVector_pushSlow, {self + SBR_ITEMS, item});
            }
        }
        strFree(item + 0x38);
        strFree(item + 0x10);
        strFree(item + 4);
    }

    // init $_3::{lambda(bool)#1}: the results' data - go on, or the retry dialog
    static void resultLoaded(uint32_t functor, uint32_t okPtr) {
        const uint8_t ok = ld8(okPtr);
        call(SceneManager_enableLoadingIndicator, {call(fn::SceneManager_getInstance, {}), 0});
        if (ok) {
            call(SBR_resultLoaded, {functor + 4});
            return;
        }
        if (!ld8(call(fn::APIManager_getInstance, {}) + API_REQUEST_FAILED)) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t retry = newFunction(f, VT_retry, {});
        call(APIManager_openErrorDialog, {api, retry, 1});
        functionDestroy(retry);
    }

private:
    static constexpr uint32_t AppState_getInstance = 0x4E66DD;
};

using L = SceneBattleResultBatch3;
RW_FUNCTION(0x684CA4, "SceneBattleResult::openLevelUpPopup::$_11::operator()", L::levelUpShown);
RW_FUNCTION(0x684E98, "SceneBattleResult::openLevelUpPopup::$_10::operator()", L::levelUpOk);
RW_FUNCTION(0x685022, "SceneBattleResult::openLevelUpPopup::$_10::{lambda()#1}::operator()", L::levelUpClosed);
RW_FUNCTION(0x6852F4, "SceneBattleResult::nextResult::$_7::operator()", L::nextResultDone);
RW_FUNCTION(0x685418, "SceneBattleResult::nextResult::$_6::operator()", L::nextResultFlag);
RW_FUNCTION(0x685754, "SceneBattleResult::init::$_0::operator()", L::resultEnter);
RW_FUNCTION(0x685FEC, "SceneBattleResult::init::$_3::{lambda(bool)#1}::operator()", L::resultLoaded);
RW_FUNCTION(0x685BC0, "SceneBattleResult (sub_685BC0): the result screen starts", L::resultReady);
RW_FUNCTION(0x681784, "SceneBattleResult (sub_681784): the sphere lock check", L::sphereCheck);
RW_FUNCTION(0x684098, "SceneBattleResult (sub_684098): add a got item", L::addResultItem);
