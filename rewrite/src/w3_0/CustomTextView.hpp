// CustomTextView: a cocos2d::Node showing a small XML markup as lines of ui::RichText. The text is wrapped in
// <xml>...</xml> (newlines become <br></br>) and fed to a SAXParser with this view as delegate: <big> / <middle>
// scale the font (x1.2 / x1.1), <important> colours the text yellow, <br> starts a new line (RichText). Each text
// run becomes a ui::Text in a RichElementCustomNode. updateLayout() stacks the lines vertically centred.
#pragma once

#include "core/Guest.hpp"

struct CustomTextView {
    const uint8_t node[0x270];             // +0x000 cocos2d::Node
    rw::Field<uint32_t> saxVtable;         // +0x270 cocos2d::SAXDelegator base
    rw::Field<int32_t> hAlign;             // +0x274 0 left, 1 centre
    rw::Field<int32_t> fontSizeKind;       // +0x278 0 normal, 1 middle (x1.1), 2 big (x1.2)
    guest::Vector richTexts;               // +0x27C cocos2d::Vector<ui::RichText*> (one per line)
    guest::Vector lineHeights;             // +0x288 std::vector<float>: each finished line's height
    rw::Field<float> lineMaxFontSize;      // +0x294 the largest font size used on the current line
    rw::Field<uint32_t> currentRichText;   // +0x298 the line being filled
    rw::Field<float> totalHeight;          // +0x29C
    rw::Field<int32_t> fontSize;           // +0x2A0
    rw::Field<uint8_t> fontColor[3];       // +0x2A4 Color3B: the base colour
    rw::Field<uint8_t> currentColor[3];    // +0x2A7 Color3B: the colour of the next text run
    const uint8_t pad_2AA[2];
    guest::String font;                    // +0x2AC font file
    rw::Field<float> lineSpacing;          // +0x2B8
    rw::Field<float> maxLineWidth;         // +0x2BC widest line (set by updateLayout)

    static constexpr uint32_t VTABLE = 0xF80B08;        // _ZTV14CustomTextView + 8
    static constexpr uint32_t SAX_VTABLE = 0xF80D8C;    // _ZTV14CustomTextView + 0x28C (SAXDelegator part)

    static uint32_t create();
    static uint32_t init(CustomTextView *self);
    static void setHAlign(CustomTextView *self, int32_t align);
    static void setLineSpacing(CustomTextView *self, float spacing);
    static void setFont(CustomTextView *self, uint32_t font);
    static void setFontColor(CustomTextView *self, uint32_t color);
    static void setFontSize(CustomTextView *self, int32_t size);
    static void setXml(CustomTextView *self, uint32_t text);
    static void updateLayout(CustomTextView *self);
    static void startElement(CustomTextView *self, uint32_t ctx, uint32_t name, uint32_t attributes);
    static void endElement(CustomTextView *self, uint32_t ctx, uint32_t name);
    static int32_t calcFontSize(CustomTextView *self, int32_t size, int32_t kind);
    static void textHandler(CustomTextView *self, uint32_t ctx, uint32_t text, int32_t length);
    static uint32_t createCustomElement(CustomTextView *self, uint32_t text);
    static CustomTextView *destruct(CustomTextView *self);
    static void deletingDestruct(CustomTextView *self);
};
RW_LAYOUT(CustomTextView, saxVtable, 0x270);
RW_LAYOUT(CustomTextView, richTexts, 0x27C);
RW_LAYOUT(CustomTextView, lineHeights, 0x288);
RW_LAYOUT(CustomTextView, fontSize, 0x2A0);
RW_LAYOUT(CustomTextView, currentColor, 0x2A7);
RW_LAYOUT(CustomTextView, font, 0x2AC);
RW_LAYOUT(CustomTextView, maxLineWidth, 0x2BC);
RW_SIZE(CustomTextView, 0x2C0);
