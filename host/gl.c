/* Window, input and OpenGL ES through ANGLE (libEGL / libGLESv2). The GL calls themselves run in lift.dll
 * (lift/gl_table.py); this file creates the context, hands lift.dll the GL strings and answers
 * eglGetProcAddress with import slots that lift.dll runs natively. */
#include "host.h"

typedef void *EGLDisplay, *EGLSurface, *EGLContext, *EGLConfig;
typedef EGLDisplay (__stdcall *PFN_GetDisplay)(void *);
typedef unsigned (__stdcall *PFN_Initialize)(EGLDisplay, int *, int *);
typedef unsigned (__stdcall *PFN_ChooseConfig)(EGLDisplay, const int *, EGLConfig *, int, int *);
typedef EGLSurface (__stdcall *PFN_CreateWindowSurface)(EGLDisplay, EGLConfig, void *, const int *);
typedef EGLContext (__stdcall *PFN_CreateContext)(EGLDisplay, EGLConfig, EGLContext, const int *);
typedef unsigned (__stdcall *PFN_MakeCurrent)(EGLDisplay, EGLSurface, EGLSurface, EGLContext);
typedef unsigned (__stdcall *PFN_SwapBuffers)(EGLDisplay, EGLSurface);
typedef unsigned (__stdcall *PFN_SwapInterval)(EGLDisplay, int);
typedef const char *(__stdcall *PFN_GetString)(unsigned);
typedef void (__stdcall *PFN_ReadPixels)(int, int, int, int, unsigned, unsigned, void *);

HWND gl_hwnd;
static EGLDisplay dpy;
static EGLSurface surf;
static PFN_SwapBuffers egl_swap;
PFN_ReadPixels gl_read_pixels;
static volatile int closed;

/* ---------------------------------------------------------------- touch queue (window thread = main thread) */
#define TQ 256
static Touch tq[TQ];
static int tq_head, tq_tail;
static int mouse_down;

void touch_push(int kind, float x, float y) {
    int next = (tq_tail + 1) % TQ;
    if (next == tq_head) return;
    tq[tq_tail].kind = kind;
    tq[tq_tail].x = x;
    tq[tq_tail].y = y;
    tq_tail = next;
}

int touch_pop(Touch *t) {
    if (tq_head == tq_tail) return 0;
    *t = tq[tq_head];
    tq_head = (tq_head + 1) % TQ;
    return 1;
}

static LRESULT CALLBACK wndproc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    float x = (float)(short)LOWORD(lp), y = (float)(short)HIWORD(lp);
    switch (msg) {
    case WM_LBUTTONDOWN: SetCapture(h); mouse_down = 1; touch_push(0, x, y); return 0;
    case WM_MOUSEMOVE: if (mouse_down) touch_push(1, x, y); return 0;
    case WM_LBUTTONUP: ReleaseCapture(); if (mouse_down) touch_push(2, x, y); mouse_down = 0; return 0;
    case WM_CLOSE: closed = 1; return 0;
    }
    return DefWindowProcA(h, msg, wp, lp);
}

int gl_pump(void) {
    MSG m;
    while (PeekMessageA(&m, NULL, 0, 0, PM_REMOVE)) {
        if (edit_box_message(&m)) continue;      /* Enter / Escape / Tab in the input box */
        TranslateMessage(&m);
        DispatchMessageA(&m);
    }
    return !closed;
}

static void *need(HMODULE m, const char *name) {
    void *p = (void *)GetProcAddress(m, name);
    if (!p) fatal("ANGLE: missing %s", name);
    return p;
}

