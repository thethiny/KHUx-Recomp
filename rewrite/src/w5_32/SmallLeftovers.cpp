// Two small lambdas from the screen tours: the gacha "check raise" popup's close button and the ranking-reward
// screen's scroll unlock.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;        // (sm, bool)
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t S_RankingRewardScrollView = 0xE06A62;   // "rankingRewardScrollView"
// SceneRankingReward
constexpr uint32_t F_ROOT = 0x290, F_SCROLL_LOCKED = 0x2A5;
constexpr uint32_t ROOT_LAYOUT = 0x588;
}  // namespace

namespace small_leftovers {

// DrawScrollView::openCheckRaisePopup $_11: the button off, the popup closed. r0 is left as the functor when the
// touch is not an end.
uint32_t checkRaiseClose(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return self;
    const uint32_t button = ld32(self + 4);
    vcall(button, V_setTouchEnabled, {button, 0});
    return call(SM_closeCurrentPopup, {sceneManager(), 0});
}
RW_FUNCTION(0x5B245E, "DrawScrollView::openCheckRaisePopup::$_11 operator()", small_leftovers::checkRaiseClose);

// SceneRankingReward::init $_7: once, the reward list scrolls again
uint32_t rankingRewardScroll(uint32_t self) {
    const uint32_t scene = ld32(self + 4);
    if (!ld8(scene + F_SCROLL_LOCKED)) return 0;
    const uint32_t root = ld32(ld32(scene + F_ROOT) + ROOT_LAYOUT);
    if (!root) return 0;
    st8(scene + F_SCROLL_LOCKED, 0);
    call(IScrollProtocol_setEnabledScroll, {findNode(root, S_RankingRewardScrollView, 0x17), 1});
    return 0;
}
RW_FUNCTION(0x7113B4, "SceneRankingReward::init::$_7 operator() (scroll on)", small_leftovers::rankingRewardScroll);

}  // namespace small_leftovers
