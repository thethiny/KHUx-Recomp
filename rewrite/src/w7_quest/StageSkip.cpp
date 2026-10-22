// StageSkipActionMap: a quest finished without playing it - every map's enemies counted as defeated (their lux and
// munny scaled by misc 1000 / 10000, rounded up; exp, raid points, drops), every treasure and material taken, then
// the stage clear and the result screen. Plus StageManager's counting lambdas (missions over the enemy list).
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t StageSkipActionMap_ctor = 0x75DD55, StageSkipActionMap_takeTreasureContent = 0x75E37D;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t SM_removeBackground = 0x611691, SM_changeSceneLayer = 0x612B99;
constexpr uint32_t AppState_createStageState = 0x4EA861, AvatarUtil_getCurrentKeyblade = 0x4976FD;
constexpr uint32_t StageState_setCpMax = 0x4EF859, StageState_addCP = 0x4EEC29, StageState_addLUX = 0x4EECD9;
constexpr uint32_t StageState_addRaidpoint = 0x4EED65, StageState_addEXP = 0x4EEDF1, StageState_addMoney = 0x4EEE7D;
constexpr uint32_t StageState_addDefeatedEnemy = 0x4EEF09, StageState_addMaterial = 0x4EF141;
constexpr uint32_t StageState_addEnemyDropItem = 0x4EF289, StageState_addTreasure = 0x4EF341;
constexpr uint32_t StageManager_create = 0x5FC7E1, StageManager_getMapTotalNumber = 0x75BBD5;
constexpr uint32_t StageManager_getMapStartInfo = 0x75BBD9, StageManager_getEnemyPlatoonInfo = 0x75C229;
constexpr uint32_t StageManager_execTransitionMap = 0x75C455, EnemyActor_create = 0x74AB79;
constexpr uint32_t StageUtil_getEnemyDropPrizes = 0x763C1D, StageUtil_getTreasurePrizes = 0x763DF9;
constexpr uint32_t APIManager_executeStageClear = 0x4CB771, SafeBuffer_read = 0x45A269, shared_addRef = 0xC8DBB5;
constexpr uint32_t function_vv_assign = 0x4EB9CD, function_vb_assign = 0x490109, libc_ceilf = 0x40EFBC;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_BATTLE_RESULT = 0xFD6864;
constexpr uint32_t VT_OnEnter = 0xF6E2C4, VT_OnExit = 0xF6E298;   // {this}
constexpr uint32_t V_release = 0x04, V_init = 0x248;
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_DONE = 0x285, SIZE = 0x288;
// StageManager: +0xA8 / +0xAC the treasures, +0xB4 / +0xB8 the materials; MapStartInfo +8 the map index
constexpr uint32_t SM_TREASURES = 0xA8, SM_MATERIALS = 0xB4;
// EnemyActor: +0x2A8 its parameters (SafeBuffer: +0x14 exp, +0x18 munny, +0x1C raid points, +0x20 lux),
// +0x2D8 / +0x2DC the enemy row (shared_ptr, id at 0)
constexpr uint32_t EA_PARAMS = 0x2A8, EA_ROW = 0x2D8;
constexpr uint32_t MISC_SKIP_RATE = 1000;
constexpr uint32_t SCM_RETRY_FLAG = 0x4CC, SCM_RETRY_FN = 0x548;
constexpr uint32_t ITEM_LUX = 11, ITEM_MUNNY = 4;

uint32_t sbRead(uint32_t sb, uint32_t index) {
    Frame f;
    const uint32_t v = f.alloc(4);
    call(SafeBuffer_read, {sb, v, ld32(sb + 0xC) + index, 4, index});
    return ld32(v);
}
uint32_t stage() { return ld32(call(fn::AppState_getInstance, {}) + 0x20); }

// float -> unsigned as __fixunssfsi (negative -> 0)
uint32_t toUnsigned(float v) { return v <= 0.0f ? 0 : uint32_t(v); }
uint32_t scaled(float rate, uint32_t value) {
    const float product = rate * float(int32_t(value));
    return toUnsigned(bitsf(call(libc_ceilf, {fbits(product)})));
}

// four empty MapPrizes::Items (type 1, nothing)
void clearPrizes(uint32_t items) {
    for (uint32_t k = 0; k < 4; k++) {
        st32(items + 16 * k, 1);
        st32(items + 16 * k + 4, 0);
        st32(items + 16 * k + 8, 0);
        st8(items + 16 * k + 12, 0);
    }
}
}  // namespace

