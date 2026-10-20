// FreeType TrueType glyph hinting (ARM code inside the game's FreeType build): loading a size's state into the
// bytecode execution context and running the CVT program (sub_44FF84), and hinting one glyph (sub_44F6D8).
// Plain struct copies between the size, the loader and the context; field names follow FreeType's ttobjs / ttgload.
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t MEMCPY = 0x442298;            // memcpy (import)
constexpr uint32_t MEMSET = 0x442310;            // memset (import)
constexpr uint32_t ft_mem_realloc = 0x9B91D8;    // (memory, item_size, cur, new, block, FT_Error* err)

inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
inline void st16(uint32_t a, uint32_t v) { rw::save<uint16_t>(a, uint16_t(v)); }
inline void copyWords(uint32_t dst, uint32_t src, uint32_t n) {
    for (uint32_t k = 0; k < n; k++) st32(dst + 4 * k, ld32(src + 4 * k));
}

// Run the size's CVT program in its execution context (TT_Size run_prep part): copy the size's metrics, graphics
// state, storage and twilight zone into the context, grow its stack / storage to the face's maxima, run the
// program unless debugging, then copy the results back. Returns the FreeType error.
uint32_t ttSizeRunPrep(uint32_t size, uint32_t debug) {
    const uint32_t face = ld32(size);
    const uint32_t exec = ld8(size + 0x124) ? ld32(size + 0x128) : ld32(ld32(face + 0x60) + 0x1C);
    if (!exec) return 0x99;
    st32(exec, face);
    st32(exec + 4, size);
    st32(exec + 0x18C, ld32(size + 0x74));
    st32(exec + 0x190, ld32(size + 0x78));
    st32(exec + 0x198, ld32(size + 0x80));
    st32(exec + 0x19C, ld32(size + 0x84));
    st32(exec + 0x194, ld32(size + 0x7C));
    st32(exec + 0x1A0, ld32(size + 0x88));
    copyWords(exec + 0xF4, size + 0x48, 10);       // metrics
    copyWords(exec + 0xD8, size + 0x2C, 7);
    st32(exec + 0x1A4, ld32(size + 0x8C));
    st32(exec + 0x1A8, ld32(size + 0x90));
    for (uint32_t k = 0; k < 0x18; k += 8) {        // the three code ranges
        st32(exec + 0x1BC + k, ld32(size + 0x94 + k));
        st32(exec + 0x1C0 + k, ld32(size + 0x98 + k));
    }
    call(MEMCPY, {exec + 0x11C, size + 0xAC, 0x44});   // graphics state
    st32(exec + 0x17C, ld32(size + 0xF0));
    st32(exec + 0x180, ld32(size + 0xF4));
    st16(exec + 0x1D4, ld16(size + 0xF8));
    st32(exec + 0x1D8, ld32(size + 0xFC));
    copyWords(exec + 0xB4, size + 0x100, 9);       // twilight zone
    call(MEMSET, {exec + 0x24, 0, 0x24});          // zp0 .. zp2 = an empty zone
    copyWords(exec + 0x48, exec + 0x24, 9);
    copyWords(exec + 0x6C, exec + 0x24, 9);

    Frame f;
    const uint32_t err = f.alloc(4);
    const uint32_t maxp = face + 0x104;
    // the stack: maxStackElements + 32
    uint32_t stackSize = ld32(exec + 0x14);
    const uint32_t wantStack = ld16(maxp + 0x18) + 0x20;
    uint32_t error = 0;
    if (stackSize < wantStack) {
        st32(exec + 0x18, rw::call(ft_mem_realloc, {ld32(exec + 8), 1, stackSize * 4, wantStack * 4,
                                                     ld32(exec + 0x18), err}));
        error = ld32(err);
        if (!error) stackSize = wantStack;
    }
    st32(exec + 0x14, stackSize);
    if (error) return error;
    // the glyph instructions buffer: maxSizeOfInstructions
    uint32_t insSize = ld32(exec + 0x184);
    const uint32_t wantIns = ld16(maxp + 0x1A);
    if (insSize < wantIns) {
        st32(exec + 0x188, rw::call(ft_mem_realloc, {ld32(exec + 8), 1, insSize, wantIns, ld32(exec + 0x188), err}));
        error = ld32(err);
        if (!error) insSize = wantIns;
    }
    st32(exec + 0x184, insSize & 0xFFFF);
    if (error) return error;

    st16(exec + 0x98, 0);                           // twilight.n_points / n_contours
    st16(exec + 0x9A, 0);
    copyWords(exec + 0x48, exec + 0x90, 9);         // zp0 = zp1 = zp2 = twilight
    copyWords(exec + 0x6C, exec + 0x90, 9);
    copyWords(exec + 0x24, exec + 0x90, 9);
    st32(exec + 0x1AC, 0);
    st32(exec + 0x10, 0);
    st8(exec + 0x1E8, 0);
    st8(exec + 0x231, uint8_t(debug));
    const uint32_t prepSize = ld32(face + 0x280), prepCode = ld32(face + 0x27C);
    st32(exec + 0x1CC, 0);
    st32(exec + 0x1C4, prepSize);
    st32(exec + 0x1C8, prepCode);
    st32(exec + 0x1D0, 0);
    if (ld32(face + 0x27C)) {
        st32(exec + 0x164, prepSize);               // code range: the CVT program
        st32(exec + 0x16C, prepCode);
        st32(exec + 0x168, 0);
        st32(exec + 0x160, 2);
        if (!ld8(size + 0x124)) error = call(ld32(face + 0x28C), {exec});   // face->interpreter(exec)
    }
    st32(size + 0x130, error);                      // cvt_ready
    st16(exec + 0x122, 0x4000);                     // GS: vectors to the x axis
    st16(exec + 0x126, 0x4000);
    st16(exec + 0x124, 0);
    st16(exec + 0x12A, 0x4000);
    st16(exec + 0x128, 0);
    st16(exec + 0x12C, 0);
    st16(exec + 0x11C, 0);                          // rp0..rp2
    st16(exec + 0x11E, 0);
    st16(exec + 0x120, 0);
    st16(exec + 0x158, 1);                          // gep0..gep2
    st16(exec + 0x15A, 1);
    st16(exec + 0x15C, 1);
    st32(exec + 0x130, 1);                          // loop
    call(MEMCPY, {size + 0xAC, exec + 0x11C, 0x44});
    st32(size + 0x74, ld32(exec + 0x18C));
    st32(size + 0x80, ld32(exec + 0x198));
    st32(size + 0x8C, ld32(exec + 0x1A4));
    st32(size + 0x90, ld32(exec + 0x1A8));
    for (uint32_t k = 0; k < 0x18; k += 8) {
        st32(size + 0x94 + k, ld32(exec + 0x1BC + k));
        st32(size + 0x98 + k, ld32(exec + 0x1C0 + k));
    }
    return error;
}

