// BattleResultScrollView: the item list of the battle result screens (BattleResultDialog). A BaseScrollView<int>
// (IScrollProtocol, a cocos2d::ui::Layout) whose items are indices into `results`, the items obtained in the battle
// (materials, enemy drops, treasures, campaign bonuses, colosseum rewards). The list is filled one item at a time
// (updateResultAnimetion -> pushFrontItem / pushBackItem): the visible rows slide by one item, then the new item
// drops in (a rare / treasure item plays the "Result_Reward_Anime" armature before showing its count).
#pragma once

#include "../scene_main/IScrollProtocol.hpp"
#include "../scene_social/SocialCommon.hpp"

struct BattleResultScrollView : IScrollProtocol {
    // One obtained item (68 bytes).
    struct ResutData {
        rw::Field<int32_t> type;            // +0x00 item type (2 jewel, 3 medal, 5 material, 13 keyblade, ...)
        guest::String image;                // +0x04 image file (SceneUtil::setItemImageData)
        guest::String name;                 // +0x10 item name
        rw::Field<int32_t> id;              // +0x1C item id (material id / reward id)
        rw::Field<uint8_t> isNew;           // +0x20 "New" mark
        rw::Field<uint8_t> isTreasure;      // +0x21 dropped from a treasure (plays the reward armature)
        const uint8_t pad_22[2];
        rw::Field<int32_t> slot;            // +0x24 reward slot (0..3)
        rw::Field<int32_t> count;           // +0x28 number obtained
        rw::Field<int32_t> treasureKind;    // +0x2C treasure image number (Prize_Treasure%02d)
        rw::Field<int32_t> amount;          // +0x30 amount shown (campaign points applied)
        rw::Field<uint8_t> hasWording;      // +0x34 a wording line ("campaign bonus" ...)
        const uint8_t pad_35[3];
        guest::String wording;              // +0x38
    };

    // ---- BaseScrollView<int>
    guest::Vector ids;                      // +0x628 std::vector<int>: the item of each list position
    const uint8_t pad_634[4];
    const uint8_t onTapItem[16];            // +0x638 std::function (BaseScrollView)
    rw::Field<uint32_t> onTapItem_f;        // +0x648
    const uint8_t pad_64C[4];
    // ---- BattleResultScrollView
    guest::Vector results;                  // +0x650 std::vector<ResutData>
    guest::Vector pending;                  // +0x65C std::vector<int>: result indices still to add to the list
    rw::Field<float> rareDropSpeed;         // +0x668 speed scale of the reward armature (0.5)
    rw::Field<float> addRightInTime;        // +0x66C seconds of the row slide (0.2)
    rw::Field<float> delayTime;             // +0x670 delay before the next item (0.2)
    rw::Field<float> listMoveTime;          // +0x674 seconds of the list move (0.2)
    rw::Field<uint8_t> adding;              // +0x678 an item is being added
    rw::Field<uint8_t> dropping;            // +0x679 the reward armature plays
    rw::Field<uint8_t> hasRare;             // +0x67A a medal (type 3) was obtained
    const uint8_t pad_67B[1];
    rw::Field<uint32_t> rowTemplate;        // +0x67C ui::Widget* "BattleResult_GetItem" (retained)
    rw::Field<uint8_t> enabledTapItem;      // +0x680
    rw::Field<uint8_t> touching;            // +0x681 a touch began on an item
    const uint8_t pad_682[2];
    rw::Field<int32_t> touchIndex;          // +0x684 item index of the touch
    rw::Field<uint8_t> tapping;             // +0x688 the tap animation plays
    const uint8_t pad_689[3];
    rw::Field<float> touchStartY;           // +0x68C

    static constexpr uint32_t SIZE = 0x690;
    static constexpr uint32_t RESULT_SIZE = 0x44;
    static constexpr uint32_t VTABLE_BASE = 0xF83F70;   // _ZTV22BattleResultScrollView
    static constexpr uint32_t BASE_VTABLE = 0xF811D0;   // _ZTV14BaseScrollViewIiE

