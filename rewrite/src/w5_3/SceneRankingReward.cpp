// SceneRankingReward: Rankings > Rewards. A BaseSlideView of pages, one per ranking id (or per tier of a
// party ranking), each with the title (category, union / party rank emblem, period) and a RankingRewardScrollView
// of the RankingReward rows. Back returns to the Rankings screen (or the adventure select for the colosseum).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_init = 0x88A8A1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t ImageView_loadTexture = 0x839731;       // (image, const string&, TextureResType)
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t TextData_format = 0x492929;             // (string out, Type, id, ...)
constexpr uint32_t Date_makeLocalDate = 0x4927CD;          // (Date out, const Date*)
constexpr uint32_t Ranking_create = 0x7912CD, RankingReward_create = 0x78ADB9;
constexpr uint32_t Colosseum_create = 0x77C4E9, ColosseumStage_create = 0x77CF95;
constexpr uint32_t IntArray5_at = 0x700B5D;                // std::array<int, 5>::at
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionWidgetInt_call = 0x54A065;      // std::function<Widget*(int)>::operator()(int)
constexpr uint32_t Swap_widgetInt = 0x54B1C5;              // std::function<Widget*(int)>::swap
constexpr uint32_t Swap_page = 0x547159;                   // std::function<Widget*(Widget*, int, bool)>::swap
constexpr uint32_t Swap_float = 0x5E40BD;                  // std::function<void(float)>::swap
constexpr uint32_t Swap_void = 0x4EE86D;                   // std::function<void()>::swap
constexpr uint32_t VectorInt_copy = 0x4AF707;              // vector<int>(const vector&)
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;      // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t VectorInt_pushBackSlowMove = 0x47D709;  // vector<int>::__push_back_slow_path(int&&)
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
constexpr uint32_t APIManager_executeRaidRewardGet151101 = 0x4CC291;   // (api, const function<void(bool)>&)
constexpr uint32_t SceneRanking_callLuxRankingAPI = 0x6FB8E9;          // (function<void(bool)> by value)
constexpr uint32_t SceneRanking_callPartyRankingAPI = 0x6FBA65;
constexpr uint32_t SceneRanking_callColosseumRankingAPI = 0x6FBBE1;
constexpr uint32_t BaseSlideView_create = 0x534E79, BaseSlideView_setItemIDs = 0x5260AD;   // (view, vector by value, bool)
constexpr uint32_t BaseSlideView_setSlideEnabled = 0x526111;
constexpr uint32_t BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t RankingRewardScrollView_create = 0x55DEA9, RankingRewardScrollView_setEnabledTapItem = 0x5613C9;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D, IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;
// GOT: the class vtable, the __func vtable of a std::function<Layer*()> holding a function pointer, and
// createSceneLayer<SceneAdventureSelect>
constexpr uint32_t GOT_VTABLE = 0xFD6964, GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_ADVENTURE_SELECT = 0xFD669C;
}  // namespace g

constexpr uint32_t SIZE = 0x2B8;
// fields
constexpr uint32_t F_INOUT = 0x288;        // IInOutAnimation (second vtable)
constexpr uint32_t F_ROOT = 0x28C;         // the scene file's node
constexpr uint32_t F_SLIDE = 0x290;        // BaseSlideView of the pages
constexpr uint32_t F_LEFT = 0x294, F_CENTER = 0x298, F_RIGHT = 0x29C;
constexpr uint32_t F_TITLE = 0x2A0;        // Ranking_Reward_ver130 widget holding the title
constexpr uint32_t F_SLIDE_LOCKED = 0x2A4; // a list scroll turned the page slide off
constexpr uint32_t F_RANKING = 0x2A8;      // the ranking id shown
constexpr uint32_t F_BACK_ARG = 0x2AC;     // passed to the back lambda's scene factory
constexpr uint32_t F_CAN_BACK = 0x2B0;

