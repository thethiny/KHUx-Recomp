"""OpenGL ES 2.0 for the guest via ANGLE (libEGL/libGLESv2 from VS Code, copied to recomp/emu/angle/).

Plain-value GL calls are forwarded straight through ctypes. Calls that take pointers translate guest
memory to host buffers. Client-side vertex arrays (no VBO bound) are captured at glVertexAttribPointer
time and copied from guest memory at draw time.
"""
import ctypes as C
import os
import struct
from pathlib import Path

import glfw

from .core import GuestFault
from .libc import u2f, imp, H

ANGLE = Path(__file__).resolve().parent / "angle"

GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER = 0x8892, 0x8893
GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, GL_FLOAT = 0x1401, 0x1403, 0x1405, 0x1406
GL_BYTE, GL_SHORT, GL_FIXED = 0x1400, 0x1402, 0x140C
GL_UNPACK_ALIGNMENT, GL_PACK_ALIGNMENT = 0x0CF5, 0x0D05
TYPE_SIZE = {GL_BYTE: 1, GL_UNSIGNED_BYTE: 1, GL_SHORT: 2, GL_UNSIGNED_SHORT: 2, GL_UNSIGNED_INT: 4,
             GL_FLOAT: 4, GL_FIXED: 4}
FORMAT_COMPONENTS = {0x1906: 1, 0x1909: 1, 0x190A: 2, 0x1907: 3, 0x1908: 4, 0x80E1: 4}  # A, L, LA, RGB, RGBA, BGRA
PACKED_TYPES = {0x8033: 2, 0x8034: 2, 0x8363: 2}  # 4444, 5551, 565

EGL_DEFAULT_DISPLAY = 0
EGL_NONE, EGL_RED_SIZE, EGL_GREEN_SIZE, EGL_BLUE_SIZE, EGL_ALPHA_SIZE = 0x3038, 0x3024, 0x3023, 0x3022, 0x3021
EGL_DEPTH_SIZE, EGL_STENCIL_SIZE, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT = 0x3025, 0x3026, 0x3040, 4
EGL_CONTEXT_CLIENT_VERSION = 0x3098

gl = None   # ctypes CDLL of libGLESv2
egl = None


