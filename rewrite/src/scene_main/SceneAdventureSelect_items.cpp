// SceneAdventureSelect: filling the stage scroll view (story stages, event stages, colosseum floors) and choosing
// the stage selected in it.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {

// std::vector<int> on the guest stack
struct IntVec {
    uint32_t addr;
    uint32_t begin() const { return ld32(addr); }
    uint32_t end() const { return ld32(addr + 4); }
    uint32_t count() const { return uint32_t(int32_t(end() - begin()) >> 2); }
};

// vec.push_back(*value) as libc++ inlines it (slow path when full)
void pushBack(uint32_t vec, uint32_t value) {
    uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(lib::IntVector_pushBackSlow, {vec, value});
        return;
    }
    if (end) st32(end, ld32(value));
    st32(vec + 4, (end ? end : 0) + 4);
}

// view->setItemIDs(std::vector<int>(ids))
void setItemIDs(uint32_t view, uint32_t ids) {
    Frame f;
    uint32_t copy = f.alloc(12);
    call(lib::IntVector_copy, {copy, ids});
    call(lib::BaseStageScrollView_setItemIDs, {view, copy});
    call(lib::IntVector_dtor, {copy});
}

}  // namespace

// Select the stage to show for `type`: the last item (story), the first (events, colosseum, select) or the last
// for a kind-3 event; when coming back to the list (flag_2E4) the item SceneManager remembered (story / events).
// The selected id goes to field_298 (-1: none) and the view's index / selection.
void SceneAdventureSelect::initSelectStage(Addr self, int32_t type) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    IntVec ids{f.alloc(12)};
    call(lib::BaseStageScrollView_getItemIDs, {ids.addr, s->stageScrollView});
    uint32_t t = uint32_t(type);
    int32_t index = int32_t(ids.count() - 1);
    if (t <= 7 && ((1u << t) & 0x9C) != 0) {     // 2, 3, 4 events, 7 adventure select
        index = 0;
        if (t != 7) {
            uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_STAGE_EVENTS);
            uint32_t at = uint32_t(s->eventIndex);
            if (eventCount(events) <= at) {
                call(lib::THROW_OUT_OF_RANGE, {events});
                return;
            }
            uint32_t row = masterRow(f, lib::EvStage_create, ld32(ld32(events) + at * EVENT_SIZE + EVENT_STAGE_ID));
            index = 0;
            if (ld32(row) && safe32(ld32(row), EVSTAGE_KIND) == 3) index = int32_t(ids.count() - 1);
            if (ld32(row + 4)) call(fn::shared_release, {ld32(row + 4)});
        }
    }
    if (s->flag_2E4) {
        int32_t fallback = index;
        s->flag_2E4 = 0;
        int32_t remembered = -1;
        if (t <= 4 && t != 1) remembered = int32_t(ld32(sceneManager() + SM_STORY_STAGE));
        index = fallback;
        if (ids.end() != ids.begin()) {
            uint32_t n = ids.count();
            for (uint32_t k = 0;;) {
                if (remembered == int32_t(ld32(ids.begin() + k * 4))) {
                    index = int32_t(k);
                    break;
                }
                k++;
                if (k >= n) break;
            }
        }
    }
    if (t == 7) {
        s->field_298 = 0;
    } else if (index < 0 || ids.count() <= uint32_t(index)) {
        s->field_298 = 0xFFFFFFFFu;
    } else {
        if (t == 0) call(lib::BaseStageScrollView_setIndex, {s->stageScrollView, 0xFFFFFFFEu});
        if (ids.count() <= uint32_t(index)) {
            call(lib::THROW_OUT_OF_RANGE, {ids.addr});
            return;
        }
        call(lib::AdvStageScrollView_setSelectItem, {s->stageScrollView, ld32(ids.begin() + uint32_t(index) * 4)});
        if (ids.count() <= uint32_t(index)) {
            call(lib::THROW_OUT_OF_RANGE, {ids.addr});
            return;
        }
        s->field_298 = ld32(ids.begin() + uint32_t(index) * 4);
        call(lib::BaseStageScrollView_setIndex, {s->stageScrollView, uint32_t(index)});
    }
    call(lib::IntVector_dtor, {ids.addr});
}
RW_FUNCTION(0x72550C, "SceneAdventureSelect::initSelectStage", SceneAdventureSelect::initSelectStage);

