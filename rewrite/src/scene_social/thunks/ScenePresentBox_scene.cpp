// ScenePresentBox: the scene, the present box popup and the lambda bodies of both (the std::function thunks are
// in ScenePresentBox_thunks.cpp).
#include "ScenePresentBox.hpp"

#include "../SocialCommon.hpp"

using namespace social;
using Self = ScenePresentBox;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1;
constexpr uint32_t Layer_dtor = 0x88A88D;
constexpr uint32_t Layer_init = 0x88A8A1;
constexpr uint32_t Scene_create = 0x8A5F37;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t FadeTo_create = 0x86B3E1;               // (float duration, uint8 opacity)
constexpr uint32_t Widget_setBright = 0x825A09;
constexpr uint32_t Widget_isBright = 0x826FA5;
constexpr uint32_t Widget_isTouchEnabled = 0x826665;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t function_vv_assign = 0x4EB9CD;          // std::function<void()>::operator=(const function&)
constexpr uint32_t function_vv_copy = 0x4683CD;            // std::function<void()>::function(const function&)
constexpr uint32_t function_vv_move = 0x47F905;            // std::function<void()>::function(function&&)
constexpr uint32_t function_vv_call = 0x470E69;            // std::function<void()>::operator()
constexpr uint32_t function_vv_swap = 0x4EE86D;            // std::function<void()>::swap
constexpr uint32_t function_bv_swap = 0x47D0F1;            // std::function<bool()>::swap
constexpr uint32_t vector_u64_pushBackSlow = 0x4ECB6D;     // std::vector<u64>::__push_back_slow_path(const u64&)
constexpr uint32_t vector_u64_dtor = 0x468499;             // std::__vector_base<u64>::~__vector_base
constexpr uint32_t StringBuffer_ctor = 0x49140D;           // StringBuffer(this, fmt, ...)
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, dst, src, n, index)
constexpr uint32_t TutorialMisc_create = 0x777BB5;         // master::TutorialMisc::create(shared_ptr out, id)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_setBackButtonType = 0x613E27;
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;   // (mgr, type, const string&)
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneManager_hideTitleHeader = 0x6130B1;    // (mgr, std::function<void()> by value)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69;
constexpr uint32_t SceneManager_openPopup = 0x61524D;      // (mgr, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneManager_ChainOfTutorialShop = 0x6138AD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_executeUserPresentGet = 0x4CCF3D;   // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;  // (api, const function<void(bool)>&, bool)
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;  // (button, text, bool enabled, bool)
constexpr uint32_t PresentBoxScrollView_create = 0x50CC15;
constexpr uint32_t PresentBoxScrollView_setEnabledTapItem = 0x50DEDD;
constexpr uint32_t PresentBoxScrollView_setGetPresentEvent = 0x50E769;   // (view, std::function<void()> by value)
constexpr uint32_t PresentBoxScrollView_popUpAllGetPresent = 0x50E3C5;   // (view, std::function<void()> by value)
constexpr uint32_t BaseScrollView_u64_setItemIDs = 0x4FD231;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t ScenePresentBox_openPresentBoxDialogMain = 0x6CFB59;
constexpr uint32_t ScenePresentBox_openPresentBoxDialog = 0x6CF9E1;
}  // namespace g

// strings
constexpr uint32_t S_DeckBG = 0xE03AF2;            // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_SceneJson = 0xE05A5D;         // "cocostudio/publish/PresentBOXScene.json"
constexpr uint32_t S_LeftUI = 0xDF85F0;
constexpr uint32_t S_CenterUI = 0xE02FBA;
constexpr uint32_t S_Txt_Wording = 0xDF862F;
constexpr uint32_t S_Scroll_Area = 0xDF9B59;
constexpr uint32_t S_Txt_None = 0xE04850;
constexpr uint32_t S_Txt_Stock = 0xE05A85;
constexpr uint32_t S_Txt_Stock_Num_Label = 0xE05A8F;
constexpr uint32_t S_Txt_Stock_All_Label = 0xE05AA3;
constexpr uint32_t S_Txt_Stock_Slash = 0xE05AB7;
constexpr uint32_t S_fmt_d = 0xDF86DE;              // "%d"
constexpr uint32_t S_Button = 0xDFD6D5;
constexpr uint32_t S_Txt_ReceiveAll = 0xE05AC7;
constexpr uint32_t S_Close_Button = 0xDF8D2F;

