// SceneAvatarEdit::initAvatarPartsList: the part ids of each tab. The parts to list are the initial items (first
// avatar: InitItem rows of type 100) or the user's avatar parts (their records, part id at +0xC). A local lambda
// (its body is sub_706610, called directly) files each part by its type (+0x88) into the tab lists at +0x2F4 and sets
// the tab's "new" mark (the gender's bit vector) when the part is in AppState's new parts. Type 2 (an accessory
// set) goes to tab 1, and also to tab 0 when every accessory of the set is owned (new marks from the new sets).
// Hidden parts (+0x94 == 1) are skipped. Finally each tab list is sorted.
#include "../scene_menu/SceneAvatarEdit.hpp"

using namespace scene;
using Addr = SceneAvatarEdit::Addr;

namespace {
namespace g {
constexpr uint32_t AppState_getNewAvatarPartsIDs = 0x4EBB45;     // -> const vector<int>*
constexpr uint32_t AppState_getNewAvatarPartsSetIDs = 0x4EBCBF;
constexpr uint32_t InitItem_getIds = 0x7839E5;                   // (set<int> out)
constexpr uint32_t InitItem_create = 0x783A01;                   // (shared_ptr out, id)
constexpr uint32_t IntSet_destroy = 0x48D9B5;                    // __tree<int>::destroy(tree, node)
constexpr uint32_t VectorInt_copy = 0x4AF707;                    // vector<int>(const vector<int>&)
constexpr uint32_t VectorInt_dtor = 0x478961;
constexpr uint32_t VectorInt_pushBackSlowMove = 0x47D709;        // __push_back_slow_path(int&&)
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;            // __push_back_slow_path(const int&)
constexpr uint32_t AvatarParts_create = 0x778DE1;
constexpr uint32_t SceneAvatarEdit_generateAccessories = 0x708DA1;   // (this, id, vector<shared_ptr> out, female)
constexpr uint32_t VectorPartsPtr_dtor = 0x709625;
constexpr uint32_t VectorPartsPtr_pushBackSlow = 0x70B8D1;        // vector<shared_ptr<const AvatarParts>> (const&)
constexpr uint32_t SortInts = 0x70CD15;                          // sort(begin, end)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
}  // namespace g

constexpr uint32_t INIT_ITEM_TYPE = 4, INIT_ITEM_VALUE = 0x14, INIT_ITEM_AVATAR_PART = 100;
constexpr uint32_t API_AVATAR_PARTS = 0xCC;                 // APIManager shared_ptr<SafeBuffer> (0x18-byte records)
constexpr uint32_t PART_RECORD = 0x18, PART_RECORD_ID = 0xC, SB_COUNT = 0x20;
constexpr uint32_t PARTS_TYPE = 0x88, PARTS_GENDER = 0x8C, PARTS_HIDDEN = 0x94;
constexpr uint32_t PARTS_IS_CHILD = 0x9C, PARTS_IN_SET = 0xA0, PARTS_SET_COUNT = 0xA4, PARTS_SET_IDS = 0xA8;
constexpr uint32_t TYPE_ACCESSORY_SET = 2;
constexpr uint32_t SAE_IS_CREATE = 0x2E0, SAE_NEW_MALE = 0x2C8, SAE_NEW_FEMALE = 0x2D4, SAE_TAB_LISTS = 0x2F4;
// the generateAccessories call takes the switch's jump target in r0 (a leftover; the callee does not read it)
constexpr uint32_t ACCESSORY_CASE_R0 = 0x7066F8;

// the lambda's captures (initAvatarPartsList's frame): new part ids, this, initial items, the user's parts
// (shared_ptr), new set ids, this
constexpr uint32_t C_NEW_IDS = 0, C_SELF = 0xC, C_INIT_ITEMS = 0x10, C_PARTS = 0x1C, C_PARTS_CTRL = 0x20,
                   C_NEW_SETS = 0x24, C_SELF2 = 0x30, C_SIZE = 0x34;

bool contains(uint32_t vec, uint32_t value) {
    for (uint32_t p = ld32(vec), e = ld32(vec + 4); p != e; p += 4)
        if (ld32(p) == value) return true;
    return false;
}

// vector<int>::push_back(const int&) as compiled inline: the slow path when full
void pushInt(uint32_t vec, uint32_t valueRef) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(g::VectorInt_pushBackSlow, {vec, valueRef});
        return;
    }
    uint32_t base = 0;
    if (end) {
        st32(end, ld32(valueRef));
        base = ld32(vec + 4);
    }
    st32(vec + 4, base + 4);
}

