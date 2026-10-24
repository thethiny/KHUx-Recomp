// StartDeckEditDialog rewrites: guest addresses, lambda vtables and small helpers shared by its files.
#pragma once

#include "StartDeckEditDialog.hpp"
#include "../core/SafeBuffer.hpp"

namespace sded {

using namespace w4b1;
using Self = StartDeckEditDialog;

inline Self *S(uint32_t a) { return rw::host_ptr<Self>(a); }
inline uint32_t A(const uint8_t *p) { return rw::guest_addr(p); }
inline void addRef(uint32_t ctrl) { if (ctrl) guest::addRef(ctrl); }
inline void release(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }

// lambda vtables (__func + 8)
constexpr uint32_t VT_init_0 = 0xF59DAC, VT_init_1 = 0xF59D80, VT_open_2 = 0xF59D28;
constexpr uint32_t VT_layout_3 = 0xF59CFC, VT_layout_4 = 0xF59CD0, VT_layout_5 = 0xF59CA4, VT_layout_6 = 0xF59C78;
constexpr uint32_t VT_layout_7 = 0xF59BC8, VT_layout_8 = 0xF59B70;
constexpr uint32_t VT_deck_9 = 0xF59AEC, VT_deck_10 = 0xF59AC0, VT_deck_11 = 0xF59A94, VT_deck_12 = 0xF59A68;
constexpr uint32_t VT_deckView_13 = 0xF59A3C;
constexpr uint32_t VT_support_15 = 0xF599E4, VT_support_16 = 0xF599B8, VT_support_17 = 0xF5998C;
constexpr uint32_t VT_close_18 = 0xF59960, VT_runIn_19 = 0xF59934, VT_runIn_20 = 0xF59908;
constexpr uint32_t VT_runOut_21 = 0xF598DC, VT_runOut_22 = 0xF598B0;
constexpr uint32_t VT_story_23 = 0xF59884, VT_story_24 = 0xF59858, VT_raid_25 = 0xF5982C, VT_raid_26 = 0xF59800;
constexpr uint32_t VT_coliseum_27 = 0xF597D4, VT_coliseum_28 = 0xF597A8;

// strings
constexpr uint32_t S_PLUS = 0xDF8A93, S_SUPPORT = 0xDFF8A4, S_BUTTON_CLOSE = 0xDF8692, S_BUTTON = 0xDFD6D5;
constexpr uint32_t S_BUTTON_OK = 0xDF8688, S_SKIP = 0xDFF8AC, S_BUTTON_SKIP = 0xDFF8B1, S_ANNOUNCE = 0xDFF8BD;
constexpr uint32_t S_RAID_INFO = 0xDFF8C6, S_COLOSSEUM_INFO = 0xDFF8CF, S_RAID_PANEL = 0xDFF867;
constexpr uint32_t S_RAID_INFO_JSON = 0xDFF8DD, S_RAID_INFO2_JSON = 0xDFF912;
constexpr uint32_t S_KB = 0xDFEF39, S_DECK = 0xDFF948, S_BASE = 0xDFEF5A, S_LEFT_UI = 0xDF85F0, S_RIGHT_UI = 0xDF85F7;
constexpr uint32_t S_EMPTY = 0xDF8A1A;
constexpr uint32_t S_TXT_OK = 0xDF869F, S_TXT = 0xDF94D1, S_TXT_SKIP = 0xDFF9D7, S_TXT_NUM = 0xDFF9E0;
constexpr uint32_t S_SUBCLEAR1_BASE = 0xDFF4A9, S_SUBCLEAR2_BASE = 0xDFF4C2, S_SUBCLEAR3_BASE = 0xDFF4DB;
constexpr uint32_t S_SUBCLEAR = 0xDFF9E8, S_PANEL_KB = 0xDFF94D, S_AREA = 0xDF9378, S_AVATAR = 0xDF8F56;
constexpr uint32_t S_SUPPORT_WIN = 0xDFF956, S_SCENE_JSON = 0xDFF871;   // StartDeckEdit2Scene_ver120.json (50 chars)

// guest functions
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base, Node_dtor = rwlib::cocos2d::Node::dtor_base, Node_init = rwlib::cocos2d::Node::init;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t AvatarIconCache_dtor = 0x5CA8FD;
constexpr uint32_t CampaignVector_baseDtor = 0x573591;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;
constexpr uint32_t FunctionVB_assign = 0x490109;          // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVV_copy = 0x4683CD;
constexpr uint32_t FunctionWidgetInt_swap = 0x54B1C5;     // std::function<Widget*(int)>::swap
constexpr uint32_t FunctionWidgetW_swap = 0x547159;       // std::function<Widget*(Widget*, int, bool)>::swap
constexpr uint32_t FunctionVI_swap = 0x5428E1;            // std::function<void(int)>::swap
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_executeStageSupportList = 0x4CB0A9;
constexpr uint32_t APIManager_executeStageStart = 0x4CB22D;
constexpr uint32_t APIManager_executeColosseumStart = 0x4D15AD;
constexpr uint32_t APIManager_executeRaidStart = 0x4CC581;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_openPopup = 0x61524D;
constexpr uint32_t SceneManager_hideHeader = 0x61154D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_createRightInAnimation = 0x45B8BD;
constexpr uint32_t SceneUtil_createRightOutAnimation = 0x45BA3D;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;
constexpr uint32_t SceneUtil_updateDeckView = 0x46A6E5;
constexpr uint32_t SceneUtil_changePartnerAvatarPanel = 0x46B15D;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t SceneUtil_calcDeckStatus = 0x469CB5;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_setTutorialCloseFunc = 0x4EB8D9;
constexpr uint32_t AdvStageScrollView_setRaidBossInfo = 0x56C211;
constexpr uint32_t AdvStageScrollView_setBossInfo = 0x56A679;
constexpr uint32_t AdvStageScrollView_setColosseumInfo = 0x56D65D;
constexpr uint32_t StageUtil_findStory = 0x764101;
constexpr uint32_t BaseSlideView_create = 0x534E79;
constexpr uint32_t BaseSlideView_setScreenWidth = 0x526595;
constexpr uint32_t BaseSlideView_setIndex = 0x526141;
constexpr uint32_t BaseSlideView_setItemIDs = 0x5260AD;
constexpr uint32_t BaseSlideView_setSlideEnabled = 0x526111;
constexpr uint32_t IntVector_copy = 0x4AF707;             // std::vector<int>::vector(const vector&)
constexpr uint32_t U64Vector_pushBackSlow = 0x4ECB6D;     // std::vector<uint64>::__push_back_slow_path(const&)
constexpr uint32_t U64Vector_baseDtor = 0x468499;
constexpr uint32_t ImageButton_create = 0x531239;         // (std::string, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;  // (btn, node, float)
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t SupportScroll_create = 0x503689;
constexpr uint32_t SupportScroll_setSelectedID = 0x504899;
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t BaseScrollViewU64_setItemIDs = 0x4FD231;
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D;
constexpr uint32_t KeyBladeDataManager_getKeyBladeData = 0x76B659;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t MedalVector_pushBackSlow = 0x487A39;
constexpr uint32_t MedalVector_baseDtor = 0x46B119;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create, FadeIn_create = rwlib::cocos2d::FadeIn::create, FadeOut_create = rwlib::cocos2d::FadeOut::create;
constexpr uint32_t Size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;

// Node / Widget virtual slots (byte offsets)
constexpr uint32_t VS_setZOrder10 = 0x10, VS_getLocalZOrder = 0x1C, VS_setPosition = 0x4C, VS_getPosition = 0x54;
constexpr uint32_t VS_getPositionX = 0x68, VS_setContentSize = 0xA8, VS_getContentSize = 0xAC, VS_setVisible = 0xB0;
constexpr uint32_t VS_addChild = 0xF0, VS_addChildZ = 0xF4, VS_addChildZName = 0xFC, VS_getChildren = 0x10C;
constexpr uint32_t VS_removeAllChildren = 0x138, VS_getOpacity = 0x20C, VS_setOpacity = 0x214;
constexpr uint32_t VS_init = 0x248, VS_setTouchEnabled = 0x29C, VS_slide31C = 0x31C;

// SceneManager / APIManager fields
constexpr uint32_t SM_KEYBLADE_ID = 0x2B0;     // uint64 selected keyblade
constexpr uint32_t SM_SUPPORT_ID = 0x2C0;      // uint64 selected support user
constexpr uint32_t SM_STAGE_ID = 0x2C8;
constexpr uint32_t API_USER_STATUS = 0x8C, API_USER_DETAIL = 0x94, API_KEYBLADES = 0xEC, API_EVENTS = 0x12C;
constexpr uint32_t API_SUPPORTS = 0x134, API_RAID_INFO = 0x14C, API_RAID_LIST = 0x154, API_COLOSSEUM = 0x24C;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline uint32_t api() { return call(APIManager_getInstance, {}); }

// n words of a SafeBuffer decoded (SafeBuffer::read into a temporary, as host values)
inline void safeWords(uint32_t sb, uint32_t index, uint32_t *out, uint32_t n) {
    const SafeBuffer *s = scene::safe(sb);
    for (uint32_t k = 0; k < n; k++) out[k] = uint32_t(s->get32(index + k * 4));
}

// one decoded byte of a SafeBuffer
inline uint32_t safe8(uint32_t sb, uint32_t index) {
    const SafeBuffer *s = scene::safe(sb);
    return uint8_t(s->data[index] ^ SafeBuffer::mask(s->key, index));
}

// A stack std::function holding an inline lambda; returns its address (destroy with functionDestroy)
inline uint32_t inlineFn(Frame &f, uint32_t vtable, std::initializer_list<uint32_t> caps) {
    return newFunction(f, vtable, caps);
}
}  // namespace sded
