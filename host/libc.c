/* bionic libc / libm / liblog / libdl imports that lift.dll does not run natively (as emu/libc.py). */
#include "host.h"
#include <math.h>
#include <time.h>
#include <ctype.h>

#define G_ENOENT 2
#define G_ENOSYS 38

/* ---------------------------------------------------------------- printf engine (as format_c) */
typedef struct Out { char *buf; size_t n, cap; } Out;

static void out_put(Out *o, const char *s, size_t n) {
    if (o->n + n + 1 > o->cap) {
        o->cap = (o->n + n + 1) * 2;
        o->buf = realloc(o->buf, o->cap);
    }
    memcpy(o->buf + o->n, s, n);
    o->n += n;
    o->buf[o->n] = 0;
}

/* format with guest arguments; returns a malloc'd string (length in *len) */
static char *format_c(const char *fmt, VA *a, size_t *len) {
    Out o = {malloc(64), 0, 64};
    o.buf[0] = 0;
    for (const char *p = fmt; *p;) {
        if (*p != '%') {
            const char *q = p;
            while (*q && *q != '%') q++;
            out_put(&o, p, (size_t)(q - p));
            p = q;
            continue;
        }
        char spec[64];
        int sn = 0;
        spec[sn++] = '%';
        const char *q = p + 1;
        while (*q && strchr("-+ #0", *q) && sn < 40) spec[sn++] = *q++;
        if (*q == '*') { sn += snprintf(spec + sn, 12, "%d", (int32_t)va_u32(a)); q++; }
        else while (isdigit((unsigned char)*q) && sn < 50) spec[sn++] = *q++;
        if (*q == '.') {
            spec[sn++] = *q++;
            if (*q == '*') { sn += snprintf(spec + sn, 12, "%d", (int32_t)va_u32(a)); q++; }
            else while (isdigit((unsigned char)*q) && sn < 58) spec[sn++] = *q++;
        }
        char len[4] = {0};
        int ln = 0;
        while (*q && strchr("hlLqjzt", *q) && ln < 3) len[ln++] = *q++;
        char conv = *q;
        if (!conv) break;
        q++;
        int is64 = !strcmp(len, "ll") || !strcmp(len, "q") || !strcmp(len, "j");
        char tmp[512];
        int n = 0;
        spec[sn] = 0;
        switch (conv) {
        case '%': out_put(&o, "%", 1); break;
        case 'd': case 'i': {
            int64_t v = is64 ? (int64_t)va_u64(a) : (int64_t)(int32_t)va_u32(a);
            if (!strcmp(len, "h")) v = (int16_t)v;
            else if (!strcmp(len, "hh")) v = (int8_t)v;
            strcat(spec, "lld");
            n = snprintf(tmp, sizeof(tmp), spec, (long long)v);
            break;
        }
        case 'u': case 'x': case 'X': case 'o': {
            uint64_t v = is64 ? va_u64(a) : va_u32(a);
            if (!strcmp(len, "h")) v &= 0xFFFF;
            else if (!strcmp(len, "hh")) v &= 0xFF;
            size_t k = strlen(spec);
            spec[k] = 'l'; spec[k + 1] = 'l'; spec[k + 2] = conv; spec[k + 3] = 0;
            n = snprintf(tmp, sizeof(tmp), spec, (unsigned long long)v);
            break;
        }
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A': {
            double v = va_f64(a);
            size_t k = strlen(spec);
            spec[k] = conv; spec[k + 1] = 0;
            n = snprintf(tmp, sizeof(tmp), spec, v);
            break;
        }
        case 'c': {
            strcat(spec, "c");
            n = snprintf(tmp, sizeof(tmp), spec, (int)(va_u32(a) & 0xFF));
            break;
        }
        case 's': {
            uint32_t sp = va_u32(a);
            const char *s = sp ? gstr(sp) : "(null)";
            strcat(spec, "s");
            int need = snprintf(NULL, 0, spec, s);
            char *big = malloc((size_t)need + 1);
            snprintf(big, (size_t)need + 1, spec, s);
            out_put(&o, big, (size_t)need);
            free(big);
            break;
        }
        case 'p': n = snprintf(tmp, sizeof(tmp), "0x%x", va_u32(a)); break;
        case 'n': va_u32(a); break;
        default: out_put(&o, p, (size_t)(q - p)); break;
        }
        if (n > 0) out_put(&o, tmp, (size_t)(n < (int)sizeof(tmp) ? n : (int)sizeof(tmp) - 1));
        p = q;
    }
    *len = o.n;
    return o.buf;
}

