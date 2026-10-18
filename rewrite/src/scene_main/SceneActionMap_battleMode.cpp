// SceneActionMap::updateBattleMode: the quest battle's state machine (one step per frame). The state is
// SceneActionMap +0x338:
//    0 battle start (counters, BGM, start skills)        1 waiting for the start heading
//    2 / 3 a boss / raid enemy's entry direction           4 the friend's covering fire
//    5 special attack: target selection                    6 / 7 enemy poison tick, then status update
//    8 deck advances after the attack                      9 the enemy team is wiped out / turn end
//   10 / 11 player poison tick                             12 the player's turn: attack or skip
//   13 approaching the enemies                             14 / 15 / 20 the medal attack animation (chains)
//   16 waiting for the enemies to calm down                17 / 18 / 19 / 21 skip / jump approach / jump back
//   22 / 23 target selection (special attack)              24 / 25 the enemies attack
//   26 / 27 ally attacks                                   28 lost: continue dialog / colosseum loss
//   29 the player is down (revive once)                    30 battle won
// After every step the actors update, the HP gauge follows and the camera moves.
#include "SceneActionMap.hpp"

#include <cmath>

using namespace smain;
using rw::guest_addr;

namespace {
// ---- SceneActionMap
constexpr uint32_t S_MENU_ALLOWED = 0x2D2, S_TOUCH_STATE = 0x2D4, S_TOUCH_TIMER = 0x2D8, S_TIMER_ON = 0x2DC;
constexpr uint32_t S_TAP_ATTACK = 0x2DD, S_TAP_WHOLE = 0x2DE, S_TAP_LONG = 0x2DF;   // touch requests (bytes)
constexpr uint32_t S_UI = 0x2B8, S_ACTORS = 0x2BC, S_FIELD = 0x2C0, S_BGM = 0x2E0;
constexpr uint32_t S_STAGE = 0x320, S_BATTLE = 0x324, S_DIRECTION = 0x32C, S_COMMON = 0x330;
constexpr uint32_t S_STATE = 0x338, S_PENDING = 0x340, S_CAMERA_TARGET = 0x368, S_SHADER = 0x394;
// ---- StageActorManager
constexpr uint32_t A_AVATAR = 0x20, A_FRIEND = 0x24, A_ALLIES = 0x40, A_ENEMIES = 0x4C;
constexpr uint32_t A_RAID = 0x7C;           // byte: a raid battle (the raid BGM)
constexpr uint32_t A_ENCOUNTER = 0x80;      // how the battle started (2 / 4 enemy first, 5 boss entry)
constexpr uint32_t A_TARGET = 0x140;        // AttackTarget: kind (1 chain, 2 whole, 3 special), +4 medal info, +0x10 enemy
constexpr uint32_t TARGET_MEDAL = 0x144, TARGET_ENEMY = 0x150;
constexpr uint32_t ATTACK_CHAIN = 1, ATTACK_WHOLE = 2, ATTACK_SPECIAL = 3;
// ---- AvatarActor / EnemyActor / others
constexpr uint32_t AV_MAP_POSITION = 0x270, AV_AVATAR = 0x288, AV_DOWN_PENDING = 0x295;
constexpr uint32_t AVATAR_MOTION = 0x524, MOTION_IDLE = 11;
constexpr uint32_t E_DATA = 0x2D8, E_FLASH = 0x330, E_HP_INDICATOR = 0x3C0, E_ENTERED = 0x3CC;
constexpr uint32_t FLASH_ENTRY_STATE = 0x520;
constexpr uint32_t ENEMY_ATTACK_KIND = 0x334;   // enemy data: 3 = attacks from where it stands
constexpr uint32_t DECK_CHAIN_A = 0x360, DECK_CHAIN_B = 0x364, DECK_CHAIN_C = 0x370, DECK_STATE = 0x368;
constexpr uint32_t CAROUSEL_ROLLING = 0x3FA, CAROUSEL_ATTACKING = 0x3FC;
constexpr uint32_t FIELD_BATTLE_FLAG = 0x88;
constexpr uint32_t STAGE_COLOSSEUM = 0x14;
constexpr uint32_t LUX_PRIZES = 0x278, LUX_PRIZE_X = 0x27C, LUX_PRIZE_Y = 0x280;
constexpr uint32_t UI_LUX_BOARD = 0x30;
constexpr uint32_t MEDAL_BURST_ID = 0x4AC, BURST_REACH = 0xC;
// ---- StageState (AppState +0x20, a SafeBuffer)
constexpr uint32_t ST_HP = 8, ST_REVIVES = 0x4244, ST_TURN_FLAG = 0x424C, ST_TURN = 0x4250, ST_TURNS_TOTAL = 0x4254;
constexpr uint32_t ST_SPECIALS = 0x4258, ST_BATTLES = 0x4260, ST_CONTINUES = 0x45BC, ST_45C0 = 0x45C0;

// ---- lambdas (std::function vtables)
constexpr uint32_t VT_START_TUTORIAL_DONE = 0xF5E754, VT_WIPE_OUT_DELAY = 0xF5E6FC, VT_TURN_TUTORIAL_DONE = 0xF5E6A4;
constexpr uint32_t VT_ENEMY_ATTACK = 0xF5E678, VT_CONTINUE = 0xF5E64C, VT_COLOSSEUM_LOST = 0xF5E620;
constexpr uint32_t VT_WIN_TUTORIAL_DONE = 0xF5E728;

namespace f {
constexpr uint32_t AppState_getInstance = 0x4E66DD, SafeBuffer_read = 0x45A269, SafeBuffer_write = 0x459FA1;
constexpr uint32_t StageUI_getMedalField = 0x75E65D, StageUI_getCarouselDeck = 0x75E661;
constexpr uint32_t StageUI_getPlayerStatusField = 0x75E671, StageUI_isPlayingHeading = 0x75F815;
constexpr uint32_t StageUI_getLastShowHeading = 0x75F7F9, StageUI_setBattleTurn = 0x761727;
constexpr uint32_t StageUI_garbageCollectHeading = 0x75F709, StageUI_showHeading = 0x75F54D;
constexpr uint32_t StageUI_openContinueDialog = 0x75FDB9;
constexpr uint32_t PlayerStatusIndicator_setHP = 0x5E3119, PlayerStatusIndicator_stopAlert = 0x5E2839;
constexpr uint32_t MapNode_updateMapPosition = 0x5DA461, MapNode_move = 0x5DA415;
constexpr uint32_t AvatarActor_takeActionFlag = 0x740689, AvatarActor_ClearChainAttack = 0x7409F9;
constexpr uint32_t AvatarActor_setChainAttackMotion = 0x740A9D, AvatarActor_choiceChainType = 0x740945;
constexpr uint32_t AvatarActor_UpdateStatus = 0x7403A9, AvatarActor_ClearBuffDebuff = 0x7401B9;
constexpr uint32_t AvatarActor_updateActionDied = 0x740889, AvatarActor_setActionResurrection = 0x740879;
constexpr uint32_t AvatarActor_isActionDied = 0x740859, AvatarActor_setActionDied = 0x7407BD;
constexpr uint32_t Avatar_activeBurst = 0x5C62D5, Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t EnemyActor_isDied = 0x6008ED, EnemyActor_UpdateStatus = 0x74B441;
constexpr uint32_t EnemyActor_select_enemy_attack = 0x74B1D1;
constexpr uint32_t IStatusProtocol_elapseTurnStatusPoison = 0x4F1EA5;
constexpr uint32_t RaidBossHPIndicator_isPlayEndCoeDirection = 0x5E78C3;
constexpr uint32_t BaseActorManager_isPlayerAttackSkipStatus = 0x74E8AD, BaseActorManager_update_burst_enable = 0x74E871;
constexpr uint32_t SAM_reflectionCondition = 0x751B29, SAM_storeDamage = 0x7514A1, SAM_attackReadyAction = 0x74FCED;
constexpr uint32_t SAM_getCurrentEncountSymbolEnemyActor = 0x74F5CD, SAM_execFriendCoveringFireAction = 0x75201D;
constexpr uint32_t SAM_isFriendCoveringFireCameraTargetPlayer = 0x752515, SAM_getLastEncountTeamUniqueID = 0x74F595;
constexpr uint32_t SAM_getLastEncountTeamInfo = 0x74F5B1, SAM_hasAliveEnemyInTeam = 0x74F871;
constexpr uint32_t SAM_executeAttackCurrentTarget = 0x74F8ED, SAM_initFriendCoveringFireAction = 0x75200D;
constexpr uint32_t SAM_startAllyAttack = 0x7527F9, SAM_isEndAllyAttack = 0x752905, SAM_doJumpFallAction = 0x75239D;
constexpr uint32_t SAM_doDisengageAttackEnd = 0x7529B9, SAM_enebleChoiceMarker = 0x74FED9, SAM_doJumpUpAction = 0x7522A5;
constexpr uint32_t SAM_checkEncountOrder = 0x74F475, SAM_turnEndAction = 0x74FE51, SAM_clearCurrentTarget = 0x74F5F5;
constexpr uint32_t SAM_entryWholeAttack = 0x74F685, SAM_battleFinishAction = 0x74FF01, SAM_hasActiveAlly = 0x74F8C5;
constexpr uint32_t SAM_autoChoiceTarget = 0x74FB81, SAM_isJumpAttackAction = 0x75266D;
constexpr uint32_t SAM_doJumpApprpachAction = 0x752699, SAM_update = 0x74FF25, SAM_computeAttackPosition = 0x750F75;
constexpr uint32_t SBM_MinHPRateEnemyTeam = 0x753A79, SBM_startSelectedTargetEnemy = 0x754181;
constexpr uint32_t SBM_isSelectedTargetEnemy = 0x7542A1, SBM_isEnemyTeamCalmDown = 0x753835;
constexpr uint32_t SBM_isEnemyTeamAnnihilation = 0x75380D, SBM_approachEnemyTeam = 0x753BF1;
constexpr uint32_t SBM_hasEnemyAttker = 0x753B69, SBM_getEnemyAttcker = 0x753B79, SBM_initApproachEnemyTeam = 0x753BC5;
constexpr uint32_t SBM_IsAllOverEnemyAttker = 0x753A21, SBM_setEnemyAttacker = 0x753891;
constexpr uint32_t SBM_firstApproachIsMoveToFront = 0x753ED9, SBM_checkSubMission12 = 0x754351;
constexpr uint32_t MedalDeck_advanceState = 0x5DFEC1, MedalDeck_getCurrentMedalInfo = 0x5DFF6B;
constexpr uint32_t MedalDeck_isCurrentBurstSingleSelect = 0x5E046D, MedalDeck_isCurrentFriendMedal = 0x5E0441;
constexpr uint32_t MedalDeck_isNextEnemyTurn = 0x5E03F9, MedalDeck_turnEnd = 0x5DFF53, MedalDeck_isCurrentFirstMedal = 0x5E041D;
constexpr uint32_t CarouselDeck_advanceDeck = 0x5D0C91, CarouselDeck_actionAttack = 0x5D0E95;
constexpr uint32_t CarouselDeck_setPossibleBurst = 0x5D0C11;
constexpr uint32_t SceneBattleBase_invokeAttackSkill = 0x5FA859, SceneBattleBase_invokeSkill = 0x5FA975;
constexpr uint32_t BattleMisc_getInstance = 0x7420A9, BattleMisc_preparePlayerAttackEnemy = 0x742765;
constexpr uint32_t CommonDirection_heal = 0x766DF9, CommonDirection_req_select_target_disp = 0x76543B;
constexpr uint32_t CommonDirection_req_select_target_hide = 0x76547D, CommonDirection_battle_start = 0x766F19;
constexpr uint32_t CommonDirection_req_skilPanel_all = 0x765AFD;
constexpr uint32_t GuiltBurstCutInShader_setShaderEnd = 0x496271, GuiltBurstCutInShader_setShaderConcentrationStart = 0x496215;
constexpr uint32_t AudioPushGameMainBGM = 0x5C0FE5;
constexpr uint32_t StageState_addExterminationTeamID = 0x4EF945, StageState_setPlayableActorPoisonEffect = 0x4F05C9;
constexpr uint32_t SceneActionMap_setEnemyPoisonEffect = 0x600A59, SceneActionMap_onBurstButton = 0x60097D;
constexpr uint32_t SceneActionMap_revivePlayer = 0x600DA5, SceneActionMap_toFieldModeSetup = 0x5FFE21;
constexpr uint32_t SceneActionMap_battleStartTutorial = 0x6003B1, SceneActionMap_stageFinishTutorial = 0x6007D5;
constexpr uint32_t SceneActionMap_updateMimicCameraPosition = 0x6002DD, SceneActionMap_updateCameraPosition = 0x5FFD39;
constexpr uint32_t StageFieldManager_isCollisionSafe = 0x756981, StageFieldManager_getScreenPosToMapPos = 0x756559;
constexpr uint32_t StageDirection_cocos_lose_colosseum = 0x769599;
constexpr uint32_t LuxBoard_setNumber = 0x5D38C1, LuxBoard_getUIPos = 0x5D3B69;
constexpr uint32_t Burst_create = 0x77A491, Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t function_b_call = 0x51022D;
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t Node_runAction = 0x894AED, MoveTo_create = 0x86941D, EaseSineOut_create = 0x860D09;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_ctor_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17, Vec2_subtract = 0x8E530F;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t ATAN2F = 0x41C768, COSF = 0x41C8AC, SINF = 0x41A9E0;   // imports
}  // namespace f

constexpr uint32_t Node_setScaleX = 0x28;
constexpr uint32_t SCENE_ON_CONTINUE = 0x308;   // SceneActionMap vtable byte offset (the revive SE / effect, 0x45C)
constexpr uint32_t MOVE_SECONDS = 0x3E2AAAAB;   // 1/6 s (float bits)
constexpr uint32_t WIPE_OUT_DELAY = 0x3F99999A; // 1.2 s

double asDouble(uint64_t bits) { double d; std::memcpy(&d, &bits, 8); return d; }
const double QUARTER_PI = asDouble(0x3FE921FB54442D18ull), HALF_PI = asDouble(0x3FF921FB54442D18ull);

struct BattleMode {
    uint32_t self, avatar, deck, dt, mapMoved;

