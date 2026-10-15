// SceneRaidBattle: lifetime, setup (init, setRaidStage, loadMapComplete, the UI hooks), the per-frame turn state
// machine (update) and the player / enemy turn helpers. The lambdas (std::function thunks) are in
// SceneRaidBattle_lambdas.cpp.
#include "SceneRaidBattle.hpp"

using namespace scene;

namespace {

// ------------------------------------------------------------------------------------------------ guest calls
constexpr uint32_t SceneBattleBase_ctor = 0x5F9FF9;
constexpr uint32_t SceneBattleBase_dtor = 0x5FA0B5;
constexpr uint32_t SceneBattleBase_init = 0x5FA135;
constexpr uint32_t SceneBattleBase_init_UI = 0x5FA2B1;
constexpr uint32_t SceneBattleBase_firstSetPlayerCP = 0x5FAF05;   // (scene, cp, keybladeId)
constexpr uint32_t SceneBattleBase_subPlayerCP = 0x5FAD79;        // (scene, cp, const MedalInfo*)
constexpr uint32_t SceneBattleBase_invokeSkill = 0x5FA975;        // (scene, InvokeShowTiming)
constexpr uint32_t Vec2_ctor = 0x8E4B95;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t GestureRecognizer_ctor = 0x4F348D;             // (recognizer, Node* owner)
constexpr uint32_t GestureRecognizer_dtor = 0x4F3621;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Scene_create = 0x8A5F37;
constexpr uint32_t Node_create = 0x89372D;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t LWFNode_create = 0x7EDF31;                     // (const char* path, void* = 0)
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventDispatcher_addListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t RefVector_pushBack = 0x5E29A9;                 // cocos2d::Vector<Ref*>::pushBack
constexpr uint32_t PrizeVector_copy = 0x60A4ED;                   // cocos2d::Vector<MapPrizes*>(const Vector&)
constexpr uint32_t PrizeVector_eraseObject = 0x60A53F;            // (vector, MapPrizes*, bool removeAll)
constexpr uint32_t PrizeVector_dtor = 0x5DB649;
constexpr uint32_t Function_vv_assign = 0x4EB9CD;                 // std::function<void()>::operator=(const&)
constexpr uint32_t Function_b_assign = 0x53327D;                  // std::function<bool()>::operator=(const&)
constexpr uint32_t Function_bu_assign = 0x607781;                 // std::function<bool(unsigned)>::operator=
constexpr uint32_t Function_vb_assign = 0x490109;                 // std::function<void(bool)>::operator=
constexpr uint32_t Function_vb_call = 0x474139;                   // std::function<void(bool)>::operator()
constexpr uint32_t Function_touchBool_swap = 0x483E4D;            // function<bool(Touch*,Event*)>::swap
constexpr uint32_t Function_touchVoid_swap = 0x4F3699;            // function<void(Touch*,Event*)>::swap
constexpr uint32_t Function_swipe_swap = 0x606759;                // function<void(GestureRecognizer*,...)>::swap
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;

constexpr uint32_t MapTreasure_firstCall = 0x5DF521, MapTreasure_finalCall = 0x5DF5C5;
constexpr uint32_t MapMaterial_firstCall = 0x5DBABD, MapMaterial_finalCall = 0x5DBB61;
constexpr uint32_t MapPrizes_firstCall = 0x5DE939, MapPrizes_finalCall = 0x5DEA99;
constexpr uint32_t MissBoard_firstCall = 0x5D406D, MissBoard_finalCall = 0x5D4139;
constexpr uint32_t MapPrizes_isEruption = 0x5DEB59;

constexpr uint32_t AppState_createStageState = 0x4EA861;
constexpr uint32_t StageState_addDamage = 0x4EEB89;
constexpr uint32_t StageState_setPlayableActorPoisonEffect = 0x4F05C9;
constexpr uint32_t StatusChange_clear = 0x4F1D15;
constexpr uint32_t IStatusProtocol_elapseTurnStatus = 0x4F1E01;
constexpr uint32_t IStatusProtocol_elapseTurnStatusPoison = 0x4F1EA5;
constexpr uint32_t IBuffProtocol_elapseTurnAttackBuff = 0x4F211D;
constexpr uint32_t IBuffProtocol_elapseTurnDefenseBuff = 0x4F219D;
constexpr uint32_t AvatarUtil_getCurrentStyle = 0x4977AD;         // (Style* out, bool)
constexpr uint32_t AvatarUtil_getSupportUserStyle = 0x49794D;     // (Style* out)
constexpr uint32_t AvatarUtil_getCurrentKeyblade = 0x4976FD;      // (KeybladeInfo* out)
constexpr uint32_t BattleUtil_getSupportMedal = 0x497011;         // (UserMedal::Medal&, UserSkill::Skill&)
constexpr uint32_t Keyblade_create = 0x783F9D;                    // master::Keyblade::create(shared_ptr out, id)
constexpr uint32_t Burst_create = 0x77A491;
constexpr uint32_t RaidEnemy_create = 0x787745;
constexpr uint32_t Avatar_preloadResources = 0x5C7ED1;
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t FlashNode_isPlaying = 0x5EE499;
constexpr uint32_t FlashNode_reflection_status = 0x5C3171;
constexpr uint32_t AvatarActor_UpdateStatus = 0x7403A9;
constexpr uint32_t BaseActorManager_setupAvatarActorPlayable = 0x74E7C5;
constexpr uint32_t BaseActorManager_setupAvatarActorFriend = 0x74E81B;
constexpr uint32_t BaseActorManager_update_burst_enable = 0x74E871;
constexpr uint32_t StageActorManager_autoChoiceTarget = 0x74FB81;
constexpr uint32_t StageActorManager_attackReadyAction = 0x74FCED;
constexpr uint32_t StageActorManager_startFriendCoveringFire = 0x751EB1;
constexpr uint32_t StageFieldManager_setStageRaid = 0x754EC5;
constexpr uint32_t StageFieldManager_addMapNodeToField = 0x755509;
constexpr uint32_t StageFieldManager_update_raid = 0x75603D;
constexpr uint32_t StageFieldManager_update = 0x7560B5;           // (manager, float dt, AvatarActor*)
constexpr uint32_t StageFieldManager_setLuxTarget = 0x7572D1;

constexpr uint32_t StageUI_getMedalField = 0x75E65D;
constexpr uint32_t StageUI_getCarouselDeck = 0x75E661;
constexpr uint32_t StageUI_getStatusField = 0x75E669;
constexpr uint32_t StageUI_getAttributeField = 0x75E66D;
constexpr uint32_t StageUI_getPlayerStatusField = 0x75E671;
constexpr uint32_t StageUI_getLuxTargetField = 0x75E675;
constexpr uint32_t StageUI_getLuxTarget = 0x75E679;
constexpr uint32_t StageUI_createCarouselDeck = 0x75E929;
constexpr uint32_t StageUI_createStatusField = 0x75EA45;
constexpr uint32_t StageUI_createAttributeField = 0x75EF25;
constexpr uint32_t StageUI_createPlayerStatusField = 0x75F001;    // (ui, hp, cpMax, const Style&)
constexpr uint32_t StageUI_createLuxTargetField = 0x75F0B5;
constexpr uint32_t StageUI_createHeading = 0x75F355;
constexpr uint32_t StageUI_showHeading = 0x75F54D;
constexpr uint32_t StageUI_isPlayingHeading = 0x75F815;
constexpr uint32_t StageUI_setBattleTurn = 0x761727;
constexpr uint32_t MedalDeck_addMedalOntoDeck = 0x5DFB51;
constexpr uint32_t MedalDeck_addFriendMedalOntoDeck = 0x5DFE6B;
constexpr uint32_t MedalDeck_setBattleState = 0x5DFEAB;
constexpr uint32_t MedalDeck_advanceState = 0x5DFEC1;
constexpr uint32_t MedalDeck_getCurrentMedalInfo = 0x5DFF6B;
constexpr uint32_t MedalDeck_isCurrentFriendMedal = 0x5E0441;
constexpr uint32_t MedalDeck_isValidCurrentBurst = 0x5E04E5;
constexpr uint32_t MedalDeck_getCurrentBurstGauge = 0x5E0565;
constexpr uint32_t CarouselDeck_changeDeckStyle = 0x5CF521;
constexpr uint32_t CarouselDeck_addMedalOntoDeck = 0x5CF80D;
constexpr uint32_t CarouselDeck_addFriendMedalOntoDeck = 0x5D073D;
constexpr uint32_t CarouselDeck_setDeckInfoWindow = 0x5D07D1;
constexpr uint32_t CarouselDeck_reflashDeck = 0x5D0BC5;
constexpr uint32_t CarouselDeck_setPossibleBurst = 0x5D0C11;
constexpr uint32_t CarouselDeck_advanceDeck = 0x5D0C91;
constexpr uint32_t CarouselDeck_actionAttack = 0x5D0E95;
constexpr uint32_t CarouselDeck_setBattleState = 0x5D1407;
constexpr uint32_t PlayerStatusIndicator_setHP = 0x5E3119;
constexpr uint32_t RaidBossStatusIndicator_setRaidBossHP = 0x5E9191;

constexpr uint32_t RaidBattle_create = 0x60867D;
constexpr uint32_t RaidBattle_startFriend = 0x5EE0AD;
constexpr uint32_t RaidBattle_start = 0x5EDF51;
constexpr uint32_t RaidBattle_startEnemy = 0x5EE209;
constexpr uint32_t RaidBattle_moveCamera = 0x5EF751;              // (battle, float, const Vec2&, float, bool)
constexpr uint32_t RaidBattle_setRaidEnemyPoisonEffect = 0x5F0DFD;
constexpr uint32_t RaidBoss_preloadResources = 0x5D7391;
constexpr uint32_t RaidBoss_changeMotion = 0x5D6815;
constexpr uint32_t RaidBossActor_create = 0x5F1EC9;               // (raidId, partyFlag)
constexpr uint32_t RaidBossActor_getCurrentHP = 0x5F2ECD;
constexpr uint32_t RaidBossActor_updateDamageAll = 0x5F2F2D;
constexpr uint32_t RaidBossActor_updateTargetPosition = 0x5F34A1;
constexpr uint32_t RaidBossActor_ShowTarget = 0x5F3641;
constexpr uint32_t RaidBossActor_ShowTargetInex = 0x5F36E9;
constexpr uint32_t RaidBossActor_HideTargetInex = 0x5F3725;
constexpr uint32_t RaidBossActor_marker_mode_chamge = 0x5F3745;
constexpr uint32_t RaidBossActor_HideTarget = 0x5F3761;
constexpr uint32_t RaidBossActor_get_active_parts = 0x5F37A5;
constexpr uint32_t RaidBossActor_make_marker = 0x5F37D1;          // (boss, scene, Node*, float scale)
constexpr uint32_t RaidBossActor_getType = 0x5F3AA5;
constexpr uint32_t RaidBossActor_getMaxHP = 0x5F3B05;
constexpr uint32_t RaidBossActor_makeWholeAttackPoint = 0x5F3B65;
constexpr uint32_t RaidBossActor_getEnemyHp = 0x5F3CE9;
constexpr uint32_t RaidBossActor_find_marker = 0x5F3D79;          // (Vec2* out, boss, index)
constexpr uint32_t RaidBossActor_createRaidBossStatusField = 0x5F3DF1;
constexpr uint32_t RaidBossActor_calc_total_attack_damage = 0x5F4761;
constexpr uint32_t RaidBossActor_getNumberOfParts = 0x5F576D;
constexpr uint32_t RaidBossActor_elapsePartsTurn = 0x5F6A99;
constexpr uint32_t RaidBossActor_UpdateStatus = 0x5F6BD9;
constexpr uint32_t RaidDirection_create = 0x6086D1;
constexpr uint32_t RaidDirection_preloadResources = 0x5F7D4D;
constexpr uint32_t RaidDirection_raid_fever = 0x5F7E69;
constexpr uint32_t RaidDirection_setup_helper = 0x5F8D81;
constexpr uint32_t RaidDirection_finalWhiteIn = 0x5F947D;
constexpr uint32_t CommonDirection_create = 0x5FC921;
constexpr uint32_t CommonDirection_preloadResources = 0x764C1D;
constexpr uint32_t CommonDirection_req_materialPanel_in = 0x765491;
constexpr uint32_t CommonDirection_req_skilPanel_in = 0x765A19;
constexpr uint32_t CommonDirection_to_start = 0x76669D;
constexpr uint32_t CommonDirection_heal = 0x766DF9;
constexpr uint32_t CommonDirection_black_out = 0x767039;
constexpr uint32_t GuiltBurstCutInShader_create = 0x5FFC51;
constexpr uint32_t GuiltBurstCutInShader_addShaderParent = 0x495561;
constexpr uint32_t GuiltBurstCutInShader_addRaidBossBg = 0x609B5D;
constexpr uint32_t GuiltBurstCutInShader_updateInit = 0x4960E1;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformFloat = 0x492261;        // (random, float min, float max, unsigned* = 0)
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t SceneUtil_createBottomInAnimation = 0x45B2F9;
constexpr uint32_t SceneUtil_createTopInAnimation = 0x45B03D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;      // (mgr, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_executeRaidClear = 0x4CC769;        // (api, parts, const array<RaidParts,6>&, done)

// ------------------------------------------------------------------------------------------------ lambdas
// __func vtables (+8) of the scene's lambdas (thunks in SceneRaidBattle_lambdas.cpp)
constexpr uint32_t VT_update_20 = 0xF5EB90;          // update(): black_out callback (tutorial ending)
constexpr uint32_t VT_update_19 = 0xF5EC14;          // update(): tutorial popup closed -> start the battle
constexpr uint32_t VT_loadUI_18 = 0xF5EC40;          // gesture swipe
constexpr uint32_t VT_loadUI_17 = 0xF5EC6C;          // touch ended
constexpr uint32_t VT_loadUI_16 = 0xF5EC98;          // touch cancelled
constexpr uint32_t VT_loadUI_15 = 0xF5ECC4;          // touch moved
constexpr uint32_t VT_loadUI_14 = 0xF5ECF0;          // touch began
constexpr uint32_t VT_setRaidStage_13 = 0xF5ED1C;    // avatar: add CP
constexpr uint32_t VT_setRaidStage_12 = 0xF5ED48;    // avatar: add HP
constexpr uint32_t VT_setRaidStage_11 = 0xF5ED74;    // raid cover direction
constexpr uint32_t VT_setRaidStage_10 = 0xF5EDCC;    // camera back to the centre
constexpr uint32_t VT_setRaidStage_9 = 0xF5EDF8;     // friend medal used: friend burst
constexpr uint32_t VT_setRaidStage_8 = 0xF5EE24;     // medal used: player burst
constexpr uint32_t VT_setRaidStage_7 = 0xF5EE50;     // carousel: burst gauge enough
constexpr uint32_t VT_setRaidStage_6 = 0xF5EE7C;     // carousel: deck enabled
constexpr uint32_t VT_setRaidStage_5 = 0xF5EEA8;     // carousel: burst button
constexpr uint32_t VT_to_slow_4 = 0xF5EED4;
constexpr uint32_t VT_to_slow_3 = 0xF5EF00;
constexpr uint32_t VT_to_slow_2 = 0xF5EF2C;
constexpr uint32_t VT_init_1 = 0xF5EF58;             // scene end: restore the time scale
constexpr uint32_t VT_init_0 = 0xF5EF84;             // scene start: schedule update, keep the time scale
constexpr uint32_t VT_LayerFactory = 0xF7E130;       // __func<Layer*(*)(), ..., Layer*()>
constexpr uint32_t createSceneLayer_SceneBattleResult = 0x601365;

// ------------------------------------------------------------------------------------------------ values
constexpr uint32_t RAID_BG_LWF_TABLE = 0xF5EB6C;     // const char*[]: background LWF per map type (0: none)
constexpr uint32_t STR_RaidEffBack = 0xE02E76;       // "lwf/raid/bg/RB_0003_00_00_eff_back/....lwf"
constexpr uint32_t STR_RaidEffFront = 0xE02EB4;      // "lwf/raid/bg/RB_0003_00_00_eff_front/....lwf"

// guest object offsets of other classes
constexpr uint32_t ACTORS_PLAYER = 0x20, ACTORS_FRIEND = 0x24, ACTORS_PLAY_STYLE = 0x30;
constexpr uint32_t ACTORS_DIRECTION = 0x98, ACTORS_SCENE = 0x9C;
constexpr uint32_t ACTOR_AVATAR = 0x288;             // AvatarActor: Avatar* (FlashNode)
constexpr uint32_t ACTOR_DIRECTION = 0x290;          // AvatarActor: CommonDirection*
constexpr uint32_t ACTOR_SHADOW = 0x2AC;             // AvatarActor: the shadow node
constexpr uint32_t ACTOR_ADD_HP = 0x2E0;             // AvatarActor: std::function<bool(unsigned)>
constexpr uint32_t ACTOR_ADD_CP = 0x2F8;             // AvatarActor: std::function<bool(unsigned)>
constexpr uint32_t AVATAR_STYLE = 0x56C;             // Avatar: const Avatar::Style*
constexpr uint32_t FLASH_MOTION = 0x524;             // FlashNode: current motion
constexpr uint32_t BOSS_SPREAD = 0x398;              // RaidBossActor: int, int (-1 = none)
constexpr uint32_t BOSS_WHOLE_POINT = 0x390;         // RaidBossActor: cocos2d::Vec2 whole attack point
constexpr uint32_t BOSS_PART_TARGET = 0x3A0;         // RaidBossActor: bool[6] parts hit by the attack
constexpr uint32_t BOSS_STATUS_INDICATOR = 0x3A8;    // RaidBossActor: RaidBossStatusIndicator*
constexpr uint32_t BOSS_DIRECTION_NODE = 0x3AC;      // RaidBossActor: CommonDirection's node
constexpr uint32_t BOSS_PARTS = 0x3B4;               // RaidBossActor: std::array<RaidParts, 6>
constexpr uint32_t BOSS_STATUS = 0x2A8;              // RaidBossActor: IStatusProtocol
constexpr uint32_t BOSS_BUFF = 0x2AC;                // RaidBossActor: IBuffProtocol
constexpr uint32_t BOSS_PRIZES = 0x7B8;              // RaidBossActor: cocos2d::Vector<MapPrizes*>
constexpr uint32_t BOSS_PART_LIST = 0x894;           // RaidBossActor: list of parts (next, ..., part at +0xC)
constexpr uint32_t BOSS_RAID_BOSS = 0x8A0;           // RaidBossActor: RaidBoss* (FlashNode)
constexpr uint32_t BOSS_RAID_ENEMY_ID = 0x8A4;
constexpr uint32_t BOSS_FIELD_ARG = 0x8A8;
constexpr uint32_t PART_BROKEN = 0x27C;              // a part node: being destroyed
constexpr uint32_t BATTLE_SCENE = 0x270;             // RaidBattle: SceneRaidBattle*
constexpr uint32_t BATTLE_ACTOR = 0x274;             // RaidBattle: AvatarActor* attacking
constexpr uint32_t BATTLE_RAID_DIRECTION = 0x284;
constexpr uint32_t BATTLE_COMMON_DIRECTION = 0x288;
constexpr uint32_t BATTLE_STEP = 0x28C;              // RaidBattle: attack sequence step (0 = done)
constexpr uint32_t BATTLE_ATTACKS = 0x320;           // RaidBattle: attacks in this turn
constexpr uint32_t BATTLE_FEVER = 0x31C;
constexpr uint32_t BATTLE_CUTIN = 0x328;
constexpr uint32_t RDIR_BOSS = 0x270, RDIR_CAMERA = 0x274, RDIR_NODE = 0x2E0;
constexpr uint32_t CDIR_NODE_A = 0x270, CDIR_NODE_B = 0x274, CDIR_MODE = 0x278, CDIR_FIELD = 0x384;
constexpr uint32_t MEDALDECK_STATE = 0x368;
constexpr uint32_t CAROUSEL_BURST_BUTTON = 0x3E0, CAROUSEL_DECK_ENABLED = 0x3C8, CAROUSEL_BURST_ENOUGH = 0x3B0;
constexpr uint32_t CAROUSEL_CUTIN = 0x418;
constexpr uint32_t SCENEMGR_RAID_RESULT = 0x4CC;     // SceneManager: bool
constexpr uint32_t SCENEMGR_RAID_DONE = 0x548;       // SceneManager: std::function<void(bool)>
constexpr uint32_t APP_STAGE_STATE = 0x20;
constexpr uint32_t API_USER_STATUS = 0x8C;           // APIManager: SafeBuffer* (index 0x38: max HP)
constexpr uint32_t API_RAID_INFO = 0x178;            // APIManager: shared_ptr<RaidBase::Info>
constexpr uint32_t DIRECTOR_SCHEDULER = 0x60, DIRECTOR_DISPATCHER = 0x84, SCHEDULER_TIME_SCALE = 0x14;
constexpr uint32_t VSLOT_PLAY_SE = 0x308;            // SceneBattleBase virtual: play a sound effect (id)

constexpr uint32_t SE_REVIVE = 1116;

// ------------------------------------------------------------------------------------------------ helpers
uint32_t addr(const SceneRaidBattle *self) { return rw::guest_addr(self); }
uint32_t appState() { return call(fn::AppState_getInstance, {}); }
uint32_t stageState() { return ld32(appState() + APP_STAGE_STATE); }
int32_t stageHP() { return bt::sbGet32(stageState(), 8); }
uint32_t player(const SceneRaidBattle *self) { return ld32(self->actorManager + ACTORS_PLAYER); }
uint32_t carouselDeck(const SceneRaidBattle *self) { return call(StageUI_getCarouselDeck, {self->stageUI}); }
uint32_t medalField(const SceneRaidBattle *self) { return call(StageUI_getMedalField, {self->stageUI}); }

void addChild(uint32_t node, uint32_t child) { vcall(node, node::addChild, {node, child}); }
void addChildZ(uint32_t node, uint32_t child, int32_t z) { vcall(node, node::addChildZ, {node, child, uint32_t(z)}); }
void retain(SceneRaidBattle *self, uint32_t obj) { call(RefVector_pushBack, {rw::guest_addr(self->retained), obj}); }

// dst = std::function holding [captured](...) (the inline lambda temporary is built, assigned, destroyed)
void assignLambda(uint32_t assign, uint32_t dst, uint32_t vtable, uint32_t captured) {
    Frame f;
    uint32_t tmp = newFunction(f, vtable, {captured});
    call(assign, {dst, tmp});
    functionDestroy(tmp);
}
// std::function::swap with a lambda temporary (the listener's previous callable is destroyed with it)
void swapLambda(uint32_t swap, uint32_t dst, uint32_t vtable, uint32_t captured) {
    Frame f;
    uint32_t tmp = newFunction(f, vtable, {captured});
    call(swap, {tmp, dst});
    functionDestroy(tmp);
}
// CallFunc::create([captured] {...})
uint32_t callFuncLambda(uint32_t vtable, uint32_t captured) {
    Frame f;
    uint32_t tmp = newFunction(f, vtable, {captured});
    uint32_t action = call(fn::CallFunc_create, {tmp});
    functionDestroy(tmp);
    return action;
}
// node->runAction(EaseSineOut(create{Top,Bottom}InAnimation(node, 0, 0, {})))
void slideIn(uint32_t create, uint32_t node) {
    Frame f;
    uint32_t done = newEmptyFunction(f);
    uint32_t anim = call(create, {node, fbits(0.0f), fbits(0.0f), done});
    call(fn::Node_runAction, {node, call(EaseSineOut_create, {anim})});
    functionDestroy(done);
}
// actor->setPosition(Vec2(x, y)) with a constructed / destroyed stack Vec2
void setActorPosition(uint32_t actor, float x, float y) {
    Frame f;
    uint32_t pos = newVec2(f, x, y);
    vcall(actor, node::setPosition, {actor, pos});
    call(Vec2_dtor, {pos});
}
// An avatar placed facing the boss: position, mirrored scale, its shadow scaled like `scaleFrom`, idle motion,
// added to the camera node.
void placeAvatar(SceneRaidBattle *self, uint32_t actor, uint32_t scaleFrom, float x, int32_t z) {
    call(Avatar_preloadResources, {ld32(actor + ACTOR_AVATAR)});
    setActorPosition(actor, x, -480.0f);
    vcall(actor, 0x48, {actor, fbits(-1.3f), fbits(1.3f)});      // setScale(sx, sy)
    uint32_t shadow = ld32(actor + ACTOR_SHADOW);
    uint32_t setScaleX = ld32(ld32(shadow) + node::setScaleX);
    call(setScaleX, {shadow, vcall(scaleFrom, node::getScaleX, {scaleFrom})});
    shadow = ld32(actor + ACTOR_SHADOW);
    uint32_t setScaleY = ld32(ld32(shadow) + node::setScaleY);
    call(setScaleY, {shadow, vcall(scaleFrom, node::getScaleY, {scaleFrom})});
    call(Avatar_changeMotion, {ld32(actor + ACTOR_AVATAR), 11});
    uint32_t cam = self->camera;
    addChildZ(cam, actor, z);
}
// the scheduler's time scale
uint32_t scheduler() { return ld32(director() + DIRECTOR_SCHEDULER); }

}  // namespace

