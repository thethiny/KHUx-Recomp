// SceneRanking::openRankingRewardPopup: the result of a finished ranking (Ranking_Result.json). The API's ranking
// results (APIManager +0x2F4: {rankingId, score, rank} entries) are shown one at a time; `index` picks the entry.
// The popup shows the period, the score, the rank title of the ranking type and the reward of the matching
// master::RankingReward row (score range, or for the party / colosseum types by rank), else a notice. OK closes it
// (lambda $_17, not rewritten here) and continues with `onClose`.
#include "SceneRanking.hpp"

#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;
using Self = SceneRanking;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t SM_getPopupLayer = 0x613E69;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;               // (float duration, uint8 opacity)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;                   // Vec2(float, float)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;       // (image, const string&, TextureResType)
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t TextData_format = 0x492929;             // (string out, Type, id, ...)
constexpr uint32_t Date_makeLocalDate = 0x4927CD;          // (Date out, const Date*)
constexpr uint32_t Ranking_create = 0x7912CD;              // master::Ranking::create(shared_ptr out, int id)
constexpr uint32_t RankingReward_create = 0x78ADB9;        // master::RankingReward::create(shared_ptr out, int id)
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;       // (string out, unsigned long long)
constexpr uint32_t SceneUtil_numberToPlaceString = 0x473689;
constexpr uint32_t SceneUtil_createPartyTitleRankImagePath = 0x473861;   // (string out, int rank)
constexpr uint32_t ArrayInt5_at = 0x700B5D;                // std::array<int, 5>::at(size_t) (throws)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;             // vector::at failure (never returns)
constexpr uint32_t ResultVector_copy = 0x700BD1;           // vector<RankingReward::Result>(const vector&)
constexpr uint32_t ResultVector_dtor = 0x4AE7F7;           // __vector_base<RankingReward::Result>::~__vector_base
constexpr uint32_t RankingRewardScrollView_createIncentivePanel = 0x55E295;   // (Widget*, int)
constexpr uint32_t RankingRewardScrollView_initLayout = 0x560A09;             // (Widget*, int rewardId, bool)
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;           // &cocos2d::Vec2::ANCHOR_MIDDLE
}  // namespace g

// __func vtables (+8)
constexpr uint32_t VT_Close = 0xF6A148;            // $_17 {root, layer, shadow, vector<Result>, index, onClose} (heap 0x40)
constexpr uint32_t VT_Opened = 0xF6A11C;           // the open animation's lambda {root}

// strings
constexpr uint32_t S_ResultJson = 0xE066AB;        // "cocostudio/publish/Ranking_Result.json"
constexpr uint32_t S_PanelResults1 = 0xE066D2, S_PanelResults2 = 0xE066E1, S_Union = 0xE065F5;
constexpr uint32_t S_RankTitle = 0xE066F0, S_TxtUnionRanking = 0xE066FB, S_TxtMonthResults = 0xE0670C;
constexpr uint32_t S_TxtWeeklyResults = 0xE0671D, S_TxtResults = 0xE0672F, S_TxtRank = 0xDFE900;
constexpr uint32_t S_TxtPeriod = 0xE0661F, S_TxtNumLabel = 0xDFD8DC, S_Plate = 0xDFFE65, S_Panel = 0xDFD420;
constexpr uint32_t S_PanelNotice = 0xE0673B, S_TxtNotice = 0xE06748, S_Area = 0xDF9378, S_ButtonOk = 0xDF8688;
constexpr uint32_t S_FmtSS = 0xDF8BBA;             // "%s%s"

// text ids
constexpr int32_t TXT_RANK = 103900026;
constexpr int32_t TXT_WEEKLY_RESULTS = 103900022, TXT_MONTH_RESULTS = 103900021, TXT_MONTH_RESULTS_5 = 103900061;
constexpr int32_t TXT_UNION_RANKING = 103900048;
constexpr int32_t TXT_RESULTS_3 = 103900023, TXT_RANK_3 = 103900035, TXT_RESULTS_4 = 103900024;
constexpr int32_t TXT_PERIOD = 103900011, TXT_PERIOD_DATES = 103901038;
constexpr int32_t TXT_NO_REWARD = 103900047;

