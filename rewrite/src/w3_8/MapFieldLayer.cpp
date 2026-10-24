#include "MapFieldLayer.hpp"

namespace {
namespace g {
constexpr uint32_t director_getInstance = rwlib::cocos2d::Director::getInstance;     // cocos2d::Director::getInstance()
constexpr uint32_t director_visibleSize = rwlib::cocos2d::Director::getVisibleSize;     // Director::getVisibleSize() (sret Size)
constexpr uint32_t director_visibleOrigin = rwlib::cocos2d::Director::getVisibleOrigin;   // Director::getVisibleOrigin() (sret Vec2)
constexpr uint32_t vec2_ctor_xy = rwlib::cocos2d::Vec2::ctor_base__ff;             // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t vec2_ctor_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;           // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;                // cocos2d::Vec2::~Vec2()
constexpr uint32_t vec2_subtract = rwlib::cocos2d::Vec2::subtract;            // cocos2d::Vec2::subtract(const Vec2&)
constexpr uint32_t vec2_equals = rwlib::cocos2d::Vec2::equals;              // cocos2d::Vec2::equals(const Vec2&) const
constexpr uint32_t rect_ctor = rwlib::cocos2d::Rect::ctor_base__v;                // cocos2d::Rect::Rect()
constexpr uint32_t rect_setRect = rwlib::cocos2d::Rect::setRect;             // Rect::setRect(float x, float y, float w, float h)
constexpr uint32_t rect_getMinX = rwlib::cocos2d::Rect::getMinX;
constexpr uint32_t rect_getMinY = rwlib::cocos2d::Rect::getMinY;
constexpr uint32_t rect_getMaxX = rwlib::cocos2d::Rect::getMaxX;
constexpr uint32_t rect_getMaxY = rwlib::cocos2d::Rect::getMaxY;
constexpr uint32_t size_ctor = rwlib::cocos2d::Size::ctor_base__v;                // cocos2d::Size::Size()
constexpr uint32_t size_setSize = rwlib::cocos2d::Size::setSize;             // Size::setSize(float, float)
constexpr uint32_t size_equals = rwlib::cocos2d::Size::equals;              // Size::equals(const Size&) const
constexpr uint32_t stringBuffer_fset = 0x491501;        // StringBuffer::fset(const char* fmt, ...) -> text
constexpr uint32_t field_create = 0x5CD0B1;             // Field::create(int, int, const MapInfo&)
constexpr uint32_t field_preloadResources = 0x5CD9A5;   // Field::preloadResources() const
constexpr uint32_t field_update = 0x5CD259;             // Field::update(float, float)
constexpr uint32_t nodes_dtor = 0x5DA621;               // cocos2d::Vector<MapNode*>::~Vector()
constexpr uint32_t nodes_clear = 0x5DAAB9;              // cocos2d::Vector<MapNode*>::clear()
constexpr uint32_t nodes_pushBack = 0x5DAEC1;           // cocos2d::Vector<MapNode*>::pushBack(MapNode*)
constexpr uint32_t nodes_eraseObject = 0x5DAF5D;        // cocos2d::Vector<MapNode*>::eraseObject(MapNode*, bool)
constexpr uint32_t vec2_ZERO = 0x1006950;               // cocos2d::Vec2::ZERO
constexpr uint32_t str_pathFormat = 0xE0219E;           // "map/%s/%s_%s/%s_%s_%%03d.png"
}  // namespace g

// cocos2d::Node virtual slots (byte offsets)
constexpr uint32_t SET_POSITION = 0x4C, SET_POSITION_XY = 0x5C, SET_VISIBLE = 0xB0, ADD_CHILD = 0xF0,
                   ADD_CHILD_Z = 0xF4, REMOVE_FROM_PARENT = 0x124, REMOVE_CHILD = 0x12C, REMOVE_ALL_CHILDREN = 0x138,
                   SET_TAG = 0x14C, SET_CASCADE_OPACITY = 0x220, INIT_INFO = 0x270;

uint32_t ga(const void *p) { return rw::guest_addr(p); }
uint32_t vcallAt(uint32_t obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(rw::load<uint32_t>(rw::load<uint32_t>(obj) + offset), args);
}
float callF(uint32_t fn, uint32_t arg) { return rw::bitsf(rw::call(fn, {arg})); }
float ldf(uint32_t a) { return rw::load<float>(a); }
// libgcc __aeabi_fcmplt / fcmpge (false when unordered)
bool lt(float a, float b) { return a < b; }
}  // namespace

