// DeckRevolverView: life cycle, medal slots, the drag / swap of medals and the touch lambdas.
#include "DeckRevolverView.hpp"

#include "../core/SafeBuffer.hpp"

using namespace w4b1;
using Self = DeckRevolverView;

namespace {
// lambda vtables (__func + 8)
constexpr uint32_t VT_init_1 = 0xF57364;           // onTouchBegan
constexpr uint32_t VT_init_2 = 0xF57338;           // onTouchMoved
constexpr uint32_t VT_init_3 = 0xF5725C;           // onTouchEnded
constexpr uint32_t VT_init_4 = 0xF57204;           // onTouchCancelled
constexpr uint32_t VT_init_5 = 0xF571D8;           // onEnter callback
constexpr uint32_t VT_init_6 = 0xF571AC;           // onExit callback
constexpr uint32_t VT_init_4_back = 0xF57230;      // $_4::{lambda()#1}: resetMoveMedal
constexpr uint32_t VT_init_3_swap = 0xF5730C;      // $_3::{lambda()#1} (heap: self, index, node, Vec2)
constexpr uint32_t VT_init_3_drop = 0xF572B4;      // $_3::{lambda()#2} (self, scale, opacity)
constexpr uint32_t VT_init_3_back = 0xF57288;      // $_3::{lambda()#3} (self)
constexpr uint32_t VT_init_3_button = 0xF572E0;    // the medal info popup's button callback
constexpr uint32_t VT_setMedalData_7 = 0xF57180;   // setMedalData::$_7 (heap, 0x30 bytes)

constexpr uint32_t MEDAL_MANAGER = 0x1003BC0;      // static DeckMedalViewManager* (created on first use)
constexpr uint32_t STYLE_CAROUSEL_NAME = 0xFEF0B0; // CarouselDeck::DECK_STYLE_RES_CAROUSEL_NAME (const char*)
constexpr uint32_t STYLE_BUTTON_NAME = 0xFEF0B4;   // CarouselDeck::DECK_STYLE_RES_BUTTON_NAME

// strings
constexpr uint32_t S_MEDAL = 0xDFD05F;             // "Medal"
constexpr uint32_t S_SUPPORT_LWF = 0xDF9023;       // "supportLWF"
constexpr uint32_t S_REVOLVER = 0xDFE17A;          // "Revolver"
constexpr uint32_t S_REVOLVER_CENTER = 0xDFE183;   // "Revolver_Center"
constexpr uint32_t S_TYPE_MATCH_EFFECT = 0xDFE0FE; // "TypeMatchEffect"
constexpr uint32_t S_AREA_FMT = 0xDFE172;          // "Area_%d"
constexpr uint32_t S_REVOLVER_JSON = 0xDFE148;     // "cocostudio/publish/DeckEdit_Revolver.json"

// guest functions
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Node_init = rwlib::cocos2d::Node::init;
constexpr uint32_t Node_convertToWorldSpace = rwlib::cocos2d::Node::convertToWorldSpace;   // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t ScaleTo_create = rwlib::cocos2d::ScaleTo::create__ff;
constexpr uint32_t FadeOut_create = rwlib::cocos2d::FadeOut::create;
constexpr uint32_t Spawn_create = rwlib::cocos2d::Spawn::create;
constexpr uint32_t ListenerTouch_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t ListenerTouch_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addSceneGraph = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;          // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVU64_call = 0x4FF3C9;          // std::function<void(uint64)>::operator()
constexpr uint32_t FunctionButton_call = 0x52F8C1;        // std::function<void(MedalInfoButtonType, bool, uint64)>
constexpr uint32_t DeckMedalView_create = 0x52BA71;
constexpr uint32_t DeckMedalView_setData = 0x52BEE5;
constexpr uint32_t DeckMedalView_updateLayout = 0x5276D9;
constexpr uint32_t MedalViewVector_pushBack = 0x527565;   // cocos2d::Vector<DeckMedalView*>::pushBack
constexpr uint32_t MedalViewVector_eraseObject = 0x5275D9;
constexpr uint32_t MedalViewPtrVector_pushBackSlow = 0x5306D5;
constexpr uint32_t MedalViewPtrVector_baseDtor = 0x52C439;
constexpr uint32_t IntVector_pushBackSlowMove = 0x47D709; // std::vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t MedalVector_baseDtor = 0x46B119;
constexpr uint32_t MedalVector_assign = 0x52E481;
constexpr uint32_t MedalVector_insert = 0x52DE2D;         // insert(const_iterator, const value_type&)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D;
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;
constexpr uint32_t SceneUtil_showSupportMedalEffect = 0x46695D;
constexpr uint32_t SceneUtil_openMedalInfoPopup = 0x466EF5;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioManager_getInstance = 0x5BECFD;
constexpr uint32_t AudioManager_getLayer = 0x5BF33D;      // (shared_ptr<Layer> out, mgr, int)
constexpr uint32_t AudioLayer_playSE = 0x5BF785;          // (layer, ResourceID, int, int)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t Keyblade_create = 0x783F9D;            // master::Keyblade::create(shared_ptr out, int)

// Node virtual slots (byte offsets) used here
constexpr uint32_t VS_setLocalZOrder = 0x0C, VS_setScale = 0x40, VS_getScale = 0x44, VS_setPosition = 0x4C;
constexpr uint32_t VS_getPosition = 0x54, VS_getPositionX = 0x68, VS_getPositionY = 0x70, VS_setAnchorPoint = 0x9C;
constexpr uint32_t VS_setContentSize = 0xA8, VS_getContentSize = 0xAC, VS_setVisible = 0xB0, VS_isVisible = 0xB4;
constexpr uint32_t VS_addChild = 0xF0, VS_getParent = 0x11C, VS_removeFromParent = 0x124, VS_removeChild = 0x12C;
constexpr uint32_t VS_getOpacity = 0x20C, VS_setOpacity = 0x214, VS_setTouchEnabled = 0x29C;

// DeckMedalView fields
constexpr uint32_t DMV_SHOW_TYPE = 0x350, DMV_MAGNIFICATION = 0x370, DMV_SHOW_MIX_INFO = 0x389;
constexpr uint32_t DMV_SHOW_MEDAL_INFO = 0x38A, DMV_INFO_PANEL = 0x390, DMV_DARKLIGHT_MAG = 0x394;

// keyblade master data (SafeBuffer at UserKeyBladeData + 0x20) positions
constexpr uint32_t KB_STYLE = 0xD0, KB_OPEN_SLOTS = 0xDC, KB_NEXT_KEYBLADE = 0x13C, KB_OPEN_LEVEL = 0xC8;
constexpr uint32_t KB_SLOT_A = 0xE8, KB_SLOT_B = 0xFC, KB_SLOT_C = 0x110, KB_SLOT_D = 0x124;

inline Self *S(uint32_t a) { return rw::host_ptr<Self>(a); }
inline uint32_t A(const uint8_t *p) { return rw::guest_addr(p); }
inline void addRef(uint32_t ctrl) { if (ctrl) guest::addRef(ctrl); }
inline void release(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }
inline uint8_t safe8(uint32_t sb, uint32_t index) {
    const SafeBuffer *s = scene::safe(sb);
    return uint8_t(s->data[index] ^ SafeBuffer::mask(s->key, index));
}
// the keyblade's master SafeBuffer
inline uint32_t keybladeData(Self *s) { return ld32(s->keyblade + 0x20); }
inline uint32_t medalAt(Self *s, uint32_t k) { return ld32(s->medals.begin + k * 8); }
inline uint32_t medalCount(Self *s) { return uint32_t(int32_t(s->medals.end - s->medals.begin) >> 3); }
inline uint32_t viewCount(Self *s) { return uint32_t(int32_t(s->medalViews.end - s->medalViews.begin) >> 2); }
inline uint32_t viewAt(Self *s, uint32_t k) { return ld32(s->medalViews.begin + k * 4); }

// SceneUtil::searchNodeByName(root, "<literal>")
inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }

// The DeckMedalViewManager singleton as the inlined getInstance makes it (0x24 zeroed bytes)
uint32_t medalManager() {
    uint32_t m = ld32(MEDAL_MANAGER);
    if (!m) {
        m = call(fn::operator_new, {0x24});
        call(wl::MEMSET, {m, 0x24, 0});
        st32(MEDAL_MANAGER, m);
    }
    return m;
}

// The support medal's id (APIManager + 0x114: a SafeBuffer, id at 0x58) equals the medal's: show its effect
void supportEffectIfSupport(uint32_t medalObj, uint32_t widget) {
    uint32_t api = call(APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + 0x118);
    uint32_t sb = ld32(api + 0x114);
    addRef(ctrl);
    uint32_t lo = ld32(medalObj), hi = ld32(medalObj + 4);
    if (uint32_t(safe32(sb, 0x58)) == lo && uint32_t(safe32(sb, 0x5C)) == hi)
        call(SceneUtil_showSupportMedalEffect, {ld32(widget), 0});
    release(ctrl);
}

// libc++ vector<shared_ptr<T>>::erase(begin() + index), inlined: move the tail down one, destroy the last
void eraseMedal(uint32_t vec, uint32_t index) {
    uint32_t begin = ld32(vec);
    uint32_t pos = begin + index * 8;
    uint32_t next = begin + (uint32_t(int32_t(index * 8) >> 3) + 1) * 8;
    uint32_t end = ld32(vec + 4);
    if (next != end) {
        uint32_t last = end - 8;
        uint32_t p = pos;
        do {
            uint32_t ctrl = ld32(p + 12), obj = ld32(p + 8);
            st32(p + 8, 0);
            st32(p + 12, 0);
            st32(p, obj);
            uint32_t old = ld32(p + 4);
            st32(p + 4, ctrl);
            release(old);
            p += 8;
        } while (last != p);
        pos = begin + (((last - next) >> 3) + uint32_t(int32_t(index * 8) >> 3) + 1) * 8;
        end = ld32(vec + 4);
    }
    while (end != pos) {
        st32(vec + 4, end - 8);
        uint32_t ctrl = ld32(end - 4);
        if (ctrl) {
            guest::release(ctrl);
        }
        end = ld32(vec + 4);
    }
}

// a std::function<void()> on the stack holding an inline lambda {vtable, captures}; CallFunc::create(it)
uint32_t makeCallFunc(Frame &f, uint32_t vtable, std::initializer_list<uint32_t> caps) {
    uint32_t fo = newFunction(f, vtable, caps);
    uint32_t cf = call(wl::CallFunc_create, {fo});
    functionDestroy(fo);
    return cf;
}

// node->runAction(Sequence(MoveTo(0.15, pos), then))
void moveBack(uint32_t node, uint32_t pos, uint32_t then) {
    uint32_t mv = call(wl::MoveTo_create, {fb(0.15f), pos});
    runSequence2(node, mv, then);
}

// touch->getLocation() into a stack Vec2
uint32_t touchLoc(Frame &f, uint32_t touch) {
    uint32_t v = f.alloc(8);
    call(Touch_getLocation, {v, touch});
    return v;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
DeckRevolverView::Addr DeckRevolverView::construct(Addr self) {
    call(Node_ctor, {self});
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    rw::set<uint32_t>(s, 0x280, 0);
    rw::set<uint32_t>(s, 0x298, 0);
    rw::set<uint32_t>(s, 0x2B0, 0);
    call(wl::MEMSET, {self + 0x2B8, 0x30, 0});
    call(wl::Vec2_ctor, {self + 0x2E8});
    call(wl::Vec2_ctor, {self + 0x2F0});
    call(wl::Vec2_ctor, {self + 0x2F8});
    s->touchDone = 0;
    s->moved = 0;
    s->popupOpened = 0;
    s->field_304 = 0;
    s->moveScale = 1.0f;
    s->moveMedalEnabled = 0;
    s->openMedalInfoPopupEnabled = 0;
    s->showType = 0;
    s->field_314 = 0;
    s->medalViews.begin = 0;
    s->medalViews.end = 0;
    s->medalViews.cap = 0;
    call(wl::MEMSET, {self + 0x318, 0x11, 0});
    return self;
}
RW_FUNCTION(0x52C55C, "DeckRevolverView::DeckRevolverView", DeckRevolverView::construct);

DeckRevolverView::Addr DeckRevolverView::create(Addr keyblade, Addr detail) {
    uint32_t self = call(fn::operator_new, {SIZE});
    construct(self);
    Frame f;
    uint32_t kb = f.alloc(8), dt = f.alloc(8);
    uint32_t kbCtrl = ld32(keyblade + 4);
    st32(kb, ld32(keyblade));
    st32(kb + 4, kbCtrl);
    addRef(kbCtrl);
    st32(dt, ld32(detail));
    st32(dt + 4, ld32(detail + 4));
    addRef(ld32(dt + 4));
    uint32_t ok = init(self, kb, dt);
    release(ld32(dt + 4));
    release(kbCtrl);
    if (ok == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, vslot::release, {self});
    return 0;
}
RW_FUNCTION(0x52C470, "DeckRevolverView::create", DeckRevolverView::create);

DeckRevolverView::Addr DeckRevolverView::destroy(Addr self) {
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    uint32_t views = self + 0x32C;
    if (s->medalViews.end != s->medalViews.begin) {
        uint32_t k = 0;
        do {
            uint32_t mgr = medalManager();
            if (!(viewCount(s) > k)) {
                call(wl::VECTOR_AT_THROW, {views});
                return self;
            }
            call(MedalViewVector_eraseObject, {mgr + 8, viewAt(s, k), 0});
            k++;
        } while (k < viewCount(s));
    }
    call(MedalViewPtrVector_baseDtor, {views});
    release(s->detailCtl);
    call(wl::Vec2_dtor, {self + 0x2F8});
    call(wl::Vec2_dtor, {self + 0x2F0});
    call(wl::Vec2_dtor, {self + 0x2E8});
    call(wl::IntVector_baseDtor, {self + 0x2CC});
    call(MedalVector_baseDtor, {self + 0x2C0});
    release(s->keybladeCtl);
    functionDestroy(A(s->onMedalChanged));
    functionDestroy(A(s->onSwapMedal));
    functionDestroy(A(s->onChangeMedal));
    call(Node_dtor, {self});
    return self;
}
RW_FUNCTION(0x52C1F4, "DeckRevolverView::~DeckRevolverView", DeckRevolverView::destroy);

// Loads the revolver layout, puts a DeckMedalView in each of Area_1..Area_5 and hooks the touch handlers.
uint32_t DeckRevolverView::init(Addr self, Addr keyblade, Addr detail) {
    if (call(Node_init, {self}) != 1) return 0;
    Self *s = S(self);
    // the owner of the medals: the given user, else the player (APIManager + 0x94)
    uint32_t obj = ld32(detail);
    if (!obj) {
        uint32_t api = call(APIManager_getInstance, {});
        uint32_t ctrl = ld32(api + 0x98);
        obj = ld32(api + 0x94);
        addRef(ctrl);
        st32(detail, obj);
        uint32_t old = ld32(detail + 4);
        st32(detail + 4, ctrl);
        if (old) {
            guest::release(old);
            obj = ld32(detail);
        }
    }
    uint32_t ctrl = ld32(detail + 4);
    addRef(ctrl);
    s->detail = obj;
    uint32_t old = s->detailCtl;
    s->detailCtl = ctrl;
    release(old);

    vcall(self, VS_setAnchorPoint, {self, scene::VEC2_ANCHOR_MIDDLE});
    uint32_t widget = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_REVOLVER_JSON});
    vcall(widget, VS_setTouchEnabled, {widget, 0});
    vcall(self, VS_setContentSize, {self, vcall(widget, VS_getContentSize, {widget})});
    {
        float x = bf(vcall(widget, VS_getPositionX, {widget})) * -0.5f;
        float y = bf(vcall(widget, VS_getPositionY, {widget})) * -0.5f;
        Frame f;
        uint32_t pos = f.alloc(8);
        call(wl::Vec2_ctorXY, {pos, fb(x), fb(y)});
        vcall(widget, VS_setPosition, {widget, pos});
        call(wl::Vec2_dtor, {pos});
    }
    vcall(self, VS_addChild, {self, widget});

    uint32_t views = self + 0x32C, levels = self + 0x2CC;
    int32_t k = 0;
    do {
        k++;
        uint32_t area;
        {
            Frame f;
            uint32_t buf = stringBuffer(f, {S_AREA_FMT, uint32_t(k)});
            uint32_t name = newStr(f, buf);
            area = SceneUtil::searchNodeByName(widget, name);
            strFree(name);
        }
        uint32_t view = call(DeckMedalView_create, {0});
        Frame f;
        uint32_t tmp = f.alloc(8);
        st32(tmp, view);
        vcall(area, VS_addChild, {area, view});
        uint32_t end = s->medalViews.end;
        if (end == s->medalViews.cap) {
            call(MedalViewPtrVector_pushBackSlow, {views, tmp});
        } else {
            uint32_t v = 0;
            if (end) {
                st32(end, view);
                v = s->medalViews.end;
            }
            s->medalViews.end = v + 4;
        }
        st32(tmp + 4, 0);
        uint32_t lend = s->slotOpenLevels.end;
        if (lend >= s->slotOpenLevels.cap) {
            call(IntVector_pushBackSlowMove, {levels, tmp + 4});
        } else {
            uint32_t v = 0;
            if (lend) {
                st32(lend, 0);
                v = s->slotOpenLevels.end;
            }
            s->slotOpenLevels.end = v + 4;
        }
        call(MedalViewVector_pushBack, {medalManager() + 8, view});
    } while (k < 5);

    {
        Frame f;
        uint32_t kb = f.alloc(8);
        uint32_t kbCtrl = ld32(keyblade + 4);
        st32(kb, ld32(keyblade));
        st32(kb + 4, kbCtrl);
        addRef(kbCtrl);
        setKeyBladeData(self, kb);
        release(kbCtrl);
    }
    uint32_t listener = call(ListenerTouch_create, {});
    call(ListenerTouch_setSwallowTouches, {listener, 0});
    const uint32_t vts[4] = {VT_init_1, VT_init_2, VT_init_3, VT_init_4};
    for (uint32_t k2 = 0; k2 < 4; k2++) {
        Frame f;
        uint32_t fo = newFunction(f, vts[k2], {self});
        call(k2 == 0 ? wl::FunctionTouchB_swap : wl::FunctionTouchV_swap, {fo, listener + 0x50 + k2 * 0x18});
        functionDestroy(fo);
    }
    call(EventDispatcher_addSceneGraph, {ld32(director() + smain::lib::Director_eventDispatcher), listener, self});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_5, {self});
        call(FunctionVV_assign, {self + 0x210, fo});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_6, {self});
        call(FunctionVV_assign, {self + 0x228, fo});
        functionDestroy(fo);
    }
    return 1;
}
RW_FUNCTION(0x52C6EC, "DeckRevolverView::init", DeckRevolverView::init);

