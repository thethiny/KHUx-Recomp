// std::__split_buffer instances (the growth buffer of vector and the block map of deque) over the game's element
// types. Layout: first +0, begin +4, end +8, end of capacity +0xC, then the allocator: empty and at +0xC (its own
// allocator) or a reference at +0x10 (__split_buffer<T, allocator<T>&>). The algorithms are libc++ 3.5's.
#include "libcxx/SplitBuffer.hpp"

using libcxx::destroyShared;
using rw::load;
using rw::save;

namespace {

using namespace libcxx::split;

// ~__split_buffer of trivially destructible Size-byte elements: end back to begin (one store, as the compiler
// folds it), then the buffer freed. Returns this.
template <uint32_t Size> uint32_t trivialDtor(uint32_t sb) {
    const uint32_t begin = load<uint32_t>(sb + BEGIN), end = load<uint32_t>(sb + END);
    if (end != begin) save<uint32_t>(sb + END, end + (~(end - Size - begin) & ~(Size - 1)));
    if (const uint32_t first = load<uint32_t>(sb + FIRST)) guest::deleteObject(first);
    return sb;
}

// ... of shared_ptrs: from the back, end steps down (stored each time) and the element at the new end is released
uint32_t sharedDtor(uint32_t sb) {
    const uint32_t begin = load<uint32_t>(sb + BEGIN);
    while (load<uint32_t>(sb + END) != begin) {
        const uint32_t e = load<uint32_t>(sb + END) - 8;
        save<uint32_t>(sb + END, e);
        destroyShared(e);
    }
    if (const uint32_t first = load<uint32_t>(sb + FIRST)) guest::deleteObject(first);
    return sb;
}

// ~__split_buffer of elements with a destructor (from the back: end stepped down and stored, then the element at
// the new end destroyed), then the buffer freed. Returns this.
template <uint32_t Size, void (*Destroy)(uint32_t)> uint32_t elementDtor(uint32_t sb) {
    const uint32_t begin = load<uint32_t>(sb + BEGIN);
    while (load<uint32_t>(sb + END) != begin) {
        const uint32_t e = load<uint32_t>(sb + END) - Size;
        save<uint32_t>(sb + END, e);
        Destroy(e);
    }
    if (const uint32_t first = load<uint32_t>(sb + FIRST)) guest::deleteObject(first);
    return sb;
}
// ResourceInfo's file record (0x1C bytes: strings at 0 and 0xC) and Entry (two vector<record>, at 4 and 0x10)
void recordDestroy(uint32_t e) {
    libcxx::destroyString(e + 0xC);
    libcxx::destroyString(e);
}
constexpr uint32_t RecordVector_dtor = 0x7762BD;   // __vector_base<record>::~__vector_base
void entryDestroy(uint32_t e) {
    rw::call(RecordVector_dtor, {e + 0x10});
    rw::call(RecordVector_dtor, {e + 4});
}

}  // namespace

