// RaidSubdueScrollView: the raid boss card strip (see RaidSubdueScrollView.hpp).
#include "RaidSubdueScrollView.hpp"

using namespace scene;
using Self = RaidSubdueScrollView;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;                 // cocos2d::ui::Layout::Layout()
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;          // std::__vector_base<int>::~__vector_base
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;      // std::vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t VectorBaseActor_dtor = 0x485BD7;        // std::__vector_base<AvatarActor*>::~__vector_base
constexpr uint32_t SubdueTree_destroy = 0x5BAB11;          // __tree<int, SubdueData>::destroy(node)
constexpr uint32_t SubdueMap_at = 0x5B5721;                // std::map<int, SubdueData>::operator[](const int&)
constexpr uint32_t String_assign = 0x498A1D;               // std::string::operator=(const string&) (sub_498A1C)
constexpr uint32_t Texture2D_getContentSize = 0x914ACB;    // (Size* out, Texture2D*)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;                   // Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B;                   // Rect::Rect(x, y, w, h)
constexpr uint32_t Rect_copy = 0x8DF1A9;                   // Rect::Rect(const Rect&)
constexpr uint32_t AvatarIconCache_makeShared = 0x509899;  // make_shared<AvatarIconCache>(Texture2D*&, Vec2, float, float)
constexpr uint32_t AvatarIconCache_find = 0x5CA9A9;        // AvatarIconCache::find(unsigned long long)
constexpr uint32_t AvatarIconCache_add = 0x5CAA57;         // AvatarIconCache::add(unsigned long long, const Style&)
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;        // (Style* out, const UserAvatar::Coordinate&, int)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;             // SafeBuffer::~SafeBuffer
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;         // (Text*)
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;       // (string* out, unsigned long long)
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41;   // (string* out, int)
constexpr uint32_t SceneUtil_setAdditionalKerning = 0x466BA9;  // (TextAtlas*, int)
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t ImageView_loadTexture = 0x839731;       // (image, const string&, TextureResType)
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;      // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;      // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t SQRT = 0x415208, FABS = 0x415214;       // libm (double)
constexpr uint32_t DIVSI3 = 0xCD5E8D, MODSI3 = 0xCD6009;
}  // namespace g

// strings
constexpr uint32_t S_EntrantJson = 0xE00A80;     // "cocostudio/publish/Adv_Raid_Boss_Dialog_Entrant_ver104.json"
constexpr uint32_t S_DummyDiscovery = 0xE009F1;  // "Dummy_Discovery"
constexpr uint32_t S_DummyMvp = 0xE00A01;        // "Dummy_Mvp"
constexpr uint32_t S_DummyMvp2 = 0xE00A0B;       // "Dummy_Mvp2"
constexpr uint32_t S_DummyEntrant = 0xE00A16;    // "Dummy_Entrant"
constexpr uint32_t S_AtlasNumL = 0xDFD2A4;       // "AtlasLabel_NumL"
constexpr uint32_t S_Panel28 = 0xE00ABC;         // "Panel_28"
constexpr uint32_t S_DiscoveryIcon = 0xDFD25E;   // "Raid_Boss_Discovery_Icon"
constexpr uint32_t S_BossIcon = 0xDFD2D3;        // "Raid_Boss_Icon"
constexpr uint32_t S_RarePop = 0xDFD318;         // "Raid_Rare_Pop"
constexpr uint32_t S_PanelName = 0xE00A24;       // "Panel_Name"
constexpr uint32_t S_NameLabel = 0xDFDBB8;       // "Txt_Name_Label"
constexpr uint32_t S_SignDmg = 0xE00A2F;         // "Txt_Sign_DMG"
constexpr uint32_t S_Dmg = 0xE00A3C;             // "Txt_DMG"
constexpr uint32_t S_AreaPoint = 0xDF9FB4;       // "Area_Point"
constexpr uint32_t S_EntrantTxt = 0xE00A56;      // "Raid_Boss_Entrant_Txt"
constexpr uint32_t S_DiscoveryTxt = 0xDFD277;    // "Raid_Boss_Discovery_Txt"
constexpr uint32_t S_MvpTxt = 0xE00A44;          // "Raid_Boss_Mvp_Txt"
constexpr uint32_t S_BossImage = 0xDFD2E2;       // "img/adventure/boss_L/raid_boss_%d.png"
constexpr uint32_t S_MvpIconObj = 0xE00A6C;      // "Avatar_Mvp_Icon_Obj"

// text ids
constexpr int32_t TXT_DAMAGE = 103200021;
constexpr int32_t TXT_DISCOVERY = 103200008;
constexpr int32_t TXT_MVP = 103200020;
constexpr int32_t TXT_ENTRANT = 103200034;

