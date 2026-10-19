// MyRecordScrollView: the "records" list of the Player Information dialog (quests cleared, medals collected...):
// category headers and value rows cloned from two cocostudio templates into a cocos2d::extension::ScrollView,
// one row per frame (update), with a scroll bar that fades out while the list stands still.
#pragma once

#include "../scene/SceneUi.hpp"

struct MyRecordScrollView {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x538;

    // one row of the list (std::vector<RecordData> element)
    struct RecordData {
        rw::Field<int32_t> textId;          // +0x00 the label (TextData)
        const uint8_t pad_04[4];
        rw::Field<uint64_t> value;          // +0x08 the number shown (rows)
        rw::Field<uint8_t> isCategory;      // +0x10 a category header
        const uint8_t pad_11[7];
    };

    rw::Field<uint32_t> vtable;             // +0x000 cocos2d::ui::Layout
    const uint8_t pad_004[0x27C];
    rw::Field<uint32_t> vtable2;            // +0x280
    const uint8_t pad_284[0xCC];
    rw::Field<uint32_t> vtable3;            // +0x350
    const uint8_t pad_354[0x1A8];
    rw::Field<uint32_t> categoryTemplate;   // +0x4FC MyRecord_Category layout (retained)
    rw::Field<uint32_t> panelTemplate;      // +0x500 MyRecord_Panel layout (retained)
    rw::Field<uint32_t> scrollView;         // +0x504 cocos2d::extension::ScrollView
    rw::Field<float> contentHeight;         // +0x508 the list's total height
    rw::Field<float> nextY;                 // +0x50C where the next row goes (counts down from the top)
    rw::Field<uint8_t> building;            // +0x510 rows are still being added, one per frame
    const uint8_t pad_511[3];
    rw::Field<uint32_t> recordsBegin;       // +0x514 std::vector<RecordData>
    rw::Field<uint32_t> recordsEnd;         // +0x518
    rw::Field<uint32_t> recordsCap;         // +0x51C
    rw::Field<uint32_t> built;              // +0x520 rows added so far
    rw::Field<uint32_t> scrollBar;          // +0x524 MyRecordScrollBar*
    rw::Field<uint8_t> scrollBarEnabled;    // +0x528
    const uint8_t pad_529[3];
    rw::Field<float> lastScrollPosition;    // +0x52C
    rw::Field<uint8_t> scrollBarShown;      // +0x530
    const uint8_t pad_531[7];

    static Addr create();
    static uint32_t init(MyRecordScrollView *self);
    static MyRecordScrollView *destruct(MyRecordScrollView *self);
    static void update(MyRecordScrollView *self, float dt);
    static void setContentSize(MyRecordScrollView *self, Addr size);
    static void setEnabledScrollBar(MyRecordScrollView *self, uint32_t enabled);
};
RW_LAYOUT(MyRecordScrollView, vtable2, 0x280);
RW_LAYOUT(MyRecordScrollView, vtable3, 0x350);
RW_LAYOUT(MyRecordScrollView, categoryTemplate, 0x4FC);
RW_LAYOUT(MyRecordScrollView, building, 0x510);
RW_LAYOUT(MyRecordScrollView, recordsBegin, 0x514);
RW_LAYOUT(MyRecordScrollView, built, 0x520);
RW_LAYOUT(MyRecordScrollView, scrollBarEnabled, 0x528);
RW_LAYOUT(MyRecordScrollView, scrollBarShown, 0x530);
RW_SIZE(MyRecordScrollView, 0x538);
RW_SIZE(MyRecordScrollView::RecordData, 0x18);
