// BattleResultDialog: life cycle, per-frame update, the parts shared by the result screens (title, sub
// missions, Lux gauge, skill level-up popups, result LWFs). The open* screens are in BattleResultDialog_Open.cpp,
// the lambdas and their std::function thunks in BattleResultDialog_Lambdas.cpp.
#include "BattleResultDialog.hpp"

using namespace brd;

namespace {
// lambda __func vtables (+8)
constexpr uint32_t VT_InitOnEnter = 0xF5A384;        // init $_0: scheduleUpdate
constexpr uint32_t VT_InitOnExit = 0xF5A358;         // init $_1: unscheduleUpdate
constexpr uint32_t VT_LuxGauge = 0xF59E04;           // updateLuxGauge $_23 (float, uint64)
constexpr uint32_t VT_LuxLvNext = 0xF59DD8;          // updateLuxLvNext $_24 (float, uint64)
constexpr uint32_t VT_Close = 0xF59EB4;              // close $_20
constexpr uint32_t VT_SkillPopupIn = 0xF59E30;       // popUpSkillLevelUp $_22
constexpr uint32_t VT_SubMissionEnd = 0xF59E88;      // initSubMissionLwf $_21 (heap: self + std::function)

constexpr uint32_t Movie_GotoAndPlay = rwlib::LWF::Movie::GotoAndPlay__i;
constexpr uint32_t Audio_getLayer = 0x5BF33D;        // audio::Manager::getLayer(shared_ptr out, int)
constexpr uint32_t AudioLayer_playSE = 0x5BF785;     // audio::Layer::playSE(ResourceID, int, int)
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1; // __vector_base_common::__throw_out_of_range
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_createRightInAnimation = 0x45B8BD;
constexpr uint32_t floatundisf = 0xCD6D88;           // libgcc (uint64) -> float
constexpr uint32_t fixunssfdi = 0xCD7429;            // libgcc (float) -> uint64

// strings
constexpr uint32_t STR_Button_Ok = 0xDF9564;
constexpr uint32_t STR_LVAnimationJson = 0xDFFCBC;   // "cocostudio/publish/BattleResultLVAnimation.ExportJson"
constexpr uint32_t STR_TapAnimationJson = 0xDFFCF2;  // "cocostudio/publish/Tap_Screen_Animation.ExportJson"
constexpr uint32_t STR_Title_Clear = 0xDFFD25, STR_Title_Result = 0xDFFD31, STR_Line = 0xDFFBC3;
constexpr uint32_t STR_Line_Red = 0xDFFD3E, STR_Txt_StageTitle_Label = 0xDFFBD8;
constexpr uint32_t STR_empty = 0xDF8A1A;
constexpr uint32_t STR_NormalBattleWin01 = 0xDFFD47; // "NormalBattleWin01_CenterUI"
constexpr uint32_t STR_SubClear[3] = {0xDFF4B8, 0xDFF4D1, 0xDFF4EA};
constexpr uint32_t STR_Scroll_Area = 0xDF9B59;
constexpr uint32_t STR_LwfPath = 0xDFFDE0;           // "lwf/battleresult/%s/%s.lwf"
constexpr uint32_t STR_AreaPointSubClear = 0xDFFDFB; // "Area_Point_SubClear%d"
constexpr uint32_t STR_SkillJson = 0xDFFE11;         // "cocostudio/publish/BattleResult_Skill.json"
constexpr uint32_t STR_Txt_GetItem_Label = 0xDFD7BA;
constexpr uint32_t STR_AtlasLabel_NumL = 0xDFD2A4;

// sounds
constexpr uint32_t SE_RESULT_LWF = 509100420;
constexpr uint32_t SE_CLEAR_ON = 509000460;
constexpr uint32_t SE_LEVEL_UP = 509000450;
constexpr uint32_t BGM_RAID_DEFEATED = 501004019;

constexpr uint32_t LWFNODE_LWF = 992;                // LWFNode -> LWF*; LWF +0x18 root Movie
constexpr uint32_t MOVIE_TOTAL_FRAMES = 0x98;

uint32_t api() { return call(g::APIManager_getInstance, {}); }
uint32_t smgr() { return call(g::SceneManager_getInstance, {}); }
uint32_t appState() { return call(g::AppState_getInstance, {}); }

// SafeBuffer::read(sb, dst, data + index, n, index)
void sbRead(uint32_t sb, uint32_t dst, uint32_t index, uint32_t n) {
    call(g::SafeBuffer_read, {sb, dst, ld32(sb + 12) + index, n, index});
}

// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
void playSE(uint32_t id) {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(Audio_getLayer, {layer, call(g::Audio_getInstance, {}), 0});
    call(AudioLayer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
}

// base + add * elapsed / duration of a counter, through the guest's uint64 <-> float conversions
uint64_t counterValue(const ResultPointLayer &p) {
    float base = rw::callf(floatundisf, {p.baseLo, p.baseHi});
    float add = rw::callf(floatundisf, {p.addLo, p.addHi});
    return rw::call64(fixunssfdi, {rw::fbits(base + add * p.elapsed / p.duration)});
}

// libc++ std::vector<int> (on the stack) push_back as inlined: placement store or the slow path
void intPushBack(uint32_t vec, uint32_t valueSlot, int32_t value) {
    st32(valueSlot, uint32_t(value));
    uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        if (end) st32(end, uint32_t(value));
        st32(vec + 4, end + 4);
    } else {
        call(g::IntVector_pushSlow, {vec, valueSlot});
    }
}

// std::lower_bound over ints [first, last) (the inlined binary search; signed compare)
uint32_t lowerBound(uint32_t first, uint32_t last, int32_t value) {
    int32_t count = int32_t(last - first) >> 2;
    while (count) {
        int32_t half = count / 2;
        uint32_t mid = first + 4 * uint32_t(half);
        if (int32_t(ld32(mid)) < value) {
            first = mid + 4;
            count = count - 1 - half;
        } else {
            count = half;
        }
    }
    return first;
}
// std::binary_search over ints
bool contains(uint32_t first, uint32_t last, int32_t value) {
    uint32_t it = lowerBound(first, last, value);
    return it != last && !(value < int32_t(ld32(it)));
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
BattleResultDialog *BattleResultDialog::construct(BattleResultDialog *self) {
    uint32_t a = addr(self);
    call(g::Node_ctor, {a});
    self->vtable = VTABLE;
    self->parent = 0;
    self->shadow = 0;
    self->root = 0;
    self->closeEvent_f = 0;
    self->canClose = 0;
    self->skillsBegin = 0;
    self->skillsEnd = 0;
    self->skillsCap = 0;
    self->field_2BC = 0;
    self->field_2C0 = 0;
    self->scrollAnimating = 0;
    // the five counters: label, running/finished flags, values and times zeroed, no callback
    ResultPointLayer *layers[] = {&self->points0, &self->luxPoints, &self->points2, &self->points3};
    for (ResultPointLayer *p : layers) {
        p->label = 0;
        p->running = 0;
        p->finished = 0;
        p->onDone_f = 0;
        p->baseLo = 0; p->baseHi = 0; p->addLo = 0; p->addHi = 0;
        p->elapsed = 0.0f; p->duration = 0.0f;
    }
    self->subMissionDelay = 0.0f;
    self->field_3C8 = 0;
    ResultPointLayer *p = &self->points4;
    p->label = 0;
    p->running = 0;
    p->finished = 0;
    p->onDone_f = 0;
    p->baseLo = 0; p->baseHi = 0; p->addLo = 0; p->addHi = 0;
    p->elapsed = 0.0f; p->duration = 0.0f;
    self->scrollView = 0;
    self->levelUpLwf = 0;
    self->tapAnimation = 0;
    self->nextText = 0;
    self->nextPlate = 0;
    return self;
}
RW_FUNCTION(0x57DAE0, "BattleResultDialog::BattleResultDialog", BattleResultDialog::construct);

BattleResultDialog *BattleResultDialog::destruct(BattleResultDialog *self) {
    uint32_t a = addr(self);
    self->vtable = VTABLE;
    functionDestroy(addr(self->points4.onDone));
    functionDestroy(addr(self->points3.onDone));
    functionDestroy(addr(self->points2.onDone));
    functionDestroy(addr(self->luxPoints.onDone));
    functionDestroy(addr(self->points0.onDone));
    call(g::LevelUpSkillVector_dtor, {addr(&self->skillsBegin)});
    functionDestroy(addr(self->closeEvent));
    call(g::Node_dtor, {a});
    return self;
}
RW_FUNCTION(0x57D95C, "BattleResultDialog::~BattleResultDialog", BattleResultDialog::destruct);

void BattleResultDialog::destructDelete(BattleResultDialog *self) {
    destruct(self);
    call(fn::operator_delete, {addr(self)});
}
RW_FUNCTION(0x57DA7E, "BattleResultDialog::~BattleResultDialog (deleting)", BattleResultDialog::destructDelete);

BattleResultDialog *BattleResultDialog::create() {
    uint32_t a = call(fn::operator_new, {SIZE});
    construct(dialog(a));
    if (vcall(a, 0x248, {a}) == 1) {        // init()
        call(g::Ref_autorelease, {a});
        return dialog(a);
    }
    vcall(a, 4, {a});                       // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x57DA8A, "BattleResultDialog::create", BattleResultDialog::create);

bool BattleResultDialog::init(BattleResultDialog *self) {
    uint32_t a = addr(self);
    if (call(g::Node_init, {a}) != 1) return false;
    {
        // onEnter: scheduleUpdate
        Frame f;
        uint32_t fn = newFunction(f, VT_InitOnEnter, {a});
        call(g::FunctionVV_assign, {addr(self->onEnter), fn});
        functionDestroy(fn);
    }
    {
        // onExit: unscheduleUpdate
        Frame f;
        uint32_t fn = newFunction(f, VT_InitOnExit, {a});
        call(g::FunctionVV_assign, {addr(self->onExit), fn});
        functionDestroy(fn);
    }
    return true;
}
RW_FUNCTION(0x57DC44, "BattleResultDialog::init", BattleResultDialog::init);

// Count the numbers up, keep the Lux gauge in step, and enable the OK button once the item list stopped.
void BattleResultDialog::update(BattleResultDialog *self, float dt) {
    ResultPointLayer::update(&self->points2, dt, 1);
    ResultPointLayer::update(&self->points3, dt, 1);
    ResultPointLayer::update(&self->points0, dt, 1);
    ResultPointLayer::update(&self->luxPoints, dt, 1);
    if (self->luxPoints.running && !self->luxPoints.finished) updateLuxGauge(self);
    ResultPointLayer::update(&self->points4, dt, 1);
    if (self->scrollAnimating && call(g::ScrollView_updateResultAnimetion, {self->scrollView, rw::fbits(dt), 1}) == 1) {
        self->scrollAnimating = 0;
        call(g::IScrollProtocol_setEnabledScroll, {self->scrollView, 1});
        self->canClose = 1;
        uint32_t ok = find(self->root, STR_Button_Ok, 9);
        if (ok) setVisible(ok, 1);
    }
}
RW_FUNCTION(0x57DD4C, "BattleResultDialog::update", BattleResultDialog::update);

// The Lux gauge for the Lux before the battle plus what the counter shows now.
void BattleResultDialog::updateLuxGauge(BattleResultDialog *self) {
    if (!self->root) return;
    int32_t rank = self->luxRank;
    uint64_t lux = uint64_t(self->luxHi) << 32 | self->luxLo;
    Frame f;
    uint32_t fn = newFunction(f, VT_LuxGauge, {addr(self)});
    uint64_t shown = counterValue(self->luxPoints);
    SceneUtil::calcLuxGaugePer(rank, shown + lux, fn);
    functionDestroy(fn);
}
RW_FUNCTION(0x57DF20, "BattleResultDialog::updateLuxGauge", BattleResultDialog::updateLuxGauge);

// The Lux gauge with the rank and Lux after the battle (the rank only goes up).
void BattleResultDialog::updateLuxLvNext(BattleResultDialog *self) {
    if (!self->root) return;
    Frame f;
    uint32_t buf = f.alloc(8);
    uint32_t user = api();
    int32_t rank = self->luxRank;
    sbRead(ld32(user + 0x94), buf, 8, 4);
    if (rank < int32_t(ld32(buf))) {
        sbRead(ld32(user + 0x94), buf, 8, 4);
        self->luxRank = int32_t(ld32(buf));
    }
    sbRead(ld32(api() + 0x8C), buf, 8, 8);
    uint64_t lux = uint64_t(ld32(buf + 4)) << 32 | ld32(buf);
    uint32_t fn = newFunction(f, VT_LuxLvNext, {addr(self)});
    SceneUtil::calcLuxGaugePer(self->luxRank, lux, fn);
    functionDestroy(fn);
}
RW_FUNCTION(0x58379C, "BattleResultDialog::updateLuxLvNext", BattleResultDialog::updateLuxLvNext);

void BattleResultDialog::close(BattleResultDialog *self) {
    call(g::AudioPlaySystemSEDecide, {});
    uint32_t root = self->root;
    {
        Frame f;
        uint32_t fn = newFunction(f, VT_Close, {addr(self)});
        call(g::Node_runAction, {root, call(SceneUtil_createClosePopupAnimation, {root, fn})});
        functionDestroy(fn);
    }
    uint32_t shadow = self->shadow;
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {rw::fbits(0.1f), 0})});
}
RW_FUNCTION(0x582AAC, "BattleResultDialog::close", BattleResultDialog::close);

