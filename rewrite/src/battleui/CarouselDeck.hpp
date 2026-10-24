// CarouselDeck: the battle screen's medal deck (bottom of the quest screen). A cocos2d::Node holding the
// ActionMap_KeyBladeDeck layout: the keyblade button in the centre, up to five medal buttons rolling around it
// (the "carousel"), the deck info window (tactical boxes with each medal's attack / skill / guilt), the keyblade
// windows with the ability infos, and the cocostudio armatures of the deck animations (deck in, medal attack,
// special attack cut-ins, guilt bonus, enemy turn, long tap, burst ready).
// The battle (StageUI) creates it with CarouselDeck::create and fills it with addMedalOntoDeck / setDeckInfoWindow.
#pragma once

#include "BattleUi.hpp"

// Guest addresses and helpers shared by the CarouselDeck files.
namespace cdeck {
using namespace scene;

// ---- cocos2d / cocostudio / runtime
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base, Node_dtor = rwlib::cocos2d::Node::dtor_base, Node_init = rwlib::cocos2d::Node::init, Node_create = rwlib::cocos2d::Node::create;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t ActionObject_stop = rwlib::cocostudio::ActionObject::stop, ActionObject_setLoop = rwlib::cocostudio::ActionObject::setLoop;
constexpr uint32_t ActionObject_updateToFrameByTime = rwlib::cocostudio::ActionObject::updateToFrameByTime;   // (action, float time)
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t ActionManagerEx_getInstance = rwlib::cocostudio::ActionManagerEx::getInstance, ActionManagerEx_getActionByName = rwlib::cocostudio::ActionManagerEx::getActionByName;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;                    // (const std::string&)
constexpr uint32_t AnimationData_getMovement = rwlib::cocostudio::AnimationData::getMovement;
constexpr uint32_t ArmatureAnimation_setMovementEventCallFunc = rwlib::cocostudio::ArmatureAnimation::setMovementEventCallFunc;
constexpr uint32_t ArmatureAnimation_setFrameEventCallFunc = rwlib::cocostudio::ArmatureAnimation::setFrameEventCallFunc;
constexpr uint32_t DisplayManager_removeDisplay = rwlib::cocostudio::DisplayManager::removeDisplay, DisplayManager_addDisplay = rwlib::cocostudio::DisplayManager::addDisplay;
constexpr uint32_t DisplayManager_changeDisplayWithIndex = rwlib::cocostudio::DisplayManager::changeDisplayWithIndex;
constexpr uint32_t AppGetGrameFrameWidth = 0x799B31;
constexpr uint32_t Vec2_ctor_xy = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_ctor_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base, Vec2_add = rwlib::cocos2d::Vec2::add;
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__RKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD, audio_Manager_load = 0x5BEEF5;
constexpr uint32_t function_vv_assign = 0x4EB9CD;                 // std::function<void()>::operator=(const&)
constexpr uint32_t function_b_call = 0x51022D;                    // std::function<bool()>::operator()
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE, Button_loadTextureNormal = rwlib::cocos2d::ui::Button::loadTextureNormal;
constexpr uint32_t Widget_getCustomSize = rwlib::cocos2d::ui::Widget::getCustomSize, Button_create = rwlib::cocos2d::ui::Button::create;
constexpr uint32_t MedalIndex_find = 0x490275;                    // unordered_map<u64, int>::find
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Medal_create = 0x785871;                       // master::Medal::create(sret, id)
constexpr uint32_t VEC2_ZERO = 0x1006950;                         // cocos2d::Vec2::ZERO

// ---- data
constexpr uint32_t APIManager_userMedals = 0xD4;   // shared_ptr<UserMedal>: SafeBuffer of 72-byte medals
constexpr uint32_t UserMedal_index = 0x24;         // unordered_map<u64 user medal id, int slot>
constexpr uint32_t MEDAL_IMAGE = 4;                // master::Medal data: image id
constexpr uint32_t SE_DECK = 0x1E56BADA;           // audio resource loaded by init
constexpr int32_t TEXT_TUTORIAL_DECK = 104820101;  // TextData: the deck tutorial hint
constexpr uint32_t CUTIN_MOVEMENT_NAMES = 0xF5D158;   // const char*[4] "MedalAttack_AtkAbilty" ...
constexpr uint32_t GUILT_NUM_BONE_NAMES = 0xF5D1BC;   // const char*[3] "GuiltBonusNum3", 2, 1
constexpr uint32_t DECK_STYLE_RES_CAROUSEL_NAME = 0xFEF0B0;   // CarouselDeck::DECK_STYLE_RES_*: const char* formats
constexpr uint32_t DECK_STYLE_RES_BUTTON_NAME = 0xFEF0B4;
constexpr uint32_t DECK_STYLE_RES_MEDAL_NUN_NAME = 0xFEF0B8;

// ---- virtual slots (byte offsets)
constexpr uint32_t VT_INIT = 0x248;                      // Node::init
constexpr uint32_t VT_GET_ANIMATION = 0x298;             // Armature::getAnimation
constexpr uint32_t VT_ARMATURE_GET_BONE = 0x27C;         // Armature::getBone(const std::string&)
constexpr uint32_t VT_BONE_GET_DISPLAY_MANAGER = 0x29C;  // Bone::getDisplayManager
constexpr uint32_t VT_ANIM_PLAY = 0x68;                  // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t VT_ANIM_GOTO_AND_PLAY = 0x7C;         // ArmatureAnimation::gotoAndPlay(frame)
constexpr uint32_t VT_ANIM_GOTO_AND_PAUSE = 0x80;        // ArmatureAnimation::gotoAndPause(frame)
constexpr uint32_t VT_ANIM_GET_ANIMATION_DATA = 0x88;    // ArmatureAnimation::getAnimationData
constexpr uint32_t VT_WIDGET_SET_ENABLED = 0x298;        // ui::Widget::setEnabled
constexpr uint32_t VT_WIDGET_SET_TOUCH_ENABLED = 0x29C;  // ui::Widget::setTouchEnabled
constexpr uint32_t VT_SET_GLOBAL_Z_ORDER = 0x20;         // Node::setGlobalZOrder(float)
constexpr uint32_t VT_REMOVE_CHILD_BY_NAME = 0x134;      // Node::removeChildByName(name, cleanup)
constexpr uint32_t VT_SET_USER_OBJECT = 0x16C;           // Node::setUserObject(Ref*)

// ---- lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_LAMBDA_KEYBLADE_TOUCH = 0xF5D388;   // init $_0: keyblade button touch
constexpr uint32_t VT_LAMBDA_CUTIN_MOVEMENT = 0xF5D35C;   // init $_1: cut-in movement event
constexpr uint32_t VT_LAMBDA_DECKIN_FRAME = 0xF5D330;     // init $_2: Deck_in frame event
constexpr uint32_t VT_LAMBDA_DECKIN_MOVEMENT = 0xF5D304;  // init $_3: Deck_in movement event
constexpr uint32_t VT_LAMBDA_ATTACK_MOVEMENT = 0xF5D2D8;  // init $_4: MedalAttack_Normal movement event
constexpr uint32_t VT_LAMBDA_MEDAL_TOUCH = 0xF5D228;      // createMadalButton $_7: medal button touch
constexpr uint32_t VT_LAMBDA_GUILT_MOVEMENT = 0xF5D1D0;   // initGuiltBonusEffect $_9

// ---- strings
constexpr uint32_t STR_EMPTY = 0xDF8A1A;
constexpr uint32_t STR_FMT_PATH = 0xE01AB6;          // "%s/%s"
constexpr uint32_t STR_FMT_EXPORT_JSON = 0xE01ABC;   // "%s/%s.ExportJson"
constexpr uint32_t STR_PUBLISH = 0xE01ED0;           // "cocostudio/publish"
constexpr uint32_t STR_FMT_MEDAL = 0xE01E8F;         // "Medal_%02d"
constexpr uint32_t STR_FMT_IMAGE = 0xE01E79;         // "Image%02d"
constexpr uint32_t STR_FMT_CUTIN = 0xE01C62;         // "img/Cutin/Cutin_%d.png"
constexpr uint32_t STR_FMT_MEDAL_S = 0xDF90C5;       // "img/medal/Medal_S_%d.png"
constexpr uint32_t STR_ActionMap_KeyBladeDeck = 0xE01EB4, STR_DeckRoll01 = 0xE01EA9;
constexpr uint32_t STR_KeyBladeButton = 0xE01E9A, STR_AbilityInfo = 0xE01E83;
constexpr uint32_t STR_ActionMap_AbilityInfo = 0xE01E5E, STR_FriendlMedalUI = 0xE01B7B;
constexpr uint32_t STR_MedalPanel = 0xE01B38, STR_AttackMedal_Anm = 0xE01E4E, STR_AttackAbility_Anm = 0xE01E3C;
constexpr uint32_t STR_ActionMap_KeyBladeWindow = 0xE01E1E, STR_KeyBladeInfo = 0xE01E11;
constexpr uint32_t STR_ActMapBuffDeBuffAnim = 0xE01DFC, STR_ActMapDeckTacticalInfo_box = 0xE01DDC;
constexpr uint32_t STR_ActMapDeckTacticalInfo_panel = 0xE01DBA, STR_DeckInfo_Null = 0xE01DAC;
constexpr uint32_t STR_Medal_Deck_Label02 = 0xE01D99, STR_Medal_Deck_Label01 = 0xE01D86;
constexpr uint32_t STR_ActMapEnmUIAnim = 0xE01D76, STR_ActMapAAMedalAnim = 0xE01D64;
constexpr uint32_t STR_DeckEnemyTurn = 0xE01D56, STR_AttackAbilityStart = 0xE01D43, STR_LongTap = 0xE01D3B;
constexpr uint32_t STR_ActMapCarouselAnim = 0xE01D28, STR_DeckInfo_anim = 0xE01D1A, STR_Deck_in = 0xE01D12;
constexpr uint32_t STR_MedalAttack_Normal = 0xE01CFF, STR_Medal_Dummy = 0xE01CF3, STR_Cutin_Dummy = 0xE01C79;
constexpr uint32_t STR_GuiltBonus = 0xE01C57, STR_ActionMapAnimation = 0xE01C44;
constexpr uint32_t STR_DeckInfoCurrentEFF = 0xE01C31, STR_GuiltBIcon = 0xE01AE8, STR_GuiltBIcon_eff = 0xE01AD9;
constexpr uint32_t STR_Tutorial_FieldMask = 0xE01BEE, STR_Tutorial_txt = 0xE01BE1;
constexpr uint32_t STR_KeyBladeDeck = 0xE01C24, STR_DeckMedalNumUI = 0xE01C15;

// ---- more library / game functions
constexpr uint32_t Burst_create = 0x77A491;          // master::Burst::create(sret, id)
constexpr uint32_t Skill_create = 0x78BDE9;          // master::Skill::create(sret, id)
constexpr uint32_t TextAtlas_setString = rwlib::cocos2d::ui::TextAtlas::setString;   // ui::TextAtlas::setString(const std::string&)
constexpr uint32_t MedalDeck_forEachMedal = 0x5E007D;
constexpr uint32_t Size_ctor_copy = rwlib::cocos2d::Size::ctor_base__RKS0_;
constexpr uint32_t Place_create = rwlib::cocos2d::Place::create, RepeatForever_create = rwlib::cocos2d::RepeatForever::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t ActionObject_playCallFunc = rwlib::cocostudio::ActionObject::play;          // ActionObject::play(CallFunc*)
constexpr uint32_t Bone_changeDisplayWithIndex = rwlib::cocostudio::Bone::changeDisplayWithIndex;
constexpr uint32_t GuiltBurstCutInShader_setShaderStart = 0x4961A5;
constexpr uint32_t GuiltBurstCutInShader_setShaderCutinEnd = 0x4961F1;
constexpr uint32_t Widget_getTouchBeganPosition = rwlib::cocos2d::ui::Widget::getTouchBeganPosition, Widget_getTouchEndPosition = rwlib::cocos2d::ui::Widget::getTouchEndPosition;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;             // (sret shared_ptr<Layer>, mgr, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;                 // (layer, ResourceID, int, int)
constexpr uint32_t string_compare_cstr = 0x47CBD5;                // std::string::compare(const char*)
constexpr uint32_t LOG10 = 0x419E7C;                              // log10 (PLT)
constexpr uint32_t COST_STRINGS = 0xF5D168;          // const char*[10] "0".."9": a burst's gauge cost
constexpr uint32_t TARGET_ICON_NAMES = 0xF5D190;     // const char*[5] burst target icons (0 and 4 unused)
constexpr uint32_t SKILL_ICON_NAMES = 0xF5D1A4;      // const char*[6] skill icons (0 unused)

// ---- more virtual slots
constexpr uint32_t VT_GET_USER_OBJECT = 0x164;           // Node::getUserObject
constexpr uint32_t VT_ANIM_PAUSE = 0x0C;                 // ArmatureAnimation::pause
constexpr uint32_t VT_WIDGET_HIT_TEST = 0x2BC;           // ui::Widget::hitTest(const Vec2&)
constexpr uint32_t VT_WIDGET_SET_SIZE = 0x2B0;           // ui::Widget::setSize(const Size&)
constexpr uint32_t VT_LAYOUT_SET_CLIPPING = 0x31C;       // ui::Layout::setClippingEnabled
constexpr uint32_t VT_SPRITE_SET_TEXTURE = 0x278;        // Sprite::setTexture(const std::string&)

// ---- master data positions
constexpr uint32_t MEDAL_BURST_KIND = 0x4A8;     // master::Medal: 1 = has a special attack
constexpr uint32_t MEDAL_BURST_ID = 0x4AC;
constexpr uint32_t MEDAL_CUTIN_TYPE = 0x45C;     // 1 = light, 2 = dark cut-in
constexpr uint32_t MEDAL_GUILT_ICON = 0x478;     // guilt icon / shader index (1-based)
constexpr uint32_t BURST_NAME = 0x14, BURST_NAME_SIZE = 65;
constexpr uint32_t BURST_INFO = 0x55, BURST_INFO_SIZE = 257;
constexpr uint32_t BURST_TARGET = 0x160, BURST_COST = 0x168;
constexpr uint32_t BURST_POWER = 0x174, BURST_POWER_LV = 0x178;      // int, int[5] (per special attack level)
constexpr uint32_t BURST_POWER2 = 0x18C, BURST_POWER2_LV = 0x190;    // second hit
constexpr uint32_t BURST_TYPE = 0x1BC, BURST_FLAG = 0x1C0;
constexpr uint32_t SKILL_ICON = 8, SKILL_NAME = 12, SKILL_NAME_SIZE = 65;
// MedalDeck::MedalInfo (40 bytes): shared_ptr<master::Medal> at +0, the user medal SafeBuffer at +8
constexpr uint32_t INFO_MEDAL = 0, INFO_DATA = 8;
constexpr uint32_t UMEDAL_ATTACK = 0, UMEDAL_SUPPORT_USER = 8, UMEDAL_BURST_LEVEL = 16, UMEDAL_GUILT = 20;

// SceneUtil::searchNodeByName with a temporary std::string of a C string literal
inline uint32_t find(uint32_t root, uint32_t name) { return findNode(root, name, host_strlen(name)); }
inline void setVisible(uint32_t n, uint32_t visible) { vcall(n, node::setVisible, {n, visible}); }
// find + SceneUtil::setShadow
inline uint32_t findShadowed(uint32_t root, uint32_t name) {
    uint32_t t = find(root, name);
    SceneUtil::setShadow(t);
    return t;
}
// ui::TextAtlas::setString(c string)
inline void setAtlasC(uint32_t atlas, uint32_t cstr) {
    Frame f;
    uint32_t s = newStr(f, cstr);
    call(TextAtlas_setString, {atlas, s});
    strFree(s);
}
// node->setColor(Color3B(v, v, v))
inline void setGray(uint32_t n, uint32_t v) {
    Frame f;
    uint32_t c = f.alloc(4);
    call(fn::Color3B_ctor, {c, v, v, v});
    vcall(n, node::setColor, {n, c});
}
// `if (p) p->release()`
inline void releaseRef(uint32_t p) {
    if (p) call(Ref_release, {p});
}
}  // namespace cdeck

