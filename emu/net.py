"""BSD sockets / DNS for the guest over Python sockets.

Every hostname resolves to REDIRECT_IP, where the private server (server/run.py, port 80) listens.
Host sockets are always non-blocking; a guest *blocking* call that cannot complete raises Block so the
calling guest thread yields (see threads.py).
"""
import select
import socket
import struct
import time

from .libc import HostFile, imp
from .threads import Block

REDIRECT_IP = "127.0.0.1"
AF_INET, AF_INET6, AF_UNSPEC = 2, 10, 0
SOCK_STREAM, SOCK_DGRAM = 1, 2
EAGAIN, EINPROGRESS, ECONNREFUSED, EINTR, ENOTCONN = 11, 115, 111, 4, 107
O_NONBLOCK = 0x800
F_GETFL, F_SETFL, F_GETFD, F_SETFD = 3, 4, 1, 2
FIONBIO, FIONREAD = 0x5421, 0x541B
SOL_SOCKET, SO_ERROR = 1, 4
POLLIN, POLLPRI, POLLOUT, POLLERR, POLLHUP, POLLNVAL = 1, 2, 4, 8, 0x10, 0x20


class GuestSocket:
    def __init__(self, family, stype):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM if stype & 0xF == SOCK_STREAM
                                  else socket.SOCK_DGRAM)
        self.sock.setblocking(False)
        self.nonblocking = bool(stype & O_NONBLOCK)
        self.peer = None
        self.connecting = False
        self.error = 0


def _sock(e, fd):
    obj = e.libc.fds.get(fd)
    return obj if isinstance(obj, GuestSocket) else None


def _fail(e, err):
    e.libc.set_errno(err)
    e.ret(0xFFFFFFFF)


def _sockaddr_in(ip, port):
    return struct.pack("<H", AF_INET) + struct.pack(">H", port) + socket.inet_aton(ip) + bytes(8)


def _parse_sockaddr(e, p):
    fam, = struct.unpack("<H", e.read(p, 2))
    port, = struct.unpack(">H", e.read(p + 2, 2))
    ip = socket.inet_ntoa(e.read(p + 4, 4))
    return fam, ip, port


# ---------------------------------------------------------------------------------------- DNS
@imp("getaddrinfo")
def _getaddrinfo(e):
    node, service, hints, res = e.args(4)
    host = e.cstr(node) if node else "localhost"
    port = 0
    if service:
        svc = e.cstr(service)
        port = int(svc) if svc.isdigit() else {"http": 80, "https": 443}.get(svc, 0)
    socktype, proto = SOCK_STREAM, 6
    if hints:
        _, _, ht, hp = struct.unpack("<4i", e.read(hints, 16))
        socktype, proto = ht or SOCK_STREAM, hp or (6 if (ht or 1) == SOCK_STREAM else 17)
    e.log(f"  getaddrinfo({host}, {port}) -> {REDIRECT_IP}")
    addr = e.alloc(16, _sockaddr_in(REDIRECT_IP, port))
    canon = e.alloc_str(host)
    # bionic 32-bit struct addrinfo: flags, family, socktype, protocol, addrlen, canonname, addr, next
    ai = e.alloc(32, struct.pack("<5i3I", 0, AF_INET, socktype, proto, 16, canon, addr, 0))
    e.w32(res, ai)
    e.ret(0)


@imp("freeaddrinfo")
def _freeaddrinfo(e):
    e.ret(0)


@imp("gai_strerror")
def _gai_strerror(e):
    e.ret(e.alloc_str(f"getaddrinfo error {e.args(1)[0]}"))


@imp("gethostbyname")
def _gethostbyname(e):
    name = e.cstr(e.args(1)[0])
    e.log(f"  gethostbyname({name}) -> {REDIRECT_IP}")
    ip = e.alloc(4, socket.inet_aton(REDIRECT_IP))
    addr_list = e.alloc(8, struct.pack("<II", ip, 0))
    aliases = e.alloc(4)
    # struct hostent: h_name, h_aliases, h_addrtype, h_length, h_addr_list
    e.ret(e.alloc(20, struct.pack("<IIiiI", e.alloc_str(name), aliases, AF_INET, 4, addr_list)))


@imp("gethostbyaddr")
def _gethostbyaddr(e):
    e.ret(0)


@imp("gethostname")
def _gethostname(e):
    buf, n = e.args(2)
    e.write(buf, b"localhost\0"[:n])
    e.ret(0)


