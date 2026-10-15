#include "CustomTextView.hpp"

#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t AEABI_MEMCPY = 0x40E164;                // __aeabi_memcpy(dst, src, n)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t STRCASECMP = 0x41540C;
constexpr uint32_t FLOORF = 0x415244;
constexpr uint32_t STRING_ASSIGN = 0x48D811;               // std::string::assign(const char*, size_t)
constexpr uint32_t STRING_COPY_ASSIGN = 0x498A1D;          // std::string::operator=(const string&) (sub_498A1C)
constexpr uint32_t STRING_REPLACE = 0x4B6DCD;              // std::string::replace(pos, n, const char*, n2) (sub_4B6DCC)
constexpr uint32_t SEARCH = 0x49A42B;                      // std::__search<bool(*&)(char, char), const char*, ...>
constexpr uint32_t CHAR_EQ = 0x49A419;                     // char_traits<char>::eq (the search predicate)
constexpr uint32_t VECTOR_THROW_OUT_OF_RANGE = 0x47CCE1;   // __vector_base_common::__throw_out_of_range (sub_47CCE0)
constexpr uint32_t FLOAT_VECTOR_PUSH_BACK_SLOW = 0x4FA109;       // vector<float>::__push_back_slow_path(const float&)
constexpr uint32_t FLOAT_VECTOR_PUSH_BACK_SLOW_MOVE = 0x4F9FCD;  // vector<float>::__push_back_slow_path(float&&)
constexpr uint32_t FLOAT_VECTOR_DTOR = 0x4F59FF;           // std::__vector_base<float>::~__vector_base
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;           // StringBuffer::StringBuffer(fmt, ...)
constexpr uint32_t NODE_CTOR = 0x892531;                   // cocos2d::Node::Node()
constexpr uint32_t NODE_DTOR = 0x8927ED;                   // cocos2d::Node::~Node()
constexpr uint32_t NODE_SET_ANCHOR_POINT = 0x893375;       // Node::setAnchorPoint(const Vec2&)
constexpr uint32_t REF_AUTORELEASE = 0x8FD0D5;
constexpr uint32_t RICHTEXT_CREATE = 0x8404AD;             // ui::RichText::create()
constexpr uint32_t RICHTEXT_FORMAT_TEXT = 0x840839;        // ui::RichText::formatText()
constexpr uint32_t RICHTEXT_PUSH_BACK_ELEMENT = 0x84064D;  // ui::RichText::pushBackElement(RichElement*)
constexpr uint32_t RICH_ELEMENT_CUSTOM_NODE_CREATE = 0x840285;  // (tag, const Color3B&, opacity, Node*)
constexpr uint32_t RICHTEXT_VECTOR_CLEAR = 0x4F976D;       // cocos2d::Vector<ui::RichText*>::clear()
constexpr uint32_t RICHTEXT_VECTOR_PUSH_BACK = 0x4F97A1;   // cocos2d::Vector<ui::RichText*>::pushBack()
constexpr uint32_t RICHTEXT_VECTOR_DTOR = 0x4FA355;        // cocos2d::Vector<ui::RichText*>::~Vector()
constexpr uint32_t TEXT_CREATE = 0x839DED;                 // ui::Text::create()
constexpr uint32_t TEXT_SET_CONFIG = 0x83A0AD;             // ui::Text::setConfig(const Config&)
constexpr uint32_t SCENEUTIL_SET_SHADOW = 0x45BD15;        // SceneUtil::setShadow(ui::Text*)
constexpr uint32_t SAXPARSER_CTOR = 0x8D93B1;
constexpr uint32_t SAXPARSER_DTOR = 0x8D93B7;
constexpr uint32_t SAXPARSER_SET_DELEGATOR = 0x8D94ED;
constexpr uint32_t SAXPARSER_PARSE = 0x8D93BD;             // parse(const char* xml, size_t size)

constexpr uint32_t COLOR3B_WHITE = 0x1007E88;              // cocos2d::Color3B::WHITE
constexpr uint32_t COLOR3B_YELLOW = 0x1007E8B;             // cocos2d::Color3B::YELLOW
constexpr uint32_t VEC2_ANCHOR_MIDDLE_LEFT = 0xFEF5E8;     // cocos2d::Vec2::ANCHOR_MIDDLE_LEFT
constexpr uint32_t STR_DEFAULT_FONT = 0xDFCF46;            // "cocostudio/publish/PNGoIWA-HW-Md.TTF" (36 chars)
constexpr uint32_t STR_NEWLINE = 0xDF9E14;                 // the 1-char line break replaced by <br></br>
constexpr uint32_t STR_BR_TAG = 0xDFCF16;                  // "<br></br>"
constexpr uint32_t STR_XML_FORMAT = 0xDFCF20;              // "<xml>%s</xml>"
constexpr uint32_t STR_BIG = 0xDFCF2E, STR_MIDDLE = 0xDFCF32, STR_IMPORTANT = 0xDFCF39, STR_BR = 0xDFCF43;

