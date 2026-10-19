// ChatScrollView: life cycle, rows, messages (see ChatScrollView.hpp); changeRow is in ChatScrollView_row.cpp.
// ChatEditBoxMessageSend (the chat input's EditBox delegate layer) is here too.
#include "ChatScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = ChatScrollView;

namespace {
namespace g {
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t VectorAt_throw = 0x47CCE1;
constexpr uint32_t MessageVector_dtor = 0x5568AD;          // __vector_base<ChatMessage>::~
constexpr uint32_t MessageVector_assign = 0x55D309;        // vector<ChatMessage>::assign(first, last)
constexpr uint32_t IntVector_pushBackSlow = 0x47D709;      // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t IntVector_base_dtor = 0x478961;
constexpr uint32_t AvatarIconCache_makeShared = 0x509899;  // make_shared<AvatarIconCache>(Texture2D*&, Vec2, float, float)
constexpr uint32_t String_substr = 0x47CD4D;               // std::string(const string&, pos, n)
constexpr uint32_t String_compareC = 0x47CBD5;             // string::compare(const char*)
constexpr uint32_t Layer_ctor = 0x88A7D1;
constexpr uint32_t Layer_dtor = 0x88A88D;
}  // namespace g

constexpr uint32_t S_ChatPanelJson = 0xDFECC1;   // "cocostudio/publish/Chat_Panel.json"
constexpr uint32_t S_MainPanel = 0xDFD1FC;
constexpr uint32_t S_Avatar = 0xDF8F56;
constexpr uint32_t S_BalloonButton = 0xDFEDB3;
constexpr uint32_t S_Newline = 0xDF9E14;
constexpr uint32_t S_Empty = 0xDF8A1A;

constexpr uint32_t V_INIT = 0x248;
constexpr uint32_t V_GET_VIRTUAL_RENDERER = 0x2D8;
constexpr uint32_t V_GET_TEXTURE = 0x280;

void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self::Addr Self::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    Self *self = rw::host_ptr<Self>(me);
    constructInt(self);
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    for (uint32_t k = 0; k < 0x14; k += 4) st32(me + 0x638 + k, 0);
    if (init(self) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}
RW_FUNCTION(0x556960, "ChatScrollView::create", ChatScrollView::create);

Self *Self::destroy(Self *self) {
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    if (self->rowTemplate) call(g::Ref_release, {self->rowTemplate});
    if (self->iconCache.ctrl) call(fn::shared_release, {self->iconCache.ctrl});
    call(g::MessageVector_dtor, {self->self() + 0x638});
    destroyInt(self);
    return self;
}
RW_FUNCTION(0x5567B4, "ChatScrollView::~ChatScrollView", ChatScrollView::destroy);

// 934 x 180 rows: an invisible template holding Chat_Panel ("mainPanel"); the avatar icons drawn from the panel's
// Avatar texture at (-80, 130), scale 1.6.
uint32_t Self::init(Self *self) {
    self->itemWidth = 934.0f;
    self->itemHeight = 180.0f;
    setEnabledScrollBar(self, 1);
    self->offsetX = self->itemWidth * 0.5f;
    self->offsetY = 0.0f;
    self->rowTemplate = call(g::Widget_create, {});
    const uint32_t row = self->rowTemplate;
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fbits(float(self->itemWidth)), fbits(float(self->itemHeight))});
        vcall(row, node::setContentSize, {row, size});
    }
    vcall(self->rowTemplate, node::setAnchorPoint, {self->rowTemplate, VEC2_ANCHOR_MIDDLE});
    call(g::Ref_retain, {self->rowTemplate});
    vcall(self->rowTemplate, node::setVisible, {self->rowTemplate, 0});
    const uint32_t reader = call(fn::GUIReader_getInstance, {});
    const uint32_t panel = call(fn::GUIReader_widgetFromJsonFile, {reader, S_ChatPanelJson});
    {
        Frame f;
        const uint32_t name = newStr(f, S_MainPanel, 9);
        vcall(self->rowTemplate, node::addChildZName, {self->rowTemplate, panel, 0, name});
        strFree(name);
    }
    const uint32_t avatar = findNode(panel, S_Avatar, 6);
    const uint32_t renderer = vcall(avatar, V_GET_VIRTUAL_RENDERER, {avatar});
    Frame f;
    const uint32_t texture = f.alloc(4), pos = f.alloc(8), s1 = f.alloc(4), s2 = f.alloc(4), made = f.alloc(8);
    st32(texture, vcall(renderer, V_GET_TEXTURE, {renderer}));
    call(g::Vec2_ctor, {pos, fbits(-80.0f), fbits(130.0f)});
    stf(s1, 1.6f);
    stf(s2, 1.6f);
    call(g::AvatarIconCache_makeShared, {made, texture, pos, s1, s2});
    const uint32_t old = self->iconCache.ctrl;
    self->iconCache.obj = ld32(made);
    self->iconCache.ctrl = ld32(made + 4);
    if (old) call(fn::shared_release, {old});
    call(g::Vec2_dtor, {pos});
    return initInt(self);
}
RW_FUNCTION(0x5569F4, "ChatScrollView::init", ChatScrollView::init);

