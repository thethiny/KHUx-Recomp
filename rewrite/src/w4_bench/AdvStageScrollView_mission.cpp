// AdvStageScrollView: the sub missions of a stage row (the three "SubMission" lines, their cleared marks and the
// bonus reward panels). All four functions are static: r0 is the row widget (or the lambda object), not a view.
//
// Stage master row fields used here (SafeBuffer offsets):
//   +284  story id (StageUtil::findStory(stageId, story) + 12: the cleared sub missions, std::vector<int>)
//   +480  number of sub missions
//   +496  char[3][129] sub mission texts (format strings)
//   +884  int[3] text format kind (1 plain, 2 value, 3 item name, 4 item name + value)
//   +896  int[3] target kind (1 Enemy, 2 Buff, 3 Badstatus, 4 Material, 5 Keyblade, 6 Skill)
//   +908  int[3] target id
//   +920  int[3] value (count)
//   +932  int[3] bonus item type, +944 int[3] bonus item id
#include "AdvStageScrollView.hpp"

using namespace scene;

namespace {
namespace lib {
constexpr uint32_t Stage_create = 0x78E4E9;            // master::Stage::create(shared_ptr out, id)
constexpr uint32_t Enemy_create = 0x77F7DD;
constexpr uint32_t Buff_create = 0x779F1D;
constexpr uint32_t Badstatus_create = 0x779525;
constexpr uint32_t Material_create = 0x7852E5;
constexpr uint32_t Keyblade_create = 0x783F9D;
constexpr uint32_t Skill_create = 0x78BDE9;
constexpr uint32_t AvatarParts_create = 0x778DE1;
constexpr uint32_t StageUtil_findStory = 0x764101;     // StageUtil::findStory(int stageId, int story)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;     // SceneUtil::setShadow(ui::Text*)
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;  // (string& plist, string& path, type, id, bool)
constexpr uint32_t IntArray3_at = 0x5149A1;            // std::array<int,3>::at(n)
constexpr uint32_t CharArray129x3_at = 0x573481;       // std::array<std::array<char,129>,3>::at(n)
constexpr uint32_t TextFormat_execute = 0x492C5D;      // TextFormat::execute(string out, fmt, ...)
constexpr uint32_t string_compare = 0x47CBD5;          // std::string::compare(const char*)
constexpr uint32_t setSubMisstion = 0x5691E1;
constexpr uint32_t subMissionText = 0x56B6C1;          // sub_56B6C0
constexpr uint32_t bonusPanel = 0x56BD35;              // sub_56BD34
}  // namespace lib

// GOT entries of cocos2d::Color3B::WHITE / BLACK
constexpr uint32_t GOT_COLOR3B_WHITE = 0xFD6060;
constexpr uint32_t GOT_COLOR3B_BLACK = 0xFD60CC;

constexpr uint32_t S_EMPTY = 0xDF8A1A;                 // ""
constexpr uint32_t S_TXT_SUBMISSION_LABEL[3] = {0xDFF440, 0xDFF456, 0xDFF46C};  // "Txt_SubMission1_Label".. (21)
constexpr uint32_t S_SUBCLEAR_BASE[3] = {0xDFF4A9, 0xDFF4C2, 0xDFF4DB};         // "SubClear1_Base".. (14)
constexpr uint32_t S_SUBCLEAR_N[3] = {0xDFF4B8, 0xDFF4D1, 0xDFF4EA};            // "SubClear1".. (9)
constexpr uint32_t S_PANEL_BONUS[3] = {0xDFF482, 0xDFF48F, 0xDFF49C};           // "Panel_Bonus1".. (12)
constexpr uint32_t S_INCENTIVE = 0xDFEF2F;             // "Incentive"
constexpr uint32_t S_MEDAL = 0xDFD05F;                 // "Medal"
constexpr uint32_t S_KB = 0xDFEF39;                    // "KB"
constexpr uint32_t S_AVT = 0xDFF863;                   // "AVT"
constexpr uint32_t S_ICON_BASE = 0xDFEB70;             // "Icon_Base"
constexpr uint32_t S_ICON_BASE_L = 0xDFEB7A;           // "Icon_Base_L"
constexpr uint32_t S_ICON_AVATAR = 0xDFEBAB;           // "Icon_Avatar"

constexpr uint32_t VS_SET_COLOR = node::setColor;      // 0x22C
constexpr uint32_t STAGE_OFF_STORY = 284;
constexpr uint32_t STAGE_OFF_MISSION_COUNT = 480;
constexpr uint32_t STAGE_OFF_MISSION_TEXT = 496;
constexpr uint32_t STAGE_OFF_TEXT_KIND = 884;
constexpr uint32_t STAGE_OFF_TARGET_KIND = 896;
constexpr uint32_t STAGE_OFF_TARGET_ID = 908;
constexpr uint32_t STAGE_OFF_VALUE = 920;
constexpr uint32_t STAGE_OFF_BONUS_TYPE = 932;
constexpr uint32_t STAGE_OFF_BONUS_ID = 944;

void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// element n of an int[3] field of a SafeBuffer, through std::array<int,3>::at (throws past 2)
int32_t safeArray3(Frame &f, uint32_t sb, uint32_t offset, int32_t n) {
    uint32_t arr = safeBytes(f, sb, offset, 12);
    return int32_t(ld32(call(lib::IntArray3_at, {arr, uint32_t(n)})));
}

// Mission `mission` (0-based) is cleared when the sorted list holds mission + 1: the first element greater than
// `mission` (std::upper_bound) is below mission + 2.
bool missionCleared(uint32_t vec, int32_t mission) {
    uint32_t first = ld32(vec), end = ld32(vec + 4);
    int32_t len = int32_t(end - first) >> 2;
    while (len != 0) {
        int32_t half = len / 2;
        uint32_t mid = first + uint32_t(half) * 4;
        if (int32_t(ld32(mid)) > mission) {
            len = half;
        } else {
            len = len - 1 - half;
            first = mid + 4;
        }
    }
    return first != end && int32_t(ld32(first)) < mission + 2;
}

// the short std::string "SubClear" the original builds inline (no string_init call)
uint32_t newSubClearStr(Frame &f) {
    uint32_t s = f.alloc(12);
    uint8_t *p = rw::frame_ptr<uint8_t>(s);
    p[0] = 8 << 1;
    std::memcpy(p + 1, "SubClear", 9);
    return s;
}

// ------------------------------------------------------------------------------------------------ functions
// AdvStageScrollView::setSubMisstion(Widget* row, int count, const vector<int>& cleared) (static): colours the
// three mission labels (yellow when cleared, grey otherwise), shows the first `count` "SubClearN_Base" frames
// (their "SubClear" child white / black) and the cleared marks. Returns the canary difference (0).
uint32_t setSubMisstion(uint32_t row, int32_t count, uint32_t cleared) {
    Frame f;
    uint32_t yellow = f.alloc(4);
    call(fn::Color3B_ctor, {yellow, 0xEA, 0xFF, 0});
    uint32_t grey = f.alloc(4);
    call(fn::Color3B_ctor, {grey, 0xC1, 0xC1, 0xC1});
    for (int32_t i = 0; i < 3; ++i) {
        bool done = missionCleared(cleared, i);
        uint32_t label = findNode(row, S_TXT_SUBMISSION_LABEL[i], 0x15);
        if (label) {
            call(lib::SceneUtil_setShadow, {label});
            vcall(label, VS_SET_COLOR, {label, done ? yellow : grey});
        }
        uint32_t base = findNode(row, S_SUBCLEAR_BASE[i], 0xE);
        if (base) {
            Frame g;
            uint32_t s = newSubClearStr(g);
            uint32_t sub = SceneUtil::searchNodeByName(base, s);
            strFree(s);
            if (sub) vcall(sub, VS_SET_COLOR, {sub, ld32(done ? GOT_COLOR3B_WHITE : GOT_COLOR3B_BLACK)});
            setVisible(base, count > i ? 1 : 0);
        }
        uint32_t mark = findNode(row, S_SUBCLEAR_N[i], 9);
        if (mark) setVisible(mark, done ? 1 : 0);
    }
    return 0;
}

// sub_56B6C0: setSubMisstionInfo's lambda [stage](int index) -> std::string: the text of sub mission `index`,
// its format filled with the target's name (enemy / buff / bad status / material / keyblade / skill) and value.
// Empty past the stage's mission count. r0 is the sret string; returns 4068 (its frame size, left in r0).
uint32_t subMissionText(uint32_t out, uint32_t stage, int32_t index) {
    // the original's sp: entry sp - 5 pushed registers - 0xFE4 (case 3 passes a stack address as an extra
    // printf argument)
    uint32_t origSp = rw::cpu->r[13] - 20 - 0xFE4;
    strInit(out, S_EMPTY, 0);
    uint32_t sb = ld32(stage);
    if (!(safe32(sb, STAGE_OFF_MISSION_COUNT) > index)) return 4068;
    Frame f;
    uint32_t texts = safeBytes(f, sb, STAGE_OFF_MISSION_TEXT, 0x183);
    uint32_t text = call(lib::CharArray129x3_at, {texts, uint32_t(index)});
    uint32_t fmt = f.alloc(12);
    strInit(fmt, text, host_strlen(text));
    uint32_t name = f.alloc(12);
    strInit(name, S_EMPTY, 0);

    int32_t id = safeArray3(f, sb, STAGE_OFF_TARGET_ID, index);
    int32_t kind = safeArray3(f, sb, STAGE_OFF_TARGET_KIND, index);
    uint32_t create = 0, nameOff = 0;
    switch (kind) {
    case 1: create = lib::Enemy_create; nameOff = 4; break;
    case 2: create = lib::Buff_create; nameOff = 8; break;
    case 3: create = lib::Badstatus_create; nameOff = 4; break;
    case 4: create = lib::Material_create; nameOff = 12; break;
    case 5: create = lib::Keyblade_create; nameOff = 4; id = id - id % 1000; break;   // the base keyblade
    case 6: create = lib::Skill_create; nameOff = 12; break;
    default: break;
    }
    if (create) {
        uint32_t row = masterRow(f, create, uint32_t(id));
        uint32_t s = safeBytes(f, ld32(row), nameOff, 0x41);
        call(fn::string_assign, {name, s, host_strlen(s)});
        sharedRelease(row);
    }

    uint32_t value = uint32_t(safeArray3(f, sb, STAGE_OFF_VALUE, index));
    int32_t textKind = safeArray3(f, sb, STAGE_OFF_TEXT_KIND, index);
    switch (uint32_t(textKind - 1)) {
    case 1: {   // format(value)
        uint32_t tmp = f.alloc(12);
        call(lib::TextFormat_execute, {tmp, strData(fmt), value});
        strMoveAssign(out, tmp);
        if (call(lib::string_compare, {out, strData(fmt)}) == 0) {   // TextFormat changed nothing: printf it
            uint32_t buf = stringBuffer(f, {strData(fmt), value});
            call(fn::string_assign, {out, buf, host_strlen(buf)});
        }
        break;
    }
    case 2: {   // printf(name), with a stray stack address as a third argument
        uint32_t buf = stringBuffer(f, {strData(fmt), strData(name), origSp + 0x24});
        call(fn::string_assign, {out, buf, host_strlen(buf)});
        break;
    }
    case 3: {   // format(name, value)
        uint32_t tmp = f.alloc(12);
        call(lib::TextFormat_execute, {tmp, strData(fmt), strData(name), value});
        strMoveAssign(out, tmp);
        if (call(lib::string_compare, {out, strData(fmt)}) == 0) {
            uint32_t buf = stringBuffer(f, {strData(fmt), strData(name), value});
            call(fn::string_assign, {out, buf, host_strlen(buf)});
        }
        break;
    }
    default: {   // 1 and others: the text as is
        uint32_t s = strData(fmt);
        call(fn::string_assign, {out, s, host_strlen(s)});
        break;
    }
    }
    strFree(name);
    strFree(fmt);
    return 4068;
}

// sub_56BD34: setSubMisstionInfo's lambda [stage](Widget* panel, int index): the bonus reward panel of sub
// mission `index`, hidden past the mission count; shows the reward icon in the "Medal" (type 3), "KB" (13),
// "AVT" (18..20, avatar parts: base frame by part kind 1 / 2) or "Incentive" (others) image. Returns 0.
uint32_t bonusPanel(uint32_t stage, uint32_t panel, int32_t index) {
    uint32_t sb = ld32(stage);
    int32_t count = safe32(sb, STAGE_OFF_MISSION_COUNT);
    if (!(count > index)) {
        setVisible(panel, 0);
        return 0;
    }
    setVisible(panel, 1);
    Frame f;
    int32_t type = safeArray3(f, ld32(stage), STAGE_OFF_BONUS_TYPE, index);
    int32_t id = safeArray3(f, ld32(stage), STAGE_OFF_BONUS_ID, index);
    uint32_t path = f.alloc(12);
    strInit(path, S_EMPTY, 0);
    uint32_t plist = f.alloc(12);
    strInit(plist, S_EMPTY, 0);
    call(lib::SceneUtil_setItemImageData, {plist, path, uint32_t(type), uint32_t(id), 0});
    uint32_t incentive = findNode(panel, S_INCENTIVE, 9);
    uint32_t medal = findNode(panel, S_MEDAL, 5);
    uint32_t kb = findNode(panel, S_KB, 2);
    uint32_t avt = findNode(panel, S_AVT, 3);
    setVisible(incentive, 0);
    setVisible(medal, 0);
    setVisible(kb, 0);
    setVisible(avt, 0);
    if (uint32_t(type - 18) < 3) {
        uint32_t row = masterRow(f, lib::AvatarParts_create, uint32_t(id));
        int32_t partKind = safe32(ld32(row), 140);
        uint32_t iconBase = findNode(avt, S_ICON_BASE, 9);
        uint32_t iconBaseL = findNode(avt, S_ICON_BASE_L, 0xB);
        uint32_t icon = findNode(avt, S_ICON_AVATAR, 0xB);
        call(fn::ImageView_loadTexture, {icon, path, 0});
        setVisible(iconBase, partKind == 1 ? 1 : 0);
        setVisible(iconBaseL, partKind == 2 ? 1 : 0);
        setVisible(avt, 1);
        sharedRelease(row);
    } else {
        uint32_t image = type == 3 ? medal : type == 13 ? kb : incentive;
        call(fn::ImageView_loadTexture, {image, path, 0});
        setVisible(image, 1);
    }
    strFree(plist);
    strFree(path);
    return 0;
}

// AdvStageScrollView::setSubMisstionInfo(Widget* row, int stageId, const vector<int>* cleared) (static): fills
// the sub mission texts and bonus panels of stage `stageId` and marks the cleared ones (the stage's story list
// when `cleared` is null). Returns the canary difference (0).
uint32_t setSubMisstionInfo(uint32_t row, int32_t stageId, uint32_t cleared) {
    if (stageId == 0) return 0;
    Frame f;
    uint32_t stage = masterRow(f, lib::Stage_create, uint32_t(stageId));
    uint32_t list = cleared;
    if (!list) {
        int32_t story = safe32(ld32(stage), STAGE_OFF_STORY);
        list = call(lib::StageUtil_findStory, {uint32_t(stageId), uint32_t(story)}) + 12;
    }
    // the text lambda: captures the stage row (a shared_ptr copy)
    uint32_t textFn = f.alloc(8);
    st32(textFn, ld32(stage));
    uint32_t textCb = ld32(stage + 4);
    st32(textFn + 4, textCb);
    if (textCb) call(fn::shared_add, {textCb});
    for (int32_t i = 0; i < 3; ++i) {
        uint32_t label = findNode(row, S_TXT_SUBMISSION_LABEL[i], 0x15);
        Frame g;
        uint32_t s = g.alloc(12);
        call(lib::subMissionText, {s, textFn, uint32_t(i)});
        call(fn::Text_setString, {label, s});
        strFree(s);
    }
    // the bonus panel lambda: another copy
    uint32_t bonusFn = f.alloc(8);
    st32(bonusFn, ld32(stage));
    uint32_t bonusCb = ld32(stage + 4);
    st32(bonusFn + 4, bonusCb);
    if (bonusCb) call(fn::shared_add, {bonusCb});
    for (int32_t i = 0; i < 3; ++i) {
        uint32_t panel = findNode(row, S_PANEL_BONUS[i], 0xC);
        if (panel) call(lib::bonusPanel, {bonusFn, panel, uint32_t(i)});
    }
    int32_t count = safe32(ld32(stage), STAGE_OFF_MISSION_COUNT);
    call(lib::setSubMisstion, {row, uint32_t(count), list});
    if (bonusCb) call(fn::shared_release, {bonusCb});
    if (textCb) call(fn::shared_release, {textCb});
    sharedRelease(stage);
    return 0;
}
}  // namespace

RW_FUNCTION(0x5691E0, "AdvStageScrollView::setSubMisstion", setSubMisstion);
RW_FUNCTION(0x56B300, "AdvStageScrollView::setSubMisstionInfo", setSubMisstionInfo);
RW_FUNCTION(0x56B6C0, "AdvStageScrollView::setSubMisstionInfo (sub mission text lambda)", subMissionText);
RW_FUNCTION(0x56BD34, "AdvStageScrollView::setSubMisstionInfo (bonus panel lambda)", bonusPanel);