    using Self = BattleResultScrollView;
    static uint32_t idCount(const Self *s) { return (s->ids.end - s->ids.begin) >> 2; }
    static uint32_t resultAt(const Self *s, uint32_t index) { return s->results.begin + RESULT_SIZE * index; }
    static uint32_t resultCount(const Self *s) {   // (end - begin) / 68, as the code computes it
        return uint32_t(int32_t(s->results.end - s->results.begin) >> 2) * 0xF0F0F0F1u;
    }

    // life cycle
    static Self *construct(Self *self);
    static Self *destruct(Self *self);
    static void destructDelete(Self *self);
    static uint32_t create();
    static uint32_t init(Self *self);
    // rows
    static uint32_t getLayoutPanel(Self *self, int32_t unused);
    static uint32_t createRow(Self *self, uint32_t index, uint32_t arg);
    static uint32_t changeRow(Self *self, uint32_t widget, uint32_t index, uint32_t arg);
    static void registItemTouchEvent(Self *self, uint32_t widget, int32_t index);
    static Self *setEnabledTapItem(Self *self, uint32_t enabled);
    // adding items
    static void pushFrontItem(Self *self, int32_t index);
    static void pushBackItem(Self *self, int32_t index);
    static uint32_t updateResultAnimetion(Self *self, float dt, uint32_t front);
    static Self *setAnimationTime(Self *self, float rareDrop, float addRightIn, float delay, float listMove);
    static Self *setRareDropSpeed(Self *self, float v);
    static Self *setAddRightInTime(Self *self, float v);
    static Self *setDelayTime(Self *self, float v);
    static Self *setListMoveTime(Self *self, float v);
    static void setDropImage(Self *self, uint32_t widget, int32_t index);
    // the result list
    static void addResultMaterial(Self *self, int32_t materialId);
    static void getImageName(Self *self, ResutData *data, int32_t id, uint32_t flag);
    static void addResultReward(Self *self, int32_t rewardId, int32_t slot);
    static uint32_t loadResultData(Self *self);
    static uint32_t loadStageResultData(Self *self);
    static uint32_t loadColosseumResultData(Self *self);
    static ResutData *copyResult(ResutData *self, const ResutData *other);
};
RW_LAYOUT(BattleResultScrollView, ids, 0x628);
RW_LAYOUT(BattleResultScrollView, onTapItem_f, 0x648);
RW_LAYOUT(BattleResultScrollView, results, 0x650);
RW_LAYOUT(BattleResultScrollView, pending, 0x65C);
RW_LAYOUT(BattleResultScrollView, rareDropSpeed, 0x668);
RW_LAYOUT(BattleResultScrollView, rowTemplate, 0x67C);
RW_LAYOUT(BattleResultScrollView, touchIndex, 0x684);
RW_LAYOUT(BattleResultScrollView, touchStartY, 0x68C);
RW_SIZE(BattleResultScrollView, 0x690);
RW_LAYOUT(BattleResultScrollView::ResutData, id, 0x1C);
RW_LAYOUT(BattleResultScrollView::ResutData, amount, 0x30);
RW_LAYOUT(BattleResultScrollView::ResutData, wording, 0x38);
RW_SIZE(BattleResultScrollView::ResutData, 0x44);

