// Medal screens' lambdas: SceneMixedMedal::initMedalSelectPanel $_29 (a base medal tapped: its info popup with the
// fusion button, the answer in UlbE_) and UlbE_'s clone; SceneEvolveMedal::init $_7 (the touch is in a captured
// rectangle of a node) and $_8's clone; MaterialScrollView::registItemTouchEvent $_0 (touch began on a material
// cell that is not masked).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_openMedalInfoPopup = 0x466EF5;   // (shared_ptr medal, bool, bool, bool,
                                                              //  function<void(ButtonType, bool)>, vector<Skill>*,
                                                              //  function<void()>, bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;  // (sret, mgr, u64 id, int, UserMedal::Medal)
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation, Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
}  // namespace g

constexpr int32_t TUTORIAL_DONE_PHASE = 0x3E3;   // 995
// SceneMixedMedal
constexpr uint32_t MIX_BUSY = 0x34C, MIX_CAN_TAP = 0x38C;
constexpr uint32_t API_USER_SETTINGS = 0x94;      // SafeBuffer; +0x54 byte: a display setting
constexpr uint32_t VT_BaseInfoResult = 0xF65654;  // $_29 UlbE_ {.., scene +8, 0 +0xC, medal id +0x10}
constexpr uint32_t VT_BaseInfoClosed = 0xF65628;  // $_29's popup closed lambda {scene}
// MaterialScrollView
constexpr uint32_t MS_TAP = 0x654, MS_BUSY = 0x65C, MS_TOUCH_Y = 0x660, MS_PRESSED = 0x655, MS_ROWS = 0x51C;
constexpr uint32_t S_PanelMask = 0xDFD14B;

// the touch, in `node`'s space, inside `rect`
bool touchIn(uint32_t touch, uint32_t node, uint32_t rect) {
    Frame f;
    uint32_t loc = f.alloc(8), local = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {local, node, loc});
    call(g::Vec2_dtor, {loc});
    bool in = call(g::Rect_containsPoint, {rect, local}) != 0;
    call(g::Vec2_dtor, {local});
    return in;
}

struct MedalLambdas {
    // SceneMixedMedal::initMedalSelectPanel $_29 (unsigned long long, int) {scene}
    static void baseMedalTapped(uint32_t functor, uint32_t idRef, uint32_t) {
        uint32_t scene = ld32(functor + 4);
        if (!ld8(scene + MIX_CAN_TAP)) return;
        uint32_t lo = ld32(idRef), hi = ld32(idRef + 4);
        st8(scene + MIX_CAN_TAP, 0);
        if (int32_t(call(g::SceneUtil_getNexTutorialphase, {})) < TUTORIAL_DONE_PHASE) return;
        call(g::AudioPlaySystemSEDecide, {});
        st8(scene + MIX_BUSY, 1);
        uint32_t mgr = call(g::MedalDataManager_getInstance, {});
        Frame f;
        uint32_t medal = f.alloc(8);
        call(g::MedalDataManager_getMedalData, {medal, mgr, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0});
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t copy = f.alloc(8);
        st32(copy, ld32(medal));
        st32(copy + 4, ld32(medal + 4));
        if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
        uint32_t setting = ld8(safeBytes(f, ld32(api + API_USER_SETTINGS), 0x54, 1));
        uint32_t result = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_BaseInfoResult);
            st32(heap + 8, scene);
            st32(heap + 0xC, 0);
            st32(heap + 0x10, lo);
            st32(heap + 0x14, hi);
        }
        st32(result + 16, heap);
        uint32_t closed = newFunction(f, VT_BaseInfoClosed, {scene});
        call(g::SceneUtil_openMedalInfoPopup, {copy, setting ? 1u : 0u, 0, 1, result, 0, closed, 1});
        functionDestroy(closed);
        functionDestroy(result);
        if (ld32(copy + 4)) call(fn::shared_release, {ld32(copy + 4)});
        sharedRelease(medal);
    }

    // $_29 UlbE_ __clone()
    static uint32_t cloneBaseInfoResult(uint32_t self) {
        uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, VT_BaseInfoResult);
            for (uint32_t k = 8; k < 0x18; k += 4) st32(obj + k, ld32(self + k));
        }
        return obj;
    }

    // SceneEvolveMedal::init $_8 __clone(__base*) {shared_ptr}
    static void cloneEvolve8(uint32_t self, uint32_t dst) {
        if (!dst) return;
        st32(dst, 0xF66414);
        st32(dst + 4, ld32(self + 4));
        uint32_t ctrl = ld32(self + 8);
        st32(dst + 8, ctrl);
        if (ctrl) call(fn::shared_add, {ctrl});
    }

    // SceneEvolveMedal::init $_7 bool(Touch*, Event*) {node, Rect}
    static uint32_t evolveTouch(uint32_t functor, uint32_t touchRef) {
        uint32_t touch = ld32(touchRef);
        return touchIn(touch, ld32(functor + 4), functor + 8) ? 1 : 0;
    }

    // MaterialScrollView::registItemTouchEvent $_0 bool(Touch*, Event*) {this, index}: a press on an unmasked cell
    static uint32_t materialTouchBegan(uint32_t functor, uint32_t touchRef) {
        uint32_t touch = ld32(touchRef);
        uint32_t self = ld32(functor + 4);
        if (!ld8(self + MS_TAP) || ld8(self + MS_BUSY)) return 0;
        {
            Frame f;
            uint32_t loc = f.alloc(8);
            call(g::Touch_getLocation, {loc, touch});
            st32(self + MS_TOUCH_Y, ld32(loc + 4));
            call(g::Vec2_dtor, {loc});
        }
        uint32_t row = ld32(ld32(self + MS_ROWS) + 4 * ld32(functor + 8));
        Frame f;
        uint32_t loc = f.alloc(8), local = f.alloc(8), rect = f.alloc(16);
        call(g::Touch_getLocation, {loc, touch});
        call(g::Node_convertToNodeSpace, {local, row, loc});
        call(g::Vec2_dtor, {loc});
        uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
        uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
        call(g::Rect_ctor, {rect, 0, 0, w, h});
        uint32_t began = 0;
        if (call(g::Rect_containsPoint, {rect, local})) {
            uint32_t mask = findNode(row, S_PanelMask, 9);
            if (!mask || !vcall(mask, node::isVisible, {mask})) {
                st8(self + MS_PRESSED, 1);
                began = 1;
            }
        }
        call(g::Vec2_dtor, {rect});
        call(g::Vec2_dtor, {local});
        return began;
    }
};
}  // namespace

RW_FUNCTION(0x6A0DB4, "SceneMixedMedal::initMedalSelectPanel $_29 __func::operator() (sub_6A0DB4)",
            MedalLambdas::baseMedalTapped);
RW_FUNCTION(0x6A10CC, "SceneMixedMedal::initMedalSelectPanel $_29 UlbE_ __func::__clone() (sub_6A10CC)",
            MedalLambdas::cloneBaseInfoResult);
RW_FUNCTION(0x6B3528, "SceneEvolveMedal::init $_8 __func::__clone(__base*) (sub_6B3528)", MedalLambdas::cloneEvolve8);
RW_FUNCTION(0x6B37F0, "SceneEvolveMedal::init $_7 __func::operator()(Touch*, Event*) (sub_6B37F0)",
            MedalLambdas::evolveTouch);
RW_FUNCTION(0x502234, "MaterialScrollView::registItemTouchEvent $_0 __func::operator() (sub_502234)",
            MedalLambdas::materialTouchBegan);
