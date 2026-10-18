// APIManager::verifyResponse: party replies (create / update / login, members, applications, invites, offers).
#include "APIManager_verify.hpp"

namespace apiv {

namespace {

constexpr uint32_t S_PARTY = 0xDFA7D3;                  // "party"
constexpr uint32_t S_PARTY_ID = 0xDFAFD5;               // "partyId"
constexpr uint32_t S_PARTY_LEADER = 0xDFAFFA;           // "partyLeader"
constexpr uint32_t S_PARTY_LIST = 0xDFAF96;             // "partyList"
constexpr uint32_t S_PARTY_USER_LIST = 0xDFA7C5;        // "partyUserList"
constexpr uint32_t S_PARTY_OFFER = 0xDFBBDE;            // "partyOffer"
constexpr uint32_t S_GET_DETAIL = 0xDFBBD4;             // "getDetail"

// PartyApplication::parse's Type argument per action
constexpr uint32_t APPLICATION_APPLY = 0;
constexpr uint32_t APPLICATION_AGREE = 1;
constexpr uint32_t APPLICATION_REJECT = 2;
constexpr uint32_t APPLICATION_CANCEL = 3;

// UserParty: the Party it holds starts at +0x20
constexpr uint32_t USER_PARTY_PARTY = 0x20;
constexpr uint32_t PARTY_INFO_LEADER = 0x70;            // APIManager::PartyInfo: the leader (a SupportUser) after the Party
constexpr uint32_t SUPPORT_USER_SIZE = 0x124;
constexpr uint32_t PARTY_EX_SIZE = 0x198;
constexpr uint32_t PARTY_EX_EXTRA = 0x70;               // PartyEx: what follows its Party
constexpr uint32_t PARTY_EX_EXTRA_SIZE = 0x128;
constexpr uint32_t MEMBER_LIST_HAS_DETAIL = 0x38;       // PartyMemberList: u8, the members came with details

// Party (0x70 bytes) members with destructors: strings at +0x10, +0x24, +0x60 and a vector<ull> at +0x40
void constructPartyLocal(const Ctx &c, uint32_t slot) {
    for (uint32_t s : {0x10u, 0x24u, 0x40u}) {
        c.F.setWord(slot + s + 0, 0);
        c.F.setWord(slot + s + 4, 0);
        c.F.setWord(slot + s + 8, 0);
    }
    frameAnd8(c, slot + 0x53, 0xFD);
    frameAnd8(c, slot + 0x5B, 0xFD);
    c.F.setWord(slot + 0x60, 0);
    c.F.setWord(slot + 0x64, 0);
    c.F.setWord(slot + 0x68, 0);
}
void destroyPartyLocal(const Ctx &c, uint32_t slot) {
    destroyString(c, c.F.at(slot + 0x60));
    c.F.call(U64_VECTOR_DTOR, {c.F.at(slot + 0x40)});
    destroyString(c, c.F.at(slot + 0x24));
    destroyString(c, c.F.at(slot + 0x10));
}

// PartyInviteList::Offer local: its string at +0x10 and two bitfields
void constructOfferLocal(const Ctx &c, uint32_t slot) {
    c.F.setWord(slot + 0x10, 0);
    c.F.setWord(slot + 0x14, 0);
    c.F.setWord(slot + 0x18, 0);
    frameAnd8(c, slot + 0x27, 0xFD);
    frameAnd8(c, slot + 0x2F, 0xFD);
}

// the player's party (UserParty) alone: kept, "JoinParty" when just joined
uint32_t keepUserParty(const Ctx &c, uint32_t slot, uint32_t joinedByte) {
    const uint32_t party = parse(c, USER_PARTY_PARSE, slot);
    if (!party) {
        drop(c, slot);
        return 0;
    }
    sendJoinParty(c, party, joinedByte);
    keep(c, slot, off::USER_PARTY);
    drop(c, slot);
    return 1;
}

// the player's party and the application it came with (agree / apply)
uint32_t keepPartyAndApplication(const Ctx &c, uint32_t partySlot, uint32_t appSlot, uint32_t type, uint32_t joinedByte) {
    parse(c, USER_PARTY_PARSE, partySlot);
    parse(c, PARTY_APPLICATION_PARSE, appSlot, type);
    uint32_t ok = 0;
    const uint32_t party = c.F.word(partySlot);
    if (party && c.F.word(appSlot)) {
        ok = 1;
        sendJoinParty(c, party, joinedByte);
        keep(c, partySlot, off::USER_PARTY);
        keep(c, appSlot, off::PARTY_APPLICATION);
    }
    drop(c, appSlot);
    drop(c, partySlot);
    return ok;
}

// an offer (partyOffer) set into the cached offer / invite list
uint32_t setOffer(const Ctx &c, uint32_t listOffset, uint32_t offerSlot) {
    constructOfferLocal(c, offerSlot);
    uint32_t ok = 0;
    if (c.F.call(OFFER_INIT, {c.F.at(offerSlot), jsonMember(c, c.json, S_PARTY_OFFER)}) == 1) {
        ok = 1;
        c.F.call(PARTY_INVITE_LIST_SET_OFFER, {member(c.self, listOffset).ptr, c.F.at(offerSlot)});
    }
    destroyString(c, c.F.at(offerSlot + 0x10));
    return ok;
}

}  // namespace

// party/create: the party and the creation result
uint32_t verifyPartyCreate(const Ctx &c) {
    parse(c, USER_PARTY_PARSE, 0xBD0);
    parse(c, PARTY_CREATE_PARSE, 0xBC8);
    uint32_t ok = 0;
    const uint32_t party = c.F.word(0xBD0);
    if (party && c.F.word(0xBC8)) {
        ok = 1;
        sendJoinParty(c, party, 0xC5E);
        keep(c, 0xBD0, off::USER_PARTY);
        keep(c, 0xBC8, off::PARTY_CREATE);
    }
    drop(c, 0xBC8);
    drop(c, 0xBD0);
    return ok;
}

// party/update, party/leave
uint32_t verifyPartyUpdate(const Ctx &c) { return keepUserParty(c, 0xBE8, 0xC5C); }

uint32_t verifyPartyNotice(const Ctx &c) { return parseAndKeep(c, PARTY_NOTICE_PARSE, 0xBC0, off::PARTY_NOTICE); }

// party/login: the party; in a party (non-zero party id) the login reply too, else the cached one is cleared
uint32_t verifyPartyLogin(const Ctx &c) {
    if (!parse(c, USER_PARTY_PARSE, 0xBB8)) {
        drop(c, 0xBB8);
        return 0;
    }
    sendJoinParty(c, c.F.word(0xBB8), 0xC5F);
    keep(c, 0xBB8, off::USER_PARTY);
    const uint32_t party = member(c.self, off::USER_PARTY).ptr;
    c.F.call(SAFEBUFFER_READ, {party, c.F.at(0xC60), load32(party + 12) + PARTY_ID, 8, PARTY_ID});
    uint32_t ok;
    if ((c.F.word(0xC60) | c.F.word(0xC64)) == 0) {
        resetShared(c.F, member(c.self, off::PARTY_LOGIN));
        ok = 1;
    } else {
        ok = 0;
        if (parse(c, PARTY_LOGIN_PARSE, 0xBB0)) {
            keep(c, 0xBB0, off::PARTY_LOGIN);
            ok = 1;
        }
        drop(c, 0xBB0);
    }
    drop(c, 0xBB8);
    return ok;
}

// party: without a partyId in the request it is the player's party; with one, another party: Party + leader
// copied into a new APIManager::PartyInfo
uint32_t verifyParty(const Ctx &c) {
    if (jsonFlags(jsonMember(c, c.self->params.ptr, S_PARTY_ID)) == 0) {   // kNullType: the member is missing
        const uint32_t party = parse(c, USER_PARTY_PARSE, 0xBE0);
        if (!party) {
            drop(c, 0xBE0);
            return 0;
        }
        sendJoinParty(c, party, 0xC5D);
        keep(c, 0xBE0, off::USER_PARTY);
        drop(c, 0xBE0);
        return 1;
    }
    StdSharedPtr &info = member(c.self, off::OTHER_PARTY);
    resetShared(c.F, info);
    const uint32_t party = c.F.at(0xD08);                 // Party
    const uint32_t leader = c.F.at(0x1600);               // StageSupportList::SupportUser
    constructPartyLocal(c, 0xD08);
    uint32_t ok = 0;
    if (c.F.call(PARTY_INIT, {party, jsonMember(c, c.json, S_PARTY)}) != 0) {
        frameAnd8(c, 0x1600 + 0x11F, 0xFD);
        if (c.F.call(SUPPORT_USER_INIT, {leader, jsonMember(c, c.json, S_PARTY_LEADER)}) == 1) {
            c.F.call(PARTY_INFO_MAKE_SHARED, {c.F.at(0xBD8)});
            // info = std::move(made)
            const uint32_t made = c.F.word(0xBD8), madeCtrl = c.F.word(0xBDC);
            c.F.setWord(0xBD8, 0);
            c.F.setWord(0xBDC, 0);
            info.ptr = made;
            const uint32_t old = info.ctrl;
            info.ctrl = madeCtrl;
            releaseIfSet(c.F, old);
            // info->party = party (Party::operator=)
            const uint32_t p = info.ptr;
            copyGuest(p, party, 16);
            assignString(c, p + 0x10, party + 0x10);
            rw::save<uint32_t>(p + 0x1C, load32(party + 0x1C));
            rw::save<uint32_t>(p + 0x20, load32(party + 0x20));
            assignString(c, p + 0x24, party + 0x24);
            copyGuest(p + 0x30, party + 0x30, 16);
            if (p != party) c.F.call(U64_VECTOR_ASSIGN, {p + 0x40, load32(party + 0x40), load32(party + 0x44)});
            c.F.call(MEMCPY, {p + 0x4C, party + 0x4C, 0x14});
            assignString(c, p + 0x60, party + 0x60);
            // info->leader = leader
            c.F.call(MEMCPY, {info.ptr + PARTY_INFO_LEADER, leader, SUPPORT_USER_SIZE});
            ok = 1;
        }
    }
    destroyPartyLocal(c, 0xD08);
    return ok;
}

// party/member/list: the party; in a party the member list too (with details when the request asked for them),
// else the cached list is cleared
uint32_t verifyPartyMemberList(const Ctx &c) {
    const uint32_t party = parse(c, USER_PARTY_PARSE, 0xBA8);
    if (!party) {
        drop(c, 0xBA8);
        return 0;
    }
    c.F.call(SAFEBUFFER_READ, {party, c.F.at(0xC68), load32(party + 12) + PARTY_ID, 8, PARTY_ID});
    if ((c.F.word(0xC68) | c.F.word(0xC6C)) == 0) {
        resetShared(c.F, member(c.self, off::PARTY_MEMBER_LIST));
    } else {
        const uint32_t list = parse(c, PARTY_MEMBER_LIST_PARSE, 0xBA0, 0);
        bool ok = list != 0;
        if (ok && jsonFlags(jsonMember(c, c.self->params.ptr, S_GET_DETAIL)) != 0 &&
            load8(list + MEMBER_LIST_HAS_DETAIL) == 0)
            ok = false;
        if (!ok) {
            drop(c, 0xBA0);
            drop(c, 0xBA8);
            return 0;
        }
        keep(c, 0xBA0, off::PARTY_MEMBER_LIST);
        drop(c, 0xBA0);
    }
    if (c.F.word(0xBA8)) sendJoinParty(c, c.F.word(0xBA8), 0xC74);
    keep(c, 0xBA8, off::USER_PARTY);
    drop(c, 0xBA8);
    return 1;
}

// party/search/active: partyList (objects) into PartyEx records that replace the manager's search results
uint32_t verifyPartySearchActive(const Ctx &c) {
    const uint32_t results = c.F.at(0xB4C);               // std::vector<PartyEx>
    c.F.setWord(0xB4C, 0);
    c.F.setWord(0xB50, 0);
    c.F.setWord(0xB54, 0);
    uint32_t ok = 0;
    const uint32_t list = jsonMember(c, c.json, S_PARTY_LIST);
    if (jsonFlags(list) == JSON_ARRAY) {
        const uint32_t n = jsonArraySize(list);
        const uint32_t ex = c.F.at(0x1728);               // the PartyEx being read
        bool good = true;
        for (uint32_t k = 0; k < n; k++) {
            const uint32_t e = jsonArrayAt(list, k);
            if (jsonFlags(e) != JSON_OBJECT) {
                good = false;
                break;
            }
            constructPartyLocal(c, 0x1728);
            frameAnd8(c, 0x1728 + 0x18F, 0xFD);
            const bool read = c.F.call(PARTY_EX_INIT, {ex, e}) != 0;
            if (read) {
                // results.push_back(ex): Party's copy constructor, then the rest as bytes
                const uint32_t end = load32(results + 4);
                if (end == load32(results + 8)) {
                    c.F.call(PARTY_EX_VECTOR_PUSH_SLOW, {results, ex});
                } else {
                    uint32_t at = 0;
                    if (end) {
                        const uint32_t copy = c.F.call(PARTY_COPY, {end, ex});
                        c.F.call(MEMCPY, {copy + PARTY_EX_EXTRA, ex + PARTY_EX_EXTRA, PARTY_EX_EXTRA_SIZE});
                        at = load32(results + 4);
                    }
                    rw::save<uint32_t>(results + 4, at + PARTY_EX_SIZE);
                }
            }
            destroyPartyLocal(c, 0x1728);
            if (!read) {
                good = false;
                break;
            }
        }
        if (good) {
            StdVector &kept = c.self->partyApplications;     // off::PARTY_SEARCH_RESULTS
            c.F.call(PARTY_EX_VECTOR_DEALLOCATE, {addr(&kept)});
            kept.begin = c.F.word(0xB4C);
            kept.end = c.F.word(0xB50);
            kept.cap = c.F.word(0xB54);
            c.F.setWord(0xB4C, 0);
            c.F.setWord(0xB50, 0);
            c.F.setWord(0xB54, 0);
            ok = 1;
        }
    }
    c.F.call(PARTY_EX_VECTOR_DTOR, {results});
    return ok;
}

// party/application/160310
uint32_t verifyPartyApplication(const Ctx &c) {
    return keepPartyAndApplication(c, 0xB90, 0xB88, APPLICATION_APPLY, 0xC75);
}
uint32_t verifyPartyApplicationList(const Ctx &c) {
    return parseAndKeep(c, PARTY_APPLICATION_LIST_PARSE, 0xB80, off::PARTY_APPLICATION_LIST);
}

// the application alone (cancel / reject)
static uint32_t keepApplication(const Ctx &c, uint32_t slot, uint32_t type) {
    uint32_t ok = 0;
    if (parse(c, PARTY_APPLICATION_PARSE, slot, type)) {
        keep(c, slot, off::PARTY_APPLICATION);
        ok = 1;
    }
    drop(c, slot);
    return ok;
}

uint32_t verifyPartyApplicationCancel(const Ctx &c) { return keepApplication(c, 0xB78, APPLICATION_CANCEL); }
uint32_t verifyPartyApplicantList(const Ctx &c) {
    return parseAndKeep(c, PARTY_APPLICANT_LIST_PARSE, 0xB70, off::PARTY_APPLICANT_LIST);
}
uint32_t verifyPartyApplicantAgree(const Ctx &c) {
    return keepPartyAndApplication(c, 0xB68, 0xB60, APPLICATION_AGREE, 0xC76);
}
uint32_t verifyPartyApplicantReject(const Ctx &c) { return keepApplication(c, 0xB58, APPLICATION_REJECT); }
uint32_t verifyPartyLeaderChange(const Ctx &c) { return keepUserParty(c, 0x350, 0xCFF); }

// party/admin/appoint, party/admin/dismiss, party/member/expel: the parsed party's fields are copied into the
// cached one (Party::operator=, inlined; the cached party is assumed present)
uint32_t verifyPartyAdmin(const Ctx &c) {
    uint32_t ok = 0;
    const uint32_t parsed = parse(c, USER_PARTY_PARSE, 0xB98);
    if (parsed) {
        const uint32_t cached = member(c.self, off::USER_PARTY).ptr;
        const uint32_t to = cached + USER_PARTY_PARTY, from = parsed + USER_PARTY_PARTY;
        copyGuest(to, from, 16);
        c.F.call(STRING_ASSIGN, {to + 0x10, from + 0x10});
        rw::save<uint32_t>(to + 0x1C, load32(from + 0x1C));
        rw::save<uint32_t>(to + 0x20, load32(from + 0x20));
        c.F.call(STRING_ASSIGN, {to + 0x24, from + 0x24});
        copyGuest(to + 0x30, from + 0x30, 16);
        if (cached != parsed) c.F.call(U64_VECTOR_ASSIGN, {to + 0x40, load32(from + 0x40), load32(from + 0x44)});
        c.F.call(MEMCPY, {to + 0x4C, from + 0x4C, 0x14});
        ok = 1;
        c.F.call(STRING_ASSIGN, {to + 0x60, from + 0x60});
    }
    drop(c, 0xB98);
    return ok;
}

// party/invite/search: the users found (a StageSupportList)
uint32_t verifyPartyInviteSearch(const Ctx &c) {
    uint32_t ok = 0;
    if (parse(c, STAGE_SUPPORT_LIST_PARSE, 0xB44, S_PARTY_USER_LIST)) {
        keep(c, 0xB44, off::SUPPORT_LIST);
        ok = 1;
    }
    drop(c, 0xB44);
    return ok;
}

uint32_t verifyPartyInviteOffer(const Ctx &c) {
    return parseAndKeep(c, PARTY_INVITE_OFFER_PARSE, 0xB3C, off::PARTY_INVITE_OFFER);
}
uint32_t verifyPartyInviteList(const Ctx &c) { return parseAndKeep(c, PARTY_INVITE_LIST_PARSE, 0xB34, off::PARTY_INVITE_LIST); }

// party/invite/cancel: the cancelled offer, set into the invite list
uint32_t verifyPartyInviteCancel(const Ctx &c) { return setOffer(c, off::PARTY_INVITE_LIST, 0xAF8); }

uint32_t verifyPartyOfferList(const Ctx &c) { return parseAndKeep(c, PARTY_OFFER_LIST_PARSE, 0xAF0, off::PARTY_OFFER_LIST); }

// party/offer/agree: the party joined and the offer (set into the offer list, which also takes the party)
uint32_t verifyPartyOfferAgree(const Ctx &c) {
    parse(c, USER_PARTY_PARSE, 0xAE8);
    constructOfferLocal(c, 0xAB0);
    uint32_t ok = 0;
    if (c.F.word(0xAE8) &&
        c.F.call(OFFER_INIT, {c.F.at(0xAB0), jsonMember(c, c.json, S_PARTY_OFFER)}) == 1) {
        if (c.F.word(0xAE8)) sendJoinParty(c, c.F.word(0xAE8), 0xC77);
        keep(c, 0xAE8, off::USER_PARTY);
        StdSharedPtr &offers = member(c.self, off::PARTY_OFFER_LIST);
        c.F.call(PARTY_INVITE_LIST_SET_OFFER, {offers.ptr, c.F.at(0xAB0)});
        ok = 1;
        c.F.call(PARTY_OFFER_LIST_SET_PARTY, {offers.ptr, c.F.word(0xAE8) + USER_PARTY_PARTY});
    }
    destroyString(c, c.F.at(0xAB0 + 0x10));
    drop(c, 0xAE8);
    return ok;
}

// party/offer/reject: the rejected offer, set into the offer list
uint32_t verifyPartyOfferReject(const Ctx &c) { return setOffer(c, off::PARTY_OFFER_LIST, 0xA78); }

}  // namespace apiv
