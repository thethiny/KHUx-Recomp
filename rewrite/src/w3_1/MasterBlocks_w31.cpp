// make_shared control block and row destructors of master::Burst, DrawMedalList, Misc and Title create(int) (the
// five compiler-generated functions, see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/Burst.hpp"
#include "master/DrawMedalList.hpp"
#include "master/Misc.hpp"
#include "master/Title.hpp"

namespace master {

using BurstBlock_w31 = EmplaceBlockFunctions<Burst>;
RW_FUNCTION(0x77BF74, "master::Burst::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", BurstBlock_w31::destruct);
RW_FUNCTION(0x77BFA8, "master::Burst::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", BurstBlock_w31::destructDelete);
RW_FUNCTION(0x77BFE0, "master::Burst::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", BurstBlock_w31::onZeroShared);
RW_FUNCTION(0x77BFEC, "master::Burst::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", BurstBlock_w31::onZeroSharedWeak);
RW_FUNCTION(0x77BFF4, "master::Burst::create(int)::impl::~impl (deleting)", BurstBlock_w31::rowDestructDelete);

using DrawMedalListBlock_w31 = EmplaceBlockFunctions<DrawMedalList>;
RW_FUNCTION(0x77DBE0, "master::DrawMedalList::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", DrawMedalListBlock_w31::destruct);
RW_FUNCTION(0x77DC14, "master::DrawMedalList::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", DrawMedalListBlock_w31::destructDelete);
RW_FUNCTION(0x77DC4C, "master::DrawMedalList::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", DrawMedalListBlock_w31::onZeroShared);
RW_FUNCTION(0x77DC58, "master::DrawMedalList::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", DrawMedalListBlock_w31::onZeroSharedWeak);
RW_FUNCTION(0x77DC60, "master::DrawMedalList::create(int)::impl::~impl (deleting)", DrawMedalListBlock_w31::rowDestructDelete);

using MiscBlock_w31 = EmplaceBlockFunctions<Misc>;
RW_FUNCTION(0x7870A8, "master::Misc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", MiscBlock_w31::destruct);
RW_FUNCTION(0x7870DC, "master::Misc::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", MiscBlock_w31::destructDelete);
RW_FUNCTION(0x787114, "master::Misc::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", MiscBlock_w31::onZeroShared);
RW_FUNCTION(0x787120, "master::Misc::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", MiscBlock_w31::onZeroSharedWeak);
RW_FUNCTION(0x787128, "master::Misc::create(int)::impl::~impl (deleting)", MiscBlock_w31::rowDestructDelete);

using TitleBlock_w31 = EmplaceBlockFunctions<Title>;
RW_FUNCTION(0x78FF6C, "master::Title::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", TitleBlock_w31::destruct);
RW_FUNCTION(0x78FFA0, "master::Title::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", TitleBlock_w31::destructDelete);
RW_FUNCTION(0x78FFD8, "master::Title::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", TitleBlock_w31::onZeroShared);
RW_FUNCTION(0x78FFE4, "master::Title::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", TitleBlock_w31::onZeroSharedWeak);
RW_FUNCTION(0x78FFEC, "master::Title::create(int)::impl::~impl (deleting)", TitleBlock_w31::rowDestructDelete);

}  // namespace master
