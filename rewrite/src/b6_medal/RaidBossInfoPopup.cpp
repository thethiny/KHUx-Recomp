// popUpRaidBossInfo(info, done): the raid boss's rewards (Adv_Raid_Boss_List_Reward): the boss's RaidParts for
// the raid's level (a SafeBuffer registered for updates: the original frame - push 0x14 + sub 0x394 - is kept so it
// sits at its original address), then three reward groups (attack, MVP, destruction: Rew1..3) of up to four rows
// each (sub_5085C8) with their incentive panels and titles, Close through SceneManager::openPopup and the eased open
// animation.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t SM_getPopupLayer = 0x613E69, SM_openPopup = 0x61524D;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create, EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t MasterRaidEnemy_create = 0x787745, MasterRaidReward_create = 0x789DF9;
constexpr uint32_t SafeBuffer_ctor = 0x459F21, SafeBuffer_dtor = 0x45A13D, SafeBuffer_read = 0x45A269;
constexpr uint32_t RaidParts_setMasterParam = 0x5F1551;     // (parts, shared_ptr<RaidEnemy>, level, 0, 0)
constexpr uint32_t ArrayInt4_at = 0x508555;
constexpr uint32_t RankingRewardScrollView_createIncentivePanel = 0x55E295;
constexpr uint32_t RewardRow = 0x5085C9;                    // sub_5085C8
constexpr uint32_t GOT_RAIDPARTS_VTABLE = 0xFD65BC, GOT_FADE_TIME = 0xFD65C0;
}  // namespace g

constexpr uint32_t FRAME = 0x3A8, PARTS = 0x15C;
constexpr uint32_t S_RewardJson = 0xDFD426;     // "cocostudio/publish/Adv_Raid_Boss_List_Reward.json"
constexpr uint32_t S_Txt_Raid_Bonus = 0xDFD467, S_Txt_Wording = 0xDF862F, S_Close_Button = 0xDF8D2F;
constexpr int32_t TXT_GROUP[3] = {103010003, 103010004, 103010005};
constexpr int32_t TXT_ONE_ROW = 103200036, TXT_ROWS = 103200029;
constexpr uint32_t PARTS_REWARD[3] = {0x4C, 0x50, 0x54};    // RaidParts: the reward ids of the three groups
constexpr uint32_t VT_Close = 0xF56024, VT_CanClose = 0xF55FF8, VT_Result = 0xF55FA0;
constexpr uint32_t SM_RAID_POPUP = 0x308;
constexpr uint32_t V_ADD_CHILD = 0xF0, V_GET_POSITION = 0x54;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
uint32_t readU32(uint32_t sb, uint32_t offset, uint32_t out) {
    call(g::SafeBuffer_read, {sb, out, ld32(sb + 0xC) + offset, 4, offset});
    return ld32(out);
}
// row i's value from the reward's int[4] at `offset`
uint32_t rowValue(uint32_t sb, uint32_t offset, uint32_t arr, uint32_t i) {
    call(g::SafeBuffer_read, {sb, arr, ld32(sb + 0xC) + offset, 0x10, offset});
    return ld32(call(g::ArrayInt4_at, {arr, i}));
}

