/* KHUx native host: runs the recompiled game (lift.dll) without the emulator.
 *
 * Guest memory is one flat 4 GB block (guest address a = host address G + a), laid out exactly as the emulator
 * lays it out: the library at its original addresses, import slots, heap, stacks. Every guest thread is a host
 * thread with its own CPU struct; lifted code runs on it directly. What the game imports (bionic libc, pthreads,
 * JNI and the Java side, Android assets, OpenGL ES / EGL, OpenSL ES, sockets) is implemented here in C, or in
 * lift.dll for the calls lifted code makes natively (memory/string functions, math, heap, stdio, OpenGL ES).
 *
 * Nothing falls back silently: an import without an implementation, or control reaching code that is not lifted,
 * stops the program with the guest address.
 */
#ifndef KHUX_HOST_H
#define KHUX_HOST_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#undef EXPORT
#include "../lift/rt.h"

/* ---------------------------------------------------------------- guest memory map (as emu/core.py) */
#define IMAGE_LO 0x1000u
#define IMAGE_HI 0x1011000u
#define TRAP_BASE 0x0F000000u          /* import slots, 8 bytes each */
#define TRAP_SIZE 0x40000u
#define RET_MAGIC (TRAP_BASE + TRAP_SIZE - 8)    /* LR for host -> guest calls */
#define HEAP_BASE 0x20000000u
#define HEAP_SIZE 0xC0000000u
#define STACK_BASE 0xE8000000u
#define STACK_SIZE 0x800000u
#define KUSER_PAGE 0xFFFF0000u
#define JHANDLE_BASE 0xF0000000u       /* JNI object handles: unmapped, a guest dereference faults */
#define JID_BASE 0xF8000000u           /* JNI method / field ids */
#define THREAD_STACK (1u << 20)

extern uint8_t *G;                     /* host address of guest address 0 */
#define GP(a) ((void *)(G + (uint32_t)(a)))
static __inline uint32_t g32(uint32_t a) { return *(uint32_t *)GP(a); }
static __inline void gw32(uint32_t a, uint32_t v) { *(uint32_t *)GP(a) = v; }
static __inline uint16_t g16(uint32_t a) { return *(uint16_t *)GP(a); }
static __inline void gw16(uint32_t a, uint16_t v) { *(uint16_t *)GP(a) = v; }
static __inline char *gstr(uint32_t a) { return a ? (char *)GP(a) : NULL; }

/* ---------------------------------------------------------------- lift.dll */
typedef struct Lift {
    HMODULE dll;
    const LiftEntry *table;
    int count;
    uint8_t *enabled;
    void (*run)(CPU *c, uint32_t target);
    LiftFn (*lookup)(uint32_t target);
    int (*native_call)(CPU *c, int id);
    void (*native_slot)(uint32_t addr, int id);
    int native_count;
    const char **native_names;
    uint32_t (*heap_malloc)(CPU *c, uint32_t n);
    uint32_t (*heap_free)(CPU *c, uint32_t p);
    uint32_t (*heap_realloc)(CPU *c, uint32_t p, uint32_t n);
    uint32_t (*heap_usable)(CPU *c, uint32_t p);
    uint32_t (*heap_ctl_size)(void);
    uint32_t *heap_ctl;
    uint32_t (**errno_fn)(void);
    void (**hook)(CPU *c, uint32_t target);
    char *fs_root, *so_guest, *so_host;
    int (*gl_init)(void);
    uint32_t *gl_strings;
    uint32_t *gl_draws;
    const struct { uint32_t addr; const char *name; } *plt;
    int plt_count;
} Lift;
extern Lift L;

/* ---------------------------------------------------------------- threads */
#define MAX_KEYS 128
typedef struct HThread {
    int tid;
    CPU *cpu;
    uint32_t errno_ptr;        /* guest int */
    uint32_t kuser_tls;        /* __kuser_get_tls block */
    uint32_t keys[MAX_KEYS];   /* pthread_getspecific values */
    uint32_t stack;            /* guest stack block (workers) */
    uint32_t fn, arg, retval;
    HANDLE handle;
    volatile LONG done;
    char name[96];
} HThread;
extern __declspec(thread) HThread *cur;     /* this host thread's guest thread */
HThread *thread_main(void);
HThread *thread_host(const char *name);   /* adopt the calling host thread as a guest thread */
CPU *cpu_new(uint32_t sp);
void threads_init(void);

/* ---------------------------------------------------------------- imports */
typedef void (*ImportFn)(CPU *c);
void imp(const char *name, ImportFn fn);            /* register an implementation */
uint32_t slot_for(const char *name);                /* guest address of an import's slot */
uint32_t host_fn(const char *name, ImportFn fn);    /* a host function the guest can call; its address */
void host_emu(CPU *c, uint32_t target);             /* lift.dll's c->emu: imports, slots, kuser helpers */
const char *import_name(uint32_t target);
extern __declspec(thread) uint32_t imp_target;   /* address of the import being run */
void imports_bind(void);
int import_exists(const char *name);               /* an implementation (host or lift.dll) exists */                            /* after every module registered its handlers */