char *host_format(const char *fmt, VA *a, size_t *len) { return format_c(fmt, a, len); }

static void put_bounded(uint32_t buf, uint32_t n, const char *s, size_t len) {
    if (!n) return;
    size_t k = len < n - 1 ? len : n - 1;
    { glog(buf, (uint32_t)(k)); memcpy(GP(buf), s, k); }
    gw8(buf + (uint32_t)k, (uint8_t)(0));
}

static void i_snprintf(CPU *c) {
    VA a = va_regs(c, 3);
    size_t len;
    char *s = format_c(gstr(c->r[2]), &a, &len);
    put_bounded(c->r[0], c->r[1], s, len);
    free(s);
    RET(c, (uint32_t)len);
}
static void i_vsnprintf(CPU *c) {
    VA a = va_list_at(c, c->r[3]);
    size_t len;
    char *s = format_c(gstr(c->r[2]), &a, &len);
    put_bounded(c->r[0], c->r[1], s, len);
    free(s);
    RET(c, (uint32_t)len);
}
static void i_sprintf(CPU *c) {
    VA a = va_regs(c, 2);
    size_t len;
    char *s = format_c(gstr(c->r[1]), &a, &len);
    { glog(c->r[0], (uint32_t)(len + 1)); memcpy(GP(c->r[0]), s, len + 1); }
    free(s);
    RET(c, (uint32_t)len);
}
static void i_vsprintf(CPU *c) {
    VA a = va_list_at(c, c->r[2]);
    size_t len;
    char *s = format_c(gstr(c->r[1]), &a, &len);
    { glog(c->r[0], (uint32_t)(len + 1)); memcpy(GP(c->r[0]), s, len + 1); }
    free(s);
    RET(c, (uint32_t)len);
}
static void i_vasprintf(CPU *c) {
    VA a = va_list_at(c, c->r[2]);
    size_t len;
    char *s = format_c(gstr(c->r[1]), &a, &len);
    gw32(c->r[0], galloc_bytes(s, (uint32_t)len + 1));
    free(s);
    RET(c, (uint32_t)len);
}
static void stdout_line(const char *tag, const char *s, size_t len) {
    while (len && (s[len - 1] == '\n' || s[len - 1] == '\r')) len--;
    hlog("[%s] %.*s", tag, (int)len, s);
}
static void i_printf(CPU *c) {
    VA a = va_regs(c, 1);
    size_t len;
    char *s = format_c(gstr(c->r[0]), &a, &len);
    stdout_line("stdout", s, len);
    free(s);
    RET(c, (uint32_t)len);
}
static void i_puts(CPU *c) { const char *s = gstr(c->r[0]); stdout_line("stdout", s, strlen(s)); RET(c, 1); }
static void i_putchar(CPU *c) { RET(c, c->r[0]); }
static void i_perror(CPU *c) { hlog("[perror] %s", c->r[0] ? gstr(c->r[0]) : ""); RET(c, 0); }

static const char LOG_LEVELS[] = "??VDIWEFS";
static void i_alog_print(CPU *c) {
    VA a = va_regs(c, 3);
    size_t len;
    char *s = format_c(gstr(c->r[2]), &a, &len);
    uint32_t prio = c->r[0];
    hlog("[log %c/%s] %s", prio < 9 ? LOG_LEVELS[prio] : '?', c->r[1] ? gstr(c->r[1]) : "", s);
    free(s);
    RET(c, (uint32_t)len);
}
static void i_alog_write(CPU *c) {
    uint32_t prio = c->r[0];
    hlog("[log %c/%s] %s", prio < 9 ? LOG_LEVELS[prio] : '?', c->r[1] ? gstr(c->r[1]) : "", gstr(c->r[2]));
    RET(c, 1);
}
static void i_syslog(CPU *c) {
    VA a = va_regs(c, 2);
    size_t len;
    char *s = format_c(gstr(c->r[1]), &a, &len);
    hlog("[syslog %u] %s", c->r[0], s);
    free(s);
    RET(c, 0);
}

/* ---------------------------------------------------------------- sscanf: the host's, with guest pointers
 * Every conversion of the game's formats stores through a pointer whose target has the same size on both
 * sides (int, long and float are 32-bit on ARM and Windows; long long and double 64-bit), so the host sscanf
 * runs on the guest's format with the pointers translated. */