uint32_t popUpRaidBossInfo(uint32_t info, uint32_t done) {
    Frame frame;
    const uint32_t base = frame.allocExact(FRAME);
    const uint32_t parts = base + PARTS;
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {ld32(ld32(g::GOT_FADE_TIME)), opacity})});
    vcall(layer, V_ADD_CHILD, {layer, shadow});
    const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_RewardJson});
    vcall(layer, V_ADD_CHILD, {layer, popup});
    {
        const uint32_t setAnchor = ld32(ld32(popup) + node::setAnchorPoint);
        const uint32_t v = base + 0x188;
        call(g::Vec2_ctor, {v, fbits(0.5f), fbits(0.5f)});
        call(setAnchor, {popup, v});
        call(g::Vec2_dtor, {v});
        const uint32_t setPos = ld32(ld32(popup) + node::setPosition);
        call(setPos, {popup, vcall(shadow, V_GET_POSITION, {shadow})});
    }
    // the boss's parts for the raid's level
    const uint32_t enemy = base + 0x180, copy = base + 0x154;
    call(g::MasterRaidEnemy_create, {enemy, ld32(info + 0x2C)});
    call(g::SafeBuffer_ctor, {parts, 0x60, 1});
    st32(parts, ld32(g::GOT_RAIDPARTS_VTABLE) + 8);
    st32(parts + 0x20, 0);
    st32(copy, ld32(enemy));
    st32(copy + 4, ld32(enemy + 4));
    if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
    const uint32_t got = call(g::RaidParts_setMasterParam, {parts, copy, ld32(info + 8), 0, 0});
    releaseIf(ld32(copy + 4));
    st8(sceneManager() + SM_RAID_POPUP, 1);
    static const uint32_t rew[3] = {0xDFD458, 0xDFD476, 0xDFD485};
    static const uint32_t area[3] = {0xDFD45D, 0xDFD47B, 0xDFD48A};
    const uint32_t rewardSp[3] = {base + 0x14C, base + 0x108, base + 0xC4};
    for (uint32_t k = 0; k < 3; k++) {
        Frame f;
        const uint32_t tmp = f.alloc(4), arr = f.alloc(16);
        const uint32_t reward = rewardSp[k];
        call(g::MasterRaidReward_create, {reward, readU32(parts, PARTS_REWARD[k], tmp)});
        const uint32_t group = findNode(popup, rew[k], 4);
        const uint32_t panel = findNode(popup, area[k], 9);
        const uint32_t sb = ld32(reward);
        call(g::RankingRewardScrollView_createIncentivePanel, {panel, readU32(sb, 0xC, tmp)});
        {
            Frame t;
            const uint32_t s = newStr(t, S_Txt_Raid_Bonus, 14);
            call(g::SceneUtil_settingTitleLabel, {group, s, uint32_t(TXT_GROUP[k])});
            strFree(s);
        }
        uint32_t rows = 0;
        for (uint32_t i = 0; i < 4; i++) {
            const uint32_t count = readU32(sb, 0xC, tmp);
            uint32_t kind = 0, a = 0, b = 0, c = 0, d = 0;
            if (i < count) {
                kind = readU32(sb, 8, tmp);
                a = rowValue(sb, 0x10, arr, i);
                b = rowValue(sb, 0x20, arr, i);
                c = rowValue(sb, 0x60, arr, i);
                if (rowValue(sb, 0x30, arr, i) != 1) d = rowValue(sb, 0x40, arr, i);
                rows++;
            }
            call(g::RewardRow, {group, panel, i, a, d, b, c, kind, ld32(info + 8) - got, i < count ? 1u : 0u});
        }
        {
            Frame t;
            const uint32_t s = newStr(t, S_Txt_Wording, 11);
            call(g::SceneUtil_settingTitleLabel, {group, s, uint32_t(int32_t(rows) < 2 ? TXT_ONE_ROW : TXT_ROWS)});
            strFree(s);
        }
    }
    const uint32_t close = findNode(popup, S_Close_Button, 12);
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_Close, {});
        call(g::Widget_addTouchEventListener, {close, fo});
        functionDestroy(fo);
    }
    // {popup, layer, shadow, done}: a copy kept to the end, one for the popup's result
    Frame f;
    const uint32_t lam = f.alloc(0x20), lam2 = f.alloc(0x20);
    st32(lam, popup);
    st32(lam + 4, layer);
    st32(lam + 8, shadow);
    call(fn::function_vv_copy, {lam + 0x10, done});
    for (uint32_t k = 0; k < 0xC; k += 4) st32(lam2 + k, ld32(lam + k));
    functionCopy(lam2 + 0x10, lam + 0x10);
    {
        const uint32_t sm = sceneManager();
        Frame t;
        const uint32_t check = newFunction(t, VT_CanClose, {close});
        const uint32_t words = t.alloc(0xC), fn3 = t.alloc(20);
        for (uint32_t k = 0; k < 0xC; k += 4) st32(words + k, ld32(lam2 + k));
        functionCopy(fn3, lam2 + 0x10);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x30});
        if (h) {
            st32(h, VT_Result);
            for (uint32_t k = 0; k < 0xC; k += 4) st32(h + 8 + k, ld32(words + k));
            call(fn::function_vv_move, {h + 0x18, fn3});
        }
        st32(result + 16, h);
        call(g::SM_openPopup, {sm, check, result, 1});
        functionDestroy(result);
        functionDestroy(fn3);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t none = newEmptyFunction(t);
        const uint32_t open = call(g::SceneUtil_createOpenPopupAnimation, {popup, none});
        call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {open})});
        functionDestroy(none);
    }
    functionDestroy(lam2 + 0x10);
    functionDestroy(lam + 0x10);
    releaseIf(ld32(rewardSp[2] + 4));
    releaseIf(ld32(rewardSp[1] + 4));
    releaseIf(ld32(rewardSp[0] + 4));
    call(g::SafeBuffer_dtor, {parts});
    releaseIf(ld32(enemy + 4));
    return 0;
}
}  // namespace