    uint32_t ui() const { return ld32(self + S_UI); }
    uint32_t actors() const { return ld32(self + S_ACTORS); }
    uint32_t battle() const { return ld32(self + S_BATTLE); }
    uint32_t common() const { return ld32(self + S_COMMON); }
    uint32_t target() const { return actors() + A_TARGET; }
    uint32_t enemies() const { return actors() + A_ENEMIES; }
    uint32_t avatarModel() const { return ld32(avatar + AV_AVATAR); }
    void setState(uint32_t s) const { st32(self + S_STATE, s); }
    uint32_t state() const { return ld32(self + S_STATE); }
    uint32_t carousel() const { return call(f::StageUI_getCarouselDeck, {ui()}); }

    static uint32_t stageState() { return ld32(call(f::AppState_getInstance, {}) + 0x20); }
    static uint32_t readState(uint32_t offset) {
        const uint32_t ss = stageState();
        Frame fr;
        const uint32_t v = fr.alloc(4);
        call(f::SafeBuffer_read, {ss, v, ld32(ss + 0xC) + offset, 4, offset});
        return ld32(v);
    }
    static void writeState(uint32_t ss, uint32_t offset, uint32_t value) {
        Frame fr;
        const uint32_t v = fr.alloc(4);
        st32(v, value);
        call(f::SafeBuffer_write, {ss, ld32(ss + 0xC) + offset, v, 4, offset});
    }
    void showHp() const {
        const uint32_t indicator = call(f::StageUI_getPlayerStatusField, {ui()});
        call(f::PlayerStatusIndicator_setHP, {indicator, readState(ST_HP)});
    }
    void clearTouchRequests() const {
        st8(self + S_TAP_ATTACK, 0);
        st8(self + S_TAP_WHOLE, 0);
        stf(self + S_TOUCH_TIMER, 0.5f);
    }
    bool calmDown() const { return call(f::SBM_isEnemyTeamCalmDown, {battle(), enemies()}) == 1; }
    bool headingPlaying() const { return call(f::StageUI_isPlayingHeading, {ui()}) != 0; }
    uint32_t medalBurstId(uint32_t actors) const {
        const uint32_t medal = ld32(ld32(actors + TARGET_MEDAL));
        Frame fr;
        const uint32_t v = fr.alloc(4);
        call(f::SafeBuffer_read, {medal, v, ld32(medal + 0xC) + MEDAL_BURST_ID, 4, MEDAL_BURST_ID});
        return ld32(v);
    }
    // the chain attack's motion for the target enemy (the deck's chain position)
    void chainMotion(uint32_t actors) const {
        const uint32_t chain = ld32(deck + DECK_CHAIN_B) + ld32(deck + DECK_CHAIN_A) - ld32(deck + DECK_CHAIN_C);
        call(f::AvatarActor_choiceChainType, {avatar, ld32(actors + TARGET_ENEMY), chain});
        call(f::AvatarActor_setChainAttackMotion, {avatar});
    }
    // a std::function holding {lambda vtable, captures} on the stack, passed to `use`, destroyed after
    template <class F> void withFunction(uint32_t vtable, std::initializer_list<uint32_t> captures, F use) const {
        Frame fr;
        const uint32_t fo = newFunction(fr, vtable, captures);
        use(fo);
        functionDestroy(fo);
    }
    // the player's attack lands: attack skills, the attack itself (it may heal), the damage
    void playerAttack(uint32_t attackTarget) const {
        call(f::SceneBattleBase_invokeAttackSkill, {self, attackTarget});
        const uint32_t misc = call(f::BattleMisc_getInstance, {});
        if (call(f::BattleMisc_preparePlayerAttackEnemy, {misc, attackTarget, actors()}) == 1) {
            showHp();
            call(f::CommonDirection_heal, {common()});
        }
        call(f::SAM_storeDamage, {actors(), avatar, attackTarget});
    }

