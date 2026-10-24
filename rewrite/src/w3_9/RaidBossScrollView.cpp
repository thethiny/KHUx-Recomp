// RaidBossScrollView: the raid list rows (see RaidBossScrollView.hpp).
#include "RaidBossScrollView.hpp"

#include <cmath>

using namespace scene;
using Self = RaidBossScrollView;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;          // std::__vector_base<int>::~__vector_base
constexpr uint32_t VectorBaseRaid_dtor = 0x4BA87D;         // std::__vector_base<RaidBase>::~__vector_base
constexpr uint32_t VectorRaid_assign = 0x5095C5;           // std::vector<RaidBase>::assign(first, last)
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;          // vector::at's out_of_range
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t popUpRaidBossInfo = 0x5072F5;           // (const RaidBase::Info&, std::function<void()>)
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;           // (Vec2 out, touch)
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;     // (Vec2 out, node, const Vec2&)
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t SceneUtil_setAdditionalKerning = 0x466BA9;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setBossTypeImage = 0x46C4C5;  // (ImageView*, int, bool)
constexpr uint32_t SceneUtil_getCampaignPoint = 0x4741BD;  // (int, int, int, const vector<int>&, int, bool)
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41;
constexpr uint32_t AdvStageScrollView_pointToString = 0x506B61;   // (string out, int, int, bool)
constexpr uint32_t AdvStageScrollView_updateRaidBossInfoTime = 0x56CBC9;
constexpr uint32_t Campaign_initCampaign = 0x598F31;       // (node, StageType, int)
constexpr uint32_t MasterRaidEnemy_create = 0x787745;
constexpr uint32_t SafeBuffer_ctor = 0x459F21;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t RaidParts_setMasterParam = 0x5F1551;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t LoadingBar_setPercent = rwlib::cocos2d::ui::LoadingBar::setPercent;
constexpr uint32_t TextAtlas_setString = rwlib::cocos2d::ui::TextAtlas::setString;
constexpr uint32_t AvatarIconCache_makeShared = 0x509899;  // make_shared<AvatarIconCache>(Texture2D*&, Vec2, float, float)
constexpr uint32_t AvatarIconCache_find = 0x5CA9A9;        // (cache, unsigned long long)
constexpr uint32_t AvatarIconCache_add = 0x5CAA57;         // (cache, unsigned long long, const Style&)
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;
constexpr uint32_t shared_add = 0xC8DBB5;
constexpr uint32_t DIVSI3 = 0xCD5E8D;
}  // namespace g

constexpr uint32_t GOT_RAIDPARTS_VTABLE = 0xFD65BC;
// __func vtables (+8)
constexpr uint32_t VT_TOUCH_BEGAN = 0xF560A8;    // registItemTouchEvent $_: {view, row}
constexpr uint32_t VT_TOUCH_MOVED = 0xF5607C;    // {view, widget}
constexpr uint32_t VT_TOUCH_ENDED = 0xF56050;    // {view}
constexpr uint32_t VT_INFO_CLOSED = 0xF560D4;    // update $_: {view} (the info popup closed)

