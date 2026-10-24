// SceneUtil: node lookup, layout against the visible area, in/out animations, text styling.
#include "SceneUtil.hpp"

using namespace scene;

namespace {
constexpr float DESIGN_WIDTH = 960.0f;
constexpr float DESIGN_HEIGHT = 640.0f;
constexpr float SLIDE_TIME = 0.15f;      // added to every slide duration
constexpr float SLIDE_DELAY = 0.032f;    // pause after a slide, before the callback

// cocos2d / game functions
constexpr uint32_t SceneReader_getInstance = rwlib::cocostudio::SceneReader::getInstance;
constexpr uint32_t SceneReader_createNodeWithSceneFile = rwlib::cocostudio::SceneReader::createNodeWithSceneFile;   // (reader, name, AttachComponentType)
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__v;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;                             // Rect(x, y, w, h)
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t TouchFunction_swap = 0x483E4D;                    // std::function<bool(Touch*,Event*)>::swap
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t EaseSineInOut_create = rwlib::cocos2d::EaseSineInOut::create;
constexpr uint32_t EaseIn_create = rwlib::cocos2d::EaseIn::create;                         // (action, float rate)
constexpr uint32_t FadeIn_create = rwlib::cocos2d::FadeIn::create;
constexpr uint32_t FadeOut_create = rwlib::cocos2d::FadeOut::create;
constexpr uint32_t Color4B_ctor = rwlib::cocos2d::Color4B::ctor_base;                          // (out, r, g, b, a)
constexpr uint32_t Text_enableShadow = rwlib::cocos2d::ui::Text::enableShadow;                     // (text, const Color4B&, const Size&, blur)
constexpr uint32_t Text_enableOutline = rwlib::cocos2d::ui::Text::enableOutline;                    // (text, const Color4B&, size)
constexpr uint32_t Text_getFontSize = rwlib::cocos2d::ui::Text::getFontSize;
constexpr uint32_t Text_getFontName = rwlib::cocos2d::ui::Text::getFontName;
constexpr uint32_t Label_setAdditionalKerning = rwlib::cocos2d::Label::setAdditionalKerning;
constexpr uint32_t std_stof = 0xC949FD;                              // std::stof(const string&, size_t*)
constexpr uint32_t CustomTextView_create = 0x4F9165;
constexpr uint32_t CustomTextView_setFontSize = 0x4F9315;
constexpr uint32_t CustomTextView_setFont = 0x4F92E9;                // (view, std::string by value)
constexpr uint32_t CustomTextView_setFontColor = 0x4F92F9;           // (view, Color3B packed in a register)

constexpr uint32_t VT_ShadowSpriteTouchLambda = 0xF55270;            // __func<createShadowSprite()::$_0>
constexpr uint32_t Sprite_setTextureRect = 0x284;                    // Sprite vtable slot (const Rect&)
constexpr uint32_t Text_getVirtualRenderer = 0x2D8;                  // Widget vtable slot -> Label*
constexpr uint32_t Director_eventDispatcher = 0x84;                  // Director field
constexpr uint32_t Listener_onTouchBegan = 0x50;                     // EventListenerTouchOneByOne field

constexpr uint32_t STR_LeftUI = 0xDF85F0;
constexpr uint32_t STR_RightUI = 0xDF85F7;
constexpr int32_t TEXT_USER_TITLE_KERNING = 100400004;              // TextData type 2: kerning of user titles

// a node's content size, or its first child's when it has children (the slide distance of a panel)
Size slideSize(uint32_t node) {
    uint32_t cs = vcall(node, node::getContentSize, {node});
    Size size{ldf(cs), ldf(cs + 4)};
    if (int32_t(vcall(node, node::getChildrenCount, {node})) >= 1) {
        uint32_t children = vcall(node, node::getChildren, {node});
        uint32_t first = ld32(ld32(children));
        uint32_t fs = vcall(first, node::getContentSize, {first});
        size = Size{ldf(fs), ldf(fs + 4)};
    }
    return size;
}

// node position copied (getPosition returns a reference)
Vec2 position(uint32_t node) { return readVec2(vcall(node, node::getPosition, {node})); }

void setPosition(uint32_t node, float x, float y) {
    Frame f;
    vcall(node, node::setPosition, {node, newVec2(f, x, y)});
}

// MoveTo(duration, target) together with FadeTo(duration, opacity), then a short pause and the callback
uint32_t slideSequence(float duration, Vec2 target, uint32_t opacity, uint32_t done) {
    Frame f;
    uint32_t to = newVec2(f, target.x, target.y);
    uint32_t move = call(fn::MoveTo_create, {fbits(duration), to});
    uint32_t fade = call(fn::FadeTo_create, {fbits(duration), opacity});
    uint32_t spawn = call(fn::Spawn_create, {move, fade, 0});
    uint32_t delay = call(fn::DelayTime_create, {fbits(SLIDE_DELAY)});
    uint32_t callback = call(fn::CallFunc_create, {done});
    return call(fn::Sequence_create, {spawn, delay, callback, 0});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ nodes

SceneUtil::Addr SceneUtil::searchNodeByName(Addr root, Addr name) {
    uint32_t found = vcall(root, node::getChildByName, {root, name});
    if (found) return found;
    uint32_t children = vcall(root, node::getChildren, {root});
    uint32_t it = ld32(children), end = ld32(children + 4);
    for (; it != end; it += 4) {
        found = searchNodeByName(ld32(it), name);
        if (found) return found;
    }
    return 0;
}
RW_FUNCTION(0x45A9F4, "SceneUtil::searchNodeByName", SceneUtil::searchNodeByName);

SceneUtil::Addr SceneUtil::createNodeWithSceneFile(Addr fileName) {
    uint32_t root = call(SceneReader_createNodeWithSceneFile, {call(SceneReader_getInstance, {}), fileName, 0});
    if (!root) return 0;
    vcall(root, node::setAnchorPoint, {root, 0xFEF5C0 /* Vec2::ANCHOR_MIDDLE */});
    Vec2 origin = visibleOrigin();
    Size size = visibleSize();
    setPosition(root, origin.x + size.width * 0.5f, origin.y + size.height * 0.5f);
    Frame f;
    uint32_t name = newStr(f, STR_LeftUI, 6);
    uint32_t left = searchNodeByName(root, name);
    strFree(name);
    if (left) leftAdsorptionNode(left);
    name = newStr(f, STR_RightUI, 7);
    uint32_t right = searchNodeByName(root, name);
    strFree(name);
    if (right) rightAdsorptionNode(right);
    return root;
}
RW_FUNCTION(0x45AA48, "SceneUtil::createNodeWithSceneFile", SceneUtil::createNodeWithSceneFile);

void SceneUtil::leftAdsorptionNode(Addr node) {
    float x = bitsf(vcall(node, node::getPositionX, {node}));
    Size size = visibleSize();
    vcall(node, node::setPositionX, {node, fbits(x + (size.width + -DESIGN_WIDTH) * -0.5f)});
}
RW_FUNCTION(0x45ABFC, "SceneUtil::leftAdsorptionNode", SceneUtil::leftAdsorptionNode);

void SceneUtil::rightAdsorptionNode(Addr node) {
    float x = bitsf(vcall(node, node::getPositionX, {node}));
    Size size = visibleSize();
    vcall(node, node::setPositionX, {node, fbits(x + (size.width + -DESIGN_WIDTH) * 0.5f)});
}
RW_FUNCTION(0x45AC84, "SceneUtil::rightAdsorptionNode", SceneUtil::rightAdsorptionNode);

float SceneUtil::getSideMargin() { return (visibleSize().width + -DESIGN_WIDTH) * 0.5f; }
RW_FUNCTION(0x45AD0C, "SceneUtil::getSideMargin", SceneUtil::getSideMargin);

float SceneUtil::getSideHeightMargin() { return (visibleSize().height + -DESIGN_HEIGHT) * 0.5f; }
RW_FUNCTION(0x45AF58, "SceneUtil::getSideHeightMargin", SceneUtil::getSideHeightMargin);

SceneUtil::Addr SceneUtil::createShadowSprite() {
    uint32_t sprite = call(Sprite_create, {});
    Frame f;
    uint32_t black = f.alloc(4);                       // Color3B(0, 0, 0)
    st8(black, 0); st8(black + 1, 0); st8(black + 2, 0);
    vcall(sprite, node::setColor, {sprite, black});
    vcall(sprite, node::setOpacity, {sprite, 128});
    Vec2 origin = visibleOrigin();
    Size size = visibleSize();
    // the rect's size is origin + size (the original's arithmetic)
    uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, fbits(origin.x), fbits(origin.y), fbits(origin.x + size.width),
                     fbits(origin.y + size.height)});
    vcall(sprite, Sprite_setTextureRect, {sprite, rect});
    setPosition(sprite, origin.x + size.width * 0.5f, origin.y + size.height * 0.5f);
    uint32_t dispatcher = ld32(director() + Director_eventDispatcher);
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    // listener->onTouchBegan = [sprite](Touch*, Event*) {...}
    uint32_t onBegan = newFunction(f, VT_ShadowSpriteTouchLambda, {sprite});
    call(TouchFunction_swap, {onBegan, listener + Listener_onTouchBegan});
    functionDestroy(onBegan);
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, sprite});
    return sprite;
}
RW_FUNCTION(0x45AD68, "SceneUtil::createShadowSprite", SceneUtil::createShadowSprite);

