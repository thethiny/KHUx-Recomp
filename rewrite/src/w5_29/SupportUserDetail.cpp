// The support list before a quest (SupportCharacterScrollView, rows of StageSupportList::SupportUser): tapping a
// supporter's medal fetches the user's detail (/user/detail, action 3) and shows the medal's info popup with that
// user's skills.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_executeUserDetailGet = 0x4C7DB9;   // (api, uint64 user, const function<void(bool)>&)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;        // (api, const function<void(bool)>&, bool)
constexpr uint32_t MEMCPY = 0x40E164;                            // __aeabi_memcpy(dst, src, n)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;     // (out, mgr, uint64 0, int 0, UserMedal::Medal by value)
constexpr uint32_t SafeBuffer_read = 0x45A269;                   // (sb, out, src, n, index)
constexpr uint32_t SceneUtil_openMedalInfoPopup = 0x466EF5;
constexpr uint32_t SkillVector_pushBackSlow = 0x487311;          // vector<UserSkill::Skill>::__push_back_slow_path(const&)
constexpr uint32_t SkillVector_dtor = 0x46BE37;
}  // namespace g

constexpr uint32_t SUPPORT_USER_SIZE = 0x128;
constexpr uint32_t SU_MEDAL = 0x48, MEDAL_SIZE = 0x48;          // SupportUser: the leader medal (UserMedal::Medal)
constexpr uint32_t SU_SKILL_COUNT = 0x90, SU_SKILLS = 0x98, SKILL_SIZE = 0x28;
constexpr uint32_t VIEW_BUSY = 0x684;          // SupportCharacterScrollView: a detail request is running
constexpr uint32_t API_USER_SB = 0x324;        // APIManager: the user's SafeBuffer (+0x54: a byte flag)
constexpr uint32_t VT_DetailDone = 0xF55F1C;   // $_0's lambda(bool) {SupportUser, view} (heap 0x138)
constexpr uint32_t VT_PopupClosed = 0xF55F48;  // its popup close callback {view}
constexpr uint32_t VT_DetailError = 0xF55F74;  // the error dialog's callback {}

// changeRow $_0 {-, view, -, SupportUser}: on touch end (no request running) /user/detail for the supporter
void medalTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t view = ld32(functor + 8);
    if (ld8(view + VIEW_BUSY)) return;
    st8(view + VIEW_BUSY, 1);
    call(g::AudioPlaySystemSEDecide, {});
    // the supporter is copied into the lambda's captures, and those moved into the heap callback
    Frame f;
    const uint32_t user = f.alloc(SUPPORT_USER_SIZE), captures = f.alloc(SUPPORT_USER_SIZE);
    call(g::MEMCPY, {user, functor + 0x10, SUPPORT_USER_SIZE});
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t lo = ld32(functor + 0x10), hi = ld32(functor + 0x14);
    call(g::MEMCPY, {captures, user, SUPPORT_USER_SIZE});
    const uint32_t cb = f.alloc(20);
    st32(cb + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x138});
    if (obj) {
        st32(obj, VT_DetailDone);
        call(g::MEMCPY, {obj + 8, captures, SUPPORT_USER_SIZE});
        st32(obj + 0x130, view);
    }
    st32(cb + 16, obj);
    call(g::APIManager_executeUserDetailGet, {api, 0, lo, hi, cb});
    functionDestroy(cb);
}

// its lambda(bool) __clone()
uint32_t detailDoneClone(uint32_t self) {
    const uint32_t obj = call(fn::operator_new, {0x138});
    if (obj) {
        st32(obj, VT_DetailDone);
        call(g::MEMCPY, {obj + 8, self + 8, 0x130});
    }
    return obj;
}

// lambda(bool ok) {SupportUser, view}: the medal's info popup with the supporter's skills, or the error dialog
void detailDone(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t view = ld32(functor + 0x130);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    Frame f;
    if (!ok) {
        if (!ld8(call(fn::APIManager_getInstance, {}) + 0x14)) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cb = newFunction(f, VT_DetailError, {});
        call(g::APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
        return;
    }
    const uint32_t user = functor + 8;
    const uint32_t skills = f.alloc(12);
    st32(skills, 0);
    st32(skills + 4, 0);
    st32(skills + 8, 0);
    for (uint32_t i = 0; i < ld32(user + SU_SKILL_COUNT); i++) {
        const uint32_t skill = user + SU_SKILLS + SKILL_SIZE * i;
        const uint32_t end = ld32(skills + 4);
        if (end == ld32(skills + 8)) {
            call(g::SkillVector_pushBackSlow, {skills, skill});
            continue;
        }
        uint32_t next = 0;
        if (end) {
            for (uint32_t k = 0; k < SKILL_SIZE; k += 4) st32(end + k, ld32(skill + k));
            next = end;
        }
        st32(skills + 4, next + SKILL_SIZE);
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
    const uint32_t found = f.alloc(8);
    {
        uint32_t w[6 + MEDAL_SIZE / 4] = {found, mdm, 0, 0, 0, 0};
        for (uint32_t k = 0; k < MEDAL_SIZE / 4; k++) w[6 + k] = ld32(user + SU_MEDAL + 4 * k);
        call(g::MedalDataManager_getMedalData,
             {w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11], w[12], w[13], w[14], w[15],
              w[16], w[17], w[18], w[19], w[20], w[21], w[22], w[23]});
    }
    const uint32_t medal = f.alloc(8);
    st32(medal, ld32(found));
    st32(medal + 4, ld32(found + 4));
    if (ld32(medal + 4)) call(fn::shared_add, {ld32(medal + 4)});
    const uint32_t sb = ld32(api + API_USER_SB);
    const uint32_t flag = f.alloc(4);
    call(g::SafeBuffer_read, {sb, flag + 3, ld32(sb + 0xC) + 0x54, 1, 0x54});
    const uint32_t closed = newFunction(f, VT_PopupClosed, {view});
    const uint32_t none = newEmptyFunction(f);
    call(g::SceneUtil_openMedalInfoPopup, {medal, ld8(flag + 3) ? 1u : 0u, 0, 0, none, skills, closed, 0});
    functionDestroy(closed);
    functionDestroy(none);
    if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
    if (ld32(found + 4)) call(fn::shared_release, {ld32(found + 4)});
    call(g::SkillVector_dtor, {skills});
}
}  // namespace

RW_FUNCTION(0x5054D0, "SupportCharacterScrollView::changeRow::$_0 operator() (medal: user detail)", medalTouched);
RW_FUNCTION(0x505654, "SupportCharacterScrollView::changeRow::$_0::lambda(bool) __clone()", detailDoneClone);
RW_FUNCTION(0x5056D8, "SupportCharacterScrollView::changeRow::$_0::lambda(bool) operator()", detailDone);