// strings
constexpr uint32_t S_BLOCK_JSON = 0xDFD3F4;      // "cocostudio/publish/Adv_Raid_Boss_Block.json"
constexpr uint32_t S_SYMBOL_LUX = 0xDFD28F;      // "Symbol_LUX_Discovery" (20)
constexpr uint32_t S_ATLAS_NUML = 0xDFD2A4;      // "AtlasLabel_NumL" (15)
constexpr uint32_t S_PANEL = 0xDFD420;           // "Panel" (5)
constexpr uint32_t S_ADV_RAID_PANEL = 0xDFD3D7;  // "Adv_Raid_Panel" (14)
constexpr uint32_t S_AREA_ANNOUNCE = 0xDFD3E6;   // "Area_Announce" (13)
constexpr uint32_t S_BOSS_ICON = 0xDFD2D3;       // "Raid_Boss_Icon" (14)
constexpr uint32_t S_BOSS_IMAGE = 0xDFD2E2;      // "img/adventure/boss_L/raid_boss_%d.png"
constexpr uint32_t S_NORMAL_POP = 0xDFD308;      // "Raid_Normal_Pop" (15)
constexpr uint32_t S_RARE_POP = 0xDFD318;        // "Raid_Rare_Pop" (13)
constexpr uint32_t S_BOSS_TYPE = 0xDFD326;       // "Raid_Boss_Type" (14)
constexpr uint32_t S_NAME_LABEL = 0xDFD335;      // "Raid_Boss_Name_Label" (20)
constexpr uint32_t S_LV_TXT = 0xDFD34A;          // "Raid_Boss_Lv_Txt" (16)
constexpr uint32_t S_LVFIX = 0xDFD35B;           // "Raid_Boss_LvFix_Label" (21)
constexpr uint32_t S_FMT_D = 0xDF86DE;           // "%d"
constexpr uint32_t S_AP_TXT = 0xDFD371;          // "Raid_Boss_Ap_Txt" (16)
constexpr uint32_t S_ATLAS_AP = 0xDFD382;        // "AtlasLabel_AP" (13)
constexpr uint32_t S_HPFIX = 0xDFD390;           // "Raid_Boss_HpFix_Label" (21)
constexpr uint32_t S_HP_BAR = 0xDFD3A6;          // "Raid_Boss_Hp_ProgressBar" (24)
constexpr uint32_t S_TIMELIMIT = 0xDFD3BF;       // "Raid_Boss_TimeLimit_Txt" (23)
constexpr uint32_t S_DISCOVERY_ICON = 0xDFD25E;  // "Raid_Boss_Discovery_Icon" (24)
constexpr uint32_t S_AVATAR_ICON_OBJ = 0xDF8F5D; // "Avatar_Icon_Obj" (15)
constexpr uint32_t S_DISCOVERY_TXT = 0xDFD277;   // "Raid_Boss_Discovery_Txt" (23)
constexpr uint32_t S_DISCOVERY_NAME = 0xDFD2B4;  // "Raid_Boss_Discovery_Name_Label" (30)

constexpr int32_t TXT_LV = 100200002, TXT_AP = 103200007, TXT_TIME_LIMIT = 103200002, TXT_DISCOVERY = 103200008;

// RaidBase::Info
constexpr uint32_t INFO_LEVEL = 8, INFO_USE_AP = 12, INFO_ENEMY = 44, INFO_HP = 52, INFO_RARE = 60;
// RaidBase::User
constexpr uint32_t USER_NAME = 8, USER_LEVEL = 20, USER_AVATAR = 24;
constexpr uint32_t RAIDENEMY_NAME = 4, RAIDPARTS_MAX_HP = 16;
constexpr uint32_t API_RAIDS = 340;              // APIManager: std::vector<RaidBase>
constexpr uint32_t API_CAMPAIGNS = 764;

// virtual slots
constexpr uint32_t V_deleting = 0x04;
constexpr uint32_t V_getLocalZOrder = 0x18;
constexpr uint32_t V_setPosition = 0x4C;
constexpr uint32_t V_getPosition = 0x54;
constexpr uint32_t V_getPositionX = 0x68;
constexpr uint32_t V_getPositionY = 0x70;
constexpr uint32_t V_addChildZName = 0xFC;
constexpr uint32_t V_getParent = 0x11C;
constexpr uint32_t V_removeFromParent = 0x124;
constexpr uint32_t V_init = 0x248;
constexpr uint32_t V_getTexture = 0x280;
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;
constexpr uint32_t V_getVirtualRenderer = 0x2D8;

inline uint32_t fb(float f) { return rw::fbits(f); }
inline int32_t idiv(int32_t a, int32_t b) { return int32_t(call(g::DIVSI3, {uint32_t(a), uint32_t(b)})); }
inline uint8_t ld8_(uint32_t a) { return rw::load<uint8_t>(a); }
inline void st8_(uint32_t a, uint8_t v) { rw::save<uint8_t>(a, v); }
inline uint32_t raidCount(uint32_t me) { return (ld32(me + Self::RAIDS + 4) - ld32(me + Self::RAIDS)) / Self::RAID_SIZE; }
inline uint32_t idCount(uint32_t me) { return (ld32(me + 0x62C) - ld32(me + 0x628)) >> 2; }

