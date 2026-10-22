// ResourceDataManager, the download half: the /system/resource answer (resource / resourceEv) becomes a queue of
// ResourceInfo; each one's data and index files are downloaded piece by piece (a worker thread per step, an
// HttpRequest per piece, CacheManager keeps the pieces), then joined into misc.mp4 / misc.png (or the event's
// files), checked and saved with the new revision.
//
// ResourceDataManager: +0 state (0 ok, 1 http error, 2 / 3 file errors), +4 bytes to download, +8 bytes done,
// +0xC downloading, +0xD started, +0x10 resources queued, +0x14 resources done, +0x18 deque<shared_ptr<
// ResourceInfo>> (map begin +0x1C, start +0x28, size +0x2C), +0x30 deque<const void*> of the pieces (map begin
// +0x34, map end +0x38, start +0x40, size +0x44), +0x48 the HttpRequest in flight.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t deque_info_clear = 0x776001, deque_info_push_back = 0x773915;
constexpr uint32_t deque_piece_clear = 0x7756BD, deque_piece_pop_front = 0x77453D;
constexpr uint32_t json_member = 0x4B1BB5;                 // rapidjson GenericValue::operator[](const char*)
constexpr uint32_t ResourceInfo_init = 0x772481, ResourceInfo_acceptForDataFile = 0x7727C5;
constexpr uint32_t ResourceInfo_acceptForIndexFile = 0x772839;
constexpr uint32_t RDM_startDownload = 0x773985, RDM_startNextDownload = 0x773C39, RDM_finishDownload = 0x773F0D;
constexpr uint32_t RDM_purge = 0x772C15, RDM_onRespond = 0x774571;
constexpr uint32_t CacheManager_getInstance = 0x4D7571, CacheManager_validate = 0x4D76F1;
constexpr uint32_t Ref_ctor = 0x8FD025, Ref_retain = 0x8FD0B5, Ref_release = 0x8FD0BD;
constexpr uint32_t aeabi_memclr = 0x40E7A0, string_assign = 0x48D811, libc_strlen = 0x40E08C;
constexpr uint32_t HttpCallback_assign = 0x49D419;        // function<void(HttpClient*, HttpResponse*)>::operator=
constexpr uint32_t HttpProgress_assign = 0x49D279;        // function<int(double, double, double, double)>::operator=
constexpr uint32_t HttpClient_getInstance = 0x7E3FCD, HttpClient_send = 0x7E4409;
constexpr uint32_t pthread_create_ = 0x410900, throw_system_error = 0xC97965;
constexpr uint32_t thread_detach = 0xC97ADD, thread_dtor = 0xC97A3D;
constexpr uint32_t THREAD_NEXT = 0x775659;                 // the worker: startNextDownloadThread
constexpr uint32_t THREAD_RESPONSE = 0x774A1D;             // the worker: the piece answered
constexpr uint32_t S_Resource = 0xE07BD9, S_Mode = 0xDFBB26, S_MinVersion = 0xE07BE2, S_Versions = 0xE07BED;
constexpr uint32_t S_ResourceEv = 0xE07BF6, S_ResourceId = 0xDFB267, S_ThreadFailed = 0xDFA2BA;
constexpr uint32_t GOT_ResourceInfo_ctrl_vtable = 0xFD6E08;   // __shared_ptr_emplace<ResourceInfo>
constexpr uint32_t GOT_HttpRequest_vtable = 0xFD6140;
constexpr uint32_t GOT_bind_vtable = 0xFD6E0C, GOT_onRespond = 0xFD6E10;   // std::bind(&onRespond, this, _1, _2)
constexpr uint32_t VT_Progress = 0xF6EC20;                 // the request's progress lambda {this}
constexpr uint32_t F_STATE = 0, F_TOTAL = 4, F_DONE = 8, F_BUSY = 0xC, F_STARTED = 0xD, F_QUEUED = 0x10;
constexpr uint32_t F_INFOS = 0x18, F_INFO_MAP = 0x1C, F_INFO_START = 0x28;
constexpr uint32_t F_PIECES = 0x30, F_PIECE_MAP = 0x34, F_PIECE_MAP_END = 0x38, F_PIECE_START = 0x40;
constexpr uint32_t F_PIECE_SIZE = 0x44, F_REQUEST = 0x48, F_PIECE_OK = 0x4C, F_PIECE_OK2 = 0x50;

constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_getResourceRevision = 0x4EAB9B;
constexpr uint32_t AppState_getResourceSize = 0x4EABA1;
constexpr uint32_t RDM_save = 0x77490D, RDM_validate = 0x774941, RDM_appendPieces = 0x774725;
constexpr uint32_t RDM_dataFilePath = 0x772E4D, RDM_indexFilePath = 0x772CB9, RDM_eventTempPath = 0x776319;
constexpr uint32_t deque_info_pop_front = 0x7749C1;
constexpr uint32_t CacheManager_convertURLToCacheFilePath = 0x4D7585, CacheManager_removeCache = 0x4D78C1;
constexpr uint32_t FileUtils_getInstance = 0xA3F44D, V_getWritablePath = 0x44, V_createDirectory = 0x54;
constexpr uint32_t string_append = 0x47AC89;               // std::string::append(const char*, size_t) -> *this
constexpr uint32_t libc_fopen = 0x414158, libc_fread = 0x414308, libc_fwrite = 0x414164, libc_fclose = 0x414170;
constexpr uint32_t libc_fseek = 0x4146D4, libc_ftell = 0x4146E0, libc_remove = 0x413C84, libc_rename = 0x4149A4;
constexpr uint32_t libc_truncate = 0x4242C8;
constexpr uint32_t S_rb = 0xDFC3A6, S_ab = 0xE07C14, S_r = 0xE07C17, S_Slash = 0xDF8C66, S_Wav = 0xE07C06;
constexpr uint32_t S_MiscWav = 0xE07C0B;
constexpr uint32_t F_RESOURCES_DONE = 0x14, F_INFO_SIZE = 0x2C;

// dst = std::move(src.append(text, n)) as the game inlines it: src's three words taken, src left empty
void appendMove(uint32_t dst, uint32_t src, uint32_t text, uint32_t n) {
    const uint32_t r = call(string_append, {src, text, n});
    for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(r + k));
    for (uint32_t k = 0; k < 12; k += 4) st32(r + k, 0);
}

// rapidjson value: type flags at +0xC (3 object, 4 array), number flags at byte +0xD (4 int, 8 uint)
uint32_t member(uint32_t v, uint32_t name) { return call(json_member, {v, name}); }

// the front of the ResourceInfo queue (a shared_ptr: object, control block)
uint32_t infoFront(uint32_t self) {
    const uint32_t start = ld32(self + F_INFO_START);
    return ld32(ld32(self + F_INFO_MAP) + ((start >> 7) & 0x1FFFFFC)) + 8 * (start & 0x1FF);
}

// a new ResourceInfo (make_shared: the control block, then the object at +0xC)
uint32_t newInfo(uint32_t &ctrl, uint32_t id, uint32_t mode, uint32_t minVersion) {
    ctrl = call(fn::operator_new, {0x28});
    if (!ctrl) return 0xC;
    st32(ctrl + 4, 0);
    st32(ctrl + 8, 0);
    st32(ctrl, ld32(GOT_ResourceInfo_ctrl_vtable) + 8);
    st32(ctrl + 0xC, id);
    st32(ctrl + 0x10, mode);
    st32(ctrl + 0x14, minVersion);
    st32(ctrl + 0x1C, 0);
    st32(ctrl + 0x20, 0);
    st32(ctrl + 0x24, 0);
    return ctrl + 0xC;
}

// the size of every piece queued (each piece's size at +0x18)
uint32_t queuedBytes(uint32_t self) {
    const uint32_t start = ld32(self + F_PIECE_START), map = ld32(self + F_PIECE_MAP);
    uint32_t block = map + ((start >> 8) & 0xFFFFFC);
    uint32_t it = 0, end = 0, sum = 0;
    if (ld32(self + F_PIECE_MAP_END) != map) {
        const uint32_t last = ld32(self + F_PIECE_SIZE) + start;
        end = ld32(map + ((last >> 8) & 0xFFFFFC)) + 4 * (last & 0x3FF);
        it = ld32(block) + 4 * (start & 0x3FF);
    }
    while (it != end) {
        sum += ld32(ld32(it) + 0x18);
        it += 4;
        if (it - ld32(block) == 0x1000) {
            it = ld32(block + 4);
            block += 4;
        }
    }
    return sum;
}

