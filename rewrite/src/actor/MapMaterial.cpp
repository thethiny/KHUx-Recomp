#include "MapMaterial.hpp"

#include "battle/MapPrizes.hpp"

namespace {
namespace g {
constexpr uint32_t armature_create = rwlib::cocostudio::Armature::create;       // cocostudio::Armature::create(const std::string&)
constexpr uint32_t adm_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;       // cocostudio::ArmatureDataManager::getInstance()
constexpr uint32_t adm_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;  // ArmatureDataManager::addArmatureFileInfo(const std::string&)
constexpr uint32_t audio_getInstance = 0x5BECFD;     // audio::Manager::getInstance()
constexpr uint32_t audio_load = 0x5BEEF5;            // audio::Manager::load(ResourceID)
constexpr uint32_t audio_getLayer = 0x5BF33D;        // audio::Manager::getLayer(int) (sret shared_ptr<Layer>)
constexpr uint32_t layer_playSE = 0x5BF785;          // audio::Layer::playSE(ResourceID, int, int)
constexpr uint32_t vecPrizes_clear = 0x5DB615;       // cocos2d::Vector<MapPrizes*>::clear()
constexpr uint32_t vecPrizes_dtor = 0x5DB649;        // cocos2d::Vector<MapPrizes*>::~Vector()
constexpr uint32_t vecPrizes_pushBack = 0x5DB985;    // cocos2d::Vector<MapPrizes*>::pushBack(MapPrizes*)
constexpr uint32_t str_excavateAnim = 0xE02207;      // "ActionMapExcavateAnim" (21)
constexpr uint32_t str_normal = 0xE02200;            // "Normal" (6)
constexpr uint32_t str_excavateJson = 0xE021CC;      // "cocostudio/publish/ActionMapExcavateAnim.ExportJson" (51)
constexpr uint32_t SE_EXCAVATE = 0x1DEBE9C6;         // audio::ResourceID of the dig sound
}  // namespace g

// cocostudio virtual slots
constexpr uint32_t ARMATURE_GET_ANIMATION = 166;     // Armature::getAnimation()
constexpr uint32_t ANIMATION_PLAY = 26;              // ArmatureAnimation::play(const std::string&, int, int)

using actor::fb;
uint32_t ga(const void *p) { return rw::guest_addr(p); }
}  // namespace

MapMaterial *MapMaterial::construct(MapMaterial *self) {
    MapNode::construct(self);
    self->vtable = VTABLE;
    SafeBuffer::construct(&self->property, 8, 1);
    self->property.vtable = Property::VTABLE;
    rw::call(actor_fn::aeabi_memset, {ga(&self->armature), 0x11, 0});   // armature, prizes, opened
    return self;
}
RW_FUNCTION(0x5DBA50, "MapMaterial::MapMaterial", MapMaterial::construct);

MapMaterial *MapMaterial::destruct(MapMaterial *self) {
    self->vtable = VTABLE;
    rw::call(actor_fn::node_removeAllChildrenWithCleanup, {ga(self), 1});
    rw::call(g::vecPrizes_clear, {ga(&self->prizes)});
    rw::call(g::vecPrizes_dtor, {ga(&self->prizes)});
    SafeBuffer::destruct(&self->property);
    MapNode::destruct(self);
    return self;
}
RW_FUNCTION(0x5DB598, "MapMaterial::~MapMaterial", MapMaterial::destruct);

void MapMaterial::destructDelete(MapMaterial *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DB674, "MapMaterial::~MapMaterial (deleting)", MapMaterial::destructDelete);

void MapMaterial::Property::destructDelete(Property *self) {
    SafeBuffer::destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DBDAA, "MapMaterial::Property::~Property (deleting)", MapMaterial::Property::destructDelete);

