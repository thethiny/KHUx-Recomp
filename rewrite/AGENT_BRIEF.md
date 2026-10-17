# Brief for a rewrite agent (M3)

You rewrite part of Kingdom Hearts Union χ v1.0.1's game library (ARMv5, 32-bit Android) as readable C++. Every
function was machine-lifted to register-level C (`recomp/out/lift_all/lift_*.c`); your C++ replaces those
functions one at a time and runs on the same guest memory (32-bit addresses). Each rewritten function is verified
at runtime against the lifted original - identical return registers and identical guest-memory stores - so it
must reproduce the lifted behaviour EXACTLY (quirks and game bugs included) while being readable: named structs and
fields, clear names, short comments on what things mean.

Read first: `recomp/rewrite/README.md` (rules), `recomp/rewrite/include/rw/rw.hpp` (runtime API),
`recomp/rewrite/src/core/Guest.hpp` (guest stack frames, libc++ layouts, operator new/delete, static guards), and
two examples: `src/core/SafeBuffer.hpp/.cpp`, `src/battle/StageState.hpp/.cpp`. Before declaring a layout of a class
you call into, look for an existing header under `recomp/rewrite/src` (grep the class name): include it instead of
redeclaring, and use rw::get/rw::set for fields it lacks (do not edit files outside your folder).

**Do all the work yourself: never start sub-agents / helper agents** (the user budgets usage per agent; one agent
per task).

## Work loop
0. **Start with `python recomp/rewrite/tools/context.py <Class> [<Class> ...]`**: the functions still to do
   (X = executed in game: do those first), where each class's layout is already declared, which files already
   register its functions, the header of every class your functions call, and the shared helpers. Use it instead
   of searching the tree; grep only for what it does not answer.
1. `python recomp/rewrite/tools/unit.py <Class> --list` lists the class's functions (X = executed in game,
   R = already rewritten). Do the executed ones first, then all the others.
2. `python recomp/rewrite/tools/unit.py <address>` prints the Hex-Rays pseudocode (a guide: types and stack
   offsets can be wrong) and the lifted C (the exact behaviour), with the callees and their rewrite status.
3. Write `<Class>.hpp` (layout struct over guest memory: `rw::Field<T>`, `rw::PtrField<T>`, `uint8_t pad_XX[n]`,
   `RW_LAYOUT` / `RW_SIZE` checks; a derived class derives from its base struct) and `<Class>.cpp` (functions as
   `static` member functions taking `self` first; register each with
   `RW_FUNCTION(0xADDRESS, "Class::method(signature if overloaded)", Class::method);`).
4. `cd recomp/rewrite && python build.py --check src/<your folder>/<File>.cpp` must exit 0 (the path is
   relative to `recomp/rewrite`). Never run the full build,
   never run the game, never edit rewrite/include, rewrite/runtime, verified.txt, status.tsv or other folders.

## Rules (the verifier enforces them; breaking them corrupts the running game)
- **Call arguments come from the lifted C, not from Hex-Rays.** At every call, pass what r0-r3 (and the stack
  words) hold at that `blx` in the lifted code. Hex-Rays often drops arguments that the original passes as register
  leftovers - e.g. `Avatar::changeLWF` calls the virtual `addChild` "with only this", but the lifted code moves
  `LWFNode::create`'s result into r1 first: the new node IS the argument (dropping it crashed the game).
- Guest-memory stores only through `Field` / `PtrField` members, `rw::store8/16/32`, `rw::set<T>(obj, off, v)`,
  `rw::save<T>(addr, v)`. Guest pointers to plain values are const (compiler-enforced); a registered function's
  pointer parameter to a plain value must be `const`. Stack scratch below sp (`guest::Frame`, `rw::frame_ptr<T>`)
  is writable directly.
- Raw byte arrays in guest structs (padding, unmodelled members) are `const uint8_t name[n];`: a whole-struct copy
  would be an unlogged byte copy, so the compiler must reject it. Assigning one Field to another is fine (logged).
- Calls to functions outside your classes: `rw::call(0xADDR, {args})` (callee name in a comment), `rw::call64`
  (64-bit result), `rw::callf` (float result), `rw::vcall(obj, slot, {args})` (virtual), `rw::call_exact` when the
  callee frame must sit at the original address (with a `guest::Frame` of the original frame size). Floats travel
  in core registers (softfp): `rw::fbits` / `rw::bitsf`.
