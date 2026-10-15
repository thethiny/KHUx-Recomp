#include "SceneTitle.hpp"

using namespace social;

namespace {
constexpr uint32_t AppState_needsUpdateData = 0x4EAE5D;
constexpr uint32_t AppState_enumerateEventResourceIDsToDownload = 0x4EAD05;   // (vector<unsigned>* out, state)
constexpr uint32_t AppState_getMasterRevision = 0x4EAB49;
constexpr uint32_t AppState_getResourceRevision = 0x4EAB9B;
constexpr uint32_t ResourceDataManager_getInstance = 0x7728AD;
constexpr uint32_t ResourceDataManager_unmount = 0x7736A5;
constexpr uint32_t APIManager_executeLogin = 0x4C52A9;        // (api, const function<void(bool)>&)

constexpr uint32_t APIMANAGER_LOGIN_STATE = 0x44;   // APIManager + 0x44: login state object (byte +6: logged out)
constexpr uint32_t SCENEMANAGER_TITLE_FLAG = 0x4CC;  // byte
constexpr uint32_t SCENEMANAGER_ON_RESUME = 0x548;   // std::function<void(bool)>

constexpr uint32_t COLOR4B_BLACK = 0x1007EBC;
constexpr uint32_t SIZE_ZERO = 0x10068C0;
constexpr uint32_t VEC2_ANCHOR_BOTTOM_RIGHT = 0xFEF5D0;
constexpr uint32_t COLOR3B_WHITE = 0x1007E88;

// lambdas (std::function __func vtables)
constexpr uint32_t VT_LoginDone = 0xF642E0;       // executeLogin callback [this]
constexpr uint32_t VT_OnEnter = 0xF642B4;         // Node onEnter callback [this]
constexpr uint32_t VT_OnExit = 0xF64288;          // Node onExit callback [this]
constexpr uint32_t VT_TitleAnmIn = 0xF64230;      // CallFunc after the delay: start the title animation [this]
constexpr uint32_t VT_TitleLogoIn = 0xF64204;     // CallFunc after the delay: start the logo animation [this]
constexpr uint32_t VT_RestoreTouch = 0xF64154;    // Button_Restore touch (no captures)
constexpr uint32_t VT_TouchBegan = 0xF64128;      // [this]
constexpr uint32_t VT_TouchEnded = 0xF64020;      // [this]
constexpr uint32_t VT_BackKey = 0xF63FF4;         // [this]

// strings
constexpr uint32_t S_TitleAnimationJson = 0xE045B8;   // "cocostudio/publish/TitleAnimation.ExportJson"
constexpr uint32_t S_TitleAnimation = 0xE045E5;
constexpr uint32_t S_TitleSceneJson = 0xE045F4;       // "cocostudio/publish/TitleScene.json"
constexpr uint32_t S_RightUI = 0xDF85F7;
constexpr uint32_t S_GameStart = 0xE04617;
constexpr uint32_t S_LeftUI = 0xDF85F0;
constexpr uint32_t S_TouchScreen_Label = 0xE04621;
constexpr uint32_t S_GAMESTART_Label = 0xE04633;
constexpr uint32_t S_TitleAnm = 0xE04643;
constexpr uint32_t S_TitleAnime_in = 0xE0464C;
constexpr uint32_t S_TitleLogo = 0xE0465A;
constexpr uint32_t S_TitleAnime_Logo = 0xE04664;
constexpr uint32_t S_Button_Restore = 0xE04674;
constexpr uint32_t S_Txt_Restore = 0xE04683;
constexpr uint32_t S_Panel_3 = 0xE0468F;
constexpr uint32_t S_info_version = 0xDFBAE0;          // "info/version"
constexpr uint32_t S_Version_fmt = 0xE04697;           // "Version %s"
constexpr uint32_t S_DFPSoGei = 0xDF9F4D;              // "cocostudio/publish/DFPSoGei-W5.TTF"

constexpr int32_t TEXT_TOUCH_SCREEN = 109100007;
constexpr int32_t TEXT_GAME_START = 109100001;
constexpr int32_t TEXT_RESTORE = 109100005;

// ArmatureAnimation virtual slots
constexpr uint32_t ANIM_playWithIndex = 0x70;        // (index, durationTo, loop)

// node->setPositionY(node->getPositionY() - (height / 2 - 320)): pins a node to the top of a tall screen
void shiftUp(Addr node, float height) {
    float y = getPosY(node);
    setPosY(node, y - (height * 0.5f + -320.0f));
}

// armature->getAnimation()->playWithIndex(index), then the frame count of `movement`
int32_t playAndCount(Addr armature, uint32_t index, Addr movement, uint32_t len) {
    Addr anim = vcall(armature, slot::getAnimation, {armature});
    vcall(anim, ANIM_playWithIndex, {anim, index, ~0u, ~0u});
    Addr data = vcall(anim, slot::getAnimationData, {anim});
    Addr mv = withStr(movement, len, [&](Addr s) { return call(sfn::AnimationData_getMovement, {data, s}); });
    return int32_t(ld32(mv + 32) - 1);
}
}  // namespace

