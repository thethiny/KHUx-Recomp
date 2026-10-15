// SceneUtil: user-state queries (AP, medal box, Facebook link, campaigns), gauges and image paths.
#include "SceneUi.hpp"

using namespace scene;

namespace {
// APIManager fields
constexpr uint32_t APIManager_user = 0x44;                // User* (link flags byte at +0x35)
constexpr uint32_t APIManager_userStatus = 0x8C;          // SafeBuffer*: max AP at 0x3C
constexpr uint32_t APIManager_userData = 0x94;            // shared_ptr<UserData>: medal box size at 0x34
constexpr uint32_t APIManager_stageResumption = 0x9C;     // SafeBuffer* (status at 0) or null
constexpr uint32_t APIManager_campaigns = 0xAC;           // std::vector<Campaign>* (12 bytes: shop id, Date)
constexpr uint32_t APIManager_userMedals = 0xD4;          // shared_ptr<UserMedal>: medal count at +0x20
constexpr uint32_t APIManager_userLinks = 0x334;          // unordered_map<PlatformType, Info>*

constexpr uint32_t AppState_getCurrentAP = 0x4EA5E9;      // (state, long* nextIn): current AP
constexpr uint32_t AppState_setNewUserMedalIDs = 0x4EB1C5;
constexpr uint32_t UserLinks_find = 0x47ABFB;             // unordered_map<PlatformType, Info>::find
constexpr uint32_t FacebookManager_getInstance = 0x4E5585;
constexpr uint32_t FacebookManager_isLoggedIn = 0x4E56C1;
constexpr uint32_t Title_create = 0x78FA8D;               // master::Title::create(out, id)
constexpr uint32_t Player_create = 0x7871A9;              // master::Player::create(out, rank)
constexpr uint32_t Shop_getShopIds = 0x790935;            // std::set<int> (sret)
constexpr uint32_t Shop_create = 0x790951;
constexpr uint32_t EvCampaign_create = 0x781DD9;
constexpr uint32_t TextData_format = 0x492929;            // (sret string, type, id, ...)
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t IntTree_destroy = 0x48D9B5;
constexpr uint32_t StringVector_pushBackSlow = 0x484465;  // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t DateVector_pushBackSlow = 0x482A19;    // vector<Date>::__push_back_slow_path(const Date&)
constexpr uint32_t DateVector_dtor = 0x473611;
constexpr uint32_t StringVector_dtor = 0x47363F;
constexpr uint32_t GaugeFunction_call = 0x46F009;         // std::function<void(float, unsigned long long)>::operator()
constexpr uint32_t CampaignFunction_assign = 0x474425;    // std::function<bool(int, shared_ptr<EvCampaign>&)>::operator=(&&)
constexpr uint32_t CampaignFunction_call = 0x474475;
constexpr uint32_t CampaignAnimationView_targetFunc[4] = {0x598A65, 0x598A81, 0x598A9D, 0x598AB9};   // story/event/colosseum/raid

constexpr uint32_t STR_Empty = 0xDF8A1A;
constexpr uint32_t STR_UnionL_on = 0xDF942D;              // "img/union/Union_L_%02d_on.png"
constexpr uint32_t STR_UnionL = 0xDF944B;                 // "img/union/Union_L_%02d.png"
constexpr uint32_t STR_Union = 0xDF9466;                  // "img/union/Union_%02d.png"
constexpr uint32_t STR_UserTitlePng = 0xDF9A2F;           // "img/userTitle/title_%04d.png"
constexpr uint32_t STR_PartyRankPng = 0xDF947F;           // "img/partyRank/Party_RankIcon%d.png"
constexpr uint32_t STR_RankTitlePng = 0xDF9A11;           // "img/ranking/Rank_Title_%d.png"
constexpr uint32_t STR_RankTitle_Board = 0xDF940E, STR_RankTitle_Name = 0xDF941E;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }
uint32_t currentAp() { return call(AppState_getCurrentAP, {call(fn::AppState_getInstance, {}), 0}); }

