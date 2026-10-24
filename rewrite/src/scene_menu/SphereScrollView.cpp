// SphereScrollView: the Avatar Board wheel (see SphereScrollView.hpp).
#include "SphereScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = SphereScrollView;

namespace {
namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;                 // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create;
constexpr uint32_t IntVector_assign = 0x47D5BD;             // std::vector<int>::assign(int*, int*)
constexpr uint32_t IntVector_base_dtor = 0x478961;          // std::__vector_base<int>::~__vector_base
constexpr uint32_t SphereVector_assign = 0x5032B9;          // vector<shared_ptr<const Sphere>>::assign(first, last)
constexpr uint32_t SphereVector_base_dtor = 0x50248D;
constexpr uint32_t Node_unscheduleUpdate = rwlib::cocos2d::Node::unscheduleUpdate;
constexpr uint32_t Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache;
constexpr uint32_t TextureCache_addImage = rwlib::cocos2d::TextureCache::addImage;        // (cache, const string& path)
constexpr uint32_t ImageView_loadTextureTexture = rwlib::cocos2d::ui::ImageView::loadTexture__PNS_9Texture2DE; // ui::ImageView::loadTexture(Texture2D*)
constexpr uint32_t master_Sphere_create = 0x78CF21;         // (shared_ptr out, id)
constexpr uint32_t UserSphereMap_find = 0x503479;           // map<int, UserSphere::Sphere>::find(const int&) const
constexpr uint32_t libm_sin = 0x415A30, libm_cos = 0x415A3C; // (imports)
}  // namespace g

constexpr uint32_t S_IncentiveFmt = 0xDF94FA;    // "img/sphere_incentive/%d.png"
constexpr uint32_t S_BoardFmt = 0xDF9516;        // "img/sphere_board/Board_%04d.png"
constexpr uint32_t S_KindFmt = 0xDF9536;         // "img/sphere_kind/Kind_%04d.png"
constexpr uint32_t S_Sprite = 0xDFD170;
constexpr uint32_t S_SelectBase = 0xDF94D5;
constexpr uint32_t S_SelectBoard = 0xDF94E1;
constexpr uint32_t S_SelectKind = 0xDF94EE;
constexpr uint32_t S_New = 0xDFD177;
constexpr uint32_t S_Clear = 0xDFD17B;
constexpr uint32_t S_JewelPanel = 0xDFD181;
constexpr uint32_t S_LbJewel = 0xDFD18D;
constexpr uint32_t S_NoPanel = 0xDFD196;
constexpr uint32_t S_TxtNoLabel = 0xDFD19F;
constexpr uint32_t S_SelectOn = 0xDFD1AC;
constexpr uint32_t S_TxtJewelLabel = 0xDFD1B6;
constexpr uint32_t S_PercentD = 0xDF86DE;        // "%d"

constexpr int32_t TXT_BOARD_NUMBER = 103400051;  // "No.%d"
constexpr int32_t DEFAULT_INCENTIVE = 2100001;

// master::Sphere fields
constexpr uint32_t SPHERE_ID = 0x00;
constexpr uint32_t SPHERE_KIND_IMAGE = 0x88;     // 0: no kind badge
constexpr uint32_t SPHERE_BOARD_IMAGE = 0x8C;
constexpr uint32_t SPHERE_TYPE = 0x98;           // 1 numbered board, 2 sold for jewels, ...
constexpr uint32_t SPHERE_PRICE = 0xA4;

constexpr uint32_t APIMANAGER_USER_SPHERES = 0xB4;   // map<int, UserSphere::Sphere>*
// map node: key at +0x10, UserSphere::Sphere at +0x14
constexpr uint32_t USER_SPHERE_BOUGHT = 0x1C;        // byte
constexpr uint32_t USER_SPHERE_STATUS = 0x20;        // 3: cleared

constexpr uint32_t STATUS_CLEARED = 3;

double dbits(uint64_t u) { double d; std::memcpy(&d, &u, 8); return d; }
uint64_t bitsd(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }
double guestLibm(uint32_t fnAddr, double x) {
    const uint64_t b = bitsd(x);
    return dbits(rw::call64(fnAddr, {uint32_t(b), uint32_t(b >> 32)}));
}
// __fixdfsi / __fixunsdfsi
int32_t d2i(double d) {
    if (d != d) return 0;
    if (d >= 2147483648.0) return INT32_MAX;
    if (d <= -2147483649.0) return INT32_MIN;
    return int32_t(d);
}
uint32_t d2u(double d) {
    if (d != d || d <= 0.0) return 0;
    if (d >= 4294967296.0) return UINT32_MAX;
    return uint32_t(d);
}

// TextureCache::addImage(printf(fmt, id))
uint32_t loadTexture(uint32_t fmt, int32_t id) {
    uint32_t cache = call(g::Director_getTextureCache, {call(fn::Director_getInstance, {})});
    Frame f;
    uint32_t buf = stringBuffer(f, {fmt, uint32_t(id)});
    uint32_t path = newStr(f, buf);
    uint32_t texture = call(g::TextureCache_addImage, {cache, path});
    strFree(path);
    return texture;
}

