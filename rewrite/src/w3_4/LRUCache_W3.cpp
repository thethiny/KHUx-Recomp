// Small helpers next to the LRUCache instances: the shared_ptr control blocks whose deleter is a lambda releasing
// a cocos2d::Ref (AvatarIconCache::add $_2, cocos2d::TextureCache::insertTexture $_2: the cached object is kept
// alive by a retain, and the last shared_ptr owner releases it), the MasterDataManager $_0 cost function (the size
// of a cached SafeBuffer) and three empty functions of the cache code.
//
// libc++ __shared_ptr_pointer<T*, Deleter, Alloc>: vtable +0, shared owners +4, weak owners +8, the pointer +12
// (the deleter lambda is empty and shares its address).
#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

namespace w3_4_lru {

namespace {
constexpr uint32_t SHARED_WEAK_COUNT_DTOR = 0xC8DB69;   // std::__shared_weak_count::~__shared_weak_count (nullsub_4)
constexpr uint32_t REF_RELEASE = 0x8FD0BD;               // cocos2d::Ref::release()
constexpr uint32_t CTRL_PTR = 12;                        // the owned pointer (and the deleter) in the control block
constexpr uint32_t TYPENAME_AVATAR_ICON_ADD_2 = 0xE4A700;      // "ZN15AvatarIconCache3addEyP6AvatarE3$_2"
constexpr uint32_t TYPENAME_INSERT_TEXTURE_2 = 0xEBCDE0;       // "ZN7cocos2d12TextureCache13insertTexture...E3$_2"

// deleting destructor: ~__shared_weak_count, then operator delete
void deleteControlBlock(uint32_t self) { guest::deleteObject(rw::call(SHARED_WEAK_COUNT_DTOR, {self})); }
// __on_zero_shared: the deleter releases the Ref
void releaseRefDeleter(uint32_t self) {
    uint32_t ref = rw::load<uint32_t>(self + CTRL_PTR);
    if (ref) rw::call(REF_RELEASE, {ref});
}
// __get_deleter(type_info): the deleter when the type name matches (compared by address)
template <uint32_t TYPENAME> uint32_t getDeleter(uint32_t self, uint32_t type) {
    return rw::load<uint32_t>(type + 4) == TYPENAME ? self + CTRL_PTR : 0;
}
// __on_zero_shared_weak: free the control block
void freeControlBlock(uint32_t self) { guest::deleteObject(self); }
}  // namespace

// AvatarIconCache::add $_2 (shared_ptr<AvatarIcon> deleter: release())
RW_FUNCTION(0x5CB5AC, "shared_ptr<AvatarIcon> control block (AvatarIconCache::add $_2) ~ (deleting)", deleteControlBlock);
RW_FUNCTION(0x5CB5B8, "shared_ptr<AvatarIcon> control block (AvatarIconCache::add $_2) __on_zero_shared", releaseRefDeleter);
RW_FUNCTION(0x5CB5CC, "shared_ptr<AvatarIcon> control block (AvatarIconCache::add $_2) __get_deleter",
            getDeleter<TYPENAME_AVATAR_ICON_ADD_2>);

// cocos2d::TextureCache::insertTexture $_2 (shared_ptr<Texture2D> deleter: release())
RW_FUNCTION(0x9191B0, "shared_ptr<Texture2D> control block (TextureCache::insertTexture $_2) ~ (deleting)", deleteControlBlock);
RW_FUNCTION(0x9191BC, "shared_ptr<Texture2D> control block (TextureCache::insertTexture $_2) __on_zero_shared",
            releaseRefDeleter);
RW_FUNCTION(0x9191CC, "shared_ptr<Texture2D> control block (TextureCache::insertTexture $_2) __get_deleter",
            getDeleter<TYPENAME_INSERT_TEXTURE_2>);
RW_FUNCTION(0x9191F0, "shared_ptr<Texture2D> control block (TextureCache::insertTexture $_2) __on_zero_shared_weak",
            freeControlBlock);

// MasterDataManager::MasterDataManager() $_0: the LRU cost of a cached master table = its byte size.
uint32_t masterDataCost(uint32_t /*lambda*/, const guest::SharedPtr *buffer) {
    return rw::host_ptr<SafeBuffer>(buffer->obj)->size;
}
RW_FUNCTION(0x76EFFA, "MasterDataManager::MasterDataManager $_0 operator()(const shared_ptr<const SafeBuffer>&)",
            masterDataCost);

// Three empty functions of the LRU cache code (bx lr).
void nothing() {}
RW_FUNCTION(0x7F4C08, "nullsub_2261 (LRUCache)", nothing);
RW_FUNCTION(0x7F4C0A, "nullsub_31 (LRUCache)", nothing);
RW_FUNCTION(0x7F4C0C, "nullsub_2262 (LRUCache)", nothing);

}  // namespace w3_4_lru
