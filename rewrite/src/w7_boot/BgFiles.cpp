// bg (the game's file layer): the archive list (vfs) - reload one archive, open a file from the first archive that
// has it (or by index) -, AssetInputStream::size, Archive's destructor, and the length-prefixed string reader
// with the two libc++ string helpers it instantiates (push_back, __grow_by).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t vfs_init = 0x8DD1A5;                    // the archive list's one-time initialisation
constexpr uint32_t G_VFS = 0x10068A0;                      // the list: sentinel +4 (first node +8), mutex +0x10
constexpr uint32_t mutex_lock = 0xC8DF55, mutex_unlock = 0xC8DF79;
constexpr uint32_t Archive_reload = 0x8DD831, Archive_find = 0x8DE18D;   // find: the name hash map at +0x68
constexpr uint32_t Archive_entryByName = 0x8DE2DD, Archive_entryByIndex = 0x8DE235;
constexpr uint32_t function_vp_copy = 0x491797;            // std::function<void(void*)>(const&)
constexpr uint32_t InputStreamFactory_create = 0x8DC5F1;
constexpr uint32_t string_push_back = 0x4D90F1, string_grow_by = 0x4D9165;
constexpr uint32_t throw_length_error = 0x47ADED, libc_memcpy = 0x40E164;
constexpr uint32_t V_read = 8, V_tell = 0xC, V_seek = 0x10;

void strClear(uint32_t s) {
    if (ld8(s) & 1) {
        st8(ld32(s + 8), 0);
        st32(s + 4, 0);
    } else {
        st8(s + 1, 0);
        st8(s, 0);
    }
}
// an entry (shared_ptr {entry, ctrl}: data +4, size +8, deleter +0x10) opened as a stream into out
void openEntry(uint32_t out, uint32_t entry) {
    Frame f;
    const uint32_t del = f.alloc(20);
    call(function_vp_copy, {del, entry + 0x10});
    call(InputStreamFactory_create, {out, ld32(entry + 4), ld32(entry + 8), del});
    functionDestroy(del);
}
}  // namespace

