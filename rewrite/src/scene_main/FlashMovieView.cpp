// FlashMovieView: a full-screen view that plays one LWF movie (scaled to the game frame, letterboxed with black bars
// when asked) and swallows touches while it is shown. create, init, the destructor, gotoLastFrame, gotoTargetFrame.
// The std::function slots of its lambdas are in FlashMovieView_thunks.cpp.
#include "../w3_6/Common.hpp"

namespace {
using namespace w36;

// layout (cocos2d::Node, then:)
constexpr uint32_t SIZE = 0x290;
constexpr uint32_t LWF_NODE = 0x270;          // cocos2d::LWFNode* the movie (retained)
constexpr uint32_t FLAG_274 = 0x274;          // uint8, cleared by create
constexpr uint32_t CALLBACK = 0x278;          // std::function (callable pointer at +0x288)

// GOT slots (the vtable and the library objects are reached through the GOT)
constexpr uint32_t GOT_VTABLE = 0xFD6668;               // &vtable for FlashMovieView (+8 = the vtable)
constexpr uint32_t GOT_COLOR3B_BLACK = 0xFD60CC;        // &cocos2d::Color3B::BLACK
constexpr uint32_t GOT_ANCHOR_TOP_LEFT = 0xFD666C;      // &cocos2d::Vec2::ANCHOR_TOP_LEFT
constexpr uint32_t GOT_ANCHOR_BOTTOM_LEFT = 0xFD6080;   // &cocos2d::Vec2::ANCHOR_BOTTOM_LEFT
constexpr uint32_t GOT_TI_LWFNODE = 0xFD6134;           // &typeinfo for cocos2d::LWFNode
constexpr uint32_t GOT_TI_AUDIOLWFNODE = 0xFD6670;      // &typeinfo for AudioLWFNode

constexpr uint32_t S_ROOT = 0xDFE0ED;                   // "_root"
constexpr uint32_t VT_ENTER_FRAME = 0xF59280;           // init::$_0 [this]: _root enterFrame
constexpr uint32_t VT_TOUCH_BEGAN = 0xF59254;           // init::$_1 []: onTouchBegan
constexpr uint32_t VT_TOUCH_ENDED = 0xF59228;           // init::$_2 [this]: onTouchEnded

constexpr uint32_t operator_new = 0xC98EF1;
constexpr uint32_t aeabi_memset = 0x40E7A0;             // __aeabi_memset(dst, n, c)
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;                // cocos2d::Node::~Node() (returns this)
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t AppGetGrameFrameWidth = 0x799B31;    // float
constexpr uint32_t AppGetGrameFrameHeight = 0x799D21;   // float
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                // (Size*, w, h)
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;                // (Rect*, x, y, w, h)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;                // (Vec2*, x, y)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;                // also called on the Rect temporaries
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;  // (Size* out, Director*)
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__v;            // Sprite::create()
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t TOUCH_ON_BEGAN = 0x50;               // std::function<bool(Touch*, Event*)>
constexpr uint32_t TOUCH_ON_ENDED = 0x80;               // std::function<void(Touch*, Event*)>
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t DynamicCast = 0xC9A171;              // __dynamic_cast(src, srcType, dstType, hint)
constexpr uint32_t AudioLWFNode_stopAllSEs = 0x5EBB01;

// cocos2d::Sprite slot
constexpr uint32_t V_setTextureRect = 0x284;            // setTextureRect(const Rect&)

uint32_t vtable() { return ld32(GOT_VTABLE) + 8; }
uint32_t rootMovie(uint32_t lwfNode) { return ld32(ld32(lwfNode + wfn::LWFNODE_LWF) + wfn::LWF_ROOT_MOVIE); }

// a black-bar sprite (Sprite::create + setTextureRect) of the visible width and the bar height
uint32_t newBar(float width, float height) {
    const uint32_t sprite = call(Sprite_create, {});
    Frame f;
    const uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, 0, 0, fbits(width), fbits(height)});
    vcall(sprite, V_setTextureRect, {sprite, rect});
    call(Vec2_dtor, {rect});
    return sprite;
}

// Plays `frame` from the start of the root movie; an AudioLWFNode also stops its sound effects.
void gotoFrame(uint32_t self, uint32_t frame) {
    if (!ld32(self + LWF_NODE)) return;
    call(wfn::Movie_GotoAndPlay, {rootMovie(ld32(self + LWF_NODE)), frame});
    const uint32_t node = ld32(self + LWF_NODE);
    if (!node) return;
    const uint32_t audio = call(DynamicCast, {node, ld32(GOT_TI_LWFNODE), ld32(GOT_TI_AUDIOLWFNODE), 0});
    if (audio) call(AudioLWFNode_stopAllSEs, {audio});
}

}  // namespace

