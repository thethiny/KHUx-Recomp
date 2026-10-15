// Sound / Category: the game-side handles of the SQEX sound library. A Sound is a sound handle; its requests are
// queued as SQEX::ACTIONs (applied by the sound thread), its queries go to the SoundManager directly. A Category
// is a category id resolved through the CategoryManager.
#include "SystemCommon.hpp"

namespace sound {

namespace {
constexpr uint32_t ADD_ACTION = 0xCDD430;               // SQEX::ActionManager::AddAction(ACTION&) -> int8 result
constexpr uint32_t GET_SOUND_VOLUME = 0xCDEEC0;         // SQEX::SoundManager::GetSoundVolume(handle)
constexpr uint32_t GET_SOUND_STATE = 0xCDEE70;          // SQEX::SoundManager::GetSoundState(handle)
constexpr uint32_t GET_SOUND_IS_LOOP = 0xCDEF9C;
constexpr uint32_t GET_SOUND_PLAY_TIME = 0xCDEFEC;
constexpr uint32_t GET_SOUND_ELAPSED = 0xCDF03C;        // GetSoundElapsedPlaybackTime
constexpr uint32_t PRINT_DEBUG_INFO = 0xCE03C0;         // SQEX::SoundManager::PrintDebugInfo(handle)
constexpr uint32_t GET_CATEGORY = 0xCE5DAC;             // SQEX::CategoryManager::GetCategory(id) -> CategoryImpl*
constexpr uint32_t CATEGORY_USING_COUNT = 0xCE5BBC;
constexpr uint32_t CATEGORY_STOP = 0xCE5444;
constexpr uint32_t CATEGORY_PAUSE_ON = 0xCE5AB8;
constexpr uint32_t CATEGORY_PAUSE_OFF = 0xCE5B38;
constexpr uint32_t CATEGORY_SET_VOLUME = 0xCE54A0;
constexpr uint32_t CATEGORY_GET_VOLUME = 0xCE54B4;
constexpr uint32_t ANDROID_LOG_PRINT = 0x413D38;
constexpr uint32_t STR_LOG_TAG = 0xEECB88;              // GetStatus: log tag
constexpr uint32_t STR_UNKNOWN_STATE = 0xEED888;        // GetStatus: "unknown state %d"-style message

// SQEX::ACTION types
enum Action : uint32_t {
    PLAY = 4, STOP = 5, PAUSE_ON = 6, PAUSE_OFF = 7, SET_VOLUME = 8, SET_PITCH = 9, SET_PAN = 10,
    SET_RANDOM_VOLUME = 11, SET_RANDOM_PITCH = 12, SET_CATEGORY_VOLUME = 13, SET_POSITION = 14,
};

// Queue an ACTION { type, handle, params... }. The 40-byte action is built in the original's frame (entry
// sp - 0x30): its unset words are stale stack bytes that AddAction copies into the queue.
int32_t addAction(uint32_t self, Action type, std::initializer_list<uint32_t> params) {
    sys::ExactFrame f(0x30);
    const uint32_t action = f.addr();
    sys::st32(action, type);
    sys::st32(action + 4, sys::ld32(self));
    uint32_t k = 8;
    for (uint32_t p : params) { sys::st32(action + k, p); k += 4; }
    return int32_t(rw::call(ADD_ACTION, {action}));
}
}  // namespace

// ------------------------------------------------------------------------------------------------ Sound
uint32_t construct(uint32_t self) { sys::st32(self, 0); return self; }
RW_FUNCTION(0xCE1328, "Sound::Sound()", construct);

uint32_t constructWithHandle(uint32_t self, uint32_t handle) { sys::st32(self, handle); return self; }
RW_FUNCTION(0xCE1334, "Sound::Sound(int)", constructWithHandle);

int32_t Play(uint32_t self, uint32_t fade, uint32_t delay) { return addAction(self, PLAY, {fade, delay}); }
RW_FUNCTION(0xCE133C, "Sound::Play", Play);

int32_t Stop(uint32_t self, uint32_t fade) { return addAction(self, STOP, {fade}); }
RW_FUNCTION(0xCE1368, "Sound::Stop", Stop);

int32_t PauseOn(uint32_t self, uint32_t fade) { return addAction(self, PAUSE_ON, {fade}); }
RW_FUNCTION(0xCE1390, "Sound::PauseOn", PauseOn);

int32_t PauseOff(uint32_t self, uint32_t fade) { return addAction(self, PAUSE_OFF, {fade}); }
RW_FUNCTION(0xCE13B8, "Sound::PauseOff", PauseOff);

// (float parameters travel as their bits)
int32_t SetVolume(uint32_t self, uint32_t volume, uint32_t fade) { return addAction(self, SET_VOLUME, {volume, fade}); }
RW_FUNCTION(0xCE13E0, "Sound::SetVolume", SetVolume);

int32_t SetPitch(uint32_t self, uint32_t pitch, uint32_t fade) { return addAction(self, SET_PITCH, {pitch, fade}); }
RW_FUNCTION(0xCE140C, "Sound::SetPitch", SetPitch);

int32_t SetPan(uint32_t self, uint32_t pan, uint32_t fade) { return addAction(self, SET_PAN, {pan, fade}); }
RW_FUNCTION(0xCE1438, "Sound::SetPan", SetPan);

int32_t SetRandomVolume(uint32_t self, uint32_t lo, uint32_t hi, uint32_t fade) {
    return addAction(self, SET_RANDOM_VOLUME, {lo, hi, fade});
}
RW_FUNCTION(0xCE1464, "Sound::SetRandomVolume", SetRandomVolume);

int32_t SetRandomPitch(uint32_t self, uint32_t lo, uint32_t hi, uint32_t fade) {
    return addAction(self, SET_RANDOM_PITCH, {lo, hi, fade});
}
RW_FUNCTION(0xCE1498, "Sound::SetRandomPitch", SetRandomPitch);

int32_t SetCategoryVolume(uint32_t self, uint32_t volume) { return addAction(self, SET_CATEGORY_VOLUME, {volume}); }
RW_FUNCTION(0xCE14CC, "Sound::SetCategoryVolume", SetCategoryVolume);

uint32_t GetVolume(uint32_t self) { return rw::call(GET_SOUND_VOLUME, {sys::ld32(self)}); }
RW_FUNCTION(0xCE14F4, "Sound::GetVolume", GetVolume);

// The library's state -> the game's Sound status (unknown states are logged and read as 0).
uint32_t GetStatus(uint32_t self) {
    uint32_t state = rw::call(GET_SOUND_STATE, {sys::ld32(self)});
    switch (state) {
    case 0: return 0;
    case 1: return 2;
    case 2: return 3;
    case 3: return 5;
    case 4: return 4;
    default:
        rw::call(ANDROID_LOG_PRINT, {5, STR_LOG_TAG, STR_UNKNOWN_STATE, state});
        return 0;
    }
}
RW_FUNCTION(0xCE14FC, "Sound::GetStatus", GetStatus);

uint32_t IsLoop(uint32_t self) { return rw::call(GET_SOUND_IS_LOOP, {sys::ld32(self)}); }
RW_FUNCTION(0xCE1574, "Sound::IsLoop", IsLoop);

uint32_t GetPlayTime(uint32_t self) { return rw::call(GET_SOUND_PLAY_TIME, {sys::ld32(self)}); }
RW_FUNCTION(0xCE157C, "Sound::GetPlayTime", GetPlayTime);

uint32_t GetElapsedPlaybackTime(uint32_t self) { return rw::call(GET_SOUND_ELAPSED, {sys::ld32(self)}); }
RW_FUNCTION(0xCE1584, "Sound::GetElapsedPlaybackTime", GetElapsedPlaybackTime);

int32_t SetPosition(uint32_t self, uint32_t x, uint32_t y, uint32_t z) {
    return addAction(self, SET_POSITION, {x, y, z});
}
RW_FUNCTION(0xCE158C, "Sound::SetPosition", SetPosition);

uint32_t PrintDebugInfo(uint32_t self) { return rw::call(PRINT_DEBUG_INFO, {sys::ld32(self)}); }
RW_FUNCTION(0xCE15C0, "Sound::PrintDebugInfo", PrintDebugInfo);

// ------------------------------------------------------------------------------------------------ Category
uint32_t categoryConstruct(uint32_t self, uint32_t id) { sys::st32(self, id); return self; }
RW_FUNCTION(0xCE15C8, "Category::Category(int)", categoryConstruct);

uint32_t GetUsingCount(uint32_t self) {
    uint32_t cat = rw::call(GET_CATEGORY, {sys::ld32(self)});
    return cat ? rw::call(CATEGORY_USING_COUNT, {cat}) : 0;
}
RW_FUNCTION(0xCE15D0, "Category::GetUsingCount", GetUsingCount);

uint32_t CategoryStop(uint32_t self, uint32_t fade) {
    uint32_t cat = rw::call(GET_CATEGORY, {sys::ld32(self)});
    return cat ? rw::call(CATEGORY_STOP, {cat, fade}) : 0xFFFFFFFFu;
}
RW_FUNCTION(0xCE15EC, "Category::Stop", CategoryStop);

uint32_t CategoryPauseOn(uint32_t self, uint32_t fade) {
    uint32_t cat = rw::call(GET_CATEGORY, {sys::ld32(self)});
    return cat ? rw::call(CATEGORY_PAUSE_ON, {cat, fade}) : 0xFFFFFFFFu;
}
RW_FUNCTION(0xCE1618, "Category::PauseOn", CategoryPauseOn);

uint32_t CategoryPauseOff(uint32_t self, uint32_t fade) {
    uint32_t cat = rw::call(GET_CATEGORY, {sys::ld32(self)});
    return cat ? rw::call(CATEGORY_PAUSE_OFF, {cat, fade}) : 0xFFFFFFFFu;
}
RW_FUNCTION(0xCE1644, "Category::PauseOff", CategoryPauseOff);

uint32_t CategorySetVolume(uint32_t self, uint32_t volume, uint32_t fade) {
    uint32_t cat = rw::call(GET_CATEGORY, {sys::ld32(self)});
    return cat ? rw::call(CATEGORY_SET_VOLUME, {cat, volume, fade}) : 0xFFFFFFFFu;
}
RW_FUNCTION(0xCE1670, "Category::SetVolume", CategorySetVolume);

uint32_t CategoryGetVolume(uint32_t self) {
    uint32_t cat = rw::call(GET_CATEGORY, {sys::ld32(self)});
    return cat ? rw::call(CATEGORY_GET_VOLUME, {cat}) : 0;
}
RW_FUNCTION(0xCE16A4, "Category::GetVolume", CategoryGetVolume);

uint32_t GetMaxCount(uint32_t self) {
    uint32_t cat = rw::call(GET_CATEGORY, {sys::ld32(self)});
    return cat ? sys::ld32(cat + 0x34) : 0;
}
RW_FUNCTION(0xCE16C8, "Category::GetMaxCount", GetMaxCount);

}  // namespace sound
