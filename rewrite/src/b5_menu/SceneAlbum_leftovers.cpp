// SceneAlbum leftovers: createScene, the medal tiles' touch lambdas, the popup close button, a std::function
// lambda's clone and the album's sort helpers (std::__sort4 / __sort5 with the album's comparator).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t SceneAlbum_create = 0x5AC9FD;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t FunctionVB_copy = 0x45C84D;             // std::function<void(bool)>(const function&)
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t sort3 = 0x6CF159;                       // std::__sort3<compare&, int*>
constexpr uint32_t compare = 0x6CEFFD;                     // the album order (int&, int&)
}  // namespace g

constexpr uint32_t V_getContentSize = 0xAC;
constexpr uint32_t V_setVisible = 0xB0;
constexpr uint32_t V_isVisible = 0xB4;
constexpr uint32_t V_addChild = 0xF0;

uint32_t createScene() {
    uint32_t scene = call(g::Scene_create, {});
    uint32_t layer = call(g::SceneAlbum_create, {});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}

// sub_6CC99C, a tile's touch began {selected, tile id, node, widget, highlight}: a touch on a tile other than the
// selected one shows its highlight (handled)
uint32_t tileTouchBegan(uint32_t functor, uint32_t touchRef, uint32_t) {
    if (ld32(functor + 4) == ld32(functor + 8)) return 0;
    uint32_t touch = ld32(touchRef);
    uint32_t node = ld32(functor + 0xC);
    uint32_t widget = ld32(functor + 0x10);
    Frame f;
    uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, node, loc});
    call(g::Vec2_dtor, {loc});
    float w = ldf(vcall(widget, V_getContentSize, {widget}));
    float h = ldf(vcall(widget, V_getContentSize, {widget}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, rw::fbits(w), rw::fbits(h)});
    uint32_t inside = call(g::Rect_containsPoint, {rect, p});
    call(g::Vec2_dtor, {rect});
    uint32_t handled = 0;
    if (inside == 1) {
        handled = 1;
        uint32_t highlight = ld32(functor + 0x14);
        vcall(highlight, V_setVisible, {highlight, 1});
    }
    call(g::Vec2_dtor, {p});
    return handled;
}

// sub_6CC818, touch moved {node, widget, highlight}: leaving the tile hides the highlight
void tileTouchMoved(uint32_t functor, uint32_t touchRef, uint32_t) {
    uint32_t touch = ld32(touchRef);
    uint32_t node = ld32(functor + 4);
    uint32_t widget = ld32(functor + 8);
    Frame f;
    uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, node, loc});
    call(g::Vec2_dtor, {loc});
    float w = ldf(vcall(widget, V_getContentSize, {widget}));
    float h = ldf(vcall(widget, V_getContentSize, {widget}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, rw::fbits(w), rw::fbits(h)});
    uint32_t inside = call(g::Rect_containsPoint, {rect, p});
    call(g::Vec2_dtor, {rect});
    if (!inside) {
        uint32_t highlight = ld32(functor + 0xC);
        vcall(highlight, V_setVisible, {highlight, 0});
    }
    call(g::Vec2_dtor, {p});
}

// sub_6CC5C8, touch ended {highlight, scene, tile}: a highlighted tile is chosen (+0x288), the decide sound,
// taps blocked for the popup time, the scene's action ($_ {scene}) after it, the highlight hidden
constexpr uint32_t VT_TILE_ACTION = 0xF67624;
void tileTouchEnded(uint32_t functor, uint32_t, uint32_t) {
    uint32_t highlight = ld32(functor + 4);
    uint32_t scene = ld32(functor + 8);
    if (vcall(highlight, V_isVisible, {highlight}) != 1) return;
    st32(scene + 0x288, ld32(functor + 0xC));
    call(g::AudioPlaySystemSEDecide, {});
    uint32_t mgr = call(g::SceneManager_getInstance, {});
    call(g::SceneManager_openNoTapScreen, {mgr, ld32(call(g::SceneManager_getInstance, {}) + 0x300)});
    uint32_t delay = call(g::DelayTime_create, {ld32(call(g::SceneManager_getInstance, {}) + 0x300)});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_TILE_ACTION, {scene});
        uint32_t done = call(g::CallFunc_create, {fo});
        call(g::Node_runAction, {scene, call(g::Sequence_create, {delay, done, 0})});
        functionDestroy(fo);
    }
    highlight = ld32(functor + 4);
    vcall(highlight, V_setVisible, {highlight, 0});
}

