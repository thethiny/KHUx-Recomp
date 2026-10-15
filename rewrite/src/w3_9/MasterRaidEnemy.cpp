// master::RaidEnemy::create(int)'s make_shared control block and row destructors (see MasterEmplaceBlock.hpp).
// sub_788E28 / sub_788E30 are not here: they are the tail of master::RaidEnemy::parse (they run in parse's frame:
// the shared_ptr construction and stack-guard epilogue at sp + 0x90..0x98), not standalone functions.
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/RaidEnemy.hpp"

namespace master {

using RaidEnemyBlock = EmplaceBlockFunctions<RaidEnemy>;
RW_FUNCTION(0x788F38, "master::RaidEnemy::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", RaidEnemyBlock::destruct);
RW_FUNCTION(0x788F6C, "master::RaidEnemy::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", RaidEnemyBlock::destructDelete);
RW_FUNCTION(0x788FA4, "master::RaidEnemy::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", RaidEnemyBlock::onZeroShared);
RW_FUNCTION(0x788FB0, "master::RaidEnemy::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", RaidEnemyBlock::onZeroSharedWeak);
RW_FUNCTION(0x788FB8, "master::RaidEnemy::create(int)::impl::~impl (deleting)", RaidEnemyBlock::rowDestructDelete);

}  // namespace master
