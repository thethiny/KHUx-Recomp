#include "Audio.hpp"

namespace audio {
namespace {

namespace g {
// SdSoundSystem (the sound library)
constexpr uint32_t sd_initialize = 0xCD8FF0;          // SdSoundSystem_Initialize
constexpr uint32_t sd_terminate = 0xCDB574;           // SdSoundSystem_Terminate
constexpr uint32_t sd_suspend = 0xCD9294;             // SdSoundSystem_Suspend
constexpr uint32_t sd_resume = 0xCD92A8;              // SdSoundSystem_Resume
constexpr uint32_t sd_addData = 0xCD92E0;             // SdSoundSystem_AddData(const void*) -> bank handle
constexpr uint32_t sd_removeData = 0xCDA740;          // SdSoundSystem_RemoveData(bank)
constexpr uint32_t sd_bankIsUsing = 0xCDA834;         // SdSoundSystem_BankCtrl_IsUsing(bank)
constexpr uint32_t sd_bankStopSounds = 0xCDA880;      // SdSoundSystem_BankCtrl_StopSounds(bank, fadeTime)
constexpr uint32_t sd_getSoundsCount = 0xCDA948;      // SdSoundSystem_GetSoundsCount()
constexpr uint32_t sd_createSound = 0xCDA94C;         // SdSoundSystem_CreateSound(bank, index) -> sound
constexpr uint32_t sd_startFadeIn = 0xCDACAC;         // SdSoundSystem_SoundCtrl_StartFadeIn(sound, fade, 0, volume, 0)
constexpr uint32_t sd_stop = 0xCDAD4C;                // SdSoundSystem_SoundCtrl_Stop(sound, fadeTime)
constexpr uint32_t sd_isExist = 0xCDADB0;             // SdSoundSystem_SoundCtrl_IsExist(sound)
constexpr uint32_t sd_setPause = 0xCDADE0;            // SdSoundSystem_SoundCtrl_SetPause(sound, pause, fadeTime)
constexpr uint32_t sd_setVolume = 0xCDAE90;           // SdSoundSystem_SoundCtrl_SetVolume(sound, volume, fadeTime)
// cocos2d
constexpr uint32_t director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t director_getScheduler = rwlib::cocos2d::Director::getScheduler;  // Director::getScheduler() const
constexpr uint32_t scheduler_scheduleUpdate = 0x5BEE19;   // Scheduler::scheduleUpdate<audio::Manager>(target, priority, paused)
constexpr uint32_t scheduler_unscheduleUpdate = rwlib::cocos2d::Scheduler::unscheduleUpdate; // Scheduler::unscheduleUpdate(void*)
// game / library
constexpr uint32_t stringBuffer_ctor = 0x49140D;      // StringBuffer::StringBuffer(const char* fmt, ...)
constexpr uint32_t stringBuffer_fset = 0x491501;      // StringBuffer::fset(const char* fmt, ...)
constexpr uint32_t vfs_readData = 0x8DD46D;           // bg::vfs::readData(const char*) (sret shared_ptr<bg::Data>)
constexpr uint32_t strlen_ = 0x40E08C;
constexpr uint32_t strtol_ = 0x4101A4;
constexpr uint32_t string_init = 0x47CD93;            // std::string::__init(const char*, size_t)
// libc++ containers of the module
constexpr uint32_t banks_find = 0x5C092F;             // unordered_map<int, Bank>::find(const int&)
constexpr uint32_t banks_insertUnique = 0x5C053B;     // unordered_map<int, Bank>::__insert_unique(pair&&)
constexpr uint32_t banks_erase = 0x5C03D1;            // unordered_map<int, Bank>::erase(const_iterator) -> next
constexpr uint32_t banks_dtor = 0x5C0A51;
constexpr uint32_t locked_find = 0x5BFE59;            // unordered_set<int>::find(const int&)
constexpr uint32_t locked_erase = 0x5BFEDD;           // unordered_set<int>::erase(const_iterator)
constexpr uint32_t locked_insertUnique = 0x5C002F;    // unordered_set<int>::__insert_unique(const int&)
constexpr uint32_t locked_eraseUnique = 0x5BFE39;     // unordered_set<int>::__erase_unique(const int&)
constexpr uint32_t locked_dtor = 0x5C0A99;
constexpr uint32_t retired_pushBack = 0x5BF061;       // list<Bank>::push_back(const Bank&)
constexpr uint32_t retired_erase = 0x5BF58D;          // list<Bank>::erase(const_iterator) -> next
constexpr uint32_t retired_clear = 0x5C0AC7;          // list<Bank>::clear()
constexpr uint32_t layers_find = 0x5BFB27;            // map<int, shared_ptr<Layer>>::find(const int&)
constexpr uint32_t layers_insertUnique = 0x5BFBE5;    // map<int, shared_ptr<Layer>>::__insert_unique(pair&&)
constexpr uint32_t layers_eraseUnique = 0x5BFB6B;     // map<int, shared_ptr<Layer>>::__erase_unique(const int&)
constexpr uint32_t layers_destroy = 0x5C0B09;         // map<int, shared_ptr<Layer>> tree destroy(node)
constexpr uint32_t layer_makeShared = 0x5BFD35;       // std::make_shared<audio::Layer>(Manager&)
constexpr uint32_t queue_find = 0x5BFAE3;             // map<int, StartParameter>::find(const int&)
constexpr uint32_t queue_erase = 0x5BFA95;            // map<int, StartParameter>::erase(const_iterator) -> next
constexpr uint32_t queue_insertUnique = 0x5BF953;     // map<int, StartParameter>::__insert_unique(pair&&)
constexpr uint32_t queue_destroy = 0x5C0B3F;          // map<int, StartParameter> tree destroy(node)
constexpr uint32_t sounds_insertMulti = 0x5BF8C7;     // map<int, int>::__insert_multi(pair&&)
constexpr uint32_t sounds_erase = 0x5BF879;           // map<int, int>::erase(const_iterator)
constexpr uint32_t names_clear = 0x5C2A93;            // unordered_map<std::string, int>::clear()
constexpr uint32_t names_find = 0x5C2997;             // unordered_map<std::string, int>::find(const std::string&)
constexpr uint32_t names_insertUnique = 0x5C23B9;     // unordered_map<std::string, int>::__insert_unique(pair<const char*, int>&&)
// strings
constexpr uint32_t str_bankPath = 0xE00D18;           // "audio/%09d.akb"
constexpr uint32_t str_namePath = 0xE00D27;           // "text/audio/%s"
}  // namespace g

constexpr uint32_t NAME_MAP = 0x1003C98;              // unordered_map<std::string, int>: SE name -> ResourceID
constexpr uint32_t STRING_BUFFER_LENGTH = 0x400;      // StringBuffer: char[1024], then the length
constexpr uint32_t QUEUE_PAUSED = 0x1C;               // queue node: key +0x10, StartParameter {fade +0x14, volume +0x18, paused +0x1C}
constexpr uint32_t SOUND_HANDLE = 0x14;               // sounds node: key +0x10, handle +0x14
constexpr uint32_t LAYER_VALUE = 0x14;                // layers node: key +0x10, shared_ptr<Layer> +0x14
constexpr uint32_t BANK_NODE_VALUE = 0x0C;            // banks node: next, hash, key +8, Bank +0x0C
constexpr uint32_t RETIRED_BANK = 0x08;               // retired list node: prev, next, Bank +8

uint32_t ga(const void *p) { return rw::guest_addr(p); }
uint32_t ld(uint32_t a) { return rw::load<uint32_t>(a); }

// The in-order successor of a tree node (libc++ __tree_next, inlined by the original)
uint32_t treeNext(uint32_t node) {
    uint32_t right = ld(node + 4);
    if (right) {
        uint32_t n = right;
        while (ld(n)) n = ld(n);
        return n;
    }
    uint32_t child = node, parent;
    do {
        parent = ld(child + 8);
        bool isLeft = ld(parent) == child;
        child = parent;
        if (isLeft) break;
    } while (true);
    return parent;
}

}  // namespace

// ------------------------------------------------------------------------------------------------ Manager
Manager *Manager::getInstance() {
    if (guest::staticInitBegin(GUARD)) {
        auto *m = rw::host_ptr<Manager>(INSTANCE);
        const uint32_t o = INSTANCE;
        rw::save<uint32_t>(o + 0x0C, 0);
        rw::save<uint32_t>(o + 0x08, 0);
        rw::save<uint32_t>(o + 0x04, 0);
        rw::save<uint32_t>(o + 0x00, 0);
        rw::save<float>(o + 0x10, 1.0f);           // banks max_load_factor
        rw::save<uint32_t>(o + 0x20, 0);
        rw::save<uint32_t>(o + 0x1C, 0);
        rw::save<uint32_t>(o + 0x18, 0);
        rw::save<uint32_t>(o + 0x14, 0);
        rw::save<float>(o + 0x24, 1.0f);           // locked max_load_factor
        m->retiredPrev = o + 0x28;                 // empty list: the sentinel links to itself
        m->retiredNext = o + 0x28;
        m->retiredSize = 0;
        m->layers.root = 0;
        m->layers.size = 0;
        m->layers.begin = o + 0x38;                // empty map: begin = end node
        m->soundCount = 0;
        m->queue.begin = o + 0x48;
        m->queue.root = 0;
        m->queue.size = 0;
        rw::call(guest::CXA_ATEXIT, {0x5BEDAD, o, guest::DSO_HANDLE});   // ~Manager at exit
        guest::staticInitDone(GUARD);
    }
    return rw::host_ptr<Manager>(INSTANCE);
}
RW_FUNCTION(0x5BECFC, "audio::Manager::getInstance", Manager::getInstance);

Manager *Manager::destruct(Manager *self) {
    uint32_t s = ga(self);
    rw::call(g::queue_destroy, {s + 0x44, self->queue.root});
    rw::call(g::layers_destroy, {s + 0x34, self->layers.root});
    rw::call(g::retired_clear, {s + 0x28});
    rw::call(g::locked_dtor, {s + 0x14});
    rw::call(g::banks_dtor, {s});
    return self;
}
RW_FUNCTION(0x5BEDAC, "audio::Manager::~Manager", Manager::destruct);

void Manager::create(Manager *self) {
    uint32_t r = rw::call(g::sd_initialize, {0});
    uint32_t director = rw::call(g::director_getInstance, {r});
    uint32_t scheduler = rw::call(g::director_getScheduler, {director, 0x7FFFFFFF});
    rw::call(g::scheduler_scheduleUpdate, {scheduler, ga(self), 0, 0});
}
RW_FUNCTION(0x5BEDE8, "audio::Manager::create", Manager::create);

void Manager::release(Manager *self) {
    uint32_t director = rw::call(g::director_getInstance, {ga(self)});
    uint32_t scheduler = rw::call(g::director_getScheduler, {director, 0x7FFFFFFF});
    uint32_t r = rw::call(g::scheduler_unscheduleUpdate, {scheduler, ga(self)});
    rw::call(g::sd_terminate, {r});
}
RW_FUNCTION(0x5BEEC0, "audio::Manager::release", Manager::release);

void Manager::suspend(Manager *self) { rw::call(g::sd_suspend, {ga(self)}); }
RW_FUNCTION(0x5BEEE4, "audio::Manager::suspend", Manager::suspend);

void Manager::resume(Manager *self) { rw::call(g::sd_resume, {ga(self)}); }
RW_FUNCTION(0x5BEEEC, "audio::Manager::resume()", Manager::resume);

// The bank handle of sound bank `id`, loading "audio/<id>.akb" into the sound system on first use (0 when the
// file is missing).
int32_t Manager::load(Manager *self, int32_t id) {
    guest::Frame f(0x450);
    const uint32_t bankPair = f.addr(0x00);   // pair<ResourceID, Bank>: id, handle, shared_ptr<bg::Data>
    const uint32_t data = f.addr(0x10);       // shared_ptr<bg::Data> read from the archive
    const uint32_t key = f.addr(0x18);
    const uint32_t inserted = f.addr(0x1C);
    const uint32_t path = f.addr(0x24);       // StringBuffer
    rw::save<int32_t>(key, id);
    uint32_t node = rw::call(g::banks_find, {ga(self), key});
    if (node) return int32_t(ld(node + BANK_NODE_VALUE));

    int32_t handle = 0;
    rw::save<uint32_t>(path + STRING_BUFFER_LENGTH, 0);
    rw::save<uint8_t>(path, 0);
    rw::call(g::stringBuffer_fset, {path, g::str_bankPath, uint32_t(id)});
    rw::call(g::vfs_readData, {data, path});
    uint32_t bytes = ld(data);
    if (bytes) {
        handle = int32_t(rw::call(g::sd_addData, {ld(bytes + 4)}));
        uint32_t obj = ld(data), ctrl = ld(data + 4);
        if (ctrl) guest::addRef(ctrl);
        rw::save<int32_t>(bankPair, id);
        rw::save<uint32_t>(bankPair + 8, obj);
        rw::save<uint32_t>(bankPair + 12, ctrl);
        rw::save<int32_t>(bankPair + 4, handle);
        rw::call(g::banks_insertUnique, {inserted, ga(self), bankPair});
        if (ld(bankPair + 12)) guest::release(ld(bankPair + 12));
    }
    if (ld(data + 4)) guest::release(ld(data + 4));
    return handle;
}
RW_FUNCTION(0x5BEEF4, "audio::Manager::load", Manager::load);

// A bank still in use is stopped and kept on the retired list (update frees it once idle).
void Manager::removeBank(Manager *self, const uint32_t *bank) {
    uint32_t handle = bank[0];
    if (rw::call(g::sd_bankIsUsing, {handle})) {
        rw::call(g::sd_bankStopSounds, {bank[0], 500});
        rw::call(g::retired_pushBack, {ga(self) + 0x28, ga(bank)});
    } else {
        rw::call(g::sd_removeData, {bank[0]});
    }
}
RW_FUNCTION(0x5BF028, "audio::Manager::removeBank", Manager::removeBank);

void Manager::unload(Manager *self, int32_t id) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), id);
    uint32_t node = rw::call(g::banks_find, {ga(self), f.addr()});
    if (!node) return;
    uint32_t bank = node + BANK_NODE_VALUE;
    uint32_t lockedSet = ga(self) + 0x14;
    uint32_t it = rw::call(g::locked_find, {lockedSet, bank});
    if (it) rw::call(g::locked_erase, {lockedSet, it});
    removeBank(self, rw::host_ptr<uint32_t>(bank));
    rw::call(g::banks_erase, {ga(self), node});
}
RW_FUNCTION(0x5BF0A0, "audio::Manager::unload", Manager::unload);

