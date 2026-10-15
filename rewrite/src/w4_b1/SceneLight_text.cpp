// SceneLight: cmd_text (the speaker name and the speech pages) and the lambdas written here.
#include "SceneLight_lib.hpp"

using namespace slight;

// Show text `textId` (pages split on '\f'), with speaker `nameId` (0: none) in the window, or as narration
void SceneLight::cmdText(Addr self, int32_t, int32_t nameId, int32_t textId) {
    stopAllSlots(self);
    Frame f;
    Size2 vs = visibleSize(f);
    Size2 window = contentSize(f, sprite(self, SLOT_WINDOW));
    removeText(self);
    Self *s = S(self);
    if (nameId != 0) {
        uint32_t id = uint32_t(nameId % TEXT_ID_BASE + TEXT_ID_BASE);
        uint32_t name = f.alloc(8);
        call(TextData_get, {name, 1, id});
        uint32_t label = s->nameLabel;
        {
            Frame g;
            uint32_t fmt = textData(g, 100099015);
            uint32_t arg = ld32(name) ? ld32(ld32(name) + 4) : S_NO_NAME;
            uint32_t buf = stringBuffer(g, {textDataStr(fmt), arg});
            uint32_t str = newStr(g, buf);
            vcall(label, VS_setString, {label, str});
            strFree(str);
            release(ld32(fmt + 4));
        }
        label = s->nameLabel;
        vcall(label, VS_setPositionX, {label, fb(vs.w * 0.5f)});
        float y = vs.h * 0.5f;
        if (s->windowType) y = ((y + -320.0f) + window.h * 0.5f + 48.0f) + 18.0f;
        label = s->nameLabel;
        vcall(label, VS_setPositionY, {label, fb(y)});
        uint32_t color = s->windowType ? Color4B_BLACK : Color4B_WHITE;
        label = s->nameLabel;
        vcall(label, VS_setTextColor, {label, color});
        call(Label_setLineHeight, {s->nameLabel, fb(48.0f)});
        call(Label_setDimensions, {s->nameLabel, f2u(window.w + -96.0f), 48});
        label = s->nameLabel;
        call(Label_setAlignment, {label, 0, ld32(label + 0x3E0)});   // (LEFT, current vertical alignment)
        vcall(self, VS_addChildZ, {self, s->nameLabel, 0x67});
        label = s->nameLabel;
        setVisible(label, 1);
        release(ld32(name + 4));
    }
    if (s->windowType) {
        call(Label_setLineHeight, {s->speech, fb(48.0f)});
        float y = ((vs.h * 0.5f + -320.0f) + window.h * 0.5f) + -6.0f;
        uint32_t d = s->speech;
        vcall(d, VS_setPositionY, {d, fb(y)});
        d = s->speech;
        vcall(d, VS_setPositionX, {d, fb(vs.w * 0.5f)});
        call(Label_setDimensions, {s->speech, f2u(window.w + -96.0f), 96});
        d = s->speech;
        vcall(d, VS_setTextColor, {d, Color4B_BLACK});
        vcall(self, VS_addChildZ, {self, s->speech, 0x67});
    } else {
        call(Label_setLineHeight, {s->speechFix, fb(42.0f)});
        uint32_t d = s->speechFix;
        vcall(d, VS_setPositionY, {d, fb(vs.h * 0.5f + -6.0f)});
        d = s->speechFix;
        vcall(d, VS_setPositionX, {d, fb(vs.w * 0.5f)});
        call(Label_setDimensions, {s->speechFix, 0, 0});
        d = s->speechFix;
        vcall(d, VS_setTextColor, {d, Color4B_WHITE});
        vcall(self, VS_addChildZ, {self, s->speechFix, 0x67});
        uint32_t label = s->nameLabel;
        setVisible(label, 0);
    }

    uint32_t text = f.alloc(12);
    call(TextData_format, {text, 1, uint32_t(textId % TEXT_ID_BASE + TEXT_ID_BASE)});
    const bool isLong = strIsLong(text);
    const uint32_t len = isLong ? ld32(text + 4) : uint32_t(ld8(text) >> 1);
    if (len == 0) {
        // no text: a following 4006 (cursor) command is skipped, go on at once
        uint32_t cmd = s->command;
        if (cmd && int32_t(ld32(cmd)) == OP_BUTTON) s->command = cmd + 0x14;
        s->wait = 0.0f;
    } else if (len != 0) {
        s->waitingPage = 0;
        uint32_t pages = f.alloc(0x18);
        call(wl::MEMSET, {pages, 0x18, 0});
        uint32_t found, pos = 0;
        do {
            Frame g;
            found = call(String_findChar, {text, 0x0C, pos});
            uint32_t n = found == 0xFFFFFFFFu ? found : found - pos;
            uint32_t page = g.alloc(12);
            call(String_substr, {page, text, pos, n, text});
            call(Deque_pushBack, {pages, page});
            strFree(page);
            pos = found + 1;
        } while (found != 0xFFFFFFFFu);
        const bool window = s->windowType != 0;
        const uint32_t keyField = window ? OFF_SPEECH : OFF_SPEECH_FIX;
        {
            // pages[disp] = std::move(pages) (a pair<SpeechDisp* const, deque> temporary)
            Frame g;
            uint32_t pair = g.alloc(0x1C);
            st32(pair, ld32(self + keyField));
            for (uint32_t k = 0; k < 4; k++) st32(pair + 4 + k * 4, ld32(pages + k * 4));
            for (uint32_t k = 4; k-- > 0;) st32(pages + k * 4, 0);
            st32(pair + 0x14, ld32(pages + 0x10));
            st32(pair + 0x18, ld32(pages + 0x14));
            st32(pages + 0x10, 0);
            st32(pages + 0x14, 0);
            uint32_t out = g.alloc(8);
            call(Pages_insert, {out, self + OFF_PAGES, pair});
            call(Deque_baseDtor, {pair + 4});
        }
        if (window) runSpeech(self, s->speech);
        else runSpeechFix(self, s->speechFix);
        call(Deque_baseDtor, {pages});
    } else {
        // (unreachable: the length was just checked) "no such line" and $_8 / $_9 wait for the speech end
        uint32_t d = s->windowType ? s->speech : s->speechFix;
        {
            Frame g;
            uint32_t str = newStr(g, S_NO_LINE, 42);
            vcall(d, VS_setString, {d, str});
            strFree(str);
        }
        Frame g;
        uint32_t fo = newFunction(g, s->windowType ? VT_text_8 : VT_text_9, {self});
        call(SpeechDisp_runSpeechSet, {d, call(wl::CallFunc_create, {fo})});
        functionDestroy(fo);
    }
    vcall(self, VS_reorderChild, {self, sprite(self, SLOT_CURSOR), 0x69});
    strFree(text);
}

