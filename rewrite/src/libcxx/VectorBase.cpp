// std::__vector_base<T>::~__vector_base for the game's element types: destroy the elements from the back, then free
// the buffer. Element layouts below are what each destructor touches (members destroyed in reverse order).
#include "libcxx/Libcxx.hpp"

using guest::Vector;
using libcxx::destroyFunction;
using libcxx::destroyShared;
using libcxx::destroyString;
using libcxx::destroyVector;

namespace {

constexpr uint32_t SafeBuffer_dtor = 0x45A13D;   // SafeBuffer::~SafeBuffer
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;

// trivially destructible elements of Size bytes
template <uint32_t Size> uint32_t trivialDtor(Vector *self) { return destroyVector<Size>(self); }

// vectors of plain values, nested in the element types below
uint32_t vectorU64Dtor(Vector *self) { return destroyVector<8>(self); }
uint32_t vectorIntDtor(Vector *self) { return destroyVector<4>(self); }
uint32_t vectorUintDtor(Vector *self) { return destroyVector<4>(self); }

// std::vector<std::shared_ptr<T>>
uint32_t sharedPtrVectorDtor(Vector *self) { return destroyVector<8>(self, destroyShared); }

uint32_t stringVectorDtor(Vector *self) { return destroyVector<12>(self, destroyString); }

uint32_t vectorOfUintVectorsDtor(Vector *self) {
    return destroyVector<12>(self, [](uint32_t e) { vectorUintDtor(rw::host_ptr<Vector>(e)); });
}
uint32_t vectorOfU64VectorsDtor(Vector *self) {
    return destroyVector<12>(self, [](uint32_t e) { vectorU64Dtor(rw::host_ptr<Vector>(e)); });
}
uint32_t vectorOfIntVectorsDtor(Vector *self) {
    return destroyVector<12>(self, [](uint32_t e) { vectorIntDtor(rw::host_ptr<Vector>(e)); });
}
uint32_t vectorOfSharedVectorsDtor(Vector *self) {
    return destroyVector<12>(self, [](uint32_t e) { sharedPtrVectorDtor(rw::host_ptr<Vector>(e)); });
}
// std::vector<std::vector<char>>: the inner destructor is inlined (same steps, one-byte elements)
uint32_t vectorOfCharVectorsDtor(Vector *self) {
    return destroyVector<12>(self, [](uint32_t e) { destroyVector<1>(rw::host_ptr<Vector>(e)); });
}

uint32_t vec2VectorDtor(Vector *self) {
    return destroyVector<8>(self, [](uint32_t e) { rw::call(Vec2_dtor, {e}); });
}

// std::function<void(bool)> / std::function<bool()> (24 bytes)
uint32_t functionVectorDtor(Vector *self) { return destroyVector<24>(self, destroyFunction); }

// element classes with a virtual destructor (slot 0, the complete-object destructor)
template <uint32_t Size> uint32_t virtualDtor(Vector *self) {
    return destroyVector<Size>(self, [](uint32_t e) { rw::vcall(rw::host_ptr<uint8_t>(e), 0, {e}); });
}

// elements whose only non-trivial members are std::strings at the given offsets (destroyed last to first)
template <uint32_t Size, uint32_t... Strings> uint32_t stringsDtor(Vector *self) {
    return destroyVector<Size>(self, [](uint32_t e) { (destroyString(e + Strings), ...); });
}
// ... only std::shared_ptrs
template <uint32_t Size, uint32_t... Shared> uint32_t sharedsDtor(Vector *self) {
    return destroyVector<Size>(self, [](uint32_t e) { (destroyShared(e + Shared), ...); });
}

// hole::network::api::PartyRanking::Info (0x78): strings 0x10, 0x24, 0x60; std::vector<uint64_t> 0x40
uint32_t partyRankingInfoDtor(Vector *self) {
    return destroyVector<0x78>(self, [](uint32_t e) {
        destroyString(e + 0x60);
        vectorU64Dtor(rw::host_ptr<Vector>(e + 0x40));
        destroyString(e + 0x24);
        destroyString(e + 0x10);
    });
}

// hole::network::api::PartyApplicationList::PartyEx (0x198): strings 0x10, 0x24, 0x60; std::vector<uint64_t> 0x40
uint32_t partyExDtor(Vector *self) {
    return destroyVector<0x198>(self, [](uint32_t e) {
        destroyString(e + 0x60);
        vectorU64Dtor(rw::host_ptr<Vector>(e + 0x40));
        destroyString(e + 0x24);
        destroyString(e + 0x10);
    });
}

// hole::network::api::RaidReward::Reward (0x148): strings 0xA0, 0xF0; shared_ptrs 0x130, 0x138, 0x140
uint32_t raidRewardDtor(Vector *self) {
    return destroyVector<0x148>(self, [](uint32_t e) {
        destroyShared(e + 0x140);
        destroyShared(e + 0x138);
        destroyShared(e + 0x130);
        destroyString(e + 0xF0);
        destroyString(e + 0xA0);
    });
}

// hole::network::api::Stage::Story (0x18): std::vector<int> at 0xC
uint32_t stageStoryVectorDtor(Vector *self) {
    return destroyVector<0x18>(self, [](uint32_t e) { vectorIntDtor(rw::host_ptr<Vector>(e + 0xC)); });
}

// hole::network::api::StageEvent::Event (0x38) / Stage::Event (0x30): std::vector<Stage::Story> at 0, string 0x14
template <uint32_t Size> uint32_t stageEventDtor(Vector *self) {
    return destroyVector<Size>(self, [](uint32_t e) {
        destroyString(e + 0x14);
        stageStoryVectorDtor(rw::host_ptr<Vector>(e));
    });
}

// Avatar::InstanceInfo (0x464): SafeBuffer at 0, string 0x20, then 45 parts of 0x18 bytes from 0x2C
// (each: a word, a shared_ptr at +4, a string at +0xC)
uint32_t avatarInstanceInfoDtor(Vector *self) {
    return destroyVector<0x464>(self, [](uint32_t e) {
        for (uint32_t part = e + 0x2C + 44 * 0x18;; part -= 0x18) {
            destroyString(part + 0xC);
            destroyShared(part + 4);
            if (part == e + 0x2C) break;
        }
        destroyString(e + 0x20);
        rw::call(SafeBuffer_dtor, {e});
    });
}

// pair<int, function<...>> (32 bytes: the function at 8)
uint32_t pairFunctionDtor(Vector *self) { return destroyVector<0x20>(self, [](uint32_t e) { destroyFunction(e + 8); }); }
// cocos2d::Label::LetterInfo (64 bytes): a Vec2 at 0x2C
uint32_t letterInfoDtor(Vector *self) { return destroyVector<0x40>(self, [](uint32_t e) { rw::call(Vec2_dtor, {e + 0x2C}); }); }

}  // namespace

