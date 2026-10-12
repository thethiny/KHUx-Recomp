"""High-level emulation of bionic libc / libm / liblog / libdl imports.

Handlers receive the Emu and read AAPCS (soft-float) arguments themselves.
"""
import math
import os
import struct
import time
from pathlib import Path

from unicorn.arm_const import UC_ARM_REG_LR, UC_ARM_REG_SP

from .. import paths
from .core import GuestFault

H = {}

# Android paths are mapped into this host directory
FS_ROOT = paths.FS_ROOT
PACKAGE = "com.square_enix.android_googleplay.khuxww"

ENOENT, EINVAL, EAGAIN, ENOSYS = 2, 22, 11, 38


def imp(*names):
    def deco(fn):
        for n in names:
            H[n] = fn
        return fn
    return deco


def install(emu):
    emu.handlers.update(H)
    emu.libc = LibcState(emu)


class LibcState:
    def __init__(self, emu):
        self.emu = emu
        self.errno_ptr = emu.alloc(4)
        self.atexit = []
        self.tls_keys = {}
        self.next_key = 1
        self.once_done = set()
        self.files = {}      # guest FILE* -> host file object
        self.fds = {}        # fd -> host file object
        self.next_fd = 10
        self.dirs = {}
        self.rand_state = 0x1234ABCD330E
        self.strtok_state = {}

    def set_errno(self, v):
        self.emu.w32(self.errno_ptr, v)


# ---------------------------------------------------------------------------------------- data imports
def make_data_import(emu, name):
    if name == "__stack_chk_guard":
        return emu.alloc(4, struct.pack("<I", 0xC0FFEE00))
    if name == "__sF":
        # bionic FILE is 84 bytes on 32-bit; stdin/stdout/stderr = __sF[0..2]
        return emu.alloc(84 * 3)
    if name == "_ctype_":
        # bionic: _ctype_ points one before the table (index -1 = EOF); flags per BSD ctype
        _U, _L, _N, _S, _P, _C, _X, _B = 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80
        tbl = [0] * 257
        for c in range(256):
            f = 0
            ch = chr(c) if c < 128 else ""
            if ch.isupper():
                f |= _U
            if ch.islower():
                f |= _L
            if ch.isdigit():
                f |= _N
            if ch in " \t\n\v\f\r":
                f |= _S
            if c < 128 and ch in "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~":
                f |= _P
            if c < 32 or c == 127:
                f |= _C
            if ch in "0123456789abcdefABCDEF" and ch:
                f |= _X
            if c == 32:
                f |= _B
            tbl[c + 1] = f
        # bionic: `extern const char *_ctype_;` is a pointer variable; macros index (_ctype_ + 1)[c]
        table = emu.alloc(260, bytes(tbl) + bytes(3))
        return emu.alloc(4, struct.pack("<I", table))
    if name in ("_tolower_tab_", "_toupper_tab_"):
        # `extern const short *_tolower_tab_;` -> pointer to 257 shorts (index 0 = EOF)
        f = str.lower if name == "_tolower_tab_" else str.upper
        vals = [0xFFFF] + [ord(f(chr(c))) if c < 128 else c for c in range(256)]
        table = emu.alloc(2 * 257, struct.pack("<257H", *vals))
        return emu.alloc(4, struct.pack("<I", table))
    if name.startswith("SL_IID_"):
        # SLInterfaceID is a pointer to a 16-byte GUID; store a unique fake GUID
        guid = emu.alloc(16, name.encode()[:16].ljust(16, b"\0"))
        if not hasattr(emu, "sl_iids"):
            emu.sl_iids = {}
        emu.sl_iids[guid] = name[len("SL_IID_"):]
        return emu.alloc(4, struct.pack("<I", guid))
    emu.log(f"?? unknown data import {name}, giving 256 zero bytes")
    return emu.alloc(256)


# ---------------------------------------------------------------------------------------- varargs
class Args:
    """Sequential AAPCS argument reader starting at argument word `start`.
    Covers r0-r3 then the stack; doubles/long longs are 8-byte aligned."""

    def __init__(self, emu, start=0, va_ptr=None):
        self.e = emu
        self.idx = start
        self.va = va_ptr
        self.sp = emu.reg(UC_ARM_REG_SP)
        self.regs = emu.args(4)

    def _word(self):
        if self.va is not None:
            v = self.e.u32(self.va)
            self.va += 4
            return v
        i = self.idx
        self.idx += 1
        if i < 4:
            return self.regs[i]
        return self.e.u32(self.sp + 4 * (i - 4))

    def i(self):
        return self._word()

    def s32(self):
        v = self._word()
        return v - (1 << 32) if v & 0x80000000 else v

    def u64(self):
        if self.va is not None:
            self.va = (self.va + 7) & ~7
        elif self.idx % 2:
            self.idx += 1
        lo = self._word()
        hi = self._word()
        return lo | (hi << 32)

    def s64(self):
        v = self.u64()
        return v - (1 << 64) if v >> 63 else v

    def d(self):
        return struct.unpack("<d", struct.pack("<Q", self.u64()))[0]


def f2u(x):
    return struct.unpack("<I", struct.pack("<f", x))[0]


def u2f(v):
    return struct.unpack("<f", struct.pack("<I", v & 0xFFFFFFFF))[0]


def d2u(x):
    return struct.unpack("<Q", struct.pack("<d", x))[0]


def format_c(e, fmt, a):
    """printf-style formatting reading guest args from Args `a`."""
    out = []
    i = 0
    n = len(fmt)
    while i < n:
        c = fmt[i]
        if c != "%":
            out.append(c)
            i += 1
            continue
        j = i + 1
        flags = ""
        while j < n and fmt[j] in "-+ #0":
            flags += fmt[j]
            j += 1
        width = ""
        if j < n and fmt[j] == "*":
            width = str(a.s32())
            j += 1
        else:
            while j < n and fmt[j].isdigit():
                width += fmt[j]
                j += 1
        prec = None
        if j < n and fmt[j] == ".":
            j += 1
            prec = ""
            if j < n and fmt[j] == "*":
                prec = str(a.s32())
                j += 1
            else:
                while j < n and fmt[j].isdigit():
                    prec += fmt[j]
                    j += 1
        length = ""
        while j < n and fmt[j] in "hlLqjzt":
            length += fmt[j]
            j += 1
        if j >= n:
            break
        conv = fmt[j]
        spec = "%" + flags + width + ("." + prec if prec is not None else "")
        if conv == "%":
            out.append("%")
        elif conv in "di":
            v = a.s64() if length in ("ll", "q", "j") else a.s32()
            if length == "h":
                v = ((v & 0xFFFF) ^ 0x8000) - 0x8000
            elif length == "hh":
                v = ((v & 0xFF) ^ 0x80) - 0x80
            out.append((spec + "d") % v)
        elif conv in "uxXo":
            v = a.u64() if length in ("ll", "q", "j") else a.i()
            if length == "h":
                v &= 0xFFFF
            elif length == "hh":
                v &= 0xFF
            out.append((spec + conv.replace("u", "d")) % v)
        elif conv in "fFeEgGaA":
            v = a.d()
            if conv in "aA":
                out.append(float.hex(v))
            else:
                out.append((spec + conv) % v)
        elif conv == "c":
            out.append((spec + "c") % chr(a.i() & 0xFF))
        elif conv == "s":
            p = a.i()
            s = e.cstr(p) if p else "(null)"
            out.append((spec + "s") % s)
        elif conv == "p":
            out.append("0x%x" % a.i())
        elif conv == "n":
            a.i()
        else:
            out.append(fmt[i:j + 1])
        i = j + 1
    return "".join(out)


