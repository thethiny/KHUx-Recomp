// SceneActionMap: creation and lifetime, the per-frame update (dispatch on the stage mode) and the touch routing.
#include "SceneActionMap.hpp"

using namespace smain;
using rw::guest_addr;

namespace {

namespace g {
constexpr uint32_t SceneBattleBase_ctor = 0x5F9FF9;
constexpr uint32_t SceneBattleBase_dtor = 0x5FA0B5;
constexpr uint32_t SceneBattleBase_init_UI = 0x5FA2B1;
constexpr uint32_t SceneBattleBase_onTouchBeganBattle = 0x5FA631;
constexpr uint32_t SceneBattleBase_onTouchMovedBattle = 0x5FA675;
constexpr uint32_t SceneBattleBase_onTouchEndedBattle = 0x5FA679;
constexpr uint32_t SceneBattleBase_onTouchCancelledBattle = 0x5FA6B9;
constexpr uint32_t GestureRecognizer_ctor = 0x4F348D;     // (recognizer, Node* owner)
constexpr uint32_t GestureRecognizer_dtor = 0x4F3621;
constexpr uint32_t Vec2_ctor = 0x8E4B95;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t aeabi_memset = 0x40E7A0;               // __aeabi_memset(dst, n, c)
constexpr uint32_t RefVector_clear = 0x5E44F1;            // cocos2d::Vector<Ref*>::clear
constexpr uint32_t SpeechMap_dtor = 0x608097;             // ~unordered_map<SpeechDisp*, deque<string>>
constexpr uint32_t AppSetTextureCacheLimit = 0x799F05;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t SafeBuffer_read = 0x45A269;            // (buf, dst, src, size, offset)
constexpr uint32_t StageSkipActionMap_create = 0x5FB8DB;
constexpr uint32_t SceneActionMap_updateBattleMode = 0x5FD001;
constexpr uint32_t SceneActionMap_updateFieldMode = 0x5FE9F9;
constexpr uint32_t SceneActionMap_updateIntrludeMode = 0x5FEEA5;
constexpr uint32_t SceneActionMap_updateTalkMode = 0x5FF255;
constexpr uint32_t SceneActionMap_onTouchEndedTalk = 0x5FF705;
constexpr uint32_t SceneActionMap_toFieldModeSetup = 0x5FFE21;
constexpr uint32_t StageFieldManager_update = 0x7560B5;          // (manager, float dt, AvatarActor*)
constexpr uint32_t StageFieldManager_reapStateBoard = 0x757721;  // (manager, float dt, Vector<EnemyActor*>&)
constexpr uint32_t StageFieldManager_zoomUpdate = 0x755D41;
constexpr uint32_t StageFieldManager_showMapNode = 0x755729;
constexpr uint32_t StageFieldManager_getScreenPosToMapPos = 0x756559;   // (Vec2* out, manager, const Vec2&)
constexpr uint32_t StageFieldManager_checkEventAreaTap = 0x756E55;
constexpr uint32_t GuiltBurstCutInShader_updateInit = 0x4960E1;
constexpr uint32_t StageUI_updateDispUI = 0x75F841;
constexpr uint32_t StageUI_getCarouselDeck = 0x75E661;
constexpr uint32_t CarouselDeck_isVisibleDeckInfoWindow = 0x5D14C5;
constexpr uint32_t CommonDirection_calc_warning = 0x76675D;     // (dir, EnemyActor*, AvatarActor*, StageManager*, bool)
constexpr uint32_t CommonDirection_to_start = 0x76669D;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t StageDirection_isEndTitle = 0x769F7F;
constexpr uint32_t StageActorManager_showWanderingSymbolEnemy = 0x74F15D;
constexpr uint32_t AimTarget_onBegan = 0x73F5D1;
constexpr uint32_t AimTarget_onMoved = 0x73F66D;
constexpr uint32_t AimTarget_onEnded = 0x73F6FD;
constexpr uint32_t AimTarget_onCancelled = 0x73F781;
constexpr uint32_t Touch_getLocationInView = 0x900E39;
constexpr uint32_t Vec2_copy = 0x8E4C03;                  // Vec2(const Vec2&)
constexpr uint32_t Vec2_subtract = 0x8E530F;              // (Vec2*, const Vec2&)
constexpr uint32_t function_vv_copy = 0x4683CD;           // std::function<void()>(const function&)
constexpr uint32_t function_vv_move = 0x47F905;           // std::function<void()>(function&&)
constexpr uint32_t function_b_call = 0x51022D;            // std::function<bool()>::operator()
constexpr uint32_t shared_release = 0xC8DBD1;
constexpr uint32_t StageFieldManager_setStageMapInfo = 0x7550D5;   // (manager, Layer*, StageManager*) -> MapInfo*
constexpr uint32_t StageFieldManager_addMapNodeToField = 0x755509;
constexpr uint32_t StageFieldManager_addNpcToFieldLayer = 0x755CED;
constexpr uint32_t StageFieldManager_setLuxTarget = 0x7572D1;
constexpr uint32_t StageFieldManager_setTreasureTarget = 0x7573D9;
constexpr uint32_t StageFieldManager_getCameraMapPosition = 0x755675;  // (Vec2* out, manager, const Vec2&, float)
constexpr uint32_t StageFieldManager_fitRectToPoint = 0x756633;        // (manager, Vec2*, Rect*)
constexpr uint32_t StageFieldManager_updateCamera = 0x7556E9;          // update(Vec2&, float)
constexpr uint32_t StageFieldManager_isCollisionRectSafe = 0x756735;
constexpr uint32_t StageFieldManager_changeMode = 0x7576D3;
constexpr uint32_t MapGateMarker_create = 0x5FFBCD;
constexpr uint32_t MapGateMarker_show = 0x5DB565;          // (marker, Vec2&, float)
constexpr uint32_t MapNodeVector_pushBack = 0x5DAEC1;
constexpr uint32_t MapTreasure_create = 0x5DF441;          // (const Info*, Node*, FromStageType)
constexpr uint32_t MapMaterial_create = 0x5DB9ED;          // (const Info*, Node*)
constexpr uint32_t MapNonPlayableCharacter_create = 0x5DC2C5;
constexpr uint32_t AimTarget_reset = 0x73F5B5;
constexpr uint32_t StageManager_getEnemyPlatoonInfo = 0x75C229;
constexpr uint32_t StageManager_changeMode = 0x75C4ED;
constexpr uint32_t StageActorManager_setupEnemyActorPlatoon = 0x74ED81;
constexpr uint32_t StageActorManager_getCurrentEncountSymbolEnemyActor = 0x74F5CD;
constexpr uint32_t StageActorManager_clearCurrentTarget = 0x74F5F5;
constexpr uint32_t StageActorManager_changeMode = 0x74FCBD;
constexpr uint32_t StageActorManager_entryCurrentTarget = 0x74F6F5;
constexpr uint32_t StageExploreManager_stopDodgeRoll = 0x754729;
constexpr uint32_t GuiltBurstCutInShader_create = 0x5FFC51;
constexpr uint32_t GuiltBurstCutInShader_addShaderParent = 0x495561;
constexpr uint32_t StageState_hasTreasure = 0x4EF649;
constexpr uint32_t StageState_hasMaterial = 0x4EF52D;
constexpr uint32_t StageState_hasActionAlreadyNPC = 0x4EF6D1;
constexpr uint32_t StageUI_getMedalField = 0x75E65D;
constexpr uint32_t StageUI_getLuxTarget = 0x75E679;
constexpr uint32_t StageUI_getCommonTreasureTarget = 0x75E7A9;
constexpr uint32_t StageUI_getRareTreasureTarget = 0x75E709;
constexpr uint32_t MedalDeck_setBattleState = 0x5DFEAB;
constexpr uint32_t MedalDeck_getCurrentMedalInfo = 0x5DFF6B;
constexpr uint32_t MedalDeck_isValidCurrentBurst = 0x5E04E5;
constexpr uint32_t MedalDeck_getCurrentBurstGauge = 0x5E0565;
constexpr uint32_t CarouselDeck_setBattleState = 0x5D1407;
constexpr uint32_t CarouselDeck_reflashDeck = 0x5D0BC5;
constexpr uint32_t SceneBattleBase_subPlayerCP = 0x5FAD79;   // (scene, cp, const MedalInfo*)
constexpr uint32_t AudioGetGameMainBGMResouceID = 0x5C1A65;
constexpr uint32_t AudioPopGameMainBGM = 0x5C18D9;
}  // namespace g

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_update_22 = 0xF5E780;               // update()::$_22 (closes the warning tutorial popup)
constexpr uint32_t VT_toFieldModeSetup_41 = 0xF5E360;     // toFieldModeSetup()::$_41
constexpr uint32_t VT_stageFinishTutorial_46 = 0xF5E22C;  // stageFinishTutorial()::$_46 (holds the callback)

inline uint32_t i2f(uint32_t v) { return fbits(float(int32_t(v))); }   // __floatsisf

// the inline ~std::function: the callable at *ptr lives in the buffer at f (destroy) or on the heap
inline void destroyFn(uint32_t f, uint32_t ptr) {
    uint32_t p = ld32(ptr);
    if (p == f) vcall(p, 0x10, {p});
    else if (p) vcall(p, 0x14, {p});
}

// StageManager +0x14: the stage kind (0 normal quest); +0x18: the scene mode
inline int32_t stageMode(SceneActionMap *self) { return rw::load<int32_t>(self->stageManager + 0x18); }

}  // namespace

