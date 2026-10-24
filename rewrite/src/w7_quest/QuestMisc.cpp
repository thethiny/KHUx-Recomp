// Quest leftovers: SceneBattleResult's keyblade-got popup (with the reward sparkle), the new keyblade and title
// popups; CarouselDeck's guilt bonus number and its burst / armature lambdas; GuiltBurstCutInShader's colour
// helpers and the raid boss backgrounds; a result scroll view's touch-began lambda; an event banner's sound lambda.
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

#include <cmath>

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create, SceneBattleResult_create = 0x6053F1;
constexpr uint32_t SceneBattleResult_openGetKeyBladePopup = 0x682BE1, SceneUtil_openUserTitlePopup = 0x470725;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61, ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_move = 0x47F905, function_b_call = 0x51022D;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD, audio_Manager_load = 0x5BEEF5;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D, audio_Manager_stop = 0x5BF3AD, audio_Layer_playSE = 0x5BF785;
constexpr uint32_t LWFNode_create = rwlib::cocos2d::LWFNode::create, Bone_changeDisplayWithIndex = rwlib::cocostudio::Bone::changeDisplayWithIndex, SafeBuffer_read = 0x45A269;
constexpr uint32_t CarouselDeck_removeArmature = 0x5D120D, CarouselDeck_setBurstEncourage = 0x5D0DE1;
constexpr uint32_t CarouselDeck_setTutorialAction = 0x5CF2D9;
constexpr uint32_t Widget_getTouchBeganPosition = rwlib::cocos2d::ui::Widget::getTouchBeganPosition, Widget_getTouchEndPosition = rwlib::cocos2d::ui::Widget::getTouchEndPosition;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create, CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t dynamic_cast_ = 0xC9A171, NodeVector_copy = 0x4962C5, NodeVector_dtor = 0x496317;
constexpr uint32_t GuiltBurst_set_lwf_color = 0x495ED9, LWFNodeVector_pushSlow = 0x60C10D;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation, Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t GOT_TI_NODE = 0xFD612C, GOT_TI_LWFBITMAP = 0xFD6130, GOT_TI_LWFNODE = 0xFD6134;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054, AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t VT_NewKeybladeDone = 0xF6457C, VT_TitleDone = 0xF645A8;   // {this}
constexpr uint32_t VT_KeybladeOk = 0xF644A0;      // {pad, root, layer, shadow, pad, done} (heap 0x30)
constexpr uint32_t VT_KeybladeOpened = 0xF64474;  // {root}
constexpr uint32_t VT_KeybladeClosed = 0xF644CC;  // {pad, layer, shadow, root, pad, done} (heap 0x30)
constexpr uint32_t VT_BurstDelay = 0xF5D254;      // {deck}
constexpr uint32_t S_KbJson = 0xE047A5, S_Txt = 0xDF94D1, S_KB = 0xDFEF39, S_Panel = 0xDFD420;
constexpr uint32_t S_TxtTitleLabel = 0xDF9554, S_TxtKbLabel = 0xE047CD, S_ButtonOk = 0xDF9564, S_TxtOK = 0xDF869F;
constexpr uint32_t S_AreaPoint = 0xDF9FB4, S_RewardLwf = 0xDF9FBF, S_MainPanel = 0xDFD1FC;
constexpr uint32_t V_setVisible = 0xB0, V_addChild = 0xF0, V_getChildren = 0x10C, V_getParent = 0x11C;
constexpr uint32_t V_setPosition = 0x4C, V_getPosition = 0x54, V_getContentSize = 0xAC, V_setOpacity = 0x214;
constexpr uint32_t V_setColor = 0x22C, V_getAnimation = 0x298, V_hitTest = 0x2BC, V_stopPress = 0x138;
constexpr uint32_t V_animPlayIndex = 0x7C, V_animStop = 0x80;
constexpr uint32_t SE_REWARD = 509100420, SE_BURST_READY = 509000410, SE_BANNER = 509000130;
// SceneBattleResult: +0x2B4 the user titles (shared_ptr), +0x2C4 the new keyblade index, +0x2CC the keyblades
// got (68 bytes each: +4 the image path, +0x10 the name)
constexpr uint32_t BR_TITLES = 0x2B4, BR_KB_INDEX = 0x2C4, BR_KBS = 0x2CC;
// CarouselDeck
constexpr uint32_t CD_GUILT_ARMATURE = 0x33C, CD_DIGITS = 0x340, CD_ICON_A = 0x34C, CD_ICON_B = 0x350;
constexpr uint32_t CD_PRESS = 0x290, CD_ARMATURE = 0x314, CD_FN_A = 0x3B0, CD_FN_B = 0x3C8, CD_FN_C = 0x3E0;
constexpr uint32_t CD_ACTIVE = 0x3F8, CD_PRESSED = 0x3F9, CD_TUTORIAL = 0x410;

