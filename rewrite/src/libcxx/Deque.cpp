// std::deque instances over the game's element types (libcxx::deque, Deque.hpp): the element policies (size,
// elements per block, how one is built and destroyed, the instance's out-of-line map members), push_front /
// __add_front_capacity, and deque<shared_ptr<RESTClient::Connection>>::erase with its segmented move algorithms.
#include "libcxx/Deque.hpp"

using namespace libcxx::deque;
using libcxx::copyShared;
using libcxx::copyWords;
using libcxx::destroyFunction;
using libcxx::destroyShared;
using libcxx::destroyString;
using libcxx::moveShared;
using libcxx::moveTriple;
using rw::load;
using rw::save;

namespace {

namespace split = libcxx::split;

// ------------------------------------------------------------------------------------------ element policies
void noDestroy(uint32_t) {}

// shared_ptr<RESTClient::Connection>
struct Connections {
    static constexpr uint32_t S = 8, BS = 512;
    static constexpr uint32_t MapPushBack = 0x49AE95, MapPushFront = 0x49AFC1, BufPushBack = 0x49B0E1, BufPushFront = 0x49B209,
                              BufDtor = 0x49B325, MapDtor = 0x49BBB9, Clear = 0x49D5B9, AddBack = 0x49AB7D;
    static void destroy(uint32_t e) { destroyShared(e); }
};
// pair<shared_ptr<RESTClient::Connection>, shared_ptr<RESTClient::Result>>
struct ConnectionResults {
    static constexpr uint32_t S = 16, BS = 256;
    static constexpr uint32_t MapPushBack = 0x499D69, MapPushFront = 0x499E95, BufPushBack = 0x499FB5, BufPushFront = 0x49A0DD,
                              BufDtor = 0x49A1F9, MapDtor = 0x49BB8D, Clear = 0x49D66D, AddBack = 0x499A51;
    static void destroy(uint32_t e) {
        destroyShared(e + 8);
        destroyShared(e);
    }
};
// api::Client::Request (0x38 bytes): a shared_ptr, a word, a shared_ptr, a std::function (0x18), two flags
struct Requests {
    static constexpr uint32_t S = 0x38, BS = 73;
    static constexpr uint32_t MapPushBack = 0x4B9C25, MapPushFront = 0x4B9D51, BufPushBack = 0x4B9E71, BufPushFront = 0x4B9F99,
                              BufDtor = 0x4BA0B5, MapDtor = 0x4BA0E1, Clear = 0x4B9251, AddBack = 0x4B9911;
    static void destroy(uint32_t e) {
        destroyFunction(e + 0x18);
        destroyShared(e + 0xC);
        destroyShared(e);
    }
};
// FlashNode::ActFlag / int / const void* (4 bytes)
struct ActFlags {
    static constexpr uint32_t S = 4, BS = 1024;
    static constexpr uint32_t MapPushBack = 0x5C4445, MapPushFront = 0x5C4571, BufPushBack = 0x5C4691, BufPushFront = 0x5C47B9,
                              BufDtor = 0x5C48D5, MapDtor = 0x5C560D, Clear = 0x5C57AD, AddBack = 0x5C412D, AddFront = 0x741D0D,
                              MapPushFrontCopy = 0x741F89;
    static void destroy(uint32_t) {}
};
struct Ints {
    static constexpr uint32_t S = 4, BS = 1024;
    static constexpr uint32_t MapDtor = 0x5C5639, Clear = 0x5C5701;
    static void destroy(uint32_t) {}
};
struct Pointers {   // const void*
    static constexpr uint32_t S = 4, BS = 1024;
    static constexpr uint32_t MapDtor = 0x776119, Clear = 0x7756BD;
    static void destroy(uint32_t) {}
};
struct Strings {
    static constexpr uint32_t S = 12, BS = 341;
    static constexpr uint32_t MapPushBack = 0x604469, MapPushFront = 0x604595, BufPushBack = 0x6046B5, BufPushFront = 0x6047DD,
                              BufDtor = 0x6048F9, MapDtor = 0x60411B, Clear = 0x6072F5, AddBack = 0x604149;
    static void destroy(uint32_t e) { destroyString(e); }
};
// MasterDataManager::TableInfo (0x28 bytes): 2 words, a string, a shared_ptr, a string
struct TableInfos {
    static constexpr uint32_t S = 0x28, BS = 102;
    static constexpr uint32_t MapPushBack = 0x76E0B5, MapPushFront = 0x76E1E1, BufPushBack = 0x76E301, BufPushFront = 0x76E429,
                              BufDtor = 0x76E545, MapDtor = 0x76F1D5, Clear = 0x76E571, AddBack = 0x76DDA1;
    static void destroy(uint32_t e) {
        destroyString(e + 0x1C);
        destroyShared(e + 0x14);
        destroyString(e + 8);
    }
};
// pair<audio::ResourceID, int> (the game-main BGM stack, a static at 0x1003C64)
struct BgmEntries {
    static constexpr uint32_t S = 8, BS = 512;
    static constexpr uint32_t MapDtor = 0x5C221B, Clear = 0x5C1F05;
    static void destroy(uint32_t) {}
};
// shared_ptr<ResourceInfo>
struct Resources {
    static constexpr uint32_t S = 8, BS = 512;
    static constexpr uint32_t MapPushBack = 0x775B45, MapPushFront = 0x775C71, BufPushBack = 0x775D91, BufPushFront = 0x775EB9,
                              BufDtor = 0x775FD5, MapDtor = 0x776145, Clear = 0x776001, AddBack = 0x77582D;
    static void destroy(uint32_t e) { destroyShared(e); }
};

// element constructors for push_back
void sharedCopy(uint32_t slot, uint32_t x) { copyShared(slot, x); }
void pairMove(uint32_t slot, uint32_t x) {
    moveShared(slot, x);
    moveShared(slot + 8, x + 8);
}
void requestCopy(uint32_t slot, uint32_t x) {
    copyShared(slot, x);
    save<uint32_t>(slot + 8, load<uint32_t>(x + 8));
    copyShared(slot + 0xC, x + 0xC);
    save<uint32_t>(slot + 8, load<uint32_t>(x + 8));
    rw::call(0x4B7329, {slot + 0x18, x + 0x18});   // std::function<StatusCode(...)>::function(const function&)
    save<uint8_t>(slot + 0x31, load<uint8_t>(x + 0x31));
    save<uint8_t>(slot + 0x30, load<uint8_t>(x + 0x30));
}
void wordCopy(uint32_t slot, uint32_t x) { copyWords(slot, x, 4); }
void stringMove(uint32_t slot, uint32_t x) { moveTriple(slot, x); }
void tableInfoMove(uint32_t slot, uint32_t x) {
    copyWords(slot, x, 8);
    moveTriple(slot + 8, x + 8);
    moveShared(slot + 0x14, x + 0x14);
    moveTriple(slot + 0x1C, x + 0x1C);
}

// ------------------------------------------------------------------------------------------ push_front
// __add_front_capacity(): a spare back block rotated to the front, else a new block added to the map, else the map
// regrown (twice the capacity) with the new block at its front
template <class P> void addFrontCapacity(uint32_t d) {
    if (backSpare<P>(d) >= P::BS) {
        save<uint32_t>(d + START, load<uint32_t>(d + START) + P::BS);
        guest::Frame tmp(4);
        save<uint32_t>(tmp.addr(), load<uint32_t>(load<uint32_t>(d + MAP_END) - 4));
        save<uint32_t>(d + MAP_END, load<uint32_t>(d + MAP_END) - 4);
        rw::call(P::MapPushFrontCopy, {d, tmp.addr()});
        return;
    }
    const uint32_t first = load<uint32_t>(d + MAP_FIRST), cap = load<uint32_t>(d + MAP_CAP);
    if (mapSize<P>(d) < (cap - first) / 4) {
        guest::Frame tmp(8);
        save<uint32_t>(tmp.addr(), guest::newObject(blockBytes<P>()));
        if (load<uint32_t>(d + MAP_BEGIN) != first) {
            rw::call(P::MapPushFront, {d, tmp.addr()});
        } else {
            rw::call(P::MapPushBack, {d, tmp.addr()});
            save<uint32_t>(tmp.addr(4), load<uint32_t>(load<uint32_t>(d + MAP_END) - 4));
            save<uint32_t>(d + MAP_END, load<uint32_t>(d + MAP_END) - 4);
            rw::call(P::MapPushFrontCopy, {d, tmp.addr(4)});
        }
        save<uint32_t>(d + START, mapSize<P>(d) == 1 ? P::BS / 2 : load<uint32_t>(d + START) + P::BS);
        return;
    }
    guest::Frame frame(24);   // the new block, then the new map (a __split_buffer<T*, allocator<T*>&>)
    const uint32_t hold = frame.addr(0), buf = frame.addr(4);
    save<uint32_t>(buf + split::ALLOC_REF, d + MAP_CAP);
    uint32_t c = (cap - first) / 4 * 2;
    if (!c) c = 1;
    const uint32_t bufFirst = guest::newObject(c * 4);
    save<uint32_t>(buf + split::FIRST, bufFirst);
    save<uint32_t>(buf + split::BEGIN, bufFirst);
    save<uint32_t>(buf + split::END, bufFirst);
    save<uint32_t>(buf + split::CAP, bufFirst + c * 4);
    save<uint32_t>(hold, guest::newObject(blockBytes<P>()));
    rw::call(P::BufPushBack, {buf, hold});
    for (uint32_t i = load<uint32_t>(d + MAP_BEGIN); i != load<uint32_t>(d + MAP_END); i += 4)
        split::pushBack<true, P::BufDtor>(buf, i);
    for (uint32_t k = 0; k < 16; k += 4) {
        const uint32_t mine = load<uint32_t>(d + k);
        save<uint32_t>(d + k, load<uint32_t>(buf + k));
        save<uint32_t>(buf + k, mine);
    }
    save<uint32_t>(d + START, mapSize<P>(d) == 1 ? P::BS / 2 : load<uint32_t>(d + START) + P::BS);
    rw::call(P::BufDtor, {buf});
}

// push_front(x): the slot before the first element (the end of the previous block at a block boundary)
template <class P, void (*Construct)(uint32_t, uint32_t)> void pushFront(uint32_t d, uint32_t x) {
    if (!load<uint32_t>(d + START)) rw::call(P::AddFront, {d});
    const uint32_t start = load<uint32_t>(d + START);
    const uint32_t mp = load<uint32_t>(d + MAP_BEGIN) + start / P::BS * 4;
    const uint32_t block = load<uint32_t>(mp);
    uint32_t p = load<uint32_t>(d + MAP_END) == load<uint32_t>(d + MAP_BEGIN) ? 0 : block + start % P::BS * P::S;
    if (p == block) p = load<uint32_t>(mp - 4) + blockBytes<P>();
    Construct(p - P::S, x);
    save<uint32_t>(d + START, load<uint32_t>(d + START) - 1);
    save<uint32_t>(d + SIZE, load<uint32_t>(d + SIZE) + 1);
}

// ------------------------------------------------------------------------------------------ erase
using ConnIter = Iter<Connections>;
constexpr uint32_t CONN_BLOCK = Connections::S * Connections::BS;
constexpr uint32_t MoveIntoDeque = 0x49A985, MoveBackwardIntoDeque = 0x49AA45;
constexpr uint32_t MoveDeque = 0x49A881, MoveBackwardDeque = 0x49A75D;

// shared_ptr move assignment: the source emptied, the old owner released
void moveAssign(uint32_t d, uint32_t s) {
    const uint32_t obj = load<uint32_t>(s), ctrl = load<uint32_t>(s + 4);
    save<uint32_t>(s, 0);
    save<uint32_t>(s + 4, 0);
    save<uint32_t>(d, obj);
    const uint32_t old = load<uint32_t>(d + 4);
    save<uint32_t>(d + 4, ctrl);
    if (old) guest::release(old);
}

// one of the pointer-range helpers below, called with (result, first, last, r.m, r.p, 0) -> the iterator it returns
ConnIter callIntoDeque(uint32_t fn, uint32_t first, uint32_t last, const ConnIter &r) {
    guest::Frame frame(8);
    rw::call(fn, {frame.addr(), first, last, r.m, r.p, 0});
    return ConnIter{load<uint32_t>(frame.addr()), load<uint32_t>(frame.addr(4))};
}

// move(T* f, T* l, deque iterator r) -> r after the moved range (block by block)
void moveIntoDeque(uint32_t result, uint32_t f, uint32_t l, uint32_t rm, uint32_t rp, uint32_t) {
    ConnIter r{rm, rp};
    while (f != l) {
        const uint32_t rb = r.p, re = load<uint32_t>(r.m) + CONN_BLOCK;
        const int32_t bs = int32_t(re - rb) / 8;
        int32_t n = int32_t(l - f) / 8;
        uint32_t m = l;
        if (n > bs) {
            n = bs;
            m = f + uint32_t(bs) * 8;
        }
        for (uint32_t dst = rb; f != m; f += 8, dst += 8) moveAssign(dst, f);
        r.advance(n);
    }
    save<uint32_t>(result, r.m);
    save<uint32_t>(result + 4, r.p);
}

// move_backward(T* f, T* l, deque iterator r) -> r before the moved range
void moveBackwardIntoDeque(uint32_t result, uint32_t f, uint32_t l, uint32_t rm, uint32_t rp, uint32_t) {
    ConnIter r{rm, rp};
    while (f != l) {
        ConnIter rpv = r;
        rpv.advance(-1);
        const uint32_t rb = load<uint32_t>(rpv.m), re = rpv.p + 8;
        const int32_t bs = int32_t(re - rb) / 8;
        int32_t n = int32_t(l - f) / 8;
        uint32_t m = f;
        if (n > bs) {
            n = bs;
            m = l - uint32_t(n) * 8;
        }
        for (uint32_t src = l, dst = re; src != m;) {
            src -= 8;
            dst -= 8;
            moveAssign(dst, src);
        }
        l = m;
        r.advance(-n);
    }
    save<uint32_t>(result, r.m);
    save<uint32_t>(result + 4, r.p);
}

// move(deque iterator f, l, r) -> r after the moved range
void moveDeque(uint32_t result, uint32_t fm, uint32_t fp, uint32_t lm, uint32_t lp, uint32_t rm, uint32_t rp) {
    ConnIter f{fm, fp}, r{rm, rp};
    int32_t n = ConnIter{lm, lp}.minus(f);
    while (n > 0) {
        const uint32_t fb = f.p;
        uint32_t fe = load<uint32_t>(f.m) + CONN_BLOCK;
        int32_t bs = int32_t(fe - fb) / 8;
        if (bs > n) {
            bs = n;
            fe = fb + uint32_t(bs) * 8;
        }
        r = callIntoDeque(MoveIntoDeque, fb, fe, r);
        n -= bs;
        f.advance(bs);
    }
    save<uint32_t>(result, r.m);
    save<uint32_t>(result + 4, r.p);
}

// move_backward(deque iterator f, l, r) -> r before the moved range
void moveBackwardDeque(uint32_t result, uint32_t fm, uint32_t fp, uint32_t lm, uint32_t lp, uint32_t rm, uint32_t rp) {
    ConnIter f{fm, fp}, l{lm, lp}, r{rm, rp};
    int32_t n = l.minus(f);
    while (n > 0) {
        if (l.p == load<uint32_t>(l.m)) {   // --l
            l.m -= 4;
            l.p = load<uint32_t>(l.m) + CONN_BLOCK;
        }
        l.p -= 8;
        uint32_t lb = load<uint32_t>(l.m);
        const uint32_t le = l.p + 8;
        int32_t bs = int32_t(le - lb) / 8;
        if (bs > n) {
            bs = n;
            lb = le - uint32_t(bs) * 8;
        }
        r = callIntoDeque(MoveBackwardIntoDeque, lb, le, r);
        n -= bs;
        l.advance(-(bs - 1));
    }
    save<uint32_t>(result, r.m);
    save<uint32_t>(result + 4, r.p);
}

// deque<shared_ptr<Connection>>::erase(it) -> the element after the erased one: the shorter side moved over it
void eraseConnection(uint32_t result, uint32_t d, uint32_t fm, uint32_t fp) {
    using P = Connections;
    const ConnIter b = beginIter<P>(d);
    const int32_t pos = ConnIter{fm, fp}.minus(b);
    ConnIter p = b;
    p.advance(pos);
    ConnIter next = p;
    next.advance(1);
    if (uint32_t(pos) < (load<uint32_t>(d + SIZE) - 1) / 2) {
        guest::Frame res(8);
        rw::call(MoveBackwardDeque, {res.addr(), b.m, b.p, p.m, p.p, next.m, next.p});
        P::destroy(b.p);
        save<uint32_t>(d + SIZE, load<uint32_t>(d + SIZE) - 1);
        save<uint32_t>(d + START, load<uint32_t>(d + START) + 1);
        if (load<uint32_t>(d + START) >= 2 * P::BS) {
            guest::deleteObject(load<uint32_t>(load<uint32_t>(d + MAP_BEGIN)));
            save<uint32_t>(d + MAP_BEGIN, load<uint32_t>(d + MAP_BEGIN) + 4);
            save<uint32_t>(d + START, load<uint32_t>(d + START) - P::BS);
        }
    } else {
        const uint32_t last = load<uint32_t>(d + START) + load<uint32_t>(d + SIZE);
        ConnIter e{load<uint32_t>(d + MAP_BEGIN) + last / P::BS * 4, 0};
        if (load<uint32_t>(d + MAP_END) != load<uint32_t>(d + MAP_BEGIN)) e.p = load<uint32_t>(e.m) + last % P::BS * P::S;
        guest::Frame res(8);
        rw::call(MoveDeque, {res.addr(), next.m, next.p, e.m, e.p, p.m, p.p});
        P::destroy(load<uint32_t>(res.addr(4)));
        save<uint32_t>(d + SIZE, load<uint32_t>(d + SIZE) - 1);
        if (backSpare<P>(d) >= 2 * P::BS) {
            guest::deleteObject(load<uint32_t>(load<uint32_t>(d + MAP_END) - 4));
            save<uint32_t>(d + MAP_END, load<uint32_t>(d + MAP_END) - 4);
        }
    }
    ConnIter r = beginIter<P>(d);
    r.advance(pos);
    save<uint32_t>(result, r.m);
    save<uint32_t>(result + 4, r.p);
}

}  // namespace

