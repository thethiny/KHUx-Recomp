// SQEX sound driver (sqexsdlib): action queue, sounds, sound pool, voices, dynamic values.
#include "Sqex.hpp"

using namespace b2;

namespace sqex {

// ============================================================================================ DynamicValue
struct DynamicValueFn {
    static float getValue(const DynamicValue *self) {
        int32_t duration = self->duration;
        if (duration == 0) return self->to;
        float t = float(int32_t(self->elapsed)) / float(duration);
        float delta = self->to - self->from;
        float p = delta * t;
        return self->from + p;
    }
    static float setTarget(DynamicValue *self, float target, int32_t duration) {
        float v = getValue(self);
        uint8_t f = uint8_t(rw::get<uint8_t>(&self->flags, 0) & ~1u) | (duration != 0 ? 1 : 0);
        self->to = target;
        self->duration = duration;
        rw::set<uint8_t>(&self->flags, 0, f);
        self->elapsed = 0;
        self->from = v;
        return v;
    }
    static uint32_t update(DynamicValue *self, int32_t dt) {
        uint32_t need = rw::call(0xCF4490, {rw::guest_addr(self)});  // DynamicValue::NeedUpdate
        if (need == 0) return need;
        int32_t e = int32_t(uint32_t(self->elapsed) + uint32_t(dt));
        self->elapsed = e;
        uint8_t f = rw::get<uint8_t>(&self->flags, 0);
        if (e >= int32_t(self->duration)) {
            self->elapsed = self->duration;
            rw::set<uint8_t>(&self->flags, 0, uint8_t(f & ~1u));
        } else {
            rw::set<uint8_t>(&self->flags, 0, uint8_t(f | 1u));
        }
        return need;
    }
    static DynamicValue *initialize(DynamicValue *self) {
        self->elapsed = 0;
        rw::set<uint8_t>(&self->flags, 0, uint8_t(rw::get<uint8_t>(&self->flags, 0) & ~1u));
        self->from = 0.0f;
        self->to = 0.0f;
        self->duration = 0;
        self->flags = 0;
        return self;
    }
};
RW_FUNCTION(0xCF4418, "SQEX::DynamicValue::GetValue", DynamicValueFn::getValue);
RW_FUNCTION(0xCF4450, "SQEX::DynamicValue::SetTarget", DynamicValueFn::setTarget);
RW_FUNCTION(0xCF449C, "SQEX::DynamicValue::Update", DynamicValueFn::update);
RW_FUNCTION(0xCF43B4, "SQEX::DynamicValue::Initialize", DynamicValueFn::initialize);

static float value(const DynamicValue &v) { return DynamicValueFn::getValue(&v); }
static uint32_t needUpdate(const DynamicValue &v) { return rw::call(0xCF4490, {rw::guest_addr(&v)}); }

// ============================================================================================ ActionManager
// A queued request from the game thread (48-byte node: vtable, 10 words of ACTION, next).
constexpr uint32_t ACTION_NODE_SIZE = 0x30;

struct ActionManager {
    static int32_t initialize() {
        const uint32_t G = ACTION_MANAGER;
        uint32_t nodes = ld32(G);
        if (nodes != 0) {
            // destroy the old nodes from the last one down (count in the array header)
            uint32_t p = nodes + ld32(nodes - 4) * ACTION_NODE_SIZE;
            while (ld32(G) != p) {
                p -= ACTION_NODE_SIZE;
                rw::call(ld32(ld32(p)), {p});
            }
            free_(ld32(G) - 8);
        }
        st32(G, 0);
        st32(G + 4, 0);
        st32(G + 8, 0);
        st32(G + 0xC, 0);
        st32(G + 0x10, 256);
        uint32_t block = malloc_(0x3008, 4);
        uint32_t vtable = got(0xFD9280) + 8;
        st32(block + 4, 256);
        st32(block, ACTION_NODE_SIZE);
        uint32_t first = block + 8;
        for (uint32_t k = 0; k < 256; k++) {
            uint32_t e = first + k * ACTION_NODE_SIZE;
            st32(e, vtable);
            for (uint32_t w = 4; w <= 0x2C; w += 4) st32(e + w, 0);
        }
        st32(G, first);
        if (first != 0) {
            st32(G + 4, first);   // free list: every node, chained through +0x2C
            for (uint32_t k = 1; k < 256; k++)
                st32(first + (k - 1) * ACTION_NODE_SIZE + 0x2C, first + k * ACTION_NODE_SIZE);
            st32(G + 8, 0);
            st32(G + 0xC, 0);
        } else {
            log(5, 0xEED038);  // "Queue:NODEs create failed!! heap memory insufficient"
        }
        return 0;
    }

