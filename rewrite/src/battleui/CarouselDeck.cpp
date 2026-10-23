// CarouselDeck: life cycle, layout loading (init), armature helpers and the small deck state functions.
#include "CarouselDeck.hpp"

using namespace scene;
using namespace cdeck;

// ------------------------------------------------------------------------------------------- life cycle
uint32_t CarouselDeck::create() {
    uint32_t obj = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<CarouselDeck>(obj));
    if (vcall(obj, VT_INIT, {obj}) == 1) {
        call(Ref_autorelease, {obj});
        return obj;
    }
    vcall(obj, 4, {obj});    // deleting destructor
    return 0;
}
RW_FUNCTION(0x75E948, "CarouselDeck::create", CarouselDeck::create);

CarouselDeck *CarouselDeck::construct(CarouselDeck *self) {
    call(Node_ctor, {rw::guest_addr(self)});
    self->vtable = VTABLE;
    self->root = 0;
    self->rollAction = 0;
    self->keybladeInfo = 0;
    self->tickerTextX = 0.0f;
    self->tickerFieldWidth = 0.0f;
    self->medalDummy = 0;
    self->cutinSprite = 0;
    self->guiltBonusArmature = 0;
    self->guiltIcon = 0;
    self->guiltIconEffect = 0;
    self->medalButtonCount = 0;
    self->friendMedalButton = 0;
    self->attackingMedal = 0;
    self->friendMedalUi = 0;
    self->keybladeButton = 0;
    self->currentEffect = 0;
    self->tacticalPanel = 0;
    self->deckLabel01 = 0;
    self->deckLabel02 = 0;
    self->attackAbilityAnm = 0;
    self->attackMedalAnm = 0;
    // memset(+0x30C, 0, 0x18): the six deck armatures
    self->enemyTurnArmature = 0;
    self->burstReadyArmature = 0;
    self->longTapArmature = 0;
    self->deckInfoArmature = 0;
    self->deckInArmature = 0;
    self->medalAttackArmature = 0;
    self->deckSize = 1;
    self->currentIndex = 0;
    self->isBurstPossible.callable = 0;
    self->isInfoWindowAllowed.callable = 0;
    self->isAttackAllowed.callable = 0;
    self->tutorialAction.callable = 0;
    self->guiltShader = 0;
    self->bursting = 0;
    self->rolling = 0;
    self->touching = 0;
    self->inBattle = 0;
    self->attacking = 0;
    return self;
}
RW_FUNCTION(0x763A34, "CarouselDeck::CarouselDeck", CarouselDeck::construct);

CarouselDeck *CarouselDeck::destruct(CarouselDeck *self) {
    self->vtable = VTABLE;
    if (self->rollAction) {
        call(ActionObject_stop, {self->rollAction});
        releaseRef(self->rollAction);
    }
    for (auto &b : self->medalButtons) releaseRef(b);
    for (auto &w : self->abilityInfoWidgets) releaseRef(w);
    for (auto &w : self->keybladeWindows) releaseRef(w);
    for (auto &w : self->tacticalBoxes) releaseRef(w);
    for (auto &a : self->cutinArmatures) releaseRef(a);
    releaseRef(self->friendMedalButton);
    releaseRef(self->enemyTurnArmature);
    releaseRef(self->burstReadyArmature);
    releaseRef(self->longTapArmature);
    releaseRef(self->deckInfoArmature);
    releaseRef(self->deckInArmature);
    releaseRef(self->medalAttackArmature);
    releaseRef(self->guiltBonusArmature);
    releaseRef(self->cutinSprite);
    releaseRef(self->currentEffect);
    const uint32_t me = rw::guest_addr(self);
    vcall(me, node::removeAllChildren, {me});
    functionDestroy(rw::guest_addr(&self->tutorialAction));
    functionDestroy(rw::guest_addr(&self->isAttackAllowed));
    functionDestroy(rw::guest_addr(&self->isInfoWindowAllowed));
    functionDestroy(rw::guest_addr(&self->isBurstPossible));
    call(Node_dtor, {me});
    return self;
}
RW_FUNCTION(0x5CDAC8, "CarouselDeck::~CarouselDeck", CarouselDeck::destruct);

void CarouselDeck::destructDelete(CarouselDeck *self) {
    call(fn::operator_delete, {rw::guest_addr(destruct(self))});
}
RW_FUNCTION(0x5CDD68, "CarouselDeck::~CarouselDeck (deleting)", CarouselDeck::destructDelete);

