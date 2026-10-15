#include "Ranking.hpp"

#include "Raid.hpp"
#include "StageCommon.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t USER_DATA = 0xDFA622;                  // the "userData" literal (User::parse's section)
constexpr uint32_t USER_PARSE = 0x49E449;                 // User::parse(json, withTag, section)
constexpr uint32_t USER_DETAIL_PARSE = 0x49EE1D;
constexpr uint32_t USER_AVATAR_PARSE = 0x49F82D;
constexpr uint32_t LUX_SHARED_PTR_CTOR = 0x4AC28D;
constexpr uint32_t LUX_PUSH_SLOW = 0x4B2FD1;
constexpr uint32_t LUX_VECTOR_BASE_DTOR = 0x4B2F77;
constexpr uint32_t PARTY_SHARED_PTR_CTOR = 0x4AC66D;
constexpr uint32_t PARTY_PUSH_SLOW = 0x4B2B39;
constexpr uint32_t PARTY_VECTOR_BASE_DTOR = 0x4B2ABD;
constexpr uint32_t COLOSSEUM_SHARED_PTR_CTOR = 0x4ACAF1;
constexpr uint32_t COLOSSEUM_PUSH_SLOW = 0x4B2869;
constexpr uint32_t COLOSSEUM_VECTOR_BASE_DTOR = 0x4B280D;
constexpr uint32_t UNION_INFO_PUSH_SLOW = 0x4B26C9;
constexpr uint32_t UNION_RANKER_PUSH_SLOW = 0x4B24D5;
constexpr uint32_t UNION_CONTROL_BLOCK_VTABLE = 0xF7E750;  // __shared_ptr_emplace<UnionRanking> vtable (+8)
constexpr uint32_t REWARD_RESULT_PUSH_SLOW = 0x4B2359;
constexpr uint32_t REWARD_CONTROL_BLOCK_VTABLE = 0xF7E728; // __shared_ptr_emplace<RankingReward> vtable (+8)

void releaseLocal(const SharedPtr &p) {
    if (p.ctrl) raid::releaseShared(p.ctrl);
}
// copy-construct a shared_ptr into fresh memory: pointer, control block, one more reference
void copySharedInto(SharedPtr *dst, const SharedPtr &src) {
    dst->ptr = src.ptr.get();
    uint32_t ctrl = src.ctrl;
    dst->ctrl = ctrl;
    if (ctrl) raid::addShared(ctrl);
}

// user / detail / avatar = X::parse(json) through the temporaries; a null one stops (both User kinds)
bool readRankedUser(RankedUser *self, const JsonValue *json, const GuestFrame &frame) {
    rw::call(USER_PARSE, {frame.addr(0x18), rw::guest_addr(json), 0, USER_DATA});
    raid::moveShared(&self->user, frame.at<SharedPtr>(0x18));
    if (!self->user.ptr) return false;
    rw::call(USER_DETAIL_PARSE, {frame.addr(0x10), rw::guest_addr(json)});
    raid::moveShared(&self->detail, frame.at<SharedPtr>(0x10));
    if (!self->detail.ptr) return false;
    rw::call(USER_AVATAR_PARSE, {frame.addr(0x8), rw::guest_addr(json)});
    raid::moveShared(&self->avatar, frame.at<SharedPtr>(0x8));
    return self->avatar.ptr != 0;
}
void destroyRankedUser(RankedUser *u) {
    releaseLocal(u->avatar);
    releaseLocal(u->detail);
    releaseLocal(u->user);
}
// the copy constructor's shared_ptr part
void copyRankedUser(RankedUser *dst, const RankedUser *src) {
    copySharedInto(&dst->user, src->user);
    copySharedInto(&dst->detail, src->detail);
    copySharedInto(&dst->avatar, src->avatar);
}
void assignRankedUser(RankedUser *dst, const RankedUser *src) {
    stage::assignShared(&dst->user, src->user.ptr, src->user.ctrl);
    stage::assignShared(&dst->detail, src->detail.ptr, src->detail.ctrl);
    stage::assignShared(&dst->avatar, src->avatar.ptr, src->avatar.ctrl);
}

// "rankingDatas" entries of a ranking: each through the local, appended with an inlined copy
template <class Entry>
bool readEntries(StdVector *vec, const JsonValue &array, Entry *local, uint32_t pushSlow,
                 void (*reset)(Entry *), uint32_t (*init)(Entry *, const JsonValue *), void (*destroy)(Entry *),
                 void (*copy)(uint32_t, Entry *)) {
    uint32_t n = array.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &entry = array.At(k);
        if (!entry.IsObject()) return false;
        reset(local);
        if (!init(local, &entry)) {
            destroy(local);
            return false;
        }
        uint32_t end = vec->end;
        if (end == vec->capEnd) {
            rw::call(pushSlow, {rw::guest_addr(vec), rw::guest_addr(local)});
        } else {
            if (end) copy(end, local);
            vec->end = vec->end + sizeof(Entry);
        }
        destroy(local);
    }
    return true;
}

