// Avatar Board: SceneSphereBoardSelect's lambdas (backScene $_15: to the quest select; init $_2: the filter button
// opens SceneUtil::openAvatarBoardFilterPopup unless a board is opening; init $_1: the visible back button calls
// backScene) and SphereBlock::init $_5 (a touch ending inside the block's node marks it tapped).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneUtil_openAvatarBoardFilterPopup = 0x47546D;   // (function<void()> on close)
constexpr uint32_t SceneSphereBoardSelect_backScene = 0x6BB63D;
constexpr uint32_t Touch_getLocation = 0x900E57, Node_convertToNodeSpace = 0x89599D;
constexpr uint32_t Rect_containsPoint = 0x8DF2A9, Vec2_dtor = 0x8E4C17;
constexpr uint32_t GOT_FuncLayerPtr_vt = 0xFD6074;      // &vtable of __func<Layer*(*)(), ..., Layer*()>
constexpr uint32_t GOT_createAdventureSelect = 0xFD669C;   // &createSceneLayer<SceneAdventureSelect>
}  // namespace g

constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t SBS_OPENING = 0x322, SBS_CAN_FILTER = 0x331;   // SceneSphereBoardSelect bytes
constexpr uint32_t VT_FilterClosed = 0xF66808;                     // init $_2's popup lambda {scene}
constexpr uint32_t BLOCK_NODE = 0x280, BLOCK_TAPPED = 0x270;        // SphereBlock
constexpr uint32_t V_GET_BOUNDING_BOX = 0x1A0;                      // Node::getBoundingBox (Rect out)

struct SphereBoardLambdas {
    // backScene $_15: to the quest select
    static void toQuestSelect(uint32_t) {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t layer = newFunction(f, ld32(g::GOT_FuncLayerPtr_vt) + 8, {ld32(g::GOT_createAdventureSelect)});
        call(g::SceneManager_changeSceneLayer, {mgr, layer, 1, 1, 1});
        functionDestroy(layer);
    }

    // init $_2 (Ref*, TouchEventType) {scene}: the filter button
    static void filterTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        uint32_t scene = ld32(functor + 4);
        if (ld8(scene + SBS_OPENING) || !ld8(scene + SBS_CAN_FILTER)) return;
        call(g::AudioPlaySystemSEDecide, {});
        Frame f;
        uint32_t closed = newFunction(f, VT_FilterClosed, {scene});
        call(g::SceneUtil_openAvatarBoardFilterPopup, {closed});
        functionDestroy(closed);
    }

    // init $_1 (Ref*, TouchEventType) {button, scene}: back, while the button shows
    static void backTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        uint32_t button = ld32(functor + 4), scene = ld32(functor + 8);
        if (vcall(button, node::isVisible, {button}) != 1) return;
        call(g::AudioPlaySystemSEDecide, {});
        call(g::SceneSphereBoardSelect_backScene, {scene});
    }

    // SphereBlock::init $_5 (Touch*, Event*) {block}
    static void blockTouchEnded(uint32_t functor, uint32_t touchRef) {
        uint32_t touch = ld32(touchRef);
        uint32_t block = ld32(functor + 4);
        Frame f;
        uint32_t loc = f.alloc(8), local = f.alloc(8), box = f.alloc(16);
        call(g::Touch_getLocation, {loc, touch});
        call(g::Node_convertToNodeSpace, {local, ld32(block + BLOCK_NODE), loc});
        call(g::Vec2_dtor, {loc});
        uint32_t node = ld32(block + BLOCK_NODE);
        vcall(node, V_GET_BOUNDING_BOX, {box, node});
        uint32_t in = call(g::Rect_containsPoint, {box, local});
        call(g::Vec2_dtor, {box});
        if (in == 1) st8(block + BLOCK_TAPPED, 1);
        call(g::Vec2_dtor, {local});
    }
};
}  // namespace

RW_FUNCTION(0x6BB824, "SceneSphereBoardSelect::backScene $_15 __func::operator() (sub_6BB824)",
            SphereBoardLambdas::toQuestSelect);
RW_FUNCTION(0x6BCF7C, "SceneSphereBoardSelect::init $_2 __func::operator() (sub_6BCF7C)", SphereBoardLambdas::filterTouched);
RW_FUNCTION(0x6BD16E, "SceneSphereBoardSelect::init $_1 __func::operator() (sub_6BD16E)", SphereBoardLambdas::backTouched);
RW_FUNCTION(0x799094, "SphereBlock::init $_5 __func::operator() (sub_799094)", SphereBoardLambdas::blockTouchEnded);
