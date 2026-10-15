// bg (Square Enix framework): memory / file / asset streams, binary (de)serialisation helpers, the BGAD archive
// codec and the virtual file system front end.
#include "B2Common.hpp"

using namespace b2;

namespace bg {

// std::function helpers (libc++, instantiated in the game)
constexpr uint32_t FN_ALLOC_COPY = 0x491765;    // function<void*(unsigned)>::function(const function&)
constexpr uint32_t FN_DELETE_COPY = 0x491797;   // function<void(void*)>::function(const function&)
constexpr uint32_t FN_ALLOC_ASSIGN = 0x4917C9;  // function<void*(unsigned)>::operator=(const function&)
constexpr uint32_t FN_DELETE_ASSIGN = 0x491839; // function<void(void*)>::operator=(const function&)
constexpr uint32_t FN_ALLOC_CALL = 0x4918A9;    // function<void*(unsigned)>::operator()
constexpr uint32_t FN_DELETE_CALL = 0x4919ED;   // function<void(void*)>::operator()
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t SHARED_ADD = 0xC8DBB5, SHARED_RELEASE = 0xC8DBD1;

// Destroys a std::function at `fn` (16-byte buffer + callable pointer at +16): the callable's destroy() when it
// lives in the buffer, destroy_deallocate() when on the heap.
static void destroyFunction(uint32_t fn) {
    uint32_t f = ld32(fn + 0x10);
    if (f == fn) vcallo(f, 0x10, {f});
    else if (f != 0) vcallo(f, 0x14, {f});
}

// ============================================================================================ MemoryStream
// MemoryInputStream: vtable +0, data +4, size +8, position +0xC, deleter std::function<void(void*)> +0x10.
// MemoryStream (derives): allocator std::function<void*(unsigned)> +0x28, deleter +0x40, capacity +0x58.
struct MemoryStream {
    static uint32_t construct(uint32_t self, uint32_t capacity, uint32_t allocator, uint32_t deleter) {
        rw::call(0x8DC3C5, {self, 0, 0, deleter});  // MemoryInputStream::MemoryInputStream(nullptr, 0, deleter)
        st32(self, got(0xFD60D0) + 8);
        rw::call(FN_ALLOC_COPY, {self + 0x28, allocator});
        rw::call(FN_DELETE_COPY, {self + 0x40, deleter});
        if (ld32(self + 0x38) == 0 || ld32(self + 0x50) == 0) {
            rw::call(FN_ALLOC_ASSIGN, {self + 0x28, got(0xFD60D4)});   // = MemoryStream::_defaultAllocator
            rw::call(FN_DELETE_ASSIGN, {self + 0x40, got(0xFD60D8)});  // = MemoryStream::_defaultDeleter
        }
        st32(self + 4, rw::call(FN_ALLOC_CALL, {self + 0x28, capacity}));
        st32(self + 0x58, capacity);
        return self;
    }
    static uint32_t destruct(uint32_t self) {
        st32(self, got(0xFD60D0) + 8);
        if (ld32(self + 0x50) != 0) rw::call(FN_DELETE_CALL, {self + 0x40, ld32(self + 4)});
        destroyFunction(self + 0x40);
        destroyFunction(self + 0x28);
        rw::call(0x8DC425, {self});  // MemoryInputStream::~MemoryInputStream
        return self;
    }
    static uint32_t write(uint32_t self, uint32_t src, uint32_t size, uint32_t count) {
        uint32_t bytes = size * count;
        uint32_t need = ld32(self + 8) + bytes;
        uint32_t cap = ld32(self + 0x58);
        uint32_t buf;
        if (cap >= need) {
            buf = ld32(self + 4);
        } else {
            uint32_t grown = need << 1;
            if ((cap << 1) >= need) grown = cap << 1;
            st32(self + 0x58, grown);
            buf = rw::call(FN_ALLOC_CALL, {self + 0x28, grown});
            rw::call(MEMCPY, {buf, ld32(self + 4), ld32(self + 0xC)});
            rw::call(FN_DELETE_CALL, {self + 0x40, ld32(self + 4)});
            st32(self + 4, buf);
        }
        rw::call(MEMCPY, {buf + ld32(self + 0xC), src, bytes});
        uint32_t pos = ld32(self + 0xC) + bytes;
        st32(self + 0xC, pos);
        uint32_t sz = ld32(self + 8);
        st32(self + 8, sz < pos ? pos : sz);
        return count;
    }
    // shared_ptr<Data> move(): hands the buffer to a new bg::Data (sret in r0); returns 0 in r0
    static uint32_t move(uint32_t out, uint32_t self) {
        guest::Frame f(0xC);
        *f.at<uint32_t>(4) = ld32(self + 4);
        rw::call(0x491B79, {out, f.addr(4), self + 8, self + 0x40});  // make_shared<Data>(data, size, deleter)
        st32(self + 4, 0);
        st32(self + 8, 0);
        st32(self + 0xC, 0);
        destroyFunction(self + 0x40);
        st32(self + 0x50, 0);
        st32(self + 0x58, 0);
        return 0;
    }
    // bg::Data: vtable +0, data +4, size +8, deleter std::function +0x10
    static uint32_t dataDestruct(uint32_t self) {
        st32(self, got(0xFD60E0) + 8);
        if (ld32(self + 0x20) != 0) {
            rw::call(FN_DELETE_CALL, {self + 0x10, ld32(self + 4)});
            st32(self + 4, 0);
        }
        destroyFunction(self + 0x10);
        return self;
    }
};
RW_FUNCTION(0x491664, "bg::MemoryStream::MemoryStream", MemoryStream::construct);
RW_FUNCTION(0x491928, "bg::MemoryStream::~MemoryStream", MemoryStream::destruct);
RW_FUNCTION(0x491A78, "bg::MemoryStream::write", MemoryStream::write);
RW_FUNCTION(0x491AF4, "bg::MemoryStream::move", MemoryStream::move);
RW_FUNCTION(0x491C84, "bg::Data::~Data", MemoryStream::dataDestruct);

// ============================================================================================ ioutil
// Containers as a 32-bit count followed by the elements (written to a MemoryStream, read from an IInputStream).
struct IoUtil {
    static uint32_t writeCount(uint32_t stream, uint32_t count) {
        guest::Frame f(0x14);
        *f.at<uint32_t>(0xC) = count;
        return MemoryStream::write(stream, f.addr(0xC), 4, 1);
    }
    static uint32_t writeVector(uint32_t vec, uint32_t stream, uint32_t shift) {
        if (writeCount(stream, uint32_t(int32_t(ld32(vec + 4) - ld32(vec)) >> shift)) != 1) return 0;
        uint32_t p = ld32(vec), end = ld32(vec + 4);
        for (; p != end; p += 1u << shift)
            if (MemoryStream::write(stream, p, 1u << shift, 1) != 1) return 0;
        return 1;
    }
    static uint32_t writeVectorU64(uint32_t vec, uint32_t stream) { return writeVector(vec, stream, 3); }
    static uint32_t writeVectorInt(uint32_t vec, uint32_t stream) { return writeVector(vec, stream, 2); }
    static uint32_t writeMap(uint32_t map, uint32_t stream) {
        if (writeCount(stream, ld32(map + 0xC)) != 1) return 0;
        for (uint32_t node = ld32(map + 8); node != 0; node = ld32(node)) {
            if (MemoryStream::write(stream, node + 8, 4, 1) != 1) return 0;
            if (MemoryStream::write(stream, node + 0xC, 4, 1) != 1) return 0;
        }
        return 1;
    }