// cocos2d::Node vtable byte offsets used here (besides scene::node)
constexpr uint32_t VSLOT_INIT = 0x248;
constexpr uint32_t VSLOT_DELETING_DTOR = 0x4;
constexpr uint32_t VSLOT_REMOVE_ALL_CHILDREN = 0x13C;

uint32_t addr(const void *p) { return rw::guest_addr(p); }

// the font size for a size kind: x1.1 (middle) or x1.2 (big), rounded down
int32_t scaledFontSize(int32_t size, int32_t kind) {
    float factor = kind == 1 ? 1.1f : 1.0f;
    if (kind == 2) factor = 1.2f;
    return f2i(rw::callf(FLOORF, {fbits(float(size) * factor)}));
}

// a Node's content size (getContentSize returns a const Size&)
uint32_t contentSize(uint32_t node) { return vcall(node, node::getContentSize, {node}); }

// std::vector<float>::push_back(value at `value`) as inlined: in place when there is room, else the slow path
// (the two call sites test the room differently: end != cap, or end < cap)
void pushHeight(CustomTextView *self, uint32_t value, uint32_t slowPath, bool lessThan) {
    uint32_t end = self->lineHeights.end;
    if (lessThan ? end < self->lineHeights.cap : end != self->lineHeights.cap) {
        uint32_t next = 0;
        if (end) {
            st32(end, ld32(value));
            next = self->lineHeights.end;
        }
        self->lineHeights.end = next + 4;
    } else {
        call(slowPath, {addr(&self->lineHeights), value});
    }
}
}  // namespace

uint32_t CustomTextView::create() {
    uint32_t p = call(fn::operator_new, {0x2C0});
    call(AEABI_MEMSET, {p, 0x2C0, 0});
    call(NODE_CTOR, {p});
    auto *self = rw::host_ptr<CustomTextView>(p);
    rw::set<uint32_t>(self, 0, VTABLE);
    self->saxVtable = SAX_VTABLE;
    self->hAlign = 1;
    self->totalHeight = 0.0f;
    call(AEABI_MEMSET, {addr(&self->fontSizeKind), 0x20, 0});
    self->fontSize = 30;
    self->fontColor[1] = rw::load<uint8_t>(COLOR3B_WHITE + 1);
    self->fontColor[0] = rw::load<uint8_t>(COLOR3B_WHITE);
    self->fontColor[2] = rw::load<uint8_t>(COLOR3B_WHITE + 2);
    self->currentColor[1] = rw::load<uint8_t>(COLOR3B_WHITE + 1);
    self->currentColor[0] = rw::load<uint8_t>(COLOR3B_WHITE);
    self->currentColor[2] = rw::load<uint8_t>(COLOR3B_WHITE + 2);
    // font = default font (a long std::string: capacity 0x31 | 1, size 36)
    uint32_t data = call(fn::operator_new, {0x30});
    const uint32_t font = addr(&self->font);
    st32(font + 8, data);
    st32(font, 0x31);
    st32(font + 4, 0x24);
    call(AEABI_MEMCPY, {data, STR_DEFAULT_FONT, 0x24});
    st8(data + 0x24, 0);
    self->lineSpacing = 10.0f;
    self->maxLineWidth = 0.0f;
    if (vcall(p, VSLOT_INIT, {p}) == 1) {
        call(REF_AUTORELEASE, {p});
        return p;
    }
    vcall(p, VSLOT_DELETING_DTOR, {p});
    return 0;
}
RW_FUNCTION(0x4F9164, "CustomTextView::create", CustomTextView::create);

uint32_t CustomTextView::init(CustomTextView *) { return 1; }
RW_FUNCTION(0x4F92D4, "CustomTextView::init", CustomTextView::init);

void CustomTextView::setHAlign(CustomTextView *self, int32_t align) { self->hAlign = align; }
RW_FUNCTION(0x4F92D8, "CustomTextView::setHAlign", CustomTextView::setHAlign);

void CustomTextView::setLineSpacing(CustomTextView *self, float spacing) { self->lineSpacing = spacing; }
RW_FUNCTION(0x4F92E0, "CustomTextView::setLineSpacing", CustomTextView::setLineSpacing);

