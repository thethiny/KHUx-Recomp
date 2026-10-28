// CampaignAnimationView's target campaign filters: the bool(int stageId, shared_ptr<const master::EvCampaign>&)
// lambdas of storyTargetCampaignFunc ($_0), eventTargetCampaignFunc ($_1), colosseumTargetCampaignFunc ($_2),
// raidTargetCampaignFunc ($_3) and helperRaidTargetCampaignFunc ($_4). A campaign counts for the stage when its
// type is one of the lambda's types, its stage type one of the lambda's stage types, and isCampaign(stageId, row).
#include "../master/EvCampaign.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t CampaignAnimationView_isCampaign = 0x59878D;   // (int stageId, shared_ptr<const EvCampaign>&)

// campaign types (bit masks over master::EvCampaign::TYPE)
constexpr uint32_t TYPES_STORY = 0x5FA, TYPES_RAID = 0x6BA, TYPES_COLOSSEUM = 0x8480;
constexpr int32_t MAX_TYPE_STORY = 10, MAX_TYPE_COLOSSEUM = 15;

bool bitIn(uint32_t mask, int32_t maxValue, int32_t v) { return uint32_t(v) <= uint32_t(maxValue) && ((1u << v) & mask); }

template <class StageTypeOk>
uint32_t targetFilter(uint32_t stageIdRef, uint32_t rowRef, uint32_t types, int32_t maxType, StageTypeOk stageTypeOk) {
    const int32_t stageId = int32_t(ld32(stageIdRef));
    const uint32_t row = ld32(rowRef);
    if (!bitIn(types, maxType, safe32(row, master::EvCampaign::TYPE))) return 0;
    if (!stageTypeOk(safe32(ld32(rowRef), master::EvCampaign::STAGE_TYPE))) return 0;
    return call(CampaignAnimationView_isCampaign, {uint32_t(stageId), rowRef});
}
}  // namespace

// $_4 / $_3: raid campaigns on raid stages (stage type 3 or 8)
static uint32_t raidTarget(uint32_t, uint32_t stageIdRef, uint32_t rowRef) {
    return targetFilter(stageIdRef, rowRef, TYPES_RAID, MAX_TYPE_STORY, [](int32_t t) { return t == 3 || t == 8; });
}
// $_2: colosseum campaigns (stage types 2, 4, 7)
static uint32_t colosseumTarget(uint32_t, uint32_t stageIdRef, uint32_t rowRef) {
    return targetFilter(stageIdRef, rowRef, TYPES_COLOSSEUM, MAX_TYPE_COLOSSEUM,
                        [](int32_t t) { return bitIn(0x94, 7, t); });
}
// $_1: event campaigns (stage types 2, 4, 6)
static uint32_t eventTarget(uint32_t, uint32_t stageIdRef, uint32_t rowRef) {
    return targetFilter(stageIdRef, rowRef, TYPES_STORY, MAX_TYPE_STORY, [](int32_t t) { return bitIn(0x54, 6, t); });
}
// $_0: story campaigns (stage types 9..18, or 2, 4, 5)
static uint32_t storyTarget(uint32_t, uint32_t stageIdRef, uint32_t rowRef) {
    return targetFilter(stageIdRef, rowRef, TYPES_STORY, MAX_TYPE_STORY,
                        [](int32_t t) { return uint32_t(t - 9) < 10 || bitIn(0x34, 5, t); });
}

RW_FUNCTION(0x5994A4, "CampaignAnimationView::helperRaidTargetCampaignFunc $_4::operator()", raidTarget);
RW_FUNCTION(0x5995CC, "CampaignAnimationView::raidTargetCampaignFunc $_3::operator()", raidTarget);
RW_FUNCTION(0x5996F0, "CampaignAnimationView::colosseumTargetCampaignFunc $_2::operator()", colosseumTarget);
RW_FUNCTION(0x599824, "CampaignAnimationView::eventTargetCampaignFunc $_1::operator()", eventTarget);
RW_FUNCTION(0x599958, "CampaignAnimationView::storyTargetCampaignFunc $_0::operator()", storyTarget);
