// AdvStageScrollView: the boss info box of a stage row (boss actor clipped into the row, name / Lux / level /
// info texts) and the raid boss info box (raid title, remaining time).
//
// These "members" are static: r0 is the row widget, not the view.
#include "AdvStageScrollView.hpp"

#include "net/api/Raid.hpp"

using namespace scene;

namespace {
namespace lib {
constexpr uint32_t FileUtils_getInstance = 0xA3F44D;
constexpr uint32_t Data_getBytes = 0x8ED90B;
constexpr uint32_t Data_dtor = 0x8ED89B;
constexpr uint32_t getInt32LE = 0x75992B;            // StageManager::getInteger32bitFromLittleEndianBytes(uint8_t*)
constexpr uint32_t MasterEnemy_create = 0x77F7DD;    // master::Enemy::create(shared_ptr out, int id)
constexpr uint32_t Enemy_create = 0x5CBE25;          // Enemy::create(int): the battle enemy actor
constexpr uint32_t setBossTypeImage = 0x46C4C5;      // SceneUtil::setBossTypeImage(ImageView*, int, bool)
constexpr uint32_t setShadow = 0x45BD15;             // SceneUtil::setShadow(ui::Text*)
constexpr uint32_t getMoneyStr = 0x461DC5;           // SceneUtil::getMoneyStr(string out, unsigned long long)
constexpr uint32_t Layout_create = 0x82861D;
constexpr uint32_t Sprite_create = 0x8A64DD;
constexpr uint32_t ClippingNode_create = 0x874711;
constexpr uint32_t ClippingNode_setAlphaThreshold = 0x875003;
constexpr uint32_t ClippingNode_setStencil = 0x874FD1;
constexpr uint32_t Size_copy = 0x8DF02D;             // Size::Size(const Size&)
constexpr uint32_t Size_div = 0x8DF0D5;              // Size::operator/(float) const (sret)
constexpr uint32_t Rect_ctor = 0x8DF18B;             // Rect::Rect(x, y, w, h)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;             // Vec2::Vec2(x, y)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t MasterRaidEnemy_create = 0x787745;  // master::RaidEnemy::create(shared_ptr out, int id)
constexpr uint32_t SafeBuffer_ctor = 0x459F21;       // SafeBuffer::SafeBuffer(unsigned size, bool)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;       // SafeBuffer::~SafeBuffer()
constexpr uint32_t shared_add = 0xC8DBB5;            // __shared_weak_count::__add_shared
constexpr uint32_t RaidParts_setMasterParam = 0x5F1551;  // RaidParts::set_master_param(shared_ptr<const RaidEnemy>, int, int, int)
constexpr uint32_t ImageView_loadTexture = 0x839731; // (image, const string&, TextureResType)
constexpr uint32_t LoadingBar_setPercent = 0x83BB05; // (bar, float)
constexpr uint32_t Date_intervalSinceNow = 0x4922D5; // Date::calculateIntervalSinceNow() const: seconds
constexpr uint32_t PartArray_at = 0x56CB51;          // std::array<RaidBase::Info::Part, 6>::at(i) const
constexpr uint32_t getCampaignPoint = 0x4741BD;      // SceneUtil::getCampaignPoint(int, int, int, const vector<int>&, int, bool)
constexpr uint32_t pointToString = 0x506B61;         // AdvStageScrollView::pointToString(string out, int, int, bool)
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t Vec2_copy = 0x8E4C03;             // Vec2::Vec2(const Vec2&)
}  // namespace lib

// raid boss box
constexpr uint32_t GOT_RAIDPARTS_VTABLE = 0xFD65BC;      // GOT: _ZTV9RaidParts (+8 is the vtable pointer)
constexpr uint32_t GOT_INFO_RAID_BOSS_PASS = 0xFD668C;   // GOT: &AdvStageScrollView::INFO_RAID_BOSS_PASS (const char*[])
constexpr uint32_t S_TXT_NAME_LV_LABEL = 0xDFF4F4;       // "Txt_Name_LV_Label"
constexpr uint32_t S_FMT_S = 0xDF8821;                   // "%s"
constexpr uint32_t S_RAID_BOSS_HPFIX_LABEL = 0xDFD390;   // "Raid_Boss_HpFix_Label"
constexpr uint32_t S_RAID_BOSS_HP_PROGRESSBAR = 0xDFD3A6;   // "Raid_Boss_Hp_ProgressBar"
constexpr uint32_t S_RAID_BOSS_TIMELIMIT_TXT = 0xDFD3BF;    // "Raid_Boss_TimeLimit_Txt"
constexpr uint32_t S_RAID_RARE_POP = 0xDFD318;           // "Raid_Rare_Pop"
constexpr uint32_t S_TIMELIMIT_HHMMSS = 0xDFF506;        // "Raid_Boss_TimeLimitFix_HHMMSS_Label"
constexpr uint32_t S_RAID_BOSS_BALLON = 0xDFF52A;        // "Raid_Boss_Ballon"
constexpr uint32_t S_RAID_BOSS_CHANCE = 0xDFF53B;        // "Raid_Boss_Chance"
constexpr uint32_t S_TXT_PARTS_BREAK = 0xDFF54C;         // "Txt_Parts_Break"
constexpr uint32_t S_TXT_NUM_LABEL = 0xDFD8DC;           // "Txt_Num_Label"
constexpr uint32_t S_TXT_SLASH = 0xDFF55C;               // "Txt_Slash"
constexpr uint32_t S_SLASH = 0xDF8C66;                   // "/"
constexpr uint32_t S_TXT_ALL_LABEL = 0xDFF566;           // "Txt_All_Label"
constexpr uint32_t S_TXT_PART_RESTORATION = 0xDFF574;    // "Txt_Part_Restoration"
constexpr uint32_t S_TXT_TIME_LABEL = 0xDFF589;          // "Txt_Time_Label"
constexpr uint32_t S_PANEL_AP = 0xDFF69E;                // "Panel_AP"
constexpr uint32_t S_AP2_PNG = 0xDFF6A7;                 // "img/ui/World_Stage_AP2.png"
constexpr uint32_t S_ATLASLABEL_AP = 0xDFD382;           // "AtlasLabel_AP"

constexpr int32_t TEXT_RAID_TIME_LIMIT = 103200002;      // the "time limit" label
constexpr int32_t TEXT_RAID_HHMMSS = 103200003;          // format: hours, minutes, seconds
constexpr int32_t TEXT_RAID_PARTS_BREAK = 103200027;
constexpr int32_t TEXT_RAID_PART_RESTORATION = 103200025;
constexpr int32_t TEXT_RAID_MMSS = 103200026;            // format: minutes, seconds

constexpr uint32_t RAIDENEMY_NAME = 4;              // master::RaidEnemy: char[0x81]
constexpr uint32_t RAIDENEMY_PART_COUNT = 0x94;     // master::RaidEnemy: number of parts
constexpr uint32_t RAIDPARTS_SIZE = 0x24;           // RaidParts: a SafeBuffer(0x60) + one word (+32, zeroed)
constexpr uint32_t RAIDPARTS_MAX_HP = 0x10;         // RaidParts SafeBuffer: the boss's max HP (unsigned)
constexpr uint32_t API_CAMPAIGNS = 764;             // APIManager + 764: std::vector<int> (campaigns)

constexpr uint32_t VS_GET_ANIMATION = 0x298;        // cocostudio::Armature::getAnimation()
constexpr uint32_t VS_PLAY_WITH_INDEX = 0x70;       // ArmatureAnimation::playWithIndex(index, durationTo, loop)

using RaidInfo = hole::network::api::RaidBase::Info;

// GOT entries (relocated at load): addresses of cocos2d::Vec2 anchor constants
constexpr uint32_t GOT_ANCHOR_DUMMY = 0xFD6054;      // the anchor point given to the "Dummy_Panel" layout
constexpr uint32_t GOT_ANCHOR_STENCIL = 0xFD6080;    // the anchor point given to the clipping stencil sprite

// virtual slots (byte offsets)
constexpr uint32_t VS_GET_Z_ORDER = 0x1C;           // Node: the z order the boss icon's parent reports
constexpr uint32_t VS_REMOVE_CHILD_BY_NAME = 0x134; // Node::removeChildByName(name, cleanup)
constexpr uint32_t VS_LAYOUT_31C = 0x31C;           // a Layout virtual taking a bool (called with false)
constexpr uint32_t VS_FILE_GET_DATA = 0x10;         // FileUtils::getDataFromFile(Data out, const string&)
constexpr uint32_t VS_SET_TEXTURE_RECT = 0x284;     // Sprite::setTextureRect(const Rect&)

// strings
constexpr uint32_t S_MAPPOI_FMT = 0xDF9089;         // "stage/mappoi_stg%05d.bin"
constexpr uint32_t S_KEYBLADE_TYPE = 0xDF8A58;      // "KeyBlade_type"
constexpr uint32_t S_RAID_BOSS_ICON = 0xDFD2D3;     // "Raid_Boss_Icon"
constexpr uint32_t S_DUMMY_PANEL = 0xDFF409;        // "Dummy_Panel"
constexpr uint32_t S_ENEMY_ACTOR = 0xDFF415;        // "EnemyActor"
constexpr uint32_t S_TXT_NAME_LABEL = 0xDFDBB8;     // "Txt_Name_Label"
constexpr uint32_t S_TXT_LUX_LABEL = 0xDFDBC7;      // "Txt_Lux_Label"
constexpr uint32_t S_FMT_S_S = 0xDFDAC7;            // "%s %s"
constexpr uint32_t S_TXT_LV = 0xDF8DE1;             // "Txt_LV"
constexpr uint32_t S_TXT_LV_NUM_LABEL = 0xDFF420;   // "Txt_LV_Num_Label"
constexpr uint32_t S_FMT_D = 0xDF86DE;              // "%d"
constexpr uint32_t S_TXT_INFO_LABEL = 0xDFF431;     // "Txt_Info_Label"
constexpr uint32_t S_CLEAR = 0xDFD17B;              // "Clear"

constexpr int32_t TEXT_LUX = 100200001;             // TextData: the "Lux" suffix
constexpr int32_t TEXT_LV = 100200002;              // TextData: the "LV" label

constexpr uint32_t API_CURRENT_STAGE = 292;         // APIManager + 292: SafeBuffer* (+12: the current stage id)

// master::Enemy row fields (SafeBuffer offsets)
constexpr uint32_t ENEMY_NAME = 4;                  // char[0x41]
constexpr uint32_t ENEMY_INFO = 0x45;               // char[0x201]
constexpr uint32_t ENEMY_ACTOR_ID = 0x248;          // the battle actor (Enemy::create)
constexpr uint32_t ENEMY_HEIGHT = 0x338;            // the actor's height in pixels
constexpr uint32_t ENEMY_LUX_STEPS = 0x27C;         // number of entries of the two tables below
constexpr uint32_t ENEMY_LUX_LEVELS = 0x280;        // int[5]: levels
constexpr uint32_t ENEMY_LUX_VALUES = 0x30C;        // int[5]: Lux at those levels

// stage/mappoi_stgNNNNN.bin header (little-endian ints)
constexpr uint32_t MAPPOI_ENEMY_ID = 0x18;          // the stage's enemy
constexpr uint32_t MAPPOI_BOSS_ID = 0x28;           // the boss (0: use the stage's enemy)
constexpr uint32_t MAPPOI_BOSS_LEVEL = 0x2C;

void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// n bytes of a SafeBuffer at `index` decoded into dst (SafeBuffer::read)
void readBytes(uint32_t dst, uint32_t sb, uint32_t index, uint32_t n) {
    const SafeBuffer *s = safe(sb);
    SafeBuffer::read(s, rw::host_ptr<uint8_t>(dst), s->data.get() + index, n, index);
}

// a std::string on the guest stack holding a copy of the C string at `text` (strlen + __init)
uint32_t newStrC(Frame &f, uint32_t text) { return newStr(f, text); }

// The raid labels' inline pattern: a std::string of `cstr`, a shadow on the label, then the label's text set from a
// second copy of that string.
void shadowText(uint32_t label, uint32_t cstr) {
    Frame f;
    uint32_t tmp = newStrC(f, cstr);
    call(lib::setShadow, {label});
    uint32_t s = newStrC(f, strData(tmp));
    call(fn::Text_setString, {label, s});
    strFree(s);
    strFree(tmp);
}
// the same with a TextData text (fetched before, released after)
void shadowTextId(uint32_t label, int32_t id) {
    Frame f;
    uint32_t sp = textData(f, id);
    shadowText(label, textDataStr(sp));
    sharedRelease(sp);
}
}  // namespace

