// DrawScrollView: the Shop's gacha list (see DrawScrollView.hpp). The popups (openRaisePopup / openKizunaPopup /
// openCheck*Popup, hasMagicStone / hasBondsPoint) and the lambda bodies are not rewritten.
#include "DrawScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = DrawScrollView;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t VectorAt_throw = 0x47CCE1;               // vector::at's out_of_range (does not return)
constexpr uint32_t String_assignString = 0x498A1D;          // std::string::operator=(const string&)
constexpr uint32_t String_assign = 0x48D811;                // std::string::assign(const char*, size_t)
constexpr uint32_t GachaVector_copy = 0x5AEBF1;             // vector<GachaList::Gacha>(const vector&)
constexpr uint32_t GachaVector_dtor = 0x4B353D;
constexpr uint32_t RaidVector_dtor = 0x4BA87D;              // __vector_base<RaidBase>::~
constexpr uint32_t ShopMap_find = 0x5B394F;                 // map<int, ShopData>::find
constexpr uint32_t ShopMap_at = 0x5AEC69;                   // map<int, ShopData>::operator[]
constexpr uint32_t ShopMap_destroy = 0x5B3993;
constexpr uint32_t GachaMap_at = 0x5AE245;                  // map<int, GachaData>::operator[]
constexpr uint32_t GachaMap_destroy = 0x5B387D;
constexpr uint32_t GachaMap_assignMulti = 0x5B3FC9;         // __tree::__assign_multi(first, last)
constexpr uint32_t TextureMap_at = 0x586C05;                // map<int, Texture2D*>::operator[]
constexpr uint32_t TextureMap_destroy = 0x588C7F;
constexpr uint32_t HandleMap_at = 0x5AF52D;                 // map<int, unsigned>::operator[]
constexpr uint32_t HandleMap_destroy = 0x5B42A7;
constexpr uint32_t IntVector_pushBackSlow = 0x4D6CC1;
constexpr uint32_t IntVector_base_dtor = 0x478961;
constexpr uint32_t WidgetVector_pushBack = 0x4F6B5D;
constexpr uint32_t DownloadManager_getInstance = 0x4DECB1;
constexpr uint32_t DownloadManager_start = 0x4DED65;        // (dm, url, const function&, long ttl, key)
constexpr uint32_t DownloadManager_cancel = 0x4DF62D;
constexpr uint32_t ImageView_loadTextureTexture = 0x8398FD;
constexpr uint32_t Rect_ctor = 0x8DF18B;                    // Rect(x, y, w, h)
constexpr uint32_t Rect_copy = 0x8DF1A9;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t TouchBoolFunction_swap = 0x483E4D;
constexpr uint32_t TouchVoidFunction_swap = 0x4F3699;
constexpr uint32_t FunctionVI_call = 0x4F321D;
constexpr uint32_t ScrollBar_setScrollPosition = 0x4F56A5;
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;        // (string out, unsigned long long)
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;  // (function out, id, function, Widget*, Widget*, Widget*, bool)
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;   // (Text*, const string& name)
constexpr uint32_t AppState_setTutorialCloseFunc = 0x4EB8D9;    // (function out, app, function, id)
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D;    // (function out, app, id)
constexpr uint32_t SceneManager_ChainOfTutorialDeckEdit = 0x613B05;
constexpr uint32_t ImageButton_create = 0x531239;
constexpr uint32_t ImageButton_setIsDisabled = 0x5318E1;
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t CustomTextView_setFontColor = 0x4F92F9;  // (view, Color3B packed in a word)
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t TextData_format = 0x492929;
}  // namespace g

constexpr uint32_t GOT_COLOR_WHITE = 0xFD6060;    // &cocos2d::Color3B::WHITE

constexpr uint32_t S_PlateJson = 0xE008DF;        // "cocostudio/publish/ShopDraw_3Plate_Plate_ver106.json"
constexpr uint32_t S_Panel1 = 0xE00821;
constexpr uint32_t S_AreaPanel1 = 0xE00815;
constexpr uint32_t S_TxtFriendPT = 0xE00828;
constexpr uint32_t S_TxtFriendPTLabel = 0xE00835;
constexpr uint32_t S_DrawButton1 = 0xE00848;
constexpr uint32_t S_DrawButton2 = 0xE00854;
constexpr uint32_t S_PlateFriendPT = 0xE00860;
constexpr uint32_t S_PlateCount = 0xE0086F;
constexpr uint32_t S_But12 = 0xE0087B;
constexpr uint32_t S_ImageButtonDraw = 0xE00881;
constexpr uint32_t S_But07 = 0xE00892;
constexpr uint32_t S_TxtCount = 0xE00898;
constexpr uint32_t S_CountLabel = 0xE008A2;
constexpr uint32_t S_TxtDrawButton2 = 0xE008AE;
constexpr uint32_t S_TxtDrawButton1 = 0xE008BE;
constexpr uint32_t S_ContinuityLabel = 0xE008CE;

