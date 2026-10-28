// std::shared_ptr<T>::make_shared<Args...>(Args&&...) for the game's types. Each instance is the libc++ frame
// (libcxx::makeShared: one block = control block + T) around T's constructor, which the compiler inlined: the
// stores below are those constructors (SafeBuffer-based API objects, filter states, streams...).
#include "libcxx/Libcxx.hpp"

using guest::SharedPtr;
using libcxx::makeShared;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t ONE_F = 0x3F800000;   // 1.0f (an unordered_map's max_load_factor)

// library and game constructors the inlined code calls
constexpr uint32_t SafeBuffer_ctor = 0x459F21;           // SafeBuffer::SafeBuffer(unsigned size, bool)
constexpr uint32_t SafeBuffer_copy = 0x45A29D;           // SafeBuffer::SafeBuffer(const SafeBuffer&)
constexpr uint32_t SafeBuffer_fromData = 0x45A421;       // SafeBuffer::SafeBuffer(const shared_ptr<bg::Data>&)
constexpr uint32_t Function_copy = 0x491797;             // std::function<void(void*)>::function(const function&)
constexpr uint32_t FuncPtr_cloneInto = 0x491D45;         // __func<void(*)(void*)>::__clone(__base*) const
constexpr uint32_t JsonStack_ctor = 0x49A659;            // rapidjson::internal::Stack<MemoryPoolAllocator>::Stack(alloc, size)
constexpr uint32_t MapU64Int_copy = 0x4D606D;            // unordered_map<unsigned long long, int>(const&)
constexpr uint32_t MapIntInt_copy = 0x4D6661;            // unordered_map<int, int>(const&)
constexpr uint32_t VectorInt_copy = 0x4AF707;            // vector<int>(const&)
constexpr uint32_t ExpressionVector_dtor = 0x46BE0D;     // __vector_base<Avatar::Expression>::~__vector_base
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;              // __aeabi_memset(dest, n, c) (import)
constexpr uint32_t AASSETMANAGER_OPEN = 0x43D0D8;        // AAssetManager_open (import)
constexpr uint32_t AssetInputStream_assetManager = 0x1006830;   // bg::AssetInputStream::assetManager_

void memclear(uint32_t dest, uint32_t n) { rw::call(AEABI_MEMSET, {dest, n, 0}); }

// unordered_map default constructor: no buckets, no nodes, max_load_factor 1.0f
void emptyHashMap(uint32_t m) {
    for (uint32_t k = 0; k < 16; k += 4) save<uint32_t>(m + k, 0);
    save<uint32_t>(m + 16, ONE_F);
}

// hole::network::api objects over a SafeBuffer of `count` entries (UserTitle, UserMedal, ...): the SafeBuffer
// (0x20), the count and an empty unordered_map index
template <uint32_t EntrySize> void apiListFromCount(uint32_t obj, uint32_t vtable, uint32_t count) {
    rw::call(SafeBuffer_ctor, {obj, count * EntrySize, 1});
    save<uint32_t>(obj, vtable);
    save<uint32_t>(obj + 0x20, count);
    emptyHashMap(obj + 0x24);
}

// ... copied: the SafeBuffer, the count, the index (the count is stored again after the map copy)
void apiListCopy(uint32_t obj, uint32_t vtable, uint32_t src, uint32_t mapCopy) {
    rw::call(SafeBuffer_copy, {obj, src});
    save<uint32_t>(obj, vtable);
    save<uint32_t>(obj + 0x20, load<uint32_t>(src + 0x20));
    rw::call(mapCopy, {obj + 0x24, src + 0x24});
    save<uint32_t>(obj + 0x20, load<uint32_t>(src + 0x20));
}

// SafeBuffer-only objects copied from another (UserStone, Login, ...)
void safeBufferCopy(uint32_t obj, uint32_t vtable, uint32_t src) {
    rw::call(SafeBuffer_copy, {obj, src});
    save<uint32_t>(obj, vtable);
}

// bg::Data(void* bytes, unsigned size, std::function<void(void*)> deleter) from a deleter object
void dataWithDeleter(uint32_t block, uint32_t obj, uint32_t bytes, uint32_t size, uint32_t deleter) {
    save<uint32_t>(obj, 0xF7E1EC);   // vtable for bg::Data
    save<uint32_t>(obj + 4, bytes);
    save<uint32_t>(obj + 8, size);
    rw::call(Function_copy, {block + 0x20, deleter});
}

// ---------------------------------------------------------------------------------------------- instances

