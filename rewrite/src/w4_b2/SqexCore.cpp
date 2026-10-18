// SQEX sound driver: Ogg Vorbis transfer, categories, driver config, core system / core sources (the mixer side).
#include "Sqex.hpp"

using namespace b2;

namespace sqex {

constexpr uint32_t OBJECT_VTABLE_GOT = 0xFD9250;  // SQEX::Object vtable (base of every SQEX object)

static void setObjectVtable(uint32_t self) { st32(self, got(OBJECT_VTABLE_GOT) + 8); }

// ============================================================================================ small objects
struct Nodes {
    // List<T>::Node / OnMemorySound deleting destructors: back to SQEX::Object, then Memory::Free
    static uint32_t deleteObject(uint32_t self) {
        setObjectVtable(self);
        free_(self);
        return self;
    }
    static uint32_t onMemorySoundConstruct(uint32_t self) {
        rw::call(0xCDD880, {self});  // SoundImpl::SoundImpl
        st32(self + 0xFC, 0);
        st32(self, got(0xFD9448) + 8);
        return self;
    }
    // Delegates: the delegate thread (entry sub_CF412C) and an empty table of 16 entries
    static int32_t delegateInitialize() {
        guest::Frame f(8);
        *f.at<uint32_t>(0) = 0xCF412C;  // CreateParam: entry, argument
        *f.at<uint32_t>(4) = 0;
        const uint32_t base = 0x100EB7C;
        st8(base, 0);
        rw::call(0xCE2398, {base + 0x54, f.addr()});  // Thread::Create
        for (uint32_t a = base + 0x10; a <= base + 0x4C; a += 4) st32(a, 0);
        return 0;
    }
    static uint32_t mutexConstruct(uint32_t self) {
        guest::Frame f(8);
        uint32_t attr = f.addr(4);
        rw::call(0x44D098, {attr});        // pthread_mutexattr_init
        rw::call(0x44D0A4, {attr, 1});     // pthread_mutexattr_settype(RECURSIVE)
        rw::call(0x4466F0, {self, attr});  // pthread_mutex_init
        rw::call(0x44D0B0, {attr});        // pthread_mutexattr_destroy
        return self;
    }
};
RW_FUNCTION(0xCE6444, "SQEX::List<SQEX::CoreSource*>::Node::~Node", Nodes::deleteObject);
RW_FUNCTION(0xCE942C, "SQEX::List<Sound>::Node::~Node", Nodes::deleteObject);
RW_FUNCTION(0xCF4528, "SQEX::OnMemorySound::~OnMemorySound", Nodes::deleteObject);
RW_FUNCTION(0xCF456C, "SQEX::OnMemorySound::OnMemorySound", Nodes::onMemorySoundConstruct);
RW_FUNCTION(0xCF41FC, "SQEX::DelegateManager::Initialize", Nodes::delegateInitialize);

// sub_CF412C: the delegate thread - until the stop flag (base + 0): pop the last queued delegate (count + 0xC,
// entries + 0x10) as the current one (+8) and run it (vtable +8), else wait on the condition (+0x50)
static void delegateThread(uint32_t /*argument*/) {
    const uint32_t base = 0x100EB7C, mutex = base + 4, cond = base + 0x50;
    while (!ld8(base)) {
        mutexLock(mutex);
        const int32_t n = lds32(base + 0xC);
        st32(base + 8, 0);
        if (n > 0) {
            st32(base + 0xC, uint32_t(n - 1));
            st32(base + 8, ld32(base + 0x10 + uint32_t(n - 1) * 4));
        }
        mutexUnlock(mutex);
        if (const uint32_t delegate = ld32(base + 8)) {
            vcallo(delegate, 8, {delegate});
            mutexLock(mutex);
            st32(base + 8, 0);
            mutexUnlock(mutex);
        } else {
            mutexLock(mutex);
            rw::call(0x4466CC, {cond, mutex});  // pthread_cond_wait
            mutexUnlock(mutex);
        }
    }
}
RW_FUNCTION(0xCF412C, "SQEX::DelegateManager (sub_CF412C): the delegate thread", delegateThread);
RW_FUNCTION(0xCE6908, "SQEX::Mutex::Mutex", Nodes::mutexConstruct);

// ============================================================================================ TransferOggVorbis
// vtable +0, finished +4 (byte), material +8, read position +0xC, buffers +0x10/+0x14, buffer size +0x18,
// current buffer +0x1C, OggVorbis_File +0x20 (0x298 bytes)
struct TransferOggVorbis {
    static uint32_t construct(uint32_t self, uint32_t material) {
        st32(self + 8, material);
        st8(self + 4, 0);
        st32(self + 0xC, 0);
        st32(self, got(0xFD92BC) + 8);
        uint32_t channels = rw::call(0xCDC9CC, {material});  // akbMaterialGetChannel
        st32(self + 0x1C, 0);
        uint32_t size = channels << 12;
        st32(self + 0x18, size);
        rw::call(0x442310, {self + 0x20, 0, 0x298});  // memset
        uint32_t b0 = malloc_(size, 4);
        st32(self + 0x10, b0);
        if (b0 == 0) log(5, 0xEEDAB4);  // "create transfer vorbis buffer failed! heap memory insufficient"
        uint32_t b1 = malloc_(ld32(self + 0x18), 4);
        st32(self + 0x14, b1);
        if (b1 == 0) log(5, 0xEEDAB4);
        return self;
    }
    static uint32_t destruct(uint32_t self) {
        st32(self, got(0xFD92BC) + 8);
        rw::call(0xCEEDB4, {self + 0x20});  // ov_clear
        free_(ld32(self + 0x10));
        free_(ld32(self + 0x14));
        setObjectVtable(self);
        return self;
    }
    // ov_pcm_seek(vf, pos)
    static void pcmSeek(uint32_t self, int64_t pos) {
        rw::call(0xCF0BC0, {self + 0x20, 0, uint32_t(uint64_t(pos)), uint32_t(uint64_t(pos) >> 32)});
    }
    static int32_t initialize(uint32_t self, uint32_t seekMs) {
        // ov_open_callbacks(this, &vf, NULL, 0, callbacks): the callbacks struct (4 words at 0x1002BE8) by value
        uint32_t cb0 = ld32(0x1002BE8), cb1 = ld32(0x1002BEC), cb2 = ld32(0x1002BF0), cb3 = ld32(0x1002BF4);
        if (int32_t(rw::call(0xCEFBAC, {self, self + 0x20, 0, 0, cb0, cb1, cb2, cb3})) < 0) {
            log(5, 0xEED86C);  // "ov_open_callbacks failed!!"
            return 2;
        }
        if (seekMs == 0) return 0;
        uint32_t rate = rw::call(0xCDC9E8, {ld32(self + 8)});  // akbMaterialGetSampleRate
        uint64_t prod = uint64_t(seekMs) * uint64_t(rate);
        int64_t pos = int64_t(rw::call64(0xCD72F8, {uint32_t(prod), uint32_t(prod >> 32), 1000, 0}));  // ldivmod
        if (rw::call(0xCDCA04, {ld32(self + 8)}) == 0) {  // akbMaterialIsLoopAudio
            uint32_t n = rw::call(0xCDCAAC, {ld32(self + 8)});  // akbMaterialGetNumSamples
            if (pos >= int64_t(uint64_t(n))) return 3;
        } else {
            uint32_t end = rw::call(0xCDCB8C, {ld32(self + 8)});  // akbMaterialGetLoopEnd
            if (pos >= int64_t(uint64_t(end))) {
                uint32_t start = rw::call(0xCDCB70, {ld32(self + 8)});  // akbMaterialGetLoopStart
                uint32_t end2 = rw::call(0xCDCB8C, {ld32(self + 8)});
                int64_t q = int64_t(rw::call64(0xCD72F8, {uint32_t(uint64_t(pos)), uint32_t(uint64_t(pos) >> 32), end2, 0}));
                int64_t rem = int64_t(uint64_t(pos) - uint64_t(q) * uint64_t(end2));
                pos = int64_t(uint64_t(rem) + uint64_t(start));
            }
        }
        pcmSeek(self, pos);
        return 0;
    }
    static int32_t execute(uint32_t self, uint32_t param) {
        int32_t want = lds32(self + 0x18);
        uint32_t vf = self + 0x20;
        if (rw::call(0xCDCA04, {ld32(self + 8)}) == 1) {
            // do not read past the loop end
            uint32_t end = rw::call(0xCDCB8C, {ld32(self + 8)});
            uint32_t tell = rw::call(0xCF11B8, {vf});  // ov_pcm_tell (low word)
            int32_t ch = int16_t(uint16_t(rw::call(0xCDC9CC, {ld32(self + 8)})));
            uint32_t frame = uint32_t(ch) << 1;
            int32_t limit = int32_t((end - tell) * frame + frame);
            if (want >= limit) want = limit;
        }
        guest::Frame f(0xC);
        uint32_t cur = ld32(self + 0x1C);
        int32_t n = int32_t(rw::call(0xCF1394, {vf, ld32(self + 0x10 + cur * 4), uint32_t(want), f.addr(4)}));  // ov_read
        if (n == 0) {
            if (rw::call(0xCDCA04, {ld32(self + 8)}) != 0) return 0;
            st8(self + 4, 1);
            return 1;
        }
        if (n < 0) return 2;
        cur = ld32(self + 0x1C);
        uint32_t buf = ld32(self + 0x10 + cur * 4);
        st32(param + 4, uint32_t(n));
        st32(param, buf);
        st32(self + 0x1C, uint32_t(int32_t(cur + 1) % 2));
        if (rw::call(0xCDCA04, {ld32(self + 8)}) == 1) {
            uint64_t t = rw::call64(0xCF11B8, {vf});
            uint32_t end = rw::call(0xCDCB8C, {ld32(self + 8)}) + 1;
            if (int64_t(t) >= int64_t(uint64_t(end))) {
                uint32_t start = rw::call(0xCDCB70, {ld32(self + 8)});
                rw::call(0xCF0BC0, {vf, uint32_t(t >> 32), start, 0});  // ov_pcm_seek(vf, loopStart)
            }
        }
        return 0;
    }
    static int32_t seekCallback(uint32_t self, int64_t offset, int32_t whence) {
        uint32_t lo = uint32_t(uint64_t(offset));
        if (whence == 0) {
            st32(self + 0xC, lo);
        } else if (whence == 1) {
            st32(self + 0xC, ld32(self + 0xC) + lo);
        } else if (whence == 2) {
            st32(self + 0xC, rw::call(0xCDCBA8, {ld32(self + 8)}) - lo);  // akbMaterialGetAudioDataSize
        }
        return 0;
    }
    static uint32_t readCallback(uint32_t ptr, uint32_t size, uint32_t nmemb, uint32_t source) {
        if (ptr == 0) {
            log(5, 0xEEDA98);  // "ov_read_callback ptr NULL!!"
            return 0;
        }
        uint32_t total = rw::call(0xCDCBA8, {ld32(source + 8)});   // akbMaterialGetAudioDataSize
        uint32_t offset = rw::call(0xCDC8C0, {ld32(source + 8)});  // akbMaterialGetAudioDataOffset
        int32_t remain = int32_t(total - ld32(source + 0xC));
        int32_t req = int32_t(nmemb * size);
        if (req >= remain) req = remain;
        uint32_t count = rw::call(0xCD5DF9, {uint32_t(req), size});  // __udivsi3
        uint32_t enc = rw::call(0xCDCF70, {ld32(source + 8)});      // akbMaterialGetEnableEncryption
        uint32_t bytes = size * count;
        if (enc == 1) {
            uint32_t pos = ld32(source + 0xC);
            rw::call(0xCDCFC0, {pos, ptr, ld32(source + 8) + offset + pos, bytes});  // akbMaterialDecode
        } else {
            uint32_t pos = ld32(source + 0xC);
            rw::call(0x442298, {ptr, ld32(source + 8) + offset + pos, bytes});  // memcpy
        }
        st32(source + 0xC, ld32(source + 0xC) + bytes);
        return count;
    }
};
RW_FUNCTION(0xCE3214, "SQEX::TransferOggVorbis::TransferOggVorbis", TransferOggVorbis::construct);
RW_FUNCTION(0xCE2DF4, "SQEX::TransferOggVorbis::~TransferOggVorbis", TransferOggVorbis::destruct);
RW_FUNCTION(0xCE2E5C, "SQEX::TransferOggVorbis::Initialize", TransferOggVorbis::initialize);
RW_FUNCTION(0xCE2F90, "SQEX::TransferOggVorbis::Execute", TransferOggVorbis::execute);
RW_FUNCTION(0xCE30DC, "SQEX::TransferOggVorbis::ov_seek_callback", TransferOggVorbis::seekCallback);
RW_FUNCTION(0xCE3138, "SQEX::TransferOggVorbis::ov_read_callback", TransferOggVorbis::readCallback);

// ============================================================================================ categories
constexpr uint32_t NUM_CATEGORIES_GOT = 0xFD9304;  // &CategoryManager::numCategories_
constexpr uint32_t CATEGORIES_GOT = 0xFD9308;      // &CategoryManager::categories_
constexpr uint32_t CATEGORY_VALID_GOT = 0xFD928C;  // &CategoryManager::valid_

struct CategoryManager {
    static int32_t initialize(uint32_t config) {
        int32_t n = lds32(config + 0x20);
        if (n <= 0) return 0;
        uint32_t num = got(NUM_CATEGORIES_GOT), cats = got(CATEGORIES_GOT);
        st32(num, uint32_t(n));
        uint32_t arr = malloc_(uint32_t(n) * 4, 4);
        st32(cats, arr);
        for (int32_t k = 0; lds32(num) > k; k++) {
            uint32_t param = rw::call(0xCE608C, {config, uint32_t(k)});  // DriverConfig::GetCategoryParam
            uint32_t v = ld32(param);
            if (v == 0) {
                st32(ld32(cats) + uint32_t(k) * 4, v);
                continue;
            }
            uint32_t c = malloc_(0x40, 4);
            rw::call(0xCE5278, {c});  // CategoryImpl::CategoryImpl
            st32(ld32(cats) + uint32_t(k) * 4, c);
            uint32_t impl = ld32(ld32(cats) + uint32_t(k) * 4);
            uint32_t p = rw::call(0xCE608C, {config, uint32_t(k)});
            rw::call(0xCE52D8, {impl, uint32_t(k), ld32(p)});  // CategoryImpl::Initialize
        }
        st8(got(CATEGORY_VALID_GOT), 1);
        return 0;
    }
    static int32_t update(float dt) {
        uint32_t cats = got(CATEGORIES_GOT), num = got(NUM_CATEGORIES_GOT);
        uint32_t arr = ld32(cats);
        if (arr == 0 || lds32(num) <= 0) return 0;
        int32_t k = 0;
        for (;;) {
            rw::call(0xCE54BC, {ld32(arr + uint32_t(k) * 4), fbits(dt)});  // CategoryImpl::Update
            k++;
            if (lds32(num) <= k) break;
            arr = ld32(cats);
        }
        return 0;
    }
    static uint32_t getCategory(uint32_t id) {
        uint32_t arr = ld32(got(CATEGORIES_GOT));
        if (arr == 0) return 0;
        uint32_t idx = id - 1;
        if (idx >= ld32(got(NUM_CATEGORIES_GOT))) return 0;
        return ld32(arr + idx * 4);
    }
};
RW_FUNCTION(0xCE5BC4, "SQEX::CategoryManager::Initialize", CategoryManager::initialize);
RW_FUNCTION(0xCE5D44, "SQEX::CategoryManager::Update", CategoryManager::update);
RW_FUNCTION(0xCE5DAC, "SQEX::CategoryManager::GetCategory", CategoryManager::getCategory);

// ============================================================================================ DriverConfig
// 8 category parameters (+0x00..+0x1C), category count +0x20, MAX_AT9 +0x24
struct DriverConfig {
    static uint32_t construct(uint32_t self) {
        for (uint32_t k = 0; k < 0x20; k += 4) st32(self + k, 0);
        st32(self + 0x20, 0);
        st32(self + 0x24, 0);
        return self;
    }
    static bool ws(uint8_t c) { return uint32_t(c) - 1u <= 0x1Fu; }  // 1..0x20

