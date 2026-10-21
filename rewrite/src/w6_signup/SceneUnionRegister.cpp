// SceneUnionRegister: the Union choice of a new account (a SceneFlashDrama). The drama, then the five Union emblems
// (a press shows the "on" emblem, a release opens "Join <Union>?"), the choice sent with the tutorial progress
// (phase 6), then the tutorial stage. An invited player skips the choice (the invitation result popups).
#include "Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_dtor = 0x88A88D, Scene_create = 0x8A5F37, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t function_vv_assign = 0x4EB9CD, function_vb_assign = 0x490109;
constexpr uint32_t SceneFlashDrama_init = 0x6F34BD, SceneFlashDrama_loadLWF = 0x6F39B1;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49, SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t SceneUtil_createBottomInAnimation = 0x45B2F9, SceneUtil_createBottomOutAnimation = 0x45B469;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B, SceneManager_hideHeader = 0x61154D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99, SceneManager_openNoTapScreen = 0x61FB25;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D, APIManager_executeTutorialClear = 0x4C5C89;
constexpr uint32_t APIManager_executeUserGet = 0x4C6081, APIManager_executeStageGet = 0x4CADB9;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionBTE_swap = 0x483E4D, FunctionVTE_swap = 0x4F3699;
constexpr uint32_t Touch_getLocation = 0x900E57, Node_convertToNodeSpace = 0x89599D;
constexpr uint32_t Rect_ctor = 0x8DF18B, Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t SceneUnionRegister_init = 0x71734D, SceneUnionRegister_create = 0x67F73D;
constexpr uint32_t SceneUnionRegister_startTutorialStage = 0x717A09, SceneUnionRegister_noInvitation = 0x718665;
constexpr uint32_t SceneUnionRegister_initUnionButtons = 0x719E39, SceneUnionRegister_assignEnded = 0x71A0C5;
constexpr uint32_t SceneUnionRegister_openPopUpbeLongToUnion = 0x717CC9;
constexpr uint32_t SceneUnionRegister_openInvitationResultPop = 0x718741, SceneUnionRegister_openInvitationNotJoinPop = 0x718DFD;
constexpr uint32_t GOT_VT_SceneUnionRegister = 0xFD6904, GOT_VT_SceneUnionRegisterBase = 0xFD694C;
constexpr uint32_t VT_OnEnter = 0xF6BDD0, VT_OnExit = 0xF6BDA4, VT_BackKey = 0xF6BD78;   // {this}
constexpr uint32_t VT_DramaEnd = 0xF6BD4C;                 // $_3 {this}: the drama's end callback (+0x2A8)
constexpr uint32_t VT_ProgressRetry = 0xF6B9B0;            // $_13 {this, byte}
constexpr uint32_t VT_ProgressSent = 0xF6B9DC;             // $_14 {byte}
constexpr uint32_t VT_TutorialPopupClosed = 0xF6BA34;      // $_12 {this}
constexpr uint32_t VT_TutorialCleared = 0xF6BA08;          // {byte}
constexpr uint32_t VT_StageGot = 0xF6BC9C;                 // $_4 {}
constexpr uint32_t VT_TutorialStageLayer = 0xF6BC70;       // $_5 {}
constexpr uint32_t VT_InDone = 0xF6BC44;                   // $_6 {}
constexpr uint32_t VT_OutDone = 0xF6BC18;                  // $_7 {done} (heap 0x20)
constexpr uint32_t VT_UserGot = 0xF6BDFC;                  // {}
constexpr uint32_t VT_EmblemBegan = 0xF6B824, VT_EmblemMoved = 0xF6B7F8;   // {emblem, emblem, "on" emblem}
constexpr uint32_t VT_EmblemEnded = 0xF6B774;              // {"on" emblem, this, index}
constexpr uint32_t VT_OpenJoinPopup = 0xF6B7A0;            // {this}
constexpr uint32_t VT_JoinAnswer = 0xF6B7CC;               // {this}
constexpr uint32_t VT_ResultClosed = 0xF6BA8C, VT_NotJoinClosed = 0xF6BA60;   // {this}
constexpr uint32_t S_Json = 0xE05902, S_UnionBase = 0xE05925, S_TxtUnionLabel = 0xE0594D;
constexpr uint32_t S_EmblemOnFmt = 0xE05930, S_EmblemFmt = 0xE05940;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_createUnionImagePath = 0x46968D;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t function_vb_copy = 0x45C84D, function_vb_copy2 = 0x47BC69;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t GOT_POPUP_FADE = 0xFD65C0;              // &float: the shadow's fade time
constexpr uint32_t VT_JoinOk = 0xF6BBC0;                   // $_8 {Close, OK, root, layer, shadow, this, answer}
constexpr uint32_t VT_JoinCancel = 0xF6BB94;               // $_9 {Close, OK}
constexpr uint32_t VT_JoinCanClose = 0xF6BB68;             // $_10 {Close}
constexpr uint32_t VT_JoinClosed = 0xF6BB10;               // $_11 {root, layer, shadow}
constexpr uint32_t S_PopupJson = 0xE06B67, S_ButtonOK = 0xDF8688, S_ButtonClose = 0xDF8692, S_TxtOK = 0xDF869F;
constexpr uint32_t S_TxtCancel = 0xDF86A6, S_Txt1Line = 0xE06B8C, S_UnionIcon = 0xE059CC;
constexpr uint32_t V_setPosition = 0x4C, V_getPosition = 0x54, V_setPositionX = 0x64, V_getPositionX = 0x68;
constexpr uint32_t V_setPositionY = 0x6C, V_getPositionY = 0x70, V_setAnchorPoint = 0x9C;
constexpr uint32_t V_release = 0x04, V_setLocalZOrder = 0x0C, V_setVisible = 0xB0, V_isVisible = 0xB4;
constexpr uint32_t V_addChild = 0xF0, V_addChildZ = 0xF4, V_removeFromParent = 0x124, V_init = 0x248;
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t SIZE = 0x2D8;
constexpr uint32_t DRAMA_INTRO = 2000001, DRAMA_CHOSEN = 2000002;
constexpr uint32_t TUTORIAL_UNION = 26;
constexpr uint32_t EPILOGUE_BUTTONS = 0x884;
// SceneUnionRegister (SceneFlashDrama: +0x288 width, +0x28C height, +0x290 drama node, +0x2A0 / +0x2B8 callbacks)
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_W = 0x288, F_H = 0x28C, F_DRAMA = 0x290;
constexpr uint32_t F_FN_2A0 = 0x2A0, F_DRAMA_END = 0x2A8, F_2C0 = 0x2C0;
constexpr uint32_t F_UNION = 0x2C4, F_STATE = 0x2C8, F_ROOT = 0x2CC, F_BASE = 0x2D0;
// SceneManager
constexpr uint32_t SM_TUTORIAL_STAGE = 0x28C, SM_RETRY_FLAG = 0x4CC, SM_RETRY_FN = 0x548, SM_NO_TAP_TIME = 0x300;
// APIManager: +0x4C the user (SafeBuffer; byte 1 = invited), +0x5C the invitation result (+0x78 joined)
constexpr uint32_t API_USER_SB = 0x4C, API_INVITATION = 0x5C, INVITATION_JOINED = 0x78;
constexpr uint32_t PHASE_UNION = 6;

