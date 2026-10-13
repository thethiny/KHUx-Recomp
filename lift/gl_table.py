"""OpenGL ES 2.0 calls made natively from lifted code (and from the emulator's Python handlers), through ANGLE.

Each entry: name -> (return kind, argument kinds). Argument kinds:
  u  32-bit integer or enum         f  float (soft-float ABI: passed as bits in a core register)
  b  GLboolean                      z  GLsizeiptr / GLintptr (64-bit on the host)
  p  guest pointer, translated to host (NULL stays NULL)
Return kinds: v (void), u (integer), i (signed integer), b (GLboolean).

Guest memory is host-addressable, so pointer arguments need no copying: client-side vertex and index arrays are
passed to ANGLE as host pointers into guest memory. Calls with special handling (offset-or-pointer arguments,
buffer mapping, string arrays, glGetString) are written by hand in SPECIAL_C.
"""

GL = {
    # state, objects, plain values
    "glActiveTexture": ("v", "u"), "glAttachShader": ("v", "uu"), "glBindFramebuffer": ("v", "uu"),
    "glBindRenderbuffer": ("v", "uu"), "glBindTexture": ("v", "uu"), "glBlendEquation": ("v", "u"),
    "glBlendEquationSeparate": ("v", "uu"), "glBlendFunc": ("v", "uu"), "glCheckFramebufferStatus": ("u", "u"),
    "glClear": ("v", "u"), "glClearColor": ("v", "ffff"), "glClearDepthf": ("v", "f"), "glClearStencil": ("v", "u"),
    "glCompileShader": ("v", "u"), "glCreateProgram": ("u", ""), "glCreateShader": ("u", "u"),
    "glCullFace": ("v", "u"), "glDeleteProgram": ("v", "u"), "glDeleteShader": ("v", "u"),
    "glDepthFunc": ("v", "u"), "glDepthMask": ("v", "b"), "glDisable": ("v", "u"),
    "glDisableVertexAttribArray": ("v", "u"), "glEnable": ("v", "u"), "glEnableVertexAttribArray": ("v", "u"),
    "glFramebufferRenderbuffer": ("v", "uuuu"), "glFramebufferTexture2D": ("v", "uuuuu"),
    "glGenerateMipmap": ("v", "u"), "glGetError": ("u", ""), "glIsBuffer": ("b", "u"), "glIsEnabled": ("b", "u"),
    "glLinkProgram": ("v", "u"), "glPolygonOffset": ("v", "ff"), "glRenderbufferStorage": ("v", "uuuu"),
    "glScissor": ("v", "uuuu"), "glStencilFunc": ("v", "uuu"), "glStencilMask": ("v", "u"),
    "glStencilOp": ("v", "uuu"), "glTexParameteri": ("v", "uuu"), "glUniform1f": ("v", "uf"),
    "glUniform1i": ("v", "uu"), "glUniform2f": ("v", "uff"), "glUniform2i": ("v", "uuu"),
    "glUniform3f": ("v", "ufff"), "glUniform3i": ("v", "uuuu"), "glUniform4f": ("v", "uffff"),
    "glUniform4i": ("v", "uuuuu"), "glUseProgram": ("v", "u"), "glViewport": ("v", "uuuu"),
    "glIsVertexArrayOES": ("b", "u"), "glBindBuffer": ("v", "uu"), "glBindVertexArrayOES": ("v", "u"),
    "glPixelStorei": ("v", "uu"), "glDrawArrays": ("v", "uuu"),
    # pointer arguments
    "glGenBuffers": ("v", "up"), "glDeleteBuffers": ("v", "up"), "glGenTextures": ("v", "up"),
    "glDeleteTextures": ("v", "up"), "glGenFramebuffers": ("v", "up"), "glDeleteFramebuffers": ("v", "up"),
    "glGenRenderbuffers": ("v", "up"), "glDeleteRenderbuffers": ("v", "up"),
    "glGenVertexArraysOES": ("v", "up"), "glDeleteVertexArraysOES": ("v", "up"),
    "glBufferData": ("v", "uzpu"), "glBufferSubData": ("v", "uzzp"),
    "glGetShaderiv": ("v", "uup"), "glGetProgramiv": ("v", "uup"),
    "glGetShaderInfoLog": ("v", "uupp"), "glGetProgramInfoLog": ("v", "uupp"), "glGetShaderSource": ("v", "uupp"),
    "glBindAttribLocation": ("v", "uup"), "glGetAttribLocation": ("i", "up"), "glGetUniformLocation": ("i", "up"),
    "glGetActiveAttrib": ("v", "uuupppp"), "glGetActiveUniform": ("v", "uuupppp"),
    **{f"glUniform{n}{t}v": ("v", "uup") for n in (1, 2, 3, 4) for t in "fi"},
    **{f"glUniformMatrix{n}fv": ("v", "uubp") for n in (2, 3, 4)},
    "glGetIntegerv": ("v", "up"), "glGetFloatv": ("v", "up"), "glGetBooleanv": ("v", "up"),
    "glTexImage2D": ("v", "uuuuuuuup"), "glTexSubImage2D": ("v", "uuuuuuuup"),
    "glCompressedTexImage2D": ("v", "uuuuuuup"), "glReadPixels": ("v", "uuuuuup"),
    "glGetBufferParameteriv": ("v", "uup"),
}

