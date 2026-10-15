// master::EvCampaign::create(int)'s make_shared control block and row destructors (see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/EvCampaign.hpp"

namespace master {

using EvCampaignBlock = EmplaceBlockFunctions<EvCampaign>;
RW_FUNCTION(0x78263C, "master::EvCampaign::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", EvCampaignBlock::destruct);
RW_FUNCTION(0x782670, "master::EvCampaign::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", EvCampaignBlock::destructDelete);
RW_FUNCTION(0x7826A8, "master::EvCampaign::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", EvCampaignBlock::onZeroShared);
RW_FUNCTION(0x7826B4, "master::EvCampaign::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", EvCampaignBlock::onZeroSharedWeak);
RW_FUNCTION(0x7826BC, "master::EvCampaign::create(int)::impl::~impl (deleting)", EvCampaignBlock::rowDestructDelete);

}  // namespace master
