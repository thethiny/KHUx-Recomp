// RankingRewardScrollView (Rankings > Rewards): the reward icon's balloon - a "balloonPanel" on popup layer 3 with
// the reward's name (a medal's skill name through TextData 103900067), its count and its picture, placed over the
// tapped icon (left / centre / right art by the icon's place in the row) - and the list's selection and row touch
// functions (RowTouch.hpp).
#include "RowTouch.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_updateMedalRarePanel = 0x4619F5;   // (widget, rarity, align, scale, bool, int, int)
constexpr uint32_t SceneManager_getPopupLayer3 = 0x613E79;
constexpr uint32_t AvatarParts_create = 0x778DE1, Medal_create = 0x785871, Skill_create = 0x78BDE9;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t Node_convertToWorldSpace = 0x895A39;
constexpr uint32_t Vec2_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17;
// this class
constexpr uint32_t updateSelectItems = 0x560889, setSelectItem = 0x5613D1, touchEndedBody = 0x561675;
}  // namespace g

constexpr uint32_t F_SELECTED = 0x668;          // std::vector<int>
constexpr uint32_t VT_ENDED = 0xF58C7C, VT_TAP_DONE = 0xF58CA8;
constexpr uint32_t V_getParent = 0x11C, V_getLocalZOrder = 0x1C;
// strings
constexpr uint32_t S_BalloonJson = 0xDFEF80, S_RankBalloon = 0xDFEFA8, S_BalloonPanel = 0xDFEFB5;
constexpr uint32_t S_Balloon2_03 = 0xDFEFC2, S_Balloon1_03 = 0xDFEFE3, S_Balloon2_02 = 0xDFF004;
constexpr uint32_t S_Balloon1_02 = 0xDFF025, S_Balloon2_01 = 0xDFF046, S_Balloon1_01 = 0xDFF067;
constexpr uint32_t S_ItemPanelAreaD = 0xDFF088, S_TxtIncentive = 0xDFF098, S_TxtIncentiveNum = 0xDFF0A6;
constexpr uint32_t S_TimesD = 0xDFF0B8, S_Area = 0xDF9378, S_IconPanel = 0xDFEC2D;
constexpr uint32_t S_Incentive = 0xDFEF2F, S_Medal = 0xDFD05F, S_KB = 0xDFEF39, S_IconBase = 0xDFEB70;
constexpr uint32_t S_IconBaseL = 0xDFEB7A, S_IconAvatar = 0xDFEBAB, S_Stamp = 0xDFED30, S_StarArea = 0xDF8C22;
constexpr uint32_t S_BalloonR = 0xDFEF3C, S_BalloonC = 0xDFECF0, S_BalloonL = 0xDFEF4B;
constexpr int32_t TEXT_SKILL_NAME = 103900067;

const rowtouch::View VIEW = {F_SELECTED, 0x659, 0x65C, 0x660, 0x664, VT_ENDED, VT_TAP_DONE,
                             g::setSelectItem, g::updateSelectItems, g::touchEndedBody};

inline void show(uint32_t n, uint32_t on) { vcall(n, node::setVisible, {n, on}); }
inline uint32_t strChars(uint32_t s) { return (ld8(s) & 1) ? ld32(s + 8) : s + 1; }
void loadTexture(uint32_t image, uint32_t path, uint32_t len) {
    Frame f;
    const uint32_t s = newStr(f, path, len);
    call(g::ImageView_loadTexture, {image, s, 0});
    strFree(s);
}
// the icon's world position plus the row's position
void balloonPosition(uint32_t out, uint32_t icon, uint32_t row) {
    Frame f;
    const uint32_t parent = vcall(icon, V_getParent, {icon});
    const uint32_t pos = vcall(icon, node::getPosition, {icon});
    call(g::Node_convertToWorldSpace, {out, parent, pos});
    const uint32_t rowPos = f.alloc(8);
    call(g::Vec2_copy, {rowPos, vcall(row, node::getPosition, {row})});
    stf(out, ldf(rowPos) + ldf(out));
    stf(out + 4, ldf(rowPos + 4) + ldf(out + 4));
    call(g::Vec2_dtor, {rowPos});
}
}  // namespace

struct RankingRewardExtra {
    static void setIncentiveItemBalloon(uint32_t icon, int32_t type, int32_t skill, uint32_t id, uint32_t image,
                                        uint32_t name, uint32_t count, int32_t pos, uint32_t flag, uint32_t row);
    static void setIncentiveItemBalloonUpdate(uint32_t icon, uint32_t row);
    static void setSelectItem(uint32_t self, uint32_t id) { rowtouch::setSelectItem(VIEW, self, id); }
    static uint32_t searchSelect(uint32_t self, uint32_t id) { return rowtouch::searchSelect(VIEW, self, id); }
    static void assignEnded(uint32_t dst, uint32_t caps) { rowtouch::assignEnded(VIEW, dst, caps); }
    static void touchEndedBody(uint32_t caps, uint32_t touch) { rowtouch::touchEndedBody(VIEW, caps, touch); }
    static void tapDone(uint32_t fo) { rowtouch::tapDone(VIEW, fo); }
    static void touchMoved(uint32_t fo, uint32_t touchRef) { rowtouch::touchMoved(VIEW, fo, touchRef); }
    static uint32_t touchBegan(uint32_t fo, uint32_t touchRef) { return rowtouch::touchBegan(VIEW, fo, touchRef); }
};

