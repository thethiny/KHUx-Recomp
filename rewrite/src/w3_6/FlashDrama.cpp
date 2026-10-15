// FlashDrama: an Avatar playing a story "drama" LWF (the avatar's own LWF, instance "my_avatar"): the LWF's text
// events open speech balloons (SpeechDisp) that show their text line by line, its sound events play SEs / swap
// the main BGM, and nextFrame() advances the drama once every speech is through.
#include "Common.hpp"

using namespace w36;

struct FlashDrama {
    const uint8_t avatar[0x578];           // +0     Avatar (lwfNode +0x29C, callbacks +0x498 / +0x4B0, sound +0x4F8..)
    rw::Field<uint32_t> seqMovie;          // +0x578 LWF::Movie* "seq_main" (set by its load handler)
    rw::Field<uint32_t> mainMovie;         // +0x57C LWF::Movie* "drama_main"
    // cocos2d::TTFConfig of the speech balloons
    guest::String fontPath;                // +0x580
    rw::Field<int32_t> fontSize;           // +0x58C
    rw::Field<uint32_t> glyphs;            // +0x590
    rw::Field<uint32_t> customGlyphs;      // +0x594
    rw::Field<uint32_t> distanceField;     // +0x598 bool (+ 3 bytes)
    rw::Field<int32_t> outlineSize;        // +0x59C
    rw::Field<uint16_t> italicsBold;       // +0x5A0
    const uint8_t pad_5a2[2];
    rw::Field<uint32_t> unknown5A4;        // +0x5A4
    const uint8_t pendingTexts[0x14];      // +0x5A8 unordered_map<string, list<int>>: text ids waiting per balloon
    const uint8_t speechByName[0x14];      // +0x5BC unordered_map<string, SpeechDisp*>
    const uint8_t activeSpeeches[0x14];    // +0x5D0 unordered_set<SpeechDisp*>: balloons showing text
    guest::Vector seIds;                   // +0x5E4 vector<int>: SEs started (stopped on destruction)
    const uint8_t speechLines[0x14];       // +0x5F0 unordered_map<SpeechDisp*, deque<string>>: lines still to show

    static constexpr uint32_t VTABLE = 0xFA2308;          // _ZTV10FlashDrama + 8
    // Avatar members used here
    static constexpr uint32_t LWF_NODE = 0x29C;           // cocos2d::LWFNode*
    static constexpr uint32_t ON_LOAD_LWF = 0x498;        // std::function<void(const char*)>
    static constexpr uint32_t ON_SOUND = 0x4B0;           // std::function<void(int, int, const vector<int>*)>
    static constexpr uint32_t PENDING_SE = 0x4F8;         // audio::ResourceID to play
    static constexpr uint32_t PENDING_BGM = 0x4FC;        // audio::ResourceID to switch the main BGM to
    static constexpr uint32_t SOUND_DIRTY = 0x500;        // byte

    static FlashDrama *construct(FlashDrama *self);
    static FlashDrama *constructWithLwf(FlashDrama *self, uint32_t lwfName);
    static FlashDrama *destruct(FlashDrama *self);
    static void destructDelete(FlashDrama *self);
    static void update(FlashDrama *self, float dt);
    static uint32_t displayString(FlashDrama *self, const guest::String *name, int32_t textId);
    static void updateSound(FlashDrama *self, float dt);
    static void runSpeech(FlashDrama *self, uint32_t speech);
    static void addEventListeners(FlashDrama *self);
    static uint32_t finalizeSpeeches(FlashDrama *self);
    static void nextFrame(FlashDrama *self);
    static uint32_t isFinished(const FlashDrama *self);
};
RW_LAYOUT(FlashDrama, fontPath, 0x580);
RW_LAYOUT(FlashDrama, distanceField, 0x598);
RW_LAYOUT(FlashDrama, unknown5A4, 0x5A4);
RW_LAYOUT(FlashDrama, seIds, 0x5E4);
RW_LAYOUT(FlashDrama, speechLines, 0x5F0);

