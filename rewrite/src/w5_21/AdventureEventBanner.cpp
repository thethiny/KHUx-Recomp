// Quests > Events / Special Quests: SceneAdventureSelect::initEventBanner(bool) - the event list panel: the
// "no event" text and the lock button (Special Quests unlocked by the player flag, $_? {this, this}), the ticker,
// the shop-campaign line ("Conpane": the running shop campaigns of APIManager+0xAC matched to their shop rows -
// name and time left, a swallowing black sprite behind, more than one rotates), then the AdvEventBannerScrollView
// (created once in the Area node, its locked / select callbacks) filled with the event indexes, and the panels'
// in animation (first open) or out animation. Also AdventurePanel::runLeftInAnimation.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15, SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;
constexpr uint32_t EaseSineOut_create = 0x860D09, Node_runAction = 0x894AED, Node_stopAllActions = 0x893815;
constexpr uint32_t Director_getVisibleSize = 0x8F0675, Director_getVisibleOrigin = 0x8F06A1;
constexpr uint32_t Sprite_create = 0x8A64DD, Color3B_ctor = 0x9062E7, Rect_ctor = 0x8DF18B;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addSceneGraph = 0x8F3649;
constexpr uint32_t FunctionBTE_swap = 0x483E4D, FunctionVV_swap = 0x4EE86D, FunctionVI_swap = 0x5428E1;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5, createTimeLeftString = 0x492B75;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t Shop_getShopIds = 0x790935, Shop_create = 0x790951, TreeInt_destroy = 0x48D9B5;
constexpr uint32_t VecString_pushSlow = 0x484465, VecDate_pushSlow = 0x482A19, VecInt_pushSlow = 0x4D6CC1;
constexpr uint32_t VecInt_dtor = 0x478961, VecEvent_copy = 0x723C99, VecEvent_dtor = 0x4B58A1;
constexpr uint32_t BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5, IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t AdvEventBanner_create = 0x5867D1, AdvEventBanner_clearLoad = 0x5869BD;
constexpr uint32_t AdvEventBanner_setSelectItem = 0x587ED9;
constexpr uint32_t changeStageInfo = 0x723089, eventBannerDisps = 0x723995, backButtonRunInAnimation = 0x726015;
constexpr uint32_t AdventurePanel_runLeftIn = 0x7264D9, AdventurePanel_runLeftOut = 0x723D11;
}  // namespace g

// SceneAdventureSelect fields
constexpr uint32_t F_SELECTED_EVENT = 0x328, F_BANNERS = 0x32C, F_PANEL_A = 0x330, F_EVENT_SELECT = 0x338;
constexpr uint32_t F_PANEL_B = 0x33C, F_LEFT_UI = 0x344;
constexpr uint32_t F_CAMPAIGN_NAMES = 0x34C;    // vector<string>
constexpr uint32_t F_CAMPAIGN_ENDS = 0x358;     // vector<Date>
constexpr uint32_t F_CAMPAIGN_SHOWN = 0x364, F_CAMPAIGN_COUNT = 0x368;
constexpr uint32_t SM_MODE = 0x28C, API_CAMPAIGNS = 0xAC, API_FLAGS_OBJ = 0x44, API_EVENTS = 0x12C;
constexpr uint32_t SHOP_CAMPAIGN = 0x12C, SHOP_NAME = 0x23A;
constexpr uint32_t BANNER_ON_LOCKED = 0x650, BANNER_ON_SELECT = 0x638;
constexpr uint32_t V_setTouchEnabled = 0x29C, V_setColor = 0x22C, V_setOpacity = 0x214, V_setTextureRect = 0x284;
// lambdas {this}
constexpr uint32_t VT_Lock = 0xF6C9D8, VT_Swallow = 0xF6C9AC, VT_Locked = 0xF6C878, VT_Select = 0xF6C7C8;
constexpr uint32_t VT_OutA = 0xF6C79C, VT_OutB = 0xF6C770;
constexpr uint32_t S_Conpane = 0xE06F74, S_TxtWording = 0xDF862F, S_TxtLimit = 0xDF9EED, S_TxtNoEvent = 0xE06F7C;
constexpr uint32_t S_ButtonLock = 0xE06F88, S_WinBottom = 0xE058DC, S_TxtLock = 0xE06F94, S_Area = 0xDF9378;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void touchable(uint32_t n, uint32_t v) { vcall(n, V_setTouchEnabled, {n, v}); }
void titleLabel2(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(g::SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}
// a std::function holding {vt, this} swapped into `dst` (`swap` of its signature), the old one destroyed
void swapIn(uint32_t swap, uint32_t dst, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(swap, {fo, dst});
    functionDestroy(fo);
}
}  // namespace

