// Exception cleanup pads of SceneUtil::updateMedalInfo (sub_464F8E, sub_465760), of
// SceneManager::openBlockPostPopup (sub_61C20C .. sub_61C2B4: one pad entered at five
// points, each skipping the cleanups already done) and of SceneManager::openUnBlockPostPopup (sub_61FAB6). They run
// in the parent's frame: everything is read relative to the guest sp the parent left, and they end in
// _Unwind_Resume. The parents' rewrites have no landing pads, so these only run from lifted parents.
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t OPERATOR_DELETE = 0xC98F9B;
constexpr uint32_t UNWIND_RESUME = 0xC9BEE9;
constexpr uint32_t SHARED_RELEASE = 0xC8DBD1;

inline uint32_t sp() { return rw::cpu->r[13]; }

// a std::function's destructor as inlined: the callable `cur`; `local` is the inline buffer, whose vtable is read
// through `vtableAt`
void destroyFunction(uint32_t cur, uint32_t local, uint32_t vtableAt) {
    if (cur == local) {
        call(ld32(ld32(vtableAt) + 0x10), {local});
        return;
    }
    if (cur) call(ld32(ld32(cur) + 0x14), {cur});
}
// a std::string's destructor as inlined: `flags` holds the size byte, `mask` the long-string bit
void freeString(uint32_t flags, uint32_t mask, uint32_t data) {
    if (ld8(flags) & mask) call(OPERATOR_DELETE, {data});
}

// ---------------------------------------------------------------- openBlockPostPopup
void blockPostTail() {   // sub_61C2B4
    const uint32_t s = sp();
    destroyFunction(ld32(ld32(s + 0x70) + 0x30), ld32(s + 0x94), ld32(s + 0x70) + 0x20);
    destroyFunction(ld32(ld32(s + 0x70) + 0x10), ld32(s + 0x98), ld32(s + 0x70));
    destroyFunction(ld32(ld32(s + 8)), ld32(s + 0xA8), ld32(s + 0xC8) + 0x50);
    if (ld32(s + 0x68)) call(SHARED_RELEASE, {ld32(s + 0x68)});
    call(UNWIND_RESUME, {ld32(s + 0xD0)});
}
void blockPostTailDelete(uint32_t p) {   // sub_61C2B0
    call(OPERATOR_DELETE, {p});
    blockPostTail();
}
void blockPostFunctions() {   // sub_61C234
    const uint32_t s = sp();
    destroyFunction(ld32(s + 0x470), ld32(s + 0x9C), ld32(s + 0x9C));
    destroyFunction(ld32(s + 0x450), ld32(s + 0x90), ld32(s + 0xB4) + 0x78);
    destroyFunction(ld32(ld32(s + 0xB4) + 0x68), ld32(s + 0xA0), ld32(s + 0xB4) + 0x58);
    const uint32_t mask = ld32(s + 0xD4);
    freeString(ld32(s + 0xA4), mask, ld32(ld32(s + 0xB4) + 0x3C));
    freeString(s + 0x3C8 + 0x1C, mask, ld32(ld32(s + 0xB4) + 0x24));
    blockPostTail();
}
void blockPostStrings() {   // sub_61C210
    const uint32_t s = sp();
    const uint32_t mask = ld32(s + 0xD4);
    freeString(ld32(s + 0x88), mask, ld32(ld32(s + 0x14) + 0x18));
    freeString(ld32(s + 0x8C), mask, ld32(ld32(s + 0x14)));
    blockPostFunctions();
}
void blockPostDelete(uint32_t p) {   // sub_61C20C
    call(OPERATOR_DELETE, {p});
    blockPostStrings();
}

// ---------------------------------------------------------------- openUnBlockPostPopup
void unblockPost() {   // sub_61FAB6
    const uint32_t s = sp();
    destroyFunction(ld32(ld32(s + 0xC) + 0x20), ld32(s + 0x60), ld32(s + 0xC) + 0x10);
    destroyFunction(ld32(ld32(s + 0x90) + 0x20), ld32(s + 0x64), ld32(s + 0x90) + 0x10);
    destroyFunction(ld32(ld32(s + 8)), ld32(s + 0x70), ld32(s + 0x90) + 0x50);
    if (ld32(s + 0x44)) call(SHARED_RELEASE, {ld32(s + 0x44)});
    call(UNWIND_RESUME, {rw::cpu->r[5]});
}
}  // namespace

RW_FUNCTION(0x61C20C, "SceneManager::openBlockPostPopup cleanup pad (sub_61C20C)", blockPostDelete);
RW_FUNCTION(0x61C210, "SceneManager::openBlockPostPopup cleanup pad (sub_61C210)", blockPostStrings);
RW_FUNCTION(0x61C234, "SceneManager::openBlockPostPopup cleanup pad (sub_61C234)", blockPostFunctions);
RW_FUNCTION(0x61C2B0, "SceneManager::openBlockPostPopup cleanup pad (sub_61C2B0)", blockPostTailDelete);
RW_FUNCTION(0x61C2B4, "SceneManager::openBlockPostPopup cleanup pad (sub_61C2B4)", blockPostTail);
RW_FUNCTION(0x61FAB6, "SceneManager::openUnBlockPostPopup cleanup pad (sub_61FAB6)", unblockPost);

namespace {
// ---------------------------------------------------------------- SceneUtil::updateMedalInfo
void medalInfoTail() {
    const uint32_t s = sp();
    if (ld32(s + 0x74)) call(SHARED_RELEASE, {ld32(s + 0x74)});
    call(UNWIND_RESUME, {rw::cpu->r[5]});
}
void medalInfoPad() {   // sub_464F8E
    const uint32_t ctl = ld32(ld32(sp() + 0xE8) + 0x54);
    if (ctl) call(SHARED_RELEASE, {ctl});
    medalInfoTail();
}
void medalInfoPadR4() {   // sub_465760
    if (rw::cpu->r[4]) call(SHARED_RELEASE, {rw::cpu->r[4]});
    medalInfoTail();
}
}  // namespace

RW_FUNCTION(0x464F8E, "SceneUtil::updateMedalInfo cleanup pad (sub_464F8E)", medalInfoPad);
RW_FUNCTION(0x465760, "SceneUtil::updateMedalInfo cleanup pad (sub_465760)", medalInfoPadR4);
