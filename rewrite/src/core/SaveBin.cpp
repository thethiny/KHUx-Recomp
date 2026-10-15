#include "SaveBin.hpp"

namespace {

constexpr uint32_t STRLEN = 0x40E08C;                 // strlen (import)
constexpr uint32_t STRING_INIT = 0x47CD93;            // std::string::__init(const char*, size_t) (sub_47CD92)
constexpr uint32_t MAP_FIND = 0x4F15A7;               // __hash_table<string, shared_ptr<bg::Data>>::find(const string&)
constexpr uint32_t MAP_ERASE_UNIQUE = 0x4F1315;       // ...::__erase_unique(const string&)
constexpr uint32_t MAP_INSERT_UNIQUE = 0x4F16A5;      // ...::__insert_unique(pair<const char*, shared_ptr<bg::Data>>&&)
constexpr uint32_t MAP_CLEAR = 0x4F12DD;              // ...::clear()
constexpr uint32_t MEMORY_STREAM_CTOR = 0x491665;     // bg::MemoryStream(size, const function<void*(unsigned)>&,
                                                      //                  const function<void(void*)>&)
constexpr uint32_t MEMORY_STREAM_DTOR = 0x491929;     // bg::MemoryStream::~MemoryStream()
constexpr uint32_t MEMORY_STREAM_WRITE = 0x491A79;    // bg::MemoryStream::write(src, size, count)
constexpr uint32_t MEMORY_STREAM_MOVE = 0x491AF5;     // bg::MemoryStream::move() -> shared_ptr<bg::Data> (sret)
constexpr uint32_t INPUT_STREAM_CTOR = 0x8DC3F9;      // bg::MemoryInputStream(const shared_ptr<bg::Data>&)
constexpr uint32_t INPUT_STREAM_DTOR = 0x8DC425;      // bg::MemoryInputStream::~MemoryInputStream()
constexpr uint32_t INPUT_STREAM_MAKE_SHARED = 0x4F11F9;  // make_shared<bg::MemoryInputStream>(const shared_ptr<Data>&)
constexpr uint32_t ARCHIVE_GET_INSTANCE = 0x8DC8F1;   // bg::Archive::getInstance()
constexpr uint32_t ARCHIVE_CONVERT = 0x8DCDFD;        // bg::Archive::convert(key, bytes, size) -> shared_ptr<Data>
constexpr uint32_t ARCHIVE_GET_KEY = 0x8DC9CD;        // bg::Archive::getKey(shared_ptr<IInputStream>, u16*)
constexpr uint32_t ARCHIVE_GET_DATA = 0x8DCBA9;       // bg::Archive::getData(shared_ptr<IInputStream>, bool, u16*)

// bg::IInputStream virtual slots
constexpr uint32_t STREAM_READ = 2;                   // read(dst, size, count) -> elements read
constexpr uint32_t STREAM_TELL = 3;
constexpr uint32_t STREAM_SEEK = 4;                   // seek(long offset, int whence)

// libc++ std::function (16-byte buffer, callable pointer __f_ at +16): virtual slots of the callable
constexpr uint32_t FUNC_DESTROY = 4;                  // destroy() (stored in the buffer)
constexpr uint32_t FUNC_DESTROY_DEALLOCATE = 5;       // destroy_deallocate() (heap)

// std::function::~function (inlined in the original)
void destroyFunction(uint32_t fn) {
    uint32_t f = rw::load<uint32_t>(fn + 16);
    if (f == fn) rw::vcall(rw::host_ptr<uint8_t>(f), FUNC_DESTROY, {f});
    else if (f) rw::vcall(rw::host_ptr<uint8_t>(f), FUNC_DESTROY_DEALLOCATE, {f});
}

// The value writers build a bg::MemoryStream with default (empty) allocator / deleter functions.
// Frame layout of the original: result shared_ptr, deleter function, allocator function, stream.
struct WriterFrame {
    static constexpr uint32_t SIZE = 0xB0;
    static constexpr uint32_t RESULT = 0x08;
    static constexpr uint32_t DELETER = 0x10;     // std::function<void(void*)>
    static constexpr uint32_t ALLOCATOR = 0x28;   // std::function<void*(unsigned)>
    static constexpr uint32_t STREAM = 0x40;
    static constexpr uint32_t TAG = 0x3C;         // ioutil tag argument (empty struct)
};

void openStream(const guest::Frame &f, uint32_t reserve) {
    rw::save<uint32_t>(f.addr(WriterFrame::ALLOCATOR + 16), 0);
    rw::save<uint32_t>(f.addr(WriterFrame::DELETER + 16), 0);
    rw::call(MEMORY_STREAM_CTOR,
             {f.addr(WriterFrame::STREAM), reserve, f.addr(WriterFrame::ALLOCATOR), f.addr(WriterFrame::DELETER)});
    destroyFunction(f.addr(WriterFrame::DELETER));
    destroyFunction(f.addr(WriterFrame::ALLOCATOR));
}

// stream.move() -> blob, set(key, blob), stream destroyed
void storeStream(const guest::Frame &f, SaveBin *self, const char *key) {
    uint32_t result = f.addr(WriterFrame::RESULT);
    rw::call(MEMORY_STREAM_MOVE, {result, f.addr(WriterFrame::STREAM)});
    SaveBin::set(self, key, rw::host_ptr<guest::SharedPtr>(result));
    uint32_t ctrl = rw::load<uint32_t>(result + 4);
    if (ctrl) guest::release(ctrl);
    rw::call(MEMORY_STREAM_DTOR, {f.addr(WriterFrame::STREAM)});
}

// Reader frame: the blob, the MemoryInputStream over it.
struct ReaderFrame {
    static constexpr uint32_t SIZE = 0x48;
    static constexpr uint32_t DATA = 0x04;
    static constexpr uint32_t TAG = 0x0C;
    static constexpr uint32_t STREAM = 0x10;
};

// The container (de)serializers of bg::ioutil, per type
struct SavedString {        // std::string
    static constexpr uint32_t SIZE = 12;
    static constexpr uint32_t WRITE = 0x4D9255;   // ioutil::_write<string>(const T&, MemoryStream*, const vector_tag&)
    static constexpr uint32_t READ = 0x4D9019;    // ioutil::_read<string>(T&, IInputStream*, const vector_tag&)
};
struct SavedIntVector {     // std::vector<int>
    static constexpr uint32_t SIZE = 12;
    static constexpr uint32_t WRITE = 0x4EDAE9;
    static constexpr uint32_t READ = 0x4EE3B1;
};
struct SavedU64Vector {     // std::vector<unsigned long long>
    static constexpr uint32_t SIZE = 12;
    static constexpr uint32_t WRITE = 0x4ED9A1;
    static constexpr uint32_t READ = 0x4EDD89;
};
struct SavedUIntMap {       // std::unordered_map<unsigned, unsigned> (map_tag)
    static constexpr uint32_t SIZE = 20;
    static constexpr uint32_t WRITE = 0x4EDA41;
    static constexpr uint32_t READ = 0x4EE2E9;
};

}  // namespace

