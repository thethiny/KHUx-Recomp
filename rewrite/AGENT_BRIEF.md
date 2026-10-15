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

## Work loop
1. `python recomp/rewrite/tools/unit.py <Class> --list` lists the class's functions (X = executed in game,
   R = already rewritten). Do the executed ones first, then all the others.
2. `python recomp/rewrite/tools/unit.py <address>` prints the Hex-Rays pseudocode (a guide: types and stack
   offsets can be wrong) and the lifted C (the exact behaviour), with the callees and their rewrite status.
3. Write `<Class>.hpp` (layout struct over guest memory: `rw::Field<T>`, `rw::PtrField<T>`, `uint8_t pad_XX[n]`,
   `RW_LAYOUT` / `RW_SIZE` checks; a derived class derives from its base struct) and `<Class>.cpp` (functions as
   `static` member functions taking `self` first; register each with
   `RW_FUNCTION(0xADDRESS, "Class::method(signature if overloaded)", Class::method);`).
4. `python recomp/rewrite/build.py --check src/<your folder>/<File>.cpp` must exit 0. Never run the full build,
   never run the game, never edit rewrite/include, rewrite/runtime, verified.txt, status.tsv or other folders.

## Rules (the verifier enforces them; breaking them corrupts the running game)
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
