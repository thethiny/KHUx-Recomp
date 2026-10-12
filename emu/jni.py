"""Fake JavaVM / JNIEnv for the guest.

Java objects live on the host as Python values behind opaque handles (unmapped guest addresses, so
a guest dereference faults loudly). Java methods are implemented in JAVA below, keyed
"pkg/Class.name(sig)" or "pkg/Class.name"; unknown methods log and return a default.
"""
import struct

from unicorn.arm_const import UC_ARM_REG_SP

from .core import GuestFault
from .libc import Args, u2f, f2u, d2u

JNI_NAMES = (["reserved0", "reserved1", "reserved2", "reserved3", "GetVersion", "DefineClass", "FindClass",
              "FromReflectedMethod", "FromReflectedField", "ToReflectedMethod", "GetSuperclass",
              "IsAssignableFrom", "ToReflectedField", "Throw", "ThrowNew", "ExceptionOccurred",
              "ExceptionDescribe", "ExceptionClear", "FatalError", "PushLocalFrame", "PopLocalFrame",
              "NewGlobalRef", "DeleteGlobalRef", "DeleteLocalRef", "IsSameObject", "NewLocalRef",
              "EnsureLocalCapacity", "AllocObject", "NewObject", "NewObjectV", "NewObjectA", "GetObjectClass",
              "IsInstanceOf", "GetMethodID"]
             + [f"Call{t}Method{v}" for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int", "Long",
                                              "Float", "Double", "Void") for v in ("", "V", "A")]
             + [f"CallNonvirtual{t}Method{v}" for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int",
                                                        "Long", "Float", "Double", "Void") for v in ("", "V", "A")]
             + ["GetFieldID"]
             + [f"Get{t}Field" for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int", "Long", "Float",
                                         "Double")]
             + [f"Set{t}Field" for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int", "Long", "Float",
                                         "Double")]
             + ["GetStaticMethodID"]
             + [f"CallStatic{t}Method{v}" for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int", "Long",
                                                    "Float", "Double", "Void") for v in ("", "V", "A")]
             + ["GetStaticFieldID"]
             + [f"GetStatic{t}Field" for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int", "Long",
                                               "Float", "Double")]
             + [f"SetStatic{t}Field" for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int", "Long",
                                               "Float", "Double")]
             + ["NewString", "GetStringLength", "GetStringChars", "ReleaseStringChars", "NewStringUTF",
                "GetStringUTFLength", "GetStringUTFChars", "ReleaseStringUTFChars", "GetArrayLength",
                "NewObjectArray", "GetObjectArrayElement", "SetObjectArrayElement"]
             + [f"New{t}Array" for t in ("Boolean", "Byte", "Char", "Short", "Int", "Long", "Float", "Double")]
             + [f"Get{t}ArrayElements" for t in ("Boolean", "Byte", "Char", "Short", "Int", "Long", "Float",
                                                 "Double")]
             + [f"Release{t}ArrayElements" for t in ("Boolean", "Byte", "Char", "Short", "Int", "Long", "Float",
                                                     "Double")]
             + [f"Get{t}ArrayRegion" for t in ("Boolean", "Byte", "Char", "Short", "Int", "Long", "Float",
                                               "Double")]
             + [f"Set{t}ArrayRegion" for t in ("Boolean", "Byte", "Char", "Short", "Int", "Long", "Float",
                                               "Double")]
             + ["RegisterNatives", "UnregisterNatives", "MonitorEnter", "MonitorExit", "GetJavaVM",
                "GetStringRegion", "GetStringUTFRegion", "GetPrimitiveArrayCritical",
                "ReleasePrimitiveArrayCritical", "GetStringCritical", "ReleaseStringCritical", "NewWeakGlobalRef",
                "DeleteWeakGlobalRef", "ExceptionCheck", "NewDirectByteBuffer", "GetDirectBufferAddress",
                "GetDirectBufferCapacity", "GetObjectRefType"])
assert len(JNI_NAMES) == 233, len(JNI_NAMES)

