#include "UserAlbum.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t ALBUM_SHARED_PTR_CTOR = 0x4A1379;       // std::shared_ptr<UserAlbum>::shared_ptr(UserAlbum*)
constexpr uint32_t CHALLENGE_SHARED_PTR_CTOR = 0x4A1535;   // std::shared_ptr<UserAlbumChallenge>::shared_ptr(...)
// std::__tree<Challenge, bool (*)(const Challenge&, const Challenge&)> members
constexpr uint32_t TREE_MOVE_ASSIGN = 0x4B07B1;    // __move_assign(__tree&, true_type)
constexpr uint32_t TREE_DESTROY = 0x4B07E7;        // destroy(__tree_node*)
constexpr uint32_t TREE_INSERT_UNIQUE = 0x4B0631;  // __insert_unique(const Challenge&) -> pair<iterator, bool>
constexpr uint32_t TREE_ERASE_UNIQUE = 0x4B04FF;   // __erase_unique<Challenge>(const Challenge&)
constexpr uint32_t CHALLENGE_LESS = 0x4B6CFB;      // a.challengeId < b.challengeId (the set's comparator)
constexpr uint32_t NODE_VALUE = 16;                // offset of the Challenge in a tree node
}  // namespace

// ---------------------------------------------------------------------------------------------- UserAlbum
uint32_t UserAlbum::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x38);   // original frame: 5 pushed registers + 0x24 of locals
    UserAlbum *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &album = (*response)["userAlbum"];
        if (album.IsObject()) {
            const JsonValue &ids = album["medalIds"];
            if (ids.IsArray()) {
                uint32_t n = ids.Size();
                self = newObject<UserAlbum>(sizeof(UserAlbum));
                constructSafeBuffer(self, n * RECORD);
                self->vtable = VTABLE;
                self->count = n;
                uint32_t k = 0;
                for (; k < n; k++) {
                    const JsonValue &id = ids.At(k);
                    if (!id.IsInt()) break;
                    writeU32(self, k * RECORD, id.GetUint());
                }
                bool ok = false;
                if (k == n) {
                    const JsonValue &all = album["allMedalCount"];
                    if (all.IsInt()) {
                        self->allMedalCount = all.GetInt();
                        ok = true;
                    }
                }
                if (!ok) {
                    deleteVirtual(self);
                    self = nullptr;
                }
            }
        }
    }
    rw::call(ALBUM_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A121C, "hole::network::api::UserAlbum::parse", UserAlbum::parse);

uint32_t UserAlbum::destroyDelete(UserAlbum *self) {
    uint32_t p = rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return rw::call(guest::operator_delete, {p});
}
RW_FUNCTION(0x4B6CEE, "hole::network::api::UserAlbum::~UserAlbum (deleting)", UserAlbum::destroyDelete);

// ---------------------------------------------------------------------------------------------- UserAlbumChallenge
UserAlbumChallenge *UserAlbumChallenge::construct(UserAlbumChallenge *self) {
    // original frame 0x30: a temporary set (with the comparator) at +8, moved into the member
    GuestFrame frame(0x30);
    StdPtrCompareSet *tmp = frame.at<StdPtrCompareSet>(8);
    self->challenges.beginNode = rw::guest_addr(&self->challenges.root);   // empty, no comparator yet
    self->challenges.root = 0;
    self->challenges.size = 0;
    self->challenges.compare = 0;
    tmp->root = 0;
    tmp->size = 0;
    tmp->compare = CHALLENGE_LESS;
    tmp->beginNode = frame.addr(12);
    rw::call(TREE_MOVE_ASSIGN, {rw::guest_addr(self), frame.addr(8)});
    rw::call(TREE_DESTROY, {frame.addr(8), tmp->root});
    return self;
}
RW_FUNCTION(0x4A11A0, "hole::network::api::UserAlbumChallenge::UserAlbumChallenge", UserAlbumChallenge::construct);

uint32_t UserAlbumChallenge::Challenge::init(Challenge *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &id = (*json)["challengeId"];
    if (!id.IsUint()) return 0;
    self->challengeId = id.GetUint();
    const JsonValue &count = (*json)["count"];
    if (!count.IsUint()) return 0;
    self->count = count.GetUint();
    const JsonValue &status = (*json)["status"];
    if (!status.IsUint()) return 0;
    self->status = uint8_t(status.GetUint());
    return 1;
}
RW_FUNCTION(0x4A13CC, "hole::network::api::UserAlbumChallenge::Challenge::init", UserAlbumChallenge::Challenge::init);

uint32_t UserAlbumChallenge::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x40: the Challenge local at +0x14, the insert result at +0x20
    GuestFrame frame(0x40);
    Challenge *record = frame.at<Challenge>(0x14);

    UserAlbumChallenge *self = newObject<UserAlbumChallenge>(sizeof(UserAlbumChallenge));
    construct(self);
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &list = (*response)["userAlbumChallenge"];
        if (list.IsArray()) {
            uint32_t n = list.Size();
            uint32_t k = 0;
            for (; k < n; k++) {
                const JsonValue &item = list.At(k);
                if (!item.IsObject()) break;
                if (Challenge::init(record, &item) != 1) break;
                rw::call(TREE_INSERT_UNIQUE, {frame.addr(0x20), rw::guest_addr(self), frame.addr(0x14)});
            }
            ok = (k == n);
        }
    }
    if (!ok) {
        rw::call(TREE_DESTROY, {rw::guest_addr(self), self->challenges.root});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(CHALLENGE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A1454, "hole::network::api::UserAlbumChallenge::parse", UserAlbumChallenge::parse);

uint32_t UserAlbumChallenge::updateChallenges(UserAlbumChallenge *self, const StdPtrCompareSet *updated) {
    // original frame 0x28: the insert result at +8
    GuestFrame frame(0x28);
    uint32_t end = rw::guest_addr(&updated->root);   // the end node
    uint32_t node = updated->beginNode;
    while (node != end) {
        rw::call(TREE_ERASE_UNIQUE, {rw::guest_addr(self), node + NODE_VALUE});
        rw::call(TREE_INSERT_UNIQUE, {frame.addr(8), rw::guest_addr(self), node + NODE_VALUE});
        // ++iterator: leftmost of the right subtree, else up until we come from a left child
        uint32_t right = rw::load<uint32_t>(node + 4);
        if (right) {
            node = right;
            while (uint32_t left = rw::load<uint32_t>(node)) node = left;
        } else {
            uint32_t child = node;
            for (;;) {
                node = rw::load<uint32_t>(child + 8);
                if (rw::load<uint32_t>(node) == child) break;
                child = node;
            }
        }
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A1590, "hole::network::api::UserAlbumChallenge::updateChallenges", UserAlbumChallenge::updateChallenges);

}  // namespace hole::network::api