    static int32_t addAction(const uint8_t *action) {
        const uint32_t G = ACTION_MANAGER;
        mutexLock(ACTION_MUTEX);
        uint32_t words[10];
        std::memcpy(words, action, 40);
        uint32_t node = ld32(G + 4);
        int32_t result;
        if (node != 0) {
            uint32_t head = ld32(G + 8);
            st32(G + 4, ld32(node + 0x2C));
            st32(node + 0x2C, 0);
            for (int k = 0; k < 10; k++) st32(node + 4 + 4 * k, words[k]);
            if (head == 0) {
                st32(G + 0xC, node);
                st32(G + 8, node);
            } else {
                uint32_t tail = ld32(G + 0xC);
                st32(G + 0xC, node);
                st32(tail + 0x2C, node);
            }
            result = 0;
        } else {
            result = -1;
        }
        mutexUnlock(ACTION_MUTEX);
        return result;
    }

    static uint32_t update(uint32_t /*dt*/) {
        const uint32_t G = ACTION_MANAGER;
        for (;;) {
            mutexLock(ACTION_MUTEX);
            uint32_t node = ld32(G + 8);
            if (node == 0) return mutexUnlock(ACTION_MUTEX);
            uint32_t next = ld32(node + 0x2C);
            uint32_t type = ld32(node + 4);
            uint32_t f4 = ld32(node + 0x18);
            uint32_t target = ld32(node + 8);  // sound id / LayoutManager argument
            uint32_t f5 = ld32(node + 0x1C);
            st32(node + 0x2C, ld32(G + 4));
            uint32_t f6 = ld32(node + 0x20);
            st32(G + 4, node);
            uint32_t f7 = ld32(node + 0x24);
            st32(G + 8, next);
            uint32_t f8 = ld32(node + 0x28);
            uint32_t a1 = ld32(node + 0xC), a2 = ld32(node + 0x10), a3 = ld32(node + 0x14);
            mutexUnlock(ACTION_MUTEX);
            auto sound = [&]() { return rw::call(0xCDEDF8, {target}); };  // SoundManager::GetSound(id)
            switch (type) {
            case 1: rw::call(0xCDF2EC, {target}); break;  // SoundManager::SuspendAll
            case 2: rw::call(0xCDF334, {target}); break;  // SoundManager::ResumeAll
            case 3: rw::call(0xCDF2A4, {target}); break;  // SoundManager::StopAll
            case 4: if (uint32_t s = sound()) rw::call(0xCDE8B0, {s, a1, a2}); break;      // SoundImpl::Play
            case 5: if (uint32_t s = sound()) rw::call(0xCDDA08, {s, a1}); break;          // SoundImpl::Stop
            case 6: if (uint32_t s = sound()) rw::call(0xCDDDD0, {s, a1}); break;          // SoundImpl::PauseOn
            case 7: if (uint32_t s = sound()) rw::call(0xCDDE60, {s, a1}); break;          // SoundImpl::PauseOff
            case 8: if (uint32_t s = sound()) rw::call(0xCDDEE0, {s, a1, a2}); break;      // SoundImpl::SetVolume
            case 9: if (uint32_t s = sound()) rw::call(0xCDE048, {s, a1, a2}); break;      // SoundImpl::SetPitch
            case 10: if (uint32_t s = sound()) rw::call(0xCDE140, {s, a1, a2}); break;     // SoundImpl::SetPan
            case 11: if (uint32_t s = sound()) rw::call(0xCDDF64, {s, a1, a2, a3}); break; // SetRandomVolume
            case 12: if (uint32_t s = sound()) rw::call(0xCDE074, {s, a1, a2, a3}); break; // SetRandomPitch
            case 13: if (uint32_t s = sound()) rw::call(0xCDDF0C, {s, a1}); break;         // SetCategoryVolume
            case 14: if (uint32_t s = sound()) rw::call(0xCDDA90, {s, a1, a2, a3}); break; // SetPosition
            case 15:  // LayoutManager::SetListener(9 floats)
                rw::call(0xCE6104, {target, a1, a2, a3, f4, f5, f6, f7, f8});
                break;
            case 16:  // LayoutManager::SetListener(6 floats)
                rw::call(0xCE62AC, {target, a1, a2, a3, f4, f5});
                break;
            default:
                log(5, 0xEED070, type);  // "ActionManager::Update execute unkown action %d !!"
                break;
            }
        }
    }
};
RW_FUNCTION(0xCDD240, "SQEX::ActionManager::Initialize", ActionManager::initialize);
RW_FUNCTION(0xCDD430, "SQEX::ActionManager::AddAction", ActionManager::addAction);
RW_FUNCTION(0xCDD554, "SQEX::ActionManager::Update", ActionManager::update);

// ============================================================================================ SoundImpl
struct SoundImplFn {
    static uint32_t data(SoundImpl *self) { return vcallo(rw::guest_addr(self), 0xC, {rw::guest_addr(self)}); }

