"""Game <-> agent control channel.

The emulator reports what the game is doing and accepts input, so the game can be driven without
anybody watching the window:

  events  scene changes (Director run/push/replace/pop), API responses (APIManager::onRespond), and
          "idle" = the framebuffer has not changed for IDLE_FRAMES frames (the game waits for input)
  ui      the live cocos2d node tree of the running scene: visible ui::Widgets that are touch-enabled,
          with class, name, text and their rectangle in window pixels
  input   tap (window pixels, or by widget name / text), swipe

Exposed over HTTP on 127.0.0.1:CONTROL_PORT (see ctl.py for the client). Guest calls only happen on the
emulator's main thread: HTTP requests are queued and served between frames.
"""
import json
import queue
import re
import struct
import threading
import time
import zlib
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import parse_qs, urlparse

from unicorn.arm_const import UC_ARM_REG_LR, UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2

from .. import paths
from .core import GuestFault
from .libc import u2f

CONTROL_PORT = 8765
IDLE_FRAMES = 60          # unchanged frames before an "idle" event (~0.5 s at 120 fps)
HASH_EVERY = 6            # framebuffer hash interval (frames)
EVENTS_FILE = paths.EVENTS_FILE

A = {  # guest addresses (IDA names in comments)
    "Director_getInstance": 0x8EE518,
    "Director_runWithScene": 0x8F06CC,
    "Director_pushScene": 0x8F06FA,
    "Director_replaceScene": 0x8F071E,
    "Director_popScene": 0x8F0804,
    "Director_setOpenGLView": 0x8EF96E,
    "APIManager_onRespond": 0x4BAF7C,
    "Node_getChildren": 0x47AB26,           # const Vector<Node*>& getChildren() const
    "Node_isVisible": 0x893334,
    "Node_getContentSize": 0x89343C,        # const Size&
    "Node_getAnchorPoint": 0x893370,        # const Vec2&
    "Node_getName": 0x89354C,               # std::string by value (sret in r0, this in r1)
    "Node_getNodeToWorldTransform": 0x895818,  # Mat4 (sret)
    "Widget_isTouchEnabled": 0x826664,
    "Widget_isEnabled": 0x8267DA,
    "Button_getTitleText": 0x837AA0,        # const std::string&
    "Text_getString": 0x83A33A,             # const std::string&
    "TextAtlas_getString": 0x83AF7E,        # const std::string& (via its Label)
    "TextBMFont_getString": 0x83B48A,       # const std::string& (member at +868)
    "GLView_getViewPortRect": 0x8D4550,     # const Rect&
    "GLView_getScaleX": 0x8D4554,
    "GLView_getScaleY": 0x8D4558,
}
MASTER_SAVE = 0x76D81C      # MasterDataManager::save
RESOURCE_SET_REVISION = 0x4EABEE   # AppState::setResourceRevisionAndSize(this, revision, size)
MASTER_TABLES = 0x1004100   # table registry: 14 words per table, word 0 = name
WIDGET_TI = "N7cocos2d2ui6WidgetE"
BUTTON_TI = "N7cocos2d2ui6ButtonE"
TEXT_TI = "N7cocos2d2ui4TextE"


def demangle_type(name):
    """'N7cocos2d2ui6ButtonE' -> 'cocos2d::ui::Button', '10SceneTitle' -> 'SceneTitle'."""
    parts = []
    s = name[1:-1] if name.startswith("N") and name.endswith("E") else name
    while s:
        m = re.match(r"(\d+)", s)
        if not m:
            parts.append(s)
            break
        n = int(m.group(1))
        start = len(m.group(1))
        parts.append(s[start:start + n])
        s = s[start + n:]
    return "::".join(parts)


