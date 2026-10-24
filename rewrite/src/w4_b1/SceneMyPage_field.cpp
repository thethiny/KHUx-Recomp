// SceneMyPage: the field (initField) - background, its LWF animations / particles, the avatar layer.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t MypageBackground_create = 0x7924E1;   // master::MypageBackground::create(shared_ptr out, id)
constexpr uint32_t Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t Node_create = rwlib::cocos2d::Node::create;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;      // (const char*, void*)
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;    // (view, const string&, TextureResType)
constexpr uint32_t FileUtils_getInstance = rwlib::cocos2d::FileUtils::getInstance;
constexpr uint32_t ParticleSystemQuad_create = rwlib::cocos2d::ParticleSystemQuad::create;
constexpr uint32_t ValueMap_dtor = 0x65253D;
constexpr uint32_t ANCHOR_BOTTOM_LEFT = 0x1006958, ANCHOR_MIDDLE = 0xFEF5C0;
constexpr uint32_t S_LWF = 0xE03969;       // lwf/home/%d/%d.lwf
constexpr uint32_t S_PNG = 0xE03956;       // lwf/home/%d/%d.png
constexpr uint32_t S_PLIST = 0xE0397C;     // img/home/%d/%d.plist
constexpr uint32_t DEFAULT_SCALE = 0x4026353Fu;   // 2.597

// MypageBackground row: int[6] arrays of the parts
int32_t part(uint32_t row, uint32_t array, int32_t i) { return scene::safe32(row, array + uint32_t(i) * 4); }
}  // namespace