    static SoundImpl *construct(SoundImpl *self) {
        uint32_t s = rw::guest_addr(self);
        self->vtable = got(0xFD9288) + 8;  // SQEX::SoundImpl vtable
        self->voice = 0;
        self->id = 0;
        const uint32_t ctor = 0xCF43FC;  // DynamicValue::DynamicValue(float)
        rw::call(ctor, {s + 0x0C, fbits(1.0f)});
        rw::call(ctor, {s + 0x20, fbits(0.0f)});
        rw::call(ctor, {s + 0x34, fbits(1.0f)});
        rw::call(ctor, {s + 0x48, fbits(1.0f)});
        rw::call(ctor, {s + 0x5C, fbits(1.0f)});
        rw::call(ctor, {s + 0x70, fbits(0.0f)});
        rw::call(ctor, {s + 0x84, fbits(1.0f)});
        rw::call(ctor, {s + 0x98, fbits(1.0f)});
        self->state = 0;
        self->position[0] = 0.0f;
        self->pauseState = 0;
        self->position[1] = 0.0f;
        self->flagB4 = 0;
        self->position[2] = 0.0f;
        self->flagB5 = 0;
        self->innerRange = 0.0f;
        self->playTime = 0;
        self->outerRange = 0.0f;
        self->flagC4 = 0;
        self->layoutPan = 0.0f;
        self->externalId = 0;
        self->interiorFactor = 0;
        self->seekTime = 0;
        self->layoutEnabled = 0;
        self->noLayout = 0;
        self->randomVolume = 1.0f;
        self->randomPitch = 1.0f;
        self->attrVolume = 1.0f;
        self->position[3] = 1.0f;
        self->layoutVolume = 1.0f;
        return self;
    }

    static int32_t getState(const SoundImpl *self) {
        int32_t st = self->state;
        if (st == 5) return 3;
        if (self->pauseState == 2) return 4;
        if (st == 1) return 1;
        return st != 0 ? 2 : 0;
    }

    static int32_t stop(SoundImpl *self, uint32_t fadeMs) {
        uint32_t st = uint32_t(int32_t(self->state));
        if (st == 5) return 0;
        if (st <= 2) {
            self->state = 5;
            return 0;
        }
        DynamicValueFn::setTarget(&self->playFade, 0.0f, int32_t(fadeMs));
        if (self->state == 4 || self->pauseState == 2 || fadeMs == 0) {
            uint32_t v = self->voice;
            if (v != 0) vcallo(v, 0x10, {v});  // Voice::Stop
        }
        self->state = 4;
        return 0;
    }