def guest_str(e, p):
    return e.cstr(p)


# ---------------------------------------------------------------------------------------- memory
@imp("malloc")
def _malloc(e):
    e.ret(e.heap.malloc(e.args(1)[0]))


@imp("calloc")
def _calloc(e):
    n, s = e.args(2)
    p = e.heap.malloc(n * s)
    e.write(p, b"\0" * (n * s))
    e.ret(p)


@imp("realloc")
def _realloc(e):
    p, n = e.args(2)
    if not p:
        e.ret(e.heap.malloc(n))
        return
    if n == 0:
        e.heap.free(p)
        e.ret(0)
        return
    old = e.heap.usable(p)
    if old >= n or e.heap.grow_in_place(p, n):
        e.ret(p)
        return
    q = e.heap.malloc(n)
    e.write(q, e.read(p, old))
    e.heap.free(p)
    e.ret(q)


@imp("free")
def _free(e):
    e.heap.free(e.args(1)[0])
    e.ret(0)


@imp("memcpy", "memmove", "__aeabi_memcpy", "__aeabi_memmove")
def _memcpy(e):
    d, s, n = e.args(3)
    if n:
        e.write(d, e.read(s, n))
    e.ret(d)


@imp("memset")
def _memset(e):
    d, c, n = e.args(3)
    if n:
        e.write(d, bytes([c & 0xFF]) * n)
    e.ret(d)


@imp("__aeabi_memset")
def _aeabi_memset(e):  # (dest, n, c)
    d, n, c = e.args(3)
    if n:
        e.write(d, bytes([c & 0xFF]) * n)
    e.ret(d)


@imp("bzero")
def _bzero(e):
    d, n = e.args(2)
    if n:
        e.write(d, b"\0" * n)
    e.ret(0)


@imp("memcmp")
def _memcmp(e):
    a, b, n = e.args(3)
    x, y = e.read(a, n), e.read(b, n)
    e.ret(0 if x == y else (-1 if x < y else 1))


@imp("memchr")
def _memchr(e):
    p, c, n = e.args(3)
    k = e.read(p, n).find(bytes([c & 0xFF]))
    e.ret(p + k if k >= 0 else 0)


@imp("memrchr")
def _memrchr(e):
    p, c, n = e.args(3)
    k = e.read(p, n).rfind(bytes([c & 0xFF]))
    e.ret(p + k if k >= 0 else 0)


@imp("memmem")
def _memmem(e):
    h, hn, nd, nn = e.args(4)
    k = e.read(h, hn).find(e.read(nd, nn))
    e.ret(h + k if k >= 0 else 0)


# ---------------------------------------------------------------------------------------- strings
@imp("strlen")
def _strlen(e):
    e.ret(len(e.cbytes(e.args(1)[0])))


@imp("strcmp")
def _strcmp(e):
    a, b = e.args(2)
    x, y = e.cbytes(a), e.cbytes(b)
    e.ret(0 if x == y else (-1 if x < y else 1))


@imp("strncmp")
def _strncmp(e):
    a, b, n = e.args(3)
    x, y = e.cbytes(a)[:n], e.cbytes(b)[:n]
    e.ret(0 if x == y else (-1 if x < y else 1))


@imp("strcasecmp")
def _strcasecmp(e):
    a, b = e.args(2)
    x, y = e.cbytes(a).lower(), e.cbytes(b).lower()
    e.ret(0 if x == y else (-1 if x < y else 1))


@imp("strncasecmp")
def _strncasecmp(e):
    a, b, n = e.args(3)
    x, y = e.cbytes(a)[:n].lower(), e.cbytes(b)[:n].lower()
    e.ret(0 if x == y else (-1 if x < y else 1))


@imp("strcpy")
def _strcpy(e):
    d, s = e.args(2)
    e.write(d, e.cbytes(s) + b"\0")
    e.ret(d)


@imp("strncpy")
def _strncpy(e):
    d, s, n = e.args(3)
    b = e.cbytes(s)[:n]
    e.write(d, b + b"\0" * (n - len(b)))
    e.ret(d)


@imp("strlcpy")
def _strlcpy(e):
    d, s, n = e.args(3)
    b = e.cbytes(s)
    if n:
        e.write(d, b[:n - 1] + b"\0")
    e.ret(len(b))


@imp("strcat")
def _strcat(e):
    d, s = e.args(2)
    e.write(d + len(e.cbytes(d)), e.cbytes(s) + b"\0")
    e.ret(d)


@imp("strdup")
def _strdup(e):
    b = e.cbytes(e.args(1)[0])
    e.ret(e.alloc(len(b) + 1, b + b"\0"))


@imp("strchr")
def _strchr(e):
    p, c = e.args(2)
    b = e.cbytes(p)
    c &= 0xFF
    if c == 0:
        e.ret(p + len(b))
        return
    k = b.find(bytes([c]))
    e.ret(p + k if k >= 0 else 0)


@imp("strrchr")
def _strrchr(e):
    p, c = e.args(2)
    b = e.cbytes(p)
    c &= 0xFF
    if c == 0:
        e.ret(p + len(b))
        return
    k = b.rfind(bytes([c]))
    e.ret(p + k if k >= 0 else 0)


@imp("strstr")
def _strstr(e):
    h, nd = e.args(2)
    k = e.cbytes(h).find(e.cbytes(nd))
    e.ret(h + k if k >= 0 else 0)


@imp("strpbrk")
def _strpbrk(e):
    p, acc = e.args(2)
    b, s = e.cbytes(p), set(e.cbytes(acc))
    for k, ch in enumerate(b):
        if ch in s:
            e.ret(p + k)
            return
    e.ret(0)


@imp("strspn")
def _strspn(e):
    p, acc = e.args(2)
    b, s = e.cbytes(p), set(e.cbytes(acc))
    k = 0
    while k < len(b) and b[k] in s:
        k += 1
    e.ret(k)