// text ids
constexpr int32_t TEXT_TICKER = 100620014;
constexpr int32_t TEXT_WORDING = 103500013;
constexpr int32_t TEXT_STOCK = 103500002;
constexpr int32_t TEXT_RECEIVE_ALL = 103500003;
constexpr int32_t TEXT_NONE = 103500009;

// lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_init0 = 0xF67AF4;        // init $_0 {this}: scheduleUpdate
constexpr uint32_t VT_init1 = 0xF67AC8;        // init $_1 {this}: unscheduleUpdate
constexpr uint32_t VT_init2 = 0xF67A70;        // init $_2 {this}: presents received
constexpr uint32_t VT_init2Error = 0xF67A9C;   // init $_2::{lambda(bool)#1}
constexpr uint32_t VT_dialog3 = 0xF67A18;      // openPresentBoxDialog $_3 {done}: presents received
constexpr uint32_t VT_dialog3Error = 0xF67A44; // openPresentBoxDialog $_3::{lambda(bool)#1}
constexpr uint32_t VT_main5 = 0xF679EC;        // {numLabel, scroll, button, scene}: a present was received
constexpr uint32_t VT_main6 = 0xF679C0;        // {scene, scroll}: refresh the "receive all" button
constexpr uint32_t VT_main7 = 0xF67994;        // {button, scroll, scene, scroll}: "receive all" tapped
constexpr uint32_t VT_main8Closed = 0xF678B8;  // $_8::{lambda()#1} {layer, shadow, scene, total, done, cancelled}
constexpr uint32_t VT_main9 = 0xF67968;        // {closeButton}: bool() - can the popup close
constexpr uint32_t VT_main10 = 0xF6793C;       // {closeButton}: close (back key)
constexpr uint32_t VT_main11 = 0xF67910;       // {closeButton}: close button tapped
constexpr uint32_t VT_main12 = 0xF678E4;       // {closeButton}: bool() - can the popup close
constexpr uint32_t VT_main13 = 0xF6788C;       // {$_8 = scene, layer, shadow, total, done}: popup closed

constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t SLOT_init = 0x250;
constexpr uint32_t SLOT_release = 0x04;
constexpr uint32_t V_removeChildByName = 0x134;   // (const string& name, bool cleanup)
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t API_PRESENT_COUNTS = 0xA4;     // shared_ptr<SafeBuffer>: present count, box capacity...
constexpr uint32_t API_PRESENTS = 0x104;          // std::map<u64, ...>* of the presents



// the number of presents the server reported: the first two words of APIManager's counts SafeBuffer, summed
// (the shared_ptr is kept alive in `ctrl` until the caller releases it)
int32_t presentTotal(uint32_t &ctrl, Addr scratch) {
    Addr api = call(g::APIManager_getInstance, {});
    ctrl = ld32(api + API_PRESENT_COUNTS + 4);
    Addr sb = ld32(api + API_PRESENT_COUNTS);
    if (ctrl) guest::addRef(ctrl);
    call(g::SafeBuffer_read, {sb, scratch, ld32(sb + 0xC), 4, 0});
    int32_t first = int32_t(ld32(scratch));
    call(g::SafeBuffer_read, {sb, scratch + 4, ld32(sb + 0xC) + 4, 4, 4});
    return int32_t(ld32(scratch + 4) + uint32_t(first));
}
}  // namespace

// ------------------------------------------------------------------------------------------------ scene
Addr Self::createScene() {
    Addr scene = call(g::Scene_create, {});
    Addr layer = create();
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x6CF654, "ScenePresentBox::createScene", ScenePresentBox::createScene);

Addr Self::create() {
    Addr layer = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {layer});
    Self *self = rw::host_ptr<Self>(layer);
    rw::set<uint32_t>(self, 0, VTABLE);
    self->field_294 = 0;
    self->field_290 = 0;
    self->sceneNode = 0;
    self->field_288 = 0;
    if (init(self) == 1) {
        call(g::Ref_autorelease, {layer});
        return layer;
    }
    vcall(layer, SLOT_release, {layer});
    return 0;
}
RW_FUNCTION(0x6CF678, "ScenePresentBox::create", ScenePresentBox::create);

