// SceneAvatarEdit: life cycle (create / init / destructors) and the in / out animations.
#include "SceneAvatarEdit.hpp"

#include "../scene_social/SocialCommon.hpp"

using namespace social;
using Self = SceneAvatarEdit;

namespace {
namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;             // __aeabi_memset(dst, n, c)
constexpr uint32_t ImageButton_create = 0x531239;       // ImageButton::create(std::string, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;   // (button, Node*, float)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;   // (button, std::function by value)
constexpr uint32_t AvatarActor_create = 0x73FD21;       // AvatarActor::create(const Avatar::Style*)
constexpr uint32_t Avatar_preloadResources = 0x5C7ED1;
constexpr uint32_t AvatarPartsScrollView_create = 0x553679;
constexpr uint32_t function_vi_swap = 0x5428E1;         // std::function<void(int)>::swap
constexpr uint32_t function_vv_copy = 0x4683CD;         // std::function<void()>::function(const function&)
constexpr uint32_t function_vv_move = 0x47F905;         // std::function<void()>::function(function&&)
constexpr uint32_t vector_int_dtor = 0x478961;          // std::__vector_base<int>::~__vector_base
constexpr uint32_t vector_vector_int_dtor = 0x51B045;   // std::__vector_base<std::vector<int>>::~__vector_base
constexpr uint32_t vector_vector_int_push_slow = 0x70DCC5;   // vector<vector<int>>::__push_back_slow_path(vector&&)
constexpr uint32_t vector_coordinate_dtor = 0x703EE1;   // std::__vector_base<Coordinate>::~__vector_base
constexpr uint32_t vector_bool_push_back = 0x62161D;    // std::vector<bool>::push_back(const bool&)
constexpr uint32_t ImageButtonVector_dtor = 0x703F1F;   // cocos2d::Vector<ImageButton*>::~Vector
constexpr uint32_t Node_create = 0x89372D;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t SceneUtil_createFadeInAnimation = 0x45BCA1;
constexpr uint32_t SceneUtil_createFadeOutAnimation = 0x45BCE5;
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;
constexpr uint32_t SceneUtil_createLeftOutAnimation = 0x45B735;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneManager_hideHeader = 0x61154D;     // (mgr, std::function<void()> by value)
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED;   // (mgr, type, const string&)
constexpr uint32_t SceneManager_setFinishSceneCallback = 0x613E3F;
constexpr uint32_t TextData_get = 0x4928AD;
}  // namespace g

constexpr uint32_t VEC2_ANCHOR_BOTTOM_LEFT_GOT = 0xFD6080;   // GOT slot of cocos2d::Vec2::ANCHOR_BOTTOM_LEFT

// std::function vtables (__func<lambda> + 8)
constexpr Addr VT_init_L0 = 0xF6AD24;     // _onEnterCallback: scheduleUpdate
constexpr Addr VT_init_L1 = 0xF6AD00;     // _onExitCallback
constexpr Addr VT_init_L2 = 0xF6ACD4;     // back key: backScene
constexpr Addr VT_init_L3 = 0xF6ACA8;     // "decide" (first creation)
constexpr Addr VT_init_L4 = 0xF6AC48;     // "save" (editing)
constexpr Addr VT_init_L5 = 0xF6AC1C;     // left switch area
constexpr Addr VT_init_L6 = 0xF6ABF0;     // right switch area
constexpr Addr VT_init_L7 = 0xF6ABC4;     // parts list: item selected
constexpr Addr VT_init_L8 = 0xF6AB98;     // parts list: item (de)selected in multi select
constexpr Addr VT_init_L9 = 0xF6AB6C;     // tap on the avatar
constexpr Addr VT_init_L10 = 0xF6AB40;    // finish scene callback
constexpr Addr VT_runOut_L11 = 0xF6AB14;  // runOutAnimationMain: the done callback, wrapped

