/* Android WebView (cocos2d-x Cocos2dxWebViewHelper) on WebView2, the Edge engine built into Windows. Windows only:
 * other targets get their own backend behind the same Java methods.
 *
 * Each web view is a WebView2 controller placed over the game window at the rectangle the game gives. The game's
 * callbacks are those of the Android helper: shouldStartLoading (may cancel a navigation the user started),
 * didFinishLoading, didFailLoading, and onJsCallback for links using the page's JavaScript-interface scheme. They
 * run on the main thread (WebView2 events arrive through the window message pump, between frames).
 * The server's host name in URLs is rewritten to the server address: WebView2 does its own networking. */
#define COBJMACROS
#include "host.h"
#include <ole2.h>
#include "third_party/webview2/WebView2.h"

extern HWND gl_hwnd;

#define WV_MAX 16
typedef struct WV {
    int used;
    ICoreWebView2Controller *ctl;
    ICoreWebView2 *web;
    RECT rect;
    int visible, has_rect;
    wchar_t *pending_url;       /* load requested before the controller existed */
    wchar_t *pending_html;
    char scheme[64];            /* JavaScript-interface scheme (links to scheme:// go to onJsCallback) */
    wchar_t *own_nav;           /* the navigation we started ourselves (not asked to the game) */
} WV;
static WV views[WV_MAX];
static ICoreWebView2Environment *env;
static int env_state;           /* 0 none, 1 creating, 2 ready, -1 failed */
static CPU *main_cpu;

static wchar_t *wdup_utf8(const char *s) {
    int n = MultiByteToWideChar(CP_UTF8, 0, s ? s : "", -1, NULL, 0);
    wchar_t *w = calloc((size_t)n + 1, sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, s ? s : "", -1, w, n);
    return w;
}
static char *utf8_of(const wchar_t *w) {
    int n = WideCharToMultiByte(CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);
    char *s = calloc((size_t)n + 1, 1);
    WideCharToMultiByte(CP_UTF8, 0, w, -1, s, n, NULL, NULL);
    return s;
}

/* the game's server name -> the server we talk to */
static wchar_t *map_url(const char *url) {
    static const char host[] = "api.sp.kingdomhearts.com";
    const char *p = strstr(url, host);
    char buf[4096];
    if (p) snprintf(buf, sizeof(buf), "%.*s%s%s", (int)(p - url), url, server_ip, p + strlen(host));
    else snprintf(buf, sizeof(buf), "%s", url);
    return wdup_utf8(buf);
}

/* ---------------------------------------------------------------- calls into the game */
static uint32_t game_fn(const char *name) {
    char full[128];
    snprintf(full, sizeof(full), "Java_org_cocos2dx_lib_Cocos2dxWebViewHelper_%s", name);
    return sym(full);
}
static uint32_t notify(const char *name, int index, const wchar_t *url) {
    char *u = utf8_of(url);
    hlog("  webview %d: %s(%s)", index, name, u);
    uint32_t r = GCALL(main_cpu, game_fn(name), jni_env, jclass_ref("org/cocos2dx/lib/Cocos2dxWebViewHelper"),
                       (uint32_t)index, jstring_new(u));
    free(u);
    return r;
}

/* ---------------------------------------------------------------- COM handler objects */
typedef struct Handler { void *lpVtbl; LONG refs; int index; } Handler;

static HRESULT STDMETHODCALLTYPE h_qi(void *This, REFIID riid, void **out) {
    *out = This;
    ((Handler *)This)->refs++;
    return S_OK;
}
static ULONG STDMETHODCALLTYPE h_addref(void *This) { return (ULONG)InterlockedIncrement(&((Handler *)This)->refs); }
static ULONG STDMETHODCALLTYPE h_release(void *This) {
    LONG r = InterlockedDecrement(&((Handler *)This)->refs);
    if (!r) free(This);
    return (ULONG)r;
}
static void *handler_new(void *vtbl, int index) {
    Handler *h = calloc(1, sizeof(Handler));
    h->lpVtbl = vtbl;
    h->refs = 1;
    h->index = index;
    return h;
}

static void apply(int k);