HANDLE_BASE = 0xF0000000   # unmapped: a guest dereference of a Java handle faults
PRIM_SIZES = {"Z": 1, "B": 1, "C": 2, "S": 2, "I": 4, "J": 8, "F": 4, "D": 8}
ARRAY_TYPES = {"Boolean": "Z", "Byte": "B", "Char": "C", "Short": "S", "Int": "I", "Long": "J", "Float": "F",
               "Double": "D"}


class JClass:
    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return f"<class {self.name}>"


class JObject:
    def __init__(self, cls, **fields):
        self.cls = cls
        self.fields = fields

    def __repr__(self):
        return f"<{self.cls} {self.fields}>"


class JArray:
    def __init__(self, kind, items):
        self.kind, self.items = kind, items


def parse_sig(sig):
    """'(ILjava/lang/String;[B)V' -> (['I', 'Ljava/lang/String;', '[B'], 'V')"""
    assert sig[0] == "("
    i, params = 1, []
    while sig[i] != ")":
        j = i
        while sig[j] == "[":
            j += 1
        if sig[j] == "L":
            j = sig.index(";", j)
        params.append(sig[i:j + 1])
        i = j + 1
    return params, sig[i + 1:]


class JNI:
    def __init__(self, emu):
        self.emu = emu
        self.objects = {}          # handle -> python value
        self.handle_of = {}        # id(value) -> handle (strings get fresh handles)
        self.next_handle = HANDLE_BASE
        self.methods = {}          # method id -> (class, name, sig, static)
        self.fields = {}
        self.next_id = 0xF8000000   # method/field IDs: opaque, outside every mapped range
        self.classes = {}
        self.guest_strings = {}    # guest char* -> handle (for Release)

        e = emu
        table = e.alloc(4 * len(JNI_NAMES))
        for k, name in enumerate(JNI_NAMES):
            fn = getattr(self, "j_" + name, None)
            if fn is None:
                fn = self._generic(name)
            e.w32(table + 4 * k, e.host_function("JNIEnv::" + name, self._wrap(name, fn)))
        self.env = e.alloc(4, struct.pack("<I", table))

        vm_names = ["r0", "r1", "r2", "DestroyJavaVM", "AttachCurrentThread", "DetachCurrentThread", "GetEnv",
                    "AttachCurrentThreadAsDaemon"]
        vtable = e.alloc(4 * len(vm_names))
        for k, name in enumerate(vm_names):
            e.w32(vtable + 4 * k, e.host_function("JavaVM::" + name, self._vm(name)))
        self.vm = e.alloc(4, struct.pack("<I", vtable))

    # ------------------------------------------------------------------ handles
    def new_ref(self, v):
        if v is None:
            return 0
        if not isinstance(v, str):
            h = self.handle_of.get(id(v))
            if h:
                return h
        h = self.next_handle
        self.next_handle += 4
        self.objects[h] = v
        if not isinstance(v, str):
            self.handle_of[id(v)] = h
        return h

    def obj(self, h):
        if h == 0:
            return None
        if h not in self.objects:
            raise GuestFault(f"JNI: unknown object handle {h:#x}")
        return self.objects[h]

    def find_class(self, name):
        if name not in self.classes:
            self.classes[name] = JClass(name)
        return self.classes[name]

    # ------------------------------------------------------------------ plumbing
    def _wrap(self, name, fn):
        def handler(e):
            r = fn(e)
            if r is not None:
                if isinstance(r, tuple) and r[0] == "64":
                    e.ret64(r[1])
                else:
                    e.ret(r)
        return handler

    def _generic(self, name):
        def fn(e):
            raise GuestFault(f"JNI {name} not implemented (lr={e.reg(14):#x})")
        return fn

    def _vm(self, name):
        def fn(e):
            if name in ("GetEnv", "AttachCurrentThread", "AttachCurrentThreadAsDaemon"):
                vm, penv = e.args(2)
                e.w32(penv, self.env)
                e.ret(0)
            else:
                e.ret(0)
        return fn

    # ------------------------------------------------------------------ args by signature
    def _read_args(self, e, params, a, jvalues=None):
        out = []
        for k, p in enumerate(params):
            if jvalues is not None:
                raw = e.read(jvalues + 8 * k, 8)
                if p in "ZBCSI":
                    v = struct.unpack_from("<i", raw)[0]
                elif p == "J":
                    v = struct.unpack_from("<q", raw)[0]
                elif p == "F":
                    v = struct.unpack_from("<f", raw)[0]
                elif p == "D":
                    v = struct.unpack_from("<d", raw)[0]
                else:
                    v = self.obj(struct.unpack_from("<I", raw)[0])
            else:  # varargs or va_list: float promoted to double
                if p in "ZBCSI":
                    v = a.s32()
                elif p == "J":
                    v = a.s64()
                elif p in "FD":
                    v = a.d()
                else:
                    v = self.obj(a.i())
            out.append(v)
        return out

    def _ret_value(self, rtype, v):
        if rtype == "V":
            return 0
        if rtype in "ZBCSI":
            return int(v or 0) & 0xFFFFFFFF
        if rtype == "J":
            return ("64", int(v or 0))
        if rtype == "F":
            return f2u(float(v or 0))
        if rtype == "D":
            return ("64", d2u(float(v or 0)))
        return self.new_ref(v)

    def _invoke(self, e, kind, variant, static):
        """Call<kind>Method[V|A] and CallStatic... : (env, obj/cls, mid, ...)"""
        env, target, mid = e.args(3)
        cls, name, sig, _ = self.methods[mid]
        params, rtype = parse_sig(sig)
        if variant == "":
            args = self._read_args(e, params, Args(e, 3))
        elif variant == "V":
            args = self._read_args(e, params, Args(e, va_ptr=e.args(4)[3]))
        else:
            args = self._read_args(e, params, None, jvalues=e.args(4)[3])
        this = None if static else self.obj(target)
        return self._ret_value(rtype, java_call(self, cls, name, sig, this, args))

    # ------------------------------------------------------------------ JNIEnv functions
    def j_GetVersion(self, e):
        return 0x10006

    def j_FindClass(self, e):
        env, name = e.args(2)
        return self.new_ref(self.find_class(e.cstr(name)))

    def j_GetObjectClass(self, e):
        env, o = e.args(2)
        v = self.obj(o)
        name = v.cls if isinstance(v, JObject) else ("java/lang/String" if isinstance(v, str) else "java/lang/Object")
        return self.new_ref(self.find_class(name))

    def j_GetSuperclass(self, e):
        return self.new_ref(self.find_class("java/lang/Object"))

    def j_IsAssignableFrom(self, e):
        return 1

    def j_IsInstanceOf(self, e):
        return 1

    def _get_mid(self, e, static):
        env, c, name, sig = e.args(4)
        cls = self.obj(c)
        key = (cls.name, e.cstr(name), e.cstr(sig), static)
        mid = self.next_id
        self.next_id += 4
        self.methods[mid] = key
        return mid

    def j_GetMethodID(self, e):
        return self._get_mid(e, False)

    def j_GetStaticMethodID(self, e):
        return self._get_mid(e, True)

    def j_GetFieldID(self, e):
        env, c, name, sig = e.args(4)
        fid = self.next_id
        self.next_id += 4
        self.fields[fid] = (self.obj(c).name, e.cstr(name), e.cstr(sig))
        return fid

    j_GetStaticFieldID = j_GetFieldID

    def j_GetStaticObjectField(self, e):
        env, c, fid = e.args(3)
        cls, name, sig = self.fields[fid]
        return self.new_ref(java_static_field(self, cls, name, sig))

    def j_GetStaticIntField(self, e):
        env, c, fid = e.args(3)
        cls, name, sig = self.fields[fid]
        return int(java_static_field(self, cls, name, sig) or 0) & 0xFFFFFFFF

    def j_GetIntField(self, e):
        env, o, fid = e.args(3)
        cls, name, sig = self.fields[fid]
        return int(self.obj(o).fields.get(name, 0)) & 0xFFFFFFFF

    def j_GetObjectField(self, e):
        env, o, fid = e.args(3)
        cls, name, sig = self.fields[fid]
        return self.new_ref(self.obj(o).fields.get(name))

    def j_NewObject(self, e):
        env, c, mid = e.args(3)
        cls, name, sig, _ = self.methods[mid]
        params, _r = parse_sig(sig)
        args = self._read_args(e, params, Args(e, 3))
        return self.new_ref(java_new(self, cls, sig, args))

    def j_NewObjectV(self, e):
        env, c, mid, va = e.args(4)
        cls, name, sig, _ = self.methods[mid]
        params, _r = parse_sig(sig)
        return self.new_ref(java_new(self, cls, sig, self._read_args(e, params, Args(e, va_ptr=va))))

    def j_ExceptionCheck(self, e):
        return 0

    def j_ExceptionOccurred(self, e):
        return 0

    def j_ExceptionClear(self, e):
        return 0

    def j_ExceptionDescribe(self, e):
        return 0

    def j_NewGlobalRef(self, e):
        return e.args(2)[1]

    def j_NewLocalRef(self, e):
        return e.args(2)[1]

    j_NewWeakGlobalRef = j_NewGlobalRef

    def j_DeleteGlobalRef(self, e):
        return 0

    j_DeleteLocalRef = j_DeleteGlobalRef
    j_DeleteWeakGlobalRef = j_DeleteGlobalRef

    def j_PushLocalFrame(self, e):
        return 0

    def j_PopLocalFrame(self, e):
        return e.args(2)[1]

    def j_EnsureLocalCapacity(self, e):
        return 0

    def j_IsSameObject(self, e):
        env, a, b = e.args(3)
        return int(a == b or (a and b and self.obj(a) is self.obj(b)))

    def j_GetJavaVM(self, e):
        env, pvm = e.args(2)
        e.w32(pvm, self.vm)
        return 0

    def j_RegisterNatives(self, e):
        env, c, methods, n = e.args(4)
        for k in range(n):
            nm, sig, fn = struct.unpack("<III", e.read(methods + 12 * k, 12))
            e.log(f"  RegisterNatives {self.obj(c).name}.{e.cstr(nm)}{e.cstr(sig)} -> {fn:#x}")
        return 0

    # strings
    def j_NewStringUTF(self, e):
        env, s = e.args(2)
        return self.new_ref(e.cbytes(s).decode("utf-8", "replace")) if s else 0

    def j_GetStringUTFChars(self, e):
        env, h, iscopy = e.args(3)
        s = self.obj(h)
        if iscopy:
            e.write(iscopy, b"\1")
        return e.alloc_str(s)

    def j_ReleaseStringUTFChars(self, e):
        env, h, p = e.args(3)
        e.heap.free(p)
        return 0

    def j_GetStringUTFLength(self, e):
        return len(self.obj(e.args(2)[1]).encode())

    def j_GetStringLength(self, e):
        return len(self.obj(e.args(2)[1]).encode("utf-16-le")) // 2

    def j_GetStringChars(self, e):
        env, h, iscopy = e.args(3)
        b = self.obj(h).encode("utf-16-le")
        return e.alloc(len(b) + 2, b + b"\0\0")

    def j_ReleaseStringChars(self, e):
        e.heap.free(e.args(3)[2])
        return 0

    def j_NewString(self, e):
        env, p, n = e.args(3)
        return self.new_ref(e.read(p, 2 * n).decode("utf-16-le", "replace"))

    def j_GetStringUTFRegion(self, e):
        env, h, start, n, buf = e.args(5)
        e.write(buf, self.obj(h)[start:start + n].encode() + b"\0")
        return 0

    # arrays
    def j_GetArrayLength(self, e):
        return len(self.obj(e.args(2)[1]).items)

    def j_NewObjectArray(self, e):
        env, n, c, init = e.args(4)
        return self.new_ref(JArray("L", [self.obj(init)] * n))

    def j_GetObjectArrayElement(self, e):
        env, a, i = e.args(3)
        return self.new_ref(self.obj(a).items[i])

    def j_SetObjectArrayElement(self, e):
        env, a, i, v = e.args(4)
        self.obj(a).items[i] = self.obj(v)
        return 0