// ------------------------------------------------------------------------------------------- init
namespace {
// ArmatureDataManager::addArmatureFileInfo("cocostudio/publish/<name>.ExportJson")
void addArmatureFile(uint32_t name) {
    uint32_t mgr = call(ArmatureDataManager_getInstance, {});
    Frame f;
    uint32_t path = stringBuffer(f, {STR_FMT_EXPORT_JSON, STR_PUBLISH, name});
    uint32_t s = newStr(f, path);
    call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
    strFree(s);
}

// GUIReader::widgetFromJsonFile(reader, "cocostudio/publish/<json>")
uint32_t loadWidget(uint32_t reader, uint32_t json) {
    Frame f;
    uint32_t path = stringBuffer(f, {STR_FMT_PATH, STR_PUBLISH, json});
    return call(GUIReader_widgetFromJsonFile, {reader, path});
}

// armature->getAnimation()->setMovementEventCallFunc / setFrameEventCallFunc(lambda capturing the deck)
void setAnimationCallback(uint32_t armature, uint32_t setter, uint32_t vtable, std::initializer_list<uint32_t> cap) {
    uint32_t anim = vcall(armature, VT_GET_ANIMATION, {armature});
    Frame f;
    uint32_t cb = newFunction(f, vtable, cap);
    call(setter, {anim, cb});
    functionDestroy(cb);
}
}  // namespace

