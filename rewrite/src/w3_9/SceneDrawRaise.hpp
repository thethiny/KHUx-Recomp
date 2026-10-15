// SceneDrawRaise: the medal draw ("raise draw") result scene. It plays the draw LWF demo (gacha / raid gacha
// result display), drops the drawn medals one by one (DropMedalLayer), then opens the result popups (one medal,
// all medals, the bonus "omake" popup). It has a comment edit box (TextField_Comment) for sharing the result.
//
// The scene is a cocos2d::Layer with a second base, cocos2d::extension::EditBoxDelegate, at +0x288 (the
// editBox* callbacks are reached through its non-virtual thunks, this - 0x288). All guest objects are 32-bit guest
// addresses.
#pragma once

#include "core/Guest.hpp"
#include "scene/SceneUi.hpp"

struct SceneDrawRaise {
    // One drawn medal falling on the result screen (std::vector<DropMedalLayer> at +0x294, 16 bytes each).
    struct DropMedalLayer {
        rw::Field<uint32_t> word[4];
    };

    const uint8_t layer[0x288];               // cocos2d::Layer (Node std::function members at +0x210 / +0x228)
    rw::Field<uint32_t> editBoxDelegateVtable; // +0x288 EditBoxDelegate base
    const uint8_t pad_28C[4];
    rw::Field<uint32_t> field_290;            // +0x290 (zeroed by create, with the vector)
    guest::Vector dropMedals;                 // +0x294 std::vector<DropMedalLayer>
    rw::Field<uint32_t> field_2A0;            // +0x2A0
    rw::Field<float> field_2A4;               // +0x2A4 0.1
    rw::Field<float> field_2A8;               // +0x2A8 0.1
    rw::Field<uint8_t> flag_2AC;              // +0x2AC
    rw::Field<uint8_t> flag_2AD;              // +0x2AD
    rw::Field<uint8_t> flag_2AE;              // +0x2AE
    const uint8_t pad_2AF[1];
    rw::Field<uint32_t> editBoxLayer;         // +0x2B0 edit box layer (SceneUtil::editBoxEditingDidEnd)
    rw::Field<uint32_t> editBoxLabel;         // +0x2B4 the label showing the edit box text
    rw::Field<uint32_t> commentText;          // +0x2B8 ui::Text mirroring TextField_Comment
    rw::Field<uint8_t> flag_2BC;              // +0x2BC
    const uint8_t pad_2BD[3];

    static constexpr uint32_t SIZE = 0x2C0;
    static constexpr uint32_t VTABLE_START = 0xF9CF10;            // _ZTV14SceneDrawRaise
    static constexpr uint32_t VTABLE = VTABLE_START + 8;
    static constexpr uint32_t DELEGATE_VTABLE = VTABLE_START + 0x320;
    static constexpr uint32_t DELEGATE_OFFSET = 0x288;            // EditBoxDelegate base (thunks: this - 0x288)

    using Addr = uint32_t;

    static Addr create();
    static Addr createSceneLayer();                     // createSceneLayer<SceneDrawRaise>()
    static Addr createScene();
    static Addr destroy(Addr self);                     // ~SceneDrawRaise (D2)
    static void destroyThunk(Addr delegate);            // non-virtual thunk from the EditBoxDelegate base (D1)
    static void destroyDelete(Addr self);               // deleting destructor (D0)
    static void destroyDeleteThunk(Addr delegate);
    static uint32_t init(Addr self);
    static void update(Addr self, float dt);
    static void editBoxTextChanged(Addr self, Addr editBox, Addr text);
    static void editBoxTextChangedThunk(Addr delegate, Addr editBox, Addr text);
    static void editBoxEditingDidEnd(Addr self, Addr editBox);
    static void editBoxEditingDidEndThunk(Addr delegate, Addr editBox);
    static void editBoxEditingDidBegin(Addr self, Addr editBox);
    static void editBoxReturn(Addr self, Addr editBox);
    // medal: std::shared_ptr<MedalDataManager::UserMedalData> passed by value (owned by the caller)
    static uint32_t isRareMedal(Addr self, Addr medal);

    // std::vector<DropMedalLayer> / its split buffer
    static Addr dropMedalsDestroy(Addr vec);            // __vector_base<DropMedalLayer>::~__vector_base
    static void dropMedalsPushBackSlow(Addr vec, Addr value);
    static Addr dropMedalsSplitBufferDestroy(Addr buf);
};
RW_LAYOUT(SceneDrawRaise, editBoxDelegateVtable, 0x288);
RW_LAYOUT(SceneDrawRaise, dropMedals, 0x294);
RW_LAYOUT(SceneDrawRaise, field_2A4, 0x2A4);
RW_LAYOUT(SceneDrawRaise, editBoxLayer, 0x2B0);
RW_LAYOUT(SceneDrawRaise, flag_2BC, 0x2BC);
RW_SIZE(SceneDrawRaise, SceneDrawRaise::SIZE);

// Guest addresses of the SceneDrawRaise functions (Thumb bit set, as the lifted calls use them), for the files that
// call each other's functions through rw::call.
namespace draw_raise {
constexpr uint32_t OPEN_DRAW_RESULT_POPUP = 0x68FEB1;     // openDrawResultPopup(this, u64 id, bool, bool, function<void()>)
constexpr uint32_t OPEN_DRAW_RESULTS_POPUP = 0x690659;    // openDrawResultsPopup(this, const vector<u64>&)
constexpr uint32_t CREATE_DRAW_MEDAL = 0x6914CD;          // createDrawMedal(this, u64 id, bool, int)
constexpr uint32_t OPEN_DRAW_OMAKE_POPUP = 0x691B55;      // openDrawOmakePopup(this, function<void()>)
constexpr uint32_t DROP_ANIMATION = 0x692661;             // dropAnimation(this, function<void()>)
constexpr uint32_t IS_RARE_MEDAL = 0x69284D;              // isRareMedal(this, shared_ptr<UserMedalData>)
constexpr uint32_t DROP_MEDALS_PUSH_BACK_SLOW = 0x69421D; // vector<DropMedalLayer>::__push_back_slow_path
}  // namespace draw_raise
