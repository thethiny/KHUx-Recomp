// Avatar: InstanceInfo constructors, resource preloading, the waist position, the hit effect and the expansion
// action.
#include "Avatar.hpp"

using namespace avatar_fn;
using avatar_detail::cx;
using avatar_detail::destroyFunction;

namespace {
constexpr uint32_t INFO = AvatarInstanceInfo::SIZE;

void copyString12(uint32_t dst, uint32_t src) {
    for (uint32_t k = 0; k < 12; k += 4) rw::save<uint32_t>(dst + k, rw::load<uint32_t>(src + k));
}
}  // namespace

// ------------------------------------------------------------------------------------------- InstanceInfo
// A registered SafeBuffer of 0x30 zero bytes, no name, 45 empty part slots.
AvatarInstanceInfo *AvatarInstanceInfo::construct(AvatarInstanceInfo *self) {
    guest::Frame f(0x20);
    uint32_t a = rw::guest_addr(self);
    cx(sb_ctor, {a, AvatarStyle::DATA_SIZE, 1});
    rw::save<uint32_t>(a, AvatarStyle::VTABLE);
    rw::save<uint32_t>(a + 0x20, 0);
    rw::save<uint32_t>(a + 0x24, 0);
    rw::save<uint32_t>(a + 0x28, 0);
    for (uint32_t p = a + 0x2C; p != a + INFO; p += 24) {
        rw::save<uint8_t>(p, 0);
        cx(memset_, {p + 4, 0x14, 0});
    }
    cx(sb_zerofill, {a});
    return self;
}

AvatarInstanceInfo *AvatarInstanceInfo::copyConstruct(AvatarInstanceInfo *self, AvatarInstanceInfo *other) {
    guest::Frame f(0x30);
    uint32_t a = rw::guest_addr(self), o = rw::guest_addr(other);
    uint32_t d = cx(sb_copy, {a, o});
    rw::save<uint32_t>(d, AvatarStyle::VTABLE);
    if (rw::load<uint8_t>(o + 0x20) & 1)
        cx(string_init, {d + 0x20, rw::load<uint32_t>(o + 0x28), rw::load<uint32_t>(o + 0x24)});
    else
        copyString12(d + 0x20, o + 0x20);
    for (uint32_t k = 0; k < PART_COUNT; k++) {
        uint32_t dp = a + 24 * k, sp = o + 24 * k;
        rw::save<uint8_t>(dp + 0x2C, rw::load<uint8_t>(sp + 0x2C));
        rw::save<uint32_t>(dp + 0x30, rw::load<uint32_t>(sp + 0x30));
        uint32_t ctrl = rw::load<uint32_t>(sp + 0x34);
        rw::save<uint32_t>(dp + 0x34, ctrl);
        if (ctrl) cx(guest::SHARED_ADD_REF, {ctrl});
        rw::save<uint8_t>(dp + 0x2C, rw::load<uint8_t>(sp + 0x2C));
        if (rw::load<uint8_t>(sp + 0x38) & 1)
            cx(string_init, {dp + 0x38, rw::load<uint32_t>(sp + 0x40), rw::load<uint32_t>(sp + 0x3C)});
        else
            copyString12(dp + 0x38, sp + 0x38);
        rw::save<uint8_t>(dp + 0x2C, rw::load<uint8_t>(sp + 0x2C));
    }
    return self;
}

// ------------------------------------------------------------------------------------------- Avatar
// The body movie's position (y up); also keeps -y as the paralysis effect height.
uint32_t Avatar::getWaistPosition(uint32_t out, Avatar *self) {
    guest::Frame f(0x18);
    uint32_t body = self->bodyMovie;
    if (!body) return cx(vec2_copy, {out, vec2_zero});
    uint32_t y = cx(movie_get_y, {body});
    rw::set<uint32_t>(self, 660, y ^ 0x80000000u);   // effectYParalysis = -y
    uint32_t x = cx(movie_get_x, {self->bodyMovie});
    uint32_t y2 = cx(movie_get_y, {self->bodyMovie});
    return cx(vec2_ctor, {out, x, y2 ^ 0x80000000u});
}

// Preload the LWF resources of the part paths [from, to) of an instance.
void Avatar::preloadPartResource(uint32_t, AvatarInstanceInfo *info, int32_t from, int32_t to, uint32_t) {
    guest::Frame f(0x38);
    if (from >= to) return;
    uint32_t p = rw::guest_addr(info) + 24 * uint32_t(from) + 0x38;
    int32_t kind = from;
    do {
        if (avatar_detail::strSize(p)) {
            uint32_t data = avatar_detail::strData(p);
            uint32_t len = cx(strlen_, {data});
            cx(string_init, {f.addr(0x14), data, len});
            cx(lwfutil_preload, {f.addr(0x14)});
            if (*f.at<uint8_t>(0x14) & 1) cx(guest::OPERATOR_DELETE, {*f.at<uint32_t>(0x1C)});
        }
        p += 24;
    } while (++kind < to);
}

