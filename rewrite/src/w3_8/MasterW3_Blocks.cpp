// make_shared control block and row destructors of master::Chapter, EvResource, LoginBonus, Medal, Sphere and World
// create(int) (the five compiler-generated functions, see w3_9/MasterEmplaceBlock.hpp).
#include "w3_9/MasterEmplaceBlock.hpp"
#include "master/Chapter.hpp"
#include "master/EvResource.hpp"
#include "master/LoginBonus.hpp"
#include "master/Medal.hpp"
#include "master/Sphere.hpp"
#include "master/World.hpp"

namespace master {

using ChapterBlock = EmplaceBlockFunctions<Chapter>;
RW_FUNCTION(0x77C3E8, "master::Chapter::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", ChapterBlock::destruct);
RW_FUNCTION(0x77C41C, "master::Chapter::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", ChapterBlock::destructDelete);
RW_FUNCTION(0x77C454, "master::Chapter::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", ChapterBlock::onZeroShared);
RW_FUNCTION(0x77C460, "master::Chapter::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", ChapterBlock::onZeroSharedWeak);
RW_FUNCTION(0x77C468, "master::Chapter::create(int)::impl::~impl (deleting)", ChapterBlock::rowDestructDelete);

using EvResourceBlock = EmplaceBlockFunctions<EvResource>;
RW_FUNCTION(0x7931A4, "master::EvResource::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", EvResourceBlock::destruct);
RW_FUNCTION(0x7931D8, "master::EvResource::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", EvResourceBlock::destructDelete);
RW_FUNCTION(0x793210, "master::EvResource::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", EvResourceBlock::onZeroShared);
RW_FUNCTION(0x79321C, "master::EvResource::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", EvResourceBlock::onZeroSharedWeak);
RW_FUNCTION(0x793224, "master::EvResource::create(int)::impl::~impl (deleting)", EvResourceBlock::rowDestructDelete);

using LoginBonusBlock = EmplaceBlockFunctions<LoginBonus>;
RW_FUNCTION(0x7851E4, "master::LoginBonus::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", LoginBonusBlock::destruct);
RW_FUNCTION(0x785218, "master::LoginBonus::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", LoginBonusBlock::destructDelete);
RW_FUNCTION(0x785250, "master::LoginBonus::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", LoginBonusBlock::onZeroShared);
RW_FUNCTION(0x78525C, "master::LoginBonus::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", LoginBonusBlock::onZeroSharedWeak);
RW_FUNCTION(0x785264, "master::LoginBonus::create(int)::impl::~impl (deleting)", LoginBonusBlock::rowDestructDelete);

using MedalBlock = EmplaceBlockFunctions<Medal>;
RW_FUNCTION(0x7867C8, "master::Medal::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", MedalBlock::destruct);
RW_FUNCTION(0x7867FC, "master::Medal::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", MedalBlock::destructDelete);
RW_FUNCTION(0x786834, "master::Medal::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", MedalBlock::onZeroShared);
RW_FUNCTION(0x786840, "master::Medal::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", MedalBlock::onZeroSharedWeak);
RW_FUNCTION(0x786848, "master::Medal::create(int)::impl::~impl (deleting)", MedalBlock::rowDestructDelete);

using SphereBlock = EmplaceBlockFunctions<Sphere>;
RW_FUNCTION(0x78D574, "master::Sphere::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", SphereBlock::destruct);
RW_FUNCTION(0x78D5A8, "master::Sphere::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", SphereBlock::destructDelete);
RW_FUNCTION(0x78D5E0, "master::Sphere::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", SphereBlock::onZeroShared);
RW_FUNCTION(0x78D5EC, "master::Sphere::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", SphereBlock::onZeroSharedWeak);
RW_FUNCTION(0x78D5F4, "master::Sphere::create(int)::impl::~impl (deleting)", SphereBlock::rowDestructDelete);

using WorldBlock = EmplaceBlockFunctions<World>;
RW_FUNCTION(0x777AB4, "master::World::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace", WorldBlock::destruct);
RW_FUNCTION(0x777AE8, "master::World::create(int) __shared_ptr_emplace<impl>::~__shared_ptr_emplace (deleting)", WorldBlock::destructDelete);
RW_FUNCTION(0x777B20, "master::World::create(int) __shared_ptr_emplace<impl>::__on_zero_shared", WorldBlock::onZeroShared);
RW_FUNCTION(0x777B2C, "master::World::create(int) __shared_ptr_emplace<impl>::__on_zero_shared_weak", WorldBlock::onZeroSharedWeak);
RW_FUNCTION(0x777B34, "master::World::create(int)::impl::~impl (deleting)", WorldBlock::rowDestructDelete);

}  // namespace master
