// SceneAdventureSelect: starting a quest - the AP cost of the selected stage (with the running campaigns) and
// the checks before the deck dialog.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {
constexpr uint32_t STAGE_AP = 0x11C;                     // master::Stage SafeBuffer: AP cost
constexpr uint32_t STAGE_RAID_SETTING = 0x3EC;           // master::Stage SafeBuffer: RaidSetting id
constexpr uint32_t STAGE_114 = 0x114;                    // master::Stage SafeBuffer
constexpr uint32_t RAID_SETTING_AP = 4;                  // master::RaidSetting SafeBuffer: AP (first battle)
constexpr uint32_t RAID_SETTING_AP_AGAIN = 0x10;         // ... AP when the boss was already fought
constexpr uint32_t API_STORY_CTRL = 0x128;               // shared_ptr control block of the story data at +0x124
constexpr uint32_t SM_RAID_ID = 0x2D0;                   // two words: the raid boss's id
constexpr uint32_t SM_RAID_AP = 0x2E4;
constexpr uint32_t SM_RAID_STAGE_114 = 0x2E8;
constexpr uint32_t SM_RAID_AGAIN = 0x2EC;                // byte
constexpr uint32_t RAID_INFO_LEVEL = 0x2C;               // RaidBase::Info word given as the campaign id of a raid
                                                         // (same offset as RAID_INFO_STAGE of the lib header)
constexpr uint32_t RAID_INFO_FOUGHT = 0x3C;              // RaidBase::Info: 1 = first battle against the boss

// SceneUtil::getCampaignPoint(base, kind, id, APIManager's campaign ids, 7, false)
int32_t campaignPoint(uint32_t base, uint32_t kind, uint32_t id, uint32_t api) {
    return SceneUtil::getCampaignPoint(int32_t(base), int32_t(kind), int32_t(id), api + API_CAMPAIGN_IDS, 7, 0);
}
}  // namespace

// Start check: too many medals -> the medal over dialog; AP above the maximum -> "not enough room" message;
// not enough AP -> the AP charge dialog; else `done`.
void SceneAdventureSelect::checkAp(Addr, int32_t ap, Addr done) {
    if (SceneUtil::isOverMedalSlot(0) == 1) {
        Frame f;
        uint32_t none = newEmptyFunction(f);
        SceneUtil::openMedalOverDialog(0, 5, none);
        functionDestroy(none);
        return;
    }
    if (SceneUtil::isMaxApOk(ap) == 1) {
        if (SceneUtil::isApOk(uint32_t(ap)) == 1) {
            call(fn::function_vv_call, {done});
            return;
        }
        playSE(SE_ERROR);
        Frame f;
        uint32_t cb = newFunction(f, VT_checkAp_64, {});
        SceneUtil::openChargeApDialog(cb);
        functionDestroy(cb);
        return;
    }
    playSE(SE_ERROR);
    Frame f;
    uint32_t text = textData(f, 102610001);
    uint32_t msg = newStr(f, textDataStr(text));
    uint32_t none = newEmptyFunction(f);
    SceneUtil::openOkDialog4Line(msg, none, 0);
    functionDestroy(none);
    strFree(msg);
    sharedRelease(text);
}
RW_FUNCTION(0x72763C, "SceneAdventureSelect::checkAp", SceneAdventureSelect::checkAp);