Self *Self::destroy(Self *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    return rw::host_ptr<Self>(call(g::Layer_dtor, {rw::guest_addr(self)}));
}
RW_FUNCTION(0x6CF6F8, "ScenePresentBox::~ScenePresentBox", ScenePresentBox::destroy);

void Self::destroyDelete(Self *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    call(fn::operator_delete, {call(g::Layer_dtor, {rw::guest_addr(self)})});
}
RW_FUNCTION(0x6CF718, "ScenePresentBox::~ScenePresentBox (deleting)", ScenePresentBox::destroyDelete);

uint32_t Self::init(Self *self) {
    Addr me = rw::guest_addr(self);
    if (call(g::Layer_init, {me}) != 1) return 0;
    withLambda(VT_init0, {me}, [&](Addr f) { call(g::function_vv_assign, {me + 0x210, f}); });
    withLambda(VT_init1, {me}, [&](Addr f) { call(g::function_vv_assign, {me + 0x228, f}); });
    call(g::SceneManager_setBackButtonType, {sceneManager(), 0});
    call(g::SceneManager_showHeader, {sceneManager()});
    {
        Addr mgr = sceneManager();
        withStr(S_DeckBG, 26, [&](Addr s) { call(g::SceneManager_changeBackground, {mgr, 0, s}); });
    }
    {
        Addr mgr = sceneManager();
        Frame f;
        Addr text = textData(f, TEXT_TICKER);
        Addr s = newStr(f, textDataStr(text));
        call(g::SceneManager_changeTickerMessage, {mgr, s});
        strFree(s);
        sharedRelease(text);
    }
    {
        Addr mgr = sceneManager();
        withEmptyFunction([&](Addr f) { call(g::SceneManager_hideTitleHeader, {mgr, f}); });
    }
    call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    Addr api = call(g::APIManager_getInstance, {});
    withLambda(VT_init2, {me}, [&](Addr f) { call(g::APIManager_executeUserPresentGet, {api, f}); });
    return 1;
}
RW_FUNCTION(0x6CF73C, "ScenePresentBox::init", ScenePresentBox::init);

void Self::update(Self *, float) {}
RW_FUNCTION(0x6CF9D8, "ScenePresentBox::update", ScenePresentBox::update);
void Self::runInAnimation(Self *) {}
RW_FUNCTION(0x6CF9DA, "ScenePresentBox::runInAnimation", ScenePresentBox::runInAnimation);
void Self::runOutAnimation(Self *, Addr) {}
RW_FUNCTION(0x6CF9DC, "ScenePresentBox::runOutAnimation", ScenePresentBox::runOutAnimation);

// ------------------------------------------------------------------------------------------------ popup
// Ask the server for the presents, then open the present box popup ($_3); `done` runs when it closes.
void Self::openPresentBoxDialog(Addr done) {
    Frame f;
    Addr doneCopy = f.alloc(24);
    call(g::function_vv_copy, {doneCopy, done});
    call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    Addr api = call(g::APIManager_getInstance, {});
    Addr captured = f.alloc(24);          // the lambda's copy of `done`
    functionCopy(captured, doneCopy);
    Addr callback = f.alloc(24);
    heapFunction(callback, VT_dialog3, 0x20, 8, g::function_vv_move, captured);
    call(g::APIManager_executeUserPresentGet, {api, callback});
    functionDestroy(callback);
    functionDestroy(captured);
    functionDestroy(doneCopy);
}
RW_FUNCTION(0x6CF9E0, "ScenePresentBox::openPresentBoxDialog", ScenePresentBox::openPresentBoxDialog);

