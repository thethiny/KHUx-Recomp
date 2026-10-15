#include "AimTarget.hpp"

namespace {
namespace g {
constexpr uint32_t ref_ctor = 0x8FD025;               // cocos2d::Ref::Ref()
constexpr uint32_t ref_retain = 0x8FD0B5;             // cocos2d::Ref::retain()
constexpr uint32_t ref_release = 0x8FD0BD;            // cocos2d::Ref::release()
constexpr uint32_t vec2_default = 0x8E4B95;           // cocos2d::Vec2::Vec2()
constexpr uint32_t vec2_dtor = 0x8E4C17;              // cocos2d::Vec2::~Vec2()
constexpr uint32_t director_getInstance = 0x8EE519;   // cocos2d::Director::getInstance()
constexpr uint32_t director_getVisibleSize = 0x8F0675;  // cocos2d::Director::getVisibleSize() const (sret Size)
constexpr uint32_t touch_getLocationInView = 0x900E39;  // cocos2d::Touch::getLocationInView() const (sret Vec2)
constexpr uint32_t node_stopAllActions = 0x893815;    // cocos2d::Node::stopAllActions()
constexpr uint32_t node_runAction = 0x894AED;         // cocos2d::Node::runAction(Action*)
constexpr uint32_t node_removeAllChildrenWithCleanup = 0x894369;  // cocos2d::Node::removeAllChildrenWithCleanup(bool)
constexpr uint32_t fadeOut_create = 0x86B25D;         // cocos2d::FadeOut::create(float)
constexpr uint32_t hide_create = 0x8665E9;            // cocos2d::Hide::create()
constexpr uint32_t sequence_create = 0x867999;        // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t mapNode_ctor = 0x5DA2D5;           // MapNode::MapNode()
constexpr uint32_t mapNode_dtor = 0x5DA38D;           // MapNode::~MapNode()
constexpr uint32_t mapNode_init = 0x5DA411;           // MapNode::init()
constexpr uint32_t string_init = 0x47CD93;            // std::string::__init(const char*, size_t)
constexpr uint32_t adm_getInstance = 0x7AC9B5;        // cocostudio::ArmatureDataManager::getInstance()
constexpr uint32_t adm_addArmatureFileInfo = 0x7AD7BD;  // ArmatureDataManager::addArmatureFileInfo(const std::string&)
constexpr uint32_t armature_create = 0x79F3C7;        // cocostudio::Armature::create(const std::string&)
constexpr uint32_t strvec_allocate = 0x47B5FD;        // std::vector<std::string>::allocate(size_t)
constexpr uint32_t strvec_constructAtEnd = 0x5E407D;  // std::vector<std::string>::__construct_at_end(first, last)
constexpr uint32_t strvec_baseDtor = 0x47363F;        // std::__vector_base<std::string>::~__vector_base()
constexpr uint32_t function_assign = 0x4EB9CD;        // std::function<void()>::operator=(const function&)
// std::function functor vtables of the marker's lambdas (capture the marker)
constexpr uint32_t fn_markerShow = 0xF6DCC0;
constexpr uint32_t fn_markerHide = 0xF6DC94;
}  // namespace g

namespace str {
constexpr uint32_t ActionMapAnimationJson = 0xE02831; // "cocostudio/publish/ActionMapAnimation.ExportJson" (48)
constexpr uint32_t ActionMapAnimation = 0xE01C44;     // (18)
constexpr uint32_t MoveMarker_anm = 0xE07344;         // "MoveMarker-anm" (14)
}  // namespace str

constexpr uint32_t VS_NODE_ADDCHILD_Z = 61;           // Node::addChild(Node*, int z)
constexpr uint32_t VS_NODE_SETVISIBLE = 44;           // Node::setVisible(bool)
constexpr uint32_t VS_NODE_ISVISIBLE = 45;            // Node::isVisible()
constexpr uint32_t VS_NODE_SETLOCALZORDER = 83;
constexpr uint32_t VS_NODE_SETOPACITY = 133;          // Node::setOpacity(GLubyte)
constexpr uint32_t VS_NODE_SETCASCADEOPACITY = 136;   // Node::setCascadeOpacityEnabled(bool)
constexpr uint32_t VS_ARMATURE_GETANIMATION = 166;    // Armature::getAnimation()
constexpr uint32_t VS_ANIM_PLAYWITHNAMES = 29;        // ArmatureAnimation::playWithNames(names, durationTo, loop)

void setWord(void *obj, uint32_t off, uint32_t v) { rw::set<uint32_t>(obj, off, v); }
uint32_t getWord(const void *obj, uint32_t off) { return rw::get<uint32_t>(obj, off); }

// Touch::getLocationInView into a guest temporary, copied (raw) to dst.
void locationInView(uint32_t touch, bt::Vec2 *dst) {
    bt::GuestFrame frame(8);
    rw::call(g::touch_getLocationInView, {frame.addr(), touch});
    setWord(dst, 0, frame.word(0));
    setWord(dst, 4, frame.word(4));
    rw::call(g::vec2_dtor, {frame.addr()});
}

void makeString(const bt::GuestFrame &frame, uint32_t off, uint32_t cstr, uint32_t len) {
    rw::call(g::string_init, {frame.addr(off), cstr, len});
}
void freeString(const bt::GuestFrame &frame, uint32_t off) {
    if (rw::load<uint8_t>(frame.addr(off)) & 1) rw::call(bt::fn::operator_delete, {frame.word(off + 8)});
}
// std::function destructor: in-place callable -> destroy() (slot 4), heap callable -> destroy_deallocate() (slot 5).
void destroyFunction(uint32_t fn) {
    uint32_t f = rw::load<uint32_t>(fn + 16);
    if (f == fn) rw::vcall(rw::host_ptr<void>(fn), 4, {fn});
    else if (f) rw::vcall(rw::host_ptr<void>(f), 5, {f});
}
}  // namespace

