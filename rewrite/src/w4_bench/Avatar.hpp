// Avatar: the player's (and friends') avatar, a FlashNode whose LWF is assembled from parts. The avatar's look is
// an Avatar::Style (a SafeBuffer holding hair, hair colour, face, costume, body, keyblade and five accessories);
// each "instance" (Avatar::InstanceInfo: a style + an instance name + 45 part slots) keeps, per part, the LWF path
// of the part and the LWF attached for it. Parts are attached to the motion LWF when its part movies load
// (updateNode's "load" handlers) and detached / re-attached on enterFrame when a part changed.
//
// Part kinds (InstanceInfo slots): 0-13 costume, 14 expression (face), 15-27 body, 28-29 hair, 30 keyblade,
// 31-44 accessories. Motions: 0-46 the motion LWFs of the table at 0xF5CD70 (27 = wait, 46 = none yet),
// 47 = burst motion of burstId.
#pragma once

#include "actor/FlashNode.hpp"
#include "core/SafeBuffer.hpp"

// Avatar::Style: a SafeBuffer of 0x30 bytes (encoded): hair +0, hair colour +4, face +8, costume +12, body +16,
// keyblade id +20 and level +24, accessories[5] +28.
struct AvatarStyle : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7E554;   // _ZTVN6Avatar5StyleE + 8
    static constexpr uint32_t DATA_SIZE = 0x30;
    enum Offset : uint32_t {
        HAIR = 0, HAIR_COLOR = 4, FACE = 8, COSTUME = 12, BODY = 16, KEYBLADE_ID = 20, KEYBLADE_LEVEL = 24,
        ACCESSORIES = 28,
    };
};
RW_SIZE(AvatarStyle, 32);

// One part slot of an instance (24 bytes).
struct AvatarPart {
    rw::Field<uint8_t> dirty;              // +0  path changed: re-attach on the next enterFrame
    const uint8_t pad_01[3];               // +1
    guest::SharedPtr lwf;                  // +4  std::shared_ptr<LWF::LWF> attached for this part
    guest::String path;                    // +12 std::string LWF path of the part
};
RW_SIZE(AvatarPart, 24);

// Avatar::InstanceInfo: a Style (same vtable), the instance name and 45 part slots.
struct AvatarInstanceInfo : AvatarStyle {
    guest::String name;                    // +32 movie instance name the parts attach under
    AvatarPart parts[45];                  // +44

    static constexpr uint32_t SIZE = 1124;
    static constexpr uint32_t PART_COUNT = 45;

    static AvatarInstanceInfo *construct(AvatarInstanceInfo *self);
    static AvatarInstanceInfo *copyConstruct(AvatarInstanceInfo *self, AvatarInstanceInfo *other);
};
RW_LAYOUT(AvatarInstanceInfo, name, 32);
RW_LAYOUT(AvatarInstanceInfo, parts, 44);
RW_SIZE(AvatarInstanceInfo, 1124);

struct Avatar : FlashNode {
    rw::Field<int32_t> accessoryId;        // +1308 accessory being processed (forEachAccessory; "%d" in its path)
    rw::Field<int32_t> burstId;            // +1312 burst whose motion plays as motion 47
    rw::Field<int32_t> motion;             // +1316 current Motion (46 = none: update() starts "wait")
    // +1320 std::unordered_set<int> bursts (addBurst): buckets, bucket count, first node, size, max load factor
    rw::Field<uint32_t> burstBuckets;      // +1320
    rw::Field<uint32_t> burstBucketCount;  // +1324
    rw::Field<uint32_t> burstFirst;        // +1328 first node (next +0, value +8)
    rw::Field<uint32_t> burstSize;         // +1332
    rw::Field<float> burstMaxLoad;         // +1336
    rw::Field<uint32_t> bodyMovie;         // +1340 LWF::Movie* "body" (waist position, shadow follows it)
    rw::Field<float> bodyBaseY;            // +1344 body y when "wait" loaded (FLT_MAX = not yet)
    rw::Field<float> shadowScaleX;         // +1348 the shadow movie's own scale
    rw::Field<float> shadowScaleY;         // +1352
    rw::Field<uint8_t> expressionDirty;    // +1356 pendingExpression must be applied at the next update
    const uint8_t pad_54d[3];              // +1357
    rw::Field<int32_t> expression;         // +1360 Expression shown (frame of the face movie)
    rw::Field<int32_t> pendingExpression;  // +1364
    rw::Field<uint8_t> expansionPending;   // +1368
    const uint8_t pad_559[3];              // +1369
    rw::Field<int32_t> expansionScale;     // +1372 percent
    rw::Field<int32_t> expansionFrames;    // +1376 duration in 1/30 s
    rw::Field<uint8_t> field_564;          // +1380 set by the constructor
    const uint8_t pad_565[3];              // +1381
    rw::Field<int32_t> currentInstance;    // +1384 index into instances
    guest::Vector instances;               // +1388 std::vector<InstanceInfo>

