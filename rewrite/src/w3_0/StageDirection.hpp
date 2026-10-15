// StageDirection: the quest start / end "direction" layer (a cocos2d::Node, 0x2C0 bytes; not derived from
// CommonDirection). It plays the opening of a quest (a black LayerColor fading out, then the target title
// armature ActMapTargetTitleAnim with the target enemy's name and its model, or in the colosseum the
// ActMapColosseumAnim title with the class / round numbers drawn from digit textures), the stage name panel, the
// "all in 1 turn" colosseum challenge, and the finish / colosseum clear / lose titles.
#pragma once

#include "../scene/SceneCommon.hpp"

struct StageDirection {
    rw::Field<uint32_t> vtable;              // +0x000
    const uint8_t nodeBody[0x20C];           // +0x004 cocos2d::Node
    const uint8_t onEnterCallback[24];       // +0x210 Node::_onEnterCallback (std::function<void()>, set by init)
    const uint8_t onExitCallback[24];        // +0x228 Node::_onExitCallback
    const uint8_t nodeTail[0x34];            // +0x240
    rw::Field<int32_t> kind;                 // +0x274 0 = quest, 1 = colosseum
    rw::Field<uint32_t> field_278;           // +0x278
    rw::Field<uint32_t> fadeLayer;           // +0x27C LayerColor (black) of stage_start / to_end
    rw::Field<uint32_t> titleArmature;       // +0x280 the title armature (target title / colosseum)
    rw::Field<uint32_t> titleBackArmature;   // +0x284 the second target title armature (movement 1)
    rw::Field<uint32_t> field_288;           // +0x288
    rw::Field<uint32_t> namePanel;           // +0x28C Sprite: the stage name panel
    const uint8_t pad_290[4];                // +0x290
    rw::Field<uint32_t> targetEnemy;         // +0x294 Enemy* shown in the target title
    rw::Field<int32_t> stageArg;             // +0x298 stage_start's argument
    rw::Field<int32_t> titleHold;            // +0x29C 1 = the title waits (isTitleHold), 2 after TitleHoldReset
    rw::Field<uint8_t> endTitle;             // +0x2A0 the opening is over (isEndTitle)
    const uint8_t pad_2A1[3];
    rw::Field<int32_t> state;                // +0x2A4 0, 4 when the opening ends
    rw::Field<uint8_t> field_2A8;            // +0x2A8
    const uint8_t pad_2A9[3];
    rw::Field<uint32_t> field_2AC;           // +0x2AC
    const uint8_t pad_2B0[4];                // +0x2B0
    rw::Field<int32_t> field_2B4;            // +0x2B4 -1
    const uint8_t pad_2B8[4];                // +0x2B8
    rw::Field<uint8_t> field_2BC;            // +0x2BC 1
    const uint8_t pad_2BD[3];

    static constexpr uint32_t SIZE = 0x2C0;
    static constexpr uint32_t VTABLE = 0xFA93DC;   // _ZTV14StageDirection + 8

    static uint32_t create();
    static StageDirection *construct(StageDirection *self);
    static StageDirection *destruct(StageDirection *self);
    static void destructDelete(StageDirection *self);
    static bool init(StageDirection *self);
    static void preloadResources(StageDirection *self);
    static void exec_stage_title(StageDirection *self, int32_t arg);
    static void stage_start(StageDirection *self, int32_t arg);
    static void stage_name_start(StageDirection *self, int32_t arg);
    static void cocos_sub_start(StageDirection *self, uint32_t armatureOut, int32_t movement, int32_t z, float x,
                                float y);
    static void cocos_start(StageDirection *self, int32_t enemyId);
    static void cocos_finish(StageDirection *self, uint32_t callFunc);
    static void cocos_clear_colosseum(StageDirection *self, uint32_t callFunc);
    static void cocos_lose_colosseum(StageDirection *self, uint32_t callFunc);
    static void all1turn_start(StageDirection *self, uint32_t callFunc);
    static StageDirection *to_start(StageDirection *self);
    static void to_end(StageDirection *self);
    static void update(StageDirection *self, float dt);
    static uint8_t isEndTitle(StageDirection *self);
    static bool isTitleHold(StageDirection *self);
    static StageDirection *TitleHoldReset(StageDirection *self);
    static bool turn_and_face_boss(StageDirection *self, uint32_t on);
};
RW_LAYOUT(StageDirection, onEnterCallback, 0x210);
RW_LAYOUT(StageDirection, kind, 0x274);
RW_LAYOUT(StageDirection, titleArmature, 0x280);
RW_LAYOUT(StageDirection, namePanel, 0x28C);
RW_LAYOUT(StageDirection, targetEnemy, 0x294);
RW_LAYOUT(StageDirection, titleHold, 0x29C);
RW_LAYOUT(StageDirection, state, 0x2A4);
RW_LAYOUT(StageDirection, field_2BC, 0x2BC);
RW_SIZE(StageDirection, 0x2C0);