// Unload every bank that is not locked.
void Manager::unloadAll(Manager *self) {
    uint32_t node = rw::load<uint32_t>(ga(self) + 8);   // first node of the banks map
    uint32_t lockedSet = ga(self) + 0x14;
    while (node) {
        uint32_t bank = node + BANK_NODE_VALUE;
        if (rw::call(g::locked_find, {lockedSet, bank})) {
            node = ld(node);
        } else {
            removeBank(self, rw::host_ptr<uint32_t>(bank));
            node = rw::call(g::banks_erase, {ga(self), node});
        }
    }
}
RW_FUNCTION(0x5BF138, "audio::Manager::unloadAll", Manager::unloadAll);

void Manager::lock(Manager *self, int32_t id) {
    guest::Frame f(16);
    rw::save<int32_t>(f.addr(), id);
    uint32_t node = rw::call(g::banks_find, {ga(self), f.addr()});
    if (node) rw::call(g::locked_insertUnique, {f.addr(4), ga(self) + 0x14, node + BANK_NODE_VALUE});
}
RW_FUNCTION(0x5BF18C, "audio::Manager::lock", Manager::lock);

void Manager::unlock(Manager *self, int32_t id) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), id);
    uint32_t node = rw::call(g::banks_find, {ga(self), f.addr()});
    if (node) rw::call(g::locked_eraseUnique, {ga(self) + 0x14, node + BANK_NODE_VALUE});
}
RW_FUNCTION(0x5BF1EC, "audio::Manager::unlock", Manager::unlock);

