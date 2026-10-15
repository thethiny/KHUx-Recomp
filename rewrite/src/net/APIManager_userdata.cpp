// APIManager: local updates of the cached user data (the parsed responses kept in APIManager). The lists
// (UserKeyblade, UserMedal, UserSkill, UserMaterial, UserTitle, UserAvatarParts) are SafeBuffers of fixed-size
// elements with the element count at +0x20; an update copies the list into a std::vector element by element,
// replacing (or appending) the changed element, and rebuilds the list with T::create(vector).
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t SAFEBUFFER_READ = 0x45A269;            // SafeBuffer::read(dst, src, n, index)
constexpr uint32_t SAFEBUFFER_WRITE = 0x459FA1;           // SafeBuffer::write(dst, src, n, index)
constexpr uint32_t LIST_COUNT = 0x20;                     // element count of a user-data list
constexpr uint32_t MASTER_KEYBLADE_CREATE = 0x783F9D;     // master::Keyblade::create(int id)
constexpr uint32_t MASTER_AVATAR_PARTS_CREATE = 0x778DE1; // master::AvatarParts::create(int id)
constexpr uint32_t TRACKING_SEND_ONCE = 0x4E04E5;         // TrackingSDKUtil::sendEventOnlyOnce(const char*, const char*)
constexpr uint32_t APPSTATE_ADD_NEW_MEDAL = 0x4EB259;     // AppState::addNewUserMedalID(unsigned long long)
constexpr uint32_t APPSTATE_ADD_NEW_PARTS = 0x4EBBDD;     // AppState::addNewAvatarPartsIDs(int)
constexpr uint32_t APPSTATE_ADD_NEW_PARTS_SET = 0x4EBD59; // AppState::addNewAvatarPartsSetIDs(int)
constexpr uint32_t ID_SET_FIND = 0x4D4AC1;                // std::unordered_set<unsigned long long>::find (const)
constexpr uint32_t INT_MAP_FIND_CONST = 0x497A21;         // std::unordered_map<int,int>::find (const)
constexpr uint32_t S_UNLOCK_KEYBLADE_SLOT = 0xDFBDB4;     // "UnlockKeybladeSlot"

constexpr uint32_t MASTER_KEYBLADE_SLOTS = 0xDC;          // master::Keyblade: medal slot count
constexpr uint32_t MASTER_PARTS_SET_ID = 0x90;            // master::AvatarParts: the set this part belongs to
constexpr uint32_t MASTER_PARTS_SET_COUNT = 0xA4;         // master::AvatarParts (a set): number of parts
constexpr uint32_t MASTER_PARTS_SET_PARTS = 0xA8;         // master::AvatarParts (a set): part ids (5)

// One user-data list and the frame slots its update uses (the original's stack layout).
struct UserList {
    uint32_t member;        // offset of the shared_ptr<list> in APIManager
    uint32_t elemSize;
    uint32_t elemSlot;      // the element read from the list
    uint32_t vecSlot;       // the std::vector being built
    uint32_t retSlot;       // the shared_ptr returned by T::create
    uint32_t slowPath;      // std::vector<T>::__push_back_slow_path
    uint32_t create;        // T::create(const std::vector<T>&)
    uint32_t vectorDtor;    // std::__vector_base<T>::~__vector_base
};

constexpr UserList KEYBLADES{0xEC, 0x58, 0x50, 0x3C, 0x24, 0x4D4FCD, 0x4A10D9, 0x4C50FF};
constexpr UserList MEDALS{0xD4, 0x48, 0x38, 0x2C, 0x24, 0x4D4E59, 0x4A0551, 0x4C513D};
constexpr UserList SKILLS{0xDC, 0x28, 0x28, 0x50, 0x20, 0x487311, 0x4A091D, 0x46BE37};
constexpr UserList MATERIALS{0xE4, 0x10, 0x30, 0x40, 0x28, 0x4D5109, 0x4A0BFD, 0x4C50D1};
constexpr UserList TITLES{0x10C, 0x10, 0x30, 0x40, 0x28, 0x4D4B99, 0x4A1815, 0x4C51B9};
constexpr UserList AVATAR_PARTS{0xCC, 0x18, 0x68, 0x84, 0x60, 0x4D4CDD, 0x49FDA9, 0x4C517B};

StdSharedPtr &listOf(APIManager *self, const UserList &L) {
    return *rw::host_ptr<StdSharedPtr>(addr(self) + L.member);
}

void clearVector(const Frame &F, uint32_t slot) {
    F.setWord(slot + 0, 0);
    F.setWord(slot + 4, 0);
    F.setWord(slot + 8, 0);
}

