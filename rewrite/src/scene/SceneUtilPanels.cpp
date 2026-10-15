// SceneUtil: keyblade / medal panels and number formatting.
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D;
constexpr uint32_t KeyBladeDataManager_getKeyBladeImagePath = 0x76B809;   // (mgr, shared_ptr<UserKeyBladeData>)
constexpr uint32_t Keyblade_create = 0x783F9D;            // master::Keyblade::create(out, id)
constexpr uint32_t Medal_create = 0x785871;               // master::Medal::create(out, id)
constexpr uint32_t Sprite_createWithFile = 0x8A637D;      // Sprite::create(const string&)
constexpr uint32_t String_concatCStr = 0x461FCD;          // std::operator+(const char*, const string&) (sret)

// UserKeyBladeData: the keyblade's SafeBuffer at +0x20
constexpr uint32_t UserKeyBlade_data = 0x20;
// keyblade SafeBuffer positions
constexpr uint32_t KB_NAME = 0x04, KB_NAME_LEN = 0x41;
constexpr uint32_t KB_ABILITY = 0x45, KB_ABILITY_LEN = 0x81;
constexpr uint32_t KB_SLOT_COUNT = 0xDC;                  // attack slots used
constexpr uint32_t KB_POWERED_COUNT = 0xE4;               // slots compared against the master row
constexpr uint32_t KB_SLOT_TYPES = 0xE8;                  // int[5]: attribute type per slot
constexpr uint32_t KB_SLOT_POWER = 0x110;                 // int[5]: multiplier per slot (x10000)

// UserMedalData: the medal row SafeBuffer at +0x58 (shared_ptr: object, control block)
constexpr uint32_t UserMedal_row = 0x58;
constexpr uint32_t MEDAL_MAX_RARE = 0x470, MEDAL_EVOLVES = 0x4D8, MEDAL_EVOLVES_TO = 0x4DC;

constexpr uint32_t STR_KeyBlade_DeckSet = 0xDF87FC, STR_KeyBlade_Name_Label = 0xDF880D, STR_s = 0xDF8821;
constexpr uint32_t STR_Ability_Label = 0xDF8824, STR_KeyBlade_Dummy = 0xDF8832, STR_ATKd = 0xDF8841;
constexpr uint32_t STR_TypeUP_x = 0xDF8847, STR_TypeUP_Lavel = 0xDF8850, STR_TypeUP_Lavel_Label = 0xDF885D;
constexpr uint32_t STR_KeyBlade_Genus = 0xDF8870, STR_NumEffect = 0xDF887F, STR_Upright_or_Reverse = 0xDF8889;
constexpr uint32_t STR_TypeNone = 0xDF889C;    // "cocostudio/publish/KeyBlade_type_none.png" (41)
constexpr uint32_t STR_Type01 = 0xDF88C6;      // "cocostudio/publish/KeyBlade_type_01.png" (39)
constexpr uint32_t STR_Type02 = 0xDF88EE;
constexpr uint32_t STR_Type03 = 0xDF8916;
constexpr uint32_t STR_TypeLight = 0xDF893E;   // "cocostudio/publish/KeyBlade_type_light.png" (42)
constexpr uint32_t STR_TypeDark = 0xDF8969;    // "cocostudio/publish/KeyBlade_type_dark.png" (41)
constexpr uint32_t STR_01_1f = 0xDF8993;       // "%01.1f"
constexpr uint32_t STR_RareStar = 0xDF899A;    // "img/ui/Rare_Medal_Star01.png" (28)
constexpr uint32_t STR_RareStarFullS = 0xDF89B7;   // "img/ui/Rare_Medal_Star_Full_S.png" (33)
constexpr uint32_t STR_RareStarFull = 0xDF89D9;    // "img/ui/Rare_Medal_Star_Full.png" (31)
constexpr uint32_t STR_RareStarBlank = 0xDF89F9;   // "img/ui/Rare_Medal_Star_Blank.png" (32)
constexpr uint32_t STR_Empty = 0xDF8A1A;       // ""
constexpr uint32_t STR_Comma03llu = 0xDF8A1B;  // ",%03llu"
constexpr uint32_t STR_llu = 0xDF8A23;         // "%llu"