    static int32_t destroy(SoundImpl *self) {
        int32_t st = self->state;
        if ((uint32_t(st) & ~4u) != 1) {
            // "sound can't destroy!! [id:%d][state:%d]"
            rw::call(ANDROID_LOG, {5, LOG_TAG, 0xEED0A4, uint32_t(int32_t(self->id)), uint32_t(st)});
            return -1;
        }
        uint32_t v = self->voice;
        if (v == 0) return 0;
        vcallo(v, 4, {v});  // deleting destructor
        self->voice = 0;
        return 0;
    }

    static int32_t updateVolume(SoundImpl *self) {
        uint32_t v = self->voice;
        if (v == 0) return 0;
        uint32_t fn = ld32(ld32(v) + 0x14);
        float x = value(self->volume);
        x = x * value(self->playFade);
        x = x * value(self->pauseFade);
        x = x * value(self->fadeVolume);
        uint32_t d = data(self);
        x = x * bitsf(rw::call(0xCDC698, {d}));  // akbSoundGetVolume
        x = x * value(self->categoryVolume);
        x = x * self->randomVolume;
        x = x * self->attrVolume;
        x = x * self->layoutVolume;
        return sxtb(rw::call(fn, {v, fbits(x)}));
    }

    static int32_t setVolume(SoundImpl *self, float volume, uint32_t fadeMs) {
        DynamicValueFn::setTarget(&self->volume, volume, int32_t(fadeMs));
        if (fadeMs == 0) updateVolume(self);
        return 0;
    }

    static int32_t updatePitch(SoundImpl *self) {
        uint32_t v = self->voice;
        if (v == 0) return 0;
        uint32_t fn = ld32(ld32(v) + 0x18);
        float x = value(self->pitch);
        x = x * value(self->categoryPitch);
        x = x * self->randomPitch;
        return sxtb(rw::call(fn, {v, fbits(x)}));
    }

    static int32_t updatePan(SoundImpl *self) {
        uint32_t v = self->voice;
        if (v == 0) return 0;
        uint32_t fn = ld32(ld32(v) + 0x1C);
        float x = value(self->pan) + self->layoutPan;
        return sxtb(rw::call(fn, {v, fbits(x)}));
    }

    // Distance attenuation and pan from the listener matrix (3D layout sounds).
    static int32_t updateLayoutPanning(SoundImpl *self) {
        if (self->layoutEnabled == 0 || self->noLayout == 1) {
            self->layoutVolume = 1.0f;
            self->layoutPan = 0.0f;
            return 0;
        }
        float outer = self->outerRange;
        if (outer <= 0.0f) {
            self->layoutVolume = 0.0f;
            return 0;
        }
        guest::Frame f(0x40);
        float *m = f.at<float>(0);
        for (int k = 0; k < 16; k++) m[k] = 0.0f;
        m[0] = m[5] = m[10] = m[15] = 1.0f;
        rw::call(0xCE63E8, {f.addr()});  // LayoutManager::GetListenerMatrix
        float px = self->position[0], py = self->position[1], pz = self->position[2], pw = self->position[3];
        float s15 = py * m[5];
        s15 = s15 + px * m[4];
        float s16 = py * m[1];
        s16 = s16 + px * m[0];
        s15 = s15 + pz * m[6];
        float s17 = py * m[9];
        s16 = s16 + pz * m[2];
        s17 = s17 + px * m[8];
        s15 = s15 + pw * m[7];
        s16 = s16 + pw * m[3];
        s17 = s17 + pz * m[10];
        s15 = s15 * s15;
        s17 = s17 + pw * m[11];
        s15 = s15 + s16 * s16;
        s15 = s15 + s17 * s17;
        float dist = std::sqrt(s15);
        if (std::isnan(dist)) dist = bitsf(rw::call(0x427C34, {fbits(s15)}));  // sqrtf (errno path)
        float inner = self->innerRange;
        if (inner >= dist) {
            self->layoutVolume = 1.0f;
        } else {
            float o = self->outerRange;
            float att = (o - dist) / (o - inner);
            if (att < 0.0f) att = 0.0f;
            self->layoutVolume = att;
        }
        if (s16 == 0.0f && s17 == 0.0f) return 0;
        float a = bitsf(rw::call(0x41C768, {fbits(s16), fbits(std::fabs(s17))}));  // atan2f
        self->layoutPan = (a + a) / bitsf(0x40490FDB);
        return 0;
    }

