// BookAnimation: a page-turning book (AdventureTopAnim2 armatures: a left and a right cover and n pages turning in
// sequence). Nothing in 1.0.1 creates one, so this is never called (unverified).
//   +0 the page turning, +4 busy, +8 its elapsed time, +0xC the time per page, +0x10 the next page to raise,
//   +0x14 the raise timer, +0x18 the page artwork (A/B/C in turn), +0x1C / +0x20 the bottom armatures, +0x24 the
//   pages (cocos2d::Vector<Armature*>)
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7, ArmatureVector_clear = 0x50A459, ArmatureVector_pushBack = 0x50A48D;
constexpr uint32_t ArmatureAnimation_setMovementEventCallFunc = 0x7A3C69;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_move = 0x47F905;
constexpr uint32_t VT_RightPage = 0xF56184, VT_LeftPage = 0xF56158;   // {pad, page, count, this, pad, done} (heap 0x30)
constexpr uint32_t S_File = 0xDFD500, S_Armature = 0xDFD530, S_RightBottom = 0xDFD542, S_LeftBottom = 0xDFD55A;
constexpr uint32_t S_PageFmt = 0xDFD572;
constexpr uint32_t S_BLeft = 0xDFD54F, S_CLeft = 0xDFD57A, S_ALeft = 0xDFD585;
constexpr uint32_t S_CRight = 0xDFD566, S_ARight = 0xDFD590, S_BRight = 0xDFD59C;
constexpr uint32_t V_setLocalZOrder = 0x0C, V_setVisible = 0xB0, V_isVisible = 0xB4, V_addChild = 0xF0;
constexpr uint32_t V_stopAll = 0x138, V_init = 0x154, V_getAnimation = 0x298, V_getLocalZOrder = 0x18;
constexpr uint32_t A_play = 0x68, A_resume = 0x0C, A_pause = 0x10, A_setSpeedScale = 0x5C;
constexpr uint32_t F_PAGE = 0, F_BUSY = 4, F_TIME = 8, F_STEP = 0xC, F_RAISE = 0x10, F_RAISE_TIME = 0x14;
constexpr uint32_t F_SET = 0x18, F_LEFT = 0x1C, F_RIGHT = 0x20, F_PAGES = 0x24;

uint32_t newArmature() {
    Frame f;
    const uint32_t s = newStr(f, S_Armature, 0x11);
    const uint32_t a = call(Armature_create, {s});
    strFree(s);
    return a;
}

void play(uint32_t armature, uint32_t name, uint32_t len) {
    const uint32_t anim = vcall(armature, V_getAnimation, {armature});
    Frame f;
    const uint32_t s = newStr(f, name, len);
    vcall(anim, A_play, {anim, s, 0xFFFFFFFF, 0xFFFFFFFF});
    strFree(s);
}
// play an animation by a name string the caller built
void playStr(uint32_t armature, uint32_t str) {
    const uint32_t anim = vcall(armature, V_getAnimation, {armature});
    vcall(anim, A_play, {anim, str, 0xFFFFFFFF, 0xFFFFFFFF});
}
void animCall(uint32_t armature, uint32_t slot) {
    const uint32_t anim = vcall(armature, V_getAnimation, {armature});
    vcall(anim, slot, {anim});
}
void speed(uint32_t armature, float s) {
    const uint32_t anim = vcall(armature, V_getAnimation, {armature});
    vcall(anim, A_setSpeedScale, {anim, fbits(s)});
}

uint32_t pageCount(uint32_t self) { return (ld32(self + F_PAGES + 4) - ld32(self + F_PAGES)) >> 2; }
uint32_t pageAt(uint32_t self, uint32_t i) { return ld32(ld32(self + F_PAGES) + 4 * i); }

struct Name { uint32_t s, len; };
// the three page artworks of a side, in turn
const Name LEFT_SET[3] = {{S_BLeft, 10}, {S_CLeft, 10}, {S_ALeft, 10}};
const Name RIGHT_SET[3] = {{S_CRight, 11}, {S_ARight, 11}, {S_BRight, 11}};

