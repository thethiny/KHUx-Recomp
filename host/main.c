/* KHUx native host: entry point, startup sequence and the render loop (as emu/run.py).
 *
 * khux.exe [--lift lift.dll] [--game dir] [--fs dir] [--server ip]
 *   --lift    the recompiled game (lift/build.py --all; default out/lift_all/lift.dll)
 *   --game    the installed game files: libcocos2dcpp.so and assets/ (default game/, made by host/install.py)
 *   --fs      the device file system: saves, preferences, downloads (default fs/, shared with the emulator)
 *   --server  the private server's address (default 127.0.0.1)
 */
#include "host.h"

void core_init(void);
void lift_load(const char *path);
void rewrite_load(const char *root);
void rewrite_report(void);
void coverage_init(const char *root);
void coverage_report(void);
void heap_init(void);
extern char so_host_path[1024];

static void run_step(CPU *c, const char *what, uint32_t fn, int n, const uint32_t *args) {
    hlog("== %s", what);
    uint32_t r = guest_call(c, fn, n, args);
    hlog("== %s -> 0x%x", what, r);
}

static uint32_t need_sym(const char *name) {
    uint32_t a = sym(name);
    if (!a) fatal("the game library has no symbol %s", name);
    return a;
}

/* the recomp folder: the nearest parent of the exe that has lift/rt.h */
static void recomp_root(char *out, size_t cap) {
    char p[MAX_PATH];
    GetModuleFileNameA(NULL, p, MAX_PATH);
    for (char *s = strrchr(p, '\\'); s; s = strrchr(p, '\\')) {
        *s = 0;
        char probe[MAX_PATH + 32];
        snprintf(probe, sizeof(probe), "%s\\lift\\rt.h", p);
        if (GetFileAttributesA(probe) != INVALID_FILE_ATTRIBUTES) {
            snprintf(out, cap, "%s", p);
            return;
        }
    }
    GetCurrentDirectoryA((DWORD)cap, out);
}

static void slashes(char *s) { for (; *s; s++) if (*s == '\\') *s = '/'; }

