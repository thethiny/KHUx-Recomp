// AvatarPartsScrollView: the two-column list of avatar parts in the avatar editor (SceneAvatarEdit). A
// BaseScrollView<int> (cocos2d::ui::Layout + IScrollProtocol) over master::AvatarParts ids: each cell shows the
// part icon, name, category, the gender marks and a "new" badge; a tap selects the part (or toggles it in the
// accessory multi-select) and reports it through two std::function<void(int)> callbacks.
#pragma once

#include "../core/Guest.hpp"
#include "../scene/SceneUi.hpp"

struct AvatarPartsScrollView {
    using Addr = uint32_t;

    const uint8_t layout[0x4FC];                   // cocos2d::ui::Layout (vtables at +0, +0x280, +0x350)
    // IScrollProtocol (set up by the inlined base constructors)
    rw::Field<float> scrollRate;             // +0x4FC 0.1
    rw::Field<float> scrollDecay;            // +0x500 0.3
    rw::Field<uint32_t> scroll504[3];        // +0x504
    rw::Field<float> scroll510;              // +0x510 200.0
    rw::Field<float> itemWidth;              // +0x514 360.0 (init)
    rw::Field<float> itemHeight;             // +0x518 122.0 (init)
    rw::Field<uint32_t> cells;               // +0x51C cell widgets (array of Widget*, one per visible row)
    const uint8_t pad_520[0x538 - 0x520];
    rw::Field<int32_t> maxColumn;            // +0x538 items per row
    rw::Field<uint8_t> scroll53C;            // +0x53C
    const uint8_t pad_53D[3];
    rw::Field<float> scrollOffset;           // +0x540 scroll position (rows = offset / row height)
    const uint8_t pad_544[0x570 - 0x544];
    rw::Field<float> scrollBar570;           // +0x570 2.0 (init)
    rw::Field<float> scrollBar574;           // +0x574 -9.0
    const uint8_t pad_578[4];
    rw::Field<float> scrollBar57C;           // +0x57C 2.0
    const uint8_t pad_580[0x628 - 0x580];
    guest::Vector itemIDs;                   // +0x628 BaseScrollView<int>: the part ids shown
    const uint8_t pad_634[4];
    const uint8_t onSelect[24];                    // +0x638 std::function<void(int)> a part was chosen
    const uint8_t onToggle[24];                    // +0x650 std::function<void(int)> a selected part was tapped again
    rw::Field<uint32_t> panelTemplate;       // +0x668 retained cell layout (AvatarEdit_Panel.json)
    rw::Field<uint8_t> touching;             // +0x66C a touch began on a cell
    const uint8_t pad_66D[3];
    rw::Field<int32_t> touchIndex;           // +0x670 item index under the touch
    rw::Field<uint8_t> tapping;              // +0x674 tap animation running
    const uint8_t pad_675[3];
    rw::Field<float> touchStartY;            // +0x678 touch y at touch begin
    rw::Field<int32_t> selectedID;           // +0x67C the selected part (single select)
    rw::Field<uint8_t> selectedIDsEnabled;   // +0x680 multi select (accessories)
    const uint8_t pad_681[3];
    guest::Vector selectedIDs;               // +0x684 std::vector<int> selected accessories
    rw::Field<uint8_t> isSet;                // +0x690 the list shows part sets (set/ icons, new set ids)
    rw::Field<uint8_t> showGender;           // +0x691 show the gender mark
    rw::Field<uint8_t> isMale;               // +0x692 male icon base (else the female one)
    rw::Field<uint8_t> showSelection;        // +0x693 mark the selected part(s)
    const uint8_t pad_694[4];

    static constexpr uint32_t SIZE = 0x698;
    static constexpr uint32_t TYPEINFO_VTABLE = 0xF87DA8;    // _ZTV21AvatarPartsScrollView
    static constexpr uint32_t VTABLE = 0xF87DB0;
    static constexpr uint32_t VTABLE2 = 0xF88108;            // +0x280
    static constexpr uint32_t VTABLE3 = 0xF8811C;            // +0x350

    static Addr create();
    static AvatarPartsScrollView *construct(AvatarPartsScrollView *self);
    static AvatarPartsScrollView *destroy(AvatarPartsScrollView *self);
    static void destroyDelete(AvatarPartsScrollView *self);
    static AvatarPartsScrollView *destroyThunk640(Addr base);
    static AvatarPartsScrollView *destroyThunk848(Addr base);
    static void destroyDeleteThunk640(Addr base);
    static void destroyDeleteThunk848(Addr base);
    static uint32_t init(AvatarPartsScrollView *self);
    static void update(AvatarPartsScrollView *self, float dt);
    static Addr getLayoutPanel(AvatarPartsScrollView *self, int32_t row);
    static Addr createRow(AvatarPartsScrollView *self, uint32_t index, int32_t column);
    static void changeRow(AvatarPartsScrollView *self, Addr cell, uint32_t index, int32_t column);
    static void registItemTouchEvent(AvatarPartsScrollView *self, Addr widget, int32_t index);
    static void setSelectedID(AvatarPartsScrollView *self, int32_t id);
    static void setSelectedIDs(AvatarPartsScrollView *self, Addr ids);   // std::vector<int> by value
    static void setSelectedIDsEnabled(AvatarPartsScrollView *self, uint32_t enabled);
};
RW_LAYOUT(AvatarPartsScrollView, scrollRate, 0x4FC);
RW_LAYOUT(AvatarPartsScrollView, cells, 0x51C);
RW_LAYOUT(AvatarPartsScrollView, maxColumn, 0x538);
RW_LAYOUT(AvatarPartsScrollView, scrollOffset, 0x540);
RW_LAYOUT(AvatarPartsScrollView, itemIDs, 0x628);
RW_LAYOUT(AvatarPartsScrollView, onSelect, 0x638);
RW_LAYOUT(AvatarPartsScrollView, panelTemplate, 0x668);
RW_LAYOUT(AvatarPartsScrollView, selectedID, 0x67C);
RW_LAYOUT(AvatarPartsScrollView, selectedIDs, 0x684);
RW_LAYOUT(AvatarPartsScrollView, isSet, 0x690);
RW_SIZE(AvatarPartsScrollView, AvatarPartsScrollView::SIZE);