void BattleResultDialog::setCloseEvent(BattleResultDialog *self, uint32_t callback) {
    call(g::FunctionVV_assign, {addr(self->closeEvent), callback});
}
RW_FUNCTION(0x582B80, "BattleResultDialog::setCloseEvent", BattleResultDialog::setCloseEvent);

// ------------------------------------------------------------------------------------------- opening
// First open only: remember the parent, put a transparent modal shadow holding the dialog on it, and load the
// two armature animations of the result screens. false when the dialog is already open.
bool BattleResultDialog::openParentChack(BattleResultDialog *self, uint32_t parent) {
    if (self->parent) return false;
    self->parent = parent;
    self->shadow = SceneUtil::createShadowSprite();
    vcall(self->shadow, node::setOpacity, {self->shadow, 0});
    vcall(self->shadow, node::addChild, {self->shadow, addr(self)});
    vcall(self->parent, node::addChild, {self->parent, self->shadow});
    self->canClose = 0;
    uint32_t adm = call(g::ArmatureDataManager_getInstance, {});
    {
        TempStr s(STR_LVAnimationJson, 0x35);
        call(g::ArmatureDataManager_addArmatureFileInfo, {adm, s});
    }
    adm = call(g::ArmatureDataManager_getInstance, {});
    {
        TempStr s(STR_TapAnimationJson, 0x32);
        call(g::ArmatureDataManager_addArmatureFileInfo, {adm, s});
    }
    return true;
}
RW_FUNCTION(0x57EB1C, "BattleResultDialog::openParentChack", BattleResultDialog::openParentChack);