@imp("strcspn")
def _strcspn(e):
    p, rej = e.args(2)
    b, s = e.cbytes(p), set(e.cbytes(rej))
    k = 0
    while k < len(b) and b[k] not in s:
        k += 1
    e.ret(k)


@imp("strtok_r")
def _strtok_r(e):
    s, delim, save = e.args(3)
    d = set(e.cbytes(delim))
    if not s:
        s = e.u32(save)
    if not s:
        e.ret(0)
        return
    b = e.cbytes(s)
    k = 0
    while k < len(b) and b[k] in d:
        k += 1
    if k == len(b):
        e.w32(save, 0)
        e.ret(0)
        return
    start = k
    while k < len(b) and b[k] not in d:
        k += 1
    if k < len(b):
        e.write(s + k, b"\0")
        e.w32(save, s + k + 1)
    else:
        e.w32(save, 0)
    e.ret(s + start)


@imp("strerror")
def _strerror(e):
    e.ret(e.alloc_str(f"error {e.args(1)[0]}"))


@imp("strerror_r")
def _strerror_r(e):
    n, buf, ln = e.args(3)
    b = f"error {n}".encode()[:max(0, ln - 1)]
    e.write(buf, b + b"\0")
    e.ret(0)


def _strto(e, signed, bits):
    p, endp, base = e.args(3)
    b = e.cbytes(p)
    k = 0
    while k < len(b) and chr(b[k]) in " \t\n\v\f\r":
        k += 1
    start = k
    neg = False
    if k < len(b) and chr(b[k]) in "+-":
        neg = chr(b[k]) == "-"
        k += 1
    if base in (0, 16) and b[k:k + 2].lower() == b"0x" and k + 2 < len(b) and \
            chr(b[k + 2]) in "0123456789abcdefABCDEF":
        k += 2
        base = 16
    elif base == 0:
        base = 8 if b[k:k + 1] == b"0" else 10
    digits_start = k
    v = 0
    while k < len(b):
        ch = chr(b[k]).lower()
        dv = int(ch, 36) if ch.isalnum() else 99
        if dv >= base:
            break
        v = v * base + dv
        k += 1
    if k == digits_start:
        k = start = 0
        v = 0
    if neg:
        v = -v
    lo, hi = (-(1 << (bits - 1)), (1 << (bits - 1)) - 1) if signed else (-(1 << bits) + 1, (1 << bits) - 1)
    v = max(lo, min(hi, v))
    if endp:
        e.w32(endp, p + k if k else p)
    if bits == 64:
        e.ret64(v)
    else:
        e.ret(v)


@imp("strtol")
def _strtol(e):
    _strto(e, True, 32)


@imp("strtoul")
def _strtoul(e):
    _strto(e, False, 32)


@imp("strtoll", "strtoumax")
def _strtoll(e):
    _strto(e, True, 64)


@imp("strtoull")
def _strtoull(e):
    _strto(e, False, 64)


@imp("atoi", "atol")
def _atoi(e):
    s = e.cbytes(e.args(1)[0]).strip()
    num = ""
    for k, ch in enumerate(s.decode("latin1")):
        if ch.isdigit() or (k == 0 and ch in "+-"):
            num += ch
        else:
            break
    try:
        e.ret(int(num))
    except ValueError:
        e.ret(0)


@imp("tolower")
def _tolower(e):
    c = e.args(1)[0]
    e.ret(c + 32 if 65 <= c <= 90 else c)


@imp("toupper")
def _toupper(e):
    c = e.args(1)[0]
    e.ret(c - 32 if 97 <= c <= 122 else c)


@imp("isspace")
def _isspace(e):
    e.ret(int(e.args(1)[0] in (9, 10, 11, 12, 13, 32)))


@imp("isupper")
def _isupper(e):
    e.ret(int(65 <= e.args(1)[0] <= 90))


@imp("isprint")
def _isprint(e):
    e.ret(int(32 <= e.args(1)[0] < 127))


@imp("isblank")
def _isblank(e):
    e.ret(int(e.args(1)[0] in (9, 32)))


@imp("wcscmp")
def _wcscmp(e):
    a, b = e.args(2)
    k = 0
    while True:
        x, y = e.u32(a + k), e.u32(b + k)
        if x != y or x == 0:
            e.ret(0 if x == y else (-1 if x < y else 1))
            return
        k += 4


# ---------------------------------------------------------------------------------------- printf family
@imp("snprintf")
def _snprintf(e):
    buf, n, fmt = e.args(3)
    s = format_c(e, e.cstr(fmt), Args(e, 3)).encode()
    if n:
        e.write(buf, s[:n - 1] + b"\0")
    e.ret(len(s))


@imp("vsnprintf")
def _vsnprintf(e):
    buf, n, fmt, va = e.args(4)
    s = format_c(e, e.cstr(fmt), Args(e, va_ptr=va)).encode()
    if n:
        e.write(buf, s[:n - 1] + b"\0")
    e.ret(len(s))


@imp("sprintf")
def _sprintf(e):
    buf, fmt = e.args(2)
    s = format_c(e, e.cstr(fmt), Args(e, 2)).encode()
    e.write(buf, s + b"\0")
    e.ret(len(s))


@imp("vsprintf")
def _vsprintf(e):
    buf, fmt, va = e.args(3)
    s = format_c(e, e.cstr(fmt), Args(e, va_ptr=va)).encode()
    e.write(buf, s + b"\0")
    e.ret(len(s))


@imp("vasprintf")
def _vasprintf(e):
    out, fmt, va = e.args(3)
    s = format_c(e, e.cstr(fmt), Args(e, va_ptr=va)).encode()
    e.w32(out, e.alloc(len(s) + 1, s + b"\0"))
    e.ret(len(s))


def _stdout(e, s):
    e.log("[stdout] " + s.rstrip("\n"))


@imp("printf")
def _printf(e):
    s = format_c(e, e.cstr(e.args(1)[0]), Args(e, 1))
    _stdout(e, s)
    e.ret(len(s))


@imp("puts")
def _puts(e):
    _stdout(e, e.cstr(e.args(1)[0]))
    e.ret(1)


@imp("putchar")
def _putchar(e):
    e.ret(e.args(1)[0])


@imp("fprintf")
def _fprintf(e):
    f, fmt = e.args(2)
    s = format_c(e, e.cstr(fmt), Args(e, 2))
    _fwrite_host(e, f, s.encode())
    e.ret(len(s))


@imp("perror")
def _perror(e):
    e.log("[perror] " + (e.cstr(e.args(1)[0]) or ""))
    e.ret(0)


# ---------------------------------------------------------------------------------------- android log
LOG_LEVELS = "??VDIWEFS"