    static int32_t corePlay(SoundImpl *self) {
        if (uint32_t(int32_t(self->state)) - 1u > 1u) return -1;
        int32_t type = int32_t(rw::call(0xCDC604, {data(self)}));  // akbSoundGetType
        uint32_t material = 0;
        if (type == 0) {
            log(4, 0xEED174);  // "empty!!!"
            self->state = 5;
            return 0;
        }
        if (type == 2) {
            // random: the first material whose cumulative weight reaches a roll of 0..100, never the last one twice
            int32_t roll = int32_t(rw::call(0x434738, {})) % 101;  // lrand48
            uint32_t k = 0;
            for (;;) {
                if (int32_t(k) >= int32_t(rw::call(0xCDC73C, {data(self)}))) {  // akbSoundGetNumMaterials
                    rw::call(0xCDC720, {data(self), 0xFF});                  // akbSoundSetWork
                    return stop(self, 0);
                }
                if (roll <= int32_t(rw::call(0xCDC754, {data(self), k}))) break;  // akbSoundGetRandomValue
                k++;
            }
            uint32_t chosen = k;
            if (int32_t(k) == int32_t(rw::call(0xCDC704, {data(self)}))) {  // akbSoundGetWork
                int32_t best = 0;
                for (uint32_t j = 0; int32_t(j) < int32_t(rw::call(0xCDC73C, {data(self)})); j++) {
                    if (k == j) continue;
                    int32_t r = int32_t(rw::call(0xCDC754, {data(self), j}));
                    if (best < r) {
                        best = r;
                        chosen = j;
                    }
                }
            }
            material = chosen;
            rw::call(0xCDC720, {data(self), chosen & 0xFF});
        } else if (type == 3) {
            // sequential
            uint32_t work = rw::call(0xCDC704, {data(self)});
            uint32_t next;
            if (work == 0xFF) {
                next = 0;
            } else {
                uint32_t n = rw::call(0xCDC73C, {data(self)});
                uint64_t qr = rw::call64(0xCD5F29, {work + 1, n});  // __aeabi_idivmod
                next = uint32_t(qr >> 32) & 0xFF;
            }
            material = next;
            rw::call(0xCDC720, {data(self), next});
        } else {
            material = 0;
            if (type == 4) self->noLayout = 1;
        }
        uint32_t attr = rw::call(0xCF3768, {rw::guest_addr(self)});  // AttributeSystem::Execute
        if (attr == 0) return stop(self, 0);
        uint32_t mdata = rw::call(0xCDC7E8, {vcallo(rw::guest_addr(self), 0xC, {rw::guest_addr(self), attr}), material});
        uint32_t voice = malloc_(0x1C, 4);
        rw::call(0xCE2900, {voice, rw::guest_addr(self), mdata});  // Voice::Voice
        self->voice = voice;
        if (voice == 0) {
            log(5, 0xEED180);  // "[SoundImpl::Play] create voice failed!! heap memory insufficient"
            self->state = 5;
            return -1;
        }
        uint32_t create = ld32(ld32(voice) + 8);
        uint32_t loopType = vcallo(rw::guest_addr(self), 8, {rw::guest_addr(self)});
        if (int32_t(rw::call(create, {voice, loopType == 1 ? 1u : 0u, self->seekTime})) == -1) {
            log(5, 0xEED1C4);  // "[SoundImpl::Play]voice create failed"
            self->state = 5;
            return -1;
        }
        uint32_t cat = rw::call(0xCDC838, {data(self)});  // akbSoundGetCategory
        uint32_t category = rw::call(0xCE5DAC, {cat});    // CategoryManager::GetCategory
        if (category != 0) {
            rw::call(0xCE5628, {category, uint32_t(int32_t(self->id))});  // CategoryImpl::AddSound
        } else if (ld8(got(0xFD928C)) != 0) {                             // CategoryManager::valid_
            log(5, 0xEED1EC, rw::call(0xCDC838, {data(self)}));
        }
        if (rw::call(0xCDCBE0, {data(self)}) == 1) {  // akbMaterialGetEnableRandomVolume
            guest::Frame f(8);
            rw::call(0xCDCCAC, {f.addr(), data(self)});  // akbMaterialGetRandomVolume(out[2])
            float lo = *f.at<float>(4);
            int32_t r = int32_t(rw::call(0x434738, {})) % 101;
            float hi = *f.at<float>(0);
            float d = hi - lo;
            float t = float(r) * bitsf(0x3C23D70A);
            self->randomVolume = lo + d * t;
        }
        if (rw::call(0xCDCC24, {data(self)}) == 1) {  // akbMaterialGetEnableRandomPitch
            guest::Frame f(8);
            rw::call(0xCDCD08, {f.addr(), data(self)});  // akbMaterialGetRandomPitch
            float lo = *f.at<float>(4);
            int32_t r = int32_t(rw::call(0x434738, {})) % 101;
            float hi = *f.at<float>(0);
            float d = hi - lo;
            float t = float(r) * bitsf(0x3C23D70A);
            self->randomPitch = lo + d * t;
        }
        rw::save<uint32_t>(rw::guest_addr(&self->innerRange), rw::call(0xCDC84C, {data(self)}));   // InnerRange
        rw::save<uint32_t>(rw::guest_addr(&self->outerRange), rw::call(0xCDC860, {data(self)}));   // OuterRange
        self->interiorFactor = rw::call(0xCDC874, {data(self)});                                     // InteriorFactor
        updateLayoutPanning(self);
        updateVolume(self);
        updatePitch(self);
        updatePan(self);
        uint32_t v = self->voice;
        vcallo(v, 0xC, {v});  // Voice::Play
        self->state = 3;
        if (self->pauseState == 2) {
            v = self->voice;
            vcallo(v, 0x20, {v});  // pause
        }
        return 0;
    }

