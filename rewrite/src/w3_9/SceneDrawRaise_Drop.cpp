// SceneDrawRaise::dropAnimation and its lambdas: the drawn medals fade in one after the other (DropMedalLayer
// list, cursor field_2A0); after the last one each medal gets a "ShopBuy_But" ImageButton that opens its result
// popup.
#include "SceneDrawRaise.hpp"

#include "../scene_main/FunctionThunks.hpp"

using namespace scene;
using Self = SceneDrawRaise;

namespace {
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;
constexpr uint32_t AEABI_MEMCPY = 0x40E164;
constexpr uint32_t ImageButton_create = 0x531239;              // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;       // (button, Node*, float scale)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;

constexpr uint32_t VSLOT_ADD_CHILD_Z_NAME = 0xFC;              // Node::addChild(child, z, const std::string&)
constexpr uint32_t VT_DropNext = 0xF6507C;                     // dropAnimation::$_17 [scene, function<void()>]
constexpr uint32_t VT_MedalTap = 0xF650A8;                     // $_17::operator()::$_16 [scene, 0, DropMedalLayer]

constexpr uint32_t STR_ShopBuy_But = 0xE00327;                 // "ShopBuy_But"
constexpr uint32_t STR_CenterButton = 0xDFFB40;                // "CenterButton"

inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }

// A long std::string built inline at `s` from a literal of `len` (> 10) chars: heap block of 16, cap | 1 = 0x11.
void longString(uint32_t s, uint32_t text, uint32_t len) {
    uint32_t data = guest::newObject(0x10);
    fst32(s, 0x11);
    fst32(s + 4, len);
    fst32(s + 8, data);
    call(AEABI_MEMCPY, {data, text, len});
    st8(data + len, 0);
}
}  // namespace

// The medal at the cursor fades in (FadeTo field_2A4 s), waits field_2A8 s, then $_17 moves the cursor on.
namespace draw_raise_drop {
void dropAnimation(uint32_t self, uint32_t done) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    uint32_t doneCopy = f.alloc(0x18);
    call(fn::function_vv_copy, {doneCopy, done});
    uint32_t index = s->field_2A0;
    uint32_t begin = s->dropMedals.begin;
    if (!(uint32_t(int32_t(s->dropMedals.end - begin) >> 4) > index)) {
        call(Vector_throwOutOfRange, {self + 0x294});
        return;
    }
    uint32_t node = ld32(begin + index * 16);
    uint32_t fade = call(fn::FadeTo_create, {rw::get<uint32_t>(s, 0x2A4), 0xFF});
    uint32_t delay = call(fn::DelayTime_create, {rw::get<uint32_t>(s, 0x2A8)});
    uint32_t captured = f.alloc(0x18);
    functionCopy(captured, doneCopy);
    uint32_t next = f.alloc(0x18);
    fst32(next + 0x10, 0);
    uint32_t obj = guest::newObject(0x28);
    if (obj) {
        st32(obj, VT_DropNext);
        st32(obj + 8, self);
        call(fn::function_vv_move, {obj + 0x10, captured});
        st32(obj + 8, self);
    }
    fst32(next + 0x10, obj);
    uint32_t callFunc = call(fn::CallFunc_create, {next});
    uint32_t seq = call(fn::Sequence_create, {fade, delay, callFunc, 0});
    call(fn::Node_runAction, {node, seq});
    functionDestroy(next);
    functionDestroy(captured);
    functionDestroy(doneCopy);
}
}  // namespace draw_raise_drop
RW_FUNCTION(0x692660, "SceneDrawRaise::dropAnimation", draw_raise_drop::dropAnimation);

