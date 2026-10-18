// TextFormat::execute(std::string* out, const char* text, va_list): expands a localized text's placeholders.
//
// A text is plain characters with `${...}` tags. `\` escapes the next character, a lone `$` is kept, a `$` at the
// very end is dropped. A tag's body is a name, then optional `:` modifiers, each one character after a colon:
//   ${N}          the N-th argument (N >= 1), printed with "%d"
//   ${N:%fmt}     ... printed with the printf format "%fmt" (the format runs until the tag closes)
//   ${N:#a#b#c}   ... picks a word by the number: 1 -> a, 0 -> b, n -> the n-th, clamped to the last (plurals)
//   ${N:o}        ... with the English ordinal suffix ("1st", "12th"; only when info/lang is "en")
//   ${N:m}        ... as an English month name ("Jan.".."Dec."; only in English, otherwise the number)
//   ${PLAYER} the player's name, ${UNION} the player's union name, ${SPACE} a space.
// All numbered tags are formatted in one printf call (their formats in argument order, joined by \x03), the
// pieces are post-processed per tag, then every tag's text is inserted into the plain text at its position.
#include <algorithm>

#include "StringBuffer.hpp"

namespace textformat {

namespace {
// ------------------------------------------------------------------------------------------------ the frame
// The original's frame (5 pushed registers + 0xDAC), kept exactly: the map's end node is a frame address that
// its heap nodes point to, and a tag is built in the frame and copied whole (unset bytes included) to the heap.
constexpr uint32_t FRAME_SIZE = 0xDC0;
constexpr uint32_t F_UNION_TEXT = 0x84;   // shared_ptr<bg::Data> from hole::text::getUnionName
constexpr uint32_t F_MAP = 0x8C;          // std::map<unsigned, Tag*> (begin node, end node = root slot, size)
constexpr uint32_t F_CHAR = 0x9A;         // char temporaries for push_back
constexpr uint32_t F_TAGS = 0x9C;         // std::vector<Tag>
constexpr uint32_t F_TEXT = 0xA8;         // std::vector<char>: the plain text
constexpr uint32_t F_LANG = 0xB4;         // shared_ptr<bg::Data> of info/lang
constexpr uint32_t F_END_PTR = 0xBC;      // strtol's end pointer
constexpr uint32_t F_TAG_CHAR = 0xC0;     // char temporaries for push_back (0xC0..0xC3)
constexpr uint32_t F_NEW_OPTION = 0xC4;   // the empty std::vector<char> a '#' appends
constexpr uint32_t F_ADD_CHAR = 0xD3;     // the character being added to a tag
constexpr uint32_t F_PARENT = 0xD4;       // map insertion: the parent / found node
constexpr uint32_t F_NUMBER = 0xF4;       // std::string of a formatted piece (for stoll)
constexpr uint32_t F_KEY = 0x100;         // the ordinal map's lookup keys (0x100, 0x104)
constexpr uint32_t F_UNION_ID = 0x108;
constexpr uint32_t F_PLAYER_NAME = 0x10C; // 0x21 bytes
constexpr uint32_t F_VALUES = 0x130;      // StringBuffer: the printf result
constexpr uint32_t F_FORMAT = 0x534;      // StringBuffer: the joined formats
constexpr uint32_t F_PENDING = 0x938;     // the Tag being opened
constexpr uint32_t F_SUFFIXES = 0xD78;    // the ordinal map's initializer list (6 pairs)

// ------------------------------------------------------------------------------------------------ a tag
// Tag (0x440 bytes): kind (0 none, 1 argument, 2 player, 3 union, 4 space), position in the plain text, flags,
// the name, the printf format, the '#' words, the modifiers, the argument number and the finished text.
constexpr uint32_t TAG_SIZE = 0x440;
constexpr uint32_t T_KIND = 0x00, T_POSITION = 0x04, T_OPEN = 0x08, T_AFTER_COLON = 0x09, T_IN_FORMAT = 0x0A;
constexpr uint32_t T_IN_WORDS = 0x0B, T_NAME_DONE = 0x0C;   // T_NAME_DONE is never set: name characters always count
constexpr uint32_t T_NAME = 0x10, T_FORMAT = 0x1C, T_WORDS = 0x28;   // vector<char>, vector<char>, vector<vector<char>>
constexpr uint32_t T_ORDINAL = 0x34, T_MONTH = 0x35, T_DEFAULT_FORMAT = 0x36, T_ARGUMENT = 0x38, T_TEXT = 0x3C;
constexpr uint32_t T_COPIED = 0x34;   // from here to the end the move copies the bytes (memcpy 0x40C)
constexpr uint32_t ARGUMENT = 1, PLAYER = 2, UNION = 3, SPACE = 4;

// ------------------------------------------------------------------------------------------------ globals
constexpr uint32_t IS_ENGLISH = 0x1003450;       // static bool: info/lang is "en"
constexpr uint32_t LANG_CHECKED = 0x1003451;
constexpr uint32_t SUFFIX_GUARD = 0x1003454;     // static std::unordered_map<int,int> suffixOf (its guard)
constexpr uint32_t SUFFIX_MAP = 0x1003458;
constexpr uint32_t SUFFIXES = 0xF555E8;          // "st", "nd", "rd", "th"
constexpr uint32_t MONTHS = 0xF555B8;            // "Jan.".."Dec."
constexpr uint32_t GOT_INT_MAP_DTOR = 0xFD610C;  // unordered_map<int,int>::~unordered_map
constexpr uint32_t GOT_DSO_HANDLE = 0xFD604C;
constexpr uint32_t GOT_EXCEPTION_VTABLE = 0xFD6110, GOT_EXCEPTION_TYPEINFO = 0xFD6114, GOT_EXCEPTION_DTOR = 0xFD6118;

constexpr uint32_t S_INFO_LANG = 0xDFA0DF, S_EN = 0xDFA0E9;
constexpr uint32_t S_PLAYER = 0xDFA0CC, S_UNION = 0xDFA0D3, S_SPACE = 0xDFA0D9;
constexpr uint32_t S_JOIN = 0xDFA080;   // "%s%c"
constexpr uint32_t S_TH = 0xDFA0C9, S_SPACE_TEXT = 0xDF9E16;   // "th", " "
constexpr uint32_t SEPARATOR = 3;

// ------------------------------------------------------------------------------------------------ calls
constexpr uint32_t STRLEN = 0x40E08C, STRCMP = 0x410234, STRTOL = 0x4101A4, STRCHR = 0x410198;   // imports
constexpr uint32_t AEABI_MEMSET = 0x40E7A0, AEABI_MEMCPY = 0x40E164, CXA_ATEXIT = 0x40DCD8;      // imports
constexpr uint32_t CHARS_PUSH_SLOW_COPY = 0x494045;   // vector<char>::__push_back_slow_path(const char&)
constexpr uint32_t CHARS_PUSH_SLOW_MOVE = 0x4940ED;   // vector<char>::__push_back_slow_path(char&&)
constexpr uint32_t WORDS_PUSH_SLOW = 0x4946D5;        // vector<vector<char>>::__push_back_slow_path(vector&&)
constexpr uint32_t WORDS_DTOR = 0x49489D;             // __vector_base<vector<char>>::~__vector_base
constexpr uint32_t TAGS_DTOR = 0x493E05;              // vector<Tag>::~vector (sub_493E04)
constexpr uint32_t TAGS_LENGTH_ERROR = 0x47B66D;      // __vector_base_common::__throw_length_error (sub_47B66C)
constexpr uint32_t MAP_DESTROY = 0x494195;            // map<unsigned, Tag*>::destroy(node) (sub_494194)
constexpr uint32_t TREE_BALANCE = 0x493F5B;           // __tree_balance_after_insert
constexpr uint32_t STRING_INIT = 0x47CD93;            // std::string::__init(const char*, size) (sub_47CD92)
constexpr uint32_t STRING_INSERT = 0x493E85;          // std::string::insert(pos, const char*, n) (sub_493E84)
constexpr uint32_t STOLL = 0xC942E9;                  // std::stoll(const string&, size_t*, int)
constexpr uint32_t INT_MAP_CTOR = 0x4941C1;           // unordered_map<int,int>(initializer_list)
constexpr uint32_t INT_MAP_FIND = 0x494251;           // __hash_table<int,int>::find
constexpr uint32_t VFS_READ_DATA = 0x8DD46D;          // bg::vfs::readData(shared_ptr<Data>*, path)
constexpr uint32_t APIMANAGER_GET_INSTANCE = 0x4BA8C9;
constexpr uint32_t SAFEBUFFER_READ = 0x45A269;        // SafeBuffer::read(dst, src, n, index)
constexpr uint32_t GET_UNION_NAME = 0x492B15;         // hole::text::getUnionName
constexpr uint32_t CXA_ALLOCATE_EXCEPTION = 0xC98131, CXA_THROW = 0xC983D5;
constexpr uint32_t SB_SET = 0x4914A7, SB_CONCAT = 0x4914C5, SB_FSET = 0x491501, SB_FCONCAT = 0x491571;
constexpr uint32_t SB_VFSET = 0x491481, SB_REPLACE = 0x491609;
constexpr uint32_t SB_LENGTH = 0x400;
constexpr uint32_t API_USER_PROFILE = 0x74, API_USER_UNION = 0x94;   // APIManager: SafeBuffers
constexpr uint32_t PROFILE_NAME = 0x14, PROFILE_NAME_SIZE = 0x21, USER_UNION_ID = 0x30;

uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
void st32(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
void st8(uint32_t a, uint8_t v) { rw::save<uint8_t>(a, v); }

// vector<char>::push_back(c); the slow path (reallocation) reads the character from the frame byte `temp`
void pushChar(uint32_t chars, uint8_t c, uint32_t slowPath, uint32_t temp) {
    const uint32_t end = ld32(chars + 4);
    if (end < ld32(chars + 8)) {
        st8(end, c);
        st32(chars + 4, end + 1);
    } else {
        *rw::frame_ptr<uint8_t>(temp) = c;
        rw::call(slowPath, {chars, temp});
    }
}

// a vector's storage freed (as its destructor)
void freeVector(uint32_t v) {
    const uint32_t begin = ld32(v);
    if (!begin) return;
    if (ld32(v + 4) != begin) st32(v + 4, begin);
    guest::deleteObject(begin);
}

void destroyTag(uint32_t tag) {
    rw::call(WORDS_DTOR, {tag + T_WORDS});
    freeVector(tag + T_FORMAT);
    freeVector(tag + T_NAME);
}

// Tag's move constructor: the scalars, the three vectors (the source's emptied), the rest copied bytewise
void moveTag(uint32_t dst, uint32_t src) {
    for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
    st8(dst + 0xC, ld8(src + 0xC));
    for (uint32_t v : {T_NAME, T_FORMAT, T_WORDS})
        for (uint32_t k = 0; k < 12; k += 4) {
            st32(dst + v + k, ld32(src + v + k));
            st32(src + v + k, 0);
        }
    rw::call(AEABI_MEMCPY, {dst + T_COPIED, src + T_COPIED, TAG_SIZE - T_COPIED});
}

// tags.push_back(std::move(pending)), libc++'s growth inlined
void pushTag(uint32_t tags, uint32_t pending) {
    const uint32_t begin = ld32(tags), end = ld32(tags + 4), cap = ld32(tags + 8);
    if (end < cap) {
        moveTag(end, pending);
        st32(tags + 4, end + TAG_SIZE);
        return;
    }
    const uint32_t size = (end - begin) / TAG_SIZE, needed = size + 1;
    if (needed >= 0x3C3C3D) rw::call(TAGS_LENGTH_ERROR, {tags});   // max_size() exceeded: throws
    const uint32_t capacity = (cap - begin) / TAG_SIZE;
    const uint32_t grown = capacity > 0x1E1E1D ? 0x3C3C3C : std::max(2 * capacity, needed);
    const uint32_t storage = grown ? guest::newObject(TAG_SIZE * grown) : 0;
    const uint32_t storageEnd = storage + TAG_SIZE * grown;
    const uint32_t slot = storage + TAG_SIZE * size;
    if (slot) moveTag(slot, pending);
    // the old tags move to just below the new one, last first
    uint32_t to = slot;
    for (uint32_t from = end; from != begin;) {
        from -= TAG_SIZE;
        to -= TAG_SIZE;
        moveTag(to, from);
    }
    st32(tags, slot - TAG_SIZE * size);
    st32(tags + 4, storage + TAG_SIZE * needed);
    st32(tags + 8, storageEnd);
    for (uint32_t e = end; e != begin; e -= TAG_SIZE) destroyTag(e - TAG_SIZE);
    if (begin) guest::deleteObject(begin);
}

// the last tag still open, or 0
uint32_t lastOpenTag(uint32_t tags) {
    const uint32_t begin = ld32(tags);
    for (uint32_t e = ld32(tags + 4); e != begin; e -= TAG_SIZE)
        if (ld8(e - TAG_SIZE + T_OPEN)) return e - TAG_SIZE;
    return 0;
}

// "${": a new tag at the current end of the plain text
void openTag(const sys::ExactFrame &f) {
    const uint32_t pending = f.addr(F_PENDING);
    uint32_t *w = rw::frame_ptr<uint32_t>(pending);
    uint8_t *b = rw::frame_ptr<uint8_t>(pending);
    w[0] = 0;   // kind
    w[1] = ld32(f.addr(F_TEXT) + 4) - ld32(f.addr(F_TEXT));
    b[T_OPEN] = 1;
    b[T_NAME_DONE] = b[T_IN_WORDS] = b[T_IN_FORMAT] = b[T_AFTER_COLON] = 0;
    rw::call(AEABI_MEMSET, {pending + T_NAME, 0x27, 0});   // the vectors and the modifier flags
    w[T_ARGUMENT / 4] = 0xFFFFFFFFu;
    w[(T_TEXT + SB_LENGTH) / 4] = 0;
    b[T_TEXT] = 0;

    if (!ld8(LANG_CHECKED)) {
        st8(LANG_CHECKED, 1);
        st8(IS_ENGLISH, 0);
        const uint32_t lang = f.addr(F_LANG);
        rw::call(VFS_READ_DATA, {lang, S_INFO_LANG});
        if (rw::call(STRCMP, {ld32(ld32(lang) + 4), S_EN}) == 0) st8(IS_ENGLISH, 1);
        if (const uint32_t ctrl = ld32(lang + 4)) guest::release(ctrl);
    }
    pushTag(f.addr(F_TAGS), pending);
    destroyTag(pending);
}

// "}": the tag's name decides its kind
void closeTag(const sys::ExactFrame &f, uint32_t tag) {
    st8(tag + T_OPEN, 0);
    pushChar(tag + T_NAME, 0, CHARS_PUSH_SLOW_MOVE, f.addr(F_TAG_CHAR + 3));
    const uint32_t name = ld32(tag + T_NAME);
    if (rw::call(STRCMP, {name, S_PLAYER}) == 0) {
        st32(tag + T_KIND, PLAYER);
    } else if (rw::call(STRCMP, {name, S_UNION}) == 0) {
        st32(tag + T_KIND, UNION);
    } else if (rw::call(STRCMP, {name, S_SPACE}) == 0) {
        st32(tag + T_KIND, SPACE);
    } else {
        const uint32_t format = tag + T_FORMAT;
        if (ld32(format) == ld32(format + 4)) {
            pushChar(format, '%', CHARS_PUSH_SLOW_MOVE, f.addr(F_TAG_CHAR + 2));
            pushChar(format, 'd', CHARS_PUSH_SLOW_MOVE, f.addr(F_TAG_CHAR + 1));
            st8(tag + T_DEFAULT_FORMAT, 1);
        }
        pushChar(format, 0, CHARS_PUSH_SLOW_MOVE, f.addr(F_TAG_CHAR));
        const int32_t number = int32_t(rw::call(STRTOL, {ld32(tag + T_NAME), f.addr(F_END_PTR), 10}));
        if (number >= 1 && ld8(ld32(f.addr(F_END_PTR))) == 0) {
            st32(tag + T_KIND, ARGUMENT);
            st32(tag + T_ARGUMENT, uint32_t(number));
        }
    }
}

// a character inside an open tag
void addToTag(const sys::ExactFrame &f, uint32_t tag, uint8_t c) {
    const uint32_t temp = f.addr(F_ADD_CHAR);
    *rw::frame_ptr<uint8_t>(temp) = c;
    if (ld8(tag + T_AFTER_COLON)) {   // the modifier character after ':'
        st8(tag + T_AFTER_COLON, 0);
        st8(tag + T_IN_FORMAT, 0);
        const int8_t m = int8_t(c);
        if (m > 'l') {
            if (m == 'm') {
                if (!ld8(tag + T_ORDINAL)) st8(tag + T_MONTH, 1);
            } else if (m == 'o') {
                if (!ld8(tag + T_MONTH)) st8(tag + T_ORDINAL, 1);
            }
        } else if (m == '#') {   // a new word
            st8(tag + T_IN_WORDS, 1);
            const uint32_t words = tag + T_WORDS, empty = f.addr(F_NEW_OPTION);
            uint32_t *e = rw::frame_ptr<uint32_t>(empty);
            e[0] = e[1] = e[2] = 0;
            const uint32_t end = ld32(words + 4);
            if (end < ld32(words + 8)) {
                st32(end, 0);
                st32(end + 4, 0);
                st32(end + 8, 0);
                st32(words + 4, end + 12);
            } else {
                rw::call(WORDS_PUSH_SLOW, {words, empty});
                if (e[0]) guest::deleteObject(e[0]);
            }
        } else if (m == '%') {   // a printf format
            st8(tag + T_IN_FORMAT, 1);
            pushChar(tag + T_FORMAT, c, CHARS_PUSH_SLOW_COPY, temp);
        }
        return;
    }
    if (c == ':') {
        st8(tag + T_AFTER_COLON, 1);
    } else if (ld8(tag + T_IN_FORMAT)) {
        pushChar(tag + T_FORMAT, c, CHARS_PUSH_SLOW_COPY, temp);
    } else if (ld8(tag + T_IN_WORDS)) {
        pushChar(ld32(tag + T_WORDS + 4) - 12, c, CHARS_PUSH_SLOW_COPY, temp);
    } else if (!ld8(tag + T_NAME_DONE)) {
        pushChar(tag + T_NAME, c, CHARS_PUSH_SLOW_COPY, temp);
    }
}

// std::stoll(std::string(piece))
int64_t parseNumber(const sys::ExactFrame &f, uint32_t piece) {
    const uint32_t s = f.addr(F_NUMBER);
    rw::call(STRING_INIT, {s, piece, rw::call(STRLEN, {piece})});
    const int64_t n = int64_t(rw::call64(STOLL, {s, 0, 10}));
    const guest::String *str = rw::host_ptr<guest::String>(s);
    if (str->isLong()) guest::deleteObject(str->data());
    return n;
}

// the English ordinal suffix of n: by n % 100 (11th, 12th, 13th), else by n % 10, else "th"
uint32_t ordinalSuffix(const sys::ExactFrame &f, int64_t n) {
    if (guest::staticInitBegin(SUFFIX_GUARD)) {
        static const uint32_t pairs[12] = {1, 0, 2, 1, 3, 2, 11, 3, 12, 3, 13, 3};
        uint32_t *init = rw::frame_ptr<uint32_t>(f.addr(F_SUFFIXES));
        for (int k = 0; k < 12; k++) init[k] = pairs[k];
        rw::call(INT_MAP_CTOR, {SUFFIX_MAP, f.addr(F_SUFFIXES), 6});
        rw::call(CXA_ATEXIT, {ld32(GOT_INT_MAP_DTOR), SUFFIX_MAP, ld32(GOT_DSO_HANDLE)});
        guest::staticInitDone(SUFFIX_GUARD);
    }
    const uint32_t byHundreds = f.addr(F_KEY + 4), byTens = f.addr(F_KEY);
    *rw::frame_ptr<uint32_t>(byHundreds) = uint32_t(int32_t(n % 100));
    uint32_t node = rw::call(INT_MAP_FIND, {SUFFIX_MAP, byHundreds});
    if (!node) {
        *rw::frame_ptr<uint32_t>(byTens) = uint32_t(int32_t(n % 10));
        node = rw::call(INT_MAP_FIND, {SUFFIX_MAP, byTens});
        if (!node) return S_TH;
    }
    return ld32(SUFFIXES + 4 * ld32(node + 0xC));
}

// a numbered tag's formatted piece -> its text
void finishTag(const sys::ExactFrame &f, uint32_t tag, uint32_t piece) {
    const uint32_t text = tag + T_TEXT;
    const uint32_t words = ld32(tag + T_WORDS), wordsEnd = ld32(tag + T_WORDS + 4);
    if (words != wordsEnd) {   // pick a word: 1 -> the first, 0 -> the second, n -> the n-th, clamped to the last
        const int64_t n = parseNumber(f, piece);
        const int64_t wanted = n ? n - 1 : 1;
        const uint32_t last = (wordsEnd - words) / 12 - 1;
        const uint32_t index = wanted > int64_t(last) ? last : uint32_t(wanted);
        pushChar(words + 12 * index, 0, CHARS_PUSH_SLOW_MOVE, f.addr(F_NUMBER - 1));
        const uint32_t word = ld32(ld32(tag + T_WORDS) + 12 * index);
        if (ld8(tag + T_DEFAULT_FORMAT)) {   // "${N:#..}": the word alone
            rw::call(SB_SET, {text, word});
        } else {   // "${N:%d#..}": the number, then the word
            rw::call(SB_SET, {text, piece});
            rw::call(SB_CONCAT, {text, word});
        }
    } else if (ld8(tag + T_ORDINAL)) {
        if (ld8(IS_ENGLISH) == 1) {
            const int64_t n = parseNumber(f, piece);
            rw::call(SB_SET, {text, piece});
            rw::call(SB_CONCAT, {text, ordinalSuffix(f, n)});
        } else {
            rw::call(SB_SET, {text, piece});
        }
    } else if (ld8(tag + T_MONTH) && ld8(IS_ENGLISH) == 1) {
        const int64_t n = parseNumber(f, piece);
        if (uint64_t(n - 1) >= 12) {   // not a month: throw std::exception()
            const uint32_t e = rw::call(CXA_ALLOCATE_EXCEPTION, {4});
            st32(e, ld32(GOT_EXCEPTION_VTABLE) + 8);
            rw::call(CXA_THROW, {e, ld32(GOT_EXCEPTION_TYPEINFO), ld32(GOT_EXCEPTION_DTOR)});
        }
        rw::call(SB_FSET, {text, ld32(MONTHS + 4 * uint32_t(n - 1))});
    } else {
        rw::call(SB_SET, {text, piece});
    }
}

// map<unsigned, Tag*>: tree nodes are {left, right, parent, black, key +0x10, tag +0x14}
constexpr uint32_t N_LEFT = 0, N_RIGHT = 4, N_PARENT = 8, N_KEY = 0x10, N_TAG = 0x14;

void mapInsert(const sys::ExactFrame &f, uint32_t key, uint32_t tag) {
    const uint32_t map = f.addr(F_MAP), endNode = map + 4;
    const uint32_t node = guest::newObject(0x18);
    st32(node + N_KEY, key);
    st32(node + N_TAG, tag);
    uint32_t parent = endNode, child = endNode;   // child: the slot the new node goes into
    for (uint32_t n = ld32(endNode + N_LEFT); n;) {
        const uint32_t k = ld32(n + N_KEY);
        parent = n;
        if (key < k) {
            child = n + N_LEFT;
        } else if (k < key) {
            child = n + N_RIGHT;
        } else {   // already there: the first tag with this number wins
            child = f.addr(F_PARENT);
            *rw::frame_ptr<uint32_t>(child) = n;
            break;
        }
        if (!ld32(child)) break;
        n = ld32(child);
    }
    if (ld32(child)) {
        guest::deleteObject(node);
        return;
    }
    st32(node + N_LEFT, 0);
    st32(node + N_RIGHT, 0);
    st32(node + N_PARENT, parent);
    st32(child, node);
    if (const uint32_t left = ld32(ld32(map))) st32(map, left);   // a new leftmost node
    rw::call(TREE_BALANCE, {ld32(endNode + N_LEFT), ld32(child)});
    st32(map + 8, ld32(map + 8) + 1);
}

// the in-order successor of a tree node
uint32_t nextNode(uint32_t n) {
    if (uint32_t r = ld32(n + N_RIGHT)) {
        while (ld32(r + N_LEFT)) r = ld32(r + N_LEFT);
        return r;
    }
    uint32_t child;
    do {
        child = n;
        n = ld32(n + N_PARENT);
    } while (ld32(n + N_LEFT) != child);
    return n;
}

// the tag with argument number `key`, or 0
uint32_t mapFind(uint32_t endNode, uint32_t key) {
    uint32_t found = endNode;
    for (uint32_t n = ld32(endNode + N_LEFT); n;) {
        if (ld32(n + N_KEY) < key) {
            n = ld32(n + N_RIGHT);
        } else {
            found = n;
            n = ld32(n + N_LEFT);
        }
    }
    if (found == endNode || key < ld32(found + N_KEY)) return 0;
    return ld32(found + N_TAG);
}
}  // namespace

void execute(uint32_t out, uint32_t text, uint32_t va) {
    sys::ExactFrame f(FRAME_SIZE);
    const uint32_t tags = f.addr(F_TAGS), plain = f.addr(F_TEXT);
    for (uint32_t k = 0; k < 6; k++) rw::frame_ptr<uint32_t>(tags)[k] = 0;   // tags and plain text

    // pass 1: split the text into plain characters and tags
    bool escaped = false;
    for (uint32_t p = text; uint8_t c = ld8(p); p++) {
        if (escaped) {
            escaped = false;
            pushChar(plain, c, CHARS_PUSH_SLOW_COPY, f.addr(F_CHAR + 1));
        } else if (c == '}') {
            if (const uint32_t tag = lastOpenTag(tags))
                closeTag(f, tag);
            else
                pushChar(plain, c, CHARS_PUSH_SLOW_COPY, f.addr(F_CHAR + 1));
        } else if (c == '\\') {
            escaped = true;
        } else if (c == '$') {
            const uint8_t next = ld8(p + 1);
            if (next == '{') {
                openTag(f);
                p++;
            } else if (next) {
                pushChar(plain, c, CHARS_PUSH_SLOW_COPY, f.addr(F_CHAR + 1));
            }
        } else if (const uint32_t tag = lastOpenTag(tags)) {
            addToTag(f, tag, c);
        } else {
            pushChar(plain, c, CHARS_PUSH_SLOW_COPY, f.addr(F_CHAR + 1));
        }
    }
    pushChar(plain, 0, CHARS_PUSH_SLOW_MOVE, f.addr(F_CHAR));

    // the numbered tags by number
    const uint32_t map = f.addr(F_MAP), endNode = map + 4;
    uint32_t *m = rw::frame_ptr<uint32_t>(map);
    m[1] = 0;
    m[2] = 0;
    m[0] = endNode;
    for (uint32_t tag = ld32(tags), end = ld32(tags + 4); tag != end; tag += TAG_SIZE)
        if (ld32(tag + T_KIND) == ARGUMENT) mapInsert(f, ld32(tag + T_ARGUMENT), tag);

    // pass 2: one printf over all the formats, separated by \x03
    const uint32_t format = f.addr(F_FORMAT), values = f.addr(F_VALUES);
    rw::frame_ptr<uint32_t>(format + SB_LENGTH)[0] = 0;
    rw::frame_ptr<uint8_t>(format)[0] = 0;
    for (uint32_t n = m[0]; n != endNode; n = nextNode(n))
        rw::call(SB_FCONCAT, {format, S_JOIN, ld32(ld32(n + N_TAG) + T_FORMAT), SEPARATOR});
    rw::frame_ptr<uint32_t>(values + SB_LENGTH)[0] = 0;
    rw::frame_ptr<uint8_t>(values)[0] = 0;
    rw::call(SB_VFSET, {values, format, va});

    // each piece belongs to the tag numbered by its position (1, 2, ...)
    uint32_t number = 0, piece = values;
    for (uint32_t sep = rw::call(STRCHR, {values, SEPARATOR}); sep; sep = rw::call(STRCHR, {piece, SEPARATOR})) {
        number++;
        if (const uint32_t tag = mapFind(endNode, number)) {
            rw::call(SB_REPLACE, {values, sep - values, 0});
            finishTag(f, tag, piece);
        }
        piece = sep + 1;
    }

    // pass 3: the plain text, with every tag's text inserted at its position (last tag first)
    const uint32_t plainText = ld32(plain);
    rw::call(STRING_INIT, {out, plainText, rw::call(STRLEN, {plainText})});
    for (uint32_t begin = ld32(tags), e = ld32(tags + 4); e != begin;) {
        e -= TAG_SIZE;
        const uint32_t position = ld32(e + T_POSITION);
        switch (ld32(e + T_KIND)) {
        case ARGUMENT: {
            const uint32_t s = e + T_TEXT;
            rw::call(STRING_INSERT, {out, position, s, rw::call(STRLEN, {s})});
            break;
        }
        case PLAYER: {
            const uint32_t profile = ld32(rw::call(APIMANAGER_GET_INSTANCE, {}) + API_USER_PROFILE);
            if (!profile) break;
            const uint32_t name = f.addr(F_PLAYER_NAME);
            rw::call_exact(SAFEBUFFER_READ,
                           {profile, name, ld32(profile + 0xC) + PROFILE_NAME, PROFILE_NAME_SIZE, PROFILE_NAME});
            rw::call(STRING_INSERT, {out, position, name, rw::call(STRLEN, {name})});
            break;
        }
        case UNION: {
            const uint32_t user = ld32(rw::call(APIMANAGER_GET_INSTANCE, {}) + API_USER_UNION);
            if (!user) break;
            const uint32_t id = f.addr(F_UNION_ID), unionText = f.addr(F_UNION_TEXT);
            rw::call_exact(SAFEBUFFER_READ, {user, id, ld32(user + 0xC) + USER_UNION_ID, 4, USER_UNION_ID});
            rw::call(GET_UNION_NAME, {unionText, ld32(id)});
            const uint32_t s = ld32(ld32(unionText) + 4);
            rw::call(STRING_INSERT, {out, position, s, rw::call(STRLEN, {s})});
            if (const uint32_t ctrl = ld32(unionText + 4)) guest::release(ctrl);
            break;
        }
        case SPACE:
            rw::call(STRING_INSERT, {out, position, S_SPACE_TEXT, 1});
            break;
        default:
            break;
        }
    }

    rw::call(MAP_DESTROY, {map, ld32(endNode + N_LEFT)});
    rw::call(TAGS_DTOR, {tags});
    freeVector(plain);
}
RW_FUNCTION(0x492CAC, "TextFormat::execute(const char*, va_list)", execute);

}  // namespace textformat
