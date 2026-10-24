#include "SafeBuffer.hpp"

const SafeBuffer *SafeBuffer::read(const SafeBuffer *self, const uint8_t *dst, const uint8_t *src, uint32_t n,
                                   uint32_t index) {
    for (uint32_t k = 0; k < n; k++) rw::store8(dst + k, uint8_t(src[k] ^ mask(self->key, index + k)));
    return self;
}
RW_FUNCTION(0x45A268, "SafeBuffer::read", SafeBuffer::read);

SafeBuffer *SafeBuffer::write(SafeBuffer *self, const uint8_t *dst, const uint8_t *src, uint32_t n, uint32_t index) {
    for (uint32_t k = 0; k < n; k++) rw::store8(dst + k, uint8_t(src[k] ^ mask(self->key, index + k)));
    // the original computes the page range from guest addresses (unsigned, even when n == 0)
    uint32_t offset = rw::guest_addr(dst) - self->data.addr;
    uint32_t first = offset >> PAGE_SHIFT;
    uint32_t end = (n + offset + 255) >> PAGE_SHIFT;
    for (uint32_t p = first; p < end; p++) rw::store8(self->pageDirty.get() + p, 1);
    return self;
}
RW_FUNCTION(0x459FA0, "SafeBuffer::write", SafeBuffer::write);

uint32_t SafeBuffer::move(SafeBuffer *self, SafeBuffer *other) {
    self->key = other->key;
    self->size = other->size;
    self->data = rw::Ptr<uint8_t>(other->data);
    self->pageCount = other->pageCount;
    self->pageHashes = rw::Ptr<uint32_t>(other->pageHashes);
    self->pageDirty = rw::Ptr<uint8_t>(other->pageDirty);
    self->hashState = other->hashState;
    other->data = nullptr;
    other->pageHashes = nullptr;
    other->pageDirty = nullptr;
    return 0;
}
RW_FUNCTION(0x45A2F0, "SafeBuffer::move", SafeBuffer::move);

int32_t SafeBuffer::get32(uint32_t index) const {
    uint8_t v[4];
    for (uint32_t k = 0; k < 4; k++) v[k] = uint8_t(data[index + k] ^ mask(key, index + k));
    int32_t out;
    std::memcpy(&out, v, 4);
    return out;
}

void SafeBuffer::set32(uint32_t index, int32_t value) {
    uint8_t v[4];
    std::memcpy(v, &value, 4);
    write(this, data.get() + index, v, 4, index);
}

// ------------------------------------------------------------------------------------------ lifetime
namespace {

constexpr uint32_t TIME_SEED = rwlib::sub_C60644;          // sub_C60644(uint64_t *out): gettimeofday in microseconds
constexpr uint32_t XXH32 = rwlib::XXH32;              // XXH32(data, length, seed)
constexpr uint32_t UPDATE_LIST_INIT = 0x459FFD;   // sub_459FFC: constructs the update list on first use
constexpr uint32_t LIST_REMOVE = 0x45A7FD;        // std::list<SafeBuffer*>::remove(const value&)
constexpr uint32_t INPUT_STREAM_CREATE = 0x8DC5F1;        // bg::InputStreamFactory::create(out, data, size, deleter)
constexpr uint32_t MEMORY_STREAM_MAKE_SHARED = 0x45A671;  // std::make_shared<bg::MemoryStream>()
constexpr uint32_t MEMORY_STREAM_WRITE = 0x491A79;        // bg::MemoryStream::write(src, size, count)
constexpr uint32_t MEMORY_STREAM_MOVE = 0x491AF5;         // bg::MemoryStream::move() -> shared_ptr<bg::Data>

// Buffers registered for per-frame re-hashing: a std::list<SafeBuffer*> plus the next buffer to check.
struct UpdateList {
    rw::Field<uint32_t> prev;     // +0  list end node: last element
    rw::Field<uint32_t> next;     // +4  first element
    rw::Field<uint32_t> size;     // +8
    rw::Field<uint32_t> cursor;   // +12 the node the per-frame update continues from
};
struct ListNode {
    rw::Field<uint32_t> prev;
    rw::Field<uint32_t> next;
    rw::Field<uint32_t> value;
};
constexpr uint32_t UPDATE_LIST = 0x10033A0;
UpdateList &updateList() { return *rw::host_ptr<UpdateList>(UPDATE_LIST); }

}  // namespace

void SafeBuffer::initialize(SafeBuffer *self, uint32_t size) {
    guest::Frame f(8);
    rw::call(TIME_SEED, {f.addr()});
    self->key = rw::load<uint32_t>(f.addr());          // low word of the microsecond clock
    self->size = size;
    self->data = rw::Ptr<uint8_t>{guest::newArray(size)};
    uint32_t pages = (size + 255) >> PAGE_SHIFT;
    self->pageCount = pages;
    self->pageHashes = rw::Ptr<uint32_t>{guest::newArray(pages * 4)};
    self->pageDirty = rw::Ptr<uint8_t>{guest::newArray(pages)};
    self->hashState = 0;
}
RW_FUNCTION(0x459E60, "SafeBuffer::initialize", SafeBuffer::initialize);