uint32_t SceneTitle::create() {
    return createLayer(SIZE, VTABLE, [](Addr p) {
        SceneTitle *t = rw::host_ptr<SceneTitle>(p);
        t->titleArmature = 0;
        t->logoArmature = 0;
        t->field_2B0 = 1;
        t->uiShown = 0;
        t->lastFrame = 0;
        t->field_2B8 = 0;
    });
}
RW_FUNCTION(0x637EC0, "SceneTitle::create", SceneTitle::create);

uint32_t SceneTitle::createScene() {
    Addr sceneObj = call(sfn::Scene_create, {});
    return sceneWith(create(), sceneObj);
}
RW_FUNCTION(0x67D868, "SceneTitle::createScene", SceneTitle::createScene);

uint32_t SceneTitle::destroy(SceneTitle *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    return call(sfn::Layer_dtor, {rw::guest_addr(self)});
}
RW_FUNCTION(0x67D88C, "SceneTitle::~SceneTitle", SceneTitle::destroy);

void SceneTitle::destroyDelete(SceneTitle *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    call(fn::operator_delete, {call(sfn::Layer_dtor, {rw::guest_addr(self)})});
}
RW_FUNCTION(0x67D8AC, "SceneTitle::~SceneTitle (deleting)", SceneTitle::destroyDelete);