// A row: a clone of the template (the original clones it twice and keeps the second).
Self::Addr Self::createRow(Self *self, int32_t index, int32_t arg) {
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {self->self() + 0x628});
    call(g::Widget_clone, {self->rowTemplate});
    const uint32_t row = call(g::Widget_clone, {self->rowTemplate});
    vcall(self->self(), V_isReplaceItem, {self->self(), row, uint32_t(index), uint32_t(arg)});
    return row;
}
RW_FUNCTION(0x556C58, "ChatScrollView::createRow", ChatScrollView::createRow);

// Lines `text` takes at `width` columns (a multi-byte UTF-8 character counts two; a line full before a character
// wraps without it), capped at maxLines.
int32_t Self::getTextLineCount(Addr text, int32_t width, int32_t maxLines) {
    Frame f;
    const uint32_t empty = newStr(f, S_Empty, 0);
    const uint32_t copy = f.alloc(12);
    copyString(copy, text);
    const bool isLong = (ld8(copy) & 1) != 0;
    const uint32_t length = isLong ? ld32(copy + 4) : uint32_t(ld8(copy) >> 1);
    int32_t lines = 1;
    if (length) {
        int32_t column = 0;
        uint32_t pos = 0;
        for (;;) {
            const uint32_t data = isLong ? ld32(copy + 8) : copy + 1;
            const int8_t c = rw::load<int8_t>(data + pos);
            const uint32_t u = uint8_t(c);
            uint32_t n = 1;
            if (c < 0) {
                n = 2;
                if ((u >> 5) >= 7) {
                    n = 3;
                    if ((u >> 4) >= 15) {
                        n = 4;
                        if ((u >> 3) >= 31) {
                            n = 5;
                            if ((u >> 2) >= 63) n = (u >> 1) >= 127 ? 3 : 6;
                        }
                    }
                }
            }
            const uint32_t sub = f.alloc(12);
            call(g::String_substr, {sub, copy, pos, n});
            if (call(g::String_compareC, {sub, S_Newline})) {
                const int32_t next = column + (n > 1 ? 2 : 1);
                if (next > width) {
                    lines++;
                    column = 0;
                } else {
                    column = next;
                }
            } else {
                lines++;
                column = 0;
            }
            strFree(sub);
            pos += n;
            if (lines > maxLines) {
                lines = maxLines;
                break;
            }
            if (pos >= length) break;
        }
    }
    strFree(copy);
    strFree(empty);
    return lines;
}
RW_FUNCTION(0x55852C, "ChatScrollView::getTextLineCount", ChatScrollView::getTextLineCount);