def _prim_array_funcs():
    for tname, code in ARRAY_TYPES.items():
        size = PRIM_SIZES[code]

        def new(self, e, code=code):
            env, n = e.args(2)
            return self.new_ref(JArray(code, bytearray(n * PRIM_SIZES[code])))

        def get_elems(self, e, size=size):
            env, h, iscopy = e.args(3)
            arr = self.obj(h)
            data = bytes(arr.items)
            p = e.alloc(max(len(data), 4), data)
            if iscopy:
                e.write(iscopy, b"\1")
            return p

        def release_elems(self, e, size=size):
            env, h, p, mode = e.args(4)
            arr = self.obj(h)
            if mode in (0, 1):  # 0/JNI_COMMIT copy back
                arr.items[:] = e.read(p, len(arr.items))
            if mode in (0, 2):
                e.heap.free(p)
            return 0

        def get_region(self, e, size=size):
            env, h, start, n, buf = e.args(5)
            arr = self.obj(h)
            e.write(buf, bytes(arr.items[start * size:(start + n) * size]))
            return 0

        def set_region(self, e, size=size):
            env, h, start, n, buf = e.args(5)
            arr = self.obj(h)
            arr.items[start * size:(start + n) * size] = e.read(buf, n * size)
            return 0

        def jarray_len(self):
            return 0
        setattr(JNI, f"j_New{tname}Array", new)
        setattr(JNI, f"j_Get{tname}ArrayElements", get_elems)
        setattr(JNI, f"j_Release{tname}ArrayElements", release_elems)
        setattr(JNI, f"j_Get{tname}ArrayRegion", get_region)
        setattr(JNI, f"j_Set{tname}ArrayRegion", set_region)


