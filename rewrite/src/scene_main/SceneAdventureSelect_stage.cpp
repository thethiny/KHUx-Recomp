// SceneAdventureSelect: the selected stage's info panel and the campaign banner above it.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {

// runInfoOutAnimation(panel, lambda{vtable, self, campaignView})
void infoOutWith(uint32_t panel, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t cb = newFunction(f, vtable, captures);
    call(lib::AdventurePanel_runInfoOutAnimation, {panel, cb});
    functionDestroy(cb);
}

// filter = CampaignAnimationView::<kind>TargetCampaignFunc() (moved in)
void setFilter(uint32_t filter, uint32_t factory) {
    Frame f;
    uint32_t tmp = f.alloc(20);
    call(factory, {tmp});
    call(lib::CampaignFilter_moveAssign, {filter, tmp});
    functionDestroy(tmp);
}

}  // namespace

// Refresh the info for the selected stage (field_298; -1 = none): the info panel of the shown type slides out
// and back with the new stage (lambdas $_43..$_47), and the campaign banner (campaignView under the announce
// panel) lists the running campaigns that apply to the stage, the event, the colosseum or the raid boss.
void SceneAdventureSelect::changeStageInfo(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->field_298 == 0xFFFFFFFFu) return;
    if (s->deferment) setVisible(s->deferment, 0);
    uint32_t campaignView = findNode(s->root, STR_campaignView, 0xC);

    Frame f;
    int32_t target = 0;
    uint32_t filter = f.alloc(20);                  // std::function<bool(int, shared_ptr<EvCampaign>&)>
    st32(filter + 16, 0);
    uint32_t story = f.alloc(24);                   // the selected Stage::Story (its vector<int> at +0xC)
    st32(story + 0xC, 0);
    st32(story + 0x10, 0);
    st32(story + 0x14, 0);
    uint32_t apiA = call(fn::APIManager_getInstance, {});
    uint32_t apiB = call(fn::APIManager_getInstance, {});
    int32_t type = s->adventureType;
    bool raidStage = false;
    if (type == 0 || type == 6) {
        uint32_t stories = f.alloc(12);
        call(lib::StoryVector_copy, {stories, ld32(apiA + API_STORIES)});
        uint32_t begin = ld32(stories), end = ld32(stories + 4);
        uint32_t count = storyCount(begin, end);
        uint32_t index = 0;
        if (int32_t(end - begin) >= 1) {
            for (uint32_t k = 0;;) {
                if (count <= k) {
                    call(lib::THROW_OUT_OF_RANGE, {stories});
                    return;
                }
                if (ld32(begin + k * STORY_SIZE) == s->field_298) {
                    index = k;
                    break;
                }
                k++;
                if (int32_t(k) >= int32_t(count)) break;
            }
        }
        if (count <= index) {
            call(lib::THROW_OUT_OF_RANGE, {stories});
            return;
        }
        uint32_t chosen = begin + index * STORY_SIZE;
        st32(story, ld32(chosen));
        st32(story + 4, ld32(chosen + 4));
        st32(story + 8, ld32(chosen + 8));
        call(lib::IntVector_assign, {story + 0xC, ld32(chosen + 0xC), ld32(chosen + 0x10)});
        uint32_t info = ld32(apiB + API_RAID_INFO);
        bool bossUp = (ld32(info) | ld32(info + 4)) != 0;
        uint32_t lastStage = ld32(ld32(apiA + API_STORIES) + STORY_DATA_CURRENT);
        if (lastStage == 0) {
            uint32_t n = storyCount(ld32(stories), ld32(stories + 4));
            if (n - 1 >= n) {
                call(lib::THROW_OUT_OF_RANGE, {stories});
                return;
            }
            lastStage = ld32(ld32(stories) + (n - 1) * STORY_SIZE);
        }
        raidStage = bossUp && lastStage == s->field_298 && int32_t(ld32(info + RAID_INFO_HP)) >= 1;
        call(lib::StoryVector_dtor, {stories});
        type = s->adventureType;
    }

    switch (uint32_t(type)) {
    case 0:
    case 6:
        s->flag_29E = 1;
        if (raidStage) {
            // the boss is on the selected stage: the story info slides out showing the boss ($_43 keeps the
            // RaidBase::Info alive)
            uint32_t ctrl = ld32(apiB + API_RAID_INFO_CTRL);
            uint32_t info = ld32(apiB + API_RAID_INFO);
            if (ctrl) call(fn::shared_add, {ctrl});
            {
                Frame g;
                uint32_t cb = g.alloc(20);
                st32(cb + 16, 0);
                uint32_t obj = call(fn::operator_new, {0x14});
                if (obj) {
                    st32(obj, VT_changeStageInfo_43);
                    st32(obj + 4, self);
                    st32(obj + 8, info);
                    st32(obj + 0xC, ctrl);
                    if (ctrl) call(fn::shared_add, {ctrl});
                    st32(obj + 0x10, campaignView);
                }
                st32(cb + 16, obj);
                call(lib::AdventurePanel_runInfoOutAnimation, {self + 0x380, cb});
                functionDestroy(cb);
            }
            if (ctrl) call(fn::shared_release, {ctrl});
            runPanel(lib::AdventurePanel_runFadeOutAnimation, self + 0x2BC);
            target = int32_t(ld32(ld32(apiB + API_RAID_INFO) + RAID_INFO_STAGE));
            setFilter(filter, lib::CampaignAnimationView_raidTargetCampaignFunc);
        } else {
            uint32_t bossIcon = findNode(s->raid.node, STR_Raid_Boss_Icon, 0xE);
            vcall(bossIcon, vslot::widgetSetTouchEnabled, {bossIcon, 0});
            infoOutWith(self + 0x380, VT_changeStageInfo_44, {self, campaignView});
            runPanel(lib::AdventurePanel_runInfoOutAnimation, self + 0x3A4);
            runPanel(lib::AdventurePanel_runFadeOutAnimation, self + 0x2BC);
            target = int32_t(s->field_298);
            setFilter(filter, lib::CampaignAnimationView_storyTargetCampaignFunc);
        }
        break;
    case 2:
    case 3:
    case 4:
        s->flag_29E = 1;
        infoOutWith(self + 0x38C, VT_changeStageInfo_45, {self, campaignView});
        runPanel(lib::AdventurePanel_runFadeOutAnimation, self + 0x2BC);
        target = int32_t(s->field_298);
        setFilter(filter, lib::CampaignAnimationView_eventTargetCampaignFunc);
        break;
    case 5:
        s->flag_29E = 1;
        infoOutWith(self + 0x398, VT_changeStageInfo_46, {self, campaignView});
        runPanel(lib::AdventurePanel_runFadeOutAnimation, self + 0x2BC);
        target = int32_t(s->field_298);
        setFilter(filter, lib::CampaignAnimationView_colosseumTargetCampaignFunc);
        break;
    case 7:
        s->flag_29E = 1;
        infoOutWith(self + 0x374, VT_changeStageInfo_47, {self});
        break;
    default:
        break;
    }

    // the campaign banner
    uint32_t announce = s->announce.node;
    if (ld32(filter + 16)) {
        setVisible(announce, 1);
        uint32_t campaigns = f.alloc(12);           // std::vector<shared_ptr<const master::EvCampaign>>
        st32(campaigns, 0);
        st32(campaigns + 4, 0);
        st32(campaigns + 8, 0);
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t ids = api + API_CAMPAIGN_IDS;
        if (ld32(ids + 4) != ld32(ids)) {
            uint32_t entry = f.alloc(8);
            for (uint32_t k = 0;;) {
                call(lib::EvCampaign_create, {entry, ld32(ld32(ids) + k * 4)});
                if (ld32(entry) && call(lib::CampaignFilter_call, {filter, uint32_t(target), entry}) == 1) {
                    uint32_t end = ld32(campaigns + 4);
                    if (end == ld32(campaigns + 8)) {
                        call(lib::CampaignVector_pushBackSlow, {campaigns, entry});
                    } else {
                        if (end) {
                            st32(end, ld32(entry));
                            uint32_t ctrl = ld32(entry + 4);
                            st32(end + 4, ctrl);
                            if (ctrl) call(fn::shared_add, {ctrl});
                        }
                        st32(campaigns + 4, ld32(campaigns + 4) + 8);
                    }
                }
                if (ld32(entry + 4)) call(fn::shared_release, {ld32(entry + 4)});
                k++;
                if (k >= uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2)) break;
            }
        }
        if (campaignView) {
            call(lib::CampaignAnimationView_setCampaings, {campaignView, campaigns});
        } else {
            uint32_t view = call(lib::CampaignAnimationView_create, {s->announce.node, campaigns});
            uint32_t parent = s->announce.node;
            Frame g;
            uint32_t name = newStr(g, STR_campaignView, 0xC);
            vcall(parent, node::addChildZName, {parent, view, 0, name});
            strFree(name);
            if (s->flag_29C) call(lib::CampaignAnimationView_hide, {view});
        }
        call(lib::CampaignVector_dtor, {campaigns});
    } else {
        setVisible(announce, 0);
    }
    call(lib::IntVector_dtor, {story + 0xC});
    functionDestroy(filter);
}
RW_FUNCTION(0x723088, "SceneAdventureSelect::changeStageInfo", SceneAdventureSelect::changeStageInfo);