constexpr int32_t TXT_FRIEND_PT = 100320002;
constexpr int32_t TXT_DRAWS_LEFT = 102600055;
constexpr int32_t TXT_FRIEND_DRAW_1 = 102600010, TXT_FRIEND_DRAW_N = 102600011;
constexpr int32_t TXT_DRAW_1 = 102602206, TXT_DRAW_N = 102602207;
constexpr int32_t TXT_LIMITED_DRAW_1 = 102601206, TXT_LIMITED_DRAW_N = 102601207;
constexpr uint32_t TUTORIAL_GACHA = 109;
constexpr uint32_t TUTORIAL_CHAIN_DECK_EDIT = 36;

constexpr uint32_t API_GACHAS = 0x284;            // vector<GachaList::Gacha>* (72-byte entries)
constexpr uint32_t API_USER_POINT = 0x8C;         // SafeBuffer*: friend points at +0x1C
constexpr uint32_t GACHA_ENTRY = 72;
constexpr uint32_t GACHA_DATA = 0x40;

// __func vtables (+8)
constexpr uint32_t VT_Draw = 0xF5C744;            // initShopTopGachaButton's lambda {this, GachaData}
constexpr uint32_t VT_TouchBegan = 0xF5C718;      // registItemTouchEvent $_0 {this, Rect, row}
constexpr uint32_t VT_TouchMoved = 0xF5C6EC;      // $_1 {this, Rect}
constexpr uint32_t VT_TouchCancelled = 0xF5C6C0;  // $_2 {this} (sub_5AF210)
constexpr uint32_t VT_Banner = 0xF5C694;          // loadShopBanner's download callback {this, row, image}

constexpr uint32_t V_INIT = 0x248;
constexpr uint32_t V_WIDGET_SET_TOUCH_ENABLED = 0x29C;
constexpr uint32_t V_REMOVE_CHILD = 0x12C;

uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
void setTouchEnabled(uint32_t node, uint32_t enabled) { vcall(node, V_WIDGET_SET_TOUCH_ENABLED, {node, enabled}); }

uint32_t treeNext(uint32_t node) {
    uint32_t r = ld32(node + 4);
    if (r) {
        node = r;
        while (ld32(node)) node = ld32(node);
        return node;
    }
    for (;;) {
        uint32_t parent = ld32(node + 8);
        if (ld32(parent) == node) return parent;
        node = parent;
    }
}

