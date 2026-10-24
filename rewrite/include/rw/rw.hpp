// M3 rewrite runtime: what rewritten functions are written against.
//
// The game still runs from its original guest addresses (32-bit guest address space, one flat block; guest
// address a is host address cpu->mem + a), so a rewritten function sees the same objects the lifted code sees.
// Game objects are declared as plain structs laid over guest memory (rw::Field members; layouts checked with
// static_assert), guest pointers are rw::Ptr<T> (a 32-bit guest address), and a function is an ordinary C++
// function with typed parameters, registered with RW_FUNCTION at the guest address it replaces.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <tuple>
#include <type_traits>

extern "C" {
#include "rt.h"
}

namespace rw {

// ---------------------------------------------------------------------------------------------------- state
// The guest thread running the current rewritten function (set by the entry trampoline).
extern thread_local CPU *cpu;

inline uint8_t *mem() { return cpu->mem; }
inline uint32_t guest_addr(const void *p) { return p ? uint32_t(static_cast<const uint8_t *>(p) - mem()) : 0; }
// A guest pointer to a plain value (integer, float) is read-only: writes must go through store8 / store32 / set /
// save so the verifier can log and undo them. Structs of Field members stay writable (their Fields log).
template <class T> using GuestRef = std::conditional_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, const T, T>;
template <class T> inline GuestRef<T> *host_ptr(uint32_t a) {
    return a ? reinterpret_cast<GuestRef<T> *>(mem() + a) : nullptr;
}

// Record a store about to happen when the verifier is logging (rt.h wlog); false = the log is full and the
// store must be skipped (the run is void anyway).
inline bool log_store(const void *p, uint32_t size) {
    if (!cpu->logging) return true;
    uint32_t a = guest_addr(p);
    if (size <= 4) return wlog(cpu, a, size) != 0;
    if ((uint64_t)cpu->wn + (size + 3) / 4 > cpu->wcap) { cpu->wfull = 1; cpu->logging = 0; return true; }
    for (uint32_t k = 0; k < size; k += 4) wlog(cpu, a + k, size - k >= 4 ? 4 : size - k);
    return true;
}

// ---------------------------------------------------------------------------------------------------- views
// A field of a guest object. Reads are plain; every write goes through log_store, so the verifier can undo it.
template <class T>
struct Field {
    T raw;
    operator T() const { return raw; }
    T get() const { return raw; }
    Field &operator=(T v) { if (log_store(&raw, sizeof(T))) raw = v; return *this; }
    // field = other field: a logged store too (the implicit copy would be a raw, unlogged byte copy)
    Field &operator=(const Field &o) { return *this = o.raw; }
    Field &operator+=(T v) { return *this = T(raw + v); }
    Field &operator-=(T v) { return *this = T(raw - v); }
    Field &operator++() { return *this = T(raw + 1); }
};

// A guest pointer: 32 bits in guest memory, dereferenced through the guest block.
template <class T>
struct Ptr {
    uint32_t addr;
    GuestRef<T> *get() const { return host_ptr<T>(addr); }
    GuestRef<T> *operator->() const { return get(); }
    GuestRef<T> &operator*() const { return *get(); }
    GuestRef<T> &operator[](size_t k) const { return get()[k]; }
    explicit operator bool() const { return addr != 0; }
};

// A guest-pointer member of a guest object (Ptr is the value type; never put a PtrField on the host stack).
template <class T>
struct PtrField {
    uint32_t addr;
    GuestRef<T> *get() const { return host_ptr<T>(addr); }
    GuestRef<T> *operator->() const { return get(); }
    GuestRef<T> &operator[](size_t k) const { return get()[k]; }
    operator Ptr<T>() const { return Ptr<T>{addr}; }
    explicit operator bool() const { return addr != 0; }
    PtrField &operator=(Ptr<T> p) { if (log_store(&addr, 4)) addr = p.addr; return *this; }
    PtrField &operator=(const PtrField &o) { return *this = Ptr<T>{o.addr}; }
    PtrField &operator=(T *p) { return *this = Ptr<T>{guest_addr(p)}; }
    PtrField &operator=(std::nullptr_t) { return *this = Ptr<T>{0}; }
};

// Byte stores outside a Field (buffers).
inline void store8(const uint8_t *p, uint8_t v) { if (log_store(p, 1)) *const_cast<uint8_t *>(p) = v; }
inline void store32(const void *p, uint32_t v) { if (log_store(p, 4)) std::memcpy(const_cast<void *>(p), &v, 4); }
inline void store16(const void *p, uint16_t v) { if (log_store(p, 2)) std::memcpy(const_cast<void *>(p), &v, 2); }

// A value at a byte offset of a guest object whose layout is not declared (yet) or belongs to another module.
template <class T> inline T get(const void *obj, uint32_t offset) {
    T v;
    std::memcpy(&v, static_cast<const uint8_t *>(obj) + offset, sizeof(T));
    return v;
}
template <class T> inline void set(const void *obj, uint32_t offset, T v) {
    uint8_t *p = const_cast<uint8_t *>(static_cast<const uint8_t *>(obj)) + offset;
    if (log_store(p, sizeof(T))) std::memcpy(p, &v, sizeof(T));
}
// The same through a guest address.
template <class T> inline T load(uint32_t addr) { return get<T>(mem() + addr, 0); }
template <class T> inline void save(uint32_t addr, T v) { set<T>(mem() + addr, 0, v); }

#define RW_LAYOUT(type, member, offset) static_assert(offsetof(type, member) == (offset), #type "::" #member)
#define RW_SIZE(type, size) static_assert(sizeof(type) == (size), "sizeof " #type)

// ---------------------------------------------------------------------------------------------------- calls
// Call a guest function (lifted or rewritten) with integer / pointer arguments; returns r0 (r0:r1 for call64).
uint32_t call(uint32_t fn, std::initializer_list<uint32_t> args);
uint64_t call64(uint32_t fn, std::initializer_list<uint32_t> args);
// Like a compiled call: sp stays exactly as it is (no realignment) and arguments 5+ go to [sp, ...) - the
// outgoing-argument area at the bottom of the caller's frame. Use it when the rewrite reproduces the original
// frame (guest::Frame with the original size) so callee frames land at the original addresses. Returns r0:r1.
uint64_t call_exact(uint32_t fn, std::initializer_list<uint32_t> args);
// A C++ virtual call on a guest object: the function pointer at vtable + slot * 4.
inline uint32_t vcall(const void *obj, uint32_t slot, std::initializer_list<uint32_t> args) {
    uint32_t vtable = *reinterpret_cast<const uint32_t *>(obj);
    uint32_t fn = *host_ptr<uint32_t>(vtable + slot * 4);
    return call(fn, args);
}
template <class T> inline uint32_t arg(T *p) { return guest_addr(p); }
// Scratch memory on the guest stack below sp (a frame): writable directly - stores there are outside what the
// verifier compares and need no undo.
template <class T> inline T *frame_ptr(uint32_t a) { return const_cast<T *>(host_ptr<T>(a)); }
// Float arguments / results travel in core registers (softfp): bit casts.
inline uint32_t fbits(float f) { uint32_t u; std::memcpy(&u, &f, 4); return u; }
inline float bitsf(uint32_t u) { float f; std::memcpy(&f, &u, 4); return f; }
// Call a guest function that returns a float (in r0).
inline float callf(uint32_t fn, std::initializer_list<uint32_t> args) { return bitsf(call(fn, args)); }

// ---------------------------------------------------------------------------------------------------- ABI
// AAPCS (softfp) argument k: r0-r3, then the stack.
inline uint32_t abi_word(CPU *c, int k) {
    return k < 4 ? c->r[k] : *reinterpret_cast<uint32_t *>(c->mem + c->r[13] + 4u * uint32_t(k - 4));
}

template <class T> struct AbiArg {
    static T get(CPU *c, int &k) {
        if constexpr (std::is_pointer_v<T>) {
            static_assert(std::is_same_v<GuestRef<std::remove_pointer_t<T>> *, T>,
                          "a guest pointer parameter to a plain value must be const (write through rw::store*)");
            return host_ptr<std::remove_pointer_t<T>>(abi_word(c, k++));
        } else if constexpr (sizeof(T) == 8) {
            k += k & 1;                                       // 64-bit values start at an even register
            uint64_t lo = abi_word(c, k), hi = abi_word(c, k + 1);
            k += 2;
            uint64_t v = lo | (hi << 32);
            T out;
            std::memcpy(&out, &v, 8);
            return out;
        } else if constexpr (std::is_floating_point_v<T>) {  // softfp: a float travels in a core register
            uint32_t v = abi_word(c, k++);
            T out;
            std::memcpy(&out, &v, 4);
            return out;
        } else {
            return T(abi_word(c, k++));
        }
    }
};

template <class R> inline void abi_return(CPU *c, R v) {
    if constexpr (std::is_pointer_v<R>) {
        c->r[0] = guest_addr(v);
    } else if constexpr (sizeof(R) == 8) {
        uint64_t u;
        std::memcpy(&u, &v, 8);
        c->r[0] = uint32_t(u);
        c->r[1] = uint32_t(u >> 32);
    } else if constexpr (std::is_floating_point_v<R>) {
        std::memcpy(&c->r[0], &v, 4);
    } else {
        c->r[0] = uint32_t(v);
    }
}

// Guest registers a result occupies (a host pointer is 8 bytes but one 32-bit guest address; void: none).
template <class R> constexpr int ret_words_of() {
    if constexpr (std::is_void_v<R>) return 0;
    else if constexpr (std::is_pointer_v<R>) return 1;
    else return sizeof(R) == 8 ? 2 : 1;
}

// Adapter from the guest ABI to a typed C++ function F.
template <auto F> struct Adapter;
template <class R, class... A, R (*F)(A...)> struct Adapter<F> {
    static constexpr int ret_words = ret_words_of<R>();
    static void run(CPU *c) {
        int k = 0;
        // braces: arguments are read left to right
        std::tuple<A...> args{AbiArg<A>::get(c, k)...};
        if constexpr (std::is_void_v<R>) std::apply(F, args);
        else abi_return<R>(c, std::apply(F, args));
    }
};

// ---------------------------------------------------------------------------------------------------- registry
struct Entry {
    uint32_t addr;          // guest address it replaces (Thumb bit ignored)
    const char *name;       // the original symbol, demangled
    void (*run)(CPU *c);    // ABI adapter
    int ret_words;          // 0, 1 or 2: return registers compared by the verifier
    bool verify;            // false: has effects the verifier cannot undo (I/O, GL...): never shadow-run
    void (*tramp)(CPU *c);  // what the lift.dll override slot points at (calls enter with this entry's index)
    bool effect = false;    // an effect boundary (runtime list), not a rewrite: see "effects" in rw_runtime.cpp
};
int add(const Entry &e);
void enter(CPU *c, int index);

struct Reg {
    Reg(uint32_t addr, const char *name, void (*run)(CPU *), int ret_words, bool verify, void (*tramp)(CPU *),
        int *index) {
        *index = add(Entry{addr, name, run, ret_words, verify, tramp});
    }
};

}  // namespace rw

#define RW_CAT2(a, b) a##b
#define RW_CAT(a, b) RW_CAT2(a, b)
// RW_FUNCTION(0x45A268, "SafeBuffer::read", SafeBuffer::read): replace the guest function at that address.
#define RW_REGISTER(addr, name, fn, verify)                                                                      static int RW_CAT(rw_idx_, __LINE__);                                                                      static void RW_CAT(rw_tramp_, __LINE__)(CPU *c) { rw::enter(c, RW_CAT(rw_idx_, __LINE__)); }                static rw::Reg RW_CAT(rw_reg_, __LINE__)(addr, name, &rw::Adapter<&fn>::run, rw::Adapter<&fn>::ret_words,                                              verify, &RW_CAT(rw_tramp_, __LINE__), &RW_CAT(rw_idx_, __LINE__))
#define RW_FUNCTION(addr, name, fn) RW_REGISTER(addr, name, fn, true)
// Same, for a function the verifier must not shadow-run (side effects outside guest memory).
#define RW_FUNCTION_NOVERIFY(addr, name, fn) RW_REGISTER(addr, name, fn, false)

// the library functions the game code calls, by name (generated: rewrite/tools/gen_lib_bindings.py)
#include "rw/lib.hpp"