@imp("getnameinfo")
def _getnameinfo(e):
    e.ret(-2 & 0xFFFFFFFF)  # EAI_NONAME


@imp("inet_pton")
def _inet_pton(e):
    fam, src, dst = e.args(3)
    try:
        if fam == AF_INET:
            e.write(dst, socket.inet_aton(e.cstr(src)))
            e.ret(1)
        else:
            e.write(dst, socket.inet_pton(socket.AF_INET6, e.cstr(src)))
            e.ret(1)
    except OSError:
        e.ret(0)


@imp("inet_ntop")
def _inet_ntop(e):
    fam, src, dst, size = e.args(4)
    s = socket.inet_ntoa(e.read(src, 4)) if fam == AF_INET else socket.inet_ntop(socket.AF_INET6, e.read(src, 16))
    e.write(dst, s.encode()[:size - 1] + b"\0")
    e.ret(dst)


@imp("if_nametoindex")
def _if_nametoindex(e):
    e.ret(0)


# ---------------------------------------------------------------------------------------- sockets
@imp("socket")
def _socket(e):
    fam, stype, proto = e.args(3)
    if fam not in (AF_INET, AF_INET6):
        _fail(e, 97)  # EAFNOSUPPORT
        return
    gs = GuestSocket(fam, stype)
    fd = e.libc.next_fd
    e.libc.next_fd += 1
    e.libc.fds[fd] = gs
    e.log(f"  socket() -> fd {fd}")
    e.ret(fd)


@imp("connect")
def _connect(e):
    fd, addr, alen = e.args(3)
    gs = _sock(e, fd)
    fam, ip, port = _parse_sockaddr(e, addr)
    ip = REDIRECT_IP
    rc = gs.sock.connect_ex((ip, port))
    gs.peer = (ip, port)
    e.log(f"  connect(fd {fd}, {ip}:{port}) -> {rc}")
    if rc == 0:
        e.ret(0)
    elif rc in (10035, 115, 36):   # WSAEWOULDBLOCK / EINPROGRESS
        gs.connecting = True
        if gs.nonblocking:
            _fail(e, EINPROGRESS)
        else:
            s = gs.sock
            raise Block(lambda: bool(select.select([], [s], [s], 0)[1]), f"connect fd {fd}")
    else:
        gs.error = ECONNREFUSED
        _fail(e, ECONNREFUSED)


def _readable(gs):
    try:
        return bool(select.select([gs.sock], [], [], 0)[0])
    except OSError:
        return True


def _writable(gs):
    try:
        r = select.select([], [gs.sock], [gs.sock], 0)
        return bool(r[1] or r[2])
    except OSError:
        return True


@imp("send", "sendto")
def _send(e):
    fd, buf, n, flags = e.args(4)
    gs = _sock(e, fd)
    data = e.read(buf, n)
    try:
        sent = gs.sock.send(data)
        e.ret(sent)
    except BlockingIOError:
        if gs.nonblocking:
            _fail(e, EAGAIN)
        else:
            raise Block(lambda: _writable(gs), f"send fd {fd}")
    except OSError as ex:
        e.log(f"  send(fd {fd}) error {ex}")
        _fail(e, 104)  # ECONNRESET


@imp("recv", "recvfrom")
def _recv(e):
    fd, buf, n, flags = e.args(4)
    gs = _sock(e, fd)
    try:
        data = gs.sock.recv(n)
        e.write(buf, data)
        e.ret(len(data))
    except BlockingIOError:
        if gs.nonblocking:
            _fail(e, EAGAIN)
        else:
            raise Block(lambda: _readable(gs), f"recv fd {fd}")
    except OSError as ex:
        e.log(f"  recv(fd {fd}) error {ex}")
        _fail(e, 104)


@imp("shutdown")
def _shutdown(e):
    fd, how = e.args(2)
    gs = _sock(e, fd)
    try:
        gs.sock.shutdown(how)
    except OSError:
        pass
    e.ret(0)


@imp("setsockopt")
def _setsockopt(e):
    e.ret(0)


@imp("getsockopt")
def _getsockopt(e):
    fd, level, opt, val, plen = e.args(5)
    gs = _sock(e, fd)
    v = 0
    if gs and level == SOL_SOCKET and opt == SO_ERROR:
        v = gs.sock.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR)
        v = ECONNREFUSED if v else 0
    e.w32(val, v)
    e.w32(plen, 4)
    e.ret(0)


