// Batch #5 leftovers of the menu view bases: BaseEndlessrollView (the looping carousel: initMigration and its
// touch lambdas) and the BaseSlideView buttons (the profile button: the user detail request and the medal info
// popup it opens).
#include "../scene/SceneUi.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Touch_getLocation = 0x900E57;           // (Vec2 out, touch)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;     // (Vec2 out, node, const Vec2&)
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Rect_ctor = 0x8DF18B;
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t FunctionVV_call = 0x470E69;             // std::function<void()>::operator()
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;    // (mgr, float)
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t Sequence_create = 0x867999;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_executeUserDetailGet = 0x4C7DB9;  // (api, u64 user, const function<void(bool)>&)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;       // (api, const function<void(bool)>&, bool)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;    // (sret, mgr, u64 id, int, Medal by value)
constexpr uint32_t SceneUtil_openMedalInfoPopup = 0x466EF5;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t VectorSkill_pushBackSlow = 0x487311;
constexpr uint32_t VectorSkill_dtor = 0x46BE37;
constexpr uint32_t shared_add = 0xC8DBB5;
constexpr uint32_t shared_release = 0xC8DBD1;
}  // namespace g

constexpr uint32_t V_getContentSize = 0xAC;
constexpr uint32_t V_isVisible = 0xB4;
inline uint32_t fb(float f) { return rw::fbits(f); }

// ------------------------------------------------------------------------------------------------ BaseEndlessrollView
// fields: +0x31C the centred item, +0x330 the item count origin, +0x348 the item count, +0x34C the migration's
// direction, +0x34D a migration runs, +0x364 the touch's start x, +0x368/+0x369 dragging flags, +0x36A, +0x36B a
// hold, +0x36C the list settles, +0x374 the velocity, +0x37C..+0x388 the last four moves, +0x390 the list moved,
// +0x3A4 a row is pressed, +0x3A8 its start x, +0x3AC; std::function<void()> at +0x2B8 (moved)
constexpr uint32_t V_updateLayout = 0x288;   // (view, bool, bool)
constexpr uint32_t V_updateItems = 0x284;    // (view, bool)

// starts a one-item step to the left (false) or right (true) unless it would leave a short list's ends
uint32_t initMigration(uint32_t view, uint32_t right) {
    int32_t count = int32_t(ld32(view + 0x348));
    if (count <= 5) {
        int32_t at = int32_t(ld32(view + 0x31C));
        if (!(at > 0 || (right ^ 1) == 1)) return uint32_t(count);
        if (!(at < count - 1 || right == 1)) return uint32_t(count);
    }
    if (ld8(view + 0x34D) || count < 1) return uint32_t(count);
    st8(view + 0x34C, uint8_t(right));
    st8(view + 0x34D, 1);
    vcall(view, V_updateLayout, {view, 0, 0});
    vcall(view, V_updateItems, {view, 0});
    st8(view + 0x390, 1);
    st8(view + 0x369, 0);
    st8(view + 0x368, 0);
    st8(view + 0x36C, 1);
    stf(view + 0x374, ld8(view + 0x34C) ? 4.0f : -4.0f);
    return view + 0x374;
}

// the {view, row} rect test the row lambdas share: the row's rect from half its width
bool touchInRow(uint32_t touch, uint32_t widget) {
    Frame f;
    uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, widget, loc});
    call(g::Vec2_dtor, {loc});
    float x = ldf(vcall(widget, V_getContentSize, {widget})) * 0.5f;
    float w = ldf(vcall(widget, V_getContentSize, {widget}));
    float h = ldf(vcall(widget, V_getContentSize, {widget}) + 4);
    call(g::Rect_ctor, {rect, fb(x), 0, fb(w), fb(h)});
    bool inside = call(g::Rect_containsPoint, {rect, p}) != 0;
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {p});
    return inside;
}

// sub_515FF0, touch began {view, row, view, row, ...}: unless a tap animates (+0x688), the touch's start y
// (+0x68C); a touch inside the row presses it (+0x681)
uint32_t rowTouchBegan(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t touch = ld32(touchRef);
    uint32_t view = ld32(functor + 4);
    if (ld8(view + 0x688)) return 0;
    {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        st32(view + 0x68C, ld32(loc + 4));
        call(g::Vec2_dtor, {loc});
    }
    uint32_t widget = ld32(ld32(view + 0x51C) + 4 * ld32(functor + 8));
    if (!touchInRow(touch, widget)) return 0;
    st8(view + 0x681, 1);
    return 1;
}

