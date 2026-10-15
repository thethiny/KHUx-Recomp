// DisplayDamageNumber: the helper that creates a DamageNumber and places it on a node.
#pragma once

#include "DamageNumber.hpp"

struct DisplayDamageNumber {
    // Show `value` on `parent` at pos (a cocos2d::Vec2 passed by reference) raised by offsetY. The number's type
    // is the first set flag of (flag1, flag2, flag3) -> 1, 2, 3 (else 0); numberScale is the DamageNumber scale
    // (0 = 0.8, 1 = 1.0), then the node is scaled by `scale` and added to parent at zOrder.
    static uint32_t disp_dmgNum(int32_t value, uint32_t parent, const float *pos, float offsetY, uint32_t flag1,
                                uint32_t flag2, uint32_t flag3, uint32_t numberScale, int32_t zOrder, float scale);
};