class GLState:
    def __init__(self, emu, width, height, title):
        self.emu = emu
        self.width, self.height = width, height
        self.array_buffer = 0
        self.vao = 0
        self.element_buffer = {0: 0}     # per VAO
        self.client_attribs = {}         # (vao, index) -> (size, type, norm, stride, guest ptr)
        self.buffer_sizes = {}
        self.mapped = {}                 # target -> (guest ptr, size, buffer)
        self.unpack_alignment = 4
        self.pack_alignment = 4
        self.strings = {}
        self.keep = []
        self.draw_calls = 0
        self._init_window(title)

    def _init_window(self, title):
        global gl, egl
        os.add_dll_directory(str(ANGLE))
        egl = C.CDLL(str(ANGLE / "libEGL.dll"))
        gl = C.CDLL(str(ANGLE / "libGLESv2.dll"))
        if not glfw.init():
            raise RuntimeError("glfw init failed")
        glfw.window_hint(glfw.CLIENT_API, glfw.NO_API)
        glfw.window_hint(glfw.RESIZABLE, False)
        self.window = glfw.create_window(self.width, self.height, title, None, None)
        hwnd = glfw.get_win32_window(self.window)

        egl.eglGetDisplay.restype = C.c_void_p
        egl.eglGetDisplay.argtypes = [C.c_void_p]
        egl.eglInitialize.argtypes = [C.c_void_p, C.c_void_p, C.c_void_p]
        egl.eglChooseConfig.argtypes = [C.c_void_p, C.c_void_p, C.c_void_p, C.c_int, C.c_void_p]
        egl.eglCreateWindowSurface.restype = C.c_void_p
        egl.eglCreateWindowSurface.argtypes = [C.c_void_p, C.c_void_p, C.c_void_p, C.c_void_p]
        egl.eglCreateContext.restype = C.c_void_p
        egl.eglCreateContext.argtypes = [C.c_void_p, C.c_void_p, C.c_void_p, C.c_void_p]
        egl.eglMakeCurrent.argtypes = [C.c_void_p, C.c_void_p, C.c_void_p, C.c_void_p]
        egl.eglSwapBuffers.argtypes = [C.c_void_p, C.c_void_p]
        egl.eglSwapInterval.argtypes = [C.c_void_p, C.c_int]

        self.display = egl.eglGetDisplay(EGL_DEFAULT_DISPLAY)
        major, minor = C.c_int(), C.c_int()
        if not egl.eglInitialize(self.display, C.byref(major), C.byref(minor)):
            raise RuntimeError("eglInitialize failed")
        attrs = (C.c_int * 15)(EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
                               EGL_DEPTH_SIZE, 24, EGL_STENCIL_SIZE, 8, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                               EGL_NONE)
        config, n = C.c_void_p(), C.c_int()
        egl.eglChooseConfig(self.display, attrs, C.byref(config), 1, C.byref(n))
        self.surface = egl.eglCreateWindowSurface(self.display, config, C.c_void_p(hwnd), None)
        ctx_attrs = (C.c_int * 3)(EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE)
        self.context = egl.eglCreateContext(self.display, config, None, ctx_attrs)
        if not egl.eglMakeCurrent(self.display, self.surface, self.surface, self.context):
            raise RuntimeError("eglMakeCurrent failed")
        egl.eglSwapInterval(self.display, 1)
        gl.glGetString.restype = C.c_char_p
        self.emu.log(f"GLES via ANGLE: {gl.glGetString(0x1F01).decode()} / {gl.glGetString(0x1F02).decode()}")

    def swap(self):
        egl.eglSwapBuffers(self.display, self.surface)

    def guest_string(self, s):
        if s not in self.strings:
            self.strings[s] = self.emu.alloc_str(s)
        return self.strings[s]


def install(emu, width, height, title="KHUx v1.0.1 (Unicorn)"):
    emu.gl = GLState(emu, width, height, title)
    for name, sig in SIMPLE.items():
        H[name] = _simple(name, sig)
    emu.handlers.update(H)


# ---------------------------------------------------------------------------------------- simple calls
# u = 32-bit int argument, f = float (soft-float: passed as bits in a core register)
SIMPLE = {
    "glActiveTexture": "u", "glAttachShader": "uu", "glBindFramebuffer": "uu", "glBindRenderbuffer": "uu",
    "glBindTexture": "uu", "glBlendEquation": "u", "glBlendEquationSeparate": "uu", "glBlendFunc": "uu",
    "glCheckFramebufferStatus": "u", "glClear": "u", "glClearColor": "ffff", "glClearDepthf": "f",
    "glClearStencil": "u", "glCompileShader": "u", "glCreateProgram": "", "glCreateShader": "u",
    "glCullFace": "u", "glDeleteProgram": "u", "glDeleteShader": "u", "glDepthFunc": "u", "glDepthMask": "u",
    "glDisable": "u", "glDisableVertexAttribArray": "u", "glEnable": "u", "glEnableVertexAttribArray": "u",
    "glFramebufferRenderbuffer": "uuuu", "glFramebufferTexture2D": "uuuuu", "glGenerateMipmap": "u",
    "glGetError": "", "glIsBuffer": "u", "glIsEnabled": "u", "glLinkProgram": "u", "glPolygonOffset": "ff",
    "glRenderbufferStorage": "uuuu", "glScissor": "uuuu", "glStencilFunc": "uuu", "glStencilMask": "u",
    "glStencilOp": "uuu", "glTexParameteri": "uuu", "glUniform1f": "uf", "glUniform1i": "uu",
    "glUniform2f": "uff", "glUniform2i": "uuu", "glUniform3f": "ufff", "glUniform3i": "uuuu",
    "glUniform4f": "uffff", "glUniform4i": "uuuuu", "glUseProgram": "u", "glViewport": "uuuu",
    "glIsVertexArrayOES": "u",
}


