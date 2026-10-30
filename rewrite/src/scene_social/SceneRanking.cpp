// SceneRanking: the Rankings screen (see SceneRanking.hpp). The reward popup is in SceneRanking_reward.cpp; the
// ranking API calls (callLux/Party/Colosseum/CurrentRankingAPI) are not rewritten yet.
#include "SceneRanking.hpp"

#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;
using Self = SceneRanking;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v;                   // Vec2::Vec2()
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;       // (image, const string&, TextureResType)
constexpr uint32_t String_assign = 0x48D811;               // std::string::assign(const char*, size_t)
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t TextData_format = 0x492929;             // (string out, Type, id, ...)
constexpr uint32_t Date_makeLocalDate = 0x4927CD;          // (Date out, const Date*)
constexpr uint32_t Ranking_create = 0x7912CD;              // master::Ranking::create(shared_ptr out, int id)
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionLayer_assignPointer = 0x60CE4D; // std::function<Layer*()>::operator=(Layer*(*)())
constexpr uint32_t FunctionLayer_copy = 0x5AAC65;          // std::function<Layer*()>(const function&)
constexpr uint32_t FunctionLayer_move = 0x5AAE3D;          // std::function<Layer*()>(function&&)
constexpr uint32_t VectorInt_copy = 0x4AF707;              // vector<int>(const vector&)
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;      // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B;     // (sm, std::function<void()> by value)
constexpr uint32_t SM_showHeader = 0x6131ED;
constexpr uint32_t SM_changeBackground = 0x6128ED;         // (sm, BackGroundType, const string&)
constexpr uint32_t SM_changeTickerMessage = 0x610B39;      // (sm, const string&)
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_isOpenedPopup = 0x621519;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, std::function<void()> by value)
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;   // (function<void(bool)> by value)
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;                 // (function<void(bool)> by value)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;               // (function<void(bool)> by value)
constexpr uint32_t SceneUtil_numberToPlaceString = 0x473689;                 // (string out, int)
constexpr uint32_t SceneRanking_callCurrentRankingAPI = 0x700FF9;            // (this unused, function<void(bool)>)
constexpr uint32_t RankingScrollView_create = 0x54F3FD;                      // (RankingScrollViewType)
constexpr uint32_t RankingScrollView_changeType = 0x54F615;                  // changeRankingScrollViewType(type)
constexpr uint32_t RankingScrollView_setRankingData = 0x5529AD;              // (season, vector<int> by value)
constexpr uint32_t RankingScrollView_setEnabledTapItem = 0x552631;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;
// createSceneLayer<T>() pointers in the GOT (the values the std::function stores)
constexpr uint32_t GOT_createSceneLayer_AdventureSelect = 0xFD669C;
constexpr uint32_t GOT_createSceneLayer_PartyTop = 0xFD6620;
constexpr uint32_t GOT_createSceneLayer_MyPage = 0xFD6078;
}  // namespace g

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_OnEnter = 0xF6A384;          // init $_6 {this}
constexpr uint32_t VT_OnExit = 0xF6A358;           // init $_7 {this}
constexpr uint32_t VT_BackKey = 0xF6A32C;          // init $_8 {this}
constexpr uint32_t VT_BackButton = 0xF6A300;       // init $_9 {this}
constexpr uint32_t VT_RewardButton = 0xF6A27C;     // init $_10 {this}
constexpr uint32_t VT_PeriodButton = 0xF6A250;     // init $_12 {this, $_11 {this, text}}
constexpr uint32_t VT_CategoryButton = 0xF6A224;   // init $_13 {this, $_14 {this, text}}
constexpr uint32_t VT_CategoryApi = 0xF6A568;      // $_14's callCurrentRankingAPI callback {this, arg}
constexpr uint32_t VT_ShowRanking = 0xF6A1F8;      // changeScrollView $_15 {this, text}
constexpr uint32_t VT_PreparationIn = 0xF6A53C;    // $_15's lambda {this}
constexpr uint32_t VT_RunIn = 0xF6A1CC;            // runInAnimation $_16 {}
constexpr uint32_t VT_RunOutDone = 0xF6A1A0;       // runOutAnimationMain lambda {std::function<void()>} (heap, 0x20)
constexpr uint32_t VT_Back = 0xF6A0F0;             // backScene $_21 {bool, std::function<Layer*()>} (heap, 0x28)

// strings
constexpr uint32_t S_Empty = 0xDF8A1A;             // ""
constexpr uint32_t S_Bg = 0xE03AF2;                // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_ColosseumScene = 0xE065C0;    // "cocostudio/publish/RankingColosseumScene_ver130.json"
constexpr uint32_t S_PartyScene = 0xE0658F;        // "cocostudio/publish/RankingPartyScene_ver130.json"
constexpr uint32_t S_PreparationJson = 0xE06500;   // "cocostudio/publish/Ranking_preparation.json"
constexpr uint32_t S_Top3 = 0xE06663;              // "cocostudio/publish/Conpane_Top3.png"
constexpr uint32_t S_Top2 = 0xE06687;              // "cocostudio/publish/Conpane_Top2.png"
constexpr uint32_t S_CenterUI = 0xE02FBA, S_CenterUIButton = 0xE064F0, S_ShadeBottom = 0xE03697;
constexpr uint32_t S_DeckBase2 = 0xE03D8A, S_LeftUI = 0xDF85F0, S_TxtPreparation = 0xE0652C;
constexpr uint32_t S_ButtonBack = 0xE03DBA, S_TxtRankReward = 0xE0653C, S_ButtonRankReward = 0xE0654B;
constexpr uint32_t S_Switch1 = 0xE0655D, S_Switch2 = 0xE0656E, S_Button1 = 0xE0657F, S_Button2 = 0xE06587;
constexpr uint32_t S_Txt = 0xDF94D1, S_TxtTitle = 0xDFF142;
constexpr uint32_t S_TxtRank = 0xDFE900, S_TxtRankLabel = 0xDFE909;
constexpr uint32_t S_Base = 0xDFEF5A, S_Win = 0xE04096, S_Area = 0xDF9378;
constexpr uint32_t S_Union = 0xE065F5, S_TxtTitleColosseum = 0xE065FB, S_TxtTitleParty = 0xE0660F;
constexpr uint32_t S_TxtPeriod = 0xE0661F, S_TxtPeriodColosseum = 0xE0662A;
constexpr uint32_t S_TxtUpdateTime = 0xE0663F, S_TxtNextTime = 0xE0664E;
constexpr uint32_t S_FmtSS = 0xDFDAC7;             // "%s %s"
constexpr uint32_t S_FmtSSS = 0xE0665B;            // "%s %s%s"
constexpr uint32_t S_FmtDS = 0xDFF1AD;             // "%d%s"