// Element i of the list `sb` into the frame's element buffer; returns its guest address.
uint32_t readElement(const Frame &F, const UserList &L, uint32_t sb, uint32_t i) {
    uint32_t index = i * L.elemSize;
    F.call(SAFEBUFFER_READ, {sb, F.at(L.elemSlot), load32(sb + 12) + index, L.elemSize, index});
    return F.at(L.elemSlot);
}

uint64_t load64(uint32_t a) { return uint64_t(load32(a)) | (uint64_t(load32(a + 4)) << 32); }

void pushBack(const Frame &F, const UserList &L, uint32_t src) {
    pushBackCopy(F, F.at(L.vecSlot), src, L.elemSize, L.slowPath);
}

// The list = T::create(vector) (shared_ptr move-assignment).
void replaceList(const Frame &F, APIManager *self, const UserList &L) {
    F.call(L.create, {F.at(L.retSlot), F.at(L.vecSlot)});
    uint32_t ctrl = F.word(L.retSlot + 4), ptr = F.word(L.retSlot);
    F.setWord(L.retSlot, 0);
    F.setWord(L.retSlot + 4, 0);
    StdSharedPtr &list = listOf(self, L);
    list.ptr = ptr;
    uint32_t old = list.ctrl;
    list.ctrl = ctrl;
    if (old) {
        releaseShared(F, old);
        releaseIfSet(F, F.word(L.retSlot + 4));   // the moved-from temporary (always empty)
    }
}

// replaceList, then the vector is destroyed
void rebuild(const Frame &F, APIManager *self, const UserList &L) {
    replaceList(F, self, L);
    F.call(L.vectorDtor, {F.at(L.vecSlot)});
}

// Copy the list into the vector with the element whose 64-bit id equals the item's replaced by the item (onFound
// runs after that push); returns whether it was found.
template <class OnFound>
bool copyReplacing(const Frame &F, APIManager *self, const UserList &L, uint32_t item, OnFound onFound) {
    clearVector(F, L.vecSlot);
    uint32_t sb = listOf(self, L).ptr;
    const uint32_t count = load32(sb + LIST_COUNT);
    bool found = false;
    for (uint32_t i = 0; i < count; i++) {
        if (i) sb = listOf(self, L).ptr;
        uint32_t elem = readElement(F, L, sb, i);
        if (load64(elem) == load64(item)) {
            pushBack(F, L, item);
            onFound(elem);
            found = true;
        } else {
            pushBack(F, L, elem);
        }
    }
    return found;
}

}  // namespace

// ---------------------------------------------------------------------------------------------- lists
// A keyblade changed (enhanced): when its medal slot count grew, report "UnlockKeybladeSlot" once.
void APIManager::updateKeyblade(APIManager *self, uint32_t keyblade) {
    const UserList &L = KEYBLADES;
    Frame F(0xC8);
    bool found = copyReplacing(F, self, L, keyblade, [&](uint32_t old) {
        F.call(MASTER_KEYBLADE_CREATE, {F.at(0x34), load32(old + 0xC)});
        F.call(MASTER_KEYBLADE_CREATE, {F.at(0x2C), load32(keyblade + 0xC)});
        uint32_t before = F.word(0x34), after = F.word(0x2C);
        if (before && after) {
            int32_t slotsBefore = int32_t(sbWord(before, MASTER_KEYBLADE_SLOTS));
            int32_t slotsAfter = int32_t(sbWord(after, MASTER_KEYBLADE_SLOTS));
            if (slotsBefore < slotsAfter) F.call(TRACKING_SEND_ONCE, {S_UNLOCK_KEYBLADE_SLOT, 0});
        }
        releaseIfSet(F, F.word(0x30));
        releaseIfSet(F, F.word(0x38));
    });
    if (!found) pushBack(F, L, keyblade);
    rebuild(F, self, L);
}
RW_FUNCTION_NOVERIFY(0x4C40E0, "APIManager::updateKeyblade", APIManager::updateKeyblade);

// A medal changed or was obtained (a new one is also marked new in AppState).
void APIManager::updateMedal(APIManager *self, uint32_t medal) {
    const UserList &L = MEDALS;
    Frame F(0xA0);
    bool found = copyReplacing(F, self, L, medal, [](uint32_t) {});
    if (!found) {
        pushBack(F, L, medal);
        uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
        F.call(APPSTATE_ADD_NEW_MEDAL, {app, medal + 8, load32(medal), load32(medal + 4)});
    }
    rebuild(F, self, L);
}
RW_FUNCTION(0x4C42F4, "APIManager::updateMedal", APIManager::updateMedal);

void APIManager::updateSkill(APIManager *self, uint32_t skill) {
    const UserList &L = SKILLS;
    Frame F(0x78);
    bool found = copyReplacing(F, self, L, skill, [](uint32_t) {});
    if (!found) pushBack(F, L, skill);
    rebuild(F, self, L);
}
RW_FUNCTION(0x4C4498, "APIManager::updateSkill", APIManager::updateSkill);

