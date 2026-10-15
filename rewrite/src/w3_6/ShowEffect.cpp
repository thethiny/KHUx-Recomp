// ShowEffect: the LWF effect played when an enemy appears on (show) or leaves (hide) the field
// (lwf/character/enemy/show_effect_fla/show_effect<type>). A cocos2d::Node holding the LWFNode; update() starts the
// in part (frame 1) or the out part (after the current frame) when show()/hide() asked for it.
#include "Common.hpp"

using namespace w36;

struct ShowEffect {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Node
    const uint8_t pad_004[0x26C];          // +4   cocos2d::Node fields (_onEnterCallback +0x210, _onExitCallback +0x228)
    rw::Field<int32_t> type;               // +624 show_effect<type>
    rw::Field<uint32_t> lwfNode;           // +628 cocos2d::LWFNode*
    rw::Field<uint8_t> showRequested;      // +632
    rw::Field<uint8_t> hideRequested;      // +633
    const uint8_t pad_27a[2];              // +634
    rw::Field<int32_t> playing;            // +636 set to 1 when a part starts
    const uint8_t pad_280[4];              // +640
    rw::Field<int32_t> state;              // +644 3 idle (hidden), 0 showing, 1 shown, 2 hiding
    const uint8_t pad_288[0x30];           // +648 two std::function members (+648 / +672, callable at +664 / +688)

    static constexpr uint32_t VTABLE = 0xF91730;     // _ZTV10ShowEffect + 8
    static constexpr uint32_t SIZE = 0x2B8;
    static constexpr uint32_t FUNC_A = 0x288, FUNC_B = 0x2A0;   // the std::function members
    static constexpr uint32_t ON_ENTER_CALLBACK = 0x210, ON_EXIT_CALLBACK = 0x228;
    static constexpr uint32_t STATE_IDLE = 3, STATE_SHOWING = 0, STATE_SHOWN = 1, STATE_HIDING = 2;

    static ShowEffect *create(int32_t type);
    static ShowEffect *construct(ShowEffect *self, int32_t type);
    static void show(ShowEffect *self);
    static void hide(ShowEffect *self);
    static void update(ShowEffect *self, float dt);
    static void preloadResources(ShowEffect *self);
    static ShowEffect *destruct(ShowEffect *self);
    static void destructDelete(ShowEffect *self);
};
RW_LAYOUT(ShowEffect, type, 624);
RW_LAYOUT(ShowEffect, state, 644);
RW_SIZE(ShowEffect, 0x2B8);

namespace {
using rw::guest_addr;
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Node_dtor = 0x8927ED;
constexpr uint32_t S_LWF_PATH = 0xE020C2;   // "lwf/character/enemy/show_effect_fla/show_effect%d/show_effect%d.lwf"
constexpr uint32_t S_ROOT = 0xDFE0ED;       // "_root"
constexpr uint32_t S_EFFECTS = 0xE020B1;    // "effects"
constexpr uint32_t S_EFFECTS2 = 0xE020B9;   // "effects2"
// lambdas (std::function __func vtables)
constexpr uint32_t VT_ENTER_FRAME = 0xF5D81C;   // $_0 [this]: _root enterFrame
constexpr uint32_t VT_EFFECTS_LOAD = 0xF5D7F0;  // $_1: "effects" load: stop at frame 1
constexpr uint32_t VT_EFFECTS2_LOAD = 0xF5D7C4; // $_2: "effects2" load: stop at the last frame
constexpr uint32_t VT_ON_ENTER = 0xF5D798;      // $_3 [this]: scheduleUpdate
constexpr uint32_t VT_ON_EXIT = 0xF5D76C;       // $_4 [this]: unscheduleUpdate

uint32_t rootLwf(const ShowEffect *self) { return ld32(self->lwfNode + wfn::LWFNODE_LWF); }
void destroyFunctionAt(uint32_t fo) {
    uint32_t p = ld32(fo + 16);
    if (p == fo) vcall(p, 0x10, {p});
    else if (p) vcall(p, 0x14, {p});
}
}  // namespace

ShowEffect *ShowEffect::create(int32_t type) {
    uint32_t p = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<ShowEffect>(p), type);
    call(sfn::Ref_autorelease, {p});
    return rw::host_ptr<ShowEffect>(p);
}
RW_FUNCTION(0x5D84A4, "ShowEffect::create", ShowEffect::create);

