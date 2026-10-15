// MedalDataManager: builds the UI-side view of an owned medal (UserMedalData) from the server's
// hole::network::api::UserMedal::Medal record: level, attack / defense with bonuses, sell price, experience
// progress, lock / new / in-deck flags and the master::Medal row. A singleton (0x404 bytes, nothing in it is
// used by the functions here).
#pragma once

#include "rw/rw.hpp"
#include "core/Guest.hpp"
#include "net/api/UserMedal.hpp"

struct MedalDataManager {
    // The medal as the menus show it (0x60 bytes, made by std::make_shared: the control block is 16 bytes before).
    struct UserMedalData {
        rw::Field<uint32_t> userMedalIdLo;       // +0x00 user medal id (u64)
        rw::Field<uint32_t> userMedalIdHi;       // +0x04
        rw::Field<int32_t> level;                // +0x08 (at least 1)
        rw::Field<int32_t> attack;               // +0x0C BattleUtil::calcMedalStatus result
        rw::Field<int32_t> defense;              // +0x10
        rw::Field<int32_t> sellPrice;            // +0x14 sell + sellPerLv * (level - 1)
        rw::Field<int32_t> nextLevelExp;         // +0x18 BattleUtil::calcMedalExp results
        rw::Field<int32_t> levelExp;             // +0x1C
        rw::Field<int32_t> exp;                  // +0x20
        rw::Field<uint8_t> locked;               // +0x24 lock == 1 (favourite)
        rw::Field<uint8_t> isNew;                // +0x25 in AppState's new medal ids
        rw::Field<uint8_t> inDeck;               // +0x26 set on a keyblade
        const uint8_t pad_27[1];
        rw::Field<int32_t> upperCost;            // +0x28 -record.upperCost
        rw::Field<int32_t> attackUpperNumber;    // +0x2C
        rw::Field<int32_t> defenseUpperNumber;   // +0x30
        const uint8_t pad_34[4];
        rw::Field<uint32_t> userSkillIds[4];     // +0x38 two u64 skill ids
        rw::Field<uint32_t> skillCount;          // +0x48
        rw::Field<int32_t> getDatetime;          // +0x4C
        rw::Field<int32_t> burstUpperNumber;     // +0x50
        rw::Field<uint32_t> guiltFactor;         // +0x54
        guest::SharedPtr medal;                  // +0x58 shared_ptr<master::Medal>
    };

    static constexpr uint32_t INSTANCE = 0x10040F0;   // the singleton pointer

    static uint32_t getInstance();
    static void construct(uint32_t self);
    static void destruct(uint32_t self);
    // shared_ptr<UserMedalData> for user medal `id`; record = the medal record passed by value (on the stack),
    // overwritten from the user's list when the id is owned. medalId (non-zero) overrides the record's medal.
    static void getMedalData(guest::SharedPtr *out, uint32_t unusedThis, uint64_t id, int32_t medalId);
    // both return the StringBuffer's text (what StringBuffer::fset returns): callers use r0 as the path
    static uint32_t getMedalSmallImagePath(uint32_t stringBuffer, const guest::SharedPtr *data);
    static uint32_t getMedalLargeImagePath(uint32_t stringBuffer, const guest::SharedPtr *data);
    static uint32_t isFavoriteMedal(uint32_t unusedThis, const guest::SharedPtr *data);

    // the std::function thunks of getMedalData's two lambdas, both capturing shared_ptr<UserMedalData>
    // $_0: void(int attack, int defense)    $_1: void(int level, int exp, int levelExp, int nextLevelExp)
    struct StatusLambda {
        static void call(uint32_t self, const int32_t *attack, const int32_t *defense);
    };
    struct ExpLambda {
        static void call(uint32_t self, const int32_t *level, const int32_t *exp, const int32_t *levelExp,
                         const int32_t *nextLevelExp);
    };
};
RW_LAYOUT(MedalDataManager::UserMedalData, locked, 0x24);
RW_LAYOUT(MedalDataManager::UserMedalData, upperCost, 0x28);
RW_LAYOUT(MedalDataManager::UserMedalData, userSkillIds, 0x38);
RW_LAYOUT(MedalDataManager::UserMedalData, medal, 0x58);
RW_SIZE(MedalDataManager::UserMedalData, 0x60);

// Thunks of a std::function __func whose lambda captures one shared_ptr (vtable, object, control block):
// the same bodies for every such lambda, parametrised by the __func vtable (address stored in the object).
namespace scene_menu {
template <uint32_t VTABLE> struct SharedCaptureFunc {
    // ~__func
    static uint32_t destroyFunc(uint32_t self) {
        rw::save<uint32_t>(self, VTABLE);
        uint32_t ctrl = rw::load<uint32_t>(self + 8);
        if (ctrl) guest::release(ctrl);
        return self;
    }
    // deleting ~__func
    static void deleteFunc(uint32_t self) {
        rw::save<uint32_t>(self, VTABLE);
        uint32_t ctrl = rw::load<uint32_t>(self + 8);
        if (ctrl) guest::release(ctrl);
        guest::deleteObject(self);
    }
    // __clone(): a heap copy
    static uint32_t clone(uint32_t self) {
        uint32_t copy = guest::newObject(12);
        if (copy) {
            rw::save<uint32_t>(copy, VTABLE);
            rw::save<uint32_t>(copy + 4, rw::load<uint32_t>(self + 4));
            uint32_t ctrl = rw::load<uint32_t>(self + 8);
            rw::save<uint32_t>(copy + 8, ctrl);
            if (ctrl) guest::addRef(ctrl);
        }
        return copy;
    }
    // __clone(dst): copy in place
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        rw::save<uint32_t>(dst, VTABLE);
        rw::save<uint32_t>(dst + 4, rw::load<uint32_t>(self + 4));
        uint32_t ctrl = rw::load<uint32_t>(self + 8);
        rw::save<uint32_t>(dst + 8, ctrl);
        if (ctrl) guest::addRef(ctrl);
    }
    // destroy(): the captured shared_ptr's destructor
    static void destroy(uint32_t self) {
        uint32_t ctrl = rw::load<uint32_t>(self + 8);
        if (ctrl) guest::release(ctrl);
    }
    // destroy_deallocate()
    static void destroyDeallocate(uint32_t self) {
        uint32_t ctrl = rw::load<uint32_t>(self + 8);
        if (ctrl) guest::release(ctrl);
        guest::deleteObject(self);
    }
};

// target(type_info): the lambda (at +4) when the type_info's name pointer is the lambda's, else null.
inline uint32_t funcTarget(uint32_t self, uint32_t typeInfo, uint32_t typeName) {
    return rw::load<uint32_t>(typeInfo + 4) == typeName ? self + 4 : 0;
}
}  // namespace scene_menu
