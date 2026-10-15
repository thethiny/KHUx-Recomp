// SceneManager: building the header (global navigation bar: Lux / AP / jewels, ticker, avatar, menu and chat
// buttons, raid alerts), the global menu that slides out of the menu button, and the title header (the board with
// the current screen's title and up to two sub titles).
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
// ---- SceneManager fields not in the layout yet
constexpr uint32_t TICKER_WIDTH = 0x344;        // float: visible width of the ticker (from its parent panel to the screen edge)
constexpr uint32_t TICKER_X = 0x348;            // float: start x of the ticker label
constexpr uint32_t MENU_ARMATURE = 0x35C;       // cocostudio::Armature* of the global menu open / close animation
constexpr uint32_t HEADER_SHADOW = 0x360;       // the shadow sprite behind the open global menu (z 3 on the manager)
constexpr uint32_t MENU_NEXT_SCENE = 0x370;     // std::function<Layer*()>: the scene picked in the global menu
constexpr uint32_t CHAT_ICON_NEW = 0x3FC;       // the chat button's "new" icon
constexpr uint32_t TITLE_BOARD = 0x390;         // title header: MenuTitle_Board
constexpr uint32_t TITLE_LOGO = 0x394;          // MenuTitle_Logo
constexpr uint32_t TITLE_SUB1_BOARD = 0x398, TITLE_SUB1_LABEL = 0x39C, TITLE_SUB1_X = 0x3A0;   // X: float, layout x
constexpr uint32_t TITLE_SUB2_BOARD = 0x3A4, TITLE_SUB2_LABEL = 0x3A8, TITLE_SUB2_X = 0x3AC;
constexpr uint32_t PREPARE_FINISH_SET = 0x4E0;  // prepareFinishSceneCallback.__f_ (0: none)
constexpr uint32_t MENU_FINISH_CALLBACK = 0x500; // function500: std::function<void(std::function<void(bool)>)>
// fields of other SceneManager instances (written through getInstance())
constexpr uint32_t SCENE_MODE_28C = 0x28C;      // uint32: 9 before SceneAdventureSelect, 1 before SceneSphereBoardSelect
constexpr uint32_t DECK_FLAG_2E0 = 0x2E0, DECK_FLAG_2E1 = 0x2E1;   // bytes cleared before SceneDeckEdit

// ---- library
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t Vec2_ctor_ff = 0x8E4B9D;               // Vec2(float x, float y)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_copy = 0x8DF02D;                  // Size(const Size&)
constexpr uint32_t Node_convertToWorldSpace = 0x895A39;   // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t CustomTextView_setHAlign = 0x4F92D9;
constexpr uint32_t ImageButton_create = 0x531239;         // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;  // (button, Node*, float)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;            // (const std::string& name)
constexpr uint32_t ArmatureAnimation_setMovementEventCallFunc = 0x7A3C69;   // (anim, std::function by value)
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t TouchBeganFunction_swap = 0x483E4D;    // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t TouchFunction_swap = 0x4F3699;         // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t TouchFunction_assignLambda = 0x611B61; // sub_611B60: function<void(Touch*,Event*)>::operator=(initGlobalMenu $_18 [this])
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t SetMenuButtonText = 0x611AA1;          // sub_611AA0(ui::Text*, text id): a global menu button label
constexpr uint32_t UpdateHeaderAvatarIcon = 0x611725;     // SceneManager::updateHeaderAvatarIcon(Node*, int)
constexpr uint32_t FinishFunction_swap = 0x607685;        // std::function<void(std::function<void(bool)>)>::swap
constexpr uint32_t LayerFactory_assign = 0x612E29;        // std::function<Layer*()>::operator=(const function&)
constexpr uint32_t FunctionBool_call = 0x51022D;          // std::function<bool()>::operator()
constexpr uint32_t OtherMenuDialog_create = 0x59E84D, OtherMenuDialog_open = 0x59E8E1;

// Widget / Armature virtual slots (byte offsets) not in node::
constexpr uint32_t WIDGET_SET_TOUCH_ENABLED = 0x29C;      // ui::Widget::setTouchEnabled(bool)
constexpr uint32_t ARMATURE_GET_ANIMATION = 0x298;        // cocostudio::Armature::getAnimation()
constexpr uint32_t ANIMATION_PLAY = 0x68;                 // ArmatureAnimation::play(name, durationTo, loop)