// ------------------------------------------------------------------------------------------------ lifetime

uint32_t SceneActionMap::createScene() {
    uint32_t scene = call(lib::Scene_create, {});
    uint32_t addChild = ld32(ld32(scene) + node::addChild);
    uint32_t layer = createLayer();
    call(addChild, {scene, layer});
    return scene;
}

// SceneManager +0x30C: a scene is being restored; APIManager +0x13C -> +0x48: the quest is a stage skip
uint32_t SceneActionMap::createLayer() {
    if (ld32(call(g::SceneManager_getInstance, {}) + 0x30C) == 0) {
        uint32_t quest = ld32(call(g::APIManager_getInstance, {}) + 0x13C);
        if (ld8(quest + 0x48) == 1) return call(g::StageSkipActionMap_create, {});
    }
    return create();
}

uint32_t SceneActionMap::create() {
    uint32_t p = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<SceneActionMap>(p));
    if (vcall(p, 0x248, {p}) == 1) {          // init()
        call(lib::Ref_autorelease, {p});
        return p;
    }
    vcall(p, 0x4, {p});                       // deleting destructor
    return 0;
}

SceneActionMap *SceneActionMap::construct(SceneActionMap *self) {
    uint32_t a = guest_addr(self);
    call(g::SceneBattleBase_ctor, {a});
    self->vtable = VTABLE;
    call(g::GestureRecognizer_ctor, {a + 0x2F0, a});
    self->titleShown = 0;
    rw::set<uint32_t>(self, 0x350, 0);        // pendingFn: empty
    call(g::Vec2_ctor, {a + 0x358});
    self->field_360 = 0;
    self->intrludeTapped = 0;
    self->avatar = 0;
    call(g::aeabi_memset, {a + 0x378, 0x14, 0});   // talkWindow + the speech map's buckets / first / size
    rw::set<float>(self, 0x38C, 1.0f);        // the speech map's max_load_factor
    self->field_390 = 0;
    self->cutInShader = 0;
    return self;
}

