// SceneMixedMedal: life cycle (create / constructor / init / destructors), the in / out animations and the small
// accessors.
#include "SceneMixedMedal.hpp"

#include "../scene_social/SocialCommon.hpp"

using namespace social;
using Self = SceneMixedMedal;
namespace F = smm::fn;

namespace {

// std::function vtables (__func<lambda> + 8) of the lambdas built here
constexpr Addr VT_init_onEnter = 0xF6604C;        // _onEnterCallback
constexpr Addr VT_init_onExit = 0xF66020;         // _onExitCallback
constexpr Addr VT_init_backKey = 0xF65FF4;        // back key
constexpr Addr VT_init_finishScene = 0xF65FC8;    // finish scene callback (no capture)
constexpr Addr VT_init_backButton = 0xF65F9C;     // "Button_Back"
constexpr Addr VT_init_random = 0xF65F70;         // "Button_Random" (auto select)
constexpr Addr VT_init_mix = 0xF65EC0;            // "Button_Mix" (captures this, the sell button)
constexpr Addr VT_init_sell = 0xF65E3C;           // "Button_Sell1"
constexpr Addr VT_runOut_done = 0xF65E10;         // runOutAnimationMain: the done callback, wrapped (heap, 0x20)

// strings
constexpr Addr S_BgDeck = 0xE03AF2;               // "img/ui/Equip_DeckBG_01.png"
constexpr Addr S_SceneJson = 0xE04E55;            // "cocostudio/publish/MedalMixScene_ver106.json"
constexpr Addr S_CursorAnimJson = 0xE04E82;       // "cocostudio/publish/Cursor_Anim_MdalMix_ver103.ExportJson"
constexpr Addr S_CenterUI_Left1 = 0xE04EBB;
constexpr Addr S_CenterUI_Left2 = 0xE04ECA;
constexpr Addr S_LeftUI = 0xDF85F0;
constexpr Addr S_CenterUI_Right = 0xE04ED9;
constexpr Addr S_Button_Back = 0xE03DBA;
constexpr Addr S_Button_Random = 0xE04EE8;
constexpr Addr S_But02 = 0xDFD6E8;
constexpr Addr S_Txt_Random = 0xE04EF6;
constexpr Addr S_Button_Mix = 0xDF87D4;
constexpr Addr S_But01 = 0xDFD027;
constexpr Addr S_Button_Sell1 = 0xE04F01;
constexpr Addr S_Txt_Sell1 = 0xE04F0E;
constexpr Addr S_But06 = 0xE04F18;
constexpr Addr S_Txt_Mix = 0xDF87DF;
constexpr Addr S_Announce = 0xDFF8BD;             // "Announce"

constexpr uint32_t SIZE_runOutDone = 0x20;        // __func holding a std::function<void()> at +8

// ImageButton::create("literal", false)
Addr newImageButton(Addr name, uint32_t len) {
    return withStr(name, len, [&](Addr s) { return call(F::ImageButton_create, {s, 0}); });
}
void setShadow(Addr text) { call(F::SceneUtil_setShadow, {text}); }

// node->runAction(EaseSineOut::create(make(empty std::function)))
template <class Make> void runEased(Addr node, Make make) {
    withEmptyFunction([&](Addr none) {
        Addr anim = make(none);
        Addr eased = call(F::EaseSineOut_create, {anim});
        call(F::Node_runAction, {node, eased});
    });
}

}  // namespace

// ------------------------------------------------------------------------------------------------ create / destroy
Addr Self::create() {
    Addr self = call(F::operator_new, {SIZE});
    call(0x65E399, {self});                                   // SceneMixedMedal::SceneMixedMedal
    if (vcall(self, smm::vslot::init, {self}) == 1) {
        call(F::Ref_autorelease, {self});
        return self;
    }
    vcall(self, smm::vslot::release, {self});
    return 0;
}
RW_FUNCTION(0x65E344, "SceneMixedMedal::create", SceneMixedMedal::create);

