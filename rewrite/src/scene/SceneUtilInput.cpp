// SceneUtil: text input helpers (replace a cocostudio TextField by an edit box / a plain Text).
#include "SceneUtil.hpp"

using namespace scene;

namespace {
constexpr uint32_t TextField_getMaxLength = 0x83F7DD;
constexpr uint32_t TextField_getFontSize = 0x83F72D;
constexpr uint32_t TextField_getTextHorizontalAlignment = 0x83FF59;
constexpr uint32_t Layer_create = 0x88A8C3;
constexpr uint32_t Label_createWithTTF = 0x883825;   // (text, fontFile, size, const Size& dims, hAlign, vAlign)
constexpr uint32_t Label_setDimensions = 0x883A8D;   // (label, width, height)
constexpr uint32_t Label_setAlignment = 0x884CB9;    // (label, hAlign, vAlign)
constexpr uint32_t Text_create = 0x839DED;
constexpr uint32_t Text_setFontName = 0x83A499;
constexpr uint32_t Text_setFontSize = 0x83A35D;
constexpr uint32_t Text_setTextAreaSize = 0x83A605;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t TouchFunction_swap = 0x483E4D;    // std::function<bool(Touch*,Event*)>::swap
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;

constexpr uint32_t SIZE_ZERO = 0x10068C0;            // cocos2d::Size::ZERO
constexpr uint32_t VT_EditBoxTouchLambda = 0xF55218; // __func<textFieldToEditBox(...)::$_1> (heap stored)
constexpr uint32_t VT_EditingDidEndLambda = 0xF551EC; // __func<editBoxEditingDidEnd(...)::$_2>

constexpr uint32_t Node_ignoreAnchorPointForPosition = 0xE8;
constexpr uint32_t Node_getLocalZOrder = 0x18;
constexpr uint32_t Label_setString = 0x2A8;          // Label vtable slot
constexpr uint32_t Widget_ignoreContentAdaptWithSize = 0x2D4;
constexpr uint32_t Label_width = 0x3D8, Label_vAlignment = 0x3E0;   // Label fields: dimensions height, v-align
constexpr uint32_t Director_animationInterval = 0xA8;              // double
constexpr uint32_t Director_eventDispatcher = 0x84;
constexpr uint32_t Listener_onTouchBegan = 0x50;
}  // namespace

// Replace a TextField by a Layer (the tap area, size `areaSize`) plus a Label showing `initialText`; tapping the
// layer opens the platform edit box (the lambda, which fills *editBoxRef / *labelRef). Returns the label;
// *layerOut (when given) receives the layer.
SceneUtil::Addr SceneUtil::textFieldToEditBox(Addr editBoxRef, Addr labelRef, Addr textField, Addr placeholder,
                                              Addr initialText, Addr fontFile, Addr areaSize, Addr delegate,
                                              Addr editingFlag, Addr layerOut, int32_t maxLength) {
    int32_t fieldMax = int32_t(call(TextField_getMaxLength, {textField}));
    uint32_t parent = vcall(textField, node::getParent, {textField});
    uint32_t layer = call(Layer_create, {});
    vcall(layer, node::setContentSize, {layer, areaSize});
    vcall(layer, Node_ignoreAnchorPointForPosition, {layer, 0});
    vcall(layer, node::setPosition, {layer, vcall(textField, node::getPosition, {textField})});
    vcall(layer, node::setAnchorPoint, {layer, vcall(textField, node::getAnchorPoint, {textField})});
    vcall(parent, node::addChildZ, {parent, layer, vcall(textField, Node_getLocalZOrder, {textField})});
    int32_t limit = maxLength > 0 ? maxLength : fieldMax;
    if (layerOut) st32(layerOut, layer);

    Frame f;
    uint32_t textStr = newStr(f, initialText);
    uint32_t fontStr = newStr(f, fontFile);
    float fontSize = float(int32_t(call(TextField_getFontSize, {textField})));
    uint32_t label = call(Label_createWithTTF, {textStr, fontStr, fbits(fontSize), SIZE_ZERO, 0, 0});
    strFree(fontStr);
    strFree(textStr);
    vcall(label, node::setPosition, {label, vcall(textField, node::getPosition, {textField})});
    vcall(label, node::setAnchorPoint, {label, vcall(textField, node::getAnchorPoint, {textField})});
    vcall(label, node::setContentSize, {label, vcall(textField, node::getContentSize, {textField})});
    float width = ldf(vcall(textField, node::getContentSize, {textField}));
    call(Label_setDimensions, {label, f2u(width), ld32(label + Label_width)});
    call(Label_setAlignment, {label, call(TextField_getTextHorizontalAlignment, {textField}),
                              ld32(label + Label_vAlignment)});
    vcall(parent, node::addChildZ, {parent, label, vcall(textField, Node_getLocalZOrder, {textField})});

    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    // the lambda captures (editBoxRef, editingFlag, label, limit, placeholder copy, delegate, labelRef): 36 bytes,
    // so libc++ stores it on the heap
    uint32_t copy = f.alloc(12);
    if (strIsLong(placeholder)) {
        strInit(copy, ld32(placeholder + 8), ld32(placeholder + 4));
    } else {
        for (uint32_t k = 0; k < 12; k += 4) st32(copy + k, ld32(placeholder + k));
    }
    uint32_t onBegan = f.alloc(20);
    st32(onBegan + 16, 0);
    uint32_t fn_ = call(fn::operator_new, {0x28});
    bool moved = false;
    if (fn_) {
        st32(fn_, VT_EditBoxTouchLambda);
        st32(fn_ + 4, editBoxRef);
        st32(fn_ + 8, editingFlag);
        st32(fn_ + 12, label);
        st32(fn_ + 16, uint32_t(limit));
        for (uint32_t k = 0; k < 12; k += 4) st32(fn_ + 20 + k, ld32(copy + k));   // the string, moved
        st32(fn_ + 0x24, labelRef);
        st32(fn_ + 0x20, delegate);
        moved = true;
    }
    st32(onBegan + 16, fn_);
    call(TouchFunction_swap, {onBegan, listener + Listener_onTouchBegan});
    functionDestroy(onBegan);
    if (!moved && strIsLong(copy)) call(fn::operator_delete, {ld32(copy + 8)});
    call(EventDispatcher_addEventListenerWithSceneGraphPriority,
         {ld32(director() + Director_eventDispatcher), listener, layer});
    vcall(parent, node::removeChild, {parent, textField, 1});
    return label;
}
RW_FUNCTION(0x45C0A0, "SceneUtil::textFieldToEditBox", SceneUtil::textFieldToEditBox);