    static constexpr uint32_t VTABLE = 0xF8F144;   // _ZTV6Avatar + 8
    static constexpr uint32_t SIZE = 0x578;
    static constexpr int32_t MOTION_WAIT = 27, MOTION_NONE = 46, MOTION_BURST = 47;
    static constexpr uint32_t PART_NAMES = 0xF5CCBC;    // const char*[45] part movie names
    static constexpr uint32_t MOTION_PATHS = 0xF5CD70;  // const char*[46] motion LWF paths
    static constexpr uint32_t EXPRESSION_NAMES = 0xFEF07C;  // const char*[13]
    static constexpr uint32_t EXPRESSION_MAP_INITIALIZED = 0x1003CB0;  // bool
    static constexpr uint32_t EXPRESSION_MAP = 0x1003CB4;  // unordered_map<const char*, Expression>
    static constexpr uint32_t ATTACH_FILTER = 0x1003CC8;   // std::function<bool(LWF::LWF*)> passed to attach calls

    AvatarInstanceInfo *instance(uint32_t index) const {
        return rw::host_ptr<AvatarInstanceInfo>(instances.begin + index * AvatarInstanceInfo::SIZE);
    }

    static uint32_t create(uint32_t style, uint32_t noCommands);
    static Avatar *construct(Avatar *self, uint32_t noCommands);
    static Avatar *constructWithStyle(Avatar *self, AvatarStyle *style, uint32_t noCommands);
    static void changeStyle(Avatar *self, AvatarStyle *style);
    static Avatar *destruct(Avatar *self);
    static void removeDetachHandlers(Avatar *self);
    static void clearMovieEventHandlers(Avatar *self);
    static void addBurst(Avatar *self, int32_t id);
    static void update(Avatar *self, float dt);
    static void changeMotion(Avatar *self, int32_t motion);
    static void changeExpression(Avatar *self, int32_t expression);
    static void makeLWFPath(Avatar *self, int32_t kind, AvatarStyle *style, uint32_t dir, uint32_t name, uint32_t sb);
    static void gotoAndStopMovie(Avatar *self, AvatarInstanceInfo *info, int32_t kind, const uint32_t *lwf);
    static uint32_t verifyGrandparent(Avatar *self, uint32_t movie, const int32_t *index);
    static void updateNode(Avatar *self, int32_t from, int32_t to, uint32_t unused);
    static void updateLWFPath(Avatar *self, AvatarInstanceInfo *info, int32_t from, int32_t to, uint32_t dir);
    static void removeLWFPath(Avatar *self, AvatarInstanceInfo *info, int32_t from, int32_t to, uint32_t dir);
    static void changeCostume(Avatar *self, int32_t id);
    static void changeFace(Avatar *self, int32_t id);
    static void changeBody(Avatar *self, int32_t id);
    static void changeHair(Avatar *self, int32_t id);
    static void changeHairColor(Avatar *self, int32_t id);
    static void changeKeyblade(Avatar *self, int32_t id, int32_t level);
    static void changeKeybladeCombined(Avatar *self, int32_t idLevel);
    static void forEachAccessory(Avatar *self, AvatarInstanceInfo *info, uint32_t fn);
    static void changeAccessories(Avatar *self, const int32_t *ids);
    static uint32_t setInstanceName(Avatar *self, uint32_t name);
    static void addInstance(Avatar *self, uint32_t name, AvatarStyle *style);
    static void changeLWF(Avatar *self, uint32_t path);
    static void addEventListeners(Avatar *self);
    static uint32_t getWaistPosition(uint32_t out, Avatar *self);
    static void preloadPartResource(uint32_t unused, AvatarInstanceInfo *info, int32_t from, int32_t to, uint32_t dir);
    static void preloadResources(Avatar *self);
    static int32_t getHitEffectID(Avatar *self);
    static uint32_t createExpansionAction(Avatar *self);

