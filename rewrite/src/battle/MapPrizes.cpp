#include "MapPrizes.hpp"

namespace {
namespace g {
constexpr uint32_t vec2_default = rwlib::cocos2d::Vec2::ctor_base__v;           // cocos2d::Vec2::Vec2()
constexpr uint32_t vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;              // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;              // cocos2d::Vec2::~Vec2()
constexpr uint32_t aeabi_memset = 0x40E7A0;           // __aeabi_memset(dst, n, c)
constexpr uint32_t strlen_ = 0x40E08C;                // strlen
constexpr uint32_t string_init = 0x47CD93;            // std::string::__init(const char*, size_t)
constexpr uint32_t stringBuffer_ctor = 0x49140D;      // StringBuffer::StringBuffer(const char* fmt, ...)
constexpr uint32_t node_removeAllChildrenWithCleanup = rwlib::cocos2d::Node::removeAllChildrenWithCleanup;  // cocos2d::Node::removeAllChildrenWithCleanup(bool)
constexpr uint32_t node_create = rwlib::cocos2d::Node::create;            // cocos2d::Node::create()
constexpr uint32_t vector_clear = 0x5DD8B9;           // cocos2d::Vector<MapPrize*>::clear()
constexpr uint32_t vector_dtor = 0x5DD8ED;            // cocos2d::Vector<MapPrize*>::~Vector()
constexpr uint32_t vector_pushBack = 0x5DE589;        // cocos2d::Vector<MapPrize*>::pushBack(MapPrize*)
constexpr uint32_t mapNode_ctor = 0x5DA2D5;           // MapNode::MapNode()
constexpr uint32_t mapNode_dtor = 0x5DA38D;           // MapNode::~MapNode()
constexpr uint32_t mapNode_init = 0x5DA411;           // MapNode::init()
constexpr uint32_t random_getInstance = 0x492055;     // Random::getInstance()
constexpr uint32_t random_uniformInt = 0x492225;      // Random::generateUniformInt(int, int, unsigned*)
constexpr uint32_t mapPrize_createItem = 0x5DCD1D;    // MapPrize::create(Vec2, int)
constexpr uint32_t mapPrize_create = 0x5DCA85;        // MapPrize::create(Vec2, MapPrize::ID, float)
constexpr uint32_t mapPrize_setItem = 0x5DCDDD;       // MapPrize::setItem(const Item*, bool)
constexpr uint32_t mapPrize_setBone = 0x5DD379;       // MapPrize::setBone(cocostudio::Bone*)
constexpr uint32_t dm_removeDisplay = rwlib::cocostudio::DisplayManager::removeDisplay;       // cocostudio::DisplayManager::removeDisplay(int)
constexpr uint32_t dm_addDisplay = rwlib::cocostudio::DisplayManager::addDisplay;          // cocostudio::DisplayManager::addDisplay(Node*, int)
constexpr uint32_t dm_changeDisplayWithIndex = rwlib::cocostudio::DisplayManager::changeDisplayWithIndex;  // DisplayManager::changeDisplayWithIndex(int, bool)
constexpr uint32_t dm_getCurrentDisplayIndex = rwlib::cocostudio::DisplayManager::getCurrentDisplayIndex;  // DisplayManager::getCurrentDisplayIndex() const
constexpr uint32_t armature_create = rwlib::cocostudio::Armature::create;        // cocostudio::Armature::create(const std::string&)
constexpr uint32_t anim_setMovementEventCallFunc = rwlib::cocostudio::ArmatureAnimation::setMovementEventCallFunc;  // ArmatureAnimation::setMovementEventCallFunc(std::function)
constexpr uint32_t anim_setFrameEventCallFunc = rwlib::cocostudio::ArmatureAnimation::setFrameEventCallFunc;     // ArmatureAnimation::setFrameEventCallFunc(std::function)
constexpr uint32_t adm_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;        // cocostudio::ArmatureDataManager::getInstance()
constexpr uint32_t adm_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;  // ArmatureDataManager::addArmatureFileInfo(const std::string&)
constexpr uint32_t luxNumber_firstCall = 0x5D4BD1;    // LuxNumber::firstCall()
constexpr uint32_t luxNumber_create = 0x5D4AC5;       // LuxNumber::create(unsigned, Style)
constexpr uint32_t luxNumber_play = 0x5D4B49;         // LuxNumber::play()
constexpr uint32_t luxNumber_isFinish = 0x5D4B99;     // LuxNumber::isFinish()
constexpr uint32_t audio_getInstance = 0x5BECFD;      // audio::Manager::getInstance()
constexpr uint32_t audio_load = 0x5BEEF5;             // audio::Manager::load(ResourceID)
// std::function functor vtables of the armature callbacks (empty lambdas)
constexpr uint32_t fn_movementEvent = 0xF5D9D8;
constexpr uint32_t fn_frameEvent = 0xF5D9AC;
}  // namespace g

namespace str {
constexpr uint32_t ExportJsonFmt = 0xE01EED;          // "cocostudio/publish/%s.ExportJson"
constexpr uint32_t ActionMapLuxAnim = 0xE023C4;
constexpr uint32_t ActionMapPrizeAnim = 0xE0242D;
constexpr uint32_t Orbit_AllMaterial = 0xE023D5;
constexpr uint32_t Orbit_All = 0xE02423;
constexpr uint32_t Orbit_Lux = 0xE023BA;
constexpr uint32_t Orbit_TakaraNoml = 0xE023F7;
constexpr uint32_t Orbit_TakaraRare = 0xE02408;
constexpr uint32_t Treasure_Takara = 0xE023E7;        // 15 characters
constexpr uint32_t PrizeFmt = 0xE02419;               // "Prize%02d"
constexpr uint32_t LuxFmt = 0xE023B2;                 // "Lux%02d"
}  // namespace str

// cocos2d / cocostudio virtual slots used here
constexpr uint32_t VS_NODE_ADDCHILD = 60;       // Node::addChild(Node*)
constexpr uint32_t VS_NODE_ADDCHILD_Z = 61;     // Node::addChild(Node*, int z)
constexpr uint32_t VS_NODE_SETPOSITION = 19;    // Node::setPosition(const Vec2&)
constexpr uint32_t VS_NODE_SETVISIBLE = 44;     // Node::setVisible(bool)
constexpr uint32_t VS_NODE_ISVISIBLE = 45;      // Node::isVisible()
constexpr uint32_t VS_NODE_SETLOCALZORDER = 83;
constexpr uint32_t VS_ARMATURE_GETANIMATION = 166;  // Armature::getAnimation()
constexpr uint32_t VS_ARMATURE_GETBONE = 159;       // Armature::getBone(const std::string&)
constexpr uint32_t VS_BONE_GETDISPLAYMANAGER = 167; // Bone::getDisplayManager()
constexpr uint32_t VS_ANIM_PLAY = 26;           // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t VS_ANIM_GOTOANDPAUSE = 32;   // (frame 0)
// ProcessBase state queries (names guessed from use: started = slot 7 != 0, paused = slot 11 == 1)
constexpr uint32_t VS_ANIM_ISPAUSE = 7;
constexpr uint32_t VS_ANIM_ISPLAYING = 11;
constexpr uint32_t VS_ANIM_ISCOMPLETE = 13;
constexpr uint32_t VS_ANIM_RESUME = 4;

// Copy a Vec2 as raw words (the original copies with ldm/stm).
void copyVec2(bt::Vec2 *dst, const bt::Vec2 *src) {
    rw::set<uint32_t>(dst, 4, rw::get<uint32_t>(src, 4));
    rw::set<uint32_t>(dst, 0, rw::get<uint32_t>(src, 0));
}

uint32_t vcall0(uint32_t obj, uint32_t slot) { return rw::vcall(rw::host_ptr<void>(obj), slot, {obj}); }
uint32_t vcall1(uint32_t obj, uint32_t slot, uint32_t a) { return rw::vcall(rw::host_ptr<void>(obj), slot, {obj, a}); }

// std::string at frame offset `off` from a C string of known length; free it (long form) afterwards.
void makeString(const bt::GuestFrame &frame, uint32_t off, uint32_t cstr, uint32_t len) {
    rw::call(g::string_init, {frame.addr(off), cstr, len});
}
void makeString(const bt::GuestFrame &frame, uint32_t off, uint32_t cstr) {
    makeString(frame, off, cstr, rw::call(g::strlen_, {cstr}));
}
void freeString(const bt::GuestFrame &frame, uint32_t off) {
    if (rw::load<uint8_t>(frame.addr(off)) & 1) rw::call(bt::fn::operator_delete, {frame.word(off + 8)});
}

// armature->getBone(format % number): the name is formatted into a StringBuffer at bufOff, then copied to a
// std::string at strOff.
uint32_t boneByNumber(uint32_t armature, const bt::GuestFrame &frame, uint32_t bufOff, uint32_t strOff,
                      uint32_t format, int32_t number) {
    uint32_t getBone = rw::load<uint32_t>(rw::load<uint32_t>(armature) + VS_ARMATURE_GETBONE * 4);
    rw::call(g::stringBuffer_ctor, {frame.addr(bufOff), format, uint32_t(number)});
    makeString(frame, strOff, frame.addr(bufOff));
    uint32_t bone = rw::call(getBone, {armature, frame.addr(strOff)});
    freeString(frame, strOff);
    return bone;
}

// Replace display 0 of the bone with a fresh empty node (the prize is added to it).
uint32_t boneNode(uint32_t bone) {
    uint32_t node = rw::call(g::node_create, {});
    uint32_t dm = vcall0(bone, VS_BONE_GETDISPLAYMANAGER);
    rw::call(g::dm_removeDisplay, {dm, 0});
    rw::call(g::dm_addDisplay, {dm, node, 0});
    rw::call(g::dm_changeDisplayWithIndex, {dm, 0, 1});
    return node;
}

// MapPrize::create(Vec2 position, ID, 1.0f) through a temporary Vec2 at frame offset off.
uint32_t createPrize(const bt::GuestFrame &frame, uint32_t off, uint32_t position, uint32_t id) {
    rw::call(g::vec2_copy, {frame.addr(off), position});
    uint32_t prize = rw::call(g::mapPrize_create, {frame.addr(off), id, bt::bits(1.0f)});
    rw::call(g::vec2_dtor, {frame.addr(off)});
    return prize;
}

// Put a prize on the bone's node and in the list.
void attachPrize(uint32_t prize, uint32_t item, uint32_t flag, uint32_t bone, uint32_t node, bt::Vector *list) {
    rw::call(g::mapPrize_setItem, {prize, item, flag});
    rw::call(g::mapPrize_setBone, {prize, bone});
    vcall1(node, VS_NODE_ADDCHILD, prize);
    rw::call(g::vector_pushBack, {rw::arg(list), prize});
}
}  // namespace