// the tab list `tab` of the editor (bounds-checked like vector::at)
uint32_t tabList(uint32_t self, uint32_t tab) {
    const uint32_t lists = self + SAE_TAB_LISTS;
    if (!((ld32(lists + 4) - ld32(lists)) / 12 > tab)) call(g::ThrowOutOfRange, {lists});
    return ld32(lists) + 12 * tab;
}

// the tab's "new" mark in the gender's bit vector (vector<bool>::at)
void markNew(uint32_t self, uint32_t gender, uint32_t tab) {
    const uint32_t bits = self + (gender == 1 ? SAE_NEW_MALE : SAE_NEW_FEMALE);
    if (!(ld32(bits + 4) > tab)) call(g::ThrowOutOfRange, {bits});
    const uint32_t word = ld32(bits), v = ld32(word);
    if (!(v & (1u << tab))) st32(word, v | (1u << tab));
}

// is part `pid` owned: among the initial items (first avatar) or the user's part records
bool owned(uint32_t closure, uint32_t pid) {
    if (ld8(ld32(closure + C_SELF) + SAE_IS_CREATE)) return contains(closure + C_INIT_ITEMS, pid);
    const uint32_t sb = ld32(closure + C_PARTS);
    for (uint32_t k = 0, n = ld32(sb + SB_COUNT); k < n; k++)
        if (uint32_t(safe32(sb, PART_RECORD * k + PART_RECORD_ID)) == pid) return true;
    return false;
}

// vector<shared_ptr<const AvatarParts>>::push_back(const&) as compiled inline (a copy: the owner count goes up)
void pushPart(uint32_t vec, uint32_t sp) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(g::VectorPartsPtr_pushBackSlow, {vec, sp});
        return;
    }
    if (end) {
        st32(end, ld32(sp));
        st32(end + 4, ld32(sp + 4));
        if (ld32(sp + 4)) call(fn::shared_add, {ld32(sp + 4)});
    }
    st32(vec + 4, ld32(vec + 4) + 8);
}

struct PartsList {
    // the lambda's body (sub_706610): file part `id`
    static void file(uint32_t closure, uint32_t id) {
        Frame f;
        uint32_t idRef = f.alloc(4);
        st32(idRef, id);
        const uint32_t self = ld32(closure + C_SELF);
        uint32_t row = masterRow(f, g::AvatarParts_create, id);
        const uint32_t sb = ld32(row);
        const uint32_t gender = uint32_t(safe32(sb, PARTS_GENDER));
        if (safe32(sb, PARTS_HIDDEN) != 1) {
            const bool isNew = contains(closure + C_NEW_IDS, id);
            const uint32_t type = uint32_t(safe32(sb, PARTS_TYPE));
            // types 3..7 -> tabs 3, 5, 6, 4, 2
            static const int8_t TAB[8] = {-1, -1, 1, 3, 5, 6, 4, 2};
            if (type == TYPE_ACCESSORY_SET) {
                uint32_t parts = f.alloc(12);
                st32(parts, 0);
                st32(parts + 4, 0);
                st32(parts + 8, 0);
                call(g::SceneAvatarEdit_generateAccessories, {ACCESSORY_CASE_R0, id, parts, 0});
                const uint32_t n = (ld32(parts + 4) - ld32(parts)) / 8;
                bool all = n != 0;
                for (uint32_t k = 0; k < n && all; k++)
                    all = owned(closure, uint32_t(safe32(ld32(ld32(parts) + 8 * k), 0)));
                if (all) {
                    pushInt(tabList(self, 0), idRef);
                    if (contains(closure + C_NEW_SETS, id)) markNew(ld32(closure + C_SELF2), gender, 0);
                }
                pushInt(tabList(self, 1), idRef);
                if (isNew) markNew(ld32(closure + C_SELF2), gender, 1);
                call(g::VectorPartsPtr_dtor, {parts});
            } else if (type >= 3 && type <= 7) {
                const uint32_t tab = uint32_t(TAB[type]);
                pushInt(tabList(self, tab), idRef);
                if (isNew) markNew(ld32(closure + C_SELF2), gender, tab);
            }
        }
        sharedRelease(row);
    }