// text ids
constexpr int32_t TXT_RANK_REWARD = 103900010;
constexpr int32_t TXT_PERIOD_CURRENT = 103900005, TXT_PERIOD_PREVIOUS = 103900006;
constexpr int32_t TXT_CATEGORY_LUX = 103900064, TXT_CATEGORY_PARTY = 103900063;
constexpr int32_t TXT_TICKER_PARTY = 100620034, TXT_TICKER_COLOSSEUM = 100620035, TXT_TICKER_LUX = 100620046;
constexpr int32_t TXT_TITLE_PARTY = 103900003, TXT_TITLE_COLOSSEUM = 103900004, TXT_TITLE_LUX = 103900059;
constexpr int32_t TXT_PERIOD = 103900011, TXT_PERIOD_DATES = 103901038;
constexpr int32_t TXT_UPDATE_TIME = 103900031, TXT_NEXT_TIME = 103900032, TXT_TIME = 103901065, TXT_TIME_SUFFIX = 103900033;
constexpr int32_t TXT_MY_RANK = 103900013, TXT_MY_RANK_NONE = 103900051, TXT_MY_RANK_NONE_COLOSSEUM = 103900052;
constexpr int32_t TXT_NOT_JOINED = 103900066, TXT_TALLYING = 103900037, TXT_NO_PARTY_ENTRIES = 103900040;
constexpr int32_t TXT_NO_ENTRIES = 103900015, TXT_CLOSED = 103900027;

// SceneManager::SelectRanking
constexpr int32_t SELECT_PARTY = 1, SELECT_COLOSSEUM = 2, SELECT_LUX = 4;
constexpr int32_t PERIOD_CURRENT = 1, PERIOD_PREVIOUS = 2;
constexpr int32_t STATUS_OPEN = 2;                 // ranking data status: the list is available

// SceneManager members
constexpr uint32_t SM_SELECT_RANKING = 0x290;
constexpr uint32_t SM_FLAG_308 = 0x308, SM_FLAG_30A = 0x30A, SM_FLAG_4CC = 0x4CC;
constexpr uint32_t SM_CALLBACK_548 = 0x548;        // std::function<void(bool)>
// APIManager members: the ranking data (status first; the entry list as a vector)
constexpr uint32_t API_LUX_CURRENT = 0x2BC, API_LUX_PREVIOUS = 0x2C4;               // entries at +0x14, 32 bytes
constexpr uint32_t API_PARTY_CURRENT = 0x2CC, API_PARTY_PREVIOUS = 0x2D4;           // entries at +0x4, 120 bytes
constexpr uint32_t API_COLOSSEUM_CURRENT = 0x2DC, API_COLOSSEUM_PREVIOUS = 0x2E4;   // entries at +0x14, 48 bytes
constexpr uint32_t API_RANKING_REWARDS = 0x2F4;    // ranking ids with a result at +0x18
constexpr uint32_t API_USER_PARTY = 0x84;          // SafeBuffer: byte 16 = in a party
constexpr uint32_t API_USER = 0x94;                // SafeBuffer: union id at 0x30

