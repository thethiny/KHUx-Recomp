"""Boot libcocos2dcpp.so v1.0.1 under Unicorn.

Usage: python -m recomp.emu.run [--fresh] [--trace-imports] [--dbg-files] [--hybrid [--no-verify]] [--coverage]
  --fresh   wipe the emulated app data first (fresh install)
  --hybrid  run lifted functions from out/lift/lift.dll (see emu/hybrid.py)
Control the running game with: python -m recomp.emu.ctl (see control.py)
"""
import sys

import struct
import time

import glfw

from . import android, control, gles, libc, net, opensl, threads
from .libc import f2u
from .core import Emu, GuestFault
from .jni import JNI, JArray


def step(emu, what, fn, *args):
    emu.log(f"== {what}")
    try:
        r = emu.call(fn, *args)
    except GuestFault as e:
        emu.log(f"!! {what} failed: {e}")
        sys.exit(1)
    emu.log(f"== {what} -> {r:#x}")
    return r


def main():
    # guest strings can hold any character; never let the Windows console encoding kill the emulator
    for stream in (sys.stdout, sys.stderr):
        try:
            stream.reconfigure(errors="backslashreplace")
        except AttributeError:
            pass
    if "--fresh" in sys.argv:
        # wipe the emulated device's app data (save, UserDefault prefs, downloaded m files) - like a
        # fresh install. A half-finished download leaves a save the game never recovers from.
        import shutil
        app_data = libc.FS_ROOT / f"data/data/{libc.PACKAGE}"
        shutil.rmtree(app_data, ignore_errors=True)
        print(f"--fresh: wiped {app_data}")
    emu = Emu(trace_imports="--trace-imports" in sys.argv)
    libc.install(emu)
    threads.install(emu)
    android.install(emu)
    opensl.install(emu)
    gles.install(emu, android.SCREEN_W, android.SCREEN_H)
    jni = JNI(emu)
    emu.jni = jni
    if "--dbg-files" in sys.argv:
        from unicorn.arm_const import UC_ARM_REG_R1, UC_ARM_REG_R2
        emu.on(0x8D4758, lambda e: e.log(f"  [getValueMapFromFile] {e.std_string(e.reg(UC_ARM_REG_R2))}"))
        emu.on(0x8D9454, lambda e: e.log(f"  [SAXParser::parse file] {e.std_string(e.reg(UC_ARM_REG_R1))}"))
        emu.on(0x8D93BC, lambda e: e.log(
            f"  [SAXParser::parse data] len={e.reg(UC_ARM_REG_R2)} head={e.read(e.reg(UC_ARM_REG_R1), 80)!r}"))
        # FileUtilsAndroid::getData(const std::string& file, bool forString): sret r0, this r1, file r2.
        # Both getStringFromFile and getDataFromFile end up here on Android.
        emu.on(0xA3F76C, lambda e: e.log(f"  [getData] {e.std_string(e.reg(UC_ARM_REG_R2))}"))
    emu.log(f"loaded: {len(emu.slot_of)} import slots, {len(emu.data_imports)} data imports, "
            f"{len(emu.init_array)} constructors")

    if "--coverage" in sys.argv:
        # record executed code blocks to out/coverage.json (the hybrid patches only executed functions)
        from . import coverage
        coverage.install(emu)
    if "--hybrid" in sys.argv:
        # lifted C functions (recomp/lift) run in place of their emulated originals, checked on first call
        from . import hybrid
        hybrid.install(emu, verify="--no-verify" not in sys.argv)

    ctors = [c for c in emu.init_array if c not in (0, 0xFFFFFFFF)]
    for k, ctor in enumerate(ctors):
        try:
            emu.call(ctor)
        except GuestFault as e:
            emu.log(f"!! constructor {k} ({ctor:#x}) failed: {e}")
            return 1
    emu.log(f"== {len(ctors)} constructors ran")

    sym = emu.symbols
    step(emu, "JNI_OnLoad", sym["JNI_OnLoad"], jni.vm, 0)
    helper = jni.new_ref(jni.find_class("org/cocos2dx/lib/Cocos2dxHelper"))
    step(emu, "nativeSetApkPath", sym["Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetApkPath"],
         jni.env, helper, jni.new_ref(android.APK_DEVICE_PATH))
    context = jni.new_ref(jni.find_class("android/content/Context"))
    asset_mgr = jni.new_ref(jni.find_class("android/content/res/AssetManager"))
    step(emu, "nativeSetContext", sym["Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetContext"],
         jni.env, helper, context, asset_mgr)
    renderer = jni.new_ref(jni.find_class("org/cocos2dx/lib/Cocos2dxRenderer"))
    emu.control = control.Control(emu, jni, renderer)   # hooks must exist before the first scene runs
    step(emu, "nativeInit", sym["Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit"],
         jni.env, renderer, android.SCREEN_W, android.SCREEN_H)
    emu.log(f"heap used {(emu.heap.ptr - 0x20000000) // 1024} KB")
    return main_loop(emu, jni, renderer)


def main_loop(emu, jni, renderer):
    sym = emu.symbols
    render = sym["Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender"]
    touch_begin = sym["Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin"]
    touch_end = sym["Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd"]
    touch_move = sym["Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove"]
    video_cb = sym["Java_org_cocos2dx_lib_Cocos2dxVideoHelper_nativeExecuteVideoCallback"]
    video_cls = jni.new_ref(jni.find_class("org/cocos2dx/lib/Cocos2dxVideoHelper"))
    ctl = emu.control
    win = emu.gl.window
    pressed = [False]
    frame = 0
    t0 = time.time()
    while not glfw.window_should_close(win):
        glfw.poll_events()
        x, y = glfw.get_cursor_pos(win)
        down = glfw.get_mouse_button(win, glfw.MOUSE_BUTTON_LEFT) == glfw.PRESS
        injected = ctl.touch_for_frame(frame)
        if injected:
            down, x, y = injected
        try:
            # nativeTouches*(env, thiz, int id, float x, float y): soft-float, floats in core registers
            if down and not pressed[0]:
                emu.call(touch_begin, jni.env, renderer, 0, f2u(x), f2u(y))
            elif down:
                # nativeTouchesMove(int[] ids, float[] xs, float[] ys)
                ids = jni.new_ref(JArray("I", bytearray(struct.pack("<i", 0))))
                xs = jni.new_ref(JArray("F", bytearray(struct.pack("<f", x))))
                ys = jni.new_ref(JArray("F", bytearray(struct.pack("<f", y))))
                emu.call(touch_move, jni.env, renderer, ids, xs, ys)
            elif pressed[0]:
                emu.call(touch_end, jni.env, renderer, 0, f2u(x), f2u(y))
            pressed[0] = down
            while emu.android.video_events:
                idx, ev = emu.android.video_events.pop(0)
                emu.log(f"  video {idx}: event {ev} (completed)")
                emu.call(video_cb, jni.env, video_cls, idx, ev)
            emu.gl.draw_calls = 0
            emu.call(render, jni.env, renderer)
            emu.sched.run_workers()
            ctl.on_frame(frame)
        except GuestFault as e:
            emu.log(f"!! frame {frame} failed: {e}")
            ctl.event("crash", error=str(e).splitlines()[0])
            return 1
        emu.gl.swap()
        frame += 1
        if frame % 600 == 0:
            dt = time.time() - t0
            emu.log(f"-- frame {frame}: {600 / dt:.1f} fps, {emu.gl.draw_calls} draws")
            t0 = time.time()
    return 0


if __name__ == "__main__":
    sys.exit(main())
