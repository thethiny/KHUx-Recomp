// Batch #10: SQEX categories (CategoryImpl: the sounds of one category, its volume fade and pause count),
// CategoryManager::Release, DriverConfig::GetCategoryParam, and the 3D listener (LayoutManager).
//
// CategoryImpl: vtable +0, a FixedList of sound handles +4 (pool: capacity +4, count +8, slots +0xC, free +0x10,
// used +0x14, tail +0x18; see w4_b2/SqexPool.hpp), volume DynamicValue +0x1C, index +0x30, max sounds +0x34,
// pause count +0x38, pause fade time +0x3C (float).
#include "../w4_b2/SqexPool.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t VT_CATEGORY_IMPL_GOT = 0xFD92FC, VT_CATEGORY_BASE_GOT = 0xFD9250;
constexpr uint32_t VT_CATEGORY_SLOT_GOT = 0xFD9300;
constexpr uint32_t CATEGORIES_GOT = 0xFD9308, CATEGORY_COUNT_GOT = 0xFD9304, CATEGORIES_READY_GOT = 0xFD928C;
constexpr uint32_t SOUND_CTOR = 0xCE1334, SOUND_STOP = 0xCE1368, SOUND_PAUSE_ON = 0xCE1390, SOUND_PAUSE_OFF = 0xCE13B8;
constexpr uint32_t DV_NEED_UPDATE = 0xCF4490, DV_UPDATE = 0xCF449C;
constexpr uint32_t CATEGORY_GET_VOLUME = 0xCE54B4;
constexpr uint32_t SOUND_MANAGER_GET_SOUND = 0xCDEDF8, SOUND_IMPL_SET_CATEGORY_VOLUME = 0xCDDF0C;
constexpr uint32_t S_INVALID_CATEGORY = 0xEEDCFC;   // "INVALID CATEGORY NUMBER [%d]"
constexpr uint32_t SOUND_IMPL_GET_STATE = 0xCDD970, SOUND_IMPL_GET_PRIORITY = 0xCDD9F0, SOUND_IMPL_STOP = 0xCDDA08,
                   SOUND_IMPL_PAUSE_ON = 0xCDDDD0;
constexpr uint32_t S_OVER_MAX_SOUNDS = 0xEEDC10;    // "CategoryImpl::AddSound category over maxsounds!! ..."
constexpr uint32_t S_APPEND_FAILED = 0xEEDC6C;      // "CategoryImpl::AddSound category list append failed!!"

constexpr uint32_t LAYOUT = 0x100E678;   // handedness flag +0; the listener matrix (4x4 floats) +4
constexpr uint32_t SINF = 0x41A9E0, COSF = 0x41C8AC, COS = 0x415A3C;   // (imports)

// each sound of the category gets `method(Sound(handle), ms)`, while the list holds used slots
void forEachSound(uint32_t category, uint32_t method, uint32_t ms) {
    guest::Frame f(8);
    for (uint32_t node = ld32(category + 0x14); node != 0; node = ld32(node + 8)) {
        rw::call(SOUND_CTOR, {f.addr(4), ld32(node + 0x14)});
        rw::call(method, {f.addr(4), ms});
        if (!(ld8(node + 0x10) & 1)) return;
    }
}

float callF(uint32_t fn, float v) { return bitsf(rw::call(fn, {fbits(v)})); }

// the rest of the listener matrix: rows 0-2 = right, up, front with the translation -dot(p, axis), row 3 = 0 0 0 1
void storeRow(uint32_t at, float x, float y, float z) {
    stf(LAYOUT + at, x);
    stf(LAYOUT + at + 4, y);
    stf(LAYOUT + at + 8, z);
}
void clearLastRow() {
    st32(LAYOUT + 0x34, 0);
    st32(LAYOUT + 0x38, 0);
    st32(LAYOUT + 0x3C, 0);
    st32(LAYOUT + 0x40, 0x3F800000);
}

}  // namespace