// Build the dig spot: the excavate armature playing "Normal", the material in the property, and one hidden
// single-material MapPrizes per prize, spaced 64 / prizeCount px apart starting 64 px left of the spot.
uint32_t MapMaterial::init(MapMaterial *self, const Info *info, uint32_t parent) {
    if (MapNode::init(self) != 1) return 0;
    if (info->prizeCount == 0) return 0;
    rw::vcall(self, node_slot::SET_TAG, {ga(self), uint32_t(NODE_TAG)});

    bt::GuestFrame f(96);
    enum : uint32_t { STR = 0, PRIZE_INFO = 16 };
    actor::makeString(f.addr(STR), g::str_excavateAnim, 21);
    self->armature = rw::call(g::armature_create, {f.addr(STR)});   // cocostudio::Armature::create
    actor::freeString(f.addr(STR));
    if (!self->armature) return 0;

    SafeBuffer::write(&self->property, self->property.data.get() + Property::MATERIAL_ID,
                      rw::host_ptr<uint8_t>(ga(&info->materialId)), 4, Property::MATERIAL_ID);
    SafeBuffer::write(&self->property, self->property.data.get() + Property::COUNT,
                      rw::host_ptr<uint8_t>(ga(&info->count)), 4, Property::COUNT);
    uint32_t animation = actor::vcallAt(self->armature, ARMATURE_GET_ANIMATION, {self->armature});
    actor::makeString(f.addr(STR), g::str_normal, 6);
    actor::vcallAt(animation, ANIMATION_PLAY, {animation, f.addr(STR), 0xFFFFFFFFu, 0xFFFFFFFFu});
    actor::freeString(f.addr(STR));
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), self->armature});
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 1});

    float startX = info->position.x + -64.0f;
    float startY = info->position.y;
    if (info->prizeCount != 0) {
        float step = (1.0f / float(uint32_t(info->prizeCount))) * 64.0f;
        uint32_t k = 0;
        do {
            float x = step * float(int32_t(k)) + startX;
            auto *pos = f.ptr<bt::Vec2>(PRIZE_INFO + 80);    // Vec2 argument (by value)
            rw::store32(&pos->x, rw::fbits(x));
            rw::store32(&pos->y, rw::fbits(startY));
            auto *prizeInfo = f.ptr<MapPrizes::Info>(PRIZE_INFO);
            MapPrizes::Info::construct(prizeInfo, pos, 3);   // FromGeneration 3: material
            prizeInfo->items[0].type = 5;                    // item type 5: material
            prizeInfo->items[0].id = info->materialId;
            prizeInfo->items[0].count = 1;
            MapPrizes *prizes = MapPrizes::create(prizeInfo, parent);
            rw::vcall(prizes, node_slot::SET_VISIBLE, {ga(prizes), 0});
            rw::call(g::vecPrizes_pushBack, {ga(&self->prizes), ga(prizes)});
            ++k;
        } while (k < info->prizeCount);
    }
    float px = info->position.x, py = info->position.y;
    self->mapPosition.y = py;
    self->mapPosition.x = px;
    return 1;
}
RW_FUNCTION(0x5DB680, "MapMaterial::init", MapMaterial::init);

MapMaterial *MapMaterial::create(const Info *info, uint32_t parent) {
    auto *self = rw::host_ptr<MapMaterial>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self);
    if (rw::vcall(self, VSLOT_INIT, {ga(self), ga(info), parent}) == 1) {
        rw::call(actor_fn::ref_autorelease, {ga(self)});
        return self;
    }
    rw::vcall(self, node_slot::DELETING_DTOR, {ga(self)});
    return nullptr;
}
RW_FUNCTION(0x5DB9EC, "MapMaterial::create", MapMaterial::create);

// Preload the excavate armature and the dig sound.
void MapMaterial::firstCall() {
    bt::GuestFrame f(16);
    uint32_t adm = rw::call(g::adm_getInstance, {});
    actor::makeString(f.addr(0), g::str_excavateJson, 51);
    rw::call(g::adm_addArmatureFileInfo, {adm, f.addr(0)});
    actor::freeString(f.addr(0));
    uint32_t audio = rw::call(g::audio_getInstance, {});
    rw::call(g::audio_load, {audio, g::SE_EXCAVATE});
}
RW_FUNCTION(0x5DBABC, "MapMaterial::firstCall", MapMaterial::firstCall);

void MapMaterial::finalCall() {}
RW_FUNCTION(0x5DBB60, "MapMaterial::finalCall", MapMaterial::finalCall);

// Dig the spot once: show and erupt every prize burst, play the dig sound, hide the spot. Returns 1 the first time.
uint32_t MapMaterial::popup(MapMaterial *self) {
    if (self->opened != 0) return 0;
    self->opened = 1;
    uint32_t end = self->prizes.end, it = self->prizes.begin;
    for (; it != end; it += 4) {
        uint32_t prizes = actor::ld(it);
        actor::vcallAt(prizes, node_slot::SET_VISIBLE, {prizes, 1});
        MapPrizes::eruption(rw::host_ptr<MapPrizes>(prizes));
    }
    bt::GuestFrame f(8);
    uint32_t audio = rw::call(g::audio_getInstance, {});
    rw::call(g::audio_getLayer, {f.addr(0), audio, 0});      // shared_ptr<audio::Layer>
    rw::call(g::layer_playSE, {f.word(0), g::SE_EXCAVATE, 0, 0});
    bt::releaseShared(f.word(4));
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 0});
    return 1;
}
RW_FUNCTION(0x5DBB64, "MapMaterial::popup", MapMaterial::popup);
