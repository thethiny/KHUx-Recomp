// ScenePartyRequest: the party search scene (party menu -> "search"). Two layouts, picked in init from the user's
// party state (APIManager + 0x84, byte 16 of that SafeBuffer):
//   - in a party ("Invitation" layout): search users to invite, by play time zone, level and Lux rank, the
//     results in a MemberScrollView;
//   - no party ("InitialRequest" layout): search parties to join, by play style, party rank, approval method and
//     member count, the results in a PartyScrollView.
// Each search condition opens a choice popup (openPopUp + check boxes); the OK button of the popup commits the
// pending choice (the *Choice field) to the condition. Search by ID opens an edit box popup (openPopUpSearchID),
// openPopUpRequest sends a join request / invitation with a comment.
// Bases: cocos2d::Layer (0x288 bytes), cocos2d::extension::EditBoxDelegate (vptr +0x288), IInOutAnimation
// (vptr +0x28C). Size 0x318.
#pragma once

#include "../core/Guest.hpp"
#include "../scene_social/SocialCommon.hpp"

struct ScenePartyRequest {
    const uint8_t layer[0x288];                 // cocos2d::Layer (its onEnter / onExit std::functions at 0x210 / 0x228)
    rw::Field<uint32_t> editBoxDelegateVtable;  // +0x288 cocos2d::extension::EditBoxDelegate base
    rw::Field<uint32_t> inOutVtable;            // +0x28C IInOutAnimation base (runOutAnimation)
    rw::Field<uint32_t> root;                   // +0x290 the scene layout (PartyTop_*_Scene_ver130.json)
    rw::Field<uint8_t> inviteMode;              // +0x294 the user is in a party: invite users instead of joining
    const uint8_t pad_295[3];
    // search conditions (value, pending choice while its popup is open)
    rw::Field<int32_t> playStyle;               // +0x298 APIManager::Frequency
    rw::Field<int32_t> playStyleChoice;         // +0x29C
    rw::Field<int32_t> partyRank;               // +0x2A0 APIManager::PartyRank
    rw::Field<int32_t> partyRankChoice;         // +0x2A4
    rw::Field<int32_t> approval;                // +0x2A8 APIManager::AgreeType
    rw::Field<int32_t> approvalChoice;          // +0x2AC
    rw::Field<int32_t> memberCount;             // +0x2B0 APIManager::PartyScale
    rw::Field<int32_t> memberCountChoice;       // +0x2B4
    rw::Field<int32_t> timeZoneChoice;          // +0x2B8 (the pair is the other way round here)
    rw::Field<int32_t> timeZone;                // +0x2BC APIManager::Timezone (init: 7)
    rw::Field<int32_t> level;                   // +0x2C0 APIManager::LevelRange
    rw::Field<int32_t> levelChoice;             // +0x2C4
    rw::Field<int32_t> luxRank;                 // +0x2C8 APIManager::LuxRank
    rw::Field<int32_t> luxRankChoice;           // +0x2CC
    rw::Field<uint32_t> partyScrollView;        // +0x2D0 PartyScrollView (join layout)
    rw::Field<uint32_t> memberScrollView;       // +0x2D4 MemberScrollView (invite layout)
    guest::String comment;                      // +0x2D8 request comment (std::string)
    const uint8_t pad_2E4[4];
    const uint8_t onPopupOk[0x18];              // +0x2E8 std::function<void()> run by the open popup's OK button
                                                //       (callable pointer at +0x2F8)
    rw::Field<uint32_t> editBoxLayer;           // +0x300 Node* filled by SceneUtil::textFieldToEditBox
    rw::Field<uint32_t> editBoxLabel;           // +0x304 Label* (the same)
    rw::Field<uint8_t> editing;                 // +0x308 an edit box is being edited
    const uint8_t pad_309[3];
    rw::Field<uint32_t> inputText;              // +0x30C ui::Text showing the edit box's text
    rw::Field<uint8_t> canBack;                 // +0x310 the back key / button may leave the scene
    const uint8_t pad_311[7];

