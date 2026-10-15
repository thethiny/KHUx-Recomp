// master::SphereArray::create(int)'s make_shared control block and row destructors (see MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/SphereArray.hpp"

namespace master {

using SphereArrayBlock = EmplaceBlockFunctions<SphereArray>;
RW_FUNCTION(0x78DC08, "master::SphereArray::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", SphereArrayBlock::destruct);
RW_FUNCTION(0x78DC3C, "master::SphereArray::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", SphereArrayBlock::destructDelete);
RW_FUNCTION(0x78DC74, "master::SphereArray::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", SphereArrayBlock::onZeroShared);
RW_FUNCTION(0x78DC80, "master::SphereArray::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", SphereArrayBlock::onZeroSharedWeak);
RW_FUNCTION(0x78DC88, "master::SphereArray::create(int)::impl::~impl (deleting)", SphereArrayBlock::rowDestructDelete);

}  // namespace master
