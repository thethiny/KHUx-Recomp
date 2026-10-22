// BattleResultDialog: the result screens (stage result, items obtained, clear rank, colosseum, raid).
#include "BattleResultDialog.hpp"

using namespace brd;

namespace {
// lambda __func vtables (+8)
constexpr uint32_t VT_StageLuxDone = 0xF5A32C;       // openStageResult $_2: the Lux count ended
constexpr uint32_t VT_StageTap = 0xF5A300;           // openStageResult $_3: centre button touch
constexpr uint32_t VT_StageOpened = 0xF5A224;        // openStageResult $_4: popup animation ended
constexpr uint32_t VT_ItemOk = 0xF5A040;             // openGettingItem $_13: OK button touch
constexpr uint32_t VT_ItemMoneyDone = 0xF5A014;      // openGettingItem $_14: munny count ended
constexpr uint32_t VT_ItemTap = 0xF59FE8;            // openGettingItem $_15: centre button touch
constexpr uint32_t VT_ItemOpened = 0xF59F90;         // openGettingItem $_16: popup animation ended

constexpr uint32_t SceneUtil_getCampaignPoint = 0x4741BD;   // (base, stage, stageId, const vector<int>&, kind, bool)
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t Node_removeChildByName = 0x134;   // Node vtable: removeChildByName(name, cleanup)
constexpr uint32_t Node_getLocalZOrder = 0x1C;       // Node vtable (as the result screens use it)

// strings
constexpr uint32_t STR_BattleResultSceneJson = 0xDFFA76;   // "cocostudio/publish/BattleResultScene.json"
constexpr uint32_t STR_RankUpSceneJson = 0xDFFC3C;   // "cocostudio/publish/BattleResultRankUPScene_ver103.json"
constexpr uint32_t STR_BattleWinBG_CenterUI = 0xDFFAA0;
constexpr uint32_t STR_LevelUP_Anime = 0xDFFAB5, STR_Tap_Screen_Anime = 0xDFFAC3;
constexpr uint32_t STR_AtlasLabel_NumL = 0xDFD2A4;
constexpr uint32_t STR_LevelUpLwf = 0xDFFAD4;        // "lwf/battleresult/Result_LevelUP/Result_LevelUP.lwf"
constexpr uint32_t STR_Txt_Next = 0xDFFB07, STR_Caution_Label01 = 0xDFFB10, STR_Plate_Next = 0xDFFB20;
constexpr uint32_t STR_Tap_Screen_Animation = 0xDFFB2B;
constexpr uint32_t STR_Txt_LUX = 0xDFEA5A, STR_Txt_LUX_Label = 0xDFEA62;
constexpr uint32_t STR_empty = 0xDF8A1A;
constexpr uint32_t STR_CenterButton = 0xDFFB40;
constexpr uint32_t STR_Button_Ok = 0xDF9564, STR_Txt_OK = 0xDF869F;
constexpr uint32_t STR_Line_Money = 0xDFFC73, STR_LankUP = 0xDFFC7E, STR_Txt_Money = 0xDFE7A3;
constexpr uint32_t STR_Txt_GetItem = 0xDFFC85, STR_Txt_Money_Label = 0xDFE7AD, STR_Txt_NoItem = 0xDFFC91;

uint32_t api() { return call(g::APIManager_getInstance, {}); }
uint32_t smgr() { return call(g::SceneManager_getInstance, {}); }
uint32_t appState() { return call(g::AppState_getInstance, {}); }
void sbRead(uint32_t sb, uint32_t dst, uint32_t index, uint32_t n) {
    call(g::SafeBuffer_read, {sb, dst, ld32(sb + 12) + index, n, index});
}
void audioLoad(uint32_t id) { call(g::Audio_load, {call(g::Audio_getInstance, {}), id}); }

// Build the screen: the cocostudio scene into root (minus its "BattleWinBG_CenterUI"), above the parent.
void loadScreen(BattleResultDialog *self, uint32_t json, uint32_t len) {
    {
        TempStr s(json, len);
        self->root = SceneUtil::createNodeWithSceneFile(s);
    }
    uint32_t root = self->root;
    {
        TempStr s(STR_BattleWinBG_CenterUI, 0x14);
        vcall(root, Node_removeChildByName, {root, s, 1});
    }
    uint32_t parent = self->parent;
    uint32_t z = vcall(parent, Node_getLocalZOrder, {parent});
    vcall(parent, node::addChildZ, {parent, self->root, z + 1});
}

// The stage being finished (by battle kind: story, event, raid): its master::Stage row (shared_ptr kept in
// stageObj / stageCtrl), its id, and the campaign ids of the stage start copied into `campaigns`. The stage result
// also collects the skill level ups of the battle deck.
struct StageInfo {
    uint32_t stageObj = 0, stageCtrl = 0, stageId = 0;
};
StageInfo selectStage(BattleResultDialog *self, uint32_t campaigns, uint32_t property, bool withSkills) {
    StageInfo info;
    uint32_t kind = ld32(smgr() + SM_BATTLE_KIND);
    uint32_t startOffset, deckOffset, listOffset;
    if (kind == 0) {
        startOffset = API_STAGE_START; deckOffset = 0x40; listOffset = 0x58;
    } else if (kind == 2) {
        startOffset = API_EVENT_START; deckOffset = 0x38; listOffset = 0x40;
    } else if (kind == 3) {
        startOffset = API_RAID_START; deckOffset = 0x40; listOffset = 0x68;
    } else {
        return info;
    }
    uint32_t holder = api() + startOffset;
    uint32_t start = ld32(holder);
    if (withSkills) {
        BattleResultDialog::initSkill(self, property,
                                      uint64_t(ld32(start + deckOffset + 4)) << 32 | ld32(start + deckOffset));
    }
    Frame f;
    uint32_t stage = f.alloc(8);
    call(g::Stage_create, {stage, ld32(start + 0x30)});
    info.stageObj = ld32(stage);
    info.stageCtrl = ld32(stage + 4);
    info.stageId = ld32(start + 0x30);
    uint32_t src = ld32(holder) + listOffset;
    if (campaigns != src) call(g::IntVector_assign, {campaigns, ld32(src), ld32(src + 4)});
    return info;
}

// the campaign bonus on a value read from the user data (+userIndex): getCampaignPoint(value, stage +0x11C, ...)
int32_t campaignPoint(uint32_t userIndex, const StageInfo &info, uint32_t campaigns, uint32_t target) {
    Frame f;
    uint32_t buf = f.alloc(8);
    sbRead(ld32(appState() + APPSTATE_USER), buf, userIndex, 4);
    sbRead(info.stageObj, buf + 4, 0x11C, 4);
    return int32_t(call(SceneUtil_getCampaignPoint, {ld32(buf), ld32(buf + 4), info.stageId, campaigns, target, 1}));
}

// A transparent centre button over the whole screen (ImageButton "CenterButton" targeting the shadow).
uint32_t addCenterButton(BattleResultDialog *self, uint32_t touchVtable) {
    uint32_t button;
    {
        TempStr s(STR_empty, 0);
        button = call(g::ImageButton_create, {s, 0});
    }
    call(g::ImageButton_setTargetView, {button, self->shadow, rw::fbits(1.0f)});
    uint32_t root = self->root;
    {
        TempStr s(STR_CenterButton, 0xC);
        vcall(root, node::addChildZName, {root, button, 0, s});
    }
    {
        Frame f;
        uint32_t fn = newFunction(f, touchVtable, {addr(self)});
        call(g::ImageButton_addTouchEventListener, {button, fn});
        functionDestroy(fn);
    }
    return button;
}

// root runs the open popup animation (eased), then the lambda `vtable` [self]
void runOpenAnimation(BattleResultDialog *self, uint32_t vtable) {
    uint32_t root = self->root;
    Frame f;
    uint32_t fn = newFunction(f, vtable, {addr(self)});
    uint32_t anim = call(SceneUtil_createOpenPopupAnimation, {root, fn});
    call(g::Node_runAction, {root, call(g::EaseSineOut_create, {anim})});
    functionDestroy(fn);
}

// a node's position (getPosition) copied into a guest Vec2
void copyPosition(uint32_t root, uint32_t name, uint32_t len, uint32_t out) {
    TempStr s(name, len);
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    call(g::Vec2_copy, {out, vcall(node, node::getPosition, {node})});
}

// The Lux rank parts shared by the stage and colosseum results: the "level up" LWF and the tap animation take
// the places of their placeholders (positions copied into levelUpPos / tapPos), the Lux rank in the big number
// atlas, the "to the next level" text and plate (hidden).
void buildLevelUpParts(BattleResultDialog *self, uint32_t levelUpPos, uint32_t tapPos) {
    copyPosition(self->root, STR_LevelUP_Anime, 0xD, levelUpPos);
    copyPosition(self->root, STR_Tap_Screen_Anime, 0x10, tapPos);
    {
        uint32_t root = self->root;
        TempStr s(STR_LevelUP_Anime, 0xD);
        vcall(root, Node_removeChildByName, {root, s, 1});
    }
    {
        uint32_t root = self->root;
        TempStr s(STR_Tap_Screen_Anime, 0x10);
        vcall(root, Node_removeChildByName, {root, s, 1});
    }
    // the Lux rank in the big number atlas
    uint32_t atlas = find(self->root, STR_AtlasLabel_NumL, 0xF);
    SceneUtil::setAdditionalKerningForAtlasLabel(atlas, -21);
    {
        Frame g2;
        uint32_t s = g2.alloc(12);
        SceneUtil::numberToAtlasString(s, self->luxRank);
        call(g::TextAtlas_setString, {atlas, s});
        strFree(s);
    }
    self->levelUpLwf = call(g::LWFNode_create, {STR_LevelUpLwf, 0});
    {
        uint32_t root = self->root, lwf = self->levelUpLwf;
        uint32_t z = vcall(root, Node_getLocalZOrder, {root});
        vcall(root, node::addChildZ, {root, lwf, z + 1});
        vcall(self->levelUpLwf, node::setPosition, {self->levelUpLwf, levelUpPos});
        call(g::Movie_Stop, {ld32(ld32(self->levelUpLwf + 992) + 0x18)});
        setVisible(self->levelUpLwf, 0);
    }
    // "to the next level" text and its plate (hidden until the gauge reports)
    uint32_t next = find(self->root, STR_Txt_Next, 8);
    {
        TempStr s(STR_Caution_Label01, 0xF);
        self->nextText = SceneUtil::textToCustomTextView(next, s);
    }
    self->nextPlate = find(self->root, STR_Plate_Next, 0xA);
    if (self->nextPlate) setVisible(self->nextPlate, 0);
    {
        TempStr s(STR_Tap_Screen_Animation, 0x14);
        self->tapAnimation = call(g::Armature_create, {s});
    }
    {
        uint32_t root = self->root, tap = self->tapAnimation;
        uint32_t z = vcall(root, Node_getLocalZOrder, {root});
        vcall(root, node::addChildZ, {root, tap, z + 1});
        vcall(self->tapAnimation, node::setPosition, {self->tapAnimation, tapPos});
        setVisible(self->tapAnimation, 0);
    }
}
}  // namespace

