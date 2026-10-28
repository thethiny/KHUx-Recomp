// SceneManager: lambda bodies and std::function slots that run in game but were left after batch 2 (batch 3, named
// from their __func vtables' target_type). `functor` is the __func object: vtable at +0, then the captures (the
// SceneManager at +4 for most).
#include "ScrollRowTouch.hpp"   // rowContainsTouch (+ SceneUi.hpp)

using namespace scene;

namespace {

constexpr uint32_t SceneManager_generateTouchEffect = 0x61257D;       // (mgr, Vec2)
constexpr uint32_t SceneManager_generateTouchMoveEffect = 0x612765;   // (mgr, Vec2)
constexpr uint32_t Touch_getLocation = 0x900E57;                      // (Vec2* out, Touch*)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_ctor = 0x8DF027;                              // (Size*, w, h)
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41;          // (std::string* out, int)
constexpr uint32_t TextAtlas_setString = 0x83AED7;                    // (atlas, const std::string&)
constexpr uint32_t String_compare = 0x47CBD5;                         // std::string::compare(const char*): 0 = equal
constexpr uint32_t FunctionFn_call = 0x634989;   // std::function<void(std::function<void(bool)>)>::operator()(function)
constexpr uint32_t PrevChangeNext_body = 0x6349CD;                    // sub_6349CC(captures, bool): $_26's inner body

// SceneManager fields
constexpr uint32_t SM_ROOT = 0x3F0;              // the layer the no-tap shadow is on
constexpr uint32_t SM_CHAT_BADGE = 0x3FC;
constexpr uint32_t SM_TOUCH_EFFECTS = 0x320;     // byte: touch effects on
constexpr uint32_t SM_HEADER = 0x338;            // the header node
constexpr uint32_t SM_MENU_OPEN = 0x35C, SM_MENU_CLOSE = 0x360, SM_MENU_FLAG = 0x369, SM_MENU_BUTTON = 0x5C8;
constexpr uint32_t SM_NO_TAP_SHADOW = 0x4B8;
constexpr uint32_t SM_BEFORE_NEXT_SCENE = 0x4E8;  // std::function<void(std::function<void(bool)>)>
constexpr uint32_t S_TIP_LUX = 0xDFE849, S_OPEN = 0xE02440, S_CLOSE = 0xE03260;

constexpr uint32_t SM_NEXT_SCENE_A = 0x4E8, SM_NEXT_SCENE_B = 0x4D0, SM_NEXT_SCENE_C = 0x5B0;   // pending callbacks
constexpr uint32_t SM_CHANGE_HOOK = 0x500;        // std::function<void(std::function<void(bool)>)>
constexpr uint32_t SM_CHANGING = 0x4CD;           // byte
constexpr uint32_t VT_prevChangeNext_hook = 0xF603CC;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;                    // (ui::Text*)
constexpr uint32_t Text_setString = 0x83A2DD;                         // ui::Text::setString(const std::string&)
constexpr uint32_t FunctionTE_swap = 0x4F3699;                        // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t VT_touchLambda = 0xF6052C;                         // [one word]
// initGlobalMenu $_16 (sub_6359EC): the footer menu's touch began
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t Touch_getLocationGM = 0x900E57;                    // (Vec2* out, Touch*)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;                // (Vec2* out, node, const Vec2&)
constexpr uint32_t Rect_ctor = 0x8DF18B;                              // (Rect*, x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t GlobalMenu_buttonAt = 0x6357F9;                    // sub_6357F8(buttons, Touch*): index or -1
constexpr uint32_t AppState_getInstanceGM = 0x4E66DD;
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D;          // (sret function, app, id)
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;         // (app, id)
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;             // (node, const std::string&)
constexpr uint32_t StringBuffer_ctorGM = 0x49140D;                    // (buffer, format, ...)
constexpr uint32_t ImageView_loadTexture = 0x839731;                  // (view, const std::string&, TextureResType)
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81, AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t FunctionHook_swap = 0x607685;     // std::function<void(std::function<void(bool)>)>::swap
constexpr uint32_t FunctionLayer_assign = 0x612E29;  // std::function<cocos2d::Layer*()>::operator=(const function&)
constexpr uint32_t FunctionBool_call = 0x51022D;     // std::function<bool()>::operator()
constexpr uint32_t SceneManager_callCurrentSceneRunOutAnimation = 0x62186D;
constexpr uint32_t OtherMenuDialog_create = 0x59E84D, OtherMenuDialog_open = 0x59E8E1;   // open(dialog, Node*)
constexpr uint32_t GOT_FUNC_LAYER_PTR = 0xFD6074;   // &vtable of the __func holding a Layer*(*)()
constexpr uint32_t GOT_CREATE_MYPAGE = 0xFD6078, GOT_CREATE_ADVENTURE_SELECT = 0xFD669C,
                   GOT_CREATE_DECK_EDIT = 0xFD66B0, GOT_CREATE_SPHERE_BOARD_SELECT = 0xFD68A0,
                   GOT_CREATE_MEDAL_LIST = 0xFD60BC, GOT_CREATE_SHOP_TOP = 0xFD66F8;   // &createSceneLayer<X>
constexpr uint32_t VT_runOutDone = 0xF60634;                          // [manager]
constexpr uint32_t VT_hookMyPage = 0xF60660, VT_hookAdventure = 0xF60608, VT_hookSphere = 0xF605DC,
                   VT_hookShop = 0xF605B0;                            // the scene change hooks (no captures)
constexpr uint32_t SM_MENU_READY = 0x588;          // byte: the footer menu takes touches
constexpr uint32_t SM_MENU_SHOWN = 0x368;          // byte: the menu is open
constexpr uint32_t SM_MENU_SELECTED = 0x364;       // the pressed button, -1 none
constexpr uint32_t SM_NEXT_LAYER = 0x370;          // std::function<cocos2d::Layer*()>
constexpr uint32_t SM_CAN_CHANGE = 0x4D0;          // std::function<bool()>
constexpr uint32_t SM_ADVENTURE_KIND = 0x28C;
constexpr uint32_t SM_DECK_FLAGS = 0x2E0, SM_AFTER_SELECT = 0x30A, SM_MEDAL_SORT = 0x2A0, SM_POPUP_PARENT = 0x3E8;
constexpr uint32_t S_BUTTON_N = 0xE03266, S_BAR_BUTTON_ON = 0xE035EC, S_BAR_BUTTON_OFF = 0xE03270;
constexpr uint32_t V_getAnimationGM = 0x298, V_animPlayGM = 0x68;
constexpr uint32_t TUTORIAL_MENU_OPEN = 0x77, TUTORIAL_MENU_BUTTON = 0x78, TUTORIAL_PHASE_DONE = 995;
constexpr uint32_t VT_hideHeader_29 = 0xF6031C;
constexpr uint32_t VT_prevChangeNext_26 = 0xF603A0;
constexpr uint32_t VT_prevChangeNext_inner = 0xF603F8;
constexpr uint32_t VT_initGlobalMenu_17 = 0xF60558;
constexpr uint32_t VT_createHeader_13 = 0xF606B8;
// the footer menu's touch moved, the scene change hooks
constexpr uint32_t MENU_BUTTONS = 7;
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;                    // (function<void(bool)>)
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261; // (function<void(bool)>)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;            // (function<void(bool)>)
constexpr uint32_t APIManager_executeUserSphereCheck = 0x4CCBC1;           // (api, SphereLockType, const function&)
constexpr uint32_t SPHERE_LOCK_MENU = 3;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;   // (mgr, const function<Layer*()>&, bool, bool, FadeType)

}  // namespace

