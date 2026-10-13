/* Device file system: Android paths map into the host folder fs_root (plus the library's own path). POSIX
 * descriptors (files and sockets share one table, as on Linux) and the stdio calls lift.dll leaves to the host
 * (the standard streams, fdopen'd streams, fflush and friends). */
#include "host.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>
#include <winsock2.h>

char fs_root[1024];
char game_dir[1024];
char so_host_path[1024];

#define ENOENT 2
#define EBADF 9
#define EEXIST 17
#define EACCES 13

int host_path(const char *path, char *out, size_t cap, int create) {
    if (!strcmp(path, SO_DEVICE_PATH)) {
        if (strlen(so_host_path) + 1 > cap) return 0;
        strcpy(out, so_host_path);
        return 1;
    }
    while (*path == '/') path++;
    if (strlen(fs_root) + strlen(path) + 2 > cap) return 0;
    sprintf(out, "%s/%s", fs_root, path);
    if (create)
        for (char *p = out + strlen(fs_root) + 1; *p; p++)
            if (*p == '/') { *p = 0; _mkdir(out); *p = '/'; }
    return 1;
}

/* ---------------------------------------------------------------- descriptors */
#define FD_MAX 1024
typedef struct Fd { int kind; int crt; SOCKET s; int nonblock; } Fd;     /* kind: 0 free, 1 file, 2 socket */
static Fd fds[FD_MAX];
static SRWLOCK fd_lock = SRWLOCK_INIT;

int fd_new_socket(SOCKET s, int nonblock) {
    AcquireSRWLockExclusive(&fd_lock);
    for (int k = 10; k < FD_MAX; k++)
        if (!fds[k].kind) {
            fds[k].kind = 2;
            fds[k].s = s;
            fds[k].nonblock = nonblock;
            ReleaseSRWLockExclusive(&fd_lock);
            return k;
        }
    ReleaseSRWLockExclusive(&fd_lock);
    fatal("out of file descriptors");
}

static int fd_new_file(int crt) {
    AcquireSRWLockExclusive(&fd_lock);
    for (int k = 10; k < FD_MAX; k++)
        if (!fds[k].kind) {
            fds[k].kind = 1;
            fds[k].crt = crt;
            ReleaseSRWLockExclusive(&fd_lock);
            return k;
        }
    ReleaseSRWLockExclusive(&fd_lock);
    fatal("out of file descriptors");
}

Fd *fd_get(uint32_t fd) { return fd < FD_MAX && fds[fd].kind ? &fds[fd] : NULL; }
SOCKET fd_socket(uint32_t fd, int **nonblock) {
    Fd *f = fd_get(fd);
    if (!f || f->kind != 2) return INVALID_SOCKET;
    if (nonblock) *nonblock = &f->nonblock;
    return f->s;
}
int fd_is_socket(uint32_t fd) { Fd *f = fd_get(fd); return f && f->kind == 2; }

#define G_O_WRONLY 1
#define G_O_RDWR 2
#define G_O_CREAT 0x40
#define G_O_EXCL 0x80
#define G_O_TRUNC 0x200
#define G_O_APPEND 0x400

static void i_open(CPU *c) {
    const char *path = gstr(c->r[0]);
    uint32_t fl = c->r[1];
    char hp[2048];
    int oflag = _O_BINARY;
    if ((fl & 3) == G_O_WRONLY) oflag |= _O_WRONLY;
    else if ((fl & 3) == G_O_RDWR) oflag |= _O_RDWR;
    else oflag |= _O_RDONLY;
    if (fl & G_O_CREAT) oflag |= _O_CREAT;
    if (fl & G_O_EXCL) oflag |= _O_EXCL;
    if (fl & G_O_TRUNC) oflag |= _O_TRUNC;
    if (fl & G_O_APPEND) oflag |= _O_APPEND;
    int crt = -1;
    if (host_path(path, hp, sizeof(hp), (fl & 3) != 0)) crt = _open(hp, oflag, _S_IREAD | _S_IWRITE);
    if (crt < 0) {
        hlog("  open(%s, 0x%X) -> -1", path, fl);
        set_errno(ENOENT);
        RET(c, 0xFFFFFFFFu);
        return;
    }
    int fd = fd_new_file(crt);
    hlog("  open(%s, 0x%X) -> %d", path, fl, fd);
    RET(c, (uint32_t)fd);
}