constexpr uint32_t ATLASNUM_KERNING_PIXEL = 0xDEC3DC;     // SceneUtil::ATLASNUM_KERNING_PIXEL (float -5)
constexpr int32_t TEXT_AP_RECOVERY_1 = 101000024, TEXT_AP_RECOVERY_2 = 101000025;
constexpr int32_t TUTORIAL_PHASE_ALERTS = 709;            // the raid alert button may show after this phase

// ---- lambdas (std::function vtables, __func vtable + 8)
constexpr uint32_t VT_Header_TapAp = 0xF607C0;            // createHeader $_9 [this, balloon]
constexpr uint32_t VT_Header_TapMagicStone = 0xF60768;    // $_10 [this, magicStone]
constexpr uint32_t VT_Header_TapAvatar = 0xF60710;        // $_11 [this]
constexpr uint32_t VT_Header_TapChat = 0xF606E4;          // $_12 [this]
constexpr uint32_t VT_Header_TapRelief = 0xF606B8;        // $_13 [this, alerts]
constexpr uint32_t VT_Menu_Movement = 0xF6068C;           // initGlobalMenu $_14 [this]
constexpr uint32_t VT_Menu_TouchBegan = 0xF60584;         // $_16 [this, menuButton, center, this, center, center] (heap)
constexpr uint32_t VT_Menu_TouchMoved = 0xF60558;         // $_17 [this, center, center]
constexpr uint32_t VT_Close_MyPage = 0xF60500;            // closeGlobalMenu $_19
constexpr uint32_t VT_Close_Adventure = 0xF604D4;         // $_21
constexpr uint32_t VT_Close_Sphere = 0xF604A8;            // $_22
constexpr uint32_t VT_Close_Shop = 0xF6047C;              // $_23
constexpr uint32_t VT_Close_RunOutDone = 0xF609FC;        // $_20(...)::{lambda()#1} [this]
constexpr uint32_t VT_LayerFactoryPtr = 0xF7E130;         // __func<Layer*(*)()>: [vtable, function pointer]

// createSceneLayer<T>
constexpr uint32_t CreateLayer_MyPage = 0x46FDBD, CreateLayer_AdventureSelect = 0x579EB1,
                   CreateLayer_DeckEdit = 0x57AFCD, CreateLayer_SphereBoardSelect = 0x612555,
                   CreateLayer_MedalList = 0x48A62D, CreateLayer_ShopTop = 0x5B27C5;

// ---- string literals
constexpr uint32_t STR_GlobalNaviScene = 0xE02F8B;       // "cocostudio/publish/GlobalNaviScene_Ver120.json"
constexpr uint32_t STR_CenterUI = 0xE02FBA, STR_Panel_1 = 0xE02A0D, STR_LeftUI = 0xDF85F0, STR_Back = 0xE02FC3;
constexpr uint32_t STR_AtlasLabel_NumL = 0xDFD2A4, STR_Symbol_LUX = 0xE02FC8, STR_Base_LUX = 0xDFE710;
constexpr uint32_t STR_Base_AP = 0xE02FD3, STR_AtlasLabel_AP = 0xDFD382, STR_AtlasLabel_AP_Max = 0xE02FDB;
constexpr uint32_t STR_AtlasLabel_AP_Slash = 0xE02FED;
constexpr uint32_t STR_BalloonApRecovery = 0xE03001;     // "cocostudio/publish/BalloonNomal_AP_Recovery.json"
constexpr uint32_t STR_Txt_Wording1 = 0xDFF14C, STR_Txt_Wording2 = 0xDF863B, STR_Txt_Time_Label = 0xDFF589;
constexpr uint32_t STR_Tap_Area_AP = 0xE03032, STR_MagicStone = 0xDFCFDC, STR_TickerBG = 0xE0303E;
constexpr uint32_t STR_Ticker_Label = 0xE03047, STR_AvatarPanel = 0xE03054, STR_Avatar = 0xDF8F56;
constexpr uint32_t STR_Empty = 0xDF8A1A, STR_imageButton = 0xDF8F6D, STR_MenuButton = 0xE02E00;
constexpr uint32_t STR_ChatButton = 0xE03060, STR_Icon_New = 0xDFDD8A, STR_Animation1 = 0xDFD0AF;
constexpr uint32_t STR_Area_Anim_Jewels = 0xE0306B, STR_Jewels_Json = 0xE0307C, STR_Jewels_Name = 0xE030B1;
constexpr uint32_t STR_Area_Anim_LUX = 0xE030C8, STR_LUX_Json = 0xE030D6, STR_LUX_Name = 0xE03108;
constexpr uint32_t STR_Alerts = 0xE0311C, STR_TickerBG_Alerts = 0xE03123;
constexpr uint32_t STR_Point_Anim_Alerts = 0xE03133, STR_Alerts_Json = 0xE03145, STR_Alerts_Name = 0xE0317A;
constexpr uint32_t STR_Point_Anim_Warning = 0xE03191, STR_Warning_Json = 0xE031A4, STR_Warning_Name = 0xE031DA;
constexpr uint32_t STR_Button_Relief = 0xE031F2;
constexpr uint32_t STR_RightUI_BarButton_Copy = 0xE03200, STR_Animation = 0xE03217, STR_Txt = 0xDF94D1;
constexpr uint32_t STR_Close = 0xE03260, STR_Button_d = 0xE03266;
constexpr uint32_t STR_TopinfoBarButOff = 0xE03270;      // "img/ui/Topinfo_BarBut%d_Off.png"
constexpr uint32_t STR_TitleStatus = 0xE02EF4;           // "cocostudio/publish/TitleStatus.json"
constexpr uint32_t STR_MenuTitle_Board = 0xE02F18, STR_MenuTitle_Logo = 0xE02F28;
constexpr uint32_t STR_Sub01_Board = 0xE02F37, STR_Sub01_Label = 0xE02F4C;
constexpr uint32_t STR_Sub02_Board = 0xE02F61, STR_Sub02_Label = 0xE02F76;