// ---------------------------------------------------------------------------------------------- deque<shared_ptr<RESTClient::Connection>>
RW_FUNCTION(0x497F64, "std::__deque_base<shared_ptr<RESTClient::Connection>>::~__deque_base", destroyBase<Connections>);
RW_FUNCTION(0x49D5B8, "std::__deque_base<shared_ptr<RESTClient::Connection>>::clear", clear<Connections>);
RW_FUNCTION(0x498A48, "std::deque<shared_ptr<RESTClient::Connection>>::push_back(const&)", (pushBack<Connections, sharedCopy>));
RW_FUNCTION(0x499160, "std::deque<shared_ptr<RESTClient::Connection>>::pop_front", popFront<Connections>);
RW_FUNCTION(0x49AB7C, "std::deque<shared_ptr<RESTClient::Connection>>::__add_back_capacity", addBackCapacity<Connections>);
RW_FUNCTION(0x498B88, "std::deque<shared_ptr<RESTClient::Connection>>::erase", eraseConnection);
RW_FUNCTION(0x49A75C, "std::move_backward<deque iterator of shared_ptr<RESTClient::Connection>>", moveBackwardDeque);
RW_FUNCTION(0x49A880, "std::move<deque iterator of shared_ptr<RESTClient::Connection>>", moveDeque);
RW_FUNCTION(0x49A984, "std::move<shared_ptr<RESTClient::Connection>*, deque iterator>", moveIntoDeque);
RW_FUNCTION(0x49AA44, "std::move_backward<shared_ptr<RESTClient::Connection>*, deque iterator>", moveBackwardIntoDeque);

