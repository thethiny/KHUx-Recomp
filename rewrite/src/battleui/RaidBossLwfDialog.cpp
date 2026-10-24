#include "RaidBossLwfDialog.hpp"

#include "FuncThunks.hpp"

using namespace scene;

namespace {
constexpr uint32_t aeabi_memset = 0x40E7A0;              // __aeabi_memset(dst, n, c)
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_init = rwlib::cocos2d::Node::init;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t VoidFunction_assign = 0x4EB9CD;       // std::function<void()>::operator=(const function&)
constexpr uint32_t VoidFunction_call = 0x470E69;         // std::function<void()>::operator()
constexpr uint32_t String_assign = 0x48D811;             // std::string::assign(const char*, size_t)
constexpr uint32_t Stage_create = 0x78E4E9;              // master::Stage::create(shared_ptr out, int id)
constexpr uint32_t StringBuffer_ctor = 0x49140D;         // StringBuffer(this, fmt, ...)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;       // (const char* path, void*)
constexpr uint32_t FlashMovieView_create = 0x5682A1;     // (LWFNode*, bool)
constexpr uint32_t FlashMovieView_gotoLastFrame = 0x568871;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;           // (const string& name)
constexpr uint32_t ImageButton_create = 0x531239;        // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1; // (button, node, float scale)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;  // (button, std::function by value)
constexpr uint32_t Vec2_ctor_ff = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Movie_SetVisible = rwlib::LWF::Movie::SetVisible;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;   // (manager, float seconds)

constexpr uint32_t STR_escape = 0xE00290;    // "lwf/raid/demo/escape_%04d/escape_%04d.lwf"
constexpr uint32_t STR_putDown = 0xE00262;   // "lwf/raid/demo/put_down_%04d/put_down_%04d.lwf"
constexpr uint32_t STR_start = 0xE002BA;     // "lwf/raid/demo/start_%04d/start_%04d.lwf"
constexpr uint32_t STR_layoutJson = 0xE002E2;    // "cocostudio/publish/RaidDemobtn_1.json"
constexpr uint32_t STR_RaidDemoButton = 0xE00308;
constexpr uint32_t STR_Tap_Screen_Area = 0xE00317;
constexpr uint32_t STR_TapExportJson = 0xDFFCF2; // "cocostudio/publish/Tap_Screen_Animation.ExportJson"
constexpr uint32_t STR_Tap_Screen_Animation = 0xDFFB2B;
constexpr uint32_t STR_ShopBuy_But = 0xE00327;
constexpr uint32_t STR_CenterButton = 0xDFFB40;
constexpr uint32_t STR_drama_bg = 0xE00333, STR_oc_fire = 0xE0033C, STR_dw_water = 0xE00344;
constexpr uint32_t STR_Animation1 = 0xDFD0AF;
// background LWFs per type 1..6 (others: bg_WL1)
constexpr uint32_t STR_bgDB1 = 0xE0034D, STR_bgDW1 = 0xE0036B, STR_bgWL1 = 0xE00389, STR_bgAG1 = 0xE003A7;
constexpr uint32_t STR_bgOC1 = 0xE003C5, STR_bgBC1 = 0xE003E3;

// lambda __func vtables (+8), type names, type_infos
constexpr uint32_t VT_Open0 = 0xF5B0EC, TS_Open0 = 0xE3FE90, TI_Open0 = 0xF8C6B0;    // open $_0 (touch)
constexpr uint32_t VT_Open1 = 0xF5B094, TS_Open1 = 0xE3FC60, TI_Open1 = 0xF8C688;    // open $_1 (movie end)
constexpr uint32_t VT_Bg2 = 0xF5B068, TS_Bg2 = 0xE3FB90, TI_Bg2 = 0xF8C674;          // settingRaidLwf $_2
constexpr uint32_t VT_Fire3 = 0xF5B03C, TS_Fire3 = 0xE3FAD0, TI_Fire3 = 0xF8C660;    // settingRaidLwf $_3
constexpr uint32_t VT_Water4 = 0xF5B010, TS_Water4 = 0xE3FA10, TI_Water4 = 0xF8C64C; // settingRaidLwf $_4
constexpr uint32_t VT_Open1In = 0xF5B0C0, TS_Open1In = 0xE3FD60, TI_Open1In = 0xF8C69C;  // $_1::operator()::{lambda()}
constexpr uint32_t VT_Open0In = 0xF5B118, TS_Open0In = 0xE3FFC0, TI_Open0In = 0xF8C6C4;  // $_0::operator()::{lambda()}

constexpr uint32_t LWFNODE_LWF = 992;
constexpr uint32_t LWFNODE_ATTACH_LWF = 0x2F0;           // LWFNode virtual: attach a child LWF at a movie
constexpr uint32_t MOVIEVIEW_ON_END = 632;               // FlashMovieView: std::function<void()> at the end
constexpr uint32_t MOVIEVIEW_ENDED_FLAG = 628;
constexpr uint32_t ARMATURE_GET_ANIMATION = 0x298;
constexpr uint32_t ANIMATION_PLAY = 0x68;                // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t DIRECTOR_ANIMATION_INTERVAL = 0xA8;   // double
constexpr uint32_t SCENEMANAGER_NO_TAP_TIME = 0x300;     // float
constexpr uint32_t NODE_REMOVE_CHILD = 300;

// a std::function<void()> holding an inline lambda, built on the stack (captures) - destroyed by the caller
uint32_t lambda(Frame &f, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    return newFunction(f, vtable, captures);
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
RaidBossLwfDialog *RaidBossLwfDialog::create() {
    uint32_t p = call(guest::OPERATOR_NEW, {SIZE});
    call(aeabi_memset, {p, SIZE, 0});
    call(Node_ctor, {p});
    RaidBossLwfDialog *d = rw::host_ptr<RaidBossLwfDialog>(p);
    d->vtable = VTABLE;
    d->function_288_f = 0;
    d->onClose_f = 0;
    call(aeabi_memset, {p + 624, 20, 0});
    if (call(Node_init, {p}) == 1) {
        call(Ref_autorelease, {p});
        return d;
    }
    vcall(p, 4, {p});           // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x59734C, "RaidBossLwfDialog::create", RaidBossLwfDialog::create);

uint32_t RaidBossLwfDialog::init(RaidBossLwfDialog *self) { return call(Node_init, {rw::guest_addr(self)}); }
RW_FUNCTION(0x5973E4, "RaidBossLwfDialog::init", RaidBossLwfDialog::init);

RaidBossLwfDialog *RaidBossLwfDialog::destroy(RaidBossLwfDialog *self) {
    uint32_t a = rw::guest_addr(self);
    self->vtable = VTABLE;
    functionDestroy(a + 672);
    functionDestroy(a + 648);
    call(Node_dtor, {a});
    return self;
}
RW_FUNCTION(0x597D54, "RaidBossLwfDialog::~RaidBossLwfDialog", RaidBossLwfDialog::destroy);

void RaidBossLwfDialog::deletingDestroy(RaidBossLwfDialog *self) {
    destroy(self);
    call(guest::OPERATOR_DELETE, {rw::guest_addr(self)});
}
RW_FUNCTION(0x597DC0, "RaidBossLwfDialog::~RaidBossLwfDialog (deleting)", RaidBossLwfDialog::deletingDestroy);

// ------------------------------------------------------------------------------------------- open
// type 1: escape, 2: put down, otherwise the start demo; the LWF path takes demoId twice. The demo's
// background comes from the stage (master::Stage data +276).
void RaidBossLwfDialog::open(RaidBossLwfDialog *self, uint32_t parent, int32_t type, int32_t stageId,
                             int32_t demoId, uint32_t onClose) {
    uint32_t a = rw::guest_addr(self);
    Frame f;
    uint32_t path = f.alloc(12);
    st32(path, 0); st32(path + 4, 0); st32(path + 8, 0);
    if (type == 1) call(String_assign, {path, STR_escape, 0x29});
    else if (type == 2) call(String_assign, {path, STR_putDown, 0x2D});
    else call(String_assign, {path, STR_start, 0x27});
    self->parent = parent;
    call(VoidFunction_assign, {a + 672, onClose});
    uint32_t shadow = SceneUtil::createShadowSprite();
    self->shadow = shadow;
    vcall(shadow, node::setOpacity, {shadow, 0});
    vcall(self->shadow, node::addChild, {self->shadow, a});
    vcall(self->parent, node::addChild, {self->parent, self->shadow});
    uint32_t stage = f.alloc(8);
    call(Stage_create, {stage, uint32_t(stageId)});
    uint32_t buf = f.alloc(1028);
    call(StringBuffer_ctor, {buf, strData(path), uint32_t(demoId), uint32_t(demoId)});
    uint32_t lwfNode = call(AudioLWFNode_create, {buf, 0});
    int32_t bgType = safe32(ld32(stage), 276);
    settingRaidLwf(f.alloc(4), lwfNode, bgType);
    uint32_t view = call(FlashMovieView_create, {lwfNode, 0});
    self->movieView = view;
    vcall(parent, node::addChild, {parent, view});
    uint32_t layout = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), STR_layoutJson});
    self->layout = layout;
    vcall(parent, node::addChild, {parent, layout});
    {
        uint32_t anchor = f.alloc(8);
        call(Vec2_ctor_ff, {anchor, rw::fbits(0.5f), rw::fbits(0.5f)});
        vcall(layout, node::setAnchorPoint, {layout, anchor});
        call(Vec2_dtor, {anchor});
    }
    vcall(layout, node::setPosition, {layout, vcall(self->shadow, node::getPosition, {self->shadow})});
    uint32_t button = findNode(layout, STR_RaidDemoButton, 14);
    vcall(button, node::setVisible, {button, 0});
    uint32_t tapArea = findNode(layout, STR_Tap_Screen_Area, 15);
    uint32_t adm = call(ArmatureDataManager_getInstance, {});
    {
        uint32_t s = newStr(f, STR_TapExportJson, 0x32);
        call(ArmatureDataManager_addArmatureFileInfo, {adm, s});
        strFree(s);
    }
    uint32_t tapAnim;
    {
        uint32_t s = newStr(f, STR_Tap_Screen_Animation, 0x14);
        tapAnim = call(Armature_create, {s});
        strFree(s);
    }
    uint32_t z = vcall(tapArea, 0x1C, {tapArea});               // getLocalZOrder
    vcall(tapArea, node::addChildZ, {tapArea, tapAnim, z + 10});
    vcall(tapAnim, node::setVisible, {tapAnim, 0});
    uint32_t center;
    {
        uint32_t s = newStr(f, STR_ShopBuy_But, 0xB);
        center = call(ImageButton_create, {s, 0});
        strFree(s);
    }
    call(ImageButton_setTargetView, {center, layout, rw::fbits(1.0f)});
    {
        uint32_t s = newStr(f, STR_CenterButton, 0xC);
        vcall(layout, node::addChildZName, {layout, center, 0, s});
        strFree(s);
    }
    {
        uint32_t fn = lambda(f, VT_Open0, {tapAnim, a});     // $_0: close on a tap once the movie ended
        call(ImageButton_addTouchEventListener, {center, fn});
        functionDestroy(fn);
    }
    {
        uint32_t fn = lambda(f, VT_Open1, {view, tapAnim, lwfNode});   // $_1: the movie reached its end
        call(VoidFunction_assign, {view + MOVIEVIEW_ON_END, fn});
        functionDestroy(fn);
    }
    rw::save<uint8_t>(view + MOVIEVIEW_ENDED_FLAG, 1);
    sharedRelease(stage);
    strFree(path);
}
RW_FUNCTION(0x5973EC, "RaidBossLwfDialog::open", RaidBossLwfDialog::open);