// The title area: the stage name for story/event/raid results (kinds 1, 2, 3, 7), cleared for 6 and 8. In the
// end every title part is hidden again (the title is drawn by the result LWF).
void BattleResultDialog::setTitleLayer(BattleResultDialog *self, uint32_t) {
    uint32_t clear = find(self->root, STR_Title_Clear, 11);
    uint32_t result = find(self->root, STR_Title_Result, 12);
    uint32_t line = find(self->root, STR_Line, 4);
    uint32_t lineRed = find(self->root, STR_Line_Red, 8);
    uint32_t stageTitle = find(self->root, STR_Txt_StageTitle_Label, 0x14);
    if (stageTitle) SceneUtil::setShadow(stageTitle);
    uint32_t kind = ld32(smgr() + SM_RESULT_KIND);
    bool hideClear = true;
    if (kind <= 8) {
        uint32_t bit = 1u << kind;
        if (bit & 0x8E) {
            setVisible(clear, 0);
            setVisible(result, 1);
            if (line) setVisible(line, 1);
            if (lineRed) setVisible(lineRed, 0);
            if (stageTitle) {
                // the stage name (master::Stage +8, 0x81 bytes) of the current stage
                Frame f;
                uint32_t id = f.alloc(4), name = f.alloc(0x84);
                uint32_t a = api();
                call(g::SafeBuffer_read, {a + API_STAGE_STATE, id, ld32(a + API_STAGE_STATE + 12) + 4, 4, 4});
                uint32_t stage = f.alloc(8);
                call(g::Stage_create, {stage, ld32(id)});
                sbRead(ld32(stage), name, 8, 0x81);
                {
                    TempStr s(name, host_strlen(name));
                    call(g::Text_setString, {stageTitle, s});
                }
                sharedRelease(stage);
            }
        } else if (bit & 0x140) {
            if (clear) setVisible(clear, 1);
            if (result) setVisible(result, 0);
            if (line) setVisible(line, 0);
            if (lineRed) setVisible(lineRed, 1);
            if (stageTitle) {
                TempStr s(STR_empty, 0);
                call(g::Text_setString, {stageTitle, s});
            }
        } else if (kind == 5) {
            setVisible(clear, 0);
            setVisible(result, 1);
            hideClear = false;
            setVisible(clear, 0);     // joins the hiding below past its null check of Title_Clear
        }
    }
    if (hideClear && clear) setVisible(clear, 0);
    if (result) setVisible(result, 0);
    if (line) setVisible(line, 0);
    if (lineRed) setVisible(lineRed, 0);
}
RW_FUNCTION(0x57EC64, "BattleResultDialog::setTitleLayer", BattleResultDialog::setTitleLayer);