uint32_t SceneTitle::init(SceneTitle *self) {
    const Addr me = rw::guest_addr(self);
    if (call(sfn::Layer_init, {me}) != 1) return 0;

    call(AppState_needsUpdateData, {call(sfn::AppState_getInstance, {})});
    {
        Frame f;
        Addr ids = f.alloc(12);
        call(AppState_enumerateEventResourceIDsToDownload, {ids, call(sfn::AppState_getInstance, {})});
        call(sfn::vector_uint_dtor, {ids});
    }
    call(ResourceDataManager_unmount, {call(ResourceDataManager_getInstance, {})});

    // not logged in yet (or logged out): log in now
    Addr login = ld32(call(sfn::APIManager_getInstance, {}) + APIMANAGER_LOGIN_STATE);
    if (!login || ld8(login + 6) != 0) {
        Addr sm = call(sfn::SceneManager_getInstance, {});
        withEmptyFunction([&](Addr fo) {
            st8(sm + SCENEMANAGER_TITLE_FLAG, 1);
            call(sfn::function_vb_assign, {sm + SCENEMANAGER_ON_RESUME, fo});
        });
        Addr api = call(sfn::APIManager_getInstance, {});
        withLambda(VT_LoginDone, {me}, [&](Addr fo) { call(APIManager_executeLogin, {api, fo}); });
    }
    withLambda(VT_OnEnter, {me}, [&](Addr fo) { call(sfn::function_vv_assign, {me + 0x210, fo}); });
    withLambda(VT_OnExit, {me}, [&](Addr fo) { call(sfn::function_vv_assign, {me + 0x228, fo}); });

    Frame f;
    Addr vs = f.alloc(8);
    call(sfn::Director_getVisibleSize, {vs, dir()});
    const float height = ldf(vs + 4);

    // black background
    Addr black = call(sfn::LayerColor_create, {COLOR4B_BLACK});
    self->blackLayer = black;
    {
        Addr size = f.alloc(8);
        call(sfn::Size_ctor, {size, rw::load<uint32_t>(vs), rw::load<uint32_t>(vs + 4)});
        vcall(black, node::setContentSize, {black, size});
    }
    addChildZ(me, self->blackLayer, 10);
    vcall(self->blackLayer, slot::setOpacity, {self->blackLayer, 255});

    // title armatures
    Addr adm = call(sfn::ArmatureDataManager_getInstance, {});
    withStr(S_TitleAnimationJson, 44, [&](Addr s) { call(sfn::ArmatureDataManager_addArmatureFileInfo, {adm, s}); });
    self->titleArmature = withStr(S_TitleAnimation, 14, [](Addr s) { return call(sfn::Armature_create, {s}); });
    self->logoArmature = withStr(S_TitleAnimation, 14, [](Addr s) { return call(sfn::Armature_create, {s}); });

    // the scene file
    self->root = withStr(S_TitleSceneJson, 34, [](Addr s) { return call(sfn::SceneUtil_createNodeWithSceneFile, {s}); });
    addChild(me, self->root);
    self->rightUI = search(self->root, S_RightUI, 7);
    self->gameStart = search(self->root, S_GameStart, 9);
    setVisible(self->rightUI, 1);
    setVisible(self->gameStart, 0);
    shiftUp(self->gameStart, height);
    self->leftUI = search(self->root, S_LeftUI, 6);
    setVisible(self->leftUI, 0);
    shiftUp(self->leftUI, height);

    Addr touchLabel = search(self->root, S_TouchScreen_Label, 17);
    call(sfn::SceneUtil_setShadow, {touchLabel});
    setTextId(touchLabel, TEXT_TOUCH_SCREEN);
    Addr startLabel = search(self->root, S_GAMESTART_Label, 15);
    call(sfn::SceneUtil_setShadow, {startLabel});
    setTextId(startLabel, TEXT_GAME_START);

    // title animation and logo, both started by a CallFunc after a short delay
    self->titleAnm = search(self->root, S_TitleAnm, 8);
    self->lastFrame = playAndCount(self->titleArmature, 0, S_TitleAnime_in, 13);
    addChild(self->titleAnm, self->titleArmature);
    setVisible(self->titleArmature, 0);
    shiftUp(self->titleArmature, height);
    self->titleLogo = search(self->root, S_TitleLogo, 9);
    self->lastFrame = playAndCount(self->logoArmature, 1, S_TitleAnime_Logo, 15);
    addChild(self->titleAnm, self->logoArmature);
    setVisible(self->logoArmature, 0);

    Addr delay = call(sfn::DelayTime_create, {fbits(0.25f)});
    Addr titleIn = withLambda(VT_TitleAnmIn, {me}, [](Addr fo) { return call(sfn::CallFunc_create, {fo}); });
    Addr logoIn = withLambda(VT_TitleLogoIn, {me}, [](Addr fo) { return call(sfn::CallFunc_create, {fo}); });
    Addr anm = self->titleAnm;
    call(sfn::Node_runAction, {anm, call(sfn::Sequence_create, {delay, titleIn, 0})});
    Addr logo = self->titleLogo;
    call(sfn::Node_runAction, {logo, call(sfn::Sequence_create, {delay, logoIn, 0})});

    // data restore button (bottom), only when master data or resources were downloaded before
    self->restoreButton = search(self->root, S_Button_Restore, 14);
    setVisible(self->restoreButton, 0);
    {
        Addr b = self->restoreButton;
        float y = getPosY(b);
        setPosY(b, y + (height * 0.5f + -320.0f));
    }
    withStr(S_Txt_Restore, 11, [&](Addr s) { call(sfn::SceneUtil_settingTitleLabel, {self->restoreButton, s, uint32_t(TEXT_RESTORE)}); });
    Addr restore = search(self->restoreButton, S_Button_Restore, 14);
    if (call(AppState_getMasterRevision, {call(sfn::AppState_getInstance, {})}) != 0 ||
        call(AppState_getResourceRevision, {call(sfn::AppState_getInstance, {})}) != 0) {
        vcall(restore, slot::setTouchEnabled, {restore, 1});
        withLambda(VT_RestoreTouch, {}, [&](Addr fo) { call(sfn::Widget_addTouchEventListener, {restore, fo}); });
    } else {
        setVisible(restore, 0);
    }

    // tap anywhere
    Addr dispatcher = ld32(dir() + 0x84);
    Addr listener = call(sfn::EventListenerTouchOneByOne_create, {});
    call(sfn::EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    withLambda(VT_TouchBegan, {me}, [&](Addr fo) { call(sfn::function_touchBool_swap, {fo, listener + 0x50}); });
    withLambda(VT_TouchEnded, {me}, [&](Addr fo) { call(sfn::function_touchVoid_swap, {fo, listener + 0x80}); });
    call(sfn::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, me});

    Addr panel = search(self->leftUI, S_Panel_3, 7);
    vcall(panel, slot::setTouchEnabled, {panel, 0});

    // "Version x.y.z" at the bottom right
    Addr data = f.alloc(8);
    call(sfn::bg_vfs_readData, {data, S_info_version});
    if (ld32(data)) {
        Addr buf = stringBuffer(f, {S_Version_fmt, ld32(ld32(data) + 4)});
        Addr text = newStr(f, buf);
        Addr font = newStr(f, S_DFPSoGei, 34);
        // createWithTTF(text, font, 20.0f, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP)
        Addr label = call(sfn::Label_createWithTTF, {text, font, fbits(20.0f), SIZE_ZERO, 0, 0});
        strFree(font);
        strFree(text);
        vcall(label, node::setAnchorPoint, {label, VEC2_ANCHOR_BOTTOM_RIGHT});
        vcall(label, slot::setColor, {label, COLOR3B_WHITE});
        Addr vs2 = f.alloc(8);
        call(sfn::Director_getVisibleSize, {vs2, dir()});
        Addr pos = f.alloc(8);
        call(sfn::Vec2_ctor, {pos, fbits(ldf(vs2) + -10.0f), fbits(10.0f)});
        vcall(label, node::setPosition, {label, pos});
        call(sfn::Vec2_dtor, {pos});
        addChildZ(me, label, 10000);
    }
    sharedRelease(data);

    Addr sm = call(sfn::SceneManager_getInstance, {});
    withLambda(VT_BackKey, {me}, [&](Addr fo) { call(sfn::SceneManager_setOnBackKeyCallback, {sm, fo}); });
    return 1;
}
RW_FUNCTION(0x67D8D0, "SceneTitle::init", SceneTitle::init);

