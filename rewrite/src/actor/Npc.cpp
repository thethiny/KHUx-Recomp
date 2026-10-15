#include "Npc.hpp"

#include "FunctionThunks.hpp"

namespace {
namespace g {
constexpr uint32_t fnVoid_swap = 0x4EE86D;           // std::function<void()>::swap(function&)
constexpr uint32_t dequeActFlag_pushBack = 0x5C40C9; // std::deque<FlashNode::ActFlag>::push_back(ActFlag&&)
constexpr uint32_t motionFiles = 0xF5D104;           // const char*[1]: "lwf/character/npc/%d/wait/wait.lwf"
constexpr uint32_t str_icon = 0xE01938;              // "icon"
constexpr uint32_t motionEndLambda = 0xF5D110;       // Npc::Npc $_0 (captures this)
}  // namespace g

uint32_t ga(const void *p) { return rw::guest_addr(p); }
}  // namespace

Npc *Npc::create(int32_t npcId) {
    auto *self = rw::host_ptr<Npc>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self, npcId);
    rw::call(actor_fn::ref_autorelease, {ga(self)});
    return self;
}
RW_FUNCTION(0x5CC964, "Npc::create", Npc::create);

Npc *Npc::construct(Npc *self, int32_t npcId) {
    FlashNode::construct(self, 0);
    self->vtable = VTABLE;
    self->npcId = npcId;
    self->motion = 1;
    self->firstLoad = 1;
    self->field_528.x = 0.0f;                                // Vec2()
    self->field_528.y = 0.0f;
    self->iconPoint.x = 0.0f;
    self->iconPoint.y = 0.0f;
    bt::GuestFrame f(24);
    fnthunk::makeLocal(f.addr(0), g::motionEndLambda, {ga(self)});
    rw::call(g::fnVoid_swap, {f.addr(0), ga(self) + CMD_MOTION_END});
    fnthunk::destroy(f.addr(0));
    changeMotion(self, 0);
    return self;
}
RW_FUNCTION(0x5CC9A0, "Npc::Npc", Npc::construct);

// Replace the LWF node with the motion's file, hook the script commands, start the movie, and record the icon
// point on the first wait load.
void Npc::changeMotion(Npc *self, int32_t m) {
    if (self->lwfNode) rw::vcall(self, flashlwf::NODE_REMOVE_CHILD, {ga(self), self->lwfNode, 1});
    enum : uint32_t { STR = 0, FN = 0x10, SB = 0x28 };
    bt::GuestFrame f(0x430);
    rw::save<uint32_t>(f.addr(SB) + 1024, 0);                // StringBuffer: empty
    rw::save<uint8_t>(f.addr(SB), 0);
    rw::call(flashlwf::stringBuffer_fset,
             {f.addr(SB), actor::ld(g::motionFiles + uint32_t(m) * 4), uint32_t(int32_t(self->npcId))});
    self->lwfNode = rw::call(flashlwf::lwfNode_create, {f.addr(SB), 0});
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), self->lwfNode});
    self->motion = m;
    uint32_t lwf = actor::ld(self->lwfNode + LWF_NODE_LWF);
    fnthunk::makeLocal(f.addr(FN), flashlwf::commandParserBind, {flashlwf::commandParser_parse, 0, ga(self) + 672});
    rw::call(flashlwf::lwfHandler_assign, {lwf + flashlwf::LWF_HANDLER, f.addr(FN)});
    fnthunk::destroy(f.addr(FN));
    actor::vcallAt(self->lwfNode, flashlwf::NODE_UPDATE, {self->lwfNode, 0});

    lwf = actor::ld(self->lwfNode + LWF_NODE_LWF);
    actor::makeString(f.addr(STR), flashlwf::STR_CHAR, 4);
    uint32_t movie = rw::call(flashlwf::lwf_searchMovieInstance, {lwf, f.addr(STR)});
    actor::freeString(f.addr(STR));
    if (movie && actor::ld(movie + flashlwf::MOVIE_FRAME) == 1) {
        actor::makeString(f.addr(STR), flashlwf::STR_START, 5);
        rw::call(flashlwf::movie_gotoAndPlay, {movie, f.addr(STR)});
        actor::freeString(f.addr(STR));
    }
    self->iconPoint.y = actor::ldf(actor_fn::vec2_ZERO + 4);
    self->iconPoint.x = actor::ldf(actor_fn::vec2_ZERO);
    actor::vcallAt(self->lwfNode, flashlwf::NODE_UPDATE, {self->lwfNode, 0});
    if (self->firstLoad == 0 || self->motion != 0) return;
    self->firstLoad = 0;
    if (!movie) return;
    actor::makeString(f.addr(STR), g::str_icon, 4);
    uint32_t icon = rw::call(flashlwf::movie_searchMovieInstance, {movie, f.addr(STR), 1});
    actor::freeString(f.addr(STR));
    if (!icon) return;
    uint32_t xb = rw::call(flashlwf::movie_getX, {icon});
    uint32_t yb = rw::call(flashlwf::movie_getY, {icon});
    self->iconPoint.x = rw::bitsf(xb);
    self->iconPoint.y = rw::bitsf(yb ^ 0x80000000u);
}
RW_FUNCTION(0x5CCAF4, "Npc::changeMotion", Npc::changeMotion);

