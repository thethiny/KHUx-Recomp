#include "ResourceInfo.hpp"

namespace hole::network {

using api::GuestFrame;
using api::JsonValue;

namespace {
constexpr uint32_t KEY_DATA = 0xDFC324;           // "data"
constexpr uint32_t KEY_INDEX = 0xE07C30;          // "index"
constexpr uint32_t FILES_PARSE = 0x7764C1;        // parse a file list (vector<record>*, const GenericValue&) -> bool
constexpr uint32_t FILES_COPY = 0x7761AD;         // vector<record> copy constructor
constexpr uint32_t FILES_DTOR = 0x7762BD;         // vector<record> destructor
constexpr uint32_t SPLIT_BUFFER_DTOR = 0x776171;  // __split_buffer<Entry>::~__split_buffer
constexpr uint32_t THROW_LENGTH = 0x47B66D;       // __vector_base_common::__throw_length_error
constexpr uint32_t DEQUE_PUSH_BACK = 0x774691;    // std::deque<const void*>::push_back(const&)
constexpr uint32_t MANAGER_QUEUE = 0x30;          // ResourceDataManager: the deque of records to fetch

inline uint32_t R(uint32_t a) { return rw::load<uint32_t>(a); }
inline void W(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
}  // namespace

uint32_t ResourceInfo::init(ResourceInfo *self, const JsonValue *json) {
    // original frame 0x80: the Entry being parsed at +0x38, the reallocation's __split_buffer at +0x54
    // {first, begin, end, capEnd, &alloc}
    GuestFrame frame(0x80);
    if (!json->IsArray()) return 0;
    uint32_t n = json->Size();
    Entry *local = frame.at<Entry>(0x38);
    uint32_t *split = frame.at<uint32_t>(0x54);
    const uint32_t localData = frame.addr(0x3C), localIndex = frame.addr(0x48);

    // copy-construct the local entry at dst (the id is stored around each copy)
    auto construct = [&](uint32_t dst, int32_t id) {
        Entry *d = rw::host_ptr<Entry>(dst);
        d->id = id;
        rw::call(FILES_COPY, {dst + 4, localData});
        d->id = local->id.get();
        rw::call(FILES_COPY, {dst + 16, localIndex});
        d->id = local->id.get();
    };

    for (uint32_t k = 0; k < n; k++) {
        rw::call(api::guest::memset_, {localData, 0x18, 0});
        int32_t id = int32_t(self->baseId.get() + k);
        const JsonValue &e = json->At(k);
        bool ok = e.IsObject() && rw::call(FILES_PARSE, {localData, rw::guest_addr(&e.member(KEY_DATA))}) == 1 &&
                  rw::call(FILES_PARSE, {localIndex, rw::guest_addr(&e.member(KEY_INDEX))}) != 0;
        if (!ok) {
            rw::call(FILES_DTOR, {localIndex});
            rw::call(FILES_DTOR, {localData});
            return 0;
        }
        local->id = id;

        uint32_t end = self->entries.end;
        if (end != self->entries.capEnd) {
            uint32_t base = 0;
            if (end) {
                construct(end, id);
                base = self->entries.end;
            }
            self->entries.end = base + sizeof(Entry);
        } else {
            // __push_back_slow_path, inlined
            uint32_t size = uint32_t(int32_t(end - self->entries.begin.get()) >> 2) * 0xB6DB6DB7u;   // / 28
            uint32_t want = size + 1;
            if (want >= 0x924924A) {
                rw::call(THROW_LENGTH, {rw::guest_addr(&self->entries)});   // does not return
                return 0;
            }
            uint32_t cap;
            split[3] = 0;
            split[4] = rw::guest_addr(&self->entries.capEnd);
            if (size >= 0x4924924) {
                cap = 0x9249249;
            } else {
                cap = 2 * size >= want ? 2 * size : want;
            }
            uint32_t buf = cap ? rw::call(api::guest::operator_new, {28 * cap}) : 0;
            uint32_t pos = buf + 28 * size;
            split[0] = buf;
            split[1] = pos;
            split[2] = pos;
            split[3] = buf + 28 * cap;
            uint32_t nb = 0, ne = 0;
            if (pos) {
                construct(pos, local->id);
                nb = split[1];
                ne = split[2];
            }
            ne += sizeof(Entry);
            split[2] = ne;

            // move the old entries in front of it, last first
            uint32_t ob = self->entries.begin, oe = self->entries.end;
            uint32_t newBegin, oldEnd;
            if (oe != ob) {
                uint32_t s = oe - 12;             // the old entry's index vector
                do {
                    W(nb - 28, R(s - 16));
                    W(nb - 24, 0); W(nb - 20, 0); W(nb - 16, 0);
                    W(nb - 24, R(s - 12)); W(nb - 20, R(s - 8)); W(nb - 16, R(s - 4));
                    W(s - 4, 0); W(s - 8, 0); W(s - 12, 0);
                    W(nb - 28, R(s - 16));
                    W(nb - 12, 0); W(nb - 8, 0); W(nb - 4, 0);
                    W(nb - 12, R(s)); W(nb - 8, R(s + 4)); W(nb - 4, R(s + 8));
                    W(s, 0); W(s + 4, 0); W(s + 8, 0);
                    W(nb - 28, R(s - 16));
                    split[1] = split[1] - 28;
                    nb = split[1];
                    s -= 28;
                } while (s != ob - 12);
                newBegin = self->entries.begin;
                oldEnd = self->entries.end;
                ne = split[2];
            } else {
                newBegin = ob;
                oldEnd = ob;
            }
            self->entries.begin = nb;
            split[1] = newBegin;
            self->entries.end = ne;
            split[2] = oldEnd;
            uint32_t oldCap = self->entries.capEnd;
            self->entries.capEnd = split[3];
            split[3] = oldCap;
            split[0] = newBegin;
            rw::call(SPLIT_BUFFER_DTOR, {frame.addr(0x54)});
        }
        rw::call(FILES_DTOR, {localIndex});
        rw::call(FILES_DTOR, {localData});
    }
    self->lastId = int32_t(n + self->baseId.get() - 1);
    return 1;
}
RW_FUNCTION(0x772480, "hole::network::ResourceInfo::init", ResourceInfo::init);

uint32_t ResourceInfo::acceptForDataFile(const ResourceInfo *self, uint32_t manager) {
    GuestFrame frame(0x28);                   // original frame: the pushed pointer at +0xc
    uint32_t *slot = frame.at<uint32_t>(0xC);
    for (uint32_t e = self->entries.begin, end = self->entries.end; e != end; e += sizeof(Entry)) {
        const Entry *entry = rw::host_ptr<Entry>(e);
        for (uint32_t p = entry->data.begin, last = entry->data.end; p != last; p += 28) {
            *slot = p;
            rw::call(DEQUE_PUSH_BACK, {manager + MANAGER_QUEUE, frame.addr(0xC)});
        }
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x7727C4, "hole::network::ResourceInfo::acceptForDataFile", ResourceInfo::acceptForDataFile);

uint32_t ResourceInfo::acceptForIndexFile(const ResourceInfo *self, uint32_t manager) {
    GuestFrame frame(0x20);                   // original frame: the pushed pointer at +4
    uint32_t *slot = frame.at<uint32_t>(4);
    uint32_t begin = self->entries.begin;
    uint32_t last = begin + ((self->entries.end - begin) & ~3u);   // the end of the last entry
    uint32_t stop = R(last - 8), p = R(last - 12);                  // its index vector
    for (; p != stop; p += 28) {
        *slot = p;
        rw::call(DEQUE_PUSH_BACK, {manager + MANAGER_QUEUE, frame.addr(4)});
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x772838, "hole::network::ResourceInfo::acceptForIndexFile", ResourceInfo::acceptForIndexFile);

}  // namespace hole::network