// label->setShadow, then its text: a c string (copied into a std::string)
void shadowC(uint32_t label, uint32_t cstr) {
    call(g::SceneUtil_setShadow, {label});
    setTextC(label, cstr);
}
void shadowId(uint32_t label, int32_t id) {
    call(g::SceneUtil_setShadow, {label});
    setTextId(label, id);
}
// the node's rect from its position and content size
void nodeRect(uint32_t rect, uint32_t node) {
    float x = rw::bitsf(vcall(node, V_getPositionX, {node}));
    float y = rw::bitsf(vcall(node, V_getPositionY, {node}));
    float w = ldf(vcall(node, IScrollProtocol::V_getContentSize, {node}));
    float h = ldf(vcall(node, IScrollProtocol::V_getContentSize, {node}) + 4);
    call(g::Rect_ctor, {rect, fb(x), fb(y), fb(w), fb(h)});
}
// sub_5084E0: a touch-ended / cancelled slot gets the [view] lambda
void assignTouchEnded(uint32_t slot, uint32_t captures) {
    Frame f;
    uint32_t fo = newFunction(f, VT_TOUCH_ENDED, {ld32(captures)});
    call(g::FunctionTouchVoid_swap, {fo, slot});
    functionDestroy(fo);
}
}  // namespace

RW_FUNCTION(0x5084E0, "RaidBossScrollView::registItemTouchEvent touch ended setter (sub_5084E0)", assignTouchEnded);

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::construct(Self *self) {
    uint32_t me = self->self();
    call(g::Layout_ctor, {me});
    initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    st32(me + ON_SELECT + 16, 0);
    st32(me + FUNCTION_2 + 16, 0);
    st8_(me + PRESSED, 0);
    rw::save<uint16_t>(me + HOLDING, 0);
    st32(me + F_674, 0);
    st32(me + PRESSED_ITEM, 0);
    st32(me + F_66C, 0);
    call(g::AEABI_MEMSET, {me + HOLD_TIME, 0x18, 0});
    return self;
}
RW_FUNCTION(0x6C7DCC, "RaidBossScrollView::RaidBossScrollView()", RaidBossScrollView::construct);

Self::Addr Self::create() {
    uint32_t p = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(p));
    if (vcall(p, V_init, {p}) == 1) {
        call(g::Ref_autorelease, {p});
        return p;
    }
    vcall(p, V_deleting, {p});
    return 0;
}
RW_FUNCTION(0x6C6C70, "RaidBossScrollView::create()", RaidBossScrollView::create);

Self *Self::destroy(Self *self) {
    uint32_t me = self->self();
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    call(g::VectorBaseRaid_dtor, {me + RAIDS});
    if (uint32_t ctrl = ld32(me + ICON_CACHE + 4)) call(fn::shared_release, {ctrl});
    functionDestroy(me + FUNCTION_2);
    functionDestroy(me + ON_SELECT);
    self->vtable = VTABLE_INT;
    self->vtable2 = VTABLE2_INT;
    self->vtable3 = VTABLE3_INT;
    call(g::VectorBaseInt_dtor, {me + 0x628});
    IScrollProtocol::destroy(self);
    return self;
}
RW_FUNCTION(0x505B0C, "RaidBossScrollView::~RaidBossScrollView()", RaidBossScrollView::destroy);

// A vertical list of 923 x 203 rows with a scroll bar; the raids are copied from the APIManager.
uint32_t Self::init(Self *self) {
    uint32_t me = self->self();
    self->itemWidth = 923.0f;
    self->itemHeight = 203.0f;
    self->offsetY = -171.5f;
    self->maxOffset = 140.0f;
    IScrollProtocol::setEnabledScrollBar(self, 1);
    vcall(me, IScrollProtocol::V_setTouchEnabled, {me, 1});
    self->touching = 0;
    self->position = 0.0f;
    self->index = 0;
    uint32_t e = self->itemIDs.end, b = self->itemIDs.begin;
    if (e != b) self->itemIDs.end = e + (~3u & ~(e - 4 - b));
    uint32_t api = call(g::APIManager_getInstance, {});
    if (me + RAIDS != api + API_RAIDS) call(g::VectorRaid_assign, {me + RAIDS, ld32(api + API_RAIDS), ld32(api + API_RAIDS + 4)});
    return BaseScrollView::initInt(self);
}
RW_FUNCTION(0x5070CC, "RaidBossScrollView::init()", RaidBossScrollView::init);