void SceneUtil::backBtnSetPosY(Addr button) {
    float y = bitsf(vcall(button, node::getPositionY, {button}));
    Size size = visibleSize();
    vcall(button, node::setPositionY, {button, fbits(y + (size.height + -DESIGN_HEIGHT) * 0.5f)});
}
RW_FUNCTION(0x45AFB4, "SceneUtil::backBtnSetPosY", SceneUtil::backBtnSetPosY);

void SceneUtil::setCascadeOpacityEnabled(Addr node, uint32_t enabled) {
    vcall(node, node::setCascadeOpacityEnabled, {node, enabled});
    if (int32_t(vcall(node, node::getChildrenCount, {node})) < 1) return;
    uint32_t it = ld32(vcall(node, node::getChildren, {node}));
    // the end is re-read after every child (the recursion could change the children)
    while (it != ld32(vcall(node, node::getChildren, {node}) + 4)) {
        setCascadeOpacityEnabled(ld32(it), enabled);
        it += 4;
    }
}
RW_FUNCTION(0x45B16C, "SceneUtil::setCascadeOpacityEnabled", SceneUtil::setCascadeOpacityEnabled);

// ------------------------------------------------------------------------------------------------ animations
// "In": the node is placed off screen (and transparent), then slides to where it was while fading in.
// "Out": the node slides off screen while fading out.

