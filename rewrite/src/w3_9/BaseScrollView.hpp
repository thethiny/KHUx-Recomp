// BaseScrollView<T>: IScrollProtocol plus the list of item ids it shows (std::vector<T> at +0x628; T is int or
// unsigned long long). setItemIDs copies the ids and resizes the list. Both instantiations share the layout.
#pragma once

#include "../scene_main/IScrollProtocol.hpp"

struct BaseScrollView : IScrollProtocol {
    guest::Vector itemIDs;                   // +0x628 std::vector<T>
    // (the derived classes' members start at +0x634)

    static constexpr uint32_t SIZE = 0x634;
    // BaseScrollView<int> vtables (+0, +0x280, +0x350)
    static constexpr uint32_t VTABLE_INT = 0xF811D8, VTABLE2_INT = 0xF81530, VTABLE3_INT = 0xF81544;
    // BaseScrollView<unsigned long long>
    static constexpr uint32_t VTABLE_ULL = 0xF8196C, VTABLE2_ULL = 0xF81CC4, VTABLE3_ULL = 0xF81CD8;

    // BaseScrollView<int>
    static BaseScrollView *constructInt(BaseScrollView *self);
    static uint32_t initInt(BaseScrollView *self);
    static void setItemIDsInt(BaseScrollView *self, Addr ids);
    static BaseScrollView *destroyInt(BaseScrollView *self);
    static void destroyDeleteInt(BaseScrollView *self);
    // BaseScrollView<unsigned long long>
    static uint32_t initUll(BaseScrollView *self);
    static void setItemIDsUll(BaseScrollView *self, Addr ids);
    static BaseScrollView *destroyUll(BaseScrollView *self);
    static void destroyDeleteUll(BaseScrollView *self);

    // the IScrollProtocol defaults the derived constructors inline (BaseScrollView<int>::BaseScrollView)
    static void initScrollDefaults(BaseScrollView *self);
};
RW_LAYOUT(BaseScrollView, itemIDs, 0x628);
RW_SIZE(BaseScrollView, BaseScrollView::SIZE);
