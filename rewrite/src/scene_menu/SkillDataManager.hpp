// SkillDataManager: the UI-side view of an owned skill (UserSkillData). A singleton (0x404 bytes, unused by the
// functions here).
#pragma once

#include "rw/rw.hpp"
#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

struct SkillDataManager {
    // SafeBuffer(0x28) + shared_ptr<master::Skill>. Data positions below.
    struct UserSkillData : SafeBuffer {
        guest::SharedPtr skill;                // +0x20
        static constexpr uint32_t VTABLE = 0xFA9870;   // _ZTVN16SkillDataManager13UserSkillDataE + 8
        static constexpr uint32_t USER_SKILL_ID = 0;   // u64
        static constexpr uint32_t LEVEL = 8;
        static constexpr uint32_t EXP = 0x10;
        static constexpr uint32_t LEVEL_EXP = 0x14;    // experience curve value at `level`
        static constexpr uint32_t PREV_LEVEL_EXP = 0x18;   // ... at level - 1
        static constexpr uint32_t VALID = 0x1C;        // byte, always 1
        static constexpr uint32_t USER_MEDAL_ID = 0x20;    // u64: the medal it is attached to
        static UserSkillData *destruct(UserSkillData *self);
        static void destructDelete(UserSkillData *self);
    };

    static constexpr uint32_t INSTANCE = 0x10040F8;

    static uint32_t getInstance();
    static void construct(uint32_t self);
    static void destruct(uint32_t self);
    // shared_ptr<UserSkillData> for user skill `id`; the UserSkill::Skill record (by value) and a bool follow on
    // the stack. The owned record replaces the argument; without one (and keepId false) the id is cleared.
    static void getData(guest::SharedPtr *out, uint32_t unusedThis, uint64_t id);
    static void getImagePath(uint32_t stringBuffer, const guest::SharedPtr *data);
};
RW_SIZE(SkillDataManager::UserSkillData, 0x28);
