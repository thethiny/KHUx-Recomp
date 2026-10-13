/* Fake JavaVM / JNIEnv and the Java side the game talks to (as emu/jni.py and emu/android.py): Cocos2dxHelper
 * (preferences, paths, sound stubs), the video helper, ClassLoader, ApplicationInfo, UUID. Java objects live in
 * the host behind handles that are unmapped guest addresses, so a guest dereference faults loudly. Unknown Java
 * methods are logged once and return a default, as on the emulator. */
#include "host.h"
#include <math.h>

uint32_t jni_env, jni_vm;
int screen_w = 1136, screen_h = 640;

/* ---------------------------------------------------------------- objects */
enum { J_CLASS = 1, J_STRING, J_OBJECT, J_ARRAY };
typedef struct JField { char name[32]; char type; int64_t j; uint32_t h; } JField;
typedef struct JObj {
    int kind;
    char *cls;               /* class name (class, object) */
    char *str;               /* UTF-8 (string) */
    char et;                 /* array element type */
    uint32_t n;              /* array length */
    uint8_t *data;           /* primitive array elements */
    uint32_t *refs;          /* object array elements (handles) */
    int nf;
    JField f[8];
} JObj;

#define JOBJ_CAP (1u << 22)
static JObj **objs;
static volatile LONG n_objs;
static SRWLOCK class_lock = SRWLOCK_INIT;

static uint32_t jnew(JObj *o) {
    LONG k = InterlockedIncrement(&n_objs) - 1;
    if ((uint32_t)k >= JOBJ_CAP) fatal("JNI: too many objects");
    objs[k] = o;
    return JHANDLE_BASE + 4u * (uint32_t)k;
}

static JObj *jget(uint32_t h) {
    if (!h) return NULL;
    uint32_t k = (h - JHANDLE_BASE) / 4;
    if (h < JHANDLE_BASE || k >= (uint32_t)n_objs || !objs[k]) fatal("JNI: unknown object handle 0x%08X", h);
    return objs[k];
}

uint32_t jclass_ref(const char *name) {
    static struct { char *name; uint32_t h; } classes[256];
    static int n;
    AcquireSRWLockExclusive(&class_lock);
    for (int k = 0; k < n; k++)
        if (!strcmp(classes[k].name, name)) { uint32_t h = classes[k].h; ReleaseSRWLockExclusive(&class_lock); return h; }
    if (n >= 256) fatal("JNI: too many classes");
    JObj *o = calloc(1, sizeof(JObj));
    o->kind = J_CLASS;
    o->cls = _strdup(name);
    classes[n].name = o->cls;
    classes[n].h = jnew(o);
    uint32_t h = classes[n++].h;
    ReleaseSRWLockExclusive(&class_lock);
    return h;
}

uint32_t jstring_new(const char *utf8) {
    JObj *o = calloc(1, sizeof(JObj));
    o->kind = J_STRING;
    o->str = _strdup(utf8 ? utf8 : "");
    return jnew(o);
}

static uint32_t jobject_new(const char *cls) {
    JObj *o = calloc(1, sizeof(JObj));
    o->kind = J_OBJECT;
    o->cls = _strdup(cls);
    return jnew(o);
}

static void jfield_set(uint32_t h, const char *name, char type, int64_t j, uint32_t ref) {
    JObj *o = jget(h);
    if (o->nf >= 8) fatal("JNI: too many fields");
    JField *f = &o->f[o->nf++];
    strncpy(f->name, name, 31);
    f->type = type;
    f->j = j;
    f->h = ref;
}

static JField *jfield(JObj *o, const char *name) {
    for (int k = 0; o && k < o->nf; k++) if (!strcmp(o->f[k].name, name)) return &o->f[k];
    return NULL;
}

static int elem_size(char t) {
    switch (t) { case 'Z': case 'B': return 1; case 'C': case 'S': return 2; case 'J': case 'D': return 8; default: return 4; }
}

uint32_t jarray_new(char type, uint32_t n, const void *data) {
    JObj *o = calloc(1, sizeof(JObj));
    o->kind = J_ARRAY;
    o->et = type;
    o->n = n;
    if (type == 'L') o->refs = calloc(n ? n : 1, 4);
    else {
        o->data = calloc(n ? n : 1, (size_t)elem_size(type));
        if (data) memcpy(o->data, data, (size_t)n * (size_t)elem_size(type));
    }
    return jnew(o);
}

static const char *class_of(JObj *o) {
    if (!o) return "java/lang/Object";
    if (o->kind == J_STRING) return "java/lang/String";
    if (o->kind == J_OBJECT || o->kind == J_CLASS) return o->cls;
    return "java/lang/Object";
}

/* ---------------------------------------------------------------- method / field ids */
typedef struct JMethod { char *cls, *name, *sig; int is_static; } JMethod;
static JMethod *mids;
static volatile LONG n_mids;
static SRWLOCK mid_lock = SRWLOCK_INIT;

static uint32_t mid_new(const char *cls, const char *name, const char *sig, int is_static) {
    AcquireSRWLockExclusive(&mid_lock);
    for (LONG k = 0; k < n_mids; k++)
        if (!strcmp(mids[k].cls, cls) && !strcmp(mids[k].name, name) && !strcmp(mids[k].sig, sig) &&
            mids[k].is_static == is_static) {
            ReleaseSRWLockExclusive(&mid_lock);
            return JID_BASE + 4u * (uint32_t)k;
        }
    LONG k = n_mids;
    if (k >= 65536) fatal("JNI: too many method ids");
    mids[k].cls = _strdup(cls);
    mids[k].name = _strdup(name);
    mids[k].sig = _strdup(sig);
    mids[k].is_static = is_static;
    n_mids = k + 1;
    ReleaseSRWLockExclusive(&mid_lock);
    return JID_BASE + 4u * (uint32_t)k;
}

static JMethod *mid_get(uint32_t id) {
    uint32_t k = (id - JID_BASE) / 4;
    if (id < JID_BASE || k >= (uint32_t)n_mids) fatal("JNI: unknown method/field id 0x%08X", id);
    return &mids[k];
}

/* signature: parameter type characters ('L' for any object or array) and the return type */
static int parse_sig(const char *sig, char *params, int cap, char *ret) {
    int n = 0;
    const char *p = sig + 1;
    while (*p && *p != ')') {
        char t = *p;
        while (*p == '[') p++;
        if (*p == 'L') p = strchr(p, ';');
        if (t == '[') t = 'L';
        if (n < cap) params[n++] = t;
        p++;
    }
    *ret = p[1] == '[' ? 'L' : p[1];
    return n;
}

/* ---------------------------------------------------------------- Java values and the Java side */
typedef union JV { int64_t j; double d; uint32_t h; } JV;

typedef JV (*JavaFn)(JObj *self, JV *a, int n);
typedef struct JavaImpl { const char *key; JavaFn fn; } JavaImpl;

static JV jv_h(uint32_t h) { JV v; v.j = 0; v.h = h; return v; }
static JV jv_j(int64_t j) { JV v; v.j = j; return v; }
static JV jv_d(double d) { JV v; v.d = d; return v; }
static const char *jv_str(JV v) { JObj *o = jget(v.h); return o && o->kind == J_STRING ? o->str : NULL; }

