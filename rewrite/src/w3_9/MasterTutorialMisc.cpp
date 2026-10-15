// master::TutorialMisc::create(int)'s make_shared control block and row destructors (see MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/TutorialMisc.hpp"

namespace master {

using TutorialMiscBlock = EmplaceBlockFunctions<TutorialMisc>;
RW_FUNCTION(0x778094, "master::TutorialMisc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", TutorialMiscBlock::destruct);
RW_FUNCTION(0x7780C8, "master::TutorialMisc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", TutorialMiscBlock::destructDelete);
RW_FUNCTION(0x778100, "master::TutorialMisc::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", TutorialMiscBlock::onZeroShared);
RW_FUNCTION(0x77810C, "master::TutorialMisc::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", TutorialMiscBlock::onZeroSharedWeak);
RW_FUNCTION(0x778114, "master::TutorialMisc::create(int)::impl::~impl (deleting)", TutorialMiscBlock::rowDestructDelete);

}  // namespace master
