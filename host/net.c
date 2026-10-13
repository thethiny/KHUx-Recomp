/* BSD sockets and DNS over Winsock (as emu/net.py). Every host name resolves to server_ip, where the private
 * server listens. Guest threads are host threads, so a blocking guest call simply blocks. */
#include <winsock2.h>
#include <ws2tcpip.h>
#include "host.h"
#include <ctype.h>

char server_ip[64] = "127.0.0.1";

#define AF_INET_G 2
#define AF_INET6_G 10
#define SOCK_STREAM_G 1
#define O_NONBLOCK_G 0x800
#define EAGAIN_G 11
#define EINPROGRESS_G 115
#define ECONNREFUSED_G 111
#define ECONNRESET_G 104
#define ENOTCONN_G 107
#define EAFNOSUPPORT_G 97
#define EOPNOTSUPP_G 95
#define EBADF_G 9

int fd_new_socket(SOCKET s, int nonblock);
SOCKET fd_socket(uint32_t fd, int **nonblock);
int fd_is_socket(uint32_t fd);

static int map_wsa(int e) {
    switch (e) {
    case WSAEWOULDBLOCK: return EAGAIN_G;
    case WSAEINPROGRESS: return EINPROGRESS_G;
    case WSAECONNREFUSED: return ECONNREFUSED_G;
    case WSAENOTCONN: return ENOTCONN_G;
    default: return ECONNRESET_G;
    }
}

static void fail(CPU *c, int e) { set_errno(e); RET(c, 0xFFFFFFFFu); }

static void sockaddr_put(uint32_t p, uint32_t ip_be, uint16_t port) {
    uint8_t b[16] = {0};
    b[0] = AF_INET_G;
    b[2] = (uint8_t)(port >> 8);
    b[3] = (uint8_t)port;
    memcpy(b + 4, &ip_be, 4);
    memcpy(GP(p), b, 16);
}

static uint32_t server_addr(void) {
    struct in_addr a;
    inet_pton(AF_INET, server_ip, &a);
    return a.s_addr;
}

/* ---------------------------------------------------------------- DNS */
static void i_getaddrinfo(CPU *c) {
    uint32_t node = c->r[0], service = c->r[1], hints = c->r[2], res = c->r[3];
    const char *host = node ? gstr(node) : "localhost";
    uint16_t port = 0;
    if (service) {
        const char *s = gstr(service);
        port = (uint16_t)(isdigit((unsigned char)s[0]) ? atoi(s) : !strcmp(s, "http") ? 80 : !strcmp(s, "https") ? 443 : 0);
    }
    int32_t socktype = SOCK_STREAM_G, proto = 6;
    if (hints) {
        int32_t ht = (int32_t)g32(hints + 8), hp = (int32_t)g32(hints + 12);
        socktype = ht ? ht : SOCK_STREAM_G;
        proto = hp ? hp : (socktype == SOCK_STREAM_G ? 6 : 17);
    }
    hlog("  getaddrinfo(%s, %u) -> %s", host, port, server_ip);
    uint32_t addr = gzalloc(16), canon = galloc_str(host), ai = gzalloc(32);
    sockaddr_put(addr, server_addr(), port);
    /* bionic 32-bit struct addrinfo: flags, family, socktype, protocol, addrlen, canonname, addr, next */
    uint32_t v[8] = {0, AF_INET_G, (uint32_t)socktype, (uint32_t)proto, 16, canon, addr, 0};
    memcpy(GP(ai), v, sizeof(v));
    gw32(res, ai);
    RET(c, 0);
}