static int scan(const char *src, const char *fmt, VA *a) {
    void *p[24];
    int n = 0;
    for (const char *q = fmt; *q; q++) {
        if (*q != '%') continue;
        q++;
        if (*q == '%') continue;
        if (*q == '*') continue;
        while (*q && strchr("0123456789hlLqjzt", *q)) q++;
        if (*q == 'p' || *q == 'z') fatal("sscanf: unsupported conversion in %s", fmt);
        if (*q == '[') { q++; if (*q == '^') q++; if (*q == ']') q++; while (*q && *q != ']') q++; }
        if (n >= 24) fatal("sscanf: too many conversions in %s", fmt);
        p[n++] = GP(va_u32(a));
    }
    for (int k = n; k < 24; k++) p[k] = NULL;
    return sscanf(src, fmt, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12],
                  p[13], p[14], p[15], p[16], p[17], p[18], p[19], p[20], p[21], p[22], p[23]);
}
static void i_sscanf(CPU *c) { VA a = va_regs(c, 2); RET(c, (uint32_t)scan(gstr(c->r[0]), gstr(c->r[1]), &a)); }
static void i_vsscanf(CPU *c) { VA a = va_list_at(c, c->r[2]); RET(c, (uint32_t)scan(gstr(c->r[0]), gstr(c->r[1]), &a)); }

/* ---------------------------------------------------------------- strings */
static int cmp3(int v) { return v < 0 ? -1 : v > 0; }
static void i_strcasecmp(CPU *c) { RET(c, (uint32_t)cmp3(_stricmp(gstr(c->r[0]), gstr(c->r[1])))); }
static void i_strncasecmp(CPU *c) {
    RET(c, c->r[2] ? (uint32_t)cmp3(_strnicmp(gstr(c->r[0]), gstr(c->r[1]), c->r[2])) : 0);
}
static void i_strlcpy(CPU *c) {
    const char *s = gstr(c->r[1]);
    size_t len = strlen(s), n = c->r[2];
    if (n) {
        size_t k = len < n - 1 ? len : n - 1;
        { glog(c->r[0], (uint32_t)(k)); memcpy(GP(c->r[0]), s, k); }
        gw8(c->r[0] + (uint32_t)k, (uint8_t)(0));
    }
    RET(c, (uint32_t)len);
}
static void i_strdup(CPU *c) { RET(c, galloc_str(gstr(c->r[0]))); }
static void i_strstr(CPU *c) {
    const char *h = gstr(c->r[0]), *r = strstr(h, gstr(c->r[1]));
    RET(c, r ? c->r[0] + (uint32_t)(r - h) : 0);
}
static void i_strpbrk(CPU *c) {
    const char *h = gstr(c->r[0]), *r = strpbrk(h, gstr(c->r[1]));
    RET(c, r ? c->r[0] + (uint32_t)(r - h) : 0);
}
static void i_strspn(CPU *c) { RET(c, (uint32_t)strspn(gstr(c->r[0]), gstr(c->r[1]))); }
static void i_strcspn(CPU *c) { RET(c, (uint32_t)strcspn(gstr(c->r[0]), gstr(c->r[1]))); }
static void i_strtok_r(CPU *c) {
    uint32_t s = c->r[0], save = c->r[2];
    const char *d = gstr(c->r[1]);
    if (!s) s = g32(save);
    if (!s) { RET(c, 0); return; }
    s += (uint32_t)strspn(gstr(s), d);
    if (!*gstr(s)) { gw32(save, 0); RET(c, 0); return; }
    uint32_t e = s + (uint32_t)strcspn(gstr(s), d);
    if (*gstr(e)) {
        *gstr(e) = 0;
        gw32(save, e + 1);
    } else {
        gw32(save, 0);
    }
    RET(c, s);
}
static void i_strerror(CPU *c) {
    char b[32];
    snprintf(b, sizeof(b), "error %u", c->r[0]);
    RET(c, galloc_str(b));
}
static void i_strerror_r(CPU *c) {
    char b[32];
    int n = snprintf(b, sizeof(b), "error %u", c->r[0]);
    put_bounded(c->r[1], c->r[2], b, (size_t)n);
    RET(c, 0);
}
static void i_memrchr(CPU *c) {
    const uint8_t *p = GP(c->r[0]);
    for (uint32_t k = c->r[2]; k-- > 0;) if (p[k] == (uint8_t)c->r[1]) { RET(c, c->r[0] + k); return; }
    RET(c, 0);
}
static void i_memmem(CPU *c) {
    const uint8_t *h = GP(c->r[0]), *nd = GP(c->r[2]);
    uint32_t hn = c->r[1], nn = c->r[3];
    if (!nn) { RET(c, c->r[0]); return; }
    for (uint32_t k = 0; k + nn <= hn; k++) if (!memcmp(h + k, nd, nn)) { RET(c, c->r[0] + k); return; }
    RET(c, 0);
}
static void i_wcscmp(CPU *c) {
    for (uint32_t k = 0;; k += 4) {
        uint32_t x = g32(c->r[0] + k), y = g32(c->r[1] + k);
        if (x != y || !x) { RET(c, x == y ? 0 : x < y ? 0xFFFFFFFFu : 1); return; }
    }
}

