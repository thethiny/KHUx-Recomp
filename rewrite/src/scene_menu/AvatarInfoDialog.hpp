// AvatarInfoDialog: the Player Information dialog (a player's avatar, titles, records, party actions), opened from
// MyPage's Other menu (the player) or from a party member / ranking entry (another player). open asks the server
// for the profile (/user/profile, executeUserProfileGet); its callback builds the page (createLayout).
#pragma once

#include "../scene/SceneUi.hpp"

struct AvatarInfoDialog {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x4C8;

    rw::Field<uint32_t> vtable;             // +0x000 cocos2d::Node
    const uint8_t pad_004[0x26C];
    rw::Field<uint32_t> vtable2;            // +0x270 cocos2d::extension::EditBoxDelegate (name / message fields)
    rw::Field<uint32_t> root;               // +0x274 the AvatarInfoScene_A layout
    rw::Field<uint32_t> parent;             // +0x278 set by open (0: closed)
    rw::Field<uint32_t> shadow;             // +0x27C
    const uint8_t onClose[0x18];            // +0x280 std::function<void(bool)>
    const uint8_t skills[0xC];              // +0x298 std::vector<UserSkill::Skill>
    rw::Field<uint8_t> field_2A4;           // +0x2A4
    const uint8_t pad_2A5[3];
    const uint8_t canClose[0x18];           // +0x2A8 std::function<bool()>
    rw::Field<uint32_t> userIdLo;           // +0x2C0 the player shown (another player only)
    rw::Field<uint32_t> userIdHi;           // +0x2C4
    rw::Field<uint8_t> isOtherUser;         // +0x2C8
    rw::Field<uint8_t> viewerFlag;          // +0x2C9 the viewer's setting byte 0x11 (UserSetting)
    rw::Field<uint8_t> field_2CA;           // +0x2CA
    const uint8_t pad_2CB[0x18D];
    const uint8_t pendingPopup[0x18];       // +0x458 std::function<void(bool)> of an open sub-popup
    const uint8_t pad_470[0x54];
    rw::Field<uint32_t> imageDownloader;    // +0x4C4 ImageDownloader* (owned)

    static Addr create();
    static AvatarInfoDialog *construct(AvatarInfoDialog *self);
    static uint32_t init(AvatarInfoDialog *self);
    static AvatarInfoDialog *destruct(AvatarInfoDialog *self);
    static uint32_t open(AvatarInfoDialog *self, Addr parent, uint32_t idLo, uint32_t idHi,
                         Addr onClose, uint32_t flag, Addr canClose);
    static void close(AvatarInfoDialog *self);
    static void setTouchEnabledUnderUIButtons(AvatarInfoDialog *self, uint32_t enabled);
};
RW_LAYOUT(AvatarInfoDialog, vtable2, 0x270);
RW_LAYOUT(AvatarInfoDialog, shadow, 0x27C);
RW_LAYOUT(AvatarInfoDialog, field_2A4, 0x2A4);
RW_LAYOUT(AvatarInfoDialog, userIdLo, 0x2C0);
RW_LAYOUT(AvatarInfoDialog, field_2CA, 0x2CA);
RW_LAYOUT(AvatarInfoDialog, imageDownloader, 0x4C4);
RW_SIZE(AvatarInfoDialog, 0x4C8);