// setFont(std::string font): font = *font (the by-value argument is the caller's to destroy)
void CustomTextView::setFont(CustomTextView *self, uint32_t font) { call(STRING_COPY_ASSIGN, {addr(&self->font), font}); }
RW_FUNCTION(0x4F92E8, "CustomTextView::setFont", CustomTextView::setFont);

// setFontColor(Color3B): the three bytes travel packed in one register
void CustomTextView::setFontColor(CustomTextView *self, uint32_t color) {
    self->fontColor[1] = uint8_t(color >> 8);
    self->fontColor[0] = uint8_t(color);
    self->fontColor[2] = uint8_t(color >> 16);
}
RW_FUNCTION(0x4F92F8, "CustomTextView::setFontColor", CustomTextView::setFontColor);

void CustomTextView::setFontSize(CustomTextView *self, int32_t size) { self->fontSize = size; }
RW_FUNCTION(0x4F9314, "CustomTextView::setFontSize", CustomTextView::setFontSize);

// setXml(text): rebuild the lines from the markup in `text`.
void CustomTextView::setXml(CustomTextView *self, uint32_t source) {
    const uint32_t me = addr(self);
    // lineHeights.clear() (inlined: end moves back by the used bytes)
    uint32_t begin = self->lineHeights.begin, end = self->lineHeights.end;
    if (end != begin) self->lineHeights.end = end + ~((end - 4 - begin) | 3u);
    call(RICHTEXT_VECTOR_CLEAR, {addr(&self->richTexts)});
    self->lineMaxFontSize = 0.0f;
    vcall(me, VSLOT_REMOVE_ALL_CHILDREN, {me});
    self->totalHeight = 0.0f;
    self->currentColor[1] = self->fontColor[1];
    self->currentColor[0] = self->fontColor[0];
    self->currentColor[2] = self->fontColor[2];
    self->fontSizeKind = 0;
    // the first line
    uint32_t line = call(RICHTEXT_CREATE, {});
    self->currentRichText = line;
    uint32_t setPositionX = ld32(ld32(line) + node::setPositionX);
    float width = ldf(contentSize(me));
    call(setPositionX, {line, fbits(width * 0.5f)});
    vcall(me, node::addChild, {me, self->currentRichText});
    call(RICHTEXT_VECTOR_PUSH_BACK, {addr(&self->richTexts), self->currentRichText});
    uint32_t parser = call(fn::operator_new, {4});
    call(SAXPARSER_CTOR, {parser});

    Frame f;
    const uint32_t text = f.alloc(12), needle = f.alloc(12), br = f.alloc(12), moved = f.alloc(12);
    const uint32_t xml = f.alloc(12), pred = f.alloc(8), buffer = f.alloc(0x410);
    if (ld8(source) & 1) {
        strInit(text, ld32(source + 8), ld32(source + 4));
    } else {
        st32(text, ld32(source));
        st32(text + 4, ld32(source + 4));
        st32(text + 8, ld32(source + 8));
    }
    strInit(needle, STR_NEWLINE, 1);
    strInit(br, STR_BR_TAG, 9);
    // text: replace every line break with <br></br> (std::string::find + replace, inlined)
    uint32_t pos = 0;
    bool found;
    {
        uint32_t size = strSize(text), data = strData(text);
        uint32_t nsize = strSize(needle), ndata = strData(needle);
        if (size < nsize) {
            found = false;
        } else if (nsize == 0) {
            found = true;
        } else {
            st32(pred, CHAR_EQ);
            uint32_t hit = call(SEARCH, {data, data + size, ndata, ndata + nsize, pred});
            found = hit != data + size && hit - data != 0xFFFFFFFFu;
            if (found) pos = hit - data;
        }
    }
    while (found) {
        uint32_t nsize = strSize(needle);
        call(STRING_REPLACE, {text, pos, nsize, strData(br), strSize(br)});
        pos += strSize(br);
        uint32_t size = strSize(text), data = strData(text);
        nsize = strSize(needle);
        uint32_t ndata = strData(needle);
        if (size < pos || size - pos < nsize) break;
        if (nsize != 0) {
            st32(pred, CHAR_EQ);
            uint32_t hit = call(SEARCH, {data + pos, data + size, ndata, ndata + nsize, pred});
            if (hit == data + size) break;
            pos = hit - data;
        }
        if (pos == 0xFFFFFFFFu) break;
    }
    // xml = "<xml>" + text + "</xml>"
    st32(moved, ld32(text));
    st32(moved + 4, ld32(text + 4));
    st32(moved + 8, ld32(text + 8));
    st32(text, 0);
    st32(text + 4, 0);
    st32(text + 8, 0);
    call(STRINGBUFFER_CTOR, {buffer, STR_XML_FORMAT, strData(moved)});
    strInit(xml, buffer, call(STRLEN, {buffer}));
    strFree(moved);
    strFree(br);
    strFree(needle);
    strFree(text);
    call(SAXPARSER_SET_DELEGATOR, {parser, addr(&self->saxVtable)});
    call(SAXPARSER_PARSE, {parser, strData(xml), strSize(xml)});
    call(SAXPARSER_DTOR, {parser});
    call(fn::operator_delete, {parser});
    pushHeight(self, addr(&self->lineMaxFontSize), FLOAT_VECTOR_PUSH_BACK_SLOW, false);   // the last line
    updateLayout(self);
    strFree(xml);
}
RW_FUNCTION(0x4F931C, "CustomTextView::setXml", CustomTextView::setXml);