// ------------------------------------------------------------------------------------------- stage result
// Lux gained (counted up with the campaign bonus) on the Lux gauge, the Lux rank, sub missions, skill level ups.
bool BattleResultDialog::openStageResult(BattleResultDialog *self, uint32_t parent) {
    if (!openParentChack(self, parent)) return false;
    loadScreen(self, STR_BattleResultSceneJson, 0x29);
    // Lux rank and Lux before the battle (the clear result)
    uint32_t clear = ld32(api() + API_STAGE_CLEAR);
    Frame f;
    uint32_t buf = f.alloc(8), campaigns = f.alloc(12), levelUpPos = f.alloc(8), tapPos = f.alloc(8);
    sbRead(ld32(clear + 0x5C), buf, 8, 4);
    self->luxRank = int32_t(ld32(buf));
    sbRead(ld32(clear + 0x54), buf, 8, 8);
    self->luxHi = ld32(buf + 4);
    self->luxLo = ld32(buf);
    setTitleLayer(self, 0);
    setSubMisstonLayer(self);
    st32(campaigns, 0); st32(campaigns + 4, 0); st32(campaigns + 8, 0);
    StageInfo stage = selectStage(self, campaigns, clear + 0x54, true);
    buildLevelUpParts(self, levelUpPos, tapPos);
    audioLoad(509000130);
    audioLoad(509100420);
    audioLoad(509000450);
    audioLoad(509000460);
    titleLabel(self->root, STR_Txt_LUX, 7, 103100040);
    uint32_t luxLabel = find(self->root, STR_Txt_LUX_Label, 0xD);
    SceneUtil::setShadow(luxLabel);
    // an unlabelled counter over the user value +0x18, then the Lux counter (2 s each)
    {
        sbRead(ld32(appState() + APPSTATE_USER), buf, 0x18, 4);
        Frame g2;
        uint32_t none = newEmptyFunction(g2);
        ResultPointLayer::set(&self->points0, 0, uint64_t(ld32(buf)), 2.0f, none);
        functionDestroy(none);
    }
    {
        int32_t lux = campaignPoint(0x10, stage, campaigns, 10);
        Frame g2;
        uint32_t done = newFunction(g2, VT_StageLuxDone, {addr(self)});
        ResultPointLayer::set(&self->luxPoints, luxLabel, uint64_t(int64_t(lux)), 2.0f, done);
        functionDestroy(done);
    }
    ResultPointLayer::updateLabel(&self->luxPoints, 1);
    updateLuxGauge(self);
    addCenterButton(self, VT_StageTap);
    runOpenAnimation(self, VT_StageOpened);
    call(g::Vec2_dtor, {tapPos});
    call(g::Vec2_dtor, {levelUpPos});
    call(g::IntVector_dtor, {campaigns});
    if (stage.stageCtrl) call(fn::shared_release, {stage.stageCtrl});
    return true;
}
RW_FUNCTION(0x57E04C, "BattleResultDialog::openStageResult", BattleResultDialog::openStageResult);