SceneMixedMedal *Self::construct(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    call(F::Layer_ctor, {me});
    call(F::aeabi_memset, {me + 0x28C, 0x18, 0});             // sortFilter's two vectors
    rw::set<uint32_t>(self, 0, VTABLE);
    self->sortFilter.vtable = VTABLE2;
    self->vtable3 = VTABLE3;
    self->field_350 = 0;
    call(F::aeabi_memset, {me + 0x2AC, 0x80, 0});             // centerUILeft1 .. baseBurst
    self->burstOverMax = 0;
    rw::set<uint32_t>(self, 0x33C, 0);                        // hasRareMaterial .. burstOverGuilt
    self->mixCost = 0;
    self->mixedExp = 0;
    self->removeMaterialOnInfo = 0;
    self->field_348 = 0;
    rw::set<uint32_t>(self, 0x344, 0);                        // skillMixRate = 0.0f
    call(F::Vec2_ctor, {me + 0x354});
    call(F::Vec2_ctor, {me + 0x35C});
    self->field_364 = 0;
    self->field_368 = 0;
    self->field_36C = 0;
    call(F::Rect_ctor, {me + 0x370});
    self->skillMixEnabled = 0;
    self->skillGroup = 0;
    self->lastMaterialCount = 0;
    self->isActive = 0;
    call(F::aeabi_memset, {me + 0x390, 0x1C, 0});             // field_390, intList, campaigns
    self->expRate = 10000;
    self->costRate = 10000;
    self->skillRate = 10000;
    self->medalListIndex = 0;
    self->canBack = 0;
    return self;
}
RW_FUNCTION(0x65E398, "SceneMixedMedal::SceneMixedMedal", SceneMixedMedal::construct);

Addr Self::createScene() {
    Addr scene = call(F::Scene_create, {});
    Addr layer = call(0x65E345, {});                          // SceneMixedMedal::create
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x695A18, "SceneMixedMedal::createScene", SceneMixedMedal::createScene);

SceneMixedMedal *Self::destroy(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    rw::set<uint32_t>(self, 0, VTABLE);
    self->sortFilter.vtable = VTABLE2;
    self->vtable3 = VTABLE3;
    if (Addr held = self->field_2FC) vcall(held, smm::vslot::release, {held});
    self->field_2FC = 0;
    // give the material medal views back to the manager
    Addr end = self->materialViews.end;
    for (Addr it = self->materialViews.begin; it != end; it += 4) {
        Addr mgr = call(F::DeckMedalViewManager_getInstance, {});
        call(F::DeckMedalViewManager_removeDeckMedalView, {mgr, ld32(it)});
    }
    call(F::vector_evCampaign_dtor, {me + 0x3A0});
    call(F::vector_int_dtor, {me + 0x394});
    call(F::Vec2_dtor, {me + 0x370});
    call(F::Vec2_dtor, {me + 0x35C});
    call(F::Vec2_dtor, {me + 0x354});
    if (self->baseBurst.ctrl) call(F::shared_release, {self->baseBurst.ctrl});
    if (self->baseMedal.ctrl) call(F::shared_release, {self->baseMedal.ctrl});
    call(F::vector_u64_dtor, {me + 0x310});
    call(F::vector_deckMedalView_dtor, {me + 0x304});
    call(F::vector_imageView_dtor, {me + 0x2E4});
    call(F::vector_widget_dtor, {me + 0x2D8});
    self->sortFilter.vtable = IMedalSortFilter::VTABLE;      // ~IMedalSortFilter (inlined)
    call(F::vector_userMedal_dtor, {me + 0x298});
    call(F::vector_userMedal_dtor, {me + 0x28C});
    call(F::Layer_dtor, {me});
    return self;
}
RW_FUNCTION(0x695A3C, "SceneMixedMedal::~SceneMixedMedal", SceneMixedMedal::destroy);

void Self::destroyDelete(SceneMixedMedal *self) {
    Addr p = call(0x695A3D, {rw::guest_addr(self)});          // ~SceneMixedMedal
    call(F::operator_delete, {p});
}
RW_FUNCTION(0x695C34, "SceneMixedMedal::~SceneMixedMedal (deleting)", SceneMixedMedal::destroyDelete);

