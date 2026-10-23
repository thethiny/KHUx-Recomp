// Batch #10: SQEX's container teardown (FixedList / FixedQueue / List destructors, ActionManager::Release,
// SoundManager::Release), the SoundManager's all-sound operations, the old C API (SoundSystem_CreateSound,
// Sound_*), Thread::Join and the DelegateManager.
//
// A FixedList is a SQEX pool (see w4_b2/SqexPool.hpp): {capacity, count, slots, free, used, tail}; its slot array
// has an 8-byte header {element size, count} and is destroyed from the last element down.
#include "../w4_b2/SqexPool.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t ACTION_SIZE = 0x30;
constexpr uint32_t DELEGATES = 0x100EB7C;   // {quit u8, mutex +4, running +8, count +0xC, IDelegate*[16] +0x10,
                                            //  cond +0x50, thread +0x54}
constexpr uint32_t COND_BROADCAST = 0x44CF3C;   // pthread_cond_broadcast (import)
constexpr uint32_t PTHREAD_JOIN = 0x446708;     // pthread_join (import)
constexpr uint32_t THREAD_JOIN = 0xCE23F4;
constexpr uint32_t SOUND_IMPL_STOP = 0xCDDA08, SOUND_IMPL_PAUSE_ON = 0xCDDDD0, SOUND_IMPL_PAUSE_OFF = 0xCDDE60;
constexpr uint32_t SOUND_DEFAULT_CTOR = 0xCE1328, SOUND_CTOR = 0xCE1334;
constexpr uint32_t SOUND_PLAY = 0xCE133C, SOUND_STOP = 0xCE1368, SOUND_PAUSE_ON = 0xCE1390,
                   SOUND_PAUSE_OFF = 0xCE13B8, SOUND_SET_VOLUME = 0xCE13E0, SOUND_SET_PITCH = 0xCE140C,
                   SOUND_GET_STATUS = 0xCE14FC;
constexpr uint32_t SOUND_SYSTEM_CREATE_SOUND = 0xCE0A20;   // SoundSystem::CreateSound(Sound*, const uint8_t*, int)
constexpr uint32_t S_DELEGATE_FULL = 0xEEE264;             // "Delegate full!!"

// an element array with an 8-byte header {element size, count}: each element's destructor (vtable slot 0), last
// first, then the block; `holder` is re-read after every destructor
void destroyArray(uint32_t holder, uint32_t elementSize) {
    const uint32_t array = ld32(holder);
    if (array == 0) return;
    uint32_t p = array + ld32(array - 4) * elementSize;
    while (ld32(holder) != p) {
        p -= elementSize;
        rw::call(ld32(ld32(p)), {p});
    }
    free_(ld32(holder) - 8);
}

// a FixedList's teardown: the slots, then every field cleared
uint32_t releaseList(uint32_t list) {
    destroyArray(list + pool::SLOTS, 24);
    st32(list + pool::SLOTS, 0);
    st32(list + pool::FREE, 0);
    st32(list + pool::USED, 0);
    st32(list + pool::TAIL, 0);
    st32(list + pool::COUNT, 0);
    return list;
}

// each used sound of the SoundManager gets `method(sound, ms)`
void forEachSound(uint32_t method, uint32_t ms) {
    for (uint32_t node = ld32(SOUND_MANAGER + pool::USED); node != 0; node = ld32(node + 8)) {
        rw::call(method, {ld32(node + 0x14), ms});
        if (!(ld8(node + 0x10) & 1)) return;
    }
}

template <class... A> uint32_t soundCall(uint32_t handle, uint32_t method, A... args) {
    guest::Frame f(8);
    rw::call(SOUND_CTOR, {f.addr(4), handle});
    return rw::call(method, {f.addr(4), uint32_t(args)...});
}

}  // namespace

struct SoundManagerMore {
    static uint32_t bankListDtor(uint32_t list) { return releaseList(list); }
    static uint32_t soundListDtor(uint32_t list) { return releaseList(list); }
    static void soundManagerRelease() { releaseList(SOUND_MANAGER); }