SceneUtil::Addr SceneUtil::createTopInAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    float top = bitsf(call(fn::AppGetGrameFrameHeight, {}));
    setPosition(node, home.x, top + offset);
    vcall(node, node::setOpacity, {node, 0});
    return slideSequence(duration + SLIDE_TIME, home, 255, done);
}
RW_FUNCTION(0x45B03C, "SceneUtil::createTopInAnimation", SceneUtil::createTopInAnimation);

SceneUtil::Addr SceneUtil::createTopOutAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    float time = duration + SLIDE_TIME;
    float top = bitsf(call(fn::AppGetGrameFrameHeight, {}));
    return slideSequence(time, Vec2{home.x, top + offset}, 0, done);
}
RW_FUNCTION(0x45B1DC, "SceneUtil::createTopOutAnimation", SceneUtil::createTopOutAnimation);

SceneUtil::Addr SceneUtil::createBottomInAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    Size size = slideSize(node);
    setPosition(node, home.x, offset - size.height);
    vcall(node, node::setOpacity, {node, 0});
    return slideSequence(duration + SLIDE_TIME, home, 255, done);
}
RW_FUNCTION(0x45B2F8, "SceneUtil::createBottomInAnimation", SceneUtil::createBottomInAnimation);

SceneUtil::Addr SceneUtil::createBottomOutAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    Size size = slideSize(node);
    return slideSequence(duration + SLIDE_TIME, Vec2{home.x, offset - size.height}, 0, done);
}
RW_FUNCTION(0x45B468, "SceneUtil::createBottomOutAnimation", SceneUtil::createBottomOutAnimation);

SceneUtil::Addr SceneUtil::createLeftInAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    Size size = slideSize(node);
    setPosition(node, offset - size.width, home.y);
    vcall(node, node::setOpacity, {node, 0});
    return slideSequence(duration + SLIDE_TIME, home, 255, done);
}
RW_FUNCTION(0x45B5C4, "SceneUtil::createLeftInAnimation", SceneUtil::createLeftInAnimation);

