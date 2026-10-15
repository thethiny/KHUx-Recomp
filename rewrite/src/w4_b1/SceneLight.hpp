// SceneLight: the "light novel" scene player (story cut-scenes drawn with sprites and speech windows). A scene is
// a command list (img/light/SEQ/<id>.l, 20-byte commands {op, a, waitMs, c, d}) executed one per wait: bind a
// texture to a sprite slot, move / fade it, show a speech (name + text split into pages on '\f'), wait for a tap
// (the "next" cursor), play sounds; 4001 ends the scene (next_scene changes to the following scene layer).
// 5014 sprite slots are children of the layer (z = slot + 2); the named slots used by the commands:
//   5001 BGM (bind = replace the BGM), 5002 SE (bind = play a sound), 5003 background, 5004 / 5008.. pictures,
//   5005 speech window, 5006 speech window tail ("hige"), 5007 the "next" cursor, 5008..5013 characters
//   (slot + 3 = the same character's fade-in copy), 5014 a full-screen black sprite (fades).
#pragma once

#include "W4B1Common.hpp"

struct SceneLight {
    // Command opcodes (Command::op)
    enum Op : int32_t {
        OP_INIT = 4000, OP_FINISH = 4001, OP_BIND = 4002, OP_PRT = 4003, OP_TEXT = 4004, OP_MOVE = 4005,
        OP_BUTTON = 4006,
    };
    // sprite slots
    enum Slot : int32_t {
        SLOT_BGM = 5001, SLOT_SE = 5002, SLOT_BG = 5003, SLOT_PICTURE = 5004, SLOT_WINDOW = 5005,
        SLOT_HIGE = 5006, SLOT_CURSOR = 5007, SLOT_CHARA = 5008, SLOT_BLACK = 5014,
    };
    static constexpr int32_t SLOT_COUNT = 5014;

    const uint8_t layer[0x210];                 // cocos2d::Layer
    const uint8_t onEnterCallback[24];          // +0x210 Node::_onEnterCallback (std::function<void()>)
    const uint8_t onExitCallback[24];           // +0x228 Node::_onExitCallback
    const uint8_t pad_240[0x48];
    rw::Field<uint32_t> sprites;                // +0x288 cocos2d::Vector<Sprite*> (data begin)
    rw::Field<uint32_t> spritesEnd;             // +0x28C
    rw::Field<uint32_t> spritesCap;             // +0x290
    rw::Field<uint8_t> touchFlag;               // +0x294
    const uint8_t pad_295[3];
    rw::Field<int32_t> currentOp;               // +0x298 op of the command being run
    rw::Field<uint32_t> nameLabel;              // +0x29C cocos2d::Label* (speaker name)
    rw::Field<uint32_t> speech;                 // +0x2A0 SpeechDisp* (speech in the window)
    rw::Field<uint32_t> speechFix;              // +0x2A4 SpeechDisp* (narration, no window)
    rw::Field<int32_t> windowType;              // +0x2A8 0 = narration, else window style 1..
    rw::Field<float> wait;                      // +0x2AC seconds until the next command (< 0: wait for input)
    rw::Field<uint32_t> command;                // +0x2B0 const Command* (next command)
    rw::Field<uint32_t> sequence;               // +0x2B4 shared_ptr<bg::Data> of the .l file
    rw::Field<uint32_t> sequenceCtl;            // +0x2B8
    const uint8_t flipped[SLOT_COUNT];          // +0x2BC per slot: flipped horizontally
    const uint8_t bound[SLOT_COUNT];            // +0x1652 per slot: a texture was bound
    rw::Field<uint8_t> textShown;               // +0x29E8 a speech is running
    rw::Field<uint8_t> waitingPage;             // +0x29E9 a page is shown, waiting for the tap
    const uint8_t pad_29EA[2];
    rw::Field<int32_t> sceneId;                 // +0x29EC
    rw::Field<uint8_t> sceneFlag;               // +0x29F0
    const uint8_t pad_29F1[3];
    rw::Field<uint32_t> avatarIcon;             // +0x29F4 AvatarIcon* (the player's avatar)
    const uint8_t speechPages[20];              // +0x29F8 unordered_map<SpeechDisp*, deque<string>>
    guest::Vector expressions;                  // +0x2A0C vector<Avatar::Expression> the avatar uses
    rw::Field<int32_t> avatarSlot;              // +0x2A18 slot the avatar icon sits on (-1: none)
    rw::Field<uint32_t> skipButton;             // +0x2A1C cocostudio::Armature* (FlashSkipAnimation)
    const uint8_t ttfSpeech[0x24];              // +0x2A20 TTFConfig (Cinema_Calligraphy_Web.ttf, 30)
    const uint8_t ttfName[0x24];                // +0x2A44 TTFConfig (DFPSoGei-W5.TTF, 30)
    const uint8_t labels[12];                   // +0x2A68 std::map<int, Label*>
    const uint8_t pad_2A74[4];

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x2A78;
    static constexpr uint32_t VTABLE = 0xF9EC80 + 8;    // _ZTV10SceneLight + 8

