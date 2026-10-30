// SceneManager::openPartyNewPopup's notice steps: after a party notice, the next one (sub_620D10) while more are
// queued, else the popup closes; with a pending "party news" message (+0x4C0) first an OK dialog whose close does
// the same. The eight dialog answers and the four steps differ only in their vtables.
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t SM_MORE_NOTICES = 0x4BD;    // SceneManager: byte, another party notice is queued
constexpr uint32_t SM_PARTY_NEWS = 0x4C0;      // byte: the "party news" message is due
constexpr uint32_t PartyNewPopup_showNext = 0x620D11;   // sub_620D10(W): the next notice {popup, shadow, opacity}
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;
constexpr int32_t TXT_PARTY_NEWS = 103800061, TXT_PARTY_LEFT = 103860001;
constexpr uint32_t SM_PARTY_LEFT = 0x4BC;      // byte: the player left / was removed from the party
constexpr uint32_t SM_BECAME_LEADER = 0x4BF;   // byte: the player became the party's leader
constexpr uint32_t SM_openPartyBecomePopup = 0x620DE5;   // (-, function<void(bool)> by value)
constexpr uint32_t VT_BecomeClosed = 0xF5F34C; // $_110's become popup answer {this, W}

// {this, W}: the next notice when one is queued, else the popup closed
void nextNotice(uint32_t f) {
    if (ld8(ld32(f + 4) + SM_MORE_NOTICES)) {
        call(PartyNewPopup_showNext, {f + 8});
        return;
    }
    call(SM_closeCurrentPopup, {sceneManager(), 1});
}
// {this, W}: the OK dialog of TextData `text`, its close going on through `answerVt` {this, W}
void dialogStep(uint32_t f, int32_t text, uint32_t answerVt) {
    const uint32_t me = ld32(f + 4);
    Frame fr;
    const uint32_t td = textData(fr, text);
    const uint32_t message = newStr(fr, textDataStr(td));
    const uint32_t w0 = ld32(f + 8), w1 = ld32(f + 0xC), w2 = ld32(f + 0x10);
    const uint32_t answer = fr.alloc(24);
    st32(answer + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x18});
    if (h) {
        st32(h, answerVt);
        st32(h + 4, me);
        st32(h + 8, w0);
        st32(h + 0xC, w1);
        st32(h + 0x10, w2);
    }
    st32(answer + 16, h);
    call(SceneUtil_openOkDialog4Line, {message, answer, 0});
    functionDestroy(answer);
    strFree(message);
    sharedRelease(td);
}
// {this, W}: the party news first (its OK goes on through `answerVt`), else the next notice
void newsThenNext(uint32_t f, uint32_t answerVt) {
    if (!ld8(ld32(f + 4) + SM_PARTY_NEWS)) {
        nextNotice(f);
        return;
    }
    dialogStep(f, TXT_PARTY_NEWS, answerVt);
}
// {this, W}: "you left the party" (+0x4BC) first, else the news / next notice
void leftThenNews(uint32_t f, uint32_t leftVt, uint32_t newsVt) {
    if (ld8(ld32(f + 4) + SM_PARTY_LEFT)) {
        dialogStep(f, TXT_PARTY_LEFT, leftVt);
        return;
    }
    newsThenNext(f, newsVt);
}
void newsThenNextF5F1C0(uint32_t f) { newsThenNext(f, 0xF5F1C0); }
void newsThenNextF5F270(uint32_t f) { newsThenNext(f, 0xF5F270); }
void newsThenNextF5F320(uint32_t f) { newsThenNext(f, 0xF5F320); }
void newsThenNextF5F3D0(uint32_t f) { newsThenNext(f, 0xF5F3D0); }
void step111(uint32_t f) { leftThenNews(f, 0xF5F244, 0xF5F218); }
void step110inner(uint32_t f) { leftThenNews(f, 0xF5F3A4, 0xF5F378); }
// $_110 {this, W}: "you became the leader" (its popup) first, then the left / news chain
void step110(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    if (!ld8(me + SM_BECAME_LEADER)) {
        leftThenNews(f, 0xF5F2F4, 0xF5F2C8);
        return;
    }
    const uint32_t w0 = ld32(f + 8), w1 = ld32(f + 0xC), w2 = ld32(f + 0x10);
    Frame fr;
    const uint32_t answer = fr.alloc(24);
    st32(answer + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x18});
    if (h) {
        st32(h, VT_BecomeClosed);
        st32(h + 4, me);
        st32(h + 8, w0);
        st32(h + 0xC, w1);
        st32(h + 0x10, w2);
    }
    st32(answer + 16, h);
    call(SM_openPartyBecomePopup, {h, answer});      // the original passes the new object in r0 (unused)
    functionDestroy(answer);
}