SceneActionMap *SceneActionMap::destruct(SceneActionMap *self) {
    uint32_t a = guest_addr(self);
    self->vtable = VTABLE;
    if (self->talkWindow) call(lib::Ref_release, {self->talkWindow});
    call(g::RefVector_clear, {a + 0x2C4});
    call(g::AppSetTextureCacheLimit, {0});
    call(g::SpeechMap_dtor, {a + 0x37C});
    call(g::Vec2_dtor, {a + 0x358});
    // ~std::function<bool()> (pendingFn)
    uint32_t f = ld32(a + 0x350);
    if (f == a + 0x340) vcall(f, 0x10, {f});
    else if (f) vcall(f, 0x14, {f});
    call(g::GestureRecognizer_dtor, {a + 0x2F0});
    return rw::host_ptr<SceneActionMap>(call(g::SceneBattleBase_dtor, {a}));
}

void SceneActionMap::destructDelete(SceneActionMap *self) {
    call(fn::operator_delete, {guest_addr(destruct(self))});
}

// ------------------------------------------------------------------------------------------------ update

void SceneActionMap::update(SceneActionMap *self, float dt) {
    if (self->menuOpen) return;
    // the original frame (5 registers + 0x64): callees store addresses of their locals in heap objects, so their
    // frames must land where the original's do
    guest::Frame frame(0x78);
    auto call = [](uint32_t fn, std::initializer_list<uint32_t> args) { return uint32_t(rw::call_exact(fn, args)); };
    uint32_t a = guest_addr(self);
    call(g::StageFieldManager_update, {self->fieldManager, fbits(dt), ld32(self->actorManager + 0x20)});
    call(g::StageFieldManager_reapStateBoard, {self->fieldManager, fbits(dt), self->actorManager + 0x4C});
    if (self->cutInShader) {
        call(g::GuiltBurstCutInShader_updateInit, {self->cutInShader, fbits(dt)});
        call(ld32(ld32(self->cutInShader) + 0x1C4), {self->cutInShader, fbits(dt)});   // update(dt)
    }
    switch (stageMode(self)) {
    case 1: call(g::SceneActionMap_updateBattleMode, {a, fbits(dt)}); break;
    case 2: call(g::SceneActionMap_updateFieldMode, {a, fbits(dt)}); break;
    case 3: call(g::SceneActionMap_updateIntrludeMode, {a, fbits(dt)}); break;
    case 4: updateStageTitle(self, dt); break;
    case 5: call(g::SceneActionMap_updateTalkMode, {a, fbits(dt)}); break;
    default: break;
    }
    call(g::StageUI_updateDispUI, {self->stageUI});
    call(g::StageFieldManager_zoomUpdate, {self->fieldManager});
    uint32_t stage = self->stageManager;
    if (rw::load<int32_t>(stage + 0x14) != 0) return;
    uint32_t actors = self->actorManager;
    uint32_t enemy = ld32(actors + 0x94);
    uint32_t hideInfo = 0;
    if (rw::load<int32_t>(stage + 0x18) == 2) {
        uint32_t deck = call(g::StageUI_getCarouselDeck, {self->stageUI});
        hideInfo = (call(g::CarouselDeck_isVisibleDeckInfoWindow, {deck}) ^ 1) & 0xFF;
        stage = self->stageManager;
        actors = self->actorManager;
    }
    if (call(g::CommonDirection_calc_warning, {self->commonDirection, enemy, ld32(actors + 0x20), stage, hideInfo}) != 1)
        return;
    self->menuOpen = 1;
    Frame f;
    uint32_t popup = f.alloc(0x18);
    uint32_t onClose = newFunction(f, VT_update_22, {a});
    rw::call(g::SceneUtil_openTutorialPopup, {popup, 4, onClose, 0, 0, 0, 0});
    functionDestroy(popup);
    functionDestroy(onClose);
}

