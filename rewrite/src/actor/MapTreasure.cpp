#include "MapTreasure.hpp"

#include "battle/MapPrizes.hpp"

namespace {
namespace g {
constexpr uint32_t vecPrizes_clear = 0x5DB615;       // cocos2d::Vector<MapPrizes*>::clear()
constexpr uint32_t vecPrizes_dtor = 0x5DB649;        // cocos2d::Vector<MapPrizes*>::~Vector()
constexpr uint32_t vecPrizes_pushBack = 0x5DB985;    // cocos2d::Vector<MapPrizes*>::pushBack(MapPrizes*)
constexpr uint32_t getTreasurePrizes = 0x763DF9;     // StageUtil::getTreasurePrizes(MapPrizes::Items*, int, int, PlayStyle)
constexpr uint32_t function_call = 0x5DF629;         // std::function<void(MapTreasure*)>::operator()(MapTreasure*)
constexpr uint32_t function_assign = 0x5DF79D;       // std::function<void(MapTreasure*)>::operator=(const function&)
constexpr uint32_t string_compare = 0x47CBD5;        // std::string::compare(const char*) (sub_47CBD4)
constexpr uint32_t str_boxAnim = 0xE025A2;           // "ActMapTreasureBoxAnim" (21)
constexpr uint32_t str_boxJson = 0xE02445;           // "cocostudio/publish/ActMapTreasureBoxAnim.ExportJson" (51)
constexpr uint32_t str_open = 0xE02440;              // "Open"
constexpr uint32_t animNames = 0xF5DA20;             // const char*[3][3]: Treasure_N, TreasureHP_N, ... (closed)
constexpr uint32_t animNamesOpen = 0xF5D9FC;         // the same, "_Open"
constexpr uint32_t SE_OPEN = 0x1DEBE9D0;             // audio::ResourceID of the chest sound
// init()'s lambdas (std::function __func vtables, typeinfo, type names)
constexpr uint32_t movementLambda = 0xF5DA78;        // $_0: movement event, empty
constexpr uint32_t movementLambdaTI = 0xF93484;
constexpr uint32_t movementLambdaTS = 0xE4D120;
constexpr uint32_t frameLambda = 0xF5DA4C;           // $_1: frame event, captures this
constexpr uint32_t frameLambdaTI = 0xF93470;
constexpr uint32_t frameLambdaTS = 0xE4CFE0;
}  // namespace g

using actor::fb;
uint32_t ga(const void *p) { return rw::guest_addr(p); }
}  // namespace

MapTreasure *MapTreasure::construct(MapTreasure *self) {
    MapNode::construct(self);
    self->vtable = VTABLE;
    SafeBuffer::construct(&self->property, 8, 1);
    self->property.vtable = Property::VTABLE;
    self->onOpened.f = 0;
    rw::call(actor_fn::aeabi_memset, {ga(&self->armature), 0x14, 0});   // armature, prizes, state, +700
    return self;
}
RW_FUNCTION(0x5DF4AC, "MapTreasure::MapTreasure", MapTreasure::construct);

MapTreasure *MapTreasure::destruct(MapTreasure *self) {
    self->vtable = VTABLE;
    rw::call(actor_fn::node_removeAllChildrenWithCleanup, {ga(self), 1});
    rw::call(g::vecPrizes_clear, {ga(&self->prizes)});
    fnthunk::destroy(ga(&self->onOpened));                   // ~std::function
    rw::call(g::vecPrizes_dtor, {ga(&self->prizes)});
    SafeBuffer::destruct(&self->property);
    MapNode::destruct(self);
    return self;
}
RW_FUNCTION(0x5DEFC8, "MapTreasure::~MapTreasure", MapTreasure::destruct);

void MapTreasure::destructDelete(MapTreasure *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DF08C, "MapTreasure::~MapTreasure (deleting)", MapTreasure::destructDelete);

void MapTreasure::Property::destructDelete(Property *self) {
    SafeBuffer::destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DFA84, "MapTreasure::Property::~Property (deleting)", MapTreasure::Property::destructDelete);

