/* Control channel, the same HTTP interface as the emulator's (emu/control.py) on 127.0.0.1:8765, so the agent's
 * drivers (recomp.emu.ctl / step / boot_to_title / campaign) work on the native host unchanged.
 *
 *   events  scene changes (Director run/push/replace/pop), API responses (APIManager::onRespond), the resource
 *           revision, "screen" (game layers changed) and "idle" (framebuffer unchanged for IDLE_FRAMES)
 *   ui      the cocos2d node tree of the running scene (visible widgets, touchable ones flagged)
 *   input   tap, swipe, text
 * Events come from entry hooks in lift.dll (rt_hook). Guest calls only happen on the main thread: requests are
 * queued and served between frames. */
#include <winsock2.h>
#include "host.h"
#include <math.h>
#include <ctype.h>

#define CONTROL_PORT 8765
#define IDLE_FRAMES 60
#define HASH_EVERY 6

/* guest addresses (IDA names), as emu/control.py */
#define A_Director_getInstance 0x8EE518u
#define A_Director_runWithScene 0x8F06CCu
#define A_Director_pushScene 0x8F06FAu
#define A_Director_replaceScene 0x8F071Eu
#define A_Director_popScene 0x8F0804u
#define A_Director_setOpenGLView 0x8EF96Eu
#define A_APIManager_onRespond 0x4BAF7Cu
#define A_Node_getChildren 0x47AB26u
#define A_Node_isVisible 0x893334u
#define A_Node_getContentSize 0x89343Cu
#define A_Node_getName 0x89354Cu
#define A_Node_getNodeToWorldTransform 0x895818u
#define A_Widget_isTouchEnabled 0x826664u
#define A_Widget_isEnabled 0x8267DAu
#define A_Button_getTitleText 0x837AA0u
#define A_Text_getString 0x83A33Au
#define A_TextAtlas_getString 0x83AF7Eu
#define A_TextBMFont_getString 0x83B48Au
#define A_GLView_getViewPortRect 0x8D4550u
#define A_GLView_getScaleX 0x8D4554u
#define A_GLView_getScaleY 0x8D4558u
#define A_RESOURCE_SET_REVISION 0x4EABEEu
#define NODE_CHILDREN 0x180u

uint32_t ctl_renderer;
static int ready;
static CRITICAL_SECTION ev_lock;
static CONDITION_VARIABLE ev_cv;
static char **events;           /* JSON objects */
static char **ev_kind;          /* kind of each event, for waits */
static char **ev_key;           /* name / action of each event */
static int n_events, cap_events;
static volatile int frame_no;
static char scene_name[128];
static char layers_json[2048] = "null";
static uint32_t glview;
static int unchanged, idle_reported;
static uint64_t last_hash;
static char events_path[1100];

/* ---------------------------------------------------------------- small JSON builder */
typedef struct SB { char *s; size_t n, cap; } SB;
static void sb_put(SB *b, const char *s, size_t n) {
    if (b->n + n + 1 > b->cap) { b->cap = (b->n + n + 1) * 2 + 256; b->s = realloc(b->s, b->cap); }
    memcpy(b->s + b->n, s, n);
    b->n += n;
    b->s[b->n] = 0;
}
static void sb_cat(SB *b, const char *s) { sb_put(b, s, strlen(s)); }
static void sb_fmt(SB *b, const char *fmt, ...) {
    char tmp[1024];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    sb_put(b, tmp, (size_t)(n < (int)sizeof(tmp) ? n : (int)sizeof(tmp) - 1));
}
static void sb_str(SB *b, const char *s) {
    if (!s) { sb_cat(b, "null"); return; }
    sb_cat(b, "\"");
    for (; *s; s++) {
        unsigned char ch = (unsigned char)*s;
        if (ch == '"') sb_cat(b, "\\\"");
        else if (ch == '\\') sb_cat(b, "\\\\");
        else if (ch == '\n') sb_cat(b, "\\n");
        else if (ch < 0x20) sb_fmt(b, "\\u%04x", ch);
        else sb_put(b, (const char *)&ch, 1);
    }
    sb_cat(b, "\"");
}