    // Skips the current token and the blanks after it; then, when `skipSecond`, the next token and its blanks
    // too. Returns the address of the token found, or 0 at the end of the text.
    static uint32_t nextToken(uint32_t p) {
        uint8_t c = ld8(p);
        bool blank = c <= 0x20;
        if (c == 0) return 0;
        for (;;) {
            if (c > 0x20) {
                if (blank) return p;
                c = ld8(++p);
                if (c == 0) return 0;
            } else {
                blank = true;
                c = ld8(++p);
                if (c == 0) return 0;
            }
        }
    }

    static int32_t analyse(uint32_t self, uint32_t text) {
        if (text == 0) return 0;
        int32_t count = 0;
        uint32_t line = text;
        uint32_t p;
        // the first line: skip leading control characters / blanks (the first character is skipped unchecked)
        auto skipLead = [&](uint32_t s) {
            uint32_t q = s + 1;
            uint32_t r;
            do { r = q; q++; } while (ws(ld8(r)));
            return r;
        };
        p = ws(ld8(line)) ? skipLead(line) : line;
        for (;;) {
            if (rw::call(0x442370, {p, 0xEEDCA4, 8}) == 0) {  // strncasecmp(p, "CATEGORY", 8)
                if (count > 7) {
                    log(5, 0xEEDCB0);  // "too much Categories Appended!!!"
                    st32(self + 0x20, uint32_t(count));
                    return 0;
                }
                uint32_t name = nextToken(p);  // the category name
                if (name == 0) return 0;
                if (ld8(name) == 0) return 0;
                uint32_t value = nextToken(name);  // its parameter
                if (value == 0) return 0;
                st32(self + uint32_t(count) * 4, rw::call(0x410A14, {value}));  // atoi
                count++;
            } else if (rw::call(0x442370, {p, 0xEEDCD0, 7}) == 0) {  // "MAX_AT9"
                uint32_t value = nextToken(p);
                if (value == 0) return 0;
                st32(self + 0x24, rw::call(0x410A14, {value}));
            } else {
                if (rw::call(0x442370, {p, 0xEEDCD8, 3}) != 0 &&     // "END"
                    rw::call(0x442370, {p, 0xEE72D2, 0}) != 0) {     // ""
                    log(5, 0xEEDCDC);  // "Config text is unknown format!!"
                    return -1;
                }
                st32(self + 0x20, uint32_t(count));
                return 0;
            }
            line = rw::call(0x410198, {line, 0xA}) + 1;  // strchr(line, '\n') + 1
            p = ws(ld8(line)) ? skipLead(line) : line;
        }
    }
};
RW_FUNCTION(0xCE5DF8, "SQEX::DriverConfig::DriverConfig", DriverConfig::construct);
RW_FUNCTION(0xCE5E34, "SQEX::DriverConfig::Analyse", DriverConfig::analyse);

// ============================================================================================ CoreSystem
// statics, reached through the GOT
constexpr uint32_t G_SAMPLE_RATE = 0xFD9338, G_NUM_CHANNELS = 0xFD9324, G_CURRENT_BUFFER = 0xFD9320;
constexpr uint32_t G_MASTER_VOLUME = 0xFD9340, G_SUSPEND_VOLUME = 0xFD933C, G_RENDER_LEN = 0xFD9310;
constexpr uint32_t G_RENDER_BUFFER = 0xFD930C, G_BUF_LEN = 0xFD932C, G_BUFFERS = 0xFD9328;
constexpr uint32_t G_PEAK_LEVELS = 0xFD9334, G_SOURCE_MUTEX = 0xFD9318, G_SOURCE_LIST = 0xFD931C;
constexpr uint32_t G_LIST_NODE_VTABLE = 0xFD9344;

struct CoreSystem {
    static int32_t initialize(uint32_t sampleRate, int32_t channels) {
        if (channels > 2) return -1;
        st32(got(G_SAMPLE_RATE), sampleRate);
        st32(got(G_NUM_CHANNELS), uint32_t(channels));
        st32(got(G_CURRENT_BUFFER), 0);
        guest::Frame f(0x18);
        rw::call(0xCF43FC, {f.addr(4), fbits(1.0f)});  // DynamicValue::DynamicValue(1.0)
        uint32_t mv = got(G_MASTER_VOLUME);
        for (uint32_t k = 0; k < 20; k += 4) st32(mv + k, *f.at<uint32_t>(4 + k));
        uint32_t renderLen = uint32_t(channels) << 11;
        st32(got(G_SUSPEND_VOLUME), fbits(1.0f));
        st32(got(G_RENDER_LEN), renderLen);
        uint32_t render = malloc_(renderLen, 4);
        st32(got(G_RENDER_BUFFER), render);
        if (render == 0) {
            log(5, 0xEEDD44);  // "CoreAPI create render buffer failed!! heap memory insufficient"
            return -1;
        }
        uint32_t bufLen = got(G_BUF_LEN);
        uint32_t len = uint32_t(channels) << 10;
        st32(bufLen, len);
        uint32_t buffers = got(G_BUFFERS);
        for (uint32_t k = 0; k < 2; k++) {
            uint32_t b = malloc_(len, 4);
            st32(buffers + k * 4, b);
            if (b == 0) {
                log(5, 0xEEDD84);  // "CoreAPI create output buffer failed!! heap memory insufficient"
                return -1;
            }
            rw::call(0x442310, {b, 0, ld32(bufLen)});  // memset
            len = ld32(bufLen);
        }
        st8(0x100E6C0, 0);
        uint32_t peaks = got(G_PEAK_LEVELS);
        st32(peaks, 0);
        st32(peaks + 4, 0);
        uint32_t out = rw::call(0xCE8260, {sampleRate, uint32_t(channels), 0xCE646C});  // CoreAudioOutInit(.., render cb)
        rw::call(0xCE8778, {out});                                                      // CoreAudioOutKick
        return 0;
    }
    static int32_t createSource(uint32_t out, uint32_t rate, uint32_t channels, uint32_t callback) {
        uint32_t src = malloc_(0x9C, 4);
        rw::call(0xCE78AC, {src});  // CoreSource::CoreSource
        if (src == 0) {
            log(5, 0xEEDDC4);  // "Create CoreSource failed!! heap memory insufficient"
            return -1;
        }
        if (int32_t(rw::call(0xCE8024, {src, rate, channels, callback})) < 0) {  // CoreSource::Initialize
            vcallo(src, 4, {src});
            return -1;
        }
        uint32_t mutex = got(G_SOURCE_MUTEX);
        mutexLock(mutex);
        uint32_t node = malloc_(0x10, 4);
        st32(node + 4, 0);
        st32(node + 8, 0);
        st32(node + 0xC, 0);
        st32(node, got(G_LIST_NODE_VTABLE) + 8);
        uint32_t list = got(G_SOURCE_LIST);  // head, tail, count
        if (ld32(list) == 0) {
            st32(list + 4, node);
            st32(list, node);
            st32(node + 0xC, src);
        } else {
            uint32_t tail = ld32(list + 4);
            st32(node + 4, tail);
            st32(tail + 8, node);
            st32(node + 0xC, src);
            st32(list + 4, node);
        }
        st32(list + 8, ld32(list + 8) + 1);
        mutexUnlock(mutex);
        st32(out, src);
        return 0;
    }
    static uint32_t update(uint32_t dt) {
        rw::call(0xCF449C, {got(G_MASTER_VOLUME), dt});  // DynamicValue::Update(masterVolume_)
        return rw::call(0xCE8A2C, {dt});                 // CoreAudioOutUpdate
    }
};
RW_FUNCTION(0xCE694C, "SQEX::CoreSystem::Initialize", CoreSystem::initialize);
RW_FUNCTION(0xCE6B28, "SQEX::CoreSystem::CreateSource", CoreSystem::createSource);
RW_FUNCTION(0xCE6D78, "SQEX::CoreSystem::Update", CoreSystem::update);

// ============================================================================================ CoreSource
// vtable +0, state mutex +4, render mutex +8, state +0xC (1 ready, 2 stopped, 3 playing, 4 destroyed),
// callback +0x10, first buffer +0x14 (byte), sample rate +0x18, channels +0x1C, queued +0x20, current +0x24,
// frames left +0x28, history count +0x2C, phase +0x30, buffers {data, size} x2 +0x34, history samples +0x44,
// volume +0x4C, pitch +0x50, volume matrix +0x54..+0x60, IIR LPF coefficients +0x64..+0x78, IIR state +0x7C..
struct CoreSource {
    static uint32_t mutexInit(uint32_t m, uint32_t attr) {
        rw::call(0x44D098, {attr});
        rw::call(0x44D0A4, {attr, 1});
        rw::call(0x4466F0, {m, attr});
        return rw::call(0x44D0B0, {attr});
    }
    static uint32_t construct(uint32_t self) {
        guest::Frame f(8);
        st32(self, got(0xFD9350) + 8);
        mutexInit(self + 4, f.addr(4));
        mutexInit(self + 8, f.addr(4));
        st8(self + 0x14, 1);
        st32(self + 0x30, 0);
        for (uint32_t o : {0x0Cu, 0x10u, 0x18u, 0x1Cu, 0x20u, 0x24u, 0x28u}) st32(self + o, 0);
        st32(self + 0x4C, fbits(1.0f));
        st32(self + 0x2C, 0);
        st32(self + 0x50, fbits(1.0f));
        for (uint32_t o : {0x34u, 0x38u, 0x3Cu, 0x40u}) st32(self + o, 0);
        for (uint32_t o : {0x44u, 0x46u, 0x48u, 0x4Au}) st16(self + o, 0);
        return self;
    }
    static uint32_t destruct(uint32_t self) {
        st32(self, got(0xFD9350) + 8);
        rw::call(0x446714, {self + 8});  // pthread_mutex_destroy
        rw::call(0x446714, {self + 4});
        setObjectVtable(self);
        return self;
    }
    // state change under the state mutex: refused (-1) once destroyed (state 0 or 4)
    static int32_t setState(uint32_t self, uint32_t state) {
        mutexLock(self + 4);
        uint32_t st = ld32(self + 0xC);
        int32_t r;
        if ((st & ~4u) != 0) {
            st32(self + 0xC, state);
            r = 0;
        } else {
            r = -1;
        }
        mutexUnlock(self + 4);
        return r;
    }
    static int32_t start(uint32_t self) { return setState(self, 3); }
    static int32_t stop(uint32_t self) { return setState(self, 2); }
    static int32_t destroy(uint32_t self) {
        mutexLock(self + 8);
        mutexLock(self + 4);
        st32(self + 0xC, 4);
        mutexUnlock(self + 4);
        mutexUnlock(self + 8);
        return 0;
    }
    static int32_t setField(uint32_t self, uint32_t offset, uint32_t v) {
        mutexLock(self + 4);
        int32_t r;
        if ((ld32(self + 0xC) & ~4u) != 0) {
            st32(self + offset, v);
            r = 0;
        } else {
            r = -1;
        }
        mutexUnlock(self + 4);
        return r;
    }
    static int32_t setVolume(uint32_t self, float v) { return setField(self, 0x4C, fbits(v)); }
    static int32_t setPitch(uint32_t self, float v) { return setField(self, 0x50, fbits(v)); }