static void i_close(CPU *c) {
    Fd *f = fd_get(c->r[0]);
    if (f) {
        if (f->kind == 1) _close(f->crt);
        else closesocket(f->s);
        AcquireSRWLockExclusive(&fd_lock);
        f->kind = 0;
        ReleaseSRWLockExclusive(&fd_lock);
    }
    RET(c, 0);
}

int net_recv(CPU *c, uint32_t fd, uint32_t buf, uint32_t n);
int net_send(CPU *c, uint32_t fd, uint32_t buf, uint32_t n);

static void i_read(CPU *c) {
    Fd *f = fd_get(c->r[0]);
    if (!f) { set_errno(EBADF); RET(c, 0xFFFFFFFFu); return; }
    if (f->kind == 2) { RET(c, (uint32_t)net_recv(c, c->r[0], c->r[1], c->r[2])); return; }
    RET(c, (uint32_t)_read(f->crt, GP(c->r[1]), c->r[2]));
}

static void i_write(CPU *c) {
    uint32_t fd = c->r[0], n = c->r[2];
    if (fd == 1 || fd == 2) {
        const char *s = GP(c->r[1]);
        uint32_t k = n;
        while (k && (s[k - 1] == '\n' || s[k - 1] == '\r')) k--;
        hlog("[fd%u] %.*s", fd, (int)k, s);
        RET(c, n);
        return;
    }
    Fd *f = fd_get(fd);
    if (!f) { set_errno(EBADF); RET(c, 0xFFFFFFFFu); return; }
    if (f->kind == 2) { RET(c, (uint32_t)net_send(c, fd, c->r[1], n)); return; }
    RET(c, (uint32_t)_write(f->crt, GP(c->r[1]), n));
}

static void i_lseek(CPU *c) {
    Fd *f = fd_get(c->r[0]);
    RET(c, f && f->kind == 1 ? (uint32_t)_lseek(f->crt, (long)(int32_t)c->r[1], (int)c->r[2]) : 0xFFFFFFFFu);
}

/* bionic arm32 struct stat (kernel stat64 layout, 104 bytes) */
static void stat_put(uint32_t buf, const struct _stat64 *st) {
    uint8_t b[104] = {0};
    uint64_t dev = st->st_dev, ino = st->st_ino;
    uint32_t mode = st->st_mode & _S_IFDIR ? 0040755 : 0100644, nlink = 1, uid = 10217;
    int64_t size = st->st_size, blocks = (st->st_size + 511) / 512;
    uint32_t blksize = 4096;
    memcpy(b, &dev, 8);
    memcpy(b + 12, &ino, 4);
    memcpy(b + 16, &mode, 4);
    memcpy(b + 20, &nlink, 4);
    memcpy(b + 24, &uid, 4);
    memcpy(b + 28, &uid, 4);
    memcpy(b + 48, &size, 8);
    memcpy(b + 56, &blksize, 4);
    memcpy(b + 64, &blocks, 8);
    uint32_t t[3] = {(uint32_t)st->st_atime, (uint32_t)st->st_mtime, (uint32_t)st->st_ctime};
    for (int k = 0; k < 3; k++) memcpy(b + 72 + 8 * k, &t[k], 4);
    memcpy(b + 96, &ino, 8);
    memcpy(GP(buf), b, sizeof(b));
}

static void i_stat(CPU *c) {
    char hp[2048];
    struct _stat64 st;
    if (!host_path(gstr(c->r[0]), hp, sizeof(hp), 0) || _stat64(hp, &st)) {
        set_errno(ENOENT);
        RET(c, 0xFFFFFFFFu);
        return;
    }
    stat_put(c->r[1], &st);
    RET(c, 0);
}

static void i_fstat(CPU *c) {
    Fd *f = fd_get(c->r[0]);
    struct _stat64 st;
    if (!f || f->kind != 1 || _fstat64(f->crt, &st)) { set_errno(EBADF); RET(c, 0xFFFFFFFFu); return; }
    stat_put(c->r[1], &st);
    RET(c, 0);
}

static void i_access(CPU *c) {
    char hp[2048];
    const char *p = gstr(c->r[0]);
    int ok = host_path(p, hp, sizeof(hp), 0) && GetFileAttributesA(hp) != INVALID_FILE_ATTRIBUTES;
    hlog("  access(%s) -> %d", p, ok ? 0 : -1);
    if (!ok) set_errno(ENOENT);
    RET(c, ok ? 0 : 0xFFFFFFFFu);
}

