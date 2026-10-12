"""Android framework side: AAssetManager over the APK, Cocos2dxHelper & friends (Java), UserDefault."""
import json
import struct
import zipfile

from .. import paths
from .core import GuestFault
from .jni import JAVA, JObject, STATIC_FIELDS, java
from .libc import FS_ROOT, PACKAGE, imp, H

APK = paths.apk()
APK_DEVICE_PATH = f"/data/app/{PACKAGE}-1/base.apk"
FILES_DIR = f"/data/data/{PACKAGE}/files/"
PREFS = FS_ROOT / f"data/data/{PACKAGE}/shared_prefs/Cocos2dxPrefsFile.json"
SCREEN_W, SCREEN_H = 1136, 640


class Android:
    def __init__(self, emu):
        self.emu = emu
        self.apk = zipfile.ZipFile(APK)
        self.assets = {}         # AAsset* -> [data, pos]
        self.video_events = []   # (video index, event) to deliver via nativeExecuteVideoCallback
        self.edit_dialog = None  # open Cocos2dxHelper.showEditTextDialog, answered by ctl text
        self.asset_mgr = emu.alloc(16)
        (FS_ROOT / FILES_DIR.lstrip("/")).mkdir(parents=True, exist_ok=True)
        PREFS.parent.mkdir(parents=True, exist_ok=True)
        self.prefs = json.loads(PREFS.read_text()) if PREFS.exists() else {}

    def save_prefs(self):
        PREFS.write_text(json.dumps(self.prefs, indent=1, ensure_ascii=False))

    def asset_data(self, name):
        try:
            return self.apk.read("assets/" + name.lstrip("/"))
        except KeyError:
            return None


def install(emu):
    emu.android = Android(emu)


# ---------------------------------------------------------------------------------------- AAsset (NDK)
@imp("AAssetManager_fromJava")
def _fromjava(e):
    e.ret(e.android.asset_mgr)


@imp("AAssetManager_open")
def _aopen(e):
    mgr, name, mode = e.args(3)
    n = e.cstr(name)
    data = e.android.asset_data(n)
    if data is None:
        e.log(f"  AAssetManager_open({n}) -> NULL")
        e.ret(0)
        return
    a = e.alloc(16)
    e.android.assets[a] = [data, 0]
    e.log(f"  AAssetManager_open({n}) -> {len(data)} bytes")
    e.ret(a)


@imp("AAsset_close")
def _aclose(e):
    e.android.assets.pop(e.args(1)[0], None)
    e.ret(0)


@imp("AAsset_getLength")
def _alen(e):
    e.ret(len(e.android.assets[e.args(1)[0]][0]))


@imp("AAsset_getRemainingLength")
def _arem(e):
    d, pos = e.android.assets[e.args(1)[0]]
    e.ret(len(d) - pos)


@imp("AAsset_read")
def _aread(e):
    a, buf, n = e.args(3)
    st = e.android.assets[a]
    chunk = st[0][st[1]:st[1] + n]
    st[1] += len(chunk)
    e.write(buf, chunk)
    e.ret(len(chunk))


@imp("AAsset_seek")
def _aseek(e):
    a, off, whence = e.args(3)
    off = off - (1 << 32) if off & 0x80000000 else off
    st = e.android.assets[a]
    st[1] = {0: off, 1: st[1] + off, 2: len(st[0]) + off}[whence]
    e.ret(st[1])


# ---------------------------------------------------------------------------------------- Java: cocos2d-x
HELPER = "org/cocos2dx/lib/Cocos2dxHelper"


@java(f"{HELPER}.getCocos2dxPackageName")
def _pkg(jni, this, a):
    return PACKAGE


@java(f"{HELPER}.getCocos2dxWritablePath")
def _writable(jni, this, a):
    return FILES_DIR.rstrip("/")


@java(f"{HELPER}.getCurrentLanguage")
def _lang(jni, this, a):
    return "en"


@java(f"{HELPER}.getDeviceModel")
def _model(jni, this, a):
    return "Android Emulator"


@java(f"{HELPER}.getDPI")
def _dpi(jni, this, a):
    return 320


def _pref_get(default_index=1):
    def fn(jni, this, a):
        return jni.emu.android.prefs.get(a[0], a[default_index] if len(a) > default_index else None)
    return fn


def _pref_set(jni, this, a):
    jni.emu.android.prefs[a[0]] = a[1]
    jni.emu.android.save_prefs()


for _t in ("Bool", "Integer", "Float", "Double", "String"):
    JAVA[f"{HELPER}.get{_t}ForKey"] = _pref_get()
    JAVA[f"{HELPER}.set{_t}ForKey"] = _pref_set


@java(f"{HELPER}.deleteValueForKey")
def _pref_del(jni, this, a):
    jni.emu.android.prefs.pop(a[0], None)
    jni.emu.android.save_prefs()


