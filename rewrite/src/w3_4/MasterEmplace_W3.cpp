// The make_shared control blocks of the master rows: master::X::create(int) builds its row as a local class
// create(int)::impl (a SafeBuffer) inside a libc++ __shared_ptr_emplace block: vtable +0, shared owners +4,
// weak owners +8, the impl object +12. These are the block's virtual functions and impl's deleting destructor,
// for the tables AvatarCombination, DrawSkillType, RaidReward, Shop, Skill and Stage.
#include "master/AvatarCombination.hpp"
#include "master/DrawSkillType.hpp"
#include "master/RaidReward.hpp"
#include "master/Shop.hpp"
#include "master/Skill.hpp"
#include "master/Stage.hpp"

namespace w3_4_master {

namespace {
constexpr uint32_t SAFEBUFFER_DTOR = 0x45A13D;          // SafeBuffer::~SafeBuffer() (returns this)
constexpr uint32_t SHARED_WEAK_COUNT_DTOR = 0xC8DB69;   // std::__shared_weak_count::~__shared_weak_count (nullsub_4)
constexpr uint32_t OBJECT = 12;                         // the impl row inside the block

template <class T> struct Emplace {
    // ~__shared_ptr_emplace(): destroys the row (its SafeBuffer), then the count base
    static uint32_t destructor(uint32_t self) {
        rw::save<uint32_t>(self, T::CREATE_BLOCK_VTABLE);
        rw::call(SAFEBUFFER_DTOR, {self + OBJECT});
        rw::call(SHARED_WEAK_COUNT_DTOR, {self});
        return self;
    }
    static void deletingDestructor(uint32_t self) {
        destructor(self);
        guest::deleteObject(self);
    }
    // __on_zero_shared(): the row's virtual destructor (vtable slot 0)
    static uint32_t onZeroShared(uint32_t self) {
        uint32_t row = self + OBJECT;
        return rw::call(rw::load<uint32_t>(rw::load<uint32_t>(row)), {row});
    }
    // __on_zero_shared_weak(): free the block
    static void onZeroSharedWeak(uint32_t self) { guest::deleteObject(self); }
    // create(int)::impl::~impl() (deleting): SafeBuffer destructor + operator delete
    static void implDeletingDestructor(uint32_t self) { guest::deleteObject(rw::call(SAFEBUFFER_DTOR, {self})); }
};
}  // namespace

// master::AvatarCombination
RW_FUNCTION(0x778CE0, "__shared_ptr_emplace<master::AvatarCombination::create(int)::impl>::~__shared_ptr_emplace", Emplace<master::AvatarCombination>::destructor);
RW_FUNCTION(0x778D14, "__shared_ptr_emplace<master::AvatarCombination::create(int)::impl>::~__shared_ptr_emplace (deleting)", Emplace<master::AvatarCombination>::deletingDestructor);
RW_FUNCTION(0x778D4C, "__shared_ptr_emplace<master::AvatarCombination::create(int)::impl>::__on_zero_shared", Emplace<master::AvatarCombination>::onZeroShared);
RW_FUNCTION(0x778D58, "__shared_ptr_emplace<master::AvatarCombination::create(int)::impl>::__on_zero_shared_weak", Emplace<master::AvatarCombination>::onZeroSharedWeak);
RW_FUNCTION(0x778D60, "master::AvatarCombination::create(int)::impl::~impl (deleting)", Emplace<master::AvatarCombination>::implDeletingDestructor);

// master::DrawSkillType
RW_FUNCTION(0x77F6DC, "__shared_ptr_emplace<master::DrawSkillType::create(int)::impl>::~__shared_ptr_emplace", Emplace<master::DrawSkillType>::destructor);
RW_FUNCTION(0x77F710, "__shared_ptr_emplace<master::DrawSkillType::create(int)::impl>::~__shared_ptr_emplace (deleting)", Emplace<master::DrawSkillType>::deletingDestructor);
RW_FUNCTION(0x77F748, "__shared_ptr_emplace<master::DrawSkillType::create(int)::impl>::__on_zero_shared", Emplace<master::DrawSkillType>::onZeroShared);
RW_FUNCTION(0x77F754, "__shared_ptr_emplace<master::DrawSkillType::create(int)::impl>::__on_zero_shared_weak", Emplace<master::DrawSkillType>::onZeroSharedWeak);
RW_FUNCTION(0x77F75C, "master::DrawSkillType::create(int)::impl::~impl (deleting)", Emplace<master::DrawSkillType>::implDeletingDestructor);

// master::RaidReward
RW_FUNCTION(0x78A52C, "__shared_ptr_emplace<master::RaidReward::create(int)::impl>::~__shared_ptr_emplace", Emplace<master::RaidReward>::destructor);
RW_FUNCTION(0x78A560, "__shared_ptr_emplace<master::RaidReward::create(int)::impl>::~__shared_ptr_emplace (deleting)", Emplace<master::RaidReward>::deletingDestructor);
RW_FUNCTION(0x78A598, "__shared_ptr_emplace<master::RaidReward::create(int)::impl>::__on_zero_shared", Emplace<master::RaidReward>::onZeroShared);
RW_FUNCTION(0x78A5A4, "__shared_ptr_emplace<master::RaidReward::create(int)::impl>::__on_zero_shared_weak", Emplace<master::RaidReward>::onZeroSharedWeak);
RW_FUNCTION(0x78A5AC, "master::RaidReward::create(int)::impl::~impl (deleting)", Emplace<master::RaidReward>::implDeletingDestructor);

// master::Shop
RW_FUNCTION(0x7911CC, "__shared_ptr_emplace<master::Shop::create(int)::impl>::~__shared_ptr_emplace", Emplace<master::Shop>::destructor);
RW_FUNCTION(0x791200, "__shared_ptr_emplace<master::Shop::create(int)::impl>::~__shared_ptr_emplace (deleting)", Emplace<master::Shop>::deletingDestructor);
RW_FUNCTION(0x791238, "__shared_ptr_emplace<master::Shop::create(int)::impl>::__on_zero_shared", Emplace<master::Shop>::onZeroShared);
RW_FUNCTION(0x791244, "__shared_ptr_emplace<master::Shop::create(int)::impl>::__on_zero_shared_weak", Emplace<master::Shop>::onZeroSharedWeak);
RW_FUNCTION(0x79124C, "master::Shop::create(int)::impl::~impl (deleting)", Emplace<master::Shop>::implDeletingDestructor);

// master::Skill
RW_FUNCTION(0x78C98C, "__shared_ptr_emplace<master::Skill::create(int)::impl>::~__shared_ptr_emplace", Emplace<master::Skill>::destructor);
RW_FUNCTION(0x78C9C0, "__shared_ptr_emplace<master::Skill::create(int)::impl>::~__shared_ptr_emplace (deleting)", Emplace<master::Skill>::deletingDestructor);
RW_FUNCTION(0x78C9F8, "__shared_ptr_emplace<master::Skill::create(int)::impl>::__on_zero_shared", Emplace<master::Skill>::onZeroShared);
RW_FUNCTION(0x78CA04, "__shared_ptr_emplace<master::Skill::create(int)::impl>::__on_zero_shared_weak", Emplace<master::Skill>::onZeroSharedWeak);
RW_FUNCTION(0x78CA0C, "master::Skill::create(int)::impl::~impl (deleting)", Emplace<master::Skill>::implDeletingDestructor);

// master::Stage
RW_FUNCTION(0x78F98C, "__shared_ptr_emplace<master::Stage::create(int)::impl>::~__shared_ptr_emplace", Emplace<master::Stage>::destructor);
RW_FUNCTION(0x78F9C0, "__shared_ptr_emplace<master::Stage::create(int)::impl>::~__shared_ptr_emplace (deleting)", Emplace<master::Stage>::deletingDestructor);
RW_FUNCTION(0x78F9F8, "__shared_ptr_emplace<master::Stage::create(int)::impl>::__on_zero_shared", Emplace<master::Stage>::onZeroShared);
RW_FUNCTION(0x78FA04, "__shared_ptr_emplace<master::Stage::create(int)::impl>::__on_zero_shared_weak", Emplace<master::Stage>::onZeroSharedWeak);
RW_FUNCTION(0x78FA0C, "master::Stage::create(int)::impl::~impl (deleting)", Emplace<master::Stage>::implDeletingDestructor);
}  // namespace w3_4_master
