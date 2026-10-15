// SQEX sound driver: sound banks (BankImpl / OnMemoryBank) - a list of the Sound handles created from a bank.
#include "Sqex.hpp"

using namespace b2;

namespace sqex {

// BankImpl: vtable +0, +4, list head +8, tail +0xC, count +0x10, flag +0x14 (byte); OnMemoryBank: data +0x18.
// List node (16 bytes): vtable, prev, next, Sound handle.
struct BankImpl {
    // Unlinks `node` from the bank's list (the original's exact pointer updates).
    static void unlink(uint32_t self, uint32_t node) {
        uint32_t prev = ld32(node + 4), next = ld32(node + 8);
        if (prev != 0) {
            st32(prev + 8, next);
            uint32_t n = ld32(node + 8);
            if (n == 0) {
                st32(self + 0xC, prev);
                st32(prev + 8, n);
            } else {
                st32(n + 4, prev);
            }
        } else {
            st32(self + 8, next);
            if (next == 0) {
                st32(self + 0xC, next);
            } else {
                st32(next + 4, prev);
                st32(next + 4, ld32(node + 4));
            }
        }
    }

    // Drops the nodes whose sound has finished.
    static uint32_t update(uint32_t self) {
        guest::Frame f(8);
        uint32_t node = ld32(self + 8);
        while (node != 0) {
            *f.at<uint32_t>(4) = ld32(node + 0xC);
            if (rw::call(0xCE14FC, {f.addr(4)}) != 0) {  // Sound::GetStatus
                node = ld32(node + 8);
                continue;
            }
            uint32_t next = ld32(node + 8);
            unlink(self, node);
            uint32_t dtor = ld32(ld32(node) + 4);
            st32(self + 0x10, ld32(self + 0x10) - 1);
            rw::call(dtor, {node});
            node = next;
        }
        return node;
    }

    static uint32_t destruct(uint32_t self) {
        uint32_t node = ld32(self + 8);
        st32(self, got(0xFD941C) + 8);
        while (node != 0) {
            uint32_t next = ld32(node + 8);
            unlink(self, node);
            uint32_t dtor = ld32(ld32(node) + 4);
            st32(self + 0x10, ld32(self + 0x10) - 1);
            rw::call(dtor, {node});
            node = next;
        }
        st32(self, got(0xFD9250) + 8);
        return self;
    }

    static uint32_t construct(uint32_t self) {
        st32(self + 4, 0);
        st32(self + 8, 0);
        st32(self + 0xC, 0);
        st32(self, got(0xFD941C) + 8);
        st32(self + 0x10, 0);
        st8(self + 0x14, 0);
        return self;
    }

    // Creates sound `index` of the bank (virtual CreateSoundCore) and remembers its handle; returns the handle.
    static uint32_t createSound(uint32_t self, uint32_t index) {
        guest::Frame f(0xC);
        uint32_t handle = f.addr(4);
        rw::call(0xCE1328, {handle});  // Sound::Sound
        if (vcallo(self, 0x18, {self, handle, index}) != 0) return 0;
        uint32_t value = *f.at<uint32_t>(4);
        uint32_t node = malloc_(0x10, 4);
        st32(node + 4, 0);
        st32(node + 8, 0);
        st32(node + 0xC, 0);
        st32(node, got(0xFD9420) + 8);
        rw::call(0xCE1328, {node + 0xC});
        uint32_t result = *f.at<uint32_t>(4);
        if (ld32(self + 8) == 0) {
            st32(self + 0xC, node);
            st32(self + 8, node);
            st32(node + 8, 0);
            st32(node + 4, 0);
        } else {
            uint32_t tail = ld32(self + 0xC);
            st32(node + 4, tail);
            st32(node + 8, 0);
            st32(tail + 8, node);
            st32(self + 0xC, node);
        }
        st32(node + 0xC, value);
        st32(self + 0x10, ld32(self + 0x10) + 1);
        return result;
    }

    static uint32_t stopBelongSounds(uint32_t self, uint32_t fadeMs) {
        guest::Frame f(8);
        uint32_t r = self;
        for (uint32_t node = ld32(self + 8); node != 0; node = ld32(node + 8)) {
            *f.at<uint32_t>(4) = ld32(node + 0xC);
            r = rw::call(0xCE1368, {f.addr(4), fadeMs});  // Sound::Stop
        }
        return r;
    }

    static uint32_t onMemoryDestruct(uint32_t self) {
        st32(self, got(0xFD9424) + 8);
        destruct(self);
        return self;
    }
    static int32_t onMemoryCreateSoundCore(uint32_t self, uint32_t sound, uint32_t index) {
        uint32_t r = rw::call(0xCE0A20, {sound, ld32(self + 0x18), index});  // SoundSystem::CreateSound
        return int32_t(r << 24) >> 31;  // negative byte -> -1, else 0
    }
    static uint32_t onMemoryConstruct(uint32_t self) {
        construct(self);
        st32(self + 0x18, 0);
        st32(self, got(0xFD9424) + 8);
        return self;
    }
};
RW_FUNCTION(0xCE9454, "SQEX::BankImpl::Update", BankImpl::update);
RW_FUNCTION(0xCE953C, "SQEX::BankImpl::~BankImpl", BankImpl::destruct);
RW_FUNCTION(0xCE960C, "SQEX::BankImpl::BankImpl", BankImpl::construct);
RW_FUNCTION(0xCE9650, "SQEX::BankImpl::CreateSound", BankImpl::createSound);
RW_FUNCTION(0xCE9950, "SQEX::BankImpl::StopBelongSounds", BankImpl::stopBelongSounds);
RW_FUNCTION(0xCE9C2C, "SQEX::OnMemoryBank::~OnMemoryBank", BankImpl::onMemoryDestruct);
RW_FUNCTION(0xCE9C90, "SQEX::OnMemoryBank::CreateSoundCore", BankImpl::onMemoryCreateSoundCore);
RW_FUNCTION(0xCE9D00, "SQEX::OnMemoryBank::OnMemoryBank", BankImpl::onMemoryConstruct);

}  // namespace sqex