/* strtol family (as _strto: clamps to the type's range) */
static void strto(CPU *c, int is_signed, int bits) {
    uint32_t s_addr = c->r[0];
    const char *s = gstr(s_addr);
    char *end;
    uint32_t endp = c->r[1];
    int base = (int)c->r[2];
    if (is_signed) {
        long long v = strtoll(s, &end, base);
        if (bits == 32) v = v < INT32_MIN ? INT32_MIN : v > INT32_MAX ? INT32_MAX : v;
        if (bits == 64) RET64(c, (uint64_t)v); else RET(c, (uint32_t)v);
    } else {
        unsigned long long v = strtoull(s, &end, base);
        int neg = 0;
        for (const char *q = s; *q == ' ' || (*q >= 9 && *q <= 13) || *q == '-' || *q == '+'; q++) if (*q == '-') neg = 1;
        if (bits == 32 && !neg && v > UINT32_MAX) v = UINT32_MAX;
        if (bits == 64) RET64(c, v); else RET(c, (uint32_t)v);
    }
    if (endp) gw32(endp, s_addr + (uint32_t)(end - s));
}
static void i_strtol(CPU *c) { strto(c, 1, 32); }
static void i_strtoul(CPU *c) { strto(c, 0, 32); }
static void i_strtoll(CPU *c) { strto(c, 1, 64); }
static void i_strtoull(CPU *c) { strto(c, 0, 64); }
static void i_atoi(CPU *c) { RET(c, (uint32_t)atoi(gstr(c->r[0]))); }

static void i_tolower(CPU *c) { uint32_t v = c->r[0]; RET(c, v >= 'A' && v <= 'Z' ? v + 32 : v); }
static void i_toupper(CPU *c) { uint32_t v = c->r[0]; RET(c, v >= 'a' && v <= 'z' ? v - 32 : v); }
static void i_isspace(CPU *c) { uint32_t v = c->r[0]; RET(c, v == 32 || (v >= 9 && v <= 13)); }
static void i_isupper(CPU *c) { RET(c, c->r[0] >= 'A' && c->r[0] <= 'Z'); }
static void i_isprint(CPU *c) { RET(c, c->r[0] >= 32 && c->r[0] < 127); }
static void i_isblank(CPU *c) { RET(c, c->r[0] == 9 || c->r[0] == 32); }

/* ---------------------------------------------------------------- process / misc */
static void i_ok(CPU *c) { RET(c, 0); }
static void i_fail_enosys(CPU *c) { set_errno(G_ENOSYS); RET(c, 0xFFFFFFFFu); }
static void i_stack_chk_fail(CPU *c) { fatal("__stack_chk_fail (lr=0x%08X)", c->r[14]); }
static void i_abort(CPU *c) { fatal("abort() (lr=0x%08X)", c->r[14]); }
static void i_exit(CPU *c) { fatal("exit(%d) (lr=0x%08X)", (int)c->r[0], c->r[14]); }
static void i_assert2(CPU *c) {
    fatal("assert %s at %s:%u in %s", gstr(c->r[3]), gstr(c->r[0]), c->r[1], gstr(c->r[2]));
}
static void i_getpid(CPU *c) { RET(c, 4242); }
static void i_getuid(CPU *c) { RET(c, 10217); }
static void i_setlocale(CPU *c) {
    static uint32_t cl;
    if (!cl) cl = galloc_str("C");
    RET(c, cl);
}
static void i_sysconf(CPU *c) {
    uint32_t v = 0;
    switch (c->r[0]) { case 0x27: case 0x28: v = 4096; break; case 0x60: case 0x61: v = 4; break; }
    RET(c, v);
}
static void i_getdtablesize(CPU *c) { RET(c, 1024); }