# hand-written: (C body using ARG(k), GLF(name) and the helpers below)
SPECIAL = {
    # pointer is an offset into the bound GL_ARRAY_BUFFER, else a client array in guest memory
    "glVertexAttribPointer": ("v", "uuubup"),
    # indices: offset into the VAO's element buffer, else a client array
    "glDrawElements": ("v", "uuup"),
    "glShaderSource": ("v", "uupp"),
    "glGetString": ("u", "u"),
    "glMapBufferOES": ("u", "uu"),
    "glUnmapBufferOES": ("b", "u"),
}

CTYPE = {"u": "uint32_t", "i": "int32_t", "f": "float", "b": "uint8_t", "z": "intptr_t", "p": "void *"}
RTYPE = {"v": "void", "u": "uint32_t", "i": "int32_t", "b": "uint8_t"}


def names():
    return list(GL) + list(SPECIAL)


def _arg(kind, k):
    a = f"ARG({k})"
    if kind == "f":
        return f"F32({a})"
    if kind == "b":
        return f"(uint8_t){a}"
    if kind == "z":
        return f"(intptr_t)(int32_t){a}"
    if kind == "p":
        return f"GPTR({a})"
    if kind == "i":
        return f"(int32_t){a}"
    return a


def c_code(first_id):
    """C source: function pointer table, loader, and one `case id:` per call for native_by_id."""
    all_names = names()
    out = ["/* ---- OpenGL ES (generated from lift/gl_table.py) ---- */",
           "#define ARG(k) ((k) < 4 ? c->r[k] : R32(c->r[13] + 4u * (uint32_t)((k) - 4)))",
           "#define GPTR(a) ((a) ? (void *)HA(a) : (void *)0)",
           f"static void *gl_fn[{len(all_names)}];",
           "static int gl_ok;",
           "EXPORT uint32_t rt_gl_draws;",
           "EXPORT uint32_t rt_gl_strings[8];      /* guest copies for glGetString, set by the host */",
           "static const char *gl_names[] = {" + ", ".join(f'"{n}"' for n in all_names) + "};"]
    for k, n in enumerate(all_names):
        ret, args = GL.get(n) or SPECIAL[n]
        proto = ", ".join(CTYPE[a] for a in args) or "void"
        out.append(f"typedef {RTYPE[ret]} (__stdcall *T_{n})({proto});")
        out.append(f"#define GLF_{n} ((T_{n})gl_fn[{k}])")
    out.append("""
/* Load the entry points from the libGLESv2.dll already loaded by the host. Returns the number missing. */
EXPORT int rt_gl_init(void) {
    HMODULE m = GetModuleHandleA("libGLESv2.dll");
    int missing = 0;
    if (!m) return -1;
    for (int k = 0; k < (int)(sizeof(gl_names) / sizeof(gl_names[0])); k++) {
        gl_fn[k] = (void *)GetProcAddress(m, gl_names[k]);
        if (!gl_fn[k]) missing++;
    }
    gl_ok = missing == 0;
    return missing;
}

static uint32_t gl_mapped[2][2];   /* [array, element] -> {guest block, size} for glMapBufferOES */
""")
    cases = []
    for k, n in enumerate(all_names):
        cid = first_id + k
        if n in GL:
            ret, args = GL[n]
            call = f"GLF_{n}({', '.join(_arg(a, j) for j, a in enumerate(args))})"
            if ret == "v":
                body = f"{call};"
            elif ret == "b":
                body = f"c->r[0] = (uint32_t){call};"
            else:
                body = f"c->r[0] = (uint32_t){call};"
            if n == "glDrawArrays":
                body += " rt_gl_draws++;"
        else:
            body = SPECIAL_C[n].format(n=n)
        cases.append(f"    case {cid}: /* {n} */ if (!gl_ok || c->logging) return 0; {body} return 1;")
    return "\n".join(out), cases