// __func vtables (+8) of registItemTouchEvent's lambdas
constexpr uint32_t VT_TouchBegan = 0xF5C9AC;     // $_0 {view, Rect, row}
constexpr uint32_t VT_TouchMoved = 0xF5C980;     // $_1 {view, Rect}

// node virtual slots (byte offsets)
constexpr uint32_t V_init = 0x248;
constexpr uint32_t V_deleting = 0x04;
constexpr uint32_t V_getLocalZOrder = 0x18;
constexpr uint32_t V_setPosition = 0x4C;
constexpr uint32_t V_getPosition = 0x54;
constexpr uint32_t V_getPositionX = 0x68;
constexpr uint32_t V_getPositionY = 0x70;
constexpr uint32_t V_addChildZName = 0xFC;
constexpr uint32_t V_removeFromParent = 0x124;
constexpr uint32_t V_removeAllChildren = 0x138;
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t V_getVirtualRenderer = 0x2D8;
constexpr uint32_t V_getTexture = 0x280;
constexpr uint32_t V_changeRow = 0x340;          // IScrollProtocol's isReplaceItem slot

inline uint32_t fb(float f) { return rw::fbits(f); }
inline bool unord(float a, float b) { return a != a || b != b; }
inline int32_t idiv(int32_t a, int32_t b) { return int32_t(call(g::DIVSI3, {uint32_t(a), uint32_t(b)})); }
inline int32_t imod(int32_t a, int32_t b) { return int32_t(call(g::MODSI3, {uint32_t(a), uint32_t(b)})); }
inline float neg(float v) { return rw::bitsf(rw::fbits(v) ^ 0x80000000u); }
inline double callD(uint32_t fn, double v) {
    uint64_t u;
    std::memcpy(&u, &v, 8);
    uint64_t r = rw::call64(fn, {uint32_t(u), uint32_t(u >> 32)});
    double out;
    std::memcpy(&out, &r, 8);
    return out;
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, IScrollProtocol::V_setVisible, {node, visible}); }

// text->setString(SceneUtil::getMoneyStr(value)): the money string's characters copied into a temporary
void setMoneyText(uint32_t text, int32_t value) {
    Frame f;
    uint32_t money = f.alloc(12);
    call(g::SceneUtil_getMoneyStr, {money, 0, uint32_t(value), uint32_t(value >> 31)});
    uint32_t s = newStr(f, strData(money));
    call(fn::Text_setString, {text, s});
    strFree(s);
    strFree(money);
}
// label->setString(SceneUtil::numberToAtlasString(value))
void setAtlasNumber(uint32_t label, int32_t value) {
    Frame f;
    uint32_t s = f.alloc(12);
    call(g::SceneUtil_numberToAtlasString, {s, uint32_t(value)});
    call(g::TextAtlas_setString, {label, s});
    strFree(s);
}
// the card data of card `index` (std::map operator[]: inserted when missing)
uint32_t subdueData(Self *self, uint32_t key) { return call(g::SubdueMap_at, {self->self() + Self::OFF_SUBDUE, key}); }
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::construct(Self *self) {
    call(g::Layout_ctor, {self->self()});
    initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    self->bounceDurationDefault = 0.2f;
    self->autoScrollInterval = 0.3f;
    self->tapAnimating = 0;
    self->autoScrollTimer = 0.0f;
    self->autoScrollLeft = 0;
    self->autoScrolling = 0;
    self->iconCache.obj = 0;
    self->iconCache.ctrl = 0;
    self->subdueRoot = 0;
    self->subdueSize = 0;
    self->subdueBegin = rw::guest_addr(&self->subdueRoot);
    self->touchedItem = 0;
    self->field_678 = 0;
    call(0x40E7A0, {rw::guest_addr(&self->actors), 0x15, 0});   // __aeabi_memset: actors .. itemTouching
    return self;
}
RW_FUNCTION(0x5BA404, "RaidSubdueScrollView::RaidSubdueScrollView()", RaidSubdueScrollView::construct);

Self::Addr Self::create() {
    uint32_t p = call(fn::operator_new, {SIZE});
    Self *self = rw::host_ptr<Self>(p);
    construct(self);
    if (vcall(p, V_init, {p}) == 1) {
        call(g::Ref_autorelease, {p});
        return p;
    }
    vcall(p, V_deleting, {p});
    return 0;
}
RW_FUNCTION(0x5B9608, "RaidSubdueScrollView::create()", RaidSubdueScrollView::create);