// ================================================================================================ lifetime

uint32_t SceneRaidBattle::create() {
    uint32_t p = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<SceneRaidBattle>(p));
    if (vcall(p, 0x248, {p}) == 1) {             // init()
        call(Ref_autorelease, {p});
        return p;
    }
    vcall(p, 0x04, {p});                          // delete
    return 0;
}
RW_FUNCTION(0x57A424, "SceneRaidBattle::create", SceneRaidBattle::create);

SceneRaidBattle *SceneRaidBattle::construct(SceneRaidBattle *self) {
    uint32_t a = addr(self);
    call(SceneBattleBase_ctor, {a});
    self->vtable = VTABLE;
    call(Vec2_ctor, {a + 0x2F8});
    self->camera = 0;
    self->raidBattle = 0;
    self->boss = 0;
    self->raidDirection = 0;
    self->commonDirection = 0;
    call(GestureRecognizer_ctor, {a + 0x318, a});
    self->cutInShader = 0;
    self->isTutorial = 0;
    self->deckEnabled = 1;
    self->markerActive = 0;
    self->partyRescue = 1;
    return self;
}
RW_FUNCTION(0x57A478, "SceneRaidBattle::SceneRaidBattle", SceneRaidBattle::construct);

uint32_t SceneRaidBattle::createScene() {
    uint32_t scene = call(Scene_create, {});
    uint32_t layer = create();
    addChild(scene, layer);
    return scene;
}
RW_FUNCTION(0x608184, "SceneRaidBattle::createScene", SceneRaidBattle::createScene);