// libc++ std::string copy constructor, inline
void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}
// dst = src (std::string::assign(src.data(), src.size()))
void assignString(uint32_t dst, uint32_t src) {
    const uint32_t size = (ld8(src) & 1) ? ld32(src + 4) : uint32_t(ld8(src) >> 1);
    call(g::String_assign, {dst, strData(src), size});
}
// GachaData::operator= as inlined in init (strings assigned, words copied)
void assignGacha(uint32_t dst, uint32_t src) {
    if (dst != src) {
        assignString(dst, src);
        assignString(dst + 0xC, src + 0xC);
        for (uint32_t k = 0x18; k < 0x24; k += 4) st32(dst + k, ld32(src + k));
        assignString(dst + 0x24, src + 0x24);
    } else {
        for (uint32_t k = 0x18; k < 0x24; k += 4) st32(dst + k, ld32(src + k));
    }
    for (uint32_t k = 0x30; k < 0x40; k += 4) st32(dst + k, ld32(src + k));
}
void freeGacha(uint32_t gacha) {
    strFree(gacha + 0x24);
    strFree(gacha + 0xC);
    strFree(gacha);
}
// an empty std::map / std::set header {begin, root, size}
uint32_t newTree(Frame &f) {
    const uint32_t t = f.alloc(12);
    st32(t + 4, 0);
    st32(t + 8, 0);
    st32(t, t + 4);
    return t;
}
void resetTree(uint32_t tree) {
    st32(tree + 8, 0);
    st32(tree, tree + 4);
    st32(tree + 4, 0);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ GachaData
void Self::GachaData::construct(Addr self, Addr other) {
    copyString(self, other);
    copyString(self + 0xC, other + 0xC);
    for (uint32_t k = 0x18; k < 0x24; k += 4) st32(self + k, ld32(other + k));
    copyString(self + 0x24, other + 0x24);
    for (uint32_t k = 0x30; k < 0x40; k += 4) st32(self + k, ld32(other + k));
}
RW_FUNCTION(0x5AE2CC, "DrawScrollView::GachaData::GachaData(const GachaData&)", DrawScrollView::GachaData::construct);

// ------------------------------------------------------------------------------------------------ life cycle
Self::Addr Self::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(me));
    if (vcall(me, V_INIT, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}
RW_FUNCTION(0x68CDBC, "DrawScrollView::create", DrawScrollView::create);

Self *Self::construct(Self *self) {
    const uint32_t me = self->self();
    call(g::Layout_ctor, {me});
    initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    st32(me + 0x648, 0);
    self->field_650 = 0;
    self->field_651 = 0;
    self->gachasRoot = 0;
    self->gachasSize = 0;
    self->gachasBegin = me + 0x658;
    self->texturesRoot = 0;
    self->texturesSize = 0;
    self->texturesBegin = me + 0x664;
    self->handlesRoot = 0;
    self->handlesSize = 0;
    self->handlesBegin = me + 0x670;
    self->field_678 = 0;
    self->field_68C = 0;
    self->field_690 = 0;
    self->raids.begin = 0;
    self->raids.end = 0;
    self->raids.cap = 0;
    self->field_688 = 0;
    self->field_684 = 0;
    self->field_680 = 0;
    self->field_67C = 0;
    return self;
}
RW_FUNCTION(0x68E7EC, "DrawScrollView::DrawScrollView", DrawScrollView::construct);

Self *Self::destroy(Self *self) {
    const uint32_t me = self->self();
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    cancelShopHandle(self);
    for (uint32_t node = self->texturesBegin; node != me + 0x664; node = treeNext(node)) {
        if (ld32(node + 0x14)) call(g::Ref_release, {ld32(node + 0x14)});
    }
    call(g::TextureMap_destroy, {me + 0x660, self->texturesRoot});
    resetTree(me + 0x660);
    call(g::RaidVector_dtor, {me + 0x694});
    call(g::HandleMap_destroy, {me + 0x66C, self->handlesRoot});
    call(g::TextureMap_destroy, {me + 0x660, self->texturesRoot});
    call(g::GachaMap_destroy, {me + 0x654, self->gachasRoot});
    functionDestroy(me + 0x638);
    destroyInt(self);
    return self;
}
RW_FUNCTION(0x5AD174, "DrawScrollView::~DrawScrollView", DrawScrollView::destroy);

// Cancel the banner downloads still running.
void Self::cancelShopHandle(Self *self) {
    const uint32_t me = self->self();
    for (uint32_t node = self->handlesBegin; node != me + 0x670; node = treeNext(node))
        call(g::DownloadManager_cancel, {call(g::DownloadManager_getInstance, {}), ld32(node + 0x14)});
    call(g::HandleMap_destroy, {me + 0x66C, self->handlesRoot});
    resetTree(me + 0x66C);
}
RW_FUNCTION(0x5AD360, "DrawScrollView::cancelShopHandle", DrawScrollView::cancelShopHandle);

// 923 x 148 rows from ShopDraw_3Plate_Plate with a scroll bar; the gachas of APIManager's list, grouped by shop (map
// of maps keyed by shop then gacha id), numbered 0..n-1 into gachas; the item ids 0..n-1.
uint32_t Self::init(Self *self) {
    const uint32_t me = self->self();
    const uint32_t ok = initInt(self);
    self->itemWidth = 923.0f;
    self->itemHeight = 148.0f;
    self->offsetX = 461.5f;
    self->maxOffset = 8.0f;
    self->bouncePixel = 200.0f;
    setEnabledScrollBar(self, 1);
    self->touching = 0;
    self->position = 0.0f;
    self->index = -1;
    if (self->itemIDs.end != self->itemIDs.begin) self->itemIDs.end = self->itemIDs.begin;
    const uint32_t reader = call(fn::GUIReader_getInstance, {});
    self->layoutPanel = call(fn::GUIReader_widgetFromJsonFile, {reader, S_PlateJson});
    call(g::Ref_retain, {self->layoutPanel});
    setTouchEnabled(self->layoutPanel, 0);
    Frame f;
    const uint32_t list = f.alloc(12);
    call(g::GachaVector_copy, {list, ld32(call(fn::APIManager_getInstance, {}) + API_GACHAS)});
    const uint32_t shops = newTree(f);              // map<int, ShopData = map<int, GachaData>>
    call(g::ShopMap_destroy, {shops, 0});
    resetTree(shops);
    const uint32_t count = (ld32(list + 4) - ld32(list)) / GACHA_ENTRY;
    for (uint32_t i = 0; i < count; i++) {
        const uint32_t e = ld32(list) + GACHA_ENTRY * i;
        Frame h;
        const uint32_t gd = h.alloc(GACHA_DATA);
        for (uint32_t k = 0x24; k < 0x30; k += 4) st32(gd + k, 0);
        for (uint32_t k = 0; k < 0x18; k += 4) st32(gd + k, 0);
        call(g::String_assignString, {gd, e + 0x24});
        call(g::String_assignString, {gd + 0xC, e + 0x30});
        st32(gd + 0x18, ld32(e + 4));
        st32(gd + 0x1C, ld32(e + 0x14));
        st32(gd + 0x20, ld32(e + 0x18));
        call(g::String_assignString, {gd + 0x24, e + 0x3C});
        st32(gd + 0x30, ld32(e + 8));
        st32(gd + 0x34, ld32(e + 0x10));
        st32(gd + 0x38, ld32(e + 0x1C));
        st32(gd + 0x3C, ld32(e + 0x20));
        const uint32_t shopKey = e + 0xC;
        if (call(g::ShopMap_find, {shops, shopKey}) == shops + 4) {
            const uint32_t tmp = newTree(h);
            assignGacha(call(g::GachaMap_at, {tmp, gd + 0x18}), gd);
            const uint32_t shop = call(g::ShopMap_at, {shops, shopKey});
            if (shop != tmp) call(g::GachaMap_assignMulti, {shop, ld32(tmp), tmp + 4});
            call(g::GachaMap_destroy, {tmp, ld32(tmp + 4)});
        } else {
            const uint32_t shop = call(g::ShopMap_at, {shops, shopKey});
            assignGacha(call(g::GachaMap_at, {shop, gd + 0x18}), gd);
        }
        freeGacha(gd);
    }
    // this->gachas[0..] = the gachas shop by shop
    call(g::GachaMap_destroy, {me + 0x654, self->gachasRoot});
    resetTree(me + 0x654);
    const uint32_t key = f.alloc(4);
    st32(key, 0);
    for (uint32_t shop = ld32(shops); shop != shops + 4; shop = treeNext(shop)) {
        const uint32_t inner = shop + 0x14;
        for (uint32_t node = ld32(inner); node != inner + 4; node = treeNext(node)) {
            Frame h;
            const uint32_t copy = h.alloc(GACHA_DATA);
            GachaData::construct(copy, node + 0x14);
            assignGacha(call(g::GachaMap_at, {me + 0x654, key}), copy);
            st32(key, ld32(key) + 1);
            freeGacha(copy);
        }
    }
    call(g::ShopMap_destroy, {shops, ld32(shops + 4)});
    resetTree(shops);
    // item ids 0..n-1
    const uint32_t ids = f.alloc(12), value = f.alloc(4);
    for (uint32_t k = 0; k < 12; k += 4) st32(ids + k, 0);
    st32(value, 0);
    const int32_t n = int32_t(self->gachasSize);
    for (int32_t v = 1; v <= n; v++) {
        const uint32_t end = ld32(ids + 4);
        if (end == ld32(ids + 8)) {
            call(g::IntVector_pushBackSlow, {ids, value});
        } else {
            if (end) st32(end, uint32_t(v - 1));
            st32(ids + 4, end + 4);
        }
        st32(value, uint32_t(v));
    }
    setItemIDsInt(self, ids);
    call(g::IntVector_base_dtor, {ids});
    call(g::ShopMap_destroy, {shops, ld32(shops + 4)});
    call(g::GachaVector_dtor, {list});
    return ok;
}
RW_FUNCTION(0x5AE4CC, "DrawScrollView::init", DrawScrollView::init);

// IScrollProtocol::update, then re-lay the rows while the list stands (not touched, no velocity).
void Self::update(Self *self, float dt) {
    IScrollProtocol::update(self, dt);
    if (self->touching) return;
    if (self->velocity == 0.0f) vcall(self->self(), V_updateScroll, {self->self(), 0});
}
RW_FUNCTION(0x5AECF4, "DrawScrollView::update", DrawScrollView::update);

Self::Addr Self::createRow(Self *self, int32_t index, int32_t arg) {
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {self->self() + 0x628});
    const uint32_t row = IScrollProtocol::getLayoutPanel(self);
    vcall(self->self(), V_isReplaceItem, {self->self(), row, uint32_t(index), uint32_t(arg)});
    return row;
}
RW_FUNCTION(0x5AED2C, "DrawScrollView::createRow", DrawScrollView::createRow);