    static constexpr uint32_t SIZE = 0x318;
    static constexpr uint32_t ON_POPUP_OK = 0x2E8;       // offset of onPopupOk
    static constexpr uint32_t COMMENT = 0x2D8;           // offset of comment

    using Addr = uint32_t;
    static Addr create();
    static Addr construct(ScenePartyRequest *self);
    static Addr createScene();
    static Addr destroy(ScenePartyRequest *self);
    static void destroyDelete(ScenePartyRequest *self);
    static uint32_t init(ScenePartyRequest *self);
    static void update(ScenePartyRequest *self, float dt);
    static void runInAnimation(ScenePartyRequest *self);
    static void runInJoinLayout(ScenePartyRequest *self);
    static void runInEntryLayout(ScenePartyRequest *self);
    static void runOutAnimationMain(ScenePartyRequest *self, Addr done);
    static void runOutJoinLayout(ScenePartyRequest *self, Addr done);
    static void runOutEntryLayout(ScenePartyRequest *self, Addr done);
    static void backScene(ScenePartyRequest *self);
    // static (no this): search requests; `done` is a std::function<void(bool)>* taken by value
    static void callUserSearchAPI(int32_t levelRange, int32_t timeZone, int32_t frequency, int32_t luxRank,
                                  Addr done);
    static void callPartySearchAPI(int32_t frequency, int32_t partyRank, int32_t agreeType, int32_t partyScale,
                                   Addr done);
    // popups; `onOk` / `onClose` are std::function<void()>* taken by value
    static Addr openPopUp(ScenePartyRequest *self, Addr json, Addr onOk, Addr onClose);
    static void openPopUpPlayStyle(ScenePartyRequest *self);
    static void openPopUpRank(ScenePartyRequest *self);
    static void openPopUpJoin(ScenePartyRequest *self);
    static void openPopUpMember(ScenePartyRequest *self);
    static void openPopUpTimeZone(ScenePartyRequest *self);
    static void openPopUpLevel(ScenePartyRequest *self);
    static void openPopUpLux(ScenePartyRequest *self);
    static void openPopUpSearchID(ScenePartyRequest *self);
    static void openPopUpRequest(ScenePartyRequest *self, Addr onOk, Addr onClose);
    // EditBoxDelegate
    static void editBoxEditingDidBegin(ScenePartyRequest *self, Addr editBox);
    static void editBoxEditingDidEnd(ScenePartyRequest *self, Addr editBox);
    static void editBoxTextChanged(ScenePartyRequest *self, Addr editBox, Addr text);
    static void editBoxReturn(ScenePartyRequest *self, Addr editBox);
};
RW_LAYOUT(ScenePartyRequest, editBoxDelegateVtable, 0x288);
RW_LAYOUT(ScenePartyRequest, root, 0x290);
RW_LAYOUT(ScenePartyRequest, inviteMode, 0x294);
RW_LAYOUT(ScenePartyRequest, playStyle, 0x298);
RW_LAYOUT(ScenePartyRequest, timeZone, 0x2BC);
RW_LAYOUT(ScenePartyRequest, luxRankChoice, 0x2CC);
RW_LAYOUT(ScenePartyRequest, partyScrollView, 0x2D0);
RW_LAYOUT(ScenePartyRequest, comment, ScenePartyRequest::COMMENT);
RW_LAYOUT(ScenePartyRequest, onPopupOk, ScenePartyRequest::ON_POPUP_OK);
RW_LAYOUT(ScenePartyRequest, editBoxLayer, 0x300);
RW_LAYOUT(ScenePartyRequest, inputText, 0x30C);
RW_LAYOUT(ScenePartyRequest, canBack, 0x310);
RW_SIZE(ScenePartyRequest, ScenePartyRequest::SIZE);
