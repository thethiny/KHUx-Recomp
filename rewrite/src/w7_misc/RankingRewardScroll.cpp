// SceneRankingReward: a sideways swipe (at least 3% of the screen width) on the reward pages locks the reward
// list's vertical scrolling for the rest of the swipe (+0x2A5).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize, IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t S_ListName = 0xE06A62;         // "rankingRewardScrollView"
constexpr uint32_t F_PAGES = 0x290, PAGES_CURRENT = 0x588, F_SWIPING = 0x2A5;
}  // namespace

namespace ranking_reward_scroll {

uint32_t swiped(uint32_t functor, uint32_t deltaRef) {
    const float dx = bitsf(ld32(deltaRef) & 0x7FFFFFFF);
    const uint32_t scene = ld32(functor + 4);
    Frame f;
    const uint32_t size = f.alloc(8);
    call(Director_getVisibleSize, {size, director()});
    if (!(ldf(size) * 0.03f <= dx)) return 0;
    const uint32_t page = ld32(ld32(scene + F_PAGES) + PAGES_CURRENT);
    if (!page) return 0;
    st8(scene + F_SWIPING, 1);
    const uint32_t list = findNode(page, S_ListName, 0x17);
    call(IScrollProtocol_setEnabledScroll, {list, 0});
    return 0;
}
RW_FUNCTION(0x711520, "SceneRankingReward page swipe operator() (sub_711520)", ranking_reward_scroll::swiped);

}  // namespace ranking_reward_scroll
