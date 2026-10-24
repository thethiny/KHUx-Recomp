// SceneUtil: text colours / effects, button states and number strings for the atlas fonts.
#include "SceneUi.hpp"

#include <cmath>

using namespace scene;

namespace {
constexpr uint32_t TintTo_create = rwlib::cocos2d::TintTo::create;              // (duration, r, g, b)
constexpr uint32_t RepeatForever_create = rwlib::cocos2d::RepeatForever::create;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;
constexpr uint32_t ImageButton_setIsDisabled = 0x5318E1;
constexpr uint32_t ButtonLabelColor = 0x4744F5;           // static helper: label colour / shadow for a button state
constexpr uint32_t Label_getAdditionalKerning = rwlib::cocos2d::Label::getAdditionalKerning;
constexpr uint32_t Label_setAdditionalKerning = rwlib::cocos2d::Label::setAdditionalKerning;
constexpr uint32_t LWFNode_create = rwlib::cocos2d::LWFNode::create;             // (file, void*)
constexpr uint32_t String_concat = 0x470EAD;              // std::operator+(const string&, const string&) (sret)

constexpr uint32_t COLOR3B_WHITE = 0x1007E88, COLOR3B_RED = 0x1007E94;
constexpr uint32_t Widget_setTouchEnabled = 0x29C;
constexpr uint32_t Text_getVirtualRenderer = 0x2D8;

constexpr uint32_t STR_supportLWF = 0xDF9023;
constexpr uint32_t STR_SupportMedalLwf = 0xDF902E;        // "lwf/supportMedal/Medal_Support/Medal_Support.lwf"
constexpr uint32_t STR_AtlasDigits = 0xDF959C;            // "a", "b", ... "k": one 2-byte C string per digit
constexpr uint32_t STR_Empty = 0xDF8A1A;

void setColor(uint32_t node, uint8_t r, uint8_t g, uint8_t b) {
    Frame f;
    uint32_t c = f.alloc(4);
    st8(c, r); st8(c + 1, g); st8(c + 2, b);
    vcall(node, node::setColor, {node, c});
}

// The atlas font maps digit d to the letter 'a' + d: the eleven one-letter strings, built like the original
uint32_t atlasDigits(Frame &f) {
    uint32_t digits = f.alloc(11 * 12);
    for (uint32_t k = 0; k < 11; k++) strInit(digits + 12 * k, STR_AtlasDigits + 2 * k, 1);
    return digits;
}
void freeAtlasDigits(uint32_t digits) {
    for (int k = 10; k >= 0; k--) strFree(digits + 12 * uint32_t(k));
}
// out = digit + out
void prependDigit(uint32_t out, uint32_t digit) {
    Frame f;
    uint32_t tmp = f.alloc(12);
    call(String_concat, {tmp, digit, out});
    strMoveAssign(out, tmp);
}
}  // namespace

// a pulsing yellow tint (a raised parameter)
void SceneUtil::setParameterUpEffect(Addr text) {
    uint32_t a = call(TintTo_create, {fbits(0.166f), 0xFF, 0xFF, 0xFF});
    uint32_t b = call(TintTo_create, {fbits(0.166f), 0xFF, 0xC7, 0});
    uint32_t loop = call(RepeatForever_create, {call(fn::Sequence_create, {a, b, 0})});
    call(fn::Node_runAction, {text, loop});
}
RW_FUNCTION(0x465994, "SceneUtil::setParameterUpEffect", SceneUtil::setParameterUpEffect);

void SceneUtil::setStatusChangeColor(Addr text) { setColor(text, 0xFF, 0xEA, 0); }
RW_FUNCTION(0x4659F0, "SceneUtil::setStatusChangeColor", SceneUtil::setStatusChangeColor);

void SceneUtil::setStatusCountStopColor(Addr text) { setColor(text, 0xFF, 0x84, 0); }
RW_FUNCTION(0x465A54, "SceneUtil::setStatusCountStopColor", SceneUtil::setStatusCountStopColor);

void SceneUtil::setNormalColor(Addr text) { vcall(text, node::setColor, {text, COLOR3B_WHITE}); }
RW_FUNCTION(0x468668, "SceneUtil::setNormalColor", SceneUtil::setNormalColor);

