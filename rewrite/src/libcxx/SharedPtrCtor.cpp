// std::shared_ptr<T>::shared_ptr(T* p) for the game's types: a separate control block,
// __shared_ptr_pointer<T*, default_delete<T>, allocator<T>> (16 bytes: vtable, owners - 1, weak owners - 1, p).
// The instances differ only in that control block's vtable (its __on_zero_shared deletes the right T).
#include "libcxx/Libcxx.hpp"

namespace {

template <uint32_t ControlVtable> uint32_t sharedFromPointer(guest::SharedPtr *self, uint32_t p) {
    self->obj = p;
    uint32_t ctrl = guest::newObject(16);
    rw::save<uint32_t>(ctrl + 4, 0);   // shared owners - 1
    rw::save<uint32_t>(ctrl + 8, 0);   // weak owners - 1
    rw::save<uint32_t>(ctrl, ControlVtable);
    rw::save<uint32_t>(ctrl + 12, p);
    self->ctrl = ctrl;
    return rw::guest_addr(self);
}

}  // namespace

RW_FUNCTION(0x49DF78, "std::shared_ptr<hole::network::api::NativeToken>::shared_ptr(hole::network::api::NativeToken*)", sharedFromPointer<0xF7F360>);
RW_FUNCTION(0x49E1B8, "std::shared_ptr<hole::network::api::Login>::shared_ptr(hole::network::api::Login*)", sharedFromPointer<0xF7F338>);
RW_FUNCTION(0x49E3EC, "std::shared_ptr<hole::network::api::LoginBonus>::shared_ptr(hole::network::api::LoginBonus*)", sharedFromPointer<0xF7F310>);
RW_FUNCTION(0x49E854, "std::shared_ptr<hole::network::api::User>::shared_ptr(hole::network::api::User*)", sharedFromPointer<0xF7F2E8>);
RW_FUNCTION(0x49EDC8, "std::shared_ptr<hole::network::api::UserPoint>::shared_ptr(hole::network::api::UserPoint*)", sharedFromPointer<0xF7F2C0>);
RW_FUNCTION(0x49F45C, "std::shared_ptr<hole::network::api::UserDetail>::shared_ptr(hole::network::api::UserDetail*)", sharedFromPointer<0xF7F298>);
RW_FUNCTION(0x49F670, "std::shared_ptr<hole::network::api::StageResumption>::shared_ptr(hole::network::api::StageResumption*)", sharedFromPointer<0xF7F270>);
RW_FUNCTION(0x49F908, "std::shared_ptr<hole::network::api::UserAvatar>::shared_ptr(hole::network::api::UserAvatar*)", sharedFromPointer<0xF7F248>);
RW_FUNCTION(0x49FA98, "std::shared_ptr<hole::network::api::UserAvatarAll>::shared_ptr(hole::network::api::UserAvatarAll*)", sharedFromPointer<0xF7F220>);
RW_FUNCTION(0x49FD54, "std::shared_ptr<hole::network::api::UserAvatarParts>::shared_ptr(hole::network::api::UserAvatarParts*)", sharedFromPointer<0xF7F1F8>);
RW_FUNCTION(0x49FFA4, "std::shared_ptr<hole::network::api::UserStone>::shared_ptr(hole::network::api::UserStone*)", sharedFromPointer<0xF7F18C>);
RW_FUNCTION(0x4A03E4, "std::shared_ptr<hole::network::api::UserMedal>::shared_ptr(hole::network::api::UserMedal*)", sharedFromPointer<0xF7F164>);
RW_FUNCTION(0x4A08C8, "std::shared_ptr<hole::network::api::UserSkill>::shared_ptr(hole::network::api::UserSkill*)", sharedFromPointer<0xF7F0F8>);
RW_FUNCTION(0x4A0BA8, "std::shared_ptr<hole::network::api::UserMaterial>::shared_ptr(hole::network::api::UserMaterial*)", sharedFromPointer<0xF7F08C>);
RW_FUNCTION(0x4A1084, "std::shared_ptr<hole::network::api::UserKeyblade>::shared_ptr(hole::network::api::UserKeyblade*)", sharedFromPointer<0xF7F020>);
RW_FUNCTION(0x4A1378, "std::shared_ptr<hole::network::api::UserAlbum>::shared_ptr(hole::network::api::UserAlbum*)", sharedFromPointer<0xF7EFB4>);
RW_FUNCTION(0x4A1534, "std::shared_ptr<hole::network::api::UserAlbumChallenge>::shared_ptr(hole::network::api::UserAlbumChallenge*)", sharedFromPointer<0xF7EF8C>);
RW_FUNCTION(0x4A17C0, "std::shared_ptr<hole::network::api::UserTitle>::shared_ptr(hole::network::api::UserTitle*)", sharedFromPointer<0xF7EF64>);
RW_FUNCTION(0x4A1CA4, "std::shared_ptr<hole::network::api::UserSupport>::shared_ptr(hole::network::api::UserSupport*)", sharedFromPointer<0xF7EEF8>);
RW_FUNCTION(0x4A1F04, "std::shared_ptr<hole::network::api::UserMedalEnhance>::shared_ptr(hole::network::api::UserMedalEnhance*)", sharedFromPointer<0xF7EED0>);
RW_FUNCTION(0x4A1FFC, "std::shared_ptr<hole::network::api::UserMedalEvolve>::shared_ptr(hole::network::api::UserMedalEvolve*)", sharedFromPointer<0xF7EEA8>);
RW_FUNCTION(0x4A2608, "std::shared_ptr<hole::network::api::Raid>::shared_ptr(hole::network::api::Raid*)", sharedFromPointer<0xF7EE80>);
RW_FUNCTION(0x4A28F4, "std::shared_ptr<hole::network::api::RaidMember>::shared_ptr(hole::network::api::RaidMember*)", sharedFromPointer<0xF7EE58>);
RW_FUNCTION(0x4A2F1C, "std::shared_ptr<hole::network::api::RaidReward>::shared_ptr(hole::network::api::RaidReward*)", sharedFromPointer<0xF7EE30>);
RW_FUNCTION(0x4A345C, "std::shared_ptr<hole::network::api::RaidReward151101>::shared_ptr(hole::network::api::RaidReward151101*)", sharedFromPointer<0xF7EE08>);
RW_FUNCTION(0x4A3CA0, "std::shared_ptr<hole::network::api::Stage>::shared_ptr(hole::network::api::Stage*)", sharedFromPointer<0xF7EDE0>);
RW_FUNCTION(0x4A3EA0, "std::shared_ptr<hole::network::api::Stage160310>::shared_ptr(hole::network::api::Stage160310*)", sharedFromPointer<0xF7EDB8>);
RW_FUNCTION(0x4A4174, "std::shared_ptr<hole::network::api::StageEvent>::shared_ptr(hole::network::api::StageEvent*)", sharedFromPointer<0xF7ED90>);
RW_FUNCTION(0x4A46E8, "std::shared_ptr<hole::network::api::StageSupportList>::shared_ptr(hole::network::api::StageSupportList*)", sharedFromPointer<0xF7ED68>);
RW_FUNCTION(0x4A4D34, "std::shared_ptr<hole::network::api::StageStart>::shared_ptr(hole::network::api::StageStart*)", sharedFromPointer<0xF7ED40>);
RW_FUNCTION(0x4A5160, "std::shared_ptr<hole::network::api::StageClear>::shared_ptr(hole::network::api::StageClear*)", sharedFromPointer<0xF7ED18>);
RW_FUNCTION(0x4A5854, "std::shared_ptr<hole::network::api::RaidStart>::shared_ptr(hole::network::api::RaidStart*)", sharedFromPointer<0xF7ECF0>);
RW_FUNCTION(0x4A5A94, "std::shared_ptr<hole::network::api::RaidClear>::shared_ptr(hole::network::api::RaidClear*)", sharedFromPointer<0xF7ECC8>);
RW_FUNCTION(0x4A5ED0, "std::shared_ptr<hole::network::api::UserSphere>::shared_ptr(hole::network::api::UserSphere*)", sharedFromPointer<0xF7ECA0>);
RW_FUNCTION(0x4A6440, "std::shared_ptr<hole::network::api::UserPresent>::shared_ptr(hole::network::api::UserPresent*)", sharedFromPointer<0xF7EC78>);
RW_FUNCTION(0x4A6604, "std::shared_ptr<hole::network::api::UserBirthday>::shared_ptr(hole::network::api::UserBirthday*)", sharedFromPointer<0xF7EC50>);
RW_FUNCTION(0x4A69A4, "std::shared_ptr<hole::network::api::SystemNeedUrl>::shared_ptr(hole::network::api::SystemNeedUrl*)", sharedFromPointer<0xF7EC28>);
RW_FUNCTION(0x4A6C78, "std::shared_ptr<hole::network::api::SystemCoppa>::shared_ptr(hole::network::api::SystemCoppa*)", sharedFromPointer<0xF7EC00>);
RW_FUNCTION(0x4A6D5C, "std::shared_ptr<hole::network::api::SystemInformationList>::shared_ptr(hole::network::api::SystemInformationList*)", sharedFromPointer<0xF7EBD8>);
RW_FUNCTION(0x4A6EF4, "std::shared_ptr<hole::network::api::SystemInformationList151203>::shared_ptr(hole::network::api::SystemInformationList151203*)", sharedFromPointer<0xF7EBB0>);
RW_FUNCTION(0x4A701C, "std::shared_ptr<hole::network::api::SystemInvitecode>::shared_ptr(hole::network::api::SystemInvitecode*)", sharedFromPointer<0xF7EB88>);
RW_FUNCTION(0x4A71E8, "std::shared_ptr<hole::network::api::PaymentInfo>::shared_ptr(hole::network::api::PaymentInfo*)", sharedFromPointer<0xF7EB60>);
RW_FUNCTION(0x4A78C4, "std::shared_ptr<hole::network::api::UserParty>::shared_ptr(hole::network::api::UserParty*)", sharedFromPointer<0xF7EB38>);
RW_FUNCTION(0x4A7984, "std::shared_ptr<hole::network::api::PartyCreate>::shared_ptr(hole::network::api::PartyCreate*)", sharedFromPointer<0xF7EB10>);
RW_FUNCTION(0x4A7D7C, "std::shared_ptr<hole::network::api::PartyNotice>::shared_ptr(hole::network::api::PartyNotice*)", sharedFromPointer<0xF7EAE8>);
RW_FUNCTION(0x4A7E78, "std::shared_ptr<hole::network::api::PartyLogin>::shared_ptr(hole::network::api::PartyLogin*)", sharedFromPointer<0xF7EAC0>);
RW_FUNCTION(0x4A8368, "std::shared_ptr<hole::network::api::PartyMemberList>::shared_ptr(hole::network::api::PartyMemberList*)", sharedFromPointer<0xF7EA98>);
RW_FUNCTION(0x4A8624, "std::shared_ptr<hole::network::api::PartyApplication>::shared_ptr(hole::network::api::PartyApplication*)", sharedFromPointer<0xF7EA70>);
RW_FUNCTION(0x4A8A8C, "std::shared_ptr<hole::network::api::PartyApplicationList>::shared_ptr(hole::network::api::PartyApplicationList*)", sharedFromPointer<0xF7EA48>);
RW_FUNCTION(0x4A8D20, "std::shared_ptr<hole::network::api::PartyApplicantList>::shared_ptr(hole::network::api::PartyApplicantList*)", sharedFromPointer<0xF7EA20>);
RW_FUNCTION(0x4A8DE0, "std::shared_ptr<hole::network::api::PartyInviteOffer>::shared_ptr(hole::network::api::PartyInviteOffer*)", sharedFromPointer<0xF7E9F8>);
RW_FUNCTION(0x4A91C4, "std::shared_ptr<hole::network::api::PartyInviteList>::shared_ptr(hole::network::api::PartyInviteList*)", sharedFromPointer<0xF7E9D0>);
RW_FUNCTION(0x4A95FC, "std::shared_ptr<hole::network::api::PartyOfferList>::shared_ptr(hole::network::api::PartyOfferList*)", sharedFromPointer<0xF7E9A8>);
RW_FUNCTION(0x4A978C, "std::shared_ptr<hole::network::api::PurchasePrepare>::shared_ptr(hole::network::api::PurchasePrepare*)", sharedFromPointer<0xF7E980>);
RW_FUNCTION(0x4A9948, "std::shared_ptr<hole::network::api::UserShop>::shared_ptr(hole::network::api::UserShop*)", sharedFromPointer<0xF7E958>);
RW_FUNCTION(0x4A9CFC, "std::shared_ptr<hole::network::api::Colosseum>::shared_ptr(hole::network::api::Colosseum*)", sharedFromPointer<0xF7E930>);
RW_FUNCTION(0x4A9FC8, "std::shared_ptr<hole::network::api::ColosseumStart>::shared_ptr(hole::network::api::ColosseumStart*)", sharedFromPointer<0xF7E908>);
RW_FUNCTION(0x4AA31C, "std::shared_ptr<hole::network::api::ColosseumClear>::shared_ptr(hole::network::api::ColosseumClear*)", sharedFromPointer<0xF7E8E0>);
RW_FUNCTION(0x4AA758, "std::shared_ptr<hole::network::api::GachaList>::shared_ptr(hole::network::api::GachaList*)", sharedFromPointer<0xF7E8B8>);
RW_FUNCTION(0x4AAA54, "std::shared_ptr<hole::network::api::GachaDraw>::shared_ptr(hole::network::api::GachaDraw*)", sharedFromPointer<0xF7E890>);
RW_FUNCTION(0x4AAC94, "std::shared_ptr<hole::network::api::TutorialProgress>::shared_ptr(hole::network::api::TutorialProgress*)", sharedFromPointer<0xF7E868>);
RW_FUNCTION(0x4AADB4, "std::shared_ptr<hole::network::api::TutorialStatus>::shared_ptr(hole::network::api::TutorialStatus*)", sharedFromPointer<0xF7E840>);
RW_FUNCTION(0x4AAFBC, "std::shared_ptr<hole::network::api::TutorialClear>::shared_ptr(hole::network::api::TutorialClear*)", sharedFromPointer<0xF7E818>);
RW_FUNCTION(0x4ABE38, "std::shared_ptr<hole::network::api::UserProfile>::shared_ptr(hole::network::api::UserProfile*)", sharedFromPointer<0xF7E7F0>);
RW_FUNCTION(0x4AC28C, "std::shared_ptr<hole::network::api::LuxRanking>::shared_ptr(hole::network::api::LuxRanking*)", sharedFromPointer<0xF7E7C8>);
RW_FUNCTION(0x4AC66C, "std::shared_ptr<hole::network::api::PartyRanking>::shared_ptr(hole::network::api::PartyRanking*)", sharedFromPointer<0xF7E7A0>);
RW_FUNCTION(0x4ACAF0, "std::shared_ptr<hole::network::api::ColosseumRanking>::shared_ptr(hole::network::api::ColosseumRanking*)", sharedFromPointer<0xF7E778>);
RW_FUNCTION(0x4AD388, "std::shared_ptr<hole::network::api::UserStamp>::shared_ptr(hole::network::api::UserStamp*)", sharedFromPointer<0xF7E700>);
RW_FUNCTION(0x4AD634, "std::shared_ptr<hole::network::api::UserChat>::shared_ptr(hole::network::api::UserChat*)", sharedFromPointer<0xF7E6D8>);
RW_FUNCTION(0x4ADC08, "std::shared_ptr<hole::network::api::MypageInfo>::shared_ptr(hole::network::api::MypageInfo*)", sharedFromPointer<0xF7E6B0>);
RW_FUNCTION(0x4ADD50, "std::shared_ptr<hole::network::api::UserOption>::shared_ptr(hole::network::api::UserOption*)", sharedFromPointer<0xF7E688>);
RW_FUNCTION(0x4B909C, "std::shared_ptr<rapidjson::GenericDocument<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>>::shared_ptr(rapidjson::GenericDocument<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>*)", sharedFromPointer<0xF7F624>);
RW_FUNCTION(0x777A00, "std::shared_ptr<master::World const>::shared_ptr(master::World const*)", sharedFromPointer<0xFAA960>);
RW_FUNCTION(0x777FE0, "std::shared_ptr<master::TutorialMisc const>::shared_ptr(master::TutorialMisc const*)", sharedFromPointer<0xFAA9FC>);
RW_FUNCTION(0x778760, "std::shared_ptr<master::AlbumChallenge const>::shared_ptr(master::AlbumChallenge const*)", sharedFromPointer<0xFAAA8C>);
RW_FUNCTION(0x778C2C, "std::shared_ptr<master::AvatarCombination const>::shared_ptr(master::AvatarCombination const*)", sharedFromPointer<0xFAAB1C>);
RW_FUNCTION(0x779370, "std::shared_ptr<master::AvatarParts const>::shared_ptr(master::AvatarParts const*)", sharedFromPointer<0xFAABAC>);
RW_FUNCTION(0x77991C, "std::shared_ptr<master::Badstatus const>::shared_ptr(master::Badstatus const*)", sharedFromPointer<0xFAAC3C>);
RW_FUNCTION(0x779D68, "std::shared_ptr<master::BattleMisc const>::shared_ptr(master::BattleMisc const*)", sharedFromPointer<0xFAACCC>);
RW_FUNCTION(0x77A2DC, "std::shared_ptr<master::Buff const>::shared_ptr(master::Buff const*)", sharedFromPointer<0xFAAD5C>);
RW_FUNCTION(0x77BEC0, "std::shared_ptr<master::Burst const>::shared_ptr(master::Burst const*)", sharedFromPointer<0xFAADEC>);
RW_FUNCTION(0x77C334, "std::shared_ptr<master::Chapter const>::shared_ptr(master::Chapter const*)", sharedFromPointer<0xFAAE7C>);
RW_FUNCTION(0x77CDE0, "std::shared_ptr<master::Colosseum const>::shared_ptr(master::Colosseum const*)", sharedFromPointer<0xFAAF0C>);
RW_FUNCTION(0x77D54C, "std::shared_ptr<master::ColosseumStage const>::shared_ptr(master::ColosseumStage const*)", sharedFromPointer<0xFAAF9C>);
RW_FUNCTION(0x77DB2C, "std::shared_ptr<master::DrawMedalList const>::shared_ptr(master::DrawMedalList const*)", sharedFromPointer<0xFAB02C>);
RW_FUNCTION(0x77EB28, "std::shared_ptr<master::DrawMedalType const>::shared_ptr(master::DrawMedalType const*)", sharedFromPointer<0xFAB0BC>);
RW_FUNCTION(0x77F048, "std::shared_ptr<master::DrawSkillList const>::shared_ptr(master::DrawSkillList const*)", sharedFromPointer<0xFAB14C>);
RW_FUNCTION(0x77F628, "std::shared_ptr<master::DrawSkillType const>::shared_ptr(master::DrawSkillType const*)", sharedFromPointer<0xFAB1DC>);
RW_FUNCTION(0x780D9C, "std::shared_ptr<master::Enemy const>::shared_ptr(master::Enemy const*)", sharedFromPointer<0xFAB26C>);
RW_FUNCTION(0x781C24, "std::shared_ptr<master::EnemyAttack const>::shared_ptr(master::EnemyAttack const*)", sharedFromPointer<0xFAB2FC>);
RW_FUNCTION(0x782588, "std::shared_ptr<master::EvCampaign const>::shared_ptr(master::EvCampaign const*)", sharedFromPointer<0xFAB38C>);
RW_FUNCTION(0x782B78, "std::shared_ptr<master::EvGroupPattern const>::shared_ptr(master::EvGroupPattern const*)", sharedFromPointer<0xFAB41C>);
RW_FUNCTION(0x78384C, "std::shared_ptr<master::EvStage const>::shared_ptr(master::EvStage const*)", sharedFromPointer<0xFAB4AC>);
RW_FUNCTION(0x783DE8, "std::shared_ptr<master::InitItem const>::shared_ptr(master::InitItem const*)", sharedFromPointer<0xFAB53C>);
RW_FUNCTION(0x7848C4, "std::shared_ptr<master::Keyblade const>::shared_ptr(master::Keyblade const*)", sharedFromPointer<0xFAB5CC>);
RW_FUNCTION(0x785130, "std::shared_ptr<master::LoginBonus const>::shared_ptr(master::LoginBonus const*)", sharedFromPointer<0xFAB65C>);
RW_FUNCTION(0x7856BC, "std::shared_ptr<master::Material const>::shared_ptr(master::Material const*)", sharedFromPointer<0xFAB6EC>);
RW_FUNCTION(0x786714, "std::shared_ptr<master::Medal const>::shared_ptr(master::Medal const*)", sharedFromPointer<0xFAB77C>);
RW_FUNCTION(0x786BA8, "std::shared_ptr<master::MedalMisc const>::shared_ptr(master::MedalMisc const*)", sharedFromPointer<0xFAB80C>);
RW_FUNCTION(0x786FF4, "std::shared_ptr<master::Misc const>::shared_ptr(master::Misc const*)", sharedFromPointer<0xFAB89C>);
RW_FUNCTION(0x787590, "std::shared_ptr<master::Player const>::shared_ptr(master::Player const*)", sharedFromPointer<0xFAB92C>);
RW_FUNCTION(0x788E84, "std::shared_ptr<master::RaidEnemy const>::shared_ptr(master::RaidEnemy const*)", sharedFromPointer<0xFAB9BC>);
RW_FUNCTION(0x789C44, "std::shared_ptr<master::RaidEnemyAttack const>::shared_ptr(master::RaidEnemyAttack const*)", sharedFromPointer<0xFABA4C>);
RW_FUNCTION(0x78A478, "std::shared_ptr<master::RaidReward const>::shared_ptr(master::RaidReward const*)", sharedFromPointer<0xFABADC>);
RW_FUNCTION(0x78AC04, "std::shared_ptr<master::RaidSetting const>::shared_ptr(master::RaidSetting const*)", sharedFromPointer<0xFABB6C>);
RW_FUNCTION(0x78B40C, "std::shared_ptr<master::RankingReward const>::shared_ptr(master::RankingReward const*)", sharedFromPointer<0xFABBFC>);
RW_FUNCTION(0x78BC34, "std::shared_ptr<master::Reward const>::shared_ptr(master::Reward const*)", sharedFromPointer<0xFABC8C>);
RW_FUNCTION(0x78C8D8, "std::shared_ptr<master::Skill const>::shared_ptr(master::Skill const*)", sharedFromPointer<0xFABD1C>);
RW_FUNCTION(0x78CD6C, "std::shared_ptr<master::SkillExp const>::shared_ptr(master::SkillExp const*)", sharedFromPointer<0xFABDAC>);
RW_FUNCTION(0x78D4C0, "std::shared_ptr<master::Sphere const>::shared_ptr(master::Sphere const*)", sharedFromPointer<0xFABE3C>);
RW_FUNCTION(0x78DB54, "std::shared_ptr<master::SphereArray const>::shared_ptr(master::SphereArray const*)", sharedFromPointer<0xFABECC>);
RW_FUNCTION(0x78E334, "std::shared_ptr<master::SphereMasu const>::shared_ptr(master::SphereMasu const*)", sharedFromPointer<0xFABF5C>);
RW_FUNCTION(0x78F8D8, "std::shared_ptr<master::Stage const>::shared_ptr(master::Stage const*)", sharedFromPointer<0xFABFEC>);
RW_FUNCTION(0x78FEB8, "std::shared_ptr<master::Title const>::shared_ptr(master::Title const*)", sharedFromPointer<0xFAC07C>);
RW_FUNCTION(0x79079C, "std::shared_ptr<master::SerialcodeReward const>::shared_ptr(master::SerialcodeReward const*)", sharedFromPointer<0xFAC10C>);
RW_FUNCTION(0x791118, "std::shared_ptr<master::Shop const>::shared_ptr(master::Shop const*)", sharedFromPointer<0xFAC19C>);
RW_FUNCTION(0x791830, "std::shared_ptr<master::Ranking const>::shared_ptr(master::Ranking const*)", sharedFromPointer<0xFAC22C>);
RW_FUNCTION(0x791E10, "std::shared_ptr<master::Stamp const>::shared_ptr(master::Stamp const*)", sharedFromPointer<0xFAC2BC>);
RW_FUNCTION(0x79232C, "std::shared_ptr<master::GuiltProb const>::shared_ptr(master::GuiltProb const*)", sharedFromPointer<0xFAC34C>);
RW_FUNCTION(0x792BE0, "std::shared_ptr<master::MypageBackground const>::shared_ptr(master::MypageBackground const*)", sharedFromPointer<0xFAC3DC>);
RW_FUNCTION(0x7930F0, "std::shared_ptr<master::EvResource const>::shared_ptr(master::EvResource const*)", sharedFromPointer<0xFAC46C>);