MapFieldLayer *MapFieldLayer::construct(MapFieldLayer *self) {
    MapNode::construct(self);
    self->vtable = VTABLE;
    self->container = 0;
    self->field = 0;
    self->pathLength = 0;
    rw::store8(self->pathBuffer, 0);
    rw::call(g::rect_ctor, {ga(&self->scrollOrigin)});
    rw::call(g::size_ctor, {ga(&self->mapWidth)});
    rw::call(g::size_ctor, {ga(&self->fieldWidth)});
    self->nodesBegin = 0;
    self->nodesEnd = 0;
    self->nodesCap = 0;
    return self;
}
RW_FUNCTION(0x5DA9E0, "MapFieldLayer::MapFieldLayer", MapFieldLayer::construct);

MapFieldLayer *MapFieldLayer::destruct(MapFieldLayer *self) {
    self->vtable = VTABLE;
    rw::call(actor_fn::node_removeAllChildrenWithCleanup, {ga(self), 1});
    rw::call(g::nodes_dtor, {ga(&self->nodesBegin)});
    rw::call(g::vec2_dtor, {ga(&self->scrollOrigin)});
    MapNode::destruct(self);
    return self;
}
RW_FUNCTION(0x5DA5B0, "MapFieldLayer::~MapFieldLayer", MapFieldLayer::destruct);

void MapFieldLayer::destructDelete(MapFieldLayer *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DA64C, "MapFieldLayer::~MapFieldLayer (deleting)", MapFieldLayer::destructDelete);

MapFieldLayer *MapFieldLayer::create(const Info *info) {
    auto *self = rw::host_ptr<MapFieldLayer>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self);
    if (rw::vcall(self, INIT_INFO / 4, {ga(self), ga(info)}) == 1) {   // virtual init(const Info*)
        rw::call(actor_fn::ref_autorelease, {ga(self)});
        return self;
    }
    rw::vcall(self, 1, {ga(self)});                                     // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5DA984, "MapFieldLayer::create", MapFieldLayer::create);

