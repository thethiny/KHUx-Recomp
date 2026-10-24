// SceneMixedMedal: the medal fusion screen ("Medal Mix"). The player picks a base medal, then drags up to five
// material medals from the medal list (MiniMedalScrollView, right page) into the material slots; the left page shows
// the base medal, the experience it will gain, the munny cost and the "skill overwrite" (skill mix) option. Fusing
// warns about rare materials / skill changes / guilt changes before calling the fusion API and showing the result.
//
// A cocos2d::Layer with two more bases: IMedalSortFilter at +0x288 (the medal list's sort / filter state) and the
// scene in / out animation interface at +0x2A4. Guest objects are 32-bit guest addresses (Addr).
#pragma once

#include "../core/Guest.hpp"
#include "../scene/SceneUi.hpp"
#include "../scene_menu/IMedalSortFilter.hpp"
#include "../scene_menu/MedalDataManager.hpp"

struct SceneMixedMedal {
    using Addr = uint32_t;

    const uint8_t layer[0x288];               // cocos2d::Layer (Node::_onEnterCallback +0x210, _onExitCallback +0x228)
    IMedalSortFilter sortFilter;              // +0x288 base: vtable, medals (+0x28C), filtered view (+0x298)
    rw::Field<uint32_t> vtable3;              // +0x2A4 base: scene in / out animation interface
    rw::Field<uint32_t> root;                 // +0x2A8 the cocostudio scene (MedalMixScene_ver106.json)
    rw::Field<uint32_t> centerUILeft1;        // +0x2AC "CenterUI_Left1" (base medal, materials, exp, cost)
    rw::Field<uint32_t> centerUILeft2;        // +0x2B0 "CenterUI_Left2" (base medal picture)
    rw::Field<uint32_t> leftUI;               // +0x2B4 "LeftUI"
    rw::Field<uint32_t> centerUIRight;        // +0x2B8 "CenterUI_Right" (medal list page)
    rw::Field<uint32_t> medalList;            // +0x2BC MiniMedalScrollView*
    rw::Field<uint32_t> skillMixTitle;        // +0x2C0 "Txt_Overwrite" title label (ui::Text)
    rw::Field<uint32_t> successRateText;      // +0x2C4 "Txt_SuccessRate"
    rw::Field<uint32_t> baseSkill;            // +0x2C8 "Base_Skill" panel
    rw::Field<uint32_t> skillMixButton;       // +0x2CC ImageButton "skill_mix_button" (on "Button_Overwrite")
    rw::Field<uint32_t> skillMixButtonText;   // +0x2D0 the button's "Txt_Overwrite"
    rw::Field<uint32_t> materialAreaAnim;     // +0x2D4 "Area_Anim" (cursor animation of the material area)
    guest::Vector skillAnimAreas;             // +0x2D8 std::vector<ui::Widget*> "Area_Anim2".."Area_Anim5"
    guest::Vector skillBasePanels;            // +0x2E4 std::vector<ui::ImageView*> "Base2".."Base5"
    rw::Field<uint32_t> expLabel;             // +0x2F0 "AtlasLabel_EXP" (ui::TextAtlas)
    rw::Field<uint32_t> mixButton;            // +0x2F4 ImageButton "But01" on "Button_Mix"
    rw::Field<int32_t> backSceneType;         // +0x2F8 SceneMixedMedal::BackScene
    rw::Field<uint32_t> field_2FC;            // +0x2FC a Ref* released (virtual slot 1) by the destructor
    rw::Field<uint32_t> materialArea;         // +0x300 "Base_Material/Area" (material slots origin)
    guest::Vector materialViews;              // +0x304 std::vector<DeckMedalView*> one per material
    guest::Vector materialIds;                // +0x310 std::vector<unsigned long long> material user medal ids
    guest::SharedPtr baseMedal;               // +0x31C shared_ptr<MedalDataManager::UserMedalData>
    guest::SharedPtr baseBurst;               // +0x324 shared_ptr<master::Burst> of the base medal (burst 1 only)
    rw::Field<uint32_t> guiltRange[2];        // +0x32C getGuiltFactorValueRange: min, max guilt of the base's group
    rw::Field<int32_t> mixedExp;              // +0x334 experience the materials give
    rw::Field<int32_t> mixCost;               // +0x338 munny cost
    rw::Field<uint8_t> hasRareMaterial;       // +0x33C a material of rarity 3+
    rw::Field<uint8_t> skillWillChange;       // +0x33D a material's skill replaces the base's (skill mix off)
    rw::Field<uint8_t> hasNoEffectMaterial;   // +0x33E a material that adds nothing
    rw::Field<uint8_t> burstOverGuilt;        // +0x33F burst level up beyond the guilt allowance
    rw::Field<uint8_t> burstOverMax;          // +0x340 burst level up beyond the maximum
    const uint8_t pad_341[3];
    rw::Field<float> skillMixRate;            // +0x344 skill overwrite success rate (0..1)
    rw::Field<uint32_t> field_348;            // +0x348
    rw::Field<uint8_t> removeMaterialOnInfo;  // +0x34C the medal info popup was opened from a material slot
    const uint8_t pad_34D[3];
    rw::Field<uint32_t> field_350;            // +0x350
    rw::Field<float> vec_354[2];              // +0x354 cocos2d::Vec2
    rw::Field<float> vec_35C[2];              // +0x35C cocos2d::Vec2
    rw::Field<uint32_t> field_364;            // +0x364
    rw::Field<uint32_t> field_368;            // +0x368
    rw::Field<uint32_t> field_36C;            // +0x36C
    rw::Field<float> listRect[4];             // +0x370 cocos2d::Rect: the medal list in world space
    rw::Field<uint8_t> skillMixEnabled;       // +0x380 skill overwrite selected
    const uint8_t pad_381[3];
    rw::Field<int32_t> skillGroup;            // +0x384 the first material skill's group (skill master +224)
    rw::Field<int32_t> lastMaterialCount;     // +0x388 material count at the last updateMaterialsPanel
    rw::Field<uint8_t> isActive;              // +0x38C set at the end of init, cleared by finalize
    const uint8_t pad_38D[3];
    rw::Field<uint32_t> field_390;            // +0x390
    guest::Vector intList;                    // +0x394 std::vector<int> (runMixed)
    guest::Vector campaigns;                  // +0x3A0 std::vector<shared_ptr<const master::EvCampaign>>
    rw::Field<int32_t> expRate;               // +0x3AC campaign type 11: experience rate (10000 = 100%)
    rw::Field<int32_t> costRate;              // +0x3B0 campaign type 14: munny cost rate
    rw::Field<int32_t> skillRate;             // +0x3B4 campaign type 16: skill overwrite rate
    rw::Field<int32_t> medalListIndex;        // +0x3B8
    rw::Field<uint8_t> canBack;               // +0x3BC set by init (backScene checks it)
    const uint8_t pad_3BD[3];