// node virtual slots (byte offsets)
constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_runInAnimation = 0x300;
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C, V_layoutSetTouchEnabled = 0x31C;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }
int32_t selectRanking() { return int32_t(ld32(sceneManager() + SM_SELECT_RANKING)); }
Self *host(uint32_t a) { return rw::host_ptr<Self>(a); }
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// node->runAction(EaseSineOut(make(node, 0, 0, done)))
void runEased(Self::Addr (*make)(Self::Addr, float, float, Self::Addr), uint32_t node, uint32_t done) {
    uint32_t action = make(node, 0.0f, 0.0f, done);
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
void runEasedEmpty(Self::Addr (*make)(Self::Addr, float, float, Self::Addr), uint32_t node) {
    Frame f;
    uint32_t done = newEmptyFunction(f);
    runEased(make, node, done);
    functionDestroy(done);
}
// text = "%s %s" of two text table entries
void setTextIdPair(uint32_t text, int32_t first, int32_t second) {
    Frame f;
    uint32_t a = textData(f, first);
    uint32_t b = textData(f, second);
    uint32_t buf = stringBuffer(f, {S_FmtSS, textDataStr(a), textDataStr(b)});
    setTextC(text, buf);
    sharedRelease(b);
    sharedRelease(a);
}
// Date (local): year u16, month, day, hour, minute bytes
struct LocalDate {
    uint32_t addr;
    uint32_t year() const { return rw::load<uint16_t>(addr); }
    uint32_t month() const { return ld8(addr + 2); }
    uint32_t day() const { return ld8(addr + 3); }
    uint32_t hour() const { return ld8(addr + 4); }
    uint32_t minute() const { return ld8(addr + 5); }
};
LocalDate localDate(Frame &f, uint32_t date) {
    uint32_t out = f.alloc(8);
    call(g::Date_makeLocalDate, {out, date});
    return LocalDate{out};
}
// text = "<label> <from - to>" (TextData::format of the two dates)
void setPeriodText(uint32_t text, LocalDate from, LocalDate to) {
    Frame f;
    uint32_t label = textData(f, TXT_PERIOD);
    uint32_t dates = f.alloc(12);
    call(g::TextData_format, {dates, 0, uint32_t(TXT_PERIOD_DATES), from.year(), from.month(), from.day(), to.year(),
                               to.month(), to.day()});
    uint32_t buf = stringBuffer(f, {S_FmtSS, textDataStr(label), strData(dates)});
    setTextC(text, buf);
    strFree(dates);
    sharedRelease(label);
}
// text = "<label> <date time><suffix>"
void setTimeText(uint32_t text, int32_t labelId, LocalDate at) {
    Frame f;
    uint32_t label = textData(f, labelId);
    uint32_t time = f.alloc(12);
    call(g::TextData_format, {time, 0, uint32_t(TXT_TIME), at.year(), at.month(), at.day(), at.hour(), at.minute()});
    uint32_t suffix = textData(f, TXT_TIME_SUFFIX);
    uint32_t buf = stringBuffer(f, {S_FmtSSS, textDataStr(label), strData(time), textDataStr(suffix)});
    setTextC(text, buf);
    sharedRelease(suffix);
    strFree(time);
    sharedRelease(label);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self::Addr Self::create() {
    uint32_t p = call(fn::operator_new, {SIZE});
    construct(host(p));
    if (vcall(p, V_init, {p}) == 1) {
        call(g::Ref_autorelease, {p});
        return p;
    }
    vcall(p, V_deleting, {p});
    return 0;
}
RW_FUNCTION(0x64A270, "SceneRanking::create()", SceneRanking::create);

Self *Self::construct(Self *self) {
    uint32_t me = rw::guest_addr(self);
    call(g::Layer_ctor, {me});
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    call(g::aeabi_memset, {me + 0x28C, 0x14, 0});
    call(g::Vec2_ctor, {me + 0x2A0});
    self->period = 0;
    self->select = SELECT_LUX;
    call(g::aeabi_memset, {me + 0x2B0, 0x18, 0});
    call(g::Vec2_ctor, {me + 0x2C8});
    self->canBack = 0;
    self->changing = 0;
    return self;
}
RW_FUNCTION(0x64A2C4, "SceneRanking::SceneRanking()", SceneRanking::construct);

Self *Self::destroy(Self *self) {
    uint32_t me = rw::guest_addr(self);
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    call(g::Vec2_dtor, {me + 0x2C8});
    call(g::Vec2_dtor, {me + 0x2A0});
    call(g::Layer_dtor, {me});
    return self;
}
RW_FUNCTION(0x6FBD80, "SceneRanking::~SceneRanking()", SceneRanking::destroy);

void Self::destroyDelete(Self *self) {
    destroy(self);
    call(fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x6FBE1C, "SceneRanking::~SceneRanking() (deleting)", SceneRanking::destroyDelete);

// ------------------------------------------------------------------------------------------------ init
uint32_t Self::init(Self *self) {
    uint32_t me = rw::guest_addr(self);
    if (call(g::Layer_init, {me}) != 1) return 0;
    Frame f;
    {
        uint32_t fo = newFunction(f, VT_OnEnter, {me});
        call(g::FunctionVV_assign, {me + 0x210, fo});
        functionDestroy(fo);
        fo = newFunction(f, VT_OnExit, {me});
        call(g::FunctionVV_assign, {me + 0x228, fo});
        functionDestroy(fo);
    }
    self->canBack = 1;
    {
        uint32_t m = sceneManager();
        uint32_t fo = newFunction(f, VT_BackKey, {me});
        call(g::SM_setOnBackKeyCallback, {m, fo});
        functionDestroy(fo);
    }
    call(g::SM_showHeader, {sceneManager()});
    self->changing = 0;
    {
        uint32_t m = sceneManager();
        uint32_t s = newStr(f, S_Bg, 0x1A);
        call(g::SM_changeBackground, {m, 0, s});
        strFree(s);
    }
    initScene(self, selectRanking());
    self->centerUI = findNode(self->root, S_CenterUI, 8);
    self->centerUIButton = findNode(self->root, S_CenterUIButton, 15);
    self->shadeBottom = findNode(self->root, S_ShadeBottom, 12);
    if (self->shadeBottom) {
        uint32_t deck = findNode(self->shadeBottom, S_DeckBase2, 9);
        vcall(deck, V_widgetSetTouchEnabled, {deck, 1});
    }
    self->leftUI = findNode(self->root, S_LeftUI, 6);
    uint32_t prep = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PreparationJson});
    self->preparation = prep;
    uint32_t pos = vcall(prep, node::getPosition, {prep});
    rw::set<uint32_t>(self, 0x2CC, ld32(pos + 4));
    rw::set<uint32_t>(self, 0x2C8, ld32(pos));
    vcall(self->root, node::addChild, {self->root, self->preparation});
    uint32_t prepText = findNode(self->preparation, S_TxtPreparation, 15);
    SceneUtil::setShadow(prepText);
    setVisible(prepText, 0);
    initRankingTitle(self);
    initMyRanking(self);
    initRankingScrollView(self);
    if (self->leftUI) {
        uint32_t back = findNode(self->leftUI, S_ButtonBack, 11);
        SceneUtil::backBtnSetPosY(back);
        addTouch(back, VT_BackButton, {me});
    } else {
        self->canBack = 0;
    }
    uint32_t rewardText = findNode(self->centerUIButton, S_TxtRankReward, 14);
    setTextId(rewardText, TXT_RANK_REWARD);
    SceneUtil::setShadow(rewardText);
    uint32_t a = api();
    uint32_t rewardButton = findNode(self->centerUIButton, S_ButtonRankReward, 17);
    uint32_t rewards = ld32(a + API_RANKING_REWARDS);
    if (ld32(rewards + 0x18) == ld32(rewards + 0x1C)) setVisible(rewardButton, 0);
    else addTouch(rewardButton, VT_RewardButton, {me});

    // the period switch and the category switch (swapped outside the colosseum ranking)
    uint32_t sw1 = newStr(f, S_Switch1, 16);
    uint32_t sw2 = newStr(f, S_Switch2, 16);
    uint32_t bt1 = newStr(f, S_Button1, 7);
    uint32_t bt2 = newStr(f, S_Button2, 7);
    if (selectRanking() != SELECT_COLOSSEUM) {
        call(g::String_assign, {sw1, S_Switch2, 16});
        call(g::String_assign, {sw2, S_Switch1, 16});
        call(g::String_assign, {bt1, S_Button2, 7});
        call(g::String_assign, {bt2, S_Button1, 7});
    }
    uint32_t periodSwitch = SceneUtil::searchNodeByName(self->centerUIButton, sw1);
    uint32_t periodText = findNode(periodSwitch, S_Txt, 3);
    SceneUtil::setShadow(periodText);
    uint32_t periodCaptures = f.alloc(8);
    st32(periodCaptures, me);
    st32(periodCaptures + 4, periodText);
    selectPeriod(periodCaptures, PERIOD_PREVIOUS, 0);
    uint32_t periodButton = SceneUtil::searchNodeByName(periodSwitch, bt1);
    addTouch(periodButton, VT_PeriodButton, {me, me, periodText});
    uint32_t categorySwitch = SceneUtil::searchNodeByName(self->centerUIButton, sw2);
    if (categorySwitch) {
        findNode(self->centerUI, S_TxtTitle, 9);
        uint32_t categoryText = findNode(categorySwitch, S_Txt, 3);
        SceneUtil::setShadow(categoryText);
        uint32_t categoryCaptures = f.alloc(8);
        st32(categoryCaptures, me);
        st32(categoryCaptures + 4, categoryText);
        uint32_t categoryButton = SceneUtil::searchNodeByName(categorySwitch, bt2);
        addTouch(categoryButton, VT_CategoryButton, {me, me, categoryText});
        selectCategory(categoryCaptures, self->select, 0, 0);
    }
    vcall(me, V_runInAnimation, {me});
    strFree(bt2);
    strFree(bt1);
    strFree(sw2);
    strFree(sw1);
    return 1;
}
RW_FUNCTION(0x6FBE80, "SceneRanking::init()", SceneRanking::init);

// The scene file of the category (colosseum or party layout; Lux uses the party one)
void Self::initScene(Self *self, int32_t select) {
    uint32_t me = rw::guest_addr(self);
    self->select = select;
    {
        Frame f;
        uint32_t s = select == SELECT_COLOSSEUM ? newStr(f, S_ColosseumScene, 0x34) : newStr(f, S_PartyScene, 0x30);
        self->root = SceneUtil::createNodeWithSceneFile(s);
        strFree(s);
    }
    changeTickerMessage(self, select);
    vcall(me, node::addChild, {me, self->root});
}
RW_FUNCTION(0x6FC928, "SceneRanking::initScene(SceneManager::SelectRanking)", SceneRanking::initScene);

void Self::changeTickerMessage(Self *, int32_t select) {
    int32_t id = select == SELECT_PARTY ? TXT_TICKER_PARTY
                 : select == SELECT_COLOSSEUM ? TXT_TICKER_COLOSSEUM : TXT_TICKER_LUX;
    uint32_t m = sceneManager();
    Frame f;
    uint32_t td = textData(f, id);
    uint32_t s = newStr(f, textDataStr(td));
    call(g::SM_changeTickerMessage, {m, s});
    strFree(s);
    sharedRelease(td);
}
RW_FUNCTION(0x6FE008, "SceneRanking::changeTickerMessage(SceneManager::SelectRanking)", SceneRanking::changeTickerMessage);

// The title area: the category title (with the period word), the ranking period and, for parties, the update
// and next update times.
void Self::initRankingTitle(Self *self) {
    uint32_t a = api();
    uint32_t unionImage = findNode(self->centerUI, S_Union, 5);
    uint32_t title = findNode(self->centerUI, S_TxtTitle, 9);
    uint32_t titleColosseum = findNode(self->centerUI, S_TxtTitleColosseum, 19);
    uint32_t titleParty = findNode(self->centerUI, S_TxtTitleParty, 15);
    uint32_t periodText = findNode(self->centerUI, S_TxtPeriod, 10);
    uint32_t periodColosseum = findNode(self->centerUI, S_TxtPeriodColosseum, 20);
    uint32_t updateTime = findNode(self->centerUI, S_TxtUpdateTime, 14);
    uint32_t nextTime = findNode(self->centerUI, S_TxtNextTime, 12);
    setVisible(unionImage, 0);
    setVisible(title, 0);
    setVisible(periodText, 0);
    setVisible(titleColosseum, 0);
    setVisible(titleParty, 0);
    setVisible(periodColosseum, 0);
    setVisible(updateTime, 0);
    setVisible(nextTime, 0);
    int32_t select = selectRanking();
    Frame f;
    if (select == SELECT_PARTY) {
        uint32_t rewards = ld32(api() + API_RANKING_REWARDS);
        uint32_t ids = ld32(rewards + 0x18);
        if (ld32(rewards + 0x1C) == ids) return;
        // the ranking's own period (master::Ranking: start at 8, end at 16) and the party data's update times
        uint32_t ranking = masterRow(f, g::Ranking_create, ld32(ids));
        uint32_t row = ld32(ranking);
        uint32_t start = f.alloc(8);
        call(g::SafeBuffer_read, {row, start, ld32(row + 0xC) + 8, 8, 8});
        LocalDate from = localDate(f, start);
        uint32_t end = f.alloc(8);
        call(g::SafeBuffer_read, {row, end, ld32(row + 0xC) + 16, 8, 16});
        LocalDate to = localDate(f, end);
        uint32_t party = api() + API_PARTY_CURRENT;
        LocalDate updated = localDate(f, ld32(party) + 0x14);
        LocalDate next = localDate(f, ld32(party) + 0x1C);
        setVisible(titleParty, 1);
        SceneUtil::setShadow(titleParty);
        setTextIdPair(titleParty, TXT_TITLE_PARTY, self->period == PERIOD_CURRENT ? TXT_PERIOD_CURRENT : TXT_PERIOD_PREVIOUS);
        setVisible(periodColosseum, 1);
        setVisible(updateTime, 1);
        setVisible(nextTime, 1);
        setPeriodText(periodColosseum, from, to);
        setTimeText(updateTime, TXT_UPDATE_TIME, updated);
        setTimeText(nextTime, TXT_NEXT_TIME, next);
        SceneUtil::setShadow(periodColosseum);
        SceneUtil::setShadow(updateTime);
        SceneUtil::setShadow(nextTime);
        sharedRelease(ranking);
    } else if (select == SELECT_COLOSSEUM) {
        uint32_t data = ld32(a + API_COLOSSEUM_CURRENT);
        LocalDate from = localDate(f, data + 4);
        LocalDate to = localDate(f, ld32(a + API_COLOSSEUM_CURRENT) + 0xC);
        setVisible(titleColosseum, 1);
        SceneUtil::setShadow(titleColosseum);
        setTextIdPair(titleColosseum, TXT_TITLE_COLOSSEUM,
                      self->period == PERIOD_CURRENT ? TXT_PERIOD_CURRENT : TXT_PERIOD_PREVIOUS);
        setVisible(periodColosseum, 1);
        setPeriodText(periodColosseum, from, to);
        SceneUtil::setShadow(periodColosseum);
    } else if (select == SELECT_LUX) {
        uint32_t data = ld32(a + API_LUX_CURRENT);
        LocalDate from = localDate(f, data + 4);
        LocalDate to = localDate(f, ld32(a + API_LUX_CURRENT) + 0xC);
        setVisible(unionImage, 1);
        // the player's union emblem
        uint32_t user = ld32(a + API_USER);
        uint32_t unionId = f.alloc(4);
        call(g::SafeBuffer_read, {user, unionId, ld32(user + 0xC) + 0x30, 4, 0x30});
        uint32_t path = f.alloc(12);
        SceneUtil::createUnionImagePath(path, int32_t(ld32(unionId)), 0, 0);
        {
            uint32_t s = newStr(f, strData(path));
            call(g::ImageView_loadTexture, {unionImage, s, 0});
            strFree(s);
        }
        strFree(path);
        setVisible(title, 1);
        SceneUtil::setShadow(title);
        setTextIdPair(title, TXT_TITLE_LUX, self->period == PERIOD_CURRENT ? TXT_PERIOD_CURRENT : TXT_PERIOD_PREVIOUS);
        setVisible(periodText, 1);
        setPeriodText(periodText, from, to);
        SceneUtil::setShadow(periodText);
    }
}
RW_FUNCTION(0x6FCA7C, "SceneRanking::initRankingTitle()", SceneRanking::initRankingTitle);

void Self::initMyRanking(Self *self) {
    self->txtRank = findNode(self->centerUI, S_TxtRank, 8);
    self->txtRankLabel = findNode(self->centerUI, S_TxtRankLabel, 14);
    setTextC(self->txtRank, S_Empty);
    setTextC(self->txtRankLabel, S_Empty);
    SceneUtil::setShadow(self->txtRank);
    SceneUtil::setShadow(self->txtRankLabel);
}
RW_FUNCTION(0x6FD858, "SceneRanking::initMyRanking()", SceneRanking::initMyRanking);

// The list panel: the RankingScrollView of the category in the panel's Area (party 1, colosseum 2, else Lux 0).
void Self::initRankingScrollView(Self *self) {
    uint32_t base = findNode(self->centerUI, S_Base, 4);
    setVisible(base, 1);
    uint32_t win = findNode(base, S_Win, 3);
    setVisible(win, 1);
    uint32_t area = findNode(base, S_Area, 4);
    setVisible(area, 1);
    vcall(area, V_layoutSetTouchEnabled, {area, 1});
    int32_t select = selectRanking();
    int32_t type = select == SELECT_PARTY ? 1 : select == SELECT_COLOSSEUM ? 2 : 0;
    {
        Frame f;
        uint32_t s = newStr(f, select == SELECT_COLOSSEUM ? S_Top2 : S_Top3, 0x23);
        call(g::ImageView_loadTexture, {win, s, 0});
        strFree(s);
    }
    self->scrollView = call(g::RankingScrollView_create, {uint32_t(type)});
    vcall(area, node::addChild, {area, self->scrollView});
    call(g::IScrollProtocol_setMaxColumn, {self->scrollView, 1});
    call(g::RankingScrollView_setEnabledTapItem, {self->scrollView, 1});
    uint32_t sv = self->scrollView;
    vcall(sv, node::setContentSize, {sv, vcall(area, node::getContentSize, {area})});
    uint32_t pos = vcall(self->scrollView, node::getPosition, {self->scrollView});
    rw::set<uint32_t>(self, 0x2A4, ld32(pos + 4));
    rw::set<uint32_t>(self, 0x2A0, ld32(pos));
}
RW_FUNCTION(0x6FD9FC, "SceneRanking::initRankingScrollView()", SceneRanking::initRankingScrollView);

// ------------------------------------------------------------------------------------------------ switches
// init $_11: the period switch (2 shows the "current" word, 1 the "previous" one), then refresh the list.
void Self::selectPeriod(Addr captures, int32_t period, uint32_t animate) {
    Self *self = host(ld32(captures));
    if (self->period == period) return;
    self->period = period;
    if (period == PERIOD_PREVIOUS) setTextId(ld32(captures + 4), TXT_PERIOD_CURRENT);
    else if (period == PERIOD_CURRENT) setTextId(ld32(captures + 4), TXT_PERIOD_PREVIOUS);
    changeScrollView(self, animate);
}
RW_FUNCTION(0x6FDCF0, "SceneRanking::init $_11::operator()(int, bool)", SceneRanking::selectPeriod);

// init $_14: the category switch (Lux / party); a change with callApi asks the server for the new list.
void Self::selectCategory(Addr captures, int32_t select, uint32_t callApi, int32_t arg) {
    uint32_t me = ld32(captures);
    Self *self = host(me);
    int32_t old = self->select;
    self->select = select;
    st32(sceneManager() + SM_SELECT_RANKING, uint32_t(select));
    if (self->select == SELECT_LUX) setTextId(ld32(captures + 4), TXT_CATEGORY_LUX);
    else if (self->select == SELECT_PARTY) setTextId(ld32(captures + 4), TXT_CATEGORY_PARTY);
    if (old != select && callApi == 1) {
        Frame f;
        uint32_t fo = newFunction(f, VT_CategoryApi, {me, uint32_t(arg)});
        call(g::SceneRanking_callCurrentRankingAPI, {me, fo});
        functionDestroy(fo);
    }
}
RW_FUNCTION(0x6FDE40, "SceneRanking::init $_14::operator()(SelectRanking, bool, int)", SceneRanking::selectCategory);

// Refresh the list: animated, the old list and the "tallying" panel slide out first ($_15 then shows the new one).
void Self::changeScrollView(Self *self, uint32_t animate) {
    uint32_t me = rw::guest_addr(self);
    self->changing = 1;
    uint32_t text = findNode(self->preparation, S_TxtPreparation, 15);
    Frame f;
    uint32_t captures = f.alloc(8);
    st32(captures, me);
    st32(captures + 4, text);
    if (animate == 1) {
        runEasedEmpty(SceneUtil::createRightOutAnimation, self->preparation);
        uint32_t list = self->scrollView;
        uint32_t done = newFunction(f, VT_ShowRanking, {me, text});
        runEased(SceneUtil::createRightOutAnimation, list, done);
        functionDestroy(done);
    } else {
        showRanking(captures);
    }
}
RW_FUNCTION(0x6FE0E4, "SceneRanking::changeScrollView(bool)", SceneRanking::changeScrollView);

// changeScrollView $_15: fill the list with the chosen ranking (its entries are 0..n-1 of the API data), show the
// player's own rank and the "tallying" text of the data's status.
void Self::showRanking(Addr captures) {
    uint32_t me = ld32(captures);
    Self *self = host(me);
    vcall(self->scrollView, node::setPosition, {self->scrollView, me + 0x2A0});
    vcall(self->preparation, node::setPosition, {self->preparation, me + 0x2C8});
    initRankingTitle(self);
    Frame f;
    uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t value = f.alloc(4);
    int32_t select = selectRanking();
    int32_t viewType = 0;
    uint32_t season = me;      // an unknown category passes the scene pointer (the register it happens to hold)
    int32_t status = 0;
    uint32_t data = 0, listOffset = 0;
    int32_t kind = 0;          // entry size: 1 party (120), 2 colosseum (48), 3 Lux (32)
    bool current = self->period == PERIOD_CURRENT;
    if (select == SELECT_PARTY) {
        viewType = 1;
        season = current ? 0 : 2;
        data = ld32(api() + (current ? API_PARTY_CURRENT : API_PARTY_PREVIOUS));
        listOffset = 4;
        kind = 1;
    } else if (select == SELECT_COLOSSEUM) {
        viewType = 2;
        season = current ? 0 : 2;
        data = ld32(api() + (current ? API_COLOSSEUM_CURRENT : API_COLOSSEUM_PREVIOUS));
        listOffset = 0x14;
        kind = 2;
    } else if (select == SELECT_LUX) {
        viewType = 0;
        season = current ? 1 : 3;
        data = ld32(api() + (current ? API_LUX_CURRENT : API_LUX_PREVIOUS));
        listOffset = 0x14;
        kind = 3;
    }
    if (data) {
        status = int32_t(ld32(data));
        if (status == STATUS_OPEN) {
            auto count = [&]() -> int32_t {
                int32_t bytes = int32_t(ld32(data + listOffset + 4) - ld32(data + listOffset));
                if (kind == 1) return int32_t(uint32_t(bytes >> 3) * 0xEEEEEEEFu);
                if (kind == 2) return int32_t(uint32_t(bytes >> 4) * 0xAAAAAAABu);
                return bytes >> 5;
            };
            st32(value, 0);
            if (int32_t(ld32(data + listOffset + 4) - ld32(data + listOffset)) >= 1) {
                int32_t k = 1;
                for (;;) {
                    uint32_t end = ld32(ids + 4);
                    if (end == ld32(ids + 8)) {
                        call(g::VectorInt_pushBackSlow, {ids, value});
                    } else {
                        uint32_t n = 0;
                        if (end) {
                            st32(end, uint32_t(k - 1));
                            n = ld32(ids + 4);
                        }
                        st32(ids + 4, n + 4);
                    }
                    st32(value, uint32_t(k));
                    bool more = k < count();
                    k++;
                    if (!more) break;
                }
            }
        }
    }
    call(g::RankingScrollView_changeType, {self->scrollView, uint32_t(viewType)});
    {
        uint32_t sv = self->scrollView;
        uint32_t copy = f.alloc(12);
        call(g::VectorInt_copy, {copy, ids});
        call(g::RankingScrollView_setRankingData, {sv, season, copy});
        call(g::VectorBaseInt_dtor, {copy});
    }
    call(g::IScrollProtocol_setIndex, {self->scrollView, 0});
    call(g::IScrollProtocol_updateLayout, {self->scrollView, 0});
    runEasedEmpty(SceneUtil::createRightInAnimation, self->scrollView);

    // the player's own rank (party: the previous data's, Lux: the previous data's)
    int32_t own = selectRanking();
    int32_t rank = 0;
    if (own == SELECT_PARTY) rank = int32_t(ld32(ld32(api() + API_PARTY_PREVIOUS) + 0x10));
    else if (own == SELECT_LUX) rank = int32_t(ld32(ld32(api() + API_LUX_PREVIOUS) + 0x20));
    if (rank) {
        setTextId(self->txtRank, TXT_MY_RANK);
        uint32_t place = f.alloc(12);
        call(g::SceneUtil_numberToPlaceString, {place, uint32_t(rank)});
        uint32_t buf = stringBuffer(f, {S_FmtDS, uint32_t(rank), strData(place)});
        setTextC(self->txtRankLabel, buf);
        strFree(place);
    } else {
        int32_t sel = selectRanking();
        setTextId(self->txtRank, sel == SELECT_COLOSSEUM ? TXT_MY_RANK_NONE_COLOSSEUM : TXT_MY_RANK_NONE);
        setTextC(self->txtRankLabel, S_Empty);
    }
    {
        uint32_t done = newFunction(f, VT_PreparationIn, {me});
        runEased(SceneUtil::createRightInAnimation, self->preparation, done);
        functionDestroy(done);
    }
    uint32_t text = ld32(captures + 4);
    setVisible(text, ld32(ids + 4) == ld32(ids) ? 1 : 0);
    bool inParty = false;
    uint32_t sb = ld32(api() + API_USER_PARTY);
    if (sb) {
        uint32_t b = f.alloc(4);
        call(g::SafeBuffer_read, {sb, b, ld32(sb + 0xC) + 0x10, 1, 0x10});
        inParty = ld8(b) != 0;
    }
    switch (status) {
    case 1:
        setTextId(text, !inParty && selectRanking() == SELECT_PARTY ? TXT_NOT_JOINED : TXT_TALLYING);
        break;
    case 3:
        if (selectRanking() == SELECT_LUX)
            setTextId(text, inParty || selectRanking() != SELECT_PARTY ? TXT_NO_PARTY_ENTRIES : TXT_NOT_JOINED);
        else
            setTextId(text, inParty || selectRanking() != SELECT_PARTY ? TXT_NO_ENTRIES : TXT_NOT_JOINED);
        break;
    case 4:
        setTextId(text, inParty || selectRanking() != SELECT_PARTY ? TXT_CLOSED : TXT_NOT_JOINED);
        break;
    default:   // 2 (open) and anything else
        setTextC(text, S_Empty);
        break;
    }
    call(g::VectorBaseInt_dtor, {ids});
}
RW_FUNCTION(0x6FE270, "SceneRanking::changeScrollView $_15::operator()()", SceneRanking::showRanking);

// ------------------------------------------------------------------------------------------------ animations
void Self::runInAnimation(Self *self) {
    Frame f;
    {
        uint32_t done = newFunction(f, VT_RunIn, {});
        runEased(SceneUtil::createLeftInAnimation, self->centerUI, done);
        functionDestroy(done);
    }
    runEasedEmpty(SceneUtil::createLeftInAnimation, self->centerUIButton);
    runEasedEmpty(SceneUtil::createLeftInAnimation, self->shadeBottom);
    if (self->leftUI) runEasedEmpty(SceneUtil::createLeftInAnimation, self->leftUI);
}
RW_FUNCTION(0x6FEEE8, "SceneRanking::runInAnimation()", SceneRanking::runInAnimation);

// Slide everything out; `done` (std::function<void()>) runs when the centre panel is gone.
void Self::runOutAnimationMain(Self *self, Addr done) {
    if (self->scrollView) call(g::RankingScrollView_setEnabledTapItem, {self->scrollView, 0});
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    uint32_t center = self->centerUI;
    {
        Frame f;
        uint32_t captured = f.alloc(20);
        call(fn::function_vv_copy, {captured, done});
        uint32_t onDone = f.alloc(20);
        heapFunction(onDone, VT_RunOutDone, 0x20, 8, fn::function_vv_move, captured);
        runEased(SceneUtil::createLeftOutAnimation, center, onDone);
        functionDestroy(onDone);
        functionDestroy(captured);
    }
    runEasedEmpty(SceneUtil::createLeftOutAnimation, self->centerUIButton);
    runEasedEmpty(SceneUtil::createLeftOutAnimation, self->shadeBottom);
    runEasedEmpty(SceneUtil::createLeftOutAnimation, self->preparation);
    if (self->leftUI) runEasedEmpty(SceneUtil::createLeftOutAnimation, self->leftUI);
}
RW_FUNCTION(0x6FF0A4, "SceneRanking::runOutAnimationMain(std::function<void()>)", SceneRanking::runOutAnimationMain);

// ------------------------------------------------------------------------------------------------ back
// Back: close an open popup, else return where the screen was opened from (party top, adventure select or MyPage,
// the latter with a fade), after refreshing that screen's data.
void Self::backScene(Self *self) {
    uint32_t me = rw::guest_addr(self);
    if (!self->canBack) return;
    if (call(g::SM_isOpenedPopup, {sceneManager()}) == 1) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 0});
        return;
    }
    self->canBack = 0;
    st8(sceneManager() + SM_FLAG_308, 0);
    Frame f;
    uint32_t layer = newEmptyFunction(f);
    uint32_t fade = 0;
    auto resetHeaderCallback = [&]() {
        uint32_t m = sceneManager();
        uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    };
    auto callApi = [&](uint32_t api) {
        uint32_t none = newEmptyFunction(f);
        call(api, {none});
        functionDestroy(none);
    };
    if (self->backTo == 2) {
        resetHeaderCallback();
        callApi(g::SceneAdventureSelect_callAdventureSelectAPI);
        call(g::FunctionLayer_assignPointer, {layer, ld32(g::GOT_createSceneLayer_AdventureSelect)});
        st8(sceneManager() + SM_FLAG_30A, 1);
    } else if (self->backTo == 1) {
        resetHeaderCallback();
        callApi(g::ScenePartyTop_callPartyTopAPI);
        call(g::FunctionLayer_assignPointer, {layer, ld32(g::GOT_createSceneLayer_PartyTop)});
    } else {
        resetHeaderCallback();
        callApi(g::SceneUtil_callPrepareMypageAPIs);
        call(g::FunctionLayer_assignPointer, {layer, ld32(g::GOT_createSceneLayer_MyPage)});
        fade = 1;
    }
    // $_21 {fade, layer}: the lambda on the stack, then moved into the heap __func
    uint32_t lambda = f.alloc(0x20);
    st8(lambda, uint8_t(fade));
    functionCopy(lambda + 8, layer);
    uint32_t back = f.alloc(20);
    st32(back + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, VT_Back);
        st8(heap + 8, ld8(lambda));
        call(g::FunctionLayer_move, {heap + 0x10, lambda + 8});
        st8(heap + 8, ld8(lambda));
    }
    st32(back + 16, heap);
    call(g::IInOutAnimation_runOutAnimation, {me + 0x288, back});
    functionDestroy(back);
    functionDestroy(lambda + 8);
    functionDestroy(layer);
}
RW_FUNCTION(0x700C44, "SceneRanking::backScene()", SceneRanking::backScene);