    // vector::clear() as compiled: end += ~(end - elem - begin) & -elem
    static void clearVector(uint32_t vec, uint32_t elem) {
        uint32_t begin = ld32(vec), end = ld32(vec + 4);
        if (end != begin) st32(vec + 4, end + (~(end - elem - begin) & ~(elem - 1)));
    }
    static uint32_t readFn(uint32_t stream) { return ld32(ld32(stream) + 8); }

    static uint32_t readVector(uint32_t vec, uint32_t stream, uint32_t elem, uint32_t pushSlow) {
        clearVector(vec, elem);
        guest::Frame f(0x24);
        const uint32_t countAt = elem == 8 ? 0x18u : 0x14u, valueAt = 0x10;
        if (rw::call(readFn(stream), {stream, f.addr(countAt), 4, 1}) != 1) return 0;
        uint32_t count = *f.at<uint32_t>(countAt);
        if (count == 0) return 1;
        for (uint32_t k = 0;;) {
            if (rw::call(readFn(stream), {stream, f.addr(valueAt), elem, 1}) != 1) {
                clearVector(vec, elem);
                return 0;
            }
            uint32_t end = ld32(vec + 4);
            if (end == ld32(vec + 8)) {
                rw::call(pushSlow, {vec, f.addr(valueAt)});  // __push_back_slow_path
            } else {
                uint32_t e = 0;
                if (end != 0) {
                    for (uint32_t w = 0; w < elem; w += 4) st32(end + w, *f.at<uint32_t>(valueAt + w));
                    e = ld32(vec + 4);
                }
                st32(vec + 4, e + elem);
            }
            if (++k >= *f.at<uint32_t>(countAt)) return 1;
        }
    }
    static uint32_t readVectorU64(uint32_t vec, uint32_t stream) { return readVector(vec, stream, 8, 0x4ECB6D); }
    static uint32_t readVectorInt(uint32_t vec, uint32_t stream) { return readVector(vec, stream, 4, 0x4D6CC1); }