void Npc::preloadResources(const Npc *self) {
    enum : uint32_t { STR = 0, SB = 0x10 };
    bt::GuestFrame f(0x420);
    rw::save<uint32_t>(f.addr(SB) + 1024, 0);
    rw::save<uint8_t>(f.addr(SB), 0);
    rw::call(flashlwf::stringBuffer_fset, {f.addr(SB), actor::ld(g::motionFiles), uint32_t(int32_t(self->npcId))});
    actor::makeString(f.addr(STR), f.addr(SB), rw::call(actor_fn::strlen_, {f.addr(SB)}));
    rw::call(flashlwf::lwfUtil_preload, {f.addr(STR)});
    actor::freeString(f.addr(STR));
}
RW_FUNCTION(0x5CCDEC, "Npc::preloadResources", Npc::preloadResources);

Npc *Npc::destruct(Npc *self) {
    self->vtable = VTABLE;                                   // (the Vec2 destructors are empty)
    FlashNode::destruct(self);
    return self;
}
RW_FUNCTION(0x5CCECC, "Npc::~Npc", Npc::destruct);

void Npc::destructDelete(Npc *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5CCF4C, "Npc::~Npc (deleting)", Npc::destructDelete);

// The constructor's motion-end command: ActFlag 5.
void Npc::motionEndLambda(uint32_t lambda) {
    bt::GuestFrame f(8);
    rw::save<uint32_t>(f.addr(0), 5);
    rw::call(g::dequeActFlag_pushBack, {actor::ld(lambda + 4) + 1248, f.addr(0)});
}

RW_FUNCTION(0x5CCFD0, "Npc::Npc $_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5CCFD8, "Npc::Npc $_0 __func::__clone()", (fnthunk::cloneWords<g::motionEndLambda, 8, 1>));
RW_FUNCTION(0x5CD004, "Npc::Npc $_0 __func::__clone(__base*)", fnthunk::cloneInto1<g::motionEndLambda>);
RW_FUNCTION(0x5CD024, "Npc::Npc $_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5CD026, "Npc::Npc $_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5CD030, "Npc::Npc $_0 (motion end: ActFlag 5)", Npc::motionEndLambda);
RW_FUNCTION(0x5CD07C, "Npc::Npc $_0 __func::target", fnthunk::target<0xE4A9D4>);
RW_FUNCTION(0x5CD09C, "Npc::Npc $_0 __func::target_type", fnthunk::targetType<0xF8FDE0>);
