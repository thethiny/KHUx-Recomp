// RaidDirection: the raid battle's direction layer (a cocos2d::Node, 0x2E8 bytes). It shows the fever level
// banners (RaidBattleAnim armatures) and runs the "cover" scene of a raid attack: the helpers (the raid team's
// avatars, or the tutorial's fixed helpers) jump in from the screen edges onto the boss's click points, attack and
// jump back; when they are all done it runs the attack's end action and removes them.
#include "BattleUi.hpp"

namespace {
using scene::Frame;
using scene::call;
using scene::ld32;
using scene::ldf;
using scene::st32;
using scene::vcall;

// ---- layout (byte offsets)
constexpr uint32_t BOSS_ACTOR = 624;        // RaidBossActor*
constexpr uint32_t FIELD = 628;             // Node the helpers are added to
constexpr uint32_t END_ACTION = 632;        // CallFunc* (retained) run when the cover ends
constexpr uint32_t FEVER = 636;             // fever level shown - 1 (-1 = none)
constexpr uint32_t ATTACKERS = 640;         // std::vector<AvatarActor*>: the helpers that attack (raid_Cover2)
constexpr uint32_t ATTACKER_HP = 652;       // std::vector<int>
constexpr uint32_t COVERS = 664;            // std::vector<AvatarActor*>: the helpers that cover (raid_Cover)
constexpr uint32_t COVER_HP = 676;          // std::vector<int>: max HP of the part each covers
constexpr uint32_t COVER_POINT = 688;       // std::vector<int>: click point index each covers
constexpr uint32_t COVERS_LANDED = 700;     // covers whose jump ended
constexpr uint32_t ATTACKS_DONE = 704;      // attackers done (or without a click point)
constexpr uint32_t ATTACKER_COUNT = 708;
constexpr uint32_t COVER_COUNT = 712;
constexpr uint32_t FEVER_TEXT = 720;        // Armature "RaidBattleAnim": the fever text
constexpr uint32_t FEVER_BACK = 724;        // Armature: the fever level back
constexpr uint32_t FEVER_LEVEL = 728;       // Armature: the fever level number
constexpr uint32_t STATE = 732;             // 0 idle, 1 covering, 2 attacking, 3 done
constexpr uint32_t FADE_LAYER = 736;        // LayerColor (retained)
constexpr uint32_t START_UPDATE = 0x210;    // std::function<void()>
constexpr uint32_t STOP_UPDATE = 0x228;
constexpr uint32_t SIZE = 0x2E8;
constexpr uint32_t BOSS_RAIDBOSS = 2208;    // RaidBossActor: RaidBoss*
constexpr uint32_t RAIDBOSS_COVER_DONE = 0x55A;
constexpr uint32_t AVATAR_ACTOR_AVATAR = 648;   // AvatarActor: Avatar*
constexpr uint32_t PLAYER_SIZE = 72;        // RaidStart::Player
constexpr uint32_t PLAYER_PART = 64;        // the part it covers (1-based)
constexpr uint32_t PLAYER_STYLE = 68;
constexpr uint32_t PLAYER_HP = 48;
constexpr uint32_t APIMANAGER_RAID_START = 376;   // shared_ptr<RaidStart>
constexpr uint32_t RAIDSTART_ATTACKERS = 72;      // std::vector<RaidStart::Player>
constexpr uint32_t RAIDSTART_COVERS = 84;

// ---- guest functions
constexpr uint32_t aeabi_memset = 0x40E7A0;     // __aeabi_memset(dst, n, c)
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t function_assign = 0x4EB9CD;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;
constexpr uint32_t LayerColor_create = rwlib::cocos2d::LayerColor::create;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t TextData_format = 0x492929;          // (string out, type, id, ...)
constexpr uint32_t SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;
constexpr uint32_t JumpTo_create = rwlib::cocos2d::JumpTo::create;            // (duration, const Vec2&, height, jumps)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t RaidBoss_getClickPoints = 0x5D7385;
constexpr uint32_t clickMap_copy = 0x5F33A1;
constexpr uint32_t clickMap_find = 0x5D78F9;
constexpr uint32_t clickMap_dtor = 0x5D8485;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t PlayerVector_copy = 0x5F4A05;
constexpr uint32_t PlayerVector_dtor = 0x4B572D;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;     // (Style out, const Coordinate&, int)
constexpr uint32_t AvatarActor_create = 0x73FD21;
constexpr uint32_t Avatar_preloadResources = 0x5C7ED1;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t RaidBossActor_getMaxHP = 0x5F3B05;
constexpr uint32_t ActorVector_pushSlow = 0x5F94AD;     // vector<AvatarActor*>::__push_back_slow_path(const&)
constexpr uint32_t IntVector_pushSlowMove = 0x47D709;   // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t IntVector_pushSlowCopy = 0x4D6CC1;   // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t shared_addRef = 0xC8DBB5;
constexpr uint32_t shared_release = 0xC8DBD1;

// ---- data
constexpr uint32_t GOT_VTABLE = 0xFD6844;               // _ZTV13RaidDirection
constexpr uint32_t GOT_Color4B_WHITE = 0xFD6848;
constexpr uint32_t GOT_Color3B_WHITE = 0xFD6060;
constexpr uint32_t GOT_CLICK_POINT_NAMES = 0xFD67AC;    // const char* CLICK_POINT_NAMES[]
constexpr uint32_t S_TARGET_TITLE_JSON = 0xE02D90;      // "cocostudio/publish/ActMapTargetTitleAnim.ExportJson" (51)
constexpr uint32_t S_RAID_BATTLE_JSON = 0xE02DC4;       // "cocostudio/publish/RaidBattleAnim.ExportJson" (44)
constexpr uint32_t S_RAID_BATTLE_ANIM = 0xE02DF1;       // "RaidBattleAnim" (14)
constexpr uint32_t T_FEVER_FIRST = 100100001;           // TextData type 2: fever animation names
constexpr uint32_t T_FEVER = 100100002;
// per helper slot (6 floats each)
constexpr uint32_t TAB_COVER_DELAY = 0xE4E370;
constexpr uint32_t TAB_ATTACK_DELAY = 0xE4E388;
constexpr uint32_t TAB_START_X = 0xE4E3A0;
constexpr uint32_t TAB_START_Y = 0xE4E3B8;
constexpr uint32_t TAB_ATTACK_DX = 0xE4E3D0;
constexpr uint32_t TAB_ATTACK_DY = 0xE4E3E8;
constexpr uint32_t TAB_HOME_X = 0xE4E400;
constexpr uint32_t TAB_HOME_Y = 0xE4E418;
constexpr uint32_t TUTORIAL_HELPERS = 0xE4E430;         // 5 avatar coordinates (48 bytes each)
constexpr uint32_t TUTORIAL_SORA = 0xE4E520;
constexpr uint32_t TUTORIAL_HELPER_STYLE = 0xE4E550;    // int per helper

// ---- lambda vtables (__func + 8)
constexpr uint32_t VT_START_UPDATE = 0xF5E074;          // init $_0
constexpr uint32_t VT_STOP_UPDATE = 0xF5E048;           // init $_1
constexpr uint32_t VT_FEVER_SHOW = 0xF5E01C;            // raid_fever {this}
constexpr uint32_t VT_FEVER_NEXT = 0xF5DFF0;            // raid_fever {this, std::string} (heap)
constexpr uint32_t VT_NO_COVER = 0xF5DFC4;              // raid_NoCover {this}
constexpr uint32_t VT_COVER_JUMP = 0xF5DF98;            // raid_Cover_sub1 {this, index}
constexpr uint32_t VT_COVER_LAND = 0xF5DF6C;            // raid_Cover_sub1 {this, index}
constexpr uint32_t VT_COVER_BACK = 0xF5DF40;            // raid_Cover_sub1 {this}
constexpr uint32_t VT_ATTACK_JUMP = 0xF5DF14;           // raid_Cover_sub2 {this, index}
constexpr uint32_t VT_ATTACK_HIT = 0xF5DEE8;            // raid_Cover_sub2 {this, index}
constexpr uint32_t VT_ATTACK_BACK = 0xF5DEBC;           // raid_Cover_sub2 {this}

// ---- virtual slots
constexpr uint32_t V_DELETE = 0x04;
constexpr uint32_t V_SET_SCALE_XY = 0x48;
constexpr uint32_t V_SET_POSITION = 0x4C;
constexpr uint32_t V_SET_POSITION_XY = 0x5C;
constexpr uint32_t V_SET_CONTENT_SIZE = 0xA8;
constexpr uint32_t V_SET_VISIBLE = 0xB0;
constexpr uint32_t V_ADD_CHILD_Z = 0xF4;
constexpr uint32_t V_REMOVE_CHILD = 0x12C;
constexpr uint32_t V_AVATAR_ACTOR_14C = 0x14C;          // AvatarActor: (bool) before its resources load
constexpr uint32_t V_SET_OPACITY = 0x214;
constexpr uint32_t V_SET_COLOR = 0x22C;
constexpr uint32_t V_INIT = 0x248;
constexpr uint32_t V_GET_ANIMATION = 0x298;
constexpr uint32_t V_ANIM_PLAY = 0x68;                  // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t V_ANIM_PLAY_WITH_INDEX = 0x70;
constexpr uint32_t NONE = 0xFFFFFFFFu;
constexpr float SCALE = 1.3f;

uint32_t fb(float f) { return rw::fbits(f); }
int32_t ldi(uint32_t a) { return rw::load<int32_t>(a); }
void inc(uint32_t a) { st32(a, ld32(a) + 1); }
uint32_t tab(uint32_t table, uint32_t k) { return ld32(table + 4 * k); }
void setVisible(uint32_t node, uint32_t v) { vcall(node, V_SET_VISIBLE, {node, v}); }

uint32_t callFunc(uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fn = scene::newFunction(f, vtable, captures);
    uint32_t action = call(CallFunc_create, {fn});
    scene::functionDestroy(fn);
    return action;
}
uint32_t jumpTo(uint32_t seconds, uint32_t pos, uint32_t height) {
    return call(EaseSineOut_create, {call(JumpTo_create, {seconds, pos, height, 1})});
}
// armature->getAnimation()
uint32_t animation(uint32_t armature) { return vcall(armature, V_GET_ANIMATION, {armature}); }
void playName(uint32_t armature, uint32_t name) {
    uint32_t a = animation(armature);
    vcall(a, V_ANIM_PLAY, {a, name, NONE, NONE});
}
void playIndex(uint32_t armature, uint32_t index) {
    uint32_t a = animation(armature);
    vcall(a, V_ANIM_PLAY_WITH_INDEX, {a, index, NONE, NONE});
}

// vector push_back the way the original inlines it: in place while there is room, else the slow path with the
// value's address (`slot` holds the value)
void pushWord(uint32_t vec, uint32_t value, uint32_t slot, uint32_t slow) {
    uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t base = 0;
        if (end) { st32(end, value); base = ld32(vec + 4); }
        st32(vec + 4, base + 4);
    } else {
        st32(slot, value);
        call(slow, {vec, slot});
    }
}
// vector<AvatarActor*>: the original compares end with the capacity for equality
void pushActor(uint32_t vec, uint32_t actor, uint32_t slot) {
    uint32_t end = ld32(vec + 4);
    if (end != ld32(vec + 8)) {
        uint32_t base = 0;
        if (end) { st32(end, actor); base = ld32(vec + 4); }
        st32(vec + 4, base + 4);
    } else {
        st32(slot, actor);
        call(ActorVector_pushSlow, {vec, slot});
    }
}