// Fill a row (1 when it has no Panel1: replace it).
uint32_t Self::changeRow(Self *self, Addr row, int32_t index, int32_t) {
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {self->self() + 0x628});
    if (!findNode(row, S_Panel1, 6)) return 1;
    setDrawFrame(self, row, index);
    return 0;
}
RW_FUNCTION(0x5AED84, "DrawScrollView::changeRow", DrawScrollView::changeRow);

// Row widget `row` shows item first + row - 1: refilled (or replaced by a new widget when changeRow asks), shown and
// centred; rows past the list are hidden off to the side.
void Self::updateRow(Self *self, int32_t row, int32_t first) {
    const uint32_t me = self->self();
    const int32_t item = first + row;
    uint32_t widget = ld32(self->items.begin + 4 * uint32_t(row));
    if (item < 1 || item > self->itemCount) {
        setVisible(widget, 0);
        vcall(widget, node::setPositionX, {widget, fbits(float(self->itemWidth) * 4.0f)});
        return;
    }
    if (vcall(me, V_isReplaceItem, {me, widget, uint32_t(item - 1), 0}) == 1) {
        if (row == 0) return;
        const uint32_t fresh = vcall(me, V_createItem, {me, uint32_t(item - 1), 0});
        call(g::WidgetVector_pushBack, {me + 0x5A8, fresh});
        call(g::WidgetVector_pushBack, {me + 0x5B4, widget});
        call(g::Ref_release, {ld32(self->items.begin + 4 * uint32_t(row))});
        st32(self->items.begin + 4 * uint32_t(row), fresh);
        call(g::Ref_retain, {fresh});
        vcall(fresh, node::setPosition, {fresh, vcall(widget, node::getPosition, {widget})});
        vcall(me, V_registItemTouchEvent, {me, fresh, uint32_t(row)});
        widget = fresh;
    }
    setVisible(widget, 1);
    vcall(widget, node::setPositionX, {widget, fbits(float(self->itemWidth) * 0.5f)});
}
RW_FUNCTION(0x5AEE58, "DrawScrollView::updateRow", DrawScrollView::updateRow);

