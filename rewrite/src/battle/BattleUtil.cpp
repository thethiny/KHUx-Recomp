#include "BattleUtil.hpp"

using namespace battle_util;

namespace {
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t master_MedalMisc_create = 0x7868C9;   // master::MedalMisc::create(int) (sret shared_ptr)
constexpr uint32_t libm_pow = 0x410654;                  // pow(double, double) (import)
constexpr uint32_t function_ii_call = 0x496B4D;          // std::function<void(int,int)>::operator()
constexpr uint32_t function_iiii_call = 0x496EBD;        // std::function<void(int,int,int,int)>::operator()
constexpr uint32_t add_shared = 0xC8DBB5;                // std::__shared_weak_count::__add_shared()
constexpr uint32_t medal_index_find = 0x490275;          // unordered_map<unsigned long long, int>::find

constexpr uint32_t APIMANAGER_USER_DATA = 828;       // SafeBuffer: support medal unique id (u64) at 8
constexpr uint32_t APIMANAGER_USER_MEDALS = 0x134;   // user medal list (296-byte records, id -> index map at +0x24)
constexpr uint32_t APIMANAGER_KEYBLADES = 0xEC;      // shared_ptr: user keyblade list (88-byte records)
constexpr uint32_t KEYBLADE_RECORD = 88;
constexpr uint32_t KEYBLADE_EQUIPPED = 76;           // byte in the record
constexpr uint32_t MEDAL_RECORD = 296;

int32_t sbGet(uint32_t obj, uint32_t index) { return bt::sbGet32(obj, index); }

// pow() of the guest C library (same rounding as the original)
double guestPow(double x, double y) {
    uint64_t a = bt::bits(x), b = bt::bits(y);
    return bt::dfromBits(rw::call64(libm_pow, {uint32_t(a), uint32_t(a >> 32), uint32_t(b), uint32_t(b >> 32)}));
}

// One medal stat at `level`: min + (max - min) * ((level - 1) / (maxLevel - 1)) ^ exponent
int32_t curveValue(uint32_t medal, uint32_t minIndex, uint32_t maxIndex, int32_t level, float exponent) {
    double base = double(sbGet(medal, minIndex));
    int32_t max = sbGet(medal, maxIndex);
    int32_t min = sbGet(medal, minIndex);
    int32_t maxLevel = sbGet(medal, MEDAL_MAX_LEVEL);
    float t = float(int32_t(uint32_t(level) - 1u)) / float(int32_t(uint32_t(maxLevel) - 1u));
    double grow = double(int32_t(uint32_t(max) - uint32_t(min))) * guestPow(double(t), double(exponent));
    return bt::d2iz(base + grow);
}
}  // namespace