    // FixedQueue<ACTION>: {array, free, head, tail}
    static uint32_t actionQueueDtor(uint32_t queue) {
        destroyArray(queue, ACTION_SIZE);
        st32(queue, 0);
        st32(queue + 4, 0);
        st32(queue + 8, 0);
        st32(queue + 0xC, 0);
        return queue;
    }
    static uint32_t actionManagerRelease() {
        actionQueueDtor(ACTION_MANAGER);
        return 0;
    }
    // every queued action back to the free list (each node's next at +0x2C)
    static uint32_t actionManagerClearAction() {
        mutexLock(ACTION_MUTEX);
        uint32_t free = ld32(ACTION_MANAGER + 4);
        for (uint32_t node = ld32(ACTION_MANAGER + 8); node != 0;) {
            const uint32_t next = ld32(node + 0x2C);
            st32(node + 0x2C, free);
            free = node;
            node = next;
        }
        st32(ACTION_MANAGER + 8, 0);
        st32(ACTION_MANAGER + 4, free);
        return mutexUnlock(ACTION_MUTEX);
    }

    static void soundManagerStopAll(uint32_t ms) { forEachSound(SOUND_IMPL_STOP, ms); }
    static void soundManagerSuspendAll(uint32_t ms) { forEachSound(SOUND_IMPL_PAUSE_ON, ms); }
    static void soundManagerResumeAll(uint32_t ms) { forEachSound(SOUND_IMPL_PAUSE_OFF, ms); }

    // List<CoreSource*>: {head, tail, count}; each node is unlinked, counted out and deleted (vtable slot 1)
    static uint32_t coreSourceListDtor(uint32_t list) {
        for (uint32_t node = ld32(list); node != 0;) {
            const uint32_t prev = ld32(node + 4), next = ld32(node + 8);
            if (prev != 0) st32(prev + 8, next);
            else st32(list, next);
            if (next != 0) st32(next + 4, prev);
            else st32(list + 4, prev);
            st32(list + 8, ld32(list + 8) - 1);
            rw::call(ld32(ld32(node) + 4), {node});
            node = next;
        }
        return list;
    }

    // the old C API: a sound from bank data, and handle wrappers around SQEX::Sound
    static void soundSystemCreateSound(uint32_t outHandle, uint32_t data) {
        guest::Frame f(8);
        rw::call(SOUND_DEFAULT_CTOR, {f.addr(4)});
        rw::call(SOUND_SYSTEM_CREATE_SOUND, {f.addr(4), data, 0});
        st32(outHandle, *f.at<uint32_t>(4));
    }
    static uint32_t soundPlay(uint32_t h, uint32_t a, uint32_t b) { return soundCall(h, SOUND_PLAY, a, b); }
    static uint32_t soundStop(uint32_t h, uint32_t ms) { return soundCall(h, SOUND_STOP, ms); }
    static uint32_t soundPauseOn(uint32_t h, uint32_t ms) { return soundCall(h, SOUND_PAUSE_ON, ms); }
    static uint32_t soundPauseOff(uint32_t h, uint32_t ms) { return soundCall(h, SOUND_PAUSE_OFF, ms); }
    static uint32_t soundSetVolume(uint32_t h, uint32_t v, uint32_t ms) { return soundCall(h, SOUND_SET_VOLUME, v, ms); }
    static uint32_t soundSetPitch(uint32_t h, uint32_t v, uint32_t ms) { return soundCall(h, SOUND_SET_PITCH, v, ms); }
    static uint32_t soundGetStatus(uint32_t h) { return soundCall(h, SOUND_GET_STATUS); }

    // SQEX::Thread {.., pthread_t +8}: joined and forgotten
    static uint32_t threadJoin(uint32_t thread) {
        guest::Frame f(8);
        rw::call(PTHREAD_JOIN, {ld32(thread + 8), f.addr(4)});
        st32(thread + 8, 0);
        return 0;
    }

