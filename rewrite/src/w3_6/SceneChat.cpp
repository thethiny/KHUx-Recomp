// SceneChat: the chat scene shell (a Layer showing the header over the deck background; the chat views are separate
// classes). Its std::function thunks are in scene_social/thunks/SceneChat_thunks.cpp.
#include "Common.hpp"

using namespace w36;

namespace {
constexpr uint32_t SIZE = 0x290;
constexpr uint32_t VTABLE = 0xF9B4A0;                 // _ZTV9SceneChat + 8
constexpr uint32_t ON_ENTER_CALLBACK = 0x210;         // cocos2d::Node::_onEnterCallback (std::function<void()>)
constexpr uint32_t ON_EXIT_CALLBACK = 0x228;          // cocos2d::Node::_onExitCallback

constexpr uint32_t VT_ON_ENTER = 0xF63FC8;            // init $_0 [this]
constexpr uint32_t VT_ON_EXIT = 0xF63F9C;             // init $_1 [this]
constexpr uint32_t VT_RUN_OUT_DONE = 0xF63F70;        // runOutAnimation lambda [std::function<void()> callback]

constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;   // (mgr, BackGroundType, const std::string&)
constexpr uint32_t SceneUtil_createLeftOutAnimation = 0x45B735; // (node, float, float, std::function<void()>)
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t S_DECK_BG = 0xE03AF2;              // "img/ui/Equip_DeckBG_01.png"
}  // namespace

struct SceneChat {
    static uint32_t createScene();
    static uint32_t create();
    static uint32_t destruct(uint32_t self);
    static void destructDelete(uint32_t self);
    static uint32_t init(uint32_t self);
    static void update(uint32_t self, float dt);
    static void runInAnimation(uint32_t self);
    static void runOutAnimation(uint32_t self, uint32_t callback);
};

uint32_t SceneChat::createScene() {
    uint32_t scene = call(sfn::Scene_create, {});
    uint32_t layer = create();
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x67D25C, "SceneChat::createScene", SceneChat::createScene);

uint32_t SceneChat::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    call(sfn::Layer_ctor, {self});
    st32(self, VTABLE);
    if (init(self) == 1) {
        call(sfn::Ref_autorelease, {self});
        return self;
    }
    vcall(self, slot::release, {self});
    return 0;
}
RW_FUNCTION(0x67D280, "SceneChat::create", SceneChat::create);

uint32_t SceneChat::destruct(uint32_t self) {
    st32(self, VTABLE);
    return call(sfn::Layer_dtor, {self});
}
RW_FUNCTION(0x67D2E4, "SceneChat::~SceneChat", SceneChat::destruct);

void SceneChat::destructDelete(uint32_t self) {
    st32(self, VTABLE);
    call(fn::operator_delete, {call(sfn::Layer_dtor, {self})});
}
RW_FUNCTION(0x67D304, "SceneChat::~SceneChat (deleting)", SceneChat::destructDelete);

uint32_t SceneChat::init(uint32_t self) {
    if (call(sfn::Layer_init, {self}) != 1) return 0;
    withLambda(VT_ON_ENTER, {self}, [&](uint32_t fo) { call(sfn::function_vv_assign, {self + ON_ENTER_CALLBACK, fo}); });
    withLambda(VT_ON_EXIT, {self}, [&](uint32_t fo) { call(sfn::function_vv_assign, {self + ON_EXIT_CALLBACK, fo}); });
    call(SceneManager_showHeader, {call(sfn::SceneManager_getInstance, {})});
    uint32_t mgr = call(sfn::SceneManager_getInstance, {});
    withStr(S_DECK_BG, 0x1A, [&](uint32_t s) { call(SceneManager_changeBackground, {mgr, 0, s}); });
    return 1;
}
RW_FUNCTION(0x67D328, "SceneChat::init", SceneChat::init);

void SceneChat::update(uint32_t, float) {}
RW_FUNCTION(0x67D488, "SceneChat::update", SceneChat::update);

void SceneChat::runInAnimation(uint32_t) {}
RW_FUNCTION(0x67D48A, "SceneChat::runInAnimation", SceneChat::runInAnimation);

// Slide the scene out to the left, then run `callback` (moved into the CallFunc lambda).
void SceneChat::runOutAnimation(uint32_t self, uint32_t callback) {
    Frame f;
    uint32_t copy = f.alloc(24);
    call(fn::function_vv_copy, {copy, callback});
    uint32_t done = f.alloc(24);
    heapFunction(done, VT_RUN_OUT_DONE, 0x20, 8, fn::function_vv_move, copy);
    uint32_t action = call(SceneUtil_createLeftOutAnimation, {self, 0, 0, done});
    call(fn::Node_runAction, {self, call(EaseSineOut_create, {action})});
    functionDestroy(done);
    functionDestroy(copy);
}
RW_FUNCTION(0x67D48C, "SceneChat::runOutAnimation", SceneChat::runOutAnimation);
