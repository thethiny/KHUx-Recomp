// SceneUtil::openAvatarBoardFilterPopup(onClose): the Avatar Board filter (SphereBoard_Filter_ver120). Four
// board-kind checkboxes, five stat checkboxes and "Release" (clear the filter), all bound to the shared
// AvatarBoardFilterState; OK and Close. The touch lambdas ($_88..$_97) are built here; every control stays
// disabled until the open animation ends ($_98 enables them). The close lambda ($_100) keeps a snapshot of the
// state taken on open, to put it back on Close.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_setTextLook = 0x4744F5;      // sub_4744F4(text, on, 0)
constexpr uint32_t AppState_getAvatarBoardFilterState = 0x4EB189;
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;
constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
}  // namespace g

constexpr uint32_t S_FilterJson = 0xDF9BC4;   // "cocostudio/publish/SphereBoard_Filter_ver120.json"
constexpr uint32_t S_Txt_wording_Label = 0xDF9BF6, S_Txt_Filter_3 = 0xDF9C08, S_Txt_Filter_4 = 0xDF9C15;
constexpr uint32_t S_Txt_AP = 0xDF93C8, S_Txt_HP = 0xDF9C22, S_Txt_Cost = 0xDF9C29, S_Txt_AVT = 0xDF9C32,
                   S_Txt_MDL = 0xDF9C3A;
constexpr uint32_t S_CheckBox_Filter_1 = 0xDF9C42, S_CheckBox_Filter_2 = 0xDF9C54, S_CheckBox_Filter_3 = 0xDF9C66,
                   S_CheckBox_Filter_4 = 0xDF9C78;
constexpr uint32_t S_CheckBox_AP = 0xDF9C8A, S_CheckBox_HP = 0xDF9C96, S_CheckBox_Cost = 0xDF9CA2,
                   S_CheckBox_AVT = 0xDF9CB0, S_CheckBox_MDL = 0xDF9CBD;
constexpr uint32_t S_Button_Release = 0xDF9CCA, S_Txt_Release = 0xDF9CD9;
constexpr uint32_t S_Button_OK = 0xDF8688, S_Txt_OK = 0xDF869F, S_Close_Button = 0xDF8D2F;
constexpr int32_t TXT_WORDING = 103400054, TXT_FILTER_3 = 103400052, TXT_FILTER_4 = 103400053;
constexpr int32_t TXT_AP = 103400007, TXT_HP = 103400006, TXT_COST = 103400008, TXT_AVT = 103400009,
                  TXT_MDL = 103400010;
constexpr int32_t TXT_RELEASE = 102800001, TXT_OK = 100100001;

constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C;                 // ui::Widget::setTouchEnabled

// the lambdas' __func vtables
constexpr uint32_t VT_Release = 0xF541F0;                      // 0x40
constexpr uint32_t VT_Filter1 = 0xF541C4, VT_Filter2 = 0xF54198, VT_Filter3 = 0xF5416C;   // 0x1C
constexpr uint32_t VT_Filter4 = 0xF54140;                      // 0x30
constexpr uint32_t VT_AP = 0xF54114, VT_HP = 0xF540E8, VT_Cost = 0xF540BC, VT_AVT = 0xF54090,
                   VT_MDL = 0xF54064;                          // 0x20
constexpr uint32_t VT_Ok = 0xF54038;                           // no captures
constexpr uint32_t VT_Opened = 0xF53FE0;                       // 0x34: the 12 controls
constexpr uint32_t VT_CanClose = 0xF53FB4;                     // {close}
constexpr uint32_t VT_Closed = 0xF53F5C;                       // 0x78

// AvatarBoardFilterState: ten bools (0 the filter is on, 1..4 the board kinds, 5 AVT, 6 AP, 7 HP, 8 Cost, 9 MDL)
constexpr uint32_t ON = 0, KIND_3 = 1, KIND_4 = 2, KIND_1 = 3, KIND_2 = 4, AVT = 5, AP = 6, HP = 7, COST = 8,
                   MDL = 9;

void addRef(uint32_t ctrl) {
    if (ctrl) call(fn::shared_add, {ctrl});
}
void release(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}

// a label with the text shadow and a TextData string
void shadowedLabel(uint32_t popup, uint32_t name, uint32_t len, int32_t id) {
    uint32_t text = findNode(popup, name, len);
    call(g::SceneUtil_setShadow, {text});
    setTextId(text, id);
}