// Colosseum: one item per floor (0 .. floors - 1) while the colosseum is open.
void SceneAdventureSelect::setStageScrollViewItemsColosseum(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    uint32_t items = f.alloc(12);
    st32(items, 0);
    st32(items + 4, 0);
    st32(items + 8, 0);
    call(lib::AdvStageScrollView_setStageType, {s->stageScrollView, 2});
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + API_COLOSSEUM_CTRL);
    uint32_t info = ld32(api + API_COLOSSEUM);
    if (ctrl) call(fn::shared_add, {ctrl});
    if (ld32(info + 4) == 1) {
        uint32_t api2 = call(fn::APIManager_getInstance, {});
        uint32_t stages = f.alloc(12);
        call(lib::ColosseumStageVector_copy, {stages, ld32(api2 + API_COLOSSEUM) + 8});
        uint32_t floor = f.alloc(4);
        st32(floor, 0);
        if (int32_t(ld32(stages + 4) - ld32(stages)) >= 1) {
            for (uint32_t k = 1;; k++) {
                uint32_t end = ld32(items + 4);
                if (end == ld32(items + 8)) {
                    call(lib::IntVector_pushBackSlow, {items, floor});
                } else {
                    if (end) st32(end, k - 1);
                    st32(items + 4, (end ? ld32(items + 4) : 0) + 4);
                }
                st32(floor, k);
                if (int32_t(k) >= (int32_t(ld32(stages + 4) - ld32(stages)) >> 4)) break;
            }
        }
        call(lib::ColosseumStageVector_dtor, {stages});
    }
    setItemIDs(s->stageScrollView, items);
    if (ctrl) call(fn::shared_release, {ctrl});
    call(lib::IntVector_dtor, {items});
}
RW_FUNCTION(0x7253CC, "SceneAdventureSelect::setStageScrollViewItemsColosseum", SceneAdventureSelect::setStageScrollViewItemsColosseum);

// Event stages: every stage of the event; for a kind-3 event with a target stage (event +0x30) only the stages
// from the target on, starting at it (the event's chain of stages); for a kind-3 event the list is left on the
// last but one stage when coming back.
void SceneAdventureSelect::setStageScrollViewItemsEvent(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    uint32_t items = f.alloc(12);
    st32(items, 0);
    st32(items + 4, 0);
    st32(items + 8, 0);
    call(lib::AdvStageScrollView_setStageType, {s->stageScrollView, 1});
    uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_STAGE_EVENTS);
    uint32_t at = uint32_t(s->eventIndex);
    if (eventCount(events) <= at) {
        call(lib::THROW_OUT_OF_RANGE, {events});
        return;
    }
    uint32_t ev = ld32(events) + at * EVENT_SIZE;
    uint32_t row = masterRow(f, lib::EvStage_create, ld32(ev + EVENT_STAGE_ID));
    if (ld32(ev + 0x24) != 2 && int32_t(ld32(ev + 4) - ld32(ev)) >= 1) {
        bool before = true;     // still before the target stage
        bool inChain = false;   // the previous stage was taken
        for (uint32_t k = 0, off = 0;; k++, off += STORY_SIZE) {
            uint32_t begin = ld32(ev), end = ld32(ev + 4);
            bool kind3 = ld32(row) && safe32(ld32(row), EVSTAGE_KIND) == 3;
            if (!kind3) {
                if (storyCount(begin, end) <= k) {
                    call(lib::THROW_OUT_OF_RANGE, {ev});
                    return;
                }
                pushBack(items, begin + off);
            } else {
                int32_t targetId = int32_t(ld32(ev + 0x30));
                if (targetId < 1) {
                    if (k == 0) {
                        if (ld32(ev + 4) == ld32(ev)) {
                            call(lib::THROW_OUT_OF_RANGE, {ev});
                            return;
                        }
                        pushBack(items, ld32(ev));
                    }
                } else {
                    begin = ld32(ev);
                    if (storyCount(begin, ld32(ev + 4)) <= k) {
                        call(lib::THROW_OUT_OF_RANGE, {ev});
                        return;
                    }
                    uint32_t stage = begin + off;
                    if (int32_t(ld32(stage)) == targetId) {
                        before = false;
                        pushBack(items, stage);
                        inChain = true;
                    } else if (inChain && !before) {
                        pushBack(items, stage);
                        inChain = false;
                        before = false;
                    } else if (before && !inChain) {
                        pushBack(items, stage);
                        inChain = false;
                    }
                }
            }
            if (int32_t(k + 1) >= int32_t(storyCount(ld32(ev), ld32(ev + 4)))) break;
        }
    }
    setItemIDs(s->stageScrollView, items);
    if (ld32(row) && safe32(ld32(row), EVSTAGE_KIND) == 3 && s->flag_29C) {
        int32_t bytes = int32_t(ld32(items + 4) - ld32(items));
        if (bytes >= 5) {
            uint32_t n = uint32_t(bytes >> 2), i = n - 2;
            if (i >= n) {
                call(lib::THROW_OUT_OF_RANGE, {items});
                return;
            }
            uint32_t id = ld32(ld32(items) + i * 4);
            st32(sceneManager() + SM_STORY_STAGE, id);
        }
    }
    if (ld32(row + 4)) call(fn::shared_release, {ld32(row + 4)});
    call(lib::IntVector_dtor, {items});
}
RW_FUNCTION(0x7250D8, "SceneAdventureSelect::setStageScrollViewItemsEvent", SceneAdventureSelect::setStageScrollViewItemsEvent);

