// SceneFlashDrama: the story's LWF drama scenes (lwf/drama/<n>/<n>.lwf), scaled to the screen with black bars,
// a skip button (FlashSkipAnimation armature), and the next drama loaded as a new scene layer when one ends.
#include "scene/SceneCommon.hpp"

using namespace scene;

namespace {
// SceneFlashDrama : cocos2d::Layer (0x2C8 bytes)
constexpr uint32_t SIZE = 0x2C8;
constexpr uint32_t VT_SCENE_FLASH_DRAMA = 0xFA258C;
constexpr uint32_t ON_ENTER = 0x210, ON_EXIT = 0x228;   // std::function<void()> slots the layer runs on enter / exit
constexpr uint32_t D_DESIGN_WIDTH = 0x288, D_DESIGN_HEIGHT = 0x28C;   // 960 x 540
constexpr uint32_t D_DRAMA = 0x290;                     // the FlashDrama playing
constexpr uint32_t D_SCALE = 0x294;
constexpr uint32_t D_NUMBER = 0x298;                    // the drama number
constexpr uint32_t D_FLAG = 0x29C;                      // passed on to the next scene's layer (byte)
constexpr uint32_t D_SKIP_BUTTON = 0x2A0;               // cocostudio::Armature
constexpr uint32_t D_ON_SKIP = 0x2A8;                   // std::function<void()>: replaces next_scene() on skip
constexpr uint32_t D_FULL_SCREEN_SKIP = 0x2C0;          // byte: a tap anywhere skips
constexpr uint32_t FLASH_DRAMA_SIZE = 0x608;
constexpr uint32_t SKIP_BUTTON_TAG = 39208979;

constexpr uint32_t VT_TOUCH_BEGAN = 0xF69DAC;      // init::$_0: forwards taps to the drama
constexpr uint32_t VT_TOUCH_ENDED = 0xF69D80;      // init::$_1: the skip button
constexpr uint32_t VT_ON_ENTER = 0xF69D54;         // init::$_2: scheduleUpdate
constexpr uint32_t VT_ON_EXIT = 0xF69D28;          // init::$_3: unscheduleUpdate
constexpr uint32_t VT_NEXT_LAYER_UPDATE = 0xF69CFC;   // update::$_?: the next drama's layer factory
constexpr uint32_t VT_NEXT_LAYER_SKIP = 0xF69CD0;     // next_scene::$_?: the same, after a skip

constexpr uint32_t BGM_DRAMA = 509000040;
constexpr uint32_t S_DRAMA_PATH = 0xE06299;         // "lwf/drama/%d/%d.lwf"
constexpr uint32_t S_SKIP_EXPORT_JSON = 0xE05815;   // "cocostudio/publish/FlashSkipAnimation.ExportJson"
constexpr uint32_t S_SKIP_ARMATURE = 0xE05846;      // "FlashSkipAnimation"
constexpr uint32_t GOT_COLOR_BLACK = 0xFD60CC;            // &cocos2d::Color3B::BLACK
constexpr uint32_t GOT_ANCHOR_TOP_LEFT = 0xFD666C, GOT_ANCHOR_BOTTOM_LEFT = 0xFD6080;
constexpr float SKIP_BUTTON_X = 96.0f, SKIP_BUTTON_TOP = -32.0f;
constexpr float SKIP_TOUCH_GROW = 64.0f, SKIP_TOUCH_SHIFT = -32.0f;

constexpr uint32_t Scene_create = 0x8A5F37, Layer_ctor = 0x88A7D1, Layer_dtor = 0x88A88D, Layer_init = 0x88A8A1;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD, audio_Manager_load = 0x5BEEF5;
constexpr uint32_t SceneManager_getInstance = 0x60C26D, SceneManager_setEnabledTouchEffect = 0x60CF65;
constexpr uint32_t SceneManager_removeBackground = 0x611691, SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t function_touch_bool_swap = 0x483E4D;   // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t function_touch_void_swap = 0x4F3699;   // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t function_vv_assign = 0x4EB9CD;         // std::function<void()>::operator=(const function&)
constexpr uint32_t AppGetGrameFrameWidth = 0x799B31;
constexpr uint32_t CEILF = 0x40EFBC;                      // ceilf (import)
constexpr uint32_t Sprite_create = 0x8A64DD, Rect_ctor = 0x8DF18B, Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t StringBuffer_ctor = 0x49140D, FlashDrama_ctor = 0x6F2839;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Touch_getLocation = 0x900E57, Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t Sprite_setTextureRect = 0x284, Armature_getAnimation = 0x298, Node_getBoundingBox = 0x1A0;
constexpr uint32_t ArmatureAnimation_playWithIndex = 0x70;   // (index, durationTo = -1, loop = -1)

constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
constexpr uint32_t LISTENER_ON_TOUCH_BEGAN = 0x50, LISTENER_ON_TOUCH_ENDED = 0x80;
constexpr uint32_t DRAMA_MOVIE = 0x578, MOVIE_FRAME = 0x9C, MOVIE_END_FRAME = 0x98;

// a black bar sprite (width x height)
uint32_t newBar(float width, float height, Frame &f) {
    const uint32_t bar = call(Sprite_create, {});
    const uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, 0, 0, fbits(width), fbits(height)});
    vcall(bar, Sprite_setTextureRect, {bar, rect});
    call(Vec2_dtor, {rect});
    return bar;
}