    static constexpr uint32_t SIZE = 0x3C0;
    static constexpr uint32_t TYPEINFO_VTABLE = 0xF9D480;   // _ZTV15SceneMixedMedal
    static constexpr uint32_t VTABLE = 0xF9D488;            // primary vtable (+8)
    static constexpr uint32_t VTABLE2 = 0xF9D79C;           // IMedalSortFilter base (+0x288)
    static constexpr uint32_t VTABLE3 = 0xF9D7B0;           // animation interface base (+0x2A4)

    // ---- life cycle (SceneMixedMedal.cpp)
    static Addr create();
    static SceneMixedMedal *construct(SceneMixedMedal *self);   // C2
    static Addr createScene();
    static SceneMixedMedal *destroy(SceneMixedMedal *self);     // D2
    static void destroyDelete(SceneMixedMedal *self);           // D0
    static uint32_t init(SceneMixedMedal *self);
    static void update(SceneMixedMedal *self, float dt);
    static void setBackScene(SceneMixedMedal *self, int32_t backScene);
    static void runInAnimation(SceneMixedMedal *self);
    static void runOutAnimationMain(SceneMixedMedal *self, Addr done);   // std::function<void()> by value
    static void setMedalListIndex(SceneMixedMedal *self, int32_t index);
    static void getSortFilter(Addr out, SceneMixedMedal *self);         // sret shared_ptr<MedalSortFilterState>
    static void callPrepareAPI(Addr callback);   // std::function<void(bool)> by value (r0: `this` is not passed)
    static void backScene(SceneMixedMedal *self);
    static void finalize(SceneMixedMedal *self);