for _m in ("setKeepScreenOn", "vibrate", "preloadBackgroundMusic", "playBackgroundMusic", "stopBackgroundMusic",
           "pauseBackgroundMusic", "resumeBackgroundMusic", "rewindBackgroundMusic", "end", "pauseAllEffects",
           "resumeAllEffects", "stopAllEffects", "stopEffect", "pauseEffect", "resumeEffect", "unloadEffect",
           "preloadEffect", "setBackgroundMusicVolume", "setEffectsVolume", "enableAccelerometer",
           "disableAccelerometer", "setAccelerometerInterval", "terminateProcess", "setEditTextDialogResult"):
    JAVA[f"{HELPER}.{_m}"] = lambda jni, this, a: None

JAVA[f"{HELPER}.isBackgroundMusicPlaying"] = lambda jni, this, a: 0
JAVA[f"{HELPER}.getBackgroundMusicVolume"] = lambda jni, this, a: 1.0
JAVA[f"{HELPER}.getEffectsVolume"] = lambda jni, this, a: 1.0
JAVA[f"{HELPER}.playEffect"] = lambda jni, this, a: 1
JAVA[f"{HELPER}.openURL"] = lambda jni, this, a: 0
JAVA[f"{HELPER}.getVersion"] = lambda jni, this, a: "1.0.1"
JAVA[f"{HELPER}.getAssetsPath"] = lambda jni, this, a: APK_DEVICE_PATH


# ---------------------------------------------------------------------------------------- Java: framework
@java("android/content/Context.getClassLoader", "android/app/Activity.getClassLoader")
def _get_class_loader(jni, this, a):
    return JObject("java/lang/ClassLoader")


@java("java/lang/ClassLoader.loadClass")
def _load_class(jni, this, a):
    return jni.find_class(a[0].replace(".", "/"))


@java("android/content/Context.getApplicationInfo", "android/app/Activity.getApplicationInfo")
def _app_info(jni, this, a):
    return JObject("android/content/pm/ApplicationInfo", sourceDir=APK_DEVICE_PATH,
                   dataDir=f"/data/data/{PACKAGE}", packageName=PACKAGE,
                   nativeLibraryDir=f"/data/app/{PACKAGE}-1/lib/arm")


NATIVE_LIB_DIR = f"/data/app/{PACKAGE}-1/lib/arm"


@java("java/util/UUID.randomUUID")
def _uuid(jni, this, a):
    import uuid
    u = uuid.uuid4()
    return JObject("java/util/UUID", hi=u.int >> 64, lo=u.int & ((1 << 64) - 1))


@java("java/util/UUID.getMostSignificantBits")
def _uuid_hi(jni, this, a):
    v = this.fields["hi"]
    return v - (1 << 64) if v >> 63 else v


@java("java/util/UUID.getLeastSignificantBits")
def _uuid_lo(jni, this, a):
    v = this.fields["lo"]
    return v - (1 << 64) if v >> 63 else v


@java("java/util/UUID.toString")
def _uuid_str(jni, this, a):
    import uuid
    return str(uuid.UUID(int=(this.fields["hi"] << 64) | this.fields["lo"]))


# ---------------------------------------------------------------------------------------- Java: video
# No video decoding: playback reports COMPLETED on the next frame (run.py drains video_events).
VIDEO = "org/cocos2dx/lib/Cocos2dxVideoHelper"
VIDEO_COMPLETED = 3
_next_video = [0]


@java(f"{VIDEO}.createVideoWidget")
def _video_create(jni, this, a):
    _next_video[0] += 1
    return _next_video[0] - 1


@java(f"{VIDEO}.startVideo")
def _video_start(jni, this, a):
    jni.emu.android.video_events.append((a[0], VIDEO_COMPLETED))


for _m in ("setVideoUrl", "setVideoRect", "removeVideoWidget", "pauseVideo", "resumeVideo", "stopVideo",
           "seekVideoTo", "setVideoVisible", "setVideoKeepRatioEnabled", "setFullScreenEnabled"):
    JAVA[f"{VIDEO}.{_m}"] = lambda jni, this, a: None


# ---------------------------------------------------------------------------------------- Java: text input
@java(f"{HELPER}.showEditTextDialog")
def _edit_text_dialog(jni, this, a):
    # (title, byte[] message (current text, UTF-8), inputMode, inputFlag, returnType, maxLength)
    current = bytes(a[1].items).decode("utf-8", "replace") if a[1] else ""
    jni.emu.android.edit_dialog = {"current": current, "max": a[5]}
    jni.emu.log(f"  edit dialog opened (current={current!r}, max={a[5]}) - answer with: ctl text <s>")


@java("org/cocos2dx/lib/Cocos2dxBitmap.getFontSizeAccordingHeight")
def _font_size_for_height(jni, this, a):
    # Java searches for the Paint text size whose bounds height matches; ~glyph height ratio 0.75
    return max(1, int(a[0] * 0.75))