// strings
constexpr uint32_t S_SceneJson = 0xE06753;        // "cocostudio/publish/AvatarEditScene_ver120.json"
constexpr uint32_t S_Book = 0xE06782;
constexpr uint32_t S_CenterUI = 0xE02FBA;
constexpr uint32_t S_CenterUI_Left = 0xE05448;
constexpr uint32_t S_CenterUI_Right = 0xE04ED9;
constexpr uint32_t S_Switch = 0xE06787;
constexpr uint32_t S_Tab_Accessorie = 0xE0678E;
constexpr uint32_t S_Decision = 0xE0679D;
constexpr uint32_t S_Txt_Wording = 0xDF862F;
constexpr uint32_t S_BgCircle = 0xE067A6;         // "img/ui/Bg_Circle_Gradation.png"
constexpr uint32_t S_BgDeck = 0xE03AF2;           // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t S_Button_AvatarSave = 0xE067C5;
constexpr uint32_t S_Txt_AvatarSave = 0xE067D7;
constexpr uint32_t S_Area_Hit_Left = 0xE04425;
constexpr uint32_t S_Area_Hit_Right = 0xE04433;
constexpr uint32_t S_ImageButton_Check = 0xDFDC5C;
constexpr uint32_t S_Empty = 0xDF8A1A;            // ""
constexpr uint32_t S_Area = 0xDF9378;
constexpr uint32_t S_Avatar = 0xDF8F56;

// node virtual slots (vtable byte offsets) not in SceneCommon.hpp
constexpr uint32_t SLOT_init = 0x250;             // read from the static vtable symbol (_ZTV + 0x250)
constexpr uint32_t SLOT_release = 0x04;
constexpr uint32_t SLOT_removeFromParentSlot = 0x124;
constexpr uint32_t SLOT_getLocalZOrder = 0x18;
constexpr uint32_t SLOT_setPositionX = 0x64;
constexpr uint32_t SLOT_onEnterSlot300 = 0x300;   // SceneAvatarEdit vtable +0x300 (called at the end of init)

Addr setTitle(Addr node, uint32_t name, uint32_t len, int32_t id) {
    return withStr(name, len, [&](Addr s) { return call(sfn::SceneUtil_settingTitleLabel, {node, s, uint32_t(id)}); });
}

// ImageButton::create("", false)
Addr newImageButton() {
    return withStr(S_Empty, 0, [&](Addr s) { return call(g::ImageButton_create, {s, 0}); });
}

// The area node's "ImageButton_Check" child, created on the fly when the layout has none (left side: the new one
// is used directly).
Addr areaButton(Addr area, bool researchAfterCreate) {
    Addr button = search(area, S_ImageButton_Check, 17);
    if (button) return button;
    Addr created = newImageButton();
    call(g::ImageButton_setTargetView, {created, area, fbits(1.0f)});
    withStr(S_ImageButton_Check, 17, [&](Addr s) { vcall(area, node::addChildZName, {area, created, 0, s}); });
    if (!researchAfterCreate) return created;
    return search(area, S_ImageButton_Check, 17);
}

// node->runAction(EaseSineOut::create(anim)) where anim = make(done) with an empty std::function
template <class F> void runEased(Addr node, F make) {
    withEmptyFunction([&](Addr none) {
        Addr anim = make(none);
        Addr eased = call(g::EaseSineOut_create, {anim});
        call(sfn::Node_runAction, {node, eased});
    });
}

}  // namespace