// The present box popup on the popup layer: present list (PresentBoxScrollView over the presents' ids), stock
// counts, "receive all" and close buttons. Closing removes it and runs `done` (unless cancelled).
void Self::openPresentBoxDialogMain(Addr done) {
    Frame f;
    Addr scratch = f.alloc(8);
    uint32_t countsCtrl = 0;
    int32_t total = presentTotal(countsCtrl, scratch);
    Addr layer = call(g::SceneManager_getPopupLayer, {sceneManager()});
    Addr shadow = call(g::SceneUtil_createShadowSprite, {});
    uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    Addr scene = withStr(S_SceneJson, 39, [&](Addr s) { return call(g::SceneUtil_createNodeWithSceneFile, {s}); });
    vcall(layer, node::addChild, {layer, scene});
    {
        uint32_t setAnchor = ld32(ld32(scene) + node::setAnchorPoint);
        Frame f2;
        Addr anchor = f2.alloc(8);
        call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        call(setAnchor, {scene, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    uint32_t setPosition = ld32(ld32(scene) + node::setPosition);
    call(setPosition, {scene, vcall(shadow, node::getPosition, {shadow})});
    {
        uint32_t removeByName = ld32(ld32(scene) + V_removeChildByName);
        withStr(S_LeftUI, 6, [&](Addr s) { call(removeByName, {scene, s, 1}); });
    }
    titleLabel(scene, S_Txt_Wording, 11, TEXT_WORDING);

    // the present ids (the keys of APIManager's present map, in order)
    Addr ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    Addr presents = call(g::APIManager_getInstance, {}) + API_PRESENTS;
    Addr node_ = ld32(ld32(presents));
    if (node_ != ld32(presents) + 4) {
        uint32_t end = 0, cap = 0;
        for (;;) {
            Addr key = node_ + 0x10;
            if (end == cap) {
                call(g::vector_u64_pushBackSlow, {ids, key});
            } else {
                uint32_t next = 0;
                if (end) {
                    uint32_t lo = ld32(key), hi = ld32(key + 4);
                    st32(end, lo);
                    st32(end + 4, hi);
                    next = ld32(ids + 4);
                }
                st32(ids + 4, next + 8);
            }
            // in-order successor
            Addr succ;
            Addr right = ld32(node_ + 4);
            if (right) {
                do {
                    succ = right;
                    right = ld32(succ);
                } while (right);
            } else {
                Addr child = node_;
                for (;;) {
                    succ = ld32(child + 8);
                    bool isLeft = ld32(succ) == child;
                    child = succ;
                    if (isLeft) break;
                }
            }
            if (succ == ld32(presents) + 4) break;
            end = ld32(ids + 4);
            cap = ld32(ids + 8);
            node_ = succ;
        }
    }

    Addr scrollArea = findNode(scene, S_Scroll_Area, 11);
    Addr scroll = call(g::PresentBoxScrollView_create, {});
    call(g::IScrollProtocol_setMaxColumn, {scroll, 1});
    call(g::PresentBoxScrollView_setEnabledTapItem, {scroll, 0});
    {
        uint32_t setContentSize = ld32(ld32(scroll) + node::setContentSize);
        call(setContentSize, {scroll, vcall(scrollArea, node::getContentSize, {scrollArea})});
    }
    call(g::BaseScrollView_u64_setItemIDs, {scroll, ids});
    vcall(scrollArea, node::addChild, {scrollArea, scroll});
    {
        Frame f2;
        Addr s = newStr(f2, S_Txt_None, 8);
        Addr none = SceneUtil::searchNodeByName(scene, s);
        vcall(none, node::setVisible, {none, 0});
        strFree(s);
    }
    Addr sceneCapture = f.alloc(8);
    st32(sceneCapture, scene);
    if (ld32(ids + 4) == ld32(ids)) call(0x6D0889, {sceneCapture});   // $_4: show "no presents"
    Addr stock = findNode(scene, S_Txt_Stock, 9);
    Addr stockNum = findNode(scene, S_Txt_Stock_Num_Label, 19);
    Addr stockAll = findNode(scene, S_Txt_Stock_All_Label, 19);
    {
        Frame f2;
        Addr s = newStr(f2, S_Txt_Stock_Slash, 15);
        call(g::SceneUtil_setShadow, {SceneUtil::searchNodeByName(scene, s)});
        strFree(s);
    }
    call(g::SceneUtil_setShadow, {stock});
    call(g::SceneUtil_setShadow, {stockNum});
    call(g::SceneUtil_setShadow, {stockAll});
    setTextId(stock, TEXT_STOCK);
    {
        Frame f2;
        Addr buf = stringBuffer(f2, {S_fmt_d, ld32(ld32(presents) + 0xC)});   // presents held
        Addr s = newStr(f2, buf);
        call(g::Text_setString, {stockNum, s});
        strFree(s);
    }
    {
        Frame f2;
        Addr buf = stringBuffer(f2, {S_fmt_d, ld32(ld32(presents) + 0x10)});  // box capacity
        Addr s = newStr(f2, buf);
        call(g::Text_setString, {stockAll, s});
        strFree(s);
    }
    Addr button = findNode(scene, S_Button, 6);
    Addr receiveAll = findNode(scene, S_Txt_ReceiveAll, 14);
    {
        Addr fo = f.alloc(24);
        st32(fo + 16, 0);
        Addr heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_main5);
            st32(heap + 4, stockNum);
            st32(heap + 8, scroll);
            st32(heap + 12, button);
            st32(heap + 16, scene);
        }
        st32(fo + 16, heap);
        call(g::PresentBoxScrollView_setGetPresentEvent, {scroll, fo});
        functionDestroy(fo);
    }
    withLambda(VT_main6, {scene, scroll}, [&](Addr fo) { call(g::function_vv_assign, {scroll + 0x680, fo}); });
    setTextId(receiveAll, TEXT_RECEIVE_ALL);
    call(g::SceneUtil_setShadow, {receiveAll});
    {
        uint32_t end = ld32(scroll + 0x62C), begin = ld32(scroll + 0x628);
        uint32_t any = end != begin ? 1 : 0;
        call(g::Widget_setBright, {button, any});
        call(g::SceneUtil_setButtonEnabled, {button, receiveAll, any, 0});
    }
    {
        Addr fo = f.alloc(24);
        st32(fo + 16, 0);
        Addr heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_main7);
            st32(heap + 4, button);
            st32(heap + 8, scroll);
            st32(heap + 12, scene);
            st32(heap + 16, scroll);
        }
        st32(fo + 16, heap);
        call(g::Widget_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    // $_8 {scene, layer, shadow, total, done}, then a copy of it
    Addr closed = f.alloc(40);
    st32(closed, scene);
    st32(closed + 4, layer);
    st32(closed + 8, shadow);
    st32(closed + 12, uint32_t(total));
    call(g::function_vv_copy, {closed + 16, done});
    Addr closedCopy = f.alloc(40);
    for (uint32_t k = 0; k < 16; k += 4) st32(closedCopy + k, ld32(closed + k));
    functionCopy(closedCopy + 16, closed + 16);
    Addr closeButton = findNode(scene, S_Close_Button, 12);
    withLambda(VT_main9, {closeButton}, [&](Addr fo) { call(g::function_bv_swap, {fo, scroll + 0x668}); });
    withLambda(VT_main10, {closeButton}, [&](Addr fo) { call(g::function_vv_swap, {fo, scroll + 0x650}); });
    withLambda(VT_main11, {closeButton},
               [&](Addr fo) { call(g::Widget_addTouchEventListener, {closeButton, fo}); });
    withEmptyFunction([&](Addr fo) {
        Addr anim = call(g::SceneUtil_createOpenPopupAnimation, {scene, fo});
        call(g::Node_runAction, {scene, anim});
    });
    Addr mgr = sceneManager();
    Addr canClose = f.alloc(24);
    st32(canClose, VT_main12);
    st32(canClose + 4, closeButton);
    st32(canClose + 16, canClose);
    Addr closedArg = f.alloc(40);
    for (uint32_t k = 0; k < 16; k += 4) st32(closedArg + k, ld32(closedCopy + k));
    functionCopy(closedArg + 16, closedCopy + 16);
    Addr onClosed = f.alloc(24);
    st32(onClosed + 16, 0);
    Addr heap = call(fn::operator_new, {0x30});
    if (heap) {
        st32(heap, VT_main13);
        st32(heap + 8, ld32(closedArg));
        st32(heap + 12, ld32(closedArg + 4));
        st32(heap + 16, ld32(closedArg + 8));
        st32(heap + 20, ld32(closedArg + 12));
        call(g::function_vv_move, {heap + 24, closedArg + 16});
    }
    st32(onClosed + 16, heap);
    call(g::SceneManager_openPopup, {mgr, canClose, onClosed, 1});
    functionDestroy(onClosed);
    functionDestroy(closedArg + 16);
    functionDestroy(canClose);
    functionDestroy(closedCopy + 16);
    functionDestroy(closed + 16);
    call(g::vector_u64_dtor, {ids});
    if (countsCtrl) guest::release(countsCtrl);
}
RW_FUNCTION(0x6CFB58, "ScenePresentBox::openPresentBoxDialogMain", ScenePresentBox::openPresentBoxDialogMain);

// ------------------------------------------------------------------------------------------------ lambdas
struct ScenePresentBoxLambdas {
    // init $_0 / $_1 {scene}
    static void onEnter(Addr func) { call(g::Node_scheduleUpdate, {ld32(func + 4)}); }
    static void onExit(Addr func) { call(g::Node_unscheduleUpdate, {ld32(func + 4)}); }

    // init $_2 {scene}(ok): the presents arrived - load the scene layout (without its CenterUI / LeftUI groups)
    // and open the popup; on failure the API error dialog (if the API wants one)
    static void presentsReceived(Addr func, const uint32_t *okArg) {
        uint8_t ok = uint8_t(*okArg);
        Addr me = ld32(func + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            Self *self = rw::host_ptr<Self>(me);
            Addr node_ = withStr(S_SceneJson, 39, [&](Addr s) {
                Addr n = call(g::SceneUtil_createNodeWithSceneFile, {s});
                self->sceneNode = n;
                return n;
            });
            node_ = self->sceneNode;
            vcall(me, node::addChild, {me, node_});
            {
                Addr n = self->sceneNode;
                uint32_t removeByName = ld32(ld32(n) + V_removeChildByName);
                withStr(S_CenterUI, 8, [&](Addr s) { call(removeByName, {n, s, 1}); });
            }
            {
                Addr n = self->sceneNode;
                uint32_t removeByName = ld32(ld32(n) + V_removeChildByName);
                withStr(S_LeftUI, 6, [&](Addr s) { call(removeByName, {n, s, 1}); });
            }
            withEmptyFunction([&](Addr fo) { call(g::ScenePresentBox_openPresentBoxDialog, {fo}); });
            return;
        }
        if (!ld8(call(g::APIManager_getInstance, {}) + 0x14)) return;
        Addr api = call(g::APIManager_getInstance, {});
        withLambda(VT_init2Error, {}, [&](Addr fo) { call(g::APIManager_openErrorDialog, {api, fo, 1}); });
    }
    // init $_2::{lambda(bool)#1} and openPresentBoxDialog $_3::{lambda(bool)#1}: after the error dialog
    static void errorClosed(Addr, const uint32_t *) {
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    }

    // openPresentBoxDialog $_3 {done}(ok)
    static void dialogPresentsReceived(Addr func, const uint32_t *okArg) {
        uint8_t ok = uint8_t(*okArg);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            Frame f;
            Addr done = f.alloc(24);
            call(g::function_vv_copy, {done, func + 8});
            call(g::ScenePresentBox_openPresentBoxDialogMain, {done});
            functionDestroy(done);
            return;
        }
        if (!ld8(call(g::APIManager_getInstance, {}) + 0x14)) return;
        Addr api = call(g::APIManager_getInstance, {});
        withLambda(VT_dialog3Error, {}, [&](Addr fo) { call(g::APIManager_openErrorDialog, {api, fo, 1}); });
    }

    // $_4 {scene} (called directly): "no presents" text
    static void showNone(const uint32_t *capture) {
        Addr scene = *capture;
        Addr none = findNode(scene, S_Txt_None, 8);
        call(g::SceneUtil_setShadow, {none});
        vcall(none, node::setVisible, {none, 1});
        setTextId(none, TEXT_NONE);
    }

    // $_5 {numLabel, scroll, button, scene}: a present was received - update the count, empty list handling
    static void presentReceived(Addr func) {
        Addr label = ld32(func + 4);
        Addr scroll = ld32(func + 8);
        {
            Frame f;
            uint32_t count = uint32_t(int32_t(ld32(scroll + 0x62C) - ld32(scroll + 0x628)) >> 3);
            Addr buf = stringBuffer(f, {S_fmt_d, count});
            Addr s = newStr(f, buf);
            call(g::Text_setString, {label, s});
            strFree(s);
        }
        scroll = ld32(func + 8);
        if (ld32(scroll + 0x62C) == ld32(scroll + 0x628)) {
            call(g::Widget_setBright, {ld32(func + 12), 0});
            call(0x6D0889, {func + 16});   // $_4
        }
    }

    // $_6 {scene, scroll}: "receive all" is bright / enabled while presents remain
    static void refreshReceiveAll(Addr func) {
        Addr button = findNode(ld32(func + 4), S_Button, 6);
        Addr label = findNode(ld32(func + 4), S_Txt_ReceiveAll, 14);
        Addr scroll = ld32(func + 8);
        uint32_t any = ld32(scroll + 0x62C) == ld32(scroll + 0x628) ? 0 : 1;
        call(g::Widget_setBright, {button, any});
        call(g::SceneUtil_setButtonEnabled, {button, label, any, 0});
    }

    // $_7 {button, scroll, scene, scroll}: "receive all" released while bright
    static void receiveAllTouch(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        if (call(g::Widget_isBright, {ld32(func + 4)}) != 1) return;
        call(g::AudioPlaySystemSEDecide, {});
        Addr scroll = ld32(func + 8);
        withLambda(VT_main6, {ld32(func + 12), ld32(func + 16)},
                   [&](Addr fo) { call(g::PresentBoxScrollView_popUpAllGetPresent, {scroll, fo}); });
    }

    // $_9 / $_12 {closeButton}: the popup may close while the close button is enabled
    static uint32_t canClose(Addr func) { return call(g::Widget_isTouchEnabled, {ld32(func + 4)}); }
    // $_10 {closeButton}: close (back key)
    static void closeByBack(Addr func) {
        call(g::SceneManager_closeCurrentPopup, {sceneManager(), 1});
        Addr button = ld32(func + 4);
        vcall(button, V_setTouchEnabled, {button, 0});
    }
    // $_11 {closeButton}: close button released
    static void closeTouch(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        call(g::AudioPlaySystemSECancel, {});
        call(g::SceneManager_closeCurrentPopup, {sceneManager(), 0});
        Addr button = ld32(func + 4);
        vcall(button, V_setTouchEnabled, {button, 0});
    }

    // $_13 {$_8 = scene, layer, shadow, total, done}(cancelled): run the close animation, then $_8's lambda
    static void popupClosed(Addr func, const uint32_t *cancelledArg) {
        uint8_t cancelled = uint8_t(*cancelledArg);
        Addr scene = ld32(func + 8);
        Frame f;
        Addr lambda = f.alloc(48);   // $_8::{lambda()#1}: layer, shadow, scene, total, done, cancelled
        st32(lambda, ld32(func + 12));
        st32(lambda + 4, ld32(func + 16));
        st32(lambda + 8, scene);
        st32(lambda + 12, ld32(func + 20));
        call(g::function_vv_copy, {lambda + 16, func + 24});
        st8(lambda + 40, cancelled);
        Addr fo = f.alloc(24);
        st32(fo + 16, 0);
        Addr heap = call(fn::operator_new, {0x38});
        if (heap) {
            st32(heap, VT_main8Closed);
            for (uint32_t k = 0; k < 16; k += 4) st32(heap + 8 + k, ld32(lambda + k));
            call(g::function_vv_move, {heap + 24, lambda + 16});
            st8(heap + 48, ld8(lambda + 40));
        }
        st32(fo + 16, heap);
        Addr anim = call(g::SceneUtil_createClosePopupAnimation, {scene, fo});
        call(g::Node_runAction, {scene, anim});
        functionDestroy(fo);
        functionDestroy(lambda + 16);
    }

    // $_8::{lambda()#1} {layer, shadow, scene, total, done, cancelled}: the popup has closed - remove it, start
    // the shop tutorial chains whose present threshold was crossed, run `done` unless cancelled
    static void afterClose(Addr func) {
        Addr layer = ld32(func + 8);
        vcall(layer, node::removeChild, {layer, ld32(func + 12), 1});
        layer = ld32(func + 8);
        vcall(layer, node::removeChild, {layer, ld32(func + 16), 1});
        Frame f;
        Addr scratch = f.alloc(8);
        uint32_t countsCtrl = 0;
        int32_t total = presentTotal(countsCtrl, scratch);
        int32_t before = int32_t(ld32(func + 20));
        Addr misc16 = masterRow(f, g::TutorialMisc_create, 16);
        Addr value = f.alloc(8);
        bool chained = false;
        if (Addr row = ld32(misc16)) {
            call(g::SafeBuffer_read, {row, value, ld32(row + 0xC) + 0xC, 4, 0xC});
            int32_t threshold = int32_t(ld32(value));
            if (threshold && total >= threshold && before < threshold) {
                call(g::SceneManager_ChainOfTutorialShop, {sceneManager(), 41});
                chained = true;
            }
        }
        if (!chained) {
            Addr misc15 = masterRow(f, g::TutorialMisc_create, 15);
            if (Addr row = ld32(misc15)) {
                call(g::SafeBuffer_read, {row, value + 4, ld32(row + 0xC) + 0xC, 4, 0xC});
                int32_t threshold = int32_t(ld32(value + 4));
                if (threshold && total >= threshold && int32_t(ld32(func + 20)) < threshold)
                    call(g::SceneManager_ChainOfTutorialShop, {sceneManager(), 40});
            }
            sharedRelease(misc15);
        }
        sharedRelease(misc16);
        if (countsCtrl) guest::release(countsCtrl);
        if (ld32(func + 40) && !ld8(func + 48)) call(g::function_vv_call, {func + 24});
    }
};

