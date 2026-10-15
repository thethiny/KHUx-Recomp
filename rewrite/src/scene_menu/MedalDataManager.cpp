// MedalDataManager: the owned-medal view model used by the medal lists, deck editing and fusion screens.
#include "MedalDataManager.hpp"

using hole::network::api::UserMedal;
using UserMedalData = MedalDataManager::UserMedalData;

namespace {
constexpr uint32_t MAKE_SHARED_USER_MEDAL_DATA = 0x76B529;   // std::make_shared<UserMedalData>()
constexpr uint32_t APIMANAGER_GET_INSTANCE = 0x4BA8C9;
constexpr uint32_t APIMANAGER_USER_MEDAL = 0xD4;              // shared_ptr<UserMedal> (72-byte records)
constexpr uint32_t MEDAL_INDEX_FIND = 0x490275;               // unordered_map<unsigned long long, int>::find
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;
constexpr uint32_t APPSTATE_NEW_MEDAL_IDS = 0x4EB1BD;         // AppState::getNewUserMedalIDs() -> vector<u64>*
constexpr uint32_t APPSTATE_WILL_LOCK_IDS = 0x4EB347;         // AppState::getWillLockUserMedalIDs()
constexpr uint32_t APPSTATE_WILL_UNLOCK_IDS = 0x4EB507;       // AppState::getWillUnLockUserMedalIDs()
constexpr uint32_t MEDAL_CREATE = 0x785871;                   // master::Medal::create(int)
constexpr uint32_t CALC_MEDAL_STATUS = 0x4966BD;              // BattleUtil::calcMedalStatus
constexpr uint32_t CALC_MEDAL_EXP = 0x496BD1;                 // BattleUtil::calcMedalExp
constexpr uint32_t FIND_MEDAL_SET_KEYBLADE_INDEX = 0x4C51E9;  // APIManager::findMedalSetKeybladeIndex(u64)
constexpr uint32_t STRINGBUFFER_FSET = 0x491501;              // StringBuffer::fset(fmt, ...)
constexpr uint32_t STR_MEDAL_S = 0xDF90C5;                    // "img/medal/Medal_S_%d.png"
constexpr uint32_t STR_MEDAL_L = 0xE079D4;                    // "img/medal/Medal_L_%d.png"
constexpr uint32_t MEDAL_THUMB_ID = 8;                        // master::Medal data positions
constexpr uint32_t MEDAL_IMAGE_ID = 4;
constexpr uint32_t MEDAL_SELL = 1220;
constexpr uint32_t MEDAL_SELL_PER_LV = 1224;

constexpr uint32_t VT_STATUS_LAMBDA = 0xF6EBC8;   // __func<getMedalData::$_0, void(int,int)> + 8
constexpr uint32_t VT_EXP_LAMBDA = 0xF6EB9C;      // __func<getMedalData::$_1, void(int,int,int,int)> + 8
constexpr uint32_t STATUS_LAMBDA_NAME = 0xEA1980; // its type_info name
constexpr uint32_t EXP_LAMBDA_NAME = 0xEA18A0;
constexpr uint32_t STATUS_LAMBDA_TYPEINFO = 0xFA97CC;
constexpr uint32_t EXP_LAMBDA_TYPEINFO = 0xFA97B8;

int32_t sb32(uint32_t sb, uint32_t index) { return rw::host_ptr<SafeBuffer>(sb)->get32(index); }

// a std::function stored in place on the guest stack, its lambda capturing shared_ptr<UserMedalData>
uint32_t inlineFunction(guest::Frame &f, uint32_t offset, uint32_t vtable, uint32_t obj, uint32_t ctrl) {
    uint32_t fo = f.addr(offset);
    rw::save<uint32_t>(fo, vtable);
    rw::save<uint32_t>(fo + 4, obj);
    rw::save<uint32_t>(fo + 8, ctrl);
    rw::save<uint32_t>(fo + 16, fo);
    return fo;
}
// ~function: destroy() when stored in place, destroy_deallocate() when on the heap
void destroyFunction(uint32_t fo) {
    uint32_t p = rw::load<uint32_t>(fo + 16);
    if (p == fo) rw::vcall(rw::host_ptr<uint8_t>(p), 4, {p});
    else if (p) rw::vcall(rw::host_ptr<uint8_t>(p), 5, {p});
}

// Is `id` in the std::vector<unsigned long long> at vec?
bool containsId(const guest::Vector *vec, uint64_t id) {
    for (uint32_t p = vec->begin; p != vec->end; p += 8)
        if (rw::load<uint64_t>(p) == id) return true;
    return false;
}
}  // namespace

