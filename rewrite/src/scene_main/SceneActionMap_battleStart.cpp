// SceneActionMap: the battle-start tutorial popup and the poison effect on enemies.
#include "SceneActionMap.hpp"

using namespace smain;
using rw::guest_addr;

namespace {
constexpr uint32_t VT_TUTORIAL_CLOSED = 0xF5E258;   // battleStartTutorial's popup callback {this x4, onClosed}
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APPSTATE_TUTORIALS = 0x20;       // AppState: the tutorial record (its lookup object at +0x20)
constexpr uint32_t FUNCTION_VV_COPY = 0x4683CD, FUNCTION_VV_MOVE = 0x47F905, FUNCTION_VV_CALL = 0x470E69;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;   // (sret function, id, function, Widget*, Widget*, Widget*, bool)
constexpr uint32_t TUTORIAL_BATTLE_START = 2;

// libc++'s std::function copy constructor, inline
void copyFunction(uint32_t dst, uint32_t src) {
    const uint32_t p = ld32(src + 16);
    if (!p) {
        st32(dst + 16, 0);
    } else if (p == src) {
        st32(dst + 16, dst);
        vcall(p, 0x0C, {p, dst});
    } else {
        st32(dst + 16, vcall(p, 0x08, {p}));
    }
}

// the tutorial record's entry `id` (an out structure whose word +8 is non-zero when it was seen)
uint32_t tutorialSeen(uint32_t out, uint32_t id) {
    const uint32_t records = ld32(call(AppState_getInstance, {}) + APPSTATE_TUTORIALS) + 0x20;
    call(ld32(ld32(records)), {out, records, id});
    return ld32(out + 8);
}
}  // namespace

// Before the first battle: the battle tutorial popup, onClosed running when it is closed; straight onClosed()
// once any of the tutorial records 0..3 is set.
void battleStartTutorial(SceneActionMap *selfPtr, uint32_t onClosed) {
    const uint32_t self = guest_addr(selfPtr);
    Frame f;
    const uint32_t record = f.alloc(16);
    uint32_t seen = tutorialSeen(record, 0);
    seen |= tutorialSeen(record, 3);
    seen |= tutorialSeen(record, 1);
    seen |= tutorialSeen(record, 2);
    if (seen) {
        call(FUNCTION_VV_CALL, {onClosed});
        return;
    }
    // the callback travels through the nested lambdas (each holding `this` and a copy of it) into the popup's
    // std::function
    const uint32_t copy1 = f.alloc(24), copy2 = f.alloc(24), copy3 = f.alloc(24), copy4 = f.alloc(24);
    const uint32_t copy5 = f.alloc(24);
    call(FUNCTION_VV_COPY, {copy1, onClosed});
    copyFunction(copy2, copy1);
    copyFunction(copy3, copy2);
    copyFunction(copy4, copy3);
    copyFunction(copy5, copy4);
    const uint32_t callback = f.alloc(24);
    st32(callback + 16, 0);
    const uint32_t lambda = call(fn::operator_new, {0x40});
    if (lambda) {
        st32(lambda, VT_TUTORIAL_CLOSED);
        for (uint32_t k = 8; k <= 0x20; k += 8) st32(lambda + k, self);
        call(FUNCTION_VV_MOVE, {lambda + 0x28, copy5});
        for (uint32_t k = 0x20; k >= 8; k -= 8) st32(lambda + k, self);
    }
    st32(callback + 16, lambda);
    const uint32_t popupResult = f.alloc(24);
    call(SceneUtil_openTutorialPopup, {popupResult, TUTORIAL_BATTLE_START, callback, 0, 0, 0, 0});
    functionDestroy(popupResult);
    functionDestroy(callback);
    functionDestroy(copy5);
    functionDestroy(copy4);
    functionDestroy(copy3);
    functionDestroy(copy2);
    functionDestroy(copy1);
}
RW_FUNCTION(0x6003B0, "SceneActionMap::battleStartTutorial", battleStartTutorial);