    static uint32_t readMap(uint32_t map, uint32_t stream) {
        guest::Frame f(0x24);
        rw::call(0x4EE2A5, {map});  // __hash_table::clear
        if (rw::call(readFn(stream), {stream, f.addr(0x1C), 4, 1}) != 1) return 0;
        if (*f.at<uint32_t>(0x1C) == 0) return 1;
        for (uint32_t k = 0;;) {
            if (rw::call(readFn(stream), {stream, f.addr(0x18), 4, 1}) != 1 ||
                rw::call(readFn(stream), {stream, f.addr(0x14), 4, 1}) != 1) {
                rw::call(0x4EE2A5, {map});
                return 0;
            }
            uint32_t value = *f.at<uint32_t>(0x14);
            uint32_t slot = rw::call(0x4EAC69, {map, f.addr(0x18)});  // unordered_map::operator[]
            st32(slot, value);
            if (++k >= *f.at<uint32_t>(0x1C)) return 1;
        }
    }
};
RW_FUNCTION(0x4ED9A0, "bg::ioutil::_write<std::vector<unsigned long long>>", IoUtil::writeVectorU64);
RW_FUNCTION(0x4EDA40, "bg::ioutil::_write<std::unordered_map<unsigned,unsigned>>", IoUtil::writeMap);
RW_FUNCTION(0x4EDAE8, "bg::ioutil::_write<std::vector<int>>", IoUtil::writeVectorInt);
RW_FUNCTION(0x4EDD88, "bg::ioutil::_read<std::vector<unsigned long long>>", IoUtil::readVectorU64);
RW_FUNCTION(0x4EE2E8, "bg::ioutil::_read<std::unordered_map<unsigned,unsigned>>", IoUtil::readMap);
RW_FUNCTION(0x4EE3B0, "bg::ioutil::_read<std::vector<int>>", IoUtil::readVectorInt);

// ============================================================================================ input streams
struct Streams {
    // AssetInputStream: vtable +0, AAsset* +4
    static uint32_t assetDestruct(uint32_t self) {
        st32(self, got(0xFD8168) + 8);
        if (ld32(self + 4) != 0) rw::call(0x43D0E4, {ld32(self + 4)});  // AAsset_close
        return self;
    }
    static uint32_t assetRead(uint32_t self, uint32_t dst, uint32_t size, uint32_t count) {
        uint32_t remain = rw::call(0x43D0F0, {ld32(self + 4)});  // AAsset_getRemainingLength
        uint32_t avail = rw::call(0xCD5DF9, {remain, size});     // __udivsi3
        if (avail <= count) count = avail;
        int32_t n = int32_t(rw::call(0x43D0FC, {ld32(self + 4), dst, size * count}));  // AAsset_read
        return n >= 1 ? count : 0;
    }
    static uint32_t assetTell(uint32_t self) {
        uint32_t len = rw::call(0x43D108, {ld32(self + 4)});  // AAsset_getLength
        return len - rw::call(0x43D0F0, {ld32(self + 4)});
    }

    // FileInputStream: vtable +0, FILE* +4. Opens `name` as is when absolute, else through FileUtils (the full
    // path when `fullPath`, the writable path + name otherwise).
    static uint32_t fileConstruct(uint32_t self, uint32_t name, uint32_t fullPath) {
        guest::Frame f(0x54);
        const uint32_t path = f.addr(0x3C), tmp = f.addr(0x24), result = f.addr(0x30), writable = f.addr(0x18);
        st32(self, got(0xFD8170) + 8);
        for (uint32_t k = 0; k < 12; k += 4) *f.at<uint32_t>(0x3C + k) = 0;
        auto clearPath = [&]() {
            if (*f.at<uint8_t>(0x3C) & 1) {
                rw::store8(rw::host_ptr<uint8_t>(*f.at<uint32_t>(0x44)), 0);
                *f.at<uint32_t>(0x40) = 0;
            } else {
                *f.at<uint8_t>(0x3D) = 0;
                *f.at<uint8_t>(0x3C) = 0;
            }
            rw::call(0x48D705, {path, 0});  // std::string::reserve(0)
        };
        if (ld8(name) == '/') {
            uint32_t len = rw::call(STRLEN, {name});
            rw::call(0x48D811, {path, name, len});  // std::string::assign(const char*, size_t)
        } else {
            uint32_t fu = rw::call(0xA3F44D, {});  // cocos2d::FileUtils::getInstance
            if (fullPath == 1) {
                uint32_t fn = ld32(ld32(fu) + 0x20);  // FileUtils::fullPathForFilename
                uint32_t len = rw::call(STRLEN, {name});
                rw::call(0x47CD93, {tmp, name, len});  // std::string::__init
                rw::call(fn, {result, fu, tmp});
                clearPath();
                for (uint32_t k = 0; k < 12; k += 4) {
                    uint32_t w = *f.at<uint32_t>(0x30 + k);
                    *f.at<uint32_t>(0x30 + k) = 0;
                    *f.at<uint32_t>(0x3C + k) = w;
                }
                if (*f.at<uint8_t>(0x24) & 1) rw::call(0xC98F9B, {*f.at<uint32_t>(0x2C)});  // operator delete
            } else {
                vcallo(fu, 0x44, {writable, fu});  // FileUtils::getWritablePath
                uint32_t len = rw::call(STRLEN, {name});
                uint32_t s = rw::call(0x47AC89, {writable, name, len});  // std::string::append
                uint8_t b0 = ld8(s);
                rw::call(MEMCPY, {f.addr(0x48), s + 1, 7});
                uint32_t data = ld32(s + 8);
                st32(s, 0);
                st32(s + 4, 0);
                st32(s + 8, 0);
                clearPath();
                *f.at<uint8_t>(0x3C) = b0;
                rw::call(MEMCPY, {f.addr(0x3D), f.addr(0x48), 7});
                *f.at<uint32_t>(0x44) = data;
                *f.at<uint8_t>(0x4E) = 0;
                *f.at<uint16_t>(0x4C) = 0;
                *f.at<uint32_t>(0x48) = 0;
                if (*f.at<uint8_t>(0x18) & 1) rw::call(0xC98F9B, {*f.at<uint32_t>(0x20)});
            }
        }
        bool isLong = *f.at<uint8_t>(0x3C) & 1;
        uint32_t cstr = isLong ? *f.at<uint32_t>(0x44) : (path | 1);
        st32(self + 4, rw::call(0x414158, {cstr, 0xDFC3A6}));  // fopen(path, "rb")
        if (*f.at<uint8_t>(0x3C) & 1) rw::call(0xC98F9B, {*f.at<uint32_t>(0x44)});
        return self;
    }
    static uint32_t fileDestruct(uint32_t self) {
        st32(self, got(0xFD8170) + 8);
        if (ld32(self + 4) != 0) rw::call(0x414170, {ld32(self + 4)});  // fclose
        return self;
    }
    static uint32_t fileSize(uint32_t self) {
        uint32_t pos = vcallo(self, 0xC, {self});  // tell
        vcallo(self, 0x10, {self, 0, 2});         // seek(0, SEEK_END)
        uint32_t size = vcallo(self, 0xC, {self});
        vcallo(self, 0x10, {self, pos, 0});
        return size;
    }