void Manager::addLayer(Manager *self, int32_t layer) {
    guest::Frame f(0x20);
    const uint32_t pair = f.addr(0x00);     // pair<int, shared_ptr<Layer>>
    const uint32_t made = f.addr(0x0C);     // shared_ptr<Layer>
    const uint32_t inserted = f.addr(0x14);
    rw::call(g::layer_makeShared, {made, ga(self)});
    rw::save<int32_t>(pair, layer);
    rw::save<uint32_t>(pair + 4, ld(made));
    uint32_t ctrl = ld(made + 4);
    rw::save<uint32_t>(pair + 8, ctrl);
    if (ctrl) guest::addRef(ctrl);
    rw::call(g::layers_insertUnique, {inserted, ga(self) + 0x34, pair});
    if (ld(pair + 8)) guest::release(ld(pair + 8));
    if (ctrl) guest::release(ctrl);
}
RW_FUNCTION(0x5BF24C, "audio::Manager::addLayer", Manager::addLayer);

void Manager::removeLayer(Manager *self, int32_t layer) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), layer);
    rw::call(g::layers_eraseUnique, {ga(self) + 0x34, f.addr()});
}
RW_FUNCTION(0x5BF2F8, "audio::Manager::removeLayer", Manager::removeLayer);

void Manager::getLayer(guest::SharedPtr *out, Manager *self, int32_t layer) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), layer);
    uint32_t node = rw::call(g::layers_find, {ga(self) + 0x34, f.addr()});
    if (node == ga(self) + 0x38) {          // end()
        out->obj = 0;
        out->ctrl = 0;
        return;
    }
    out->obj = ld(node + LAYER_VALUE);
    uint32_t ctrl = ld(node + LAYER_VALUE + 4);
    out->ctrl = ctrl;
    if (ctrl) guest::addRef(ctrl);
}
RW_FUNCTION(0x5BF33C, "audio::Manager::getLayer", Manager::getLayer);

