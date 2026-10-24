// Small batch #5 leftovers: lambdas and helpers of lists whose other functions are written elsewhere
// (MyRecordScrollView, PresentBoxScrollView, SceneShopTop, the ranking reward item balloon).
#include "TapRow.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t MEMCPY = 0x40E164;                      // memcpy (import)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ScrollView_setContentOffset = rwlib::cocos2d::extension::ScrollView::setContentOffset; // (view, const Vec2&, bool animated)
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_executeGachaDraw = 0x4CE579; // (api, int, int, std::function)
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;    // (std::function)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(function&&)
constexpr uint32_t FunctionVV_call = 0x470E69;             // std::function<void()>::operator()
constexpr uint32_t Ranking_setIncentiveItemBalloon = 0x55EEA9;
constexpr uint32_t Ranking_setIncentiveItemBalloonUpdate = 0x55FD19;   // (widget, widget)
constexpr uint32_t Ranking_setIncentiveItemBalloonRemove = 0x55FE3D;
}  // namespace g

constexpr uint32_t V_setVisible = 0xB0;
constexpr uint32_t V_isVisible = 0xB4;
constexpr uint32_t V_getContentSize = 0xAC;

// ------------------------------------------------------------------------------------------------ MyRecordScrollView
// sub_518DC4, a lambda {dialog, widget}: the dialog's two selection flags cleared, the widget's "Select_On" hidden
// (the short name is built inline: memcpy into the string)
void clearSelection(uint32_t functor) {
    uint32_t dialog = ld32(functor + 4);
    st8(dialog + 0x3A4, 0);
    st8(dialog + 0x36B, 0);
    uint32_t widget = ld32(functor + 8);
    Frame f;
    uint32_t name = f.alloc(12);
    st8(name, 18);
    call(g::MEMCPY, {name + 1, 0xDFD1AC, 9});             // "Select_On"
    st8(name + 10, 0);
    uint32_t node = SceneUtil::searchNodeByName(widget, name);
    strFree(name);
    if (node) vcall(node, V_setVisible, {node, 0});
}

// the list back at its top (the content offset that shows its first row)
uint32_t resetScroll(uint32_t self) {
    uint32_t view = ld32(self + 0x504);
    if (!view) return 0;
    float height = ldf(self + 0x508);
    float shown = ldf(vcall(self, V_getContentSize, {self}) + 4);
    Frame f;
    uint32_t offset = f.alloc(8);
    call(g::Vec2_ctor, {offset, 0, rw::fbits(shown - height)});
    call(g::ScrollView_setContentOffset, {view, offset, 0});
    call(g::Vec2_dtor, {offset});
    return 0;
}

// ------------------------------------------------------------------------------------------------ PresentBoxScrollView
constexpr uint32_t PB_VT_TOUCH_ENDED = 0xF56394, PB_VT_TAP_DONE = 0xF563C0;
constexpr tap_row::Layout PB_TAP{0x6B8, 0x6B5, 0x6BC, 0x6C0, 8, PB_VT_TAP_DONE};

void presentBoxAssignTouchEnded(uint32_t slot, uint32_t captures) {
    tap_row::assignTouchEnded(PB_VT_TOUCH_ENDED, slot, captures);
}
void presentBoxTouchEnded(uint32_t captures, uint32_t touch) { tap_row::touchEnded(PB_TAP, captures, touch); }

// ------------------------------------------------------------------------------------------------ SceneShopTop
// sub_5B2528, a lambda {gacha, count, scene} (draw confirmed): the scene manager's connection callback set ($_
// below) and its loading flag raised, the gacha draw request sent, then the scene's own callback (+0x638)
constexpr uint32_t VT_DRAW_DONE = 0xF5C58C;
void drawConfirmed(uint32_t functor) {
    uint32_t scene = ld32(functor + 0xC);
    uint32_t mgr = call(g::SceneManager_getInstance, {});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_DRAW_DONE, {});
        st8(mgr + 0x4CC, 1);
        call(g::FunctionVB_assign, {mgr + 0x548, fo});
        functionDestroy(fo);
    }
    uint32_t api = call(g::APIManager_getInstance, {});
    {
        Frame f;
        uint32_t fo = newEmptyFunction(f);
        call(g::APIManager_executeGachaDraw, {api, ld32(functor + 4), ld32(functor + 8), fo});
        functionDestroy(fo);
    }
    call(g::FunctionVV_call, {scene + 0x638});
}

// sub_5B26A4, a lambda: unless a request is waiting to be retried, the shop request again and the shop top scene
// layer replaced (createSceneLayer<SceneShopTop>)
constexpr uint32_t GOT_FUNC_CREATE_LAYER_VT = 0xFD6074;     // __func<Layer*(*)(), ...> vtable
constexpr uint32_t GOT_CREATE_SHOP_TOP = 0xFD66F8;          // createSceneLayer<SceneShopTop>
void reloadShopTop(uint32_t) {
    if (call(g::APIManager_needsRetry, {call(g::APIManager_getInstance, {})})) return;
    {
        Frame f;
        uint32_t fo = newEmptyFunction(f);
        call(g::SceneShopTop_callShopAPI, {fo});
        functionDestroy(fo);
    }
    uint32_t mgr = call(g::SceneManager_getInstance, {});
    Frame f;
    uint32_t fo = newFunction(f, ld32(GOT_FUNC_CREATE_LAYER_VT) + 8, {ld32(GOT_CREATE_SHOP_TOP)});
    call(g::SceneManager_changeSceneLayer, {mgr, fo, 1, 1, 1});
    functionDestroy(fo);
}