/* ---------------------------------------------------------------- events */
static double now_s(void) {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return ((((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime) - 116444736000000000ULL) / 1e7;
}

/* fields: a JSON fragment (`"how": "x", "name": "y"`) or NULL */
static void event_full(const char *kind, const char *fields, const char *key) {
    if (!ready) return;
    EnterCriticalSection(&ev_lock);
    SB b = {0};
    sb_fmt(&b, "{\"seq\": %d, \"frame\": %d, \"t\": %.3f, \"kind\": ", n_events, frame_no, now_s());
    sb_str(&b, kind);
    if (fields && *fields) { sb_cat(&b, ", "); sb_cat(&b, fields); }
    sb_cat(&b, "}");
    if (n_events == cap_events) {
        cap_events = cap_events ? cap_events * 2 : 256;
        events = realloc(events, sizeof(char *) * (size_t)cap_events);
        ev_kind = realloc(ev_kind, sizeof(char *) * (size_t)cap_events);
        ev_key = realloc(ev_key, sizeof(char *) * (size_t)cap_events);
    }
    events[n_events] = b.s;
    ev_kind[n_events] = _strdup(kind);
    ev_key[n_events] = _strdup(key ? key : "");
    n_events++;
    FILE *f = fopen(events_path, "ab");
    if (f) { fputs(b.s, f); fputc('\n', f); fclose(f); }
    WakeAllConditionVariable(&ev_cv);
    LeaveCriticalSection(&ev_lock);
    hlog(">>> %s {%s}", kind, fields ? fields : "");
}

void ctl_event(const char *kind, const char *fields) { event_full(kind, fields, NULL); }

/* ---------------------------------------------------------------- guest introspection (main thread) */
static uint32_t entry(uint32_t ida) {
    int lo = 0, hi = L.count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        uint32_t a = L.table[mid].addr & ~1u;
        if (a == ida) return L.table[mid].addr;
        if (a < ida) lo = mid + 1; else hi = mid - 1;
    }
    fatal("control: 0x%08X is not a lifted function", ida);
}

static void hook_on(uint32_t ida) {
    uint32_t a = entry(ida);
    int lo = 0, hi = L.count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (L.table[mid].addr == a) { L.enabled[mid] |= 2; return; }
        if (L.table[mid].addr < a) lo = mid + 1; else hi = mid - 1;
    }
}

static char *std_string(uint32_t p, char *out, size_t cap) {
    uint8_t b0 = *(uint8_t *)GP(p);
    uint32_t size, data;
    if (b0 & 1) { size = g32(p + 4); data = g32(p + 8); }
    else { size = b0 >> 1; data = p + 1; }
    if (size >= cap) size = (uint32_t)cap - 1;
    memcpy(out, GP(data), size);
    out[size] = 0;
    return out;
}

static void demangle(const char *m, char *out, size_t cap) {
    out[0] = 0;
    const char *s = m;
    size_t len = strlen(m);
    char buf[256];
    if (len >= 2 && m[0] == 'N' && m[len - 1] == 'E') {
        memcpy(buf, m + 1, len - 2);
        buf[len - 2] = 0;
        s = buf;
    }
    while (*s) {
        if (!isdigit((unsigned char)*s)) { strncat(out, s, cap - strlen(out) - 1); break; }
        int n = atoi(s);
        while (isdigit((unsigned char)*s)) s++;
        if (out[0]) strncat(out, "::", cap - strlen(out) - 1);
        size_t k = strlen(out);
        if (k + (size_t)n >= cap) break;
        memcpy(out + k, s, (size_t)n);
        out[k + (size_t)n] = 0;
        s += n;
    }
}

/* only committed guest memory (a host read elsewhere would fault) */
static int plausible(uint32_t a) {
    return (a >= IMAGE_LO && a < IMAGE_HI - 16) || (a >= HEAP_BASE && a < HEAP_BASE + HEAP_SIZE - 16) ||
           (a >= STACK_BASE && a < STACK_BASE + STACK_SIZE - 16);
}

static uint32_t typeinfo(uint32_t obj) {
    if (!plausible(obj)) return 0;
    uint32_t vt = g32(obj);
    return plausible(vt) ? g32(vt - 4) : 0;
}
static const char *ti_name(uint32_t ti) { return plausible(ti) && plausible(g32(ti + 4)) ? gstr(g32(ti + 4)) : ""; }

static void class_name(uint32_t obj, char *out, size_t cap) {
    uint32_t ti = typeinfo(obj);
    if (!ti) { snprintf(out, cap, "<0x%x>", obj); return; }
    demangle(ti_name(ti), out, cap);
}

static int is_a(uint32_t obj, const char *mangled) {
    uint32_t stack[64], seen[64];
    int sp = 0, ns = 0;
    uint32_t ti = typeinfo(obj);
    if (!ti) return 0;
    stack[sp++] = ti;
    while (sp) {
        ti = stack[--sp];
        int dup = 0;
        for (int k = 0; k < ns; k++) if (seen[k] == ti) dup = 1;
        if (dup || !plausible(ti) || ns >= 64) continue;
        seen[ns++] = ti;
        if (!strcmp(ti_name(ti), mangled)) return 1;
        const char *kind = ti_name(typeinfo(ti));
        if (strstr(kind, "__si_class_type_info")) { if (sp < 64) stack[sp++] = g32(ti + 8); }
        else if (strstr(kind, "__vmi_class_type_info")) {
            uint32_t n = g32(ti + 12);
            for (uint32_t k = 0; k < n && k < 16 && sp < 64; k++) stack[sp++] = g32(ti + 16 + 8 * k);
        }
    }
    return 0;
}

static int children(uint32_t node, uint32_t *out, int cap) {
    uint32_t b = g32(node + NODE_CHILDREN), e = g32(node + NODE_CHILDREN + 4);
    int n = 0;
    for (uint32_t p = b; p < e && n < cap && n < 500; p += 4) out[n++] = g32(p);
    return n;
}

static CPU *mc;     /* main thread CPU */

