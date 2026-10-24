// Batch #6's small leftovers: SceneMedalList (createScene, setTouchEnabledAllButton, a back button to the medal
// list, the sale mode toggle), MedalListScrollView (getSelectedMedals, clearSelectedMedals), SceneSaleMedal
// (createScene, backScene2), AvatarPartsScrollView (a closure's destructors, the party info touch),
// AvatarTitleScrollView (touch began / moved of a cell, a lambda's __clone(p)), SceneDrawRaise (share, Facebook
// share, back to the shop) and a StartDeckEditDialog lambda's __clone().
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create, SceneMedalList_create = 0x48A635, SceneSaleMedal_create = 0x48A0ED;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;
constexpr uint32_t FunctionVB_assign = 0x490109;
constexpr uint32_t MedalListClose = 0x489EE1;                   // sub_489EE0 (the closure's close)
constexpr uint32_t MapULLBool_index = 0x4FFAC5, MapULLBool_destroy = 0x5004F1;
constexpr uint32_t VectorU64_pushSlow = 0x4ECB6D, VectorU64_dtor = 0x468499, VectorInt_copy = 0x4AF707;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t SceneUtil_openPartyInfoPopup = 0x46A3E9;
constexpr uint32_t SceneUtil_shareScreenShot = 0x47A559, SceneUtil_openFBSharePhotoPopup = 0x4799D9;
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation, Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t TitleCellTouched = 0x50F7F5;                 // sub_50F7F4
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074;               // __func<Layer*(*)()> vtable
constexpr uint32_t GOT_CREATE_MEDAL_LIST = 0xFD60BC, GOT_CREATE_MIXED_MEDAL = 0xFD68E8;
constexpr uint32_t GOT_CREATE_DECK_EDIT = 0xFD66B0;
}  // namespace g

constexpr uint32_t VT_PartyInfoClosed = 0xF58AF0, VT_PartsClosure = 0xF58AC4;
constexpr uint32_t VT_TitleClosure = 0xF56444, VT_DeckClosure = 0xF594E8;
constexpr uint32_t VT_ShareDone = 0xF651DC, VT_FBShareDone = 0xF65208, VT_ShopBack = 0xF6528C;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C;
constexpr uint32_t SM_SPHERE_FLAG = 0x4CC, SM_SPHERE_DONE = 0x548, SM_SELECTED_MEDAL = 0x2A0;

// changeSceneLayer(function<Layer*()> = a createSceneLayer<T> pointer)
void changeTo(uint32_t createGot, uint32_t a, uint32_t b, uint32_t c) {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(createGot)});
    call(g::SM_changeSceneLayer, {sm, fo, a, b, c});
    functionDestroy(fo);
}

