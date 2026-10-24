// BattleMarker: the raid battle's tappable target marker.
#include "BattleMarker.hpp"
#include "StageUI_Thunks.hpp"

using namespace scene;

namespace {
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;          // (Vec2 out, node, const Vec2&)
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;                // (Vec2 out, touch)
constexpr uint32_t Touch_getPreviousLocation = rwlib::cocos2d::Touch::getPreviousLocation;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;                        // (rect, x, y, w, h)
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t TouchBoolFunction_swap = 0x483E4D;           // std::function<bool(Touch*,Event*)>::swap
constexpr uint32_t TouchVoidFunction_swap = 0x4F3699;           // std::function<void(Touch*,Event*)>::swap
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t Misc_create = 0x786D5D;                      // master::Misc::create(shared_ptr out, id)
constexpr uint32_t RaidBossHPIndicator_create = 0x5E5165;       // (Enemy_Type, Type, hp, misc value)
constexpr uint32_t RaidBossHPIndicator_setHP = 0x5E6305;
constexpr uint32_t MarkerFunction_call = 0x5D6109;              // std::function<void(BattleMarker*)>::operator()

// virtual slots (byte offsets)
constexpr uint32_t Node_getEventDispatcher = 0x1A8;
constexpr uint32_t Armature_getAnimation = 0x298;
constexpr uint32_t Anim_playWithIndex = 0x70;                   // (index, durationTo, loop)

// listener callbacks (EventListenerTouchOneByOne fields)
constexpr uint32_t LISTENER_ON_TOUCH_BEGAN = 0x50;
constexpr uint32_t LISTENER_ON_TOUCH_MOVED = 0x68;
constexpr uint32_t LISTENER_ON_TOUCH_ENDED = 0x80;
constexpr uint32_t LISTENER_ON_TOUCH_CANCELLED = 0x98;

constexpr uint32_t STR_ExportJsonFmt = 0xE01EED;   // "cocostudio/publish/%s.ExportJson"
constexpr uint32_t STR_TapAnim = 0xE01F70;         // "ActMapTapAnim"

constexpr uint32_t MISC_BOSS_MARKER = 203;         // master::Misc ids: HP indicator parameter (int at 4)
constexpr uint32_t MISC_ENEMY_MARKER = 201;

// ring animation per state (index = color * 4 + state)
constexpr uint32_t RING_WAIT = 0, RING_PRESSED = 1, RING_SELECTED = 2, RING_SELECT = 3;
constexpr uint32_t CAPTION_ANIMATION = 8;
constexpr float PRESS_TIME = 0.166666672f;

// lambda vtables (__func + 8)
constexpr uint32_t VT_TouchBegan = 0xF5D650;       // BattleMarker(...)::$_0
constexpr uint32_t VT_TouchMoved = 0xF5D624;       // $_1
constexpr uint32_t VT_TouchEnded = 0xF5D5F8;       // $_2
constexpr uint32_t VT_TouchCancelled = 0xF5D5CC;   // $_3
constexpr uint32_t VT_ShowSelected = 0xF5D5A0;     // show_selected()::$_4
constexpr uint32_t VT_PressDone = 0xF5D67C;        // $_0::operator()::{lambda()#1}

void playRing(uint32_t armature, uint32_t index) {
    uint32_t anim = vcall(armature, Armature_getAnimation, {armature});
    vcall(anim, Anim_playWithIndex, {anim, index, 0xFFFFFFFFu, 0xFFFFFFFFu});
}

uint32_t ringIndex(const BattleMarker *self, uint32_t state) { return (uint32_t(self->color) << 2) | state; }

// the touch area: (-120, -120, 240, 240) around the marker's origin, in node space
bool hit(uint32_t marker, uint32_t pos) {
    Frame f;
    uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, scene::fbits(-120.0f), scene::fbits(-120.0f), scene::fbits(240.0f), scene::fbits(240.0f)});
    (void)marker;
    return call(Rect_containsPoint, {rect, pos}) == 1;
}