// ------------------------------------------------------------------------------------------------ create / destroy
Addr Self::create() {
    Addr self = call(fn::operator_new, {SIZE});
    call(sfn::Layer_ctor, {self});
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->vtable2 = VTABLE2;
    s->isTutorialEdit = 0;
    call(g::aeabi_memset, {self + 0x290, 0x51, 0});         // root .. isCreate
    rw::set<uint16_t>(s, 0x2EC, 0);                          // isMale, showKeyblade
    s->tabIndex = 0;
    s->coordinateIndex = 0;
    call(g::aeabi_memset, {self + 0x2F0, 0x1C, 0});         // scrollView, partIdsByTab, coordinates
    for (int k = 9; k >= 0; k--) s->filter[k] = 1;
    // init() through the static vtable entry
    if (call(ld32(TYPEINFO_VTABLE + SLOT_init), {self}) == 1) {
        call(sfn::Ref_autorelease, {self});
        return self;
    }
    vcall(self, SLOT_release, {self});
    return 0;
}
RW_FUNCTION(0x54BEF8, "SceneAvatarEdit::create", SceneAvatarEdit::create);

Addr Self::createSceneLayer() { return call(0x54BEF9, {}); }   // SceneAvatarEdit::create
RW_FUNCTION(0x54BEF0, "createSceneLayer<SceneAvatarEdit>", SceneAvatarEdit::createSceneLayer);

Addr Self::createScene() {
    Addr scene = call(sfn::Scene_create, {});
    Addr layer = call(0x54BEF9, {});                         // SceneAvatarEdit::create
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x703E3C, "SceneAvatarEdit::createScene", SceneAvatarEdit::createScene);

SceneAvatarEdit *Self::destroy(SceneAvatarEdit *self) {
    Addr a = rw::guest_addr(self);
    rw::set<uint32_t>(self, 0, VTABLE);
    self->vtable2 = VTABLE2;
    call(g::vector_coordinate_dtor, {a + 0x300});
    call(g::vector_vector_int_dtor, {a + 0x2F4});
    if (self->newTabsFemale[0]) call(fn::operator_delete, {self->newTabsFemale[0]});
    if (self->newTabsMale[0]) call(fn::operator_delete, {self->newTabsMale[0]});
    call(g::ImageButtonVector_dtor, {a + 0x2BC});
    call(g::ImageButtonVector_dtor, {a + 0x2B0});
    call(sfn::Layer_dtor, {a});
    return self;
}
RW_FUNCTION(0x703E60, "SceneAvatarEdit::~SceneAvatarEdit", SceneAvatarEdit::destroy);

SceneAvatarEdit *Self::destroyThunk(Addr base2) {
    return rw::host_ptr<Self>(call(0x703E61, {base2 - 0x288}));   // ~SceneAvatarEdit
}
RW_FUNCTION(0x703F4C, "non-virtual thunk to SceneAvatarEdit::~SceneAvatarEdit", SceneAvatarEdit::destroyThunk);

void Self::destroyDelete(SceneAvatarEdit *self) {
    Addr p = call(0x703E61, {rw::guest_addr(self)});           // ~SceneAvatarEdit
    call(fn::operator_delete, {p});
}
RW_FUNCTION(0x703F5C, "SceneAvatarEdit::~SceneAvatarEdit (deleting)", SceneAvatarEdit::destroyDelete);

void Self::destroyDeleteThunk(Addr base2) {
    Addr p = call(0x703E61, {base2 - 0x288});
    call(fn::operator_delete, {p});
}
RW_FUNCTION(0x703F68, "non-virtual thunk to SceneAvatarEdit::~SceneAvatarEdit (deleting)",
            SceneAvatarEdit::destroyDeleteThunk);