// ------------------------------------------------------------------------------------------- items obtained
// Munny gained (with the campaign bonus) and the items list; "no items" when nothing dropped.
bool BattleResultDialog::openGettingItem(BattleResultDialog *self, uint32_t parent) {
    if (!openParentChack(self, parent)) return false;
    loadScreen(self, STR_RankUpSceneJson, 0x36);
    setTitleLayer(self, 0);
    uint32_t ok = find(self->root, STR_Button_Ok, 9);
    titleLabel(ok, STR_Txt_OK, 6, 100100001);
    addTouch(ok, VT_ItemOk, {addr(self)});
    setVisible(ok, 0);
    audioLoad(509000140);
    {
        TempStr s(STR_Line_Money, 0xA);
        setVisible(SceneUtil::searchNodeByName(self->root, s), 1);
    }
    {
        TempStr s(STR_LankUP, 6);
        setVisible(SceneUtil::searchNodeByName(self->root, s), 0);
    }
    titleLabel(self->root, STR_Txt_Money, 9, 100300001);
    titleLabel(self->root, STR_Txt_GetItem, 0xB, 103100016);
    Frame f;
    uint32_t campaigns = f.alloc(12);
    st32(campaigns, 0); st32(campaigns + 4, 0); st32(campaigns + 8, 0);
    StageInfo stage = selectStage(self, campaigns, 0, false);
    uint32_t moneyLabel = find(self->root, STR_Txt_Money_Label, 0xF);
    {
        int32_t money = campaignPoint(0x1C, stage, campaigns, 5);
        Frame g2;
        uint32_t done = newFunction(g2, VT_ItemMoneyDone, {addr(self)});
        ResultPointLayer::set(&self->points3, moneyLabel, uint64_t(int64_t(money)), 2.0f, done);
        functionDestroy(done);
    }
    ResultPointLayer::updateLabel(&self->points3, 1);
    setResultItemLayer(self);
    uint32_t results = call(g::ScrollView_loadResultData, {self->scrollView});
    uint32_t stageResults = call(g::ScrollView_loadStageResultData, {self->scrollView});
    addCenterButton(self, VT_ItemTap);
    uint32_t noItem = find(self->root, STR_Txt_NoItem, 0xA);
    if (results + stageResults == 0) {
        SceneUtil::setShadow(noItem);
        setTextId(noItem, 103100047);
    } else {
        setVisible(noItem, 0);
    }
    audioLoad(509000060);
    audioLoad(509100420);
    audioLoad(509000430);
    runOpenAnimation(self, VT_ItemOpened);
    call(g::IntVector_dtor, {campaigns});
    if (stage.stageCtrl) call(fn::shared_release, {stage.stageCtrl});
    return true;
}
RW_FUNCTION(0x581804, "BattleResultDialog::openGettingItem", BattleResultDialog::openGettingItem);

