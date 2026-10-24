// SceneSphereBoard: one Avatar Board (the sphere grid, the selected reward's texts, the open button, the board's
// incentive / board / kind images). A cocos2d::Layer with an IInOutAnimation at +0x288.
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
namespace a {
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base, Layer_dtor = rwlib::cocos2d::Layer::dtor_base, Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;          // function<void()>::operator=(const function&)
constexpr uint32_t FunctionVB_move = 0x47BC69;            // function<void(bool)>::function(function&&)
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneManager_setFinishSceneCallback = 0x613E3F;
constexpr uint32_t SceneManager_hideInfoHeader = 0x612F4D;
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_removeBackground = 0x611691;
constexpr uint32_t SceneManager_setBackButtonType = 0x613E27;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;   // (type, const string&)
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneManager_isOpenedPopup = 0x621519;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;   // (function<Layer*()>, bool, bool, FadeType)
constexpr uint32_t audio_getInstance = 0x5BECFD, audio_load = 0x5BEEF5;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;
constexpr uint32_t SceneUtil_backBtnSetPosY = 0x45AFB5;
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;    // (Node*, float, float, function<void()>)
constexpr uint32_t SceneUtil_createLeftOutAnimation = 0x45B735;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;   // (sret function, id, function<void()>, w, w, w, bool)
constexpr uint32_t SphereUI_create = 0x6BF039, SphereUI_SetGrid = 0x793ABD;
constexpr uint32_t SphereUI_isOpenIncentive = 0x79597D, SphereUI_createBoard = 0x793E39;
constexpr uint32_t ConnectingSphere_getUserData = 0x798B99, ConnectingSphere_executeSphereOpenlist = 0x798EC9;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__PNS_9Texture2DE;        // (Texture2D*)
constexpr uint32_t Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache;
constexpr uint32_t TextureCache_addImage = rwlib::cocos2d::TextureCache::addImage;
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Sphere_create = 0x78CF21;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t loadTextureIncentive = 0x6BF121, loadTextureBoard = 0x6BF1ED, loadTextureKind = 0x6BF2B9;
constexpr uint32_t openTutorial30 = 0x6BF781;
}  // namespace a

constexpr uint32_t SIZE = 0x2B0;
constexpr uint32_t VTABLE = 0xF9E858, VTABLE_INOUT = 0xF9EB68;
// fields
constexpr uint32_t F_INOUT = 0x288;        // IInOutAnimation
constexpr uint32_t F_SPHERE_UI = 0x28C;    // SphereUI* (retained)
constexpr uint32_t F_BOARD = 0x290;
constexpr uint32_t F_ROOT = 0x294;         // the SphereScene layout
constexpr uint32_t F_29C = 0x29C, F_2A4 = 0x2A4;
constexpr uint32_t F_CAN_BACK = 0x2A8;
// SphereUI fields used here
constexpr uint32_t U_SPHERE_DATA = 0x14, U_READY = 0x594, U_TXT_POINT = 0x598, U_TXT_TYPE = 0x59C;
constexpr uint32_t U_TXT_NAME = 0x5A0, U_RELEASE_BUTTON = 0x5A4, U_MODE = 0x5A8;

// lambda vtables
constexpr uint32_t VT_IN_DONE = 0xF66C58, VT_OUT_DONE = 0xF66C2C, VT_BACK_KEY = 0xF66C00, VT_FINISH = 0xF66BD4;
constexpr uint32_t VT_RELEASE = 0xF66B7C, VT_BACK_BUTTON = 0xF66B50, VT_BACK_DONE = 0xF66AA0;
constexpr uint32_t VT_OUT_MAIN = 0xF66ACC;  // heap: vtable, -, std::function<void()> at +8
// the grid's widget names (5 x 9 pointers)
constexpr uint32_t GRID_NAMES = 0xF669E4;
// GOT entries of the scene-change lambda: __func<Layer*(*)()> vtable, createSceneLayer<SceneSphereBoardSelect>
constexpr uint32_t GOT_LAYER_FUNC_VT = 0xFD6074, GOT_CREATE_BOARD_SELECT = 0xFD68A0;