// sub_6CE8A4, the medal popup's close button {popup, a, b}: on release the cancel sound and the close animation
// ($_ {a, b, popup} at its end)
constexpr uint32_t VT_POPUP_CLOSED = 0xF67784;
void popupCloseTouch(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    call(g::AudioPlaySystemSECancel, {});
    uint32_t popup = ld32(functor + 4);
    Frame f;
    uint32_t fo = newFunction(f, VT_POPUP_CLOSED, {ld32(functor + 8), ld32(functor + 0xC), popup});
    uint32_t anim = call(g::SceneUtil_createClosePopupAnimation, {popup, fo});
    call(g::Node_runAction, {popup, anim});
    functionDestroy(fo);
}

// sub_6CC08C / sub_6CC0F0: __clone() and __clone(dst) of a lambda {function<void(bool)>, 16 bytes}
constexpr uint32_t VT_CLONED = 0xF6751C;
void cloneInto(uint32_t functor, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_CLONED);
    call(g::FunctionVB_copy, {dst + 8, functor + 8});
    call(g::MEMCPY, {dst + 0x20, functor + 0x20, 0x10});
}
uint32_t clone(uint32_t functor) {
    uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, VT_CLONED);
        call(g::FunctionVB_copy, {obj + 8, functor + 8});
        call(g::MEMCPY, {obj + 0x20, functor + 0x20, 0x10});
    }
    return obj;
}