_prim_array_funcs()


def _call_variants():
    for t in ("Object", "Boolean", "Byte", "Char", "Short", "Int", "Long", "Float", "Double", "Void"):
        for v in ("", "V", "A"):
            setattr(JNI, f"j_Call{t}Method{v}", lambda self, e, v=v: self._invoke(e, None, v, False))
            setattr(JNI, f"j_CallStatic{t}Method{v}", lambda self, e, v=v: self._invoke(e, None, v, True))
            setattr(JNI, f"j_CallNonvirtual{t}Method{v}", lambda self, e, v=v: self._invoke_nv(e, v))


def _invoke_nv(self, e, variant):
    # (env, obj, clazz, mid, ...): shift by one register
    env, obj, clazz, mid = e.args(4)
    cls, name, sig, _ = self.methods[mid]
    params, rtype = parse_sig(sig)
    if variant == "":
        args = self._read_args(e, params, Args(e, 4))
    elif variant == "V":
        args = self._read_args(e, params, Args(e, va_ptr=e.args(5)[4]))
    else:
        args = self._read_args(e, params, None, jvalues=e.args(5)[4])
    return self._ret_value(rtype, java_call(self, cls, name, sig, self.obj(obj), args))


JNI._invoke_nv = _invoke_nv
_call_variants()