void assignVV(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}

// the manager's retry callback cleared (an empty function<void(bool)>)
void clearRetry() {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    st8(sm + SM_RETRY_FLAG, 1);
    call(function_vb_assign, {sm + SM_RETRY_FN, none});
    functionDestroy(none);
}

// is the touch inside `node`'s content rect (the point converted into `space`)?
bool touchIn(uint32_t space, uint32_t node, uint32_t touch, uint32_t local) {
    Frame f;
    const uint32_t at = f.alloc(8);
    call(Touch_getLocation, {at, touch});
    call(Node_convertToNodeSpace, {local, space, at});
    call(Vec2_dtor, {at});
    const uint32_t w = ld32(vcall(node, 0xAC, {node}));
    const uint32_t h = ld32(vcall(node, 0xAC, {node}) + 4);
    const uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, 0, 0, w, h});
    const bool in = call(Rect_containsPoint, {rect, local}) != 0;
    call(Vec2_dtor, {rect});
    return in;
}
}  // namespace

namespace scene_union_register {

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    st32(self + F_W, fbits(960.0f));
    st32(self + F_H, fbits(540.0f));
    st32(self + F_DRAMA, 0);
    st32(self + F_FN_2A0, 0);
    st32(self + F_DRAMA_END + 0x10, 0);
    st8(self + F_2C0, 0);
    st32(self, ld32(GOT_VT_SceneUnionRegister) + 8);
    st32(self + F_BASE, 0);
    st32(self + F_ROOT, 0);
    st32(self + F_STATE, 0);
    st32(self + F_UNION, 0);
    if (vcall(self, V_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x67F73C, "SceneUnionRegister::create", scene_union_register::create);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneUnionRegister_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x7172D0, "SceneUnionRegister::createScene", scene_union_register::createScene);

uint32_t destruct(uint32_t self) {
    st32(self, ld32(GOT_VT_SceneUnionRegisterBase) + 8);
    functionDestroy(self + F_DRAMA_END);
    return call(Layer_dtor, {self});
}
RW_FUNCTION(0x7172F4, "SceneUnionRegister::~SceneUnionRegister", scene_union_register::destruct);

uint32_t init(uint32_t self) {
    if (call(SceneFlashDrama_init, {self, 1}) != 1) return 0;
    assignVV(self + F_ON_ENTER, VT_OnEnter, self);
    assignVV(self + F_ON_EXIT, VT_OnExit, self);
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t fo = newFunction(f, VT_BackKey, {self});
        call(SceneManager_setOnBackKeyCallback, {sm, fo});
        functionDestroy(fo);
    }
    st32(self + F_STATE, 0);
    {
        Frame f;
        const uint32_t s = newStr(f, S_Json, 0x22);
        st32(self + F_ROOT, call(SceneUtil_createNodeWithSceneFile, {s}));
        strFree(s);
    }
    vcall(ld32(self + F_ROOT), V_setVisible, {ld32(self + F_ROOT), 0});
    vcall(self, V_addChildZ, {self, ld32(self + F_ROOT), 1000});
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneManager_hideHeader, {sm, none});
        functionDestroy(none);
    }
    st32(self + F_BASE, findNode(ld32(self + F_ROOT), S_UnionBase, 0xA));
    {
        const uint32_t text = findNode(ld32(self + F_ROOT), S_TxtUnionLabel, 0xF);
        SceneUtil::setShadow(text);
        setTextId(text, 109150001);
    }
    assignVV(self + F_DRAMA_END, VT_DramaEnd, self);
    call(SceneFlashDrama_loadLWF, {self, DRAMA_INTRO});
    return 1;
}
RW_FUNCTION(0x71734C, "SceneUnionRegister::init", scene_union_register::init);

