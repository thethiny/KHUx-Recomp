// LoadingIndicator: the "now loading" overlay shown while a request is pending: a half-transparent black layer
// that swallows touches and the cocostudio "LoadingAnimation" armature in the bottom right corner, faded in and
// out (hide removes the node at the end of the fade).
#pragma once

#include "BattleUi.hpp"

struct LoadingIndicator {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Node
    const uint8_t pad_004[620];                  // +4
    rw::Field<uint32_t> shade;             // +624 cocos2d::LayerColor (black, opacity 127)
    rw::Field<uint32_t> animation;         // +628 cocostudio::Armature "LoadingAnimation"
    rw::Field<uint8_t> showing;            // +632
    const uint8_t pad_279[7];                    // +633

    static constexpr uint32_t SIZE = 0x280;
    static constexpr uint32_t VTABLE = 0xF83008;   // _ZTV16LoadingIndicator + 8

    static uint32_t init(LoadingIndicator *self);
    static LoadingIndicator *destroy(LoadingIndicator *self);
    static void destroyDelete(LoadingIndicator *self);
    static uint32_t show(LoadingIndicator *self);
    static uint32_t hide(LoadingIndicator *self);
    static LoadingIndicator *create();
};
RW_LAYOUT(LoadingIndicator, shade, 624);
RW_LAYOUT(LoadingIndicator, showing, 632);
RW_SIZE(LoadingIndicator, LoadingIndicator::SIZE);
