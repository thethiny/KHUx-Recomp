#include "LRUCache.hpp"

namespace {

constexpr uint32_t STRING_INIT = 0x47CD93;   // std::string::__init(const char*, size_t) (sub_47CD92)
constexpr uint32_t COST_FN_SLOT = 6;         // std::function __base::operator()(const V&) virtual slot

using Node = LRUCache::ListNode;
Node *node(uint32_t a) { return rw::host_ptr<Node>(a); }

// ------------------------------------------------------------------------------------------ keys
// Key of a list node (+8) compared with the looked-up key.
bool keyEquals(uint32_t listNode, const uint32_t *key) { return rw::load<uint32_t>(listNode + 8) == *key; }
bool keyEquals(uint32_t listNode, const uint64_t *key) { return rw::load<uint64_t>(listNode + 8) == *key; }
bool keyEquals(uint32_t listNode, const guest::String *key) {
    const guest::String &k = *rw::host_ptr<guest::String>(listNode + 8);
    uint32_t n = key->size();
    return k.size() == n && std::memcmp(rw::host_ptr<uint8_t>(k.data()), rw::host_ptr<uint8_t>(key->data()), n) == 0;
}

// list<K> node for a scalar key, linked at the front (push_front inlined in the original)
template <class K> void pushFrontScalar(LRUCache *self, const K *key) {
    uint32_t n = guest::newObject(8 + sizeof(K));
    rw::save<K>(n + 8, *key);
    uint32_t first = self->listFront;
    node(node(first)->prev)->next = n;
    node(n)->prev = node(first)->prev.get();
    node(first)->prev = n;
    node(n)->next = first;
    self->listSize += 1;
}

// list.erase(n) + delete (inlined in the original)
void unlinkNode(LRUCache *self, uint32_t n) {
    node(node(n)->prev)->next = node(n)->next.get();
    node(node(n)->next)->prev = node(n)->prev.get();
    self->listSize -= 1;
    guest::deleteObject(n);
}

// The list node holding `key` (scan from the front), or the end node.
template <class K> uint32_t listFind(const LRUCache *self, const K *key) {
    uint32_t end = rw::guest_addr(self) + LRUCache::LIST_END;
    uint32_t n = self->listFront;
    while (n != end && !keyEquals(n, key)) n = node(n)->next;
    return n;
}

// ------------------------------------------------------------------------------------------ instances
// Per instance: key type, value offset in the map's hash node, the libc++ hash-table functions it calls, how
// it calls the cost function (COST_CALL: std::function::operator() out of line; 0: virtual call on __f_ inlined).
struct AvatarIconCache {        // LRUCache<unsigned long long, shared_ptr<AvatarIcon>>
    using Key = uint64_t;
    static constexpr uint32_t VALUE = 16;
    static constexpr uint32_t FIND = 0x5CB4D5, ERASE = 0x5CB369, INSERT_UNIQUE = 0x5CAE39, INDEX = 0x5CACD1;
    static constexpr uint32_t COST_CALL = 0x5CADBD;
};
struct SafeBufferCache {        // LRUCache<unsigned long long, shared_ptr<const SafeBuffer>> (MasterDataManager)
    using Key = uint64_t;
    static constexpr uint32_t VALUE = 16;
    static constexpr uint32_t FIND = 0x76EE6D, ERASE = 0x76ED01, INSERT_UNIQUE = 0x76E7D1, INDEX = 0x76E669;
    static constexpr uint32_t COST_CALL = 0x76E755;
};
struct LwfDataCache {           // LRUCache<unsigned, shared_ptr<LWF::Data>>
    using Key = uint32_t;
    static constexpr uint32_t VALUE = 12;
    static constexpr uint32_t FIND = 0x7FA5C7, ERASE = 0x7FA45D, INSERT_UNIQUE = 0x7FA08D, INDEX = 0x7F9FA5;
    static constexpr uint32_t COST_CALL = 0;
};
struct ValueMapCache {          // LRUCache<unsigned, shared_ptr<unordered_map<string, cocos2d::Value>>>
    using Key = uint32_t;
    static constexpr uint32_t VALUE = 12;
    static constexpr uint32_t FIND = 0x7F981F, ERASE = 0x7F96B5, INSERT_UNIQUE = 0x7F9975, INDEX = 0x7F98C1;
    static constexpr uint32_t ERASE_UNIQUE = 0x7F98A1;
    static constexpr uint32_t COST_CALL = 0;
};
struct TextureCache {           // LRUCache<unsigned, shared_ptr<cocos2d::Texture2D>>
    using Key = uint32_t;
    static constexpr uint32_t VALUE = 12;
    static constexpr uint32_t FIND = 0x91912F, ERASE = 0x918FC5, INSERT_UNIQUE = 0x918BF5, INDEX = 0x918AE5;
    static constexpr uint32_t ERASE_UNIQUE = 0x9191F9;
    static constexpr uint32_t COST_CALL = 0;
};
struct TextRendererCache {      // LRUCache<std::string, shared_ptr<cocos2d::LWFTextRendererContext>>
    using Key = guest::String;
    static constexpr uint32_t VALUE = 20;
    static constexpr uint32_t FIND = 0x7F1993, ERASE = 0x7F181D, INSERT_UNIQUE = 0x7F1291, INDEX = 0x7F10C5;
    static constexpr uint32_t LIST_PUSH_FRONT = 0x7F11ED;   // std::list<string>::push_front(const string&)
    static constexpr uint32_t LIST_POP_BACK = 0x7F1263;     // std::list<string>::pop_back()
    static constexpr uint32_t COST_CALL = 0;
};

template <class I> struct Cache {
    using Key = typename I::Key;
    static constexpr bool STRING_KEY = std::is_same_v<Key, guest::String>;

    static uint32_t cost(LRUCache *self, uint32_t value) {
        if constexpr (I::COST_CALL != 0) {
            return rw::call(I::COST_CALL, {rw::guest_addr(self->costFn), value});
        } else {
            uint32_t f = self->costFnCallable;
            return rw::vcall(rw::host_ptr<uint8_t>(f), COST_FN_SLOT, {f, value});
        }
    }

    // find(key) -> shared_ptr (sret; empty when missing); a hit is moved to the front of the recency list.
    static void find(guest::SharedPtr *out, LRUCache *self, const Key *key) {
        uint32_t n = rw::call(I::FIND, {rw::arg(self), rw::arg(key)});
        if (n) {
            if (self->costFnCallable) update(self, key);
            out->obj = rw::load<uint32_t>(n + I::VALUE);
            uint32_t ctrl = rw::load<uint32_t>(n + I::VALUE + 4);
            out->ctrl = ctrl;
            if (ctrl) guest::addRef(ctrl);
        } else {
            out->obj = 0;
            out->ctrl = 0;
        }
    }

    // update(key): move the key's list node to the front (most recently used).
    static void update(LRUCache *self, const Key *key) {
        uint32_t first = self->listFront;
        uint32_t end = rw::guest_addr(self) + LRUCache::LIST_END;
        uint32_t n = listFind(self, key);
        if (n == first || n == end) return;
        // list.splice(begin(), list, n) as libc++ inlines it
        uint32_t a = node(n)->prev, b = node(n)->next;
        node(node(first)->prev)->next = node(a)->next.get();
        node(node(a)->next)->prev = node(first)->prev.get();
        node(node(b)->prev)->next = first;
        node(first)->prev = node(b)->prev.get();
        node(b)->prev = a;
        node(a)->next = b;
    }

    // removeData(key): drop the map entry and its cost (the list node stays).
    static void removeData(LRUCache *self, const Key *key) {
        uint32_t n = rw::call(I::FIND, {rw::arg(self), rw::arg(key)});
        if (!n) return;
        uint32_t c = cost(self, n + I::VALUE);
        self->used = self->used - c;
        rw::call(I::ERASE, {rw::arg(self), n});
    }

    // removeLRUData(): evict the least recently used entry (the list's back).
    static void removeLRUData(LRUCache *self) {
        removeData(self, rw::host_ptr<Key>(self->listBack + 8));
        if constexpr (STRING_KEY) {
            rw::call(I::LIST_POP_BACK, {rw::guest_addr(self) + LRUCache::LIST_END});
        } else {
            unlinkNode(self, self->listBack);
        }
    }

    // remove(key)
    static void remove(LRUCache *self, const Key *key) {
        if (self->costFnCallable) {
            removeData(self, key);
            uint32_t n = listFind(self, key);
            if (n != rw::guest_addr(self) + LRUCache::LIST_END) unlinkNode(self, n);
        } else {
            rw::call(I::ERASE_UNIQUE, {rw::arg(self), rw::arg(key)});
        }
    }

    // insert(key, value)
    static void insert(LRUCache *self, const Key *key, const guest::SharedPtr *value) {
        if (!self->costFnCallable) {
            // plain map: map[key] = value
            uint32_t mapped = rw::call(I::INDEX, {rw::arg(self), rw::arg(key)});
            uint32_t obj = value->obj, ctrl = value->ctrl;
            if (ctrl) guest::addRef(ctrl);
            rw::save<uint32_t>(mapped, obj);
            uint32_t old = rw::load<uint32_t>(mapped + 4);
            rw::save<uint32_t>(mapped + 4, ctrl);
            if (old) guest::release(old);
            return;
        }
        // an existing entry is replaced (its old cost is not subtracted) and becomes the most recent
        uint32_t existing = rw::call(I::FIND, {rw::arg(self), rw::arg(key)});
        if (existing) {
            rw::call(I::ERASE, {rw::arg(self), existing});
            update(self, key);
        } else if constexpr (STRING_KEY) {
            rw::call(I::LIST_PUSH_FRONT, {rw::guest_addr(self) + LRUCache::LIST_END, rw::arg(key)});
        } else {
            pushFrontScalar(self, key);
        }

        // map.insert(pair<K, V>(key, value)), the pair on the stack
        guest::Frame f(0x30);
        uint32_t pair = f.addr(8), result = f.addr(0x20);
        uint32_t pairValue = pair + (STRING_KEY ? 12 : sizeof(Key));
        if constexpr (STRING_KEY) {
            if (key->isLong()) {
                rw::call(STRING_INIT, {pair, rw::get<uint32_t>(key->raw, 8), rw::get<uint32_t>(key->raw, 4)});
            } else {
                for (uint32_t k = 0; k < 12; k += 4) rw::save<uint32_t>(pair + k, rw::get<uint32_t>(key->raw, k));
            }
        } else {
            rw::save<Key>(pair, *key);
        }
        rw::save<uint32_t>(pairValue, value->obj);
        uint32_t ctrl = value->ctrl;
        rw::save<uint32_t>(pairValue + 4, ctrl);
        if (ctrl) guest::addRef(ctrl);
        rw::call(I::INSERT_UNIQUE, {result, rw::arg(self), pair});
        ctrl = rw::load<uint32_t>(pairValue + 4);   // re-read: the insert may have moved from the pair
        if (ctrl) guest::release(ctrl);
        if constexpr (STRING_KEY) {
            if (rw::load<uint8_t>(pair) & 1) guest::deleteObject(rw::load<uint32_t>(pair + 8));
        }

        self->used = self->used + cost(self, rw::arg(value));
        while (self->used > self->capacity) removeLRUData(self);
    }
};

}  // namespace