// The keyblade: its revolver / centre images by deck style, the slot open levels; type-match effects removed.
void DeckRevolverView::setKeyBladeData(Addr self, Addr keyblade) {
    Self *s = S(self);
    uint32_t obj = ld32(keyblade), ctrl = ld32(keyblade + 4);
    addRef(ctrl);
    s->keyblade = obj;
    uint32_t old = s->keybladeCtl;
    s->keybladeCtl = ctrl;
    release(old);
    uint32_t revolver = find(self, S_REVOLVER, 8);
    uint32_t center = find(revolver, S_REVOLVER_CENTER, 15);
    int32_t style = safe32(keybladeData(s), KB_STYLE);
    {
        Frame f;
        uint32_t buf = stringBuffer(f, {ld32(STYLE_CAROUSEL_NAME), uint32_t(style)});
        uint32_t name = newStr(f, buf);
        call(ImageView_loadTexture, {revolver, name, 0});
        strFree(name);
    }
    {
        Frame f;
        uint32_t buf = stringBuffer(f, {ld32(STYLE_BUTTON_NAME), uint32_t(style)});
        uint32_t name = newStr(f, buf);
        call(ImageView_loadTexture, {center, name, 0});
        strFree(name);
    }
    {
        Frame f;
        uint32_t kb = f.alloc(8);
        uint32_t kbCtrl = ld32(keyblade + 4);
        st32(kb, ld32(keyblade));
        st32(kb + 4, kbCtrl);
        addRef(kbCtrl);
        settingSlotOpenLevels(kb, self + 0x2CC);
        release(kbCtrl);
    }
    uint32_t end = s->medalViews.end;
    for (uint32_t p = s->medalViews.begin; p != end; p += 4) {
        uint32_t effect = find(ld32(p), S_TYPE_MATCH_EFFECT, 15);
        if (effect) vcall(effect, VS_removeFromParent, {effect});
    }
}
RW_FUNCTION(0x52CC20, "DeckRevolverView::setKeyBladeData", DeckRevolverView::setKeyBladeData);

// Puts a dragged medal back into its slot (when dragging was on) and forgets the drag.
void DeckRevolverView::resetMoveMedal(Addr self) {
    Self *s = S(self);
    s->field_304 = 0;
    s->moved = 0;
    s->touchDone = 0;
    if (s->moveMedalEnabled) {
        call(Ref_retain, {s->moveMedal});
        uint32_t m = s->moveMedal;
        uint32_t parent = vcall(m, VS_getParent, {m});
        vcall(parent, VS_removeChild, {parent, s->moveMedal, 1});
        uint32_t home = s->moveParent;
        vcall(home, VS_addChild, {home, s->moveMedal});
        m = s->moveMedal;
        vcall(m, VS_setScale, {m, rw::get<uint32_t>(s, 0x308)});
        m = s->moveMedal;
        vcall(m, VS_setLocalZOrder, {m, 0});
        m = s->moveMedal;
        vcall(m, VS_setPosition, {m, self + 0x2F0});
        call(Ref_release, {s->moveMedal});
        uint32_t panel = ld32(s->moveView + DMV_INFO_PANEL);
        vcall(panel, VS_setVisible, {panel, 1});
        uint32_t lwf = find(s->moveMedal, S_SUPPORT_LWF, 10);
        if (lwf) {
            m = s->moveMedal;
            vcall(m, VS_removeChild, {m, lwf, 1});
        }
        uint32_t idx = uint32_t(s->moveIndex.get());
        if (!(medalCount(s) > idx)) {
            call(wl::VECTOR_AT_THROW, {self + 0x2C0});
            return;
        }
        uint32_t medal = s->medals.begin + idx * 8;
        uint32_t medalObj = ld32(medal), medalCtrl = ld32(medal + 4);
        addRef(medalCtrl);
        supportEffectIfSupport(medalObj, self + 0x2DC);
        release(medalCtrl);
    }
    s->moveMedal = 0;
    s->moveParent = 0;
    s->moveView = 0;
}
RW_FUNCTION(0x52CEF0, "DeckRevolverView::resetMoveMedal", DeckRevolverView::resetMoveMedal);

