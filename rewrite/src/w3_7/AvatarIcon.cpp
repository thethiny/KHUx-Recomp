#include "AvatarIcon.hpp"
#include "actor/FunctionThunks.hpp"

using actor::ld;
using actor::vcallAt;
using rw::guest_addr;

namespace {
namespace fn {
constexpr uint32_t AVATAR_CREATE = 0x5C5883;             // Avatar::create(const Style&, bool)
constexpr uint32_t AVATAR_CHANGE_MOTION = 0x5C63C5;      // Avatar::changeMotion(Motion)
constexpr uint32_t AVATAR_CHANGE_EXPRESSION = 0x5C64B5;  // Avatar::changeExpression(Expression)
constexpr uint32_t AVATAR_ICON_INIT = 0x5C9E8D;          // AvatarIcon::init (below)
constexpr uint32_t AVATAR_ICON_CREATE_NODE = 0x5CA235;   // AvatarIcon::createNode (below)
constexpr uint32_t NODE_INIT = 0x892B69;                 // cocos2d::Node::init()
constexpr uint32_t NODE_VISIT = 0x8944C9;                // cocos2d::Node::visit()
constexpr uint32_t NODE_VECTOR_PUSH_BACK = 0x5CA3F1;     // cocos2d::Vector<Node*>::pushBack(Node*)
constexpr uint32_t NODE_VECTOR_DTOR = 0x496317;          // cocos2d::Vector<Node*>::~Vector()
constexpr uint32_t INT_MAP_INSERT_UNIQUE = 0x4B18D1;     // unordered_map<int,int>::__insert_unique(pair<int,int>&&) (sret)
constexpr uint32_t INT_MAP_FIND = 0x494251;              // unordered_map<int,int>::find(const int&)
constexpr uint32_t INT_MAP_ASSIGN_MULTI = 0x5CB829;      // unordered_map<int,int>::__assign_multi(first, last)
constexpr uint32_t INT_MAP_DTOR = 0x4942D3;              // ~__hash_table<int,int>
constexpr uint32_t DIRECTOR_GET_INSTANCE = 0x8EE519;
constexpr uint32_t GLVIEW_GET_SCALE_X = 0x8D4555;        // GLViewProtocol::getScaleX() const
constexpr uint32_t GLVIEW_GET_SCALE_Y = 0x8D4559;
constexpr uint32_t TEXTURE_GET_CONTENT_SIZE_IN_PIXELS = 0x914B07;  // Texture2D::getContentSizeInPixels()
constexpr uint32_t TEXTURE_SET_ANTI_ALIAS = 0x915E49;    // Texture2D::setAntiAliasTexParameters()
constexpr uint32_t SIZE_COPY = 0x8DF02D;                 // cocos2d::Size::Size(const Size&)
constexpr uint32_t VEC2_CTOR = 0x8E4B9D;                 // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;
constexpr uint32_t SPRITE_CREATE_WITH_TEXTURE = 0x8A6295;  // cocos2d::Sprite::createWithTexture(Texture2D*)
constexpr uint32_t SPRITE_SET_FLIPPED_Y = 0x8A8201;      // cocos2d::Sprite::setFlippedY(bool)
constexpr uint32_t RENDER_TEXTURE_CREATE_FORMAT = 0x8A3FFD;  // RenderTexture::create(w, h, PixelFormat, depthStencil)
constexpr uint32_t RENDER_TEXTURE_CREATE = 0x8A452D;     // RenderTexture::create(w, h)
constexpr uint32_t FADE_TO_CREATE = 0x86B3E1;            // cocos2d::FadeTo::create(float, GLubyte)
constexpr uint32_t CALL_FUNC_CREATE = 0x866B39;          // cocos2d::CallFunc::create(const std::function<void()>&)
constexpr uint32_t SEQUENCE_CREATE = 0x867999;
}  // namespace fn

// cocos2d slots used here (beyond node_slot)
constexpr uint32_t SLOT_SET_POSITION_XY = 23;            // Node::setPosition(float, float)
constexpr uint32_t SLOT_SET_SCALE_XY = 18;               // Node::setScale(float, float)
constexpr uint32_t SLOT_SET_CASCADE_OPACITY = 136;       // Node::setCascadeOpacityEnabled(bool)
constexpr uint32_t SLOT_SET_CASCADE_COLOR = 142;         // Node::setCascadeColorEnabled(bool)
constexpr uint32_t SLOT_RT_BEGIN_WITH_CLEAR = 157;       // RenderTexture::beginWithClear(r, g, b, a)
constexpr uint32_t SLOT_RT_END = 160;                    // RenderTexture::end()
constexpr uint32_t SLOT_SPRITE_GET_TEXTURE = 160;        // Sprite::getTexture() const
constexpr uint32_t SLOT_SPRITE_SET_BLEND_FUNC = 174;     // Sprite::setBlendFunc(const BlendFunc&)
constexpr uint32_t RT_SPRITE = 740;                      // RenderTexture::_sprite
constexpr uint32_t AVATAR_EXPRESSION = 1360;             // Avatar: current expression
constexpr uint32_t AVATAR_MOTION = 0x524;                // Avatar: current motion
constexpr uint32_t GL_DEPTH24_STENCIL8 = 0x88F0;
constexpr uint32_t GL_ONE_MINUS_SRC_ALPHA = 0x303;
constexpr uint32_t PIXEL_FORMAT_RGBA8888 = 2;
constexpr int32_t AVATAR_MOTION_ICON = 27;               // the motion the icon is drawn in

constexpr float MAX_ICON_SCALE = 3.9f;                   // the icon texture is never rendered more than 3.9x the node

// std::function __func vtables (+8) and lambda type names / typeinfos
constexpr uint32_t ANON_FUNC_VTABLE = 0xF5D040;          // (anonymous namespace)::$_0
constexpr uint32_t ANON_TYPE_NAME = 0xE4A020;            // "N12_GLOBAL__N_13$_0E"
constexpr uint32_t ANON_TYPE_INFO = 0xF8F4C8;
constexpr uint32_t SET_LIMIT_FUNC_VTABLE = 0xF5D06C;     // AvatarIconCache::setLimit(unsigned)::$_1
constexpr uint32_t SET_LIMIT_TYPE_NAME = 0xE4A7B0;       // "ZN15AvatarIconCache8setLimitEjE3$_1"
constexpr uint32_t SET_LIMIT_TYPE_INFO = 0xF8F600;
constexpr uint32_t FADE_DONE_FUNC_VTABLE = 0xF5D098;     // AvatarIcon::changeExpression(Expression, float)::$_0
constexpr uint32_t FADE_DONE_TYPE_NAME = 0xE4A860;       // "ZN10AvatarIcon16changeExpressionEN6Avatar10ExpressionEfE3$_0"
constexpr uint32_t FADE_DONE_TYPE_INFO = 0xF8F614;

float asFloat(uint32_t u) { return rw::bitsf(u); }
uint32_t bitsOf(float f) { return rw::fbits(f); }

// A new AvatarIcon as the constructor (inlined into create / clone) leaves it: zeroed node list and expression
// map (max load factor 1.0). `zeroAll`: create() also memsets the whole object before the Node constructor.
uint32_t newIcon(bool zeroAll) {
    uint32_t p = guest::newObject(AvatarIcon::SIZE);
    if (zeroAll) rw::call(actor_fn::aeabi_memset, {p, AvatarIcon::SIZE, 0});
    rw::call(actor_fn::node_ctor, {p});
    AvatarIcon *icon = rw::host_ptr<AvatarIcon>(p);
    icon->vtable = AvatarIcon::VTABLE;
    rw::call(actor_fn::aeabi_memset, {guest_addr(&icon->nodes), 0x1C, 0});
    icon->mapMaxLoadFactor = 1.0f;
    return p;
}
uint32_t finishCreate(uint32_t p, uint32_t ok) {
    if (ok == 1) {
        rw::call(actor_fn::ref_autorelease, {p});
        return p;
    }
    rw::call(actor_fn::ref_release, {p});
    return 0;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ create
// create(style, texture, offset, scaleX, scaleY, expressions): a new Avatar in the icon motion, drawn into an icon.
static uint32_t AvatarIcon_createFromStyle(uint32_t style, uint32_t texture, uint32_t offset, uint32_t scaleX,
                                           uint32_t scaleY, uint32_t expressions) {
    guest::Frame f(0x1C);
    uint32_t avatar = rw::call(fn::AVATAR_CREATE, {style, 0});
    if (!avatar) return 0;
    rw::call(fn::AVATAR_CHANGE_MOTION, {avatar, uint32_t(AVATAR_MOTION_ICON)});
    uint32_t p = newIcon(true);
    return finishCreate(p, rw::call(fn::AVATAR_ICON_INIT, {p, avatar, texture, offset, scaleX, scaleY, expressions}));
}
RW_FUNCTION(0x5C9DB4, "AvatarIcon::create(const Avatar::Style&, Texture2D*, const Vec2&, float, float, const vector<Expression>&)",
            AvatarIcon_createFromStyle);

static uint32_t AvatarIcon_create(uint32_t avatar, uint32_t texture, uint32_t offset, uint32_t scaleX, uint32_t scaleY,
                                  uint32_t expressions) {
    guest::Frame f(0x1C);
    uint32_t p = newIcon(true);
    return finishCreate(p, rw::call(fn::AVATAR_ICON_INIT, {p, avatar, texture, offset, scaleX, scaleY, expressions}));
}
RW_FUNCTION(0x5CA184, "AvatarIcon::create(Avatar*, Texture2D*, const Vec2&, float, float, const vector<Expression>&)",
            AvatarIcon_create);

// ------------------------------------------------------------------------------------------------ init
// One RenderTexture per expression (the first one visible). The texture is drawn at the screen's scale (at least
// 1, at most 3.9 / the node scale) and the node scaled back down, so the icon stays sharp.
static uint32_t AvatarIcon_init(AvatarIcon *self, uint32_t avatar, uint32_t texture, const bt::Vec2 *offset,
                                float scaleX, float scaleY, const bt::Vector *expressions) {
    guest::Frame f(0x74);
    const uint32_t me = guest_addr(self);
    const uint32_t size = f.addr(0x60), offsetPos = f.addr(0x58), inserted = f.addr(0x68), pair = f.addr(0x50);
    if (rw::call(fn::NODE_INIT, {me}) != 1) return 0;
    vcallAt(me, SLOT_SET_CASCADE_COLOR, {me, 1});
    vcallAt(me, SLOT_SET_CASCADE_OPACITY, {me, 1});
    self->current = 0;

    uint32_t glview = ld(rw::call(fn::DIRECTOR_GET_INSTANCE, {}) + 0x9C);
    float screenX = rw::callf(fn::GLVIEW_GET_SCALE_X, {glview});
    float screenY = rw::callf(fn::GLVIEW_GET_SCALE_Y, {glview});
    rw::call(fn::SIZE_COPY, {size, rw::call(fn::TEXTURE_GET_CONTENT_SIZE_IN_PIXELS, {texture})});

    float limitX = MAX_ICON_SCALE / scaleX;
    float fx = screenX > 1.0f ? screenX : 1.0f;
    float absX = asFloat(bitsOf(limitX) & 0x7FFFFFFF);
    fx = fx < absX ? fx : absX;
    *rw::frame_ptr<float>(size) = fx * rw::load<float>(size);
    float limitY = MAX_ICON_SCALE / scaleY;
    float fy = screenY > 1.0f ? screenY : 1.0f;
    float absY = asFloat(bitsOf(limitY) & 0x7FFFFFFF);
    fy = fy < absY ? fy : absY;
    *rw::frame_ptr<float>(size + 4) = rw::load<float>(size + 4) * fy;

    uint32_t sprite = rw::call(fn::SPRITE_CREATE_WITH_TEXTURE, {texture});
    float halfW = rw::load<float>(size) * 0.5f, halfH = rw::load<float>(size + 4) * 0.5f;
    vcallAt(sprite, SLOT_SET_POSITION_XY, {sprite, bitsOf(halfW), bitsOf(halfH)});
    vcallAt(sprite, SLOT_SET_SCALE_XY, {sprite, bitsOf(fx), bitsOf(fy)});
    float ox = fx * float(offset->x), oy = fy * float(offset->y);
    rw::call(fn::VEC2_CTOR, {offsetPos, bitsOf(ox) ^ 0x80000000u, bitsOf(oy) ^ 0x80000000u});
    float nodeScaleX = fx * scaleX, nodeScaleY = fy * scaleY;

    uint32_t begin = expressions->begin, end = expressions->end;
    if (begin != end) {
        float invX = 1.0f / fx, invY = 1.0f / fy;
        for (uint32_t i = 0; begin != end; end -= 4, i++) {
            uint32_t expression = ld(begin + 4 * i);
            uint32_t node = rw::call(fn::AVATAR_ICON_CREATE_NODE,
                                     {offsetPos, avatar, size, sprite, offsetPos, bitsOf(nodeScaleX), bitsOf(nodeScaleY),
                                      expression});
            uint32_t rtSprite = ld(node + RT_SPRITE);
            vcallAt(rtSprite, SLOT_SET_SCALE_XY, {rtSprite, bitsOf(invX), bitsOf(invY)});
            vcallAt(node, SLOT_SET_CASCADE_COLOR, {node, 1});
            rw::call(fn::NODE_VECTOR_PUSH_BACK, {guest_addr(&self->nodes), node});
            *rw::frame_ptr<uint32_t>(pair) = ld(begin + 4 * i);   // expression -> index
            *rw::frame_ptr<uint32_t>(pair + 4) = i;
            rw::call(fn::INT_MAP_INSERT_UNIQUE, {inserted, guest_addr(&self->mapBuckets), pair});
            vcallAt(me, node_slot::ADD_CHILD, {me, node});
            if (i) vcallAt(node, node_slot::SET_VISIBLE, {node, 0});
        }
    }
    rw::call(fn::VEC2_DTOR, {offsetPos});
    return 1;
}
RW_FUNCTION(0x5C9E8C, "AvatarIcon::init", AvatarIcon_init);

// Render the avatar (in `expression`, at `position`, scaled) and the frame sprite into a new RenderTexture.
static uint32_t AvatarIcon_createNode(uint32_t /*unused this*/, uint32_t avatar, const bt::Vec2 *size, uint32_t sprite,
                                      uint32_t position, float scaleX, float scaleY, int32_t expression) {
    guest::Frame f(0x2C);
    const uint32_t blend = f.addr(0x20);
    uint32_t avatarLayer = rw::call(fn::RENDER_TEXTURE_CREATE_FORMAT,
                                    {uint32_t(bt::f2iz(size->x)), uint32_t(bt::f2iz(size->y)), PIXEL_FORMAT_RGBA8888,
                                     GL_DEPTH24_STENCIL8});
    uint32_t setPosition = ld(ld(avatarLayer) + node_slot::SET_POSITION * 4);
    rw::call(setPosition, {avatarLayer, vcallAt(sprite, node_slot::GET_POSITION, {sprite})});
    if (rw::load<int32_t>(avatar + AVATAR_EXPRESSION) != expression) {
        rw::call(fn::AVATAR_CHANGE_EXPRESSION, {avatar, uint32_t(expression)});
        rw::call(fn::AVATAR_CHANGE_MOTION, {avatar, ld(avatar + AVATAR_MOTION)});
    }
    vcallAt(avatar, node_slot::SET_POSITION, {avatar, position});
    vcallAt(avatar, SLOT_SET_SCALE_XY, {avatar, rw::fbits(scaleX), rw::fbits(scaleY)});
    vcallAt(avatarLayer, SLOT_RT_BEGIN_WITH_CLEAR, {avatarLayer, 0, 0, 0, 0});
    rw::call(fn::NODE_VISIT, {avatar});
    vcallAt(avatarLayer, SLOT_RT_END, {avatarLayer});

    uint32_t icon = rw::call(fn::RENDER_TEXTURE_CREATE, {uint32_t(bt::f2iz(size->x)), uint32_t(bt::f2iz(size->y))});
    vcallAt(icon, SLOT_RT_BEGIN_WITH_CLEAR, {icon, 0, 0, 0, 0});
    rw::call(fn::NODE_VISIT, {avatarLayer});
    uint32_t setBlend = ld(ld(sprite) + SLOT_SPRITE_SET_BLEND_FUNC * 4);
    *rw::frame_ptr<uint32_t>(blend) = 0;                        // GL_ZERO
    *rw::frame_ptr<uint32_t>(blend + 4) = GL_ONE_MINUS_SRC_ALPHA;
    rw::call(setBlend, {sprite, blend});
    rw::call(fn::NODE_VISIT, {sprite});
    vcallAt(icon, SLOT_RT_END, {icon});
    uint32_t iconSprite = ld(icon + RT_SPRITE);
    rw::call(fn::TEXTURE_SET_ANTI_ALIAS, {vcallAt(iconSprite, SLOT_SPRITE_GET_TEXTURE, {iconSprite})});
    vcallAt(icon, SLOT_SET_CASCADE_OPACITY, {icon, 1});
    return icon;
}
RW_FUNCTION(0x5CA234, "AvatarIcon::createNode", AvatarIcon_createNode);

// Show another expression: at once (duration 0) or fading the new one in over `duration` seconds, the old one
// hidden when the fade ends ($_0).
static void AvatarIcon_changeExpression(AvatarIcon *self, int32_t expression, float duration) {
    guest::Frame f(0x3C);
    const uint32_t key = f.addr(0x14), done = f.addr(0x18);
    *rw::frame_ptr<int32_t>(key) = expression;
    uint32_t entry = rw::call(fn::INT_MAP_FIND, {guest_addr(&self->mapBuckets), key});
    if (!entry) return;
    int32_t index = rw::load<int32_t>(entry + 12);
    int32_t current = self->current;
    if (index == current) return;
    uint32_t nodes = self->nodes.begin;
    uint32_t oldNode = ld(nodes + 4 * uint32_t(current));
    uint32_t newNode = ld(nodes + 4 * uint32_t(index));
    self->current = index;
    vcallAt(newNode, node_slot::SET_VISIBLE, {newNode, 1});
    if (duration == 0.0f) {
        vcallAt(oldNode, node_slot::SET_VISIBLE, {oldNode, 0});
        return;
    }
    vcallAt(oldNode, node_slot::SET_LOCAL_Z_ORDER, {oldNode, 0});
    vcallAt(newNode, node_slot::SET_LOCAL_Z_ORDER, {newNode, 1});
    vcallAt(newNode, node_slot::SET_OPACITY, {newNode, 0});
    uint32_t fade = rw::call(fn::FADE_TO_CREATE, {rw::fbits(duration), 255});
    *rw::frame_ptr<uint32_t>(done) = FADE_DONE_FUNC_VTABLE;
    *rw::frame_ptr<uint32_t>(done + 4) = oldNode;
    *rw::frame_ptr<uint32_t>(done + 8) = guest_addr(self);
    *rw::frame_ptr<uint32_t>(done + 12) = newNode;
    *rw::frame_ptr<uint32_t>(done + 16) = done;
    uint32_t callFunc = rw::call(fn::CALL_FUNC_CREATE, {done});
    rw::call(actor_fn::node_runAction, {newNode, rw::call(fn::SEQUENCE_CREATE, {fade, callFunc, 0})});
    fnthunk::destroy(done);
}
RW_FUNCTION(0x5CA458, "AvatarIcon::changeExpression(Avatar::Expression, float)", AvatarIcon_changeExpression);

// A copy sharing the rendered textures: each expression a flipped sprite of the source's RenderTexture.
static uint32_t AvatarIcon_clone(const AvatarIcon *self) {
    guest::Frame f(0x24);
    uint32_t p = newIcon(false);
    AvatarIcon *copy = rw::host_ptr<AvatarIcon>(p);
    uint32_t begin = self->nodes.begin, end = self->nodes.end;
    for (uint32_t i = 0; begin != end; end -= 4, i++) {
        uint32_t rtSprite = ld(ld(begin + 4 * i) + RT_SPRITE);
        uint32_t texture = vcallAt(rtSprite, SLOT_SPRITE_GET_TEXTURE, {rtSprite});
        uint32_t sprite = rw::call(fn::SPRITE_CREATE_WITH_TEXTURE, {texture});
        uint32_t setScale = ld(ld(sprite) + SLOT_SET_SCALE_XY * 4);
        uint32_t sx = vcallAt(rtSprite, node_slot::GET_SCALE_X, {rtSprite});
        uint32_t sy = vcallAt(rtSprite, node_slot::GET_SCALE_Y, {rtSprite});
        rw::call(setScale, {sprite, sx, sy});
        rw::call(fn::SPRITE_SET_FLIPPED_Y, {sprite, 1});
        uint32_t node = rw::call(actor_fn::node_create, {});
        vcallAt(node, node_slot::ADD_CHILD, {node, sprite});
        vcallAt(node, SLOT_SET_CASCADE_COLOR, {node, 1});
        rw::call(fn::NODE_VECTOR_PUSH_BACK, {guest_addr(&copy->nodes), node});
        vcallAt(p, node_slot::ADD_CHILD, {p, node});
        if (int32_t(i) != self->current) vcallAt(node, node_slot::SET_VISIBLE, {node, 0});
    }
    if (copy != self) {
        copy->mapMaxLoadFactor = self->mapMaxLoadFactor;
        rw::call(fn::INT_MAP_ASSIGN_MULTI, {guest_addr(&copy->mapBuckets), self->mapFirst, 0});
    }
    copy->current = self->current;
    rw::call(actor_fn::ref_autorelease, {p});
    return p;
}
RW_FUNCTION(0x5CA5C0, "AvatarIcon::clone", AvatarIcon_clone);

static AvatarIcon *AvatarIcon_destruct(AvatarIcon *self) {
    const uint32_t me = guest_addr(self);
    self->vtable = AvatarIcon::VTABLE;
    rw::call(fn::INT_MAP_DTOR, {guest_addr(&self->mapBuckets)});
    rw::call(fn::NODE_VECTOR_DTOR, {guest_addr(&self->nodes)});
    rw::call(actor_fn::node_dtor, {me});
    return self;
}
RW_FUNCTION(0x5CBDA0, "AvatarIcon::~AvatarIcon", AvatarIcon_destruct);

static void AvatarIcon_destructDelete(AvatarIcon *self) {
    const uint32_t me = guest_addr(self);
    self->vtable = AvatarIcon::VTABLE;
    rw::call(fn::INT_MAP_DTOR, {guest_addr(&self->mapBuckets)});
    rw::call(fn::NODE_VECTOR_DTOR, {guest_addr(&self->nodes)});
    rw::call(actor_fn::node_dtor, {me});
    guest::deleteObject(me);
}
RW_FUNCTION(0x5CBDE0, "AvatarIcon::~AvatarIcon (deleting)", AvatarIcon_destructDelete);

// ------------------------------------------------------------------------------------------------ lambdas
// changeExpression $_0 (captures old node, icon, new node): when the fade ends, hide the old node unless it became
// the current one again, and make the new one fully opaque.
static uint32_t AvatarIcon_fadeDone(uint32_t lambda) {
    uint32_t oldNode = ld(lambda + 4), icon = ld(lambda + 8);
    const AvatarIcon *self = rw::host_ptr<AvatarIcon>(icon);
    if (oldNode != ld(self->nodes.begin + 4 * uint32_t(int32_t(self->current))))
        vcallAt(oldNode, node_slot::SET_VISIBLE, {oldNode, 0});
    uint32_t newNode = ld(lambda + 12);
    return vcallAt(newNode, node_slot::SET_OPACITY, {newNode, 255});
}
static uint32_t AvatarIcon_fadeDoneCloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    rw::save<uint32_t>(dst, FADE_DONE_FUNC_VTABLE);
    uint32_t a = ld(self + 4), b = ld(self + 8), c = ld(self + 12);
    rw::save<uint32_t>(dst + 12, c);
    rw::save<uint32_t>(dst + 8, b);
    rw::save<uint32_t>(dst + 4, a);
    return c;
}
RW_FUNCTION(0x5CBB10, "AvatarIcon::changeExpression $_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5CBB18, "AvatarIcon::changeExpression $_0 __func::__clone()", (fnthunk::cloneWords<FADE_DONE_FUNC_VTABLE, 16, 3>));
RW_FUNCTION(0x5CBB48, "AvatarIcon::changeExpression $_0 __func::__clone(__base*)", AvatarIcon_fadeDoneCloneInto);
RW_FUNCTION(0x5CBB70, "AvatarIcon::changeExpression $_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5CBB72, "AvatarIcon::changeExpression $_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5CBB7A, "AvatarIcon::changeExpression $_0 (fade done: hide the old expression)", AvatarIcon_fadeDone);
RW_FUNCTION(0x5CBBB4, "AvatarIcon::changeExpression $_0 __func::target", fnthunk::target<FADE_DONE_TYPE_NAME>);
RW_FUNCTION(0x5CBBD4, "AvatarIcon::changeExpression $_0 __func::target_type", fnthunk::targetType<FADE_DONE_TYPE_INFO>);

// (anonymous namespace)::$_0 of the Avatar translation unit (no captures): a movie callback that clears the
// movie's +72 field and calls virtual slot 8 of its +24 object; returns 0.
static uint32_t AvatarIcon_anonMovieCallback(uint32_t /*lambda*/, const uint32_t *movieArg) {
    uint32_t movie = *movieArg;
    rw::save<uint32_t>(movie + 72, 0);
    uint32_t target = ld(movie + 24);
    vcallAt(target, 8, {target});
    return 0;
}
RW_FUNCTION(0x5C9B46, "(anonymous namespace)::$_0 __func::~__func", fnthunk::nothing);
RW_FUNCTION(0x5C9B48, "(anonymous namespace)::$_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C9B50, "(anonymous namespace)::$_0 __func::__clone()", fnthunk::clone0<ANON_FUNC_VTABLE>);
RW_FUNCTION(0x5C9B74, "(anonymous namespace)::$_0 __func::__clone(__base*)", fnthunk::cloneInto0<ANON_FUNC_VTABLE>);
RW_FUNCTION(0x5C9B90, "(anonymous namespace)::$_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C9B92, "(anonymous namespace)::$_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C9B9A, "(anonymous namespace)::$_0 __func::operator()", AvatarIcon_anonMovieCallback);
RW_FUNCTION(0x5C9BB0, "(anonymous namespace)::$_0 __func::target", fnthunk::target<ANON_TYPE_NAME>);
RW_FUNCTION(0x5C9BD0, "(anonymous namespace)::$_0 __func::target_type", fnthunk::targetType<ANON_TYPE_INFO>);

// AvatarIconCache::setLimit(unsigned)::$_1 (every icon costs 1) and the last thunk of add()'s shared_ptr deleter
// block - attributed here by position (AvatarIconCache's own file is another group's).
static uint32_t AvatarIcon_costOne() { return 1; }
RW_FUNCTION(0x5CB5F0, "AvatarIconCache::add $_2 __shared_ptr_pointer::__on_zero_shared_weak", fnthunk::deleteThis);
RW_FUNCTION(0x5CB5F8, "AvatarIconCache::setLimit $_1 __func::~__func", fnthunk::nothing);
RW_FUNCTION(0x5CB5FA, "AvatarIconCache::setLimit $_1 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5CB604, "AvatarIconCache::setLimit $_1 __func::__clone()", fnthunk::clone0<SET_LIMIT_FUNC_VTABLE>);
RW_FUNCTION(0x5CB628, "AvatarIconCache::setLimit $_1 __func::__clone(__base*)", fnthunk::cloneInto0<SET_LIMIT_FUNC_VTABLE>);
RW_FUNCTION(0x5CB644, "AvatarIconCache::setLimit $_1 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5CB646, "AvatarIconCache::setLimit $_1 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5CB64E, "AvatarIconCache::setLimit $_1 (cost of an icon: 1)", AvatarIcon_costOne);
RW_FUNCTION(0x5CB654, "AvatarIconCache::setLimit $_1 __func::target", fnthunk::target<SET_LIMIT_TYPE_NAME>);
RW_FUNCTION(0x5CB674, "AvatarIconCache::setLimit $_1 __func::target_type", fnthunk::targetType<SET_LIMIT_TYPE_INFO>);