// ---------------------------------------------------------------- SceneMedalList / MedalListScrollView
uint32_t medalListCreateScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(g::SceneMedalList_create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
uint32_t setTouchEnabledAllButton(uint32_t self, uint32_t enabled) {
    vcall(ld32(self + 0x2B8), V_SET_TOUCH_ENABLED, {ld32(self + 0x2B8), enabled});
    return vcall(ld32(self + 0x2BC), V_SET_TOUCH_ENABLED, {ld32(self + 0x2BC), enabled});
}
// a back button {-, closure}(sender, type): ended -> close, then the medal list
uint32_t backToMedalList(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    call(g::AudioPlaySystemSEDecide, {});
    call(g::MedalListClose, {self + 8, 1});
    changeTo(g::GOT_CREATE_MEDAL_LIST, 1, 1, 1);
    return 0;
}
// the sale mode toggle {scene, list}(bool on): the list's sell button shown / hidden and its mode switched
uint32_t saleModeToggle(uint32_t self, uint32_t onp) {
    const uint32_t on = ld8(onp);
    st8(ld32(self + 4) + 0x2CF, uint8_t(on));
    const uint32_t button = ld32(ld32(self + 8) + 0x28C);
    if (!button) return 0;
    uint32_t slot;
    if (on) {
        vcall(button, V_SET_TOUCH_ENABLED, {button, 0});
        vcall(button, node::setVisible, {button, 0});
        slot = 0x330;
    } else {
        vcall(button, node::setVisible, {button, 1});
        vcall(button, V_SET_TOUCH_ENABLED, {button, 1});
        slot = 0x334;
    }
    const uint32_t b = ld32(ld32(self + 8) + 0x28C);
    const uint32_t m = ld32(ld32(b) + slot);
    return call(m, {b, m});
}
// the selected medals (the ids marked in the selection map), in list order
uint32_t getSelectedMedals(uint32_t out, uint32_t self) {
    st32(out, 0);
    st32(out + 4, 0);
    st32(out + 8, 0);
    const uint32_t ids = self + 0x628, map = self + 0x668;
    if (ld32(ids + 4) == ld32(ids)) return ld32(ids);
    uint32_t i = 0;
    do {
        if (ld8(call(g::MapULLBool_index, {map, ld32(ids) + 8 * i}))) {
            if (!(((ld32(ids + 4) - ld32(ids)) >> 3) > i)) {
                call(g::ThrowOutOfRange, {ids});
                return 0;
            }
            const uint32_t id = ld32(ids) + 8 * i;
            const uint32_t end = ld32(out + 4);
            if (end == ld32(out + 8)) {
                call(g::VectorU64_pushSlow, {out, id});
            } else {
                if (end) {
                    st32(end, ld32(id));
                    st32(end + 4, ld32(id + 4));
                }
                st32(out + 4, (end ? ld32(out + 4) : 0) + 8);
            }
        }
        i++;
    } while (i < (ld32(ids + 4) - ld32(ids)) >> 3);
    return ld32(ids);
}
uint32_t clearSelectedMedals(uint32_t self) {
    const uint32_t map = self + 0x668;
    call(g::MapULLBool_destroy, {map, ld32(self + 0x66C)});
    st32(self + 0x670, 0);
    st32(map, self + 0x66C);
    st32(self + 0x66C, 0);
    const uint32_t ids = self + 0x628;
    if (ld32(ids + 4) == ld32(ids)) return ld32(ids);
    uint32_t i = 0;
    do {
        st8(call(g::MapULLBool_index, {map, ld32(ids) + 8 * i}), 0);
        i++;
    } while (i < (ld32(ids + 4) - ld32(ids)) >> 3);
    return ld32(ids);
}

// ---------------------------------------------------------------- SceneSaleMedal
uint32_t saleMedalCreateScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(g::SceneSaleMedal_create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}
// back to where the sale was opened from: 0 deck edit, 1 the medal list (no medal selected), 2 the fusion
uint32_t backScene2(uint32_t self) {
    const uint32_t from = ld32(self + 0x2AC);
    if (from == 2) {
        changeTo(g::GOT_CREATE_MIXED_MEDAL, 1, 0, 0);
    } else if (from == 1) {
        st32(sceneManager() + SM_SELECTED_MEDAL + 4, 0);
        st32(sceneManager() + SM_SELECTED_MEDAL, 0);
        changeTo(g::GOT_CREATE_MEDAL_LIST, 1, 0, 0);
    } else if (from == 0) {
        changeTo(g::GOT_CREATE_DECK_EDIT, 1, 0, 0);
    }
    return 0;
}

// ---------------------------------------------------------------- AvatarPartsScrollView
// a closure {strings +0x20, +0x34, vector<ull> +0x50, string +0x70}: its destructor parts (the last r0 kept)
uint32_t partsDestroyFields(uint32_t self) {
    if (ld8(self + 0x70) & 1) call(fn::operator_delete, {ld32(self + 0x78)});
    call(g::VectorU64_dtor, {self + 0x50});
    if (ld8(self + 0x34) & 1) call(fn::operator_delete, {ld32(self + 0x3C)});
    const uint32_t b = ld8(self + 0x20);
    if (b & 1) return call(fn::operator_delete, {ld32(self + 0x28)});
    return b;
}
uint32_t partsDtor(uint32_t self) {
    st32(self, VT_PartsClosure);
    partsDestroyFields(self);
    return self;
}
uint32_t partsDelete(uint32_t self) {
    st32(self, VT_PartsClosure);
    partsDestroyFields(self);
    return call(fn::operator_delete, {self});
}
uint32_t partsDestroy(uint32_t self) { return partsDestroyFields(self); }
// a party member cell {-, view, node, id lo, id hi}(sender, type): ended (when allowed, visible and not open
// yet) -> the party info popup
uint32_t partyInfoTouch(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    const uint32_t view = ld32(self + 8);
    if (!ld8(view + 0x661)) return 0;
    const uint32_t node = ld32(self + 0xC);
    if (vcall(node, 0xB4, {node}) != 1) return 0;
    if (ld8(view + 0x660)) return 0;
    st8(view + 0x660, 1);
    Frame f;
    const uint32_t fo = newFunction(f, VT_PartyInfoClosed, {view});
    call(g::SceneUtil_openPartyInfoPopup, {ld32(self + 0x10), ld32(self + 0x14), 0, fo});
    functionDestroy(fo);
    return 0;
}

// ---------------------------------------------------------------- AvatarTitleScrollView
// the cell's hit rect: (width / 2, 0, width, height) in the cell's space
bool cellHit(uint32_t cell, uint32_t touch) {
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, cell, loc});
    call(g::Vec2_dtor, {loc});
    const float x = ldf(vcall(cell, node::getContentSize, {cell})) * 0.5f;
    const uint32_t w = ld32(vcall(cell, node::getContentSize, {cell}));
    const uint32_t h = ld32(vcall(cell, node::getContentSize, {cell}) + 4);
    call(g::Rect_ctor, {rect, fbits(x), 0, w, h});
    const bool hit = call(g::Rect_containsPoint, {rect, p}) != 0;
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {p});
    return hit;
}
// touch moved {scroll, index, ...}: a pressed cell left -> released (the ended handler at +0xC)
uint32_t titleTouchMoved(uint32_t self, uint32_t touchp) {
    const uint32_t touch = ld32(touchp);
    const uint32_t scroll = ld32(self + 4);
    if (!ld8(scroll + 0x6B5)) return 0;
    const uint32_t cell = ld32(ld32(scroll + 0x51C) + 4 * ld32(self + 8));
    if (!cellHit(cell, touch)) {
        st8(scroll + 0x6B5, 0);
        call(g::TitleCellTouched, {self + 0xC, touch});
    }
    return 0;
}
// touch began {scroll, index}: (not while scrolling) remember the y, pressed when on the cell
uint32_t titleTouchBegan(uint32_t self, uint32_t touchp) {
    const uint32_t touch = ld32(touchp);
    const uint32_t scroll = ld32(self + 4);
    if (ld8(scroll + 0x6BC)) return 0;
    {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        st32(scroll + 0x6C0, ld32(loc + 4));
        call(g::Vec2_dtor, {loc});
    }
    const uint32_t cell = ld32(ld32(scroll + 0x51C) + 4 * ld32(self + 8));
    if (!cellHit(cell, touch)) return 0;
    st8(scroll + 0x6B5, 1);
    return 1;
}
// a lambda {2 words, 7 words, string, 9 bytes, 2 words} __clone(p)
uint32_t titleCloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    st32(dst, VT_TitleClosure);
    st32(dst + 0xC, ld32(self + 0xC));
    st32(dst + 8, ld32(self + 8));
    for (uint32_t k = 0x10; k < 0x2C; k += 4) st32(dst + k, ld32(self + k));
    if (ld8(self + 0x2C) & 1) {
        call(fn::string_init, {dst + 0x2C, ld32(self + 0x34), ld32(self + 0x30)});
    } else {
        for (uint32_t k = 0; k < 12; k += 4) st32(dst + 0x2C + k, ld32(self + 0x2C + k));
    }
    call(g::MEMCPY, {dst + 0x38, self + 0x38, 9});
    const uint32_t r = ld32(self + 0x48);
    st32(dst + 0x48, r);
    st32(dst + 0x4C, ld32(self + 0x4C));
    return r;
}

