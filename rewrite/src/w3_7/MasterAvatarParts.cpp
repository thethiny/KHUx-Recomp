// master::AvatarParts::create(int)'s make_shared control block and row destructors (see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/AvatarParts.hpp"

namespace master {

using AvatarPartsBlock = EmplaceBlockFunctions<AvatarParts>;
RW_FUNCTION(0x779424, "master::AvatarParts::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", AvatarPartsBlock::destruct);
RW_FUNCTION(0x779458, "master::AvatarParts::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", AvatarPartsBlock::destructDelete);
RW_FUNCTION(0x779490, "master::AvatarParts::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", AvatarPartsBlock::onZeroShared);
RW_FUNCTION(0x77949C, "master::AvatarParts::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", AvatarPartsBlock::onZeroSharedWeak);
RW_FUNCTION(0x7794A4, "master::AvatarParts::create(int)::impl::~impl (deleting)", AvatarPartsBlock::rowDestructDelete);

}  // namespace master