static uint32_t running_scene(void) {
    uint32_t d = GCALL0(mc, entry(A_Director_getInstance));
    uint32_t b = g32(d + 236), e = g32(d + 240);
    return e > b ? g32(e - 4) : 0;
}

static void screen_layers(uint32_t scene, SB *b) {
    uint32_t level[200], next[200];
    int nl = 0, nn;
    char names[32][128];
    int n_names = 0;
    if (!scene) { sb_cat(b, "[]"); return; }
    level[nl++] = scene;
    for (int depth = 0; depth < 3; depth++) {
        nn = 0;
        for (int k = 0; k < nl; k++) {
            uint32_t kids[500];
            int nk = children(level[k], kids, 500);
            for (int j = 0; j < nk; j++) {
                char cls[128];
                class_name(kids[j], cls, sizeof(cls));
                int dup = 0;
                for (int q = 0; q < n_names; q++) if (!strcmp(names[q], cls)) dup = 1;
                if (strncmp(cls, "cocos2d::", 9) && !dup && n_names < 32) strcpy(names[n_names++], cls);
                if (nn < 200) next[nn++] = kids[j];
            }
        }
        memcpy(level, next, sizeof(uint32_t) * (size_t)nn);
        nl = nn;
    }
    sb_cat(b, "[");
    for (int k = 0; k < n_names; k++) {
        if (k) sb_cat(b, ", ");
        sb_str(b, names[k]);
    }
    sb_cat(b, "]");
}

static void node_name(uint32_t node, char *out, size_t cap) {
    uint32_t s = gzalloc(12);
    GCALL(mc, entry(A_Node_getName), s, node);
    std_string(s, out, cap);       /* a long name's heap copy is leaked (debug only) */
    gfree(s);
}

typedef struct Xf { float vx, vy, vw, vh, sx, sy; int ok; } Xf;
static Xf screen_xf(void) {
    Xf x = {0};
    if (!glview) return x;
    x.sx = U2F(GCALL(mc, entry(A_GLView_getScaleX), glview));
    x.sy = U2F(GCALL(mc, entry(A_GLView_getScaleY), glview));
    uint32_t r = GCALL(mc, entry(A_GLView_getViewPortRect), glview);
    float *f = GP(r);
    x.vx = f[0]; x.vy = f[1]; x.vw = f[2]; x.vh = f[3];
    x.ok = 1;
    return x;
}
static void to_screen(const Xf *x, float wx, float wy, float *sx, float *sy) {
    if (!x->ok) { *sx = wx; *sy = (float)screen_h - wy; return; }
    *sx = x->vx + wx * x->sx;
    *sy = (float)screen_h - (x->vy + wy * x->sy);
}

static void node_rect(uint32_t node, const Xf *x, int r[4]) {
    float *sz = GP(GCALL(mc, entry(A_Node_getContentSize), node));
    float w = sz[0], h = sz[1];
    uint32_t m = gzalloc(64);
    GCALL(mc, entry(A_Node_getNodeToWorldTransform), m, node);
    float t[16];
    memcpy(t, GP(m), 64);
    gfree(m);
    float minx = 1e9f, miny = 1e9f, maxx = -1e9f, maxy = -1e9f;
    float pts[4][2] = {{0, 0}, {w, 0}, {0, h}, {w, h}};
    for (int k = 0; k < 4; k++) {
        float wx = t[0] * pts[k][0] + t[4] * pts[k][1] + t[12], wy = t[1] * pts[k][0] + t[5] * pts[k][1] + t[13];
        float sx, sy;
        to_screen(x, wx, wy, &sx, &sy);
        if (sx < minx) minx = sx;
        if (sy < miny) miny = sy;
        if (sx > maxx) maxx = sx;
        if (sy > maxy) maxy = sy;
    }
    r[0] = (int)lroundf(minx); r[1] = (int)lroundf(miny); r[2] = (int)lroundf(maxx); r[3] = (int)lroundf(maxy);
}

typedef struct W { char path[512], cls[128], name[128], text[512], label[512]; int touch, has_text, rect[4]; uint32_t addr; } W;
static W *ws;
static int n_ws, cap_ws;