// Builds the Field (its picture clipped to the screen size, centred) in a container node and records the map /
// scroll / field geometry.
uint32_t MapFieldLayer::init(MapFieldLayer *self, const Info *info) {
    if (MapNode::init(self) != 1) return 0;
    rw::vcall(self, SET_TAG / 4, {ga(self), uint32_t(NODE_TAG)});
    bt::GuestFrame f(0x20);
    const uint32_t visible = f.addr(0x00), origin = f.addr(0x08), center = f.addr(0x10), pos = f.addr(0x18);
    rw::call(g::director_visibleSize, {visible, rw::call(g::director_getInstance, {})});
    rw::call(g::director_visibleOrigin, {origin, rw::call(g::director_getInstance, {})});
    float visW = ldf(visible), visH = ldf(visible + 4);
    float cx = visW * 0.5f + ldf(origin);
    float cy = visH * 0.5f + ldf(origin + 4);
    rw::call(g::vec2_ctor_xy, {center, rw::fbits(cx), rw::fbits(cy)});   // (unused) screen centre

    uint32_t area = info->area, world = info->world;
    self->infoWidth = bt::f2iz(info->fieldWidth);
    self->infoHeight = bt::f2iz(info->fieldHeight);
    self->infoPath = rw::call(g::stringBuffer_fset, {ga(self->pathBuffer), g::str_pathFormat, world, world, area,
                                                     world, area});

    // the picture is at most the screen size
    float fw = info->fieldWidth, fh = info->fieldHeight;
    float w = lt(fw, visW) ? fw : visW;
    float h = lt(fh, visH) ? fh : visH;
    int32_t iw = bt::f2iz(w);
    int32_t ih = bt::f2iz(h);
    uint32_t field = rw::call(g::field_create, {uint32_t(iw), uint32_t(ih), ga(&self->infoWidth)});
    self->field = field;
    float px = (visW - w) * 0.5f;
    float py = (visH - h) * 0.5f;
    rw::call(g::vec2_ctor_xy, {pos, rw::fbits(-px), rw::fbits(py)});
    vcallAt(field, SET_POSITION, {field, pos});
    rw::call(g::vec2_dtor, {pos});
    rw::call(g::field_preloadResources, {self->field});

    uint32_t container = rw::call(actor_fn::node_create, {});
    self->container = container;
    vcallAt(container, ADD_CHILD, {container, self->field});
    vcallAt(self->field, SET_CASCADE_OPACITY, {self->field, info->cascadeOpacity});
    vcallAt(self->container, SET_CASCADE_OPACITY, {self->container, info->cascadeOpacity});

    // the scroll area is at least the screen size
    fw = info->fieldWidth;
    bool wider = lt(visW, fw);
    fh = info->fieldHeight;
    float sh = lt(visH, fh) ? fh : visH;
    float sw = wider ? fw : visW;
    rw::call(g::rect_setRect, {ga(&self->scrollOrigin), rw::fbits(info->scrollX), rw::fbits(info->scrollY),
                               rw::fbits(sw), rw::fbits(sh)});
    rw::call(g::size_setSize, {ga(&self->mapWidth), rw::fbits(info->mapWidth), rw::fbits(info->mapHeight)});
    rw::call(g::size_setSize, {ga(&self->fieldWidth), rw::fbits(info->fieldWidth), rw::fbits(info->fieldHeight)});
    self->userValue = info->userValue;
    rw::vcall(self, SET_CASCADE_OPACITY / 4, {ga(self), info->cascadeOpacity});
    rw::vcall(self, ADD_CHILD / 4, {ga(self), self->container});
    rw::vcall(self, SET_VISIBLE / 4, {ga(self), 1});
    rw::call(g::vec2_dtor, {center});
    rw::call(g::vec2_dtor, {origin});
    return 1;
}
RW_FUNCTION(0x5DA658, "MapFieldLayer::init(const MapFieldLayer::Info*)", MapFieldLayer::init);

void MapFieldLayer::remove(MapFieldLayer *self) {
    rw::call(g::nodes_clear, {ga(&self->nodesBegin)});
    vcallAt(self->field, REMOVE_ALL_CHILDREN, {self->field});
    vcallAt(self->field, REMOVE_FROM_PARENT, {self->field});
    rw::vcall(self, REMOVE_FROM_PARENT / 4, {ga(self)});
}
RW_FUNCTION(0x5DAA7C, "MapFieldLayer::remove", MapFieldLayer::remove);

