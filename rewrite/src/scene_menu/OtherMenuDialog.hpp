// OtherMenuDialog: the "Other" page of MyPage's side MENU (notices, player information, album, party, union
// transfer, serial codes, legal information, help, settings, credits): a cocostudio layout over a shadow, opened
// from the top and closed back up.
#pragma once

#include "../scene/SceneUi.hpp"

struct OtherMenuDialog {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x2A0;

    rw::Field<uint32_t> vtable;             // +0x000 cocos2d::Node
    const uint8_t pad_004[0x26C];
    rw::Field<uint32_t> vtable2;            // +0x270 cocos2d::extension::EditBoxDelegate (serial code field)
    rw::Field<uint32_t> root;               // +0x274 the MenuDialog_ver120 layout
    rw::Field<uint32_t> parent;             // +0x278 set by open, the node it was opened over (0: closed)
    rw::Field<uint32_t> shadow;             // +0x27C the shadow sprite under the layout
    const uint8_t pad_280[0x20];            // the popups' state (cleared by create)

    static Addr create();
    static void open(OtherMenuDialog *self, Addr parent);
    static void close(OtherMenuDialog *self);
    // a menu button's "Txt" label: the shadow style and a TextData text
    static void setButtonLabel(Addr button, int32_t textId);
};
RW_LAYOUT(OtherMenuDialog, vtable2, 0x270);
RW_LAYOUT(OtherMenuDialog, root, 0x274);
RW_LAYOUT(OtherMenuDialog, shadow, 0x27C);
RW_SIZE(OtherMenuDialog, 0x2A0);