static void i_ok(CPU *c) { RET(c, 0); }
static void i_gai_strerror(CPU *c) {
    char b[48];
    snprintf(b, sizeof(b), "getaddrinfo error %d", (int)c->r[0]);
    RET(c, galloc_str(b));
}
static void i_gethostbyname(CPU *c) {
    hlog("  gethostbyname(%s) -> %s", gstr(c->r[0]), server_ip);
    uint32_t ip = server_addr();
    uint32_t ipp = galloc_bytes(&ip, 4), list = gzalloc(8), aliases = gzalloc(4), he = gzalloc(20);
    gw32(list, ipp);
    uint32_t v[5] = {galloc_str(gstr(c->r[0])), aliases, AF_INET_G, 4, list};
    memcpy(GP(he), v, sizeof(v));
    RET(c, he);
}
static void i_gethostname(CPU *c) {
    const char h[] = "localhost";
    uint32_t n = c->r[1] < sizeof(h) ? c->r[1] : (uint32_t)sizeof(h);
    memcpy(GP(c->r[0]), h, n);
    RET(c, 0);
}
static void i_getnameinfo(CPU *c) { RET(c, (uint32_t)-2); }
static void i_inet_pton(CPU *c) {
    int ok = inet_pton(c->r[0] == AF_INET_G ? AF_INET : AF_INET6, gstr(c->r[1]), GP(c->r[2]));
    RET(c, ok == 1 ? 1 : 0);
}
static void i_inet_ntop(CPU *c) {
    char b[64];
    if (!inet_ntop(c->r[0] == AF_INET_G ? AF_INET : AF_INET6, GP(c->r[1]), b, sizeof(b))) { RET(c, 0); return; }
    size_t n = strlen(b), cap = c->r[3];
    if (n + 1 > cap) n = cap ? cap - 1 : 0;
    memcpy(GP(c->r[2]), b, n);
    *(char *)GP(c->r[2] + (uint32_t)n) = 0;
    RET(c, c->r[2]);
}

/* ---------------------------------------------------------------- sockets */
static void set_nonblock(SOCKET s, int on) {
    u_long v = (u_long)on;
    ioctlsocket(s, FIONBIO, &v);
}

static void i_socket(CPU *c) {
    uint32_t fam = c->r[0], type = c->r[1];
    if (fam != AF_INET_G && fam != AF_INET6_G) { fail(c, EAFNOSUPPORT_G); return; }
    SOCKET s = socket(AF_INET, (type & 0xF) == SOCK_STREAM_G ? SOCK_STREAM : SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET) { fail(c, map_wsa(WSAGetLastError())); return; }
    int nb = (type & O_NONBLOCK_G) != 0;
    set_nonblock(s, nb);
    int fd = fd_new_socket(s, nb);
    hlog("  socket() -> fd %d", fd);
    RET(c, (uint32_t)fd);
}

static void i_connect(CPU *c) {
    int *nb;
    SOCKET s = fd_socket(c->r[0], &nb);
    if (s == INVALID_SOCKET) { fail(c, EBADF_G); return; }
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = *(uint16_t *)GP(c->r[1] + 2);
    sa.sin_addr.s_addr = server_addr();
    int rc = connect(s, (struct sockaddr *)&sa, sizeof(sa));
    int err = rc ? WSAGetLastError() : 0;
    hlog("  connect(fd %u, %s:%u) -> %d", c->r[0], server_ip, ntohs(sa.sin_port), err);
    if (!rc) { RET(c, 0); return; }
    if (err == WSAEWOULDBLOCK) { fail(c, EINPROGRESS_G); return; }
    fail(c, map_wsa(err));
}

int net_send(CPU *c, uint32_t fd, uint32_t buf, uint32_t n) {
    SOCKET s = fd_socket(fd, NULL);
    if (s == INVALID_SOCKET) { set_errno(EBADF_G); return -1; }
    int r = send(s, GP(buf), (int)n, 0);
    if (r == SOCKET_ERROR) { set_errno(map_wsa(WSAGetLastError())); return -1; }
    return r;
}

int net_recv(CPU *c, uint32_t fd, uint32_t buf, uint32_t n) {
    SOCKET s = fd_socket(fd, NULL);
    if (s == INVALID_SOCKET) { set_errno(EBADF_G); return -1; }
    int r = recv(s, GP(buf), (int)n, 0);
    if (r == SOCKET_ERROR) { set_errno(map_wsa(WSAGetLastError())); return -1; }
    return r;
}

static void i_send(CPU *c) { RET(c, (uint32_t)net_send(c, c->r[0], c->r[1], c->r[2])); }
static void i_recv(CPU *c) { RET(c, (uint32_t)net_recv(c, c->r[0], c->r[1], c->r[2])); }

static void i_shutdown(CPU *c) {
    SOCKET s = fd_socket(c->r[0], NULL);
    if (s != INVALID_SOCKET) shutdown(s, (int)c->r[1]);
    RET(c, 0);
}

