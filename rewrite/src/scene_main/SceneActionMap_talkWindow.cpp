// SceneActionMap: the story talk window (a 9-slice window with a typing SpeechDisp label, a hopping "next" key
// and the speech tail) and its tap handling.
#include "SceneActionMap.hpp"

using namespace smain;
using rw::guest_addr;

namespace {
// talk window parts (SceneActionMap)
constexpr uint32_t T_WINDOW = 0x36C;    // cocos2d::extension::Scale9Sprite*
constexpr uint32_t T_TAIL = 0x370;      // Sprite: the speech tail
constexpr uint32_t T_KEY = 0x374;       // Sprite: the "next" key
constexpr uint32_t T_LABEL = 0x378;     // SpeechDisp* (retained)
constexpr uint32_t T_SHOWN = 0x390;     // byte: the open animation finished

constexpr uint32_t VT_WINDOW_OPENED = 0xF5E410;   // makeTalkWindow $_?: the open animation finished
constexpr uint32_t S_TALK_KEY = 0xE02E0B, S_TALK_TAIL = 0xE02E25, S_TALK_WINDOW = 0xE02E43;
constexpr uint32_t S_TALK_FONT = 0xDF9F1F;   // "cocostudio/publish/Cinema_Calligraphy_Web.ttf"
constexpr uint32_t GOT_COLOR4B_BLACK = 0xFD65C8;
constexpr int32_t TEXT_TALK_BASE = 200000000;
constexpr uint32_t TALK_FONT_SIZE = 24;
constexpr float KEY_X = 336.0f, WINDOW_HEIGHT = 108.0f;
constexpr uint32_t POP_IN_SECONDS = 0x3E888889;   // 4/15 s (float bits)

constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__v, Sprite_setFlippedX = rwlib::cocos2d::Sprite::setFlippedX, Sprite_setFlippedY = rwlib::cocos2d::Sprite::setFlippedY;
constexpr uint32_t Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache, TextureCache_addImage = rwlib::cocos2d::TextureCache::addImage;
constexpr uint32_t Texture2D_getContentSize = rwlib::cocos2d::Texture2D::getContentSize;   // (Size* out, texture)
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff, Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t SpeechDisp_create = 0x601C21, SceneActionMap_setTalkText = 0x601A15;
constexpr uint32_t Label_setLineHeight = rwlib::cocos2d::Label::setLineHeight, Label_setDimensions = rwlib::cocos2d::Label::setDimensions;
constexpr uint32_t Scale9Sprite_create = rwlib::cocos2d::extension::Scale9Sprite::create__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEERKNS_4RectESD_;   // (file, Rect rect, Rect capInsets)
constexpr uint32_t JumpBy_create = rwlib::cocos2d::JumpBy::create, RepeatForever_create = rwlib::cocos2d::RepeatForever::create;
constexpr uint32_t ScaleTo_create = rwlib::cocos2d::ScaleTo::create__ff, EaseBackOut_create = rwlib::cocos2d::EaseBackOut::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create, Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Sprite_setTexture = 0x27C, Sprite_setTextureRect = 0x284;   // virtual slots (byte offsets)
constexpr uint32_t Label_setTextColor = 0x2B0, Label_setTTFConfig = 0x280;

uint32_t textureOf(uint32_t file, uint32_t length) {
    const uint32_t cache = call(Director_getTextureCache, {director()});
    Frame f;
    const uint32_t s = newStr(f, file, length);
    const uint32_t texture = call(TextureCache_addImage, {cache, s});
    strFree(s);
    return texture;
}

void setTextureRect(uint32_t sprite, uint32_t size) {
    Frame f;
    const uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, 0, 0, ld32(size), ld32(size + 4)});
    vcall(sprite, Sprite_setTextureRect, {sprite, rect});
    call(Vec2_dtor, {rect});
}
}  // namespace