SceneRaidBattle *SceneRaidBattle::destruct(SceneRaidBattle *self) {
    uint32_t a = addr(self);
    self->vtable = VTABLE;
    call(MapTreasure_finalCall, {});
    call(MapMaterial_finalCall, {});
    call(MapPrizes_finalCall, {});
    call(MissBoard_finalCall, {});
    call(GestureRecognizer_dtor, {a + 0x318});
    call(Vec2_dtor, {a + 0x2F8});
    call(SceneBattleBase_dtor, {a});
    return self;
}
RW_FUNCTION(0x6081A8, "SceneRaidBattle::~SceneRaidBattle", SceneRaidBattle::destruct);

void SceneRaidBattle::destructDelete(SceneRaidBattle *self) {
    destruct(self);
    call(fn::operator_delete, {addr(self)});
}
RW_FUNCTION(0x608220, "SceneRaidBattle::~SceneRaidBattle(deleting)", SceneRaidBattle::destructDelete);

// ================================================================================================ setup

// The battle objects (RaidBattle, the two directions, the camera node) and the player's avatar on the left.
bool SceneRaidBattle::init(SceneRaidBattle *self) {
    uint32_t a = addr(self);
    if (call(SceneBattleBase_init, {a}) != 1) return false;
    call(MapTreasure_firstCall, {});
    call(MapMaterial_firstCall, {});
    call(MapPrizes_firstCall, {});
    call(MissBoard_firstCall, {});
    call(AppState_createStageState, {appState(), 1});

    self->raidBattle = call(RaidBattle_create, {});
    retain(self, self->raidBattle);
    addChildZ(a, self->raidBattle, 80);
    rw::save<uint32_t>(self->actorManager + ACTORS_SCENE, a);

    self->raidDirection = call(RaidDirection_create, {});
    retain(self, self->raidDirection);
    addChildZ(a, self->raidDirection, 50);
    call(RaidDirection_preloadResources, {self->raidDirection});

    self->commonDirection = call(CommonDirection_create, {});
    retain(self, self->commonDirection);
    addChildZ(a, self->commonDirection, 70);
    addChildZ(a, ld32(self->commonDirection + CDIR_NODE_A), 100);
    st32(self->commonDirection + CDIR_MODE, 2);
    call(CommonDirection_preloadResources, {self->commonDirection});
    st32(self->commonDirection + CDIR_FIELD, self->fieldManager);
    st32(self->actorManager + ACTORS_DIRECTION, self->commonDirection);

    self->battleState = INTRO;
    self->waitFrames = 120;
    assignLambda(Function_vv_assign, a + 0x210, VT_init_0, a);
    assignLambda(Function_vv_assign, a + 0x228, VT_init_1, a);

    // the camera node, centred on the visible area, half scale
    self->camera = call(Node_create, {});
    retain(self, self->camera);
    Size size = visibleSize();
    Vec2 origin = visibleOrigin();
    self->cameraX = origin.x + size.width * 0.5f;
    self->cameraY = origin.y + size.height * 0.5f;
    uint32_t cam = self->camera;
    vcall(cam, node::setPosition, {cam, a + 0x2F8});
    cam = self->camera;
    vcall(cam, node::setScale, {cam, fbits(0.5f)});
    addChild(a, self->camera);

    uint32_t battle = self->raidBattle;
    st32(battle + BATTLE_SCENE, a);
    st32(self->raidBattle + BATTLE_RAID_DIRECTION, self->raidDirection);
    st32(self->raidBattle + BATTLE_COMMON_DIRECTION, self->commonDirection);
    addChildZ(a, ld32(self->commonDirection + CDIR_NODE_B), 60);
    addChildZ(a, ld32(self->raidDirection + RDIR_NODE), 151);
    st32(self->actorManager + ACTORS_PLAY_STYLE, 2);           // PlayStyle raid

    // the player's avatar
    Frame f;
    uint32_t style = f.alloc(32);                             // Avatar::Style (a SafeBuffer)
    call(AvatarUtil_getCurrentStyle, {style, 0});
    call(BaseActorManager_setupAvatarActorPlayable, {self->actorManager, style});
    uint32_t actor = player(self);
    placeAvatar(self, actor, actor, -500.0f, 200);

    uint32_t target = newVec2(f, -500.0f, size.height * -0.5f);
    call(RaidBattle_moveCamera, {self->raidBattle, fbits(0.1f), target, fbits(1.0f), 1});
    st32(actor + ACTOR_DIRECTION, self->commonDirection);
    call(Vec2_dtor, {target});
    call(SafeBuffer_dtor, {style});
    return true;
}
RW_FUNCTION(0x60822C, "SceneRaidBattle::init", SceneRaidBattle::init);