/* preferences: the emulator's JSON file (flat object of strings, numbers, booleans) */
typedef struct Pref { char *key; char type; char *s; double d; } Pref;     /* type: s, i, f, b, n */
static Pref *prefs;
static int n_prefs;
static char prefs_path[1100];
static SRWLOCK prefs_lock = SRWLOCK_INIT;

static const char *json_str(const char *p, char **out) {
    size_t cap = 64, n = 0;
    char *s = malloc(cap);
    p++;
    while (*p && *p != '"') {
        if (n + 8 > cap) s = realloc(s, cap *= 2);
        if (*p == '\\') {
            p++;
            switch (*p) {
            case 'n': s[n++] = '\n'; break;
            case 't': s[n++] = '\t'; break;
            case 'r': s[n++] = '\r'; break;
            case 'b': s[n++] = '\b'; break;
            case 'f': s[n++] = '\f'; break;
            case 'u': {
                unsigned u = 0;
                sscanf(p + 1, "%4x", &u);
                p += 4;
                if (u >= 0xD800 && u < 0xDC00 && p[1] == '\\' && p[2] == 'u') {
                    unsigned lo = 0;
                    sscanf(p + 3, "%4x", &lo);
                    u = 0x10000 + ((u - 0xD800) << 10) + (lo - 0xDC00);
                    p += 6;
                }
                if (u < 0x80) s[n++] = (char)u;
                else if (u < 0x800) { s[n++] = (char)(0xC0 | (u >> 6)); s[n++] = (char)(0x80 | (u & 0x3F)); }
                else if (u < 0x10000) {
                    s[n++] = (char)(0xE0 | (u >> 12)); s[n++] = (char)(0x80 | ((u >> 6) & 0x3F));
                    s[n++] = (char)(0x80 | (u & 0x3F));
                } else {
                    s[n++] = (char)(0xF0 | (u >> 18)); s[n++] = (char)(0x80 | ((u >> 12) & 0x3F));
                    s[n++] = (char)(0x80 | ((u >> 6) & 0x3F)); s[n++] = (char)(0x80 | (u & 0x3F));
                }
                break;
            }
            default: s[n++] = *p; break;
            }
            p++;
        } else {
            s[n++] = *p++;
        }
    }
    s[n] = 0;
    *out = s;
    return *p ? p + 1 : p;
}