void labelOf(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

uint32_t playSE(uint32_t id) {
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    const uint32_t r = call(audio_Layer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
    return r;
}

// a std::string copied the way libc++ copies (long: a new buffer; short: the 12 bytes)
void strCopy(uint32_t dst, uint32_t src) {
    if (strIsLong(src)) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}

uint32_t f2b(float v) { return v <= 0.0f ? 0 : uint32_t(v) & 0xFF; }
}  // namespace

namespace quest_misc {

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneBattleResult_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x681440, "SceneBattleResult::createScene", quest_misc::createScene);

// the keyblade just got (image, name) in the "got a keyblade" popup
uint32_t openNewKeyBladePopup(uint32_t self) {
    const uint32_t entry = ld32(self + BR_KBS) + 68 * ld32(self + BR_KB_INDEX);
    Frame f;
    const uint32_t path = f.alloc(12), name = f.alloc(12);
    strCopy(path, entry + 4);
    strCopy(name, entry + 0x10);
    const uint32_t done = newFunction(f, VT_NewKeybladeDone, {self});
    call(SceneBattleResult_openGetKeyBladePopup, {self, path, name, done});
    functionDestroy(done);
    strFree(name);
    strFree(path);
    return 0;
}
RW_FUNCTION(0x681CC4, "SceneBattleResult::openNewKeyBladePopup", quest_misc::openNewKeyBladePopup);

uint32_t openUserTitlePopup(uint32_t self, uint32_t title) {
    Frame f;
    const uint32_t done = newFunction(f, VT_TitleDone, {self});
    call(SceneUtil_openUserTitlePopup, {self + BR_TITLES, title, done});
    functionDestroy(done);
    return 0;
}
RW_FUNCTION(0x682B48, "SceneBattleResult::openUserTitlePopup", quest_misc::openUserTitlePopup);

// BattleResult_KB.json: the keyblade's image and name, OK closes it and runs `done`
uint32_t openGetKeyBladePopup(uint32_t, uint32_t path, uint32_t name, uint32_t done) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x85});
    vcall(shadow, V_setOpacity, {shadow, 0});
    vcall(layer, V_addChild, {layer, shadow});
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_KbJson});
    centerOnMargins(root);
    vcall(layer, V_addChild, {layer, root});
    {
        const uint32_t text = findNode(root, S_Txt, 3);
        SceneUtil::setShadow(text);
        setTextC(text, strData(name));
    }
    {
        const uint32_t image = findNode(root, S_KB, 2);
        Frame f;
        const uint32_t s = newStr(f, strData(path));
        call(ImageView_loadTexture, {image, s, 0});
        strFree(s);
    }
    {
        Frame f;
        const uint32_t s = newStr(f, S_Panel, 5);
        const uint32_t panel = SceneUtil::searchNodeByName(root, s);
        vcall(panel, V_setOpacity, {panel, 255});
        strFree(s);
    }
    labelOf(root, S_TxtTitleLabel, 0xF, 101910002);
    {
        const uint32_t text = findNode(root, S_TxtKbLabel, 0xC);
        SceneUtil::setShadow(text);
        setTextId(text, 103010001);
    }
    const uint32_t ok = findNode(root, S_ButtonOk, 9);
    labelOf(ok, S_TxtOK, 6, 100100001);
    {
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(function_vv_copy, {copy, done});
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapObject(VT_KeybladeOk, 0x30);
        if (obj) {
            st32(obj + 8, root);
            st32(obj + 0xC, layer);
            st32(obj + 0x10, shadow);
            call(function_vv_move, {obj + 0x18, copy});
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        functionDestroy(copy);
    }
    call(audio_Manager_load, {call(audio_Manager_getInstance, {}), SE_REWARD});
    Frame f;
    const uint32_t opened = newFunction(f, VT_KeybladeOpened, {root});
    call(Node_runAction, {root, call(SceneUtil_createOpenPopupAnimation, {root, opened})});
    functionDestroy(opened);
    return 0;
}
RW_FUNCTION(0x682BE0, "SceneBattleResult::openGetKeyBladePopup", quest_misc::openGetKeyBladePopup);