uint32_t SafeBuffer::createHashes(SafeBuffer *self) {
    uint32_t pages = (self->size + 255) >> PAGE_SHIFT;
    self->pageCount = pages;
    self->pageHashes = rw::Ptr<uint32_t>{guest::newArray(pages * 4)};
    uint32_t dirty = guest::newArray(pages);
    self->pageDirty = rw::Ptr<uint8_t>{dirty};
    self->hashState = 0;
    return dirty;
}
RW_FUNCTION(0x459ED4, "SafeBuffer::createHashes", SafeBuffer::createHashes);

void SafeBuffer::deinitialize(SafeBuffer *self) {
    if (self->pageDirty) guest::deleteArray(self->pageDirty.addr);
    if (self->pageHashes) guest::deleteArray(self->pageHashes.addr);
    if (self->data) guest::deleteArray(self->data.addr);
}
RW_FUNCTION(0x459EFA, "SafeBuffer::deinitialize", SafeBuffer::deinitialize);

SafeBuffer *SafeBuffer::construct(SafeBuffer *self, uint32_t size, uint32_t registerForUpdate) {
    self->vtable = VTABLE;
    initialize(self, size);
    // encodes the fresh (uninitialised) bytes over themselves
    write(self, self->data.get(), self->data.get(), self->size, 0);
    if (registerForUpdate == 1) {
        rw::call(UPDATE_LIST_INIT, {});
        // update list push_back(self)
        uint32_t node = guest::newObject(12);
        ListNode &n = *rw::host_ptr<ListNode>(node);
        UpdateList &list = updateList();
        n.value = rw::guest_addr(self);
        rw::host_ptr<ListNode>(list.prev)->next = node;
        n.prev = list.prev;
        list.prev = node;
        n.next = UPDATE_LIST;
        list.size += 1;
        list.cursor = list.next;
    }
    return self;
}
RW_FUNCTION(0x459F20, "SafeBuffer::SafeBuffer(unsigned int, bool)", SafeBuffer::construct);

SafeBuffer *SafeBuffer::destruct(SafeBuffer *self) {
    self->vtable = VTABLE;
    rw::call(UPDATE_LIST_INIT, {});
    {
        guest::Frame f(8);
        rw::store32(f.at<uint32_t>(0), rw::guest_addr(self));
        rw::call(LIST_REMOVE, {UPDATE_LIST, f.addr()});    // update list .remove(self)
    }
    UpdateList &list = updateList();
    list.cursor = list.next;
    deinitialize(self);
    return self;
}
RW_FUNCTION(0x45A13C, "SafeBuffer::~SafeBuffer", SafeBuffer::destruct);

void SafeBuffer::destructDelete(SafeBuffer *self) {
    guest::deleteObject(rw::guest_addr(destruct(self)));
}
RW_FUNCTION(0x45A1C6, "SafeBuffer::~SafeBuffer (deleting)", SafeBuffer::destructDelete);

void SafeBuffer::copy(SafeBuffer *self, const SafeBuffer *other) {
    initialize(self, other->size);
    for (uint32_t k = 0; k < self->size; k++) {
        uint8_t plain = uint8_t(other->data[k] ^ mask(other->key, k));
        write(self, self->data.get() + k, &plain, 1, k);
    }
}
RW_FUNCTION(0x45A1D4, "SafeBuffer::copy", SafeBuffer::copy);

SafeBuffer *SafeBuffer::copyConstruct(SafeBuffer *self, const SafeBuffer *other) {
    self->vtable = VTABLE;
    copy(self, other);
    return self;
}
RW_FUNCTION(0x45A29C, "SafeBuffer::SafeBuffer(const SafeBuffer&)", SafeBuffer::copyConstruct);

SafeBuffer *SafeBuffer::copyAssign(SafeBuffer *self, const SafeBuffer *other) {
    if (self != other) {
        deinitialize(self);
        copy(self, other);
    }
    return self;
}
RW_FUNCTION(0x45A2C8, "SafeBuffer::operator=(const SafeBuffer&)", SafeBuffer::copyAssign);

