// stage::skill::invoke: whether the skill of the medal `medalUid` (a user medal, or the support medal when the uid
// is not the user's) fires now: its master row's timing must match, its condition must hold, and a random roll
// must fall under its chance (base + per level, in 1/10000). Returns the master::Skill row, or null.
#include "Common.hpp"

using namespace w36;

namespace stage_skill {

namespace {
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t API_USER_SKILLS = 0xDC;              // APIManager: shared_ptr<UserSkill> (a SafeBuffer)
constexpr uint32_t USER_SKILL_INDEX = 0x24;             // UserSkill: unordered_map<uint64 medal uid, int index>
constexpr uint32_t HASH_FIND_U64 = 0x490275;            // unordered_map<unsigned long long, int>::find
constexpr uint32_t SafeBuffer_read = 0x45A269;          // (sb, dst, src, n, index)
constexpr uint32_t BattleUtil_getSupportMedal = 0x497011;   // (UserMedal::Medal& out, UserSkill::Skill& out)
constexpr uint32_t Skill_create = 0x78BDE9;             // master::Skill::create(shared_ptr out, int id)
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformFloat = 0x492261;   // (rnd, float lo, float hi, unsigned* out)
constexpr uint32_t SKILL_RECORD = 0x28;                 // UserSkill::Skill: 40 bytes (skill id +8, level +0xC)
// master::Skill data positions
constexpr uint32_t SKILL_CONDITION = 0xE4;
constexpr uint32_t SKILL_RATE = 0xE8;                   // chance at level 1 (1/10000)
constexpr uint32_t SKILL_RATE_PER_LEVEL = 0xEC;
constexpr uint32_t SKILL_TIMING = 0xF0;

// a 32-bit field of a master row (SafeBuffer::read into the stack slot `dst`, as the original does)
int32_t readField(uint32_t row, uint32_t dst, uint32_t pos) {
    rw::call_exact(SafeBuffer_read, {row, dst, ld32(row + 12) + pos, 4, pos});
    return int32_t(ld32(dst));
}
}  // namespace

void invoke(uint32_t out, uint64_t medalUid, int32_t timing, uint32_t afterAttack, uint32_t onDamage) {
    Frame f;
    const uint32_t sp = f.allocExact(0xF8);     // the original frame (arguments 5+ of SafeBuffer::read at [sp])
    const uint32_t api = call(APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_USER_SKILLS + 4);
    const uint32_t skills = ld32(api + API_USER_SKILLS);
    if (ctrl) call(fn::shared_add, {ctrl});
    const uint32_t key = sp + 0x78;
    st32(key, uint32_t(medalUid));
    st32(key + 4, uint32_t(medalUid >> 32));
    uint32_t it = call(HASH_FIND_U64, {skills + USER_SKILL_INDEX, key});
    int32_t index = it ? int32_t(ld32(it + 0x10)) : -1;
    if (medalUid == 0) {
        st32(out, 0);
        st32(out + 4, 0);
    } else {
        int32_t skillId = 0, level = 0;
        bool found = true;
        if (index < 0) {
            const uint32_t skill = sp + 0x50;
            if (call(BattleUtil_getSupportMedal, {sp + 0x90, skill}) != 1 || int32_t(ld32(skill + 8)) <= 0) {
                found = false;
            } else {
                skillId = int32_t(ld32(skill + 8));
                level = int32_t(ld32(skill + 0xC));
            }
        } else {
            const uint32_t pos = SKILL_RECORD * uint32_t(index);
            const uint32_t skill = sp + 0x28;
            rw::call_exact(SafeBuffer_read, {skills, skill, ld32(skills + 12) + pos, SKILL_RECORD, pos});
            skillId = int32_t(ld32(skill + 8));
            level = int32_t(ld32(skill + 0xC));
        }
        if (!found) {
            st32(out, 0);
            st32(out + 4, 0);
        } else {
            const int32_t levelSteps = level - 1;
            const uint32_t row = sp + 0x20;         // shared_ptr<const master::Skill>
            call(Skill_create, {row, uint32_t(skillId)});
            bool fires = false;
            if (readField(ld32(row), sp + 0x8C, SKILL_TIMING) == timing) {
                int32_t condition = readField(ld32(row), sp + 0x88, SKILL_CONDITION);
                bool ok;
                switch (condition) {
                case 0: ok = false; break;
                case 3: ok = timing == 4; break;
                case 4: ok = onDamage != 0; break;
                case 5: ok = afterAttack != 0; break;
                case 7: ok = !(onDamage == 1 || afterAttack == 1); break;
                default: ok = true; break;
                }
                if (ok) {
                    int32_t rate = readField(ld32(row), sp + 0x84, SKILL_RATE);
                    int32_t perLevel = readField(ld32(row), sp + 0x80, SKILL_RATE_PER_LEVEL);
                    float chance = float(rate) / 10000.0f + float(levelSteps) * (float(perLevel) / 10000.0f);
                    uint32_t rnd = call(Random_getInstance, {});
                    float roll = rw::bitsf(call(Random_generateUniformFloat, {rnd, rw::fbits(0.0f), rw::fbits(1.0f), 0}));
                    fires = !(chance < roll);       // chance >= roll, or unordered
                }
            }
            if (fires) {
                st32(out, ld32(row));
                st32(out + 4, ld32(row + 4));
                st32(row, 0);
                st32(row + 4, 0);
            } else {
                st32(out, 0);
                st32(out + 4, 0);
            }
            sharedRelease(row);
        }
    }
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x748EE8, "stage::skill::invoke(unsigned long long, InvokeShowTiming, bool, bool)", invoke);

}  // namespace stage_skill