// BaseSlideView members (std::function)
constexpr uint32_t SLIDE_MAKE_PAGE = 0x500, SLIDE_UPDATE_PAGE = 0x518, SLIDE_MOVE = 0x548, SLIDE_MOVED = 0x560;
// RankingRewardScrollView members (std::function)
constexpr uint32_t LIST_SCROLL = 0x5F8, LIST_SCROLL_END = 0x610;

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_OnEnter = 0xF6B068, VT_OnExit = 0xF6B03C, VT_BackKey = 0xF6B010, VT_BackButton = 0xF6AFE4;
constexpr uint32_t VT_MakePage = 0xF6AFB8;    // {}
constexpr uint32_t VT_UpdatePage = 0xF6AF8C;  // {this}: sub_7116D0
constexpr uint32_t VT_SlideMove = 0xF6AF60, VT_SlideMoved = 0xF6AF34;
constexpr uint32_t VT_LeftArrow = 0xF6AF08, VT_RightArrow = 0xF6AEDC;
constexpr uint32_t VT_ListScroll = 0xF6AEB0;  // {this}: sub_71112C
constexpr uint32_t VT_ListScrollEnd = 0xF6AE84;   // {this}: sub_711076
constexpr uint32_t VT_RunIn = 0xF6AE58;       // {this}
constexpr uint32_t VT_RunOutDone = 0xF6AE2C;  // {std::function<void()>} (heap, 0x20)
constexpr uint32_t VT_Back = 0xF6AD50;        // {this}: sub_7107A0
constexpr uint32_t VT_RaidRewardDone = 0xF6ADA8;  // {bool}
constexpr uint32_t VT_BackRanking = 0xF6AD7C; // {int}

// strings
constexpr uint32_t S_Bg = 0xE03AF2;                // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_SceneJson = 0xE069CA;         // "cocostudio/publish/RankingRewardScene_ver130.json"
constexpr uint32_t S_RewardJson = 0xE069FC;        // "cocostudio/publish/Ranking_Reward_ver130.json"
constexpr uint32_t S_RightUI = 0xDF85F7, S_LeftUI = 0xDF85F0, S_ButtonBack = 0xE03DBA, S_CenterUI = 0xE02FBA;
constexpr uint32_t S_Area = 0xDF9378, S_LeftArrow = 0xE05629, S_WinBottom = 0xE058DC, S_Empty = 0xDF8A1A;
constexpr uint32_t S_Union = 0xE065F5, S_RankTitle = 0xE066F0, S_TxtTitle = 0xDFF142, S_TxtMonth = 0xE06A2A;
constexpr uint32_t S_TxtMonth2 = 0xE06A34, S_TxtMonth3 = 0xE06A3F, S_TxtColosseum = 0xE06A4A, S_TxtParty = 0xDF8DC4;
constexpr uint32_t S_TxtClass = 0xE06A58, S_TxtPeriod = 0xE0661F;
constexpr uint32_t S_ListName = 0xE06A62;          // "rankingRewardScrollView"
constexpr uint32_t S_ShadeTop = 0xE06A7A, S_ShadeBottom = 0xE061E9, S_Win = 0xE04096, S_Plate = 0xDFFE65;
constexpr uint32_t S_FmtSS = 0xDFDAC7;             // "%s %s"

// text ids
constexpr int32_t TXT_TICKER_RAID = 100620047;
constexpr int32_t TXT_TITLE_RAID = 103300029, TXT_TITLE_PARTY = 103900042, TXT_TITLE_COLOSSEUM = 103900043;
constexpr int32_t TXT_MONTH = 103900060, TXT_MONTH2 = 103900044, TXT_MONTH3 = 103900046;
constexpr int32_t TXT_PERIOD = 103900011, TXT_PERIOD_DATES = 103901038;

// SceneManager::SelectRanking (3: the raid reward list of the adventure select)
constexpr int32_t SELECT_PARTY = 1, SELECT_COLOSSEUM = 2, SELECT_RAID = 3, SELECT_LUX = 4;
constexpr uint32_t SM_SELECT_RANKING = 0x290, SM_FLAG_308 = 0x308, SM_FLAG_30A = 0x30A, SM_FLAG_4CC = 0x4CC;
constexpr uint32_t SM_CALLBACK_548 = 0x548;
constexpr uint32_t API_COLOSSEUM_STAGE = 0x24C;    // the current colosseum stage id (via a pointer)
constexpr uint32_t API_RANKING_REWARDS = 0x2F4;    // ranking ids with a reward at +0x18
constexpr uint32_t API_USER = 0x94;                // SafeBuffer: union id at 0x30

constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_runInAnimation = 0x300;
constexpr uint32_t V_layoutSetTouchEnabled = 0x31C, V_widgetSetTouchEnabled = 0x29C;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }
int32_t selectRanking() { return int32_t(ld32(sceneManager() + SM_SELECT_RANKING)); }
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
void sbRead(uint32_t sb, uint32_t dst, uint32_t offset, uint32_t len) {
    rw::call(g::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, len, offset});
}
// the 4-byte field at `offset` of a master row (shared_ptr at `row`)
uint32_t rowInt(uint32_t row, uint32_t offset) {
    Frame f;
    uint32_t v = f.alloc(4);
    sbRead(ld32(row), v, offset, 4);
    return ld32(v);
}
// vector<int> {begin, end, cap}: push_back(value), the value taken from the slot `tmp` on the slow path
void pushInt(uint32_t vec, uint32_t tmp, uint32_t value, uint32_t slow = g::VectorInt_pushBackSlow) {
    st32(tmp, value);
    uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(slow, {vec, tmp});
        return;
    }
    uint32_t n = 0;
    if (end) {
        st32(end, value);
        n = ld32(vec + 4);
    }
    st32(vec + 4, n + 4);
}
// node->runAction(EaseSineOut(make(node, 0, 0, done)))
using MakeAnim = SceneUtil::Addr (*)(SceneUtil::Addr, float, float, SceneUtil::Addr);
void runEased(MakeAnim make, uint32_t node, uint32_t done) {
    uint32_t action = make(node, 0.0f, 0.0f, done);
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
void runEasedEmpty(MakeAnim make, uint32_t node) {
    Frame f;
    uint32_t done = newEmptyFunction(f);
    runEased(make, node, done);
    functionDestroy(done);
}
// swap the member std::function at `member` with a new lambda (the old one is destroyed)
void swapInto(uint32_t member, uint32_t swap, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, captures);
    call(swap, {fo, member});
    functionDestroy(fo);
}
// Date (local): year u16, month, day bytes
struct LocalDate {
    uint32_t addr;
    uint32_t year() const { return rw::load<uint16_t>(addr); }
    uint32_t month() const { return ld8(addr + 2); }
    uint32_t day() const { return ld8(addr + 3); }
};
// the 8-byte Date at `offset` of a master row, made local
LocalDate rowDate(Frame &f, uint32_t row, uint32_t offset) {
    uint32_t date = f.alloc(8);
    sbRead(ld32(row), date, offset, 8);
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
// image->loadTexture(path) from a path std::string built by `make`, then freed
void loadTexturePath(uint32_t image, uint32_t path) {
    Frame f;
    uint32_t s = newStr(f, strData(path));
    call(g::ImageView_loadTexture, {image, s, 0});
    strFree(s);
    strFree(path);
}
}  // namespace

struct SceneRankingReward {
    static uint32_t create();
    static void destroyDelete(uint32_t self);
    static uint32_t init(uint32_t self);
    static void initRankingTitle(uint32_t self, uint32_t widget, int32_t index);
    static void initRankingScrollVew(uint32_t self, uint32_t widget, int32_t index);
    static void runInAnimation(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static void listScrollEnd(uint32_t functor);
    static void listScroll(uint32_t functor, uint32_t delta);
    static uint32_t updatePage(uint32_t functor, uint32_t page, uint32_t index, uint32_t first);
};

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t SceneRankingReward::create() {
    uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    uint32_t vt = ld32(g::GOT_VTABLE);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    st32(me + F_BACK_ARG, 0);
    st8(me + F_CAN_BACK, 0);
    call(g::aeabi_memset, {me + F_ROOT, 0x1A, 0});
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

void SceneRankingReward::destroyDelete(uint32_t self) {
    uint32_t vt = ld32(g::GOT_VTABLE);
    st32(self, vt + 8);
    st32(self + F_INOUT, vt + 0x318);
    call(g::Layer_dtor, {self});
    call(fn::operator_delete, {self});
}

// ------------------------------------------------------------------------------------------------ init
uint32_t SceneRankingReward::init(uint32_t me) {
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
    st8(me + F_CAN_BACK, 1);
    {
        uint32_t m = sceneManager();
        uint32_t fo = newFunction(f, VT_BackKey, {me});
        call(g::SM_setOnBackKeyCallback, {m, fo});
        functionDestroy(fo);
    }
    call(g::SM_showHeader, {sceneManager()});
    {
        uint32_t m = sceneManager();
        uint32_t s = newStr(f, S_Bg, 0x1A);
        call(g::SM_changeBackground, {m, 0, s});
        strFree(s);
    }
    {
        uint32_t s = newStr(f, S_SceneJson, 0x31);
        st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
        strFree(s);
    }
    vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
    st32(me + F_RIGHT, findNode(ld32(me + F_ROOT), S_RightUI, 7));
    st32(me + F_LEFT, findNode(ld32(me + F_ROOT), S_LeftUI, 6));
    {
        uint32_t back = findNode(ld32(me + F_LEFT), S_ButtonBack, 0xB);
        SceneUtil::backBtnSetPosY(back);
        addTouch(back, VT_BackButton, {me});
    }
    uint32_t slide = call(g::BaseSlideView_create, {});
    st32(me + F_SLIDE, slide);
    vcall(slide, V_layoutSetTouchEnabled, {slide, 1});
    st32(me + F_CENTER, findNode(ld32(me + F_ROOT), S_CenterUI, 8));
    {
        uint32_t center = ld32(me + F_CENTER);
        vcall(center, node::removeAllChildren, {center});
    }
    {
        uint32_t size = f.alloc(8);
        call(fn::Director_getVisibleSize, {size, director()});
        slide = ld32(me + F_SLIDE);
        vcall(slide, node::setContentSize, {slide, size});
    }
    {
        uint32_t center = ld32(me + F_CENTER);
        vcall(center, node::addChildZ, {center, ld32(me + F_SLIDE), 0});
    }
    uint32_t title = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_RewardJson});
    st32(me + F_TITLE, title);
    {
        uint32_t area = findNode(title, S_Area, 4);
        setVisible(area, 0);
    }
    {
        uint32_t center = ld32(me + F_CENTER);
        vcall(center, node::addChildZ, {center, ld32(me + F_TITLE), 1});
    }
    swapInto(ld32(me + F_SLIDE) + SLIDE_MAKE_PAGE, g::Swap_widgetInt, VT_MakePage, {});
    st8(sceneManager() + SM_FLAG_308, 1);
    swapInto(ld32(me + F_SLIDE) + SLIDE_UPDATE_PAGE, g::Swap_page, VT_UpdatePage, {me});
    swapInto(ld32(me + F_SLIDE) + SLIDE_MOVE, g::Swap_float, VT_SlideMove, {me});
    swapInto(ld32(me + F_SLIDE) + SLIDE_MOVED, g::Swap_void, VT_SlideMoved, {me});

