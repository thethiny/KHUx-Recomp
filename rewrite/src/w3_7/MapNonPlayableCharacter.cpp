#include "MapNonPlayableCharacter.hpp"

using Npc_ = MapNonPlayableCharacter;
using actor::ld;
using actor::vcallAt;
using rw::guest_addr;

namespace {
namespace fn {
constexpr uint32_t MAPNODE_CTOR = 0x5DA2D5;              // MapNode::MapNode()
constexpr uint32_t MAPNODE_DTOR = 0x5DA38D;              // MapNode::~MapNode()
constexpr uint32_t MAPNODE_INIT = 0x5DA411;              // MapNode::init()
constexpr uint32_t MAPNODE_LOCK_Z = 0x5DA575;            // MapNode::lockMapPosZ(float)
constexpr uint32_t SAFEBUFFER_CTOR = 0x459F21;           // SafeBuffer::SafeBuffer(size, registerForUpdate)
constexpr uint32_t SAFEBUFFER_DTOR = 0x45A13D;           // SafeBuffer::~SafeBuffer()
constexpr uint32_t SAFEBUFFER_WRITE = 0x459FA1;          // SafeBuffer::write(dst, src, n, index)
constexpr uint32_t NPC_CREATE = 0x5CC965;                // Npc::create(int)
constexpr uint32_t NPC_PRELOAD = 0x5CCDED;               // Npc::preloadResources() const
constexpr uint32_t SET_NPC_DIRECTION = 0x5DC179;         // MapNonPlayableCharacter::setNPCDirection(bool)
constexpr uint32_t GET_ICON_OFFSET = 0x5DC3A9;           // MapNonPlayableCharacter::get_icon_offset(int) (sret Vec2)
constexpr uint32_t NODE_SCHEDULE_UPDATE = 0x894BA7;      // cocos2d::Node::scheduleUpdate()
constexpr uint32_t SPRITE_CREATE = 0x8A64DD;             // cocos2d::Sprite::create()
constexpr uint32_t DIRECTOR_GET_INSTANCE = 0x8EE519;
constexpr uint32_t DIRECTOR_GET_TEXTURE_CACHE = 0x8EFE69;  // Director::getTextureCache() const
constexpr uint32_t TEXTURE_CACHE_ADD_IMAGE = 0x917DA9;   // TextureCache::addImage(const std::string&)
constexpr uint32_t TEXTURE_GET_CONTENT_SIZE = 0x914ACB;  // Texture2D::getContentSize() const (sret Size)
constexpr uint32_t RECT_CTOR = 0x8DF18B;                 // cocos2d::Rect::Rect(float x, float y, float w, float h)
constexpr uint32_t VEC2_CTOR = 0x8E4B9D;                 // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t VEC2_DEFAULT = 0x8E4B95;              // cocos2d::Vec2::Vec2()
constexpr uint32_t VEC2_COPY = 0x8E4C03;                 // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;
constexpr uint32_t MOVE_BY_CREATE = 0x86911D;            // cocos2d::MoveBy::create(float, const Vec2&)
constexpr uint32_t EASE_SINE_OUT_CREATE = 0x860D09;      // cocos2d::EaseSineOut::create(ActionInterval*)
constexpr uint32_t EASE_SINE_IN_CREATE = 0x860C05;       // cocos2d::EaseSineIn::create(ActionInterval*)
constexpr uint32_t SEQUENCE_CREATE = 0x867999;           // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t REPEAT_FOREVER_CREATE = 0x86815D;     // cocos2d::RepeatForever::create(ActionInterval*)
constexpr uint32_t STRINGBUFFER_FSET = 0x491501;         // StringBuffer::fset(const char* fmt, ...)
constexpr uint32_t VFS_READ_DATA = 0x8DD46D;             // bg::vfs::readData(out shared_ptr<bg::Data>, const char*)
constexpr uint32_t UDIVSI3 = 0xCD5DF9;                   // __udivsi3
}  // namespace fn

constexpr uint32_t SPRITE_SET_TEXTURE_SLOT = 159;        // Sprite::setTexture(Texture2D*)
constexpr uint32_t SPRITE_SET_TEXTURE_RECT_SLOT = 161;   // Sprite::setTextureRect(const Rect&)
constexpr uint32_t NODE_SET_GLOBAL_Z_SLOT = 4;           // the slot after setLocalZOrder (called with 100)
constexpr uint32_t STR_TALK_ICON = 0xE0221D;             // "img/stage_npc/NPCTalkIcon.png"
constexpr uint32_t STR_NPC_RULE = 0xE0223B;              // "img/stage_npc/npc_rule.bin": {npcId, y, x} int triples
constexpr uint32_t ICON_DIRECTION_LEFT = 0xFFFF;
}  // namespace