static void walk(uint32_t node, const char *path, const Xf *x, int all) {
    if (!(GCALL(mc, entry(A_Node_isVisible), node) & 0xFF)) return;
    W w = {0};
    node_name(node, w.name, sizeof(w.name));
    class_name(node, w.cls, sizeof(w.cls));
    const char *short_cls = strrchr(w.cls, ':') ? strrchr(w.cls, ':') + 1 : w.cls;
    snprintf(w.path, sizeof(w.path), "%s/%s", path, w.name[0] ? w.name : short_cls);
    w.addr = node;
    int add = all;
    if (is_a(node, "N7cocos2d2ui6WidgetE")) {
        int touch = GCALL(mc, entry(A_Widget_isTouchEnabled), node) & 0xFF;
        int enabled = GCALL(mc, entry(A_Widget_isEnabled), node) & 0xFF;
        uint32_t sp = 0;
        size_t cl = strlen(w.cls);
        if (is_a(node, "N7cocos2d2ui6ButtonE")) sp = GCALL(mc, entry(A_Button_getTitleText), node);
        else if (is_a(node, "N7cocos2d2ui4TextE")) sp = GCALL(mc, entry(A_Text_getString), node);
        else if (cl >= 9 && !strcmp(w.cls + cl - 9, "TextAtlas")) sp = GCALL(mc, entry(A_TextAtlas_getString), node);
        else if (cl >= 10 && !strcmp(w.cls + cl - 10, "TextBMFont")) sp = GCALL(mc, entry(A_TextBMFont_getString), node);
        if (sp) std_string(sp, w.text, sizeof(w.text));
        w.has_text = w.text[0] != 0;
        w.touch = touch && enabled;
        if (touch || w.has_text) add = 1;
    }
    if (add) {
        node_rect(node, x, w.rect);
        if (n_ws == cap_ws) { cap_ws = cap_ws ? cap_ws * 2 : 256; ws = realloc(ws, sizeof(W) * (size_t)cap_ws); }
        ws[n_ws++] = w;
    }
    uint32_t vec = GCALL(mc, entry(A_Node_getChildren), node);
    uint32_t b = g32(vec), e = g32(vec + 4);
    char p[512];
    snprintf(p, sizeof(p), "%s", w.path);
    for (uint32_t q = b; q < e && (q - b) / 4 < 500; q += 4) walk(g32(q), p, x, all);
}

static char *ui_json(int all) {
    SB b = {0};
    uint32_t scene = running_scene();
    if (!scene) { sb_cat(&b, "{\"scene\": null, \"widgets\": []}"); return b.s; }
    Xf x = screen_xf();
    n_ws = 0;
    walk(scene, "", &x, all);
    for (int k = 0; k < n_ws; k++) {
        size_t cl = strlen(ws[k].cls);
        if (cl >= 6 && !strcmp(ws[k].cls + cl - 6, "Button") && !ws[k].has_text) {
            size_t pl = strlen(ws[k].path);
            for (int j = 0; j < n_ws; j++)
                if (j != k && ws[j].has_text && !strncmp(ws[j].path, ws[k].path, pl) && ws[j].path[pl] == '/') {
                    if (ws[k].label[0]) strncat(ws[k].label, " / ", sizeof(ws[k].label) - strlen(ws[k].label) - 1);
                    strncat(ws[k].label, ws[j].text, sizeof(ws[k].label) - strlen(ws[k].label) - 1);
                }
        }
    }
    char cls[128];
    class_name(scene, cls, sizeof(cls));
    sb_cat(&b, "{\"scene\": ");
    sb_str(&b, cls);
    sb_cat(&b, ", \"screen\": ");
    screen_layers(scene, &b);
    sb_cat(&b, ", \"widgets\": [");
    for (int k = 0; k < n_ws; k++) {
        W *w = &ws[k];
        sb_cat(&b, k ? ",\n {" : "\n {");
        sb_cat(&b, "\"path\": "); sb_str(&b, w->path);
        sb_cat(&b, ", \"class\": "); sb_str(&b, w->cls);
        sb_cat(&b, ", \"name\": "); sb_str(&b, w->name);
        sb_cat(&b, ", \"text\": "); sb_str(&b, w->has_text ? w->text : NULL);
        if (w->label[0]) { sb_cat(&b, ", \"label\": "); sb_str(&b, w->label); }
        sb_fmt(&b, ", \"touch\": %s, \"rect\": [%d, %d, %d, %d], \"addr\": \"0x%x\"}", w->touch ? "true" : "false",
               w->rect[0], w->rect[1], w->rect[2], w->rect[3], w->addr);
    }
    sb_cat(&b, "]}");
    return b.s;
}

/* ---------------------------------------------------------------- input */
typedef struct In { int frame, down; float x, y; } In;
static In pend[4096];
static int n_pend;
static CRITICAL_SECTION in_lock;

int ctl_touch(int frame, int *down, float *x, float *y) {
    EnterCriticalSection(&in_lock);
    while (n_pend && pend[0].frame < frame) memmove(pend, pend + 1, sizeof(In) * (size_t)--n_pend);
    int r = 0;
    if (n_pend && pend[0].frame == frame) {
        *down = pend[0].down; *x = pend[0].x; *y = pend[0].y;
        memmove(pend, pend + 1, sizeof(In) * (size_t)--n_pend);
        r = 1;
    }
    LeaveCriticalSection(&in_lock);
    return r;
}

static void push_in(int frame, int down, float x, float y) {
    if (n_pend < 4096) { pend[n_pend].frame = frame; pend[n_pend].down = down; pend[n_pend].x = x; pend[n_pend].y = y; n_pend++; }
}

static char *tap_json(float x, float y) {
    EnterCriticalSection(&in_lock);
    int start = frame_no + 2;
    for (int k = 0; k < 3; k++) push_in(start + k, 1, x, y);
    push_in(start + 3, 0, x, y);
    LeaveCriticalSection(&in_lock);
    unchanged = 0;
    idle_reported = 0;
    SB b = {0};
    sb_fmt(&b, "{\"tap\": [%g, %g], \"frame\": %d}", x, y, start);
    return b.s;
}