    // ---- panels (SceneMixedMedal_Panels.cpp)
    static void initMedalSelectPanel(SceneMixedMedal *self);
    static void openMedalSelectPanel(SceneMixedMedal *self);
    static void initBaseMedalPanel(SceneMixedMedal *self);
    static void updateBaseMedal(SceneMixedMedal *self, uint64_t userMedalId);
    static void initMaterialPanel(SceneMixedMedal *self);
    static void initCampaign(SceneMixedMedal *self);
    static void initSkillMixPanel(SceneMixedMedal *self);

    // ---- materials (SceneMixedMedal_Materials.cpp)
    static void updateMaterial(SceneMixedMedal *self, const guest::SharedPtr *base);   // shared_ptr by value
    static void updateExpGauge(SceneMixedMedal *self, Addr widget, const guest::SharedPtr *medal, int32_t exp);
    static void getGuiltFactorValueRange(int32_t guiltGroup, const uint32_t *range);   // static, unsigned (&)[2]
    static void updateMaterialsPanel(SceneMixedMedal *self);
    static void calcMaterialPosition(SceneMixedMedal *self, const float *out, int32_t index);
    static void setEnabledSkillMix(SceneMixedMedal *self, uint32_t enabled);
    static void updateSkillMixPanel(SceneMixedMedal *self);
    static uint32_t hasMixedEffectMedal(SceneMixedMedal *self, const guest::SharedPtr *material, const int32_t *exp,
                                        const int32_t *attack, const int32_t *defense, const int32_t *burst,
                                        const int32_t *cost, const int32_t *count, uint32_t accumulate);
    static void clearMaterial(SceneMixedMedal *self);
    static int32_t calcMixedExp(SceneMixedMedal *self, const guest::SharedPtr *base, const guest::SharedPtr *material);
    static uint32_t isBurstUpMedal(SceneMixedMedal *self, const guest::SharedPtr *base, const guest::SharedPtr *material);
    static void addMaterialMedal(SceneMixedMedal *self, uint64_t userMedalId);
    static void settingOmakaseMedals(SceneMixedMedal *self);
    static void removeMaterialMedal(SceneMixedMedal *self, int32_t index);
    static void onTapMedalInfoButton(SceneMixedMedal *self, int32_t type, uint32_t flag, uint64_t userMedalId);

    // ---- fusion and popups (SceneMixedMedal_Popups.cpp)
    static void openRareWarnPopup(SceneMixedMedal *self, const guest::SharedPtr *base, Addr callback);
    static void openSkillMixPopup(SceneMixedMedal *self, Addr callback);
    static void openChangeGuiltPopup(SceneMixedMedal *self, const guest::SharedPtr *base, Addr callback);
    static void runMixed(SceneMixedMedal *self);
    static void openResultPopup(SceneMixedMedal *self, const guest::SharedPtr *result);
};
RW_LAYOUT(SceneMixedMedal, sortFilter, 0x288);
RW_LAYOUT(SceneMixedMedal, vtable3, 0x2A4);
RW_LAYOUT(SceneMixedMedal, medalList, 0x2BC);
RW_LAYOUT(SceneMixedMedal, materialAreaAnim, 0x2D4);
RW_LAYOUT(SceneMixedMedal, expLabel, 0x2F0);
RW_LAYOUT(SceneMixedMedal, materialViews, 0x304);
RW_LAYOUT(SceneMixedMedal, baseMedal, 0x31C);
RW_LAYOUT(SceneMixedMedal, guiltRange, 0x32C);
RW_LAYOUT(SceneMixedMedal, skillMixRate, 0x344);
RW_LAYOUT(SceneMixedMedal, vec_354, 0x354);
RW_LAYOUT(SceneMixedMedal, listRect, 0x370);
RW_LAYOUT(SceneMixedMedal, skillMixEnabled, 0x380);
RW_LAYOUT(SceneMixedMedal, isActive, 0x38C);
RW_LAYOUT(SceneMixedMedal, campaigns, 0x3A0);
RW_LAYOUT(SceneMixedMedal, medalListIndex, 0x3B8);
RW_SIZE(SceneMixedMedal, SceneMixedMedal::SIZE);

