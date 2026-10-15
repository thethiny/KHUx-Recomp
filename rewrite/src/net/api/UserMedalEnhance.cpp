#include "UserMedalEnhance.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t ENHANCE_SHARED_PTR_CTOR = 0x4A1F05;   // std::shared_ptr<UserMedalEnhance>::shared_ptr(...)
constexpr uint32_t EVOLVE_SHARED_PTR_CTOR = 0x4A1FFD;    // std::shared_ptr<UserMedalEvolve>::shared_ptr(...)
constexpr uint32_t MEDAL_INIT = 0x49FFF9;          // UserMedal::Medal::init(const GenericValue&) -> 1 = ok
constexpr uint32_t SKILL_INIT = 0x4A0619;          // UserSkill::Skill::init(const GenericValue&) -> 1 = ok
constexpr uint32_t SKILLS_PUSH_BACK_SLOW = 0x4B00A5;   // vector<UserSkill::Skill>::__push_back_slow_path(Skill&&)
constexpr uint32_t SKILLS_DTOR = 0x46BE37;         // ~__vector_base<UserSkill::Skill>

// Medal::init on a JSON member that must be an object.
bool initMedal(const uint8_t *medal, const JsonValue &json) {
    if (!json.IsObject()) return false;
    return rw::call(MEDAL_INIT, {rw::guest_addr(medal), rw::guest_addr(&json)}) == 1;
}

// vector<Skill>::push_back(std::move(skill)) as inlined: copy in place while there is room (the 40 bytes as
// words 3,2,1,0, 7,6,5,4, 8,9), else the slow path. With no storage at all (end == null) the copy is skipped
// but end still moves to 40.
void pushSkill(StdVector *skills, uint32_t skill) {
    uint32_t end = skills->end;
    if (end >= skills->capEnd) {
        rw::call(SKILLS_PUSH_BACK_SLOW, {rw::guest_addr(skills), skill});
        return;
    }
    if (end) {
        static constexpr uint32_t kOrder[] = {3, 2, 1, 0, 7, 6, 5, 4, 8, 9};
        for (uint32_t w : kOrder) rw::save<uint32_t>(end + 4 * w, rw::load<uint32_t>(skill + 4 * w));
    }
    skills->end = end + USER_SKILL_RECORD;
}

bool readEnhance(UserMedalEnhance *self, const JsonValue &response, const GuestFrame &frame) {
    if (!initMedal(self->beforeMedal, response["beforeEnhanceUserMedal"])) return false;
    if (!initMedal(self->afterMedal, response["afterEnhanceUserMedal"])) return false;
    const JsonValue &effect = response["enhanceEffect"];
    if (!effect.IsInt()) return false;
    self->enhanceEffect = effect.GetInt();

    const JsonValue &skills = response["overwriteSkills"];
    if (!skills.IsArray()) return false;
    uint32_t n = skills.Size();
    uint32_t k = 0;
    for (; k < n; k++) {
        // the Skill local at +0x30 of the frame
        if (rw::call(SKILL_INIT, {frame.addr(0x30), rw::guest_addr(&skills.At(k))}) != 1) break;
        pushSkill(&self->overwriteSkills, frame.addr(0x30));
    }
    if (k != n) return false;

    const JsonValue &success = response["successOverwrite"];
    if (!success.IsInt()) return false;
    self->successOverwrite = success.GetInt() != 0 ? 1 : 0;
    return true;
}
}  // namespace

uint32_t UserMedalEnhance::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x78);   // original frame: 5 pushed registers + 0x64 of locals
    UserMedalEnhance *self = nullptr;
    if (response->IsObject()) {
        self = newObject<UserMedalEnhance>(sizeof(UserMedalEnhance));
        self->overwriteSkills.begin = 0;
        self->overwriteSkills.end = 0;
        self->overwriteSkills.capEnd = 0;
        if (!readEnhance(self, *response, frame)) {
            rw::call(SKILLS_DTOR, {rw::guest_addr(&self->overwriteSkills)});
            guestDelete(rw::guest_addr(self));
            self = nullptr;
        }
    }
    rw::call(ENHANCE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A1CF8, "hole::network::api::UserMedalEnhance::parse", UserMedalEnhance::parse);

uint32_t UserMedalEvolve::parse(SharedPtr *out, const JsonValue *response) {
    UserMedalEvolve *self = nullptr;
    if (response->IsObject()) {
        self = newObject<UserMedalEvolve>(sizeof(UserMedalEvolve));
        // the after medal only has to give a non-zero init result (the before medal exactly 1)
        bool ok = initMedal(self->beforeMedal, (*response)["beforeEvolveUserMedal"]);
        if (ok) {
            const JsonValue &after = (*response)["afterEvolveUserMedal"];
            ok = after.IsObject() &&
                 rw::call(MEDAL_INIT, {rw::guest_addr(self->afterMedal), rw::guest_addr(&after)}) != 0;
        }
        if (!ok) {
            guestDelete(rw::guest_addr(self));
            self = nullptr;
        }
    }
    return rw::call(EVOLVE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A1F60, "hole::network::api::UserMedalEvolve::parse", UserMedalEvolve::parse);

}  // namespace hole::network::api
