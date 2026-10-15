// AppState's nested types: sort / filter / option settings persisted in the SaveBin, the version triple, and the
// SafeBuffer-backed records (see AppStateTypes.hpp).
#include "AppStateTypes.hpp"

namespace appstate {

namespace {

// StringBuffer is 1028 bytes (char[1024] + length); the key is its first bytes.
constexpr uint32_t STRING_BUFFER_SIZE = 0x404;

// Format a save key into the frame's StringBuffer: StringBuffer(buf, fmt, a, b).
uint32_t formatKey(const guest::Frame &f, uint32_t fmt, uint32_t a, uint32_t b = 0) {
    uint32_t buf = f.addr(0);
    rw::call(STRING_BUFFER_CTOR, {buf, fmt, a, b});
    return buf;
}

// Empty std::string (the three words a default-constructed libc++ string holds).
void clearString(guest::String *s) {
    rw::store32(s->raw + 0, 0);
    rw::store32(s->raw + 4, 0);
    rw::store32(s->raw + 8, 0);
}

// string.assign(text, strlen(text))
void assignString(guest::String *s, uint32_t text) {
    uint32_t len = rw::call(STRLEN, {text});
    rw::call(STRING_ASSIGN, {rw::guest_addr(s), text, len});
}

// Save keys of the flag bytes, in field order (field k is at byte offset k).
constexpr uint32_t MEDAL_KEYS[23] = {
    0xDFC907,  // tabIndex       (u8)
    0xDFC910,  // sortIndex      (u8)
    0xDFC91A,  // isAsc
    0xDFC920,  // isPower
    0xDFC928,  // isSpeed
    0xDFC930,  // isMagic
    0xDFC938,  // isLight
    0xDFC940,  // isDark
    0xDFC947,  // isRare1
    0xDFC94F,  // isRare2
    0xDFC957,  // isRare3
    0xDFC95F,  // isRare4
    0xDFC967,  // isRare5
    0xDFC96F,  // isRare6
    0xDFC977,  // isAttack
    0xDFC980,  // isLargeExp
    0xDFC98B,  // isPowerUp
    0xDFC995,  // isCostMinus
    0xDFC9A1,  // isEvolve
    0xDFC9AA,  // isLargeMoney
    0xDFB60E,  // isFavorite
    0xDFC9B7,  // isSetSkill
    0xDFC9C2,  // isNotSetSkill
};
constexpr uint32_t MEDAL_U8_FIELDS = 2;  // tabIndex, sortIndex are unsigned; the rest signed

constexpr uint32_t AVATAR_BOARD_KEYS[14] = {
    0xDFC9D0,  // isFilterOn
    0xDFC9DB,  // isComp
    0xDFC9E2,  // isNotComp
    0xDFC9EC,  // isMan
    0xDFC9F2,  // isWoman
    0xDFC9FA,  // isAvatar
    0xDFCA03,  // isAp
    0xDFCA08,  // isHp
    0xDFCA0D,  // isCost
    0xDFCA14,  // isMedal
    0xDFCA1C,  // isSkill
    0xDFCA24,  // isKeybread
    0xDFCA2F,  // isMaterial
    0xDFCA3A,  // isTitle
};

constexpr uint32_t ADVENTURE_SELECT_KEYS[8] = {
    0xDFC9D0,  // isFilterOn
    0xDFC9DB,  // isComp
    0xDFC9E2,  // isNotComp
    0xDFCA42,  // isHaveStory
    0xDFCA4E,  // isNotHaveStory
    0xDFC920,  // isPower
    0xDFC928,  // isSpeed
    0xDFC930,  // isMagic
};

constexpr uint32_t KEY_BGM_VOLUME = 0xDFCA5D;            // "bgmVolume"
constexpr uint32_t KEY_SE_VOLUME = 0xDFCA67;             // "seVolume"
constexpr uint32_t KEY_IS_NOTIFY_RECOVER_AP = 0xDFCA70;  // "isNotifyRecoverAp"
constexpr uint32_t KEY_IS_NOTIFY_HELP_RAID = 0xDFCA82;   // "isNotifyHelpRaid"
constexpr uint32_t KEY_IS_NOTIFY_EVENT = 0xDFCA93;       // "isNotifyEvent"

constexpr uint32_t ONE_F_BITS = 0x3F800000;  // 1.0f

// Load / save `count` flag bytes at obj + k under the keys fmt(prefix, keys[k]); the first `u8Count` are unsigned.
void loadFlags(uint32_t saveBin, uint32_t obj, uint32_t fmt, uint32_t prefix, const uint32_t *keys, uint32_t count,
               uint32_t u8Count) {
    guest::Frame f(STRING_BUFFER_SIZE + 4);
    for (uint32_t k = 0; k < count; k++) {
        uint32_t key = formatKey(f, fmt, prefix, keys[k]);
        rw::call(k < u8Count ? SAVEBIN_GET_U8 : SAVEBIN_GET_S8, {saveBin, key, obj + k});
    }
}
void saveFlags(uint32_t saveBin, uint32_t obj, uint32_t fmt, uint32_t prefix, const uint32_t *keys, uint32_t count,
               uint32_t u8Count) {
    guest::Frame f(STRING_BUFFER_SIZE + 4);
    for (uint32_t k = 0; k < count; k++) {
        uint32_t key = formatKey(f, fmt, prefix, keys[k]);
        rw::call(k < u8Count ? SAVEBIN_SET_U8 : SAVEBIN_SET_S8, {saveBin, key, obj + k});
    }
}

// __fixunssfsi (libgcc soft-float): float -> unsigned; negative and NaN give 0, too large gives 0xFFFFFFFF.
uint32_t floatToUnsigned(float v) {
    uint32_t bits = rw::fbits(v);
    uint32_t twice = bits << 1;
    if ((bits & 0x80000000u) || twice < 0x7F000000u) return 0;
    int32_t shift = 158 - int32_t(twice >> 24);
    if (shift >= 0) return ((bits << 8) | 0x80000000u) >> shift;
    if (shift == -97 && (bits << 9) != 0) return 0;  // NaN
    return 0xFFFFFFFFu;
}

// Volume stored as 0..10000 -> 0..1 (clamped)
uint32_t volumeFromSaved(uint16_t saved) {
    float v = float(saved) / OptionState::VOLUME_SCALE;
    float clampedLow = (v < 0.0f) ? 0.0f : v;
    return (clampedLow > 1.0f) ? ONE_F_BITS : rw::fbits(clampedLow);
}

// Volume 0..1 -> 0..10000 (clamped)
uint16_t volumeToSaved(float volume) {
    float v = (volume < 0.0f) ? 0.0f : volume;
    bool above = v > 1.0f;
    uint32_t scaled = floatToUnsigned(v * OptionState::VOLUME_SCALE);
    return uint16_t(above ? 10000u : scaled);
}

}  // namespace

// ------------------------------------------------------------------------------------------ MedalSortFilterState
MedalSortFilterState *MedalSortFilterState::construct(MedalSortFilterState *self, uint32_t name) {
    clearString(&self->name);
    rw::call(AEABI_MEMSET, {rw::guest_addr(self), 0x17, 0});  // every flag byte (0..22)
    assignString(&self->name, name);
    // default sort key: the number in text 2/100100003
    guest::Frame f(8);
    rw::call(TEXTDATA_GET, {f.addr(0), 2, 100100003});
    uint32_t entry = f.at<guest::SharedPtr>(0)->obj;
    uint8_t sortIndex = uint8_t(rw::call(ATOI, {rw::load<uint32_t>(entry + 4)}));
    uint32_t ctrl = f.at<guest::SharedPtr>(0)->ctrl;
    if (ctrl) guest::release(ctrl);
    self->sortIndex = sortIndex;
    return self;
}
RW_FUNCTION(0x4EC120, "AppState::MedalSortFilterState::MedalSortFilterState(const char*)",
            MedalSortFilterState::construct);

uint32_t MedalSortFilterState::load(MedalSortFilterState *self, uint32_t saveBin) {
    loadFlags(saveBin, rw::guest_addr(self), FMT_MEDAL_SORT_FILTER, self->name.data(), MEDAL_KEYS, 23,
              MEDAL_U8_FIELDS);
    return 1;
}
RW_FUNCTION(0x4E7970, "AppState::MedalSortFilterState::load", MedalSortFilterState::load);

uint32_t MedalSortFilterState::save(MedalSortFilterState *self, uint32_t saveBin) {
    saveFlags(saveBin, rw::guest_addr(self), FMT_MEDAL_SORT_FILTER, self->name.data(), MEDAL_KEYS, 23,
              MEDAL_U8_FIELDS);
    return 1;
}
RW_FUNCTION(0x4E9688, "AppState::MedalSortFilterState::save", MedalSortFilterState::save);

// ------------------------------------------------------------------------------------------ AvatarBoardFilterState
AvatarBoardFilterState *AvatarBoardFilterState::construct(AvatarBoardFilterState *self, uint32_t name) {
    clearString(&self->name);
    self->isTitle = 0;
    self->isMaterial = 0;
    self->isKeybread = 0;
    self->isSkill = 0;
    self->isMedal = 0;
    self->isCost = 0;
    self->isHp = 0;
    self->isAp = 0;
    self->isAvatar = 0;
    self->isWoman = 0;
    self->isMan = 0;
    self->isNotComp = 0;
    self->isComp = 0;
    self->isFilterOn = 0;
    assignString(&self->name, name);
    return self;
}
RW_FUNCTION(0x4EC3A8, "AppState::AvatarBoardFilterState::AvatarBoardFilterState(const char*)",
            AvatarBoardFilterState::construct);

uint32_t AvatarBoardFilterState::load(AvatarBoardFilterState *self, uint32_t saveBin) {
    loadFlags(saveBin, rw::guest_addr(self), FMT_NAME_FIELD, self->name.data(), AVATAR_BOARD_KEYS, 14, 0);
    return 1;
}
RW_FUNCTION(0x4E7ECC, "AppState::AvatarBoardFilterState::load", AvatarBoardFilterState::load);

uint32_t AvatarBoardFilterState::save(AvatarBoardFilterState *self, uint32_t saveBin) {
    saveFlags(saveBin, rw::guest_addr(self), FMT_NAME_FIELD, self->name.data(), AVATAR_BOARD_KEYS, 14, 0);
    return 1;
}
RW_FUNCTION(0x4E9DD4, "AppState::AvatarBoardFilterState::save", AvatarBoardFilterState::save);

// ------------------------------------------------------------------------------------------ AdventureSelectFilterState
AdventureSelectFilterState *AdventureSelectFilterState::construct(AdventureSelectFilterState *self, uint32_t name) {
    rw::call(AEABI_MEMSET, {rw::guest_addr(self), 0x14, 0});  // flags and the (empty) string
    assignString(&self->name, name);
    return self;
}
RW_FUNCTION(0x4EC414, "AppState::AdventureSelectFilterState::AdventureSelectFilterState(const char*)",
            AdventureSelectFilterState::construct);

uint32_t AdventureSelectFilterState::load(AdventureSelectFilterState *self, uint32_t saveBin) {
    loadFlags(saveBin, rw::guest_addr(self), FMT_NAME_FIELD, self->name.data(), ADVENTURE_SELECT_KEYS, 8, 0);
    return 1;
}
RW_FUNCTION(0x4E8210, "AppState::AdventureSelectFilterState::load", AdventureSelectFilterState::load);

uint32_t AdventureSelectFilterState::save(AdventureSelectFilterState *self, uint32_t saveBin) {
    saveFlags(saveBin, rw::guest_addr(self), FMT_NAME_FIELD, self->name.data(), ADVENTURE_SELECT_KEYS, 8, 0);
    return 1;
}
RW_FUNCTION(0x4EA118, "AppState::AdventureSelectFilterState::save", AdventureSelectFilterState::save);

// ------------------------------------------------------------------------------------------ OptionState
// Defaults: full volume, every notification on.
OptionState *OptionState::reset(OptionState *self) {
    rw::set<uint32_t>(self, 0, ONE_F_BITS);  // bgmVolume = 1.0f
    rw::set<uint32_t>(self, 4, ONE_F_BITS);  // seVolume = 1.0f
    self->isNotifyHelpRaid = 1;
    self->isNotifyRecoverAp = 1;
    self->isNotifyEvent = 1;
    return self;
}
RW_FUNCTION(0x4E8DB4, "AppState::OptionState::reset", OptionState::reset);

OptionState *OptionState::construct(OptionState *self) { return reset(self); }
RW_FUNCTION(0x4EC464, "AppState::OptionState::OptionState", OptionState::construct);

// Any missing key resets every option to its default and returns 0.
uint32_t OptionState::load(OptionState *self, uint32_t saveBin) {
    guest::Frame f(0x20 + STRING_BUFFER_SIZE);
    uint32_t saved = f.addr(0x1A);  // unsigned short read from the save
    uint32_t key = f.addr(0x20);
    rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_BGM_VOLUME});
    if (rw::call(SAVEBIN_GET_U16, {saveBin, key, saved}) != 0) {
        rw::set<uint32_t>(self, 0, volumeFromSaved(rw::load<uint16_t>(saved)));
        rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_SE_VOLUME});
        if (rw::call(SAVEBIN_GET_U16, {saveBin, key, saved}) == 1) {
            rw::set<uint32_t>(self, 4, volumeFromSaved(rw::load<uint16_t>(saved)));
            rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_IS_NOTIFY_RECOVER_AP});
            if (rw::call(SAVEBIN_GET_S8, {saveBin, key, rw::guest_addr(self) + 8}) == 1) {
                rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_IS_NOTIFY_HELP_RAID});
                if (rw::call(SAVEBIN_GET_S8, {saveBin, key, rw::guest_addr(self) + 9}) == 1) {
                    rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_IS_NOTIFY_EVENT});
                    if (rw::call(SAVEBIN_GET_S8, {saveBin, key, rw::guest_addr(self) + 10}) != 0) return 1;
                }
            }
        }
    }
    reset(self);
    return 0;
}
RW_FUNCTION(0x4E8418, "AppState::OptionState::load", OptionState::load);