    static int32_t queueBuffer(uint32_t self, uint32_t data, uint32_t size) {
        mutexLock(self + 4);
        int32_t r = -1;
        if ((ld32(self + 0xC) & ~4u) != 0) {
            int32_t queued = lds32(self + 0x20);
            if (queued <= 1) {
                uint64_t qr = rw::call64(0xCD5E79, {size, ld32(self + 0x1C) << 1});  // __aeabi_uidivmod
                if (uint32_t(qr >> 32) == 0) {
                    uint32_t idx = ld32(self + 0x24);
                    if (queued != 0) idx = uint32_t(int32_t(idx + 1) % 2);
                    st32(self + 0x34 + idx * 8, data);
                    st32(self + 0x38 + idx * 8, size);
                    st32(self + 0x20, uint32_t(queued + 1));
                    r = 0;
                }
            }
        }
        mutexUnlock(self + 4);
        return r;
    }

    static int32_t setVolumeMatrix(uint32_t self, float l, float r) {
        uint32_t out = rw::call(0xCE6C74, {});  // CoreSystem::GetOutputChannels
        int32_t ch = lds32(self + 0x1C);
        if (out == 1) {
            if (ch == 1) {
                stf(self + 0x54, 1.0f);
                return 0;
            }
            if (ch != 2) return 0;
        } else if (out == 2) {
            if (ch != 1) {
                if (ch == 2) {
                    stf(self + 0x54, 1.0f);
                    st32(self + 0x5C, 0);
                    st32(self + 0x58, 0);
                    stf(self + 0x60, 1.0f);
                }
                return 0;
            }
        } else {
            return 0;
        }
        stf(self + 0x54, l);
        stf(self + 0x58, r);
        return 0;
    }

