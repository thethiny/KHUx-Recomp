// ResourceDataManager: the downloaded resource archives. Resource 0 is the main archive ("misc.mp4" + the profile
// "misc.png" in the writable path's resource folder); every event resource id has its own pair "<id>.mp4" /
// "<id>.png". A profile is a bg::Archive holding the archive's md5 ("/" -> "md5", 32 bytes) and size ("size").
// mount() mounts the main archive as vfs slot 1 and the valid event archives (sorted by priority) from slot 2 on;
// check() / purge() drop archives whose size or profile does not match what AppState recorded.
// The download half (update / downloadEventResources -> startDownload -> a download thread per archive piece,
// finishDownload joins the pieces) uses threads, HTTP and file writes.
#include "core/Guest.hpp"
#include "scene/SceneCommon.hpp"

namespace resource_data_manager_w31 {

using scene::ld32;
using scene::ld8;
using scene::st32;
using scene::st8;

// The manager (a function-local static of getInstance at 0x10054C4).
struct ResourceDataManager {
    rw::Field<int32_t> error;            // +0x00 0 ok, 1 HTTP failure, 2 / 3 file errors
    rw::Field<uint32_t> totalSize;       // +0x04 bytes to download
    rw::Field<uint32_t> doneSize;        // +0x08 bytes downloaded
    rw::Field<uint8_t> downloading;      // +0x0C
    rw::Field<uint8_t> processing;       // +0x0D a download thread is running
    const uint8_t pad_0e[2];
    rw::Field<uint32_t> remaining;       // +0x10 archives left to download
    rw::Field<uint32_t> field_14;        // +0x14
    // +0x18 std::deque<shared_ptr<hole::network::ResourceInfo>> (map first/begin/end/cap, start, size)
    rw::Field<uint32_t> infoMapFirst;    // +0x18
    rw::Field<uint32_t> infoMapBegin;    // +0x1C
    rw::Field<uint32_t> infoMapEnd;      // +0x20
    rw::Field<uint32_t> infoMapCap;      // +0x24
    rw::Field<uint32_t> infoStart;       // +0x28
    rw::Field<uint32_t> infoSize;        // +0x2C
    // +0x30 std::deque<const void*>: the pieces (ResourceInfo items) of the archive being downloaded
    rw::Field<uint32_t> itemMapFirst;    // +0x30
    rw::Field<uint32_t> itemMapBegin;    // +0x34
    rw::Field<uint32_t> itemMapEnd;      // +0x38
    rw::Field<uint32_t> itemMapCap;      // +0x3C
    rw::Field<uint32_t> itemStart;       // +0x40
    rw::Field<uint32_t> itemSize;        // +0x44
    rw::Field<uint32_t> request;         // +0x48 cocos2d::network::HttpRequest* in flight
    rw::Field<uint32_t> pieceDone;       // +0x4C bytes of the current piece received
    rw::Field<uint32_t> pieceTotal;      // +0x50 its total
};
RW_LAYOUT(ResourceDataManager, infoMapFirst, 0x18);
RW_LAYOUT(ResourceDataManager, itemMapFirst, 0x30);
RW_LAYOUT(ResourceDataManager, request, 0x48);
RW_SIZE(ResourceDataManager, 0x54);

constexpr uint32_t INSTANCE = 0x10054C4;
constexpr uint32_t INSTANCE_GUARD = 0x10054C0;

// literals
constexpr uint32_t LIT_RESOURCE_DIR = 0xE07C17;   // the resource folder name (1 char)
constexpr uint32_t LIT_SLASH = 0xDF8C66;          // "/"
constexpr uint32_t LIT_PNG = 0xDFED65;            // ".png"
constexpr uint32_t LIT_MP4 = 0xE07C19;            // ".mp4"
constexpr uint32_t LIT_MISC_PNG = 0xE07C27;       // "misc.png"
constexpr uint32_t LIT_MISC_MP4 = 0xE07C1E;       // "misc.mp4"
constexpr uint32_t LIT_RB = 0xDFC3A6;             // "rb"
constexpr uint32_t LIT_MD5 = 0xE07BBD;            // "md5"
constexpr uint32_t LIT_SIZE = 0xE07C01;           // "size"
constexpr uint32_t LIT_RESOURCE = 0xE07BD9;       // "resource"
constexpr uint32_t LIT_MODE = 0xDFBB26;           // "mode"
constexpr uint32_t LIT_MIN_VERSION = 0xE07BE2;    // "minVersion"
constexpr uint32_t LIT_VERSIONS = 0xE07BED;       // "versions"
constexpr uint32_t LIT_RESOURCE_EV = 0xE07BF6;    // "resourceEv"
constexpr uint32_t LIT_RESOURCE_ID = 0xDFB267;    // "resourceId"

// functions
constexpr uint32_t STRING_APPEND = 0x47AC89;         // std::string::append(const char*, size_t) -> string&
constexpr uint32_t TO_STRING_UINT = 0xC95595;        // std::to_string(unsigned) (sret)
constexpr uint32_t FILEUTILS_GET_INSTANCE = 0xA3F44D;
constexpr uint32_t FILEUTILS_GET_WRITABLE_PATH = 0x44;   // vtable byte offset: getWritablePath() (sret)
constexpr uint32_t FOPEN = 0x414158, FSEEK = 0x4146D4, FTELL = 0x4146E0, FCLOSE = 0x414170;   // PLT
constexpr uint32_t REMOVE = 0x413C84;                // remove (PLT)
constexpr uint32_t STRCMP = 0x410234;                // strcmp (PLT)
constexpr uint32_t STRTOL = 0x4101A4;                // strtol (PLT)
constexpr uint32_t MEMSET = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t CXA_ATEXIT = 0x40DCD8;
constexpr uint32_t DTOR = 0x772945;                  // ResourceDataManager::~ResourceDataManager
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;
constexpr uint32_t APPSTATE_GET_RESOURCE_REVISION = 0x4EAB9B;
constexpr uint32_t APPSTATE_GET_RESOURCE_SIZE = 0x4EABA1;
constexpr uint32_t APPSTATE_GET_EVENT_RESOURCES = 0x4EACF5;   // const unordered_map<unsigned, unsigned>& (id -> size)
constexpr uint32_t APPSTATE_SET_EVENT_RESOURCE = 0x4EAC01;    // setEventResourceIDAndSize(unsigned, unsigned)
constexpr uint32_t APPSTATE_SET_RESOURCE_REVISION = 0x4EABEF; // setResourceRevisionAndSize(int, unsigned)
constexpr uint32_t UMAP_COPY = 0x772B2D;             // unordered_map<unsigned, unsigned>(const unordered_map&)
constexpr uint32_t UMAP_DTOR = 0x4B4EC9;             // ~__hash_table<unsigned, unsigned>
constexpr uint32_t INFO_DEQUE_DTOR = 0x772BE9;       // ~__deque_base<shared_ptr<ResourceInfo>>
constexpr uint32_t ITEM_DEQUE_DTOR = 0x772BBD;       // ~__deque_base<const void*>
constexpr uint32_t INFO_DEQUE_CLEAR = 0x776001;      // __deque_base<shared_ptr<ResourceInfo>>::clear
constexpr uint32_t INFO_DEQUE_PUSH_BACK = 0x773915;
constexpr uint32_t ITEM_DEQUE_CLEAR = 0x7756BD;      // __deque_base<const void*>::clear
constexpr uint32_t ITEM_DEQUE_PUSH_BACK = 0x774691;
constexpr uint32_t INPUT_STREAM_OPEN = 0x8DC555;     // bg::InputStreamFactory::open(const char*, bool) (sret)
constexpr uint32_t ARCHIVE_GET_INSTANCE = 0x8DC8F1;
constexpr uint32_t ARCHIVE_GET_KEY = 0x8DC9CD;       // getKey(shared_ptr<IInputStream>, unsigned short*) (sret)
constexpr uint32_t ARCHIVE_GET_DATA = 0x8DCBA9;      // getData(shared_ptr<IInputStream>, bool, unsigned short*) (sret)
constexpr uint32_t STREAM_TELL = 0x0C, STREAM_SEEK = 0x10;   // IInputStream vtable byte offsets
constexpr uint32_t VFS_MOUNT = 0x8DD6AD;             // bg::vfs::mount(unsigned slot, const char* data, const char* index)
constexpr uint32_t VFS_UNMOUNT = 0x8DD791;           // bg::vfs::unmount(unsigned slot)
constexpr uint32_t AUDIO_INIT_RESOURCE_ID_MAP = 0x5C227D;
constexpr uint32_t DIRECTOR_GET_INSTANCE = 0x8EE519;
constexpr uint32_t DIRECTOR_GET_TEXTURE_CACHE = 0x8EFE69;
constexpr uint32_t TEXTURE_CACHE_REMOVE_ALL = 0x9181FD;
constexpr uint32_t EV_RESOURCE_CREATE = 0x792D95;    // master::EvResource::create(int) (sret)
constexpr uint32_t SAFEBUFFER_READ = 0x45A269;
constexpr uint32_t DATE_INTERVAL_SINCE_NOW = 0x4922D5;
constexpr uint32_t TREE_BALANCE_AFTER_INSERT = 0x493F5B;
constexpr uint32_t ADD_SHARED = 0xC8DBB5, RELEASE_SHARED = 0xC8DBD1;

inline uint32_t self_(const ResourceDataManager *p) { return rw::guest_addr(p); }
inline void release(uint32_t ctrl) {
    if (ctrl) rw::call(RELEASE_SHARED, {ctrl});
}
inline void strFree(uint32_t s) {
    if (ld8(s) & 1) guest::deleteObject(ld32(s + 8));
}
// c_str() of a guest std::string (the original computes `s | 1` for the short form: s is even-aligned)
inline uint32_t cstr(uint32_t s) { return (ld8(s) & 1) ? ld32(s + 8) : (s | 1); }
// dst = std::move(src.append(text, n))
inline void appendMove(uint32_t dst, uint32_t src, uint32_t text, uint32_t n) {
    uint32_t r = rw::call(STRING_APPEND, {src, text, n});
    uint32_t w0 = ld32(r), w1 = ld32(r + 4), w2 = ld32(r + 8);
    st32(dst, w0);
    st32(dst + 4, w1);
    st32(dst + 8, w2);
    st32(r, 0);
    st32(r + 4, 0);
    st32(r + 8, 0);
}
// sub_776318: the folder of an event resource: <writable>/<dir>/<id>
uint32_t eventFolder(uint32_t out, uint32_t id) {
    guest::Frame f(0x40);
    uint32_t writable = f.addr(0x00), folder = f.addr(0x0C), slash = f.addr(0x18), number = f.addr(0x24);
    uint32_t fu = rw::call(FILEUTILS_GET_INSTANCE, {});
    rw::call(ld32(ld32(fu) + FILEUTILS_GET_WRITABLE_PATH), {writable, fu});
    appendMove(folder, writable, LIT_RESOURCE_DIR, 1);
    strFree(writable);
    appendMove(slash, folder, LIT_SLASH, 1);
    rw::call(TO_STRING_UINT, {number, id});
    uint32_t text, n;
    if (ld8(number) & 1) {
        n = ld32(number + 4);
        text = ld32(number + 8);
    } else {
        text = number | 1;
        n = ld8(number) >> 1;
    }
    appendMove(out, slash, text, n);
    strFree(number);
    strFree(slash);
    strFree(folder);
    return 0;
}

// the archive / profile path of resource `id` (0 = the main resource "misc")
inline uint32_t resourcePath(uint32_t out, uint32_t id, uint32_t ext, uint32_t miscName) {
    guest::Frame f(0x40);
    if (id) {
        uint32_t folder = f.addr(0);
        eventFolder(folder, id);
        appendMove(out, folder, ext, 4);
        strFree(folder);
    } else {
        uint32_t writable = f.addr(0x00), folder = f.addr(0x0C), slash = f.addr(0x18);
        uint32_t fu = rw::call(FILEUTILS_GET_INSTANCE, {});
        rw::call(ld32(ld32(fu) + FILEUTILS_GET_WRITABLE_PATH), {writable, fu});
        appendMove(folder, writable, LIT_RESOURCE_DIR, 1);
        strFree(writable);
        appendMove(slash, folder, LIT_SLASH, 1);
        appendMove(out, slash, miscName, 8);
        strFree(slash);
        strFree(folder);
    }
    return 0;
}
// sub_772CB8: the profile path ("<id>.png" / "misc.png")
uint32_t profilePath(uint32_t out, uint32_t id) { return resourcePath(out, id, LIT_PNG, LIT_MISC_PNG); }
// sub_772E4C: the archive path ("<id>.mp4" / "misc.mp4")
uint32_t archivePath(uint32_t out, uint32_t id) { return resourcePath(out, id, LIT_MP4, LIT_MISC_MP4); }

// ------------------------------------------------------------------------------------------- life cycle
uint32_t check(ResourceDataManager *self, uint32_t id, uint32_t size);

ResourceDataManager *construct(ResourceDataManager *self) {
    rw::save<uint16_t>(self_(self) + 0x0C, 0);
    self->error = 0;
    self->totalSize = 0;
    self->doneSize = 0;
    rw::call(MEMSET, {self_(self) + 0x10, 0x44, 0});
    uint32_t revision = rw::call(APPSTATE_GET_RESOURCE_REVISION, {rw::call(APPSTATE_GET_INSTANCE, {})});
    if (revision) {
        uint32_t size = rw::call(APPSTATE_GET_RESOURCE_SIZE, {rw::call(APPSTATE_GET_INSTANCE, {})});
        check(self, 0, size);
    }
    guest::Frame f(0x18);
    uint32_t events = rw::call(APPSTATE_GET_EVENT_RESOURCES, {rw::call(APPSTATE_GET_INSTANCE, {})});
    rw::call(UMAP_COPY, {f.addr(), events});
    for (uint32_t node = ld32(f.addr(8)); node; node = ld32(node)) check(self, ld32(node + 8), ld32(node + 12));
    rw::call(UMAP_DTOR, {f.addr()});
    return self;
}

ResourceDataManager *destruct(ResourceDataManager *self) {
    rw::call(ITEM_DEQUE_DTOR, {self_(self) + 0x30});
    rw::call(INFO_DEQUE_DTOR, {self_(self) + 0x18});
    return self;
}

// getInstance(): the function-local static, constructed on first use (destroyed at exit)
uint32_t getInstance() {
    if (!(rw::call(guest::ATOMIC_LOAD_GUARD, {INSTANCE_GUARD, 0, 0}) & 1) &&
        rw::call(guest::CXA_GUARD_ACQUIRE, {INSTANCE_GUARD})) {
        construct(rw::host_ptr<ResourceDataManager>(INSTANCE));
        rw::call(CXA_ATEXIT, {DTOR, INSTANCE, guest::DSO_HANDLE});
        guest::staticInitDone(INSTANCE_GUARD);
    }
    return INSTANCE;
}

ResourceDataManager *reset(ResourceDataManager *self) {
    self->totalSize = 0;
    self->doneSize = 0;
    self->remaining = 0;
    self->field_14 = 0;
    return self;
}

// ------------------------------------------------------------------------------------------- profiles
// save(id, revision, size): record what was installed (id 0 = the main resource's revision)
uint32_t save(uint32_t, uint32_t id, int32_t revision, uint32_t size) {
    uint32_t app = rw::call(APPSTATE_GET_INSTANCE, {});
    if (id) return rw::call(APPSTATE_SET_EVENT_RESOURCE, {app, id, size});
    return rw::call(APPSTATE_SET_RESOURCE_REVISION, {app, uint32_t(revision), size});
}
uint32_t invalidate(uint32_t self, uint32_t id) { return save(self, id, 0, 0); }

// a copy of a held shared_ptr<IInputStream> (the by-value argument of the Archive functions), at `copy`
inline void copyStream(uint32_t copy, uint32_t stream) {
    st32(copy, ld32(stream));
    st32(copy + 4, ld32(stream + 4));
    if (ld32(stream + 4)) rw::call(ADD_SHARED, {ld32(stream + 4)});
}
// Archive::getKey(stream) as a held shared_ptr at `out`; true when its name is `name`
inline bool keyIs(uint32_t out, uint32_t stream, uint32_t copy, uint32_t name) {
    uint32_t archive = rw::call(ARCHIVE_GET_INSTANCE, {});
    copyStream(copy, stream);
    rw::call(ARCHIVE_GET_KEY, {out, archive, copy, 0});
    release(ld32(copy + 4));
    uint32_t key = ld32(out);
    return key && rw::call(STRCMP, {ld32(key + 4), name}) == 0;
}
inline void getData(uint32_t out, uint32_t stream, uint32_t copy) {
    uint32_t archive = rw::call(ARCHIVE_GET_INSTANCE, {});
    copyStream(copy, stream);
    rw::call(ARCHIVE_GET_DATA, {out, archive, copy, 1, 0});
    release(ld32(copy + 4));
}

// readProfile(id, size*): the md5 of resource `id`'s profile (a 32-byte bg::Data), null when the profile is
// missing or malformed; *size (when given) = the recorded archive size
uint32_t readProfile(uint32_t out, uint32_t, uint32_t id, uint32_t sizeOut) {
    guest::Frame f(0x80);
    uint32_t path = f.addr(0x70);
    uint32_t stream = f.addr(0x68);      // shared_ptr<IInputStream>
    uint32_t rootKey = f.addr(0x60), md5Key = f.addr(0x50), md5 = f.addr(0x40), sizeKey = f.addr(0x30);
    uint32_t sizeData = f.addr(0x20), copy = f.addr(0x10), endptr = f.addr(0x08);
    profilePath(path, id);
    rw::call(INPUT_STREAM_OPEN, {stream, cstr(path), 0});
    uint32_t s = ld32(stream);
    if (keyIs(rootKey, stream, copy, LIT_SLASH)) {
        uint32_t pos = rw::call(ld32(ld32(s) + STREAM_TELL), {s});
        if (keyIs(md5Key, stream, copy, LIT_MD5)) {
            rw::call(ld32(ld32(s) + STREAM_SEEK), {s, pos, 0});
            getData(md5, stream, copy);
            uint32_t data = ld32(md5);
            if (data && ld32(data + 8) == 32) {
                if (sizeOut) {
                    uint32_t at = rw::call(ld32(ld32(s) + STREAM_TELL), {s});
                    if (keyIs(sizeKey, stream, copy, LIT_SIZE)) {
                        rw::call(ld32(ld32(s) + STREAM_SEEK), {s, at, 0});
                        getData(sizeData, stream, copy);
                        if (ld32(sizeData)) {
                            int32_t v = int32_t(rw::call(STRTOL, {ld32(ld32(sizeData) + 4), endptr, 10}));
                            if (v >= 0 && ld8(ld32(endptr)) == 0) st32(sizeOut, uint32_t(v));
                        }
                        release(ld32(sizeData + 4));
                    }
                    release(ld32(sizeKey + 4));
                }
                st32(out, ld32(md5));
                st32(out + 4, ld32(md5 + 4));
                st32(md5, 0);
                st32(md5 + 4, 0);
            } else {
                st32(out, 0);
                st32(out + 4, 0);
                release(ld32(md5 + 4));
            }
        } else {
            st32(out, 0);
            st32(out + 4, 0);
        }
        release(ld32(md5Key + 4));
    } else {
        st32(out, 0);
        st32(out + 4, 0);
    }
    release(ld32(rootKey + 4));
    release(ld32(stream + 4));
    strFree(path);
    return 0;
}

// validate(id, size): the profile exists and records `size` (or no size)
uint32_t validate(ResourceDataManager *self, uint32_t id, uint32_t size) {
    guest::Frame f(16);
    st32(f.addr(8), 0);
    readProfile(f.addr(), self_(self), id, f.addr(8));
    uint32_t ok = 0;
    if (ld32(f.addr())) {
        uint32_t recorded = ld32(f.addr(8));
        ok = (recorded == size ? 1u : 0u) | (recorded == 0 ? 1u : 0u);
    }
    release(ld32(f.addr(4)));
    return ok;
}

// the size of a file (0 when it cannot be opened)
inline uint32_t fileSize(uint32_t path) {
    uint32_t file = rw::call(FOPEN, {path, LIT_RB});
    uint32_t size = 0;
    if (file) {
        rw::call(FSEEK, {file, 0, 2});
        size = rw::call(FTELL, {file});
        rw::call(FCLOSE, {file});
    }
    return size;
}

// purge(id): delete the archive and its profile, forget the recorded size
uint32_t purge(ResourceDataManager *self, uint32_t id) {
    guest::Frame f(0x20);
    uint32_t profile = f.addr(0x10), archive = f.addr(0);
    profilePath(profile, id);
    if (ld8(profile) & 1) {
        rw::call(REMOVE, {ld32(profile + 8)});
        guest::deleteObject(ld32(profile + 8));
    } else {
        rw::call(REMOVE, {profile | 1});
    }
    archivePath(archive, id);
    if (ld8(archive) & 1) {
        rw::call(REMOVE, {ld32(archive + 8)});
        guest::deleteObject(ld32(archive + 8));
    } else {
        rw::call(REMOVE, {archive | 1});
    }
    save(self_(self), id, 0, 0);
    return 0;
}

// check(id, size): purge the resource unless its archive has the recorded size and a readable profile
uint32_t check(ResourceDataManager *self, uint32_t id, uint32_t size) {
    guest::Frame f(0x18);
    uint32_t path = f.addr(8);
    archivePath(path, id);
    uint32_t isLong = ld8(path) & 1;
    uint32_t actual = fileSize(cstr(path));
    bool ok = false;
    if (actual == size) {
        readProfile(f.addr(0), self_(self), id, 0);
        uint32_t md5 = ld32(f.addr(0));
        release(ld32(f.addr(4)));
        ok = md5 != 0;
    }
    if (!ok) purge(self, id);
    if (isLong) guest::deleteObject(ld32(path + 8));
    return 0;
}

// ------------------------------------------------------------------------------------------- mounting
uint32_t unmount(ResourceDataManager *) {
    uint32_t cache = rw::call(DIRECTOR_GET_TEXTURE_CACHE, {rw::call(DIRECTOR_GET_INSTANCE, {})});
    rw::call(TEXTURE_CACHE_REMOVE_ALL, {cache});
    uint32_t events = rw::call(APPSTATE_GET_EVENT_RESOURCES, {rw::call(APPSTATE_GET_INSTANCE, {})});
    uint32_t slot = 2;
    for (uint32_t node = ld32(events + 8); node; node = ld32(node)) rw::call(VFS_UNMOUNT, {slot++});
    rw::call(VFS_UNMOUNT, {1});
    return rw::call(AUDIO_INIT_RESOURCE_ID_MAP, {});
}

// sub_776488: std::set<shared_ptr<const master::EvResource>>::destroy(node)
void destroyEventTree(uint32_t tree, uint32_t node) {
    if (!node) return;
    destroyEventTree(tree, ld32(node));
    destroyEventTree(tree, ld32(node + 4));
    release(ld32(node + 0x14));
    guest::deleteObject(node);
}

// a 32-bit value at data position `index` of an EvResource row (a SafeBuffer)
inline uint32_t evValue(uint32_t row, uint32_t index, uint32_t scratch) {
    rw::call(SAFEBUFFER_READ, {row, scratch, ld32(row + 12) + index, 4, index});
    return ld32(scratch);
}

// mount(): the main archive in slot 1, then every event archive that is still open (priority order) from slot 2;
// expired or unknown event resources are purged. Returns the main archive's mount result.
uint32_t mount(ResourceDataManager *self) {
    uint32_t s = self_(self);
    unmount(self);
    guest::Frame f(0x80);
    uint32_t profile = f.addr(0x70), archive = f.addr(0x60);
    profilePath(profile, 0);
    uint32_t data = cstr(profile);
    archivePath(archive, 0);
    uint32_t mainResult = rw::call(VFS_MOUNT, {1, data, cstr(archive)});
    strFree(archive);
    strFree(profile);

    // std::set<shared_ptr<const EvResource>> ordered by the row's priority (data position 4)
    uint32_t set = f.addr(0x50);     // begin node, end node (root at +4), size
    uint32_t endNode = set + 4;
    st32(endNode, 0);
    st32(set + 8, 0);
    st32(set, endNode);
    uint32_t events = f.addr(0x38);   // unordered_map copy (20 bytes)
    uint32_t row = f.addr(0x30);      // shared_ptr<const EvResource>
    uint32_t date = f.addr(0x28);     // Date (8 bytes, flag bits in byte 7)
    uint32_t scratch = f.addr(0x20);
    rw::call(UMAP_COPY, {events, rw::call(APPSTATE_GET_EVENT_RESOURCES, {rw::call(APPSTATE_GET_INSTANCE, {})})});
    for (uint32_t node = ld32(events + 8); node; node = ld32(node)) {
        rw::call(EV_RESOURCE_CREATE, {row, ld32(node + 8)});
        uint32_t r = ld32(row);
        bool keep = false;
        if (r) {
            rw::call(SAFEBUFFER_READ, {r, scratch, ld32(r + 12) + 8, 4, 8});
            if (ld32(scratch)) {
                st8(date + 7, ld8(date + 7) & 0xFD);
                rw::call(SAFEBUFFER_READ, {ld32(row), date, ld32(ld32(row) + 12) + 12, 8, 12});
                if ((ld8(date + 7) & 2) && int32_t(rw::call(DATE_INTERVAL_SINCE_NOW, {date})) >= 0) keep = true;
            }
        }
        if (!keep) {
            purge(self, ld32(node + 8));
        } else {
            // find the leaf: equal priorities go right
            uint32_t parent = ld32(endNode);
            uint32_t slot;
            if (!parent) {
                parent = endNode;
                slot = endNode;
            } else {
                for (;;) {
                    uint32_t mine = evValue(ld32(row), 4, scratch + 4);
                    uint32_t theirs = evValue(ld32(parent + 0x10), 4, scratch);
                    if (int32_t(mine) < int32_t(theirs)) {
                        if (!ld32(parent)) {
                            slot = parent;
                            break;
                        }
                        parent = ld32(parent);
                    } else {
                        if (!ld32(parent + 4)) {
                            slot = parent + 4;
                            break;
                        }
                        parent = ld32(parent + 4);
                    }
                }
            }
            uint32_t leaf = guest::newObject(0x18);
            st32(leaf + 0x10, ld32(row));
            st32(leaf + 0x14, ld32(row + 4));
            if (ld32(row + 4)) rw::call(ADD_SHARED, {ld32(row + 4)});
            st32(leaf, 0);
            st32(leaf + 4, 0);
            st32(leaf + 8, parent);
            st32(slot, leaf);
            uint32_t first = ld32(ld32(set));
            if (first) {
                st32(set, first);
                leaf = ld32(slot);
            }
            rw::call(TREE_BALANCE_AFTER_INSERT, {ld32(endNode), leaf});
            st32(set + 8, ld32(set + 8) + 1);
        }
        release(ld32(row + 4));
    }
    uint32_t mountSlot = 1;
    for (uint32_t it = ld32(set); it != endNode;) {
        uint32_t ev = ld32(it + 0x10);
        rw::call(SAFEBUFFER_READ, {ev, scratch, ld32(ev + 12), 4, 0});
        uint32_t id = ld32(scratch);
        mountSlot++;
        uint32_t png = f.addr(0x10), mp4 = f.addr(0x00);
        profilePath(png, id);
        uint32_t pngLong = ld8(png) & 1;
        uint32_t pngData = ld32(png + 8);
        archivePath(mp4, id);
        rw::call(VFS_MOUNT, {mountSlot, pngLong ? pngData : (png | 1), cstr(mp4)});
        strFree(mp4);
        strFree(png);
        // in-order successor
        if (ld32(it + 4)) {
            it = ld32(it + 4);
            while (ld32(it)) it = ld32(it);
        } else {
            uint32_t child = it;
            for (;;) {
                it = ld32(child + 8);
                if (ld32(it) == child) break;
                child = it;
            }
        }
    }
    rw::call(AUDIO_INIT_RESOURCE_ID_MAP, {});
    rw::call(UMAP_DTOR, {events});
    destroyEventTree(set, ld32(endNode));
    return mainResult;
}

// remove(): unmount, then delete every event archive and the main one
uint32_t removeAll(ResourceDataManager *self) {
    unmount(self);
    guest::Frame f(0x18);
    rw::call(UMAP_COPY, {f.addr(), rw::call(APPSTATE_GET_EVENT_RESOURCES, {rw::call(APPSTATE_GET_INSTANCE, {})})});
    for (uint32_t node = ld32(f.addr(8)); node; node = ld32(node)) purge(self, ld32(node + 8));
    purge(self, 0);
    rw::call(UMAP_DTOR, {f.addr()});
    return 0;
}

// ------------------------------------------------------------------------------------------- state queries
uint32_t isDownloading(ResourceDataManager *self) {
    if (!self->downloading) return 0;
    return self->error == 0 ? 1 : 0;
}
uint32_t isProcessing(ResourceDataManager *self) { return self->processing; }
uint32_t getProgress(ResourceDataManager *self, uint32_t total) {
    if (total) st32(total, self->totalSize);
    return self->pieceTotal + self->doneSize;
}
uint32_t pushBackItem(ResourceDataManager *self, uint32_t item) {
    guest::Frame f(8);
    st32(f.addr(), item);
    rw::call(ITEM_DEQUE_PUSH_BACK, {self_(self) + 0x30, f.addr()});
    return 0;
}

}  // namespace resource_data_manager_w31

