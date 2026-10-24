// SceneSphereBoardSelect::init: builds the board select screen from its cocostudio scene.
#include "SceneSphereBoardSelect.hpp"

using namespace scene;
using Self = SceneSphereBoardSelect;

namespace {
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;                  // cocos2d::Layer::init
constexpr uint32_t SceneManager_hideInfoHeader = 0x612F4D;
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_removeBackground = 0x611691;
constexpr uint32_t SceneManager_setBackButtonType = 0x613E27;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_backBtnSetPosY = 0x45AFB5;

constexpr uint32_t VSLOT_REMOVE_CHILD_BY_NAME = 0x134;     // Node::removeChildByName(name, cleanup)
constexpr uint32_t VSLOT_SET_TOUCH_ENABLED = 0x29C;        // ui::Widget::setTouchEnabled
constexpr uint32_t VSLOT_AFTER_INIT = 0x300;               // the scene's virtual run after building it
constexpr uint32_t SCENEMANAGER_MODE = 0x28C;

constexpr uint32_t VT_BackKey = 0xF66860;                  // init $_0 [self]
constexpr uint32_t VT_TapBack = 0xF66834;                  // init $_1 [button, self]
constexpr uint32_t VT_TapFilter = 0xF667DC;                // init $_2 [self]

constexpr int32_t TEXT_TICKER = 0x5FF56FD;
constexpr int32_t TEXT_OPEN = 0x629C278;
constexpr int32_t TEXT_FILTER_OFF = 0x5F767C3;

constexpr uint32_t STR_SceneJson = 0xE05547;               // "cocostudio/publish/SphereBoardScene_ver104.json"
constexpr uint32_t STR_BG = 0xE04BF3;
constexpr uint32_t STR_DeckBG = 0xE03AF2;                  // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t STR_Txt_SphereBoard_Label = 0xE05577;
constexpr uint32_t STR_Txt_Open = 0xE0558D;
constexpr uint32_t STR_Txt_Num_Label = 0xDFD8DC;
constexpr uint32_t STR_Txt_Total_Label = 0xE05596;
constexpr uint32_t STR_d = 0xDF86DE;                       // "%d"
constexpr uint32_t STR_empty = 0xDF8A1A;                   // ""
constexpr uint32_t STR_0 = 0xE01C01;                       // "0"
constexpr uint32_t STR_LeftUI = 0xDF85F0;
constexpr uint32_t STR_Button_Back = 0xE03DBA;
constexpr uint32_t STR_Bar_Filter = 0xE055A6;
constexpr uint32_t STR_Button_Filter = 0xE055B1;
constexpr uint32_t STR_Txt_Filter = 0xE04442;
constexpr uint32_t STR_SpherePointIcon = 0xE055BF;
constexpr uint32_t STR_SpherePointIcon_OC = 0xE055CF;
constexpr uint32_t STR_SpherePointIcon_EV = 0xE055E2;
constexpr uint32_t STR_SpherePointIcon_Raid = 0xE055F5;

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }
void setShadow(uint32_t text) { call(SceneUtil_setShadow, {text}); }
uint32_t find(uint32_t root, uint32_t name) { return findNode(root, name, host_strlen(name)); }
}  // namespace

