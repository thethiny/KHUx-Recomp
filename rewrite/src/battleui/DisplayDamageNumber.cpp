// DisplayDamageNumber: create a DamageNumber and add it to a node.
#include "DisplayDamageNumber.hpp"

using namespace scene;

uint32_t DisplayDamageNumber::disp_dmgNum(int32_t value, uint32_t parent, const float *pos, float offsetY,
                                          uint32_t flag1, uint32_t flag2, uint32_t flag3, uint32_t numberScale,
                                          int32_t zOrder, float scale) {
    visibleSize();       // unused (the original asks the director for it)
    uint32_t type = flag1 ? 1 : flag2 ? 2 : flag3 ? 3 : 0;
    uint32_t number = rw::guest_addr(DamageNumber::create(value, type, numberScale));
    {
        Frame f;
        vcall(number, node::setPosition, {number, newVec2(f, pos[0], pos[1] + offsetY)});
    }
    vcall(number, node::setScale, {number, scene::fbits(scale)});
    vcall(parent, node::addChildZ, {parent, number, uint32_t(zOrder)});
    return 0;
}
RW_FUNCTION(0x5EB45C, "DisplayDamageNumber::disp_dmgNum", DisplayDamageNumber::disp_dmgNum);
