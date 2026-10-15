// BattleResultDialog: the result screens shown after a battle, over a modal shadow on the battle scene: the
// stage result (Lux gained, Lux level gauge, sub missions, skill level ups), the items obtained, the clear rank,
// the raid and colosseum results. Each open* builds a cocostudio scene file into `root`, plays LWF effects
// (initResultLwf) and counts its numbers up with ResultPointLayer; a tap on the centre button advances.
#pragma once

#include "BattleUi.hpp"
#include "ResultPointLayer.hpp"

struct BattleResultDialog {
    // A skill whose level went up in the battle (shown one after the other by popUpSkillLevelUp).
    struct LevelUpSkill {
        rw::Field<int32_t> skillId;         // +0  master::Skill id
        rw::Field<int32_t> level;           // +4  level before the battle
        rw::Field<uint32_t> uidLo;          // +8  the user skill's unique id (uint64)
        rw::Field<uint32_t> uidHi;          // +12
    };

    rw::Field<uint32_t> vtable;             // +0   cocos2d::Node
    const uint8_t node_04[524];                   // +4   cocos2d::Node
    const uint8_t onEnter[24];                    // +528 Node::_onEnterCallback (std::function<void()>)
    const uint8_t onExit[24];                     // +552 Node::_onExitCallback
    const uint8_t node_240[48];                   // +576 cocos2d::Node
    rw::Field<uint32_t> parent;             // +624 the node the dialog was opened on (0: not open yet)
    rw::Field<uint32_t> shadow;             // +628 modal shadow sprite (holds the dialog)
    rw::Field<uint32_t> root;               // +632 the cocostudio scene of the open screen
    const uint8_t pad_27C[4];                     // +636
    const uint8_t closeEvent[16];                 // +640 std::function<void()> run when the dialog closed
    rw::Field<uint32_t> closeEvent_f;       // +656 its callable pointer
    const uint8_t pad_294[4];                     // +660
    rw::Field<uint8_t> canClose;            // +664 the OK / centre button may close the screen
    const uint8_t pad_299[3];                     // +665
    rw::Field<int32_t> luxRank;             // +668 Lux rank (level) shown by the gauge
    rw::Field<uint32_t> luxLo;              // +672 Lux before the battle (uint64)
    rw::Field<uint32_t> luxHi;              // +676
    rw::Field<uint32_t> skillsBegin;        // +680 std::vector<LevelUpSkill>
    rw::Field<uint32_t> skillsEnd;          // +684
    rw::Field<uint32_t> skillsCap;          // +688
    rw::Field<uint32_t> skillIndex;         // +692 next level-up skill popup
    rw::Field<uint8_t> skillPopupOpen;      // +696 a skill popup is on screen
    const uint8_t pad_2B9[3];                     // +697
    rw::Field<uint32_t> field_2BC;          // +700 cleared by the constructor
    rw::Field<uint32_t> field_2C0;          // +704 cleared by the constructor
    rw::Field<uint8_t> scrollAnimating;     // +708 the item list is still animating in
    const uint8_t pad_2C5[3];                     // +709
    rw::Field<uint32_t> scrollView;         // +712 BattleResultScrollView (items)
    rw::Field<uint32_t> levelUpLwf;         // +716 LWFNode "Result_LevelUP"
    rw::Field<uint32_t> tapAnimation;       // +720 cocostudio::Armature "Tap_Screen_Animation"
    rw::Field<uint32_t> nextText;           // +724 CustomTextView "to the next level"
    rw::Field<uint32_t> nextPlate;          // +728 "Plate_Next"
    const uint8_t pad_2DC[4];                     // +732
    ResultPointLayer points0;               // +736 first counter (rank-up screens)
    ResultPointLayer luxPoints;             // +792 Lux gained
    ResultPointLayer points2;               // +848
    ResultPointLayer points3;               // +904 munny (openGettingItem)
    rw::Field<uint32_t> field_3C0;          // +960
    rw::Field<float> subMissionDelay;       // +964 0, or 0.5 s while a sub mission LWF plays
    rw::Field<uint8_t> field_3C8;           // +968 cleared by the constructor
    const uint8_t pad_3C9[7];                     // +969
    ResultPointLayer points4;               // +976

    static constexpr uint32_t SIZE = 0x408;
    static constexpr uint32_t VTABLE = 0xF8B2BC;    // _ZTV18BattleResultDialog + 8

