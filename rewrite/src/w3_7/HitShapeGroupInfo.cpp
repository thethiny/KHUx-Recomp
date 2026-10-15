// HitShapeGroupInfo::isHitTest: does one corner of a rect hit any shape of the group?
#include "core/Guest.hpp"

namespace {
constexpr uint32_t VEC2_COPY = 0x8E4C03;        // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;        // cocos2d::Vec2::~Vec2()
constexpr uint32_t VEC2_SET = 0x8E530B;         // cocos2d::Vec2::set(float, float)
constexpr uint32_t RECT_GET_MIN_X = 0x8DF275;   // cocos2d::Rect::getMinX() const
constexpr uint32_t RECT_GET_MAX_X = 0x8DF249;
constexpr uint32_t RECT_GET_MIN_Y = 0x8DF2A5;
constexpr uint32_t RECT_GET_MAX_Y = 0x8DF279;
constexpr uint32_t HIT_SHAPE_IS_INTERSECTS = 0x758009;   // HitShapeInfo::isIntersects(Vec2&, Rect&) const
constexpr uint32_t VEC2_ZERO_GOT = 0xFD605C;    // GOT slot: &cocos2d::Vec2::ZERO
constexpr uint32_t HIT_SHAPE_INFO_SIZE = 20;

struct HitShapeGroupInfo {
    rw::Field<uint32_t> shapes;   // +0  HitShapeInfo[count]
    rw::Field<uint32_t> count;    // +4
    rw::Field<uint32_t> field_8;  // +8
    rw::Field<int32_t> corner;    // +12 which corner of the rect is tested: 0 min/min, 1 max/min, 2 min/max, 3 max/max
};
}  // namespace

// The test point is the chosen corner of `rect`; it hits when any shape intersects it with `area`.
static uint32_t HitShapeGroupInfo_isHitTest(const HitShapeGroupInfo *self, uint32_t area, uint32_t rect) {
    guest::Frame f(0x1C);
    const uint32_t point = f.addr(0x10);
    rw::call(VEC2_COPY, {point, rw::load<uint32_t>(VEC2_ZERO_GOT)});
    uint32_t hit = 0;
    int32_t corner = self->corner;
    if (uint32_t(corner) <= 3) {
        uint32_t x = rw::call((corner & 1) ? RECT_GET_MAX_X : RECT_GET_MIN_X, {rect});
        uint32_t y = rw::call((corner & 2) ? RECT_GET_MAX_Y : RECT_GET_MIN_Y, {rect});
        rw::call(VEC2_SET, {point, x, y});
        for (uint32_t k = 0, offset = 0; k < self->count; offset += HIT_SHAPE_INFO_SIZE) {
            k++;
            hit = 1;
            if (rw::call(HIT_SHAPE_IS_INTERSECTS, {self->shapes + offset, point, area}) == 1) break;
            hit = 0;
        }
    }
    rw::call(VEC2_DTOR, {point});
    return hit;
}
RW_FUNCTION(0x757D20, "HitShapeGroupInfo::isHitTest(cocos2d::Rect&, cocos2d::Rect&) const", HitShapeGroupInfo_isHitTest);