- ARM semantics: integer arithmetic wraps (compute in uint32_t, cast back), signed shifts are arithmetic, float
  stays float, the original order of calls with side effects is kept. Float to int conversions follow libgcc
  (truncate, saturate, NaN -> 0).
- Time, random numbers, file reads, allocations, audio, requests and saving are all verifiable (the verifier
  freezes clocks, replays random numbers, and replays effect calls): use `RW_FUNCTION`. `RW_FUNCTION_NOVERIFY`
  only for something truly unrepeatable that is not in the runtime's effect list - say why in a comment.
- Library code (cocos2d, cocostudio, libc++, rapidjson, LWF, ...) is not yours: call it.
- Function-local statics and their guards are guest globals: `guest::staticInitBegin/Done`, rw::load/save.
- 32-bit libc++ layouts: std::string 12 bytes (byte0 & 1 == 0: short, size = byte0 >> 1, chars at +1; long:
  capacity|1 at +0, size +4, data pointer +8); std::vector = begin, end, capacity; std::shared_ptr = object,
  control block; std::function = 16-byte buffer + callable pointer at +16; cocos2d::Ref = vtable +0, refcount +4,
  20 bytes in total (script binding on).
- Same calls as the original, in kind too: a direct (non-virtual) call stays direct (call the rewritten function or
  `rw::call` its address), a virtual one stays `rw::vcall`. A wrong vtable slot returns garbage silently.
- Where callees read the stack frame's position (OpenSSL's `OPENSSL_cleanse` folds addresses into a global, code
  that hands frame addresses to the heap or to globals), give the frame its original size (`guest::Frame` of the
  pushed registers + the `sub sp` total) and call with `rw::call_exact`, so the callee's frame lands at the same
  address as in the original.
- A function far too large to reproduce faithfully (lifted C over ~2500 lines) may be skipped: list it with the
  reason.

After the named functions: also rewrite the unnamed functions (sub_XXXX: lambda bodies, std::function thunks,
outlined pieces) attributed to your classes: `python recomp/rewrite/tools/owners.py <Class>` lists them (the
attribution is a heuristic - if one clearly belongs elsewhere, skip it and say so). Name them after what they do
(e.g. `ScenePartyTop::onTapMemberLambda`) and register them with their address. std::function thunks follow a few
fixed shapes (operator() forwarding to the lambda body, clone, destroy, target, target_type): write them faithfully
like any other function.

You may split your class list among helper agents of your own (each owning separate files in your folder). Work
through ALL functions of ALL your classes; this is a long task, keep going until done. Final report (concise):
per class, rewritten / skipped (with reasons), files, runtime features you wished for, layout facts others need.

`src/gen/` is generated by `tools/gen_simple.py` (re-run it, never edit the files). Rewriting a function that is
generated there: register it in your own file, then re-run the generator (it skips addresses registered elsewhere).

## Self-check before moving on (every function)
Walk the lifted C top to bottom against your C++ and tick off: (a) every store - address, width (strb/strh/str),
value, order; (b) every call - target, r0-r3 at that instruction and the stack words for arguments 5+ (register
leftovers included); (c) every branch - signed vs unsigned (bge/blt vs bhs/blo), flags from ADDS/SUBS, byte vs word
loads, lsr vs asr; (d) the return registers - void when r0 is only a leftover, r0:r1 for 64-bit, tail calls
(`b <fn>`) return the callee's result. Frames: a local whose address goes to a guest function lives in a
`guest::Frame` big enough for everything you put in it. Virtual calls stay virtual; direct `bl` stays direct.
Constants: multi-byte magic values are little-endian words (`movw r1, #0x4b41; movt r1, #0x2042` = "AKB " =
0x20424B41) - copy the value from the instructions, never re-derive it from the text.

**Never reproduce garbage.** The original sometimes copies stack bytes it never set (a struct with unset fields, a
short std::string's bytes past its terminator) into memory that outlives the call. Do not mimic frame sizes, pushes
or call depths to make those bytes match: write the plain C++ and say in your report that the original copies
uninitialised bytes there. Such differences are reviewed and listed in `accepted.txt` (bytes nobody reads), never
"fixed" in the code - the rewrite is the readable version of the game, not a copy of its stack.
Soft-float results must be bit-identical. Report anything you were unsure of, per address.
