// Guest functions, globals, literals and small helpers of the PlayerStatusIndicator files.
#pragma once

#include <cmath>

#include "PlayerStatusIndicator.hpp"

namespace psi {
namespace lib {

// cocos2d / cocostudio / libc++ / game functions (Thumb bit set as the lifted calls use them)
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Node_init = rwlib::cocos2d::Node::init;
constexpr uint32_t Node_create = rwlib::cocos2d::Node::create;
constexpr uint32_t Node_setAnchorPoint = rwlib::cocos2d::Node::setAnchorPoint;
constexpr uint32_t Node_visit = rwlib::cocos2d::Node::visit__v;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Action_ctor = rwlib::cocos2d::Action::ctor_base;
constexpr uint32_t ActionInterval_initWithDuration = rwlib::cocos2d::ActionInterval::initWithDuration;
constexpr uint32_t RefVector_pushBack = 0x5E29A9;     // cocos2d::Vector<Ref*>::pushBack(Ref*)
constexpr uint32_t RefVector_dtor = 0x5E2971;         // cocos2d::Vector<Ref*>::~Vector()
constexpr uint32_t SpriteVector_pushBack = 0x5E4551;  // cocos2d::Vector<Sprite*>::pushBack(Sprite*)
constexpr uint32_t FloatFunction_swap = 0x5E40BD;     // std::function<void(float)>::swap(function&)
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;   // (reader, const char*)
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache;
constexpr uint32_t TextureCache_addImage = rwlib::cocos2d::TextureCache::addImage;  // (cache, const std::string&)
constexpr uint32_t Texture2D_getContentSize = rwlib::cocos2d::Texture2D::getContentSize;        // (Size* out, tex)
constexpr uint32_t Texture2D_getContentSizeInPixels = rwlib::cocos2d::Texture2D::getContentSizeInPixels;  // -> const Size&
constexpr uint32_t Texture2D_setAliasTexParameters = rwlib::cocos2d::Texture2D::setAliasTexParameters;
constexpr uint32_t Texture2D_setAntiAliasTexParameters = rwlib::cocos2d::Texture2D::setAntiAliasTexParameters;
constexpr uint32_t ClippingNode_create = rwlib::cocos2d::ClippingNode::create;
constexpr uint32_t ClippingNode_setStencil = rwlib::cocos2d::ClippingNode::setStencil;
constexpr uint32_t ClippingNode_setAlphaThreshold = rwlib::cocos2d::ClippingNode::setAlphaThreshold;
constexpr uint32_t ClippingNode_setInverted = rwlib::cocos2d::ClippingNode::setInverted;
constexpr uint32_t Sprite_createWithTexture = rwlib::cocos2d::Sprite::createWithTexture__PNS_9Texture2DE;      // (tex)
constexpr uint32_t Sprite_createWithTextureRect = rwlib::cocos2d::Sprite::createWithTexture__PNS_9Texture2DERKNS_4RectEb;  // (tex, const Rect&, bool rotated)
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__RKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE;                 // (const std::string&)
constexpr uint32_t Label_setAdditionalKerning = rwlib::cocos2d::Label::setAdditionalKerning;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;               // (const std::string&)
constexpr uint32_t strvec_allocate = 0x47B5FD;               // std::vector<std::string>::allocate(n)
constexpr uint32_t strvec_constructAtEnd = 0x5E407D;         // ::__construct_at_end(first, last)
constexpr uint32_t strvec_baseDtor = 0x47363F;               // std::__vector_base<std::string>::~__vector_base
constexpr uint32_t exprvec_allocate = 0x487635;              // std::vector<Avatar::Expression>::allocate(n)
constexpr uint32_t exprvec_baseDtor = 0x46BE0D;
constexpr uint32_t AvatarIcon_create = 0x5C9DB5;  // (style, Texture2D*, const Vec2&, float, float, const vector<Expression>&)
constexpr uint32_t AvatarIcon_changeExpression = 0x5CA459;   // (icon, Expression, float)
constexpr uint32_t audio_getInstance = 0x5BECFD;             // audio::Manager::getInstance()
constexpr uint32_t audio_load = 0x5BEEF5;                    // audio::Manager::load(ResourceID)
constexpr uint32_t audio_getLayer = 0x5BF33D;                // (shared_ptr<Layer>* out, mgr, int)
constexpr uint32_t audio_stop = 0x5BF3AD;                    // audio::Manager::stop(int handle, int fadeMs)
constexpr uint32_t audioLayer_playSE = 0x5BF785;             // (layer, ResourceID, int, int) -> handle
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t TextAtlas_setString = rwlib::cocos2d::ui::TextAtlas::setString;
constexpr uint32_t LoadingBar_setPercent = rwlib::cocos2d::ui::LoadingBar::setPercent;
constexpr uint32_t Color3B_ctor = rwlib::cocos2d::Color3B::ctor_base;                  // (out, r, g, b)
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t ScaleTo_create = rwlib::cocos2d::ScaleTo::create__ff;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;          // Size(float, float)
constexpr uint32_t Size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_;          // Size(const Size&)
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;          // Rect(x, y, w, h)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;          // Vec2(float, float)
constexpr uint32_t Vec2_ctor0 = rwlib::cocos2d::Vec2::ctor_base__v;         // Vec2()
constexpr uint32_t Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Vec2_add = rwlib::cocos2d::Vec2::add;
constexpr uint32_t RenderTexture_create = rwlib::cocos2d::RenderTexture::create__ii;       // (w, h)
constexpr uint32_t RenderTexture_create_fmt = rwlib::cocos2d::RenderTexture::create__iiNS_9Texture2D11PixelFormatEj;   // (w, h, PixelFormat, depthStencilFormat)
constexpr uint32_t DrawNode_create = rwlib::cocos2d::DrawNode::create;
constexpr uint32_t DrawNode_drawTriangle = rwlib::cocos2d::DrawNode::drawTriangle;      // (dn, a, b, c, const Color4F&)
constexpr uint32_t tanf_ = 0x41A968;              // (PLT)
constexpr uint32_t fmod_ = 0x41A908;              // (PLT, doubles)
constexpr uint32_t roundf_ = 0x419C54;            // (PLT)
constexpr uint32_t memcpy_ = 0x40E164;            // __aeabi_memcpy (PLT)
constexpr uint32_t arc_setRate = 0x5E4BED;        // (anonymous)::Arc::setRate(float) (sub_5E4BEC)

// globals
constexpr uint32_t VEC2_ANCHOR_MIDDLE_LEFT = 0xFEF5E8;
constexpr uint32_t VEC2_ANCHOR_TOP_LEFT = 0xFEF5C8;
constexpr uint32_t VEC2_ANCHOR_MIDDLE_RIGHT = 0xFEF5E0;
constexpr uint32_t VEC2_ANCHOR_BOTTOM_LEFT = 0x1006958;
constexpr uint32_t COLOR3B_WHITE = 0x1007E88;
constexpr uint32_t COLOR4F_WHITE = 0x1007EC8;

// virtual slots (byte offsets) beyond scene::node
constexpr uint32_t NODE_GET_LOCAL_Z_ORDER = 0x18;
constexpr uint32_t NODE_SET_CASCADE_OPACITY = 0x220;   // called with 1 on the ClippingNodes
constexpr uint32_t SPRITE_GET_TEXTURE = 0x280;
constexpr uint32_t RT_BEGIN_WITH_CLEAR = 0x274;        // RenderTexture::beginWithClear(r, g, b, a)
constexpr uint32_t RT_END = 0x280;                     // RenderTexture::end()
constexpr uint32_t RT_SPRITE = 740;                    // RenderTexture::_sprite
constexpr uint32_t WIDGET_GET_VIRTUAL_RENDERER = 0x2D8;
constexpr uint32_t WIDGET_29C = 0x29C;                 // called with 0 on the loaded layout
constexpr uint32_t ARMATURE_GET_ANIMATION = 0x298;
constexpr uint32_t ANIM_PLAY = 0x68;                   // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t ANIM_PLAY_WITH_NAMES = 0x74;        // ::playWithNames(const vector<string>&, durationTo, loop)

// literals
constexpr uint32_t STR_D = 0xDF86DE;                   // "%d"
constexpr uint32_t STR_LAYOUT_JSON = 0xE025B8;         // "cocostudio/publish/ActionMap_PlayerStatusField_new.json"
constexpr uint32_t STR_CIRCLE_BODY = 0xE025F0;         // "Avatar_Circle_Body" (18)
constexpr uint32_t STR_SIDE_BODY = 0xE02603;           // "Avatar_Side_Body" (16)
constexpr uint32_t STR_CIRCLE_MSK = 0xE02862;          // ".../Avatar_Circle_Msk.png" (50)
constexpr uint32_t STR_CIRCLE_04 = 0xE02895;           // ".../Avatar_Circle_04.png"
constexpr uint32_t STR_CIRCLE_03 = 0xE028C7;           // ".../Avatar_Circle_03.png"
constexpr uint32_t STR_HP_NUM = 0xE02614;              // "Avatar_HP_Num" (13)
constexpr uint32_t STR_HP_RED_NUM = 0xE02622;          // "Avatar_HPRed_Num" (16)
constexpr uint32_t STR_MEDAL_DECK_LABEL = 0xE02633;    // "Medal_Deck_Label" (16)
constexpr uint32_t STR_ANIM_JSON = 0xE02831;           // "cocostudio/publish/ActionMapAnimation.ExportJson" (48)
constexpr uint32_t STR_ACTION_MAP_ANIMATION = 0xE01C44;  // "ActionMapAnimation" (18)
constexpr uint32_t STR_HP_ALERT = 0xE02829;            // "HPAlert" (7)
constexpr uint32_t STR_BUERST_MAX = 0xE02644;          // "Buerst_MAX" (10)
constexpr uint32_t STR_BURST_MAX = 0xE0281F;           // "Burst_MAX" (9)
constexpr uint32_t STR_AVATAR_ICONTEST = 0xE0264F;     // "avatar_icontest" (15)
constexpr uint32_t STR_BURST_NUM = 0xE01B5E;           // "Avatar_Burst_Num" (16)
constexpr uint32_t STR_BURST_GAUGE = 0xE0265F;         // "Burst_Gauge" (11)
constexpr uint32_t STR_SIDE_04 = 0xE027BF;             // ".../Avatar_Side_04.png" (47)
constexpr uint32_t STR_SIDE_03 = 0xE027EF;             // ".../Avatar_Side_03.png" (47)
constexpr uint32_t STR_STATUS_FIELD = 0xE0266B;        // "PlayerStatusField" (17)
constexpr uint32_t STR_CIRCLE_01 = 0xE0267D;           // ".../Avatar_Circle_01.png" (49)
constexpr uint32_t STR_CIRCLE_05 = 0xE026AF;
constexpr uint32_t STR_CIRCLE_06 = 0xE026E1;
constexpr uint32_t STR_AVATAR_GAUGE = 0xE02713;        // "cocostudio/publish/avatar_gauge.png" (35)
constexpr uint32_t STR_SIDE_01 = 0xE02737;             // ".../Avatar_Side_01.png" (47)
constexpr uint32_t STR_SIDE_02 = 0xE02767;             // ".../Avatar_Side_02.png" (47)
constexpr uint32_t STR_BURST_USE = 0xE027B5;           // "Burst_Use" (9)
constexpr uint32_t STR_BURST_CHARGE = 0xE027A8;        // "Burst_Charge" (12)
constexpr uint32_t STR_SKILL_COST_CANCEL = 0xE02797;   // "Skill_CostCancel" (16)

constexpr uint32_t SE_HP_ALERT = 509000150;            // audio ResourceID of the HP alert loop
constexpr uint32_t QUARTER_PI = 0x3F490FDB;            // float pi / 4

}  // namespace lib

using scene::ldf;
using scene::stf;

inline uint64_t rw_dbits(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }
inline double rw_bitsd(uint64_t u) { double d; std::memcpy(&d, &u, 8); return d; }

inline void arcSetRate(uint32_t arcNode, float rate) { rw::call(lib::arc_setRate, {arcNode, rw::fbits(rate)}); }

inline uint32_t textureCache() {
    return rw::call(lib::Director_getTextureCache, {rw::call(lib::Director_getInstance, {})});
}
// TextureCache::addImage(std::string(literal)) with the given cache / the director's
inline uint32_t addImageTo(uint32_t cache, uint32_t path, uint32_t len) {
    scene::Frame f;
    uint32_t s = scene::newStr(f, path, len);
    uint32_t tex = rw::call(lib::TextureCache_addImage, {cache, s});
    scene::strFree(s);
    return tex;
}
inline uint32_t addImage(uint32_t path, uint32_t len) { return addImageTo(textureCache(), path, len); }

// TextAtlas::setString(printf("%d", value))
inline void setAtlasNumber(uint32_t atlas, uint32_t value) {
    scene::Frame f;
    uint32_t text = scene::stringBuffer(f, {lib::STR_D, value});
    uint32_t s = scene::newStr(f, text);
    rw::call(lib::TextAtlas_setString, {atlas, s});
    scene::strFree(s);
}

// armature->getAnimation()->playWithNames({name}, -1, loop)
inline void playNames(uint32_t armature, uint32_t name, uint32_t len, uint32_t loop) {
    uint32_t anim = scene::vcall(armature, lib::ARMATURE_GET_ANIMATION, {armature});
    uint32_t play = scene::ld32(scene::ld32(anim) + lib::ANIM_PLAY_WITH_NAMES);
    scene::Frame f;
    uint32_t s = scene::newStr(f, name, len);
    uint32_t names = f.alloc(12);
    scene::st32(names, 0);
    scene::st32(names + 4, 0);
    scene::st32(names + 8, 0);
    rw::call(lib::strvec_allocate, {names, 1});
    rw::call(lib::strvec_constructAtEnd, {names, s, s + 12});
    rw::call(play, {anim, names, 0xFFFFFFFFu, loop});
    rw::call(lib::strvec_baseDtor, {names});
    scene::strFree(s);
}

// node->setPosition(anchor->getPosition() + (width / 2, 0))
inline void placeRightOf(uint32_t node, uint32_t anchor, float width) {
    uint32_t setPosition = scene::ld32(scene::ld32(node) + scene::node::setPosition);
    uint32_t pos = scene::vcall(anchor, scene::node::getPosition, {anchor});
    scene::Frame f;
    uint32_t size = f.alloc(8), offset = f.alloc(8), p = f.alloc(8);
    rw::call(lib::Size_ctor, {size, rw::fbits(width * 0.5f), 0});
    rw::call(lib::Vec2_ctor, {offset, scene::ld32(size), scene::ld32(size + 4)});
    rw::call(lib::Vec2_copy, {p, pos});
    rw::call(lib::Vec2_add, {p, offset});
    rw::call(setPosition, {node, p});
    rw::call(lib::Vec2_dtor, {p});
    rw::call(lib::Vec2_dtor, {offset});
}

}  // namespace psi