// A story quest: its AP (or, on the stage the raid boss is on, the raid's AP: the scene switches to raid mode
// 6 and SceneManager keeps the boss), then checkAp with lambda $_61.
void SceneAdventureSelect::checkApStory(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    playSE(SE_DECIDE);
    s->adventureType = 0;
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t storyCtrl = ld32(api + API_STORY_CTRL);
    uint32_t storyData = ld32(api + API_STORIES);
    if (storyCtrl) call(fn::shared_add, {storyCtrl});
    Frame f;
    uint32_t stories = f.alloc(12);
    call(lib::StoryVector_copy, {stories, storyData});
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
            index = 0;
            if (int32_t(k) >= int32_t(count)) break;
        }
    }
    if (count <= index) {
        call(lib::THROW_OUT_OF_RANGE, {stories});
        return;
    }
    uint32_t story = begin + index * STORY_SIZE;
    uint32_t base = ld32(story + 4);
    uint32_t info = ld32(call(fn::APIManager_getInstance, {}) + API_RAID_INFO);
    uint32_t lo = ld32(info), hi = ld32(info + 4);
    uint32_t last = ld32(storyData + STORY_DATA_CURRENT);
    if (last == 0) {
        uint32_t n = storyCount(ld32(stories), ld32(stories + 4));
        if (n - 1 >= n) {
            call(lib::THROW_OUT_OF_RANGE, {stories});
            return;
        }
        last = ld32(ld32(stories) + (n - 1) * STORY_SIZE);
    }
    bool raid = false;
    if ((lo | hi) != 0 && ld32(ld32(call(fn::APIManager_getInstance, {}) + API_RAID_INFO) + RAID_INFO_HP) != 0 &&
        s->field_298 == last) {
        // the raid boss is on this stage
        s->field_298 = 0;
        uint32_t mgr = sceneManager();
        st32(mgr + SM_RAID_ID + 4, ld32(info + 4));
        st32(mgr + SM_RAID_ID, ld32(info));
        base = ld32(info + 0xC);
        Frame g;
        uint32_t stage = masterRow(g, lib::Stage_create, ld32(info + 0x24));
        int32_t settingId = safe32(ld32(stage), STAGE_RAID_SETTING);
        uint32_t setting = masterRow(g, lib::RaidSetting_create, uint32_t(settingId));
        uint32_t fought = ld32(info + RAID_INFO_FOUGHT);
        st8(sceneManager() + SM_RAID_AGAIN, fought == 1 ? 0 : 1);
        int32_t raidAp;
        if (ld8(sceneManager() + SM_RAID_AGAIN)) raidAp = safe32(ld32(setting), RAID_SETTING_AP_AGAIN);
        else raidAp = safe32(ld32(setting), RAID_SETTING_AP);
        st32(sceneManager() + SM_RAID_AP, uint32_t(raidAp));
        int32_t v114 = safe32(ld32(stage), STAGE_114);
        st32(sceneManager() + SM_RAID_STAGE_114, uint32_t(v114));
        st32(sceneManager() + SM_ADVENTURE_MODE, 6);
        s->adventureType = 6;
        if (ld32(setting + 4)) call(fn::shared_release, {ld32(setting + 4)});
        if (ld32(stage + 4)) call(fn::shared_release, {ld32(stage + 4)});
        raid = true;
    } else {
        st32(sceneManager() + SM_ADVENTURE_MODE, 1);
        s->adventureType = 0;
    }
    uint32_t api2 = call(fn::APIManager_getInstance, {});
    uint32_t stage = masterRow(f, lib::Stage_create, ld32(story));
    uint32_t kind = 4;
    int32_t ap = safe32(ld32(stage), STAGE_AP);
    uint32_t id;
    if (raid) {
        id = ld32(ld32(call(fn::APIManager_getInstance, {}) + API_RAID_INFO) + RAID_INFO_LEVEL);
    } else {
        kind = uint32_t(ap);
        id = ld32(story);
    }
    int32_t points = campaignPoint(base, kind, id, api2);
    {
        Frame g;
        uint32_t cb = newFunction(g, VT_checkApStory_61, {self});
        checkAp(self, points, cb);
        functionDestroy(cb);
    }
    s->flag_325 = 1;
    if (ld32(stage + 4)) call(fn::shared_release, {ld32(stage + 4)});
    call(lib::StoryVector_dtor, {stories});
    if (storyCtrl) call(fn::shared_release, {storyCtrl});
}
RW_FUNCTION(0x727288, "SceneAdventureSelect::checkApStory", SceneAdventureSelect::checkApStory);