// A sound still waiting in the start queue is just dropped from it.
void Manager::stop(Manager *self, int32_t sound, int32_t fadeTime) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), sound);
    uint32_t queue = ga(self) + 0x44;
    uint32_t node = rw::call(g::queue_find, {queue, f.addr()});
    if (node != ga(self) + 0x48) rw::call(g::queue_erase, {queue, node});
    else rw::call(g::sd_stop, {uint32_t(sound), uint32_t(fadeTime)});
}
RW_FUNCTION(0x5BF3AC, "audio::Manager::stop", Manager::stop);

void Manager::pause(Manager *self, int32_t sound, int32_t fadeTime) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), sound);
    uint32_t node = rw::call(g::queue_find, {ga(self) + 0x44, f.addr()});
    if (node != ga(self) + 0x48) rw::save<uint8_t>(node + QUEUE_PAUSED, 1);
    else rw::call(g::sd_setPause, {uint32_t(sound), 1, uint32_t(fadeTime)});
}
RW_FUNCTION(0x5BF424, "audio::Manager::pause", Manager::pause);

void Manager::resumeSound(Manager *self, int32_t sound, int32_t fadeTime) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), sound);
    uint32_t node = rw::call(g::queue_find, {ga(self) + 0x44, f.addr()});
    if (node != ga(self) + 0x48) rw::save<uint8_t>(node + QUEUE_PAUSED, 0);
    else rw::call(g::sd_setPause, {uint32_t(sound), 0, uint32_t(fadeTime)});
}
RW_FUNCTION(0x5BF48C, "audio::Manager::resume(int, int)", Manager::resumeSound);

