// SQEX::AttributeSystem::Execute: the sound's attribute rules (akb attribute data) against the sounds playing -
// how many "same group" sounds already play, which rule matches, and the result command (stop the oldest / the
// quietest / the lowest priority, duck their volume, or refuse to play) - then the global sound limit.
#include "Sqex.hpp"

using namespace b2;

namespace {

constexpr uint32_t AKB_GET_ATTRIBUTE_DATA = 0xCDC61C;   // akbSoundGetAttributeData(sound data)
constexpr uint32_t AKB_GET_SOUND_ID = 0xCDC65C;
constexpr uint32_t AKB_GET_VOLUME = 0xCDC698;           // float
constexpr uint32_t AKB_GET_PRIORITY = 0xCDC6D4;
constexpr uint32_t ATTR_GET_RESULT_COMMAND = 0xCDD00C;  // (attribute data, matched rule mark)
constexpr uint32_t ATTR_INT_CMP = 0xCDD03C;             // (op, a, b)
constexpr uint32_t ATTR_FLOAT_CMP = 0xCDD0D8;           // (op, float a, b)
constexpr uint32_t SOUND_GET_STATE = 0xCDD970;
constexpr uint32_t SOUND_GET_LOCAL_STATE = 0xCDD9B8;
constexpr uint32_t SOUND_STOP = 0xCDDA08;               // (sound, fade frames)
constexpr uint32_t SOUND_SET_ATTR_VOLUME = 0xCDDF38;    // (sound, float volume, time)
constexpr uint32_t MANAGER_GET_COUNT = 0xCDEDE4;
constexpr uint32_t MANAGER_BEGIN = 0xCDF088;            // (iterator* out): the first node of the playing sounds
constexpr uint32_t MANAGER_END = 0xCDF0A0;              // (iterator* out)
constexpr uint32_t GET_SOUND_LIMIT = 0xCE1314;          // SoundSystem::GetSoundLimit
constexpr uint32_t DYNAMIC_VALUE_GET = 0xCF4418;        // DynamicValue::GetValue (float)

// SoundImpl
constexpr uint32_t V_SOUND_DATA = 0xC;                  // virtual: the akb sound data
constexpr uint32_t MARK = 0xB5;                         // int8: the rule round that matched it (0: none)
constexpr uint32_t CATEGORY = 0xB8;
constexpr uint32_t HAS_GROUP = 0xC4;                    // uint8
constexpr uint32_t GROUP = 0xC6;                        // int16

uint32_t soundData(uint32_t s) { return rw::call(ld32(ld32(s) + V_SOUND_DATA), {s}); }
float value(uint32_t dv) { return bitsf(rw::call(DYNAMIC_VALUE_GET, {dv})); }

// the sound's effective volume: its fades and volumes times the akb volume and the mixer factors
float volumeOf(uint32_t s) {
    float v = value(s + 0x48);
    v = v * value(s + 0x20);
    v = v * value(s + 0x34);
    v = v * value(s + 0xC);
    v = v * bitsf(rw::call(AKB_GET_VOLUME, {soundData(s)}));
    v = v * value(s + 0x84);
    v = v * ldf(s + 0xBC);
    v = v * ldf(s + 0xCC);
    v = v * ldf(s + 0xEC);
    return v;
}

// every playing sound in list order (a node: +0x14 the sound, +0x10 bit 0 has-next, +8 next); `body` returns
// true to stop early
template <class F> void forEachSound(F body) {
    guest::Frame f(8);
    rw::call(MANAGER_BEGIN, {f.addr(4)});
    uint32_t node = ld32(f.addr(4));
    for (;;) {
        rw::call(MANAGER_END, {f.addr(4)});
        if (node == ld32(f.addr(4))) return;
        if (body(node)) return;
        node = (ld8(node + 0x10) & 1) ? ld32(node + 8) : 0;
    }
}

bool sameGroup(uint32_t a, uint32_t b) {
    return ld8(a + HAS_GROUP) && ld8(b + HAS_GROUP) && lds16(a + GROUP) == lds16(b + GROUP);
}

}  // namespace