// Shows the medals in the five slots (empty slots past the keyblade's open slot count are shown locked).
void DeckRevolverView::setMedalDatas(Addr self, Addr medals) {
    Self *s = S(self);
    if (self + 0x2C0 != medals) call(MedalVector_assign, {self + 0x2C0, ld32(medals), ld32(medals + 4)});
    int32_t openSlots = safe32(keybladeData(s), KB_OPEN_SLOTS);
    for (uint32_t k = 0; int32_t(k) < 5; k++) {
        if (!(viewCount(s) > k)) {
            call(wl::VECTOR_AT_THROW, {self + 0x32C});
            return;
        }
        uint32_t view = viewAt(s, k);
        rw::save<int32_t>(view + DMV_SHOW_TYPE, s->showType);
        uint32_t obj = 0, ctrl = 0;
        if (k < medalCount(s)) {
            uint32_t m = s->medals.begin + k * 8;
            obj = ld32(m);
            ctrl = ld32(m + 4);
            addRef(ctrl);
        }
        Frame f;
        uint32_t medal = f.alloc(8), owner = f.alloc(8);
        if (int32_t(k) < openSlots) {
            st32(medal, obj);
            st32(medal + 4, ctrl);
            addRef(ctrl);
            uint32_t kb = keybladeData(s);
            int32_t a = safe32(kb, KB_SLOT_A + k * 4);
            kb = keybladeData(s);
            int32_t b = safe32(kb, KB_SLOT_B + k * 4);
            kb = keybladeData(s);
            int32_t c = safe32(kb, KB_SLOT_C + k * 4);
            kb = keybladeData(s);
            int32_t d = safe32(kb, KB_SLOT_D + k * 4);
            uint32_t level = ld32(s->slotOpenLevels.begin + k * 4);
            uint32_t skills = s->skills;
            uint32_t dCtrl = s->detailCtl;
            st32(owner, s->detail);
            st32(owner + 4, dCtrl);
            addRef(dCtrl);
            call(DeckMedalView_setData, {view, medal, k, uint32_t(a), uint32_t(b), uint32_t(c), uint32_t(d), 0, level,
                                         skills, owner});
            release(dCtrl);
            if (ctrl) {
                guest::release(ctrl);
                guest::release(ctrl);
            }
        } else {
            st32(medal, 0);
            st32(medal + 4, 0);
            uint32_t level = ld32(s->slotOpenLevels.begin + k * 4);
            uint32_t skills = s->skills;
            uint32_t dCtrl = s->detailCtl;
            st32(owner, s->detail);
            st32(owner + 4, dCtrl);
            addRef(dCtrl);
            call(DeckMedalView_setData, {view, medal, 0xFFFFFFFFu, 1, 1, 1000, 1000, 0, level, skills, owner});
            release(dCtrl);
            release(ctrl);
        }
    }
}
RW_FUNCTION(0x52D134, "DeckRevolverView::setMedalDatas", DeckRevolverView::setMedalDatas);

// levels[slot] = the level of the keyblade evolution at which the slot opens (0 when it is open already).
// Walks the evolution chain (KB_NEXT_KEYBLADE) while the open slot count stays the same.
void DeckRevolverView::settingSlotOpenLevels(Addr keyblade, Addr levels) {
    uint32_t obj = ld32(keyblade);
    uint32_t sb = ld32(obj + 0x20);
    uint32_t held = ld32(obj + 0x24);
    addRef(held);
    int32_t open = safe32(sb, KB_OPEN_SLOTS);
    if (open <= 4) {
        int32_t cur = open;
        int32_t slot = open;
        do {
            int32_t k = slot;
            for (;;) {
                int32_t next = safe32(sb, KB_NEXT_KEYBLADE);
                if (!next) break;
                Frame f;
                uint32_t row = f.alloc(8);
                call(Keyblade_create, {row, uint32_t(next)});
                uint32_t rowObj = ld32(row), rowCtrl = ld32(row + 4);
                rw::save<uint32_t>(row, 0);
                rw::save<uint32_t>(row + 4, 0);
                if (held) {
                    guest::release(held);
                    release(ld32(row + 4));
                }
                sb = rowObj;
                held = rowCtrl;
                if (cur != safe32(sb, KB_OPEN_SLOTS)) break;
            }
            cur = safe32(sb, KB_OPEN_SLOTS);
            slot = k + 1;
            uint32_t at = ld32(levels) + uint32_t(k) * 4;
            if (cur == slot) rw::save<int32_t>(at, safe32(sb, KB_OPEN_LEVEL));
            else rw::save<int32_t>(at, 0);
        } while (slot < 5);
    }
    release(held);
}
RW_FUNCTION(0x52D434, "DeckRevolverView::settingSlotOpenLevels", DeckRevolverView::settingSlotOpenLevels);

void DeckRevolverView::changeShowType(Addr self, int32_t type) {
    Self *s = S(self);
    s->showType = type;
    if (s->medalViews.end != s->medalViews.begin) {
        uint32_t k = 0;
        do {
            rw::save<int32_t>(viewAt(s, k) + DMV_SHOW_TYPE, type);
            k++;
        } while (k < viewCount(s));
    }
    setMedalDatas(self, self + 0x2C0);
}
RW_FUNCTION(0x52D5B8, "DeckRevolverView::changeShowType", DeckRevolverView::changeShowType);

// The slot under pos (world space): its index when it holds a medal (or any slot when anySlot), else -1.
int32_t DeckRevolverView::getTouchMedalIndex(Addr self, Addr pos, uint32_t anySlot) {
    Self *s = S(self);
    int32_t result = -1;
    if (s->medalViews.end == s->medalViews.begin) return result;
    uint32_t notAny = anySlot ^ 1;
    uint32_t k = 0;
    for (;;) {
        uint32_t view = viewAt(s, k);
        Frame f;
        uint32_t p = f.alloc(8), rect = f.alloc(16);
        call(Node_convertToNodeSpace, {p, view, pos});
        uint32_t w = ld32(vcall(view, VS_getContentSize, {view}));
        uint32_t h = ld32(vcall(view, VS_getContentSize, {view}) + 4);
        call(Rect_ctor, {rect, 0, 0, w, h});
        bool hit = false;
        if (call(Rect_containsPoint, {rect, p}) == 1) {
            if (!(viewCount(s) > k)) {
                call(wl::VECTOR_AT_THROW, {self + 0x32C});
                return result;
            }
            uint32_t medal = find(viewAt(s, k), S_MEDAL, 5);
            hit = true;
            if ((medal != 0 ? 1u : 0u) | notAny) {
                if (k < medalCount(s)) result = int32_t(k);
            }
        }
        call(wl::Vec2_dtor, {rect});
        call(wl::Vec2_dtor, {p});
        k++;
        if (hit) break;
        if (!(k < viewCount(s))) break;
    }
    return result;
}
RW_FUNCTION(0x52D604, "DeckRevolverView::getTouchMedalIndex", DeckRevolverView::getTouchMedalIndex);

// The world position of the medal under pos (zero when none).
void DeckRevolverView::getTouchMedalPosition(Addr out, Addr self, Addr pos) {
    Self *s = S(self);
    call(wl::Vec2_ctor, {out});
    if (s->medalViews.end == s->medalViews.begin) return;
    uint32_t k = 0;
    for (;;) {
        uint32_t view = viewAt(s, k);
        Frame f;
        uint32_t p = f.alloc(8), rect = f.alloc(16);
        call(Node_convertToNodeSpace, {p, view, pos});
        uint32_t w = ld32(vcall(view, VS_getContentSize, {view}));
        uint32_t h = ld32(vcall(view, VS_getContentSize, {view}) + 4);
        call(Rect_ctor, {rect, 0, 0, w, h});
        if (call(Rect_containsPoint, {rect, p}) == 1) {
            if (k < medalCount(s)) {
                if (!(viewCount(s) > k)) {
                    call(wl::VECTOR_AT_THROW, {self + 0x32C});
                    return;
                }
                uint32_t medal = find(viewAt(s, k), S_MEDAL, 5);
                uint32_t parent = vcall(medal, VS_getParent, {medal});
                uint32_t mpos = vcall(medal, VS_getPosition, {medal});
                uint32_t world = f.alloc(8);
                call(Node_convertToWorldSpace, {world, parent, mpos});
                uint32_t x = ld32(world), y = ld32(world + 4);
                st32(out, x);
                st32(out + 4, y);
                call(wl::Vec2_dtor, {world});
            }
            call(wl::Vec2_dtor, {rect});
            call(wl::Vec2_dtor, {p});
            return;
        }
        call(wl::Vec2_dtor, {rect});
        call(wl::Vec2_dtor, {p});
        k++;
        if (!(k < viewCount(s))) return;
    }
}
RW_FUNCTION(0x52D7B8, "DeckRevolverView::getTouchMedalPosition", DeckRevolverView::getTouchMedalPosition);

