#include "SphereBlock.hpp"

namespace {
namespace fn {
constexpr uint32_t NODE_CTOR = rwlib::cocos2d::Node::ctor_base;                 // cocos2d::Node::Node()
constexpr uint32_t NODE_DTOR = rwlib::cocos2d::Node::dtor_base;                 // cocos2d::Node::~Node()
constexpr uint32_t NODE_INIT = rwlib::cocos2d::Node::init;                 // cocos2d::Node::init()
constexpr uint32_t NODE_STOP_ALL_ACTIONS = rwlib::cocos2d::Node::stopAllActions;     // cocos2d::Node::stopAllActions()
constexpr uint32_t NODE_RUN_ACTION = rwlib::cocos2d::Node::runAction;           // cocos2d::Node::runAction(Action*)
constexpr uint32_t NODE_REMOVE_ALL_CHILDREN = rwlib::cocos2d::Node::removeAllChildrenWithCleanup;  // cocos2d::Node::removeAllChildrenWithCleanup(bool)
constexpr uint32_t REF_AUTORELEASE = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t REF_RETAIN = rwlib::cocos2d::Ref::retain;
constexpr uint32_t REF_RELEASE = rwlib::cocos2d::Ref::release;
constexpr uint32_t MEMSET = 0x40E7A0;                    // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t STRLEN = 0x40E08C;                    // strlen (import)
constexpr uint32_t MODSI3 = 0xCD6009;                    // __modsi3
constexpr uint32_t STRING_INIT = 0x47CD93;               // std::string::__init(const char*, size_t)
constexpr uint32_t FADE_OUT_CREATE = rwlib::cocos2d::FadeOut::create;           // cocos2d::FadeOut::create(float)
constexpr uint32_t HIDE_CREATE = rwlib::cocos2d::Hide::create;               // cocos2d::Hide::create()
constexpr uint32_t SEQUENCE_CREATE = rwlib::cocos2d::Sequence::create;           // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t ARMATURE_CREATE = rwlib::cocostudio::Armature::create;           // cocostudio::Armature::create(const std::string&)
constexpr uint32_t GUIREADER_GET_INSTANCE = rwlib::cocostudio::GUIReader::getInstance;    // cocostudio::GUIReader::getInstance()
constexpr uint32_t GUIREADER_WIDGET_FROM_JSON = rwlib::cocostudio::GUIReader::widgetFromJsonFile;  // GUIReader::widgetFromJsonFile(const char*)
constexpr uint32_t VEC2_CTOR = rwlib::cocos2d::Vec2::ctor_base__ff;                 // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t VEC2_DTOR = rwlib::cocos2d::Vec2::dtor_base;                 // cocos2d::Vec2::~Vec2()
constexpr uint32_t SEARCH_NODE_BY_NAME = 0x45A9F5;       // SceneUtil::searchNodeByName(Node*, const std::string&)
constexpr uint32_t SET_SHADOW = 0x45BD15;                // SceneUtil::setShadow(ui::Text*)
constexpr uint32_t TEXT_ENABLE_OUTLINE = rwlib::cocos2d::ui::Text::enableOutline;       // ui::Text::enableOutline(const Color4B&, int)
constexpr uint32_t TEXT_SET_STRING = rwlib::cocos2d::ui::Text::setString;           // ui::Text::setString(const std::string&)
constexpr uint32_t TEXTDATA_GET = 0x4928AD;              // TextData::get(out shared_ptr, Type, int id)
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;         // StringBuffer::StringBuffer(const char* fmt, ...)
constexpr uint32_t DIRECTOR_GET_INSTANCE = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t TOUCH_LISTENER_CREATE = rwlib::cocos2d::EventListenerTouchOneByOne::create;     // EventListenerTouchOneByOne::create()
constexpr uint32_t TOUCH_LISTENER_SET_SWALLOW = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;  // EventListenerTouchOneByOne::setSwallowTouches(bool)
constexpr uint32_t FUNCTION_TOUCH_BOOL_SWAP = 0x483E4D;  // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FUNCTION_TOUCH_VOID_SWAP = 0x4F3699;  // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t ADD_LISTENER_SCENE_GRAPH = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;  // EventDispatcher::addEventListenerWithSceneGraphPriority
}  // namespace fn

// cocos2d::Node virtual slots
constexpr uint32_t SLOT_DELETE = 1;          // deleting destructor
constexpr uint32_t SLOT_SET_POSITION = 19;   // setPosition(const Vec2&)
constexpr uint32_t SLOT_SET_VISIBLE = 44;    // setVisible(bool)
constexpr uint32_t SLOT_IS_VISIBLE = 45;     // isVisible()
constexpr uint32_t SLOT_ADD_CHILD = 60;      // addChild(Node*)
constexpr uint32_t SLOT_SET_OPACITY = 133;   // setOpacity(GLubyte)
constexpr uint32_t ARMATURE_GET_ANIMATION_SLOT = 166;   // cocostudio::Armature::getAnimation()
constexpr uint32_t ANIMATION_PLAY_WITH_INDEX_SLOT = 28; // ArmatureAnimation::playWithIndex(index, durationTo, loop)

// armature names per cell id % 1000 (char* tables in .data), one table per kind
constexpr uint32_t ARMATURE_NAMES_KIND0 = 0xF6EC9C;
constexpr uint32_t ARMATURE_NAMES_KIND2 = 0xF6ED14;
constexpr uint32_t ARMATURE_NAMES_KIND3 = 0xF6ECFC;
constexpr uint32_t INCENTIVE_TEXT_IDS = 0xEAC248;        // int[]: TextData id of the reward name per cell id

constexpr uint32_t STR_SPHERE_ANIMATION = 0xE09C25;      // "SphereAnimation"
constexpr uint32_t STR_INCENTIVE_JSON = 0xE09C35;        // "cocostudio/publish/Sphere_Incentive.json"
constexpr uint32_t STR_GET = 0xE09C5E;                   // "Get"
constexpr uint32_t STR_TXT_INCENTIVE_LABEL = 0xE05694;   // "Txt_Incentive_Label"
constexpr uint32_t STR_TXT_PRICE_LABEL = 0xDFFA66;       // "Txt_Price_Label"
constexpr uint32_t STR_FMT_S = 0xDF8821;                 // "%s"
constexpr uint32_t STR_FMT_D = 0xDF86DE;                 // "%d"
constexpr uint32_t COLOR4B_BLUE_GOT = 0xFD7074;          // GOT slot: &cocos2d::Color4B::BLUE

// the touch listener callbacks (std::function __func vtables): began, moved, ended (captures the block), cancelled
constexpr uint32_t ON_TOUCH_BEGAN_FUNC = 0xF6EDB8;
constexpr uint32_t ON_TOUCH_MOVED_FUNC = 0xF6ED8C;
constexpr uint32_t ON_TOUCH_ENDED_FUNC = 0xF6ED60;
constexpr uint32_t ON_TOUCH_CANCELLED_FUNC = 0xF6ED34;

using rw::guest_addr;
uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }

// std::string temporary at a frame address: construct from a C string of known length / free its heap buffer
void newString(uint32_t at, uint32_t chars, uint32_t length) { rw::call(fn::STRING_INIT, {at, chars, length}); }
void freeString(uint32_t at) {
    if (rw::load<uint8_t>(at) & 1) guest::deleteObject(ld32(at + 8));
}

// Install one callback (a captureless or one-capture lambda built in `buf`) into the listener's std::function at
// `slot` by swapping, then destroy what the temporary holds afterwards.
void installCallback(uint32_t swapFn, uint32_t buf, uint32_t vtable, const uint32_t *capture, uint32_t slot) {
    *rw::frame_ptr<uint32_t>(buf + 16) = buf;
    *rw::frame_ptr<uint32_t>(buf) = vtable;
    if (capture) *rw::frame_ptr<uint32_t>(buf + 4) = *capture;
    rw::call(swapFn, {buf, slot});
    uint32_t callable = ld32(buf + 16);
    if (callable == buf) rw::vcall(rw::host_ptr<uint8_t>(callable), 4, {callable});        // destroy
    else if (callable) rw::vcall(rw::host_ptr<uint8_t>(callable), 5, {callable});          // destroy_deallocate
}
}  // namespace

uint32_t SphereBlock::create(int32_t id, int32_t type, int32_t kind, int32_t price) {
    guest::Frame f(0xC);
    uint32_t p = guest::newObject(SIZE);
    rw::call(fn::NODE_CTOR, {p});
    SphereBlock *self = rw::host_ptr<SphereBlock>(p);
    self->vtable = VTABLE;
    self->flag_270 = 0;
    self->touchState = 0;
    self->flag_278 = 0;
    rw::call(fn::MEMSET, {guest_addr(&self->field_27C), 0x18, 0});   // __aeabi_memset(dst, n, c)
    if (init(self, id, type, kind, price)) {   // returns exactly 0 or 1
        rw::call(fn::REF_AUTORELEASE, {p});
        return p;
    }
    rw::vcall(self, SLOT_DELETE, {p});
    return 0;
}
RW_FUNCTION(0x7936E0, "SphereBlock::create(int, int, int, int)", SphereBlock::create);