uint32_t mgrAddr(SceneManager *self) { return rw::guest_addr(self); }
uint32_t get32(SceneManager *self, uint32_t off) { return rw::get<uint32_t>(self, off); }
void set32(SceneManager *self, uint32_t off, uint32_t v) { rw::set<uint32_t>(self, off, v); }

float positionX(uint32_t n) { return bitsf(vcall(n, node::getPositionX, {n})); }
void setPositionX(uint32_t n, float x) { vcall(n, node::setPositionX, {n, fbits(x)}); }
// move a node right by `dx` (the header widens with the visible area)
void shiftX(uint32_t n, float dx) {
    float x = positionX(n);
    setPositionX(n, dx + x);
}
void setVisible(uint32_t n, uint32_t visible) { vcall(n, node::setVisible, {n, visible}); }
// the first child of a node (getChildren()->at(0), unchecked)
uint32_t firstChild(uint32_t n) { return ld32(ld32(vcall(n, node::getChildren, {n}))); }
// Node::getChildByName with a temporary name
uint32_t childByName(uint32_t parent, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    uint32_t child = vcall(parent, node::getChildByName, {parent, s});
    strFree(s);
    return child;
}
// armature->getAnimation()->play(name, -1, -1)
void playAnimation(uint32_t armature, uint32_t name, uint32_t len) {
    uint32_t anim = vcall(armature, ARMATURE_GET_ANIMATION, {armature});
    Frame f;
    uint32_t s = newStr(f, name, len);
    vcall(anim, ANIMATION_PLAY, {anim, s, 0xFFFFFFFFu, 0xFFFFFFFFu});
    strFree(s);
}
// load a cocostudio armature export, create the armature `name` and play its "Animation1"
uint32_t createLoopArmature(uint32_t json, uint32_t jsonLen, uint32_t name, uint32_t nameLen) {
    uint32_t manager = call(ArmatureDataManager_getInstance, {});
    uint32_t armature;
    {
        Frame f;
        uint32_t s = newStr(f, json, jsonLen);
        call(ArmatureDataManager_addArmatureFileInfo, {manager, s});
        strFree(s);
    }
    {
        Frame f;
        uint32_t s = newStr(f, name, nameLen);
        armature = call(Armature_create, {s});
        strFree(s);
    }
    playAnimation(armature, STR_Animation1, 10);
    return armature;
}