// ---------------------------------------------------------------------------------------------- deque<pair<shared_ptr<Connection>, shared_ptr<Result>>>
RW_FUNCTION(0x497F38, "std::__deque_base<pair<shared_ptr<Connection>, shared_ptr<Result>>>::~__deque_base", destroyBase<ConnectionResults>);
RW_FUNCTION(0x49D66C, "std::__deque_base<pair<shared_ptr<Connection>, shared_ptr<Result>>>::clear", clear<ConnectionResults>);
RW_FUNCTION(0x499874, "std::deque<pair<shared_ptr<Connection>, shared_ptr<Result>>>::push_back(&&)", (pushBack<ConnectionResults, pairMove>));
RW_FUNCTION(0x499A50, "std::deque<pair<shared_ptr<Connection>, shared_ptr<Result>>>::__add_back_capacity", addBackCapacity<ConnectionResults>);

// ---------------------------------------------------------------------------------------------- deque<api::Client::Request>
RW_FUNCTION(0x4B71D4, "std::__deque_base<api::Client::Request>::~__deque_base", destroyBase<Requests>);
RW_FUNCTION(0x4B9250, "std::__deque_base<api::Client::Request>::clear", clear<Requests>);
RW_FUNCTION(0x4B735A, "std::deque<api::Client::Request>::push_back(const&)", (pushBack<Requests, requestCopy>));
RW_FUNCTION(0x4B91C0, "std::deque<api::Client::Request>::pop_front", popFront<Requests>);
RW_FUNCTION(0x4B9910, "std::deque<api::Client::Request>::__add_back_capacity", addBackCapacity<Requests>);