    static int32_t setPan(uint32_t self, float pan) {
        mutexLock(self + 4);
        if ((ld32(self + 0xC) & ~4u) == 0) {
            mutexUnlock(self + 4);
            return -1;
        }
        const float TWO_PI = bitsf(0x40C90FDB), HALF_PI = bitsf(0x3FC90FDB);
        uint32_t c, s;
        if (pan < -1.0f) {
            c = rw::call(0x41C8AC, {fbits(TWO_PI)});  // cosf
            s = rw::call(0x41A9E0, {fbits(TWO_PI)});  // sinf
        } else if (pan > 1.0f) {
            c = 0xB33BBD2E;  // cos(pi/2)
            s = fbits(1.0f);
        } else {
            float a = pan + 1.0f;
            a = a * 0.5f;
            a = a * HALF_PI;
            if (a <= 0.0f) {
                float b = a + TWO_PI;
                c = rw::call(0x41C8AC, {fbits(b)});
                s = rw::call(0x41A9E0, {fbits(b)});
            } else {
                if (a > TWO_PI) a = a - TWO_PI;
                c = rw::call(0x41C8AC, {fbits(a)});
                s = rw::call(0x41A9E0, {fbits(a)});
            }
        }
        setVolumeMatrix(self, bitsf(c), bitsf(s));
        mutexUnlock(self + 4);
        return 0;
    }

