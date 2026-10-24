// UI building blocks of the scene code: find a named widget in a cocostudio layout, fill texts from the text
// table (TextData), attach touch callbacks (inline lambdas), copy std::function objects the way libc++ does.
#pragma once

#include "../core/SafeBuffer.hpp"
#include "SceneUtil.hpp"

namespace scene {

namespace fn {
constexpr uint32_t StringBuffer_ctor = 0x49140D;          // StringBuffer(this, fmt, ...): a 1 KB printf buffer
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;   // (widget, std::function<void(Ref*,TouchEventType)>)
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;   // (reader, const char* file)
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69;
constexpr uint32_t SceneManager_getPopupLayer2 = 0x613E71;
constexpr uint32_t SceneManager_openPopup = 0x61524D;     // (mgr, function<bool()>*, function<void(bool)>*, bool)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t Misc_create = 0x786D5D;                // master::Misc::create(shared_ptr out, int id)
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;     // (image, const string&, TextureResType)
constexpr uint32_t Color3B_ctor = rwlib::cocos2d::Color3B::ctor_base;              // (out, r, g, b)
}  // namespace fn

constexpr uint32_t VEC2_ANCHOR_MIDDLE = 0xFEF5C0;         // cocos2d::Vec2::ANCHOR_MIDDLE
constexpr uint32_t STRING_BUFFER_SIZE = 1028;

// ------------------------------------------------------------------------------------------------ SafeBuffer
inline const SafeBuffer *safe(uint32_t sb) { return rw::host_ptr<SafeBuffer>(sb); }
// a 32-bit value of a SafeBuffer (the original decodes it to a stack temporary with SafeBuffer::read)
inline int32_t safe32(uint32_t sb, uint32_t index) { return safe(sb)->get32(index); }
// n decoded bytes of a SafeBuffer in a guest stack buffer (strings stored in master/user data)
inline uint32_t safeBytes(Frame &f, uint32_t sb, uint32_t index, uint32_t n) {
    uint32_t dst = f.alloc(n);
    const SafeBuffer *s = safe(sb);
    SafeBuffer::read(s, rw::host_ptr<uint8_t>(dst), s->data.get() + index, n, index);
    return dst;
}
// master::<Table>::create(shared_ptr out, id): the row (a SafeBuffer) in a shared_ptr on the guest stack
inline uint32_t masterRow(Frame &f, uint32_t create, uint32_t id) {
    uint32_t sp = f.alloc(8);
    call(create, {sp, id});
    return sp;
}

// ------------------------------------------------------------------------------------------------ widgets
// SceneUtil::searchNodeByName with a temporary name built from a string literal (guest address, length)
inline uint32_t findNode(uint32_t root, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    strFree(s);
    return node;
}

// TextData::get(type 0, id) into a shared_ptr on the guest stack; the entry's text is at +4
inline uint32_t textData(Frame &f, int32_t id, uint32_t type = 0) {
    uint32_t sp = f.alloc(8);
    call(fn::TextData_get, {sp, type, uint32_t(id)});
    return sp;
}
inline uint32_t textDataStr(uint32_t sp) { return ld32(ld32(sp) + 4); }

// text->setString(c string)
inline void setTextC(uint32_t text, uint32_t cstr) {
    Frame f;
    uint32_t s = newStr(f, cstr);
    call(fn::Text_setString, {text, s});
    strFree(s);
}
// text->setString(TextData text `id`)
inline void setTextId(uint32_t text, int32_t id, uint32_t type = 0) {
    Frame f;
    uint32_t sp = textData(f, id, type);
    setTextC(text, textDataStr(sp));
    sharedRelease(sp);
}

// SceneUtil::settingTitleLabel(node, literal name, text id) with the temporary name
inline uint32_t titleLabel(uint32_t node, uint32_t name, uint32_t len, int32_t textId) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    uint32_t text = SceneUtil::settingTitleLabel(node, s, textId);
    strFree(s);
    return text;
}