// std::__sort4 / __sort5 over int*: insertion of the 4th (5th) element after sorting the first three (four);
// returns the number of swaps
inline bool less(uint32_t a, uint32_t b) { return call(g::compare, {a, b}) == 1; }
inline void swap(uint32_t a, uint32_t b) {
    uint32_t t = ld32(b);
    st32(b, ld32(a));
    st32(a, t);
}
uint32_t sort4(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint32_t r = call(g::sort3, {a, b, c});
    if (!less(d, c)) return r;
    swap(c, d);
    if (!less(c, b)) return r + 1;
    swap(b, c);
    if (!less(b, a)) return r + 2;
    swap(a, b);
    return r + 3;
}
uint32_t sort5(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
    uint32_t r = call(0x6CF1F5, {a, b, c, d});
    if (!less(e, d)) return r;
    swap(d, e);
    if (!less(d, c)) return r + 1;
    swap(c, d);
    if (!less(c, b)) return r + 2;
    swap(b, c);
    if (!less(b, a)) return r + 3;
    swap(a, b);
    return r + 4;
}
// SceneAlbum::openAlbumMedalPopup(index): the medal info popup (MedalInfoFlavorScene_ver106) for the album's
// index-th medal, over a shadow on popup layer 2: its close button hidden, the info centred and filled
// (updateMedalInfo, with the player's setting at user +0x54), the tap screen animation and a screen-wide button
// that closes it ($_ {node, layer, shadow}); then the open animation
constexpr uint32_t VT_POPUP_TAP = 0xF67758;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054, GOT_VEC2_ZERO = 0xFD605C;
void openAlbumMedalPopup(uint32_t self, uint32_t index) {
    constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;
    constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
    constexpr uint32_t SceneManager_getPopupLayer2 = 0x613E71;
    constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
    constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
    constexpr uint32_t SceneUtil_updateMedalInfo = 0x462195;
    constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
    constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;
    constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
    constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
    constexpr uint32_t SafeBuffer_read = 0x45A269;
    constexpr uint32_t AEABI_MEMSET = 0x40E7A0;
    constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance, ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
    constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
    constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
    constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
    constexpr uint32_t shared_add = 0xC8DBB5, shared_release = 0xC8DBD1;
    constexpr uint32_t V_setPosition = 0x4C, V_getPosition = 0x54, V_setAnchorPoint = 0x9C, V_addChildZName = 0xFC;
    constexpr uint32_t V_getAnimation = 0x298, V_play = 0x68;

    uint32_t album = ld32(self + 0x294);
    uint32_t ids = ld32(album + 0x660);
    if ((ld32(album + 0x664) - ids) >> 2 <= index) call(THROW_OUT_OF_RANGE, {});
    uint32_t medalId = ld32(ids + 4 * index);
    call(AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(SceneManager_getPopupLayer2, {call(g::SceneManager_getInstance, {})});
    uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    vcall(layer, V_addChild, {layer, shadow});
    uint32_t node;
    {
        Frame f;
        uint32_t path = newStr(f, 0xE04B8F, 0x33);   // "cocostudio/publish/MedalInfoFlavorScene_ver106.json"
        node = call(SceneUtil_createNodeWithSceneFile, {path});
        strFree(path);
    }
    vcall(layer, V_addChild, {layer, node});
    vcall(node, V_setAnchorPoint, {node, ld32(GOT_ANCHOR_MIDDLE)});
    {
        uint32_t setPos = ld32(ld32(node) + V_setPosition);
        call(setPos, {node, vcall(shadow, V_getPosition, {shadow})});
    }
    uint32_t close = findNode(node, 0xDF8D2F, 12);       // "Close_Button"
    vcall(close, V_setVisible, {close, 0});
    uint32_t outer = findNode(node, 0xDF8D25, 9);        // "MedalInfo"
    uint32_t info = findNode(outer, 0xDF8D25, 9);
    vcall(info, V_setAnchorPoint, {info, ld32(GOT_VEC2_ZERO)});
    {
        uint32_t setPos = ld32(ld32(info) + V_setPosition);
        float w = ldf(vcall(node, V_getContentSize, {node}));
        uint32_t size = vcall(node, V_getContentSize, {node});
        float x = w * 0.5f, y = ldf(size + 4) * 0.5f;
        Frame f;
        uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, rw::fbits(x), rw::fbits(y)});
        call(setPos, {info, pos});
        call(g::Vec2_dtor, {pos});
    }

    // the medal's data: an empty user medal record (the bytes 0x2C..0x30 are never set) for the medal id
    uint32_t mdm = call(MedalDataManager_getInstance, {});
    Frame f;
    uint32_t record = f.alloc(0x48), data = f.alloc(8), copy = f.alloc(8), none = f.alloc(8), flag = f.alloc(4);
    uint32_t sp = f.alloc(0x58);   // the outgoing stack arguments (call_exact passes sp as it is)
    call(AEABI_MEMSET, {record, 0x2C, 0});
    call(AEABI_MEMSET, {record + 0x30, 0x18, 0});
    st32(sp, medalId);
    for (uint32_t k = 0; k < 0x48; k += 4) st32(sp + 8 + k, ld32(record + k));
    rw::call_exact(MedalDataManager_getMedalData, {data, mdm, 0, 0});
    uint32_t api = call(APIManager_getInstance, {});
    st32(copy, ld32(data));
    st32(copy + 4, ld32(data + 4));
    if (ld32(copy + 4)) call(shared_add, {ld32(copy + 4)});
    uint32_t user = ld32(api + 0x94);
    call(SafeBuffer_read, {user, flag, ld32(user + 0xC) + 0x54, 1, 0x54});
    st32(none, 0);
    st32(none + 4, 0);
    st32(sp + 4, 0);
    st32(sp, none);
    rw::call_exact(SceneUtil_updateMedalInfo, {node, copy, ld8(flag) ? 1u : 0u, 0});
    if (ld32(none + 4)) call(shared_release, {ld32(none + 4)});
    if (ld32(copy + 4)) call(shared_release, {ld32(copy + 4)});

    // the tap screen animation
    uint32_t armatures = call(ArmatureDataManager_getInstance, {});
    {
        Frame g2;
        uint32_t s = newStr(g2, 0xDFFCF2, 0x32);         // "cocostudio/publish/Tap_Screen_Animation.ExportJson"
        call(ArmatureDataManager_addArmatureFileInfo, {armatures, s});
        strFree(s);
    }
    uint32_t area = findNode(node, 0xE00317, 15);       // "Tap_Screen_Area"
    uint32_t armature;
    {
        Frame g2;
        uint32_t s = newStr(g2, 0xDFFB2B, 0x14);         // "Tap_Screen_Animation"
        armature = call(Armature_create, {s});
        strFree(s);
    }
    vcall(area, V_addChild, {area, armature});
    uint32_t animation = vcall(armature, V_getAnimation, {armature});
    {
        uint32_t play = ld32(ld32(animation) + V_play);
        Frame g2;
        uint32_t s = newStr(g2, 0xDFD0AF, 10);           // "Animation1"
        call(play, {animation, s, 0xFFFFFFFF, 0xFFFFFFFF});
        strFree(s);
    }
    // a screen-wide button over the shadow closes the popup
    uint32_t button;
    {
        Frame g2;
        uint32_t s = newStr(g2, 0xDF8A1A, 0);
        button = call(ImageButton_create, {s, 0});
        strFree(s);
    }
    call(ImageButton_setTargetView, {button, shadow, rw::fbits(1.0f)});
    {
        uint32_t add = ld32(ld32(node) + V_addChildZName);
        Frame g2;
        uint32_t s = newStr(g2, 0xDFFB40, 12);           // "CenterButton"
        call(add, {node, button, 0, s});
        strFree(s);
    }
    {
        Frame g2;
        uint32_t fo = newFunction(g2, VT_POPUP_TAP, {node, layer, shadow});
        call(ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    {
        Frame g2;
        uint32_t fo = newEmptyFunction(g2);
        uint32_t anim = call(SceneUtil_createOpenPopupAnimation, {node, fo});
        call(g::Node_runAction, {node, anim});
        functionDestroy(fo);
    }
    if (ld32(data + 4)) call(shared_release, {ld32(data + 4)});
}
}  // namespace

