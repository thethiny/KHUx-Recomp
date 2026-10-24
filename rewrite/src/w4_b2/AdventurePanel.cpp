// AdventurePanel (a quest-select panel: its widget and in / out animations) and SceneAdventureSelect's tutorial
// popups 33, 34, 104, 105.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace w4b2 {
using namespace scene;

constexpr uint32_t SEARCH_NODE = 0x45A9F5;       // SceneUtil::searchNodeByName(root, name)
constexpr uint32_t STOP_ALL_ACTIONS = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t WRAP_ACTION = rwlib::cocos2d::EaseSineOut::create;       // the eased wrapper the panel animations run
constexpr uint32_t FADEOUT_GOT = 0xFD65C0;       // &fade duration (float)
constexpr uint32_t DELAY_GOT = 0xFD6988;         // &delay (float)

// AdventurePanel: home position +0 (Vec2), widget +8
struct AdventurePanel {
    static uint32_t searchWidget(uint32_t self, uint32_t root, uint32_t name) {
        Frame f;
        uint32_t s = f.alloc(12);
        strInitC(s, name);
        st32(self + 8, call(SEARCH_NODE, {root, s}));
        strFree(s);
        uint32_t w = ld32(self + 8);
        uint32_t pos = vcall(w, 0x54, {w});  // getPosition
        st32(self, ld32(pos));
        st32(self + 4, ld32(pos + 4));
        return 0;
    }
    static uint32_t createWidget(uint32_t self, uint32_t parent, uint32_t file) {
        uint32_t reader = call(rwlib::cocostudio::GUIReader::getInstance, {});  // GUIReader::getInstance
        uint32_t w = call(rwlib::cocostudio::GUIReader::widgetFromJsonFile, {reader, file});  // widgetFromJsonFile
        st32(self + 8, w);
        vcall(parent, 0xF0, {parent, w});  // addChild
        w = ld32(self + 8);
        uint32_t pos = vcall(w, 0x54, {w});
        uint32_t x = ld32(pos), y = ld32(pos + 4);
        st32(self, x);
        st32(self + 4, y);
        return y;
    }
    // SceneUtil::create*OutAction(node, 0, 0, callback) wrapped and run on the widget
    static uint32_t runOut(uint32_t self, uint32_t callback, uint32_t create) {
        Frame f;
        uint32_t node = ld32(self + 8);
        if (node == 0) return 0;
        uint32_t cb = f.alloc(20);
        call(fn::function_vv_copy, {cb, callback});
        uint32_t a = call(create, {node, 0, 0, cb});
        call(fn::Node_runAction, {node, call(WRAP_ACTION, {a})});
        functionDestroy(cb);
        return 0;
    }
    static uint32_t runLeftOut(uint32_t self, uint32_t cb) { return runOut(self, cb, 0x45B735); }
    static uint32_t runTopOut(uint32_t self, uint32_t cb) { return runOut(self, cb, 0x45B1DD); }
    static uint32_t runRightOut(uint32_t self, uint32_t cb) { return runOut(self, cb, 0x45BA3D); }
    static uint32_t runBottomOut(uint32_t self, uint32_t cb) { return runOut(self, cb, 0x45B469); }

    // back to the home position, then SceneUtil::create*InAction(node, 0, 0, std::function())
    static uint32_t runIn(uint32_t self, uint32_t create) {
        Frame f;
        if (ld32(self + 8) == 0) return 0;
        call(STOP_ALL_ACTIONS, {ld32(self + 8)});
        uint32_t w = ld32(self + 8);
        vcall(w, 0x4C, {w, self});  // setPosition(home)
        uint32_t node = ld32(self + 8);
        uint32_t empty = newEmptyFunction(f);
        uint32_t a = call(create, {node, 0, 0, empty});
        call(fn::Node_runAction, {node, call(WRAP_ACTION, {a})});
        functionDestroy(empty);
        return 0;
    }
    static uint32_t runRightIn(uint32_t self) { return runIn(self, 0x45B8BD); }
    static uint32_t runBottomIn(uint32_t self) { return runIn(self, 0x45B2F9); }