void SceneMyPage_initField(uint32_t self) {
    Self *s = S(self);
    Frame f;
    uint32_t bg = f.alloc(8);
    call(MypageBackground_create, {bg, ld32(ld32(call(APIManager_getInstance, {}) + 0x314) + 0x24)});
    const uint32_t row = ld32(bg);
    // field scale per background type (1: default, 2: 2.0, 3: 1.5, none / 100: random 1..3)
    int32_t type = 0;
    bool random = !row;
    if (row) {
        type = scene::safe32(row, 0x20);
        if (type > 99) random = type == 100;
    }
    if (random) type = int32_t(call(Random_generateUniformInt, {call(Random_getInstance, {}), 1, 3, 0}));
    uint32_t scale = DEFAULT_SCALE;
    if (type == 2) scale = 0x40000000u;
    else if (type == 3) scale = 0x3FC00000u;
    rw::set<uint32_t>(s, OFF_SCALE_BASE, scale);
    rw::set<uint32_t>(s, OFF_WALK_UNIT, 0x40477319u);
    float ratio = bf(scale) / bf(DEFAULT_SCALE);
    stf(self + 0x370, ratio);
    stf(self + OFF_SCALE, ratio * bf(0x3FA66666u));

    s->fieldLayer = call(Widget_create, {});
    vcall(s->fieldLayer, node::setAnchorPoint, {s->fieldLayer, ANCHOR_BOTTOM_LEFT});
    {
        uint32_t layer = s->fieldLayer;
        vcall(layer, node::setContentSize, {layer, vcall(s->panel6, node::getContentSize, {s->panel6})});
    }
    vcall(s->panel6, node::addChild, {s->panel6, s->fieldLayer});
    rw::set<uint32_t>(s, 0x3C4, 0);
    {
        uint32_t vs = f.alloc(8);
        call(fn::Director_getVisibleSize, {vs, call(fn::Director_getInstance, {})});
        stf(self + 0x3C8, ldf(vs) * bf(0x3D4CCCCDu));
    }
    // the front layer of the field (parts with z >= 10)
    uint32_t front = call(Widget_create, {});
    rw::set<uint32_t>(s, 0x2A8, front);
    vcall(front, node::setAnchorPoint, {front, ANCHOR_MIDDLE});
    {
        float pw = ldf(vcall(s->panel6, node::getContentSize, {s->panel6}));
        float ph = ldf(vcall(s->panel6, node::getContentSize, {s->panel6}) + 4);
        uint32_t v = f.alloc(8);
        call(wl::Vec2_ctorXY, {v, fb(pw * 0.5f), fb(ph * 0.5f)});
        vcall(front, node::setPosition, {front, v});
        call(wl::Vec2_dtor, {v});
        uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fb(1590.0f), fb(896.0f)});
        vcall(front, node::setContentSize, {front, size});
        uint32_t sc = vcall(s->fieldWidget, node::getScale, {s->fieldWidget});
        vcall(front, node::setScale, {front, sc});
        vcall(s->fieldLayer, node::addChildZ, {s->fieldLayer, front, 9999});
    }
    uint32_t center = f.alloc(8);
    call(wl::Vec2_ctorXY, {center, fb(795.0f), fb(448.0f)});
    const int32_t count = scene::safe32(row, 0x24);
    for (int32_t i = 0; i < count; i++) {
        int32_t kind = part(row, 0x40, i);
        if (kind == 1) {
            // the background image
            uint32_t image = s->fieldImage;
            int32_t a = part(row, 0x28, i), b = part(row, 0x28, i);
            Frame g;
            uint32_t buf = stringBuffer(g, {S_PNG, uint32_t(a), uint32_t(b)});
            uint32_t path = newStr(g, buf);
            call(ImageView_loadTexture, {image, path, 0});
            strFree(path);
        } else if (kind == 2) {
            // an LWF animation (z >= 10: in front of the avatars)
            uint32_t holder = call(Node_create, {});
            int32_t a = part(row, 0x28, i), b = part(row, 0x28, i);
            Frame g;
            uint32_t buf = stringBuffer(g, {S_LWF, uint32_t(a), uint32_t(b)});
            uint32_t lwf = call(AudioLWFNode_create, {buf, 0});
            if (!lwf) continue;
            float x0 = ldf(center);
            int32_t px = part(row, 0x58, i);
            float y0 = ldf(center + 4);
            int32_t py = part(row, 0x70, i);
            uint32_t pos = g.alloc(8);
            call(wl::Vec2_ctorXY, {pos, fb(x0 + float(px)), fb(y0 + float(py))});
            vcall(lwf, node::setPosition, {lwf, pos});
            vcall(holder, node::addChild, {holder, lwf});
            int32_t z = part(row, 0x88, i);
            uint32_t parent = z >= 10 ? ld32(self + 0x2A8) : uint32_t(s->fieldWidget);
            vcall(parent, node::addChildZ, {parent, holder, uint32_t(z)});
            call(wl::Vec2_dtor, {pos});
        } else if (kind == 3) {
            // a particle effect (when its plist exists)
            int32_t a = part(row, 0x28, i), b = part(row, 0x28, i);
            Frame g;
            uint32_t buf = stringBuffer(g, {S_PLIST, uint32_t(a), uint32_t(b)});
            uint32_t fu = call(FileUtils_getInstance, {});
            uint32_t full = g.alloc(12);
            {
                uint32_t name = newStr(g, buf);
                call(ld32(ld32(fu) + 0x1C), {full, fu, name});   // fullPathForFilename
                strFree(name);
            }
            uint32_t fu2 = call(FileUtils_getInstance, {});
            uint32_t cstr = strIsLong(full) ? ld32(full + 8) : full + 1;
            uint32_t map = g.alloc(0x14);
            {
                uint32_t name = newStr(g, cstr);
                call(ld32(ld32(fu2) + 0x68), {map, fu2, name});   // getValueMapFromFile
                strFree(name);
            }
            if (ld32(map + 0xC)) {
                uint32_t name = newStr(g, buf);
                uint32_t ps = call(ParticleSystemQuad_create, {name});
                strFree(name);
                vcall(ps, 0x280, {ps, 0});
                vcall(ps, node::setScale, {ps, fb(1.0f / ldf(self + OFF_SCALE_BASE))});
                float pw = ldf(vcall(s->panel6, node::getContentSize, {s->panel6}));
                vcall(ps, node::setPositionX, {ps, fb(pw * 0.5f)});
                vcall(ps, 0x1C4, {ps, fb(3.0f)});
                vcall(s->centerWidget, node::addChild, {s->centerWidget, ps});
            }
            call(ValueMap_dtor, {map});
            strFree(full);
        }
    }
    call(wl::Vec2_dtor, {center});
    release(ld32(bg + 4));
}