// The three sub mission marks of the story/event stage: cleared before (user data) or now (the clear result).
void BattleResultDialog::setSubMisstonLayer(BattleResultDialog *self) {
    if ((ld32(smgr() + SM_BATTLE_KIND) & ~1u) == 2) return;
    uint32_t area = find(self->root, STR_NormalBattleWin01, 0x1A);
    Frame f;
    uint32_t buf = f.alloc(16), vec = f.alloc(12), slot = f.alloc(8), stage = f.alloc(8);
    uint32_t a = api();
    call(g::SafeBuffer_read, {a + API_STAGE_STATE, buf, ld32(a + API_STAGE_STATE + 12) + 4, 4, 4});
    uint32_t stageId = ld32(buf);
    call(g::AdvStageScrollView_setSubMisstionInfo, {area, stageId, 0});
    call(g::Stage_create, {stage, stageId});
    st32(vec, 0); st32(vec + 4, 0); st32(vec + 8, 0);
    // missions cleared before: up to three ids, -1 ends the list
    uint32_t state = appState();
    uint32_t userData = ld32(state + APPSTATE_USER), userCtrl = ld32(state + APPSTATE_USER + 4);
    if (userCtrl) call(fn::shared_add, {userCtrl});
    sbRead(userData, buf, 0x4228, 12);
    for (uint32_t k = 0; k < 3; k++) {
        int32_t m = int32_t(ld32(buf + 4 * k));
        if (m == -1) break;
        intPushBack(vec, slot, m);
    }
    // missions cleared in this battle (sorted list of the stage start)
    uint32_t start = ld32(api() + API_STAGE_START);
    for (int32_t m = 1; m <= 3; m++) {
        uint32_t first = ld32(start + 0x4C), last = ld32(start + 0x50);
        if (contains(first, last, m) && !contains(ld32(vec), ld32(vec + 4), m)) {
            intPushBack(vec, slot, m);
            call(g::IntSort, {ld32(vec), ld32(vec + 4), buf});
        }
    }
    call(g::IntSort, {ld32(vec), ld32(vec + 4), buf});
    uint32_t kind = ld32(smgr() + SM_RESULT_KIND);
    if (kind <= 7 && ((1u << kind) & 0x8E)) {
        sbRead(ld32(stage), buf, 0x1E0, 4);
        call(g::AdvStageScrollView_setSubMisstion, {area, ld32(buf), vec});
        for (uint32_t k = 0; k < 3; k++) {
            bool cleared = contains(ld32(start + 0x4C), ld32(start + 0x50), int32_t(k + 1));
            uint32_t mark = find(area, STR_SubClear[k], 9);
            if (mark) setVisible(mark, cleared ? 1 : 0);
        }
    }
    if (userCtrl) call(fn::shared_release, {userCtrl});
    call(g::IntVector_dtor, {vec});
    sharedRelease(stage);
}
RW_FUNCTION(0x57F06C, "BattleResultDialog::setSubMisstonLayer", BattleResultDialog::setSubMisstonLayer);