// The talk window for talk `id` (text 200000000 + id % 200000000 of type 1): pops up from scale 0, its label
// typing the first line (setTalkText); 8 when the talk has no text.
uint32_t makeTalkWindow(SceneActionMap *selfPtr, int32_t id) {
    const uint32_t self = guest_addr(selfPtr);
    Frame f;
    const uint32_t text = f.alloc(8);
    call(TextData_get, {text, 1, uint32_t(id % TEXT_TALK_BASE + TEXT_TALK_BASE)});
    if (!ld32(text)) {
        sharedRelease(text);
        return 8;
    }
    const uint32_t visible = f.alloc(8);
    call(fn::Director_getVisibleSize, {visible, director()});
    st32(self + T_KEY, call(Sprite_create, {}));
    st32(self + T_TAIL, call(Sprite_create, {}));
    const uint32_t keyTexture = textureOf(S_TALK_KEY, 0x19);
    const uint32_t tailTexture = textureOf(S_TALK_TAIL, 0x1D);
    const uint32_t keySize = f.alloc(8), tailSize = f.alloc(8);
    call(Texture2D_getContentSize, {keySize, keyTexture});
    call(Texture2D_getContentSize, {tailSize, tailTexture});
    vcall(ld32(self + T_KEY), Sprite_setTexture, {ld32(self + T_KEY), keyTexture});
    vcall(ld32(self + T_TAIL), Sprite_setTexture, {ld32(self + T_TAIL), tailTexture});
    setTextureRect(ld32(self + T_KEY), keySize);
    setTextureRect(ld32(self + T_TAIL), tailSize);
    vcall(ld32(self + T_KEY), node::setVisible, {ld32(self + T_KEY), 0});
    vcall(ld32(self + T_TAIL), node::setVisible, {ld32(self + T_TAIL), 0});

    // the label: a SpeechDisp in the calligraphy font (TTFConfig: path, size 24, the rest default)
    const uint32_t ttf = f.alloc(0x24);
    strInit(ttf, S_TALK_FONT, 0x2D);
    st32(ttf + 0xC, TALK_FONT_SIZE);
    st32(ttf + 0x10, 0);
    st32(ttf + 0x14, 0);
    st8(ttf + 0x18, 0);
    rw::save<uint16_t>(ttf + 0x20, 0);
    st32(ttf + 0x1C, 0);
    if (const uint32_t old = ld32(self + T_LABEL)) {
        call(Ref_release, {old});
        st32(self + T_LABEL, 0);
    }
    st32(self + T_LABEL, call(SpeechDisp_create, {}));
    call(Ref_retain, {ld32(self + T_LABEL)});
    vcall(ld32(self + T_LABEL), Label_setTextColor, {ld32(self + T_LABEL), ld32(GOT_COLOR4B_BLACK)});
    vcall(ld32(self + T_LABEL), Label_setTTFConfig, {ld32(self + T_LABEL), ttf});
    call(SceneActionMap_setTalkText, {self, uint32_t(id)});
    vcall(ld32(self + T_LABEL), node::setPositionXY, {ld32(self + T_LABEL), fbits(200.0f), fbits(50.0f)});
    call(Label_setLineHeight, {ld32(self + T_LABEL), fbits(32.0f)});
    call(Label_setDimensions, {ld32(self + T_LABEL), 0, 0});

    // the window: TalkWindow1.png as a 9-slice (66 x 108, stretching the 2 x 55 middle at 32, 53)
    {
        const uint32_t file = newStr(f, S_TALK_WINDOW, 0x1D);
        const uint32_t rect = f.alloc(16), insets = f.alloc(16);
        call(Rect_ctor, {rect, 0, 0, fbits(66.0f), fbits(WINDOW_HEIGHT)});
        call(Rect_ctor, {insets, fbits(32.0f), fbits(53.0f), fbits(2.0f), fbits(55.0f)});
        st32(self + T_WINDOW, call(Scale9Sprite_create, {file, rect, insets}));
        call(Vec2_dtor, {insets});
        call(Vec2_dtor, {rect});
        strFree(file);
    }

    // the key hops at the window's right end; the tail hangs under the speaker
    const uint32_t key = ld32(self + T_KEY);
    vcall(key, node::setPositionX, {key, fbits(ldf(keySize) * 0.5f + KEY_X)});
    vcall(ld32(self + T_KEY), node::setPositionY, {ld32(self + T_KEY), 0});
    {
        const uint32_t by = f.alloc(8);
        call(Vec2_ctor, {by, 0, 0});
        const uint32_t jump = call(JumpBy_create, {fbits(0.5f), by, fbits(8.0f), 1});
        call(Vec2_dtor, {by});
        const uint32_t target = ld32(self + T_KEY);
        call(Node_runAction, {target, call(RepeatForever_create, {jump})});
    }
    vcall(ld32(self + T_KEY), node::setVisible, {ld32(self + T_KEY), 0});
    call(Sprite_setFlippedY, {ld32(self + T_TAIL), 0});
    call(Sprite_setFlippedX, {ld32(self + T_TAIL), 0});
    vcall(ld32(self + T_TAIL), node::setPositionX, {ld32(self + T_TAIL), fbits(84.0f)});
    vcall(ld32(self + T_TAIL), node::setPositionY, {ld32(self + T_TAIL), fbits(ldf(tailSize + 4) * -0.5f + 11.0f)});

    const uint32_t window = self + T_WINDOW;
    vcall(ld32(window), node::setScale, {ld32(window), 0});
    vcall(ld32(window), node::setOpacity, {ld32(window), 255});
    {
        const uint32_t size = f.alloc(8);
        const uint32_t w = ld32(window);
        const uint32_t setContentSize = ld32(ld32(w) + node::setContentSize);
        call(Size_ctor, {size, fbits(ldf(keySize) + KEY_X), fbits(WINDOW_HEIGHT)});
        call(setContentSize, {w, size});
    }
    vcall(ld32(window), node::setVisible, {ld32(window), 1});
    const uint32_t popIn = call(EaseBackOut_create, {call(ScaleTo_create, {POP_IN_SECONDS, fbits(1.0f)})});
    vcall(self, node::addChildZTag, {self, ld32(window), 1, 1});
    vcall(ld32(window), node::addChildZTag, {ld32(window), ld32(self + T_KEY), 1, 1});
    vcall(ld32(window), node::addChildZTag, {ld32(window), ld32(self + T_TAIL), 1, 1});
    vcall(ld32(window), node::setPositionX, {ld32(window), fbits(ldf(visible) * 0.5f)});
    vcall(ld32(window), node::setPositionY, {ld32(window), fbits(ldf(visible + 4) * 0.125f * 6.0f)});
    st8(self + T_SHOWN, 0);
    {
        const uint32_t target = ld32(window);
        const uint32_t opened = newFunction(f, VT_WINDOW_OPENED, {self});
        call(Node_runAction, {target, call(Sequence_create, {popIn, call(CallFunc_create, {opened}), 0})});
        functionDestroy(opened);
    }
    strFree(ttf);
    sharedRelease(text);
    return 1;
}
RW_FUNCTION(0x60136C, "SceneActionMap::makeTalkWindow", makeTalkWindow);