// the drama ended: state 0 shows the emblems (and the Union tutorial popup), 1 plays the "chosen" drama, 2 sends
// the choice (an invited player: the user data first) and starts the tutorial stage
uint32_t nextState(uint32_t self) {
    const uint32_t state = ld32(self + F_STATE);
    if (state == 2) {
        vcall(ld32(self + F_DRAMA), V_removeFromParent, {ld32(self + F_DRAMA)});
        st32(self + F_DRAMA, 0);
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t sb = ld32(api + API_USER_SB);
        Frame f;
        const uint32_t flag = f.alloc(4);
        call(SafeBuffer_read, {sb, flag + 3, ld32(sb + 0xC) + 1, 1, 1});
        if (!ld8(flag + 3)) {
            call(SceneUnionRegister_noInvitation, {});
        } else {
            const uint32_t sm = sceneManager();
            {
                Frame g;
                const uint32_t retry = newFunction(g, VT_ProgressRetry, {self, 0});
                st8(sm + SM_RETRY_FLAG, 1);
                call(function_vb_assign, {sm + SM_RETRY_FN, retry});
                functionDestroy(retry);
            }
            const uint32_t api2 = call(fn::APIManager_getInstance, {});
            const uint32_t unionId = ld32(self + F_UNION);
            Frame g;
            const uint32_t sent = newFunction(g, VT_ProgressSent, {0});
            call(APIManager_executeTutorialProgress, {api2, PHASE_UNION, 0, 0, 0, 0, unionId + 1, 0, sent});
            functionDestroy(sent);
        }
        call(SceneUnionRegister_startTutorialStage, {self});
    } else if (state == 1) {
        vcall(ld32(self + F_ROOT), V_setVisible, {ld32(self + F_ROOT), 0});
        call(SceneFlashDrama_loadLWF, {self, DRAMA_CHOSEN});
    } else if (state == 0) {
        vcall(ld32(self + F_DRAMA), V_removeFromParent, {ld32(self + F_DRAMA)});
        st32(self + F_DRAMA, 0);
        vcall(ld32(self + F_ROOT), V_setVisible, {ld32(self + F_ROOT), 1});
        Frame f;
        const uint32_t closed = newFunction(f, VT_TutorialPopupClosed, {self});
        const uint32_t popup = f.alloc(20);
        call(SceneUtil_openTutorialPopup, {popup, TUTORIAL_UNION, closed, 0, 0, 0, 0});
        functionDestroy(popup);
        functionDestroy(closed);
    }
    st32(self + F_STATE, ld32(self + F_STATE) + 1);
    return 0;
}
RW_FUNCTION(0x7176EC, "SceneUnionRegister::nextState", scene_union_register::nextState);