struct SceneManagerBatch3 {
    // openNoTapScreen $_102 [this]: the time is up - remove the shadow
    static void noTapScreenEnd(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        const uint32_t root = ld32(call(fn::SceneManager_getInstance, {}) + SM_ROOT);
        vcall(root, node::removeChild, {root, ld32(self + SM_NO_TAP_SHADOW), 1});
        st32(self + SM_NO_TAP_SHADOW, 0);
    }

    // chatDataUpdate $_34 [this](a, b, c): the chat badge when there is something new
    static void chatBadge(uint32_t functor, uint32_t aPtr, uint32_t bPtr, uint32_t cPtr) {
        const uint32_t c = ld32(cPtr), sum = ld32(bPtr) + c;
        if (ld32(aPtr) != 0 || c == 0 || sum < 2) return;
        const uint32_t badge = ld32(ld32(functor + 4) + SM_CHAT_BADGE);
        vcall(badge, node::setVisible, {badge, 1});
    }

    // updateHeader $_30 [this, atlas](float ratio, int value): the Lux tip gauge and its number
    static void headerLux(uint32_t functor, uint32_t ratioPtr, uint32_t valuePtr) {
        const int32_t value = int32_t(ld32(valuePtr));
        const float ratio = ldf(ratioPtr);
        const uint32_t tip = findNode(ld32(ld32(functor + 4) + SM_HEADER), S_TIP_LUX, 7);
        const uint32_t setContentSize = ld32(ld32(tip) + node::setContentSize);
        const uint32_t size = vcall(tip, node::getContentSize, {tip});
        Frame f;
        const uint32_t newSize = f.alloc(8);
        call(Size_ctor, {newSize, fbits(ratio * 100.0f), ld32(size + 4)});
        call(setContentSize, {tip, newSize});
        const uint32_t atlas = ld32(functor + 8);
        const uint32_t text = f.alloc(12);
        call(SceneUtil_numberToAtlasString, {text, uint32_t(value)});
        call(TextAtlas_setString, {atlas, text});
        strFree(text);
    }