static uint64_t rand_state = 0x1234ABCD330EULL;
static void i_srand48(CPU *c) { rand_state = (((uint64_t)c->r[0] << 16) | 0x330E) & 0xFFFFFFFFFFFFULL; RET(c, 0); }
static void i_lrand48(CPU *c) {
    rand_state = (rand_state * 0x5DEECE66DULL + 0xB) & 0xFFFFFFFFFFFFULL;
    RET(c, (uint32_t)(rand_state >> 17));
}

static void i_qsort(CPU *c) {
    /* lift.dll sorts natively; it declines only when the comparator is not lifted code */
    fatal("qsort: comparator 0x%08X is not lifted", c->r[3]);
}
static void i_bsearch(CPU *c) {
    uint32_t key = c->r[0], base = c->r[1], n = c->r[2], size = c->r[3], cmp = ARG(c, 4);
    uint32_t lo = 0, hi = n;
    while (lo < hi) {
        uint32_t mid = (lo + hi) / 2;
        int32_t r = (int32_t)GCALL(c, cmp, key, base + mid * size);
        if (!r) { RET(c, base + mid * size); return; }
        if (r < 0) hi = mid; else lo = mid + 1;
    }
    RET(c, 0);
}
static void i_div(CPU *c) {
    /* div_t returned in memory: hidden result pointer in r0 */
    int32_t a = (int32_t)c->r[1], b = (int32_t)c->r[2];
    gw32(c->r[0], (uint32_t)(a / b));
    gw32(c->r[0] + 4, (uint32_t)(a % b));
    RET(c, c->r[0]);
}

static void i_cxa_atexit(CPU *c) { RET(c, 0); }

/* ---------------------------------------------------------------- time */
/* M3 verifier (rewrite_shadow): while a rewritten function and its lifted original are shadow-run on this thread,
 * the clocks stand still and both runs draw the same random numbers, so time- or rand-dependent results match. */
static __declspec(thread) int shadow_on;
static __declspec(thread) double shadow_real, shadow_mono;
static uint64_t shadow_rand;
static double now_real_raw(void);
static double now_mono_raw(void);
int libc_shadow(int phase) {
    if (phase >= 3) return import_shadow(phase);
    import_shadow(phase);
    if (phase == 0) {                       /* begin: freeze the clocks, remember the random state */
        L.heap_hold(1);                     /* no other guest thread allocates during the shadow run */
        shadow_real = now_real_raw();
        shadow_mono = now_mono_raw();
        shadow_rand = rand_state;
        shadow_on = 1;
    } else if (phase == 1) {                /* before another run: the same random numbers again */
        rand_state = shadow_rand;
    } else {
        shadow_on = 0;
        L.heap_hold(0);
    }
    return 0;
}
/* A blocking wait inside a shadow run (a mutex another thread holds, a condition, a sleep): the run holds the guest
 * heap, and the thread it waits for may need the heap first, so the hold is let go for the wait. Other threads may
 * then change the heap, which an undo would clobber: the run becomes opaque (kept as the real call, not compared). */
static __declspec(thread) int shadow_waits;
void shadow_wait_begin(void) {
    if (!shadow_on) return;
    shadow_waits++;
    import_shadow_opaque();
    L.heap_hold(0);
}
void shadow_wait_end(void) {
    if (!shadow_on || !shadow_waits) return;
    shadow_waits--;
    L.heap_hold(1);
}