RW_FUNCTION(0x6CDF48, "SceneAlbum::openAlbumMedalPopup(int)", openAlbumMedalPopup);
RW_FUNCTION(0x6CCC9C, "SceneAlbum::createScene()", createScene);
RW_FUNCTION(0x6CC99C, "SceneAlbum tile touch began (sub_6CC99C)", tileTouchBegan);
RW_FUNCTION(0x6CC818, "SceneAlbum tile touch moved (sub_6CC818)", tileTouchMoved);
RW_FUNCTION(0x6CC5C8, "SceneAlbum tile touch ended (sub_6CC5C8)", tileTouchEnded);
RW_FUNCTION(0x6CE8A4, "SceneAlbum medal popup close button (sub_6CE8A4)", popupCloseTouch);
RW_FUNCTION(0x6CC08C, "SceneAlbum lambda __clone() (sub_6CC08C)", clone);
RW_FUNCTION(0x6CC0F0, "SceneAlbum lambda __clone(__base*) (sub_6CC0F0)", cloneInto);
RW_FUNCTION(0x6CF1F4, "std::__sort4 (SceneAlbum order) (sub_6CF1F4)", sort4);
RW_FUNCTION(0x6CF26C, "std::__sort5 (SceneAlbum order) (sub_6CF26C)", sort5);
