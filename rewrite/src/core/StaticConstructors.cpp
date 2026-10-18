// The game's own source files' static constructors (the .init_array entries the host runs at load): the objects
// at namespace scope of Avatar, StageActorManager, the snow particle settings and MasterDataManager's table
// registry, each with its destructor registered through __cxa_atexit. The engine's (cocos2d-x, cocostudio, LWF,
// FreeType) come from upstream source.
#include "Guest.hpp"

namespace {

using rw::call;
using rw::load;
using rw::save;

constexpr uint32_t DSO_HANDLE_GOT = 0xFD604C;      // GOT slot holding &__dso_handle
constexpr uint32_t VEC2_CTOR = 0x8E4B9D;           // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;
constexpr uint32_t VEC2_DTOR_GOT = 0xFD681C;       // &cocos2d::Vec2::~Vec2

uint32_t fbits(float v) {
    uint32_t u;
    memcpy(&u, &v, 4);
    return u;
}
void atexit(uint32_t dtor, uint32_t object) { call(guest::CXA_ATEXIT, {dtor, object, load<uint32_t>(DSO_HANDLE_GOT)}); }

// A constant from a header the game's sources include (internal linkage, so every source file has its own copy
// and fills it in its constructor): {0, 0.5, 0.5, 0.1, 0.5, 0.5}.
void headerConstant(uint32_t at) {
    save<uint32_t>(at, 0);
    save<uint32_t>(at + 4, fbits(0.5f));
    save<uint32_t>(at + 8, fbits(0.5f));
    save<uint32_t>(at + 0xC, 0x3DCCCCCD);  // 0.1f
    save<uint32_t>(at + 0x10, fbits(0.5f));
    save<uint32_t>(at + 0x14, fbits(0.5f));
}

// an empty std::unordered_map (bucket list, size, first node; max_load_factor 1.0)
void emptyHashMap(uint32_t at) {
    save<uint32_t>(at, 0);
    save<uint32_t>(at + 4, 0);
    save<uint32_t>(at + 8, 0);
    save<uint32_t>(at + 0xC, 0);
    save<uint32_t>(at + 0x10, fbits(1.0f));
}

// ------------------------------------------------------------------------------------------ Avatar.cpp
constexpr uint32_t AVATAR_HEADER_CONSTANT = 0x1003CE0;
constexpr uint32_t AVATAR_LWF_CALLBACK = 0x1003CC8;      // a file-local std::function<bool(LWF::LWF*)> = $_0
constexpr uint32_t VT_AVATAR_LWF_CALLBACK = 0xF5D040;    // its __func (anonymous namespace $_0, no captures)
constexpr uint32_t FUNCTION_LWF_DTOR_GOT = 0xFD6770;     // &std::function<bool(LWF::LWF*)>::~function
constexpr uint32_t EXPRESSION_MAP_GOT = 0xFD6738;        // &Avatar::expressionMap_
constexpr uint32_t EXPRESSION_MAP_DTOR_GOT = 0xFD6774;   // &unordered_map<const char*, Avatar::Expression>::~

// ------------------------------------------------------------------------------------------ StageActorManager.cpp
constexpr uint32_t STAGE_ACTOR_HEADER_CONSTANT = 0x1004000;
constexpr uint32_t NULL_TARGET_GOT = 0xFD69C4;           // &StageActorManager::NullTarget (an AttackTarget)
constexpr uint32_t ATTACK_TARGET_DTOR_GOT = 0xFD69C8;    // &StageActorManager::AttackTarget::~AttackTarget
constexpr uint32_t ATTACK_TARGET_SIZE = 0x18;

// ------------------------------------------------------------------------------------------ the snow particles
// Two Vec2 pairs and six particle settings (sub_5EC870(settings, name, a, b, c, Vec2&, Vec2&, Vec2&, Vec2&)).
constexpr uint32_t SNOW = 0x1003DE0;                      // Vec2 x4 at +0, +8, +0x10, +0x18
constexpr uint32_t SNOW_HEADER_CONSTANT = SNOW + 0x20;
constexpr uint32_t SNOW_SETTINGS = SNOW + 0x38;          // one setting (0x30 bytes)
constexpr uint32_t SNOW_SETTINGS_2 = 0x1003D20;          // four settings
constexpr uint32_t SETTING_SIZE = 0x30;
constexpr uint32_t PARTICLE_SETTING_CTOR = 0x5EC871;
constexpr uint32_t SNOW_SETTING_DTOR = 0x4534D5;         // sub_4534D4: ~ the single setting
constexpr uint32_t SNOW_SETTINGS_2_DTOR = 0x453511;      // sub_453510: ~ the four settings
constexpr uint32_t S_PT_SNOW_N = 0xE02D57, S_PT_SNOW_NSP = 0xE02D61, S_PT_SNOW_SP = 0xE02D6D;

struct Vec2c { float x, y; };
struct SnowSetting { uint32_t name; float a, b, c; Vec2c p0, p1, p2, p3; };
constexpr Vec2c STD0{0, 0.25f}, STD1{1, 1.5f}, STD2{0, 0.75f}, STD3{1, 0.5f};
constexpr Vec2c ALT0{0, 0}, ALT1{1, 1}, ALT2{0.75f, 0.25f}, ALT3{0.5f, 1.5f};
constexpr SnowSetting SNOW_N{S_PT_SNOW_N, 800, 800, 800, STD0, STD1, STD2, STD3};
constexpr SnowSetting SNOW_TABLE[4] = {
    {S_PT_SNOW_NSP, 400, 20, 200, STD0, STD1, STD2, STD3},
    {S_PT_SNOW_SP, 40, 8, 80, STD0, STD1, STD2, STD3},
    {S_PT_SNOW_NSP, 0, 40, 400, ALT0, ALT1, ALT2, ALT3},
    {S_PT_SNOW_SP, 0, 16, 80, ALT0, ALT1, ALT2, ALT3},
};

void staticVec2(uint32_t at, float x, float y) {
    call(VEC2_CTOR, {at, fbits(x), fbits(y)});
    atexit(load<uint32_t>(VEC2_DTOR_GOT), at);
}
// a setting from four temporary points (the temporaries live until the caller's frame ends; `points` gets 4 Vec2)
void snowSetting(uint32_t at, const SnowSetting &s, uint32_t points) {
    const Vec2c p[4] = {s.p0, s.p1, s.p2, s.p3};
    for (uint32_t k = 0; k < 4; k++) call(VEC2_CTOR, {points + 8 * k, fbits(p[k].x), fbits(p[k].y)});
    call(PARTICLE_SETTING_CTOR, {at, s.name, fbits(s.a), fbits(s.b), fbits(s.c), points, points + 8, points + 16,
                                 points + 24});
}
void destroyPoints(uint32_t points) {
    for (uint32_t k = 4; k-- > 0;) call(VEC2_DTOR, {points + 8 * k});
}

// ------------------------------------------------------------------------------------------ MasterDataManager.cpp
// MasterDataManager::TABLE_DESCRIPTORS: per master table {name, std::function parse = &master::X::parse,
// std::function<const char*()> getHash = &master::X::getHash}, 0x38 bytes each. Both std::functions hold a plain
// function pointer inline: __func vtable (from the GOT) + 8, the pointer, __f_ = the buffer.
constexpr uint32_t MASTER_HEADER_CONSTANT = 0x10054A4;
constexpr uint32_t TABLE_DESCRIPTORS = 0x1004100, DESCRIPTOR_SIZE = 0x38;
constexpr uint32_t VT_GET_HASH_GOT = 0xFD6A18;    // &vtable of __func<const char*(*)()>, shared by every table
constexpr uint32_t TABLE_DESCRIPTORS_DTOR = 0x4541F5;   // sub_4541F4: ~ the 49 descriptors
struct TableRow {
    uint32_t name;     // the table's name (a string literal)
    uint32_t parseGot; // GOT: &vtable of __func<shared_ptr<const X>(*)(const Value&)>, then &X::parse
    uint32_t hashGot;  // GOT: &X::getHash
};
constexpr TableRow MASTER_TABLES[] = {
    {0xE07A08, 0xFD6A10, 0xFD6A1C},  // albumChallenge
    {0xE07A17, 0xFD6A20, 0xFD6A28},  // avatarCombination
    {0xE07A29, 0xFD6A2C, 0xFD6A34},  // avatarParts
    {0xE07A35, 0xFD6A38, 0xFD6A40},  // badstatus
    {0xE07A3F, 0xFD6A44, 0xFD6A4C},  // battleMisc
    {0xE07A4A, 0xFD6A50, 0xFD6A58},  // buff
    {0xDFB632, 0xFD6A5C, 0xFD6A64},  // burst
    {0xE07A4F, 0xFD6A68, 0xFD6A70},  // chapter
    {0xDFB46F, 0xFD6A74, 0xFD6A7C},  // colosseum
    {0xE07A57, 0xFD6A80, 0xFD6A88},  // colosseumStage
    {0xE07A66, 0xFD6A8C, 0xFD6A94},  // drawMedalList
    {0xE07A74, 0xFD6A98, 0xFD6AA0},  // drawMedalType
    {0xE07A82, 0xFD6AA4, 0xFD6AAC},  // drawSkillList
    {0xE07A90, 0xFD6AB0, 0xFD6AB8},  // drawSkillType
    {0xE07A9E, 0xFD6ABC, 0xFD6AC4},  // enemyAttack
    {0xDFCDB7, 0xFD6AC8, 0xFD6AD0},  // enemy
    {0xE07AAA, 0xFD6AD4, 0xFD6ADC},  // evCampaign
    {0xE07AB5, 0xFD6AE0, 0xFD6AE8},  // evGroupPattern
    {0xE07AC4, 0xFD6AEC, 0xFD6AF4},  // evResource
    {0xE07ACF, 0xFD6AF8, 0xFD6B00},  // evStage
    {0xE07AD7, 0xFD6B04, 0xFD6B0C},  // guiltProb
    {0xE07AE1, 0xFD6B10, 0xFD6B18},  // initItem
    {0xE01578, 0xFD6B1C, 0xFD6B24},  // keyblade
    {0xDFA612, 0xFD6B28, 0xFD6B30},  // loginBonus
    {0xE07AEA, 0xFD6B34, 0xFD6B3C},  // material
    {0xE07AF3, 0xFD6B40, 0xFD6B48},  // medal
    {0xE07AF9, 0xFD6B4C, 0xFD6B54},  // medalMisc
    {0xDFA07B, 0xFD6B58, 0xFD6B60},  // misc
    {0xE07B03, 0xFD6B64, 0xFD6B6C},  // mypageBackground
    {0xE07B14, 0xFD6B70, 0xFD6B78},  // player
    {0xE07B1B, 0xFD6B7C, 0xFD6B84},  // raidEnemyAttack
    {0xE07B2B, 0xFD6B88, 0xFD6B90},  // raidEnemy
    {0xE07B35, 0xFD6B94, 0xFD6B9C},  // raidReward
    {0xE07B40, 0xFD6BA0, 0xFD6BA8},  // raidSetting
    {0xE07B4C, 0xFD6BAC, 0xFD6BB4},  // ranking
    {0xDFBD81, 0xFD6BB8, 0xFD6BC0},  // rankingReward
    {0xE07B54, 0xFD6BC4, 0xFD6BCC},  // reward
    {0xE07B5B, 0xFD6BD0, 0xFD6BD8},  // serialcodeReward
    {0xE07B6C, 0xFD6BDC, 0xFD6BE4},  // shop
    {0xE07B71, 0xFD6BE8, 0xFD6BF0},  // skillExp
    {0xE07B7A, 0xFD6BF4, 0xFD6BFC},  // skill
    {0xE07B80, 0xFD6C00, 0xFD6C08},  // sphereArray
    {0xE07B8C, 0xFD6C0C, 0xFD6C14},  // sphere
    {0xE07B93, 0xFD6C18, 0xFD6C20},  // sphereMasu
    {0xE07B9E, 0xFD6C24, 0xFD6C2C},  // stage
    {0xE06455, 0xFD6C30, 0xFD6C38},  // stamp
    {0xE07BA4, 0xFD6C3C, 0xFD6C44},  // title
    {0xE07BAA, 0xFD6C48, 0xFD6C50},  // tutorialMisc
    {0xE07BB7, 0xFD6C54, 0xFD6C5C},  // world
};

}  // namespace