// a std::string result (sret) = printf(fmt, value) through a StringBuffer
void formatPath(uint32_t out, uint32_t fmt, uint32_t value) {
    Frame f;
    uint32_t buf = stringBuffer(f, {fmt, value});
    strInitC(out, buf);
}

// libc++ std::set<int> in-order successor of `node`
uint32_t treeNext(uint32_t node) {
    uint32_t right = ld32(node + 4);
    if (right) {
        node = right;
        while (ld32(node)) node = ld32(node);
        return node;
    }
    uint32_t child;
    do {
        child = node;
        node = ld32(child + 8);
    } while (ld32(node) != child);
    return node;
}

// clamp to [0, 1] like the original (NaN passes through)
float clamp01(float v) {
    float a = v < 0.0f ? 0.0f : v;
    return a > 1.0f ? 1.0f : a;
}

float u64ToFloat(uint64_t v) { return float(v); }
}  // namespace

// ------------------------------------------------------------------------------------------------ queries

// The user has linked a Facebook account and is logged in to it.
bool SceneUtil::isLoggedInFacebook() {
    uint32_t user = ld32(api() + APIManager_user);
    if (!user || !(ld8(user + 0x35) & 2)) return false;
    uint32_t links = ld32(api() + APIManager_userLinks);
    if (!links) return false;
    Frame f;
    uint32_t key = f.alloc(4);
    st8(key + 3, 1);                                   // PlatformType::Facebook
    if (!call(UserLinks_find, {links, key + 3})) return false;
    return call(FacebookManager_isLoggedIn, {call(FacebookManager_getInstance, {})}) != 0;
}
RW_FUNCTION(0x4697E8, "SceneUtil::isLoggedInFacebook", SceneUtil::isLoggedInFacebook);

uint32_t SceneUtil::getLinkPlatformType() { return isLoggedInFacebook(); }
RW_FUNCTION(0x47AAAC, "SceneUtil::getLinkPlatformType", SceneUtil::getLinkPlatformType);

bool SceneUtil::isMaxAp() {
    uint32_t maxAp = uint32_t(safe32(ld32(api() + APIManager_userStatus), 0x3C));
    return maxAp <= currentAp();
}
RW_FUNCTION(0x469F84, "SceneUtil::isMaxAp", SceneUtil::isMaxAp);

bool SceneUtil::isMaxApOk(int32_t ap) { return safe32(ld32(api() + APIManager_userStatus), 0x3C) >= ap; }
RW_FUNCTION(0x469FF8, "SceneUtil::isMaxApOk", SceneUtil::isMaxApOk);

bool SceneUtil::isApOk(uint32_t ap) { return currentAp() >= ap; }
RW_FUNCTION(0x46A060, "SceneUtil::isApOk", SceneUtil::isApOk);

// would `adding` more medals overflow the medal box?
bool SceneUtil::isOverMedalSlot(int32_t adding) {
    uint32_t a = api();
    uint32_t dataCtl = ld32(a + APIManager_userData + 4);
    uint32_t data = ld32(a + APIManager_userData);
    if (dataCtl) call(fn::shared_add, {dataCtl});
    a = api();
    uint32_t medalsCtl = ld32(a + APIManager_userMedals + 4);
    uint32_t medals = ld32(a + APIManager_userMedals);
    if (medalsCtl) call(fn::shared_add, {medalsCtl});
    uint32_t box = uint32_t(safe32(data, 0x34));
    uint32_t count = ld32(medals + 0x20) + uint32_t(adding);
    if (medalsCtl) call(fn::shared_release, {medalsCtl});
    if (dataCtl) call(fn::shared_release, {dataCtl});
    return box < count;
}
RW_FUNCTION(0x46A084, "SceneUtil::isOverMedalSlot", SceneUtil::isOverMedalSlot);