// The items list (a BattleResultScrollView filling "Scroll_Area"; scrolling and taps off while it animates).
void BattleResultDialog::setResultItemLayer(BattleResultDialog *self) {
    uint32_t area = find(self->root, STR_Scroll_Area, 11);
    self->scrollView = call(g::ScrollView_create, {});
    call(g::IScrollProtocol_setMaxColumn, {self->scrollView, 1});
    call(g::ScrollView_setEnabledTapItem, {self->scrollView, 0});
    uint32_t view = self->scrollView;
    vcall(view, node::setContentSize, {view, vcall(area, node::getContentSize, {area})});
    call(g::IScrollProtocol_setEnabledScroll, {self->scrollView, 0});
    vcall(area, node::addChild, {area, self->scrollView});
    call(g::ScrollView_setRareDropSpeed, {self->scrollView, rw::fbits(1.0f)});
    call(g::ScrollView_setAddRightInTime, {self->scrollView, rw::fbits(0.2f)});
    call(g::ScrollView_setDelayTime, {self->scrollView, rw::fbits(0.2f)});
    call(g::ScrollView_setListMoveTime, {self->scrollView, rw::fbits(0.2f)});
}
RW_FUNCTION(0x5820C0, "BattleResultDialog::setResultItemLayer", BattleResultDialog::setResultItemLayer);