// ------------------------------------------------------------------------------------------------ per frame
// A row held for 0.8 s opens its raid's info popup (once per press); a list at rest lays its rows out again.
void Self::update(Self *self, float dt) {
    uint32_t me = self->self();
    IScrollProtocol::update(self, dt);
    if (ld8_(me + HOLDING) && !ld8_(me + LONG_PRESSED)) {
        float t = ldf(me + HOLD_TIME) + dt;
        rw::save<float>(me + HOLD_TIME, t);
        if (t >= 0.8f) {
            st8_(me + LONG_PRESSED, 1);
            uint32_t item = ld32(me + PRESSED_ITEM);
            if (raidCount(me) <= item) call(g::THROW_OUT_OF_RANGE, {});
            Frame f;
            uint32_t fo = newFunction(f, VT_INFO_CLOSED, {me});
            call(g::popUpRaidBossInfo, {ld32(me + RAIDS) + RAID_SIZE * item, fo});
            functionDestroy(fo);
        }
    }
    if (!self->touching && self->velocity == 0.0f) vcall(me, IScrollProtocol::V_updateScroll, {me, 0});
}
RW_FUNCTION(0x50718C, "RaidBossScrollView::update(float)", RaidBossScrollView::update);

// The scroll position held within the bounce range, and each visible row's raid timer refreshed.
void Self::updateScroll(Self *self, float delta) {
    uint32_t me = self->self();
    IScrollProtocol::updateScroll(self, delta);
    float item = IScrollProtocol::getItemPixel(self);
    float mx = IScrollProtocol::getMaxPosition(self);
    float pos = self->position;
    float bp = IScrollProtocol::getBouncePixel(self);
    float p = pos + delta;
    float lo = rw::bitsf(rw::fbits(bp) ^ 0x80000000u);
    if (p < lo) {
        self->position = lo;
        p = lo;
    } else {
        float hi = mx + bp;
        if (hi < p) {
            self->position = hi;
            p = hi;
        }
    }
    float rows = self->rowCount;
    if (!(rows > 0.0f)) return;
    int32_t first = self->maxColumn * idiv(f2i(p), f2i(item));
    for (int32_t k = 0; float(k) < self->rowCount; k++) {
        int32_t n = first + k;
        if (n < 1 || uint32_t(n - 1) >= idCount(me)) continue;
        uint32_t widget = ld32(self->items.begin + 4 * uint32_t(k));
        uint32_t id = ld32(self->itemIDs.begin + 4 * uint32_t(n - 1));
        if (raidCount(me) <= id) call(g::THROW_OUT_OF_RANGE, {});
        call(g::AdvStageScrollView_updateRaidBossInfoTime, {widget, ld32(me + RAIDS) + RAID_SIZE * id});
    }
}
RW_FUNCTION(0x508984, "RaidBossScrollView::updateScroll(float)", RaidBossScrollView::updateScroll);

// ------------------------------------------------------------------------------------------------ rows
// A raid row (its finder's Lux number kerned closer), filled through changeRow.
Self::Addr Self::createRow(Self *self, uint32_t index, int32_t arg) {
    uint32_t me = self->self();
    uint32_t ids = self->itemIDs.begin;
    if (idCount(me) <= index) call(g::THROW_OUT_OF_RANGE, {});
    uint32_t reader = call(g::GUIReader_getInstance, {});
    uint32_t row = call(g::GUIReader_widgetFromJsonFile, {reader, S_BLOCK_JSON});
    uint32_t symbol = findNode(row, S_SYMBOL_LUX, 20);
    uint32_t label = findNode(symbol, S_ATLAS_NUML, 15);
    call(g::SceneUtil_setAdditionalKerning, {label, uint32_t(-21)});
    vcall(me, IScrollProtocol::V_isReplaceItem, {me, row, ld32(ids + 4 * index), uint32_t(arg)});
    return row;
}
RW_FUNCTION(0x508198, "RaidBossScrollView::createRow(int, int)", RaidBossScrollView::createRow);

// Fills a raid row; 1 when the widget is not one (no "Panel").
uint32_t Self::changeRow(Self *self, Addr widget, uint32_t index, int32_t) {
    uint32_t me = self->self();
    uint32_t ids = self->itemIDs.begin;
    if (idCount(me) <= index) call(g::THROW_OUT_OF_RANGE, {});
    if (!findNode(widget, S_PANEL, 5)) return 1;
    setRaidBossFrame(self, widget, ld32(ids + 4 * index));
    return 0;
}
RW_FUNCTION(0x5082F0, "RaidBossScrollView::changeRow(Widget*, int, int)", RaidBossScrollView::changeRow);