Self::Addr Self::backCloneLambda(Addr functor) {
    uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, VT_Back);
        st8(heap + 8, ld8(functor + 8));
        call(g::FunctionLayer_copy, {heap + 0x10, functor + 0x10});
        st8(heap + 8, ld8(functor + 8));
    }
    return heap;
}
RW_FUNCTION(0x701194, "SceneRanking::backScene $_21 __func::__clone()", SceneRanking::backCloneLambda);

void Self::backLambda(Addr functor) {
    uint32_t fade = ld8(functor + 8);
    uint32_t m = sceneManager();
    if (fade) call(g::SM_changeSceneLayer, {m, functor + 0x10, 1, 1, 1});
    else call(g::SM_changeSceneLayer, {m, functor + 0x10, 1, 0, 0});
}
RW_FUNCTION(0x701278, "SceneRanking::backScene $_21 __func::operator()()", SceneRanking::backLambda);

// ------------------------------------------------------------------------------------------------ buttons
namespace {
constexpr uint32_t SceneRanking_openRankingRewardPopup = 0x6FF349;   // static (int, std::function<void()> by value)
constexpr uint32_t VT_BackOutDone = 0xF6A2A8;      // $_10's run-out end {this}
constexpr uint32_t VT_BackLayer = 0xF6A2D4;        // the next scene's layer factory {backTo}
constexpr uint32_t SM_SCENE_KIND = 0x290;          // SceneManager: int
constexpr uint32_t SCENE_KIND_RANKING = 4;
constexpr uint32_t TOUCH_ENDED = 2;
constexpr int32_t RUN_IN_ANIMATION = 0x288;        // the scene's IInOutAnimation base
}  // namespace