// ---------------------------------------------------------------------------------------------- deque<FlashNode::ActFlag> / deque<int> / deque<const void*>
RW_FUNCTION(0x5C37D8, "std::__deque_base<FlashNode::ActFlag>::~__deque_base", destroyBase<ActFlags>);
RW_FUNCTION(0x5C57AC, "std::__deque_base<FlashNode::ActFlag>::clear", clear<ActFlags>);
RW_FUNCTION(0x5C40C8, "std::deque<FlashNode::ActFlag>::push_back(&&)", (pushBack<ActFlags, wordCopy>));
RW_FUNCTION(0x5C412C, "std::deque<FlashNode::ActFlag>::__add_back_capacity", addBackCapacity<ActFlags>);
RW_FUNCTION(0x741CA8, "std::deque<FlashNode::ActFlag>::push_front(const&)", (pushFront<ActFlags, wordCopy>));
RW_FUNCTION(0x741D0C, "std::deque<FlashNode::ActFlag>::__add_front_capacity", addFrontCapacity<ActFlags>);
RW_FUNCTION(0x5C3804, "std::__deque_base<int>::~__deque_base", destroyBase<Ints>);
RW_FUNCTION(0x5C5700, "std::__deque_base<int>::clear", clear<Ints>);
RW_FUNCTION(0x772BBC, "std::__deque_base<const void*>::~__deque_base", destroyBase<Pointers>);
RW_FUNCTION(0x7756BC, "std::__deque_base<const void*>::clear", clear<Pointers>);
RW_FUNCTION(0x77453C, "std::deque<const void*>::pop_front", popFront<Pointers>);