// Touches on a row: began picks it, moved cancels it when the finger leaves it, ended / cancelled select it.
void Self::registItemTouchEvent(Self *self, Addr widget, int32_t index) {
    uint32_t me = self->self();
    uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_TOUCH_BEGAN, {me, uint32_t(index)});
        call(g::FunctionTouchBool_swap, {fo, listener + 0x50});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_TOUCH_MOVED, {me, widget});
        call(g::FunctionTouchVoid_swap, {fo, listener + 0x68});
        functionDestroy(fo);
    }
    Frame f;
    uint32_t caps = f.alloc(8);
    st32(caps, me);
    ::assignTouchEnded(listener + 0x80, caps);
    ::assignTouchEnded(listener + 0x98, caps);
    uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    call(g::EventDispatcher_addListenerWithSceneGraphPriority, {dispatcher, listener, widget});
}
RW_FUNCTION(0x5083D0, "RaidBossScrollView::registItemTouchEvent(Widget*, int)", RaidBossScrollView::registItemTouchEvent);

// The row of raid `raid`: not touchable itself, its finder (the icon cache goes through setUserFrame and comes
// back), the boss card and the campaign announcement.
void Self::setRaidBossFrame(Self *self, Addr widget, uint32_t raid) {
    uint32_t me = self->self();
    vcall(widget, V_widgetSetTouchEnabled, {widget, 0});
    uint32_t panel = findNode(widget, S_ADV_RAID_PANEL, 14);
    vcall(panel, V_widgetSetTouchEnabled, {panel, 0});
    if (raidCount(me) <= raid) call(g::THROW_OUT_OF_RANGE, {});
    Frame f;
    uint32_t cache = f.alloc(8), out = f.alloc(8);
    st32(cache, ld32(me + ICON_CACHE));
    st32(cache + 4, ld32(me + ICON_CACHE + 4));
    uint32_t info = ld32(me + RAIDS) + RAID_SIZE * raid;
    if (ld32(cache + 4)) call(g::shared_add, {ld32(cache + 4)});
    setUserFrame(out, widget, info + RAID_FINDER, cache);
    uint32_t obj = ld32(out), ctrl = ld32(out + 4);
    st32(out, 0);
    st32(out + 4, 0);
    st32(me + ICON_CACHE, obj);
    uint32_t old = ld32(me + ICON_CACHE + 4);
    st32(me + ICON_CACHE + 4, ctrl);
    if (old) call(fn::shared_release, {old});
    if (ld32(cache + 4)) call(fn::shared_release, {ld32(cache + 4)});
    if (raidCount(me) <= raid) call(g::THROW_OUT_OF_RANGE, {});
    setBossFrame(widget, ld32(me + RAIDS) + RAID_SIZE * raid);
    uint32_t announce = findNode(widget, S_AREA_ANNOUNCE, 13);
    call(g::Campaign_initCampaign, {announce, 4, raid});
}
RW_FUNCTION(0x506EF4, "RaidBossScrollView::setRaidBossFrame(Widget*, int)", RaidBossScrollView::setRaidBossFrame);