static void i_mkdir(CPU *c) {
    char hp[2048];
    if (!host_path(gstr(c->r[0]), hp, sizeof(hp), 0)) { set_errno(ENOENT); RET(c, 0xFFFFFFFFu); return; }
    if (_mkdir(hp) == 0) { RET(c, 0); return; }
    set_errno(errno == EEXIST ? EEXIST : ENOENT);
    RET(c, 0xFFFFFFFFu);
}

static void i_remove(CPU *c) {
    char hp[2048];
    if (!host_path(gstr(c->r[0]), hp, sizeof(hp), 0)) { RET(c, 0xFFFFFFFFu); return; }
    DWORD at = GetFileAttributesA(hp);
    int ok = at != INVALID_FILE_ATTRIBUTES && ((at & FILE_ATTRIBUTE_DIRECTORY) ? RemoveDirectoryA(hp) : DeleteFileA(hp));
    RET(c, ok ? 0 : 0xFFFFFFFFu);
}

/* Windows refuses to replace a file that is still open; POSIX allows it. Fall back to copying over it. */
static void i_rename(CPU *c) {
    char a[2048], b[2048];
    const char *src = gstr(c->r[0]), *dst = gstr(c->r[1]);
    if (!host_path(src, a, sizeof(a), 0) || !host_path(dst, b, sizeof(b), 1)) { RET(c, 0xFFFFFFFFu); return; }
    if (MoveFileExA(a, b, MOVEFILE_REPLACE_EXISTING)) { RET(c, 0); return; }
    if (CopyFileA(a, b, FALSE)) {
        DeleteFileA(a);
        RET(c, 0);
        return;
    }
    hlog("  rename(%s, %s) -> -1 (error %lu)", src, dst, GetLastError());
    set_errno(EACCES);
    RET(c, 0xFFFFFFFFu);
}

static void i_truncate(CPU *c) {
    char hp[2048];
    int crt = host_path(gstr(c->r[0]), hp, sizeof(hp), 0) ? _open(hp, _O_RDWR | _O_BINARY) : -1;
    if (crt < 0) { RET(c, 0xFFFFFFFFu); return; }
    int r = _chsize_s(crt, (int64_t)c->r[1]);
    _close(crt);
    RET(c, r ? 0xFFFFFFFFu : 0);
}

/* directories: DIR* is a guest block; the listing is taken at opendir */
typedef struct Dir { uint32_t g; char **names; int n, pos; uint32_t ent; } Dir;
#define DIR_MAX 64
static Dir dirs[DIR_MAX];
static SRWLOCK dir_lock = SRWLOCK_INIT;

static void i_opendir(CPU *c) {
    char hp[2048], pat[2100];
    if (!host_path(gstr(c->r[0]), hp, sizeof(hp), 0)) { RET(c, 0); return; }
    DWORD at = GetFileAttributesA(hp);
    if (at == INVALID_FILE_ATTRIBUTES || !(at & FILE_ATTRIBUTE_DIRECTORY)) { RET(c, 0); return; }
    snprintf(pat, sizeof(pat), "%s/*", hp);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat, &fd);
    Dir d = {0};
    if (h != INVALID_HANDLE_VALUE) {
        do {
            if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, "..")) continue;
            d.names = realloc(d.names, sizeof(char *) * (size_t)(d.n + 1));
            d.names[d.n++] = _strdup(fd.cFileName);
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    d.g = gzalloc(16);
    d.ent = gzalloc(280);
    AcquireSRWLockExclusive(&dir_lock);
    int k = 0;
    while (k < DIR_MAX && dirs[k].g) k++;
    if (k == DIR_MAX) fatal("too many open directories");
    dirs[k] = d;
    ReleaseSRWLockExclusive(&dir_lock);
    RET(c, d.g);
}

static Dir *dir_get(uint32_t g) {
    for (int k = 0; k < DIR_MAX; k++) if (dirs[k].g == g) return &dirs[k];
    return NULL;
}

