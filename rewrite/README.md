# M3: the rewrite

Readable C++ that replaces the lifted (machine-translated) functions of the game library one at a time. Each
rewritten function is checked against the lifted original on real calls while the game runs, and only functions
that passed are used in normal play.

The game still runs at its original guest addresses (one flat 32-bit address space), so rewritten code works on
the same objects as the lifted code. Moving data off guest addresses comes after the code is rewritten.

## Layout

```
rewrite/
  include/rw/rw.hpp      the runtime API (fields, pointers, calls, registration)
  runtime/               registry, trampolines, the verifier
  src/<area>/<Class>.hpp game object layouts (structs over guest memory)
  src/<area>/<Class>.cpp the class's functions, each registered with RW_FUNCTION
  tools/unit.py          the work unit for one function or class (pseudocode, lifted C, callees)
  tools/merge_stats.py   folds verification results into status.tsv / verified.txt
  build.py               builds out/rewrite/rewrite.dll (--check FILE...: compile only)
  verified.txt           functions that passed verification (installed in normal play)
  status.tsv             per-function verification results
```

Areas: `core` (SafeBuffer, AppState, utilities), `net` (APIManager, hole::network), `master` (master data
tables), `battle` (StageState, StageManager, actors), `scene` (Scene*, UI), `lib/<library>` (cocos2d-x,
cocostudio, LWF, libc++ pieces, rapidjson, tinyxml2...).

## Writing a function

1. `python recomp/rewrite/tools/unit.py <address or Class>` prints the Hex-Rays pseudocode (a guide; its types and
   stack layouts can be wrong) and the **lifted C, which is the exact behaviour you must reproduce**.
2. Declare the object layout in the class header: `rw::Field<T>` for values, `rw::PtrField<T>` for guest
   pointers, `uint8_t pad_<offset>[n]` for unknown bytes. Check known offsets with `RW_LAYOUT(Type, member, off)`
   and the size with `RW_SIZE` when it is known. A derived class derives from its base struct.
3. Write the function as ordinary C++ with typed parameters (a member function's `this` is the first parameter,
   named `self`) and register it: `RW_FUNCTION(0xADDRESS, "Class::method", Class::method);` (address without the
   Thumb bit is fine).
4. Every store to guest memory must be logged so the verifier can undo it: `Field` / `PtrField` members,
   `rw::store8` / `store16` / `store32`, `rw::set<T>(obj, offset, v)`, `rw::save<T>(addr, v)`. The compiler
   enforces this: a guest pointer to a plain value (`rw::host_ptr<uint32_t>`, `Ptr<int32_t>`, `PtrField<uint8_t>`)
   is `const`, and a registered function's pointer parameter to a plain value must be declared `const`. Scratch
   memory on the guest stack below sp (a `guest::Frame`, `rw::frame_ptr<T>`) is writable directly. Raw byte
   arrays in guest structs (padding, unmodelled members) are `const`, so a whole-struct copy (an unlogged byte
   copy) does not compile; `field = otherField` is a logged store.
5. Calls to guest functions that are not rewritten (or not yours): `rw::call(0xADDRESS, {args...})` with the
   callee's name in a comment; virtual calls: `rw::vcall(obj, slot, {args})`. Calling a rewritten function of the
   same module directly in C++ is fine.
6. Match the original exactly, including its quirks: ARM integer arithmetic wraps (compute in `uint32_t`), signed
   shifts are arithmetic, `float` stays `float`, the evaluation order of calls with side effects is the
   original's. The game's own bugs stay in (this is a faithful rewrite).
7. Functions with effects the verifier cannot undo (drawing with OpenGL, writing files, sockets, playing audio,
   JNI) are registered with `RW_FUNCTION_NOVERIFY`. Time and random numbers are fine with `RW_FUNCTION`: during
   a shadow run the clocks stand still and both runs draw the same random numbers. Reading files and allocating
   guest memory are fine too (the guest heap is guest memory, so the second run allocates the same addresses).
   Use `rw::call_exact` for calls whose callee frames must sit where the original's did (frames reproduced with
   `guest::Frame` of the original size).
8. `python recomp/rewrite/build.py --check src/<area>/<Class>.cpp` must compile cleanly.

Helpers: `rw::callf` (a guest call returning a float), `rw::fbits` / `rw::bitsf` (softfp float bits),
`rw::call_exact` (a call laid out like the compiled one: no stack realignment, arguments 5+ at sp),
`guest::Frame` (src/core/Guest.hpp: guest stack scratch, the libc++ layouts, operator new/delete, static guards).

## Verifying

`KHUX_RW_VERIFY=1 recomp/out/host/khux.exe` installs every rewritten function and shadow-runs each call: the
lifted original first (its stores logged, then undone), then the rewrite, comparing the return registers and every
guest byte either one stored outside the callee stack frames. A mismatch is logged (`RW MISMATCH ...`) and the
lifted result is kept, so the game is never affected. During a shadow run the clocks stand still and both runs
draw the same random numbers, and calls to the effect functions (sending a request, saving, analytics, sounds;
the list is `EFFECTS` in runtime/rw_runtime.cpp) run once for real in the lifted run and are replayed in the
rewrite run, so functions that reach them are verified without doing anything twice. NOVERIFY functions are not
installed while verifying. `python recomp/rewrite/tools/merge_stats.py` then updates
`status.tsv` and `verified.txt`. Without `KHUX_RW_VERIFY`, only the verified functions run; `KHUX_REWRITE=0` runs
the lifted code only.