// closeGlobalMenu: swap a lambda (no captures) into function500, destroying the previous callback
void replaceMenuFinishCallback(SceneManager *self, uint32_t vtable) {
    Frame f;
    uint32_t tmp = newFunction(f, vtable, {});
    call(FinishFunction_swap, {tmp, mgrAddr(self) + MENU_FINISH_CALLBACK});
    functionDestroy(tmp);
}
// closeGlobalMenu: pick the next scene layer, and when the scene may finish run the current scene's out
// animation (its callback changes to the picked scene)
void goToScene(SceneManager *self, uint32_t createLayer) {
    uint32_t m = mgrAddr(self);
    Frame f;
    uint32_t factory = newFunction(f, VT_LayerFactoryPtr, {createLayer});
    call(LayerFactory_assign, {m + MENU_NEXT_SCENE, factory});
    if (get32(self, PREPARE_FINISH_SET) == 0 ||
        call(FunctionBool_call, {rw::guest_addr(self->prepareFinishSceneCallback)}) == 1) {
        uint32_t done = newFunction(f, VT_Close_RunOutDone, {m});
        SceneManager::callCurrentSceneRunOutAnimation(self, done);
        functionDestroy(done);
    }
    functionDestroy(factory);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ header

// The global navigation header (GlobalNaviScene_Ver120): laid out for the visible width (items shift right by
// a share of the extra width), the AP recovery balloon, touch handlers, the looping jewel / Lux / alert
// armatures, and the ticker geometry.
SceneManager::Addr SceneManager::createHeader(SceneManager *self) {
    uint32_t m = mgrAddr(self);
    self->apShown = 0xFFFFFFFFu;
    self->apMaxShown = 0xFFFFFFFFu;
    self->apSeconds = 0xFFFFFFFFu;
    Frame frame;
    const float visibleWidth = visibleSize().width;
    const uint32_t tickerWorld = frame.alloc(8);              // Vec2, destroyed at the end

    uint32_t root;
    float k;                                                  // extra width in units of 176 px
    {
        Frame f;
        uint32_t s = newStr(f, STR_GlobalNaviScene, 0x2E);
        k = (visibleWidth + -960.0f) / 176.0f;
        root = SceneUtil::createNodeWithSceneFile(s);
        strFree(s);
    }
    {
        float x = positionX(root);
        float margin = SceneUtil::getSideMargin();
        setPositionX(root, x - margin);
        float y = bitsf(vcall(root, node::getPositionY, {root}));
        float heightMargin = SceneUtil::getSideHeightMargin();
        vcall(root, node::setPositionY, {root, fbits(y + heightMargin)});
    }
    const uint32_t center = findNode(root, STR_CenterUI, 8);
    {
        uint32_t panel = findNode(center, STR_Panel_1, 7);
        vcall(panel, WIDGET_SET_TOUCH_ENABLED, {panel, 0});
    }
    {
        uint32_t left = findNode(root, STR_LeftUI, 6);
        vcall(left, node::removeFromParent, {left});
        uint32_t back = findNode(root, STR_Back, 4);
        vcall(back, node::removeFromParent, {back});
    }
    SceneUtil::setAdditionalKerningForAtlasLabel(findNode(center, STR_AtlasLabel_NumL, 15), -21);
    const float k37 = k * 37.0f;
    {
        uint32_t symbol = findNode(center, STR_Symbol_LUX, 10);
        float x = positionX(symbol);
        setPositionX(symbol, k37 + x);
    }
    shiftX(findNode(center, STR_Base_LUX, 8), k * 62.0f);
    const uint32_t baseAp = findNode(center, STR_Base_AP, 7);
    shiftX(baseAp, k * 88.0f);
    {
        uint32_t ap = findNode(root, STR_AtlasLabel_AP, 13);
        int32_t kerning = f2i(ldf(ATLASNUM_KERNING_PIXEL));
        SceneUtil::setAdditionalKerningForAtlasLabel(ap, kerning);
        SceneUtil::setAdditionalKerningForAtlasLabel(findNode(root, STR_AtlasLabel_AP_Max, 17), kerning);
        SceneUtil::setAdditionalKerningForAtlasLabel(findNode(root, STR_AtlasLabel_AP_Slash, 19), kerning);
    }

    // the AP recovery balloon, left of the AP gauge (hidden: opacity 0)
    const uint32_t balloon =
        call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), STR_BalloonApRecovery});
    vcall(root, node::addChild, {root, balloon});
    vcall(balloon, WIDGET_SET_TOUCH_ENABLED, {balloon, 0});
    {
        float apX = positionX(baseAp);
        float balloonWidth = ldf(vcall(balloon, node::getContentSize, {balloon}));
        float apWidth = ldf(vcall(baseAp, node::getContentSize, {baseAp}));
        Frame f;
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor_ff, {pos, fbits((apX - balloonWidth) - apWidth), fbits(540.0f)});
        vcall(balloon, node::setPosition, {balloon, pos});
        call(Vec2_dtor, {pos});
    }
    vcall(balloon, node::setOpacity, {balloon, 0});
    vcall(balloon, node::setLocalZOrder, {balloon, 2});
    {
        uint32_t wording1 = findNode(balloon, STR_Txt_Wording1, 12);
        SceneUtil::setShadow(wording1);
        setTextId(wording1, TEXT_AP_RECOVERY_1);
        SceneUtil::setShadow(findNode(wording1, STR_Txt_Time_Label, 14));
        uint32_t wording2 = findNode(balloon, STR_Txt_Wording2, 12);
        SceneUtil::setShadow(wording2);
        setTextId(wording2, TEXT_AP_RECOVERY_2);
        SceneUtil::setShadow(findNode(wording2, STR_Txt_Time_Label, 14));
    }
    {
        uint32_t tapAp = findNode(center, STR_Tap_Area_AP, 11);
        vcall(tapAp, WIDGET_SET_TOUCH_ENABLED, {tapAp, 1});
        addTouch(tapAp, VT_Header_TapAp, {m, balloon});
    }
    {
        uint32_t magicStone = findNode(center, STR_MagicStone, 10);
        shiftX(magicStone, k * 114.0f);
        vcall(magicStone, WIDGET_SET_TOUCH_ENABLED, {magicStone, 1});
        addTouch(magicStone, VT_Header_TapMagicStone, {m, magicStone});
    }

    // the ticker: a CustomTextView replacing the label; its scroll width runs from the parent panel to the screen edge
    const float k26 = k * 26.0f;
    {
        uint32_t tickerBg = findNode(center, STR_TickerBG, 8);
        float x = positionX(tickerBg);
        setPositionX(tickerBg, k26 + x);
    }
    {
        uint32_t label = findNode(center, STR_Ticker_Label, 12);
        Frame f;
        uint32_t s = newStr(f, STR_Ticker_Label, 12);
        SceneUtil::textToCustomTextView(label, s);
        strFree(s);
    }
    {
        uint32_t view = findNode(root, STR_Ticker_Label, 12);
        call(CustomTextView_setHAlign, {view, 0});
        uint32_t parent = vcall(view, node::getParent, {view});
        uint32_t grandParent = vcall(parent, node::getParent, {parent});
        uint32_t pos = vcall(parent, node::getPosition, {parent});
        call(Node_convertToWorldSpace, {tickerWorld, grandParent, pos});
        float panelWidth = ldf(vcall(parent, node::getContentSize, {parent}));
        float worldX = ldf(tickerWorld);
        float panelWidth2 = ldf(vcall(parent, node::getContentSize, {parent}));
        rw::set<float>(self, TICKER_WIDTH, panelWidth - ((worldX + panelWidth2) - visibleWidth));
        set32(self, TICKER_X, vcall(view, node::getPositionX, {view}));
    }
    if (get32(self, HEADER_SHADOW) == 0) {
        uint32_t shadow = SceneUtil::createShadowSprite();
        set32(self, HEADER_SHADOW, shadow);
        vcall(m, node::addChildZ, {m, shadow, 3});
    }

    // the avatar: an image button over the avatar panel
    {
        uint32_t avatarPanel = findNode(center, STR_AvatarPanel, 11);
        shiftX(avatarPanel, k * 18.0f);
        setVisible(findNode(center, STR_Avatar, 6), 0);
        uint32_t button;
        {
            Frame f;
            uint32_t s = newStr(f, STR_Empty, 0);
            button = call(ImageButton_create, {s, 1});
            strFree(s);
        }
        call(ImageButton_setTargetView, {button, avatarPanel, fbits(1.0f)});
        {
            Frame f;
            uint32_t s = newStr(f, STR_imageButton, 11);
            vcall(avatarPanel, node::addChildZName, {avatarPanel, button, 0, s});
            strFree(s);
        }
        {
            Frame f;
            uint32_t cb = newFunction(f, VT_Header_TapAvatar, {m});
            call(ImageButton_addTouchEventListener, {button, cb});
            functionDestroy(cb);
        }
        call(UpdateHeaderAvatarIcon, {m, center, 0xFFFFFFFFu});
    }
    {
        uint32_t menuButton = findNode(center, STR_MenuButton, 10);
        shiftX(menuButton, k * 176.0f);
        vcall(menuButton, node::setLocalZOrder, {menuButton, 999});
    }
    {
        uint32_t chat = findNode(center, STR_ChatButton, 10);
        shiftX(chat, k * 146.0f);
        addTouch(chat, VT_Header_TapChat, {m});
        set32(self, CHAT_ICON_NEW, findNode(chat, STR_Icon_New, 8));
        setVisible(get32(self, CHAT_ICON_NEW), 0);
    }

    // looping jewel and Lux animations
    {
        uint32_t area = findNode(center, STR_Area_Anim_Jewels, 16);
        uint32_t armature = createLoopArmature(STR_Jewels_Json, 0x34, STR_Jewels_Name, 0x16);
        vcall(area, node::addChild, {area, armature});
    }
    {
        uint32_t area = findNode(center, STR_Area_Anim_LUX, 13);
        uint32_t armature = createLoopArmature(STR_LUX_Json, 0x31, STR_LUX_Name, 0x13);
        vcall(area, node::addChild, {area, armature});
        float x = positionX(area);
        setPositionX(area, k37 + x);
    }

    // raid alerts: ticker background, the alert / warning armatures at their marker points, the relief button
    const uint32_t alerts = findNode(root, STR_Alerts, 6);
    const uint32_t alertsBg = findNode(alerts, STR_TickerBG_Alerts, 15);
    {
        float x = positionX(alertsBg);
        setPositionX(alertsBg, k26 + x);
    }
    {
        uint32_t panel = findNode(alerts, STR_Panel_1, 7);
        Frame f;
        uint32_t size = f.alloc(8);
        call(Size_copy, {size, vcall(panel, node::getContentSize, {panel})});
        st32(size, 0);                                        // width 0
        vcall(panel, node::setContentSize, {panel, size});
    }
    {
        uint32_t point = findNode(alerts, STR_Point_Anim_Alerts, 17);
        uint32_t armature = createLoopArmature(STR_Alerts_Json, 0x34, STR_Alerts_Name, 0x16);
        vcall(alertsBg, node::addChild, {alertsBg, armature});
        vcall(armature, node::setPosition, {armature, vcall(point, node::getPosition, {point})});
    }
    {
        uint32_t point = findNode(alerts, STR_Point_Anim_Warning, 18);
        uint32_t armature = createLoopArmature(STR_Warning_Json, 0x35, STR_Warning_Name, 0x17);
        vcall(alertsBg, node::addChild, {alertsBg, armature});
        vcall(armature, node::setPosition, {armature, vcall(point, node::getPosition, {point})});
    }
    {
        uint32_t relief = findNode(alerts, STR_Button_Relief, 13);
        shiftX(relief, k * 72.0f);
        addTouch(relief, VT_Header_TapRelief, {m, alerts});
    }
    self->flag588 = SceneUtil::getNexTutorialphase() > TUTORIAL_PHASE_ALERTS ? 1 : 0;
    call(Vec2_dtor, {tickerWorld});
    return root;
}
RW_FUNCTION(0x60EA64, "SceneManager::createHeader", SceneManager::createHeader);

