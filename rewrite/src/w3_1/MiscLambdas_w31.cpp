// Small lambda bodies (std::function operator()) of several classes whose remaining functions the owner tool
// attributed to this group's classes by position: AimTarget::AimMarker, MarkFader, SceneManager.
#include "scene/SceneCommon.hpp"

namespace misc_lambdas_w31 {

using scene::ld32;

constexpr uint32_t NODE_SCHEDULE_UPDATE = 0x894BA7;    // cocos2d::Node::scheduleUpdate()
constexpr uint32_t DIRECTOR_GET_INSTANCE = 0x8EE519;
constexpr uint32_t LOG = 0xCA4A3D;                     // cocos2d::log(const char*, ...)
constexpr uint32_t LIT_FRAME_LOG = 0xE02193;           // "frame: %d\n"
constexpr uint32_t DIRECTOR_TOTAL_FRAMES = 0xD4;       // Director::_totalFrames
constexpr uint32_t FUNCTION_VV_CALL = 0x470E69;        // std::function<void()>::operator()
constexpr uint32_t SPRITE_CREATE = 0x8A64DD;           // cocos2d::Sprite::create()
constexpr uint32_t CLIPPING_SET_STENCIL = 0x874FD1;    // cocos2d::ClippingNode::setStencil(Node*)

// AimTarget::AimMarker::init $_0 (captures the marker): clear its +648 field and start its update
uint32_t aimMarkerInitLambda(uint32_t lambda) {
    uint32_t marker = ld32(lambda + 4);
    rw::save<uint32_t>(marker + 648, 0);
    return rw::call(NODE_SCHEDULE_UPDATE, {marker});
}

// MarkFader::fadeIn $_2 (captures a std::function<void()> at +8): log the frame, then call it
uint32_t markFaderFadeInDoneLambda(uint32_t lambda) {
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    rw::call(LOG, {LIT_FRAME_LOG, ld32(director + DIRECTOR_TOTAL_FRAMES)});
    return rw::call(FUNCTION_VV_CALL, {lambda + 8});
}

// MarkFader::fadeIn $_1 (captures the clipping node): log the frame, then an empty sprite as its stencil
uint32_t markFaderFadeInStencilLambda(uint32_t lambda) {
    uint32_t clipping = ld32(lambda + 4);
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    uint32_t logged = rw::call(LOG, {LIT_FRAME_LOG, ld32(director + DIRECTOR_TOTAL_FRAMES)});
    uint32_t sprite = rw::call(SPRITE_CREATE, {logged});
    return rw::call(CLIPPING_SET_STENCIL, {clipping, sprite});
}

// MarkFader::init $_0 (touch began): swallow every touch
uint32_t markFaderInitTouchLambda() { return 1; }

// SceneManager::init $_3 (captures a node): start its update
uint32_t sceneManagerInitScheduleLambda(uint32_t lambda) {
    return rw::call(NODE_SCHEDULE_UPDATE, {ld32(lambda + 4)});
}

}  // namespace misc_lambdas_w31

RW_FUNCTION(0x73FCDA, "AimTarget::AimMarker::init::$_0 operator()()", misc_lambdas_w31::aimMarkerInitLambda);
RW_FUNCTION(0x5DA110, "MarkFader::fadeIn::$_2 operator()()", misc_lambdas_w31::markFaderFadeInDoneLambda);
RW_FUNCTION(0x5DA1D4, "MarkFader::fadeIn::$_1 operator()()", misc_lambdas_w31::markFaderFadeInStencilLambda);
RW_FUNCTION(0x5DA29A, "MarkFader::init::$_0 operator()(Touch*, Event*)", misc_lambdas_w31::markFaderInitTouchLambda);
RW_FUNCTION(0x638192, "SceneManager::init::$_3 operator()()", misc_lambdas_w31::sceneManagerInitScheduleLambda);