bool SphereBlock::init(SphereBlock *self, int32_t id, int32_t /*type*/, int32_t kind, int32_t price) {
    guest::Frame f(0x8D4);
    const uint32_t me = guest_addr(self);
    const int32_t cell = int32_t(rw::call(fn::MODSI3, {uint32_t(id), 1000}));
    if (rw::call(fn::NODE_INIT, {me}) != 1) return false;

    // the cell's armature (kind 1 and unknown kinds keep the null armature create() left)
    auto createArmature = [&](uint32_t strAt, uint32_t name, uint32_t length) {
        newString(strAt, name, length);
        self->armature = rw::call(fn::ARMATURE_CREATE, {strAt});
        freeString(strAt);
    };
    auto fromTable = [&](uint32_t strAt, uint32_t table) {
        uint32_t name = ld32(table + uint32_t(cell) * 4);
        createArmature(strAt, name, rw::call(fn::STRLEN, {name}));
    };
    switch (uint32_t(kind)) {
    case 0: fromTable(f.addr(0x9C), ARMATURE_NAMES_KIND0); break;
    case 2: fromTable(f.addr(0x90), ARMATURE_NAMES_KIND2); break;
    case 3: fromTable(f.addr(0x84), ARMATURE_NAMES_KIND3); break;
    case 4: createArmature(f.addr(0x78), STR_SPHERE_ANIMATION, 15); break;
    default: break;
    }
    rw::call(fn::REF_RETAIN, {self->armature});
    rw::vcall(self, SLOT_ADD_CHILD, {me, self->armature});

    if (kind == 0) {
        // the incentive panel: reward name and price, the "Get" mark hidden
        uint32_t reader = rw::call(fn::GUIREADER_GET_INSTANCE, {});
        uint32_t panel = rw::call(fn::GUIREADER_WIDGET_FROM_JSON, {reader, STR_INCENTIVE_JSON});
        const uint32_t pos = f.addr(0x70);
        rw::call(fn::VEC2_CTOR, {pos, rw::fbits(-50.0f), rw::fbits(-50.0f)});
        rw::vcall(rw::host_ptr<uint8_t>(panel), SLOT_SET_POSITION, {panel, pos});
        rw::call(fn::VEC2_DTOR, {pos});
        rw::vcall(self, SLOT_ADD_CHILD, {me, panel});

        const uint32_t name1 = f.addr(0x64), name2 = f.addr(0x58), name3 = f.addr(0x4C);
        newString(name1, STR_GET, 3);
        self->getMark = rw::call(fn::SEARCH_NODE_BY_NAME, {panel, name1});
        freeString(name1);
        newString(name2, STR_TXT_INCENTIVE_LABEL, 19);
        self->incentiveLabel = rw::call(fn::SEARCH_NODE_BY_NAME, {panel, name2});
        freeString(name2);
        rw::call(fn::SET_SHADOW, {self->incentiveLabel});
        newString(name3, STR_TXT_PRICE_LABEL, 15);
        self->priceLabel = rw::call(fn::SEARCH_NODE_BY_NAME, {panel, name3});
        freeString(name3);
        rw::call(fn::SET_SHADOW, {self->priceLabel});
        rw::vcall(rw::host_ptr<uint8_t>(self->getMark), SLOT_SET_VISIBLE, {self->getMark, 0});
        uint32_t blue = ld32(COLOR4B_BLUE_GOT);
        rw::call(fn::TEXT_ENABLE_OUTLINE, {self->incentiveLabel, blue, 2});
        rw::call(fn::TEXT_ENABLE_OUTLINE, {self->priceLabel, blue, 2});

        // reward name: TextData(type 0, INCENTIVE_TEXT_IDS[cell])
        uint32_t label = self->incentiveLabel;
        const uint32_t text = f.addr(0x38), sbuf = f.addr(0x4AC), str = f.addr(0x40);
        rw::call(fn::TEXTDATA_GET, {text, 0, ld32(INCENTIVE_TEXT_IDS + uint32_t(cell) * 4)});
        rw::call(fn::STRINGBUFFER_CTOR, {sbuf, STR_FMT_S, ld32(ld32(text) + 4)});
        newString(str, sbuf, rw::call(fn::STRLEN, {sbuf}));
        rw::call(fn::TEXT_SET_STRING, {label, str});
        freeString(str);
        uint32_t ctrl = ld32(text + 4);
        if (ctrl) guest::release(ctrl);

        // price
        label = self->priceLabel;
        const uint32_t sbuf2 = f.addr(0xA8), str2 = f.addr(0x2C);
        rw::call(fn::STRINGBUFFER_CTOR, {sbuf2, STR_FMT_D, uint32_t(price)});
        newString(str2, sbuf2, rw::call(fn::STRLEN, {sbuf2}));
        rw::call(fn::TEXT_SET_STRING, {label, str2});
        freeString(str2);

        // touch listener on the block
        uint32_t director = rw::call(fn::DIRECTOR_GET_INSTANCE, {});
        uint32_t dispatcher = ld32(director + 0x84);
        uint32_t listener = rw::call(fn::TOUCH_LISTENER_CREATE, {});
        rw::call(fn::TOUCH_LISTENER_SET_SWALLOW, {listener, 0});
        const uint32_t buf = f.addr(0x8B0);
        installCallback(fn::FUNCTION_TOUCH_BOOL_SWAP, buf, ON_TOUCH_BEGAN_FUNC, nullptr, listener + 0x50);
        installCallback(fn::FUNCTION_TOUCH_VOID_SWAP, buf, ON_TOUCH_MOVED_FUNC, nullptr, listener + 0x68);
        installCallback(fn::FUNCTION_TOUCH_VOID_SWAP, buf, ON_TOUCH_ENDED_FUNC, &me, listener + 0x80);
        installCallback(fn::FUNCTION_TOUCH_VOID_SWAP, buf, ON_TOUCH_CANCELLED_FUNC, nullptr, listener + 0x98);
        rw::call(fn::ADD_LISTENER_SCENE_GRAPH, {dispatcher, listener, self->armature});
    }
    self->touchState = 0;
    return true;
}
RW_FUNCTION(0x7983DC, "SphereBlock::init(int, int, int, int)", SphereBlock::init);