// strings
constexpr uint32_t S_SceneJson = 0xE05669, S_BG = 0xE04BF3, S_DeckBG = 0xE03AF2, S_TxtIncentive = 0xE05694,
                   S_TxtIncentiveName = 0xE056A8, S_TxtCoin = 0xE04769, S_TxtCoinName = 0xE0560A,
                   S_PointIcon = 0xE055BF, S_PointIconOC = 0xE055CF, S_PointIconEV = 0xE055E2,
                   S_PointIconRaid = 0xE055F5, S_TxtRelease = 0xDF9CD9, S_Button = 0xDFD6D5,
                   S_ButtonBack = 0xE03DBA, S_PercentD = 0xDF86DE, S_TxtBoard = 0xE05577, S_SelectBase = 0xDF94D5,
                   S_SelectBoard = 0xDF94E1, S_SelectKind = 0xDF94EE, S_Grid = 0xE056C1, S_GridR = 0xE056C6,
                   S_Info = 0xE056CD, S_LeftUI = 0xDF85F0;
constexpr uint32_t S_IncentivePng = 0xDF94FA, S_BoardPng = 0xDF9516, S_KindPng = 0xDF9536;

uint32_t sm() { return call(a::SceneManager_getInstance, {}); }

uint32_t findNode(uint32_t parent, uint32_t name, uint32_t len) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    const uint32_t n = call(a::SceneUtil_searchNodeByName, {parent, s});
    strFree(s);
    return n;
}

void setTextC(uint32_t text, uint32_t chars) {
    Frame f;
    const uint32_t s = f.alloc(12);
    strInit(s, chars, call(a::STRLEN, {chars}));
    call(fn::Text_setString, {text, s});
    strFree(s);
}

void setTextData(uint32_t text, uint32_t id) {
    Frame f;
    const uint32_t td = f.alloc(8);
    call(fn::TextData_get, {td, 0, id});
    setTextC(text, ld32(ld32(td) + 4));
    sharedRelease(td);
}

void tickerData(uint32_t id) {
    const uint32_t m = sm();
    Frame f;
    const uint32_t td = f.alloc(8);
    call(fn::TextData_get, {td, 0, id});
    const uint32_t chars = ld32(ld32(td) + 4);
    const uint32_t s = f.alloc(12);
    strInit(s, chars, call(a::STRLEN, {chars}));
    call(a::SceneManager_changeTickerMessage, {m, s});
    strFree(s);
    sharedRelease(td);
}

void showNode(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// a widget slides in from / out to the left (the out animation of the last one carries `done`)
void slide(uint32_t root, uint32_t name, uint32_t len, uint32_t animation, uint32_t done) {
    const uint32_t node = findNode(root, name, len);
    call(fn::Node_runAction, {node, call(a::EaseSineOut_create, {rw::call(animation, {node, 0, 0, done})})});
}

uint32_t loadTexture(uint32_t fmt, uint32_t id) {
    const uint32_t cache = call(a::Director_getTextureCache, {call(fn::Director_getInstance, {})});
    Frame f;
    const uint32_t buf = f.alloc(0x404);
    call(a::StringBuffer_ctor, {buf, fmt, id});
    const uint32_t s = f.alloc(12);
    strInit(s, buf, call(a::STRLEN, {buf}));
    const uint32_t tex = call(a::TextureCache_addImage, {cache, s});
    strFree(s);
    return tex;
}
}  // namespace

