# KHUx v1.0.1 emulation and static recompilation

AI Assisted emulation with human reversed addresses and index generation.

This repository runs the original, unmodified Android native library of Kingdom Hearts Union χ v1.0.1
(`libcocos2dcpp.so`, 32-bit ARM / Thumb) on a desktop PC. It is also the groundwork for a static
recompilation of that library into portable C.

- **Emulator** (`emu/`): Unicorn executes the ARM code. A Python runtime implements libc, pthreads, JNI,
  OpenGL ES (through ANGLE), sockets and the Android pieces the game calls. The game boots, downloads its
  data, and plays through the tutorial, story quests and raids against a compatible private server.
- **Analyzer** (`analyze.py`): reads the ELF and the function index and produces the function list,
  ARM/Thumb modes, PLT map, switch tables, noreturn calls and a coverage report. This is the input to the
  recompiler.
- **Lifter** (`lift/`): translates ARM and Thumb-1 functions into portable C99 (`lift/lifter.py`, runtime
  header `lift/rt.h`) and builds them into `out/lift/lift.dll` (`lift/build.py`).
- **Hybrid mode** (`emu/hybrid.py`): lifted functions run natively in place of their emulated originals.
  Emulator and C code share one block of guest memory. Anything the C code cannot follow (unsupported
  instructions, switch tables, indirect jumps) is handed back to the emulator at that exact instruction.

The function addresses, names and the index the tools rely on come from manual reverse engineering of the
binary. The code that uses them was written with AI assistance.

## Requirements

- Python 3.9 or newer
- `pip install unicorn pyelftools capstone glfw`
- ANGLE (`libEGL.dll`, `libGLESv2.dll`) copied into `emu/angle/`. Any recent Chromium-based build ships
  them. The emulator renders OpenGL ES 2.0 through them.
- A private server for the game listening on port 80 of `127.0.0.1` (see `REDIRECT_IP` in `emu/net.py`).
  Every hostname the game resolves is redirected there.
- For the lifter: Visual Studio 2022 with the C++ workload (MSVC). `lift/build.py` runs `vcvars64.bat` and `cl`.

## Inputs

The game files are not part of this repository. Put them in one folder:

| File | Needed by |
|---|---|
| `libcocos2dcpp.so` (v1.0.1, armeabi) | emulator, analyzer |
| KHUx v1.0.1 APK, named `khux_1.0.1.apk` (or set `KHUX_APK` to its path or file name) | emulator (assets, Java side) |
| `libcocos2dcpp_decompile/index.txt`, `names.txt` | analyzer, function tracing |

Then either set `KHUX_INPUT` to that folder, or place it at `../Input/1.0.1` relative to this repository.
If neither exists, the tools stop with an error naming the missing folder or file. See `paths.py`.

## Running

Run the modules from the folder that contains this repository, with the repository folder named `recomp`.

```
python -m recomp.emu.run                # start the emulator (opens a window)
python -m recomp.emu.run --fresh        # wipe the emulated app data first (fresh install)
python -m recomp.emu.boot_to_title      # wait for the title screen and tap through it
python -m recomp.emu.play_quest         # play the quest or raid offered on screen
python -m recomp.emu.campaign 10 2400   # play quests back to back
python recomp/analyze.py                # write out/analysis.json and out/report.md
python -m recomp.emu.run --coverage     # record executed code (out/coverage.json)
python recomp/lift/build.py             # lift the executed functions and build out/lift/lift.dll
python -m recomp.emu.run --hybrid       # run with lifted functions (checked against the emulator)
python recomp/lift/find_bad.py          # bisect a lifted function that breaks the game
python recomp/lift/fn_test.py 0xADDR arm r0 r1 ...   # one lifted function vs the original, outside the game
```

Hybrid mode checks every lifted leaf function against the emulator on its first calls
(`KHUX_VERIFY_CALLS`, default 20) and writes `out/lift/hybrid_report.json`. `--no-verify` skips the checks.

While the emulator runs, a control channel on `127.0.0.1:8765` exposes the scene, the UI tree, taps and
swipes, text lookups and memory reads:

```
python -m recomp.emu.ctl ui | state | events
python -m recomp.emu.step tap X Y
curl "127.0.0.1:8765/ui?all=1"
```

Debug switches: `KHUX_TRACE_FUNCS=0xADDR,...`, `KHUX_TRACE_PARSE=Class`, `KHUX_BATTLE_VERBOSE=1`.

## Layout

```
paths.py        input and output locations
analyze.py      binary analysis for the recompiler
lift/           lifter (ARM/Thumb to C), runtime header, build and bisect tools
emu/            Unicorn emulator and Python runtime
  core.py         CPU, memory, ELF loading, guest calls
  libc.py         libc and file system (the device file system maps into fs/)
  threads.py      guest threads
  jni.py          JNI environment and Java-side stubs
  android.py      APK and Android specifics
  gles.py         OpenGL ES through ANGLE
  net.py          sockets and DNS
  control.py      control channel
  coverage.py     executed-code recording (with ARM/Thumb mode)
  hybrid.py       runs lifted C functions inside the emulator
  run.py          entry point
  auto_battle.py, play_quest.py, campaign.py   automated play for testing
fs/             emulated device storage (generated, ignored)
out/            analyzer output (generated, ignored)
```

