// master::RaidSetting::create(int)'s make_shared control block and row destructors (see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/RaidSetting.hpp"

namespace master {

using RaidSettingBlock = EmplaceBlockFunctions<RaidSetting>;
RW_FUNCTION(0x78ACB8, "master::RaidSetting::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", RaidSettingBlock::destruct);
RW_FUNCTION(0x78ACEC, "master::RaidSetting::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", RaidSettingBlock::destructDelete);
RW_FUNCTION(0x78AD24, "master::RaidSetting::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", RaidSettingBlock::onZeroShared);
RW_FUNCTION(0x78AD30, "master::RaidSetting::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", RaidSettingBlock::onZeroSharedWeak);
RW_FUNCTION(0x78AD38, "master::RaidSetting::create(int)::impl::~impl (deleting)", RaidSettingBlock::rowDestructDelete);

}  // namespace master