void SceneActionMap::updateLoadingMode(SceneActionMap *, float) {}

// stage title (mode 4): when the title animation is over, start the field
void SceneActionMap::updateStageTitle(SceneActionMap *self, float) {
    if (call(g::StageDirection_isEndTitle, {self->stageDirection}) != 1) return;
    uint32_t a = guest_addr(self);
    call(g::SceneBattleBase_init_UI, {a});
    call(g::CommonDirection_to_start, {self->commonDirection});
    call(g::StageActorManager_showWanderingSymbolEnemy, {self->actorManager});
    call(g::StageFieldManager_showMapNode, {self->fieldManager});
    call(g::SceneActionMap_toFieldModeSetup, {a, 0});
    self->titleShown = 1;
}

// ------------------------------------------------------------------------------------------------ touches

uint32_t SceneActionMap::onTouchBegan(SceneActionMap *self, uint32_t touch, uint32_t event) {
    switch (stageMode(self)) {
    case 3: onTouchBeganIntrlude(self, touch, event); return 1;
    case 2: onTouchBeganField(self, touch, event); return 1;
    case 1: return call(g::SceneBattleBase_onTouchBeganBattle, {guest_addr(self), touch, event});
    default: return 1;
    }
}

uint32_t SceneActionMap::onTouchBeganField(SceneActionMap *self, uint32_t touch, uint32_t) {
    call(g::AimTarget_onBegan, {self->aimTarget, touch});
    uint32_t field = self->fieldManager;
    Frame f;
    uint32_t mapPos = f.alloc(8);
    uint32_t view = f.alloc(8);
    call(g::Touch_getLocationInView, {view, touch});
    call(g::StageFieldManager_getScreenPosToMapPos, {mapPos, field, view});
    call(g::Vec2_dtor, {view});
    call(g::StageFieldManager_checkEventAreaTap, {self->fieldManager, mapPos});
    call(g::Vec2_dtor, {mapPos});
    return 1;
}

// interlude: a tap is remembered when the stage state's word +8 is set, then handled like a field tap
uint32_t SceneActionMap::onTouchBeganIntrlude(SceneActionMap *self, uint32_t touch, uint32_t event) {
    Frame f;
    uint32_t out = f.alloc(8);
    uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
    call(g::SafeBuffer_read, {state, out, ld32(state + 0xC) + 8, 4, 8});
    if (ld32(out) != 0) self->intrludeTapped = 1;
    onTouchBeganField(self, touch, event);
    return 1;
}

void SceneActionMap::onTouchMoved(SceneActionMap *self, uint32_t touch, uint32_t event) {
    int32_t mode = stageMode(self);
    if (mode == 3 || mode == 2) call(g::AimTarget_onMoved, {self->aimTarget, touch});
    else if (mode == 1) call(g::SceneBattleBase_onTouchMovedBattle, {guest_addr(self), touch, event});
}

void SceneActionMap::onTouchMovedField(SceneActionMap *self, uint32_t touch, uint32_t) {
    call(g::AimTarget_onMoved, {self->aimTarget, touch});
}

void SceneActionMap::onTouchEnded(SceneActionMap *self, uint32_t touch, uint32_t event) {
    switch (stageMode(self)) {
    case 2: case 3: call(g::AimTarget_onEnded, {self->aimTarget, touch}); break;
    case 1: call(g::SceneBattleBase_onTouchEndedBattle, {guest_addr(self), touch, event}); break;
    case 5: call(g::SceneActionMap_onTouchEndedTalk, {guest_addr(self), touch, event}); break;
    default: break;
    }
}

void SceneActionMap::onTouchEndedField(SceneActionMap *self, uint32_t touch, uint32_t) {
    call(g::AimTarget_onEnded, {self->aimTarget, touch});
}

void SceneActionMap::onTouchCancelled(SceneActionMap *self, uint32_t touch, uint32_t event) {
    switch (stageMode(self)) {
    case 2: case 3: case 5: call(g::AimTarget_onCancelled, {self->aimTarget}); break;
    case 1: call(g::SceneBattleBase_onTouchCancelledBattle, {guest_addr(self), touch, event}); break;
    default: break;
    }
}

