// RaidBossResultPopUp: the result popup of a finished raid (rewards for the attack, the MVP and the destruction).
#include "BattleUi.hpp"

namespace {
using scene::strCopy;
using scene::Frame;
using scene::call;
using scene::ld32;
using scene::st32;
using scene::vcall;

constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;
constexpr uint32_t MasterRaidEnemy_create = 0x787745;
constexpr uint32_t MasterRaidReward_create = 0x789DF9;
constexpr uint32_t SafeBuffer_ctor = 0x459F21;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t RaidParts_setMasterParam = 0x5F1551;
constexpr uint32_t shared_add = 0xC8DBB5;
constexpr uint32_t shared_release = 0xC8DBD1;
constexpr uint32_t array4_at = 0x508555;              // std::array<int, 4>::at(i) -> int*
constexpr uint32_t BattleUtil_calc_many_lux_level = 0x4970FD;
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;   // (string&, string&, type, id, bool)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t Size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_;
constexpr uint32_t RankingRewardScrollView_setIncentiveItem = 0x55E4D1;
constexpr uint32_t GOT_RAIDPARTS_VTABLE = 0xFD65BC;

constexpr uint32_t S_REW = 0xE00B8C;              // "Rew%d"
constexpr uint32_t S_PANEL_BONUS = 0xE00B92;      // "Panel_Bonus%d"
constexpr uint32_t S_MVP_BONUS_MEDAL = 0xE00BA0;  // "Raid_Boss_Mvp_Bonus_Medal%d"
constexpr uint32_t S_TXT_RAID_BONUS = 0xDFD467;   // "Txt_Raid_Bonus" (14)
constexpr uint32_t S_TXT_INCENTIVE_NUM = 0xDFF0A6;   // "Txt_Incentive_Num" (17)
constexpr uint32_t S_TIMES_D = 0xDFD670;          // "×%d"
constexpr uint32_t S_ICON_PANEL_JSON = 0xDFEC2D;  // "cocostudio/publish/Ranking_IconPanel.json"
constexpr uint32_t S_EMPTY = 0xDF8A1A;

constexpr uint32_t API_RAID_RESULTS = 360;        // APIManager: std::vector<RaidResult>* (176 bytes each)
constexpr uint32_t API_USER = 116;                // APIManager: SafeBuffer* user data (+0 user id)
constexpr uint32_t API_RAID_START = 368;          // APIManager: the raid start data (+16 MVP, +24 MVP 2 user ids)
constexpr uint32_t RESULT_SIZE = 176;
constexpr uint32_t RESULT_LEVEL = 8, RESULT_RANK = 12, RESULT_MVP_RANK = 16, RESULT_DESTROY_RANK = 20;
constexpr uint32_t RESULT_ENEMY = 28;
constexpr uint32_t PARTS_REWARD_ID = 0x4C, PARTS_MVP_REWARD_ID = 0x50, PARTS_DESTROY_REWARD_ID = 0x54;
constexpr uint32_t SCENEMANAGER_POPUP_FLAG = 0x308;
constexpr int32_t TXT_REWARD = 103010003, TXT_MVP_REWARD = 103010004, TXT_DESTROY_REWARD = 103010005;
// master::RaidReward
constexpr uint32_t RR_LUX_BASE = 8, RR_TYPE = 16, RR_ID = 32, RR_PARAM = 64, RR_COUNT = 96;

constexpr uint32_t V_SET_SCALE = 0x40;
constexpr uint32_t V_GET_Z = 0x1C;
constexpr uint32_t V_GET_CONTENT_SIZE = 0xAC;
constexpr uint32_t V_SET_VISIBLE = 0xB0;
constexpr uint32_t V_ADD_CHILD_Z = 0xF4;

uint32_t sbGet(uint32_t sb, uint32_t index) { return uint32_t(rw::host_ptr<SafeBuffer>(sb)->get32(index)); }
// the node named printf(fmt, k) under `root`
uint32_t findFmt(uint32_t root, uint32_t fmt, uint32_t k) {
    Frame f;
    uint32_t name = scene::stringBuffer(f, {fmt, k});
    uint32_t s = scene::newStr(f, name);
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    scene::strFree(s);
    return node;
}
void setVisible(uint32_t node, uint32_t v) { vcall(node, V_SET_VISIBLE, {node, v}); }
// one int of a RaidReward array (std::array<int, 4>::at, bounds-checked like the original)
uint32_t rewardAt(uint32_t row, uint32_t index, uint32_t k) {
    Frame f;
    uint32_t arr = f.alloc(16);
    const SafeBuffer *s = rw::host_ptr<SafeBuffer>(row);
    SafeBuffer::read(s, rw::host_ptr<uint8_t>(arr), s->data.get() + index, 16, index);
    return ld32(call(array4_at, {arr, k}));
}
}  // namespace