class Control:
    def __init__(self, emu, jni, renderer):
        self.emu, self.jni, self.renderer = emu, jni, renderer
        self.frame = 0
        self.scene = None
        self.glview = 0
        self.events = []
        self.lock = threading.Lock()
        self.cv = threading.Condition(self.lock)
        self.commands = queue.Queue()
        self.pending_input = []     # (frame, kind, x, y)
        self.last_hash = None
        self.unchanged = 0
        self.idle_reported = False
        self.ti_cache = {}
        EVENTS_FILE.write_text("")
        # IDA addresses have no Thumb bit; take it from the dynamic symbol table so guest calls run
        # in the right mode
        thumb = {v & ~1: v for v in emu.symbols.values()}
        self.fn = {k: thumb.get(v, v) for k, v in A.items()}
        self._install_hooks()
        self.server = ThreadingHTTPServer(("127.0.0.1", CONTROL_PORT), self._handler_class())
        threading.Thread(target=self.server.serve_forever, daemon=True).start()
        emu.log(f"control: http://127.0.0.1:{CONTROL_PORT}  (python -m recomp.emu.ctl ...)")

    # ------------------------------------------------------------------ events
    def event(self, kind, **data):
        with self.cv:
            ev = {"seq": len(self.events), "frame": self.frame, "t": round(time.time(), 3), "kind": kind, **data}
            self.events.append(ev)
            self.cv.notify_all()
        with open(EVENTS_FILE, "a", encoding="utf-8") as f:
            f.write(json.dumps(ev) + "\n")
        self.emu.log(f">>> {kind} {json.dumps(data)}")

    def _install_hooks(self):
        e = self.emu

        def scene_hook(how):
            def cb(emu):
                scene = emu.reg(UC_ARM_REG_R1)
                # screens are plain cocos2d::Scene wrappers; the game class is the child layer
                layers = [self.class_name(c) for c in self.children(scene)]
                name = next((n for n in layers if not n.startswith("cocos2d::")), None) or self.class_name(scene)
                self.scene = name
                self.unchanged = 0
                self.idle_reported = False
                self.event("scene", how=how, name=name, layers=layers)
            return cb
        for how in ("runWithScene", "pushScene", "replaceScene"):
            e.on(A[f"Director_{how}"], scene_hook(how))
        e.on(A["Director_popScene"], lambda emu: self.event("scene", how="popScene", name=None))
        e.on(A["Director_setOpenGLView"], lambda emu: setattr(self, "glview", emu.reg(UC_ARM_REG_R1)))
        e.on(A["APIManager_onRespond"], lambda emu: self.event(
            "api", action=emu.reg(UC_ARM_REG_R1), status=emu.reg(UC_ARM_REG_R2)))
        self._hook_master_save()
        # resource download bookkeeping: the revision is only saved when finishDownload fully succeeds
        e.on(RESOURCE_SET_REVISION, lambda emu: self.event(
            "resource_revision_saved", revision=emu.reg(UC_ARM_REG_R1), size=emu.reg(UC_ARM_REG_R2)))

    def _hook_master_save(self):
        """MasterDataManager::save(this, TableInfo*, rapidjson doc) converts the server's JSON rows for one
        table into the binary m-file. Report the table, row count and result; on failure the game stores
        a reason in this+4: 3 = a row failed its master::X parser, 2 = file write / TableFile::init."""
        e = self.emu
        pending = {}
        return_hooks = set()

        def on_return(emu):
            st = pending.pop(self.emu.sched.current.tid, None)
            if not st:
                return
            ok = emu.reg(UC_ARM_REG_R0) & 0xFF
            reason = emu.u32(st["mgr"] + 4)
            self.event("master_save", table=st["table"], index=st["index"], rows=st["rows"], ok=bool(ok),
                       **({} if ok else {"reason": {3: "row parse failed", 2: "write/TableFile::init failed"}
                                         .get(reason, reason)}))

        def on_entry(emu):
            mgr, info, doc = emu.reg(UC_ARM_REG_R0), emu.reg(UC_ARM_REG_R1), emu.reg(UC_ARM_REG_R2)
            idx = emu.u32(info)
            name = emu.cstr(emu.u32(MASTER_TABLES + 56 * idx)) if idx < 64 else "?"
            rows = emu.u32(doc + 4)    # rapidjson array size (doc is the rows array value)
            pending[self.emu.sched.current.tid] = {"mgr": mgr, "index": idx, "table": name, "rows": rows}
            lr = emu.reg(UC_ARM_REG_LR) & ~1
            if lr not in return_hooks:
                return_hooks.add(lr)
                emu.on(lr, on_return)
        e.on(MASTER_SAVE, on_entry)

        import os
        # KHUX_TRACE_FUNCS=0x532160,0x...: log each call's r0-r3 and the value it returns (debugging aid)
        for a in filter(None, os.environ.get("KHUX_TRACE_FUNCS", "").split(",")):
            fa = int(a, 16) & ~1

            def f_entry(emu, fa=fa):
                lr = emu.reg(UC_ARM_REG_LR) & ~1
                emu.log(f"-> {fa:#x}({', '.join(hex(emu.reg(r)) for r in (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2))}) lr={lr:#x}")
                if ("fn", fa, lr) not in return_hooks:
                    return_hooks.add(("fn", fa, lr))
                    emu.on(lr, lambda emu2, fa=fa: emu2.log(f"<- {fa:#x} = {emu2.reg(UC_ARM_REG_R0):#x}"))
            e.on(fa, f_entry)
            e.log(f"KHUX_TRACE_FUNCS: hooked {fa:#x}")

        # KHUX_TRACE_PARSE=DrawMedalType,Stage: report which row (0-based, in JSON order) a table's
        # master::X::parse rejects
        for cls in filter(None, os.environ.get("KHUX_TRACE_PARSE", "").split(",")):
            prefix = f"_ZN6master{len(cls)}{cls}5parseE"
            addr = next((a for n, a in e.symbols.items() if n.startswith(prefix)), None)
            if not addr:
                e.log(f"KHUX_TRACE_PARSE: no parse function for {cls}")
                continue
            calls = {"n": 0}

            def parse_entry(emu, cls=cls, calls=calls):
                calls["n"] += 1
                row = calls["n"] - 1
                lr = emu.reg(UC_ARM_REG_LR) & ~1
                key = (cls, lr)
                if key not in return_hooks:
                    return_hooks.add(key)

                    def parse_ret(emu2, cls=cls, calls=calls):
                        if emu2.reg(UC_ARM_REG_R0) == 0:      # parse returns the row object, NULL on failure
                            self.event("parse_fail", table=cls, row=calls["n"] - 1)
                            emu2.log(f"!! master::{cls}::parse rejected row {calls['n'] - 1}")
                    emu.on(lr, parse_ret)
            e.on(addr & ~1, parse_entry)
            e.log(f"KHUX_TRACE_PARSE: hooked master::{cls}::parse at {addr:#x}")

    # ------------------------------------------------------------------ RTTI
    def _typeinfo(self, obj):
        vt = self.emu.u32(obj)
        return self.emu.u32(vt - 4)

    def _ti_name(self, ti):
        return self.emu.cstr(self.emu.u32(ti + 4))

    def _ti_bases(self, ti):
        """Base typeinfos of a typeinfo (handles __si_class_type_info and __vmi_class_type_info)."""
        if ti in self.ti_cache:
            return self.ti_cache[ti]
        e = self.emu
        bases = []
        try:
            # the typeinfo object's own dynamic type tells its layout:
            #   __si_class_type_info  = {vptr, name, base*}
            #   __vmi_class_type_info = {vptr, name, flags, count, {base*, offset_flags}[count]}
            kind = self._ti_name(self._typeinfo(ti))
            if "__si_class_type_info" in kind:
                bases = [e.u32(ti + 8)]
            elif "__vmi_class_type_info" in kind:
                bases = [e.u32(ti + 16 + 8 * k) for k in range(e.u32(ti + 12))]
        except Exception:
            bases = []
        self.ti_cache[ti] = bases
        return bases

    def is_a(self, obj, mangled):
        try:
            stack, seen = [self._typeinfo(obj)], set()
            while stack:
                ti = stack.pop()
                if ti in seen or ti < 0x1000:
                    continue
                seen.add(ti)
                if self._ti_name(ti) == mangled:
                    return True
                stack.extend(self._ti_bases(ti))
        except Exception:
            return False
        return False

    def class_name(self, obj):
        try:
            return demangle_type(self._ti_name(self._typeinfo(obj)))
        except Exception:
            return f"<{obj:#x}>"

    # ------------------------------------------------------------------ guest introspection
    NODE_CHILDREN = 0x180   # cocos2d::Node::_children (Vector<Node*>), from Node::getChildren

    def children(self, node):
        """Direct memory read (safe inside code hooks, unlike guest calls)."""
        e = self.emu
        begin, end = e.u32(node + self.NODE_CHILDREN), e.u32(node + self.NODE_CHILDREN + 4)
        return [e.u32(begin + 4 * k) for k in range(min((end - begin) // 4, 500))]

    def running_scene(self):
        d = self.emu.call(self.fn["Director_getInstance"])
        begin, end = self.emu.u32(d + 236), self.emu.u32(d + 240)
        return self.emu.u32(end - 4) if end > begin else 0

    def node_name(self, node):
        """Node::getName() returns std::string by value: sret buffer in r0, this in r1."""
        e = self.emu
        out = e.alloc(12)
        e.call(self.fn["Node_getName"], out, node)
        name = e.std_string(out)     # a long name's heap copy is leaked (tiny, debug-only)
        e.heap.free(out)
        return name

    def textdata(self, tid, ttype=0):
        """TextData::get(Type, id): the game's localized text for a text id (std::shared_ptr result in r0;
        the entry's char* is at +4). Used to name reward types, item categories etc."""
        e = self.emu
        out = e.alloc(8)
        e.call(e.symbols["_ZN8TextData3getENS_4TypeEi"], out, ttype, tid)
        entry = e.u32(out)
        s = e.cstr(e.u32(entry + 4)) if entry else None     # shared_ptr ref leaked (debug-only)
        e.heap.free(out)
        return s

    def screen_transform(self):
        """design (world) point -> window pixel (origin top-left)."""
        e = self.emu
        if not self.glview:
            return lambda x, y: (x, e.gl.height - y)
        sx = u2f(e.call(self.fn["GLView_getScaleX"], self.glview))
        sy = u2f(e.call(self.fn["GLView_getScaleY"], self.glview))
        r = struct.unpack("<4f", e.read(e.call(self.fn["GLView_getViewPortRect"], self.glview), 16))
        return lambda x, y: (r[0] + x * sx, e.gl.height - (r[1] + y * sy))

    def node_rect(self, node, to_screen):
        e = self.emu
        w, h = struct.unpack("<2f", e.read(e.call(self.fn["Node_getContentSize"], node), 8))
        out = e.heap.malloc(64)
        e.call(self.fn["Node_getNodeToWorldTransform"], out, node)
        m = struct.unpack("<16f", e.read(out, 64))
        e.heap.free(out)
        pts = []
        for x, y in ((0, 0), (w, 0), (0, h), (w, h)):
            wx = m[0] * x + m[4] * y + m[12]
            wy = m[1] * x + m[5] * y + m[13]
            pts.append(to_screen(wx, wy))
        xs, ys = [p[0] for p in pts], [p[1] for p in pts]
        return [round(min(xs)), round(min(ys)), round(max(xs)), round(max(ys))]

    def ui(self, all_nodes=False):
        """Visible widgets of the running scene, touchable ones flagged."""
        e = self.emu
        scene = self.running_scene()
        if not scene:
            return {"scene": None, "widgets": []}
        to_screen = self.screen_transform()
        out = []

        def walk(node, depth, path):
            if not (e.call(self.fn["Node_isVisible"], node) & 0xFF):
                return
            name = self.node_name(node)
            cls = self.class_name(node)
            p = f"{path}/{name or cls.split('::')[-1]}"
            if self.is_a(node, WIDGET_TI):
                touch = bool(e.call(self.fn["Widget_isTouchEnabled"], node) & 0xFF)
                enabled = bool(e.call(self.fn["Widget_isEnabled"], node) & 0xFF)
                text = None
                if self.is_a(node, BUTTON_TI):
                    text = e.std_string(e.call(self.fn["Button_getTitleText"], node))
                elif self.is_a(node, TEXT_TI):
                    text = e.std_string(e.call(self.fn["Text_getString"], node))
                elif cls.endswith("::TextAtlas"):
                    text = e.std_string(e.call(self.fn["TextAtlas_getString"], node))
                elif cls.endswith("::TextBMFont"):
                    text = e.std_string(e.call(self.fn["TextBMFont_getString"], node))
                if touch or text or all_nodes:
                    out.append({"path": p, "class": cls, "name": name, "text": text or None,
                                "touch": touch and enabled, "rect": self.node_rect(node, to_screen),
                                "addr": hex(node)})
            elif all_nodes:
                out.append({"path": p, "class": cls, "name": name, "touch": False,
                            "rect": self.node_rect(node, to_screen), "addr": hex(node)})
            vec = e.call(self.fn["Node_getChildren"], node)
            begin, end = e.u32(vec), e.u32(vec + 4)
            for k in range(min((end - begin) // 4, 500)):
                walk(e.u32(begin + 4 * k), depth + 1, p)
        walk(scene, 0, "")
        # buttons carry their caption in a child Text: expose it as "label"
        for w in out:
            if w["class"].endswith("::Button") and not w.get("text"):
                kids = [c["text"] for c in out if c["path"].startswith(w["path"] + "/") and c.get("text")]
                w["label"] = " / ".join(kids) or None
        return {"scene": self.class_name(scene), "screen": self.screen_layers(scene), "widgets": out}

    def screen_layers(self, scene=None):
        """Game classes (non-cocos2d) in the top three levels of the running scene: the 'screen'."""
        scene = scene or self.running_scene()
        names, level = [], [scene]
        for _ in range(3):
            nxt = []
            for n in level:
                for c in self.children(n):
                    cls = self.class_name(c)
                    if not cls.startswith("cocos2d::") and cls not in names:
                        names.append(cls)
                    nxt.append(c)
            level = nxt[:200]
        return names

    # ------------------------------------------------------------------ per-frame
    def on_frame(self, frame):
        """Called by the main loop after each rendered frame (before swap)."""
        self.frame = frame
        if frame % 30 == 0:
            try:
                layers = self.screen_layers()
            except Exception:
                layers = None
            if layers is not None and layers != getattr(self, "layers", None):
                self.layers = layers
                self.unchanged = 0
                self.idle_reported = False
                self.event("screen", layers=layers)
        if frame % HASH_EVERY == 0:
            from . import gles
            import ctypes as C
            w, h = self.emu.gl.width, self.emu.gl.height
            buf = C.create_string_buffer(w * h * 4)
            gles.gl.glReadPixels(0, 0, w, h, 0x1908, 0x1401, buf)
            hsh = zlib.crc32(buf.raw)
            if hsh == self.last_hash:
                self.unchanged += HASH_EVERY
            else:
                self.unchanged = 0
                self.idle_reported = False
            self.last_hash = hsh
            if self.unchanged >= IDLE_FRAMES and not self.idle_reported:
                self.idle_reported = True
                self.event("idle", scene=self.scene)
        while True:
            try:
                fn, box = self.commands.get_nowait()
            except queue.Empty:
                break
            try:
                box["result"] = fn()
            except GuestFault as ex:
                box["result"] = {"error": str(ex)}
            except Exception as ex:
                box["result"] = {"error": repr(ex)}
            box["done"].set()

    def touch_for_frame(self, frame):
        """Injected touch state for this frame: None (no injection) or (down, x, y)."""
        while self.pending_input and self.pending_input[0][0] < frame:
            self.pending_input.pop(0)
        if self.pending_input and self.pending_input[0][0] == frame:
            _, down, x, y = self.pending_input.pop(0)
            return down, x, y
        return None

    def tap(self, x, y, hold=3):
        start = self.frame + 2
        for k in range(hold):
            self.pending_input.append((start + k, True, x, y))
        self.pending_input.append((start + hold, False, x, y))
        self.unchanged = 0
        self.idle_reported = False
        return {"tap": [x, y], "frame": start}

    def swipe(self, x0, y0, x1, y1, frames=12, hold=0):
        """Drag from (x0,y0) to (x1,y1) over `frames` frames, after holding still at the start for `hold`
        frames (long-press drags, e.g. medals out of the deck-edit list) and at the end for `hold` frames."""
        start = self.frame + 2
        for k in range(hold):
            self.pending_input.append((start + k, True, x0, y0))
        start += hold
        for k in range(frames + 1):
            t = k / frames
            self.pending_input.append((start + k, True, x0 + (x1 - x0) * t, y0 + (y1 - y0) * t))
        start += frames + 1
        for k in range(hold):
            self.pending_input.append((start + k, True, x1, y1))
        self.pending_input.append((start + hold, False, x1, y1))
        self.unchanged = 0
        self.idle_reported = False
        return {"swipe": [x0, y0, x1, y1], "frames": frames, "hold": hold}

    def type_text(self, s, enter=True):
        """Text input. If the game opened an edit dialog (ui::EditBox -> showEditTextDialog), answer it the way
        Android's dialog does on Done (nativeSetEditTextDialogResult(byte[] utf8)); otherwise type into the
        focused TextField via the IME path (nativeInsertText)."""
        e = self.emu
        if e.android.edit_dialog is not None:
            from .jni import JArray
            e.android.edit_dialog = None
            fn = e.symbols["Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetEditTextDialogResult"]
            helper = self.jni.new_ref(self.jni.find_class("org/cocos2dx/lib/Cocos2dxHelper"))
            e.call(fn, self.jni.env, helper, self.jni.new_ref(JArray("B", bytearray(s.encode("utf-8")))))
            return {"text": s, "via": "edit dialog"}
        fn = e.symbols["Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInsertText"]
        e.call(fn, self.jni.env, self.renderer, self.jni.new_ref(s))
        if enter:
            e.call(fn, self.jni.env, self.renderer, self.jni.new_ref("\n"))
        return {"text": s, "enter": enter}

    def state(self):
        return {"frame": self.frame, "scene": self.scene, "screen": getattr(self, "layers", None),
                "unchanged_frames": self.unchanged,
                "idle": self.idle_reported, "events": len(self.events), "threads": self.emu.sched.status(),
                "window": [self.emu.gl.width, self.emu.gl.height]}

    # ------------------------------------------------------------------ HTTP
    def run_on_main(self, fn, timeout=10):
        box = {"done": threading.Event()}
        self.commands.put((fn, box))
        if not box["done"].wait(timeout):
            return {"error": "timeout waiting for the emulator main loop"}
        return box["result"]

    def wait(self, cond, since, timeout):
        """Block until an event matching cond (e.g. 'idle', 'scene', 'scene=SceneTitle', 'api=62')."""
        kind, _, val = cond.partition("=")
        deadline = time.time() + timeout
        with self.cv:
            while True:
                for ev in self.events[since:]:
                    if ev["kind"] != kind:
                        continue
                    if not val or str(ev.get("name")) == val or str(ev.get("action")) == val \
                            or val in (ev.get("layers") or []):
                        return ev
                left = deadline - time.time()
                if left <= 0:
                    return {"timeout": True, "state": {"frame": self.frame, "scene": self.scene}}
                self.cv.wait(left)

    def _find_widget(self, name=None, text=None):
        for w in self.ui()["widgets"]:
            if not w["touch"]:
                continue
            caption = (w.get("text") or "") + " " + (w.get("label") or "")
            if (name and w["name"] == name) or (text and text.lower() in caption.lower()):
                return w
        return None

    def _handler_class(self):
        ctl = self

        class Handler(BaseHTTPRequestHandler):
            def log_message(self, *a):
                pass

            def _send(self, obj):
                body = json.dumps(obj, ensure_ascii=False, indent=1).encode()
                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                self.wfile.write(body)

            def do_GET(self):
                u = urlparse(self.path)
                q = {k: v[0] for k, v in parse_qs(u.query).items()}
                p = u.path.strip("/")
                if p == "state":
                    self._send(ctl.state())
                elif p == "events":
                    since = int(q.get("since", 0))
                    with ctl.lock:
                        self._send(ctl.events[since:])
                elif p == "wait":
                    self._send(ctl.wait(q.get("for", "idle"), int(q.get("since", len(ctl.events))),
                                        float(q.get("timeout", 60))))
                elif p == "ui":
                    self._send(ctl.run_on_main(lambda: ctl.ui(all_nodes=q.get("all") == "1")))
                elif p == "tap":
                    if "x" in q:
                        x, y = float(q["x"]), float(q["y"])
                        self._send(ctl.run_on_main(lambda: ctl.tap(x, y)))
                    else:
                        def by_widget():
                            w = ctl._find_widget(q.get("name"), q.get("text"))
                            if not w:
                                return {"error": "no touchable widget matches", "query": q}
                            r = w["rect"]
                            res = ctl.tap((r[0] + r[2]) / 2, (r[1] + r[3]) / 2)
                            res["widget"] = w
                            return res
                        self._send(ctl.run_on_main(by_widget))
                elif p == "text":
                    s, enter = q.get("s", ""), q.get("enter", "1") == "1"
                    self._send(ctl.run_on_main(lambda: ctl.type_text(s, enter)))
                elif p == "peek":
                    # peek?addr=0x..&n=64 -> words; peek?addr=..&call=0xFN calls fn(addr) and returns r0
                    addr, n = int(q["addr"], 16), int(q.get("n", 64))
                    if "call" in q:
                        fn = int(q["call"], 16)
                        self._send(ctl.run_on_main(lambda: {"r0": hex(ctl.emu.call(fn, addr))}))
                    else:
                        self._send(ctl.run_on_main(lambda: {"addr": hex(addr), "words": [
                            hex(w) for w in struct.unpack(f"<{n // 4}I", ctl.emu.read(addr, n // 4 * 4))]}))
                elif p == "warp":
                    # DEV/TEST HARNESS ONLY: warp?avatar=0x..&to=0x.. puts the player next to another map actor
                    # (e.g. an EnemyActor behind terrain the blind battle driver cannot route round) through the
                    # game's own AvatarActor::setSefeMoveMapPotision (walkability-checked; map position lives at
                    # +624/+628 in both actor classes). Never part of the server.
                    avatar, to = int(q["avatar"], 16), int(q["to"], 16)

                    def warp():
                        e = ctl.emu
                        x, y = struct.unpack("<2f", e.read(to + 624, 8))
                        fn = e.symbols["_ZN11AvatarActor22setSefeMoveMapPotisionERN7cocos2d4Vec2E"]
                        v = e.alloc(8)
                        try:
                            for dx, dy in ((0, -80), (80, 0), (-80, 0), (0, 80), (0, -160), (160, 0), (-160, 0)):
                                e.write(v, struct.pack("<2f", x + dx, y + dy))
                                if e.call(fn, avatar, v) & 0xFF:
                                    return {"warped": [x + dx, y + dy]}
                            return {"error": "no walkable spot near target", "target": [x, y]}
                        finally:
                            e.heap.free(v)
                    self._send(ctl.run_on_main(warp))
                elif p == "textdata":
                    ids = [int(x) for x in q.get("id", "").split(",") if x]
                    tt = int(q.get("type", 0))
                    self._send(ctl.run_on_main(lambda: {str(i): ctl.textdata(i, tt) for i in ids}))
                elif p == "swipe":
                    v = [float(q[k]) for k in ("x0", "y0", "x1", "y1")]
                    fr, hold = int(q.get("frames", 12)), int(q.get("hold", 0))
                    self._send(ctl.run_on_main(lambda: ctl.swipe(*v, frames=fr, hold=hold)))
                else:
                    self._send({"error": f"unknown endpoint /{p}",
                                "endpoints": ["state", "events?since=", "wait?for=idle|scene[=X]|api[=N]",
                                              "ui[?all=1]", "tap?x=&y= | tap?name= | tap?text=",
                                              "swipe?x0=&y0=&x1=&y1=[&frames=&hold=]", "textdata?id=a,b[&type=0]"]})
        return Handler