uint32_t MedalDataManager::getInstance() {
    uint32_t self = rw::load<uint32_t>(INSTANCE);
    if (!self) {
        self = guest::newObject(0x404);
        rw::save<uint32_t>(self + 0x400, 0);
        rw::save<uint8_t>(self, 0);
        rw::save<uint32_t>(INSTANCE, self);
    }
    return self;
}
RW_FUNCTION(0x76AD68, "MedalDataManager::getInstance", MedalDataManager::getInstance);

void MedalDataManager::construct(uint32_t self) {
    rw::save<uint32_t>(self + 0x400, 0);
    rw::save<uint8_t>(self, 0);
}
RW_FUNCTION(0x76ADA4, "MedalDataManager::MedalDataManager", MedalDataManager::construct);

void MedalDataManager::destruct(uint32_t) {}
RW_FUNCTION(0x76ADB0, "MedalDataManager::~MedalDataManager", MedalDataManager::destruct);

void MedalDataManager::getMedalData(guest::SharedPtr *out, uint32_t, uint64_t id, int32_t medalId) {
    // the UserMedal::Medal argument passed by value: 8 bytes above the stack arguments' start (8-aligned)
    UserMedal::Medal &record = *rw::host_ptr<UserMedal::Medal>(rw::cpu->r[13] + 8);
    guest::Frame f(0x60);
    rw::call(MAKE_SHARED_USER_MEDAL_DATA, {rw::guest_addr(out)});
    uint32_t api = rw::call(APIMANAGER_GET_INSTANCE, {});
    uint32_t list = rw::load<uint32_t>(api + APIMANAGER_USER_MEDAL);
    rw::save<uint64_t>(f.addr(0), id);
    uint32_t node = rw::call(MEDAL_INDEX_FIND, {list + 0x24, f.addr(0)});
    int32_t index = node ? rw::load<int32_t>(node + 16) : -1;
    UserMedalData *data;
    bool isNew = false;
    if (node && index >= 0) {
        // the owned medal: the record comes from the user's list
        uint32_t pos = uint32_t(index) * 72u;
        const SafeBuffer *sb = rw::host_ptr<SafeBuffer>(list);
        for (uint32_t k = 0; k < 72; k += 4) rw::set<uint32_t>(&record, k, uint32_t(sb->get32(pos + k)));
        data = rw::host_ptr<UserMedalData>(out->obj);
        data->userMedalIdLo = rw::get<uint32_t>(&record, 0);
        data->userMedalIdHi = rw::get<uint32_t>(&record, 4);
        uint32_t app = rw::call(APPSTATE_GET_INSTANCE, {});
        isNew = containsId(rw::host_ptr<guest::Vector>(rw::call(APPSTATE_NEW_MEDAL_IDS, {app})), id);
    } else {
        rw::set<uint32_t>(&record, 0, 0);
        rw::set<uint32_t>(&record, 4, 0);
        if (record.medalId == 0) record.medalId = 1;
        data = rw::host_ptr<UserMedalData>(out->obj);
        data->userMedalIdLo = 0;
        data->userMedalIdHi = 0;
    }
    int32_t masterId;
    if (medalId != 0) {
        record.medalId = medalId;
        masterId = medalId;
    } else {
        masterId = record.medalId;
    }
    // master row (falls back to medal 1)
    uint32_t medal = f.addr(0x10), fallback = f.addr(0x18);
    rw::call(MEDAL_CREATE, {medal, uint32_t(masterId)});
    if (rw::load<uint32_t>(medal) == 0) {
        rw::call(MEDAL_CREATE, {fallback, 1});
        uint32_t obj = rw::load<uint32_t>(fallback), ctrl = rw::load<uint32_t>(fallback + 4);
        rw::save<uint32_t>(fallback, 0);
        rw::save<uint32_t>(fallback + 4, 0);
        rw::save<uint32_t>(medal, obj);
        uint32_t old = rw::load<uint32_t>(medal + 4);
        rw::save<uint32_t>(medal + 4, ctrl);
        if (old) guest::release(old);
    }
    {
        uint32_t ctrl = rw::load<uint32_t>(medal + 4), obj = rw::load<uint32_t>(medal);
        if (ctrl) guest::addRef(ctrl);
        data->medal.obj = obj;
        uint32_t old = data->medal.ctrl;
        data->medal.ctrl = ctrl;
        if (old) guest::release(old);
    }
    int32_t level = record.level;
    if (level == 0) {
        level = 1;
        record.level = 1;
    }
    data->level = level;

    uint32_t outCtrl = out->ctrl;
    if (outCtrl) {
        guest::addRef(outCtrl);
        guest::addRef(outCtrl);
    }
    // attack / defense: calcMedalStatus(medal, record, [data](attack, defense){...}, no keyblade, slot 0)
    {
        uint32_t fn = inlineFunction(f, 0x20, VT_STATUS_LAMBDA, out->obj, outCtrl);
        uint32_t keyblade = f.addr(0x38);
        rw::save<uint32_t>(keyblade, 0);
        rw::save<uint32_t>(keyblade + 4, 0);
        rw::call(CALC_MEDAL_STATUS, {medal, rw::guest_addr(&record), fn, keyblade, 0});
        if (rw::load<uint32_t>(keyblade + 4)) guest::release(rw::load<uint32_t>(keyblade + 4));
        destroyFunction(fn);
    }
    uint32_t medalRow = rw::load<uint32_t>(medal);
    int32_t sell = sb32(medalRow, MEDAL_SELL);
    int32_t lv = record.level;
    int32_t perLv = sb32(medalRow, MEDAL_SELL_PER_LV);
    data->sellPrice = int32_t(uint32_t(perLv) * uint32_t(lv - 1) + uint32_t(sell));
    if (outCtrl) {
        guest::addRef(outCtrl);
        guest::addRef(outCtrl);
    }
    // experience progress: calcMedalExp(medal, record, [data](level, exp, levelExp, nextExp){...}, 0)
    {
        uint32_t fn = inlineFunction(f, 0x40, VT_EXP_LAMBDA, out->obj, outCtrl);
        rw::call(CALC_MEDAL_EXP, {medal, rw::guest_addr(&record), fn, 0});
        destroyFunction(fn);
    }
    uint32_t api2 = rw::call(APIMANAGER_GET_INSTANCE, {});
    int32_t slot = int32_t(rw::call(FIND_MEDAL_SET_KEYBLADE_INDEX, {api2, 0, uint32_t(id), uint32_t(id >> 32)}));
    data->getDatetime = record.getDatetime;
    data->locked = record.lock == 1 ? 1 : 0;
    data->isNew = isNew ? 1 : 0;
    data->inDeck = slot >= 0 ? 1 : 0;
    data->upperCost = int32_t(0u - uint32_t(int32_t(record.upperCost)));
    data->attackUpperNumber = record.attackUpperNumber;
    data->defenseUpperNumber = record.defenseUpperNumber;
    for (uint32_t k = 0; k < 4; k++) data->userSkillIds[k] = rw::get<uint32_t>(&record, 48 + 4 * k);
    data->skillCount = record.skillCount;
    data->burstUpperNumber = record.burstUpperNumber;
    data->guiltFactor = record.guiltFactor;
    if (outCtrl) {
        guest::release(outCtrl);
        guest::release(outCtrl);
    }
    if (rw::load<uint32_t>(medal + 4)) guest::release(rw::load<uint32_t>(medal + 4));
}
RW_FUNCTION(0x76ADB4, "MedalDataManager::getMedalData", MedalDataManager::getMedalData);