// The boss card: image, normal / rare frame, type, name, level, AP (with campaigns), HP and bar, time limit.
// The RaidParts local (for the max HP) is a SafeBuffer registered for updates: the original frame (push 0x14 +
// sub 0xE04) keeps it at its original address.
uint32_t Self::setBossFrame(Addr widget, Addr info) {
    Frame f;
    uint32_t base = f.allocExact(0xE18);
    uint32_t parts = base + 0x140, enemyRow = base + 0x164, copy = base + 0x138;
    int32_t enemyId = int32_t(ld32(info + INFO_ENEMY));
    call(g::MasterRaidEnemy_create, {enemyRow, uint32_t(enemyId)});
    call(g::SafeBuffer_ctor, {parts, 0x60, 1});
    st32(parts, ld32(GOT_RAIDPARTS_VTABLE) + 8);
    st32(parts + 32, 0);
    st32(copy, ld32(enemyRow));
    st32(copy + 4, ld32(enemyRow + 4));
    if (ld32(copy + 4)) call(g::shared_add, {ld32(copy + 4)});
    rw::call_exact(g::RaidParts_setMasterParam, {parts, copy, ld32(info + INFO_LEVEL), 0, 0});
    sharedRelease(copy);

    uint32_t icon = findNode(widget, S_BOSS_ICON, 14);
    {
        Frame g2;
        uint32_t path = stringBuffer(g2, {S_BOSS_IMAGE, uint32_t(enemyId)});
        uint32_t s = newStr(g2, path);
        call(g::ImageView_loadTexture, {icon, s, 0});
        strFree(s);
    }
    int32_t rare = int32_t(ld32(info + INFO_RARE));
    if (uint32_t pop = findNode(widget, S_NORMAL_POP, 15)) vcall(pop, IScrollProtocol::V_setVisible, {pop, rare == 2 ? 0u : 1u});
    if (uint32_t pop = findNode(widget, S_RARE_POP, 13)) vcall(pop, IScrollProtocol::V_setVisible, {pop, rare == 2 ? 1u : 0u});
    if (uint32_t type = findNode(widget, S_BOSS_TYPE, 14)) call(g::SceneUtil_setBossTypeImage, {type, uint32_t(enemyId), 1});
    if (uint32_t name = findNode(widget, S_NAME_LABEL, 20)) {
        call(g::SceneUtil_setShadow, {name});
        Frame g2;
        uint32_t raw = g2.alloc(0x88);
        const SafeBuffer *row = rw::host_ptr<SafeBuffer>(ld32(enemyRow));
        SafeBuffer::read(row, rw::host_ptr<uint8_t>(raw), row->data.get() + RAIDENEMY_NAME, 0x81, RAIDENEMY_NAME);
        setTextC(name, raw);
    }
    if (uint32_t lv = findNode(widget, S_LV_TXT, 16)) shadowId(lv, TXT_LV);
    if (uint32_t lvFix = findNode(widget, S_LVFIX, 21)) {
        call(g::SceneUtil_setShadow, {lvFix});
        Frame g2;
        setTextC(lvFix, stringBuffer(g2, {S_FMT_D, ld32(info + INFO_LEVEL)}));
    }
    if (uint32_t ap = findNode(widget, S_AP_TXT, 16)) shadowId(ap, TXT_AP);
    if (uint32_t apNum = findNode(widget, S_ATLAS_AP, 13)) {
        uint32_t api = call(g::APIManager_getInstance, {});
        uint32_t point = call(g::SceneUtil_getCampaignPoint,
                              {ld32(info + INFO_USE_AP), 4, ld32(info + INFO_ENEMY), api + API_CAMPAIGNS, 7, 0});
        Frame g2;
        uint32_t str = g2.alloc(12);
        call(g::AdvStageScrollView_pointToString, {str, point, 0, 0});
        uint32_t s = newStr(g2, strData(str));
        call(g::TextAtlas_setString, {apNum, s});
        strFree(s);
        strFree(str);
    }
    if (uint32_t hpFix = findNode(widget, S_HPFIX, 21)) {
        call(g::SceneUtil_setShadow, {hpFix});
        Frame g2;
        setTextC(hpFix, stringBuffer(g2, {S_FMT_D, ld32(info + INFO_HP)}));
    }
    if (uint32_t bar = findNode(widget, S_HP_BAR, 24)) {
        int32_t hp = int32_t(ld32(info + INFO_HP));
        uint32_t maxHp = uint32_t(rw::host_ptr<SafeBuffer>(parts)->get32(RAIDPARTS_MAX_HP));
        call(g::LoadingBar_setPercent, {bar, fb((float(hp) / float(maxHp)) * 100.0f)});
    }
    if (uint32_t limit = findNode(widget, S_TIMELIMIT, 23)) shadowId(limit, TXT_TIME_LIMIT);
    call(g::AdvStageScrollView_updateRaidBossInfoTime, {widget, info});
    call(g::SafeBuffer_dtor, {parts});
    sharedRelease(enemyRow);
    return 0;
}
RW_FUNCTION(0x506224, "RaidBossScrollView::setBossFrame(Widget*, const RaidBase::Info&)", RaidBossScrollView::setBossFrame);