SPECIAL_C = {
    "glVertexAttribPointer": (
        "{{ int32_t bound = 0; GLF_glGetIntegerv(0x8894u, &bound); "
        "GLF_{n}(ARG(0), ARG(1), ARG(2), (uint8_t)ARG(3), ARG(4), "
        "bound ? (void *)(uintptr_t)ARG(5) : GPTR(ARG(5))); }}"),
    "glDrawElements": (
        "{{ int32_t bound = 0; GLF_glGetIntegerv(0x8895u, &bound); "
        "GLF_{n}(ARG(0), ARG(1), ARG(2), bound ? (void *)(uintptr_t)ARG(3) : GPTR(ARG(3))); rt_gl_draws++; }}"),
    "glShaderSource": (
        "{{ uint32_t n = ARG(1), strs = ARG(2), lens = ARG(3); const char *s[64]; int32_t l[64]; "
        "if (n > 64) return 0; "
        "for (uint32_t k = 0; k < n; k++) {{ s[k] = (const char *)HA(R32(strs + 4u * k)); "
        "l[k] = lens ? (int32_t)R32(lens + 4u * k) : -1; if (l[k] < 0) l[k] = (int32_t)strlen(s[k]); }} "
        "GLF_{n}(ARG(0), n, (void *)s, (void *)l); }}"),
    # the host fills rt_gl_strings (guest copies; GL_EXTENSIONS with the emulated extensions added)
    "glGetString": (
        "{{ uint32_t k = ARG(0) - 0x1F00u; if (k >= 4 || !rt_gl_strings[k]) return 0; "
        "c->r[0] = rt_gl_strings[k]; }}"),
    "glMapBufferOES": (
        "{{ uint32_t t = ARG(0) == 0x8893u; int32_t size = 0; GLF_glGetBufferParameteriv(ARG(0), 0x8764u, &size); "
        "uint32_t p = rt_heap_ctl ? rt_heap_malloc(c, size > 16 ? (uint32_t)size : 16u) : 0; if (!p) return 0; "
        "gl_mapped[t][0] = p; gl_mapped[t][1] = (uint32_t)size; c->r[0] = p; }}"),
    "glUnmapBufferOES": (
        "{{ uint32_t t = ARG(0) == 0x8893u, p = gl_mapped[t][0], size = gl_mapped[t][1]; if (!p) return 0; "
        "if (size) GLF_glBufferSubData(ARG(0), 0, (intptr_t)size, HA(p)); "
        "rt_heap_free(c, p); gl_mapped[t][0] = 0; c->r[0] = 1; }}"),
}