// ------------------------------------------------------------------------------------------------ lifetime
static Npc_ *MapNPC_construct(Npc_ *self) {
    rw::call(fn::MAPNODE_CTOR, {guest_addr(self)});
    self->vtable = Npc_::VTABLE;
    rw::call(fn::SAFEBUFFER_CTOR, {guest_addr(&self->property), 4, 1});
    self->property.vtable = Npc_::Property::VTABLE;
    self->talkIconNode = 0;
    self->talkIcon = 0;
    self->npc = 0;
    self->useSecondTalk = 0;
    self->npcId = 0;
    self->fixedDirection = 0;
    return self;
}
RW_FUNCTION(0x5DC320, "MapNonPlayableCharacter::MapNonPlayableCharacter", MapNPC_construct);

static Npc_ *MapNPC_destruct(Npc_ *self) {
    const uint32_t me = guest_addr(self);
    self->vtable = Npc_::VTABLE;
    rw::call(actor_fn::node_removeAllChildrenWithCleanup, {me, 1});
    vcallAt(self->talkIconNode, node_slot::REMOVE_ALL_CHILDREN_CLEANUP, {self->talkIconNode, 1});
    if (self->talkIconNode) rw::call(actor_fn::ref_release, {self->talkIconNode});
    if (self->talkIcon) {
        rw::call(actor_fn::ref_release, {self->talkIcon});
        self->talkIcon = 0;
    }
    rw::call(fn::SAFEBUFFER_DTOR, {guest_addr(&self->property)});
    rw::call(fn::MAPNODE_DTOR, {me});
    return self;
}
RW_FUNCTION(0x5DBDB8, "MapNonPlayableCharacter::~MapNonPlayableCharacter", MapNPC_destruct);

static void MapNPC_destructDelete(Npc_ *self) { guest::deleteObject(guest_addr(MapNPC_destruct(self))); }
RW_FUNCTION(0x5DBE48, "MapNonPlayableCharacter::~MapNonPlayableCharacter (deleting)", MapNPC_destructDelete);

static void MapNPC_Property_destructDelete(SafeBuffer *self) {
    guest::deleteObject(rw::call(fn::SAFEBUFFER_DTOR, {guest_addr(self)}));
}
RW_FUNCTION(0x5DC550, "MapNonPlayableCharacter::Property::~Property (deleting)", MapNPC_Property_destructDelete);

static uint32_t MapNPC_create(const Npc_::Info *info) {
    uint32_t p = guest::newObject(Npc_::SIZE);
    MapNPC_construct(rw::host_ptr<Npc_>(p));
    if (vcallAt(p, Npc_::VSLOT_INIT, {p, guest_addr(info)}) == 1) {
        rw::call(actor_fn::ref_autorelease, {p});
        return p;
    }
    vcallAt(p, node_slot::DELETING_DTOR, {p});
    return 0;
}
RW_FUNCTION(0x5DC2C4, "MapNonPlayableCharacter::create(const Info*)", MapNPC_create);