constexpr uint32_t API_RANKING_RESULTS = 0x2F4;    // -> {vector<Result> (12 bytes: rankingId, score, rank), ...}
constexpr uint32_t API_USER = 0x94;                // SafeBuffer: union id at 0x30
constexpr uint32_t RESULT_SIZE = 12;
constexpr uint32_t SM_FLAG_308 = 0x308;

inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
// a text from the text table, then its shadow (released after the shadow, as the code does)
void setTextIdShadow(uint32_t text, int32_t id) {
    Frame f;
    uint32_t td = textData(f, id);
    setTextC(text, textDataStr(td));
    SceneUtil::setShadow(text);
    sharedRelease(td);
}
int32_t resultCount(uint32_t results) {
    return int32_t(uint32_t(int32_t(ld32(results + 4) - ld32(results)) >> 2) * 0xAAAAAAABu);
}
// entries[index] (std::vector::at)
uint32_t resultAt(uint32_t results, int32_t index) {
    if (!(uint32_t(resultCount(results)) > uint32_t(index))) call(g::ThrowOutOfRange, {results});
    return ld32(results) + RESULT_SIZE * uint32_t(index);
}
// a 4-byte field of a SafeBuffer row
int32_t readInt(Frame &f, uint32_t row, uint32_t index) {
    uint32_t out = f.alloc(4);
    call(g::SafeBuffer_read, {row, out, ld32(row + 0xC) + index, 4, index});
    return int32_t(ld32(out));
}
// the union emblem of the player into `image`
void loadUnionImage(Frame &f, uint32_t userRef, uint32_t image) {
    uint32_t user = ld32(userRef);
    int32_t unionId = readInt(f, user, 0x30);
    uint32_t path = f.alloc(12);
    SceneUtil::createUnionImagePath(path, unionId, 0, 0);
    uint32_t s = newStr(f, strData(path));
    call(g::ImageView_loadTexture, {image, s, 0});
    strFree(s);
    strFree(path);
}
}  // namespace

