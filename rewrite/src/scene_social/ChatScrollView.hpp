// ChatScrollView: the chat window's message list (SceneManager's chat popup). A BaseScrollView<int> of 932 x 180 rows
// built from Chat_Panel; the messages (ChatMessage, 0x40 bytes) are copied in by setMessages, the players' avatar
// icons come from an AvatarIconCache. changeRow lays a message out (balloon or stamp, name, date, medal).
//
// ChatMessage: +0..+0xF words, +0x10 byte, +0x14 std::string, +0x20 three words (+0x28 the message number),
// +0x2C std::string, +0x38 byte.
#pragma once

#include "../w3_9/BaseScrollView.hpp"

struct ChatScrollView : BaseScrollView {
    rw::Field<uint32_t> rowTemplate;        // +0x634 ui::Widget* (retained) holding Chat_Panel as "mainPanel"
    guest::Vector messages;                 // +0x638 std::vector<ChatMessage>
    guest::SharedPtr iconCache;             // +0x644 std::shared_ptr<AvatarIconCache>
    const uint8_t pad_64C[4];

    static constexpr uint32_t SIZE = 0x650;
    static constexpr uint32_t VTABLE = 0xF88538;
    static constexpr uint32_t VTABLE2 = 0xF88890;
    static constexpr uint32_t VTABLE3 = 0xF888A4;
    static constexpr uint32_t MESSAGE_SIZE = 0x40;

    static Addr create();
    static ChatScrollView *destroy(ChatScrollView *self);
    static uint32_t init(ChatScrollView *self);
    static Addr createRow(ChatScrollView *self, int32_t index, int32_t arg);
    static uint32_t changeRow(ChatScrollView *self, Addr row, int32_t index, int32_t arg);
    static int32_t getTextLineCount(Addr text, int32_t width, int32_t maxLines);
    static void setMessages(ChatScrollView *self, Addr messages, uint32_t selectNumber);
};
RW_LAYOUT(ChatScrollView, rowTemplate, 0x634);
RW_LAYOUT(ChatScrollView, messages, 0x638);
RW_LAYOUT(ChatScrollView, iconCache, 0x644);
RW_SIZE(ChatScrollView, ChatScrollView::SIZE);