// the next drama as a new scene layer (fade), via a layer factory capturing {scene, number, flag}
void changeToNextDrama(uint32_t self, uint32_t factoryVtable) {
    const uint32_t number = ld32(self + D_NUMBER) + 1;
    st32(self + D_NUMBER, number);
    const uint32_t flag = ld8(self + D_FLAG);
    const uint32_t manager = call(SceneManager_getInstance, {});
    Frame f;
    const uint32_t factory = newFunction(f, factoryVtable, {self, number, flag});
    call(SceneManager_changeSceneLayer, {manager, factory, 1, 1, 1});
    functionDestroy(factory);
}
}  // namespace

namespace flash_drama {

struct SceneFlashDrama {
    static uint32_t createScene() {
        const uint32_t scene = call(Scene_create, {});
        vcall(scene, node::addChild, {scene, create(1)});
        return scene;
    }

    static uint32_t create(uint32_t letterbox) {
        const uint32_t self = call(fn::operator_new, {SIZE});
        call(Layer_ctor, {self});
        st32(self, VT_SCENE_FLASH_DRAMA);
        stf(self + D_DESIGN_WIDTH, 960.0f);
        stf(self + D_DESIGN_HEIGHT, 540.0f);
        st32(self + D_DRAMA, 0);
        st32(self + D_SKIP_BUTTON, 0);
        st32(self + D_ON_SKIP + 16, 0);
        st8(self + D_FULL_SCREEN_SKIP, 0);
        if (init(self, letterbox & 0xFF) == 1) {
            call(Ref_autorelease, {self});
            return self;
        }
        vcall(self, 4, {self});   // release
        return 0;
    }