/* arguments and results (AAPCS soft-float) */
static __inline uint32_t ARG(CPU *c, int k) { return k < 4 ? c->r[k] : g32(c->r[13] + 4u * (uint32_t)(k - 4)); }
static __inline void RET(CPU *c, uint32_t v) { c->r[0] = v; }
static __inline void RET64(CPU *c, uint64_t v) { c->r[0] = (uint32_t)v; c->r[1] = (uint32_t)(v >> 32); }
static __inline float U2F(uint32_t u) { union { uint32_t u; float f; } x; x.u = u; return x.f; }
static __inline uint32_t F2U(float f) { union { uint32_t u; float f; } x; x.f = f; return x.u; }
static __inline double U2D(uint64_t u) { union { uint64_t u; double d; } x; x.u = u; return x.d; }
static __inline uint64_t D2U(double d) { union { uint64_t u; double d; } x; x.d = d; return x.u; }

/* sequential argument reader (printf, JNI varargs): words from r0-r3 then the stack, or from a va_list */
typedef struct VA { CPU *c; int idx; uint32_t va; int use_va; } VA;
VA va_regs(CPU *c, int start);
VA va_list_at(CPU *c, uint32_t va);
uint32_t va_u32(VA *a);
uint64_t va_u64(VA *a);
double va_f64(VA *a);

/* calls into the guest; registers of the calling context are preserved */
uint32_t guest_call(CPU *c, uint32_t fn, int n, const uint32_t *args);
uint64_t guest_call64(CPU *c, uint32_t fn, int n, const uint32_t *args);
void guest_run(CPU *c, uint32_t target, uint32_t stop);
#define GCALL(c, fn, ...) guest_call((c), (fn), sizeof((uint32_t[]){__VA_ARGS__}) / 4, (uint32_t[]){__VA_ARGS__})
#define GCALL0(c, fn) guest_call((c), (fn), 0, NULL)

/* guest heap (lift.dll's rt_heap) */
uint32_t galloc(uint32_t n);
uint32_t gzalloc(uint32_t n);
uint32_t galloc_str(const char *s);
uint32_t galloc_bytes(const void *p, uint32_t n);
void gfree(uint32_t p);

/* errno of the current guest thread */
void set_errno(int e);

/* ---------------------------------------------------------------- loader, symbols */
void mem_init(void);
void mem_commit(uint32_t lo, uint32_t size);
void elf_load(const char *path);
uint32_t sym(const char *name);                     /* defined dynamic symbol, 0 if none */
extern uint32_t *init_array;
extern int init_count;
extern uint32_t exidx_addr, exidx_count;
uint32_t data_import(const char *name);

/* ---------------------------------------------------------------- paths, config */
extern char game_dir[1024];       /* extracted APK: lib .so + assets/ */
extern char fs_root[1024];        /* the device file system */
extern char server_ip[64];
#define PACKAGE "com.square_enix.android_googleplay.khuxww"
#define APK_DEVICE_PATH "/data/app/" PACKAGE "-1/base.apk"
#define SO_DEVICE_PATH "/data/app/" PACKAGE "-1/lib/arm/libcocos2dcpp.so"
#define FILES_DIR "/data/data/" PACKAGE "/files"
int host_path(const char *path, char *out, size_t cap, int create_parents);

/* ---------------------------------------------------------------- modules */
void libc_init(void);
void fs_init(void);
void net_init(void);
void jni_init(void);
void android_init(void);
void gl_init_imports(void);
void sl_init(void);
int gl_window(int w, int h, const char *title, const char *angle_dir);   /* window + EGL + ANGLE; 0 on success */
void gl_swap(void);
int gl_pump(void);                                 /* window messages; 0 when closed */

/* JNI (jni.c) */
extern uint32_t jni_env, jni_vm;
uint32_t jstring_new(const char *utf8);
uint32_t jclass_ref(const char *name);
uint32_t jarray_new(char type, uint32_t n, const void *data);
void android_video_poll(CPU *c);
extern int screen_w, screen_h;

/* touch input queued by the window (main thread consumes) */
typedef struct Touch { int kind; float x, y; } Touch;   /* 0 begin, 1 move, 2 end */
int touch_pop(Touch *t);
void touch_push(int kind, float x, float y);

/* Android UI (ui.c): text input box, text in a system font */
void edit_box_open(const char *title, const char *current, int max_len, int password);
int edit_box_message(MSG *m);
char *edit_box_take(void);
void edit_box_dismiss(void);
void edit_dialog_answer(CPU *c, const char *utf8);
extern volatile LONG edit_dialog_open;
uint8_t *text_render(const char *utf8, int size, int align, int want_w, int want_h, float r, float g, float b,
                     int stroke, float sr, float sg, float sb, int *out_w, int *out_h);

/* WebView (webview.c, WebView2) */
void webview_init(void);
int webview_create(void);
void webview_remove(int64_t k);
void webview_rect(int64_t k, int x, int y, int w, int h);
void webview_visible(int64_t k, int on);
void webview_load_url(int64_t k, const char *url);
void webview_load_html(int64_t k, const char *html);
void webview_scheme(int64_t k, const char *scheme);
void webview_js(int64_t k, const char *js);
int webview_nav(int64_t k, int what);

/* control channel (ctl.c) */
void ctl_init(void);
void ctl_frame(CPU *c, int frame);
int ctl_touch(int frame, int *down, float *x, float *y);
void ctl_event(const char *kind, const char *json_fields);
void ctl_hooks(void);
extern uint32_t ctl_renderer;

/* ---------------------------------------------------------------- logging */
void hlog(const char *fmt, ...);
__declspec(noreturn) void fatal(const char *fmt, ...);
const char *thread_label(void);

#endif