namespace stage_skip {

uint32_t create() {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(StageSkipActionMap_ctor, {self});
    if (vcall(self, V_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x5FB8DA, "StageSkipActionMap::create", stage_skip::create);

// a new stage state with the equipped keyblade's CP
uint32_t init(uint32_t self) {
    call(SM_removeBackground, {sceneManager()});
    call(AppState_createStageState, {call(fn::AppState_getInstance, {}), 0});
    Frame f;
    const uint32_t kb = f.alloc(0x60);
    call(AvatarUtil_getCurrentKeyblade, {kb});
    call(StageState_setCpMax, {stage(), ld32(kb + 0xC)});
    call(StageState_addCP, {stage(), ld32(kb + 0x40)});
    for (uint32_t k = 0; k < 2; k++) {
        Frame g;
        const uint32_t fo = newFunction(g, k ? VT_OnExit : VT_OnEnter, {self});
        call(function_vv_assign, {self + (k ? F_ON_EXIT : F_ON_ENTER), fo});
        functionDestroy(fo);
    }
    st8(self + F_DONE, 0);
    return 1;
}
RW_FUNCTION(0x75DD90, "StageSkipActionMap::init", stage_skip::init);

// the first frame does the whole stage, then the clear and the result screen
uint32_t update(uint32_t self, uint32_t) {
    if (ld8(self + F_DONE)) return 0;
    st8(self + F_DONE, 1);
    const uint32_t sm = call(StageManager_create, {0});
    const uint32_t maps = call(StageManager_getMapTotalNumber, {sm});
    float rate;
    {
        Frame f;
        const uint32_t row = masterRow(f, fn::Misc_create, MISC_SKIP_RATE);
        rate = float(int32_t(sbRead(ld32(row), 4))) / 10000.0f;
        sharedRelease(row);
    }
    Frame f;
    const uint32_t items = f.alloc(64);
    for (uint32_t m = 0; m < maps; m++) {
        const uint32_t platoons = call(StageManager_getEnemyPlatoonInfo, {sm});
        for (uint32_t p = 0; p < (ld32(platoons + 4) - ld32(platoons)) >> 2; p++) {
            const uint32_t info = ld32(ld32(platoons) + 4 * p);
            if ((ld32(info + 8) & 0x20) || !ld32(info + 4)) continue;
            const uint32_t base = ld32(info);
            for (uint32_t e = 0; e < ld32(info + 4); e++) {
                const uint32_t entry = base + 32 * e;
                const uint32_t actor = call(EnemyActor_create, {entry, 0});
                {
                    const uint32_t st = stage();
                    const uint32_t ctrl = ld32(actor + EA_ROW + 4);
                    const uint32_t row = ld32(actor + EA_ROW);
                    if (ctrl) call(shared_addRef, {ctrl});
                    call(StageState_addDefeatedEnemy, {st, sbRead(row, 0)});
                    if (ctrl) call(fn::shared_release, {ctrl});
                }
                const uint32_t params = actor + EA_PARAMS;
                {
                    const uint32_t st = stage();
                    call(StageState_addLUX, {st, scaled(rate, sbRead(params, 0x20))});
                }
                {
                    const uint32_t st = stage();
                    call(StageState_addEXP, {st, sbRead(params, 0x14)});
                }
                {
                    const uint32_t st = stage();
                    call(StageState_addRaidpoint, {st, sbRead(params, 0x1C)});
                }
                {
                    const uint32_t st = stage();
                    call(StageState_addMoney, {st, scaled(rate, sbRead(params, 0x18))});
                }
                call(StageState_addEnemyDropItem, {stage(), ld32(entry + 0x14), ld32(entry + 0x10)});
                clearPrizes(items);
                const uint32_t r = call(StageUtil_getEnemyDropPrizes, {items, ld32(entry + 0x14), ld32(entry + 0x10), 0});
                call(StageSkipActionMap_takeTreasureContent, {r, items});
            }
        }
        const uint32_t tEnd = ld32(sm + SM_TREASURES + 4);
        for (uint32_t t = ld32(sm + SM_TREASURES); t != tEnd; t += 4) {
            const uint32_t box = ld32(t);
            call(StageState_addTreasure, {stage(), ld32(box + 0x10), ld32(box + 0xC)});
            clearPrizes(items);
            const uint32_t r = call(StageUtil_getTreasurePrizes, {items, ld32(box + 0x10), ld32(box + 0xC), 0});
            call(StageSkipActionMap_takeTreasureContent, {r, items});
        }
        const uint32_t mEnd = ld32(sm + SM_MATERIALS + 4);
        for (uint32_t q = ld32(sm + SM_MATERIALS); q != mEnd; q += 4) {
            const uint32_t mat = ld32(q);
            for (uint32_t k = 0; k < ld32(mat + 0xC); k++)
                call(StageState_addMaterial, {stage(), ld32(mat + 8), ld32(mat + 0x10)});
        }
        const uint32_t start = call(StageManager_getMapStartInfo, {sm});
        uint32_t next = ld32(start + 8) + 1;
        if (maps <= next) next = 0;
        st32(start + 8, next);
        call(StageManager_execTransitionMap, {sm});
    }
    {
        const uint32_t scm = sceneManager();
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        st8(scm + SCM_RETRY_FLAG, 1);
        call(function_vb_assign, {scm + SCM_RETRY_FN, none});
        functionDestroy(none);
    }
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(APIManager_executeStageClear, {api, none});
        functionDestroy(none);
    }
    const uint32_t scm = sceneManager();
    Frame g;
    const uint32_t layerFn = newFunction(g, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(GOT_CREATE_BATTLE_RESULT)});
    call(SM_changeSceneLayer, {scm, layerFn, 1, 1, 1});
    functionDestroy(layerFn);
    return 0;
}
RW_FUNCTION(0x75DEC8, "StageSkipActionMap::update", stage_skip::update);

// the lux and munny among four prizes
uint32_t takeTreasureContent(uint32_t, uint32_t items) {
    uint32_t result = 0;
    for (uint32_t k = 0; k < 4; k++) {
        const uint32_t type = ld32(items + 16 * k);
        const uint32_t n = ld32(items + 16 * k + 8);
        result = type;
        if (type == ITEM_LUX) result = call(StageState_addLUX, {stage(), n});
        else if (type == ITEM_MUNNY) result = call(StageState_addMoney, {stage(), n});
    }
    return result;
}
RW_FUNCTION(0x75E37C, "StageSkipActionMap::takeTreasureContent", stage_skip::takeTreasureContent);

// StageManager's enemy-list lambdas, captures by reference {&left, ...}; each takes one enemy row (shared_ptr):
// how many enemies of a kind (+0x470) are among the first `left`
uint32_t countKind(uint32_t functor, uint32_t rowRef) {
    if (int32_t(ld32(ld32(functor + 4))) >= 1 && sbRead(ld32(rowRef), 0x470) == ld32(ld32(functor + 8)))
        st32(ld32(functor + 0xC), ld32(ld32(functor + 0xC)) + 1);
    st32(ld32(functor + 4), ld32(ld32(functor + 4)) - 1);
    return 1;
}
RW_FUNCTION(0x75C920, "StageManager enemy count of a kind operator() (sub_75C920)", stage_skip::countKind);

// the highest kind (+0x470) among the first `left`
uint32_t maxKind(uint32_t functor, uint32_t rowRef) {
    if (int32_t(ld32(ld32(functor + 4))) >= 1) {
        const int32_t v = int32_t(sbRead(ld32(rowRef), 0x470));
        const uint32_t best = ld32(functor + 8);
        if (int32_t(ld32(best)) < v) st32(best, uint32_t(v));
    }
    st32(ld32(functor + 4), ld32(ld32(functor + 4)) - 1);
    return 1;
}
RW_FUNCTION(0x75CA34, "StageManager enemy highest kind operator() (sub_75CA34)", stage_skip::maxKind);

// the first `left` counted by size class (+0x458: 1, 2, 3)
uint32_t countSizes(uint32_t functor, uint32_t rowRef) {
    if (int32_t(ld32(ld32(functor + 4))) >= 1) {
        const uint32_t v = sbRead(ld32(rowRef), 0x458);
        uint32_t counter = 0;
        if (v == 3) counter = ld32(functor + 0x10);
        else if (v == 2) counter = ld32(functor + 0xC);
        else if (v == 1) counter = ld32(functor + 8);
        if (counter) st32(counter, ld32(counter) + 1);
    }
    st32(ld32(functor + 4), ld32(ld32(functor + 4)) - 1);
    return 1;
}
RW_FUNCTION(0x75CB50, "StageManager enemy sizes operator() (sub_75CB50)", stage_skip::countSizes);

}  // namespace stage_skip