uint32_t SphereBlock::show(SphereBlock *self) {
    const uint32_t me = guest_addr(self);
    rw::call(fn::NODE_STOP_ALL_ACTIONS, {me});
    rw::vcall(self, SLOT_SET_OPACITY, {me, 255});
    return rw::vcall(self, SLOT_SET_VISIBLE, {me, 1});
}
RW_FUNCTION(0x793788, "SphereBlock::show", SphereBlock::show);

// Fade out over 0.2 s, then hide (only when visible).
uint32_t SphereBlock::hide(SphereBlock *self) {
    const uint32_t me = guest_addr(self);
    uint32_t visible = rw::vcall(self, SLOT_IS_VISIBLE, {me});
    if (visible != 1) return visible;
    rw::call(fn::NODE_STOP_ALL_ACTIONS, {me});
    uint32_t fade = rw::call(fn::FADE_OUT_CREATE, {rw::fbits(0.2f)});
    uint32_t hideAction = rw::call(fn::HIDE_CREATE, {});
    uint32_t sequence = rw::call(fn::SEQUENCE_CREATE, {fade, hideAction, 0});
    return rw::call(fn::NODE_RUN_ACTION, {me, sequence});
}
RW_FUNCTION(0x798ADC, "SphereBlock::hide", SphereBlock::hide);

// Play animation 0..2 of the armature (other indices: nothing; r0 stays this).
uint32_t SphereBlock::setAnimNo(SphereBlock *self, uint32_t index) {
    if (index > 2) return guest_addr(self);
    uint32_t armature = self->armature;
    uint32_t animation = rw::vcall(rw::host_ptr<uint8_t>(armature), ARMATURE_GET_ANIMATION_SLOT, {armature});
    return rw::vcall(rw::host_ptr<uint8_t>(animation), ANIMATION_PLAY_WITH_INDEX_SLOT,
                     {animation, index, 0xFFFFFFFFu, 0xFFFFFFFFu});
}
RW_FUNCTION(0x795CCC, "SphereBlock::SetAnimNo(int)", SphereBlock::setAnimNo);

SphereBlock *SphereBlock::destruct(SphereBlock *self) {
    const uint32_t me = guest_addr(self);
    self->vtable = VTABLE;
    rw::call(fn::NODE_REMOVE_ALL_CHILDREN, {me, 1});
    if (self->retained_284) rw::call(fn::REF_RELEASE, {self->retained_284});
    if (self->armature) rw::call(fn::REF_RELEASE, {self->armature});
    rw::call(fn::NODE_DTOR, {me});
    return self;
}
RW_FUNCTION(0x79836C, "SphereBlock::~SphereBlock", SphereBlock::destruct);

void SphereBlock::destructDelete(SphereBlock *self) { guest::deleteObject(guest_addr(destruct(self))); }
RW_FUNCTION(0x7983D0, "SphereBlock::~SphereBlock (deleting)", SphereBlock::destructDelete);