// Hint one glyph (TT_Hint_Glyph): save the unhinted points, set up the context's glyph zone and run the glyph's
// instructions; the four phantom points are rounded to whole pixels first, and copied back to the loader after.
uint32_t ttHintGlyph(uint32_t loader, uint32_t isComposite) {
    const uint32_t nIns = ld32(ld32(loader + 8) + 0x8C);
    if (nIns) call(MEMCPY, {ld32(loader + 0x88), ld32(loader + 0x8C), ld16(loader + 0x84) * 8});   // orus
    call(MEMCPY, {ld32(loader + 0xA0) + 0x11C, ld32(loader + 4) + 0xAC, 0x44});   // the size's graphics state
    if (isComposite) {
        st32(ld32(loader + 0xA0) + 0xDC, 0x10000);
        st32(ld32(loader + 0xA0) + 0xE0, 0x10000);
        call(MEMCPY, {ld32(loader + 0x90), ld32(loader + 0x8C), ld16(loader + 0x84) * 8});
    } else {
        st32(ld32(loader + 0xA0) + 0xDC, ld32(ld32(loader + 4) + 0x30));   // x / y scale
        st32(ld32(loader + 0xA0) + 0xE0, ld32(ld32(loader + 4) + 0x34));
    }
    // round the phantom points: x of pp1, pp2, y of pp3, pp4
    auto point = [&](uint32_t back) { return ld32(loader + 0x8C) + (ld16(loader + 0x84) - back) * 8; };
    for (uint32_t back : {4u, 3u}) {
        const uint32_t p = point(back);
        st32(p, (ld32(p) + 0x20) & ~0x3Fu);
    }
    for (uint32_t back : {2u, 1u}) {
        const uint32_t p = point(back) + 4;
        st32(p, (ld32(p) + 0x20) & ~0x3Fu);
    }
    if (nIns) {
        const uint32_t exec = ld32(loader + 0xA0);
        const uint32_t glyphFlags = ld32(ld32(loader + 0xC) + 0x40);
        st32(exec + 0x1D0, nIns);                   // glyph code range
        st32(exec + 0x1CC, ld32(exec + 0x188));
        st8(ld32(loader + 0xA0) + 0x230, uint8_t(isComposite));
        uint32_t x = ld32(loader + 0xA0);
        copyWords(x + 0x90, loader + 0x7C, 9);      // the glyph zone
        x = ld32(loader + 0xA0);
        st32(x + 0x164, ld32(x + 0x1CC));
        st32(x + 0x16C, ld32(x + 0x1D0));
        st32(x + 0x168, 0);
        st32(x + 0x160, 3);
        copyWords(x + 0x24, x + 0x90, 9);           // zp0 = zp1 = zp2 = the glyph zone
        copyWords(x + 0x48, x + 0x90, 9);
        copyWords(x + 0x6C, x + 0x90, 9);
        st16(x + 0x158, 1);                         // gep0..gep2
        st16(x + 0x15A, 1);
        st16(x + 0x15C, 1);
        st16(x + 0x126, 0x4000);                    // vectors to the x axis
        st16(x + 0x128, 0);
        st16(x + 0x12A, 0x4000);
        st16(x + 0x12C, 0);
        st16(x + 0x122, 0x4000);
        st16(x + 0x124, 0);
        st32(x + 0x138, 1);
        st32(x + 0x130, 1);
        st32(x + 0x10, 0);
        st32(x + 0x1AC, 0);
        const uint32_t error = vcall(x, 0x28C, {x});   // face->interpreter(exec)
        const uint32_t e2 = ld32(loader + 0xA0);
        if (error && ld8(e2 + 0x231)) return error;   // debugging: keep the error
        st8(glyphFlags, uint8_t(ld8(glyphFlags) | (ld32(e2 + 0x154) << 5) | 4));
    }
    const uint32_t pp1 = point(4);
    st32(loader + 0x44, ld32(pp1));
    st32(loader + 0x48, ld32(pp1 + 4));
    st32(loader + 0x4C, ld32(pp1 + 8));
    st32(loader + 0x50, ld32(pp1 + 0xC));
    st32(loader + 0xB8, ld32(pp1 + 0x10));
    st32(loader + 0xBC, ld32(pp1 + 0x14));
    st32(loader + 0xC0, ld32(pp1 + 0x18));
    st32(loader + 0xC4, ld32(pp1 + 0x1C));
    return 0;
}
}  // namespace

RW_FUNCTION(0x44FF84, "FreeType tt_size_run_prep (sub_44FF84)", ttSizeRunPrep);
RW_FUNCTION(0x44F6D8, "FreeType TT_Hint_Glyph (sub_44F6D8)", ttHintGlyph);
