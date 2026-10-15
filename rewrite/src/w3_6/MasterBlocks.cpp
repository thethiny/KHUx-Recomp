// make_shared control block and row destructors of master::Buff, EvStage, Keyblade, MypageBackground and
// SkillExp create(int) (the five compiler-generated functions, see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/Buff.hpp"
#include "master/EvStage.hpp"
#include "master/Keyblade.hpp"
#include "master/MypageBackground.hpp"
#include "master/SkillExp.hpp"

namespace master {

using BuffBlock = EmplaceBlockFunctions<Buff>;
RW_FUNCTION(0x77A390, "master::Buff::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", BuffBlock::destruct);
RW_FUNCTION(0x77A3C4, "master::Buff::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", BuffBlock::destructDelete);
RW_FUNCTION(0x77A3FC, "master::Buff::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", BuffBlock::onZeroShared);
RW_FUNCTION(0x77A408, "master::Buff::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", BuffBlock::onZeroSharedWeak);
RW_FUNCTION(0x77A410, "master::Buff::create(int)::impl::~impl (deleting)", BuffBlock::rowDestructDelete);

using EvStageBlock = EmplaceBlockFunctions<EvStage>;
RW_FUNCTION(0x783900, "master::EvStage::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", EvStageBlock::destruct);
RW_FUNCTION(0x783934, "master::EvStage::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", EvStageBlock::destructDelete);
RW_FUNCTION(0x78396C, "master::EvStage::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", EvStageBlock::onZeroShared);
RW_FUNCTION(0x783978, "master::EvStage::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", EvStageBlock::onZeroSharedWeak);
RW_FUNCTION(0x783980, "master::EvStage::create(int)::impl::~impl (deleting)", EvStageBlock::rowDestructDelete);

using KeybladeBlock = EmplaceBlockFunctions<Keyblade>;
RW_FUNCTION(0x784978, "master::Keyblade::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", KeybladeBlock::destruct);
RW_FUNCTION(0x7849AC, "master::Keyblade::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", KeybladeBlock::destructDelete);
RW_FUNCTION(0x7849E4, "master::Keyblade::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", KeybladeBlock::onZeroShared);
RW_FUNCTION(0x7849F0, "master::Keyblade::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", KeybladeBlock::onZeroSharedWeak);
RW_FUNCTION(0x7849F8, "master::Keyblade::create(int)::impl::~impl (deleting)", KeybladeBlock::rowDestructDelete);

using MypageBackgroundBlock = EmplaceBlockFunctions<MypageBackground>;
RW_FUNCTION(0x792C94, "master::MypageBackground::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", MypageBackgroundBlock::destruct);
RW_FUNCTION(0x792CC8, "master::MypageBackground::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", MypageBackgroundBlock::destructDelete);
RW_FUNCTION(0x792D00, "master::MypageBackground::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", MypageBackgroundBlock::onZeroShared);
RW_FUNCTION(0x792D0C, "master::MypageBackground::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", MypageBackgroundBlock::onZeroSharedWeak);
RW_FUNCTION(0x792D14, "master::MypageBackground::create(int)::impl::~impl (deleting)", MypageBackgroundBlock::rowDestructDelete);

using SkillExpBlock = EmplaceBlockFunctions<SkillExp>;
RW_FUNCTION(0x78CE20, "master::SkillExp::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", SkillExpBlock::destruct);
RW_FUNCTION(0x78CE54, "master::SkillExp::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", SkillExpBlock::destructDelete);
RW_FUNCTION(0x78CE8C, "master::SkillExp::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", SkillExpBlock::onZeroShared);
RW_FUNCTION(0x78CE98, "master::SkillExp::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", SkillExpBlock::onZeroSharedWeak);
RW_FUNCTION(0x78CEA0, "master::SkillExp::create(int)::impl::~impl (deleting)", SkillExpBlock::rowDestructDelete);

}  // namespace master