// ---------------------------------------------------------------------------------------------- deque<string>
RW_FUNCTION(0x601D94, "std::__deque_base<string>::~__deque_base", destroyBase<Strings>);
RW_FUNCTION(0x6072F4, "std::__deque_base<string>::clear", clear<Strings>);
RW_FUNCTION(0x601D04, "std::deque<string>::push_back(&&)", (pushBack<Strings, stringMove>));
RW_FUNCTION(0x604148, "std::deque<string>::__add_back_capacity", addBackCapacity<Strings>);

// ---------------------------------------------------------------------------------------------- deque<MasterDataManager::TableInfo>
RW_FUNCTION(0x76CD20, "std::__deque_base<MasterDataManager::TableInfo>::~__deque_base", destroyBase<TableInfos>);
RW_FUNCTION(0x76E570, "std::__deque_base<MasterDataManager::TableInfo>::clear", clear<TableInfos>);
RW_FUNCTION(0x76D35C, "std::deque<MasterDataManager::TableInfo>::push_back(&&)", (pushBack<TableInfos, tableInfoMove>));
RW_FUNCTION(0x76DC5C, "std::deque<MasterDataManager::TableInfo>::pop_front", popFront<TableInfos>);
RW_FUNCTION(0x76DDA0, "std::deque<MasterDataManager::TableInfo>::__add_back_capacity", addBackCapacity<TableInfos>);

// ---------------------------------------------------------------------------------------------- deque<shared_ptr<ResourceInfo>>
RW_FUNCTION(0x772BE8, "std::__deque_base<shared_ptr<ResourceInfo>>::~__deque_base", destroyBase<Resources>);
RW_FUNCTION(0x776000, "std::__deque_base<shared_ptr<ResourceInfo>>::clear", clear<Resources>);
RW_FUNCTION(0x773914, "std::deque<shared_ptr<ResourceInfo>>::push_back(const&)", (pushBack<Resources, sharedCopy>));
RW_FUNCTION(0x7749C0, "std::deque<shared_ptr<ResourceInfo>>::pop_front", popFront<Resources>);
RW_FUNCTION(0x77582C, "std::deque<shared_ptr<ResourceInfo>>::__add_back_capacity", addBackCapacity<Resources>);

// ---------------------------------------------------------------------------------------------- deque<pair<audio::ResourceID, int>>
RW_FUNCTION(0x5C0B72, "std::__deque_base<pair<audio::ResourceID, int>>::~__deque_base", destroyBase<BgmEntries>);
RW_FUNCTION(0x5C1F04, "std::__deque_base<pair<audio::ResourceID, int>>::clear", clear<BgmEntries>);