// touch location (or previous location) in the marker's node space, into `out` (8 bytes)
void touchPoint(uint32_t out, uint32_t marker, uint32_t touch, uint32_t getter) {
    Frame f;
    uint32_t world = f.alloc(8);
    call(getter, {world, touch});
    call(Node_convertToNodeSpace, {out, marker, world});
}

void setTouchCallback(uint32_t listener, uint32_t field, uint32_t swap, uint32_t vtable, uint32_t marker) {
    Frame f;
    uint32_t cb = newFunction(f, vtable, {marker});
    call(swap, {cb, listener + field});
    functionDestroy(cb);
}

// run `then` (a lambda capturing the marker) on the ring after the press time
void afterPress(BattleMarker *self, uint32_t vtable) {
    uint32_t delay = call(DelayTime_create, {scene::fbits(PRESS_TIME)});
    uint32_t callback;
    {
        Frame f;
        uint32_t fn = newFunction(f, vtable, {rw::guest_addr(self)});
        callback = call(CallFunc_create, {fn});
        functionDestroy(fn);
    }
    uint32_t ring = self->ring;
    call(Node_runAction, {ring, call(Sequence_create, {delay, callback, 0})});
}

// the master::Misc parameter of the HP indicator (int at data 4)
int32_t miscValue(uint32_t id) {
    Frame f;
    uint32_t row = masterRow(f, Misc_create, id);
    int32_t v = safe32(ld32(row), 4);
    sharedRelease(row);
    return v;
}
}  // namespace

BattleMarker *BattleMarker::create(uint32_t type, uint32_t hp) {
    uint32_t a = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<BattleMarker>(a), type, hp);
    call(Ref_autorelease, {a});
    return rw::host_ptr<BattleMarker>(a);
}
RW_FUNCTION(0x5D553C, "BattleMarker::create", BattleMarker::create);

// type: RaidBossHPIndicator::Type (3 = raid boss marker); hp: the indicator's HP.
BattleMarker *BattleMarker::construct(BattleMarker *self, uint32_t type, uint32_t hp) {
    uint32_t a = rw::guest_addr(self);
    call(Node_ctor, {a});
    self->vtable = VTABLE;
    self->ring = 0;
    self->caption = 0;
    self->hpIndicator = 0;
    self->ringOnly = 0;
    self->color = 1;
    self->pressed = 0;
    rw::set<uint32_t>(self, 672, 0);          // onTap: empty
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    setTouchCallback(listener, LISTENER_ON_TOUCH_BEGAN, TouchBoolFunction_swap, VT_TouchBegan, a);
    setTouchCallback(listener, LISTENER_ON_TOUCH_MOVED, TouchVoidFunction_swap, VT_TouchMoved, a);
    setTouchCallback(listener, LISTENER_ON_TOUCH_ENDED, TouchVoidFunction_swap, VT_TouchEnded, a);
    setTouchCallback(listener, LISTENER_ON_TOUCH_CANCELLED, TouchVoidFunction_swap, VT_TouchCancelled, a);
    uint32_t dispatcher = vcall(a, Node_getEventDispatcher, {a});
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, a});
    uint32_t manager = call(ArmatureDataManager_getInstance, {});
    {
        Frame f;
        uint32_t buf = stringBuffer(f, {STR_ExportJsonFmt, STR_TapAnim});
        uint32_t s = newStr(f, buf);
        call(ArmatureDataManager_addArmatureFileInfo, {manager, s});
        strFree(s);
    }
    for (uint32_t k = 0; k < 2; k++) {
        Frame f;
        uint32_t s = newStr(f, STR_TapAnim, 13);
        uint32_t armature = call(Armature_create, {s});
        if (k == 0) self->ring = armature;
        else self->caption = armature;
        strFree(s);
    }
    vcall(a, node::addChildZ, {a, self->ring, 10});
    vcall(a, node::addChildZ, {a, self->caption, 11});
    uint32_t indicator;
    int32_t color;
    if (type == 3) {
        int32_t v = miscValue(MISC_BOSS_MARKER);
        indicator = call(RaidBossHPIndicator_create, {3, 3, hp, uint32_t(v)});
        color = 0;
    } else {
        int32_t v = miscValue(MISC_ENEMY_MARKER);
        indicator = call(RaidBossHPIndicator_create, {0, type, hp, uint32_t(v)});
        color = 1;
    }
    self->hpIndicator = indicator;
    self->color = color;
    playRing(self->ring, uint32_t(self->color) << 2);
    playRing(self->caption, CAPTION_ANIMATION);
    uint32_t caption = self->caption;
    vcall(caption, node::setVisible, {caption, 0});
    indicator = self->hpIndicator;
    if (indicator) {
        {
            Frame f;
            vcall(indicator, node::setPosition, {indicator, newVec2(f, 0.0f, 54.0f)});
        }
        vcall(a, node::addChild, {a, self->hpIndicator});
    }
    vcall(a, node::setVisible, {a, 0});
    return self;
}
RW_FUNCTION(0x5D5580, "BattleMarker::BattleMarker", BattleMarker::construct);