uint32_t callTutorialStageStartAPI(uint32_t) {
    clearRetry();
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_StageGot, {});
    call(APIManager_executeStageGet, {api, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x71792C, "SceneUnionRegister::callTutorialStageStartAPI", scene_union_register::callTutorialStageStartAPI);

uint32_t startTutorialStage(uint32_t) {
    st32(sceneManager() + SM_TUTORIAL_STAGE, 1);
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneManager_hideHeader, {sm, none});
        functionDestroy(none);
    }
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, VT_TutorialStageLayer, {});
    call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x717A08, "SceneUnionRegister::startTutorialStage", scene_union_register::startTutorialStage);

uint32_t runInAnimation(uint32_t self) {
    vcall(ld32(self + F_BASE), V_setLocalZOrder, {ld32(self + F_BASE), 2});
    const uint32_t base = ld32(self + F_BASE);
    Frame f;
    const uint32_t done = newFunction(f, VT_InDone, {});
    const uint32_t in = call(SceneUtil_createBottomInAnimation, {base, 0, 0, done});
    call(Node_runAction, {base, call(EaseSineOut_create, {in})});
    functionDestroy(done);
    return 0;
}
RW_FUNCTION(0x717AEC, "SceneUnionRegister::runInAnimation", scene_union_register::runInAnimation);

uint32_t runOutAnimation(uint32_t self, uint32_t done) {
    const uint32_t base = ld32(self + F_BASE);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(0x4683CD, {copy, done});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(VT_OutDone, 0x20);
    if (obj) call(0x47F905, {obj + 8, copy});
    st32(fo + 16, obj);
    const uint32_t out = call(SceneUtil_createBottomOutAnimation, {base, 0, 0, fo});
    call(Node_runAction, {base, call(EaseSineOut_create, {out})});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x717BB0, "SceneUnionRegister::runOutAnimation", scene_union_register::runOutAnimation);