// Preload the burst motions (and their hit effects), hit effect 2, every motion LWF and every instance's parts.
void Avatar::preloadResources(Avatar *self) {
    guest::Frame f(0x480);
    uint32_t a = rw::guest_addr(self), sb = f.addr(0x60);
    *f.at<uint32_t>(0x460) = 0;
    *f.at<uint8_t>(0x60) = 0;
    for (uint32_t node = self->burstFirst; node; node = rw::load<uint32_t>(node)) {
        cx(burst_create, {f.addr(0x38), rw::load<uint32_t>(node + 8)});
        uint32_t burst = *f.at<uint32_t>(0x38);
        cx(sb_read, {burst, f.addr(0x40), rw::load<uint32_t>(burst + 12) + 4, 4, 4});
        uint32_t lwfId = *f.at<uint32_t>(0x40);
        cx(sbuf_fset, {sb, S_BURST_LWF, lwfId, lwfId});
        uint32_t len = cx(strlen_, {sb});
        cx(string_init, {f.addr(0x2C), sb, len});
        cx(lwfutil_preload, {f.addr(0x2C)});
        if (*f.at<uint8_t>(0x2C) & 1) cx(guest::OPERATOR_DELETE, {*f.at<uint32_t>(0x34)});
        burst = *f.at<uint32_t>(0x38);
        cx(sb_read, {burst, f.addr(0x44), rw::load<uint32_t>(burst + 12) + 8, 4, 8});
        cx(hiteffect_preload, {*f.at<uint32_t>(0x44)});
        uint32_t ctrl = *f.at<uint32_t>(0x3C);
        if (ctrl) cx(guest::SHARED_RELEASE, {ctrl});
    }
    cx(hiteffect_preload, {2});
    for (uint32_t i = 0; i < 46; i++) {
        uint32_t path = rw::load<uint32_t>(MOTION_PATHS + 4 * i);
        uint32_t len = cx(strlen_, {path});
        cx(string_init, {f.addr(0x20), path, len});
        cx(lwfutil_preload, {f.addr(0x20)});
        if (*f.at<uint8_t>(0x20) & 1) cx(guest::OPERATOR_DELETE, {*f.at<uint32_t>(0x28)});
    }
    uint32_t end = self->instances.end;
    for (uint32_t p = self->instances.begin; p != end; p += INFO) {
        AvatarInstanceInfo *info = rw::host_ptr<AvatarInstanceInfo>(p);
        preloadPartResource(0, info, 0, 14, 0);
        preloadPartResource(0, info, 14, 15, 0);
        preloadPartResource(0, info, 15, 28, 0);
        preloadPartResource(0, info, 28, 30, 0);
        preloadPartResource(0, info, 30, 31, 0);
        // forEachAccessory(info, std::bind(&Avatar::preloadPartResource, this, ...))
        *f.at<uint32_t>(0x58) = f.addr(0x48);
        *f.at<uint32_t>(0x48) = FUNC_BIND_PRELOAD;
        *f.at<uint32_t>(0x4C) = AV_PRELOAD_PART;
        *f.at<uint32_t>(0x50) = 0;
        *f.at<uint32_t>(0x54) = a;
        forEachAccessory(self, info, f.addr(0x48));
        destroyFunction(f.addr(0x48), true);
    }
}

// The burst's hit effect while a burst motion plays, else 1.
int32_t Avatar::getHitEffectID(Avatar *self) {
    guest::Frame f(0x28);
    int32_t id = 1;
    if (self->motion == MOTION_BURST) {
        cx(burst_create, {f.addr(8), uint32_t(self->burstId)});
        uint32_t burst = *f.at<uint32_t>(8);
        cx(sb_read, {burst, f.addr(0x10), rw::load<uint32_t>(burst + 12) + 8, 4, 8});
        uint32_t ctrl = *f.at<uint32_t>(0xC);
        id = *f.at<int32_t>(0x10);
        if (ctrl) cx(guest::SHARED_RELEASE, {ctrl});
    }
    return id;
}

// EaseSineInOut(ScaleTo(expansionFrames / 30 s, expansionScale %))
uint32_t Avatar::createExpansionAction(Avatar *self) {
    self->expansionPending = 0;
    float duration = float(int32_t(self->expansionFrames)) / 30.0f;
    float scale = float(int32_t(self->expansionScale)) / 100.0f;
    uint32_t action = rw::call(scaleto_create, {rw::fbits(duration), rw::fbits(scale)});
    return rw::call(ease_sine_create, {action});
}

RW_FUNCTION(0x5C5D9C, "Avatar::InstanceInfo::InstanceInfo()", AvatarInstanceInfo::construct);
RW_FUNCTION(0x5C95B8, "Avatar::InstanceInfo::InstanceInfo(const InstanceInfo&)", AvatarInstanceInfo::copyConstruct);
RW_FUNCTION(0x5C7D90, "Avatar::getWaistPosition", Avatar::getWaistPosition);
RW_FUNCTION(0x5C7DFC, "Avatar::preloadPartResource", Avatar::preloadPartResource);
RW_FUNCTION(0x5C7ED0, "Avatar::preloadResources", Avatar::preloadResources);
RW_FUNCTION(0x5C815C, "Avatar::getHitEffectID", Avatar::getHitEffectID);
RW_FUNCTION(0x5C81EC, "Avatar::createExpansionAction", Avatar::createExpansionAction);