// ------------------------------------------------------------------------------------------------ init
static uint32_t MapNPC_init(Npc_ *self, const Npc_::Info *info) {
    guest::Frame f(0x64);
    const uint32_t me = guest_addr(self);
    if (rw::call(fn::MAPNODE_INIT, {me}) != 1) return 0;
    vcallAt(me, node_slot::SET_TAG, {me, Npc_::NODE_TAG});

    // the flash actor
    int32_t npcId = info->npcId;
    self->npcId = npcId;
    self->npc = rw::call(fn::NPC_CREATE, {uint32_t(npcId)});
    rw::call(fn::NPC_PRELOAD, {self->npc});
    vcallAt(me, node_slot::ADD_CHILD, {me, self->npc});
    vcallAt(me, node_slot::SET_VISIBLE, {me, 1});

    uint32_t x = rw::get<uint32_t>(&info->position.x, 0), y = rw::get<uint32_t>(&info->position.y, 0);
    rw::set<uint32_t>(&self->mapPosition.y, 0, y);
    rw::set<uint32_t>(&self->mapPosition.x, 0, x);
    rw::call(fn::MAPNODE_LOCK_Z, {me, rw::fbits(float(info->position.y) + float(int32_t(info->zOffset)))});
    self->useSecondTalk = (info->flags >> 1) & 1;
    self->talkType = info->talkType;
    self->talkId = info->talkId;
    self->talkId2 = info->talkId2;
    rw::call(fn::SAFEBUFFER_WRITE, {guest_addr(&self->property), self->property.data.addr, guest_addr(&info->property), 4, 0});

    // the talk icon holder: follows the NPC (update), hidden until show_talkicon
    self->talkIconNode = rw::call(actor_fn::node_create, {});
    rw::call(actor_fn::ref_retain, {self->talkIconNode});
    vcallAt(self->talkIconNode, NODE_SET_GLOBAL_Z_SLOT, {self->talkIconNode, 100});
    vcallAt(self->talkIconNode, node_slot::SET_VISIBLE, {self->talkIconNode, 0});

    if ((self->talkType | 2) == 3) {
        // a bobbing "talk" bubble: up 2 px and back in 0.25 s steps, forever
        self->talkIcon = rw::call(fn::SPRITE_CREATE, {});
        uint32_t director = rw::call(fn::DIRECTOR_GET_INSTANCE, {});
        uint32_t cache = rw::call(fn::DIRECTOR_GET_TEXTURE_CACHE, {director});
        const uint32_t name = f.addr(0x54), size = f.addr(0x4C), rect = f.addr(0x3C);
        actor::makeString(name, STR_TALK_ICON, 29);
        uint32_t texture = rw::call(fn::TEXTURE_CACHE_ADD_IMAGE, {cache, name});
        actor::freeString(name);
        rw::call(fn::TEXTURE_GET_CONTENT_SIZE, {size, texture});
        vcallAt(self->talkIcon, SPRITE_SET_TEXTURE_SLOT, {self->talkIcon, texture});
        uint32_t icon = self->talkIcon;
        uint32_t setRect = ld(ld(icon) + SPRITE_SET_TEXTURE_RECT_SLOT * 4);
        rw::call(fn::RECT_CTOR, {rect, 0, 0, ld(size), ld(size + 4)});
        rw::call(setRect, {icon, rect});
        rw::call(fn::VEC2_DTOR, {rect});

        const uint32_t up = f.addr(0x34), down = f.addr(0x2C);
        rw::call(fn::VEC2_CTOR, {up, 0, rw::fbits(2.0f)});
        uint32_t moveUp = rw::call(fn::MOVE_BY_CREATE, {rw::fbits(0.25f), up});
        rw::call(fn::VEC2_DTOR, {up});
        rw::call(fn::VEC2_CTOR, {down, 0, rw::fbits(-2.0f)});
        uint32_t moveDown = rw::call(fn::MOVE_BY_CREATE, {rw::fbits(0.25f), down});
        rw::call(fn::VEC2_DTOR, {down});
        uint32_t a = rw::call(fn::EASE_SINE_OUT_CREATE, {moveUp});
        uint32_t b = rw::call(fn::EASE_SINE_IN_CREATE, {moveDown});
        uint32_t c = rw::call(fn::EASE_SINE_OUT_CREATE, {moveDown});
        uint32_t d = rw::call(fn::EASE_SINE_IN_CREATE, {moveUp});
        uint32_t sequence = rw::call(fn::SEQUENCE_CREATE, {a, b, c, d, 0});
        uint32_t target = self->talkIcon;
        rw::call(actor_fn::node_runAction, {target, rw::call(fn::REPEAT_FOREVER_CREATE, {sequence})});
        rw::call(actor_fn::ref_retain, {self->talkIcon});
        vcallAt(self->talkIconNode, node_slot::ADD_CHILD_Z, {self->talkIconNode, self->talkIcon, 10});
    }
    rw::call(fn::SET_NPC_DIRECTION, {me, info->direction == ICON_DIRECTION_LEFT ? 1u : 0u});
    self->fixedDirection = uint8_t(rw::get<uint8_t>(&info->flags, 0) & 1);
    rw::call(fn::NODE_SCHEDULE_UPDATE, {me});
    return 1;
}
RW_FUNCTION(0x5DBE54, "MapNonPlayableCharacter::init(const Info*)", MapNPC_init);