// The functions as they exist in the library (other uses of the templates were inlined by the compiler).
using AvatarIcon = Cache<AvatarIconCache>;
RW_FUNCTION(0x5CAA04, "LRUCache<unsigned long long, shared_ptr<AvatarIcon>>::find", AvatarIcon::find);
RW_FUNCTION(0x5CAB74, "LRUCache<unsigned long long, shared_ptr<AvatarIcon>>::insert", AvatarIcon::insert);
RW_FUNCTION(0x5CAD6C, "LRUCache<unsigned long long, shared_ptr<AvatarIcon>>::update", AvatarIcon::update);
RW_FUNCTION(0x5CAE00, "LRUCache<unsigned long long, shared_ptr<AvatarIcon>>::removeData", AvatarIcon::removeData);

using SafeBufferLru = Cache<SafeBufferCache>;
RW_FUNCTION(0x76CE54, "LRUCache<unsigned long long, shared_ptr<const SafeBuffer>>::find", SafeBufferLru::find);
RW_FUNCTION(0x76CF34, "LRUCache<unsigned long long, shared_ptr<const SafeBuffer>>::insert", SafeBufferLru::insert);
RW_FUNCTION(0x76E704, "LRUCache<unsigned long long, shared_ptr<const SafeBuffer>>::update", SafeBufferLru::update);
RW_FUNCTION(0x76E798, "LRUCache<unsigned long long, shared_ptr<const SafeBuffer>>::removeData",
            SafeBufferLru::removeData);

