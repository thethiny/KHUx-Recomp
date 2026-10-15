// BattleResultScrollView: building the result list from the battle's outcome - materials, enemy drops and
// treasures (loadResultData), the stage's sub-mission / campaign rewards (loadStageResultData), colosseum rewards
// (loadColosseumResultData) - and the ResutData entries they add.
#include "BattleResultScrollView.hpp"

using namespace brsv;

namespace {
constexpr uint32_t TEXT_FIRST_CLEAR = 103010001;   // wording of a first-clear reward
constexpr uint32_t TEXT_MISSION = 103010002;       // wording of a mission reward
constexpr uint32_t TEXT_COLO_RANK = 103010009;     // colosseum rank reward wording (format)
constexpr uint32_t TEXT_COLO_REWARD = 103010007;   // colosseum reward wording (format)
constexpr uint32_t SM_BATTLE_KIND = 0x30C;         // SceneManager: 0 story, 2 event, 3 raid, ...
constexpr uint32_t API_STAGE_START = 0x13C, API_RAID_START = 0x178, API_EVENT_START = 0x260;
constexpr uint32_t API_STAGE_CLEAR = 0x144;        // StageClear (a SafeBuffer): data byte 0 = first clear
constexpr uint32_t API_COLOSSEUM_STAGE = 0x24C;    // int*: the colosseum stage id
constexpr uint32_t API_COLOSSEUM_RANKS = 0x268;    // vector<pair<int,int>>*: (rank, class) of the battle
constexpr uint32_t API_STAGE_STATE = 0x33C;        // embedded SafeBuffer: +4 current stage id
constexpr uint32_t APPSTATE_USER = 0x20;           // shared_ptr<SafeBuffer> user data

// a ResutData temporary on the guest stack: strings empty, the "New" / wording fields cleared
Addr newResult(Frame &f) {
    Addr r = f.alloc(Self::RESULT_SIZE);
    st8(r + 0x34, 0);
    st32(r + 0x38, 0);
    st32(r + 0x3C, 0);
    st32(r + 0x40, 0);
    call(g::aeabi_memset, {r + 4, 0x18, 0});
    return r;
}

// clear the result vector (destroying each entry from the back, the way the inlined clear() does)
void clearResults(Self *s) {
    uint32_t begin = s->results.begin;
    for (uint32_t end = s->results.end; end != begin; end = s->results.end) {
        s->results.end = end - Self::RESULT_SIZE;
        strFree(end - 0x0C);   // wording
        strFree(end - 0x34);   // name
        strFree(end - 0x40);   // image
    }
}

// (enemy drops / treasures) add each prize of an item list, or count it into an equal entry (same reward, slot)
void addPrizes(Self *s, Addr items, int32_t rewardId) {
    for (int32_t k = 0; k < 4; k++) {
        Addr item = items + 16 * uint32_t(k);
        if (!ld32(item + 8)) continue;
        uint32_t end = s->results.end;
        uint32_t p = s->results.begin;
        bool found = false;
        for (; p != end; p += Self::RESULT_SIZE) {
            if (int32_t(ld32(p + 0x1C)) == rewardId && int32_t(ld32(p + 0x24)) == k) {
                found = true;
                break;
            }
        }
        if (!found) {
            Self::addResultReward(s, rewardId, k);
        } else {
            st32(p + 0x28, ld32(p + 0x28) + 1);
            st32(p + 0x30, ld32(p + 0x30) + ld32(item + 8));
        }
    }
}

// one MapPrizes::Items list (4 x {1, 0, 0, false}) on the stack
Addr newPrizes(Frame &f) {
    Addr items = f.alloc(0x40);
    for (uint32_t k = 0; k < 4; k++) {
        *rw::frame_ptr<uint32_t>(items + 16 * k) = 1;
        *rw::frame_ptr<uint32_t>(items + 16 * k + 4) = 0;
        *rw::frame_ptr<uint32_t>(items + 16 * k + 8) = 0;
        *rw::frame_ptr<uint8_t>(items + 16 * k + 12) = 0;
    }
    return items;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ entries
// A material (type 5) obtained once.
void Self::addResultMaterial(Self *self, int32_t materialId) {
    Frame f;
    Addr r = newResult(f);
    st32(r, 5);
    call(g::SceneUtil_setItemImageData, {r + 0x10, r + 4, 5, uint32_t(materialId), 0});
    st32(r + 0x1C, uint32_t(materialId));
    st8(r + 0x20, 0);
    st8(r + 0x21, 0);
    st32(r + 0x24, 0);
    st32(r + 0x28, 1);
    st32(r + 0x2C, 1);
    st32(r + 0x30, 1);
    pushResult(self, r);
    destroyResult(r);
}
RW_FUNCTION(0x5139B0, "BattleResultScrollView::addResultMaterial(int)", BattleResultScrollView::addResultMaterial);

void Self::getImageName(Self * /*self*/, ResutData *data, int32_t id, uint32_t flag) {
    Addr d = rw::guest_addr(data);
    call(g::SceneUtil_setItemImageData, {d + 0x10, d + 4, uint32_t(int32_t(data->type)), uint32_t(id), flag});
}
RW_FUNCTION(0x513AF4, "BattleResultScrollView::getImageName(ResutData&, int, bool)", BattleResultScrollView::getImageName);

// Prize `slot` of master::Reward `rewardId` (a treasure prize) - unless the reward has fewer slots or the slot's
// item type is 8, 9 or 13.
void Self::addResultReward(Self *self, int32_t rewardId, int32_t slot) {
    Frame f;
    Addr reward = f.alloc(8);
    call(g::Reward_create, {reward, uint32_t(rewardId)});
    uint32_t sb = ld32(reward);
    Addr word = f.alloc(4);
    sbRead(sb, word, 4, 4);
    if (int32_t(ld32(word)) > slot) {
        Addr r = newResult(f);
        Addr types = f.alloc(0x10);
        sbRead(ld32(reward), types, 0x18, 0x10);
        uint32_t type = ld32(types + 4 * uint32_t(slot));
        st32(r, type);
        if (type <= 13 && ((1u << type) & 0x2300u)) {
            destroyResult(r);
        } else {
            Addr ids = f.alloc(0x10);
            sbRead(ld32(reward), ids, 0x28, 0x10);
            call(g::SceneUtil_setItemImageData, {r + 0x10, r + 4, ld32(r), ld32(ids + 4 * uint32_t(slot)), 1});
            st32(r + 0x1C, uint32_t(rewardId));
            st8(r + 0x20, 0);
            st8(r + 0x21, 1);
            st32(r + 0x24, uint32_t(slot));
            Addr counts = f.alloc(0x10);
            sbRead(ld32(reward), counts, 8, 0x10);
            st32(r + 0x28, 1);
            st32(r + 0x2C, ld32(counts + 4 * uint32_t(slot)));
            Addr amounts = f.alloc(0x10);
            sbRead(ld32(reward), amounts, 0x68, 0x10);
            st32(r + 0x30, ld32(amounts + 4 * uint32_t(slot)));
            pushResult(self, r);
            destroyResult(r);
        }
    }
    sharedRelease(reward);
}
RW_FUNCTION(0x513B1C, "BattleResultScrollView::addResultReward(int, int)", BattleResultScrollView::addResultReward);

// (loadStageResultData's helper) a sub-mission (mission = 1) or first-clear reward of item type `type` and id `id`,
// with `amount`; keyblades (type 13) are not listed. this_ = {view}.
static void addStageReward(uint32_t this_, int32_t type, uint32_t id, uint32_t amount, uint32_t mission) {
    Self *s = rw::host_ptr<Self>(ld32(this_));
    Frame f;
    Addr r = newResult(f);
    st32(r, uint32_t(type));
    if (type != 13) {
        call(g::SceneUtil_setItemImageData, {r + 0x10, r + 4, uint32_t(type), id, 1});
        st32(r + 0x1C, id);
        st8(r + 0x20, 0);
        st8(r + 0x21, 0);
        st32(r + 0x24, 0);
        st32(r + 0x28, 1);
        st32(r + 0x2C, 1);
        st32(r + 0x30, amount);
        st8(r + 0x34, 1);
        Addr text = f.alloc(8);
        call(g::TextData_get, {text, 0, mission ? TEXT_MISSION : TEXT_FIRST_CLEAR});
        uint32_t str = ld32(ld32(text) + 4);
        call(g::string_assign, {r + 0x38, str, call(g::strlen_, {str})});
        sharedRelease(text);
        pushResult(s, r);
    }
    destroyResult(r);
}
RW_FUNCTION(0x5147E0, "BattleResultScrollView::loadStageResultData add a reward (outlined lambda)", addStageReward);

// ------------------------------------------------------------------------------------------------ loading
uint32_t Self::loadResultData(Self *self) {
    clearResults(self);
    Frame f;
    uint32_t state = call(g::AppState_getInstance, {});
    uint32_t user = ld32(state + APPSTATE_USER);
    uint32_t userCtrl = ld32(state + APPSTATE_USER + 4);
    if (userCtrl) call(g::shared_add, {userCtrl});
    // materials obtained: 1000 x {material id, ?}
    Addr materials = f.alloc(0x1F40);
    sbRead(user, materials, 0x38, 0x1F40);
    for (uint32_t k = 0; k < 1000; k++) {
        int32_t id = int32_t(ld32(materials + 8 * k));
        if (id == -1) continue;
        uint32_t p = self->results.begin, end = self->results.end;
        for (; p != end; p += RESULT_SIZE)
            if (int32_t(ld32(p + 0x1C)) == id) break;
        if (p == end) {
            addResultMaterial(self, id);
        } else {
            st32(p + 0x28, ld32(p + 0x28) + 1);
            st32(p + 0x30, ld32(p + 0x30) + 1);
        }
    }
    // campaign bonuses of the stage played
    Addr campaigns = f.alloc(12);
    *rw::frame_ptr<uint32_t>(campaigns) = 0;
    *rw::frame_ptr<uint32_t>(campaigns + 4) = 0;
    *rw::frame_ptr<uint32_t>(campaigns + 8) = 0;
    uint32_t stage = 0, stageCtrl = 0;
    int32_t stageId = 0;
    int32_t kind = int32_t(ld32(call(g::SceneManager_getInstance, {}) + SM_BATTLE_KIND));
    uint32_t startField = 0, campaignsOffset = 0;
    if (kind == 0) startField = API_STAGE_START, campaignsOffset = 0x58;
    else if (kind == 2) startField = API_EVENT_START, campaignsOffset = 0x40;
    else if (kind == 3) startField = API_RAID_START, campaignsOffset = 0x68;
    if (startField) {
        uint32_t start = call(g::APIManager_getInstance, {}) + startField;
        uint32_t info = ld32(start);
        Addr row = f.alloc(8);
        call(g::Stage_create, {row, ld32(info + 0x30)});
        stage = ld32(row);
        stageCtrl = ld32(row + 4);
        stageId = int32_t(ld32(info + 0x30));
        uint32_t src = ld32(start) + campaignsOffset;
        if (campaigns != src) call(g::vector_int_assign, {campaigns, ld32(src), ld32(src + 4)});
        if (stage) {
            uint32_t end = self->results.end;
            Addr word = f.alloc(4);
            for (uint32_t p = self->results.begin; p != end; p += RESULT_SIZE) {
                uint32_t amount = ld32(p + 0x30);
                sbRead(stage, word, 0x11C, 4);
                st32(p + 0x30, call(g::SceneUtil_getCampaignPoint,
                                    {amount, ld32(word), uint32_t(stageId), campaigns, 4, 1}));
            }
        }
    }
    // enemy drops: 1000 x {enemy, reward}
    Addr drops = f.alloc(0x1F40);
    sbRead(user, drops, 0x1F78, 0x1F40);
    for (uint32_t k = 0; k < 1000; k++) {
        Addr e = drops + 8 * k;
        if (ld32(e) == 0xFFFFFFFFu) continue;
        Frame g2;
        Addr items = newPrizes(g2);
        call(g::StageUtil_getEnemyDropPrizes, {items, ld32(e), ld32(e + 4), 0});
        addPrizes(self, items, int32_t(ld32(e + 4)));
    }
    // treasures: 100 x {box, reward}
    Addr treasures = f.alloc(0x320);
    sbRead(user, treasures, 0x3EB8, 0x320);
    for (uint32_t k = 0; k < 100; k++) {
        Addr e = treasures + 8 * k;
        if (ld32(e) == 0xFFFFFFFFu) continue;
        Frame g2;
        Addr items = newPrizes(g2);
        call(g::StageUtil_getTreasurePrizes, {items, ld32(e), ld32(e + 4), 0});
        addPrizes(self, items, int32_t(ld32(e + 4)));
    }
    Addr counter = f.alloc(4);
    uint32_t n = fillPending(self, counter);
    call(g::vector_int_dtor, {campaigns});
    if (stageCtrl) call(fn::shared_release, {stageCtrl});
    if (userCtrl) call(fn::shared_release, {userCtrl});
    return n;
}
RW_FUNCTION(0x513D84, "BattleResultScrollView::loadResultData()", BattleResultScrollView::loadResultData);

namespace {
// libc++ lower_bound over a sorted std::vector<int> [begin, end) for the first value > key (upper_bound shape:
// `*mid <= key` moves right); returns the position.
uint32_t upperBound(uint32_t begin, uint32_t end, int32_t key) {
    int32_t len = int32_t(end - begin) >> 2;
    uint32_t p = begin;
    while (len != 0) {
        int32_t half = (len + int32_t(uint32_t(len) >> 31)) >> 1;
        uint32_t mid = p + 4 * uint32_t(half);
        if (int32_t(ld32(mid)) > key) {
            len = half;
        } else {
            len = len - 1 - half;
            p = mid + 4;
        }
    }
    return p;
}
// push_back(value) on a guest-stack std::vector<int> (vec: begin, end, cap); `tmp` is the stack word passed to
// the slow path
void pushInt(Addr vec, int32_t value, Addr tmp) {
    *rw::frame_ptr<int32_t>(tmp) = value;
    uint32_t end = ld32(vec + 4);
    if (end >= ld32(vec + 8)) {
        call(g::vector_int_pushSlow, {vec, tmp});
    } else {
        uint32_t e = 0;
        if (end) {
            st32(end, uint32_t(value));
            e = ld32(vec + 4);
        }
        *rw::frame_ptr<uint32_t>(vec + 4) = e + 4;
    }
}
}  // namespace

// Sub-mission rewards newly achieved (a mission cleared now and not before) and the stage's first-clear rewards.
uint32_t Self::loadStageResultData(Self *self) {
    int32_t kind = int32_t(ld32(call(g::SceneManager_getInstance, {}) + SM_BATTLE_KIND));
    if ((kind & ~1) == 2) return 0;
    Frame f;
    uint32_t api = call(g::APIManager_getInstance, {});
    Addr word = f.alloc(4);
    call(g::SafeBuffer_read, {api + API_STAGE_STATE, word, ld32(api + API_STAGE_STATE + 12) + 4, 4, 4});
    Addr stageRow = f.alloc(8);
    call(g::Stage_create, {stageRow, ld32(word)});
    // missions cleared before (from the user data: 3 ints, up to the first -1)
    Addr before = f.alloc(12);
    for (uint32_t k = 0; k < 12; k += 4) *rw::frame_ptr<uint32_t>(before + k) = 0;
    uint32_t state = call(g::AppState_getInstance, {});
    uint32_t user = ld32(state + APPSTATE_USER);
    uint32_t userCtrl = ld32(state + APPSTATE_USER + 4);
    if (userCtrl) call(g::shared_add, {userCtrl});
    Addr cleared = f.alloc(12);
    sbRead(user, cleared, 0x4228, 0xC);
    Addr tmp = f.alloc(4);
    for (uint32_t k = 0; k < 3; k++) {
        int32_t v = int32_t(ld32(cleared + 4 * k));
        if (v == -1) break;
        pushInt(before, v, tmp);
    }
    // missions cleared now (sorted): newly achieved = in `now` and not in `before`
    uint32_t start = ld32(call(g::APIManager_getInstance, {}) + API_STAGE_START);
    Addr fresh = f.alloc(12);
    for (uint32_t k = 0; k < 12; k += 4) *rw::frame_ptr<uint32_t>(fresh + k) = 0;
    Addr tmp1 = f.alloc(4), tmp2 = f.alloc(4), tmp3 = f.alloc(4);
    Addr tmps[3] = {tmp1, tmp2, tmp3};
    for (int32_t m = 1; m <= 3; m++) {
        uint32_t nowBegin = ld32(start + 0x4C), nowEnd = ld32(start + 0x50);
        uint32_t p = upperBound(nowBegin, nowEnd, m - 1);
        if (p != nowEnd && int32_t(ld32(p)) < m + 1) continue;   // cleared now ... before as well?
        uint32_t q = upperBound(ld32(before), ld32(before + 4), m - 1);
        if (q == ld32(before + 4) || int32_t(ld32(q)) > m) continue;
        pushInt(fresh, m, tmps[m - 1]);
    }
    // each newly achieved mission's reward (stage arrays of 3: type, id, amount)
    Addr thisWord = f.alloc(4);
    *rw::frame_ptr<uint32_t>(thisWord) = me(self);
    uint32_t stage = ld32(stageRow);
    Addr a = f.alloc(12), b = f.alloc(12), c = f.alloc(12);
    for (uint32_t p = ld32(fresh), end = ld32(fresh + 4); p != end; p += 4) {
        uint32_t m = ld32(p) - 1;
        sbRead(stage, a, 0x3A4, 0xC);
        uint32_t type = ld32(call(g::array3_at, {a, m}));
        sbRead(stage, b, 0x3B0, 0xC);
        uint32_t id = ld32(call(g::array3_at, {b, m}));
        sbRead(stage, c, 0x3E0, 0xC);
        uint32_t amount = ld32(call(g::array3_at, {c, m}));
        addStageReward(thisWord, int32_t(type), id, amount, 1);
    }
    // first clear: the stage's clear rewards
    uint32_t clearInfo = ld32(call(g::APIManager_getInstance, {}) + API_STAGE_CLEAR);
    Addr flag = f.alloc(4);
    call(g::SafeBuffer_read, {clearInfo, flag + 3, ld32(clearInfo + 12), 1, 0});
    if (ld8(flag + 3)) {
        Addr count = f.alloc(4);
        for (uint32_t k = 0;; k++) {
            sbRead(stage, count, 0x190, 4);
            if (int32_t(k) >= int32_t(ld32(count))) break;
            sbRead(stage, a, 0x194, 0xC);
            uint32_t type = ld32(call(g::array3_at, {a, k}));
            sbRead(stage, b, 0x1A0, 0xC);
            uint32_t id = ld32(call(g::array3_at, {b, k}));
            sbRead(stage, c, 0x1D0, 0xC);
            uint32_t amount = ld32(call(g::array3_at, {c, k}));
            addStageReward(thisWord, int32_t(type), id, amount, 0);
        }
    }
    uint32_t n = fillPending(self, thisWord);
    call(g::vector_int_dtor, {fresh});
    if (userCtrl) call(fn::shared_release, {userCtrl});
    call(g::vector_int_dtor, {before});
    if (ld32(stageRow + 4)) call(fn::shared_release, {ld32(stageRow + 4)});
    return n;
}
RW_FUNCTION(0x5142A4, "BattleResultScrollView::loadStageResultData()", BattleResultScrollView::loadStageResultData);

// The colosseum rewards: for each (rank, class) reached, the rank reward of the colosseum entry, then the class
// rewards (up to 4 items).
uint32_t Self::loadColosseumResultData(Self *self) {
    clearResults(self);
    Frame f;
    uint32_t api = call(g::APIManager_getInstance, {});
    Addr stageRow = f.alloc(8);
    call(g::ColosseumStage_create, {stageRow, ld32(ld32(api + API_COLOSSEUM_STAGE))});
    uint32_t ranks = call(g::APIManager_getInstance, {}) + API_COLOSSEUM_RANKS;
    uint32_t rankBegin = ld32(ld32(ranks)), rankEnd = ld32(ld32(ranks) + 4);
    Addr word = f.alloc(4), word2 = f.alloc(4);
    Addr colo = f.alloc(8), v = f.alloc(4);
    if (rankBegin != rankEnd) {
        // rank rewards
        for (uint32_t pr = rankBegin; pr != rankEnd; pr += 8) {
            sbRead(ld32(stageRow), word, 0x1C, 4);
            for (int32_t i = int32_t(ld32(word));; i++) {
                sbRead(ld32(stageRow), word2, 0x20, 4);
                if (i > int32_t(ld32(word2))) break;
                call(g::Colosseum_create, {colo, uint32_t(i)});
                sbRead(ld32(colo), v, 8, 4);
                bool used = false;
                if (ld32(v) == ld32(pr)) {
                    sbRead(ld32(colo), v, 0x5C, 4);
                    if (ld32(v)) {
                        Frame g2;
                        Addr r = newResult(g2);
                        sbRead(ld32(colo), v, 0x60, 4);
                        uint32_t type = ld32(v);
                        st32(r, type);
                        sbRead(ld32(colo), v, 0x64, 4);
                        uint32_t id = ld32(v);
                        st32(r + 0x1C, id);
                        call(g::SceneUtil_setItemImageData, {r + 0x10, r + 4, type, id, 1});
                        st8(r + 0x20, 0);
                        st8(r + 0x21, 0);
                        st32(r + 0x24, 0);
                        st32(r + 0x28, 1);
                        st32(r + 0x2C, 1);
                        sbRead(ld32(colo), v, 0x74, 4);
                        st32(r + 0x30, ld32(v));
                        st8(r + 0x34, 1);
                        Addr text = g2.alloc(8);
                        call(g::TextData_get, {text, 0, TEXT_COLO_RANK});
                        uint32_t rank = ld32(pr), cls = ld32(pr + 4);
                        uint32_t fmt = ld32(ld32(text) + 4);
                        Addr colosseumNo = g2.alloc(4);
                        sbRead(ld32(colo), colosseumNo, 0x10, 4);
                        Addr buf = stringBuffer(g2, {fmt, rank, cls, ld32(colosseumNo)});
                        call(g::string_assign, {r + 0x38, buf, call(g::strlen_, {buf})});
                        sharedRelease(text);
                        pushResult(self, r);
                        destroyResult(r);
                        used = true;
                    }
                }
                sharedRelease(colo);
                if (used) break;
            }
        }
        // class rewards
        uint32_t list = ld32(ranks);
        uint32_t classBegin = ld32(list), classEnd = ld32(list + 4);
        for (uint32_t pr = classBegin; pr != classEnd; pr += 8) {
            sbRead(ld32(stageRow), word, 0x1C, 4);
            for (int32_t i = int32_t(ld32(word));; i++) {
                sbRead(ld32(stageRow), word2, 0x20, 4);
                if (i > int32_t(ld32(word2))) break;
                call(g::Colosseum_create, {colo, uint32_t(i)});
                sbRead(ld32(colo), v, 8, 4);
                if (ld32(v) == ld32(pr)) {
                    sbRead(ld32(colo), v, 0x10, 4);
                    if (ld32(v) == ld32(pr + 4)) {
                        for (int32_t k = 0;; k++) {
                            sbRead(ld32(colo), v, 0x78, 4);
                            if (k >= int32_t(ld32(v))) break;
                            Frame g2;
                            Addr r = newResult(g2);
                            Addr arr = g2.alloc(16);
                            sbRead(ld32(colo), arr, 0x7C, 0x10);
                            uint32_t type = ld32(call(g::array4_at, {arr, uint32_t(k)}));
                            st32(r, type);
                            sbRead(ld32(colo), arr, 0x8C, 0x10);
                            uint32_t id = ld32(call(g::array4_at, {arr, uint32_t(k)}));
                            st32(r + 0x1C, id);
                            call(g::SceneUtil_setItemImageData, {r + 0x10, r + 4, type, id, 1});
                            st8(r + 0x20, 0);
                            st8(r + 0x21, 0);
                            st32(r + 0x24, 0);
                            st32(r + 0x28, 1);
                            st32(r + 0x2C, 1);
                            sbRead(ld32(colo), arr, 0xCC, 0x10);
                            st32(r + 0x30, ld32(call(g::array4_at, {arr, uint32_t(k)})));
                            st8(r + 0x34, 1);
                            Addr text = g2.alloc(8);
                            call(g::TextData_get, {text, 0, TEXT_COLO_REWARD});
                            uint32_t fmt = ld32(ld32(text) + 4);
                            Addr colosseumNo = g2.alloc(4);
                            sbRead(ld32(colo), colosseumNo, 8, 4);
                            Addr buf = stringBuffer(g2, {fmt, ld32(colosseumNo)});
                            call(g::string_assign, {r + 0x38, buf, call(g::strlen_, {buf})});
                            sharedRelease(text);
                            pushResult(self, r);
                            destroyResult(r);
                        }
                    }
                }
                sharedRelease(colo);
            }
        }
    }
    Addr counter = f.alloc(4);
    uint32_t n = fillPending(self, counter);
    sharedRelease(stageRow);
    return n;
}
RW_FUNCTION(0x514A14, "BattleResultScrollView::loadColosseumResultData()", BattleResultScrollView::loadColosseumResultData);
