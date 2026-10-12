"""Silent OpenSL ES: objects and interfaces that accept everything and play nothing.

An SLObjectItf / SLxxxItf in the guest is a pointer to a cell holding a vtable pointer. Every vtable
slot is a host function; the ones that write out-params are implemented, the rest return SUCCESS.
"""
import struct

from .libc import imp

SL_RESULT_SUCCESS = 0
VTABLE_SLOTS = 40

# method names per interface, in vtable order (only those we special-case need to be right)
METHODS = {
    "OBJECT": ["Realize", "Resume", "GetState", "GetInterface", "RegisterCallback", "AbortAsyncOperation",
               "Destroy", "SetPriority", "GetPriority", "SetLossOfControlInterfaces"],
    "ENGINE": ["CreateLEDDevice", "CreateVibraDevice", "CreateAudioPlayer", "CreateAudioRecorder",
               "CreateMidiPlayer", "CreateListener", "Create3DGroup", "CreateOutputMix",
               "CreateMetadataExtractor", "CreateExtensionObject", "QueryNumSupportedInterfaces",
               "QuerySupportedInterfaces", "QueryNumSupportedExtensions", "QuerySupportedExtension",
               "IsExtensionSupported"],
    "PLAY": ["SetPlayState", "GetPlayState", "GetDuration", "GetPosition", "RegisterCallback",
             "SetCallbackEventsMask", "GetCallbackEventsMask", "SetMarkerPosition", "ClearMarkerPosition",
             "GetMarkerPosition", "SetPositionUpdatePeriod", "GetPositionUpdatePeriod"],
    "ANDROIDSIMPLEBUFFERQUEUE": ["Enqueue", "Clear", "GetState", "RegisterCallback"],
    "BUFFERQUEUE": ["Enqueue", "Clear", "GetState", "RegisterCallback"],
    "VOLUME": ["SetVolumeLevel", "GetVolumeLevel", "GetMaxVolumeLevel", "SetMute", "GetMute",
               "EnableStereoPosition", "IsEnabledStereoPosition", "SetStereoPosition", "GetStereoPosition"],
}


class SLObject:
    def __init__(self, sl, kind):
        self.sl, self.kind = sl, kind
        self.state = 1          # SL_OBJECT_STATE_UNREALIZED
        self.play_state = 1     # SL_PLAYSTATE_STOPPED
        self.queued = 0
        self.itfs = {}
        self.cell = sl.make_cell("OBJECT", self)

    def interface(self, name):
        if name not in self.itfs:
            self.itfs[name] = self.sl.make_cell(name, self)
        return self.itfs[name]


class OpenSL:
    def __init__(self, emu):
        self.emu = emu
        self.vtables = {}
        self.owner = {}   # cell -> SLObject

    def make_cell(self, itf, obj):
        e = self.emu
        if itf not in self.vtables:
            vt = e.alloc(4 * VTABLE_SLOTS)
            names = METHODS.get(itf, [])
            for k in range(VTABLE_SLOTS):
                m = names[k] if k < len(names) else f"slot{k}"
                e.w32(vt + 4 * k, e.host_function(f"SL{itf}::{m}", self._method(itf, m)))
            self.vtables[itf] = vt
        cell = e.alloc(4, struct.pack("<I", self.vtables[itf]))
        self.owner[cell] = obj
        return cell

    def _method(self, itf, m):
        def fn(e):
            obj = self.owner.get(e.args(1)[0])
            h = getattr(self, f"{itf}_{m}", None) or getattr(self, f"ANY_{m}", None)
            r = h(e, obj) if h else SL_RESULT_SUCCESS
            e.ret(r or 0)
        return fn

    # object
    def OBJECT_Realize(self, e, obj):
        obj.state = 2   # SL_OBJECT_STATE_REALIZED

    def OBJECT_GetState(self, e, obj):
        e.w32(e.args(2)[1], obj.state)

    def OBJECT_GetInterface(self, e, obj):
        _, iid, out = e.args(3)
        name = getattr(e, "sl_iids", {}).get(iid, f"IID_{iid:#x}")
        e.w32(out, obj.interface(name))

    # engine
    def _new_object(self, e, out, kind):
        o = SLObject(self, kind)
        e.w32(out, o.cell)
        e.log(f"  OpenSL: created {kind}")

    def ENGINE_CreateAudioPlayer(self, e, obj):
        self._new_object(e, e.args(2)[1], "AudioPlayer")

    def ENGINE_CreateOutputMix(self, e, obj):
        self._new_object(e, e.args(2)[1], "OutputMix")

    def ENGINE_CreateAudioRecorder(self, e, obj):
        return 12  # SL_RESULT_FEATURE_UNSUPPORTED

    # play
    def PLAY_SetPlayState(self, e, obj):
        obj.play_state = e.args(2)[1]

    def PLAY_GetPlayState(self, e, obj):
        e.w32(e.args(2)[1], obj.play_state)

    def PLAY_GetPosition(self, e, obj):
        e.w32(e.args(2)[1], 0)

    def PLAY_GetDuration(self, e, obj):
        e.w32(e.args(2)[1], 0xFFFFFFFF)  # SL_TIME_UNKNOWN

    # buffer queue: data is dropped; state reports an empty queue
    def ANDROIDSIMPLEBUFFERQUEUE_GetState(self, e, obj):
        e.write(e.args(2)[1], struct.pack("<II", 0, obj.queued))

    BUFFERQUEUE_GetState = ANDROIDSIMPLEBUFFERQUEUE_GetState

    def ANDROIDSIMPLEBUFFERQUEUE_Enqueue(self, e, obj):
        obj.queued += 1

    BUFFERQUEUE_Enqueue = ANDROIDSIMPLEBUFFERQUEUE_Enqueue

    # volume
    def VOLUME_GetMaxVolumeLevel(self, e, obj):
        e.write(e.args(2)[1], struct.pack("<h", 0))

    def VOLUME_GetVolumeLevel(self, e, obj):
        e.write(e.args(2)[1], struct.pack("<h", 0))

    def VOLUME_GetMute(self, e, obj):
        e.w32(e.args(2)[1], 0)


def install(emu):
    emu.opensl = OpenSL(emu)


@imp("slCreateEngine")
def _sl_create_engine(e):
    out = e.args(1)[0]
    o = SLObject(e.opensl, "Engine")
    e.w32(out, o.cell)
    e.log("  slCreateEngine -> silent engine")
    e.ret(SL_RESULT_SUCCESS)
