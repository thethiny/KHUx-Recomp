// AvatarUtil: the player's avatar style (a SafeBuffer of 0x30 bytes: six part ids, keyblade id / 100 and % 100,
// five accessory ids) from the user data, the support user, or the defaults.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace w4b2 {
using namespace scene;

constexpr uint32_t SB_CTOR = 0x459F21;       // SafeBuffer::SafeBuffer(size, bool)
constexpr uint32_t SB_WRITE = 0x459FA1;      // SafeBuffer::write(dst, src, n, index)
constexpr uint32_t SB_READ = 0x45A269;       // SafeBuffer::read(dst, src, n, index)
constexpr uint32_t SB_DTOR = 0x45A13D;
constexpr uint32_t SB_MOVE_CTOR = 0x45A319;  // SafeBuffer::SafeBuffer(SafeBuffer&&)
constexpr uint32_t SB_MOVE_ASSIGN = 0x45A355;
constexpr uint32_t DEFAULT_STYLE = 0x497595;  // sub_497594: the default style
constexpr uint32_t API_MANAGER = 0x4BA8C9;
constexpr uint32_t STYLE_VTABLE_GOT = 0xFD6138;

static uint32_t sbData(uint32_t sb) { return ld32(sb + 0xC); }
static void sbWrite(uint32_t sb, uint32_t off, uint32_t src, uint32_t n) {
    call(SB_WRITE, {sb, sbData(sb) + off, src, n, off});
}
static void sbRead(uint32_t sb, uint32_t dst, uint32_t off, uint32_t n) {
    call(SB_READ, {sb, dst, sbData(sb) + off, n, off});
}

struct AvatarUtil {
    // set<int> lower_bound(v) == v (the compiled inline search)
    static bool setContains(uint32_t set, int32_t v) {
        uint32_t end = set + 4;
        uint32_t node = ld32(end), cand = end;
        while (node != 0) {
            if (rw::load<int32_t>(node + 0x10) >= v) {
                cand = node;
                node = ld32(node);
            } else {
                node = ld32(node + 4);
            }
        }
        return cand != end && !(v < rw::load<int32_t>(cand + 0x10));
    }

    static uint32_t makeStyle(uint32_t out, uint32_t coord, int32_t keyblade) {
        Frame f;
        call(SB_CTOR, {out, 0x30, 1});
        st32(out, ld32(STYLE_VTABLE_GOT) + 8);
        sbWrite(out, 0, coord + 8, 4);
        sbWrite(out, 4, coord + 0xC, 4);
        sbWrite(out, 8, coord + 0x10, 4);
        sbWrite(out, 0xC, coord + 0x14, 4);
        sbWrite(out, 0x10, coord + 0x18, 4);
        sbWrite(out, 0x1C, coord + 0x1C, 0x14);
        uint32_t kb = f.alloc(8);
        call(0x783F9D, {kb, uint32_t(keyblade)});  // master::Keyblade::create -> shared_ptr
        uint32_t tmp = f.alloc(8);
        if (ld32(kb) != 0) {
            uint32_t v = f.alloc(8);
            sbRead(ld32(kb), v, 0xCC, 4);  // the keyblade's style number
            int32_t n = rw::load<int32_t>(v);
            st32(tmp + 4, call(0xCD5E8D, {uint32_t(n), 100}));  // __divsi3
            sbWrite(out, 0x14, tmp + 4, 4);
            st32(tmp, call(0xCD6009, {uint32_t(n), 100}));  // __modsi3
            sbWrite(out, 0x18, tmp, 4);
        } else {
            st32(tmp + 4, 0);
            sbWrite(out, 0x14, tmp + 4, 4);
            st32(tmp, 0);
            sbWrite(out, 0x18, tmp, 4);
        }
        // every part id must be a valid avatar part, else the default style
        uint32_t set = f.alloc(12);
        call(0x76D091, {set, call(0x76C091, {}), 2});  // MasterDataManager::getIDs(avatarParts)
        uint32_t ids = f.alloc(0x14);
        bool ok = true;
        const uint32_t order[5] = {0, 4, 8, 0xC, 0x10};
        for (uint32_t k = 0; k < 5 && ok; k++) {
            uint32_t v = ids + k * 4;
            sbRead(out, v, order[k], 4);
            if (ld32(set + 4) == 0) {
                call(0x48D9B5, {set, 0});  // __tree::destroy
                ok = false;
                goto fallback;
            }
            if (!setContains(set, rw::load<int32_t>(v))) {
                call(0x48D9B5, {set, ld32(set + 4)});
                ok = false;
                goto fallback;
            }
        }
        {
            uint32_t acc = f.alloc(0x14);
            sbRead(out, acc, 0x1C, 0x14);
            for (uint32_t k = 0; k < 0x14; k += 4) {
                if (ld32(acc + k) != 0 && call(0x497AA3, {set, acc + k}) == set + 4) {  // set.find == end
                    call(0x48D9B5, {set, ld32(set + 4)});
                    goto fallback;
                }
            }
            call(0x48D9B5, {set, ld32(set + 4)});
            goto done;
        }
    fallback: {
        uint32_t def = f.alloc(0x20);
        call(DEFAULT_STYLE, {def});
        call(SB_MOVE_ASSIGN, {out, def});
        call(SB_DTOR, {def});
        if (keyblade == 0) {
            uint32_t z = f.alloc(8);
            st32(z + 4, 0);
            sbWrite(out, 0x14, z + 4, 4);
            st32(z, 0);
            sbWrite(out, 0x18, z, 4);
        }
    }
    done:
        if (ld32(kb + 4) != 0) call(fn::shared_release, {ld32(kb + 4)});
        return 0;
    }