// setResultItem's lambda (sub_5B730C) [&widget](text, k, rewardId, index, levelGap, shown): reward row k ("Rew%d",
// "Panel_Bonus%d", "Raid_Boss_Mvp_Bonus_Medal%d") shown or not; when shown, its title, the reward's count (Lux
// rewards scaled by the level gap) and the reward icon in Panel_Bonus.
void raidRewardRow(uint32_t widgetRef, int32_t textId, uint32_t k, uint32_t rewardId, uint32_t index, uint32_t levelGap,
                   uint32_t shown) {
    uint32_t rew = findFmt(ld32(widgetRef), S_REW, k);
    if (rew) setVisible(rew, shown);
    if (uint32_t panel = findFmt(ld32(widgetRef), S_PANEL_BONUS, k)) setVisible(panel, shown);
    if (uint32_t medal = findFmt(ld32(widgetRef), S_MVP_BONUS_MEDAL, k)) setVisible(medal, shown);
    if (shown != 1) return;
    Frame f;
    uint32_t reward = f.alloc(8), image = f.alloc(12), frame = f.alloc(12), copy2 = f.alloc(12), copy1 = f.alloc(12);
    call(MasterRaidReward_create, {reward, rewardId});
    uint32_t luxBase = sbGet(ld32(reward), RR_LUX_BASE);
    uint32_t type = rewardAt(ld32(reward), RR_TYPE, index);
    uint32_t id = rewardAt(ld32(reward), RR_ID, index);
    uint32_t count = rewardAt(ld32(reward), RR_COUNT, index);
    uint32_t param = rewardAt(ld32(reward), RR_PARAM, index);
    if (type == 4 || type == 11) count = call(BattleUtil_calc_many_lux_level, {levelGap, luxBase, count});
    scene::strInit(image, S_EMPTY, 0);
    scene::strInit(frame, S_EMPTY, 0);
    call(SceneUtil_setItemImageData, {image, frame, type, id, 0});
    uint32_t title = scene::findNode(rew, S_TXT_RAID_BONUS, 14);
    call(SceneUtil_setShadow, {title});
    scene::setTextId(title, textId);
    uint32_t num = scene::findNode(rew, S_TXT_INCENTIVE_NUM, 17);
    call(SceneUtil_setShadow, {num});
    {
        Frame g;
        scene::setTextC(num, scene::stringBuffer(g, {S_TIMES_D, count}));
    }
    setVisible(findFmt(ld32(widgetRef), S_MVP_BONUS_MEDAL, k), 0);
    uint32_t panel = findFmt(ld32(widgetRef), S_PANEL_BONUS, k);
    uint32_t icon = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_ICON_PANEL_JSON});
    {
        Frame g;
        uint32_t size = g.alloc(8);
        call(Size_copy, {size, vcall(panel, V_GET_CONTENT_SIZE, {panel})});
    }
    vcall(icon, V_SET_SCALE, {icon, rw::fbits(1.0f)});
    uint32_t add = ld32(ld32(panel) + V_ADD_CHILD_Z);
    call(add, {panel, icon, vcall(panel, V_GET_Z, {panel})});
    strCopy(copy2, frame);
    strCopy(copy1, image);
    call(RankingRewardScrollView_setIncentiveItem, {icon, type, param, id, copy2, copy1, count, 2, 0});
    scene::strFree(copy1);
    scene::strFree(copy2);
    scene::strFree(frame);
    scene::strFree(image);
    if (ld32(reward + 4)) call(shared_release, {ld32(reward + 4)});
}
RW_FUNCTION(0x5B730C, "RaidBossResultPopUp::setResultItem lambda (sub_5B730C)", raidRewardRow);

// The rewards of raid result `index`: attack (always), MVP (when the player was MVP or MVP 2) and destruction (when
// the boss was destroyed). The RaidParts local (the boss's reward ids per level) is a SafeBuffer registered for
// updates: the original frame (push 0x14 + sub 0x84) keeps it, and the lambda's capture, at the original addresses.
namespace RaidBossResultPopUp {
void setResultItem(uint32_t widget, uint32_t index) {
    Frame f;
    uint32_t base = f.allocExact(0x98);
    uint32_t parts = base + 0x38, enemyRow = base + 0x5C, copy = base + 0x30, widgetRef = base + 0x64, user = base + 0x68;
    st32(widgetRef, widget);
    uint32_t results = ld32(call(APIManager_getInstance, {}) + API_RAID_RESULTS);
    if ((ld32(results + 4) - ld32(results)) / RESULT_SIZE <= index) call(THROW_OUT_OF_RANGE, {});
    uint32_t r = ld32(results) + RESULT_SIZE * index;
    call(MasterRaidEnemy_create, {enemyRow, ld32(r + RESULT_ENEMY)});
    call(SafeBuffer_ctor, {parts, 0x60, 1});
    st32(parts, ld32(GOT_RAIDPARTS_VTABLE) + 8);
    st32(parts + 32, 0);
    st32(copy, ld32(enemyRow));
    st32(copy + 4, ld32(enemyRow + 4));
    if (ld32(copy + 4)) call(shared_add, {ld32(copy + 4)});
    uint32_t got = uint32_t(rw::call_exact(RaidParts_setMasterParam, {parts, copy, ld32(r + RESULT_LEVEL), 0, 0}));
    if (ld32(copy + 4)) call(shared_release, {ld32(copy + 4)});
    uint32_t levelGap = ld32(r + RESULT_LEVEL) - got;
    uint32_t api = call(APIManager_getInstance, {});
    const SafeBuffer *u = rw::host_ptr<SafeBuffer>(ld32(api + API_USER));
    SafeBuffer::read(u, rw::host_ptr<uint8_t>(user), u->data.get(), 8, 0);
    uint32_t idLo = ld32(user), idHi = ld32(user + 4);
    uint32_t start = ld32(call(APIManager_getInstance, {}) + API_RAID_START);
    uint32_t mvp = ((ld32(start + 24) == idLo && ld32(start + 28) == idHi) ? 1u : 0u) |
                   ((ld32(start + 16) == idLo && ld32(start + 20) == idHi) ? 1u : 0u);
    uint32_t rank = ld32(r + RESULT_RANK) - 1, mvpRank = ld32(r + RESULT_MVP_RANK) - 1;
    uint32_t destroyRank = ld32(r + RESULT_DESTROY_RANK) - 1;
    uint32_t reward = sbGet(parts, PARTS_REWARD_ID);
    uint32_t mvpReward = sbGet(parts, PARTS_MVP_REWARD_ID);
    uint32_t destroyReward = sbGet(parts, PARTS_DESTROY_REWARD_ID);
    uint32_t destroyed = ld32(r + RESULT_DESTROY_RANK);
    scene::st8(call(SceneManager_getInstance, {}) + SCENEMANAGER_POPUP_FLAG, 1);
    rw::call_exact(0x5B730D, {widgetRef, uint32_t(TXT_REWARD), 1, reward, rank, levelGap, 1});
    rw::call_exact(0x5B730D, {widgetRef, uint32_t(TXT_MVP_REWARD), 2, mvpReward, mvpRank, levelGap, mvp});
    rw::call_exact(0x5B730D, {widgetRef, uint32_t(TXT_DESTROY_REWARD), 3, destroyReward, destroyRank, levelGap,
                              destroyed ? 1u : 0u});
    call(SafeBuffer_dtor, {parts});
    if (ld32(enemyRow + 4)) call(shared_release, {ld32(enemyRow + 4)});
}
}  // namespace RaidBossResultPopUp
RW_FUNCTION(0x5B70D4, "RaidBossResultPopUp::setResultItem(Widget*, int)", RaidBossResultPopUp::setResultItem);