// the page after the turning one slows to 1.5x once it is the last or not shown yet
void slowNext(uint32_t self) {
    const uint32_t last = pageCount(self) - 1;
    const uint32_t page = ld32(self + F_PAGE);
    if (last == page) {
        speed(pageAt(self, last), 1.5f);
    } else if (last > page && !vcall(pageAt(self, page + 1), V_isVisible, {pageAt(self, page + 1)})) {
        speed(pageAt(self, page), 1.5f);
    }
}

// turn `count` pages: `first` / `last` the bottom armatures, `pageSet` the pages' artwork, `step` the artwork turn
uint32_t turn(uint32_t self, uint32_t count, uint32_t done, uint32_t first, const Name *firstSet, const Name *pageSet,
              uint32_t last, const Name *lastSet, bool forward, uint32_t vt) {
    if (int32_t(count) < 1 || ld8(self + F_BUSY)) return 0;
    // the nine names are built up front like the original (left set, right set, then the pages' set; the 11-char
    // right names live on the heap) and destroyed in reverse at the end, so the heap is used the same way
    Frame names;
    uint32_t str[9];
    for (int k = 0; k < 3; k++) str[k] = newStr(names, LEFT_SET[k].s, LEFT_SET[k].len);
    for (int k = 0; k < 3; k++) str[3 + k] = newStr(names, RIGHT_SET[k].s, RIGHT_SET[k].len);
    for (int k = 0; k < 3; k++) str[6 + k] = newStr(names, pageSet[k].s, pageSet[k].len);
    auto nameOf = [&](const Name *set, int32_t i) {
        return set == LEFT_SET ? str[i] : str[3 + i];        // the first two arrays (the pages use str[6..8])
    };
    for (uint32_t k = 0; int32_t(ld32(self + F_PAGES + 4) - ld32(self + F_PAGES)) >= 1 && k < pageCount(self); k++)
        vcall(pageAt(self, k), V_setVisible, {pageAt(self, k), 0});
    st32(self + F_TIME, 0);
    st8(self + F_BUSY, 1);
    st32(self + F_PAGE, 0);
    st32(self + F_RAISE, 0);
    st32(self + F_RAISE_TIME, 0);
    {
        playStr(ld32(self + first), nameOf(firstSet, int32_t(ld32(self + F_SET)) % 3));
        animCall(ld32(self + first), A_resume);
    }
    int32_t z = int32_t(ld32(self + F_PAGES + 4) - ld32(self + F_PAGES)) >> 1;
    for (uint32_t i = 0; i < pageCount(self); i++) {
        const uint32_t page = pageAt(self, i);
        vcall(page, V_setVisible, {page, 1});
        playStr(page, str[6 + int32_t(ld32(self + F_SET)) % 3]);
        animCall(page, A_resume);
        speed(page, 3.0f);
        vcall(page, V_setLocalZOrder, {page, uint32_t(z)});
        const uint32_t anim = vcall(page, V_getAnimation, {page});
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(function_vv_copy, {copy, done});
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, vt);
            st32(obj + 8, i);
            st32(obj + 0xC, count);
            st32(obj + 0x10, self);
            call(function_vv_move, {obj + 0x18, copy});
        }
        st32(fo + 16, obj);
        call(ArmatureAnimation_setMovementEventCallFunc, {anim, fo});
        functionDestroy(fo);
        functionDestroy(copy);
        const int32_t set = int32_t(ld32(self + F_SET));
        st32(self + F_SET, forward ? uint32_t((set + 1) % 3) : uint32_t(set >= 1 ? set - 1 : 2));
        z--;
        if (i + 1 >= count) break;
    }
    animCall(pageAt(self, 0), A_pause);
    slowNext(self);
    {
        playStr(ld32(self + last), nameOf(lastSet, int32_t(ld32(self + F_SET)) % 3));
        animCall(ld32(self + last), A_resume);
    }
    for (int k = 8; k >= 0; k--) strFree(str[k]);
    return 1;
}
}  // namespace

