// AvatarIconCache: the avatar icons (AvatarIcon nodes rendered from an Avatar into a shared texture) of the party /
// ranking lists, kept in an LRUCache<unsigned long long, shared_ptr<AvatarIcon>> keyed by user id. The cache
// header is the LRUCache itself (core/LRUCache.hpp); the icon parameters follow it.
#include "core/LRUCache.hpp"
#include "scene/SceneCommon.hpp"

namespace avatar_icon_cache_w31 {

using namespace scene;

// layout after the LRUCache header (60 bytes)
constexpr uint32_t TEXTURE = 0x40;          // cocos2d::Texture2D* (retained)
constexpr uint32_t ICON_POSITION = 0x44;    // cocos2d::Vec2
constexpr uint32_t ICON_SCALE_X = 0x4C;     // float
constexpr uint32_t ICON_SCALE_Y = 0x50;     // float
constexpr uint32_t EXPRESSIONS = 0x54;      // std::vector<Avatar::Expression>

constexpr uint32_t COST_LAMBDA_VTABLE = 0xF5D06C;     // setLimit(unsigned)::$_1: every icon costs 1
constexpr uint32_t ICON_DELETER_BLOCK_VTABLE = 0xF8F5D4;   // __shared_ptr_pointer<AvatarIcon*, add::$_2> + 8
constexpr uint32_t DEFAULT_LIMIT = 30;

constexpr uint32_t VEC2_COPY = 0x8E4C03;           // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;           // cocos2d::Vec2::~Vec2()
constexpr uint32_t EXPRESSIONS_COPY = 0x5CA801;    // std::vector<Avatar::Expression>::vector(const vector&)
constexpr uint32_t EXPRESSIONS_DTOR = 0x46BE0D;    // std::__vector_base<Avatar::Expression>::~__vector_base
constexpr uint32_t REF_RETAIN = 0x8FD0B5;
constexpr uint32_t REF_RELEASE = 0x8FD0BD;
constexpr uint32_t COST_FN_ASSIGN = 0x5CB689;      // std::function<unsigned(const shared_ptr<AvatarIcon>&)>::operator=
constexpr uint32_t LIST_CLEAR = 0x5CBD6D;          // std::__list_imp<unsigned long long>::clear
constexpr uint32_t MAP_DTOR = 0x5CBD25;            // ~__hash_table<u64, shared_ptr<AvatarIcon>>
constexpr uint32_t LRU_FIND = 0x5CAA05;            // LRUCache<u64, shared_ptr<AvatarIcon>>::find(out, this, const u64&)
constexpr uint32_t LRU_INSERT = 0x5CAB75;          // LRUCache<u64, shared_ptr<AvatarIcon>>::insert(this, const u64&, const V&)
constexpr uint32_t AVATAR_CREATE = 0x5C5883;       // Avatar::create(const Avatar::Style&, bool)
constexpr uint32_t AVATAR_CHANGE_MOTION = 0x5C63C5;   // Avatar::changeMotion(Avatar::Motion)
constexpr uint32_t AVATAR_ICON_CREATE = 0x5CA185;  // AvatarIcon::create(Avatar*, Texture2D*, const Vec2&, float, float, const vector<Expression>&)
constexpr uint32_t MOTION_ICON = 27;               // Avatar::Motion used for icons

// setLimit(limit): capacity = limit, cost function = $_1 (one per icon)
uint32_t setLimit(LRUCache *self, uint32_t limit) {
    Frame f;
    uint32_t fn = newFunction(f, COST_LAMBDA_VTABLE, {});
    self->capacity = limit;
    call(COST_FN_ASSIGN, {rw::guest_addr(self) + 40, fn});
    functionDestroy(fn);
    return 0;   // r0: the stack-protector difference (always 0)
}

// AvatarIconCache(texture, position, scaleX, scaleY, expressions)
LRUCache *construct(LRUCache *self, uint32_t texture, uint32_t position, uint32_t scaleX, uint32_t scaleY,
                    uint32_t expressions) {
    uint32_t s = rw::guest_addr(self);
    self->mapSize = 0;
    self->firstNode = 0;
    self->bucketCount = 0;
    self->buckets = 0;
    self->maxLoadFactor = 1.0f;
    self->listBack = s + LRUCache::LIST_END;
    self->listFront = s + LRUCache::LIST_END;
    self->listSize = 0;
    self->capacity = 0;
    self->used = 0;
    self->costFnCallable = 0;
    st32(s + TEXTURE, texture);
    call(VEC2_COPY, {s + ICON_POSITION, position});
    st32(s + ICON_SCALE_X, scaleX);
    st32(s + ICON_SCALE_Y, scaleY);
    call(EXPRESSIONS_COPY, {s + EXPRESSIONS, expressions});
    if (ld32(s + TEXTURE)) call(REF_RETAIN, {ld32(s + TEXTURE)});
    setLimit(self, DEFAULT_LIMIT);
    return self;
}

LRUCache *destruct(LRUCache *self) {
    uint32_t s = rw::guest_addr(self);
    if (ld32(s + TEXTURE)) call(REF_RELEASE, {ld32(s + TEXTURE)});
    call(EXPRESSIONS_DTOR, {s + EXPRESSIONS});
    call(VEC2_DTOR, {s + ICON_POSITION});
    functionDestroy(s + 40);
    call(LIST_CLEAR, {s + LRUCache::LIST_END});
    call(MAP_DTOR, {s});
    return self;
}

// find(userId): the cached icon or null (the shared_ptr copy is dropped right away)
uint32_t find(LRUCache *self, uint64_t userId) {
    Frame f;
    uint32_t out = f.alloc(8);
    uint32_t key = f.alloc(8);
    st32(key, uint32_t(userId));
    st32(key + 4, uint32_t(userId >> 32));
    call(LRU_FIND, {out, rw::guest_addr(self), key});
    uint32_t icon = ld32(out);
    if (ld32(out + 4)) call(fn::shared_release, {ld32(out + 4)});
    return icon;
}

// add(userId, avatar): a new icon of the avatar, cached (the cache owns one reference, released by $_2)
uint32_t addAvatar(LRUCache *self, uint64_t userId, uint32_t avatar) {
    uint32_t s = rw::guest_addr(self);
    Frame f;
    uint32_t key = f.alloc(8);
    uint32_t value = f.alloc(8);   // shared_ptr<AvatarIcon>
    st32(key, uint32_t(userId));
    st32(key + 4, uint32_t(userId >> 32));
    uint32_t icon = call(AVATAR_ICON_CREATE, {avatar, ld32(s + TEXTURE), s + ICON_POSITION, ld32(s + ICON_SCALE_X),
                                              ld32(s + ICON_SCALE_Y), s + EXPRESSIONS});
    call(REF_RETAIN, {icon});
    st32(value, icon);
    uint32_t block = call(fn::operator_new, {16});
    st32(block + 4, 0);
    st32(block + 8, 0);
    st32(block, ICON_DELETER_BLOCK_VTABLE);
    st32(block + 12, icon);
    st32(value + 4, block);
    call(LRU_INSERT, {s, key, value});
    uint32_t result = ld32(value);
    if (ld32(value + 4)) call(fn::shared_release, {ld32(value + 4)});
    return result;
}

// add(userId, style): an avatar of the style in the icon motion, then add(userId, avatar)
uint32_t addStyle(LRUCache *self, uint64_t userId, uint32_t style) {
    uint32_t avatar = call(AVATAR_CREATE, {style, 1});
    call(AVATAR_CHANGE_MOTION, {avatar, MOTION_ICON});
    return addAvatar(self, userId, avatar);
}

}  // namespace avatar_icon_cache_w31

RW_FUNCTION(0x5CA740, "AvatarIconCache::AvatarIconCache(Texture2D*, const Vec2&, float, float, const vector<Avatar::Expression>&)",
            avatar_icon_cache_w31::construct);
RW_FUNCTION(0x5CA864, "AvatarIconCache::setLimit(unsigned int)", avatar_icon_cache_w31::setLimit);
RW_FUNCTION(0x5CA8FC, "AvatarIconCache::~AvatarIconCache()", avatar_icon_cache_w31::destruct);
RW_FUNCTION(0x5CA9A8, "AvatarIconCache::find(unsigned long long)", avatar_icon_cache_w31::find);
RW_FUNCTION(0x5CAA56, "AvatarIconCache::add(unsigned long long, const Avatar::Style&)", avatar_icon_cache_w31::addStyle);
RW_FUNCTION(0x5CAA90, "AvatarIconCache::add(unsigned long long, Avatar*)", avatar_icon_cache_w31::addAvatar);
