// Library free functions: base64 wrappers, xxHash32, MD5 block function, OpenSSL GOST method table / CONF
// cleanup, operator new, Tremor (integer Vorbis) codebook setup.
#include "B2Common.hpp"

using namespace b2;

namespace freelib {

static uint32_t rotl(uint32_t x, int r) { return (x << r) | (x >> (32 - r)); }
// ARM register shifts: the amount is the low byte; 32 or more shifts everything out
static uint32_t lslReg(uint32_t x, uint32_t amount) {
    amount &= 0xFF;
    return amount >= 32 ? 0 : x << amount;
}
static int32_t asrReg(int32_t x, uint32_t amount) {
    amount &= 0xFF;
    return amount >= 32 ? (x >> 31) : (x >> amount);
}

struct Lib {
    // ------------------------------------------------------------------------------------------ base64
    static uint32_t base64Decode(uint32_t in, uint32_t len, uint32_t out) {
        guest::Frame f(0x14);
        *f.at<uint32_t>(0xC) = 0;
        float size = float(len);
        size = size * 3.0f;
        size = size * 0.25f;
        size = size + 1.0f;
        uint32_t p = rw::call(0x40FF58, {f2u(size)});  // malloc
        st32(out, p);
        if (p == 0) return 0;
        if (int32_t(rw::call(rwlib::cocos2d::_base64Decode, {in, len, p, f.addr(0xC)})) > 0) {  // cocos2d::_base64Decode
            rw::call(0x40FF64, {ld32(out)});  // free
            st32(out, 0);
            *f.at<uint32_t>(0xC) = 0;
            return 0;
        }
        return *f.at<uint32_t>(0xC);
    }
    static uint32_t base64Encode(uint32_t in, uint32_t len, uint32_t out) {
        uint32_t q = rw::call(0xCD5DF9, {len << 2, 3});  // __udivsi3
        uint32_t r = rw::call(0xCD5F3D, {len, 3});       // __umodsi3
        uint32_t outLen = r == 0 ? q : q + 4;
        uint32_t p = rw::call(0x40FF58, {outLen + 1});
        st32(out, p);
        if (p != 0) rw::call(rwlib::cocos2d::_base64Encode, {in, len, p});  // cocos2d::_base64Encode
        return outLen;
    }

    // ------------------------------------------------------------------------------------------ xxHash32
    static uint32_t xxh32(uint32_t input, int32_t len, uint32_t seed) {
        const uint32_t P1 = 0x9E3779B1u, P2 = 0x85EBCA77u, P3 = 0xC2B2AE3Du, P4 = 0x27D4EB2Fu, P5 = 0x165667B1u;
        auto rd = [](uint32_t a) {
            return uint32_t(ld8(a)) | (uint32_t(ld8(a + 1)) << 8) | (uint32_t(ld8(a + 2)) << 16) |
                   (uint32_t(ld8(a + 3)) << 24);
        };
        uint32_t p = input, end = input + uint32_t(len);
        uint32_t h;
        if (len >= 16) {
            uint32_t limit = end - 16;
            uint32_t v1 = seed + P1 + P2, v2 = seed + P2, v3 = seed, v4 = seed - P1;
            do {
                v1 = rotl(v1 + rd(p) * P2, 13) * P1;
                v2 = rotl(v2 + rd(p + 4) * P2, 13) * P1;
                v3 = rotl(v3 + rd(p + 8) * P2, 13) * P1;
                v4 = rotl(v4 + rd(p + 12) * P2, 13) * P1;
                p += 16;
            } while (p <= limit);
            h = rotl(v1, 1) + rotl(v2, 7) + rotl(v3, 12) + rotl(v4, 18);
        } else {
            h = seed + P5;
        }
        h += uint32_t(len);
        while (p <= end - 4) {
            h = rotl(h + rd(p) * P3, 17) * P4;
            p += 4;
        }
        while (p < end) {
            h = rotl(h + ld8(p) * P5, 11) * P1;
            p++;
        }
        h ^= h >> 15;
        h *= P2;
        h ^= h >> 13;
        h *= P3;
        h ^= h >> 16;
        return h;
    }

