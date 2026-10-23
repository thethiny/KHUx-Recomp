// SceneAdventureSelect: the raid boss result sequence (boss appearing / escaping / defeated, the reward popup),
// the start deck dialog.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {
constexpr uint32_t RaidBossLwfDialog_create = 0x59734D;
constexpr uint32_t RaidBossLwfDialog_open = 0x5973ED;    // (dialog, Node*, RaidBossLwfType, int, int, function)
constexpr uint32_t RaidBossResultPopUp_popUpRaidBossResult = 0x5B6DA9;   // (int, const vector<int>&, function)
constexpr uint32_t StartDeckEditDialog_create = 0x5735DF;
constexpr uint32_t StartDeckEditDialog_open = 0x573851;  // (dialog, Node*, StageType, int stageId, function<void(bool)>)
constexpr uint32_t StartDeckEditDialog_setCloseEvent = 0x57683D;
constexpr uint32_t STAGE_TYPE_TABLE = 0xE9A460;          // StartDeckEditDialog::StageType per AdventureType 0..6
constexpr uint32_t SM_RAID_BOSS_NEW = 0x2F5;             // byte: a raid boss appeared (play its animation once)
constexpr uint32_t API_RAID_REWARDS = 0x168;             // std::vector<...>* (176-byte entries)
constexpr uint32_t RAID_INFO_ENEMY = 0x24;               // RaidBase::Info: boss enemy id
constexpr uint32_t RAID_INFO_LEVEL = 0x2C;               // RaidBase::Info: (second RaidBossLwfDialog::open value)

// RaidBossLwfDialog::open(create(), popup layer 2, type, info +0x24, info +0x2C, lambda {self, done}) with the
// lambda in a 0x28-byte heap __func (self at +8, the moved std::function at +0x10)
void openRaidDialog(uint32_t self, uint32_t done, uint32_t type, uint32_t vtable, uint32_t enemy, uint32_t level,
                    uint32_t dialog, uint32_t layer) {
    Frame f;
    uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    uint32_t cb = f.alloc(20);
    st32(cb + 16, 0);
    uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, vtable);
        st32(obj + 8, self);
        call(fn::function_vv_move, {obj + 0x10, copy});
        st32(obj + 8, self);
    }
    st32(cb + 16, obj);
    call(RaidBossLwfDialog_open, {dialog, layer, type, enemy, level, cb});
    functionDestroy(cb);
    functionDestroy(copy);
}
}  // namespace