MapPrizes::Info *MapPrizes::Info::construct(Info *self, const bt::Vec2 *position, int32_t from) {
    rw::call(g::vec2_default, {rw::arg(self)});
    for (MapPrizeItem &item : self->items) {
        item.type = 1;
        item.id = 0;
        item.count = 0;
        item.rare = 0;
    }
    self->from = 0;
    self->luxStyle = 0;
    copyVec2(&self->position, position);
    self->from = from;
    return self;
}
RW_FUNCTION(0x5DB934, "MapPrizes::Info::Info", MapPrizes::Info::construct);

MapPrizes *MapPrizes::destruct(MapPrizes *self) {
    self->vtable = VTABLE;
    rw::call(g::node_removeAllChildrenWithCleanup, {rw::arg(self), 1});
    rw::call(g::vector_clear, {rw::arg(&self->orbitPrizes)});
    rw::call(g::vector_clear, {rw::arg(&self->luxPrizes)});
    rw::call(g::vector_clear, {rw::arg(&self->treasurePrizes)});
    uint32_t lux = self->luxNumber;
    if (lux) {
        if (vcall0(lux, 71) != 0)                 // getParent()
            vcall0(self->luxNumber, 73);           // removeFromParent()
    }
    rw::call(g::vector_dtor, {rw::arg(&self->treasurePrizes)});
    rw::call(g::vector_dtor, {rw::arg(&self->luxPrizes)});
    rw::call(g::vector_dtor, {rw::arg(&self->orbitPrizes)});
    rw::call(g::mapNode_dtor, {rw::arg(self)});   // MapNode::~MapNode
    return self;
}
RW_FUNCTION(0x5DD7D0, "MapPrizes::~MapPrizes", MapPrizes::destruct);