namespace {
constexpr uint32_t VT_LINE_DONE_3 = 0xF5E494;   // onTouchEndedTalk $_?: a queued line has typed out (state 3 -> 2)
constexpr uint32_t VT_NEXT_TALK_DONE = 0xF5E4C0;   // ... the next talk's first line (state 3, queue empty)
constexpr uint32_t VT_LINE_DONE_5 = 0xF5E468;   // ... a queued line in the second stage (state 5 -> 4)
constexpr uint32_t STAGE_TALK_STATE = 0x1C;     // StageManager: the talk mode's step
constexpr uint32_t FIELD_TALKING_NPC = 0xD4;    // StageFieldManager: the MapNonPlayableCharacter being talked to
constexpr uint32_t NPC_HEALS = 0x2B4, NPC_NEXT_TALK = 0x2BC, NPC_HEALED = 0x2C0;
constexpr uint32_t AVATAR_HEAL_PENDING = 0x295;
constexpr uint32_t T_HEALED_ONCE = 0x390;
constexpr uint32_t SpeechDisp_toEndSpeech = 0x4929F5, SpeechDisp_runSpeechSet = 0x492A4D;
constexpr uint32_t SceneActionMap_subTalkText = 0x601BF5;
constexpr uint32_t SpeechMap_at = 0x601971;           // unordered_map<SpeechDisp*, deque<string>>::operator[]
constexpr uint32_t SpeechMap_erase = 0x604EE1;        // __erase_unique(const key&)
constexpr uint32_t CommonDirection_heal = 0x766DF9;
constexpr uint32_t StageUI_getPlayerStatusField = 0x75E671, PlayerStatusIndicator_setHP = 0x5E3119;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9, SafeBuffer_read = 0x45A269;
constexpr uint32_t API_USER_STATUS = 0x8C, STATUS_HP = 0x38;
constexpr uint32_t Label_setString = 0x2A8;
constexpr uint32_t LINES_PER_BLOCK = 341;             // deque<std::string>: 4092-byte blocks

void setKeyVisible(uint32_t self, uint32_t visible) { vcall(ld32(self + T_KEY), node::setVisible, {ld32(self + T_KEY), visible}); }

// the label types its text out, then the lambda (capturing the scene) runs
void typeOut(uint32_t self, uint32_t lambdaVtable) {
    const uint32_t label = ld32(self + T_LABEL);
    Frame f;
    const uint32_t done = newFunction(f, lambdaVtable, {self});
    call(SpeechDisp_runSpeechSet, {label, call(CallFunc_create, {done})});
    functionDestroy(done);
}

// the label's queued lines: the next one shown (true), or the queue removed when it is empty (false)
bool showQueuedLine(uint32_t self, uint32_t keyVisibleAgain) {
    const uint32_t queues = self + 0x37C, label = self + T_LABEL;
    const uint32_t queue = call(SpeechMap_at, {queues, label});
    if (!ld32(queue + 0x14)) {
        call(SpeechMap_erase, {queues, label});
        return false;
    }
    const uint32_t start = ld32(queue + 0x10);
    const uint32_t line = ld32(ld32(queue + 4) + 4 * (start / LINES_PER_BLOCK)) + 12 * (start % LINES_PER_BLOCK);
    vcall(ld32(label), Label_setString, {ld32(label), line});
    if (keyVisibleAgain) setKeyVisible(self, 0);
    return true;
}
}  // namespace