using LwfData = Cache<LwfDataCache>;
RW_FUNCTION(0x7F02C8, "LRUCache<unsigned, shared_ptr<LWF::Data>>::find", LwfData::find);
RW_FUNCTION(0x7F031C, "LRUCache<unsigned, shared_ptr<LWF::Data>>::insert", LwfData::insert);
RW_FUNCTION(0x7FA014, "LRUCache<unsigned, shared_ptr<LWF::Data>>::update", LwfData::update);
RW_FUNCTION(0x7FA058, "LRUCache<unsigned, shared_ptr<LWF::Data>>::removeData", LwfData::removeData);

using ValueMap = Cache<ValueMapCache>;
RW_FUNCTION(0x7F064C, "LRUCache<unsigned, shared_ptr<ValueMap>>::find", ValueMap::find);
RW_FUNCTION(0x7F06A0, "LRUCache<unsigned, shared_ptr<ValueMap>>::insert", ValueMap::insert);
RW_FUNCTION(0x7F0858, "LRUCache<unsigned, shared_ptr<ValueMap>>::remove", ValueMap::remove);
RW_FUNCTION(0x7F9680, "LRUCache<unsigned, shared_ptr<ValueMap>>::removeData", ValueMap::removeData);
RW_FUNCTION(0x7F9930, "LRUCache<unsigned, shared_ptr<ValueMap>>::update", ValueMap::update);