@imp("__android_log_print")
def _alog_print(e):
    prio, tag, fmt = e.args(3)
    msg = format_c(e, e.cstr(fmt), Args(e, 3))
    e.log(f"[log {LOG_LEVELS[prio] if prio < 9 else prio}/{e.cstr(tag)}] {msg}")
    e.ret(len(msg))


@imp("__android_log_write")
def _alog_write(e):
    prio, tag, msg = e.args(3)
    e.log(f"[log {LOG_LEVELS[prio] if prio < 9 else prio}/{e.cstr(tag)}] {e.cstr(msg)}")
    e.ret(1)


@imp("syslog")
def _syslog(e):
    prio, fmt = e.args(2)
    e.log(f"[syslog {prio}] " + format_c(e, e.cstr(fmt), Args(e, 2)))
    e.ret(0)


# ---------------------------------------------------------------------------------------- process / misc
@imp("__cxa_atexit", "__aeabi_atexit")
def _cxa_atexit(e):
    e.libc.atexit.append(tuple(e.args(3)))
    e.ret(0)


@imp("__cxa_finalize")
def _cxa_finalize(e):
    e.ret(0)


@imp("__errno")
def _errno(e):
    e.ret(e.libc.errno_ptr)


@imp("__stack_chk_fail")
def _stack_chk_fail(e):
    raise GuestFault(f"__stack_chk_fail (lr={e.reg(UC_ARM_REG_LR):#x})")


@imp("abort")
def _abort(e):
    e.dump()     # registers + return addresses on the stack: who threw (uncaught C++ exceptions end here)
    raise GuestFault(f"abort() (lr={e.reg(UC_ARM_REG_LR):#x})")


@imp("exit")
def _exit(e):
    raise GuestFault(f"exit({e.args(1)[0]}) (lr={e.reg(UC_ARM_REG_LR):#x})")


@imp("__assert2")
def _assert2(e):
    file, line, func, expr = e.args(4)
    raise GuestFault(f"assert {e.cstr(expr)} at {e.cstr(file)}:{line} in {e.cstr(func)}")


@imp("getpid", "getppid")
def _getpid(e):
    e.ret(4242)


@imp("getuid", "geteuid", "getgid", "getegid")
def _getuid(e):
    e.ret(10217)


@imp("getenv")
def _getenv(e):
    e.ret(0)


@imp("setlocale")
def _setlocale(e):
    e.ret(e.alloc_str("C"))


@imp("sysconf")
def _sysconf(e):
    name = e.args(1)[0]
    # _SC_PAGESIZE=0x27, _SC_NPROCESSORS_CONF=0x60, _SC_NPROCESSORS_ONLN=0x61
    e.ret({0x27: 4096, 0x28: 4096, 0x60: 4, 0x61: 4}.get(name, 0))


@imp("getdtablesize")
def _getdtablesize(e):
    e.ret(1024)


@imp("bsd_signal", "sigaction", "sigprocmask", "raise", "kill", "alarm", "umask", "setsid", "setuid",
     "setgid", "chdir", "chmod", "pause")
def _ignore_ok(e):
    e.ret(0)


@imp("fork", "execl", "waitpid", "system", "pipe", "socketpair", "dup2")
def _unsupported(e):
    e.libc.set_errno(ENOSYS)
    e.ret(0xFFFFFFFF)


@imp("srand48")
def _srand48(e):
    e.libc.rand_state = ((e.args(1)[0] << 16) | 0x330E) & 0xFFFFFFFFFFFF
    e.ret(0)


@imp("lrand48")
def _lrand48(e):
    st = e.libc
    st.rand_state = (st.rand_state * 0x5DEECE66D + 0xB) & 0xFFFFFFFFFFFF
    e.ret(st.rand_state >> 17)


@imp("qsort")
def _qsort(e):
    base, n, size, cmp = e.args(4)
    import functools
    if n < 2:
        e.ret(0)
        return
    if n >= 64:
        lr = e.reg(UC_ARM_REG_LR)
        seen = e.libc.__dict__.setdefault("qsort_sites", {})
        seen[lr] = seen.get(lr, 0) + 1
        if seen[lr] in (1, 100, 1000):
            e.log(f"qsort n={n} size={size} from {e.sym(lr)} (x{seen[lr]})")
    # sort indices; the comparator reads elements in place from an unchanging copy (no per-compare writes)
    data = e.read(base, n * size)
    copy = e.alloc(n * size)
    e.write(copy, data)
    fn, done = e.caller(cmp)
    try:
        order = sorted(range(n), key=functools.cmp_to_key(lambda i, j: fn(copy + i * size, copy + j * size)))
    finally:
        done()
    e.write(base, b"".join(data[i * size:(i + 1) * size] for i in order))
    e.heap.free(copy)
    e.ret(0)


@imp("bsearch")
def _bsearch(e):
    key, base, n, size, cmp = e.args(5)
    lo, hi = 0, n
    while lo < hi:
        mid = (lo + hi) // 2
        r = e.call(cmp, key, base + mid * size)
        r = r - (1 << 32) if r & 0x80000000 else r
        if r == 0:
            e.ret(base + mid * size)
            return
        if r < 0:
            hi = mid
        else:
            lo = mid + 1
    e.ret(0)


@imp("div")
def _div(e):
    # AAPCS: div_t (8 bytes) is returned in memory -> hidden result pointer in r0, numer/denom in r1/r2.
    # (Returning it in r0:r1 gave LuxBoard::setNumber garbage digits -> out-of-range armature display.)
    out, a, b = e.args(3)
    a = a - (1 << 32) if a & 0x80000000 else a
    b = b - (1 << 32) if b & 0x80000000 else b
    q = int(a / b)   # C truncates toward zero
    e.write(out, struct.pack("<ii", q, a - q * b))
    e.ret(out)


# ---------------------------------------------------------------------------------------- time
def _now():
    return time.time()


@imp("time")
def _time(e):
    t = int(_now())
    p = e.args(1)[0]
    if p:
        e.w32(p, t)
    e.ret(t)


@imp("gettimeofday")
def _gettimeofday(e):
    tv = e.args(1)[0]
    t = _now()
    if tv:
        e.write(tv, struct.pack("<II", int(t), int((t % 1) * 1e6)))
    e.ret(0)


@imp("clock_gettime")
def _clock_gettime(e):
    clk, ts = e.args(2)
    t = time.monotonic() if clk in (1, 4, 7) else _now()
    e.write(ts, struct.pack("<II", int(t), int((t % 1) * 1e9)))
    e.ret(0)


@imp("nanosleep")
def _nanosleep(e):
    req = e.args(1)[0]
    s, ns = struct.unpack("<II", e.read(req, 8))
    time.sleep(min(s + ns / 1e9, 1.0))
    e.ret(0)