// Hook the demo LWF's "drama_bg" (attach the stage background by type), "oc_fire" and "dw_water" (hidden) movies.
// `this` is not used (open passes a stack address).
void RaidBossLwfDialog::settingRaidLwf(uint32_t, uint32_t lwfNode, int32_t bgType) {
    struct { uint32_t text, len, vtable; } handlers[] = {
        {STR_drama_bg, 8, VT_Bg2}, {STR_oc_fire, 7, VT_Fire3}, {STR_dw_water, 8, VT_Water4}};
    for (auto &h : handlers) {
        uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
        Frame f;
        uint32_t name = newStr(f, h.text, h.len);
        if (h.vtable == VT_Bg2) bui::lwf::addLoadHandler(lwf, name, h.vtable, {uint32_t(bgType), lwfNode});
        else bui::lwf::addLoadHandler(lwf, name, h.vtable, {});
        strFree(name);
    }
}
RW_FUNCTION(0x597940, "RaidBossLwfDialog::settingRaidLwf", RaidBossLwfDialog::settingRaidLwf);

// ------------------------------------------------------------------------------------------- lambdas
namespace RaidBossLwfDialogLambdas {
using namespace bui::thunk;

// settingRaidLwf $_4 / $_3 ("dw_water" / "oc_fire" loaded): hide the movie
void hideMovieCall(uint32_t, uint32_t movieRef) { call(Movie_SetVisible, {ld32(movieRef), 0}); }

// settingRaidLwf $_2 ("drama_bg" loaded): attach the background LWF for the type (captures type, LWFNode)
void dramaBgCall(uint32_t self, uint32_t movieRef) {
    uint32_t movie = ld32(movieRef);
    Frame f;
    uint32_t path = f.alloc(12);
    st32(path, 0); st32(path + 4, 0); st32(path + 8, 0);
    uint32_t text;
    switch (ld32(self + 4)) {
    case 1: text = STR_bgDB1; break;
    case 2: text = STR_bgDW1; break;
    case 3: text = STR_bgWL1; break;
    case 4: text = STR_bgAG1; break;
    case 5: text = STR_bgOC1; break;
    case 6: text = STR_bgBC1; break;
    default: text = STR_bgWL1; break;
    }
    call(String_assign, {path, text, 0x1D});
    uint32_t node = ld32(self + 8);
    uint32_t fn = newEmptyFunction(f);
    uint32_t out = f.alloc(8);
    // node->attachLWF(path, movie, "drama_bg", std::function()) -> shared_ptr
    call(ld32(ld32(node) + LWFNODE_ATTACH_LWF), {out, node, strData(path), movie, STR_drama_bg, fn});
    sharedRelease(out);
    functionDestroy(fn);
    strFree(path);
}

// open $_1 (the demo movie ended): one frame later, jump to its last frame and show the tap animation
void movieEndCall(uint32_t self) {
    uint32_t view = ld32(self + 4), tapAnim = ld32(self + 8), lwfNode = ld32(self + 12);
    double interval = rw::load<double>(call(Director_getInstance, {}) + DIRECTOR_ANIMATION_INTERVAL);
    uint32_t delay = call(DelayTime_create, {rw::fbits(float(interval))});
    Frame f;
    uint32_t fn = lambda(f, VT_Open1In, {view, tapAnim});
    uint32_t cf = call(CallFunc_create, {fn});
    call(Node_runAction, {lwfNode, call(Sequence_create, {delay, cf, 0})});
    functionDestroy(fn);
}

// open $_1::{lambda()}: last frame, tap animation "Animation1" looping, visible
void showTapCall(uint32_t self) {
    call(FlashMovieView_gotoLastFrame, {ld32(self + 4)});
    uint32_t tapAnim = ld32(self + 8);
    uint32_t anim = vcall(tapAnim, ARMATURE_GET_ANIMATION, {tapAnim});
    uint32_t play = ld32(ld32(anim) + ANIMATION_PLAY);
    Frame f;
    uint32_t s = f.alloc(12);
    st8(s, 20);                                      // short string of 10 chars
    for (uint32_t k = 0; k < 10; k++) st8(s + 1 + k, rw::load<uint8_t>(STR_Animation1 + k));
    st8(s + 11, 0);
    call(play, {anim, s, 0xFFFFFFFFu, 0xFFFFFFFFu});
    strFree(s);
    uint32_t arm = ld32(self + 8);
    vcall(arm, node::setVisible, {arm, 1});
}

// open $_0 (touch on the centre button; captures tap animation, dialog): on touch end while the tap animation is
// visible, hide it, block taps for the scene manager's no-tap time and close after that delay
void touchCall(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    uint32_t tapAnim = ld32(self + 4);
    uint32_t dialog = ld32(self + 8);
    if (vcall(tapAnim, node::isVisible, {tapAnim}) != 1) return;
    call(Ref_retain, {dialog});
    call(Ref_autorelease, {dialog});
    tapAnim = ld32(self + 4);
    vcall(tapAnim, node::setVisible, {tapAnim, 0});
    call(AudioPlaySystemSEDecide, {});
    uint32_t sm = call(SceneManager_getInstance, {});
    call(SceneManager_openNoTapScreen, {sm, ld32(call(SceneManager_getInstance, {}) + SCENEMANAGER_NO_TAP_TIME)});
    uint32_t delay = call(DelayTime_create, {ld32(call(SceneManager_getInstance, {}) + SCENEMANAGER_NO_TAP_TIME)});
    Frame f;
    uint32_t fn = lambda(f, VT_Open0In, {dialog});
    uint32_t cf = call(CallFunc_create, {fn});
    call(Node_runAction, {dialog, call(Sequence_create, {delay, cf, 0})});
    functionDestroy(fn);
}

// open $_0::{lambda()}: remove the shadow, movie and layout from the parent and run the close callback
void closeCall(uint32_t self) {
    uint32_t d = ld32(self + 4);
    RaidBossLwfDialog *dlg = rw::host_ptr<RaidBossLwfDialog>(d);
    call(Ref_retain, {dlg->movieView});
    call(Ref_autorelease, {dlg->movieView});
    call(Ref_retain, {dlg->layout});
    call(Ref_autorelease, {dlg->layout});
    vcall(dlg->parent, NODE_REMOVE_CHILD, {dlg->parent, dlg->shadow, 1});
    vcall(dlg->parent, NODE_REMOVE_CHILD, {dlg->parent, dlg->movieView, 1});
    vcall(dlg->parent, NODE_REMOVE_CHILD, {dlg->parent, dlg->layout, 1});
    dlg->parent = 0;
    if (dlg->onClose_f) call(VoidFunction_call, {d + 672});
}

void funcDelete(uint32_t self) { deleteSelf(self); }
void funcNop(uint32_t) {}
#define RBLD_THUNKS(N, VT, TS, TI, SIZE, WORDS)                                                              \
    uint32_t N##Clone(uint32_t self) { return cloneNew(self, VT, SIZE, WORDS); }                               \
    void N##CloneInto(uint32_t self, uint32_t dst) { cloneInto(self, dst, VT, WORDS); }                       \
    uint32_t N##Target(uint32_t self, uint32_t ti) { return target(self, ti, TS); }                           \
    uint32_t N##TargetType(uint32_t) { return TI; }
RBLD_THUNKS(water4, VT_Water4, TS_Water4, TI_Water4, 8, 0)
RBLD_THUNKS(fire3, VT_Fire3, TS_Fire3, TI_Fire3, 8, 0)
RBLD_THUNKS(bg2, VT_Bg2, TS_Bg2, TI_Bg2, 12, 2)
RBLD_THUNKS(open1, VT_Open1, TS_Open1, TI_Open1, 16, 3)
RBLD_THUNKS(open1In, VT_Open1In, TS_Open1In, TI_Open1In, 12, 2)
RBLD_THUNKS(open0, VT_Open0, TS_Open0, TI_Open0, 12, 2)
RBLD_THUNKS(open0In, VT_Open0In, TS_Open0In, TI_Open0In, 8, 1)
#undef RBLD_THUNKS
}  // namespace RaidBossLwfDialogLambdas