struct StaticConstructors {
    // Avatar.cpp: the LWF callback and Avatar::expressionMap_
    static void avatar() {
        headerConstant(AVATAR_HEADER_CONSTANT);
        save<uint32_t>(AVATAR_LWF_CALLBACK + 0x10, AVATAR_LWF_CALLBACK);
        save<uint32_t>(AVATAR_LWF_CALLBACK, VT_AVATAR_LWF_CALLBACK);
        atexit(load<uint32_t>(FUNCTION_LWF_DTOR_GOT), AVATAR_LWF_CALLBACK);
        const uint32_t map = load<uint32_t>(EXPRESSION_MAP_GOT);
        emptyHashMap(map);
        atexit(load<uint32_t>(EXPRESSION_MAP_DTOR_GOT), map);
    }

    // StageActorManager.cpp: StageActorManager::NullTarget (all zero)
    static void stageActorManager() {
        headerConstant(STAGE_ACTOR_HEADER_CONSTANT);
        const uint32_t target = load<uint32_t>(NULL_TARGET_GOT);
        for (uint32_t k = 0; k < ATTACK_TARGET_SIZE; k += 4) save<uint32_t>(target + k, 0);
        atexit(load<uint32_t>(ATTACK_TARGET_DTOR_GOT), target);
    }