    // the pages: raid one page; Lux/party (4/5) the ranking's tiers (5, or 1 for a single-tier ranking) of the
    // first ranking id (the first party one for 4); else the first ranking id
    uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t tmp = f.alloc(4);
    uint32_t rewards = api() + API_RANKING_REWARDS;
    int32_t select = selectRanking();
    if (select == SELECT_RAID) {
        pushInt(ids, tmp, 0);
        st32(tmp, 1);
        uint32_t m = sceneManager();
        uint32_t td = textData(f, TXT_TICKER_RAID);
        uint32_t s = newStr(f, textDataStr(td));
        call(g::SM_changeTickerMessage, {m, s});
        strFree(s);
        sharedRelease(td);
    } else if (uint32_t(select - 4) <= 1) {
        uint32_t list = ld32(rewards);
        if (ld32(list + 0x1C) != ld32(list + 0x18)) {
            for (uint32_t k = 0;;) {
                st32(me + F_RANKING, ld32(ld32(list + 0x18) + 4 * k));
                Frame r;
                uint32_t ranking = masterRow(r, g::Ranking_create, ld32(me + F_RANKING));
                uint32_t kind = rowInt(ranking, 4);
                bool stop = kind == 1 && selectRanking() == SELECT_LUX;
                sharedRelease(ranking);
                if (stop) break;
                list = ld32(rewards);
                k++;
                if (k >= (ld32(list + 0x1C) - ld32(list + 0x18)) >> 2) break;
            }
        }
        uint32_t ranking = masterRow(f, g::Ranking_create, ld32(me + F_RANKING));
        int32_t count = rowInt(ranking, 4) == 1 ? 1 : 5;
        st32(tmp, 0);
        for (int32_t k = 1;; k++) {
            uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(g::VectorInt_pushBackSlow, {ids, tmp});
            } else {
                uint32_t n = 0;
                if (end) {
                    st32(end, uint32_t(k - 1));
                    n = ld32(ids + 4);
                }
                st32(ids + 4, n + 4);
            }
            st32(tmp, uint32_t(k));
            if (!(k < count)) break;
        }
        sharedRelease(ranking);
    } else {
        uint32_t list = ld32(rewards);
        if (ld32(list + 0x1C) != ld32(list + 0x18)) st32(me + F_RANKING, ld32(ld32(list + 0x18)));
        pushInt(ids, tmp, 0);
    }
    {
        uint32_t view = ld32(me + F_SLIDE);
        uint32_t copy = f.alloc(12);
        call(g::VectorInt_copy, {copy, ids});
        call(g::BaseSlideView_setItemIDs, {view, copy, 1});
        call(g::VectorBaseInt_dtor, {copy});
    }
    uint32_t pages = (ld32(ids + 4) - ld32(ids)) >> 2;
    if (pages >= 2) {
        // the arrows: an invisible ImageButton over each side's first child
        const auto arrow = [&](uint32_t target, uint32_t vtable) {
            const uint32_t child = ld32(ld32(vcall(target, node::getChildren, {target})));
            Frame a;
            const uint32_t empty = newStr(a, S_Empty, 0);
            const uint32_t button = call(g::ImageButton_create, {empty, 0});
            strFree(empty);
            call(g::ImageButton_setTargetView, {button, child, fbits(1.0f)});
            const uint32_t anchor = a.alloc(8);
            call(g::Vec2_ctorXY, {anchor, fbits(-0.5f), fbits(-0.5f)});
            call(g::ImageButton_setTargetAnchorPoint, {button, anchor});
            call(g::Vec2_dtor, {anchor});
            vcall(child, node::addChild, {child, button});
            const uint32_t cb = newFunction(a, vtable, {me});
            call(g::ImageButton_addTouchEventListener, {button, cb});
            functionDestroy(cb);
        };
        arrow(findNode(ld32(me + F_LEFT), S_LeftArrow, 0xC), VT_LeftArrow);
        arrow(findNode(ld32(me + F_ROOT), S_RightUI, 7), VT_RightArrow);
    }
    setVisible(findNode(ld32(me + F_LEFT), S_LeftArrow, 0xC), pages > 1 ? 1 : 0);
    setVisible(findNode(ld32(me + F_ROOT), S_RightUI, 7), pages > 1 ? 1 : 0);
    {
        uint32_t bottom = findNode(ld32(me + F_CENTER), S_WinBottom, 0xA);
        if (bottom) vcall(bottom, V_widgetSetTouchEnabled, {bottom, 1});
    }
    vcall(me, V_runInAnimation, {me});
    call(g::VectorBaseInt_dtor, {ids});
    return 1;
}

