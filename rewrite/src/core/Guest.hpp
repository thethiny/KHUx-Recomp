// Guest helpers shared by the core / master rewrites: well-known library entry points, guest stack scratch
// memory, and the 32-bit libc++ layouts (std::string, std::vector, std::shared_ptr) seen from rewritten code.
#pragma once

#include <string>

#include "rw/rw.hpp"

// rw.hpp's Adapter evaluates sizeof(R) even for a void result (MSVC error C2070), so a void function cannot be
// registered with it; this more specialised form covers void functions (no return register compared).
namespace rw {
template <class... A, void (*F)(A...)> struct Adapter<F> {
    static constexpr int ret_words = 0;
    static void run(CPU *c) {
        int k = 0;
        std::tuple<A...> args{AbiArg<A>::get(c, k)...};
        std::apply(F, args);
    }
};
}  // namespace rw

namespace guest {

// ------------------------------------------------------------------------------------------ library functions
// (Thumb addresses, as the lifted code calls them)
constexpr uint32_t OPERATOR_NEW = 0xC98EF1;           // operator new(size_t)
constexpr uint32_t OPERATOR_DELETE = 0xC98F9B;        // operator delete(void*)
constexpr uint32_t OPERATOR_NEW_ARRAY = 0xC98F6F;     // operator new[](size_t)
constexpr uint32_t OPERATOR_DELETE_ARRAY = 0xC98FB3;  // operator delete[](void*)
constexpr uint32_t SHARED_ADD_REF = 0xC8DBB5;         // std::__shared_weak_count::__add_shared (sub_C8DBB4)
constexpr uint32_t SHARED_RELEASE = 0xC8DBD1;         // std::__shared_weak_count::__release_shared (sub_C8DBD0)
constexpr uint32_t ATOMIC_LOAD_GUARD = 0xCD8411;      // sub_CD8410(guard, 0, 0): atomic read of a static guard
constexpr uint32_t CXA_GUARD_ACQUIRE = 0xC98BC5;      // __cxa_guard_acquire (sub_C98BC4)
constexpr uint32_t CXA_GUARD_RELEASE = 0xC98C85;      // __cxa_guard_release (sub_C98C84)
constexpr uint32_t CXA_ATEXIT = 0x40DCD8;             // __cxa_atexit (import)
constexpr uint32_t EXIT = 0x40DE04;                   // exit (import)
constexpr uint32_t DSO_HANDLE = 0xFEF000;             // &__dso_handle passed to __cxa_atexit

inline uint32_t newObject(uint32_t size) { return rw::call(OPERATOR_NEW, {size}); }
inline void deleteObject(uint32_t p) { rw::call(OPERATOR_DELETE, {p}); }
inline uint32_t newArray(uint32_t size) { return rw::call(OPERATOR_NEW_ARRAY, {size}); }
inline void deleteArray(uint32_t p) { rw::call(OPERATOR_DELETE_ARRAY, {p}); }
inline void addRef(uint32_t ctrl) { rw::call(SHARED_ADD_REF, {ctrl}); }
inline void release(uint32_t ctrl) { rw::call(SHARED_RELEASE, {ctrl}); }

// A function-local static's guard, as the compiler emits it: true when this call must run the initializer
// (the caller then runs it and calls staticInitDone).
inline bool staticInitBegin(uint32_t guard) {
    if (rw::call(ATOMIC_LOAD_GUARD, {guard, 0, 0}) & 1) return false;
    return rw::call(CXA_GUARD_ACQUIRE, {guard}) != 0;
}
inline void staticInitDone(uint32_t guard) { rw::call(CXA_GUARD_RELEASE, {guard}); }

// ------------------------------------------------------------------------------------------ guest stack
// Scratch memory on the guest stack for the lifetime of a scope (out-parameters of guest calls, buffers the
// original kept in its frame). Stores there are outside what the verifier compares. Give the original frame size
// when the original reads bytes it never initialised (stale stack contents), so they come from the same address.
class Frame {
public:
    explicit Frame(uint32_t size) : saved_(rw::cpu->r[13]) {
        base_ = (saved_ - size) & ~7u;
        rw::cpu->r[13] = base_;
    }
    ~Frame() { rw::cpu->r[13] = saved_; }
    Frame(const Frame &) = delete;
    Frame &operator=(const Frame &) = delete;
    uint32_t addr(uint32_t offset = 0) const { return base_ + offset; }
    template <class T> T *at(uint32_t offset) const { return rw::frame_ptr<T>(base_ + offset); }

private:
    uint32_t saved_, base_;
};

// ------------------------------------------------------------------------------------------ libc++ layouts
// std::shared_ptr<T>: object pointer, control block (__shared_weak_count: vtable, owners - 1, weak - 1, ...).
struct SharedPtr {
    rw::Field<uint32_t> obj;   // +0
    rw::Field<uint32_t> ctrl;  // +4
};
RW_SIZE(SharedPtr, 8);

// std::vector<T>: begin, end, end of capacity.
struct Vector {
    rw::Field<uint32_t> begin;  // +0
    rw::Field<uint32_t> end;    // +4
    rw::Field<uint32_t> cap;    // +8
};
RW_SIZE(Vector, 12);

// std::string (32-bit libc++): short form when byte0 bit0 == 0 (size = byte0 >> 1, chars at +1), else long
// form (capacity | 1 at +0, size at +4, data pointer at +8).
struct String {
    const uint8_t raw[12];
    bool isLong() const { return raw[0] & 1; }
    uint32_t size() const { return isLong() ? rw::get<uint32_t>(raw, 4) : uint32_t(raw[0] >> 1); }
    // guest address of the characters (c_str())
    uint32_t data() const { return isLong() ? rw::get<uint32_t>(raw, 8) : rw::guest_addr(raw + 1); }
    std::string str() const { return std::string(rw::host_ptr<const char>(data()), size()); }
};
RW_SIZE(String, 12);

// A NUL-terminated guest string as a host std::string.
inline std::string cstr(uint32_t addr) { return addr ? std::string(rw::host_ptr<const char>(addr)) : std::string(); }

}  // namespace guest