    // ------------------------------------------------------------------------------------------- states
    void start() const {   // 0
        writeState(stageState(), ST_BATTLES, readState(ST_BATTLES) + 1);
        writeState(stageState(), ST_TURN_FLAG, 0);
        writeState(stageState(), ST_TURN, 1);
        writeState(stageState(), ST_45C0, 0);
        const uint32_t bgm = ld8(actors() + A_RAID) ? 2 : 1;
        call(f::AudioPushGameMainBGM, {ld32(self + S_BGM + 4 * bgm), 500});
        call(f::SceneBattleBase_invokeSkill, {self, 4});
        const uint32_t am = actors();
        const uint32_t encounter = ld32(am + A_ENCOUNTER);
        if (encounter == 2 || encounter == 4) {   // the enemies come first
            setState(2);
        } else if (encounter == 5) {              // a boss entry: its attack is chosen now
            const uint32_t enemy = call(f::SAM_getCurrentEncountSymbolEnemyActor, {am});
            const uint32_t rate = call(f::SBM_MinHPRateEnemyTeam, {battle(), actors() + A_ENEMIES});
            call(f::EnemyActor_select_enemy_attack, {enemy, rate, actors()});
            setState(3);
        } else {
            call(f::StageUI_showHeading, {ui(), ld8(am + A_RAID) ? 1u : 0u});
            setState(1);
            const uint32_t bm = battle();
            const uint32_t enemy = call(f::SAM_getCurrentEncountSymbolEnemyActor, {actors()});
            call(f::SBM_firstApproachIsMoveToFront, {bm, avatar, enemy});
        }
    }
    void startHeading() const {   // 1
        if (headingPlaying()) return;
        if (call(f::StageUI_getLastShowHeading, {ui()}) > 1) {
            setState(33);
            call(f::CarouselDeck_setPossibleBurst, {carousel(), 1, 1});
            withFunction(VT_START_TUTORIAL_DONE, {self},
                         [&](uint32_t fo) { call(f::SceneActionMap_battleStartTutorial, {self, fo}); });
            return;
        }
        writeState(stageState(), ST_TURN_FLAG, 1);
        call(f::SceneBattleBase_invokeSkill, {self, 3});
        call(f::StageUI_setBattleTurn, {ui(), 1});
        call(f::StageUI_garbageCollectHeading, {ui()});
    }
    bool entryDirectionDone() const {
        const uint32_t enemy = call(f::SAM_getCurrentEncountSymbolEnemyActor, {actors()});
        return ld32(ld32(enemy + E_FLASH) + FLASH_ENTRY_STATE) <= 1;
    }
    void raidEntry() const {   // 2
        const uint32_t boss = call(f::SAM_getCurrentEncountSymbolEnemyActor, {actors()});
        if (call(f::RaidBossHPIndicator_isPlayEndCoeDirection, {ld32(boss + E_HP_INDICATOR)}) != 1) return;
        const uint8_t entered = ld8(call(f::SAM_getCurrentEncountSymbolEnemyActor, {actors()}) + E_ENTERED);
        const uint32_t enemy = call(f::SAM_getCurrentEncountSymbolEnemyActor, {actors()});
        if (!entered) {   // its entry: attack chosen, the battle start direction
            const uint32_t rate = call(f::SBM_MinHPRateEnemyTeam, {battle(), actors() + A_ENEMIES});
            call(f::EnemyActor_select_enemy_attack, {enemy, rate, actors()});
            call(f::CommonDirection_battle_start, {common()});
            st8(call(f::SAM_getCurrentEncountSymbolEnemyActor, {actors()}) + E_ENTERED, 1);
            return;
        }
        if (ld32(ld32(enemy + E_FLASH) + FLASH_ENTRY_STATE) > 1) return;
        call(f::StageUI_showHeading, {ui(), 0});
        setState(1);
    }
    void bossEntry() const {   // 3
        if (!entryDirectionDone()) return;
        call(f::StageUI_showHeading, {ui(), 0});
        setState(1);
    }
    void coveringFire() const {   // 4
        if (call(f::SAM_execFriendCoveringFireAction, {actors(), avatar, ld32(actors() + A_FRIEND)}) == 1) {
            setState(16);
            call(f::GuiltBurstCutInShader_setShaderEnd, {ld32(self + S_SHADER)});
        }
        const uint32_t cameraTarget =
            call(f::SAM_isFriendCoveringFireCameraTargetPlayer, {actors()}) ? avatar : ld32(actors() + A_FRIEND);
        st32(self + S_CAMERA_TARGET, cameraTarget);
        call(f::CarouselDeck_actionAttack, {carousel(), 1});
    }
    void specialTargets() const {   // 5
        call(f::CarouselDeck_actionAttack, {carousel(), 1});
        if (ld8(carousel() + CAROUSEL_ATTACKING)) return;
        const uint32_t ss = stageState();
        writeState(ss, ST_SPECIALS, readState(ST_SPECIALS) + 1);
        const uint32_t single = call(f::MedalDeck_isCurrentBurstSingleSelect, {deck});
        const uint32_t bm = battle();
        if (single != 1) {
            st32(bm + 0x28, 0);
            setState(23);
            return;
        }
        call(f::SBM_startSelectedTargetEnemy, {bm, actors() + A_ENEMIES});
        if (call(f::SBM_isSelectedTargetEnemy, {battle()}) == 1) {
            setState(23);
            return;
        }
        setState(22);
        call(f::CommonDirection_req_select_target_disp, {common()});
    }
    void enemyPoison() const {   // 6
        if (headingPlaying()) return;
        setState(call(f::SceneActionMap_setEnemyPoisonEffect, {self, actors() + A_ENEMIES}) == 1 ? 7 : 24);
    }
    void enemyStatus() const {   // 7
        if (!calmDown()) return;
        const uint32_t list = actors() + A_ENEMIES;
        for (uint32_t p = ld32(list), end = ld32(list + 4); p != end; p += 4) {
            call(f::IStatusProtocol_elapseTurnStatusPoison, {ld32(p) + 0x2C8});
            call(f::EnemyActor_UpdateStatus, {ld32(p)});
        }
        setState(call(f::SBM_isEnemyTeamAnnihilation, {battle(), list}) == 1 ? 9 : 24);
    }
    void deckAdvance() const {   // 8
        if (!calmDown()) return;
        call(f::MedalDeck_advanceState, {deck});
        call(f::CarouselDeck_advanceDeck, {carousel()});
        setState(9);
    }
    // the battle is won: the Lux board counts the turns and its prizes fly to the board, the result heading
    void battleFinish() const {
        const uint32_t am = actors();
        const uint32_t team = call(f::SAM_battleFinishAction, {am, readState(ST_TURN)});
        const uint32_t board = ld32(ui() + UI_LUX_BOARD);
        call(f::LuxBoard_setNumber, {board, ld32(team + 0x2C0)});
        const uint32_t field = ld32(self + S_FIELD);
        const uint32_t ui = call(f::LuxBoard_getUIPos, {board});
        Frame fr;
        const uint32_t to = fr.alloc(8);
        call(f::StageFieldManager_getScreenPosToMapPos, {to, field, ui});
        st32(board + LUX_PRIZES, team);
        st32(board + LUX_PRIZE_Y, ld32(to + 4));
        st32(board + LUX_PRIZE_X, ld32(to));
        call(f::Vec2_dtor, {to});
        setState(30);
        const uint32_t turn = readState(ST_TURN);
        uint32_t heading = 4;
        if (turn == 2) {
            heading = 3;
        } else if (turn == 1) {
            const uint32_t ss = stageState();
            writeState(ss, ST_CONTINUES, readState(ST_CONTINUES) - 1);
            heading = 2;
        }
        call(f::StageUI_showHeading, {this->ui(), heading});
        call(f::CommonDirection_req_skilPanel_all, {common()});
        call(f::SAM_turnEndAction, {actors(), readState(ST_TURN), 1, 0});
        const uint32_t ss = stageState();
        const uint32_t total = readState(ST_TURNS_TOTAL);
        writeState(ss, ST_TURNS_TOTAL, readState(ST_TURN) + total);
    }
    void wipedOutOrTurnEnd() const {   // 9
        if (!calmDown()) return;
        if (call(f::SBM_isEnemyTeamAnnihilation, {battle(), actors() + A_ENEMIES}) == 1) {
            const uint32_t ss = stageState();
            const uint32_t id = call(f::SAM_getLastEncountTeamUniqueID, {actors(), ld32(self + S_STAGE)});
            call(f::StageState_addExterminationTeamID, {ss, id});
            bool last = call(f::function_b_call, {self + S_PENDING}) != 1;
            if (!last) last = (call(f::SAM_getLastEncountTeamInfo, {actors(), ld32(self + S_STAGE)}) & 2) != 0;
            if (!last) {   // more teams on the map: the field comes back after a moment
                const uint32_t delay = call(f::DelayTime_create, {WIPE_OUT_DELAY});
                withFunction(VT_WIPE_OUT_DELAY, {self}, [&](uint32_t fo) {
                    const uint32_t then = call(f::CallFunc_create, {fo});
                    call(f::Node_runAction, {avatar, call(f::Sequence_create, {delay, then, 0})});
                });
            }
            battleFinish();
            return;
        }
        // the enemies survived: the turn ends
        call(f::SAM_checkEncountOrder, {actors(), ld32(self + S_STAGE)});
        if (ld32(deck + DECK_STATE) == 2) {   // the tutorial's turn end
            setState(33);
            call(f::SAM_doDisengageAttackEnd, {actors(), avatar, actors() + A_TARGET});
            const uint32_t am = actors();
            call(f::SAM_turnEndAction, {am, readState(ST_TURN), 0, 0});
            call(f::SAM_clearCurrentTarget, {actors()});
            call(f::SBM_setEnemyAttacker, {battle(), actors() + A_ENEMIES});
            withFunction(VT_TURN_TUTORIAL_DONE, {self}, [&](uint32_t fo) {
                Frame fr;
                const uint32_t result = fr.alloc(24);
                call(f::SceneUtil_openTutorialPopup, {result, 3, fo, 0, 0, 0, 0});
                functionDestroy(result);
            });
            return;
        }
        call(f::SAM_attackReadyAction, {actors(), 0});
        const uint32_t am = actors();
        call(f::SAM_turnEndAction, {am, readState(ST_TURN), 0, 1});
        if (call(f::MedalDeck_isCurrentFirstMedal, {deck}) != 1) {
            setState(12);
            return;
        }
        call(f::SBM_checkSubMission12, {battle(), actors() + A_ENEMIES});
        if (readState(ST_TURN) < 2) {
            setState(12);
        } else {
            setState(call(f::SAM_hasActiveAlly, {actors()}) ? 26 : 10);
            call(f::StageUI_setBattleTurn, {ui(), 1});
        }
        clearTouchRequests();
    }
    void playerPoison() const {   // 10
        if (headingPlaying()) return;
        setState(11);
        if (call(f::StageState_setPlayableActorPoisonEffect, {stageState(), avatar, 0}) != 1) return;
        const uint32_t hp = readState(ST_HP);
        call(f::PlayerStatusIndicator_setHP, {call(f::StageUI_getPlayerStatusField, {ui()}), hp});
        if (hp == 0) st8(avatar + AV_DOWN_PENDING, 1);
    }
    void playerStatus() const {   // 11
        if (ld32(avatarModel() + AVATAR_MOTION) != MOTION_IDLE) return;
        setState(12);
        call(f::IStatusProtocol_elapseTurnStatusPoison, {stageState() + 0x20});
        call(f::AvatarActor_UpdateStatus, {ld32(actors() + A_AVATAR)});
    }
    void playerTurn() const {   // 12
        if (call(f::BaseActorManager_isPlayerAttackSkipStatus, {actors(), 0}) == 1) {
            setState(17);
            return;
        }
        call(f::SAM_enebleChoiceMarker, {actors()});
        call(f::SBM_initApproachEnemyTeam, {battle(), actors() + A_ENEMIES});
        setState(13);
    }
    // the attack on the chosen target begins: a jump attack, or a run to the attack position
    void attackTarget(uint32_t am) const {
        const uint32_t at = am + A_TARGET;
        if (!ld32(at)) return;
        playerAttack(at);
        float reach = 100.0f;
        if (ld32(at) == ATTACK_SPECIAL) {
            Frame fr;
            const uint32_t burst = fr.alloc(8), range = fr.alloc(4);
            call(f::Burst_create, {burst, medalBurstId(am)});
            const uint32_t b = ld32(burst);
            call(f::SafeBuffer_read, {b, range, ld32(b + 0xC) + BURST_REACH, 4, BURST_REACH});
            reach = float(int32_t(ld32(range)));
            sharedRelease(burst);
        }
        if (call(f::SAM_isJumpAttackAction, {actors(), at, fbits(reach)}) == 1) {
            call(f::SAM_doJumpApprpachAction, {actors(), avatar});
            setState(18);
            return;
        }
        Frame fr;
        const uint32_t to = fr.alloc(8), move = fr.alloc(8);
        call(f::SAM_computeAttackPosition, {to, actors(), at, 0});
        const uint32_t here = vcall(avatar, node::getPosition, {avatar});
        call(f::Vec2_ctor_copy, {move, to});
        call(f::Vec2_subtract, {move, here});
        call(f::Vec2_dtor, {to});
        const float facing = ldf(move) < 0.0f ? 1.0f : -1.0f;
        vcall(avatar, Node_setScaleX, {avatar, fbits(facing)});
        stf(move, reach * facing + ldf(move));
        const uint32_t kind = ld32(at);
        if (kind == ATTACK_SPECIAL) {
            call(f::Avatar_activeBurst, {avatarModel(), medalBurstId(am)});
            call(f::Avatar_changeMotion, {avatarModel(), 8});
        } else if (kind == ATTACK_WHOLE) {
            call(f::Avatar_changeMotion, {avatarModel(), 2});
        } else if (kind == ATTACK_CHAIN) {
            chainMotion(am);
        }
        call(f::MapNode_move, {avatar, call(f::EaseSineOut_create, {call(f::MoveTo_create, {MOVE_SECONDS, move})})});
        setState(ld32(at) == ATTACK_SPECIAL ? 14 : 15);
        call(f::Vec2_dtor, {move});
    }
    void approach() const {   // 13
        call(f::SBM_approachEnemyTeam,
             {battle(), dt, avatar + AV_MAP_POSITION, actors() + A_ENEMIES, ld32(self + S_FIELD)});
        if (!call(f::SAM_hasAliveEnemyInTeam, {actors()})) {
            setState(8);
            return;
        }
        call(f::SAM_executeAttackCurrentTarget, {actors()});
        uint32_t am = actors();
        const uint32_t kind = ld32(am + A_TARGET);
        if (kind == ATTACK_SPECIAL) {
            clearTouchRequests();
            st8(self + S_TAP_LONG, 0);
            st8(self + S_TIMER_ON, 1);
            st32(self + S_TOUCH_STATE, 0);
        } else if (kind != 0) {
            clearTouchRequests();
            st8(self + S_TAP_LONG, 1);
        } else {   // no target yet: a tap picks one (the whole team or the nearest)
            if (ld8(self + S_TAP_WHOLE)) {
                st8(self + S_TAP_WHOLE, 0);
                const uint32_t medal = call(f::MedalDeck_getCurrentMedalInfo, {call(f::StageUI_getMedalField, {ui()})});
                call(f::SAM_entryWholeAttack, {am, medal});
            } else if (ld8(self + S_TAP_ATTACK)) {
                clearTouchRequests();
                const uint32_t medal = call(f::MedalDeck_getCurrentMedalInfo, {call(f::StageUI_getMedalField, {ui()})});
                call(f::SAM_autoChoiceTarget, {am, medal, 0});
            }
            call(f::SAM_executeAttackCurrentTarget, {actors()});
            am = actors();
        }
        attackTarget(am);
    }
    void specialAnimation() const {   // 14
        if (call(f::AvatarActor_takeActionFlag, {avatar, 4}) != 1) return;
        if (call(f::MedalDeck_isCurrentFriendMedal, {deck}) == 1) {
            call(f::SAM_initFriendCoveringFireAction, {actors(), 0});
            setState(4);
            return;
        }
        call(f::Avatar_changeMotion, {avatarModel(), 0x2F});
        setState(15);
    }
    // 15 / 20: the attack animation; a tap during a chain hit chains the next medal
    void attackAnimation() const {
        const uint32_t am = actors();
        const uint32_t at = am + A_TARGET;
        if (call(f::AvatarActor_takeActionFlag, {avatar, 6}) == 1 && ld32(at) == ATTACK_CHAIN &&
            ld32(am + TARGET_ENEMY) && !call(f::EnemyActor_isDied, {ld32(am + TARGET_ENEMY)}) &&
            (ld32(self + S_TOUCH_STATE) == 1 || ld8(self + S_TAP_ATTACK))) {
            clearTouchRequests();
            if (call(f::BaseActorManager_isPlayerAttackSkipStatus, {actors(), 0}) == 1) {
                st32(at, 0);
                call(f::AvatarActor_ClearChainAttack, {avatar});
                setState(state() == 20 ? 21 : 16);
            }
            if (call(f::AvatarActor_setChainAttackMotion, {avatar}) == 1) {
                call(f::SAM_reflectionCondition, {actors(), avatar, actors() + A_TARGET});
                call(f::MedalDeck_advanceState, {deck});
                call(f::CarouselDeck_advanceDeck, {carousel()});
                st32(am + TARGET_MEDAL, call(f::MedalDeck_getCurrentMedalInfo, {deck}));
                playerAttack(at);
                call(f::SAM_attackReadyAction, {actors(), 0});
            }
        }
        if (ld32(avatarModel() + AVATAR_MOTION) == MOTION_IDLE) {
            setState(state() == 20 ? 21 : 16);
            if (ld32(at) == ATTACK_SPECIAL) {
                clearTouchRequests();
                call(f::GuiltBurstCutInShader_setShaderEnd, {ld32(self + S_SHADER)});
            }
        }
        const uint32_t kind = ld32(at);
        if (kind != 0 && !ld8(carousel() + CAROUSEL_ATTACKING)) {
            if (ld32(at) == ATTACK_SPECIAL)
                call(f::GuiltBurstCutInShader_setShaderConcentrationStart, {ld32(self + S_SHADER)});
            const uint32_t deckUi = carousel();
            call(f::CarouselDeck_actionAttack, {deckUi, ld32(at) == ATTACK_SPECIAL ? 1u : 0u});
        }
        if (state() == 21) call(f::SAM_doJumpFallAction, {actors(), avatar});
        if (state() == 16 && call(f::MedalDeck_isNextEnemyTurn, {deck}) == 1)
            call(f::SAM_doDisengageAttackEnd, {actors(), avatar, actors() + A_TARGET});
    }
    void afterAttack() const {   // 16
        if (!calmDown()) return;
        call(f::SAM_reflectionCondition, {actors(), avatar, actors() + A_TARGET});
        setState(8);
    }
    void skipped() const {   // 17
        if (ld8(carousel() + CAROUSEL_ROLLING)) return;
        setState(8);
    }
    void jumpApproach() const {   // 18
        if (call(f::AvatarActor_takeActionFlag, {avatar, 4}) != 1) return;
        const uint32_t am = actors();
        if (call(f::MedalDeck_isCurrentFriendMedal, {deck}) == 1 && ld32(am + A_TARGET) == ATTACK_SPECIAL) {
            call(f::SAM_initFriendCoveringFireAction, {actors(), 1});
            setState(4);
            return;
        }
        call(f::SAM_doJumpUpAction, {actors(), avatar});
        setState(19);
    }
    void jumpUp() const {   // 19
        if (mapMoved) return;
        const uint32_t am = actors();
        const uint32_t kind = ld32(am + A_TARGET);
        if (kind == ATTACK_SPECIAL) {
            const uint32_t id = medalBurstId(am);
            call(f::Avatar_activeBurst, {avatarModel(), id});
            call(f::Avatar_changeMotion, {avatarModel(), 0x2F});
        } else if (kind == ATTACK_WHOLE) {
            call(f::Avatar_changeMotion, {avatarModel(), 2});
        } else {
            chainMotion(am);
        }
        setState(20);
    }
    void jumpFall() const {   // 21
        if (mapMoved) return;
        setState(16);
    }
    void selectingTargets() const {   // 22
        if (call(f::SBM_isSelectedTargetEnemy, {battle()}) != 1) return;
        setState(23);
        call(f::CommonDirection_req_select_target_hide, {common()});
    }
    // 24: the next attacking enemy turns to the player, steps in (unless it attacks from afar) and attacks after
    // a random 2..18 frames
    void enemyAttack() const {
        const uint32_t bm = battle();
        if (call(f::SBM_hasEnemyAttker, {bm}) != 1) {
            if (call(f::SBM_IsAllOverEnemyAttker, {battle(), actors() + A_ENEMIES}) == 1) setState(16);
            return;
        }
        const uint32_t enemy = call(f::SBM_getEnemyAttcker, {battle()});
        if (!enemy) return;
        Frame fr;
        const uint32_t player = fr.alloc(8), offset = fr.alloc(8), away = fr.alloc(8);
        call(f::Vec2_ctor_copy, {player, vcall(avatar, node::getPosition, {avatar})});
        const uint32_t enemyPosition = vcall(enemy, node::getPosition, {enemy});
        call(f::Vec2_ctor_copy, {away, player});
        call(f::Vec2_subtract, {away, enemyPosition});
        const float facing = ldf(away) < 0.0f ? 1.0f : -1.0f;
        vcall(enemy, Node_setScaleX, {enemy, fbits(facing)});
        const float half = bitsf(call(f::ATAN2F, {ld32(away + 4), ld32(away)})) * 0.5f;
        const double h = double(half);
        float angle;
        if (!(h < QUARTER_PI) && !std::isnan(h)) angle = float(h + HALF_PI);
        else if (h >= -QUARTER_PI || std::isnan(h)) angle = half;
        else angle = float(h + -HALF_PI);
        const float dx = bitsf(call(f::COSF, {fbits(angle)})) * 240.0f;
        const float dy = bitsf(call(f::SINF, {fbits(angle)})) * 240.0f;
        call(f::Vec2_ctor, {offset, fbits(dx), fbits(dy)});
        {
            Frame fr2;
            const uint32_t stand = fr2.alloc(8), step = fr2.alloc(8);
            call(f::Vec2_ctor_copy, {stand, player});
            call(f::Vec2_subtract, {stand, offset});
            call(f::Vec2_ctor_copy, {step, stand});
            call(f::Vec2_subtract, {step, enemyPosition});
            st32(away, ld32(step));
            st32(away + 4, ld32(step + 4));
            call(f::Vec2_dtor, {step});
            call(f::Vec2_dtor, {stand});
        }
        const uint32_t data = ld32(enemy + E_DATA);
        const uint32_t kindBuf = fr.alloc(4);
        call(f::SafeBuffer_read, {data, kindBuf, ld32(data + 0xC) + ENEMY_ATTACK_KIND, 4, ENEMY_ATTACK_KIND});
        if (ld32(kindBuf) != 3) {
            const uint32_t at = fr.alloc(8);
            call(f::Vec2_ctor_copy, {at, avatar + AV_MAP_POSITION});
            stf(at, ldf(away) + ldf(at));
            stf(at + 4, ldf(at + 4) - ldf(away + 4));
            if (call(f::StageFieldManager_isCollisionSafe, {ld32(self + S_FIELD), at}) == 1)
                call(f::MapNode_move,
                     {enemy, call(f::EaseSineOut_create, {call(f::MoveTo_create, {MOVE_SECONDS, away})})});
            call(f::Vec2_dtor, {at});
        }
        const uint32_t rate = call(f::SBM_MinHPRateEnemyTeam, {battle(), actors() + A_ENEMIES});
        call(f::EnemyActor_select_enemy_attack, {enemy, rate, actors()});
        const int32_t frames = int32_t(call(f::Random_generateUniformInt, {call(f::Random_getInstance, {}), 2, 18, 0}));
        const uint32_t delay = call(f::DelayTime_create, {fbits(float(frames) / 60.0f)});
        withFunction(VT_ENEMY_ATTACK, {self}, [&](uint32_t fo) {
            const uint32_t then = call(f::CallFunc_create, {fo});
            call(f::Node_runAction, {self, call(f::Sequence_create, {delay, then, 0})});
        });
        setState(25);
        call(f::Vec2_dtor, {offset});
        call(f::Vec2_dtor, {away});
        call(f::Vec2_dtor, {player});
    }
    void allyAttack() const {   // 26
        const uint32_t bm = battle();
        const uint32_t am = actors();
        const uint32_t rate = call(f::SBM_MinHPRateEnemyTeam, {bm, am + A_ALLIES});
        call(f::SAM_startAllyAttack, {am, rate});
        setState(27);
    }
    void allyAttackEnd() const {   // 27
        if (call(f::SAM_isEndAllyAttack, {actors()}) != 1) return;
        setState(call(f::SAM_hasAliveEnemyInTeam, {actors()}) ? 10 : 9);
    }
    void lost() const {   // 28
        writeState(stageState(), ST_TURN_FLAG, 1);
        setState(33);
        if (!ld32(ld32(self + S_STAGE) + STAGE_COLOSSEUM)) {
            withFunction(VT_CONTINUE, {deck, self, avatar},
                         [&](uint32_t fo) { call(f::StageUI_openContinueDialog, {ui(), fo}); });
            return;
        }
        uint32_t then = 0;
        withFunction(VT_COLOSSEUM_LOST, {self}, [&](uint32_t fo) { then = call(f::CallFunc_create, {fo}); });
        call(f::StageDirection_cocos_lose_colosseum, {ld32(self + S_DIRECTION), then});
    }
    void playerDown() const {   // 29
        if (ld32(avatarModel() + AVATAR_MOTION) == MOTION_IDLE) {
            setState(9);
            call(f::CarouselDeck_setPossibleBurst, {carousel(), 1, 1});
            return;
        }
        if (call(f::AvatarActor_updateActionDied, {avatar}) != 2) return;
        if (!calmDown()) return;
        if (readState(ST_REVIVES) == 1) {   // the tutorial's automatic revival
            call(f::MedalDeck_turnEnd, {deck});
            vcall(self, SCENE_ON_CONTINUE, {self, 0x45C});
            call(f::SceneActionMap_revivePlayer, {self, 1});
            writeState(stageState(), ST_REVIVES, 2);
            call(f::CommonDirection_heal, {common()});
            call(f::AvatarActor_setActionResurrection, {avatar});
            return;
        }
        setState(28);
        call(f::AvatarActor_ClearBuffDebuff, {ld32(actors() + A_AVATAR)});
    }
    void won() const {   // 30
        if (headingPlaying()) return;
        if (call(f::function_b_call, {self + S_PENDING}) != 1) {   // more of the stage left: back to the field
            call(f::AvatarActor_ClearBuffDebuff, {ld32(actors() + A_AVATAR)});
            call(f::Avatar_changeMotion, {avatarModel(), 10});
            call(f::SceneActionMap_toFieldModeSetup, {self, 0});
            return;
        }
        setState(33);
        call(f::PlayerStatusIndicator_stopAlert, {call(f::StageUI_getPlayerStatusField, {ui()})});
        withFunction(VT_WIN_TUTORIAL_DONE, {self},
                     [&](uint32_t fo) { call(f::SceneActionMap_stageFinishTutorial, {self, fo}); });
    }