// ------------------------------------------------------------------------------------------------ init
uint32_t Self::init(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    if (call(F::Layer_init, {me}) != 1) return 0;

    // Node::_onEnterCallback / _onExitCallback
    withLambda(VT_init_onEnter, {me}, [&](Addr f) { call(F::function_vv_assign, {me + 0x210, f}); });
    withLambda(VT_init_onExit, {me}, [&](Addr f) { call(F::function_vv_assign, {me + 0x228, f}); });
    self->canBack = 1;
    {
        Addr mgr = call(F::SceneManager_getInstance, {});
        withLambda(VT_init_backKey, {me}, [&](Addr f) { call(F::SceneManager_setOnBackKeyCallback, {mgr, f}); });
    }
    call(F::SceneManager_showHeader, {call(F::SceneManager_getInstance, {})});
    {
        Addr mgr = call(F::SceneManager_getInstance, {});
        withStr(S_BgDeck, 26, [&](Addr s) { call(F::SceneManager_changeBackground, {mgr, 0, s}); });
    }
    {
        Addr mgr = call(F::SceneManager_getInstance, {});
        Frame f;
        Addr text = textData(f, 100620003);
        Addr s = newStr(f, textDataStr(text));
        call(F::SceneManager_changeTickerMessage, {mgr, s});
        strFree(s);
        sharedRelease(text);
    }
    {
        Addr mgr = call(F::SceneManager_getInstance, {});
        withLambda(VT_init_finishScene, {}, [&](Addr f) { call(F::SceneManager_setFinishSceneCallback, {mgr, f}); });
    }

    self->root = withStr(S_SceneJson, 44, [&](Addr s) { return call(F::SceneUtil_createNodeWithSceneFile, {s}); });
    vcall(me, node::addChild, {me, self->root});
    {
        Addr armatures = call(F::ArmatureDataManager_getInstance, {});
        withStr(S_CursorAnimJson, 56, [&](Addr s) { call(F::ArmatureDataManager_addArmatureFileInfo, {armatures, s}); });
    }
    self->centerUILeft1 = search(self->root, S_CenterUI_Left1, 14);
    self->centerUILeft2 = search(self->root, S_CenterUI_Left2, 14);
    self->leftUI = search(self->root, S_LeftUI, 6);
    self->centerUIRight = search(self->root, S_CenterUI_Right, 14);

    Addr back = search(self->root, S_Button_Back, 11);
    call(F::SceneUtil_backBtnSetPosY, {back});
    withLambda(VT_init_backButton, {me}, [&](Addr f) { call(F::Widget_addTouchEventListener, {back, f}); });

    initMedalSelectPanel(self);
    openMedalSelectPanel(self);
    initBaseMedalPanel(self);
    {
        // the medal chosen on the previous screen (SceneManager +0x2A0, u64)
        Addr mgr = call(F::SceneManager_getInstance, {});
        uint64_t id = uint64_t(ld32(mgr + 0x2A0)) | (uint64_t(ld32(mgr + 0x2A4)) << 32);
        updateBaseMedal(self, id);
    }
    initMaterialPanel(self);

    // "Button_Random": pick materials automatically
    Addr randomArea = search(self->centerUILeft1, S_Button_Random, 13);
    Addr randomButton = newImageButton(S_But02, 5);
    call(F::ImageButton_setTargetImageView, {randomButton, randomArea});
    vcall(randomArea, node::addChild, {randomArea, randomButton});
    withLambda(VT_init_random, {me}, [&](Addr f) { call(F::ImageButton_addTouchEventListener, {randomButton, f}); });
    Addr randomText = search(randomArea, S_Txt_Random, 10);
    setShadow(randomText);
    setTextId(randomText, 101200003);

    // "Button_Mix"
    Addr mixArea = search(self->centerUILeft1, S_Button_Mix, 10);
    self->mixButton = newImageButton(S_But01, 5);
    call(F::ImageButton_setTargetImageView, {self->mixButton, mixArea});
    vcall(mixArea, node::addChild, {mixArea, self->mixButton});

    // "Button_Sell1"
    Addr sellArea = search(self->centerUILeft1, S_Button_Sell1, 12);
    Addr sellText = search(sellArea, S_Txt_Sell1, 9);
    setShadow(sellText);
    setTextId(sellText, 101100004);
    Addr sellButton = newImageButton(S_But06, 5);
    call(F::ImageButton_setTargetImageView, {sellButton, sellArea});
    vcall(sellArea, node::addChild, {sellArea, sellButton});

    {
        Addr mix = self->mixButton;
        withLambda(VT_init_mix, {me, sellButton}, [&](Addr f) { call(F::ImageButton_addTouchEventListener, {mix, f}); });
    }
    Addr mixText = search(mixArea, S_Txt_Mix, 7);
    setShadow(mixText);
    setTextId(mixText, 101300001);

    {
        // updateMaterial(baseMedal) with a copy of the shared_ptr
        Frame f;
        Addr copy = f.alloc(8);
        st32(copy, self->baseMedal.obj);
        Addr ctrl = self->baseMedal.ctrl;
        st32(copy + 4, ctrl);
        if (ctrl) call(F::shared_add, {ctrl});
        updateMaterial(self, rw::host_ptr<guest::SharedPtr>(copy));
        if (ctrl) call(F::shared_release, {ctrl});
    }
    withLambda(VT_init_sell, {me}, [&](Addr f) { call(F::ImageButton_addTouchEventListener, {sellButton, f}); });

    initCampaign(self);
    vcall(me, smm::vslot::onEnterScene, {me});
    self->isActive = 1;
    return 1;
}
RW_FUNCTION(0x695C68, "SceneMixedMedal::init", SceneMixedMedal::init);

// ------------------------------------------------------------------------------------------------ small members
void Self::update(SceneMixedMedal *, float) {}
RW_FUNCTION(0x699454, "SceneMixedMedal::update", SceneMixedMedal::update);