def _simple(name, sig):
    fn = None

    def handler(e):
        nonlocal fn
        if fn is None:
            fn = getattr(gl, name.replace("OES", "OES"))
            fn.argtypes = [C.c_float if c == "f" else C.c_uint32 for c in sig]
            fn.restype = C.c_uint32
        raw = e.args(len(sig))
        args = [u2f(v) if c == "f" else v for v, c in zip(raw, sig)]
        e.ret(fn(*args) or 0)
    return handler


def _fn(name, argtypes, restype=None):
    f = getattr(gl, name)
    f.argtypes = argtypes
    f.restype = restype
    return f


def _signed(v):
    return v - (1 << 32) if v & 0x80000000 else v


# ---------------------------------------------------------------------------------------- objects
def _gen(glname):
    def handler(e):
        n, p = e.args(2)
        buf = (C.c_uint32 * n)()
        _fn(glname, [C.c_int, C.c_void_p])(n, buf)
        e.write(p, bytes(buf))
        e.ret(0)
    return handler


def _delete(glname):
    def handler(e):
        n, p = e.args(2)
        ids = (C.c_uint32 * n).from_buffer_copy(e.read(p, 4 * n))
        _fn(glname, [C.c_int, C.c_void_p])(n, ids)
        e.ret(0)
    return handler


for _n in ("Buffers", "Textures", "Framebuffers", "Renderbuffers"):
    H[f"glGen{_n}"] = _gen(f"glGen{_n}")
    H[f"glDelete{_n}"] = _delete(f"glDelete{_n}")
H["glGenVertexArraysOES"] = _gen("glGenVertexArraysOES")
H["glDeleteVertexArraysOES"] = _delete("glDeleteVertexArraysOES")


@imp("glBindBuffer")
def _bind_buffer(e):
    target, buf = e.args(2)
    st = e.gl
    if target == GL_ARRAY_BUFFER:
        st.array_buffer = buf
    elif target == GL_ELEMENT_ARRAY_BUFFER:
        st.element_buffer[st.vao] = buf
    _fn("glBindBuffer", [C.c_uint32, C.c_uint32])(target, buf)
    e.ret(0)


@imp("glBindVertexArrayOES")
def _bind_vao(e):
    vao = e.args(1)[0]
    e.gl.vao = vao
    e.gl.element_buffer.setdefault(vao, 0)
    _fn("glBindVertexArrayOES", [C.c_uint32])(vao)
    e.ret(0)


@imp("glBufferData")
def _buffer_data(e):
    target, size, data, usage = e.args(4)
    host = e.read(data, size) if data else None
    _fn("glBufferData", [C.c_uint32, C.c_ssize_t, C.c_char_p, C.c_uint32])(target, size, host, usage)
    st = e.gl
    cur = st.array_buffer if target == GL_ARRAY_BUFFER else st.element_buffer.get(st.vao, 0)
    st.buffer_sizes[cur] = size
    e.ret(0)


@imp("glBufferSubData")
def _buffer_subdata(e):
    target, off, size, data = e.args(4)
    _fn("glBufferSubData", [C.c_uint32, C.c_ssize_t, C.c_ssize_t, C.c_char_p])(target, off, size, e.read(data, size))
    e.ret(0)


@imp("glMapBufferOES")
def _map_buffer(e):
    target, access = e.args(2)
    st = e.gl
    cur = st.array_buffer if target == GL_ARRAY_BUFFER else st.element_buffer.get(st.vao, 0)
    size = st.buffer_sizes.get(cur, 0)
    p = e.heap.malloc(max(size, 16))
    st.mapped[target] = (p, size)
    e.ret(p)


@imp("glUnmapBufferOES")
def _unmap_buffer(e):
    target = e.args(1)[0]
    p, size = e.gl.mapped.pop(target)
    if size:
        _fn("glBufferSubData", [C.c_uint32, C.c_ssize_t, C.c_ssize_t, C.c_char_p])(target, 0, size, e.read(p, size))
    e.heap.free(p)
    e.ret(1)