    // second-order low-pass coefficients for the cutoff frequency
    static int32_t setIIRLPF(uint32_t self, float cutoff) {
        int32_t rate = int32_t(rw::call(0xCE6C60, {}));  // CoreSystem::GetOutputSampleRate
        float x = cutoff * bitsf(0x40490FDB);
        x = x / float(rate);
        float t = bitsf(rw::call(0x41A968, {fbits(x)}));  // tanf
        const float TWO_PI = bitsf(0x40C90FDB);
        float s15 = t / TWO_PI;
        float s12 = s15 * TWO_PI;
        s12 = s12 / bitsf(0x3F3504F3);
        float s13 = s15 * bitsf(0x411DE9E7);
        s15 = s13 * s15;
        float s14 = s15 * 4.0f;
        s13 = s15 * 8.0f;
        float a0 = s12 + 1.0f;
        float s11 = 1.0f - s12;
        a0 = a0 + s14;
        s11 = s14 + s11;
        stf(self + 0x64, a0);
        float b1 = s13 - 2.0f;
        float b0 = s14 / a0;
        s11 = s11 / a0;
        stf(self + 0x70, b0);
        stf(self + 0x78, b0);
        b1 = b1 / a0;
        stf(self + 0x6C, s11);
        float b2 = s13 / a0;
        stf(self + 0x68, b1);
        stf(self + 0x74, b2);
        return 0;
    }