// ------------------------------------------------------------------------------------------- skills
// Collect the skills of the battle deck's medals whose level went up (old level from the clear result's table,
// new from the user skill table), then show the first popup.
void BattleResultDialog::initSkill(BattleResultDialog *self, uint32_t property, uint64_t deckId) {
    // skills.clear()
    uint32_t end = self->skillsEnd, begin = self->skillsBegin;
    if (end != begin) self->skillsEnd = end + (~(end - 16 - begin) & ~15u);
    self->skillIndex = 0;
    self->skillPopupOpen = 0;
    Frame f;
    uint32_t deck = f.alloc(0x58), medal = f.alloc(0x48), skillBefore = f.alloc(0x28), skillNow = f.alloc(0x28);
    uint32_t key = f.alloc(8), item = f.alloc(16);
    uint32_t a1 = api();
    uint32_t deckTable = ld32(a1 + 0xEC);
    st32(key, uint32_t(deckId));
    st32(key + 4, uint32_t(deckId >> 32));
    uint32_t node = call(g::U64Map_find, {deckTable + 0x24, key});
    if (!node || int32_t(ld32(node + 16)) < 0) return;
    uint32_t deckOffset = ld32(node + 16) * 0x58;
    uint32_t a2 = api();         // medal table at +0xD4
    uint32_t a3 = api();         // skill table at +0xDC
    deckTable = ld32(a1 + 0xEC);
    call(g::SafeBuffer_read, {deckTable, deck, ld32(deckTable + 12) + deckOffset, 0x58, deckOffset});
    // the deck row: medal count at +0x10, the medals' unique ids from +0x18
    for (uint32_t k = 0; k < ld32(deck + 0x10); k++) {
        uint32_t medalTable = ld32(a2 + 0xD4);
        st32(key, ld32(deck + 0x18 + 8 * k));
        st32(key + 4, ld32(deck + 0x1C + 8 * k));
        uint32_t mn = call(g::U64Map_find, {medalTable + 0x24, key});
        if (!mn || int32_t(ld32(mn + 16)) < 0) continue;
        uint32_t medalOffset = ld32(mn + 16) * 0x48;
        call(g::SafeBuffer_read, {medalTable, medal, ld32(medalTable + 12) + medalOffset, 0x48, medalOffset});
        // the medal row: skill count at +0x40, skill unique ids (std::array<uint64,2>) at +0x30
        int32_t skills = int32_t(ld32(medal + 0x40));
        for (int32_t s = 0; s < skills; s++) {
            uint32_t uid = call(g::U64Array2_at, {medal + 0x30, uint32_t(s)});
            uint32_t uidLo = ld32(uid), uidHi = ld32(uid + 4);
            // the skill before the battle (the clear result's table)
            uint32_t before = ld32(property + 0x10);
            st32(key, uidLo); st32(key + 4, uidHi);
            uint32_t bn = call(g::U64Map_find, {before + 0x24, key});
            uint32_t bIndex = bn ? ld32(bn + 16) : 0xFFFFFFFFu;
            uint32_t bOffset = bIndex * 0x28;
            call(g::SafeBuffer_read, {before, skillBefore, ld32(before + 12) + bOffset, 0x28, bOffset});
            // the skill now (user skill table)
            uint32_t now = ld32(a3 + 0xDC);
            st32(key, uidLo); st32(key + 4, uidHi);
            uint32_t nn = call(g::U64Map_find, {now + 0x24, key});
            uint32_t nIndex = nn ? ld32(nn + 16) : 0xFFFFFFFFu;
            uint32_t nOffset = nIndex * 0x28;
            call(g::SafeBuffer_read, {now, skillNow, ld32(now + 12) + nOffset, 0x28, nOffset});
            // level (+0xC) went up: remember {skill id (+8), old level, user skill uid (+0x18)}
            if (int32_t(ld32(skillNow + 0xC)) > int32_t(ld32(skillBefore + 0xC))) {
                st32(item, ld32(skillBefore + 8));
                st32(item + 4, ld32(skillBefore + 0xC));
                st32(item + 8, ld32(skillBefore + 0x18));
                st32(item + 12, ld32(skillBefore + 0x1C));
                uint32_t e = self->skillsEnd;
                if (e != self->skillsCap) {
                    if (e) {
                        for (uint32_t w = 0; w < 16; w += 4) st32(e + w, ld32(item + w));
                    }
                    self->skillsEnd = e + 16;
                } else {
                    call(g::LevelUpSkillVector_pushSlow, {addr(&self->skillsBegin), item});
                }
            }
        }
    }
    popUpSkillLevelUp(self);
}
RW_FUNCTION(0x57F614, "BattleResultDialog::initSkill", BattleResultDialog::initSkill);