// Scroll by `delta` (clamped to the bounce range, the position only stored when clamped), refresh the first row
// index (onIndexChanged), lay the row widgets out and refill them when the index moved, move the scroll bar.
void Self::updateScroll(Self *self, float delta) {
    const uint32_t me = self->self();
    if (!self->scrollEnabled || self->itemIDs.end == self->itemIDs.begin) return;
    const float itemPixel = IScrollProtocol::getItemPixel(self);
    const float maxPosition = IScrollProtocol::getMaxPosition(self);
    const float start = self->position;
    const float bounce = IScrollProtocol::getBouncePixel(self);
    const float moved = start + delta;
    const float low = rw::bitsf(rw::fbits(bounce) ^ 0x80000000u);
    float at = moved;
    if (moved < low) {
        self->position = low;
        at = low;
    } else {
        const float high = maxPosition + bounce;
        if (high < moved) {
            self->position = high;
            at = high;
        }
    }
    const int32_t whole = f2i(at), step = f2i(itemPixel);
    const int32_t first = self->maxColumn * (step ? whole / step : 0);
    const int32_t previous = self->index;
    self->index = first;
    const int32_t rest = step ? whole % step : 0;
    if (previous != first && ld32(me + OFF_ON_INDEX_CHANGED + 0x10))
        call(g::FunctionVI_call, {me + OFF_ON_INDEX_CHANGED, uint32_t(first + 1)});
    if (self->rowCount > 0.0f) {
        const float half = itemPixel * 0.5f;
        const float restF = float(rest);
        int32_t i = 0;
        do {
            const uint32_t widget = ld32(self->items.begin + 4 * uint32_t(i));
            const float x = rw::bitsf(vcall(widget, node::getPositionX, {widget}));
            const uint32_t size = vcall(me, node::getContentSize, {me});
            const float y = restF + (float(self->offsetY) + ((ldf(size + 4) - itemPixel * float(i - 1)) - half));
            Frame f;
            const uint32_t pos = f.alloc(8);
            call(g::Vec2_ctor, {pos, fbits(x), fbits(y)});
            vcall(widget, node::setPosition, {widget, pos});
            call(g::Vec2_dtor, {pos});
            if (previous != first) vcall(me, V_updateRow, {me, uint32_t(i), uint32_t(first)});
            i++;
        } while (float(i) < float(self->rowCount));
    }
    if (self->scrollBarEnabled) call(g::ScrollBar_setScrollPosition, {self->scrollBar, fbits(at)});
}
RW_FUNCTION(0x5AF284, "DrawScrollView::updateScroll", DrawScrollView::updateScroll);