@imp("usleep")
def _usleep(e):
    time.sleep(min(e.args(1)[0] / 1e6, 1.0))
    e.ret(0)


@imp("sched_yield")
def _sched_yield(e):
    e.ret(0)


def _tm_pack(t):
    # struct tm (bionic 32-bit): 9 ints + long tm_gmtoff + const char* tm_zone = 44 bytes
    return struct.pack("<9iiI", t.tm_sec, t.tm_min, t.tm_hour, t.tm_mday, t.tm_mon - 1, t.tm_year - 1900,
                       (t.tm_wday + 1) % 7, t.tm_yday - 1, t.tm_isdst, 0, 0)


@imp("gmtime_r")
def _gmtime_r(e):
    tp, out = e.args(2)
    e.write(out, _tm_pack(time.gmtime(struct.unpack("<i", e.read(tp, 4))[0])))
    e.ret(out)


@imp("gmtime")
def _gmtime(e):
    tp = e.args(1)[0]
    out = e.alloc(44)
    e.write(out, _tm_pack(time.gmtime(struct.unpack("<i", e.read(tp, 4))[0])))
    e.ret(out)


@imp("localtime")
def _localtime(e):
    tp = e.args(1)[0]
    out = e.alloc(44)
    e.write(out, _tm_pack(time.localtime(struct.unpack("<i", e.read(tp, 4))[0])))
    e.ret(out)


@imp("timegm64")
def _timegm64(e):
    import calendar
    v = struct.unpack("<9i", e.read(e.args(1)[0], 36))
    e.ret64(calendar.timegm((v[5] + 1900, v[4] + 1, v[3], v[2], v[1], v[0], 0, 0, 0)))


@imp("strftime")
def _strftime(e):
    buf, mx, fmt, tm = e.args(4)
    v = struct.unpack("<9i", e.read(tm, 36))
    t = (v[5] + 1900, v[4] + 1, v[3], v[2], v[1], v[0], (v[6] - 1) % 7, v[7] + 1, v[8])
    s = time.strftime(e.cstr(fmt), t).encode()
    if len(s) >= mx:
        e.ret(0)
        return
    e.write(buf, s + b"\0")
    e.ret(len(s))


# pthreads, sleeping and blocking calls live in threads.py


# ---------------------------------------------------------------------------------------- setjmp
# The jmp_buf contents live on the host, keyed by the guest buffer address: callee-saved r4-r11, sp and
# the return address (setjmp's LR). longjmp restores them and writes PC = that LR; from inside a trap
# hook a PC write is a jump, which is exactly the non-local return wanted (Thumb bit comes from LR).
_JMP_REGS = None


def _jmp_regs():
    global _JMP_REGS
    if _JMP_REGS is None:
        from unicorn.arm_const import UC_ARM_REG_R4
        _JMP_REGS = [UC_ARM_REG_R4 + k for k in range(8)] + [UC_ARM_REG_SP]
    return _JMP_REGS


@imp("setjmp", "sigsetjmp")
def _setjmp(e):
    buf = e.args(1)[0]
    if not hasattr(e, "jmpbufs"):
        e.jmpbufs = {}
    e.jmpbufs[buf] = ([e.reg(r) for r in _jmp_regs()], e.reg(UC_ARM_REG_LR))
    e.ret(0)


@imp("longjmp", "siglongjmp")
def _longjmp(e):
    buf, val = e.args(2)
    saved = getattr(e, "jmpbufs", {}).get(buf)
    if saved is None:
        raise GuestFault(f"longjmp to unknown jmp_buf {buf:#x} (lr={e.reg(UC_ARM_REG_LR):#x})")
    regs, ret = saved
    e.log(f"  longjmp({buf:#x}, {val}) -> {e.sym(ret)}")
    if "rapidjson" in e.sym(ret):
        # this rapidjson reports parse errors with longjmp; dump the words after the jmp_buf to locate
        # GenericReader::parseError_ (const char*) / errorOffset_
        words = struct.unpack("<12I", e.read(buf + 248, 48))
        for k, w in enumerate(words):
            s = e.cstr(w, 80) if 0x1000 <= w < 0x80000000 else None
            if s and s.isprintable() and len(s) > 3:
                e.log(f"    rapidjson error @jmpbuf+{248 + 4 * k}: {s!r} (next word {words[k + 1] if k + 1 < 12 else 0})")
    for r, v in zip(_jmp_regs(), regs):
        e.set_reg(r, v)
    e.ret(val or 1)
    from unicorn.arm_const import UC_ARM_REG_PC
    e.set_reg(UC_ARM_REG_PC, ret)


# ---------------------------------------------------------------------------------------- dl
@imp("dlopen")
def _dlopen(e):
    name = e.cstr(e.args(1)[0])
    e.log(f"  dlopen({name})")
    e.ret(0x1)


@imp("dlsym")
def _dlsym(e):
    h, name = e.args(2)
    n = e.cstr(name)
    if n in e.symbols:
        e.ret(e.symbols[n])
    elif n in e.handlers:
        e.ret(e.slot(n))
    else:
        e.log(f"  dlsym({n}) -> NULL")
        e.ret(0)


@imp("dlclose")
def _dlclose(e):
    e.ret(0)


@imp("dlerror")
def _dlerror(e):
    e.ret(0)