    // after every step: the actors, the burst button, the HP gauge (the player goes down at 0), the camera
    void after() const {
        call(f::SAM_update, {actors(), dt});
        call(f::BaseActorManager_update_burst_enable, {actors()});
        const uint32_t hp = readState(ST_HP);
        call(f::PlayerStatusIndicator_setHP, {call(f::StageUI_getPlayerStatusField, {ui()}), hp});
        if (hp == 0 && ld8(avatar + AV_DOWN_PENDING) && !call(f::AvatarActor_isActionDied, {avatar})) {
            call(f::AvatarActor_setActionDied, {avatar});
            setState(29);
            st8(self + S_MENU_ALLOWED, 0);
        }
        if ((state() & ~1u) == 2)
            call(f::SceneActionMap_updateMimicCameraPosition, {self, dt, fbits(1.0f)});
        else
            call(f::SceneActionMap_updateCameraPosition, {self, dt, fbits(0.5f)});
    }
};
}  // namespace

void updateBattleMode(SceneActionMap *selfPtr, uint32_t dt) {
    BattleMode m{guest_addr(selfPtr), 0, 0, dt, 0};
    const uint32_t self = m.self;
    m.avatar = ld32(ld32(self + S_ACTORS) + A_AVATAR);
    m.deck = call(f::StageUI_getMedalField, {ld32(self + S_UI)});
    // a held touch: after its timer runs out it counts as a tap and a long tap
    if (ld32(self + S_TOUCH_STATE) == 1) {
        const float timer = ldf(self + S_TOUCH_TIMER);
        if (timer > 0.0f && ld8(self + S_TIMER_ON)) {
            const float left = timer - bitsf(dt);
            stf(self + S_TOUCH_TIMER, left);
            if (left <= 0.0f) {
                st8(self + S_TAP_ATTACK, 1);
                st8(self + S_TAP_LONG, 1);
                st32(self + S_TOUCH_TIMER, 0);
            }
        }
    }
    const uint32_t state = m.state();
    if (state >= 2)
        st32(ld32(self + S_FIELD) + FIELD_BATTLE_FLAG, (state == 13 || state == 22) ? 0u : 1u);
    m.mapMoved = call(f::MapNode_updateMapPosition, {m.avatar});
    switch (m.state()) {
    case 0: m.start(); break;
    case 1: m.startHeading(); break;
    case 2: m.raidEntry(); break;
    case 3: m.bossEntry(); break;
    case 4: m.coveringFire(); break;
    case 5: m.specialTargets(); break;
    case 6: m.enemyPoison(); break;
    case 7: m.enemyStatus(); break;
    case 8: m.deckAdvance(); break;
    case 9: m.wipedOutOrTurnEnd(); break;
    case 10: m.playerPoison(); break;
    case 11: m.playerStatus(); break;
    case 12: m.playerTurn(); break;
    case 13: m.approach(); break;
    case 14: m.specialAnimation(); break;
    case 15:
    case 20: m.attackAnimation(); break;
    case 16: m.afterAttack(); break;
    case 17: m.skipped(); break;
    case 18: m.jumpApproach(); break;
    case 19: m.jumpUp(); break;
    case 21: m.jumpFall(); break;
    case 22: m.selectingTargets(); break;
    case 23: call(f::SceneActionMap_onBurstButton, {self}); break;
    case 24: m.enemyAttack(); break;
    case 26: m.allyAttack(); break;
    case 27: m.allyAttackEnd(); break;
    case 28: m.lost(); break;
    case 29: m.playerDown(); break;
    case 30: m.won(); break;
    default: break;   // 25 (the enemy attack runs) and beyond
    }
    m.after();
}
RW_FUNCTION(0x5FD000, "SceneActionMap::updateBattleMode", updateBattleMode);
