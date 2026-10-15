// SceneLight rewrites: guest addresses, lambda vtables and small helpers shared by its files.
#pragma once

#include "SceneLight.hpp"

namespace slight {

using namespace w4b1;
using Self = SceneLight;

inline Self *S(uint32_t a) { return rw::host_ptr<Self>(a); }
inline void addRef(uint32_t ctrl) { if (ctrl) guest::addRef(ctrl); }
inline void release(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }

// field offsets used as addresses (byte arrays, embedded containers)
constexpr uint32_t OFF_FLIPPED = 0x2BC, OFF_BOUND = 0x1652, OFF_SPEECH = 0x2A0, OFF_SPEECH_FIX = 0x2A4;
constexpr uint32_t OFF_PAGES = 0x29F8, OFF_EXPRESSIONS = 0x2A0C, OFF_TTF_SPEECH = 0x2A20, OFF_TTF_NAME = 0x2A44;
constexpr uint32_t OFF_LABELS = 0x2A68;

// lambda vtables (__func + 8)
constexpr uint32_t VT_init_0 = 0xF67020, VT_init_1 = 0xF66FF4, VT_init_2 = 0xF66FC8, VT_init_3 = 0xF66F9C;
constexpr uint32_t VT_init_4 = 0xF66F70, VT_init_5 = 0xF66F44, VT_nextScene_6 = 0xF66F18, VT_bind_7 = 0xF66EEC;
constexpr uint32_t VT_text_8 = 0xF66EC0, VT_text_9 = 0xF66E94, VT_speech_10 = 0xF66E68;
constexpr uint32_t VT_move_11 = 0xF66E3C, VT_move_12 = 0xF66E10, VT_move_13 = 0xF66DE4, VT_move_14 = 0xF66DB8;
constexpr uint32_t VT_move_15 = 0xF66D8C, VT_move_16 = 0xF66D60, VT_move_17 = 0xF66D34, VT_move_18 = 0xF66D08;
constexpr uint32_t VT_move_19 = 0xF66CDC, VT_move_20 = 0xF66CB0, VT_move_21 = 0xF66C84;

// strings
constexpr uint32_t S_SEQ = 0xE057AD;            // img/light/SEQ/%d.l
constexpr uint32_t S_IMG = 0xE057C0;            // img/light/%s.png
constexpr uint32_t S_IMG_BG = 0xE057D1;         // img/light/BG/%d.png
constexpr uint32_t S_IMG_NPC = 0xE057E5;        // img/light/NPC/%d.png
constexpr uint32_t S_IMG_NARRATION = 0xE057FA;  // img/light/NARRATION/%d.png
constexpr uint32_t S_FONT_SPEECH = 0xDF9F1F;    // cocostudio/publish/Cinema_Calligraphy_Web.ttf (45)
constexpr uint32_t S_FONT_NAME = 0xDF9F4D;      // cocostudio/publish/DFPSoGei-W5.TTF (34)
constexpr uint32_t S_SKIP_JSON = 0xE05815;      // cocostudio/publish/FlashSkipAnimation.ExportJson (48)
constexpr uint32_t S_SKIP = 0xE05846;           // FlashSkipAnimation (18)
constexpr uint32_t S_NO_NAME = 0xE05859;        // "注意！！　その名前該当なし" (no such name)
constexpr uint32_t S_NO_LINE = 0xE05881;        // "注意！！　そのセリフ該当なし" (no such line, 42 bytes)
constexpr uint32_t S_EMPTY = 0xDF8A1A;

// tables
constexpr uint32_t T_MOVE_X = 0xE7BB60;         // int16[5]: x offsets of the 5 positions (cmd_move)
constexpr uint32_t T_HIGE_X = 0xE7BB68;         // int16[]: x offset of the window tail per window type
constexpr uint32_t T_EXPRESSION = 0xE7C7D0;     // int32[5]: Avatar::Expression of avatar textures 40011..40015
constexpr int32_t SEQ_DEFAULT = 1010201;        // the sequence loaded when the asked one is missing
constexpr int32_t AVATAR_TEXTURE = 40000;       // textures from here on are the player's avatar (expression)
constexpr int32_t TEXT_ID_BASE = 200000000;

// guest functions
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t Director_getTextureCache = 0x8EFE69;
constexpr uint32_t Director_getVisibleSize = 0x8F0675;      // (Size* out, Director*)
constexpr uint32_t Director_getVisibleOrigin = 0x8F06A1;    // (Vec2* out, Director*)
constexpr uint32_t TextureCache_addImage = 0x917DA9;        // (cache, const string&)
constexpr uint32_t Texture2D_getContentSize = 0x914ACB;     // (Size* out, Texture2D*)
constexpr uint32_t StringBuffer_fset = 0x491501;
constexpr uint32_t bg_vfs_readData = 0x8DD46D;              // (shared_ptr<bg::Data>* out, const char*)
constexpr uint32_t Scene_create = 0x8A5F37;
constexpr uint32_t Sprite_create = 0x8A64DD;
constexpr uint32_t Sprite_setFlippedX = 0x8A81C5;
constexpr uint32_t Sprite_setFlippedY = 0x8A8201;
constexpr uint32_t SpriteVector_pushBack = 0x5E4551;
constexpr uint32_t SpriteVector_dtor = 0x5E4761;
constexpr uint32_t Color3B_ctor = 0x9062E7;
constexpr uint32_t Color3B_BLACK = 0x1007E9A, Color4B_BLACK = 0x1007EBC, Color4B_WHITE = 0x1007EA4;
constexpr uint32_t ANCHOR_TOP_LEFT = 0xFEF5C8;
constexpr uint32_t Size_copy = 0x8DF02D, Size_assign = 0x8DF041, Vec2_copy = 0x8E4C03;
constexpr uint32_t Label_create = 0x883789;
constexpr uint32_t Label_setLineHeight = 0x88693D;
constexpr uint32_t Label_setDimensions = 0x883A8D;
constexpr uint32_t Label_setAlignment = 0x884CB9;
constexpr uint32_t SpeechDisp_create = 0x601C21;
constexpr uint32_t SpeechDisp_runSpeechSet = 0x492A4D;      // (disp, CallFunc*)
constexpr uint32_t SpeechDisp_toEndSpeech = 0x4929F5;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t AvatarIcon_create = 0x5C9DB5;            // (style, texture, pos, float, float, expressions)
constexpr uint32_t AvatarIcon_changeExpression = 0x5CA459;  // (icon, expression, float)
constexpr uint32_t AvatarUtil_getCurrentStyle = 0x4977AD;   // (Style* out, bool)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t ExpressionVector_pushSlowMove = 0x6C5C95;   // vector<Expression>::__push_back_slow_path(&&)
constexpr uint32_t ExpressionVector_pushSlowCopy = 0x6C5B59;   // (const&)
constexpr uint32_t ExpressionVector_baseDtor = 0x46BE0D;
constexpr uint32_t LabelMap_destroy = 0x6C6335;             // std::__tree<int, Label*>::destroy(node)
constexpr uint32_t Pages_index = 0x601971;                  // unordered_map<SpeechDisp*, deque<string>>::operator[]
constexpr uint32_t Pages_erase = 0x604EE1;                  // __hash_table::__erase_unique(const key&)
constexpr uint32_t Pages_clear = 0x605285;
constexpr uint32_t Pages_insert = 0x603C09;                 // __insert_unique(pair&&) (pair<iterator,bool>* out)
constexpr uint32_t Pages_dtor = 0x608097;
constexpr uint32_t Deque_popFront = 0x601DC1;
constexpr uint32_t Deque_pushBack = 0x601D05;               // (deque, string&&)
constexpr uint32_t Deque_baseDtor = 0x601D95;
constexpr uint32_t String_findChar = 0x601CD1;              // string::find(char, pos)
constexpr uint32_t String_substr = 0x47CD4D;                // string(const string&, pos, n, alloc)
constexpr uint32_t TextData_get = 0x4928AD;                 // (shared_ptr out, type, id)
constexpr uint32_t TextData_format = 0x492929;              // (string out, type, id, ...)
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_setEnabledTouchEffect = 0x60CF65;
constexpr uint32_t SceneManager_hideInfoHeader = 0x612F4D;
constexpr uint32_t SceneManager_removeBackground = 0x611691;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;       // (shared_ptr<Layer>* out, mgr, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;           // (layer, ResourceID, int, int)
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t ScaleTo_create = 0x86A8CD;               // (float, float)
constexpr uint32_t FadeTo_create = 0x86B3E1;                // (float, uint8)
constexpr uint32_t JumpBy_create = 0x869B79;                // (float, const Vec2&, float, int)
constexpr uint32_t JumpTo_create = 0x869F39;
constexpr uint32_t RepeatForever_create = 0x86815D;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t EaseBackOut_create = 0x8616CD;
constexpr uint32_t EaseBackIn_create = 0x8615C9;
constexpr uint32_t EaseExponentialIn_create = 0x8608F9;
constexpr uint32_t EaseOut_create = 0x8606A5;               // (action, float rate)
constexpr uint32_t Spawn_createWithTwoActions = 0x86838D;

// virtual slots
constexpr uint32_t VS_scaleX = 0x28, VS_setScale = 0x40, VS_setScaleXY = 0x48, VS_setPositionV = 0x4C;
constexpr uint32_t VS_getPositionV = 0x54, VS_setPositionXY = 0x5C, VS_getPositionXY = 0x60;
constexpr uint32_t VS_setPositionX = 0x64, VS_setPositionY = 0x6C, VS_setAnchorPoint = 0x9C;
constexpr uint32_t VS_getContentSize = 0xAC, VS_setVisible = 0xB0, VS_addChild = 0xF0, VS_addChildZ = 0xF4;
constexpr uint32_t VS_addChildZTag = 0xF8, VS_removeFromParentAndCleanup = 0x128, VS_reorderChild = 0x140;
constexpr uint32_t VS_getBoundingBox = 0x1A0, VS_setOpacity = 0x214, VS_setColor = 0x22C, VS_init = 0x248;
constexpr uint32_t VS_setTexture = 0x27C, VS_setTTFConfig = 0x280, VS_setTextureRect = 0x284;
constexpr uint32_t VS_getAnimation = 0x298, VS_setString = 0x2A8, VS_setTextColor = 0x2B0;
constexpr uint32_t VS_Animation_playWithIndex = 0x70;

// sprite of a slot (no range check, as the original)
inline uint32_t sprite(uint32_t self, int32_t slot) { return ld32(ld32(self + 0x288) + uint32_t(slot) * 4); }
inline uint32_t flipAddr(uint32_t self, int32_t slot) { return self + OFF_FLIPPED + uint32_t(slot); }
inline uint32_t boundAddr(uint32_t self, int32_t slot) { return self + OFF_BOUND + uint32_t(slot); }

inline float msToSec(int32_t ms) { return float(ms) / 1000.0f; }

inline void setVisible(uint32_t node, uint32_t v) { vcall(node, VS_setVisible, {node, v}); }
inline void setOpacity(uint32_t node, uint32_t v) { vcall(node, VS_setOpacity, {node, v}); }

// Size / Vec2 results on the guest stack
struct Size2 { float w, h; };
inline Size2 visibleSize(Frame &f) {
    uint32_t out = f.alloc(8);
    call(Director_getVisibleSize, {out, call(Director_getInstance, {})});
    return Size2{ldf(out), ldf(out + 4)};
}
// Size::Size(const Size&) of a node's content size, copied into the frame
inline Size2 contentSize(Frame &f, uint32_t node) {
    uint32_t out = f.alloc(8);
    call(Size_copy, {out, vcall(node, VS_getContentSize, {node})});
    return Size2{ldf(out), ldf(out + 4)};
}

// a temporary Vec2(x, y) for a call taking const Vec2&
template <class Fn> inline uint32_t withVec2(float x, float y, Fn fn) {
    Frame f;
    uint32_t v = f.alloc(8);
    call(wl::Vec2_ctorXY, {v, fb(x), fb(y)});
    uint32_t r = fn(v);
    call(wl::Vec2_dtor, {v});
    return r;
}

// `this->wait >= 0` (or NaN): the command does not wait for its action to end
inline bool waitNotNegative(uint32_t self) {
    float t = S(self)->wait;
    return t >= 0.0f || t != t;
}

// node->runAction(Sequence(action, CallFunc(lambda{this}), nullptr)), the inline std::function destroyed after
inline void runWithDone(uint32_t node, uint32_t action, uint32_t vtable, uint32_t self) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, {self});
    uint32_t cf = call(wl::CallFunc_create, {fo});
    call(wl::Node_runAction, {node, call(wl::Sequence_create, {action, cf, 0})});
    functionDestroy(fo);
}
// node->runAction(Sequence(action, nullptr))
inline void runAlone(uint32_t node, uint32_t action) {
    call(wl::Node_runAction, {node, call(wl::Sequence_create, {action, 0})});
}

// stopAllActions on every slot sprite (the vector is re-read each time)
inline void stopAllSlots(uint32_t self) {
    for (int32_t i = 0; i != -SceneLight::SLOT_COUNT; i--)
        call(smain::lib::Node_stopAllActions, {ld32(ld32(self + 0x288) - uint32_t(i) * 4)});
}

// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
inline void playSE(uint32_t id) {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    call(audio_Layer_playSE, {ld32(layer), id, 0, 0});
    release(ld32(layer + 4));
}
}  // namespace slight
