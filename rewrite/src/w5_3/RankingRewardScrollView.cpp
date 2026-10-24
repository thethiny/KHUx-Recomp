// RankingRewardScrollView: the reward list of SceneRankingReward (Rankings > Rewards). A BaseScrollView<int> over
// RankingReward master ids; each row shows the rank range ("1st" / "1st-10th") and up to ten reward icons (Medal,
// Keyblade, avatar parts, stamp, jewels...) with a balloon popup on tap. Type 3 (colosseum) uses its own layout.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace a {
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t BaseScrollViewInt_init = 0x4FA8A1;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;   // (string& out, string& out, type, id, bool)
constexpr uint32_t SceneUtil_addBackKeyCallback = 0x478BA9;  // (Node*, function<void()>)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;              // vector::at failure (does not return)
constexpr uint32_t TextData_format = 0x492929;              // (string out, type, id, ...)
constexpr uint32_t RankingReward_create = 0x78ADB9;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t IntArray10_at = 0x561179;                 // std::array<int, 10>::at
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;         // (const string&, TextureResType)
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t AvatarParts_create = 0x778DE1;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t FunctionBTE_swap = 0x483E4D;             // function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVTE_swap = 0x4F3699;             // function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionVTE_assignLambda = 0x561335;     // sub_561334: function<void(Touch*, Event*)> = lambda
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t initColosseumRewardLayout = 0x560019, initLayout = 0x560A09, updateSelectItems = 0x560889;
constexpr uint32_t createIncentivePanel = 0x55E295, getLayoutPanel = 0x55E3AD, setIncentiveItem = 0x55E4D1;
}  // namespace a

constexpr uint32_t SIZE = 0x678;
constexpr uint32_t VTABLE = 0xF89AD8, VTABLE2 = 0xF89E34, VTABLE3 = 0xF89E48;
// fields (after BaseScrollView<int>)
constexpr uint32_t F_FUNCTION = 0x638;        // std::function (callable pointer at +0x648)
constexpr uint32_t F_TYPE = 0x650;            // RankingRewardScrollViewType (3 = colosseum)
constexpr uint32_t F_PANEL = 0x654;           // ui::Widget* row template (retained)
constexpr uint32_t F_TOUCH = 0x658;           // rows take touches
constexpr uint32_t F_SELECTED = 0x668;        // std::vector<int>: highlighted reward ids
// strings
constexpr uint32_t S_RewardPanel = 0xDFEED6, S_RewardPanel2 = 0xDFEF02, S_Area = 0xDF9378, S_IconPanel = 0xDFEC2D;
constexpr uint32_t S_ItemPanelD = 0xDFD4B4, S_Panel = 0xDFD420, S_PanelEff = 0xDFE94F, S_Incentive = 0xDFEF2F;
constexpr uint32_t S_Medal = 0xDFD05F, S_KB = 0xDFEF39, S_IconBase = 0xDFEB70, S_IconBaseL = 0xDFEB7A;
constexpr uint32_t S_IconAvatar = 0xDFEBAB, S_Stamp = 0xDFED30, S_StarArea = 0xDF8C22, S_BalloonR = 0xDFEF3C;
constexpr uint32_t S_Balloon = 0xDFECF0, S_BalloonL = 0xDFEF4B, S_Base = 0xDFEF5A, S_BaseButton = 0xDFEF5F;
constexpr uint32_t S_BaseBut = 0xDFEF77, S_TxtNum1 = 0xDFF0BE, S_PercentSS = 0xDF8BBA, S_ClearMark = 0xDFF0CD;
constexpr uint32_t S_TxtNum = 0xDFD8DC, S_Empty = 0xDF8A1A, S_TimesD = 0xDFF0B8;
// lambda vtables
constexpr uint32_t VT_ICON_TOUCH = 0xF58D58;   // heap 0x3C: widget, balloon, type, ?, id, string, string, ?, pos, bool
constexpr uint32_t VT_BACK_KEY = 0xF58D2C;     // (widget)
constexpr uint32_t VT_ROW_BEGAN = 0xF58D00;    // (this, index)
constexpr uint32_t VT_ROW_ENDED = 0xF58CD4;    // heap 0x1C: (this, index) x 3

void showNode(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
void sbRead(uint32_t sb, uint32_t dst, uint32_t offset, uint32_t len) {
    rw::call(a::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, len, offset});
}
// dst = a copy of the std::string at src (libc++: a long string gets its own buffer)
void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        strInit(dst, ld32(src + 8), ld32(src + 4));
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}
}  // namespace