#define SOL_SOCKET_G 1
#define SO_ERROR_G 4
static void i_getsockopt(CPU *c) {
    SOCKET s = fd_socket(c->r[0], NULL);
    uint32_t v = 0;
    if (s != INVALID_SOCKET && c->r[1] == SOL_SOCKET_G && c->r[2] == SO_ERROR_G) {
        int e = 0, len = sizeof(e);
        getsockopt(s, SOL_SOCKET, SO_ERROR, (char *)&e, &len);
        v = e ? ECONNREFUSED_G : 0;
    }
    gw32(c->r[3], v);
    gw32(ARG(c, 4), 4);
    RET(c, 0);
}

static void i_getsockname(CPU *c) {
    SOCKET s = fd_socket(c->r[0], NULL);
    struct sockaddr_in sa = {0};
    int len = sizeof(sa);
    if (s != INVALID_SOCKET) getpeername(s, (struct sockaddr *)&sa, &len);
    sockaddr_put(c->r[1], sa.sin_addr.s_addr ? sa.sin_addr.s_addr : server_addr(), ntohs(sa.sin_port));
    gw32(c->r[2], 16);
    RET(c, 0);
}

static void i_server_side(CPU *c) { fail(c, EOPNOTSUPP_G); }

#define F_GETFL 3
#define F_SETFL 4
static void i_fcntl(CPU *c) {
    int *nb;
    SOCKET s = fd_socket(c->r[0], &nb);
    if (c->r[1] == F_GETFL) { RET(c, s != INVALID_SOCKET && *nb ? O_NONBLOCK_G : 2); return; }
    if (c->r[1] == F_SETFL && s != INVALID_SOCKET) {
        *nb = (c->r[2] & O_NONBLOCK_G) != 0;
        set_nonblock(s, *nb);
    }
    RET(c, 0);
}

#define FIONBIO_G 0x5421
#define FIONREAD_G 0x541B
static void i_ioctl(CPU *c) {
    int *nb;
    SOCKET s = fd_socket(c->r[0], &nb);
    if (c->r[1] == FIONBIO_G && s != INVALID_SOCKET) {
        *nb = g32(c->r[2]) != 0;
        set_nonblock(s, *nb);
    } else if (c->r[1] == FIONREAD_G) {
        u_long n = 0;
        if (s != INVALID_SOCKET) ioctlsocket(s, FIONREAD, &n);
        gw32(c->r[2], (uint32_t)n);
    }
    RET(c, 0);
}

/* ---------------------------------------------------------------- readiness */
static void i_select(CPU *c) {
    uint32_t nfds = c->r[0], pr = c->r[1], pw = c->r[2], px = c->r[3], ptv = ARG(c, 4);
    fd_set r, w, x;
    FD_ZERO(&r); FD_ZERO(&w); FD_ZERO(&x);
    int any_sock = 0, others_r = 0, others_w = 0;
    uint32_t bits_r[32] = {0}, bits_w[32] = {0}, out_r[32] = {0}, out_w[32] = {0};
    if (pr) memcpy(bits_r, GP(pr), 128);
    if (pw) memcpy(bits_w, GP(pw), 128);
    for (uint32_t fd = 0; fd < nfds && fd < 1024; fd++) {
        int in_r = (bits_r[fd / 32] >> (fd % 32)) & 1, in_w = (bits_w[fd / 32] >> (fd % 32)) & 1;
        if (!in_r && !in_w) continue;
        SOCKET s = fd_socket(fd, NULL);
        if (s == INVALID_SOCKET) {                 /* files are always ready */
            if (in_r) { out_r[fd / 32] |= 1u << (fd % 32); others_r++; }
            if (in_w) { out_w[fd / 32] |= 1u << (fd % 32); others_w++; }
            continue;
        }
        if (in_r) FD_SET(s, &r);
        if (in_w) { FD_SET(s, &w); FD_SET(s, &x); }
        any_sock = 1;
    }
    int n = others_r + others_w;
    if (any_sock) {
        struct timeval tv, *ptv_h = NULL;
        if (n) { tv.tv_sec = 0; tv.tv_usec = 0; ptv_h = &tv; }
        else if (ptv) { tv.tv_sec = (long)g32(ptv); tv.tv_usec = (long)g32(ptv + 4); ptv_h = &tv; }
        if (select(0, &r, &w, &x, ptv_h) == SOCKET_ERROR) { fail(c, map_wsa(WSAGetLastError())); return; }
        for (uint32_t fd = 0; fd < nfds && fd < 1024; fd++) {
            SOCKET s = fd_socket(fd, NULL);
            if (s == INVALID_SOCKET) continue;
            if (FD_ISSET(s, &r)) { out_r[fd / 32] |= 1u << (fd % 32); n++; }
            if (FD_ISSET(s, &w) || FD_ISSET(s, &x)) { out_w[fd / 32] |= 1u << (fd % 32); n++; }
        }
    } else if (!n && ptv) {
        Sleep(g32(ptv) * 1000 + g32(ptv + 4) / 1000);
    }
    if (pr) memcpy(GP(pr), out_r, 128);
    if (pw) memcpy(GP(pw), out_w, 128);
    if (px) memset(GP(px), 0, 128);
    RET(c, (uint32_t)n);
}