@imp("dl_unwind_find_exidx")
def _dl_unwind_find_exidx(e):
    pc, pcount = e.args(2)
    # .ARM.exidx of our image
    e.w32(pcount, 0x4FDA8 // 8)
    e.ret(0xD9C4B0)


# ---------------------------------------------------------------------------------------- math (soft-float)
def _d1(e):
    return Args(e).d()


def _d2(e):
    a = Args(e)
    return a.d(), a.d()


def _f1(e):
    return u2f(e.args(1)[0])


def _f2(e):
    x, y = e.args(2)
    return u2f(x), u2f(y)


def _ret_d(e, v):
    e.ret64(d2u(v))


def _ret_f(e, v):
    e.ret(f2u(v))


def _safe(fn, *a):
    try:
        return fn(*a)
    except (ValueError, OverflowError, ZeroDivisionError):
        return float("nan")


for _name, _fn in {"sin": math.sin, "cos": math.cos, "tan": math.tan, "asin": math.asin, "acos": math.acos,
                   "atan": math.atan, "sinh": math.sinh, "cosh": math.cosh, "tanh": math.tanh,
                   "asinh": math.asinh, "acosh": math.acosh, "atanh": math.atanh, "exp": math.exp,
                   "exp2": lambda x: 2.0 ** x, "expm1": math.expm1, "log": math.log, "log10": math.log10,
                   "log1p": math.log1p, "logb": lambda x: float(math.frexp(x)[1] - 1), "sqrt": math.sqrt,
                   "cbrt": lambda x: math.copysign(abs(x) ** (1 / 3), x), "fabs": math.fabs,
                   "floor": math.floor, "rint": lambda x: float(round(x)), "nearbyint": lambda x: float(round(x)),
                   "erf": math.erf, "erfc": math.erfc, "lgamma": math.lgamma, "tgamma": math.gamma}.items():
    def _mk(fn):
        return lambda e: _ret_d(e, float(_safe(fn, _d1(e))))
    H[_name] = _mk(_fn)

for _name, _fn in {"sinf": math.sin, "cosf": math.cos, "tanf": math.tan, "asinf": math.asin,
                   "acosf": math.acos, "expf": math.exp, "exp2f": lambda x: 2.0 ** x, "logf": math.log,
                   "log10f": math.log10, "sqrtf": math.sqrt, "floorf": math.floor, "ceilf": math.ceil,
                   "roundf": lambda x: float(math.floor(abs(x) + 0.5)) * (1 if x >= 0 else -1)}.items():
    def _mkf(fn):
        return lambda e: _ret_f(e, float(_safe(fn, _f1(e))))
    H[_name] = _mkf(_fn)

H["atan2"] = lambda e: _ret_d(e, math.atan2(*_d2(e)))
H["pow"] = lambda e: _ret_d(e, float(_safe(math.pow, *_d2(e))))
H["fmod"] = lambda e: _ret_d(e, float(_safe(math.fmod, *_d2(e))))
H["hypot"] = lambda e: _ret_d(e, math.hypot(*_d2(e)))
H["remainder"] = lambda e: _ret_d(e, float(_safe(math.remainder, *_d2(e))))
H["nextafter"] = lambda e: _ret_d(e, math.nextafter(*_d2(e)) if hasattr(math, "nextafter") else _d2(e)[0])
H["atan2f"] = lambda e: _ret_f(e, math.atan2(*_f2(e)))
H["powf"] = lambda e: _ret_f(e, float(_safe(math.pow, *_f2(e))))
H["fmodf"] = lambda e: _ret_f(e, float(_safe(math.fmod, *_f2(e))))
H["fminf"] = lambda e: _ret_f(e, min(*_f2(e)))
H["nextafterf"] = lambda e: _ret_f(e, _f2(e)[0])
H["isnan"] = lambda e: e.ret(int(math.isnan(_d1(e))))
H["isnanf"] = lambda e: e.ret(int(math.isnan(_f1(e))))
H["__isfinite"] = lambda e: e.ret(int(math.isfinite(_d1(e))))
H["__signbit"] = lambda e: e.ret(int(math.copysign(1, _d1(e)) < 0))


@imp("__fpclassifyd")
def _fpclassifyd(e):
    x = _d1(e)
    # bionic: FP_INFINITE=1 FP_NAN=2 FP_NORMAL=4 FP_SUBNORMAL=8 FP_ZERO=16
    if math.isnan(x):
        e.ret(2)
    elif math.isinf(x):
        e.ret(1)
    elif x == 0:
        e.ret(16)
    elif abs(x) < 2.2250738585072014e-308:
        e.ret(8)
    else:
        e.ret(4)


@imp("modf")
def _modf(e):
    a = Args(e)
    x = a.d()
    ip = a.i()
    f, i = math.modf(x)
    e.write(ip, struct.pack("<d", i))
    _ret_d(e, f)


@imp("scalbn", "scalbnl")
def _scalbn(e):
    a = Args(e)
    x = a.d()
    n = a.s32()
    _ret_d(e, math.ldexp(x, n))


@imp("remquo")
def _remquo(e):
    a = Args(e)
    x, y = a.d(), a.d()
    qp = a.i()
    r = math.remainder(x, y)
    e.w32(qp, int(round((x - r) / y)) & 0xFFFFFFFF)
    _ret_d(e, r)


@imp("lrint")
def _lrint(e):
    e.ret(int(round(_d1(e))) & 0xFFFFFFFF)


@imp("llrint")
def _llrint(e):
    e.ret64(int(round(_d1(e))))


# ---------------------------------------------------------------------------------------- files
# device paths backed by files outside FS_ROOT
VIRTUAL_FILES = {f"/data/app/{PACKAGE}-1/lib/arm/libcocos2dcpp.so": paths.so()}


def host_path(path):
    """Android absolute path -> host path under FS_ROOT."""
    if path in VIRTUAL_FILES:
        return VIRTUAL_FILES[path]
    p = path.lstrip("/")
    return FS_ROOT / p


class HostFile:
    def __init__(self, f, path):
        self.f, self.path, self.eof, self.err = f, path, False, False


def _open_host(path, mode):
    hp = host_path(path)
    if any(c in mode for c in "wa"):
        hp.parent.mkdir(parents=True, exist_ok=True)
    m = mode.replace("t", "")
    if "b" not in m:
        m += "b"
    return open(hp, m)


@imp("fopen")
def _fopen(e):
    path, mode = e.cstr(e.args(2)[0]), e.cstr(e.args(2)[1])
    try:
        f = _open_host(path, mode)
    except OSError:
        e.log(f"  fopen({path}, {mode}) -> NULL")
        e.libc.set_errno(ENOENT)
        e.ret(0)
        return
    fp = e.alloc(84)
    e.libc.files[fp] = HostFile(f, path)
    e.log(f"  fopen({path}, {mode}) -> {fp:#x}")
    e.ret(fp)


@imp("fdopen")
def _fdopen(e):
    fd, mode = e.args(2)
    hf = e.libc.fds.get(fd)
    if hf is None:
        e.ret(0)
        return
    fp = e.alloc(84)
    e.libc.files[fp] = hf
    e.ret(fp)


def _file(e, fp):
    hf = e.libc.files.get(fp)
    if hf is None:
        raise GuestFault(f"unknown FILE* {fp:#x}")
    return hf


def _fwrite_host(e, fp, data):
    sf = e.data_imports.get("__sF")
    if sf is not None and sf <= fp < sf + 84 * 3:
        e.log(f"[std{'in out err'.split()[(fp - sf) // 84]}] " + data.decode("utf-8", "replace").rstrip("\n"))
        return len(data)
    return _file(e, fp).f.write(data)


@imp("fclose")
def _fclose(e):
    fp = e.args(1)[0]
    hf = e.libc.files.pop(fp, None)
    if hf:
        hf.f.close()
        e.heap.free(fp)
    e.ret(0)


@imp("fread")
def _fread(e):
    buf, size, n, fp = e.args(4)
    hf = _file(e, fp)
    data = hf.f.read(size * n)
    if len(data) < size * n:
        hf.eof = True
    e.write(buf, data)
    e.ret(len(data) // size if size else 0)


@imp("fwrite")
def _fwrite(e):
    buf, size, n, fp = e.args(4)
    _fwrite_host(e, fp, e.read(buf, size * n))
    e.ret(n)


@imp("fputs")
def _fputs(e):
    s, fp = e.args(2)
    _fwrite_host(e, fp, e.cbytes(s))
    e.ret(1)


@imp("fputc", "fputwc")
def _fputc(e):
    c, fp = e.args(2)
    _fwrite_host(e, fp, bytes([c & 0xFF]))
    e.ret(c)


@imp("fflush")
def _fflush(e):
    e.ret(0)


@imp("fgets")
def _fgets(e):
    buf, n, fp = e.args(3)
    line = _file(e, fp).f.readline(n - 1)
    if not line:
        e.ret(0)
        return
    e.write(buf, line + b"\0")
    e.ret(buf)


@imp("fseek", "fseeko")
def _fseek(e):
    fp, off, whence = e.args(3)
    off = off - (1 << 32) if off & 0x80000000 else off
    hf = _file(e, fp)
    hf.f.seek(off, whence)
    hf.eof = False
    e.ret(0)


@imp("ftell", "ftello")
def _ftell(e):
    e.ret(_file(e, e.args(1)[0]).f.tell())


@imp("setvbuf")
def _setvbuf(e):
    e.ret(0)


@imp("freopen")
def _freopen(e):
    e.ret(e.args(3)[2])


# POSIX fds
O_WRONLY, O_RDWR, O_CREAT, O_TRUNC, O_APPEND = 1, 2, 0x40, 0x200, 0x400


@imp("open")
def _open(e):
    path, flags = e.cstr(e.args(2)[0]), e.args(2)[1]
    mode = "rb"
    if flags & 3:
        mode = "ab" if flags & O_APPEND else ("wb" if flags & O_TRUNC or not host_path(path).exists() else "r+b")
    if flags & 3 == O_RDWR and mode == "rb":
        mode = "r+b"
    try:
        f = _open_host(path, mode)
    except OSError:
        e.log(f"  open({path}, {flags:#x}) -> -1")
        e.libc.set_errno(ENOENT)
        e.ret(0xFFFFFFFF)
        return
    fd = e.libc.next_fd
    e.libc.next_fd += 1
    e.libc.fds[fd] = HostFile(f, path)
    e.log(f"  open({path}, {flags:#x}) -> {fd}")
    e.ret(fd)


@imp("close")
def _close(e):
    hf = e.libc.fds.pop(e.args(1)[0], None)
    if hf is not None:
        (hf.sock if hasattr(hf, "sock") else hf.f).close()
    e.ret(0)


@imp("read")
def _read(e):
    fd, buf, n = e.args(3)
    hf = e.libc.fds.get(fd)
    if hf is None:
        e.ret(0xFFFFFFFF)
        return
    data = hf.f.read(n)
    e.write(buf, data)
    e.ret(len(data))


@imp("write")
def _write(e):
    fd, buf, n = e.args(3)
    data = e.read(buf, n)
    if fd in (1, 2):
        e.log(f"[fd{fd}] " + data.decode("utf-8", "replace").rstrip("\n"))
        e.ret(n)
        return
    hf = e.libc.fds.get(fd)
    if hf is None:
        e.ret(0xFFFFFFFF)
        return
    e.ret(hf.f.write(data))


@imp("lseek")
def _lseek(e):
    fd, off, whence = e.args(3)
    off = off - (1 << 32) if off & 0x80000000 else off
    hf = e.libc.fds.get(fd)
    e.ret(hf.f.seek(off, whence) if hf else 0xFFFFFFFF)


def _stat_pack(st):
    # bionic arm32 struct stat (kernel stat64 layout, 104 bytes)
    b = bytearray(104)
    struct.pack_into("<Q", b, 0, st.st_dev)
    struct.pack_into("<I", b, 12, st.st_ino & 0xFFFFFFFF)
    struct.pack_into("<II", b, 16, st.st_mode, st.st_nlink)
    struct.pack_into("<II", b, 24, 10217, 10217)
    struct.pack_into("<q", b, 48, st.st_size)
    struct.pack_into("<I", b, 56, 4096)
    struct.pack_into("<Q", b, 64, (st.st_size + 511) // 512)
    for k, t in enumerate((st.st_atime, st.st_mtime, st.st_ctime)):
        struct.pack_into("<II", b, 72 + 8 * k, int(t), 0)
    struct.pack_into("<Q", b, 96, st.st_ino)
    return bytes(b)


@imp("stat")
def _stat(e):
    path, buf = e.args(2)
    p = e.cstr(path)
    try:
        st = os.stat(host_path(p))
    except OSError:
        e.libc.set_errno(ENOENT)
        e.ret(0xFFFFFFFF)
        return
    e.write(buf, _stat_pack(st))
    e.ret(0)


@imp("fstat")
def _fstat(e):
    fd, buf = e.args(2)
    hf = e.libc.fds.get(fd)
    if hf is None:
        e.ret(0xFFFFFFFF)
        return
    e.write(buf, _stat_pack(os.fstat(hf.f.fileno())))
    e.ret(0)


@imp("access")
def _access(e):
    p = e.cstr(e.args(1)[0])
    ok = host_path(p).exists()
    e.log(f"  access({p}) -> {0 if ok else -1}")
    if not ok:
        e.libc.set_errno(ENOENT)
    e.ret(0 if ok else 0xFFFFFFFF)


@imp("mkdir")
def _mkdir(e):
    p = e.cstr(e.args(1)[0])
    try:
        host_path(p).mkdir(parents=False)
        e.ret(0)
    except FileExistsError:
        e.libc.set_errno(17)
        e.ret(0xFFFFFFFF)
    except OSError:
        e.libc.set_errno(ENOENT)
        e.ret(0xFFFFFFFF)


@imp("remove", "unlink")
def _remove(e):
    p = host_path(e.cstr(e.args(1)[0]))
    try:
        if p.is_dir():
            p.rmdir()
        else:
            p.unlink()
        e.ret(0)
    except OSError:
        e.ret(0xFFFFFFFF)


@imp("rename")
def _rename(e):
    a, b = e.args(2)
    src, dst = e.cstr(a), e.cstr(b)
    hs, hd = host_path(src), host_path(dst)
    try:
        os.replace(hs, hd)
        e.ret(0)
        return
    except PermissionError:
        pass
    except OSError as ex:
        e.log(f"  rename({src}, {dst}) -> -1 ({ex})")
        e.libc.set_errno(ENOENT)
        e.ret(0xFFFFFFFF)
        return
    # Windows refuses to replace/rename a file that is still open (here: AppState's s000.gif/s001.gif,
    # which made every save fail); POSIX allows it. Emulate: copy the contents over, then drop the source.
    try:
        import shutil
        shutil.copyfile(hs, hd)
        try:
            os.remove(hs)
        except OSError:
            pass
        e.ret(0)
    except OSError as ex:
        e.log(f"  rename({src}, {dst}) -> -1 ({ex})")
        e.libc.set_errno(13)
        e.ret(0xFFFFFFFF)


@imp("truncate")
def _truncate(e):
    p, n = e.args(2)
    e.log(f"  truncate({e.cstr(p)}, {n})")
    try:
        os.truncate(host_path(e.cstr(p)), n)
        e.ret(0)
    except OSError:
        e.ret(0xFFFFFFFF)


@imp("opendir")
def _opendir(e):
    p = e.cstr(e.args(1)[0])
    hp = host_path(p)
    if not hp.is_dir():
        e.ret(0)
        return
    d = e.alloc(16)
    e.libc.dirs[d] = [list(os.listdir(hp)), 0, e.alloc(280)]
    e.ret(d)


@imp("readdir")
def _readdir(e):
    d = e.args(1)[0]
    st = e.libc.dirs.get(d)
    if not st or st[1] >= len(st[0]):
        e.ret(0)
        return
    name = st[0][st[1]].encode()[:255]
    st[1] += 1
    # bionic dirent: u64 d_ino, s64 d_off, u16 d_reclen, u8 d_type, char d_name[256]
    e.write(st[2], struct.pack("<QqHB", 1, 0, 280, 8) + name + b"\0")
    e.ret(st[2])


@imp("closedir")
def _closedir(e):
    e.libc.dirs.pop(e.args(1)[0], None)
    e.ret(0)


@imp("basename")
def _basename(e):
    p = e.cstr(e.args(1)[0]) or ""
    e.ret(e.alloc_str(p.rstrip("/").split("/")[-1] or "/"))


@imp("mmap")
def _mmap(e):
    addr, length, prot, flags, fd, off = e.args(6)
    p = e.heap.malloc(length)
    if fd != 0xFFFFFFFF and fd in e.libc.fds:
        hf = e.libc.fds[fd]
        pos = hf.f.tell()
        hf.f.seek(off)
        e.write(p, hf.f.read(length))
        hf.f.seek(pos)
    else:
        e.write(p, b"\0" * length)
    e.ret(p)


@imp("munmap")
def _munmap(e):
    e.heap.free(e.args(2)[0])
    e.ret(0)


def scan_c(e, src, fmt, a):
    """sscanf: returns number of assigned conversions (or -1 on input failure before any)."""
    import re
    i = j = 0
    count = 0
    n, m = len(fmt), len(src)
    while j < n:
        c = fmt[j]
        if c.isspace():
            while i < m and src[i].isspace():
                i += 1
            j += 1
            continue
        if c != "%":
            if i >= m or src[i] != c:
                break
            i += 1
            j += 1
            continue
        j += 1
        if fmt[j] == "%":
            if i < m and src[i] == "%":
                i += 1
                j += 1
                continue
            break
        skip = fmt[j] == "*"
        if skip:
            j += 1
        width = ""
        while fmt[j].isdigit():
            width += fmt[j]
            j += 1
        width = int(width) if width else None
        length = ""
        while fmt[j] in "hlLqjzt":
            length += fmt[j]
            j += 1
        conv = fmt[j]
        j += 1
        if conv == "[":
            k = j + (1 if fmt[j:j + 1] == "^" else 0)
            k += 1 if fmt[k:k + 1] == "]" else 0
            end = fmt.index("]", k)
            spec = fmt[j:end]
            j = end + 1
            neg = spec.startswith("^")
            chars = spec[1:] if neg else spec
            start = i
            while i < m and (width is None or i - start < width) and ((src[i] in chars) != neg):
                i += 1
            if i == start:
                break
            if not skip:
                e.write(a.i(), src[start:i].encode() + b"\0")
                count += 1
            continue
        if conv == "n":
            if not skip:
                e.w32(a.i(), i)
            continue
        if conv == "c":
            w = width or 1
            if i + w > m:
                break
            if not skip:
                e.write(a.i(), src[i:i + w].encode())
                count += 1
            i += w
            continue
        while i < m and src[i].isspace():
            i += 1
        if i >= m:
            break
        limit = m if width is None else min(m, i + width)
        if conv == "s":
            start = i
            while i < limit and not src[i].isspace():
                i += 1
            if not skip:
                e.write(a.i(), src[start:i].encode() + b"\0")
                count += 1
            continue
        if conv in "diuxXop":
            base = {"d": 10, "u": 10, "x": 16, "X": 16, "p": 16, "o": 8, "i": 0}[conv]
            pat = {10: r"[+-]?\d+", 16: r"[+-]?(0[xX])?[0-9a-fA-F]+", 8: r"[+-]?[0-7]+",
                   0: r"[+-]?(0[xX][0-9a-fA-F]+|0[0-7]*|[1-9]\d*)"}[base]
            mt = re.match(pat, src[i:limit])
            if not mt:
                break
            v = int(mt.group(0), base)
            i += mt.end()
            if not skip:
                p = a.i()
                if length in ("ll", "q", "j"):
                    e.write(p, struct.pack("<Q", v & 0xFFFFFFFFFFFFFFFF))
                elif length == "h":
                    e.write(p, struct.pack("<H", v & 0xFFFF))
                elif length == "hh":
                    e.write(p, struct.pack("<B", v & 0xFF))
                else:
                    e.w32(p, v)
                count += 1
            continue
        if conv in "fFeEgGaA":
            mt = re.match(r"[+-]?(\d+\.?\d*([eE][+-]?\d+)?|\.\d+([eE][+-]?\d+)?|inf|nan)", src[i:limit], re.I)
            if not mt:
                break
            v = float(mt.group(0))
            i += mt.end()
            if not skip:
                p = a.i()
                e.write(p, struct.pack("<d", v) if length in ("l", "L") else struct.pack("<f", v))
                count += 1
            continue
        raise GuestFault(f"sscanf: unsupported conversion %{conv} in {fmt!r}")
    if count == 0 and i >= m and m == 0:
        return 0xFFFFFFFF
    return count


@imp("sscanf")
def _sscanf(e):
    src, fmt = e.args(2)
    e.ret(scan_c(e, e.cbytes(src).decode("latin1"), e.cstr(fmt), Args(e, 2)))


@imp("vsscanf")
def _vsscanf(e):
    src, fmt, va = e.args(3)
    e.ret(scan_c(e, e.cbytes(src).decode("latin1"), e.cstr(fmt), Args(e, va_ptr=va)))