static char *swipe_json(float x0, float y0, float x1, float y1, int frames, int hold) {
    EnterCriticalSection(&in_lock);
    int start = frame_no + 2;
    for (int k = 0; k < hold; k++) push_in(start + k, 1, x0, y0);
    start += hold;
    for (int k = 0; k <= frames; k++) {
        float t = frames ? (float)k / (float)frames : 1.0f;
        push_in(start + k, 1, x0 + (x1 - x0) * t, y0 + (y1 - y0) * t);
    }
    start += frames + 1;
    for (int k = 0; k < hold; k++) push_in(start + k, 1, x1, y1);
    push_in(start + hold, 0, x1, y1);
    LeaveCriticalSection(&in_lock);
    unchanged = 0;
    idle_reported = 0;
    SB b = {0};
    sb_fmt(&b, "{\"swipe\": [%g, %g, %g, %g], \"frames\": %d, \"hold\": %d}", x0, y0, x1, y1, frames, hold);
    return b.s;
}

static char *text_json(const char *s, int enter) {
    SB b = {0};
    if (edit_dialog_open) {
        edit_box_dismiss();
        edit_dialog_answer(mc, s);
        sb_cat(&b, "{\"text\": "); sb_str(&b, s); sb_cat(&b, ", \"via\": \"edit dialog\"}");
        return b.s;
    }
    uint32_t fn = sym("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInsertText");
    GCALL(mc, fn, jni_env, ctl_renderer, jstring_new(s));
    if (enter) GCALL(mc, fn, jni_env, ctl_renderer, jstring_new("\n"));
    sb_cat(&b, "{\"text\": "); sb_str(&b, s); sb_fmt(&b, ", \"enter\": %s}", enter ? "true" : "false");
    return b.s;
}

/* ---------------------------------------------------------------- requests served on the main thread */
typedef struct Req { char path[64]; char q[1024]; char *result; HANDLE done; struct Req *next; } Req;
static Req *req_head;
static CRITICAL_SECTION req_lock;

static const char *qget(const char *q, const char *key, char *out, size_t cap) {
    size_t kl = strlen(key);
    for (const char *p = q; p && *p;) {
        if (!strncmp(p, key, kl) && p[kl] == '=') {
            const char *v = p + kl + 1;
            size_t n = 0;
            while (*v && *v != '&' && n + 1 < cap) {
                if (*v == '%' && isxdigit((unsigned char)v[1]) && isxdigit((unsigned char)v[2])) {
                    char h[3] = {v[1], v[2], 0};
                    out[n++] = (char)strtol(h, NULL, 16);
                    v += 3;
                } else {
                    out[n++] = *v == '+' ? ' ' : *v;
                    v++;
                }
            }
            out[n] = 0;
            return out;
        }
        p = strchr(p, '&');
        if (p) p++;
    }
    return NULL;
}

static char *find_widget_tap(const char *q) {
    char name[256] = "", text[256] = "";
    int has_name = qget(q, "name", name, sizeof(name)) != NULL, has_text = qget(q, "text", text, sizeof(text)) != NULL;
    free(ui_json(0));
    for (int k = 0; k < n_ws; k++) {
        W *w = &ws[k];
        if (!w->touch) continue;
        char cap[1100];
        snprintf(cap, sizeof(cap), "%s %s", w->has_text ? w->text : "", w->label);
        int hit = has_name && !strcmp(w->name, name);
        if (!hit && has_text && text[0]) {
            char a[1100], t[256];
            for (size_t i = 0; i <= strlen(cap); i++) a[i] = (char)tolower((unsigned char)cap[i]);
            for (size_t i = 0; i <= strlen(text); i++) t[i] = (char)tolower((unsigned char)text[i]);
            hit = strstr(a, t) != NULL;
        }
        if (hit) {
            char *r = tap_json((w->rect[0] + w->rect[2]) / 2.0f, (w->rect[1] + w->rect[3]) / 2.0f);
            SB b = {0};
            sb_put(&b, r, strlen(r) - 1);
            sb_cat(&b, ", \"widget\": {\"name\": ");
            sb_str(&b, w->name);
            sb_fmt(&b, ", \"rect\": [%d, %d, %d, %d]}}", w->rect[0], w->rect[1], w->rect[2], w->rect[3]);
            free(r);
            return b.s;
        }
    }
    return _strdup("{\"error\": \"no touchable widget matches\"}");
}

