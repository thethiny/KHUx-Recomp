// The game's own source files' static constructors (the .init_array entries the host runs at load): the objects
// at namespace scope of Avatar, AvatarActor, StageActorManager, PauseState, RemoteNotification, the audio sources,
// the snow particle settings, the direction and shape tables, MasterDataManager's table registry and bg's
// (MemoryStream, Archive, pixel formats), each with its destructor registered through __cxa_atexit (the array
// destructors are here too). The engine's (cocos2d-x, cocostudio, LWF, FreeType, plugins) and the C libraries' come
// from upstream source.
#include <iterator>

#include "Guest.hpp"

namespace {

using rw::call;
using rw::load;
using rw::save;

constexpr uint32_t DSO_HANDLE_GOT = 0xFD604C;      // GOT slot holding &__dso_handle
constexpr uint32_t VEC2_CTOR = rwlib::cocos2d::Vec2::ctor_base__ff;           // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t VEC2_DTOR = rwlib::cocos2d::Vec2::dtor_base;
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
constexpr uint32_t PARTICLE_SETTING_CTOR = rwlib::sub_5EC870;
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
// sub_5EC870: the snow particle setting's constructor (0x30 bytes: name, a, b, c, then four Vec2 - the second and
// the fourth halved). libsplit counts it as cocos2d-x; it is the game's. The original first sets the four points to
// Vec2::ZERO and scales copies on its stack; only the final values remain.
uint32_t particleSetting(uint32_t self, uint32_t name, uint32_t a, uint32_t b, uint32_t c, uint32_t p0, uint32_t p1,
                         uint32_t p2, uint32_t p3) {
    save<uint32_t>(self, name);
    save<uint32_t>(self + 4, a);
    save<uint32_t>(self + 8, b);
    save<uint32_t>(self + 12, c);
    const uint32_t src[4] = {p0, p1, p2, p3};
    for (uint32_t k = 0; k < 4; k++) {
        const float scale = (k & 1) ? 0.5f : 1.0f;
        save<float>(self + 16 + 8 * k, load<float>(src[k]) * scale);
        save<float>(self + 20 + 8 * k, load<float>(src[k] + 4) * scale);
    }
    return self;
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
constexpr uint32_t TABLE_DESCRIPTORS_DTOR = 0x4540F5;   // sub_4540F4: ~ the 49 descriptors
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

// ------------------------------------------------------------------------------------------ std::function globals
// A std::function holding a plain function pointer inline: __func vtable (from the GOT) + 8, the pointer, __f_ =
// the buffer. Its destructor is registered through __cxa_atexit.
void staticFunction(uint32_t at, uint32_t vtableGot, uint32_t fn, uint32_t dtorGot) {
    save<uint32_t>(at + 0x10, at);
    save<uint32_t>(at, load<uint32_t>(vtableGot) + 8);
    save<uint32_t>(at + 4, fn);
    atexit(load<uint32_t>(dtorGot), at);
}
constexpr uint32_t VT_ALLOCATOR_FN_GOT = 0xFD60F0;   // &vtable of __func<void*(*)(unsigned int)>
constexpr uint32_t VT_DELETER_FN_GOT = 0xFD60E4;     // &vtable of __func<void(*)(void*)>
constexpr uint32_t ALLOCATOR_FN_DTOR_GOT = 0xFD60FC; // &std::function<void*(unsigned int)>::~function
constexpr uint32_t DELETER_FN_DTOR_GOT = 0xFD6100;   // &std::function<void(void*)>::~function

// ------------------------------------------------------------------------------------------ bg::MemoryStream
constexpr uint32_t MEMORY_STREAM_HEADER_CONSTANT = 0x10033F0;
constexpr uint32_t MEMORY_STREAM_ALLOCATOR_GOT = 0xFD60D4;  // &bg::MemoryStream::_defaultAllocator
constexpr uint32_t MEMORY_STREAM_DELETER_GOT = 0xFD60D8;    // &bg::MemoryStream::_defaultDeleter
constexpr uint32_t MEMORY_STREAM_ALLOCATE = 0x49160F;       // sub_49160E: malloc
constexpr uint32_t MEMORY_STREAM_FREE = 0x491639;           // sub_491638: free

// ------------------------------------------------------------------------------------------ bg::Archive
constexpr uint32_t ARCHIVE_ALLOCATOR_GOT = 0xFD8180;        // &bg::Archive::_defaultAllocator
constexpr uint32_t ARCHIVE_DELETER_GOT = 0xFD8184;          // &bg::Archive::_defaultDeleter
constexpr uint32_t ARCHIVE_ALLOCATE = 0x8DC8E1;             // sub_8DC8E0
constexpr uint32_t ARCHIVE_FREE = 0x8DC8E9;                 // sub_8DC8E8

// ------------------------------------------------------------------------------------------ RemoteNotification.cpp
// the device token callback (RemoteNotification::getToken sets it, AppActivity.didRegister calls it): empty
constexpr uint32_t REMOTE_NOTIFICATION_HEADER_CONSTANT = 0x1003A14;
constexpr uint32_t TOKEN_CALLBACK = 0x10039F0;              // std::function<void(const char*)>
constexpr uint32_t TOKEN_CALLBACK_DTOR_GOT = 0xFD6508;

// ------------------------------------------------------------------------------------------ the audio sources
// AudioPlayGameMainSE / AudioPushGameMainBGM ...: the resource map and the main BGM stack
constexpr uint32_t AUDIO_HEADER_CONSTANT = 0x1003C38;
constexpr uint32_t AUDIO_RESOURCES = 0x1003C50;             // unordered_map<int, audio::ResourceID>
constexpr uint32_t AUDIO_RESOURCES_DTOR_GOT = 0xFD6724;
constexpr uint32_t MAIN_BGM_STACK = 0x1003C64;              // six words (begin / end / capacity of two vectors)
constexpr uint32_t MAIN_BGM_STACK_DTOR = 0x5C0B73;          // sub_5C0B72
constexpr uint32_t AUDIO_NAMES_HEADER_CONSTANT = 0x1003C80;
constexpr uint32_t AUDIO_NAMES = 0x1003C98;                 // unordered_map<std::string, int>
constexpr uint32_t AUDIO_NAMES_DTOR_GOT = 0xFD6728;

// ------------------------------------------------------------------------------------------ PauseState.cpp
constexpr uint32_t PAUSE_STATE_HEADER_CONSTANT = 0x1003E5C;
constexpr uint32_t PAUSE_MUTEX = 0x1003E50;                 // std::mutex (sub_C8DE14 lock, sub_C8DE58 unlock)
constexpr uint32_t MUTEX_DTOR_GOT = 0xFD6828;
constexpr uint32_t PAUSED_TARGETS = 0x1003E74;              // std::set<void*>: the Scheduler targets it paused
constexpr uint32_t PAUSED_TARGETS_DTOR_GOT = 0xFD682C;

// ------------------------------------------------------------------------------------------ AvatarActor.cpp
constexpr uint32_t AVATAR_ACTOR_HEADER_CONSTANT = 0x1003E90;
constexpr uint32_t AVATAR_ACTOR_SIZES = 0x1003EA8;          // unordered_map<int, float>
constexpr uint32_t UNORDERED_MAP_INT_FLOAT_CTOR = 0x740A0D; // unordered_map<int, float>(initializer_list)
constexpr uint32_t UNORDERED_MAP_INT_FLOAT_DTOR_GOT = 0xFD69AC;
struct IntFloat { uint32_t key; float value; };
constexpr IntFloat AVATAR_ACTOR_SIZE_LIST[] = {{31, 20}, {33, 96}, {34, 85}, {35, 108}, {36, 74}, {37, 67}};

// ------------------------------------------------------------------------------------------ the eight directions
// 2.0, then the 3x3 grid of unit steps {-1, 0, 1} x {-1, 0, 1} (y outer)
constexpr uint32_t DIRECTIONS = 0x1003F70;
constexpr uint32_t DIRECTIONS_DTOR = 0x453C49;              // sub_453C48: ~ the nine Vec2

// ------------------------------------------------------------------------------------------ the shape tables
// Shapes {int kind; Vec2 at; float a, b}: kind 2 = circle (center, radius), kind 1 = rectangle (origin, size).
constexpr uint32_t VEC2_DEFAULT_CTOR = rwlib::cocos2d::Vec2::ctor_base__v;            // cocos2d::Vec2::Vec2()
constexpr uint32_t SHAPES_HEADER_CONSTANT = 0x1004020;
constexpr uint32_t SHAPES_1 = 0x1004038, SHAPES_2 = 0x1004060, SHAPES_3 = 0x1004090, SHAPE_SIZE = 0x14;
constexpr uint32_t SHAPES_1_DTOR = 0x453E31, SHAPES_2_DTOR = 0x453E55, SHAPES_3_DTOR = 0x453E79;
struct Shape { uint32_t kind; float x, y, a, b; };
constexpr Shape SHAPE_TABLE_1[] = {{2, 70, 74, 67, 0}, {1, 0, 0, 340, 110}};
constexpr Shape SHAPE_TABLE_2[] = {{1, -630, 0, 630, 42}, {1, -128, 4, 128, 62}};
constexpr Shape SHAPE_TABLE_3[] = {{2, 45, -51, 151, 0}, {2, 76, -170, 69, 0}, {2, 171, -56, 61, 0}};

// a shape: zeroed (the constructor), then filled in; a circle has no fourth value
void shape(uint32_t at, const Shape &s) {
    save<uint32_t>(at, 0);
    call(VEC2_DEFAULT_CTOR, {at + 4});
    save<uint32_t>(at, s.kind);
    save<uint32_t>(at + 4, fbits(s.x));
    save<uint32_t>(at + 8, fbits(s.y));
    save<uint32_t>(at + 0xC, fbits(s.a));
    if (s.kind != 2) save<uint32_t>(at + 0x10, fbits(s.b));
}

// ------------------------------------------------------------------------------------------ bg's pixel formats
// Two std::map built from initializer lists (the lists themselves are globals the constructor fills first):
// a format code -> Texture2D::PixelFormat, and (channel order fourcc | bits per channel << 32) -> PixelFormat.
constexpr uint32_t FORMAT_CODES = 0x10067C0;                // std::map<uint8_t, int>
constexpr uint32_t FORMAT_CODE_LIST = 0x10067D8;            // pair<uint8_t, int>[11]
constexpr uint32_t FORMAT_CODES_DTOR = 0x8D9657;            // sub_8D9656
constexpr uint32_t FORMAT_LAYOUTS = 0x10067CC;              // std::map<uint64_t, int>
constexpr uint32_t FORMAT_LAYOUT_LIST = 0x10066F0;          // pair<uint64_t, int>[13] (16 bytes each)
constexpr uint32_t FORMAT_LAYOUTS_DTOR = 0x8D9669;          // sub_8D9668
constexpr uint32_t TREE_BALANCE_AFTER_INSERT = 0x493F5B;    // std::__tree_balance_after_insert
struct FormatCode { uint8_t code; uint32_t format; };
constexpr FormatCode FORMAT_CODE_ROWS[] = {{26, 1}, {18, 2}, {16, 8}, {17, 9}, {19, 4}, {21, 3},
                                           {27, 5}, {22, 6}, {23, 7}, {24, 13}, {25, 11}};
struct FormatLayout { uint32_t order, bits, format; };
constexpr uint32_t BGRA = 0x61726762, RGBA = 0x61626772, RGB = 0x626772, LA = 0x616C;
constexpr FormatLayout FORMAT_LAYOUT_ROWS[] = {
    {BGRA, 0x08080808, 1}, {RGBA, 0x08080808, 2}, {RGBA, 0x04040404, 8}, {RGBA, 0x01050505, 9},
    {RGB, 0x050605, 4},    {RGB, 0x080808, 3},    {'a', 8, 5},           {'l', 8, 6},
    {LA, 0x0808, 7},       {0, 0, 12},            {1, 0, 13},            {2, 0, 10},
    {3, 0, 11},
};

// libc++'s map insert(end(), value) as the initializer_list constructor inlines it. The node (its value already
// copied in) goes after the last element when its key is greater, else to the place a search from the root
// finds; a key already present frees it. Map: begin node +0, root (the end node's left) +4, size +8.
template <class Less>
void insertAtEnd(uint32_t map, uint32_t node, Less less) {
    const uint32_t end = map + 4;
    const uint32_t root = load<uint32_t>(end);
    uint32_t parent = end, child = end;
    bool found = false;
    bool afterLast = load<uint32_t>(map) == end;
    if (!afterLast) {
        uint32_t last = end;
        if (root) {
            for (last = root; load<uint32_t>(last + 4);) last = load<uint32_t>(last + 4);
        } else {
            uint32_t n = end;
            do { last = load<uint32_t>(n + 8); } while (load<uint32_t>(last) == n && ((n = last), true));
        }
        afterLast = less(last, node);
        if (afterLast && root) parent = last, child = last + 4;
    }
    if (!afterLast && root) {
        for (uint32_t n = root;;) {
            if (less(node, n)) {
                parent = n, child = n;
                if (!load<uint32_t>(n)) break;
                n = load<uint32_t>(n);
            } else if (less(n, node)) {
                parent = n, child = n + 4;
                if (!load<uint32_t>(n + 4)) break;
                n = load<uint32_t>(n + 4);
            } else {
                found = true;
                break;
            }
        }
    }
    if (found) {
        guest::deleteObject(node);
        return;
    }
    save<uint32_t>(node, 0);
    save<uint32_t>(node + 4, 0);
    save<uint32_t>(node + 8, parent);
    save<uint32_t>(child, node);
    const uint32_t beginLeft = load<uint32_t>(load<uint32_t>(map));
    if (beginLeft) save<uint32_t>(map, beginLeft);
    call(TREE_BALANCE_AFTER_INSERT, {load<uint32_t>(end), load<uint32_t>(child)});
    save<uint32_t>(map + 8, load<uint32_t>(map + 8) + 1);
}
void emptyTree(uint32_t map) {
    save<uint32_t>(map + 4, 0);
    save<uint32_t>(map + 8, 0);
    save<uint32_t>(map, map + 4);
}

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

    // bg::MemoryStream's default allocator and deleter (malloc / free)
    static void memoryStream() {
        headerConstant(MEMORY_STREAM_HEADER_CONSTANT);
        staticFunction(load<uint32_t>(MEMORY_STREAM_ALLOCATOR_GOT), VT_ALLOCATOR_FN_GOT, MEMORY_STREAM_ALLOCATE,
                       ALLOCATOR_FN_DTOR_GOT);
        staticFunction(load<uint32_t>(MEMORY_STREAM_DELETER_GOT), VT_DELETER_FN_GOT, MEMORY_STREAM_FREE,
                       DELETER_FN_DTOR_GOT);
    }

    // RemoteNotification.cpp: the token callback, empty
    static void remoteNotification() {
        headerConstant(REMOTE_NOTIFICATION_HEADER_CONSTANT);
        save<uint32_t>(TOKEN_CALLBACK + 0x10, 0);
        atexit(load<uint32_t>(TOKEN_CALLBACK_DTOR_GOT), TOKEN_CALLBACK);
    }

    // the audio resources and the main BGM stack
    static void audio() {
        headerConstant(AUDIO_HEADER_CONSTANT);
        emptyHashMap(AUDIO_RESOURCES);
        atexit(load<uint32_t>(AUDIO_RESOURCES_DTOR_GOT), AUDIO_RESOURCES);
        for (uint32_t k = 0; k < 0x18; k += 4) save<uint32_t>(MAIN_BGM_STACK + k, 0);
        atexit(MAIN_BGM_STACK_DTOR, MAIN_BGM_STACK);
    }

    // the audio names
    static void audioNames() {
        headerConstant(AUDIO_NAMES_HEADER_CONSTANT);
        emptyHashMap(AUDIO_NAMES);
        atexit(load<uint32_t>(AUDIO_NAMES_DTOR_GOT), AUDIO_NAMES);
    }

    // PauseState.cpp: its mutex and the set of targets it paused
    static void pauseState() {
        headerConstant(PAUSE_STATE_HEADER_CONSTANT);
        save<uint32_t>(PAUSE_MUTEX, 0);
        atexit(load<uint32_t>(MUTEX_DTOR_GOT), PAUSE_MUTEX);
        emptyTree(PAUSED_TARGETS);
        atexit(load<uint32_t>(PAUSED_TARGETS_DTOR_GOT), PAUSED_TARGETS);
    }

    // AvatarActor.cpp: a size per id
    static void avatarActor() {
        headerConstant(AVATAR_ACTOR_HEADER_CONSTANT);
        guest::Frame f(0x50);
        const uint32_t list = f.addr(8);
        uint32_t at = list;
        for (const IntFloat &p : AVATAR_ACTOR_SIZE_LIST) {
            save<uint32_t>(at, p.key);
            save<uint32_t>(at + 4, fbits(p.value));
            at += 8;
        }
        call(UNORDERED_MAP_INT_FLOAT_CTOR, {AVATAR_ACTOR_SIZES, list, 6});
        atexit(load<uint32_t>(UNORDERED_MAP_INT_FLOAT_DTOR_GOT), AVATAR_ACTOR_SIZES);
    }

    // 2.0 and the nine unit steps
    static void directions() {
        headerConstant(DIRECTIONS + 4);
        save<uint32_t>(DIRECTIONS, fbits(2.0f));
        uint32_t at = DIRECTIONS + 0x1C;
        for (int y = -1; y <= 1; y++)
            for (int x = -1; x <= 1; x++, at += 8) call(VEC2_CTOR, {at, fbits(float(x)), fbits(float(y))});
        atexit(DIRECTIONS_DTOR, 0);
    }

    // the three shape tables
    static void shapes() {
        headerConstant(SHAPES_HEADER_CONSTANT);
        for (uint32_t k = 0; k < 2; k++) shape(SHAPES_1 + k * SHAPE_SIZE, SHAPE_TABLE_1[k]);
        atexit(SHAPES_1_DTOR, 0);
        for (uint32_t k = 0; k < 2; k++) shape(SHAPES_2 + k * SHAPE_SIZE, SHAPE_TABLE_2[k]);
        atexit(SHAPES_2_DTOR, 0);
        for (uint32_t k = 0; k < 3; k++) shape(SHAPES_3 + k * SHAPE_SIZE, SHAPE_TABLE_3[k]);
        atexit(SHAPES_3_DTOR, 0);
    }

    // bg's pixel format maps
    static void pixelFormats() {
        uint32_t at = FORMAT_CODE_LIST;
        for (const FormatCode &r : FORMAT_CODE_ROWS) {
            save<uint8_t>(at, r.code);
            save<uint32_t>(at + 4, r.format);
            at += 8;
        }
        emptyTree(FORMAT_CODES);
        for (at = FORMAT_CODE_LIST; at != FORMAT_CODE_LIST + 8 * 11; at += 8) {
            const uint32_t node = guest::newObject(0x18);
            save<uint32_t>(node + 0x10, load<uint32_t>(at));
            save<uint32_t>(node + 0x14, load<uint32_t>(at + 4));
            insertAtEnd(FORMAT_CODES, node, [](uint32_t a, uint32_t b) {
                return load<uint8_t>(a + 0x10) < load<uint8_t>(b + 0x10);
            });
        }
        atexit(FORMAT_CODES_DTOR, FORMAT_CODES);

        at = FORMAT_LAYOUT_LIST;
        for (const FormatLayout &r : FORMAT_LAYOUT_ROWS) {
            save<uint32_t>(at + 4, r.bits);
            save<uint32_t>(at, r.order);
            save<uint32_t>(at + 8, r.format);
            at += 0x10;
        }
        emptyTree(FORMAT_LAYOUTS);
        for (at = FORMAT_LAYOUT_LIST; at != FORMAT_LAYOUT_LIST + 0x10 * 13; at += 0x10) {
            const uint32_t node = guest::newObject(0x20);
            save<uint32_t>(node + 0x14, load<uint32_t>(at + 4));
            save<uint32_t>(node + 0x10, load<uint32_t>(at));
            save<uint32_t>(node + 0x18, load<uint32_t>(at + 8));
            insertAtEnd(FORMAT_LAYOUTS, node, [](uint32_t a, uint32_t b) {
                return load<uint64_t>(a + 0x10) < load<uint64_t>(b + 0x10);
            });
        }
        atexit(FORMAT_LAYOUTS_DTOR, FORMAT_LAYOUTS);
    }

    // bg::Archive's default allocator and deleter
    static void archive() {
        staticFunction(load<uint32_t>(ARCHIVE_ALLOCATOR_GOT), VT_ALLOCATOR_FN_GOT, ARCHIVE_ALLOCATE,
                       ALLOCATOR_FN_DTOR_GOT);
        staticFunction(load<uint32_t>(ARCHIVE_DELETER_GOT), VT_DELETER_FN_GOT, ARCHIVE_FREE, DELETER_FN_DTOR_GOT);
    }
};
// The exit-time destructors the constructors above register (__cxa_atexit(dtor, nullptr, &__dso_handle)): each
// destroys its array's objects last to first. Vec2's destructor does nothing, but the calls stay.
struct StaticDestructors {
    // the four snow settings: each one's four points (+0x10 .. +0x28), last point first
    static void snowSettings() {
        for (uint32_t k = 4; k-- > 0;) {
            const uint32_t end = SNOW_SETTINGS_2 + (k + 1) * SETTING_SIZE;
            for (uint32_t p = 1; p <= 4; p++) call(VEC2_DTOR, {end - 8 * p});
        }
    }
    // the nine unit steps
    static void directions() {
        for (uint32_t k = 9; k-- > 0;) call(VEC2_DTOR, {DIRECTIONS + 0x1C + 8 * k});
    }
    // each shape's position
    static void shapes(uint32_t table, uint32_t count) {
        for (uint32_t k = count; k-- > 0;) call(VEC2_DTOR, {table + k * SHAPE_SIZE + 4});
    }
    static void shapes1() { shapes(SHAPES_1, 2); }
    static void shapes2() { shapes(SHAPES_2, 2); }
    static void shapes3() { shapes(SHAPES_3, 3); }
    // MasterDataManager::TABLE_DESCRIPTORS: per descriptor, getHash's std::function, then parse's
    static void masterTables() {
        for (uint32_t k = std::size(MASTER_TABLES); k-- > 0;) {
            const uint32_t at = TABLE_DESCRIPTORS + k * DESCRIPTOR_SIZE;
            destroyFunction(at + 0x20);
            destroyFunction(at + 8);
        }
    }

private:
    // std::function's destructor: a callable kept in the buffer is destroyed in place (__base::destroy, slot 4),
    // one on the heap is destroyed and freed (destroy_deallocate, slot 5)
    static void destroyFunction(uint32_t fn) {
        const uint32_t f = load<uint32_t>(fn + 0x10);
        if (f == fn) call(load<uint32_t>(load<uint32_t>(f) + 0x10), {f});
        else if (f) call(load<uint32_t>(load<uint32_t>(f) + 0x14), {f});
    }
};
RW_FUNCTION(0x453510, "snow settings static destructor (sub_453510)", StaticDestructors::snowSettings);
RW_FUNCTION(0x453C48, "direction table static destructor (sub_453C48)", StaticDestructors::directions);
RW_FUNCTION(0x453E30, "shape table 1 static destructor (sub_453E30)", StaticDestructors::shapes1);
RW_FUNCTION(0x453E54, "shape table 2 static destructor (sub_453E54)", StaticDestructors::shapes2);
RW_FUNCTION(0x453E78, "shape table 3 static destructor (sub_453E78)", StaticDestructors::shapes3);
RW_FUNCTION(0x4540F4, "MasterDataManager.cpp static destructor (sub_4540F4)", StaticDestructors::masterTables);

RW_FUNCTION(0x45344C, "Avatar.cpp static constructor (sub_45344C)", StaticConstructors::avatar);
RW_FUNCTION(0x453DD4, "StageActorManager.cpp static constructor (sub_453DD4)", StaticConstructors::stageActorManager);
RW_FUNCTION(0x45355C, "snow particle settings static constructor (sub_45355C)", StaticConstructors::snow);
RW_FUNCTION(0x5EC870, "snow particle setting constructor (sub_5EC870)", particleSetting);
RW_FUNCTION(0x454160, "MasterDataManager.cpp static constructor (sub_454160)", StaticConstructors::masterTables);
RW_FUNCTION(0x453280, "bg::MemoryStream static constructor (sub_453280)", StaticConstructors::memoryStream);
RW_FUNCTION(0x453324, "RemoteNotification.cpp static constructor (sub_453324)", StaticConstructors::remoteNotification);
RW_FUNCTION(0x453374, "audio sources static constructor (sub_453374)", StaticConstructors::audio);
RW_FUNCTION(0x4533F8, "audio names static constructor (sub_4533F8)", StaticConstructors::audioNames);
RW_FUNCTION(0x453B04, "PauseState.cpp static constructor (sub_453B04)", StaticConstructors::pauseState);
RW_FUNCTION(0x453B7C, "AvatarActor.cpp static constructor (sub_453B7C)", StaticConstructors::avatarActor);
RW_FUNCTION(0x453C6C, "direction table static constructor (sub_453C6C)", StaticConstructors::directions);
RW_FUNCTION(0x453EA4, "shape tables static constructor (sub_453EA4)", StaticConstructors::shapes);
RW_FUNCTION(0x457A00, "bg pixel formats static constructor (sub_457A00)", StaticConstructors::pixelFormats);
RW_FUNCTION(0x457E40, "bg::Archive static constructor (sub_457E40)", StaticConstructors::archive);
