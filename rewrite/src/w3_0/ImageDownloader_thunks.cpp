// ImageDownloader: the std::function __func thunks of its lambdas (destructors, clone - copy the captures into a new /
// given block -, destroy, target / target_type typeinfo checks), generated from the lifted code by a scratch
// tool (gen_thunks.py, symbolic walk of each thunk) and checked against it. The lambda bodies are written by hand.
#include "../scene/SceneCommon.hpp"

namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// ImageDownloader::getProfileImageUrl $_1: __func::~__func
static uint32_t getProfileImageUrl_L1_dtor(uint32_t self) {
    st32(self, 0xf5ca88u);
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5BDC88, "ImageDownloader::getProfileImageUrl $_1 __func::~__func", getProfileImageUrl_L1_dtor);

// ImageDownloader::getProfileImageUrl $_1: __func::~__func (deleting)
static void getProfileImageUrl_L1_deletingDtor(uint32_t self) {
    st32(self, 0xf5ca88u);
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5BDCD4, "ImageDownloader::getProfileImageUrl $_1 __func::~__func (deleting)", getProfileImageUrl_L1_deletingDtor);

// ImageDownloader::getProfileImageUrl $_1: __func::destroy
static void getProfileImageUrl_L1_destroy(uint32_t self) {
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5BDE5C, "ImageDownloader::getProfileImageUrl $_1 __func::destroy", getProfileImageUrl_L1_destroy);

// ImageDownloader::getProfileImageUrl $_1: __func::destroy_deallocate
static void getProfileImageUrl_L1_destroyDeallocate(uint32_t self) {
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5BDE90, "ImageDownloader::getProfileImageUrl $_1 __func::destroy_deallocate", getProfileImageUrl_L1_destroyDeallocate);

// ImageDownloader::getProfileImageUrl $_1: __func::target
static uint32_t getProfileImageUrl_L1_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe490f0u ? self + 8 : 0; }
RW_FUNCTION(0x5BE138, "ImageDownloader::getProfileImageUrl $_1 __func::target", getProfileImageUrl_L1_target);

// ImageDownloader::getProfileImageUrl $_1: __func::target_type
static uint32_t getProfileImageUrl_L1_targetType() { return 0xf8ed40u; }
RW_FUNCTION(0x5BE158, "ImageDownloader::getProfileImageUrl $_1 __func::target_type", getProfileImageUrl_L1_targetType);

}  // namespace