// The balloon is made once (its art by the icon's place: under 2 left, under 8 centre, else right; the flag picks
// the second art set) with an icon panel "ItemPanelArea<pos+1>" in its Area, then moved over the icon and filled.
// Ranking_Balloon.json is loaded on every call, also when the balloon exists.
void RankingRewardExtra::setIncentiveItemBalloon(uint32_t icon, int32_t type, int32_t skill, uint32_t id,
                                                 uint32_t image, uint32_t name, uint32_t count, int32_t pos,
                                                 uint32_t flag, uint32_t row) {
    Frame f;
    const uint32_t layer = call(g::SceneManager_getPopupLayer3, {call(fn::SceneManager_getInstance, {})});
    const uint32_t json = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_BalloonJson});
    const uint32_t art = findNode(json, S_RankBalloon, 0xC);
    const uint32_t at = f.alloc(8);
    balloonPosition(at, icon, row);
    uint32_t balloon = findNode(layer, S_BalloonPanel, 0xC);
    if (!balloon) {
        const bool second = flag == 1;
        if (pos <= 1) loadTexture(art, second ? S_Balloon2_03 : S_Balloon1_03, 0x20);
        else if (pos < 8) loadTexture(art, second ? S_Balloon2_01 : S_Balloon1_01, 0x20);
        else loadTexture(art, second ? S_Balloon2_02 : S_Balloon1_02, 0x20);
        {
            Frame g2;
            const uint32_t clone = call(g::Widget_clone, {json});
            const uint32_t s = newStr(g2, S_BalloonPanel, 0xC);
            vcall(layer, node::addChildZName, {layer, clone, 0, s});
            strFree(s);
        }
        balloon = findNode(layer, S_BalloonPanel, 0xC);
        const uint32_t area = findNode(balloon, S_Area, 4);
        const uint32_t panel = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_IconPanel});
        const uint32_t clone = call(g::Widget_clone, {panel});
        const uint32_t z = vcall(area, V_getLocalZOrder, {area});
        Frame g2;
        const uint32_t buf = stringBuffer(g2, {S_ItemPanelAreaD, uint32_t(pos + 1)});
        const uint32_t s = newStr(g2, buf);
        vcall(area, node::addChildZName, {area, clone, z, s});
        strFree(s);
    }
    vcall(balloon, node::setPosition, {balloon, at});
    {
        const uint32_t text = findNode(balloon, S_TxtIncentive, 0xD);
        call(g::SceneUtil_setShadow, {text});
        if (skill < 1) {
            setTextC(text, strChars(name));
        } else {
            Frame g2;
            const uint32_t row2 = masterRow(g2, g::Skill_create, uint32_t(skill));
            if (ld32(row2)) {
                const uint32_t sb = ld32(row2);
                const uint32_t skillName = g2.alloc(0x44);
                rw::call(g::SafeBuffer_read, {sb, skillName, ld32(sb + 0xC) + 0xC, 0x41, 0xC});
                const uint32_t td = textData(g2, TEXT_SKILL_NAME);
                setTextC(text, stringBuffer(g2, {textDataStr(td), strChars(name), skillName}));
                sharedRelease(td);
            } else {
                setTextC(text, strChars(name));
            }
            sharedRelease(row2);
        }
    }
    {
        const uint32_t text = findNode(balloon, S_TxtIncentiveNum, 0x11);
        call(g::SceneUtil_setShadow, {text});
        Frame g2;
        setTextC(text, stringBuffer(g2, {S_TimesD, count}));
    }
    const uint32_t incentive = findNode(balloon, S_Incentive, 9);
    const uint32_t medal = findNode(balloon, S_Medal, 5);
    const uint32_t kb = findNode(balloon, S_KB, 2);
    const uint32_t iconBase = findNode(balloon, S_IconBase, 9);
    const uint32_t iconBaseL = findNode(balloon, S_IconBaseL, 0xB);
    const uint32_t iconAvatar = findNode(balloon, S_IconAvatar, 0xB);
    const uint32_t stamp = findNode(balloon, S_Stamp, 5);
    const uint32_t star = findNode(balloon, S_StarArea, 9);
    show(star, 0);
    const uint32_t right = findNode(balloon, S_BalloonR, 0xE);
    const uint32_t centre = findNode(balloon, S_BalloonC, 0xC);
    const uint32_t left = findNode(balloon, S_BalloonL, 0xE);
    show(right, 0);
    show(centre, 0);
    show(left, 0);
    switch (type) {
    case 3: {
        show(incentive, 0);
        call(g::ImageView_loadTexture, {medal, image, 0});
        show(medal, 1);
        show(kb, 0);
        show(iconBase, 0);
        show(iconBaseL, 0);
        show(iconAvatar, 0);
        show(stamp, 0);
        show(star, 1);
        Frame g2;
        const uint32_t m = masterRow(g2, g::Medal_create, id);
        const uint32_t rarity = g2.alloc(4);
        rw::call(g::SafeBuffer_read, {ld32(m), rarity, ld32(ld32(m) + 0xC) + 0x470, 4, 0x470});
        call(g::SceneUtil_updateMedalRarePanel, {star, ld32(rarity), 1, fbits(0.5f), 1, 0, 0});
        sharedRelease(m);
        break;
    }
    case 13:
        show(incentive, 0);
        show(medal, 0);
        call(g::ImageView_loadTexture, {kb, image, 0});
        show(kb, 1);
        show(iconBase, 0);
        show(iconBaseL, 0);
        show(iconAvatar, 0);
        show(stamp, 0);
        break;
    case 18: case 19: case 20: case 23: {
        Frame g2;
        const uint32_t parts = masterRow(g2, g::AvatarParts_create, id);
        const uint32_t rarity = g2.alloc(4);
        rw::call(g::SafeBuffer_read, {ld32(parts), rarity, ld32(ld32(parts) + 0xC) + 0x8C, 4, 0x8C});
        const uint32_t r = ld32(rarity);
        show(incentive, 0);
        show(medal, 0);
        show(kb, 0);
        show(iconBase, r == 1 ? 1u : 0u);
        show(iconBaseL, r == 2 ? 1u : 0u);
        show(iconAvatar, 1);
        call(g::ImageView_loadTexture, {iconAvatar, image, 0});
        show(stamp, 0);
        sharedRelease(parts);
        break;
    }
    case 24:
        show(incentive, 0);
        show(medal, 0);
        show(kb, 0);
        show(iconAvatar, 0);
        show(iconBase, 0);
        show(iconBaseL, 0);
        show(stamp, 1);
        call(g::ImageView_loadTexture, {stamp, image, 0});
        break;
    default:
        call(g::ImageView_loadTexture, {incentive, image, 0});
        show(incentive, 1);
        show(medal, 0);
        show(kb, 0);
        show(iconBase, 0);
        show(iconBaseL, 0);
        show(iconAvatar, 0);
        show(stamp, 0);
        break;
    }
    call(g::Vec2_dtor, {at});
}