namespace FlashMovieView {

uint32_t init(uint32_t self, uint32_t lwfNode, uint32_t letterbox) {
    st32(self + LWF_NODE, lwfNode);
    call(Ref_retain, {lwfNode});
    {
        const uint32_t w = call(AppGetGrameFrameWidth, {});
        const uint32_t h = call(AppGetGrameFrameHeight, {});
        Frame f;
        const uint32_t size = f.alloc(8);
        call(Size_ctor, {size, w, h});
        vcall(self, node::setContentSize, {self, size});
    }
    vcall(self, node::addChild, {self, lwfNode});
    addMovieHandler(ld32(lwfNode + wfn::LWFNODE_LWF), S_ROOT, 5, "enterFrame", VT_ENTER_FRAME, {self});

    // scale the 960x540 movie to the frame; the bar is the half of the height it leaves free
    float visW, visH;
    {
        Frame f;
        const uint32_t vis = f.alloc(8);
        call(Director_getVisibleSize, {vis, call(Director_getInstance, {})});
        visW = ldf(vis);
        visH = ldf(vis + 4);
    }
    float scale = bitsf(call(AppGetGrameFrameWidth, {})) / 960.0f;
    const float bar = std::ceil((visH + scale * -540.0f) * 0.5f);
    if (bar == 1.0f) {
        scale = bitsf(call(AppGetGrameFrameHeight, {})) / 540.0f;
    } else if (bar > 0.0f && letterbox == 1) {
        const uint32_t top = newBar(visW, bar);
        {
            Frame f;
            const uint32_t pos = f.alloc(8);
            call(Vec2_ctor, {pos, 0, fbits(visH)});
            vcall(top, node::setPosition, {top, pos});
            call(Vec2_dtor, {pos});
        }
        vcall(top, node::setColor, {top, ld32(GOT_COLOR3B_BLACK)});
        vcall(top, node::setAnchorPoint, {top, ld32(GOT_ANCHOR_TOP_LEFT)});
        vcall(self, node::addChild, {self, top});
        const uint32_t bottom = newBar(visW, bar);
        vcall(bottom, node::setColor, {bottom, ld32(GOT_COLOR3B_BLACK)});
        vcall(bottom, node::setAnchorPoint, {bottom, ld32(GOT_ANCHOR_BOTTOM_LEFT)});
        vcall(self, node::addChild, {self, bottom});
    }
    vcall(lwfNode, node::setPositionY, {lwfNode, fbits(bitsf(call(AppGetGrameFrameHeight, {})) - bar)});
    vcall(lwfNode, node::setScale, {lwfNode, fbits(scale)});

    // swallow every touch while shown
    const uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    withLambda(VT_TOUCH_BEGAN, {}, [&](uint32_t fo) { call(FunctionTouchBool_swap, {fo, listener + TOUCH_ON_BEGAN}); });
    withLambda(VT_TOUCH_ENDED, {self},
               [&](uint32_t fo) { call(FunctionTouchVoid_swap, {fo, listener + TOUCH_ON_ENDED}); });
    const uint32_t dispatcher = ld32(call(Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self});
    return 1;
}

uint32_t create(uint32_t lwfNode, uint32_t letterbox) {
    const uint32_t self = call(operator_new, {SIZE});
    call(aeabi_memset, {self, SIZE, 0});
    call(Node_ctor, {self});
    st32(self, vtable());
    st32(self + LWF_NODE, 0);
    st8(self + FLAG_274, 0);
    st32(self + CALLBACK + 16, 0);
    init(self, lwfNode, letterbox);
    call(Ref_autorelease, {self});
    return self;
}

uint32_t destruct(uint32_t self) {
    st32(self, vtable());
    if (const uint32_t node = ld32(self + LWF_NODE)) call(Ref_release, {node});
    functionDestroy(self + CALLBACK);
    call(Node_dtor, {self});
    return self;
}

void gotoLastFrame(uint32_t self) {
    const uint32_t node = ld32(self + LWF_NODE);
    if (!node) return;
    gotoFrame(self, ld32(rootMovie(node) + wfn::MOVIE_TOTAL_FRAMES));
}

void gotoTargetFrame(uint32_t self, uint32_t frame) { gotoFrame(self, frame); }

}  // namespace FlashMovieView

RW_FUNCTION(0x5682A0, "FlashMovieView::create", FlashMovieView::create);
RW_FUNCTION(0x56832C, "FlashMovieView::init", FlashMovieView::init);
RW_FUNCTION(0x5687D0, "FlashMovieView::~FlashMovieView", FlashMovieView::destruct);
RW_FUNCTION(0x568870, "FlashMovieView::gotoLastFrame", FlashMovieView::gotoLastFrame);
RW_FUNCTION(0x5688C0, "FlashMovieView::gotoTargetFrame", FlashMovieView::gotoTargetFrame);