// Guest addresses shared by the StageDirection files.
namespace sdir {
constexpr uint32_t Node_ctor = 0x892531, Node_dtor = 0x8927ED;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_runAction = 0x894AED, Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Director_getTextureCache = 0x8EFE69;
constexpr uint32_t TextureCache_addImage = 0x917DA9;      // (cache, const std::string&)
constexpr uint32_t Texture2D_getContentSize = 0x914ACB;   // (Size* out, texture)
constexpr uint32_t Sprite_create = 0x8A64DD;              // Sprite::create()
constexpr uint32_t LayerColor_create = 0x88B539;          // (const Color4B&)
constexpr uint32_t ParticleSystemQuad_create = 0x8A0B3D;  // (const std::string& plist)
constexpr uint32_t Label_createWithTTF = 0x883AE5;        // (const TTFConfig&, const std::string&, hAlign, maxWidth)
constexpr uint32_t Label_setAdditionalKerning = 0x88697D; // (label, float)
constexpr uint32_t Size_ctor = 0x8DF027;                  // Size(out, w, h)
constexpr uint32_t Size_assign = 0x8DF041;                // Size::operator=(const Size&)
constexpr uint32_t Rect_ctor = 0x8DF18B;                  // Rect(out, x, y, w, h)
constexpr uint32_t Vec2_ctor_xy = 0x8E4B9D, Vec2_ctor_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Color4B_ctor = 0x9064D3;               // (out, r, g, b, a)
constexpr uint32_t DelayTime_create = 0x86852D, FadeTo_create = 0x86B3E1, MoveTo_create = 0x86941D;
constexpr uint32_t ScaleTo_create = 0x86A8CD, Spawn_create = 0x8682F5, Sequence_create = 0x867999;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t EaseSineIn_create = 0x860C05, EaseSineOut_create = 0x860D09;
constexpr uint32_t EaseCubicActionIn_create = 0x862669;
constexpr uint32_t Armature_create = 0x79F3C7;            // (const std::string&)
constexpr uint32_t AnimationData_getMovement = 0x7A767D;  // (data, const std::string&)
constexpr uint32_t Bone_addDisplay = 0x7A2657;            // (bone, Node*, index)
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t function_vv_assign = 0x4EB9CD;         // std::function<void()>::operator=(const&)
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;     // (sret shared_ptr<Layer>, mgr, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;         // (layer, ResourceID, int, int)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t TextData_get = 0x4928AD;               // (sret shared_ptr, type, id)
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t master_Enemy_create = 0x77F7DD, master_Stage_create = 0x78E4E9;
constexpr uint32_t master_Colosseum_create = 0x77C4E9, master_ColosseumStage_create = 0x77CF95;
constexpr uint32_t Enemy_create = 0x5CBE25;               // Enemy::create(int model id)

constexpr uint32_t COLOR4B_BLACK = 0x1007DBC;             // cocos2d::Color4B::BLACK
constexpr uint32_t COLOR3B_WHITE = 0x1007E88;             // cocos2d::Color3B::WHITE
constexpr uint32_t BLENDFUNC_ADDITIVE = 0xEBBD8C;         // cocos2d::BlendFunc::ADDITIVE

// virtual slots (byte offsets)
constexpr uint32_t VT_INIT = 0x248;                       // Node::init
constexpr uint32_t VT_SET_OPACITY = 0x214;                // Node::setOpacity
constexpr uint32_t VT_SPRITE_SET_TEXTURE = 0x27C;         // Sprite::setTexture(Texture2D*)
constexpr uint32_t VT_SPRITE_SET_TEXTURE_RECT = 0x284;    // Sprite::setTextureRect(const Rect&)
constexpr uint32_t VT_SPRITE_SET_BLEND_FUNC = 0x2B8;      // Sprite::setBlendFunc(const BlendFunc&)
constexpr uint32_t VT_ARMATURE_GET_BONE = 0x27C;          // Armature::getBone(const std::string&)
constexpr uint32_t VT_GET_ANIMATION = 0x298;              // Armature::getAnimation
constexpr uint32_t VT_BONE_GET_DISPLAY_MANAGER = 0x29C;   // Bone::getDisplayManager
constexpr uint32_t VT_DISPLAY_SET_VISIBLE = 0x18;         // DisplayManager::setVisible(bool)
constexpr uint32_t VT_ANIM_PLAY_WITH_INDEX = 0x70;        // ArmatureAnimation::playWithIndex(index, durationTo, loop)
constexpr uint32_t VT_ANIM_GET_ANIMATION_DATA = 0x88;     // ArmatureAnimation::getAnimationData
constexpr uint32_t VT_LABEL_ENABLE_OUTLINE = 0x2B8;       // Label::enableOutline(const Color4B&, int)
constexpr uint32_t VT_PARTICLE_SET_AUTO_REMOVE = 0x280;   // ParticleSystem::setAutoRemoveOnFinish(bool)

// a temporary std::string of a literal (guest address, length) on the guest stack
inline uint32_t literal(scene::Frame &f, uint32_t text, uint32_t len) { return scene::newStr(f, text, len); }
}  // namespace sdir