// plain values and pointers
RW_FUNCTION(0x468498, "std::__vector_base<unsigned long long>::~__vector_base", vectorU64Dtor);
RW_FUNCTION(0x478960, "std::__vector_base<int>::~__vector_base", vectorIntDtor);
RW_FUNCTION(0x485ABA, "std::__vector_base<unsigned int>::~__vector_base", vectorUintDtor);
RW_FUNCTION(0x4F3324, "std::__vector_base<long>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x4F59FE, "std::__vector_base<float>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x8CAC22, "std::__vector_base<unsigned short>::~__vector_base", trivialDtor<2>);
RW_FUNCTION(0x473610, "std::__vector_base<Date>::~__vector_base", trivialDtor<8>);
RW_FUNCTION(0x46BE0C, "std::__vector_base<Avatar::Expression>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x485A38, "std::__vector_base<Avatar::Motion>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x46BE36, "std::__vector_base<hole::network::api::UserSkill::Skill>::~__vector_base", trivialDtor<0x28>);
RW_FUNCTION(0x4AE7F6, "std::__vector_base<hole::network::api::RankingReward::Result>::~__vector_base", trivialDtor<0xC>);
RW_FUNCTION(0x4B1F9A, "std::__vector_base<hole::network::api::MypageInfo::OpenBanner>::~__vector_base", trivialDtor<0x10>);
RW_FUNCTION(0x4B38B4, "std::__vector_base<hole::network::api::ColosseumClear::ClearColosseum>::~__vector_base", trivialDtor<8>);
RW_FUNCTION(0x4B3A94, "std::__vector_base<hole::network::api::Colosseum::Stage>::~__vector_base", trivialDtor<0x10>);
RW_FUNCTION(0x4B3C08, "std::__vector_base<hole::network::api::UserShop::Benefit>::~__vector_base", trivialDtor<0xC>);
RW_FUNCTION(0x4C50D0, "std::__vector_base<hole::network::api::UserMaterial::Material>::~__vector_base", trivialDtor<0x10>);
RW_FUNCTION(0x4C50FE, "std::__vector_base<hole::network::api::UserKeyblade::Keyblade>::~__vector_base", trivialDtor<0x58>);
RW_FUNCTION(0x4C513C, "std::__vector_base<hole::network::api::UserMedal::Medal>::~__vector_base", trivialDtor<0x48>);
RW_FUNCTION(0x4C517A, "std::__vector_base<hole::network::api::UserAvatarParts::Part>::~__vector_base", trivialDtor<0x18>);
RW_FUNCTION(0x4C51B8, "std::__vector_base<hole::network::api::UserTitle::Title>::~__vector_base", trivialDtor<0x10>);
RW_FUNCTION(0x703EE0, "std::__vector_base<hole::network::api::UserAvatar::Coordinate>::~__vector_base", trivialDtor<0x30>);
RW_FUNCTION(0x4F59D0, "std::__vector_base<IScrollProtocol::UpdateData>::~__vector_base", trivialDtor<8>);
RW_FUNCTION(0x51976C, "std::__vector_base<MyRecordScrollView::RecordData>::~__vector_base", trivialDtor<0x18>);
RW_FUNCTION(0x525830, "std::__vector_base<BaseSlideView::UpdateTarget>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5329D4, "std::__vector_base<APIManager::Timezone>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x57DA50, "std::__vector_base<BattleResultDialog::LevelUpSkill>::~__vector_base", trivialDtor<0x10>);
RW_FUNCTION(0x59BD10, "std::__vector_base<BaseStageScrollView::UpdateData>::~__vector_base", trivialDtor<8>);
RW_FUNCTION(0x485B78, "std::__vector_base<cocos2d::ui::Widget*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x485BD6, "std::__vector_base<AvatarActor*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x495294, "std::__vector_base<cocos2d::LWFNode*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x4960B6, "std::__vector_base<MapPlacementObject*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x52C438, "std::__vector_base<DeckMedalView*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5D4FD6, "std::__vector_base<const char*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5DB1CC, "std::__vector_base<MapNode*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x607F5C, "std::__vector_base<MapNonPlayableCharacter::Info*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x607F86, "std::__vector_base<MapMaterial::Info*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x607FB0, "std::__vector_base<MapTreasure::Info*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x607FDA, "std::__vector_base<MapTransferInfo*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x608004, "std::__vector_base<MapEnemyTeamInfo*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x65E564, "std::__vector_base<cocos2d::ui::ImageView*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x748C1A, "std::__vector_base<IBuffProtocol*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x748C44, "std::__vector_base<IStatusProtocol*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x74EACC, "std::__vector_base<EnemyActor*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x75C1FE, "std::__vector_base<MapJumpInfo*>::~__vector_base", trivialDtor<4>);