// $_114 (the become popup's OK) {-, answer function<void(bool)> +8, popup, layer, shadow}(sender, type): the
// decide sound, answer(true), the popup shrinks (its end removes it) and the shadow fades
constexpr uint32_t VT_BecomeEnd = 0xF5F13C;    // {layer, shadow, popup}
void becomeOk(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    call(fn::AudioPlaySystemSEDecide, {});
    if (ld32(f + 8 + 16)) call(fn::function_vb_call, {f + 8, 1});
    const uint32_t popup = ld32(f + 0x20);
    Frame fr;
    const uint32_t end = newFunction(fr, VT_BecomeEnd, {ld32(f + 0x24), ld32(f + 0x28), popup});
    shrinkPopup(popup, end, ld32(f + 0x28));
}

// $_104 (a notice's close) {popup, a, -, b, shared_ptr, shared_ptr, 6 words, shadow}(sender, type): the decide
// sound, the popup shrinks and its end ($_103's lambda, the captures moved into it: heap 0x38) shows the next
constexpr uint32_t VT_NoticeEnd = 0xF5F480;
void noticeClose(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t b = ld32(f + 0x10), ctl1 = ld32(f + 0x14), a = ld32(f + 8), popup = ld32(f + 4);
    if (ctl1) call(fn::shared_add, {ctl1});
    const uint32_t c = ld32(f + 0x18), ctl2 = ld32(f + 0x1C);
    if (ctl2) call(fn::shared_add, {ctl2});
    uint32_t w[7];
    for (uint32_t k = 0; k < 7; k++) w[k] = ld32(f + 0x20 + 4 * k);
    Frame fr;
    const uint32_t end = fr.alloc(24);
    st32(end + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x38});
    uint32_t left1 = ctl1, left2 = ctl2;
    if (h) {
        st32(h, VT_NoticeEnd);
        st32(h + 4, a);
        st32(h + 0xC, b);
        st32(h + 0x10, ctl1);
        st32(h + 0x14, c);
        st32(h + 0x18, ctl2);
        for (uint32_t k = 0; k < 7; k++) st32(h + 0x1C + 4 * k, w[k]);
        left1 = left2 = 0;
    }
    st32(end + 16, h);
    call(fn::Node_runAction, {popup, call(0x45BC61, {popup, end})});
    functionDestroy(end);
    if (left2) call(fn::shared_release, {left2});
    if (left1) call(fn::shared_release, {left1});
    call(fn::Node_runAction, {ld32(f + 0x3C), call(fn::FadeTo_create, {fbits(0.1f), 0})});
}

// $_105 (the party news) {-, this, -, notices +0xC (vector at +4 of {string, u64 user, Date, message}), -, users
// SafeBuffer +0x14, -, title, date, name, message labels +0x1C.., next captures +0x2C}: nothing to tell -> the
// popup closed; else from the notice at +0x4C4 on, the first whose user is known is shown (the next one
// counted); one shown -> the popup goes on (sub_620D10), else it closes
constexpr uint32_t SM_PARTY_FLAGS = 0x4BC;      // five bytes: left, news shown, -, leader, -
constexpr uint32_t SM_NEWS_SHOWN = 0x4BD, SM_NEWS_INDEX = 0x4C4;
constexpr uint32_t NOTICE_SIZE = 0x30, NOTICE_USER = 0x10, NOTICE_DATE = 0x18, NOTICE_TEXT = 0x20;
constexpr uint32_t USER_RECORD = 0x128, USER_NAME = 0xC, USER_FLAGS = 0x11F;
constexpr uint32_t U64Map_find = 0x490275, SafeBuffer_read = 0x45A269, Date_makeLocalDate = 0x4927CD;
constexpr uint32_t TextData_format = 0x492929;       // (string out, type, id, ...)
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;
constexpr int32_t TXT_NEWS_TITLE = 103860002, TXT_NEWS_DATE = 103861003;