ShowEffect *ShowEffect::construct(ShowEffect *self, int32_t type) {
    const uint32_t me = guest_addr(self);
    call(Node_ctor, {me});
    self->vtable = VTABLE;
    self->type = type;
    self->lwfNode = 0;
    self->showRequested = 0;
    self->hideRequested = 0;
    self->state = STATE_IDLE;
    st32(me + FUNC_A + 16, 0);
    st32(me + FUNC_B + 16, 0);
    {
        Frame f;
        uint32_t path = newStringBuffer(f);
        call(wfn::StringBuffer_fset, {path, S_LWF_PATH, uint32_t(type), uint32_t(type)});
        uint32_t node = call(wfn::LWFNode_create, {path, 0});
        self->lwfNode = node;
        vcall(me, node::addChild, {me, node});
    }
    addMovieHandler(rootLwf(self), S_ROOT, 5, "enterFrame", VT_ENTER_FRAME, {me});
    addMovieHandler(rootLwf(self), S_EFFECTS, 7, "load", VT_EFFECTS_LOAD, {});
    addMovieHandler(rootLwf(self), S_EFFECTS2, 8, "load", VT_EFFECTS2_LOAD, {});
    uint32_t node = self->lwfNode;
    vcall(node, node::setVisible, {node, 0});
    assignLambda(me + ON_ENTER_CALLBACK, VT_ON_ENTER, {me});
    assignLambda(me + ON_EXIT_CALLBACK, VT_ON_EXIT, {me});
    return self;
}
RW_FUNCTION(0x5D84E0, "ShowEffect::ShowEffect(int)", ShowEffect::construct);

void ShowEffect::show(ShowEffect *self) {
    self->hideRequested = 0;
    self->showRequested = 1;
}
RW_FUNCTION(0x5D8ADC, "ShowEffect::show", ShowEffect::show);

void ShowEffect::hide(ShowEffect *self) {
    self->hideRequested = 1;
    self->showRequested = 0;
}
RW_FUNCTION(0x5D8AF0, "ShowEffect::hide", ShowEffect::hide);

void ShowEffect::update(ShowEffect *self, float) {
    if (self->showRequested && self->state == int32_t(STATE_IDLE)) {
        self->showRequested = 0;
        self->state = STATE_SHOWING;
        self->playing = 1;
        call(wfn::Movie_GotoAndPlay, {ld32(rootLwf(self) + wfn::LWF_ROOT_MOVIE), 1});
    }
    if (self->hideRequested && self->state == int32_t(STATE_SHOWN)) {
        self->hideRequested = 0;
        self->state = STATE_HIDING;
        self->playing = 1;
        uint32_t movie = ld32(rootLwf(self) + wfn::LWF_ROOT_MOVIE);
        call(wfn::Movie_GotoAndPlay, {movie, ld32(movie + wfn::MOVIE_CURRENT_FRAME) + 1});
    }
}
RW_FUNCTION(0x5D8B04, "ShowEffect::update", ShowEffect::update);

void ShowEffect::preloadResources(ShowEffect *self) {
    Frame f;
    uint32_t path = newStringBuffer(f);
    call(wfn::StringBuffer_fset, {path, S_LWF_PATH, uint32_t(self->type), uint32_t(self->type)});
    uint32_t len = call(wfn::STRLEN, {path});
    withStr(path, len, [&](uint32_t s) { call(wfn::LWFUtil_preloadResources, {s}); });
}
RW_FUNCTION(0x5D8B88, "ShowEffect::preloadResources", ShowEffect::preloadResources);

ShowEffect *ShowEffect::destruct(ShowEffect *self) {
    const uint32_t me = guest_addr(self);
    self->vtable = VTABLE;
    destroyFunctionAt(me + FUNC_B);
    destroyFunctionAt(me + FUNC_A);
    call(Node_dtor, {me});
    return self;
}
RW_FUNCTION(0x5D8C44, "ShowEffect::~ShowEffect", ShowEffect::destruct);

void ShowEffect::destructDelete(ShowEffect *self) {
    destruct(self);
    call(fn::operator_delete, {guest_addr(self)});
}
RW_FUNCTION(0x5D8CB0, "ShowEffect::~ShowEffect (deleting)", ShowEffect::destructDelete);

// ---------------------------------------------------------------------------------------- lambda bodies
// $_4 (onExit): unscheduleUpdate()
static void onExitLambda(uint32_t func) { call(wfn::Node_unscheduleUpdate, {ld32(func + 4)}); }
RW_FUNCTION(0x5D8D7E, "ShowEffect::ShowEffect(int) $_4 __func::operator()", onExitLambda);

// $_3 (onEnter): scheduleUpdate()
static void onEnterLambda(uint32_t func) { call(wfn::Node_scheduleUpdate, {ld32(func + 4)}); }
RW_FUNCTION(0x5D8E1A, "ShowEffect::ShowEffect(int) $_3 __func::operator()", onEnterLambda);

// $_2 ("effects2" load): stop at the movie's last frame
static void effects2LoadLambda(uint32_t, const uint32_t *movieArg) {
    uint32_t movie = *movieArg;
    call(wfn::Movie_GotoAndStop, {movie, ld32(movie + wfn::MOVIE_TOTAL_FRAMES)});
}
RW_FUNCTION(0x5D8EAA, "ShowEffect::ShowEffect(int) $_2 __func::operator()", effects2LoadLambda);

// $_1 ("effects" load): stop at frame 1
static void effectsLoadLambda(uint32_t, const uint32_t *movieArg) { call(wfn::Movie_GotoAndStop, {*movieArg, 1}); }
RW_FUNCTION(0x5D8F3E, "ShowEffect::ShowEffect(int) $_1 __func::operator()", effectsLoadLambda);