// would `adding` more medals overflow the largest possible medal box?
bool SceneUtil::isOverMaxMedalSlot(int32_t adding) {
    uint32_t a = api();
    uint32_t medalsCtl = ld32(a + APIManager_userMedals + 4);
    uint32_t medals = ld32(a + APIManager_userMedals);
    if (medalsCtl) call(fn::shared_add, {medalsCtl});
    Frame f;
    uint32_t misc = masterRow(f, fn::Misc_create, 104);
    uint32_t max = uint32_t(safe32(ld32(misc), 4));
    uint32_t count = ld32(medals + 0x20) + uint32_t(adding);
    sharedRelease(misc);
    if (medalsCtl) call(fn::shared_release, {medalsCtl});
    return max < count;
}
RW_FUNCTION(0x46A164, "SceneUtil::isOverMaxMedalSlot", SceneUtil::isOverMaxMedalSlot);

int32_t SceneUtil::getStageResumptionStatus() {
    uint32_t r = ld32(api() + APIManager_stageResumption);
    return r ? safe32(r, 0) : 0;
}
RW_FUNCTION(0x46F100, "SceneUtil::getStageResumptionStatus", SceneUtil::getStageResumptionStatus);

void SceneUtil::clearMedalNewMark() { call(AppState_setNewUserMedalIDs, {call(fn::AppState_getInstance, {}), 0}); }
RW_FUNCTION(0x46C4B4, "SceneUtil::clearMedalNewMark", SceneUtil::clearMedalNewMark);

// Any shop campaign still running (its date in the future) with a matching shop? (After the tutorial only.)
int32_t SceneUtil::isBenefits() {
    if (getNexTutorialphase() < 995) return 1;
    uint32_t campaigns = ld32(api() + APIManager_campaigns);
    int32_t found = 0;
    Frame f;
    uint32_t dates = f.alloc(12), names = f.alloc(12);       // std::vector<Date>, std::vector<std::string>
    for (uint32_t k = 0; k < 12; k += 4) { st32(dates + k, 0); st32(names + k, 0); }
    if (ld32(campaigns + 4) != ld32(campaigns) && ld32(campaigns + 4) != ld32(campaigns)) {
        uint32_t date = f.alloc(8);
        uint32_t str = f.alloc(12);
        uint32_t ids = f.alloc(12);
        uint32_t k = 0;
        do {
            uint32_t entry = ld32(campaigns) + 12 * k;
            uint32_t lo = rw::load<uint16_t>(entry + 4) | uint32_t(rw::load<uint16_t>(entry + 6)) << 16;
            uint32_t hi = rw::load<uint16_t>(entry + 8) | uint32_t(rw::load<uint16_t>(entry + 10)) << 16;
            st32(date, lo);
            st32(date + 4, hi);
            if (int32_t(call(Date_calculateIntervalSinceNow, {date})) >= 0) {
                call(Shop_getShopIds, {ids});
                for (uint32_t node = ld32(ids); node != ids + 4;) {
                    Frame g;
                    uint32_t row = masterRow(g, Shop_create, ld32(node + 16));
                    bool match = safe32(ld32(row), 0x12C) == int32_t(ld32(entry));
                    if (match) {
                        uint32_t raw = safeBytes(g, ld32(row), 0x23A, 0x81);
                        strInitC(str, raw);
                        uint32_t end = ld32(names + 4);
                        if (end < ld32(names + 8)) {
                            if (end) {
                                for (uint32_t w = 0; w < 12; w += 4) st32(end + w, ld32(str + w));
                                for (uint32_t w = 0; w < 12; w += 4) st32(str + w, 0);
                                end = ld32(names + 4);
                            }
                            st32(names + 4, end + 12);
                        } else {
                            call(StringVector_pushBackSlow, {names, str});
                        }
                        strFree(str);
                        uint32_t dend = ld32(dates + 4);
                        if (dend != ld32(dates + 8)) {
                            if (dend) {
                                rw::save<uint16_t>(dend + 4, uint16_t(hi));
                                rw::save<uint16_t>(dend, uint16_t(lo));
                                rw::save<uint16_t>(dend + 6, uint16_t(hi >> 16));
                                rw::save<uint16_t>(dend + 2, uint16_t(lo >> 16));
                                dend = ld32(dates + 4);
                            }
                            st32(dates + 4, dend + 8);
                        } else {
                            call(DateVector_pushBackSlow, {dates, date});
                        }
                        found++;
                    }
                    sharedRelease(row);
                    if (match) break;
                    node = treeNext(node);
                }
                call(IntTree_destroy, {ids, ld32(ids + 4)});
            }
            k++;
        } while (k < ((ld32(campaigns + 4) - ld32(campaigns)) >> 2) * 0xAAAAAAABu);
    }
    call(DateVector_dtor, {dates});
    call(StringVector_dtor, {names});
    return found != 0;
}
RW_FUNCTION(0x4733C4, "SceneUtil::isBenefits", SceneUtil::isBenefits);