@imp("getsockname", "getpeername")
def _getsockname(e):
    fd, addr, plen = e.args(3)
    gs = _sock(e, fd)
    ip, port = gs.peer if gs and gs.peer else ("127.0.0.1", 0)
    e.write(addr, _sockaddr_in(ip, port))
    e.w32(plen, 16)
    e.ret(0)


@imp("bind", "listen", "accept")
def _server_side(e):
    _fail(e, 95)  # EOPNOTSUPP


@imp("fcntl")
def _fcntl(e):
    fd, cmd, arg = e.args(3)
    gs = _sock(e, fd)
    if cmd == F_GETFL:
        e.ret(O_NONBLOCK if gs and gs.nonblocking else 2)
    elif cmd == F_SETFL:
        if gs:
            gs.nonblocking = bool(arg & O_NONBLOCK)
        e.ret(0)
    else:
        e.ret(0)


@imp("ioctl")
def _ioctl(e):
    fd, req, arg = e.args(3)
    gs = _sock(e, fd)
    if req == FIONBIO and gs:
        gs.nonblocking = bool(e.u32(arg))
    elif req == FIONREAD:
        e.w32(arg, 0)
    e.ret(0)


# ---------------------------------------------------------------------------------------- readiness
def _ready_sets(e, fds_r, fds_w):
    socks_r = {fd: _sock(e, fd) for fd in fds_r}
    socks_w = {fd: _sock(e, fd) for fd in fds_w}
    rr = [fd for fd, gs in socks_r.items() if gs is None or _readable(gs)]
    ww = [fd for fd, gs in socks_w.items() if gs is None or _writable(gs)]
    return rr, ww


def _timeout_ready(e, key, timeout_s):
    """Resumable timeout: True once `timeout_s` passed since the first attempt of this call."""
    t = e.sched.current
    if t.wait is None or t.wait[0] != key:
        t.wait = (key, time.time() + timeout_s)
    return t.wait[1]


@imp("select")
def _select(e):
    nfds, pr, pw, px, ptv = e.args(5)

    def bits(p):
        if not p:
            return []
        raw = int.from_bytes(e.read(p, 128), "little")
        return [fd for fd in range(nfds) if raw >> fd & 1]
    fr, fw = bits(pr), bits(pw)
    timeout = None
    if ptv:
        sec, usec = struct.unpack("<ii", e.read(ptv, 8))
        timeout = sec + usec / 1e6
    rr, ww = _ready_sets(e, fr, fw)
    if not rr and not ww and timeout != 0:
        deadline = _timeout_ready(e, ("select", pr, pw), timeout if timeout is not None else 1e9)
        if time.time() < deadline:   # once the deadline passed, fall through and report the timeout (0)
            raise Block(lambda: any(_ready_sets(e, fr, fw)) or time.time() >= deadline, "select")
    e.sched.current.wait = None

    def put(p, fds):
        if p:
            v = 0
            for fd in fds:
                v |= 1 << fd
            e.write(p, v.to_bytes(128, "little"))
    put(pr, rr)
    put(pw, ww)
    if px:
        e.write(px, bytes(128))
    e.ret(len(rr) + len(ww))


@imp("poll")
def _poll(e):
    pfds, n, timeout_ms = e.args(3)
    timeout_ms = timeout_ms - (1 << 32) if timeout_ms & 0x80000000 else timeout_ms
    entries = [struct.unpack("<ihh", e.read(pfds + 8 * k, 8)) for k in range(n)]

    def compute():
        out = []
        for fd, events, _ in entries:
            gs = _sock(e, fd)
            rev = 0
            if fd < 0:            # POSIX: negative fds are ignored (revents 0)
                pass
            elif gs is None:
                rev = POLLNVAL if fd not in e.libc.fds else (events & (POLLIN | POLLOUT))
            else:
                if events & (POLLIN | POLLPRI) and _readable(gs):
                    rev |= POLLIN
                if events & POLLOUT and _writable(gs):
                    rev |= POLLOUT
            out.append(rev)
        return out
    revs = compute()
    if not any(revs) and timeout_ms != 0:
        deadline = _timeout_ready(e, ("poll", pfds), timeout_ms / 1000 if timeout_ms > 0 else 1e9)
        if time.time() < deadline:   # once the deadline passed, fall through and report the timeout (0)
            raise Block(lambda: any(compute()) or time.time() >= deadline, "poll")
    e.sched.current.wait = None
    for k, (fd, events, _) in enumerate(entries):
        e.write(pfds + 8 * k, struct.pack("<ihh", fd, events, revs[k]))
    e.ret(sum(1 for r in revs if r))