// runSpeech::$_10 {this, disp}: a page was shown - drop it; the next one waits for the tap
void SceneLight::speechPageDone(Addr fn) {
    uint32_t self = ld32(fn + 4);
    uint32_t pages = self + OFF_PAGES;
    uint32_t deque = call(Pages_index, {pages, fn + 8});
    call(Deque_popFront, {deque});
    if (ld32(deque + 0x14)) {
        S(self)->waitingPage = 1;
        priExecCommand(self, 0.0f);
    } else {
        call(Pages_erase, {pages, fn + 8});
        S(self)->wait = 0.0f;
    }
}

// init::$_4 {this} (Touch*, Event*): a tap. During a speech: show it whole / hide the cursor; on a cursor wait:
// a sound, the text and actions stop. A tap on the skip button (its box grown by 32 px) ends the scene.
void SceneLight::onTouchEnded(Addr fn, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    int32_t op = s->currentOp;
    if (op == OP_TEXT) {
        if (s->waitingPage) {
            setVisible(sprite(self, SLOT_CURSOR), 0);
        } else {
            call(SpeechDisp_toEndSpeech, {s->speech});
            call(SpeechDisp_toEndSpeech, {s->speechFix});
            setVisible(sprite(self, SLOT_CURSOR), 1);
        }
        s->wait = 0.0f;
    } else if (op == OP_BUTTON) {
        playSE(0x1E5841B6);
        removeText(self);
        setVisible(sprite(self, SLOT_CURSOR), 0);
        stopAllSlots(self);
        s->wait = 0.0f;
    }
    s->touchFlag = 0;
    Frame f;
    uint32_t box = f.alloc(16);
    uint32_t arm = s->skipButton;
    call(ld32(ld32(arm) + VS_getBoundingBox), {box, arm});
    stf(box + 0xC, ldf(box + 0xC) + 64.0f);
    stf(box + 4, ldf(box + 4) + -32.0f);
    stf(box + 8, ldf(box + 8) + 64.0f);
    stf(box, ldf(box) + -32.0f);
    uint32_t loc = f.alloc(8);
    call(smain::lib::Touch_getLocation, {loc, touch});
    uint32_t hit = call(smain::lib::Rect_containsPoint, {box, loc});
    call(wl::Vec2_dtor, {loc});
    if (hit == 1) {
        uint32_t anim = vcall(s->skipButton, VS_getAnimation, {s->skipButton});
        vcall(anim, VS_Animation_playWithIndex, {anim, 1, 0xFFFFFFFFu, 0xFFFFFFFFu});
        call(AudioPlaySystemSEDecide, {});
        nextScene(self);
    }
    call(wl::Vec2_dtor, {box});
}

RW_FUNCTION(0x6C2F90, "SceneLight::cmd_text", SceneLight::cmdText);
RW_FUNCTION(0x6C5820, "SceneLight::runSpeech::$_10::operator()", SceneLight::speechPageDone);
RW_FUNCTION(0x6C5EA4, "SceneLight::init::$_4::operator()", SceneLight::onTouchEnded);
