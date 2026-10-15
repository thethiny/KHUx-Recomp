// AppState's nested types: the per-screen sort / filter settings (medal lists, avatar board, adventure select),
// the sound / notification options, the app version triple, and the small SafeBuffer-backed records (login,
// BRIDGE, personal info, keyblade / skill sort state). The settings are persisted in the save file (SaveBin)
// under text keys: "medalsortfilter_<name>_<field>", "<name>_<field>", "option_state_<field>".
// (AppState itself is in AppState.hpp; this file does not depend on it.)
#pragma once

#include "Guest.hpp"
#include "SafeBuffer.hpp"

namespace appstate {

// ------------------------------------------------------------------------------------------ guest functions
constexpr uint32_t STRING_BUFFER_CTOR = 0x49140D;   // StringBuffer::StringBuffer(const char* fmt, ...) (1028 bytes)
constexpr uint32_t SAVEBIN_GET_U8 = 0x4E0335;       // SaveBin::get<unsigned char>(const char*, unsigned char&) -> bool
constexpr uint32_t SAVEBIN_GET_S8 = 0x4EC1D9;       // SaveBin::get<signed char>(const char*, signed char&) -> bool
constexpr uint32_t SAVEBIN_GET_U16 = 0x4EC479;      // SaveBin::get<unsigned short>(const char*, unsigned short&)
constexpr uint32_t SAVEBIN_SET_U8 = 0x4E00FD;       // SaveBin::set<unsigned char>(const char*, const unsigned char&)
constexpr uint32_t SAVEBIN_SET_S8 = 0x4EC28D;       // SaveBin::set<signed char>(const char*, const signed char&)
constexpr uint32_t SAVEBIN_SET_U16 = 0x4EC529;      // SaveBin::set<unsigned short>(const char*, const unsigned short&)
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;         // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t STRLEN = 0x40E08C;               // strlen (import)
constexpr uint32_t ATOI = 0x410A14;                 // atoi (import)
constexpr uint32_t STRCHR = 0x410198;               // strchr (import)
constexpr uint32_t STRTOL = 0x4101A4;               // strtol (import)
constexpr uint32_t STRING_ASSIGN = 0x48D811;        // std::string::assign(const char*, size_t) (sub_48D810)
constexpr uint32_t TEXTDATA_GET = 0x4928AD;         // TextData::get(shared_ptr<entry> out, Type, int id)

// ------------------------------------------------------------------------------------------ key strings
constexpr uint32_t FMT_MEDAL_SORT_FILTER = 0xDFCAB7;  // "medalsortfilter_%s_%s"
constexpr uint32_t FMT_NAME_FIELD = 0xDFCAB1;         // "%s_%s"
constexpr uint32_t FMT_OPTION_STATE = 0xDFCAA1;       // "option_state_%s"
constexpr uint32_t VERSION_SEPARATORS = 0xDFC904;     // ".." + NUL: the separators after major, minor, patch

// AppState::MedalSortFilterState (36 bytes): sort tab / key / order and the filter toggles of one medal list;
// `name` selects the save keys ("medalsortfilter_<name>_<field>").
struct MedalSortFilterState {
    rw::Field<uint8_t> tabIndex;      // +0  "tabIndex"   (u8)
    rw::Field<uint8_t> sortIndex;     // +1  "sortIndex"  (u8; default: text 2/100100003 as a number)
    rw::Field<int8_t> isAsc;          // +2  "isAsc"
    rw::Field<int8_t> isPower;        // +3  medal attribute filters
    rw::Field<int8_t> isSpeed;        // +4
    rw::Field<int8_t> isMagic;        // +5
    rw::Field<int8_t> isLight;        // +6
    rw::Field<int8_t> isDark;         // +7
    rw::Field<int8_t> isRare1;        // +8  rarity filters
    rw::Field<int8_t> isRare2;        // +9
    rw::Field<int8_t> isRare3;        // +10
    rw::Field<int8_t> isRare4;        // +11
    rw::Field<int8_t> isRare5;        // +12
    rw::Field<int8_t> isRare6;        // +13
    rw::Field<int8_t> isAttack;       // +14 medal kind filters
    rw::Field<int8_t> isLargeExp;     // +15
    rw::Field<int8_t> isPowerUp;      // +16
    rw::Field<int8_t> isCostMinus;    // +17
    rw::Field<int8_t> isEvolve;       // +18
    rw::Field<int8_t> isLargeMoney;   // +19
    rw::Field<int8_t> isFavorite;     // +20
    rw::Field<int8_t> isSetSkill;     // +21
    rw::Field<int8_t> isNotSetSkill;  // +22
    const uint8_t pad_17[1];                // +23 (never written by the constructor)
    guest::String name;               // +24 key prefix