// A helper avatar: created from its style, kept hidden until it jumps in, added to the field.
uint32_t newHelper(uint32_t style) {
    uint32_t actor = call(AvatarActor_create, {style});
    vcall(actor, V_AVATAR_ACTOR_14C, {actor, 1});
    call(Avatar_preloadResources, {ld32(actor + AVATAR_ACTOR_AVATAR)});
    setVisible(actor, 0);
    return actor;
}
void addToField(uint32_t self, uint32_t actor) {
    uint32_t field = ld32(self + FIELD);
    vcall(field, V_ADD_CHILD_Z, {field, actor, 200});
}

// The boss's click point `name`: searched in a copy of the RaidBoss's map (`map`, destroyed by the caller after
// its actions are made, like the original); the map node or 0.
uint32_t findClickPoint(uint32_t self, uint32_t name, uint32_t map) {
    uint32_t boss = ld32(ld32(self + BOSS_ACTOR) + BOSS_RAIDBOSS);
    call(clickMap_copy, {map, call(RaidBoss_getClickPoints, {boss})});
    Frame f;
    uint32_t key = f.alloc(12);
    scene::strInitC(key, name);
    uint32_t node = call(clickMap_find, {map, key});
    scene::strFree(key);
    return node;
}

// One helper's jump: shown at its start position (mirrored on the slots of 0x15), then
// delay, `jump` callback, jump to the click point, `land` callback, 0.4 s, jump home, `back` callback.
// Without the click point nothing runs (`missing` is called).
void coverJump(uint32_t self, uint32_t vec, uint32_t k, uint32_t delays, uint32_t vtJump, uint32_t vtLand,
               uint32_t vtBack, uint32_t name, float attackSeconds, bool offset) {
    uint32_t node = ld32(ld32(self + vec) + 4 * k);
    setVisible(node, 1);
    call(Node_stopAllActions, {ld32(ld32(self + vec) + 4 * k)});
    Frame f;
    uint32_t home = f.alloc(8), start = f.alloc(8), target = f.alloc(8);
    st32(home, tab(TAB_HOME_X, k));
    st32(home + 4, tab(TAB_HOME_Y, k));
    node = ld32(ld32(self + vec) + 4 * k);
    float sx = ((0x15u >> k) & 1) ? -SCALE : SCALE;
    vcall(node, V_SET_SCALE_XY, {node, fb(sx), fb(SCALE)});
    node = ld32(ld32(self + vec) + 4 * k);
    st32(start, tab(TAB_START_X, k));
    st32(start + 4, tab(TAB_START_Y, k));
    vcall(node, V_SET_POSITION, {node, start});
    uint32_t delay = call(DelayTime_create, {tab(delays, k)});
    uint32_t jump = callFunc(vtJump, {self, k});
    uint32_t land = callFunc(vtLand, {self, k});
    uint32_t pause = call(DelayTime_create, {fb(0.4f)});
    uint32_t back = jumpTo(fb(0.7f), home, fb(290.0f));
    uint32_t done = callFunc(vtBack, {self});
    uint32_t map = f.alloc(20);
    if (uint32_t found = findClickPoint(self, name, map)) {
        st32(target, ld32(found + 20));
        st32(target + 4, ld32(found + 24));
        float y = -480.0f - ldf(target + 4);
        if (offset) {
            st32(target + 4, fb(y + ldf(TAB_ATTACK_DY + 4 * k)));
            st32(target, fb(ldf(target) - ldf(TAB_ATTACK_DX + 4 * k)));
        } else {
            st32(target + 4, fb(y));
        }
        uint32_t to = jumpTo(fb(attackSeconds), target, fb(580.0f));
        node = ld32(ld32(self + vec) + 4 * k);
        call(Node_runAction, {node, call(Sequence_create, {delay, jump, to, land, pause, back, done, 0})});
    } else if (offset) {
        inc(self + ATTACKS_DONE);   // an attacker without its click point counts as done
    }
    call(clickMap_dtor, {map});
}
}  // namespace

