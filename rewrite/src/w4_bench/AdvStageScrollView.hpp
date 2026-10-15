// AdvStageScrollView: the quest list of the adventure screen (SceneAdventureSelect). A BaseStageScrollView whose
// rows are cocostudio panels (story / event / colosseum, AdventureTop2_*_Panel.json), with the stage frames,
// boss / raid / colosseum info boxes and party avatar frames built from the master data and the user's data.
// operator new(0x6B8): its own fields start at +1616, right after BaseStageScrollView.
#pragma once

#include "w4_bench/BaseStageScrollView.hpp"
#include "scene/SceneUi.hpp"

struct AdvStageScrollView : BaseStageScrollView {
    using Addr = uint32_t;

    // +1600: std::function<void(int)> (its 16-byte buffer is the last bytes of the base; the callable pointer is
    // the first word here)
    rw::Field<uint32_t> onSelect_f;          // +1616  (function at +1600)
    const uint8_t pad_654[4];                // +1620
    StdFunction callback2;                   // +1624  std::function (callable pointer at +1640)
    rw::Field<uint32_t> field_66C;           // +1644
    rw::Field<int32_t> stageType;            // +1648  StartDeckEditDialog::StageType: 0 story, 1 event, 2 colosseum
    rw::Field<uint8_t> touching;             // +1652  a touch began on a row (cleared when it moves out / ends)
    const uint8_t pad_675[3];                // +1653
    rw::Field<uint32_t> field_678;           // +1656  (+1656..+1681 zeroed by create); 0 when a row is touched
    rw::Field<int32_t> touchedIndex;         // +1660  item index of the row a touch began on
    guest::Vector selectedItems;             // +1664  std::vector<int>: selected item ids, kept sorted
    rw::Field<uint32_t> avatarIconCache;     // +1676  AvatarIconCache*
    rw::Field<uint8_t> touchingLast;         // +1680  the touched row is the last item
    rw::Field<uint8_t> field_691;            // +1681  a touch on a row is ignored while set
    const uint8_t pad_692[2];                // +1682
    rw::Field<uint32_t> field_694;           // +1684
    rw::Field<uint32_t> newStageLwf;         // +1688  cocos2d::LWFNode* lwf/adventure/minig/minig.lwf (new stage mark)
    rw::Field<uint8_t> eventNewStage;        // +1692  the new stage mark is for an event stage
    rw::Field<uint8_t> raidBossShown;        // +1693  the raid boss button is up
    const uint8_t pad_69E[2];                // +1694
    rw::Field<uint32_t> raidBossLwf;         // +1696  cocos2d::LWFNode* lwf/raid/bossbtn/bossbtn.lwf
    rw::Field<uint8_t> partyIconsShown;      // +1700  the party icons area was built (user has a party)
    const uint8_t pad_6A5[3];                // +1701
    guest::Vector rowWidgets;                // +1704  std::vector<cocos2d::ui::Widget*>
    rw::Field<uint32_t> imageDownloader;     // +1716  ImageDownloader* (made by init)

    static constexpr uint32_t SIZE = 0x6B8;
    static constexpr uint32_t ON_SELECT = 1600;   // the std::function whose callable pointer is onSelect_f

    // life cycle (AdvStageScrollView.cpp)
    static Addr create();
    static AdvStageScrollView *destruct(AdvStageScrollView *self);
    static uint32_t init(AdvStageScrollView *self);
    // rows
    static Addr createRow(AdvStageScrollView *self, int32_t a, int32_t b);
    static uint32_t changeRow(AdvStageScrollView *self, Addr widget, int32_t index, int32_t b);
    static void changeOverRow(AdvStageScrollView *self, Addr widget, int32_t index);
    static void registItemTouchEvent(AdvStageScrollView *self, Addr widget, Addr index);
    static void setTouchEndedCallback(Addr fn, const uint32_t *self);
    static void setSelectItem(AdvStageScrollView *self, int32_t item);
    static uint32_t removeSelectItemAll(AdvStageScrollView *self);
    static void setStageType(AdvStageScrollView *self, int32_t type);
    static void pointToString(Addr out, int32_t point, int32_t second, uint32_t hasSecond);   // static
    static uint32_t getSelestItemPositionY(AdvStageScrollView *self, Addr outY);
    static void selectCenterStage(AdvStageScrollView *self);
    static void updateScroll(AdvStageScrollView *self, float dt);
    static void setAdvemtureStageFrame(AdvStageScrollView *self, Addr widget, int32_t index);
    static void setPartyIconsArea(AdvStageScrollView *self, Addr widget);
    // new stage / raid boss marks
    static uint32_t initNewStage(AdvStageScrollView *self, Addr cursor);
    static uint32_t initRaidBoss(AdvStageScrollView *self);
    static void CheckTimeOverRaidBoss(AdvStageScrollView *self);
    // lambda bodies (operator() of the __func objects; `fn` is the __func, captures from +4)
    static uint32_t onTouchBegan(Addr fn, Addr touch, Addr event);       // registItemTouchEvent $_18
    static void onTouchMoved(Addr fn, Addr touch, Addr event);           // registItemTouchEvent $_19
    static void onTouchEnded(Addr fn, Addr touch, Addr event);           // registItemTouchEvent $_20
    static void raidBossFadeIn(Addr fn);                                 // updateScroll $_22
    static void avatarImageLoaded(Addr fn, Addr ok, Addr url);           // settingAvatarFrame $_17
};
RW_LAYOUT(AdvStageScrollView, onSelect_f, 1616);
RW_LAYOUT(AdvStageScrollView, callback2, 1624);
RW_LAYOUT(AdvStageScrollView, stageType, 1648);
RW_LAYOUT(AdvStageScrollView, selectedItems, 1664);
RW_LAYOUT(AdvStageScrollView, avatarIconCache, 1676);
RW_LAYOUT(AdvStageScrollView, touchingLast, 1680);
RW_LAYOUT(AdvStageScrollView, eventNewStage, 1692);
RW_LAYOUT(AdvStageScrollView, raidBossLwf, 1696);
RW_LAYOUT(AdvStageScrollView, rowWidgets, 1704);
RW_LAYOUT(AdvStageScrollView, imageDownloader, 1716);
RW_SIZE(AdvStageScrollView, 0x6B8);