// Camera at (scrollX, scrollY) (0..1 of the map's scrollable range): scroll the Field picture, keep the container
// inside the scroll area, and place every map node relative to the camera centre.
uint32_t MapFieldLayer::update(MapFieldLayer *self, float scrollX, float scrollY) {
    bt::GuestFrame f(0x18);
    const uint32_t visible = f.addr(0x00), center = f.addr(0x08), tmp = f.addr(0x10);
    rw::call(g::director_visibleSize, {visible, rw::call(g::director_getInstance, {})});
    float visW = ldf(visible), visH = ldf(visible + 4);
    float halfW = visW * 0.5f;
    float halfH = visH * 0.5f;
    float cx = halfW + (float(self->mapWidth) - visW) * scrollX;
    float cy = halfH + (float(self->mapHeight) - visH) * scrollY;
    rw::call(g::vec2_ctor_xy, {center, rw::fbits(cx), rw::fbits(cy)});
    uint32_t rect = ga(&self->scrollOrigin);

    if (rw::call(g::size_equals, {ga(&self->scrollWidth), ga(&self->mapWidth)}) == 1 &&
        rw::call(g::vec2_equals, {rect, g::vec2_ZERO}) == 1) {
        // the scroll area is the whole map: the picture follows the camera
        float centerX = ldf(center);
        float y = float(self->mapHeight) - ldf(center + 4);
        rw::call(g::field_update, {self->field, rw::fbits(centerX), rw::fbits(y)});
    } else {
        float dx = cx - callF(g::rect_getMinX, rect);
        float minY = callF(g::rect_getMinY, rect);
        // camera's top-left corner, clamped to [0, map size - screen size]
        float t = cy - halfH;
        float top = lt(t, 0.0f) ? 0.0f : t;
        float maxTop = float(self->mapHeight) - visH;
        float oy = lt(maxTop, top) ? maxTop : top;
        float u = cx - halfW;
        float left = lt(u, 0.0f) ? 0.0f : u;
        float maxLeft = float(self->mapWidth) - visW;
        float ox = lt(maxLeft, left) ? maxLeft : left;
        float dy = cy - minY;
        // container offset: how far the camera pokes out of the scroll area
        float minX = callF(g::rect_getMinX, rect);
        float offX = 0.0f;
        if (!(ox >= minX || ox != ox || minX != minX)) offX = callF(g::rect_getMinX, rect) - ox;
        float minY2 = callF(g::rect_getMinY, rect);
        float offY = 0.0f;
        if (!(oy >= minY2 || oy != oy || minY2 != minY2)) offY = callF(g::rect_getMinY, rect) - oy;
        float maxX = callF(g::rect_getMaxX, rect) - visW;
        if (!(maxX >= ox || maxX != maxX || ox != ox)) offX = maxX - ox;
        float maxY = callF(g::rect_getMaxY, rect) - visH;
        if (!(maxY >= oy || maxY != maxY || oy != oy)) offY = maxY - oy;
        vcallAt(self->container, SET_POSITION_XY, {self->container, rw::fbits(offX), rw::fbits(offY) ^ 0x80000000u});
        rw::call(g::field_update, {self->field, rw::fbits(dx), rw::fbits(float(self->fieldHeight) - dy)});
    }

    for (uint32_t it = self->nodesBegin, end = self->nodesEnd; it != end; it += 4) {
        uint32_t node = rw::load<uint32_t>(it);
        rw::call(g::vec2_ctor_copy, {tmp, node + 624});        // the node's map position
        rw::call(g::vec2_subtract, {tmp, center});
        rw::save<float>(tmp + 4, -0.0f - ldf(tmp + 4));         // map y grows downward
        vcallAt(node, SET_POSITION, {node, tmp});
        rw::call(g::vec2_dtor, {tmp});
    }
    rw::call(g::vec2_dtor, {center});
    return 0;
}
RW_FUNCTION(0x5DAAEC, "MapFieldLayer::update", MapFieldLayer::update);

void MapFieldLayer::addFieldMapNode(MapFieldLayer *self, MapNode *node, int32_t z) {
    vcallAt(self->container, ADD_CHILD_Z, {self->container, ga(node), uint32_t(z)});
    rw::call(g::nodes_pushBack, {ga(&self->nodesBegin), ga(node)});
}
RW_FUNCTION(0x5DAE90, "MapFieldLayer::addFieldMapNode", MapFieldLayer::addFieldMapNode);

void MapFieldLayer::removeFieldMapNode(MapFieldLayer *self, MapNode *node) {
    vcallAt(self->container, REMOVE_CHILD, {self->container, ga(node), 1});
    rw::call(g::nodes_eraseObject, {ga(&self->nodesBegin), ga(node), 0});
}
RW_FUNCTION(0x5DAF28, "MapFieldLayer::removeFieldMapNode", MapFieldLayer::removeFieldMapNode);

void MapFieldLayer::addLayerMapNode(MapFieldLayer *self, MapNode *node, int32_t z) {
    rw::vcall(self, ADD_CHILD_Z / 4, {ga(self), ga(node), uint32_t(z)});
    rw::call(g::nodes_pushBack, {ga(&self->nodesBegin), ga(node)});
}
RW_FUNCTION(0x5DB030, "MapFieldLayer::addLayerMapNode", MapFieldLayer::addLayerMapNode);

void MapFieldLayer::removeLayerMapNode(MapFieldLayer *self, MapNode *node) {
    rw::vcall(self, REMOVE_CHILD / 4, {ga(self), ga(node), 1});
    rw::call(g::nodes_eraseObject, {ga(&self->nodesBegin), ga(node), 0});
}
RW_FUNCTION(0x5DB05C, "MapFieldLayer::removeLayerMapNode", MapFieldLayer::removeLayerMapNode);