// ------------------------------------------------------------------------------------------- AimTarget

AimTarget *AimTarget::create() {
    auto *self = rw::host_ptr<AimTarget>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self);
    if (rw::vcall(self, 2, {rw::arg(self)}) == 1) {            // init()
        rw::call(bt::fn::ref_autorelease, {rw::arg(self)});
        return self;
    }
    rw::vcall(self, 1, {rw::arg(self)});                       // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5FC974, "AimTarget::create", AimTarget::create);

AimTarget *AimTarget::construct(AimTarget *self) {
    rw::call(g::ref_ctor, {rw::arg(self)});
    self->vtable = VTABLE;
    rw::call(g::vec2_default, {rw::arg(&self->touchStart)});
    rw::call(g::vec2_default, {rw::arg(&self->touchCurrent)});
    setWord(self, 72, 0);                                      // callback: empty
    self->markerTimer = 0.0f;
    self->dragThreshold = 0.0f;
    self->marker = 0;
    self->disabled = 0;
    self->active = 0;
    self->touching = 0;
    self->field_24 = 0;
    self->stopped = 0;
    return self;
}
RW_FUNCTION(0x607C60, "AimTarget::AimTarget", AimTarget::construct);

AimTarget *AimTarget::destruct(AimTarget *self) {
    self->vtable = VTABLE;
    if (uint32_t m = self->marker) rw::call(g::ref_release, {m});   // cocos2d::Ref::release
    destroyFunction(rw::arg(self->callback));
    rw::call(g::vec2_dtor, {rw::arg(&self->touchCurrent)});
    rw::call(g::vec2_dtor, {rw::arg(&self->touchStart)});
    rw::call(bt::fn::ref_dtor, {rw::arg(self)});
    return self;
}
RW_FUNCTION(0x73F40C, "AimTarget::~AimTarget", AimTarget::destruct);

void AimTarget::destructDelete(AimTarget *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {rw::arg(self)});
}
RW_FUNCTION(0x73F4BC, "AimTarget::~AimTarget(deleting)", AimTarget::destructDelete);

uint32_t AimTarget::init(AimTarget *self) {
    AimMarker *marker = AimMarker::create();
    self->marker = rw::arg(marker);
    rw::call(g::ref_retain, {rw::arg(marker)});               // cocos2d::Ref::retain
    uint32_t director = rw::call(g::director_getInstance, {});
    {
        bt::GuestFrame frame(8);
        rw::call(g::director_getVisibleSize, {frame.addr(), director});
        self->dragThreshold = bt::fromBits(frame.word(4)) * 0.125f;
    }
    self->touching = 0;
    self->active = 0;
    self->markerTimer = 0.0f;
    if (uint32_t m = self->marker) AimMarker::hide(rw::host_ptr<AimMarker>(m));
    return 1;
}
RW_FUNCTION(0x73F4C8, "AimTarget::init", AimTarget::init);

void AimTarget::reset(AimTarget *self) {
    self->touching = 0;
    self->active = 0;
    self->markerTimer = 0.0f;
    if (uint32_t m = self->marker) AimMarker::hide(rw::host_ptr<AimMarker>(m));
}
RW_FUNCTION(0x73F5B4, "AimTarget::reset", AimTarget::reset);

void AimTarget::onBegan(AimTarget *self, uint32_t touch) {
    if (self->stopped != 0 || self->disabled != 0) return;
    locationInView(touch, &self->touchStart);
    uint32_t x = getWord(self, 20), y = getWord(self, 24);
    setWord(self, 32, y);
    setWord(self, 28, x);                                      // touchCurrent = touchStart
    self->touching = 1;
    self->active = 1;
    self->markerTimer = 0.0f;
    if (uint32_t m = self->marker) rw::host_ptr<AimMarker>(m)->deltaTime = 0.0f;
}
RW_FUNCTION(0x73F5D0, "AimTarget::onBegan", AimTarget::onBegan);