static char *serve_main(Req *r) {
    char v[256], v2[256];
    if (!strcmp(r->path, "ui")) return ui_json(qget(r->q, "all", v, sizeof(v)) && !strcmp(v, "1"));
    if (!strcmp(r->path, "tap")) {
        if (qget(r->q, "x", v, sizeof(v)) && qget(r->q, "y", v2, sizeof(v2))) return tap_json((float)atof(v), (float)atof(v2));
        return find_widget_tap(r->q);
    }
    if (!strcmp(r->path, "swipe")) {
        char a[4][64];
        const char *k[4] = {"x0", "y0", "x1", "y1"};
        for (int i = 0; i < 4; i++) if (!qget(r->q, k[i], a[i], 64)) return _strdup("{\"error\": \"swipe needs x0 y0 x1 y1\"}");
        int frames = qget(r->q, "frames", v, sizeof(v)) ? atoi(v) : 12, hold = qget(r->q, "hold", v2, sizeof(v2)) ? atoi(v2) : 0;
        return swipe_json((float)atof(a[0]), (float)atof(a[1]), (float)atof(a[2]), (float)atof(a[3]), frames, hold);
    }
    if (!strcmp(r->path, "text")) {
        char s[512] = "";
        qget(r->q, "s", s, sizeof(s));
        return text_json(s, !(qget(r->q, "enter", v, sizeof(v)) && !strcmp(v, "0")));
    }
    if (!strcmp(r->path, "peek")) {
        if (!qget(r->q, "addr", v, sizeof(v))) return _strdup("{\"error\": \"peek needs addr\"}");
        uint32_t addr = (uint32_t)strtoul(v, NULL, 16);
        SB b = {0};
        if (qget(r->q, "call", v2, sizeof(v2))) {
            sb_fmt(&b, "{\"r0\": \"0x%x\"}", GCALL(mc, (uint32_t)strtoul(v2, NULL, 16), addr));
            return b.s;
        }
        int n = qget(r->q, "n", v2, sizeof(v2)) ? atoi(v2) : 64;
        sb_fmt(&b, "{\"addr\": \"0x%x\", \"words\": [", addr);
        for (int k = 0; k < n / 4; k++) sb_fmt(&b, "%s\"0x%x\"", k ? ", " : "", g32(addr + 4u * (uint32_t)k));
        sb_cat(&b, "]}");
        return b.s;
    }
    if (!strcmp(r->path, "warp")) {
        /* DEV/TEST HARNESS ONLY: warp?avatar=0x..&to=0x.. puts the player next to another map actor through the
         * game's own walkability-checked AvatarActor::setSefeMoveMapPotision (map position at +624/+628) */
        if (!qget(r->q, "avatar", v, sizeof(v)) || !qget(r->q, "to", v2, sizeof(v2)))
            return _strdup("{\"error\": \"warp needs avatar and to\"}");
        uint32_t avatar = (uint32_t)strtoul(v, NULL, 16), to = (uint32_t)strtoul(v2, NULL, 16);
        float x = *(float *)GP(to + 624), y = *(float *)GP(to + 628);
        uint32_t fn = sym("_ZN11AvatarActor22setSefeMoveMapPotisionERN7cocos2d4Vec2E"), vec = gzalloc(8);
        static const float off[7][2] = {{0, -80}, {80, 0}, {-80, 0}, {0, 80}, {0, -160}, {160, 0}, {-160, 0}};
        SB b = {0};
        for (int k = 0; k < 7; k++) {
            float p[2] = {x + off[k][0], y + off[k][1]};
            memcpy(GP(vec), p, 8);
            if (GCALL(mc, fn, avatar, vec) & 0xFF) {
                sb_fmt(&b, "{\"warped\": [%g, %g]}", p[0], p[1]);
                gfree(vec);
                return b.s;
            }
        }
        gfree(vec);
        sb_fmt(&b, "{\"error\": \"no walkable spot near target\", \"target\": [%g, %g]}", x, y);
        return b.s;
    }
    if (!strcmp(r->path, "textdata")) {
        char ids[512] = "";
        qget(r->q, "id", ids, sizeof(ids));
        uint32_t tt = qget(r->q, "type", v, sizeof(v)) ? (uint32_t)atoi(v) : 0;
        uint32_t fn = sym("_ZN8TextData3getENS_4TypeEi");
        SB b = {0};
        sb_cat(&b, "{");
        int first = 1;
        for (char *tok = strtok(ids, ","); tok; tok = strtok(NULL, ",")) {
            uint32_t out = gzalloc(8);
            GCALL(mc, fn, out, tt, (uint32_t)atoi(tok));
            uint32_t e = g32(out);
            gfree(out);
            sb_cat(&b, first ? "" : ", ");
            first = 0;
            sb_str(&b, tok);
            sb_cat(&b, ": ");
            sb_str(&b, e ? gstr(g32(e + 4)) : NULL);
        }
        sb_cat(&b, "}");
        return b.s;
    }
    return NULL;
}

static char *run_on_main(const char *path, const char *q) {
    Req *r = calloc(1, sizeof(Req));
    strncpy(r->path, path, 63);
    strncpy(r->q, q, 1023);
    r->done = CreateEventA(NULL, TRUE, FALSE, NULL);
    EnterCriticalSection(&req_lock);
    Req **p = &req_head;
    while (*p) p = &(*p)->next;
    *p = r;
    LeaveCriticalSection(&req_lock);
    char *res;
    if (WaitForSingleObject(r->done, 10000) != WAIT_OBJECT_0) {
        res = _strdup("{\"error\": \"timeout waiting for the main loop\"}");    /* r leaks; the loop still owns it */
        return res;
    }
    res = r->result;
    CloseHandle(r->done);
    free(r);
    return res;
}

/* ---------------------------------------------------------------- per frame (main thread) */
extern void (__stdcall *gl_read_pixels)(int, int, int, int, unsigned, unsigned, void *);
static uint8_t *pixels;

