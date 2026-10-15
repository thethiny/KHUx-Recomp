#include "MedalDeck.hpp"

namespace {
constexpr uint32_t Node_ctor = 0x892531;               // cocos2d::Node::Node()
constexpr uint32_t Node_dtor = 0x8927ED;               // cocos2d::Node::~Node()
constexpr uint32_t Ref_autorelease = 0x8FD0D5;         // cocos2d::Ref::autorelease()
constexpr uint32_t SafeBuffer_ctor = 0x459F21;         // SafeBuffer::SafeBuffer(size, registerForUpdate)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;         // SafeBuffer::~SafeBuffer()
constexpr uint32_t SafeBuffer_read = 0x45A269;         // SafeBuffer::read(dst, src, n, index) const
constexpr uint32_t SafeBuffer_write = 0x459FA1;        // SafeBuffer::write(dst, src, n, index)
constexpr uint32_t SafeBuffer_moveAssign = 0x45A355;   // SafeBuffer::operator=(SafeBuffer&&)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t UserMedalIndex_find = 0x490275;     // unordered_map<unsigned long long, int>::find
constexpr uint32_t master_Medal_create = 0x785871;     // master::Medal::create(int) (sret shared_ptr)
constexpr uint32_t master_Burst_create = 0x77A491;     // master::Burst::create(int) (sret shared_ptr)
constexpr uint32_t BattleUtil_calcMedalStatus = 0x4966BD;  // (medal, userMedal, function<void(int,int)>, keyblade, index)
constexpr uint32_t FunctionVoid_call = 0x470E69;       // std::function<void()>::operator()
constexpr uint32_t FunctionMedalInfo_call = 0x5E00C1;  // std::function<bool(const MedalInfo&)>::operator()
constexpr uint32_t FunctionMedal_call = 0x5E03B5;      // std::function<void(const shared_ptr<const Medal>&)>::operator()
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;            // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t AEABI_MEMCPY = 0x40E164;            // __aeabi_memcpy(dst, src, n) (import)
constexpr uint32_t AEABI_IDIV = 0xCD5E8D;              // __aeabi_idiv

constexpr uint32_t APIMANAGER_USER_MEDAL = 0xD4;       // APIManager + 212: UserMedal* (SafeBuffer of 72-byte rows)
constexpr uint32_t USER_MEDAL_INDEX = 0x24;            // UserMedal + 36: unordered_map<medal uid, row>
constexpr uint32_t USER_MEDAL_ROW = 72;                // hole::network::api::UserMedal::Medal
constexpr uint32_t VSLOT_INIT = 146;                   // cocos2d::Node::init (vtable + 584)

constexpr uint32_t SB_DATA = 12;                       // SafeBuffer::data

uint32_t sbData(uint32_t sb) { return rw::load<uint32_t>(sb + SB_DATA); }

// sb->read(dst, sb->data + index, n, index)
uint32_t sbRead(uint32_t sb, uint32_t dst, uint32_t index, uint32_t n) {
    return uint32_t(rw::call_exact(SafeBuffer_read, {sb, dst, sbData(sb) + index, n, index}));
}
// sb->write(sb->data + index, src, n, index)
void sbWrite(uint32_t sb, uint32_t src, uint32_t index, uint32_t n) {
    rw::call_exact(SafeBuffer_write, {sb, sbData(sb) + index, src, n, index});
}

// The inline ~function(): destroy() for a callable in the inline buffer, destroy_deallocate() for a heap one.
void destroyFunction(uint32_t fn) {
    uint32_t impl = rw::load<uint32_t>(fn + 16);
    if (impl == fn) rw::vcall(rw::host_ptr<uint8_t>(impl), 4, {impl});
    else if (impl) rw::vcall(rw::host_ptr<uint8_t>(impl), 5, {impl});
}

uint32_t addr(const void *p) { return rw::guest_addr(p); }

// The slot in use: currentIndex during the player's turn, else the first medal.
int32_t currentSlot(const MedalDeck *self) { return self->state == MedalDeck::STATE_PLAYER ? int32_t(self->currentIndex) : 0; }
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
MedalDeck *MedalDeck::construct(MedalDeck *self) {
    MedalDeck *node = rw::host_ptr<MedalDeck>(rw::call(Node_ctor, {addr(self)}));
    rw::set<uint32_t>(node, 0, VTABLE);
    for (int k = 0; k < 6; k++) {
        MedalInfo &info = node->medals[k];
        info.medal.obj = 0;
        info.medal.ctrl = 0;
        rw::call(SafeBuffer_ctor, {addr(&info.property), 0x20, 1});
        info.property.vtable = PROPERTY_VTABLE;
    }
    self->onAdvance.impl = 0;
    self->onDeckEnd.impl = 0;
    self->onTurnStart.impl = 0;
    rw::call(AEABI_MEMSET, {addr(&self->deckCount), 0x14, 0});  // deckCount .. currentIndex
    return self;
}
RW_FUNCTION(0x763B3C, "MedalDeck::MedalDeck()", MedalDeck::construct);

MedalDeck *MedalDeck::destruct(MedalDeck *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    destroyFunction(addr(&self->onTurnStart));
    destroyFunction(addr(&self->onDeckEnd));
    destroyFunction(addr(&self->onAdvance));
    for (int k = 5; k >= 0; k--) {
        MedalInfo &info = self->medals[k];
        rw::call(SafeBuffer_dtor, {addr(&info.property)});
        if (info.medal.ctrl) guest::release(info.medal.ctrl);
    }
    rw::call(Node_dtor, {addr(self)});
    return self;
}
RW_FUNCTION(0x5DFA90, "MedalDeck::~MedalDeck()", MedalDeck::destruct);

// CREATE_FUNC: new + init, autoreleased; a failed init deletes the node.
uint32_t MedalDeck::create() {
    uint32_t obj = guest::newObject(sizeof(MedalDeck));
    construct(rw::host_ptr<MedalDeck>(obj));
    if (rw::vcall(rw::host_ptr<uint8_t>(obj), VSLOT_INIT, {obj}) == 1) {
        rw::call(Ref_autorelease, {obj});
        return obj;
    }
    rw::vcall(rw::host_ptr<uint8_t>(obj), 1, {obj});  // deleting destructor
    return 0;
}
RW_FUNCTION(0x75E8D4, "MedalDeck::create", MedalDeck::create);

// ------------------------------------------------------------------------------------------- filling the deck
// Fill the deck from the party's medal uids (up to 5): each uid is looked up in the user's medals (APIManager);
// an unknown uid gives a blank medal (zeros, ids 1 / 1).
uint32_t MedalDeck::addMedalOntoDeck(MedalDeck *self, uint32_t keyblade, uint32_t medalIds, int32_t count,
                                     uint32_t onAdded) {
    guest::Frame f(0xD8);                 // push 5 + sub 0xC4: addOntoDeck's frame sits where the original's did
    const uint32_t row = f.addr(0x28), userMedal = f.addr(0x70);
    int32_t n = count > 5 ? 5 : count;
    self->deckCount = n;
    if (n >= 1) {
        for (int32_t k = 0; k != n; k++) {
            uint32_t api = rw::call(APIManager_getInstance, {});
            uint32_t medals = rw::load<uint32_t>(api + APIMANAGER_USER_MEDAL);
            uint32_t idLo = rw::load<uint32_t>(medalIds + 8u * uint32_t(k));
            uint32_t idHi = rw::load<uint32_t>(medalIds + 8u * uint32_t(k) + 4);
            *f.at<uint32_t>(0x74) = idHi;
            *f.at<uint32_t>(0x70) = idLo;
            uint32_t node = rw::call(UserMedalIndex_find, {medals + USER_MEDAL_INDEX, userMedal});
            int32_t index;
            if (node && (index = rw::load<int32_t>(node + 16)) >= 0) {
                uint32_t off = uint32_t(index) * USER_MEDAL_ROW;
                rw::call_exact(SafeBuffer_read, {medals, row, sbData(medals) + off, USER_MEDAL_ROW, off});
                rw::call(AEABI_MEMCPY, {userMedal, row, USER_MEDAL_ROW});
            } else {
                rw::call(AEABI_MEMSET, {userMedal, USER_MEDAL_ROW, 0});
                *f.at<uint32_t>(0x78) = 1;
                *f.at<uint32_t>(0x7C) = 1;
            }
            addOntoDeck(self, userMedal, &self->medals[k], onAdded, keyblade, k);
        }
    }
    self->currentIndex = 0;
    return 0;
}
RW_FUNCTION(0x5DFB50, "MedalDeck::addMedalOntoDeck", MedalDeck::addMedalOntoDeck);

// Fill one slot: the master medal row, the battle status (calcMedalStatus writes attack / defense through the
// $_0 callback into a fresh Property buffer), burst level, guilt, the burst targeting flag and the supporting
// user; then report the medal through onAdded.
uint32_t MedalDeck::addOntoDeck(MedalDeck *self, uint32_t userMedal, MedalInfo *info, uint32_t onAdded,
                                uint32_t keyblade, int32_t index) {
    (void)self;
    // push 5 + sub 0x74: the Property buffer at +0x24 is registered in the SafeBuffer update list, so the frame
    // must sit at the original address
    guest::Frame f(0x88);
    const uint32_t zero8 = f.addr(0x10), singleSelect = f.addr(0x1B), keybladeCopy = f.addr(0x1C);
    const uint32_t property = f.addr(0x24), medal = f.addr(0x44), burstId = f.addr(0x4C), status = f.addr(0x50);

    rw::call(master_Medal_create, {medal, rw::load<uint32_t>(userMedal + 8)});
    rw::call(SafeBuffer_ctor, {property, 0x20, 1});
    *f.at<uint32_t>(0x24) = PROPERTY_VTABLE;
    // std::function<void(int,int)> holding $_0 (captures the Property buffer) in its inline buffer
    *f.at<uint32_t>(0x60) = status;
    *f.at<uint32_t>(0x50) = ADD_ONTO_DECK_LAMBDA_VTABLE;
    *f.at<uint32_t>(0x54) = property;
    // the keyblade shared_ptr passed by value
    uint32_t kbCtrl = rw::load<uint32_t>(keyblade + 4);
    *f.at<uint32_t>(0x1C) = rw::load<uint32_t>(keyblade);
    *f.at<uint32_t>(0x20) = kbCtrl;
    if (kbCtrl) guest::addRef(kbCtrl);
    rw::call_exact(BattleUtil_calcMedalStatus, {medal, userMedal, status, keybladeCopy, uint32_t(index)});
    if (uint32_t c = rw::load<uint32_t>(f.addr(0x20))) guest::release(c);
    destroyFunction(status);

    sbWrite(property, userMedal + 0x1C, MedalInfo::BURST_LEVEL, 4);
    sbWrite(property, userMedal + 0x28, MedalInfo::GUILT, 4);
    uint32_t medalRow = rw::load<uint32_t>(medal);
    uint32_t r = sbRead(medalRow, burstId, MEDAL_BURST_ID, 4);
    *f.at<uint8_t>(0x1B) = uint8_t(isBurstSingleSelect(r, rw::load<int32_t>(burstId)));
    sbWrite(property, singleSelect, MedalInfo::BURST_SINGLE_SELECT, 1);
    if (rw::load<uint32_t>(userMedal + 0x40)) {
        sbWrite(property, userMedal + 0x30, MedalInfo::SUPPORT_USER, 8);
    } else {
        *f.at<uint32_t>(0x10) = 0;
        *f.at<uint32_t>(0x14) = 0;
        sbWrite(property, zero8, MedalInfo::SUPPORT_USER, 8);
    }

    // info.medal = medal; info.property = std::move(property)
    uint32_t obj = rw::load<uint32_t>(medal), ctrl = rw::load<uint32_t>(medal + 4);
    if (ctrl) guest::addRef(ctrl);
    info->medal.obj = obj;
    uint32_t old = info->medal.ctrl;
    info->medal.ctrl = ctrl;
    if (old) guest::release(old);
    rw::call(SafeBuffer_moveAssign, {addr(&info->property), property});
    if (rw::load<uint32_t>(onAdded + 16)) rw::call(FunctionMedal_call, {onAdded, medal});
    rw::call(SafeBuffer_dtor, {property});
    if (uint32_t c = rw::load<uint32_t>(medal + 4)) guest::release(c);
    return 0;
}
RW_FUNCTION(0x5DFC60, "MedalDeck::addOntoDeck", MedalDeck::addOntoDeck);

// The friend medal goes after the deck medals (once).
uint32_t MedalDeck::addFriendMedalOntoDeck(MedalDeck *self, uint32_t keyblade, uint32_t userMedal, uint32_t onAdded) {
    guest::Frame f(0x20);                 // push 5 + sub 0xC
    if (self->friendCount != 0) return addr(self);
    addOntoDeck(self, userMedal, &self->medals[self->deckCount], onAdded, keyblade, -1);
    self->friendCount = int32_t(uint32_t(self->friendCount.get()) + 1);
    return uint32_t(self->friendCount.get());
}
RW_FUNCTION(0x5DFE6A, "MedalDeck::addFriendMedalOntoDeck", MedalDeck::addFriendMedalOntoDeck);

// addOntoDeck $_0: (attack, defense) -> Property data 0 / 4
void MedalDeck::addOntoDeckLambda(uint32_t func, const int32_t *attack, const int32_t *defense) {
    guest::Frame f(0x28);                 // push 5 + sub 0x14
    *f.at<int32_t>(0xC) = *attack;
    *f.at<int32_t>(0x8) = *defense;
    uint32_t property = rw::load<uint32_t>(func + 4);
    sbWrite(property, f.addr(0xC), MedalInfo::ATTACK, 4);
    property = rw::load<uint32_t>(func + 4);
    sbWrite(property, f.addr(0x8), MedalInfo::DEFENSE, 4);
}
RW_FUNCTION(0x5E138C, "MedalDeck::addOntoDeck $_0 __func::operator()(int&&, int&&)", MedalDeck::addOntoDeckLambda);

// ------------------------------------------------------------------------------------------- turn flow
// Enemy's turn -> player's turn from the first medal; player's turn -> next medal, and when every medal was
// used, the enemy's turn (one more player turn counted). Returns the new state.
int32_t MedalDeck::advanceState(MedalDeck *self) {
    if (self->state == STATE_ENEMY) {
        self->currentIndex = 0;
        if (self->onTurnStart.impl) rw::call(FunctionVoid_call, {addr(&self->onTurnStart)});
        self->state = STATE_PLAYER;
    } else if (self->state == STATE_PLAYER) {
        self->currentIndex = int32_t(uint32_t(self->currentIndex.get()) + 1);
        int32_t index = self->currentIndex;
        if (self->onAdvance.impl) {
            rw::call(FunctionVoid_call, {addr(&self->onAdvance)});
            index = self->currentIndex;
        }
        if (!(int32_t(uint32_t(self->friendCount.get()) + uint32_t(self->deckCount.get())) > index)) {
            self->state = STATE_ENEMY;
            self->turnCount = int32_t(uint32_t(self->turnCount.get()) + 1);
            if (self->onDeckEnd.impl) rw::call(FunctionVoid_call, {addr(&self->onDeckEnd)});
        }
    }
    return self->state;
}
RW_FUNCTION(0x5DFEC0, "MedalDeck::advanceState", MedalDeck::advanceState);

MedalInfo *MedalDeck::getCurrentMedalInfo(MedalDeck *self) {
    return rw::host_ptr<MedalInfo>(addr(self) + 40u * uint32_t(currentSlot(self)) + 0x270u);
}
RW_FUNCTION(0x5DFF6A, "MedalDeck::getCurrentMedalInfo", MedalDeck::getCurrentMedalInfo);

int32_t MedalDeck::getCurrentAttack(MedalDeck *self) {
    guest::Frame f(0x20);                 // push 4 + sub 0x10
    uint32_t property = addr(self) + 40u * uint32_t(currentSlot(self)) + 0x278u;
    sbRead(property, f.addr(8), MedalInfo::ATTACK, 4);
    return *f.at<int32_t>(8);
}
RW_FUNCTION(0x5DFF8C, "MedalDeck::getCurrentAttack", MedalDeck::getCurrentAttack);

// Average defense of the deck medals (the friend medal not included); an empty deck divides by zero.
int32_t MedalDeck::getCurrentDefense(MedalDeck *self) {
    guest::Frame f(0x28);                 // push 5 + sub 0x14
    int32_t count = self->deckCount;
    uint32_t sum = 0;
    if (count >= 1) {
        for (int32_t k = 0; k < count; k++) {
            sbRead(addr(&self->medals[k].property), f.addr(0xC), MedalInfo::DEFENSE, 4);
            sum += *f.at<uint32_t>(0xC);
        }
    }
    return int32_t(rw::call(AEABI_IDIV, {sum, uint32_t(count)}));
}
RW_FUNCTION(0x5DFFFC, "MedalDeck::getCurrentDefense", MedalDeck::getCurrentDefense);

// Call fn on every medal (deck + friend) while it returns true; returns the last result (self when empty).
uint32_t MedalDeck::forEachMedal(MedalDeck *self, uint32_t fn) {
    int32_t total = int32_t(uint32_t(self->friendCount.get()) + uint32_t(self->deckCount.get()));
    uint32_t r = addr(self);
    if (total < 1) return r;
    for (int32_t k = 1;; k++) {
        r = rw::call(FunctionMedalInfo_call, {fn, addr(&self->medals[k - 1])});
        if (r != 1 || !(k < total)) break;
    }
    return r;
}
RW_FUNCTION(0x5E007C, "MedalDeck::forEachMedal", MedalDeck::forEachMedal);

uint32_t MedalDeck::isNextEnemyTurn(MedalDeck *self) {
    return uint32_t(self->friendCount.get()) + uint32_t(self->deckCount.get()) ==
           uint32_t(self->currentIndex.get()) + 1;
}
RW_FUNCTION(0x5E03F8, "MedalDeck::isNextEnemyTurn", MedalDeck::isNextEnemyTurn);

uint32_t MedalDeck::isCurrentFirstMedal(MedalDeck *self) {
    if (self->state != STATE_PLAYER) return 0;
    return self->currentIndex == 0;
}
RW_FUNCTION(0x5E041C, "MedalDeck::isCurrentFirstMedal", MedalDeck::isCurrentFirstMedal);

// The friend medal is in use (unsigned compare, as compiled).
uint32_t MedalDeck::isCurrentFriendMedal(MedalDeck *self) {
    if (self->friendCount == 0) return 0;
    return uint32_t(self->deckCount.get()) <= uint32_t(self->currentIndex.get());
}
RW_FUNCTION(0x5E0440, "MedalDeck::isCurrentFriendMedal", MedalDeck::isCurrentFriendMedal);

uint32_t MedalDeck::isCurrentBurstSingleSelect(MedalDeck *self) {
    guest::Frame f(0x20);                 // push 4 + sub 0x10
    uint32_t property = addr(self) + 40u * uint32_t(currentSlot(self)) + 0x278u;
    sbRead(property, f.addr(0xB), MedalInfo::BURST_SINGLE_SELECT, 1);
    return *f.at<uint8_t>(0xB);
}
RW_FUNCTION(0x5E046C, "MedalDeck::isCurrentBurstSingleSelect", MedalDeck::isCurrentBurstSingleSelect);

uint32_t MedalDeck::isValidCurrentBurst(MedalDeck *self) {
    guest::Frame f(0x20);                 // push 4 + sub 0x10
    uint32_t medal = self->medals[currentSlot(self)].medal.obj;
    sbRead(medal, f.addr(8), MEDAL_HAS_BURST, 4);
    return *f.at<int32_t>(8) == 1 ? 1u : 0u;
}
RW_FUNCTION(0x5E04E4, "MedalDeck::isValidCurrentBurst", MedalDeck::isValidCurrentBurst);

// Gauge cost of the current medal's burst.
int32_t MedalDeck::getCurrentBurstGauge(MedalDeck *self) {
    guest::Frame f(0x30);                 // push 5 + sub 0x1C
    uint32_t medal = self->medals[currentSlot(self)].medal.obj;
    sbRead(medal, f.addr(0x14), MEDAL_BURST_ID, 4);
    rw::call(master_Burst_create, {f.addr(0xC), *f.at<uint32_t>(0x14)});
    uint32_t burst = *f.at<uint32_t>(0xC);
    sbRead(burst, f.addr(0x14), BURST_GAUGE, 4);
    int32_t gauge = *f.at<int32_t>(0x14);
    if (uint32_t c = *f.at<uint32_t>(0x10)) guest::release(c);
    return gauge;
}
RW_FUNCTION(0x5E0564, "MedalDeck::getCurrentBurstGauge", MedalDeck::getCurrentBurstGauge);

// ------------------------------------------------------------------------------------------- bursts
// A burst targets a single enemy when its single-select field is 1 or any of its 18 effect values (0x200 ..
// 0x2C0, every 12 bytes) is set. `self` is unused. Unknown burst ids give false.
uint32_t MedalDeck::isBurstSingleSelect(uint32_t self, int32_t burstId) {
    (void)self;
    guest::Frame f(0xB8);                 // push 5 + sub 0xA4
    const uint32_t sp = f.addr(0x4C), value = f.addr(0x54);
    rw::call(master_Burst_create, {sp, uint32_t(burstId)});
    uint32_t result = 0;
    uint32_t burst = *f.at<uint32_t>(0x4C);
    if (burst) {
        static const uint32_t EFFECTS[17] = {0x200, 0x20C, 0x218, 0x224, 0x230, 0x23C, 0x248, 0x254, 0x260,
                                             0x26C, 0x278, 0x284, 0x290, 0x29C, 0x2A8, 0x2B4, 0x2BC};
        uint32_t any = 0;
        for (uint32_t off : EFFECTS) {
            sbRead(*f.at<uint32_t>(0x4C), value, off, 4);
            any |= *f.at<uint32_t>(0x54);
        }
        sbRead(*f.at<uint32_t>(0x4C), value, 0x2C0, 4);
        any |= *f.at<uint32_t>(0x54);
        sbRead(*f.at<uint32_t>(0x4C), value, BURST_SINGLE_SELECT, 4);
        result = *f.at<int32_t>(0x54) == 1 ? 1u : 0u;
        result |= any != 0 ? 1u : 0u;
    }
    if (uint32_t c = *f.at<uint32_t>(0x50)) guest::release(c);
    return result;
}
RW_FUNCTION(0x5E0104, "MedalDeck::isBurstSingleSelect", MedalDeck::isBurstSingleSelect);

namespace {
// One block of a burst row as getBitFieldBurstAddStatus summarises it: 8 signed values (12 bytes apart) give
// two bits each (bit 2i: > 0, bit 2i+1: < 0); then flag values (bit set when non-zero).
struct BurstFlag { uint32_t offset, bit; };
struct BurstBlock {
    uint32_t signedBase;            // first of the 8 signed values (each one is read twice, as compiled)
    const BurstFlag *flags;
    uint32_t flagCount;
};
constexpr BurstFlag FLAGS_1[] = {{0x260, 16}, {0x26C, 17}, {0x278, 18}, {0x284, 19}, {0x290, 20},
                                 {0x29C, 21}, {0x2A8, 22}, {0x2B4, 23}, {0x2BC, 24}, {0x2C0, 25}};
constexpr BurstFlag FLAGS_2[] = {{0x264, 16}, {0x270, 17}, {0x27C, 18}, {0x288, 19},
                                 {0x294, 20}, {0x2A0, 21}, {0x2AC, 22}, {0x2B8, 23}};
constexpr BurstFlag FLAGS_3[] = {{0x25C, 16}, {0x268, 17}, {0x274, 18}, {0x280, 19}, {0x28C, 20},
                                 {0x298, 21}, {0x2A4, 22}, {0x2B0, 23}, {0x2C4, 26}, {0x2C8, 27},
                                 {0x2CC, 28}, {0x2D0, 29}, {0x2D8, 30}};
constexpr BurstBlock BLOCK_1 = {0x200, FLAGS_1, 10};
constexpr BurstBlock BLOCK_2 = {0x204, FLAGS_2, 8};
constexpr BurstBlock BLOCK_3 = {0x1FC, FLAGS_3, 13};
}  // namespace

// The burst's effects of one kind as a bit field. `self` is unused; burst id 0 gives 0 without a lookup, an
// unknown buffer type gives 0.
uint32_t MedalDeck::getBitFieldBurstAddStatus(uint32_t self, uint32_t type, int32_t burstId) {
    (void)self;
    guest::Frame f(0x1D0);                // push 5 + sub 0x1BC
    if (burstId == 0) return 0;
    rw::call(master_Burst_create, {f.addr(0x74), uint32_t(burstId)});
    const BurstBlock *block = type == BURST_BUFFER_3   ? &BLOCK_3
                              : type == BURST_BUFFER_2 ? &BLOCK_2
                              : type == BURST_BUFFER_1 ? &BLOCK_1
                                                       : nullptr;
    uint32_t bits = 0;
    if (block) {
        const uint32_t value = f.addr(0x80);
        auto read = [&](uint32_t off) {
            sbRead(*f.at<uint32_t>(0x74), value, off, 4);
            return *f.at<int32_t>(0x80);
        };
        for (uint32_t i = 0; i < 8; i++) {
            uint32_t off = block->signedBase + 12u * i;
            int32_t positive = read(off);
            int32_t negative = read(off);
            if (positive > 0) bits |= 1u << (2 * i);
            if (negative < 0) bits |= 1u << (2 * i + 1);
        }
        for (uint32_t k = 0; k < block->flagCount; k++)
            if (read(block->flags[k].offset) != 0) bits |= 1u << block->flags[k].bit;
    }
    if (uint32_t c = *f.at<uint32_t>(0x78)) guest::release(c);
    return bits;
}
RW_FUNCTION(0x5E0620, "MedalDeck::getBitFieldBurstAddStatus", MedalDeck::getBitFieldBurstAddStatus);
