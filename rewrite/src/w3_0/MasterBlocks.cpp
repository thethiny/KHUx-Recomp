// make_shared control block and row destructors of master::AlbumChallenge, ColosseumStage, MedalMisc, SphereMasu and
// RaidEnemyAttack create(int) (the five compiler-generated functions, see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/AlbumChallenge.hpp"
#include "master/ColosseumStage.hpp"
#include "master/MedalMisc.hpp"
#include "master/RaidEnemyAttack.hpp"
#include "master/SphereMasu.hpp"

namespace master {

using AlbumChallengeBlock = EmplaceBlockFunctions<AlbumChallenge>;
RW_FUNCTION(0x778814, "master::AlbumChallenge::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", AlbumChallengeBlock::destruct);
RW_FUNCTION(0x778848, "master::AlbumChallenge::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", AlbumChallengeBlock::destructDelete);
RW_FUNCTION(0x778880, "master::AlbumChallenge::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", AlbumChallengeBlock::onZeroShared);
RW_FUNCTION(0x77888C, "master::AlbumChallenge::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", AlbumChallengeBlock::onZeroSharedWeak);
RW_FUNCTION(0x778894, "master::AlbumChallenge::create(int)::impl::~impl (deleting)", AlbumChallengeBlock::rowDestructDelete);

using ColosseumStageBlock = EmplaceBlockFunctions<ColosseumStage>;
RW_FUNCTION(0x77D600, "master::ColosseumStage::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", ColosseumStageBlock::destruct);
RW_FUNCTION(0x77D634, "master::ColosseumStage::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", ColosseumStageBlock::destructDelete);
RW_FUNCTION(0x77D66C, "master::ColosseumStage::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", ColosseumStageBlock::onZeroShared);
RW_FUNCTION(0x77D678, "master::ColosseumStage::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", ColosseumStageBlock::onZeroSharedWeak);
RW_FUNCTION(0x77D680, "master::ColosseumStage::create(int)::impl::~impl (deleting)", ColosseumStageBlock::rowDestructDelete);

using MedalMiscBlock = EmplaceBlockFunctions<MedalMisc>;
RW_FUNCTION(0x786C5C, "master::MedalMisc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", MedalMiscBlock::destruct);
RW_FUNCTION(0x786C90, "master::MedalMisc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", MedalMiscBlock::destructDelete);
RW_FUNCTION(0x786CC8, "master::MedalMisc::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", MedalMiscBlock::onZeroShared);
RW_FUNCTION(0x786CD4, "master::MedalMisc::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", MedalMiscBlock::onZeroSharedWeak);
RW_FUNCTION(0x786CDC, "master::MedalMisc::create(int)::impl::~impl (deleting)", MedalMiscBlock::rowDestructDelete);

using SphereMasuBlock = EmplaceBlockFunctions<SphereMasu>;
RW_FUNCTION(0x78E3E8, "master::SphereMasu::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", SphereMasuBlock::destruct);
RW_FUNCTION(0x78E41C, "master::SphereMasu::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", SphereMasuBlock::destructDelete);
RW_FUNCTION(0x78E454, "master::SphereMasu::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", SphereMasuBlock::onZeroShared);
RW_FUNCTION(0x78E460, "master::SphereMasu::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", SphereMasuBlock::onZeroSharedWeak);
RW_FUNCTION(0x78E468, "master::SphereMasu::create(int)::impl::~impl (deleting)", SphereMasuBlock::rowDestructDelete);

using RaidEnemyAttackBlock = EmplaceBlockFunctions<RaidEnemyAttack>;
RW_FUNCTION(0x789CF8, "master::RaidEnemyAttack::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", RaidEnemyAttackBlock::destruct);
RW_FUNCTION(0x789D2C, "master::RaidEnemyAttack::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", RaidEnemyAttackBlock::destructDelete);
RW_FUNCTION(0x789D64, "master::RaidEnemyAttack::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", RaidEnemyAttackBlock::onZeroShared);
RW_FUNCTION(0x789D70, "master::RaidEnemyAttack::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", RaidEnemyAttackBlock::onZeroSharedWeak);
RW_FUNCTION(0x789D78, "master::RaidEnemyAttack::create(int)::impl::~impl (deleting)", RaidEnemyAttackBlock::rowDestructDelete);

}  // namespace master