// ------------------------------------------------------------------------------------------------ the result popup
namespace {
namespace rr {
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69;
constexpr uint32_t SceneManager_openPopup = 0x61524D;    // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;   // (api, const function<void(bool)>&, bool)
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;   // (node, float, float, function<void()>)
constexpr uint32_t SceneUtil_setBossTypeImage = 0x46C4C5;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t RaidSubdueScrollView_create = 0x5B9609;
constexpr uint32_t RaidSubdueScrollView_reflectionList = 0x5B58B5;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t Function_copy = 0x4683CD;             // std::function<void()>(const function&)
constexpr uint32_t Function_move = 0x47F905;             // std::function<void()>(function&&)
constexpr uint32_t VectorInt_copy = 0x4AF707;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t VectorCooperator_copy = 0x5B9AF5;
constexpr uint32_t VectorBaseCooperator_dtor = 0x4B69DB;
constexpr uint32_t VectorBaseSubdue_dtor = 0x5B965D;
constexpr uint32_t VectorSubdue_pushSlow = 0x5B9869;
constexpr uint32_t pushRaidBossResult = 0x5B6491;        // (SubdueData out, id, int, int, PanelType, string, const Coordinate*, bool)
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ImageButton_create = 0x531239;        // (string, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1; // (button, node, float)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t setResultItem = 0x5B70D5;
constexpr uint32_t GOT_POPUP_FADE_TIME = 0xFD65C0;       // &(float) the popups' shadow fade time
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
}  // namespace rr

// lambda vtables (__func + 8)
constexpr uint32_t VT_ERROR_CLOSED = 0xF5C928;   // {}
constexpr uint32_t VT_OK_TOUCH = 0xF5C8FC;       // {okButton}
constexpr uint32_t VT_POPUP_OPEN = 0xF5C8D0;     // function<bool()> {okButton}
constexpr uint32_t VT_POPUP_CLOSED = 0xF5C878;   // function<void(bool)> on the heap (0x40 bytes)
constexpr uint32_t VT_LEFT_IN_DONE = 0xF5C84C;   // {widget}
constexpr uint32_t VT_ARROW_LEFT = 0xF5C820;     // {scroll view}
constexpr uint32_t VT_ARROW_RIGHT = 0xF5C7F4;    // {scroll view}

// strings
constexpr uint32_t S_RESULT_JSON = 0xE00AC5;     // "cocostudio/publish/Adv_Raid_Boss_Dialog_Result_ver104.json"
constexpr uint32_t S_BOSS_TYPE = 0xDFD326;       // "Raid_Boss_Type" (14)
constexpr uint32_t S_NAME_LABEL = 0xDFD335;      // "Raid_Boss_Name_Label" (20)
constexpr uint32_t S_LV_TXT = 0xDFD34A;          // "Raid_Boss_Lv_Txt" (16)
constexpr uint32_t S_LVFIX = 0xDFD35B;           // "Raid_Boss_LvFix_Label" (21)
constexpr uint32_t S_FMT_D = 0xDF86DE;           // "%d"
constexpr uint32_t S_BUTTON_OK = 0xDF8688;       // "Button_OK" (9)
constexpr uint32_t S_RAID_BOSS_JP = 0xE00B00;    // "Raid Boss" in Japanese (15 bytes)
constexpr uint32_t S_ARROW_JSON = 0xE00B10;      // "cocostudio/publish/ArrowAnim.ExportJson" (39)
constexpr uint32_t S_ARROW_LEFT = 0xE00B38;      // "Arrow_Left" (10)
constexpr uint32_t S_ARROW_ANIM = 0xE00B43;      // "ArrowAnim" (9)
constexpr uint32_t S_ARROW_RIGHT = 0xE00B4D;     // "Arrow_Right" (11)
constexpr int32_t TXT_LV = 100200002, TXT_OK = 100100001;

// the request lambda's captures
constexpr uint32_t CB_ENEMY = 8, CB_LEVEL = 12, CB_RESULT = 16, CB_IDS = 20, CB_DONE = 32, CB_FLAG = 56;
constexpr uint32_t API_RAID_MEMBERS = 368;       // APIManager: RaidMember* (+0 cooperators, +16 finder, +24 MVP ids)
constexpr uint32_t COOP_SIZE = 80, COOP_ID = 0, COOP_NAME = 8, COOP_LEVEL = 0x14, COOP_AVATAR = 0x18, COOP_DAMAGE = 0x48;
constexpr uint32_t SUBDUE_SIZE = 80, SUBDUE_NAME = 0x10;
constexpr uint32_t API_ERROR_SHOWN = 0x14;
constexpr uint32_t SV_LAYOUT_PANEL = 0x50C, SV_ITEM_WIDTH = 0x514, SV_OFFSET_X = 0x570;

// node virtual slots
constexpr uint32_t V_SET_ANCHOR = 0x9C, V_SET_CONTENT_SIZE = 0xA8, V_GET_CHILDREN = 0x10C, V_GET_OPACITY = 0x20C;
constexpr uint32_t V_SET_OPACITY = 0x214, V_ADD_CHILD = 0xF0, V_TOUCH_ENABLED = 0x29C, V_GET_ANIMATION = 0x298;
constexpr uint32_t V_ANIM_PLAY = 0x68, V_SET_POSITION = 0x4C, V_GET_POSITION = 0x54;
constexpr uint32_t V_SET_X = 0x64, V_GET_X = 0x68, V_SET_Y = 0x6C, V_GET_Y = 0x70, V_SV_1C4 = 0x1C4;

uint32_t fbf(float f) { return rw::fbits(f); }
uint32_t coopCount(uint32_t vec) { return (ld32(vec + 4) - ld32(vec)) / COOP_SIZE; }
void throwOutOfRange() { call(THROW_OUT_OF_RANGE, {}); }
// a card: pushRaidBossResult into a temporary, appended to the card list (moved in place while it has room)
void pushCard(uint32_t cards, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t type, uint32_t name, uint32_t avatar,
              uint32_t flag) {
    Frame f;
    uint32_t card = f.alloc(SUBDUE_SIZE);
    call(rr::pushRaidBossResult, {card, a0, a1, a2, type, name, avatar, flag});
    uint32_t end = ld32(cards + 4);
    if (end < ld32(cards + 8)) {
        uint32_t base = 0;
        if (end) {
            for (uint32_t k = 0; k < SUBDUE_SIZE; k += 4) st32(end + k, ld32(card + k));
            for (uint32_t k = 0; k < 12; k += 4) st32(card + SUBDUE_NAME + k, 0);
            base = ld32(cards + 4);
        }
        st32(cards + 4, base + SUBDUE_SIZE);
    } else {
        call(rr::VectorSubdue_pushSlow, {cards, card});
    }
    scene::strFree(card + SUBDUE_NAME);
}
// the cooperator `k`'s card of `type` (its name copied for the by-value argument)
void coopCard(uint32_t cards, uint32_t coop, uint32_t k, uint32_t enemy, uint32_t type) {
    if (coopCount(coop) <= k) throwOutOfRange();
    uint32_t e = ld32(coop) + COOP_SIZE * k;
    uint32_t level = ld32(e + COOP_LEVEL), damage = ld32(e + COOP_DAMAGE);
    Frame f;
    uint32_t name = f.alloc(12);
    strCopy(name, e + COOP_NAME);
    if (coopCount(coop) <= k) throwOutOfRange();
    e = ld32(coop) + COOP_SIZE * k;
    pushCard(cards, enemy, level, damage, type, name, e + COOP_AVATAR, 1);
    scene::strFree(name);
}
// the index of the cooperator with user id (lo, hi), 0 when none
uint32_t findCoop(uint32_t coop, uint32_t n, uint32_t lo, uint32_t hi) {
    for (uint32_t k = 0; int32_t(k) < int32_t(n); k++) {
        if (coopCount(coop) <= k) throwOutOfRange();
        uint32_t e = ld32(coop) + COOP_SIZE * k;
        if (ld32(e + COOP_ID) == lo && ld32(e + COOP_ID + 4) == hi) return k;
    }
    return 0;
}
// an arrow: the ArrowAnim armature playing `anim`, 50 pixels up and right of where it was, added to the arrow
void arrowArmature(uint32_t node, uint32_t anim, uint32_t animLen, uint32_t pos, bool first) {
    uint32_t arm;
    {
        Frame f;
        uint32_t name = scene::newStr(f, S_ARROW_ANIM, 9);
        arm = call(rr::Armature_create, {name});
        scene::strFree(name);
    }
    uint32_t a = vcall(arm, V_GET_ANIMATION, {arm});
    uint32_t play = ld32(ld32(a) + V_ANIM_PLAY);
    {
        Frame f;
        uint32_t s = scene::newStr(f, anim, animLen);
        call(play, {a, s, 0xFFFFFFFFu, 0xFFFFFFFFu});
        scene::strFree(s);
    }
    uint32_t p = vcall(arm, V_GET_POSITION, {arm});
    if (first) {
        call(rr::Vec2_copy, {pos, p});
    } else {
        st32(pos, ld32(p));
        st32(pos + 4, ld32(p + 4));
    }
    st32(pos, fbf(scene::ldf(pos) + 50.0f));
    st32(pos + 4, fbf(scene::ldf(pos + 4) + 50.0f));
    vcall(arm, V_SET_POSITION, {arm, pos});
    vcall(node, V_ADD_CHILD, {node, arm});
}
// a 100 x 100 node centred on its anchor
void squareArrow(uint32_t node) {
    uint32_t setSize = ld32(ld32(node) + V_SET_CONTENT_SIZE);
    Frame f;
    uint32_t size = f.alloc(8);
    call(rr::Size_ctor, {size, fbf(100.0f), fbf(100.0f)});
    call(setSize, {node, size});
    vcall(node, V_SET_ANCHOR, {node, ld32(rr::GOT_ANCHOR_MIDDLE)});
}
// an invisible button (an empty ImageButton) for an arrow
uint32_t emptyButton() {
    Frame f;
    uint32_t empty = f.alloc(12);
    for (uint32_t k = 0; k < 12; k += 4) st32(empty + k, 0);
    uint32_t button = call(rr::ImageButton_create, {empty, 1});
    scene::strFree(empty);
    return button;
}
// the button over an arrow: added to the arrow's first child, scrolling the card strip
void armArrowButton(uint32_t button, uint32_t arrow, uint32_t first, uint32_t vtable, uint32_t sv) {
    call(rr::ImageButton_setTargetView, {button, arrow, fbf(1.0f)});
    vcall(first, V_ADD_CHILD, {first, button});
    Frame f;
    uint32_t fo = scene::newFunction(f, vtable, {sv});
    call(rr::ImageButton_addTouchEventListener, {button, fo});
    scene::functionDestroy(fo);
}
}  // namespace