using TextRenderer = Cache<TextRendererCache>;
RW_FUNCTION(0x7F0D3C, "LRUCache<string, shared_ptr<LWFTextRendererContext>>::find", TextRenderer::find);
RW_FUNCTION(0x7F0D90, "LRUCache<string, shared_ptr<LWFTextRendererContext>>::insert", TextRenderer::insert);
RW_FUNCTION(0x7F1134, "LRUCache<string, shared_ptr<LWFTextRendererContext>>::update", TextRenderer::update);
RW_FUNCTION(0x7F122E, "LRUCache<string, shared_ptr<LWFTextRendererContext>>::removeData", TextRenderer::removeData);

using Texture = Cache<TextureCache>;
RW_FUNCTION(0x917464, "LRUCache<unsigned, shared_ptr<Texture2D>>::find", Texture::find);
RW_FUNCTION(0x918320, "LRUCache<unsigned, shared_ptr<Texture2D>>::remove", Texture::remove);
RW_FUNCTION(0x918444, "LRUCache<unsigned, shared_ptr<Texture2D>>::insert", Texture::insert);
RW_FUNCTION(0x918B54, "LRUCache<unsigned, shared_ptr<Texture2D>>::update", Texture::update);
RW_FUNCTION(0x918B98, "LRUCache<unsigned, shared_ptr<Texture2D>>::removeLRUData", Texture::removeLRUData);
RW_FUNCTION(0x918BC0, "LRUCache<unsigned, shared_ptr<Texture2D>>::removeData", Texture::removeData);