// sub_515E30, touch moved: a pressed touch leaving its row ends the press through the ended body (sub_515AF4)
void rowTouchMoved(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t touch = ld32(touchRef);
    uint32_t view = ld32(functor + 4);
    if (!ld8(view + 0x681)) return;
    uint32_t widget = ld32(ld32(view + 0x51C) + 4 * ld32(functor + 8));
    if (touchInRow(touch, widget)) return;
    st8(view + 0x681, 0);
    call(0x515AF5, {functor + 0xC, touch});
}

// sub_517F94, touch ended on the carousel: a drag moves it by an eighth of the distance; the touch state is
// cleared and the fling speed set from the last four moves (none for a short list); then the moved callback
void carouselTouchEnded(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t view = ld32(functor + 4);
    if (ld8(view + 0x36C) || int32_t(ld32(view + 0x348)) < 2) return;
    uint32_t touch = ld32(touchRef);
    if (ld8(view + 0x369)) {
        float start = ldf(view + 0x364);
        Frame f;
        uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        stf(view + 0x318, ldf(view + 0x318) + (start - ldf(loc)) * -0.125f);
        call(g::Vec2_dtor, {loc});
        vcall(view, V_updateLayout, {view, 0, 0});
        vcall(view, V_updateItems, {view, 0});
    }
    st32(view + 0x3AC, 0);
    st8(view + 0x390, 1);
    st8(view + 0x368, 0);
    st8(view + 0x369, 0);
    st8(view + 0x36C, 0);
    if (int32_t(ld32(view + 0x378)) >= 2) {
        st32(view + 0x374, 0);
    } else {
        float speed = (((ldf(view + 0x37C) + ldf(view + 0x380)) + ldf(view + 0x384)) + ldf(view + 0x388)) * 0.03125f;
        float cur = ldf(view + 0x374);
        bool keep = (speed <= 0.0f || std::isnan(speed)) ? (speed <= cur) : (speed >= cur);
        if (!(keep || std::isnan(speed) || std::isnan(cur))) stf(view + 0x374, speed);
    }
    if (ld32(view + 0x2C8)) call(g::FunctionVV_call, {view + 0x2B8});
}

// sub_5188D0, a row's tap animation ended {view, index}: unless the list moved or a hold runs, the pressed row
// scrolls to the centre (speed from its distance) and the moved callback runs
uint32_t rowTapped(uint32_t functor) {
    uint32_t view = ld32(functor + 4);
    uint8_t moved = ld8(view + 0x390);
    st8(view + 0x36A, moved ^ 1);
    if (moved == 1 || !ld8(view + 0x3A4) || ld8(view + 0x36B)) return 1;
    st8(view + 0x390, 1);
    st8(view + 0x368, 0);
    st8(view + 0x369, 0);
    st8(view + 0x3A4, 0);
    st8(view + 0x36C, 1);
    vcall(view, V_updateLayout, {view, 0, 0});
    vcall(view, V_updateItems, {view, 0});
    stf(view + 0x374, float(int32_t(ld32(functor + 8) - ld32(view + 0x330))) * -4.0f);
    uint32_t callback = ld32(view + 0x2C8);
    if (!callback) return 0;
    return call(g::FunctionVV_call, {view + 0x2B8});
}

// sub_518A18, touch moved: a press that drifted more than 20 pixels sideways is no longer a tap
void rowTouchDrift(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t view = ld32(functor + 4);
    if (!ld8(view + 0x3A4)) return;
    float start = ldf(view + 0x3A8);
    Frame f;
    uint32_t loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, ld32(touchRef)});
    float x = ldf(loc);
    call(g::Vec2_dtor, {loc});
    float d = std::fabs(start - x);
    if (!(d <= 20.0f || std::isnan(d))) st8(view + 0x3A4, 0);
}

