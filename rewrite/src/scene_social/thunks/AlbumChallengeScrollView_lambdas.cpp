// The remaining std::function thunks and the lambda bodies of AlbumChallengeScrollView, plus those of
// RaidSubdueScrollView::registItemTouchEvent's lambdas that sit right before it in the binary (RaidSubdueScrollView
// is not rewritten; owners.py attributes them here).
#include "../../scene_main/FunctionThunks.hpp"
#include "../SocialCommon.hpp"
#include "AlbumChallengeScrollView.hpp"

using namespace social;

namespace {
namespace g {
constexpr uint32_t Vec2_dtor = 0x8E4C17;                   // cocos2d::Vec2::~Vec2
constexpr uint32_t Rect_copy = 0x8DF1A9;                   // cocos2d::Rect::Rect(const Rect&)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Touch_getLocation = 0x900E57;           // (Vec2* out, Touch*)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;     // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t IScrollProtocol_getItemPixel = 0x4F6751;
constexpr uint32_t divsi3 = 0xCD5E8D;                      // __divsi3
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;         // AudioLWFNode::create(const char* path, void*)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;   // (node, std::function<void()> by value)
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t AlbumChallenge_openRewardPopup = 0x5BBB8D;
}  // namespace g

constexpr uint32_t S_Area_Point = 0xDF9FB4;
constexpr uint32_t S_ResultRewardLwf = 0xDF9FBF;   // "lwf/battleresult/Result_Reward/Result_Reward.lwf"
constexpr uint32_t VT_popupClose = 0xF5CA30;      // openAlbumChallengeGetRewardPopup $_3::{lambda()#1} + 8
constexpr uint32_t VT_raidTouchBegan = 0xF5C9AC;  // RaidSubdueScrollView::registItemTouchEvent $_0 + 8

constexpr uint32_t TOUCH_ENDED = 2;               // ui::Widget::TouchEventType::ENDED
constexpr uint32_t V_isVisible = 0xB4;
constexpr uint32_t V_removeChild = 0x12C;          // (child, cleanup)
constexpr uint32_t V_addChild = 0xF0;

using thunk::cap;
using PopupOk1 = thunk::Lambda<0xF5CA04, 16, 0xF8ED04, 0xE48DE0, 4, cap(4, 4), cap(8, 4), cap(12, 4)>;
using PopupClose3 = thunk::Lambda<VT_popupClose, 16, 0xF8ED18, 0xE48EC0, 4, cap(4, 4), cap(8, 4), cap(12, 4)>;
using RowTouch0 = thunk::Lambda<0xF5CA5C, 16, 0xF8ED2C, 0xE48FC0, 4, cap(4, 4), cap(8, 4), cap(12, 4)>;
using PopupOpened2 = thunk::Lambda<0xF5C9D8, 8, 0xF8ECF0, 0xE48CE0, 4, cap(4, 4)>;
using RaidTouchMoved1 = thunk::Lambda<0xF5C980, 0, 0xF8E930, 0xE48B00, 4>;   // only target / target_type here
using RaidTouchBegan0 = thunk::Lambda<VT_raidTouchBegan, 0x1C, 0xF8E944, 0xE48BF0, 4>;
}  // namespace