RW_FUNCTION(0x5072F4, "popUpRaidBossInfo(const RaidBase::Info&, function<void()>)", popUpRaidBossInfo);

// ------------------------------------------------------------------------------------------------ an incentive row
namespace {
constexpr uint32_t S_TxtIncentiveLabelD = 0xDFD49E;   // "Txt_Incentive_Label%d"
constexpr uint32_t S_ItemPanelD = 0xDFD4B4;           // "ItemPanel%d"
constexpr uint32_t S_Empty = 0xDF8A1A;
constexpr int32_t TXT_INCENTIVE = 103101055;
constexpr uint32_t ITEM_PANEL_STEP = 88;
constexpr uint32_t BattleUtil_calc_many_lux_level = 0x4970FD;   // (level gap, Lux base, count)
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;       // (string& image, string& frame, type, id, bool)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t RankingRewardScrollView_setIncentiveItem = 0x55E4D1;
constexpr uint32_t TextData_format = 0x492929;                  // (string out, type, id, ...)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;

// the node printf(fmt, n) under `root`
uint32_t nodeFmt(uint32_t root, uint32_t fmt, uint32_t n) {
    Frame f;
    const uint32_t s = newStr(f, stringBuffer(f, {fmt, n}));
    const uint32_t node = SceneUtil::searchNodeByName(root, s);
    strFree(s);
    return node;
}
}  // namespace

// sub_5085C8(root, panels, k, type, param, id, count, Lux base, level gap, shown): incentive row k's label shown or
// not; shown: the reward's icon in "ItemPanel<k+1>" (88 apart; Lux rewards scaled by the level gap) and the label's
// text
uint32_t incentiveRow(uint32_t root, uint32_t panels, uint32_t k, uint32_t type, uint32_t param, uint32_t id,
                      uint32_t count, uint32_t luxBase, uint32_t levelGap, uint32_t shown) {
    const uint32_t label = nodeFmt(root, S_TxtIncentiveLabelD, k + 1);
    vcall(label, node::setVisible, {label, shown});
    if (!shown) return 0x8AC;
    const uint32_t n = (type == 4 || type == 11) ? call(BattleUtil_calc_many_lux_level, {levelGap, luxBase, count}) : count;
    Frame f;
    const uint32_t image = f.alloc(12), frame = f.alloc(12), frameCopy = f.alloc(12), imageCopy = f.alloc(12);
    strInit(image, S_Empty, 0);
    strInit(frame, S_Empty, 0);
    call(SceneUtil_setItemImageData, {image, frame, type, id, 0});
    const uint32_t panel = nodeFmt(panels, S_ItemPanelD, k + 1);
    {
        const uint32_t setPos = ld32(ld32(panel) + node::setPosition);
        Frame p;
        const uint32_t pos = p.alloc(8);
        call(Vec2_ctor, {pos, fbits(float(int32_t(ITEM_PANEL_STEP * k))), fbits(0.0f)});
        call(setPos, {panel, pos});
        call(Vec2_dtor, {pos});
    }
    strCopy(frameCopy, frame);
    strCopy(imageCopy, image);
    call(RankingRewardScrollView_setIncentiveItem, {panel, type, param, id, frameCopy, imageCopy, n, k + 6, 0});
    strFree(imageCopy);
    strFree(frameCopy);
    call(SceneUtil_setShadow, {label});
    {
        Frame t;
        const uint32_t text = t.alloc(12);
        call(TextData_format, {text, 0, uint32_t(TXT_INCENTIVE)});
        setTextC(label, strIsLong(text) ? ld32(text + 8) : text + 1);
        strFree(text);
    }
    strFree(frame);
    strFree(image);
    return 0x8AC;
}
RW_FUNCTION(0x5085C8, "RaidBossScrollView incentive row (sub_5085C8)", incentiveRow);