// Materials: {id (64-bit), materialId, number}; a material whose number dropped to 0 is removed.
void APIManager::updateMaterial(APIManager *self, uint32_t material) {
    const UserList &L = MATERIALS;
    Frame F(0x68);
    clearVector(F, L.vecSlot);
    uint32_t sb = listOf(self, L).ptr;
    const uint32_t count = load32(sb + LIST_COUNT);
    bool found = false;
    for (uint32_t i = 0; i < count; i++) {
        if (i) sb = listOf(self, L).ptr;
        uint32_t elem = readElement(F, L, sb, i);
        if (load64(elem) == load64(material)) {
            copyGuest(elem, material, 16);
            found = true;
        }
        if (load32(elem + 12) != 0) pushBack(F, L, elem);
    }
    if (!found && load32(material + 12) != 0) pushBack(F, L, material);
    rebuild(F, self, L);
}
RW_FUNCTION(0x4C4670, "APIManager::updateMaterial", APIManager::updateMaterial);

// Drop the medals / skills whose ids are in the set (sold, used as material).
void APIManager::removeMedals(APIManager *self, uint32_t ids) {
    const UserList L{0xD4, 0x48, 0x30, 0x24, 0x1C, 0x4D4E59, 0x4A0551, 0x4C513D};
    Frame F(0x98);
    clearVector(F, L.vecSlot);
    uint32_t sb = listOf(self, L).ptr;
    const uint32_t count = load32(sb + LIST_COUNT);
    for (uint32_t i = 0; i < count; i++) {
        if (i) sb = listOf(self, L).ptr;
        uint32_t elem = readElement(F, L, sb, i);
        if (F.call(ID_SET_FIND, {ids, elem}) == 0) pushBack(F, L, elem);
    }
    rebuild(F, self, L);
}
RW_FUNCTION(0x4C47DC, "APIManager::removeMedals", APIManager::removeMedals);

void APIManager::removeSkills(APIManager *self, uint32_t ids) {
    const UserList &L = SKILLS;
    Frame F(0x78);
    clearVector(F, L.vecSlot);
    uint32_t sb = listOf(self, L).ptr;
    const uint32_t count = load32(sb + LIST_COUNT);
    for (uint32_t i = 0; i < count; i++) {
        if (i) sb = listOf(self, L).ptr;
        uint32_t elem = readElement(F, L, sb, i);
        if (F.call(ID_SET_FIND, {ids, elem}) == 0) pushBack(F, L, elem);
    }
    rebuild(F, self, L);
}
RW_FUNCTION(0x4C4904, "APIManager::removeSkills", APIManager::removeSkills);

void APIManager::updateTitle(APIManager *self, uint32_t title) {
    const UserList &L = TITLES;
    Frame F(0x68);
    bool found = copyReplacing(F, self, L, title, [](uint32_t) {});
    if (!found) pushBack(F, L, title);
    rebuild(F, self, L);
}
RW_FUNCTION(0x4C4A20, "APIManager::updateTitle", APIManager::updateTitle);

// Select the favorite keyblade: every keyblade's favorite flag (+0x4C) = (its id == id).
void APIManager::updateFavoriteKeyblade(APIManager *self, uint64_t id) {
    const UserList L{0xEC, 0x58, 0x38, 0x2C, 0x24, 0x4D494D, 0x4A10D9, 0x4C50FF};   // push_back(T&&)
    Frame F(0xB0);
    clearVector(F, L.vecSlot);
    uint32_t sb = listOf(self, L).ptr;
    const uint32_t count = load32(sb + LIST_COUNT);
    for (uint32_t i = 0; i < count; i++) {
        if (i) sb = listOf(self, L).ptr;
        uint32_t elem = readElement(F, L, sb, i);
        rw::store8(rw::host_ptr<uint8_t>(elem + 0x4C), load64(elem) == id ? 1 : 0);
        pushBackCopy(F, F.at(L.vecSlot), elem, L.elemSize, L.slowPath, true);
    }
    rebuild(F, self, L);
}
RW_FUNCTION(0x4C4BAC, "APIManager::updateFavoriteKeyblade", APIManager::updateFavoriteKeyblade);