// sub_5AF210: a touch function = lambda {view} (the cancelled / ended callback).
void Self::assignTouchCancelled(Addr function, Addr captures) {
    Frame f;
    const uint32_t cb = newFunction(f, VT_TouchCancelled, {ld32(captures)});
    call(g::TouchVoidFunction_swap, {cb, function});
    functionDestroy(cb);
}
RW_FUNCTION(0x5AF210, "DrawScrollView::registItemTouchEvent $_2 assign (sub_5AF210)", DrawScrollView::assignTouchCancelled);

// Touch listeners for a row: began / moved carry the row's rect, ended / cancelled the view.
void Self::registItemTouchEvent(Self *self, Addr row, int32_t index) {
    const uint32_t me = self->self();
    const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    const uint32_t width = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t height = ld32(vcall(row, node::getContentSize, {row}) + 4);
    Frame f;
    const uint32_t rect = f.alloc(16);
    call(g::Rect_ctor, {rect, 0, 0, width, height});
    {
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x1C});
        if (heap) {
            st32(heap, VT_TouchBegan);
            st32(heap + 4, me);
            call(g::Rect_copy, {heap + 8, rect});
            st32(heap + 0x18, uint32_t(index));
        }
        st32(fo + 16, heap);
        call(g::TouchBoolFunction_swap, {fo, listener + 0x50});
        functionDestroy(fo);
    }
    {
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_TouchMoved);
            st32(heap + 4, me);
            call(g::Rect_copy, {heap + 8, rect});
            st32(heap + 4, me);
        }
        st32(fo + 16, heap);
        call(g::TouchVoidFunction_swap, {fo, listener + 0x68});
        functionDestroy(fo);
    }
    const uint32_t captures = f.alloc(4);
    st32(captures, me);
    assignTouchCancelled(listener + 0x80, captures);
    assignTouchCancelled(listener + 0x98, captures);
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority,
         {ld32(call(fn::Director_getInstance, {}) + 0x84), listener, row});
}
RW_FUNCTION(0x5AEF88, "DrawScrollView::registItemTouchEvent", DrawScrollView::registItemTouchEvent);

// ------------------------------------------------------------------------------------------------ rows
// A row: untouchable plate, the gacha's buttons, the friend points title and count.
void Self::setDrawFrame(Self *self, Addr row, int32_t index) {
    const uint32_t me = self->self();
    setTouchEnabled(row, 0);
    setTouchEnabled(findNode(row, S_AreaPanel1, 11), 0);
    const uint32_t panel = findNode(row, S_Panel1, 6);
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, uint32_t(index));
    {
        Frame h;
        const uint32_t copy = h.alloc(GACHA_DATA);
        GachaData::construct(copy, call(g::GachaMap_at, {me + 0x654, key}));
        initShopTopGachaButton(self, panel, index, copy);
        freeGacha(copy);
    }
    titleLabel(panel, S_TxtFriendPT, 12, TXT_FRIEND_PT);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t label = findNode(panel, S_TxtFriendPTLabel, 18);
    SceneUtil::setShadow(label);
    const int32_t points = safe32(ld32(api + API_USER_POINT), 0x1C);
    const uint32_t money = f.alloc(12);
    call(g::SceneUtil_getMoneyStr, {money, 0, uint32_t(points), uint32_t(points >> 31)});
    call(fn::Text_setString, {label, money});
    strFree(money);
}
RW_FUNCTION(0x5AD41C, "DrawScrollView::setDrawFrame", DrawScrollView::setDrawFrame);

