// master::Material::create(int)'s make_shared control block and row destructors (see MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/Material.hpp"

namespace master {

using MaterialBlock = EmplaceBlockFunctions<Material>;
RW_FUNCTION(0x785770, "master::Material::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", MaterialBlock::destruct);
RW_FUNCTION(0x7857A4, "master::Material::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", MaterialBlock::destructDelete);
RW_FUNCTION(0x7857DC, "master::Material::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", MaterialBlock::onZeroShared);
RW_FUNCTION(0x7857E8, "master::Material::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", MaterialBlock::onZeroSharedWeak);
RW_FUNCTION(0x7857F0, "master::Material::create(int)::impl::~impl (deleting)", MaterialBlock::rowDestructDelete);

}  // namespace master
