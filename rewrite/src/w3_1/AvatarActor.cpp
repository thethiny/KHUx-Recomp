// AvatarActor: the player's (or a friend's) avatar on the quest map. A MapNode carrying the Avatar (FlashNode) that
// draws it, a status board (a RaidBossHPIndicator showing bad statuses and buffs above the head), the chain attack
// state (a list of motions played one after another while closing in on a target) and the prize bag node that
// collects dropped prizes. Four std::function hooks set by the stage decide where it may move and what pickups do.
#include "actor/FlashNode.hpp"
#include "scene/SceneCommon.hpp"

namespace avatar_actor_w31 {

using actor::ld;
using actor::ldf;

struct AvatarActor : MapNode {
    rw::Field<uint32_t> avatar;              // +648 Avatar* (FlashNode)
    rw::Field<uint32_t> shownStatusBits;     // +652 the avatar's bad status bits at the last UpdateStatus
    rw::Field<uint32_t> direction;           // +656 CommonDirection* (material panel, prize parent)
    rw::Field<uint16_t> field_294;           // +660
    const uint8_t pad_296[2];                // +662
    // chain attack (cleared as 20 bytes by ClearChainAttack)
    rw::Field<uint32_t> chainMotions;        // +664 const int* the motions of the chain
    rw::Field<int32_t> chainLength;          // +668
    rw::Field<int32_t> chainIndex;           // +672 next motion
    rw::Field<float> chainStep;              // +676 x distance of the current motion (per unit of scale x)
    rw::Field<uint32_t> chainTarget;         // +680 MapNode* attacked
    rw::Field<uint32_t> statusBoard;         // +684 RaidBossHPIndicator*
    StdFunction canMoveTo;                   // +688 function<bool(float x, float y)>
    const uint8_t pad_2c4[4];                // +708
    StdFunction collisionCheck;              // +712 function<bool(const Vec2&, const Vec2&)>
    const uint8_t pad_2dc[4];                // +732
    StdFunction onGetMunny;                  // +736 function<bool(unsigned)> (prize type 9)
    const uint8_t pad_2f4[4];                // +756
    StdFunction onGetMaterial;               // +760 function<bool(unsigned)> (prize type 8)
    const uint8_t pad_30c[4];                // +780
    rw::Field<uint32_t> prizeBag;            // +784 cocos2d::Node* holding the collected prizes
    rw::Field<uint8_t> field_314;            // +788
    const uint8_t pad_315[3];