    static uint32_t init(uint32_t self, uint32_t letterbox) {
        if (call(Layer_init, {self}) != 1) return 0;
        call(audio_Manager_load, {call(audio_Manager_getInstance, {}), BGM_DRAMA});
        call(SceneManager_setEnabledTouchEffect, {call(SceneManager_getInstance, {}), 0});
        const uint32_t listener = call(EventListenerTouchOneByOne_create, {});
        call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
        Frame f;
        {
            const uint32_t began = newFunction(f, VT_TOUCH_BEGAN, {self});
            call(function_touch_bool_swap, {began, listener + LISTENER_ON_TOUCH_BEGAN});
            functionDestroy(began);
        }
        call(EventDispatcher_addEventListenerWithSceneGraphPriority,
             {ld32(director() + DIRECTOR_EVENT_DISPATCHER), listener, self});
        call(SceneManager_removeBackground, {call(SceneManager_getInstance, {})});
        {
            const uint32_t ended = newFunction(f, VT_TOUCH_ENDED, {self});
            call(function_touch_void_swap, {ended, listener + LISTENER_ON_TOUCH_ENDED});
            functionDestroy(ended);
        }

        // fit the 960 x 540 drama to the frame's width; what is left above and below gets black bars
        const uint32_t visible = f.alloc(8);
        call(fn::Director_getVisibleSize, {visible, director()});
        const float visibleWidth = ldf(visible), visibleHeight = ldf(visible + 4);
        const float scale = bitsf(call(AppGetGrameFrameWidth, {})) / ldf(self + D_DESIGN_WIDTH);
        stf(self + D_SCALE, scale);
        const float bar = bitsf(call(CEILF, {fbits((visibleHeight - scale * ldf(self + D_DESIGN_HEIGHT)) * 0.5f)}));
        if (bar == 1.0f) {   // a 1-pixel bar: fit the height instead
            stf(self + D_SCALE, bitsf(call(fn::AppGetGrameFrameHeight, {})) / ldf(self + D_DESIGN_HEIGHT));
        } else if (bar > 0.0f && letterbox == 1) {
            const uint32_t top = newBar(visibleWidth, bar, f);
            const uint32_t position = f.alloc(8);
            call(Vec2_ctor, {position, 0, fbits(visibleHeight)});
            vcall(top, node::setPosition, {top, position});
            call(Vec2_dtor, {position});
            vcall(top, node::setColor, {top, ld32(GOT_COLOR_BLACK)});
            vcall(top, node::setAnchorPoint, {top, ld32(GOT_ANCHOR_TOP_LEFT)});
            vcall(self, node::addChildZ, {self, top, 200});
            const uint32_t bottom = newBar(visibleWidth, bar, f);
            vcall(bottom, node::setColor, {bottom, ld32(GOT_COLOR_BLACK)});
            vcall(bottom, node::setAnchorPoint, {bottom, ld32(GOT_ANCHOR_BOTTOM_LEFT)});
            vcall(self, node::addChildZ, {self, bottom, 200});
        }

        // the skip button, top left (below the top bar when letterboxed)
        const uint32_t armatures = call(ArmatureDataManager_getInstance, {});
        const uint32_t exportJson = newStr(f, S_SKIP_EXPORT_JSON, 0x30);
        call(ArmatureDataManager_addArmatureFileInfo, {armatures, exportJson});
        strFree(exportJson);
        const uint32_t name = newStr(f, S_SKIP_ARMATURE, 0x12);
        st32(self + D_SKIP_BUTTON, call(Armature_create, {name}));
        strFree(name);
        const uint32_t skip = ld32(self + D_SKIP_BUTTON);
        const float top = bitsf(call(fn::AppGetGrameFrameHeight, {})) + SKIP_BUTTON_TOP;
        const uint32_t position = f.alloc(8);
        call(Vec2_ctor, {position, fbits(SKIP_BUTTON_X), fbits(top - (letterbox ? bar : 0.0f))});
        vcall(skip, node::setPosition, {skip, position});
        call(Vec2_dtor, {position});
        vcall(self, node::addChildZTag, {self, ld32(self + D_SKIP_BUTTON), 200, SKIP_BUTTON_TAG});

        const uint32_t onEnter = newFunction(f, VT_ON_ENTER, {self});
        call(function_vv_assign, {self + ON_ENTER, onEnter});
        functionDestroy(onEnter);
        const uint32_t onExit = newFunction(f, VT_ON_EXIT, {self});
        call(function_vv_assign, {self + ON_EXIT, onExit});
        functionDestroy(onExit);
        return 1;
    }