// Replace a TextField by a ui::Text with the same placement, font and area, showing `text`.
SceneUtil::Addr SceneUtil::textFieldToUiText(Addr textField, Addr text, Addr fontName) {
    uint32_t parent = vcall(textField, node::getParent, {textField});
    uint32_t label = call(Text_create, {});
    vcall(label, node::setPosition, {label, vcall(textField, node::getPosition, {textField})});
    vcall(label, node::setAnchorPoint, {label, vcall(textField, node::getAnchorPoint, {textField})});
    Frame f;
    uint32_t font = newStr(f, fontName);
    call(Text_setFontName, {label, font});
    strFree(font);
    call(Text_setFontSize, {label, call(TextField_getFontSize, {textField})});
    call(Text_setTextAreaSize, {label, vcall(textField, node::getContentSize, {textField})});
    vcall(label, node::setContentSize, {label, vcall(textField, node::getContentSize, {textField})});
    vcall(label, Widget_ignoreContentAdaptWithSize, {label, 0});
    uint32_t str = newStr(f, text);
    call(fn::Text_setString, {label, str});
    strFree(str);
    vcall(parent, node::addChildZ, {parent, label, vcall(textField, Node_getLocalZOrder, {textField})});
    return label;
}
RW_FUNCTION(0x45C464, "SceneUtil::textFieldToUiText", SceneUtil::textFieldToUiText);

// The edit box closed: show the entered text, hide the tap layer and re-enable it one frame later.
void SceneUtil::editBoxEditingDidEnd(Addr layerRef, Addr label, Addr text) {
    if (!ld32(layerRef)) return;
    Frame f;
    uint32_t str = newStr(f, text);
    vcall(label, Label_setString, {label, str});
    strFree(str);
    uint32_t layer = ld32(layerRef);
    vcall(layer, node::setVisible, {layer, 0});
    layer = ld32(layerRef);
    double interval = rw::load<double>(director() + Director_animationInterval);
    uint32_t delay = call(fn::DelayTime_create, {fbits(float(interval))});
    uint32_t done = newFunction(f, VT_EditingDidEndLambda, {layerRef});
    uint32_t callback = call(fn::CallFunc_create, {done});
    call(fn::Node_runAction, {layer, call(fn::Sequence_create, {delay, callback, 0})});
    functionDestroy(done);
}
RW_FUNCTION(0x45C61C, "SceneUtil::editBoxEditingDidEnd", SceneUtil::editBoxEditingDidEnd);