namespace AdvStageScrollViewRaid {

uint32_t updateRaidBossInfoTime(uint32_t widget, const RaidInfo *info);

// sub_56B1F4: the setBossInfo lambda `[&widget](const string& name, const string& text)`: the Text named `name`
// in the row gets a shadow and `text`. capture: the lambda object (its first word is the row widget). Returns the
// stack-guard difference (0).
uint32_t setLabel(uint32_t capture, uint32_t name, uint32_t text) {
    uint32_t root = ld32(capture);
    uint32_t label;
    {
        Frame f;
        uint32_t s = newStrC(f, strData(name));
        label = SceneUtil::searchNodeByName(root, s);
        strFree(s);
    }
    if (label) {
        call(lib::setShadow, {label});
        Frame f;
        uint32_t s = newStrC(f, strData(text));
        call(fn::Text_setString, {label, s});
        strFree(s);
    }
    return 0;
}

// The boss box of a stage row: the boss type image, the boss actor (from the stage's mappoi file) clipped into a
// "Dummy_Panel" over the raid boss icon, the boss's name, Lux (interpolated from its level), level and info, and
// the "Clear" mark (story stages already passed). stageId 0: nothing. Returns what the epilogue leaves in r0 (the
// frame size, 0x102C) on every path.
uint32_t setBossInfo(uint32_t widget, int32_t stageId, int32_t stageType) {
    constexpr uint32_t EPILOGUE_R0 = 0x102C;
    if (stageId == 0) return EPILOGUE_R0;
    Frame f;
    uint32_t data = f.alloc(8);   // cocos2d::Data
    {
        uint32_t fu = call(lib::FileUtils_getInstance, {});
        uint32_t getData = ld32(ld32(fu) + VS_FILE_GET_DATA);
        Frame g;
        uint32_t path = stringBuffer(g, {S_MAPPOI_FMT, uint32_t(stageId)});
        uint32_t s = newStrC(g, path);
        call(getData, {data, fu, s});
        strFree(s);
    }
    uint32_t bytes = call(lib::Data_getBytes, {data});
    uint32_t bossId = call(lib::getInt32LE, {bytes + MAPPOI_BOSS_ID});
    if (bossId == 0) bossId = call(lib::getInt32LE, {call(lib::Data_getBytes, {data}) + MAPPOI_ENEMY_ID});
    uint32_t enemyRow = masterRow(f, lib::MasterEnemy_create, bossId);

    uint32_t typeImage = findNode(widget, S_KEYBLADE_TYPE, 13);
    call(lib::setBossTypeImage, {typeImage, uint32_t(stageId), 0});
    uint32_t bossIcon = findNode(widget, S_RAID_BOSS_ICON, 14);
    setVisible(bossIcon, 0);

    uint32_t dummy = findNode(widget, S_DUMMY_PANEL, 11);
    if (!dummy) {
        uint32_t layout = call(lib::Layout_create, {});
        uint32_t parent = vcall(bossIcon, node::getParent, {bossIcon});
        uint32_t addChild = ld32(ld32(parent) + node::addChildZName);
        Frame g;
        uint32_t s = newStr(g, S_DUMMY_PANEL, 11);
        call(addChild, {parent, layout, 0, s});
        strFree(s);
        dummy = layout;
    }
    // the panel sits at the centre of the boss icon's parent
    {
        uint32_t setPosition = ld32(ld32(dummy) + node::setPosition);
        uint32_t parent = vcall(bossIcon, node::getParent, {bossIcon});
        uint32_t size = vcall(parent, node::getContentSize, {parent});
        Frame g;
        uint32_t half = g.alloc(8);
        call(lib::Size_div, {half, size, fbits(2.0f)});
        uint32_t pos = g.alloc(8);
        call(lib::Vec2_ctor, {pos, ld32(half), ld32(half + 4)});
        call(setPosition, {dummy, pos});
        call(lib::Vec2_dtor, {pos});
    }
    vcall(dummy, node::setAnchorPoint, {dummy, ld32(GOT_ANCHOR_DUMMY)});
    vcall(dummy, VS_LAYOUT_31C, {dummy, 0});
    {
        uint32_t removeByName = ld32(ld32(dummy) + VS_REMOVE_CHILD_BY_NAME);
        Frame g;
        uint32_t s = newStr(g, S_ENEMY_ACTOR, 10);
        call(removeByName, {dummy, s, 1});
        strFree(s);
    }

    // the boss actor, scaled down to fit the parent's height, standing at a quarter of the icon's width
    uint32_t row = ld32(enemyRow);
    uint32_t enemy = call(lib::Enemy_create, {uint32_t(safe32(row, ENEMY_ACTOR_ID))});
    float halfHeight = float(safe32(row, ENEMY_HEIGHT)) * 0.5f;
    float quarterW = ldf(vcall(bossIcon, node::getContentSize, {bossIcon})) * 0.25f;
    uint32_t iconParent = vcall(bossIcon, node::getParent, {bossIcon});
    int32_t parentH = f2i(ldf(vcall(iconParent, node::getContentSize, {iconParent}) + 4));
    float scale = 1.0f;
    if (safe32(row, ENEMY_HEIGHT) > parentH) scale = float(parentH) / float(safe32(row, ENEMY_HEIGHT));
    uint32_t setPositionXY = ld32(ld32(enemy) + node::setPositionXY);
    float iconX = ldf(vcall(bossIcon, node::getPosition, {bossIcon}));
    uint32_t iconPos = vcall(bossIcon, node::getPosition, {bossIcon});
    float x = quarterW + iconX;
    float y = ldf(iconPos + 4) - halfHeight * scale;
    call(setPositionXY, {enemy, fbits(x), fbits(y)});
    vcall(enemy, node::setScale, {enemy, fbits(scale)});

    // clipped to the panel's rectangle
    uint32_t stencil = call(lib::Sprite_create, {});
    {
        Frame g;
        uint32_t size = g.alloc(8);
        call(lib::Size_copy, {size, vcall(dummy, node::getContentSize, {dummy})});
        uint32_t setTextureRect = ld32(ld32(stencil) + VS_SET_TEXTURE_RECT);
        uint32_t rect = g.alloc(16);
        call(lib::Rect_ctor, {rect, 0, 0, ld32(size), ld32(size + 4)});
        call(setTextureRect, {stencil, rect});
        call(lib::Vec2_dtor, {rect});
    }
    vcall(stencil, node::setAnchorPoint, {stencil, ld32(GOT_ANCHOR_STENCIL)});
    uint32_t clip = call(lib::ClippingNode_create, {});
    call(lib::ClippingNode_setAlphaThreshold, {clip, 0});
    call(lib::ClippingNode_setStencil, {clip, stencil});
    vcall(clip, node::addChild, {clip, enemy});
    {
        uint32_t addChild = ld32(ld32(dummy) + node::addChildZName);
        uint32_t parent = vcall(bossIcon, node::getParent, {bossIcon});
        uint32_t z = vcall(parent, VS_GET_Z_ORDER, {parent});
        Frame g;
        uint32_t s = newStr(g, S_ENEMY_ACTOR, 10);
        call(addChild, {dummy, clip, z, s});
        strFree(s);
    }

    uint32_t capture = f.alloc(4);   // the label lambda captures the row widget
    st32(capture, widget);
    // the levels / Lux tables and the name are adjacent like in the original frame (the level search can read one
    // entry past the levels table)
    uint32_t tables = f.alloc(0x14 + 0x14 + 0x48);
    uint32_t luxValues = tables, luxLevels = tables + 0x14, nameBuf = tables + 0x28;
    {
        Frame g;
        uint32_t label = newStr(g, S_TXT_NAME_LABEL, 14);
        readBytes(nameBuf, row, ENEMY_NAME, 0x41);
        uint32_t text = newStrC(g, nameBuf);
        setLabel(capture, label, text);
        strFree(text);
        strFree(label);
    }

    // Lux: linear interpolation of the boss row's level -> Lux table at the stage's boss level
    uint32_t enemyId = call(lib::getInt32LE, {bytes + MAPPOI_ENEMY_ID});
    uint32_t bossId2 = call(lib::getInt32LE, {bytes + MAPPOI_BOSS_ID});
    int32_t level = int32_t(call(lib::getInt32LE, {bytes + MAPPOI_BOSS_LEVEL}));
    if (bossId2 == 0) bossId2 = enemyId;
    uint32_t bossRow = masterRow(f, lib::MasterEnemy_create, bossId2);
    uint32_t brow = ld32(bossRow);
    int32_t steps = safe32(brow, ENEMY_LUX_STEPS);
    readBytes(luxLevels, brow, ENEMY_LUX_LEVELS, 0x14);
    readBytes(luxValues, brow, ENEMY_LUX_VALUES, 0x14);
    int32_t k = 1;
    int32_t hiLevel;
    for (;;) {
        hiLevel = int32_t(ld32(luxLevels + uint32_t(k) * 4));
        if (k >= steps || hiLevel >= level) break;
        ++k;
    }
    int32_t loLevel = int32_t(ld32(luxLevels + uint32_t(k - 1) * 4));
    int32_t loLux = int32_t(ld32(luxValues + uint32_t(k - 1) * 4));
    int32_t hiLux = int32_t(ld32(luxValues + uint32_t(k) * 4));
    {
        Frame g;
        uint32_t label = newStr(g, S_TXT_LUX_LABEL, 13);
        float dLux = float(int32_t(uint32_t(hiLux) - uint32_t(loLux)));
        float t = float(int32_t(uint32_t(level) - uint32_t(loLevel))) /
                  float(int32_t(uint32_t(hiLevel) - uint32_t(loLevel)));
        float d = dLux * t;
        int32_t lux = f2i(float(loLux) + d);
        uint32_t money = g.alloc(12);
        call(lib::getMoneyStr, {money, uint32_t(lux), uint32_t(lux), uint32_t(lux >> 31)});
        uint32_t moneyC = strIsLong(money) ? ld32(money + 8) : (money | 1);
        uint32_t suffix = textData(g, TEXT_LUX);
        uint32_t buf = stringBuffer(g, {S_FMT_S_S, moneyC, textDataStr(suffix)});
        uint32_t text = newStrC(g, buf);
        setLabel(capture, label, text);
        strFree(text);
        sharedRelease(suffix);
        strFree(money);
        strFree(label);
    }
    {
        Frame g;
        uint32_t label = newStr(g, S_TXT_LV, 6);
        uint32_t lv = textData(g, TEXT_LV);
        uint32_t text = newStrC(g, textDataStr(lv));
        setLabel(capture, label, text);
        strFree(text);
        sharedRelease(lv);
        strFree(label);
    }
    {
        Frame g;
        uint32_t label = newStr(g, S_TXT_LV_NUM_LABEL, 16);
        uint32_t buf = stringBuffer(g, {S_FMT_D, uint32_t(level)});
        uint32_t text = newStrC(g, buf);
        setLabel(capture, label, text);
        strFree(text);
        strFree(label);
    }
    {
        Frame g;
        uint32_t label = newStr(g, S_TXT_INFO_LABEL, 14);
        uint32_t info = g.alloc(0x208);
        readBytes(info, row, ENEMY_INFO, 0x201);
        uint32_t text = newStrC(g, info);
        setLabel(capture, label, text);
        strFree(text);
        strFree(label);
    }

    // "Clear": story rows before the current stage
    uint32_t current = ld32(ld32(call(lib::APIManager_getInstance, {}) + API_CURRENT_STAGE) + 12);
    uint32_t clear = findNode(widget, S_CLEAR, 5);
    if (clear) {
        if (stageType != 0) setVisible(clear, 0);
        else if (current == 0) setVisible(clear, 1);
        else setVisible(clear, current > uint32_t(stageId) ? 1 : 0);
    }
    sharedRelease(bossRow);
    sharedRelease(enemyRow);
    call(lib::Data_dtor, {data});
    return EPILOGUE_R0;
}

// The boss box of a raid row: boss image, type, name, level, HP (number and bar), time texts, the rare mark.
// Returns what the epilogue leaves in r0 (the frame size, 4620).
uint32_t setRaidBossInfo(uint32_t widget, const RaidInfo *info) {
    // The RaidParts local is a SafeBuffer registered for updates (its address goes into a global list): the
    // original frame (push 0x14 + sub 0x120C) keeps it at its original address.
    Frame f;
    uint32_t base = f.allocExact(0x1220);
    uint32_t parts = base + 0x108;
    uint32_t enemyRow = base + 0x12C;   // shared_ptr<const master::RaidEnemy>
    int32_t enemyId = info->parts[0].raidEnemyId;
    call(lib::MasterRaidEnemy_create, {enemyRow, uint32_t(enemyId)});
    call(lib::SafeBuffer_ctor, {parts, 0x60, 1});
    st32(parts, ld32(GOT_RAIDPARTS_VTABLE) + 8);
    st32(parts + 32, 0);
    {
        uint32_t copy = base + 0x100;   // the shared_ptr passed by value
        st32(copy, ld32(enemyRow));
        st32(copy + 4, ld32(enemyRow + 4));
        if (ld32(copy + 4)) call(lib::shared_add, {ld32(copy + 4)});
        // at the original sp (its fifth argument in the frame's outgoing-argument word)
        rw::call_exact(lib::RaidParts_setMasterParam, {parts, copy, uint32_t(int32_t(info->level)), 0, 0});
        sharedRelease(copy);
    }

    uint32_t icon = findNode(widget, S_RAID_BOSS_ICON, 14);
    {
        Frame g;
        uint32_t fmt = ld32(ld32(GOT_INFO_RAID_BOSS_PASS));   // INFO_RAID_BOSS_PASS[0]
        uint32_t path = stringBuffer(g, {fmt, uint32_t(int32_t(info->parts[0].raidEnemyId))});
        uint32_t s = newStrC(g, path);
        call(lib::ImageView_loadTexture, {icon, s, 0});
        strFree(s);
    }
    uint32_t type = findNode(widget, S_KEYBLADE_TYPE, 13);
    if (type) call(lib::setBossTypeImage, {type, uint32_t(int32_t(info->parts[0].raidEnemyId)), 1});

    uint32_t name = findNode(widget, S_TXT_NAME_LV_LABEL, 17);
    if (name) {
        Frame g;
        uint32_t raw = g.alloc(0x88);
        readBytes(raw, ld32(enemyRow), RAIDENEMY_NAME, 0x81);
        uint32_t buf = stringBuffer(g, {S_FMT_S, raw});
        shadowText(name, buf);
    }
    uint32_t lv = findNode(widget, S_TXT_LV, 6);
    if (lv) shadowTextId(lv, TEXT_LV);
    uint32_t lvNum = findNode(widget, S_TXT_LV_NUM_LABEL, 16);
    if (lvNum) {
        Frame g;
        shadowText(lvNum, stringBuffer(g, {S_FMT_D, uint32_t(int32_t(info->level))}));
    }
    uint32_t hpText = findNode(widget, S_RAID_BOSS_HPFIX_LABEL, 21);
    if (hpText) {
        Frame g;
        shadowText(hpText, stringBuffer(g, {S_FMT_D, uint32_t(int32_t(info->parts[0].hp))}));
    }
    uint32_t hpBar = findNode(widget, S_RAID_BOSS_HP_PROGRESSBAR, 24);
    if (hpBar) {
        int32_t hp = info->parts[0].hp;
        uint32_t maxHp = uint32_t(safe32(parts, RAIDPARTS_MAX_HP));
        float percent = (float(hp) / float(maxHp)) * 100.0f;
        call(lib::LoadingBar_setPercent, {hpBar, fbits(percent)});
    }
    uint32_t limit = findNode(widget, S_RAID_BOSS_TIMELIMIT_TXT, 23);
    if (limit) shadowTextId(limit, TEXT_RAID_TIME_LIMIT);
    updateRaidBossInfoTime(widget, info);
    uint32_t rare = findNode(widget, S_RAID_RARE_POP, 13);
    if (rare) setVisible(rare, int32_t(info->parts[0].rare) == 1 ? 0 : 1);
    uint32_t clear = findNode(widget, S_CLEAR, 5);
    if (clear) setVisible(clear, 0);
    call(lib::SafeBuffer_dtor, {parts});
    sharedRelease(enemyRow);
    return 4620;
}

// The raid row's timers: the time left (h:m:s, capped at 99:59:59), the fever ("chance") balloons, the broken parts
// count and the part restoration countdown (m:s, capped at 59:59) while the fever lasts. Returns 1 while the raid has
// time left, else 0.
uint32_t updateRaidBossInfoTime(uint32_t widget, const RaidInfo *info) {
    uint32_t inf = rw::guest_addr(info);
    Frame f;
    uint32_t enemyRow = masterRow(f, lib::MasterRaidEnemy_create, uint32_t(int32_t(info->parts[0].raidEnemyId)));
    uint32_t fever = info->feverFlag;
    int32_t left = int32_t(call(lib::Date_intervalSinceNow, {inf + 16}));   // timeLeft
    if (left < 0) left = 0;
    if (left > 359999) left = 359999;

    uint32_t hhmmss = findNode(widget, S_TIMELIMIT_HHMMSS, 35);
    if (hhmmss) {
        Frame g;
        uint32_t fmt = textData(g, TEXT_RAID_HHMMSS);
        uint32_t buf = stringBuffer(g, {textDataStr(fmt), uint32_t(left / 3600), uint32_t(left / 60 % 60),
                                        uint32_t(left % 60)});
        shadowText(hhmmss, buf);
        sharedRelease(fmt);
    }

    // broken parts: parts 1..count with no HP left (part 0 skipped and part `count` read: the game's indexing)
    int32_t count = safe32(ld32(enemyRow), RAIDENEMY_PART_COUNT);
    if (count < 0) count = 0;
    int32_t broken = 0;
    for (int32_t i = 1; i <= count; ++i) {
        uint32_t part = call(lib::PartArray_at, {inf + 44, uint32_t(i)});
        if (ld32(part + 8) == 0) ++broken;   // Part::hp
    }

    int32_t feverLeft = int32_t(call(lib::Date_intervalSinceNow, {inf + 26}));   // feverTime
    if (feverLeft < 0) feverLeft = 0;
    if (feverLeft > 3599) feverLeft = 3599;
    bool feverOn = fever != 0 && feverLeft != 0;

    uint32_t balloon = findNode(widget, S_RAID_BOSS_BALLON, 16);
    if (balloon) setVisible(balloon, feverOn ? 1 : 0);
    uint32_t chance = findNode(widget, S_RAID_BOSS_CHANCE, 16);
    if (chance) setVisible(chance, feverOn ? 1 : 0);

    uint32_t partsBreak = findNode(widget, S_TXT_PARTS_BREAK, 15);
    if (partsBreak) {
        if (feverOn) {
            setVisible(partsBreak, 1);
            shadowTextId(partsBreak, TEXT_RAID_PARTS_BREAK);
            uint32_t num = findNode(partsBreak, S_TXT_NUM_LABEL, 13);
            {
                Frame g;
                shadowText(num, stringBuffer(g, {S_FMT_D, uint32_t(broken)}));
            }
            uint32_t slash = findNode(partsBreak, S_TXT_SLASH, 9);
            shadowText(slash, S_SLASH);
            uint32_t all = findNode(partsBreak, S_TXT_ALL_LABEL, 13);
            {
                Frame g;
                shadowText(all, stringBuffer(g, {S_FMT_D, uint32_t(count)}));
            }
        } else {
            setVisible(partsBreak, 0);
        }
    }
    uint32_t restoration = findNode(widget, S_TXT_PART_RESTORATION, 20);
    if (restoration) {
        if (feverOn) {
            setVisible(restoration, 1);
            shadowTextId(restoration, TEXT_RAID_PART_RESTORATION);
            uint32_t time = findNode(restoration, S_TXT_TIME_LABEL, 14);
            Frame g;
            uint32_t fmt = textData(g, TEXT_RAID_MMSS);
            uint32_t buf = stringBuffer(g, {textDataStr(fmt), uint32_t(feverLeft / 60), uint32_t(feverLeft % 60)});
            shadowText(time, buf);
            sharedRelease(fmt);
        } else {
            setVisible(restoration, 0);
        }
    }
    sharedRelease(enemyRow);
    return left > 0 ? 1 : 0;
}

// The raid row's AP cost: with a campaign point for the raid the campaign armature is hidden; without one the AP
// panel switches to the AP2 image, the armature shows and plays, and the AP number moves by half the panel's size
// change. The number shows the campaign point. No raid (id 0): nothing. Returns the stack-guard difference (0).
uint32_t setRaidTitle(uint32_t widget, const RaidInfo *info, uint32_t armature) {
    if (uint64_t(info->raidId) == 0) return 0;
    uint32_t api = call(lib::APIManager_getInstance, {});
    int32_t point = int32_t(call(lib::getCampaignPoint, {uint32_t(int32_t(info->useAp)), 4,
                                                         uint32_t(int32_t(info->parts[0].raidEnemyId)),
                                                         api + API_CAMPAIGNS, 7, 0}));
    float dx = 0.0f, dy = 0.0f;
    if (point != 0) {
        setVisible(armature, 0);
    } else {
        uint32_t panel = findNode(widget, S_PANEL_AP, 8);
        Frame g;
        uint32_t before = g.alloc(8);
        call(lib::Size_copy, {before, vcall(panel, node::getContentSize, {panel})});
        {
            Frame h;
            uint32_t s = newStr(h, S_AP2_PNG, 26);
            call(lib::ImageView_loadTexture, {panel, s, 0});
            strFree(s);
        }
        uint32_t after = g.alloc(8);
        call(lib::Size_copy, {after, vcall(panel, node::getContentSize, {panel})});
        setVisible(armature, 1);
        uint32_t anim = vcall(armature, VS_GET_ANIMATION, {armature});
        vcall(anim, VS_PLAY_WITH_INDEX, {anim, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
        dx = float(f2i(ldf(after) - ldf(before)) / 2);
        dy = float(f2i(ldf(after + 4) - ldf(before + 4)) / 2);
    }
    uint32_t label = findNode(widget, S_ATLASLABEL_AP, 13);
    Frame g;
    uint32_t text = g.alloc(12);
    call(lib::pointToString, {text, uint32_t(point), 0, 0});
    {
        Frame h;
        uint32_t s = newStrC(h, strData(text));
        call(lib::TextAtlas_setString, {label, s});
        strFree(s);
    }
    strFree(text);
    uint32_t pos = g.alloc(8);
    call(lib::Vec2_copy, {pos, vcall(label, node::getPosition, {label})});
    vcall(label, node::setPositionX, {label, fbits(dx + ldf(pos))});
    vcall(label, node::setPositionY, {label, fbits(dy + ldf(pos + 4))});
    call(lib::Vec2_dtor, {pos});
    return 0;
}

}  // namespace AdvStageScrollViewRaid

RW_FUNCTION(0x56A678, "AdvStageScrollView::setBossInfo(Widget*, int, StartDeckEditDialog::StageType)",
            AdvStageScrollViewRaid::setBossInfo);
RW_FUNCTION(0x56B1F4, "AdvStageScrollView::setBossInfo (lambda: set a label's text with a shadow)",
            AdvStageScrollViewRaid::setLabel);
RW_FUNCTION(0x56C210, "AdvStageScrollView::setRaidBossInfo(Widget*, const RaidBase::Info&)",
            AdvStageScrollViewRaid::setRaidBossInfo);
RW_FUNCTION(0x56CBC8, "AdvStageScrollView::updateRaidBossInfoTime(Widget*, const RaidBase::Info&)",
            AdvStageScrollViewRaid::updateRaidBossInfoTime);
RW_FUNCTION(0x56F10C, "AdvStageScrollView::setRaidTitle(Widget*, const RaidBase::Info&, cocostudio::Armature*)",
            AdvStageScrollViewRaid::setRaidTitle);