// The title of page `index`: the category text (raid, party, colosseum) or, for Lux/party tiers, the union
// emblem, the party rank emblem of the tier and the month texts; then the period of the ranking (raid: of the
// colosseum stage).
void SceneRankingReward::initRankingTitle(uint32_t me, uint32_t widget, int32_t index) {
    uint32_t a = api();
    uint32_t unionImage = findNode(widget, S_Union, 5);
    uint32_t rankTitle = findNode(widget, S_RankTitle, 0xA);
    uint32_t title = findNode(widget, S_TxtTitle, 9);
    uint32_t month = findNode(widget, S_TxtMonth, 9);
    uint32_t month2 = findNode(widget, S_TxtMonth2, 0xA);
    uint32_t month3 = findNode(widget, S_TxtMonth3, 0xA);
    uint32_t colosseum = findNode(widget, S_TxtColosseum, 0xD);
    uint32_t party = findNode(widget, S_TxtParty, 9);
    uint32_t klass = findNode(widget, S_TxtClass, 9);
    uint32_t period = findNode(widget, S_TxtPeriod, 0xA);
    for (uint32_t n : {unionImage, rankTitle, title, month, month2, month3, colosseum, party, klass, period})
        setVisible(n, 0);
    Frame f;
    int32_t select = selectRanking();
    if (select == SELECT_RAID) {
        setVisible(title, 1);
        SceneUtil::setShadow(title);
        setTextId(title, TXT_TITLE_RAID);
        setVisible(period, 1);
        uint32_t stage = masterRow(f, g::ColosseumStage_create, ld32(ld32(api() + API_COLOSSEUM_STAGE)));
        LocalDate from = rowDate(f, stage, 4);
        LocalDate to = rowDate(f, stage, 0xC);
        SceneUtil::setShadow(period);
        setPeriodText(period, from, to);
        sharedRelease(stage);
        return;
    }
    select = selectRanking();
    if (uint32_t(select - 4) < 2) {
        setVisible(unionImage, 1);
        uint32_t unionId = f.alloc(4);
        sbRead(ld32(a + API_USER), unionId, 0x30, 4);
        uint32_t path = f.alloc(12);
        SceneUtil::createUnionImagePath(path, int32_t(ld32(unionId)), 0, 0);
        loadTexturePath(unionImage, path);
        setVisible(rankTitle, 1);
        path = f.alloc(12);
        SceneUtil::createPartyTitleRankImagePath(path, index + 1);
        loadTexturePath(rankTitle, path);
        setVisible(month, 1);
        setTextId(month, TXT_MONTH);
        setVisible(month2, 1);
        setTextId(month2, TXT_MONTH2);
        setVisible(month3, 1);
        setTextId(month3, TXT_MONTH3);
        SceneUtil::setShadow(month);
        SceneUtil::setShadow(month2);
        SceneUtil::setShadow(month3);
    } else if (select == SELECT_COLOSSEUM) {
        setVisible(title, 1);
        SceneUtil::setShadow(title);
        setTextId(title, TXT_TITLE_COLOSSEUM);
    } else if (select == SELECT_PARTY) {
        setVisible(title, 1);
        SceneUtil::setShadow(title);
        setTextId(title, TXT_TITLE_PARTY);
    }
    uint32_t list = ld32(api() + API_RANKING_REWARDS);
    if (ld32(list + 0x1C) == ld32(list + 0x18)) return;
    uint32_t ranking = masterRow(f, g::Ranking_create, ld32(me + F_RANKING));
    LocalDate from = rowDate(f, ranking, 8);
    LocalDate to = rowDate(f, ranking, 0x10);
    setVisible(period, 1);
    SceneUtil::setShadow(period);
    setPeriodText(period, from, to);
    sharedRelease(ranking);
}