void MapPrizes::destructDelete(MapPrizes *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {rw::arg(self)});
}
RW_FUNCTION(0x5DD918, "MapPrizes::~MapPrizes(deleting)", MapPrizes::destructDelete);

uint32_t MapPrizes::init(MapPrizes *self, Info *info, uint32_t parent) {
    if (rw::call(g::mapNode_init, {rw::arg(self)}) != 1) return 0;   // MapNode::init
    self->parent = parent;
    vcall1(rw::arg(self), VS_NODE_SETLOCALZORDER, 4);
    if (info->from == 3) initMaterialPrizes(self, info);
    else initNormalPrizes(self, info);
    copyVec2(&self->position, &info->position);
    return 1;
}
RW_FUNCTION(0x5DD924, "MapPrizes::init", MapPrizes::init);

// Material drop: the all-material orbit; the material prize sits on one random bone of the 15.
void MapPrizes::initMaterialPrizes(MapPrizes *self, Info *info) {
    if (self->orbitArmature == 0) {
        self->orbitArmature = createArmature(nullptr,
                                             rw::host_ptr<const char>(str::ActionMapPrizeAnim),
                                             rw::host_ptr<const char>(str::Orbit_AllMaterial));
        rw::vcall(self, VS_NODE_ADDCHILD_Z, {rw::arg(self), self->orbitArmature, 11});
    }
    uint32_t rnd = rw::call(g::random_getInstance, {});
    int32_t chosen = int32_t(rw::call(g::random_uniformInt, {rnd, 0, 14, 0}));
    enum : uint32_t { VEC = 0x28, STR = 0x30, BUF = 0x3C };
    bt::GuestFrame frame(0x450);
    int32_t k = 0;
    do {
        uint32_t bone = boneByNumber(self->orbitArmature, frame, BUF, STR, str::PrizeFmt, k + 1);
        if (bone) {
            uint32_t node = boneNode(bone);
            if (chosen == k) {
                rw::call(g::vec2_copy, {frame.addr(VEC), rw::arg(info)});
                uint32_t prize = rw::call(g::mapPrize_createItem, {frame.addr(VEC), uint32_t(info->items[0].id)});
                rw::call(g::vec2_dtor, {frame.addr(VEC)});
                if (prize) attachPrize(prize, rw::arg(&info->items[0]), 0, bone, node, &self->orbitPrizes);
            }
        }
        k++;
    } while (k < 15);
}
RW_FUNCTION(0x5DD988, "MapPrizes::initMaterialPrizes", MapPrizes::initMaterialPrizes);

