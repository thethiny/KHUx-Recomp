// master::Colosseum::create(int)'s make_shared control block and row destructors (see MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/Colosseum.hpp"

namespace master {

using ColosseumBlock = EmplaceBlockFunctions<Colosseum>;
RW_FUNCTION(0x77CE94, "master::Colosseum::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", ColosseumBlock::destruct);
RW_FUNCTION(0x77CEC8, "master::Colosseum::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", ColosseumBlock::destructDelete);
RW_FUNCTION(0x77CF00, "master::Colosseum::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", ColosseumBlock::onZeroShared);
RW_FUNCTION(0x77CF0C, "master::Colosseum::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", ColosseumBlock::onZeroSharedWeak);
RW_FUNCTION(0x77CF14, "master::Colosseum::create(int)::impl::~impl (deleting)", ColosseumBlock::rowDestructDelete);

}  // namespace master