struct Category {
    static uint32_t dtor(uint32_t category) {
        st32(category, ld32(VT_CATEGORY_IMPL_GOT) + 8);
        if (ld32(category + 0xC) != 0) pool::destroySlots(category + 4);
        st32(category + 0xC, 0);
        st32(category + 0x10, 0);
        st32(category + 0x14, 0);
        st32(category, ld32(VT_CATEGORY_BASE_GOT) + 8);
        st32(category + 0x18, 0);
        st32(category + 8, 0);
        return category;
    }
    // a fresh list of `maxSounds` free slots
    static uint32_t initialize(uint32_t category, uint32_t index, uint32_t maxSounds) {
        const uint32_t list = category + 4;
        st32(category + 0x30, index);
        st32(category + 0x34, maxSounds);
        if (ld32(category + 0xC) != 0) pool::destroySlots(list);
        st32(category + 0xC, 0);
        st32(category + 0x10, 0);
        st32(category + 0x14, 0);
        st32(category + 0x18, 0);
        st32(category + 8, 0);
        st32(category + 4, maxSounds);
        const uint32_t block = malloc_(maxSounds <= 0x5500000u ? maxSounds * 24 + 8 : 0xFFFFFFFFu, 4);
        st32(block + 4, maxSounds);
        st32(block, 24);
        if (maxSounds != 0) pool::initSlots(block, maxSounds, VT_CATEGORY_SLOT_GOT);
        const uint32_t slots = block + 8;
        st32(category + 0xC, slots);
        st32(category + 0x10, slots);
        if (lds32(category + 4) > 0) pool::chain(slots, 0, lds32(category + 4));
        st32(category + 0x14, 0);
        st32(category + 0x18, 0);
        st32(category + 8, 0);
        return 0;
    }
    static uint32_t stop(uint32_t category, uint32_t ms) {
        forEachSound(category, SOUND_STOP, ms);
        return 0;
    }
    // the volume fade moves on; each sound gets the category volume, sounds gone from the SoundManager leave
    static uint32_t update(uint32_t category, uint32_t dtBits) {
        const float dt = bitsf(dtBits);
        if (rw::call(DV_NEED_UPDATE, {category + 0x1C}) != 0)
            rw::call(DV_UPDATE, {category + 0x1C, uint32_t(f2i(dt))});
        float pause = ldf(category + 0x3C) - dt;
        if (pause < 0.0f) pause = 0.0f;
        stf(category + 0x3C, pause);
        const uint32_t volume = rw::call(CATEGORY_GET_VOLUME, {category});
        for (uint32_t node = ld32(category + 0x14); node != 0;) {
            const uint32_t sound = rw::call(SOUND_MANAGER_GET_SOUND, {ld32(node + 0x14)});
            if (sound != 0) {
                rw::call(SOUND_IMPL_SET_CATEGORY_VOLUME, {sound, volume});
                if (!(ld8(node + 0x10) & 1)) break;
                node = ld32(node + 8);
            } else {
                const uint32_t next = ld32(node + 8);
                // (a node that is no longer a valid used slot is visited again, as the original does)
                if (pool::release(category + 4, lds32(node + 0xC))) node = next;
            }
        }
        return 0;
    }
    // the first pause pauses every sound (and keeps the fade time); later ones only count
    static uint32_t pauseOn(uint32_t category, uint32_t ms) {
        if (ld32(category + 0x38) == 0) {
            forEachSound(category, SOUND_PAUSE_ON, ms);
            stf(category + 0x3C, float(ms));
        }
        st32(category + 0x38, ld32(category + 0x38) + 1);
        return 0;
    }
    // the last resume resumes every sound
    static uint32_t pauseOff(uint32_t category, uint32_t ms) {
        if (ld32(category + 0x38) == 1) {
            if (ld32(category + 0x14) == 0) {
                st32(category + 0x38, 0);
                return 0;
            }
            forEachSound(category, SOUND_PAUSE_OFF, ms);
        }
        const int32_t count = lds32(category + 0x38) - 1;
        st32(category + 0x38, uint32_t(count < 0 ? 0 : count));
        return 0;
    }