namespace {
using rw::guest_addr;
constexpr uint32_t Avatar_ctor = 0x5C58C9;              // Avatar::Avatar(bool)
constexpr uint32_t Avatar_dtor = 0x5C6069;
constexpr uint32_t Avatar_update = 0x5C62DD;
constexpr uint32_t Avatar_changeStyle = 0x5C5F15;       // (const Avatar::Style&)
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;      // (Avatar::Motion)
constexpr uint32_t Avatar_setInstanceName = 0x5C7359;
constexpr uint32_t Avatar_changeLWF = 0x5C7575;
constexpr uint32_t Avatar_addEventListeners = 0x5C7745;
constexpr uint32_t AvatarUtil_getCurrentStyle = 0x4977AD;   // (Style* out, bool)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;                   // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Node_getNumberOfRunningActions = 0x894B4F;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t SpeechDisp_runSpeechSet = 0x492A4D;  // (disp, CallFunc*)
constexpr uint32_t SpeechDisp_toEndSpeech = 0x4929F5;
constexpr uint32_t TextData_format = 0x492929;          // (std::string* out, Type, id, ...)
constexpr uint32_t STRING_FIND_CHAR = 0x601CD1;         // std::string::find(char, pos)
constexpr uint32_t STRING_SUBSTR_CTOR = 0x47CD4D;       // std::string(const string&, pos, n, alloc)
constexpr uint32_t DEQUE_PUSH_BACK = 0x601D05;          // deque<string>::push_back(string&&)
constexpr uint32_t DEQUE_POP_FRONT = 0x601DC1;          // deque<string>::pop_front()
constexpr uint32_t DEQUE_BASE_DTOR = 0x601D95;          // __deque_base<string>::~__deque_base
constexpr uint32_t UDIV = 0xCD5DF9;                     // __udivsi3
constexpr uint32_t UMOD = 0xCD5F3D;                     // __umodsi3
constexpr uint32_t SPEECH_BY_NAME_FIND = 0x6F537F;      // unordered_map<string, SpeechDisp*>::find
constexpr uint32_t ACTIVE_FIND = 0x6F452D;              // unordered_set<SpeechDisp*>::find
constexpr uint32_t ACTIVE_INSERT = 0x6F4F95;            // unordered_set<SpeechDisp*>::__insert_unique (sret pair)
constexpr uint32_t ACTIVE_ERASE = 0x6F42CD;             // unordered_set<SpeechDisp*>::erase(const_iterator)
constexpr uint32_t ACTIVE_DTOR = 0x6F62D7;
constexpr uint32_t LINES_INSERT = 0x603C09;             // unordered_map<SpeechDisp*, deque>::__insert_unique(pair&&)
constexpr uint32_t LINES_AT = 0x601971;                 // unordered_map<SpeechDisp*, deque>::operator[]
constexpr uint32_t LINES_ERASE = 0x604EE1;              // unordered_map<SpeechDisp*, deque>::__erase_unique(key)
constexpr uint32_t LINES_DTOR = 0x608097;
constexpr uint32_t SPEECH_BY_NAME_DTOR = 0x6F628B;
constexpr uint32_t PENDING_TEXTS_DTOR = 0x6F6235;
constexpr uint32_t VECTOR_INT_DTOR = 0x478961;
constexpr uint32_t VECTOR_INT_PUSH_SLOW = 0x47D709;     // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t FUNC_STR_SWAP = 0x4E2961;            // std::function<void(const char*)>::swap
constexpr uint32_t FUNC_SOUND_SWAP = 0x6F547D;          // std::function<void(int,int,const vector<int>*)>::swap
constexpr uint32_t FUNC_TEXT_RENDERER_ASSIGN = 0x6F6095;   // std::function<void(const string&, LWF::TextRenderer*)>::operator=
constexpr uint32_t Audio_getInstance = 0x5BECFD;
constexpr uint32_t Audio_getLayer = 0x5BF33D;
constexpr uint32_t Audio_stop = 0x5BF3AD;               // (mgr, int id, int fadeFrames)
constexpr uint32_t Layer_playSE = 0x5BF785;             // (layer, ResourceID, int, int)
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;  // (ResourceID, int fadeFrames)
constexpr uint32_t SE_SPEECH_NEXT = 0x1E56B968;         // the "next line" SE

constexpr uint32_t S_EMPTY = 0xDF8A1A;                  // ""
constexpr uint32_t S_FONT = 0xDF9F1F;                   // "cocostudio/publish/Cinema_Calligraphy_Web.ttf"
constexpr uint32_t S_MY_AVATAR = 0xE0627B;              // "my_avatar"
constexpr uint32_t S_SEQ_MAIN = 0xE06285;               // "seq_main"
constexpr uint32_t S_DRAMA_MAIN = 0xE0628E;             // "drama_main"

// SpeechDisp virtual slots (byte offsets)
constexpr uint32_t SPEECH_GET_TAG = 0x148;              // value stored as the font size
constexpr uint32_t SPEECH_SET_TTF_CONFIG = 0x280;       // (const TTFConfig&)
constexpr uint32_t SPEECH_SET_STRING = 0x2A8;           // (const std::string&)

// lambdas (std::function __func vtables)
constexpr uint32_t VT_ON_LOAD_LWF = 0xF69EB4;           // FlashDrama() $_0 [this]
constexpr uint32_t VT_ON_SOUND = 0xF69E88;              // FlashDrama() $_1 [this]
constexpr uint32_t VT_SEQ_MAIN_LOAD = 0xF69E5C;         // addEventListeners [this]
constexpr uint32_t VT_DRAMA_MAIN_LOAD = 0xF69E30;       // addEventListeners [this]
constexpr uint32_t VT_TEXT_RENDERER = 0xF69E04;         // addEventListeners [this]
constexpr uint32_t VT_SPEECH_DONE = 0xF69DD8;           // runSpeech [this, disp]

constexpr uint32_t ONE_F = 0x3F800000;                  // 1.0f (hash tables' max_load_factor)
constexpr uint32_t DEQUE_BLOCK = 0x155;                 // 341 strings per deque block

uint32_t lwfOf(FlashDrama *self) {
    return ld32(ld32(guest_addr(self) + FlashDrama::LWF_NODE) + wfn::LWFNODE_LWF);
}
// address of deque<string>::front(): map blocks at +4, start index at +0x10
uint32_t dequeFront(uint32_t deque) {
    uint32_t start = ld32(deque + 0x10);
    uint32_t block = call(UDIV, {start, DEQUE_BLOCK});
    uint32_t index = call(UMOD, {start, DEQUE_BLOCK});
    return ld32(ld32(deque + 4) + block * 4) + index * 12;
}
void playSpeechSE() {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(Audio_getLayer, {layer, call(Audio_getInstance, {}), 0});
    call(Layer_playSE, {ld32(layer), SE_SPEECH_NEXT, 0, 0});
    sharedRelease(layer);
}
// the std::function temporary `function(lambda)`, swapped into member and destroyed
void swapLambdaInto(uint32_t member, uint32_t swapFn, uint32_t vtable, uint32_t capture) {
    withLambda(vtable, {capture}, [&](uint32_t fo) { call(swapFn, {fo, member}); });
}
}  // namespace