// an int array appended to vec through the local at `localAddr`
bool readInts(StdVector *vec, const JsonValue &array, uint32_t localAddr) {
    if (!array.IsArray()) return false;
    uint32_t n = array.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &v = array.At(k);
        if (!v.IsInt()) return false;
        stage::pushInt(vec, localAddr, v.GetUint());
    }
    return true;
}

bool readDate(const JsonValue &obj, const char *key, const uint8_t *date, bool exactlyOne) {
    const JsonValue &v = obj[key];
    if (!v.IsString()) return false;
    uint32_t r = rw::call(guest::Date_init, {rw::guest_addr(date), v.GetString(), 0});
    return exactlyOne ? r == 1 : r != 0;
}
}  // namespace

// ---------------------------------------------------------------------------------------------- LuxRanking
uint32_t LuxRanking::User::init(User *self, const JsonValue *json) {
    GuestFrame frame(0x38);                   // original frame; the list temporaries at +0x18 / +0x10 / +8
    if (!json->IsObject()) return 0;
    if (!readRankedUser(self, json, frame)) return 0;
    const JsonValue &lux = (*json)["lux"];
    if (!lux.IsInt()) return 0;
    self->lux = lux.GetInt();
    const JsonValue &rank = (*json)["rank"];
    if (!rank.IsInt()) return 0;
    self->rank = rank.GetInt();
    return 1;
}
RW_FUNCTION(0x4ABE90, "hole::network::api::LuxRanking::User::init", LuxRanking::User::init);

LuxRanking::User *LuxRanking::User::assign(User *self, const User *other) {
    assignRankedUser(self, other);
    self->lux = other->lux.get();
    self->rank = other->rank.get();
    return self;
}
RW_FUNCTION(0x5525B8, "hole::network::api::LuxRanking::User::operator=", LuxRanking::User::assign);