    // adds a sound (handle) to the category; when it is full, the head entry is dropped if its sound is gone or
    // playing (state 3), else the lower-priority one of the new sound and the head's sound is stopped. The search
    // follows prev links from the list head, so it only ever looks at the head (sic). The sound gets the category's
    // volume, and its pause when the category is paused.
    static int32_t addSound(uint32_t category, uint32_t handle) {
        const uint32_t sound = rw::call(SOUND_MANAGER_GET_SOUND, {handle});
        if (sound == 0) return -1;
        const uint32_t list = category + 4;
        if (lds32(category + 8) >= lds32(category + 0x34)) {
            uint32_t node = ld32(category + 0x14);
            uint32_t lowest = 0;
            bool drop = true;
            while (node != 0) {
                const uint32_t s = rw::call(SOUND_MANAGER_GET_SOUND, {ld32(node + 0x14)});
                if (s == 0 || rw::call(SOUND_IMPL_GET_STATE, {s}) == 3) break;
                if (lowest == 0 ||
                    rw::call(SOUND_IMPL_GET_PRIORITY, {s}) < rw::call(SOUND_IMPL_GET_PRIORITY, {lowest}))
                    lowest = s;
                if (!(ld8(node + 0x10) & 1) || ld32(node + 4) == 0) {
                    drop = false;
                    break;
                }
                node = ld32(node + 4);
            }
            if (drop) {
                pool::release(list, lds32(ld32(category + 0x14) == 0 ? 0xC : node + 0xC));
            } else {
                if (rw::call(SOUND_IMPL_GET_PRIORITY, {sound}) >= rw::call(SOUND_IMPL_GET_PRIORITY, {lowest}))
                    rw::call(SOUND_IMPL_STOP, {lowest, 0});
                else
                    rw::call(SOUND_IMPL_STOP, {sound, 0});
                log(5, S_OVER_MAX_SOUNDS, ld32(category + 0x30));
            }
        }
        uint32_t free = ld32(list + pool::FREE);
        if (free == 0) {
            free = pool::grow(list, VT_CATEGORY_SLOT_GOT);
            if (free == 0) {
                log(5, S_APPEND_FAILED);
                return -1;
            }
        }
        pool::take(list, free, handle);
        rw::call(SOUND_IMPL_SET_CATEGORY_VOLUME, {sound, rw::call(CATEGORY_GET_VOLUME, {category})});
        if (lds32(category + 0x38) > 0) rw::call(SOUND_IMPL_PAUSE_ON, {sound, f2u(ldf(category + 0x3C))});
        return 0;
    }

    static uint32_t managerRelease() {
        const uint32_t categories = ld32(CATEGORIES_GOT), count = ld32(CATEGORY_COUNT_GOT);
        if (ld32(categories) == 0) return 0;
        for (int32_t k = 0; k < lds32(count); k++) {
            const uint32_t c = ld32(ld32(categories) + uint32_t(k) * 4);
            if (c != 0) vcallo(c, 4, {c});   // deleting destructor
        }
        free_(ld32(categories));
        st32(categories, 0);
        st32(count, 0);
        st8(ld32(CATEGORIES_READY_GOT), 0);
        return 0;
    }

    static uint32_t driverConfigGetCategoryParam(uint32_t config, int32_t index) {
        if (lds32(config + 0x20) <= index) log(5, S_INVALID_CATEGORY, uint32_t(index));
        return config + uint32_t(index) * 4;
    }
};
RW_FUNCTION(0xCE51C8, "SQEX::CategoryImpl::~CategoryImpl", Category::dtor);
RW_FUNCTION(0xCE52D8, "SQEX::CategoryImpl::Initialize", Category::initialize);
RW_FUNCTION(0xCE5444, "SQEX::CategoryImpl::Stop", Category::stop);
RW_FUNCTION(0xCE54BC, "SQEX::CategoryImpl::Update", Category::update);
RW_FUNCTION(0xCE5628, "SQEX::CategoryImpl::AddSound", Category::addSound);
RW_FUNCTION(0xCE5AB8, "SQEX::CategoryImpl::PauseOn", Category::pauseOn);
RW_FUNCTION(0xCE5B38, "SQEX::CategoryImpl::PauseOff", Category::pauseOff);
RW_FUNCTION(0xCE5CB0, "SQEX::CategoryManager::Release", Category::managerRelease);
RW_FUNCTION(0xCE608C, "SQEX::DriverConfig::GetCategoryParam", Category::driverConfigGetCategoryParam);

