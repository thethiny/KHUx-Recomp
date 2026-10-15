// SceneUtil: deck (keyblade + medals) status and views.
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t APIManager_userMedals = 0xD4;           // shared_ptr<UserMedal> (a SafeBuffer of 72-byte medals)
constexpr uint32_t UserMedal_index = 0x24;                 // unordered_map<u64 medal id, int slot>
constexpr uint32_t MEDAL_SIZE = 72;                         // hole::network::api::UserMedal::Medal
constexpr uint32_t UserMedalData_row = 0x58;               // shared_ptr<master::Medal> (SafeBuffer)
constexpr uint32_t UserMedalData_costBonus = 0x28;
constexpr uint32_t UserKeyBladeData_row = 0x20;            // shared_ptr<master::Keyblade>
constexpr uint32_t MEDAL_KIND = 0x00, MEDAL_ATTACK = 0x490, MEDAL_DEFENCE = 0x49C, MEDAL_COST = 0x484;

constexpr uint32_t MedalIndex_find = 0x490275;             // unordered_map<unsigned long long, int>::find
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Memcpy = 0x40E164;                      // __aeabi_memcpy(dst, src, n)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;             // throws std::out_of_range("vector")
constexpr uint32_t BattleUtil_calcMedalStatus = 0x4966BD;  // (row, medal, function<void(int,int)>, shared_ptr<Keyblade>, int)
constexpr uint32_t NewArray = 0xC98F6F, DeleteArray = 0xC98FB3;
constexpr uint32_t VT_DeckStatusAdd = 0xF54AE0;            // calcDeckStatus $_34: [&attack, &defence] += (atk, def)

constexpr uint32_t STR_Txt_Power_Label = 0xDF8ECC, STR_Txt_Defence_Label = 0xDF8EE6, STR_d = 0xDF86DE;
}  // namespace

// Sum a deck's medal stats: out[0] attack, out[1] defence, out[2] cost. Medals of kind 1 count their master
// values; the others go through BattleUtil::calcMedalStatus with the keyblade's bonuses. The medal records come
// from `medalList` (by index) when given, else from the user's medal box (by id).
void SceneUtil::calcDeckStatus(Addr keyblade, Addr medals, Addr out, Addr medalList) {
    // the original's frame, at its addresses: the medal record is not reset between medals (an unknown medal
    // keeps the previous one's, or the stale stack bytes)
    Frame frame;
    const uint32_t base = frame.allocExact(0x138);
    const uint32_t kbCopy = base + 0x40, defence = base + 0x48, attack = base + 0x4C, key = base + 0x50;
    const uint32_t addFn = base + 0x70, decoded = base + 0x88, medal = base + 0xD0;
    st32(defence, 0);
    st32(attack, 0);
    int32_t cost = 0;
    if (ld32(medals + 4) != ld32(medals)) {
        uint32_t offset = 0;
        for (uint32_t index = 0;; index++, offset += MEDAL_SIZE) {
            uint32_t entry = ld32(medals) + 8 * index;
            uint32_t user = ld32(entry), userCtl = ld32(entry + 4);
            if (userCtl) call(fn::shared_add, {userCtl});
            uint32_t a = call(fn::APIManager_getInstance, {});
            uint32_t listCtl = ld32(a + APIManager_userMedals + 4), list = ld32(a + APIManager_userMedals);
            if (listCtl) call(fn::shared_add, {listCtl});
            if (medalList) {
                if (((ld32(medalList + 4) - ld32(medalList)) >> 3) * 0x38E38E39u <= index)
                    call(ThrowOutOfRange, {medalList});
                call(Memcpy, {medal, ld32(medalList) + offset, MEDAL_SIZE});
            } else {
                st32(key, ld32(user));
                st32(key + 4, ld32(user + 4));
                uint32_t node = call(MedalIndex_find, {list + UserMedal_index, key});
                if (node) {
                    int32_t slot = int32_t(ld32(node + 0x10));
                    if (slot >= 0) {
                        uint32_t at = MEDAL_SIZE * uint32_t(slot);
                        rw::call_exact(SafeBuffer_read, {list, decoded, ld32(list + 12) + at, MEDAL_SIZE, at});
                        call(Memcpy, {medal, decoded, MEDAL_SIZE});
                    }
                }
            }
            uint32_t rowRef = user + UserMedalData_row;
            int32_t c;
            if (safe32(ld32(rowRef), MEDAL_KIND) == 1) {
                st32(attack, ld32(attack) + uint32_t(safe32(ld32(rowRef), MEDAL_ATTACK)));
                st32(defence, ld32(defence) + uint32_t(safe32(ld32(rowRef), MEDAL_DEFENCE)));
                c = safe32(ld32(rowRef), MEDAL_COST);
            } else {
                st32(addFn + 16, addFn);
                st32(addFn, VT_DeckStatusAdd);
                st32(addFn + 4, attack);
                st32(addFn + 8, defence);
                uint32_t kb = ld32(keyblade);
                st32(kbCopy, ld32(kb + UserKeyBladeData_row));
                uint32_t kbCtl = ld32(kb + UserKeyBladeData_row + 4);
                st32(kbCopy + 4, kbCtl);
                if (kbCtl) call(fn::shared_add, {kbCtl});
                rw::call_exact(BattleUtil_calcMedalStatus, {rowRef, medal, addFn, kbCopy, index});
                if (ld32(kbCopy + 4)) call(fn::shared_release, {ld32(kbCopy + 4)});
                functionDestroy(addFn);
                c = int32_t(uint32_t(safe32(ld32(rowRef), MEDAL_COST)) - ld32(user + UserMedalData_costBonus));
            }
            cost = int32_t(uint32_t(c) + uint32_t(cost));
            if (listCtl) call(fn::shared_release, {listCtl});
            if (userCtl) call(fn::shared_release, {userCtl});
            if (index + 1 >= (ld32(medals + 4) - ld32(medals)) >> 3) break;
        }
    }
    st32(out, ld32(attack));
    st32(out + 4, ld32(defence));
    st32(out + 8, uint32_t(cost));
}
RW_FUNCTION(0x469CB4, "SceneUtil::calcDeckStatus", SceneUtil::calcDeckStatus);

// The deck's attack / defence labels.
void SceneUtil::updateDeckStauts(Addr panel, Addr keyblade, Addr medals) {
    // calcDeckStatus reads stale stack bytes: call it from the original frame position
    Frame frame;
    const uint32_t base = frame.allocExact(0x870);
    const uint32_t kbCopy = base + 0x48;
    uint32_t status = call(NewArray, {12});
    st32(kbCopy, ld32(keyblade));
    uint32_t ctl = ld32(keyblade + 4);
    st32(kbCopy + 4, ctl);
    if (ctl) call(fn::shared_add, {ctl});
    calcDeckStatus(kbCopy, medals, status, 0);
    if (ctl) call(fn::shared_release, {ctl});
    int32_t attack = int32_t(ld32(status)), defence = int32_t(ld32(status + 4));
    call(DeleteArray, {status});
    uint32_t power = findNode(panel, STR_Txt_Power_Label, 15);
    setShadow(power);
    {
        Frame g;
        setTextC(power, stringBuffer(g, {STR_d, uint32_t(attack)}));
    }
    uint32_t def = findNode(panel, STR_Txt_Defence_Label, 17);
    setShadow(def);
    {
        Frame g;
        setTextC(def, stringBuffer(g, {STR_d, uint32_t(defence)}));
    }
}
RW_FUNCTION(0x46A4AC, "SceneUtil::updateDeckStauts", SceneUtil::updateDeckStauts);