// popUpRaidBossResult's request lambda (sub_5B7DBC): when the raid result arrived, the result popup - the boss
// (type, name, level), the card strip (the "raid boss" card, then the MVP - or the MVP who also found it - and the
// finder, then the other cooperators; arrows when there are four cards or more), the rewards and the OK button -
// opened as a popup; on failure the error dialog.
void raidResultLoaded(uint32_t functor, uint32_t okRef) {
    uint8_t ok = scene::ld8(okRef);
    call(rr::SceneManager_enableLoadingIndicator, {call(SceneManager_getInstance, {}), 0});
    if (!ok) {
        if (!scene::ld8(call(APIManager_getInstance, {}) + API_ERROR_SHOWN)) return;
        uint32_t api = call(APIManager_getInstance, {});
        Frame f;
        uint32_t fo = scene::newFunction(f, VT_ERROR_CLOSED, {});
        call(rr::APIManager_openErrorDialog, {api, fo, 1});
        scene::functionDestroy(fo);
        return;
    }
    call(rr::AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(rr::SceneManager_getPopupLayer, {call(SceneManager_getInstance, {})});
    uint32_t shadow = call(rr::SceneUtil_createShadowSprite, {});
    uint32_t opacity = vcall(shadow, V_GET_OPACITY, {shadow});
    vcall(shadow, V_SET_OPACITY, {shadow, 0});
    call(rr::Node_runAction, {shadow, call(rr::FadeTo_create, {ld32(ld32(rr::GOT_POPUP_FADE_TIME)), opacity})});
    vcall(layer, V_ADD_CHILD, {layer, shadow});
    uint32_t widget = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_RESULT_JSON});
    Frame frame;
    uint32_t enemy = frame.alloc(8);
    call(MasterRaidEnemy_create, {enemy, ld32(functor + CB_ENEMY)});
    if (uint32_t type = scene::findNode(widget, S_BOSS_TYPE, 14)) call(rr::SceneUtil_setBossTypeImage, {type, ld32(functor + CB_ENEMY), 1});
    if (uint32_t name = scene::findNode(widget, S_NAME_LABEL, 20)) {
        call(SceneUtil_setShadow, {name});
        Frame f;
        uint32_t raw = f.alloc(0x88);
        const SafeBuffer *row = rw::host_ptr<SafeBuffer>(ld32(enemy));
        SafeBuffer::read(row, rw::host_ptr<uint8_t>(raw), row->data.get() + 4, 0x81, 4);
        scene::setTextC(name, raw);
    }
    if (uint32_t lv = scene::findNode(widget, S_LV_TXT, 16)) {
        call(SceneUtil_setShadow, {lv});
        scene::setTextId(lv, TXT_LV);
    }
    if (uint32_t lvFix = scene::findNode(widget, S_LVFIX, 21)) {
        call(SceneUtil_setShadow, {lvFix});
        Frame f;
        scene::setTextC(lvFix, scene::stringBuffer(f, {S_FMT_D, ld32(functor + CB_LEVEL)}));
    }
    uint32_t area;
    {
        Frame f;
        uint32_t s = f.alloc(12);
        st32(s, 8);                                      // "Area" (short string)
        st32(s + 4, 0);
        st32(s + 8, 0);
        rw::save<uint8_t>(s + 1, 'A');
        rw::save<uint8_t>(s + 2, 'r');
        rw::save<uint8_t>(s + 3, 'e');
        rw::save<uint8_t>(s + 4, 'a');
        area = SceneUtil::searchNodeByName(widget, s);
    }
    vcall(area, V_TOUCH_ENABLED, {area, 0});
    uint32_t sv = call(rr::RaidSubdueScrollView_create, {});
    {
        Frame f;
        uint32_t size = f.alloc(8);
        call(Size_copy, {size, vcall(area, V_GET_CONTENT_SIZE, {area})});
        vcall(sv, V_SET_CONTENT_SIZE, {sv, size});
    }
    uint32_t okButton = scene::findNode(widget, S_BUTTON_OK, 9);
    {
        Frame f;
        uint32_t s = f.alloc(12);
        st32(s, 0);
        st32(s + 4, 0);
        st32(s + 8, 0);
        const char *txt = "Txt_OK";
        rw::save<uint8_t>(s, 12);                        // "Txt_OK" (short string)
        for (uint32_t k = 0; k < 6; k++) rw::save<uint8_t>(s + 1 + k, uint8_t(txt[k]));
        SceneUtil::settingTitleLabel(okButton, s, TXT_OK);
    }
    vcall(okButton, V_TOUCH_ENABLED, {okButton, 1});
    {
        Frame f;
        uint32_t fo = scene::newFunction(f, VT_OK_TOUCH, {okButton});
        call(rr::Widget_addTouchEventListener, {okButton, fo});
        scene::functionDestroy(fo);
    }
    // what the popup's close handler keeps: the button, the result, the ids, the widget, the layer, the shadow and
    // the done callback
    uint32_t keep = frame.alloc(0x38);
    st32(keep, okButton);
    st32(keep + 4, ld32(functor + CB_RESULT));
    call(rr::VectorInt_copy, {keep + 8, functor + CB_IDS});
    st32(keep + 0x14, widget);
    st32(keep + 0x18, layer);
    st32(keep + 0x1C, shadow);
    call(rr::Function_copy, {keep + 0x20, functor + CB_DONE});
    {
        uint32_t sm = call(SceneManager_getInstance, {});
        Frame f;
        uint32_t openFn = scene::newFunction(f, VT_POPUP_OPEN, {okButton});
        uint32_t tmp = f.alloc(0x38), closeFn = f.alloc(24);
        st32(tmp, ld32(keep));
        st32(tmp + 4, ld32(keep + 4));
        call(rr::VectorInt_copy, {tmp + 8, keep + 8});
        for (uint32_t k = 0x14; k < 0x20; k += 4) st32(tmp + k, ld32(keep + k));
        call(rr::Function_copy, {tmp + 0x20, keep + 0x20});
        st32(closeFn + 16, 0);
        uint32_t heap = call(scene::fn::operator_new, {0x40});
        if (heap) {
            st32(heap, VT_POPUP_CLOSED);
            st32(heap + 8, ld32(tmp));
            st32(heap + 12, ld32(tmp + 4));
            call(rr::VectorInt_copy, {heap + 0x10, tmp + 8});
            for (uint32_t k = 0; k < 12; k += 4) st32(heap + 0x1C + k, ld32(tmp + 0x14 + k));
            call(rr::Function_move, {heap + 0x28, tmp + 0x20});
            for (uint32_t k = 0; k < 12; k += 4) st32(heap + 0x1C + k, ld32(tmp + 0x14 + k));
        }
        st32(closeFn + 16, heap);
        call(rr::SceneManager_openPopup, {sm, openFn, closeFn, 0});
        scene::functionDestroy(closeFn);
        scene::functionDestroy(tmp + 0x20);
        call(rr::VectorBaseInt_dtor, {tmp + 8});
        scene::functionDestroy(openFn);
    }
    {
        uint32_t setX = ld32(ld32(widget) + V_SET_X);
        float x = rw::bitsf(vcall(widget, V_GET_X, {widget}));
        call(setX, {widget, fbf(x + rw::bitsf(call(rr::SceneUtil_getSideMargin, {})))});
        uint32_t setY = ld32(ld32(widget) + V_SET_Y);
        float y = rw::bitsf(vcall(widget, V_GET_Y, {widget}));
        call(setY, {widget, fbf(y + rw::bitsf(call(rr::SceneUtil_getSideHeightMargin, {})))});
    }
    {
        Frame f;
        uint32_t fo = scene::newFunction(f, VT_LEFT_IN_DONE, {widget});
        uint32_t anim = call(rr::SceneUtil_createLeftInAnimation, {widget, 0, 0, fo});
        call(rr::Node_runAction, {widget, call(rr::EaseSineOut_create, {anim})});
        scene::functionDestroy(fo);
    }

    // the cards
    uint32_t cards = frame.alloc(12), coop = frame.alloc(12);
    st32(cards, 0);
    st32(cards + 4, 0);
    st32(cards + 8, 0);
    uint32_t membersAt = call(APIManager_getInstance, {}) + API_RAID_MEMBERS;
    call(rr::VectorCooperator_copy, {coop, ld32(membersAt)});
    int32_t bytes = int32_t(ld32(coop + 4) - ld32(coop));
    uint32_t n = uint32_t(bytes >> 4) * 0xCCCCCCCDu;
    uint32_t enemyId = ld32(functor + CB_ENEMY), level = ld32(functor + CB_LEVEL);
    uint32_t members = ld32(membersAt);
    uint32_t finderLo = ld32(members + 16), finderHi = ld32(members + 20);
    uint32_t mvpLo = ld32(members + 24), mvpHi = ld32(members + 28);
    {
        Frame f;
        uint32_t title = scene::newStr(f, S_RAID_BOSS_JP, 15);
        pushCard(cards, enemyId, level, 0, 4, title, 0, scene::ld8(functor + CB_FLAG));
        scene::strFree(title);
    }
    uint32_t mvp = bytes >= 1 ? findCoop(coop, n, mvpLo, mvpHi) : 0;
    uint32_t finder = mvp;
    if (finderLo == mvpLo && finderHi == mvpHi) {
        coopCard(cards, coop, mvp, ld32(functor + CB_ENEMY), 3);   // the MVP found it too
    } else {
        coopCard(cards, coop, mvp, ld32(functor + CB_ENEMY), 2);
        finder = bytes >= 1 ? findCoop(coop, n, finderLo, finderHi) : 0;
        coopCard(cards, coop, finder, ld32(functor + CB_ENEMY), 1);
    }
    if (bytes >= 1) {
        for (uint32_t k = 0; int32_t(k) < int32_t(n); k++) {
            if (k == finder || k == mvp) continue;
            coopCard(cards, coop, k, ld32(functor + CB_ENEMY), 8);
        }
    }

    uint32_t cardBytes = ld32(cards + 4) - ld32(cards);
    if (uint32_t(int32_t(cardBytes) >> 4) * 0xCCCCCCCDu < 4) {
        // up to three cards: a half (two) or a third of the view each
        uint32_t getSize = ld32(ld32(sv) + V_GET_CONTENT_SIZE);
        uint32_t svSize = call(getSize, {sv});
        uint32_t panel = ld32(sv + SV_LAYOUT_PANEL);
        uint32_t panelGet = ld32(ld32(panel) + V_GET_CONTENT_SIZE);
        float w = scene::ldf(svSize);
        float pw = scene::ldf(call(panelGet, {panel}));
        rw::save<float>(sv + SV_ITEM_WIDTH, cardBytes == 160 ? w * 0.5f : w / 3.0f);
        rw::save<float>(sv + SV_OFFSET_X, pw * 0.5f);
    } else {
        call(rr::IScrollProtocol_setEnabledScroll, {sv, 1});
        {
            uint32_t mgr = call(rr::ArmatureDataManager_getInstance, {});
            Frame f;
            uint32_t s = scene::newStr(f, S_ARROW_JSON, 39);
            call(rr::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
            scene::strFree(s);
        }
        Frame f;
        uint32_t pos = f.alloc(8);
        uint32_t left = scene::findNode(widget, S_ARROW_LEFT, 10);
        uint32_t leftText = f.alloc(8);
        st32(leftText, 0x7466654C);                      // "Left"
        rw::save<uint8_t>(leftText + 4, 0);
        arrowArmature(left, leftText, 4, pos, true);
        uint32_t right = scene::findNode(widget, S_ARROW_RIGHT, 11);
        uint32_t rightText = f.alloc(8);
        st32(rightText, 0x68676952);                     // "Righ" "t"
        rw::save<uint8_t>(rightText + 4, 't');
        rw::save<uint8_t>(rightText + 5, 0);
        arrowArmature(right, rightText, 5, pos, false);
        uint32_t leftFirst = ld32(ld32(vcall(left, V_GET_CHILDREN, {left})));
        uint32_t leftButton = emptyButton();
        squareArrow(left);
        squareArrow(right);
        armArrowButton(leftButton, left, leftFirst, VT_ARROW_LEFT, sv);
        uint32_t rightFirst = ld32(ld32(vcall(right, V_GET_CHILDREN, {right})));
        uint32_t rightButton = emptyButton();
        armArrowButton(rightButton, right, rightFirst, VT_ARROW_RIGHT, sv);
        call(rr::Vec2_dtor, {pos});
    }
    call(rr::RaidSubdueScrollView_reflectionList, {sv, cards});
    vcall(area, V_ADD_CHILD, {area, sv});
    call(rr::setResultItem, {widget, ld32(functor + CB_RESULT)});
    vcall(layer, V_ADD_CHILD, {layer, widget});
    vcall(sv, V_SV_1C4, {sv, 0});
    if (uint32_t(int32_t(ld32(cards + 4) - ld32(cards)) >> 4) * 0xCCCCCCCDu <= 3) call(rr::IScrollProtocol_setEnabledScroll, {sv, 0});
    call(rr::VectorBaseCooperator_dtor, {coop});
    call(rr::VectorBaseSubdue_dtor, {cards});
    scene::functionDestroy(keep + 0x20);
    call(rr::VectorBaseInt_dtor, {keep + 8});
    if (ld32(enemy + 4)) call(shared_release, {ld32(enemy + 4)});
}
RW_FUNCTION(0x5B7DBC, "RaidBossResultPopUp::popUpRaidBossResult request lambda operator() (sub_5B7DBC)", raidResultLoaded);

