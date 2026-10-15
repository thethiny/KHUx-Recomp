// SkillDataManager: the owned-skill view model (skill lists, medal skill slots).
#include "SkillDataManager.hpp"

#include <cmath>

namespace {
constexpr uint32_t MAKE_SHARED_USER_SKILL_DATA = 0x76BEF5;   // std::make_shared<UserSkillData>()
constexpr uint32_t APIMANAGER_GET_INSTANCE = 0x4BA8C9;
constexpr uint32_t APIMANAGER_USER_SKILL = 0xDC;              // shared_ptr<UserSkill> (40-byte records)
constexpr uint32_t SKILL_CREATE = 0x78BDE9;                   // master::Skill::create(int)
constexpr uint32_t SKILL_EXP_CREATE = 0x78CA8D;               // master::SkillExp::create(int)
constexpr uint32_t LIBM_POW = 0x410654;                       // pow(double, double)
constexpr uint32_t STRINGBUFFER_FSET = 0x491501;
constexpr uint32_t STR_SKILL_ICON = 0xE079ED;                 // "img/skill/Skill_Icon%d.png"
constexpr uint32_t SKILL_IMAGE_ID = 8;                        // master::Skill data positions
constexpr uint32_t SKILL_EXP_TYPE = 216;
constexpr uint32_t SKILL_EXP_VALUE = 4;                       // master::SkillExp: int32[3]
constexpr uint32_t RECORD_SIZE = 40;

using UserSkillData = SkillDataManager::UserSkillData;

double guestPow(double x, double y) {
    uint64_t a, b;
    std::memcpy(&a, &x, 8);
    std::memcpy(&b, &y, 8);
    uint64_t r = rw::call64(LIBM_POW, {uint32_t(a), uint32_t(a >> 32), uint32_t(b), uint32_t(b >> 32)});
    double d;
    std::memcpy(&d, &r, 8);
    return d;
}
// __fixdfsi
int32_t d2iz(double d) {
    if (std::isnan(d)) return 0;
    if (d >= 2147483648.0) return INT32_MAX;
    if (d <= -2147483649.0) return INT32_MIN;
    return int32_t(d);
}
void writeValue(UserSkillData *data, uint32_t index, const void *src, uint32_t n) {
    SafeBuffer::write(data, data->data.get() + index, static_cast<const uint8_t *>(src), n, index);
}
}  // namespace

uint32_t SkillDataManager::getInstance() {
    uint32_t self = rw::load<uint32_t>(INSTANCE);
    if (!self) {
        self = guest::newObject(0x404);
        rw::save<uint32_t>(self + 0x400, 0);
        rw::save<uint8_t>(self, 0);
        rw::save<uint32_t>(INSTANCE, self);
    }
    return self;
}
RW_FUNCTION(0x76BA0C, "SkillDataManager::getInstance", SkillDataManager::getInstance);

void SkillDataManager::construct(uint32_t self) {
    rw::save<uint32_t>(self + 0x400, 0);
    rw::save<uint8_t>(self, 0);
}
RW_FUNCTION(0x76BA48, "SkillDataManager::SkillDataManager", SkillDataManager::construct);

void SkillDataManager::destruct(uint32_t) {}
RW_FUNCTION(0x76BA54, "SkillDataManager::~SkillDataManager", SkillDataManager::destruct);

