// SceneAdventureSelect rewrites: the guest functions, lambda vtables, strings and foreign offsets they use.
#pragma once

#include "SceneAdventureSelect.hpp"
#include "../scene/SceneUtil.hpp"

namespace sas {

using namespace scene;

namespace lib {
// cocos2d / cocostudio
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v;                  // Vec2::Vec2() (0, 0)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t MEMSET = 0x40E7A0;                     // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;         // std::vector .at() failure (never returns)
// libc++ containers
constexpr uint32_t DateVector_dtor = 0x473611;            // std::__vector_base<Date>::~__vector_base
constexpr uint32_t StringVector_dtor = 0x47363F;          // std::__vector_base<std::string>::~__vector_base
// game
constexpr uint32_t APIManager_executeCampaignGet = 0x4D346D;
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815;
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t createTimeLeftString = 0x492B75;
constexpr uint32_t StoryVector_copy = 0x4B5BC9;           // std::vector<Stage::Story>(const vector&)
constexpr uint32_t StoryVector_dtor = 0x4B58EF;           // std::__vector_base<Stage::Story>::~__vector_base       // hole::text::createTimeLeftString(string out, long)
constexpr uint32_t BaseStageScrollView_migrationLeft = 0x59DA99;
constexpr uint32_t BaseStageScrollView_migrationRight = 0x59DAB3;
constexpr uint32_t BaseStageScrollView_isScroll = 0x59DC19;
constexpr uint32_t AdvStageScrollView_updateRaidBossInfoTime = 0x56CBC9;
// AdventurePanel (12-byte panel records inside the scene)
constexpr uint32_t AdventurePanel_searchWidget = 0x71EF59;
constexpr uint32_t AdventurePanel_createWidget = 0x723955;
constexpr uint32_t AdventurePanel_runLeftInAnimation = 0x7264D9;
constexpr uint32_t AdventurePanel_runRightInAnimation = 0x726115;
constexpr uint32_t AdventurePanel_runBottomInAnimation = 0x7261CD;
constexpr uint32_t AdventurePanel_runSelectOutAnimation = 0x72592D;
constexpr uint32_t AdventurePanel_runTopOutAnimation = 0x725AA9;
constexpr uint32_t AdventurePanel_runRightOutAnimation = 0x725B55;
constexpr uint32_t AdventurePanel_runLeftOutAnimation = 0x723D11;
constexpr uint32_t AdventurePanel_runFadeOutAnimation = 0x725865;
constexpr uint32_t AdventurePanel_runBottomOutAnimation = 0x725C01;
constexpr uint32_t AdventurePanel_runInfoOutAnimation = 0x7256DD;
}  // namespace lib

// cocos2d::Node virtual slots used here (byte offsets from the vtable pointer)
namespace vslot {
constexpr uint32_t deleteSelf = 0x04;                     // deleting destructor
constexpr uint32_t init = 0x248;                          // Node::init()
}  // namespace vslot


// libraries used by init() and the panels
namespace lib {
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff;                // Vec2::Vec2(float, float)
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;          // std::function<void()>::operator=(const function&)
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneManager_setBackButtonType = 0x613E27;
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;   // (mgr, BackGroundType, const string&)
constexpr uint32_t SceneManager_hideTitleHeader = 0x6130B1;    // (mgr, function<void()>)
constexpr uint32_t ImageButton_create = 0x531239;         // (string file, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;  // (button, Node*, float scale)
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;            // (const string& name)
}  // namespace lib

// virtual slots (byte offsets from the vtable pointer) beyond scene::node
namespace vslot {
constexpr uint32_t widgetSetEnabled = 0x298;              // ui::Widget::setEnabled(bool)
constexpr uint32_t widgetSetTouchEnabled = 0x29C;         // ui::Widget::setTouchEnabled(bool)
constexpr uint32_t armatureGetAnimation = 0x298;          // cocostudio::Armature::getAnimation()
constexpr uint32_t animationPlay = 0x68;                  // ArmatureAnimation::play(const string&, int, int)
constexpr uint32_t runInAnimation = 0x300;                // SceneAdventureSelect::runInAnimation (own vtable)
constexpr uint32_t runOutAnimationMain = 0x304;           // SceneAdventureSelect::runOutAnimationMain
}  // namespace vslot

// SceneManager fields
constexpr uint32_t SM_ADVENTURE_MODE = 0x28C;             // int: how the scene was entered (2..4 event, 5, 9)
constexpr uint32_t SM_FLAG_2A8 = 0x2A8;                   // byte cleared by init()
constexpr uint32_t SM_EVENT_ID = 0x2F0;                   // int: event to show (StageEvent::Event id at +0xC)
constexpr uint32_t SM_FLAG_2F4 = 0x2F4;                   // byte copied to flag_29F
constexpr uint32_t SM_FLAG_2F6 = 0x2F6;                   // byte
constexpr uint32_t SM_FILTER_ENABLED = 0x30A;             // byte: the stage filter was unlocked (tutorial > 994)


// changeAdventure / initAdvScrollView / initStory
namespace lib {
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t AppState_getAdventureSelectFilterState = 0x4EB1A1;   // (shared_ptr out, AppState*)
constexpr uint32_t APIManager_executeColosseumGet = 0x4D1325;
constexpr uint32_t AdvStageScrollView_create = 0x7249FD;
constexpr uint32_t AdvStageScrollView_initNewStage = 0x570BDD;          // (view, Widget* cursor) -> bool
constexpr uint32_t AdvStageScrollView_resetScrollPosition = 0x570A91;
constexpr uint32_t AdvStageScrollView_initRaidBoss = 0x570E7D;
constexpr uint32_t BaseStageScrollView_migrationMaxPosition = 0x59DAC9;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                  // Size::Size(float, float)
constexpr uint32_t FunctionVI_swap = 0x5428E1;            // std::function<void(int)>::swap
constexpr uint32_t FunctionVV_swap = 0x4EE86D;            // std::function<void()>::swap
constexpr uint32_t FunctionVF_swap = 0x5E40BD;            // std::function<void(float)>::swap
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;
}  // namespace lib
// AdvStageScrollView callbacks (std::function members)
constexpr uint32_t VIEW_CALLBACK_640 = 0x640;             // function<void(int)>
constexpr uint32_t VIEW_CALLBACK_560 = 0x560;             // function<void()>
constexpr uint32_t VIEW_CALLBACK_500 = 0x500;             // function<void(int)>
constexpr uint32_t VIEW_CALLBACK_530 = 0x530;             // function<void(float)>


// initAdvSelect
namespace lib {
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;           // ui::Widget::setBright(bool)
constexpr uint32_t BaseStageScrollView_setEnabledScroll = 0x59D63D;
}  // namespace lib
constexpr uint32_t API_USER = 0x94;                       // SafeBuffer* of the user's data (+0x40: rank)
constexpr uint32_t API_BENEFITS = 0xAC;                   // std::vector<...>* (the user's benefits)
constexpr uint32_t USER_RANK = 0x40;



// animations
namespace lib {
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t BaseStageScrollView_getItemIDs = 0x59D341;   // (vector<int> out, view)
constexpr uint32_t IntVector_dtor = 0x478961;             // std::__vector_base<int>::~__vector_base
}  // namespace lib
// node->runAction(EaseSineOut::create(action))
inline void runEased(uint32_t node, uint32_t action) {
    call(fn::Node_runAction, {node, call(lib::EaseSineOut_create, {action})});
}
// node->runAction(EaseSineOut::create(create(node, 0, 0, std::function<void()>())))
inline void runEasedEmpty(uint32_t node, uint32_t (*create)(uint32_t, float, float, uint32_t)) {
    Frame f;
    uint32_t none = newEmptyFunction(f);
    runEased(node, create(node, 0.0f, 0.0f, none));
    functionDestroy(none);
}
// a std::function<void()> at `dst` holding a heap lambda {vtable, std::function moved from `copy` at +8}
// (0x20 bytes: the completion lambdas that just forward to the callback they captured)
inline void forwardingLambda(uint32_t dst, uint32_t vtable, uint32_t copy) {
    heapFunction(dst, vtable, 0x20, 8, fn::function_vv_move, copy);
}


// changeStageInfo
namespace lib {
constexpr uint32_t IntVector_assign = 0x47D5BD;           // std::vector<int>::assign(int*, int*)
constexpr uint32_t EvCampaign_create = 0x781DD9;          // master::EvCampaign::create(shared_ptr out, id)
constexpr uint32_t CampaignFilter_call = 0x474475;        // function<bool(int, shared_ptr<EvCampaign>&)>::operator()
constexpr uint32_t CampaignFilter_moveAssign = 0x474425;  // ... ::operator=(function&&)
constexpr uint32_t CampaignAnimationView_storyTargetCampaignFunc = 0x598A65;
constexpr uint32_t CampaignAnimationView_eventTargetCampaignFunc = 0x598A81;
constexpr uint32_t CampaignAnimationView_colosseumTargetCampaignFunc = 0x598A9D;
constexpr uint32_t CampaignAnimationView_raidTargetCampaignFunc = 0x598AB9;
constexpr uint32_t CampaignAnimationView_setCampaings = 0x7258E9;
constexpr uint32_t CampaignAnimationView_create = 0x598AF1;   // (Node* parent, vector<shared_ptr<EvCampaign>>&)
constexpr uint32_t CampaignAnimationView_hide = 0x598F01;
constexpr uint32_t CampaignVector_pushBackSlow = 0x599229;
constexpr uint32_t CampaignVector_dtor = 0x573591;
}  // namespace lib
constexpr uint32_t API_CAMPAIGN_IDS = 0x2FC;              // std::vector<int> of running campaign ids (begin, end)
constexpr uint32_t STORY_SIZE = 24;                       // hole::network::api::Stage::Story: id, 2 words, vector<int>
constexpr uint32_t STORY_DATA_CURRENT = 0xC;              // *(APIManager +0x124) +0xC: the story stage reached
constexpr uint32_t RAID_INFO_STAGE = 0x2C;                // RaidBase::Info: stage id the boss is on
constexpr uint32_t RAID_INFO_HP = 0x34;                   // RaidBase::Info: boss hit points left
// (end - begin) / 24 as compiled ((bytes >> 3) * inverse of 3)
inline uint32_t storyCount(uint32_t begin, uint32_t end) {
    return uint32_t(int32_t(end - begin) >> 3) * 0xAAAAAAABu;
}


// stage lists
namespace lib {
constexpr uint32_t EvStage_create = 0x782D2D;             // master::EvStage::create(shared_ptr out, id)
constexpr uint32_t Stage_create = 0x78E4E9;               // master::Stage::create(shared_ptr out, id)
constexpr uint32_t Enemy_create = 0x77F7DD;               // master::Enemy::create(shared_ptr out, id)
constexpr uint32_t BaseStageScrollView_setIndex = 0x59D5A9;
constexpr uint32_t BaseStageScrollView_setItemIDs = 0x59D351;   // (view, vector<int> by value)
constexpr uint32_t AdvStageScrollView_setSelectItem = 0x5708D1;
constexpr uint32_t AdvStageScrollView_setStageType = 0x57186D;   // (view, StageType 0 story / 1 event / 2 colosseum)
constexpr uint32_t IntVector_copy = 0x4AF707;             // std::vector<int>(const vector&)
constexpr uint32_t IntVector_pushBackSlow = 0x4D6CC1;     // std::vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t FileUtils_getInstance = rwlib::cocos2d::FileUtils::getInstance;
constexpr uint32_t Data_getBytes = rwlib::cocos2d::Data::getBytes;
constexpr uint32_t Data_dtor = rwlib::cocos2d::Data::dtor_base;
constexpr uint32_t StageManager_getInteger32bitFromLittleEndianBytes = 0x75992B;
constexpr uint32_t ColosseumStageVector_copy = 0x54E489;
constexpr uint32_t ColosseumStageVector_dtor = 0x4B3A95;
}  // namespace lib
constexpr uint32_t API_COLOSSEUM = 0x24C;                 // shared_ptr<Colosseum info>: object (+4 open, +8 vector<Stage>)
constexpr uint32_t API_COLOSSEUM_CTRL = 0x250;
constexpr uint32_t SM_STORY_STAGE = 0x2C8;                // int: the story stage the list was left on
constexpr uint32_t EVENT_STAGE_ID = 0xC;                  // StageEvent::Event: EvStage id
constexpr uint32_t EVSTAGE_KIND = 0x28;                   // master::EvStage SafeBuffer: kind (3 = ...)


// sounds
namespace lib {
constexpr uint32_t AudioManager_getInstance = 0x5BECFD;
constexpr uint32_t AudioManager_getLayer = 0x5BF33D;      // (shared_ptr<Layer> out, Manager*, int layer)
constexpr uint32_t AudioLayer_playSE = 0x5BF785;          // (Layer*, ResourceID, int, int)
constexpr uint32_t RaidSetting_create = 0x78A62D;         // master::RaidSetting::create(shared_ptr out, id)
}  // namespace lib
constexpr uint32_t SE_DECIDE = 509000010;
constexpr uint32_t SE_ERROR = 509000180;
// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
inline void playSE(uint32_t id) {
    uint32_t mgr = call(lib::AudioManager_getInstance, {});
    Frame f;
    uint32_t layer = f.alloc(8);
    call(lib::AudioManager_getLayer, {layer, mgr, 0});
    call(lib::AudioLayer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
}

// AdventurePanel::run*Animation(panel, std::function<void()>()) with an empty callback
inline void runPanel(uint32_t animation, uint32_t panel) {
    Frame f;
    uint32_t none = newEmptyFunction(f);
    call(animation, {panel, none});
    functionDestroy(none);
}
// fn(self, std::function<void()> copy of `done`) - a callback passed on by value
inline void callWithCopy(void (*fn)(uint32_t, uint32_t), uint32_t self, uint32_t done) {
    Frame f;
    uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    fn(self, copy);
    functionDestroy(copy);
}

// cocos2d::Node std::function<void()> members
constexpr uint32_t NODE_ON_ENTER = 0x210;
constexpr uint32_t NODE_ON_EXIT = 0x228;
constexpr uint32_t STR_EMPTY = 0xDF8A1A;                  // ""

// small node helpers
inline void setVisible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void setOpacity(uint32_t n, uint32_t v) { vcall(n, node::setOpacity, {n, v}); }
inline void searchWidget(uint32_t panel, uint32_t root, uint32_t name) {
    call(lib::AdventurePanel_searchWidget, {panel, root, name});
}
// member = lambda{vtable, captures} (a std::function<void()> member assigned from an inline temporary)
inline void assignFunction(uint32_t member, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, captures);
    call(lib::FunctionVV_assign, {member, fo});
    functionDestroy(fo);
}

// APIManager fields
constexpr uint32_t API_STAGE_EVENTS = 0x12C;              // std::vector<StageEvent::Event>* (56 bytes each)
constexpr uint32_t API_STORIES = 0x124;                   // std::vector<Stage::Story>*
constexpr uint32_t API_RAID_INFO = 0x14C;                 // shared_ptr<RaidBase::Info>: object
constexpr uint32_t API_RAID_INFO_CTRL = 0x150;            // ... control block
constexpr uint32_t RAID_INFO_STATE = 0xF0;                // RaidBase::Info: 1 defeated, 2 escaped, 3 up
// hole::network::api::StageEvent::Event
constexpr uint32_t EVENT_SIZE = 56;
constexpr uint32_t EVENT_END_DATE = 0x28;                 // Date

// Lambda __func vtables (the value stored at +0 of the __func: the _ZTV symbol + 8)
constexpr uint32_t VT_runOutAnimationMain_68 = 0xF6BE28;
constexpr uint32_t VT_backScene_67 = 0xF6BE54;
constexpr uint32_t VT_backScene_66 = 0xF6BE80;
constexpr uint32_t VT_backScene_65 = 0xF6BEAC;
constexpr uint32_t VT_checkAp_64 = 0xF6BED8;
constexpr uint32_t VT_checkApColosseum_63 = 0xF6BF04;
constexpr uint32_t VT_checkApEvent_62 = 0xF6BF30;
constexpr uint32_t VT_checkApStory_61 = 0xF6BF5C;
constexpr uint32_t VT_backButtonRunOutAnimation_60 = 0xF6BF88;
constexpr uint32_t VT_backButtonRunInAnimation_59 = 0xF6BFB4;
constexpr uint32_t VT_scrollRunOutAnimation_58 = 0xF6BFE0;
constexpr uint32_t VT_openPopupStartDeckEdit_57 = 0xF6C00C;
constexpr uint32_t VT_openPopupStartDeckEdit_56 = 0xF6C038;
constexpr uint32_t VT_raidBossRewardPopup_55 = 0xF6C064;
constexpr uint32_t VT_raidBossEscapeAnim_54 = 0xF6C0E8;
constexpr uint32_t VT_raidBossAnim_53 = 0xF6C114;
constexpr uint32_t VT_initStageRaidChack_52 = 0xF6C16C;
constexpr uint32_t VT_colosseumOpenAnimation_51 = 0xF6C198;
constexpr uint32_t VT_colosseumOpenAnimation_50 = 0xF6C1C4;
constexpr uint32_t VT_storyInAnimation_48 = 0xF6C1F0;
constexpr uint32_t VT_changeStageInfo_47 = 0xF6C21C;
constexpr uint32_t VT_changeStageInfo_46 = 0xF6C274;
constexpr uint32_t VT_changeStageInfo_45 = 0xF6C2CC;
constexpr uint32_t VT_changeStageInfo_44 = 0xF6C324;
constexpr uint32_t VT_changeStageInfo_43 = 0xF6C400;
constexpr uint32_t VT_initAdvScrollView_42 = 0xF6C458;
constexpr uint32_t VT_initAdvScrollView_41 = 0xF6C484;
constexpr uint32_t VT_initAdvScrollView_40 = 0xF6C4B0;
constexpr uint32_t VT_initAdvScrollView_39 = 0xF6C4DC;
constexpr uint32_t VT_initColosseum_38 = 0xF6C508;
constexpr uint32_t VT_initColosseum_37 = 0xF6C560;
constexpr uint32_t VT_initColosseum_36 = 0xF6C5B8;
constexpr uint32_t VT_initColosseum_33 = 0xF6C63C;
constexpr uint32_t VT_initEvent_32 = 0xF6C668;
constexpr uint32_t VT_initEvent_31 = 0xF6C6C0;
constexpr uint32_t VT_initEvent_28 = 0xF6C718;
constexpr uint32_t VT_eventBannerDisps_27 = 0xF6C744;
constexpr uint32_t VT_initEventBanner_26 = 0xF6C770;
constexpr uint32_t VT_initEventBanner_25 = 0xF6C79C;
constexpr uint32_t VT_initEventBanner_24 = 0xF6C7C8;
constexpr uint32_t VT_initEventBanner_23 = 0xF6C878;
constexpr uint32_t VT_initEventBanner_22 = 0xF6C9AC;
constexpr uint32_t VT_initEventBanner_21 = 0xF6C9D8;
constexpr uint32_t VT_initStory_20 = 0xF6CA04;
constexpr uint32_t VT_initStory_19 = 0xF6CA30;
constexpr uint32_t VT_initStory_18 = 0xF6CA88;
constexpr uint32_t VT_initAdvSelect_17 = 0xF6CAB4;
constexpr uint32_t VT_initAdvSelect_16 = 0xF6CAE0;
constexpr uint32_t VT_initAdvSelect_15 = 0xF6CB90;
constexpr uint32_t VT_initAdvSelect_14 = 0xF6CC98;
constexpr uint32_t VT_initAdvSelect_13 = 0xF6CDA0;
constexpr uint32_t VT_changeAdventure_12 = 0xF6CE50;
constexpr uint32_t VT_changeAdventure_11 = 0xF6CE7C;
constexpr uint32_t VT_init_10 = 0xF6CED4;
constexpr uint32_t VT_init_9 = 0xF6CF00;
constexpr uint32_t VT_init_8 = 0xF6CF2C;
constexpr uint32_t VT_init_7 = 0xF6CF58;
constexpr uint32_t VT_init_6 = 0xF6CF84;
constexpr uint32_t VT_init_5 = 0xF6CFB0;
constexpr uint32_t VT_init_4 = 0xF6D008;
constexpr uint32_t VT_init_3 = 0xF6D034;
constexpr uint32_t VT_init_2 = 0xF6D060;
constexpr uint32_t VT_init_1 = 0xF6D08C;
constexpr uint32_t VT_callAdventureSelectAPI_0 = 0xF6D0B8;

// Strings
constexpr uint32_t STR_LeftUI = 0xDF85F0;                                      // "LeftUI"
constexpr uint32_t STR_RightUI = 0xDF85F7;                                     // "RightUI"
constexpr uint32_t STR_Txt_Wording = 0xDF862F;                                 // "Txt_Wording"
constexpr uint32_t STR_stage_mappoi_stg_05d_bin = 0xDF9089;                    // "stage/mappoi_stg%05d.bin"
constexpr uint32_t STR_Area = 0xDF9378;                                        // "Area"
constexpr uint32_t STR_Txt_Title4 = 0xDF9D19;                                  // "Txt_Title4"
constexpr uint32_t STR_Txt_Limit = 0xDF9EED;                                   // "Txt_Limit"
constexpr uint32_t STR_Area_Point = 0xDF9FB4;                                  // "Area_Point"
constexpr uint32_t STR_Animation1 = 0xDFD0AF;                                  // "Animation1"
constexpr uint32_t STR_Raid_Boss_Icon = 0xDFD2D3;                              // "Raid_Boss_Icon"
constexpr uint32_t STR_Story = 0xDFF3CF;                                       // "Story"
constexpr uint32_t STR_Left_arrow = 0xDFF5DA;                                  // "Left_arrow"
constexpr uint32_t STR_Right_arrow = 0xDFF5E5;                                 // "Right_arrow"
constexpr uint32_t STR_Cursor = 0xDFF78E;                                      // "Cursor"
constexpr uint32_t STR_Announce = 0xDFF8BD;                                    // "Announce"
constexpr uint32_t STR_RaidInfo = 0xDFF8C6;                                    // "RaidInfo"
constexpr uint32_t STR_ColosseumInfo = 0xDFF8CF;                               // "ColosseumInfo"
constexpr uint32_t STR_Txt_Description = 0xE00690;                             // "Txt_Description"
constexpr uint32_t STR_lwf = 0xE02256;                                         // "lwf"
constexpr uint32_t STR_Button_Relief = 0xE031F2;                               // "Button_Relief"
constexpr uint32_t STR_Button_Relief_Ico = 0xE039A4;                           // "Button_Relief_Ico"
constexpr uint32_t STR_Button_Colosseum = 0xE039CC;                            // "Button_Colosseum"
constexpr uint32_t STR_Button_EV = 0xE039EC;                                   // "Button_EV"
constexpr uint32_t STR_img_ui_Equip_DeckBG_01_png = 0xE03AF2;                  // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t STR_Button_Back = 0xE03DBA;                                 // "Button_Back"
constexpr uint32_t STR_Txt_Filter = 0xE04442;                                  // "Txt_Filter"
constexpr uint32_t STR_Button_Filter = 0xE055B1;                               // "Button_Filter"
constexpr uint32_t STR_Win_Bottom = 0xE058DC;                                  // "Win_Bottom"
constexpr uint32_t STR_Title2 = 0xE06376;                                      // "Title2"
constexpr uint32_t STR_cocostudio_publish_AdventureTop2Scene_ve = 0xE06C94;    // "cocostudio/publish/AdventureTop2Scene_ver130.json"
constexpr uint32_t STR_RightUI_Button_Filter = 0xE06CC6;                       // "RightUI_Button_Filter"
constexpr uint32_t STR_LeftUI_Buck = 0xE06CDC;                                 // "LeftUI_Buck"
constexpr uint32_t STR_Cursor_Arrow = 0xE06CE8;                                // "Cursor_Arrow"
constexpr uint32_t STR_Shade_Left = 0xE06CF5;                                  // "Shade_Left"
constexpr uint32_t STR_Shade_Right = 0xE06D00;                                 // "Shade_Right"
constexpr uint32_t STR_Adv_Button = 0xE06D0C;                                  // "Adv_Button"
constexpr uint32_t STR_Adv_Panel_Area = 0xE06D17;                              // "Adv_Panel_Area"
constexpr uint32_t STR_deferment = 0xE06D26;                                   // "deferment"
constexpr uint32_t STR_Adv_Select = 0xE06D30;                                  // "Adv_Select"
constexpr uint32_t STR_QuestBut1 = 0xE06D3B;                                   // "QuestBut1"
constexpr uint32_t STR_QuestBut2 = 0xE06D45;                                   // "QuestBut2"
constexpr uint32_t STR_QuestBut3 = 0xE06D4F;                                   // "QuestBut3"
constexpr uint32_t STR_QuestBut4 = 0xE06D59;                                   // "QuestBut4"
constexpr uint32_t STR_Event = 0xE06D63;                                       // "Event"
constexpr uint32_t STR_Colosseum = 0xE06D69;                                   // "Colosseum"
constexpr uint32_t STR_Raid = 0xE06D73;                                        // "Raid"
constexpr uint32_t STR_Event_Select = 0xE06D78;                                // "Event_Select"
constexpr uint32_t STR_Event_Select_LeftUI = 0xE06D85;                         // "Event_Select_LeftUI"
constexpr uint32_t STR_Title = 0xE06D99;                                       // "Title"
constexpr uint32_t STR_Area_AP = 0xE06D9F;                                     // "Area_AP"
constexpr uint32_t STR_cocostudio_publish_Cursor_Anim_AP_Export = 0xE06DA7;    // "cocostudio/publish/Cursor_Anim_AP.ExportJson"
constexpr uint32_t STR_Cursor_Anim_AP = 0xE06DD4;                              // "Cursor_Anim_AP"
constexpr uint32_t STR_cocostudio_publish_Cursor_Anim_QuestStar = 0xE06DE3;    // "cocostudio/publish/Cursor_Anim_QuestStart.ExportJson"
constexpr uint32_t STR_Cursor_Anim_QuestStart = 0xE06E18;                      // "Cursor_Anim_QuestStart"
constexpr uint32_t STR_ButtonEffect = 0xE06E2F;                                // "ButtonEffect"
constexpr uint32_t STR_Title2_Lock = 0xE06E3C;                                 // "Title2_Lock"
constexpr uint32_t STR_Panel2 = 0xE06E48;                                      // "Panel2"
constexpr uint32_t STR_Title2_Lock_Icon = 0xE06E4F;                            // "Title2_Lock_Icon"
constexpr uint32_t STR_Txt_Title2 = 0xE06E60;                                  // "Txt_Title2"
constexpr uint32_t STR_Title3_Lock = 0xE06E6B;                                 // "Title3_Lock"
constexpr uint32_t STR_Title3 = 0xE06E77;                                      // "Title3"
constexpr uint32_t STR_Panel3 = 0xE06E7E;                                      // "Panel3"
constexpr uint32_t STR_Txt_Title3 = 0xE06E85;                                  // "Txt_Title3"
constexpr uint32_t STR_Title3_Lock_Icon = 0xE06E90;                            // "Title3_Lock_Icon"
constexpr uint32_t STR_Txt_Panel3 = 0xE06EA1;                                  // "Txt_Panel3"
constexpr uint32_t STR_Title4_Lock = 0xE06EAC;                                 // "Title4_Lock"
constexpr uint32_t STR_Title4 = 0xE06EB8;                                      // "Title4"
constexpr uint32_t STR_Title4_Lock_Icon = 0xE06EBF;                            // "Title4_Lock_Icon"
constexpr uint32_t STR_Button_QuestStart = 0xE06ED0;                           // "Button_QuestStart"
constexpr uint32_t STR_Txt_QuestStart = 0xE06EE2;                              // "Txt_QuestStart"
constexpr uint32_t STR_ImageButton_QuestStart = 0xE06EF1;                      // "ImageButton_QuestStart"
constexpr uint32_t STR_But10 = 0xE06F08;                                       // "But10"
constexpr uint32_t STR_Button_RaidStart = 0xE06F0E;                            // "Button_RaidStart"
constexpr uint32_t STR_Txt_RaidStart = 0xE06F1F;                               // "Txt_RaidStart"
constexpr uint32_t STR_ImageButton_RaidStart = 0xE06F2D;                       // "ImageButton_RaidStart"
constexpr uint32_t STR_cocostudio_publish_AdventureTop2_Adv_But = 0xE06F43;    // "cocostudio/publish/AdventureTop2_Adv_Button.json"
constexpr uint32_t STR_Conpane = 0xE06F74;                                     // "Conpane"
constexpr uint32_t STR_Txt_NoEvent = 0xE06F7C;                                 // "Txt_NoEvent"
constexpr uint32_t STR_Button_Lock = 0xE06F88;                                 // "Button_Lock"
constexpr uint32_t STR_Txt_Lock = 0xE06F94;                                    // "Txt_Lock"
constexpr uint32_t STR_cocostudio_publish_AdventureTop2_EV_Butt = 0xE06F9D;    // "cocostudio/publish/AdventureTop2_EV_Button.json"
constexpr uint32_t STR_Button_EV_Board = 0xE06FCD;                             // "Button_EV_Board"
constexpr uint32_t STR_Button_EV_Board_0 = 0xE06FDD;                           // "Button_EV_Board_0"
constexpr uint32_t STR_cocostudio_publish_AdventureTop2_Raid_Bu = 0xE06FEF;    // "cocostudio/publish/AdventureTop2_Raid_Button.json"
constexpr uint32_t STR_Button_Start = 0xE07021;                                // "Button_Start"
constexpr uint32_t STR_Txt_Start = 0xE0702E;                                   // "Txt_Start"
constexpr uint32_t STR_cocostudio_publish_AdventureTop2_Colosse = 0xE07038;    // "cocostudio/publish/AdventureTop2_Colosseum_Button.json"
constexpr uint32_t STR_Button_Ranking = 0xE0706F;                              // "Button_Ranking"
constexpr uint32_t STR_Button_Reward = 0xE0707E;                               // "Button_Reward"
constexpr uint32_t STR_Button_Colosseum_Board = 0xE0708C;                      // "Button_Colosseum_Board"
constexpr uint32_t STR_campaignView = 0xE070A3;                                // "campaignView"
constexpr uint32_t STR_lwf_adventure_Colosseum_Open_Colosseum_O = 0xE070B0;    // "lwf/adventure/Colosseum_Open/Colosseum_Open.lwf"

// Elements of a std::vector<StageEvent::Event> (begin, end at vec / vec + 4): (end - begin) / 56 as the compiled
// code computes it ((bytes >> 3) * inverse of 7).
inline uint32_t eventCount(uint32_t vec) {
    uint32_t bytes = ld32(vec + 4) - ld32(vec);
    return uint32_t(int32_t(bytes) >> 3) * 0xB6DB6DB7u;
}

}  // namespace sas