void BattleMarker::set_scene(BattleMarker *self, uint32_t scene) { self->scene = scene; }
RW_FUNCTION(0x5D59F4, "BattleMarker::set_scene", BattleMarker::set_scene);

uint32_t BattleMarker::show(BattleMarker *self) {
    uint32_t a = rw::guest_addr(self);
    vcall(a, node::setVisible, {a, 1});
    uint32_t ring = self->ring;
    return vcall(ring, node::setVisible, {ring, 1});
}
RW_FUNCTION(0x5D59FC, "BattleMarker::show", BattleMarker::show);

// The idle ring (and the caption's animation), shown.
uint32_t BattleMarker::show_wait(BattleMarker *self) {
    playRing(self->ring, ringIndex(self, RING_WAIT));
    playRing(self->caption, CAPTION_ANIMATION);
    return show(self);
}
RW_FUNCTION(0x5D5A26, "BattleMarker::show_wait", BattleMarker::show_wait);

uint32_t BattleMarker::show_select(BattleMarker *self) {
    playRing(self->ring, ringIndex(self, RING_SELECT));
    playRing(self->caption, CAPTION_ANIMATION);
    return show(self);
}
RW_FUNCTION(0x5D5A9A, "BattleMarker::show_select", BattleMarker::show_select);

uint32_t BattleMarker::disp_select_string(BattleMarker *self) {
    uint32_t caption = self->caption;
    return vcall(caption, node::setVisible, {caption, 1});
}
RW_FUNCTION(0x5D5B12, "BattleMarker::disp_select_string", BattleMarker::disp_select_string);

// Pressed ring, then the selected ring after the press time ($_4); the caption is hidden.
uint32_t BattleMarker::show_selected(BattleMarker *self) {
    playRing(self->ring, ringIndex(self, RING_PRESSED));
    afterPress(self, VT_ShowSelected);
    uint32_t a = rw::guest_addr(self);
    vcall(a, node::setVisible, {a, 1});
    uint32_t ring = self->ring;
    vcall(ring, node::setVisible, {ring, 1});
    uint32_t caption = self->caption;
    vcall(caption, node::setVisible, {caption, 0});
    return 0;
}
RW_FUNCTION(0x5D5B28, "BattleMarker::show_selected", BattleMarker::show_selected);

uint32_t BattleMarker::show_selected_random(BattleMarker *self) {
    playRing(self->ring, ringIndex(self, RING_PRESSED));
    uint32_t a = rw::guest_addr(self);
    vcall(a, node::setVisible, {a, 1});
    uint32_t ring = self->ring;
    vcall(ring, node::setVisible, {ring, 1});
    uint32_t caption = self->caption;
    return vcall(caption, node::setVisible, {caption, 0});
}
RW_FUNCTION(0x5D5C54, "BattleMarker::show_selected_random", BattleMarker::show_selected_random);