uint32_t MedalDataManager::getMedalSmallImagePath(uint32_t stringBuffer, const guest::SharedPtr *data) {
    uint32_t row = rw::host_ptr<UserMedalData>(data->obj)->medal.obj;
    return rw::call(STRINGBUFFER_FSET, {stringBuffer, STR_MEDAL_S, uint32_t(sb32(row, MEDAL_THUMB_ID))});
}
RW_FUNCTION(0x76B17C, "MedalDataManager::getMedalSmallImagePath", MedalDataManager::getMedalSmallImagePath);

uint32_t MedalDataManager::getMedalLargeImagePath(uint32_t stringBuffer, const guest::SharedPtr *data) {
    uint32_t row = rw::host_ptr<UserMedalData>(data->obj)->medal.obj;
    return rw::call(STRINGBUFFER_FSET, {stringBuffer, STR_MEDAL_L, uint32_t(sb32(row, MEDAL_IMAGE_ID))});
}
RW_FUNCTION(0x76B1E4, "MedalDataManager::getMedalLargeImagePath", MedalDataManager::getMedalLargeImagePath);

// The favourite (lock) state as the menus show it: a locked medal stays favourite unless it is about to be
// unlocked; an unlocked one is favourite when it is about to be locked.
uint32_t MedalDataManager::isFavoriteMedal(uint32_t, const guest::SharedPtr *data) {
    const UserMedalData *d = rw::host_ptr<UserMedalData>(data->obj);
    uint8_t locked = d->locked;
    uint32_t app = rw::call(APPSTATE_GET_INSTANCE, {});
    uint64_t id = rw::load<uint64_t>(data->obj);
    if (locked) {
        const guest::Vector *v = rw::host_ptr<guest::Vector>(rw::call(APPSTATE_WILL_UNLOCK_IDS, {app}));
        return containsId(v, id) ? 0 : 1;
    }
    const guest::Vector *v = rw::host_ptr<guest::Vector>(rw::call(APPSTATE_WILL_LOCK_IDS, {app}));
    return containsId(v, id) ? 1 : 0;
}
RW_FUNCTION(0x76B248, "MedalDataManager::isFavoriteMedal", MedalDataManager::isFavoriteMedal);