// ------------------------------------------------------------------------------------------------ behaviour
// Face left (mirror the actor) or right; the talk icon moves to the actor's "icon" point plus the per-NPC offset.
static void MapNPC_setNPCDirection(Npc_ *self, uint32_t left) {
    guest::Frame f(0x1C);
    vcallAt(self->npc, node_slot::SET_SCALE_X, {self->npc, rw::fbits(left == 1 ? -1.0f : 1.0f)});
    if (!self->talkIcon) return;
    const uint32_t pos = f.addr(0x10), offset = f.addr(0x8);
    rw::call(fn::VEC2_COPY, {pos, self->npc + 1328});   // Npc::iconPoint
    rw::call(fn::GET_ICON_OFFSET, {offset, guest_addr(self), uint32_t(int32_t(self->npcId))});
    float px = rw::load<float>(pos), ox = rw::load<float>(offset);
    *rw::frame_ptr<float>(pos) = left == 1 ? px - ox : ox + px;
    *rw::frame_ptr<float>(pos + 4) = rw::load<float>(offset + 4) + rw::load<float>(pos + 4);
    vcallAt(self->talkIcon, node_slot::SET_POSITION, {self->talkIcon, pos});
    rw::call(fn::VEC2_DTOR, {offset});
    rw::call(fn::VEC2_DTOR, {pos});
}
RW_FUNCTION(0x5DC178, "MapNonPlayableCharacter::setNPCDirection(bool)", MapNPC_setNPCDirection);

// The talk icon's offset for an NPC: npc_rule.bin rows {npcId, y, x}; (0, 200) when the NPC has no row.
static void MapNPC_getIconOffset(const bt::Vec2 *out, uint32_t /*self*/, int32_t npcId) {
    guest::Frame f(0x42C);
    const uint32_t data = f.addr(0x1C), path = f.addr(0x24);   // shared_ptr<bg::Data>, StringBuffer (1024 + length)
    rw::call(fn::VEC2_DEFAULT, {guest_addr(out)});
    *rw::frame_ptr<uint32_t>(path + 0x400) = 0;
    *rw::frame_ptr<uint8_t>(path) = 0;
    rw::call(fn::STRINGBUFFER_FSET, {path, STR_NPC_RULE});
    rw::call(fn::VFS_READ_DATA, {data, path});
    bt::Vec2 *o = const_cast<bt::Vec2 *>(out);
    o->x = 0.0f;
    rw::set<uint32_t>(&o->y, 0, 0x43480000u);   // 200.0f
    uint32_t blob = ld(data);
    if (blob) {
        uint32_t bytes = ld(blob + 8);
        uint32_t rows = rw::call(fn::UDIVSI3, {bytes, 12});
        if (bytes >= 12) {
            uint32_t row = ld(blob + 4);
            for (uint32_t k = 0; k < rows; k++, row += 12) {
                if (rw::load<int32_t>(row) == npcId) {
                    o->x = float(rw::load<int32_t>(row + 8));
                    o->y = float(rw::load<int32_t>(row + 4));
                }
            }
        }
    }
    uint32_t ctrl = ld(data + 4);
    if (ctrl) guest::release(ctrl);
}
RW_FUNCTION(0x5DC3A8, "MapNonPlayableCharacter::get_icon_offset(int)", MapNPC_getIconOffset);