    static uint32_t memoryConstruct(uint32_t self, uint32_t data, uint32_t size, uint32_t deleter) {
        st32(self, got(0xFD8174) + 8);
        st32(self + 4, data);
        st32(self + 8, size);
        st32(self + 0xC, 0);
        rw::call(FN_DELETE_COPY, {self + 0x10, deleter});
        return self;
    }
    static uint32_t memoryConstructData(uint32_t self, uint32_t dataPtr) {
        uint32_t d = ld32(dataPtr);
        uint32_t vt = got(0xFD8174) + 8;
        uint32_t data = ld32(d + 4), size = ld32(d + 8);
        st32(self + 0x20, 0);
        st32(self, vt);
        st32(self + 4, data);
        st32(self + 8, size);
        st32(self + 0xC, 0);
        return self;
    }
    static uint32_t memoryDestruct(uint32_t self) {
        guest::Frame f(0xC);
        st32(self, got(0xFD8174) + 8);
        uint32_t fn = ld32(self + 0x20);
        if (fn != 0) {
            *f.at<uint32_t>(4) = ld32(self + 4);
            vcallo(fn, 0x18, {fn, f.addr(4)});  // the deleter's operator()(void*&&)
        }
        destroyFunction(self + 0x10);
        return self;
    }
    static uint32_t memoryRead(uint32_t self, uint32_t dst, uint32_t size, uint32_t count) {
        uint32_t pos = ld32(self + 0xC);
        uint32_t avail = rw::call(0xCD5DF9, {ld32(self + 8) - pos, size});
        uint32_t src = ld32(self + 4) + pos;
        if (avail <= count) count = avail;
        uint32_t bytes = size * count;
        rw::call(MEMCPY, {dst, src, bytes});
        st32(self + 0xC, ld32(self + 0xC) + bytes);
        return count;
    }
    static int32_t memorySeek(uint32_t self, uint32_t offset, int32_t whence) {
        uint32_t pos = offset;
        if (whence != 0) {
            if (whence == 2) pos = ld32(self + 8) + offset;
            else if (whence == 1) pos = ld32(self + 0xC) + offset;
            else return -1;
        }
        if (int32_t(pos) < 0 || pos > ld32(self + 8)) return -1;
        st32(self + 0xC, pos);
        return 0;
    }