void SceneActionMap::onTouchCancelledField(SceneActionMap *self, uint32_t, uint32_t) {
    call(g::AimTarget_onCancelled, {self->aimTarget});
}

// ------------------------------------------------------------------------------------------------ map setup

// A new map: place the avatar at the map's start, the gate markers, the aim marker, the enemy platoons, the
// treasures not taken yet, the materials not picked up yet and the NPCs, then the HUD targets and the camera.
void SceneActionMap::setNewMap(SceneActionMap *self, uint32_t avatar) {
    uint32_t a = guest_addr(self);
    uint32_t info = call(g::StageFieldManager_setStageMapInfo, {self->fieldManager, a, self->stageManager});
    uint32_t startX = ld32(info), startY = ld32(info + 4);
    st32(avatar + 0x274, startY);
    st32(avatar + 0x270, startX);
    uint32_t scale = i2f(ld32(info + 12));
    vcall(avatar, 0x28, {avatar, scale});
    call(g::StageFieldManager_addMapNodeToField, {self->fieldManager, avatar});
    uint32_t x = ld32(avatar + 0x270);
    rw::set<uint32_t>(self, 0x35C, ld32(avatar + 0x274));
    rw::set<uint32_t>(self, 0x358, x);
    // gate markers (StageManager +0x9C: vector<GateInfo*>; a gate's position first, its angle at +8)
    uint32_t stage = self->stageManager;
    uint32_t end = ld32(stage + 0xA0);
    for (uint32_t it = ld32(stage + 0x9C); it != end; it += 4) {
        uint32_t gate = ld32(it);
        uint32_t marker = call(g::MapGateMarker_create, {});
        call(g::MapGateMarker_show, {marker, gate, i2f(ld32(gate + 8))});
        call(g::StageFieldManager_addMapNodeToField, {self->fieldManager, marker});
    }
    // the aim marker joins the field node and the field's map node lists
    uint32_t fieldNode = ld32(self->fieldManager + 0x14);
    vcall(fieldNode, node::addChildZ, {fieldNode, ld32(self->aimTarget + 0x34), 20000});
    call(g::MapNodeVector_pushBack, {ld32(self->fieldManager + 0x18) + 0x6C4, ld32(self->aimTarget + 0x34)});
    call(g::MapNodeVector_pushBack, {self->fieldManager + 0x3C, ld32(self->aimTarget + 0x34)});
    call(g::AimTarget_reset, {self->aimTarget});
    uint32_t actors = self->actorManager;
    uint32_t field = self->fieldManager;
    call(g::StageActorManager_setupEnemyActorPlatoon,
         {actors, field, call(g::StageManager_getEnemyPlatoonInfo, {self->stageManager})});
    // the guilt burst cut-in shader (created once, re-targeted on later maps)
    if (self->cutInShader) {
        st32(self->cutInShader + 0x278, self->stageManager);
        st32(self->cutInShader + 0x27C, self->fieldManager);
    } else {
        uint32_t shader = call(g::GuiltBurstCutInShader_create, {});
        self->cutInShader = shader;
        vcall(a, node::addChild, {a, shader});
        call(g::GuiltBurstCutInShader_addShaderParent, {self->cutInShader, self->stageManager, self->fieldManager});
    }
    st32(call(g::StageUI_getCarouselDeck, {self->stageUI}) + 0x418, self->cutInShader);
    st32(self->actorManager + 0x13C, self->cutInShader);
    // treasures (StageManager +0xA8): the "taken" flag (+0x14) from the stage state
    stage = self->stageManager;
    end = ld32(stage + 0xAC);
    for (uint32_t it = ld32(stage + 0xA8); it != end; it += 4) {
        uint32_t t = ld32(it);
        uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
        st8(t + 0x14, uint8_t(call(g::StageState_hasTreasure, {state, ld32(t + 0x10)})));
        uint32_t target = ld32(self->commonDirection + 0x274);
        uint32_t fromRaid = ld32(self->stageManager + 0x14) == 1 ? 1 : 0;
        uint32_t f = self->fieldManager;
        call(g::StageFieldManager_addMapNodeToField, {f, call(g::MapTreasure_create, {t, target, fromRaid})});
    }
    // materials (StageManager +0xB4): only those not picked up yet
    stage = self->stageManager;
    end = ld32(stage + 0xB8);
    for (uint32_t it = ld32(stage + 0xB4); it != end; it += 4) {
        uint32_t m = ld32(it);
        uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
        if (call(g::StageState_hasMaterial, {state, ld32(m + 0x10)})) continue;
        uint32_t target = ld32(self->commonDirection + 0x274);
        uint32_t f = self->fieldManager;
        call(g::StageFieldManager_addMapNodeToField, {f, call(g::MapMaterial_create, {m, target})});
    }
    // NPCs (StageManager +0xC0): bit 1 of the flags (+0x22) = already talked to; kind 5 goes on its own layer
    stage = self->stageManager;
    end = ld32(stage + 0xC4);
    for (uint32_t it = ld32(stage + 0xC0); it != end; it += 4) {
        uint32_t n = ld32(it);
        uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
        uint32_t done = call(g::StageState_hasActionAlreadyNPC, {state, ld32(n + 0x18)});
        uint16_t flags = rw::load<uint16_t>(n + 0x22);
        rw::save<uint16_t>(n + 0x22, uint16_t((done << 1) | (flags & 0xFFFD)));
        uint32_t npc = call(g::MapNonPlayableCharacter_create, {n});
        if (rw::load<uint16_t>(n + 0xC) == 5) {
            call(g::StageFieldManager_addNpcToFieldLayer,
                 {self->fieldManager, npc, uint32_t(int32_t(rw::load<int16_t>(n + 0x20))), ld32(n + 0x1C)});
        } else {
            call(g::StageFieldManager_addMapNodeToField, {self->fieldManager, npc});
            uint32_t child = ld32(npc + 0x2A8);
            uint32_t fn = ld32(self->fieldManager + 0x14);
            vcall(fn, node::addChild, {fn, child});
        }
    }
    field = self->fieldManager;
    uint32_t lux = call(g::StageUI_getLuxTarget, {self->stageUI});
    call(g::StageFieldManager_setLuxTarget, {field, lux, ld32(self->fieldManager + 0x18)});
    field = self->fieldManager;
    uint32_t common = call(g::StageUI_getCommonTreasureTarget, {self->stageUI});
    uint32_t rare = call(g::StageUI_getRareTreasureTarget, {self->stageUI});
    call(g::StageFieldManager_setTreasureTarget, {field, common, rare, ld32(self->fieldManager + 0x18)});
    updateCameraPosition(self, 0.0f, 0.0f);
    call(g::AppSetTextureCacheLimit, {ld32(self->stageManager + 0x11C) << 22});
}