void AimTarget::onMoved(AimTarget *self, uint32_t touch) {
    if (self->stopped != 0 || self->disabled != 0) return;
    locationInView(touch, &self->touchCurrent);
    if (self->touching == 0) {                                 // the drag started outside: start here
        uint32_t x = getWord(self, 28), y = getWord(self, 32);
        setWord(self, 20, x);
        setWord(self, 24, y);
    }
    self->touching = 1;
    self->active = 1;
    self->markerTimer = 0.0f;
}
RW_FUNCTION(0x73F66C, "AimTarget::onMoved", AimTarget::onMoved);

void AimTarget::onEnded(AimTarget *self, uint32_t touch) {
    if (self->stopped != 0 || self->disabled != 0) {
        self->stopped = 0;
        return;
    }
    if (self->touching == 0) return;
    locationInView(touch, &self->touchCurrent);
    self->touching = 0;
    self->markerTimer = 0.0f;
}
RW_FUNCTION(0x73F6FC, "AimTarget::onEnded", AimTarget::onEnded);

void AimTarget::onCancelled(AimTarget *self) {
    self->touching = 0;
    self->active = 0;
    self->markerTimer = 0.0f;
    self->stopped = 0;
}
RW_FUNCTION(0x73F780, "AimTarget::onCancelled", AimTarget::onCancelled);

void AimTarget::stop(AimTarget *self, uint32_t stopped) {
    self->touching = 0;
    self->active = 0;
    self->markerTimer = 0.0f;
    self->stopped = uint8_t(stopped);
    if (uint32_t m = self->marker) rw::vcall(rw::host_ptr<void>(m), VS_NODE_SETVISIBLE, {m, 0});
}
RW_FUNCTION(0x73F7E0, "AimTarget::stop", AimTarget::stop);

// After release the marker stays for 0.5 s (unless `keep`), then fades out.
void AimTarget::updateMarker(AimTarget *self, float dt, uint32_t keep) {
    float t = self->markerTimer;
    if (!(t > 0.0f)) {                                         // t <= 0 or NaN
        if (keep == 0) self->markerTimer = 0.5f;
        return;
    }
    float left = t - dt;
    self->markerTimer = left;
    if (left > 0.0f || std::isnan(left)) return;
    self->touching = 0;
    self->active = 0;
    self->markerTimer = 0.0f;
    if (uint32_t m = self->marker) AimMarker::hide(rw::host_ptr<AimMarker>(m));
}
RW_FUNCTION(0x73F804, "AimTarget::updateMarker", AimTarget::updateMarker);

// ------------------------------------------------------------------------------------------- AimMarker

AimTarget::AimMarker *AimTarget::AimMarker::create() {
    auto *self = rw::host_ptr<AimMarker>(rw::call(bt::fn::operator_new, {SIZE}));
    rw::call(g::mapNode_ctor, {rw::arg(self)});               // MapNode::MapNode
    self->vtable = VTABLE;
    self->deltaTime = 0.0f;
    if (init(self) == 1) {
        rw::call(bt::fn::ref_autorelease, {rw::arg(self)});
        return self;
    }
    rw::vcall(self, 1, {rw::arg(self)});                       // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x73F544, "AimTarget::AimMarker::create", AimTarget::AimMarker::create);

float AimTarget::AimMarker::takeDeltaTime(AimMarker *self) {
    float dt = self->deltaTime;
    self->deltaTime = 0.0f;
    return dt;
}
RW_FUNCTION(0x73F65C, "AimTarget::AimMarker::takeDeltaTime", AimTarget::AimMarker::takeDeltaTime);

// Fade out (0.2 s) and hide, when visible.
void AimTarget::AimMarker::hide(AimMarker *self) {
    if (rw::vcall(self, VS_NODE_ISVISIBLE, {rw::arg(self)}) != 1) return;
    rw::call(g::node_stopAllActions, {rw::arg(self)});
    uint32_t fade = rw::call(g::fadeOut_create, {bt::bits(0.2f)});   // cocos2d::FadeOut::create
    uint32_t hide = rw::call(g::hide_create, {});                    // cocos2d::Hide::create
    uint32_t seq = rw::call(g::sequence_create, {fade, hide, 0});
    rw::call(g::node_runAction, {rw::arg(self), seq});
}
RW_FUNCTION(0x73F794, "AimTarget::AimMarker::hide", AimTarget::AimMarker::hide);

AimTarget::AimMarker *AimTarget::AimMarker::destruct(AimMarker *self) {
    self->vtable = VTABLE;
    rw::call(g::node_removeAllChildrenWithCleanup, {rw::arg(self), 1});
    rw::call(g::mapNode_dtor, {rw::arg(self)});               // MapNode::~MapNode
    return self;
}
RW_FUNCTION(0x73F898, "AimTarget::AimMarker::~AimMarker", AimTarget::AimMarker::destruct);

void AimTarget::AimMarker::destructDelete(AimMarker *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {rw::arg(self)});
}
RW_FUNCTION(0x73F8E0, "AimTarget::AimMarker::~AimMarker(deleting)", AimTarget::AimMarker::destructDelete);