    // lambda bodies (operator() of the __func objects; capture: avatar at +4)
    static void onExpressionCommand(uint32_t lambda, const uint32_t *name);       // constructor $_3
    static void onPartLoad(uint32_t lambda, const uint32_t *movie);               // updateNode $_7
    static void onPartEnterFrame(uint32_t lambda, const uint32_t *movie);         // updateNode $_8
    static void onBodyLoad(uint32_t lambda, const uint32_t *movie);               // addEventListeners $_9
    static void onShadowLoad(uint32_t lambda, const uint32_t *movie);             // addEventListeners $_11
    static void onShadowEnterFrame(uint32_t lambda, const uint32_t *movie);       // addEventListeners $_12
};
RW_LAYOUT(Avatar, accessoryId, 1308);
RW_LAYOUT(Avatar, burstBuckets, 1320);
RW_LAYOUT(Avatar, bodyMovie, 1340);
RW_LAYOUT(Avatar, expressionDirty, 1356);
RW_LAYOUT(Avatar, expansionPending, 1368);
RW_LAYOUT(Avatar, field_564, 1380);
RW_LAYOUT(Avatar, currentInstance, 1384);
RW_LAYOUT(Avatar, instances, 1388);
RW_SIZE(Avatar, 0x578);

// Guest functions the Avatar code calls (Thumb addresses).
namespace avatar_fn {
constexpr uint32_t sb_ctor = 0x459F21;           // SafeBuffer::SafeBuffer(unsigned, bool)
constexpr uint32_t sb_copy = 0x45A29D;           // SafeBuffer::SafeBuffer(const SafeBuffer&)
constexpr uint32_t sb_move = 0x45A319;           // SafeBuffer::SafeBuffer(SafeBuffer&&)
constexpr uint32_t sb_dtor = 0x45A13D;           // SafeBuffer::~SafeBuffer()
constexpr uint32_t sb_zerofill = 0x45A605;       // SafeBuffer::zerofill()
constexpr uint32_t sb_read = 0x45A269;           // SafeBuffer::read(dst, src, n, index) const
constexpr uint32_t sb_write = 0x459FA1;          // SafeBuffer::write(dst, src, n, index)
constexpr uint32_t strlen_ = 0x40E08C;           // strlen
constexpr uint32_t memset_ = 0x40E7A0;           // __aeabi_memset(dst, n, c)
constexpr uint32_t memcpy_ = 0x40E164;           // __aeabi_memcpy
constexpr uint32_t memcmp_ = 0x40EF2C;           // memcmp
constexpr uint32_t string_init = 0x47CD93;       // std::string::__init(const char*, size_t)
constexpr uint32_t string_assign = 0x48D811;     // std::string::assign(const char*, size_t)
constexpr uint32_t sbuf_fset = 0x491501;         // StringBuffer::fset(const char*, ...)
constexpr uint32_t expr_map_insert = 0x5C96BD;   // unordered_map<const char*, Expression>::__insert_unique(pair&&)
constexpr uint32_t expr_map_find = 0x5C914D;     // unordered_map<const char*, Expression>::find
constexpr uint32_t instances_push_slow = 0x5C9341;  // vector<InstanceInfo>::__push_back_slow_path(InstanceInfo&&)
constexpr uint32_t instances_dtor = 0x5C5E55;    // __vector_base<InstanceInfo>::~__vector_base
constexpr uint32_t intset_dtor = 0x5C0A99;       // __hash_table<int>::~__hash_table
constexpr uint32_t intset_insert = 0x5C002F;     // __hash_table<int>::__insert_unique(const int&)
constexpr uint32_t fn_void_swap = 0x4EE86D;      // std::function<void()>::swap
constexpr uint32_t fn_cstr_swap = 0x4E2961;      // std::function<void(const char*)>::swap
constexpr uint32_t fn_int_swap = 0x5428E1;       // std::function<void(int)>::swap
constexpr uint32_t fn_parse_assign = 0x5C9BE5;   // std::function<void(const string&, Movie*, Button*)>::operator=(&&)
constexpr uint32_t fn_instance_call = 0x5C7185;  // std::function<void(InstanceInfo&, int, int, const char*)>::operator()
constexpr uint32_t fn_filter_copy = 0x5C8B15;    // std::function<bool(LWF::LWF*)>::function(const function&)
constexpr uint32_t flash_ctor = 0x5C32F5;        // FlashNode::FlashNode(bool)
constexpr uint32_t flash_change_sub = 0x5C2F49;  // FlashNode::change_sub()
constexpr uint32_t flash_update = 0x5C3AF7;      // FlashNode::update(float)
constexpr uint32_t intvec_dtor = 0x478961;       // __vector_base<int>::~__vector_base
constexpr uint32_t actflag_deque_dtor = 0x5C37D9;  // __deque_base<FlashNode::ActFlag>::~__deque_base
constexpr uint32_t int_deque_dtor = 0x5C3805;    // __deque_base<int>::~__deque_base
constexpr uint32_t cmdparser_dtor = 0x5C3831;    // CommandParser::~CommandParser
constexpr uint32_t vec4_ctor = 0x8E691F;         // cocos2d::Vec4::Vec4(float, float, float, float)
constexpr uint32_t vec4_dtor = 0x8E6A69;         // cocos2d::Vec4::~Vec4
constexpr uint32_t vec2_ctor = 0x8E4B9D;         // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t vec2_copy = 0x8E4C03;         // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t vec2_dtor = 0x8E4C17;         // cocos2d::Vec2::~Vec2
constexpr uint32_t vec2_zero = 0x1006950;        // cocos2d::Vec2::ZERO
constexpr uint32_t node_dtor = 0x8927ED;         // cocos2d::Node::~Node
constexpr uint32_t ref_autorelease = 0x8FD0D5;   // cocos2d::Ref::autorelease
constexpr uint32_t lwfnode_create = 0x7EDF31;    // cocos2d::LWFNode::create(const char*, void*)
constexpr uint32_t lwf_clear_handler = 0x808975; // LWF::LWF::ClearMovieEventHandler(const string&)
constexpr uint32_t lwf_add_handler = 0x808645;   // LWF::LWF::AddMovieEventHandler(const string&, const map&)
constexpr uint32_t lwf_search_instance = 0x7FE1B9;  // LWF::LWF::SearchMovieInstance(const string&) const
constexpr uint32_t movie_goto_stop = 0x815D75;   // LWF::Movie::GotoAndStop(int)
constexpr uint32_t movie_goto_play = 0x815DCF;   // LWF::Movie::GotoAndPlay(int)
constexpr uint32_t movie_get_x = 0x815FCD;       // LWF::Movie::GetX() const
constexpr uint32_t movie_get_y = 0x816017;       // LWF::Movie::GetY() const
constexpr uint32_t movie_set_x = 0x815FE5;       // LWF::Movie::SetX(float)
constexpr uint32_t movie_get_scale_x = 0x816061; // LWF::Movie::GetScaleX() const
constexpr uint32_t movie_get_scale_y = 0x8160AB; // LWF::Movie::GetScaleY() const
constexpr uint32_t movie_set_scale_x = 0x816079; // LWF::Movie::SetScaleX(float)
constexpr uint32_t movie_set_scale_y = 0x8160C3; // LWF::Movie::SetScaleY(float)
constexpr uint32_t movie_attach_lwf = 0x814835;  // LWF::Movie::AttachLWF(shared_ptr<LWF>, const string&, function, int, bool)
constexpr uint32_t movie_detach_lwf = 0x814E81;  // LWF::Movie::DetachLWF(shared_ptr<LWF>)
constexpr uint32_t map_insert = 0x530869;        // map<string, function<void(Movie*)>>::__insert_unique(hint, const pair&)
constexpr uint32_t map_destroy = 0x530811;       // map<string, function<void(Movie*)>>::destroy(node)
constexpr uint32_t burst_create = 0x77A491;      // master::Burst::create(int) (sret shared_ptr)
constexpr uint32_t lwfutil_preload = 0x491361;   // LWFUtil::preloadResources(const string&)
constexpr uint32_t hiteffect_preload = 0x5D95F9; // HitEffect::preloadResources(int)
constexpr uint32_t scaleto_create = 0x86A8CD;    // cocos2d::ScaleTo::create(float, float)
constexpr uint32_t ease_sine_create = 0x860E0D;  // cocos2d::EaseSineInOut::create(ActionInterval*)
// cocos2d::Node / LWFNode virtual slots used here
constexpr uint32_t VSLOT_REMOVE_CHILD = 75;      // Node::removeChild(Node*, bool)
constexpr uint32_t VSLOT_60 = 60;                // Node::addChild(Node*): the avatar adds its new LWFNode
constexpr uint32_t VSLOT_158 = 158;              // (FlashNode) called on the avatar after a new LWFNode
constexpr uint32_t VSLOT_LWFNODE_113 = 113;      // LWFNode slot called with 0 (twice) after creating it
constexpr uint32_t VSLOT_LWFNODE_ATTACH = 188;   // LWFNode: attach a part LWF (sret shared_ptr, path, movie, name, filter)
// lambda __func vtables (+8)
constexpr uint32_t FUNC_CTOR_1 = 0xF5D014, FUNC_CTOR_2 = 0xF5CFE8, FUNC_CTOR_3 = 0xF5CFBC, FUNC_CTOR_4 = 0xF5CF90,
                   FUNC_CTOR_5 = 0xF5CF64, FUNC_CTOR_6 = 0xF5CF38;
constexpr uint32_t FUNC_PART_LOAD = 0xF5CF0C, FUNC_PART_ENTER_FRAME = 0xF5CEE0;
constexpr uint32_t FUNC_BODY_LOAD = 0xF5CEB4, FUNC_BODY_UNLOAD = 0xF5CE88, FUNC_SHADOW_LOAD = 0xF5CE5C,
                   FUNC_SHADOW_ENTER_FRAME = 0xF5CE30;
constexpr uint32_t FUNC_BIND_INSTANCE = 0xF8F580;   // __func<__bind<void (Avatar::*)(InstanceInfo&, int, int, const char*)>>
constexpr uint32_t FUNC_BIND_PRELOAD = 0xF8F4D8;    // __func<__bind<void (Avatar::*)(const InstanceInfo&, ...)>>
constexpr uint32_t FUNC_BIND_PARSE = 0xF8F52C;      // __func<__bind<void (CommandParser::*)(const string&)>>
constexpr uint32_t CMDPARSER_PARSE = 0x4F28F9;      // CommandParser::parse(const string&)
// own functions (for member-function pointers and calls through the address)
constexpr uint32_t AV_REMOVE_LWF_PATH = 0x5C6CA5, AV_UPDATE_LWF_PATH = 0x5C6BD1, AV_PRELOAD_PART = 0x5C7DFD;
constexpr uint32_t AV_CTOR = 0x5C58C9, AV_CTOR_STYLE = 0x5C5EDD, AV_INSTANCE_INFO_CTOR = 0x5C5D9D;
// strings
constexpr uint32_t S_BODY = 0xE00D8B, S_SHADOW = 0xE00E40, S_CHAR = 0xDFCDC1, S_ENTER_FRAME = 0xDFE0F3;
constexpr uint32_t S_EXPRESSION = 0xE01586, S_COSTUME = 0xE01591, S_HAIR = 0xE01581, S_KEYBLADE = 0xE01578,
                   S_ACCESSORY = 0xE0156E;
constexpr uint32_t S_WAIT_LWF = 0xE01281;        // "lwf/avatar/motion/wait/wait.lwf"
constexpr uint32_t S_BURST_LWF = 0xE01547;       // "lwf/burst/motion/abl_%05d/abl_%05d.lwf"
constexpr uint32_t ACCESSORY_RANGES = 0xE49900;  // int[][2]: part slot range per accessory category (1-based)
}  // namespace avatar_fn