SceneUtil::Addr SceneUtil::createLeftOutAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    Size size = slideSize(node);
    float time = duration + SLIDE_TIME;
    Size visible = visibleSize();
    float x = ((visible.width + -DESIGN_WIDTH) * -0.5f - size.width) + offset;
    return slideSequence(time, Vec2{x, home.y}, 0, done);
}
RW_FUNCTION(0x45B734, "SceneUtil::createLeftOutAnimation", SceneUtil::createLeftOutAnimation);

SceneUtil::Addr SceneUtil::createRightInAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    Vec2 origin = visibleOrigin();
    Size visible = visibleSize();
    float width = ldf(vcall(node, node::getContentSize, {node}));
    setPosition(node, ((origin.x + visible.width) + width) + offset, home.y);
    vcall(node, node::setOpacity, {node, 0});
    return slideSequence(duration + SLIDE_TIME, home, 255, done);
}
RW_FUNCTION(0x45B8BC, "SceneUtil::createRightInAnimation", SceneUtil::createRightInAnimation);

SceneUtil::Addr SceneUtil::createRightOutAnimation(Addr node, float offset, float duration, Addr done) {
    setCascadeOpacityEnabled(node, 1);
    Vec2 home = position(node);
    Vec2 origin = visibleOrigin();
    Size visible = visibleSize();
    float time = duration + SLIDE_TIME;
    float width = ldf(vcall(node, node::getContentSize, {node}));
    return slideSequence(time, Vec2{((origin.x + visible.width) + width) + offset, home.y}, 0, done);
}
RW_FUNCTION(0x45BA3C, "SceneUtil::createRightOutAnimation", SceneUtil::createRightOutAnimation);

// a quick pulse: scale up 10 %, back, then the callback
SceneUtil::Addr SceneUtil::createTapAnimation(Addr node, Addr done) {
    float scale = bitsf(vcall(node, node::getScale, {node}));
    uint32_t up = call(fn::ScaleTo_create, {fbits(0.1f), fbits(scale * 1.1f)});
    uint32_t back = call(fn::ScaleTo_create, {fbits(0.05f), fbits(scale)});
    uint32_t callback = call(fn::CallFunc_create, {done});
    return call(fn::Sequence_create, {up, back, callback, 0});
}
RW_FUNCTION(0x45BBA8, "SceneUtil::createTapAnimation", SceneUtil::createTapAnimation);

// grows the popup from nothing to its current scale
SceneUtil::Addr SceneUtil::createOpenPopupAnimation(Addr node, Addr done) {
    uint32_t scale = vcall(node, node::getScale, {node});
    vcall(node, node::setScale, {node, fbits(0.0f)});
    uint32_t grow = call(EaseSineInOut_create, {call(fn::ScaleTo_create, {fbits(0.15f), scale})});
    uint32_t callback = call(fn::CallFunc_create, {done});
    return call(fn::Sequence_create, {grow, callback, 0});
}
RW_FUNCTION(0x45BC04, "SceneUtil::createOpenPopupAnimation", SceneUtil::createOpenPopupAnimation);

SceneUtil::Addr SceneUtil::createClosePopupAnimation(Addr node, Addr done) {
    (void)node;
    uint32_t shrink = call(EaseIn_create, {call(fn::ScaleTo_create, {fbits(0.15f), fbits(0.0f)}), fbits(7.0f)});
    uint32_t callback = call(fn::CallFunc_create, {done});
    return call(fn::Sequence_create, {shrink, callback, 0});
}
RW_FUNCTION(0x45BC60, "SceneUtil::createClosePopupAnimation", SceneUtil::createClosePopupAnimation);

SceneUtil::Addr SceneUtil::createFadeInAnimation(Addr node, Addr done) {
    vcall(node, node::setOpacity, {node, 0});
    uint32_t fade = call(EaseSineInOut_create, {call(FadeIn_create, {fbits(0.15f)})});
    uint32_t callback = call(fn::CallFunc_create, {done});
    return call(fn::Sequence_create, {fade, callback, 0});
}
RW_FUNCTION(0x45BCA0, "SceneUtil::createFadeInAnimation", SceneUtil::createFadeInAnimation);

