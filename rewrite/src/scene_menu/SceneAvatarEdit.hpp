// SceneAvatarEdit: the avatar editor ("My Avatar"). Two modes: creating the first avatar (isCreate: the user has
// no avatar yet - gender switch, a single coordinate, "decide" button) and editing the saved coordinates
// (MyCoordinate slots 1..n, accessories with filters, save popup). The left page holds the parts list
// (AvatarPartsScrollView) with seven tabs, the right page the AvatarActor preview.
//
// A cocos2d::Layer with a second base (the scene in/out animation interface at +0x288). Guest objects are 32-bit
// guest addresses (Addr).
#pragma once

#include "../core/Guest.hpp"
#include "../scene/SceneUi.hpp"

struct SceneAvatarEdit {
    using Addr = uint32_t;

    // hole::network::api::UserAvatar::Coordinate (48 bytes): one saved outfit.
    struct Coordinate {
        rw::Field<int32_t> id;            // +0x00
        rw::Field<int32_t> gender;        // +0x04 1 = male, 2 = female
        rw::Field<int32_t> parts[5];      // +0x08 tab parts: hair (+8), face (+0xC), body (+0x10), +0x14, +0x18
        rw::Field<int32_t> accessory[5];  // +0x1C accessory part ids (0 = empty)
    };

    const uint8_t layer[0x288];                     // cocos2d::Layer (Node::_onEnterCallback +0x210, _onExitCallback +0x228)
    rw::Field<uint32_t> vtable2;              // +0x288 second base (in / out animation interface)
    rw::Field<uint32_t> root;                 // +0x28C the cocostudio scene (AvatarEditScene_ver120.json)
    rw::Field<uint32_t> book;                 // +0x290 "Book"
    rw::Field<uint32_t> centerUI;             // +0x294 "CenterUI"
    rw::Field<uint32_t> centerUILeft;         // +0x298 "CenterUI_Left" (parts list page)
    rw::Field<uint32_t> centerUIRight;        // +0x29C "CenterUI_Right" (avatar page)
    rw::Field<uint32_t> switchNode;           // +0x2A0 "Switch" (gender switch)
    rw::Field<uint32_t> tabAccessory;         // +0x2A4 "Tab_Accessorie" (accessory cost panel)
    rw::Field<uint32_t> filterButton;         // +0x2A8 "Button_Filter" (accessory filter)
    rw::Field<uint32_t> avatarActor;          // +0x2AC AvatarActor preview
    guest::Vector tabButtons;                 // +0x2B0 cocos2d::Vector<ImageButton*> the seven tabs
    guest::Vector coordinateButtons;          // +0x2BC cocos2d::Vector<ImageButton*> MyCoordinate slots
    rw::Field<uint32_t> newTabsMale[3];       // +0x2C8 std::vector<bool> "new parts" mark per tab (male)
    rw::Field<uint32_t> newTabsFemale[3];     // +0x2D4 std::vector<bool> (female)
    rw::Field<uint8_t> isCreate;              // +0x2E0 user has no avatar yet (UserData byte 1)
    const uint8_t pad_2E1[3];
    rw::Field<int32_t> coordinateIndex;       // +0x2E4 selected MyCoordinate slot
    rw::Field<int32_t> tabIndex;              // +0x2E8 selected tab (0..6; 2 = accessories)
    rw::Field<uint8_t> isMale;                // +0x2EC the current coordinate is male
    rw::Field<uint8_t> showKeyblade;          // +0x2ED preview with the equipped keyblade
    const uint8_t pad_2EE[2];
    rw::Field<uint32_t> scrollView;           // +0x2F0 AvatarPartsScrollView*
    guest::Vector partIdsByTab;               // +0x2F4 std::vector<std::vector<int>> part ids per tab
    guest::Vector coordinates;                // +0x300 std::vector<Coordinate>
    rw::Field<uint32_t> currentCoordinate;    // +0x30C Coordinate* being edited
    Coordinate defaultCoordinate;             // +0x310 the outfit of a new avatar
    rw::Field<uint8_t> isTutorialEdit;        // +0x340 entered from the menu (not the first creation)
    rw::Field<uint8_t> filter[10];            // +0x341 accessory filters (all on)
    const uint8_t pad_34B[0x358 - 0x34B];