RW_FUNCTION(0x6D1E8A, "ScenePresentBox::init $_0 __func::operator()", ScenePresentBoxLambdas::onEnter);
RW_FUNCTION(0x6D1DEE, "ScenePresentBox::init $_1 __func::operator()", ScenePresentBoxLambdas::onExit);
RW_FUNCTION(0x6D1AA0, "ScenePresentBox::init $_2 __func::operator()", ScenePresentBoxLambdas::presentsReceived);
RW_FUNCTION(0x6D1D4E, "ScenePresentBox::init $_2 UlbE_ __func::operator()", ScenePresentBoxLambdas::errorClosed);
RW_FUNCTION(0x6D1890, "ScenePresentBox::openPresentBoxDialog $_3 __func::operator()", ScenePresentBoxLambdas::dialogPresentsReceived);
RW_FUNCTION(0x6D19FE, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::operator()", ScenePresentBoxLambdas::errorClosed);
RW_FUNCTION(0x6D0888, "ScenePresentBox::openPresentBoxDialogMain $_4::operator()", ScenePresentBoxLambdas::showNone);
RW_FUNCTION(0x6D1624, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::operator()", ScenePresentBoxLambdas::presentReceived);
RW_FUNCTION(0x6D1450, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::operator()", ScenePresentBoxLambdas::refreshReceiveAll);
RW_FUNCTION(0x6D12F4, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::operator()", ScenePresentBoxLambdas::receiveAllTouch);
RW_FUNCTION(0x6D1242, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::operator()", ScenePresentBoxLambdas::canClose);
RW_FUNCTION(0x6D118E, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::operator()", ScenePresentBoxLambdas::closeByBack);
RW_FUNCTION(0x6D10CA, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::operator()", ScenePresentBoxLambdas::closeTouch);
RW_FUNCTION(0x6D102E, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::operator()", ScenePresentBoxLambdas::canClose);
RW_FUNCTION(0x6D0B04, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::operator()", ScenePresentBoxLambdas::popupClosed);
RW_FUNCTION(0x6D0DF4, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::operator()", ScenePresentBoxLambdas::afterClose);

// ------------------------------------------------------------------------------------------------ SceneAlbum
// The bodies of SceneAlbum::init's lambdas sit right before ScenePresentBox and are attributed to it (their
// thunks are in SceneAlbum_thunks.cpp): $_2 {album} goes back, $_1 / $_0 {album} stop / start the update.
struct SceneAlbumInitLambdas {
    static void back(Addr func) { call(0x6CE5E5, {ld32(func + 4)}); }   // SceneAlbum::backScene
    static void onExit(Addr func) { call(g::Node_unscheduleUpdate, {ld32(func + 4)}); }
    static void onEnter(Addr func) { call(g::Node_scheduleUpdate, {ld32(func + 4)}); }
};
RW_FUNCTION(0x6CF4DE, "SceneAlbum::init $_2 __func::operator()", SceneAlbumInitLambdas::back);
RW_FUNCTION(0x6CF57A, "SceneAlbum::init $_1 __func::operator()", SceneAlbumInitLambdas::onExit);
RW_FUNCTION(0x6CF616, "SceneAlbum::init $_0 __func::operator()", SceneAlbumInitLambdas::onEnter);