void ctl_frame(CPU *c, int frame) {
    mc = c;
    frame_no = frame;
    if (frame % 30 == 0) {
        SB b = {0};
        screen_layers(running_scene(), &b);
        if (strcmp(b.s, layers_json)) {
            strncpy(layers_json, b.s, sizeof(layers_json) - 1);
            unchanged = 0;
            idle_reported = 0;
            SB f = {0};
            sb_cat(&f, "\"layers\": ");
            sb_cat(&f, b.s);
            event_full("screen", f.s, NULL);
            free(f.s);
        }
        free(b.s);
    }
    if (frame % HASH_EVERY == 0 && gl_read_pixels) {
        if (!pixels) pixels = malloc((size_t)screen_w * screen_h * 4);
        gl_read_pixels(0, 0, screen_w, screen_h, 0x1908, 0x1401, pixels);
        uint64_t h = 1469598103934665603ULL;
        const uint64_t *w = (const uint64_t *)pixels;
        for (size_t k = 0; k < (size_t)screen_w * screen_h / 2; k++) h = (h ^ w[k]) * 1099511628211ULL;
        if (h == last_hash) unchanged += HASH_EVERY;
        else { unchanged = 0; idle_reported = 0; }
        last_hash = h;
        if (unchanged >= IDLE_FRAMES && !idle_reported) {
            idle_reported = 1;
            SB f = {0};
            sb_cat(&f, "\"scene\": ");
            sb_str(&f, scene_name[0] ? scene_name : NULL);
            event_full("idle", f.s, NULL);
            free(f.s);
        }
    }
    for (;;) {
        EnterCriticalSection(&req_lock);
        Req *r = req_head;
        if (r) req_head = r->next;
        LeaveCriticalSection(&req_lock);
        if (!r) break;
        r->result = serve_main(r);
        if (!r->result) r->result = _strdup("{\"error\": \"unknown endpoint\"}");
        SetEvent(r->done);
    }
}

/* ---------------------------------------------------------------- hooks (any thread) */
static void on_hook(CPU *c, uint32_t target) {
    uint32_t a = target & ~1u;
    char fields[2600];
    if (a == A_Director_runWithScene || a == A_Director_pushScene || a == A_Director_replaceScene) {
        const char *how = a == A_Director_runWithScene ? "runWithScene" : a == A_Director_pushScene ? "pushScene" : "replaceScene";
        uint32_t scene = c->r[1], kids[500];
        int nk = children(scene, kids, 500);
        SB b = {0};
        char name[128] = "", cls[128];
        sb_cat(&b, "[");
        for (int k = 0; k < nk; k++) {
            class_name(kids[k], cls, sizeof(cls));
            if (k) sb_cat(&b, ", ");
            sb_str(&b, cls);
            if (!name[0] && strncmp(cls, "cocos2d::", 9)) strcpy(name, cls);
        }
        sb_cat(&b, "]");
        if (!name[0]) class_name(scene, name, sizeof(name));
        strcpy(scene_name, name);
        unchanged = 0;
        idle_reported = 0;
        snprintf(fields, sizeof(fields), "\"how\": \"%s\", \"name\": \"%s\", \"layers\": %s", how, name, b.s);
        free(b.s);
        event_full("scene", fields, name);
    } else if (a == A_Director_popScene) {
        event_full("scene", "\"how\": \"popScene\", \"name\": null", NULL);
    } else if (a == A_Director_setOpenGLView) {
        glview = c->r[1];
    } else if (a == A_APIManager_onRespond) {
        char key[16];
        snprintf(fields, sizeof(fields), "\"action\": %u, \"status\": %u", c->r[1], c->r[2]);
        snprintf(key, sizeof(key), "%u", c->r[1]);
        event_full("api", fields, key);
    } else if (a == A_RESOURCE_SET_REVISION) {
        snprintf(fields, sizeof(fields), "\"revision\": %u, \"size\": %u", c->r[1], c->r[2]);
        event_full("resource_revision_saved", fields, NULL);
    }
}

void ctl_hooks(void) {
    *L.hook = on_hook;
    uint32_t hooks[] = {A_Director_runWithScene, A_Director_pushScene, A_Director_replaceScene, A_Director_popScene,
                        A_Director_setOpenGLView, A_APIManager_onRespond, A_RESOURCE_SET_REVISION};
    for (int k = 0; k < (int)(sizeof(hooks) / sizeof(*hooks)); k++) hook_on(hooks[k]);
}

/* ---------------------------------------------------------------- HTTP */
static char *state_json(void) {
    SB b = {0};
    sb_fmt(&b, "{\"frame\": %d, \"scene\": ", frame_no);
    sb_str(&b, scene_name[0] ? scene_name : NULL);
    sb_fmt(&b, ", \"screen\": %s, \"unchanged_frames\": %d, \"idle\": %s, \"events\": %d, \"threads\": [], "
           "\"window\": [%d, %d]}", layers_json, unchanged, idle_reported ? "true" : "false", n_events, screen_w, screen_h);
    return b.s;
}