namespace raid_direction {

uint32_t construct(uint32_t self) {
    call(Node_ctor, {self});
    st32(self, ld32(GOT_VTABLE) + 8);
    st32(self + BOSS_ACTOR, 0);
    st32(self + FIELD, 0);
    st32(self + END_ACTION, 0);
    st32(self + FEVER, NONE);
    call(aeabi_memset, {self + ATTACKERS, 0x5C, 0});
    return self;
}
RW_FUNCTION(0x5F7ADC, "RaidDirection::RaidDirection", construct);

uint32_t create() {
    uint32_t self = call(scene::fn::operator_new, {SIZE});
    construct(self);
    if (vcall(self, V_INIT, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_DELETE, {self});
    return 0;
}
RW_FUNCTION(0x6086D0, "RaidDirection::create", create);

// The update start / stop functions and the full-screen fade layer (white, transparent, retained).
uint32_t init(uint32_t self) {
    st32(self + STATE, 0);
    {
        Frame f;
        uint32_t fn = scene::newFunction(f, VT_START_UPDATE, {self});
        call(function_assign, {self + START_UPDATE, fn});
        scene::functionDestroy(fn);
    }
    {
        Frame f;
        uint32_t fn = scene::newFunction(f, VT_STOP_UPDATE, {self});
        call(function_assign, {self + STOP_UPDATE, fn});
        scene::functionDestroy(fn);
    }
    Frame f;
    uint32_t visible = f.alloc(8), size = f.alloc(8);
    call(Director_getVisibleSize, {visible, call(Director_getInstance, {})});
    uint32_t layer = call(LayerColor_create, {ld32(GOT_Color4B_WHITE)});
    st32(self + FADE_LAYER, layer);
    call(Size_ctor, {size, ld32(visible), ld32(visible + 4)});
    vcall(layer, V_SET_CONTENT_SIZE, {layer, size});
    layer = ld32(self + FADE_LAYER);
    vcall(layer, V_SET_OPACITY, {layer, 0});
    call(Ref_retain, {ld32(self + FADE_LAYER)});
    return 1;
}
RW_FUNCTION(0x5F7BF8, "RaidDirection::init", init);

void preloadResources(uint32_t) {
    for (uint32_t json : {S_TARGET_TITLE_JSON, S_RAID_BATTLE_JSON}) {
        uint32_t mgr = call(ArmatureDataManager_getInstance, {});
        Frame f;
        uint32_t s = f.alloc(12);
        scene::strInitC(s, json);
        call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        scene::strFree(s);
    }
}
RW_FUNCTION(0x5F7D4C, "RaidDirection::preloadResources", preloadResources);

// The fever banner for `level` (1..5): created on first use (three RaidBattleAnim armatures above the screen),
// the text animation (the first fever has its own), the level number and back, then after 0.5 s twice the
// banner's next animation.
void raid_fever(uint32_t self, int32_t level) {
    if (level > 5) return;
    int32_t old = ldi(self + FEVER);
    if (old == level - 1) return;
    st32(self + FEVER, uint32_t(level - 1));
    uint32_t first = level == 1 ? 1u : 0u;
    Frame f;
    uint32_t textFirst = f.alloc(12), text = f.alloc(12);
    call(TextData_format, {textFirst, 2, T_FEVER_FIRST, first});
    call(TextData_format, {text, 2, T_FEVER, first});
    uint32_t banner = ld32(self + FEVER_TEXT);
    if (!banner) {
        uint32_t visible = f.alloc(8);
        call(Director_getVisibleSize, {visible, call(Director_getInstance, {})});
        for (uint32_t at : {FEVER_TEXT, FEVER_LEVEL, FEVER_BACK}) {
            Frame g;
            uint32_t s = g.alloc(12);
            scene::strInit(s, S_RAID_BATTLE_ANIM, 14);
            st32(self + at, call(Armature_create, {s}));
            scene::strFree(s);
        }
        for (uint32_t at : {FEVER_TEXT, FEVER_LEVEL, FEVER_BACK}) {
            uint32_t a = ld32(self + at);
            uint32_t fn = ld32(ld32(a) + V_SET_POSITION_XY);
            double m = double(rw::bitsf(call(SceneUtil_getSideHeightMargin, {})));
            float y = float(m + m + 550.0);
            call(fn, {a, fb(359.0f), fb(y)});
        }
        playName(ld32(self + FEVER_TEXT), textFirst);
        playIndex(ld32(self + FEVER_LEVEL), ld32(self + FEVER) + 7);
        setVisible(ld32(self + FEVER_LEVEL), 0);
        vcall(self, V_ADD_CHILD_Z, {self, ld32(self + FEVER_TEXT), 12});
        vcall(self, V_ADD_CHILD_Z, {self, ld32(self + FEVER_LEVEL), 13});
        vcall(self, V_ADD_CHILD_Z, {self, ld32(self + FEVER_BACK), 14});
    } else if (ld32(self + FEVER) == 0) {
        bool shown = false;
        if (old == -1) {
            playName(banner, textFirst);
            setVisible(ld32(self + FEVER_LEVEL), 0);
            shown = true;
        }
        playIndex(ld32(self + FEVER_LEVEL), ld32(self + FEVER) + 7);
        if (!shown) playName(ld32(self + FEVER_TEXT), text);
    } else {
        playName(banner, text);
    }
    playIndex(ld32(self + FEVER_BACK), ld32(self + FEVER) + 2);
    setVisible(ld32(self + FEVER_BACK), 1);
    setVisible(ld32(self + FEVER_TEXT), 1);
    uint32_t delay = call(DelayTime_create, {fb(0.5f)});
    uint32_t show = callFunc(VT_FEVER_SHOW, {self});
    // [this, text] stored on the heap (20 bytes): the string is copied, then moved into the lambda
    uint32_t copy = f.alloc(12);
    if (scene::strIsLong(text)) scene::strInit(copy, ld32(text + 8), ld32(text + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(copy + k, ld32(text + k));
    uint32_t fn = f.alloc(20);
    uint32_t heap = call(scene::fn::operator_new, {0x14});
    if (heap) {
        st32(heap, VT_FEVER_NEXT);
        for (uint32_t k = 0; k < 12; k += 4) st32(heap + 8 + k, ld32(copy + k));
        for (uint32_t k = 0; k < 12; k += 4) st32(copy + k, 0);
        st32(heap + 4, self);
    }
    st32(fn + 16, heap);
    uint32_t next = call(CallFunc_create, {fn});
    scene::functionDestroy(fn);
    scene::strFree(copy);
    uint32_t node = ld32(self + FEVER_TEXT);
    call(Node_runAction, {node, call(Sequence_create, {delay, show, delay, next, 0})});
    scene::strFree(text);
    scene::strFree(textFirst);
}
RW_FUNCTION(0x5F7E68, "RaidDirection::raid_fever", raid_fever);

void raid_fever_over(uint32_t self) {
    for (uint32_t at : {FEVER_TEXT, FEVER_LEVEL, FEVER_BACK})
        if (uint32_t a = ld32(self + at)) setVisible(a, 0);
    st32(self + FEVER, NONE);
}
RW_FUNCTION(0x5F83C4, "RaidDirection::raid_fever_over", raid_fever_over);

void raid_Cover_sub1(uint32_t self, int32_t k) {
    uint32_t point = ld32(ld32(self + COVER_POINT) + 4 * uint32_t(k));
    uint32_t name = ld32(ld32(GOT_CLICK_POINT_NAMES) + 4 * point);
    coverJump(self, COVERS, uint32_t(k), TAB_COVER_DELAY, VT_COVER_JUMP, VT_COVER_LAND, VT_COVER_BACK, name, 0.5f, false);
}
RW_FUNCTION(0x5F8644, "RaidDirection::raid_Cover_sub1", raid_Cover_sub1);

void raid_Cover_sub2(uint32_t self, int32_t k) {
    uint32_t name = ld32(ld32(GOT_CLICK_POINT_NAMES));
    coverJump(self, ATTACKERS, uint32_t(k), TAB_ATTACK_DELAY, VT_ATTACK_JUMP, VT_ATTACK_HIT, VT_ATTACK_BACK, name, 0.7f, true);
}
RW_FUNCTION(0x5F89C4, "RaidDirection::raid_Cover_sub2", raid_Cover_sub2);

void raid_Cover2(uint32_t self) {
    st32(self + STATE, 2);
    for (int32_t k = 0; k < ldi(self + ATTACKER_COUNT); k++) raid_Cover_sub2(self, k);
}
RW_FUNCTION(0x5F8510, "RaidDirection::raid_Cover2", raid_Cover2);

void raid_Cover(uint32_t self, uint32_t endAction) {
    st32(self + END_ACTION, endAction);
    call(Ref_retain, {endAction});
    st32(self + STATE, 1);
    for (int32_t k = 0; k < ldi(self + COVER_COUNT); k++) raid_Cover_sub1(self, k);
}
RW_FUNCTION(0x5F8604, "RaidDirection::raid_Cover", raid_Cover);

// No helpers: the attack goes on after 1/6 s (the end action is not used).
void raid_NoCover(uint32_t self, uint32_t) {
    uint32_t delay = call(DelayTime_create, {0x3E2AAAAB});
    uint32_t next = callFunc(VT_NO_COVER, {self});
    call(Node_runAction, {self, call(Sequence_create, {delay, next, 0})});
}
RW_FUNCTION(0x5F8544, "RaidDirection::raid_NoCover", raid_NoCover);

// Covering: once every cover landed the attackers go. Attacking: once every attacker is done, the end action
// runs and the helpers leave the field. Either way the RaidBoss is told.
void update(uint32_t self, float) {
    int32_t state = ldi(self + STATE);
    if (state == 2) {
        if (ldi(self + ATTACKS_DONE) < ldi(self + ATTACKER_COUNT)) return;
        if (uint32_t end = ld32(self + END_ACTION)) call(Node_runAction, {self, call(Sequence_create, {end, 0})});
        uint32_t field = self + FIELD;
        for (int32_t k = 0; k < ldi(self + ATTACKER_COUNT); k++) {
            uint32_t node = ld32(field);
            vcall(node, V_REMOVE_CHILD, {node, ld32(ld32(self + ATTACKERS) + 4 * uint32_t(k)), 1});
        }
        for (int32_t k = 0; k < ldi(self + COVER_COUNT); k++) {
            uint32_t node = ld32(field);
            vcall(node, V_REMOVE_CHILD, {node, ld32(ld32(self + COVERS) + 4 * uint32_t(k)), 1});
        }
        st32(self + STATE, 3);
    } else if (state == 1) {
        if (ldi(self + COVERS_LANDED) < ldi(self + COVER_COUNT)) return;
        raid_Cover2(self);
    } else {
        return;
    }
    scene::st8(ld32(ld32(self + BOSS_ACTOR) + BOSS_RAIDBOSS) + RAIDBOSS_COVER_DONE, 1);
}
RW_FUNCTION(0x5F8414, "RaidDirection::update", update);

// The helpers. Tutorial: five fixed attackers sharing the body's HP above 450 and Sora covering click point 1.
// Otherwise the raid start's players: attackers, then covers (up to 6 each). True when any helper was made.
uint32_t setup_helper(uint32_t self, uint32_t tutorial) {
    st32(self + COVERS_LANDED, 0);
    st32(self + ATTACKS_DONE, 0);
    Frame f;
    uint32_t slot = f.alloc(8), style = f.alloc(0x20);
    if (tutorial == 1) {
        pushWord(self + COVER_POINT, 1, slot, IntVector_pushSlowMove);
        uint32_t hp = (uint32_t(call(RaidBossActor_getMaxHP, {ld32(self + BOSS_ACTOR), 0})) - 450u) / 5u;
        for (uint32_t k = 0; k < 5; k++) {
            call(AvatarUtil_makeStyle, {style, TUTORIAL_HELPERS + 48 * k, ld32(TUTORIAL_HELPER_STYLE + 4 * k)});
            uint32_t actor = newHelper(style);
            pushActor(self + ATTACKERS, actor, slot);
            pushWord(self + ATTACKER_HP, hp, slot, IntVector_pushSlowMove);
            inc(self + ATTACKER_COUNT);
            addToField(self, actor);
            call(SafeBuffer_dtor, {style});
        }
        call(AvatarUtil_makeStyle, {style, TUTORIAL_SORA, 1000});
        uint32_t actor = newHelper(style);
        pushActor(self + COVERS, actor, slot);
        pushWord(self + COVER_HP, call(RaidBossActor_getMaxHP, {ld32(self + BOSS_ACTOR), 1}), slot, IntVector_pushSlowMove);
        inc(self + COVER_COUNT);
        addToField(self, actor);
        call(SafeBuffer_dtor, {style});
        return 1;
    }
    uint32_t api = call(APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + APIMANAGER_RAID_START + 4);
    uint32_t start = ld32(api + APIMANAGER_RAID_START);
    if (ctrl) call(shared_addRef, {ctrl});
    uint32_t made = 0;
    if (start) {
        uint32_t attackers = f.alloc(12), covers = f.alloc(12);
        call(PlayerVector_copy, {attackers, start + RAIDSTART_ATTACKERS});
        for (uint32_t p = ld32(attackers), end = ld32(attackers + 4); p != end; p += PLAYER_SIZE) {
            call(AvatarUtil_makeStyle, {style, p, ld32(p + PLAYER_STYLE)});
            uint32_t actor = call(AvatarActor_create, {style});
            made = 1;
            vcall(actor, V_AVATAR_ACTOR_14C, {actor, 1});
            call(Avatar_preloadResources, {ld32(actor + AVATAR_ACTOR_AVATAR)});
            setVisible(actor, 0);
            pushActor(self + ATTACKERS, actor, slot);
            uint32_t vec = self + ATTACKER_HP;
            uint32_t e = ld32(vec + 4);
            if (e != ld32(vec + 8)) {
                uint32_t base = 0;
                if (e) { st32(e, ld32(p + PLAYER_HP)); base = ld32(vec + 4); }
                st32(vec + 4, base + 4);
            } else {
                call(IntVector_pushSlowCopy, {vec, p + PLAYER_HP});
            }
            inc(self + ATTACKER_COUNT);
            addToField(self, actor);
            int32_t n = ldi(self + ATTACKER_COUNT);
            call(SafeBuffer_dtor, {style});
            if (n > 5) break;
        }
        call(PlayerVector_copy, {covers, start + RAIDSTART_COVERS});
        for (uint32_t p = ld32(covers), end = ld32(covers + 4); p != end; p += PLAYER_SIZE) {
            call(AvatarUtil_makeStyle, {style, p, ld32(p + PLAYER_STYLE)});
            uint32_t actor = call(AvatarActor_create, {style});
            made = 1;
            vcall(actor, V_AVATAR_ACTOR_14C, {actor, 1});
            call(Avatar_preloadResources, {ld32(actor + AVATAR_ACTOR_AVATAR)});
            setVisible(actor, 0);
            pushActor(self + COVERS, actor, slot);
            uint32_t hp = call(RaidBossActor_getMaxHP, {ld32(self + BOSS_ACTOR), ld32(p + PLAYER_PART) - 1});
            pushWord(self + COVER_HP, hp, slot, IntVector_pushSlowMove);
            pushWord(self + COVER_POINT, ld32(p + PLAYER_PART) - 1, slot, IntVector_pushSlowMove);
            inc(self + COVER_COUNT);
            addToField(self, actor);
            int32_t n = ldi(self + COVER_COUNT);
            call(SafeBuffer_dtor, {style});
            if (n > 5) break;
        }
        call(PlayerVector_dtor, {covers});
        call(PlayerVector_dtor, {attackers});
    }
    if (ctrl) call(shared_release, {ctrl});
    return made;
}
RW_FUNCTION(0x5F8D80, "RaidDirection::setup_helper", setup_helper);

// The end flash: the fade layer white and opaque, fades to 64 and back to 255, then `done`.
void to_end(uint32_t self, uint32_t done) {
    uint32_t layer = ld32(self + FADE_LAYER);
    vcall(layer, V_SET_COLOR, {layer, ld32(GOT_Color3B_WHITE)});
    layer = ld32(self + FADE_LAYER);
    vcall(layer, V_SET_OPACITY, {layer, 255});
    layer = ld32(self + FADE_LAYER);
    setVisible(layer, 1);
    uint32_t a = call(FadeTo_create, {0x3DEEEEEF, 64});
    uint32_t b = call(DelayTime_create, {0x3C888889});
    uint32_t c = call(FadeTo_create, {0x3E888889, 255});
    uint32_t d = call(DelayTime_create, {0x3C888889});
    call(Node_runAction, {ld32(self + FADE_LAYER), call(Sequence_create, {a, b, c, d, done, 0})});
}
RW_FUNCTION(0x5F93CC, "RaidDirection::to_end", to_end);

}  // namespace raid_direction

// ------------------------------------------------------------------------------------------------ helper lambdas
// raid_Cover_sub1 / sub2's lambdas {this, helper index}: the helper's hit ($_6 covers, $_9 attackers) and their
// jump motion ($_5, $_8)
namespace {
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;      // (avatar, Avatar::Motion)
constexpr uint32_t RaidBoss_changeMotion = 0x5D6815;    // (boss, RaidBoss::Motion)
constexpr uint32_t RaidBossActor_makeWholeAttackPoint = 0x5F3B65;
constexpr uint32_t MarkerMap_find = 0x5F1251;           // unordered_map<int, BattleMarker*>::find(map, const int&)
constexpr uint32_t display_damage = 0x5EB551;           // (int, Node*)
constexpr uint32_t BOSS_MARKERS = 0x88C;                // RaidBossActor: unordered_map<int, BattleMarker*>
constexpr uint32_t MOTION_ATTACK = 2, MOTION_JUMP = 0x10, BOSS_MOTION_DAMAGE = 2;
constexpr uint32_t FLIPPED_SLOTS = 0x15;                // helper slots 0, 2, 4 face the other way
constexpr float HELPER_SCALE = 1.3f;

uint32_t helperAt(uint32_t dir, uint32_t vectorOffset, uint32_t index) { return ld32(ld32(dir + vectorOffset) + 4 * index); }
void helperMotion(uint32_t dir, uint32_t vectorOffset, uint32_t index, uint32_t motion) {
    call(Avatar_changeMotion, {ld32(helperAt(dir, vectorOffset, index) + AVATAR_ACTOR_AVATAR), motion});
}

// the helper attacks: its attack motion facing its side, the damage shown at the boss marker `key` (when the boss
// has it), the boss's damage motion and its whole attack point
void helperHit(uint32_t functor, uint32_t helpers, uint32_t hp, uint32_t key) {
    const uint32_t dir = ld32(functor + 4);
    helperMotion(dir, helpers, ld32(functor + 8), MOTION_ATTACK);
    const uint32_t index = ld32(functor + 8);
    const float sx = ((FLIPPED_SLOTS >> index) & 1) ? -HELPER_SCALE : HELPER_SCALE;
    const uint32_t actor = helperAt(dir, helpers, index);
    vcall(actor, V_SET_SCALE_XY, {actor, fb(sx), fb(HELPER_SCALE)});
    uint32_t boss = ld32(dir + BOSS_ACTOR);
    const uint32_t node = call(MarkerMap_find, {boss + BOSS_MARKERS, key});
    if (node) {
        call(display_damage, {ld32(ld32(dir + hp) + 4 * ld32(functor + 8)), ld32(node + 0xC)});
        boss = ld32(dir + BOSS_ACTOR);
    }
    call(RaidBoss_changeMotion, {ld32(boss + BOSS_RAIDBOSS), BOSS_MOTION_DAMAGE});
    call(RaidBossActor_makeWholeAttackPoint, {boss});
}
}  // namespace

// raid_Cover_sub2 $_9: an attacker hits (the boss's marker 0)
static void attackerHit(uint32_t functor) {
    Frame f;
    const uint32_t key = f.alloc(8);
    st32(key, 0);
    helperHit(functor, ATTACKERS, ATTACKER_HP, key);
}
// raid_Cover_sub1 $_6: a cover hits (the marker of the click point it covers)
static void coverHit(uint32_t functor) {
    const uint32_t dir = ld32(functor + 4);
    helperHit(functor, COVERS, COVER_HP, ld32(dir + COVER_POINT) + 4 * ld32(functor + 8));
}
static void attackerJumpMotion(uint32_t functor) { helperMotion(ld32(functor + 4), ATTACKERS, ld32(functor + 8), MOTION_JUMP); }
static void coverJumpMotion(uint32_t functor) { helperMotion(ld32(functor + 4), COVERS, ld32(functor + 8), MOTION_JUMP); }

RW_FUNCTION(0x5F96F0, "RaidDirection::raid_Cover_sub2 $_9::operator() (attacker hit)", attackerHit);
RW_FUNCTION(0x5F9866, "RaidDirection::raid_Cover_sub2 $_8::operator() (attacker jump motion)", attackerJumpMotion);
RW_FUNCTION(0x5F99C4, "RaidDirection::raid_Cover_sub1 $_6::operator() (cover hit)", coverHit);
RW_FUNCTION(0x5F9AFE, "RaidDirection::raid_Cover_sub1 $_5::operator() (cover jump motion)", coverJumpMotion);