// The next skill level-up popup ("<medal>'s <skill> leveled up"), sliding in from the right.
void BattleResultDialog::popUpSkillLevelUp(BattleResultDialog *self) {
    if (self->skillPopupOpen) return;
    if ((self->skillsEnd - self->skillsBegin) >> 4 <= self->skillIndex) return;
    self->skillPopupOpen = 1;
    uint32_t area = find(self->root, STR_Scroll_Area, 11);
    uint32_t popup = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), STR_SkillJson});
    vcall(area, node::addChild, {area, popup});
    vcall(popup, node::setPositionX, {popup, vcall(popup, node::getPositionX, {popup})});
    uint32_t begin = self->skillsBegin;
    if ((self->skillsEnd - begin) >> 4 <= self->skillIndex) call(Vector_throwOutOfRange, {});
    uint32_t e = begin + 16 * self->skillIndex;
    uint32_t uidLo = ld32(e + 8), uidHi = ld32(e + 12);
    Frame f;
    uint32_t skill = f.alloc(8), medal = f.alloc(8);
    call(g::Skill_create, {skill, ld32(e)});
    uint32_t mdm = call(g::MedalDataManager_getInstance, {});
    // getMedalData(out, manager, uid, 0, UserMedal::Medal{} by value: 72 bytes on the stack)
    call(g::MedalDataManager_getMedalData, {medal, mdm, uidLo, uidHi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0});
    uint32_t medalName = f.alloc(0x44), skillName = f.alloc(0x44);
    sbRead(ld32(ld32(medal) + 0x58), medalName, 0x10, 0x41);
    uint32_t label = find(popup, STR_Txt_GetItem_Label, 0x11);
    sbRead(ld32(skill), skillName, 0xC, 0x41);
    {
        uint32_t text = f.alloc(12);
        call(g::TextData_format, {text, 0, 100451004, medalName, skillName});
        {
            TempStr s(strData(text), host_strlen(strData(text)));
            call(g::Text_setString, {label, s});
        }
        strFree(text);
    }
    SceneUtil::setShadow(label);
    {
        Frame g2;
        uint32_t fn = g2.alloc(24);
        st32(fn, VT_SkillPopupIn);
        st32(fn + 4, popup);
        st32(fn + 8, area);
        st32(fn + 12, addr(self));
        st32(fn + 16, fn);
        uint32_t anim = call(SceneUtil_createRightInAnimation, {popup, rw::fbits(0.0f), rw::fbits(0.0f), fn});
        call(g::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
        functionDestroy(fn);
    }
    sharedRelease(medal);
    sharedRelease(skill);
}
RW_FUNCTION(0x583234, "BattleResultDialog::popUpSkillLevelUp", BattleResultDialog::popUpSkillLevelUp);

// The "level up" LWF from its first frame, the level up sound, and the new Lux rank in the atlas label.
void BattleResultDialog::playLevelUpAnimation(BattleResultDialog *self) {
    if (!self->root || !self->levelUpLwf) return;
    uint32_t lwf = ld32(self->levelUpLwf + LWFNODE_LWF);
    call(Movie_GotoAndPlay, {ld32(lwf + 0x18), 0});
    call(g::Movie_Play, {ld32(lwf + 0x18)});
    setVisible(self->levelUpLwf, 1);
    playSE(SE_LEVEL_UP);
    uint32_t atlas = find(self->root, STR_AtlasLabel_NumL, 0xF);
    if (atlas) {
        Frame f;
        uint32_t s = f.alloc(12);
        SceneUtil::numberToAtlasString(s, self->luxRank);
        call(g::TextAtlas_setString, {atlas, s});
        strFree(s);
    }
}
RW_FUNCTION(0x58362C, "BattleResultDialog::playLevelUpAnimation", BattleResultDialog::playLevelUpAnimation);