// ------------------------------------------------------------------------------------------------ init
uint32_t Self::init(SceneAvatarEdit *self) {
    Addr me = rw::guest_addr(self);
    if (call(sfn::Layer_init, {me}) != 1) return 0;

    // Node::_onEnterCallback = $_0 (scheduleUpdate), _onExitCallback = $_1
    withLambda(VT_init_L0, {me}, [&](Addr f) { call(sfn::function_vv_assign, {me + 0x210, f}); });
    withLambda(VT_init_L1, {me}, [&](Addr f) { call(sfn::function_vv_assign, {me + 0x228, f}); });
    {
        Addr mgr = call(sfn::SceneManager_getInstance, {});
        withLambda(VT_init_L2, {me}, [&](Addr f) { call(sfn::SceneManager_setOnBackKeyCallback, {mgr, f}); });
    }
    // creating the first avatar: UserData byte 1 (no avatar yet)
    Addr api = call(sfn::APIManager_getInstance, {});
    self->isCreate = uint8_t(safe32(ld32(api + 0x4C), 0) >> 8);
    {
        Addr mgr = call(sfn::SceneManager_getInstance, {});
        Frame f;
        Addr text = textData(f, 100620036);
        Addr s = newStr(f, textDataStr(text));
        call(g::SceneManager_changeTickerMessage, {mgr, s});
        strFree(s);
        sharedRelease(text);
    }
    self->root = withStr(S_SceneJson, 46, [&](Addr s) { return call(sfn::SceneUtil_createNodeWithSceneFile, {s}); });
    self->book = search(self->root, S_Book, 4);
    self->centerUI = search(self->root, S_CenterUI, 8);
    self->centerUILeft = search(self->root, S_CenterUI_Left, 13);
    self->centerUIRight = search(self->root, S_CenterUI_Right, 14);
    self->switchNode = search(self->root, S_Switch, 6);
    self->tabAccessory = search(self->centerUILeft, S_Tab_Accessorie, 14);
    Addr decision = search(self->root, S_Decision, 8);

    if (self->isCreate) {
        Addr mgr = call(sfn::SceneManager_getInstance, {});
        withEmptyFunction([&](Addr none) { call(g::SceneManager_hideHeader, {mgr, none}); });
        setVisible(self->centerUI, 1);
        setTitle(self->centerUI, S_Txt_Wording, 11, 102500021);
        setVisible(decision, 1);
        Addr mgr2 = call(sfn::SceneManager_getInstance, {});
        withStr(S_BgCircle, 30, [&](Addr s) { call(g::SceneManager_changeBackground, {mgr2, 1, s}); });
        Addr save = search(decision, S_Button_AvatarSave, 17);
        withLambda(VT_init_L3, {me}, [&](Addr f) { call(sfn::Widget_addTouchEventListener, {save, f}); });
        setTitle(save, S_Txt_AvatarSave, 14, 100100001);
    } else {
        self->isTutorialEdit = 1;
        call(g::SceneManager_showHeader, {call(sfn::SceneManager_getInstance, {})});
        vcall(self->book, node::removeFromParent, {self->book});
        Addr mgr = call(sfn::SceneManager_getInstance, {});
        withStr(S_BgDeck, 26, [&](Addr s) { call(g::SceneManager_changeBackground, {mgr, 0, s}); });
        self->book = 0;
        setVisible(self->centerUI, 0);
        setVisible(decision, 1);
        Addr save = search(decision, S_Button_AvatarSave, 17);
        withLambda(VT_init_L4, {me}, [&](Addr f) { call(sfn::Widget_addTouchEventListener, {save, f}); });
        setTitle(save, S_Txt_AvatarSave, 14, 102500025);
        self->showKeyblade = 1;
    }

    // gender switch: the two tap areas
    {
        Addr area = search(self->switchNode, S_Area_Hit_Left, 13);
        Addr button = areaButton(area, false);
        withLambda(VT_init_L5, {me}, [&](Addr f) { call(g::ImageButton_addTouchEventListener, {button, f}); });
    }
    {
        Addr area = search(self->switchNode, S_Area_Hit_Right, 14);
        Addr button = areaButton(area, true);
        withLambda(VT_init_L6, {me}, [&](Addr f) { call(g::ImageButton_addTouchEventListener, {button, f}); });
    }

    // seven tabs: an empty part id list and two "new" flags each
    for (int k = 0; k < 7; k++) {
        Frame f;
        Addr tmp = f.alloc(12);
        st32(tmp, 0);
        st32(tmp + 4, 0);
        st32(tmp + 8, 0);
        uint32_t end = self->partIdsByTab.end;
        if (end < self->partIdsByTab.cap) {
            if (end) {
                st32(end, 0);
                st32(end + 4, 0);
                st32(end + 8, 0);
            }
            self->partIdsByTab.end = end + 12;
        } else {
            call(g::vector_vector_int_push_slow, {me + 0x2F4, tmp});
        }
        call(g::vector_int_dtor, {tmp});
        Addr flag = f.alloc(4);
        st8(flag, 0);
        call(g::vector_bool_push_back, {me + 0x2C8, flag});
        st8(flag, 0);
        call(g::vector_bool_push_back, {me + 0x2D4, flag});
    }
    initAvatarPartsList(self);

    // the parts list in the left page's "Area"
    Addr area = search(self->centerUILeft, S_Area, 4);
    {
        Addr view = call(g::AvatarPartsScrollView_create, {});
        self->scrollView = view;
        uint32_t setSize = ld32(ld32(view) + node::setContentSize);
        Addr size = vcall(area, node::getContentSize, {area});
        call(setSize, {view, size});
    }
    withLambda(VT_init_L7, {me}, [&](Addr f) { call(g::function_vi_swap, {f, self->scrollView + 0x638}); });
    withLambda(VT_init_L8, {me}, [&](Addr f) { call(g::function_vi_swap, {f, self->scrollView + 0x650}); });
    addChild(area, self->scrollView);
    initTabButton(self);
    initMyCordinateButton(self);

    // the avatar preview in place of the right page's "Avatar" placeholder
    Addr avatar = search(self->centerUIRight, S_Avatar, 6);
    self->avatarActor = call(g::AvatarActor_create, {0});
    changeMyCordinate(self, self->coordinateIndex);
    setVisible(self->avatarActor, 1);
    vcall(self->avatarActor, node::setScale, {self->avatarActor, fbits(2.0f)});
    {
        Addr actor = self->avatarActor;
        uint32_t setPosition = ld32(ld32(actor) + node::setPosition);
        float x1 = getPosX(avatar);
        float x2 = getPosX(avatar);
        float x = x1 + x2 * 0.5f;
        float y1 = getPosY(avatar);
        float y2 = getPosY(avatar);
        float y = (y1 - y2 * 0.5f) + -50.0f;
        Frame f;
        Addr pos = f.alloc(8);
        call(sfn::Vec2_ctor, {pos, fbits(x), fbits(y)});
        call(setPosition, {actor, pos});
        call(sfn::Vec2_dtor, {pos});
    }
    call(g::Avatar_preloadResources, {ld32(self->avatarActor + 0x288)});
    {
        Addr parent = vcall(avatar, node::getParent, {avatar});
        uint32_t addChildZ = ld32(ld32(parent) + node::addChildZ);
        Addr actor = self->avatarActor;
        uint32_t z = vcall(avatar, SLOT_getLocalZOrder, {avatar});
        call(addChildZ, {parent, actor, z});
    }
    vcall(avatar, node::removeFromParent, {avatar});

    if (!self->isCreate) {
        // a 100x150 tap area on the avatar
        Addr hit = call(g::Node_create, {});
        uint32_t setSize = ld32(ld32(hit) + node::setContentSize);
        {
            Frame f;
            Addr size = f.alloc(8);
            call(sfn::Size_ctor, {size, fbits(100.0f), fbits(150.0f)});
            call(setSize, {hit, size});
        }
        vcall(hit, node::setAnchorPoint, {hit, ld32(VEC2_ANCHOR_BOTTOM_LEFT_GOT)});
        uint32_t setX = ld32(ld32(hit) + SLOT_setPositionX);
        float w = ldf(vcall(hit, node::getContentSize, {hit}));
        call(setX, {hit, fbits(w * -0.5f)});
        Addr button = newImageButton();
        call(g::ImageButton_setTargetView, {button, hit, fbits(1.0f)});
        addChild(hit, button);
        addChild(self->avatarActor, hit);
        withLambda(VT_init_L9, {me}, [&](Addr f) { call(g::ImageButton_addTouchEventListener, {button, f}); });
    }
    changeTab(self, 0);
    settingDefaultCoordinate(self, &self->defaultCoordinate, self->isMale ? 0 : 1);

    if (!self->isCreate) {
        Addr mgr = call(sfn::SceneManager_getInstance, {});
        withLambda(VT_init_L10, {me}, [&](Addr f) { call(g::SceneManager_setFinishSceneCallback, {mgr, f}); });
    }
    vcall(me, SLOT_onEnterSlot300, {me});
    addChild(me, self->root);
    return 1;
}
RW_FUNCTION(0x703F7C, "SceneAvatarEdit::init", SceneAvatarEdit::init);

