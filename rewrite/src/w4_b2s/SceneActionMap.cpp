// SceneActionMap::init and SceneActionMap::updateFieldMode (the quest scene, see scene_main/SceneActionMap.hpp).
//
// init builds the stage managers, wires the actor / UI callbacks (std::function members assigned from lambdas that
// capture `this`), lays out the UI, puts the keyblade's medals on the deck and registers the touch listener.
// It keeps a 0x3E8 byte frame in which one variable is never written (the second argument of firstSetPlayerCP is a
// stale stack word); the frame is reproduced with the original offsets.
#include "B2sCommon.hpp"

using namespace b2s;

namespace {
// ---- guest functions: init
constexpr Addr SceneBattleBase_init = 0x5FA135;
constexpr Addr SceneBattleBase_firstSetPlayerCP = 0x5FAF05;
constexpr Addr AppState_createStageState = 0x4EA861;
constexpr Addr StageManager_create = 0x5FC7E1;
constexpr Addr StageBattleManager_create = 0x5FC831;
constexpr Addr StageExploreManager_create = 0x5FC87F;
constexpr Addr StageDirection_create = 0x5FC8CD;
constexpr Addr CommonDirection_create = 0x5FC921;
constexpr Addr AimTarget_create = 0x5FC975;
constexpr Addr CocosVector_pushBack = 0x5E29A9;        // cocos2d::Vector<Ref*>::pushBack
constexpr Addr StageDirection_preloadResources = 0x767B79;
constexpr Addr CommonDirection_preloadResources = 0x764C1D;
constexpr Addr StageDirection_exec_stage_title = 0x767C79;
constexpr Addr Stage_create = 0x78E4E9;
constexpr Addr Keyblade_create = 0x783F9D;
constexpr Addr audio_Manager_getInstance = 0x5BECFD;
constexpr Addr audio_Manager_load = 0x5BEEF5;
constexpr Addr AudioGetGameMainBGMResourceID = 0x5C1A65;
constexpr Addr AudioReplaceGameMainBGM = 0x5C0FA1;
constexpr Addr AvatarUtil_getCurrentStyle = 0x4977AD;
constexpr Addr AvatarUtil_getSupportUserStyle = 0x49794D;
constexpr Addr AvatarUtil_getCurrentKeyblade = 0x4976FD;
constexpr Addr BattleUtil_getSupportMedal = 0x497011;
constexpr Addr StageManager_getAvatarActorStylePlayable = 0x75BBE9;
constexpr Addr StageManager_getAvatarActorStyleFriend = 0x75BBEF;
constexpr Addr StageManager_getStageBossEnemyID = 0x75BBE1;
constexpr Addr StageManager_getStageEnemyPlatoonCount = 0x75BBE5;
constexpr Addr BaseActorManager_setupAvatarActorPlayable = 0x74E7C5;
constexpr Addr BaseActorManager_setupAvatarActorFriend = 0x74E81B;
constexpr Addr StageUI_getMedalField = 0x75E65D;
constexpr Addr StageUI_getCarouselDeck = 0x75E661;
constexpr Addr StageUI_getStatusField = 0x75E669;
constexpr Addr StageUI_getAttributeField = 0x75E66D;
constexpr Addr StageUI_getPlayerStatusField = 0x75E671;
constexpr Addr StageUI_getLuxTargetField = 0x75E675;
constexpr Addr StageUI_createCarouselDeck = 0x75E929;
constexpr Addr StageUI_createStatusField = 0x75EA45;
constexpr Addr StageUI_createAttributeField = 0x75EF25;
constexpr Addr StageUI_createPlayerStatusField = 0x75F001;
constexpr Addr StageUI_createLuxTargetField = 0x75F0B5;
constexpr Addr StageUI_createHeading = 0x75F355;
constexpr Addr CarouselDeck_changeDeckStyle = 0x5CF521;
constexpr Addr CarouselDeck_addMedalOntoDeck = 0x5CF80D;
constexpr Addr CarouselDeck_addFriendMedalOntoDeck = 0x5D073D;
constexpr Addr CarouselDeck_setDeckInfoWindow = 0x5D07D1;
constexpr Addr CarouselDeck_reflashDeck = 0x5D0BC5;
constexpr Addr MedalDeck_addMedalOntoDeck = 0x5DFB51;
constexpr Addr MedalDeck_addFriendMedalOntoDeck = 0x5DFE6B;
constexpr Addr SceneActionMap_setNewMap = 0x5FCACD;
constexpr Addr SceneUtil_createTopInAnimation = 0x45B03D;
constexpr Addr SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr Addr SafeBuffer_write = 0x459FA1;
constexpr Addr SafeBuffer_dtor = 0x45A13D;
constexpr Addr Director_getInstance = 0x8EE519;
constexpr Addr ListenerOneByOne_create = 0x8FACA5;
constexpr Addr EventDispatcher_addListenerSceneGraph = 0x8F3649;
constexpr Addr Widget_addTouchEventListener = 0x826CCF;
constexpr Addr EaseSineOut_create = 0x860D09;
constexpr Addr FunctionBool_swap = 0x47D0F1;             // std::function<bool()>::swap
constexpr Addr FunctionGesture_swap = 0x606759;          // std::function<void(GestureRecognizer*, const Vec2&, unsigned)>::swap
constexpr Addr FunctionVoid_assign = 0x4EB9CD;           // std::function<void()>::operator=
constexpr Addr FunctionVoidBool_assign = 0x490109;       // std::function<void(bool)>::operator=
constexpr Addr FunctionBool_assign = 0x53327D;           // std::function<bool()>::operator=
constexpr Addr FunctionBoolU_assign = 0x607781;          // std::function<bool(unsigned)>::operator=
constexpr Addr FunctionBoolVec2_assign = 0x607921;       // std::function<bool(const Vec2&, const Vec2&)>::operator=
constexpr Addr FunctionBoolFF_assign = 0x607AC1;         // std::function<bool(float, float)>::operator=
constexpr Addr FunctionVoidVec2_assign = 0x607441;       // std::function<void(Vec2&)>::operator=
constexpr Addr FunctionVoidFn_assign = 0x6075E1;         // std::function<void(std::function<void(bool)>)>::operator=
constexpr Addr FunctionTouchBegan_assignBind = 0x5FC9C5; // std::function<bool(Touch*, Event*)>::operator=(bind)
constexpr Addr FunctionTouch_assignBind = 0x5FCA49;      // std::function<void(Touch*, Event*)>::operator=(bind)
constexpr Addr SceneManager_getInstance = 0x60C26D;

// ---- guest functions: updateFieldMode
constexpr Addr StageManager_checkMapTransportGate = 0x75C3D1;
constexpr Addr SceneActionMap_toLoadingModeSetup = 0x601141;
constexpr Addr SceneActionMap_toBattleModeSetup = 0x601291;
constexpr Addr SceneActionMap_updateAimTarget = 0x601075;     // (Vec2* out, this)
constexpr Addr SceneActionMap_updateCameraPosition = 0x5FFD39;
constexpr Addr AvatarActor_setActionJump = 0x740705;
constexpr Addr AvatarActor_lookAt = 0x741651;
constexpr Addr MapNode_updateMapPosition = 0x5DA461;
constexpr Addr Avatar_changeMotion = 0x5C63C5;
constexpr Addr AimTarget_reset = 0x73F5B5;
constexpr Addr AimTarget_stop = 0x73F7E1;
constexpr Addr AimTarget_updateMarker = 0x73F805;
constexpr Addr StageFieldManager_checkEventOccurs = 0x756FD5;
constexpr Addr StageFieldManager_fitRectToPoint = 0x756633;
constexpr Addr StageFieldManager_isCollisionDetection = 0x7569ED;
constexpr Addr StageFieldManager_isCollisionSafe = 0x756981;
constexpr Addr StageFieldManager_hasJumpData = 0x756AD5;
constexpr Addr StageExploreManager_searchMovablePosition = 0x754A65;
constexpr Addr StageActorManager_update = 0x74FF25;
constexpr Addr StageActorManager_checkEncount = 0x74F1A5;
constexpr Addr FunctionVoid_call = 0x470E69;
constexpr Addr Vec2_copy = 0x8E4C03;
constexpr Addr Vec2_dtor = 0x8E4C17;
constexpr Addr Vec2_distance = 0x8E4EF5;
constexpr Addr Vec2_subtract = 0x8E530F;
constexpr Addr fcmpeq = 0xCD71A4, fcmplt = 0xCD71B8, fcmple = 0xCD71CC, fcmpgt = 0xCD71F4;
constexpr Addr unordsf2 = 0xCD7208;
constexpr Addr divsf3 = 0xCD6FAC;

constexpr Addr STR_MenuButton = 0xE02E00;               // "MenuButton" (10)

// lambda __func vtables created by init (each captures `this`)
constexpr Addr VT_G1 = 0xF5EB48, VT_G2 = 0xF5EB1C, VT_G3 = 0xF5EAF0, VT_G4 = 0xF5EAC4, VT_G5 = 0xF5EA98,
               VT_G6 = 0xF5EA6C, VT_G7 = 0xF5EA40;
constexpr Addr VT_titleCb0 = 0xF5EA14, VT_titleCb1 = 0xF5E9BC;       // stage title callback (no captures)
constexpr Addr VT_swapFn0 = 0xF5E9E8, VT_swapFn1 = 0xF5E990;
constexpr Addr VT_swipe = 0xF5E964, VT_onEnter = 0xF5E938, VT_onExit = 0xF5E90C;
constexpr Addr VT_carousel0 = 0xF5E8E0, VT_carousel1 = 0xF5E8B4, VT_carousel2 = 0xF5E888, VT_aimMarker = 0xF5E85C;
constexpr Addr VT_deckMedal = 0xF5E830, VT_deckFriendMedal = 0xF5E804, VT_menuButton = 0xF5E7D8,
               VT_backKey = 0xF5E7AC;

// frame offsets of init
constexpr uint32_t I_FRAME = 0x3E8;
constexpr uint32_t I_STAGE_SP = 0x110;                  // shared_ptr<master::Stage>: object, control block
constexpr uint32_t I_KEYBLADE_SP = 0x94;                // shared_ptr<master::Keyblade>
constexpr uint32_t I_PLAYER_STYLE = 0xF0;               // Avatar::Style (SafeBuffer inside)
constexpr uint32_t I_FRIEND_STYLE = 0xD0;
constexpr uint32_t I_USER_KEYBLADE = 0x1F0;             // UserKeyblade::Keyblade
constexpr uint32_t I_STALE = 0x230;                     // never written: firstSetPlayerCP's argument

// a function object of the original frame holding a lambda that captures `this` (or nothing): vtable, this, and
// the callable slot pointing at itself (the buffer is the callable)
void makeLambda(Addr fn, Addr vtable, Addr capture, bool hasCapture = true) {
    st32(fn + 16, fn);
    st32(fn, vtable);
    if (hasCapture) st32(fn + 4, capture);
}
}  // namespace