// ------------------------------------------------------------------------------------------- clear rank
namespace {
constexpr uint32_t VT_RankOpened = 0xF59EE0;         // openClearRank $_19: popup animation ended
constexpr uint32_t VT_RankTap = 0xF59F38;            // openClearRank $_18: centre button touch
constexpr uint32_t VT_RankOk = 0xF59F64;             // openClearRank $_17: OK button touch
constexpr uint32_t ColosseumStage_create = 0x77CF95; // master::ColosseumStage::create(shared_ptr out, id)
constexpr uint32_t Colosseum_create = 0x77C4E9;      // master::Colosseum::create(shared_ptr out, id)
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;
constexpr uint32_t API_COLOSSEUM_STAGE = 0x24C;      // APIManager: -> colosseum stage id
constexpr uint32_t API_COLOSSEUM_RANKS = 0x268;      // APIManager: -> vector<pair<int,int>> (rank, ...)
constexpr uint32_t STR_AtlasLabel_Num1 = 0xDFFC9C, STR_AtlasLabel_Num2 = 0xDFFCAC;
constexpr uint32_t STR_d = 0xDF86DE;                 // "%d"

// atlas->setString(printf("%d", value))
void setAtlasNumber(uint32_t atlas, int32_t value) {
    Frame f;
    uint32_t buf = stringBuffer(f, {STR_d, uint32_t(value)});
    TempStr s(buf, host_strlen(buf));
    call(g::TextAtlas_setString, {atlas, s});
}
}  // namespace