Self *Self::destroy(Self *self) {
    uint32_t me = self->self();
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    call(g::VectorBaseActor_dtor, {rw::guest_addr(&self->actors)});
    call(g::SubdueTree_destroy, {me + OFF_SUBDUE, self->subdueRoot});
    if (self->iconCache.ctrl) call(fn::shared_release, {self->iconCache.ctrl});
    self->vtable = VTABLE_INT;
    self->vtable2 = VTABLE2_INT;
    self->vtable3 = VTABLE3_INT;
    call(g::VectorBaseInt_dtor, {rw::guest_addr(&self->itemIDs)});
    IScrollProtocol::destroy(self);
    return self;
}
RW_FUNCTION(0x5B42D4, "RaidSubdueScrollView::~RaidSubdueScrollView()", RaidSubdueScrollView::destroy);

void Self::destroyDelete(Self *self) {
    destroy(self);
    call(fn::operator_delete, {self->self()});
}
RW_FUNCTION(0x5B43A0, "RaidSubdueScrollView::~RaidSubdueScrollView() (deleting)", RaidSubdueScrollView::destroyDelete);

// A horizontal list of entrant cards (the card layout is the template row), no scroll bar.
uint32_t Self::init(Self *self) {
    uint32_t ok = BaseScrollView::initInt(self);
    self->bounceDuration = self->bounceDurationDefault;
    IScrollProtocol::setEnabledScrollBar(self, 0);
    self->touching = 0;
    self->position = 0.0f;
    self->index = -1;
    uint32_t e = self->itemIDs.end, b = self->itemIDs.begin;
    if (e != b) self->itemIDs.end = e + (~3u & ~(e - 4 - b));
    uint32_t reader = call(g::GUIReader_getInstance, {});
    uint32_t panel = call(g::GUIReader_widgetFromJsonFile, {reader, S_EntrantJson});
    self->layoutPanel = panel;
    call(g::Ref_retain, {panel});
    vcall(self->layoutPanel, V_setTouchEnabled, {self->layoutPanel, 0});
    self->itemWidth = ldf(vcall(self->layoutPanel, V_getContentSize, {self->layoutPanel}));
    float h = ldf(vcall(self->layoutPanel, V_getContentSize, {self->layoutPanel}) + 4);
    self->itemHeight = h;
    self->offsetX = self->itemWidth * 0.5f;
    self->offsetY = h * 0.5f;
    self->maxOffset = 8.0f;
    self->bouncePixel = 0.0f;
    self->scrollType = 1;
    return ok;
}
RW_FUNCTION(0x5B57A8, "RaidSubdueScrollView::init()", RaidSubdueScrollView::init);

// Card widths: half the view for two columns, a third otherwise.
void Self::reSetWidthAndOffsetX(Self *self, int32_t columns) {
    uint32_t me = self->self();
    float w = ldf(vcall(me, V_getContentSize, {me}));
    float panelW = ldf(vcall(self->layoutPanel, V_getContentSize, {self->layoutPanel}));
    self->itemWidth = columns == 2 ? w * 0.5f : w / 3.0f;
    self->offsetX = panelW * 0.5f;
}
RW_FUNCTION(0x5B6428, "RaidSubdueScrollView::reSetWidthAndOffsetX(int)", RaidSubdueScrollView::reSetWidthAndOffsetX);