struct SceneSphereBoard {
    static uint32_t create();
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static uint32_t loadTextureIncentive(uint32_t self, uint32_t id) { (void)self; return loadTexture(S_IncentivePng, id); }
    static uint32_t loadTextureBoard(uint32_t self, uint32_t id) { (void)self; return loadTexture(S_BoardPng, id); }
    static uint32_t loadTextureKind(uint32_t self, uint32_t id) { (void)self; return loadTexture(S_KindPng, id); }
    static uint32_t boardMaking(uint32_t self, uint32_t board);
    static void openTutorial30(uint32_t self, uint32_t done);
    static void runInAnimation(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void toBoardSelect(uint32_t self);
    static void openPicked(uint32_t self, uint32_t done);
};

uint32_t SceneSphereBoard::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(a::Layer_ctor, {me});
    st32(me, VTABLE);
    st32(me + F_INOUT, VTABLE_INOUT);
    st32(me + F_SPHERE_UI, 0);
    st32(me + F_ROOT, 0);
    st8(me + 0x298, 0);
    st8(me + 0x299, 0);
    st32(me + F_2A4, 0);
    st8(me + F_CAN_BACK, 0);
    if (vcall(me, 0x248, {me}) == 1) {            // init
        call(a::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

uint32_t SceneSphereBoard::destruct(uint32_t self) {
    st32(self, VTABLE);
    st32(self + F_INOUT, VTABLE_INOUT);
    if (ld32(self + F_SPHERE_UI)) {
        call(a::Ref_release, {ld32(self + F_SPHERE_UI)});
        st32(self + F_SPHERE_UI, 0);
    }
    call(a::Layer_dtor, {self});
    return self;
}

uint32_t SceneSphereBoard::init(uint32_t self) {
    st32(self + F_29C, 0);
    if (call(a::Layer_init, {self}) != 1) return 0;
    Frame f;
    {
        const uint32_t fo = newFunction(f, VT_IN_DONE, {self});
        call(a::FunctionVV_assign, {self + 0x210, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t fo = newFunction(f, VT_OUT_DONE, {self});
        call(a::FunctionVV_assign, {self + 0x228, fo});
        functionDestroy(fo);
    }
    st8(self + F_CAN_BACK, 1);
    {
        const uint32_t m = sm();
        const uint32_t fo = newFunction(f, VT_BACK_KEY, {self});
        call(a::SceneManager_setOnBackKeyCallback, {m, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t m = sm();
        const uint32_t fo = newFunction(f, VT_FINISH, {self});
        call(a::SceneManager_setFinishSceneCallback, {m, fo});
        functionDestroy(fo);
    }
    call(a::audio_load, {call(a::audio_getInstance, {}), 509000510});
    {
        const uint32_t m = sm();
        const uint32_t fo = newEmptyFunction(f);
        call(a::SceneManager_hideInfoHeader, {m, fo});
        functionDestroy(fo);
    }
    call(a::SceneManager_showHeader, {sm()});
    call(a::SceneManager_removeBackground, {sm()});
    call(a::SceneManager_setBackButtonType, {sm(), 0});
    {
        const uint32_t s = newStr(f, S_SceneJson, 0x2A);
        st32(self + F_ROOT, call(a::SceneUtil_createNodeWithSceneFile, {s}));
        strFree(s);
    }
    vcall(self, node::addChild, {self, ld32(self + F_ROOT)});
    if (findNode(ld32(self + F_ROOT), S_BG, 2)) {
        const uint32_t root = ld32(self + F_ROOT);
        const uint32_t s = newStr(f, S_BG, 2);
        vcall(root, 0x134, {root, s, 1});            // removeChildByName(name, cleanup)
        strFree(s);
    }
    {
        const uint32_t m = sm();
        const uint32_t s = newStr(f, S_DeckBG, 0x1A);
        call(a::SceneManager_changeBackground, {m, 0, s});
        strFree(s);
    }
    const uint32_t ui = call(a::SphereUI_create, {});
    st32(self + F_SPHERE_UI, ui);
    call(a::Ref_retain, {ui});
    const uint32_t mode = ld32(ld32(self + F_SPHERE_UI) + U_MODE);
    tickerData(mode == 3 ? 100620031 : mode == 4 ? 100620032 : mode == 5 ? 100620044 : 100620030);

    for (uint32_t row = 0; row < 5; row++)
        for (uint32_t col = 0; col < 9; col++) {
            const uint32_t sui = ld32(self + F_SPHERE_UI), root = ld32(self + F_ROOT);
            const uint32_t name = ld32(GRID_NAMES + 4 * (row * 9 + col));
            const uint32_t widget = findNode(root, name, call(a::STRLEN, {name}));
            call(a::SphereUI_SetGrid, {sui, row, col, widget});
        }
    const uint32_t root = ld32(self + F_ROOT);
    st32(ld32(self + F_SPHERE_UI) + U_TXT_TYPE, findNode(root, S_TxtIncentive, 0x13));
    call(a::SceneUtil_setShadow, {ld32(ld32(self + F_SPHERE_UI) + U_TXT_TYPE)});
    st32(ld32(self + F_SPHERE_UI) + U_TXT_NAME, findNode(ld32(self + F_ROOT), S_TxtIncentiveName, 0x18));
    call(a::SceneUtil_setShadow, {ld32(ld32(self + F_SPHERE_UI) + U_TXT_NAME)});
    st32(ld32(self + F_SPHERE_UI) + U_TXT_POINT, findNode(ld32(self + F_ROOT), S_TxtCoin, 0xE));
    call(a::SceneUtil_setShadow, {ld32(ld32(self + F_SPHERE_UI) + U_TXT_POINT)});
    const uint32_t coinName = findNode(ld32(self + F_ROOT), S_TxtCoinName, 0x13);
    call(a::SceneUtil_setShadow, {coinName});
    const uint32_t icon = findNode(ld32(self + F_ROOT), S_PointIcon, 0xF);
    const uint32_t iconOC = findNode(ld32(self + F_ROOT), S_PointIconOC, 0x12);
    const uint32_t iconEV = findNode(ld32(self + F_ROOT), S_PointIconEV, 0x12);
    const uint32_t iconRaid = findNode(ld32(self + F_ROOT), S_PointIconRaid, 0x14);
    showNode(icon, 0);
    showNode(iconOC, 0);
    showNode(iconEV, 0);
    showNode(iconRaid, 0);
    // the board's currency: its icon and name
    switch (ld32(ld32(self + F_SPHERE_UI) + U_MODE)) {
    case 3: showNode(iconOC, 1); setTextData(coinName, 103400002); break;
    case 4: showNode(iconEV, 1); setTextData(coinName, 103400003); break;
    case 5: showNode(iconRaid, 1); setTextData(coinName, 103400027); break;
    default: showNode(icon, 1); setTextData(coinName, 103400001); break;
    }
    const uint32_t release = findNode(ld32(self + F_ROOT), S_TxtRelease, 0xB);
    setTextData(release, 103400005);
    st32(ld32(self + F_SPHERE_UI) + U_RELEASE_BUTTON, findNode(ld32(self + F_ROOT), S_Button, 6));
    const uint32_t open = call(a::SphereUI_isOpenIncentive, {ld32(self + F_SPHERE_UI), 0});
    call(a::SceneUtil_setButtonEnabled, {ld32(ld32(self + F_SPHERE_UI) + U_RELEASE_BUTTON), release, open ? 0u : 1u, 0});
    {
        const uint32_t button = ld32(ld32(self + F_SPHERE_UI) + U_RELEASE_BUTTON);
        const uint32_t fo = newFunction(f, VT_RELEASE, {self});
        call(a::Widget_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t back = findNode(ld32(self + F_ROOT), S_ButtonBack, 0xB);
        call(a::SceneUtil_backBtnSetPosY, {back});
        const uint32_t fo = newFunction(f, VT_BACK_BUTTON, {self});
        call(a::Widget_addTouchEventListener, {back, fo});
        functionDestroy(fo);
    }
    vcall(self, 0x300, {self});
    return 1;
}

// builds board `board`: the player's state, the grid, the points, the board name and its three images
uint32_t SceneSphereBoard::boardMaking(uint32_t self, uint32_t board) {
    st32(self + F_BOARD, board);
    st8(ld32(self + F_SPHERE_UI) + U_READY, 1);
    call(a::ConnectingSphere_getUserData, {ld32(ld32(self + F_SPHERE_UI) + U_SPHERE_DATA), board});
    call(a::SphereUI_createBoard, {ld32(self + F_SPHERE_UI), ld32(self + F_BOARD)});
    Frame f;
    const uint32_t buf = f.alloc(0x404);
    {
        const uint32_t sui = ld32(self + F_SPHERE_UI);
        const uint32_t text = ld32(sui + U_TXT_POINT);
        call(a::StringBuffer_ctor, {buf, S_PercentD, ld32(ld32(sui + U_SPHERE_DATA) + 0x88)});
        setTextC(text, buf);
    }
    const uint32_t sphere = f.alloc(8);
    call(a::Sphere_create, {sphere, board});
    const uint32_t label = findNode(ld32(self + F_ROOT), S_TxtBoard, 0x15);
    call(a::SceneUtil_setShadow, {label});
    const uint32_t sb = ld32(sphere);
    const uint32_t name = f.alloc(0x84);
    rw::call(a::SafeBuffer_read, {sb, name, ld32(sb + 0xC) + 4, 0x81, 4});
    call(a::StringBuffer_ctor, {buf, name});        // the name is the format (as the original)
    setTextC(label, buf);
    const uint32_t v = f.alloc(4);
    rw::call(a::SafeBuffer_read, {sb, v, ld32(sb + 0xC), 4, 0});
    const uint32_t incentive = ld32(v);
    rw::call(a::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + 0x8C, 4, 0x8C});
    const uint32_t boardImage = ld32(v);
    rw::call(a::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + 0x88, 4, 0x88});
    const uint32_t kind = ld32(v);
    const uint32_t base = findNode(ld32(self + F_ROOT), S_SelectBase, 0xB);
    const uint32_t boardView = findNode(ld32(self + F_ROOT), S_SelectBoard, 0xC);
    const uint32_t kindView = findNode(ld32(self + F_ROOT), S_SelectKind, 0xB);
    uint32_t tex = call(a::loadTextureIncentive, {self, incentive});
    if (!tex) tex = call(a::loadTextureIncentive, {0, 2100001});
    call(a::ImageView_loadTexture, {base, tex});
    tex = call(a::loadTextureBoard, {self, boardImage});
    if (!tex) tex = call(a::loadTextureBoard, {0, 1});
    call(a::ImageView_loadTexture, {boardView, tex});
    if (kind) {
        tex = call(a::loadTextureKind, {self, kind});
        if (!tex) tex = call(a::loadTextureKind, {0, 1});
        call(a::ImageView_loadTexture, {kindView, tex});
        showNode(kindView, 1);
    } else {
        showNode(kindView, 0);
    }
    const uint32_t none = newEmptyFunction(f);
    call(a::openTutorial30, {self, none});
    functionDestroy(none);
    sharedRelease(sphere);
    return 1;
}

void SceneSphereBoard::openTutorial30(uint32_t self, uint32_t done) {
    (void)self;
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    const uint32_t result = f.alloc(20);
    rw::call(a::SceneUtil_openTutorialPopup, {result, 30, copy, 0, 0, 0, 0});
    functionDestroy(result);
    functionDestroy(copy);
}

void SceneSphereBoard::runInAnimation(uint32_t self) {
    Frame f;
    for (auto [name, len] : {std::pair<uint32_t, uint32_t>{S_Grid, 4}, {S_GridR, 6}, {S_Info, 4}, {S_LeftUI, 6}}) {
        const uint32_t root = ld32(self + F_ROOT);
        const uint32_t node = findNode(root, name, len);
        const uint32_t none = newEmptyFunction(f);
        call(fn::Node_runAction,
             {node, call(a::EaseSineOut_create, {rw::call(a::SceneUtil_createLeftInAnimation, {node, 0, 0, none})})});
        functionDestroy(none);
    }
}

// the board slides out; `done` runs when the LeftUI has left (a heap lambda holding a moved copy of it)
void SceneSphereBoard::runOutAnimationMain(uint32_t self, uint32_t done) {
    call(a::SceneManager_setCanTouchHeader, {sm(), 0});
    Frame f;
    for (auto [name, len] : {std::pair<uint32_t, uint32_t>{S_Grid, 4}, {S_GridR, 6}, {S_Info, 4}}) {
        const uint32_t node = findNode(ld32(self + F_ROOT), name, len);
        const uint32_t none = newEmptyFunction(f);
        call(fn::Node_runAction,
             {node, call(a::EaseSineOut_create, {rw::call(a::SceneUtil_createLeftOutAnimation, {node, 0, 0, none})})});
        functionDestroy(none);
    }
    const uint32_t left = findNode(ld32(self + F_ROOT), S_LeftUI, 6);
    const uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, VT_OUT_MAIN);
        call(fn::function_vv_move, {obj + 8, copy});
    }
    st32(fo + 16, obj);
    call(fn::Node_runAction,
         {left, call(a::EaseSineOut_create, {rw::call(a::SceneUtil_createLeftOutAnimation, {left, 0, 0, fo})})});
    functionDestroy(fo);
    functionDestroy(copy);
}

// the back button: closes a popup first, else leaves the board (out animation, then back to the board list)
void SceneSphereBoard::backScene(uint32_t self) {
    if (!ld8(self + F_CAN_BACK)) return;
    if (call(a::SceneManager_isOpenedPopup, {sm()}) == 1) {
        call(a::SceneManager_closeCurrentPopup, {sm(), 0});
        return;
    }
    st8(self + F_CAN_BACK, 0);
    st8(sm() + 0x308, 0);
    Frame f;
    const uint32_t done = newFunction(f, VT_BACK_DONE, {});
    call(a::IInOutAnimation_runOutAnimation, {self + F_INOUT, done});
    functionDestroy(done);
}

// lambda: to the board list (changeSceneLayer(createSceneLayer<SceneSphereBoardSelect>, true, true, 1))
void SceneSphereBoard::toBoardSelect(uint32_t self) {
    (void)self;
    const uint32_t m = sm();
    Frame f;
    const uint32_t fo = newFunction(f, ld32(GOT_LAYER_FUNC_VT) + 8, {ld32(GOT_CREATE_BOARD_SELECT)});
    rw::call(a::SceneManager_changeSceneLayer, {m, fo, 1, 1, 1});
    functionDestroy(fo);
}

// lambda (the open popup's OK): sends the picked spheres; `done` (moved in, then copied) is the request callback
void SceneSphereBoard::openPicked(uint32_t self, uint32_t done) {
    Frame f;
    const uint32_t moved = f.alloc(20);
    call(a::FunctionVB_move, {moved, done});
    const uint32_t data = ld32(ld32(ld32(self + 4) + F_SPHERE_UI) + U_SPHERE_DATA);
    const uint32_t copy = f.alloc(20);
    const uint32_t src = ld32(moved + 16);
    if (!src) {
        st32(copy + 16, 0);
    } else if (src != moved) {
        st32(copy + 16, vcall(src, 0x08, {src}));     // __clone()
    } else {
        st32(copy + 16, copy);
        vcall(src, 0x0C, {src, copy});                // __clone(dst)
    }
    call(a::ConnectingSphere_executeSphereOpenlist, {data, copy});
    functionDestroy(copy);
    functionDestroy(moved);
}

RW_FUNCTION(0x6BCE78, "SceneSphereBoard::create", SceneSphereBoard::create);
RW_FUNCTION(0x6BE1D8, "SceneSphereBoard::~SceneSphereBoard", SceneSphereBoard::destruct);
RW_FUNCTION(0x6BE26C, "SceneSphereBoard::init", SceneSphereBoard::init);
RW_FUNCTION(0x6BF120, "SceneSphereBoard::loadTextureIncentive", SceneSphereBoard::loadTextureIncentive);
RW_FUNCTION(0x6BF1EC, "SceneSphereBoard::loadTextureBoard", SceneSphereBoard::loadTextureBoard);
RW_FUNCTION(0x6BF2B8, "SceneSphereBoard::loadTextureKind", SceneSphereBoard::loadTextureKind);
RW_FUNCTION(0x6BF384, "SceneSphereBoard::BoardMaking", SceneSphereBoard::boardMaking);
RW_FUNCTION(0x6BF780, "SceneSphereBoard::openTutorial30", SceneSphereBoard::openTutorial30);
RW_FUNCTION(0x6BF84C, "SceneSphereBoard::runInAnimation", SceneSphereBoard::runInAnimation);
RW_FUNCTION(0x6BFB44, "SceneSphereBoard::runOutAnimationMain", SceneSphereBoard::runOutAnimationMain);
RW_FUNCTION(0x6BFEBC, "SceneSphereBoard::backScene", SceneSphereBoard::backScene);
RW_FUNCTION(0x6BFFE8, "SceneSphereBoard back lambda: to the board list (sub_6BFFE8)", SceneSphereBoard::toBoardSelect);
RW_FUNCTION(0x6C0DB8, "SceneSphereBoard open lambda: send the picked spheres (sub_6C0DB8)", SceneSphereBoard::openPicked);