// Replaces the medal of a slot. animate: a clone of the old medal grows and fades on moveLayer, then $_7 shows
// the new one; otherwise the slot is refreshed at once.
void DeckRevolverView::setMedalData(Addr self, uint32_t index, Addr medal, uint32_t animate, uint32_t relayout) {
    // bytes of the original frame the heap lambda copies from uninitialised lambda padding (sp + 0x85..0x87)
    uint32_t entrySp = rw::cpu->r[13];
    uint8_t pad1 = rw::load<uint8_t>(entrySp - 0x110 + 0x85);
    uint8_t pad2 = rw::load<uint8_t>(entrySp - 0x110 + 0x86);
    uint8_t pad3 = rw::load<uint8_t>(entrySp - 0x110 + 0x87);
    Self *s = S(self);
    if (!(medalCount(s) > index)) {
        call(wl::VECTOR_AT_THROW, {self + 0x2C0});
        return;
    }
    uint32_t oldObj = medalAt(s, index);
    uint32_t oldLo = ld32(oldObj), oldHi = ld32(oldObj + 4);
    eraseMedal(self + 0x2C0, index);
    call(MedalVector_insert, {self + 0x2C0, s->medals.begin + index * 8, medal});
    if (!(viewCount(s) > index)) {
        call(wl::VECTOR_AT_THROW, {self + 0x32C});
        return;
    }
    uint32_t view = viewAt(s, index);
    if (animate == 1) {
        uint32_t node = find(view, S_MEDAL, 5);
        uint32_t layer = s->moveLayer;
        uint32_t parent = vcall(node, VS_getParent, {node});
        uint32_t npos = vcall(node, VS_getPosition, {node});
        Frame f;
        uint32_t world = f.alloc(8), local = f.alloc(8);
        call(Node_convertToWorldSpace, {world, parent, npos});
        call(Node_convertToNodeSpace, {local, layer, world});
        call(wl::Vec2_dtor, {world});
        uint32_t clone = call(Widget_clone, {node});
        vcall(clone, VS_setLocalZOrder, {clone, 2});
        vcall(clone, VS_setPosition, {clone, local});
        uint32_t ml = s->moveLayer;
        vcall(ml, VS_addChild, {ml, clone});
        uint32_t scale = vcall(clone, VS_getScale, {clone});
        uint32_t opacity = vcall(clone, VS_getOpacity, {clone});
        uint32_t mObj = ld32(medal), mCtrl = ld32(medal + 4);
        addRef(mCtrl);
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x30});
        uint32_t toRelease = 0;
        if (heap) {
            st32(heap, VT_setMedalData_7);
            st32(heap + 8, clone);
            st32(heap + 0xC, scale);
            st8(heap + 0x10, uint8_t(opacity));
            st8(heap + 0x12, pad2);
            st8(heap + 0x11, pad1);
            st8(heap + 0x13, pad3);
            st32(heap + 0x14, self);
            st32(heap + 0x18, index);
            st32(heap + 0x1C, mObj);
            st32(heap + 0x20, mCtrl);
            st32(heap + 0x28, oldLo);
            st32(heap + 0x2C, oldHi);
        } else {
            toRelease = mCtrl;
        }
        st32(fo + 16, heap);
        uint32_t cf = call(wl::CallFunc_create, {fo});
        functionDestroy(fo);
        release(toRelease);
        uint32_t grow = call(ScaleTo_create, {fb(0.15f), fb(bf(scale) * 1.2f)});
        uint32_t fade = call(FadeOut_create, {fb(0.15f)});
        uint32_t spawn = call(Spawn_create, {grow, fade, 0});
        runSequence2(clone, spawn, cf);
        call(wl::Vec2_dtor, {local});
        return;
    }
    rw::save<int32_t>(view + DMV_SHOW_TYPE, s->showType);
    Frame f;
    uint32_t copy = f.alloc(8), owner = f.alloc(8);
    uint32_t mCtrl = ld32(medal + 4);
    st32(copy, ld32(medal));
    st32(copy + 4, mCtrl);
    addRef(mCtrl);
    int32_t a = safe32(keybladeData(s), KB_SLOT_A + index * 4);
    int32_t b = safe32(keybladeData(s), KB_SLOT_B + index * 4);
    int32_t c = safe32(keybladeData(s), KB_SLOT_C + index * 4);
    int32_t d = safe32(keybladeData(s), KB_SLOT_D + index * 4);
    uint32_t skills = s->skills;
    uint32_t level = ld32(s->slotOpenLevels.begin + index * 4);
    uint32_t dCtrl = s->detailCtl;
    st32(owner, s->detail);
    st32(owner + 4, dCtrl);
    addRef(dCtrl);
    call(DeckMedalView_setData, {view, copy, index, uint32_t(a), uint32_t(b), uint32_t(c), uint32_t(d), relayout,
                                 level, skills, owner});
    release(dCtrl);
    release(mCtrl);
}
RW_FUNCTION(0x52D980, "DeckRevolverView::setMedalData", DeckRevolverView::setMedalData);

uint32_t DeckRevolverView::setIsShowMixInfo(Addr self, uint32_t show) {
    Self *s = S(self);
    uint32_t end = s->medalViews.end;
    if (end == s->medalViews.begin) return end;
    uint32_t k = 0;
    do {
        st8(viewAt(s, k) + DMV_SHOW_MIX_INFO, uint8_t(show));
        k++;
    } while (k < viewCount(s));
    return k;
}
RW_FUNCTION(0x52E09C, "DeckRevolverView::setIsShowMixInfo", DeckRevolverView::setIsShowMixInfo);

uint32_t DeckRevolverView::setIsShowMedalInfo(Addr self, uint32_t show) {
    Self *s = S(self);
    uint32_t end = s->medalViews.end;
    if (end == s->medalViews.begin) return end;
    uint32_t k = 0;
    do {
        st8(viewAt(s, k) + DMV_SHOW_MEDAL_INFO, uint8_t(show));
        k++;
    } while (k < viewCount(s));
    return k;
}
RW_FUNCTION(0x52E0D4, "DeckRevolverView::setIsShowMedalInfo", DeckRevolverView::setIsShowMedalInfo);

// ------------------------------------------------------------------------------------------------ lambdas
// setMedalData::$_7: the grow / fade clone is done - restore it, take it off, show the new medal in the slot
void DeckRevolverView::onSetMedalDone(Addr fn) {
    uint32_t self = ld32(fn + 0x14);
    Self *s = S(self);
    uint32_t clone = ld32(fn + 8);
    vcall(clone, VS_setScale, {clone, ld32(fn + 0xC)});
    clone = ld32(fn + 8);
    vcall(clone, VS_setOpacity, {clone, rw::load<uint8_t>(fn + 0x10)});
    uint32_t layer = s->moveLayer;
    vcall(layer, VS_removeChild, {layer, ld32(fn + 8), 1});
    uint32_t k = ld32(fn + 0x18);
    if (!(viewCount(s) > k)) {
        call(wl::VECTOR_AT_THROW, {self + 0x32C});
        return;
    }
    uint32_t view = viewAt(s, k);
    rw::save<int32_t>(view + DMV_SHOW_TYPE, s->showType);
    Frame f;
    uint32_t copy = f.alloc(8), owner = f.alloc(8);
    uint32_t mCtrl = ld32(fn + 0x20);
    st32(copy, ld32(fn + 0x1C));
    st32(copy + 4, mCtrl);
    addRef(mCtrl);
    uint32_t idx = ld32(fn + 0x18);
    int32_t a = safe32(keybladeData(s), KB_SLOT_A + ld32(fn + 0x18) * 4);
    int32_t b = safe32(keybladeData(s), KB_SLOT_B + ld32(fn + 0x18) * 4);
    int32_t c = safe32(keybladeData(s), KB_SLOT_C + ld32(fn + 0x18) * 4);
    int32_t d = safe32(keybladeData(s), KB_SLOT_D + ld32(fn + 0x18) * 4);
    uint32_t skills = s->skills;
    uint32_t level = ld32(s->slotOpenLevels.begin + ld32(fn + 0x18) * 4);
    uint32_t dCtrl = s->detailCtl;
    st32(owner, s->detail);
    st32(owner + 4, dCtrl);
    addRef(dCtrl);
    call(DeckMedalView_setData, {view, copy, idx, uint32_t(a), uint32_t(b), uint32_t(c), uint32_t(d), 1, level,
                                 skills, owner});
    release(dCtrl);
    release(mCtrl);
    if (rw::get<uint32_t>(s, 0x298)) call(FunctionVU64_call, {A(s->onSwapMedal), 0, ld32(fn + 0x28), ld32(fn + 0x2C)});
}
RW_FUNCTION(0x52E234, "DeckRevolverView::setMedalData::$_7::operator()", DeckRevolverView::onSetMedalDone);