void Self::update(SceneAvatarEdit *self, float dt) {
    (void)self;
    (void)dt;
}
RW_FUNCTION(0x706228, "SceneAvatarEdit::update", SceneAvatarEdit::update);

// ------------------------------------------------------------------------------------------------ animations
void Self::runInAnimation(SceneAvatarEdit *self) {
    Addr book = self->book;
    if (book) {
        runEased(book, [&](Addr none) { return call(g::SceneUtil_createFadeInAnimation, {book, none}); });
        Addr center = self->centerUI;
        runEased(center, [&](Addr none) { return call(g::SceneUtil_createFadeInAnimation, {center, none}); });
    }
    Addr left = self->centerUILeft;
    runEased(left, [&](Addr none) { return call(g::SceneUtil_createLeftInAnimation, {left, 0, 0, none}); });
    Addr right = self->centerUIRight;   // the right page slides in from the left too
    runEased(right, [&](Addr none) { return call(g::SceneUtil_createLeftInAnimation, {right, 0, 0, none}); });
}
RW_FUNCTION(0x70622C, "SceneAvatarEdit::runInAnimation", SceneAvatarEdit::runInAnimation);

void Self::runInAnimationThunk(Addr base2) { call(0x70622D, {base2 - 0x288}); }
RW_FUNCTION(0x7063BC, "non-virtual thunk to SceneAvatarEdit::runInAnimation", SceneAvatarEdit::runInAnimationThunk);