// bg::MemoryStream(0x10000, std::function<void*(unsigned)>(), std::function<void(void*)>()): both empty
void makeMemoryStream(SharedPtr *result) {
    makeShared(result, 0x70, 0xF7CF64, 0x10, [](uint32_t, uint32_t obj) {
        guest::Frame frame(0x30);
        const uint32_t freeFn = frame.addr(0), allocFn = frame.addr(0x18);   // two empty std::functions
        *frame.at<uint32_t>(0x10) = 0;
        *frame.at<uint32_t>(0x28) = 0;
        rw::call(0x491665, {obj, 0x10000, allocFn, freeFn});   // bg::MemoryStream::MemoryStream
        libcxx::destroyFunction(freeFn);
        libcxx::destroyFunction(allocFn);
    });
}

void makeStartDeckEditDialog(SharedPtr *result) {
    makeShared(result, 0x300, 0xF7E070, 0x10,
               [](uint32_t, uint32_t obj) { rw::call(0x485701, {obj}); });   // StartDeckEditDialog::StartDeckEditDialog
}

// bg::Data(void*&, unsigned&, std::function<void(void*)>&)
void makeData(SharedPtr *result, const uint32_t *bytes, const uint32_t *size, uint32_t deleter) {
    makeShared(result, 0x38, 0xF7E1C4, 0x10,
               [&](uint32_t block, uint32_t obj) { dataWithDeleter(block, obj, *bytes, *size, deleter); });
}

// bg::Data(p, size, void(&)(void*)): the deleter function becomes a std::function on the stack first, which is
// then copied into the Data and destroyed
void makeDataWithFreeFunction(SharedPtr *result, const uint32_t *bytes, const uint32_t *size, uint32_t freeFn) {
    makeShared(result, 0x38, 0xF7E1C4, 0x10, [&](uint32_t block, uint32_t obj) {
        guest::Frame frame(0x18);
        const uint32_t tmp = frame.addr();   // 16-byte buffer, __f_ at +16
        *frame.at<uint32_t>(16) = 0;
        if (freeFn) {
            *frame.at<uint32_t>(16) = tmp;
            *frame.at<uint32_t>(0) = 0xF7E204;   // vtable for __func<void(*)(void*), allocator, void(void*)>
            *frame.at<uint32_t>(4) = freeFn;
        }
        save<uint32_t>(obj, 0xF7E1EC);   // vtable for bg::Data
        save<uint32_t>(obj + 4, *bytes);
        save<uint32_t>(obj + 8, *size);
        // std::function copy into the Data's member (buffer at block + 0x20, __f_ at block + 0x30)
        const uint32_t f = load<uint32_t>(tmp + 16);
        if (!f) save<uint32_t>(block + 0x30, 0);
        else if (f == tmp) {
            save<uint32_t>(block + 0x30, block + 0x20);
            rw::call(FuncPtr_cloneInto, {f, block + 0x20});
        } else {
            save<uint32_t>(block + 0x30, rw::vcall(rw::host_ptr<uint8_t>(f), 2, {f}));   // __clone()
        }
        libcxx::destroyFunction(tmp);
    });
}

// hole::network::RESTClient::Result: the response document (rapidjson) with its parse stack
void makeRestResult(SharedPtr *result) {
    makeShared(result, 0x44, 0xF7E598, 0xC, [](uint32_t block, uint32_t) {
        save<uint32_t>(block + 0x20, 0);
        rw::call(JsonStack_ctor, {block + 0x24, 0, 0x400});
        save<uint32_t>(block + 0x3C, 0);
        save<uint32_t>(block + 0x40, 0);
    });
}

void makeRestConnection(SharedPtr *result) {
    makeShared(result, 0x58, 0xF7E61C, 0x10, [](uint32_t block, uint32_t) {
        save<uint32_t>(block + 0x10, 0);
        save<uint32_t>(block + 0x28, 0);
        save<uint8_t>(block + 0x31, 1);
        save<uint8_t>(block + 0x30, 0);
        memclear(block + 0x34, 0x20);
    });
}

void makeUserLink(SharedPtr *result) {
    makeShared(result, 0x20, 0xF7E660, 0xC, [](uint32_t block, uint32_t) {
        save<uint32_t>(block + 0x18, 0);
        save<uint32_t>(block + 0x14, 0);
        save<uint32_t>(block + 0x10, 0);
        save<uint32_t>(block + 0xC, 0);
        save<uint32_t>(block + 0x1C, ONE_F);
    });
}

