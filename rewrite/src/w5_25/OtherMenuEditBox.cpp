// MENU > Other > Serial Code / invitation code: OtherMenuDialog's EditBox delegate. While typing, the field's OK
// button is enabled when it holds text (TXT_SERIAL_INPUT: the serial code popup; TXT_CODE_INPUT: the invitation
// code popup); when editing ends the text is shown on the popup's label and SceneUtil::editBoxEditingDidEnd
// restores the field's display.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t EditBox_getText = 0x85520D;
constexpr uint32_t String_compareC = 0x47CBD5;         // std::string::compare(const char*)
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;   // (Button*, Text*, bool enabled, bool)
constexpr uint32_t SceneUtil_editBoxEditingDidEnd = 0x45C61D;   // (Node**, Label*, const char*)
constexpr uint32_t GOT_TXT_SERIAL_INPUT = 0xFD66DC;    // &TXT_SERIAL_INPUT (const char*: the serial field's name)
constexpr uint32_t GOT_TXT_CODE_INPUT = 0xFD66E0;      // &TXT_CODE_INPUT ("TextField_Input_Code")
}  // namespace g

constexpr uint32_t V_GET_NAME = 0x150;
// OtherMenuDialog
constexpr uint32_t F_SERIAL_OK = 0x280, F_SERIAL_OK_TEXT = 0x284;
constexpr uint32_t F_CODE_OK = 0x288, F_CODE_OK_TEXT = 0x28C;
constexpr uint32_t F_INPUT_LABEL = 0x290;              // the label showing the typed text
constexpr uint32_t F_INPUT_FIELD = 0x294;              // Node* (SceneUtil keeps the field here)
constexpr uint32_t F_INPUT_DISPLAY = 0x298;            // Label*

// the edit box's name is the field global's text
bool isField(uint32_t name, uint32_t got) { return call(g::String_compareC, {name, ld32(ld32(got))}) == 0; }

struct OtherMenuEditBox {
    static void textChanged(uint32_t self, uint32_t editBox, uint32_t) {
        Frame f;
        uint32_t name = f.alloc(12);
        vcall(editBox, V_GET_NAME, {name, editBox});
        if (isField(name, g::GOT_TXT_SERIAL_INPUT)) {
            uint32_t text = call(g::EditBox_getText, {editBox});
            call(g::SceneUtil_setButtonEnabled,
                 {ld32(self + F_SERIAL_OK), ld32(self + F_SERIAL_OK_TEXT), ld8(text) ? 1u : 0u, 0});
        } else if (isField(name, g::GOT_TXT_CODE_INPUT)) {
            uint32_t text = call(g::EditBox_getText, {editBox});
            call(g::SceneUtil_setButtonEnabled,
                 {ld32(self + F_CODE_OK), ld32(self + F_CODE_OK_TEXT), ld8(text) ? 1u : 0u, 0});
        }
        strFree(name);
    }

    static void editingDidEnd(uint32_t self, uint32_t editBox) {
        Frame f;
        uint32_t name = f.alloc(12);
        vcall(editBox, V_GET_NAME, {name, editBox});
        if (isField(name, g::GOT_TXT_SERIAL_INPUT) || isField(name, g::GOT_TXT_CODE_INPUT)) {
            Frame t;
            uint32_t text = newStr(t, call(g::EditBox_getText, {editBox}));
            uint32_t label = ld32(self + F_INPUT_LABEL);
            if (label) setTextC(label, strData(text));
            strFree(text);
        }
        call(g::SceneUtil_editBoxEditingDidEnd,
             {self + F_INPUT_FIELD, ld32(self + F_INPUT_DISPLAY), call(g::EditBox_getText, {editBox})});
        strFree(name);
    }
};
}  // namespace

RW_FUNCTION(0x5A0EB0, "OtherMenuDialog::editBoxTextChanged", OtherMenuEditBox::textChanged);
RW_FUNCTION(0x5A0FD8, "OtherMenuDialog::editBoxEditingDidEnd", OtherMenuEditBox::editingDidEnd);
