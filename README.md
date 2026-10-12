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

The function addresses, names and the index the tools rely on come from manual reverse engineering of the
binary. The code that uses them was written with AI assistance.

## Requirements

- Python 3.9 or newer
- `pip install unicorn pyelftools capstone glfw`
- ANGLE (`libEGL.dll`, `libGLESv2.dll`) copied into `emu/angle/`. Any recent Chromium-based build ships
  them. The emulator renders OpenGL ES 2.0 through them.
- A private server for the game listening on port 80 of `127.0.0.1` (see `REDIRECT_IP` in `emu/net.py`).
  Every hostname the game resolves is redirected there.

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
```

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
emu/            Unicorn emulator and Python runtime
  core.py         CPU, memory, ELF loading, guest calls
  libc.py         libc and file system (the device file system maps into fs/)
  threads.py      guest threads
  jni.py          JNI environment and Java-side stubs
  android.py      APK and Android specifics
  gles.py         OpenGL ES through ANGLE
  net.py          sockets and DNS
  control.py      control channel
  run.py          entry point
  auto_battle.py, play_quest.py, campaign.py   automated play for testing
fs/             emulated device storage (generated, ignored)
out/            analyzer output (generated, ignored)
```

## Status

- Milestone M0 (analysis and coverage report): done.
- The emulator is the reference used to develop the private server and the specification for the native
  runtime.
- Next: the lifter (ARM and Thumb to C), checked function by function against the emulator, then a native
  runtime replacing the Python one.