void makeUserTitle(SharedPtr *result, const uint32_t *count) {
    makeShared(result, 0x44, 0xF7EF20, 0xC, [&](uint32_t, uint32_t obj) { apiListFromCount<0x10>(obj, 0xF7EF48, *count); });
}
void makeUserKeyblade(SharedPtr *result, const uint32_t *count) {
    makeShared(result, 0x44, 0xF7EFDC, 0xC, [&](uint32_t, uint32_t obj) { apiListFromCount<0x58>(obj, 0xF7F004, *count); });
}
void makeUserMaterial(SharedPtr *result, const uint32_t *count) {
    makeShared(result, 0x44, 0xF7F048, 0xC, [&](uint32_t, uint32_t obj) { apiListFromCount<0x10>(obj, 0xF7F070, *count); });
}
void makeUserSkill(SharedPtr *result, const uint32_t *count) {
    makeShared(result, 0x44, 0xF7F0B4, 0xC, [&](uint32_t, uint32_t obj) { apiListFromCount<0x28>(obj, 0xF7F0DC, *count); });
}
void makeUserMedal(SharedPtr *result, const uint32_t *count) {
    makeShared(result, 0x44, 0xF7F120, 0xC, [&](uint32_t, uint32_t obj) { apiListFromCount<0x48>(obj, 0xF7F148, *count); });
}
void makeUserAvatarParts(SharedPtr *result, const uint32_t *count) {
    makeShared(result, 0x44, 0xF7F1B4, 0xC, [&](uint32_t, uint32_t obj) { apiListFromCount<0x18>(obj, 0xF7F1DC, *count); });
}

void makeRet(SharedPtr *result) {
    makeShared(result, 0x4C, 0xF7F388, 0xC, [](uint32_t block, uint32_t) {
        save<uint8_t>(block + 0x10, 1);
        memclear(block + 0x14, 0x24);
        save<uint8_t>(block + 0x4B, load<uint8_t>(block + 0x4B) & 0xFD);
    });
}

void makeJsonDocument(SharedPtr *result) {
    makeShared(result, 0x3C, 0xF7F5FC, 0xC, [](uint32_t block, uint32_t) {
        save<uint32_t>(block + 0x18, 0);
        rw::call(JsonStack_ctor, {block + 0x1C, 0, 0x400});
        save<uint32_t>(block + 0x34, 0);
        save<uint32_t>(block + 0x38, 0);
    });
}

void makeCipher(SharedPtr *result, uint32_t key) {
    makeShared(result, 0x30, 0xF7F6A0, 0xC,
               [&](uint32_t, uint32_t obj) { rw::call(0x4E0671, {obj, key}); });   // Cipher::Cipher(const array<uint8_t, 32>&)
}

void makePartyInfo(SharedPtr *result) {
    makeShared(result, 0x1A8, 0xF7F714, 0x10, [](uint32_t block, uint32_t) {
        for (uint32_t off : {0x28u, 0x24u, 0x20u, 0x3Cu, 0x38u, 0x34u, 0x50u, 0x54u, 0x58u}) save<uint32_t>(block + off, 0);
        save<uint8_t>(block + 0x63, load<uint8_t>(block + 0x63) & 0xFD);
        save<uint8_t>(block + 0x6B, load<uint8_t>(block + 0x6B) & 0xFD);
        save<uint32_t>(block + 0x70, 0);
        save<uint32_t>(block + 0x74, 0);
        save<uint32_t>(block + 0x78, 0);
        save<uint8_t>(block + 0x19F, load<uint8_t>(block + 0x19F) & 0xFD);
    });
}

// hole::network::api::StageStart(const StageStart&)
void makeStageStart(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x78, 0xF7F7C0, 0x10, [&](uint32_t, uint32_t obj) {
        safeBufferCopy(obj, 0xF7F4C8, src);
        for (uint32_t k = 0; k < 12; k += 4) save<uint32_t>(obj + 0x20 + k, load<uint32_t>(src + 0x20 + k));
        for (uint32_t k = 0; k < 16; k += 4) save<uint32_t>(obj + 0x30 + k, load<uint32_t>(src + 0x30 + k));
        save<uint32_t>(obj + 0x44, load<uint32_t>(src + 0x44));
        save<uint32_t>(obj + 0x40, load<uint32_t>(src + 0x40));
        save<uint8_t>(obj + 0x48, load<uint8_t>(src + 0x48));
        rw::call(VectorInt_copy, {obj + 0x4C, src + 0x4C});
        rw::call(VectorInt_copy, {obj + 0x58, src + 0x58});
    });
}

void makeUserAlbum(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x34, 0xF7F7E8, 0xC, [&](uint32_t, uint32_t obj) {
        safeBufferCopy(obj, 0xF7F500, src);
        save<uint32_t>(obj + 0x20, load<uint32_t>(src + 0x20));
        save<uint32_t>(obj + 0x24, load<uint32_t>(src + 0x24));
    });
}

void makeUserKeybladeCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x44, 0xF7EFDC, 0xC, [&](uint32_t, uint32_t obj) { apiListCopy(obj, 0xF7F004, src, MapU64Int_copy); });
}
void makeUserMaterialCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x44, 0xF7F048, 0xC, [&](uint32_t, uint32_t obj) { apiListCopy(obj, 0xF7F070, src, MapU64Int_copy); });
}
void makeUserSkillCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x44, 0xF7F0B4, 0xC, [&](uint32_t, uint32_t obj) { apiListCopy(obj, 0xF7F0DC, src, MapU64Int_copy); });
}
void makeUserMedalCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x44, 0xF7F120, 0xC, [&](uint32_t, uint32_t obj) { apiListCopy(obj, 0xF7F148, src, MapU64Int_copy); });
}
void makeUserAvatarPartsCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x44, 0xF7F1B4, 0xC, [&](uint32_t, uint32_t obj) { apiListCopy(obj, 0xF7F1DC, src, MapU64Int_copy); });
}
void makeUserAvatarAllCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x44, 0xF7F810, 0xC, [&](uint32_t, uint32_t obj) { apiListCopy(obj, 0xF7F538, src, MapIntInt_copy); });
}

void makeUserStoneCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x2C, 0xF7F838, 0xC, [&](uint32_t, uint32_t obj) { safeBufferCopy(obj, 0xF7F51C, src); });
}
void makeStageResumptionCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x2C, 0xF7F860, 0xC, [&](uint32_t, uint32_t obj) { safeBufferCopy(obj, 0xF7F570, src); });
}
void makeUserDetailCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x2C, 0xF7F888, 0xC, [&](uint32_t, uint32_t obj) { safeBufferCopy(obj, 0xF7F58C, src); });
}
void makeUserPointCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x2C, 0xF7F8B0, 0xC, [&](uint32_t, uint32_t obj) { safeBufferCopy(obj, 0xF7F5A8, src); });
}
void makeUserCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x2C, 0xF7F8D8, 0xC, [&](uint32_t, uint32_t obj) { safeBufferCopy(obj, 0xF7F5C4, src); });
}
void makeLoginCopy(SharedPtr *result, uint32_t src) {
    makeShared(result, 0x2C, 0xF7F900, 0xC, [&](uint32_t, uint32_t obj) { safeBufferCopy(obj, 0xF7F5E0, src); });
}

void makeTransaction(SharedPtr *result) {
    makeShared(result, 0x54, 0xF7F96C, 0xC, [](uint32_t block, uint32_t) {
        save<uint32_t>(block + 0x18, 0);
        save<uint32_t>(block + 0x14, 0);
        save<uint32_t>(block + 0x10, 0);
        save<uint32_t>(block + 0xC, 0);
        memclear(block + 0x20, 0x34);
    });
}

void makeEmptyData(SharedPtr *result) {
    makeShared(result, 0x38, 0xF7E1C4, 0x10, [](uint32_t block, uint32_t obj) {
        save<uint32_t>(block + 0x30, 0);   // no deleter
        save<uint32_t>(obj, 0xF7E1EC);     // vtable for bg::Data
        save<uint32_t>(obj + 4, 0);
        save<uint32_t>(obj + 8, 0);
    });
}

void makeStoreManagerImpl(SharedPtr *result) {
    makeShared(result, 0x50, 0xF7FB08, 0x10,
               [](uint32_t, uint32_t obj) { rw::call(0x4E1525, {obj}); });   // iap::StoreManagerImpl::StoreManagerImpl
}
void makeFacebookManagerImpl(SharedPtr *result) {
    makeShared(result, 0xC8, 0xF7FB30, 0x10,
               [](uint32_t, uint32_t obj) { rw::call(0x4E2D19, {obj}); });   // BGFacebook::FacebookManagerImpl::FacebookManagerImpl
}

// AppState's saved states: a SafeBuffer of `size` bytes (or copied from saved data) under the state's vtable
template <uint32_t Block, uint32_t Size, uint32_t Vtable> void makeStateOfSize(SharedPtr *result) {
    makeShared(result, 0x2C, Block, 0xC, [](uint32_t, uint32_t obj) {
        rw::call(SafeBuffer_ctor, {obj, Size, 1});
        save<uint32_t>(obj, Vtable);
    });
}
template <uint32_t Block, uint32_t Vtable> void makeStateFromData(SharedPtr *result, uint32_t data) {
    makeShared(result, 0x2C, Block, 0xC, [&](uint32_t, uint32_t obj) {
        rw::call(SafeBuffer_fromData, {obj, data});
        save<uint32_t>(obj, Vtable);
    });
}

void makeOptionState(SharedPtr *result) {
    makeShared(result, 0x18, 0xF7FC2C, 0xC, [](uint32_t, uint32_t obj) {
        save<uint32_t>(obj, ONE_F);       // volumes
        save<uint32_t>(obj + 4, ONE_F);
        save<uint8_t>(obj + 9, 1);
        save<uint8_t>(obj + 8, 1);
        save<uint8_t>(obj + 10, 1);
    });
}