uint32_t OptionState::save(const OptionState *self, uint32_t saveBin) {
    guest::Frame f(0x24 + STRING_BUFFER_SIZE);
    uint32_t saved = f.addr(0x22);  // unsigned short written to the save
    uint32_t key = f.addr(0x24);
    uint16_t bgm = volumeToSaved(self->bgmVolume);
    rw::set<uint16_t>(rw::host_ptr<uint8_t>(saved), 0, bgm);
    rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_BGM_VOLUME});
    rw::call(SAVEBIN_SET_U16, {saveBin, key, saved});
    uint16_t se = volumeToSaved(self->seVolume);
    rw::set<uint16_t>(rw::host_ptr<uint8_t>(saved), 0, se);
    rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_SE_VOLUME});
    rw::call(SAVEBIN_SET_U16, {saveBin, key, saved});
    uint32_t obj = rw::guest_addr(self);
    rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_IS_NOTIFY_RECOVER_AP});
    rw::call(SAVEBIN_SET_S8, {saveBin, key, obj + 8});
    rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_IS_NOTIFY_HELP_RAID});
    rw::call(SAVEBIN_SET_S8, {saveBin, key, obj + 9});
    rw::call(STRING_BUFFER_CTOR, {key, FMT_OPTION_STATE, KEY_IS_NOTIFY_EVENT});
    rw::call(SAVEBIN_SET_S8, {saveBin, key, obj + 10});
    return 1;
}
RW_FUNCTION(0x4E9BE4, "AppState::OptionState::save", OptionState::save);