// a new player (no invitation): the user data again, then on
uint32_t noInvitation() {
    clearRetry();
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_UserGot, {});
    call(APIManager_executeUserGet, {api, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x718664, "SceneUnionRegister::nextState user get (sub_718664)", scene_union_register::noInvitation);

// the five emblems: the "on" emblem hidden; a touch listener per emblem
uint32_t initUnionButtons(uint32_t self) {
    for (uint32_t i = 0; i < 5; i++) {
        uint32_t on, emblem;
        {
            Frame f;
            const uint32_t name = stringBuffer(f, {S_EmblemOnFmt, i + 1});
            on = findNode(ld32(self + F_ROOT), name, host_strlen(name));
        }
        vcall(on, V_setVisible, {on, 0});
        vcall(on, V_setTouchEnabled, {on, 0});
        {
            Frame f;
            const uint32_t name = stringBuffer(f, {S_EmblemFmt, i + 1});
            emblem = findNode(ld32(self + F_ROOT), name, host_strlen(name));
        }
        const uint32_t dispatcher = ld32(call(Director_getInstance, {}) + 0x84);
        const uint32_t listener = call(EventListenerTouchOneByOne_create, {});
        call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
        Frame f;
        {
            const uint32_t began = newFunction(f, VT_EmblemBegan, {emblem, emblem, on});
            call(FunctionBTE_swap, {began, listener + 0x50});
            functionDestroy(began);
        }
        const uint32_t captures = f.alloc(12);
        st32(captures, on);
        st32(captures + 4, self);
        st32(captures + 8, i);
        {
            const uint32_t moved = newFunction(f, VT_EmblemMoved, {emblem, emblem, on});
            call(FunctionVTE_swap, {moved, listener + 0x68});
            functionDestroy(moved);
        }
        call(SceneUnionRegister_assignEnded, {listener + 0x80, captures});
        call(SceneUnionRegister_assignEnded, {listener + 0x98, captures});
        call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self});
        vcall(emblem, V_setTouchEnabled, {emblem, 0});
    }
    return EPILOGUE_BUTTONS;
}
RW_FUNCTION(0x719E38, "SceneUnionRegister::initUnionButtons", scene_union_register::initUnionButtons);

// sub_71A0C4: listener callback = the release lambda {"on" emblem, this, index}
uint32_t assignEnded(uint32_t dst, uint32_t captures) {
    Frame f;
    const uint32_t fo = newFunction(f, VT_EmblemEnded, {ld32(captures), ld32(captures + 4), ld32(captures + 8)});
    call(FunctionVTE_swap, {fo, dst});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x71A0C4, "SceneUnionRegister::initUnionButtons ended assign (sub_71A0C4)", scene_union_register::assignEnded);