SceneUtil::Addr SceneUtil::createFadeOutAnimation(Addr node, Addr done) {
    (void)node;
    uint32_t fade = call(EaseSineInOut_create, {call(FadeOut_create, {fbits(0.15f)})});
    uint32_t callback = call(fn::CallFunc_create, {done});
    return call(fn::Sequence_create, {fade, callback, 0});
}
RW_FUNCTION(0x45BCE4, "SceneUtil::createFadeOutAnimation", SceneUtil::createFadeOutAnimation);

// ------------------------------------------------------------------------------------------------ text styling

// drop shadow: black at 50 %, offset (2, -2), blur 6
void SceneUtil::setShadow(Addr text) {
    Frame f;
    uint32_t color = f.alloc(4);
    st8(color, 0); st8(color + 1, 0); st8(color + 2, 0); st8(color + 3, 0x80);
    uint32_t offset = newVec2(f, 2.0f, -2.0f);
    call(Text_enableShadow, {text, color, offset, 6});
}
RW_FUNCTION(0x45BD14, "SceneUtil::setShadow", SceneUtil::setShadow);

// black 2px outline, letter spacing from the text table
void SceneUtil::setUserTitleOutline(Addr text) {
    Frame f;
    uint32_t color = f.alloc(4);
    st8(color, 0); st8(color + 1, 0); st8(color + 2, 0); st8(color + 3, 0xFF);
    call(Text_enableOutline, {text, color, 2});
    uint32_t entry = f.alloc(8);                       // shared_ptr<TextData entry>
    call(fn::TextData_get, {entry, 2, uint32_t(TEXT_USER_TITLE_KERNING)});
    uint32_t value = newStr(f, ld32(ld32(entry) + 4));
    uint32_t kerning = call(std_stof, {value, 0});
    strFree(value);
    sharedRelease(entry);
    uint32_t label = vcall(text, Text_getVirtualRenderer, {text});
    call(Label_setAdditionalKerning, {label, kerning});
}
RW_FUNCTION(0x45BD94, "SceneUtil::setUserTitleOutline", SceneUtil::setUserTitleOutline);

void SceneUtil::setBlackOutline(Addr text) {
    Frame f;
    uint32_t color = f.alloc(4);
    st8(color, 0); st8(color + 1, 0); st8(color + 2, 0); st8(color + 3, 0xFF);
    call(Text_enableOutline, {text, color, 2});
    uint32_t label = vcall(text, Text_getVirtualRenderer, {text});
    call(Label_setAdditionalKerning, {label, fbits(-4.0f)});
}
RW_FUNCTION(0x45BE94, "SceneUtil::setBlackOutline", SceneUtil::setBlackOutline);

SceneUtil::Addr SceneUtil::textToCustomTextView(Addr text, Addr name) {
    uint32_t view = call(CustomTextView_create, {});
    vcall(view, node::setPosition, {view, vcall(text, node::getPosition, {text})});
    vcall(view, node::setAnchorPoint, {view, vcall(text, node::getAnchorPoint, {text})});
    vcall(view, node::setContentSize, {view, vcall(text, node::getContentSize, {text})});
    call(CustomTextView_setFontSize, {view, call(Text_getFontSize, {text})});
    // setFont takes the string by value: copy the text's font name
    uint32_t fontName = call(Text_getFontName, {text});
    Frame f;
    uint32_t font = f.alloc(12);
    if (strIsLong(fontName)) {
        strInit(font, ld32(fontName + 8), ld32(fontName + 4));
    } else {
        st32(font, ld32(fontName)); st32(font + 4, ld32(fontName + 4)); st32(font + 8, ld32(fontName + 8));
    }
    call(CustomTextView_setFont, {view, font});
    strFree(font);
    uint32_t color = vcall(text, node::getColor, {text});
    call(CustomTextView_setFontColor,
         {view, uint32_t(ld8(color)) | uint32_t(ld8(color + 1)) << 8 | uint32_t(ld8(color + 2)) << 16});
    // put the view where the text was (same z order, given name), then drop the text
    uint32_t parent = vcall(text, node::getParent, {text});
    uint32_t z = vcall(text, 0x18 /* getLocalZOrder */, {text});
    vcall(parent, node::addChildZName, {parent, view, z, name});
    parent = vcall(text, node::getParent, {text});
    vcall(parent, node::removeChild, {parent, text, 1});
    return view;
}
RW_FUNCTION(0x45BF14, "SceneUtil::textToCustomTextView", SceneUtil::textToCustomTextView);