// The campaign bonus on `base` points: the first campaign of `ids` matching the kind's filter and `target`
// adds its rate (1/10000ths); rounded up when `roundUp`, at least 1.
int32_t SceneUtil::getCampaignPoint(int32_t base, int32_t kind, int32_t id, Addr ids, int32_t target,
                                    uint32_t roundUp) {
    if (!base) return 0;
    Frame f;
    uint32_t filter = f.alloc(20);
    st32(filter + 16, 0);
    if (uint32_t(kind - 1) > 3) return base;
    {
        Frame g;
        uint32_t tmp = g.alloc(20);
        call(CampaignAnimationView_targetFunc[kind - 1], {tmp});
        call(CampaignFunction_assign, {filter, tmp});
        functionDestroy(tmp);
    }
    if (!ld32(filter + 16)) return base;
    int32_t result = base;
    uint32_t it = ld32(ids), end = ld32(ids + 4);
    if (it != end) {
        uint32_t last = end - 4;
        bool found;
        do {
            Frame g;
            uint32_t row = masterRow(g, EvCampaign_create, ld32(it));
            found = false;
            if (ld32(row) && call(CampaignFunction_call, {filter, uint32_t(id), row})) {
                if (safe32(ld32(row), 4) == target) {
                    int32_t rate = safe32(ld32(row), 8);
                    int64_t product = int64_t(rate) * int64_t(base);
                    int32_t q = int32_t(product / 10000);
                    bool exact = product % 10000 == 0;
                    uint32_t add = 1u & ~((exact ? 1u : 0u) | (roundUp ^ 1u));
                    int32_t v = int32_t(add + uint32_t(q));
                    result = v >= 1 ? v : 1;
                    found = true;
                }
            }
            sharedRelease(row);
            bool atLast = last == it;
            it += 4;
            found = found || atLast;
        } while (!found);
    }
    functionDestroy(filter);
    return result;
}
RW_FUNCTION(0x4741BC, "SceneUtil::getCampaignPoint", SceneUtil::getCampaignPoint);

// ------------------------------------------------------------------------------------------------ gauges