    // the delegate thread: told to quit, woken, joined
    static uint32_t delegateRelease() {
        st8(DELEGATES, 1);
        mutexLock(DELEGATES + 4);
        rw::call(COND_BROADCAST, {DELEGATES + 0x50});
        mutexUnlock(DELEGATES + 4);
        return rw::call(THREAD_JOIN, {DELEGATES + 0x54});
    }
    // queues a delegate (at most 16) and wakes the thread: 0, or -1 when full
    static int32_t delegateAdd(uint32_t delegate) {
        mutexLock(DELEGATES + 4);
        int32_t r;
        const int32_t count = lds32(DELEGATES + 0xC);
        if (count > 15) {
            log(5, S_DELEGATE_FULL);
            r = -1;
        } else {
            st32(DELEGATES + 0xC, uint32_t(count + 1));
            st32(DELEGATES + 0x10 + uint32_t(count) * 4, delegate);
            rw::call(COND_BROADCAST, {DELEGATES + 0x50});
            r = 0;
        }
        mutexUnlock(DELEGATES + 4);
        return r;
    }
    // the delegate is running or queued
    static uint32_t delegateIsExist(uint32_t delegate) {
        mutexLock(DELEGATES + 4);
        uint32_t found = ld32(DELEGATES + 8) == delegate;
        const int32_t count = lds32(DELEGATES + 0xC);
        for (int32_t k = 0; !found && k < count; k++) found = ld32(DELEGATES + 0x10 + uint32_t(k) * 4) == delegate;
        mutexUnlock(DELEGATES + 4);
        return found;
    }
};
RW_FUNCTION(0xCD8F84, "SQEX::FixedList<BankImpl*>::~FixedList", SoundManagerMore::bankListDtor);
RW_FUNCTION(0xCDD1D8, "SQEX::FixedQueue<ACTION>::~FixedQueue", SoundManagerMore::actionQueueDtor);
RW_FUNCTION(0xCDD3B4, "SQEX::ActionManager::Release", SoundManagerMore::actionManagerRelease);
RW_FUNCTION(0xCDD4F4, "SQEX::ActionManager::ClearAction", SoundManagerMore::actionManagerClearAction);
RW_FUNCTION(0xCDED78, "SQEX::FixedList<SoundImpl*>::~FixedList", SoundManagerMore::soundListDtor);
RW_FUNCTION(0xCDF228, "SQEX::SoundManager::Release", SoundManagerMore::soundManagerRelease);
RW_FUNCTION(0xCDF2A4, "SQEX::SoundManager::StopAll", SoundManagerMore::soundManagerStopAll);
RW_FUNCTION(0xCDF2EC, "SQEX::SoundManager::SuspendAll", SoundManagerMore::soundManagerSuspendAll);
RW_FUNCTION(0xCDF334, "SQEX::SoundManager::ResumeAll", SoundManagerMore::soundManagerResumeAll);
RW_FUNCTION(0xCE16FC, "SoundSystem_CreateSound", SoundManagerMore::soundSystemCreateSound);
RW_FUNCTION(0xCE174C, "Sound_Play", SoundManagerMore::soundPlay);
RW_FUNCTION(0xCE1784, "Sound_Stop", SoundManagerMore::soundStop);
RW_FUNCTION(0xCE17B4, "Sound_PauseOn", SoundManagerMore::soundPauseOn);
RW_FUNCTION(0xCE17E4, "Sound_PauseOff", SoundManagerMore::soundPauseOff);
RW_FUNCTION(0xCE1814, "Sound_SetVolume", SoundManagerMore::soundSetVolume);
RW_FUNCTION(0xCE184C, "Sound_SetPitch", SoundManagerMore::soundSetPitch);
RW_FUNCTION(0xCE1884, "Sound_GetStatus", SoundManagerMore::soundGetStatus);
RW_FUNCTION(0xCE23F4, "SQEX::Thread::Join", SoundManagerMore::threadJoin);
RW_FUNCTION(0xCE6878, "SQEX::List<CoreSource*>::~List", SoundManagerMore::coreSourceListDtor);
RW_FUNCTION(0xCF4260, "SQEX::DelegateManager::Release", SoundManagerMore::delegateRelease);
RW_FUNCTION(0xCF42A0, "SQEX::DelegateManager::AddDelegate", SoundManagerMore::delegateAdd);
RW_FUNCTION(0xCF4328, "SQEX::DelegateManager::IsExist", SoundManagerMore::delegateIsExist);

}  // namespace sqex
