// RankingScrollView: the selection functions (shared with the other batch #9 lists, RowTouch.hpp) and the row's
// avatar lambda: tapping a ranked player opens their profile (SceneUtil::openAvatarInfoPopup).
#include "RowTouch.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_openAvatarInfoPopup = 0x46A229;   // (u64 userId, bool, function<void(bool)>, function<bool()>)
constexpr uint32_t updateSelectItems = 0x5527E1, setSelectItem = 0x55263D;
}  // namespace g

constexpr uint32_t F_SELECTED = 0x670;          // std::vector<int>
constexpr uint32_t F_POPUP_OPEN = 0x660, F_AVATAR_TAP = 0x661;   // bools
constexpr uint32_t VT_POPUP_CLOSED = 0xF58A98;  // {this}
constexpr uint32_t V_isVisible = 0xB4;

// the touch fields are unused here (RankingScrollView's rows have no own listener)
const rowtouch::View VIEW = {F_SELECTED, 0, 0, 0, 0, 0, 0, g::setSelectItem, g::updateSelectItems, 0};
}  // namespace

struct RankingScrollViewSelect {
    static void setSelectItem(uint32_t self, uint32_t id) { rowtouch::setSelectItem(VIEW, self, id); }
    static void updateSelectItems(uint32_t self) { rowtouch::updateSelectItems(VIEW, self); }
    static uint32_t searchSelect(uint32_t self, uint32_t id) { return rowtouch::searchSelect(VIEW, self, id); }
    static void avatarTouched(uint32_t fo, uint32_t sender, uint32_t typeRef);
};

// {?, this, row widget, userId (u64)}: released on a shown row while avatar taps are on and no profile is open
void RankingScrollViewSelect::avatarTouched(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t self = ld32(fo + 8);
    if (!ld8(self + F_AVATAR_TAP)) return;
    const uint32_t widget = ld32(fo + 0xC);
    if (vcall(widget, V_isVisible, {widget}) != 1) return;
    if (ld8(self + F_POPUP_OPEN)) return;
    st8(self + F_POPUP_OPEN, 1);
    Frame f;
    const uint32_t closed = newFunction(f, VT_POPUP_CLOSED, {self});
    const uint32_t check = newEmptyFunction(f);
    call(g::SceneUtil_openAvatarInfoPopup, {ld32(fo + 0x10), ld32(fo + 0x14), 0, closed, check});
    functionDestroy(check);
    functionDestroy(closed);
}

RW_FUNCTION(0x55263C, "RankingScrollView::setSelectItem", RankingScrollViewSelect::setSelectItem);
RW_FUNCTION(0x5527E0, "RankingScrollView::updateSelectItems", RankingScrollViewSelect::updateSelectItems);
RW_FUNCTION(0x552960, "RankingScrollView::searchSelect", RankingScrollViewSelect::searchSelect);
RW_FUNCTION(0x552F00, "RankingScrollView avatar touch lambda __func::operator() (sub_552F00)",
            RankingScrollViewSelect::avatarTouched);
