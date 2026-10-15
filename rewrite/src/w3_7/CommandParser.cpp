// CommandParser: the script commands embedded in LWF movies (frame labels / fscommands such as "playSE_17",
// "knockback_Enemy_3", "dispString_1_2_..."). parse() splits the command at '_' and calls the std::function slot
// registered for it; a slot left empty ignores the command. The owner (FlashNode, Enemy, RaidBoss...) fills the
// slots. 23 std::function slots, 24 bytes apart (552 bytes).
#include "core/Guest.hpp"

namespace {
namespace fn {
constexpr uint32_t STRCHR = 0x410198;                     // strchr (import)
constexpr uint32_t STRNCMP = 0x415094;                    // strncmp (import)
constexpr uint32_t STRLEN = 0x40E08C;                     // strlen (import)
constexpr uint32_t ATOI = 0x410A14;                       // atoi (import)
constexpr uint32_t CALL_VOID = 0x470E69;                  // std::function<void()>::operator()
constexpr uint32_t CALL_STRING = 0x4E28AD;                // std::function<void(const char*)>::operator()
constexpr uint32_t CALL_INT = 0x4F321D;                   // std::function<void(int)>::operator()
constexpr uint32_t CALL_DISP_STRING = 0x4F329D;           // std::function<void(int, int, const std::vector<int>*)>::operator()
constexpr uint32_t LONG_VECTOR_PUSH_SLOW = 0x4F3351;      // std::vector<long>::__push_back_slow_path(long&&)
constexpr uint32_t LONG_VECTOR_BASE_DTOR = 0x4F3325;      // std::__vector_base<long>::~__vector_base
constexpr uint32_t INT_VECTOR_PUSH_SLOW = 0x47D709;       // std::vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t INT_VECTOR_BASE_DTOR = 0x478961;       // std::__vector_base<int>::~__vector_base
}  // namespace fn

// command names (the code compares from the second character on: the first one picked the branch)
namespace str {
constexpr uint32_t isPlayMoveSE = 0xDFCD54, isChangeFace = 0xDFCD61, isPlaySE = 0xDFCD77, isPlaySound = 0xDFCD80;
constexpr uint32_t isSkip = 0xDFC017, isMove = 0xDFCDC6, isExpand = 0xDFCDCD;
constexpr uint32_t char_ = 0xDFCDC1, die = 0xDFCDBD, end = 0xDFCD73, effectStart = 0xDFCDAB, enemy = 0xDFCDB7;
constexpr uint32_t eff = 0xDFCDF0, face = 0xDFCD6E, frame = 0xDFCDDB, MIn = 0xDFCDEC, tgtComment = 0xDFCDFD;
constexpr uint32_t dispString = 0xDFCE08, wIn = 0xDFCDF4, wOut = 0xDFCDF8, knockback = 0xDFCD9B, Enemy = 0xDFCDA5;
constexpr uint32_t playSE = 0xDFCD8C, playBGM = 0xDFCD93, rate = 0xDFCDD6, mIn = 0xDFCDE8, expand = 0xDFCDE1;
}  // namespace str

// std::function slots (byte offsets in the parser); the callable pointer is at slot + 16
enum Slot : uint32_t {
    KNOCKBACK = 0,          // function<void(int)>        knockback_<n> / knockback_Enemy_<n>
    EFFECT_START = 24,      // function<void()>           effectStart
    PLAY_SOUND = 48,        // function<void()>           isPlaySE / isPlaySound
    PLAY_MOVE_SE = 72,      // function<void()>           isPlayMoveSE
    PLAY_SE = 96,           // function<void(const char*)> playSE_<id> / se...
    PLAY_BGM = 120,         // function<void(const char*)> playBGM_<id>
    END = 144,              // function<void()>           end
    CHANGE_FACE = 168,      // function<void()>           isChangeFace
    FACE = 192,             // function<void(const char*)> face_<name>
    EXPAND = 216,           // function<void()>           isExpand
    RATE = 240,             // function<void(int)>        rate_<n>
    FRAME_EXPAND = 264,     // function<void(int)>        frame_expand_<n>
    ENEMY_DIE = 288,        // function<void()>           enemy_die
    CHAR_DIE = 312,         // function<void()>           char_die
    SKIP = 336,             // function<void()>           isSkip
    MOVE = 360,             // function<void()>           isMove
    FRAME_IN = 384,         // function<void(int)>        frame_mIn_<n>
    M_IN = 408,             // function<void()>           MIn
    EFF = 432,              // function<void()>           eff
    W_IN = 456,             // function<void()>           wIn
    W_OUT = 480,            // function<void()>           wOut
    TGT_COMMENT = 504,      // function<void(const char*)> tgtComment_<text>
    DISP_STRING = 528,      // function<void(int, int, const vector<int>*)> dispString_<a>_<b>_<c>..._<last>
};
constexpr uint32_t SLOT_COUNT = 23, SLOT_SIZE = 24;

uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
bool same(uint32_t s, uint32_t literal, uint32_t n) { return rw::call(fn::STRNCMP, {s, literal, n}) == 0; }

// std::vector<T> (4-byte T) push_back as inlined by the compiler: construct in place when there is room (the
// placement-new null check included), else the slow path. `value` is a frame slot holding the value.
void pushBack(uint32_t vec, uint32_t value, uint32_t slowPath) {
    uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t base = 0;
        if (end) {
            rw::store32(rw::host_ptr<uint8_t>(end), ld32(value));
            base = ld32(vec + 4);
        }
        *rw::frame_ptr<uint32_t>(vec + 4) = base + 4;
    } else {
        rw::call(slowPath, {vec, value});
    }
}
}  // namespace