// ------------------------------------------------------------------------------------------ typed values
template <class T>
void SaveBin::setValue(SaveBin *self, const char *key, const T *value) {
    guest::Frame f(WriterFrame::SIZE);
    openStream(f, sizeof(T));
    rw::call(MEMORY_STREAM_WRITE, {f.addr(WriterFrame::STREAM), rw::arg(value), sizeof(T), 1});
    storeStream(f, self, key);
}

template <class T>
uint32_t SaveBin::getValue(const SaveBin *self, const char *key, const T *out) {
    guest::Frame f(ReaderFrame::SIZE);
    uint32_t data = f.addr(ReaderFrame::DATA);
    get(rw::host_ptr<guest::SharedPtr>(data), self, key);
    uint32_t ok = 0;
    if (rw::load<uint32_t>(data)) {
        uint32_t stream = f.addr(ReaderFrame::STREAM);
        rw::call(INPUT_STREAM_CTOR, {stream, data});
        uint32_t n = rw::vcall(rw::host_ptr<uint8_t>(stream), STREAM_READ, {stream, rw::arg(out), sizeof(T), 1});
        rw::call(INPUT_STREAM_DTOR, {stream});
        ok = n == 1 ? 1 : 0;
    }
    uint32_t ctrl = rw::load<uint32_t>(data + 4);
    if (ctrl) guest::release(ctrl);
    return ok;
}