    static int32_t initialize(uint32_t self, uint32_t rate, int32_t channels, uint32_t callback) {
        if (channels > 2) return -1;
        st32(self + 0x1C, uint32_t(channels));
        st32(self + 0x10, callback);
        st32(self + 0x18, rate);
        st32(self + 0xC, 1);
        int32_t out = int32_t(rw::call(0xCE6C60, {}));
        setIIRLPF(self, float(out) * bitsf(0x3ECCCCCD));
        for (uint32_t o : {0x90u, 0x8Cu, 0x80u, 0x7Cu, 0x98u, 0x94u, 0x88u, 0x84u}) st32(self + o, 0);
        setVolumeMatrix(self, bitsf(0x3F3504F3), bitsf(0x3F3504F3));
        return 0;
    }

    // RenderMix(out, bytes): resamples the queued 16-bit buffers (two buffers, +0x34/+0x38 data and size, +0x24 the
    // current one, +0x28 its frames left) by linear interpolation between the last two input frames (history at
    // +0x44.., +0x2C how many are loaded, +0x30 the position between them; step = source rate / output rate *
    // pitch) and adds them into the float output with the channel gains (+0x54, times volume and master volume).
    // A used-up buffer is released (+0x20 queued count) and the source's callback (+0x10) is told.
    static int32_t renderMix(uint32_t self, uint32_t out, uint32_t bytes) {
        constexpr uint32_t UDIVSI3 = 0xCD5DF9;
        auto bufData = [&](uint32_t i) { return ld32(self + 0x34 + i * 8); };
        auto bufSize = [&](uint32_t i) { return ld32(self + 0x38 + i * 8); };
        float gains[32] = {};
        int32_t inCh, outCh, frames, left, hc, ch;
        uint32_t rate, idx, src, n;
        float step, frac;

        mutexLock(self + 8);
        mutexLock(self + 4);
        if (ld32(self + 0xC) != 3) {
            mutexUnlock(self + 4);
            goto end;
        }
        mutexUnlock(self + 4);
        rate = rw::call(0xCE6C60, {});                    // CoreSystem::GetOutputSampleRate
        outCh = int32_t(rw::call(0xCE6C74, {}));          // CoreSystem::GetOutputChannels
        inCh = lds32(self + 0x1C);
        n = 0;
        if (int32_t(uint32_t(outCh) * uint32_t(inCh)) > 0) {
            do {
                const float gain = ldf(self + 0x54 + 4 * n);
                const float volume = ldf(self + 0x4C);
                const float master = bitsf(rw::call(0xCE6CF8, {}));   // CoreSystem::GetMasterVolume
                inCh = lds32(self + 0x1C);
                if (n < 32) gains[n] = gain * (volume * master) / 32767.0f;
                n++;
            } while (int32_t(uint32_t(outCh) * uint32_t(inCh)) > int32_t(n));
        }
        {
            const int32_t sourceRate = lds32(self + 0x18);
            const float ratio = uint32_t(sourceRate) == rate ? 1.0f : float(sourceRate) / float(int32_t(rate));
            step = ratio * ldf(self + 0x50);
        }
        if (step <= 0.0f) goto end;                       // (a NaN step goes on)
        frames = int32_t(rw::call(UDIVSI3, {bytes, uint32_t(outCh) * 4}));
        idx = ld32(self + 0x24);
        left = lds32(self + 0x28);
        src = bufData(idx) + 2 * ((bufSize(idx) >> 1) - uint32_t(left) * uint32_t(inCh));
        if (frames <= 0 || lds32(self + 0x20) <= 0) goto end;

    next:
        if (left > 0) goto mix;
    empty:
        if (lds32(self + 0x20) <= 0) goto mix;
        // take the next queued buffer (the first one after a start)
        if (ld8(self + 0x14)) {
            mutexLock(self + 4);
            const uint32_t q = rw::call(UDIVSI3, {bufSize(ld32(self + 0x24)), ld32(self + 0x1C) << 1});
            st8(self + 0x14, 0);
            st32(self + 0x28, q);
            mutexUnlock(self + 4);
        } else {
            mutexLock(self + 4);
            const int32_t i = int32_t(ld32(self + 0x24) + 1) % 2;
            const uint32_t channels = ld32(self + 0x1C);
            st32(self + 0x20, ld32(self + 0x20) - 1);
            st32(self + 0x24, uint32_t(i));
            st32(self + 0x28, rw::call(UDIVSI3, {bufSize(uint32_t(i)), channels << 1}));
            mutexUnlock(self + 4);
            if (const uint32_t callback = ld32(self + 0x10)) rw::call(ld32(ld32(callback)), {callback});
        }
        idx = ld32(self + 0x24);
        hc = lds32(self + 0x2C);
        left = lds32(self + 0x28);
        src = bufData(idx);
        if (hc > 1 || left <= 0) goto next;
        // load the history's first two frames from the new buffer
        ch = lds32(self + 0x1C);
        for (;;) {
            if (ch > 0) {
                st16(self + 0x44 + uint32_t(hc) * 2, ld16(src));
                if (ch != 1) st16(self + 0x48 + uint32_t(hc) * 2, ld16(src + 2));
                src += uint32_t(ch) * 2;
            }
            hc++;
            left--;
            st32(self + 0x28, uint32_t(left));
            st32(self + 0x2C, uint32_t(hc));
            if (hc == 2) goto next;
            if (left == 0) goto empty;
        }

    mix:
        if (lds32(self + 0x2C) <= 1) goto end;
        if (outCh > 0) {
            ch = lds32(self + 0x1C);
            for (int32_t o = 0; o != outCh; o++) {
                if (ch <= 0) continue;
                const uint32_t dst = out + uint32_t(o) * 4;
                float acc = ldf(dst);
                for (int32_t i = 0; i < ch; i++) {
                    const int32_t a = lds16(self + 0x44 + uint32_t(i) * 4);
                    const float t = ldf(self + 0x30);
                    const int32_t b = lds16(self + 0x46 + uint32_t(i) * 4);
                    float s = float(a);
                    s = s + float(int32_t(uint32_t(b) - uint32_t(a))) * t;
                    const uint32_t g = uint32_t(o) * uint32_t(ch) + uint32_t(i);
                    acc = acc + s * (g < 32 ? gains[g] : 0.0f);
                    stf(dst, acc);
                }
            }
            out += uint32_t(outCh) * 4;
        }
        frac = step + ldf(self + 0x30);
        frames--;
        stf(self + 0x30, frac);
        while (frac >= 1.0f) {                            // step to the next input frame
            frac = frac - 1.0f;
            ch = lds32(self + 0x1C);
            const uint32_t loaded = ld32(self + 0x2C);
            st32(self + 0x2C, loaded - 1);
            stf(self + 0x30, frac);
            if (ch > 0) {
                st16(self + 0x44, ld16(self + 0x46));
                if (ch != 1) st16(self + 0x48, ld16(self + 0x4A));
            }
            const int32_t remaining = lds32(self + 0x28);
            if (remaining > 0) {
                if (ch > 0) {
                    st16(self + 0x46, ld16(src));
                    if (ch != 1) st16(self + 0x4A, ld16(src + 2));
                    src += uint32_t(ch) * 2;
                }
                st32(self + 0x2C, loaded);
                st32(self + 0x28, uint32_t(remaining - 1));
            }
        }
        if (frames == 0 || lds32(self + 0x20) <= 0) goto end;
        left = lds32(self + 0x28);
        goto next;

    end:
        mutexUnlock(self + 8);
        return 0;
    }
};
RW_FUNCTION(0xCE78AC, "SQEX::CoreSource::CoreSource", CoreSource::construct);
RW_FUNCTION(0xCE7684, "SQEX::CoreSource::~CoreSource", CoreSource::destruct);
RW_FUNCTION(0xCE76E4, "SQEX::CoreSource::Start", CoreSource::start);
RW_FUNCTION(0xCE7720, "SQEX::CoreSource::Stop", CoreSource::stop);
RW_FUNCTION(0xCE775C, "SQEX::CoreSource::Destroy", CoreSource::destroy);
RW_FUNCTION(0xCE779C, "SQEX::CoreSource::SetVolume", CoreSource::setVolume);
RW_FUNCTION(0xCE77D8, "SQEX::CoreSource::SetPitch", CoreSource::setPitch);
RW_FUNCTION(0xCE7814, "SQEX::CoreSource::QueueBuffer", CoreSource::queueBuffer);
RW_FUNCTION(0xCE7990, "SQEX::CoreSource::SetVolumeMatrix", CoreSource::setVolumeMatrix);
RW_FUNCTION(0xCE7A18, "SQEX::CoreSource::SetPan", CoreSource::setPan);
RW_FUNCTION(0xCE7F60, "SQEX::CoreSource::SetIIRLPF", CoreSource::setIIRLPF);
RW_FUNCTION(0xCE8024, "SQEX::CoreSource::Initialize", CoreSource::initialize);
RW_FUNCTION(0xCE7B18, "SQEX::CoreSource::RenderMix", CoreSource::renderMix);

}  // namespace sqex