// Lux gauge of rank `rank` with `lux` points: callback(fill 0..1, Lux still missing for the next rank).
void SceneUtil::calcLuxGaugePer(int32_t rank, uint64_t lux, Addr callback) {
    Frame f;
    uint32_t next = masterRow(f, Player_create, uint32_t(rank + 1));
    uint32_t misc = masterRow(f, fn::Misc_create, 100);
    float ratio;
    uint64_t remain;
    if (rank <= 1) {
        int32_t need = safe32(ld32(next), 4);
        int32_t need2 = safe32(ld32(next), 4);
        remain = uint64_t(int64_t(need2)) - lux;
        ratio = u64ToFloat(lux) / float(need);
    } else {
        int32_t maxRank = safe32(ld32(misc), 4);
        ratio = 1.0f;
        remain = 0;
        if (maxRank > rank) {
            uint32_t cur = masterRow(f, Player_create, uint32_t(rank));
            int32_t base = safe32(ld32(cur), 4);
            int32_t needTotal = safe32(ld32(next), 4);
            uint64_t have = lux - uint64_t(int64_t(base));
            int32_t span = int32_t(uint32_t(needTotal) - uint32_t(base));
            ratio = u64ToFloat(have) / float(span);
            remain = uint64_t(int64_t(span)) - have;
            sharedRelease(cur);
        }
    }
    call(GaugeFunction_call, {callback, fbits(clamp01(ratio)), uint32_t(remain), uint32_t(remain >> 32)});
    sharedRelease(misc);
    sharedRelease(next);
}
RW_FUNCTION(0x46EE0C, "SceneUtil::calcLuxGaugePer", SceneUtil::calcLuxGaugePer);

// Skill experience gauge: callback(fill 0..1, experience still missing for the next level).
void SceneUtil::calcSkillExpGaugePer(Addr skill, uint64_t exp, Addr callback) {
    uint32_t data = ld32(skill);                       // UserSkillData: a SafeBuffer, master row at +0x20
    int32_t level = safe32(data, 8);
    int32_t maxLevel = safe32(ld32(data + 0x20), 0xD4);
    float ratio = 1.0f;
    uint64_t remain = 0;
    if (level != maxLevel) {
        int32_t base = safe32(ld32(skill), 0x18);
        int32_t next = safe32(ld32(skill), 0x14);
        uint64_t have = exp - uint64_t(int64_t(base));
        int32_t span = int32_t(uint32_t(next) - uint32_t(base));
        ratio = u64ToFloat(have) / float(span);
        remain = uint64_t(int64_t(span)) - have;
    }
    call(GaugeFunction_call, {callback, fbits(clamp01(ratio)), uint32_t(remain), uint32_t(remain >> 32)});
}
RW_FUNCTION(0x4738D4, "SceneUtil::calcSkillExpGaugePer", SceneUtil::calcSkillExpGaugePer);

// ------------------------------------------------------------------------------------------------ image paths

void SceneUtil::createUnionImagePath(Addr out, int32_t unionId, uint32_t large, uint32_t selected) {
    uint32_t fmt = large == 1 ? (selected == 1 ? STR_UnionL_on : STR_UnionL) : STR_Union;
    formatPath(out, fmt, uint32_t(unionId));
}
RW_FUNCTION(0x46968C, "SceneUtil::createUnionImagePath", SceneUtil::createUnionImagePath);

void SceneUtil::createUserTitleImagePath(Addr out, int32_t id) { formatPath(out, STR_UserTitlePng, uint32_t(id)); }
RW_FUNCTION(0x46ED98, "SceneUtil::createUserTitleImagePath", SceneUtil::createUserTitleImagePath);

void SceneUtil::createPartyRankImagePath(Addr out, int32_t rank) { formatPath(out, STR_PartyRankPng, uint32_t(rank)); }
RW_FUNCTION(0x46F08C, "SceneUtil::createPartyRankImagePath", SceneUtil::createPartyRankImagePath);

void SceneUtil::createPartyTitleRankImagePath(Addr out, int32_t rank) {
    formatPath(out, STR_RankTitlePng, uint32_t(rank));
}
RW_FUNCTION(0x473860, "SceneUtil::createPartyTitleRankImagePath", SceneUtil::createPartyTitleRankImagePath);

// ------------------------------------------------------------------------------------------------ user titles