// ------------------------------------------------------------------------------------------------ shared helpers
// Guest addresses and small helpers used by the SceneMixedMedal*.cpp files (the class's lambdas may use them too).
namespace smm {
using Addr = uint32_t;

// game / library functions (Thumb addresses as the lifted calls use them)
namespace fn {
constexpr Addr aeabi_memset = 0x40E7A0;                 // __aeabi_memset(dst, n, c)
constexpr Addr strlen_ = 0x40E08C;                      // strlen
constexpr Addr floorf_ = 0x415244;                      // floorf
constexpr Addr pow_ = 0x410654;                         // pow(double, double)
constexpr Addr floor_ = 0x41F0B4;                       // floor(double)
constexpr Addr Layer_ctor = rwlib::cocos2d::Layer::ctor_base;
constexpr Addr Layer_dtor = rwlib::cocos2d::Layer::dtor_base;
constexpr Addr Layer_init = rwlib::cocos2d::Layer::init;
constexpr Addr Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr Addr Scene_create = rwlib::cocos2d::Scene::create;
constexpr Addr Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v;                    // Vec2::Vec2()
constexpr Addr Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff;                  // Vec2::Vec2(float, float)
constexpr Addr Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr Addr Rect_ctor = rwlib::cocos2d::Rect::ctor_base__v;                    // Rect::Rect()
constexpr Addr Rect_ctorXYWH = rwlib::cocos2d::Rect::ctor_base__ffff;                // Rect::Rect(x, y, w, h)
constexpr Addr Rect_assign = rwlib::cocos2d::Rect::op_assign;                  // Rect::operator=
constexpr Addr Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                    // Size::Size(float, float)
constexpr Addr Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr Addr Node_convertToWorldSpace = rwlib::cocos2d::Node::convertToWorldSpace;     // (Vec2* out, node, const Vec2&)
constexpr Addr Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;      // (Vec2* out, node, const Vec2&)
constexpr Addr Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr Addr Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;      // (Size* out, director)
constexpr Addr Director_getVisibleOrigin = rwlib::cocos2d::Director::getVisibleOrigin;    // (Vec2* out, director)
constexpr Addr EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr Addr FadeTo_create = rwlib::cocos2d::FadeTo::create;
constexpr Addr Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr Addr ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;        // (image, const string&, TextureResType)
constexpr Addr Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr Addr EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr Addr EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr Addr EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr Addr GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr Addr GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr Addr ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr Addr ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr Addr Armature_create = rwlib::cocostudio::Armature::create;
constexpr Addr function_vv_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr Addr function_vv_copy = 0x4683CD;             // std::function<void()>::function(const function&)
constexpr Addr function_vv_move = 0x47F905;             // std::function<void()>::function(function&&)
constexpr Addr function_vb_copy = 0x45C84D;             // std::function<void(bool)>::function(const function&)
constexpr Addr function_vb_move = 0x47BC69;             // std::function<void(bool)>::function(function&&)
constexpr Addr function_touchBool_swap = 0x483E4D;      // std::function<bool(Touch*, Event*)>::swap
constexpr Addr function_touchVoid_swap = 0x4F3699;      // std::function<void(Touch*, Event*)>::swap
constexpr Addr function_vyi_swap = 0x678E79;            // std::function<void(unsigned long long, int)>::swap
constexpr Addr function_iyVecPf_swap = 0x6791B1;        // std::function<int(unsigned long long, Vec2, float*)>::swap
constexpr Addr function_viy_swap = 0x67947D;            // std::function<void(int, unsigned long long)>::swap
constexpr Addr string_init = 0x47CD93;                  // std::string::__init(const char*, size_t)
constexpr Addr vector_out_of_range = 0x47CCE1;          // __vector_base_common::__throw_out_of_range (vector::at)
constexpr Addr shared_add = 0xC8DBB5;
constexpr Addr shared_release = 0xC8DBD1;
constexpr Addr operator_new = 0xC98EF1;
constexpr Addr operator_delete = 0xC98F9B;
constexpr Addr operator_new_array = 0xC98F6F;
constexpr Addr operator_delete_array = 0xC98FB3;
// containers
constexpr Addr vector_userMedal_dtor = 0x46B119;        // __vector_base<shared_ptr<UserMedalData>>::~
constexpr Addr vector_userMedal_push_slow = 0x487A39;   // vector<shared_ptr<UserMedalData>>::__push_back_slow_path(const&)
constexpr Addr vector_u64_dtor = 0x468499;              // __vector_base<unsigned long long>::~
constexpr Addr vector_u64_push_slow_move = 0x4B33F9;    // vector<unsigned long long>::__push_back_slow_path(&&)
constexpr Addr vector_u64_push_slow_copy = 0x4ECB6D;    // vector<unsigned long long>::__push_back_slow_path(const&)
constexpr Addr vector_int_dtor = 0x478961;
constexpr Addr vector_widget_dtor = 0x485B79;           // __vector_base<ui::Widget*>::~
constexpr Addr vector_widget_push_slow = 0x4F8405;
constexpr Addr vector_imageView_dtor = 0x65E565;        // __vector_base<ui::ImageView*>::~
constexpr Addr vector_imageView_push_slow = 0x6A5D9D;
constexpr Addr vector_deckMedalView_dtor = 0x52C439;    // __vector_base<DeckMedalView*>::~
constexpr Addr vector_evCampaign_dtor = 0x573591;       // __vector_base<shared_ptr<const EvCampaign>>::~
constexpr Addr vector_evCampaign_push_slow = 0x599229;
constexpr Addr vector_schar_push_slow = 0x6A5CF5;       // vector<signed char>::__push_back_slow_path(&&)
constexpr Addr tree_int_destroy = 0x48D9B5;             // __tree<int>::destroy(node)
constexpr Addr hash_u64_find = 0x490275;                // unordered_map<unsigned long long, int>::find
// game
constexpr Addr SceneUtil_searchNodeByName = 0x45A9F5;
constexpr Addr SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr Addr SceneUtil_getSideMargin = 0x45AD0D;
constexpr Addr SceneUtil_createShadowSprite = 0x45AD69;
constexpr Addr SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr Addr SceneUtil_backBtnSetPosY = 0x45AFB5;
constexpr Addr SceneUtil_createLeftInAnimation = 0x45B5C5;
constexpr Addr SceneUtil_createLeftOutAnimation = 0x45B735;
constexpr Addr SceneUtil_createRightInAnimation = 0x45B8BD;
constexpr Addr SceneUtil_createRightOutAnimation = 0x45BA3D;
constexpr Addr SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr Addr SceneUtil_setShadow = 0x45BD15;
constexpr Addr SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr Addr SceneUtil_updateMedalRarePanel = 0x4619F5;
constexpr Addr SceneUtil_getMoneyStr = 0x461DC5;
constexpr Addr SceneUtil_setStatusChangeColor = 0x4659F1;
constexpr Addr SceneUtil_updateMixCircle = 0x465C85;
constexpr Addr SceneUtil_setAdditionalKerningForAtlasLabel = 0x466BA9;
constexpr Addr SceneUtil_setNormalColor = 0x468669;
constexpr Addr SceneUtil_setWarningColor = 0x4686A9;
constexpr Addr SceneUtil_openTutorialPopup = 0x4684C9;
constexpr Addr SceneUtil_getNexTutorialphase = 0x472B31;
constexpr Addr SceneUtil_setButtonEnabled = 0x474615;
constexpr Addr SafeBuffer_read = 0x45A269;
constexpr Addr StringBuffer_ctor = 0x49140D;
constexpr Addr TextData_get = 0x4928AD;
constexpr Addr BattleUtil_calcMedalExp = 0x496BD1;
constexpr Addr APIManager_getInstance = 0x4BA8C9;
constexpr Addr APIManager_executeCampaignGet = 0x4D346D;
constexpr Addr AppState_getInstance = 0x4E66DD;
constexpr Addr AppState_getMixedMedalSortFilterState = 0x4EB0AD;
constexpr Addr AppState_setTutorialCloseFunc = 0x4EB8D9;
constexpr Addr AppState_getTutorialCloseFunc = 0x4EBA3D;
constexpr Addr AppState_execTutorialCloseFunc = 0x4EBA5D;
constexpr Addr IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr Addr IScrollProtocol_setIndex = 0x4F71A5;
constexpr Addr MiniMedalScrollView_setItemIDs = 0x4FD1FD;
constexpr Addr MiniMedalScrollView_setTargetRect = 0x4FD3FD;
constexpr Addr MiniMedalScrollView_setBurstUpMedalIDs = 0x4FD40B;
constexpr Addr MiniMedalScrollView_setDisableMedalID = 0x4FD425;
constexpr Addr MiniMedalScrollView_changeShowType = 0x4FD531;
constexpr Addr MiniMedalScrollView_setIsShowMixInfo = 0x4FD545;
constexpr Addr MiniMedalScrollView_clearNoEffectMedalIDs = 0x4FD6B9;
constexpr Addr MiniMedalScrollView_addNoEffectMedalID = 0x4FD6E1;
constexpr Addr MiniMedalScrollView_create = 0x664D51;
constexpr Addr MiniMedalScrollView_clearDisableMedals = 0x6654E9;
constexpr Addr DeckMedalViewManager_getInstance = 0x527131;
constexpr Addr DeckMedalViewManager_removeDeckMedalView = 0x5275CD;
constexpr Addr ImageButton_create = 0x531239;           // (std::string by value, bool)
constexpr Addr ImageButton_addTouchEventListener = 0x5315B1;   // (button, std::function by value)
constexpr Addr ImageButton_setTargetImageView = 0x531631;
constexpr Addr ImageButton_setTargetView = 0x5318C1;    // (button, Node*, float)
constexpr Addr ImageButton_setSelectEnabled = 0x5318F1;
constexpr Addr ImageButton_setIsSelected = 0x5318FD;
constexpr Addr CampaignAnimationView_create = 0x598AF1;
constexpr Addr AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr Addr SceneManager_getInstance = 0x60C26D;
constexpr Addr SceneManager_setCanTouchHeader = 0x60E86D;
constexpr Addr SceneManager_changeTickerMessage = 0x610B39;
constexpr Addr SceneManager_changeBackground = 0x6128ED;
constexpr Addr SceneManager_showHeader = 0x6131ED;
constexpr Addr SceneManager_tutorial_menu = 0x613CA1;
constexpr Addr SceneManager_setFinishSceneCallback = 0x613E3F;
constexpr Addr SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr Addr SceneManager_getPopupLayer = 0x613E69;
constexpr Addr SceneManager_openPopup = 0x61524D;
constexpr Addr IMedalSortFilter_runSortFilter = 0x66204D;
constexpr Addr IMedalSortFilter_registSortButton = 0x664DA5;
constexpr Addr IMedalSortFilter_getShowType = 0x66527D;
constexpr Addr MedalDataManager_getInstance = 0x76AD69;
constexpr Addr MedalDataManager_getMedalData = 0x76ADB5;
constexpr Addr MedalDataManager_getMedalLargeImagePath = 0x76B1E5;
constexpr Addr SkillDataManager_getInstance = 0x76BA0D;
constexpr Addr SkillDataManager_getData = 0x76BA59;
constexpr Addr SkillDataManager_getImagePath = 0x76BE8D;
constexpr Addr master_Burst_create = 0x77A491;
constexpr Addr master_EvCampaign_create = 0x781DD9;
constexpr Addr master_MedalMisc_create = 0x7868C9;
constexpr Addr master_GuiltProb_getIds = 0x791FA9;
constexpr Addr master_GuiltProb_create = 0x791FC5;
}  // namespace fn

// virtual slots (vtable byte offsets)
namespace vslot {
constexpr uint32_t release = 0x04;           // deleting destructor
constexpr uint32_t init = 0x248;             // SceneMixedMedal::init
constexpr uint32_t onEnterScene = 0x300;     // SceneMixedMedal vtable +0x300 (called at the end of init)
constexpr uint32_t getSortFilter = 0x308;    // SceneMixedMedal::getSortFilter (sret shared_ptr)
}  // namespace vslot

inline uint32_t ld32(Addr a) { return rw::load<uint32_t>(a); }
inline uint8_t ld8(Addr a) { return rw::load<uint8_t>(a); }
inline float ldf(Addr a) { return rw::load<float>(a); }
// virtual call through a byte offset into the vtable
inline uint32_t vcallAt(Addr obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(obj) + offset), args);
}
}  // namespace smm