uint32_t LuxRanking::parse(SharedPtr *out, const JsonValue *json, uint32_t key) {
    GuestFrame frame(0x70);                   // original frame; the User local at +0x38
    using U = LuxRanking::User;
    LuxRanking *self = newObject<LuxRanking>(sizeof(LuxRanking));
    rw::store8(self->rankingTermStart + 7, 0);
    for (uint32_t off = 30; off >= 19; off--) rw::store8(reinterpret_cast<uint8_t *>(self) + off, 0);
    rw::store8(reinterpret_cast<uint8_t *>(self) + 31, 0);     // termEnd flags + the vector, bytewise
    auto parseAll = [&]() -> bool {
        if (!json->IsObject()) return false;
        const JsonValue &ranking = json->member(key);
        if (!ranking.IsObject()) return false;
        const JsonValue &datas = ranking["rankingDatas"];
        if (!datas.IsArray()) return false;
        auto reset = [](U *u) { rw::call(guest::memset_, {rw::guest_addr(u), 0x18, 0}); };
        auto destroy = [](U *u) { destroyRankedUser(u); };
        auto copy = [](uint32_t end, U *u) {
            U *d = rw::host_ptr<U>(end);
            copyRankedUser(d, u);
            d->lux = u->lux.get();
            d->rank = u->rank.get();
        };
        if (!readEntries<U>(&self->users, datas, frame.at<U>(0x38), LUX_PUSH_SLOW, reset, &U::init, destroy, copy))
            return false;
        const JsonValue &status = ranking["rankingStatus"];
        if (!status.IsInt()) return false;
        self->rankingStatus = status.GetInt();
        if (!readDate(ranking, "rankingTermStart", self->rankingTermStart, true)) return false;
        if (!readDate(ranking, "rankingTermEnd", self->rankingTermEnd, true)) return false;
        const JsonValue &last = ranking["lastRank"];
        if (!last.IsInt()) return false;
        self->lastRank = last.GetInt();
        return true;
    };
    if (!parseAll()) {
        rw::call(LUX_VECTOR_BASE_DTOR, {rw::guest_addr(&self->users)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(LUX_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4ABFE0, "hole::network::api::LuxRanking::parse", LuxRanking::parse);

// ---------------------------------------------------------------------------------------------- PartyRanking
uint32_t PartyRanking::Info::init(Info *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &party = (*json)["party"];
    if (!party.IsObject() || Party::init(&self->party, &party) != 1) return 0;
    const JsonValue &rank = (*json)["rank"];
    if (!rank.IsInt()) return 0;
    self->rank = rank.GetInt();
    const JsonValue &lux = (*json)["lux"];
    if (!lux.IsInt()) return 0;
    self->lux = lux.GetInt();
    return 1;
}
RW_FUNCTION(0x4AC2E8, "hole::network::api::PartyRanking::Info::init", PartyRanking::Info::init);

uint32_t PartyRanking::parse(SharedPtr *out, const JsonValue *json, uint32_t key) {
    GuestFrame frame(0xE0);                   // original frame; the Info local at +0x48
    PartyRanking *self = newObject<PartyRanking>(sizeof(PartyRanking));
    self->parties.begin = 0;
    self->parties.end = 0;
    self->parties.capEnd = 0;
    rw::store8(self->updateTime + 7, 0);
    rw::store8(self->nextUpdateTime + 7, 0);
    auto parseAll = [&]() -> bool {
        if (!json->IsObject()) return false;
        const JsonValue &ranking = json->member(key);
        if (!ranking.IsObject()) return false;
        const JsonValue &datas = ranking["rankingDatas"];
        if (!datas.IsArray()) return false;
        auto reset = [](Info *i) { party::resetLocal(&i->party); };
        auto destroy = [](Info *i) { party::destroy(&i->party); };
        auto copy = [](uint32_t end, Info *i) {
            Party *d = Party::copyConstruct(rw::host_ptr<Party>(end), &i->party);
            rw::set<int32_t>(d, 0x70, i->rank.get());
            rw::set<int32_t>(d, 0x74, i->lux.get());
        };
        if (!readEntries<Info>(&self->parties, datas, frame.at<Info>(0x48), PARTY_PUSH_SLOW, reset, &Info::init,
                               destroy, copy))
            return false;
        const JsonValue &status = ranking["rankingStatus"];
        if (!status.IsInt()) return false;
        self->rankingStatus = status.GetInt();
        const JsonValue &last = ranking["lastRank"];
        if (!last.IsInt()) return false;
        self->lastRank = last.GetInt();
        if (!readDate(ranking, "updateTime", self->updateTime, true)) return false;
        return readDate(ranking, "nextUpdateTime", self->nextUpdateTime, false);
    };
    if (!parseAll()) {
        rw::call(PARTY_VECTOR_BASE_DTOR, {rw::guest_addr(&self->parties)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(PARTY_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AC37C, "hole::network::api::PartyRanking::parse", PartyRanking::parse);

// ---------------------------------------------------------------------------------------------- ColosseumRanking
uint32_t ColosseumRanking::User::init(User *self, const JsonValue *json) {
    GuestFrame frame(0x38);                   // original frame; the list temporaries at +0x18 / +0x10 / +8
    if (!json->IsObject()) return 0;
    if (!readRankedUser(self, json, frame)) return 0;
    const JsonValue &state = (*json)["userColosseum"];
    if (!state.IsObject() || Colosseum::UserState::init(&self->state, &state) != 1) return 0;
    if (!readDate(*json, "arrivalTime", self->arrivalTime, true)) return 0;
    const JsonValue &rank = (*json)["rank"];
    if (!rank.IsInt()) return 0;
    self->rank = rank.GetInt();
    return 1;
}
RW_FUNCTION(0x4AC6C4, "hole::network::api::ColosseumRanking::User::init", ColosseumRanking::User::init);

ColosseumRanking::User *ColosseumRanking::User::assign(User *self, const User *other) {
    assignRankedUser(self, other);
    for (uint32_t off = 24; off < 48; off += 4) rw::set<uint32_t>(self, off, rw::get<uint32_t>(other, off));
    return self;
}
RW_FUNCTION(0x55204C, "hole::network::api::ColosseumRanking::User::operator=", ColosseumRanking::User::assign);

uint32_t ColosseumRanking::parse(SharedPtr *out, const JsonValue *json, uint32_t key) {
    GuestFrame frame(0x80);                   // original frame; the User local at +0x38
    using U = ColosseumRanking::User;
    ColosseumRanking *self = newObject<ColosseumRanking>(sizeof(ColosseumRanking));
    rw::store8(self->rankingTermStart + 7, 0);
    for (uint32_t off = 30; off >= 19; off--) rw::store8(reinterpret_cast<uint8_t *>(self) + off, 0);
    rw::store8(reinterpret_cast<uint8_t *>(self) + 31, 0);     // termEnd flags + the vector, bytewise
    auto parseAll = [&]() -> bool {
        if (!json->IsObject()) return false;
        const JsonValue &ranking = json->member(key);
        if (!ranking.IsObject()) return false;
        const JsonValue &datas = ranking["rankingDatas"];
        if (!datas.IsArray()) return false;
        auto reset = [](U *u) {
            rw::call(guest::memset_, {rw::guest_addr(u), 0x18, 0});
            rw::store8(u->arrivalTime + 7, uint8_t(u->arrivalTime[7] & 0xFD));
        };
        auto destroy = [](U *u) { destroyRankedUser(u); };
        auto copy = [](uint32_t end, U *u) {
            copyRankedUser(rw::host_ptr<U>(end), u);
            for (uint32_t off = 24; off < 48; off += 4) rw::save<uint32_t>(end + off, rw::get<uint32_t>(u, off));
        };
        if (!readEntries<U>(&self->users, datas, frame.at<U>(0x38), COLOSSEUM_PUSH_SLOW, reset, &U::init, destroy,
                            copy))
            return false;
        const JsonValue &status = ranking["rankingStatus"];
        if (!status.IsInt()) return false;
        self->rankingStatus = status.GetInt();
        if (!readDate(ranking, "rankingTermStart", self->rankingTermStart, true)) return false;
        return readDate(ranking, "rankingTermEnd", self->rankingTermEnd, false);
    };
    if (!parseAll()) {
        rw::call(COLOSSEUM_VECTOR_BASE_DTOR, {rw::guest_addr(&self->users)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(COLOSSEUM_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AC854, "hole::network::api::ColosseumRanking::parse", ColosseumRanking::parse);

// ---------------------------------------------------------------------------------------------- UnionRanking
uint32_t UnionRanking::Info::init(Info *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &id = (*json)["unionId"];
    if (!id.IsInt()) return 0;
    self->unionId = id.GetInt();
    const JsonValue &rank = (*json)["rank"];
    if (!rank.IsInt()) return 0;
    self->rank = rank.GetInt();
    const JsonValue &lux = (*json)["lux"];
    if (!lux.IsUint64()) return 0;
    self->lux = lux.GetUint64();
    return 1;
}
RW_FUNCTION(0x4ACB4C, "hole::network::api::UnionRanking::Info::init", UnionRanking::Info::init);

uint32_t UnionRanking::Ranker::init(Ranker *self, const JsonValue *json) {
    GuestFrame frame(0x30);                   // original frame; the temporaries at +0x10 (user) / +8 (avatar)
    if (!json->IsObject()) return 0;
    rw::call(USER_PARSE, {frame.addr(0x10), rw::guest_addr(json), 0, USER_DATA});
    raid::moveShared(&self->user, frame.at<SharedPtr>(0x10));
    if (!self->user.ptr) return 0;
    const JsonValue &keyblade = (*json)["userKeyblade"];
    if (!keyblade.IsObject() || UserKeyblade::Keyblade::init(&self->keyblade, &keyblade) != 1) return 0;
    rw::call(USER_AVATAR_PARSE, {frame.addr(0x8), rw::guest_addr(json)});
    raid::moveShared(&self->avatar, frame.at<SharedPtr>(0x8));
    if (!self->avatar.ptr) return 0;
    const JsonValue &rank = (*json)["rank"];
    if (!rank.IsInt()) return 0;
    self->rank = rank.GetInt();
    const JsonValue &lux = (*json)["lux"];
    if (!lux.IsInt()) return 0;
    self->lux = lux.GetInt();
    return 1;
}
RW_FUNCTION(0x4ACBD8, "hole::network::api::UnionRanking::Ranker::init", UnionRanking::Ranker::init);

uint32_t UnionRanking::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0xc8: the Info local at +0x28, the Ranker local at +0x38
    GuestFrame frame(0xC8);
    // make_shared<UnionRanking>(): control block {vtable, shared count, weak count} + the object at +12
    uint32_t block = guestNew(0x24);
    UnionRanking *self = rw::host_ptr<UnionRanking>(block + 12);
    if (block) {
        rw::save<uint32_t>(block + 4, 0);
        rw::save<uint32_t>(block + 8, 0);
        rw::save<uint32_t>(block, UNION_CONTROL_BLOCK_VTABLE);
        rw::call(guest::memset_, {block + 12, 0x18, 0});
    }
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &parade = (*response)["unionParade"];
        if (!parade.IsObject()) return false;
        const JsonValue &unions = parade["rankingUnionResult"];
        if (!unions.IsArray()) return false;
        uint32_t n = unions.Size();
        Info *info = frame.at<Info>(0x28);
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = unions.At(k);
            if (!entry.IsObject() || Info::init(info, &entry) != 1) return false;
            uint32_t end = self->unions.end;
            if (end == self->unions.capEnd) {
                rw::call(UNION_INFO_PUSH_SLOW, {rw::guest_addr(&self->unions), rw::guest_addr(info)});
            } else {
                uint32_t base = 0;
                if (end) {
                    for (uint32_t w = 0; w < 16; w += 4) rw::save<uint32_t>(end + w, rw::get<uint32_t>(info, w));
                    base = self->unions.end;
                }
                self->unions.end = base + sizeof(Info);
            }
        }
        const JsonValue &rankers = parade["rankingUnionTopRanker"];
        if (!rankers.IsArray()) return false;
        uint32_t m = rankers.Size();
        Ranker *ranker = frame.at<Ranker>(0x38);
        for (uint32_t k = 0; k < m; k++) {
            const JsonValue &entry = rankers.At(k);
            if (!entry.IsObject()) return false;
            ranker->user.reset();
            ranker->avatar.reset();
            if (!Ranker::init(ranker, &entry)) {
                releaseLocal(ranker->avatar);
                releaseLocal(ranker->user);
                return false;
            }
            uint32_t end = self->rankers.end;
            if (end == self->rankers.capEnd) {
                rw::call(UNION_RANKER_PUSH_SLOW, {rw::guest_addr(&self->rankers), rw::guest_addr(ranker)});
            } else {
                if (end) {
                    Ranker *d = rw::host_ptr<Ranker>(end);
                    copySharedInto(&d->user, ranker->user);
                    copySharedInto(&d->avatar, ranker->avatar);
                    rw::call(guest::memcpy_, {end + 16, rw::guest_addr(ranker) + 16, 0x60});
                }
                self->rankers.end = self->rankers.end + sizeof(Ranker);
            }
            releaseLocal(ranker->avatar);
            releaseLocal(ranker->user);
        }
        return true;
    };
    if (parseAll()) {
        out->ptr = rw::guest_addr(self);
        out->ctrl = block;
    } else {
        out->reset();
        if (block) raid::releaseShared(block);
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4ACD30, "hole::network::api::UnionRanking::parse", UnionRanking::parse);

// ---------------------------------------------------------------------------------------------- RankingReward
uint32_t RankingReward::Result::init(Result *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    static const char *const kKeys[] = {"lastRankingId", "lastRank", "lastUnionRank"};
    for (uint32_t k = 0; k < 3; k++) {
        const JsonValue &v = (*json)[kKeys[k]];
        if (!v.IsInt()) return 0;
        rw::set<int32_t>(self, 4 * k, v.GetInt());
    }
    return 1;
}
RW_FUNCTION(0x4ACFC4, "hole::network::api::RankingReward::Result::init", RankingReward::Result::init);

uint32_t RankingReward::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x48: the Result local at +0x1c, the int locals at +0x28 / +0x2c
    GuestFrame frame(0x48);
    uint32_t block = guestNew(0x30);
    RankingReward *self = rw::host_ptr<RankingReward>(block + 12);
    if (block) {
        rw::save<uint32_t>(block + 4, 0);
        rw::save<uint32_t>(block + 8, 0);
        rw::save<uint32_t>(block, REWARD_CONTROL_BLOCK_VTABLE);
        rw::call(guest::memset_, {block + 12, 0x24, 0});
    }
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &results = (*response)["lastRankingResults"];
        if (!results.IsArray()) return false;
        uint32_t n = results.Size();
        Result *local = frame.at<Result>(0x1C);
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = results.At(k);
            if (!entry.IsObject() || Result::init(local, &entry) != 1) return false;
            uint32_t end = self->results.end;
            if (end == self->results.capEnd) {
                rw::call(REWARD_RESULT_PUSH_SLOW, {rw::guest_addr(&self->results), rw::guest_addr(local)});
            } else {
                uint32_t base = 0;
                if (end) {
                    for (uint32_t w = 0; w < 12; w += 4) rw::save<uint32_t>(end + w, rw::get<uint32_t>(local, w));
                    base = self->results.end;
                }
                self->results.end = base + sizeof(Result);
            }
        }
        if (!readInts(&self->rankingRewardIds, (*response)["rankingRewardIds"], frame.addr(0x28))) return false;
        return readInts(&self->openRankingIds, (*response)["openRankingIds"], frame.addr(0x2C));
    };
    if (parseAll()) {
        out->ptr = rw::guest_addr(self);
        out->ctrl = block;
    } else {
        out->reset();
        if (block) raid::releaseShared(block);
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AD04C, "hole::network::api::RankingReward::parse", RankingReward::parse);

}  // namespace hole::network::api
