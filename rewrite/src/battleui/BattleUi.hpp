// Shared base for the battle UI rewrites (StageUI, carousel / medal decks, indicators, directions, raid screens,
// battle result): pulls in the scene helpers (guest stack Frame, std::string / std::function / shared_ptr
// temporaries, cocos2d Node virtual slots, SceneUtil) and the battle module helpers (bt::GuestFrame, soft-float
// conversions). Owned by the battleui folder lead: class files include it and keep their own helpers in an
// anonymous namespace (or a namespace named after the class) so files never clash.
#pragma once

#include "../core/Guest.hpp"
#include "../core/SafeBuffer.hpp"
#include "../scene/SceneCommon.hpp"
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"
#include "../battle/BattleCommon.hpp"

namespace bui {

// cocos2d::Ref: vtable +0, reference count +4, script binding fields; 20 bytes.
struct Ref {
    rw::Field<uint32_t> vtable;     // +0
    rw::Field<uint32_t> refCount;   // +4
    const uint8_t pad_08[12];             // +8
};
RW_SIZE(Ref, 20);

// The stack protector: the original copies *__stack_chk_guard into its frame and compares it on return. The copy
// lives in the frame, which the verifier ignores, so rewrites need not reproduce it.
constexpr uint32_t STACK_CHK_FAIL = 0x40DD44;   // __stack_chk_fail (PLT)

}  // namespace bui