// ------------------------------------------------------------------------------------------------ global menu

// The global menu under the header's CenterUI: the bar button copy and the menu animation shifted with the
// visible width, the seven button labels, the animation event callback and the menu button's touch listener.
void SceneManager::initGlobalMenu(SceneManager *self) {
    uint32_t m = mgrAddr(self);
    const uint32_t center = findNode(self->header, STR_CenterUI, 8);
    const uint32_t menuButton = findNode(center, STR_MenuButton, 10);
    const float visibleWidth = visibleSize().width;
    self->barButtonCopy = findNode(self->header, STR_RightUI_BarButton_Copy, 22);
    const float k = (visibleWidth + -960.0f) / 176.0f;
    {
        uint32_t first = firstChild(self->barButtonCopy);
        vcall(first, WIDGET_SET_TOUCH_ENABLED, {first, 0});
    }
    setVisible(self->barButtonCopy, 0);
    const float shift = k * 176.0f;
    {
        uint32_t copy = self->barButtonCopy;
        float x = positionX(copy);
        setPositionX(copy, shift + x);
    }
    {
        uint32_t animation = findNode(center, STR_Animation, 9);
        setVisible(animation, 1);
        float x = positionX(animation);
        setPositionX(animation, shift + x);
        set32(self, MENU_ARMATURE, firstChild(animation));
    }
    setVisible(get32(self, MENU_ARMATURE), 0);
    {
        uint32_t armature = get32(self, MENU_ARMATURE);
        uint32_t anim = vcall(armature, ARMATURE_GET_ANIMATION, {armature});
        Frame f;
        uint32_t cb = newFunction(f, VT_Menu_Movement, {m});
        call(ArmatureAnimation_setMovementEventCallFunc, {anim, cb});
        functionDestroy(cb);
    }
    static const struct { uint32_t name; int32_t text; } BUTTONS[7] = {
        {0xE03221, 100100003}, {0xE0322A, 100100008}, {0xE03233, 100100006}, {0xE0323C, 100100009},
        {0xE03245, 101200027}, {0xE0324E, 100100007}, {0xE03257, 100100031},
    };                                                        // "Button_1".."Button_7", their label text ids
    for (const auto &b : BUTTONS) {
        uint32_t button = findNode(center, b.name, 8);
        Frame f;
        uint32_t s = newStr(f, STR_Txt, 3);
        uint32_t text = SceneUtil::searchNodeByName(button, s);
        call(SetMenuButtonText, {text, uint32_t(b.text)});
        strFree(s);
    }
    vcall(menuButton, WIDGET_SET_TOUCH_ENABLED, {menuButton, 0});

    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    Frame f;
    {
        // onTouchBegan: a heap lambda (24 bytes of captures)
        uint32_t began = f.alloc(20);
        st32(began + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x1C});
        if (heap) {
            st32(heap, VT_Menu_TouchBegan);
            st32(heap + 4, m);
            st32(heap + 8, menuButton);
            st32(heap + 12, center);
            st32(heap + 16, m);
            st32(heap + 20, center);
            st32(heap + 24, center);
        }
        st32(began + 16, heap);
        call(TouchBeganFunction_swap, {began, listener + 0x50});
        functionDestroy(began);
    }
    {
        uint32_t moved = newFunction(f, VT_Menu_TouchMoved, {m, center, center});
        call(TouchFunction_swap, {moved, listener + 0x68});
        functionDestroy(moved);
    }
    {
        // onTouchEnded / onTouchCancelled = the same [this] lambda
        uint32_t lambda = f.alloc(4);
        st32(lambda, m);
        call(TouchFunction_assignLambda, {listener + 0x80, lambda});
        call(TouchFunction_assignLambda, {listener + 0x98, lambda});
    }
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {ld32(director() + 0x84), listener, menuButton});
}
RW_FUNCTION(0x6102D4, "SceneManager::initGlobalMenu", SceneManager::initGlobalMenu);