void Self::runOutAnimationMain(SceneAvatarEdit *self, Addr done) {
    Addr mgr = call(sfn::SceneManager_getInstance, {});
    withEmptyFunction([&](Addr none) { call(g::SceneManager_hideHeader, {mgr, none}); });
    Addr book = self->book;
    if (book) {
        runEased(book, [&](Addr none) { return call(g::SceneUtil_createFadeOutAnimation, {book, none}); });
        Addr center = self->centerUI;
        runEased(center, [&](Addr none) { return call(g::SceneUtil_createFadeOutAnimation, {center, none}); });
    }
    Addr left = self->centerUILeft;
    runEased(left, [&](Addr none) { return call(g::SceneUtil_createLeftOutAnimation, {left, 0, 0, none}); });
    Addr right = self->centerUIRight;
    Frame f;
    Addr copy = f.alloc(24);
    call(g::function_vv_copy, {copy, done});
    Addr wrapped = f.alloc(24);
    heapFunction(wrapped, VT_runOut_L11, 0x20, 8, g::function_vv_move, copy);
    Addr anim = call(g::SceneUtil_createLeftOutAnimation, {right, 0, 0, wrapped});
    call(sfn::Node_runAction, {right, call(g::EaseSineOut_create, {anim})});
    functionDestroy(wrapped);
    functionDestroy(copy);
}
RW_FUNCTION(0x7063CC, "SceneAvatarEdit::runOutAnimationMain", SceneAvatarEdit::runOutAnimationMain);

void Self::runOutAnimationMainThunk(Addr base2, Addr done) { call(0x7063CD, {base2 - 0x288, done}); }
RW_FUNCTION(0x706600, "non-virtual thunk to SceneAvatarEdit::runOutAnimationMain",
            SceneAvatarEdit::runOutAnimationMainThunk);