struct AdventureEventBanner {
    static void initEventBanner(uint32_t self, uint32_t first);
    static void runLeftInAnimation(uint32_t panel);
};

void AdventureEventBanner::initEventBanner(uint32_t self, uint32_t first) {
    Frame f;
    const uint32_t vs = f.alloc(8);
    call(g::Director_getVisibleSize, {vs, call(fn::Director_getInstance, {})});
    const uint32_t conpane = find(ld32(self + F_LEFT_UI), S_Conpane, 7);
    const uint32_t wording = find(ld32(self + F_LEFT_UI), S_TxtWording, 0xB);
    const uint32_t limit = find(ld32(self + F_LEFT_UI), S_TxtLimit, 9);
    const uint32_t noEvent = find(ld32(self + F_EVENT_SELECT), S_TxtNoEvent, 0xB);
    const uint32_t lock = find(ld32(self + F_LEFT_UI), S_ButtonLock, 0xB);
    {
        const uint32_t bottom = find(ld32(self + F_LEFT_UI), S_WinBottom, 0xA);
        if (bottom) touchable(bottom, 1);
    }
    titleLabel2(ld32(self + F_EVENT_SELECT), S_TxtNoEvent, 0xB, 102900026);
    titleLabel2(lock, S_TxtLock, 8, 102900039);
    visible(noEvent, 0);
    addTouch(lock, VT_Lock, {self, self});
    const uint32_t mode = ld32(sceneManager() + SM_MODE);
    {
        const uint32_t sm = sceneManager();
        Frame g2;
        const uint32_t td = textData(g2, mode == 3 ? 100620050 : 100620051);
        const uint32_t text = textDataStr(td);
        const uint32_t s = newStr(g2, text, host_strlen(text));
        call(g::SceneManager_changeTickerMessage, {sm, s});
        strFree(s);
        sharedRelease(td);
    }
    const uint32_t campaigns = ld32(call(fn::APIManager_getInstance, {}) + API_CAMPAIGNS);
    st32(self + F_CAMPAIGN_SHOWN, 0);
    st32(self + F_CAMPAIGN_COUNT, 0);
    {
        // names.clear()
        const uint32_t names = self + F_CAMPAIGN_NAMES;
        for (uint32_t e = ld32(names + 4), b = ld32(names); e != b;) {
            const uint32_t prev = e - 12;
            st32(names + 4, prev);
            if (ld8(prev) & 1) call(fn::operator_delete, {ld32(e - 4)});
            e = ld32(names + 4);
        }
        // ends.clear()
        const uint32_t ends = self + F_CAMPAIGN_ENDS;
        const uint32_t e = ld32(ends + 4), b = ld32(ends);
        if (e != b) st32(ends + 4, e + (~7u & ~(e - 8 - b)));
    }
    const bool special = mode == 3;
    if (ld32(campaigns + 4) == ld32(campaigns) || !special) {
        visible(conpane, 0);
        const uint32_t obj = ld32(call(fn::APIManager_getInstance, {}) + API_FLAGS_OBJ);
        if (obj && (ld8(obj + 0x34) & 4)) {
            visible(lock, special ? 1 : 0);
            touchable(lock, special ? 1 : 0);
        } else {
            visible(lock, 0);
            touchable(lock, 0);
        }
    } else {
        visible(lock, 0);
        touchable(lock, 0);
        const uint32_t date = f.alloc(8), ids = f.alloc(12), shop = f.alloc(8), name = f.alloc(12);
        for (uint32_t i = 0; ld32(campaigns + 4) != ld32(campaigns);) {
            const uint32_t c = ld32(campaigns) + 12 * i;
            st32(date, ld32(c + 4));
            st32(date + 4, ld32(c + 8));
            if (int32_t(call(g::Date_calculateIntervalSinceNow, {date})) >= 0) {
                call(g::Shop_getShopIds, {ids});
                for (uint32_t n = ld32(ids); n != ids + 4;) {
                    call(g::Shop_create, {shop, ld32(n + 0x10)});
                    const uint32_t row = ld32(shop);
                    const uint32_t v = f.alloc(4);
                    rw::call(g::SafeBuffer_read, {row, v, ld32(row + 0xC) + SHOP_CAMPAIGN, 4, SHOP_CAMPAIGN});
                    if (ld32(v) == ld32(c)) {
                        const uint32_t raw = f.alloc(0x84);
                        rw::call(g::SafeBuffer_read, {row, raw, ld32(row + 0xC) + SHOP_NAME, 0x81, SHOP_NAME});
                        strInit(name, raw, host_strlen(raw));
                        const uint32_t names = self + F_CAMPAIGN_NAMES;
                        const uint32_t end = ld32(names + 4);
                        if (end >= ld32(names + 8)) {
                            call(g::VecString_pushSlow, {names, name});
                        } else {
                            if (end) {
                                st32(end, ld32(name));
                                st32(end + 4, ld32(name + 4));
                                st32(end + 8, ld32(name + 8));
                                st32(name, 0);
                                st32(name + 4, 0);
                                st32(name + 8, 0);
                            }
                            st32(names + 4, ld32(names + 4) + 12);
                        }
                        strFree(name);
                        const uint32_t ends = self + F_CAMPAIGN_ENDS;
                        const uint32_t de = ld32(ends + 4);
                        if (de == ld32(ends + 8)) {
                            call(g::VecDate_pushSlow, {ends, date});
                        } else {
                            if (de) {
                                st32(de, ld32(date));
                                st32(de + 4, ld32(date + 4));
                            }
                            st32(ends + 4, ld32(ends + 4) + 8);
                        }
                        st32(self + F_CAMPAIGN_COUNT, ld32(self + F_CAMPAIGN_COUNT) + 1);
                        sharedRelease(shop);
                        break;
                    }
                    sharedRelease(shop);
                    if (ld32(n + 4)) {
                        n = ld32(n + 4);
                        while (ld32(n)) n = ld32(n);
                    } else {
                        uint32_t p = n;
                        for (;;) {
                            n = ld32(p + 8);
                            if (ld32(n) == p) break;
                            p = n;
                        }
                    }
                }
                call(g::TreeInt_destroy, {ids, ld32(ids + 4)});
            }
            ++i;
            if (!(i < ((ld32(campaigns + 4) - ld32(campaigns)) >> 2) * 0xAAAAAAABu)) break;
        }
        if (int32_t(ld32(self + F_CAMPAIGN_COUNT)) >= 1) {
            visible(conpane, 1);
            const uint32_t sprite = call(g::Sprite_create, {});
            {
                const uint32_t color = f.alloc(4);
                call(g::Color3B_ctor, {color, 0, 0, 0});
                vcall(sprite, V_setColor, {sprite, color});
            }
            vcall(sprite, V_setOpacity, {sprite, 0});
            const uint32_t origin = f.alloc(8), size = f.alloc(8);
            call(g::Director_getVisibleOrigin, {origin, call(fn::Director_getInstance, {})});
            call(g::Director_getVisibleSize, {size, call(fn::Director_getInstance, {})});
            {
                const uint32_t rect = f.alloc(16);
                call(g::Rect_ctor, {rect, fbits(480.0f), fbits(31.0f), fbits(ldf(origin) + ldf(size)), fbits(71.0f)});
                vcall(sprite, V_setTextureRect, {sprite, rect});
                call(g::Vec2_dtor, {rect});
            }
            {
                const uint32_t pos = f.alloc(8);
                call(g::Vec2_ctor, {pos, 0, 0});
                vcall(sprite, node::setPosition, {sprite, pos});
                call(g::Vec2_dtor, {pos});
            }
            const uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + 0x84);
            const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
            call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
            {
                Frame g2;
                const uint32_t fo = newFunction(g2, VT_Swallow, {sprite});
                call(g::FunctionBTE_swap, {fo, listener + 0x50});
                functionDestroy(fo);
            }
            call(g::EventDispatcher_addSceneGraph, {dispatcher, listener, sprite});
            vcall(conpane, node::addChild, {conpane, sprite});
            const uint32_t shown = ld32(self + F_CAMPAIGN_SHOWN);
            {
                const uint32_t names = self + F_CAMPAIGN_NAMES;
                if (((ld32(names + 4) - ld32(names)) >> 2) * 0xAAAAAAABu <= shown)
                    call(g::ThrowOutOfRange, {names});
                call(fn::Text_setString, {wording, ld32(names) + 12 * ld32(self + F_CAMPAIGN_SHOWN)});
            }
            {
                const uint32_t ends = self + F_CAMPAIGN_ENDS;
                const uint32_t at = ld32(self + F_CAMPAIGN_SHOWN);
                if (((ld32(ends + 4) - ld32(ends)) >> 3) <= at) call(g::ThrowOutOfRange, {ends});
                const int32_t secs = int32_t(call(g::Date_calculateIntervalSinceNow, {ld32(ends) + 8 * at}));
                Frame g2;
                const uint32_t left = g2.alloc(12);
                call(g::createTimeLeftString, {left, uint32_t(secs)});
                setTextC(limit, (ld8(left) & 1) ? ld32(left + 8) : left + 1);
                strFree(left);
            }
            call(g::SceneUtil_setShadow, {wording});
            call(g::SceneUtil_setShadow, {limit});
            if (int32_t(ld32(self + F_CAMPAIGN_COUNT)) >= 2) call(g::eventBannerDisps, {self});
            call(g::Vec2_dtor, {origin});
        } else {
            visible(conpane, 0);
            visible(lock, special ? 1 : 0);
            touchable(lock, special ? 1 : 0);
        }
    }

    // ---------------------------------------------------------------- the banner list
    if (ld32(self + F_BANNERS)) {
        call(g::AdvEventBanner_clearLoad, {ld32(self + F_BANNERS)});
    } else {
        const float shift = (ldf(vs) + -960.0f) * 0.5f;
        vcall(conpane, node::setPositionX, {conpane, fbits(-shift)});
        {
            const float x = bitsf(vcall(lock, node::getPositionX, {lock}));
            const float s2 = (ldf(vs) + -960.0f) * 0.5f;
            vcall(lock, node::setPositionX, {lock, fbits(x - s2)});
        }
        const uint32_t area = find(ld32(self + F_EVENT_SELECT), S_Area, 4);
        const uint32_t list = call(g::AdvEventBanner_create, {});
        st32(self + F_BANNERS, list);
        vcall(list, node::setContentSize, {list, vcall(area, node::getContentSize, {area})});
        vcall(area, node::addChildZ, {area, ld32(self + F_BANNERS), 0});
        swapIn(g::FunctionVV_swap, ld32(self + F_BANNERS) + BANNER_ON_LOCKED, VT_Locked, self);
        swapIn(g::FunctionVI_swap, ld32(self + F_BANNERS) + BANNER_ON_SELECT, VT_Select, self);
        if (first == 1) st32(self + F_SELECTED_EVENT, 0);
    }
    const uint32_t ids = f.alloc(12), evs = f.alloc(12), prev = f.alloc(4);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    call(g::VecEvent_copy, {evs, ld32(call(fn::APIManager_getInstance, {}) + API_EVENTS)});
    st32(prev, 0);
    if (int32_t(ld32(evs + 4) - ld32(evs)) >= 1) {
        for (int32_t k = 1;; ++k) {
            const uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(g::VecInt_pushSlow, {ids, prev});
            } else {
                if (end) st32(end, uint32_t(k - 1));
                st32(ids + 4, ld32(ids + 4) + 4);
            }
            st32(prev, uint32_t(k));
            const int32_t n = int32_t(((ld32(evs + 4) - ld32(evs)) >> 3) * 0xB6DB6DB7u);
            if (!(k < n)) break;
        }
    }
    call(g::BaseScrollViewInt_setItemIDs, {ld32(self + F_BANNERS), ids});
    call(g::AdvEventBanner_setSelectItem, {ld32(self + F_BANNERS), ld32(self + F_SELECTED_EVENT)});
    call(g::IScrollProtocol_setIndex, {ld32(self + F_BANNERS), 0});
    call(g::IScrollProtocol_updateLayout, {ld32(self + F_BANNERS), 0});
    if (ld32(ids + 4) == ld32(ids)) visible(noEvent, 1);
    if (first == 1) {
        call(g::changeStageInfo, {self});
        call(g::AdventurePanel_runLeftIn, {self + F_PANEL_A});
        call(g::AdventurePanel_runLeftIn, {self + F_PANEL_B});
        call(g::backButtonRunInAnimation, {self});
    } else {
        if (ld32(self + F_EVENT_SELECT)) visible(ld32(self + F_EVENT_SELECT), 0);
        if (ld32(self + F_LEFT_UI)) visible(ld32(self + F_LEFT_UI), 0);
        for (auto [panel, vt] : {std::pair<uint32_t, uint32_t>{F_PANEL_A, VT_OutA}, {F_PANEL_B, VT_OutB}}) {
            Frame g2;
            const uint32_t fo = newFunction(g2, vt, {self});
            call(g::AdventurePanel_runLeftOut, {self + panel, fo});
            functionDestroy(fo);
        }
    }
    call(g::VecEvent_dtor, {evs});
    call(g::VecInt_dtor, {ids});
}

// The panel (node at +8) back at its position (+0) and slid in from the left
void AdventureEventBanner::runLeftInAnimation(uint32_t panel) {
    const uint32_t n = ld32(panel + 8);
    if (!n) return;
    call(g::Node_stopAllActions, {n});
    vcall(ld32(panel + 8), node::setPosition, {ld32(panel + 8), panel});
    const uint32_t node = ld32(panel + 8);
    Frame f;
    const uint32_t done = newEmptyFunction(f);
    const uint32_t anim = call(g::SceneUtil_createLeftInAnimation, {node, 0, 0, done});
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {anim})});
    functionDestroy(done);
}

RW_FUNCTION(0x720A5C, "SceneAdventureSelect::initEventBanner", AdventureEventBanner::initEventBanner);
RW_FUNCTION(0x7264D8, "AdventurePanel::runLeftInAnimation", AdventureEventBanner::runLeftInAnimation);