// Shared by the BattleResultScrollView files.
namespace brsv {
using namespace social;
using Self = BattleResultScrollView;
using ResutData = BattleResultScrollView::ResutData;
inline Addr me(const Self *s) { return rw::guest_addr(s); }

namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;           // __aeabi_memset(dst, n, c)
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t IScrollProtocol_setScrollType = 0x4F70B5;
constexpr uint32_t IScrollProtocol_getItemPixel = 0x4F6751;
constexpr uint32_t IScrollProtocol_getMaxPosition = 0x4F6511;
constexpr uint32_t IScrollProtocol_setItemIDSize = 0x4F6BC9;
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t ScrollBar_updateLayout = 0x4F53F9;
constexpr uint32_t ScrollBar_setScrollPosition = 0x4F56A5;
constexpr uint32_t BaseScrollView_int_init = 0x4FA8A1;
constexpr uint32_t vector_int_dtor = 0x478961;        // std::__vector_base<int>::~__vector_base
constexpr uint32_t vector_int_assign = 0x47D5BD;      // vector<int>::assign(int*, int*)
constexpr uint32_t vector_int_insert = 0x516209;      // vector<int>::insert(const_iterator, const int&)
constexpr uint32_t vector_int_pushSlowRef = 0x4D6CC1; // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t vector_int_pushSlow = 0x47D709;    // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t ResutVector_dtor = 0x511C85;       // __vector_base<ResutData>::~__vector_base
constexpr uint32_t ResutVector_pushSlow = 0x51517D;   // vector<ResutData>::__push_back_slow_path(const&)
constexpr uint32_t throwOutOfRange = 0x47CCE1;
constexpr uint32_t array3_at = 0x5149A1;              // std::array<int,3>::at(n)
constexpr uint32_t array4_at = 0x508555;              // std::array<int,4>::at(n)
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;              // (this, float x, float y)
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t MoveTo_create = 0x86941D;          // (float duration, const Vec2&)
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t Sequence_create = 0x867999;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t ImageView_loadTexture = 0x839731;  // (image, const string&, TextureResType)
constexpr uint32_t Touch_getLocation = 0x900E57;      // (Vec2* out, Touch*)
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t function_touchBool_swap = 0x483E4D;
constexpr uint32_t function_touchVoid_swap = 0x4F3699;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t ArmatureAnimation_setMovementEventCallFunc = 0x7A3C69;
constexpr uint32_t Skin_create = 0x7AB597;            // cocostudio::Skin::create(const string&)
constexpr uint32_t Bone_removeDisplay = 0x7A2665;
constexpr uint32_t Bone_addDisplay = 0x7A2657;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;   // (string& name, string& image, type, id, bool)
constexpr uint32_t SceneUtil_getCampaignPoint = 0x4741BD;   // (base, kind, stageId, const vector<int>&, target, bool)
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t StageUtil_getEnemyDropPrizes = 0x763C1D; // (MapPrizes::Items*, enemy, id, PlayStyle)
constexpr uint32_t StageUtil_getTreasurePrizes = 0x763DF9;
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t strlen_ = 0x40E08C;                // strlen (import)
constexpr uint32_t string_init = 0x47CD93;            // std::string::__init(const char*, size_t)
constexpr uint32_t string_assign = 0x48D811;          // std::string::assign(const char*, size_t)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t Stage_create = 0x78E4E9;
constexpr uint32_t Reward_create = 0x78B5C1;
constexpr uint32_t Colosseum_create = 0x77C4E9;
constexpr uint32_t ColosseumStage_create = 0x77CF95;
constexpr uint32_t Audio_getInstance = 0x5BECFD;
constexpr uint32_t Audio_getLayer = 0x5BF33D;
constexpr uint32_t AudioLayer_playSE = 0x5BF785;
constexpr uint32_t shared_add = 0xC8DBB5;
constexpr uint32_t divsi3 = 0xCD5E8D;
}  // namespace g

// vtable byte offsets
constexpr uint32_t V_getAnimation = 0x298;     // cocostudio::Armature
constexpr uint32_t V_getBone = 0x27C;          // cocostudio::Armature::getBone(const string&)
constexpr uint32_t V_setSpeedScale = 0x5C;     // ArmatureAnimation
constexpr uint32_t V_play = 0x68;              // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t V_setScaleXY = 0x48;        // Node::setScale(float, float)
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t V_changeRow = 0x340;
constexpr uint32_t V_init = 0x248;

constexpr uint32_t GOT_ITEM_MARGIN = 0xFD659C;  // GOT slot: &IScrollProtocol::ITEM_MARGIN

// a guest frame temporary std::string from a literal: free it after use
inline void freeStr(Addr s) { strFree(s); }
// SafeBuffer::read(sb, dst, data + index, n, index)
inline void sbRead(Addr sb, Addr dst, uint32_t index, uint32_t n) {
    call(g::SafeBuffer_read, {sb, dst, ld32(sb + 12) + index, n, index});
}
// a ResutData temporary: destroy its strings (wording, name, image)
inline void destroyResult(Addr r) {
    strFree(r + 0x38);
    strFree(r + 0x10);
    strFree(r + 0x04);
}
// results.push_back(*data), then the medal flag
void pushResult(Self *s, Addr data);
// pending = 0, 1, ..., resultCount - 1 (after clearing it); returns the result count
uint32_t fillPending(Self *s, Addr counter);
// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
void playSE(uint32_t id);
}  // namespace brsv