// ------------------------------------------------------------------------------------------------ data
// Take a new card list: the map is rebuilt with keys 0..n-1, the avatar icon cache is created on first use
// (icons the size of the discovery icon's texture), and the item ids become 0..n-1.
uint32_t Self::reflectionList(Self *self, Addr list) {
    uint32_t me = self->self();
    Frame f;
    uint32_t key = f.alloc(4);
    st32(key, 0);
    call(g::SubdueTree_destroy, {me + OFF_SUBDUE, self->subdueRoot});
    self->subdueSize = 0;
    self->subdueBegin = rw::guest_addr(&self->subdueRoot);
    self->subdueRoot = 0;
    uint32_t ae = self->actors.end, ab = self->actors.begin;
    if (ae != ab) self->actors.end = ae + (~3u & ~(ae - 4 - ab));
    uint32_t p = ld32(list), end = ld32(list + 4);
    int32_t k = 1;
    for (; p != end; p += SUBDUE_SIZE) {
        uint32_t d = subdueData(self, key);
        for (uint32_t o = 0; o < 16; o += 4) st32(d + o, ld32(p + o));
        call(g::String_assign, {d + 16, p + 16});
        for (uint32_t o = 0x1C; o < 0x50; o += 4) st32(d + o, ld32(p + o));
        st32(key, uint32_t(k));
        k++;
    }
    if (!self->iconCache.obj) {
        uint32_t icon = findNode(self->layoutPanel, S_DiscoveryIcon, 24);
        uint32_t renderer = vcall(icon, V_getVirtualRenderer, {icon});
        uint32_t texture = f.alloc(4), size = f.alloc(8), pos = f.alloc(8);
        uint32_t s1 = f.alloc(4), s2 = f.alloc(4), made = f.alloc(8);
        st32(texture, vcall(renderer, V_getTexture, {renderer}));
        call(g::Texture2D_getContentSize, {size, ld32(texture)});
        call(g::Vec2_ctor, {pos, fb(neg(ldf(size) * 0.5f)), fb(40.0f)});
        stf(s1, 1.15f);
        stf(s2, 1.15f);
        call(g::AvatarIconCache_makeShared, {made, texture, pos, s1, s2});
        uint32_t obj = ld32(made), ctrl = ld32(made + 4);
        st32(made, 0);
        st32(made + 4, 0);
        self->iconCache.obj = obj;
        uint32_t old = self->iconCache.ctrl;
        self->iconCache.ctrl = ctrl;
        if (old) call(fn::shared_release, {old});
        call(g::Vec2_dtor, {pos});
    }
    // item ids 0..n-1
    uint32_t value = f.alloc(4), ids = f.alloc(12);
    st32(value, 0);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    if (ld32(list + 4) != ld32(list)) {
        uint32_t n = 1;
        for (;;) {
            uint32_t e = ld32(ids + 4);
            if (e == ld32(ids + 8)) {
                call(g::VectorInt_pushBackSlow, {ids, value});
            } else {
                uint32_t next = 0;
                if (e) {
                    st32(e, n - 1);
                    next = ld32(ids + 4);
                }
                st32(ids + 4, next + 4);
            }
            st32(value, n);
            uint32_t count = uint32_t(int32_t(ld32(list + 4) - ld32(list)) >> 4) * 0xCCCCCCCDu;
            if (!(n < count)) break;
            n++;
        }
    }
    BaseScrollView::setItemIDsInt(self, ids);
    call(g::VectorBaseInt_dtor, {ids});
    return 1;
}
RW_FUNCTION(0x5B58B4, "RaidSubdueScrollView::reflectionList(vector<SubdueData>&)", RaidSubdueScrollView::reflectionList);

// ------------------------------------------------------------------------------------------------ rows
// A card: the template panel with the four count labels drawn tighter, then filled by changeRow.
Self::Addr Self::createRow(Self *self, int32_t index, int32_t arg) {
    uint32_t panel = IScrollProtocol::getLayoutPanel(self);
    uint32_t discovery = findNode(panel, S_DummyDiscovery, 15);
    uint32_t mvp = findNode(panel, S_DummyMvp, 9);
    uint32_t mvp2 = findNode(panel, S_DummyMvp2, 10);
    uint32_t entrant = findNode(panel, S_DummyEntrant, 13);
    uint32_t n1 = findNode(discovery, S_AtlasNumL, 15);
    uint32_t n2 = findNode(mvp, S_AtlasNumL, 15);
    uint32_t n3 = findNode(mvp2, S_AtlasNumL, 15);
    uint32_t n4 = findNode(entrant, S_AtlasNumL, 15);
    call(g::SceneUtil_setAdditionalKerning, {n1, uint32_t(-21)});
    call(g::SceneUtil_setAdditionalKerning, {n2, uint32_t(-21)});
    call(g::SceneUtil_setAdditionalKerning, {n3, uint32_t(-21)});
    call(g::SceneUtil_setAdditionalKerning, {n4, uint32_t(-21)});
    uint32_t me = self->self();
    vcall(me, V_changeRow, {me, panel, uint32_t(index), uint32_t(arg)});
    return panel;
}
RW_FUNCTION(0x5B5B28, "RaidSubdueScrollView::createRow(int, int)", RaidSubdueScrollView::createRow);

// A card widget (it has "Panel_28") is redrawn in place; anything else must be replaced.
uint32_t Self::changeRow(Self *self, Addr widget, int32_t index, int32_t) {
    if (findNode(widget, S_Panel28, 8)) {
        setDrawFrame(self, widget, index);
        return 0;
    }
    return 1;
}
RW_FUNCTION(0x5B5E0C, "RaidSubdueScrollView::changeRow(Widget*, int, int)", RaidSubdueScrollView::changeRow);