namespace {
// Coin / munny piles: up to four prize slots from `cur`; the amount thresholds pick how many piles and whether
// they are the small (low) or large (high) pile. Returns the number of slots used; items[cur] = the item.
int32_t fillPiles(const bt::GuestFrame &frame, uint32_t idsOff, uint32_t itemsOff, int32_t cur, int32_t n,
                  const int32_t (&threshold)[8], uint32_t low, uint32_t high, uint32_t item) {
    auto id = [&](int32_t k, uint32_t v) { frame.setWord(idsOff + 4 * uint32_t(cur + k), v); };
    int32_t used = 0;
    for (int32_t k = 0; k < 4; k++) {
        if (n < threshold[k]) break;
        id(k, low);
        used = k + 1;
    }
    if (used == 4) {
        for (int32_t k = 0; k < 4; k++) {
            if (n < threshold[4 + k]) break;
            id(k, high);
        }
    }
    frame.setWord(itemsOff + 4 * uint32_t(cur), item);
    return used;
}
}  // namespace

// Enemy / treasure drop: items go directly into the treasure list (or Lux via initLuxPrizes); munny and coins
// become piles placed on random bones of the "Orbit_All" armature; then the treasure box armature (rare or normal)
// holds the first direct prize.
void MapPrizes::initNormalPrizes(MapPrizes *self, Info *info) {
    // guest frame offsets as in the original
    enum : uint32_t { STR_T = 0x38, VEC_O = 0x44, STR = 0x4C, VEC_I = 0x58, VEC_L = 0x60, BUF = 0x68,
                      ITEMS = 0x46C, IDS = 0x4A8, ORDER = 0x4E4 };
    bt::GuestFrame frame(0x528);
    rw::call(g::aeabi_memset, {frame.addr(ITEMS), 0x3C, 0});
    for (uint32_t k = 0; k < 15; k++) {
        frame.setWord(ORDER + 4 * k, k);
        frame.setWord(IDS + 4 * k, 12);
    }
    shuffleArray(nullptr, frame.ptr<int32_t>(ORDER), 15);
    static constexpr int32_t MUNNY[8] = {1, 101, 501, 1001, 5001, 10001, 15001, 20001};
    static constexpr int32_t COINS[8] = {1, 11, 26, 51, 101, 201, 301, 401};
    int32_t piles = 0, rare = 0;
    for (MapPrizeItem &item : info->items) {
        int32_t n = item.count;
        if (n == 0) continue;
        int32_t type = item.type;
        uint32_t itemAddr = rw::arg(&item);
        switch (type) {
        case 0:
        case 1:
            break;
        case 4:
            if (piles + 4 <= 14) piles += fillPiles(frame, IDS, ITEMS, piles, n, MUNNY, 0, 1, itemAddr);
            break;
        case 8:
            if (piles + 4 <= 14) piles += fillPiles(frame, IDS, ITEMS, piles, n, COINS, 4, 5, itemAddr);
            break;
        case 9:
            if (piles + 4 <= 14) piles += fillPiles(frame, IDS, ITEMS, piles, n, COINS, 2, 3, itemAddr);
            break;
        case 11:
            rw::call(g::vec2_copy, {frame.addr(VEC_L), rw::arg(info)});
            initLuxPrizes(self, &item, frame.ptr<bt::Vec2>(VEC_L), info->luxStyle);
            rw::call(g::vec2_dtor, {frame.addr(VEC_L)});
            break;
        default: {
            if (type > 20) break;
            uint32_t id = info->from == 1 ? 6 : 8;
            if (item.rare != 0) {
                id |= 1;
                rare++;
            }
            uint32_t prize = createPrize(frame, VEC_I, rw::arg(info), id);
            if (prize) {
                rw::call(g::mapPrize_setItem, {prize, itemAddr, 0});
                rw::call(g::vector_pushBack, {rw::arg(&self->treasurePrizes), prize});
            }
            break;
        }
        }
    }
    if (piles >= 1 && self->orbitArmature == 0) {
        self->orbitArmature = createArmature(nullptr,
                                             rw::host_ptr<const char>(str::ActionMapPrizeAnim),
                                             rw::host_ptr<const char>(str::Orbit_All));
        rw::vcall(self, VS_NODE_ADDCHILD_Z, {rw::arg(self), self->orbitArmature, 10});
        int32_t k = 0;
        do {
            int32_t bonenum = int32_t(frame.word(ORDER + 4 * uint32_t(k)));
            uint32_t bone = boneByNumber(self->orbitArmature, frame, BUF, STR, str::PrizeFmt, bonenum + 1);
            if (bone) {
                uint32_t node = boneNode(bone);
                if (k < piles) {
                    uint32_t prize = createPrize(frame, VEC_O, rw::arg(info), frame.word(IDS + 4 * uint32_t(k)));
                    if (prize)
                        attachPrize(prize, frame.word(ITEMS + 4 * uint32_t(k)), 0, bone, node, &self->orbitPrizes);
                }
            }
            k++;
        } while (k < 15);
    }
    if (int32_t(self->treasurePrizes.end.get() - self->treasurePrizes.begin.get()) >= 1) {
        uint32_t movement = rare > 0 ? str::Orbit_TakaraRare : str::Orbit_TakaraNoml;
        self->treasureArmature = createArmature(nullptr,
                                                rw::host_ptr<const char>(str::ActionMapPrizeAnim),
                                                rw::host_ptr<const char>(movement));
        rw::vcall(self, VS_NODE_ADDCHILD_Z, {rw::arg(self), self->treasureArmature, 12});
        uint32_t armature = self->treasureArmature;
        uint32_t getBone = rw::load<uint32_t>(rw::load<uint32_t>(armature) + VS_ARMATURE_GETBONE * 4);
        makeString(frame, STR_T, str::Treasure_Takara, 15);
        uint32_t bone = rw::call(getBone, {armature, frame.addr(STR_T)});
        freeString(frame, STR_T);
        if (bone) {
            uint32_t first = rw::load<uint32_t>(self->treasurePrizes.begin);
            uint32_t dm = vcall0(bone, VS_BONE_GETDISPLAYMANAGER);
            uint32_t index = rw::call(g::dm_getCurrentDisplayIndex, {dm});
            uint32_t node = rw::call(g::node_create, {});
            rw::call(g::dm_removeDisplay, {dm, index});
            rw::call(g::dm_addDisplay, {dm, node, index});
            rw::call(g::dm_changeDisplayWithIndex, {dm, index, 1});
            vcall1(node, VS_NODE_ADDCHILD, first);
            rw::call(g::mapPrize_setBone, {first, bone});
        }
    }
}
RW_FUNCTION(0x5DDB80, "MapPrizes::initNormalPrizes", MapPrizes::initNormalPrizes);