bool CarouselDeck::init(CarouselDeck *self) {
    const uint32_t me = rw::guest_addr(self);
    if (call(Node_init, {me}) != 1) return false;

    uint32_t reader = call(GUIReader_getInstance, {});
    self->root = loadWidget(reader, STR_ActionMap_KeyBladeDeck);
    vcall(me, node::addChildZ, {me, self->root, 1});
    uint32_t actions = call(ActionManagerEx_getInstance, {});
    self->rollAction = call(ActionManagerEx_getActionByName, {actions, STR_ActionMap_KeyBladeDeck, STR_DeckRoll01});
    call(Ref_retain, {self->rollAction});
    call(ActionObject_setLoop, {self->rollAction, 0});

    self->keybladeButton = find(self->root, STR_KeyBladeButton);
    addTouch(self->keybladeButton, VT_LAMBDA_KEYBLADE_TOUCH, {me});

    // the five medal places: Medal_0k (the first three hold an AbilityInfo); the placeholder image is removed
    for (uint32_t k = 1; k <= 5; k++) {
        uint32_t medal;
        {
            Frame f;
            uint32_t s = newStr(f, stringBuffer(f, {STR_FMT_MEDAL, k}));
            medal = SceneUtil::searchNodeByName(self->root, s);
            self->medalNodes[k - 1] = medal;
            strFree(s);
        }
        if (k - 1 <= 2) self->abilityInfos[k - 1] = find(medal, STR_AbilityInfo);
        medal = self->medalNodes[k - 1];
        Frame f;
        uint32_t s = newStr(f, stringBuffer(f, {STR_FMT_IMAGE, k}));
        vcall(medal, VT_REMOVE_CHILD_BY_NAME, {medal, s, 1});
        strFree(s);
    }
    for (auto &w : self->abilityInfoWidgets) {
        uint32_t r = call(GUIReader_getInstance, {});
        w = loadWidget(r, STR_ActionMap_AbilityInfo);
        call(Ref_retain, {w});
        vcall(w, node::setTag, {w, 0});
    }
    self->friendMedalUi = find(self->root, STR_FriendlMedalUI);
    setVisible(self->friendMedalUi, 0);
    setVisible(find(self->root, STR_MedalPanel), 1);
    self->attackMedalAnm = find(self->root, STR_AttackMedal_Anm);
    self->attackAbilityAnm = find(self->root, STR_AttackAbility_Anm);
    for (auto &w : self->keybladeWindows) {
        uint32_t r = call(GUIReader_getInstance, {});
        w = loadWidget(r, STR_ActionMap_KeyBladeWindow);
        call(Ref_retain, {w});
    }
    self->keybladeInfo = find(self->root, STR_KeyBladeInfo);
    addArmatureFile(STR_ActMapBuffDeBuffAnim);
    reader = call(GUIReader_getInstance, {});
    for (auto &w : self->tacticalBoxes) {
        w = loadWidget(reader, STR_ActMapDeckTacticalInfo_box);
        call(Ref_retain, {w});
        reader = call(GUIReader_getInstance, {});
    }
    self->tacticalPanel = loadWidget(reader, STR_ActMapDeckTacticalInfo_panel);
    setVisible(self->tacticalPanel, 0);
    uint32_t holder = find(self->root, STR_DeckInfo_Null);
    vcall(holder, node::addChildZ, {holder, self->tacticalPanel, 50});
    self->deckLabel02 = find(self->root, STR_Medal_Deck_Label02);
    self->deckLabel01 = find(self->root, STR_Medal_Deck_Label01);

    addArmatureFile(STR_ActMapEnmUIAnim);
    addArmatureFile(STR_ActMapAAMedalAnim);
    self->enemyTurnArmature = createArmature(self, STR_ActMapEnmUIAnim, STR_DeckEnemyTurn);
    self->burstReadyArmature = createArmature(self, STR_ActMapAAMedalAnim, STR_AttackAbilityStart);
    self->longTapArmature = createArmature(self, STR_ActMapAAMedalAnim, STR_LongTap);
    self->medalButtonCount = 0;    // memset(+0x354, 0, 0x18)
    for (auto &b : self->medalButtons) b = 0;
    addArmatureFile(STR_ActMapCarouselAnim);
    self->deckInfoArmature = createArmature(self, STR_ActMapCarouselAnim, STR_DeckInfo_anim);
    self->deckInArmature = createArmature(self, STR_ActMapCarouselAnim, STR_Deck_in);
    self->medalAttackArmature = createArmature(self, STR_ActMapCarouselAnim, STR_MedalAttack_Normal);
    self->medalDummy = replaceBone(self, self->medalAttackArmature, STR_Medal_Dummy);

    // the special attack cut-ins, centred on the frame
    for (uint32_t k = 0; k < 4; k++) {
        uint32_t a = createArmature(self, STR_ActMapCarouselAnim, ld32(CUTIN_MOVEMENT_NAMES + 4 * k));
        self->cutinArmatures[k] = a;
        if (!a) continue;
        uint32_t bone = replaceBone(self, a, STR_Cutin_Dummy);
        vcall(a, VT_SET_USER_OBJECT, {a, bone});
        setAnimationCallback(self->cutinArmatures[k], ArmatureAnimation_setMovementEventCallFunc,
                             VT_LAMBDA_CUTIN_MOVEMENT, {me});
        uint32_t cut = self->cutinArmatures[k];
        float x = bitsf(call(AppGetGrameFrameWidth, {})) * 0.5f;
        float y = bitsf(call(fn::AppGetGrameFrameHeight, {})) * 0.5f;
        Frame f;
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor_xy, {pos, fbits(x), fbits(y)});
        vcall(cut, node::setPosition, {cut, pos});
        call(Vec2_dtor, {pos});
    }
    {
        Frame f;
        uint32_t s = newStr(f, stringBuffer(f, {STR_FMT_CUTIN, 31053}));
        self->cutinSprite = call(Sprite_create, {s});
        strFree(s);
    }
    call(Ref_retain, {self->cutinSprite});
    self->guiltBonusArmature = initGuiltBonusEffect(self, STR_ActMapCarouselAnim, STR_GuiltBonus);
    for (auto &w : self->guiltValues) w = 0;    // memset(+0x394, 0, 0x18)

    setAnimationCallback(self->deckInArmature, ArmatureAnimation_setFrameEventCallFunc, VT_LAMBDA_DECKIN_FRAME, {me});
    setAnimationCallback(self->deckInArmature, ArmatureAnimation_setMovementEventCallFunc, VT_LAMBDA_DECKIN_MOVEMENT,
                         {me});
    setAnimationCallback(self->medalAttackArmature, ArmatureAnimation_setMovementEventCallFunc,
                         VT_LAMBDA_ATTACK_MOVEMENT, {me});
    addArmatureFile(STR_ActionMapAnimation);
    self->currentEffect = createArmature(self, STR_ActionMapAnimation, STR_DeckInfoCurrentEFF);

    // Deck_in sits on the keyblade button, the medal attack on the first medal place
    Frame f;
    uint32_t pos1 = f.alloc(8), pos2 = f.alloc(8);
    call(Vec2_ctor_copy, {pos1, VEC2_ZERO});
    trackingNodePosition(self, pos1, self->keybladeButton, me);
    vcall(self->deckInArmature, node::setPosition, {self->deckInArmature, pos1});
    vcall(me, node::addChildZ, {me, self->deckInArmature, 10});
    call(Vec2_ctor_copy, {pos2, VEC2_ZERO});
    trackingNodePosition(self, pos2, self->medalNodes[0], me);
    vcall(self->medalAttackArmature, node::setPosition, {self->medalAttackArmature, pos2});
    setVisible(self->root, 0);
    {
        Frame g;
        uint32_t none = newEmptyFunction(g);
        setTutorialAction(self, none);
        functionDestroy(none);
    }
    call(audio_Manager_load, {call(audio_Manager_getInstance, {}), SE_DECK});
    call(Vec2_dtor, {pos2});
    call(Vec2_dtor, {pos1});
    return true;
}
RW_FUNCTION(0x5CDD74, "CarouselDeck::init", CarouselDeck::init);

