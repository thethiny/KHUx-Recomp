// SQEX sound driver: the static constructors of its source files (the .init_array entries the host runs at load) -
// the SdSoundSystem state, the action queue, the sound list, the threads, the core's mutexes / source list /
// master volume, and the OpenSL ES interface id table. Each object's destructor is registered with
// __aeabi_atexit(object, destructor, &__dso_handle).
#include "Sqex.hpp"

using namespace b2;

namespace sqex {

namespace {

constexpr uint32_t AEABI_ATEXIT = 0x44DA10;     // __aeabi_atexit (import)
constexpr uint32_t DSO_HANDLE = 0xFEF000;       // &__dso_handle
constexpr uint32_t THREAD_CTOR = 0xCE2370;      // SQEX::Thread::Thread
constexpr uint32_t MUTEX_CTOR = 0xCE6908;       // SQEX::Mutex::Mutex (a recursive pthread mutex)
constexpr uint32_t DYNAMIC_VALUE_CTOR = 0xCF43FC;   // SQEX::DynamicValue::DynamicValue(float)
constexpr uint32_t COND_INIT = 0x4466FC;        // pthread_cond_init (import)
// GOT slots holding the destructors
constexpr uint32_t FIXED_LIST_BANK_DTOR_GOT = 0xFD925C;    // SQEX::FixedList<BankImpl*>::~FixedList
constexpr uint32_t THREAD_DTOR_GOT = 0xFD9260;             // SQEX::Thread::~Thread
constexpr uint32_t MUTEX_DTOR_GOT = 0xFD9264;              // SQEX::Mutex::~Mutex
constexpr uint32_t FIXED_QUEUE_ACTION_DTOR_GOT = 0xFD9284; // SQEX::FixedQueue<ACTION>::~FixedQueue
constexpr uint32_t FIXED_LIST_SOUND_DTOR_GOT = 0xFD9294;   // SQEX::FixedList<SoundImpl*>::~FixedList
constexpr uint32_t CONDITION_DTOR_GOT = 0xFD9444;          // SQEX::Condition::~Condition
constexpr uint32_t CORE_SOURCE_LIST_DTOR_GOT = 0xFD934C;   // SQEX::List<CoreSource*>::~List
// GOT slots holding the core's statics
constexpr uint32_t CORE_MUTEX_GOT = 0xFD9318, CORE_SOURCE_LIST_GOT = 0xFD931C, CORE_MASTER_VOLUME_GOT = 0xFD9340,
                   CORE_CAPTURE_MUTEX_GOT = 0xFD9314, CORE_OUTER_CAPTURE_MUTEX_GOT = 0xFD9348;

constexpr uint32_t SD_STATE = 0x100E598;        // SdSoundSystem: mutex +0, bank list +4, thread +0x2C
constexpr uint32_t ACTION_QUEUE = 0x100E5D8;    // FixedQueue<ACTION>, its mutex at +0x14
constexpr uint32_t SOUND_LIST = 0x100E5F0;      // FixedList<SoundImpl*>, its mutex at +0x18
constexpr uint32_t SOUND_THREAD = 0x100E610;    // a SQEX::Thread
constexpr uint32_t CATEGORY_RECORDS = 0x100E67C;   // four {1.0f, 0, 0, 0, 0} records (after CategoryManager's)
constexpr uint32_t DELEGATES = 0x100EB7C;       // DelegateManager: mutex +4, condition +0x50, thread +0x54
constexpr uint32_t SL_INTERFACES = 0x1002C00, SL_INTERFACE_SIZE = 0x24;   // {SLInterfaceID, ...} x 49

void atexit(uint32_t object, uint32_t dtorGot) { rw::call(AEABI_ATEXIT, {object, got(dtorGot), DSO_HANDLE}); }

// SQEX::Mutex's constructor as these constructors inline it: a recursive pthread mutex
void recursiveMutex(uint32_t at) {
    guest::Frame f(8);
    const uint32_t attr = f.addr(4);
    rw::call(0x44D098, {attr});     // pthread_mutexattr_init
    rw::call(0x44D0A4, {attr, 1});  // pthread_mutexattr_settype(RECURSIVE)
    rw::call(0x4466F0, {at, attr}); // pthread_mutex_init
    rw::call(0x44D0B0, {attr});     // pthread_mutexattr_destroy
}
void zeroWords(uint32_t at, uint32_t words) {
    for (uint32_t k = 0; k < words; k++) st32(at + 4 * k, 0);
}

// SL_IID_* in table order (GOT slots holding &SL_IID_X)
constexpr uint32_t SL_IID_GOT[] = {
    0xFD9364,  // SL_IID_NULL
    0xFD9368,  // SL_IID_OBJECT
    0xFD936C,  // SL_IID_AUDIOIODEVICECAPABILITIES
    0xFD9370,  // SL_IID_LED
    0xFD9374,  // SL_IID_VIBRA
    0xFD9378,  // SL_IID_METADATAEXTRACTION
    0xFD937C,  // SL_IID_METADATATRAVERSAL
    0xFD9380,  // SL_IID_DYNAMICSOURCE
    0xFD9384,  // SL_IID_OUTPUTMIX
    0xFD9360,  // SL_IID_PLAY
    0xFD9388,  // SL_IID_PREFETCHSTATUS
    0xFD938C,  // SL_IID_PLAYBACKRATE
    0xFD9390,  // SL_IID_SEEK
    0xFD9394,  // SL_IID_RECORD
    0xFD9398,  // SL_IID_EQUALIZER
    0xFD93C4,  // SL_IID_VOLUME
    0xFD939C,  // SL_IID_DEVICEVOLUME
    0xFD93A0,  // SL_IID_BUFFERQUEUE
    0xFD93A4,  // SL_IID_PRESETREVERB
    0xFD93A8,  // SL_IID_ENVIRONMENTALREVERB
    0xFD93AC,  // SL_IID_EFFECTSEND
    0xFD93B0,  // SL_IID_3DGROUPING
    0xFD93B4,  // SL_IID_3DCOMMIT
    0xFD93B8,  // SL_IID_3DLOCATION
    0xFD93BC,  // SL_IID_3DDOPPLER
    0xFD93C0,  // SL_IID_3DSOURCE
    0xFD93F0,  // SL_IID_3DMACROSCOPIC
    0xFD93C8,  // SL_IID_MUTESOLO
    0xFD93CC,  // SL_IID_DYNAMICINTERFACEMANAGEMENT
    0xFD93D0,  // SL_IID_MIDIMESSAGE
    0xFD93D4,  // SL_IID_MIDIMUTESOLO
    0xFD93D8,  // SL_IID_MIDITEMPO
    0xFD93DC,  // SL_IID_MIDITIME
    0xFD93E0,  // SL_IID_AUDIODECODERCAPABILITIES
    0xFD93E4,  // SL_IID_AUDIOENCODERCAPABILITIES
    0xFD93E8,  // SL_IID_AUDIOENCODER
    0xFD93EC,  // SL_IID_BASSBOOST
    0xFD9418,  // SL_IID_PITCH
    0xFD93F4,  // SL_IID_RATEPITCH
    0xFD93F8,  // SL_IID_VIRTUALIZER
    0xFD93FC,  // SL_IID_VISUALIZATION
    0xFD9354,  // SL_IID_ENGINE
    0xFD9400,  // SL_IID_ENGINECAPABILITIES
    0xFD9404,  // SL_IID_THREADSYNC
    0xFD9408,  // SL_IID_ANDROIDEFFECT
    0xFD940C,  // SL_IID_ANDROIDEFFECTSEND
    0xFD9410,  // SL_IID_ANDROIDEFFECTCAPABILITIES
    0xFD9414,  // SL_IID_ANDROIDCONFIGURATION
    0xFD9358,  // SL_IID_ANDROIDSIMPLEBUFFERQUEUE
};

}  // namespace

struct SqexStatics {
    // SdSoundSystem's state: the bank list, the sound thread, the mutex
    static void sdSoundSystem() {
        zeroWords(SD_STATE + 4, 6);
        atexit(SD_STATE + 4, FIXED_LIST_BANK_DTOR_GOT);
        rw::call(THREAD_CTOR, {SD_STATE + 0x2C});
        atexit(SD_STATE + 0x2C, THREAD_DTOR_GOT);
        recursiveMutex(SD_STATE);
        atexit(SD_STATE, MUTEX_DTOR_GOT);
    }
    static void actionQueue() {
        atexit(ACTION_QUEUE, FIXED_QUEUE_ACTION_DTOR_GOT);
        recursiveMutex(ACTION_QUEUE + 0x14);
        atexit(ACTION_QUEUE + 0x14, MUTEX_DTOR_GOT);
    }
    static void soundList() {
        zeroWords(SOUND_LIST, 6);
        atexit(SOUND_LIST, FIXED_LIST_SOUND_DTOR_GOT);
        recursiveMutex(SOUND_LIST + 0x18);
        atexit(SOUND_LIST + 0x18, MUTEX_DTOR_GOT);
    }
    static void soundThread() {
        rw::call(THREAD_CTOR, {SOUND_THREAD});
        atexit(SOUND_THREAD, THREAD_DTOR_GOT);
    }
    static void categoryRecords() {
        for (uint32_t k = 0; k < 4; k++) {
            const uint32_t r = CATEGORY_RECORDS + k * 0x14;
            st32(r, 0x3F800000);  // 1.0f
            zeroWords(r + 4, 4);
        }
    }
    // SQEX::CoreSystem: mutex_, sourceList_, masterVolume_ (1.0), captureMutex_, outerCaptureMutex_
    static void core() {
        const uint32_t mutex = got(CORE_MUTEX_GOT);
        rw::call(MUTEX_CTOR, {mutex});
        atexit(mutex, MUTEX_DTOR_GOT);
        const uint32_t sources = got(CORE_SOURCE_LIST_GOT);
        zeroWords(sources, 3);
        atexit(sources, CORE_SOURCE_LIST_DTOR_GOT);
        rw::call(DYNAMIC_VALUE_CTOR, {got(CORE_MASTER_VOLUME_GOT), 0x3F800000});
        const uint32_t capture = got(CORE_CAPTURE_MUTEX_GOT);
        rw::call(MUTEX_CTOR, {capture});
        atexit(capture, MUTEX_DTOR_GOT);
        const uint32_t outer = got(CORE_OUTER_CAPTURE_MUTEX_GOT);
        rw::call(MUTEX_CTOR, {outer});
        atexit(outer, MUTEX_DTOR_GOT);
    }
    // the OpenSL ES interface ids (the table's other fields are constant data)
    static void slInterfaces() {
        uint32_t at = SL_INTERFACES;
        for (uint32_t slot : SL_IID_GOT) {
            st32(at, ld32(got(slot)));
            at += SL_INTERFACE_SIZE;
        }
    }
    // DelegateManager: its thread, mutex and condition
    static void delegates() {
        rw::call(THREAD_CTOR, {DELEGATES + 0x54});
        atexit(DELEGATES + 0x54, THREAD_DTOR_GOT);
        recursiveMutex(DELEGATES + 4);
        atexit(DELEGATES + 4, MUTEX_DTOR_GOT);
        rw::call(COND_INIT, {DELEGATES + 0x50, 0});
        atexit(DELEGATES + 0x50, CONDITION_DTOR_GOT);
    }
};
RW_FUNCTION(0x4595C8, "SdSoundSystem static constructor (sub_4595C8)", SqexStatics::sdSoundSystem);
RW_FUNCTION(0x45968C, "SQEX action queue static constructor (sub_45968C)", SqexStatics::actionQueue);
RW_FUNCTION(0x459710, "SQEX sound list static constructor (sub_459710)", SqexStatics::soundList);
RW_FUNCTION(0x4597B4, "SQEX thread static constructor (sub_4597B4)", SqexStatics::soundThread);
RW_FUNCTION(0x4597F4, "SQEX category records static constructor (sub_4597F4)", SqexStatics::categoryRecords);
RW_FUNCTION(0x45984C, "SQEX::CoreSystem static constructor (sub_45984C)", SqexStatics::core);
RW_FUNCTION(0x45992C, "OpenSL ES interface table static constructor (sub_45992C)", SqexStatics::slInterfaces);
RW_FUNCTION(0x459D40, "SQEX::DelegateManager static constructor (sub_459D40)", SqexStatics::delegates);

}  // namespace sqex
