// MarkFader: a full-screen black cocos2d::ClippingNode with a sprite-shaped hole (an inverted stencil) - the iris
// transition. fadeIn shrinks the hole from 4x to nothing, fadeOut grows it back to 4x; both run a callback at the
// end. While it is up it swallows every touch.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"
#include "../scene_main/FunctionThunks.hpp"

namespace {
using scene::call;
using scene::fbits;
using scene::ld32;
using scene::ldf;

constexpr uint32_t SIZE = 0x338;
constexpr uint32_t VTABLE = 0xF91A2C;              // _ZTV9MarkFader + 8
constexpr uint32_t STENCIL = 0x330;                // cocos2d::Sprite* (retained): the hole's shape

constexpr uint32_t ClippingNode_ctor = rwlib::cocos2d::ClippingNode::ctor_base;
constexpr uint32_t ClippingNode_dtor = rwlib::cocos2d::ClippingNode::dtor_base;
constexpr uint32_t ClippingNode_setStencil = rwlib::cocos2d::ClippingNode::setStencil;
constexpr uint32_t ClippingNode_setInverted = rwlib::cocos2d::ClippingNode::setInverted;
constexpr uint32_t ClippingNode_setAlphaThreshold = rwlib::cocos2d::ClippingNode::setAlphaThreshold;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__RKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE;          // Sprite::create(const string&)
constexpr uint32_t LayerColor_create = rwlib::cocos2d::LayerColor::create;      // LayerColor::create(const Color4B&)
constexpr uint32_t GOT_COLOR4B_BLACK = 0xFD65C8;      // GOT slot: &Color4B::BLACK
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;              // Vec2(x, y)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;  // (Size* out, Director*)
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
constexpr uint32_t DIRECTOR_ANIMATION_INTERVAL = 0xA8;  // double: seconds per frame
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t TouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t TouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t LISTENER_ON_BEGAN = 0x50;
constexpr uint32_t FunctionTouchBegan_swap = 0x483E4D;  // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVV_copy = 0x4683CD;          // std::function<void()>(const function&)
constexpr uint32_t FunctionVV_move = 0x47F905;          // std::function<void()>(function&&)
constexpr uint32_t FunctionVV_call = 0x470E69;          // std::function<void()>::operator()
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t ScaleTo_create = rwlib::cocos2d::ScaleTo::create__ff;           // (duration, scale)
constexpr uint32_t EaseSineIn_create = rwlib::cocos2d::EaseSineIn::create;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;          // (action, ..., nullptr)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;

// lambdas (__func vtables + 8)
constexpr uint32_t VT_INIT_TOUCH_BEGAN = 0xF5D8F8;   // init::$_0 {this}: swallow the touch
constexpr uint32_t VT_FADE_IN_STEP = 0xF5D8CC;       // fadeIn::$_1 {this}
constexpr uint32_t VT_FADE_IN_DONE = 0xF5D8A0;       // fadeIn::$_2 {std::function<void()> callback}
constexpr uint32_t VT_FADE_OUT_DONE = 0xF5D874;      // fadeOut::$_3 {std::function<void()> callback}
constexpr uint32_t DONE_SIZE = 0x20;                 // their __func objects (the function at +8)

constexpr uint32_t V_SET_SCALE = 0x40, V_SET_POSITION = 0x4C, V_SET_CONTENT_SIZE = 0xA8, V_GET_CONTENT_SIZE = 0xAC,
                   V_ADD_CHILD = 0xF0, V_SET_CASCADE_OPACITY = 0x220;

constexpr float FADE_TIME = 0.4f;

// CallFunc::create(lambda {callback}): the callback is copied, then moved into a heap __func
uint32_t callDone(uint32_t vtable, uint32_t callback) {
    scene::Frame f;
    uint32_t copy = f.alloc(20);
    call(FunctionVV_copy, {copy, callback});
    uint32_t fo = f.alloc(20);
    scene::st32(fo + 16, 0);
    uint32_t obj = call(scene::fn::operator_new, {DONE_SIZE});
    if (obj) {
        scene::st32(obj, vtable);
        call(FunctionVV_move, {obj + 8, copy});
    }
    scene::st32(fo + 16, obj);
    uint32_t action = call(CallFunc_create, {fo});
    scene::functionDestroy(fo);
    scene::functionDestroy(copy);
    return action;
}

}  // namespace