// An event quest: the event holding the selected stage becomes SceneManager's event; checkAp with lambda $_62.
// The stage copy is the original's frame slot: when no event holds the stage, its id / base are whatever that
// stack slot held (the frame is reproduced so they are the same bytes).
void SceneAdventureSelect::checkApEvent(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    uint32_t sp = f.allocExact(0x88);        // push {r4-r7, lr} + sub sp, #0x74
    playSE(SE_DECIDE);
    uint32_t story = sp + 0x2C;               // id, base, word, vector<int> at +0xC
    st32(story + 0xC, 0);
    st32(story + 0x10, 0);
    st32(story + 0x14, 0);
    uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_STAGE_EVENTS);
    uint32_t evBegin = ld32(events);
    int32_t bytes = int32_t(ld32(events + 4) - evBegin);
    if (bytes >= 1) {
        uint32_t count = eventCount(events);
        bool found = false;
        for (uint32_t i = 0; !found;) {
            if (count <= i) {
                call(lib::THROW_OUT_OF_RANGE, {events});
                return;
            }
            uint32_t ev = evBegin + i * EVENT_SIZE;
            uint32_t stage = ld32(ev);
            int32_t stageBytes = int32_t(ld32(ev + 4) - stage);
            if (stageBytes >= 1) {
                uint32_t n = uint32_t(stageBytes >> 3) * 0xAAAAAAABu;
                for (uint32_t j = 0;;) {
                    if (n <= j) {
                        call(lib::THROW_OUT_OF_RANGE, {ev});
                        return;
                    }
                    if (ld32(stage) == s->field_298) {
                        st32(story, ld32(stage));
                        st32(story + 4, ld32(stage + 4));
                        st32(story + 8, ld32(stage + 8));
                        call(lib::IntVector_assign, {story + 0xC, ld32(stage + 0xC), ld32(stage + 0x10)});
                        uint32_t eventId = ld32(ev + EVENT_STAGE_ID);
                        st32(sceneManager() + SM_EVENT_ID, eventId);
                        found = true;
                        break;
                    }
                    stage += STORY_SIZE;
                    j++;
                    if (int32_t(j) >= int32_t(n)) break;
                }
            }
            if (found) break;
            i++;
            if (int32_t(i) >= int32_t(count)) break;
        }
    }
    uint32_t row = masterRow(f, lib::Stage_create, ld32(story));
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t base = ld32(story + 4);
    int32_t ap = safe32(ld32(row), STAGE_AP);
    int32_t points = campaignPoint(base, uint32_t(ap), ld32(story), api);
    {
        Frame g;
        uint32_t cb = newFunction(g, VT_checkApEvent_62, {self});
        checkAp(self, points, cb);
        functionDestroy(cb);
    }
    s->flag_325 = 1;
    if (ld32(row + 4)) call(fn::shared_release, {ld32(row + 4)});
    call(lib::IntVector_dtor, {story + 0xC});
}
RW_FUNCTION(0x727858, "SceneAdventureSelect::checkApEvent", SceneAdventureSelect::checkApEvent);

// A colosseum floor (field_298 is the floor index): checkAp with lambda $_63 {floor's 4 words, self}.
void SceneAdventureSelect::checkApColosseum(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    playSE(SE_DECIDE);
    uint32_t info = ld32(call(fn::APIManager_getInstance, {}) + API_COLOSSEUM);
    uint32_t begin = ld32(info + 8);
    uint32_t count = uint32_t(int32_t(ld32(info + 0xC) - begin) >> 4);
    uint32_t floor = s->field_298;
    if (count <= floor) {
        call(lib::THROW_OUT_OF_RANGE, {info + 8});
        return;
    }
    uint32_t entry = begin + floor * 16;
    uint32_t w0 = ld32(entry), w1 = ld32(entry + 4), stageId = ld32(entry + 8), base = ld32(entry + 0xC);
    Frame f;
    uint32_t row = masterRow(f, lib::Stage_create, stageId);
    uint32_t api = call(fn::APIManager_getInstance, {});
    int32_t ap = safe32(ld32(row), STAGE_AP);
    int32_t points = campaignPoint(base, uint32_t(ap), stageId, api);
    {
        Frame g;
        uint32_t cb = g.alloc(20);
        heapLambda(cb, VT_checkApColosseum_63, 0x18, {w0, w1, stageId, base, self});
        checkAp(self, points, cb);
        functionDestroy(cb);
    }
    s->flag_325 = 1;
    if (ld32(row + 4)) call(fn::shared_release, {ld32(row + 4)});
}
RW_FUNCTION(0x727A74, "SceneAdventureSelect::checkApColosseum", SceneAdventureSelect::checkApColosseum);