// The colosseum rank before and after (one more when the last fight was the rank's final colosseum entry), and
// the colosseum rewards.
bool BattleResultDialog::openClearRank(BattleResultDialog *self, uint32_t parent) {
    if (!openParentChack(self, parent)) return false;
    loadScreen(self, STR_RankUpSceneJson, 0x36);
    setTitleLayer(self, 0);
    {
        TempStr s(STR_Line_Money, 0xA);
        setVisible(SceneUtil::searchNodeByName(self->root, s), 0);
    }
    {
        TempStr s(STR_LankUP, 6);
        setVisible(SceneUtil::searchNodeByName(self->root, s), 1);
    }
    titleLabel(self->root, STR_Txt_GetItem, 0xB, 103100016);
    uint32_t stageIdRef = ld32(api() + API_COLOSSEUM_STAGE);
    uint32_t ranks = ld32(api() + API_COLOSSEUM_RANKS);
    uint32_t first = ld32(ranks);
    uint32_t count = (ld32(ranks + 4) - first) >> 3;
    if (count == 0) call(Vector_throwOutOfRange, {});
    uint32_t lastIndex = count - 1;
    if (lastIndex >= count) call(Vector_throwOutOfRange, {});
    Frame f;
    uint32_t stage = f.alloc(8), entry = f.alloc(8), buf = f.alloc(8);
    call(ColosseumStage_create, {stage, ld32(stageIdRef)});
    uint32_t last = first + 8 * lastIndex;
    int32_t rankAfter = int32_t(ld32(last));
    int32_t rankBefore = int32_t(ld32(first));
    // the colosseum entries of the stage (+0x1C first id .. +0x20 last id)
    sbRead(ld32(stage), buf, 0x1C, 4);
    for (int32_t id = int32_t(ld32(buf));; id++) {
        sbRead(ld32(stage), buf + 4, 0x20, 4);
        if (id > int32_t(ld32(buf + 4))) break;
        call(Colosseum_create, {entry, uint32_t(id)});
        sbRead(ld32(entry), buf + 4, 8, 4);
        if (ld32(buf + 4) == ld32(last)) {
            sbRead(ld32(entry), buf + 4, 0x10, 4);
            if (ld32(buf + 4) == ld32(last + 4)) rankAfter = int32_t(ld32(last)) + 1;
        }
        sharedRelease(entry);
    }
    uint32_t rankUp = find(self->root, STR_LankUP, 6);
    uint32_t num1 = find(rankUp, STR_AtlasLabel_Num1, 0xF);
    uint32_t num2 = find(rankUp, STR_AtlasLabel_Num2, 0xF);
    SceneUtil::setAdditionalKerningForAtlasLabel(num1, -10);
    SceneUtil::setAdditionalKerningForAtlasLabel(num2, -14);
    setAtlasNumber(num1, rankBefore);
    setAtlasNumber(num2, rankAfter);
    setVisible(rankUp, rankBefore < rankAfter ? 1 : 0);
    setResultItemLayer(self);
    uint32_t results = call(g::ScrollView_loadColosseumResultData, {self->scrollView});
    uint32_t ok = find(self->root, STR_Button_Ok, 9);
    titleLabel(ok, STR_Txt_OK, 6, 100100001);
    addTouch(ok, VT_RankOk, {addr(self)});
    setVisible(ok, 0);
    addCenterButton(self, VT_RankTap);
    uint32_t noItem = find(self->root, STR_Txt_NoItem, 0xA);
    if (results) {
        setVisible(noItem, 0);
    } else {
        SceneUtil::setShadow(noItem);
        setTextId(noItem, 103100047);
    }
    audioLoad(509100420);
    runOpenAnimation(self, VT_RankOpened);
    sharedRelease(stage);
    return true;
}
RW_FUNCTION(0x5821DC, "BattleResultDialog::openClearRank", BattleResultDialog::openClearRank);

// ------------------------------------------------------------------------------------------- colosseum result
namespace {
constexpr uint32_t VT_ColoLuxDone = 0xF5A0F0;        // openColosseumResult $_10: the Lux count ended
constexpr uint32_t VT_ColoTap = 0xF5A0C4;            // openColosseumResult $_11: centre button touch
constexpr uint32_t VT_ColoOpened = 0xF5A06C;         // openColosseumResult $_12: popup animation ended
constexpr uint32_t STR_ColosseumSceneJson = 0xDFFC09;   // "cocostudio/publish/BattleResultColosseumScene.json"
constexpr uint32_t API_COLOSSEUM_RESULT = 0x268;     // APIManager: colosseum clear result (+0x34 Property:
                                                     // Lux SafeBuffer, +0x3C rank SafeBuffer)
}  // namespace