// $_4: the touch was cancelled - slide the dragged medal home, then reset
void DeckRevolverView::onTouchCancelled(Addr fn, Addr, Addr) {
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    if (!s->moveMedal || s->touchDone) return;
    s->touchDone = 1;
    if (!s->moveMedalEnabled) {
        resetMoveMedal(self);
        return;
    }
    Frame f;
    uint32_t cf = makeCallFunc(f, VT_init_4_back, {self});
    uint32_t m = s->moveMedal;
    uint32_t parent = vcall(m, VS_getParent, {m});
    uint32_t world = f.alloc(8), local = f.alloc(8);
    call(Node_convertToWorldSpace, {world, s->moveParent, self + 0x2F0});
    call(Node_convertToNodeSpace, {local, parent, world});
    moveBack(m, local, cf);
    call(wl::Vec2_dtor, {local});
    call(wl::Vec2_dtor, {world});
}
RW_FUNCTION(0x52E7D8, "DeckRevolverView::init::$_4::operator()", DeckRevolverView::onTouchCancelled);

// $_3: the finger is lifted. Over a slot (while dragging): fly to that slot and swap (lambda #1). Outside the
// slots: dropped on moveLayer it grows and fades (lambda #2), else it flies home (lambda #3). A tap without a
// drag opens the medal info popup (from tutorial phase 700 on; the tutorial close function 115 runs first).
void DeckRevolverView::onTouchEnded(Addr fn, Addr touchRef, Addr) {
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    if (!s->moveMedal || s->touchDone) return;
    uint32_t touch = ld32(touchRef);
    s->touchDone = 1;
    {
        Frame f;
        uint32_t loc = touchLoc(f, touch);
        uint32_t p = f.alloc(8);
        call(Node_convertToNodeSpace, {p, self, loc});
        uint32_t x = ld32(p), y = ld32(p + 4);
        rw::set<uint32_t>(s, 0x2FC, y);
        rw::set<uint32_t>(s, 0x2F8, x);
        call(wl::Vec2_dtor, {p});
        call(wl::Vec2_dtor, {loc});
    }
    int32_t idx;
    {
        Frame f;
        uint32_t loc = touchLoc(f, touch);
        idx = getTouchMedalIndex(self, loc, 0);
        call(wl::Vec2_dtor, {loc});
    }
    if (idx == -1) {
        if (!s->moveMedalEnabled) {
            resetMoveMedal(self);
            return;
        }
        uint32_t layer = s->moveLayer;
        bool inside = false;
        if (layer) {
            Frame f;
            uint32_t loc = touchLoc(f, touch);
            uint32_t p = f.alloc(8), rect = f.alloc(16);
            call(Node_convertToNodeSpace, {p, layer, loc});
            call(wl::Vec2_dtor, {loc});
            uint32_t l = s->moveLayer;
            uint32_t w = ld32(vcall(l, VS_getContentSize, {l}));
            l = s->moveLayer;
            uint32_t h = ld32(vcall(l, VS_getContentSize, {l}) + 4);
            call(Rect_ctor, {rect, 0, 0, w, h});
            inside = call(Rect_containsPoint, {rect, p}) != 0;
            call(wl::Vec2_dtor, {rect});
            call(wl::Vec2_dtor, {p});
        }
        if (inside) {
            uint32_t m = s->moveMedal;
            uint32_t scale = vcall(m, VS_getScale, {m});
            m = s->moveMedal;
            uint32_t opacity = vcall(m, VS_getOpacity, {m});
            Frame f;
            uint32_t cf = makeCallFunc(f, VT_init_3_drop, {self, scale, opacity});
            uint32_t grow = call(ScaleTo_create, {fb(0.15f), fb(bf(scale) * 1.2f)});
            uint32_t fade = call(FadeOut_create, {fb(0.15f)});
            uint32_t spawn = call(Spawn_create, {grow, fade, 0});
            runSequence2(s->moveMedal, spawn, cf);
            return;
        }
        Frame f;
        uint32_t cf = makeCallFunc(f, VT_init_3_back, {self});
        uint32_t m = s->moveMedal;
        uint32_t parent = vcall(m, VS_getParent, {m});
        uint32_t world = f.alloc(8), local = f.alloc(8);
        call(Node_convertToWorldSpace, {world, s->moveParent, self + 0x2F0});
        call(Node_convertToNodeSpace, {local, parent, world});
        moveBack(m, local, cf);
        call(wl::Vec2_dtor, {local});
        call(wl::Vec2_dtor, {world});
        return;
    }
    if (s->moveMedalEnabled) {
        if (!(viewCount(s) > uint32_t(idx))) {
            call(wl::VECTOR_AT_THROW, {self + 0x32C});
            return;
        }
        uint32_t node = find(viewAt(s, uint32_t(idx)), S_MEDAL, 5);
        Frame f;
        uint32_t target = f.alloc(8);
        call(wl::Vec2_ctor, {target});
        if (node) {
            uint32_t p = vcall(node, VS_getPosition, {node});
            st32(target, ld32(p));
            st32(target + 4, ld32(p + 4));
        }
        // lambda #1 {self, index, node, Vec2 target} on the heap (a copy of the stack capture)
        uint32_t capPos = f.alloc(8);
        call(scene::fn::Vec2_ctor_copy, {capPos, target});
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_init_3_swap);
            st32(heap + 4, self);
            st32(heap + 8, uint32_t(idx));
            st32(heap + 0xC, node);
            call(scene::fn::Vec2_ctor_copy, {heap + 0x10, capPos});
        }
        st32(fo + 16, heap);
        uint32_t cf = call(wl::CallFunc_create, {fo});
        functionDestroy(fo);
        call(wl::Vec2_dtor, {capPos});
        bool toSlot = false;
        uint32_t m = s->moveMedal;
        if (node && node != m) {
            uint32_t mi = uint32_t(s->moveIndex.get());
            if (!(medalCount(s) > mi)) {
                call(wl::VECTOR_AT_THROW, {self + 0x2C0});
                return;
            }
            uint32_t obj = medalAt(s, mi);
            toSlot = (ld32(obj) | ld32(obj + 4)) != 0;
        }
        uint32_t world = f.alloc(8), local = f.alloc(8);
        if (toSlot) {
            uint32_t mParent = vcall(m, VS_getParent, {m});
            uint32_t nParent = vcall(node, VS_getParent, {node});
            call(Node_convertToWorldSpace, {world, nParent, target});
            call(Node_convertToNodeSpace, {local, mParent, world});
        } else {
            uint32_t mParent = vcall(m, VS_getParent, {m});
            call(Node_convertToWorldSpace, {world, s->moveParent, self + 0x2F0});
            call(Node_convertToNodeSpace, {local, mParent, world});
        }
        call(wl::Vec2_dtor, {world});
        moveBack(s->moveMedal, local, cf);
        call(wl::Vec2_dtor, {local});
        call(wl::Vec2_dtor, {target});
    }
    if (s->openMedalInfoPopupEnabled && !s->popupOpened && !s->moved) {
        if (s->moveIndex == 0) {
            Frame f;
            uint32_t close = f.alloc(24);
            call(AppState_getTutorialCloseFunc, {close, call(AppState_getInstance, {}), 0x73});
            uint32_t p = ld32(close + 16);
            if (p) {
                if (p == close) vcall(p, 0x10, {p});
                else vcall(p, 0x14, {p});
                call(AppState_execTutorialCloseFunc, {call(AppState_getInstance, {}), 0x73});
            }
        }
        if (int32_t(call(SceneUtil_getNexTutorialphase, {})) >= 0x2BC) {
            s->popupOpened = 1;
            uint32_t mi = uint32_t(s->moveIndex.get());
            if (!(medalCount(s) > mi)) {
                call(wl::VECTOR_AT_THROW, {self + 0x2C0});
                return;
            }
            uint32_t m = s->medals.begin + mi * 8;
            uint32_t obj = ld32(m), ctrl = ld32(m + 4);
            addRef(ctrl);
            if (safe32(ld32(obj + 0x58), 0) == 1) {
                release(ctrl);
            } else {
                call(AudioPlaySystemSEDecide, {});
                Frame f;
                uint32_t medal = f.alloc(8);
                if (ctrl) {
                    guest::addRef(ctrl);
                    st32(medal, obj);
                    st32(medal + 4, ctrl);
                    guest::addRef(ctrl);
                } else {
                    st32(medal, obj);
                    st32(medal + 4, 0);
                }
                uint8_t ownerFlag = safe8(s->detail, 0x54);
                uint8_t flag314 = s->field_314;
                uint8_t flag328 = s->field_328;
                addRef(ctrl);
                uint32_t onButton = f.alloc(24);
                st32(onButton, VT_init_3_button);
                st32(onButton + 8, obj);
                st32(onButton + 12, ctrl);
                st32(onButton + 4, self);
                st32(onButton + 16, onButton);
                uint32_t skills = s->skills;
                uint32_t onClose = f.alloc(24);
                st32(onClose + 16, 0);
                call(SceneUtil_openMedalInfoPopup, {medal, ownerFlag != 0 ? 1u : 0u, flag328 != 0 ? 1u : 0u,
                                                    flag314 != 0 ? 1u : 0u, onButton, skills, onClose, 0});
                functionDestroy(onClose);
                functionDestroy(onButton);
                release(ld32(medal + 4));
                if (ctrl) {
                    guest::release(ctrl);
                    guest::release(ctrl);
                }
            }
        }
    }
    if (!s->moveMedalEnabled) resetMoveMedal(self);
}
RW_FUNCTION(0x52EA5C, "DeckRevolverView::init::$_3::operator()", DeckRevolverView::onTouchEnded);