// Slow motion on the scene: three steps of the scheduler's time scale (lambdas $_2, $_3, $_4).
void SceneRaidBattle::to_slow(SceneRaidBattle *self) {
    uint32_t a = addr(self);
    uint32_t step1 = callFuncLambda(VT_to_slow_2, a);
    uint32_t wait1 = call(fn::DelayTime_create, {0x3DEEEEEF});      // ~0.1167 s
    uint32_t step2 = callFuncLambda(VT_to_slow_3, a);
    uint32_t wait2 = call(fn::DelayTime_create, {0x3C888889});      // ~0.0167 s
    uint32_t step3 = callFuncLambda(VT_to_slow_4, a);
    uint32_t battle = self->raidBattle;
    call(fn::Node_runAction, {battle, call(fn::Sequence_create, {step1, wait1, step2, wait2, step3, 0})});
}
RW_FUNCTION(0x608724, "SceneRaidBattle::to_slow", SceneRaidBattle::to_slow);

// The battle UI comes in: the boss status bar (name, HP, type), the input hooks, the fever level.
void SceneRaidBattle::RaidBattle_Start(SceneRaidBattle *self) {
    uint32_t a = addr(self);
    call(SceneBattleBase_init_UI, {a});
    uint32_t status = call(StageUI_getStatusField, {self->stageUI});
    vcall(status, node::setVisible, {status, 0});

    uint32_t boss = self->boss;
    Frame f;
    uint32_t name = f.alloc(12);
    vcall(boss, 0x150, {name, boss});                         // std::string name()
    uint32_t text = strData(name);
    uint32_t hp = call(RaidBossActor_getCurrentHP, {self->boss, 0});
    uint32_t maxHp = call(RaidBossActor_getMaxHP, {self->boss, 0});
    uint32_t arg = ld32(self->boss + BOSS_FIELD_ARG);
    uint32_t type = call(RaidBossActor_getType, {self->boss, 0}) - 1;
    call(RaidBossActor_createRaidBossStatusField, {boss, text, hp, maxHp, arg, type});
    strFree(name);

    uint32_t indicator = ld32(self->boss + BOSS_STATUS_INDICATOR);
    call(RaidBossStatusIndicator_setRaidBossHP, {indicator, call(RaidBossActor_getCurrentHP, {self->boss, 0})});
    slideIn(SceneUtil_createBottomInAnimation, indicator);
    addChildZ(a, indicator, 8);
    st32(self->boss + BOSS_DIRECTION_NODE, ld32(self->commonDirection + CDIR_NODE_B));
    loadUIComplete(self);

    int32_t parts = int32_t(call(RaidBossActor_getNumberOfParts, {self->boss}));
    int32_t active = int32_t(call(RaidBossActor_get_active_parts, {self->boss}));
    uint32_t battle = self->raidBattle;
    uint32_t fever = uint32_t(parts - 1 - active) + ld32(battle + BATTLE_FEVER);
    st32(battle + BATTLE_FEVER, fever);
    call(RaidDirection_raid_fever, {ld32(battle + BATTLE_RAID_DIRECTION), fever});
}
RW_FUNCTION(0x608894, "SceneRaidBattle::RaidBattle_Start", SceneRaidBattle::RaidBattle_Start);

// Touch / swipe input on, the decks in battle mode.
void SceneRaidBattle::loadUIComplete(SceneRaidBattle *self) {
    uint32_t a = addr(self);
    self->triggerFired = 0;
    self->field_2DE = 0;
    self->triggerTimer = 0.5f;
    self->field_2DF = 0;
    self->triggerRunning = 1;
    self->triggerMode = 0;
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    swapLambda(Function_touchBool_swap, listener + 0x50, VT_loadUI_14, a);    // onTouchBegan
    swapLambda(Function_touchVoid_swap, listener + 0x68, VT_loadUI_15, a);    // onTouchMoved
    swapLambda(Function_touchVoid_swap, listener + 0x98, VT_loadUI_16, a);    // onTouchCancelled
    swapLambda(Function_touchVoid_swap, listener + 0x80, VT_loadUI_17, a);    // onTouchEnded
    call(EventDispatcher_addListenerWithSceneGraphPriority, {ld32(director() + DIRECTOR_DISPATCHER), listener, a});
    swapLambda(Function_swipe_swap, a + 0x330, VT_loadUI_18, a);             // gesture onSwipe
    call(MedalDeck_setBattleState, {medalField(self), 1});
    call(CarouselDeck_setBattleState, {carouselDeck(self), 1});
    call(CarouselDeck_reflashDeck, {carouselDeck(self)});
}
RW_FUNCTION(0x608A74, "SceneRaidBattle::loadUIComplete", SceneRaidBattle::loadUIComplete);