    static void init(SceneAvatarEdit *editor) {
        const uint32_t self = rw::guest_addr(editor);
        const uint32_t newIds = call(g::AppState_getNewAvatarPartsIDs, {call(fn::AppState_getInstance, {})});
        const uint32_t newSets = call(g::AppState_getNewAvatarPartsSetIDs, {call(fn::AppState_getInstance, {})});
        Frame f;
        uint32_t items = f.alloc(12);
        st32(items, 0);
        st32(items + 4, 0);
        st32(items + 8, 0);
        if (ld8(self + SAE_IS_CREATE)) {
            uint32_t set = f.alloc(12);
            call(g::InitItem_getIds, {set});
            // in-order walk of the set's red-black tree (begin at +0, the end node at +4)
            uint32_t node = ld32(set);
            while (node != set + 4) {
                {
                    Frame t;
                    uint32_t row = masterRow(t, g::InitItem_create, ld32(node + 0x10));
                    if (safe32(ld32(row), INIT_ITEM_TYPE) == int32_t(INIT_ITEM_AVATAR_PART)) {
                        uint32_t value = t.alloc(4);
                        st32(value, uint32_t(safe32(ld32(row), INIT_ITEM_VALUE)));
                        const uint32_t end = ld32(items + 4);
                        if (end < ld32(items + 8)) {
                            uint32_t base = 0;
                            if (end) {
                                st32(end, ld32(value));
                                base = ld32(items + 4);
                            }
                            st32(items + 4, base + 4);
                        } else call(g::VectorInt_pushBackSlowMove, {items, value});
                    }
                    sharedRelease(row);
                }
                if (ld32(node + 4)) {
                    node = ld32(node + 4);
                    while (ld32(node)) node = ld32(node);
                } else {
                    uint32_t from = node;
                    do {
                        node = ld32(from + 8);
                        const bool left = ld32(node) == from;
                        from = node;
                        if (left) break;
                    } while (true);
                }
            }
            call(g::IntSet_destroy, {set, ld32(set + 4)});
        }
        const uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t closure = f.alloc(C_SIZE);
        call(g::VectorInt_copy, {closure + C_NEW_IDS, newIds});
        st32(closure + C_SELF, self);
        call(g::VectorInt_copy, {closure + C_INIT_ITEMS, items});
        st32(closure + C_PARTS, ld32(api + API_AVATAR_PARTS));
        st32(closure + C_PARTS_CTRL, ld32(api + API_AVATAR_PARTS + 4));
        if (ld32(closure + C_PARTS_CTRL)) call(fn::shared_add, {ld32(closure + C_PARTS_CTRL)});
        call(g::VectorInt_copy, {closure + C_NEW_SETS, newSets});
        st32(closure + C_SELF2, self);
        if (ld8(self + SAE_IS_CREATE)) {
            for (uint32_t k = 0; k < (ld32(items + 4) - ld32(items)) / 4; k++) file(closure, ld32(ld32(items) + 4 * k));
        } else {
            const uint32_t partsRef = api + API_AVATAR_PARTS;
            for (uint32_t k = 0; k < ld32(ld32(partsRef) + SB_COUNT); k++)
                file(closure, uint32_t(safe32(ld32(partsRef), PART_RECORD * k + PART_RECORD_ID)));
        }
        const uint32_t lists = self + SAE_TAB_LISTS;
        for (uint32_t k = 0; k < (ld32(lists + 4) - ld32(lists)) / 12; k++) {
            const uint32_t list = ld32(lists) + 12 * k;
            call(g::SortInts, {ld32(list), ld32(list + 4)});
        }
        call(g::VectorInt_dtor, {closure + C_NEW_SETS});
        if (ld32(closure + C_PARTS_CTRL)) call(fn::shared_release, {ld32(closure + C_PARTS_CTRL)});
        call(g::VectorInt_dtor, {closure + C_INIT_ITEMS});
        call(g::VectorInt_dtor, {closure + C_NEW_IDS});
        call(g::VectorInt_dtor, {items});
    }
};
}  // namespace