// Close the global menu (when open): hide the bar button copy, play the menu's "Close" animation, switch the
// picked button's image back to "off" and go to the picked screen (button 7: the other-menu dialog).
bool SceneManager::closeGlobalMenu(SceneManager *self) {
    if (!self->globalMenuOpen) return false;                   // +0x368: the global menu is open
    call(AudioPlaySystemSEDecide, {});
    const uint32_t center = findNode(self->header, STR_CenterUI, 8);
    setVisible(self->barButtonCopy, 0);
    uint32_t armature = get32(self, MENU_ARMATURE);
    setVisible(armature, 1);
    playAnimation(get32(self, MENU_ARMATURE), STR_Close, 5);
    self->globalMenuClosed = 1;
    self->globalMenuOpen = 0;
    if (int32_t(self->globalMenuSelected.get()) != -1) {                // +0x364: the picked menu button (0-based)
        uint32_t button;
        {
            Frame f;
            uint32_t buf = stringBuffer(f, {STR_Button_d, self->globalMenuSelected + 1});
            uint32_t s = newStr(f, buf);
            button = SceneUtil::searchNodeByName(center, s);
            strFree(s);
        }
        {
            Frame f;
            uint32_t buf = stringBuffer(f, {STR_TopinfoBarButOff, self->globalMenuSelected + 1});
            uint32_t s = newStr(f, buf);
            call(fn::ImageView_loadTexture, {button, s, 0});
            strFree(s);
        }
        switch (self->globalMenuSelected.get()) {
        case 0:
            replaceMenuFinishCallback(self, VT_Close_MyPage);
            goToScene(self, CreateLayer_MyPage);
            break;
        case 1:
            replaceMenuFinishCallback(self, VT_Close_Adventure);
            rw::set<uint32_t>(getInstance(), SCENE_MODE_28C, 9);
            goToScene(self, CreateLayer_AdventureSelect);
            break;
        case 2:
            rw::set<uint8_t>(self, DECK_FLAG_2E0, 0);
            rw::set<uint8_t>(self, DECK_FLAG_2E1, 0);
            goToScene(self, CreateLayer_DeckEdit);
            break;
        case 3:
            replaceMenuFinishCallback(self, VT_Close_Sphere);
            rw::set<uint32_t>(getInstance(), SCENE_MODE_28C, 1);
            goToScene(self, CreateLayer_SphereBoardSelect);
            break;
        case 4: {
            SceneManager *mgr = getInstance();
            mgr->field2A4 = 0;
            mgr->field2A0 = 0;
            goToScene(self, CreateLayer_MedalList);
            break;
        }
        case 5:
            replaceMenuFinishCallback(self, VT_Close_Shop);
            goToScene(self, CreateLayer_ShopTop);
            break;
        case 6: {
            uint32_t dialog = call(OtherMenuDialog_create, {});
            call(OtherMenuDialog_open, {dialog, getInstance()->popupLayer});
            break;
        }
        default:
            break;
        }
    }
    self->globalMenuSelected = 0xFFFFFFFFu;
    return true;
}
RW_FUNCTION(0x611BD4, "SceneManager::closeGlobalMenu", SceneManager::closeGlobalMenu);