void BattleUtil::calcMedalStatus(const bt::SharedPtr *medal, const uint8_t *userMedal, uint32_t onResult,
                                 const bt::SharedPtr *keyblade, int32_t slot) {
    bt::GuestFrame frame(8);
    auto *misc = frame.ptr<bt::SharedPtr>();
    int32_t curve = sbGet(medal->obj, MEDAL_STAT_CURVE);
    uint32_t miscId = uint32_t(curve - 1) <= 4 ? 100u + uint32_t(curve) : 101u;
    rw::call(master_MedalMisc_create, {frame.addr(), miscId});
    uint32_t miscRow = misc->obj;
    uint32_t miscCtrl = misc->ctrl;
    float exponent = float(sbGet(miscRow, 4)) / 10000.0f;

    int32_t level = rw::get<int32_t>(userMedal, USER_MEDAL_LEVEL);
    int32_t attack, defense;
    if (level == 1) {
        attack = sbGet(medal->obj, MEDAL_ATTACK_MIN);
        defense = sbGet(medal->obj, MEDAL_DEFENSE_MIN);
    } else {
        attack = curveValue(medal->obj, MEDAL_ATTACK_MIN, MEDAL_ATTACK_MAX, level, exponent);
        level = rw::get<int32_t>(userMedal, USER_MEDAL_LEVEL);
        defense = curveValue(medal->obj, MEDAL_DEFENSE_MIN, MEDAL_DEFENSE_MAX, level, exponent);
    }
    int32_t defenseTotal = int32_t(rw::get<uint32_t>(userMedal, USER_MEDAL_DEFENSE_BONUS) + uint32_t(defense));
    int32_t attackTotal = int32_t(rw::get<uint32_t>(userMedal, USER_MEDAL_ATTACK_BONUS) + uint32_t(attack));

    // keyblade multiplier (x10000)
    int32_t rate = 10000;
    uint32_t kb = keyblade->obj;
    if (kb) {
        if (slot < 0) {
            rate = sbGet(kb, KEYBLADE_SUPPORT_BONUS);
        } else {
            int32_t attribute = sbGet(medal->obj, MEDAL_ATTRIBUTE);
            int32_t type = sbGet(medal->obj, MEDAL_TYPE);
            int32_t slotType = sbGet(kb, KEYBLADE_SLOT_TYPE + 4u * uint32_t(slot));
            int32_t slotAttribute = sbGet(kb, KEYBLADE_SLOT_ATTRIBUTE + 4u * uint32_t(slot));
            if (attribute == slotAttribute) {
                uint32_t table = (slotType == 3 || slotType != type) ? KEYBLADE_SLOT_BONUS : KEYBLADE_SLOT_BONUS_TYPED;
                rate = sbGet(kb, table + 4u * uint32_t(slot));
            }
        }
    }
    int32_t attackFinal = int32_t(uint32_t(attackTotal) * uint32_t(rate)) / 10000;
    rw::call(function_ii_call, {onResult, uint32_t(attackFinal), uint32_t(defenseTotal)});
    bt::releaseShared(miscCtrl);
}
RW_FUNCTION(0x4966BC, "BattleUtil::calcMedalStatus", BattleUtil::calcMedalStatus);

int32_t BattleUtil::calcMedalExp(const bt::SharedPtr *medal, const uint8_t *userMedal, uint32_t onResult,
                                 int32_t exp) {
    int32_t level = rw::get<int32_t>(userMedal, USER_MEDAL_LEVEL);
    int32_t total = rw::get<int32_t>(userMedal, USER_MEDAL_EXP);
    if (exp >= 1) {
        int32_t maxLevel = sbGet(medal->obj, MEDAL_MAX_LEVEL);
        total = int32_t(uint32_t(total) + uint32_t(exp));
        while (level < maxLevel && total >= calcNeedExp(medal, level)) level++;
    }
    int32_t needPrev = calcNeedExp(medal, int32_t(uint32_t(level) - 1u));
    int32_t needNext = calcNeedExp(medal, level);
    if (rw::load<uint32_t>(onResult + 16) != 0)  // std::function not empty
        rw::call(function_iiii_call, {onResult, uint32_t(level), uint32_t(total), uint32_t(needPrev), uint32_t(needNext)});
    return level;
}
RW_FUNCTION(0x496BD0, "BattleUtil::calcMedalExp", BattleUtil::calcMedalExp);

// need = base * (level / (maxLevel - 1)) ^ (exponent / 10000), from MedalMisc row [maxLevel, base, exponent]
int32_t BattleUtil::calcNeedExp(const bt::SharedPtr *medal, int32_t level) {
    bt::GuestFrame frame(8);
    auto *misc = frame.ptr<bt::SharedPtr>();
    int32_t curve = sbGet(medal->obj, MEDAL_EXP_CURVE);
    uint32_t miscId = uint32_t(curve - 1) <= 5 ? 105u + uint32_t(curve) : 106u;
    rw::call(master_MedalMisc_create, {frame.addr(), miscId});
    uint32_t row = misc->obj;
    uint32_t ctrl = misc->ctrl;
    float levels = float(sbGet(row, 4));
    int32_t base = sbGet(row, 8);
    float exponent = float(sbGet(row, 12)) / 10000.0f;
    float t = float(level) / (levels + -1.0f);
    int32_t need = bt::d2iz(double(base) * guestPow(double(t), double(exponent)));
    bt::releaseShared(ctrl);
    return need;
}
RW_FUNCTION(0x496C98, "BattleUtil::calcNeedExp", BattleUtil::calcNeedExp);