// Swap n random pairs.
void MapPrizes::shuffleArray(MapPrizes *, const int32_t *array, int32_t n) {
    int32_t last = int32_t(uint32_t(n) - 1);
    if (n < 1) return;
    int32_t k = -1;
    do {
        uint32_t rnd = rw::call(g::random_getInstance, {});
        uint32_t a = rw::call(g::random_uniformInt, {rnd, 0, uint32_t(last), 0});
        rnd = rw::call(g::random_getInstance, {});
        uint32_t b = rw::call(g::random_uniformInt, {rnd, 0, uint32_t(last), 0});
        uint32_t pa = rw::arg(array) + 4 * a, pb = rw::arg(array) + 4 * b;
        uint32_t va = rw::load<uint32_t>(pa), vb = rw::load<uint32_t>(pb);
        rw::save<uint32_t>(pa, vb);
        rw::save<uint32_t>(pb, va);
        k++;
    } while (k < last);
}
RW_FUNCTION(0x5DE24C, "MapPrizes::shuffleArray", MapPrizes::shuffleArray);

// Lux drop: big (200 each) and small (10 each) Lux piles on random bones of the Lux orbit, plus the LuxNumber.
void MapPrizes::initLuxPrizes(MapPrizes *self, MapPrizeItem *item, const bt::Vec2 *position, uint32_t style) {
    enum : uint32_t { VEC = 0x38, STR = 0x40, BUF = 0x4C, IDS = 0x450, ORDER = 0x4A0 };
    bt::GuestFrame frame(0x4F8);
    if (self->luxArmature == 0) {
        self->luxArmature = createArmature(nullptr,
                                           rw::host_ptr<const char>(str::ActionMapLuxAnim),
                                           rw::host_ptr<const char>(str::Orbit_Lux));
        rw::vcall(self, VS_NODE_ADDCHILD_Z, {rw::arg(self), self->luxArmature, 13});
    }
    for (uint32_t k = 0; k < 20; k++) {
        frame.setWord(ORDER + 4 * k, k);
        frame.setWord(IDS + 4 * k, 12);
    }
    uint32_t n = uint32_t(item->count);
    int32_t used = 0;
    bool small = true;
    if (n >= 200) {
        int32_t left = int32_t(n / 200 + 1);
        int32_t k = 0;
        for (;;) {
            frame.setWord(IDS + 4 * uint32_t(k), 11);     // large Lux pile
            if (k >= 19) break;
            left--;
            if (left < 2) break;
            k++;
        }
        used = k + 1;
        small = used <= 19;
    }
    if (small) {
        int32_t slot = int32_t((n % 200) / 10);
        int32_t k;
        for (;;) {
            k = used;
            frame.setWord(IDS + 4 * uint32_t(slot), 10);  // small Lux pile (indexed by the countdown)
            used = k + 1;
            if (k >= 19) break;
            int32_t old = slot;
            slot--;
            if (old < 1) break;
        }
        used = k + 1;
    }
    shuffleArray(nullptr, frame.ptr<int32_t>(ORDER), 20);
    for (int32_t k = 0; k < 20; k++) {
        int32_t bonenum = int32_t(frame.word(ORDER + 4 * uint32_t(k)));
        uint32_t bone = boneByNumber(self->luxArmature, frame, BUF, STR, str::LuxFmt, bonenum + 1);
        if (!bone) continue;
        uint32_t node = boneNode(bone);
        if (k >= used) continue;
        uint32_t prize = createPrize(frame, VEC, rw::arg(position), frame.word(IDS + 4 * uint32_t(k)));
        if (prize) attachPrize(prize, rw::arg(item), k != 0 ? 1 : 0, bone, node, &self->luxPrizes);
    }
    uint32_t lux = rw::call(g::luxNumber_create, {uint32_t(item->count), style});   // LuxNumber::create
    self->luxNumber = lux;
    self->luxAmount = uint32_t(item->count);
    if (lux) {
        uint32_t parent = self->parent;
        if (parent) vcall1(parent, VS_NODE_ADDCHILD, lux);
        else rw::vcall(self, VS_NODE_ADDCHILD_Z, {rw::arg(self), lux, 23});
    }
}
RW_FUNCTION(0x5DE2A8, "MapPrizes::initLuxPrizes", MapPrizes::initLuxPrizes);