// a detached std::thread running fn(arg)
void startThread(uint32_t fn, uint32_t arg) {
    Frame f;
    const uint32_t thr = f.alloc(4);
    const uint32_t err = call(pthread_create_, {thr, 0, fn, arg});
    if (err) call(throw_system_error, {err, S_ThreadFailed});
    call(thread_detach, {thr});
    call(thread_dtor, {thr});
}
}  // namespace

namespace resource_download {

// /system/resource: {"resource": {"mode": 1|2, "minVersion": n, "versions": [...]}} queued and started
uint32_t update(uint32_t self, uint32_t doc) {
    call(deque_info_clear, {self + F_INFOS});
    if (ld32(doc + 0xC) != 3) return 0;
    const uint32_t res = member(doc, S_Resource);
    if (ld32(res + 0xC) != 3) return 0;
    const uint32_t m = member(res, S_Mode);
    if (!(ld8(m + 0xD) & 4)) return 0;
    const uint32_t mode = ld32(m);
    if (mode != 1 && mode != 2) return 0;
    const uint32_t mv = member(res, S_MinVersion);
    if (!(ld8(mv + 0xD) & 4)) return 0;
    const uint32_t minVersion = ld32(mv);
    const uint32_t versions = member(res, S_Versions);
    if (ld32(versions + 0xC) != 4) return 0;
    Frame f;
    const uint32_t sp = f.alloc(8);
    uint32_t ctrl;
    const uint32_t info = newInfo(ctrl, 0, mode, minVersion);
    st32(sp, info);
    st32(sp + 4, ctrl);
    if (!call(ResourceInfo_init, {info, versions})) {
        if (ctrl) call(fn::shared_release, {ctrl});
        return 0;
    }
    call(deque_info_push_back, {self + F_INFOS, sp});
    st32(self + F_QUEUED, 1);
    call(RDM_startDownload, {self});
    if (ctrl) call(fn::shared_release, {ctrl});
    return 1;
}
RW_FUNCTION(0x773778, "ResourceDataManager::update", resource_download::update);

// the front resource: its pieces listed (data file, then index file), their total size, an older revision
// purged first (mode 1), then the first piece
uint32_t startDownload(uint32_t self) {
    const uint32_t front = infoFront(self);
    const uint32_t info = ld32(front), ctrl = ld32(front + 4);
    if (ctrl) call(fn::shared_add, {ctrl});
    call(deque_piece_clear, {self + F_PIECES});
    call(ResourceInfo_acceptForDataFile, {info, self});
    call(ResourceInfo_acceptForIndexFile, {info, self});
    st32(self + F_TOTAL, queuedBytes(self));
    st32(self + F_DONE, 0);
    if (ld32(info + 4) == 1) call(RDM_purge, {self, ld32(info)});
    uint32_t r = call(RDM_startNextDownload, {self});
    if (ctrl) r = call(fn::shared_release, {ctrl});
    return r;
}
RW_FUNCTION(0x773984, "ResourceDataManager::startDownload", resource_download::startDownload);

// /event/...: {"resourceEv": [{"resourceId": id, "versions": [...]}, ...]} - all of them queued, or none started
uint32_t downloadEventResources(uint32_t self, uint32_t doc) {
    call(deque_info_clear, {self + F_INFOS});
    if (ld32(doc + 0xC) != 3) return 0;
    const uint32_t list = member(doc, S_ResourceEv);
    if (ld32(list + 0xC) != 4) return 0;
    const uint32_t n = ld32(list + 4);
    uint32_t k = 0;
    if (int32_t(n) >= 1) {
        for (;;) {
            const uint32_t e = ld32(list) + 16 * k;
            if (ld32(e + 0xC) != 3) break;
            const uint32_t id = member(e, S_ResourceId);
            if (!(ld8(id + 0xD) & 8)) break;
            const uint32_t versions = member(e, S_Versions);
            if (ld32(versions + 0xC) != 4) break;
            Frame f;
            const uint32_t sp = f.alloc(8);
            uint32_t ctrl;
            const uint32_t info = newInfo(ctrl, ld32(id), 1, 0);
            st32(sp, info);
            st32(sp + 4, ctrl);
            if (!call(ResourceInfo_init, {info, versions})) {
                if (ctrl) call(fn::shared_release, {ctrl});
                break;
            }
            call(deque_info_push_back, {self + F_INFOS, sp});
            if (ctrl) call(fn::shared_release, {ctrl});
            if (int32_t(++k) >= int32_t(n)) break;
        }
    }
    if (k != n) return 0;
    st32(self + F_QUEUED, n);
    call(RDM_startDownload, {self});
    return 1;
}
RW_FUNCTION(0x773A90, "ResourceDataManager::downloadEventResources", resource_download::downloadEventResources);

uint32_t startNextDownload(uint32_t self) {
    st32(self + F_STATE, 0);
    st8(self + F_STARTED, 1);
    st8(self + F_BUSY, 1);
    const uint32_t arg = call(fn::operator_new, {4});
    st32(arg, self);
    startThread(THREAD_NEXT, arg);
    return 0;
}
RW_FUNCTION(0x773C38, "ResourceDataManager::startNextDownload", resource_download::startNextDownload);

// (worker) pieces the cache already holds are counted as done; the next missing one is requested over HTTP;
// none left: finishDownload
uint32_t startNextDownloadThread(uint32_t self) {
    while (ld32(self + F_PIECE_SIZE)) {
        st32(self + F_PIECE_OK, 0);
        st32(self + F_PIECE_OK2, 0);
        const uint32_t start = ld32(self + F_PIECE_START);
        const uint32_t piece = ld32(ld32(ld32(self + F_PIECE_MAP) + ((start >> 8) & 0xFFFFFC)) + 4 * (start & 0x3FF));
        Frame f;
        const uint32_t out = f.alloc(8);
        call(CacheManager_validate, {out, call(CacheManager_getInstance, {}), piece, ld32(piece + 0x18), piece + 0xC});
        const uint32_t cached = ld32(out);
        if (ld32(out + 4)) call(fn::shared_release, {ld32(out + 4)});
        if (!cached) {
            // an HttpRequest (GET) for the piece's URL, answered by onRespond
            const uint32_t req = call(fn::operator_new, {0x88});
            call(Ref_ctor, {req});
            st32(req, ld32(GOT_HttpRequest_vtable) + 8);
            st32(req + 0x64, 0);
            st32(req + 0x68, 0);
            st32(req + 0x6C, 0);
            call(aeabi_memclr, {req + 0x18, 0x24});
            st8(req + 0x19, 0);
            st8(req + 0x18, 0);
            st8(req + 0x31, 0);
            st8(req + 0x30, 0);
            st32(req + 0x3C, 0);
            st32(req + 0x44, 0);
            st32(req + 0x40, 0);
            st32(req + 0x58, 0);
            st32(req + 0x60, 0);
            st32(req + 0x80, 0);
            st32(self + F_REQUEST, req);
            st32(req + 0x14, 0);
            const uint32_t url = strIsLong(piece) ? ld32(piece + 8) : piece + 1;
            call(string_assign, {req + 0x18, url, call(libc_strlen, {url})});
            {
                Frame g;
                const uint32_t fo = newFunction(g, ld32(GOT_bind_vtable) + 8, {ld32(GOT_onRespond), 0, self});
                call(HttpCallback_assign, {ld32(self + F_REQUEST) + 0x48, fo});
                functionDestroy(fo);
            }
            {
                Frame g;
                const uint32_t fo = newFunction(g, VT_Progress, {self});
                call(HttpProgress_assign, {ld32(self + F_REQUEST) + 0x70, fo});
                functionDestroy(fo);
            }
            call(HttpClient_send, {call(HttpClient_getInstance, {}), ld32(self + F_REQUEST)});
            return 0;
        }
        st32(self + F_DONE, ld32(self + F_DONE) + ld32(piece + 0x18));
        call(deque_piece_pop_front, {self + F_PIECES});
    }
    st8(self + F_BUSY, 0);
    call(RDM_finishDownload, {self});
    return 0;
}
RW_FUNCTION(0x773CF4, "ResourceDataManager::startNextDownloadThread", resource_download::startNextDownloadThread);

// the piece answered: the request released; a success is stored by a worker, a failure stops (state 1)
uint32_t onRespond(uint32_t self, uint32_t, uint32_t response) {
    if (ld32(self + F_REQUEST)) {
        call(Ref_release, {ld32(self + F_REQUEST)});
        st32(self + F_REQUEST, 0);
    }
    if (!ld8(response + 0x18)) {
        st32(self + F_STATE, 1);
        return 0;
    }
    call(Ref_retain, {response});
    const uint32_t arg = call(fn::operator_new, {8});
    st32(arg, response);
    st32(arg + 4, self);
    startThread(THREAD_RESPONSE, arg);
    return 0;
}
RW_FUNCTION(0x774570, "ResourceDataManager::onRespond", resource_download::onRespond);

// the pieces of `pieces` (a deque<const void*>) appended from the cache to the file `path`, each cache file removed
// after; *counter grows by each piece's size. 0 when a file does not open or a piece is short (files left open).
uint32_t appendPieces(uint32_t path, uint32_t pieces, uint32_t counter) {
    const uint32_t start = ld32(pieces + 0x10), map = ld32(pieces + 4);
    uint32_t block = map + ((start >> 8) & 0xFFFFFC);
    uint32_t it = 0, end = 0;
    if (ld32(pieces + 8) != map) {
        const uint32_t last = start + ld32(pieces + 0x14);
        end = ld32(map + ((last >> 8) & 0xFFFFFC)) + 4 * (last & 0x3FF);
        it = ld32(block) + 4 * (start & 0x3FF);
    }
    while (it != end) {
        const uint32_t piece = ld32(it);
        Frame f;
        const uint32_t cache = f.alloc(12);
        call(CacheManager_convertURLToCacheFilePath, {cache, call(CacheManager_getInstance, {}), piece});
        const uint32_t size = ld32(piece + 0x18);
        const uint32_t in = call(libc_fopen, {strData(cache), S_rb});
        const uint32_t out = call(libc_fopen, {path, S_ab});
        if (!in || !out) {
            strFree(cache);
            return 0;
        }
        const uint32_t buf = f.alloc(0x400);
        uint32_t written = 0;
        for (uint32_t n = call(libc_fread, {buf, 1, 0x400, in}); n; n = call(libc_fread, {buf, 1, 0x400, in}))
            if (call(libc_fwrite, {buf, 1, n, out}) == n) written += n;
        call(libc_fclose, {out});
        call(libc_fclose, {in});
        if (written != size) {
            strFree(cache);
            return 0;
        }
        call(libc_remove, {strData(cache)});
        if (counter) st32(counter, ld32(counter) + ld32(piece + 0x18));
        strFree(cache);
        it += 4;
        if (it - ld32(block) == 0x1000) {
            it = ld32(block + 4);
            block += 4;
        }
    }
    return 1;
}
RW_FUNCTION(0x774724, "ResourceDataManager: append cached pieces to a file (sub_774724)", resource_download::appendPieces);

// (worker) every piece of the front resource is cached: the data file is built (mode 1: anew; mode 2: appended
// to the size AppState saved, a failed append truncates it back), then the index file through a temporary file
// renamed into place, the whole checked and saved with the resource's revision. The cache pieces are removed and
// the next resource started. state: 2 data file damaged, 3 failed.
uint32_t finishDownload(uint32_t self) {
    const uint32_t revision = call(AppState_getResourceRevision, {call(AppState_getInstance, {})});
    const uint32_t length = call(AppState_getResourceSize, {call(AppState_getInstance, {})});
    const uint32_t front = infoFront(self);
    const uint32_t info = ld32(front), ctrl = ld32(front + 4);
    if (ctrl) call(fn::shared_add, {ctrl});
    const uint32_t id = ld32(info);
    call(RDM_save, {info, id, 0, 0});
    Frame f;
    {
        // the writable directory's "r" folder created
        const uint32_t fu = call(FileUtils_getInstance, {});
        const uint32_t create = ld32(ld32(fu) + V_createDirectory);
        const uint32_t fu2 = call(FileUtils_getInstance, {});
        const uint32_t w = f.alloc(12), dir = f.alloc(12);
        call(ld32(ld32(fu2) + V_getWritablePath), {w, fu2});
        appendMove(dir, w, S_r, 1);
        strFree(w);
        call(create, {fu, dir, 0});
        strFree(dir);
    }
    const uint32_t data = f.alloc(12);
    call(RDM_dataFilePath, {data, id});
    const uint32_t pieces = self + F_PIECES, done = self + F_DONE;
    call(deque_piece_clear, {pieces});
    call(ResourceInfo_acceptForDataFile, {info, self});
    uint32_t total = queuedBytes(self);
    st32(self + F_DONE, 0);
    bool failed;
    if (ld32(info + 4) == 1) {
        call(libc_remove, {strData(data)});
        failed = !call(RDM_appendPieces, {strData(data), pieces, done});
        if (failed) st32(self + F_STATE, 3);
    } else {
        uint32_t size = 0;
        const uint32_t fp = call(libc_fopen, {strData(data), S_rb});
        if (fp) {
            call(libc_fseek, {fp, 0, 2});
            size = call(libc_ftell, {fp});
            call(libc_fclose, {fp});
        }
        if (size != length) {
            st32(self + F_STATE, 3);
            failed = true;
        } else if (call(RDM_appendPieces, {strData(data), pieces, done})) {
            total += length;
            failed = false;
        } else {
            st32(self + F_STATE, 2);
            const uint32_t t = call(libc_truncate, {strData(data), length});
            if (t) st32(self + F_STATE, 3);
            failed = true;
            call(RDM_save, {t ? 3u : 0u, id, revision, length});
        }
    }
    strFree(data);
    if (!failed) {
        const uint32_t index = f.alloc(12), temp = f.alloc(12);
        call(RDM_indexFilePath, {index, id});
        if (id) {
            const uint32_t base = f.alloc(12);
            call(RDM_eventTempPath, {base, id});
            appendMove(temp, base, S_Wav, 4);
            strFree(base);
        } else {
            const uint32_t w = f.alloc(12), a = f.alloc(12), b = f.alloc(12);
            const uint32_t fu = call(FileUtils_getInstance, {});
            call(ld32(ld32(fu) + V_getWritablePath), {w, fu});
            appendMove(a, w, S_r, 1);
            strFree(w);
            appendMove(b, a, S_Slash, 1);
            appendMove(temp, b, S_MiscWav, 8);
            strFree(b);
            strFree(a);
        }
        call(libc_remove, {strData(temp)});
        call(deque_piece_clear, {pieces});
        call(ResourceInfo_acceptForIndexFile, {info, self});
        const bool indexFailed = call(RDM_appendPieces, {strData(temp), pieces, done}) != 1 ||
                                 call(libc_rename, {strData(temp), strData(index)}) != 0;
        if (indexFailed) {
            st32(self + F_STATE, 3);
            call(libc_remove, {strData(temp)});
        }
        strFree(temp);
        const uint32_t indexByte = ld8(index);
        strFree(index);
        if (!indexFailed) {
            if (call(RDM_validate, {indexByte, id, total})) call(RDM_save, {info, id, ld32(info + 0xC), total});
            else st32(self + F_STATE, 3);
        }
    }
    call(deque_piece_clear, {pieces});
    if (ld32(self + F_STATE) != 2) {
        call(ResourceInfo_acceptForDataFile, {info, self});
        call(ResourceInfo_acceptForIndexFile, {info, self});
        const uint32_t start = ld32(self + F_PIECE_START), map = ld32(self + F_PIECE_MAP);
        uint32_t block = map + ((start >> 8) & 0xFFFFFC);
        uint32_t it = 0, end = 0;
        if (ld32(self + F_PIECE_MAP_END) != map) {
            const uint32_t last = start + ld32(self + F_PIECE_SIZE);
            end = ld32(map + ((last >> 8) & 0xFFFFFC)) + 4 * (last & 0x3FF);
            it = ld32(block) + 4 * (start & 0x3FF);
        }
        while (it != end) {
            const uint32_t piece = ld32(it);
            call(CacheManager_removeCache, {call(CacheManager_getInstance, {}), piece});
            it += 4;
            if (it - ld32(block) == 0x1000) {
                it = ld32(block + 4);
                block += 4;
            }
        }
        call(deque_piece_clear, {pieces});
        st32(self + F_RESOURCES_DONE, ld32(self + F_RESOURCES_DONE) + 1);
        if (!ld32(self + F_STATE)) {
            call(deque_info_pop_front, {self + F_INFOS});
            if (ld32(self + F_INFO_SIZE)) call(RDM_startDownload, {self});
        }
    }
    if (ctrl) call(fn::shared_release, {ctrl});
    return 0;
}
RW_FUNCTION(0x773F0C, "ResourceDataManager::finishDownload", resource_download::finishDownload);

}  // namespace resource_download