// The raid: background, the boss (the tutorial's boss or raidId's), the friend avatar, the decks (keyblade medals
// and the support medal), the player status, the opening camera move.
void SceneRaidBattle::setRaidStage(SceneRaidBattle *self, int32_t raidId, int32_t mapType, int32_t unused,
                                   uint32_t tutorial, int32_t movieId, uint32_t movieFlag) {
    (void)unused;
    uint32_t a = addr(self);
    if (!self->cutInShader) {
        self->cutInShader = call(GuiltBurstCutInShader_create, {});
        addChild(a, self->cutInShader);
        call(GuiltBurstCutInShader_addShaderParent, {self->cutInShader, 0, 0});
    }
    self->isTutorial = uint8_t(tutorial);
    self->movieId = movieId;
    self->movieFlag = uint8_t(movieFlag);
    loadMapComplete(self, mapType);

    Frame f;
    if (self->isTutorial) {
        self->partyRescue = 0;
        self->boss = call(RaidBossActor_create, {0, 0});
    } else {
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t ctrl = ld32(api + API_RAID_INFO + 4);
        uint32_t info = ld32(api + API_RAID_INFO);
        if (ctrl) call(fn::shared_add, {ctrl});
        uint32_t party = 1;
        if (info) {
            uint32_t buf = f.alloc(0xA8);                    // hole::network::api::RaidBase::Info
            const SafeBuffer *sb = rw::host_ptr<SafeBuffer>(info);
            SafeBuffer::read(sb, rw::host_ptr<uint8_t>(buf), sb->data.get(), 0xA8, 0);
            self->partyRescue = uint8_t(ld8(buf + 0x18) ^ 1);
            party = ld32(buf + 8);
        }
        self->boss = call(RaidBossActor_create, {uint32_t(raidId), party});
        if (ctrl) call(fn::shared_release, {ctrl});
    }
    retain(self, self->boss);
    call(RaidBoss_preloadResources, {ld32(self->boss + BOSS_RAID_BOSS)});
    setActorPosition(self->boss, 0.0f, -480.0f);
    uint32_t cam = self->camera;
    addChildZ(cam, self->boss, 100);
    st32(self->raidDirection + RDIR_CAMERA, self->camera);
    st32(self->raidDirection + RDIR_BOSS, self->boss);
    call(RaidDirection_setup_helper, {self->raidDirection, self->isTutorial});
    cam = self->camera;
    uint32_t boss = self->boss;
    float camScale = bitsf(vcall(cam, node::getScale, {cam}));
    call(RaidBossActor_make_marker, {boss, a, cam, fbits(1.0f / camScale)});

    if (!self->isTutorial) {
        uint32_t scaleFrom = player(self);
        uint32_t style = f.alloc(32);
        call(AvatarUtil_getSupportUserStyle, {style});
        call(BaseActorManager_setupAvatarActorFriend, {self->actorManager, style});
        uint32_t friendActor = ld32(self->actorManager + ACTORS_FRIEND);
        placeAvatar(self, friendActor, scaleFrom, -436.0f, 202);
        call(SafeBuffer_dtor, {style});
    }

    call(StageUI_createCarouselDeck, {self->stageUI});
    call(StageUI_createStatusField, {self->stageUI, 0});
    call(StageUI_createAttributeField, {self->stageUI});
    call(StageUI_createLuxTargetField, {self->stageUI});
    assignLambda(Function_b_assign, carouselDeck(self) + CAROUSEL_BURST_BUTTON, VT_setRaidStage_5, a);
    assignLambda(Function_b_assign, carouselDeck(self) + CAROUSEL_DECK_ENABLED, VT_setRaidStage_6, a);
    assignLambda(Function_b_assign, carouselDeck(self) + CAROUSEL_BURST_ENOUGH, VT_setRaidStage_7, a);
    uint32_t addChildFn = ld32(ld32(a) + node::addChildZ);
    call(addChildFn, {a, carouselDeck(self), 55});
    addChildFn = ld32(ld32(a) + node::addChildZ);
    call(addChildFn, {a, call(StageUI_getStatusField, {self->stageUI}), 8});
    addChildFn = ld32(ld32(a) + node::addChildZ);
    call(addChildFn, {a, call(StageUI_getAttributeField, {self->stageUI}), 8});
    addChildFn = ld32(ld32(a) + node::addChildZ);
    call(addChildFn, {a, call(StageUI_getLuxTargetField, {self->stageUI}), 8});
    uint32_t field = self->fieldManager;
    call(StageFieldManager_setLuxTarget, {field, call(StageUI_getLuxTarget, {self->stageUI}), self->camera});

    // the keyblade's medals
    uint32_t medals = medalField(self);
    uint32_t keyInfo = f.alloc(0x58);                          // AvatarUtil::getCurrentKeyblade() result
    call(AvatarUtil_getCurrentKeyblade, {keyInfo});
    uint32_t keyblade = f.alloc(8);                            // shared_ptr<const master::Keyblade>
    call(Keyblade_create, {keyblade, ld32(keyInfo + 0xC)});
    if (ld32(keyblade)) {
        Frame g;
        uint32_t cb = newFunction(g, VT_setRaidStage_8, {a});
        call(MedalDeck_addMedalOntoDeck, {medals, keyblade, keyInfo + 0x18, ld32(keyInfo + 0x10), cb});
        functionDestroy(cb);
        uint32_t deck = carouselDeck(self);
        int32_t deckStyle = bt::sbGet32(ld32(keyblade), 0xD0);
        call(CarouselDeck_changeDeckStyle, {deck, uint32_t(deckStyle)});
        call(CarouselDeck_addMedalOntoDeck, {carouselDeck(self), keyInfo + 0x18, ld32(keyInfo + 0x10)});
    }
    // the friend's support medal
    if (ld32(self->actorManager + ACTORS_FRIEND)) {
        uint32_t medal = f.alloc(0x48);                        // hole::network::api::UserMedal::Medal
        uint32_t skill = f.alloc(0x10);                        // hole::network::api::UserSkill::Skill
        if (call(BattleUtil_getSupportMedal, {medal, skill}) == 1) {
            Frame g;
            uint32_t cb = newFunction(g, VT_setRaidStage_9, {a});
            call(MedalDeck_addFriendMedalOntoDeck, {medals, keyblade, medal, cb});
            functionDestroy(cb);
            call(CarouselDeck_addFriendMedalOntoDeck, {carouselDeck(self), ld32(medal + 8), ld32(skill + 8)});
        }
    }
    call(CarouselDeck_setDeckInfoWindow, {carouselDeck(self), medals});
    call(CarouselDeck_reflashDeck, {carouselDeck(self)});
    call(SceneBattleBase_firstSetPlayerCP, {a, ld32(keyInfo + 0x40), ld32(keyInfo + 0xC)});

    // the player status (HP from the user status, CP max from the stage state)
    uint32_t ui = self->stageUI;
    uint32_t api = call(fn::APIManager_getInstance, {});
    int32_t maxHp = bt::sbGet32(ld32(api + API_USER_STATUS), 0x38);
    int32_t cpMax = bt::sbGet32(stageState(), 0x34);
    uint32_t avatar = ld32(player(self) + ACTOR_AVATAR);
    call(StageUI_createPlayerStatusField, {ui, uint32_t(maxHp), uint32_t(cpMax), ld32(avatar + AVATAR_STYLE)});
    addChildFn = ld32(ld32(a) + node::addChildZ);
    call(addChildFn, {a, call(StageUI_getPlayerStatusField, {self->stageUI}), 8});
    slideIn(SceneUtil_createTopInAnimation, call(StageUI_getStatusField, {self->stageUI}));

    // the opening: camera back to the centre, then the raid cover direction
    uint32_t wait1 = call(fn::DelayTime_create, {fbits(0.3f)});
    uint32_t wait2 = call(fn::DelayTime_create, {fbits(0.8f)});
    uint32_t centre = callFuncLambda(VT_setRaidStage_10, a);
    uint32_t cover = callFuncLambda(VT_setRaidStage_11, a);
    call(fn::Node_runAction, {a, call(fn::Sequence_create, {wait1, centre, wait2, cover, 0})});
    call(StageFieldManager_setStageRaid, {self->fieldManager, a});

    assignLambda(Function_bu_assign, player(self) + ACTOR_ADD_HP, VT_setRaidStage_12, a);
    assignLambda(Function_bu_assign, player(self) + ACTOR_ADD_CP, VT_setRaidStage_13, a);
    if (self->cutInShader) {
        st32(carouselDeck(self) + CAROUSEL_CUTIN, self->cutInShader);
        st32(self->raidBattle + BATTLE_CUTIN, self->cutInShader);
    }
    sharedRelease(keyblade);
}
RW_FUNCTION(0x608C6C, "SceneRaidBattle::setRaidStage", SceneRaidBattle::setRaidStage);

// The background LWFs for map type `mapType` (0: none) on the camera node, behind and in front of the boss.
void SceneRaidBattle::loadMapComplete(SceneRaidBattle *self, int32_t mapType) {
    if (mapType == 0) return;
    uint32_t bg = call(LWFNode_create, {ld32(RAID_BG_LWF_TABLE + 4 * uint32_t(mapType)), 0});
    const float designW = 1136.0f, designH = 640.0f;
    Size visible = visibleSize();
    setActorPosition(bg, 0.0f, 0.0f);
    vcall(bg, 0x48, {bg, fbits(2.0f), fbits(2.0f)});
    uint32_t cam = self->camera;
    addChildZ(cam, bg, 0);
    if (self->cutInShader) call(GuiltBurstCutInShader_addRaidBossBg, {self->cutInShader, bg});

    float y = designH - (visible.height + -640.0f);
    uint32_t back = call(LWFNode_create, {STR_RaidEffBack, 0});
    setActorPosition(back, -designW, y);
    vcall(back, 0x48, {back, fbits(2.0f), fbits(2.0f)});
    cam = self->camera;
    addChildZ(cam, back, 10);
    if (self->cutInShader) call(GuiltBurstCutInShader_addRaidBossBg, {self->cutInShader, back});

    uint32_t front = call(LWFNode_create, {STR_RaidEffFront, 0});
    setActorPosition(front, -designW, designH - (visible.height + -640.0f));
    vcall(front, 0x48, {front, fbits(2.0f), fbits(2.0f)});
    cam = self->camera;
    addChildZ(cam, front, 300);
    if (self->cutInShader) call(GuiltBurstCutInShader_addRaidBossBg, {self->cutInShader, front});
}
RW_FUNCTION(0x609548, "SceneRaidBattle::loadMapComplete", SceneRaidBattle::loadMapComplete);