uint32_t BattleUtil::getCurrentKeybladeIndex() {
    uint32_t api = rw::call(APIManager_getInstance, {});
    uint32_t ctrl = rw::load<uint32_t>(api + APIMANAGER_KEYBLADES + 4);
    uint32_t list = rw::load<uint32_t>(api + APIMANAGER_KEYBLADES);
    if (ctrl) rw::call(add_shared, {ctrl});     // local shared_ptr copy
    uint32_t found = 0;
    const SafeBuffer *buf = rw::host_ptr<SafeBuffer>(list);
    for (uint32_t k = 0; k < rw::load<uint32_t>(list + 32); k++) {     // record count after the SafeBuffer
        uint32_t index = k * KEYBLADE_RECORD + KEYBLADE_EQUIPPED;
        uint8_t equipped = uint8_t(buf->data[index] ^ SafeBuffer::mask(buf->key, index));
        if (equipped) { found = k; break; }
    }
    bt::releaseShared(ctrl);
    return found;
}
RW_FUNCTION(0x496F4C, "BattleUtil::getCurrentKeybladeIndex", BattleUtil::getCurrentKeybladeIndex);

uint32_t BattleUtil::getSupportMedal(const uint8_t *medal, const uint8_t *skill) {
    uint32_t medals = rw::call(APIManager_getInstance, {}) + APIMANAGER_USER_MEDALS;
    if (rw::load<uint32_t>(medals) == 0) return 0;
    uint32_t api = rw::call(APIManager_getInstance, {});
    uint32_t list = rw::load<uint32_t>(medals);
    bt::GuestFrame frame(8);
    // the friend's support medal unique id (u64) is the map key
    const SafeBuffer *user = rw::host_ptr<SafeBuffer>(api + APIMANAGER_USER_DATA);
    frame.setWord(0, uint32_t(user->get32(8)));
    frame.setWord(4, uint32_t(user->get32(12)));
    uint32_t node = rw::call(medal_index_find, {list + 0x24, frame.addr()});
    if (node == 0) return 0;
    int32_t index = rw::load<int32_t>(node + 16);
    if (index < 0) return 0;
    list = rw::load<uint32_t>(medals);
    uint8_t record[MEDAL_RECORD];
    const SafeBuffer *buf = rw::host_ptr<SafeBuffer>(list);
    uint32_t at = uint32_t(index) * MEDAL_RECORD;
    for (uint32_t k = 0; k < MEDAL_RECORD; k++) record[k] = uint8_t(buf->data[at + k] ^ SafeBuffer::mask(buf->key, at + k));
    for (uint32_t k = 0; k < 72; k += 4) rw::store32(medal + k, rw::get<uint32_t>(record, 72 + k));   // UserMedal::Medal
    uint32_t skillA = 0, skillB = 0;
    if (rw::get<uint32_t>(record, 0x90) != 0) {
        skillB = rw::get<uint32_t>(record, 0xA4);
        skillA = rw::get<uint32_t>(record, 0xA0);
    }
    rw::set<uint32_t>(skill, 8, skillA);
    rw::set<uint32_t>(skill, 12, skillB);
    return 1;
}
RW_FUNCTION(0x497010, "BattleUtil::getSupportMedal", BattleUtil::getSupportMedal);

int32_t BattleUtil::calc_many_lux_level(int32_t a, int32_t b, int32_t c) {
    return int32_t(uint32_t(a) * uint32_t(b) + uint32_t(c));
}
RW_FUNCTION(0x4970FC, "BattleUtil::calc_many_lux_level", BattleUtil::calc_many_lux_level);