// ---------------------------------------------------------------------------------------------------------------
uint32_t SceneActionMap_init(uint32_t self) {
    guest::Frame F(I_FRAME);
    const Addr me = self;
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto sm = [&]() { return cx(SceneManager_getInstance, {}); };
    auto actorManager = [&]() { return ld32(me + 0x2BC); };
    auto avatarActor = [&]() { return ld32(ld32(me + 0x2BC) + 0x20); };
    // target = lambda(this): the std::function object lives in the frame and is destroyed after the assignment
    auto assignLambda = [&](Addr target, Addr fnObj, Addr assign, Addr vtable) {
        makeLambda(fnObj, vtable, me);
        cx(assign, {target, fnObj});
        fnDestroy(fnObj);
    };

    if (cx(SceneBattleBase_init, {me}) != 1) return 0;
    cx(AppState_createStageState, {cx(APPSTATE_GETINSTANCE, {}), 0});

    // ---- the stage managers
    st32(me + 0x320, cx(StageManager_create, {}));
    st32(me + 0x324, cx(StageBattleManager_create, {}));
    st32(me + 0x328, cx(StageExploreManager_create, {}));
    st32(me + 0x32C, cx(StageDirection_create, {}));
    st32(me + 0x330, cx(CommonDirection_create, {}));
    st32(ld32(me + 0x330) + 0x384, ld32(me + 0x2C0));                  // CommonDirection: the field manager
    st32(actorManager() + 0x98, ld32(me + 0x330));                      // the actor manager: CommonDirection, scene
    st32(actorManager() + 0x9C, me);
    st32(me + 0x334, cx(AimTarget_create, {}));
    for (uint32_t off : {0x320u, 0x324u, 0x328u, 0x32Cu, 0x330u, 0x334u}) {      // the Vector<Ref*> at +0x2C4 retains them
        cx(CocosVector_pushBack, {me + 0x2C4, ld32(me + off)});
    }
    st32(ld32(me + 0x32C) + 0x274, ld32(ld32(me + 0x320) + 0x14));
    cx(StageDirection_preloadResources, {ld32(me + 0x32C)});
    st32(ld32(me + 0x330) + 0x278, ld32(ld32(me + 0x320) + 0x14));
    cx(CommonDirection_preloadResources, {ld32(me + 0x330)});

    // ---- the stage row and its three BGM ids
    {
        const Addr api = cx(APIMANAGER_GETINSTANCE, {});
        cx(SAFEBUFFER_READ, {api + 0x33C, fa(0x3B0), ld32(api + 0x348) + 4, 4, 4});      // stage id
    }
    cx(Stage_create, {fa(I_STAGE_SP), ld32(fa(0x3B0))});
    {
        const Addr row = ld32(fa(I_STAGE_SP));
        cx(SAFEBUFFER_READ, {row, fa(0x118), ld32(row + 0xC) + 0x3F0, 4, 0x3F0});
        st32(me + 0x2E0, ld32(fa(0x118)));
        cx(SAFEBUFFER_READ, {row, fa(0x11C), ld32(row + 0xC) + 0x3F4, 4, 0x3F4});
        st32(me + 0x2E4, ld32(fa(0x11C)));
        cx(SAFEBUFFER_READ, {row, fa(0x120), ld32(row + 0xC) + 0x3F8, 4, 0x3F8});
        st32(me + 0x2E8, ld32(fa(0x120)));
    }
    for (uint32_t k = 0; k < 3; k++) {
        const uint32_t bgm = ld32(me + 0x2E0 + 4 * k);
        cx(audio_Manager_load, {cx(audio_Manager_getInstance, {}), bgm});
    }
    st32(actorManager() + 0x30, ld32(ld32(me + 0x320) + 0x14));

    // ---- the avatar styles
    cx(AvatarUtil_getCurrentStyle, {fa(I_PLAYER_STYLE), 0});
    {
        const Addr am = actorManager();
        const Addr style = cx(StageManager_getAvatarActorStylePlayable, {ld32(me + 0x320), fa(I_PLAYER_STYLE)});
        cx(BaseActorManager_setupAvatarActorPlayable, {am, style});
    }
    cx(AvatarUtil_getSupportUserStyle, {fa(I_FRIEND_STYLE)});
    {
        const Addr am = actorManager();
        const Addr style = cx(StageManager_getAvatarActorStyleFriend, {ld32(me + 0x320), fa(I_FRIEND_STYLE)});
        cx(BaseActorManager_setupAvatarActorFriend, {am, style});
    }

    // ---- actor callbacks (lambdas capturing this)
    assignLambda(actorManager() + 0x190, fa(0x398), FunctionVoidBool_assign, VT_G1);
    assignLambda(avatarActor() + 0x2B0, fa(0x380), FunctionBoolFF_assign, VT_G2);
    assignLambda(avatarActor() + 0x2C8, fa(0x368), FunctionBoolVec2_assign, VT_G3);
    assignLambda(avatarActor() + 0x2E0, fa(0x350), FunctionBoolU_assign, VT_G4);
    assignLambda(avatarActor() + 0x2F8, fa(0x338), FunctionBoolU_assign, VT_G5);
    assignLambda(actorManager() + 0x158, fa(0x320), FunctionBool_assign, VT_G6);
    assignLambda(actorManager() + 0x170, fa(0x308), FunctionVoid_assign, VT_G7);

    // ---- the number of enemy platoons goes into the stage state
    {
        const Addr state = ld32(cx(APPSTATE_GETINSTANCE, {}) + 0x20);
        st32(fa(0xCC), cx(StageManager_getStageEnemyPlatoonCount, {ld32(me + 0x320)}));
        cx(SafeBuffer_write, {state, ld32(state + 0xC) + 0x45BC, fa(0xCC), 4, 0x45BC});
    }
    st32(avatarActor() + 0x290, ld32(me + 0x330));

    // ---- the stage title: kind 0 shows it with the boss id, kind 1 without; other kinds skip it
    {
        const uint32_t kind = ld32(ld32(me + 0x320) + 0x14);
        if (kind == 0 || kind == 1) {
            uint32_t titleArg = 0;
            if (kind == 0) titleArg = cx(StageManager_getStageBossEnemyID, {ld32(me + 0x320)});
            cx(StageDirection_exec_stage_title, {ld32(me + 0x32C), titleArg});
            st32(ld32(me + 0x320) + 0x18, 4);                                  // StageManager mode 4: stage title
            const Addr titleFn = fa(kind == 0 ? 0x2F0 : 0x2D8);
            makeLambda(titleFn, kind == 0 ? VT_titleCb0 : VT_titleCb1, 0, false);
            cx(FunctionVoidFn_assign, {ld32(me + 0x2B8) + 0x78, titleFn});
            fnDestroy(titleFn);
            const Addr swapFn = fa(0x3B0);
            makeLambda(swapFn, kind == 0 ? VT_swapFn0 : VT_swapFn1, me);
            cx(FunctionBool_swap, {swapFn, me + 0x340});
            fnDestroy(swapFn);
        }
    }
    vcx(me, slot::addChildZ, {me, ld32(me + 0x32C), 0xC});
    vcx(me, slot::addChildZ, {me, ld32(me + 0x330), 0x11});
    vcx(me, slot::addChildZ, {me, ld32(ld32(me + 0x330) + 0x270), 0xB});
    vcx(me, slot::addChildZ, {me, ld32(ld32(me + 0x330) + 0x274), 0xA});

    // ---- touch listener: began / moved / ended / cancelled are bound virtual slots of the scene (pmf {offset, 1})
    const Addr dispatcher = ld32(cx(Director_getInstance, {}) + 0x84);
    const Addr listener = cx(ListenerOneByOne_create, {});
    {
        auto bindPmf = [&](Addr bind, uint32_t vtableOffset) {
            st32(bind, vtableOffset);
            st32(bind + 4, 1);
            st32(bind + 8, me);
        };
        bindPmf(fa(0xC0), 0x290);
        cx(FunctionTouchBegan_assignBind, {listener + 0x50, fa(0xC0)});
        bindPmf(fa(0xB4), 0x294);
        cx(FunctionTouch_assignBind, {listener + 0x68, fa(0xB4)});
        bindPmf(fa(0xA8), 0x298);
        cx(FunctionTouch_assignBind, {listener + 0x80, fa(0xA8)});
        bindPmf(fa(0x9C), 0x29C);
        cx(FunctionTouch_assignBind, {listener + 0x98, fa(0x9C)});
    }
    cx(EventDispatcher_addListenerSceneGraph, {dispatcher, listener, me});

    // ---- swipe gesture, enter / exit callbacks
    {
        const Addr swipe = fa(0x3B0);
        makeLambda(swipe, VT_swipe, me);
        cx(FunctionGesture_swap, {swipe, me + 0x308});                 // GestureRecognizer (at +0x2F0) onSwipe
        fnDestroy(swipe);
    }
    assignLambda(me + 0x210, fa(0x2C0), FunctionVoid_assign, VT_onEnter);
    assignLambda(me + 0x228, fa(0x2A8), FunctionVoid_assign, VT_onExit);

    // ---- the UI
    const Addr stageUIField = me + 0x2B8;
    cx(StageUI_createCarouselDeck, {ld32(stageUIField)});
    cx(StageUI_createStatusField, {ld32(stageUIField), 0});
    cx(StageUI_createAttributeField, {ld32(stageUIField)});
    cx(StageUI_createLuxTargetField, {ld32(stageUIField)});
    {
        const Addr addChildZ = ld32(ld32(me) + slot::addChildZ);
        const Addr heading = cx(StageUI_createHeading, {ld32(stageUIField)});
        cx(addChildZ, {me, heading, 0xA});
    }
    vcx(me, slot::addChildZ, {me, cx(StageUI_getCarouselDeck, {ld32(stageUIField)}), 9});
    vcx(me, slot::addChildZ, {me, cx(StageUI_getStatusField, {ld32(stageUIField)}), 8});
    vcx(me, slot::addChildZ, {me, cx(StageUI_getAttributeField, {ld32(stageUIField)}), 8});
    vcx(me, slot::addChildZ, {me, cx(StageUI_getLuxTargetField, {ld32(stageUIField)}), 8});
    assignLambda(cx(StageUI_getCarouselDeck, {ld32(stageUIField)}) + 0x3E0, fa(0x290), FunctionBool_assign, VT_carousel0);
    assignLambda(cx(StageUI_getCarouselDeck, {ld32(stageUIField)}) + 0x3C8, fa(0x278), FunctionBool_assign, VT_carousel1);
    assignLambda(cx(StageUI_getCarouselDeck, {ld32(stageUIField)}) + 0x3B0, fa(0x260), FunctionBool_assign, VT_carousel2);
    assignLambda(ld32(me + 0x334) + 0x38, fa(0x248), FunctionVoidVec2_assign, VT_aimMarker);

    // ---- the keyblade medals on the deck (medal field and carousel)
    const Addr medalDeck = cx(StageUI_getMedalField, {ld32(stageUIField)});
    cx(AvatarUtil_getCurrentKeyblade, {fa(I_USER_KEYBLADE)});
    cx(Keyblade_create, {fa(I_KEYBLADE_SP), ld32(fa(0x1FC))});
    const Addr medalIds = fa(0x208);                                // UserKeyblade medal ids (64 bit), count at +0x200
    if (ld32(fa(I_KEYBLADE_SP)) != 0) {
        const Addr fn = fa(0x1D8);
        makeLambda(fn, VT_deckMedal, me);
        cx(MedalDeck_addMedalOntoDeck, {medalDeck, fa(I_KEYBLADE_SP), medalIds, ld32(fa(0x200)), fn});
        fnDestroy(fn);
        const Addr carousel = cx(StageUI_getCarouselDeck, {ld32(stageUIField)});
        const Addr kb = ld32(fa(I_KEYBLADE_SP));
        cx(SAFEBUFFER_READ, {kb, fa(0x124), ld32(kb + 0xC) + 0xD0, 4, 0xD0});
        cx(CarouselDeck_changeDeckStyle, {carousel, ld32(fa(0x124))});
        cx(CarouselDeck_addMedalOntoDeck, {cx(StageUI_getCarouselDeck, {ld32(stageUIField)}), medalIds, ld32(fa(0x200))});
    }
    // the support medal of the friend
    if (ld32(actorManager() + 0x24) != 0) {
        if (cx(BattleUtil_getSupportMedal, {fa(0x190), fa(0x68)}) == 1) {
            const Addr fn = fa(0x178);
            makeLambda(fn, VT_deckFriendMedal, me);
            cx(MedalDeck_addFriendMedalOntoDeck, {medalDeck, fa(I_KEYBLADE_SP), fa(0x190), fn});
            fnDestroy(fn);
            const Addr carousel = cx(StageUI_getCarouselDeck, {ld32(stageUIField)});
            cx(CarouselDeck_addFriendMedalOntoDeck, {carousel, ld32(fa(0x198)), ld32(fa(0x70))});
        }
    }
    cx(CarouselDeck_setDeckInfoWindow, {cx(StageUI_getCarouselDeck, {ld32(stageUIField)}), medalDeck});
    cx(CarouselDeck_reflashDeck, {cx(StageUI_getCarouselDeck, {ld32(stageUIField)})});
    cx(SceneBattleBase_firstSetPlayerCP, {me, ld32(fa(I_STALE)), ld32(fa(0x1FC))});

    // ---- player status field (Lux rank etc.) and the status field slide-in
    {
        const Addr stageUI = ld32(stageUIField);
        const Addr api = cx(APIMANAGER_GETINSTANCE, {});
        const Addr sb1 = ld32(api + 0x8C);
        cx(SAFEBUFFER_READ, {sb1, fa(0x128), ld32(sb1 + 0xC) + 0x38, 4, 0x38});
        const uint32_t rankA = ld32(fa(0x128));
        const Addr app = cx(APPSTATE_GETINSTANCE, {});
        const Addr sb2 = ld32(app + 0x20);
        cx(SAFEBUFFER_READ, {sb2, fa(0x12C), ld32(sb2 + 0xC) + 0x34, 4, 0x34});
        const uint32_t rankB = ld32(fa(0x12C));
        const Addr avatarObject = ld32(avatarActor() + 0x288);
        cx(StageUI_createPlayerStatusField, {stageUI, rankA, rankB, ld32(avatarObject + 0x56C)});
    }
    {
        const Addr addChildZ = ld32(ld32(me) + slot::addChildZ);
        cx(addChildZ, {me, cx(StageUI_getPlayerStatusField, {ld32(stageUIField)}), 8});
    }
    {
        const Addr statusField = cx(StageUI_getStatusField, {ld32(stageUIField)});
        const Addr empty = fa(0x160);
        st32(empty + 0x10, 0);                                      // an empty std::function<void()>
        const Addr action = cx(SceneUtil_createTopInAnimation, {statusField, 0, 0, empty});
        const Addr ease = cx(EaseSineOut_create, {action});
        cx(RUN_ACTION, {statusField, ease});
        fnDestroy(empty);
    }
    {
        const Addr statusField = cx(StageUI_getStatusField, {ld32(stageUIField)});
        const Addr button = findNode(statusField, fa(0x5C), STR_MenuButton, 10);
        const Addr fn = fa(0x148);
        makeLambda(fn, VT_menuButton, me);
        cx(Widget_addTouchEventListener, {button, fn});
        fnDestroy(fn);
    }
    st32(me + 0x368, ld32(actorManager() + 0x20));
    cx(SceneActionMap_setNewMap, {me, ld32(actorManager() + 0x20)});

    // ---- BGM: replace the main BGM when the stage has its own
    {
        const uint32_t bgm = ld32(me + 0x2E0);
        if (bgm != cx(AudioGetGameMainBGMResourceID, {})) {
            cx(AudioReplaceGameMainBGM, {ld32(me + 0x2E0), 500});
        }
    }
    {
        const Addr mgr = sm();
        const Addr fn = fa(0x130);
        makeLambda(fn, VT_backKey, me);
        cx(SceneManager_setOnBackKeyCallback, {mgr, fn});
        fnDestroy(fn);
    }
    sharedRelease(ld32(fa(I_KEYBLADE_SP + 4)));
    cx(SafeBuffer_dtor, {fa(I_FRIEND_STYLE)});
    cx(SafeBuffer_dtor, {fa(I_PLAYER_STYLE)});
    sharedRelease(ld32(fa(I_STAGE_SP + 4)));
    return 1;
}