// ---------------------------------------------------------------- SceneDrawRaise
// the share button {scene, button}(sender, type): ended (when sharing is allowed) -> share a screenshot
uint32_t shareButton(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    const uint32_t scene = ld32(self + 4);
    if (!ld8(scene + 0x2AE)) return 0;
    Frame f;
    const uint32_t fo = newFunction(f, VT_ShareDone, {scene});
    call(g::SceneUtil_shareScreenShot, {ld32(self + 8), fo});
    functionDestroy(fo);
    return 0;
}
// the screenshot taken {scene}(path, image): the Facebook photo share popup
uint32_t fbShare(uint32_t self, uint32_t pathp, uint32_t image) {
    const uint32_t path = ld32(pathp);
    const uint32_t me = ld32(self + 4);
    Frame f;
    const uint32_t fo = newFunction(f, VT_FBShareDone, {});
    const uint32_t args = f.alloc(16);
    st32(args, me + 0x288);
    st32(args + 4, image);
    st32(args + 8, path);
    st32(args + 0xC, fo);
    rw::call_exact(g::SceneUtil_openFBSharePhotoPopup, {me + 0x2BC, me + 0x2B0, me + 0x2B4, me + 0x2B8});
    functionDestroy(fo);
    return 0;
}
// back to the shop: no sphere callback pending, the shop API, then its layer
uint32_t backToShop() {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    st8(sm + SM_SPHERE_FLAG, 1);
    call(g::FunctionVB_assign, {sm + SM_SPHERE_DONE, none});
    functionDestroy(none);
    {
        Frame t;
        const uint32_t fo = newEmptyFunction(t);
        call(g::SceneShopTop_callShopAPI, {fo});
        functionDestroy(fo);
    }
    const uint32_t sm2 = sceneManager();
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_ShopBack, {});
        call(g::SM_changeSceneLayer, {sm2, fo, 1, 1, 1});
        functionDestroy(fo);
    }
    return 0;
}

