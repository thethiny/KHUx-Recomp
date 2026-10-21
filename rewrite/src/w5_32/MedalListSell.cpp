// SceneMedalList's Sell Medals button (init $_4): once, the buttons off, the "new" marks cleared and the list
// runs out; then (its lambda) the scene changes to the sale screen.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneUtil_clearMedalNewMark = 0x46C4B5;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, std::function<void()> by value)
constexpr uint32_t SM_changeSceneLayer = 0x612B99;               // (sm, const function<Layer*()>&, bool, bool, FadeType)
// SceneMedalList
constexpr uint32_t F_INOUT = 0x2A4, F_SELL_BUTTON = 0x2B8, F_SORT_BUTTON = 0x2BC, F_CAN_BACK = 0x2C0;
constexpr uint32_t VT_SellOut = 0xF6DB34;       // $_4's lambda {}
constexpr uint32_t VT_SaleScene = 0xF6DB60;     // its scene factory {}
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;

void sellTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t me = ld32(functor + 4);
    if (!ld8(me + F_CAN_BACK)) return;
    st8(me + F_CAN_BACK, 0);
    vcall(ld32(me + F_SELL_BUTTON), V_widgetSetTouchEnabled, {ld32(me + F_SELL_BUTTON), 0});
    vcall(ld32(me + F_SORT_BUTTON), V_widgetSetTouchEnabled, {ld32(me + F_SORT_BUTTON), 0});
    call(AudioPlaySystemSEDecide, {});
    call(SceneUtil_clearMedalNewMark, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_SellOut, {});
    call(IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

void sellOut(uint32_t) {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, VT_SaleScene, {});
    rw::call(SM_changeSceneLayer, {sm, fo, 1, 0, 0});
    functionDestroy(fo);
}
}  // namespace

RW_FUNCTION(0x73E93C, "SceneMedalList::init::$_4 operator() (Sell Medals)", sellTouched);
RW_FUNCTION(0x73EAAC, "SceneMedalList::init::$_4::lambda() operator()", sellOut);
