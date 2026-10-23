// Batch #10: the rest of SQEX::SoundImpl (category volume, random volume / pitch, debug print) and the
// SoundManager's per-sound volume and debug print.
#include "../w4_b2/Sqex.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t LRAND48 = 0x434738;   // (import)
constexpr uint32_t DV_GET_VALUE = 0xCF4418, DV_GET_TARGET = 0xCF44F8, DV_GET_REMAIN_TIME = 0xCF44E8,
                   DV_SET_TARGET = 0xCF4450;
constexpr uint32_t SOUND_IMPL_UPDATE_VOLUME = 0xCDDD14, SOUND_IMPL_UPDATE_PITCH = 0xCDDFE8,
                   SOUND_IMPL_PRINT_DEBUG_INFO = 0xCDDB3C;
constexpr uint32_t SOUND_MANAGER_GET_SOUND = 0xCDEDF8;
constexpr uint32_t AKB_SOUND_GET_VOLUME = 0xCDC698;
constexpr uint32_t S_SOUND_INFO = 0xEED0CC;     // "Sound[id:%d][type:%d]"
constexpr uint32_t S_STATE = 0xEED0E4;          // "-State %d:%d"
constexpr uint32_t S_VOL_MAIN = 0xEED0F4, S_VOL_ATTR = 0xEED110, S_VOL_FADE = 0xEED12C, S_VOL_PAUSE = 0xEED148;
constexpr uint32_t S_VOICE = 0xEED168;          // "-Voice %p"
constexpr uint32_t S_SOUND_NOT_FOUND = 0xEED3F4; // "Sound[%d] not find"

float value(uint32_t dynamicValue) { return bitsf(rw::call(DV_GET_VALUE, {dynamicValue})); }

// a random value between to and from: to + (lrand48() % 101) * 0.01 * (from - to), the fade's new target
void setRandom(uint32_t dynamicValue, uint32_t fromBits, uint32_t toBits, uint32_t ms) {
    const float from = bitsf(fromBits), to = bitsf(toBits);
    const int32_t roll = int32_t(rw::call(LRAND48, {})) % 101;
    const float step = float(roll) * 0.01f;
    const float product = step * (from - to);
    rw::call(DV_SET_TARGET, {dynamicValue, fbits(to + product), ms});
}

// "-Vol <name>[n:%f][t:%f][r:%d]": a DynamicValue's value, target and remaining time
void logDynamicValue(uint32_t format, uint32_t dynamicValue) {
    const double now = bitsf(rw::call(DV_GET_VALUE, {dynamicValue}));
    const double target = bitsf(rw::call(DV_GET_TARGET, {dynamicValue}));
    const uint32_t remain = rw::call(DV_GET_REMAIN_TIME, {dynamicValue});
    uint64_t n, t;
    std::memcpy(&n, &now, 8);
    std::memcpy(&t, &target, 8);
    // the doubles go 8-aligned on the stack, so r3 is skipped
    rw::call(ANDROID_LOG, {4, LOG_TAG, format, 0, uint32_t(n), uint32_t(n >> 32), uint32_t(t), uint32_t(t >> 32),
                           remain});
}

}  // namespace

struct SoundImplMore {
    static uint32_t setCategoryVolume(uint32_t sound, uint32_t volumeBits) {
        if (ldf(sound + 0xCC) != bitsf(volumeBits)) {
            st32(sound + 0xCC, volumeBits);
            rw::call(SOUND_IMPL_UPDATE_VOLUME, {sound});
        }
        return 0;
    }
    static uint32_t setRandomVolume(uint32_t sound, uint32_t from, uint32_t to, uint32_t ms) {
        setRandom(sound + 0x84, from, to, ms);
        if (ms == 0) rw::call(SOUND_IMPL_UPDATE_VOLUME, {sound});
        return 0;
    }
    static uint32_t setRandomPitch(uint32_t sound, uint32_t from, uint32_t to, uint32_t ms) {
        setRandom(sound + 0x98, from, to, ms);
        if (ms == 0) rw::call(SOUND_IMPL_UPDATE_PITCH, {sound});
        return 0;
    }
    static uint32_t printDebugInfo(uint32_t sound) {
        const uint32_t id = ld32(sound + 8);
        const uint32_t type = vcallo(sound, 8, {sound});
        rw::call(ANDROID_LOG, {4, LOG_TAG, S_SOUND_INFO, id, type});
        rw::call(ANDROID_LOG, {4, LOG_TAG, S_STATE, ld32(sound + 0xAC), ld32(sound + 0xB0)});
        logDynamicValue(S_VOL_MAIN, sound + 0x48);
        logDynamicValue(S_VOL_ATTR, sound + 0xC);
        logDynamicValue(S_VOL_FADE, sound + 0x20);
        logDynamicValue(S_VOL_PAUSE, sound + 0x34);
        return rw::call(ANDROID_LOG, {4, LOG_TAG, S_VOICE, ld32(sound + 4)});
    }

    // the volume sound `id` plays at: every fade, the AKB volume, the random / category / attribute / layout
    // factors (0 for an unknown sound)
    static uint32_t getSoundVolume(uint32_t id) {
        mutexLock(SOUND_MUTEX);
        float v = 0.0f;
        const uint32_t s = rw::call(SOUND_MANAGER_GET_SOUND, {id});
        if (s != 0) {
            float a = value(s + 0x48) * value(s + 0x20);
            a = a * value(s + 0x34);
            a = a * value(s + 0xC);
            const uint32_t akbSound = vcallo(s, 0xC, {s});
            a = a * bitsf(rw::call(AKB_SOUND_GET_VOLUME, {akbSound}));
            v = a * value(s + 0x84);
            v = v * ldf(s + 0xBC);
            v = v * ldf(s + 0xCC);
            v = v * ldf(s + 0xEC);
        }
        mutexUnlock(SOUND_MUTEX);
        return fbits(v);
    }
    static int32_t printSoundDebugInfo(uint32_t id) {
        mutexLock(SOUND_MUTEX);
        int32_t r = 0;
        const uint32_t s = rw::call(SOUND_MANAGER_GET_SOUND, {id});
        if (s != 0) {
            rw::call(SOUND_IMPL_PRINT_DEBUG_INFO, {s});
        } else {
            log(4, S_SOUND_NOT_FOUND, id);
            r = -1;
        }
        mutexUnlock(SOUND_MUTEX);
        return r;
    }
};
RW_FUNCTION(0xCDDB3C, "SQEX::SoundImpl::PrintDebugInfo", SoundImplMore::printDebugInfo);
RW_FUNCTION(0xCDDF0C, "SQEX::SoundImpl::SetCategoryVolume", SoundImplMore::setCategoryVolume);
RW_FUNCTION(0xCDDF64, "SQEX::SoundImpl::SetRandomVolume", SoundImplMore::setRandomVolume);
RW_FUNCTION(0xCDE074, "SQEX::SoundImpl::SetRandomPitch", SoundImplMore::setRandomPitch);
RW_FUNCTION(0xCDEEC0, "SQEX::SoundManager::GetSoundVolume", SoundImplMore::getSoundVolume);
RW_FUNCTION(0xCE03C0, "SQEX::SoundManager::PrintDebugInfo", SoundImplMore::printSoundDebugInfo);

}  // namespace sqex