// Armature `file` paused on frame 0 of `movement`, hidden, with empty movement / frame event callbacks.
uint32_t MapPrizes::createArmature(MapPrizes *, const char *file, const char *movement) {
    enum : uint32_t { STR_M = 0x10, STR_F = 0x1C, FN_FRAME = 0x28, FN_MOVE = 0x40 };
    bt::GuestFrame frame(0x68);
    makeString(frame, STR_F, rw::guest_addr(file));
    uint32_t armature = rw::call(g::armature_create, {frame.addr(STR_F)});   // cocostudio::Armature::create
    freeString(frame, STR_F);
    uint32_t anim = vcall0(armature, VS_ARMATURE_GETANIMATION);
    uint32_t play = rw::load<uint32_t>(rw::load<uint32_t>(anim) + VS_ANIM_PLAY * 4);
    makeString(frame, STR_M, rw::guest_addr(movement));
    rw::call(play, {anim, frame.addr(STR_M), 0xFFFFFFFFu, 0xFFFFFFFFu});
    freeString(frame, STR_M);
    vcall1(anim, VS_ANIM_GOTOANDPAUSE, 0);
    frame.setWord(FN_MOVE + 16, frame.addr(FN_MOVE));
    frame.setWord(FN_MOVE, g::fn_movementEvent);
    rw::call(g::anim_setMovementEventCallFunc, {anim, frame.addr(FN_MOVE)});
    uint32_t f = frame.word(FN_MOVE + 16);
    if (f == frame.addr(FN_MOVE)) vcall0(f, 4);
    else if (f) vcall0(f, 5);
    frame.setWord(FN_FRAME + 16, frame.addr(FN_FRAME));
    frame.setWord(FN_FRAME, g::fn_frameEvent);
    rw::call(g::anim_setFrameEventCallFunc, {anim, frame.addr(FN_FRAME)});
    f = frame.word(FN_FRAME + 16);
    if (f == frame.addr(FN_FRAME)) vcall0(f, 4);
    else if (f) vcall0(f, 5);
    vcall1(armature, VS_NODE_SETVISIBLE, 0);
    return armature;
}
RW_FUNCTION(0x5DE5F0, "MapPrizes::createArmature", MapPrizes::createArmature);