// Build the chest: armature paused on the first frame of its (closed or opened) animation, ids in the property,
// the prizes rolled now into a hidden MapPrizes burst (unless already opened).
uint32_t MapTreasure::init(MapTreasure *self, const Info *info, uint32_t parent, int32_t fromStageType) {
    if (MapNode::init(self) != 1) return 0;
    rw::vcall(self, node_slot::SET_TAG, {ga(self), uint32_t(NODE_TAG)});

    enum : uint32_t { STR = 0, MOVEMENT_FN = 0x10, FRAME_FN = 0x28, PRIZE_INFO = 0x40, POS = 0x90 };
    bt::GuestFrame f(0xA0);
    actor::makeString(f.addr(STR), g::str_boxAnim, 21);
    self->armature = rw::call(actor_fn::armature_create, {f.addr(STR)});   // cocostudio::Armature::create
    actor::freeString(f.addr(STR));
    if (!self->armature) return 0;

    SafeBuffer::write(&self->property, self->property.data.get() + Property::TREASURE_ID,
                      rw::host_ptr<uint8_t>(ga(&info->treasureId)), 4, Property::TREASURE_ID);
    SafeBuffer::write(&self->property, self->property.data.get() + Property::REWARD_ID,
                      rw::host_ptr<uint8_t>(ga(&info->rewardId)), 4, Property::REWARD_ID);
    int32_t boxType = info->boxType;
    if ((boxType & 1) == 0) {
        rw::vcall(self, node_slot::SET_SCALE_X, {ga(self), fb(-1.0f)});   // even types face the other way
        boxType = info->boxType;
    }
    uint32_t anim = actor::vcallAt(self->armature, cs_slot::ARMATURE_GET_ANIMATION, {self->armature});
    uint32_t table = info->alreadyOpened ? g::animNamesOpen : g::animNames;
    uint32_t row = uint32_t((boxType % 6) >> 1), col = uint32_t(boxType / 6);
    uint32_t name = actor::ld(table + row * 12 + col * 4);
    actor::makeString(f.addr(STR), name, rw::call(actor_fn::strlen_, {name}));
    actor::vcallAt(anim, cs_slot::ANIM_PLAY, {anim, f.addr(STR), 0xFFFFFFFFu, 0xFFFFFFFFu});
    actor::freeString(f.addr(STR));
    anim = actor::vcallAt(self->armature, cs_slot::ARMATURE_GET_ANIMATION, {self->armature});
    actor::vcallAt(anim, cs_slot::ANIM_GOTO_AND_PAUSE, {anim, 0});

    anim = actor::vcallAt(self->armature, cs_slot::ARMATURE_GET_ANIMATION, {self->armature});
    fnthunk::makeLocal(f.addr(MOVEMENT_FN), g::movementLambda);
    rw::call(actor_fn::anim_setMovementEventCallFunc, {anim, f.addr(MOVEMENT_FN)});
    fnthunk::destroy(f.addr(MOVEMENT_FN));
    anim = actor::vcallAt(self->armature, cs_slot::ARMATURE_GET_ANIMATION, {self->armature});
    fnthunk::makeLocal(f.addr(FRAME_FN), g::frameLambda, {ga(self)});
    rw::call(actor_fn::anim_setFrameEventCallFunc, {anim, f.addr(FRAME_FN)});
    fnthunk::destroy(f.addr(FRAME_FN));
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), self->armature});

    if (info->alreadyOpened) {
        self->state = ALREADY_OPENED;
    } else {
        auto *pos = f.ptr<bt::Vec2>(POS);
        rw::store32(&pos->x, rw::fbits(info->position.x));
        rw::store32(&pos->y, rw::fbits(info->position.y));
        auto *prizeInfo = f.ptr<MapPrizes::Info>(PRIZE_INFO);
        MapPrizes::Info::construct(prizeInfo, pos, 2);      // FromGeneration 2: treasure
        if (rw::call(g::getTreasurePrizes, {ga(&prizeInfo->items), uint32_t(int32_t(info->rewardId)),
                                            uint32_t(int32_t(info->treasureId)), uint32_t(fromStageType)}) == 1) {
            MapPrizes *prizes = MapPrizes::create(prizeInfo, parent);
            rw::vcall(prizes, node_slot::SET_VISIBLE, {ga(prizes), 0});
            rw::call(g::vecPrizes_pushBack, {ga(&self->prizes), ga(prizes)});
        }
    }
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 1});
    float px = info->position.x, py = info->position.y;
    self->mapPosition.y = py;
    self->mapPosition.x = px;
    return 1;
}
RW_FUNCTION(0x5DF098, "MapTreasure::init", MapTreasure::init);

MapTreasure *MapTreasure::create(const Info *info, uint32_t parent, int32_t fromStageType) {
    auto *self = rw::host_ptr<MapTreasure>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self);
    if (rw::vcall(self, VSLOT_INIT, {ga(self), ga(info), parent, uint32_t(fromStageType)}) == 1) {
        rw::call(actor_fn::ref_autorelease, {ga(self)});
        return self;
    }
    rw::vcall(self, node_slot::DELETING_DTOR, {ga(self)});
    return nullptr;
}
RW_FUNCTION(0x5DF440, "MapTreasure::create", MapTreasure::create);