// Draw card `index`: the dummy of its type is shown with the count, the damage and the player's name; a rare
// boss card only shows the boss image. Every card but the boss image gets the player's avatar icon (from the
// icon cache) in place of the MVP icon placeholder, or next to the discovery icon.
void Self::setDrawFrame(Self *self, Addr widget, int32_t index) {
    Frame f;
    uint32_t key = f.alloc(4);
    st32(key, uint32_t(index));
    uint32_t d = subdueData(self, key);
    vcall(widget, V_setTouchEnabled, {widget, 0});
    uint32_t bossIcon = findNode(widget, S_BossIcon, 14);
    uint32_t rarePop = findNode(widget, S_RarePop, 13);
    uint32_t discovery = findNode(widget, S_DummyDiscovery, 15);
    uint32_t mvp = findNode(widget, S_DummyMvp, 9);
    uint32_t mvp2 = findNode(widget, S_DummyMvp2, 10);
    uint32_t entrant = findNode(widget, S_DummyEntrant, 13);
    uint32_t panelName = findNode(widget, S_PanelName, 10);
    uint32_t nameLabel = findNode(widget, S_NameLabel, 14);
    uint32_t signDmg = findNode(widget, S_SignDmg, 12);
    uint32_t dmg = findNode(widget, S_Dmg, 7);
    uint32_t area = findNode(widget, S_AreaPoint, 10);
    vcall(area, V_removeAllChildren, {area});
    int32_t type = int32_t(ld32(d + 28));
    // the damage line: "damage" sign text and the amount
    auto damage = [&]() {
        call(g::SceneUtil_setShadow, {signDmg});
        setTextId(signDmg, TXT_DAMAGE);
        call(g::SceneUtil_setShadow, {dmg});
        setMoneyText(dmg, int32_t(ld32(d + 12)));
    };
    auto countAndTitle = [&](uint32_t dummy, uint32_t titleName, uint32_t titleLen, int32_t titleId) {
        setAtlasNumber(findNode(dummy, S_AtlasNumL, 15), int32_t(ld32(d + 4)));
        setTextId(findNode(dummy, titleName, titleLen), titleId);
    };
    switch (type) {
    case 1:   // found the boss
        setVisible(discovery, 1);
        setVisible(mvp, 0);
        setVisible(mvp2, 0);
        setVisible(entrant, 0);
        setVisible(bossIcon, 0);
        setVisible(rarePop, 0);
        setVisible(signDmg, 1);
        setVisible(dmg, 1);
        damage();
        setVisible(panelName, 1);
        countAndTitle(discovery, S_DiscoveryTxt, 23, TXT_DISCOVERY);
        break;
    case 2:   // MVP
        setVisible(discovery, 0);
        setVisible(mvp, 1);
        setVisible(mvp2, 0);
        setVisible(entrant, 0);
        setVisible(bossIcon, 0);
        setVisible(rarePop, 0);
        setVisible(signDmg, 1);
        setVisible(dmg, 1);
        setVisible(panelName, 1);
        damage();
        countAndTitle(mvp, S_MvpTxt, 17, TXT_MVP);
        break;
    case 3:   // found it and MVP
        setVisible(discovery, 0);
        setVisible(mvp, 0);
        setVisible(mvp2, 1);
        setVisible(entrant, 0);
        setVisible(bossIcon, 0);
        setVisible(rarePop, 0);
        setVisible(signDmg, 1);
        setVisible(dmg, 1);
        damage();
        setVisible(panelName, 1);
        countAndTitle(mvp2, S_MvpTxt, 17, TXT_MVP);
        setTextId(findNode(mvp2, S_DiscoveryTxt, 23), TXT_DISCOVERY);
        break;
    case 4: {   // the boss image (rare marker)
        setVisible(discovery, 0);
        setVisible(mvp, 0);
        setVisible(mvp2, 0);
        setVisible(entrant, 0);
        setVisible(bossIcon, 1);
        uint32_t buf = stringBuffer(f, {S_BossImage, ld32(d)});
        uint32_t s = newStr(f, buf);
        call(g::ImageView_loadTexture, {bossIcon, s, 0});
        strFree(s);
        setVisible(rarePop, ld8(d + 8));
        setVisible(signDmg, 0);
        setVisible(dmg, 0);
        setVisible(panelName, 0);
        break;
    }
    default:   // an entrant
        setVisible(discovery, 0);
        setVisible(mvp, 0);
        setVisible(mvp2, 0);
        setVisible(entrant, 1);
        setVisible(bossIcon, 0);
        setVisible(rarePop, 0);
        setVisible(signDmg, 1);
        setVisible(dmg, 1);
        damage();
        setVisible(panelName, 1);
        countAndTitle(entrant, S_EntrantTxt, 21, TXT_ENTRANT);
        break;
    }
    if (type != 4) call(fn::Text_setString, {nameLabel, d + 16});
    if (int32_t(ld32(d + 28)) == 4) return;
    // the player's avatar icon
    uint32_t discoveryIcon = findNode(widget, S_DiscoveryIcon, 24);
    uint32_t mvpObj = findNode(widget, S_MvpIconObj, 19);
    uint32_t lo = uint32_t(index), hi = uint32_t(index >> 31);
    uint32_t icon = call(g::AvatarIconCache_find, {self->iconCache.obj, 0, lo, hi});
    auto addIcon = [&]() {
        uint32_t cache = self->iconCache.obj;
        uint32_t entry = subdueData(self, key);
        uint32_t style = f.alloc(32);
        call(g::AvatarUtil_makeStyle, {style, entry + 32, 0});
        uint32_t made;
        {
            // AvatarIconCache::add(cache, id, style): the style pointer is the fifth word, at sp
            Frame args;
            uint32_t sp = args.alloc(8);
            st32(sp, style);
            made = uint32_t(rw::call_exact(g::AvatarIconCache_add, {cache, 0, lo, hi, style}));
        }
        call(g::SafeBuffer_dtor, {style});
        return made;
    };
    if (mvpObj) {
        if (!icon) icon = addIcon();
        if (icon == mvpObj) return;
        uint32_t setPos = ld32(ld32(icon) + V_setPosition);
        call(setPos, {icon, vcall(mvpObj, V_getPosition, {mvpObj})});
        uint32_t parent = vcall(mvpObj, IScrollProtocol::V_getParent, {mvpObj});
        vcall(icon, V_removeFromParent, {icon});
        uint32_t addChild = ld32(ld32(parent) + V_addChildZName);
        uint32_t z = vcall(mvpObj, V_getLocalZOrder, {mvpObj});
        {
            Frame fs;
            uint32_t name = newStr(fs, S_MvpIconObj, 19);
            call(addChild, {parent, icon, z, name});
            strFree(name);
        }
        vcall(mvpObj, V_removeFromParent, {mvpObj});
    } else {
        if (!icon) icon = addIcon();
        uint32_t setPos = ld32(ld32(icon) + V_setPosition);
        call(setPos, {icon, vcall(discoveryIcon, V_getPosition, {discoveryIcon})});
        vcall(icon, V_removeFromParent, {icon});
        uint32_t parent = vcall(discoveryIcon, IScrollProtocol::V_getParent, {discoveryIcon});
        uint32_t addChild = ld32(ld32(parent) + V_addChildZName);
        {
            Frame fs;
            uint32_t name = newStr(fs, S_MvpIconObj, 19);
            call(addChild, {parent, icon, uint32_t(-1), name});
            strFree(name);
        }
        setVisible(discoveryIcon, 0);
    }
}
RW_FUNCTION(0x5B43D4, "RaidSubdueScrollView::setDrawFrame(Widget*, int)", RaidSubdueScrollView::setDrawFrame);

