/* Runtime support for code lifted from the ARMv5TE (ARM + Thumb-1) game library.
 *
 * Guest memory is one flat 4 GB block: guest address a is host address mem + a. The emulator maps the same
 * block, so lifted code and emulated code share every byte. Registers and flags live in CPU, which the
 * caller fills before a lifted function runs and reads back afterwards.
 *
 * A lifted function returns by storing the guest return address (Thumb bit included) in c->pc.
 */
#ifndef KHUX_LIFT_RT_H
#define KHUX_LIFT_RT_H

#include <stdint.h>

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#define INLINE static __forceinline
#else
#define EXPORT __attribute__((visibility("default")))
#define INLINE static inline __attribute__((always_inline))
#endif

typedef struct WLog {
    uint32_t addr, size, old_lo, old_hi;   /* store of `size` bytes at addr; previous contents */
} WLog;

typedef struct CPU {
    uint32_t r[16];        /* r0..r12, sp, lr, pc (pc unused while running) */
    uint32_t n, z, c, v;   /* condition flags, 0 or 1 */
    uint32_t pc;           /* guest address the function returned to */
    uint32_t logging;      /* nonzero: record every store in wlog (differential testing) */
    uint32_t wn, wcap;     /* used / capacity of wlog */
    uint8_t *mem;          /* host address of guest address 0 */
    WLog *wlog;
    /* Run guest code at `target` in the emulator (callee not lifted or not enabled). On return the
     * registers are back in the struct and c->pc is the return address, or anything else after a fault. */
    void (*emu)(struct CPU *c, uint32_t target);
    uint32_t tail;         /* set with c->pc by a tail call: continue at c->pc with the same LR */
    uint32_t lpc;          /* guest address of the instruction being executed (crash reports) */
    uint32_t icount_pad;
    uint64_t icount;       /* guest instructions executed in lifted code (profiling) */
} CPU;

/* Call guest function `target` (Thumb bit set for Thumb code): a lifted, enabled function runs directly,
 * anything else through c->emu. Defined in the generated lift_table.c. Callers set LR first and afterwards
 * check that c->pc is their return address; otherwise they return too (the emulator continues there). */
void rt_call(CPU *c, uint32_t target);
/* Entry from the host hook: like rt_call, but a tail call into code that is not lifted is not run in the
 * emulator - it returns with c->pc = target so the emulator simply jumps there. */
EXPORT void rt_run(CPU *c, uint32_t target);
/* A lifted function's tail call: tail-call chains (IDA-split loops) run in rt_call's loop, not recursively. */
#define TAIL(t) do { c->pc = (t); c->tail = 1; return; } while (0)
/* Call into a frame fragment (an IDA-split piece of a function that returns through its parent's frame, not to
 * LR): run it in C when lifted, otherwise escape to it - the emulator cannot run it as a call, since it never
 * returns to the call site. */
void rt_call_frag(CPU *c, uint32_t target);

/* ---------------------------------------------------------------- memory */
#define HA(a) (c->mem + (uint32_t)(a))
#define R8(a)  (*(uint8_t *)HA(a))
#define R16(a) (*(uint16_t *)HA(a))
#define R32(a) (*(uint32_t *)HA(a))
#define RS8(a)  ((uint32_t)(int32_t)*(int8_t *)HA(a))
#define RS16(a) ((uint32_t)(int32_t)*(int16_t *)HA(a))

INLINE void wlog(CPU *c, uint32_t a, uint32_t size) {
    if (c->wn < c->wcap) {
        WLog *w = &c->wlog[c->wn++];
        w->addr = a;
        w->size = size;
        w->old_lo = size == 1 ? R8(a) : size == 2 ? R16(a) : R32(a);
        w->old_hi = 0;
    } else {
        c->wn = c->wcap + 1;   /* overflow marker: the test is inconclusive */
    }
}

INLINE void W8(CPU *c, uint32_t a, uint32_t v)  { if (c->logging) wlog(c, a, 1); R8(a) = (uint8_t)v; }
INLINE void W16(CPU *c, uint32_t a, uint32_t v) { if (c->logging) wlog(c, a, 2); R16(a) = (uint16_t)v; }
INLINE void W32(CPU *c, uint32_t a, uint32_t v) { if (c->logging) wlog(c, a, 4); R32(a) = v; }

/* ---------------------------------------------------------------- flags */
INLINE uint32_t ADC_F(CPU *c, uint32_t a, uint32_t b, uint32_t cin) {
    uint64_t s = (uint64_t)a + b + cin;
    uint32_t r = (uint32_t)s;
    c->c = (uint32_t)(s >> 32) & 1;
    c->v = ((~(a ^ b)) & (a ^ r)) >> 31;
    c->n = r >> 31;
    c->z = r == 0;
    return r;
}
#define ADD_F(a, b)      ADC_F(c, (a), (b), 0)
#define SUB_F(a, b)      ADC_F(c, (a), ~(uint32_t)(b), 1)
#define SBC_F(a, b, cin) ADC_F(c, (a), ~(uint32_t)(b), (cin))

INLINE uint32_t NZ(CPU *c, uint32_t r) { c->n = r >> 31; c->z = r == 0; return r; }

/* ---------------------------------------------------------------- barrel shifter
 * type: 0 LSL, 1 LSR, 2 ASR, 3 ROR, 4 RRX. amt is the shift amount (immediate, or the low byte of a
 * register). *co receives the shifter carry-out (unchanged cin when the amount is 0). */
INLINE uint32_t SHIFT(uint32_t v, int type, uint32_t amt, uint32_t cin, uint32_t *co) {
    *co = cin;
    switch (type) {
    case 0:
        if (amt == 0) return v;
        if (amt < 32) { *co = (v >> (32 - amt)) & 1; return v << amt; }
        *co = amt == 32 ? (v & 1) : 0;
        return 0;
    case 1:
        if (amt == 0) return v;
        if (amt < 32) { *co = (v >> (amt - 1)) & 1; return v >> amt; }
        *co = amt == 32 ? (v >> 31) : 0;
        return 0;
    case 2:
        if (amt == 0) return v;
        if (amt < 32) { *co = (v >> (amt - 1)) & 1; return (uint32_t)((int32_t)v >> amt); }
        *co = v >> 31;
        return (uint32_t)((int32_t)v >> 31);
    case 3:
        if (amt == 0) return v;
        amt &= 31;
        if (amt == 0) { *co = v >> 31; return v; }
        v = (v >> amt) | (v << (32 - amt));
        *co = v >> 31;
        return v;
    default: /* RRX */
        *co = v & 1;
        return (cin << 31) | (v >> 1);
    }
}

INLINE uint32_t CLZ(uint32_t v) {
    uint32_t n = 0;
    if (!v) return 32;
    while (!(v & 0x80000000u)) { v <<= 1; n++; }
    return n;
}

/* ---------------------------------------------------------------- export table */
typedef void (*LiftFn)(CPU *);
typedef struct LiftEntry {
    uint32_t addr;   /* guest entry, Thumb bit set for Thumb functions */
    uint32_t flags;  /* bit 0: leaf (no calls, no escapes) - verifiable against the emulator */
    LiftFn fn;
} LiftEntry;

#endif