namespace {
// $_17 [scene at +8, function<void()> done at +0x10]: next medal, or - after the last one - run `done` and put a
// tap button on every medal.
void dropNext(uint32_t self) {
    Frame f;
    uint32_t scene = ld32(self + 8);
    Self *s = rw::host_ptr<Self>(scene);
    uint32_t index = s->field_2A0 + 1;
    s->field_2A0 = index;
    uint32_t p = s->dropMedals.begin;
    uint32_t end = s->dropMedals.end;
    if (int32_t(index) < (int32_t(end - p) >> 4)) {
        uint32_t doneCopy = f.alloc(0x18);
        call(fn::function_vv_copy, {doneCopy, self + 0x10});
        draw_raise_drop::dropAnimation(scene, doneCopy);
        functionDestroy(doneCopy);
        return;
    }
    if (ld32(self + 0x20)) {
        call(fn::function_vv_call, {self + 0x10});
        end = s->dropMedals.end;
        p = s->dropMedals.begin;
    }
    uint32_t file = f.alloc(12), name = f.alloc(12), cb = f.alloc(0x18);
    for (; p != end; p += 16) {
        uint32_t target = ld32(p), w1 = ld32(p + 4), idLo = ld32(p + 8), idHi = ld32(p + 12);
        longString(file, STR_ShopBuy_But, 0xB);
        uint32_t button = call(ImageButton_create, {file, 0});
        strFree(file);
        call(ImageButton_setTargetView, {button, target, 0x3F800000});
        uint32_t addChild = ld32(ld32(target) + VSLOT_ADD_CHILD_Z_NAME);
        longString(name, STR_CenterButton, 0xC);
        call(addChild, {target, button, 0, name});
        strFree(name);
        // button->addTouchEventListener($_16{scene, 0, DropMedalLayer})
        fst32(cb + 0x10, 0);
        uint32_t obj = guest::newObject(0x20);
        if (obj) {
            st32(obj, VT_MedalTap);
            st32(obj + 8, scene);
            st32(obj + 0xC, 0);
            st32(obj + 0x10, target);
            st32(obj + 0x14, w1);
            st32(obj + 0x18, idLo);
            st32(obj + 0x1C, idHi);
        }
        fst32(cb + 0x10, obj);
        call(ImageButton_addTouchEventListener, {button, cb});
        functionDestroy(cb);
    }
}

// $_16 [scene at +8, 0, DropMedalLayer at +0x10]: tapping a medal (touch ended) opens its result popup when the
// scene allows it (flag_2AE). The original passes a null `this` to openDrawResultPopup.
void medalTap(uint32_t self, const uint32_t *, const uint32_t *type) {
    if (*type != 2) return;
    if (!rw::host_ptr<Self>(ld32(self + 8))->flag_2AE) return;
    Frame f;
    uint32_t cb = f.alloc(0x18);
    fst32(cb + 0x10, 0);
    call(draw_raise::OPEN_DRAW_RESULT_POPUP, {0, cb, ld32(self + 0x18), ld32(self + 0x1C), 0, 1, cb});
    functionDestroy(cb);
}

using DropNextFunc = thunk::LambdaFn<VT_DropNext, 0x28, 0xF9D27C, 0xE72BC0, 8, 0x10, fn::function_vv_copy,
                                     thunk::cap(8, 4)>;
using MedalTapFunc = thunk::Lambda<VT_MedalTap, 0x20, 0xF9D290, 0xE72CB0, 8, thunk::cap(8, 4), thunk::cap(0xC, 4),
                                   thunk::cap(0x10, 4), thunk::cap(0x14, 4), thunk::cap(0x18, 4), thunk::cap(0x1C, 4)>;
}  // namespace

RW_FUNCTION(0x692A34, "SceneDrawRaise::dropAnimation::$_17::operator()", dropNext);
RW_FUNCTION(0x6928D4, "SceneDrawRaise::dropAnimation::$_17 ~__func()", DropNextFunc::destructor);
RW_FUNCTION(0x692910, "SceneDrawRaise::dropAnimation::$_17 ~__func() (deleting)", DropNextFunc::destructorDelete);
RW_FUNCTION(0x692950, "SceneDrawRaise::dropAnimation::$_17 __clone()", DropNextFunc::clone);
RW_FUNCTION(0x6929A8, "SceneDrawRaise::dropAnimation::$_17 __clone(__base*)", DropNextFunc::cloneInto);
RW_FUNCTION(0x6929E8, "SceneDrawRaise::dropAnimation::$_17 destroy()", DropNextFunc::destroy);
RW_FUNCTION(0x692A08, "SceneDrawRaise::dropAnimation::$_17 destroy_deallocate()", DropNextFunc::destroyDeallocate);
RW_FUNCTION(0x692CB0, "SceneDrawRaise::dropAnimation::$_17 target(const type_info&)", DropNextFunc::target);
RW_FUNCTION(0x692CD4, "SceneDrawRaise::dropAnimation::$_17 target_type()", DropNextFunc::targetType);

RW_FUNCTION(0x692D68, "SceneDrawRaise::dropAnimation::$_16::lambda::operator()", medalTap);
RW_FUNCTION(0x692CE8, "SceneDrawRaise::dropAnimation::$_16::lambda ~__func() (deleting)", MedalTapFunc::deleteThis);
RW_FUNCTION(0x692CF0, "SceneDrawRaise::dropAnimation::$_16::lambda __clone()", MedalTapFunc::clone);
RW_FUNCTION(0x692D2C, "SceneDrawRaise::dropAnimation::$_16::lambda __clone(__base*)", MedalTapFunc::cloneInto);
RW_FUNCTION(0x692D5C, "SceneDrawRaise::dropAnimation::$_16::lambda destroy()", MedalTapFunc::destroy);
RW_FUNCTION(0x692D5E, "SceneDrawRaise::dropAnimation::$_16::lambda destroy_deallocate()", MedalTapFunc::deleteThis);
RW_FUNCTION(0x692E10, "SceneDrawRaise::dropAnimation::$_16::lambda target(const type_info&)", MedalTapFunc::target);
RW_FUNCTION(0x692E34, "SceneDrawRaise::dropAnimation::$_16::lambda target_type()", MedalTapFunc::targetType);