// ================================================================================================ player input

// The burst button: pay the gauge, target the boss, attack (the friend's covering fire for a friend medal).
void SceneRaidBattle::onBurstButton(SceneRaidBattle *self) {
    uint32_t a = addr(self);
    if (self->battleState != PLAYER_INPUT) return;
    uint32_t deck = medalField(self);
    uint32_t info = call(MedalDeck_getCurrentMedalInfo, {deck});
    uint32_t valid = call(MedalDeck_isValidCurrentBurst, {deck});
    int32_t gauge = int32_t(call(MedalDeck_getCurrentBurstGauge, {deck}));
    if (valid != 1) return;
    if (call(SceneBattleBase_subPlayerCP, {a, uint32_t(gauge) * 10000u, info}) != 1) return;
    st32(self->boss + BOSS_SPREAD, 0xFFFFFFFFu);
    st32(self->boss + BOSS_SPREAD + 4, 0xFFFFFFFFu);
    call(StageActorManager_autoChoiceTarget, {self->actorManager, info, 1});
    if (call(MedalDeck_isCurrentFriendMedal, {deck}) == 1) {
        call(StageActorManager_startFriendCoveringFire, {self->actorManager});
        execRaidAttackFriend(self, self->boss + BOSS_WHOLE_POINT, 1, 1);
    } else {
        execRaidAttack(self, self->boss + BOSS_WHOLE_POINT, 1, 1);
    }
    Frame f;
    uint32_t burst = f.alloc(8);
    call(Burst_create, {burst, uint32_t(bt::sbGet32(ld32(info), 0x4AC))});
    if (bt::sbGet32(ld32(burst), 0x160) == 3) {                // random targets
        SelectRandom(self, 0);
    } else if (bt::sbGet32(ld32(burst), 0x160) != 1) {         // not single: every part is hit
        for (uint32_t k = 0; k < 6; k++) st8(self->boss + k + BOSS_PART_TARGET, 1);
    }
    sharedRelease(burst);
}
RW_FUNCTION(0x609794, "SceneRaidBattle::onBurstButton", SceneRaidBattle::onBurstButton);

namespace {
// AttackTarget::Type of an attack: 1 single medal, 2 whole (burst, not "whole"), 3 burst
uint32_t attackType(uint32_t burst, uint32_t whole) { return burst == 1 ? (whole ? 3u : 2u) : 1u; }
}  // namespace

void SceneRaidBattle::execRaidAttackFriend(SceneRaidBattle *self, uint32_t target, uint32_t burst, uint32_t whole) {
    if (self->battleState != PLAYER_INPUT) return;
    self->battleState = PLAYER_ATTACK;
    self->deckEnabled = 0;
    self->markerActive = 0;
    call(RaidBattle_startFriend, {self->raidBattle, ld32(self->actorManager + ACTORS_FRIEND), self->boss, self->camera,
                                  addr(self), target, attackType(burst, whole)});
    call(CarouselDeck_actionAttack, {carouselDeck(self), whole});
}
RW_FUNCTION(0x609958, "SceneRaidBattle::execRaidAttackFriend", SceneRaidBattle::execRaidAttackFriend);

void SceneRaidBattle::execRaidAttack(SceneRaidBattle *self, uint32_t target, uint32_t burst, uint32_t whole) {
    if (self->battleState != PLAYER_INPUT) return;
    self->battleState = PLAYER_ATTACK;
    self->deckEnabled = 0;
    self->markerActive = 0;
    call(RaidBattle_start, {self->raidBattle, ld32(self->actorManager + ACTORS_PLAYER), self->boss, self->camera,
                            addr(self), target, attackType(burst, whole)});
    call(CarouselDeck_actionAttack, {carouselDeck(self), whole});
}
RW_FUNCTION(0x6099E4, "SceneRaidBattle::execRaidAttack", SceneRaidBattle::execRaidAttack);

// Random burst targets: each living part is picked with a 50% chance, at most one; none -> part 0.
void SceneRaidBattle::SelectRandom(SceneRaidBattle *self, uint32_t showMarkers) {
    uint32_t picked = 0;
    for (int32_t k = 0; k < 6; k++) {
        st8(self->boss + uint32_t(k) + BOSS_PART_TARGET, 0);
        uint32_t random = call(Random_getInstance, {});
        float roll = rw::callf(Random_generateUniformFloat, {random, 0, fbits(1.0f), 0});
        bool pick = false;
        if (!(roll >= 0.5f) && !std::isnan(roll)) {
            uint32_t hp = call(RaidBossActor_getCurrentHP, {self->boss, uint32_t(k)});
            pick = picked == 0 && hp != 0;
        }
        if (pick) {
            picked = 1;
            st8(self->boss + uint32_t(k) + BOSS_PART_TARGET, 1);
            if (showMarkers == 1) call(RaidBossActor_ShowTargetInex, {self->boss, uint32_t(k)});
        } else if (showMarkers == 1) {
            call(RaidBossActor_HideTargetInex, {self->boss, uint32_t(k)});
        }
    }
    if (!picked) {
        st8(self->boss + BOSS_PART_TARGET, 1);
        if (showMarkers == 1) call(RaidBossActor_ShowTargetInex, {self->boss, 0});
    }
}
RW_FUNCTION(0x609A70, "SceneRaidBattle::SelectRandom", SceneRaidBattle::SelectRandom);

uint32_t SceneRaidBattle::getCameraPosition(SceneRaidBattle *self) { return addr(self) + 0x2F8; }
RW_FUNCTION(0x609B44, "SceneRaidBattle::getCameraPosition", SceneRaidBattle::getCameraPosition);

void SceneRaidBattle::alterCamera(SceneRaidBattle *self, uint32_t action) {
    call(fn::Node_runAction, {self->camera, action});
}
RW_FUNCTION(0x609B4C, "SceneRaidBattle::alterCamera", SceneRaidBattle::alterCamera);

// The attack trigger timer ran out: during an attack count one more attack; at the input step attack the
// first marker automatically (unless a part is being destroyed).
void SceneRaidBattle::battleTrgUI(SceneRaidBattle *self) {
    int32_t state = self->battleState;
    if (state == PLAYER_ATTACK) {
        uint32_t battle = self->raidBattle;
        st32(battle + BATTLE_ATTACKS, ld32(battle + BATTLE_ATTACKS) + 1);
    } else if (state == PLAYER_INPUT) {
        uint32_t boss = self->boss;
        for (uint32_t node = ld32(boss + BOSS_PART_LIST); node; node = ld32(node))
            if (ld8(ld32(node + 0xC) + PART_BROKEN)) return;
        st32(boss + BOSS_SPREAD, 0);
        st32(self->boss + BOSS_SPREAD + 4, 0);
        Frame f;
        uint32_t marker = f.alloc(8);
        call(RaidBossActor_find_marker, {marker, self->boss, 0});
        execRaidAttack(self, marker, 0, 0);
        call(Vec2_dtor, {marker});
        st8(self->boss + BOSS_PART_TARGET, 1);
    }
}
RW_FUNCTION(0x609BC8, "SceneRaidBattle::battleTrgUI", SceneRaidBattle::battleTrgUI);

// ================================================================================================ the turn loop