// The raid's finder: the avatar icon (from the icon cache, made on first use the size of the discovery icon's
// texture), put in place of the "Avatar_Icon_Obj" placeholder or over the discovery icon (then hidden); the
// "found by" text, the finder's level and name. The cache is handed back through `out`.
void Self::setUserFrame(Addr out, Addr widget, Addr user, Addr cache) {
    if (!ld32(cache)) {
        uint32_t discovery = findNode(widget, S_DISCOVERY_ICON, 24);
        uint32_t renderer = vcall(discovery, V_getVirtualRenderer, {discovery});
        Frame f;
        uint32_t texture = f.alloc(8), made = f.alloc(8), vec = f.alloc(8), f1 = f.alloc(8), f2 = f.alloc(8);
        st32(texture, vcall(renderer, V_getTexture, {renderer}));
        call(g::Vec2_ctor, {vec, fb(-85.0f), fb(75.0f)});
        st32(f1, 0xBFB33333);
        st32(f2, 0x3FB33333);
        call(g::AvatarIconCache_makeShared, {made, texture, vec, f1, f2});
        uint32_t obj = ld32(made), ctrl = ld32(made + 4);
        st32(made, 0);
        st32(made + 4, 0);
        st32(cache, obj);
        uint32_t old = ld32(cache + 4);
        st32(cache + 4, ctrl);
        if (old) call(fn::shared_release, {old});
        call(g::Vec2_dtor, {vec});
    }
    uint32_t idLo = ld32(user), idHi = ld32(user + 4);
    uint32_t discovery = findNode(widget, S_DISCOVERY_ICON, 24);
    uint32_t placeholder = findNode(widget, S_AVATAR_ICON_OBJ, 15);
    uint32_t icon = call(g::AvatarIconCache_find, {ld32(cache), 0, idLo, idHi});
    auto addIcon = [&]() {
        uint32_t c = ld32(cache);
        Frame f;
        uint32_t style = f.alloc(0x20);
        call(g::AvatarUtil_makeStyle, {style, user + USER_AVATAR, 0});
        icon = call(g::AvatarIconCache_add, {c, 0, idLo, idHi, style});
        call(g::SafeBuffer_dtor, {style});
    };
    auto addAsChild = [&](uint32_t parent, uint32_t zFrom) {
        uint32_t add = ld32(ld32(parent) + V_addChildZName);
        uint32_t z = vcall(zFrom, V_getLocalZOrder, {zFrom});
        Frame f;
        uint32_t name = newStr(f, S_AVATAR_ICON_OBJ, 15);
        call(add, {parent, icon, z, name});
        strFree(name);
    };
    if (placeholder) {
        if (!icon) addIcon();
        if (icon != placeholder) {
            uint32_t setPos = ld32(ld32(icon) + V_setPosition);
            call(setPos, {icon, vcall(placeholder, V_getPosition, {placeholder})});
            uint32_t parent = vcall(placeholder, V_getParent, {placeholder});
            vcall(icon, V_removeFromParent, {icon});
            addAsChild(parent, placeholder);
            vcall(placeholder, V_removeFromParent, {placeholder});
        }
    } else {
        if (!icon) addIcon();
        uint32_t setPos = ld32(ld32(icon) + V_setPosition);
        call(setPos, {icon, vcall(discovery, V_getPosition, {discovery})});
        vcall(icon, V_removeFromParent, {icon});
        uint32_t parent = vcall(discovery, V_getParent, {discovery});
        addAsChild(parent, discovery);
        vcall(discovery, IScrollProtocol::V_setOpacity, {discovery, 0});
    }
    uint32_t text = findNode(widget, S_DISCOVERY_TXT, 23);
    shadowId(text, TXT_DISCOVERY);
    uint32_t symbol = findNode(widget, S_SYMBOL_LUX, 20);
    uint32_t number = findNode(symbol, S_ATLAS_NUML, 15);
    {
        Frame f;
        uint32_t s = f.alloc(12);
        call(g::SceneUtil_numberToAtlasString, {s, ld32(user + USER_LEVEL)});
        call(g::TextAtlas_setString, {number, s});
        strFree(s);
    }
    uint32_t name = findNode(widget, S_DISCOVERY_NAME, 30);
    call(g::SceneUtil_setShadow, {name});
    call(fn::Text_setString, {name, user + USER_NAME});
    st32(out, ld32(cache));
    st32(out + 4, ld32(cache + 4));
    st32(cache, 0);
    st32(cache + 4, 0);
}
RW_FUNCTION(0x505C38, "RaidBossScrollView::setUserFrame(Widget*, const RaidBase::User&, shared_ptr<AvatarIconCache>)",
            RaidBossScrollView::setUserFrame);

