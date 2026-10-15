#include "RaidBoss.hpp"

#include "FuncThunks.hpp"

using namespace scene;

namespace {
// library / game functions
constexpr uint32_t FlashNode_ctor = 0x5C32F5;            // FlashNode::FlashNode(bool)
constexpr uint32_t FlashNode_update = 0x5C3AF7;          // FlashNode::update(float)
constexpr uint32_t FlashNode_change_sub = 0x5C2F49;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t StringBuffer_fset = 0x491501;         // StringBuffer::fset(fmt, ...)
constexpr uint32_t LWFNode_create = 0x7EDF31;            // cocos2d::LWFNode::create(const char*, void*)
constexpr uint32_t LWF_SearchMovieInstance = 0x7FE1B9;   // LWF::LWF::SearchMovieInstance(const string&)
constexpr uint32_t Movie_SearchMovieInstance = 0x8122D7; // LWF::Movie::SearchMovieInstance(const string&, bool)
constexpr uint32_t Movie_GotoAndPlay = 0x815D91;
constexpr uint32_t Movie_SetVisible = 0x815CB1;
constexpr uint32_t Movie_GetX = 0x815FCD;
constexpr uint32_t Movie_GetY = 0x816017;
constexpr uint32_t LWFUtil_preloadResources = 0x491361;  // (const string&)
constexpr uint32_t MovieEventFunction_assign = 0x5C9BE5; // function<void(const string&,Movie*,Button*)>::operator=
constexpr uint32_t ClickMap_clear = 0x5D8417;            // unordered_map<string,Vec2>::clear()
constexpr uint32_t ClickMap_find = 0x5D78F9;             // unordered_map<string,Vec2>::find(const string&)
constexpr uint32_t ClickMap_erase = 0x5D75BD;            // unordered_map<string,Vec2>::erase(const_iterator)
constexpr uint32_t ClickMap_insert_unique = 0x5D7DF9;    // __insert_unique(pair<const char*, Vec2>&&)
constexpr uint32_t ClickMap_dtor = 0x5D8485;
constexpr uint32_t NameSet_find = 0x5D7733;              // unordered_set<string>::find(const string&)
constexpr uint32_t NameSet_insert_unique = 0x4DA505;     // unordered_set<string>::__insert_unique(const string&)
constexpr uint32_t NameSet_dtor = 0x4D6DD1;
constexpr uint32_t IntVector_base_dtor = 0x478961;       // std::__vector_base<int>::~__vector_base
constexpr uint32_t ActFlagDeque_base_dtor = 0x5C37D9;    // std::__deque_base<FlashNode::ActFlag>
constexpr uint32_t IntDeque_base_dtor = 0x5C3805;        // std::__deque_base<int>
constexpr uint32_t CommandParser_dtor = 0x5C3831;
constexpr uint32_t Vec4_dtor = 0x8E6A69;
constexpr uint32_t Node_dtor = 0x8927ED;
constexpr uint32_t Vec2_ctor_ff = 0x8E4B9D;              // Vec2::Vec2(float, float)
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Vec2_add = 0x8E4D2D;                  // Vec2::add(const Vec2&)
constexpr uint32_t String_compare_cstr = 0x47CBD5;       // std::string::compare(const char*) (result unused)

constexpr uint32_t CommandParser_parse = 0x4F28F9;       // the member function bound to the LWF commands
constexpr uint32_t VT_BindCommandParser = 0xF8F52C;      // __func<__bind<&CommandParser::parse, ...>> + 8
constexpr uint32_t MOTION_LWF_PATHS = 0xF5D6A0;          // const char* [5]: "lwf/character/raid/..." per motion
constexpr uint32_t CLICK_POINT_NAMES = 0xFEF0BC;         // const char* [6]: the click point movie names

// __func<updateTargetPos()::$_N> vtables (+8) and type names
constexpr uint32_t VT_Load0 = 0xF5D740, VT_Load1 = 0xF5D714, VT_Load2 = 0xF5D6E8, VT_Load3 = 0xF5D6BC;
constexpr uint32_t TS_Load0 = 0xE4C3C0, TS_Load1 = 0xE4C320, TS_Load2 = 0xE4C280, TS_Load3 = 0xE4C1E0;
constexpr uint32_t TI_Load0 = 0xF91720, TI_Load1 = 0xF9170C, TI_Load2 = 0xF916F8, TI_Load3 = 0xF916E4;

constexpr uint32_t STR_char = 0xDFCDC1, STR_start = 0xE01812, STR_attack = 0xDFB8DD, STR_center = 0xE0181C;
constexpr uint32_t STR_Break = 0xE01FA8;
constexpr uint32_t STR_hitEffect = 0xE01FAE;             // "lwf/ability/hit_effect/eff_0002/eff_0002.lwf"

constexpr uint32_t LWFNODE_LWF = 992;                    // LWFNode -> LWF::LWF*
constexpr uint32_t LWF_COMMAND_HANDLER = 248;            // LWF: std::function for the movie commands
constexpr uint32_t MOVIE_PARENT = 8;                     // LWF::Movie parent movie
constexpr uint32_t MOVIE_NAME = 0x70;                    // LWF::Movie instance name (std::string)
constexpr uint32_t MOVIE_TOTAL_FRAMES = 156;
constexpr uint32_t NODE_REMOVE_CHILD = 300;              // Node::removeChild(child, cleanup)
constexpr uint32_t LWFNODE_UPDATE = 452;                 // LWFNode virtual (node, 0): advance/refresh

uint32_t lwfOf(const RaidBoss *self) { return ld32(self->lwfNode + LWFNODE_LWF); }

using bui::lwf::newStringBuffer;
using bui::lwf::addLoadHandler;

// Sum of a movie's position and its parents' (its position in the LWF root)
void addMovieChainPosition(uint32_t movie, uint32_t pos) {
    for (uint32_t p = movie; p; p = ld32(p + MOVIE_PARENT)) {
        stf(pos, rw::callf(Movie_GetX, {p}) + ldf(pos));
        stf(pos + 4, rw::callf(Movie_GetY, {p}) + ldf(pos + 4));
    }
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
RaidBoss *RaidBoss::create(int32_t bossId) {
    uint32_t p = call(guest::OPERATOR_NEW, {SIZE});
    construct(rw::host_ptr<RaidBoss>(p), bossId);
    call(Ref_autorelease, {p});
    return rw::host_ptr<RaidBoss>(p);
}
RW_FUNCTION(0x5D6684, "RaidBoss::create", RaidBoss::create);

RaidBoss *RaidBoss::construct(RaidBoss *self, int32_t bossId) {
    call(FlashNode_ctor, {rw::guest_addr(self), 0});
    self->vtable = VTABLE;
    self->bossId = bossId;
    self->scale = 1.0f;
    self->motion = 5;
    self->firstStart = 1;
    self->clickSize = 0;
    self->clickFirst = 0;
    self->clickBucketCount = 0;
    self->clickBuckets = 0;
    self->clickMaxLoad = 1.0f;
    self->attackSize = 0;
    self->attackFirst = 0;
    self->attackBucketCount = 0;
    self->attackBuckets = 0;
    self->attackMaxLoad = 1.0f;
    self->breakVisible = 0;
    self->pointEnabled[4] = 1;
    self->pointEnabled[3] = 1;
    self->pointEnabled[2] = 1;
    self->pointEnabled[1] = 1;
    self->pointEnabled[5] = 1;
    self->pointEnabled[0] = 0;
    changeMotion(self, 4);
    return self;
}
RW_FUNCTION(0x5D66C0, "RaidBoss::RaidBoss(int)", RaidBoss::construct);

RaidBoss *RaidBoss::destroy(RaidBoss *self) {
    uint32_t a = rw::guest_addr(self);
    self->vtable = VTABLE;
    call(NameSet_dtor, {a + 1344});
    call(ClickMap_dtor, {a + 1324});
    self->vtable = FlashNode::VTABLE;
    call(IntVector_base_dtor, {a + 1284});
    call(ActFlagDeque_base_dtor, {a + 1248});
    call(IntDeque_base_dtor, {a + 1224});
    call(CommandParser_dtor, {a + 672});
    call(Vec4_dtor, {a + 632});
    call(Node_dtor, {a});
    return self;
}
RW_FUNCTION(0x5D74B8, "RaidBoss::~RaidBoss", RaidBoss::destroy);

void RaidBoss::deletingDestroy(RaidBoss *self) {
    destroy(self);
    call(guest::OPERATOR_DELETE, {rw::guest_addr(self)});
}
RW_FUNCTION(0x5D7538, "RaidBoss::~RaidBoss (deleting)", RaidBoss::deletingDestroy);

// ------------------------------------------------------------------------------------------- motion
// Replace the LWF node by the motion's animation, hook its commands to the CommandParser, re-register the click
// point handlers and start the "char" movie. On the first idle (motion 4) after construction, click points
// that never loaded an "attack" movie are dropped.
void RaidBoss::changeMotion(RaidBoss *self, int32_t motion) {
    uint32_t a = rw::guest_addr(self);
    if (self->lwfNode) vcall(a, NODE_REMOVE_CHILD, {a, self->lwfNode, 1});
    {
        Frame f;
        uint32_t buf = newStringBuffer(f);
        call(StringBuffer_fset, {buf, ld32(MOTION_LWF_PATHS + 4 * uint32_t(motion)), uint32_t(self->bossId)});
        self->lwfNode = call(LWFNode_create, {buf, 0});
    }
    vcall(a, node::addChild, {a, self->lwfNode});
    self->motion = motion;
    {
        Frame f;
        uint32_t lwf = lwfOf(self);
        uint32_t fn = f.alloc(20);     // std::bind(&CommandParser::parse, &commandParser, _1, _2, _3)
        st32(fn, VT_BindCommandParser);
        st32(fn + 4, CommandParser_parse);
        st32(fn + 8, 0);
        st32(fn + 12, a + 672);
        st32(fn + 16, fn);
        call(MovieEventFunction_assign, {lwf + LWF_COMMAND_HANDLER, fn});
        functionDestroy(fn);
    }
    vcall(self->lwfNode, LWFNODE_UPDATE, {self->lwfNode, 0});
    updateTargetPos(self);
    {
        uint32_t lwf = lwfOf(self);
        Frame f;
        uint32_t s = newStr(f, STR_char, 4);
        uint32_t chr = call(LWF_SearchMovieInstance, {lwf, s});
        strFree(s);
        if (chr && ld32(chr + MOVIE_TOTAL_FRAMES) == 1) {
            uint32_t start = newStr(f, STR_start, 5);
            call(Movie_GotoAndPlay, {chr, start});
            strFree(start);
        }
    }
    vcall(self->lwfNode, LWFNODE_UPDATE, {self->lwfNode, 0});
    self->freezeOnStatus = 0;
    if (self->firstStart && self->motion == 4) {
        self->freezeOnStatus = 1;
        self->firstStart = 0;
        uint32_t node = self->clickFirst;
        while (node) {
            if (call(NameSet_find, {a + 1344, node + 8})) node = ld32(node);
            else node = call(ClickMap_erase, {a + 1324, node});
        }
    }
    call(FlashNode_change_sub, {a});
}
RW_FUNCTION(0x5D6814, "RaidBoss::changeMotion", RaidBoss::changeMotion);

// Register "load" handlers on the click point movies ($_0: record the position), "attack" ($_1: offset the
// click point by the attack movie, remember the part), "center" ($_2: nothing) and "Break" ($_3: nothing).
void RaidBoss::updateTargetPos(RaidBoss *self) {
    uint32_t a = rw::guest_addr(self);
    call(ClickMap_clear, {a + CLICK_POINTS_OFFSET});
    for (uint32_t k = 0; k < 6; k++) {
        uint32_t lwf = lwfOf(self);
        uint32_t nameText = ld32(CLICK_POINT_NAMES + 4 * k);
        Frame f;
        uint32_t name = newStr(f, nameText);
        addLoadHandler(lwf, name, VT_Load0, {a, ld32(CLICK_POINT_NAMES + 4 * k)});
        strFree(name);
    }
    struct { uint32_t text, len, vtable; } fixed[] = {
        {STR_attack, 6, VT_Load1}, {STR_center, 6, VT_Load2}, {STR_Break, 5, VT_Load3}};
    for (auto &h : fixed) {
        uint32_t lwf = lwfOf(self);
        Frame f;
        uint32_t name = newStr(f, h.text, h.len);
        addLoadHandler(lwf, name, h.vtable, {a});
        strFree(name);
    }
}
RW_FUNCTION(0x5D6AC0, "RaidBoss::updateTargetPos", RaidBoss::updateTargetPos);

// Every frame: rebuild clickPoints from the click point movies under "char" (plus their "attack" child), move the
// effect armature to the first point and show each point's "Break" movie when it is enabled.
void RaidBoss::update(RaidBoss *self, float dt) {
    uint32_t a = rw::guest_addr(self);
    call(FlashNode_update, {a, rw::fbits(dt)});
    uint32_t lwf = lwfOf(self);
    uint32_t chr;
    {
        Frame f;
        uint32_t s = newStr(f, STR_char, 4);
        chr = call(LWF_SearchMovieInstance, {lwf, s});
        strFree(s);
    }
    if (!chr) return;
    call(ClickMap_clear, {a + CLICK_POINTS_OFFSET});
    for (uint32_t k = 0; k < 6; k++) {
        Frame f;
        uint32_t point;
        {
            uint32_t s = newStr(f, ld32(CLICK_POINT_NAMES + 4 * k));
            point = call(Movie_SearchMovieInstance, {chr, s, 1});
            strFree(s);
        }
        if (!point) continue;
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor_ff, {pos, 0, 0});
        addMovieChainPosition(point, pos);
        {
            uint32_t s = newStr(f, STR_attack, 6);
            uint32_t attack = call(Movie_SearchMovieInstance, {point, s, 1});
            strFree(s);
            if (attack) {
                stf(pos, rw::callf(Movie_GetX, {attack}) + ldf(pos));
                stf(pos + 4, rw::callf(Movie_GetY, {attack}) + ldf(pos + 4));
            }
        }
        if (k == 0) {
            uint32_t arm = self->effectArmature;
            vcall(arm, node::setPositionXY, {arm, ld32(pos), ld32(pos + 4) ^ 0x80000000u});
        }
        float scale = self->scale;
        stf(pos, scale * ldf(pos));
        stf(pos + 4, scale * ldf(pos + 4));
        uint32_t entry = f.alloc(12);                   // pair<const char*, Vec2>
        st32(entry, ld32(CLICK_POINT_NAMES + 4 * k));
        call(Vec2_copy, {entry + 4, pos});
        uint32_t out = f.alloc(8);
        call(ClickMap_insert_unique, {out, a + CLICK_POINTS_OFFSET, entry});
        call(Vec2_dtor, {entry + 4});
        {
            uint32_t s = newStr(f, STR_Break, 5);
            uint32_t brk = call(Movie_SearchMovieInstance, {point, s, 1});
            strFree(s);
            if (brk) {
                bool show = self->pointEnabled[k] != 0 && self->breakVisible != 0;
                call(Movie_SetVisible, {brk, show ? 1u : 0u});
            }
        }
        call(Vec2_dtor, {pos});
    }
}
RW_FUNCTION(0x5D7090, "RaidBoss::update", RaidBoss::update);

uint32_t RaidBoss::getClickPoints(RaidBoss *self) { return rw::guest_addr(self) + CLICK_POINTS_OFFSET; }
RW_FUNCTION(0x5D7384, "RaidBoss::getClickPoints", RaidBoss::getClickPoints);

void RaidBoss::preloadResources(const RaidBoss *self) {
    Frame f;
    uint32_t buf = newStringBuffer(f);
    for (uint32_t k = 0; k < 5; k++) {
        call(StringBuffer_fset, {buf, ld32(MOTION_LWF_PATHS + 4 * k), uint32_t(self->bossId)});
        Frame g;
        uint32_t s = newStr(g, buf);
        call(LWFUtil_preloadResources, {s});
        strFree(s);
    }
    uint32_t s = newStr(f, STR_hitEffect, 0x2C);
    call(LWFUtil_preloadResources, {s});
    strFree(s);
}
RW_FUNCTION(0x5D7390, "RaidBoss::preloadResources", RaidBoss::preloadResources);

// ------------------------------------------------------------------------------------------- lambdas
// std::function<void(LWF::Movie*)> handlers of updateTargetPos. Captures: $_0 (boss, point name), others (boss).
namespace RaidBossLambdas {
using namespace bui::thunk;

// $_0: a click point movie loaded - record its root position (scaled) under its name
void load0Call(uint32_t self, uint32_t movieRef) {
    uint32_t movie = ld32(movieRef);
    const RaidBoss *boss = rw::host_ptr<RaidBoss>(ld32(self + 4));
    Frame f;
    uint32_t pos = f.alloc(8);
    call(Vec2_ctor_ff, {pos, 0, 0});
    addMovieChainPosition(movie, pos);
    float scale = boss->scale;
    float x = ldf(pos) * scale;
    float y = ldf(pos + 4) * scale;
    uint32_t scaled = f.alloc(8);
    call(Vec2_ctor_ff, {scaled, rw::fbits(x), rw::fbits(y)});
    uint32_t entry = f.alloc(12);
    st32(entry, ld32(self + 8));
    call(Vec2_copy, {entry + 4, scaled});
    uint32_t out = f.alloc(8);
    call(ClickMap_insert_unique, {out, rw::guest_addr(boss) + RaidBoss::CLICK_POINTS_OFFSET, entry});
    call(Vec2_dtor, {entry + 4});
    call(Vec2_dtor, {scaled});
    call(Vec2_dtor, {pos});
}

// $_1: an "attack" movie loaded - move its parent's click point by the movie's scaled offset and remember the part
void load1Call(uint32_t self, uint32_t movieRef) {
    uint32_t movie = ld32(movieRef);
    uint32_t boss = ld32(self + 4);
    uint32_t node = call(ClickMap_find, {boss + RaidBoss::CLICK_POINTS_OFFSET, ld32(movie + MOVIE_PARENT) + MOVIE_NAME});
    if (!node) return;
    uint32_t value = node + 20;
    Frame f;
    uint32_t v = f.alloc(8);
    call(Vec2_copy, {v, value});
    float mx = rw::callf(Movie_GetX, {movie});
    float scale = rw::host_ptr<RaidBoss>(boss)->scale;
    float my = rw::callf(Movie_GetY, {movie});
    uint32_t t = f.alloc(8);
    call(Vec2_ctor_ff, {t, rw::fbits(mx * scale), rw::fbits(my * scale)});
    call(Vec2_add, {v, t});
    call(Vec2_dtor, {t});
    stf(v, ldf(v) - rw::callf(Movie_GetX, {movie}));
    float y = ldf(v + 4) - rw::callf(Movie_GetY, {movie});
    stf(v + 4, y);
    rw::save<uint32_t>(value, ld32(v));
    rw::save<float>(value + 4, y);
    uint32_t out = f.alloc(8);
    call(NameSet_insert_unique, {out, boss + 1344, ld32(movie + MOVIE_PARENT) + MOVIE_NAME});
    call(Vec2_dtor, {v});
}

// $_2 ("center"): nothing
void load2Call(uint32_t, uint32_t) {}

// $_3 ("Break"): compares the parent's name with the click point names 1-5; the results are unused
void load3Call(uint32_t, uint32_t movieRef) {
    uint32_t movie = ld32(movieRef);
    for (uint32_t k = 1; k < 6; k++)
        call(String_compare_cstr, {ld32(movie + MOVIE_PARENT) + MOVIE_NAME, ld32(CLICK_POINT_NAMES + 4 * k)});
}

void funcDelete(uint32_t self) { deleteSelf(self); }
void funcNop(uint32_t) {}
uint32_t load0Clone(uint32_t self) { return cloneNew(self, VT_Load0, 12, 2); }
void load0CloneInto(uint32_t self, uint32_t dst) { cloneInto(self, dst, VT_Load0, 2); }
uint32_t load0Target(uint32_t self, uint32_t ti) { return target(self, ti, TS_Load0); }
uint32_t load0TargetType(uint32_t) { return TI_Load0; }
uint32_t load1Clone(uint32_t self) { return cloneNew(self, VT_Load1, 8, 1); }
void load1CloneInto(uint32_t self, uint32_t dst) { cloneInto(self, dst, VT_Load1, 1); }
uint32_t load1Target(uint32_t self, uint32_t ti) { return target(self, ti, TS_Load1); }
uint32_t load1TargetType(uint32_t) { return TI_Load1; }
uint32_t load2Clone(uint32_t self) { return cloneNew(self, VT_Load2, 8, 1); }
void load2CloneInto(uint32_t self, uint32_t dst) { cloneInto(self, dst, VT_Load2, 1); }
uint32_t load2Target(uint32_t self, uint32_t ti) { return target(self, ti, TS_Load2); }
uint32_t load2TargetType(uint32_t) { return TI_Load2; }
uint32_t load3Clone(uint32_t self) { return cloneNew(self, VT_Load3, 8, 1); }
void load3CloneInto(uint32_t self, uint32_t dst) { cloneInto(self, dst, VT_Load3, 1); }
uint32_t load3Target(uint32_t self, uint32_t ti) { return target(self, ti, TS_Load3); }
uint32_t load3TargetType(uint32_t) { return TI_Load3; }
}  // namespace RaidBossLambdas

using namespace RaidBossLambdas;
// $_3 (Break)
RW_FUNCTION(0x5D782E, "RaidBoss::updateTargetPos $_3 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x5D7838, "RaidBoss::updateTargetPos $_3 __clone()", load3Clone);
RW_FUNCTION(0x5D7864, "RaidBoss::updateTargetPos $_3 __clone(dst)", load3CloneInto);
RW_FUNCTION(0x5D7884, "RaidBoss::updateTargetPos $_3 destroy", funcNop);
RW_FUNCTION(0x5D7886, "RaidBoss::updateTargetPos $_3 destroy_deallocate", funcDelete);
RW_FUNCTION(0x5D7890, "RaidBoss::updateTargetPos $_3 operator()", load3Call);
RW_FUNCTION(0x5D78C4, "RaidBoss::updateTargetPos $_3 target", load3Target);
RW_FUNCTION(0x5D78E4, "RaidBoss::updateTargetPos $_3 target_type", load3TargetType);
// $_2 (center)
RW_FUNCTION(0x5D79F4, "RaidBoss::updateTargetPos $_2 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x5D79FC, "RaidBoss::updateTargetPos $_2 __clone()", load2Clone);
RW_FUNCTION(0x5D7A28, "RaidBoss::updateTargetPos $_2 __clone(dst)", load2CloneInto);
RW_FUNCTION(0x5D7A48, "RaidBoss::updateTargetPos $_2 destroy", funcNop);
RW_FUNCTION(0x5D7A4A, "RaidBoss::updateTargetPos $_2 destroy_deallocate", funcDelete);
RW_FUNCTION(0x5D7A52, "RaidBoss::updateTargetPos $_2 operator()", load2Call);
RW_FUNCTION(0x5D7A54, "RaidBoss::updateTargetPos $_2 target", load2Target);
RW_FUNCTION(0x5D7A74, "RaidBoss::updateTargetPos $_2 target_type", load2TargetType);
// $_1 (attack)
RW_FUNCTION(0x5D7A88, "RaidBoss::updateTargetPos $_1 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x5D7A90, "RaidBoss::updateTargetPos $_1 __clone()", load1Clone);
RW_FUNCTION(0x5D7ABC, "RaidBoss::updateTargetPos $_1 __clone(dst)", load1CloneInto);
RW_FUNCTION(0x5D7ADC, "RaidBoss::updateTargetPos $_1 destroy", funcNop);
RW_FUNCTION(0x5D7ADE, "RaidBoss::updateTargetPos $_1 destroy_deallocate", funcDelete);
RW_FUNCTION(0x5D7AE8, "RaidBoss::updateTargetPos $_1 operator()", load1Call);
RW_FUNCTION(0x5D7C14, "RaidBoss::updateTargetPos $_1 target", load1Target);
RW_FUNCTION(0x5D7C34, "RaidBoss::updateTargetPos $_1 target_type", load1TargetType);
// $_0 (click point)
RW_FUNCTION(0x5D7C48, "RaidBoss::updateTargetPos $_0 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x5D7C50, "RaidBoss::updateTargetPos $_0 __clone()", load0Clone);
RW_FUNCTION(0x5D7C80, "RaidBoss::updateTargetPos $_0 __clone(dst)", load0CloneInto);
RW_FUNCTION(0x5D7CA4, "RaidBoss::updateTargetPos $_0 destroy", funcNop);
RW_FUNCTION(0x5D7CA6, "RaidBoss::updateTargetPos $_0 destroy_deallocate", funcDelete);
RW_FUNCTION(0x5D7CB0, "RaidBoss::updateTargetPos $_0 operator()", load0Call);
RW_FUNCTION(0x5D7DC4, "RaidBoss::updateTargetPos $_0 target", load0Target);
RW_FUNCTION(0x5D7DE4, "RaidBoss::updateTargetPos $_0 target_type", load0TargetType);