// $_3 lambda #3: flown home - reset, refresh the slot
void DeckRevolverView::onMoveBackDone(Addr fn) {
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    resetMoveMedal(self);
    uint32_t mi = uint32_t(s->moveIndex.get());
    if (!(medalCount(s) > mi)) {
        call(wl::VECTOR_AT_THROW, {self + 0x2C0});
        return;
    }
    Frame f;
    uint32_t copy = f.alloc(8);
    uint32_t m = s->medals.begin + mi * 8;
    st32(copy, ld32(m));
    st32(copy + 4, ld32(m + 4));
    addRef(ld32(copy + 4));
    setMedalData(self, mi, copy, 0, 0);
    release(ld32(copy + 4));
    uint32_t k = uint32_t(s->moveIndex.get());
    if (!(viewCount(s) > k)) {
        call(wl::VECTOR_AT_THROW, {self + 0x32C});
        return;
    }
    call(DeckMedalView_updateLayout, {viewAt(s, k), 1});
}
RW_FUNCTION(0x52F390, "DeckRevolverView::init::$_3::{lambda()#3}::operator()", DeckRevolverView::onMoveBackDone);

// $_3 lambda #2: dropped outside the slots - remove the medal from the deck (a sound, the slot gets an empty medal)
void DeckRevolverView::onDropOutsideDone(Addr fn) {
    // stale words of the original frame passed in the outgoing UserMedal::Medal argument (padding)
    uint32_t entrySp = rw::cpu->r[13];
    uint32_t argPad = rw::load<uint32_t>(entrySp - 0xF0 + 4);
    uint32_t medalPad = rw::load<uint32_t>(entrySp - 0xF0 + 0xB4);
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    {
        Frame f;
        uint32_t layer = f.alloc(8);
        call(AudioManager_getLayer, {layer, call(AudioManager_getInstance, {}), 10000});
        call(AudioLayer_playSE, {ld32(layer), 0x1E56BA76u, 0, 0});
        release(ld32(layer + 4));
    }
    uint32_t m = s->moveMedal;
    vcall(m, VS_setScale, {m, ld32(fn + 8)});
    m = s->moveMedal;
    vcall(m, VS_setOpacity, {m, rw::load<uint8_t>(fn + 0xC)});
    uint32_t mi = uint32_t(s->moveIndex.get());
    if (!(medalCount(s) > mi)) {
        call(wl::VECTOR_AT_THROW, {self + 0x2C0});
        return;
    }
    uint32_t obj = medalAt(s, mi);
    uint32_t lo = ld32(obj), hi = ld32(obj + 4);
    resetMoveMedal(self);
    uint32_t mdm = call(MedalDataManager_getInstance, {});
    Frame f;
    uint32_t empty = f.alloc(8);
    // getMedalData(sret, mdm, id 0 (r2:r3), int 0, UserMedal::Medal{} by value at sp + 8)
    uint32_t w[18];
    for (uint32_t k = 0; k < 18; k++) w[k] = 0;
    w[11] = medalPad;    // bytes 0x2C..0x2F are not cleared by the original
    call(MedalDataManager_getMedalData, {empty, mdm, 0, 0, 0, argPad, w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7],
                                         w[8], w[9], w[10], w[11], w[12], w[13], w[14], w[15], w[16], w[17]});
    eraseMedal(self + 0x2C0, uint32_t(s->moveIndex.get()));
    call(MedalVector_insert, {self + 0x2C0, s->medals.begin + uint32_t(s->moveIndex.get()) * 8, empty});
    setMedalDatas(self, self + 0x2C0);
    if (rw::get<uint32_t>(s, 0x298)) call(FunctionVU64_call, {A(s->onSwapMedal), 0, lo, hi});
    if (rw::get<uint32_t>(s, 0x2B0)) call(wl::FunctionVV_call, {A(s->onMedalChanged)});
    release(ld32(empty + 4));
}
RW_FUNCTION(0x52F508, "DeckRevolverView::init::$_3::{lambda()#2}::operator()", DeckRevolverView::onDropOutsideDone);

// the medal info popup's buttons: forwarded to onChangeMedal with the medal's id
void DeckRevolverView::onMedalInfoButton(Addr fn, Addr typeRef, Addr flagRef) {
    uint32_t self = ld32(fn + 4);
    if (!ld32(self + 0x280)) return;
    uint32_t obj = ld32(fn + 8);
    call(FunctionButton_call, {self + 0x270, ld32(typeRef), rw::load<uint8_t>(flagRef), self, ld32(obj), ld32(obj + 4)});
}
RW_FUNCTION(0x52F85C, "DeckRevolverView::init::$_3::popup button lambda::operator()", DeckRevolverView::onMedalInfoButton);

// $_3 lambda #1: flown onto another slot - swap the two medals (the same slot: just refresh it)
void DeckRevolverView::onSwapDone(Addr fn) {
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    resetMoveMedal(self);
    uint32_t mi = uint32_t(s->moveIndex.get());
    uint32_t vec = self + 0x2C0;
    if (mi == ld32(fn + 8)) {
        if (s->popupOpened) return;
        if (!(viewCount(s) > mi)) {
            call(wl::VECTOR_AT_THROW, {self + 0x32C});
            return;
        }
        call(DeckMedalView_updateLayout, {viewAt(s, mi), 1});
        return;
    }
    if (!(medalCount(s) > mi)) {
        call(wl::VECTOR_AT_THROW, {vec});
        return;
    }
    if (safe32(ld32(medalAt(s, mi) + 0x58), 0) == 1) return;
    uint32_t node = ld32(fn + 0xC);
    vcall(node, VS_setPosition, {node, fn + 0x10});
    uint32_t target = ld32(fn + 8);
    if (!(medalCount(s) > target)) {
        call(wl::VECTOR_AT_THROW, {vec});
        return;
    }
    Frame f;
    uint32_t tMedal = f.alloc(8), mMedal = f.alloc(8);
    uint32_t p = s->medals.begin + target * 8;
    st32(tMedal, ld32(p));
    st32(tMedal + 4, ld32(p + 4));
    addRef(ld32(tMedal + 4));
    mi = uint32_t(s->moveIndex.get());
    if (!(medalCount(s) > mi)) {
        call(wl::VECTOR_AT_THROW, {vec});
        return;
    }
    p = s->medals.begin + mi * 8;
    st32(mMedal, ld32(p));
    st32(mMedal + 4, ld32(p + 4));
    addRef(ld32(mMedal + 4));
    uint32_t insertAt;
    if (int32_t(mi) >= int32_t(ld32(fn + 8))) {
        eraseMedal(vec, mi);
        eraseMedal(vec, ld32(fn + 8));
        insertAt = target;
    } else {
        eraseMedal(vec, ld32(fn + 8));
        eraseMedal(vec, mi);
        insertAt = target - 1;
    }
    call(MedalVector_insert, {vec, s->medals.begin + insertAt * 8, mMedal});
    call(MedalVector_insert, {vec, s->medals.begin + mi * 8, tMedal});
    if (!(medalCount(s) > mi)) {
        call(wl::VECTOR_AT_THROW, {vec});
        return;
    }
    uint32_t c1 = f.alloc(8);
    p = s->medals.begin + mi * 8;
    st32(c1, ld32(p));
    st32(c1 + 4, ld32(p + 4));
    addRef(ld32(c1 + 4));
    setMedalData(self, mi, c1, 0, 1);
    release(ld32(c1 + 4));
    uint32_t t = ld32(fn + 8);
    if (!(medalCount(s) > t)) {
        call(wl::VECTOR_AT_THROW, {vec});
        return;
    }
    uint32_t c2 = f.alloc(8);
    p = s->medals.begin + t * 8;
    st32(c2, ld32(p));
    st32(c2 + 4, ld32(p + 4));
    addRef(ld32(c2 + 4));
    setMedalData(self, t, c2, 0, 1);
    release(ld32(c2 + 4));
    if (rw::get<uint32_t>(s, 0x2B0)) call(wl::FunctionVV_call, {A(s->onMedalChanged)});
    release(ld32(mMedal + 4));
    release(ld32(tMedal + 4));
}
RW_FUNCTION(0x52FA64, "DeckRevolverView::init::$_3::{lambda()#1}::operator()", DeckRevolverView::onSwapDone);