// a StringBuffer (printf into a 1 KB stack buffer)
inline uint32_t stringBuffer(Frame &f, std::initializer_list<uint32_t> fmtAndArgs) {
    uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
    uint32_t args[10];
    uint32_t n = 0;
    args[n++] = buf;
    for (uint32_t a : fmtAndArgs) args[n++] = a;
    switch (n) {
    case 2: call(fn::StringBuffer_ctor, {args[0], args[1]}); break;
    case 3: call(fn::StringBuffer_ctor, {args[0], args[1], args[2]}); break;
    case 4: call(fn::StringBuffer_ctor, {args[0], args[1], args[2], args[3]}); break;
    case 5: call(fn::StringBuffer_ctor, {args[0], args[1], args[2], args[3], args[4]}); break;
    case 6: call(fn::StringBuffer_ctor, {args[0], args[1], args[2], args[3], args[4], args[5]}); break;
    case 7: call(fn::StringBuffer_ctor, {args[0], args[1], args[2], args[3], args[4], args[5], args[6]}); break;
    case 8: call(fn::StringBuffer_ctor, {args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]}); break;
    default: call(fn::StringBuffer_ctor, {args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]}); break;
    }
    return buf;
}

// widget->addTouchEventListener(lambda): the std::function is built inline on the stack and destroyed after
inline void addTouch(uint32_t widget, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t cb = newFunction(f, vtable, captures);
    call(fn::Widget_addTouchEventListener, {widget, cb});
    functionDestroy(cb);
}

// libc++ std::function copy constructor, inline (clone the callable into dst)
inline void functionCopy(uint32_t dst, uint32_t src) {
    uint32_t p = ld32(src + 16);
    if (!p) {
        st32(dst + 16, 0);
    } else if (p == src) {
        st32(dst + 16, dst);
        vcall(p, 0x0C, {p, dst});      // __clone(dst)
    } else {
        st32(dst + 16, vcall(p, 0x08, {p}));   // __clone() -> heap copy
    }
}

// A std::function at `dst` whose callable is a heap __func (vtable, `size` bytes) that holds, at `fnOffset`, a
// std::function moved (moveCtor) from the stack temporary `tmp` (a lambda capturing a callback by value).
// Returns the heap object (0 when the allocation failed); the caller stores the other captures.
inline uint32_t heapFunction(uint32_t dst, uint32_t vtable, uint32_t size, uint32_t fnOffset, uint32_t moveCtor,
                             uint32_t tmp) {
    st32(dst + 16, 0);
    uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, vtable);
        call(moveCtor, {heap + fnOffset, tmp});
    }
    st32(dst + 16, heap);
    return heap;
}

// Centre a cocostudio popup on the screen: anchor in the middle, position offset by half its size and by the
// margins of a visible area larger than the 960x640 design size.
inline void centerPopup(uint32_t popup) {
    vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_MIDDLE});
    float px = bitsf(vcall(popup, node::getPositionX, {popup}));
    float w = ldf(vcall(popup, node::getContentSize, {popup}));
    float x = (px + w * 0.5f) + (visibleSize().width + -960.0f) * 0.5f;
    float py = bitsf(vcall(popup, node::getPositionY, {popup}));
    float h = ldf(vcall(popup, node::getContentSize, {popup}) + 4);
    float y = (py + h * 0.5f) + (visibleSize().height + -640.0f) * 0.5f;
    Frame f;
    vcall(popup, node::setPosition, {popup, newVec2(f, x, y)});
}

inline uint32_t sceneManager() { return call(fn::SceneManager_getInstance, {}); }

// The usual popup opening: the popup sound, a modal shadow fading in on popup layer 2, then the cocostudio
// layout `json` added on top and centred.
struct PopupBase {
    uint32_t layer, shadow, popup;
};
inline PopupBase loadPopup(uint32_t json) {
    PopupBase p;
    call(fn::AudioPlaySystemSEPopUp, {});
    p.layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    p.shadow = SceneUtil::createShadowSprite();
    uint32_t opacity = vcall(p.shadow, node::getOpacity, {p.shadow});
    vcall(p.shadow, node::setOpacity, {p.shadow, 0});
    call(fn::Node_runAction, {p.shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(p.layer, node::addChild, {p.layer, p.shadow});
    p.popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    vcall(p.layer, node::addChild, {p.layer, p.popup});
    centerPopup(p.popup);
    return p;
}

}  // namespace scene