// ---------------------------------------------------------------- StartDeckEditDialog
// a lambda {9 words, 3 vector<int>} __clone()
uint32_t deckClone(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0x4C});
    if (c) {
        st32(c, VT_DeckClosure);
        for (uint32_t k = 4; k < 0x28; k += 4) st32(c + k, ld32(self + k));
        call(g::VectorInt_copy, {c + 0x28, self + 0x28});
        call(g::VectorInt_copy, {c + 0x34, self + 0x34});
        call(g::VectorInt_copy, {c + 0x40, self + 0x40});
    }
    return c;
}
}  // namespace

RW_FUNCTION(0x73CCCC, "SceneMedalList::createScene()", medalListCreateScene);
RW_FUNCTION(0x73DC70, "SceneMedalList::setTouchEnabledAllButton(bool)", setTouchEnabledAllButton);
RW_FUNCTION(0x48A520, "SceneMedalList back button (sub_48A520)", backToMedalList);
RW_FUNCTION(0x73C9E0, "SceneMedalList sale mode toggle (sub_73C9E0)", saleModeToggle);
RW_FUNCTION(0x59B190, "MedalListScrollView::getSelectedMedals()", getSelectedMedals);
RW_FUNCTION(0x59B240, "MedalListScrollView::clearSelectedMedals()", clearSelectedMedals);
RW_FUNCTION(0x65B40C, "SceneSaleMedal::createScene()", saleMedalCreateScene);
RW_FUNCTION(0x65D7B0, "SceneSaleMedal::backScene2()", backScene2);
RW_FUNCTION(0x553110, "AvatarPartsScrollView closure __func::~__func (sub_553110)", partsDtor);
RW_FUNCTION(0x553168, "AvatarPartsScrollView closure __func::~__func (deleting) (sub_553168)", partsDelete);
RW_FUNCTION(0x553274, "AvatarPartsScrollView closure __func::destroy (sub_553274)", partsDestroy);
RW_FUNCTION(0x5532F8, "AvatarPartsScrollView party info touch (sub_5532F8)", partyInfoTouch);
RW_FUNCTION(0x50FB38, "AvatarTitleScrollView cell touch moved (sub_50FB38)", titleTouchMoved);
RW_FUNCTION(0x50FCF8, "AvatarTitleScrollView cell touch began (sub_50FCF8)", titleTouchBegan);
RW_FUNCTION(0x50FF84, "AvatarTitleScrollView lambda __clone(p) (sub_50FF84)", titleCloneInto);
RW_FUNCTION(0x693790, "SceneDrawRaise share button (sub_693790)", shareButton);
RW_FUNCTION(0x6938D4, "SceneDrawRaise Facebook share (sub_6938D4)", fbShare);
RW_FUNCTION(0x693F74, "SceneDrawRaise back to the shop (sub_693F74)", backToShop);
RW_FUNCTION(0x5729F4, "StartDeckEditDialog lambda __clone() (sub_5729F4)", deckClone);
