// master::Ranking::create(int)'s make_shared control block and row destructors (see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/Ranking.hpp"

namespace master {

using RankingBlock = EmplaceBlockFunctions<Ranking>;
RW_FUNCTION(0x7918E4, "master::Ranking::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", RankingBlock::destruct);
RW_FUNCTION(0x791918, "master::Ranking::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", RankingBlock::destructDelete);
RW_FUNCTION(0x791950, "master::Ranking::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", RankingBlock::onZeroShared);
RW_FUNCTION(0x79195C, "master::Ranking::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", RankingBlock::onZeroSharedWeak);
RW_FUNCTION(0x791964, "master::Ranking::create(int)::impl::~impl (deleting)", RankingBlock::rowDestructDelete);

}  // namespace master
