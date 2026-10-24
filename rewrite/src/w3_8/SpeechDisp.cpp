// SpeechDisp: a cocos2d::Label that types its text out letter by letter (the story / light scene speech boxes).
// runSpeechSet hides every letter but the first and shows letter i after i * 0.05 s, then runs the caller's
// CallFunc once the whole text is out; toEndSpeech shows everything at once.
#include "core/Guest.hpp"

namespace {

constexpr uint32_t SIZE = 0x4D0;
constexpr uint32_t VTABLE = 0xF97418;            // _ZTV10SpeechDisp + 8
constexpr uint32_t VTABLE_PROTOCOL_A = 0xF976FC; // secondary vtables (Label's LabelProtocol / BlendProtocol bases)
constexpr uint32_t VTABLE_PROTOCOL_B = 0xF9771C;
constexpr float LETTER_DELAY = 0.05f;

namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;            // __aeabi_memset(dst, n, c)
constexpr uint32_t label_ctor = rwlib::cocos2d::Label::ctor_base;              // Label::Label(FontAtlas*, TextHAlignment, TextVAlignment, bool, bool)
constexpr uint32_t label_dtor = rwlib::cocos2d::Label::dtor_base;              // Label::~Label()
constexpr uint32_t label_getStringLength = rwlib::cocos2d::Label::getStringLength;   // Label::getStringLength() const
constexpr uint32_t spriteBatchNode_init = rwlib::cocos2d::SpriteBatchNode::init;    // SpriteBatchNode::init()
constexpr uint32_t ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t delayTime_create = rwlib::cocos2d::DelayTime::create;        // DelayTime::create(float)
constexpr uint32_t show_create = rwlib::cocos2d::Show::create;             // Show::create()
constexpr uint32_t sequence_createWithTwoActions = rwlib::cocos2d::Sequence::createWithTwoActions;
constexpr uint32_t sequence_create = rwlib::cocos2d::Sequence::create;         // Sequence::create(FiniteTimeAction*, ...) (null-terminated)
}  // namespace g

constexpr uint32_t GET_LETTER = 0x2C8;   // Label::getLetter(int) (virtual, byte offset)
constexpr uint32_t SET_VISIBLE = 0xB0;   // Node::setVisible(bool)

uint32_t vcallAt(uint32_t obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(rw::load<uint32_t>(rw::load<uint32_t>(obj) + offset), args);
}
int32_t stringLength(uint32_t self) { return int32_t(rw::call(g::label_getStringLength, {self})); }

}  // namespace

namespace SpeechDispFns {

uint32_t create() {
    uint32_t self = guest::newObject(SIZE);
    rw::call(g::aeabi_memset, {self, SIZE, 0});
    rw::call(g::label_ctor, {self, 0, 0, 0, 0, 0});
    rw::save<uint32_t>(self, VTABLE);
    rw::save<uint32_t>(self + 0x270, VTABLE_PROTOCOL_A);
    rw::save<uint32_t>(self + 0x2F0, VTABLE_PROTOCOL_B);
    if (rw::call(g::spriteBatchNode_init, {self}) == 1) {
        rw::call(g::ref_autorelease, {self});
        return self;
    }
    vcallAt(self, 4, {self});   // deleting destructor
    return 0;
}
RW_FUNCTION(0x601C20, "SpeechDisp::create", create);

void destructDelete(uint32_t self) {
    uint32_t p = rw::call(g::label_dtor, {self});
    guest::deleteObject(p);
}
RW_FUNCTION(0x6073C4, "SpeechDisp::~SpeechDisp (deleting)", destructDelete);

// Show the whole text now. r0 = the last getStringLength (the original's return register).
int32_t toEndSpeech(uint32_t self) {
    rw::call(g::node_stopAllActions, {self});
    int32_t n = stringLength(self);
    if (n < 1) return n;
    for (int32_t i = 0; i < n; ) {
        uint32_t letter = vcallAt(self, GET_LETTER, {self, uint32_t(i)});
        if (letter) {
            vcallAt(letter, SET_VISIBLE, {letter, 1});
            rw::call(g::node_stopAllActions, {letter});
        }
        i++;
        n = stringLength(self);
    }
    return n;
}
RW_FUNCTION(0x4929F4, "SpeechDisp::toEndSpeech", toEndSpeech);

// Type the text out (one letter every 0.05 s), then run `done`.
uint32_t runSpeechSet(uint32_t self, uint32_t done) {
    rw::call(g::node_stopAllActions, {self});
    if (stringLength(self) >= 1) {
        int32_t i = 0;
        do {
            uint32_t letter = vcallAt(self, GET_LETTER, {self, uint32_t(i)});
            if (letter) {
                rw::call(g::node_stopAllActions, {letter});
                vcallAt(letter, SET_VISIBLE, {letter, i == 0 ? 1u : 0u});
                uint32_t delay = rw::call(g::delayTime_create, {rw::fbits(float(i) * LETTER_DELAY)});
                uint32_t show = rw::call(g::show_create, {});
                uint32_t seq = rw::call(g::sequence_createWithTwoActions, {delay, show});
                rw::call(g::node_runAction, {letter, seq});
            }
            i++;
        } while (i < stringLength(self));
    }
    float total = float(stringLength(self)) * LETTER_DELAY;
    uint32_t delay = rw::call(g::delayTime_create, {rw::fbits(total)});
    uint32_t seq = rw::call(g::sequence_create, {delay, done, 0});
    return rw::call(g::node_runAction, {self, seq});
}
RW_FUNCTION(0x492A4C, "SpeechDisp::runSpeechSet", runSpeechSet);

// std::string::find(char c, size_t pos) (libc++, instantiated here): the index of c at or after pos, or npos
uint32_t stringFindChar(const guest::String *str, uint32_t c, uint32_t pos) {
    const uint32_t size = str->size(), data = str->data();
    if (size <= pos) return 0xFFFFFFFFu;
    const uint32_t hit = rw::call(0x41CDA4, {data + pos, c & 0xFF, size - pos});   // memchr
    return hit ? hit - data : 0xFFFFFFFFu;
}
RW_FUNCTION(0x601CD0, "std::string::find(char, size_t) (sub_601CD0)", stringFindChar);

}  // namespace SpeechDispFns