namespace AttributeSystemFn {

// Returns 1 when the sound may play, 0 when a rule or the sound limit refuses it.
int32_t execute(uint32_t self) {
    const uint32_t mine = soundData(self);
    const uint32_t attr = rw::call(AKB_GET_ATTRIBUTE_DATA, {mine});
    int32_t result = 1;
    if (attr) do {
        // 1. the playing sounds of the same kind (filter flags at +6) get mark 1; count them
        uint32_t count = 0;
        forEachSound([&](uint32_t node) {
            st8(ld32(node + 0x14) + MARK, 0);
            const uint32_t s = ld32(node + 0x14);
            if (self == s) return false;
            if (rw::call(SOUND_GET_LOCAL_STATE, {s}) != 3) return false;
            const uint32_t other = soundData(s);
            uint8_t flags = ld8(attr + 6);
            if (flags & 1) {                                      // same attribute id
                const uint32_t oa = rw::call(AKB_GET_ATTRIBUTE_DATA, {other});
                if (oa && ld16(attr + 4) != ld16(oa + 2)) return false;
                flags = ld8(attr + 6);
            }
            if (flags & 2) {                                      // sound id in [+8, +0xC]
                if (rw::call(AKB_GET_SOUND_ID, {other}) < ld32(attr + 8)) return false;
                const uint32_t hi = ld32(attr + 0xC);
                if (hi < rw::call(AKB_GET_SOUND_ID, {other})) return false;
                flags = ld8(attr + 6);
            }
            if (flags & 4) {                                      // the same sound
                const uint32_t a = rw::call(AKB_GET_SOUND_ID, {mine});
                if (a != rw::call(AKB_GET_SOUND_ID, {other})) return false;
                flags = ld8(attr + 6);
            }
            if (flags & 8) {                                      // the same thousand
                const uint32_t a = rw::call(AKB_GET_SOUND_ID, {mine});
                const uint32_t b = rw::call(AKB_GET_SOUND_ID, {other});
                if (b / 1000 != a / 1000) return false;
                flags = ld8(attr + 6);
            }
            if ((flags & 0x10) && !sameGroup(self, ld32(node + 0x14))) return false;   // the same group
            const uint32_t m = ld32(node + 0x14);
            st8(m + MARK, uint8_t(ld8(m + MARK) + 1));
            count++;
            return false;
        });
        if (count == 0) break;

        // 2. the rules (byte +7 of them, 0x18 bytes each from +0x1B): a sound marked so far that meets every
        // condition (or a condition joined by "or") is marked again; the last rule round that marked any sound
        // decides the result command
        int32_t mark = 1;
        const uint32_t rules = ld8(attr + 7);
        uint32_t rule = attr + 0x1B;
        for (uint32_t k = 0; k < rules; k++, rule += 0x18) {
            uint32_t matched = 0;
            forEachSound([&](uint32_t node) {
                const uint32_t s = ld32(node + 0x14);
                if (int8_t(ld8(s + MARK)) <= 0) return false;
                const uint32_t n = ld8(rule + 4);
                if (n == 0) return false;
                uint32_t ok = 0, prev = 0;
                for (uint32_t i = 0; i < n; i++) {
                    const uint32_t op = ld8(rule + 1 + i);
                    const uint32_t arg = ld32(rule + 5 + 4 * i);
                    switch (op & 0xF) {
                    case 0: ok = 1; break;
                    case 1: ok = rw::call(ATTR_INT_CMP, {op, ld32(s + CATEGORY), arg}) == 1; break;
                    case 2: ok = rw::call(ATTR_FLOAT_CMP, {op, fbits(volumeOf(s)), arg}) == 1; break;
                    case 4: ok = rw::call(ATTR_INT_CMP, {op, count, arg}) == 1; break;
                    case 5:
                        ok = rw::call(ATTR_INT_CMP, {op, rw::call(AKB_GET_PRIORITY, {soundData(s)}), arg}) == 1;
                        break;
                    case 6:
                        ok = ld8(s + HAS_GROUP) && ld8(self + HAS_GROUP) &&
                             rw::call(ATTR_INT_CMP, {op, uint32_t(int32_t(lds16(s + GROUP))),
                                                     uint32_t(int32_t(lds16(self + GROUP)))}) == 1;
                        break;
                    default: ok = 0; break;
                    }
                    if (i != 0 && ld8(rule + 2 + i) == 0) ok &= prev;   // 0 after a condition: "and"
                    prev = ok;
                }
                if (ok) {
                    matched++;
                    const uint32_t m = ld32(node + 0x14);
                    st8(m + MARK, uint8_t(ld8(m + MARK) + 1));
                }
                return false;
            });
            if (matched) mark = int32_t(k) + 2;
            count = matched;
        }

        // 3. the result command for the mark
        const uint32_t cmd = rw::call(ATTR_GET_RESULT_COMMAND, {attr, uint32_t(mark)});
        if (!cmd) break;
        auto marked = [&](uint32_t node) { return int8_t(ld8(ld32(node + 0x14) + MARK)) == mark; };
        switch (ld8(cmd + 1)) {
        case 1:                                                   // stop them all
            forEachSound([&](uint32_t node) {
                if (marked(node)) rw::call(SOUND_STOP, {ld32(node + 0x14), 0});
                return false;
            });
            break;
        case 3:                                                   // fade them all out
            forEachSound([&](uint32_t node) {
                if (marked(node)) rw::call(SOUND_STOP, {ld32(node + 0x14), ld32(cmd + 8)});
                return false;
            });
            break;
        case 5: {                                                 // stop the lowest priority one
            int32_t best = 0x100;
            uint32_t pick = 0;
            forEachSound([&](uint32_t node) {
                if (!marked(node)) return false;
                const int32_t p = int32_t(rw::call(AKB_GET_PRIORITY, {soundData(ld32(node + 0x14))}));
                if (best > p) {
                    best = p;
                    pick = ld32(node + 0x14);
                }
                return false;
            });
            if (pick) rw::call(SOUND_STOP, {pick, 0});
            break;
        }
        case 7: {                                                 // duck them: volume % (low byte), time (the rest)
            const int32_t v = int32_t(ld32(cmd + 8));
            const float volume = float(int32_t(uint32_t(v) & 0xFF)) * 0.01f;
            const uint32_t time = uint32_t(v >> 8);
            forEachSound([&](uint32_t node) {
                if (marked(node)) rw::call(SOUND_SET_ATTR_VOLUME, {ld32(node + 0x14), fbits(volume), time});
                return false;
            });
            break;
        }
        case 8:                                                   // stop the first (oldest) one
            forEachSound([&](uint32_t node) {
                if (!marked(node)) return false;
                rw::call(SOUND_STOP, {ld32(node + 0x14), 0});
                return true;
            });
            break;
        case 13: {                                                // stop the quietest one
            float best = 1.1f;
            uint32_t pick = 0;
            forEachSound([&](uint32_t node) {
                if (!marked(node)) return false;
                const uint32_t s = ld32(node + 0x14);
                const float v = volumeOf(s);
                if (best > v) {
                    best = v;
                    pick = ld32(node + 0x14);
                }
                return false;
            });
            if (pick) rw::call(SOUND_STOP, {pick, 0});
            break;
        }
        default: break;
        }
        // what happens to this sound
        const uint8_t own = ld8(cmd);
        if (own == 7) {                                           // refused
            result = 0;
        } else if (own == 10) {                                   // plays at the given volume
            rw::call(SOUND_SET_ATTR_VOLUME, {self, ld32(cmd + 4), 0});
        } else if (own == 1) {                                    // fades in over the given time
            rw::call(SOUND_SET_ATTR_VOLUME, {self, 0, 0});
            rw::call(SOUND_SET_ATTR_VOLUME, {self, fbits(1.0f), ld32(cmd + 4)});
        }
    } while (false);

    // 4. the global sound limit: refuse when more than the limit are playing
    const int32_t limit = int32_t(rw::call(GET_SOUND_LIMIT, {}));
    if (limit > 0 && limit < int32_t(rw::call(MANAGER_GET_COUNT, {uint32_t(limit)}))) {
        int32_t playing = 0;
        forEachSound([&](uint32_t node) {
            if (rw::call(SOUND_GET_STATE, {ld32(node + 0x14)}) != 3 && rw::call(SOUND_GET_STATE, {ld32(node + 0x14)}) != 0)
                playing++;
            return false;
        });
        if (limit < playing) result = 0;
    }
    return result;
}

}  // namespace AttributeSystemFn

RW_FUNCTION(0xCF3768, "SQEX::AttributeSystem::Execute", AttributeSystemFn::execute);