static void i_readdir(CPU *c) {
    Dir *d = dir_get(c->r[0]);
    if (!d || d->pos >= d->n) { RET(c, 0); return; }
    /* bionic dirent: u64 d_ino, s64 d_off, u16 d_reclen, u8 d_type, char d_name[256] */
    uint8_t *e = GP(d->ent);
    memset(e, 0, 280);
    e[0] = 1;
    e[16] = 280 & 0xFF;
    e[17] = 280 >> 8;
    e[18] = 8;
    strncpy((char *)e + 19, d->names[d->pos++], 255);
    RET(c, d->ent);
}

static void i_closedir(CPU *c) {
    AcquireSRWLockExclusive(&dir_lock);
    Dir *d = dir_get(c->r[0]);
    if (d) {
        for (int k = 0; k < d->n; k++) free(d->names[k]);
        free(d->names);
        gfree(d->ent);
        gfree(d->g);
        memset(d, 0, sizeof(*d));
    }
    ReleaseSRWLockExclusive(&dir_lock);
    RET(c, 0);
}

static void i_basename(CPU *c) {
    char b[1024];
    strncpy(b, gstr(c->r[0]) ? gstr(c->r[0]) : "", sizeof(b) - 1);
    b[sizeof(b) - 1] = 0;
    size_t n = strlen(b);
    while (n > 1 && b[n - 1] == '/') b[--n] = 0;
    char *s = strrchr(b, '/');
    RET(c, galloc_str(s && s[1] ? s + 1 : (n ? b : "/")));
}

static void i_mmap(CPU *c) {
    uint32_t len = c->r[1], fd = ARG(c, 4), off = ARG(c, 5);
    uint32_t p = gzalloc(len);
    Fd *f = fd != 0xFFFFFFFFu ? fd_get(fd) : NULL;
    if (f && f->kind == 1) {
        long pos = _lseek(f->crt, 0, SEEK_CUR);
        _lseek(f->crt, (long)off, SEEK_SET);
        _read(f->crt, GP(p), len);
        _lseek(f->crt, pos, SEEK_SET);
    }
    RET(c, p);
}
static void i_munmap(CPU *c) { gfree(c->r[0]); RET(c, 0); }

/* ---------------------------------------------------------------- stdio the DLL leaves to the host:
 * the standard streams (__sF), and streams from fdopen (host CRT FILE* over the descriptor's CRT fd) */
static uint32_t std_streams;
typedef struct HStream { uint32_t g; FILE *f; } HStream;
#define HS_MAX 64
static HStream hstreams[HS_MAX];

static int is_std(uint32_t fp) { return std_streams && fp >= std_streams && fp < std_streams + 84 * 3; }
static FILE *hstream(uint32_t fp) {
    for (int k = 0; k < HS_MAX; k++) if (hstreams[k].g == fp) return hstreams[k].f;
    return NULL;
}
static FILE *stream_or_die(uint32_t fp, const char *what) {
    FILE *f = hstream(fp);
    if (!f) fatal("%s on unknown FILE* 0x%08X", what, fp);
    return f;
}

static void std_write(uint32_t fp, const char *s, size_t n) {
    static const char *names[] = {"stdin", "stdout", "stderr"};
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r')) n--;
    hlog("[%s] %.*s", names[(fp - std_streams) / 84], (int)n, s);
}

static void i_fdopen(CPU *c) {
    Fd *f = fd_get(c->r[0]);
    if (!f || f->kind != 1) { RET(c, 0); return; }
    char mode[8];
    strncpy(mode, gstr(c->r[1]), 5);
    mode[5] = 0;
    if (!strchr(mode, 'b')) strcat(mode, "b");
    FILE *h = _fdopen(_dup(f->crt), mode);
    if (!h) { RET(c, 0); return; }
    for (int k = 0; k < HS_MAX; k++)
        if (!hstreams[k].g) {
            hstreams[k].g = gzalloc(84);
            hstreams[k].f = h;
            RET(c, hstreams[k].g);
            return;
        }
    fatal("too many fdopen streams");
}