void SceneTitle::runInAnimation(SceneTitle *) {}
RW_FUNCTION(0x67E8BC, "SceneTitle::runInAnimation", SceneTitle::runInAnimation);

// Once the title animation has stopped, slide the UI in (once).
void SceneTitle::update(SceneTitle *self, float) {
    Addr anim = vcall(self->titleArmature, slot::getAnimation, {self->titleArmature});
    if (vcall(anim, slot::isPlaying, {anim}) != 0 || self->uiShown != 0) return;
    auto slide = [](Addr node, uint32_t create) {
        withEmptyFunction([&](Addr fo) {
            Addr a = call(create, {node, fbits(0.0f), fbits(1.0f), fo});
            call(sfn::Node_runAction, {node, call(sfn::EaseSineOut_create, {a})});
        });
    };
    slide(self->leftUI, sfn::SceneUtil_createBottomInAnimation);
    slide(self->gameStart, sfn::SceneUtil_createBottomInAnimation);
    slide(self->restoreButton, sfn::SceneUtil_createTopInAnimation);
    setVisible(self->restoreButton, 1);
    setVisible(self->leftUI, 1);
    setVisible(self->gameStart, 1);
    self->uiShown = 1;
}
RW_FUNCTION(0x67E8C0, "SceneTitle::update", SceneTitle::update);

void SceneTitle::runOutAnimation(SceneTitle *, uint32_t done) { call(sfn::function_vv_call, {done}); }
RW_FUNCTION(0x67EA68, "SceneTitle::runOutAnimation", SceneTitle::runOutAnimation);