// The banner of row `index` from the texture cache, else downloaded (the texture slot reserved, the request kept
// in handles). 0 when the banner was cached, 1 when a download started.
uint32_t Self::loadShopBanner(Self *self, Addr image, int32_t index, Addr url) {
    const uint32_t me = self->self();
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, uint32_t(index));
    for (uint32_t node = self->texturesBegin; node != me + 0x664; node = treeNext(node)) {
        if (int32_t(ld32(node + 0x10)) == index) {
            if (ld32(node + 0x14)) call(g::ImageView_loadTextureTexture, {image, ld32(node + 0x14)});
            return 0;
        }
    }
    st32(call(g::TextureMap_at, {me + 0x660, key}), 0);
    const uint32_t cb = newFunction(f, VT_Banner, {me, uint32_t(index), image});
    const uint32_t handle = call(g::DownloadManager_start, {call(g::DownloadManager_getInstance, {}), url, cb, 0, 0});
    st32(call(g::HandleMap_at, {me + 0x66C, key}), handle);
    functionDestroy(cb);
    return 1;
}
RW_FUNCTION(0x5AE390, "DrawScrollView::loadShopBanner", DrawScrollView::loadShopBanner);

// The plate of gacha `index`: its banner, the draw button (But07 on DrawButton1, or But12 on DrawButton2 for
// friend point draws) with the draws left, the button text (cost and count), the tutorial on the second plate.
void Self::initShopTopGachaButton(Self *self, Addr plate, int32_t index, Addr gacha) {
    const uint32_t me = self->self();
    loadShopBanner(self, plate, index, strData(gacha + 0x24));
    const uint32_t draw1 = findNode(plate, S_DrawButton1, 11);
    const uint32_t draw2 = findNode(plate, S_DrawButton2, 11);
    const uint32_t plateFriend = findNode(plate, S_PlateFriendPT, 14);
    const int32_t type = int32_t(ld32(gacha + 0x30));
    const uint32_t plateCount = findNode(plate, S_PlateCount, 11);
    if (index == 1) {
        Frame f;
        const uint32_t app = call(fn::AppState_getInstance, {});
        const uint32_t none = newEmptyFunction(f);
        const uint32_t popup = f.alloc(20), previous = f.alloc(20);
        call(g::SceneUtil_openTutorialPopup, {popup, TUTORIAL_GACHA, none, draw1, 0, 0, 0});
        call(g::AppState_setTutorialCloseFunc, {previous, app, popup, TUTORIAL_GACHA});
        functionDestroy(previous);
        functionDestroy(popup);
        functionDestroy(none);
        const uint32_t close = f.alloc(20);
        call(g::AppState_getTutorialCloseFunc, {close, call(fn::AppState_getInstance, {}), TUTORIAL_GACHA});
        if (ld32(close + 16)) functionDestroy(close);
        else call(g::SceneManager_ChainOfTutorialDeckEdit, {call(fn::SceneManager_getInstance, {}), TUTORIAL_CHAIN_DECK_EDIT});
    }
    uint32_t button, target;
    if (type == 2) {
        Frame f;
        const uint32_t file = newStr(f, S_But12, 5);
        button = call(g::ImageButton_create, {file, 0});
        strFree(file);
        const uint32_t old = findNode(draw2, S_ImageButtonDraw, 16);
        if (old) vcall(draw2, V_REMOVE_CHILD, {draw2, old, 1});
        setVisible(plateCount, 0);
        target = draw2;
    } else {
        {
            Frame f;
            const uint32_t file = newStr(f, S_But07, 5);
            button = call(g::ImageButton_create, {file, 0});
            strFree(file);
        }
        const uint32_t old = findNode(draw1, S_ImageButtonDraw, 16);
        if (old) vcall(draw1, V_REMOVE_CHILD, {draw1, old, 1});
        target = draw1;
        if (!ld32(gacha + 0x38)) {
            call(g::ImageButton_setIsDisabled, {button, 0});
            setVisible(plateCount, 0);
        } else {
            if (ld32(gacha + 0x3C)) {
                call(g::ImageButton_setIsDisabled, {button, 0});
                setVisible(plateCount, 1);
            } else {
                call(g::ImageButton_setIsDisabled, {button, 1});
                setVisible(plateCount, 0);
            }
            const uint32_t text = findNode(plateCount, S_TxtCount, 9);
            uint32_t view;
            {
                Frame f;
                const uint32_t name = newStr(f, S_CountLabel, 11);
                view = text ? call(g::SceneUtil_textToCustomTextView, {text, name})
                            : SceneUtil::searchNodeByName(plateCount, name);
                strFree(name);
            }
            if (view) {
                Frame f;
                const uint32_t td = textData(f, TXT_DRAWS_LEFT);
                const uint32_t buf = stringBuffer(f, {textDataStr(td), ld32(gacha + 0x3C)});
                const uint32_t xml = newStr(f, buf);
                call(g::CustomTextView_setXml, {view, xml});
                strFree(xml);
                sharedRelease(td);
            } else {
                setVisible(plateCount, 0);
            }
        }
    }
    const uint32_t friendDraw = type == 2 ? 1 : 0;
    setVisible(draw2, friendDraw);
    setTouchEnabled(draw2, friendDraw);
    setVisible(draw1, friendDraw ^ 1);
    setTouchEnabled(draw1, friendDraw ^ 1);
    setVisible(plateFriend, friendDraw);
    call(g::ImageButton_setTargetImageView, {button, target});
    {
        Frame f;
        const uint32_t tag = newStr(f, S_ImageButtonDraw, 16);
        vcall(target, node::addChildZName, {target, button, 0, tag});
        strFree(tag);
    }
    st32(button + 0x2B0, me);
    {
        // the lambda {this, GachaData}: a copy on the stack moved into the heap
        Frame f;
        const uint32_t copy = f.alloc(GACHA_DATA);
        GachaData::construct(copy, gacha);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x48});
        if (heap) {
            st32(heap, VT_Draw);
            for (uint32_t s : {0x00u, 0x0Cu}) {
                for (uint32_t k = 0; k < 12; k += 4) st32(heap + 8 + s + k, ld32(copy + s + k));
                for (uint32_t k = 0; k < 12; k += 4) st32(copy + s + k, 0);
            }
            for (uint32_t k = 0x18; k < 0x24; k += 4) st32(heap + 8 + k, ld32(copy + k));
            for (uint32_t k = 0; k < 12; k += 4) st32(heap + 8 + 0x24 + k, ld32(copy + 0x24 + k));
            for (uint32_t k = 0; k < 12; k += 4) st32(copy + 0x24 + k, 0);
            for (uint32_t k = 0x30; k < 0x40; k += 4) st32(heap + 8 + k, ld32(copy + k));
            st32(heap + 4, me);
        }
        st32(fo + 16, heap);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        freeGacha(copy);
    }
    // the button text: cost and draws
    const int32_t cost = int32_t(ld32(gacha + 0x1C));
    const int32_t draws = int32_t(ld32(gacha + 0x20));
    int32_t textId, args0, args1;
    uint32_t textNode;
    if (ld32(gacha + 0x30) == 2) {
        const int32_t points = safe32(ld32(call(fn::APIManager_getInstance, {}) + API_USER_POINT), 0x1C);
        int32_t n = cost ? points / cost : 0;
        if (n > 10) n = 10;
        int32_t total = cost;
        if (n < 2) n = draws;
        else total = cost * n;
        textId = n == 1 ? TXT_FRIEND_DRAW_1 : TXT_FRIEND_DRAW_N;
        args0 = total;
        args1 = n;
        textNode = findNode(target, S_TxtDrawButton2, 15);
    } else {
        textId = draws == 1 ? TXT_DRAW_1 : TXT_DRAW_N;
        if (ld32(gacha + 0x38) && !ld32(gacha + 0x3C)) textId = draws == 1 ? TXT_LIMITED_DRAW_1 : TXT_LIMITED_DRAW_N;
        args0 = cost;
        args1 = draws;
        textNode = findNode(target, S_TxtDrawButton1, 15);
    }
    uint32_t view;
    {
        Frame f;
        const uint32_t name = newStr(f, S_ContinuityLabel, 16);
        view = textNode ? call(g::SceneUtil_textToCustomTextView, {textNode, name})
                        : SceneUtil::searchNodeByName(target, name);
        strFree(name);
    }
    if (view) {
        const uint32_t white = ld32(GOT_COLOR_WHITE);
        call(g::CustomTextView_setFontColor, {view, uint32_t(ld8(white)) | (uint32_t(ld8(white + 1)) << 8) | (uint32_t(ld8(white + 2)) << 16)});
        Frame f;
        const uint32_t text = f.alloc(12);
        call(g::TextData_format, {text, 0, uint32_t(textId), uint32_t(args0), uint32_t(args1)});
        const uint32_t xml = newStr(f, strData(text));
        call(g::CustomTextView_setXml, {view, xml});
        strFree(xml);
        strFree(text);
    } else {
        // the original sets the colour on the missing view (a null this)
        Frame f;
        const uint32_t grey = f.alloc(4);
        call(fn::Color3B_ctor, {grey, 0xA7, 0xA7, 0xA7});
        call(g::CustomTextView_setFontColor, {0, ld16(grey) | (uint32_t(ld8(grey + 2)) << 16)});
    }
}
RW_FUNCTION(0x5AD6C4, "DrawScrollView::initShopTopGachaButton", DrawScrollView::initShopTopGachaButton);