// ------------------------------------------------------------------------------------------ lambda thunks
void MedalDataManager::StatusLambda::call(uint32_t self, const int32_t *attack, const int32_t *defense) {
    int32_t d = *defense;
    UserMedalData *data = rw::host_ptr<UserMedalData>(rw::load<uint32_t>(self + 4));
    data->attack = *attack;
    rw::host_ptr<UserMedalData>(rw::load<uint32_t>(self + 4))->defense = d;
}

void MedalDataManager::ExpLambda::call(uint32_t self, const int32_t *, const int32_t *exp, const int32_t *levelExp,
                                       const int32_t *nextLevelExp) {
    int32_t next = *nextLevelExp, prev = *levelExp, e = *exp;
    UserMedalData *data = rw::host_ptr<UserMedalData>(rw::load<uint32_t>(self + 4));
    data->exp = e;
    data->levelExp = prev;
    data->nextLevelExp = next;
}

namespace {
using StatusFunc = scene_menu::SharedCaptureFunc<VT_STATUS_LAMBDA>;
using ExpFunc = scene_menu::SharedCaptureFunc<VT_EXP_LAMBDA>;
uint32_t statusTarget(uint32_t self, uint32_t ti) { return scene_menu::funcTarget(self, ti, STATUS_LAMBDA_NAME); }
uint32_t expTarget(uint32_t self, uint32_t ti) { return scene_menu::funcTarget(self, ti, EXP_LAMBDA_NAME); }
uint32_t statusTargetType() { return STATUS_LAMBDA_TYPEINFO; }
uint32_t expTargetType() { return EXP_LAMBDA_TYPEINFO; }
}  // namespace

RW_FUNCTION(0x76B2B8, "MedalDataManager::getMedalData::$_1 __func::~__func", ExpFunc::destroyFunc);
RW_FUNCTION(0x76B2E4, "MedalDataManager::getMedalData::$_1 __func::~__func (deleting)", ExpFunc::deleteFunc);
RW_FUNCTION(0x76B314, "MedalDataManager::getMedalData::$_1 __func::__clone()", ExpFunc::clone);
RW_FUNCTION(0x76B354, "MedalDataManager::getMedalData::$_1 __func::__clone(dst)", ExpFunc::cloneInto);
RW_FUNCTION(0x76B380, "MedalDataManager::getMedalData::$_1 __func::destroy", ExpFunc::destroy);
RW_FUNCTION(0x76B38E, "MedalDataManager::getMedalData::$_1 __func::destroy_deallocate", ExpFunc::destroyDeallocate);
RW_FUNCTION(0x76B3A8, "MedalDataManager::getMedalData::$_1 __func::operator()", MedalDataManager::ExpLambda::call);
RW_FUNCTION(0x76B3C0, "MedalDataManager::getMedalData::$_1 __func::target", expTarget);
RW_FUNCTION(0x76B3E0, "MedalDataManager::getMedalData::$_1 __func::target_type", expTargetType);
RW_FUNCTION(0x76B3F4, "MedalDataManager::getMedalData::$_0 __func::~__func", StatusFunc::destroyFunc);
RW_FUNCTION(0x76B420, "MedalDataManager::getMedalData::$_0 __func::~__func (deleting)", StatusFunc::deleteFunc);
RW_FUNCTION(0x76B450, "MedalDataManager::getMedalData::$_0 __func::__clone()", StatusFunc::clone);
RW_FUNCTION(0x76B490, "MedalDataManager::getMedalData::$_0 __func::__clone(dst)", StatusFunc::cloneInto);
RW_FUNCTION(0x76B4BC, "MedalDataManager::getMedalData::$_0 __func::destroy", StatusFunc::destroy);
RW_FUNCTION(0x76B4CA, "MedalDataManager::getMedalData::$_0 __func::destroy_deallocate", StatusFunc::destroyDeallocate);
RW_FUNCTION(0x76B4E4, "MedalDataManager::getMedalData::$_0 __func::operator()", MedalDataManager::StatusLambda::call);
RW_FUNCTION(0x76B4F4, "MedalDataManager::getMedalData::$_0 __func::target", statusTarget);
RW_FUNCTION(0x76B514, "MedalDataManager::getMedalData::$_0 __func::target_type", statusTargetType);