// ------------------------------------------------------------------------------------------ Version
// Parse "major.minor.patch" (strtol of each part); valid only when all three were found.
Version *Version::construct(Version *self, uint32_t text) {
    for (uint32_t part = 0;; part++) {
        int32_t separator = rw::load<int8_t>(VERSION_SEPARATORS + part);  // '.', '.', '\0'
        uint32_t next = rw::call(STRCHR, {text, uint32_t(separator)});
        if (next == 0) {
            self->valid = 0;
            return self;
        }
        int32_t value = int32_t(rw::call(STRTOL, {text, 0, 10}));
        if (part == 2) {
            self->patch = value;
            self->valid = 1;
            return self;
        }
        if (part == 1) {
            self->minor = value;
        } else if (part == 0) {
            self->major = value;
        } else {
            self->valid = 0;  // unreachable
            return self;
        }
        text = next + 1;
    }
}
RW_FUNCTION(0x4EC09C, "AppState::Version::Version(const char*)", Version::construct);

uint32_t Version::greaterThan(const Version *self, const Version *other) {
    if (self->valid == 0 || other->valid == 0) return 0;
    if (self->major != other->major) return self->major > other->major ? 1 : 0;
    if (self->minor != other->minor) return self->minor > other->minor ? 1 : 0;
    return self->patch > other->patch ? 1 : 0;
}
RW_FUNCTION(0x4EAEDC, "AppState::Version::operator>", Version::greaterThan);