// runInAnimation $_16: the run-in done -> the ranking reward popup (if one is due)
static void rankingRunInDone(Self::Addr) {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(SceneRanking_openRankingRewardPopup, {0, none});
    functionDestroy(none);
}
RW_FUNCTION(0x701DAC, "SceneRanking::runInAnimation $_16::operator()()", rankingRunInDone);

// init $_13 {this, $_14's captures}: the category button -> Lux <-> party (with the API)
static void rankingCategoryTouch(Self::Addr functor, Self::Addr, Self::Addr typep) {
    if (ld32(typep) != TOUCH_ENDED) return;
    Self *self = host(ld32(functor + 4));
    if (self->changing) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const int32_t next = self->select == SELECT_LUX ? SELECT_PARTY : SELECT_LUX;
    call(0x6FDE41, {functor + 8, uint32_t(next), 1, 1});
}
RW_FUNCTION(0x701F72, "SceneRanking::init $_13::operator() (category)", rankingCategoryTouch);

// init $_12 {this, $_11's captures}: the period button -> current <-> previous
static void rankingPeriodTouch(Self::Addr functor, Self::Addr, Self::Addr typep) {
    if (ld32(typep) != TOUCH_ENDED) return;
    Self *self = host(ld32(functor + 4));
    if (self->changing) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const int32_t next = self->period == PERIOD_CURRENT ? PERIOD_PREVIOUS : PERIOD_CURRENT;
    call(0x6FDCF1, {functor + 8, uint32_t(next), 1});
}
RW_FUNCTION(0x702052, "SceneRanking::init $_12::operator() (period)", rankingPeriodTouch);