// ------------------------------------------------------------------------------------------- armatures
// A retained armature of `file` playing `movement` paused on its first frame; 0 when the movement is unknown.
uint32_t CarouselDeck::createArmature(CarouselDeck *, uint32_t file, uint32_t movement) {
    uint32_t arm;
    {
        Frame f;
        uint32_t s = newStr(f, file);
        arm = call(Armature_create, {s});
        strFree(s);
    }
    uint32_t anim = vcall(arm, VT_GET_ANIMATION, {arm});
    uint32_t data = vcall(anim, VT_ANIM_GET_ANIMATION_DATA, {anim});
    uint32_t found;
    {
        Frame f;
        uint32_t s = newStr(f, movement);
        found = call(AnimationData_getMovement, {data, s});
        strFree(s);
    }
    if (!found) return 0;
    call(Ref_retain, {arm});
    anim = vcall(arm, VT_GET_ANIMATION, {arm});
    {
        Frame f;
        uint32_t s = newStr(f, movement);
        vcall(anim, VT_ANIM_PLAY, {anim, s, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(s);
    }
    anim = vcall(arm, VT_GET_ANIMATION, {arm});
    vcall(anim, VT_ANIM_GOTO_AND_PAUSE, {anim, 0});
    return arm;
}
RW_FUNCTION(0x5CED78, "CarouselDeck::createArmature", CarouselDeck::createArmature);

// Replace the display of bone `bone` with an empty node (returned) the caller can hang things on.
uint32_t CarouselDeck::replaceBone(CarouselDeck *, uint32_t armature, uint32_t bone) {
    if (!armature) return 0;
    uint32_t b;
    {
        Frame f;
        uint32_t s = newStr(f, bone);
        b = vcall(armature, VT_ARMATURE_GET_BONE, {armature, s});
        strFree(s);
    }
    if (!b) return 0;
    uint32_t n = call(Node_create, {});
    vcall(n, node::setCascadeOpacityEnabled, {n, 1});
    uint32_t dm = vcall(b, VT_BONE_GET_DISPLAY_MANAGER, {b});
    call(DisplayManager_removeDisplay, {dm, 0});
    call(DisplayManager_addDisplay, {dm, n, 0});
    call(DisplayManager_changeDisplayWithIndex, {dm, 0, 1});
    return n;
}
RW_FUNCTION(0x5CEF10, "CarouselDeck::replaceBone", CarouselDeck::replaceBone);

// The guilt bonus armature: added to the deck (hidden), its three number bones shown, the icon bones kept.
uint32_t CarouselDeck::initGuiltBonusEffect(CarouselDeck *self, uint32_t file, uint32_t movement) {
    uint32_t arm = createArmature(self, file, movement);
    if (!arm) return 0;
    setAnimationCallback(arm, ArmatureAnimation_setMovementEventCallFunc, VT_LAMBDA_GUILT_MOVEMENT, {});
    vcall(arm, node::setPositionX, {arm, call(AppGetGrameFrameWidth, {})});
    for (uint32_t k = 0; k < 3; k++) {
        Frame f;
        uint32_t s = newStr(f, ld32(GUILT_NUM_BONE_NAMES + 4 * k));
        self->guiltBonusNum[k] = vcall(arm, VT_ARMATURE_GET_BONE, {arm, s});
        strFree(s);
        setVisible(self->guiltBonusNum[k], 1);
    }
    {
        Frame f;
        uint32_t s = newStr(f, STR_GuiltBIcon, 10);
        self->guiltIcon = vcall(arm, VT_ARMATURE_GET_BONE, {arm, s});
        strFree(s);
    }
    {
        Frame f;
        uint32_t s = newStr(f, STR_GuiltBIcon_eff, 14);
        self->guiltIconEffect = vcall(arm, VT_ARMATURE_GET_BONE, {arm, s});
        strFree(s);
    }
    const uint32_t me = rw::guest_addr(self);
    vcall(me, node::addChildZ, {me, arm, 40});
    setVisible(arm, 0);
    return arm;
}
RW_FUNCTION(0x5CF024, "CarouselDeck::initGuiltBonusEffect", CarouselDeck::initGuiltBonusEffect);

// Show an armature under `parent` (playing from its first frame) unless it already has a parent.
bool CarouselDeck::addArmature(CarouselDeck *, uint32_t armature, uint32_t parent, int32_t zOrder) {
    if (!armature || vcall(armature, node::getParent, {armature})) return false;
    uint32_t anim = vcall(armature, VT_GET_ANIMATION, {armature});
    vcall(anim, VT_ANIM_GOTO_AND_PLAY, {anim, 0});
    vcall(parent, node::addChildZ, {parent, armature, uint32_t(zOrder)});
    return true;
}
RW_FUNCTION(0x5D1468, "CarouselDeck::addArmature", CarouselDeck::addArmature);

// Take a shown armature off its parent (paused on its first frame).
bool CarouselDeck::removeArmature(CarouselDeck *, uint32_t armature) {
    if (!armature || !vcall(armature, node::getParent, {armature})) return false;
    uint32_t anim = vcall(armature, VT_GET_ANIMATION, {armature});
    vcall(anim, VT_ANIM_GOTO_AND_PAUSE, {anim, 0});
    vcall(armature, node::removeFromParent, {armature});
    return true;
}
RW_FUNCTION(0x5D120C, "CarouselDeck::removeArmature", CarouselDeck::removeArmature);

// pos = node's position in the space of its ancestor `stop` (sum of the positions up the parent chain);
// false when `stop` is not an ancestor.
bool CarouselDeck::trackingNodePosition(CarouselDeck *, uint32_t pos, uint32_t target, uint32_t stop) {
    uint32_t p = vcall(target, node::getPosition, {target});
    st32(pos, ld32(p));
    st32(pos + 4, ld32(p + 4));
    uint32_t parent = vcall(target, node::getParent, {target});
    if (parent == stop) return true;
    while (parent) {
        call(Vec2_add, {pos, vcall(parent, node::getPosition, {parent})});
        parent = vcall(parent, node::getParent, {parent});
        if (parent == stop) return true;
    }
    return false;
}
RW_FUNCTION(0x5CF268, "CarouselDeck::trackingNodePosition", CarouselDeck::trackingNodePosition);

// ------------------------------------------------------------------------------------------- tutorial / style
// A tutorial step on the deck: with an action, the field mask and the hint text are shown, the current medal
// button is brought to the front and the long-tap hint plays; without one, everything is reset and the pending
// action runs once and is cleared.
void CarouselDeck::setTutorialAction(CarouselDeck *self, uint32_t action) {
    const uint32_t me = rw::guest_addr(self);
    uint32_t has = ld32(action + 16) ? 1 : 0;
    uint32_t mask = find(self->root, STR_Tutorial_FieldMask);
    setVisible(mask, has);
    vcall(mask, VT_WIDGET_SET_TOUCH_ENABLED, {mask, has});
    uint32_t text = find(self->root, STR_Tutorial_txt);
    setVisible(text, has);
    if (has) {
        call(function_vv_assign, {rw::guest_addr(&self->tutorialAction), action});
        setTextId(text, TEXT_TUTORIAL_DECK);
        uint32_t button = self->medalButtons[self->currentIndex];
        vcall(button, VT_SET_GLOBAL_Z_ORDER, {button, fbits(1.0f)});
        setBurstEncourage(self, 1);
        return;
    }
    for (auto &b : self->medalButtons)
        if (uint32_t button = b) vcall(button, VT_SET_GLOBAL_Z_ORDER, {button, 0});
    const uint32_t fo = rw::guest_addr(&self->tutorialAction);
    uint32_t p = 0;
    if (self->tutorialAction.callable) {
        call(fn::function_vv_call, {fo});
        p = self->tutorialAction.callable;
    }
    if (p == fo) vcall(p, 0x10, {p});
    else if (p) vcall(p, 0x14, {p});
    self->tutorialAction.callable = 0;
    (void)me;
}
RW_FUNCTION(0x5CF2D8, "CarouselDeck::setTutorialAction", CarouselDeck::setTutorialAction);

// The deck skin (DECK_STYLE_RES_* file name formats take the style number); the deck info animation is put on
// the keyblade button.
void CarouselDeck::changeDeckStyle(CarouselDeck *self, uint32_t style) {
    if (!style) return;
    {
        uint32_t deck = find(self->root, STR_KeyBladeDeck);
        Frame f;
        uint32_t s = newStr(f, stringBuffer(f, {ld32(DECK_STYLE_RES_CAROUSEL_NAME), style}));
        call(ImageView_loadTexture, {deck, s, 0});
        strFree(s);
    }
    {
        uint32_t button = self->keybladeButton;
        Frame f;
        uint32_t s = newStr(f, stringBuffer(f, {ld32(DECK_STYLE_RES_BUTTON_NAME), style}));
        call(Button_loadTextureNormal, {button, s, 0});
        strFree(s);
    }
    {
        uint32_t num = find(self->root, STR_DeckMedalNumUI);
        Frame f;
        uint32_t s = newStr(f, stringBuffer(f, {ld32(DECK_STYLE_RES_MEDAL_NUN_NAME), style}));
        call(ImageView_loadTexture, {num, s, 0});
        strFree(s);
    }
    uint32_t size = call(Widget_getCustomSize, {self->keybladeButton});
    uint32_t button = self->keybladeButton;
    vcall(button, node::addChildZ, {button, self->deckInfoArmature, 10});
    uint32_t anim = vcall(self->deckInfoArmature, VT_GET_ANIMATION, {self->deckInfoArmature});
    vcall(anim, VT_ANIM_GOTO_AND_PLAY, {anim, 0});
    uint32_t arm = self->deckInfoArmature;
    float x = ldf(size) * 0.5f;
    float y = ldf(size + 4) * 0.5f;
    vcall(arm, node::setPositionXY, {arm, fbits(x), fbits(y)});
}
RW_FUNCTION(0x5CF520, "CarouselDeck::changeDeckStyle", CarouselDeck::changeDeckStyle);

// ------------------------------------------------------------------------------------------- deck medals
// One medal button per deck medal (at most five), with its ability info, keyblade window and tactical box.
// medalIds: the user medal ids (u64) of the deck; their master medal ids come from the user's medal box.
void CarouselDeck::addMedalOntoDeck(CarouselDeck *self, uint32_t medalIds, int32_t count) {
    uint32_t api = call(fn::APIManager_getInstance, {});
    self->medalButtonCount = 0;
    uint32_t added = 0;
    if (uint32_t(count) > 5 || count >= 1) {
        uint32_t n = uint32_t(count) >= 5 ? 5 : uint32_t(count);
        for (uint32_t k = 0; k < n; k++) {
            int32_t medalId = 1;
            {
                Frame f;
                uint32_t key = f.alloc(8), medal = f.alloc(72);
                uint32_t box = ld32(api + APIManager_userMedals);
                st32(key, ld32(medalIds + 8 * k));
                st32(key + 4, ld32(medalIds + 8 * k + 4));
                uint32_t node = call(MedalIndex_find, {box + UserMedal_index, key});
                if (node) {
                    int32_t slot = int32_t(ld32(node + 0x10));
                    if (slot >= 0) {
                        uint32_t at = 72u * uint32_t(slot);
                        call(SafeBuffer_read, {box, medal, ld32(box + 12) + at, 72, at});
                        medalId = int32_t(ld32(medal + 0x08));   // UserMedal record: the master medal id
                    }
                }
            }
            uint32_t button = createMadalButton(self, medalId);
            if (button) {
                uint32_t c = self->medalButtonCount;
                if (c <= 4) {
                    if (uint32_t old = self->medalButtons[c]) {
                        call(Ref_release, {old});
                        c = self->medalButtonCount;
                    }
                    self->medalButtons[c] = button;
                    call(Ref_retain, {self->medalButtons[self->medalButtonCount]});
                    self->medalButtonCount = self->medalButtonCount + 1;
                }
            }
            int32_t r = medalId % 10;
            self->cutinMedalIds[k] = (r == 4 || r == 2) ? int32_t(uint32_t(medalId) - 1) : medalId;
            setAbilityInfo(self, self->abilityInfoWidgets[k], self->keybladeWindows[k], medalId);
            setDeckMedalInfoPanel(self, self->tacticalBoxes[k], medalId, 0, 0);
        }
        added = self->medalButtonCount;
    }
    self->deckSize = int32_t(uint32_t(self->deckSize) + added);
}
RW_FUNCTION(0x5CF80C, "CarouselDeck::addMedalOntoDeck", CarouselDeck::addMedalOntoDeck);

// A medal button with the medal's small image (img/medal/Medal_S_<image>.png).
uint32_t CarouselDeck::createMadalButton(CarouselDeck *self, int32_t medalId) {
    uint32_t button;
    {
        Frame f;
        uint32_t row = masterRow(f, Medal_create, uint32_t(medalId >= 1 ? medalId : 1));
        int32_t image = safe32(ld32(row), MEDAL_IMAGE);
        uint32_t normal = newStr(f, stringBuffer(f, {STR_FMT_MEDAL_S, uint32_t(image)}));
        uint32_t selected = newStr(f, STR_EMPTY, 0);
        uint32_t disabled = newStr(f, STR_EMPTY, 0);
        button = call(Button_create, {normal, selected, disabled, 0});
        strFree(disabled);
        strFree(selected);
        strFree(normal);
        sharedRelease(row);
    }
    addTouch(button, VT_LAMBDA_MEDAL_TOUCH, {rw::guest_addr(self)});
    return button;
}
RW_FUNCTION(0x5CF9A0, "CarouselDeck::createMadalButton", CarouselDeck::createMadalButton);

// The cut-in of medal ids ending in 2 / 4 is the one of the id below (ending in 1 / 3).
int32_t CarouselDeck::convertMedalIDtoCutinMedalID(CarouselDeck *, int32_t medalId) {
    int32_t r = medalId % 10;
    return (r == 4 || r == 2) ? int32_t(uint32_t(medalId) - 1) : medalId;
}
RW_FUNCTION(0x5CFB6C, "CarouselDeck::convertMedalIDtoCutinMedalID", CarouselDeck::convertMedalIDtoCutinMedalID);

// ------------------------------------------------------------------------------------------- deck state
// Back to the first medal: the special attack hint off; when not on the first medal, Deck_in plays again.
bool CarouselDeck::reflashDeck(CarouselDeck *self) {
    setPossibleBurst(self, 0, 0);
    if (!self->currentIndex) return false;
    uint32_t anim = vcall(self->deckInArmature, VT_GET_ANIMATION, {self->deckInArmature});
    vcall(anim, VT_ANIM_GOTO_AND_PLAY, {anim, 0});
    setVisible(self->deckInArmature, 1);
    return true;
}
RW_FUNCTION(0x5D0BC4, "CarouselDeck::reflashDeck", CarouselDeck::reflashDeck);

// The "special attack possible" armature on the attack medal: shown when asked for, not on the last medal, the
// isBurstPossible callback agrees and the battle is on. show: make it visible.
void CarouselDeck::setPossibleBurst(CarouselDeck *self, uint32_t possible, uint32_t show) {
    uint32_t ok = (self->currentIndex < int32_t(uint32_t(self->deckSize) - 1)) ? 1 : 0;
    ok &= possible;
    uint32_t cb = 0;
    if (self->isBurstPossible.callable) cb = call(function_b_call, {rw::guest_addr(&self->isBurstPossible)});
    ok &= cb;
    if (ok & self->inBattle) addArmature(self, self->burstReadyArmature, self->attackMedalAnm, 2);
    else removeArmature(self, self->burstReadyArmature);
    if (show == 1) setVisible(self->burstReadyArmature, 1);
    setActiveMedalBurstCost(self);
}
RW_FUNCTION(0x5D0C10, "CarouselDeck::setPossibleBurst", CarouselDeck::setPossibleBurst);

// The long-tap hint on the attack medal while the special attack armature is shown and visible.
void CarouselDeck::setBurstEncourage(CarouselDeck *self, uint32_t on) {
    uint32_t ready = self->burstReadyArmature;
    if (on == 1 && ready && vcall(ready, node::getParent, {ready}) &&
        vcall(self->burstReadyArmature, node::isVisible, {self->burstReadyArmature}) == 1) {
        addArmature(self, self->longTapArmature, self->attackMedalAnm, 0);
        return;
    }
    removeArmature(self, self->longTapArmature);
}
RW_FUNCTION(0x5D0DE0, "CarouselDeck::setBurstEncourage", CarouselDeck::setBurstEncourage);

// The keyblade button was pressed / released: the deck info window follows (only in battle when allowed).
void CarouselDeck::onCenterButton(CarouselDeck *self, uint32_t on) {
    if (self->inBattle) {
        uint32_t allowed = 0;
        if (self->isInfoWindowAllowed.callable)
            allowed = call(function_b_call, {rw::guest_addr(&self->isInfoWindowAllowed)});
        on = allowed & on;
    }
    setVisibleDeckInfoWindow(self, on);
}
RW_FUNCTION(0x5D0E32, "CarouselDeck::onCenterButton", CarouselDeck::onCenterButton);

void CarouselDeck::finishRollAnimation(CarouselDeck *self) {
    setDeckMedal(self, 0);
    call(ActionObject_updateToFrameByTime, {self->rollAction, 0});
    self->rolling = 0;
}
RW_FUNCTION(0x5D0E6C, "CarouselDeck::finishRollAnimation", CarouselDeck::finishRollAnimation);

void CarouselDeck::finishActionAttack(CarouselDeck *self) {
    vcall(self->medalDummy, node::removeAllChildren, {self->medalDummy});
    removeArmature(self, self->medalAttackArmature);
    self->attacking = 0;
}
RW_FUNCTION(0x5D1258, "CarouselDeck::finishActionAttack", CarouselDeck::finishActionAttack);

void CarouselDeck::finishActionBurst(CarouselDeck *self, uint32_t armature) {
    removeArmature(self, armature);
    self->bursting = 0;
}
RW_FUNCTION(0x5D1284, "CarouselDeck::finishActionBurst", CarouselDeck::finishActionBurst);

void CarouselDeck::disableButton(CarouselDeck *self) {
    setBattleState(self, 0);
    vcall(self->keybladeButton, VT_WIDGET_SET_ENABLED, {self->keybladeButton, 0});
}
RW_FUNCTION(0x5D13E0, "CarouselDeck::disableButton", CarouselDeck::disableButton);

// Battle on / off; off clears the keyblade info, the long-tap hint, the special attack hint and the info window.
void CarouselDeck::setBattleState(CarouselDeck *self, uint32_t on) {
    self->inBattle = uint8_t(on);
    if (!on) {
        vcall(self->keybladeInfo, node::removeAllChildren, {self->keybladeInfo});
        removeArmature(self, self->longTapArmature);
        setPossibleBurst(self, 0, 0);
        onCenterButton(self, 0);
    }
    vcall(self->medalButtons[0], VT_WIDGET_SET_ENABLED, {self->medalButtons[0], on});
}
RW_FUNCTION(0x5D1406, "CarouselDeck::setBattleState", CarouselDeck::setBattleState);

bool CarouselDeck::isVisibleDeckInfoWindow(CarouselDeck *self) {
    if (!self->tacticalPanel) return false;
    return vcall(self->tacticalPanel, node::isVisible, {self->tacticalPanel}) != 0;
}
RW_FUNCTION(0x5D14C4, "CarouselDeck::isVisibleDeckInfoWindow", CarouselDeck::isVisibleDeckInfoWindow);

void CarouselDeck::onAutoButton(CarouselDeck *) {}
RW_FUNCTION(0x5D14E4, "CarouselDeck::onAutoButton", CarouselDeck::onAutoButton);

// The special attack armature is shown: its visibility.
bool CarouselDeck::isEffectiveBurstRound(CarouselDeck *self) {
    uint32_t ready = self->burstReadyArmature;
    if (!ready || !vcall(ready, node::getParent, {ready})) return false;
    return vcall(self->burstReadyArmature, node::isVisible, {self->burstReadyArmature}) != 0;
}
RW_FUNCTION(0x5D1B28, "CarouselDeck::isEffectiveBurstRound", CarouselDeck::isEffectiveBurstRound);
