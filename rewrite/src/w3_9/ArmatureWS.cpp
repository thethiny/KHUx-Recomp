#include "ArmatureWS.hpp"

#include "../core/Guest.hpp"

namespace {
namespace g {
constexpr uint32_t armature_ctor = 0x79F489;          // cocostudio::Armature::Armature()
constexpr uint32_t armature_dtor = 0x79F5C1;          // cocostudio::Armature::~Armature()
constexpr uint32_t armature_init = 0x79F721;          // cocostudio::Armature::init()
constexpr uint32_t armature_initName = 0x79F7BD;      // cocostudio::Armature::init(const std::string&)
constexpr uint32_t armature_initNameBone = 0x79FFD1;  // cocostudio::Armature::init(const std::string&, Bone*)
constexpr uint32_t armature_update = 0x7A07CD;        // cocostudio::Armature::update(float)
constexpr uint32_t ref_autorelease = 0x8FD0D5;        // cocos2d::Ref::autorelease()
}  // namespace g

constexpr float ONE = 1.0f;

// What every init does after the Armature's own init succeeded.
uint32_t afterInit(ArmatureWS *self, uint32_t ok) {
    if (ok != 1) return 0;
    self->timeScale = ONE;
    return 1;
}

// The inlined construction + init + autorelease of the create functions; the init is the Armature's (non-virtual).
ArmatureWS *finishCreate(ArmatureWS *self, uint32_t ok) {
    if (ok == 1) {
        self->timeScale = ONE;
        rw::call(g::ref_autorelease, {rw::arg(self)});
        return self;
    }
    rw::vcall(self, 1, {rw::arg(self)});                  // deleting destructor
    return nullptr;
}

ArmatureWS *allocate() {
    auto *self = rw::host_ptr<ArmatureWS>(guest::newObject(sizeof(ArmatureWS)));
    ArmatureWS::construct(self);
    return self;
}
}  // namespace

ArmatureWS *ArmatureWS::construct(ArmatureWS *self) {
    rw::call(g::armature_ctor, {rw::arg(self)});
    self->vtable = VTABLE;
    self->blendVtable = BLEND_VTABLE;
    return self;
}
RW_FUNCTION(0x7651AC, "ArmatureWS::ArmatureWS", ArmatureWS::construct);

uint32_t ArmatureWS::destruct(ArmatureWS *self) { return rw::call(g::armature_dtor, {rw::arg(self)}); }
RW_FUNCTION(0x7651DC, "ArmatureWS::~ArmatureWS", ArmatureWS::destruct);

void ArmatureWS::destructDelete(ArmatureWS *self) {
    uint32_t p = rw::call(g::armature_dtor, {rw::arg(self)});
    guest::deleteObject(p);                               // (the base destructor returns this)
}
RW_FUNCTION(0x7651F4, "ArmatureWS::~ArmatureWS(deleting)", ArmatureWS::destructDelete);

uint32_t ArmatureWS::init(ArmatureWS *self) { return afterInit(self, rw::call(g::armature_init, {rw::arg(self)})); }
RW_FUNCTION(0x765214, "ArmatureWS::init()", ArmatureWS::init);

uint32_t ArmatureWS::initWithName(ArmatureWS *self, uint32_t name) {
    return afterInit(self, rw::call(g::armature_initName, {rw::arg(self), name}));
}
RW_FUNCTION(0x765236, "ArmatureWS::init(const std::string&)", ArmatureWS::initWithName);

uint32_t ArmatureWS::initWithNameBone(ArmatureWS *self, uint32_t name, uint32_t parentBone) {
    return afterInit(self, rw::call(g::armature_initNameBone, {rw::arg(self), name, parentBone}));
}
RW_FUNCTION(0x765258, "ArmatureWS::init(const std::string&, cocostudio::Bone*)", ArmatureWS::initWithNameBone);

ArmatureWS *ArmatureWS::create() {
    ArmatureWS *self = allocate();
    return finishCreate(self, rw::call(g::armature_init, {rw::arg(self)}));
}
RW_FUNCTION(0x76527C, "ArmatureWS::create()", ArmatureWS::create);

ArmatureWS *ArmatureWS::createWithName(uint32_t name) {
    ArmatureWS *self = allocate();
    return finishCreate(self, rw::call(g::armature_initName, {rw::arg(self), name}));
}
RW_FUNCTION(0x7652FC, "ArmatureWS::create(const std::string&)", ArmatureWS::createWithName);

ArmatureWS *ArmatureWS::createWithNameBone(uint32_t name, uint32_t parentBone) {
    ArmatureWS *self = allocate();
    return finishCreate(self, rw::call(g::armature_initNameBone, {rw::arg(self), name, parentBone}));
}
RW_FUNCTION(0x765384, "ArmatureWS::create(const std::string&, cocostudio::Bone*)", ArmatureWS::createWithNameBone);

// Armature::update with the scaled time step.
void ArmatureWS::update(ArmatureWS *self, float dt) {
    float scaled = self->timeScale.get() * dt;
    rw::call(g::armature_update, {rw::arg(self), rw::fbits(scaled)});
}
RW_FUNCTION(0x765414, "ArmatureWS::update", ArmatureWS::update);

void ArmatureWS::set_time_scale(ArmatureWS *self, float scale) { self->timeScale = scale; }
RW_FUNCTION(0x765432, "ArmatureWS::set_time_scale", ArmatureWS::set_time_scale);