FlashDrama *FlashDrama::construct(FlashDrama *self) {
    const uint32_t me = guest_addr(self);
    Frame f;
    const uint32_t sp = f.allocExact(0x98);   // the original frame: its TTFConfig temporary has unset bytes
    call(Avatar_ctor, {me, 0});
    rw::set<uint32_t>(self, 0, VTABLE);
    self->seqMovie = 0;
    self->mainMovie = 0;
    strInit(me + 0x580, S_EMPTY, 0);
    self->fontSize = 12;
    self->glyphs = 0;
    self->customGlyphs = 0;
    rw::set<uint8_t>(self, 0x598, 0);
    self->outlineSize = 0;
    rw::set<uint8_t>(self, 0x5A1, 0);
    rw::set<uint8_t>(self, 0x5A0, 0);
    call(AEABI_MEMSET, {me + 0x5A4, 0x14, 0});
    rw::set<uint32_t>(self, 0x5B8, ONE_F);
    rw::set<uint32_t>(self, 0x5C8, 0);
    rw::set<uint32_t>(self, 0x5C4, 0);
    rw::set<uint32_t>(self, 0x5C0, 0);
    rw::set<uint32_t>(self, 0x5BC, 0);
    rw::set<uint32_t>(self, 0x5CC, ONE_F);
    rw::set<uint32_t>(self, 0x5DC, 0);
    rw::set<uint32_t>(self, 0x5D8, 0);
    rw::set<uint32_t>(self, 0x5D4, 0);
    rw::set<uint32_t>(self, 0x5D0, 0);
    rw::set<uint32_t>(self, 0x5E0, ONE_F);
    call(AEABI_MEMSET, {me + 0x5E4, 0x1C, 0});
    rw::set<uint32_t>(self, 0x600, ONE_F);
    // the avatar wears the player's current style, in motion 27
    const uint32_t style = sp + 0x1C;
    call(AvatarUtil_getCurrentStyle, {style, 0});
    call(Avatar_changeStyle, {me, style});
    call(SafeBuffer_dtor, {style});
    call(Avatar_changeMotion, {me, 0x1B});
    // ttf = TTFConfig("cocostudio/publish/Cinema_Calligraphy_Web.ttf", 24)
    const uint32_t ttf = sp + 0x3C;
    strInit(ttf, S_FONT, 0x2D);
    st32(ttf + 0x0C, 0x18);
    st32(ttf + 0x10, 0);
    st32(ttf + 0x14, 0);
    st8(ttf + 0x18, 0);
    st32(ttf + 0x1C, 0);
    st8(ttf + 0x20, 0);
    st8(ttf + 0x21, 0);
    // this->ttf = std::move(ttf): the string moved, the rest copied word by word (with the 3 unset bytes after
    // distanceFieldEnabled)
    strMoveAssign(me + 0x580, ttf);
    for (uint32_t k = 0; k < 0x14; k += 4) rw::set<uint32_t>(self, 0x58C + k, ld32(ttf + 0x0C + k));
    rw::set<uint16_t>(self, 0x5A0, rw::load<uint16_t>(ttf + 0x20));
    swapLambdaInto(me + ON_LOAD_LWF, FUNC_STR_SWAP, VT_ON_LOAD_LWF, me);
    swapLambdaInto(me + ON_SOUND, FUNC_SOUND_SWAP, VT_ON_SOUND, me);
    return self;
}
RW_FUNCTION(0x6F2564, "FlashDrama::FlashDrama()", FlashDrama::construct);