    // slide by `dy(height)` with a fade to `opacity`, then the delay and the callback
    static uint32_t runSlide(uint32_t self, uint32_t callback, int mode) {
        Frame f;
        uint32_t node = ld32(self + 8);
        if (node == 0) return 0;
        call(STOP_ALL_ACTIONS, {node});
        call(0x45B16D, {ld32(self + 8), 1});  // SceneUtil::setCascadeOpacity(widget, true)
        uint32_t posCopy = 0;
        if (mode != 2) {
            uint32_t w = ld32(self + 8);
            posCopy = f.alloc(8);
            call(rwlib::cocos2d::Vec2::ctor_base__RKS0_, {posCopy, vcall(w, 0x54, {w})});  // Vec2 copy of getPosition
        }
        uint32_t size = f.alloc(8);
        uint32_t w = ld32(self + 8);
        call(rwlib::cocos2d::Size::ctor_base__RKS0_, {size, vcall(w, 0xAC, {w})});
        w = ld32(self + 8);
        if (int32_t(vcall(w, 0x114, {w})) >= 1) {  // getChildrenCount
            w = ld32(self + 8);
            uint32_t children = vcall(w, 0x10C, {w});
            uint32_t first = ld32(ld32(children));
            call(rwlib::cocos2d::Size::op_assign__RKS0_, {size, vcall(first, 0xAC, {first})});  // Size::operator=
        }
        uint32_t target = f.alloc(8);
        float h = ldf(size + 4);
        uint32_t opacity;
        if (mode == 0) {  // info out: up by (-50 - h) / 3
            float y = (-50.0f - h) / 3.0f + ldf(self + 4);
            call(rwlib::cocos2d::Vec2::ctor_base__ff, {target, ld32(posCopy), fbits(y)});
            opacity = 0;
        } else if (mode == 1) {  // select out: up by h / 3
            float y = h / 3.0f + ldf(self + 4);
            call(rwlib::cocos2d::Vec2::ctor_base__ff, {target, ld32(posCopy), fbits(y)});
            opacity = 0;
        } else {  // info in: back home, opaque
            call(rwlib::cocos2d::Vec2::ctor_base__ff, {target, ld32(self), ld32(self + 4)});
            opacity = 0xFF;
        }
        uint32_t dur = ld32(ld32(FADEOUT_GOT));
        uint32_t move = call(fn::MoveTo_create, {dur, target});
        uint32_t fade = call(fn::FadeTo_create, {dur, opacity});
        uint32_t spawn = call(fn::Spawn_create, {move, fade, 0});
        call(rwlib::cocos2d::Vec2::dtor_base, {target});
        uint32_t delayTime = ld32(ld32(DELAY_GOT));
        uint32_t widget = ld32(self + 8);
        uint32_t delay = call(fn::DelayTime_create, {delayTime});
        uint32_t cf = call(fn::CallFunc_create, {callback});
        uint32_t seq = call(fn::Sequence_create, {spawn, delay, cf, 0});
        call(fn::Node_runAction, {widget, call(WRAP_ACTION, {seq})});
        if (posCopy != 0) call(rwlib::cocos2d::Vec2::dtor_base, {posCopy});
        return 0;
    }
    static uint32_t runInfoOut(uint32_t self, uint32_t cb) { return runSlide(self, cb, 0); }
    static uint32_t runSelectOut(uint32_t self, uint32_t cb) { return runSlide(self, cb, 1); }
    static uint32_t runInfoIn(uint32_t self, uint32_t cb) { return runSlide(self, cb, 2); }

    static uint32_t runFadeOut(uint32_t self, uint32_t callback) {
        uint32_t node = ld32(self + 8);
        if (node == 0) return node;
        call(STOP_ALL_ACTIONS, {node});
        uint32_t dur = ld32(ld32(FADEOUT_GOT));
        uint32_t fade = call(fn::FadeTo_create, {dur, 0});
        uint32_t spawn = call(fn::Spawn_create, {fade, 0});
        uint32_t delayTime = ld32(ld32(DELAY_GOT));
        uint32_t widget = ld32(self + 8);
        uint32_t delay = call(fn::DelayTime_create, {delayTime});
        uint32_t cf = call(fn::CallFunc_create, {callback});
        uint32_t seq = call(fn::Sequence_create, {spawn, delay, cf, 0});
        return call(fn::Node_runAction, {widget, call(WRAP_ACTION, {seq})});
    }
};
RW_FUNCTION(0x71EF58, "AdventurePanel::searchWidget", AdventurePanel::searchWidget);
RW_FUNCTION(0x723954, "AdventurePanel::createWidget", AdventurePanel::createWidget);
RW_FUNCTION(0x723D10, "AdventurePanel::runLeftOutAnimation", AdventurePanel::runLeftOut);
RW_FUNCTION(0x725AA8, "AdventurePanel::runTopOutAnimation", AdventurePanel::runTopOut);
RW_FUNCTION(0x725B54, "AdventurePanel::runRightOutAnimation", AdventurePanel::runRightOut);
RW_FUNCTION(0x725C00, "AdventurePanel::runBottomOutAnimation", AdventurePanel::runBottomOut);
RW_FUNCTION(0x726114, "AdventurePanel::runRightInAnimation", AdventurePanel::runRightIn);
RW_FUNCTION(0x7261CC, "AdventurePanel::runBottomInAnimation", AdventurePanel::runBottomIn);
RW_FUNCTION(0x7256DC, "AdventurePanel::runInfoOutAnimation", AdventurePanel::runInfoOut);
RW_FUNCTION(0x72592C, "AdventurePanel::runSelectOutAnimation", AdventurePanel::runSelectOut);
RW_FUNCTION(0x72ABDC, "AdventurePanel::runInfoInAnimation", AdventurePanel::runInfoIn);
RW_FUNCTION(0x725864, "AdventurePanel::runFadeOutAnimation", AdventurePanel::runFadeOut);