namespace bg_files {

// a std::string written as a 4-byte length and its bytes; false (string emptied) on a short read
uint32_t readString(uint32_t s, uint32_t stream) {
    strClear(s);
    Frame f;
    const uint32_t len = f.alloc(4), ch = f.alloc(4);
    if (vcall(stream, V_read, {stream, len, 4, 1}) != 1) return 0;
    for (uint32_t i = 0; i < ld32(len); i++) {
        if (vcall(stream, V_read, {stream, ch, 1, 1}) != 1) {
            strClear(s);
            return 0;
        }
        call(string_push_back, {s, uint32_t(int32_t(int8_t(ld8(ch))))});
    }
    return 1;
}
RW_FUNCTION(0x4D9018, "bg::ioutil::_read<std::string>", bg_files::readString);

// std::string::push_back
uint32_t pushBack(uint32_t s, uint32_t c) {
    const bool isLong = ld8(s) & 1;
    uint32_t size = isLong ? ld32(s + 4) : uint32_t(ld8(s) >> 1);
    const uint32_t cap = isLong ? (ld32(s) & ~1u) - 1 : 10;
    if (size == cap) call(string_grow_by, {s, cap, 1, cap, cap, 0, 0});
    uint32_t p;
    if (ld8(s) & 1) {
        p = ld32(s + 8);
        st32(s + 4, size + 1);
    } else {
        st8(s, 2 * size + 2);
        p = s + 1;
    }
    st8(p + size, c);
    st8(p + size + 1, 0);
    return p + size + 1;
}
RW_FUNCTION(0x4D90F0, "std::string::push_back (bg instance, sub_4D90F0)", bg_files::pushBack);

// std::string::__grow_by(old_cap, delta, old_size, n_copy, n_del, n_add)
uint32_t growBy(uint32_t s, uint32_t oldCap, uint32_t delta, uint32_t oldSize, uint32_t nCopy, uint32_t nDel,
                uint32_t nAdd) {
    if (0xFFFFFFEFu - oldCap < delta) call(throw_length_error, {s});
    const uint32_t old = (ld8(s) & 1) ? ld32(s + 8) : s + 1;
    uint32_t cap = 0xFFFFFFEF;
    if (oldCap <= 0x7FFFFFE6) {
        uint32_t want = delta + oldCap;
        if (want < 2 * oldCap) want = 2 * oldCap;
        cap = want < 11 ? 11 : (want + 16) & ~15u;
    }
    const uint32_t p = call(fn::operator_new, {cap});
    if (nCopy) call(libc_memcpy, {p, old, nCopy});
    if (oldSize - nDel != nCopy) call(libc_memcpy, {p + nAdd + nCopy, old + nDel + nCopy, oldSize - nDel - nCopy});
    if (oldCap != 10) call(fn::operator_delete, {old});
    st32(s + 8, p);
    st32(s, cap | 1);
    return s;
}
RW_FUNCTION(0x4D9164, "std::string::__grow_by (bg instance, sub_4D9164)", bg_files::growBy);

uint32_t assetSize(uint32_t self) {
    const uint32_t pos = vcall(self, V_tell, {self});
    vcall(self, V_seek, {self, 0, 2});
    const uint32_t size = vcall(self, V_tell, {self});
    vcall(self, V_seek, {self, pos, 0});
    return size;
}
RW_FUNCTION(0x8DC0C2, "bg::AssetInputStream::size", bg_files::assetSize);

uint32_t archiveDtor(uint32_t self) {
    functionDestroy(self + 0x18);
    functionDestroy(self);
    return self;
}
RW_FUNCTION(0x8DC98C, "bg::Archive::~Archive", bg_files::archiveDtor);

// every archive with this id reloaded
uint32_t reload(uint32_t id) {
    call(vfs_init, {});
    call(mutex_lock, {G_VFS + 0x10});
    for (uint32_t node = ld32(G_VFS + 8); node != G_VFS + 4; node = ld32(node + 4)) {
        const uint32_t arch = ld32(node + 8);
        if (ld32(arch + 4) == id) call(Archive_reload, {arch, ld32(arch + 4)});
    }
    return call(mutex_unlock, {G_VFS + 0x10});
}
RW_FUNCTION(0x8DD144, "bg::vfs::reload", bg_files::reload);

// a stream (shared_ptr into out) for `name` from the first archive holding it; a negative index left by an
// archive's lookup counts down through the next archives' entries instead; {0, 0} when none has it
uint32_t openInputStream(uint32_t out, uint32_t name) {
    call(vfs_init, {});
    call(mutex_lock, {G_VFS + 0x10});
    Frame f;
    const uint32_t index = f.alloc(4), key = f.alloc(4), res = f.alloc(8), tmp = f.alloc(8);
    st32(index, 0);
    for (uint32_t node = ld32(G_VFS + 8); node != G_VFS + 4; node = ld32(node + 4)) {
        const uint32_t arch = ld32(node + 8);
        if (int32_t(ld32(index)) < 0) {
            const int32_t left = int32_t(ld32(arch + 0x4C) + ld32(index));
            st32(index, uint32_t(left));
            if (left < 0) continue;
            call(Archive_entryByIndex, {res, arch});
            if (ld32(res)) openEntry(out, ld32(res));
            else {
                st32(out, 0);
                st32(out + 4, 0);
            }
            if (ld32(res + 4)) call(fn::shared_release, {ld32(res + 4)});
            call(mutex_unlock, {G_VFS + 0x10});
            return 0;
        }
        st32(key, name);
        if (!call(Archive_find, {arch + 0x68, key})) continue;
        call(Archive_entryByName, {res, arch, name, index});
        if (ld32(res)) openEntry(tmp, ld32(res));
        else {
            st32(tmp, 0);
            st32(tmp + 4, 0);
        }
        if (ld32(res + 4)) call(fn::shared_release, {ld32(res + 4)});
        if (ld32(tmp)) {
            st32(out, ld32(tmp));
            st32(out + 4, ld32(tmp + 4));
            st32(tmp, 0);
            st32(tmp + 4, 0);
            call(mutex_unlock, {G_VFS + 0x10});
            return 0;
        }
        if (ld32(tmp + 4)) call(fn::shared_release, {ld32(tmp + 4)});
    }
    st32(out, 0);
    st32(out + 4, 0);
    call(mutex_unlock, {G_VFS + 0x10});
    return 0;
}
RW_FUNCTION(0x8DD2B4, "bg::vfs::openInputStream", bg_files::openInputStream);

}  // namespace bg_files