int gl_window(int w, int h, const char *title, const char *angle_dir) {
    char exe[MAX_PATH], dir[MAX_PATH];
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    /* ANGLE next to the exe, else the given folder (the emulator's copy, recomp/emu/angle) */
    snprintf(dir, sizeof(dir), "%s", exe);
    *strrchr(dir, '\\') = 0;
    char egl_path[MAX_PATH + 32];
    snprintf(egl_path, sizeof(egl_path), "%s\\libEGL.dll", dir);
    if (GetFileAttributesA(egl_path) == INVALID_FILE_ATTRIBUTES) snprintf(dir, sizeof(dir), "%s", angle_dir);
    SetDllDirectoryA(dir);
    char path[MAX_PATH + 32];
    snprintf(path, sizeof(path), "%s\\libGLESv2.dll", dir);
    HMODULE gles = LoadLibraryA(path);
    snprintf(path, sizeof(path), "%s\\libEGL.dll", dir);
    HMODULE egl = LoadLibraryA(path);
    if (!gles || !egl) fatal("cannot load ANGLE from %s (libEGL.dll, libGLESv2.dll)", dir);

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = wndproc;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = "KHUxHost";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);
    RECT r = {0, 0, w, h};
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    AdjustWindowRect(&r, style, FALSE);
    gl_hwnd = CreateWindowA("KHUxHost", title, style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left,
                         r.bottom - r.top, NULL, NULL, wc.hInstance, NULL);
    if (!gl_hwnd) fatal("cannot create the window");

    PFN_GetDisplay get_display = need(egl, "eglGetDisplay");
    PFN_Initialize initialize = need(egl, "eglInitialize");
    PFN_ChooseConfig choose = need(egl, "eglChooseConfig");
    PFN_CreateWindowSurface create_surface = need(egl, "eglCreateWindowSurface");
    PFN_CreateContext create_context = need(egl, "eglCreateContext");
    PFN_MakeCurrent make_current = need(egl, "eglMakeCurrent");
    PFN_SwapInterval swap_interval = need(egl, "eglSwapInterval");
    egl_swap = need(egl, "eglSwapBuffers");
    dpy = get_display(NULL);
    int major, minor;
    if (!initialize(dpy, &major, &minor)) fatal("eglInitialize failed");
    const int attrs[] = {0x3024, 8, 0x3023, 8, 0x3022, 8, 0x3021, 8, 0x3025, 24, 0x3026, 8, 0x3040, 4, 0x3038};
    EGLConfig cfg;
    int n;
    choose(dpy, attrs, &cfg, 1, &n);
    surf = create_surface(dpy, cfg, gl_hwnd, NULL);
    const int ctx_attrs[] = {0x3098, 2, 0x3038};
    EGLContext ctx = create_context(dpy, cfg, NULL, ctx_attrs);
    if (!make_current(dpy, surf, surf, ctx)) fatal("eglMakeCurrent failed");
    swap_interval(dpy, 1);

    int missing = L.gl_init();
    if (missing) fatal("lift.dll: %d OpenGL ES entry points missing in ANGLE", missing);
    PFN_GetString get_string = need(gles, "glGetString");
    gl_read_pixels = need(gles, "glReadPixels");
    for (int k = 0; k < 4; k++) {
        const char *s = get_string(0x1F00u + (unsigned)k);
        char buf[8192];
        snprintf(buf, sizeof(buf), "%s%s", s ? s : "", k == 3 ? " GL_OES_mapbuffer GL_OES_vertex_array_object" : "");
        L.gl_strings[k] = galloc_str(buf);
    }
    hlog("GLES via ANGLE: %s / %s", get_string(0x1F01), get_string(0x1F02));
    return 0;
}

void gl_swap(void) { egl_swap(dpy, surf); }

/* eglGetProcAddress: a slot that lift.dll runs natively (every GL entry the game asks for is in gl_table) */
static int native_gl_id(const char *name) {
    for (int k = 0; k < L.native_count; k++) if (!strcmp(L.native_names[k], name)) return k;
    return -1;
}
static void i_egl_get_proc(CPU *c) {
    const char *name = gstr(c->r[0]);
    int id = native_gl_id(name);
    if (id < 0) { hlog("  eglGetProcAddress(%s) -> NULL", name); RET(c, 0); return; }
    uint32_t a = slot_for(name);
    L.native_slot(a, id);
    RET(c, a);
}

/* a GL import lift.dll declined: it only declines inside logged runs, which the host never makes */
static void i_gl_declined(CPU *c) { fatal("OpenGL ES call %s declined by lift.dll", import_name(imp_target)); }

void gl_init_imports(void) {
    imp("eglGetProcAddress", i_egl_get_proc);
    for (int k = 0; k < L.native_count; k++)
        if (!strncmp(L.native_names[k], "gl", 2)) imp(L.native_names[k], i_gl_declined);
}