// the popup opened: the reward sound and sparkle at Area_Point
uint32_t keybladeOpened(uint32_t functor) {
    playSE(SE_REWARD);
    const uint32_t point = findNode(ld32(functor + 4), S_AreaPoint, 10);
    const uint32_t lwf = call(LWFNode_create, {S_RewardLwf, 0});
    const uint32_t parent = vcall(point, V_getParent, {point});
    vcall(parent, V_addChild, {parent, lwf});
    vcall(lwf, V_setPosition, {lwf, vcall(point, V_getPosition, {point})});
    return 0;
}
RW_FUNCTION(0x684284, "SceneBattleResult::openGetKeyBladePopup opened operator() (sub_684284)", quest_misc::keybladeOpened);

// OK: the closing animation, then `done`
uint32_t keybladeOk(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t root = ld32(functor + 8);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vv_copy, {copy, functor + 0x18});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(VT_KeybladeClosed, 0x30);
    if (obj) {
        st32(obj + 8, ld32(functor + 0xC));
        st32(obj + 0xC, ld32(functor + 0x10));
        st32(obj + 0x10, root);
        call(function_vv_move, {obj + 0x18, copy});
    }
    st32(fo + 16, obj);
    call(Node_runAction, {root, call(SceneUtil_createClosePopupAnimation, {root, fo})});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x68454C, "SceneBattleResult::openGetKeyBladePopup OK operator() (sub_68454C)", quest_misc::keybladeOk);

// the guilt bonus percentage (`bonus` / 100, at most 999) in the three digit bones, the medal's attribute icons,
// the bonus armature shown
uint32_t playGuiltBonusEffect(uint32_t self, uint32_t medalRef, uint32_t bonus) {
    if (!ld32(self + CD_GUILT_ARMATURE)) return 0;
    uint32_t value = bonus / 800 <= 124 ? bonus / 100 : 999;
    const uint32_t digits = value ? uint32_t(int32_t(std::log10(double(value)))) + 1 : 1;
    for (uint32_t i = 0; i < 3; i++) {
        const uint32_t bone = ld32(self + CD_DIGITS + 4 * i);
        if (!bone) continue;
        if (i >= digits) {
            call(Bone_changeDisplayWithIndex, {bone, 0xFFFFFFFF, 1});
        } else {
            call(Bone_changeDisplayWithIndex, {bone, uint32_t(int32_t(value) % 10), 1});
            value = uint32_t(int32_t(value) / 10);
        }
    }
    const uint32_t medal = ld32(medalRef);
    if (medal) {
        Frame f;
        const uint32_t v = f.alloc(4);
        call(SafeBuffer_read, {medal, v, ld32(medal + 0xC) + 0x478, 4, 0x478});
        const uint32_t icon = ld32(v) - 1;
        if (ld32(self + CD_ICON_A)) call(Bone_changeDisplayWithIndex, {ld32(self + CD_ICON_A), icon, 1});
        if (ld32(self + CD_ICON_B)) call(Bone_changeDisplayWithIndex, {ld32(self + CD_ICON_B), icon, 1});
    }
    const uint32_t armature = ld32(self + CD_GUILT_ARMATURE);
    const uint32_t anim = vcall(armature, V_getAnimation, {armature});
    vcall(anim, V_animPlayIndex, {anim, 0});
    vcall(ld32(self + CD_GUILT_ARMATURE), V_setVisible, {ld32(self + CD_GUILT_ARMATURE), 1});
    return 0;
}
RW_FUNCTION(0x5D129C, "CarouselDeck::playGuiltBonusEffect", quest_misc::playGuiltBonusEffect);

// an armature's movement ended (complete / loop complete): stopped and hidden
uint32_t armatureFinished(uint32_t, uint32_t armatureRef, uint32_t typeRef) {
    const uint32_t r = ld32(typeRef) - 1;
    if (r > 1) return r;
    const uint32_t armature = ld32(armatureRef);
    const uint32_t anim = vcall(armature, V_getAnimation, {armature});
    vcall(anim, V_animStop, {anim, 0});
    return vcall(armature, V_setVisible, {armature, 0});
}
RW_FUNCTION(0x5D298A, "CarouselDeck armature movement end operator() (sub_5D298A)", quest_misc::armatureFinished);