// parse(command, movie, button): split `command` at '_' and run the matching slot.
static void CommandParser_parse(uint32_t self, const guest::String *command, uint32_t /*movie*/, uint32_t /*button*/) {
    guest::Frame f(0x4C);
    const uint32_t parts = f.addr(0x3C);       // std::vector<long>: offsets of the '_' separators, then strlen
    const uint32_t value = f.addr(0x38), length = f.addr(0x34);
    auto slot = [&](uint32_t s) { return self + s; };
    auto has = [&](uint32_t s) { return ld32(self + s + 16) != 0; };
    auto callVoid = [&](uint32_t s) {
        if (has(s)) rw::call(fn::CALL_VOID, {slot(s)});
    };

    const uint32_t text = command->data();
    for (uint32_t k = 0; k < 12; k += 4) *rw::frame_ptr<uint32_t>(parts + k) = 0;
    for (uint32_t p = rw::call(fn::STRCHR, {text, '_'}); p; p = rw::call(fn::STRCHR, {p + 1, '_'})) {
        *rw::frame_ptr<uint32_t>(value) = p - text;
        pushBack(parts, value, fn::LONG_VECTOR_PUSH_SLOW);
    }
    *rw::frame_ptr<uint32_t>(length) = rw::call(fn::STRLEN, {text});
    pushBack(parts, length, fn::LONG_VECTOR_PUSH_SLOW);

    // the text after separator k (the argument), and the character right after it
    auto part = [&](uint32_t k) { return text + ld32(ld32(parts) + 4 * k) + 1; };
    auto partStart = [&](uint32_t k) { return text + ld32(ld32(parts) + 4 * k); };   // points at the '_'

    const uint8_t first = ld8(text);
    const uint32_t rest = text + 1;
    bool fallThrough = false;   // the branch did not match: the generic chain below decides

    if (first == 'i') {
        if (same(rest, str::isPlayMoveSE + 1, 11)) callVoid(PLAY_MOVE_SE);
        else if (same(rest, str::isChangeFace + 1, 11)) callVoid(CHANGE_FACE);
        else if (same(rest, str::isPlaySE + 1, 7) || same(rest, str::isPlaySound + 1, 10)) callVoid(PLAY_SOUND);
        else if (same(rest, str::isSkip + 1, 5)) callVoid(SKIP);
        else if (same(rest, str::isMove + 1, 5)) callVoid(MOVE);
        else if (same(rest, str::isExpand + 1, 7)) callVoid(EXPAND);
    } else if (int8_t(first) > 0x62) {
        switch (first) {
        case 'c':
            if (same(rest, str::char_ + 1, 3)) {
                uint32_t q = partStart(0);
                if (ld8(q + 1) == 'd' && same(q + 2, str::die + 1, 2)) callVoid(CHAR_DIE);
            } else {
                fallThrough = true;
            }
            break;
        case 'd':
            fallThrough = true;   // dispString
            break;
        case 'e':
            if (same(rest, str::end + 1, 2)) callVoid(END);
            else if (same(rest, str::effectStart + 1, 10)) callVoid(EFFECT_START);
            else if (same(rest, str::enemy + 1, 4)) {
                uint32_t q = partStart(0);
                if (ld8(q + 1) == 'd' && same(q + 2, str::die + 1, 2)) callVoid(ENEMY_DIE);
            } else if (same(rest, str::eff + 1, 2)) callVoid(EFF);
            else fallThrough = true;
            break;
        case 'f':
            if (same(rest, str::face + 1, 3)) {
                if (has(FACE)) rw::call(fn::CALL_STRING, {slot(FACE), part(0)});
            } else if (same(rest, str::frame + 1, 4)) {
                uint32_t q = partStart(0);
                uint8_t kind = ld8(q + 1);
                if (kind == 'm') {
                    if (same(q + 2, str::mIn + 1, 2) && has(FRAME_IN))
                        rw::call(fn::CALL_INT, {slot(FRAME_IN), rw::call(fn::ATOI, {part(1)})});
                } else if (kind == 'e') {
                    if (same(q + 2, str::expand + 1, 5) && has(FRAME_EXPAND))
                        rw::call(fn::CALL_INT, {slot(FRAME_EXPAND), rw::call(fn::ATOI, {part(1)})});
                }
            } else {
                fallThrough = true;
            }
            break;
        case 'k':
            if (same(rest, str::knockback + 1, 8)) {
                // knockback_<n>, or knockback_Enemy_<n> (the value is then the second argument)
                uint32_t index = 0;
                uint32_t q = partStart(0);
                if (ld8(q + 1) == 'E') index = same(q + 2, str::Enemy + 1, 4) ? 1 : 0;
                if (has(KNOCKBACK)) rw::call(fn::CALL_INT, {slot(KNOCKBACK), rw::call(fn::ATOI, {part(index)})});
            }
            break;
        case 'p':
            if (same(rest, str::playSE + 1, 5)) {
                if (has(PLAY_SE)) rw::call(fn::CALL_STRING, {slot(PLAY_SE), part(0)});
            } else if (same(rest, str::playBGM + 1, 6)) {
                if (has(PLAY_BGM)) rw::call(fn::CALL_STRING, {slot(PLAY_BGM), part(0)});
            }
            break;
        case 'r':
            if (same(rest, str::rate + 1, 3) && has(RATE))
                rw::call(fn::CALL_INT, {slot(RATE), rw::call(fn::ATOI, {part(0)})});
            break;
        case 's':
            if (ld8(text + 1) == 'e' && has(PLAY_SE)) rw::call(fn::CALL_STRING, {slot(PLAY_SE), part(0)});
            break;
        case 't':
            fallThrough = true;   // tgtComment
            break;
        case 'w':
            fallThrough = true;   // wIn / wOut
            break;
        default:
            break;
        }
    } else if (first == 'M') {
        if (same(rest, str::MIn + 1, 2)) callVoid(M_IN);
        else fallThrough = true;
    }

    if (fallThrough) {
        if (first == 'd') {
            if (same(rest, str::dispString + 1, 9) && has(DISP_STRING)) {
                // dispString_<a>_<b>[_<c>..._<y>]_<z>: (a, b, {c..y}); the last argument is not passed
                uint32_t begin = ld32(parts);
                uint32_t count = uint32_t(int32_t(ld32(parts + 4) - begin) >> 2);
                uint32_t a = rw::call(fn::ATOI, {text + ld32(begin) + 1});
                if (count < 3) {
                    rw::call(fn::CALL_DISP_STRING, {slot(DISP_STRING), a, 0, 0});
                } else {
                    uint32_t b = rw::call(fn::ATOI, {text + ld32(begin + 4) + 1});
                    const uint32_t numbers = f.addr(0x28), number = f.addr(0x24);   // std::vector<int>, its push value
                    for (uint32_t k = 0; k < 12; k += 4) *rw::frame_ptr<uint32_t>(numbers + k) = 0;
                    if (count >= 4) {
                        for (uint32_t i = 2;; i++) {
                            *rw::frame_ptr<uint32_t>(number) = rw::call(fn::ATOI, {part(i)});
                            pushBack(numbers, number, fn::INT_VECTOR_PUSH_SLOW);
                            if (i + 2 >= count) break;
                        }
                    }
                    rw::call(fn::CALL_DISP_STRING, {slot(DISP_STRING), a, b, numbers});
                    rw::call(fn::INT_VECTOR_BASE_DTOR, {numbers});
                }
            }
        } else if (first == 't') {
            if (same(rest, str::tgtComment + 1, 9) && has(TGT_COMMENT))
                rw::call(fn::CALL_STRING, {slot(TGT_COMMENT), part(0)});
        } else if (first == 'w') {
            if (same(rest, str::wIn + 1, 2)) callVoid(W_IN);
            else if (same(rest, str::wOut + 1, 3)) callVoid(W_OUT);
        }
    }
    rw::call(fn::LONG_VECTOR_BASE_DTOR, {parts});
}
RW_FUNCTION(0x4F28F8, "CommandParser::parse(const std::string&, LWF::Movie*, LWF::Button*)", CommandParser_parse);

// ~CommandParser: destroy the std::function slots, last to first (destroy in place / destroy_deallocate).
static uint32_t CommandParser_destruct(uint32_t self) {
    for (uint32_t k = SLOT_COUNT; k-- > 0;) {
        uint32_t buf = self + k * SLOT_SIZE;
        uint32_t callable = ld32(buf + 16);
        if (callable == buf) rw::vcall(rw::host_ptr<uint8_t>(callable), 4, {callable});
        else if (callable) rw::vcall(rw::host_ptr<uint8_t>(callable), 5, {callable});
    }
    return self;
}
RW_FUNCTION(0x5C3830, "CommandParser::~CommandParser", CommandParser_destruct);