## Progress

```
Decompiled              ████████████████████   99.0%
Lifted                  ████████████████████  100.0%
Rewritten               ████████████░░░░░░░░   59.4%
Run in game             ██░░░░░░░░░░░░░░░░░░   11.8%
Verified                ██░░░░░░░░░░░░░░░░░░    8.9%
  Game: rewritten       ███████████████████░   94.4%
  Game: verified        ███░░░░░░░░░░░░░░░░░   17.2%
  Libraries: rewritten  ████████░░░░░░░░░░░░   41.2%
  Libraries: verified   █░░░░░░░░░░░░░░░░░░░    4.6%
1:1 binary match        ░░░░░░░░░░░░░░░░░░░░    0.0%
Recompiled              ████████████████████  100.0%
```

Updated by `python recomp/progress.py`.

- **Decompiled**: 56,014 of 56,604 functions have Hex-Rays pseudocode (590 failed). Reference only.
- **Lifted**: 56,381 of 56,381 analysed functions translate to C (machine-generated, one C function per guest function, 65 instructions left to the emulator). 14,189 of them run and are checked against the emulator in game.
- **Rewritten**: Readable, maintainable source (named types and variables, no register-level code). 33,510 functions.
- **Run in game**: Rewritten functions the game has called while the M3 verifier compared them with the lifted original (6,662).
- **Verified**: Rewritten functions whose every checked call matched the lifted original (5,023); normal play installs only these.
- **Game: rewritten**: The game's own code (19,342 functions, including Square Enix's sound engine): 18,257 rewritten.
- **Game: verified**: Game functions verified: 3,334.
- **Libraries: rewritten**: Open-source libraries (37,033 functions: cocos2d-x, libc++, OpenSSL, freetype, libpng, ...; `rewrite/tools/libsplit.py`), which a native build can take from upstream source: 15,253 rewritten.
- **Libraries: verified**: Library functions verified: 1,689.
- **1:1 binary match**: Source that rebuilds the original ARM binary byte for byte (a matching decompilation). Not started.
- **Recompiled**: A native executable that runs without the emulator. The native host (`host/`, Windows) runs all 56,381 lifted functions natively, with real threads, sound, sockets, JNI and OpenGL ES, and no emulator or Python: it plays the game through login, the main menu and quests against the private server.

## Status

- Milestone M0 (analysis and coverage report): done.
- M1 (lifter, hybrid execution): done. The game boots to the main menu with every function it executes lifted
  to C.
- M2 (native runtime): done. The native host (`host/`) loads the library image and runs every lifted function
  as native C, with real threads, sound, sockets, JNI and OpenGL ES, and no emulator or Python. It plays the game
  through login, the main menu, quests, raids and multiplayer against the private server.
  - Every function of the library lifts and compiles (`lift/build.py --all --out out/lift_all`: 56,363
    functions, 0 errors). IDA's split pieces of a function (fragments, Thumb-1 long-branch pieces, switch
    dispatch halves) are absorbed into it, and `__gnu_thumb1_case_*` switches dispatch natively.
  - In C: memory and string functions, math, `qsort`, the guest heap (`lift/rt_heap.c`), stdio streams
    (`lift/rt_files.c`) and OpenGL ES (`lift/gl_table.py`).
- M3 (the rewrite), in progress: readable C++ in `rewrite/` replacing lifted functions one at a time
  (`out/rewrite/rewrite.dll`, loaded by the host through per-function override slots). 31,044 functions written so
  far (16,055 of the game's 19,776): the game's core, master data, network, battle, scenes, actors, UI classes and
  the sound engine. 4,756 have run under the verifier and 3,091 are verified (sessions with real fights:
  `verify_session.py --fight`). Every game function the game executes (native or emulator coverage) is written except
  two big ones (`APIManager::verifyResponse`, `DeckMedalView::updateLayout`); after those come the never-executed
  ones. The open-source libraries come from upstream
  source at the native step.
  - A differential verifier (`KHUX_RW_VERIFY=1`) runs each call twice, first the lifted original and then the
    rewrite, from the same state. It compares the results and every guest byte stored, then undoes the losing
    run. Clocks, random numbers, host imports and outside effects (server calls, saves, sound) are recorded in
    the first run and replayed in the second.
  - Normal play installs only the functions that passed verification (`rewrite/verified.txt`). Reviewed, harmless
    mismatches (stale stack bytes the original copies, audio-thread stack races) are listed in
    `rewrite/accepted.txt`.
  - Small fixed-shape functions (forwarders, getters, setters, constants, thunks, std::function slots) are
    generated by `rewrite/tools/gen_simple.py` into `rewrite/src/gen/` (about 13,600): it analyses each lifted
    function symbolically and emits it only when every instruction is understood.
- The emulator is the reference used to develop the private server and the specification for the native
  runtime.


## License

MIT, see [LICENSE](LICENSE). Credit is required: keep the copyright notice in copies and derived work. Game data and assets are not covered (they belong to Square Enix).