struct AvatarBoardFilterPopup {
    // widget->addTouchEventListener(heap lambda `words`, preceded by the state shared_ptr copied twice)
    static void addSharedTouch(uint32_t widget, uint32_t vt, uint32_t size, uint32_t state, uint32_t ctrl,
                               uint32_t ctrl2, std::initializer_list<uint32_t> words) {
        addRef(ctrl);
        addRef(ctrl2);
        Frame f;
        uint32_t cb = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {size});
        if (heap) {
            st32(heap, vt);
            st32(heap + 4, state);
            st32(heap + 8, ctrl);
            uint32_t k = 12;
            for (uint32_t w : words) { st32(heap + k, w); k += 4; }
        }
        st32(cb + 16, heap);
        call(fn::Widget_addTouchEventListener, {widget, cb});
        functionDestroy(cb);
        if (!heap) {
            release(ctrl2);
            release(ctrl);
        }
    }

    static void open(uint32_t onClose) {
        call(fn::AudioPlaySystemSEPopUp, {});
        uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
        uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
        uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.15f), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
        uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_FilterJson});
        vcall(shadow, node::addChild, {shadow, popup});
        Frame f;
        {
            Frame t;
            uint32_t anchor = t.alloc(8);
            call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
            vcall(popup, node::setAnchorPoint, {popup, anchor});
            call(g::Vec2_dtor, {anchor});
        }
        vcall(popup, node::setPosition, {popup, vcall(shadow, node::getPosition, {shadow})});

        uint32_t sp = f.alloc(8);
        call(g::AppState_getAvatarBoardFilterState, {sp, call(fn::AppState_getInstance, {})});
        uint32_t state = ld32(sp), ctrl = ld32(sp + 4);
        // the snapshot for Close, in the lambda's order
        uint8_t saved[10] = {ld8(state + ON),  ld8(state + KIND_3), ld8(state + KIND_4), ld8(state + KIND_1),
                             ld8(state + KIND_2), ld8(state + AP), ld8(state + HP), ld8(state + COST),
                             ld8(state + AVT), ld8(state + MDL)};

        shadowedLabel(popup, S_Txt_wording_Label, 17, TXT_WORDING);
        shadowedLabel(popup, S_Txt_Filter_3, 12, TXT_FILTER_3);
        shadowedLabel(popup, S_Txt_Filter_4, 12, TXT_FILTER_4);
        shadowedLabel(popup, S_Txt_AP, 6, TXT_AP);
        shadowedLabel(popup, S_Txt_HP, 6, TXT_HP);
        shadowedLabel(popup, S_Txt_Cost, 8, TXT_COST);
        shadowedLabel(popup, S_Txt_AVT, 7, TXT_AVT);
        shadowedLabel(popup, S_Txt_MDL, 7, TXT_MDL);
        uint32_t kind1 = findNode(popup, S_CheckBox_Filter_1, 17);
        uint32_t kind2 = findNode(popup, S_CheckBox_Filter_2, 17);
        uint32_t kind3 = findNode(popup, S_CheckBox_Filter_3, 17);
        uint32_t kind4 = findNode(popup, S_CheckBox_Filter_4, 17);
        uint32_t ap = findNode(popup, S_CheckBox_AP, 11);
        uint32_t hp = findNode(popup, S_CheckBox_HP, 11);
        uint32_t cost = findNode(popup, S_CheckBox_Cost, 13);
        uint32_t avt = findNode(popup, S_CheckBox_AVT, 12);
        uint32_t mdl = findNode(popup, S_CheckBox_MDL, 12);
        uint32_t releaseButton = findNode(popup, S_Button_Release, 14);
        titleLabel(releaseButton, S_Txt_Release, 11, TXT_RELEASE);
        uint32_t releaseText = findNode(releaseButton, S_Txt_Release, 11);

        uint32_t ctrlA = ctrl;   // three more owners of the state for the lambdas below
        addRef(ctrlA);
        auto flag = [&](uint32_t i) { return uint32_t(ld8(state + i) != 0); };
        call(g::CheckBox_setSelectedState, {kind1, flag(KIND_1)});
        call(g::CheckBox_setSelectedState, {kind2, flag(KIND_2)});
        call(g::CheckBox_setSelectedState, {kind3, flag(KIND_3)});
        call(g::CheckBox_setSelectedState, {kind4, flag(KIND_4)});
        call(g::CheckBox_setSelectedState, {ap, flag(AP)});
        call(g::CheckBox_setSelectedState, {hp, flag(HP)});
        call(g::CheckBox_setSelectedState, {cost, flag(COST)});
        call(g::CheckBox_setSelectedState, {avt, flag(AVT)});
        call(g::CheckBox_setSelectedState, {mdl, flag(MDL)});
        uint32_t ctrlB = ctrl, ctrlC = ctrl;
        addRef(ctrlB);
        addRef(ctrlC);
        call(g::Widget_setBright, {releaseButton, flag(ON)});

        addSharedTouch(releaseButton, VT_Release, 0x40, state, ctrl, ctrlA,
                       {releaseButton, releaseText, kind1, state, ctrlA, kind2, kind3, kind4, ap, hp, cost, avt, mdl});
        for (auto [box, vt] : {std::pair{kind1, VT_Filter1}, {kind2, VT_Filter2}, {kind3, VT_Filter3}})
            addSharedTouch(box, vt, 0x1C, state, ctrl, ctrlB, {state, ctrlB, releaseButton, releaseText});
        addSharedTouch(kind4, VT_Filter4, 0x30, state, ctrl, ctrlB,
                       {ap, hp, cost, avt, mdl, state, ctrlB, releaseButton, releaseText});
        for (auto [box, vt] : {std::pair{ap, VT_AP}, {hp, VT_HP}, {cost, VT_Cost}, {avt, VT_AVT}, {mdl, VT_MDL}})
            addSharedTouch(box, vt, 0x20, state, ctrl, ctrlB, {state, ctrlB, kind4, releaseButton, releaseText});
        for (uint32_t box : {kind1, kind2, kind3, kind4, ap, hp, cost, avt, mdl})
            vcall(box, V_SET_TOUCH_ENABLED, {box, 0});

        uint32_t ok = findNode(popup, S_Button_OK, 9);
        addTouch(ok, VT_Ok, {});
        titleLabel(ok, S_Txt_OK, 6, TXT_OK);
        vcall(ok, V_SET_TOUCH_ENABLED, {ok, 0});
        vcall(releaseButton, V_SET_TOUCH_ENABLED, {releaseButton, 0});
        uint32_t on = flag(ON);
        call(g::Widget_setBright, {releaseButton, on});
        vcall(releaseButton, V_SET_TOUCH_ENABLED, {releaseButton, on});
        call(g::SceneUtil_setTextLook, {releaseText, on, 0});
        uint32_t close = findNode(popup, S_Close_Button, 12);
        vcall(close, V_SET_TOUCH_ENABLED, {close, 0});

        // $_100's captures, first as the lambda object (state copied, onClose copied)
        uint32_t ctrlD = ctrl;
        addRef(ctrlD);
        uint32_t closeFn = f.alloc(20);
        call(fn::function_vv_copy, {closeFn, onClose});
        {
            Frame t;
            uint32_t opened = newEmptyFunction(t);
            uint32_t heap = call(fn::operator_new, {0x34});
            if (heap) {
                st32(heap, VT_Opened);
                uint32_t k = 4;
                for (uint32_t w : {ok, releaseButton, close, kind1, kind2, kind3, kind4, ap, hp, cost, avt, mdl}) {
                    st32(heap + k, w);
                    k += 4;
                }
            }
            st32(opened + 16, heap);
            call(fn::Node_runAction, {popup, call(g::SceneUtil_createOpenPopupAnimation, {popup, opened})});
            functionDestroy(opened);
        }
        uint32_t mgr = sceneManager();
        {
            Frame t;
            uint32_t canClose = newFunction(t, VT_CanClose, {close});
            // the lambda passed on: another copy of the state and of onClose
            uint32_t ctrlE = ctrl;
            addRef(ctrlE);
            uint32_t fnCopy = t.alloc(20);
            functionCopy(fnCopy, closeFn);
            uint32_t closed = newEmptyFunction(t);
            uint32_t heap = call(fn::operator_new, {0x78});
            if (heap) {
                st32(heap, VT_Closed);
                uint32_t k = 8;
                for (uint32_t w : {close, releaseButton, ok, kind1, kind2, kind3, kind4, ap, hp, cost, avt, mdl}) {
                    st32(heap + k, w);
                    k += 4;
                }
                st32(heap + 0x38, state);
                st32(heap + 0x3C, ctrlE);
                ctrlE = 0;
                for (uint32_t i = 0; i < 10; i++) st8(heap + 0x40 + i, saved[i]);
                call(fn::function_vv_move, {heap + 0x50, fnCopy});
                st32(heap + 0x68, shadow);
                st32(heap + 0x6C, popup);
                st32(heap + 0x70, layer);
            }
            st32(closed + 16, heap);
            call(fn::SceneManager_openPopup, {mgr, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(fnCopy);
            release(ctrlE);
            functionDestroy(canClose);
        }
        functionDestroy(closeFn);
        release(ctrlD);
        release(ctrlC);
        release(ctrlB);
        release(ctrlA);
        release(ctrl);
    }
};
}  // namespace

RW_FUNCTION(0x47546C, "SceneUtil::openAvatarBoardFilterPopup(function<void()>)", AvatarBoardFilterPopup::open);