static void prefs_load(void) {
    FILE *f = fopen(prefs_path, "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = calloc(1, (size_t)size + 1);
    fread(buf, 1, (size_t)size, f);
    fclose(f);
    const char *p = buf;
    while (*p && *p != '{') p++;
    if (*p) p++;
    for (;;) {
        while (*p && *p != '"' && *p != '}') p++;
        if (*p != '"') break;
        char *key;
        p = json_str(p, &key);
        while (*p == ' ' || *p == ':' || *p == '\n' || *p == '\r' || *p == '\t') p++;
        Pref pr = {key, 'n', NULL, 0};
        if (*p == '"') { pr.type = 's'; p = json_str(p, &pr.s); }
        else if (!strncmp(p, "true", 4)) { pr.type = 'b'; pr.d = 1; p += 4; }
        else if (!strncmp(p, "false", 5)) { pr.type = 'b'; pr.d = 0; p += 5; }
        else if (!strncmp(p, "null", 4)) { pr.type = 'n'; p += 4; }
        else {
            char *end;
            pr.d = strtod(p, &end);
            int is_float = 0;
            for (const char *q = p; q < end; q++) if (*q == '.' || *q == 'e' || *q == 'E') is_float = 1;
            pr.type = is_float ? 'f' : 'i';
            p = end;
        }
        prefs = realloc(prefs, sizeof(Pref) * (size_t)(n_prefs + 1));
        prefs[n_prefs++] = pr;
    }
    free(buf);
}

static void json_put_str(FILE *f, const char *s) {
    fputc('"', f);
    for (; *s; s++) {
        unsigned char ch = (unsigned char)*s;
        if (ch == '"') fputs("\\\"", f);
        else if (ch == '\\') fputs("\\\\", f);
        else if (ch == '\n') fputs("\\n", f);
        else if (ch == '\r') fputs("\\r", f);
        else if (ch == '\t') fputs("\\t", f);
        else if (ch < 0x20) fprintf(f, "\\u%04x", ch);
        else fputc(ch, f);
    }
    fputc('"', f);
}

static void prefs_save(void) {
    char tmp[1200];
    snprintf(tmp, sizeof(tmp), "%s.tmp", prefs_path);
    FILE *f = fopen(tmp, "wb");
    if (!f) { hlog("!! cannot write %s", tmp); return; }
    fputs("{", f);
    for (int k = 0; k < n_prefs; k++) {
        fputs(k ? ",\n " : "\n ", f);
        json_put_str(f, prefs[k].key);
        fputs(": ", f);
        switch (prefs[k].type) {
        case 's': json_put_str(f, prefs[k].s); break;
        case 'b': fputs(prefs[k].d ? "true" : "false", f); break;
        case 'i': fprintf(f, "%lld", (long long)prefs[k].d); break;
        case 'f': {
            char b[64];
            snprintf(b, sizeof(b), "%.17g", prefs[k].d);
            if (!strpbrk(b, ".eEn")) strcat(b, ".0");
            fputs(b, f);
            break;
        }
        default: fputs("null", f); break;
        }
    }
    fputs("\n}", f);
    fclose(f);
    MoveFileExA(tmp, prefs_path, MOVEFILE_REPLACE_EXISTING);
}

static Pref *pref_find(const char *key) {
    for (int k = 0; k < n_prefs; k++) if (!strcmp(prefs[k].key, key)) return &prefs[k];
    return NULL;
}

static void pref_set(const char *key, char type, const char *s, double d) {
    AcquireSRWLockExclusive(&prefs_lock);
    Pref *p = pref_find(key);
    if (!p) {
        prefs = realloc(prefs, sizeof(Pref) * (size_t)(n_prefs + 1));
        p = &prefs[n_prefs++];
        p->key = _strdup(key);
        p->s = NULL;
    }
    free(p->s);
    p->type = type;
    p->s = s ? _strdup(s) : NULL;
    p->d = d;
    prefs_save();
    ReleaseSRWLockExclusive(&prefs_lock);
}

#define HELPER "org/cocos2dx/lib/Cocos2dxHelper"
#define VIDEO "org/cocos2dx/lib/Cocos2dxVideoHelper"

static JV j_null(JObj *s, JV *a, int n) { return jv_h(0); }
static JV j_zero(JObj *s, JV *a, int n) { return jv_j(0); }
static JV j_one(JObj *s, JV *a, int n) { return jv_j(1); }
static JV j_onef(JObj *s, JV *a, int n) { return jv_d(1.0); }
static JV j_package(JObj *s, JV *a, int n) { return jv_h(jstring_new(PACKAGE)); }
static JV j_writable(JObj *s, JV *a, int n) { return jv_h(jstring_new(FILES_DIR)); }
static JV j_lang(JObj *s, JV *a, int n) { return jv_h(jstring_new("en")); }
static JV j_model(JObj *s, JV *a, int n) { return jv_h(jstring_new("Android Emulator")); }
static JV j_dpi(JObj *s, JV *a, int n) { return jv_j(320); }
static JV j_version(JObj *s, JV *a, int n) { return jv_h(jstring_new("1.0.1")); }
static JV j_assets_path(JObj *s, JV *a, int n) { return jv_h(jstring_new(APK_DEVICE_PATH)); }

/* get<T>ForKey(key, default): the stored value, else the default (a[1]) */
static JV pref_get(JV *a, int n, char want) {
    const char *key = jv_str(a[0]);
    AcquireSRWLockShared(&prefs_lock);
    Pref *p = key ? pref_find(key) : NULL;
    JV r;
    if (!p || p->type == 'n') r = n > 1 ? a[1] : jv_j(0);
    else if (want == 'S') r = jv_h(p->type == 's' ? jstring_new(p->s) : 0);
    else if (want == 'D') r = jv_d(p->type == 's' ? atof(p->s) : p->d);
    else r = jv_j(p->type == 's' ? atoll(p->s) : (int64_t)p->d);
    ReleaseSRWLockShared(&prefs_lock);
    return r;
}
static JV j_get_bool(JObj *s, JV *a, int n) { return pref_get(a, n, 'I'); }
static JV j_get_int(JObj *s, JV *a, int n) { return pref_get(a, n, 'I'); }
static JV j_get_float(JObj *s, JV *a, int n) { return pref_get(a, n, 'D'); }
static JV j_get_string(JObj *s, JV *a, int n) { return pref_get(a, n, 'S'); }
static JV j_set_bool(JObj *s, JV *a, int n) { pref_set(jv_str(a[0]), 'b', NULL, a[1].j ? 1 : 0); return jv_j(0); }
static JV j_set_int(JObj *s, JV *a, int n) { pref_set(jv_str(a[0]), 'i', NULL, (double)(int32_t)a[1].j); return jv_j(0); }
static JV j_set_float(JObj *s, JV *a, int n) { pref_set(jv_str(a[0]), 'f', NULL, a[1].d); return jv_j(0); }
static JV j_set_string(JObj *s, JV *a, int n) {
    const char *v = jv_str(a[1]);
    pref_set(jv_str(a[0]), v ? 's' : 'n', v, 0);
    return jv_j(0);
}
static JV j_del_key(JObj *s, JV *a, int n) {
    const char *key = jv_str(a[0]);
    AcquireSRWLockExclusive(&prefs_lock);
    for (int k = 0; key && k < n_prefs; k++)
        if (!strcmp(prefs[k].key, key)) {
            memmove(&prefs[k], &prefs[k + 1], sizeof(Pref) * (size_t)(n_prefs - k - 1));
            n_prefs--;
            prefs_save();
            break;
        }
    ReleaseSRWLockExclusive(&prefs_lock);
    return jv_j(0);
}

static JV j_class_loader(JObj *s, JV *a, int n) { return jv_h(jobject_new("java/lang/ClassLoader")); }
static JV j_load_class(JObj *s, JV *a, int n) {
    char name[256];
    strncpy(name, jv_str(a[0]) ? jv_str(a[0]) : "", 255);
    name[255] = 0;
    for (char *p = name; *p; p++) if (*p == '.') *p = '/';
    return jv_h(jclass_ref(name));
}
static JV j_app_info(JObj *s, JV *a, int n) {
    uint32_t o = jobject_new("android/content/pm/ApplicationInfo");
    jfield_set(o, "sourceDir", 'L', 0, jstring_new(APK_DEVICE_PATH));
    jfield_set(o, "dataDir", 'L', 0, jstring_new("/data/data/" PACKAGE));
    jfield_set(o, "packageName", 'L', 0, jstring_new(PACKAGE));
    jfield_set(o, "nativeLibraryDir", 'L', 0, jstring_new("/data/app/" PACKAGE "-1/lib/arm"));
    return jv_h(o);
}
static JV j_uuid(JObj *s, JV *a, int n) {
    uint32_t o = jobject_new("java/util/UUID");
    uint64_t hi = ((uint64_t)rand() << 48) ^ ((uint64_t)rand() << 32) ^ ((uint64_t)GetTickCount64() << 8) ^ (uint64_t)rand();
    uint64_t lo = ((uint64_t)rand() << 48) ^ ((uint64_t)rand() << 24) ^ (uint64_t)rand() ^ (uint64_t)GetCurrentThreadId();
    hi = (hi & ~0xF000ULL) | 0x4000ULL;
    lo = (lo & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;
    jfield_set(o, "hi", 'J', (int64_t)hi, 0);
    jfield_set(o, "lo", 'J', (int64_t)lo, 0);
    return jv_h(o);
}
static JV j_uuid_hi(JObj *s, JV *a, int n) { JField *f = jfield(s, "hi"); return jv_j(f ? f->j : 0); }
static JV j_uuid_lo(JObj *s, JV *a, int n) { JField *f = jfield(s, "lo"); return jv_j(f ? f->j : 0); }
static JV j_uuid_str(JObj *s, JV *a, int n) {
    JField *h = jfield(s, "hi"), *l = jfield(s, "lo");
    uint64_t hi = h ? (uint64_t)h->j : 0, lo = l ? (uint64_t)l->j : 0;
    char b[40];
    snprintf(b, sizeof(b), "%08x-%04x-%04x-%04x-%012llx", (unsigned)(hi >> 32), (unsigned)(hi >> 16) & 0xFFFF,
             (unsigned)hi & 0xFFFF, (unsigned)(lo >> 48), (unsigned long long)(lo & 0xFFFFFFFFFFFFULL));
    return jv_h(jstring_new(b));
}

/* video: no decoding; playback completes on the next frame */
static volatile LONG next_video;
static int video_events[64], n_video_events;
static SRWLOCK video_lock = SRWLOCK_INIT;
static JV j_video_create(JObj *s, JV *a, int n) { return jv_j(InterlockedIncrement(&next_video) - 1); }
static JV j_video_start(JObj *s, JV *a, int n) {
    AcquireSRWLockExclusive(&video_lock);
    if (n_video_events < 64) video_events[n_video_events++] = (int)a[0].j;
    ReleaseSRWLockExclusive(&video_lock);
    return jv_j(0);
}

void android_video_poll(CPU *c) {
    static uint32_t cb, cls;
    if (!cb) {
        cb = sym("Java_org_cocos2dx_lib_Cocos2dxVideoHelper_nativeExecuteVideoCallback");
        cls = jclass_ref(VIDEO);
    }
    for (;;) {
        AcquireSRWLockExclusive(&video_lock);
        int idx = -1;
        if (n_video_events) {
            idx = video_events[0];
            memmove(video_events, video_events + 1, sizeof(int) * (size_t)--n_video_events);
        }
        ReleaseSRWLockExclusive(&video_lock);
        if (idx < 0) return;
        hlog("  video %d: event 3 (completed)", idx);
        GCALL(c, cb, jni_env, cls, (uint32_t)idx, 3);
    }
}

/* text input dialog (ui::EditBox): a Windows input box (gl.c), or the control channel's `text`. The answer goes back
 * through nativeSetEditTextDialogResult(byte[] utf8) on the main thread (main loop / ctl), as Android's dialog does. */
volatile LONG edit_dialog_open;
static JV j_edit_dialog(JObj *s, JV *a, int n) {
    /* (String title, byte[] message (current text, UTF-8), inputMode, inputFlag, returnType, maxLength) */
    JObj *msg = jget(a[1].h);
    char cur[1024] = "";
    if (msg && msg->n) {
        uint32_t k = msg->n < sizeof(cur) - 1 ? msg->n : (uint32_t)sizeof(cur) - 1;
        memcpy(cur, msg->data, k);
        cur[k] = 0;
    }
    const char *title = jv_str(a[0]);
    hlog("  edit dialog opened (current=%s, max=%d)", cur, (int)a[5].j);
    InterlockedExchange(&edit_dialog_open, 1);
    edit_box_open(title ? title : "", cur, (int)a[5].j, (int)a[3].j == 0 /* kEditBoxInputFlagPassword */);
    return jv_j(0);
}

/* the answer (from the input box or the control channel): main thread only */
void edit_dialog_answer(CPU *c, const char *utf8) {
    InterlockedExchange(&edit_dialog_open, 0);
    uint32_t arr = jarray_new('B', (uint32_t)strlen(utf8), utf8);
    GCALL(c, sym("Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetEditTextDialogResult"), jni_env,
          jclass_ref("org/cocos2dx/lib/Cocos2dxHelper"), arr);
    hlog("  edit dialog answered");
}

static JV j_font_size(JObj *s, JV *a, int n) { int32_t v = (int32_t)(a[0].j * 3 / 4); return jv_j(v < 1 ? 1 : v); }

/* Cocos2dxBitmap.createTextBitmapShadowStroke: labels in a system font. Java renders the text into a bitmap and hands
 * the premultiplied RGBA pixels to nativeInitBitmapDC(width, height, byte[]); here GDI renders it. Alignment: low
 * nibble horizontal (1 left, 2 right, 3 centre), high nibble vertical (1 top, 2 bottom, 3 centre). */
static JV j_text_bitmap(JObj *s, JV *a, int n) {
    const char *text = jv_str(a[0]);
    int size = (int)a[2].j, align = (int)a[6].j, want_w = (int)a[7].j, want_h = (int)a[8].j;
    float fr = (float)a[3].d, fg = (float)a[4].d, fb = (float)a[5].d;
    int stroke = (int)a[14].j && a[18].d > 0;
    float sr = (float)a[15].d, sg = (float)a[16].d, sb = (float)a[17].d;
    int ssize = stroke ? (int)(a[18].d + 0.5) : 0;
    if (!text || size <= 0) return jv_j(0);
    int w, h;
    uint8_t *rgba = text_render(text, size, align, want_w, want_h, fr, fg, fb, ssize, sr, sg, sb, &w, &h);
    if (!rgba) return jv_j(0);
    uint32_t arr = jarray_new('B', (uint32_t)(w * h * 4), rgba);
    free(rgba);
    GCALL(cur->cpu, sym("Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeInitBitmapDC"), jni_env,
          jclass_ref("org/cocos2dx/lib/Cocos2dxBitmap"), (uint32_t)w, (uint32_t)h, arr);
    return jv_j(1);
}

#define WEBVIEW "org/cocos2dx/lib/Cocos2dxWebViewHelper"
static JV w_create(JObj *s, JV *a, int n) { return jv_j(webview_create()); }
static JV w_remove(JObj *s, JV *a, int n) { webview_remove(a[0].j); return jv_j(0); }
static JV w_visible(JObj *s, JV *a, int n) { webview_visible(a[0].j, (int)a[1].j); return jv_j(0); }
static JV w_rect(JObj *s, JV *a, int n) {
    webview_rect(a[0].j, (int)a[1].j, (int)a[2].j, (int)a[3].j, (int)a[4].j);
    return jv_j(0);
}
static JV w_scheme(JObj *s, JV *a, int n) { webview_scheme(a[0].j, jv_str(a[1])); return jv_j(0); }
static JV w_url(JObj *s, JV *a, int n) { webview_load_url(a[0].j, jv_str(a[1])); return jv_j(0); }
static JV w_html(JObj *s, JV *a, int n) { webview_load_html(a[0].j, jv_str(a[1])); return jv_j(0); }
static JV w_file(JObj *s, JV *a, int n) { hlog("  webview loadFile(%s): not supported", jv_str(a[1])); return jv_j(0); }
static JV w_js(JObj *s, JV *a, int n) { webview_js(a[0].j, jv_str(a[1])); return jv_j(0); }
static JV w_back(JObj *s, JV *a, int n) { webview_nav(a[0].j, 0); return jv_j(0); }
static JV w_forward(JObj *s, JV *a, int n) { webview_nav(a[0].j, 1); return jv_j(0); }
static JV w_reload(JObj *s, JV *a, int n) { webview_nav(a[0].j, 2); return jv_j(0); }
static JV w_stop(JObj *s, JV *a, int n) { webview_nav(a[0].j, 3); return jv_j(0); }
static JV w_can_back(JObj *s, JV *a, int n) { return jv_j(webview_nav(a[0].j, 4)); }
static JV w_can_forward(JObj *s, JV *a, int n) { return jv_j(webview_nav(a[0].j, 5)); }

static const JavaImpl JAVA[] = {
    {WEBVIEW ".createWebView", w_create},
    {WEBVIEW ".removeWebView", w_remove},
    {WEBVIEW ".setVisible", w_visible},
    {WEBVIEW ".setWebViewRect", w_rect},
    {WEBVIEW ".setJavascriptInterfaceScheme", w_scheme},
    {WEBVIEW ".loadUrl", w_url},
    {WEBVIEW ".loadHTMLString", w_html},
    {WEBVIEW ".loadData", w_html},
    {WEBVIEW ".loadFile", w_file},
    {WEBVIEW ".evaluateJS", w_js},
    {WEBVIEW ".goBack", w_back},
    {WEBVIEW ".goForward", w_forward},
    {WEBVIEW ".reload", w_reload},
    {WEBVIEW ".stopLoading", w_stop},
    {WEBVIEW ".canGoBack", w_can_back},
    {WEBVIEW ".canGoForward", w_can_forward},
    {HELPER ".getCocos2dxPackageName", j_package},
    {HELPER ".getCocos2dxWritablePath", j_writable},
    {HELPER ".getCurrentLanguage", j_lang},
    {HELPER ".getDeviceModel", j_model},
    {HELPER ".getDPI", j_dpi},
    {HELPER ".getBoolForKey", j_get_bool},
    {HELPER ".getIntegerForKey", j_get_int},
    {HELPER ".getFloatForKey", j_get_float},
    {HELPER ".getDoubleForKey", j_get_float},
    {HELPER ".getStringForKey", j_get_string},
    {HELPER ".setBoolForKey", j_set_bool},
    {HELPER ".setIntegerForKey", j_set_int},
    {HELPER ".setFloatForKey", j_set_float},
    {HELPER ".setDoubleForKey", j_set_float},
    {HELPER ".setStringForKey", j_set_string},
    {HELPER ".deleteValueForKey", j_del_key},
    {HELPER ".isBackgroundMusicPlaying", j_zero},
    {HELPER ".getBackgroundMusicVolume", j_onef},
    {HELPER ".getEffectsVolume", j_onef},
    {HELPER ".playEffect", j_one},
    {HELPER ".openURL", j_zero},
    {HELPER ".getVersion", j_version},
    {HELPER ".getAssetsPath", j_assets_path},
    {HELPER ".showEditTextDialog", j_edit_dialog},
    {"android/content/Context.getClassLoader", j_class_loader},
    {"android/app/Activity.getClassLoader", j_class_loader},
    {"java/lang/ClassLoader.loadClass", j_load_class},
    {"android/content/Context.getApplicationInfo", j_app_info},
    {"android/app/Activity.getApplicationInfo", j_app_info},
    {"java/util/UUID.randomUUID", j_uuid},
    {"java/util/UUID.getMostSignificantBits", j_uuid_hi},
    {"java/util/UUID.getLeastSignificantBits", j_uuid_lo},
    {"java/util/UUID.toString", j_uuid_str},
    {VIDEO ".createVideoWidget", j_video_create},
    {VIDEO ".startVideo", j_video_start},
    {"org/cocos2dx/lib/Cocos2dxBitmap.getFontSizeAccordingHeight", j_font_size},
    {"org/cocos2dx/lib/Cocos2dxBitmap.createTextBitmapShadowStroke", j_text_bitmap},
};
static const char *JAVA_NOOPS[] = {
    HELPER ".setKeepScreenOn", HELPER ".vibrate", HELPER ".preloadBackgroundMusic", HELPER ".playBackgroundMusic",
    HELPER ".stopBackgroundMusic", HELPER ".pauseBackgroundMusic", HELPER ".resumeBackgroundMusic",
    HELPER ".rewindBackgroundMusic", HELPER ".end", HELPER ".pauseAllEffects", HELPER ".resumeAllEffects",
    HELPER ".stopAllEffects", HELPER ".stopEffect", HELPER ".pauseEffect", HELPER ".resumeEffect",
    HELPER ".unloadEffect", HELPER ".preloadEffect", HELPER ".setBackgroundMusicVolume", HELPER ".setEffectsVolume",
    HELPER ".enableAccelerometer", HELPER ".disableAccelerometer", HELPER ".setAccelerometerInterval",
    HELPER ".terminateProcess", HELPER ".setEditTextDialogResult",
    VIDEO ".setVideoUrl", VIDEO ".setVideoRect", VIDEO ".removeVideoWidget", VIDEO ".pauseVideo", VIDEO ".resumeVideo",
    VIDEO ".stopVideo", VIDEO ".seekVideoTo", VIDEO ".setVideoVisible", VIDEO ".setVideoKeepRatioEnabled",
    VIDEO ".setFullScreenEnabled", WEBVIEW ".setScalesPageToFit",
};

static JV java_call(JMethod *m, JObj *self, JV *a, int n, char ret) {
    char key[512];
    snprintf(key, sizeof(key), "%s.%s", m->cls, m->name);
    for (int k = 0; k < (int)(sizeof(JAVA) / sizeof(*JAVA)); k++)
        if (!strcmp(JAVA[k].key, key)) {
            JV r = JAVA[k].fn(self, a, n);
            hlog("  java %s%s", key, m->sig);
            return r;
        }
    for (int k = 0; k < (int)(sizeof(JAVA_NOOPS) / sizeof(*JAVA_NOOPS)); k++)
        if (!strcmp(JAVA_NOOPS[k], key)) return jv_j(0);
    static char *seen[256];
    static int n_seen;
    int known = 0;
    for (int k = 0; k < n_seen; k++) if (!strcmp(seen[k], key)) known = 1;
    if (!known) {
        if (n_seen < 256) seen[n_seen++] = _strdup(key);
        hlog("  ?? java %s%s -> default", key, m->sig);
    }
    if (ret == 'L' && !strcmp(strchr(m->sig, ')') + 1, "Ljava/lang/String;")) return jv_h(jstring_new(""));
    return jv_j(0);
}

/* ---------------------------------------------------------------- argument marshalling */
static int read_args(CPU *c, const char *params, int n, VA *va, uint32_t jvalues, JV *out) {
    for (int k = 0; k < n; k++) {
        char t = params[k];
        if (jvalues) {
            uint32_t p = jvalues + 8u * (uint32_t)k;
            if (t == 'J') out[k].j = *(int64_t *)GP(p);
            else if (t == 'F') out[k].d = *(float *)GP(p);
            else if (t == 'D') out[k].d = *(double *)GP(p);
            else if (t == 'L') out[k] = jv_h(g32(p));
            else out[k].j = (int32_t)g32(p);
        } else {
            if (t == 'J') out[k].j = (int64_t)va_u64(va);
            else if (t == 'F' || t == 'D') out[k].d = va_f64(va);       /* float promoted to double */
            else if (t == 'L') out[k] = jv_h(va_u32(va));
            else out[k].j = (int32_t)va_u32(va);
        }
    }
    return n;
}

static void put_ret(CPU *c, char ret, JV v) {
    switch (ret) {
    case 'V': RET(c, 0); break;
    case 'J': RET64(c, (uint64_t)v.j); break;
    case 'F': RET(c, F2U((float)v.d)); break;
    case 'D': RET64(c, D2U(v.d)); break;
    case 'L': RET(c, v.h); break;
    default: RET(c, (uint32_t)v.j); break;
    }
}

/* Call<T>Method[V|A], CallStatic..., CallNonvirtual...: kind 0 instance, 1 static, 2 nonvirtual */
static void invoke(CPU *c, int kind, int variant) {
    int shift = kind == 2 ? 1 : 0;
    uint32_t target = c->r[1], mid = c->r[2 + shift];
    JMethod *m = mid_get(mid);
    char params[32], ret;
    int n = parse_sig(m->sig, params, 32, &ret);
    JV a[32] = {0};
    if (variant == 0) { VA va = va_regs(c, 3 + shift); read_args(c, params, n, &va, 0, a); }
    else if (variant == 1) { VA va = va_list_at(c, ARG(c, 3 + shift)); read_args(c, params, n, &va, 0, a); }
    else read_args(c, params, n, NULL, ARG(c, 3 + shift), a);
    JObj *self = kind == 1 ? NULL : jget(target);
    put_ret(c, ret, java_call(m, self, a, n, ret));
}
static void jn_call(CPU *c) { invoke(c, 0, 0); }
static void jn_callv(CPU *c) { invoke(c, 0, 1); }
static void jn_calla(CPU *c) { invoke(c, 0, 2); }
static void jn_scall(CPU *c) { invoke(c, 1, 0); }
static void jn_scallv(CPU *c) { invoke(c, 1, 1); }
static void jn_scalla(CPU *c) { invoke(c, 1, 2); }
static void jn_ncall(CPU *c) { invoke(c, 2, 0); }
static void jn_ncallv(CPU *c) { invoke(c, 2, 1); }
static void jn_ncalla(CPU *c) { invoke(c, 2, 2); }

/* ---------------------------------------------------------------- JNIEnv functions */
static void jn_unimplemented(CPU *c) {
    fatal("JNI %s not implemented (lr=0x%08X)", import_name(imp_target), c->r[14]);
}
static void jn_version(CPU *c) { RET(c, 0x10006); }
static void jn_find_class(CPU *c) { RET(c, jclass_ref(gstr(c->r[1]))); }
static void jn_object_class(CPU *c) { RET(c, jclass_ref(class_of(jget(c->r[1])))); }
static void jn_superclass(CPU *c) { RET(c, jclass_ref("java/lang/Object")); }
static void jn_true(CPU *c) { RET(c, 1); }
static void jn_zero(CPU *c) { RET(c, 0); }
static void jn_arg1(CPU *c) { RET(c, c->r[1]); }
static void jn_get_mid(CPU *c) { RET(c, mid_new(jget(c->r[1])->cls, gstr(c->r[2]), gstr(c->r[3]), 0)); }
static void jn_get_smid(CPU *c) { RET(c, mid_new(jget(c->r[1])->cls, gstr(c->r[2]), gstr(c->r[3]), 1)); }
static void jn_get_static_field(CPU *c) {
    JMethod *f = mid_get(c->r[2]);
    hlog("  java static field %s.%s:%s -> null", f->cls, f->name, f->sig);
    RET(c, 0);
}
static void jn_get_int_field(CPU *c) {
    JField *f = jfield(jget(c->r[1]), mid_get(c->r[2])->name);
    RET(c, f ? (uint32_t)f->j : 0);
}
static void jn_get_object_field(CPU *c) {
    JField *f = jfield(jget(c->r[1]), mid_get(c->r[2])->name);
    RET(c, f ? f->h : 0);
}
static void jn_new_object(CPU *c) {
    JMethod *m = mid_get(c->r[2]);
    hlog("  java new %s%s", m->cls, m->sig);
    RET(c, jobject_new(m->cls));
}
static void jn_same(CPU *c) { RET(c, c->r[1] == c->r[2] || (c->r[1] && c->r[2] && jget(c->r[1]) == jget(c->r[2]))); }
static void jn_get_vm(CPU *c) { gw32(c->r[1], jni_vm); RET(c, 0); }
static void jn_register_natives(CPU *c) {
    for (uint32_t k = 0; k < c->r[3]; k++) {
        uint32_t m = c->r[2] + 12 * k;
        hlog("  RegisterNatives %s.%s%s -> 0x%08X", jget(c->r[1])->cls, gstr(g32(m)), gstr(g32(m + 4)), g32(m + 8));
    }
    RET(c, 0);
}
static void jn_new_string_utf(CPU *c) { RET(c, c->r[1] ? jstring_new(gstr(c->r[1])) : 0); }
static void jn_get_string_utf_chars(CPU *c) {
    if (c->r[2]) *(uint8_t *)GP(c->r[2]) = 1;
    RET(c, galloc_str(jget(c->r[1])->str));
}
static void jn_release_free2(CPU *c) { gfree(c->r[2]); RET(c, 0); }
static void jn_string_utf_length(CPU *c) { RET(c, (uint32_t)strlen(jget(c->r[1])->str)); }
static void jn_string_length(CPU *c) {
    RET(c, (uint32_t)MultiByteToWideChar(CP_UTF8, 0, jget(c->r[1])->str, -1, NULL, 0) - 1);
}
static void jn_get_string_chars(CPU *c) {
    const char *s = jget(c->r[1])->str;
    int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
    uint32_t p = gzalloc(2u * (uint32_t)n + 2);
    MultiByteToWideChar(CP_UTF8, 0, s, -1, GP(p), n);
    if (c->r[2]) *(uint8_t *)GP(c->r[2]) = 1;
    RET(c, p);
}
static void jn_new_string(CPU *c) {
    int n = (int)c->r[2];
    int len = WideCharToMultiByte(CP_UTF8, 0, GP(c->r[1]), n, NULL, 0, NULL, NULL);
    char *s = calloc(1, (size_t)len + 1);
    WideCharToMultiByte(CP_UTF8, 0, GP(c->r[1]), n, s, len, NULL, NULL);
    RET(c, jstring_new(s));
    free(s);
}
static void jn_get_string_utf_region(CPU *c) {
    /* (env, str, start, len, buf): start/len count characters; the game's strings are ASCII here */
    const char *s = jget(c->r[1])->str;
    uint32_t start = c->r[2], n = c->r[3], buf = ARG(c, 4), slen = (uint32_t)strlen(s);
    if (start > slen) start = slen;
    if (start + n > slen) n = slen - start;
    memcpy(GP(buf), s + start, n);
    *(char *)GP(buf + n) = 0;
    RET(c, 0);
}
static void jn_array_length(CPU *c) { RET(c, jget(c->r[1])->n); }
static void jn_new_object_array(CPU *c) {
    uint32_t h = jarray_new('L', c->r[1], NULL);
    JObj *o = jget(h);
    for (uint32_t k = 0; k < o->n; k++) o->refs[k] = c->r[3];
    RET(c, h);
}
static void jn_get_object_array_element(CPU *c) { RET(c, jget(c->r[1])->refs[c->r[2]]); }
static void jn_set_object_array_element(CPU *c) { jget(c->r[1])->refs[c->r[2]] = c->r[3]; RET(c, 0); }

static char array_type_of_import(void) {
    const char *n = import_name(imp_target);
    static const struct { const char *name; char t; } T[] = {{"Boolean", 'Z'}, {"Byte", 'B'}, {"Char", 'C'},
        {"Short", 'S'}, {"Int", 'I'}, {"Long", 'J'}, {"Float", 'F'}, {"Double", 'D'}};
    for (int k = 0; k < 8; k++) if (strstr(n, T[k].name)) return T[k].t;
    fatal("JNI: no array type in %s", n);
}
static void jn_new_prim_array(CPU *c) { RET(c, jarray_new(array_type_of_import(), c->r[1], NULL)); }
static void jn_get_array_elements(CPU *c) {
    JObj *o = jget(c->r[1]);
    uint32_t bytes = o->n * (uint32_t)elem_size(o->et);
    if (c->r[2]) *(uint8_t *)GP(c->r[2]) = 1;
    RET(c, galloc_bytes(o->data, bytes < 4 ? 4 : bytes));
}
static void jn_release_array_elements(CPU *c) {
    JObj *o = jget(c->r[1]);
    uint32_t mode = c->r[3];
    if (mode == 0 || mode == 1) memcpy(o->data, GP(c->r[2]), (size_t)o->n * (size_t)elem_size(o->et));
    if (mode == 0 || mode == 2) gfree(c->r[2]);
    RET(c, 0);
}
static void jn_get_array_region(CPU *c) {
    JObj *o = jget(c->r[1]);
    int es = elem_size(o->et);
    memcpy(GP(ARG(c, 4)), o->data + (size_t)c->r[2] * es, (size_t)c->r[3] * es);
    RET(c, 0);
}
static void jn_set_array_region(CPU *c) {
    JObj *o = jget(c->r[1]);
    int es = elem_size(o->et);
    memcpy(o->data + (size_t)c->r[2] * es, GP(ARG(c, 4)), (size_t)c->r[3] * es);
    RET(c, 0);
}

static const char *JNI_TYPES[] = {"Object", "Boolean", "Byte", "Char", "Short", "Int", "Long", "Float", "Double", "Void"};
static const char *PRIM_TYPES[] = {"Boolean", "Byte", "Char", "Short", "Int", "Long", "Float", "Double"};
static const char *FIELD_TYPES[] = {"Object", "Boolean", "Byte", "Char", "Short", "Int", "Long", "Float", "Double"};

static void jni_table(void) {
    static char *names[233];
    int n = 0;
#define ADD(s) names[n++] = _strdup(s)
    const char *head[] = {"reserved0", "reserved1", "reserved2", "reserved3", "GetVersion", "DefineClass", "FindClass",
        "FromReflectedMethod", "FromReflectedField", "ToReflectedMethod", "GetSuperclass", "IsAssignableFrom",
        "ToReflectedField", "Throw", "ThrowNew", "ExceptionOccurred", "ExceptionDescribe", "ExceptionClear",
        "FatalError", "PushLocalFrame", "PopLocalFrame", "NewGlobalRef", "DeleteGlobalRef", "DeleteLocalRef",
        "IsSameObject", "NewLocalRef", "EnsureLocalCapacity", "AllocObject", "NewObject", "NewObjectV", "NewObjectA",
        "GetObjectClass", "IsInstanceOf", "GetMethodID"};
    char b[64];
    for (int k = 0; k < (int)(sizeof(head) / sizeof(*head)); k++) ADD(head[k]);
    const char *vars[] = {"", "V", "A"};
    for (int t = 0; t < 10; t++) for (int v = 0; v < 3; v++) { snprintf(b, 64, "Call%sMethod%s", JNI_TYPES[t], vars[v]); ADD(b); }
    for (int t = 0; t < 10; t++) for (int v = 0; v < 3; v++) { snprintf(b, 64, "CallNonvirtual%sMethod%s", JNI_TYPES[t], vars[v]); ADD(b); }
    ADD("GetFieldID");
    for (int t = 0; t < 9; t++) { snprintf(b, 64, "Get%sField", FIELD_TYPES[t]); ADD(b); }
    for (int t = 0; t < 9; t++) { snprintf(b, 64, "Set%sField", FIELD_TYPES[t]); ADD(b); }
    ADD("GetStaticMethodID");
    for (int t = 0; t < 10; t++) for (int v = 0; v < 3; v++) { snprintf(b, 64, "CallStatic%sMethod%s", JNI_TYPES[t], vars[v]); ADD(b); }
    ADD("GetStaticFieldID");
    for (int t = 0; t < 9; t++) { snprintf(b, 64, "GetStatic%sField", FIELD_TYPES[t]); ADD(b); }
    for (int t = 0; t < 9; t++) { snprintf(b, 64, "SetStatic%sField", FIELD_TYPES[t]); ADD(b); }
    const char *mid[] = {"NewString", "GetStringLength", "GetStringChars", "ReleaseStringChars", "NewStringUTF",
        "GetStringUTFLength", "GetStringUTFChars", "ReleaseStringUTFChars", "GetArrayLength", "NewObjectArray",
        "GetObjectArrayElement", "SetObjectArrayElement"};
    for (int k = 0; k < (int)(sizeof(mid) / sizeof(*mid)); k++) ADD(mid[k]);
    for (int t = 0; t < 8; t++) { snprintf(b, 64, "New%sArray", PRIM_TYPES[t]); ADD(b); }
    for (int t = 0; t < 8; t++) { snprintf(b, 64, "Get%sArrayElements", PRIM_TYPES[t]); ADD(b); }
    for (int t = 0; t < 8; t++) { snprintf(b, 64, "Release%sArrayElements", PRIM_TYPES[t]); ADD(b); }
    for (int t = 0; t < 8; t++) { snprintf(b, 64, "Get%sArrayRegion", PRIM_TYPES[t]); ADD(b); }
    for (int t = 0; t < 8; t++) { snprintf(b, 64, "Set%sArrayRegion", PRIM_TYPES[t]); ADD(b); }
    const char *tail[] = {"RegisterNatives", "UnregisterNatives", "MonitorEnter", "MonitorExit", "GetJavaVM",
        "GetStringRegion", "GetStringUTFRegion", "GetPrimitiveArrayCritical", "ReleasePrimitiveArrayCritical",
        "GetStringCritical", "ReleaseStringCritical", "NewWeakGlobalRef", "DeleteWeakGlobalRef", "ExceptionCheck",
        "NewDirectByteBuffer", "GetDirectBufferAddress", "GetDirectBufferCapacity", "GetObjectRefType"};
    for (int k = 0; k < (int)(sizeof(tail) / sizeof(*tail)); k++) ADD(tail[k]);
#undef ADD
    if (n != 233) fatal("JNI table has %d entries", n);

    uint32_t table = gzalloc(4 * 233);
    for (int k = 0; k < 233; k++) {
        const char *nm = names[k];
        ImportFn fn = jn_unimplemented;
        char full[96];
        if (!strncmp(nm, "CallStatic", 10)) fn = nm[strlen(nm) - 1] == 'V' && strstr(nm, "MethodV") ? jn_scallv : strstr(nm, "MethodA") ? jn_scalla : jn_scall;
        else if (!strncmp(nm, "CallNonvirtual", 14)) fn = strstr(nm, "MethodV") ? jn_ncallv : strstr(nm, "MethodA") ? jn_ncalla : jn_ncall;
        else if (!strncmp(nm, "Call", 4)) fn = strstr(nm, "MethodV") ? jn_callv : strstr(nm, "MethodA") ? jn_calla : jn_call;
        else if (!strcmp(nm, "GetVersion")) fn = jn_version;
        else if (!strcmp(nm, "FindClass")) fn = jn_find_class;
        else if (!strcmp(nm, "GetObjectClass")) fn = jn_object_class;
        else if (!strcmp(nm, "GetSuperclass")) fn = jn_superclass;
        else if (!strcmp(nm, "IsAssignableFrom") || !strcmp(nm, "IsInstanceOf")) fn = jn_true;
        else if (!strcmp(nm, "GetMethodID")) fn = jn_get_mid;
        else if (!strcmp(nm, "GetStaticMethodID")) fn = jn_get_smid;
        else if (!strcmp(nm, "GetFieldID")) fn = jn_get_mid;
        else if (!strcmp(nm, "GetStaticFieldID")) fn = jn_get_smid;
        else if (!strcmp(nm, "GetStaticObjectField") || !strcmp(nm, "GetStaticIntField")) fn = jn_get_static_field;
        else if (!strcmp(nm, "GetIntField")) fn = jn_get_int_field;
        else if (!strcmp(nm, "GetObjectField")) fn = jn_get_object_field;
        else if (!strcmp(nm, "NewObject") || !strcmp(nm, "NewObjectV")) fn = jn_new_object;
        else if (!strcmp(nm, "ExceptionCheck") || !strcmp(nm, "ExceptionOccurred") || !strcmp(nm, "ExceptionClear") ||
                 !strcmp(nm, "ExceptionDescribe") || !strcmp(nm, "DeleteGlobalRef") || !strcmp(nm, "DeleteLocalRef") ||
                 !strcmp(nm, "DeleteWeakGlobalRef") || !strcmp(nm, "PushLocalFrame") ||
                 !strcmp(nm, "EnsureLocalCapacity")) fn = jn_zero;
        else if (!strcmp(nm, "NewGlobalRef") || !strcmp(nm, "NewLocalRef") || !strcmp(nm, "NewWeakGlobalRef") ||
                 !strcmp(nm, "PopLocalFrame")) fn = jn_arg1;
        else if (!strcmp(nm, "IsSameObject")) fn = jn_same;
        else if (!strcmp(nm, "GetJavaVM")) fn = jn_get_vm;
        else if (!strcmp(nm, "RegisterNatives")) fn = jn_register_natives;
        else if (!strcmp(nm, "NewStringUTF")) fn = jn_new_string_utf;
        else if (!strcmp(nm, "GetStringUTFChars")) fn = jn_get_string_utf_chars;
        else if (!strcmp(nm, "ReleaseStringUTFChars") || !strcmp(nm, "ReleaseStringChars")) fn = jn_release_free2;
        else if (!strcmp(nm, "GetStringUTFLength")) fn = jn_string_utf_length;
        else if (!strcmp(nm, "GetStringLength")) fn = jn_string_length;
        else if (!strcmp(nm, "GetStringChars")) fn = jn_get_string_chars;
        else if (!strcmp(nm, "NewString")) fn = jn_new_string;
        else if (!strcmp(nm, "GetStringUTFRegion")) fn = jn_get_string_utf_region;
        else if (!strcmp(nm, "GetArrayLength")) fn = jn_array_length;
        else if (!strcmp(nm, "NewObjectArray")) fn = jn_new_object_array;
        else if (!strcmp(nm, "GetObjectArrayElement")) fn = jn_get_object_array_element;
        else if (!strcmp(nm, "SetObjectArrayElement")) fn = jn_set_object_array_element;
        else if (!strncmp(nm, "New", 3) && strstr(nm, "Array")) fn = jn_new_prim_array;
        else if (strstr(nm, "ArrayElements")) fn = !strncmp(nm, "Get", 3) ? jn_get_array_elements : jn_release_array_elements;
        else if (strstr(nm, "ArrayRegion")) fn = !strncmp(nm, "Get", 3) ? jn_get_array_region : jn_set_array_region;
        snprintf(full, sizeof(full), "JNIEnv::%s", nm);
        gw32(table + 4u * (uint32_t)k, host_fn(_strdup(full), fn));
    }
    jni_env = gzalloc(4);
    gw32(jni_env, table);
}

/* JavaVM: GetEnv / AttachCurrentThread write the (single) env */
static void vm_get_env(CPU *c) { gw32(c->r[1], jni_env); RET(c, 0); }
static void vm_ok(CPU *c) { RET(c, 0); }

/* ---------------------------------------------------------------- AAsset (NDK) over the extracted assets */
typedef struct Asset { uint32_t g; uint8_t *data; uint32_t len, pos; } Asset;
#define ASSET_MAX 256
static Asset assets[ASSET_MAX];
static SRWLOCK asset_lock = SRWLOCK_INIT;
static uint32_t asset_mgr;

static Asset *asset_get(uint32_t g) {
    for (int k = 0; k < ASSET_MAX; k++) if (assets[k].g == g) return &assets[k];
    fatal("unknown AAsset 0x%08X", g);
}

static void a_from_java(CPU *c) { RET(c, asset_mgr); }
static void a_open(CPU *c) {
    const char *name = gstr(c->r[1]);
    while (*name == '/') name++;
    char path[2048];
    snprintf(path, sizeof(path), "%s/assets/%s", game_dir, name);
    FILE *f = fopen(path, "rb");
    if (!f) { hlog("  AAssetManager_open(%s) -> NULL", name); RET(c, 0); return; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *d = malloc((size_t)len + 1);
    fread(d, 1, (size_t)len, f);
    fclose(f);
    uint32_t g = gzalloc(16);
    AcquireSRWLockExclusive(&asset_lock);
    int k = 0;
    while (k < ASSET_MAX && assets[k].g) k++;
    if (k == ASSET_MAX) fatal("too many open assets");
    assets[k].g = g;
    assets[k].data = d;
    assets[k].len = (uint32_t)len;
    assets[k].pos = 0;
    ReleaseSRWLockExclusive(&asset_lock);
    hlog("  AAssetManager_open(%s) -> %ld bytes", name, len);
    RET(c, g);
}
static void a_close(CPU *c) {
    AcquireSRWLockExclusive(&asset_lock);
    Asset *a = asset_get(c->r[0]);
    free(a->data);
    gfree(a->g);
    memset(a, 0, sizeof(*a));
    ReleaseSRWLockExclusive(&asset_lock);
    RET(c, 0);
}
static void a_length(CPU *c) { RET(c, asset_get(c->r[0])->len); }
static void a_remaining(CPU *c) { Asset *a = asset_get(c->r[0]); RET(c, a->len - a->pos); }
static void a_read(CPU *c) {
    Asset *a = asset_get(c->r[0]);
    uint32_t n = c->r[2];
    if (n > a->len - a->pos) n = a->len - a->pos;
    memcpy(GP(c->r[1]), a->data + a->pos, n);
    a->pos += n;
    RET(c, n);
}
static void a_seek(CPU *c) {
    Asset *a = asset_get(c->r[0]);
    int64_t off = (int32_t)c->r[1], base = c->r[2] == 0 ? 0 : c->r[2] == 1 ? a->pos : a->len;
    a->pos = (uint32_t)(base + off);
    RET(c, a->pos);
}

void jni_init(void) {
    objs = calloc(JOBJ_CAP, sizeof(JObj *));
    mids = calloc(65536, sizeof(JMethod));
    jni_table();
    uint32_t vt = gzalloc(4 * 8);
    static const char *vm_names[] = {"r0", "r1", "r2", "DestroyJavaVM", "AttachCurrentThread", "DetachCurrentThread",
                                     "GetEnv", "AttachCurrentThreadAsDaemon"};
    for (int k = 0; k < 8; k++) {
        char full[64];
        snprintf(full, sizeof(full), "JavaVM::%s", vm_names[k]);
        int env = k == 4 || k == 6 || k == 7;
        gw32(vt + 4u * (uint32_t)k, host_fn(_strdup(full), env ? vm_get_env : vm_ok));
    }
    jni_vm = gzalloc(4);
    gw32(jni_vm, vt);
}

void android_init(void) {
    asset_mgr = gzalloc(16);
    char dir[1100];
    snprintf(dir, sizeof(dir), "%s/data/data/" PACKAGE "/shared_prefs/x", fs_root);
    char tmp[1100];
    host_path("/data/data/" PACKAGE "/shared_prefs/x", tmp, sizeof(tmp), 1);
    host_path(FILES_DIR "/x", tmp, sizeof(tmp), 1);
    snprintf(prefs_path, sizeof(prefs_path), "%s/data/data/" PACKAGE "/shared_prefs/Cocos2dxPrefsFile.json", fs_root);
    prefs_load();
    hlog("preferences: %d keys from %s", n_prefs, prefs_path);
    imp("AAssetManager_fromJava", a_from_java);
    imp("AAssetManager_open", a_open);
    imp("AAsset_close", a_close);
    imp("AAsset_getLength", a_length);
    imp("AAsset_getRemainingLength", a_remaining);
    imp("AAsset_read", a_read);
    imp("AAsset_seek", a_seek);
}