static_assert(sizeof(long) == 4, "guest long is 32-bit");
RW_FUNCTION(0x4D8880, "SaveBin::set<unsigned int>", SaveBin::setValue<uint32_t>);
RW_FUNCTION(0x4D8AB4, "SaveBin::set<int>", SaveBin::setValue<int32_t>);
RW_FUNCTION(0x4E00FC, "SaveBin::set<unsigned char>", SaveBin::setValue<uint8_t>);
RW_FUNCTION(0x4E0218, "SaveBin::set<long>", SaveBin::setValue<long>);
RW_FUNCTION(0x4E956C, "SaveBin::set<bool>", SaveBin::setValue<bool>);
RW_FUNCTION(0x4EC28C, "SaveBin::set<signed char>", SaveBin::setValue<int8_t>);
RW_FUNCTION(0x4EC528, "SaveBin::set<unsigned short>", SaveBin::setValue<uint16_t>);

RW_FUNCTION(0x4D8DDC, "SaveBin::get<int>", SaveBin::getValue<int32_t>);
RW_FUNCTION(0x4D8E8C, "SaveBin::get<unsigned int>", SaveBin::getValue<uint32_t>);
RW_FUNCTION(0x4E0334, "SaveBin::get<unsigned char>", SaveBin::getValue<uint8_t>);
RW_FUNCTION(0x4E03E8, "SaveBin::get<long>", SaveBin::getValue<long>);
RW_FUNCTION(0x4E86F4, "SaveBin::get<bool>", SaveBin::getValue<bool>);
RW_FUNCTION(0x4EC1D8, "SaveBin::get<signed char>", SaveBin::getValue<int8_t>);
RW_FUNCTION(0x4EC478, "SaveBin::get<unsigned short>", SaveBin::getValue<uint16_t>);

// ------------------------------------------------------------------------------------------ containers
template <class C>
void SaveBin::setContainer(SaveBin *self, const char *key, uint32_t value) {
    guest::Frame f(WriterFrame::SIZE);
    openStream(f, C::SIZE);
    rw::call(C::WRITE, {value, f.addr(WriterFrame::STREAM), f.addr(WriterFrame::TAG)});
    storeStream(f, self, key);
}

// returns what bg::ioutil::_read returned (0 when the key is missing)
template <class C>
uint32_t SaveBin::getContainer(const SaveBin *self, const char *key, uint32_t out) {
    guest::Frame f(ReaderFrame::SIZE);
    uint32_t data = f.addr(ReaderFrame::DATA);
    get(rw::host_ptr<guest::SharedPtr>(data), self, key);
    uint32_t ok = 0;
    if (rw::load<uint32_t>(data)) {
        uint32_t stream = f.addr(ReaderFrame::STREAM);
        rw::call(INPUT_STREAM_CTOR, {stream, data});
        ok = rw::call(C::READ, {out, stream, f.addr(ReaderFrame::TAG)});
        rw::call(INPUT_STREAM_DTOR, {stream});
    }
    uint32_t ctrl = rw::load<uint32_t>(data + 4);
    if (ctrl) guest::release(ctrl);
    return ok;
}

RW_FUNCTION(0x4D899C, "SaveBin::set<std::string>", SaveBin::setContainer<SavedString>);
RW_FUNCTION(0x4E933C, "SaveBin::set<std::vector<int>>", SaveBin::setContainer<SavedIntVector>);
RW_FUNCTION(0x4E9454, "SaveBin::set<std::unordered_map<unsigned, unsigned>>", SaveBin::setContainer<SavedUIntMap>);
RW_FUNCTION(0x4EA320, "SaveBin::set<std::vector<unsigned long long>>", SaveBin::setContainer<SavedU64Vector>);

RW_FUNCTION(0x4D8F3C, "SaveBin::get<std::string>", SaveBin::getContainer<SavedString>);
RW_FUNCTION(0x4E7828, "SaveBin::get<std::vector<int>>", SaveBin::getContainer<SavedIntVector>);
RW_FUNCTION(0x4E78CC, "SaveBin::get<std::unordered_map<unsigned, unsigned>>", SaveBin::getContainer<SavedUIntMap>);
RW_FUNCTION(0x4E8650, "SaveBin::get<std::vector<unsigned long long>>", SaveBin::getContainer<SavedU64Vector>);

// ------------------------------------------------------------------------------------------ entries
void SaveBin::set(SaveBin *self, const char *key, const guest::SharedPtr *data) {
    remove(self, key);
    guest::Frame f(0x20);
    // pair<const char*, shared_ptr<bg::Data>>(key, data) at +4, insert result at +0x10
    uint32_t pair = f.addr(4);
    rw::save<uint32_t>(pair, rw::arg(key));
    rw::save<uint32_t>(pair + 4, data->obj);
    uint32_t ctrl = data->ctrl;
    rw::save<uint32_t>(pair + 8, ctrl);
    if (ctrl) guest::addRef(ctrl);
    rw::call(MAP_INSERT_UNIQUE, {f.addr(0x10), rw::arg(self), pair});
    ctrl = rw::load<uint32_t>(pair + 8);   // the insert moved from the pair
    if (ctrl) guest::release(ctrl);
}
RW_FUNCTION(0x4F0CE0, "SaveBin::set", SaveBin::set);