struct CarouselDeck {
    // libc++ std::function (24 bytes here: 16-byte buffer, the callable pointer at +16, 4 bytes of padding)
    struct Function {
        const uint8_t buf[16];
        rw::Field<uint32_t> callable;   // +16 == the object itself when stored inline
        const uint8_t pad_14[4];
    };

    rw::Field<uint32_t> vtable;                 // +0x000
    const uint8_t nodeBody[0x26C];                    // +0x004 cocos2d::Node
    rw::Field<uint32_t> root;                   // +0x270 ActionMap_KeyBladeDeck.json layout
    rw::Field<uint32_t> rollAction;             // +0x274 cocostudio::ActionObject "DeckRoll01" (retained)
    rw::Field<uint32_t> keybladeWindows[6];     // +0x278 ActionMap_KeyBladeWindow.json (retained), one per deck slot
    rw::Field<uint32_t> keybladeInfo;           // +0x290 "KeyBladeInfo"
    rw::Field<float> tickerTextX;               // +0x294 x of the burst info ticker text (first seen)
    rw::Field<float> tickerFieldWidth;          // +0x298 width of the ticker clipping field (first seen)
    rw::Field<uint32_t> tacticalBoxes[6];       // +0x29C ActMapDeckTacticalInfo_box.json (retained), one per slot
    rw::Field<uint32_t> tacticalPanel;          // +0x2B4 ActMapDeckTacticalInfo_panel.json (the deck info window)
    rw::Field<uint32_t> currentEffect;          // +0x2B8 armature "DeckInfoCurrentEFF" (retained)
    rw::Field<uint32_t> keybladeButton;         // +0x2BC "KeyBladeButton"
    rw::Field<uint32_t> friendMedalUi;          // +0x2C0 "FriendlMedalUI"
    rw::Field<uint32_t> medalNodes[5];          // +0x2C4 "Medal_01".."Medal_05"
    rw::Field<uint32_t> attackMedalAnm;         // +0x2D8 "AttackMedal_Anm"
    rw::Field<uint32_t> attackAbilityAnm;       // +0x2DC "AttackAbility_Anm"
    rw::Field<uint32_t> deckLabel02;            // +0x2E0 "Medal_Deck_Label02"
    rw::Field<uint32_t> deckLabel01;            // +0x2E4 "Medal_Deck_Label01"
    rw::Field<uint32_t> abilityInfos[3];        // +0x2E8 "AbilityInfo" of Medal_01..03
    rw::Field<uint32_t> abilityInfoWidgets[6];  // +0x2F4 ActionMap_AbilityInfo.json (retained), one per slot
    rw::Field<uint32_t> enemyTurnArmature;      // +0x30C "DeckEnemyTurn" (retained; all armatures below too)
    rw::Field<uint32_t> burstReadyArmature;     // +0x310 "AttackAbilityStart" (special attack possible)
    rw::Field<uint32_t> longTapArmature;        // +0x314 "LongTap" (burst encouragement)
    rw::Field<uint32_t> deckInfoArmature;       // +0x318 "DeckInfo_anim" (on the keyblade button)
    rw::Field<uint32_t> deckInArmature;         // +0x31C "Deck_in"
    rw::Field<uint32_t> medalAttackArmature;    // +0x320 "MedalAttack_Normal"
    rw::Field<uint32_t> cutinArmatures[4];      // +0x324 "MedalAttack_AtkAbilty" (+_Guilt, _Dark, _Dark_Guilt)
    rw::Field<uint32_t> medalDummy;             // +0x334 node replacing the "Medal_Dummy" bone of the attack
    rw::Field<uint32_t> cutinSprite;            // +0x338 img/Cutin/Cutin_31053.png (retained)
    rw::Field<uint32_t> guiltBonusArmature;     // +0x33C "GuiltBonus" (initGuiltBonusEffect)
    rw::Field<uint32_t> guiltBonusNum[3];       // +0x340 bones "GuiltBonusNum3", 2, 1
    rw::Field<uint32_t> guiltIcon;              // +0x34C bone "GuiltBIcon"
    rw::Field<uint32_t> guiltIconEffect;        // +0x350 bone "GuiltBIcon_eff"
    rw::Field<uint32_t> medalButtonCount;       // +0x354
    rw::Field<uint32_t> medalButtons[5];        // +0x358 ui::Button per deck medal (retained)
    rw::Field<uint32_t> friendMedalButton;      // +0x36C the friend (support) medal's button (retained)
    rw::Field<uint32_t> attackingMedal;         // +0x370 the button on the attack place (setDeckMedal)
    rw::Field<int32_t> deckSize;                // +0x374 1 (the keyblade) + medals added
    rw::Field<int32_t> currentIndex;            // +0x378 the medal whose turn it is
    rw::Field<int32_t> cutinMedalIds[5];        // +0x37C medal id per slot (convertMedalIDtoCutinMedalID)
    rw::Field<uint32_t> field_390;              // +0x390
    rw::Field<uint32_t> guiltValues[6];         // +0x394 guilt bonus per slot (user medal data +20; setDeckInfoWindow)
    rw::Field<uint32_t> field_3AC;              // +0x3AC
    Function isBurstPossible;                   // +0x3B0 std::function<bool()>
    Function isInfoWindowAllowed;               // +0x3C8 std::function<bool()>
    Function isAttackAllowed;                   // +0x3E0 std::function<bool()>
    rw::Field<uint8_t> inBattle;                // +0x3F8 setBattleState
    rw::Field<uint8_t> touching;                // +0x3F9 a medal button is held
    rw::Field<uint8_t> rolling;                 // +0x3FA deck roll animation running
    rw::Field<uint8_t> bursting;                // +0x3FB special attack animation running
    rw::Field<uint8_t> attacking;               // +0x3FC medal attack animation running
    const uint8_t pad_3FD[3];
    Function tutorialAction;                    // +0x400 std::function<void()> (setTutorialAction)
    rw::Field<uint32_t> guiltShader;            // +0x418 GuiltBurstCutInShader*
    rw::Field<uint32_t> field_41C;              // +0x41C