    static constexpr uint32_t VTABLE = 0xFA7C34;   // _ZTV11AvatarActor + 8
    static constexpr uint32_t SIZE = 0x318;
};
RW_LAYOUT(AvatarActor, avatar, 648);
RW_LAYOUT(AvatarActor, chainMotions, 664);
RW_LAYOUT(AvatarActor, statusBoard, 684);
RW_LAYOUT(AvatarActor, canMoveTo, 688);
RW_LAYOUT(AvatarActor, collisionCheck, 712);
RW_LAYOUT(AvatarActor, onGetMunny, 736);
RW_LAYOUT(AvatarActor, onGetMaterial, 760);
RW_LAYOUT(AvatarActor, prizeBag, 784);
RW_SIZE(AvatarActor, 0x318);

// Avatar (FlashNode subclass) fields used here
constexpr uint32_t AVATAR_MOTION = 0x524;       // Avatar::Motion currently played
constexpr uint32_t AVATAR_SHADOW = 0x510;       // FlashNode +1296: the effect armature (scaled with the actor)

// Avatar motions
constexpr uint32_t MOTION_WAIT = 11;
constexpr uint32_t MOTION_JUMP = 16;
constexpr uint32_t MOTION_DIE = 41;            // 41 dying, 42 dead, 43 resurrecting
constexpr uint32_t MOTION_DEAD = 42;
constexpr uint32_t MOTION_RESURRECT = 43;

// sound resource ids
constexpr uint32_t SE_DIE = 509000080;
constexpr uint32_t SE_JUMP = 502000030;
constexpr uint32_t SE_MUNNY = 502000040;
constexpr uint32_t SE_MATERIAL = 502000050;
constexpr uint32_t SE_PANEL = 509000060;

// functions
constexpr uint32_t MAPNODE_CTOR = 0x5DA2D5;
constexpr uint32_t MAPNODE_DTOR = 0x5DA38D;
constexpr uint32_t MAPNODE_PAUSE = 0x5DA57D;
constexpr uint32_t MAPNODE_RESUME = 0x5DA597;
constexpr uint32_t NODE_INIT = 0x892B69;
constexpr uint32_t NODE_CREATE = 0x89372D;
constexpr uint32_t NODE_REMOVE_ALL_CHILDREN = 0x894359;
constexpr uint32_t NODE_SET_SCALE_X = 0x892F39;
constexpr uint32_t NODE_GET_SCALE_X = 0x892E9B;
constexpr uint32_t NODE_SCHEDULE_UPDATE = 0x894BA7;
constexpr uint32_t NODE_UNSCHEDULE_UPDATE = 0x894CB3;
constexpr uint32_t REF_RELEASE = 0x8FD0BD;
constexpr uint32_t MEMSET = 0x40E7A0;                  // __aeabi_memset(dst, n, c)
constexpr uint32_t AVATAR_CREATE = 0x5C5883;           // Avatar::create(const Style&, bool)
constexpr uint32_t AVATAR_CHANGE_STYLE = 0x5C5F15;
constexpr uint32_t AVATAR_CHANGE_MOTION = 0x5C63C5;
constexpr uint32_t AVATAR_CHANGE_KEYBLADE = 0x5C7081;
constexpr uint32_t AUDIO_GET_INSTANCE = 0x5BECFD;
constexpr uint32_t AUDIO_LOAD = 0x5BEEF5;
constexpr uint32_t AUDIO_GET_LAYER = 0x5BF33D;
constexpr uint32_t LAYER_PLAY_SE = 0x5BF785;
constexpr uint32_t FUNCTION_VV_ASSIGN = 0x4EB9CD;      // std::function<void()>::operator=
constexpr uint32_t FUNCTION_BFF_CALL = 0x740C71;       // std::function<bool(float, float)>::operator()
constexpr uint32_t FUNCTION_BJ_CALL = 0x7410D9;        // std::function<bool(unsigned)>::operator()
constexpr uint32_t FUNCTION_BVV_CALL = 0x741561;       // std::function<bool(const Vec2&, const Vec2&)>::operator()
constexpr uint32_t HP_INDICATOR_CREATE = 0x5E5165;     // RaidBossHPIndicator::create(Enemy_Type, Type, unsigned, unsigned)
constexpr uint32_t HP_SET_BUF_DEBUF = 0x5E7D91;
constexpr uint32_t HP_CLR_BUF_DEBUF = 0x5E7F41;
constexpr uint32_t HP_SET_ATTACK_BUFF = 0x5E7F89;
constexpr uint32_t HP_SET_DEFENCE_BUFF = 0x5E8189;
constexpr uint32_t HP_NEXT_DISP = 0x5E7DB9;
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;
constexpr uint32_t APIMANAGER_GET_INSTANCE = 0x4BA8C9;
constexpr uint32_t ADD_SHARED = 0xC8DBB5;
constexpr uint32_t RELEASE_SHARED = 0xC8DBD1;
constexpr uint32_t STATUS_CURE_POISON = 0x74030D;      // IStatusProtocol::curePoison()
constexpr uint32_t STATUS_CHANGE_CLEAR = 0x4F1D15;     // StatusChange::clear()
constexpr uint32_t BUFF_CLEAR = 0x4F27E9;              // IBuffProtocol::buff_clear(BuffType, BuffTarget)
constexpr uint32_t BUFF_LEVEL = 0x4F23CD;              // IBuffProtocol::buff_level(BuffType, BuffTarget) const
constexpr uint32_t FLASH_REFLECTION_STATUS = 0x5C3171;
constexpr uint32_t FLASH_SET_BAD_STATUS = 0x5C2D61;
constexpr uint32_t FLASH_STATUS_POISON = 0x5C3C55;
constexpr uint32_t FLASH_STATUS_DEEP_POISON = 0x5C3BCD;
constexpr uint32_t FLASH_STATUS_SLEEP = 0x5C3CD9;
constexpr uint32_t FLASH_STATUS_PARALYSIS = 0x5C3D61;
constexpr uint32_t FLASH_FLAG_SHIFT = 0x5EFD39;        // FlashNode::flagShift()
constexpr uint32_t ACTFLAG_PUSH_FRONT = 0x741CA9;      // std::deque<FlashNode::ActFlag>::push_front
constexpr uint32_t RANDOM_GET_INSTANCE = 0x492055;
constexpr uint32_t RANDOM_UNIFORM_INT = 0x492225;      // Random::generateUniformInt(int, int, unsigned*)
constexpr uint32_t CHAIN_STEP_FIND = 0x741789;         // unordered_map<int, float>::find (const)
constexpr uint32_t CHAIN_STEP_AT = 0x740BF9;           // unordered_map<int, float>::at (const)
constexpr uint32_t VEC2_COPY = 0x8E4C03;
constexpr uint32_t VEC2_CTOR_XY = 0x8E4B9D;
constexpr uint32_t VEC2_DTOR = 0x8E4C17;
constexpr uint32_t VEC2_SUBTRACT = 0x8E530F;
constexpr uint32_t ROUNDF = 0x419C54;                  // roundf (PLT)
constexpr uint32_t MAPPRIZE_SET_ABSORPTION = 0x5DCF45; // MapPrize::setAbsorptionVortex(Vec2&, function<void(MapPrize*)>)
constexpr uint32_t MAPPRIZE_GET_ITEM = 0x5DCE95;       // MapPrize::getItem(Item*, bool)
constexpr uint32_t DIRECTION_MATERIAL_PANEL_IN = 0x765491;   // CommonDirection::req_materialPanel_in(int)
constexpr uint32_t NODE_VECTOR_COPY = 0x4962C5;        // cocos2d::Vector<Node*>::Vector(const Vector&)
constexpr uint32_t NODE_VECTOR_DTOR = 0x496317;
constexpr uint32_t BATTLE_MISC_CREATE = 0x779AD1;      // master::BattleMisc::create(int) (sret)
constexpr uint32_t SAFEBUFFER_READ = 0x45A269;
constexpr uint32_t SAFEBUFFER_WRITE = 0x459FA1;
constexpr uint32_t STAGESTATE_ADD_LUX = 0x4EECD9;
constexpr uint32_t PRIZES_INFO_CTOR = 0x5DB935;        // MapPrizes::Info::Info(Vec2, FromGeneration)
constexpr uint32_t PRIZES_CREATE = 0x5DE8AD;           // MapPrizes::create(const Info*, Node*)
constexpr uint32_t VEC2_ZERO = 0x1006950;

constexpr uint32_t INIT_LAMBDA_VTABLE = 0xF6DD78;       // init(const Style*)::$_0 (captures this)
constexpr uint32_t ADD_PRIZE_LAMBDA_VTABLE = 0xF6DD4C;  // addPrize(MapPrize*)::$_1 (captures this)
constexpr uint32_t CHAIN_TABLE_NEAR = 0xF6DCE4;         // {const int* motions, int count}[6], target closer than 240
constexpr uint32_t CHAIN_TABLE_FAR = 0xF6DD14;
constexpr uint32_t CHAIN_STEPS = 0x1003EA8;             // unordered_map<int, float>: motion -> x distance

// cocos2d::Node vtable byte offsets
constexpr uint32_t V_SET_SCALE_X = 0x28, V_GET_SCALE_X = 0x2C, V_SET_POSITION_Y = 0x6C, V_GET_POSITION_Y = 0x70;
constexpr uint32_t V_SET_VISIBLE = 0xB0, V_ADD_CHILD = 0xF0, V_ADD_CHILD_Z = 0xF4, V_GET_CHILDREN = 0x10C;
constexpr uint32_t V_GET_CHILDREN_COUNT = 0x114, V_REMOVE_FROM_PARENT = 0x124, V_REMOVE_ALL_CHILDREN = 0x138;
constexpr uint32_t V_UPDATE = 0x1C4;

inline uint32_t vc(uint32_t obj, uint32_t off, std::initializer_list<uint32_t> args) {
    return scene::vcall(obj, off, args);
}
inline uint32_t addr(const void *p) { return rw::guest_addr(p); }

// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
inline void playSE(uint32_t id) {
    guest::Frame f(8);
    uint32_t mgr = rw::call(AUDIO_GET_INSTANCE, {});
    rw::call(AUDIO_GET_LAYER, {f.addr(), mgr, 0});
    rw::call(LAYER_PLAY_SE, {rw::load<uint32_t>(f.addr()), id, 0, 0});
    uint32_t ctrl = rw::load<uint32_t>(f.addr(4));
    if (ctrl) rw::call(RELEASE_SHARED, {ctrl});
}

// the inline ~function()
inline void destroyFunction(StdFunction &fn) {
    uint32_t p = fn.f;
    uint32_t buf = addr(&fn);
    if (p == buf) vc(buf, 0x10, {buf});
    else if (p) vc(p, 0x14, {p});
}

// ------------------------------------------------------------------------------------------- life cycle
void initFields(AvatarActor *self) {
    rw::save<uint32_t>(addr(self), AvatarActor::VTABLE);
    self->canMoveTo.f = 0;
    self->collisionCheck.f = 0;
    self->onGetMunny.f = 0;
    self->onGetMaterial.f = 0;
    self->prizeBag = 0;
    self->field_314 = 0;
    self->field_294 = 0;
    self->direction = 0;
    self->shownStatusBits = 0;
    self->avatar = 0;
    rw::call(MEMSET, {addr(self) + 664, 0x18, 0});
}

uint32_t init(AvatarActor *self, uint32_t style);

// AvatarActor::create(style): new, init(style), autorelease (null when init fails)
uint32_t create(uint32_t style) {
    uint32_t obj = guest::newObject(AvatarActor::SIZE);
    rw::call(MAPNODE_CTOR, {obj});
    AvatarActor *self = rw::host_ptr<AvatarActor>(obj);
    initFields(self);
    if (init(self, style) == 1) {
        rw::call(actor_fn::ref_autorelease, {obj});
        return obj;
    }
    rw::call(REF_RELEASE, {obj});
    return 0;
}

AvatarActor *construct(AvatarActor *self) {
    rw::call(MAPNODE_CTOR, {addr(self)});
    initFields(self);
    return self;
}

AvatarActor *destruct(AvatarActor *self) {
    rw::save<uint32_t>(addr(self), AvatarActor::VTABLE);
    rw::call(NODE_REMOVE_ALL_CHILDREN, {addr(self)});
    destroyFunction(self->onGetMaterial);
    destroyFunction(self->onGetMunny);
    destroyFunction(self->collisionCheck);
    destroyFunction(self->canMoveTo);
    rw::call(MAPNODE_DTOR, {addr(self)});
    return self;
}

void destructDelete(AvatarActor *self) { guest::deleteObject(addr(destruct(self))); }

void setStatusBoard(AvatarActor *self);

// init(style): hidden at first; the avatar (when a style is given), the sounds the actor plays, the prize bag
// (60 above the feet, z 10) and the node's exit callback ($_0)
uint32_t init(AvatarActor *self, uint32_t style) {
    uint32_t s = addr(self);
    uint32_t ok = 0;
    if (rw::call(NODE_INIT, {s}) == 1) {
        vc(s, V_SET_VISIBLE, {s, 0});
        if (style) {
            uint32_t avatar = rw::call(AVATAR_CREATE, {style, 0});
            self->avatar = avatar;
            vc(s, V_ADD_CHILD, {s, avatar});
        }
        rw::call(AUDIO_LOAD, {rw::call(AUDIO_GET_INSTANCE, {}), SE_DIE});
        rw::call(AUDIO_LOAD, {rw::call(AUDIO_GET_INSTANCE, {}), SE_JUMP});
        rw::call(AUDIO_LOAD, {rw::call(AUDIO_GET_INSTANCE, {}), SE_MUNNY});
        rw::call(AUDIO_LOAD, {rw::call(AUDIO_GET_INSTANCE, {}), SE_MATERIAL});
        rw::call(AUDIO_LOAD, {rw::call(AUDIO_GET_INSTANCE, {}), SE_PANEL});
        uint32_t bag = rw::call(NODE_CREATE, {});
        self->prizeBag = bag;
        vc(bag, V_SET_POSITION_Y, {bag, rw::fbits(60.0f)});
        vc(s, V_ADD_CHILD_Z, {s, self->prizeBag, 10});
        {
            scene::Frame f;
            uint32_t fn = scene::newFunction(f, INIT_LAMBDA_VTABLE, {s});
            rw::call(FUNCTION_VV_ASSIGN, {s + 552, fn});
            scene::functionDestroy(fn);
        }
        ok = 1;
        if (self->avatar) setStatusBoard(self);
    }
    return ok;
}

// the status board above the head (created once), and the avatar's status effect heights
void setStatusBoard(AvatarActor *self) {
    uint32_t s = addr(self);
    uint32_t board = self->statusBoard;
    if (!board) {
        board = rw::call(HP_INDICATOR_CREATE, {3, 3, 0, 20000});
        self->statusBoard = board;
    }
    if (board) {
        vc(board, V_SET_VISIBLE, {board, 1});
        vc(self->statusBoard, V_SET_POSITION_Y, {self->statusBoard, rw::fbits(200.0f)});
        uint32_t b = self->statusBoard;
        uint32_t setScaleX = ld(ld(b) + V_SET_SCALE_X);
        uint32_t scaleX = rw::call(NODE_GET_SCALE_X, {s});
        rw::call(setScaleX, {b, scaleX});
        vc(s, V_ADD_CHILD_Z, {s, self->statusBoard, 10});
    }
    rw::save<float>(self->avatar + 660, 80.0f);    // FlashNode::effectYParalysis
    rw::save<float>(self->avatar + 664, 150.0f);   // FlashNode::effectY
}
uint32_t setStatusBoardR(AvatarActor *self) {
    setStatusBoard(self);
    return self->avatar;
}

// ------------------------------------------------------------------------------------------- status display
// the stage state (AppState +32) as a held shared_ptr: object, control block (add_shared done)
struct HeldState {
    uint32_t state, ctrl;
};
inline HeldState holdStageState() {
    uint32_t app = rw::call(APPSTATE_GET_INSTANCE, {});
    HeldState h{ld(app + 0x20), ld(app + 0x24)};
    if (h.ctrl) rw::call(ADD_SHARED, {h.ctrl});
    return h;
}

void updateStatus(AvatarActor *self);

// ClearBuffDebuff(): cure every bad status and clear every buff of the stage state, then refresh the display
uint32_t clearBuffDebuff(AvatarActor *self) {
    HeldState h = holdStageState();
    if (h.state) {
        uint32_t status = h.state + 32;   // IStatusProtocol
        rw::call(STATUS_CURE_POISON, {status});
        for (uint32_t type = 1; type <= 2; type++) {
            guest::Frame f(12);
            uint32_t get = ld(ld(status));
            uint32_t set = ld(ld(status) + 4);
            rw::call(get, {f.addr(), status, type});
            uint32_t cleared = rw::call(STATUS_CHANGE_CLEAR, {f.addr()});
            rw::call(set, {status, type, cleared});
        }
        uint32_t buff = h.state + 36;     // IBuffProtocol
        for (uint32_t target = 0; target < 4; target++) {
            rw::call(BUFF_CLEAR, {buff, 0, target});
            rw::call(BUFF_CLEAR, {buff, 1, target});
        }
    }
    if (self->statusBoard) updateStatus(self);
    rw::call(FLASH_REFLECTION_STATUS, {self->avatar, 0});
    if (h.ctrl) rw::call(RELEASE_SHARED, {h.ctrl});
    return 0;
}

// UpdateStatus(): mirror the stage state's bad statuses and buff levels on the avatar and the status board
void updateStatus(AvatarActor *self) {
    uint32_t avatar = self->avatar;
    if (!avatar) return;
    HeldState h = holdStageState();
    self->shownStatusBits = ld(avatar + 1300);   // FlashNode::statusBits
    uint32_t status = h.state + 32;
    struct Bad {
        uint32_t type, indicator, bit, update;
    };
    const Bad bads[4] = {{0, 16, 1, FLASH_STATUS_POISON},
                         {3, 19, 8, FLASH_STATUS_DEEP_POISON},
                         {1, 17, 2, FLASH_STATUS_SLEEP},
                         {2, 18, 4, FLASH_STATUS_PARALYSIS}};
    for (const Bad &b : bads) {
        guest::Frame f(12);
        rw::call(ld(ld(status)), {f.addr(), status, b.type});
        uint32_t board = self->statusBoard;
        if (rw::load<uint32_t>(f.addr(8))) {
            rw::call(HP_SET_BUF_DEBUF, {board, b.indicator, 21});
            if ((rw::load<uint8_t>(addr(self) + 652) & b.bit) == 0) rw::call(b.update, {avatar, 1});
        } else {
            rw::call(HP_CLR_BUF_DEBUF, {board, b.indicator});
            rw::call(b.update, {avatar, 0});
        }
    }
    if (ld(avatar + 1300) == 0) rw::call(FLASH_SET_BAD_STATUS, {avatar, 4});
    uint32_t buff = h.state + 36;
    uint32_t atk0 = rw::call(BUFF_LEVEL, {buff, 0, 0});
    uint32_t atk1 = rw::call(BUFF_LEVEL, {buff, 0, 1});
    uint32_t atk2 = rw::call(BUFF_LEVEL, {buff, 0, 2});
    uint32_t atk3 = rw::call(BUFF_LEVEL, {buff, 0, 3});
    uint32_t def0 = rw::call(BUFF_LEVEL, {buff, 1, 0});
    uint32_t def1 = rw::call(BUFF_LEVEL, {buff, 1, 1});
    uint32_t def2 = rw::call(BUFF_LEVEL, {buff, 1, 2});
    uint32_t def3 = rw::call(BUFF_LEVEL, {buff, 1, 3});
    rw::call(HP_SET_ATTACK_BUFF, {self->statusBoard, atk0, atk1, atk2, atk3});
    rw::call(HP_SET_DEFENCE_BUFF, {self->statusBoard, def0, def1, def2, def3});
    rw::call(HP_NEXT_DISP, {self->statusBoard});
    if (h.ctrl) rw::call(RELEASE_SHARED, {h.ctrl});
}
uint32_t updateStatusR(AvatarActor *self) {
    updateStatus(self);
    return 0;
}

// ------------------------------------------------------------------------------------------- appearance
uint32_t setStyle(AvatarActor *self, uint32_t style) {
    if (self->avatar) {
        rw::call(AVATAR_CHANGE_STYLE, {self->avatar, style});
    } else {
        uint32_t avatar = rw::call(AVATAR_CREATE, {style, 0});
        self->avatar = avatar;
        vc(addr(self), V_ADD_CHILD, {addr(self), avatar});
        setStatusBoard(self);
    }
    return 1;
}

uint32_t setKeyblade(AvatarActor *self, int32_t keyblade) {
    rw::call(AVATAR_CHANGE_KEYBLADE, {self->avatar, uint32_t(keyblade)});
    return 1;
}

// setScaleX(scale): the node, the avatar's effect armature (to the node's scale), the prize bag, the status board
uint32_t setScaleX(AvatarActor *self, float scale) {
    uint32_t s = addr(self);
    rw::call(NODE_SET_SCALE_X, {s, rw::fbits(scale)});
    uint32_t effect = ld(self->avatar + AVATAR_SHADOW);
    uint32_t setScale = ld(ld(effect) + V_SET_SCALE_X);
    uint32_t current = vc(s, V_GET_SCALE_X, {s});
    rw::call(setScale, {effect, current});
    if (self->prizeBag) vc(self->prizeBag, V_SET_SCALE_X, {self->prizeBag, rw::fbits(scale)});
    uint32_t board = self->statusBoard;
    if (board) return vc(board, V_SET_SCALE_X, {board, rw::fbits(scale)});
    return 0;
}

// lookAt(point): face right (scale x -1) when the point is to the right of the actor
uint32_t lookAt(AvatarActor *self, uint32_t point) {
    float scale = self->mapPosition.x < ldf(point) ? -1.0f : 1.0f;
    return vc(addr(self), V_SET_SCALE_X, {addr(self), rw::fbits(scale)});
}

// ------------------------------------------------------------------------------------------- actions
// takeActionFlag(flag): pop the avatar's oldest action flag; 1 when it is `flag`, otherwise it is pushed back
// to the front and 0 is returned (also 0 when there is none)
uint32_t takeActionFlag(AvatarActor *self, uint32_t flag) {
    uint32_t avatar = self->avatar;
    if (ld(avatar + 0x4F4) == 0) return 0;   // actFlags.size()
    uint32_t got = rw::call(FLASH_FLAG_SHIFT, {avatar});
    if (got == flag) return 1;
    guest::Frame f(8);
    rw::save<uint32_t>(f.addr(), got);
    rw::call(ACTFLAG_PUSH_FRONT, {self->avatar + 1248, f.addr()});
    return 0;
}

uint32_t setActionJump(AvatarActor *self) {
    uint32_t avatar = self->avatar;
    if (ld(avatar + AVATAR_MOTION) != MOTION_JUMP) {
        rw::call(AVATAR_CHANGE_MOTION, {avatar, MOTION_JUMP});
        playSE(SE_JUMP);
    }
    return 0;
}

uint32_t isActionJump(AvatarActor *self) { return ld(self->avatar + AVATAR_MOTION) == MOTION_JUMP ? 1 : 0; }

uint32_t setActionDied(AvatarActor *self) {
    uint32_t avatar = self->avatar;
    if (ld(avatar + AVATAR_MOTION) - MOTION_DIE >= 3) {
        rw::call(AVATAR_CHANGE_MOTION, {avatar, MOTION_DIE});
        playSE(SE_DIE);
    }
    return 0;
}

uint32_t isActionDied(AvatarActor *self) { return ld(self->avatar + AVATAR_MOTION) - MOTION_DIE < 3 ? 1 : 0; }

uint32_t setActionResurrection(AvatarActor *self) {
    return rw::call(AVATAR_CHANGE_MOTION, {self->avatar, MOTION_RESURRECT});
}

// updateActionDied(): 0 alive (or back to waiting), 1 dying, 2 dead, 3 resurrecting; the movie's action flag 4
// ends the dying / resurrecting motions
uint32_t updateActionDied(AvatarActor *self) {
    uint32_t motion = ld(self->avatar + AVATAR_MOTION);
    if (motion == MOTION_RESURRECT) {
        if (takeActionFlag(self, 4) != 1) return 3;
        rw::call(AVATAR_CHANGE_MOTION, {self->avatar, MOTION_WAIT});
        return 0;
    }
    if (motion == MOTION_DEAD) return 2;
    if (motion != MOTION_DIE) return 0;
    if (takeActionFlag(self, 4) != 1) return 1;
    rw::call(AVATAR_CHANGE_MOTION, {self->avatar, MOTION_DEAD});
    return 2;
}

// ------------------------------------------------------------------------------------------- chain attack
void clearChainAttack(AvatarActor *self) { rw::call(MEMSET, {addr(self) + 664, 0x14, 0}); }

// choiceChainType(target, type): pick one of the chain motion lists of `type` (1..6; near or far table by the x
// distance to the target, a random one when there are several); other types clear the chain
void choiceChainType(AvatarActor *self, uint32_t target, int32_t type) {
    uint32_t index = uint32_t(type) - 1;
    if (index > 5) {
        clearChainAttack(self);
        return;
    }
    MapNode *t = rw::host_ptr<MapNode>(target);
    bool near = std::fabs(self->mapPosition.x - t->mapPosition.x) < 240.0f;
    uint32_t entry = (near ? CHAIN_TABLE_NEAR : CHAIN_TABLE_FAR) + index * 8;
    int32_t count = rw::load<int32_t>(entry + 4);
    uint32_t pick = 0;
    if (count >= 2) {
        uint32_t random = rw::call(RANDOM_GET_INSTANCE, {});
        pick = rw::call(RANDOM_UNIFORM_INT, {random, 0, uint32_t(count - 1), 0});
    }
    self->chainMotions = ld(ld(entry) + pick * 4);
    self->chainLength = type;
    self->chainIndex = 0;
    rw::save<uint32_t>(addr(self) + 676, 0);
    self->chainTarget = target;
}

// setChainAttackMotion(): advance by the last motion's step, then start the next motion of the chain (0 = done)
uint32_t setChainAttackMotion(AvatarActor *self) {
    uint32_t s = addr(self);
    if (!self->chainMotions) return 0;
    float step = self->chainStep;
    float scaleX = rw::bitsf(vc(s, V_GET_SCALE_X, {s}));
    self->mapPosition.x = self->mapPosition.x - step * scaleX;
    int32_t index = self->chainIndex;
    if (index >= self->chainLength) {
        self->chainMotions = 0;
        return 0;
    }
    uint32_t motion = ld(self->chainMotions + uint32_t(index) * 4);
    uint32_t distance = 0;
    {
        guest::Frame f(8);
        rw::save<uint32_t>(f.addr(4), motion);
        if (rw::call(CHAIN_STEP_FIND, {CHAIN_STEPS, f.addr(4)})) {
            rw::save<uint32_t>(f.addr(), motion);
            distance = ld(rw::call(CHAIN_STEP_AT, {CHAIN_STEPS, f.addr()}));
        }
    }
    rw::save<uint32_t>(s + 676, distance);
    rw::call(AVATAR_CHANGE_MOTION, {self->avatar, motion});
    self->chainIndex = int32_t(uint32_t(int32_t(self->chainIndex)) + 1);
    if (self->canMoveTo.f) {
        uint32_t target = self->chainTarget;
        float targetX = ldf(target + 624);
        float next = self->chainStep;
        float sx = rw::bitsf(vc(s, V_GET_SCALE_X, {s}));
        float x = targetX - next * sx;
        if (rw::call(FUNCTION_BFF_CALL, {s + 688, rw::fbits(x), ld(target + 628)}) == 0) self->chainStep = 0.0f;
    }
    return 1;
}

// ------------------------------------------------------------------------------------------- prizes
void delPrize(AvatarActor *self, uint32_t prize, uint32_t playSound);

// addPrize(prize): the prize flies into the bag ($_1 drops it into the bag when it arrives)
uint32_t addPrize(AvatarActor *self, uint32_t prize) {
    uint32_t s = addr(self);
    if (!self->prizeBag) return 0;
    scene::Frame f;
    uint32_t to = f.alloc(8);
    rw::call(VEC2_COPY, {to, prize + 624});
    rw::call(VEC2_SUBTRACT, {to, s + 624});
    uint32_t bag = self->prizeBag;
    uint32_t getY = ld(ld(bag) + V_GET_POSITION_Y);
    float dy = ldf(to + 4);
    float bagY = rw::bitsf(rw::call(getY, {bag}));
    rw::save<float>(to + 4, -dy - bagY);
    uint32_t fn = scene::newFunction(f, ADD_PRIZE_LAMBDA_VTABLE, {s});
    rw::call(MAPPRIZE_SET_ABSORPTION, {prize, to, fn});
    scene::functionDestroy(fn);
    vc(self->prizeBag, V_ADD_CHILD, {self->prizeBag, prize});
    rw::call(VEC2_DTOR, {to});
    return 0;
}

// addPrize $_1 (MapPrize* arrived): delPrize(prize, play the sound)
uint32_t addPrizeLambda(uint32_t lambda, const uint32_t *prize) {
    delPrize(rw::host_ptr<AvatarActor>(ld(lambda + 4)), *prize, 1);
    return 0;
}

// setPrizeContent(item, playSound): munny (9) and materials (8) go to the stage hooks (a sound when both the hook
// and playSound say so), Lux (4) is added to the stage state
uint32_t setPrizeContent(AvatarActor *self, uint32_t item, uint32_t playSound) {
    HeldState h = holdStageState();
    uint32_t type = ld(item);
    if (type == 9) {
        if ((rw::call(FUNCTION_BJ_CALL, {addr(self) + 736, ld(item + 8)}) & playSound) == 1) playSE(SE_MUNNY);
    } else if (type == 8) {
        if ((rw::call(FUNCTION_BJ_CALL, {addr(self) + 760, ld(item + 8)}) & playSound) == 1) playSE(SE_MATERIAL);
    } else if (type == 4) {
        rw::save<uint32_t>(h.state + 0x2C, ld(h.state + 0x2C) + ld(item + 8));
    }
    if (h.ctrl) rw::call(RELEASE_SHARED, {h.ctrl});
    return 0;
}

// delPrize(prize, playSound): take the prize's item (panel materials show the material panel), then remove it
void delPrize(AvatarActor *self, uint32_t prize, uint32_t playSound) {
    if (!self->prizeBag) return;
    guest::Frame f(16);
    uint32_t item = f.addr();   // MapPrize::Item {type, id, count, flag}
    rw::save<uint32_t>(item, 1);
    rw::save<uint32_t>(item + 4, 0);
    rw::save<uint32_t>(item + 8, 0);
    rw::save<uint8_t>(item + 12, 0);
    if (rw::call(MAPPRIZE_GET_ITEM, {prize, item, 1}) == 1) {
        if (rw::load<uint8_t>(prize + 0x2B2)) {
            if (self->direction) rw::call(DIRECTION_MATERIAL_PANEL_IN, {self->direction, ld(item + 4)});
            if (playSound == 1) playSE(SE_PANEL);
        } else {
            setPrizeContent(self, item, playSound);
        }
    }
    vc(prize, V_REMOVE_FROM_PARENT, {prize});
}
uint32_t delPrizeR(AvatarActor *self, uint32_t prize, uint32_t playSound) {
    delPrize(self, prize, playSound);
    return 0;
}

// clearPrize(): take every prize in the bag (no sound), then empty the bag
uint32_t clearPrize(AvatarActor *self) {
    uint32_t bag = self->prizeBag;
    if (!bag) return 0;
    guest::Frame f(16);
    uint32_t children = vc(bag, V_GET_CHILDREN, {bag});
    rw::call(NODE_VECTOR_COPY, {f.addr(), children});
    uint32_t end = ld(f.addr(4));
    for (uint32_t it = ld(f.addr()); it != end; it += 4) delPrize(self, ld(it), 0);
    vc(self->prizeBag, V_REMOVE_ALL_CHILDREN, {self->prizeBag});
    rw::call(NODE_VECTOR_DTOR, {f.addr()});
    return 0;
}

uint32_t hasPrize(AvatarActor *self) {
    uint32_t bag = self->prizeBag;
    if (!bag) return 0;
    return int32_t(vc(bag, V_GET_CHILDREN_COUNT, {bag})) > 0 ? 1 : 0;
}

// a BattleMisc value (master table #4, the int at data position 4)
inline int32_t battleMisc(uint32_t id) {
    guest::Frame f(16);
    rw::call(BATTLE_MISC_CREATE, {f.addr(), id});
    uint32_t row = ld(f.addr());
    rw::call(SAFEBUFFER_READ, {row, f.addr(8), ld(row + 12) + 4, 4, 4});
    int32_t value = rw::load<int32_t>(f.addr(8));
    uint32_t ctrl = ld(f.addr(4));
    if (ctrl) rw::call(RELEASE_SHARED, {ctrl});
    return value;
}

// ceil of a non-negative float as the original computes it (truncate, +1 when something was cut off)
inline uint32_t ceilCount(float v) {
    uint32_t n = bt::f2uiz(v);
    return float(n) < v ? n + 1 : n;
}

// getShortTermFinishBonus(rank): the prizes for finishing a quest fast (rank 1 or 2): munny and materials as a
// rate of the quest's values, plus the Lux saved in the stage state times a rate (paid out right away). Returns
// the MapPrizes node, or null when there is nothing.
uint32_t getShortTermFinishBonus(AvatarActor *self, uint32_t rank) {
    int32_t luxRate = battleMisc(910);
    float munnyRate = 0.0f, materialRate = 0.0f;
    if (rank == 2) {
        munnyRate = float(battleMisc(905)) / 10000.0f;
        int32_t material = battleMisc(901);
        int32_t lux = battleMisc(909);
        materialRate = float(material) / 10000.0f;
        luxRate = lux;
    } else if (rank == 1) {
        munnyRate = float(battleMisc(904)) / 10000.0f;
        int32_t material = battleMisc(900);
        int32_t lux = battleMisc(908);
        materialRate = float(material) / 10000.0f;
        luxRate = lux;
    }
    guest::Frame f(0x80);
    uint32_t zero = f.addr(0);
    uint32_t info = f.addr(0x10);   // MapPrizes::Info: Vec2, then {type, ?, count, ?} items at +8, +0x18, +0x28
    uint32_t scratch = f.addr(0x70);
    rw::call(VEC2_COPY, {zero, VEC2_ZERO});
    float luxFactor = float(luxRate) / 10000.0f;
    rw::call(PRIZES_INFO_CTOR, {info, zero, 1});
    rw::call(VEC2_DTOR, {zero});

    uint32_t api = rw::call(APIMANAGER_GET_INSTANCE, {});
    uint32_t questBuf = ld(api + 0x8C);
    rw::call(SAFEBUFFER_READ, {questBuf, scratch, ld(questBuf + 12) + 0x38, 4, 0x38});
    bool any = false;
    uint32_t munny = ceilCount(munnyRate * float(rw::load<int32_t>(scratch)));
    if (munny) {
        rw::save<uint32_t>(info + 8, 9);
        rw::save<uint32_t>(info + 0x10, munny);
        any = true;
    }
    uint32_t materials = ceilCount(materialRate * 10000.0f);
    if (materials) {
        rw::save<uint32_t>(info + 0x18, 8);
        rw::save<uint32_t>(info + 0x20, materials);
        any = true;
    }
    uint32_t state = ld(rw::call(APPSTATE_GET_INSTANCE, {}) + 0x20);
    rw::call(SAFEBUFFER_READ, {state, scratch + 4, ld(state + 12) + 0x45C0, 4, 0x45C0});
    uint32_t lux = ceilCount(luxFactor * float(rw::load<uint32_t>(scratch + 4)));
    uint32_t result = 0;
    if (lux) {
        rw::save<uint32_t>(info + 0x28, 11);
        rw::save<uint32_t>(info + 0x30, lux);
        rw::save<uint32_t>(info + 0x4C, 2);
        uint32_t st = ld(rw::call(APPSTATE_GET_INSTANCE, {}) + 0x20);
        rw::save<uint32_t>(scratch + 8, 0);
        rw::call(SAFEBUFFER_WRITE, {st, ld(st + 12) + 0x45C0, scratch + 8, 4, 0x45C0});
        rw::call(STAGESTATE_ADD_LUX, {ld(rw::call(APPSTATE_GET_INSTANCE, {}) + 0x20), lux});
        result = rw::call(PRIZES_CREATE, {info, ld(self->direction + 628)});
    } else if (any) {
        result = rw::call(PRIZES_CREATE, {info, ld(self->direction + 628)});
    }
    rw::call(VEC2_DTOR, {info});
    return result;
}

// ------------------------------------------------------------------------------------------- movement
uint32_t isCollisionDetectionToDestination(AvatarActor *self, uint32_t destination) {
    if (!self->collisionCheck.f) return 1;
    return rw::call(FUNCTION_BVV_CALL, {addr(self) + 712, addr(self) + 624, destination});
}

// setSefeMoveMapPotision(position): move to the rounded position when the stage allows it
uint32_t setSafeMoveMapPosition(AvatarActor *self, uint32_t position) {
    if (!self->canMoveTo.f) return 0;
    if (rw::call(FUNCTION_BFF_CALL, {addr(self) + 688, ld(position), ld(position + 4)}) != 1) return 0;
    uint32_t x = rw::call(ROUNDF, {ld(position)});
    uint32_t y = rw::call(ROUNDF, {ld(position + 4)});
    guest::Frame f(8);
    rw::call(VEC2_CTOR_XY, {f.addr(), x, y});
    uint32_t vx = ld(f.addr()), vy = ld(f.addr(4));
    rw::save<uint32_t>(addr(self) + 628, vy);
    rw::save<uint32_t>(addr(self) + 624, vx);
    rw::call(VEC2_DTOR, {f.addr()});
    return 1;
}

uint32_t updateGauge(AvatarActor *self, uint32_t dt) {
    uint32_t board = self->statusBoard;
    if (board) return vc(board, V_UPDATE, {board, dt});
    return 0;
}

uint32_t pause(AvatarActor *self) {
    if (self->avatar) {
        uint32_t lwf = ld(self->avatar + 668);
        if (lwf) rw::call(NODE_UNSCHEDULE_UPDATE, {lwf});
    }
    return rw::call(MAPNODE_PAUSE, {addr(self)});
}

uint32_t resume(AvatarActor *self) {
    rw::call(MAPNODE_RESUME, {addr(self)});
    uint32_t avatar = self->avatar;
    if (!avatar) return 0;
    uint32_t lwf = ld(avatar + 668);
    if (!lwf) return 0;
    return rw::call(NODE_SCHEDULE_UPDATE, {lwf});
}

// the complete ~__func of addPrize::$_1 (shared with StageFieldManager::addPrizeAttract::$_1): nothing to do
uint32_t emptyLambdaDestructor(uint32_t self) { return self; }

}  // namespace avatar_actor_w31