// the burst area: pressed - after 0.65 s the burst encouragement (when allowed); released / cancelled - the press
// effect stops; a swipe to the right that left the area while every condition holds plays the burst-ready sound
uint32_t burstAreaTouched(uint32_t functor, uint32_t widgetRef, uint32_t typeRef) {
    const uint32_t deck = ld32(functor + 4);
    if (!ld8(deck + CD_ACTIVE)) return 0;
    const uint32_t type = ld32(typeRef);
    if (type == 2) {
        vcall(ld32(deck + CD_PRESS), V_stopPress, {ld32(deck + CD_PRESS)});
        if (!ld32(deck + CD_TUTORIAL)) call(CarouselDeck_removeArmature, {CD_ARMATURE, ld32(deck + CD_ARMATURE)});
        st8(deck + CD_PRESSED, 0);
        return 0;
    }
    if (type == 1) return 0;
    if (type == 0) {
        const uint32_t delay = call(DelayTime_create, {fbits(0.65f)});
        Frame f;
        const uint32_t fo = newFunction(f, VT_BurstDelay, {deck});
        const uint32_t then = call(CallFunc_create, {fo});
        call(Node_runAction, {deck, call(Sequence_create, {delay, then, 0})});
        functionDestroy(fo);
        if (ld32(deck + CD_FN_B + 0x10) && call(function_b_call, {deck + CD_FN_B}) == 1)
            call(CarouselDeck_setBurstEncourage, {deck, 1});
        st8(deck + CD_PRESSED, 1);
        return 0;
    }
    const uint32_t widget = ld32(widgetRef);
    const uint32_t began = call(Widget_getTouchBeganPosition, {widget});
    const uint32_t ended = call(Widget_getTouchEndPosition, {widget});
    if (vcall(widget, V_hitTest, {widget, began}) == 1 && !vcall(widget, V_hitTest, {widget, ended}) &&
        ldf(began) <= ldf(ended) && ld32(deck + CD_FN_B + 0x10) && call(function_b_call, {deck + CD_FN_B}) == 1 &&
        ld32(deck + CD_FN_A + 0x10) && call(function_b_call, {deck + CD_FN_A}) == 1 &&
        ld32(deck + CD_FN_C + 0x10) && call(function_b_call, {deck + CD_FN_C}) == 1) {
        playSE(SE_BURST_READY);
        if (ld32(deck + CD_TUTORIAL)) {
            Frame f;
            const uint32_t none = newEmptyFunction(f);
            call(CarouselDeck_setTutorialAction, {deck, none});
            functionDestroy(none);
        }
    }
    vcall(ld32(deck + CD_PRESS), V_stopPress, {ld32(deck + CD_PRESS)});
    call(CarouselDeck_removeArmature, {CD_ARMATURE, ld32(deck + CD_ARMATURE)});
    st8(deck + CD_PRESSED, 0);
    return 0;
}
RW_FUNCTION(0x5D2AF4, "CarouselDeck burst area touch operator() (sub_5D2AF4)", quest_misc::burstAreaTouched);

// a sprite's colour at `level` (0..1) grey
uint32_t setSprColor(uint32_t, uint32_t node, uint32_t level) {
    const uint32_t c = f2b(bitsf(level) * 255.0f);
    Frame f;
    const uint32_t color = f.alloc(4);
    call(fn::Color3B_ctor, {color, c, c, c});
    vcall(node, V_setColor, {node, color});
    return 0;
}
RW_FUNCTION(0x495DF8, "GuiltBurstCutInShader::set_spr_color", quest_misc::setSprColor);