// ------------------------------------------------------------------------------------------------ title header

// The title header (TitleStatus.json): board, logo and the two sub title boards / labels (their layout x kept,
// then moved onto the board's x), placed right below the header.
SceneManager::Addr SceneManager::createTitleHeader(SceneManager *self, uint32_t, uint32_t, uint32_t) {
    self->titleHeader = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), STR_TitleStatus});
    set32(self, TITLE_BOARD, childByName(self->titleHeader, STR_MenuTitle_Board, 15));
    set32(self, TITLE_LOGO, childByName(get32(self, TITLE_BOARD), STR_MenuTitle_Logo, 14));
    struct Sub { uint32_t board, label, x, boardName, labelName; };
    static const Sub SUBS[2] = {
        {TITLE_SUB1_BOARD, TITLE_SUB1_LABEL, TITLE_SUB1_X, STR_Sub01_Board, STR_Sub01_Label},
        {TITLE_SUB2_BOARD, TITLE_SUB2_LABEL, TITLE_SUB2_X, STR_Sub02_Board, STR_Sub02_Label},
    };
    for (const Sub &sub : SUBS) {
        set32(self, sub.board, childByName(self->titleHeader, sub.boardName, 20));
        set32(self, sub.x, vcall(get32(self, sub.board), node::getPositionX, {get32(self, sub.board)}));
        setPositionX(get32(self, sub.board), positionX(get32(self, TITLE_BOARD)));
        set32(self, sub.label, childByName(get32(self, sub.board), sub.labelName, 20));
        SceneUtil::setShadow(get32(self, sub.label));
        setPositionX(get32(self, sub.label), positionX(get32(self, TITLE_BOARD)));
    }
    uint32_t widget = self->titleHeader;
    float visibleHeight = visibleSize().height;
    float headerHeight = ldf(vcall(self->header, node::getContentSize, {self->header}) + 4);
    float titleHeight = ldf(vcall(self->titleHeader, node::getContentSize, {self->titleHeader}) + 4);
    Frame f;
    uint32_t pos = f.alloc(8);
    call(Vec2_ctor_ff, {pos, fbits(0.0f), fbits((visibleHeight - headerHeight) - titleHeight)});
    vcall(widget, node::setPosition, {widget, pos});
    call(Vec2_dtor, {pos});
    return self->titleHeader;
}
RW_FUNCTION(0x610DA0, "SceneManager::createTitleHeader", SceneManager::createTitleHeader);