    static constexpr uint32_t SIZE = 0x420;
    static constexpr uint32_t VTABLE = 0xF902F8;   // _ZTV12CarouselDeck + 8

    // life cycle
    static uint32_t create();
    static CarouselDeck *construct(CarouselDeck *self);
    static CarouselDeck *destruct(CarouselDeck *self);
    static void destructDelete(CarouselDeck *self);
    static bool init(CarouselDeck *self);

    // armatures
    static uint32_t createArmature(CarouselDeck *self, uint32_t file, uint32_t movement);
    static uint32_t replaceBone(CarouselDeck *self, uint32_t armature, uint32_t bone);
    static uint32_t initGuiltBonusEffect(CarouselDeck *self, uint32_t file, uint32_t movement);
    static bool addArmature(CarouselDeck *self, uint32_t armature, uint32_t parent, int32_t zOrder);
    static bool removeArmature(CarouselDeck *self, uint32_t armature);
    static bool trackingNodePosition(CarouselDeck *self, uint32_t pos, uint32_t node, uint32_t stop);

    // deck
    static void setTutorialAction(CarouselDeck *self, uint32_t action);
    static void changeDeckStyle(CarouselDeck *self, uint32_t style);
    static void addMedalOntoDeck(CarouselDeck *self, uint32_t medalIds, int32_t count);
    static uint32_t createMadalButton(CarouselDeck *self, int32_t medalId);
    static int32_t convertMedalIDtoCutinMedalID(CarouselDeck *self, int32_t medalId);
    static bool reflashDeck(CarouselDeck *self);
    static void setPossibleBurst(CarouselDeck *self, uint32_t possible, uint32_t show);
    static void setBurstEncourage(CarouselDeck *self, uint32_t on);
    static void onCenterButton(CarouselDeck *self, uint32_t on);
    static void finishRollAnimation(CarouselDeck *self);
    static void finishActionAttack(CarouselDeck *self);
    static void finishActionBurst(CarouselDeck *self, uint32_t armature);
    static void disableButton(CarouselDeck *self);
    static void setBattleState(CarouselDeck *self, uint32_t on);
    static bool isVisibleDeckInfoWindow(CarouselDeck *self);
    static void onAutoButton(CarouselDeck *self);
    static bool isEffectiveBurstRound(CarouselDeck *self);
    static void addFriendMedalOntoDeck(CarouselDeck *self, int32_t medalId, int32_t skillId);
    static void advanceDeck(CarouselDeck *self);
    static void actionAttack(CarouselDeck *self, uint32_t burst);
    static void playGuiltBonusEffect(CarouselDeck *self, uint32_t medal, uint32_t guilt);
    static void setDeckMedal(CarouselDeck *self, uint32_t unused);

