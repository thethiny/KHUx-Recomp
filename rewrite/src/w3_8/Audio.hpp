// audio::Manager / audio::Layer: the game's sound front end over the SdSoundSystem library (sound banks .akb read
// from the archive, sounds created per play and started from a queue on the next update).
//
// Manager (a function-local static singleton at 0x1003BD4):
//   banks      unordered_map<ResourceID, Bank>   loaded sound banks (Bank: SdSoundSystem bank handle + the file data)
//   locked     unordered_set<int>                bank handles that must not be unloaded
//   retired    list<Bank>                         banks removed while still playing (freed once idle, in update)
//   layers     map<int, shared_ptr<Layer>>        named sound layers (BGM, SE, voice...)
//   soundCount int                                sounds started minus sounds seen finished
//   queue      map<int, StartParameter>           sounds created this frame, started (faded in) by updateQueue
// Layer: its manager, map<int, int> (play key -> sound handle) and the layer volume.
#pragma once

#include "core/Guest.hpp"

namespace audio {

// libc++ red-black tree node header (left, right, parent, color) followed by the value
struct TreeNode {
    rw::Field<uint32_t> left;     // +0
    rw::Field<uint32_t> right;    // +4
    rw::Field<uint32_t> parent;   // +8
    rw::Field<uint32_t> color;    // +12 (bool is_black)
};

// std::map header: begin node, then the end node (whose left is the root), size
struct TreeHeader {
    rw::Field<uint32_t> begin;    // +0 leftmost node
    rw::Field<uint32_t> root;     // +4 end node's left
    rw::Field<uint32_t> size;     // +8
};
RW_SIZE(TreeHeader, 12);

struct Manager {
    const uint8_t banks[0x14];       // +0x00 unordered_map<int, Bank> (buckets, count, first, size, max load)
    const uint8_t locked[0x14];      // +0x14 unordered_set<int>
    rw::Field<uint32_t> retiredPrev; // +0x28 list<Bank> (sentinel prev, next, size)
    rw::Field<uint32_t> retiredNext; // +0x2C
    rw::Field<uint32_t> retiredSize; // +0x30
    TreeHeader layers;               // +0x34 map<int, shared_ptr<Layer>>
    rw::Field<int32_t> soundCount;   // +0x40
    TreeHeader queue;                // +0x44 map<int, StartParameter>

    static constexpr uint32_t INSTANCE = 0x1003BD4;
    static constexpr uint32_t GUARD = 0x1003BD0;

    static Manager *getInstance();
    static Manager *destruct(Manager *self);
    static void create(Manager *self);
    static void release(Manager *self);
    static void suspend(Manager *self);
    static void resume(Manager *self);
    static int32_t load(Manager *self, int32_t id);
    static void removeBank(Manager *self, const uint32_t *bank);
    static void unload(Manager *self, int32_t id);
    static void unloadAll(Manager *self);
    static void lock(Manager *self, int32_t id);
    static void unlock(Manager *self, int32_t id);
    static void addLayer(Manager *self, int32_t layer);
    static void removeLayer(Manager *self, int32_t layer);
    static void getLayer(guest::SharedPtr *out, Manager *self, int32_t layer);
    static void stop(Manager *self, int32_t sound, int32_t fadeTime);
    static void pause(Manager *self, int32_t sound, int32_t fadeTime);
    static void resumeSound(Manager *self, int32_t sound, int32_t fadeTime);
    static void incrementSoundCount(Manager *self);
    static void update(Manager *self, float dt);
    static void enqueue(Manager *self, int32_t sound, int32_t fadeTime, float volume);
    static void updateQueue(Manager *self);
};
RW_LAYOUT(Manager, retiredPrev, 0x28);
RW_LAYOUT(Manager, layers, 0x34);
RW_LAYOUT(Manager, soundCount, 0x40);
RW_LAYOUT(Manager, queue, 0x44);
RW_SIZE(Manager, 0x50);

struct Layer {
    rw::Field<uint32_t> manager;     // +0x00 Manager&
    TreeHeader sounds;               // +0x04 map<int, int>: play key -> sound handle (multimap insert)
    rw::Field<float> volume;         // +0x10

    static Layer *construct(Layer *self, Manager *manager);
    static uint32_t removeFinishedSound(Layer *self);
    static void pause(Layer *self, int32_t fadeTime);
    static void resume(Layer *self, int32_t fadeTime);
    static int32_t playSE(Layer *self, int32_t id, int32_t key, int32_t fadeTime);
    static int32_t playBGM(Layer *self, int32_t id, int32_t fadeTime);
    static void setVolume(Layer *self, float volume, int32_t fadeTime);
};
RW_LAYOUT(Layer, volume, 0x10);
RW_SIZE(Layer, 0x14);

// SE name -> ResourceID cache (unordered_map<std::string, int> at 0x1003C98), filled from "text/audio/<name>"
void initResourceIDMap();
int32_t convertSENameToResourceID(uint32_t name);

}  // namespace audio