// Place the marker (x, y) with a ring colour and show it idle.
uint32_t BattleMarker::showEff(BattleMarker *self, float x, float y, int32_t color) {
    self->color = color;
    uint32_t a = rw::guest_addr(self);
    {
        Frame f;
        vcall(a, node::setPosition, {a, newVec2(f, x, y)});
    }
    show_wait(self);
    return 0;
}
RW_FUNCTION(0x5D5CBC, "BattleMarker::showEff", BattleMarker::showEff);

uint32_t BattleMarker::hide(BattleMarker *self) {
    uint32_t caption = self->caption;
    vcall(caption, node::setVisible, {caption, 0});
    uint32_t target = self->ringOnly ? uint32_t(self->ring) : rw::guest_addr(self);
    return vcall(target, node::setVisible, {target, 0});
}
RW_FUNCTION(0x5D5D3C, "BattleMarker::hide", BattleMarker::hide);

void BattleMarker::setHP(BattleMarker *self, uint32_t hp) {
    uint32_t indicator = self->hpIndicator;
    if (indicator) call(RaidBossHPIndicator_setHP, {indicator, hp});
}
RW_FUNCTION(0x5D5D78, "BattleMarker::setHP", BattleMarker::setHP);

BattleMarker *BattleMarker::destroy(BattleMarker *self) {
    uint32_t a = rw::guest_addr(self);
    self->vtable = VTABLE;
    functionDestroy(a + 656);                 // ~std::function onTap
    call(Node_dtor, {a});
    return self;
}
RW_FUNCTION(0x5D5D8C, "BattleMarker::~BattleMarker", BattleMarker::destroy);