// A tap in talk mode: step 2 / 4 (a line is typing) shows it whole; step 3 / 5 (a line is out) types the next
// queued line, or in step 3 opens the NPC's next talk (6 when there is none); step 4 heals the player once when
// the NPC heals.
void onTouchEndedTalk(SceneActionMap *selfPtr, uint32_t, uint32_t) {
    const uint32_t self = guest_addr(selfPtr);
    const uint32_t stage = self + 0x320;
    switch (ld32(ld32(stage) + STAGE_TALK_STATE)) {
    case 2:
        setKeyVisible(self, 1);
        call(SpeechDisp_toEndSpeech, {ld32(self + T_LABEL)});
        call(SceneActionMap_subTalkText, {self, 3});
        break;
    case 3:
        if (showQueuedLine(self, 0)) {
            setKeyVisible(self, 0);
            typeOut(self, VT_LINE_DONE_3);
            st32(ld32(stage) + STAGE_TALK_STATE, 2);
        } else {
            st32(ld32(stage) + STAGE_TALK_STATE, 4);
            setKeyVisible(self, 0);
            const uint32_t next = ld32(ld32(ld32(self + 0x2C0) + FIELD_TALKING_NPC) + NPC_NEXT_TALK);
            Frame f;
            const uint32_t text = f.alloc(8);
            call(TextData_get, {text, 1, next % uint32_t(TEXT_TALK_BASE) + uint32_t(TEXT_TALK_BASE)});
            if (ld32(text)) {
                // a TTFConfig (size 22) is built here and never used
                const uint32_t ttf = f.alloc(0x24);
                strInit(ttf, S_TALK_FONT, 0x2D);
                st32(ttf + 0xC, 22);
                st32(ttf + 0x10, 0);
                st32(ttf + 0x14, 0);
                st8(ttf + 0x18, 0);
                rw::save<uint16_t>(ttf + 0x20, 0);
                st32(ttf + 0x1C, 0);
                call(SceneActionMap_setTalkText, {self, next});
                typeOut(self, VT_NEXT_TALK_DONE);
                strFree(ttf);
            } else {
                st32(ld32(stage) + STAGE_TALK_STATE, 6);
            }
            sharedRelease(text);
        }
        break;
    case 4: {
        setKeyVisible(self, 1);
        call(SpeechDisp_toEndSpeech, {ld32(self + T_LABEL)});
        const uint32_t npc = ld32(ld32(self + 0x2C0) + FIELD_TALKING_NPC);
        if (ld32(npc + NPC_HEALS) == 1 && !ld8(self + T_HEALED_ONCE) && !ld32(npc + NPC_HEALED)) {
            st32(npc + NPC_HEALED, 1);
            call(CommonDirection_heal, {ld32(self + 0x330)});
            st8(ld32(ld32(self + 0x2BC) + 0x20) + AVATAR_HEAL_PENDING, 0);
            const uint32_t indicator = call(StageUI_getPlayerStatusField, {ld32(self + 0x2B8)});
            const uint32_t status = ld32(call(APIManager_getInstance, {}) + API_USER_STATUS);
            Frame f;
            const uint32_t hp = f.alloc(4);
            call(SafeBuffer_read, {status, hp, ld32(status + 0xC) + STATUS_HP, 4, STATUS_HP});
            call(PlayerStatusIndicator_setHP, {indicator, ld32(hp)});
            st8(self + T_HEALED_ONCE, 1);
        }
        call(SceneActionMap_subTalkText, {self, 5});
        break;
    }
    case 5:
        setKeyVisible(self, 0);
        if (showQueuedLine(self, 1)) {
            typeOut(self, VT_LINE_DONE_5);
            st32(ld32(stage) + STAGE_TALK_STATE, 4);
        } else {
            st32(ld32(stage) + STAGE_TALK_STATE, 6);
        }
        break;
    default:
        break;
    }
}
RW_FUNCTION(0x5FF704, "SceneActionMap::onTouchEndedTalk", onTouchEndedTalk);