// Touch handling per card: $_0 picks the card a touch begins on, $_1 follows the finger (no effect), $_2 ends it.
void Self::registItemTouchEvent(Self *self, Addr widget, int32_t index) {
    uint32_t me = self->self();
    Frame f;
    uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    float w = ldf(vcall(widget, IScrollProtocol::V_getContentSize, {widget}));
    float h = ldf(vcall(widget, IScrollProtocol::V_getContentSize, {widget}) + 4);
    uint32_t rect = f.alloc(16);
    call(g::Rect_ctor, {rect, 0, 0, fb(w), fb(h)});
    {
        // $_0 {this, rect, index} -> listener->onTouchBegan
        uint32_t lambda = f.alloc(24), copy = f.alloc(24), fo = f.alloc(24);
        st32(lambda, me);
        call(g::Rect_copy, {lambda + 4, rect});
        st32(lambda + 20, uint32_t(index));
        st32(copy, ld32(lambda));
        call(g::Rect_copy, {copy + 4, lambda + 4});
        st32(copy + 20, ld32(lambda + 20));
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x1C});
        if (heap) {
            st32(heap, VT_TouchBegan);
            st32(heap + 4, ld32(copy));
            call(g::Rect_copy, {heap + 8, copy + 4});
            st32(heap + 0x18, ld32(copy + 20));
        }
        st32(fo + 16, heap);
        call(g::FunctionTouchBool_swap, {fo, listener + 0x50});
        functionDestroy(fo);
        call(g::Vec2_dtor, {copy + 4});
        call(g::Vec2_dtor, {lambda + 4});
    }
    {
        // $_1 {this, rect} -> listener->onTouchMoved
        uint32_t lambda = f.alloc(24), copy = f.alloc(24), fo = f.alloc(24);
        st32(lambda, me);
        call(g::Rect_copy, {lambda + 4, rect});
        st32(copy, ld32(lambda));
        call(g::Rect_copy, {copy + 4, lambda + 4});
        st32(copy, ld32(lambda));
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_TouchMoved);
            st32(heap + 4, ld32(copy));
            call(g::Rect_copy, {heap + 8, copy + 4});
            st32(heap + 4, ld32(copy));
        }
        st32(fo + 16, heap);
        call(g::FunctionTouchVoid_swap, {fo, listener + 0x68});
        functionDestroy(fo);
        call(g::Vec2_dtor, {copy + 4});
        call(g::Vec2_dtor, {lambda + 4});
    }
    {
        // $_2 {this} -> onTouchEnded and onTouchCancelled
        uint32_t caps = f.alloc(4);
        st32(caps, me);
        assignTouchEnded(listener + 0x80, caps);
        assignTouchEnded(listener + 0x98, caps);
    }
    uint32_t dispatcher = ld32(call(0x8EE519, {}) + 0x84);   // Director::getInstance()->getEventDispatcher()
    call(g::EventDispatcher_addListenerWithSceneGraphPriority, {dispatcher, listener, widget});
    call(g::Vec2_dtor, {rect});
}
RW_FUNCTION(0x5B5EB8, "RaidSubdueScrollView::registItemTouchEvent(Widget*, int)", RaidSubdueScrollView::registItemTouchEvent);

