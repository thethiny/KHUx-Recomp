/* stdio FILE* streams in C, shared by lifted code and the emulator's Python runtime (emu/libc.py, whose
 * behaviour this matches). Android paths map to files under the host folder rt_fs_root (set by the host), plus
 * one virtual file: the library itself. Streams this table does not own (stdin/stdout/stderr, fdopen) are
 * declined and handled by the host. Declined too inside logged runs: file positions cannot be rolled back.
 * Included by the generated lift_table.c. */
#include <stdio.h>
#include <direct.h>

EXPORT char rt_fs_root[1024];              /* host folder for the device file system */
EXPORT char rt_so_guest[256];              /* the library's device path ... */
EXPORT char rt_so_host[1024];              /* ... and its host path */
EXPORT uint32_t rt_file_opens;             /* streams opened here (diagnostics) */

#define MAX_FILES 512
static struct { uint32_t g; FILE *f; int eof; } files[MAX_FILES];

static int file_slot(uint32_t g) {
    for (int k = 0; k < MAX_FILES; k++) if (files[k].f && files[k].g == g) return k;
    return -1;
}

/* host path for a device path; creates the parent folders when `create` */
static int host_path(const char *path, char *out, size_t cap, int create) {
    if (rt_so_guest[0] && !strcmp(path, rt_so_guest)) {
        if (strlen(rt_so_host) + 1 > cap) return 0;
        strcpy(out, rt_so_host);
        return 1;
    }
    while (*path == '/') path++;
    if (strlen(rt_fs_root) + strlen(path) + 2 > cap) return 0;
    sprintf(out, "%s/%s", rt_fs_root, path);
    if (create)
        for (char *p = out + strlen(rt_fs_root) + 1; *p; p++)
            if (*p == '/') { *p = 0; _mkdir(out); *p = '/'; }
    return 1;
}

static int native_fopen(CPU *c) {
    const char *path = (const char *)HA(c->r[0]), *mode = (const char *)HA(c->r[1]);
    char host[2048], m[8];
    int k = 0, slot = -1;
    if (!rt_fs_root[0] || !rt_heap_ctl || c->logging) return 0;
    for (const char *p = mode; *p && k < 5; p++) if (*p != 't' && *p != 'b') m[k++] = *p;
    m[k++] = 'b';
    m[k] = 0;
    int writing = strchr(m, 'w') || strchr(m, 'a');
    for (int j = 0; j < MAX_FILES; j++) if (!files[j].f) { slot = j; break; }
    if (slot < 0 || !host_path(path, host, sizeof(host), writing)) return 0;
    FILE *f = fopen(host, m);
    if (!f) {
        if (rt_errno_ptr) R32(rt_errno_ptr) = 2;          /* ENOENT */
        c->r[0] = 0;
        return 1;
    }
    uint32_t fp = rt_heap_malloc(c, 84);                    /* the guest's FILE (bionic: 84 bytes) */
    if (!fp) { fclose(f); return 0; }
    files[slot].g = fp;
    files[slot].f = f;
    files[slot].eof = 0;
    rt_file_opens++;
    c->r[0] = fp;
    return 1;
}

static int native_fclose(CPU *c) {
    int k;
    if (c->logging || (k = file_slot(c->r[0])) < 0) return 0;
    fclose(files[k].f);
    files[k].f = 0;
    rt_heap_free(c, files[k].g);
    c->r[0] = 0;
    return 1;
}

static int native_fread(CPU *c) {
    uint32_t buf = c->r[0], size = c->r[1], n = c->r[2];
    int k;
    if (c->logging || (k = file_slot(c->r[3])) < 0) return 0;
    uint64_t want = (uint64_t)size * n;
    if (want > 0x40000000u) return 0;
    size_t got = want ? fread(HA(buf), 1, (size_t)want, files[k].f) : 0;
    if (got < want) files[k].eof = 1;
    c->r[0] = size ? (uint32_t)(got / size) : 0;
    return 1;
}

static int native_fwrite(CPU *c) {
    uint32_t buf = c->r[0], size = c->r[1], n = c->r[2];
    int k;
    if (c->logging || (k = file_slot(c->r[3])) < 0) return 0;
    fwrite(HA(buf), 1, (size_t)size * n, files[k].f);
    c->r[0] = n;
    return 1;
}

static int native_fgets(CPU *c) {
    uint32_t buf = c->r[0], n = c->r[1];
    int k;
    if (c->logging || (k = file_slot(c->r[2])) < 0 || (int32_t)n <= 0) return 0;
    c->r[0] = fgets((char *)HA(buf), (int)n, files[k].f) ? buf : 0;
    return 1;
}

static int native_fseek(CPU *c) {
    int k;
    if (c->logging || (k = file_slot(c->r[0])) < 0) return 0;
    _fseeki64(files[k].f, (int64_t)(int32_t)c->r[1], (int)c->r[2]);
    files[k].eof = 0;
    c->r[0] = 0;
    return 1;
}

static int native_ftell(CPU *c) {
    int k;
    if (c->logging || (k = file_slot(c->r[0])) < 0) return 0;
    c->r[0] = (uint32_t)_ftelli64(files[k].f);
    return 1;
}

static int native_feof(CPU *c) {
    int k;
    if (c->logging || (k = file_slot(c->r[0])) < 0) return 0;
    c->r[0] = files[k].eof || feof(files[k].f) ? 1u : 0u;
    return 1;
}

static int native_fputs(CPU *c) {
    int k;
    if (c->logging || (k = file_slot(c->r[1])) < 0) return 0;
    fputs((const char *)HA(c->r[0]), files[k].f);
    c->r[0] = 1;
    return 1;
}

static int native_fputc(CPU *c) {
    int k;
    if (c->logging || (k = file_slot(c->r[1])) < 0) return 0;
    fputc((int)(c->r[0] & 0xFF), files[k].f);
    return 1;                                             /* returns its argument: r0 unchanged */
}