void loadTexture(uint32_t image, uint32_t path, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, path, len);
    call(fn::ImageView_loadTexture, {image, s, 0});
    strFree(s);
}

// attribute icon per slot type: genus image, and the light/dark overlay (types 5-10)
struct SlotType {
    uint32_t genus, genusLen, overlay, overlayLen;
};
const SlotType SLOT_TYPES[10] = {
    {STR_TypeNone, 41, 0, 0},  {STR_Type01, 39, 0, 0},         {STR_Type02, 39, 0, 0},
    {STR_Type03, 39, 0, 0},    {STR_Type01, 39, STR_TypeLight, 42}, {STR_Type02, 39, STR_TypeLight, 42},
    {STR_Type03, 39, STR_TypeLight, 42}, {STR_Type01, 39, STR_TypeDark, 41}, {STR_Type02, 39, STR_TypeDark, 41},
    {STR_Type03, 39, STR_TypeDark, 41},
};
}  // namespace

// Fill a keyblade panel: name, ability, image and the five attack slots (type icon and multiplier). With
// `compare` == 1 the slots stronger than the master row `keybladeId` show the "up" effect.
void SceneUtil::updateKeyBladePanel(Addr panel, Addr keyblade, uint32_t compare, int32_t keybladeId) {
    uint32_t deckSet = findNode(panel, STR_KeyBlade_DeckSet, 16);
    if (deckSet) vcall(deckSet, node::setVisible, {deckSet, 0});
    uint32_t kb = ld32(ld32(keyblade) + UserKeyBlade_data);
    {
        uint32_t name = findNode(panel, STR_KeyBlade_Name_Label, 19);
        setShadow(name);
        Frame f;
        uint32_t raw = safeBytes(f, kb, KB_NAME, KB_NAME_LEN);
        setTextC(name, stringBuffer(f, {STR_s, raw}));
    }
    {
        uint32_t ability = findNode(panel, STR_Ability_Label, 13);
        setShadow(ability);
        Frame f;
        uint32_t raw = safeBytes(f, ld32(ld32(keyblade) + UserKeyBlade_data), KB_ABILITY, KB_ABILITY_LEN);
        setTextC(ability, stringBuffer(f, {STR_s, raw}));
    }
    {
        uint32_t image = findNode(panel, STR_KeyBlade_Dummy, 14);
        uint32_t mgr = call(KeyBladeDataManager_getInstance, {});
        Frame f;
        uint32_t copy = f.alloc(8);                    // the shared_ptr argument, by value
        st32(copy, ld32(keyblade));
        uint32_t ctl = ld32(keyblade + 4);
        st32(copy + 4, ctl);
        if (ctl) call(fn::shared_add, {ctl});
        uint32_t path = call(KeyBladeDataManager_getKeyBladeImagePath, {mgr, copy});
        uint32_t s = newStr(f, path);
        call(fn::ImageView_loadTexture, {image, s, 0});
        strFree(s);
        sharedRelease(copy);
    }

    // which slots beat the master row
    uint8_t powered[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (compare == 1) {
        Frame f;
        uint32_t row = masterRow(f, Keyblade_create, uint32_t(keybladeId));
        for (int32_t k = 0; k < 5; k++) {
            uint32_t data = ld32(ld32(keyblade) + UserKeyBlade_data);
            int32_t count = safe32(data, KB_POWERED_COUNT);
            if (k >= count) {
                powered[k] = 0;            // (k < count) is false
            } else {
                int32_t base = safe32(ld32(row), KB_SLOT_POWER + 4 * uint32_t(k));
                int32_t mine = safe32(data, KB_SLOT_POWER + 4 * uint32_t(k));
                powered[k] = base < mine;
            }
        }
        sharedRelease(row);
    }

    for (int32_t k = 0; k < 5; k++) {
        uint32_t slot;
        {
            Frame f;
            uint32_t buf = stringBuffer(f, {STR_ATKd, uint32_t(k + 1)});
            uint32_t s = newStr(f, buf);
            slot = searchNodeByName(panel, s);
            strFree(s);
        }
        uint32_t x = findNode(slot, STR_TypeUP_x, 8);
        setShadow(x);
        uint32_t level = findNode(slot, STR_TypeUP_Lavel, 12);
        if (!level) level = findNode(slot, STR_TypeUP_Lavel_Label, 18);
        setShadow(level);
        uint32_t genus = findNode(slot, STR_KeyBlade_Genus, 14);
        uint32_t effect = findNode(genus, STR_NumEffect, 9);
        vcall(effect, node::setVisible, {effect, (compare == 1 && powered[k]) ? 1u : 0u});
        uint32_t overlay = findNode(slot, STR_Upright_or_Reverse, 18);
        uint32_t data = ld32(ld32(keyblade) + UserKeyBlade_data);
        if (k >= safe32(data, KB_SLOT_COUNT)) {
            vcall(slot, node::setVisible, {slot, 0});
            continue;
        }
        vcall(slot, node::setVisible, {slot, 1});
        int32_t type = safe32(ld32(ld32(keyblade) + UserKeyBlade_data), KB_SLOT_TYPES + 4 * uint32_t(k));
        if (type >= 1 && type <= 10) {
            const SlotType &t = SLOT_TYPES[type - 1];
            vcall(genus, node::setVisible, {genus, 1});
            vcall(overlay, node::setVisible, {overlay, t.overlay ? 1u : 0u});
            loadTexture(genus, t.genus, t.genusLen);
            if (t.overlay) loadTexture(overlay, t.overlay, t.overlayLen);
        }
        int32_t power = safe32(ld32(ld32(keyblade) + UserKeyBlade_data), KB_SLOT_POWER + 4 * uint32_t(k));
        double shown = double(float(power) / 10000.0f);
        uint64_t bits;
        std::memcpy(&bits, &shown, 8);
        Frame f;
        uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
        call(fn::StringBuffer_ctor, {buf, STR_01_1f, uint32_t(bits), uint32_t(bits >> 32)});
        setTextC(level, buf);
    }
}
RW_FUNCTION(0x460BB4, "SceneUtil::updateKeyBladePanel", SceneUtil::updateKeyBladePanel);

// Rarity stars on a panel: `rare` filled stars (of `maxStars` when given, the rest blank), aligned left /
// centre / right; rarity 6 is one special "full" star.
void SceneUtil::updateMedalRarePanel(Addr panel, int32_t rare, int32_t align, float scale, uint32_t small,
                                     int32_t maxStars, int32_t zOrder) {
    vcall(panel, node::removeAllChildren, {panel});
    Frame f;
    uint32_t star = newStr(f, STR_RareStar, 28);
    float step = scale * (small == 1 ? 24.0f : 39.0f);
    int32_t count = maxStars ? maxStars : rare;
    float total = float(count) * step;
    float left = 0.0f;
    if (align == 2) left = ldf(vcall(panel, node::getContentSize, {panel})) - total;
    else if (align == 1) left = (ldf(vcall(panel, node::getContentSize, {panel})) - total) * 0.5f;

    if (rare == 6) {
        uint32_t sprite;
        {
            Frame g;
            uint32_t s = small == 1 ? newStr(g, STR_RareStarFullS, 33) : newStr(g, STR_RareStarFull, 31);
            sprite = call(Sprite_createWithFile, {s});
            strFree(s);
        }
        vcall(sprite, node::setScale, {sprite, fbits(scale)});
        float panelW = ldf(vcall(panel, node::getContentSize, {panel}));
        float spriteW = ldf(vcall(sprite, node::getContentSize, {sprite}));
        float spriteW2 = ldf(vcall(sprite, node::getContentSize, {sprite}));
        float spriteH = ldf(vcall(sprite, node::getContentSize, {sprite}) + 4);
        Frame g;
        vcall(sprite, node::setPosition,
              {sprite, newVec2(g, (panelW - spriteW) * 0.5f + spriteW2 * 0.5f, (spriteH * 0.5f) * scale)});
        vcall(panel, node::addChildZ, {panel, sprite, uint32_t(zOrder)});
    } else if (count >= 1) {
        float half = step * 0.5f;
        for (int32_t i = 0; i < count; i++) {
            uint32_t sprite;
            if (i < rare || maxStars == 0) {
                sprite = call(Sprite_createWithFile, {star});
            } else {
                Frame g;
                uint32_t s = newStr(g, STR_RareStarBlank, 32);
                sprite = call(Sprite_createWithFile, {s});
                strFree(s);
            }
            float h = ldf(vcall(sprite, node::getContentSize, {sprite}) + 4);
            vcall(sprite, node::setScale, {sprite, fbits(scale)});
            float y = (h * scale) * 0.5f;
            float x = half + (left + step * float(i));
            Frame g;
            vcall(sprite, node::setPosition, {sprite, newVec2(g, x, y)});
            vcall(panel, node::addChildZ, {panel, sprite, uint32_t(zOrder)});
        }
    }
    strFree(star);
}
RW_FUNCTION(0x4619F4, "SceneUtil::updateMedalRarePanel", SceneUtil::updateMedalRarePanel);

// "1,234,567": the number with thousands separators (written to the std::string `out`).
void SceneUtil::getMoneyStr(Addr out, uint64_t value) {
    strInit(out, STR_Empty, 0);
    uint64_t rest = value % 1000;
    // out = text + out, then the moved temporary
    auto prepend = [out](uint32_t text) {
        Frame f;
        uint32_t tmp = f.alloc(12);
        call(String_concatCStr, {tmp, text, out});
        if (strIsLong(out)) {
            st8(ld32(out + 8), 0);
            st32(out + 4, 0);
        } else {
            st8(out + 1, 0);
            st8(out, 0);
        }
        call(fn::string_reserve, {out, 0});
        st32(out, ld32(tmp));
        st32(out + 4, ld32(tmp + 4));
        st32(out + 8, ld32(tmp + 8));
    };
    if (!((value >> 3) < 125)) {           // value >= 1000
        bool more;
        do {
            uint64_t next = value / 1000;
            {
                Frame f;
                uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
                call(fn::StringBuffer_ctor, {buf, STR_Comma03llu, uint32_t(rest), uint32_t(rest >> 32)});
                prepend(buf);
            }
            more = value > 999999;
            rest = next % 1000;
            value = next;
        } while (more);
    }
    Frame f;
    uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
    call(fn::StringBuffer_ctor, {buf, STR_llu, uint32_t(rest), uint32_t(rest >> 32)});
    prepend(buf);
}
RW_FUNCTION(0x461DC4, "SceneUtil::getMoneyStr", SceneUtil::getMoneyStr);

// The highest rarity a medal reaches through its evolutions (follows the evolution chain, at most 100 steps).
int32_t SceneUtil::getMedalMaxRare(Addr medal) {
    uint32_t user = ld32(medal);
    int32_t maxRare = safe32(ld32(user + UserMedal_row), MEDAL_MAX_RARE);
    uint32_t row = ld32(user + UserMedal_row);
    uint32_t ctl = ld32(user + UserMedal_row + 4);
    uint32_t held = 0;
    if (ctl) {
        call(fn::shared_add, {ctl});
        held = ctl;
    }
    for (int32_t steps = -1;;) {
        if (safe32(row, MEDAL_EVOLVES) != 1) break;
        int32_t next = safe32(row, MEDAL_EVOLVES_TO);
        Frame f;
        uint32_t sp = masterRow(f, Medal_create, uint32_t(next));
        uint32_t nextCtl = ld32(sp + 4);
        row = ld32(sp);
        if (held) call(fn::shared_release, {held});
        maxRare = safe32(row, MEDAL_MAX_RARE);
        held = nextCtl;
        if (++steps > 99) break;
    }
    if (held) call(fn::shared_release, {held});
    return maxRare;
}
RW_FUNCTION(0x462048, "SceneUtil::getMedalMaxRare", SceneUtil::getMedalMaxRare);