// The colosseum result: the Lux screen of the stage result; when the user data flag (+8) is set the battle was
// an event colosseum fight: Lux and rank from its clear result, skill level ups, and the Lux with the campaign
// bonus of the event stage.
bool BattleResultDialog::openColosseumResult(BattleResultDialog *self, uint32_t parent) {
    if (!openParentChack(self, parent)) return false;
    loadScreen(self, STR_ColosseumSceneJson, 0x32);
    Frame f;
    uint32_t buf = f.alloc(24), levelUpPos = f.alloc(8), tapPos = f.alloc(8);
    sbRead(ld32(appState() + APPSTATE_USER), buf, 8, 4);
    uint32_t flag = ld32(buf);
    uint32_t a = api();
    uint32_t luxBuffer;
    if (flag) {
        uint32_t result = ld32(a + API_COLOSSEUM_RESULT);
        sbRead(ld32(result + 0x3C), buf, 8, 4);
        self->luxRank = int32_t(ld32(buf));
        luxBuffer = ld32(result + 0x34);
    } else {
        uint32_t a2 = api();
        sbRead(ld32(a + 0x94), buf, 8, 4);
        self->luxRank = int32_t(ld32(buf));
        luxBuffer = ld32(a2 + 0x8C);
    }
    sbRead(luxBuffer, buf, 8, 8);
    self->luxHi = ld32(buf + 4);
    self->luxLo = ld32(buf);
    setTitleLayer(self, 0);
    sbRead(ld32(appState() + APPSTATE_USER), buf, 8, 4);
    if (ld32(buf)) {
        uint32_t start = ld32(api() + API_EVENT_START);
        uint32_t property = ld32(api() + API_COLOSSEUM_RESULT) + 0x34;
        initSkill(self, property, uint64_t(ld32(start + 0x3C)) << 32 | ld32(start + 0x38));
    }
    buildLevelUpParts(self, levelUpPos, tapPos);
    audioLoad(509000130);
    audioLoad(509100420);
    audioLoad(509000450);
    titleLabel(self->root, STR_Txt_LUX, 7, 103100040);
    sbRead(ld32(appState() + APPSTATE_USER), buf, 8, 4);
    int32_t base = 0, lux = 0;
    if (ld32(buf)) {
        uint32_t holder = api() + API_EVENT_START;
        uint32_t stage = f.alloc(8);
        call(g::Stage_create, {stage, ld32(ld32(holder) + 0x30)});
        sbRead(ld32(appState() + APPSTATE_USER), buf + 4, 0x18, 4);
        base = int32_t(ld32(buf + 4));
        sbRead(ld32(appState() + APPSTATE_USER), buf + 8, 0x10, 4);
        sbRead(ld32(stage), buf + 12, 0x11C, 4);
        uint32_t start = ld32(holder);
        lux = int32_t(call(SceneUtil_getCampaignPoint,
                           {ld32(buf + 8), ld32(buf + 12), ld32(start + 0x30), start + 0x40, 10, 1}));
        sharedRelease(stage);
    }
    uint32_t luxLabel = find(self->root, STR_Txt_LUX_Label, 0xD);
    SceneUtil::setShadow(luxLabel);
    {
        Frame g2;
        uint32_t none = newEmptyFunction(g2);
        ResultPointLayer::set(&self->points0, 0, uint64_t(int64_t(base)), 2.0f, none);
        functionDestroy(none);
    }
    {
        Frame g2;
        uint32_t done = newFunction(g2, VT_ColoLuxDone, {addr(self)});
        ResultPointLayer::set(&self->luxPoints, luxLabel, uint64_t(int64_t(lux)), 2.0f, done);
        functionDestroy(done);
    }
    ResultPointLayer::updateLabel(&self->luxPoints, 1);
    updateLuxGauge(self);
    addCenterButton(self, VT_ColoTap);
    runOpenAnimation(self, VT_ColoOpened);
    call(g::Vec2_dtor, {tapPos});
    call(g::Vec2_dtor, {levelUpPos});
    return true;
}
RW_FUNCTION(0x580D7C, "BattleResultDialog::openColosseumResult", BattleResultDialog::openColosseumResult);