# ---------------------------------------------------------------------------------------- shaders / programs
@imp("glShaderSource")
def _shader_source(e):
    shader, count, strings, lengths = e.args(4)
    parts = []
    for k in range(count):
        sp = e.u32(strings + 4 * k)
        ln = _signed(e.u32(lengths + 4 * k)) if lengths else -1
        parts.append(e.read(sp, ln) if ln >= 0 else e.cbytes(sp))
    arr = (C.c_char_p * count)(*parts)
    lens = (C.c_int * count)(*[len(p) for p in parts])
    _fn("glShaderSource", [C.c_uint32, C.c_int, C.c_void_p, C.c_void_p])(shader, count, arr, lens)
    e.ret(0)


def _get_iv(glname):
    def handler(e):
        obj, pname, out = e.args(3)
        v = C.c_int()
        _fn(glname, [C.c_uint32, C.c_uint32, C.c_void_p])(obj, pname, C.byref(v))
        e.w32(out, v.value)
        e.ret(0)
    return handler


H["glGetShaderiv"] = _get_iv("glGetShaderiv")
H["glGetProgramiv"] = _get_iv("glGetProgramiv")


def _get_log(glname):
    def handler(e):
        obj, bufsize, plen, buf = e.args(4)
        out = C.create_string_buffer(max(bufsize, 1))
        ln = C.c_int()
        _fn(glname, [C.c_uint32, C.c_int, C.c_void_p, C.c_void_p])(obj, bufsize, C.byref(ln), out)
        if buf and bufsize:
            e.write(buf, out.raw[:ln.value] + b"\0")
        if plen:
            e.w32(plen, ln.value)
        if ln.value:
            e.log(f"  {glname}: {out.value.decode(errors='replace').strip()}")
        e.ret(0)
    return handler


H["glGetShaderInfoLog"] = _get_log("glGetShaderInfoLog")
H["glGetProgramInfoLog"] = _get_log("glGetProgramInfoLog")
H["glGetShaderSource"] = _get_log("glGetShaderSource")


@imp("glBindAttribLocation")
def _bind_attrib(e):
    prog, idx, name = e.args(3)
    _fn("glBindAttribLocation", [C.c_uint32, C.c_uint32, C.c_char_p])(prog, idx, e.cbytes(name))
    e.ret(0)


def _get_location(glname):
    def handler(e):
        prog, name = e.args(2)
        e.ret(_fn(glname, [C.c_uint32, C.c_char_p], C.c_int)(prog, e.cbytes(name)))
    return handler


H["glGetAttribLocation"] = _get_location("glGetAttribLocation")
H["glGetUniformLocation"] = _get_location("glGetUniformLocation")


def _get_active(glname):
    def handler(e):
        prog, idx, bufsize, plen, psize, ptype, name = e.args(7)
        ln, size, typ = C.c_int(), C.c_int(), C.c_uint32()
        out = C.create_string_buffer(max(bufsize, 1))
        _fn(glname, [C.c_uint32, C.c_uint32, C.c_int, C.c_void_p, C.c_void_p, C.c_void_p, C.c_void_p])(
            prog, idx, bufsize, C.byref(ln), C.byref(size), C.byref(typ), out)
        if plen:
            e.w32(plen, ln.value)
        e.w32(psize, size.value)
        e.w32(ptype, typ.value)
        e.write(name, out.raw[:ln.value] + b"\0")
        e.ret(0)
    return handler


H["glGetActiveAttrib"] = _get_active("glGetActiveAttrib")
H["glGetActiveUniform"] = _get_active("glGetActiveUniform")


# ---------------------------------------------------------------------------------------- uniforms
def _uniform_v(glname, comps, kind):
    def handler(e):
        loc, count, p = e.args(3)
        n = comps * count
        ctype = C.c_float if kind == "f" else C.c_int
        data = (ctype * n).from_buffer_copy(e.read(p, 4 * n)) if n else (ctype * 1)()
        _fn(glname, [C.c_int, C.c_int, C.c_void_p])(_signed(loc), count, data)
        e.ret(0)
    return handler