namespace rdm = resource_data_manager_w31;
RW_FUNCTION(0x7728AC, "ResourceDataManager::getInstance()", rdm::getInstance);
RW_FUNCTION(0x772944, "ResourceDataManager::~ResourceDataManager()", rdm::destruct);
RW_FUNCTION(0x772960, "ResourceDataManager::ResourceDataManager()", rdm::construct);
RW_FUNCTION(0x772A38, "ResourceDataManager::check(unsigned int, unsigned int) const", rdm::check);
// purge / remove delete files (remove()): a second run would see them gone
RW_FUNCTION_NOVERIFY(0x772C14, "ResourceDataManager::purge(unsigned int) const", rdm::purge);
RW_FUNCTION_NOVERIFY(0x7736DC, "ResourceDataManager::remove()", rdm::removeAll);
RW_FUNCTION(0x772CB8, "ResourceDataManager profile path (\"<id>.png\" / \"misc.png\")", rdm::profilePath);
RW_FUNCTION(0x772E4C, "ResourceDataManager archive path (\"<id>.mp4\" / \"misc.mp4\")", rdm::archivePath);
RW_FUNCTION(0x776318, "ResourceDataManager event resource folder (<writable>/<dir>/<id>)", rdm::eventFolder);
RW_FUNCTION(0x772FE0, "ResourceDataManager::invalidate(unsigned int) const", rdm::invalidate);
RW_FUNCTION(0x772FF0, "ResourceDataManager::readProfile(unsigned int, unsigned int*) const", rdm::readProfile);
RW_FUNCTION(0x773354, "ResourceDataManager::mount() const", rdm::mount);
RW_FUNCTION(0x7736A4, "ResourceDataManager::unmount() const", rdm::unmount);
RW_FUNCTION(0x77376C, "ResourceDataManager::reset()", rdm::reset);
RW_FUNCTION(0x77464C, "ResourceDataManager::pushBackItem(const void*)", rdm::pushBackItem);
RW_FUNCTION(0x7746F4, "ResourceDataManager::isDownloading() const", rdm::isDownloading);
RW_FUNCTION(0x774710, "ResourceDataManager::isProcessing() const", rdm::isProcessing);
RW_FUNCTION(0x774714, "ResourceDataManager::getProgress(unsigned int*) const", rdm::getProgress);
RW_FUNCTION(0x77490C, "ResourceDataManager::save(unsigned int, int, unsigned int) const", rdm::save);
RW_FUNCTION(0x774940, "ResourceDataManager::validate(unsigned int, unsigned int) const", rdm::validate);
RW_FUNCTION(0x776488, "std::set<shared_ptr<const master::EvResource>>::destroy (ResourceDataManager::mount)", rdm::destroyEventTree);