struct MarkFader {
    static uint32_t destruct(uint32_t self) {
        scene::st32(self, VTABLE);
        uint32_t stencil = ld32(self + STENCIL);
        if (stencil) call(Ref_release, {stencil});
        call(ClippingNode_dtor, {self});
        return self;
    }
    static void destructDelete(uint32_t self) { call(scene::fn::operator_delete, {destruct(self)}); }

    static uint32_t create(uint32_t spriteFile) {
        uint32_t self = call(scene::fn::operator_new, {SIZE});
        call(ClippingNode_ctor, {self});
        scene::st32(self, VTABLE);
        init(self, spriteFile);
        call(Ref_autorelease, {self});
        return self;
    }

    static uint32_t init(uint32_t self, uint32_t spriteFile) {
        uint32_t dispatcher = ld32(call(Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
        uint32_t listener = call(TouchOneByOne_create, {});
        call(TouchOneByOne_setSwallowTouches, {listener, 1});
        {
            scene::Frame f;
            uint32_t fo = scene::newFunction(f, VT_INIT_TOUCH_BEGAN, {self});
            call(FunctionTouchBegan_swap, {fo, listener + LISTENER_ON_BEGAN});
            scene::functionDestroy(fo);
        }
        call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self});
        scene::Frame f;
        uint32_t size = f.alloc(8);
        call(Director_getVisibleSize, {size, call(Director_getInstance, {})});
        scene::vcall(self, V_SET_CONTENT_SIZE, {self, size});
        scene::vcall(self, V_SET_CASCADE_OPACITY, {self, 1});
        uint32_t stencil = call(Sprite_create, {spriteFile});
        scene::st32(self + STENCIL, stencil);
        call(Ref_retain, {stencil});
        // the hole starts at the centre of the screen
        stencil = ld32(self + STENCIL);
        uint32_t setPosition = ld32(ld32(stencil) + V_SET_POSITION);
        float w = ldf(scene::vcall(self, V_GET_CONTENT_SIZE, {self}));
        uint32_t s2 = scene::vcall(self, V_GET_CONTENT_SIZE, {self});
        float x = w * 0.5f;
        float y = ldf(s2 + 4) * 0.5f;
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits(y)});
        call(setPosition, {stencil, pos});
        call(Vec2_dtor, {pos});
        call(ClippingNode_setStencil, {self, ld32(self + STENCIL)});
        call(ClippingNode_setInverted, {self, 1});
        call(ClippingNode_setAlphaThreshold, {self, fbits(0.01f)});
        uint32_t black = call(LayerColor_create, {ld32(GOT_COLOR4B_BLACK)});
        scene::vcall(self, V_ADD_CHILD, {self, black});
        return 1;
    }

    // close the iris: the hole shrinks from 4x to nothing, then (two frames later) the callback
    static void fadeIn(uint32_t self, uint32_t callback) {
        uint32_t director = call(Director_getInstance, {});
        float frame = float(rw::load<double>(director + DIRECTOR_ANIMATION_INTERVAL));
        uint32_t delay1 = call(DelayTime_create, {fbits(frame)});
        uint32_t delay2 = call(DelayTime_create, {fbits(frame)});
        uint32_t step;
        {
            scene::Frame f;
            uint32_t fo = scene::newFunction(f, VT_FADE_IN_STEP, {self});
            step = call(CallFunc_create, {fo});
            scene::functionDestroy(fo);
        }
        uint32_t done = callDone(VT_FADE_IN_DONE, callback);
        uint32_t stencil = ld32(self + STENCIL);
        scene::vcall(stencil, V_SET_SCALE, {stencil, fbits(4.0f)});
        uint32_t target = ld32(self + STENCIL);
        uint32_t scale = call(ScaleTo_create, {fbits(FADE_TIME), fbits(0.0f)});
        uint32_t ease = call(EaseSineOut_create, {scale});
        uint32_t seq = call(Sequence_create, {ease, delay1, step, delay2, done, 0});
        call(Node_runAction, {target, seq});
    }

    // open the iris: the hole grows to 4x, then the callback
    static void fadeOut(uint32_t self, uint32_t callback) {
        uint32_t done = callDone(VT_FADE_OUT_DONE, callback);
        call(ClippingNode_setStencil, {self, ld32(self + STENCIL)});
        uint32_t target = ld32(self + STENCIL);
        uint32_t scale = call(ScaleTo_create, {fbits(FADE_TIME), fbits(4.0f)});
        uint32_t ease = call(EaseSineIn_create, {scale});
        uint32_t seq = call(Sequence_create, {ease, done, 0});
        call(Node_runAction, {target, seq});
    }

    // fadeOut::$_3 / fadeIn::$_2 operator(): run the captured callback
    static void runCallback(uint32_t lambda) { call(FunctionVV_call, {lambda + 8}); }
};