// The reward list of page `index` (in the page's Area): raid: the colosseum stage's colosseums (last to first)
// with a reward; party/colosseum: the ranking's RankingReward ids; Lux: the tier's range (single tier: all).
void SceneRankingReward::initRankingScrollVew(uint32_t me, uint32_t widget, int32_t index) {
    Frame f;
    uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t area = findNode(widget, S_Area, 4);
    uint32_t tmp = f.alloc(4);
    int32_t type = 0;
    // ids `from`..`to` (inclusive) that have a RankingReward row
    const auto pushRewards = [&](int32_t from, auto to) {
        for (int32_t id = from; id <= to(); id++) {
            Frame r;
            uint32_t reward = masterRow(r, g::RankingReward_create, uint32_t(id));
            if (ld32(reward)) pushInt(ids, tmp, uint32_t(id));
            sharedRelease(reward);
        }
    };
    if (selectRanking() == SELECT_RAID) {
        uint32_t stage = masterRow(f, g::ColosseumStage_create, ld32(ld32(api() + API_COLOSSEUM_STAGE)));
        for (int32_t id = int32_t(rowInt(stage, 0x20)); id >= int32_t(rowInt(stage, 0x1C)); id--) {
            Frame r;
            uint32_t c = masterRow(r, g::Colosseum_create, uint32_t(id));
            if (int32_t(rowInt(c, 0x78)) >= 1) pushInt(ids, tmp, uint32_t(id), g::VectorInt_pushBackSlowMove);
            sharedRelease(c);
        }
        type = 3;
        sharedRelease(stage);
    } else {
        uint32_t a = api();
        int32_t select = selectRanking();
        if (select == SELECT_COLOSSEUM) type = select;
        if (select == SELECT_PARTY) type = select;
        if (type) {
            uint32_t list = ld32(a + API_RANKING_REWARDS);
            if (ld32(list + 0x1C) != ld32(list + 0x18)) {
                uint32_t ranking = masterRow(f, g::Ranking_create, ld32(me + F_RANKING));
                pushRewards(int32_t(rowInt(ranking, 0x18)), [&] { return int32_t(rowInt(ranking, 0x1C)); });
                sharedRelease(ranking);
            }
        } else {
            uint32_t ranking = masterRow(f, g::Ranking_create, ld32(me + F_RANKING));
            if (rowInt(ranking, 4) == 1) {
                pushRewards(int32_t(rowInt(ranking, 0x18)), [&] { return int32_t(rowInt(ranking, 0x1C)); });
            } else {
                // the tiers' first and last reward ids (int[5] at 0x20 and 0x34)
                uint32_t firsts = f.alloc(0x14);
                sbRead(ld32(ranking), firsts, 0x20, 0x14);
                int32_t from = int32_t(ld32(call(g::IntArray5_at, {firsts, uint32_t(index)})));
                uint32_t lasts = f.alloc(0x14);
                pushRewards(from, [&] {
                    sbRead(ld32(ranking), lasts, 0x34, 0x14);
                    return int32_t(ld32(call(g::IntArray5_at, {lasts, uint32_t(index)})));
                });
            }
            sharedRelease(ranking);
        }
    }
    uint32_t view = findNode(area, S_ListName, 0x17);
    if (!view) {
        view = call(g::RankingRewardScrollView_create, {uint32_t(type)});
        {
            Frame n;
            uint32_t s = newStr(n, S_ListName, 0x17);
            vcall(area, node::addChildZName, {area, view, 0, s});
            strFree(s);
        }
        call(g::IScrollProtocol_setMaxColumn, {view, 1});
        call(g::RankingRewardScrollView_setEnabledTapItem, {view, 0});
        vcall(view, node::setContentSize, {view, vcall(area, node::getContentSize, {area})});
    }
    call(g::BaseScrollViewInt_setItemIDs, {view, ids});
    call(g::IScrollProtocol_setIndex, {view, 0});
    call(g::IScrollProtocol_updateLayout, {view, 0});
    swapInto(view + LIST_SCROLL, g::Swap_float, VT_ListScroll, {me});
    swapInto(view + LIST_SCROLL_END, g::Swap_void, VT_ListScrollEnd, {me});
    call(g::VectorBaseInt_dtor, {ids});
}