// ------------------------------------------------------------------------------------------- raid result
namespace {
constexpr uint32_t VT_RaidDamageDone = 0xF5A1F8;     // openRaidResult $_5: the damage count ended
constexpr uint32_t VT_RaidMoneyDone = 0xF5A1CC;      // openRaidResult $_6: the munny count ended
constexpr uint32_t VT_RaidLuxDone = 0xF5A1A0;        // openRaidResult $_7: the Lux count ended
constexpr uint32_t VT_RaidTap = 0xF5A174;            // openRaidResult $_8: centre button touch
constexpr uint32_t VT_RaidOpened = 0xF5A11C;         // openRaidResult $_9 [this, lwf kind]: popup animation ended
constexpr uint32_t STR_RaidWinSceneJson = 0xDFFB95;  // "cocostudio/publish/BattleResultReidScene.json"
constexpr uint32_t STR_RaidLoseSceneJson = 0xDFFB4D; // "cocostudio/publish/BattleResultDefeatedScene.json"
constexpr uint32_t STR_BattleLoseBG_CenterUI = 0xDFFB7F;
constexpr uint32_t STR_Line = 0xDFFBC3, STR_Txt_Wording_Top = 0xDFFBC8, STR_Txt_StageTitle_Label = 0xDFFBD8;
constexpr uint32_t STR_Txt_Wording = 0xDF862F, STR_Txt_Damage = 0xDFFBED, STR_Txt_Damage_Label = 0xDFFBF8;
constexpr uint32_t API_RAID_CLEAR = 0x180;           // raid clear result: +4 the boss was beaten by this user (byte),
                                                     // +0x20 Property (Lux SafeBuffer), +0x28 rank SafeBuffer
constexpr uint32_t API_USER_ID = 0x74;               // SafeBuffer: the user id (uint64 at 0)
// the raid start (a SafeBuffer at its head, 0xA8 bytes): +0x2C stage id (decoded), +0x40 deck id,
// +0x60 the user id of the one who found the boss, +0x68 campaign ids
constexpr uint32_t RAID_START_SIZE = 0xA8;
}  // namespace