int main(int argc, char **argv) {
    char root[MAX_PATH], lift_path[1024];
    core_init();
    shadow_main_thread();
    recomp_root(root, sizeof(root));
    snprintf(lift_path, sizeof(lift_path), "%s\\out\\lift_all\\lift.dll", root);
    snprintf(game_dir, sizeof(game_dir), "%s\\game", root);
    snprintf(fs_root, sizeof(fs_root), "%s\\fs", root);
    for (int k = 1; k + 1 < argc; k += 2) {
        if (!strcmp(argv[k], "--lift")) snprintf(lift_path, sizeof(lift_path), "%s", argv[k + 1]);
        else if (!strcmp(argv[k], "--game")) snprintf(game_dir, sizeof(game_dir), "%s", argv[k + 1]);
        else if (!strcmp(argv[k], "--fs")) snprintf(fs_root, sizeof(fs_root), "%s", argv[k + 1]);
        else if (!strcmp(argv[k], "--server")) snprintf(server_ip, sizeof(server_ip), "%s", argv[k + 1]);
        else fatal("unknown option %s", argv[k]);
    }
    slashes(game_dir);
    slashes(fs_root);
    snprintf(so_host_path, sizeof(so_host_path), "%s/libcocos2dcpp.so", game_dir);
    if (GetFileAttributesA(so_host_path) == INVALID_FILE_ATTRIBUTES)
        fatal("%s not found: install the game first (python recomp/host/install.py <your KHUx v1.0.1 APK>)",
              so_host_path);

    mem_init();
    lift_load(lift_path);
    rewrite_load(root);
    coverage_init(root);
    heap_init();
    HThread *mt = thread_main();
    CPU *c = mt->cpu;

    libc_init();
    threads_init();
    fs_init();
    net_init();
    jni_init();
    android_init();
    sl_init();
    gl_init_imports();
    elf_load(so_host_path);
    imports_bind();
    webview_init();
    char angle[MAX_PATH + 16];
    snprintf(angle, sizeof(angle), "%s\\emu\\angle", root);
    gl_window(screen_w, screen_h, "KHUx v1.0.1 (native)", angle);
    ctl_init();
    ctl_hooks();

    int ran = 0;
    for (int k = 0; k < init_count; k++) {
        if (!init_array[k] || init_array[k] == 0xFFFFFFFFu) continue;
        guest_call(c, init_array[k], 0, NULL);
        ran++;
    }
    hlog("== %d constructors ran", ran);

    uint32_t helper = jclass_ref("org/cocos2dx/lib/Cocos2dxHelper");
    uint32_t a1[2] = {jni_vm, 0};
    run_step(c, "JNI_OnLoad", need_sym("JNI_OnLoad"), 2, a1);
    uint32_t a2[3] = {jni_env, helper, jstring_new(APK_DEVICE_PATH)};
    run_step(c, "nativeSetApkPath", need_sym("Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetApkPath"), 3, a2);
    uint32_t a3[4] = {jni_env, helper, jclass_ref("android/content/Context"), jclass_ref("android/content/res/AssetManager")};
    run_step(c, "nativeSetContext", need_sym("Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetContext"), 4, a3);
    ctl_renderer = jclass_ref("org/cocos2dx/lib/Cocos2dxRenderer");
    uint32_t a4[4] = {jni_env, ctl_renderer, (uint32_t)screen_w, (uint32_t)screen_h};
    run_step(c, "nativeInit", need_sym("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit"), 4, a4);

    uint32_t render = need_sym("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender");
    uint32_t t_begin = need_sym("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin");
    uint32_t t_end = need_sym("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd");
    uint32_t t_move = need_sym("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove");
    int frame = 0, pressed = 0;
    LARGE_INTEGER freq, t0, t1;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);
    while (gl_pump()) {
        shadow_park_point();                       /* M3 verifier: another thread's shadow run */
        /* window input, then injected input (the control channel), as nativeTouches* */
        Touch t;
        while (touch_pop(&t)) {
            if (t.kind == 0) GCALL(c, t_begin, jni_env, ctl_renderer, 0, F2U(t.x), F2U(t.y));
            else if (t.kind == 2) GCALL(c, t_end, jni_env, ctl_renderer, 0, F2U(t.x), F2U(t.y));
            else {
                int32_t id = 0;
                GCALL(c, t_move, jni_env, ctl_renderer, jarray_new('I', 1, &id), jarray_new('F', 1, &t.x),
                      jarray_new('F', 1, &t.y));
            }
        }
        int down;
        float x, y;
        if (ctl_touch(frame, &down, &x, &y)) {
            if (down && !pressed) GCALL(c, t_begin, jni_env, ctl_renderer, 0, F2U(x), F2U(y));
            else if (down) {
                int32_t id = 0;
                GCALL(c, t_move, jni_env, ctl_renderer, jarray_new('I', 1, &id), jarray_new('F', 1, &x),
                      jarray_new('F', 1, &y));
            } else if (pressed) GCALL(c, t_end, jni_env, ctl_renderer, 0, F2U(x), F2U(y));
            pressed = down;
        }
        char *answer = edit_box_take();
        if (answer) {
            edit_dialog_answer(c, answer);
            free(answer);
        }
        android_video_poll(c);
        android_iap_poll(c);
        *L.gl_draws = 0;
        GCALL(c, render, jni_env, ctl_renderer);
        ctl_frame(c, frame);
        gl_swap();
        frame++;
        if (frame % 600 == 0) {
            QueryPerformanceCounter(&t1);
            double dt = (double)(t1.QuadPart - t0.QuadPart) / (double)freq.QuadPart;
            hlog("-- frame %d: %.1f fps, %u draws", frame, 600 / dt, *L.gl_draws);
            t0 = t1;
        }
        if (frame % 1200 == 0) { rewrite_report(); coverage_report(); }
    }
    hlog("window closed");
    rewrite_report();
    coverage_report();
    ExitProcess(0);
}