// Per frame: the attack trigger timer, the boss markers, prizes dropped by the boss, the field, the cut-in
// shader, then one step of the battle state machine.
void SceneRaidBattle::update(SceneRaidBattle *self, float dt) {
    uint32_t a = addr(self);
    if (self->triggerMode == 1) {
        float left = self->triggerTimer;
        if (left > 0.0f && self->triggerRunning) {
            left = left - dt;
            self->triggerTimer = left;
            if (left <= 0.0f) {
                self->triggerFired = 1;
                self->field_2DF = 1;
                self->triggerTimer = 0.0f;
            }
        }
    }
    call(RaidBossActor_updateTargetPosition, {self->boss});
    call(BaseActorManager_update_burst_enable, {self->actorManager});
    if (self->triggerFired) {
        self->triggerFired = 0;
        self->field_2DE = 0;
        self->triggerTimer = 0.5f;
        battleTrgUI(self);
    }

    // prizes the boss dropped move onto the field once they burst out
    Frame f;
    uint32_t prizes = f.alloc(12);                              // cocos2d::Vector<MapPrizes*> (a copy)
    call(PrizeVector_copy, {prizes, self->boss + BOSS_PRIZES});
    for (uint32_t it = ld32(prizes), end = ld32(prizes + 4); it != end; it += 4) {
        uint32_t prize = ld32(it);
        if (call(MapPrizes_isEruption, {prize}) == 1) {
            call(StageFieldManager_addMapNodeToField, {self->fieldManager, prize});
            call(PrizeVector_eraseObject, {self->boss + BOSS_PRIZES, prize, 0});
        }
    }
    call(StageFieldManager_update_raid, {self->fieldManager});
    call(StageFieldManager_update, {self->fieldManager, fbits(dt), player(self)});
    if (self->cutInShader) {
        call(GuiltBurstCutInShader_updateInit, {self->cutInShader, fbits(dt)});
        uint32_t shader = self->cutInShader;
        vcall(shader, 0x1C4, {shader, fbits(dt)});           // update(dt)
    }

    switch (self->battleState) {
    case INTRO:                                               // wait, then the "battle start" direction
        if (self->waitFrames >= 1) {
            self->waitFrames = self->waitFrames - 1;
            break;
        }
        self->battleState = BOSS_APPEAR;
        call(CommonDirection_to_start, {self->commonDirection});
        break;

    case BOSS_APPEAR: {                                       // the boss enters, then the heading
        uint32_t rd = self->raidDirection;
        if (ld32(rd + 0x2BC) == ld32(rd + 0x2C8) && ld32(rd + 0x2C0) == ld32(rd + 0x2C4)) {
            uint32_t raidBoss = ld32(self->boss + BOSS_RAID_BOSS);
            if (ld32(raidBoss + FLASH_MOTION) != 4) {
                if (call(FlashNode_isPlaying, {raidBoss})) break;
                uint32_t boss = self->boss;
                call(RaidBoss_changeMotion, {ld32(boss + BOSS_RAID_BOSS), 4});
                call(RaidBossActor_makeWholeAttackPoint, {boss});
            }
            uint32_t addChildFn = ld32(ld32(a) + node::addChildZ);
            call(addChildFn, {a, call(StageUI_createHeading, {self->stageUI}), 90});
            call(StageUI_showHeading, {self->stageUI, 0});
            self->battleState = HEADING;
        } else if (!call(FlashNode_isPlaying, {ld32(self->boss + BOSS_RAID_BOSS)})) {
            uint32_t boss = self->boss;
            call(RaidBoss_changeMotion, {ld32(boss + BOSS_RAID_BOSS), 4});
            call(RaidBossActor_makeWholeAttackPoint, {boss});
        }
        break;
    }

    case HEADING:                                             // heading done: the battle UI (or the tutorial)
        if (call(StageUI_isPlayingHeading, {self->stageUI})) break;
        if (!self->isTutorial) {
            call(RaidBossActor_marker_mode_chamge, {self->boss});
            self->markerActive = 1;
            call(StageActorManager_attackReadyAction, {self->actorManager, self->boss});
            RaidBattle_Start(self);
            self->waitFrames = 60;
            self->battleState = START_WAIT;
        } else {
            self->battleState = TUTORIAL;
            Frame g;
            uint32_t closed = newFunction(g, VT_update_19, {a});
            uint32_t popup = g.alloc(20);                     // the returned std::function (sret)
            call(SceneUtil_openTutorialPopup, {popup, 11, closed, 0, 0, 0, 0});
            functionDestroy(popup);
            functionDestroy(closed);
        }
        break;

    case START_WAIT:
        if (self->waitFrames >= 1) {
            self->waitFrames = self->waitFrames - 1;
            break;
        }
        self->deckEnabled = 0;
        self->battleState = PLAYER_TURN;
        call(StageUI_setBattleTurn, {self->stageUI, 1});
        call(SceneBattleBase_invokeSkill, {a, 3});
        call(SceneBattleBase_invokeSkill, {a, 4});
        call(RaidBossActor_updateDamageAll, {self->boss});
        break;

    case ENEMY_DONE:
        self->deckEnabled = 0;
        self->battleState = PLAYER_TURN;
        break;

    case PLAYER_TURN:                                         // poison on the player, or the boss attacks
        if (call(StageUI_isPlayingHeading, {self->stageUI})) break;
        if (int32_t(ld32(self->raidBattle + BATTLE_STEP)) <= 11) {
            self->deckEnabled = 1;
            self->battleState = PLAYER_POISON;
            if (call(StageState_setPlayableActorPoisonEffect, {stageState(), player(self), 1}) == 1) {
                uint32_t status = call(StageUI_getPlayerStatusField, {self->stageUI});
                call(PlayerStatusIndicator_setHP, {status, uint32_t(stageHP())});
            }
        } else {
            self->battleState = ENEMY_TURN;
            call(RaidBattle_setRaidEnemyPoisonEffect, {self->raidBattle});
        }
        break;

    case PLAYER_POISON: {                                     // the poison motion ends: the player's turn
        uint32_t avatar = ld32(player(self) + ACTOR_AVATAR);
        if (ld32(avatar + FLASH_MOTION) != 11) {
            if (call(FlashNode_isPlaying, {avatar})) break;
            call(Avatar_changeMotion, {ld32(player(self) + ACTOR_AVATAR), 11});
        }
        call(IStatusProtocol_elapseTurnStatusPoison, {stageState() + 0x20});
        call(AvatarActor_UpdateStatus, {player(self)});
        endEnemyTurn(self);
        break;
    }

    case ENEMY_TURN:                                          // the boss attacks (when alive)
        if (call(RaidBossActor_getCurrentHP, {self->boss, 0}) == 0) {
            self->battleState = CLEAR;
            break;
        }
        {
            uint32_t playing = call(FlashNode_isPlaying, {ld32(self->boss + BOSS_RAID_BOSS)});
            uint32_t boss = self->boss;
            if (playing == 1 && ld32(ld32(boss + BOSS_RAID_BOSS) + FLASH_MOTION) != 4) break;
            call(IStatusProtocol_elapseTurnStatusPoison, {boss + BOSS_STATUS});
            call(RaidBossActor_UpdateStatus, {self->boss});
            self->battleState = PLAYER_ATTACK;
            call(RaidBattle_startEnemy, {self->raidBattle});
        }
        break;

    case PLAYER_ATTACK: {                                     // an attack sequence ended
        if (ld32(self->raidBattle + BATTLE_STEP) != 0) break;
        if (stageHP() != 0) {
            self->battleState = PLAYER_TURN;
            call(StageUI_setBattleTurn, {self->stageUI, 1});
        } else if (bt::sbGet32(stageState(), 0x4244) == 1) {  // knocked out with a revive left
            vcall(a, VSLOT_PLAY_SE, {a, SE_REVIVE});
            rw::host_ptr<SafeBuffer>(stageState())->set32(0x4244, 2);
            call(CommonDirection_heal, {self->commonDirection});
            int32_t hp = int32_t(call(StageState_addDamage, {stageState(), 0xFFFFFFFFu}));
            call(PlayerStatusIndicator_setHP, {call(StageUI_getPlayerStatusField, {self->stageUI}), uint32_t(hp)});
            self->battleState = PLAYER_TURN;
            call(StageUI_setBattleTurn, {self->stageUI, 1});
            uint32_t actor = ld32(self->raidBattle + BATTLE_ACTOR);
            call(Avatar_changeMotion, {ld32(actor + ACTOR_AVATAR), 43});
        } else {
            self->battleState = LOSE;
        }
        break;
    }

    case CLEAR:
    case LOSE: {                                              // send the raid result
        bool cleared = self->battleState == CLEAR;
        Frame g;
        uint32_t done = newEmptyFunction(g);
        executeRaidBattle(self, done);
        functionDestroy(done);
        self->battleState = RESULT_WAIT;
        self->waitFrames = cleared ? 180 : 30;
        break;
    }

    case RESULT_WAIT:
        if (self->waitFrames >= 1) {
            self->waitFrames = self->waitFrames - 1;
            break;
        }
        rw::save<float>(scheduler() + SCHEDULER_TIME_SCALE, self->savedTimeScale);
        {
            uint32_t node = ld32(self->commonDirection + CDIR_NODE_B);
            vcall(node, node::setVisible, {node, 0});
        }
        if (self->isTutorial) {                               // the tutorial ends with a movie
            self->battleState = TUTORIAL;
            call(RaidDirection_finalWhiteIn, {self->raidDirection});
            uint32_t direction = self->commonDirection;
            uint32_t action = callFuncLambda(VT_update_20, a);
            call(CommonDirection_black_out, {direction, action});
        } else {                                              // to the battle result
            call(Node_unscheduleUpdate, {a});
            uint32_t mgr = sceneManager();
            Frame g;
            uint32_t factory = newFunction(g, VT_LayerFactory, {createSceneLayer_SceneBattleResult});
            call(SceneManager_changeSceneLayer, {mgr, factory, 1, 1, 1});
            functionDestroy(factory);
            call(SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        }
        break;

    case CONTINUE:                                            // revived: wait for the motion
        if (!call(FlashNode_isPlaying, {ld32(player(self) + ACTOR_AVATAR)})) self->battleState = PLAYER_ATTACK;
        break;

    default:
        break;
    }
    call(PrizeVector_dtor, {prizes});
}
RW_FUNCTION(0x609CA8, "SceneRaidBattle::update", SceneRaidBattle::update);

// The boss's turn ended: next player turn, or the continue offer when knocked out with the revive pending.
void SceneRaidBattle::endEnemyTurn(SceneRaidBattle *self) {
    if (endPlayerCount(self)) return;
    int32_t hp = stageHP();
    int32_t revive = bt::sbGet32(stageState(), 0x4244);
    if (hp == 0 && revive == 1) self->battleState = CONTINUE;
}
RW_FUNCTION(0x60A610, "SceneRaidBattle::endEnemyTurn", SceneRaidBattle::endEnemyTurn);

void SceneRaidBattle::startEnemyTurn(SceneRaidBattle *self) {
    self->battleState = PLAYER_ATTACK;
    call(RaidBattle_startEnemy, {self->raidBattle});
}
RW_FUNCTION(0x60A6AC, "SceneRaidBattle::startEnemyTurn", SceneRaidBattle::startEnemyTurn);

// The raid is over: the total damage goes to the stage state, then the raid-clear request (the tutorial calls
// `done` directly).
bool SceneRaidBattle::executeRaidBattle(SceneRaidBattle *self, uint32_t done) {
    uint32_t state = stageState();
    int32_t damage = int32_t(call(RaidBossActor_calc_total_attack_damage, {self->boss}));
    rw::host_ptr<SafeBuffer>(state)->set32(0x30, damage);
    if (self->isTutorial) {
        if (ld32(done + 16)) call(Function_vb_call, {done, 1});
        return true;
    }
    uint32_t mgr = sceneManager();
    st8(mgr + SCENEMGR_RAID_RESULT, 1);
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(Function_vb_assign, {mgr + SCENEMGR_RAID_DONE, empty});
        functionDestroy(empty);
    }
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t parts = call(RaidBossActor_getNumberOfParts, {self->boss});
    call(APIManager_executeRaidClear, {api, parts, self->boss + BOSS_PARTS, done});
    return true;
}
RW_FUNCTION(0x60A6C4, "SceneRaidBattle::executeRaidBattle", SceneRaidBattle::executeRaidBattle);