// The raid result: won (the boss beaten, the "Reid" scene) or not (the defeated scene), titled by the raid time
// status (1 beaten, 2 escaped, 3 none, 4 and 5 time over); damage dealt, munny and Lux (both with the campaign
// bonus of the raid stage) counted up, skill level ups.
bool BattleResultDialog::openRaidResult(BattleResultDialog *self, uint32_t parent, uint32_t timeStatus) {
    if (!openParentChack(self, parent)) return false;
    Frame f;
    uint32_t buf = f.alloc(8), start = f.alloc(RAID_START_SIZE), levelUpPos = f.alloc(8), tapPos = f.alloc(8);
    sbRead(ld32(appState() + APPSTATE_USER), buf, 8, 4);
    const uint32_t won = ld32(buf);
    uint32_t kind;
    if (won) {
        {
            TempStr s(STR_RaidWinSceneJson, 0x2D);
            self->root = SceneUtil::createNodeWithSceneFile(s);
        }
        uint32_t root = self->root;
        TempStr s(STR_BattleWinBG_CenterUI, 0x14);
        vcall(root, Node_removeChildByName, {root, s, 1});
        kind = LWF_RAID_CLEAR;
    } else {
        {
            TempStr s(STR_RaidLoseSceneJson, 0x31);
            self->root = SceneUtil::createNodeWithSceneFile(s);
        }
        uint32_t root = self->root;
        TempStr s(STR_BattleLoseBG_CenterUI, 0x15);
        vcall(root, Node_removeChildByName, {root, s, 1});
        kind = LWF_RAID_DEFEATED;
    }
    {
        uint32_t root = self->root, shadow = self->shadow;
        uint32_t z = vcall(shadow, Node_getLocalZOrder, {shadow});
        vcall(shadow, node::addChildZ, {shadow, root, z + 1});
    }
    // Lux rank and Lux before the battle (the raid clear result)
    const uint32_t clear = ld32(api() + API_RAID_CLEAR);
    sbRead(ld32(clear + 40), buf, 8, 4);
    self->luxRank = int32_t(ld32(buf));
    sbRead(ld32(clear + 32), buf, 8, 8);
    self->luxHi = ld32(buf + 4);
    self->luxLo = ld32(buf);
    setTitleLayer(self, 0);
    const uint32_t raidStart = ld32(api() + API_RAID_START);
    call(g::SafeBuffer_read, {raidStart, start, ld32(raidStart + 12), RAID_START_SIZE, 0});
    const uint32_t stageId = ld32(start + 0x2C);
    // the title: the "Txt_Wording_Top" line on a win (the separator "Line" hidden), else the stage title
    uint32_t line = 0, title;
    if (won) {
        line = find(self->root, STR_Line, 4);
        if (line) setVisible(line, 0);
        title = find(self->root, STR_Txt_Wording_Top, 0xF);
        setVisible(title, 1);
    } else {
        title = find(self->root, STR_Txt_StageTitle_Label, 0x14);
    }
    const uint32_t wording = find(self->root, STR_Txt_Wording, 0xB);
    SceneUtil::setShadow(wording);
    setVisible(wording, 1);
    switch (timeStatus) {
    case 1: {
        setTextId(title, 103200016);
        // the finder line when the finder is this user and the boss was beaten by them
        const uint32_t rs = ld32(api() + API_RAID_START);
        const uint32_t lo = ld32(rs + 96), hi = ld32(rs + 100);
        sbRead(ld32(api() + API_USER_ID), buf, 0, 8);
        if (ld32(buf) != lo || ld32(buf + 4) != hi || !ld8(ld32(api() + API_RAID_CLEAR) + 4)) {
            setVisible(wording, 0);
            break;
        }
        setTextId(wording, 103200019);
        setVisible(wording, 1);
        break;
    }
    case 2:
        setTextId(title, 103200011);
        setVisible(wording, 0);
        break;
    case 3:
        if (line) setVisible(line, 1);
        setVisible(title, 0);
        setVisible(wording, 0);
        break;
    case 4:
        setTextId(title, 103200010);
        setTextId(wording, 103200031);
        break;
    case 5:
        setTextId(title, 103200032);
        setTextId(wording, 103200031);
        break;
    default:
        break;
    }
    {
        const uint32_t rs = ld32(api() + API_RAID_START);
        initSkill(self, clear + 32, uint64_t(ld32(rs + 0x44)) << 32 | ld32(rs + 0x40));
    }
    buildLevelUpParts(self, levelUpPos, tapPos);
    audioLoad(509000130);
    audioLoad(509100420);
    audioLoad(509000450);
    titleLabel(self->root, STR_Txt_Damage, 0xA, 103200017);
    titleLabel(self->root, STR_Txt_Money, 9, 100300001);
    titleLabel(self->root, STR_Txt_LUX, 7, 103100040);
    const uint32_t damageLabel = find(self->root, STR_Txt_Damage_Label, 0x10);
    SceneUtil::setShadow(damageLabel);
    const uint32_t moneyLabel = find(self->root, STR_Txt_Money_Label, 0xF);
    SceneUtil::setShadow(moneyLabel);
    const uint32_t luxLabel = find(self->root, STR_Txt_LUX_Label, 0xD);
    SceneUtil::setShadow(luxLabel);
    const uint32_t campaigns = ld32(api() + API_RAID_START) + 0x68;
    // damage dealt (user +0x30)
    {
        sbRead(ld32(appState() + APPSTATE_USER), buf, 0x30, 4);
        Frame g2;
        uint32_t done = newFunction(g2, VT_RaidDamageDone, {addr(self)});
        ResultPointLayer::set(&self->points2, damageLabel, uint64_t(ld32(buf)), 2.0f, done);
        functionDestroy(done);
    }
    // munny (user +0x1C) with the campaign bonus
    {
        sbRead(ld32(appState() + APPSTATE_USER), buf, 0x1C, 4);
        int32_t money = int32_t(call(SceneUtil_getCampaignPoint, {ld32(buf), 4, stageId, campaigns, 5, 1}));
        Frame g2;
        uint32_t done = newFunction(g2, VT_RaidMoneyDone, {addr(self)});
        ResultPointLayer::set(&self->points3, moneyLabel, uint64_t(int64_t(money)), 2.0f, done);
        functionDestroy(done);
    }
    // an unlabelled counter over the user value +0x18
    {
        sbRead(ld32(appState() + APPSTATE_USER), buf, 0x18, 4);
        Frame g2;
        uint32_t none = newEmptyFunction(g2);
        ResultPointLayer::set(&self->points0, 0, uint64_t(ld32(buf)), 2.0f, none);
        functionDestroy(none);
    }
    // Lux (user +0x10) with the campaign bonus
    {
        sbRead(ld32(appState() + APPSTATE_USER), buf, 0x10, 4);
        int32_t lux = int32_t(call(SceneUtil_getCampaignPoint, {ld32(buf), 4, stageId, campaigns, 10, 1}));
        Frame g2;
        uint32_t done = newFunction(g2, VT_RaidLuxDone, {addr(self)});
        ResultPointLayer::set(&self->luxPoints, luxLabel, uint64_t(int64_t(lux)), 2.0f, done);
        functionDestroy(done);
    }
    ResultPointLayer::updateLabel(&self->points2, 1);
    ResultPointLayer::updateLabel(&self->points3, 1);
    ResultPointLayer::updateLabel(&self->luxPoints, 1);
    updateLuxGauge(self);
    addCenterButton(self, VT_RaidTap);
    {
        uint32_t root = self->root;
        Frame g2;
        uint32_t fn = newFunction(g2, VT_RaidOpened, {addr(self), kind});
        uint32_t anim = call(SceneUtil_createOpenPopupAnimation, {root, fn});
        call(g::Node_runAction, {root, call(g::EaseSineOut_create, {anim})});
        functionDestroy(fn);
    }
    call(g::Vec2_dtor, {tapPos});
    call(g::Vec2_dtor, {levelUpPos});
    return true;
}
RW_FUNCTION(0x57FAB4, "BattleResultDialog::openRaidResult", BattleResultDialog::openRaidResult);