    enum ResultLwfType : int32_t {
        LWF_RESULT = 0, LWF_RESULT_REWARD = 1, LWF_RAID_CLEAR = 2, LWF_RAID_DEFEATED = 3, LWF_RAID_RESULT = 4,
        LWF_CO_RANK_UP = 5, LWF_RANK_RESULT_REWARDS = 6, LWF_CLEAR_ON_EFFECT = 7, LWF_LUX_LEVEL_UP = 8,
    };

    // life cycle
    static BattleResultDialog *construct(BattleResultDialog *self);
    static BattleResultDialog *destruct(BattleResultDialog *self);
    static void destructDelete(BattleResultDialog *self);
    static BattleResultDialog *create();
    static bool init(BattleResultDialog *self);
    static void update(BattleResultDialog *self, float dt);
    static void close(BattleResultDialog *self);
    static void setCloseEvent(BattleResultDialog *self, uint32_t callback);

    // screens
    static bool openParentChack(BattleResultDialog *self, uint32_t parent);
    static bool openStageResult(BattleResultDialog *self, uint32_t parent);
    static bool openGettingItem(BattleResultDialog *self, uint32_t parent);
    static bool openClearRank(BattleResultDialog *self, uint32_t parent);
    static bool openColosseumResult(BattleResultDialog *self, uint32_t parent);
    static bool openRaidResult(BattleResultDialog *self, uint32_t parent, uint32_t timeStatus);

    // parts
    static void updateLuxGauge(BattleResultDialog *self);
    static void updateLuxLvNext(BattleResultDialog *self);
    static void setTitleLayer(BattleResultDialog *self, uint32_t unused);
    static void setSubMisstonLayer(BattleResultDialog *self);
    static void setResultItemLayer(BattleResultDialog *self);
    static void initSkill(BattleResultDialog *self, uint32_t property, uint64_t deckId);
    static void popUpSkillLevelUp(BattleResultDialog *self);
    static void playLevelUpAnimation(BattleResultDialog *self);
    static void initResultLwf(BattleResultDialog *self, uint32_t parent, int32_t type, uint32_t onEnd);
    static void initSubMissionLwf(BattleResultDialog *self, int32_t mission, uint32_t onEnd);
};
RW_LAYOUT(BattleResultDialog, onEnter, 528);
RW_LAYOUT(BattleResultDialog, onExit, 552);
RW_LAYOUT(BattleResultDialog, parent, 624);
RW_LAYOUT(BattleResultDialog, closeEvent_f, 656);
RW_LAYOUT(BattleResultDialog, canClose, 664);
RW_LAYOUT(BattleResultDialog, luxRank, 668);
RW_LAYOUT(BattleResultDialog, skillsBegin, 680);
RW_LAYOUT(BattleResultDialog, skillPopupOpen, 696);
RW_LAYOUT(BattleResultDialog, scrollAnimating, 708);
RW_LAYOUT(BattleResultDialog, scrollView, 712);
RW_LAYOUT(BattleResultDialog, nextPlate, 728);
RW_LAYOUT(BattleResultDialog, points0, 736);
RW_LAYOUT(BattleResultDialog, luxPoints, 792);
RW_LAYOUT(BattleResultDialog, points2, 848);
RW_LAYOUT(BattleResultDialog, points3, 904);
RW_LAYOUT(BattleResultDialog, subMissionDelay, 964);
RW_LAYOUT(BattleResultDialog, points4, 976);
RW_SIZE(BattleResultDialog, 0x408);
RW_SIZE(BattleResultDialog::LevelUpSkill, 16);

// Shared by the BattleResultDialog files: guest addresses, strings and lambda vtables.
namespace brd {
using namespace scene;

inline uint32_t addr(const void *p) { return rw::guest_addr(p); }
inline BattleResultDialog *dialog(uint32_t a) { return rw::host_ptr<BattleResultDialog>(a); }

// a temporary std::string from a literal: build, use, free (the original's pattern around each call)
struct TempStr {
    Frame f;
    uint32_t s;
    TempStr(uint32_t text, uint32_t len) { s = f.alloc(12); strInit(s, text, len); }
    ~TempStr() { strFree(s); }
    operator uint32_t() const { return s; }
};
inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }

namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Node_dtor = 0x8927ED;
constexpr uint32_t Node_init = 0x892B69;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;      // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVV_copy = 0x4683CD;        // std::function<void()>(const function&)
constexpr uint32_t FunctionVV_call = 0x470E69;
constexpr uint32_t LevelUpSkillVector_dtor = 0x57DA51;     // __vector_base<LevelUpSkill>::~__vector_base
constexpr uint32_t LevelUpSkillVector_pushSlow = 0x583CA9; // vector<LevelUpSkill>::__push_back_slow_path
constexpr uint32_t IntVector_dtor = 0x478961;          // __vector_base<int>::~__vector_base
constexpr uint32_t IntVector_assign = 0x47D5BD;        // vector<int>::assign(int*, int*)
constexpr uint32_t IntVector_pushSlow = 0x47D709;      // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t IntSort = 0x4EC65D;                 // std::__sort<__less<int>&, int*>(first, last, comp)
constexpr uint32_t U64Map_find = 0x490275;             // unordered_map<uint64,int>::find(const uint64&)
constexpr uint32_t U64Array2_at = 0x5835B9;            // std::array<uint64,2>::at(n)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t Stage_create = 0x78E4E9;            // master::Stage::create(shared_ptr out, id)
constexpr uint32_t Skill_create = 0x78BDE9;            // master::Skill::create(shared_ptr out, id)
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t TextData_format = 0x492929;         // TextData::format(string out, type, id, ...)
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t ImageButton_create = 0x531239;      // (std::string, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t FadeTo_create = 0x86B3E1;
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t Sequence_create = 0x867999;
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t LWFNode_create = 0x7EDF31;          // (const char* path, void*)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t Movie_Stop = 0x815C33;
constexpr uint32_t Movie_Play = 0x815C2B;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t Audio_getInstance = 0x5BECFD;       // audio::Manager::getInstance
constexpr uint32_t Audio_load = 0x5BEEF5;              // audio::Manager::load(ResourceID)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t String_assign = 0x48D811;
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t ScrollView_create = 0x511D4D;               // BattleResultScrollView::create
constexpr uint32_t ScrollView_setEnabledTapItem = 0x512911;
constexpr uint32_t ScrollView_setRareDropSpeed = 0x5133E1;
constexpr uint32_t ScrollView_setAddRightInTime = 0x5133E9;
constexpr uint32_t ScrollView_setDelayTime = 0x5133F5;
constexpr uint32_t ScrollView_setListMoveTime = 0x5133FD;
constexpr uint32_t ScrollView_updateResultAnimetion = 0x513335;
constexpr uint32_t ScrollView_loadResultData = 0x513D85;
constexpr uint32_t ScrollView_loadStageResultData = 0x5142A5;
constexpr uint32_t ScrollView_loadColosseumResultData = 0x514A15;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t AdvStageScrollView_setSubMisstionInfo = 0x56B301;
constexpr uint32_t AdvStageScrollView_setSubMisstion = 0x5691E1;
}  // namespace g

// APIManager / AppState / SceneManager fields read by the result screens
constexpr uint32_t API_STAGE_CLEAR = 0x144;       // shared_ptr<StageClear>: +0x54 Property (Lux SafeBuffer at +0,
                                                  // rank SafeBuffer at +8, new skills table at +0x10)
constexpr uint32_t API_STAGE_START = 0x13C;       // story stage start: +0x30 stage id, +0x40 deck id, +0x4C/+0x50
                                                  // cleared missions, +0x58 campaign ids
constexpr uint32_t API_EVENT_START = 0x260;       // event stage start: +0x30 stage, +0x38 deck, +0x40 campaigns
constexpr uint32_t API_RAID_START = 0x178;        // raid stage start: +0x30 stage, +0x40 deck, +0x68 campaigns
constexpr uint32_t API_STAGE_STATE = 0x33C;       // SafeBuffer (embedded): +4 current stage id
constexpr uint32_t SM_BATTLE_KIND = 0x30C;        // SceneManager: 0 story, 2 event, 3 raid, ...
constexpr uint32_t SM_RESULT_KIND = 0x28C;        // SceneManager: result screen kind (0..8)
constexpr uint32_t APPSTATE_USER = 0x20;          // AppState: shared_ptr<SafeBuffer> user data

}  // namespace brd