// shared_ptrs, strings, nested vectors, functions
RW_FUNCTION(0x46B118, "std::__vector_base<shared_ptr<MedalDataManager::UserMedalData>>::~__vector_base", sharedPtrVectorDtor);
RW_FUNCTION(0x498FDE, "std::__vector_base<shared_ptr<hole::network::RESTClient::Connection>>::~__vector_base", sharedPtrVectorDtor);
RW_FUNCTION(0x50248C, "std::__vector_base<shared_ptr<const master::Sphere>>::~__vector_base", sharedPtrVectorDtor);
RW_FUNCTION(0x573590, "std::__vector_base<shared_ptr<const master::EvCampaign>>::~__vector_base", sharedPtrVectorDtor);
RW_FUNCTION(0x6F6428, "std::__vector_base<shared_ptr<const master::LoginBonus>>::~__vector_base", sharedPtrVectorDtor);
RW_FUNCTION(0x709624, "std::__vector_base<shared_ptr<const master::AvatarParts>>::~__vector_base", sharedPtrVectorDtor);
RW_FUNCTION(0x47363E, "std::__vector_base<std::string>::~__vector_base", stringVectorDtor);
RW_FUNCTION(0x485AE4, "std::__vector_base<std::vector<unsigned int>>::~__vector_base", vectorOfUintVectorsDtor);
RW_FUNCTION(0x51B014, "std::__vector_base<std::vector<unsigned long long>>::~__vector_base", vectorOfU64VectorsDtor);
RW_FUNCTION(0x51B044, "std::__vector_base<std::vector<int>>::~__vector_base", vectorOfIntVectorsDtor);
RW_FUNCTION(0x6ADAB0, "std::__vector_base<std::vector<shared_ptr<MedalDataManager::UserMedalData>>>::~__vector_base", vectorOfSharedVectorsDtor);
RW_FUNCTION(0x49489C, "std::__vector_base<std::vector<char>>::~__vector_base", vectorOfCharVectorsDtor);
RW_FUNCTION(0x485B14, "std::__vector_base<cocos2d::Vec2>::~__vector_base", vec2VectorDtor);
RW_FUNCTION(0x60CA60, "std::__vector_base<std::function<void(bool)>>::~__vector_base", functionVectorDtor);
RW_FUNCTION(0x60CAAA, "std::__vector_base<std::function<bool()>>::~__vector_base", functionVectorDtor);
RW_FUNCTION(0x5BABC8, "std::__vector_base<AvatarActor>::~__vector_base", virtualDtor<0x318>);
RW_FUNCTION(0x736654, "std::__vector_base<Avatar::Style>::~__vector_base", virtualDtor<0x20>);