RW_FUNCTION(0x63B620, "SceneMyPage::initField", SceneMyPage_initField);

// ------------------------------------------------------------------------------------------------ move points
namespace {
constexpr uint32_t Vec2Vector_pushSlow = 0x64DFA9;       // vector<Vec2>::__push_back_slow_path(const Vec2&)
constexpr uint32_t UIntVector_pushSlow = 0x4ED6F1;       // vector<unsigned>::__push_back_slow_path(unsigned&&)
constexpr uint32_t UIntVector_copy = 0x641E91;           // vector<unsigned>::vector(const vector&)
constexpr uint32_t RouteVector_pushSlow = 0x64DE09;      // vector<vector<unsigned>>::__push_back_slow_path(const&)

// vector<Vec2>::push_back(Vec2(x, y)) as inlined (a null end pointer becomes 8)
void pushPoint(uint32_t vec, float x, float y) {
    Frame f;
    uint32_t tmp = f.alloc(8);
    call(wl::Vec2_ctorXY, {tmp, fb(x), fb(y)});
    uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t e = 0;
        if (end) {
            call(fn::Vec2_ctor_copy, {end, tmp});
            e = ld32(vec + 4);
        }
        st32(vec + 4, e + 8);
    } else {
        call(Vec2Vector_pushSlow, {vec, tmp});
    }
    call(wl::Vec2_dtor, {tmp});
}

// a temporary vector<unsigned> of the neighbours, pushed onto the routes
void pushRoute(uint32_t routes, std::initializer_list<uint32_t> next) {
    Frame f;
    uint32_t tmp = f.alloc(12);
    st32(tmp, 0);
    st32(tmp + 4, 0);
    st32(tmp + 8, 0);
    for (uint32_t v : next) {
        uint32_t value = f.alloc(4);
        st32(value, v);
        uint32_t end = ld32(tmp + 4);
        if (end < ld32(tmp + 8)) {
            uint32_t e = 0;
            if (end) {
                st32(end, v);
                e = ld32(tmp + 4);
            }
            st32(tmp + 4, e + 4);
        } else {
            call(UIntVector_pushSlow, {tmp, value});
        }
    }
    uint32_t end = ld32(routes + 4);
    if (end != ld32(routes + 8)) {
        uint32_t e = 0;
        if (end) {
            call(UIntVector_copy, {end, tmp});
            e = ld32(routes + 4);
        }
        st32(routes + 4, e + 0xC);
    } else {
        call(RouteVector_pushSlow, {routes, tmp});
    }
    call(UIntVector_baseDtor, {tmp});
}
}  // namespace

// The 11 points the avatars walk between (field coordinates) and the points reachable from each
void SceneMyPage_initMovePoints(uint32_t self) {
    const uint32_t points = self + OFF_POINTS, routes = self + OFF_ROUTES;
    struct Point { float x, y; std::initializer_list<uint32_t> next; };
    const Point graph[] = {
        {200, 110, {3, 7, 8}}, {200, 65, {3, 7, 8}}, {200, 20, {3, 7, 8}},
        {460, 65, {0, 1, 2, 4, 5, 6, 8, 10}},
        {700, 110, {3, 9, 10}}, {700, 65, {3, 9, 10}}, {700, 20, {3, 9, 10}},
        {360, 70, {0, 1, 2, 8}}, {360, 20, {0, 1, 2, 3, 7}},
        {560, 70, {4, 5, 6, 10}}, {560, 20, {3, 4, 5, 6, 9}},
    };
    for (const Point &p : graph) {
        pushPoint(points, p.x, p.y);
        pushRoute(routes, p.next);
    }
    // one "occupied" flag per point
    if (ld32(points) != ld32(points + 4)) {
        for (uint32_t i = 0; i < uint32_t(int32_t(ld32(points + 4) - ld32(points)) >> 3); i++) {
            Frame f;
            uint32_t no = f.alloc(4);
            st8(no, 0);
            call(BoolVector_pushBack, {self + OFF_VEC_328, no});
        }
    }
}

RW_FUNCTION(0x63BDDC, "SceneMyPage::initMovePoints", SceneMyPage_initMovePoints);