void SceneAvatarEdit::initAvatarPartsList(SceneAvatarEdit *self) { PartsList::init(self); }

RW_FUNCTION(0x7051F8, "SceneAvatarEdit::initAvatarPartsList", SceneAvatarEdit::initAvatarPartsList);
RW_FUNCTION(0x706610, "SceneAvatarEdit::initAvatarPartsList lambda body (sub_706610)", PartsList::file);

// generateAccessories(id, out, female): the parts an accessory pick stands for. A set (type 2): its members - all
// of them unless `female`, else those that are no child part, in a set of their own and listed under it. A single
// accessory: itself (a child part, +0x9C == 0 while in a set: its parent instead), and when it belongs to a set,
// the members listed under it the same way.
void SceneAvatarEdit::generateAccessories(SceneAvatarEdit *, int32_t partsId, Addr out, uint32_t female) {
    Frame f;
    uint32_t row = masterRow(f, g::AvatarParts_create, uint32_t(partsId));
    uint32_t owner = uint32_t(partsId);
    bool all = false, members = false;
    if (safe32(ld32(row), PARTS_TYPE) == int32_t(TYPE_ACCESSORY_SET)) {
        all = (female ^ 1) != 0;
        members = true;
    } else {
        bool inSet = false;
        if (safe32(ld32(row), PARTS_IN_SET) == 1) {
            inSet = true;
            if (safe32(ld32(row), PARTS_IS_CHILD) == 0) {
                owner = uint32_t(safe32(ld32(row), PARTS_SET_IDS));
                Frame t;
                uint32_t parent = masterRow(t, g::AvatarParts_create, owner);
                const uint32_t oldCtrl = ld32(row + 4);
                st32(row, ld32(parent));
                st32(row + 4, ld32(parent + 4));
                if (oldCtrl) call(fn::shared_release, {oldCtrl});
            }
        }
        pushPart(out, row);
        members = inSet;
    }
    if (members) {
        const int32_t n = safe32(ld32(row), PARTS_SET_COUNT);
        for (int32_t k = 0; k < n; k++) {
            Frame t;
            uint32_t member = masterRow(t, g::AvatarParts_create, uint32_t(safe32(ld32(row), PARTS_SET_IDS + 4 * uint32_t(k))));
            const uint32_t m = ld32(member);
            if (all || (safe32(m, PARTS_IS_CHILD) != 1 && safe32(m, PARTS_SET_COUNT) == 1 &&
                        uint32_t(safe32(m, PARTS_SET_IDS)) == owner))
                pushPart(out, member);
            sharedRelease(member);
        }
    }
    sharedRelease(row);
}

RW_FUNCTION(0x708DA0, "SceneAvatarEdit::generateAccessories(int, vector<shared_ptr<const AvatarParts>>&, bool)",
            SceneAvatarEdit::generateAccessories);