// Show `messages` (copied): the rows hidden (their balloon buttons removed), item ids 1..n, scrolled to the message
// numbered `selectNumber` when given, else to the last one.
void Self::setMessages(Self *self, Addr messages, uint32_t selectNumber) {
    const uint32_t me = self->self();
    if (me + 0x638 != messages) call(g::MessageVector_assign, {me + 0x638, ld32(messages), ld32(messages + 4)});
    if (self->itemSizes.end != self->itemSizes.begin) self->itemSizes.end = self->itemSizes.begin;
    if (self->rowCount > 0.0f) {
        int32_t i = 0;
        do {
            const uint32_t row = ld32(self->items.begin + 4 * uint32_t(i));
            vcall(row, node::setVisible, {row, 0});
            const uint32_t button = findNode(row, S_BalloonButton, 13);
            if (button) vcall(button, node::removeFromParentAndCleanup, {button, 1});
            i++;
        } while (float(i) < float(self->rowCount));
    }
    Frame f;
    const uint32_t ids = f.alloc(12), value = f.alloc(4);
    for (uint32_t k = 0; k < 12; k += 4) st32(ids + k, 0);
    int32_t found = 0;
    const uint32_t count = (ld32(messages + 4) - ld32(messages)) >> 6;
    for (uint32_t k = 1; count && k <= count; k++) {
        st32(value, k);
        const uint32_t end = ld32(ids + 4);
        if (end >= ld32(ids + 8)) {
            call(g::IntVector_pushBackSlow, {ids, value});
        } else {
            if (end) st32(end, k);
            st32(ids + 4, end + 4);
        }
        // a copy of the message (only its number is read)
        const uint32_t m = ld32(messages) + MESSAGE_SIZE * (k - 1);
        Frame h;
        const uint32_t copy = h.alloc(MESSAGE_SIZE);
        for (uint32_t w = 0; w < 0x10; w += 4) st32(copy + w, ld32(m + w));
        st8(copy + 0x10, ld8(m + 0x10));
        copyString(copy + 0x14, m + 0x14);
        for (uint32_t w = 0x20; w < 0x2C; w += 4) st32(copy + w, ld32(m + w));
        copyString(copy + 0x2C, m + 0x2C);
        st8(copy + 0x38, ld8(m + 0x38));
        if (selectNumber && ld32(copy + 0x28) == selectNumber) found = int32_t(k - 1);
        strFree(copy + 0x2C);
        strFree(copy + 0x14);
    }
    setItemIDsInt(self, ids);
    const int32_t bytes = int32_t(ld32(messages + 4) - ld32(messages));
    if (bytes < 1) IScrollProtocol::setIndex(self, 0);
    else if (selectNumber) IScrollProtocol::setIndexPrev(self, found, -1);
    else IScrollProtocol::setIndex(self, (bytes >> 6) - 1);
    call(g::IntVector_base_dtor, {ids});
}
RW_FUNCTION(0x558710, "ChatScrollView::setMessages", ChatScrollView::setMessages);

// ------------------------------------------------------------------------------------------------ ChatEditBoxMessageSend
namespace chat_edit_box {
constexpr uint32_t SIZE = 0x2B8;
constexpr uint32_t VTABLE = 0xF896F4, VTABLE2 = 0xF89A0C;   // Layer, EditBoxDelegate (+0x288)

uint32_t create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    st32(me, VTABLE);
    st32(me + 0x288, VTABLE2);
    st32(me + 0x2A0, 0);
    st32(me + 0x2A8, 0);
    st32(me + 0x2AC, 0);
    st8(me + 0x2B0, 0);
    call(g::Ref_autorelease, {me});
    return me;
}
RW_FUNCTION(0x55C6E8, "ChatEditBoxMessageSend::create", chat_edit_box::create);

uint32_t destroy(uint32_t me) {
    st32(me, VTABLE);
    st32(me + 0x288, VTABLE2);
    functionDestroy(me + 0x290);
    call(g::Layer_dtor, {me});
    return me;
}
RW_FUNCTION(0x55C65C, "ChatEditBoxMessageSend::~ChatEditBoxMessageSend", chat_edit_box::destroy);
}  // namespace chat_edit_box