// game structs
RW_FUNCTION(0x4AE050, "std::__vector_base<hole::network::api::UserLink::Info>::~__vector_base", (stringsDtor<0x10, 0x04>));
RW_FUNCTION(0x4B1FC8, "std::__vector_base<hole::network::api::MypageInfo::AdminText>::~__vector_base", (stringsDtor<0x14, 0x08>));
RW_FUNCTION(0x4B353C, "std::__vector_base<hole::network::api::GachaList::Gacha>::~__vector_base", (stringsDtor<0x48, 0x3C, 0x30, 0x24>));
RW_FUNCTION(0x4B3E5C, "std::__vector_base<hole::network::api::PartyInviteList::Offer>::~__vector_base", (stringsDtor<0x38, 0x10>));
RW_FUNCTION(0x4B4452, "std::__vector_base<hole::network::api::PartyApplication::Applicant>::~__vector_base", (stringsDtor<0x38, 0x10>));
RW_FUNCTION(0x4B4A34, "std::__vector_base<hole::network::api::PartyMemberList::Detail>::~__vector_base", (stringsDtor<0x98, 0x88, 0x7C>));
RW_FUNCTION(0x4B4B00, "std::__vector_base<hole::network::api::PartyNotice::NewcomerUser>::~__vector_base", (stringsDtor<0x30, 0x20, 0x00>));
RW_FUNCTION(0x4B572C, "std::__vector_base<hole::network::api::RaidStart::Player>::~__vector_base", (stringsDtor<0x48, 0x34>));
RW_FUNCTION(0x4B69DA, "std::__vector_base<hole::network::api::RaidMember::Cooperator>::~__vector_base", (stringsDtor<0x50, 0x08>));
RW_FUNCTION(0x4BA87C, "std::__vector_base<hole::network::api::RaidBase>::~__vector_base", (stringsDtor<0xF0, 0xB0>));
RW_FUNCTION(0x4DB600, "std::__vector_base<ChatManager::Block>::~__vector_base", (stringsDtor<0x44, 0x30, 0x24, 0x18, 0x0C, 0x00>));
RW_FUNCTION(0x4DBBBC, "std::__vector_base<ChatManager::Message>::~__vector_base", (stringsDtor<0x4C, 0x34, 0x28, 0x1C, 0x10, 0x04>));
RW_FUNCTION(0x511C84, "std::__vector_base<BattleResultScrollView::ResutData>::~__vector_base", (stringsDtor<0x44, 0x38, 0x10, 0x04>));
RW_FUNCTION(0x5568AC, "std::__vector_base<ChatScrollView::ChatMessage>::~__vector_base", (stringsDtor<0x40, 0x2C, 0x14>));
RW_FUNCTION(0x55A988, "std::__vector_base<ChatBlockListScrollView::ChatBlock>::~__vector_base", (stringsDtor<0x1C, 0x10>));
RW_FUNCTION(0x55B938, "std::__vector_base<ChatStampListScrollView::ChatStamp>::~__vector_base", (stringsDtor<0x1C, 0x08>));
RW_FUNCTION(0x5B133C, "std::__vector_base<DrawScrollView::GachaData>::~__vector_base", (stringsDtor<0x40, 0x24, 0x0C, 0x00>));
RW_FUNCTION(0x5B965C, "std::__vector_base<RaidSubdueScrollView::SubdueData>::~__vector_base", (stringsDtor<0x50, 0x10>));
RW_FUNCTION(0x4AE8D8, "std::__vector_base<hole::network::api::UnionRanking::Ranker>::~__vector_base", (sharedsDtor<0x70, 0x08, 0x00>));
RW_FUNCTION(0x4B280C, "std::__vector_base<hole::network::api::ColosseumRanking::User>::~__vector_base", (sharedsDtor<0x30, 0x10, 0x08, 0x00>));
RW_FUNCTION(0x4B2F76, "std::__vector_base<hole::network::api::LuxRanking::User>::~__vector_base", (sharedsDtor<0x20, 0x10, 0x08, 0x00>));
RW_FUNCTION(0x4B6118, "std::__vector_base<hole::network::api::RaidReward151101::Reward>::~__vector_base", (sharedsDtor<0xB0, 0xA8, 0xA0, 0x98>));
RW_FUNCTION(0x4B2ABC, "std::__vector_base<hole::network::api::PartyRanking::Info>::~__vector_base", partyRankingInfoDtor);
RW_FUNCTION(0x4B3DD0, "std::__vector_base<hole::network::api::PartyApplicationList::PartyEx>::~__vector_base", partyExDtor);
RW_FUNCTION(0x4B63A4, "std::__vector_base<hole::network::api::RaidReward::Reward>::~__vector_base", raidRewardDtor);
RW_FUNCTION(0x4B58EE, "std::__vector_base<hole::network::api::Stage::Story>::~__vector_base", stageStoryVectorDtor);
RW_FUNCTION(0x4B58A0, "std::__vector_base<hole::network::api::StageEvent::Event>::~__vector_base", stageEventDtor<0x38>);
RW_FUNCTION(0x4B5E58, "std::__vector_base<hole::network::api::Stage::Event>::~__vector_base", stageEventDtor<0x30>);
RW_FUNCTION(0x5C5E54, "std::__vector_base<Avatar::InstanceInfo>::~__vector_base", avatarInstanceInfoDtor);