    // shared_ptr<IInputStream> InputStreamFactory::open(path, fromAssets) (sret; r0 = 0)
    static uint32_t factoryOpen(uint32_t out, uint32_t path, uint32_t assets) {
        guest::Frame f(0x24);
        *f.at<uint32_t>(0x1C) = path;
        *f.at<uint8_t>(0x1B) = uint8_t(assets);
        st32(out, 0);
        st32(out + 4, 0);
        uint32_t obj, ctrl;
        if (assets == 1) {
            rw::call(0x8DC7E1, {f.addr(0x10), f.addr(0x1C)});  // make_shared<AssetInputStream>(path)
            obj = *f.at<uint32_t>(0x10);
            ctrl = *f.at<uint32_t>(0x14);
        } else {
            rw::call(0x8DC6E9, {f.addr(8), f.addr(0x1C), f.addr(0x1B)});  // make_shared<FileInputStream>(path, b)
            ctrl = *f.at<uint32_t>(0xC);
            obj = *f.at<uint32_t>(8);
        }
        st32(out, obj);
        st32(out + 4, ctrl);
        if (vcallo(obj, 0x14, {obj}) == 0) {  // isOpen
            st32(out, 0);
            st32(out + 4, 0);
            if (ctrl != 0) rw::call(SHARED_RELEASE, {ctrl});
        }
        return 0;
    }
    static uint32_t factoryCreate(uint32_t out, uint32_t data, uint32_t size, uint32_t deleter) {
        guest::Frame f(0x1C);
        *f.at<uint32_t>(0x10) = size;
        *f.at<uint32_t>(0x14) = data;
        rw::call(0x8DC67D, {f.addr(8), f.addr(0x14), f.addr(0x10), deleter});  // make_shared<MemoryInputStream>
        uint32_t obj = *f.at<uint32_t>(8);
        if (vcallo(obj, 0x14, {obj}) != 0) {
            uint32_t ctrl = *f.at<uint32_t>(0xC);
            st32(out, obj);
            st32(out + 4, ctrl);
            if (ctrl != 0) {
                rw::call(SHARED_ADD, {ctrl});
                rw::call(SHARED_RELEASE, {ctrl});
            }
        } else {
            uint32_t ctrl = *f.at<uint32_t>(0xC);
            if (ctrl != 0) rw::call(SHARED_RELEASE, {ctrl});
            st32(out, 0);
            st32(out + 4, 0);
        }
        return 0;
    }
};
RW_FUNCTION(0x8DBFE8, "bg::AssetInputStream::~AssetInputStream", Streams::assetDestruct);
RW_FUNCTION(0x8DC044, "bg::AssetInputStream::read", Streams::assetRead);
RW_FUNCTION(0x8DC090, "bg::AssetInputStream::tell", Streams::assetTell);
RW_FUNCTION(0x8DC114, "bg::FileInputStream::FileInputStream", Streams::fileConstruct);
RW_FUNCTION(0x8DC2DC, "bg::FileInputStream::~FileInputStream", Streams::fileDestruct);
RW_FUNCTION(0x8DC374, "bg::FileInputStream::size", Streams::fileSize);
RW_FUNCTION(0x8DC3C4, "bg::MemoryInputStream::MemoryInputStream(const void*,unsigned,const function&)",
            Streams::memoryConstruct);
RW_FUNCTION(0x8DC3F8, "bg::MemoryInputStream::MemoryInputStream(const shared_ptr<Data>&)",
            Streams::memoryConstructData);
RW_FUNCTION(0x8DC424, "bg::MemoryInputStream::~MemoryInputStream", Streams::memoryDestruct);
RW_FUNCTION(0x8DC4B4, "bg::MemoryInputStream::read", Streams::memoryRead);
RW_FUNCTION(0x8DC4FE, "bg::MemoryInputStream::seek", Streams::memorySeek);
RW_FUNCTION(0x8DC554, "bg::InputStreamFactory::open", Streams::factoryOpen);
RW_FUNCTION(0x8DC5F0, "bg::InputStreamFactory::create", Streams::factoryCreate);

// ============================================================================================ Archive (BGAD)
// Archive singleton at 0x1006868: allocator std::function +0x00 (seen from an Archive* as +0x10 = its callable),
// deleter std::function +0x18 (callable +0x28).
// BGAD header (24 bytes): magic "BGAD", version u16 +4, key u16 +6, name offset u16 +8, name size u16 +0xA,
// encryption u16 +0xC, compression u16 +0xE, stored size u32 +0x10, original size u32 +0x14.
constexpr uint32_t BGAD_MAGIC = 0x44414742;

static uint32_t lcgNext(uint32_t s) { return s * 1664525u + 1013904223u; }
static void xorWords(uint32_t p, uint32_t words, uint32_t seed) {
    for (uint32_t k = 0; k < words; k++) {
        seed = lcgNext(seed);
        st32(p + 4 * k, ld32(p + 4 * k) ^ seed);
    }
}
static void xorBytes(uint32_t p, uint32_t n, uint32_t seed) {
    for (; n != 0; n--, p++) {
        seed = lcgNext(seed);
        st8(p, uint8_t(ld8(p) ^ seed));
    }
}

struct Archive {
    static uint32_t getInstance() {
        const uint32_t guard = 0x1006898, inst = 0x1006868;
        if (guest::staticInitBegin(guard)) {
            rw::call(FN_ALLOC_COPY, {inst, got(0xFD8180)});         // = Archive::_defaultAllocator
            rw::call(FN_DELETE_COPY, {inst + 0x18, got(0xFD8184)});  // = Archive::_defaultDeleter
            rw::call(guest::CXA_ATEXIT, {got(0xFD8188), inst, got(0xFD604C)});
            guest::staticInitDone(guard);
        }
        return inst;
    }
    static uint32_t allocate(uint32_t self, uint32_t sizeVar) {
        uint32_t fn = ld32(self + 0x10);
        return vcallo(fn, 0x18, {fn, sizeVar});
    }
    static void release(uint32_t self, uint32_t ptrVar) {
        uint32_t fn = ld32(self + 0x28);
        vcallo(fn, 0x18, {fn, ptrVar});
    }