// ------------------------------------------------------------------------------------------ SafeBuffer records
LoginInfo *LoginInfo::construct(LoginInfo *self) {
    SafeBuffer::construct(self, DATA_SIZE, 1);
    self->vtable = VTABLE;
    SafeBuffer::zerofill(self);
    return self;
}
RW_FUNCTION(0x4EA9A0, "AppState::LoginInfo::LoginInfo", LoginInfo::construct);

BRIDGEInfo *BRIDGEInfo::construct(BRIDGEInfo *self) {
    SafeBuffer::construct(self, DATA_SIZE, 1);
    self->vtable = VTABLE;
    SafeBuffer::zerofill(self);
    return self;
}
RW_FUNCTION(0x4EA9E8, "AppState::BRIDGEInfo::BRIDGEInfo", BRIDGEInfo::construct);

// Deleting destructors: ~SafeBuffer, then operator delete.
void LoginInfo::deletingDestroy(LoginInfo *self) { guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self))); }
RW_FUNCTION(0x4EC644, "AppState::LoginInfo::~LoginInfo (deleting)", LoginInfo::deletingDestroy);

void BRIDGEInfo::deletingDestroy(BRIDGEInfo *self) { guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self))); }
RW_FUNCTION(0x4EC650, "AppState::BRIDGEInfo::~BRIDGEInfo (deleting)", BRIDGEInfo::deletingDestroy);

void SkillSortFilterState::deletingDestroy(SkillSortFilterState *self) {
    guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self)));
}
RW_FUNCTION(0x4EDC7C, "AppState::SkillSortFilterState::~SkillSortFilterState (deleting)",
            SkillSortFilterState::deletingDestroy);

void KeyBladeSortFilterState::deletingDestroy(KeyBladeSortFilterState *self) {
    guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self)));
}
RW_FUNCTION(0x4EDD7C, "AppState::KeyBladeSortFilterState::~KeyBladeSortFilterState (deleting)",
            KeyBladeSortFilterState::deletingDestroy);

void PersonalInfo::deletingDestroy(PersonalInfo *self) {
    guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self)));
}
RW_FUNCTION(0x4EE594, "AppState::PersonalInfo::~PersonalInfo (deleting)", PersonalInfo::deletingDestroy);

}  // namespace appstate