static char *events_json(int since) {
    SB b = {0};
    sb_cat(&b, "[");
    EnterCriticalSection(&ev_lock);
    for (int k = since < 0 ? 0 : since; k < n_events; k++) {
        if (k > since) sb_cat(&b, ",\n");
        sb_cat(&b, events[k]);
    }
    LeaveCriticalSection(&ev_lock);
    sb_cat(&b, "]");
    return b.s;
}

static char *wait_json(const char *cond, int since, double timeout) {
    char kind[64], val[128] = "";
    strncpy(kind, cond, 63);
    kind[63] = 0;
    char *eq = strchr(kind, '=');
    if (eq) { strncpy(val, eq + 1, 127); *eq = 0; }
    double deadline = now_s() + timeout;
    EnterCriticalSection(&ev_lock);
    for (;;) {
        for (int k = since < 0 ? 0 : since; k < n_events; k++) {
            if (strcmp(ev_kind[k], kind)) continue;
            char quoted[140];
            snprintf(quoted, sizeof(quoted), "\"%s\"", val);
            if (!val[0] || !strcmp(ev_key[k], val) || strstr(events[k], quoted)) {
                char *r = _strdup(events[k]);
                LeaveCriticalSection(&ev_lock);
                return r;
            }
        }
        double left = deadline - now_s();
        if (left <= 0) break;
        SleepConditionVariableCS(&ev_cv, &ev_lock, (DWORD)(left * 1000) + 1);
    }
    LeaveCriticalSection(&ev_lock);
    SB b = {0};
    sb_fmt(&b, "{\"timeout\": true, \"state\": {\"frame\": %d, \"scene\": ", frame_no);
    sb_str(&b, scene_name[0] ? scene_name : NULL);
    sb_cat(&b, "}}");
    return b.s;
}

static DWORD WINAPI client(LPVOID p) {
    SOCKET s = (SOCKET)(uintptr_t)p;
    char req[4096];
    int n = recv(s, req, sizeof(req) - 1, 0);
    if (n <= 0) { closesocket(s); return 0; }
    req[n] = 0;
    char target[2048] = "";
    sscanf(req, "GET %2047s", target);
    char *q = strchr(target, '?');
    if (q) *q++ = 0; else q = "";
    char *path = target;
    while (*path == '/') path++;
    char v[64], v2[64];
    char *body;
    if (!strcmp(path, "state")) body = state_json();
    else if (!strcmp(path, "events")) body = events_json(qget(q, "since", v, sizeof(v)) ? atoi(v) : 0);
    else if (!strcmp(path, "wait")) {
        char cond[128] = "idle";
        qget(q, "for", cond, sizeof(cond));
        body = wait_json(cond, qget(q, "since", v, sizeof(v)) ? atoi(v) : n_events,
                         qget(q, "timeout", v2, sizeof(v2)) ? atof(v2) : 60);
    } else if (!strcmp(path, "ui") || !strcmp(path, "tap") || !strcmp(path, "swipe") || !strcmp(path, "text") ||
               !strcmp(path, "peek") || !strcmp(path, "textdata") || !strcmp(path, "warp")) body = run_on_main(path, q);
    else body = _strdup("{\"error\": \"unknown endpoint\", \"endpoints\": [\"state\", \"events?since=\", "
                        "\"wait?for=\", \"ui\", \"tap\", \"swipe\", \"text\", \"peek\", \"textdata\"]}");
    char head[256];
    int hn = snprintf(head, sizeof(head), "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n"
                      "Connection: close\r\n\r\n", strlen(body));
    send(s, head, hn, 0);
    send(s, body, (int)strlen(body), 0);
    free(body);
    shutdown(s, SD_SEND);
    closesocket(s);
    return 0;
}

static DWORD WINAPI server(LPVOID unused) {
    SOCKET ls = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = {0};
    a.sin_family = AF_INET;
    a.sin_port = htons(CONTROL_PORT);
    a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    BOOL yes = TRUE;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));
    if (bind(ls, (struct sockaddr *)&a, sizeof(a)) || listen(ls, 16)) {
        hlog("!! control: cannot listen on 127.0.0.1:%d", CONTROL_PORT);
        return 0;
    }
    for (;;) {
        SOCKET s = accept(ls, NULL, NULL);
        if (s == INVALID_SOCKET) continue;
        CloseHandle(CreateThread(NULL, 0, client, (LPVOID)(uintptr_t)s, 0, NULL));
    }
}

void ctl_init(void) {
    InitializeCriticalSection(&ev_lock);
    InitializeConditionVariable(&ev_cv);
    InitializeCriticalSection(&in_lock);
    InitializeCriticalSection(&req_lock);
    char exe[MAX_PATH];
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    snprintf(events_path, sizeof(events_path), "%s/../emu_events.jsonl", fs_root);
    FILE *f = fopen(events_path, "wb");
    if (f) fclose(f);
    ready = 1;
    CloseHandle(CreateThread(NULL, 0, server, NULL, 0, NULL));
    hlog("control: http://127.0.0.1:%d  (python -m recomp.emu.ctl ...)", CONTROL_PORT);
}