namespace {
constexpr uint32_t EnemyActor_isDied = 0x6008ED, EnemyActor_changeMotion = 0x74B3F1, EnemyActor_addDamage = 0x74B795;
constexpr uint32_t Badstatus_create = 0x779525, SafeBuffer_read = 0x45A269;
constexpr uint32_t Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t FlashNode_reflection_status = 0x5C3171, DisplayDamageNumber_disp_dmgNum = 0x5EB45D;
constexpr uint32_t Vec2_ctor_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17;
constexpr uint32_t E_STATUS = 0x2C8;          // EnemyActor: IStatusProtocol (getCondition at vtable slot 0)
constexpr uint32_t E_PROPERTY = 0x2A8;        // EnemyActor: its Property (a SafeBuffer; data pointer at +0x2B4)
constexpr uint32_t E_FLASH = 0x330;           // EnemyActor: FlashNode*
constexpr uint32_t FLASH_DAMAGE_SCALE = 0x52C;
constexpr uint32_t BADSTATUS_DAMAGE = 0x54;   // master::Badstatus: damage {base, spread} (per 10000 of max HP)
constexpr uint32_t SCENE_COMMON_DIRECTION = 0x330, DIRECTION_EFFECT_LAYER = 0x274;
constexpr int32_t POISON = 0, POISON_STRONG = 3;
constexpr int32_t BADSTATUS_POISON = 102, BADSTATUS_POISON_STRONG = 104;

// one poison tick on an enemy: max HP x (base + random spread) / 10000 damage, at least 1
void poisonTick(uint32_t self, uint32_t enemy, int32_t badstatus) {
    Frame f;
    const uint32_t status = f.alloc(8);
    call(Badstatus_create, {status, uint32_t(badstatus)});
    const uint32_t data = ld32(status);
    const uint32_t first = f.alloc(8), second = f.alloc(8);
    call(SafeBuffer_read, {data, first, ld32(data + 0xC) + BADSTATUS_DAMAGE, 8, BADSTATUS_DAMAGE});
    const int32_t base = int32_t(ld32(first));
    call(SafeBuffer_read, {data, second, ld32(data + 0xC) + BADSTATUS_DAMAGE, 8, BADSTATUS_DAMAGE});
    const int32_t spread = int32_t(ld32(second + 4));
    const uint32_t random = call(Random_getInstance, {});
    const int32_t roll = int32_t(call(Random_generateUniformInt, {random, uint32_t(-(spread / 2)), uint32_t(spread / 2), 0}));
    const uint32_t maxHp = f.alloc(4);
    call(SafeBuffer_read, {enemy + E_PROPERTY, maxHp, ld32(enemy + 0x2B4) + 4, 4, 4});
    const int32_t damage = int32_t(uint32_t(roll + base) * ld32(maxHp));
    const int32_t amount = uint32_t(damage + 9999) < 19999 ? 1 : damage / 10000;
    call(EnemyActor_changeMotion, {enemy, 5});
    const uint32_t noBurst = f.alloc(8);
    st32(noBurst, 0);
    st32(noBurst + 4, 0);
    call(EnemyActor_addDamage, {enemy, uint32_t(amount), noBurst});
    sharedRelease(noBurst);
    call(FlashNode_reflection_status, {ld32(enemy + E_FLASH), 1});
    const uint32_t layer = ld32(ld32(self + SCENE_COMMON_DIRECTION) + DIRECTION_EFFECT_LAYER);
    const uint32_t position = f.alloc(8);
    call(Vec2_ctor_copy, {position, vcall(enemy, node::getPosition, {enemy})});
    call(DisplayDamageNumber_disp_dmgNum, {uint32_t(amount), layer, position,
                                           ld32(ld32(enemy + E_FLASH) + FLASH_DAMAGE_SCALE), 0, 0, 1, 0, 400,
                                           fbits(1.0f)});
    call(Vec2_dtor, {position});
    sharedRelease(status);
}

// the enemy's condition `type` is active (its turn count, the third word, is non-zero)
bool hasCondition(uint32_t enemy, int32_t type) {
    Frame f;
    const uint32_t condition = f.alloc(12);
    const uint32_t protocol = enemy + E_STATUS;
    call(ld32(ld32(protocol)), {condition, protocol, uint32_t(type)});
    return ld32(condition + 8) != 0;
}
}  // namespace

// The poison tick at the start of a turn: every living poisoned enemy takes its damage (the strong poison when
// only that one is on). True when any enemy was hit.
uint32_t setEnemyPoisonEffect(SceneActionMap *selfPtr, uint32_t enemies) {
    const uint32_t self = guest_addr(selfPtr);
    uint32_t hit = 0;
    for (uint32_t p = ld32(enemies), end = ld32(enemies + 4); p != end; p += 4) {
        const uint32_t enemy = ld32(p);
        if (call(EnemyActor_isDied, {enemy})) continue;
        if (hasCondition(enemy, POISON)) {
            poisonTick(self, enemy, BADSTATUS_POISON);
            hit = 1;
        } else if (hasCondition(enemy, POISON_STRONG)) {
            poisonTick(self, enemy, BADSTATUS_POISON_STRONG);
            hit = 1;
        }
    }
    return hit;
}
RW_FUNCTION(0x600A58, "SceneActionMap::setEnemyPoisonEffect", setEnemyPoisonEffect);