/* navigation starting: ask the game (as Android's shouldOverrideUrlLoading), JS scheme -> onJsCallback */
static HRESULT STDMETHODCALLTYPE nav_starting(void *This, ICoreWebView2 *sender, ICoreWebView2NavigationStartingEventArgs *args) {
    int k = ((Handler *)This)->index;
    WV *v = &views[k];
    LPWSTR uri = NULL;
    ICoreWebView2NavigationStartingEventArgs_get_Uri(args, &uri);
    if (!uri) return S_OK;
    char *u = utf8_of(uri);
    size_t sl = strlen(v->scheme);
    if (sl && !strncmp(u, v->scheme, sl) && u[sl] == ':') {
        ICoreWebView2NavigationStartingEventArgs_put_Cancel(args, TRUE);
        notify("onJsCallback", k, uri);
    } else if (v->own_nav && !wcscmp(v->own_nav, uri)) {
        free(v->own_nav);                       /* a load the game asked for itself */
        v->own_nav = NULL;
    } else if (!strncmp(u, "http", 4) || !strncmp(u, "file", 4)) {
        if (!(notify("shouldStartLoading", k, uri) & 0xFF)) ICoreWebView2NavigationStartingEventArgs_put_Cancel(args, TRUE);
    }
    free(u);
    CoTaskMemFree(uri);
    return S_OK;
}
static struct { void *qi, *addref, *release, *invoke; } nav_starting_vtbl = {h_qi, h_addref, h_release, nav_starting};

static HRESULT STDMETHODCALLTYPE nav_completed(void *This, ICoreWebView2 *sender, ICoreWebView2NavigationCompletedEventArgs *args) {
    int k = ((Handler *)This)->index;
    BOOL ok = FALSE;
    ICoreWebView2NavigationCompletedEventArgs_get_IsSuccess(args, &ok);
    LPWSTR uri = NULL;
    ICoreWebView2_get_Source(sender, &uri);
    notify(ok ? "didFinishLoading" : "didFailLoading", k, uri ? uri : L"");
    if (uri) CoTaskMemFree(uri);
    return S_OK;
}
static struct { void *qi, *addref, *release, *invoke; } nav_completed_vtbl = {h_qi, h_addref, h_release, nav_completed};

static HRESULT STDMETHODCALLTYPE controller_created(void *This, HRESULT hr, ICoreWebView2Controller *ctl) {
    int k = ((Handler *)This)->index;
    WV *v = &views[k];
    if (!FAILED(hr) && ctl && v->used && v->ctl) return S_OK;     /* already has one */
    if (FAILED(hr) || !ctl || !v->used) {
        hlog("!! webview %d: controller creation failed (0x%08lX)", k, (unsigned long)hr);
        return S_OK;
    }
    v->ctl = ctl;
    ICoreWebView2Controller_AddRef(ctl);
    ICoreWebView2Controller_get_CoreWebView2(ctl, &v->web);
    EventRegistrationToken tok;
    ICoreWebView2_add_NavigationStarting(v->web, handler_new(&nav_starting_vtbl, k), &tok);
    ICoreWebView2_add_NavigationCompleted(v->web, handler_new(&nav_completed_vtbl, k), &tok);
    /* behave like an Android WebView: no link-preview status bar (bottom left on hover), no right-click menu,
     * no Ctrl+wheel zoom */
    ICoreWebView2Settings *set = NULL;
    if (SUCCEEDED(ICoreWebView2_get_Settings(v->web, &set)) && set) {
        ICoreWebView2Settings_put_IsStatusBarEnabled(set, FALSE);
        ICoreWebView2Settings_put_AreDefaultContextMenusEnabled(set, FALSE);
        ICoreWebView2Settings_put_IsZoomControlEnabled(set, FALSE);
        ICoreWebView2Settings_Release(set);
    }
    /* Android-style scroll bar: thin, translucent, no track or arrows (WebView2 shows the desktop one) */
    ICoreWebView2_AddScriptToExecuteOnDocumentCreated(v->web,
        L"(function(){var s=document.createElement('style');"
        L"s.textContent='::-webkit-scrollbar{width:4px;height:4px;background:transparent}"
        L"::-webkit-scrollbar-track{background:transparent}::-webkit-scrollbar-button{display:none}"
        L"::-webkit-scrollbar-thumb{background:rgba(255,255,255,0.35);border-radius:2px}';"
        L"(document.head||document.documentElement).appendChild(s);"
        L"document.addEventListener('DOMContentLoaded',function(){document.head.appendChild(s);});})();", NULL);
    hlog("  webview %d ready", k);
    apply(k);
    return S_OK;
}
static struct { void *qi, *addref, *release, *invoke; } controller_vtbl = {h_qi, h_addref, h_release, controller_created};

static void create_controller(int k) {
    ICoreWebView2Environment_CreateCoreWebView2Controller(env, gl_hwnd, handler_new(&controller_vtbl, k));
}

static HRESULT STDMETHODCALLTYPE env_created(void *This, HRESULT hr, ICoreWebView2Environment *e) {
    if (FAILED(hr) || !e) {
        env_state = -1;
        hlog("!! WebView2 is not available (0x%08lX): web pages will not show", (unsigned long)hr);
        return S_OK;
    }
    env = e;
    ICoreWebView2Environment_AddRef(e);
    env_state = 2;
    for (int k = 0; k < WV_MAX; k++) if (views[k].used && !views[k].ctl) create_controller(k);
    return S_OK;
}
static struct { void *qi, *addref, *release, *invoke; } env_vtbl = {h_qi, h_addref, h_release, env_created};