FlashDrama *FlashDrama::constructWithLwf(FlashDrama *self, uint32_t lwfName) {
    construct(self);
    call(Avatar_setInstanceName, {guest_addr(self), S_MY_AVATAR});
    call(Avatar_changeLWF, {guest_addr(self), lwfName});
    return self;
}
RW_FUNCTION(0x6F2838, "FlashDrama::FlashDrama(const char*)", FlashDrama::constructWithLwf);

FlashDrama *FlashDrama::destruct(FlashDrama *self) {
    const uint32_t me = guest_addr(self);
    rw::set<uint32_t>(self, 0, VTABLE);
    for (uint32_t node = ld32(me + 0x5D8); node; node = ld32(node)) call(Ref_release, {ld32(node + 8)});
    for (uint32_t node = ld32(me + 0x5C4); node; node = ld32(node)) call(Ref_release, {ld32(node + 0x14)});
    const uint32_t end = self->seIds.end;
    for (uint32_t it = self->seIds.begin; it != end; it += 4)
        call(Audio_stop, {call(Audio_getInstance, {}), ld32(it), 0xFA});
    call(LINES_DTOR, {me + 0x5F0});
    call(VECTOR_INT_DTOR, {me + 0x5E4});
    call(ACTIVE_DTOR, {me + 0x5D0});
    call(SPEECH_BY_NAME_DTOR, {me + 0x5BC});
    call(PENDING_TEXTS_DTOR, {me + 0x5A8});
    strFree(me + 0x580);
    call(Avatar_dtor, {me});
    return self;
}
RW_FUNCTION(0x6F2884, "FlashDrama::~FlashDrama", FlashDrama::destruct);

void FlashDrama::destructDelete(FlashDrama *self) { call(fn::operator_delete, {guest_addr(destruct(self))}); }
RW_FUNCTION(0x6F29AC, "FlashDrama::~FlashDrama (deleting)", FlashDrama::destructDelete);

// Avatar::update, then per balloon name the first pending text that could be shown leaves the waiting list.
void FlashDrama::update(FlashDrama *self, float dt) {
    call(Avatar_update, {guest_addr(self), rw::fbits(dt)});
    for (uint32_t node = ld32(guest_addr(self) + 0x5B0); node; node = ld32(node)) {
        const uint32_t sentinel = node + 0x14;      // list<int>: end node at +0x14, size at +0x1C
        uint32_t link = node + 0x18;
        for (;;) {
            uint32_t item = ld32(link);
            if (item == sentinel) break;
            uint32_t shown = displayString(self, rw::host_ptr<guest::String>(node + 8), int32_t(ld32(item + 8)));
            link = item + 4;
            if (shown == 1) {
                st32(ld32(item) + 4, ld32(item + 4));     // prev->next = next
                st32(ld32(item + 4), ld32(item));         // next->prev = prev
                st32(node + 0x1C, ld32(node + 0x1C) - 1);
                call(fn::operator_delete, {item});
                break;
            }
        }
    }
}
RW_FUNCTION(0x6F29B8, "FlashDrama::update", FlashDrama::update);