// ---------------------------------------------------------------------------------------------- more element types
RW_FUNCTION(0x496342, "std::__vector_base<cocos2d::Node*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x4964B6, "std::__vector_base<MapNonPlayableCharacter*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5DBD80, "std::__vector_base<MapPrizes*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5DEF9C, "std::__vector_base<MapPrize*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5FB6E0, "std::__vector_base<MapFieldLayer*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5FB73E, "std::__vector_base<MapMaterial*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5FB79C, "std::__vector_base<MapTreasure*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x5FB7FA, "std::__vector_base<MapGateMarker*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x70E4BC, "std::__vector_base<ImageButton*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x7C8A4C, "std::__vector_base<cocos2d::Component*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x8845CA, "std::__vector_base<cocos2d::SpriteBatchNode*>::~__vector_base", trivialDtor<4>);
RW_FUNCTION(0x7EF43E, "std::__vector_base<pair<int, function<void(LWF::Movie*, LWF::Button*)>>>::~__vector_base", pairFunctionDtor);
RW_FUNCTION(0x7EF4CE, "std::__vector_base<pair<int, function<void(LWF::Movie*)>>>::~__vector_base", pairFunctionDtor);
RW_FUNCTION(0x7EF51A, "std::__vector_base<pair<int, function<void(LWF::Button*, int)>>>::~__vector_base", pairFunctionDtor);
RW_FUNCTION(0x7EF566, "std::__vector_base<pair<int, function<void(LWF::Button*)>>>::~__vector_base", pairFunctionDtor);
RW_FUNCTION(0x884594, "std::__vector_base<cocos2d::Label::LetterInfo>::~__vector_base", letterInfoDtor);