    static constexpr uint32_t SIZE = 0x358;
    static constexpr uint32_t TYPEINFO_VTABLE = 0xFA3380;   // _ZTV15SceneAvatarEdit
    static constexpr uint32_t VTABLE = 0xFA3388;            // primary vtable (+8)
    static constexpr uint32_t VTABLE2 = 0xFA3698;           // second base (+0x318)

    // ---- life cycle (SceneAvatarEdit.cpp)
    static Addr create();
    static Addr createScene();
    static Addr createSceneLayer();                            // template createSceneLayer<SceneAvatarEdit>
    static SceneAvatarEdit *destroy(SceneAvatarEdit *self);    // ~SceneAvatarEdit (D2)
    static void destroyDelete(SceneAvatarEdit *self);          // deleting destructor (D0)
    static SceneAvatarEdit *destroyThunk(Addr base2);          // D1 via the second base
    static void destroyDeleteThunk(Addr base2);                // D0 via the second base
    static uint32_t init(SceneAvatarEdit *self);
    static void update(SceneAvatarEdit *self, float dt);
    static void runInAnimation(SceneAvatarEdit *self);
    static void runInAnimationThunk(Addr base2);
    static void runOutAnimationMain(SceneAvatarEdit *self, Addr done);   // std::function<void()> by value
    static void runOutAnimationMainThunk(Addr base2, Addr done);

    // ---- lists and tabs (SceneAvatarEdit_Lists.cpp)
    static void initAvatarPartsList(SceneAvatarEdit *self);
    static void initDebugAvatarPartsList(SceneAvatarEdit *self);
    static void initTabButton(SceneAvatarEdit *self);
    static void initMyCordinateButton(SceneAvatarEdit *self);
    static void changeMyCordinate(SceneAvatarEdit *self, int32_t index);
    static void changeTab(SceneAvatarEdit *self, uint32_t tab);
    static void settingDefaultCoordinate(SceneAvatarEdit *self, Coordinate *coordinate, uint32_t male);
    static int32_t getEquipKeybladeID(SceneAvatarEdit *self, uint32_t show);
    static void updateGenderPanel(SceneAvatarEdit *self);
    static void updateCost(SceneAvatarEdit *self);
    static void updateTabNew(SceneAvatarEdit *self, int32_t tab);
    static void createFilterdAccessoryIDs(Addr out, SceneAvatarEdit *self);   // sret std::vector<int>
    static void genderFiliteringIDs(Addr out, SceneAvatarEdit *self, int32_t tab);
    static void createAccessoryIDs(Addr out, SceneAvatarEdit *self);
    static void changeGender(SceneAvatarEdit *self, uint32_t male);
    static void generateAccessories(SceneAvatarEdit *self, int32_t partsId, Addr out, uint32_t female);
    static void setAccessory(SceneAvatarEdit *self, int32_t partsId, uint32_t add, uint32_t female);

    // ---- popups and saving (SceneAvatarEdit_Popups.cpp)
    static void openAccessoriesFilterPopup(SceneAvatarEdit *self);
    static void openCreateCheckPopup(SceneAvatarEdit *self);
    static void openSavePopup(SceneAvatarEdit *self);
    static void saveMyCoordinate(SceneAvatarEdit *self, Addr callback);   // std::function<void(bool)> by value
    static void finalize(SceneAvatarEdit *self, Addr callback);
    static void backScene(SceneAvatarEdit *self);
};
RW_LAYOUT(SceneAvatarEdit, vtable2, 0x288);
RW_LAYOUT(SceneAvatarEdit, avatarActor, 0x2AC);
RW_LAYOUT(SceneAvatarEdit, newTabsMale, 0x2C8);
RW_LAYOUT(SceneAvatarEdit, isCreate, 0x2E0);
RW_LAYOUT(SceneAvatarEdit, scrollView, 0x2F0);
RW_LAYOUT(SceneAvatarEdit, currentCoordinate, 0x30C);
RW_LAYOUT(SceneAvatarEdit, defaultCoordinate, 0x310);
RW_LAYOUT(SceneAvatarEdit, isTutorialEdit, 0x340);
RW_SIZE(SceneAvatarEdit::Coordinate, 48);
RW_SIZE(SceneAvatarEdit, SceneAvatarEdit::SIZE);