// Load ActionMapAnimation, play "MoveMarker-anm" in a loop, set the show / hide callbacks.
uint32_t AimTarget::AimMarker::init(AimMarker *self) {
    if (rw::call(g::mapNode_init, {rw::arg(self)}) != 1) return 0;   // MapNode::init
    const uint32_t me = rw::arg(self);
    rw::vcall(self, VS_NODE_SETLOCALZORDER, {me, 7});
    rw::vcall(self, VS_NODE_SETVISIBLE, {me, 0});
    rw::vcall(self, VS_NODE_SETCASCADEOPACITY, {me, 1});
    // guest frame offsets as in the original
    enum : uint32_t { NAMES = 0x1C, STR_ARM = 0x28, STR_JSON = 0x34, FN_HIDE = 0x40, FN_SHOW = 0x58, STR_MOVE = 0x70 };
    bt::GuestFrame frame(0x88);
    uint32_t adm = rw::call(g::adm_getInstance, {});
    makeString(frame, STR_JSON, str::ActionMapAnimationJson, 0x30);
    rw::call(g::adm_addArmatureFileInfo, {adm, frame.addr(STR_JSON)});
    freeString(frame, STR_JSON);
    makeString(frame, STR_ARM, str::ActionMapAnimation, 0x12);
    uint32_t armature = rw::call(g::armature_create, {frame.addr(STR_ARM)});
    freeString(frame, STR_ARM);
    uint32_t anim = rw::vcall(rw::host_ptr<void>(armature), VS_ARMATURE_GETANIMATION, {armature});
    uint32_t playWithNames = rw::load<uint32_t>(rw::load<uint32_t>(anim) + VS_ANIM_PLAYWITHNAMES * 4);
    makeString(frame, STR_MOVE, str::MoveMarker_anm, 14);
    // std::vector<std::string> names{"MoveMarker-anm"}
    frame.setWord(NAMES, 0);
    frame.setWord(NAMES + 4, 0);
    frame.setWord(NAMES + 8, 0);
    rw::call(g::strvec_allocate, {frame.addr(NAMES), 1});
    rw::call(g::strvec_constructAtEnd, {frame.addr(NAMES), frame.addr(STR_MOVE), frame.addr(STR_MOVE + 12)});
    rw::call(playWithNames, {anim, frame.addr(NAMES), 0xFFFFFFFFu, 1});
    rw::call(g::strvec_baseDtor, {frame.addr(NAMES)});
    freeString(frame, STR_MOVE);
    rw::vcall(self, VS_NODE_ADDCHILD_Z, {me, armature, 1});
    // onShow / onHide = lambdas capturing the marker
    frame.setWord(FN_SHOW + 16, frame.addr(FN_SHOW));
    frame.setWord(FN_SHOW, g::fn_markerShow);
    frame.setWord(FN_SHOW + 4, me);
    rw::call(g::function_assign, {rw::arg(self->onShow), frame.addr(FN_SHOW)});
    destroyFunction(frame.addr(FN_SHOW));
    frame.setWord(FN_HIDE + 16, frame.addr(FN_HIDE));
    frame.setWord(FN_HIDE, g::fn_markerHide);
    frame.setWord(FN_HIDE + 4, me);
    rw::call(g::function_assign, {rw::arg(self->onHide), frame.addr(FN_HIDE)});
    destroyFunction(frame.addr(FN_HIDE));
    return 1;
}
RW_FUNCTION(0x73F8EC, "AimTarget::AimMarker::init", AimTarget::AimMarker::init);

void AimTarget::AimMarker::show(AimMarker *self) {
    rw::call(g::node_stopAllActions, {rw::arg(self)});
    rw::vcall(self, VS_NODE_SETOPACITY, {rw::arg(self), 255});
    rw::vcall(self, VS_NODE_SETVISIBLE, {rw::arg(self), 1});
}
RW_FUNCTION(0x73FBA0, "AimTarget::AimMarker::show", AimTarget::AimMarker::show);

void AimTarget::AimMarker::update(AimMarker *self, float dt) { self->deltaTime = self->deltaTime + dt; }
RW_FUNCTION(0x73FBCA, "AimTarget::AimMarker::update", AimTarget::AimMarker::update);