// init $_10 {this}: Back -> the scene runs out ($_10's lambda goes on)
static void rankingBackTouch(Self::Addr functor, Self::Addr, Self::Addr typep) {
    if (ld32(typep) != TOUCH_ENDED) return;
    const uint32_t me = ld32(functor + 4);
    call(fn::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t done = newFunction(f, VT_BackOutDone, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + RUN_IN_ANIMATION, done});
    functionDestroy(done);
}
RW_FUNCTION(0x702120, "SceneRanking::init $_10::operator() (Back)", rankingBackTouch);

// $_10's lambda {this}: run out -> back to where the ranking came from (no fade)
static void rankingBackOutDone(Self::Addr functor) {
    const uint32_t me = ld32(functor + 4);
    if (ld32(sceneManager() + SM_SCENE_KIND) == SCENE_KIND_RANKING) st32(sceneManager() + SM_SCENE_KIND, SCENE_KIND_RANKING);
    const uint32_t backTo = uint32_t(host(me)->backTo);
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t layer = newFunction(f, VT_BackLayer, {backTo});
    call(g::SM_changeSceneLayer, {sm, layer, 1, 0, 0});
    functionDestroy(layer);
}
RW_FUNCTION(0x702260, "SceneRanking::init $_10 run-out lambda::operator()()", rankingBackOutDone);