// ------------------------------------------------------------------------------------------------ BaseSlideView
// sub_5247B8, a button's touch lambda {.., button, view, a, b}: a release on a shown button of a shown view with a
// target plays the decide sound, keeps (a, b) at +0x6E0, blocks taps for the popup time and calls the view's
// action ($_ {view}) after it
constexpr uint32_t VT_SLIDE_ACTION = 0xF56F18;
void slideButtonTouch(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    uint32_t button = ld32(functor + 8);
    uint32_t view = ld32(functor + 0xC);
    if (vcall(button, V_isVisible, {button}) != 1) return;
    if (vcall(view, V_isVisible, {view}) != 1) return;
    call(g::AudioPlaySystemSEDecide, {});
    if (!ld32(view + 0x678)) return;
    st32(view + 0x6E4, ld32(functor + 0x14));
    st32(view + 0x6E0, ld32(functor + 0x10));
    uint32_t mgr = call(g::SceneManager_getInstance, {});
    call(g::SceneManager_openNoTapScreen, {mgr, ld32(call(g::SceneManager_getInstance, {}) + 0x300)});
    uint32_t delay = call(g::DelayTime_create, {ld32(call(g::SceneManager_getInstance, {}) + 0x300)});
    Frame f;
    uint32_t fo = newFunction(f, VT_SLIDE_ACTION, {view});
    uint32_t done = call(g::CallFunc_create, {fo});
    call(g::Node_runAction, {view, call(g::Sequence_create, {delay, done, 0})});
    functionDestroy(fo);
}

// sub_524A84, the profile button's touch lambda {.., button, view, user (0x128 bytes: id, medal, skills)}: a
// release on a shown button of a shown view requests the user's details with the loading indicator on; the
// request's callback ($_ below) keeps a copy of the user
constexpr uint32_t VT_USER_DETAIL = 0xF56F70;
constexpr uint32_t USER_SIZE = 0x128;
void profileButtonTouch(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    uint32_t button = ld32(functor + 8);
    uint32_t view = ld32(functor + 0xC);
    if (vcall(button, V_isVisible, {button}) != 1) return;
    if (vcall(view, V_isVisible, {view}) != 1) return;
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    uint32_t copy = f.alloc(USER_SIZE), copy2 = f.alloc(USER_SIZE);
    call(g::MEMCPY, {copy, functor + 0x10, USER_SIZE});
    call(g::SceneManager_enableLoadingIndicator, {call(g::SceneManager_getInstance, {}), 1});
    uint32_t api = call(g::APIManager_getInstance, {});
    uint32_t idLo = ld32(functor + 0x10), idHi = ld32(functor + 0x14);
    call(g::MEMCPY, {copy2, copy, USER_SIZE});
    uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    uint32_t obj = call(fn::operator_new, {USER_SIZE + 8});
    if (obj) {
        st32(obj, VT_USER_DETAIL);
        call(g::MEMCPY, {obj + 8, copy2, USER_SIZE});
    }
    st32(fo + 16, obj);
    call(g::APIManager_executeUserDetailGet, {api, 0, idLo, idHi, fo});
    functionDestroy(fo);
}

// sub_524C34: that lambda's __clone()
uint32_t userDetailClone(uint32_t functor) {
    uint32_t obj = call(fn::operator_new, {USER_SIZE + 8});
    if (obj) {
        st32(obj, VT_USER_DETAIL);
        call(g::MEMCPY, {obj + 8, functor + 8, USER_SIZE});
    }
    return obj;
}