struct Layout {
    // the listener from its position, front and up vectors: front normalized (flipped when the flag at +0 is 0),
    // right = normalize(up x front) and up' = front x right (the products are rounded one by one, as VFP does)
    static uint32_t setListener(uint32_t pxB, uint32_t pyB, uint32_t pzB, uint32_t fxB, uint32_t fyB, uint32_t fzB,
                                uint32_t uxB, uint32_t uyB, uint32_t uzB) {
        const float px = bitsf(pxB), py = bitsf(pyB), pz = bitsf(pzB);
        float fx = bitsf(fxB), fy = bitsf(fyB), fz = bitsf(fzB);
        const float ux = bitsf(uxB), uy = bitsf(uyB), uz = bitsf(uzB);
        if (ld32(LAYOUT) == 0) {
            fy = -fy;
            fx = -fx;
            fz = -fz;
        }
        float sq = fy * fy;
        sq = sq + fx * fx;
        sq = sq + fz * fz;
        const float len = std::sqrt(sq);
        if (len > 0.0f || len != len) {
            fz = fz / len;
            fx = fx / len;
            fy = fy / len;
        } else {
            fz = fy = fx = 0.0f;
        }
        float ry = uz * fx - ux * fz;
        float rx = uy * fz - uz * fy;
        float rz = ux * fy - uy * fx;
        float sq2 = ry * ry;
        sq2 = sq2 + rx * rx;
        sq2 = sq2 + rz * rz;
        const float len2 = std::sqrt(sq2);
        storeRow(0x24, fx, fy, fz);
        clearLastRow();
        if (len2 > 0.0f || len2 != len2) {
            rx = rx / len2;
            ry = ry / len2;
            rz = rz / len2;
        } else {
            rz = ry = rx = 0.0f;
        }
        storeRow(4, rx, ry, rz);
        const float vy = rx * fz - rz * fx;
        const float vx = rz * fy - ry * fz;
        const float vz = ry * fx - rx * fy;
        storeRow(0x14, vx, vy, vz);
        float tr = py * ry;
        tr = tr + px * rx;
        tr = tr + pz * rz;
        float tv = py * vy;
        tv = tv + px * vx;
        tv = tv + pz * vz;
        float tf = py * fy;
        tf = tf + px * fx;
        tf = tf + pz * fz;
        stf(LAYOUT + 0x10, -tr);
        stf(LAYOUT + 0x30, -tf);
        stf(LAYOUT + 0x20, -tv);
        return 0;
    }
    // the listener from its position and three rotation angles a, b, c (sinf / cosf; c's cosine in double)
    static uint32_t setListenerRotation(uint32_t pxB, uint32_t pyB, uint32_t pzB, uint32_t aB, uint32_t bB,
                                        uint32_t cB) {
        const float px = bitsf(pxB), py = bitsf(pyB), pz = bitsf(pzB);
        const float sa = callF(SINF, bitsf(aB)), sb = callF(SINF, bitsf(bB)), sc = callF(SINF, bitsf(cB));
        const float ca = callF(COSF, bitsf(aB)), cb = callF(COSF, bitsf(bB));
        const double cDouble = double(bitsf(cB));
        uint64_t cBits;
        std::memcpy(&cBits, &cDouble, 8);
        const uint64_t ccBits = rw::call64(COS, {uint32_t(cBits), uint32_t(cBits >> 32)});
        double ccDouble;
        std::memcpy(&ccDouble, &ccBits, 8);
        const float cc = float(ccDouble);
        clearLastRow();
        const float sasb = sa * sb, sbca = sb * ca;
        float m14 = -(sc * ca);
        float m18 = ca * cc;
        float m28 = -(sa * cc);
        m18 = m18 + sc * sasb;
        float m24 = sa * sc;
        stf(LAYOUT + 0x18, m18);
        m28 = m28 + sc * sbca;
        const float m08 = sc * cb;
        stf(LAYOUT + 0x28, m28);
        m14 = m14 + cc * sasb;
        stf(LAYOUT + 8, m08);
        m24 = m24 + cc * sbca;
        stf(LAYOUT + 0x14, m14);
        const float m04 = cb * cc;
        stf(LAYOUT + 0x24, m24);
        float tv = py * m18;
        stf(LAYOUT + 4, m04);
        float tr = py * m08;
        float tf = py * m28;
        tv = tv + px * m14;
        tr = tr + px * m04;
        tf = tf + px * m24;
        const float m2c = ca * cb, m1c = sa * cb;
        stf(LAYOUT + 0x2C, m2c);
        const float m0c = -sb;
        stf(LAYOUT + 0x1C, m1c);
        tr = tr + pz * m0c;
        stf(LAYOUT + 0xC, m0c);
        tv = tv + pz * m1c;
        tf = tf + pz * m2c;
        stf(LAYOUT + 0x10, -tr);
        stf(LAYOUT + 0x20, -tv);
        stf(LAYOUT + 0x30, -tf);
        return 0;
    }
    static uint32_t getListenerMatrix(uint32_t out) {
        for (uint32_t k = 0; k < 16; k++) st32(out + k * 4, ld32(LAYOUT + 4 + k * 4));
        return 0;
    }
};
RW_FUNCTION(0xCE6104, "SQEX::LayoutManager::SetListener (position, front, up)", Layout::setListener);
RW_FUNCTION(0xCE62AC, "SQEX::LayoutManager::SetListener (position, rotation)", Layout::setListenerRotation);
RW_FUNCTION(0xCE63E8, "SQEX::LayoutManager::GetListenerMatrix", Layout::getListenerMatrix);

}  // namespace sqex