#define POLLIN_G 1
#define POLLPRI_G 2
#define POLLOUT_G 4
#define POLLERR_G 8
#define POLLHUP_G 0x10
#define POLLNVAL_G 0x20
static void i_poll(CPU *c) {
    uint32_t pfds = c->r[0], n = c->r[1];
    int32_t timeout = (int32_t)c->r[2];
    WSAPOLLFD h[64];
    int map[64], nh = 0, ready = 0;
    if (n > 64) fatal("poll with %u descriptors", n);
    for (uint32_t k = 0; k < n; k++) {
        int32_t fd = (int32_t)g32(pfds + 8 * k);
        uint16_t ev = g16(pfds + 8 * k + 4);
        uint16_t rev = 0;
        map[k] = -1;
        if (fd >= 0) {
            SOCKET s = fd_socket((uint32_t)fd, NULL);
            if (s != INVALID_SOCKET) {
                h[nh].fd = s;
                h[nh].events = (short)(((ev & (POLLIN_G | POLLPRI_G)) ? POLLRDNORM : 0) | ((ev & POLLOUT_G) ? POLLWRNORM : 0));
                h[nh].revents = 0;
                map[k] = nh++;
            } else {
                rev = (uint16_t)(ev & (POLLIN_G | POLLOUT_G));      /* a file: ready */
                if (rev) ready++;
            }
        }
        gw16(pfds + 8 * k + 6, rev);
    }
    if (nh) {
        int r = WSAPoll(h, (ULONG)nh, ready ? 0 : timeout);
        if (r == SOCKET_ERROR) { fail(c, map_wsa(WSAGetLastError())); return; }
        for (uint32_t k = 0; k < n; k++) {
            if (map[k] < 0) continue;
            short re = h[map[k]].revents;
            uint16_t rev = 0;
            if (re & (POLLRDNORM | POLLHUP)) rev |= POLLIN_G;
            if (re & POLLWRNORM) rev |= POLLOUT_G;
            if (re & POLLERR) rev |= POLLERR_G;
            if (re & POLLHUP) rev |= POLLHUP_G;
            if (re & POLLNVAL) rev |= POLLNVAL_G;
            gw16(pfds + 8 * k + 6, rev);
            if (rev) ready++;
        }
    } else if (!ready && timeout > 0) {
        Sleep((DWORD)timeout);
    }
    RET(c, (uint32_t)ready);
}

void net_init(void) {
    WSADATA wd;
    if (WSAStartup(MAKEWORD(2, 2), &wd)) fatal("WSAStartup failed");
    imp("getaddrinfo", i_getaddrinfo);
    imp("freeaddrinfo", i_ok);
    imp("gai_strerror", i_gai_strerror);
    imp("gethostbyname", i_gethostbyname);
    imp("gethostbyaddr", i_ok);
    imp("gethostname", i_gethostname);
    imp("getnameinfo", i_getnameinfo);
    imp("inet_pton", i_inet_pton);
    imp("inet_ntop", i_inet_ntop);
    imp("if_nametoindex", i_ok);
    imp("socket", i_socket);
    imp("connect", i_connect);
    imp("send", i_send);
    imp("sendto", i_send);
    imp("recv", i_recv);
    imp("recvfrom", i_recv);
    imp("shutdown", i_shutdown);
    imp("setsockopt", i_ok);
    imp("getsockopt", i_getsockopt);
    imp("getsockname", i_getsockname);
    imp("getpeername", i_getsockname);
    imp("bind", i_server_side);
    imp("listen", i_server_side);
    imp("accept", i_server_side);
    imp("fcntl", i_fcntl);
    imp("ioctl", i_ioctl);
    imp("select", i_select);
    imp("poll", i_poll);
}