    // the snow particle settings
    static void snow() {
        headerConstant(SNOW_HEADER_CONSTANT);
        staticVec2(SNOW, 0, 0.25f);
        staticVec2(SNOW + 8, 1, 1.5f);
        guest::Frame f(0xDC);
        {
            const uint32_t points = f.addr(0xB8);
            snowSetting(SNOW_SETTINGS, SNOW_N, points);
            destroyPoints(points);
        }
        atexit(SNOW_SETTING_DTOR, 0);
        staticVec2(SNOW + 0x10, 0.125f, 0.25f);
        staticVec2(SNOW + 0x18, 1.75f, 1.5f);
        for (uint32_t k = 0; k < 4; k++) snowSetting(SNOW_SETTINGS_2 + k * SETTING_SIZE, SNOW_TABLE[k], f.addr(0x38 + k * 0x20));
        for (uint32_t k = 4; k-- > 0;) destroyPoints(f.addr(0x38 + k * 0x20));
        atexit(SNOW_SETTINGS_2_DTOR, 0);
    }

    // MasterDataManager.cpp: the table registry
    static void masterTables() {
        headerConstant(MASTER_HEADER_CONSTANT);
        uint32_t at = TABLE_DESCRIPTORS;
        for (const TableRow &t : MASTER_TABLES) {
            save<uint32_t>(at, t.name);
            const uint32_t parse = at + 8, hash = at + 0x20;
            save<uint32_t>(parse, load<uint32_t>(t.parseGot) + 8);
            save<uint32_t>(parse + 4, load<uint32_t>(t.parseGot + 4));
            save<uint32_t>(parse + 0x10, parse);
            save<uint32_t>(hash, load<uint32_t>(VT_GET_HASH_GOT) + 8);
            save<uint32_t>(hash + 4, load<uint32_t>(t.hashGot));
            save<uint32_t>(hash + 0x10, hash);
            at += DESCRIPTOR_SIZE;
        }
        atexit(TABLE_DESCRIPTORS_DTOR, 0);
    }
};
RW_FUNCTION(0x45344C, "Avatar.cpp static constructor (sub_45344C)", StaticConstructors::avatar);
RW_FUNCTION(0x453DD4, "StageActorManager.cpp static constructor (sub_453DD4)", StaticConstructors::stageActorManager);
RW_FUNCTION(0x45355C, "snow particle settings static constructor (sub_45355C)", StaticConstructors::snow);
RW_FUNCTION(0x454160, "MasterDataManager.cpp static constructor (sub_454160)", StaticConstructors::masterTables);