// Resume a paused animation; 1 = started.
uint32_t MapPrizes::playArmature(MapPrizes *, uint32_t armature) {
    if (!armature) return 0;
    uint32_t anim = vcall0(armature, VS_ARMATURE_GETANIMATION);
    if (vcall0(anim, VS_ANIM_ISPAUSE) != 0) return 0;
    if (vcall0(anim, VS_ANIM_ISPLAYING) != 1) return 0;
    vcall0(anim, VS_ANIM_RESUME);
    return 1;
}
RW_FUNCTION(0x5DE7B0, "MapPrizes::playArmature", MapPrizes::playArmature);

uint32_t MapPrizes::isFinishPlayArmature(MapPrizes *, uint32_t armature) {
    if (!armature) return 1;
    uint32_t anim = vcall0(armature, VS_ARMATURE_GETANIMATION);
    if (vcall0(anim, VS_ANIM_ISPAUSE) == 0) return 0;
    return vcall0(anim, VS_ANIM_ISCOMPLETE);
}
RW_FUNCTION(0x5DE7F8, "MapPrizes::isFinishPlayArmature", MapPrizes::isFinishPlayArmature);

uint32_t MapPrizes::eruptionStartArmature(MapPrizes *self, uint32_t armature, bt::Vector *prizes) {
    uint32_t r = playArmature(self, armature);
    if (r != 1) return r;
    r = prizes->end.get() - prizes->begin.get();
    if (int32_t(r) < 1) return r;
    return vcall1(armature, VS_NODE_SETVISIBLE, 1);
}
RW_FUNCTION(0x5DE832, "MapPrizes::eruptionStartArmature", MapPrizes::eruptionStartArmature);

// Same, moving every prize to `position` first.
uint32_t MapPrizes::eruptionStartArmatureAt(MapPrizes *self, uint32_t armature, bt::Vector *prizes,
                                            const bt::Vec2 *position) {
    uint32_t r = playArmature(self, armature);
    if (r != 1) return r;
    uint32_t it = prizes->begin, end = prizes->end;
    if (int32_t(end - it) < 1) return it;
    for (; it != end; it += 4) {
        bt::Vec2 *p = rw::host_ptr<bt::Vec2>(rw::load<uint32_t>(it) + 624);
        copyVec2(p, position);
    }
    return vcall1(armature, VS_NODE_SETVISIBLE, 1);
}
RW_FUNCTION(0x5DE85C, "MapPrizes::eruptionStartArmature(Vec2)", MapPrizes::eruptionStartArmatureAt);