// ------------------------------------------------------------------------------------------------ animations
void SceneRankingReward::runInAnimation(uint32_t me) {
    {
        uint32_t left = ld32(me + F_LEFT);
        vcall(left, node::setLocalZOrder, {left, 2});
    }
    Frame f;
    {
        uint32_t done = newFunction(f, VT_RunIn, {me});
        runEased(SceneUtil::createLeftInAnimation, ld32(me + F_LEFT), done);
        functionDestroy(done);
    }
    runEasedEmpty(SceneUtil::createRightInAnimation, ld32(me + F_RIGHT));
    runEasedEmpty(SceneUtil::createLeftInAnimation, ld32(me + F_CENTER));
}

// Slide everything out; `done` (std::function<void()>) runs when the left panel is gone.
void SceneRankingReward::runOutAnimationMain(uint32_t me, uint32_t done) {
    {
        uint32_t slide = ld32(me + F_SLIDE);
        vcall(slide, V_layoutSetTouchEnabled, {slide, 1});
    }
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    uint32_t left = ld32(me + F_LEFT);
    {
        Frame f;
        uint32_t captured = f.alloc(20);
        call(fn::function_vv_copy, {captured, done});
        uint32_t onDone = f.alloc(20);
        heapFunction(onDone, VT_RunOutDone, 0x20, 8, fn::function_vv_move, captured);
        runEased(SceneUtil::createLeftOutAnimation, left, onDone);
        functionDestroy(onDone);
        functionDestroy(captured);
    }
    runEasedEmpty(SceneUtil::createRightOutAnimation, ld32(me + F_RIGHT));
    runEasedEmpty(SceneUtil::createLeftOutAnimation, ld32(me + F_CENTER));
}

// ------------------------------------------------------------------------------------------------ back
void SceneRankingReward::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    uint32_t opened = call(g::SM_isOpenedPopup, {sceneManager()});
    uint32_t m = sceneManager();
    if (opened == 1) {
        call(g::SM_closeCurrentPopup, {m, 0});
        return;
    }
    st8(m + SM_FLAG_308, 0);
    st8(me + F_CAN_BACK, 0);
    Frame f;
    uint32_t fo = newFunction(f, VT_Back, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// backScene's lambda: raid: fetch the raid rewards and return to the adventure select; else ask the server for
// the ranking of the category and return to the Rankings screen.
void SceneRankingReward::backDone(uint32_t functor) {
    uint32_t me = ld32(functor + 4);
    Frame f;
    if (selectRanking() == SELECT_RAID) {
        uint32_t a = api();
        uint32_t cb = newFunction(f, VT_RaidRewardDone, {});
        st8(cb + 4, 0);
        call(g::APIManager_executeRaidRewardGet151101, {a, cb});
        functionDestroy(cb);
        uint32_t m = sceneManager();
        uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(g::GOT_CREATE_ADVENTURE_SELECT)});
        rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
        functionDestroy(layer);
        st8(sceneManager() + SM_FLAG_30A, 1);
        return;
    }
    {
        uint32_t m = sceneManager();
        uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    }
    {
        uint32_t none = newEmptyFunction(f);
        if (selectRanking() == SELECT_PARTY) call(g::SceneRanking_callPartyRankingAPI, {none});
        else if (selectRanking() == SELECT_COLOSSEUM) call(g::SceneRanking_callColosseumRankingAPI, {none});
        else call(g::SceneRanking_callLuxRankingAPI, {none});
        functionDestroy(none);
    }
    uint32_t arg = ld32(me + F_BACK_ARG);
    uint32_t m = sceneManager();
    uint32_t layer = newFunction(f, VT_BackRanking, {arg});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
    functionDestroy(layer);
}