void SceneUtil::setNeedMedalColor(Addr text) { vcall(text, node::setColor, {text, COLOR3B_RED}); }
RW_FUNCTION(0x468688, "SceneUtil::setNeedMedalColor", SceneUtil::setNeedMedalColor);

void SceneUtil::setWarningColor(Addr text) { vcall(text, node::setColor, {text, COLOR3B_RED}); }
RW_FUNCTION(0x4686A8, "SceneUtil::setWarningColor", SceneUtil::setWarningColor);

void SceneUtil::setNormalTextFieldColor(Addr field) { vcall(field, node::setColor, {field, COLOR3B_WHITE}); }
RW_FUNCTION(0x4686C8, "SceneUtil::setNormalTextFieldColor", SceneUtil::setNormalTextFieldColor);

void SceneUtil::setFocusTextFieldColor(Addr field) { setColor(field, 0xFF, 0xEA, 0); }
RW_FUNCTION(0x4686E8, "SceneUtil::setFocusTextFieldColor", SceneUtil::setFocusTextFieldColor);

// enable / disable a button and grey its label
void SceneUtil::setButtonEnabled(Addr button, Addr label, uint32_t enabled, uint32_t style) {
    call(Widget_setBright, {button, enabled});
    vcall(button, Widget_setTouchEnabled, {button, enabled});
    call(ButtonLabelColor, {label, enabled, style});
}
RW_FUNCTION(0x468624, "SceneUtil::setButtonEnabled", SceneUtil::setButtonEnabled);

void SceneUtil::setImageButtonEnabled(Addr button, Addr label, uint32_t enabled, uint32_t style) {
    call(ImageButton_setIsDisabled, {button, enabled ^ 1u});
    call(ButtonLabelColor, {label, enabled, style});
}
RW_FUNCTION(0x474614, "SceneUtil::setButtonEnabled(ImageButton)", SceneUtil::setImageButtonEnabled);

// A text wider than `width` scrolls right to left forever; a shorter one just sits at `restX`.
void SceneUtil::runTicker(Addr node, float width, float startX, float restX) {
    float textWidth = ldf(vcall(node, node::getContentSize, {node}));
    float x = bitsf(vcall(node, node::getPositionX, {node}));
    call(Node_stopAllActions, {node});
    float span = textWidth + 15.0f;
    float endX = x - span;
    bool fits = span <= width || std::isnan(span) || std::isnan(width);
    float duration = (width / 1136.0f) * 17.0f;
    if (fits) {
        vcall(node, node::setPositionX, {node, fbits(restX)});
        return;
    }
    vcall(node, node::setPositionX, {node, fbits(startX)});
    Frame f;
    uint32_t p1 = newVec2(f, restX, bitsf(vcall(node, node::getPositionY, {node})));
    uint32_t enter = call(fn::MoveTo_create, {fbits(1.0f), p1});
    uint32_t wait = call(fn::DelayTime_create, {fbits(1.0f)});
    uint32_t p2 = newVec2(f, endX, bitsf(vcall(node, node::getPositionY, {node})));
    uint32_t scroll = call(fn::MoveTo_create, {fbits(duration), p2});
    uint32_t p3 = newVec2(f, startX, bitsf(vcall(node, node::getPositionY, {node})));
    uint32_t reset = call(fn::MoveTo_create, {fbits(0.0f), p3});
    uint32_t loop = call(RepeatForever_create, {call(fn::Sequence_create, {enter, wait, scroll, reset, 0})});
    call(fn::Node_runAction, {node, loop});
}
RW_FUNCTION(0x465AB8, "SceneUtil::runTicker", SceneUtil::runTicker);