// ---------------------------------------------------------------------------------------------- destructors
RW_FUNCTION(0x47D818, "std::__split_buffer<int&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x487B7A, "std::__split_buffer<shared_ptr<MedalDataManager::UserMedalData>&>::~__split_buffer", sharedDtor);
RW_FUNCTION(0x49A1F8, "std::__split_buffer<pair<shared_ptr<RESTClient::Connection>, shared_ptr<RESTClient::Result>>*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x49B324, "std::__split_buffer<shared_ptr<RESTClient::Connection>*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x49BB8C, "std::__split_buffer<pair<shared_ptr<RESTClient::Connection>, shared_ptr<RESTClient::Result>>*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x49BBB8, "std::__split_buffer<shared_ptr<RESTClient::Connection>*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x4B350C, "std::__split_buffer<unsigned long long&>::~__split_buffer", trivialDtor<8>);
RW_FUNCTION(0x4BA0B4, "std::__split_buffer<Client::Request*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x4BA0E0, "std::__split_buffer<Client::Request*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5C48D4, "std::__split_buffer<FlashNode::ActFlag*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5C560C, "std::__split_buffer<FlashNode::ActFlag*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5C5638, "std::__split_buffer<int*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5DB1A0, "std::__split_buffer<MapNode*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5DBD54, "std::__split_buffer<MapPrizes*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5DEF70, "std::__split_buffer<MapPrize*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5F95BC, "std::__split_buffer<AvatarActor*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x60411A, "std::__split_buffer<string*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x6048F8, "std::__split_buffer<string*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x70C7E8, "std::__split_buffer<ImageButton*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x739A28, "std::__split_buffer<LWF::Movie*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x7586A4, "std::__split_buffer<MapGateMarker*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x7587E0, "std::__split_buffer<MapNonPlayableCharacter*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x75891C, "std::__split_buffer<MapTreasure*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x758A58, "std::__split_buffer<MapMaterial*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x758B94, "std::__split_buffer<MapFieldLayer*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x75D838, "std::__split_buffer<MapNonPlayableCharacter::Info*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x75D974, "std::__split_buffer<MapMaterial::Info*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x75DAB0, "std::__split_buffer<MapTreasure::Info*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x75DBEC, "std::__split_buffer<MapTransferInfo*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x75DD28, "std::__split_buffer<MapEnemyTeamInfo*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x76E544, "std::__split_buffer<MasterDataManager::TableInfo*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x76F1D4, "std::__split_buffer<MasterDataManager::TableInfo*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x775FD4, "std::__split_buffer<shared_ptr<ResourceInfo>*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x776118, "std::__split_buffer<void const**>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x776144, "std::__split_buffer<shared_ptr<ResourceInfo>*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x7C9234, "std::__split_buffer<cocos2d::Component*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x8C7D00, "std::__split_buffer<unsigned short&>::~__split_buffer", trivialDtor<2>);
RW_FUNCTION(0x5C1FB0, "std::__split_buffer<pair<audio::ResourceID, int>*&>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x5C221A, "std::__split_buffer<pair<audio::ResourceID, int>*>::~__split_buffer", trivialDtor<4>);
RW_FUNCTION(0x776170, "std::__split_buffer<ResourceInfo Entry&>::~__split_buffer", (elementDtor<0x1C, entryDestroy>));
RW_FUNCTION(0x7768CC, "std::__split_buffer<ResourceInfo record&>::~__split_buffer", (elementDtor<0x1C, recordDestroy>));

// ---------------------------------------------------------------------------------------------- push_back / push_front
RW_FUNCTION(0x499D68, "std::__split_buffer<pair<shared_ptr<RESTClient::Connection>, shared_ptr<RESTClient::Result>>*>::push_back(&&)", (pushBack<false, 0x49A1F9>));
RW_FUNCTION(0x499E94, "std::__split_buffer<pair<shared_ptr<RESTClient::Connection>, shared_ptr<RESTClient::Result>>*>::push_front(&&)", (pushFront<false, 0x49A1F9>));
RW_FUNCTION(0x499FB4, "std::__split_buffer<pair<shared_ptr<RESTClient::Connection>, shared_ptr<RESTClient::Result>>*&>::push_back(&&)", (pushBack<true, 0x49A1F9>));
RW_FUNCTION(0x49A0DC, "std::__split_buffer<pair<shared_ptr<RESTClient::Connection>, shared_ptr<RESTClient::Result>>*&>::push_front(const&)", (pushFront<true, 0x49A1F9>));
RW_FUNCTION(0x49AE94, "std::__split_buffer<shared_ptr<RESTClient::Connection>*>::push_back(&&)", (pushBack<false, 0x49B325>));
RW_FUNCTION(0x49AFC0, "std::__split_buffer<shared_ptr<RESTClient::Connection>*>::push_front(&&)", (pushFront<false, 0x49B325>));
RW_FUNCTION(0x49B0E0, "std::__split_buffer<shared_ptr<RESTClient::Connection>*&>::push_back(&&)", (pushBack<true, 0x49B325>));
RW_FUNCTION(0x49B208, "std::__split_buffer<shared_ptr<RESTClient::Connection>*&>::push_front(const&)", (pushFront<true, 0x49B325>));
RW_FUNCTION(0x4B9C24, "std::__split_buffer<Client::Request*>::push_back(&&)", (pushBack<false, 0x4BA0B5>));
RW_FUNCTION(0x4B9D50, "std::__split_buffer<Client::Request*>::push_front(&&)", (pushFront<false, 0x4BA0B5>));
RW_FUNCTION(0x4B9E70, "std::__split_buffer<Client::Request*&>::push_back(&&)", (pushBack<true, 0x4BA0B5>));
RW_FUNCTION(0x4B9F98, "std::__split_buffer<Client::Request*&>::push_front(const&)", (pushFront<true, 0x4BA0B5>));
RW_FUNCTION(0x5C4444, "std::__split_buffer<FlashNode::ActFlag*>::push_back(&&)", (pushBack<false, 0x5C48D5>));
RW_FUNCTION(0x5C4570, "std::__split_buffer<FlashNode::ActFlag*>::push_front(&&)", (pushFront<false, 0x5C48D5>));
RW_FUNCTION(0x5C4690, "std::__split_buffer<FlashNode::ActFlag*&>::push_back(&&)", (pushBack<true, 0x5C48D5>));
RW_FUNCTION(0x5C47B8, "std::__split_buffer<FlashNode::ActFlag*&>::push_front(const&)", (pushFront<true, 0x5C48D5>));
RW_FUNCTION(0x604468, "std::__split_buffer<string*>::push_back(&&)", (pushBack<false, 0x6048F9>));
RW_FUNCTION(0x604594, "std::__split_buffer<string*>::push_front(&&)", (pushFront<false, 0x6048F9>));
RW_FUNCTION(0x6046B4, "std::__split_buffer<string*&>::push_back(&&)", (pushBack<true, 0x6048F9>));
RW_FUNCTION(0x6047DC, "std::__split_buffer<string*&>::push_front(const&)", (pushFront<true, 0x6048F9>));
RW_FUNCTION(0x741F88, "std::__split_buffer<FlashNode::ActFlag*>::push_front(const&)", (pushFront<false, 0x5C48D5>));
RW_FUNCTION(0x76E0B4, "std::__split_buffer<MasterDataManager::TableInfo*>::push_back(&&)", (pushBack<false, 0x76E545>));
RW_FUNCTION(0x76E1E0, "std::__split_buffer<MasterDataManager::TableInfo*>::push_front(&&)", (pushFront<false, 0x76E545>));
RW_FUNCTION(0x76E300, "std::__split_buffer<MasterDataManager::TableInfo*&>::push_back(&&)", (pushBack<true, 0x76E545>));
RW_FUNCTION(0x76E428, "std::__split_buffer<MasterDataManager::TableInfo*&>::push_front(const&)", (pushFront<true, 0x76E545>));
RW_FUNCTION(0x775B44, "std::__split_buffer<shared_ptr<ResourceInfo>*>::push_back(&&)", (pushBack<false, 0x775FD5>));
RW_FUNCTION(0x775C70, "std::__split_buffer<shared_ptr<ResourceInfo>*>::push_front(&&)", (pushFront<false, 0x775FD5>));
RW_FUNCTION(0x775D90, "std::__split_buffer<shared_ptr<ResourceInfo>*&>::push_back(&&)", (pushBack<true, 0x775FD5>));
RW_FUNCTION(0x775EB8, "std::__split_buffer<shared_ptr<ResourceInfo>*&>::push_front(const&)", (pushFront<true, 0x775FD5>));