// ------------------------------------------------------------------------------------------------ camera

// Follow the avatar: the camera target is the map position in front of the avatar (facing from the sign of its
// x scale).
void SceneActionMap::updateCameraPosition(SceneActionMap *self, float rate, float dt) {
    uint32_t field = self->fieldManager;
    uint32_t avatar = self->avatar;
    float sx = rw::callf(ld32(ld32(avatar) + 0x2C), {avatar});
    float facing = sx < 0.0f ? -1.0f : 1.0f;
    Frame f;
    uint32_t target = f.alloc(8);
    call(g::StageFieldManager_getCameraMapPosition, {target, field, avatar + 0x270, fbits(facing)});
    uint32_t from = f.alloc(8);
    call(g::Vec2_copy, {from, self->fieldManager + 0x94});
    uint32_t to = f.alloc(8);
    call(g::Vec2_copy, {to, target});
    commonCameraPosition(self, rw::host_ptr<CameraVec>(from), rw::host_ptr<CameraVec>(to), rate, dt);
    call(g::Vec2_dtor, {to});
    call(g::Vec2_dtor, {from});
    call(g::Vec2_dtor, {target});
}

// Move the camera position `from` towards `to` by rate * 880 * dt per axis (no overshoot), keep it inside the
// map rect (StageFieldManager +0x9C) and apply it.
void SceneActionMap::commonCameraPosition(SceneActionMap *self, CameraVec *from, CameraVec *to, float rate,
                                          float dt) {
    Frame f;
    uint32_t d = f.alloc(8);
    call(g::Vec2_copy, {d, guest_addr(from)});
    call(g::Vec2_subtract, {d, guest_addr(to)});
    float step = rate * 880.0f * dt;
    if (ldf(d) > 0.0f) {
        float x = from->x - step;
        from->x = x;
        if (x < to->x) from->x = to->x.get();
    } else {
        float x = step + from->x;
        from->x = x;
        if (x > to->x) from->x = to->x.get();
    }
    if (ldf(d + 4) > 0.0f) {
        float y = from->y - step;
        from->y = y;
        if (y < to->y) from->y = to->y.get();
    } else {
        float y = step + from->y;
        from->y = y;
        if (y > to->y) from->y = to->y.get();
    }
    call(g::StageFieldManager_fitRectToPoint, {self->fieldManager, guest_addr(from), self->fieldManager + 0x9C});
    call(g::StageFieldManager_updateCamera, {self->fieldManager, guest_addr(from), fbits(rate)});
    call(g::Vec2_dtor, {d});
}

namespace {
// the camera moves towards `target` (a Vec2 in the caller's frame), 80 above it
void cameraTowards(SceneActionMap *self, uint32_t source, float rate, float dt) {
    Frame f;
    uint32_t target = f.alloc(8);
    call(g::Vec2_copy, {target, source});
    stf(target + 4, ldf(target + 4) + -80.0f);
    uint32_t from = f.alloc(8);
    call(g::Vec2_copy, {from, self->fieldManager + 0x94});
    uint32_t to = f.alloc(8);
    call(g::Vec2_copy, {to, target});
    using V = SceneActionMap::CameraVec;
    SceneActionMap::commonCameraPosition(self, rw::host_ptr<V>(from), rw::host_ptr<V>(to), rate, dt);
    call(g::Vec2_dtor, {to});
    call(g::Vec2_dtor, {from});
    call(g::Vec2_dtor, {target});
}
}  // namespace