    // shared_ptr<Data> getKey(stream, key*) (sret; r0 = 0): the archive's entry name, decrypted
    static uint32_t getKey(uint32_t out, uint32_t self, uint32_t streamPtr, uint32_t keyOut) {
        guest::Frame f(0x4C);
        const uint32_t hdr = f.addr(0x30);
        *f.at<uint32_t>(0x28) = 0;
        uint32_t s = ld32(streamPtr);
        uint32_t base = vcallo(s, 0xC, {s});  // tell
        s = ld32(streamPtr);
        uint32_t freeIt = 0;
        if (vcallo(s, 8, {s, hdr, 0x18, 1}) == 1 && *f.at<uint32_t>(0x30) == BGAD_MAGIC &&
            uint32_t(uint16_t(*f.at<uint16_t>(0x34) - 1)) <= 1) {
            uint32_t size = *f.at<uint16_t>(0x3A);
            *f.at<uint32_t>(0x2C) = (size + 4) & 0x1FFFC;
            uint32_t p = allocate(self, f.addr(0x2C));
            *f.at<uint32_t>(0x28) = p;
            if (p != 0) {
                s = ld32(streamPtr);
                vcallo(s, 0x10, {s, *f.at<uint16_t>(0x38) + base, 0});  // seek to the name
                s = ld32(streamPtr);
                if (vcallo(s, 8, {s, p, *f.at<uint16_t>(0x3A), 1}) == 1) {
                    uint32_t n = *f.at<uint16_t>(0x3A);
                    uint32_t seed = *f.at<uint32_t>(0x40);
                    if (*f.at<uint16_t>(0x34) == 1) xorBytes(p, n, seed);
                    else xorWords(p, (n + 3) >> 2, seed);
                    st8(p + *f.at<uint16_t>(0x3A), 0);
                    if (keyOut != 0) st16(keyOut, *f.at<uint16_t>(0x36));
                    s = ld32(streamPtr);
                    vcallo(s, 0x10, {s, *f.at<uint32_t>(0x40), 1});
                    *f.at<uint32_t>(0x24) = *f.at<uint16_t>(0x3A) + 1u;
                    rw::call(0x8DD0AD, {out, f.addr(0x28), f.addr(0x24), self + 0x18});  // make_shared<Data>
                    return 0;
                }
                freeIt = p;
            }
        }
        *f.at<uint32_t>(0x2C) = freeIt;
        release(self, f.addr(0x2C));
        st32(out, 0);
        st32(out + 4, 0);
        return 0;
    }

    // shared_ptr<Data> getData(stream, nullTerminate, key*) (sret; r0 = 0): the entry's payload, decrypted and
    // inflated
    static uint32_t getData(uint32_t out, uint32_t self, uint32_t streamPtr, uint32_t terminate, uint32_t keyOut) {
        guest::Frame f(0x4C);
        const uint32_t hdr = f.addr(0x30);
        *f.at<uint32_t>(0x24) = 0;
        uint32_t s = ld32(streamPtr);
        uint32_t base = vcallo(s, 0xC, {s});
        s = ld32(streamPtr);
        uint32_t first = 0, second = 0;  // freed through the deleter at the end
        bool fail = true;
        if (vcallo(s, 8, {s, hdr, 0x18, 1}) == 1 && *f.at<uint32_t>(0x30) == BGAD_MAGIC &&
            uint32_t(uint16_t(*f.at<uint16_t>(0x34) - 1)) <= 1) {
            uint32_t add = terminate != 0 ? 4 : 3;
            *f.at<uint32_t>(0x2C) = (add + *f.at<uint32_t>(0x40)) & ~3u;
            uint32_t p = allocate(self, f.addr(0x2C));
            if (p != 0) {
                second = p;
                uint32_t w = *f.at<uint32_t>(0x38);
                uint32_t off = (w & 0xFFFF) + base + (w >> 16);
                s = ld32(streamPtr);
                vcallo(s, 0x10, {s, off, 0});
                s = ld32(streamPtr);
                bool ok = vcallo(s, 8, {s, p, *f.at<uint32_t>(0x40), 1}) == 1;
                if (ok) {
                    uint32_t enc = *f.at<uint16_t>(0x3C);
                    uint32_t seed = *f.at<uint16_t>(0x3A);
                    if (enc == 2) xorWords(p, (*f.at<uint32_t>(0x40) + 3) >> 2, seed);
                    else if (enc == 1) xorBytes(p, *f.at<uint32_t>(0x40), seed);
                    else if (enc != 0) ok = false;
                }
                if (ok) {
                    uint32_t comp = *f.at<uint16_t>(0x3E);
                    if (comp - 1u < 2u) {
                        uint32_t usize = *f.at<uint32_t>(0x44);
                        *f.at<uint32_t>(0x28) = usize;
                        *f.at<uint32_t>(0x2C) = usize + terminate;
                        uint32_t q = allocate(self, f.addr(0x2C));
                        if (q != 0) {
                            if (rw::call(0xA4C4E5, {q, f.addr(0x28), p, *f.at<uint32_t>(0x40)}) == 0 &&  // uncompress
                                *f.at<uint32_t>(0x28) == *f.at<uint32_t>(0x44)) {
                                *f.at<uint32_t>(0x24) = q;
                                uint32_t len = *f.at<uint32_t>(0x28);
                                *f.at<uint32_t>(0x20) = len;
                                if (terminate == 1) st8(q + len, 0);
                                fail = false;
                            } else {
                                first = q;
                            }
                        }
                    } else if (comp == 0) {
                        *f.at<uint32_t>(0x24) = p;
                        second = 0;
                        uint32_t len = *f.at<uint32_t>(0x40);
                        *f.at<uint32_t>(0x20) = len;
                        if (terminate == 1) st8(p + len, 0);
                        fail = false;
                    }
                }
            }
        }
        *f.at<uint32_t>(0x2C) = first;
        release(self, f.addr(0x2C));
        *f.at<uint32_t>(0x2C) = second;
        release(self, f.addr(0x2C));
        if (fail) {
            st32(out, 0);
            st32(out + 4, 0);
        } else {
            if (keyOut != 0) st16(keyOut, *f.at<uint16_t>(0x36));
            rw::call(0x8DD041, {out, f.addr(0x24), f.addr(0x20), self + 0x18});  // make_shared<Data>
        }
        return 0;
    }