    // deck info window (CarouselDeck_Info.cpp)
    static void setAbilityInfo(CarouselDeck *self, uint32_t ability, uint32_t window, int32_t medalId);
    static void setDeckMedalInfoPanel(CarouselDeck *self, uint32_t box, int32_t medalId, uint32_t friendMedal,
                                      int32_t skillId);
    static void setDeckMedalInfoPanelInfo(CarouselDeck *self, uint32_t box, uint32_t info);
    static void setDeckMedalInfoSkill(CarouselDeck *self, uint32_t box, int32_t skillId);
    static void setBurstPower(CarouselDeck *self, uint32_t info, uint32_t typeLabel, uint32_t valueLabel);
    static void controlTargetIcon(CarouselDeck *self, uint32_t widget, int32_t target);
    static void controlSkillIcon(CarouselDeck *self, uint32_t widget, int32_t icon);
    static void setTickerAction(CarouselDeck *self);
    static void setDeckInfoWindow(CarouselDeck *self, uint32_t medalDeck);
    static void admission(CarouselDeck *self);
    static void setDeckInfoWindowLayout(CarouselDeck *self);
    static void setActiveMedalBurstCost(CarouselDeck *self);
    static void setVisibleDeckInfoWindow(CarouselDeck *self, uint32_t visible);
};
RW_LAYOUT(CarouselDeck, root, 0x270);
RW_LAYOUT(CarouselDeck, tacticalPanel, 0x2B4);
RW_LAYOUT(CarouselDeck, abilityInfoWidgets, 0x2F4);
RW_LAYOUT(CarouselDeck, enemyTurnArmature, 0x30C);
RW_LAYOUT(CarouselDeck, guiltBonusArmature, 0x33C);
RW_LAYOUT(CarouselDeck, medalButtons, 0x358);
RW_LAYOUT(CarouselDeck, cutinMedalIds, 0x37C);
RW_LAYOUT(CarouselDeck, isBurstPossible, 0x3B0);
RW_LAYOUT(CarouselDeck, isInfoWindowAllowed, 0x3C8);
RW_LAYOUT(CarouselDeck, isAttackAllowed, 0x3E0);
RW_LAYOUT(CarouselDeck, guiltValues, 0x394);
RW_LAYOUT(CarouselDeck, guiltShader, 0x418);
RW_LAYOUT(CarouselDeck, inBattle, 0x3F8);
RW_LAYOUT(CarouselDeck, tutorialAction, 0x400);
RW_SIZE(CarouselDeck, 0x420);