// talk mode: frame the talking actor (StageFieldManager +0xD4)
void SceneActionMap::updateTalkCameraPosition(SceneActionMap *self, float rate, float dt) {
    cameraTowards(self, ld32(self->fieldManager + 0xD4) + 0x270, rate, dt);
}

// an encounter: frame the symbol enemy met on the map
void SceneActionMap::updateMimicCameraPosition(SceneActionMap *self, float rate, float dt) {
    uint32_t enemy = call(g::StageActorManager_getCurrentEncountSymbolEnemyActor, {self->actorManager});
    cameraTowards(self, enemy + 0x270, rate, dt);
}

// ------------------------------------------------------------------------------------------------ modes

// Back to the field (mode 2): reset the decks, the aim, the battle input state; keepBgm: do not pop the battle
// BGM. Then the field tutorial popup (the scene stays paused until it closes).
void SceneActionMap::toFieldModeSetup(SceneActionMap *self, uint32_t keepBgm) {
    uint32_t a = guest_addr(self);
    st32(self->stageManager + 0x18, 2);
    st32(self->stageDirection + 0x2A4, 4);
    st32(self->fieldManager + 0x88, 0);
    uint32_t avatar = ld32(self->actorManager + 0x20);
    if (call(g::StageFieldManager_isCollisionRectSafe, {self->fieldManager, avatar + 0x270}) == 0) {
        // the avatar stands in a wall: back to the map position saved by setNewMap
        uint32_t x = rw::get<uint32_t>(self, 0x358), y = rw::get<uint32_t>(self, 0x35C);
        uint32_t av = ld32(self->actorManager + 0x20);
        st32(av + 0x274, y);
        st32(av + 0x270, x);
    }
    uint32_t bgm = uint32_t(self->bgmIds[0].get());
    if (bgm != call(g::AudioGetGameMainBGMResouceID, {}) && (keepBgm & 0xFF) == 0)
        call(g::AudioPopGameMainBGM, {500});
    call(g::MedalDeck_setBattleState, {call(g::StageUI_getMedalField, {self->stageUI}), 0});
    call(g::CarouselDeck_setBattleState, {call(g::StageUI_getCarouselDeck, {self->stageUI}), 0});
    call(g::CarouselDeck_reflashDeck, {call(g::StageUI_getCarouselDeck, {self->stageUI})});
    call(g::AimTarget_reset, {self->aimTarget});
    call(g::StageExploreManager_stopDodgeRoll, {self->exploreManager});
    call(g::StageActorManager_clearCurrentTarget, {self->actorManager});
    rw::set<uint32_t>(self, 0x338, 0);
    rw::set<uint8_t>(self, 0x2DD, 0);
    rw::set<uint8_t>(self, 0x2DE, 0);
    rw::set<float>(self, 0x2D8, 0.5f);
    rw::set<uint8_t>(self, 0x2DF, 0);
    rw::set<uint8_t>(self, 0x2DC, 1);
    self->touchState = 0;
    call(g::StageManager_changeMode, {self->stageManager, 0});
    if (call(g::function_b_call, {a + 0x340}) == 0) {
        Frame f;
        uint32_t out = f.alloc(8);
        uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
        call(g::SafeBuffer_read, {state, out, ld32(state + 0xC) + 8, 4, 8});
        if (ld32(out) != 0) {
            call(g::StageActorManager_changeMode, {self->actorManager, 0});
            call(g::StageFieldManager_changeMode, {self->fieldManager, 0});
        }
    }
    self->menuOpen = 1;
    Frame f;
    uint32_t popup = f.alloc(0x18);
    uint32_t onClose = newFunction(f, VT_toFieldModeSetup_41, {a});
    call(g::SceneUtil_openTutorialPopup, {popup, 1, onClose, 0, 0, 0, 0});
    functionDestroy(popup);
    functionDestroy(onClose);
}

// the tutorial popup shown when a stage ends; `done` runs when it closes
void SceneActionMap::stageFinishTutorial(SceneActionMap *, uint32_t done) {
    Frame f;
    uint32_t cb = f.alloc(0x18);
    call(g::function_vv_copy, {cb, done});
    uint32_t fn = f.alloc(0x18);
    st32(fn + 0x10, 0);
    uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, VT_stageFinishTutorial_46);
        call(g::function_vv_move, {obj + 8, cb});
    }
    st32(fn + 0x10, obj);
    uint32_t popup = f.alloc(0x18);
    call(g::SceneUtil_openTutorialPopup, {popup, 8, fn, 0, 0, 0, 0});
    destroyFn(popup, popup + 0x10);
    destroyFn(fn, fn + 0x10);
    destroyFn(cb, cb + 0x10);
}

