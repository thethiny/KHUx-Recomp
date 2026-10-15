#include "PartyMemberList.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t PARTY_USER_LIST = 0xDFA7C5;        // the "partyUserList" literal (the default key)
constexpr uint32_t SHARED_PTR_CTOR = 0x4A8369;        // std::shared_ptr<PartyMemberList>::shared_ptr(...)
constexpr uint32_t DETAIL_PUSH_SLOW = 0x4B4719;       // vector<Detail>::__push_back_slow_path(const Detail&)
constexpr uint32_t DETAIL_VECTOR_BASE_DTOR = 0x4B4A35;   // __vector_base<Detail>::~__vector_base
constexpr uint32_t SUPPORT_RECORD = sizeof(StageSupportList::SupportUser);
}  // namespace

uint32_t PartyMemberList::Detail::init(Detail *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    if (UserKeyblade::Keyblade::init(&self->keyblade, &(*json)["userKeyblade"]) != 1) return 0;
    if (Ranking::init(&self->ranking, &(*json)["userRanking"]) != 1) return 0;
    const JsonValue &stage = (*json)["lastStageId"];
    if (!stage.IsInt()) return 0;
    self->lastStageId = stage.GetInt();
    const JsonValue &comment = (*json)["comment"];
    if (!comment.IsString()) return 0;
    self->comment.assign(comment.GetString());
    const JsonValue &link = (*json)["linkPlatformId"];
    if (!link.IsString()) return 0;
    self->linkPlatformId.assign(link.GetString());
    return 1;
}
RW_FUNCTION(0x4A7EC8, "hole::network::api::PartyMemberList::Detail::init", PartyMemberList::Detail::init);

uint32_t PartyMemberList::parse(SharedPtr *out, const JsonValue *json, uint32_t key) {
    // original frame 0x240: the map pair at +0x48, the insert result at +0x58, the SupportUser local at +0x60,
    // the Detail local at +0x188
    const records::ParseFrame at = {0x240, 0x60, 0x48, 0x58};
    GuestFrame frame(at.size);
    SupportUser *member = frame.at<SupportUser>(at.record);
    Detail *detail = frame.at<Detail>(0x188);

    PartyMemberList *self = nullptr;
    if (json->IsObject()) {
        uint32_t name = key ? key : PARTY_USER_LIST;
        const JsonValue &list = json->member(name);
        if (list.IsArray()) {
            uint32_t n = list.Size();
            self = newObject<PartyMemberList>(sizeof(PartyMemberList));
            constructSafeBuffer(self, SUPPORT_RECORD * n);
            self->count = n;
            for (uint32_t off = 36; off < 52; off += 4) rw::set<uint32_t>(self, off, 0);   // empty map
            rw::set<uint32_t>(self, 52, records::ONE_F);
            self->vtable = VTABLE;
            self->details.begin = 0;
            self->details.end = 0;
            self->details.capEnd = 0;

            // pass 1: the SupportUser records
            uint32_t k = 0;
            for (; k < n; k++) {
                const uint8_t *flags = member->lastActionDatetime + 7;
                rw::store8(flags, uint8_t(*flags & 0xFD));
                if (!SupportUser::init(member, &list.At(k))) break;
                records::addRecord(self, frame, at, SUPPORT_RECORD, k, records::MAP_INSERT_INT);
            }
            if (k != n) {
                deleteVirtual(self);
                self = nullptr;
            } else {
                // pass 2: the details; a failure keeps the list, with `complete` left 0
                self->complete = 0;
                const JsonValue &again = json->member(name);
                if (again.IsArray()) {
                    uint32_t m = again.Size();
                    StdVector *vec = &self->details;
                    uint32_t j = 0;
                    for (; j < m; j++) {
                        const JsonValue &entry = again.At(j);
                        if (!entry.IsObject()) break;
                        rw::call(guest::memset_, {rw::guest_addr(&detail->comment), 0x18, 0});
                        if (!Detail::init(detail, &entry)) {
                            party::freeString(&detail->linkPlatformId);
                            party::freeString(&detail->comment);
                            break;
                        }
                        uint32_t end = vec->end;
                        if (end == vec->capEnd) {
                            rw::call(DETAIL_PUSH_SLOW, {rw::guest_addr(vec), rw::guest_addr(detail)});
                        } else {
                            if (end) {
                                Detail *dst = rw::host_ptr<Detail>(end);
                                rw::call(guest::memcpy_, {end, rw::guest_addr(detail), 0x7C});
                                party::copyString(&dst->comment, &detail->comment);
                                party::copyString(&dst->linkPlatformId, &detail->linkPlatformId);
                            }
                            vec->end = vec->end + sizeof(Detail);
                        }
                        party::freeString(&detail->linkPlatformId);
                        party::freeString(&detail->comment);
                    }
                    if (j == m) self->complete = 1;
                }
            }
        }
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A8040, "hole::network::api::PartyMemberList::parse", PartyMemberList::parse);

PartyMemberList *PartyMemberList::destroy(PartyMemberList *self) {
    self->vtable = VTABLE;
    rw::call(DETAIL_VECTOR_BASE_DTOR, {rw::guest_addr(&self->details)});
    StageSupportList::destroy(self);          // ~StageSupportList (inlined in the original)
    return self;
}
RW_FUNCTION(0x4B4998, "hole::network::api::PartyMemberList::~PartyMemberList", PartyMemberList::destroy);

uint32_t PartyMemberList::destroyDelete(PartyMemberList *self) {
    destroy(self);
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B49E4, "hole::network::api::PartyMemberList::~PartyMemberList (deleting)",
            PartyMemberList::destroyDelete);

}  // namespace hole::network::api