// ------------------------------------------------------------------------------------------------ item balloon
// sub_561FA8, a touch listener lambda {widget, parent, type, rarity, id, string name, string image, amount, a,
// flag} on a reward icon: while the widget is shown, a press opens the item balloon, a move updates it, a release
// or cancel removes it; with balloons off (SceneManager +0x308 clear) every event only updates it
void itemBalloonTouch(uint32_t functor, uint32_t, uint32_t typeRef) {
    uint32_t type = ld32(typeRef);
    uint32_t widget = ld32(functor + 4);
    if (vcall(widget, V_isVisible, {widget}) != 1) return;
    uint32_t mgr = call(g::SceneManager_getInstance, {});
    if (!ld8(mgr + 0x308)) {
        call(g::Ranking_setIncentiveItemBalloonUpdate, {ld32(functor + 4), ld32(functor + 8)});
        return;
    }
    switch (type) {
    case 0: {
        Frame f;
        uint32_t image = f.alloc(12), name = f.alloc(12);
        auto copy = [](uint32_t dst, uint32_t src) {
            if (strIsLong(src)) {
                strInit(dst, ld32(src + 8), ld32(src + 4));
                return;
            }
            st32(dst, ld32(src));
            st32(dst + 4, ld32(src + 4));
            st32(dst + 8, ld32(src + 8));
        };
        copy(name, functor + 0x18);
        copy(image, functor + 0x24);
        call(g::Ranking_setIncentiveItemBalloon,
             {ld32(functor + 4), ld32(functor + 0xC), ld32(functor + 0x10), ld32(functor + 0x14), name, image,
              ld32(functor + 0x30), ld32(functor + 0x34), ld8(functor + 0x38), ld32(functor + 8)});
        strFree(image);
        strFree(name);
        break;
    }
    case 1: call(g::Ranking_setIncentiveItemBalloonUpdate, {ld32(functor + 4), ld32(functor + 8)}); break;
    case 2:
    case 3: call(g::Ranking_setIncentiveItemBalloonRemove, {}); break;
    default: break;
    }
}
// ------------------------------------------------------------------------------------------------ campaigns
// CampaignAnimationView::isCampaign(stageId, evCampaign): whether the campaign covers the stage - types 2 and 3
// list up to five stage ids, 4..8 cover every stage, 9..18 cover the stages of kind 1..10
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t MasterStage_create = 0x78E4E9;
constexpr uint32_t SHARED_RELEASE = 0xC8DBD1;
constexpr uint32_t EV_TYPE = 0x110, EV_STAGE_COUNT = 0x114, EV_STAGES = 0x118, STAGE_KIND = 0x110;

uint32_t readInt(uint32_t row, uint32_t offset, uint32_t dst) {
    call(SafeBuffer_read, {row, dst, ld32(row + 0xC) + offset, 4, offset});
    return ld32(dst);
}
uint32_t isCampaign(uint32_t stageId, uint32_t campaignRef) {
    Frame f;
    uint32_t stage = f.alloc(8), word = f.alloc(4), ids = f.alloc(0x14);
    call(MasterStage_create, {stage, stageId});
    uint32_t type = readInt(ld32(campaignRef), EV_TYPE, word);
    uint32_t result = 0;
    switch (int32_t(type)) {
    case 2:
    case 3:
        for (int32_t i = 0;; i++) {
            result = 0;
            if (i >= int32_t(readInt(ld32(campaignRef), EV_STAGE_COUNT, word))) break;
            uint32_t row = ld32(campaignRef);
            call(SafeBuffer_read, {row, ids, ld32(row + 0xC) + EV_STAGES, 0x14, EV_STAGES});
            result = 1;
            if (ld32(ids + 4 * uint32_t(i)) == stageId) break;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8: result = 1; break;
    default:
        if (type >= 9 && type <= 18) result = readInt(ld32(stage), STAGE_KIND, word) == type - 8 ? 1 : 0;
        break;
    }
    if (ld32(stage + 4)) call(SHARED_RELEASE, {ld32(stage + 4)});
    return result;
}
}  // namespace

RW_FUNCTION(0x59878C, "CampaignAnimationView::isCampaign(int, shared_ptr<const master::EvCampaign>&)", isCampaign);
RW_FUNCTION(0x518DC4, "MyRecordScrollView lambda: clear the selection (sub_518DC4)", clearSelection);
RW_FUNCTION(0x51B074, "MyRecordScrollView::resetScroll()", resetScroll);
RW_FUNCTION(0x50DE48, "PresentBoxScrollView::registItemTouchEvent touch ended setter (sub_50DE48)", presentBoxAssignTouchEnded);
RW_FUNCTION(0x50F7F4, "PresentBoxScrollView::registItemTouchEvent $_2 body (sub_50F7F4)", presentBoxTouchEnded);
RW_FUNCTION(0x5B2528, "SceneShopTop lambda: gacha draw confirmed (sub_5B2528)", drawConfirmed);
RW_FUNCTION(0x5B26A4, "SceneShopTop lambda: reload the shop top (sub_5B26A4)", reloadShopTop);
RW_FUNCTION(0x561FA8, "RankingRewardScrollView item balloon touch lambda (sub_561FA8)", itemBalloonTouch);