for _c in (1, 2, 3, 4):
    H[f"glUniform{_c}fv"] = _uniform_v(f"glUniform{_c}fv", _c, "f")
    H[f"glUniform{_c}iv"] = _uniform_v(f"glUniform{_c}iv", _c, "i")


def _uniform_matrix(glname, dim):
    def handler(e):
        loc, count, transpose, p = e.args(4)
        n = dim * dim * count
        data = (C.c_float * n).from_buffer_copy(e.read(p, 4 * n))
        _fn(glname, [C.c_int, C.c_int, C.c_uint8, C.c_void_p])(_signed(loc), count, transpose, data)
        e.ret(0)
    return handler


for _d in (2, 3, 4):
    H[f"glUniformMatrix{_d}fv"] = _uniform_matrix(f"glUniformMatrix{_d}fv", _d)


# ---------------------------------------------------------------------------------------- state queries
@imp("glGetString")
def _get_string(e):
    name = e.args(1)[0]
    s = gl.glGetString(name)
    if name == 0x1F03:  # GL_EXTENSIONS: advertise what we emulate
        s = (s or b"") + b" GL_OES_mapbuffer GL_OES_vertex_array_object"
    e.ret(e.gl.guest_string(s or b""))


def _get_v(glname, ctype, size):
    def handler(e):
        pname, out = e.args(2)
        buf = (ctype * 16)()
        _fn(glname, [C.c_uint32, C.c_void_p])(pname, buf)
        n = {0x0BA2: 4, 0x0C10: 4, 0x0C22: 4, 0x0B73: 2, 0x0D3A: 2, 0x846D: 2, 0x846E: 2}.get(pname, 1)
        e.write(out, bytes(buf)[:size * n])
        e.ret(0)
    return handler


H["glGetIntegerv"] = _get_v("glGetIntegerv", C.c_int, 4)
H["glGetFloatv"] = _get_v("glGetFloatv", C.c_float, 4)
H["glGetBooleanv"] = _get_v("glGetBooleanv", C.c_uint8, 1)


@imp("glPixelStorei")
def _pixel_store(e):
    pname, v = e.args(2)
    if pname == GL_UNPACK_ALIGNMENT:
        e.gl.unpack_alignment = v
    elif pname == GL_PACK_ALIGNMENT:
        e.gl.pack_alignment = v
    _fn("glPixelStorei", [C.c_uint32, C.c_int])(pname, v)
    e.ret(0)


# ---------------------------------------------------------------------------------------- textures
def _image_size(w, h, fmt, typ, align):
    bpp = PACKED_TYPES.get(typ) or FORMAT_COMPONENTS.get(fmt, 4) * TYPE_SIZE.get(typ, 1)
    row = (w * bpp + align - 1) // align * align
    return row * (h - 1) + w * bpp if h else 0


@imp("glTexImage2D")
def _tex_image(e):
    target, level, ifmt, w, h, border, fmt, typ, pixels = e.args(9)
    data = e.read(pixels, _image_size(w, h, fmt, typ, e.gl.unpack_alignment)) if pixels else None
    _fn("glTexImage2D", [C.c_uint32, C.c_int, C.c_int, C.c_int, C.c_int, C.c_int, C.c_uint32, C.c_uint32,
                         C.c_char_p])(target, level, ifmt, w, h, border, fmt, typ, data)
    e.ret(0)


@imp("glTexSubImage2D")
def _tex_subimage(e):
    target, level, x, y, w, h, fmt, typ, pixels = e.args(9)
    data = e.read(pixels, _image_size(w, h, fmt, typ, e.gl.unpack_alignment))
    _fn("glTexSubImage2D", [C.c_uint32, C.c_int, C.c_int, C.c_int, C.c_int, C.c_int, C.c_uint32, C.c_uint32,
                            C.c_char_p])(target, level, x, y, w, h, fmt, typ, data)
    e.ret(0)