// A raid boss appeared: its LWF animation (once; flag_29D makes the list jump to the boss), else the rewards.
void SceneAdventureSelect::raidBossAnim(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    if (!ld8(sceneManager() + SM_RAID_BOSS_NEW)) {
        callWithCopy(raidBossRewardPopup, self, done);
        return;
    }
    st8(sceneManager() + SM_RAID_BOSS_NEW, 0);
    s->flag_29D = 1;
    call(fn::APIManager_getInstance, {});
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t info = ld32(api + API_RAID_INFO);
    uint32_t dialog = call(RaidBossLwfDialog_create, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t level = ld32(info + RAID_INFO_LEVEL);
    uint32_t enemy = ld32(ld32(api + API_RAID_INFO) + RAID_INFO_ENEMY);
    openRaidDialog(self, done, 0, VT_raidBossAnim_53, enemy, level, dialog, layer);
}
RW_FUNCTION(0x726B60, "SceneAdventureSelect::raidBossAnim", SceneAdventureSelect::raidBossAnim);

// The raid boss escaped: its LWF animation (type 1).
void SceneAdventureSelect::raidBossEscapeAnim(Addr self, Addr done) {
    st8(sceneManager() + SM_RAID_BOSS_NEW, 0);
    uint32_t info = ld32(call(fn::APIManager_getInstance, {}) + API_RAID_INFO);
    uint32_t dialog = call(RaidBossLwfDialog_create, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t level = ld32(info + RAID_INFO_LEVEL);
    uint32_t enemy = ld32(info + RAID_INFO_ENEMY);
    openRaidDialog(self, done, 1, VT_raidBossEscapeAnim_54, enemy, level, dialog, layer);
}
RW_FUNCTION(0x726D18, "SceneAdventureSelect::raidBossEscapeAnim", SceneAdventureSelect::raidBossEscapeAnim);

// The raid boss was defeated: straight to the rewards.
void SceneAdventureSelect::raidBossWinAnim(Addr self, Addr done) {
    st8(sceneManager() + SM_RAID_BOSS_NEW, 0);
    callWithCopy(raidBossRewardPopup, self, done);
}
RW_FUNCTION(0x726E64, "SceneAdventureSelect::raidBossWinAnim", SceneAdventureSelect::raidBossWinAnim);

// The raid result popup over every pending reward (indices 0 .. n - 1), then lambda $_55 {self, done}.
void SceneAdventureSelect::raidBossRewardPopup(Addr self, Addr done) {
    Frame f;
    uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t rewards = ld32(call(fn::APIManager_getInstance, {}) + API_RAID_REWARDS);
    if (rewards) {
        uint32_t index = f.alloc(4);
        st32(index, 0);
        if (int32_t(ld32(rewards + 4) - ld32(rewards)) >= 1) {
            for (uint32_t k = 1;; k++) {
                uint32_t end = ld32(ids + 4);
                if (end == ld32(ids + 8)) {
                    call(lib::IntVector_pushBackSlow, {ids, index});
                } else {
                    if (end) st32(end, k - 1);
                    st32(ids + 4, (end ? ld32(ids + 4) : 0) + 4);
                }
                st32(index, k);
                uint32_t n = uint32_t(int32_t(ld32(rewards + 4) - ld32(rewards)) >> 4) * 0xBA2E8BA3u;
                if (int32_t(k) >= int32_t(n)) break;
            }
        }
    }
    // the lambda {self, std::function done} built on the stack, then moved into the heap __func
    uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, done});
    uint32_t lambda = f.alloc(0x20);       // self at +0, the std::function at +8 (0x1C bytes)
    functionCopy(lambda + 8, doneCopy);
    uint32_t cb = f.alloc(20);
    st32(cb + 16, 0);
    uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, VT_raidBossRewardPopup_55);
        st32(obj + 8, self);
        call(fn::function_vv_move, {obj + 0x10, lambda + 8});
        st32(obj + 8, self);
    }
    st32(cb + 16, obj);
    call(RaidBossResultPopUp_popUpRaidBossResult, {0, ids, cb});
    functionDestroy(cb);
    functionDestroy(lambda + 8);
    call(fn::APIManager_getInstance, {});
    uint32_t app = call(fn::AppState_getInstance, {});
    call(lib::AppState_isClearTutorialPopupIDs, {app, 24});
    functionDestroy(doneCopy);
    call(lib::IntVector_dtor, {ids});
}
RW_FUNCTION(0x726F10, "SceneAdventureSelect::raidBossRewardPopup", SceneAdventureSelect::raidBossRewardPopup);

// The deck check before a quest: StartDeckEditDialog for the stage (field_298) with the StageType of the shown
// AdventureType; field_2C8 holds the dialog while it is open.
void SceneAdventureSelect::openPopupStartDeckEdit(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    // types above 6 pass what the original had in r1: a guest stack address inside its frame (sp + 0x28 after
    // the 0x14-byte push and the 0x54-byte frame)
    uint32_t stageType = rw::cpu->r[13] - 0x14 - 0x54 + 0x28;
    if (s->field_2C8) return;
    uint32_t type = uint32_t(s->adventureType);
    if (type <= 6) stageType = ld32(STAGE_TYPE_TABLE + type * 4);
    uint32_t dialog = call(StartDeckEditDialog_create, {});
    s->field_2C8 = dialog;
    uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    uint32_t opened;
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_openPopupStartDeckEdit_56, {self});
        opened = call(StartDeckEditDialog_open, {dialog, layer, stageType, s->field_298, cb});
        functionDestroy(cb);
    }
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_openPopupStartDeckEdit_57, {self});
        call(StartDeckEditDialog_setCloseEvent, {s->field_2C8, cb});
        functionDestroy(cb);
    }
    if (!opened) {
        s->field_2C8 = 0;
        s->flag_324 = 0;
    }
}
RW_FUNCTION(0x727128, "SceneAdventureSelect::openPopupStartDeckEdit", SceneAdventureSelect::openPopupStartDeckEdit);