struct AlbumChallengeLambdas {
    // openAlbumChallengeGetRewardPopup $_2 {popup} (the popup finished opening): the reward effect at "Area_Point"
    static void popupOpened(Addr func) {
        Addr popup = ld32(func + 4);
        Addr point = search(popup, S_Area_Point, 10);
        Addr lwf = call(g::AudioLWFNode_create, {S_ResultRewardLwf, 0});
        vcall(point, V_addChild, {point, lwf});
    }
    // $_1 (OK button {popup, layer, shadow}): on release, close the popup; $_3's lambda then removes the nodes
    static void popupOk(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        call(g::AudioPlaySystemSEDecide, {});
        Addr popup = ld32(func + 4);
        withLambda(VT_popupClose, {ld32(func + 8), ld32(func + 12), popup}, [&](Addr done) {
            Addr anim = call(g::SceneUtil_createClosePopupAnimation, {popup, done});
            call(g::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
        });
    }
    // $_3::{lambda()#1} {layer, shadow, popup}: remove the shadow and the popup from the popup layer
    static void popupClosed(Addr func) {
        Addr layer = ld32(func + 4);
        vcall(layer, V_removeChild, {layer, ld32(func + 8), 1});
        layer = ld32(func + 4);
        vcall(layer, V_removeChild, {layer, ld32(func + 12), 1});
    }
    // changeRow $_0 {row, button, view}: a release on the visible receive button of a visible row opens the popup
    static void rowTouch(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        Addr view = ld32(func + 12);
        Addr row = ld32(func + 4);
        if (row && vcall(row, V_isVisible, {row}) != 1) return;
        Addr button = ld32(func + 8);
        if (vcall(button, V_isVisible, {button}) != 1) return;
        call(g::AudioPlaySystemSEDecide, {});
        call(g::AlbumChallenge_openRewardPopup, {view});
    }
};

// RaidSubdueScrollView::registItemTouchEvent $_0 {view, Rect area, row}: onTouchBegan. A touch inside the row's
// area (not while a tap animation runs) selects item (row + first visible row - 1) when that item exists.
struct RaidSubdueTouchBegan {
    static Addr destructor(Addr self) {
        st32(self, VT_raidTouchBegan);
        call(g::Vec2_dtor, {self + 8});
        return self;
    }
    static void destructorDelete(Addr self) {
        st32(self, VT_raidTouchBegan);
        call(g::Vec2_dtor, {self + 8});
        call(fn::operator_delete, {self});
    }
    static Addr clone(Addr self) {
        Addr copy = call(fn::operator_new, {0x1C});
        if (copy) {
            st32(copy, VT_raidTouchBegan);
            st32(copy + 4, ld32(self + 4));
            call(g::Rect_copy, {copy + 8, self + 8});
            st32(copy + 0x18, ld32(self + 0x18));
        }
        return copy;
    }
    static void cloneInto(Addr self, Addr dst) {
        if (!dst) return;
        st32(dst, VT_raidTouchBegan);
        st32(dst + 4, ld32(self + 4));
        call(g::Rect_copy, {dst + 8, self + 8});
        st32(dst + 0x18, ld32(self + 0x18));
    }
    static void destroy(Addr self) { call(g::Vec2_dtor, {self + 8}); }
    static void destroyDeallocate(Addr self) {
        call(g::Vec2_dtor, {self + 8});
        call(fn::operator_delete, {self});
    }
    static uint32_t touchBegan(Addr func, const uint32_t *touchArg, const uint32_t *) {
        Addr touch = *touchArg;
        Addr view = ld32(func + 4);
        if (ld8(view + 0x63C)) return 0;   // tap animation running
        Frame f;
        Addr loc = f.alloc(8);
        Addr local = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        call(g::Node_convertToNodeSpace, {local, view, loc});
        call(g::Vec2_dtor, {loc});
        uint32_t result = 0;
        if (call(g::Rect_containsPoint, {func + 8, local}) == 1) {
            float pixel = rw::callf(g::IScrollProtocol_getItemPixel, {view});
            int32_t position = f2i(ldf(view + 0x540));
            int32_t rows = int32_t(call(g::divsi3, {uint32_t(position), uint32_t(f2i(pixel))}));
            int32_t item = int32_t(ld32(func + 0x18) + uint32_t(rows));
            uint32_t index = uint32_t(item) - 1;
            uint32_t count = (ld32(view + 0x62C) - ld32(view + 0x628)) >> 2;
            if (item >= 1 && index < count) {
                st32(view + 0x674, index);   // touched item
                st8(view + 0x670, 1);        // touching
                result = 1;
            }
        }
        call(g::Vec2_dtor, {local});
        return result;
    }
};

RW_FUNCTION(0x5BCB84, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::operator()", AlbumChallengeLambdas::popupOpened);
RW_FUNCTION(0x5BCC40, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::target", PopupOpened2::target);
RW_FUNCTION(0x5BCC60, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_2 __func::target_type", PopupOpened2::targetType);

RW_FUNCTION(0x5BCC74, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::~__func (deleting)", PopupOk1::deleteThis);
RW_FUNCTION(0x5BCC7C, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::__clone()", PopupOk1::clone);
RW_FUNCTION(0x5BCCAC, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::__clone(__base*)", PopupOk1::cloneInto);
RW_FUNCTION(0x5BCCD4, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::destroy", PopupOk1::destroy);
RW_FUNCTION(0x5BCCD6, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::destroy_deallocate", PopupOk1::deleteThis);
RW_FUNCTION(0x5BCCE0, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::operator()", AlbumChallengeLambdas::popupOk);
RW_FUNCTION(0x5BCDA4, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::target", PopupOk1::target);
RW_FUNCTION(0x5BCDC4, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_1 __func::target_type", PopupOk1::targetType);

RW_FUNCTION(0x5BCDD8, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::~__func (deleting)", PopupClose3::deleteThis);
RW_FUNCTION(0x5BCDE0, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::__clone()", PopupClose3::clone);
RW_FUNCTION(0x5BCE10, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::__clone(__base*)", PopupClose3::cloneInto);
RW_FUNCTION(0x5BCE38, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::destroy", PopupClose3::destroy);
RW_FUNCTION(0x5BCE3A, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::destroy_deallocate", PopupClose3::deleteThis);
RW_FUNCTION(0x5BCE42, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::operator()", AlbumChallengeLambdas::popupClosed);
RW_FUNCTION(0x5BCE6C, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::target", PopupClose3::target);
RW_FUNCTION(0x5BCE8C, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup $_3 UlvE_ __func::target_type", PopupClose3::targetType);

RW_FUNCTION(0x5BCEA0, "AlbumChallengeScrollView::changeRow $_0 __func::~__func (deleting)", RowTouch0::deleteThis);
RW_FUNCTION(0x5BCEA8, "AlbumChallengeScrollView::changeRow $_0 __func::__clone()", RowTouch0::clone);
RW_FUNCTION(0x5BCED8, "AlbumChallengeScrollView::changeRow $_0 __func::__clone(__base*)", RowTouch0::cloneInto);
RW_FUNCTION(0x5BCF00, "AlbumChallengeScrollView::changeRow $_0 __func::destroy", RowTouch0::destroy);
RW_FUNCTION(0x5BCF02, "AlbumChallengeScrollView::changeRow $_0 __func::destroy_deallocate", RowTouch0::deleteThis);
RW_FUNCTION(0x5BCF0A, "AlbumChallengeScrollView::changeRow $_0 __func::operator()", AlbumChallengeLambdas::rowTouch);
RW_FUNCTION(0x5BCF48, "AlbumChallengeScrollView::changeRow $_0 __func::target", RowTouch0::target);
RW_FUNCTION(0x5BCF68, "AlbumChallengeScrollView::changeRow $_0 __func::target_type", RowTouch0::targetType);

RW_FUNCTION(0x5BA89C, "RaidSubdueScrollView::registItemTouchEvent $_1 __func::target", RaidTouchMoved1::target);
RW_FUNCTION(0x5BA8BC, "RaidSubdueScrollView::registItemTouchEvent $_1 __func::target_type", RaidTouchMoved1::targetType);
RW_FUNCTION(0x5BA8D0, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::~__func", RaidSubdueTouchBegan::destructor);
RW_FUNCTION(0x5BA8FC, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::~__func (deleting)", RaidSubdueTouchBegan::destructorDelete);
RW_FUNCTION(0x5BA92C, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::__clone()", RaidSubdueTouchBegan::clone);
RW_FUNCTION(0x5BA984, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::__clone(__base*)", RaidSubdueTouchBegan::cloneInto);
RW_FUNCTION(0x5BA9C0, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::destroy", RaidSubdueTouchBegan::destroy);
RW_FUNCTION(0x5BA9CA, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::destroy_deallocate", RaidSubdueTouchBegan::destroyDeallocate);
RW_FUNCTION(0x5BA9E0, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::operator()", RaidSubdueTouchBegan::touchBegan);
RW_FUNCTION(0x5BAADC, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::target", RaidTouchBegan0::target);
RW_FUNCTION(0x5BAAFC, "RaidSubdueScrollView::registItemTouchEvent $_0 __func::target_type", RaidTouchBegan0::targetType);