namespace MarkFader_w3 {
using fadeOut_3 = thunk::LambdaFn<0xF5D874, 32, 0xF91CB8, 0xE4C7D0, 8, 8, 0x4683CD>;
using fadeIn_2 = thunk::LambdaFn<0xF5D8A0, 32, 0xF91CCC, 0xE4C870, 8, 8, 0x4683CD>;
}  // namespace MarkFader_w3

RW_FUNCTION(0x5D98D8, "MarkFader::~MarkFader", MarkFader::destruct);
RW_FUNCTION(0x5D9924, "MarkFader::~MarkFader (deleting)", MarkFader::destructDelete);
RW_FUNCTION(0x5D9930, "MarkFader::create(const std::string&)", MarkFader::create);
RW_FUNCTION(0x5D998C, "MarkFader::init(const std::string&)", MarkFader::init);
RW_FUNCTION(0x5D9B48, "MarkFader::fadeIn(std::function<void()>)", MarkFader::fadeIn);
RW_FUNCTION(0x5D9D04, "MarkFader::fadeOut(std::function<void()>)", MarkFader::fadeOut);

RW_FUNCTION(0x5D9E38, "MarkFader::fadeOut::$_3 ~__func()", MarkFader_w3::fadeOut_3::destructor);
RW_FUNCTION(0x5D9E74, "MarkFader::fadeOut::$_3 ~__func() (deleting)", MarkFader_w3::fadeOut_3::destructorDelete);
RW_FUNCTION(0x5D9EB4, "MarkFader::fadeOut::$_3 __clone()", MarkFader_w3::fadeOut_3::clone);
RW_FUNCTION(0x5D9F04, "MarkFader::fadeOut::$_3 __clone(__base*)", MarkFader_w3::fadeOut_3::cloneInto);
RW_FUNCTION(0x5D9F38, "MarkFader::fadeOut::$_3 destroy()", MarkFader_w3::fadeOut_3::destroy);
RW_FUNCTION(0x5D9F58, "MarkFader::fadeOut::$_3 destroy_deallocate()", MarkFader_w3::fadeOut_3::destroyDeallocate);
RW_FUNCTION(0x5D9F84, "MarkFader::fadeOut::$_3 operator()", MarkFader::runCallback);
RW_FUNCTION(0x5D9F90, "MarkFader::fadeOut::$_3 target(const type_info&)", MarkFader_w3::fadeOut_3::target);
RW_FUNCTION(0x5D9FB0, "MarkFader::fadeOut::$_3 target_type()", MarkFader_w3::fadeOut_3::targetType);
RW_FUNCTION(0x5D9FC4, "MarkFader::fadeIn::$_2 ~__func()", MarkFader_w3::fadeIn_2::destructor);