    static int32_t play(SoundImpl *self, uint32_t fadeMs, uint32_t seek) {
        int32_t st = self->state;
        if (st == 1) {
            DynamicValueFn::setTarget(&self->playFade, 1.0f, int32_t(fadeMs));
            uint32_t fn = ld32(ld32(rw::guest_addr(self)) + 0x14);
            self->seekTime = seek;
            self->playTime = self->playTime + seek;
            if (rw::call(fn, {rw::guest_addr(self)}) == 0) {
                self->state = 2;
                return 0;
            }
            return corePlay(self);
        }
        uint32_t id = uint32_t(int32_t(self->id));
        switch (st) {
        case 0: log(5, 0xEED254, id); return -1;  // "Play sound is not ready[%d]"
        case 2: log(4, 0xEED270, id); return 0;   // "... already play waiting[%d]"
        case 3: log(4, 0xEED298, id); return 0;   // "... already playing[%d]"
        case 4: log(4, 0xEED2BC, id); return 0;   // "... already finish waiting[%d]"
        case 5: log(4, 0xEED2E8, id); return 0;   // "... already finished[%d]"
        default: return 0;
        }
    }

    // void in the original (r0 is whatever the last step left; callers ignore it)
    static void update(SoundImpl *self, uint32_t dt) {
        int32_t st = self->state;
        if (st == 5 || uint32_t(st) <= 1) return;
        uint32_t v = self->voice;
        if (v != 0) {
            if (vcallo(v, 0x2C, {v}) == 1) {  // finished
                self->state = 5;
                return;
            }
        }
        if (self->pauseState != 2) {
            uint32_t pan = self->layoutEnabled;
            if (pan != 0) {
                pan = 1;
                updateLayoutPanning(self);
            }
            uint32_t vol = needUpdate(self->playFade);
            if (vol == 1) DynamicValueFn::update(&self->playFade, int32_t(dt));
            else vol = pan;
            if (needUpdate(self->fadeVolume) == 1) { DynamicValueFn::update(&self->fadeVolume, int32_t(dt)); vol = 1; }
            if (needUpdate(self->volume) == 1) { DynamicValueFn::update(&self->volume, int32_t(dt)); vol = 1; }
            if (needUpdate(self->pauseFade) == 1) { DynamicValueFn::update(&self->pauseFade, int32_t(dt)); vol = 1; }
            if (needUpdate(self->categoryVolume) == 1) { DynamicValueFn::update(&self->categoryVolume, int32_t(dt)); vol = 1; }
            uint32_t pitch = needUpdate(self->pitch);
            if (pitch == 1) DynamicValueFn::update(&self->pitch, int32_t(dt));
            else pitch = 0;
            if (needUpdate(self->pan) == 1) { DynamicValueFn::update(&self->pan, int32_t(dt)); pan = 1; }
            if (needUpdate(self->categoryPitch) == 1) { DynamicValueFn::update(&self->categoryPitch, int32_t(dt)); pitch = 1; }
            if (vol == 1) updateVolume(self);
            if (pitch == 1) updatePitch(self);
            if (pan == 1) updatePan(self);
        }
        if (self->state == 2) {
            if (vcallo(rw::guest_addr(self), 0x14, {rw::guest_addr(self)}) != 0) corePlay(self);
        }
        if (self->pauseState == 1) {
            if (!(value(self->pauseFade) > 0.0f)) {
                v = self->voice;
                if (v != 0) vcallo(v, 0x20, {v});  // pause
                self->pauseState = 2;
            }
        }
        if (self->state == 4) {
            if (!(value(self->playFade) > 0.0f)) {
                v = self->voice;
                if (v == 0) goto tail;
                vcallo(v, 0x10, {v});  // Voice::Stop
            }
        }
        v = self->voice;
        if (v != 0) vcallo(v, 0x28, {v, dt});  // Voice::Update
    tail:
        if (self->state == 2) return;
        if (self->pauseState != 2) self->playTime = self->playTime + dt;
    }
};
RW_FUNCTION(0xCDD880, "SQEX::SoundImpl::SoundImpl", SoundImplFn::construct);
RW_FUNCTION(0xCDD970, "SQEX::SoundImpl::GetState", SoundImplFn::getState);
RW_FUNCTION(0xCDDA08, "SQEX::SoundImpl::Stop", SoundImplFn::stop);
RW_FUNCTION(0xCDDAC0, "SQEX::SoundImpl::Destroy", SoundImplFn::destroy);
RW_FUNCTION(0xCDDD14, "SQEX::SoundImpl::UpdateVolume", SoundImplFn::updateVolume);
RW_FUNCTION(0xCDDEE0, "SQEX::SoundImpl::SetVolume", SoundImplFn::setVolume);
RW_FUNCTION(0xCDDFE8, "SQEX::SoundImpl::UpdatePitch", SoundImplFn::updatePitch);
RW_FUNCTION(0xCDE0F8, "SQEX::SoundImpl::UpdatePan", SoundImplFn::updatePan);
RW_FUNCTION(0xCDE16C, "SQEX::SoundImpl::UpdateLayoutPanning", SoundImplFn::updateLayoutPanning);
RW_FUNCTION(0xCDE348, "SQEX::SoundImpl::CorePlay", SoundImplFn::corePlay);
RW_FUNCTION(0xCDE8B0, "SQEX::SoundImpl::Play", SoundImplFn::play);
RW_FUNCTION(0xCDEA30, "SQEX::SoundImpl::Update", SoundImplFn::update);

}  // namespace sqex