void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
SphereScrollView::Addr Self::create(int32_t type) {
    uint32_t me = call(fn::operator_new, {SIZE});
    Self *self = rw::host_ptr<Self>(me);
    BaseEndlessrollView::construct(self);
    self->vtable = VTABLE;
    call(g::aeabi_memset, {me + 0x3B8, 0x18, 0});
    if (call(ld32(VTABLE + 0x270), {me, uint32_t(type)}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}
RW_FUNCTION(0x6B84C4, "SphereScrollView::create", SphereScrollView::create);

Self *Self::destroy(Self *self) {
    const Addr me = rw::guest_addr(self);
    self->vtable = VTABLE;
    call(g::Node_unscheduleUpdate, {me});
    call(g::SphereVector_base_dtor, {me + 0x3C4});
    call(g::IntVector_base_dtor, {me + 0x3B8});
    BaseEndlessrollView::destroy(self);
    return self;
}
RW_FUNCTION(0x50241C, "SphereScrollView::~SphereScrollView", SphereScrollView::destroy);

// Nine rows cloned from an empty widget, 45 degrees apart, row 4 in front; `type` is not used.
uint32_t Self::init(Self *self, int32_t) {
    self->rowTemplate = call(g::Widget_create, {});
    self->rowCount = 9;
    self->centerRow = 4;
    self->step = 45.0f;
    self->field_344 = 1;
    return BaseEndlessrollView::init(self, 4);
}
RW_FUNCTION(0x502F58, "SphereScrollView::init", SphereScrollView::init);

// Show `spheres` (copied), item `index` in front, and refill the rows.
void Self::setSphereBoard(Self *self, Addr spheres, int32_t index) {
    const Addr me = rw::guest_addr(self);
    if (me + 0x3C4 != spheres) call(g::SphereVector_assign, {me + 0x3C4, ld32(spheres), ld32(spheres + 4)});
    self->itemCount = int32_t(self->spheres.end - self->spheres.begin) >> 3;
    self->index = index;
    self->pos = 0.0f;
    vcall(me, V_SET_SUB, {me, 1, 0});
}
RW_FUNCTION(0x502F98, "SphereScrollView::setSphereBoard", SphereScrollView::setSphereBoard);

// The boards marked New (copied; none when `ids` is null).
void Self::setSphereBoardNewId(Self *self, Addr ids) {
    const Addr me = rw::guest_addr(self);
    if (ids) {
        if (me + 0x3B8 != ids) call(g::IntVector_assign, {me + 0x3B8, ld32(ids), ld32(ids + 4)});
        return;
    }
    if (self->newIds.end != self->newIds.begin) self->newIds.end = self->newIds.begin;
}
RW_FUNCTION(0x503278, "SphereScrollView::setSphereBoardNewId", SphereScrollView::setSphereBoardNewId);

SphereScrollView::Addr Self::loadTextureIncentive(Addr, int32_t id) { return loadTexture(S_IncentiveFmt, id); }
RW_FUNCTION(0x5024DC, "SphereScrollView::loadTextureIncentive", SphereScrollView::loadTextureIncentive);

SphereScrollView::Addr Self::loadTextureBoard(Addr, int32_t id) { return loadTexture(S_BoardFmt, id); }
RW_FUNCTION(0x5025A8, "SphereScrollView::loadTextureBoard", SphereScrollView::loadTextureBoard);

SphereScrollView::Addr Self::loadTextureKind(Addr, int32_t id) { return loadTexture(S_KindFmt, id); }
RW_FUNCTION(0x502674, "SphereScrollView::loadTextureKind", SphereScrollView::loadTextureKind);

// ------------------------------------------------------------------------------------------------ rows
// Fill `row` with board `item` of the list.
void Self::changeRow(Self *self, Addr row, int32_t item) {
    const uint32_t sphereSlot = self->spheres.begin + 8 * uint32_t(item);
    const auto sphere = [&] { return ld32(sphereSlot); };
    const int32_t id = safe32(sphere(), SPHERE_ID);
    const int32_t type = safe32(sphere(), SPHERE_TYPE);
    const int32_t boardImage = safe32(sphere(), SPHERE_BOARD_IMAGE);
    const int32_t kindImage = safe32(sphere(), SPHERE_KIND_IMAGE);
    uint32_t icon;
    {
        Frame f;
        uint32_t name = newStr(f, S_Sprite, 6);
        icon = vcall(row, node::getChildByName, {row, name});
        strFree(name);
    }
    const uint32_t base = findNode(icon, S_SelectBase, 11);
    const uint32_t board = findNode(icon, S_SelectBoard, 12);
    const uint32_t kind = findNode(icon, S_SelectKind, 11);
    uint32_t texture = loadTextureIncentive(0, id);
    if (!texture) texture = loadTextureIncentive(0, DEFAULT_INCENTIVE);
    call(g::ImageView_loadTextureTexture, {base, texture});
    texture = loadTextureBoard(0, boardImage);
    if (!texture) texture = loadTextureBoard(0, 1);
    call(g::ImageView_loadTextureTexture, {board, texture});
    if (kindImage) {
        texture = loadTextureKind(0, kindImage);
        if (!texture) texture = loadTextureKind(0, 1);
        call(g::ImageView_loadTextureTexture, {kind, texture});
        setVisible(kind, 1);
    } else {
        setVisible(kind, 0);
    }
    const uint32_t newMark = findNode(icon, S_New, 3);
    const uint32_t clear = findNode(icon, S_Clear, 5);
    const uint32_t jewelPanel = findNode(icon, S_JewelPanel, 11);
    const uint32_t jewelLabel = findNode(icon, S_LbJewel, 8);
    const uint32_t noPanel = findNode(icon, S_NoPanel, 8);
    setVisible(noPanel, type == 1 ? 1 : 0);
    if (type == 1) {
        const uint32_t text = findNode(icon, S_TxtNoLabel, 12);
        SceneUtil::setShadow(text);
        Frame f;
        uint32_t td = textData(f, TXT_BOARD_NUMBER);
        uint32_t buf = stringBuffer(f, {textDataStr(td), uint32_t(id % 100000)});
        setTextC(text, buf);
        sharedRelease(td);
    }
    const uint32_t selectOn = findNode(icon, S_SelectOn, 9);
    if (selectOn) setVisible(selectOn, 0);
    if (safe32(sphere(), SPHERE_TYPE) == 2) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t key = f.alloc(4);
        st32(key, uint32_t(safe32(sphere(), SPHERE_ID)));
        const uint32_t master = masterRow(f, g::master_Sphere_create, ld32(key));
        const uint32_t map = ld32(api + APIMANAGER_USER_SPHERES);
        const uint32_t node = call(g::UserSphereMap_find, {map, key});
        if (node == map + 4) {
            setVisible(jewelPanel, 0);
            setVisible(jewelLabel, 0);
        } else if (rw::load<uint8_t>(node + USER_SPHERE_BOUGHT)) {
            setVisible(jewelPanel, 1);
            setVisible(jewelLabel, 0);
        } else {
            setVisible(jewelPanel, 0);
            setVisible(jewelLabel, 1);
            const uint32_t text = findNode(icon, S_TxtJewelLabel, 15);
            SceneUtil::setShadow(text);
            Frame g2;
            uint32_t buf = stringBuffer(g2, {S_PercentD, uint32_t(safe32(sphere(), SPHERE_PRICE))});
            setTextC(text, buf);
        }
        sharedRelease(master);
    } else {
        setVisible(jewelPanel, 0);
        setVisible(jewelLabel, 0);
    }
    // the Clear mark: the user's board has status 3
    bool cleared = false;
    {
        const uint32_t map = ld32(call(fn::APIManager_getInstance, {}) + APIMANAGER_USER_SPHERES);
        if (map) {
            Frame f;
            const uint32_t key = f.alloc(4);
            st32(key, uint32_t(id));
            const uint32_t node = call(g::UserSphereMap_find, {map, key});
            cleared = node != map + 4 && ld32(node + USER_SPHERE_STATUS) == STATUS_CLEARED;
        }
    }
    setVisible(newMark, 0);
    for (uint32_t k = 0; k < (self->newIds.end - self->newIds.begin) >> 2; k++) {
        if (int32_t(ld32(self->newIds.begin + 4 * k)) == id) setVisible(newMark, 1);
    }
    setVisible(clear, cleared ? 1 : 0);
}
RW_FUNCTION(0x502740, "SphereScrollView::changeRow", SphereScrollView::changeRow);

// Place row `row` at wheel angle (row + 4) * step + angle: x = 480 + 380 sin, scale 0.5..1 and depth from the
// cosine, y 240..340, front rows white and back rows tinted toward (127, 127, 159).
void Self::Set_disp_param(Self *self, int32_t row, float angle) {
    const uint32_t widget = ld32(self->rows.begin + 4 * uint32_t(row));
    if (!widget) return;
    const float slot = float(row + 4);
    const double s = guestLibm(g::libm_sin, double(degree2radian(0, slot * self->step + angle)));
    const double c = guestLibm(g::libm_cos, double(degree2radian(0, slot * self->step + angle)));
    vcall(widget, node::setPositionX, {widget, fbits(float(s * 380.0 + 480.0))});
    const double depth = c * 0.5 + 0.5;
    vcall(widget, node::setScale, {widget, fbits(float(depth))});
    vcall(widget, node::setLocalZOrder, {widget, uint32_t(d2i(depth * 255.0))});
    vcall(widget, node::setPositionY, {widget, fbits(float(depth * 100.0 + 240.0))});
    const double front = c > 0.0 ? c : 0.0;
    const uint32_t blue = d2u(front * 0.375 * 255.0 + 159.375);
    const uint32_t red = d2u(front * 0.5 * 255.0 + 127.5);
    Frame f;
    const uint32_t color = f.alloc(4);
    call(fn::Color3B_ctor, {color, red, red, blue});
    vcall(widget, node::setColor, {widget, color});
}
RW_FUNCTION(0x503008, "SphereScrollView::Set_disp_param", SphereScrollView::Set_disp_param);