namespace aa = avatar_actor_w31;
RW_FUNCTION(0x73FD20, "AvatarActor::create(const Avatar::Style*)", aa::create);
RW_FUNCTION(0x73FDDC, "AvatarActor::AvatarActor()", aa::construct);
RW_FUNCTION(0x73FE4C, "AvatarActor::~AvatarActor()", aa::destruct);
RW_FUNCTION(0x73FFAC, "AvatarActor::~AvatarActor() (deleting)", aa::destructDelete);
RW_FUNCTION(0x73FFB8, "AvatarActor::init(const Avatar::Style*)", aa::init);
RW_FUNCTION(0x740128, "AvatarActor::setStatusBoard()", aa::setStatusBoardR);
RW_FUNCTION(0x7401B8, "AvatarActor::ClearBuffDebuff()", aa::clearBuffDebuff);
RW_FUNCTION(0x7403A8, "AvatarActor::UpdateStatus()", aa::updateStatusR);
RW_FUNCTION(0x740634, "AvatarActor::setStyle(const Avatar::Style&)", aa::setStyle);
RW_FUNCTION(0x740678, "AvatarActor::setKeyblade(int)", aa::setKeyblade);
RW_FUNCTION(0x740688, "AvatarActor::takeActionFlag(FlashNode::ActFlag)", aa::takeActionFlag);
RW_FUNCTION(0x740704, "AvatarActor::setActionJump()", aa::setActionJump);
RW_FUNCTION(0x7407A0, "AvatarActor::isActionJump()", aa::isActionJump);
RW_FUNCTION(0x7407BC, "AvatarActor::setActionDied()", aa::setActionDied);
RW_FUNCTION(0x740858, "AvatarActor::isActionDied()", aa::isActionDied);
RW_FUNCTION(0x740878, "AvatarActor::setActionResurrection()", aa::setActionResurrection);
RW_FUNCTION(0x740888, "AvatarActor::updateActionDied()", aa::updateActionDied);
RW_FUNCTION(0x7408E4, "AvatarActor::setScaleX(float)", aa::setScaleX);
RW_FUNCTION(0x740944, "AvatarActor::choiceChainType(MapNode*, int)", aa::choiceChainType);
RW_FUNCTION(0x7409F8, "AvatarActor::ClearChainAttack()", aa::clearChainAttack);
RW_FUNCTION(0x740A9C, "AvatarActor::setChainAttackMotion()", aa::setChainAttackMotion);
RW_FUNCTION(0x740CF4, "AvatarActor::addPrize(MapPrize*)", aa::addPrize);
RW_FUNCTION(0x740E08, "AvatarActor::delPrize(MapPrize*, bool)", aa::delPrizeR);
RW_FUNCTION(0x740EFC, "AvatarActor::setPrizeContent(const MapPrize::Item&, bool)", aa::setPrizeContent);
RW_FUNCTION(0x741034, "AvatarActor::clearPrize()", aa::clearPrize);
RW_FUNCTION(0x741158, "AvatarActor::hasPrize()", aa::hasPrize);
RW_FUNCTION(0x741188, "AvatarActor::getShortTermFinishBonus(unsigned int)", aa::getShortTermFinishBonus);
RW_FUNCTION(0x741530, "AvatarActor::isCollisionDetectionToDestination(const Vec2&)", aa::isCollisionDetectionToDestination);
RW_FUNCTION(0x7415A4, "AvatarActor::setSefeMoveMapPotision(Vec2&)", aa::setSafeMoveMapPosition);
RW_FUNCTION(0x741650, "AvatarActor::lookAt(const Vec2&)", aa::lookAt);
RW_FUNCTION(0x741680, "AvatarActor::updateGauge(float)", aa::updateGauge);
RW_FUNCTION(0x741698, "AvatarActor::pause()", aa::pause);
RW_FUNCTION(0x7416C0, "AvatarActor::resume()", aa::resume);
RW_FUNCTION(0x7416E4, "AvatarActor::addPrize $_1 / StageFieldManager::addPrizeAttract $_1 __func::~__func()", aa::emptyLambdaDestructor);
RW_FUNCTION(0x741746, "AvatarActor::addPrize $_1 operator()(MapPrize*)", aa::addPrizeLambda);