struct RankingRewardScrollView {
    static uint32_t create(int32_t type);
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self, int32_t type);
    static void createIncentivePanel(uint32_t area, int32_t count);
    static uint32_t getLayoutPanel(uint32_t self, uint32_t unused);
    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t flag);
    static uint32_t changeRow(uint32_t self, uint32_t widget, uint32_t index, uint32_t flag);
    static void updateSelectItems(uint32_t self);
    static void initLayout(uint32_t widget, uint32_t id, uint32_t flag);
    static void setIncentiveItem(uint32_t widget, uint32_t type, uint32_t sub, uint32_t id, uint32_t image,
                                 uint32_t image2, uint32_t count, int32_t pos, uint32_t flag);
    static void registItemTouchEvent(uint32_t self, uint32_t widget, uint32_t index);
    static uint32_t cloneIconTouch(uint32_t lambda);
    static void destroyDeallocateIconTouch(uint32_t lambda);
};

uint32_t RankingRewardScrollView::create(int32_t type) {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(me);
    if (vcall(me, 0x350, {me, uint32_t(type)}) == 1) {       // init
        call(a::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

uint32_t RankingRewardScrollView::construct(uint32_t self) {
    auto *base = rw::host_ptr<BaseScrollView>(self);
    call(a::Layout_ctor, {self});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    st32(self, VTABLE);
    st32(self + 0x280, VTABLE2);
    st32(self + 0x350, VTABLE3);
    st32(self + F_FUNCTION + 0x10, 0);
    st8(self + F_TOUCH, 0);
    st8(self + 0x659, 0);
    st32(self + 0x65C, 0);
    st8(self + 0x660, 0);
    for (uint32_t k : {0x670u, 0x66Cu, 0x668u, 0x664u}) st32(self + k, 0);
    return self;
}

uint32_t RankingRewardScrollView::destruct(uint32_t self) {
    st32(self, VTABLE);
    st32(self + 0x280, VTABLE2);
    st32(self + 0x350, VTABLE3);
    if (ld32(self + F_PANEL)) call(a::Ref_release, {ld32(self + F_PANEL)});
    call(a::VectorBaseInt_dtor, {self + F_SELECTED});
    functionDestroy(self + F_FUNCTION);
    st32(self, BaseScrollView::VTABLE_INT);
    st32(self + 0x280, BaseScrollView::VTABLE2_INT);
    st32(self + 0x350, BaseScrollView::VTABLE3_INT);
    call(a::VectorBaseInt_dtor, {self + 0x628});
    call(a::IScrollProtocol_dtor, {self});
    return self;
}

// type 3 (colosseum): 945 x 120 rows from Ranking_RewardPanel with 4 icons; the rest 950 x 180 from
// Ranking_RewardPanel2 with 10 icons
uint32_t RankingRewardScrollView::init(uint32_t self, int32_t type) {
    st32(self + F_TYPE, uint32_t(type));
    const bool colosseum = type == 3;
    stf(self + 0x514, colosseum ? 945.0f : 950.0f);
    st32(self + 0x518, colosseum ? 0x42F00000u : 0x43340000u);
    call(a::IScrollProtocol_setEnabledScrollBar, {self, 1});
    stf(self + 0x570, ldf(self + 0x514) * 0.5f);
    stf(self + 0x574, -5.0f - ldf(self + 0x518) * 0.5f);
    st32(self + 0x57C, 0x42C80000);                          // 100
    const uint32_t panel = call(a::GUIReader_widgetFromJsonFile,
                                {call(a::GUIReader_getInstance, {}), colosseum ? S_RewardPanel : S_RewardPanel2});
    st32(self + F_PANEL, panel);
    call(a::Ref_retain, {panel});
    vcall(ld32(self + F_PANEL), 0x29C, {ld32(self + F_PANEL), 0});   // setTouchEnabled(false)
    const uint32_t area = findNode(ld32(self + F_PANEL), S_Area, 4);
    call(a::createIncentivePanel, {area, colosseum ? 4u : 10u});
    return call(a::BaseScrollViewInt_init, {self});
}

// count icon panels (Ranking_IconPanel clones) named ItemPanel1..count under the area
void RankingRewardScrollView::createIncentivePanel(uint32_t area, int32_t count) {
    const uint32_t icon = call(a::GUIReader_widgetFromJsonFile, {call(a::GUIReader_getInstance, {}), S_IconPanel});
    for (int32_t k = 1; k <= count; k++) {
        const uint32_t clone = call(a::Widget_clone, {icon});
        const uint32_t z = vcall(area, 0x1C, {area});           // getLocalZOrder
        Frame f;
        const uint32_t buf = f.alloc(0x404);
        call(a::StringBuffer_ctor, {buf, S_ItemPanelD, uint32_t(k)});
        const uint32_t s = f.alloc(12);
        strInit(s, buf, call(a::STRLEN, {buf}));
        vcall(area, 0xFC, {area, clone, z, s});                 // addChild(child, z, name)
        strFree(s);
    }
}

uint32_t RankingRewardScrollView::getLayoutPanel(uint32_t self, uint32_t unused) {
    (void)unused;
    const uint32_t w = call(a::Widget_create, {});
    const uint32_t clone = call(a::Widget_clone, {ld32(self + F_PANEL)});
    vcall(w, node::setAnchorPoint, {w, VEC2_ANCHOR_MIDDLE});
    vcall(w, node::addChild, {w, clone});
    Frame f;
    const uint32_t size = f.alloc(8);
    call(a::Size_ctor, {size, ld32(self + 0x514), ld32(self + 0x518)});
    vcall(w, node::setContentSize, {w, size});
    vcall(w, node::setPositionX, {w, fbits(ldf(self + 0x514) * 0.5f)});
    return w;
}

uint32_t RankingRewardScrollView::createRow(uint32_t self, uint32_t index, uint32_t flag) {
    if ((ld32(self + 0x62C) - ld32(self + 0x628)) >> 2 <= index) call(a::ThrowOutOfRange, {});
    const uint32_t row = call(a::getLayoutPanel, {self, 0x628});
    vcall(self, 0x340, {self, row, index, flag});              // isReplaceItem / changeRow
    return row;
}

uint32_t RankingRewardScrollView::changeRow(uint32_t self, uint32_t widget, uint32_t index, uint32_t flag) {
    (void)flag;
    const uint32_t ids = ld32(self + 0x628);
    if ((ld32(self + 0x62C) - ids) >> 2 <= index) call(a::ThrowOutOfRange, {});
    if (!findNode(widget, S_Panel, 5)) return 1;
    switch (ld32(self + F_TYPE)) {
    case 0: case 1: case 2: call(a::initLayout, {widget, ld32(ids + 4 * index), 0}); break;
    case 3: call(a::initColosseumRewardLayout, {ids + 4 * index, widget, ld32(ids + 4 * index), 0}); break;
    default: break;
    }
    call(a::updateSelectItems, {self});
    return 0;
}

// the rows' "Panel_Eff" highlight: on when the row's id is in the selected list
void RankingRewardScrollView::updateSelectItems(uint32_t self) {
    if (ld32(self + 0x62C) == ld32(self + 0x628)) return;
    if (!(ldf(self + 0x528) > 0.0f)) return;
    int32_t k = 0;
    do {
        const uint32_t begin = ld32(self + F_SELECTED), end = ld32(self + F_SELECTED + 4);
        uint32_t on = 0;
        if (end != begin) {
            const uint32_t id = ld32(uint32_t(k + int32_t(ld32(self + 0x544))) * 4 + ld32(self + 0x628) - 4);
            const uint32_t n = uint32_t(int32_t(end - begin) >> 2);
            uint32_t j = 0;
            do {
                const uint32_t v = ld32(begin + 4 * j++);
                on = 1;
                if (v == id) break;
                on = 0;
            } while (j < n);
        }
        const uint32_t row = ld32(ld32(self + 0x51C) + 4 * uint32_t(k));
        const uint32_t eff = findNode(row, S_PanelEff, 9);
        if (eff) showNode(eff, on);
        k++;
    } while (float(k) < ldf(self + 0x528));
}

// one reward row: the rank range, the cleared mark (hidden) and up to ten reward icons with their counts
void RankingRewardScrollView::initLayout(uint32_t widget, uint32_t id, uint32_t flag) {
    Frame f;
    const uint32_t reward = f.alloc(8);
    call(a::RankingReward_create, {reward, id});
    if (!ld32(reward)) {
        showNode(widget, 0);
        vcall(widget, 0x29C, {widget, 0});                        // setTouchEnabled(false)
        sharedRelease(reward);
        return;
    }
    const uint32_t sb = ld32(reward);
    const uint32_t label = findNode(widget, S_TxtNum1, 0xE);
    const uint32_t v = f.alloc(4);
    sbRead(sb, v, 4, 4);
    const uint32_t from = ld32(v);
    sbRead(sb, v, 8, 4);
    const uint32_t to = ld32(v);
    {
        Frame g;
        if (from == to) {
            const uint32_t s = g.alloc(12);
            call(a::TextData_format, {s, 0, 100221005, from});
            setTextC(label, strData(s));
            strFree(s);
        } else {
            const uint32_t s1 = g.alloc(12), s2 = g.alloc(12);
            call(a::TextData_format, {s1, 0, 100221006, from});
            call(a::TextData_format, {s2, 0, 100221005, to});
            const uint32_t buf = g.alloc(0x404);
            call(a::StringBuffer_ctor, {buf, S_PercentSS, strData(s1), strData(s2)});
            setTextC(label, buf);
            strFree(s2);
            strFree(s1);
        }
    }
    call(a::SceneUtil_setShadow, {label});
    if (const uint32_t clear = findNode(widget, S_ClearMark, 9)) showNode(clear, 0);
    const uint32_t area = findNode(widget, S_Area, 4);
    const uint32_t arr = f.alloc(0x28);
    int32_t x = 0;
    for (uint32_t k = 0; k < 10; k++, x += 92) {
        Frame g;
        const uint32_t buf = g.alloc(0x404);
        call(a::StringBuffer_ctor, {buf, S_ItemPanelD, k + 1});
        const uint32_t item = findNode(area, buf, call(a::STRLEN, {buf}));
        const uint32_t txt = findNode(item, S_TxtNum, 0xD);
        const uint32_t pos = g.alloc(8);
        call(a::Vec2_ctor, {pos, fbits(float(x)), 0});
        vcall(item, node::setPosition, {item, pos});
        call(a::Vec2_dtor, {pos});
        sbRead(sb, v, 0xC, 4);
        if (int32_t(ld32(v)) <= int32_t(k)) {
            showNode(item, 0);
            showNode(txt, 0);
            continue;
        }
        showNode(item, 1);
        showNode(txt, 1);
        const uint32_t imageA = g.alloc(12), imageB = g.alloc(12);
        strInit(imageB, S_Empty, 0);
        strInit(imageA, S_Empty, 0);
        sbRead(sb, arr, 0x10, 0x28);
        const uint32_t type = ld32(call(a::IntArray10_at, {arr, k}));
        sbRead(sb, arr, 0x38, 0x28);
        const uint32_t itemId = ld32(call(a::IntArray10_at, {arr, k}));
        sbRead(sb, arr, 0x60, 0x28);
        uint32_t sub = 0;
        if (ld32(call(a::IntArray10_at, {arr, k})) != 1) {
            sbRead(sb, arr, 0x88, 0x28);
            sub = ld32(call(a::IntArray10_at, {arr, k}));
        }
        rw::call(a::SceneUtil_setItemImageData, {imageA, imageB, type, itemId, 0});
        const uint32_t argA = g.alloc(12), argB = g.alloc(12);
        {
            const uint32_t chars = strData(imageB);
            strInit(argA, chars, call(a::STRLEN, {chars}));
        }
        copyString(argB, imageA);
        sbRead(sb, arr, 0xD8, 0x28);
        const uint32_t count = ld32(call(a::IntArray10_at, {arr, k}));
        rw::call(a::setIncentiveItem, {item, type, sub, itemId, argA, argB, count, k, flag});
        strFree(argB);
        strFree(argA);
        sbRead(sb, arr, 0xD8, 0x28);
        call(a::StringBuffer_ctor, {buf, S_TimesD, ld32(call(a::IntArray10_at, {arr, k}))});
        setTextC(txt, buf);
        call(a::SceneUtil_setShadow, {txt});
        strFree(imageA);
        strFree(imageB);
    }
    sharedRelease(reward);
}

// one reward icon: the image on the widget its type uses (incentive / medal / keyblade / avatar part / stamp),
// a button that opens its balloon (left, centre or right by its position in the row) and the back key closing it
void RankingRewardScrollView::setIncentiveItem(uint32_t widget, uint32_t type, uint32_t sub, uint32_t id,
                                               uint32_t image, uint32_t image2, uint32_t count, int32_t pos,
                                               uint32_t flag) {
    const uint32_t incentive = findNode(widget, S_Incentive, 9);
    const uint32_t medal = findNode(widget, S_Medal, 5);
    const uint32_t kb = findNode(widget, S_KB, 2);
    const uint32_t iconBase = findNode(widget, S_IconBase, 9);
    const uint32_t iconBaseL = findNode(widget, S_IconBaseL, 0xB);
    const uint32_t iconAvatar = findNode(widget, S_IconAvatar, 0xB);
    const uint32_t stamp = findNode(widget, S_Stamp, 5);
    const uint32_t star = findNode(widget, S_StarArea, 9);
    showNode(star, 0);
    const uint32_t balloonR = findNode(widget, S_BalloonR, 0xE);
    const uint32_t balloon = findNode(widget, S_Balloon, 0xC);
    const uint32_t balloonL = findNode(widget, S_BalloonL, 0xE);
    showNode(balloonR, 0);
    showNode(balloon, 0);
    showNode(balloonL, 0);
    const uint32_t shown = pos < 2 ? balloonL : pos <= 7 ? balloon : balloonR;
    const uint32_t base = findNode(widget, S_Base, 4);
    uint32_t button = findNode(widget, S_BaseButton, 0x17);
    if (!button) {
        Frame f;
        const uint32_t s = newStr(f, S_BaseBut, 8);
        button = call(a::ImageButton_create, {s, 0});
        strFree(s);
        call(a::ImageButton_setTargetView, {button, base, fbits(1.0f)});
        const uint32_t name = newStr(f, S_BaseButton, 0x17);
        vcall(base, 0xFC, {base, button, 0, name});             // addChild(child, z, name)
        strFree(name);
    }
    {
        // the touch lambda: (widget, balloon, type, sub, id, image, image2, count, pos, flag)
        Frame f;
        const uint32_t s1 = f.alloc(12), s2 = f.alloc(12);
        copyString(s1, image);
        copyString(s2, image2);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x3C});
        if (obj) {
            st32(obj, VT_ICON_TOUCH);
            st32(obj + 4, widget);
            st32(obj + 8, shown);
            st32(obj + 0xC, type);
            st32(obj + 0x10, sub);
            st32(obj + 0x14, id);
            for (uint32_t w = 0; w < 12; w += 4) {
                st32(obj + 0x18 + w, ld32(s1 + w));
                st32(s1 + w, 0);
            }
            for (uint32_t w = 0; w < 12; w += 4) {
                st32(obj + 0x24 + w, ld32(s2 + w));
                st32(s2 + w, 0);
            }
            st32(obj + 0x30, count);
            st32(obj + 0x34, uint32_t(pos));
            st8(obj + 0x38, uint8_t(flag));
        }
        st32(fo + 16, obj);
        call(a::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        strFree(s2);
        strFree(s1);
        const uint32_t back = newFunction(f, VT_BACK_KEY, {widget});
        call(a::SceneUtil_addBackKeyCallback, {widget, back});
        functionDestroy(back);
    }
    const auto hideAllBut = [&](std::initializer_list<uint32_t> hidden) { for (uint32_t n : hidden) showNode(n, 0); };
    switch (int32_t(type)) {
    case 3:
        showNode(incentive, 0);
        call(a::ImageView_loadTexture, {medal, image, 0});
        showNode(medal, 1);
        hideAllBut({kb, iconBase, iconBaseL, iconAvatar, stamp, star});
        break;
    case 13:
        showNode(incentive, 0);
        showNode(medal, 0);
        call(a::ImageView_loadTexture, {kb, image, 0});
        showNode(kb, 1);
        hideAllBut({iconBase, iconBaseL, iconAvatar, stamp});
        break;
    case 18: case 19: case 20: case 23: {
        Frame f;
        const uint32_t parts = f.alloc(8), rarity = f.alloc(4);
        call(a::AvatarParts_create, {parts, id});
        sbRead(ld32(parts), rarity, 0x8C, 4);
        const uint32_t r = ld32(rarity);
        showNode(incentive, 0);
        showNode(medal, 0);
        showNode(kb, 0);
        showNode(iconBase, r == 1 ? 1u : 0u);
        showNode(iconBaseL, r == 2 ? 1u : 0u);
        showNode(iconAvatar, 1);
        call(a::ImageView_loadTexture, {iconAvatar, image, 0});
        showNode(stamp, 0);
        sharedRelease(parts);
        break;
    }
    case 24:
        hideAllBut({incentive, medal, kb, iconAvatar, iconBase, iconBaseL});
        showNode(stamp, 1);
        call(a::ImageView_loadTexture, {stamp, image, 0});
        break;
    default:
        call(a::ImageView_loadTexture, {incentive, image, 0});
        showNode(incentive, 1);
        hideAllBut({medal, kb, iconBase, iconBaseL, iconAvatar, stamp});
        break;
    }
}

// rows take touches through their own listener: began (this, index), ended / moved / cancelled lambdas
void RankingRewardScrollView::registItemTouchEvent(uint32_t self, uint32_t widget, uint32_t index) {
    (void)widget;
    if (!ld8(self + F_TOUCH)) return;
    const uint32_t dispatcher = ld32(call(a::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(a::EventListenerTouchOneByOne_create, {});
    call(a::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    const uint32_t began = newFunction(f, VT_ROW_BEGAN, {self, index});
    call(a::FunctionBTE_swap, {began, listener + 0x50});
    functionDestroy(began);
    const uint32_t captures = f.alloc(16);
    st32(captures, self);
    st32(captures + 4, index);
    st32(captures + 8, self);
    st32(captures + 12, index);
    const uint32_t ended = f.alloc(20);
    st32(ended + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, VT_ROW_ENDED);
        for (uint32_t w = 4; w < 0x1C; w += 8) {
            st32(obj + w, self);
            st32(obj + w + 4, index);
        }
    }
    st32(ended + 16, obj);
    call(a::FunctionVTE_swap, {ended, listener + 0x68});
    functionDestroy(ended);
    call(a::FunctionVTE_assignLambda, {listener + 0x80, captures});
    call(a::FunctionVTE_assignLambda, {listener + 0x98, captures});
    call(a::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self});
}

// __func<setIncentiveItem lambda>::__clone(): a heap copy (its two strings copied)
uint32_t RankingRewardScrollView::cloneIconTouch(uint32_t lambda) {
    const uint32_t obj = call(fn::operator_new, {0x3C});
    if (obj) {
        st32(obj, VT_ICON_TOUCH);
        for (uint32_t w = 4; w <= 0x14; w += 4) st32(obj + w, ld32(lambda + w));
        copyString(obj + 0x18, lambda + 0x18);
        copyString(obj + 0x24, lambda + 0x24);
        st32(obj + 0x30, ld32(lambda + 0x30));
        st32(obj + 0x34, ld32(lambda + 0x34));
        st8(obj + 0x38, ld8(lambda + 0x38));
    }
    return obj;
}

void RankingRewardScrollView::destroyDeallocateIconTouch(uint32_t lambda) {
    strFree(lambda + 0x24);
    strFree(lambda + 0x18);
    call(fn::operator_delete, {lambda});
}

RW_FUNCTION(0x55DEA8, "RankingRewardScrollView::create", RankingRewardScrollView::create);
RW_FUNCTION(0x55DF04, "RankingRewardScrollView::RankingRewardScrollView", RankingRewardScrollView::construct);
RW_FUNCTION(0x55DD24, "RankingRewardScrollView::~RankingRewardScrollView", RankingRewardScrollView::destruct);
RW_FUNCTION(0x55E070, "RankingRewardScrollView::init", RankingRewardScrollView::init);
RW_FUNCTION(0x55E294, "RankingRewardScrollView::createIncentivePanel", RankingRewardScrollView::createIncentivePanel);
RW_FUNCTION(0x55E3AC, "RankingRewardScrollView::getLayoutPanel", RankingRewardScrollView::getLayoutPanel);
RW_FUNCTION(0x55E478, "RankingRewardScrollView::createRow", RankingRewardScrollView::createRow);
RW_FUNCTION(0x55FEE0, "RankingRewardScrollView::changeRow", RankingRewardScrollView::changeRow);
RW_FUNCTION(0x560888, "RankingRewardScrollView::updateSelectItems", RankingRewardScrollView::updateSelectItems);
RW_FUNCTION(0x560A08, "RankingRewardScrollView::initLayout", RankingRewardScrollView::initLayout);
RW_FUNCTION(0x55E4D0, "RankingRewardScrollView::setIncentiveItem", RankingRewardScrollView::setIncentiveItem);
RW_FUNCTION(0x5611EC, "RankingRewardScrollView::registItemTouchEvent", RankingRewardScrollView::registItemTouchEvent);
RW_FUNCTION(0x561DD4, "RankingRewardScrollView icon touch lambda __func::__clone() (sub_561DD4)",
            RankingRewardScrollView::cloneIconTouch);
RW_FUNCTION(0x561F7C, "RankingRewardScrollView icon touch lambda __func::destroy_deallocate() (sub_561F7C)",
            RankingRewardScrollView::destroyDeallocateIconTouch);
