// Small SceneDeckEdit / IMedalSortFilter pieces: createScene, setTouchEnabledAllButton, and lambda copies
// (__clone) of the medal sort popup and playKeybladeStepUp.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create, SceneDeckEdit_create = 0x57AFD5;
constexpr uint32_t libc_memcpy = 0x40E164;
constexpr uint32_t function_vb_copy = 0x45C84D;            // std::function<void(bool)>(const&)
constexpr uint32_t V_addChild = 0xF0, V_setTouchEnabled = 0x29C;
constexpr uint32_t VT_SortClose = 0xF62B80;                // the sort popup's close handler (0xE4 bytes)
constexpr uint32_t VT_SortRelease = 0xF62CB4;              // Release (0x68 bytes)
constexpr uint32_t VT_SortUp = 0xF63050, VT_SortDown = 0xF6307C;   // Up / Down (0x18 bytes)
constexpr uint32_t VT_StepUpEnd = 0xF633EC;                // playKeybladeStepUp's lambda {w, shared_ptr}
// SceneDeckEdit: the four bottom buttons
constexpr uint32_t F_BUTTONS = 0x310;

uint32_t addShared(uint32_t ctrl) { return call(fn::shared_add, {ctrl}); }

// the Up / Down sort lambda {state copy (12 bytes), word, shared_ptr ctrl} copied into p
uint32_t cloneSortOrder(uint32_t self, uint32_t p, uint32_t vt) {
    if (!p) return self;
    st32(p, vt);
    call(libc_memcpy, {p + 4, self + 4, 0xC});
    st32(p + 0x10, ld32(self + 0x10));
    const uint32_t ctrl = ld32(self + 0x14);
    st32(p + 0x14, ctrl);
    return ctrl ? addShared(ctrl) : 0;
}
}  // namespace

namespace deck_edit_small {

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneDeckEdit_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x65FFB4, "SceneDeckEdit::createScene", deck_edit_small::createScene);

uint32_t setTouchEnabledAllButton(uint32_t self, uint32_t on) {
    uint32_t r = 0;
    for (uint32_t k = 0; k < 4; k++) {
        const uint32_t b = ld32(self + F_BUTTONS + 4 * k);
        r = vcall(b, V_setTouchEnabled, {b, on});
    }
    return r;
}
RW_FUNCTION(0x66DE9C, "SceneDeckEdit::setTouchEnabledAllButton", deck_edit_small::setTouchEnabledAllButton);

// the sort popup's close handler copied into p: callback state, the medal list (shared_ptr), the 22 state bytes, the
// 0x94-byte filter copy, the callback, the sorter's shared_ptr
uint32_t sortCloseCloneInto(uint32_t self, uint32_t p) {
    if (!p) return self;
    st32(p, VT_SortClose);
    st32(p + 8, ld32(self + 8));
    st32(p + 0xC, ld32(self + 0xC));
    if (ld32(p + 0xC)) addShared(ld32(p + 0xC));
    call(libc_memcpy, {p + 0x10, self + 0x10, 0x16});
    call(libc_memcpy, {p + 0x28, self + 0x28, 0x94});
    call(function_vb_copy, {p + 0xC0, self + 0xC0});
    st32(p + 0xD8, ld32(self + 0xD8));
    st32(p + 0xDC, ld32(self + 0xDC));
    st32(p + 0xE0, ld32(self + 0xE0));
    if (ld32(p + 0xE0)) addShared(ld32(p + 0xE0));
    const uint32_t r = ld32(self + 0xD8);
    st32(p + 0xD8, r);
    return r;
}
RW_FUNCTION(0x66E088, "IMedalSortFilter::openMedalSortPopup close handler __clone(p)", deck_edit_small::sortCloseCloneInto);

uint32_t sortReleaseCloneInto(uint32_t self, uint32_t p) {
    if (!p) return self;
    st32(p, VT_SortRelease);
    call(libc_memcpy, {p + 4, self + 4, 0x58});
    st32(p + 0x5C, ld32(self + 0x5C));
    st32(p + 0x60, ld32(self + 0x60));
    if (ld32(p + 0x60)) addShared(ld32(p + 0x60));
    const uint32_t r = ld32(self + 0x64);
    st32(p + 0x64, r);
    return r;
}
RW_FUNCTION(0x66EC84, "IMedalSortFilter::openMedalSortPopup Release __clone(p)", deck_edit_small::sortReleaseCloneInto);

uint32_t sortUpCloneInto(uint32_t self, uint32_t p) { return cloneSortOrder(self, p, VT_SortUp); }
RW_FUNCTION(0x671C58, "IMedalSortFilter::openMedalSortPopup Up __clone(p)", deck_edit_small::sortUpCloneInto);
uint32_t sortDownCloneInto(uint32_t self, uint32_t p) { return cloneSortOrder(self, p, VT_SortDown); }
RW_FUNCTION(0x671DFC, "IMedalSortFilter::openMedalSortPopup Down __clone(p)", deck_edit_small::sortDownCloneInto);

uint32_t stepUpEndClone(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0x10});
    if (c) {
        st32(c, VT_StepUpEnd);
        st32(c + 4, ld32(self + 4));
        st32(c + 8, ld32(self + 8));
        if (ld32(c + 8)) addShared(ld32(c + 8));
        st32(c + 0xC, ld32(self + 0xC));
    }
    return c;
}
RW_FUNCTION(0x675B30, "SceneDeckEdit::playKeybladeStepUp lambda __clone()", deck_edit_small::stepUpEndClone);

}  // namespace deck_edit_small