// An avatar part changed or was obtained. A new part is marked new in AppState, and when it completes its set
// (every part of the set is owned) the set is marked new too.
void APIManager::updateAvatarPart(APIManager *self, uint32_t part) {
    const UserList &L = AVATAR_PARTS;
    Frame F(0xC8);
    bool found = copyReplacing(F, self, L, part, [](uint32_t) {});
    if (!found) {
        pushBack(F, L, part);
        uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
        F.call(APPSTATE_ADD_NEW_PARTS, {app, load32(part + 0xC)});
    }
    replaceList(F, self, L);
    if (found) {
        F.call(L.vectorDtor, {F.at(L.vecSlot)});
        return;
    }

    F.call(MASTER_AVATAR_PARTS_CREATE, {F.at(0x58), load32(part + 0xC)});
    int32_t setId = int32_t(sbWord(F.word(0x58), MASTER_PARTS_SET_ID));
    if (setId != 0) {
        uint32_t list = listOf(self, L).ptr;
        const uint32_t count = load32(list + LIST_COUNT);
        if (count != 0) {
            // the set is one of the owned parts' ids
            bool owned = false;
            for (uint32_t i = 0; i < count; i++) {
                F.call(SAFEBUFFER_READ, {list, F.at(0x40), load32(list + 12) + i * 0x18, 0x18, i * 0x18});
                if (int32_t(F.word(0x4C)) == setId) {
                    owned = true;
                    break;
                }
                list = listOf(self, L).ptr;
            }
            if (owned) {
                F.call(MASTER_AVATAR_PARTS_CREATE, {F.at(0x38), uint32_t(setId)});
                uint32_t set = F.word(0x38);
                int32_t parts = int32_t(sbWord(set, MASTER_PARTS_SET_COUNT));
                bool complete = true;
                for (int32_t j = 0; j < parts && complete; j++) {
                    F.call(SAFEBUFFER_READ, {F.word(0x38), F.at(0x98), load32(F.word(0x38) + 12) + MASTER_PARTS_SET_PARTS,
                                             0x14, MASTER_PARTS_SET_PARTS});
                    uint32_t wanted = F.word(0x98 + 4 * uint32_t(j));
                    complete = false;
                    for (uint32_t k = 0; k < count; k++) {
                        uint32_t sb = listOf(self, L).ptr;
                        F.call(SAFEBUFFER_READ, {sb, F.at(0x20), load32(sb + 12) + k * 0x18, 0x18, k * 0x18});
                        if (F.word(0x2C) == wanted) {
                            complete = true;
                            break;
                        }
                    }
                }
                if (complete) {
                    uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
                    F.call(APPSTATE_ADD_NEW_PARTS_SET, {app, uint32_t(setId)});
                }
                releaseIfSet(F, F.word(0x3C));
            }
        }
    }
    releaseIfSet(F, F.word(0x5C));
    F.call(L.vectorDtor, {F.at(L.vecSlot)});
}
RW_FUNCTION_NOVERIFY(0x4C4CD0, "APIManager::updateAvatarPart", APIManager::updateAvatarPart);

// ---------------------------------------------------------------------------------------------- values
// UserAvatar coordinate (48 bytes) stored at its slot (the unordered_map<int,int> at +0x24 maps id -> slot).
void APIManager::updateCoordinate(APIManager *self, uint32_t coordinate) {
    Frame F(0x28);
    uint32_t avatar = self->userAvatarAll.ptr;
    F.setWord(0xC, load32(coordinate));
    uint32_t node = F.call(INT_MAP_FIND_CONST, {avatar + 0x24, F.at(0xC)});
    if (node) {
        int32_t slot = int32_t(load32(node + 12));
        if (slot >= 0) {
            uint32_t index = uint32_t(slot) * 0x30;
            F.call(SAFEBUFFER_WRITE, {avatar, load32(avatar + 12) + index, coordinate, 0x30, index});
        }
    }
}
RW_FUNCTION(0x4C406C, "APIManager::updateCoordinate", APIManager::updateCoordinate);

// The user's money (UserPoint data position 0).
void APIManager::updateMoney(APIManager *self, int32_t money) {
    Frame F(0x20);
    F.setWord(0x8, uint32_t(money));
    uint32_t points = self->userPoint.ptr;
    F.call(SAFEBUFFER_WRITE, {points, load32(points + 12), F.at(0x8), 4, 0});
}
RW_FUNCTION(0x4C5080, "APIManager::updateMoney", APIManager::updateMoney);

// The index of the keyblade whose set medals (count at +0x10, ids at +0x18) include the medal, or -1.
int32_t APIManager::findMedalSetKeybladeIndex(APIManager *self, uint64_t medalId) {
    Frame F(0x98);
    uint32_t sb = self->userKeyblade.ptr;
    const uint32_t count = load32(sb + LIST_COUNT);
    for (uint32_t i = 0; i < count; i++) {
        if (i) sb = self->userKeyblade.ptr;
        F.call(SAFEBUFFER_READ, {sb, F.at(0x20), load32(sb + 12) + i * 0x58, 0x58, i * 0x58});
        uint32_t medals = F.word(0x30);
        for (uint32_t j = 0; j < medals; j++) {
            if (load64(F.at(0x38 + 8 * j)) == medalId) return int32_t(i);
        }
    }
    return -1;
}
RW_FUNCTION(0x4C51E8, "APIManager::findMedalSetKeybladeIndex", APIManager::findMedalSetKeybladeIndex);
