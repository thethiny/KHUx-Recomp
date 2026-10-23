"""Split the library's functions into the game's own code and open-source libraries (progress reporting).

python recomp/rewrite/tools/libsplit.py            counts per group and per library
python recomp/rewrite/tools/libsplit.py ADDR       the group of one function

Open-source libraries are the ones a native build can take from upstream source (at the step that leaves the
guest memory model): cocos2d-x and the libraries bundled with it, LWF, libc++ and the C++ runtime, and the C
libraries (libpng, freetype, OpenSSL, curl, zlib, libjpeg, ...). Everything else is game code - including
Square Enix's sound engine (SQEX / SdSoundSystem), which is not open source. Unnamed functions (sub_XXXX) take the
group of the nearest named function before them: the linker keeps each library's objects together.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import unit  # noqa: E402

LIBS = [
    ("cocos2d-x", r"(cocos2d::|cocostudio::|CocosDenshion::|cocosbuilder::|spine::|sp[A-Z]\w*_|"
                  r"flatbuffers::|tinyxml2::|rapidjson::|ClipperLib::|Json::|unz|zip[A-Z]|ConvertUTF|kmMat|kmVec|"
                  r"kmQuaternion|kmPlane|kmGL|ccDrawInit|ccGL|_spAtlas|spAnimation|spSkeleton|spBone|spSlot|"
                  r"poly2tri|p2t::|Box2D|b2[A-Z]|chipmunk|cp[A-Z]\w+|cpv|"
                  # the Android glue and helpers bundled with cocos2d (platform/android/jni, etc1, xxhash,
                  # ConvertUTF, edtaa3func, VideoPlayer-android, the plugin-x wrappers)
                  r"Java_org_cocos2dx_lib_|Java_org_cocos2dx_plugin_|\w*JNI(::)?$|\w*Jni(::)?$|etc1_|XXH|"
                  r"getUTF8StringLength|isLegalUTF8|computegradient|edgedf|distaa3|edtaa3|setVideoVisible|"
                  r"cc_assert|"
                  # the NDK cpufeatures module and cocos2d's texture decoders (C names); covariant thunks
                  # (_ZTc...) of cocos2d classes, which outer_name does not unwrap
                  r"android_getCpu|android_setCpu|atitc_|s3tc_|_ZTc\w*cocos2d)"),
    ("LWF", r"(LWF::|lwf_)"),
    ("libc++ / C++ runtime", r"(std::|__cxa|__cxxabiv1|__gnu_|_Unwind|__aeabi|__udiv|__div|__mod|__umod|"
                             r"operator new|operator delete|__dynamic_cast|__gxx|__cxx|_ZN?St|__clz|__ctz|"
                             r"__(add|sub|mul|div|cmp|fix|float|extend|trunc|neg|ash|lsh|lsr)\w*[sd]f\d|__popcount|"
                             # libunwind (unw_* and its C++ namespace) and compiler-rt's soft-float helpers
                             r"unw_|libunwind::|__(gt|lt|ge|le|eq|ne|un)[sd]f2$|__float(un)?[sd]i[sd]f$|"
                             r"__fix(uns)?[sd]f[sd]i$)"),
    ("libpng", r"(png_|_png)"),
    ("freetype", r"(FT_|ft_|af_|tt_|TT_|cff_|CFF_|ps_|PS_|t1_|T1_|t42_|pfr_|bdf_|pcf_|sfnt_|gxv_|otv_|FTC_|"
                 r"winfnt|pshinter|psh_|psaux|_ft_|tt_face|ft_smooth|gray_)"),
    ("OpenSSL", r"(EVP_|SSL|ssl|BN_|bn_|ASN1|asn1|X509|x509|RSA|rsa_|EC_|ec_|ECDSA|ECDH|CRYPTO|ERR_|BIO|bio_|"
                r"OBJ_|obj_|PEM|pem_|DH_|dh_|DSA|dsa_|HMAC|MD4|MD5|SHA|AES|aes_|DES|des_|RC4|RAND|rand_|CMS|PKCS|"
                r"pkcs|OPENSSL|OpenSSL|ENGINE|engine_|CONF|conf_|UI_|COMP|TLS|tls|dtls|DTLS|i2d_|d2i_|i2v|v2i|i2s|"
                r"s2i|i2r|BUF_|lh_|LHASH|sk_|OCSP|ocsp|TS_|NCONF|CAST|cast_|BF_|bf_|CAMELLIA|Camellia|SEED|IDEA|"
                r"MDC2|RIPEMD|WHIRLPOOL|CMAC|GCM|CRYPTO_|gcm|ecdh|ecdsa|ec_GF|EC_GF|SRP|srp|policy_|level_|"
                r"tree_|v3_|X9|ssl3|SSLv|TLSv|dtls1|ssl23|asn1_|ASN1_|cms_|ts_|NETSCAPE|PROXY|AUTHORITY|DIST|"
                r"ACCESS|USERNOTICE|POLICY|GENERAL|EDIPARTY|OTHERNAME|SXNET|ISSUING|CRL|crl|EXTENDED|BASIC|"
                r"AES_|private_|EVP|HMAC_|MD5_|SHA1|SHA256|SHA512|ERR|ERR_load|BIO_|PEM_)"),
    # OpenSSL's C helpers whose names the prefixes above miss (asn1 / x509v3 / dso / txt_db / pqueue / rc2 / idea /
    # md4 / ripemd / whirlpool / smime and the GOST engine's helpers), matched whole
    ("OpenSSL", r"(n_ssl3_mac|do_dtls1_write|md4_block|int_rsa_verify|a2d_ASN1|[ic]2[aic]_ASN1|a2i_|i2a_|"
                r"hex_to_string|string_to_hex|name_cmp|NAME_CONSTRAINTS|pqueue_|RC2_|idea_|o2i_|i2o_|ECPK|"
                r"DHparams|DSO_|UTF8_(get|put)c|_CONF_|TXT_DB_|pkey_GOST|fill_GOST|getbnfrombuf|unpack_cp_|"
                r"hashsum2bn|store_bignum|pack_sign_cp|mac_block|get_mac$|get_encryption_params|start_hash|"
                r"hash_block|finish_hash|keyDiversifyCryptoPro|keyUnwrapCryptoPro|ripemd160|whirlpool|SMIME_)"),
    ("curl", r"(curl|Curl|CURL)"),
    ("Ogg Vorbis", r"(vorbis|ogg_|ov_|_vorbis|_ogg|oggpack|drft_|mdct_|_ve_|floor0|floor1|res0|res1|res2|"
                   r"mapping0|vorbis_)"),
    ("OpenSSL (engines)", r"(gost|GOST|hwcrypto|ubsec|aep_|atalla|chil|cswift|nuron|sureware|padlock|"
                          r"capi_|e_gost|ccgost|keyWrap|cryptopro|gostcrypt|get_gost|register_ameth|"
                          r"init_gost|pkey_gost|gost2001|gost94|gost_)"),
    ("zlib", r"(inflate|deflate|crc32|adler32|zcalloc|zcfree|zError|zlib|_tr_|gz[a-z]|compress|uncompress|"
             r"get_crc_table)"),
    ("libjpeg", r"(jpeg_|jinit|jcopy|jdiv|jround|jzero|jsimd|jpeg)"),
    ("libtiff / webp / xml", r"(TIFF|_TIFF|tiff|WebP|webp|VP8|vp8|xml|XML|html|HTML|LogL16|LogL10|LogLuv|"
                             r"uv_encode|uv_decode|XYZtoRGB24|libport_|Huffman|EstimateBestFilter|QuantizeLevels)"),
    ("LZMA SDK", r"(Lzma)"),
    ("websockets", r"(lws|libwebsocket|_lws|interface_to_sa)"),
    ("libc / math helpers", r"(__sfp|__smakebuf|__swsetup|__sflush|__srefill|__svfscanf|__vfprintf|__dtoa|"
                            r"__hdtoa|__ldtoa|__gdtoa|__mprec|__Balloc|__Bfree|__multadd|__s2b|__hi0bits|"
                            r"__lo0bits|__i2b|__multiply|__pow5mult|__lshift|__cmp|__mdiff|__ulp|__b2d|__d2b|"
                            r"__ratio|__tens|__strtod|_strtod|strtod|__ieee754|__kernel|scalbn|copysign|"
                            r"frexp|ldexp|modf|fmod|floor|ceil|sqrt|sin|cos|tan|exp|log|pow|atan|asin|acos|lfind)"),
]
_LIB_RE = [(name, re.compile(pat)) for name, pat in LIBS]

# Unnamed functions the nearest-name rule gives to the game although they are a library's, classified by what they
# reference (2026-10-20): the libraries' static constructors (.init_array, linked after the game's own sources
# 0x453280-0x454160) and helpers the boot runs.
_COCOS = "cocos2d-x"
OVERRIDES = {
    # cocostudio: DataReaderHelper, the Com* components and widget readers (ObjectFactory::TInfo registrations)
    **{a: _COCOS for a in (0x45534C, 0x455388, 0x4553DC, 0x45548C, 0x455574, 0x455644, 0x455728, 0x455818,
                           0x45591C, 0x455A24, 0x455B34, 0x455C2C, 0x455D34, 0x455E40, 0x455F50, 0x45605C,
                           0x456170, 0x456274, 0x45636C, 0x456480, 0x456578)},
    # cocos2d: HttpClient's mutexes / flags, JNI and plugin-x globals, the ui widgets' TInfo, VideoPlayer, colors,
    # fonts, Data::Null, the event listener ids (__cc_touch_one_by_one ...), apk path, renderer statics
    **{a: _COCOS for a in (0x45666C, 0x4567B8, 0x4567F4, 0x456854, 0x456A80, 0x456B68, 0x456C38, 0x456D08,
                           0x456EA0, 0x456F84, 0x457074, 0x457164, 0x457224, 0x4572F8, 0x4573D0, 0x4574C8,
                           0x4575B0, 0x457684, 0x4576C0, 0x4576E4, 0x457714, 0x457754, 0x457784, 0x4577C4,
                           0x457800, 0x4578B4, 0x4579C4, 0x457EC4, 0x457F18, 0x457F4C, 0x457FA4, 0x457FFC,
                           0x458074, 0x4580E4, 0x45813C, 0x4581E8, 0x45821C, 0x4582CC, 0x4584EC, 0x458524,
                           0x45888C, 0x4588C4, 0x458EFC, 0x458F3C, 0x458F7C, 0x458FB4, 0x92E42C)},
    # cocos2d-x's Android glue (platform/android/jni: Java_org_cocos2dx_lib_*, the edit-text dialog, font size);
    # their extern "C" names do not match the cocos2d patterns
    **{a: _COCOS for a in (0xA407F0, 0xA40A60, 0xA40C3C, 0xA41D74)},
    **{a: "LWF" for a in (0x456714, 0x456748, 0x456780)},
    # a vector destructor and a string helper instantiated in the game's objects
    **{a: "libc++ / C++ runtime" for a in (0x493E84, 0x498A1C)},
    # newlocale / uselocale, strtod, the vfprintf family and its output sink, printf; with IDA's chunks of strtod
    # (0xC9DF62, 0xC9E16A) and vfprintf (0xCA338A, 0xCA3A22, 0xCA3B02)
    **{a: "libc / math helpers" for a in (0xC9C858, 0xC9C8E0, 0xC9CC58, 0xCA1C5C, 0xCA1CDE, 0xCA358E, 0xCA35A4,
                                          0xCA3B24, 0xCA3BC4, 0xCD2ED0, 0xCA1BF4, 0xCA4A3C, 0xC9DF62, 0xC9E16A,
                                          0xCA338A, 0xCA3A22, 0xCA3B02)},
    **{a: "curl" for a in (0x44F07C, 0x44F154, 0x44F368, 0x44F3D0)},
    **{a: "OpenSSL" for a in (0xC34D9C, 0xC34EB0, 0xC353F8, 0xC35EEC)},   # conf_def.c
}


# the outermost name of an Itanium-mangled symbol: "_ZN7cocos2d6Sprite..." -> "cocos2d", "_ZNSt3__1..." -> "std",
# "_ZThn20_N7cocos2d..." (thunks) -> "cocos2d", "_Z12freeFunctioni" -> "freeFunction"
_MANGLED = re.compile(r"_Z(?:T[hv][n\d_]*?_|TV|TI|TS|GV|GR)?N?[KVr]*(?:(St)|(\d+))")


def outer_name(name):
    m = _MANGLED.match(name)
    if not m:
        return name
    if m.group(1):
        return "std::"
    n = int(m.group(2))
    start = m.end()
    return name[start:start + n] + "::"


def lib_of(name):
    """The open-source library a named function belongs to, or None for game code."""
    key = outer_name(name)
    for lib, rx in _LIB_RE:
        if rx.match(key):
            return lib
    return None


def groups():
    """{address: library name or None (game)} for every analysed function."""
    idx = unit.load_index()
    funcs = idx.get("functions", idx) if isinstance(idx, dict) else idx
    items = sorted((int(k, 16), v["name"]) for k, v in funcs.items() if isinstance(v, dict) and "name" in v)
    out, last = {}, None
    for a, name in items:
        if name.startswith(("sub_", "nullsub_", "j_", "loc_", "def_", "jpt_")):
            out[a] = last
        else:
            last = lib_of(name)
            out[a] = last
    for a, lib in OVERRIDES.items():
        if a in out: out[a] = lib
        elif a | 1 in out: out[a | 1] = lib
    # unnamed functions the nearest-name rule gives to the game although only one library calls them (IDA's
    # outlined pieces of freetype / curl at the start of .text): the callers' library, to a fixed point
    callers = call_graph()
    unnamed = [a for a, name in items if name.startswith(("sub_", "nullsub_", "j_")) and out.get(a) is None]
    changed = True
    while changed:
        changed = False
        for a in unnamed:
            if out.get(a) is not None:
                continue
            libs = {out.get(c) for c in callers.get(a, ())}
            # not libc++: its templates (std::function thunks, std::sort) call the game's own lambdas
            if len(libs) == 1 and None not in libs and "libc++ / C++ runtime" not in libs:
                out[a] = libs.pop()
                changed = True
    return out


_CALLS = None


def call_graph():
    """callee -> callers, from the lifted C (rt_call / TAIL targets)."""
    global _CALLS
    if _CALLS is None:
        import collections
        _CALLS = collections.defaultdict(set)
        fn = re.compile(r"^void F_([0-9A-F]+)\(CPU \*c\) \{")
        call = re.compile(r"(?:rt_call(?:_frag)?\(c, |TAIL\()0x([0-9A-F]+)u\)")
        for f in sorted(unit.LIFT.glob("lift_*.c")):
            if f.name == "lift_table.c":
                continue
            cur = None
            for line in f.read_text(encoding="utf-8", errors="replace").splitlines():
                m = fn.match(line)
                if m:
                    cur = int(m.group(1), 16)
                    continue
                if cur is not None and "0x" in line:
                    for t in call.findall(line):
                        _CALLS[int(t, 16) & ~1].add(cur)
    return _CALLS


def main():
    g = groups()
    if len(sys.argv) > 1:
        a = int(sys.argv[1], 16) & ~1
        print(g.get(a, g.get(a | 1)) or "game code")
        return
    from collections import Counter
    c = Counter(v or "game code" for v in g.values())
    for k, n in c.most_common():
        print(f"{n:7,}  {k}")


if __name__ == "__main__":
    main()