SafeBuffer *SafeBuffer::moveConstruct(SafeBuffer *self, SafeBuffer *other) {
    self->vtable = VTABLE;
    self->key = other->key;
    self->size = other->size;
    self->data = rw::Ptr<uint8_t>(other->data);
    self->pageCount = other->pageCount;
    self->pageHashes = rw::Ptr<uint32_t>(other->pageHashes);
    self->pageDirty = rw::Ptr<uint8_t>(other->pageDirty);
    self->hashState = other->hashState;
    other->data = nullptr;
    other->pageHashes = nullptr;
    other->pageDirty = nullptr;
    return self;
}
RW_FUNCTION(0x45A318, "SafeBuffer::SafeBuffer(SafeBuffer&&)", SafeBuffer::moveConstruct);

SafeBuffer *SafeBuffer::moveAssign(SafeBuffer *self, SafeBuffer *other) {
    if (self != other) {
        deinitialize(self);
        move(self, other);
    }
    return self;
}
RW_FUNCTION(0x45A354, "SafeBuffer::operator=(SafeBuffer&&)", SafeBuffer::moveAssign);

bool SafeBuffer::update(SafeBuffer *self, const uint8_t *wrapped) {
    bool ok = true;
    if (self->size == 0) return ok;
    uint32_t start = self->hashState << PAGE_SHIFT;
    uint32_t left = self->size - start;
    uint32_t length = left <= PAGE_SIZE ? left : PAGE_SIZE;
    uint32_t hash = rw::call(XXH32, {self->data.addr + start, length, 0});
    uint32_t page = self->hashState;
    const uint8_t *dirty = self->pageDirty.get() + page;
    if (*dirty) {
        // written since the last pass: take the new hash
        rw::store8(dirty, 0);
        rw::store32(self->pageHashes.get() + self->hashState, hash);
        page = self->hashState;
    } else {
        ok = self->pageHashes[page] == hash;
    }
    uint32_t next = page + 1;
    if (next >= self->pageCount) next = 0;
    self->hashState = next;
    if (wrapped) rw::store8(wrapped, next == 0 ? 1 : 0);
    return ok;
}
RW_FUNCTION(0x45A394, "SafeBuffer::update", SafeBuffer::update);

SafeBuffer *SafeBuffer::constructFromData(SafeBuffer *self, const guest::SharedPtr *data) {
    self->vtable = VTABLE;
    // the original's frame: the stream shared_ptr at +0x10, the (empty) deleter std::function at +0x18 with
    // its __f_ pointer at +0x28
    guest::Frame f(0x3C);
    const uint32_t stream = f.addr(0x10), deleter = f.addr(0x18);
    uint32_t blob = data->obj;   // bg::Data: +4 bytes, +8 size
    rw::save<uint32_t>(f.addr(0x28), 0);
    rw::call(INPUT_STREAM_CREATE, {stream, rw::load<uint32_t>(blob + 4), rw::load<uint32_t>(blob + 8), deleter});
    // ~std::function of the deleter
    uint32_t fn = rw::load<uint32_t>(f.addr(0x28));
    if (fn == deleter) {
        rw::call(rw::load<uint32_t>(rw::load<uint32_t>(deleter) + 16), {deleter});   // destroy() (in place)
    } else if (fn) {
        rw::call(rw::load<uint32_t>(rw::load<uint32_t>(fn) + 20), {fn});             // destroy_deallocate()
    }
    // the stream's read(dst, size, count) is vtable slot 2; anything short of one item quits the game
    auto read = [&](uint32_t dst, uint32_t size) {
        uint32_t in = rw::load<uint32_t>(stream);
        return rw::vcall(rw::host_ptr<void>(in), 2, {in, dst, size, 1}) == 1;
    };
    if (!read(rw::guest_addr(&self->key), 4)) rw::call(guest::EXIT, {0});
    if (!read(rw::guest_addr(&self->size), 4)) rw::call(guest::EXIT, {0});
    uint32_t size = self->size;
    uint32_t bytes = guest::newArray(size);
    self->data = rw::Ptr<uint8_t>{bytes};
    if (!read(bytes, size)) rw::call(guest::EXIT, {0});
    uint32_t pages = (self->size + 255) >> PAGE_SHIFT;
    self->pageCount = pages;
    self->pageHashes = rw::Ptr<uint32_t>{guest::newArray(pages * 4)};
    self->pageDirty = rw::Ptr<uint8_t>{guest::newArray(pages)};
    self->hashState = 0;
    uint32_t ctrl = rw::load<uint32_t>(stream + 4);
    if (ctrl) guest::release(ctrl);
    return self;
}
RW_FUNCTION(0x45A420, "SafeBuffer::SafeBuffer(const shared_ptr<bg::Data>&)", SafeBuffer::constructFromData);