    // shared_ptr<Data> convert(name, data, size) (sret; r0 = 0): packs `data` as a BGAD entry (zlib level 1,
    // word-XOR encryption keyed by the name length / compressed size)
    static uint32_t convert(uint32_t out, uint32_t self, uint32_t name, uint32_t data, uint32_t size) {
        guest::Frame f(0x44);
        *f.at<uint32_t>(0x18) = 0;
        *f.at<uint32_t>(0x38) = 0;
        double bound_d;
        {
            uint64_t k = 0x3FF004189374BC6Aull;  // 1.001
            double factor;
            std::memcpy(&factor, &k, 8);
            bound_d = double(size) * factor + 12.0;
        }
        uint32_t bound = d2u(bound_d);
        uint32_t nameLen = rw::call(STRLEN, {name});
        *f.at<uint32_t>(0x30) = bound;
        *f.at<uint32_t>(0x3C) = (bound + 3) & ~3u;
        uint32_t buf = allocate(self, f.addr(0x3C));
        uint32_t nb = 0;
        bool fail = true;
        if (buf != 0) {
            *f.at<uint32_t>(0) = 1;  // level (5th argument)
            if (rw::call(0xA4C461, {buf, f.addr(0x30), data, size, 1}) == 0) {  // compress2
                uint32_t clen = *f.at<uint32_t>(0x30);
                *f.at<uint32_t>(0x18) = clen;
                xorWords(buf, (clen + 3) >> 2, nameLen);
                uint32_t r4 = nameLen + 3;
                *f.at<uint32_t>(0x3C) = r4 & ~3u;
                nb = allocate(self, f.addr(0x3C));
                if (nb != 0) {
                    rw::call(MEMCPY, {nb, name, nameLen});
                    xorWords(nb, r4 >> 2, *f.at<uint32_t>(0x30));
                    uint32_t total = *f.at<uint32_t>(0x30) + nameLen + 0x18;
                    *f.at<uint32_t>(0x34) = total;
                    *f.at<uint32_t>(0x3C) = (total + 3) & ~3u;
                    uint32_t ob = allocate(self, f.addr(0x3C));
                    if (ob != 0) {
                        *f.at<uint32_t>(0x38) = ob;
                        const uint8_t head[] = {'B', 'G', 'A', 'D', 2, 0, 0, 0, 0x18, 0};
                        for (uint32_t k = 0; k < 10; k++) st8(ob + k, head[k]);
                        st8(ob + 0xB, uint8_t(nameLen >> 8));
                        st8(ob + 0xA, uint8_t(nameLen));
                        st8(ob + 0xD, 0);
                        st8(ob + 0xC, 2);
                        st8(ob + 0xF, 0);
                        st8(ob + 0xE, 2);
                        uint32_t c = *f.at<uint32_t>(0x18);
                        for (uint32_t k = 0; k < 4; k++) st8(ob + 0x10 + k, uint8_t(c >> (8 * k)));
                        for (uint32_t k = 0; k < 4; k++) st8(ob + 0x14 + k, uint8_t(size >> (8 * k)));
                        rw::call(MEMCPY, {ob + 0x18, nb, nameLen});
                        rw::call(MEMCPY, {ob + nameLen + 0x18, buf, *f.at<uint32_t>(0x30)});
                        fail = false;
                    }
                }
            }
        }
        *f.at<uint32_t>(0x3C) = nb;
        release(self, f.addr(0x3C));
        *f.at<uint32_t>(0x3C) = buf;
        release(self, f.addr(0x3C));
        if (fail) {
            st32(out, 0);
            st32(out + 4, 0);
        } else {
            rw::call(0x8DD041, {out, f.addr(0x38), f.addr(0x34), self + 0x18});  // make_shared<Data>
        }
        return 0;
    }
};
RW_FUNCTION(0x8DC8F0, "bg::Archive::getInstance", Archive::getInstance);
RW_FUNCTION(0x8DC9CC, "bg::Archive::getKey", Archive::getKey);
RW_FUNCTION(0x8DCBA8, "bg::Archive::getData", Archive::getData);
RW_FUNCTION(0x8DCDFC, "bg::Archive::convert", Archive::convert);

// ============================================================================================ vfs
// The mounted archives: a std::list at 0x10068A4 (sentinel: prev, next; size at +8) of {prev, next, archive,
// control block}, guarded by the recursive mutex at 0x10068B0. An archive keeps its id at +4, its entry count at
// +0x4C, its entry index table at +0x58 and the name -> entry map at +0x68.
constexpr uint32_t VFS = 0x10068A0, VFS_END = VFS + 4, VFS_MUTEX = VFS + 0x10;
constexpr uint32_t VFS_INIT = 0x8DD1A5;     // sub_8DD1A4: the vfs statics on first use
constexpr uint32_t MUTEX_LOCK_ = 0xC8DF55;  // recursive_mutex::lock
constexpr uint32_t MUTEX_UNLOCK_ = 0xC8DF79;
constexpr uint32_t MAP_FIND = 0x8DE18D;     // unordered_map<const char*, int>::find
constexpr uint32_t VFS_UNMOUNT = 0x8DD7A5;  // sub_8DD7A4

struct Vfs {
    static uint32_t readData(uint32_t out, uint32_t name) {
        guest::Frame f(0x1C);
        rw::call(VFS_INIT, {});
        rw::call(MUTEX_LOCK_, {VFS_MUTEX});
        *f.at<int32_t>(0x10) = 0;  // entry index carried into the next archive (negative: relative)
        for (uint32_t node = ld32(VFS + 8); node != VFS_END; node = ld32(node + 4)) {
            uint32_t arc = ld32(node + 8);
            int32_t idx = *f.at<int32_t>(0x10);
            if (idx < 0) {
                int32_t r = int32_t(ld32(arc + 0x4C) + uint32_t(idx));
                *f.at<int32_t>(0x10) = r;
                if (r >= 0) {
                    rw::call(0x8DE235, {out, arc});  // sub_8DE234: the entry by index
                    rw::call(MUTEX_UNLOCK_, {VFS_MUTEX});
                    return 0;
                }
            } else {
                *f.at<uint32_t>(0x14) = name;
                if (rw::call(MAP_FIND, {arc + 0x68, f.addr(0x14)}) != 0) {
                    rw::call(0x8DE2DD, {f.addr(8), arc, name, f.addr(0x10)});  // sub_8DE2DC: the entry by name
                    uint32_t obj = *f.at<uint32_t>(8);
                    if (obj != 0) {
                        st32(out, obj);
                        st32(out + 4, *f.at<uint32_t>(0xC));
                        rw::call(MUTEX_UNLOCK_, {VFS_MUTEX});
                        return 0;
                    }
                    if (*f.at<uint32_t>(0xC) != 0) rw::call(SHARED_RELEASE, {*f.at<uint32_t>(0xC)});
                }
            }
        }
        st32(out, 0);
        st32(out + 4, 0);
        rw::call(MUTEX_UNLOCK_, {VFS_MUTEX});
        return 0;
    }
    static uint32_t exists(uint32_t name) {
        guest::Frame f(0x14);
        rw::call(VFS_INIT, {});
        for (uint32_t node = ld32(VFS + 8); node != VFS_END; node = ld32(node + 4)) {
            *f.at<uint32_t>(0xC) = name;
            if (rw::call(MAP_FIND, {ld32(node + 8) + 0x68, f.addr(0xC)}) != 0) return 1;
        }
        return 0;
    }
    static int32_t getID(uint32_t name) {
        guest::Frame f(0x1C);
        rw::call(VFS_INIT, {});
        int32_t idx = 0;
        for (uint32_t node = ld32(VFS + 8); node != VFS_END; node = ld32(node + 4)) {
            uint32_t arc = ld32(node + 8);
            if (idx < 0) {
                idx = int32_t(ld32(arc + 0x4C) + uint32_t(idx));
            } else {
                *f.at<uint32_t>(0x14) = name;
                if (rw::call(MAP_FIND, {arc + 0x68, f.addr(0x14)}) == 0) continue;
                *f.at<uint32_t>(0x10) = name;
                uint32_t it = rw::call(MAP_FIND, {arc + 0x68, f.addr(0x10)});
                idx = lds32(ld32(arc + 0x58) + ld32(it + 0xC) * 4);
            }
            if (idx >= 0) return int32_t((ld32(arc + 4) << 24) | uint32_t(idx));
        }
        return -1;
    }
    static uint32_t mount(uint32_t id, uint32_t path, uint32_t key) {
        rw::call(VFS_INIT, {});
        rw::call(MUTEX_LOCK_, {VFS_MUTEX});
        rw::call(VFS_UNMOUNT, {id});
        uint32_t block = guest::newObject(0x90);  // shared_ptr control block + archive
        uint32_t arc = 0x10;
        if (block != 0) {
            st32(block + 4, 0);
            st32(block + 8, 0);
            st32(block, 0xFCCD44);
            arc = block + 0x10;
            rw::call(0x8DE105, {arc, id, path, key});  // the archive's constructor
        }
        uint32_t ok = 0;
        if (rw::call(0x8DD831, {arc}) == 1) {  // open
            uint32_t n = guest::newObject(0x10);
            st32(n + 8, arc);
            st32(n + 0xC, block);
            if (block != 0) rw::call(SHARED_ADD, {block});
            uint32_t first = ld32(VFS + 8);
            st32(ld32(first) + 4, n);  // push_front
            st32(n, ld32(first));
            st32(first, n);
            st32(n + 4, first);
            st32(VFS + 0xC, ld32(VFS + 0xC) + 1);
            ok = 1;
        }
        if (block != 0) rw::call(SHARED_RELEASE, {block});
        rw::call(MUTEX_UNLOCK_, {VFS_MUTEX});
        return ok;
    }
    static uint32_t unmount(uint32_t id) {
        rw::call(VFS_INIT, {});
        return rw::call(VFS_UNMOUNT, {id});
    }
};
RW_FUNCTION(0x8DD46C, "bg::vfs::readData", Vfs::readData);
RW_FUNCTION(0x8DD564, "bg::vfs::exists", Vfs::exists);
RW_FUNCTION(0x8DD5F0, "bg::vfs::getID", Vfs::getID);
RW_FUNCTION(0x8DD6AC, "bg::vfs::mount", Vfs::mount);
RW_FUNCTION(0x8DD790, "bg::vfs::unmount", Vfs::unmount);

}  // namespace bg