namespace {
constexpr uint32_t STAGE_BOSS_FLAG = 0x150;               // master::Stage SafeBuffer: a boss stage
constexpr uint32_t STAGE_FLAG_124 = 0x124;                // master::Stage SafeBuffer
constexpr uint32_t ENEMY_ATTRIBUTE = 0x278;               // master::Enemy SafeBuffer: attribute (1..3)
constexpr uint32_t FILEUTILS_GET_DATA = 0x10;             // FileUtils virtual getDataFromFile(Data out, string)
}  // namespace

// Story stages, passed through the adventure select filter (AppState: byte 1 all clear-state, 2 / 3 / 4 the
// stage state classes, 5 / 6 / 7 the boss attributes): a stage is listed unless the filter hides it; the last
// stage is always listed (and the last but one when flag_29C asks for it on an empty list). The remembered stage
// (SceneManager +0x2C8) moves to the next listed stage when it was filtered out.
void SceneAdventureSelect::setStageScrollViewItemsStory(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    uint32_t items = f.alloc(12);
    st32(items, 0);
    st32(items + 4, 0);
    st32(items + 8, 0);
    call(lib::AdvStageScrollView_setStageType, {s->stageScrollView, 0});
    uint32_t stories = f.alloc(12);
    call(lib::StoryVector_copy, {stories, ld32(call(fn::APIManager_getInstance, {}) + API_STORIES)});
    uint32_t app = call(fn::AppState_getInstance, {});
    uint32_t filterState = f.alloc(8);
    call(lib::AppState_getAdventureSelectFilterState, {filterState, app});
    uint32_t state = ld32(filterState);
    uint8_t b1 = ld8(state + 1), b2 = ld8(state + 2), b3 = ld8(state + 3), b4 = ld8(state + 4);
    uint8_t b5 = ld8(state + 5), b6 = ld8(state + 6), b7 = ld8(state + 7);
    int32_t remembered = int32_t(ld32(sceneManager() + SM_STORY_STAGE));

    // the filter flags (each 0 / 1)
    uint32_t allClear, classA, classB, classC, attr7, attr6, attr5, filtered;
    attr7 = b7 != 0;
    attr6 = b6 != 0;
    attr5 = b5 != 0;
    if (uint8_t(b2 | b1 | b3 | b4 | b5 | b6 | b7) == 0) {
        classC = b4 != 0;
        classB = b3 != 0;
        classA = b2 != 0;
        allClear = b1 != 0;
        filtered = 0;
    } else {
        uint32_t r7 = b1 != 0, a = b2 != 0, r6 = b3 != 0, r4 = b4 != 0;
        if (!b1 && !b2) {
            r7 = 1;
            a = 1;
        }
        if (!b3 && !b4) {
            r6 = 1;
            r4 = 1;
        }
        allClear = r7;
        classA = a;
        classB = r6;
        classC = r4;
        if (b5) {
            attr5 = 1;
        } else {
            attr7 = (b7 != 0) | (b6 == 0);
            attr6 = (b6 != 0) | (b7 == 0);
            attr5 = (b6 | b7) == 0;
        }
        filtered = 1;
    }

    uint32_t begin = ld32(stories), end = ld32(stories + 4);
    if (int32_t(end - begin) >= 1) {
        uint32_t notAttr7 = attr7 ^ 1, notAttr6 = attr6 ^ 1, notAttr5 = attr5 ^ 1, notClassC = classC ^ 1;
        uint32_t notClassA = classA ^ 1;
        uint32_t pending = 0;     // the remembered stage was skipped: move it to the next listed one
        uint32_t count = storyCount(begin, end);
        uint32_t story = f.alloc(24);   // the current Stage::Story copy: id, 2 words, vector<int> at +0xC
        for (uint32_t k = 0, off = 0;;) {
            if (count <= k) {
                call(lib::THROW_OUT_OF_RANGE, {stories});
                return;
            }
            uint32_t elem = ld32(stories) + off;
            st32(story, ld32(elem));
            st32(story + 4, ld32(elem + 4));
            st32(story + 8, ld32(elem + 8));
            call(lib::IntVector_copy, {story + 0xC, elem + 0xC});
            int32_t id = int32_t(ld32(story));
            uint32_t n = storyCount(ld32(stories), ld32(stories + 4));
            if (filtered != 1 || k == n - 1) {
                pushBack(items, story);
            } else if (k == n - 2 && s->flag_29C && ld32(items + 4) == ld32(items)) {
                pushBack(items, story);
            } else {
                uint32_t has3 = (ld32(story + 0x10) - ld32(story + 0xC)) == 12 ? 1 : 0;
                if (((has3 | notClassA) ^ (allClear & has3)) != 0) {
                    pending |= remembered == id ? 1 : 0;
                } else {
                    Frame g;
                    uint32_t stage = masterRow(g, lib::Stage_create, uint32_t(id));
                    bool lookAtBoss;
                    if (safe32(ld32(stage), STAGE_BOSS_FLAG) != 0) {
                        lookAtBoss = classB != 0;
                    } else {
                        uint32_t flag = safe32(ld32(stage), STAGE_FLAG_124) != 0 ? 1 : 0;
                        lookAtBoss = (classB & flag) == (flag | notClassC);
                    }
                    if (!lookAtBoss) {
                        pending |= remembered == id ? 1 : 0;
                    } else {
                        // the boss attribute from the stage map file (enemy id at +0x28, else +0x18)
                        uint32_t fu = call(lib::FileUtils_getInstance, {});
                        uint32_t getData = ld32(ld32(fu) + FILEUTILS_GET_DATA);
                        uint32_t buf = stringBuffer(g, {STR_stage_mappoi_stg_05d_bin, uint32_t(id)});
                        uint32_t file = newStr(g, buf);
                        uint32_t data = g.alloc(8);
                        call(getData, {data, fu, file});
                        strFree(file);
                        uint32_t enemyId = call(lib::StageManager_getInteger32bitFromLittleEndianBytes,
                                                {call(lib::Data_getBytes, {data}) + 0x28});
                        if (enemyId == 0)
                            enemyId = call(lib::StageManager_getInteger32bitFromLittleEndianBytes,
                                           {call(lib::Data_getBytes, {data}) + 0x18});
                        uint32_t enemy = masterRow(g, lib::Enemy_create, enemyId);
                        int32_t attribute = safe32(ld32(enemy), ENEMY_ATTRIBUTE);
                        uint32_t c3 = (attribute != 3 ? 1u : 0u) | notAttr7;
                        uint32_t c2 = (attribute != 2 ? 1u : 0u) | notAttr6;
                        uint32_t c1 = (attribute != 1 ? 1u : 0u) | notAttr5;
                        if (c1 != 0 && c2 == 1 && c3 != 0) {
                            pending |= remembered == id ? 1 : 0;
                        } else {
                            pushBack(items, story);
                            if (pending & 1) {
                                pending = 0;
                                uint32_t listed = ld32(story);
                                st32(sceneManager() + SM_STORY_STAGE, listed);
                            } else {
                                pending = 0;
                            }
                        }
                        if (ld32(enemy + 4)) call(fn::shared_release, {ld32(enemy + 4)});
                        call(lib::Data_dtor, {data});
                    }
                    if (ld32(stage + 4)) call(fn::shared_release, {ld32(stage + 4)});
                }
            }
            call(lib::IntVector_dtor, {story + 0xC});
            count = storyCount(ld32(stories), ld32(stories + 4));
            off += STORY_SIZE;
            k++;
            if (int32_t(k) >= int32_t(count)) break;
        }
    }
    setItemIDs(s->stageScrollView, items);
    if (s->flag_29C) {
        int32_t bytes = int32_t(ld32(items + 4) - ld32(items));
        if (bytes >= 5) {
            uint32_t n = uint32_t(bytes >> 2), i = n - 2;
            if (i >= n) {
                call(lib::THROW_OUT_OF_RANGE, {items});
                return;
            }
            uint32_t id = ld32(ld32(items) + i * 4);
            st32(sceneManager() + SM_STORY_STAGE, id);
        }
    }
    sharedRelease(filterState);
    call(lib::StoryVector_dtor, {stories});
    call(lib::IntVector_dtor, {items});
}
RW_FUNCTION(0x724AEC, "SceneAdventureSelect::setStageScrollViewItemsStory", SceneAdventureSelect::setStageScrollViewItemsStory);
