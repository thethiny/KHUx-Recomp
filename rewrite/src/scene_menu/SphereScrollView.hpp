// SphereScrollView: the Avatar Board list of SceneSphereBoardSelect, a BaseEndlessrollView wheel of nine board
// icons 45 degrees apart. changeRow fills an icon from its master::Sphere (incentive, board and kind images, the
// "no." plate, the jewel price of a board for sale, the New / Clear marks); Set_disp_param places a row on the
// wheel (x along a sine, scale / depth / tint from the cosine).
#pragma once

#include "BaseEndlessrollView.hpp"

struct SphereScrollView : BaseEndlessrollView {
    guest::Vector newIds;                    // +0x3B8 std::vector<int>: boards marked New
    guest::Vector spheres;                   // +0x3C4 std::vector<shared_ptr<const master::Sphere>> shown

    static constexpr uint32_t SIZE = 0x3D0;
    static constexpr uint32_t VTABLE_START = 0xF824AC;   // _ZTV16SphereScrollView
    static constexpr uint32_t VTABLE = VTABLE_START + 8;

    static Addr create(int32_t type);
    static SphereScrollView *destroy(SphereScrollView *self);
    static uint32_t init(SphereScrollView *self, int32_t type);
    static void setSphereBoard(SphereScrollView *self, Addr spheres, int32_t index);   // vector by value
    static void setSphereBoardNewId(SphereScrollView *self, Addr ids);                // const vector<int>*
    static Addr loadTextureIncentive(Addr self, int32_t id);
    static Addr loadTextureBoard(Addr self, int32_t id);
    static Addr loadTextureKind(Addr self, int32_t id);
    static void changeRow(SphereScrollView *self, Addr row, int32_t item);
    static void Set_disp_param(SphereScrollView *self, int32_t row, float angle);
};
RW_LAYOUT(SphereScrollView, newIds, 0x3B8);
RW_LAYOUT(SphereScrollView, spheres, 0x3C4);
RW_SIZE(SphereScrollView, SphereScrollView::SIZE);