// released on a shown "on" emblem: the Union kept, no taps for a moment, then the join popup; the emblem hides
uint32_t emblemReleased(uint32_t functor) {
    const uint32_t self = ld32(functor + 8);
    const uint32_t on = ld32(functor + 4);
    if (vcall(on, V_isVisible, {on}) != 1) return 0;
    st32(self + F_UNION, ld32(functor + 0xC));
    call(AudioPlaySystemSEDecide, {});
    const uint32_t sm = sceneManager();
    const uint32_t wait = ld32(sceneManager() + SM_NO_TAP_TIME);
    call(SceneManager_openNoTapScreen, {sm, wait});
    const uint32_t delay = call(DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
    Frame f;
    const uint32_t fo = newFunction(f, VT_OpenJoinPopup, {self});
    const uint32_t then = call(CallFunc_create, {fo});
    call(Node_runAction, {self, call(Sequence_create, {delay, then, 0})});
    functionDestroy(fo);
    vcall(ld32(functor + 4), V_setVisible, {ld32(functor + 4), 0});
    return 0;
}
RW_FUNCTION(0x71A1AC, "SceneUnionRegister::initUnionButtons ended operator()", scene_union_register::emblemReleased);

// the delay over: "Join <Union>?"
uint32_t openJoinPopup(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    Frame f;
    const uint32_t fo = newFunction(f, VT_JoinAnswer, {self});
    call(SceneUnionRegister_openPopUpbeLongToUnion, {self, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x71A354, "SceneUnionRegister::initUnionButtons open join popup operator()", scene_union_register::openJoinPopup);

// moved off the emblem: the "on" emblem hides
uint32_t emblemMoved(uint32_t functor, uint32_t touchRef) {
    Frame f;
    const uint32_t local = f.alloc(8);
    if (!touchIn(ld32(functor + 4), ld32(functor + 8), ld32(touchRef), local))
        vcall(ld32(functor + 0xC), V_setVisible, {ld32(functor + 0xC), 0});
    call(Vec2_dtor, {local});
    return 0;
}
RW_FUNCTION(0x71A524, "SceneUnionRegister::initUnionButtons moved operator()", scene_union_register::emblemMoved);

// pressed on the emblem: the "on" emblem shows
uint32_t emblemPressed(uint32_t functor, uint32_t touchRef) {
    Frame f;
    const uint32_t local = f.alloc(8);
    uint32_t result = 0;
    if (touchIn(ld32(functor + 4), ld32(functor + 8), ld32(touchRef), local)) {
        result = 1;
        vcall(ld32(functor + 0xC), V_setVisible, {ld32(functor + 0xC), 1});
    }
    call(Vec2_dtor, {local});
    return result;
}
RW_FUNCTION(0x71A6A4, "SceneUnionRegister::initUnionButtons began operator()", scene_union_register::emblemPressed);

// $_12: the Union tutorial popup closed - the emblems, and for an invited player the invitation result
uint32_t tutorialPopupClosed(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    call(SceneUnionRegister_initUnionButtons, {self});
    const uint32_t invitation = ld32(api + API_INVITATION);
    if (!invitation) return 0;
    Frame f;
    if (ld8(invitation + INVITATION_JOINED)) {
        const uint32_t fo = newFunction(f, VT_ResultClosed, {self});
        call(SceneUnionRegister_openInvitationResultPop, {VT_ResultClosed, fo});
        functionDestroy(fo);
    } else {
        const uint32_t fo = newFunction(f, VT_NotJoinClosed, {self});
        call(SceneUnionRegister_openInvitationNotJoinPop, {VT_NotJoinClosed, fo});
        functionDestroy(fo);
    }
    return 0;
}
RW_FUNCTION(0x71B920, "SceneUnionRegister::nextState::$_12 operator()", scene_union_register::tutorialPopupClosed);

// $_13: the retry callback - unless the request retries itself, the choice is sent again
uint32_t progressRetry(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    if (call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) return 0;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t unionId = ld32(self + F_UNION);
    Frame f;
    const uint32_t sent = newFunction(f, VT_ProgressSent, {ld8(functor + 8)});
    call(APIManager_executeTutorialProgress, {api, PHASE_UNION, 0, 0, 0, 0, unionId + 1, 0, sent});
    functionDestroy(sent);
    return 0;
}
RW_FUNCTION(0x71B59C, "SceneUnionRegister::nextState::$_13 operator()", scene_union_register::progressRetry);

// $_14: the choice sent - the tutorial cleared on the server
uint32_t progressSent(uint32_t, uint32_t okRef) {
    if (!ld8(okRef)) return 0;
    clearRetry();
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_TutorialCleared, {0});
    call(APIManager_executeTutorialClear, {api, fo, 0});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x71B704, "SceneUnionRegister::nextState::$_14 operator()", scene_union_register::progressSent);

// "Join <Union>?": the Union's name and emblem, OK ($_8: the answer) / Cancel
uint32_t openPopUpbeLongToUnion(uint32_t self, uint32_t answer) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x20C});
    const uint32_t opacity = vcall(shadow, 0x20C, {shadow});
    vcall(shadow, 0x214, {shadow, 0});
    call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), opacity})});
    vcall(layer, V_addChild, {layer, shadow});
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PopupJson});
    {
        const float x = bitsf(vcall(root, V_getPositionX, {root}));
        const float m = bitsf(call(SceneUtil_getSideMargin, {fbits(x)}));
        vcall(root, V_setPositionX, {root, fbits(x + m)});
        const float y = bitsf(vcall(root, V_getPositionY, {root}));
        const float hm = bitsf(call(SceneUtil_getSideHeightMargin, {fbits(y)}));
        vcall(root, V_setPositionY, {root, fbits(y + hm)});
    }
    vcall(layer, V_addChild, {layer, root});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(root, V_setAnchorPoint, {root, anchor});
        call(Vec2_dtor, {anchor});
    }
    vcall(root, V_setPosition, {root, vcall(shadow, V_getPosition, {shadow})});
    const uint32_t ok = findNode(root, S_ButtonOK, 9);
    const uint32_t close = findNode(root, S_ButtonClose, 0xC);
    Frame f;
    {
        const uint32_t copy = f.alloc(20);
        call(function_vb_copy, {copy, answer});
        Frame g;
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(VT_JoinOk, 0x38);
        if (obj) {
            const uint32_t words[6] = {close, ok, root, layer, shadow, self};
            for (uint32_t k = 0; k < 6; k++) st32(obj + 8 + 4 * k, words[k]);
            call(function_vb_copy2, {obj + 0x20, copy});
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        functionDestroy(copy);
    }
    {
        Frame g;
        const uint32_t s = newStr(g, S_TxtOK, 6);
        call(SceneUtil_settingTitleLabel, {ok, s, 100100001});
        strFree(s);
    }
    addTouch(close, VT_JoinCancel, {close, ok});
    {
        Frame g;
        const uint32_t s = newStr(g, S_TxtCancel, 0xA);
        call(SceneUtil_settingTitleLabel, {close, s, 100100002});
        strFree(s);
    }
    {
        const uint32_t sm = sceneManager();
        Frame g;
        const uint32_t canClose = newFunction(g, VT_JoinCanClose, {close});
        const uint32_t closed = newFunction(g, VT_JoinClosed, {root, layer, shadow});
        call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
        functionDestroy(closed);
        functionDestroy(canClose);
    }
    // the five Union names
    uint32_t names = f.alloc(60);
    {
        uint32_t td[5];
        for (uint32_t k = 0; k < 5; k++) {
            td[k] = textData(f, 103710001 + int32_t(k));
            strInitC(names + 12 * k, textDataStr(td[k]));
        }
        for (int k = 4; k >= 0; k--) sharedRelease(td[k]);
    }
    {
        const uint32_t text = findNode(root, S_Txt1Line, 0xF);
        SceneUtil::setShadow(text);
        Frame g;
        const uint32_t td = textData(g, 109150002);
        const uint32_t name = names + 12 * ld32(self + F_UNION);
        setTextC(text, stringBuffer(g, {textDataStr(td), strData(name)}));
        sharedRelease(td);
    }
    {
        const uint32_t icon = findNode(root, S_UnionIcon, 9);
        Frame g;
        const uint32_t path = g.alloc(12);
        call(SceneUtil_createUnionImagePath, {path, ld32(self + F_UNION) + 1, 1, 0});
        const uint32_t s = newStr(g, strData(path));
        call(ImageView_loadTexture, {icon, s, 0});
        strFree(s);
        strFree(path);
    }
    {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(Node_runAction, {root, call(SceneUtil_createOpenPopupAnimation, {root, none})});
        functionDestroy(none);
    }
    for (int k = 4; k >= 0; k--) strFree(names + 12 * uint32_t(k));
    return 0;
}
RW_FUNCTION(0x717CC8, "SceneUnionRegister::openPopUpbeLongToUnion", scene_union_register::openPopUpbeLongToUnion);

}  // namespace scene_union_register