// ------------------------------------------------------------------------------------------------ touch lambdas
namespace {
// $_ began {view, row}: a touch inside the row's widget presses its item (true), else not handled
uint32_t touchBegan(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t touch = ld32(touchRef);
    uint32_t me = ld32(functor + 4);
    Self *self = rw::host_ptr<Self>(me);
    uint32_t widget = ld32(self->items.begin + 4 * ld32(functor + 8));
    Frame f;
    uint32_t rect = f.alloc(16), loc = f.alloc(8), p = f.alloc(8);
    nodeRect(rect, widget);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, me, loc});
    uint32_t handled = 0;
    if (call(g::Rect_containsPoint, {rect, p}) == 1) {
        st32(me + Self::F_66C, 0);
        float item = IScrollProtocol::getItemPixel(self);
        int32_t n = int32_t(ld32(functor + 8)) + idiv(f2i(self->position), f2i(item));
        if (n >= 1 && uint32_t(n - 1) < idCount(me)) {
            st32(me + Self::PRESSED_ITEM, uint32_t(n - 1));
            handled = 1;
            st8_(me + Self::PRESSED, 1);
            st8_(me + Self::HOLDING, 1);
            st32(me + Self::HOLD_TIME, 0);
        }
    }
    return handled;
}
// $_ moved {view, widget}: the press ends when the finger leaves the row
void touchMoved(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t me = ld32(functor + 4);
    if (!ld8_(me + Self::HOLDING)) return;
    uint32_t touch = ld32(touchRef);
    uint32_t parent = vcall(me, V_getParent, {me});
    Frame f;
    uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, parent, loc});
    nodeRect(rect, ld32(functor + 8));
    if (!call(g::Rect_containsPoint, {rect, p})) st8_(me + Self::HOLDING, 0);
}
// $_ ended {view}: a press released within 10 pixels of where it started selects the item (unless the long
// press fired); the press state is cleared
void touchEnded(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t me = ld32(functor + 4);
    Self *self = rw::host_ptr<Self>(me);
    if (ld32(me + Self::ON_SELECT + 16) && ld8_(me + Self::PRESSED) && !ld8_(me + Self::LONG_PRESSED) &&
        ld8_(me + Self::HOLDING)) {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, ld32(touchRef)});
        float d = std::fabs(ldf(loc + 4) - self->touchStartY);
        if (d <= 10.0f)
            call(g::FunctionVI_call, {me + Self::ON_SELECT, ld32(self->itemIDs.begin + 4 * ld32(me + Self::PRESSED_ITEM))});
    }
    st8_(me + Self::PRESSED, 0);
    st8_(me + Self::LONG_PRESSED, 0);
    st8_(me + Self::HOLDING, 0);
}
}  // namespace
RW_FUNCTION(0x509374, "RaidBossScrollView::registItemTouchEvent touch began operator() (sub_509374)", touchBegan);
RW_FUNCTION(0x5091C4, "RaidBossScrollView::registItemTouchEvent touch moved operator() (sub_5091C4)", touchMoved);
RW_FUNCTION(0x509038, "RaidBossScrollView::registItemTouchEvent touch ended operator() (sub_509038)", touchEnded);

// popUpRaidBossInfo's $_8 (sub_508C9C) {.., .., popup, shadow, .., function<void()> done (+0x18)}: the info popup
// closed (false: by the player) - the cancel sound, the popup flag cleared, the close animation ($_ {+0xC, +0x10,
// popup}), the done callback, the shadow faded out.
namespace {
void infoPopupClosed(uint32_t functor, uint32_t argRef) {
    if (ld8_(argRef)) return;
    call(0x5C0D81, {});                                       // AudioPlaySystemSECancel
    st8_(call(0x60C26D, {}) + 0x308, 0);                      // SceneManager::getInstance()
    uint32_t popup = ld32(functor + 8);
    {
        Frame f;
        uint32_t fo = newFunction(f, 0xF55FCC, {ld32(functor + 0xC), ld32(functor + 0x10), popup});
        uint32_t anim = call(0x45BC61, {popup, fo});          // SceneUtil::createClosePopupAnimation
        call(rwlib::cocos2d::Node::runAction, {popup, call(rwlib::cocos2d::EaseSineOut::create, {anim})});      // runAction(EaseSineOut::create)
        functionDestroy(fo);
    }
    if (ld32(functor + 0x28)) call(0x470E69, {functor + 0x18});   // std::function<void()>::operator()
    uint32_t fade = call(rwlib::cocos2d::FadeTo::create, {ld32(ld32(0xFD65C0)), 0});     // FadeTo(popup fade time, 0)
    call(rwlib::cocos2d::Node::runAction, {ld32(functor + 0x10), fade});
}
}  // namespace
RW_FUNCTION(0x508C9C, "popUpRaidBossInfo $_8 operator() (sub_508C9C)", infoPopupClosed);