// ---------------------------------------------------------------------------------------------------------------
// The field mode tick: the avatar walks towards the aim target (or jumps), the stage managers update, and a
// meeting with an enemy platoon starts the battle mode.
void SceneActionMap_updateFieldMode(uint32_t self, float dt) {
    guest::Frame F(0x88);
    const Addr me = self;
    const uint32_t dtBits = rw::fbits(dt);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    const Addr P = fa(0x64), T = fa(0x5C), D = fa(0x54), V = fa(0x48);       // avatar position, target, delta, new position
    constexpr uint32_t ONE = 0x3F800000;
    const Addr stageManagerField = me + 0x320, actorManagerField = me + 0x2BC;
    const Addr avatar = ld32(ld32(actorManagerField) + 0x20);                // AvatarActor (position at +0x270)

    const Addr stageManager = ld32(stageManagerField);
    if (ld32(stageManager + 0x18) == 2 && cx(StageManager_checkMapTransportGate, {stageManager, avatar + 0x270}) == 1) {
        cx(SceneActionMap_toLoadingModeSetup, {me});
        return;
    }
    const uint32_t substate = ld32(me + 0x33C);                              // 1 walking, 2 jumping
    auto idle = [&]() {
        const Addr av = ld32(avatar + 0x288);                                // Avatar: motion at +0x524
        if (ld32(av + 0x524) != 27) cx(Avatar_changeMotion, {av, 27});
    };
    if (substate == 2) {
        cx(SceneActionMap_updateAimTarget, {fa(0x40), me});
        cx(Vec2_dtor, {fa(0x40)});
        cx(AvatarActor_setActionJump, {avatar});
        if (cx(MapNode_updateMapPosition, {avatar}) == 0) {
            const Addr av = ld32(avatar + 0x288);
            if (ld32(av + 0x524) != 27) {
                cx(Avatar_changeMotion, {av, 27});
                st32(me + 0x33C, 1);
                cx(AimTarget_reset, {ld32(me + 0x334)});
            }
        }
    } else if (substate == 1) {
        const Addr fm = ld32(me + 0x2C0);
        const uint32_t occurs = cx(StageFieldManager_checkEventOccurs, {fm, ld32(stageManagerField), avatar});
        const Addr aim = ld32(me + 0x334);
        if (occurs == 1) {
            cx(AimTarget_stop, {aim, 0});
        } else if (ld8(aim + 0x25) == 0 && ld8(aim + 0x26) == 0) {
            idle();
        } else {
            cx(Vec2_copy, {P, avatar + 0x270});
            cx(SceneActionMap_updateAimTarget, {T, me});
            const uint32_t dist = cx(Vec2_distance, {P, T});
            cx(Vec2_copy, {D, T});
            cx(Vec2_subtract, {D, P});                                        // D = T - P
            const uint32_t ax = ld32(D) & 0x7FFFFFFF, ay = ld32(D + 4) & 0x7FFFFFFF;
            bool far = cx(fcmpgt, {ax, ONE}) != 0;
            if (!far) {
                const uint32_t le = cx(fcmple, {ay, ONE});
                const uint32_t un = cx(unordsf2, {ay, ONE});
                far = (un | le) == 0;
            }
            if (!far) {
                cx(AimTarget_reset, {ld32(me + 0x334)});
            } else {
                // face the target when it is more than one unit away horizontally
                {
                    const uint32_t le = cx(fcmple, {ax, ONE});
                    const uint32_t un = cx(unordsf2, {ax, ONE});
                    if ((un | le) == 0) {
                        const uint32_t targetLeft = cx(fcmplt, {ld32(T), ld32(P)});
                        const uint32_t dir = targetLeft ? ONE : 0xBF800000u;
                        const uint32_t scaleX = vcx(avatar, 0x2C, {avatar});
                        if (cx(fcmpeq, {scaleX, dir}) == 0) vcx(avatar, 0x28, {avatar, dir});
                    }
                }
                // walk / run motion and speed factor by distance
                uint32_t factor;
                const Addr av = ld32(avatar + 0x288);
                const uint32_t motion = ld32(av + 0x524);
                {
                    const uint32_t lt = cx(fcmplt, {dist, 0x43480000});            // 200
                    const uint32_t un = cx(unordsf2, {dist, 0x43480000});
                    if ((lt | un) == 0) {
                        factor = 0x3FC00000;                                       // 1.5
                        if (motion != 0x18) cx(Avatar_changeMotion, {av, 0x18});
                    } else {
                        const uint32_t lt2 = cx(fcmplt, {dist, 0x42C80000});       // 100
                        const uint32_t un2 = cx(unordsf2, {dist, 0x42C80000});
                        if ((lt2 | un2) == 0) {
                            factor = ONE;
                            if (motion != 0x17) cx(Avatar_changeMotion, {av, 0x17});
                        } else {
                            factor = 0x3F000000;                                   // 0.5
                            if (motion != 0x1C) cx(Avatar_changeMotion, {av, 0x1C});
                        }
                    }
                }
                // step = clamp(dt * 440 * factor / dist, 0, 1); V = P + D * step
                cx(Vec2_copy, {V, P});
                const uint32_t scaled = cx(FMUL, {dtBits, 0x43DC0000});
                const uint32_t speed = cx(FMUL, {scaled, factor});
                const uint32_t ratio = cx(divsf3, {speed, dist});
                uint32_t step = 0;
                if (cx(fcmplt, {ratio, 0}) == 0) step = ratio;
                uint32_t t = step;
                if (cx(fcmpgt, {step, ONE}) != 0) t = ONE;
                const uint32_t dx = cx(FMUL, {ld32(D), t});
                st32(V, cx(FADD, {ld32(V), dx}));
                const uint32_t dy = cx(FMUL, {ld32(D + 4), t});
                st32(V + 4, cx(FADD, {ld32(V + 4), dy}));
                const Addr fm2 = ld32(me + 0x2C0);
                cx(StageFieldManager_fitRectToPoint, {fm2, V, fm2 + 0xAC});
                if (cx(StageFieldManager_isCollisionDetection, {ld32(me + 0x2C0), P, V}) != 0) {
                    if (cx(StageFieldManager_hasJumpData, {ld32(me + 0x2C0), avatar, T}) == 1) {
                        st32(me + 0x33C, 2);
                    } else if (cx(StageExploreManager_searchMovablePosition,
                                  {ld32(me + 0x328), avatar, V, ld32(me + 0x2C0)}) == 1) {
                        cx(AvatarActor_lookAt, {avatar, T});
                    }
                } else {
                    st32(avatar + 0x270, ld32(V));
                    st32(avatar + 0x274, ld32(V + 4));
                }
                cx(Vec2_dtor, {V});
            }
            cx(Vec2_dtor, {D});
            cx(Vec2_dtor, {T});
            cx(Vec2_dtor, {P});
        }
    } else {
        idle();
    }

    // ---- common: the aim marker, the explore manager callback, the actors, the camera
    {
        const Addr aim = ld32(me + 0x334);
        const uint32_t safe = cx(StageFieldManager_isCollisionSafe, {ld32(me + 0x2C0), ld32(aim + 0x34) + 0x270});
        cx(AimTarget_updateMarker, {aim, dtBits, safe});
    }
    {
        const Addr explore = ld32(me + 0x328);
        if (ld32(explore + 0x38) != 0) cx(FunctionVoid_call, {explore + 0x28});
    }
    cx(StageActorManager_update, {ld32(actorManagerField), dtBits});
    cx(SceneActionMap_updateCameraPosition, {me, dtBits, ONE});
    if (ld32(ld32(avatar + 0x288) + 0x524) != 16 && ld32(ld32(stageManagerField) + 0x18) == 2) {
        if (cx(StageActorManager_checkEncount, {ld32(actorManagerField), ld32(stageManagerField), avatar}) == 1) {
            cx(SceneActionMap_toBattleModeSetup, {me});
        }
    }
}

RW_FUNCTION(0x5FBA70, "SceneActionMap::init()", SceneActionMap_init);
RW_FUNCTION(0x5FE9F8, "SceneActionMap::updateFieldMode(float)", SceneActionMap_updateFieldMode);