    // plays drama `number`, centred and scaled, the skip button animating
    static void loadLWF(uint32_t self, uint32_t number) {
        Frame f;
        const uint32_t origin = f.alloc(8), visible = f.alloc(8);
        call(fn::Director_getVisibleOrigin, {origin, director()});
        call(fn::Director_getVisibleSize, {visible, director()});
        const uint32_t drama = call(fn::operator_new, {FLASH_DRAMA_SIZE});
        const uint32_t path = f.alloc(0x404);
        call(StringBuffer_ctor, {path, S_DRAMA_PATH, number, number});
        call(FlashDrama_ctor, {drama, path});
        call(Ref_autorelease, {drama});
        vcall(drama, node::setScale, {drama, ld32(self + D_SCALE)});
        const float y = (ldf(origin + 4) + ldf(visible + 4) * 0.5f) +
                        ldf(self + D_SCALE) * ldf(self + D_DESIGN_HEIGHT) * 0.5f;
        const uint32_t position = f.alloc(8);
        call(Vec2_ctor, {position, ld32(origin), fbits(y)});
        vcall(drama, node::setPosition, {drama, position});
        call(Vec2_dtor, {position});
        st32(self + D_DRAMA, drama);
        vcall(self, node::addChildZ, {self, drama, 100});
        if (const uint32_t skip = ld32(self + D_SKIP_BUTTON)) {
            const uint32_t animation = vcall(skip, Armature_getAnimation, {skip});
            vcall(animation, ArmatureAnimation_playWithIndex, {animation, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
        }
        call(Vec2_dtor, {origin});
    }

    // a finished drama is removed and the next one follows
    static void update(uint32_t self, uint32_t) {
        const uint32_t drama = ld32(self + D_DRAMA);
        if (!drama) return;
        const uint32_t movie = ld32(drama + DRAMA_MOVIE);
        if (!movie || ld32(movie + MOVIE_FRAME) != ld32(movie + MOVIE_END_FRAME)) return;
        vcall(drama, node::removeFromParent, {drama});
        st32(self + D_DRAMA, 0);
        changeToNextDrama(self, VT_NEXT_LAYER_UPDATE);
    }

    // skipped: the drama goes, the next one follows
    static void nextScene(uint32_t self) {
        call(Node_unscheduleUpdate, {self});
        const uint32_t drama = ld32(self + D_DRAMA);
        vcall(drama, node::removeFromParent, {drama});
        st32(self + D_DRAMA, 0);
        changeToNextDrama(self, VT_NEXT_LAYER_SKIP);
    }

    static void destroyMembers(uint32_t self) {
        st32(self, VT_SCENE_FLASH_DRAMA);
        functionDestroy(self + D_ON_SKIP);
        call(Layer_dtor, {self});
    }
    static uint32_t destruct(uint32_t self) {
        destroyMembers(self);
        return self;
    }
    static void destructDelete(uint32_t self) {
        destroyMembers(self);
        call(fn::operator_delete, {self});
    }

    // init::$_1: touch ended - a tap on the skip button (its box grown by 32 on each side), or anywhere in
    // full-screen skip mode, skips
    static void onTouchEnded(uint32_t functor, uint32_t touch, uint32_t) {
        const uint32_t self = ld32(functor + 4);
        const uint32_t skip = self + D_SKIP_BUTTON;
        if (!ld8(self + D_FULL_SCREEN_SKIP)) {
            const uint32_t t = ld32(touch);
            Frame f;
            const uint32_t box = f.alloc(16), location = f.alloc(8);
            vcall(ld32(skip), Node_getBoundingBox, {box, ld32(skip)});
            stf(box + 12, ldf(box + 12) + SKIP_TOUCH_GROW);
            stf(box + 4, ldf(box + 4) + SKIP_TOUCH_SHIFT);
            stf(box + 8, ldf(box + 8) + SKIP_TOUCH_GROW);
            stf(box, ldf(box) + SKIP_TOUCH_SHIFT);
            call(Touch_getLocation, {location, t});
            const uint32_t hit = call(Rect_containsPoint, {box, location});
            call(Vec2_dtor, {location});
            call(Vec2_dtor, {box});
            if (!hit) return;
        }
        const uint32_t animation = vcall(ld32(skip), Armature_getAnimation, {ld32(skip)});
        vcall(animation, ArmatureAnimation_playWithIndex, {animation, 1, 0xFFFFFFFFu, 0xFFFFFFFFu});
        call(AudioPlaySystemSEDecide, {});
        if (ld32(self + D_ON_SKIP + 16))
            call(fn::function_vv_call, {self + D_ON_SKIP});
        else
            nextScene(self);
    }
};

}  // namespace flash_drama

using flash_drama::SceneFlashDrama;
RW_FUNCTION(0x6F33F6, "SceneFlashDrama::createScene", SceneFlashDrama::createScene);
RW_FUNCTION(0x6F341C, "SceneFlashDrama::create", SceneFlashDrama::create);
RW_FUNCTION(0x6F34BC, "SceneFlashDrama::init", SceneFlashDrama::init);
RW_FUNCTION(0x6F39B0, "SceneFlashDrama::loadLWF", SceneFlashDrama::loadLWF);
RW_FUNCTION(0x6F3B3C, "SceneFlashDrama::update", SceneFlashDrama::update);
RW_FUNCTION(0x6F3C44, "SceneFlashDrama::next_scene", SceneFlashDrama::nextScene);
RW_FUNCTION(0x6F3D40, "SceneFlashDrama::~SceneFlashDrama", SceneFlashDrama::destruct);
RW_FUNCTION(0x6F3D8C, "SceneFlashDrama::~SceneFlashDrama (deleting)", SceneFlashDrama::destructDelete);
RW_FUNCTION(0x6F40CC, "SceneFlashDrama::init::$_1 operator() (touch ended: skip)", SceneFlashDrama::onTouchEnded);