// The title plate: the two title words (joined by the separator text) and the plate image of `plateId`.
void SceneUtil::updateUserTitlePanel(Addr panel, int32_t firstWord, int32_t secondWord, int32_t plateId) {
    Frame f;
    uint32_t name = f.alloc(12);
    strInit(name, STR_Empty, 0);
    if (firstWord) {
        Frame g;
        uint32_t row = masterRow(g, Title_create, uint32_t(firstWord));
        if (ld32(row)) {
            uint32_t raw = safeBytes(g, ld32(row), 8, 0x41);
            call(fn::string_append, {name, raw, host_strlen(raw)});
        }
        sharedRelease(row);
    }
    if (secondWord) {
        Frame g;
        uint32_t row = masterRow(g, Title_create, uint32_t(secondWord));
        if (ld32(row)) {
            if (strSize(name)) {
                uint32_t sep = g.alloc(12);
                call(TextData_format, {sep, 0, 102200013});
                call(fn::string_append, {name, strData(sep), strSize(sep)});
                strFree(sep);
            }
            uint32_t raw = safeBytes(g, ld32(row), 8, 0x41);
            call(fn::string_append, {name, raw, host_strlen(raw)});
        }
        sharedRelease(row);
    }
    uint32_t board = findNode(panel, STR_RankTitle_Board, 15);
    if (!panel) board = 0;
    uint32_t plate = masterRow(f, Title_create, uint32_t(plateId));
    int32_t image = ld32(plate) ? safe32(ld32(plate), 0x154) : 0;
    if (!image) {
        vcall(board, node::setVisible, {board, 0});
    } else {
        vcall(board, node::setVisible, {board, 1});
        Frame g;
        uint32_t path = g.alloc(12);
        createUserTitleImagePath(path, image);
        uint32_t s = newStr(g, strData(path));
        call(fn::ImageView_loadTexture, {board, s, 0});
        strFree(s);
        strFree(path);
    }
    uint32_t label = findNode(board, STR_RankTitle_Name, 14);
    setUserTitleOutline(label);
    setTextC(label, strData(name));
    sharedRelease(plate);
    strFree(name);
}
RW_FUNCTION(0x46E9E4, "SceneUtil::updateUserTitlePanel(Node*, int, int, int)", SceneUtil::updateUserTitlePanel);

// the same from a UserDetail (title words at 0x10 / 0x14, plate at 0x18)
void SceneUtil::updateUserTitlePanelDetail(Addr panel, Addr detail) {
    uint32_t data = ld32(detail);
    int32_t first = safe32(data, 0x10);
    int32_t second = safe32(ld32(detail), 0x14);
    int32_t plate = safe32(ld32(detail), 0x18);
    updateUserTitlePanel(panel, first, second, plate);
}
RW_FUNCTION(0x46974C, "SceneUtil::updateUserTitlePanel(Node*, shared_ptr<UserDetail>)",
            SceneUtil::updateUserTitlePanelDetail);

// the same from a support user (StageSupportList::SupportUser: titles at +0x30, +0x34, +0x38)
void SceneUtil::updateUserTitlePanelSupport(Addr panel, Addr user) {
    updateUserTitlePanel(panel, int32_t(ld32(user + 0x30)), int32_t(ld32(user + 0x34)), int32_t(ld32(user + 0x38)));
}
RW_FUNCTION(0x46ED84, "SceneUtil::updateUserTitlePanel(Node*, SupportUser)", SceneUtil::updateUserTitlePanelSupport);

// settingTitleLabel with the text given as a std::string
SceneUtil::Addr SceneUtil::settingTitleLabelText(Addr root, Addr name, Addr text) {
    Frame f;
    uint32_t key = newStr(f, strData(name));
    uint32_t label = searchNodeByName(root, key);
    strFree(key);
    setShadow(label);
    setTextC(label, strData(text));
    return label;
}
RW_FUNCTION(0x46E8D8, "SceneUtil::settingTitleLabel(Node*, string, string)", SceneUtil::settingTitleLabelText);