// ------------------------------------------------------------------------------------------------ list / slide
// a list scroll ended: give the page slide back
void SceneRankingReward::listScrollEnd(uint32_t functor) {
    uint32_t me = ld32(functor + 4);
    if (!ld8(me + F_SLIDE_LOCKED)) return;
    st8(me + F_SLIDE_LOCKED, 0);
    call(g::BaseSlideView_setSlideEnabled, {ld32(me + F_SLIDE), 1});
}

// a list scroll (float&& delta): one of 3% of the screen height or more turns the page slide off
void SceneRankingReward::listScroll(uint32_t functor, uint32_t delta) {
    float d = std::fabs(ldf(delta));
    uint32_t me = ld32(functor + 4);
    float w = visibleSize().height * 0.03f;
    if (!(w > d) && !std::isnan(w) && !std::isnan(d)) {
        st8(me + F_SLIDE_LOCKED, 1);
        call(g::BaseSlideView_setSlideEnabled, {ld32(me + F_SLIDE), 0});
    }
}

// the page updater (Widget*& page, int& index, bool& first): a new page from the factory with the reward layout
// (its shades, window and plate hidden); the title for the shown page; the list of the page
uint32_t SceneRankingReward::updatePage(uint32_t functor, uint32_t pagePtr, uint32_t indexPtr, uint32_t firstPtr) {
    uint32_t first = ld8(firstPtr);
    int32_t index = int32_t(ld32(indexPtr));
    uint32_t page = ld32(pagePtr);
    uint32_t me = ld32(functor + 4);
    if (!page) {
        page = call(g::FunctionWidgetInt_call, {ld32(me + F_SLIDE) + SLIDE_MAKE_PAGE, uint32_t(index)});
        call(g::Ref_retain, {page});
        uint32_t json = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_RewardJson});
        setVisible(findNode(json, S_ShadeTop, 0xC), 0);
        setVisible(findNode(json, S_ShadeBottom, 0xF), 0);
        setVisible(findNode(json, S_Win, 3), 0);
        setVisible(findNode(json, S_WinBottom, 0xA), 0);
        setVisible(findNode(json, S_Plate, 5), 0);
        vcall(page, node::addChild, {page, json});
    }
    if (first) initRankingTitle(me, ld32(me + F_TITLE), index);
    initRankingScrollVew(me, page, index);
    return page;
}

RW_FUNCTION(0x702404, "SceneRankingReward::create", SceneRankingReward::create);
RW_FUNCTION(0x70E570, "SceneRankingReward::~SceneRankingReward (deleting)", SceneRankingReward::destroyDelete);
RW_FUNCTION(0x70E5DC, "SceneRankingReward::init", SceneRankingReward::init);
RW_FUNCTION(0x70F218, "SceneRankingReward::initRankingTitle", SceneRankingReward::initRankingTitle);
RW_FUNCTION(0x70FDC8, "SceneRankingReward::initRankingScrollVew", SceneRankingReward::initRankingScrollVew);
RW_FUNCTION(0x7102E8, "SceneRankingReward::runInAnimation", SceneRankingReward::runInAnimation);
RW_FUNCTION(0x710460, "SceneRankingReward::runOutAnimationMain", SceneRankingReward::runOutAnimationMain);
RW_FUNCTION(0x71065C, "SceneRankingReward::backScene", SceneRankingReward::backScene);
RW_FUNCTION(0x7107A0, "SceneRankingReward::backScene lambda __func::operator() (sub_7107A0)", SceneRankingReward::backDone);
RW_FUNCTION(0x711076, "SceneRankingReward list scroll end lambda __func::operator() (sub_711076)",
            SceneRankingReward::listScrollEnd);
RW_FUNCTION(0x71112C, "SceneRankingReward list scroll lambda __func::operator() (sub_71112C)", SceneRankingReward::listScroll);
RW_FUNCTION(0x7116D0, "SceneRankingReward page update lambda __func::operator() (sub_7116D0)", SceneRankingReward::updatePage);
