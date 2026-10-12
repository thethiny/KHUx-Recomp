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

## Status

- Milestone M0 (analysis and coverage report): done.
- M1 (lifter, hybrid execution): the game boots to the main menu with every function it executes on the
  way lifted to C (about 4,900 functions). Switch tables and indirect jumps still run in the emulator.
- The emulator is the reference used to develop the private server and the specification for the native
  runtime.
- Next: lift switch tables and indirect calls natively, then a native runtime replacing the Python one.