// the support medal sparkle (an LWF animation named "supportLWF"), created on first use
void SceneUtil::showSupportMedalEffect(Addr widget, uint32_t small) {
    uint32_t effect = findNode(widget, STR_supportLWF, 10);
    if (!effect) {
        effect = call(LWFNode_create, {STR_SupportMedalLwf, 0});
        float w = ldf(vcall(widget, node::getContentSize, {widget}));
        float h = ldf(vcall(widget, node::getContentSize, {widget}) + 4);
        Frame f;
        vcall(effect, node::setPosition, {effect, newVec2(f, w * 0.5f, h * 0.5f)});
        uint32_t name = newStr(f, STR_supportLWF, 10);
        vcall(widget, node::addChildZName, {widget, effect, 0, name});
        strFree(name);
    }
    vcall(effect, node::setVisible, {effect, 1});
    vcall(effect, node::setScale, {effect, small == 1 ? fbits(0.28f) : fbits(1.0f)});
}
RW_FUNCTION(0x46695C, "SceneUtil::showSupportMedalEffect", SceneUtil::showSupportMedalEffect);

void SceneUtil::hideSupportMedalEffect(Addr widget, uint32_t small) {
    (void)small;
    uint32_t effect = findNode(widget, STR_supportLWF, 10);
    if (effect) vcall(effect, node::setVisible, {effect, 0});
}
RW_FUNCTION(0x466B08, "SceneUtil::hideSupportMedalEffect", SceneUtil::hideSupportMedalEffect);

// letter spacing for an atlas label (once: only while it has none); a centred label is shifted by half of it
void SceneUtil::setAdditionalKerningForAtlasLabel(Addr atlas, int32_t kerning) {
    uint32_t label = vcall(atlas, Text_getVirtualRenderer, {atlas});
    if (bitsf(call(Label_getAdditionalKerning, {label})) > 0.0f) return;
    float k = float(kerning);
    call(Label_setAdditionalKerning, {label, fbits(k)});
    if (ldf(vcall(atlas, node::getAnchorPoint, {atlas})) == 0.5f) {
        float x = bitsf(vcall(atlas, node::getPositionX, {atlas}));
        vcall(atlas, node::setPositionX, {atlas, fbits(k * 0.5f + x)});
    }
}
RW_FUNCTION(0x466BA8, "SceneUtil::setAdditionalKerningForAtlasLabel", SceneUtil::setAdditionalKerningForAtlasLabel);

// a number in the atlas font's letters ('a' = 0 ... 'j' = 9); negative numbers show 0
void SceneUtil::numberToAtlasString(Addr out, int32_t value) {
    int32_t v = value >= 0 ? value : 0;
    Frame f;
    uint32_t digits = atlasDigits(f);
    strInit(out, STR_Empty, 0);
    int32_t old;
    do {
        old = v;
        prependDigit(out, digits + 12 * uint32_t(v % 10));
        v = v / 10;
    } while (uint32_t(old + 9) > 18);
    freeAtlasDigits(digits);
}
RW_FUNCTION(0x466C40, "SceneUtil::numberToAtlasString", SceneUtil::numberToAtlasString);

void SceneUtil::numberToAtlasString64(Addr out, uint64_t value) {
    Frame f;
    uint32_t digits = atlasDigits(f);
    strInit(out, STR_Empty, 0);
    bool more;
    do {
        prependDigit(out, digits + 12 * uint32_t(value % 10));
        more = value > 9;
        value /= 10;
    } while (more);
    freeAtlasDigits(digits);
}
RW_FUNCTION(0x470F28, "SceneUtil::numberToAtlasString(unsigned long long)", SceneUtil::numberToAtlasString64);

// "1st", "2nd", "3rd", "4th"... (the suffix texts; 11-13 take "th")
void SceneUtil::numberToPlaceString(Addr out, int32_t value) {
    strInit(out, STR_Empty, 0);
    int32_t v = value >= 0 ? value : 0;
    int32_t last = v % 10;
    bool teen = (v / 10) % 10 == 1;
    int32_t id = 100220004;
    if (last == 3) id = teen ? 100220004 : 100220003;
    else if (last == 2) id = teen ? 100220004 : 100220002;
    else if (last == 1) id = teen ? 100220004 : 100220001;
    Frame f;
    uint32_t text = textData(f, id);
    uint32_t s = textDataStr(text);
    call(fn::string_assign, {out, s, host_strlen(s)});
    sharedRelease(text);
}
RW_FUNCTION(0x473688, "SceneUtil::numberToPlaceString", SceneUtil::numberToPlaceString);