void Manager::incrementSoundCount(Manager *self) { self->soundCount = self->soundCount + 1; }
RW_FUNCTION(0x5BF4F4, "audio::Manager::incrementSoundCount", Manager::incrementSoundCount);

// Free retired banks that went idle; when the sound system's count differs from ours, let the layers drop one
// finished sound (one per frame) or resynchronise the count.
void Manager::update(Manager *self, float) {
    uint32_t end = ga(self) + 0x28;
    uint32_t node = self->retiredNext;
    while (node != end) {
        if (rw::call(g::sd_bankIsUsing, {ld(node + RETIRED_BANK)})) {
            node = ld(node + 4);
            continue;
        }
        rw::call(g::sd_removeData, {ld(node + RETIRED_BANK)});
        node = rw::call(g::retired_erase, {end, node});
    }
    int32_t count = int32_t(rw::call(g::sd_getSoundsCount, {}));
    if (count == self->soundCount) return;
    uint32_t layersEnd = ga(self) + 0x38;
    for (uint32_t it = self->layers.begin; it != layersEnd; it = treeNext(it)) {
        if (Layer::removeFinishedSound(rw::host_ptr<Layer>(ld(it + LAYER_VALUE))) == 1) {
            self->soundCount = self->soundCount - 1;
            return;
        }
    }
    self->soundCount = count;
}
RW_FUNCTION(0x5BF4FC, "audio::Manager::update", Manager::update);