// the same for an LWF bitmap or LWF node and, recursively, all its children
uint32_t setLwfColor(uint32_t self, uint32_t node, uint32_t level) {
    if (node) {
        uint32_t target = call(dynamic_cast_, {node, ld32(GOT_TI_NODE), ld32(GOT_TI_LWFBITMAP), 0});
        if (!target) target = call(dynamic_cast_, {node, ld32(GOT_TI_NODE), ld32(GOT_TI_LWFNODE), 0});
        if (target) {
            const uint32_t c = f2b(bitsf(level) * 255.0f);
            Frame f;
            const uint32_t color = f.alloc(4);
            call(fn::Color3B_ctor, {color, c, c, c});
            vcall(target, V_setColor, {target, color});
        }
    }
    Frame f;
    const uint32_t children = f.alloc(12);
    call(NodeVector_copy, {children, vcall(node, V_getChildren, {node})});
    for (uint32_t k = 0; int32_t(ld32(children + 4) - ld32(children)) >= 1 && k < (ld32(children + 4) - ld32(children)) >> 2; k++)
        call(GuiltBurst_set_lwf_color, {self, ld32(ld32(children) + 4 * k), level});
    call(NodeVector_dtor, {children});
    return 0;
}
RW_FUNCTION(0x495ED8, "GuiltBurstCutInShader::set_lwf_color", quest_misc::setLwfColor);

uint32_t addRaidBossBg(uint32_t self, uint32_t lwf) {
    Frame f;
    const uint32_t slot = f.alloc(4);
    st32(slot, lwf);
    const uint32_t end = ld32(self + 0x2BC);
    if (end == ld32(self + 0x2C0)) {
        call(LWFNodeVector_pushSlow, {self + 0x2B8, slot});
        return 0;
    }
    uint32_t base = 0;
    if (end) {
        st32(end, lwf);
        base = ld32(self + 0x2BC);
    }
    st32(self + 0x2BC, base + 4);
    return 0;
}
RW_FUNCTION(0x609B5C, "GuiltBurstCutInShader::addRaidBossBg", quest_misc::addRaidBossBg);

// a page cell's touch began: inside its main panel (while not scrolling or dragging) the touched row is kept
uint32_t resultCellTouchBegan(uint32_t functor, uint32_t touchRef) {
    const uint32_t view = ld32(functor + 4);
    if (!ld8(view + 0x664) || ld8(view + 0x654) || ld8(view + 0x65C)) return 0;
    const uint32_t touch = ld32(touchRef);
    Frame f;
    {
        const uint32_t at = f.alloc(8);
        call(Touch_getLocation, {at, touch});
        st32(view + 0x660, ld32(at + 4));
        call(Vec2_dtor, {at});
    }
    const uint32_t cell = ld32(ld32(view + 0x51C) + 4 * ld32(functor + 8));
    const uint32_t panel = findNode(cell, S_MainPanel, 9);
    if (!panel) return 0;
    const uint32_t at = f.alloc(8), local = f.alloc(8);
    call(Touch_getLocation, {at, touch});
    call(Node_convertToNodeSpace, {local, panel, at});
    call(Vec2_dtor, {at});
    const uint32_t w = ld32(vcall(panel, V_getContentSize, {panel}));
    const uint32_t h = ld32(vcall(panel, V_getContentSize, {panel}) + 4);
    const uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, 0, 0, w, h});
    uint32_t r = 0;
    if (call(Rect_containsPoint, {rect, local})) {
        const int32_t row = int32_t(ldf(view + 0x540)) / int32_t(ldf(view + 0x518) + 0.0f) + int32_t(ld32(functor + 8)) - 1;
        st32(view + 0x658, uint32_t(row));
        const uint32_t n = (ld32(view + 0x62C) - ld32(view + 0x628)) >> 2;
        if (n <= uint32_t(row)) st32(view + 0x658, n - 1);
        st8(view + 0x654, 1);
        r = 1;
    }
    call(Vec2_dtor, {rect});
    call(Vec2_dtor, {local});
    return r;
}
RW_FUNCTION(0x51195C, "AvatarTitleScrollView item touch began operator() (sub_51195C)", quest_misc::resultCellTouchBegan);

// an event banner page turned: the previous sound stopped, the page sound played
uint32_t bannerSound(uint32_t functor) {
    const uint32_t view = ld32(functor + 4);
    const uint32_t old = ld32(view + 0x3C0);
    call(audio_Manager_stop, {call(audio_Manager_getInstance, {}), old, 50});
    const uint32_t id = playSE(SE_BANNER);
    st32(view + 0x3C0, id);
    st8(view + 0x2E4, 1);
    st8(view + 0x31C, 1);
    return 0;
}
RW_FUNCTION(0x5858C8, "AdvEventBannerScrollView page sound operator() (sub_5858C8)", quest_misc::bannerSound);

}  // namespace quest_misc