// ------------------------------------------------------------------------------------------------ scrolling
// Lay the cards out from position + delta (the position itself is not changed here: update() moves it).
void Self::updateScroll(Self *self, float delta) {
    uint32_t me = self->self();
    if (!self->scrollEnabled) return;
    if (uint32_t(self->itemIDs.end) == uint32_t(self->itemIDs.begin)) return;
    float item = IScrollProtocol::getItemPixel(self);
    IScrollProtocol::getMaxPosition(self);
    IScrollProtocol::getMinPosition(self);
    float pos = self->position;
    IScrollProtocol::getBouncePixel(self);
    int32_t ipos = f2i(pos + delta);
    int32_t ipx = f2i(item);
    int32_t first = self->maxColumn * idiv(ipos, ipx);
    int32_t old = self->index;
    self->index = first;
    int32_t rem = imod(ipos, ipx);
    if (old != first && (self->moving || self->dragged) && ld32(me + OFF_ON_INDEX_CHANGED + 16))
        call(g::FunctionVI_call, {me + OFF_ON_INDEX_CHANGED, uint32_t(first + 1)});
    float rows = self->rowCount;
    if (rows <= 0.0f || unord(rows, 0.0f)) return;
    float remF = float(rem);
    Frame f;
    uint32_t v = f.alloc(8);
    int32_t k = 0;
    do {
        int32_t columns = self->maxColumn;
        float h = self->itemHeight;
        float y = float(imod(k, columns) + 1) * h + h * 0.5f;
        uint32_t w = ld32(self->items.begin + 4 * uint32_t(k));
        float x = (item * float(idiv(k, columns)) - remF) - self->offsetX;
        if (columns == 1) y = rw::bitsf(vcall(w, V_getPositionY, {w}));
        uint32_t setPos = ld32(ld32(w) + V_setPosition);
        call(g::Vec2_ctor, {v, fb(x), fb(y)});
        call(setPos, {w, v});
        call(g::Vec2_dtor, {v});
        if (old != first) vcall(me, IScrollProtocol::V_updateRow, {me, uint32_t(k), uint32_t(first)});
        k++;
    } while (float(k) < self->rowCount);
}
RW_FUNCTION(0x5B61B4, "RaidSubdueScrollView::updateScroll(float)", RaidSubdueScrollView::updateScroll);