void Manager::enqueue(Manager *self, int32_t sound, int32_t fadeTime, float volume) {
    guest::Frame f(0x20);
    const uint32_t pair = f.addr(0x00);     // pair<int, StartParameter>
    rw::save<int32_t>(pair, sound);
    rw::save<int32_t>(pair + 4, fadeTime);
    rw::save<float>(pair + 8, volume);
    rw::save<uint8_t>(pair + 12, 0);
    rw::call(g::queue_insertUnique, {f.addr(0x10), ga(self) + 0x44, pair});
}
RW_FUNCTION(0x5BF618, "audio::Manager::enqueue", Manager::enqueue);

// Start every queued sound that is not paused (fade in from its start parameter).
void Manager::updateQueue(Manager *self) {
    uint32_t queue = ga(self) + 0x44, end = ga(self) + 0x48;
    uint32_t node = self->queue.begin;
    while (node != end) {
        if (rw::load<uint8_t>(node + QUEUE_PAUSED)) {
            node = treeNext(node);
            continue;
        }
        rw::call(g::sd_startFadeIn, {ld(node + 0x10), ld(node + 0x14), 0, ld(node + 0x18), 0});
        node = rw::call(g::queue_erase, {queue, node});
    }
}
RW_FUNCTION(0x5BF66C, "audio::Manager::updateQueue", Manager::updateQueue);

// ------------------------------------------------------------------------------------------------ Layer
Layer *Layer::construct(Layer *self, Manager *manager) {
    self->manager = ga(manager);
    self->sounds.begin = ga(self) + 8;      // empty map
    self->sounds.root = 0;
    self->sounds.size = 0;
    self->volume = 1.0f;
    return self;
}
RW_FUNCTION(0x5BF6D0, "audio::Layer::Layer", Layer::construct);

// Forget the first sound the sound system no longer knows; 1 when one was removed.
uint32_t Layer::removeFinishedSound(Layer *self) {
    uint32_t end = ga(self) + 8;
    for (uint32_t it = self->sounds.begin; it != end; it = treeNext(it)) {
        if (!rw::call(g::sd_isExist, {ld(it + SOUND_HANDLE)})) {
            rw::call(g::sounds_erase, {ga(self) + 4, it});
            return 1;
        }
    }
    return 0;
}
RW_FUNCTION(0x5BF5BC, "audio::Layer::removeFinishedSound", Layer::removeFinishedSound);

void Layer::pause(Layer *self, int32_t fadeTime) {
    uint32_t end = ga(self) + 8;
    for (uint32_t it = self->sounds.begin; it != end; it = treeNext(it))
        Manager::pause(rw::host_ptr<Manager>(self->manager), int32_t(ld(it + SOUND_HANDLE)), fadeTime);
}
RW_FUNCTION(0x5BF6EA, "audio::Layer::pause", Layer::pause);

void Layer::resume(Layer *self, int32_t fadeTime) {
    uint32_t end = ga(self) + 8;
    for (uint32_t it = self->sounds.begin; it != end; it = treeNext(it))
        Manager::resumeSound(rw::host_ptr<Manager>(self->manager), int32_t(ld(it + SOUND_HANDLE)), fadeTime);
}
RW_FUNCTION(0x5BF736, "audio::Layer::resume", Layer::resume);