# ======================================================================================== Java side
JAVA = {}       # "cls.name" or "cls.name(sig)" -> fn(jni, this, args)
STATIC_FIELDS = {}
unknown_calls = {}


def java(*keys):
    def deco(fn):
        for k in keys:
            JAVA[k] = fn
        return fn
    return deco


def short(v, n=160):
    s = repr(v)
    return s if len(s) <= n else s[:n] + f"...({len(s)} chars)"


def java_call(jni, cls, name, sig, this, args):
    fn = JAVA.get(f"{cls}.{name}{sig}") or JAVA.get(f"{cls}.{name}")
    if fn is None:
        params, rtype = parse_sig(sig)
        default = {"V": None, "Ljava/lang/String;": ""}.get(rtype, 0 if rtype in "ZBCSIJFD" else None)
        key = f"{cls}.{name}{sig}"
        if key not in unknown_calls:
            jni.emu.log(f"  ?? java {key} args={short(args)} -> default {default!r}")
        unknown_calls[key] = unknown_calls.get(key, 0) + 1
        return default
    r = fn(jni, this, args)
    jni.emu.log(f"  java {cls}.{name}{sig} {short(args)} -> {short(r)}")
    return r


def java_new(jni, cls, sig, args):
    jni.emu.log(f"  java new {cls}{sig} {args!r}")
    return JObject(cls, args=args)


def java_static_field(jni, cls, name, sig):
    v = STATIC_FIELDS.get(f"{cls}.{name}")
    jni.emu.log(f"  java static field {cls}.{name}:{sig} -> {v!r}")
    return v