// helpers shared by the Avatar files
namespace avatar_detail {
inline uint32_t cx(uint32_t fn, std::initializer_list<uint32_t> args) { return uint32_t(rw::call_exact(fn, args)); }
// libc++ ~function() of a std::function at f: in its buffer -> destroy(), on the heap -> destroy_deallocate()
inline void destroyFunction(uint32_t f, bool exact) {
    uint32_t p = rw::load<uint32_t>(f + 16);
    if (p == f) {
        uint32_t fn = rw::load<uint32_t>(rw::load<uint32_t>(p) + 0x10);
        exact ? (void)rw::call_exact(fn, {p}) : (void)rw::call(fn, {p});
    } else if (p) {
        uint32_t fn = rw::load<uint32_t>(rw::load<uint32_t>(p) + 0x14);
        exact ? (void)rw::call_exact(fn, {p}) : (void)rw::call(fn, {p});
    }
}
// a guest std::string's characters (c_str()) / size
inline uint32_t strData(uint32_t s) {
    return (rw::load<uint8_t>(s) & 1) ? rw::load<uint32_t>(s + 8) : s + 1;
}
inline uint32_t strSize(uint32_t s) {
    uint8_t b = rw::load<uint8_t>(s);
    return (b & 1) ? rw::load<uint32_t>(s + 4) : uint32_t(b >> 1);
}
}  // namespace avatar_detail