// The burst button: only in the "burst ready" battle step (23): pay the burst's CP and queue the burst as the
// current attack target (type 3) on the battle manager's target. Then back to step 13.
void SceneActionMap::onBurstButton(SceneActionMap *self) {
    if (rw::get<int32_t>(self, 0x338) != 0x17) return;
    uint32_t deck = call(g::StageUI_getMedalField, {self->stageUI});
    uint32_t medal = call(g::MedalDeck_getCurrentMedalInfo, {deck});
    uint32_t valid = call(g::MedalDeck_isValidCurrentBurst, {deck});
    uint32_t gauge = call(g::MedalDeck_getCurrentBurstGauge, {deck});
    if (valid == 1 && call(g::SceneBattleBase_subPlayerCP, {guest_addr(self), gauge * 10000u, medal}) == 1) {
        Frame f;
        uint32_t t = f.alloc(0x18);      // StageActorManager::AttackTarget
        st32(t, 3);
        st32(t + 4, medal);
        st32(t + 8, 0);
        st32(t + 0xC, 0);
        st32(t + 0x10, ld32(self->battleManager + 0x28));
        st32(t + 0x14, 0);
        call(g::StageActorManager_entryCurrentTarget, {self->actorManager, t});
        if (ld32(t + 0xC)) call(g::shared_release, {ld32(t + 0xC)});
    }
    rw::set<int32_t>(self, 0x338, 13);
}

RW_FUNCTION(0x5FCACC, "SceneActionMap::setNewMap", SceneActionMap::setNewMap);
RW_FUNCTION(0x5FFD38, "SceneActionMap::updateCameraPosition", SceneActionMap::updateCameraPosition);
RW_FUNCTION(0x600048, "SceneActionMap::commonCameraPosition", SceneActionMap::commonCameraPosition);
RW_FUNCTION(0x600210, "SceneActionMap::updateTalkCameraPosition", SceneActionMap::updateTalkCameraPosition);
RW_FUNCTION(0x6002DC, "SceneActionMap::updateMimicCameraPosition", SceneActionMap::updateMimicCameraPosition);
RW_FUNCTION(0x5FFE20, "SceneActionMap::toFieldModeSetup", SceneActionMap::toFieldModeSetup);
RW_FUNCTION(0x6007D4, "SceneActionMap::stageFinishTutorial", SceneActionMap::stageFinishTutorial);
RW_FUNCTION(0x60097C, "SceneActionMap::onBurstButton", SceneActionMap::onBurstButton);
RW_FUNCTION(0x5FB888, "SceneActionMap::createScene", SceneActionMap::createScene);
RW_FUNCTION(0x5FB8AC, "SceneActionMap::createLayer", SceneActionMap::createLayer);
RW_FUNCTION(0x5FB92E, "SceneActionMap::create", SceneActionMap::create);
RW_FUNCTION(0x6080B8, "SceneActionMap::SceneActionMap", SceneActionMap::construct);
RW_FUNCTION(0x5FB984, "SceneActionMap::~SceneActionMap", SceneActionMap::destruct);
RW_FUNCTION(0x5FBA64, "SceneActionMap::~SceneActionMap (deleting)", SceneActionMap::destructDelete);
RW_FUNCTION(0x5FCDEC, "SceneActionMap::update", SceneActionMap::update);
RW_FUNCTION(0x5FCFFC, "SceneActionMap::updateLoadingMode", SceneActionMap::updateLoadingMode);
RW_FUNCTION(0x5FF204, "SceneActionMap::updateStageTitle", SceneActionMap::updateStageTitle);
RW_FUNCTION(0x5FF51C, "SceneActionMap::onTouchBegan", SceneActionMap::onTouchBegan);
RW_FUNCTION(0x5FF550, "SceneActionMap::onTouchBeganField", SceneActionMap::onTouchBeganField);
RW_FUNCTION(0x5FF5FC, "SceneActionMap::onTouchBeganIntrlude", SceneActionMap::onTouchBeganIntrlude);
RW_FUNCTION(0x5FF674, "SceneActionMap::onTouchMoved", SceneActionMap::onTouchMoved);
RW_FUNCTION(0x5FF69E, "SceneActionMap::onTouchMovedField", SceneActionMap::onTouchMovedField);
RW_FUNCTION(0x5FF6AC, "SceneActionMap::onTouchEnded", SceneActionMap::onTouchEnded);
RW_FUNCTION(0x5FF6F4, "SceneActionMap::onTouchEndedField", SceneActionMap::onTouchEndedField);
RW_FUNCTION(0x5FFB78, "SceneActionMap::onTouchCancelled", SceneActionMap::onTouchCancelled);
RW_FUNCTION(0x5FFBBC, "SceneActionMap::onTouchCancelledField", SceneActionMap::onTouchCancelledField);