    // the 0x58-byte UserKeyblade record of the equipped keyblade
    static uint32_t getCurrentKeyblade(uint32_t out) {
        Frame f;
        uint32_t am = call(API_MANAGER, {});
        uint32_t am2 = call(API_MANAGER, {});
        uint32_t table = ld32(am + 0xEC);
        uint32_t key = f.alloc(8);
        call(SB_READ, {am2 + 0x33C, key, ld32(am2 + 0x348) + 0x10, 8, 0x10});  // the equipped keyblade's uid
        uint32_t it = call(0x490275, {table + 0x24, key});  // unordered_map<u64,int>::find
        int32_t idx = it != 0 ? rw::load<int32_t>(it + 0x10) : -1;
        uint32_t off = idx >= 0 ? uint32_t(idx) * 0x58 : 0;
        uint32_t t = ld32(am + 0xEC);
        call(SB_READ, {t, out, ld32(t + 0xC) + off, 0x58, off});
        return 0;
    }

    static uint32_t fromBuffer(uint32_t out, uint32_t tmp) {
        call(SB_MOVE_CTOR, {out, tmp});
        st32(out, ld32(STYLE_VTABLE_GOT) + 8);
        return call(SB_DTOR, {tmp});
    }

    static uint32_t getCurrentStyle(uint32_t out, uint32_t avatarOnly) {
        Frame f;
        uint32_t b = f.alloc(8);
        uint32_t flags = ld32(call(API_MANAGER, {}) + 0x4C);
        sbRead(flags, b, 1, 1);
        uint32_t useAvatar = ld8(b);
        uint32_t am = call(API_MANAGER, {});
        if (useAvatar != 0) {
            uint32_t user = ld32(am + 0x54);
            if (user != 0) {
                makeStyle(out, user + 0x1C, 0);
                return 0;
            }
        } else if (ld32(am + 0xC4) != 0) {
            uint32_t status = ld32(call(API_MANAGER, {}) + 0x94);
            if (status != 0) {
                sbRead(status, b, 0x3C, 4);  // the selected avatar id
                uint32_t table = ld32(am + 0xC4);
                uint32_t it = call(0x497A21, {table + 0x24, b});  // unordered_map<int,int>::find
                if (it != 0 && rw::load<int32_t>(it + 0xC) >= 0) {
                    uint32_t off = ld32(it + 0xC) * 0x30;
                    uint32_t coord = f.alloc(0x30);
                    call(SB_READ, {table, coord, ld32(table + 0xC) + off, 0x30, off});
                    if (avatarOnly == 1) {
                        makeStyle(out, coord, 0);
                        return 0;
                    }
                    uint32_t kb = f.alloc(0x58);
                    getCurrentKeyblade(kb);
                    uint32_t tmp = f.alloc(0x20);
                    makeStyle(tmp, coord, ld32(kb + 0xC));
                    fromBuffer(out, tmp);
                    return 0;
                }
            }
        }
        uint32_t def = f.alloc(0x20);
        call(DEFAULT_STYLE, {def});
        if (avatarOnly != 0) {
            uint32_t z = f.alloc(8);
            st32(z + 4, 0);
            call(SB_WRITE, {def, ld32(def + 0xC) + 0x14, z + 4, 4, 0x14});
            st32(z, 0);
            call(SB_WRITE, {def, ld32(def + 0xC) + 0x18, z, 4, 0x18});
        }
        fromBuffer(out, def);
        return 0;
    }

    static uint32_t getSupportUserStyle(uint32_t out) {
        Frame f;
        uint32_t am = call(API_MANAGER, {});
        uint32_t slot = am + 0x134;
        if (ld32(slot) != 0) {
            uint32_t am2 = call(API_MANAGER, {});
            uint32_t table = ld32(slot);
            uint32_t rec = f.alloc(0x128);
            call(SB_READ, {am2 + 0x33C, rec, ld32(am2 + 0x348) + 8, 8, 8});  // the support user's uid
            uint32_t it = call(0x490275, {table + 0x24, rec});
            if (it != 0 && rw::load<int32_t>(it + 0x10) >= 0) {
                uint32_t t = ld32(slot);
                st8(rec + 0x11F, 0);
                uint32_t off = ld32(it + 0x10) * 0x128;
                call(SB_READ, {t, rec, ld32(t + 0xC) + off, 0x128, off});
                makeStyle(out, rec + 0xE8, ld32(rec + 0x40));
                return 0;
            }
        }
        call(DEFAULT_STYLE, {out});
        return 0;
    }
};
RW_FUNCTION(0x497104, "AvatarUtil::makeStyle", AvatarUtil::makeStyle);
RW_FUNCTION(0x4976FC, "AvatarUtil::getCurrentKeyblade", AvatarUtil::getCurrentKeyblade);
RW_FUNCTION(0x4977AC, "AvatarUtil::getCurrentStyle", AvatarUtil::getCurrentStyle);
RW_FUNCTION(0x49794C, "AvatarUtil::getSupportUserStyle", AvatarUtil::getSupportUserStyle);

}  // namespace w4b2