// updateLayout(): centre the lines vertically (each at its height's middle), align them horizontally, and
// measure the widest line.
void CustomTextView::updateLayout(CustomTextView *self) {
    const uint32_t me = addr(self);
    self->maxLineWidth = 0.0f;
    float height = ldf(contentSize(me) + 4);
    float total = 0.0f;
    {
        uint32_t p = self->lineHeights.begin;
        uint32_t n = uint32_t(int32_t(self->lineHeights.end - p) >> 2);
        for (uint32_t k = 0; k < n; k++, p += 4) total = total + ldf(p);
    }
    if (int32_t(self->richTexts.end - self->richTexts.begin) < 1) return;
    float y = height + (height - total) * -0.5f;
    for (uint32_t i = 0;; i++) {
        uint32_t line = ld32(self->richTexts.begin + i * 4);
        call(RICHTEXT_FORMAT_TEXT, {line});
        uint32_t getContentSize = ld32(ld32(line) + node::getContentSize);
        float widest = self->maxLineWidth;
        float width = ldf(call(getContentSize, {line}));
        if (widest < width) self->maxLineWidth = ldf(vcall(line, node::getContentSize, {line}));
        // lineHeights.at(i)
        uint32_t heights = self->lineHeights.begin;
        if (uint32_t(int32_t(self->lineHeights.end - heights) >> 2) <= i)
            call(VECTOR_THROW_OUT_OF_RANGE, {addr(&self->lineHeights)});
        vcall(line, node::setPositionY, {line, fbits(y + ldf(heights + i * 4) * -0.5f)});
        heights = self->lineHeights.begin;
        if (uint32_t(int32_t(self->lineHeights.end - heights) >> 2) <= i)
            call(VECTOR_THROW_OUT_OF_RANGE, {addr(&self->lineHeights)});
        y = y - ldf(heights + i * 4);
        int32_t align = self->hAlign;
        if (align == 1) {
            uint32_t setPositionX = ld32(ld32(line) + node::setPositionX);
            float viewWidth = ldf(contentSize(me));
            call(setPositionX, {line, fbits(viewWidth * 0.5f)});
        } else if (align == 0) {
            vcall(line, node::setPositionX, {line, 0});
            call(NODE_SET_ANCHOR_POINT, {line, VEC2_ANCHOR_MIDDLE_LEFT});
        }
        int32_t n = int32_t(self->richTexts.end - self->richTexts.begin) >> 2;
        if (int32_t(i + 1) >= n) break;
    }
}
RW_FUNCTION(0x4F9808, "CustomTextView::updateLayout", CustomTextView::updateLayout);

// SAXDelegator::startElement: <big>, <middle> pick the font size, <important> switches to yellow.
void CustomTextView::startElement(CustomTextView *self, uint32_t, uint32_t name, uint32_t) {
    if (call(STRCASECMP, {name, STR_BIG}) == 0) {
        self->fontSizeKind = 2;
    } else if (call(STRCASECMP, {name, STR_MIDDLE}) == 0) {
        self->fontSizeKind = 1;
    } else if (call(STRCASECMP, {name, STR_IMPORTANT}) == 0) {
        self->currentColor[1] = rw::load<uint8_t>(COLOR3B_YELLOW + 1);
        self->currentColor[0] = rw::load<uint8_t>(COLOR3B_YELLOW);
        self->currentColor[2] = rw::load<uint8_t>(COLOR3B_YELLOW + 2);
    }
}
RW_FUNCTION(0x4F99EC, "CustomTextView::startElement", CustomTextView::startElement);