// filter states built from their save key: their own constructors
template <uint32_t Size, uint32_t Block, uint32_t Ctor> void makeFilterState(SharedPtr *result, const uint32_t *key) {
    makeShared(result, Size, Block, 0xC, [&](uint32_t, uint32_t obj) { rw::call(Ctor, {obj, *key}); });
}

template <uint32_t Block, uint32_t Ctor> void makeInfo(SharedPtr *result) {
    makeShared(result, 0x2C, Block, 0xC, [](uint32_t, uint32_t obj) { rw::call(Ctor, {obj}); });
}

// StageState: SafeBuffer base + two more vtables (multiple inheritance) + an empty std::set
void makeStageState(SharedPtr *result) {
    makeShared(result, 0x50, 0xF7FE48, 0xC, [](uint32_t block, uint32_t obj) {
        constexpr uint32_t ZTV = 0xF7FDA0;   // vtable for StageState
        rw::call(SafeBuffer_ctor, {obj, 0x464C, 1});
        save<uint32_t>(obj, ZTV + 8);
        save<uint32_t>(obj + 0x20, ZTV + 0x38);
        save<uint32_t>(obj + 0x24, ZTV + 0x54);
        save<uint32_t>(obj + 0x38, block + 0x48);   // set: begin node = end node
        save<uint32_t>(obj + 0x3C, 0);
        save<uint32_t>(obj + 0x40, 0);
    });
}

void makeMemoryInputStreamFromData(SharedPtr *result, uint32_t data) {
    makeShared(result, 0x38, 0xF7FE70, 0x10, [&](uint32_t, uint32_t obj) {
        rw::call(0x8DC3F9, {obj, data});   // bg::MemoryInputStream::MemoryInputStream(const shared_ptr<bg::Data>&)
    });
}

// AvatarIconCache(texture, offset, scale, ..., {Avatar::Expression 1}): the expression list is a temporary vector
void makeAvatarIconCache(SharedPtr *result, const uint32_t *texture, uint32_t offset, const uint32_t *f1,
                         const uint32_t *f2) {
    makeShared(result, 0x70, 0xF82FC4, 0x10, [&](uint32_t, uint32_t obj) {
        const uint32_t a = *f2, b = *f1, tex = *texture;
        guest::Frame frame(0x10);
        guest::Vector *expressions = frame.at<guest::Vector>(0);
        *frame.at<uint32_t>(0) = 0;
        *frame.at<uint32_t>(4) = 0;
        *frame.at<uint32_t>(8) = 0;
        const uint32_t p = guest::newObject(4);
        *frame.at<uint32_t>(0) = p;
        *frame.at<uint32_t>(4) = p;
        *frame.at<uint32_t>(8) = p + 4;
        if (p) save<uint32_t>(p, 1);   // Avatar::Expression 1
        *frame.at<uint32_t>(4) = p + 4;
        // AvatarIconCache::AvatarIconCache(Texture2D*, const Vec2&, float, float, const vector<Expression>&)
        rw::call(0x5CA741, {obj, tex, offset, b, a, frame.addr()});
        rw::call(ExpressionVector_dtor, {rw::guest_addr(expressions)});
    });
}

void makeAudioLayer(SharedPtr *result, uint32_t manager) {
    makeShared(result, 0x20, 0xF8ED64, 0xC, [&](uint32_t block, uint32_t obj) {
        save<uint32_t>(obj, manager);
        save<uint32_t>(block + 0x14, 0);
        save<uint32_t>(block + 0x18, 0);
        save<uint32_t>(block + 0x10, block + 0x14);
        save<uint32_t>(block + 0x1C, ONE_F);
    });
}

void makeUserMedalData(SharedPtr *result) {
    makeShared(result, 0x70, 0xFA97DC, 0x10, [](uint32_t block, uint32_t) {
        save<uint32_t>(block + 0x38, 0);
        save<uint32_t>(block + 0x3C, 0);
        save<uint32_t>(block + 0x40, 0);
        memclear(block + 0x18, 0x1F);
        memclear(block + 0x58, 0x18);
    });
}

template <uint32_t Block, uint32_t Size, uint32_t Vtable> void makeUserItemData(SharedPtr *result) {
    makeShared(result, 0x34, Block, 0xC, [](uint32_t, uint32_t obj) {
        rw::call(SafeBuffer_ctor, {obj, Size, 1});
        save<uint32_t>(obj, Vtable);
        save<uint32_t>(obj + 0x20, 0);
        save<uint32_t>(obj + 0x24, 0);
    });
}

void makeMemoryInputStream(SharedPtr *result, const uint32_t *bytes, const uint32_t *size, uint32_t deleter) {
    makeShared(result, 0x38, 0xF7FE70, 0x10, [&](uint32_t block, uint32_t obj) {
        save<uint32_t>(obj, 0xFCCCA4);   // vtable for bg::MemoryInputStream
        save<uint32_t>(obj + 4, *bytes);
        save<uint32_t>(obj + 8, *size);
        save<uint32_t>(obj + 0xC, 0);    // position
        rw::call(Function_copy, {block + 0x20, deleter});
    });
}

