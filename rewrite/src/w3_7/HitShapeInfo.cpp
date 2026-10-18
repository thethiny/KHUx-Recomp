// HitShapeInfo: one hit shape of an actor (a rectangle or a circle, offset from the actor's position) and its
// overlap test against a rectangle.
#include "scene/SceneCommon.hpp"

using namespace scene;

namespace {
// HitShapeInfo: type +0 (1 rectangle, 2 circle), offset x +4 / y +8, then width +0xC / height +0x10 or radius +0xC
constexpr uint32_t H_TYPE = 0, H_X = 4, H_Y = 8, H_WIDTH = 0xC, H_HEIGHT = 0x10, H_RADIUS = 0xC;
constexpr uint32_t RECTANGLE = 1, CIRCLE = 2;
// cocos2d::Rect: origin x, y, size width +8, height +0xC
constexpr uint32_t R_WIDTH = 8, R_HEIGHT = 0xC;

constexpr uint32_t Rect_ctor = 0x8DF18B, Rect_getMinX = 0x8DF275, Rect_getMinY = 0x8DF2A5;
constexpr uint32_t Rect_getMaxX = 0x8DF249, Rect_getMaxY = 0x8DF279;
constexpr uint32_t Rect_containsPoint = 0x8DF2A9, Rect_intersectsRect = 0x8DF375;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17, Vec2_distanceSquared = 0x8E4F45;

float callFloat(uint32_t fn, uint32_t arg) { return bitsf(call(fn, {arg})); }

// |corner - center|^2 <= radius^2 (false for NaN, as __aeabi_fcmple)
uint32_t cornerInside(Frame &f, uint32_t center, float x, float y, float radiusSquared) {
    const uint32_t corner = f.alloc(8);
    call(Vec2_ctor, {corner, fbits(x), fbits(y)});
    const float d = bitsf(call(Vec2_distanceSquared, {center, corner}));
    call(Vec2_dtor, {corner});
    return d <= radiusSquared ? 1 : 0;
}
}  // namespace

namespace hit_shape {
// a circle overlaps the rectangle when its centre lies in the rectangle grown by the radius horizontally or
// vertically, or within the radius of a corner
uint32_t isIntersects(const uint8_t *selfPtr, uint32_t position, uint32_t rect) {
    const uint32_t self = rw::guest_addr(selfPtr);
    const uint32_t type = ld32(self + H_TYPE);
    if (type == CIRCLE) {
        Frame f;
        const uint32_t center = f.alloc(8);
        const float cx = ldf(position) + ldf(self + H_X);
        call(Vec2_ctor, {center, fbits(cx), fbits(ldf(position + 4) + ldf(self + H_Y))});
        const float radiusSquared = ldf(self + H_RADIUS) * ldf(self + H_RADIUS);

        const float minX = callFloat(Rect_getMinX, rect);
        const float radius = ldf(self + H_RADIUS);
        const float minY = callFloat(Rect_getMinY, rect);
        const uint32_t wide = f.alloc(16);
        const float wideWidth = ldf(rect + R_WIDTH) + (ldf(self + H_RADIUS) + ldf(self + H_RADIUS));
        call(Rect_ctor, {wide, fbits(minX - radius), fbits(minY), fbits(wideWidth), ld32(rect + R_HEIGHT)});
        const float minX2 = callFloat(Rect_getMinX, rect);
        const float minY2 = callFloat(Rect_getMinY, rect);
        const float r = ldf(self + H_RADIUS);
        const float tallHeight = (r + r) + ldf(rect + R_HEIGHT);
        const uint32_t tall = f.alloc(16);
        call(Rect_ctor, {tall, fbits(minX2), fbits(minY2 - r), ld32(rect + R_WIDTH), fbits(tallHeight)});
        uint32_t hit = call(Rect_containsPoint, {wide, center});
        hit |= call(Rect_containsPoint, {tall, center});

        const uint32_t corners[3][2] = {{Rect_getMinX, Rect_getMinY}, {Rect_getMaxX, Rect_getMinY},
                                        {Rect_getMinX, Rect_getMaxY}};
        for (const auto &c : corners) {
            const float x = callFloat(c[0], rect);
            const float y = callFloat(c[1], rect);
            hit |= cornerInside(f, center, x, y, radiusSquared);
        }
        // the last corner: its distance is compared after the other temporaries are gone
        const uint32_t corner = f.alloc(8);
        const float maxX = callFloat(Rect_getMaxX, rect);
        const float maxY = callFloat(Rect_getMaxY, rect);
        call(Vec2_ctor, {corner, fbits(maxX), fbits(maxY)});
        const float d = bitsf(call(Vec2_distanceSquared, {center, corner}));
        call(Vec2_dtor, {corner});
        call(Vec2_dtor, {tall});
        call(Vec2_dtor, {wide});
        call(Vec2_dtor, {center});
        return hit | (d <= radiusSquared ? 1 : 0);
    }
    if (type == RECTANGLE) {
        Frame f;
        const uint32_t box = f.alloc(16);
        const float x = ldf(position) + ldf(self + H_X), y = ldf(position + 4) + ldf(self + H_Y);
        call(Rect_ctor, {box, fbits(x), fbits(y), ld32(self + H_WIDTH), ld32(self + H_HEIGHT)});
        const uint32_t hit = call(Rect_intersectsRect, {box, rect});
        call(Vec2_dtor, {box});
        return hit;
    }
    return 0;
}
}  // namespace hit_shape
RW_FUNCTION(0x758008, "HitShapeInfo::isIntersects", hit_shape::isIntersects);