    static Addr construct(Addr self);                    // C2
    static Addr destroy(Addr self);                      // D2
    static Addr create();
    static Addr createScene();
    static uint32_t init(Addr self);
    static void load(Addr self, int32_t id);
    static uint32_t making(Addr self, int32_t a, int32_t sceneId, uint32_t flag);
    static uint32_t loadTexture(Addr self, Addr name);
    static uint32_t loadTextureBG(Addr self, int32_t id);
    static uint32_t loadTextureNPC(Addr self, int32_t id);
    static uint32_t loadTextureNARRATION(Addr self, int32_t id);
    static uint32_t bindTexture(Addr self, int32_t slot, int32_t texture, uint32_t fade);   // bind_texture_common
    static void removeText(Addr self);
    static void execCommandCheckAvatar(Addr self);
    static void update(Addr self, float dt);
    static void runSpeech(Addr self, Addr disp);
    static void runSpeechFix(Addr self, Addr disp);
    static uint32_t priExecCommand(Addr self, float dt);
    static void execCommand(Addr self, float dt);
    static void cmdButton(Addr self, int32_t slot, uint32_t unused, int32_t ms);
    static void cmdInit(Addr self);
    static void cmdBind(Addr self, int32_t slot, int32_t texture, int32_t flip);
    static void cmdPrt(Addr self, int32_t slot, int32_t z, int32_t unused);
    static void cmdText(Addr self, int32_t unused, int32_t nameId, int32_t textId);
    static void cmdMove(Addr self, int32_t slot, int32_t move, int32_t ms);
    static void nextScene(Addr self);
    static void cmdRebind(Addr self, int32_t slot, int32_t texture, int32_t flip);
    static void setHige(Addr self);

    // lambdas
    static void speechPageDone(Addr fn);                                  // runSpeech::$_10
    static void onTouchEnded(Addr fn, Addr touchRef, Addr eventRef);      // init::$_4
};
RW_LAYOUT(SceneLight, sprites, 0x288);
RW_LAYOUT(SceneLight, currentOp, 0x298);
RW_LAYOUT(SceneLight, sequenceCtl, 0x2B8);
RW_LAYOUT(SceneLight, flipped, 0x2BC);
RW_LAYOUT(SceneLight, bound, 0x1652);
RW_LAYOUT(SceneLight, textShown, 0x29E8);
RW_LAYOUT(SceneLight, avatarIcon, 0x29F4);
RW_LAYOUT(SceneLight, expressions, 0x2A0C);
RW_LAYOUT(SceneLight, ttfSpeech, 0x2A20);
RW_LAYOUT(SceneLight, labels, 0x2A68);
RW_SIZE(SceneLight, SceneLight::SIZE);