void makeFileInputStream(SharedPtr *result, const uint32_t *path, const uint8_t *flag) {
    makeShared(result, 0x14, 0xFCCCF4, 0xC, [&](uint32_t, uint32_t obj) {
        const uint32_t f = *flag, p = *path;
        rw::call(0x8DC115, {obj, p, f});   // bg::FileInputStream::FileInputStream(const char*, bool)
    });
}

void makeAssetInputStream(SharedPtr *result, const uint32_t *path) {
    makeShared(result, 0x14, 0xFCCD1C, 0xC, [&](uint32_t, uint32_t obj) {
        save<uint32_t>(obj, 0xFCCC5C);   // vtable for bg::AssetInputStream
        save<uint32_t>(obj + 4, rw::call(AASSETMANAGER_OPEN, {load<uint32_t>(AssetInputStream_assetManager), *path, 0}));
    });
}

// shared_ptr<LWF::LWF>::make_shared(shared_ptr<Data>&, shared_ptr<LWFRendererFactory>&, void*&): LWF(data, factory,
// p) takes both shared_ptrs by value: copies on the stack, released after the constructor
void makeLwf(SharedPtr *result, uint32_t data, uint32_t factory, uint32_t p) {
    makeShared(result, 0x270, 0xFAF6F0, 0x10, [&](uint32_t, uint32_t obj) {
        guest::Frame frame(16);
        const uint32_t factoryArg = frame.addr(0), dataArg = frame.addr(8);
        libcxx::copyShared(dataArg, data);
        libcxx::copyShared(factoryArg, factory);
        rw::call(0x7FD26D, {obj, dataArg, factoryArg, load<uint32_t>(p)});   // LWF::LWF(shared_ptr<Data>, shared_ptr<IRendererFactory>, void*)
        libcxx::destroyShared(factoryArg);
        libcxx::destroyShared(dataArg);
    });
}

// shared_ptr<LWF::LWFRendererFactory>::make_shared(LWFNode*&&): the vtable, the node, 0x18 zeroed bytes
void makeLwfRendererFactory(SharedPtr *result, uint32_t node) {
    makeShared(result, 0x34, 0xFAF718, 0xC, [&](uint32_t, uint32_t obj) {
        save<uint32_t>(obj, 0xFAF240);   // vtable for LWF::LWFRendererFactory
        save<uint32_t>(obj + 4, load<uint32_t>(node));
        memclear(obj + 8, 0x18);
    });
}

// shared_ptr<LWF::LWFContainer>::make_shared(Movie*&&, shared_ptr<LWF>&): the shared_ptr by value (a stack copy)
void makeLwfContainer(SharedPtr *result, uint32_t movie, uint32_t lwf) {
    makeShared(result, 0x148, 0xFB0F0C, 0xC, [&](uint32_t, uint32_t obj) {
        guest::Frame frame(8);
        libcxx::copyShared(frame.addr(), lwf);
        rw::call(0x81013D, {obj, load<uint32_t>(movie), frame.addr()});   // LWFContainer(Movie*, shared_ptr<LWF>)
        libcxx::destroyShared(frame.addr());
    });
}

}  // namespace

