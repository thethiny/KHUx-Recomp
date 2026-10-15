// master::X::create(int) returns std::make_shared<impl>(...): the row object (a SafeBuffer subclass, "impl") lives
// inside a libc++ __shared_ptr_emplace control block (vtable, owners - 1, weak owners - 1, then the impl at +12).
// These are that control block's slots and the impl's deleting destructor, for the tables whose blocks were left.
#include "../core/SafeBuffer.hpp"
#include "../master/DrawMedalType.hpp"
#include "../master/DrawSkillList.hpp"
#include "../master/Player.hpp"
#include "../master/SerialcodeReward.hpp"

namespace {

constexpr uint32_t SHARED_WEAK_COUNT_DTOR = 0xC8DB69;   // nullsub_4: std::__shared_weak_count::~__shared_weak_count
constexpr uint32_t BLOCK_IMPL = 12;                     // the emplaced impl inside the control block

template <class T>
struct CreateBlock {
    // ~__shared_ptr_emplace(): destroys the emplaced impl (its SafeBuffer), then the __shared_weak_count base
    static uint32_t destruct(uint32_t self) {
        rw::save<uint32_t>(self, T::CREATE_BLOCK_VTABLE);
        SafeBuffer::destruct(rw::host_ptr<SafeBuffer>(self + BLOCK_IMPL));
        rw::call(SHARED_WEAK_COUNT_DTOR, {self});
        return self;
    }
    static void destructDelete(uint32_t self) {
        destruct(self);
        guest::deleteObject(self);
    }
    // __on_zero_shared(): the last owner went away - the impl's virtual destructor (slot 0)
    static void onZeroShared(uint32_t self) {
        uint32_t impl = self + BLOCK_IMPL;
        rw::vcall(rw::host_ptr<SafeBuffer>(impl), 0, {impl});
    }
    // __on_zero_shared_weak(): the last weak owner went away - free the block
    static void onZeroSharedWeak(uint32_t self) { guest::deleteObject(self); }
    // impl's deleting destructor: ~SafeBuffer, then operator delete on what it returned
    static void implDestructDelete(SafeBuffer *self) {
        guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self)));
    }
};

using DrawMedalTypeBlock = CreateBlock<master::DrawMedalType>;
using DrawSkillListBlock = CreateBlock<master::DrawSkillList>;
using PlayerBlock = CreateBlock<master::Player>;
using SerialcodeRewardBlock = CreateBlock<master::SerialcodeReward>;

}  // namespace

RW_FUNCTION(0x77EBDC, "master::DrawMedalType::create::impl __shared_ptr_emplace::~__shared_ptr_emplace", DrawMedalTypeBlock::destruct);
RW_FUNCTION(0x77EC10, "master::DrawMedalType::create::impl __shared_ptr_emplace::~__shared_ptr_emplace (deleting)", DrawMedalTypeBlock::destructDelete);
RW_FUNCTION(0x77EC48, "master::DrawMedalType::create::impl __shared_ptr_emplace::__on_zero_shared", DrawMedalTypeBlock::onZeroShared);
RW_FUNCTION(0x77EC54, "master::DrawMedalType::create::impl __shared_ptr_emplace::__on_zero_shared_weak", DrawMedalTypeBlock::onZeroSharedWeak);
RW_FUNCTION(0x77EC5C, "master::DrawMedalType::create::impl::~impl (deleting)", DrawMedalTypeBlock::implDestructDelete);

RW_FUNCTION(0x77F0FC, "master::DrawSkillList::create::impl __shared_ptr_emplace::~__shared_ptr_emplace", DrawSkillListBlock::destruct);
RW_FUNCTION(0x77F130, "master::DrawSkillList::create::impl __shared_ptr_emplace::~__shared_ptr_emplace (deleting)", DrawSkillListBlock::destructDelete);
RW_FUNCTION(0x77F168, "master::DrawSkillList::create::impl __shared_ptr_emplace::__on_zero_shared", DrawSkillListBlock::onZeroShared);
RW_FUNCTION(0x77F174, "master::DrawSkillList::create::impl __shared_ptr_emplace::__on_zero_shared_weak", DrawSkillListBlock::onZeroSharedWeak);
RW_FUNCTION(0x77F17C, "master::DrawSkillList::create::impl::~impl (deleting)", DrawSkillListBlock::implDestructDelete);

RW_FUNCTION(0x787644, "master::Player::create::impl __shared_ptr_emplace::~__shared_ptr_emplace", PlayerBlock::destruct);
RW_FUNCTION(0x787678, "master::Player::create::impl __shared_ptr_emplace::~__shared_ptr_emplace (deleting)", PlayerBlock::destructDelete);
RW_FUNCTION(0x7876B0, "master::Player::create::impl __shared_ptr_emplace::__on_zero_shared", PlayerBlock::onZeroShared);
RW_FUNCTION(0x7876BC, "master::Player::create::impl __shared_ptr_emplace::__on_zero_shared_weak", PlayerBlock::onZeroSharedWeak);
RW_FUNCTION(0x7876C4, "master::Player::create::impl::~impl (deleting)", PlayerBlock::implDestructDelete);

RW_FUNCTION(0x790850, "master::SerialcodeReward::create::impl __shared_ptr_emplace::~__shared_ptr_emplace", SerialcodeRewardBlock::destruct);
RW_FUNCTION(0x790884, "master::SerialcodeReward::create::impl __shared_ptr_emplace::~__shared_ptr_emplace (deleting)", SerialcodeRewardBlock::destructDelete);
RW_FUNCTION(0x7908BC, "master::SerialcodeReward::create::impl __shared_ptr_emplace::__on_zero_shared", SerialcodeRewardBlock::onZeroShared);
RW_FUNCTION(0x7908C8, "master::SerialcodeReward::create::impl __shared_ptr_emplace::__on_zero_shared_weak", SerialcodeRewardBlock::onZeroSharedWeak);
RW_FUNCTION(0x7908D0, "master::SerialcodeReward::create::impl::~impl (deleting)", SerialcodeRewardBlock::implDestructDelete);