void SafeBuffer::serialize(guest::SharedPtr *out, const SafeBuffer *self) {
    guest::Frame f(0x14);
    const uint32_t ms = f.addr(8);                     // shared_ptr<bg::MemoryStream>
    rw::call(MEMORY_STREAM_MAKE_SHARED, {ms});
    uint32_t stream = rw::load<uint32_t>(ms);
    if (rw::call(MEMORY_STREAM_WRITE, {stream, rw::guest_addr(&self->key), 4, 1}) == 1 &&
        rw::call(MEMORY_STREAM_WRITE, {stream, rw::guest_addr(&self->size), 4, 1}) == 1)
        rw::call(MEMORY_STREAM_WRITE, {stream, self->data.addr, self->size, 1});
    rw::call(MEMORY_STREAM_MOVE, {rw::guest_addr(out), stream});
    uint32_t ctrl = rw::load<uint32_t>(ms + 4);
    if (ctrl) guest::release(ctrl);
}
RW_FUNCTION(0x45A554, "SafeBuffer::serialize", SafeBuffer::serialize);

void SafeBuffer::zerofill(SafeBuffer *self) {
    const uint8_t zero = 0;
    for (uint32_t k = 0; k < self->size; k++) write(self, self->data.get() + k, &zero, 1, k);
}
RW_FUNCTION(0x45A604, "SafeBuffer::zerofill", SafeBuffer::zerofill);

// ------------------------------------------------------------------------------------------ update list
namespace {
constexpr uint32_t UPDATE_LIST_GUARD = 0x10033B4;          // the update list's static guard
constexpr uint32_t UPDATE_LIST_DTOR = 0x45A879;            // sub_45A878: ~list (registered with __cxa_atexit)
constexpr uint32_t UPDATE_FUNCTOR_VTABLE = 0xF53858;       // std::function<void(float)> __func of the update lambda
constexpr uint32_t DSO_HANDLE_GOT = 0xFD604C;              // GOT slot holding &__dso_handle
constexpr uint32_t DIRECTOR_GET_INSTANCE = rwlib::cocos2d::Director::getInstance;       // cocos2d::Director::getInstance()
constexpr uint32_t DIRECTOR_GET_SCHEDULER = rwlib::cocos2d::Director::getScheduler;      // cocos2d::Director::getScheduler() const
constexpr uint32_t SCHEDULER_SCHEDULE_PER_FRAME = rwlib::cocos2d::Scheduler::schedulePerFrame;  // Scheduler::schedulePerFrame(fn, target, priority, paused)
}  // namespace

// The update list's first use (a function-local static): an empty std::list plus a cursor, and a per-frame
// scheduler callback (target = the list) that re-hashes the registered buffers page by page.
static void SafeBuffer_initUpdateList() {
    guest::Frame f(0x2C);
    if (rw::call(guest::ATOMIC_LOAD_GUARD, {UPDATE_LIST_GUARD, 0, 0}) & 1) return;
    if (!rw::call(guest::CXA_GUARD_ACQUIRE, {UPDATE_LIST_GUARD})) return;
    UpdateList &list = updateList();
    list.prev = UPDATE_LIST;
    list.next = UPDATE_LIST;
    list.size = 0;
    list.cursor = 0;
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    uint32_t scheduler = rw::call(DIRECTOR_GET_SCHEDULER, {director, 0x7FFFFFFF});   // r1: a stale literal
    // std::function<void(float)> holding the lambda (captures the list) in its small buffer
    const uint32_t fn = f.addr(8);
    *rw::frame_ptr<uint32_t>(fn) = UPDATE_FUNCTOR_VTABLE;
    *rw::frame_ptr<uint32_t>(fn + 4) = UPDATE_LIST;
    *rw::frame_ptr<uint32_t>(fn + 16) = fn;
    rw::call(SCHEDULER_SCHEDULE_PER_FRAME, {scheduler, fn, UPDATE_LIST, 0, 0});
    // ~function: destroy (slot 4) when stored inline, destroy_deallocate (slot 5) when on the heap
    uint32_t callable = rw::load<uint32_t>(fn + 16);
    if (callable == fn) rw::vcall(rw::host_ptr<uint32_t>(fn), 4, {fn});
    else if (callable) rw::vcall(rw::host_ptr<uint32_t>(callable), 5, {callable});
    rw::call(guest::CXA_ATEXIT, {UPDATE_LIST_DTOR, UPDATE_LIST, rw::load<uint32_t>(DSO_HANDLE_GOT)});
    guest::staticInitDone(UPDATE_LIST_GUARD);
}
RW_FUNCTION(0x459FFC, "SafeBuffer update list static init (sub_459FFC)", SafeBuffer_initUpdateList);

// __clang_call_terminate: an exception escaped a noexcept destructor (~SafeBuffer's landing pad).
static void SafeBuffer_callTerminate(uint32_t exception) {
    rw::call(0xC985F1, {exception});   // __cxa_begin_catch
    rw::call(0xC98E59, {});            // std::terminate (does not return)
}
RW_FUNCTION(0x45A1BC, "__clang_call_terminate (sub_45A1BC)", SafeBuffer_callTerminate);