/* ---------------------------------------------------------------- state -> controller */
static void apply(int k) {
    WV *v = &views[k];
    if (!v->ctl) return;
    if (v->has_rect) ICoreWebView2Controller_put_Bounds(v->ctl, v->rect);
    ICoreWebView2Controller_put_IsVisible(v->ctl, v->visible);
    if (v->pending_html) {
        ICoreWebView2_NavigateToString(v->web, v->pending_html);
        free(v->pending_html);
        v->pending_html = NULL;
    }
    if (v->pending_url) {
        free(v->own_nav);
        v->own_nav = _wcsdup(v->pending_url);
        ICoreWebView2_Navigate(v->web, v->pending_url);
        free(v->pending_url);
        v->pending_url = NULL;
    }
}

static void ensure_env(void) {
    if (env_state) return;
    env_state = 1;
    char dir[1200];
    snprintf(dir, sizeof(dir), "%s/webview2", fs_root);
    CreateDirectoryA(dir, NULL);
    wchar_t *wdir = wdup_utf8(dir);
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(NULL, wdir, NULL, handler_new(&env_vtbl, 0));
    free(wdir);
    if (FAILED(hr)) {
        env_state = -1;
        hlog("!! WebView2 is not available (0x%08lX): web pages will not show", (unsigned long)hr);
    }
}

/* ---------------------------------------------------------------- Java methods (jni.c dispatches here) */
static WV *view(int64_t k) { return k >= 0 && k < WV_MAX && views[k].used ? &views[k] : NULL; }

int webview_create(void) {
    main_cpu = cur->cpu;
    for (int k = 0; k < WV_MAX; k++)
        if (!views[k].used) {
            memset(&views[k], 0, sizeof(WV));
            views[k].used = 1;
            views[k].visible = 1;
            int ready = env_state == 2;
            ensure_env();                       /* may complete at once and create the controller itself */
            if (ready) create_controller(k);
            hlog("  webview %d created", k);
            return k;
        }
    return -1;
}

void webview_remove(int64_t k) {
    WV *v = view(k);
    if (!v) return;
    if (v->ctl) {
        ICoreWebView2Controller_Close(v->ctl);
        ICoreWebView2Controller_Release(v->ctl);
    }
    if (v->web) ICoreWebView2_Release(v->web);
    free(v->pending_url);
    free(v->pending_html);
    free(v->own_nav);
    memset(v, 0, sizeof(*v));
    hlog("  webview %d removed", (int)k);
}

void webview_rect(int64_t k, int x, int y, int w, int h) {
    WV *v = view(k);
    if (!v) return;
    v->rect.left = x;
    v->rect.top = y;
    v->rect.right = x + w;
    v->rect.bottom = y + h;
    v->has_rect = 1;
    apply((int)k);
}

void webview_visible(int64_t k, int on) {
    WV *v = view(k);
    if (!v) return;
    v->visible = on;
    apply((int)k);
}

void webview_load_url(int64_t k, const char *url) {
    WV *v = view(k);
    if (!v || !url) return;
    free(v->pending_url);
    v->pending_url = map_url(url);
    hlog("  webview %d: load %s", (int)k, url);
    apply((int)k);
}

void webview_load_html(int64_t k, const char *html) {
    WV *v = view(k);
    if (!v || !html) return;
    free(v->pending_html);
    v->pending_html = wdup_utf8(html);
    apply((int)k);
}

void webview_scheme(int64_t k, const char *scheme) {
    WV *v = view(k);
    if (v && scheme) snprintf(v->scheme, sizeof(v->scheme), "%s", scheme);
}

void webview_js(int64_t k, const char *js) {
    WV *v = view(k);
    if (!v || !v->web || !js) return;
    wchar_t *w = wdup_utf8(js);
    ICoreWebView2_ExecuteScript(v->web, w, NULL);
    free(w);
}

int webview_nav(int64_t k, int what) {      /* 0 back, 1 forward, 2 reload, 3 stop, 4 can back, 5 can forward */
    WV *v = view(k);
    BOOL b = FALSE;
    if (!v || !v->web) return 0;
    switch (what) {
    case 0: ICoreWebView2_GoBack(v->web); break;
    case 1: ICoreWebView2_GoForward(v->web); break;
    case 2: ICoreWebView2_Reload(v->web); break;
    case 3: ICoreWebView2_Stop(v->web); break;
    case 4: ICoreWebView2_get_CanGoBack(v->web, &b); break;
    case 5: ICoreWebView2_get_CanGoForward(v->web, &b); break;
    }
    return b ? 1 : 0;
}

void webview_init(void) {
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) hlog("!! CoInitializeEx failed (0x%08lX)", (unsigned long)hr);
}