// $_2: dragging - track the touch; past 20 pixels it is a move; the dragged clone follows on moveLayer
void DeckRevolverView::onTouchMoved(Addr fn, Addr touchRef, Addr) {
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    if (!s->moveMedal || s->touchDone) return;
    uint32_t touch = ld32(touchRef);
    {
        Frame f;
        uint32_t loc = touchLoc(f, touch);
        uint32_t p = f.alloc(8);
        call(Node_convertToNodeSpace, {p, self, loc});
        uint32_t x = ld32(p), y = ld32(p + 4);
        rw::set<uint32_t>(s, 0x2F8, x);
        rw::set<uint32_t>(s, 0x2FC, y);
        call(wl::Vec2_dtor, {p});
        call(wl::Vec2_dtor, {loc});
    }
    float dx = bf(fb(s->touchStartX - s->touchX) & 0x7FFFFFFFu);
    if (dx >= 20.0f) {
        s->moved = 1;
    } else {
        float dy = bf(fb(s->touchStartY - s->touchY) & 0x7FFFFFFFu);
        if (!(dy < 20.0f || dy != dy)) s->moved = 1;
    }
    if (!s->moveMedalEnabled) return;
    uint32_t mi = uint32_t(s->moveIndex.get());
    if (!(medalCount(s) > mi)) {
        call(wl::VECTOR_AT_THROW, {self + 0x2C0});
        return;
    }
    if (safe32(ld32(medalAt(s, mi) + 0x58), 0) == 1) return;
    uint32_t panel = ld32(s->moveView + DMV_INFO_PANEL);
    vcall(panel, VS_setVisible, {panel, 0});
    uint32_t view = s->moveView;
    uint32_t mag = ld32(view + DMV_DARKLIGHT_MAG);
    if (mag && rw::load<int32_t>(view + DMV_MAGNIFICATION) >= 10001) vcall(mag, VS_setVisible, {mag, 1});
    uint32_t layer = s->moveLayer;
    uint32_t m = s->moveMedal;
    Frame f;
    uint32_t loc = touchLoc(f, touch);
    uint32_t p = f.alloc(8);
    call(Node_convertToNodeSpace, {p, layer, loc});
    vcall(m, VS_setPosition, {m, p});
    call(wl::Vec2_dtor, {p});
    call(wl::Vec2_dtor, {loc});
}
RW_FUNCTION(0x5300BC, "DeckRevolverView::init::$_2::operator()", DeckRevolverView::onTouchMoved);

// $_1: a touch on a visible medal starts a drag: with dragging on, a clone 1.2x larger moves to moveLayer
uint32_t DeckRevolverView::onTouchBegan(Addr fn, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    {
        Frame f;
        uint32_t loc = touchLoc(f, touch);
        uint32_t p = f.alloc(8);
        call(Node_convertToNodeSpace, {p, self, loc});
        uint32_t x = ld32(p), y = ld32(p + 4);
        rw::set<uint32_t>(s, 0x2FC, y);
        rw::set<uint32_t>(s, 0x2F8, x);
        call(wl::Vec2_dtor, {p});
        call(wl::Vec2_dtor, {loc});
    }
    if (s->touchDone) return 0;
    s->popupOpened = 0;
    int32_t idx;
    {
        Frame f;
        uint32_t loc = touchLoc(f, touch);
        idx = getTouchMedalIndex(self, loc, 1);
        call(wl::Vec2_dtor, {loc});
    }
    if (idx == -1) return 0;
    if (!(viewCount(s) > uint32_t(idx))) {
        call(wl::VECTOR_AT_THROW, {self + 0x32C});
        return 0;
    }
    uint32_t view = viewAt(s, uint32_t(idx));
    s->moveView = view;
    s->moveMedal = find(view, S_MEDAL, 5);
    uint32_t m = s->moveMedal;
    if (!m) return 0;
    if (!vcall(m, VS_isVisible, {m})) return 0;
    s->moveIndex = idx;
    m = s->moveMedal;
    uint32_t pos = vcall(m, VS_getPosition, {m});
    uint32_t px = ld32(pos), py = ld32(pos + 4);
    rw::set<uint32_t>(s, 0x2F4, py);
    rw::set<uint32_t>(s, 0x2F0, px);
    uint32_t tx = rw::get<uint32_t>(s, 0x2F8), ty = rw::get<uint32_t>(s, 0x2FC);
    rw::set<uint32_t>(s, 0x2EC, ty);
    rw::set<uint32_t>(s, 0x2E8, tx);
    if (!s->moveMedalEnabled) return 1;
    m = s->moveMedal;
    s->moveParent = vcall(m, VS_getParent, {m});
    uint32_t clone = call(Widget_clone, {s->moveMedal});
    uint32_t parent = s->moveParent;
    vcall(parent, VS_removeChild, {parent, s->moveMedal, 1});
    uint32_t layer = s->moveLayer;
    vcall(layer, VS_addChild, {layer, clone});
    s->moveMedal = clone;
    uint32_t scale = vcall(clone, VS_getScale, {clone});
    rw::set<uint32_t>(s, 0x308, scale);
    m = s->moveMedal;
    vcall(m, VS_setScale, {m, fb(bf(scale) * 1.2f)});
    m = s->moveMedal;
    vcall(m, VS_setLocalZOrder, {m, 2});
    layer = s->moveLayer;
    parent = s->moveParent;
    m = s->moveMedal;
    {
        Frame f;
        uint32_t mpos = vcall(m, VS_getPosition, {m});
        uint32_t world = f.alloc(8), local = f.alloc(8);
        call(Node_convertToWorldSpace, {world, parent, mpos});
        call(Node_convertToNodeSpace, {local, layer, world});
        vcall(m, VS_setPosition, {m, local});
        call(wl::Vec2_dtor, {local});
        call(wl::Vec2_dtor, {world});
    }
    uint32_t mi = uint32_t(s->moveIndex.get());
    if (!(medalCount(s) > mi)) {
        call(wl::VECTOR_AT_THROW, {self + 0x2C0});
        return 1;
    }
    uint32_t mp = s->medals.begin + mi * 8;
    uint32_t obj = ld32(mp), ctrl = ld32(mp + 4);
    addRef(ctrl);
    supportEffectIfSupport(obj, self + 0x2DC);
    release(ctrl);
    return 1;
}
RW_FUNCTION(0x530330, "DeckRevolverView::init::$_1::operator()", DeckRevolverView::onTouchBegan);