// Per frame: queued rows, the centring bounce (eases towards targetPosition over bounceDuration +
// bounceExtra), coasting with a minimum speed, the bounce back into range when it stops, and the automatic
// card-by-card scroll.
void Self::update(Self *self, float dt) {
    uint32_t me = self->self();
    IScrollProtocol::updatePending(self);
    float mn = IScrollProtocol::getMinPosition(self);
    float mx = IScrollProtocol::getMaxPosition(self);
    uint8_t stillBouncing = 0;
    if (self->bouncing) {
        float bt = self->bounceTime + dt;
        self->bounceTime = bt;
        float ratio = bt / (self->bounceDuration + self->bounceExtra);
        float dist = self->bounceDistance;
        double s = callD(g::FABS, callD(g::SQRT, double(ratio)));
        bool minusInf = ratio == rw::bitsf(0xFF800000u);
        double pw = minusInf ? double(rw::bitsf(0x7F800000u)) : s;
        double move = double(-0.0f - dist) * pw;
        if (!(move <= 0.0 || move != move)) {
            float negDist = -0.0f - dist;
            float target = self->targetPosition;
            float pos = self->position;
            double u = move - double(pos + (negDist - target));
            self->position = float(double(pos) + u);
            double c = double(negDist);
            if (!(c > move || c != c || move != move)) {
                self->bouncing = 0;
                self->centering = 0;
                self->position = target;
            }
        } else {
            float target = self->targetPosition;
            float pos = self->position;
            double u = double(pos - (dist + target)) - move;
            self->position = float(double(pos) - u);
            double v = -0.0 - move;
            double dd = double(dist);
            if (!(dd > v || dd != dd || v != v)) {
                self->bouncing = 0;
                self->centering = 0;
                self->position = target;
            }
        }
        vcall(me, IScrollProtocol::V_updateScroll, {me, 0});
        stillBouncing = self->bouncing;
        if (!stillBouncing) self->bounceExtra = 0.0f;
    }
    if (self->touching) {
        self->moveDelta = self->moveDelta * 0.5f;
    } else if (self->dragged && !(self->velocity == 0.0f)) {
        // coasting
        float v = self->velocity;
        float dec = v * self->deceleration;
        bool stop;
        if (!(v <= 0.0f || unord(v, 0.0f))) {
            float pos = self->position;
            if (!(mx >= pos || unord(mx, pos))) dec = (pos - mx) * self->bounceDeceleration;
            float nv = v - dec;
            self->velocity = nv;
            if (nv < self->field_52C) {
                stop = true;
            } else {
                float lo = mn - IScrollProtocol::getBouncePixel(self);
                stop = !(pos > lo || unord(pos, lo));
            }
        } else {
            float pos = self->position;
            float d = dec;
            if (!(pos >= 0.0f || unord(pos, 0.0f))) d = pos * self->bounceDeceleration;
            float nv = v - d;
            self->velocity = nv;
            if (nv > -0.0f - self->field_52C) {
                stop = true;
            } else {
                float hi = mx + IScrollProtocol::getBouncePixel(self);
                float now = self->position;
                stop = !(hi > now || unord(hi, now));
            }
        }
        if (stop) self->velocity = 0.0f;
        if (self->moving) {
            float nv = self->velocity;
            if (!(nv == 0.0f)) {
                float minSpeed = self->field_52C;
                float step;
                if (!(nv <= 0.0f || unord(nv, 0.0f))) {
                    step = nv < minSpeed ? minSpeed : nv;
                } else {
                    float lo = neg(minSpeed);
                    step = nv > lo ? lo : nv;
                }
                if (self->scrollType) self->position = self->position - step;
                else self->position = step + self->position;
                vcall(me, IScrollProtocol::V_updateScroll, {me, 0});
            }
        }
    } else {
        // stopped
        self->dragged = 0;
        if (self->moving) {
            self->moving = 0;
            if (ld32(me + OFF_ON_SCROLL_END + 16)) {
                float pos = self->position;
                int32_t item = 0;
                if (!(pos > 0.0f)) {
                    int32_t ip = f2i(pos);
                    item = int32_t(1u - uint32_t(idiv(ip, f2i(IScrollProtocol::getItemPixel(self)))));
                }
                call(g::FunctionVI_call, {me + OFF_ON_SCROLL_END, uint32_t(item)});
                stillBouncing = self->bouncing;
            }
            if (stillBouncing) return;
            float pos = self->position;
            if (!(pos >= mn || unord(pos, mn))) {
                self->bouncing = 1;
                self->centering = 1;
                self->bounceDistance = pos - mn;
                self->bounceTime = 0.0f;
                self->targetPosition = mn;
            } else if (!(mx >= pos || unord(mx, pos))) {
                self->bouncing = 1;
                self->centering = 1;
                self->bounceDistance = pos - mx;
                self->bounceTime = 0.0f;
                self->targetPosition = mx;
            }
        }
    }
    // automatic card-by-card scroll
    if (self->autoScrolling) {
        float timer = self->autoScrollTimer;
        float interval = self->autoScrollInterval;
        if (!(timer >= interval || unord(timer, interval))) {
            self->autoScrollTimer = timer + dt;
        } else {
            uint32_t moved = self->autoScrollLeft ? IScrollProtocol::migrationLeft(self, 1)
                                                  : IScrollProtocol::migrationRight(self, 1);
            self->autoScrolling = uint8_t(moved);
        }
    }
    if (self->touching) return;
    if (self->velocity == 0.0f) vcall(me, IScrollProtocol::V_updateScroll, {me, 0});
}
RW_FUNCTION(0x5B64FC, "RaidSubdueScrollView::update(float)", RaidSubdueScrollView::update);