static void i_fwrite(CPU *c) {
    uint32_t n = c->r[1] * c->r[2], fp = c->r[3];
    if (is_std(fp)) { std_write(fp, GP(c->r[0]), n); RET(c, c->r[2]); return; }
    fwrite(GP(c->r[0]), 1, n, stream_or_die(fp, "fwrite"));
    RET(c, c->r[2]);
}
static void i_fputs(CPU *c) {
    const char *s = gstr(c->r[0]);
    if (is_std(c->r[1])) { std_write(c->r[1], s, strlen(s)); RET(c, 1); return; }
    fputs(s, stream_or_die(c->r[1], "fputs"));
    RET(c, 1);
}
static void i_fputc(CPU *c) {
    char ch = (char)c->r[0];
    if (is_std(c->r[1])) { std_write(c->r[1], &ch, 1); RET(c, c->r[0]); return; }
    fputc(ch, stream_or_die(c->r[1], "fputc"));
    RET(c, c->r[0]);
}
char *host_format(const char *fmt, VA *a, size_t *len);
static void i_fprintf(CPU *c) {
    VA a = va_regs(c, 2);
    size_t len;
    char *s = host_format(gstr(c->r[1]), &a, &len);
    if (is_std(c->r[0])) std_write(c->r[0], s, len);
    else fwrite(s, 1, len, stream_or_die(c->r[0], "fprintf"));
    free(s);
    RET(c, (uint32_t)len);
}
static void i_fread(CPU *c) {
    FILE *f = stream_or_die(c->r[3], "fread");
    size_t want = (size_t)c->r[1] * c->r[2], got = fread(GP(c->r[0]), 1, want, f);
    RET(c, c->r[1] ? (uint32_t)(got / c->r[1]) : 0);
}
static void i_fgets(CPU *c) {
    FILE *f = stream_or_die(c->r[2], "fgets");
    RET(c, fgets(GP(c->r[0]), (int)c->r[1], f) ? c->r[0] : 0);
}
static void i_fclose(CPU *c) {
    for (int k = 0; k < HS_MAX; k++)
        if (hstreams[k].g == c->r[0]) {
            fclose(hstreams[k].f);
            gfree(hstreams[k].g);
            hstreams[k].g = 0;
        }
    RET(c, 0);
}
static void i_fseek(CPU *c) {
    RET(c, (uint32_t)_fseeki64(stream_or_die(c->r[0], "fseek"), (int32_t)c->r[1], (int)c->r[2]));
}
static void i_ftell(CPU *c) { RET(c, (uint32_t)_ftelli64(stream_or_die(c->r[0], "ftell"))); }
static void i_feof(CPU *c) { RET(c, feof(stream_or_die(c->r[0], "feof")) ? 1 : 0); }
static void i_fflush(CPU *c) {
    FILE *f = c->r[0] ? hstream(c->r[0]) : NULL;
    if (f) fflush(f);
    RET(c, 0);
}
static void i_ok(CPU *c) { RET(c, 0); }
static void i_freopen(CPU *c) { RET(c, c->r[2]); }

static void i_fopen(CPU *c) {
    /* lift.dll's fopen declined: only when a stream table or path is unusable */
    fatal("fopen(%s, %s) declined by lift.dll", gstr(c->r[0]), gstr(c->r[1]));
}

void fs_init(void) {
    std_streams = data_import("__sF");
    strncpy(L.fs_root, fs_root, 1023);
    strncpy(L.so_guest, SO_DEVICE_PATH, 255);
    strncpy(L.so_host, so_host_path, 1023);
    imp("open", i_open);
    imp("close", i_close);
    imp("read", i_read);
    imp("write", i_write);
    imp("lseek", i_lseek);
    imp("stat", i_stat);
    imp("fstat", i_fstat);
    imp("access", i_access);
    imp("mkdir", i_mkdir);
    imp("remove", i_remove);
    imp("unlink", i_remove);
    imp("rename", i_rename);
    imp("truncate", i_truncate);
    imp("opendir", i_opendir);
    imp("readdir", i_readdir);
    imp("closedir", i_closedir);
    imp("basename", i_basename);
    imp("mmap", i_mmap);
    imp("munmap", i_munmap);
    imp("fdopen", i_fdopen);
    imp("fopen", i_fopen);
    imp("fwrite", i_fwrite);
    imp("fputs", i_fputs);
    imp("fputc", i_fputc);
    imp("fputwc", i_fputc);
    imp("fprintf", i_fprintf);
    imp("fread", i_fread);
    imp("fgets", i_fgets);
    imp("fclose", i_fclose);
    imp("fseek", i_fseek);
    imp("fseeko", i_fseek);
    imp("ftell", i_ftell);
    imp("ftello", i_ftell);
    imp("feof", i_feof);
    imp("fflush", i_fflush);
    imp("setvbuf", i_ok);
    imp("freopen", i_freopen);
}