// Preload the chest armature and its sound.
void MapTreasure::firstCall() {
    bt::GuestFrame f(16);
    uint32_t adm = rw::call(actor_fn::adm_getInstance, {});
    actor::makeString(f.addr(0), g::str_boxJson, 51);
    rw::call(actor_fn::adm_addArmatureFileInfo, {adm, f.addr(0)});
    actor::freeString(f.addr(0));
    uint32_t audio = rw::call(actor_fn::audio_getInstance, {});
    rw::call(actor_fn::audio_load, {audio, g::SE_OPEN});
}
RW_FUNCTION(0x5DF520, "MapTreasure::firstCall", MapTreasure::firstCall);

void MapTreasure::finalCall() {}
RW_FUNCTION(0x5DF5C4, "MapTreasure::finalCall", MapTreasure::finalCall);

// The opening animation reached its "Open" frame: erupt the prizes, then report to the open callback.
void MapTreasure::terminate(MapTreasure *self) {
    if (self->state != OPENING) return;
    self->state = OPENED;
    uint32_t end = self->prizes.end, it = self->prizes.begin;
    for (; it != end; it += 4) {
        uint32_t prizes = actor::ld(it);
        actor::vcallAt(prizes, node_slot::SET_VISIBLE, {prizes, 1});
        MapPrizes::eruption(rw::host_ptr<MapPrizes>(prizes));
    }
    if (self->onOpened.f) rw::call(g::function_call, {ga(&self->onOpened), ga(self)});
}
RW_FUNCTION(0x5DF5C6, "MapTreasure::terminate", MapTreasure::terminate);

// Start opening a closed chest whose animation is paused on its first frame. Returns 1 when it started.
uint32_t MapTreasure::open(MapTreasure *self, uint32_t onOpened) {
    uint32_t anim = actor::vcallAt(self->armature, cs_slot::ARMATURE_GET_ANIMATION, {self->armature});
    if (actor::vcallAt(anim, cs_slot::ANIM_GET_CURRENT_FRAME_INDEX, {anim}) != 0) return 0;
    if (actor::vcallAt(anim, cs_slot::ANIM_IS_PAUSE, {anim}) != 1) return 0;
    if (self->state != CLOSED) return 0;
    self->state = OPENING;
    rw::call(g::function_assign, {ga(&self->onOpened), onOpened});
    actor::vcallAt(anim, cs_slot::ANIM_RESUME, {anim});
    bt::GuestFrame f(8);
    uint32_t audio = rw::call(actor_fn::audio_getInstance, {});
    rw::call(actor_fn::audio_getLayer, {f.addr(0), audio, 0});
    rw::call(actor_fn::layer_playSE, {f.word(0), g::SE_OPEN, 0, 0});
    bt::releaseShared(f.word(4));
    return 1;
}
RW_FUNCTION(0x5DF6A8, "MapTreasure::open", MapTreasure::open);

// ------------------------------------------------------------------------------------------- init() lambdas
void MapTreasure::onFrameEvent(uint32_t lambda, uint32_t, uint32_t eventName) {
    uint32_t self = actor::ld(lambda + 4);
    if (rw::call(g::string_compare, {eventName, g::str_open}) == 0) terminate(rw::host_ptr<MapTreasure>(self));
}

// $_1 frame event (captures this)
RW_FUNCTION(0x5DF93C, "MapTreasure::init $_1 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5DF944, "MapTreasure::init $_1 __func::__clone()", (fnthunk::cloneWords<g::frameLambda, 8, 1>));
RW_FUNCTION(0x5DF970, "MapTreasure::init $_1 __func::__clone(__base*)", fnthunk::cloneInto1<g::frameLambda>);
RW_FUNCTION(0x5DF990, "MapTreasure::init $_1 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5DF992, "MapTreasure::init $_1 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5DF99C, "MapTreasure::init $_1 (frame event: Open -> terminate)", MapTreasure::onFrameEvent);
RW_FUNCTION(0x5DF9C8, "MapTreasure::init $_1 __func::target", fnthunk::target<g::frameLambdaTS>);
RW_FUNCTION(0x5DF9E8, "MapTreasure::init $_1 __func::target_type", fnthunk::targetType<g::frameLambdaTI>);
// $_0 movement event (empty lambda, empty body)
RW_FUNCTION(0x5DF9FC, "MapTreasure::init $_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5DFA04, "MapTreasure::init $_0 __func::__clone()", fnthunk::clone0<g::movementLambda>);
RW_FUNCTION(0x5DFA28, "MapTreasure::init $_0 __func::__clone(__base*)", fnthunk::cloneInto0<g::movementLambda>);
RW_FUNCTION(0x5DFA44, "MapTreasure::init $_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5DFA46, "MapTreasure::init $_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5DFA4E, "MapTreasure::init $_0 (movement event, empty)", fnthunk::nothing);
RW_FUNCTION(0x5DFA50, "MapTreasure::init $_0 __func::target", fnthunk::target<g::movementLambdaTS>);
RW_FUNCTION(0x5DFA70, "MapTreasure::init $_0 __func::target_type", fnthunk::targetType<g::movementLambdaTI>);