using namespace RaidBossLwfDialogLambdas;
// settingRaidLwf $_4 (dw_water)
RW_FUNCTION(0x597E30, "RaidBossLwfDialog::settingRaidLwf $_4 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x597E38, "RaidBossLwfDialog::settingRaidLwf $_4 __clone()", water4Clone);
RW_FUNCTION(0x597E5C, "RaidBossLwfDialog::settingRaidLwf $_4 __clone(dst)", water4CloneInto);
RW_FUNCTION(0x597E78, "RaidBossLwfDialog::settingRaidLwf $_4 destroy", funcNop);
RW_FUNCTION(0x597E7A, "RaidBossLwfDialog::settingRaidLwf $_4 destroy_deallocate", funcDelete);
RW_FUNCTION(0x597E82, "RaidBossLwfDialog::settingRaidLwf $_4 operator()", hideMovieCall);
RW_FUNCTION(0x597E90, "RaidBossLwfDialog::settingRaidLwf $_4 target", water4Target);
RW_FUNCTION(0x597EB0, "RaidBossLwfDialog::settingRaidLwf $_4 target_type", water4TargetType);
// settingRaidLwf $_3 (oc_fire)
RW_FUNCTION(0x597EC4, "RaidBossLwfDialog::settingRaidLwf $_3 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x597ECC, "RaidBossLwfDialog::settingRaidLwf $_3 __clone()", fire3Clone);
RW_FUNCTION(0x597EF0, "RaidBossLwfDialog::settingRaidLwf $_3 __clone(dst)", fire3CloneInto);
RW_FUNCTION(0x597F0C, "RaidBossLwfDialog::settingRaidLwf $_3 destroy", funcNop);
RW_FUNCTION(0x597F0E, "RaidBossLwfDialog::settingRaidLwf $_3 destroy_deallocate", funcDelete);
RW_FUNCTION(0x597F16, "RaidBossLwfDialog::settingRaidLwf $_3 operator()", hideMovieCall);
RW_FUNCTION(0x597F24, "RaidBossLwfDialog::settingRaidLwf $_3 target", fire3Target);
RW_FUNCTION(0x597F44, "RaidBossLwfDialog::settingRaidLwf $_3 target_type", fire3TargetType);
// settingRaidLwf $_2 (drama_bg)
RW_FUNCTION(0x597F58, "RaidBossLwfDialog::settingRaidLwf $_2 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x597F60, "RaidBossLwfDialog::settingRaidLwf $_2 __clone()", bg2Clone);
RW_FUNCTION(0x597F90, "RaidBossLwfDialog::settingRaidLwf $_2 __clone(dst)", bg2CloneInto);
RW_FUNCTION(0x597FB4, "RaidBossLwfDialog::settingRaidLwf $_2 destroy", funcNop);
RW_FUNCTION(0x597FB6, "RaidBossLwfDialog::settingRaidLwf $_2 destroy_deallocate", funcDelete);
RW_FUNCTION(0x597FC0, "RaidBossLwfDialog::settingRaidLwf $_2 operator()", dramaBgCall);
RW_FUNCTION(0x59819C, "RaidBossLwfDialog::settingRaidLwf $_2 target", bg2Target);
RW_FUNCTION(0x5981BC, "RaidBossLwfDialog::settingRaidLwf $_2 target_type", bg2TargetType);
// open $_1 (movie end)
RW_FUNCTION(0x5981D0, "RaidBossLwfDialog::open $_1 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x5981D8, "RaidBossLwfDialog::open $_1 __clone()", open1Clone);
RW_FUNCTION(0x598208, "RaidBossLwfDialog::open $_1 __clone(dst)", open1CloneInto);
RW_FUNCTION(0x598230, "RaidBossLwfDialog::open $_1 destroy", funcNop);
RW_FUNCTION(0x598232, "RaidBossLwfDialog::open $_1 destroy_deallocate", funcDelete);
RW_FUNCTION(0x59823C, "RaidBossLwfDialog::open $_1 operator()", movieEndCall);
RW_FUNCTION(0x598320, "RaidBossLwfDialog::open $_1 target", open1Target);
RW_FUNCTION(0x598340, "RaidBossLwfDialog::open $_1 target_type", open1TargetType);
// open $_1::{lambda()}
RW_FUNCTION(0x598354, "RaidBossLwfDialog::open $_1 inner ~__func (deleting)", funcDelete);
RW_FUNCTION(0x59835C, "RaidBossLwfDialog::open $_1 inner __clone()", open1InClone);
RW_FUNCTION(0x59838C, "RaidBossLwfDialog::open $_1 inner __clone(dst)", open1InCloneInto);
RW_FUNCTION(0x5983B0, "RaidBossLwfDialog::open $_1 inner destroy", funcNop);
RW_FUNCTION(0x5983B2, "RaidBossLwfDialog::open $_1 inner destroy_deallocate", funcDelete);
RW_FUNCTION(0x5983BC, "RaidBossLwfDialog::open $_1 inner operator()", showTapCall);
RW_FUNCTION(0x59848C, "RaidBossLwfDialog::open $_1 inner target", open1InTarget);
RW_FUNCTION(0x5984AC, "RaidBossLwfDialog::open $_1 inner target_type", open1InTargetType);
// open $_0 (touch)
RW_FUNCTION(0x5984C0, "RaidBossLwfDialog::open $_0 ~__func (deleting)", funcDelete);
RW_FUNCTION(0x5984C8, "RaidBossLwfDialog::open $_0 __clone()", open0Clone);
RW_FUNCTION(0x5984F8, "RaidBossLwfDialog::open $_0 __clone(dst)", open0CloneInto);
RW_FUNCTION(0x59851C, "RaidBossLwfDialog::open $_0 destroy", funcNop);
RW_FUNCTION(0x59851E, "RaidBossLwfDialog::open $_0 destroy_deallocate", funcDelete);
RW_FUNCTION(0x598528, "RaidBossLwfDialog::open $_0 operator()", touchCall);
RW_FUNCTION(0x598644, "RaidBossLwfDialog::open $_0 target", open0Target);
RW_FUNCTION(0x598664, "RaidBossLwfDialog::open $_0 target_type", open0TargetType);
// open $_0::{lambda()}
RW_FUNCTION(0x598678, "RaidBossLwfDialog::open $_0 inner ~__func (deleting)", funcDelete);
RW_FUNCTION(0x598680, "RaidBossLwfDialog::open $_0 inner __clone()", open0InClone);
RW_FUNCTION(0x5986AC, "RaidBossLwfDialog::open $_0 inner __clone(dst)", open0InCloneInto);
RW_FUNCTION(0x5986CC, "RaidBossLwfDialog::open $_0 inner destroy", funcNop);
RW_FUNCTION(0x5986CE, "RaidBossLwfDialog::open $_0 inner destroy_deallocate", funcDelete);
RW_FUNCTION(0x5986D6, "RaidBossLwfDialog::open $_0 inner operator()", closeCall);
RW_FUNCTION(0x598758, "RaidBossLwfDialog::open $_0 inner target", open0InTarget);
RW_FUNCTION(0x598778, "RaidBossLwfDialog::open $_0 inner target_type", open0InTargetType);