// Show text `textId` in the balloon `name` unless it is busy: the text is split at '\f' into lines, the balloon
// takes the font, shows the first line and runs; 1 = shown (or empty), 0 = no such balloon / balloon busy.
uint32_t FlashDrama::displayString(FlashDrama *self, const guest::String *name, int32_t textId) {
    const uint32_t me = guest_addr(self);
    Frame f;
    const uint32_t sp = f.allocExact(0x98);
    uint32_t it = call(SPEECH_BY_NAME_FIND, {me + 0x5BC, guest_addr(name)});
    if (!it) return 0;
    const uint32_t disp = it + 0x14;          // SpeechDisp* in the map node
    if (call(ACTIVE_FIND, {me + 0x5D0, disp}) != 0) return 0;
    const uint32_t text = sp + 0x64;
    call(TextData_format, {text, 1, uint32_t(textId)});
    if (strSize(text) != 0) {
        const uint32_t lines = sp + 0x4C;     // deque<string>
        call(AEABI_MEMSET, {lines, 0x18, 0});
        uint32_t pos = 0;
        uint32_t found;
        do {
            found = call(STRING_FIND_CHAR, {text, 0x0C, pos});
            uint32_t n = found == 0xFFFFFFFFu ? found : found - pos;
            const uint32_t line = sp + 0x40;
            rw::call_exact(STRING_SUBSTR_CTOR, {line, text, pos, n, text});
            call(DEQUE_PUSH_BACK, {lines, line});
            strFree(line);
            pos = found + 1;
        } while (found != 0xFFFFFFFFu);
        uint32_t d = ld32(disp);
        rw::set<uint32_t>(self, 0x58C, vcall(d, SPEECH_GET_TAG, {d}));
        d = ld32(disp);
        vcall(d, SPEECH_SET_TTF_CONFIG, {d, me + 0x580});
        uint32_t front = dequeFront(lines);
        d = ld32(disp);
        vcall(d, SPEECH_SET_STRING, {d, front});
        call(Ref_retain, {ld32(disp)});
        call(ACTIVE_INSERT, {sp + 0x78, me + 0x5D0, disp});
        // pair<SpeechDisp*, deque<string>>(disp, std::move(lines))
        const uint32_t pair = sp + 0x24;
        st32(pair, ld32(disp));
        st32(pair + 4, ld32(lines));
        st32(pair + 8, ld32(lines + 4));
        st32(pair + 0xC, ld32(lines + 8));
        st32(pair + 0x10, ld32(lines + 0xC));
        st32(lines + 0xC, 0);
        st32(lines + 8, 0);
        st32(lines + 4, 0);
        st32(lines, 0);
        st32(pair + 0x14, ld32(lines + 0x10));
        st32(pair + 0x18, ld32(lines + 0x14));
        st32(lines + 0x10, 0);
        st32(lines + 0x14, 0);
        call(LINES_INSERT, {sp + 0x70, me + 0x5F0, pair});
        call(DEQUE_BASE_DTOR, {pair + 4});
        runSpeech(self, ld32(disp));
        call(DEQUE_BASE_DTOR, {lines});
    }
    strFree(text);
    return 1;
}
RW_FUNCTION(0x6F2A20, "FlashDrama::displayString", FlashDrama::displayString);

// Play the SE / switch the main BGM the LWF's sound events asked for.
void FlashDrama::updateSound(FlashDrama *self, float) {
    const uint32_t me = guest_addr(self);
    if (!rw::load<uint8_t>(me + SOUND_DIRTY)) return;
    rw::set<uint8_t>(self, SOUND_DIRTY, 0);
    uint32_t se = ld32(me + PENDING_SE);
    if (se) {
        Frame f;
        uint32_t id = f.alloc(4);
        uint32_t layer = f.alloc(8);
        call(Audio_getLayer, {layer, call(Audio_getInstance, {}), 0});
        uint32_t sound = call(Layer_playSE, {ld32(layer), se, 0, 0});
        sharedRelease(layer);
        st32(id, sound);
        uint32_t end = self->seIds.end;
        if (end < self->seIds.cap) {
            uint32_t base = 0;
            if (end) {
                st32(end, sound);
                base = self->seIds.end;
            }
            self->seIds.end = base + 4;
        } else {
            call(VECTOR_INT_PUSH_SLOW, {me + 0x5E4, id});
        }
        rw::set<uint32_t>(self, PENDING_SE, 0);
    }
    uint32_t bgm = ld32(me + PENDING_BGM);
    if (bgm) {
        call(AudioReplaceGameMainBGM, {bgm, 500});
        rw::set<uint32_t>(self, PENDING_BGM, 0);
    }
}
RW_FUNCTION(0x6F2C70, "FlashDrama::updateSound", FlashDrama::updateSound);