MapPrizes *MapPrizes::create(Info *info, uint32_t parent) {
    auto *self = rw::host_ptr<MapPrizes>(rw::call(bt::fn::operator_new, {SIZE}));
    rw::call(g::mapNode_ctor, {rw::arg(self)});   // MapNode::MapNode
    self->vtable = VTABLE;
    rw::call(g::aeabi_memset, {rw::arg(self) + 648, 0x3C, 0});
    if (init(self, info, parent) == 1) {
        rw::call(bt::fn::ref_autorelease, {rw::arg(self)});
        return self;
    }
    rw::vcall(self, 1, {rw::arg(self)});           // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5DE8AC, "MapPrizes::create", MapPrizes::create);

// Preload the prize / Lux armatures, LuxNumber resources and the prize sound.
void MapPrizes::firstCall() {
    enum : uint32_t { STR2 = 0x10, STR1 = 0x1C, BUF2 = 0x28, BUF1 = 0x42C };
    bt::GuestFrame frame(0x838);
    uint32_t adm = rw::call(g::adm_getInstance, {});
    rw::call(g::stringBuffer_ctor, {frame.addr(BUF1), str::ExportJsonFmt, str::ActionMapPrizeAnim});
    makeString(frame, STR1, frame.addr(BUF1));
    rw::call(g::adm_addArmatureFileInfo, {adm, frame.addr(STR1)});
    freeString(frame, STR1);
    adm = rw::call(g::adm_getInstance, {});
    rw::call(g::stringBuffer_ctor, {frame.addr(BUF2), str::ExportJsonFmt, str::ActionMapLuxAnim});
    makeString(frame, STR2, frame.addr(BUF2));
    rw::call(g::adm_addArmatureFileInfo, {adm, frame.addr(STR2)});
    freeString(frame, STR2);
    rw::call(g::luxNumber_firstCall, {});
    uint32_t audio = rw::call(g::audio_getInstance, {});
    rw::call(g::audio_load, {audio, 0x1E56BB16});
}
RW_FUNCTION(0x5DE938, "MapPrizes::firstCall", MapPrizes::firstCall);

void MapPrizes::finalCall() {}
RW_FUNCTION(0x5DEA98, "MapPrizes::finalCall", MapPrizes::finalCall);

// Start all eruption animations and the Lux counter.
uint32_t MapPrizes::eruption(MapPrizes *self) {
    eruptionStartArmature(self, self->orbitArmature, &self->orbitPrizes);
    eruptionStartArmature(self, self->luxArmature, &self->luxPrizes);
    eruptionStartArmature(self, self->treasureArmature, &self->treasurePrizes);
    uint32_t lux = self->luxNumber;
    if (lux) return rw::call(g::luxNumber_play, {lux});
    return lux;
}
RW_FUNCTION(0x5DEA9A, "MapPrizes::eruption", MapPrizes::eruption);

uint32_t MapPrizes::eruptionAt(MapPrizes *self, const bt::Vec2 *position, const bt::Vec2 *luxPosition) {
    copyVec2(&self->position, position);
    eruptionStartArmatureAt(self, self->orbitArmature, &self->orbitPrizes, position);
    eruptionStartArmatureAt(self, self->luxArmature, &self->luxPrizes, position);
    eruptionStartArmatureAt(self, self->treasureArmature, &self->treasurePrizes, position);
    uint32_t lux = self->luxNumber;
    if (!lux) return 0;
    vcall1(lux, VS_NODE_SETPOSITION, rw::arg(luxPosition));
    return rw::call(g::luxNumber_play, {self->luxNumber});
}
RW_FUNCTION(0x5DEAE0, "MapPrizes::eruption(Vec2, Vec2)", MapPrizes::eruptionAt);

uint32_t MapPrizes::isEruption(MapPrizes *self) {
    uint32_t r = 0;
    if (uint32_t a = self->orbitArmature) r = vcall0(a, VS_NODE_ISVISIBLE);
    if (uint32_t a = self->luxArmature) r |= vcall0(a, VS_NODE_ISVISIBLE);
    if (uint32_t a = self->treasureArmature) r |= vcall0(a, VS_NODE_ISVISIBLE);
    return r;
}
RW_FUNCTION(0x5DEB58, "MapPrizes::isEruption", MapPrizes::isEruption);

namespace {
// The isFinishPlayArmature test, inlined by the original (no armature = finished).
uint32_t finished(uint32_t armature) {
    if (!armature) return 1;
    uint32_t anim = vcall0(armature, VS_ARMATURE_GETANIMATION);
    if (vcall0(anim, VS_ANIM_ISPAUSE) == 0) return 0;
    return vcall0(anim, VS_ANIM_ISCOMPLETE);
}
}  // namespace

uint32_t MapPrizes::isFinishEruption(MapPrizes *self) {
    uint32_t r = finished(self->orbitArmature);
    r &= finished(self->luxArmature);
    r &= finished(self->treasureArmature);
    if (uint32_t lux = self->luxNumber) r &= rw::call(g::luxNumber_isFinish, {lux});
    return r;
}
RW_FUNCTION(0x5DEBA4, "MapPrizes::isFinishEruption", MapPrizes::isFinishEruption);