// sub_524CB8, the user detail callback (functor: the user copy at +8): the loading indicator off; on success
// the user's medal info popup with its skills (no buttons); a failure shows the error dialog when the request
// asks for one ($_ {} on close)
constexpr uint32_t VT_DETAIL_ERROR = 0xF56F9C;
constexpr uint32_t U_MEDAL = 0x50, U_SKILL_COUNT = 0x98, U_SKILLS = 0xA0, SKILL_SIZE = 0x28;
void userDetailDone(uint32_t functor, uint32_t okRef) {
    uint8_t ok = ld8(okRef);
    call(g::SceneManager_enableLoadingIndicator, {call(g::SceneManager_getInstance, {}), 0});
    // the original frame: getMedalData and openMedalInfoPopup take arguments on the stack
    guest::Frame frame(0xD4);
    uint32_t sp = frame.addr();
    if (!ok) {
        uint32_t api = call(g::APIManager_getInstance, {});
        if (!ld8(api + 0x14)) return;
        api = call(g::APIManager_getInstance, {});
        uint32_t fo = sp + 0xB0;
        st32(fo + 16, fo);
        st32(fo, VT_DETAIL_ERROR);
        call(g::APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
        return;
    }
    uint32_t skills = sp + 0x70;   // std::vector<UserSkill::Skill>
    st32(skills, 0);
    st32(skills + 4, 0);
    st32(skills + 8, 0);
    for (uint32_t i = 0; i < ld32(functor + U_SKILL_COUNT); i++) {
        uint32_t skill = functor + U_SKILLS + SKILL_SIZE * i;
        uint32_t end = ld32(skills + 4);
        if (end == ld32(skills + 8)) {
            call(g::VectorSkill_pushBackSlow, {skills, skill});
            continue;
        }
        if (end)
            for (uint32_t k = 0; k < SKILL_SIZE; k += 4) st32(end + k, ld32(skill + k));
        st32(skills + 4, end + SKILL_SIZE);
    }
    uint32_t api = call(g::APIManager_getInstance, {});
    uint32_t mdm = call(g::MedalDataManager_getInstance, {});
    st32(sp, 0);
    for (uint32_t k = 0; k < 0x48; k += 4) st32(sp + 8 + k, ld32(functor + U_MEDAL + k));
    uint32_t data = sp + 0x68;     // shared_ptr<UserMedalData>
    rw::call_exact(g::MedalDataManager_getMedalData, {data, mdm, 0, 0});
    uint32_t copy = sp + 0x60;
    st32(copy, ld32(data));
    st32(copy + 4, ld32(data + 4));
    if (ld32(copy + 4)) call(g::shared_add, {ld32(copy + 4)});
    uint32_t user = ld32(api + 0x324);
    call(g::SafeBuffer_read, {user, sp + 0x7F, ld32(user + 0xC) + 0x54, 1, 0x54});
    uint32_t flag = ld8(sp + 0x7F) ? 1 : 0;
    uint32_t onButton = sp + 0x98, onClose = sp + 0x80;
    st32(onButton + 16, 0);
    st32(onClose + 16, 0);
    st32(sp + 0xC, 0);
    st32(sp + 8, onClose);
    st32(sp + 4, skills);
    st32(sp, onButton);
    rw::call_exact(g::SceneUtil_openMedalInfoPopup, {copy, flag, 0, 0});
    functionDestroy(onClose);
    functionDestroy(onButton);
    if (ld32(copy + 4)) call(g::shared_release, {ld32(copy + 4)});
    if (ld32(data + 4)) call(g::shared_release, {ld32(data + 4)});
    call(g::VectorSkill_dtor, {skills});
}
}  // namespace

RW_FUNCTION(0x517E88, "BaseEndlessrollView::initMigration(bool)", initMigration);
RW_FUNCTION(0x515FF0, "BaseEndlessrollView row touch began (sub_515FF0)", rowTouchBegan);
RW_FUNCTION(0x515E30, "BaseEndlessrollView row touch moved (sub_515E30)", rowTouchMoved);
RW_FUNCTION(0x517F94, "BaseEndlessrollView touch ended (sub_517F94)", carouselTouchEnded);
RW_FUNCTION(0x5188D0, "BaseEndlessrollView row tapped (sub_5188D0)", rowTapped);
RW_FUNCTION(0x518A18, "BaseEndlessrollView row touch moved: drift (sub_518A18)", rowTouchDrift);
RW_FUNCTION(0x5247B8, "BaseSlideView button touch lambda (sub_5247B8)", slideButtonTouch);
RW_FUNCTION(0x524A84, "BaseSlideView profile button touch lambda (sub_524A84)", profileButtonTouch);
RW_FUNCTION(0x524C34, "BaseSlideView user detail lambda __clone() (sub_524C34)", userDetailClone);
RW_FUNCTION(0x524CB8, "BaseSlideView user detail callback (sub_524CB8)", userDetailDone);