// Run the balloon's speech; when it is done, the lambda [this, disp] continues with the next line.
void FlashDrama::runSpeech(FlashDrama *self, uint32_t speech) {
    withLambda(VT_SPEECH_DONE, {guest_addr(self), speech}, [&](uint32_t fo) {
        call(SpeechDisp_runSpeechSet, {speech, call(CallFunc_create, {fo})});
    });
}
RW_FUNCTION(0x6F2D74, "FlashDrama::runSpeech", FlashDrama::runSpeech);

void FlashDrama::addEventListeners(FlashDrama *self) {
    const uint32_t me = guest_addr(self);
    call(Avatar_addEventListeners, {me});
    addMovieHandler(lwfOf(self), S_SEQ_MAIN, 8, "load", VT_SEQ_MAIN_LOAD, {me});
    addMovieHandler(lwfOf(self), S_DRAMA_MAIN, 10, "load", VT_DRAMA_MAIN_LOAD, {me});
    uint32_t lwf = lwfOf(self);
    withLambda(VT_TEXT_RENDERER, {me}, [&](uint32_t fo) { call(FUNC_TEXT_RENDERER_ASSIGN, {lwf + 0x110, fo}); });
}
RW_FUNCTION(0x6F2E20, "FlashDrama::addEventListeners", FlashDrama::addEventListeners);

// Hurry every showing balloon: a running one jumps to the end of its line and drops it (the balloon is released
// when it has no more lines), an idle one shows its next line. 1 = no balloon was showing.
uint32_t FlashDrama::finalizeSpeeches(FlashDrama *self) {
    const uint32_t me = guest_addr(self);
    if (ld32(me + 0x5DC) == 0) return 1;
    uint32_t node = ld32(me + 0x5D8);
    while (node) {
        const uint32_t key = node + 8;
        uint32_t lines = call(LINES_AT, {me + 0x5F0, key});
        uint32_t disp = ld32(node + 8);
        if (call(Node_getNumberOfRunningActions, {disp}) != 0) {
            call(SpeechDisp_toEndSpeech, {ld32(key)});
            call(DEQUE_POP_FRONT, {lines});
            if (ld32(lines + 0x14) == 0) {
                call(LINES_ERASE, {me + 0x5F0, key});
                call(Ref_release, {ld32(node + 8)});
                node = call(ACTIVE_ERASE, {me + 0x5D0, node});
            } else {
                node = ld32(node);
            }
        } else {
            playSpeechSE();
            uint32_t front = dequeFront(lines);
            uint32_t d = ld32(node + 8);
            vcall(d, SPEECH_SET_STRING, {d, front});
            runSpeech(self, ld32(node + 8));
            node = ld32(node);
        }
    }
    return 0;
}
RW_FUNCTION(0x6F3194, "FlashDrama::finalizeSpeeches", FlashDrama::finalizeSpeeches);

// Continue the drama (both movies one frame on) once no balloon is showing.
void FlashDrama::nextFrame(FlashDrama *self) {
    if (finalizeSpeeches(self) != 1) return;
    bool advanced = false;
    uint32_t movie = self->seqMovie;
    if (movie && rw::load<uint8_t>(movie + 0xAD) == 0) {
        call(wfn::Movie_GotoAndPlay, {movie, ld32(movie + wfn::MOVIE_CURRENT_FRAME) + 1});
        advanced = true;
    }
    movie = self->mainMovie;
    if (movie && rw::load<uint8_t>(movie + 0xAD) == 0) {
        call(wfn::Movie_GotoAndPlay, {movie, ld32(movie + wfn::MOVIE_CURRENT_FRAME) + 1});
    } else if (!advanced) {
        return;
    }
    playSpeechSE();
}
RW_FUNCTION(0x6F32FC, "FlashDrama::nextFrame", FlashDrama::nextFrame);

// The sequence movie reached its last frame.
uint32_t FlashDrama::isFinished(const FlashDrama *self) {
    uint32_t movie = self->seqMovie;
    if (!movie) return 0;
    return ld32(movie + wfn::MOVIE_CURRENT_FRAME) == ld32(movie + wfn::MOVIE_TOTAL_FRAMES) ? 1 : 0;
}
RW_FUNCTION(0x6F33D4, "FlashDrama::isFinished", FlashDrama::isFinished);