namespace book_animation {

uint32_t init(uint32_t self, uint32_t node, uint32_t pages, uint32_t reset) {
    if (int32_t(pages) < 1) return 0;
    if (reset == 1) vcall(node, V_stopAll, {node});
    {
        const uint32_t mgr = call(ArmatureDataManager_getInstance, {});
        Frame f;
        const uint32_t s = newStr(f, S_File, 0x2F);
        call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        strFree(s);
    }
    const Name sides[2][2] = {{{S_RightBottom, 0xC}, {S_BLeft, 10}}, {{S_LeftBottom, 0xB}, {S_CRight, 11}}};
    for (uint32_t k = 0; k < 2; k++) {
        const uint32_t field = k ? F_RIGHT : F_LEFT;
        st32(self + field, newArmature());
        {
            Frame f;
            const uint32_t s = newStr(f, sides[k][0].s, sides[k][0].len);
            vcall(ld32(self + field), V_init, {ld32(self + field), s});
            strFree(s);
        }
        vcall(node, V_addChild, {node, ld32(self + field)});
        vcall(ld32(self + field), V_setLocalZOrder, {ld32(self + field), 0});
        play(ld32(self + field), sides[k][1].s, sides[k][1].len);
        animCall(ld32(self + field), A_resume);
    }
    call(ArmatureVector_clear, {self + F_PAGES});
    for (uint32_t i = 0; i < pages; i++) {
        const uint32_t page = newArmature();
        Frame f;
        const uint32_t name = newStr(f, stringBuffer(f, {S_PageFmt, i}));
        vcall(page, V_init, {page, name});
        strFree(name);
        vcall(node, V_addChild, {node, page});
        vcall(page, V_setVisible, {page, 0});
        call(ArmatureVector_pushBack, {self + F_PAGES, page});
    }
    st8(self + F_BUSY, 0);
    return 0;
}
RW_FUNCTION(0x509FFC, "BookAnimation::init", book_animation::init);

uint32_t rightAnimation(uint32_t self, uint32_t, uint32_t count, uint32_t done) {
    return turn(self, count, done, F_LEFT, LEFT_SET, RIGHT_SET, F_RIGHT, RIGHT_SET, true, VT_RightPage);
}
RW_FUNCTION(0x50A4F4, "BookAnimation::rightAnimation", book_animation::rightAnimation);

uint32_t leftAnimation(uint32_t self, uint32_t, uint32_t count, uint32_t done) {
    return turn(self, count, done, F_RIGHT, RIGHT_SET, LEFT_SET, F_LEFT, LEFT_SET, false, VT_LeftPage);
}
RW_FUNCTION(0x50AA84, "BookAnimation::leftAnimation", book_animation::leftAnimation);

// the pages start one after another (every `step` seconds) and are raised in z order in turn
uint32_t update(uint32_t self, uint32_t dtBits) {
    uint32_t result = ld8(self + F_BUSY);
    if (!result) return result;
    const float dt = bitsf(dtBits);
    const uint32_t page = ld32(self + F_PAGE);
    if (pageCount(self) - 1 > page && vcall(pageAt(self, page + 1), V_isVisible, {pageAt(self, page + 1)}) == 1) {
        const float t = ldf(self + F_TIME) + dt;
        stf(self + F_TIME, t);
        if (!(ldf(self + F_STEP) > t)) {
            st32(self + F_TIME, 0);
            const uint32_t next = ld32(self + F_PAGE) + 1;
            st32(self + F_PAGE, next);
            animCall(pageAt(self, next), A_pause);
            slowNext(self);
        }
    }
    result = ld32(self + F_PAGES);
    const uint32_t raise = ld32(self + F_RAISE);
    if (pageCount(self) - 1 > raise) {
        result = vcall(pageAt(self, raise + 1), V_isVisible, {pageAt(self, raise + 1)});
        if (result == 1) {
            const float t = ldf(self + F_RAISE_TIME) + dt;
            stf(self + F_RAISE_TIME, t);
            const float due = ldf(self + F_STEP) * float(int32_t(ld32(self + F_RAISE))) + 0.083333f;
            result = due > t ? 1 : 0;
            if (!(due > t)) {
                const uint32_t lower = pageAt(self, ld32(self + F_RAISE));
                const uint32_t upper = pageAt(self, ld32(self + F_RAISE) + 1);
                vcall(upper, V_setLocalZOrder, {upper, vcall(lower, V_getLocalZOrder, {lower}) + 1});
                result = ld32(self + F_RAISE) + 1;
                st32(self + F_RAISE, result);
            }
        }
    }
    return result;
}
RW_FUNCTION(0x50B01C, "BookAnimation::update", book_animation::update);

}  // namespace book_animation