// SAXDelegator::endElement: </big>, </middle> back to the normal size, </important> back to the base colour,
// <br> closes the line (its height = the tallest font + spacing) and starts a new RichText.
void CustomTextView::endElement(CustomTextView *self, uint32_t, uint32_t name) {
    const uint32_t me = addr(self);
    if (call(STRCASECMP, {name, STR_BIG}) == 0 || call(STRCASECMP, {name, STR_MIDDLE}) == 0) {
        self->fontSizeKind = 0;
        return;
    }
    if (call(STRCASECMP, {name, STR_IMPORTANT}) == 0) {
        self->currentColor[1] = self->fontColor[1];
        self->currentColor[2] = self->fontColor[2];
        self->currentColor[0] = self->fontColor[0];
        return;
    }
    if (call(STRCASECMP, {name, STR_BR}) != 0) return;
    uint32_t line = self->currentRichText;
    float lineHeight = ldf(vcall(line, node::getContentSize, {line}) + 4);
    float step = lineHeight + float(self->lineSpacing);
    self->totalHeight = float(self->totalHeight) + step;
    self->currentRichText = call(RICHTEXT_CREATE, {});
    vcall(me, node::addChild, {me, self->currentRichText});
    call(RICHTEXT_VECTOR_PUSH_BACK, {addr(&self->richTexts), self->currentRichText});
    Frame f;
    const uint32_t height = f.alloc(4);
    st32(height, fbits(float(self->lineMaxFontSize) + float(self->lineSpacing)));
    pushHeight(self, height, FLOAT_VECTOR_PUSH_BACK_SLOW_MOVE, true);
}
RW_FUNCTION(0x4F9AA0, "CustomTextView::endElement", CustomTextView::endElement);

int32_t CustomTextView::calcFontSize(CustomTextView *, int32_t size, int32_t kind) { return scaledFontSize(size, kind); }
RW_FUNCTION(0x4F9C24, "CustomTextView::calcFontSize", CustomTextView::calcFontSize);

// SAXDelegator::textHandler: the text run becomes an element of the current line (the length is not used).
void CustomTextView::textHandler(CustomTextView *self, uint32_t, uint32_t text, int32_t) {
    uint32_t element = createCustomElement(self, text);
    call(RICHTEXT_PUSH_BACK_ELEMENT, {self->currentRichText, element});
}
RW_FUNCTION(0x4F9C60, "CustomTextView::textHandler", CustomTextView::textHandler);

// A ui::Text with the current font, size and colour (and the game's text shadow), wrapped in a
// RichElementCustomNode.
uint32_t CustomTextView::createCustomElement(CustomTextView *self, uint32_t text) {
    int32_t size = scaledFontSize(self->fontSize, self->fontSizeKind);
    float fsize = float(size);
    if (float(self->lineMaxFontSize) < fsize) self->lineMaxFontSize = fsize;
    uint32_t label = call(TEXT_CREATE, {});
    Frame f;
    // ui::Text::Config: 3 flag bytes, std::string text (+4), std::string font (+0x10), int size (+0x1C)
    const uint32_t config = f.alloc(0x20);
    st8(config, 0);
    st8(config + 1, 0);
    st8(config + 2, 0);
    call(AEABI_MEMSET, {config + 4, 0x18, 0});
    call(STRING_ASSIGN, {config + 4, text, call(STRLEN, {text})});
    call(STRING_COPY_ASSIGN, {config + 0x10, addr(&self->font)});
    st32(config + 0x1C, uint32_t(size));
    call(TEXT_SET_CONFIG, {label, config});
    const uint32_t color = addr(&self->currentColor);
    vcall(label, node::setColor, {label, color});
    call(SCENEUTIL_SET_SHADOW, {label});
    uint32_t element = call(RICH_ELEMENT_CUSTOM_NODE_CREATE, {0, color, 0xFF, label});
    strFree(config + 0x10);
    strFree(config + 4);
    return element;
}
RW_FUNCTION(0x4F9C80, "CustomTextView::createCustomElement", CustomTextView::createCustomElement);

CustomTextView *CustomTextView::destruct(CustomTextView *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    self->saxVtable = SAX_VTABLE;
    strFree(addr(&self->font));
    call(FLOAT_VECTOR_DTOR, {addr(&self->lineHeights)});
    call(RICHTEXT_VECTOR_DTOR, {addr(&self->richTexts)});
    call(NODE_DTOR, {addr(self)});
    return self;
}
RW_FUNCTION(0x4F9E38, "CustomTextView::~CustomTextView", CustomTextView::destruct);

void CustomTextView::deletingDestruct(CustomTextView *self) {
    destruct(self);
    call(fn::operator_delete, {addr(self)});
}
RW_FUNCTION(0x4F9E9C, "CustomTextView::~CustomTextView (deleting)", CustomTextView::deletingDestruct);