    static MedalSortFilterState *construct(MedalSortFilterState *self, uint32_t name);
    static uint32_t load(MedalSortFilterState *self, uint32_t saveBin);
    static uint32_t save(MedalSortFilterState *self, uint32_t saveBin);
};
RW_LAYOUT(MedalSortFilterState, isNotSetSkill, 22);
RW_LAYOUT(MedalSortFilterState, name, 24);
RW_SIZE(MedalSortFilterState, 36);

// AppState::AvatarBoardFilterState (28 bytes): avatar board filter toggles; keys "<name>_<field>".
struct AvatarBoardFilterState {
    rw::Field<int8_t> isFilterOn;     // +0
    rw::Field<int8_t> isComp;         // +1  completed / not completed
    rw::Field<int8_t> isNotComp;      // +2
    rw::Field<int8_t> isMan;          // +3
    rw::Field<int8_t> isWoman;        // +4
    rw::Field<int8_t> isAvatar;       // +5  reward kinds
    rw::Field<int8_t> isAp;           // +6
    rw::Field<int8_t> isHp;           // +7
    rw::Field<int8_t> isCost;         // +8
    rw::Field<int8_t> isMedal;        // +9
    rw::Field<int8_t> isSkill;        // +10
    rw::Field<int8_t> isKeybread;     // +11 (sic: keyblade)
    rw::Field<int8_t> isMaterial;     // +12
    rw::Field<int8_t> isTitle;        // +13
    const uint8_t pad_0E[2];                // +14
    guest::String name;               // +16 key prefix

    static AvatarBoardFilterState *construct(AvatarBoardFilterState *self, uint32_t name);
    static uint32_t load(AvatarBoardFilterState *self, uint32_t saveBin);
    static uint32_t save(AvatarBoardFilterState *self, uint32_t saveBin);
};
RW_LAYOUT(AvatarBoardFilterState, isTitle, 13);
RW_LAYOUT(AvatarBoardFilterState, name, 16);
RW_SIZE(AvatarBoardFilterState, 28);

// AppState::AdventureSelectFilterState (20 bytes): adventure (quest) select filter toggles; keys "<name>_<field>".
struct AdventureSelectFilterState {
    rw::Field<int8_t> isFilterOn;     // +0
    rw::Field<int8_t> isComp;         // +1
    rw::Field<int8_t> isNotComp;      // +2
    rw::Field<int8_t> isHaveStory;    // +3
    rw::Field<int8_t> isNotHaveStory; // +4
    rw::Field<int8_t> isPower;        // +5
    rw::Field<int8_t> isSpeed;        // +6
    rw::Field<int8_t> isMagic;        // +7
    guest::String name;               // +8  key prefix

    static AdventureSelectFilterState *construct(AdventureSelectFilterState *self, uint32_t name);
    static uint32_t load(AdventureSelectFilterState *self, uint32_t saveBin);
    static uint32_t save(AdventureSelectFilterState *self, uint32_t saveBin);
};
RW_LAYOUT(AdventureSelectFilterState, name, 8);
RW_SIZE(AdventureSelectFilterState, 20);

// AppState::OptionState (12 bytes): sound volumes (0..1, saved as 0..10000) and notification switches;
// keys "option_state_<field>".
struct OptionState {
    rw::Field<float> bgmVolume;          // +0  "bgmVolume"
    rw::Field<float> seVolume;           // +4  "seVolume"
    rw::Field<int8_t> isNotifyRecoverAp; // +8
    rw::Field<int8_t> isNotifyHelpRaid;  // +9
    rw::Field<int8_t> isNotifyEvent;     // +10
    const uint8_t pad_0B[1];                   // +11

    static constexpr float VOLUME_SCALE = 10000.0f;

    static OptionState *construct(OptionState *self);
    static OptionState *reset(OptionState *self);
    static uint32_t load(OptionState *self, uint32_t saveBin);
    static uint32_t save(const OptionState *self, uint32_t saveBin);
};
RW_LAYOUT(OptionState, isNotifyEvent, 10);
RW_SIZE(OptionState, 12);

// AppState::Version (13+ bytes): "major.minor.patch".
struct Version {
    rw::Field<int32_t> major;   // +0
    rw::Field<int32_t> minor;   // +4
    rw::Field<int32_t> patch;   // +8
    rw::Field<uint8_t> valid;   // +12 all three parts parsed

    static Version *construct(Version *self, uint32_t text);
    static uint32_t greaterThan(const Version *self, const Version *other);
};
RW_LAYOUT(Version, valid, 12);

// SafeBuffer-backed records (obfuscated values). Data sizes: LoginInfo 44 bytes, BRIDGEInfo 32,
// PersonalInfo 16, SkillSortFilterState / KeyBladeSortFilterState 8 (from their constructors / make_shared).
struct LoginInfo : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7FB6C;   // _ZTVN8AppState9LoginInfoE + 8
    static constexpr uint32_t DATA_SIZE = 0x2C;
    static LoginInfo *construct(LoginInfo *self);
    static void deletingDestroy(LoginInfo *self);
};
struct BRIDGEInfo : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7FB7C;   // _ZTVN8AppState10BRIDGEInfoE + 8
    static constexpr uint32_t DATA_SIZE = 0x20;
    static BRIDGEInfo *construct(BRIDGEInfo *self);
    static void deletingDestroy(BRIDGEInfo *self);
};
struct PersonalInfo : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7FCF4;   // _ZTVN8AppState12PersonalInfoE + 8
    static void deletingDestroy(PersonalInfo *self);
};
struct SkillSortFilterState : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7FBCC;   // _ZTVN8AppState20SkillSortFilterStateE + 8
    static void deletingDestroy(SkillSortFilterState *self);
};
struct KeyBladeSortFilterState : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7FC10;   // _ZTVN8AppState23KeyBladeSortFilterStateE + 8
    static void deletingDestroy(KeyBladeSortFilterState *self);
};

}  // namespace appstate