// Create a sound from bank `id` under play key `key` and queue it to start at the layer volume.
int32_t Layer::playSE(Layer *self, int32_t id, int32_t key, int32_t fadeTime) {
    auto *manager = rw::host_ptr<Manager>(self->manager);
    manager->soundCount = manager->soundCount + 1;
    int32_t bank = Manager::load(rw::host_ptr<Manager>(self->manager), id);
    int32_t sound = int32_t(rw::call(g::sd_createSound, {uint32_t(bank), 0}));
    guest::Frame f(0x28);
    const uint32_t entry = f.addr(0x00);    // pair<int, int>
    const uint32_t start = f.addr(0x08);    // pair<int, StartParameter>
    rw::save<int32_t>(entry, key);
    rw::save<int32_t>(entry + 4, sound);
    rw::call(g::sounds_insertMulti, {ga(self) + 4, entry});
    uint32_t m = self->manager;
    rw::save<int32_t>(start, sound);
    rw::save<int32_t>(start + 4, fadeTime);
    rw::save<float>(start + 8, self->volume);
    rw::save<uint8_t>(start + 12, 0);
    rw::call(g::queue_insertUnique, {f.addr(0x18), m + 0x44, start});
    return sound;
}
RW_FUNCTION(0x5BF784, "audio::Layer::playSE", Layer::playSE);

int32_t Layer::playBGM(Layer *self, int32_t id, int32_t fadeTime) { return playSE(self, id, 0x7FFFFFFF, fadeTime); }
RW_FUNCTION(0x5BF810, "audio::Layer::playBGM", Layer::playBGM);

void Layer::setVolume(Layer *self, float volume, int32_t fadeTime) {
    self->volume = volume;
    uint32_t end = ga(self) + 8;
    for (uint32_t it = self->sounds.begin; it != end; it = treeNext(it))
        rw::call(g::sd_setVolume, {ld(it + SOUND_HANDLE), rw::fbits(self->volume), uint32_t(fadeTime)});
}
RW_FUNCTION(0x5BF824, "audio::Layer::setVolume", Layer::setVolume);

// ------------------------------------------------------------------------------------------------ SE names
void initResourceIDMap() { rw::call(g::names_clear, {NAME_MAP}); }
RW_FUNCTION(0x5C227C, "audio::initResourceIDMap", initResourceIDMap);

// The ResourceID of an SE name: cached, else the number in "text/audio/<name>" (0 when missing or not a
// positive number).
int32_t convertSENameToResourceID(uint32_t name) {
    guest::Frame f(0x438);
    const uint32_t pair = f.addr(0x00);     // pair<const char*, int>
    const uint32_t endp = f.addr(0x08);     // char* strtol end
    const uint32_t data = f.addr(0x0C);     // shared_ptr<bg::Data>
    const uint32_t key = f.addr(0x14);      // std::string
    const uint32_t inserted = f.addr(0x20);
    const uint32_t path = f.addr(0x28);     // StringBuffer
    uint32_t n = rw::call(g::strlen_, {name});
    rw::call(g::string_init, {key, name, n});
    uint32_t node = rw::call(g::names_find, {NAME_MAP, key});
    if (rw::load<uint8_t>(key) & 1) guest::deleteObject(ld(key + 8));
    if (node) return int32_t(ld(node + 0x14));

    rw::call(g::stringBuffer_ctor, {path, g::str_namePath, name});
    rw::call(g::vfs_readData, {data, path});
    int32_t id = 0;
    uint32_t bytes = ld(data);
    if (bytes) {
        int32_t v = int32_t(rw::call(g::strtol_, {ld(bytes + 4), endp, 10}));
        if (v >= 1 && rw::load<uint8_t>(ld(endp)) == 0) {
            rw::save<uint32_t>(pair, name);
            rw::save<int32_t>(pair + 4, v);
            rw::call(g::names_insertUnique, {inserted, NAME_MAP, pair});
            id = v;
        }
    }
    if (ld(data + 4)) guest::release(ld(data + 4));
    return id;
}
RW_FUNCTION(0x5C2298, "audio::convertSENameToResourceID", convertSENameToResourceID);

}  // namespace audio
