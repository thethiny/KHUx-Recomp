// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// AlbumChallengeScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2: __func::~__func (deleting)
static void t_5BCB24(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5BCB24, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::~__func (deleting)", t_5BCB24);

// AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2: __func::__clone()
static uint32_t t_5BCB2C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c9d8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5BCB2C, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::__clone()", t_5BCB2C);

// AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2: __func::__clone(__base*)
static void t_5BCB58(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c9d8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5BCB58, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::__clone(__base*)", t_5BCB58);

// AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2: __func::destroy
static void t_5BCB78(uint32_t self) {
}
RW_FUNCTION(0x5BCB78, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::destroy", t_5BCB78);

// AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2: __func::destroy_deallocate
static void t_5BCB7A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5BCB7A, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::destroy_deallocate", t_5BCB7A);