void openRankingRewardPopup(int32_t index, uint32_t onClose) {
    Frame f;
    uint32_t results = ld32(call(fn::APIManager_getInstance, {}) + API_RANKING_RESULTS);
    if (!(uint32_t(resultCount(results)) > uint32_t(index))) {
        if (ld32(onClose + 16)) call(fn::function_vv_call, {onClose});
        return;
    }
    call(g::AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    uint32_t shadow = SceneUtil::createShadowSprite();
    uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_ResultJson});
    vcall(root, node::setAnchorPoint, {root, ld32(g::GOT_ANCHOR_MIDDLE)});
    {
        float x0 = bitsf(vcall(root, node::getPositionX, {root}));
        float w = ldf(vcall(root, node::getContentSize, {root}));
        float side = SceneUtil::getSideMargin();
        float y0 = bitsf(vcall(root, node::getPositionY, {root}));
        float h = ldf(vcall(root, node::getContentSize, {root}) + 4);
        float x = side + (x0 + w * 0.5f);
        float y = y0 + h * 0.5f;
        y = SceneUtil::getSideHeightMargin() + y;
        uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(x), fbits(y)});
        vcall(root, node::setPosition, {root, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(layer, node::addChild, {layer, root});
    uint32_t ranking = masterRow(f, g::Ranking_create, ld32(resultAt(results, index)));
    int32_t score = int32_t(ld32(resultAt(results, index) + 4));
    uint32_t userRef = call(fn::APIManager_getInstance, {}) + API_USER;

    uint32_t results1 = findNode(root, S_PanelResults1, 14);
    uint32_t results2 = findNode(root, S_PanelResults2, 14);
    uint32_t unionImage = findNode(results1, S_Union, 5);
    uint32_t rankTitle = findNode(results1, S_RankTitle, 10);
    uint32_t unionRanking = findNode(results1, S_TxtUnionRanking, 16);
    uint32_t monthResults = findNode(results1, S_TxtMonthResults, 16);
    uint32_t weeklyResults = findNode(results1, S_TxtWeeklyResults, 17);
    uint32_t txtResults = findNode(results2, S_TxtResults, 11);
    setVisible(results1, 0);
    setVisible(results2, 0);
    uint32_t txtRank = findNode(root, S_TxtRank, 8);
    uint32_t row = ld32(ranking);
    int32_t type = readInt(f, row, 4);
    switch (type) {
    case 1:     // weekly union ranking
        setVisible(results1, 1);
        loadUnionImage(f, userRef, unionImage);
        setTextIdShadow(weeklyResults, TXT_WEEKLY_RESULTS);
        setVisible(rankTitle, 0);
        setVisible(unionRanking, 0);
        setVisible(monthResults, 0);
        setTextIdShadow(txtRank, TXT_RANK);
        break;
    case 2:     // monthly union ranking, with the rank title
    case 5: {
        setVisible(results1, 1);
        loadUnionImage(f, userRef, unionImage);
        uint32_t path = f.alloc(12);
        call(g::SceneUtil_createPartyTitleRankImagePath, {path, ld32(resultAt(results, index) + 8)});
        {
            uint32_t s = newStr(f, strData(path));
            call(g::ImageView_loadTexture, {rankTitle, s, 0});
            strFree(s);
        }
        strFree(path);
        setTextIdShadow(unionRanking, TXT_UNION_RANKING);
        setTextIdShadow(monthResults, type == 2 ? TXT_MONTH_RESULTS : TXT_MONTH_RESULTS_5);
        setTextIdShadow(txtRank, TXT_RANK);
        setVisible(weeklyResults, 0);
        break;
    }
    case 3:
        setVisible(results2, 1);
        setTextIdShadow(txtResults, TXT_RESULTS_3);
        setTextIdShadow(txtRank, TXT_RANK_3);
        break;
    case 4:
        setVisible(results2, 1);
        setTextIdShadow(txtResults, TXT_RESULTS_4);
        setTextIdShadow(txtRank, TXT_RANK);
        break;
    default:
        setTextIdShadow(txtRank, TXT_RANK);
        break;
    }

    // the ranking period
    {
        uint32_t txtPeriod = findNode(root, S_TxtPeriod, 10);
        uint32_t start = f.alloc(8);
        call(g::SafeBuffer_read, {row, start, ld32(row + 0xC) + 8, 8, 8});
        uint32_t from = f.alloc(8);
        call(g::Date_makeLocalDate, {from, start});
        uint32_t end = f.alloc(8);
        call(g::SafeBuffer_read, {row, end, ld32(row + 0xC) + 16, 8, 16});
        uint32_t to = f.alloc(8);
        call(g::Date_makeLocalDate, {to, end});
        Frame t;
        uint32_t label = textData(t, TXT_PERIOD);
        uint32_t dates = t.alloc(12);
        call(g::TextData_format, {dates, 0, uint32_t(TXT_PERIOD_DATES), rw::load<uint16_t>(from), ld8(from + 2),
                                  ld8(from + 3), rw::load<uint16_t>(to), ld8(to + 2), ld8(to + 3)});
        uint32_t buf = stringBuffer(t, {S_FmtSS, textDataStr(label), strData(dates)});
        setTextC(txtPeriod, buf);
        SceneUtil::setShadow(txtPeriod);
        strFree(dates);
        sharedRelease(label);
        setVisible(txtPeriod, 1);
    }
    // the score
    {
        uint32_t num = findNode(root, S_TxtNumLabel, 13);
        Frame t;
        uint32_t money = t.alloc(12);
        call(g::SceneUtil_getMoneyStr, {money, 0, uint32_t(score), uint32_t(score >> 31)});
        uint32_t place = t.alloc(12);
        call(g::SceneUtil_numberToPlaceString, {place, uint32_t(score)});
        uint32_t buf = stringBuffer(t, {S_FmtSS, strData(money), strData(place)});
        setTextC(num, buf);
        SceneUtil::setShadow(num);
        strFree(place);
        strFree(money);
    }
    uint32_t plate = findNode(root, S_Plate, 5);
    setVisible(plate, 1);
    uint32_t panel = findNode(root, S_Panel, 5);
    uint32_t notice = findNode(root, S_PanelNotice, 12);

    // the reward: master::RankingReward rows first..last; the one whose score range holds the score
    int32_t first, last;
    if (readInt(f, row, 4) == 2 || readInt(f, row, 4) == 5) {
        int32_t at = int32_t(ld32(resultAt(results, index) + 8)) - 1;
        uint32_t firsts = f.alloc(0x14);
        call(g::SafeBuffer_read, {row, firsts, ld32(row + 0xC) + 0x20, 0x14, 0x20});
        first = int32_t(ld32(call(g::ArrayInt5_at, {firsts, uint32_t(at)})));
        uint32_t lasts = f.alloc(0x14);
        call(g::SafeBuffer_read, {row, lasts, ld32(row + 0xC) + 0x34, 0x14, 0x34});
        last = int32_t(ld32(call(g::ArrayInt5_at, {lasts, uint32_t(at)})));
    } else {
        first = readInt(f, row, 0x18);
        last = readInt(f, row, 0x1C);
    }
    int32_t found = 0;
    if (first <= last) {
        for (int32_t id = first;; id++) {
            bool match = false;
            {
                Frame t;
                uint32_t reward = masterRow(t, g::RankingReward_create, uint32_t(id));
                uint32_t rr = ld32(reward);
                if (rr) {
                    int32_t lo = readInt(t, rr, 4);
                    int32_t hi = readInt(t, rr, 8);
                    if (!(score > hi) && !(lo > score)) found = id;
                    match = score <= hi && lo <= score;
                }
                sharedRelease(reward);
            }
            if (match || !(id < last)) break;
        }
    }
    if (found) {
        st8(sceneManager() + SM_FLAG_308, 1);
        uint32_t area = findNode(panel, S_Area, 4);
        call(g::RankingRewardScrollView_createIncentivePanel, {area, 10});
        call(g::RankingRewardScrollView_initLayout, {panel, uint32_t(found), 1});
        setVisible(notice, 0);
    } else {
        st8(sceneManager() + SM_FLAG_308, 0);
        uint32_t txt = findNode(notice, S_TxtNotice, 10);
        setTextIdShadow(txt, TXT_NO_REWARD);
        setVisible(panel, 0);
        setVisible(notice, 1);
    }

    // OK: $_17 {root, layer, shadow, results, index, onClose}
    uint32_t ok = findNode(root, S_ButtonOk, 9);
    {
        Frame t;
        uint32_t lambda = t.alloc(0x38);
        st32(lambda, root);
        st32(lambda + 4, layer);
        st32(lambda + 8, shadow);
        call(g::ResultVector_copy, {lambda + 0xC, results});
        st32(lambda + 0x18, uint32_t(index));
        call(fn::function_vv_copy, {lambda + 0x20, onClose});
        uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x40});
        if (heap) {
            st32(heap, VT_Close);
            st32(heap + 8, ld32(lambda));
            st32(heap + 0xC, ld32(lambda + 4));
            st32(heap + 0x10, ld32(lambda + 8));
            call(g::ResultVector_copy, {heap + 0x14, lambda + 0xC});
            st32(heap + 0x20, ld32(lambda + 0x18));
            call(fn::function_vv_move, {heap + 0x28, lambda + 0x20});
            st32(heap + 0x20, ld32(lambda + 0x18));
        }
        st32(fo + 16, heap);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        functionDestroy(lambda + 0x20);
        call(g::ResultVector_dtor, {lambda + 0xC});
    }
    {
        Frame t;
        uint32_t done = newFunction(t, VT_Opened, {root});
        uint32_t action = SceneUtil::createOpenPopupAnimation(root, done);
        call(g::Node_runAction, {root, action});
        functionDestroy(done);
    }
    sharedRelease(ranking);
}
RW_FUNCTION(0x6FF348, "SceneRanking::openRankingRewardPopup(int, std::function<void()>)", openRankingRewardPopup);