RW_FUNCTION(0x45A670, "std::shared_ptr<bg::MemoryStream>::make_shared<>", makeMemoryStream);
RW_FUNCTION(0x485624, "std::shared_ptr<StartDeckEditDialog>::make_shared<>", makeStartDeckEditDialog);
RW_FUNCTION(0x491B78, "std::shared_ptr<bg::Data>::make_shared<void*&, unsigned&, function<void(void*)>&>", makeData);
RW_FUNCTION(0x8DD040, "std::shared_ptr<bg::Data>::make_shared<unsigned char*&, unsigned&, function<void(void*)>&>", makeData);
RW_FUNCTION(0x8DD0AC, "std::shared_ptr<bg::Data>::make_shared<char*&, int, function<void(void*)>&>", makeData);
RW_FUNCTION(0x4D7928, "std::shared_ptr<bg::Data>::make_shared<unsigned char*&, unsigned&, void(&)(void*)>", makeDataWithFreeFunction);
RW_FUNCTION(0x4DF884, "std::shared_ptr<bg::Data>::make_shared<void*&, unsigned&, void(&)(void*)>", makeDataWithFreeFunction);
RW_FUNCTION(0x4E0A3C, "std::shared_ptr<bg::Data>::make_shared<>", makeEmptyData);
RW_FUNCTION(0x49A510, "std::shared_ptr<hole::network::RESTClient::Result>::make_shared<>", makeRestResult);
RW_FUNCTION(0x49B5E4, "std::shared_ptr<hole::network::RESTClient::Connection>::make_shared<>", makeRestConnection);
RW_FUNCTION(0x4AE4A0, "std::shared_ptr<hole::network::api::UserLink>::make_shared<>", makeUserLink);
RW_FUNCTION(0x4B02A0, "std::shared_ptr<hole::network::api::UserTitle>::make_shared<unsigned>", makeUserTitle);
RW_FUNCTION(0x4B0814, "std::shared_ptr<hole::network::api::UserKeyblade>::make_shared<unsigned>", makeUserKeyblade);
RW_FUNCTION(0x4B0A3C, "std::shared_ptr<hole::network::api::UserMaterial>::make_shared<unsigned>", makeUserMaterial);
RW_FUNCTION(0x4B0C34, "std::shared_ptr<hole::network::api::UserSkill>::make_shared<unsigned>", makeUserSkill);
RW_FUNCTION(0x4B0E2C, "std::shared_ptr<hole::network::api::UserMedal>::make_shared<unsigned>", makeUserMedal);
RW_FUNCTION(0x4B1574, "std::shared_ptr<hole::network::api::UserAvatarParts>::make_shared<unsigned>", makeUserAvatarParts);
RW_FUNCTION(0x4B1C84, "std::shared_ptr<hole::network::api::Ret>::make_shared<>", makeRet);
RW_FUNCTION(0x4B9344, "std::shared_ptr<rapidjson::Document>::make_shared<>", makeJsonDocument);
RW_FUNCTION(0x4B975C, "std::shared_ptr<Cipher>::make_shared<array<unsigned char, 32>&>", makeCipher);
RW_FUNCTION(0x4D5618, "std::shared_ptr<APIManager::PartyInfo>::make_shared<>", makePartyInfo);
RW_FUNCTION(0x4D5CF8, "std::shared_ptr<hole::network::api::StageStart>::make_shared<StageStart&>", makeStageStart);
RW_FUNCTION(0x4D5EB4, "std::shared_ptr<hole::network::api::UserAlbum>::make_shared<UserAlbum&>", makeUserAlbum);
RW_FUNCTION(0x4D5FB4, "std::shared_ptr<hole::network::api::UserKeyblade>::make_shared<UserKeyblade&>", makeUserKeybladeCopy);
RW_FUNCTION(0x4D6218, "std::shared_ptr<hole::network::api::UserMaterial>::make_shared<UserMaterial&>", makeUserMaterialCopy);
RW_FUNCTION(0x4D62D0, "std::shared_ptr<hole::network::api::UserSkill>::make_shared<UserSkill&>", makeUserSkillCopy);
RW_FUNCTION(0x4D6388, "std::shared_ptr<hole::network::api::UserMedal>::make_shared<UserMedal&>", makeUserMedalCopy);
RW_FUNCTION(0x4D6440, "std::shared_ptr<hole::network::api::UserAvatarParts>::make_shared<UserAvatarParts&>", makeUserAvatarPartsCopy);
RW_FUNCTION(0x4D64F8, "std::shared_ptr<hole::network::api::UserAvatarAll>::make_shared<UserAvatarAll&>", makeUserAvatarAllCopy);
RW_FUNCTION(0x4D66F0, "std::shared_ptr<hole::network::api::UserStone>::make_shared<UserStone&>", makeUserStoneCopy);
RW_FUNCTION(0x4D67E8, "std::shared_ptr<hole::network::api::StageResumption>::make_shared<StageResumption&>", makeStageResumptionCopy);
RW_FUNCTION(0x4D68E0, "std::shared_ptr<hole::network::api::UserDetail>::make_shared<UserDetail&>", makeUserDetailCopy);
RW_FUNCTION(0x4D69D8, "std::shared_ptr<hole::network::api::UserPoint>::make_shared<UserPoint&>", makeUserPointCopy);
RW_FUNCTION(0x4D6AD0, "std::shared_ptr<hole::network::api::User>::make_shared<User&>", makeUserCopy);
RW_FUNCTION(0x4D6BC8, "std::shared_ptr<hole::network::api::Login>::make_shared<Login&>", makeLoginCopy);
RW_FUNCTION(0x4DA06C, "std::shared_ptr<PurchaseManager::Transaction>::make_shared<>", makeTransaction);
RW_FUNCTION(0x4E2510, "std::shared_ptr<iap::StoreManagerImpl>::make_shared<>", makeStoreManagerImpl);
RW_FUNCTION(0x4E618C, "std::shared_ptr<BGFacebook::FacebookManagerImpl>::make_shared<>", makeFacebookManagerImpl);
RW_FUNCTION(0x4EDB88, "std::shared_ptr<AppState::SkillSortFilterState>::make_shared<>", (makeStateOfSize<0xF7FBA4, 8, 0xF7FBCC>));
RW_FUNCTION(0x4EDC88, "std::shared_ptr<AppState::KeyBladeSortFilterState>::make_shared<>", (makeStateOfSize<0xF7FBE8, 8, 0xF7FC10>));
RW_FUNCTION(0x4EE7F8, "std::shared_ptr<AppState::PersonalInfo>::make_shared<>", (makeStateOfSize<0xF7FCCC, 0x10, 0xF7FCF4>));
RW_FUNCTION(0x4EDEDC, "std::shared_ptr<AppState::SkillSortFilterState>::make_shared<shared_ptr<bg::Data>&>", (makeStateFromData<0xF7FBA4, 0xF7FBCC>));
RW_FUNCTION(0x4EDF54, "std::shared_ptr<AppState::KeyBladeSortFilterState>::make_shared<shared_ptr<bg::Data>&>", (makeStateFromData<0xF7FBE8, 0xF7FC10>));
RW_FUNCTION(0x4EE49C, "std::shared_ptr<AppState::PersonalInfo>::make_shared<shared_ptr<bg::Data>&>", (makeStateFromData<0xF7FCCC, 0xF7FCF4>));
RW_FUNCTION(0x4EDE7C, "std::shared_ptr<AppState::OptionState>::make_shared<>", makeOptionState);
RW_FUNCTION(0x4EDFCC, "std::shared_ptr<AppState::AdventureSelectFilterState>::make_shared<const char*&>", (makeFilterState<0x20, 0xF7FC54, 0x4EC415>));
RW_FUNCTION(0x4EE0BC, "std::shared_ptr<AppState::AvatarBoardFilterState>::make_shared<const char*&>", (makeFilterState<0x28, 0xF7FC7C, 0x4EC3A9>));
RW_FUNCTION(0x4EE1AC, "std::shared_ptr<AppState::MedalSortFilterState>::make_shared<const char*&>", (makeFilterState<0x30, 0xF7FCA4, 0x4EC121>));
RW_FUNCTION(0x4EE640, "std::shared_ptr<AppState::LoginInfo>::make_shared<>", (makeInfo<0xF7FD50, 0x4EA9A1>));
RW_FUNCTION(0x4EE71C, "std::shared_ptr<AppState::BRIDGEInfo>::make_shared<>", (makeInfo<0xF7FD78, 0x4EA9E9>));
RW_FUNCTION(0x4F0B6C, "std::shared_ptr<StageState>::make_shared<>", makeStageState);
RW_FUNCTION(0x4F11F8, "std::shared_ptr<bg::MemoryInputStream>::make_shared<const shared_ptr<bg::Data>&>", makeMemoryInputStreamFromData);
RW_FUNCTION(0x509898, "std::shared_ptr<AvatarIconCache>::make_shared<Texture2D*&, Vec2, float, float>", makeAvatarIconCache);
RW_FUNCTION(0x5BFD34, "std::shared_ptr<audio::Layer>::make_shared<audio::Manager&>", makeAudioLayer);
RW_FUNCTION(0x76B528, "std::shared_ptr<MedalDataManager::UserMedalData>::make_shared<>", makeUserMedalData);
RW_FUNCTION(0x76B870, "std::shared_ptr<KeyBladeDataManager::UserKeyBladeData>::make_shared<>", (makeUserItemData<0xFA9804, 8, 0xFA982C>));
RW_FUNCTION(0x76BEF4, "std::shared_ptr<SkillDataManager::UserSkillData>::make_shared<>", (makeUserItemData<0xFA9848, 0x28, 0xFA9870>));
RW_FUNCTION(0x8DC67C, "std::shared_ptr<bg::MemoryInputStream>::make_shared<const void*&, unsigned&, const function<void(void*)>&>", makeMemoryInputStream);
RW_FUNCTION(0x8DC6E8, "std::shared_ptr<bg::FileInputStream>::make_shared<const char*&, bool&>", makeFileInputStream);
RW_FUNCTION(0x8DC7E0, "std::shared_ptr<bg::AssetInputStream>::make_shared<const char*&>", makeAssetInputStream);
RW_FUNCTION(0x7EF6C0, "std::shared_ptr<LWF::LWF>::make_shared<shared_ptr<Data>&, shared_ptr<LWFRendererFactory>&, void*&>", makeLwf);
RW_FUNCTION(0x7EF788, "std::shared_ptr<LWF::LWFRendererFactory>::make_shared<LWFNode*>", makeLwfRendererFactory);
RW_FUNCTION(0x815188, "std::shared_ptr<LWF::LWFContainer>::make_shared<Movie*, shared_ptr<LWF>&>", makeLwfContainer);