void SaveBin::remove(SaveBin *self, const char *key) {
    guest::Frame f(0x18);
    uint32_t name = f.addr(4);   // std::string(key)
    uint32_t len = rw::call(STRLEN, {rw::arg(key)});
    rw::call(STRING_INIT, {name, rw::arg(key), len});
    rw::call(MAP_ERASE_UNIQUE, {rw::arg(self), name});
    if (rw::load<uint8_t>(name) & 1) guest::deleteObject(rw::load<uint32_t>(name + 8));
}
RW_FUNCTION(0x4F0D74, "SaveBin::remove", SaveBin::remove);

void SaveBin::get(guest::SharedPtr *out, const SaveBin *self, const char *key) {
    guest::Frame f(0x20);
    uint32_t name = f.addr(0xC);
    uint32_t len = rw::call(STRLEN, {rw::arg(key)});
    rw::call(STRING_INIT, {name, rw::arg(key), len});
    uint32_t found = rw::call(MAP_FIND, {rw::arg(self), name});
    if (rw::load<uint8_t>(name) & 1) guest::deleteObject(rw::load<uint32_t>(name + 8));
    if (found) {
        Node *node = rw::host_ptr<Node>(found);
        out->obj = node->value.obj;
        uint32_t ctrl = node->value.ctrl;
        out->ctrl = ctrl;
        if (ctrl) guest::addRef(ctrl);
    } else {
        out->obj = 0;
        out->ctrl = 0;
    }
}
RW_FUNCTION(0x4F0DEC, "SaveBin::get", SaveBin::get);

void SaveBin::clear(SaveBin *self) { rw::call(MAP_CLEAR, {rw::arg(self)}); }
RW_FUNCTION(0x4F0E8C, "SaveBin::clear", SaveBin::clear);

// ------------------------------------------------------------------------------------------ whole store
// Every entry goes through bg::Archive::convert(key, bytes) and is appended to one MemoryStream. `out` doubles as
// the per-entry scratch shared_ptr (its references are released, the pointer values stay until the final move).
// An entry that fails to convert ends it early: `out` is then the empty result of that convert.
void SaveBin::serialize(guest::SharedPtr *out, const SaveBin *self) {
    guest::Frame f(WriterFrame::SIZE);
    rw::save<uint32_t>(f.addr(WriterFrame::ALLOCATOR + 16), 0);
    rw::save<uint32_t>(f.addr(WriterFrame::DELETER + 16), 0);
    uint32_t stream = f.addr(WriterFrame::STREAM);
    rw::call(MEMORY_STREAM_CTOR,
             {stream, 0x10000, f.addr(WriterFrame::ALLOCATOR), f.addr(WriterFrame::DELETER)});
    destroyFunction(f.addr(WriterFrame::DELETER));
    destroyFunction(f.addr(WriterFrame::ALLOCATOR));

    for (uint32_t n = self->firstNode; n; n = rw::load<uint32_t>(n)) {
        uint32_t archive = rw::call(ARCHIVE_GET_INSTANCE, {});
        Node *node = rw::host_ptr<Node>(n);
        const Data *value = rw::host_ptr<Data>(node->value.obj);
        rw::call(ARCHIVE_CONVERT, {rw::arg(out), archive, node->key.data(), value->bytes, value->length});
        uint32_t blob = out->obj;
        if (!blob) {
            rw::call(MEMORY_STREAM_DTOR, {stream});
            return;
        }
        const Data *packed = rw::host_ptr<Data>(blob);
        rw::call(MEMORY_STREAM_WRITE, {stream, packed->bytes, packed->length, 1});
        uint32_t ctrl = out->ctrl;
        if (ctrl) guest::release(ctrl);
    }
    rw::call(MEMORY_STREAM_MOVE, {rw::arg(out), stream});
    rw::call(MEMORY_STREAM_DTOR, {stream});
}
RW_FUNCTION(0x4F0E94, "SaveBin::serialize", SaveBin::serialize);

