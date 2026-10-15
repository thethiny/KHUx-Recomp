// std::function thunks of MapPrizes::createArmature's lambdas, which sit right before MapTreasure's in the binary
// (attributed to MapTreasure by address; MapPrizes.cpp does not register them). Same shapes as MapTreasure::init's
// two lambdas: $_0 is the empty movement-event lambda, $_1 the frame-event lambda (only its target_type here).
#include "FunctionThunks.hpp"

namespace {
namespace g {
constexpr uint32_t prizeMovementLambda = 0xF5D9D8;    // __func<MapPrizes::createArmature $_0> vtable + 8
constexpr uint32_t prizeMovementLambdaTS = 0xE4CED0;  // _ZTSZN9MapPrizes14createArmatureEPKcS1_E3$_0
constexpr uint32_t prizeMovementLambdaTI = 0xF931D4;  // _ZTIZN9MapPrizes14createArmatureEPKcS1_E3$_0
constexpr uint32_t prizeFrameLambdaTI = 0xF931C0;     // _ZTIZN9MapPrizes14createArmatureEPKcS1_E3$_1
}  // namespace g
}  // namespace

RW_FUNCTION(0x5DEDC4, "MapPrizes::createArmature $_1 __func::target_type", fnthunk::targetType<g::prizeFrameLambdaTI>);

RW_FUNCTION(0x5DEDD8, "MapPrizes::createArmature $_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5DEDE0, "MapPrizes::createArmature $_0 __func::__clone()", fnthunk::clone0<g::prizeMovementLambda>);
RW_FUNCTION(0x5DEE04, "MapPrizes::createArmature $_0 __func::__clone(__base*)", fnthunk::cloneInto0<g::prizeMovementLambda>);
RW_FUNCTION(0x5DEE20, "MapPrizes::createArmature $_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5DEE22, "MapPrizes::createArmature $_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5DEE2A, "MapPrizes::createArmature $_0 (movement event, empty)", fnthunk::nothing);
RW_FUNCTION(0x5DEE2C, "MapPrizes::createArmature $_0 __func::target", fnthunk::target<g::prizeMovementLambdaTS>);
RW_FUNCTION(0x5DEE4C, "MapPrizes::createArmature $_0 __func::target_type", fnthunk::targetType<g::prizeMovementLambdaTI>);