// ------------------------------------------------------------------------------------------- LWF effects
// Play the result LWF `type` (lwf/battleresult/<name>/<name>.lwf) once on `parent`, with its sound, and run
// onEnd after its length (at least 0.1 s).
void BattleResultDialog::initResultLwf(BattleResultDialog *, uint32_t parent, int32_t type, uint32_t onEnd) {
    Frame f;
    uint32_t name = f.alloc(12);
    if (type == LWF_RAID_DEFEATED) {
        call(g::AudioReplaceGameMainBGM, {BGM_RAID_DEFEATED, 500});
    } else if (uint32_t(type) <= 7) {
        if (type == LWF_CLEAR_ON_EFFECT) playSE(SE_CLEAR_ON);
        else if (type != LWF_CO_RANK_UP && type != LWF_RANK_RESULT_REWARDS) playSE(SE_RESULT_LWF);
    }
    st32(name, 0); st32(name + 4, 0); st32(name + 8, 0);
    static const struct { uint32_t text, len; } names[] = {
        {0xDFFD62, 6},  {0xDFFD69, 0xD}, {0xDFFD77, 0xA}, {0xDFFD82, 0xD}, {0xDFFD90, 0xB},
        {0xDFFD9C, 9},  {0xDFFDA6, 0x13}, {0xDFFDBA, 0x13}, {0xDFFDCE, 0x11}};
    if (uint32_t(type) <= 8) call(g::String_assign, {name, names[type].text, names[type].len});
    uint32_t text = strData(name);
    uint32_t path = stringBuffer(f, {STR_LwfPath, text, text});
    uint32_t node = call(type == LWF_LUX_LEVEL_UP ? g::AudioLWFNode_create : g::LWFNode_create, {path, 0});
    vcall(parent, node::addChild, {parent, node});
    uint32_t lwf = ld32(node + LWFNODE_LWF);
    uint32_t movie = ld32(lwf + 0x18);
    call(Movie_GotoAndPlay, {movie, ld32(movie + MOVIE_TOTAL_FRAMES)});
    float seconds = float(int32_t(ld32(ld32(lwf + 0x18) + MOVIE_TOTAL_FRAMES))) / 60.0f;
    float delay = seconds < 0.1f ? 0.1f : seconds;
    uint32_t wait = call(g::DelayTime_create, {rw::fbits(delay)});
    uint32_t done = call(g::CallFunc_create, {onEnd});
    call(g::Node_runAction, {node, call(g::Sequence_create, {wait, done, 0})});
    strFree(name);
}
RW_FUNCTION(0x582B90, "BattleResultDialog::initResultLwf", BattleResultDialog::initResultLwf);

// The "sub mission cleared" effect on mission mark `mission` when it was cleared in this battle (not before):
// onEnd runs after the effect (0.5 s delay noted in subMissionDelay); otherwise onEnd runs at once.
void BattleResultDialog::initSubMissionLwf(BattleResultDialog *self, int32_t mission, uint32_t onEnd) {
    if ((ld32(smgr() + SM_BATTLE_KIND) & ~1u) == 2) return;
    Frame f;
    uint32_t vec = f.alloc(12), slot = f.alloc(8), buf = f.alloc(16);
    st32(vec, 0); st32(vec + 4, 0); st32(vec + 8, 0);
    uint32_t start = ld32(api() + API_STAGE_START);
    sbRead(ld32(appState() + APPSTATE_USER), buf, 0x4228, 12);
    // missions cleared in this battle that were not cleared before (unsigned compares, as compiled)
    for (uint32_t k = 0; k < 3; k++) {
        uint32_t m = ld32(buf + 4 * k);
        if (m == 0xFFFFFFFFu) break;
        uint32_t first = ld32(start + 0x4C), last = ld32(start + 0x50);
        int32_t count = int32_t(last - first) >> 2;
        while (count) {
            int32_t half = count / 2;
            uint32_t mid = first + 4 * uint32_t(half);
            if (ld32(mid) < m) {
                first = mid + 4;
                count = count - 1 - half;
            } else {
                count = half;
            }
        }
        if (first == last || m < ld32(first)) intPushBack(vec, slot, int32_t(m));
    }
    uint32_t area = find(self->root, STR_NormalBattleWin01, 0x1A);
    bool listed = contains(ld32(vec), ld32(vec + 4), mission);
    uint32_t name = stringBuffer(f, {STR_AreaPointSubClear, uint32_t(mission)});
    uint32_t point;
    {
        TempStr s(name, host_strlen(name));
        point = SceneUtil::searchNodeByName(area, s);
    }
    self->subMissionDelay = 0.0f;
    if (listed && point) {
        self->subMissionDelay = 0.5f;
        // $_21 [self, onEnd]: a heap __func of 40 bytes (self at +8, the std::function at +16)
        uint32_t copy = f.alloc(24);
        call(g::FunctionVV_copy, {copy, onEnd});
        uint32_t fn = f.alloc(24);
        st32(fn + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x28});
        uint32_t thisArg = 0;
        if (heap) {
            st32(heap, VT_SubMissionEnd);
            st32(heap + 8, addr(self));
            call(fn::function_vv_move, {heap + 16, copy});
            st32(heap + 8, addr(self));
            thisArg = addr(self);
        }
        st32(fn + 16, heap);
        initResultLwf(dialog(thisArg), point, LWF_CLEAR_ON_EFFECT, fn);
        functionDestroy(fn);
        functionDestroy(copy);
    } else if (ld32(onEnd + 16)) {
        call(g::FunctionVV_call, {onEnd});
    }
    call(g::IntVector_dtor, {vec});
}
RW_FUNCTION(0x582EF8, "BattleResultDialog::initSubMissionLwf", BattleResultDialog::initSubMissionLwf);