// ------------------------------------------------------------------------------------------------ the popup's lambdas
namespace {
using scene::ld8;
using scene::st8;

constexpr uint32_t VT_SLID_OUT = 0xF5C8A4;       // sub_5BA100's slide-out end {layer, shadow, widget}
constexpr uint32_t S_AREA_POINT = 0xDF9FB4;      // "Area_Point"
constexpr uint32_t S_RAID_RESULT_LWF = 0xE00B5F; // "lwf/battleresult/Raid_Result/Raid_Result.lwf"
constexpr uint32_t SV_MOVING = 0x640, SV_MOVE_LEFT = 0x644, SV_MIGRATING = 0x645;   // RaidSubdueScrollView
constexpr uint32_t IScrollProtocol_migrationLeft = 0x4F81B9, IScrollProtocol_migrationRight = 0x4F81D3;
constexpr uint32_t SceneUtil_createRightOutAnimation = 0x45BA3D;   // (node, float, float, function<void()>)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t popUpRaidBossResult = 0x5B6DA9;   // (index, const vector<int>& ids, function<void()> by value)
constexpr uint32_t slideOut = 0x5BA101;             // sub_5BA100 below
constexpr uint32_t TOUCH_BEGAN = 0, TOUCH_ENDED = 2, TOUCH_CANCELED = 3;

// the list arrows {scroll view}(sender, type): pressed -> the list moves one page (the decide sound); released
// -> the page move ends
void arrowTouched(uint32_t f, uint32_t typeRef, uint32_t migrate, uint8_t left) {
    const uint32_t type = ld32(typeRef);
    if (type == TOUCH_CANCELED || type == TOUCH_ENDED) {
        st8(ld32(f + 4) + SV_MIGRATING, 0);
    } else if (type == TOUCH_BEGAN) {
        call(scene::fn::AudioPlaySystemSEDecide, {});
        st32(ld32(f + 4) + SV_MOVING, 0);
        st8(ld32(f + 4) + SV_MOVE_LEFT, left);
        st8(ld32(f + 4) + SV_MIGRATING, uint8_t(call(migrate, {ld32(f + 4), 1})));
    }
}
void rightArrow(uint32_t f, uint32_t, uint32_t typeRef) { arrowTouched(f, typeRef, IScrollProtocol_migrationRight, 0); }
void leftArrow(uint32_t f, uint32_t, uint32_t typeRef) { arrowTouched(f, typeRef, IScrollProtocol_migrationLeft, 1); }

// the popup slid in {widget}: the Raid_Result LWF on its "Area_Point"
void slidIn(uint32_t f) {
    const uint32_t widget = ld32(f + 4);
    Frame fr;
    const uint32_t name = scene::newShortStr(fr, "Area_Point");
    const uint32_t point = SceneUtil::searchNodeByName(widget, name);
    scene::strFree(name);
    const uint32_t lwf = call(AudioLWFNode_create, {S_RAID_RESULT_LWF, 0});
    vcall(point, scene::node::addChild, {point, lwf});
}

// sub_5BA100({widget, layer, shadow}, done): the cancel sound, the popup slides out to the right (its end removes
// it), done(), the shadow fades out
void slideOutPopup(uint32_t cap, uint32_t done) {
    call(scene::fn::AudioPlaySystemSECancel, {});
    const uint32_t widget = ld32(cap);
    {
        Frame fr;
        const uint32_t end = scene::newFunction(fr, VT_SLID_OUT, {ld32(cap + 4), ld32(cap + 8), widget});
        const uint32_t anim = call(SceneUtil_createRightOutAnimation, {widget, 0, 0, end});
        call(rr::Node_runAction, {widget, call(rr::EaseSineOut_create, {anim})});
        scene::functionDestroy(end);
    }
    if (ld32(done + 16)) call(scene::fn::function_vv_call, {done});
    const uint32_t shadow = ld32(cap + 8);
    call(rr::Node_runAction, {shadow, call(rr::FadeTo_create, {ld32(ld32(rr::GOT_POPUP_FADE_TIME)), 0})});
}

// the popup's close {OK, index, ids, widget, layer, shadow, done}(ok): OK off; the last result -> the popup slides
// out and done(); otherwise it slides out and the next result's popup opens (done passed on)
void popupClosed(uint32_t f, uint32_t okRef) {
    if (!ld8(okRef)) return;
    vcall(ld32(f + 8), V_TOUCH_ENABLED, {ld32(f + 8), 0});
    const uint32_t index = ld32(f + 0xC);
    Frame fr;
    if (index + 1 >= (ld32(f + 0x14) - ld32(f + 0x10)) >> 2) {
        const uint32_t done = fr.alloc(24);
        call(rr::Function_copy, {done, f + 0x28});
        call(slideOut, {f + 0x1C, done});
        scene::functionDestroy(done);
        return;
    }
    const uint32_t none = scene::newEmptyFunction(fr);
    call(slideOut, {f + 0x1C, none});
    scene::functionDestroy(none);
    const uint32_t done = fr.alloc(24);
    call(rr::Function_copy, {done, f + 0x28});
    call(popUpRaidBossResult, {ld32(f + 0xC) + 1, f + 0x10, done});
    scene::functionDestroy(done);
}
}  // namespace

RW_FUNCTION(0x5B9708, "RaidBossResultPopUp::popUpRaidBossResult $_3 right arrow::operator()", rightArrow);
RW_FUNCTION(0x5B97E8, "RaidBossResultPopUp::popUpRaidBossResult $_3 left arrow::operator()", leftArrow);
RW_FUNCTION(0x5B9C68, "RaidBossResultPopUp::popUpRaidBossResult $_3 slid in::operator()", slidIn);
RW_FUNCTION(0x5B9F78, "RaidBossResultPopUp::popUpRaidBossResult $_3 closed::operator()(bool)", popupClosed);
RW_FUNCTION(0x5BA100, "RaidBossResultPopUp popup slide out (sub_5BA100)", slideOutPopup);