// Reads (key, data) records through bg::Archive until getKey returns nothing (success). Before each data record
// the stream is seeked back to the position after the previous record. A missing data record clears the store
// and fails.
uint32_t SaveBin::deserialize(SaveBin *self, const guest::SharedPtr *data) {
    guest::Frame f(0x68);
    // frame slots of the original (shared_ptrs)
    const uint32_t entry = f.addr(0x24);       // pair<const char*, shared_ptr<Data>>
    const uint32_t dataArg = f.addr(0x30);     // shared_ptr<IInputStream> argument of getData
    const uint32_t record = f.addr(0x38);      // getData result
    const uint32_t keyArg = f.addr(0x40);      // shared_ptr<IInputStream> argument of getKey
    const uint32_t keyResult = f.addr(0x48);   // getKey result
    const uint32_t input = f.addr(0x50);       // shared_ptr<MemoryInputStream>
    const uint32_t insertResult = f.addr(0x58);

    rw::call(MAP_CLEAR, {rw::arg(self)});
    rw::call(INPUT_STREAM_MAKE_SHARED, {input, rw::arg(data)});
    uint32_t stream = rw::load<uint32_t>(input);
    uint32_t streamCtrl = rw::load<uint32_t>(input + 4);
    const uint8_t *streamObj = rw::host_ptr<uint8_t>(stream);
    uint32_t pos = rw::vcall(streamObj, STREAM_TELL, {stream});

    uint32_t prevKeyCtrl = 0;
    uint32_t result;
    for (;;) {
        uint32_t archive = rw::call(ARCHIVE_GET_INSTANCE, {});
        rw::save<uint32_t>(keyArg, stream);
        rw::save<uint32_t>(keyArg + 4, streamCtrl);
        if (streamCtrl) guest::addRef(streamCtrl);
        rw::call(ARCHIVE_GET_KEY, {keyResult, archive, keyArg, 0});
        // key = std::move(getKey result); the previous key is released
        uint32_t key = rw::load<uint32_t>(keyResult);
        uint32_t keyCtrl = rw::load<uint32_t>(keyResult + 4);
        rw::save<uint32_t>(keyResult, 0);
        rw::save<uint32_t>(keyResult + 4, 0);
        if (prevKeyCtrl) {
            guest::release(prevKeyCtrl);
            uint32_t moved = rw::load<uint32_t>(keyResult + 4);
            if (moved) guest::release(moved);
        }
        uint32_t argCtrl = rw::load<uint32_t>(keyArg + 4);
        if (argCtrl) guest::release(argCtrl);
        if (!key) {                             // end of the records
            if (keyCtrl) guest::release(keyCtrl);
            result = 1;
            break;
        }

        rw::vcall(streamObj, STREAM_SEEK, {stream, pos, 0});
        archive = rw::call(ARCHIVE_GET_INSTANCE, {});
        rw::save<uint32_t>(dataArg, stream);
        rw::save<uint32_t>(dataArg + 4, streamCtrl);
        if (streamCtrl) guest::addRef(streamCtrl);
        rw::call(ARCHIVE_GET_DATA, {record, archive, dataArg, 0, 0});
        argCtrl = rw::load<uint32_t>(dataArg + 4);
        if (argCtrl) guest::release(argCtrl);

        bool ok;
        uint32_t blob = rw::load<uint32_t>(record);
        if (blob) {
            // insert(pair(key chars, record))
            rw::save<uint32_t>(entry, rw::host_ptr<Data>(key)->bytes);
            rw::save<uint32_t>(entry + 4, blob);
            uint32_t ctrl = rw::load<uint32_t>(record + 4);
            rw::save<uint32_t>(entry + 8, ctrl);
            if (ctrl) guest::addRef(ctrl);
            rw::call(MAP_INSERT_UNIQUE, {insertResult, rw::arg(self), entry});
            ctrl = rw::load<uint32_t>(entry + 8);
            if (ctrl) guest::release(ctrl);
            pos = rw::vcall(streamObj, STREAM_TELL, {stream});
            ok = true;
        } else {
            rw::call(MAP_CLEAR, {rw::arg(self)});
            ok = false;
        }
        uint32_t recordCtrl = rw::load<uint32_t>(record + 4);
        if (recordCtrl) guest::release(recordCtrl);
        prevKeyCtrl = keyCtrl;
        if (!ok) {
            if (keyCtrl) guest::release(keyCtrl);
            result = 0;
            break;
        }
    }
    if (streamCtrl) guest::release(streamCtrl);
    return result;
}
RW_FUNCTION(0x4F0FE8, "SaveBin::deserialize", SaveBin::deserialize);