// the shown balloon follows the icon (the list scrolled)
void RankingRewardExtra::setIncentiveItemBalloonUpdate(uint32_t icon, uint32_t row) {
    const uint32_t layer = call(g::SceneManager_getPopupLayer3, {call(fn::SceneManager_getInstance, {})});
    const uint32_t balloon = findNode(layer, S_BalloonPanel, 0xC);
    if (!balloon) return;
    Frame f;
    const uint32_t at = f.alloc(8);
    balloonPosition(at, icon, row);
    vcall(balloon, node::setPosition, {balloon, at});
    call(g::Vec2_dtor, {at});
}

RW_FUNCTION(0x55EEA8, "RankingRewardScrollView::setIncentiveItemBalloon", RankingRewardExtra::setIncentiveItemBalloon);
RW_FUNCTION(0x55FD18, "RankingRewardScrollView::setIncentiveItemBalloonUpdate",
            RankingRewardExtra::setIncentiveItemBalloonUpdate);
RW_FUNCTION(0x5613D0, "RankingRewardScrollView::setSelectItem", RankingRewardExtra::setSelectItem);
RW_FUNCTION(0x561574, "RankingRewardScrollView::searchSelect", RankingRewardExtra::searchSelect);
RW_FUNCTION(0x561334, "RankingRewardScrollView ended lambda function::operator= (sub_561334)",
            RankingRewardExtra::assignEnded);
RW_FUNCTION(0x561674, "RankingRewardScrollView ended lambda body (sub_561674)", RankingRewardExtra::touchEndedBody);
RW_FUNCTION(0x5618A0, "RankingRewardScrollView tap-done lambda __func::operator() (sub_5618A0)",
            RankingRewardExtra::tapDone);
RW_FUNCTION(0x5619B0, "RankingRewardScrollView moved lambda __func::operator() (sub_5619B0)",
            RankingRewardExtra::touchMoved);
RW_FUNCTION(0x561B54, "RankingRewardScrollView began lambda __func::operator() (sub_561B54)",
            RankingRewardExtra::touchBegan);
