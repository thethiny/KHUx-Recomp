// DeckRevolverView: the keyblade "revolver" of the deck editor (a cocos2d::Node holding the cocostudio layout
// DeckEdit_Revolver.json): five medal slots (DeckMedalView in Area_1..Area_5), the keyblade's carousel / button
// style images and per-slot open levels. A medal can be dragged out of a slot (a clone of its "Medal" widget
// follows the finger on moveLayer) and dropped onto another slot to swap them; a tap opens the medal info popup.
#pragma once

#include "W4B1Common.hpp"

struct DeckRevolverView {
    const uint8_t node[0x270];                // cocos2d::Node
    const uint8_t onChangeMedal[24];          // +0x270 std::function<void(MedalInfoButtonType, bool, uint64)>
    const uint8_t onSwapMedal[24];            // +0x288 std::function<void(uint64)>: a medal was replaced (old id)
    const uint8_t onMedalChanged[24];         // +0x2A0 std::function<void()>
    rw::Field<uint32_t> keyblade;             // +0x2B8 shared_ptr<KeyBladeDataManager::UserKeyBladeData>
    rw::Field<uint32_t> keybladeCtl;          // +0x2BC
    guest::Vector medals;                     // +0x2C0 vector<shared_ptr<MedalDataManager::UserMedalData>>
    guest::Vector slotOpenLevels;             // +0x2CC vector<int>: level each slot opens at (0 = open)
    rw::Field<int32_t> moveIndex;             // +0x2D8 slot of the touched / dragged medal
    rw::Field<uint32_t> moveMedal;            // +0x2DC ui::Widget* "Medal" being dragged (a clone while moving)
    rw::Field<uint32_t> moveParent;           // +0x2E0 its original parent
    rw::Field<uint32_t> moveView;             // +0x2E4 DeckMedalView* of the touched slot
    rw::Field<float> touchStartX;             // +0x2E8 where the touch began (node space)
    rw::Field<float> touchStartY;             // +0x2EC
    rw::Field<float> moveStartX;              // +0x2F0 the medal's original position
    rw::Field<float> moveStartY;              // +0x2F4
    rw::Field<float> touchX;                  // +0x2F8 the current touch (node space)
    rw::Field<float> touchY;                  // +0x2FC
    rw::Field<uint8_t> touchDone;             // +0x300 the touch ended / was cancelled
    rw::Field<uint8_t> moved;                 // +0x301 dragged more than 20 pixels
    rw::Field<uint8_t> popupOpened;           // +0x302 the medal info popup was opened by this touch
    const uint8_t pad_303[1];
    rw::Field<uint32_t> field_304;            // +0x304
    rw::Field<float> moveScale;               // +0x308 the dragged medal's original scale
    rw::Field<uint8_t> moveMedalEnabled;      // +0x30C medals can be dragged
    rw::Field<uint8_t> openMedalInfoPopupEnabled;  // +0x30D a tap opens the medal info popup
    const uint8_t pad_30E[2];
    rw::Field<int32_t> showType;              // +0x310 DeckMedalView::ShowType of the slots
    rw::Field<uint8_t> field_314;             // +0x314 (medal info popup flag)
    const uint8_t pad_315[3];
    rw::Field<uint32_t> skills;               // +0x318 vector<UserSkill::Skill>*
    rw::Field<uint32_t> detail;               // +0x31C shared_ptr<UserDetail> (SafeBuffer)
    rw::Field<uint32_t> detailCtl;            // +0x320
    rw::Field<uint32_t> moveLayer;            // +0x324 node the dragged medal moves on
    rw::Field<uint8_t> field_328;             // +0x328 (medal info popup flag)
    const uint8_t pad_329[3];
    guest::Vector medalViews;                 // +0x32C vector<DeckMedalView*>

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x338;
    static constexpr uint32_t VTABLE = 0xF85F68 + 8;     // _ZTV16DeckRevolverView + 8

    static Addr destroy(Addr self);                                     // D2
    static Addr create(Addr keyblade, Addr detail);
    static Addr construct(Addr self);                                   // C2
    static uint32_t init(Addr self, Addr keyblade, Addr detail);
    static void setKeyBladeData(Addr self, Addr keyblade);
    static void resetMoveMedal(Addr self);
    static void setMedalDatas(Addr self, Addr medals);
    static void settingSlotOpenLevels(Addr keyblade, Addr levels);      // static
    static void changeShowType(Addr self, int32_t type);
    static int32_t getTouchMedalIndex(Addr self, Addr pos, uint32_t anySlot);
    static void getTouchMedalPosition(Addr out, Addr self, Addr pos);
    static void setMedalData(Addr self, uint32_t index, Addr medal, uint32_t animate, uint32_t relayout);
    static uint32_t setIsShowMixInfo(Addr self, uint32_t show);
    static uint32_t setIsShowMedalInfo(Addr self, uint32_t show);

    // lambdas
    static uint32_t onTouchBegan(Addr fn, Addr touchRef, Addr eventRef);       // init::$_1
    static void onTouchMoved(Addr fn, Addr touchRef, Addr eventRef);           // init::$_2
    static void onTouchEnded(Addr fn, Addr touchRef, Addr eventRef);           // init::$_3
    static void onTouchCancelled(Addr fn, Addr touchRef, Addr eventRef);       // init::$_4
    static void onSwapDone(Addr fn);                                           // $_3::{lambda()#1}
    static void onDropOutsideDone(Addr fn);                                    // $_3::{lambda()#2}
    static void onMoveBackDone(Addr fn);                                       // $_3::{lambda()#3}
    static void onMedalInfoButton(Addr fn, Addr typeRef, Addr flagRef);        // $_3::{lambda()#2}::{lambda#1}
    static void onSetMedalDone(Addr fn);                                       // setMedalData::$_7
};
RW_LAYOUT(DeckRevolverView, onChangeMedal, 0x270);
RW_LAYOUT(DeckRevolverView, keyblade, 0x2B8);
RW_LAYOUT(DeckRevolverView, medals, 0x2C0);
RW_LAYOUT(DeckRevolverView, moveIndex, 0x2D8);
RW_LAYOUT(DeckRevolverView, touchStartX, 0x2E8);
RW_LAYOUT(DeckRevolverView, touchDone, 0x300);
RW_LAYOUT(DeckRevolverView, moveScale, 0x308);
RW_LAYOUT(DeckRevolverView, showType, 0x310);
RW_LAYOUT(DeckRevolverView, skills, 0x318);
RW_LAYOUT(DeckRevolverView, moveLayer, 0x324);
RW_LAYOUT(DeckRevolverView, medalViews, 0x32C);
RW_SIZE(DeckRevolverView, DeckRevolverView::SIZE);