uint32_t SceneSphereBoardSelect::init(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    if (call(Layer_init, {self}) != 1) return 0;
    s->changingScene = 0;
    {
        Frame f;
        uint32_t mgr = sceneManager();
        uint32_t none = newEmptyFunction(f);
        call(SceneManager_hideInfoHeader, {mgr, none});
        functionDestroy(none);
    }
    call(SceneManager_showHeader, {sceneManager()});
    call(SceneManager_removeBackground, {sceneManager()});
    call(SceneManager_setBackButtonType, {sceneManager(), 0});
    {
        Frame f;
        uint32_t mgr = sceneManager();
        uint32_t text = textData(f, TEXT_TICKER);
        uint32_t message = newStr(f, textDataStr(text));
        call(SceneManager_changeTickerMessage, {mgr, message});
        strFree(message);
        sharedRelease(text);
    }
    s->canBack = 1;
    {
        Frame f;
        uint32_t mgr = sceneManager();
        uint32_t onBack = newFunction(f, VT_BackKey, {self});
        call(SceneManager_setOnBackKeyCallback, {mgr, onBack});
        functionDestroy(onBack);
    }
    {
        Frame f;
        uint32_t file = newStr(f, STR_SceneJson, 0x2F);
        s->root = call(SceneUtil_createNodeWithSceneFile, {file});
        strFree(file);
    }
    vcall(self, node::addChild, {self, s->root});
    if (find(s->root, STR_BG)) {
        uint32_t root = s->root;
        Frame f;
        uint32_t name = newStr(f, STR_BG, 2);
        vcall(root, VSLOT_REMOVE_CHILD_BY_NAME, {root, name, 1});
        strFree(name);
    }
    {
        Frame f;
        uint32_t mgr = sceneManager();
        uint32_t bg = newStr(f, STR_DeckBG, 0x1A);
        call(SceneManager_changeBackground, {mgr, 0, bg});
        strFree(bg);
    }
    s->boardNameLabel = find(s->root, STR_Txt_SphereBoard_Label);
    setShadow(s->boardNameLabel);
    uint32_t open = find(s->root, STR_Txt_Open);
    setShadow(open);
    setTextId(open, TEXT_OPEN);
    s->numLabel = find(open, STR_Txt_Num_Label);
    s->totalLabel = find(open, STR_Txt_Total_Label);
    {
        Frame f;
        setTextC(s->totalLabel, stringBuffer(f, {STR_d, 0}));
    }
    {
        Frame f;
        uint32_t num = s->numLabel;
        setTextC(num, stringBuffer(f, {STR_d, 0}));
    }
    {
        Frame f;
        uint32_t name = s->boardNameLabel;
        setTextC(name, stringBuffer(f, {STR_empty}));
    }
    find(s->root, STR_LeftUI);
    uint32_t back = find(s->root, STR_Button_Back);
    call(SceneUtil_backBtnSetPosY, {back});
    addTouch(back, VT_TapBack, {back, self});
    uint32_t barFilter = find(s->root, STR_Bar_Filter);
    uint32_t buttonFilter = find(s->root, STR_Button_Filter);
    addTouch(buttonFilter, VT_TapFilter, {self});
    s->filterText = find(buttonFilter, STR_Txt_Filter);
    setShadow(s->filterText);
    setTextId(s->filterText, TEXT_FILTER_OFF);
    uint32_t bar = find(s->root, STR_Bar_Filter);
    s->filterNumLabel = find(bar, STR_Txt_Num_Label);
    s->filterTotalLabel = find(bar, STR_Txt_Total_Label);
    setShadow(s->filterNumLabel);
    setTextC(s->filterNumLabel, STR_0);
    setShadow(s->filterTotalLabel);
    setTextC(s->filterTotalLabel, STR_0);
    uint32_t icon = find(s->root, STR_SpherePointIcon);
    uint32_t iconColosseum = find(s->root, STR_SpherePointIcon_OC);
    uint32_t iconEvent = find(s->root, STR_SpherePointIcon_EV);
    uint32_t iconRaid = find(s->root, STR_SpherePointIcon_Raid);
    setVisible(icon, 0);
    setVisible(iconColosseum, 0);
    setVisible(iconEvent, 0);
    setVisible(iconRaid, 0);

    // the coin board type follows the menu the scene was opened from
    int32_t mode = int32_t(ld32(sceneManager() + SCENEMANAGER_MODE));
    uint32_t coinIcon = 0;
    int32_t type = 0;
    if (uint32_t(mode) - 2u < 2u) {
        coinIcon = iconEvent;
        type = 4;                               // stage (event) coins
    } else if (mode == 4) {
        coinIcon = iconRaid;
        type = 5;
    } else if (mode == 5) {
        coinIcon = iconColosseum;
        type = 3;
    }
    if (type != 0) {
        s->canBack = 1;
        setVisible(back, 1);
        setVisible(coinIcon, 1);
        setVisible(buttonFilter, 0);
        vcall(buttonFilter, VSLOT_SET_TOUCH_ENABLED, {buttonFilter, 0});
        setVisible(barFilter, 0);
        s->boardType = type;
    } else {
        setVisible(back, 0);
        setVisible(icon, 1);
        s->boardType = 1;
    }
    initSphereBoardList(self);
    vcall(self, VSLOT_AFTER_INIT, {self});
    return 1;
}
RW_FUNCTION(0x6B64B4, "SceneSphereBoardSelect::init", SceneSphereBoardSelect::init);