static double now_real(void) { return shadow_on ? shadow_real : now_real_raw(); }
static double now_mono(void) { return shadow_on ? shadow_mono : now_mono_raw(); }
static double now_real_raw(void) {
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);
    return ((((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime) - 116444736000000000ULL) / 1e7;
}
static double now_mono_raw(void) {
    static LARGE_INTEGER f;
    LARGE_INTEGER t;
    if (!f.QuadPart) QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / (double)f.QuadPart;
}
static void i_time(CPU *c) {
    uint32_t t = (uint32_t)(int64_t)now_real();
    if (c->r[0]) gw32(c->r[0], t);
    RET(c, t);
}
static void i_gettimeofday(CPU *c) {
    double t = now_real();
    if (c->r[0]) {
        gw32(c->r[0], (uint32_t)(int64_t)t);
        gw32(c->r[0] + 4, (uint32_t)((t - floor(t)) * 1e6));
    }
    RET(c, 0);
}
static void i_clock_gettime(CPU *c) {
    uint32_t clk = c->r[0];
    double t = (clk == 1 || clk == 4 || clk == 7) ? now_mono() : now_real();
    gw32(c->r[1], (uint32_t)(int64_t)t);
    gw32(c->r[1] + 4, (uint32_t)((t - floor(t)) * 1e9));
    RET(c, 0);
}
/* struct tm (bionic 32-bit): 9 ints + long tm_gmtoff + const char *tm_zone */
static void tm_put(uint32_t out, const struct tm *t) {
    int32_t v[11] = {t->tm_sec, t->tm_min, t->tm_hour, t->tm_mday, t->tm_mon, t->tm_year, t->tm_wday, t->tm_yday,
                     t->tm_isdst, 0, 0};
    { glog(out, (uint32_t)(sizeof(v))); memcpy(GP(out), v, sizeof(v)); }
}
static void tm_get(uint32_t in, struct tm *t) {
    int32_t v[9];
    memcpy(v, GP(in), sizeof(v));
    memset(t, 0, sizeof(*t));
    t->tm_sec = v[0]; t->tm_min = v[1]; t->tm_hour = v[2]; t->tm_mday = v[3]; t->tm_mon = v[4];
    t->tm_year = v[5]; t->tm_wday = v[6]; t->tm_yday = v[7]; t->tm_isdst = v[8];
}
static void i_gmtime_r(CPU *c) {
    __time64_t t = (int32_t)g32(c->r[0]);
    struct tm tm;
    _gmtime64_s(&tm, &t);
    tm_put(c->r[1], &tm);
    RET(c, c->r[1]);
}
static void i_gmtime(CPU *c) {
    static __declspec(thread) uint32_t buf;
    if (!buf) buf = gzalloc(44);
    __time64_t t = (int32_t)g32(c->r[0]);
    struct tm tm;
    _gmtime64_s(&tm, &t);
    tm_put(buf, &tm);
    RET(c, buf);
}
static void i_localtime(CPU *c) {
    static __declspec(thread) uint32_t buf;
    if (!buf) buf = gzalloc(44);
    __time64_t t = (int32_t)g32(c->r[0]);
    struct tm tm;
    _localtime64_s(&tm, &t);
    tm_put(buf, &tm);
    RET(c, buf);
}
static void i_timegm64(CPU *c) {
    struct tm tm;
    tm_get(c->r[0], &tm);
    RET64(c, (uint64_t)_mkgmtime64(&tm));
}
static void i_strftime(CPU *c) {
    struct tm tm;
    tm_get(c->r[3], &tm);
    char buf[512];
    size_t n = strftime(buf, sizeof(buf), gstr(c->r[2]), &tm);
    if (!n || n >= c->r[1]) { RET(c, 0); return; }
    { glog(c->r[0], (uint32_t)(n + 1)); memcpy(GP(c->r[0]), buf, n + 1); }
    RET(c, (uint32_t)n);
}

/* ---------------------------------------------------------------- setjmp / dl */
static void i_setjmp(CPU *c) { RET(c, 0); }
static void i_longjmp(CPU *c) {
    fatal("longjmp(0x%08X, %d) from 0x%08X: non-local returns out of lifted code are not supported",
          c->r[0], (int)c->r[1], c->r[14]);
}
static void i_dlopen(CPU *c) { hlog("  dlopen(%s)", c->r[0] ? gstr(c->r[0]) : "NULL"); RET(c, 1); }
static void i_dlsym(CPU *c) {
    const char *n = gstr(c->r[1]);
    uint32_t a = sym(n);
    if (!a && import_exists(n)) a = slot_for(_strdup(n));
    if (!a) hlog("  dlsym(%s) -> NULL", n);
    RET(c, a);
}

/* struct passwd (bionic 32-bit): pw_name, pw_passwd, pw_uid, pw_gid, pw_dir, pw_shell */
static void i_getpwuid(CPU *c) {
    static uint32_t pw;
    if (!pw) {
        pw = gzalloc(24);
        uint32_t v[6] = {galloc_str("u0_a217"), galloc_str(""), c->r[0], c->r[0], galloc_str("/data"),
                         galloc_str("/system/bin/sh")};
        { glog(pw, (uint32_t)(sizeof(v))); memcpy(GP(pw), v, sizeof(v)); }
    }
    RET(c, pw);
}
static void i_dlerror(CPU *c) { RET(c, 0); }
static void i_exidx(CPU *c) {
    gw32(c->r[1], exidx_count);
    RET(c, exidx_addr);
}

/* ---------------------------------------------------------------- math lift.dll does not have */
#define D1 U2D(c->r[0] | ((uint64_t)c->r[1] << 32))
#define D2 U2D(c->r[2] | ((uint64_t)c->r[3] << 32))
#define F1 U2F(c->r[0])
#define F2 U2F(c->r[1])
#define RD(x) RET64(c, D2U(x))
#define RF(x) RET(c, F2U((float)(x)))
static void m_sinh(CPU *c) { RD(sinh(D1)); }
static void m_cosh(CPU *c) { RD(cosh(D1)); }
static void m_tanh(CPU *c) { RD(tanh(D1)); }
static void m_asinh(CPU *c) { RD(asinh(D1)); }
static void m_acosh(CPU *c) { RD(acosh(D1)); }
static void m_atanh(CPU *c) { RD(atanh(D1)); }
static void m_exp2(CPU *c) { RD(exp2(D1)); }
static void m_expm1(CPU *c) { RD(expm1(D1)); }
static void m_log1p(CPU *c) { RD(log1p(D1)); }
static void m_logb(CPU *c) { RD(logb(D1)); }
static void m_cbrt(CPU *c) { RD(cbrt(D1)); }
static void m_rint(CPU *c) { RD(rint(D1)); }
static void m_erf(CPU *c) { RD(erf(D1)); }
static void m_erfc(CPU *c) { RD(erfc(D1)); }
static void m_lgamma(CPU *c) { RD(lgamma(D1)); }
static void m_tgamma(CPU *c) { RD(tgamma(D1)); }
static void m_hypot(CPU *c) { RD(hypot(D1, D2)); }
static void m_remainder(CPU *c) { RD(remainder(D1, D2)); }
static void m_nextafter(CPU *c) { RD(nextafter(D1, D2)); }
static void m_exp2f(CPU *c) { RF(exp2((double)F1)); }
static void m_roundf(CPU *c) { RF(roundf(F1)); }
static void m_fminf(CPU *c) { RF(fminf(F1, F2)); }
static void m_nextafterf(CPU *c) { RF(nextafterf(F1, F2)); }
static void m_isnan(CPU *c) { RET(c, isnan(D1) ? 1 : 0); }
static void m_isnanf(CPU *c) { RET(c, isnan(F1) ? 1 : 0); }
static void m_isfinite(CPU *c) { RET(c, isfinite(D1) ? 1 : 0); }
static void m_signbit(CPU *c) { RET(c, signbit(D1) ? 1 : 0); }
static void m_fpclassify(CPU *c) {
    double x = D1;
    RET(c, isnan(x) ? 2 : isinf(x) ? 1 : x == 0 ? 16 : fabs(x) < 2.2250738585072014e-308 ? 8 : 4);
}
static void m_modf(CPU *c) {
    double ip, f = modf(D1, &ip);
    *(double *)GP(c->r[2]) = ip;
    RD(f);
}
static void m_scalbn(CPU *c) { RD(ldexp(D1, (int32_t)c->r[2])); }
static void m_remquo(CPU *c) {
    int q;
    double r = remquo(D1, D2, &q);
    gw32(ARG(c, 4), (uint32_t)q);
    RD(r);
}
static void m_lrint(CPU *c) { RET(c, (uint32_t)(int32_t)lrint(D1)); }
static void m_llrint(CPU *c) { RET64(c, (uint64_t)llrint(D1)); }

void libc_init(void) {
    imp("snprintf", i_snprintf);
    imp("vsnprintf", i_vsnprintf);
    imp("sprintf", i_sprintf);
    imp("vsprintf", i_vsprintf);
    imp("vasprintf", i_vasprintf);
    imp("printf", i_printf);
    imp("puts", i_puts);
    imp("putchar", i_putchar);
    imp("perror", i_perror);
    imp("__android_log_print", i_alog_print);
    imp("__android_log_write", i_alog_write);
    imp("syslog", i_syslog);
    imp("sscanf", i_sscanf);
    imp("vsscanf", i_vsscanf);
    imp("strcasecmp", i_strcasecmp);
    imp("strncasecmp", i_strncasecmp);
    imp("strlcpy", i_strlcpy);
    imp("strdup", i_strdup);
    imp("strstr", i_strstr);
    imp("strpbrk", i_strpbrk);
    imp("strspn", i_strspn);
    imp("strcspn", i_strcspn);
    imp("strtok_r", i_strtok_r);
    imp("strerror", i_strerror);
    imp("strerror_r", i_strerror_r);
    imp("memrchr", i_memrchr);
    imp("memmem", i_memmem);
    imp("wcscmp", i_wcscmp);
    imp("strtol", i_strtol);
    imp("strtoul", i_strtoul);
    imp("strtoll", i_strtoll);
    imp("strtoumax", i_strtoll);
    imp("strtoull", i_strtoull);
    imp("atoi", i_atoi);
    imp("atol", i_atoi);
    imp("tolower", i_tolower);
    imp("toupper", i_toupper);
    imp("isspace", i_isspace);
    imp("isupper", i_isupper);
    imp("isprint", i_isprint);
    imp("isblank", i_isblank);
    imp("__cxa_atexit", i_cxa_atexit);
    imp("__aeabi_atexit", i_cxa_atexit);
    imp("__cxa_finalize", i_ok);
    imp("__stack_chk_fail", i_stack_chk_fail);
    imp("abort", i_abort);
    imp("exit", i_exit);
    imp("__assert2", i_assert2);
    imp("getpid", i_getpid);
    imp("getppid", i_getpid);
    imp("getuid", i_getuid);
    imp("geteuid", i_getuid);
    imp("getgid", i_getuid);
    imp("getegid", i_getuid);
    imp("getenv", i_ok);
    imp("setlocale", i_setlocale);
    imp("sysconf", i_sysconf);
    imp("getdtablesize", i_getdtablesize);
    static const char *ignore[] = {"bsd_signal", "sigaction", "sigprocmask", "raise", "kill", "alarm", "umask",
                                   "setsid", "setuid", "setgid", "chdir", "chmod", "pause"};
    for (int k = 0; k < (int)(sizeof(ignore) / sizeof(*ignore)); k++) imp(ignore[k], i_ok);
    static const char *unsupported[] = {"fork", "execl", "waitpid", "system", "pipe", "socketpair", "dup2"};
    for (int k = 0; k < (int)(sizeof(unsupported) / sizeof(*unsupported)); k++) imp(unsupported[k], i_fail_enosys);
    imp("srand48", i_srand48);
    imp("lrand48", i_lrand48);
    imp("qsort", i_qsort);
    imp("bsearch", i_bsearch);
    imp("div", i_div);
    imp("time", i_time);
    imp("gettimeofday", i_gettimeofday);
    imp("clock_gettime", i_clock_gettime);
    imp("gmtime_r", i_gmtime_r);
    imp("gmtime", i_gmtime);
    imp("localtime", i_localtime);
    imp("timegm64", i_timegm64);
    imp("strftime", i_strftime);
    imp("setjmp", i_setjmp);
    imp("sigsetjmp", i_setjmp);
    imp("longjmp", i_longjmp);
    imp("siglongjmp", i_longjmp);
    imp("dlopen", i_dlopen);
    imp("dlsym", i_dlsym);
    imp("getpwuid", i_getpwuid);
    imp("dlclose", i_ok);
    imp("dlerror", i_dlerror);
    imp("dl_unwind_find_exidx", i_exidx);
    imp("sinh", m_sinh); imp("cosh", m_cosh); imp("tanh", m_tanh); imp("asinh", m_asinh); imp("acosh", m_acosh);
    imp("atanh", m_atanh); imp("exp2", m_exp2); imp("expm1", m_expm1); imp("log1p", m_log1p); imp("logb", m_logb);
    imp("cbrt", m_cbrt); imp("rint", m_rint); imp("nearbyint", m_rint); imp("erf", m_erf); imp("erfc", m_erfc);
    imp("lgamma", m_lgamma); imp("tgamma", m_tgamma); imp("hypot", m_hypot); imp("remainder", m_remainder);
    imp("nextafter", m_nextafter); imp("exp2f", m_exp2f); imp("roundf", m_roundf); imp("fminf", m_fminf);
    imp("nextafterf", m_nextafterf); imp("isnan", m_isnan); imp("isnanf", m_isnanf); imp("__isfinite", m_isfinite);
    imp("__signbit", m_signbit); imp("__fpclassifyd", m_fpclassify); imp("modf", m_modf); imp("scalbn", m_scalbn);
    imp("scalbnl", m_scalbn); imp("remquo", m_remquo); imp("lrint", m_lrint); imp("llrint", m_llrint);
}