void SkillDataManager::getData(guest::SharedPtr *out, uint32_t, uint64_t id) {
    // stack arguments: the UserSkill::Skill record (40 bytes) at +0, then the keepId bool at +40
    const uint32_t args = rw::cpu->r[13];
    uint32_t skill[10];   // the function's own copy of the record
    for (uint32_t k = 0; k < 10; k++) skill[k] = rw::load<uint32_t>(args + 4 * k);
    uint8_t keepId = rw::load<uint8_t>(args + 40);
    guest::Frame f(16);
    rw::call(MAKE_SHARED_USER_SKILL_DATA, {rw::guest_addr(out)});
    uint32_t api = rw::call(APIMANAGER_GET_INSTANCE, {});
    uint32_t apiCtrl = rw::load<uint32_t>(api + APIMANAGER_USER_SKILL + 4);
    uint32_t list = rw::load<uint32_t>(api + APIMANAGER_USER_SKILL);
    if (apiCtrl) guest::addRef(apiCtrl);
    const SafeBuffer *sb = rw::host_ptr<SafeBuffer>(list);
    uint32_t count = rw::load<uint32_t>(list + 0x20);
    bool found = false;
    uint32_t pos = 0;
    for (uint32_t k = 0; k < count; k++, pos += RECORD_SIZE) {
        uint64_t recId = uint32_t(sb->get32(pos)) | (uint64_t(uint32_t(sb->get32(pos + 4))) << 32);
        if (recId == id) {
            found = k != 0xFFFFFFFFu;
            break;
        }
    }
    if (found) {
        for (uint32_t k = 0; k < 10; k++) skill[k] = uint32_t(sb->get32(pos + 4 * k));
        UserSkillData *d = rw::host_ptr<UserSkillData>(out->obj);
        writeValue(d, 0, skill, 8);
    } else if (!keepId) {
        skill[0] = 0;
        skill[1] = 0;
    }
    uint32_t row = f.addr(0);
    rw::call(SKILL_CREATE, {row, skill[2]});
    const uint8_t valid = 1;
    UserSkillData *data = rw::host_ptr<UserSkillData>(out->obj);
    {
        uint32_t obj = rw::load<uint32_t>(row), ctrl = rw::load<uint32_t>(row + 4);
        if (ctrl) guest::addRef(ctrl);
        data->skill.obj = obj;
        uint32_t old = data->skill.ctrl;
        data->skill.ctrl = ctrl;
        if (old) guest::release(old);
    }
    writeValue(data, UserSkillData::USER_SKILL_ID, &id, 8);
    writeValue(data, UserSkillData::LEVEL, &skill[3], 4);
    writeValue(data, UserSkillData::EXP, &skill[4], 4);
    writeValue(data, UserSkillData::USER_MEDAL_ID, &skill[6], 8);
    writeValue(data, UserSkillData::VALID, &valid, 1);
    if (skill[0] | skill[1]) {
        // experience curve: value(level) = E1 * (level / (E0 - 1)) ^ (E2 / 10000)
        int32_t expType = rw::host_ptr<SafeBuffer>(rw::load<uint32_t>(row))->get32(SKILL_EXP_TYPE);
        uint32_t exp = f.addr(8);
        rw::call(SKILL_EXP_CREATE, {exp, uint32_t(expType)});
        const SafeBuffer *e = rw::host_ptr<SafeBuffer>(rw::load<uint32_t>(exp));
        int32_t level = int32_t(skill[3]);
        int32_t e0 = e->get32(SKILL_EXP_VALUE), e1 = e->get32(SKILL_EXP_VALUE + 4), e2 = e->get32(SKILL_EXP_VALUE + 8);
        {
            float t = float(int32_t(uint32_t(level) - 1u)) / float(int32_t(uint32_t(e0) - 1u));
            float p = float(e2) / 10000.0f;
            int32_t v = d2iz(double(e1) * guestPow(double(t), double(p)));
            writeValue(data, UserSkillData::PREV_LEVEL_EXP, &v, 4);
        }
        {
            float t = float(level) / float(int32_t(uint32_t(e0) - 1u));
            float p = float(e2) / 10000.0f;
            int32_t v = d2iz(double(e1) * guestPow(double(t), double(p)));
            writeValue(data, UserSkillData::LEVEL_EXP, &v, 4);
        }
        if (rw::load<uint32_t>(exp + 4)) guest::release(rw::load<uint32_t>(exp + 4));
    }
    if (rw::load<uint32_t>(row + 4)) guest::release(rw::load<uint32_t>(row + 4));
    if (apiCtrl) guest::release(apiCtrl);
}
RW_FUNCTION(0x76BA58, "SkillDataManager::getData", SkillDataManager::getData);

void SkillDataManager::getImagePath(uint32_t stringBuffer, const guest::SharedPtr *data) {
    uint32_t row = rw::host_ptr<UserSkillData>(data->obj)->skill.obj;
    int32_t imageId = rw::host_ptr<SafeBuffer>(row)->get32(SKILL_IMAGE_ID);
    rw::call(STRINGBUFFER_FSET, {stringBuffer, STR_SKILL_ICON, uint32_t(imageId)});
}
RW_FUNCTION(0x76BE8C, "SkillDataManager::getImagePath", SkillDataManager::getImagePath);

SkillDataManager::UserSkillData *SkillDataManager::UserSkillData::destruct(UserSkillData *self) {
    self->vtable = VTABLE;
    if (self->skill.ctrl) guest::release(self->skill.ctrl);
    SafeBuffer::destruct(self);
    return self;
}
RW_FUNCTION(0x76C024, "SkillDataManager::UserSkillData::~UserSkillData", SkillDataManager::UserSkillData::destruct);

void SkillDataManager::UserSkillData::destructDelete(UserSkillData *self) {
    destruct(self);
    guest::deleteObject(rw::guest_addr(self));
}
RW_FUNCTION(0x76C058, "SkillDataManager::UserSkillData::~UserSkillData (deleting)",
            SkillDataManager::UserSkillData::destructDelete);