void Self::setBackScene(SceneMixedMedal *self, int32_t backScene) { self->backSceneType = backScene; }
RW_FUNCTION(0x699456, "SceneMixedMedal::setBackScene", SceneMixedMedal::setBackScene);

void Self::setMedalListIndex(SceneMixedMedal *self, int32_t index) {
    self->medalListIndex = index;
    call(F::IScrollProtocol_setIndex, {self->medalList, uint32_t(index)});
}
RW_FUNCTION(0x69ED34, "SceneMixedMedal::setMedalListIndex", SceneMixedMedal::setMedalListIndex);

// The fusion screen's sort / filter state; during the fusion tutorial (next phase 700) it is forced to sort
// key 4, descending.
void Self::getSortFilter(Addr out, SceneMixedMedal *) {
    Addr app = call(F::AppState_getInstance, {});
    call(F::AppState_getMixedMedalSortFilterState, {out, app});
    if (call(F::SceneUtil_getNexTutorialphase, {}) == 700) {
        st8(ld32(out) + 1, 4);
        st8(ld32(out) + 2, 1);
    }
}
RW_FUNCTION(0x69ED48, "SceneMixedMedal::getSortFilter", SceneMixedMedal::getSortFilter);

void Self::callPrepareAPI(Addr callback) {
    Addr api = call(F::APIManager_getInstance, {});
    call(F::APIManager_executeCampaignGet, {api, callback});
}
RW_FUNCTION(0x69F474, "SceneMixedMedal::callPrepareAPI", SceneMixedMedal::callPrepareAPI);

// ------------------------------------------------------------------------------------------------ animations
void Self::runInAnimation(SceneMixedMedal *self) {
    Addr left1 = self->centerUILeft1;
    runEased(left1, [&](Addr none) { return call(F::SceneUtil_createLeftInAnimation, {left1, 0, 0, none}); });
    Addr left2 = self->centerUILeft2;
    runEased(left2, [&](Addr none) { return call(F::SceneUtil_createLeftInAnimation, {left2, 0, 0, none}); });
    Addr leftUI = self->leftUI;
    runEased(leftUI, [&](Addr none) { return call(F::SceneUtil_createLeftInAnimation, {leftUI, 0, 0, none}); });
    Addr right = self->centerUIRight;
    runEased(right, [&](Addr none) { return call(F::SceneUtil_createRightInAnimation, {right, 0, 0, none}); });
    Addr announce = search(self->root, S_Announce, 8);
    runEased(announce, [&](Addr none) { return call(F::SceneUtil_createLeftInAnimation, {announce, 0, 0, none}); });
    call(F::SceneManager_tutorial_menu, {call(F::SceneManager_getInstance, {})});
}
RW_FUNCTION(0x699460, "SceneMixedMedal::runInAnimation", SceneMixedMedal::runInAnimation);

void Self::runOutAnimationMain(SceneMixedMedal *self, Addr done) {
    call(F::SceneManager_setCanTouchHeader, {call(F::SceneManager_getInstance, {}), 0});
    Addr left1 = self->centerUILeft1;
    runEased(left1, [&](Addr none) { return call(F::SceneUtil_createLeftOutAnimation, {left1, 0, 0, none}); });
    Addr left2 = self->centerUILeft2;
    runEased(left2, [&](Addr none) { return call(F::SceneUtil_createLeftOutAnimation, {left2, 0, 0, none}); });
    Addr leftUI = self->leftUI;
    runEased(leftUI, [&](Addr none) { return call(F::SceneUtil_createLeftOutAnimation, {leftUI, 0, 0, none}); });
    Addr right = self->centerUIRight;
    {
        // the right page's animation ends with `done` (copied, then moved into a heap lambda)
        Frame f;
        Addr copy = f.alloc(20);
        call(F::function_vv_copy, {copy, done});
        Addr wrapped = f.alloc(20);
        heapFunction(wrapped, VT_runOut_done, SIZE_runOutDone, 8, F::function_vv_move, copy);
        Addr anim = call(F::SceneUtil_createRightOutAnimation, {right, 0, 0, wrapped});
        Addr eased = call(F::EaseSineOut_create, {anim});
        call(F::Node_runAction, {right, eased});
        functionDestroy(wrapped);
        functionDestroy(copy);
    }
    Addr announce = search(self->root, S_Announce, 8);
    runEased(announce, [&](Addr none) { return call(F::SceneUtil_createLeftOutAnimation, {announce, 0, 0, none}); });
}
RW_FUNCTION(0x6996B4, "SceneMixedMedal::runOutAnimationMain", SceneMixedMedal::runOutAnimationMain);