    // hideHeader $_29 [?, parent, header, done]: remove the header, then `done`
    static void headerHidden(uint32_t functor) {
        const uint32_t parent = ld32(functor + 8);
        vcall(parent, node::removeChild, {parent, ld32(functor + 0xC), 1});
        if (ld32(functor + 0x20)) call(fn::function_vv_call, {functor + 0x10});
    }
    static uint32_t headerHiddenClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x28});
        if (obj) {
            st32(obj, VT_hideHeader_29);
            const uint32_t a = ld32(self + 8), b = ld32(self + 0xC);
            st32(obj + 8, a);
            st32(obj + 0xC, b);
            call(fn::function_vv_copy, {obj + 0x10, self + 0x10});
        }
        return obj;
    }
    static void headerHiddenDelete(uint32_t self) {
        functionDestroy(self + 0x10);
        call(fn::operator_delete, {self});
    }

    // callPrevChangeNextSceneAPIs $_26 [this, a, b](bool ok): through the "before the next scene" hook if set
    static void prevChangeNext(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t self = ld32(functor + 4);
        if (ld32(self + SM_BEFORE_NEXT_SCENE + 16)) {
            Frame f;
            const uint32_t next = newFunction(f, VT_prevChangeNext_inner, {ld32(functor + 8), ld32(functor + 0xC)});
            call(FunctionFn_call, {self + SM_BEFORE_NEXT_SCENE, next});
            functionDestroy(next);
        } else {
            call(PrevChangeNext_body, {functor + 8, 1});
        }
    }

    // initGlobalMenu $_16 (sub_6359EC) [this, menu, buttons (+0xC), manager (+0x10), off root (+0x14), on root
    // (+0x18)](Touch*, Event*): a touch on the footer menu. Closed: a touch inside opens it. Open: a button lights
    // up (only button 1 while the tutorial's close function is set); a touch outside the buttons (after the
    // tutorial) closes it and goes to the scene of the button pressed before.
    static uint32_t globalMenuTouched(uint32_t functor, uint32_t touchPtr, uint32_t /*event*/) {
        const uint32_t self = ld32(functor + 4);
        if (!ld8(self + SM_MENU_READY)) return 0;
        const uint32_t touch = ld32(touchPtr);
        if (ld8(self + SM_MENU_FLAG)) return 0;
        Frame f;
        const uint32_t at = f.alloc(8), local = f.alloc(8), rect = f.alloc(16);
        call(Touch_getLocationGM, {at, touch});
        call(Node_convertToNodeSpace, {local, ld32(functor + 8), at});
        call(Vec2_dtor, {at});
        uint32_t menu = ld32(functor + 8);
        const uint32_t width = ld32(vcall(menu, node::getContentSize, {menu}));
        menu = ld32(functor + 8);
        const uint32_t height = ld32(vcall(menu, node::getContentSize, {menu}) + 4);
        call(Rect_ctor, {rect, 0, 0, width, height});
        const uint32_t result = touchedMenu(f, functor, self, touch, rect, local);
        call(Vec2_dtor, {rect});
        call(Vec2_dtor, {local});
        return result;
    }

    static uint32_t touchedMenu(Frame &f, uint32_t functor, uint32_t self, uint32_t touch, uint32_t rect,
                                uint32_t local) {
        if (!ld8(self + SM_MENU_SHOWN)) {
            if (!call(Rect_containsPoint, {rect, local})) return 0;
            if (!ld8(self + SM_MENU_SHOWN)) {
                openMenu(f, self);
                return 1;
            }
        }
        const int32_t index = int32_t(call(GlobalMenu_buttonAt, {functor + 0xC, touch}));
        const uint32_t closeFunc = f.alloc(24);
        call(AppState_getTutorialCloseFunc, {closeFunc, call(AppState_getInstanceGM, {}), TUTORIAL_MENU_BUTTON});
        const bool tutorial = ld32(closeFunc + 16) != 0;
        functionDestroy(closeFunc);
        if (tutorial) {
            if (index != 1) return 0;
            call(AppState_execTutorialCloseFunc, {call(AppState_getInstanceGM, {}), TUTORIAL_MENU_BUTTON});
        } else {
            if (int32_t(call(SceneUtil_getNexTutorialphase, {})) < TUTORIAL_PHASE_DONE) return 0;
            if (index == -1) {
                closeMenu(f, functor);
                return 0;
            }
        }
        // the button lights up
        const uint32_t button = menuButton(f, ld32(functor + 0x18), uint32_t(index));
        setButtonImage(f, button, S_BAR_BUTTON_ON, uint32_t(index));
        st32(self + SM_MENU_SELECTED, uint32_t(index));
        return 1;
    }

    // "Button_<index + 1>" under root
    static uint32_t menuButton(Frame &f, uint32_t root, uint32_t index) {
        const uint32_t buffer = f.alloc(STRING_BUFFER_SIZE);
        call(StringBuffer_ctorGM, {buffer, S_BUTTON_N, index + 1});
        const uint32_t name = f.alloc(12);
        strInit(name, buffer, call(STRLEN, {buffer}));
        const uint32_t button = call(SceneUtil_searchNodeByName, {root, name});
        strFree(name);
        return button;
    }
    static void setButtonImage(Frame &f, uint32_t button, uint32_t format, uint32_t index) {
        const uint32_t buffer = f.alloc(STRING_BUFFER_SIZE);
        call(StringBuffer_ctorGM, {buffer, format, index + 1});
        const uint32_t path = f.alloc(12);
        strInit(path, buffer, call(STRLEN, {buffer}));
        call(ImageView_loadTexture, {button, path, 0});
        strFree(path);
    }
    static void playMenuAnimation(Frame &f, uint32_t armature, uint32_t name) {
        const uint32_t animation = vcall(armature, V_getAnimationGM, {armature});
        const uint32_t play = ld32(ld32(animation) + V_animPlayGM);
        const uint32_t movement = newStr(f, name);
        call(play, {animation, movement, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(movement);
    }

    static void openMenu(Frame &f, uint32_t self) {
        const uint32_t closeFunc = f.alloc(24);
        call(AppState_getTutorialCloseFunc, {closeFunc, call(AppState_getInstanceGM, {}), TUTORIAL_MENU_OPEN});
        const bool tutorial = ld32(closeFunc + 16) != 0;
        functionDestroy(closeFunc);
        if (tutorial) call(AppState_execTutorialCloseFunc, {call(AppState_getInstanceGM, {}), TUTORIAL_MENU_OPEN});
        call(AudioPlaySystemSEPopUp, {});
        st8(self + SM_MENU_SHOWN, 1);
        vcall(ld32(self + SM_MENU_CLOSE), node::setVisible, {ld32(self + SM_MENU_CLOSE), 1});
        vcall(ld32(self + SM_MENU_OPEN), node::setVisible, {ld32(self + SM_MENU_OPEN), 1});
        playMenuAnimation(f, ld32(self + SM_MENU_OPEN), S_OPEN);
        st8(self + SM_MENU_FLAG, 1);
    }

    static void closeMenu(Frame &f, uint32_t functor) {
        call(AudioPlaySystemSECancel, {});
        const uint32_t mgr = ld32(functor + 0x10);
        vcall(ld32(mgr + SM_MENU_BUTTON), node::setVisible, {ld32(mgr + SM_MENU_BUTTON), 0});
        vcall(ld32(mgr + SM_MENU_OPEN), node::setVisible, {ld32(mgr + SM_MENU_OPEN), 1});
        playMenuAnimation(f, ld32(mgr + SM_MENU_OPEN), S_CLOSE);
        st8(mgr + SM_MENU_FLAG, 1);
        st8(mgr + SM_MENU_SHOWN, 0);
        if (ld32(mgr + SM_MENU_SELECTED) != 0xFFFFFFFFu) {
            const uint32_t button = menuButton(f, ld32(functor + 0x14), ld32(mgr + SM_MENU_SELECTED));
            setButtonImage(f, button, S_BAR_BUTTON_OFF, ld32(mgr + SM_MENU_SELECTED));
            switch (ld32(mgr + SM_MENU_SELECTED)) {
            case 0:
                setChangeHook(f, mgr, VT_hookMyPage);
                changeScene(f, mgr, GOT_CREATE_MYPAGE);
                break;
            case 1:
                setChangeHook(f, mgr, VT_hookAdventure);
                st32(call(fn::SceneManager_getInstance, {}) + SM_ADVENTURE_KIND, 9);
                changeScene(f, mgr, GOT_CREATE_ADVENTURE_SELECT);
                st8(call(fn::SceneManager_getInstance, {}) + SM_AFTER_SELECT, 1);
                break;
            case 2:
                st8(mgr + SM_DECK_FLAGS, 0);
                st8(mgr + SM_DECK_FLAGS + 1, 0);
                changeScene(f, mgr, GOT_CREATE_DECK_EDIT);
                break;
            case 3:
                setChangeHook(f, mgr, VT_hookSphere);
                st32(call(fn::SceneManager_getInstance, {}) + SM_ADVENTURE_KIND, 1);
                changeScene(f, mgr, GOT_CREATE_SPHERE_BOARD_SELECT);
                break;
            case 4: {
                const uint32_t g = call(fn::SceneManager_getInstance, {});
                st32(g + SM_MEDAL_SORT + 4, 0);
                st32(g + SM_MEDAL_SORT, 0);
                changeScene(f, mgr, GOT_CREATE_MEDAL_LIST);
                break;
            }
            case 5:
                setChangeHook(f, mgr, VT_hookShop);
                changeScene(f, mgr, GOT_CREATE_SHOP_TOP);
                break;
            case 6: {
                const uint32_t dialog = call(OtherMenuDialog_create, {});
                call(OtherMenuDialog_open,
                     {dialog, ld32(call(fn::SceneManager_getInstance, {}) + SM_POPUP_PARENT)});
                break;
            }
            default:
                break;
            }
        }
        st32(mgr + SM_MENU_SELECTED, 0xFFFFFFFFu);
    }

    // the manager's scene change hook = a lambda (the old one destroyed)
    static void setChangeHook(Frame &f, uint32_t mgr, uint32_t vtable) {
        const uint32_t hook = newFunction(f, vtable, {});
        call(FunctionHook_swap, {hook, mgr + SM_CHANGE_HOOK});
        functionDestroy(hook);
    }
    // the next scene's layer factory, then (when the current scene allows it) its run-out animation
    static void changeScene(Frame &f, uint32_t mgr, uint32_t creatorGot) {
        const uint32_t layer = newFunction(f, ld32(GOT_FUNC_LAYER_PTR) + 8, {ld32(creatorGot)});
        call(FunctionLayer_assign, {mgr + SM_NEXT_LAYER, layer});
        if (!ld32(mgr + SM_CAN_CHANGE + 16) || call(FunctionBool_call, {mgr + SM_CAN_CHANGE}) == 1) {
            const uint32_t done = newFunction(f, VT_runOutDone, {mgr});
            call(SceneManager_callCurrentSceneRunOutAnimation, {mgr, done});
            functionDestroy(done);
        }
        functionDestroy(layer);
    }

    // sub_611AA0(text, id): a text with a shadow, from the text data
    static void setShadowText(uint32_t text, uint32_t id) {
        call(SceneUtil_setShadow, {text});
        Frame f;
        const uint32_t td = f.alloc(8);
        call(fn::TextData_get, {td, 0, id});
        const uint32_t str = ld32(ld32(td) + 4);
        const uint32_t value = newStr(f, str);
        call(Text_setString, {text, value});
        strFree(value);
        sharedRelease(td);
    }

    // sub_611B60(function, captures): function = a touch lambda carrying the first capture word
    static void assignTouchLambda(uint32_t function, uint32_t captures) {
        Frame f;
        const uint32_t lambda = newFunction(f, VT_touchLambda, {ld32(captures)});
        call(FunctionTE_swap, {lambda, function});
        functionDestroy(lambda);
    }

    // sub_6349CC(captures [this, next manager, ?], int ok): the scene change went through - drop the pending
    // callbacks, then through the change hook (or, without one, end the change)
    static void prevChangeNextBody(uint32_t captures, uint32_t ok) {
        if (ok != 1) return;
        const uint32_t self = ld32(captures);
        for (uint32_t fnAt : {SM_NEXT_SCENE_A, SM_NEXT_SCENE_B, SM_NEXT_SCENE_C}) {
            functionDestroy(self + fnAt);
            st32(self + fnAt + 16, 0);
        }
        if (ld32(self + SM_CHANGE_HOOK + 16)) {
            Frame f;
            const uint32_t next = newFunction(f, VT_prevChangeNext_hook, {ld32(captures + 4)});
            call(FunctionFn_call, {self + SM_CHANGE_HOOK, next});
            functionDestroy(next);
            return;
        }
        const uint32_t other = ld32(captures + 4);
        functionDestroy(other + SM_CHANGE_HOOK);
        st32(other + SM_CHANGE_HOOK + 16, 0);
        st8(other + SM_CHANGING, 0);
    }

    // __clone(__base*) of three plain-word lambdas ($_26, initGlobalMenu $_17) and a two-word one (createHeader $_13)
    static void prevChangeNextCloneInto(uint32_t self, uint32_t p) {
        if (!p) return;
        st32(p, VT_prevChangeNext_26);
        const uint32_t a = ld32(self + 4), b = ld32(self + 8), c = ld32(self + 0xC);
        st32(p + 0xC, c);
        st32(p + 8, b);
        st32(p + 4, a);
    }
    static void globalMenuCloneInto(uint32_t self, uint32_t p) {
        if (!p) return;
        st32(p, VT_initGlobalMenu_17);
        const uint32_t a = ld32(self + 4), b = ld32(self + 8), c = ld32(self + 0xC);
        st32(p + 0xC, c);
        st32(p + 8, b);
        st32(p + 4, a);
    }
    static void headerCloneInto(uint32_t self, uint32_t p) {
        if (!p) return;
        st32(p, VT_createHeader_13);
        const uint32_t a = ld32(self + 4), b = ld32(self + 8);
        st32(p + 8, b);
        st32(p + 4, a);
    }

    // initGlobalMenu $_14 [this](armature, MovementEventType, const std::string& id): the menu's open / close
    // animation ended
    static void globalMenuAnimation(uint32_t functor, uint32_t /*armature*/, uint32_t typePtr, uint32_t id) {
        if (ld32(typePtr) != 1) return;                       // MovementEventType::COMPLETE
        const uint32_t self = ld32(functor + 4);
        st8(self + SM_MENU_FLAG, 0);
        if (call(String_compare, {id, S_OPEN}) == 0) {
            const uint32_t button = ld32(self + SM_MENU_BUTTON);
            vcall(button, node::setVisible, {button, 1});
            const uint32_t open = ld32(self + SM_MENU_OPEN);
            vcall(open, node::setVisible, {open, 0});
            return;
        }
        if (call(String_compare, {id, S_CLOSE}) != 0) return;
        const uint32_t open = ld32(self + SM_MENU_OPEN);
        vcall(open, node::setVisible, {open, 0});
        if (const uint32_t close = ld32(self + SM_MENU_CLOSE)) vcall(close, node::setVisible, {close, 0});
    }

    // init $_1 / $_2 [this](Touch*, Event*): the touch effect on touch began (returns true) / moved
    static uint32_t touchBegan(uint32_t functor, uint32_t touchPtr) {
        const uint32_t self = ld32(functor + 4);
        if (ld8(self + SM_TOUCH_EFFECTS)) {
            Frame f;
            const uint32_t at = f.alloc(8);
            call(Touch_getLocation, {at, ld32(touchPtr)});
            call(SceneManager_generateTouchEffect, {self, at});
            call(Vec2_dtor, {at});
        }
        return 1;
    }
    static void touchMoved(uint32_t functor, uint32_t touchPtr) {
        const uint32_t self = ld32(functor + 4);
        if (!ld8(self + SM_TOUCH_EFFECTS)) return;
        Frame f;
        const uint32_t at = f.alloc(8);
        call(Touch_getLocation, {at, ld32(touchPtr)});
        call(SceneManager_generateTouchMoveEffect, {self, at});
        call(Vec2_dtor, {at});
    }

    // initGlobalMenu $_17 [this, buttons root (+8), button images root (+0xC)](Touch*, Event*): a touch moving
    // over the open menu (after the tutorial) lights the button under it and darkens the one lit before
    static void globalMenuMoved(uint32_t functor, uint32_t touchPtr) {
        const uint32_t self = ld32(functor + 4);
        if (!ld8(self + SM_MENU_SHOWN)) return;
        const uint32_t touch = ld32(touchPtr);
        if (int32_t(call(SceneUtil_getNexTutorialphase, {})) < TUTORIAL_PHASE_DONE) return;
        const uint32_t index = call(GlobalMenu_buttonAt, {functor + 8, touch});
        const uint32_t selected = ld32(self + SM_MENU_SELECTED);
        if (index == selected) return;
        Frame f;
        if (selected != 0xFFFFFFFFu)
            setButtonImage(f, menuButton(f, ld32(functor + 0xC), selected), S_BAR_BUTTON_OFF, selected);
        if (index != 0xFFFFFFFFu)
            setButtonImage(f, menuButton(f, ld32(functor + 0xC), index), S_BAR_BUTTON_ON, index);
        st32(self + SM_MENU_SELECTED, index);
    }

    // sub_6357F8(&root, Touch*): the footer button (0..6) under the touch, -1 for none
    static uint32_t buttonAt(uint32_t rootRef, uint32_t touch) {
        for (uint32_t i = 0; i < MENU_BUTTONS; i++) {
            Frame f;
            if (rowContainsTouch(menuButton(f, ld32(rootRef), i), touch)) return i;
        }
        return 0xFFFFFFFFu;
    }

    // initGlobalMenu $_121's scene change hooks and closeGlobalMenu's ($_19 .. $_23, the same code) [](function<void
    // (bool)> done): the next scene's API calls with `done`; all but My Page's first mark the API busy and drop the
    // pending payment callback
    // done moved out of the argument, then `callApi(copy of done)`
    static void hookCall(uint32_t donePtr, uint32_t callApi, bool reset) {
        Frame f;
        const uint32_t done = f.alloc(24);
        call(fn::function_vb_move, {done, donePtr});
        if (reset) apiBusyDropPayment();
        const uint32_t copy = f.alloc(24);
        functionCopy(copy, done);
        call(callApi, {copy});
        functionDestroy(copy);
        functionDestroy(done);
    }
    static void hookShop(uint32_t, uint32_t donePtr) { hookCall(donePtr, SceneShopTop_callShopAPI, true); }
    static void hookAdventure(uint32_t, uint32_t donePtr) {
        hookCall(donePtr, SceneAdventureSelect_callAdventureSelectAPI, true);
    }
    static void hookMyPage(uint32_t, uint32_t donePtr) { hookCall(donePtr, SceneUtil_callPrepareMypageAPIs, false); }
    static void hookSphere(uint32_t, uint32_t donePtr) {
        Frame f;
        const uint32_t done = f.alloc(24);
        call(fn::function_vb_move, {done, donePtr});
        apiBusyDropPayment();
        call(APIManager_executeUserSphereCheck, {call(fn::APIManager_getInstance, {}), SPHERE_LOCK_MENU, done});
        functionDestroy(done);
    }

    // initGlobalMenu $_121's run-out end [this]: the next scene's layer (faded), then its factory dropped
    static void runOutDone(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        call(SceneManager_changeSceneLayer, {self, self + SM_NEXT_LAYER, 1, 1, 1});
        functionDestroy(self + SM_NEXT_LAYER);
        st32(self + SM_NEXT_LAYER + 16, 0);
    }
};

using L = SceneManagerBatch3;
RW_FUNCTION(0x625546, "SceneManager::openNoTapScreen::$_102::operator()", L::noTapScreenEnd);
RW_FUNCTION(0x633A9E, "SceneManager::chatDataUpdate::$_34::operator()", L::chatBadge);
RW_FUNCTION(0x634160, "SceneManager::updateHeader::$_30::operator()", L::headerLux);
RW_FUNCTION(0x634414, "SceneManager::hideHeader::$_29::operator()", L::headerHidden);
RW_FUNCTION(0x634334, "SceneManager::hideHeader::$_29 __clone()", L::headerHiddenClone);
RW_FUNCTION(0x6343E8, "SceneManager::hideHeader::$_29 destroy_deallocate()", L::headerHiddenDelete);
RW_FUNCTION(0x634884, "SceneManager::callPrevChangeNextSceneAPIs::$_26::operator()", L::prevChangeNext);
RW_FUNCTION(0x634850, "SceneManager::callPrevChangeNextSceneAPIs::$_26 __clone(__base*)", L::prevChangeNextCloneInto);
RW_FUNCTION(0x635520, "SceneManager::initGlobalMenu::$_17 __clone(__base*)", L::globalMenuCloneInto);
RW_FUNCTION(0x637168, "SceneManager::createHeader::$_13 __clone(__base*)", L::headerCloneInto);
RW_FUNCTION(0x637070, "SceneManager::initGlobalMenu::$_14::operator()", L::globalMenuAnimation);
RW_FUNCTION(0x63833C, "SceneManager::init::$_1::operator()", L::touchBegan);
RW_FUNCTION(0x638230, "SceneManager::init::$_2::operator()", L::touchMoved);
RW_FUNCTION(0x6349CC, "SceneManager (sub_6349CC): callPrevChangeNextSceneAPIs body", L::prevChangeNextBody);
RW_FUNCTION(0x611AA0, "SceneManager (sub_611AA0): a shadowed text from the text data", L::setShadowText);
RW_FUNCTION(0x611B60, "SceneManager (sub_611B60): assign a touch lambda", L::assignTouchLambda);
RW_FUNCTION(0x6359EC, "SceneManager::initGlobalMenu::$_16 (sub_6359EC): the footer menu touched", L::globalMenuTouched);

RW_FUNCTION(0x635554, "SceneManager::initGlobalMenu::$_17::operator() (touch moved)", L::globalMenuMoved);
RW_FUNCTION(0x6357F8, "SceneManager (sub_6357F8): the footer button under a touch", L::buttonAt);
RW_FUNCTION(0x636758, "SceneManager::initGlobalMenu::$_121 Shop hook", L::hookShop);
RW_FUNCTION(0x634E44, "SceneManager::closeGlobalMenu::$_23 (Shop hook)", L::hookShop);
RW_FUNCTION(0x636A7C, "SceneManager::initGlobalMenu::$_121 Adventure hook", L::hookAdventure);
RW_FUNCTION(0x635168, "SceneManager::closeGlobalMenu::$_21 (Adventure hook)", L::hookAdventure);
RW_FUNCTION(0x636910, "SceneManager::initGlobalMenu::$_121 Sphere hook", L::hookSphere);
RW_FUNCTION(0x634FFC, "SceneManager::closeGlobalMenu::$_22 (Sphere hook)", L::hookSphere);
RW_FUNCTION(0x636D0C, "SceneManager::initGlobalMenu::$_121 My Page hook", L::hookMyPage);
RW_FUNCTION(0x635320, "SceneManager::closeGlobalMenu::$_19 (My Page hook)", L::hookMyPage);
RW_FUNCTION(0x636C3E, "SceneManager::initGlobalMenu::$_121 run-out end", L::runOutDone);