void SceneRaidBattle::removeEnemyMarker(SceneRaidBattle *self) { (void)self; }
RW_FUNCTION(0x60A7D8, "SceneRaidBattle::removeEnemyMarker", SceneRaidBattle::removeEnemyMarker);

// The player's attacks are done: the boss acts next (or the raid is cleared).
void SceneRaidBattle::endPlayerTurn(SceneRaidBattle *self) {
    call(RaidBossActor_HideTarget, {self->boss});
    self->deckEnabled = 1;
    self->markerActive = 0;
    if (call(RaidBossActor_getCurrentHP, {self->boss, 0}) != 0) {
        self->battleState = ENEMY_DONE;
        call(StageActorManager_attackReadyAction, {self->actorManager, self->boss});
        call(StageUI_setBattleTurn, {self->stageUI, 0});
    } else {
        self->battleState = CLEAR;
    }
}
RW_FUNCTION(0x60A7DC, "SceneRaidBattle::endPlayerTurn", SceneRaidBattle::endPlayerTurn);

// A new player turn when alive (or with the revive pending): markers on, deck advanced. Returns true when the
// player is out (state LOSE).
bool SceneRaidBattle::endPlayerCount(SceneRaidBattle *self) {
    if (stageHP() == 0 && bt::sbGet32(stageState(), 0x4244) != 1) {
        self->battleState = LOSE;
        return true;
    }
    call(RaidBossActor_UpdateStatus, {self->boss});
    call(FlashNode_reflection_status, {ld32(self->boss + BOSS_RAID_BOSS), 0});
    call(RaidBossActor_ShowTarget, {self->boss, 0});
    if (ld32(medalField(self) + MEDALDECK_STATE) == 2) {
        call(MedalDeck_advanceState, {medalField(self)});
        call(CarouselDeck_advanceDeck, {carouselDeck(self)});
    }
    st32(self->boss + BOSS_SPREAD, 0xFFFFFFFFu);
    st32(self->boss + BOSS_SPREAD + 4, 0xFFFFFFFFu);
    self->deckEnabled = 1;
    self->markerActive = 1;
    call(CarouselDeck_setPossibleBurst, {carouselDeck(self), 1, 0});
    call(StageActorManager_attackReadyAction, {self->actorManager, self->boss});
    self->battleState = PLAYER_INPUT;
    return false;
}
RW_FUNCTION(0x60A838, "SceneRaidBattle::endPlayerCount", SceneRaidBattle::endPlayerCount);

// The boss hit the player: HP down, the condition of type 1 cleared, the avatar's status effect shown.
void SceneRaidBattle::setPlayerDamage(SceneRaidBattle *self, int32_t damage) {
    uint32_t status = call(StageUI_getPlayerStatusField, {self->stageUI});
    uint32_t hp = call(StageState_addDamage, {stageState(), uint32_t(damage)});
    call(PlayerStatusIndicator_setHP, {status, hp});
    uint32_t app = appState();
    uint32_t state = ld32(app + APP_STAGE_STATE);
    uint32_t ctrl = ld32(app + APP_STAGE_STATE + 4);
    if (ctrl) call(fn::shared_add, {ctrl});
    if (state) {
        uint32_t proto = state + 0x20;                        // IStatusProtocol
        uint32_t getCondition = ld32(ld32(proto));
        uint32_t setCondition = ld32(ld32(proto) + 4);
        Frame f;
        uint32_t change = f.alloc(12);                        // StatusChange
        call(getCondition, {change, proto, 1});
        uint32_t cleared = call(StatusChange_clear, {change});
        call(setCondition, {proto, 1, cleared});
    }
    uint32_t actor = ld32(self->raidBattle + BATTLE_ACTOR);
    call(FlashNode_reflection_status, {ld32(actor + ACTOR_AVATAR), 1});
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x60A96C, "SceneRaidBattle::setPlayerDamage", SceneRaidBattle::setPlayerDamage);

int32_t SceneRaidBattle::getEnemyHp(SceneRaidBattle *self) {
    return int32_t(call(RaidBossActor_getEnemyHp, {self->boss}));
}
RW_FUNCTION(0x60AA54, "SceneRaidBattle::getEnemyHp", SceneRaidBattle::getEnemyHp);

bool SceneRaidBattle::advanceBattleSequence(SceneRaidBattle *self) {
    uint32_t deck = medalField(self);
    call(MedalDeck_advanceState, {deck});
    call(CarouselDeck_advanceDeck, {carouselDeck(self)});
    call(StageActorManager_attackReadyAction, {self->actorManager, self->boss});
    return ld32(deck + MEDALDECK_STATE) == 2;
}
RW_FUNCTION(0x60AA62, "SceneRaidBattle::advanceBattleSequence", SceneRaidBattle::advanceBattleSequence);

void SceneRaidBattle::enemy_state_update_turn_attack(SceneRaidBattle *self) {
    call(IStatusProtocol_elapseTurnStatus, {self->boss + BOSS_STATUS});
    call(IBuffProtocol_elapseTurnAttackBuff, {self->boss + BOSS_BUFF});
    call(RaidBossActor_UpdateStatus, {self->boss});
}
RW_FUNCTION(0x60AAA8, "SceneRaidBattle::enemy_state_update_turn_attack",
            SceneRaidBattle::enemy_state_update_turn_attack);

// The defence buffs and every part's turn counters run down.
void SceneRaidBattle::enemy_state_update_turn_defense(SceneRaidBattle *self) {
    call(IBuffProtocol_elapseTurnDefenseBuff, {self->boss + BOSS_BUFF});
    Frame f;
    uint32_t raidEnemy = f.alloc(8);                          // shared_ptr<master::RaidEnemy>: the boss
    call(RaidEnemy_create, {raidEnemy, ld32(self->boss + BOSS_RAID_ENEMY_ID)});
    int32_t count = bt::sbGet32(ld32(raidEnemy), 0x94);       // parts
    uint32_t partIds = f.alloc(0x14);                         // int[5] part RaidEnemy ids
    uint32_t part = f.alloc(8);
    for (int32_t k = 0; k < count; k++) {
        const SafeBuffer *row = rw::host_ptr<SafeBuffer>(ld32(raidEnemy));
        SafeBuffer::read(row, rw::host_ptr<uint8_t>(partIds), row->data.get() + 0x98, 0x14, 0x98);
        call(RaidEnemy_create, {part, ld32(partIds + 4 * uint32_t(k))});
        int32_t index = bt::sbGet32(ld32(part), 0x90);
        call(RaidBossActor_elapsePartsTurn, {self->boss, uint32_t(index - 1)});
        sharedRelease(part);
    }
    call(RaidBossActor_UpdateStatus, {self->boss});
    sharedRelease(raidEnemy);
}
RW_FUNCTION(0x60AAD4, "SceneRaidBattle::enemy_state_update_turn_defense",
            SceneRaidBattle::enemy_state_update_turn_defense);

void SceneRaidBattle::req_skilPanel_in(SceneRaidBattle *self, int32_t skillId) {
    call(CommonDirection_req_skilPanel_in, {self->commonDirection, uint32_t(skillId)});
}
RW_FUNCTION(0x60ABDC, "SceneRaidBattle::req_skilPanel_in", SceneRaidBattle::req_skilPanel_in);

void SceneRaidBattle::req_materialPanel_in(SceneRaidBattle *self, int32_t materialId) {
    call(CommonDirection_req_materialPanel_in, {self->commonDirection, uint32_t(materialId)});
}
RW_FUNCTION(0x60ABEA, "SceneRaidBattle::req_materialPanel_in", SceneRaidBattle::req_materialPanel_in);