// ============================================================================================ SceneAdventureSelect
struct AdventureSelectTutorials {
    static constexpr uint32_t OPEN_TUTORIAL = 0x4684C9;  // SceneUtil::openTutorialPopup(out, id, cb, node, ...)
    static uint32_t simple(uint32_t self, uint32_t callback, uint32_t id) {
        Frame f;
        call(0x4BA8C9, {});  // APIManager::getInstance
        if (ld32(self + 0x28C) != 0) {
            if (ld32(callback + 0x10) != 0) call(fn::function_vv_call, {callback});
            return 0;
        }
        uint32_t cb = f.alloc(20);
        call(fn::function_vv_copy, {cb, callback});
        uint32_t out = f.alloc(20);
        call(OPEN_TUTORIAL, {out, id, cb, 0, 0, 0, 0});
        functionDestroy(out);
        functionDestroy(cb);
        return 0;
    }
    static uint32_t open33(uint32_t self, uint32_t cb) { return simple(self, cb, 33); }
    static uint32_t open34(uint32_t self, uint32_t cb) { return simple(self, cb, 34); }

    // a popup pointing at `node`, closed through AppState's tutorial close function
    static void withCloseFunc(Frame &f, uint32_t node, uint32_t callback, uint32_t id) {
        uint32_t st = call(0x4E66DD, {});  // AppState::getInstance
        uint32_t cb = f.alloc(20);
        call(fn::function_vv_copy, {cb, callback});
        uint32_t popup = f.alloc(20);
        call(OPEN_TUTORIAL, {popup, id, cb, node, 0, 0, 0});
        uint32_t closer = f.alloc(20);
        call(0x4EB8D9, {closer, st, popup, id});  // AppState::setTutorialCloseFunc
        functionDestroy(closer);
        functionDestroy(popup);
        functionDestroy(cb);
    }
    static uint32_t open104(uint32_t self, uint32_t callback) {
        Frame f;
        uint32_t root = ld32(self + 0x37C);
        uint32_t s = f.alloc(12);
        strInit(s, 0xE06D3B, 9);  // "QuestBut1"
        uint32_t node = call(SEARCH_NODE, {root, s});
        strFree(s);
        withCloseFunc(f, node, callback, 104);
        return 0;
    }
    static uint32_t open105(uint32_t self, uint32_t callback) {
        Frame f;
        uint32_t root = ld32(self + 0x388);
        uint32_t s = f.alloc(12);
        strInit(s, 0xDFF8C6, 8);  // "RaidInfo"
        uint32_t info = call(SEARCH_NODE, {root, s});
        strFree(s);
        uint32_t s2 = f.alloc(12);
        strInit(s2, 0xE06ED0, 0x11);  // "Button_QuestStart"
        uint32_t node = call(SEARCH_NODE, {info, s2});
        strFree(s2);
        withCloseFunc(f, node, callback, 105);
        return 0;
    }
};
RW_FUNCTION(0x727E94, "SceneAdventureSelect::openTutorial33", AdventureSelectTutorials::open33);
RW_FUNCTION(0x727F70, "SceneAdventureSelect::openTutorial34", AdventureSelectTutorials::open34);
RW_FUNCTION(0x72804C, "SceneAdventureSelect::openTutorial104", AdventureSelectTutorials::open104);
RW_FUNCTION(0x7281A4, "SceneAdventureSelect::openTutorial105", AdventureSelectTutorials::open105);

}  // namespace w4b2