void BattleMarker::destroyDelete(BattleMarker *self) {
    destroy(self);
    call(fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x5D5DD8, "BattleMarker::~BattleMarker (deleting)", BattleMarker::destroyDelete);

// ------------------------------------------------------------------------------------------- lambdas
namespace battlemarker {
BattleMarker *marker(uint32_t self) { return rw::host_ptr<BattleMarker>(ld32(self + 4)); }

// show_selected()::$_4 and the press callback of $_0: the selected ring
void showSelectedRing(uint32_t self) {
    BattleMarker *m = marker(self);
    playRing(m->ring, ringIndex(m, RING_SELECTED));
}

// $_0 onTouchBegan (bool(Touch*, Event*)): only while visible, with input enabled and the battle idle; a touch
// inside the marker presses it (pressed ring, then the selected ring).
uint32_t touchBegan(uint32_t self, const uint32_t *touchRef, uint32_t) {
    uint32_t touch = *touchRef;
    BattleMarker *m = marker(self);
    uint32_t a = rw::guest_addr(m);
    if (vcall(a, node::isVisible, {a}) != 1) return 0;
    uint32_t scene = m->scene;
    if (!ld8(scene + BattleMarker::SCENE_INPUT_ENABLED)) return 0;
    if (int32_t(ld32(ld32(scene + BattleMarker::SCENE_BATTLE) + 0x39C)) >= 0) return 0;
    Frame f;
    uint32_t pos = f.alloc(8);
    touchPoint(pos, a, touch, Touch_getLocation);
    if (!hit(a, pos)) {
        m->pressed = 0;
        return 0;
    }
    m->pressed = 1;
    playRing(m->ring, ringIndex(m, RING_PRESSED));
    afterPress(m, VT_PressDone);
    return 1;
}

// $_1 onTouchMoved: a pressed touch that moves from outside the marker into it shows the idle ring
void touchMoved(uint32_t self, const uint32_t *touchRef, uint32_t) {
    BattleMarker *m = marker(self);
    if (!m->pressed) return;
    uint32_t touch = *touchRef;
    if (!ld8(m->scene + BattleMarker::SCENE_INPUT_ENABLED)) return;
    uint32_t a = rw::guest_addr(m);
    Frame f;
    uint32_t previous = f.alloc(8);
    uint32_t current = f.alloc(8);
    touchPoint(previous, a, touch, Touch_getPreviousLocation);
    touchPoint(current, a, touch, Touch_getLocation);
    if (hit(a, previous)) return;
    if (hit(a, current)) BattleMarker::show_wait(m);
}

// $_2 onTouchEnded: released inside the marker -> onTap(marker)
void touchEnded(uint32_t self, const uint32_t *touchRef, uint32_t) {
    BattleMarker *m = marker(self);
    if (!m->pressed) return;
    uint32_t touch = *touchRef;
    m->pressed = 0;
    uint32_t a = rw::guest_addr(m);
    Frame f;
    uint32_t pos = f.alloc(8);
    touchPoint(pos, a, touch, Touch_getLocation);
    if (hit(a, pos) && rw::get<uint32_t>(m, 672) != 0) call(MarkerFunction_call, {a + 656, a});
}

// $_3 onTouchCancelled
void touchCancelled(uint32_t self, uint32_t, uint32_t) {
    BattleMarker *m = marker(self);
    if (m->pressed) m->pressed = 0;
}

constexpr uint32_t NAME_ShowSelected = 0xE4BCF0, TI_ShowSelected = 0xF913E0;
constexpr uint32_t NAME_TouchCancelled = 0xE4BDB0, TI_TouchCancelled = 0xF913F4;
constexpr uint32_t NAME_TouchEnded = 0xE4BE80, TI_TouchEnded = 0xF91408;
constexpr uint32_t NAME_TouchMoved = 0xE4BF50, TI_TouchMoved = 0xF9141C;
constexpr uint32_t NAME_TouchBegan = 0xE4C020, TI_TouchBegan = 0xF91430;
constexpr uint32_t NAME_PressDone = 0xE4C100, TI_PressDone = 0xF91444;
}  // namespace battlemarker

// show_selected()::$_4
RW_FUNCTION(0x5D5E28, "BattleMarker::show_selected::$_4::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D5E30, "BattleMarker::show_selected::$_4::__clone", thunk::clone1<VT_ShowSelected>);
RW_FUNCTION(0x5D5E5C, "BattleMarker::show_selected::$_4::__clone(dst)", thunk::cloneTo1<VT_ShowSelected>);
RW_FUNCTION(0x5D5E7C, "BattleMarker::show_selected::$_4::destroy", thunk::nop);
RW_FUNCTION(0x5D5E7E, "BattleMarker::show_selected::$_4::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D5E86, "BattleMarker::show_selected::$_4::operator()", battlemarker::showSelectedRing);
RW_FUNCTION(0x5D5EB8, "BattleMarker::show_selected::$_4::target", thunk::target<battlemarker::NAME_ShowSelected>);
RW_FUNCTION(0x5D5ED8, "BattleMarker::show_selected::$_4::target_type",
            thunk::targetType<battlemarker::TI_ShowSelected>);

// BattleMarker(...)::$_3 (onTouchCancelled)
RW_FUNCTION(0x5D5EEC, "BattleMarker::$_3::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D5EF4, "BattleMarker::$_3::__clone", thunk::clone1<VT_TouchCancelled>);
RW_FUNCTION(0x5D5F20, "BattleMarker::$_3::__clone(dst)", thunk::cloneTo1<VT_TouchCancelled>);
RW_FUNCTION(0x5D5F40, "BattleMarker::$_3::destroy", thunk::nop);
RW_FUNCTION(0x5D5F42, "BattleMarker::$_3::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D5F4A, "BattleMarker::$_3::operator()", battlemarker::touchCancelled);
RW_FUNCTION(0x5D5F60, "BattleMarker::$_3::target", thunk::target<battlemarker::NAME_TouchCancelled>);
RW_FUNCTION(0x5D5F80, "BattleMarker::$_3::target_type", thunk::targetType<battlemarker::TI_TouchCancelled>);

// BattleMarker(...)::$_2 (onTouchEnded)
RW_FUNCTION(0x5D5F94, "BattleMarker::$_2::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D5F9C, "BattleMarker::$_2::__clone", thunk::clone1<VT_TouchEnded>);
RW_FUNCTION(0x5D5FC8, "BattleMarker::$_2::__clone(dst)", thunk::cloneTo1<VT_TouchEnded>);
RW_FUNCTION(0x5D5FE8, "BattleMarker::$_2::destroy", thunk::nop);
RW_FUNCTION(0x5D5FEA, "BattleMarker::$_2::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D5FF4, "BattleMarker::$_2::operator()", battlemarker::touchEnded);
RW_FUNCTION(0x5D60D4, "BattleMarker::$_2::target", thunk::target<battlemarker::NAME_TouchEnded>);
RW_FUNCTION(0x5D60F4, "BattleMarker::$_2::target_type", thunk::targetType<battlemarker::TI_TouchEnded>);

// BattleMarker(...)::$_1 (onTouchMoved)
RW_FUNCTION(0x5D6188, "BattleMarker::$_1::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D6190, "BattleMarker::$_1::__clone", thunk::clone1<VT_TouchMoved>);
RW_FUNCTION(0x5D61BC, "BattleMarker::$_1::__clone(dst)", thunk::cloneTo1<VT_TouchMoved>);
RW_FUNCTION(0x5D61DC, "BattleMarker::$_1::destroy", thunk::nop);
RW_FUNCTION(0x5D61DE, "BattleMarker::$_1::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D61E8, "BattleMarker::$_1::operator()", battlemarker::touchMoved);
RW_FUNCTION(0x5D6338, "BattleMarker::$_1::target", thunk::target<battlemarker::NAME_TouchMoved>);
RW_FUNCTION(0x5D6358, "BattleMarker::$_1::target_type", thunk::targetType<battlemarker::TI_TouchMoved>);

// BattleMarker(...)::$_0 (onTouchBegan)
RW_FUNCTION(0x5D636C, "BattleMarker::$_0::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D6374, "BattleMarker::$_0::__clone", thunk::clone1<VT_TouchBegan>);
RW_FUNCTION(0x5D63A0, "BattleMarker::$_0::__clone(dst)", thunk::cloneTo1<VT_TouchBegan>);
RW_FUNCTION(0x5D63C0, "BattleMarker::$_0::destroy", thunk::nop);
RW_FUNCTION(0x5D63C2, "BattleMarker::$_0::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D63CC, "BattleMarker::$_0::operator()", battlemarker::touchBegan);
RW_FUNCTION(0x5D658C, "BattleMarker::$_0::target", thunk::target<battlemarker::NAME_TouchBegan>);
RW_FUNCTION(0x5D65AC, "BattleMarker::$_0::target_type", thunk::targetType<battlemarker::TI_TouchBegan>);

// $_0::operator()::{lambda()#1} (the press finished)
RW_FUNCTION(0x5D65C0, "BattleMarker::$_0::press::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5D65C8, "BattleMarker::$_0::press::__clone", thunk::clone1<VT_PressDone>);
RW_FUNCTION(0x5D65F4, "BattleMarker::$_0::press::__clone(dst)", thunk::cloneTo1<VT_PressDone>);
RW_FUNCTION(0x5D6614, "BattleMarker::$_0::press::destroy", thunk::nop);
RW_FUNCTION(0x5D6616, "BattleMarker::$_0::press::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5D661E, "BattleMarker::$_0::press::operator()", battlemarker::showSelectedRing);
RW_FUNCTION(0x5D6650, "BattleMarker::$_0::press::target", thunk::target<battlemarker::NAME_PressDone>);
RW_FUNCTION(0x5D6670, "BattleMarker::$_0::press::target_type", thunk::targetType<battlemarker::TI_PressDone>);
