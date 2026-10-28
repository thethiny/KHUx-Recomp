// make_shared control block destructors of ten master tables' create(int) (the complete and deleting
// ~__shared_ptr_emplace, see w3_9/MasterEmplaceBlock.hpp; their other block functions are registered elsewhere).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/Badstatus.hpp"
#include "master/BattleMisc.hpp"
#include "master/Enemy.hpp"
#include "master/EnemyAttack.hpp"
#include "master/EvGroupPattern.hpp"
#include "master/GuiltProb.hpp"
#include "master/InitItem.hpp"
#include "master/RankingReward.hpp"
#include "master/Reward.hpp"
#include "master/Stamp.hpp"

namespace master {

using BadstatusBlockDtors = EmplaceBlockFunctions<Badstatus>;
RW_FUNCTION(0x7799D0, "master::Badstatus::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", BadstatusBlockDtors::destruct);
RW_FUNCTION(0x779A04, "master::Badstatus::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", BadstatusBlockDtors::destructDelete);

using BattleMiscBlockDtors = EmplaceBlockFunctions<BattleMisc>;
RW_FUNCTION(0x779E1C, "master::BattleMisc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", BattleMiscBlockDtors::destruct);
RW_FUNCTION(0x779E50, "master::BattleMisc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", BattleMiscBlockDtors::destructDelete);

using EnemyBlockDtors = EmplaceBlockFunctions<Enemy>;
RW_FUNCTION(0x780E50, "master::Enemy::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", EnemyBlockDtors::destruct);
RW_FUNCTION(0x780E84, "master::Enemy::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", EnemyBlockDtors::destructDelete);

using EnemyAttackBlockDtors = EmplaceBlockFunctions<EnemyAttack>;
RW_FUNCTION(0x781CD8, "master::EnemyAttack::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", EnemyAttackBlockDtors::destruct);
RW_FUNCTION(0x781D0C, "master::EnemyAttack::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", EnemyAttackBlockDtors::destructDelete);

using EvGroupPatternBlockDtors = EmplaceBlockFunctions<EvGroupPattern>;
RW_FUNCTION(0x782C2C, "master::EvGroupPattern::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", EvGroupPatternBlockDtors::destruct);
RW_FUNCTION(0x782C60, "master::EvGroupPattern::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", EvGroupPatternBlockDtors::destructDelete);

using InitItemBlockDtors = EmplaceBlockFunctions<InitItem>;
RW_FUNCTION(0x783E9C, "master::InitItem::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", InitItemBlockDtors::destruct);
RW_FUNCTION(0x783ED0, "master::InitItem::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", InitItemBlockDtors::destructDelete);

using RankingRewardBlockDtors = EmplaceBlockFunctions<RankingReward>;
RW_FUNCTION(0x78B4C0, "master::RankingReward::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", RankingRewardBlockDtors::destruct);
RW_FUNCTION(0x78B4F4, "master::RankingReward::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", RankingRewardBlockDtors::destructDelete);

using RewardBlockDtors = EmplaceBlockFunctions<Reward>;
RW_FUNCTION(0x78BCE8, "master::Reward::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", RewardBlockDtors::destruct);
RW_FUNCTION(0x78BD1C, "master::Reward::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", RewardBlockDtors::destructDelete);

using StampBlockDtors = EmplaceBlockFunctions<Stamp>;
RW_FUNCTION(0x791EC4, "master::Stamp::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", StampBlockDtors::destruct);
RW_FUNCTION(0x791EF8, "master::Stamp::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", StampBlockDtors::destructDelete);

using GuiltProbBlockDtors = EmplaceBlockFunctions<GuiltProb>;
RW_FUNCTION(0x7923E0, "master::GuiltProb::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", GuiltProbBlockDtors::destruct);
RW_FUNCTION(0x792414, "master::GuiltProb::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", GuiltProbBlockDtors::destructDelete);

}  // namespace master