// TextData `id` as a label's text
void labelText(uint32_t label, int32_t id) {
    Frame fr;
    const uint32_t td = textData(fr, id);
    const uint32_t s = newStr(fr, textDataStr(td));
    call(fn::Text_setString, {label, s});
    strFree(s);
    sharedRelease(td);
}
// a C string as a label's text
void labelTextC(uint32_t label, uint32_t text) {
    Frame fr;
    const uint32_t s = newStr(fr, text);
    call(fn::Text_setString, {label, s});
    strFree(s);
}
void partyNews(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    bool any = false;
    for (uint32_t k = 0; k < 5; k++) any = any || ld8(me + SM_PARTY_FLAGS + k);
    if (!any) {
        call(SM_closeCurrentPopup, {sceneManager(), 1});
        return;
    }
    st8(me + SM_NEWS_SHOWN, 0);
    const uint32_t index = me + SM_NEWS_INDEX;
    auto count = [&] { const uint32_t v = ld32(f + 0xC) + 4; return ((ld32(v + 4) - ld32(v)) >> 4) * 0xAAAAAAABu; };
    Frame fr;
    const uint32_t notice = fr.alloc(NOTICE_SIZE), key = fr.alloc(8), record = fr.alloc(USER_RECORD + 0xC);
    for (uint32_t i = ld32(index); i < count(); i = ld32(index)) {
        const uint32_t v = ld32(f + 0xC) + 4;
        if (count() <= i) call(Vector_throwOutOfRange, {v});
        const uint32_t src = ld32(v) + NOTICE_SIZE * i;
        strCopy(notice, src);
        copyWords(notice + NOTICE_USER, src + NOTICE_USER, 16);
        strCopy(notice + NOTICE_TEXT, src + NOTICE_TEXT);
        const uint32_t users = ld32(f + 0x14);
        copyWords(key, notice + NOTICE_USER, 8);
        const uint32_t node = call(U64Map_find, {users + 0x24, key});
        const int32_t row = node ? int32_t(ld32(node + 0x10)) : -1;
        if (row < 0) {
            strFree(notice + NOTICE_TEXT);
            strFree(notice);
            st32(index, ld32(index) + 1);
            continue;
        }
        st8(record + USER_FLAGS, ld8(record + USER_FLAGS) & 0xFD);
        const uint32_t off = USER_RECORD * uint32_t(row);
        call(SafeBuffer_read, {users, record, ld32(users + 0xC) + off, USER_RECORD, off});
        labelText(ld32(f + 0x1C), TXT_NEWS_TITLE);
        {
            Frame df;
            const uint32_t date = df.alloc(8), text = df.alloc(12);
            call(Date_makeLocalDate, {date, notice + NOTICE_DATE});
            const uint32_t label = ld32(f + 0x20);
            const uint32_t h1 = rw::load<uint16_t>(date + 2), h2 = rw::load<uint16_t>(date + 4);
            call(TextData_format, {text, 0, uint32_t(TXT_NEWS_DATE), rw::load<uint16_t>(date), h1 & 0xFF, h1 >> 8,
                                   h2 & 0xFF, h2 >> 8});
            labelTextC(label, strData(text));
            strFree(text);
        }
        labelTextC(ld32(f + 0x24), record + USER_NAME);
        call(fn::Text_setString, {ld32(f + 0x28), notice + NOTICE_TEXT});
        st32(index, ld32(index) + 1);
        st8(me + SM_NEWS_SHOWN, 1);
        strFree(notice + NOTICE_TEXT);
        strFree(notice);
        break;
    }
    if (ld8(me + SM_NEWS_SHOWN))
        call(PartyNewPopup_showNext, {f + 0x2C});
    else
        call(SM_closeCurrentPopup, {sceneManager(), 1});
}
}  // namespace

RW_FUNCTION(0x623038, "SceneManager::openPartyBecomePopup $_114::operator()", becomeOk);
RW_FUNCTION(0x624D88, "SceneManager::openPartyNewPopup $_104::operator() (close)", noticeClose);
RW_FUNCTION(0x62361C, "SceneManager::openPartyNewPopup $_111::operator()", step111);
RW_FUNCTION(0x624394, "SceneManager::openPartyNewPopup $_110 lambda(bool)#1::operator()", step110inner);
RW_FUNCTION(0x623C94, "SceneManager::openPartyNewPopup $_110::operator()", step110);
RW_FUNCTION(0x623354, "SceneManager::openPartyNewPopup $_112::operator()(bool)", newsThenNextF5F1C0);
RW_FUNCTION(0x6239CC, "SceneManager::openPartyNewPopup $_111 lambda(bool)#1::operator()", newsThenNextF5F270);
RW_FUNCTION(0x6240CC, "SceneManager::openPartyNewPopup $_110 lambda(bool)#2::operator()", newsThenNextF5F320);
RW_FUNCTION(0x624744, "SceneManager::openPartyNewPopup $_110 lambda(bool)#1 lambda(bool)#1::operator()", newsThenNextF5F3D0);
RW_FUNCTION(0x62328C, "SceneManager::openPartyNewPopup $_113::operator()", nextNotice);
RW_FUNCTION(0x623554, "SceneManager::openPartyNewPopup $_112 news answer::operator()", nextNotice);
RW_FUNCTION(0x623904, "SceneManager::openPartyNewPopup $_111 lambda(bool)#2::operator()", nextNotice);
RW_FUNCTION(0x623BCC, "SceneManager::openPartyNewPopup $_111 news answer::operator()", nextNotice);
RW_FUNCTION(0x624004, "SceneManager::openPartyNewPopup $_110 lambda(bool)#3::operator()", nextNotice);
RW_FUNCTION(0x6242CC, "SceneManager::openPartyNewPopup $_110 news answer::operator()", nextNotice);
RW_FUNCTION(0x62467C, "SceneManager::openPartyNewPopup $_110 lambda(bool)#1 lambda(bool)#2::operator()", nextNotice);
RW_FUNCTION(0x624944, "SceneManager::openPartyNewPopup $_110 news answer 2::operator()", nextNotice);
RW_FUNCTION(0x6250D8, "SceneManager::openPartyNewPopup $_105::operator() (party news)", partyNews);