@imp("glCompressedTexImage2D")
def _compressed_tex(e):
    target, level, ifmt, w, h, border, size, data = e.args(8)
    _fn("glCompressedTexImage2D", [C.c_uint32, C.c_int, C.c_uint32, C.c_int, C.c_int, C.c_int, C.c_int,
                                   C.c_char_p])(target, level, ifmt, w, h, border, size, e.read(data, size))
    e.ret(0)


@imp("glReadPixels")
def _read_pixels(e):
    x, y, w, h, fmt, typ, out = e.args(7)
    n = _image_size(w, h, fmt, typ, e.gl.pack_alignment)
    buf = C.create_string_buffer(n)
    _fn("glReadPixels", [C.c_int, C.c_int, C.c_int, C.c_int, C.c_uint32, C.c_uint32, C.c_void_p])(
        x, y, w, h, fmt, typ, buf)
    e.write(out, buf.raw)
    e.ret(0)


# ---------------------------------------------------------------------------------------- vertex arrays / draws
@imp("glVertexAttribPointer")
def _attrib_pointer(e):
    idx, size, typ, norm, stride, ptr = e.args(6)
    st = e.gl
    vap = _fn("glVertexAttribPointer", [C.c_uint32, C.c_int, C.c_uint32, C.c_uint8, C.c_int, C.c_void_p])
    if st.array_buffer:
        st.client_attribs.pop((st.vao, idx), None)
        vap(idx, size, typ, norm, stride, C.c_void_p(ptr))
    else:
        st.client_attribs[(st.vao, idx)] = (size, typ, norm, stride, ptr)
    e.ret(0)


def _upload_client_arrays(e, max_index):
    st = e.gl
    vap = _fn("glVertexAttribPointer", [C.c_uint32, C.c_int, C.c_uint32, C.c_uint8, C.c_int, C.c_void_p])
    keep = []
    for (vao, idx), (size, typ, norm, stride, ptr) in st.client_attribs.items():
        if vao != st.vao:
            continue
        elem = size * TYPE_SIZE.get(typ, 4)
        stride_eff = stride or elem
        n = stride_eff * max_index + elem
        buf = C.create_string_buffer(e.read(ptr, n), n)
        keep.append(buf)
        _fn("glBindBuffer", [C.c_uint32, C.c_uint32])(GL_ARRAY_BUFFER, 0)
        vap(idx, size, typ, norm, stride, buf)
    if keep:
        _fn("glBindBuffer", [C.c_uint32, C.c_uint32])(GL_ARRAY_BUFFER, st.array_buffer)
    return keep


@imp("glDrawArrays")
def _draw_arrays(e):
    mode, first, count = e.args(3)
    keep = _upload_client_arrays(e, first + count - 1) if count else []
    _fn("glDrawArrays", [C.c_uint32, C.c_int, C.c_int])(mode, first, count)
    e.gl.draw_calls += 1
    del keep
    e.ret(0)


@imp("glDrawElements")
def _draw_elements(e):
    mode, count, typ, indices = e.args(4)
    st = e.gl
    de = _fn("glDrawElements", [C.c_uint32, C.c_int, C.c_uint32, C.c_void_p])
    if st.element_buffer.get(st.vao, 0):
        if any(v == st.vao for v, _ in st.client_attribs):
            raise GuestFault("client vertex arrays with an index buffer: max index unknown")
        de(mode, count, typ, C.c_void_p(indices))
    else:
        isz = TYPE_SIZE[typ]
        raw = e.read(indices, count * isz)
        fmt = {1: "B", 2: "H", 4: "I"}[isz]
        idx = struct.unpack(f"<{count}{fmt}", raw)
        keep = _upload_client_arrays(e, max(idx) if idx else 0)
        ibuf = C.create_string_buffer(raw, len(raw))
        de(mode, count, typ, ibuf)
        del keep
    st.draw_calls += 1
    e.ret(0)


# ---------------------------------------------------------------------------------------- EGL
@imp("eglGetProcAddress")
def _egl_get_proc(e):
    name = e.cstr(e.args(1)[0])
    if name in H:
        e.ret(e.slot(name))
        e.handlers[name] = H[name]
    else:
        e.log(f"  eglGetProcAddress({name}) -> NULL")
        e.ret(0)