// The talk icon holder follows the node.
static void MapNPC_update(Npc_ *self, float /*dt*/) {
    uint32_t node = self->talkIconNode;
    if (!node) return;
    uint32_t setPosition = ld(ld(node) + node_slot::SET_POSITION * 4);
    uint32_t pos = vcallAt(guest_addr(self), node_slot::GET_POSITION, {guest_addr(self)});
    rw::call(setPosition, {node, pos});
}
RW_FUNCTION(0x5DC264, "MapNonPlayableCharacter::update(float)", MapNPC_update);

static void MapNPC_showTalkIcon(Npc_ *self, uint32_t show) {
    vcallAt(self->talkIconNode, node_slot::SET_VISIBLE, {self->talkIconNode, show});
}
RW_FUNCTION(0x5DC286, "MapNonPlayableCharacter::show_talkicon(bool)", MapNPC_showTalkIcon);

// The talk to start: type 3 its talk, type 1 the second talk once flagged, others none (0).
static int32_t MapNPC_getTalk(const Npc_ *self) {
    int32_t type = self->talkType;
    if (type == 3) return self->talkId;
    if (type == 1) return self->useSecondTalk ? int32_t(self->talkId2) : int32_t(self->talkId);
    return 0;
}
RW_FUNCTION(0x5DC298, "MapNonPlayableCharacter::get_talk", MapNPC_getTalk);

static void MapNPC_talk() {}
RW_FUNCTION(0x5DC3A4, "MapNonPlayableCharacter::talk(StageManager*) (empty)", MapNPC_talk);

// Turn toward a map point unless the direction is fixed (the mirrored pose when the point lies right of the NPC).
static void MapNPC_lookAt(Npc_ *self, const bt::Vec2 *point) {
    if (self->fixedDirection) return;
    float x = self->mapPosition.x;
    rw::call(fn::SET_NPC_DIRECTION, {guest_addr(self), x < float(point->x) ? 1u : 0u});
}
RW_FUNCTION(0x5DC490, "MapNonPlayableCharacter::lookAt(cocos2d::Vec2&)", MapNPC_lookAt);

// Tap centre on the map: the map position, raised by half the talk icon's y position when there is an icon.
static void MapNPC_getTapCenterMapPosition(const bt::Vec2 *out, const Npc_ *self) {
    rw::call(fn::VEC2_COPY, {guest_addr(out), guest_addr(&self->mapPosition)});
    uint32_t icon = self->talkIcon;
    if (!icon) return;
    uint32_t p = vcallAt(icon, node_slot::GET_POSITION, {icon});
    bt::Vec2 *o = const_cast<bt::Vec2 *>(out);
    o->x = rw::load<float>(p) + float(o->x);
    o->y = float(o->y) + rw::load<float>(p + 4) * -0.5f;
}
RW_FUNCTION(0x5DC4C0, "MapNonPlayableCharacter::getTapCenterMapPosition", MapNPC_getTapCenterMapPosition);

static float MapNPC_getTapRange(const Npc_ *self) {
    uint32_t icon = self->talkIcon;
    if (!icon) return 0.0f;
    uint32_t p = vcallAt(icon, node_slot::GET_POSITION, {icon});
    return rw::load<float>(p + 4) * 0.5f + 0.0f;
}
RW_FUNCTION(0x5DC522, "MapNonPlayableCharacter::getTapRange", MapNPC_getTapRange);