    // ------------------------------------------------------------------------------------------ MD5
    // md5_block_data_order(MD5_CTX *c, const void *data, size_t blocks): the A..D state at c+0..+0xC
    static void md5Blocks(uint32_t ctx, uint32_t data, uint32_t blocks) {
        static const uint32_t K[64] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
        static const int S[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                                  5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
                                  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                                  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
        uint32_t A = ld32(ctx), B = ld32(ctx + 4), C = ld32(ctx + 8), D = ld32(ctx + 0xC);
        for (uint32_t n = blocks; n != 0; n--, data += 64) {
            uint32_t X[16];
            for (int k = 0; k < 16; k++) {
                uint32_t a = data + 4 * k;
                X[k] = uint32_t(ld8(a)) | (uint32_t(ld8(a + 1)) << 8) | (uint32_t(ld8(a + 2)) << 16) |
                       (uint32_t(ld8(a + 3)) << 24);
            }
            uint32_t a = A, b = B, c = C, d = D;
            for (int i = 0; i < 64; i++) {
                uint32_t fv;
                int g;
                if (i < 16) { fv = ((c ^ d) & b) ^ d; g = i; }
                else if (i < 32) { fv = ((b ^ c) & d) ^ c; g = (5 * i + 1) & 15; }
                else if (i < 48) { fv = b ^ c ^ d; g = (3 * i + 5) & 15; }
                else { fv = (~d | b) ^ c; g = (7 * i) & 15; }
                uint32_t t = d;
                d = c;
                c = b;
                b = b + rotl(a + fv + K[i] + X[g], S[i]);
                a = t;
            }
            A += a;
            B += b;
            C += c;
            D += d;
            st32(ctx, A);
            st32(ctx + 4, B);
            st32(ctx + 8, C);
            st32(ctx + 0xC, D);
        }
    }

    // ------------------------------------------------------------------------------------------ OpenSSL
    static uint32_t registerPmethGost(int32_t id, uint32_t pmeth, uint32_t flags) {
        uint32_t m = rw::call(rwlib::EVP_PKEY_meth_new, {uint32_t(id), flags});  // EVP_PKEY_meth_new
        st32(pmeth, m);
        if (m == 0) return 0;
        auto P = [&]() { return ld32(pmeth); };
        if (id == 812) {  // GOST R 34.10-94
            rw::call(rwlib::EVP_PKEY_meth_set_ctrl, {m, rwlib::sub_BF74F4, rwlib::sub_BF7970});           // set_ctrl
            rw::call(rwlib::EVP_PKEY_meth_set_keygen, {P(), 0, rwlib::sub_BF7F54});                // set_keygen
            rw::call(rwlib::EVP_PKEY_meth_set_sign, {P(), 0, rwlib::sub_BF7760});                // set_sign
            rw::call(rwlib::EVP_PKEY_meth_set_verify, {P(), 0, rwlib::sub_BF76F4});                // set_verify
            rw::call(rwlib::EVP_PKEY_meth_set_encrypt, {P(), rwlib::sub_BF71C8, got(0xFD8F58)});    // set_encrypt
            rw::call(rwlib::EVP_PKEY_meth_set_decrypt, {P(), 0, got(0xFD8F5C)});           // set_decrypt
            rw::call(rwlib::EVP_PKEY_meth_set_derive, {P(), rwlib::sub_BF71D0, got(0xFD8F60)});    // set_derive
            rw::call(rwlib::EVP_PKEY_meth_set_paramgen, {P(), rwlib::sub_BF71C0, rwlib::sub_BF78E8});         // set_paramgen
        } else if (id == 815) {  // GOST 28147-89 MAC
            rw::call(rwlib::EVP_PKEY_meth_set_ctrl, {m, rwlib::sub_BF7364, rwlib::sub_BF7CEC});
            rw::call(rwlib::EVP_PKEY_meth_set_signctx, {P(), rwlib::sub_BF71D8, rwlib::sub_BF7320});         // set_signctx
            rw::call(rwlib::EVP_PKEY_meth_set_keygen, {P(), 0, rwlib::sub_BF77D0});
            rw::call(rwlib::EVP_PKEY_meth_set_init, {P(), rwlib::sub_BF72D0});                   // set_init
            rw::call(rwlib::EVP_PKEY_meth_set_cleanup, {P(), rwlib::sub_BF7208});                   // set_cleanup
            rw::call(rwlib::EVP_PKEY_meth_set_copy, {P(), rwlib::sub_BF7E34});                   // set_copy
            return 1;
        } else if (id == 811) {  // GOST R 34.10-2001
            rw::call(rwlib::EVP_PKEY_meth_set_ctrl, {m, rwlib::sub_BF74F4, rwlib::sub_BF7B18});
            rw::call(rwlib::EVP_PKEY_meth_set_sign, {P(), 0, rwlib::sub_BF7684});
            rw::call(rwlib::EVP_PKEY_meth_set_verify, {P(), 0, rwlib::sub_BF7618});
            rw::call(rwlib::EVP_PKEY_meth_set_keygen, {P(), 0, rwlib::sub_BF7EC0});
            rw::call(rwlib::EVP_PKEY_meth_set_encrypt, {P(), rwlib::sub_BF71C8, got(0xFD8F64)});
            rw::call(rwlib::EVP_PKEY_meth_set_decrypt, {P(), 0, got(0xFD8F68)});
            rw::call(rwlib::EVP_PKEY_meth_set_derive, {P(), rwlib::sub_BF71D0, got(0xFD8F6C)});
            rw::call(rwlib::EVP_PKEY_meth_set_paramgen, {P(), rwlib::sub_BF71C0, rwlib::sub_BF785C});
        } else {
            return 0;
        }
        rw::call(rwlib::EVP_PKEY_meth_set_init, {P(), rwlib::sub_BF7218});
        rw::call(rwlib::EVP_PKEY_meth_set_cleanup, {P(), rwlib::sub_BF71E0});
        rw::call(rwlib::EVP_PKEY_meth_set_copy, {P(), rwlib::sub_BF7FE4});
        return 1;
    }
    static uint32_t confFreeData(uint32_t conf) {
        if (conf == 0) return conf;
        uint32_t data = ld32(conf + 8);
        if (data == 0) return conf;
        st32(data + 0x20, 0);
        rw::call(rwlib::lh_doall_arg, {ld32(conf + 8), rwlib::sub_C349BC, ld32(conf + 8)});  // lh_doall_arg(data, value_free_hash)
        rw::call(rwlib::lh_doall, {ld32(conf + 8), rwlib::sub_C34938});                  // lh_doall(data, value_free_stack)
        return rw::call(rwlib::lh_free, {ld32(conf + 8)});                     // lh_free
    }
    // obj_cleanup_defer = 1 for objects 920+ while it is clear
    static uint32_t checkDefer(int32_t nid) {
        uint32_t flag = got(0xFD8C28);
        if (nid >= 920 && ld32(flag) == 0) st32(flag, 1);
        return uint32_t(nid);
    }

    // ------------------------------------------------------------------------------------------ operator new
    static uint32_t operatorNew(uint32_t size) {
        uint32_t n = size != 0 ? size : 1;
        for (;;) {
            uint32_t p = rw::call(0x40FF58, {n});  // malloc
            if (p != 0) return p;
            uint32_t handler = rw::call(0xC98ED5, {});  // std::get_new_handler
            if (handler == 0) {
                uint32_t e = rw::call(0xC98131, {4});  // __cxa_allocate_exception
                st32(e, got(0xFD9200) + 8);           // std::bad_alloc vtable
                rw::call(0xC983D5, {e, got(0xFD90B8), got(0xFD9204)});  // __cxa_throw
                return 0;
            }
            rw::call(handler, {handler});  // the handler (r0 still holds it)
        }
    }
    static uint32_t operatorNewNothrow(uint32_t size, uint32_t) { return rw::call(0xC98EF1, {size}); }

    // ------------------------------------------------------------------------------------------ Tremor codebooks
    static uint32_t oggMalloc(uint32_t n) { return rw::call(ld32(got(0xFD9298)), {n}); }
    static uint32_t oggCalloc(uint32_t n, uint32_t s) { return rw::call(ld32(got(0xFD929C)), {n, s}); }
    static void oggFree(uint32_t p) { rw::call(ld32(got(0xFD92A4)), {p}); }

    static uint32_t ilog(uint32_t v) {
        uint32_t r = 0;
        while (v) {
            v >>= 1;
            r++;
        }
        return r;
    }

    // canonical Huffman codewords from the length list (bit-reversed); 0 on an over-specified tree
    static uint32_t makeWords(uint32_t lengths, int32_t n, uint32_t sparse) {
        guest::Frame f(0xA4);
        uint32_t r = oggMalloc((sparse != 0 ? sparse : uint32_t(n)) << 2);
        rw::call(0x442310, {f.addr(0x1C), 0, 0x84});  // memset(marker, 0, 33 words)
        auto M = [&](int32_t j) { return f.at<uint32_t>(0x1C + uint32_t(j) * 4); };
        if (n > 0) {
            uint32_t count = 0;
            for (int32_t i = 0; i < n; i++) {
                int32_t length = lds32(lengths + uint32_t(i) * 4);
                if (length <= 0) {
                    if (sparse == 0) count++;
                    continue;
                }
                uint32_t entry = *M(length);
                if (length <= 31 && (entry >> length) != 0) {
                    oggFree(r);
                    return 0;
                }
                st32(r + count * 4, entry);
                count++;
                for (int32_t j = length; j > 0; j--) {
                    uint32_t m = *M(j);
                    if (m & 1) {
                        if (j == 1) *M(1) = *M(1) + 1;
                        else *M(j) = *M(j - 1) << 1;
                        break;
                    }
                    *M(j) = m + 1;
                }
                for (int32_t j = length + 1; j < 33; j++) {
                    if ((*M(j) >> 1) == entry) {
                        entry = *M(j);
                        *M(j) = *M(j - 1) << 1;
                    } else {
                        break;
                    }
                }
            }
        }
        if (sparse != 1) {
            for (int32_t i = 1; i < 33; i++) {
                if (*M(i) & (0xFFFFFFFFu >> (32 - i))) {
                    oggFree(r);
                    return 0;
                }
            }
        }
        if (n > 0) {
            uint32_t count = 0;
            for (int32_t i = 0; i < n; i++) {
                int32_t length = lds32(lengths + uint32_t(i) * 4);
                uint32_t temp = 0;
                if (length > 0) {
                    uint32_t w = ld32(r + count * 4);
                    for (int32_t j = 0; j < length; j++) temp = ((w >> j) & 1) | (temp << 1);
                } else if (sparse != 0 && length == 0) {
                    continue;
                }
                st32(r + count * 4, temp);
                count++;
            }
        }
        return r;
    }

    static int32_t maptype1Quantvals(uint32_t book) {
        uint32_t bits = ilog(ld32(book + 4));
        int32_t dim = lds32(book), entries = lds32(book + 4);
        int32_t vals = entries >> int32_t(rw::call(0xCD5E8D, {(bits - 1) * uint32_t(dim - 1), uint32_t(dim)}));
        for (;;) {
            uint32_t acc = 1, acc1 = 1;
            if (dim > 0) {
                for (int32_t k = 0; k < dim; k++) {
                    acc *= uint32_t(vals);
                    acc1 *= uint32_t(vals + 1);
                }
            }
            if (entries < int32_t(acc)) {
                vals--;
                continue;
            }
            if (entries < int32_t(acc1)) return vals;
            vals++;
        }
    }

    // Tremor's packed float: mantissa (normalised to bit 30, signed) and exponent
    static int32_t float32Unpack(uint32_t v, int32_t &point) {
        uint32_t mant = v & 0x1FFFFF;
        if (mant == 0) {
            point = -9999;
            return 0;
        }
        int32_t p = int32_t((v >> 21) & 0x3FF) - 788;
        do {
            mant <<= 1;
            p--;
        } while ((mant & 0x40000000) == 0);
        point = p;
        return int32_t(v) < 0 ? -int32_t(mant) : int32_t(mant);
    }
    // VFLOAT_ADD(a, ap, b, bp) with both non-zero
    static int32_t vfloatAdd(int32_t a, int32_t ap, int32_t b, int32_t bp, int32_t &p) {
        if (ap > bp) {
            int32_t shift = ap - bp + 1;
            p = ap + 1;
            a >>= 1;
            b = shift <= 31 ? int32_t(uint32_t(b) + (1u << (shift - 1))) >> shift : 0;
        } else {
            int32_t shift = bp - ap + 1;
            p = bp + 1;
            b >>= 1;
            a = shift <= 31 ? int32_t(uint32_t(a) + (1u << (shift - 1))) >> shift : 0;
        }
        int32_t s = int32_t(uint32_t(a) + uint32_t(b));
        uint32_t top = uint32_t(s) & 0xC0000000u;
        if (top == 0 || top == 0xC0000000u) {
            s = int32_t(uint32_t(s) << 1);
            p--;
        }
        return s;
    }

    static uint32_t bookUnquantize(uint32_t book, int32_t n, uint32_t sparse, const uint32_t *maxPointPtr) {
        const uint32_t maxpoint = rw::guest_addr(maxPointPtr);
        if (ld32(book + 0xC) - 1u > 1u) return 0;
        int32_t minpoint, delpoint;
        int32_t mindel = float32Unpack(ld32(book + 0x10), minpoint);
        int32_t delta = float32Unpack(ld32(book + 0x14), delpoint);
        uint32_t r = oggCalloc(ld32(book) * uint32_t(n), 4);
        uint32_t rp = oggCalloc(ld32(book) * uint32_t(n), 4);
        st32(maxpoint, uint32_t(minpoint));
        uint32_t maptype = ld32(book + 0xC);
        int32_t quantvals = 0;
        if (maptype == 1) quantvals = maptype1Quantvals(book);
        if (maptype == 1 || maptype == 2) {
            int32_t entries = lds32(book + 4);
            int32_t count = 0;
            for (int32_t j = 0; j < entries; j = j + 1, entries = lds32(book + 4)) {
                if (sparse != 0 && ld32(ld32(book + 8) + uint32_t(j) * 4) == 0) continue;
                int32_t dim = lds32(book);
                if (dim > 0) {
                    int32_t last = 0, lastpoint = 0;
                    uint32_t indexdiv = 1;
                    for (int32_t k = 0; k < dim; k++, dim = lds32(book)) {
                        uint32_t index;
                        if (maptype == 1) {
                            uint32_t q = rw::call(0xCD5E8D, {uint32_t(j), indexdiv});              // __divsi3
                            index = uint32_t(rw::call64(0xCD5F29, {q, uint32_t(quantvals)}) >> 32);  // % quantvals
                        } else {
                            index = uint32_t(dim) * uint32_t(j) + uint32_t(k);
                        }
                        int32_t q = lds32(ld32(book + 0x20) + index * 4);
                        uint32_t iabs = q < 0 ? uint32_t(0) - uint32_t(q) : uint32_t(q);
                        int32_t ip = int32_t(ilog(iabs)) - 31;
                        uint32_t scaled = lslReg(iabs, uint32_t(-ip));
                        int32_t val, point;
                        if (scaled != 0 && delta != 0) {
                            int64_t prod = int64_t(delta) * int64_t(int32_t(scaled));
                            val = int32_t(uint64_t(prod) >> 32);
                            point = delpoint + ip + 32;
                            if (mindel != 0) {
                                if (val == 0) {
                                    val = mindel;
                                    point = minpoint;
                                } else {
                                    val = vfloatAdd(mindel, minpoint, val, point, point);
                                }
                            }
                        } else if (mindel != 0) {
                            val = mindel;
                            point = minpoint;
                        } else {
                            val = 0;
                            point = 0;
                        }
                        if (last != 0) {
                            if (val == 0) {
                                val = last;
                                point = lastpoint;
                            } else {
                                val = vfloatAdd(last, lastpoint, val, point, point);
                            }
                        }
                        if (ld32(book + 0x1C) != 0) {
                            last = val;
                            lastpoint = point;
                        }
                        uint32_t slot;
                        if (sparse != 0) slot = ld32(sparse + uint32_t(count) * 4) * ld32(book) + uint32_t(k);
                        else slot = uint32_t(count) * ld32(book) + uint32_t(k);
                        st32(r + slot * 4, uint32_t(val));
                        if (sparse != 0) slot = ld32(sparse + uint32_t(count) * 4) * ld32(book) + uint32_t(k);
                        st32(rp + slot * 4, uint32_t(point));
                        if (lds32(maxpoint) < point) st32(maxpoint, uint32_t(point));
                        if (maptype == 1) indexdiv *= uint32_t(quantvals);
                    }
                }
                count++;
            }
        }
        int32_t total = int32_t(ld32(book) * uint32_t(n));
        for (int32_t j = 0; j < total; j++) {
            int32_t mp = lds32(maxpoint), pj = lds32(rp + uint32_t(j) * 4);
            if (pj < mp) st32(r + uint32_t(j) * 4, uint32_t(asrReg(lds32(r + uint32_t(j) * 4), uint32_t(mp - pj))));
        }
        oggFree(rp);
        return r;
    }
};
RW_FUNCTION(0x905A88, "base64Decode", Lib::base64Decode);
RW_FUNCTION(0x905B2C, "base64Encode", Lib::base64Encode);
RW_FUNCTION(0x936E1C, "XXH32", Lib::xxh32);
RW_FUNCTION(0xB7FE50, "md5_block_data_order", Lib::md5Blocks);
RW_FUNCTION(0xBF80D4, "register_pmeth_gost", Lib::registerPmethGost);
RW_FUNCTION(0xC34C50, "_CONF_free_data", Lib::confFreeData);
RW_FUNCTION(0xB7DC84, "check_defer", Lib::checkDefer);
RW_FUNCTION(0xC98EF0, "operator new(unsigned)", Lib::operatorNew);
RW_FUNCTION(0xC98F4C, "operator new(unsigned, const std::nothrow_t&)", Lib::operatorNewNothrow);
RW_FUNCTION(0xCEC410, "_ilog", Lib::ilog);
RW_FUNCTION(0xCEC434, "_make_words", Lib::makeWords);
RW_FUNCTION(0xCEC6CC, "_book_maptype1_quantvals", Lib::maptype1Quantvals);
RW_FUNCTION(0xCEC75C, "_book_unquantize", Lib::bookUnquantize);

}  // namespace freelib
