// SceneMyPage: small helpers - touch guards, field scale, scene changes, profile refresh, party switch.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;   // (const string&, function<void(bool)>, ?, ?)
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t FunctionVB_assign = 0x490109;
constexpr uint32_t SceneRanking_callLuxRankingAPI = 0x6FB8E9;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t hole_text_createTimeLeftString = 0x492B75;      // (string* out, long seconds)
constexpr uint32_t M_updateFukidashi = 0x642C39, M_changeRankingMode = 0x64397D, M_changeRankingHeader = 0x643C9D;
constexpr uint32_t VT_back_42 = 0xF611B8, VT_ranking_43 = 0xF6118C, VT_party_55 = 0xF60D6C;
constexpr uint32_t DEFAULT_SCALE = 0x4026353Fu;   // 2.597
}  // namespace

// refresh the player's status balloon after a profile change
void SceneMyPage_onChangeProfile(uint32_t self) {
    uint32_t me = ld32(self + 0x2BC);
    if (!me) return;
    uint32_t lo = ld32(self + 0x3A0), hi = ld32(self + 0x3A4);
    if (!(lo | hi)) return;
    call(M_updateFukidashi, {self, me, lo, hi, 0});
}

// the back key: "back to the title?" ($_42 answers)
void SceneMyPage_backScene(uint32_t) {
    Frame f;
    uint32_t td = textData(f, 109190001);
    uint32_t text = newStr(f, textDataStr(td));
    uint32_t fo = newFunction(f, VT_back_42, {});
    call(SceneUtil_openOkCancelDialog4Line, {text, fo, 0, 0});
    functionDestroy(fo);
    strFree(text);
    release(ld32(td + 4));
}

// buttons react only once the scene is ready and no mode change runs
uint32_t SceneMyPage_canTouchButton(uint32_t self) {
    if (!ld8(self + 0x410) || ld8(self + 0x2F2)) return 0;
    return ld8(self + 0x2F3) ? 0 : 1;
}
uint32_t SceneMyPage_canTouchRankingModeButton(uint32_t self) {
    if (!ld8(self + 0x410) || ld8(self + 0x2F2) || ld8(self + 0x2F3)) return 0;
    return ld8(self + 0x2F0) ? 0 : 1;
}

// to the ranking scene (after the Lux ranking request)
void SceneMyPage_gotoSceneRanking(uint32_t) {
    {
        uint32_t sm = call(SceneManager_getInstance, {});
        st8(sm + 0x4CC, 1);
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(FunctionVB_assign, {sm + 0x548, empty});
        functionDestroy(empty);
    }
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(SceneRanking_callLuxRankingAPI, {empty});
        functionDestroy(empty);
    }
    uint32_t sm = call(SceneManager_getInstance, {});
    Frame f;
    uint32_t fo = newFunction(f, VT_ranking_43, {});
    call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
}

// field scale for background type 1 / 2 / 3
void SceneMyPage_initFieldScale(uint32_t self, int32_t type) {
    uint32_t scale = type == 3 ? 0x3FC00000u : type == 2 ? 0x40000000u : DEFAULT_SCALE;
    st32(self + OFF_SCALE_BASE, scale);
    st32(self + OFF_WALK_UNIT, 0x40477319u);
    float ratio = bf(scale) / bf(DEFAULT_SCALE);
    stf(self + 0x370, ratio);
    stf(self + OFF_SCALE, ratio * bf(0x3FA66666u));
}

// switch the ranking party type (header animation $_55 when the mode does not change at once)
void SceneMyPage_changeParty(uint32_t self, uint32_t type) {
    if (!ld8(self + 0x410) || ld8(self + 0x2F2) || ld8(self + 0x2F3)) return;
    uint32_t old = ld32(self + 0x28C);
    st32(self + 0x28C, type);
    if (call(M_changeRankingMode, {self})) return;
    st32(self + 0x28C, old);
    Frame f;
    uint32_t fo = newFunction(f, VT_party_55, {self, type});
    call(M_changeRankingHeader, {self, 0, fo});
    functionDestroy(fo);
}

// "n days left" for a banner, hidden once expired
void SceneMyPage_updateExpirationDate(uint32_t self, uint32_t text, int32_t seconds) {
    if (!(int32_t(ld32(self + OFF_EXPIRE_SECONDS)) > seconds)) {
        vcall(text, node::setVisible, {text, 0});
        return;
    }
    vcall(text, node::setVisible, {text, 1});
    Frame f;
    uint32_t left = f.alloc(12);
    call(hole_text_createTimeLeftString, {left, uint32_t(seconds)});
    uint32_t cstr = strIsLong(left) ? ld32(left + 8) : left + 1;
    uint32_t s = newStr(f, cstr);
    call(fn::Text_setString, {text, s});
    strFree(s);
    strFree(left);
}

// x of the first avatar of the ranking line-up
float SceneMyPage_getAvatarLineupStartX(uint32_t self) {
    float margin = bf(call(SceneUtil_getSideMargin, {}));
    int32_t n = int32_t(ld32(self + 0x394) - ld32(self + OFF_IDS2)) >> 3;
    float x = margin / bf(0xC026353Fu) + 365.0f;
    if (int32_t(ld32(self + OFF_COLUMNS)) > n) {
        Frame f;
        uint32_t vs = f.alloc(8);
        call(fn::Director_getVisibleSize, {vs, call(fn::Director_getInstance, {})});
        uint32_t cnt = uint32_t(int32_t(ld32(self + 0x394) - ld32(self + OFF_IDS2)) >> 3);
        float a = float(cnt) + 1.5f;
        float b = ldf(vs) / bf(DEFAULT_SCALE);
        float c = a * -60.0f;
        x = x + (b + c) * 0.5f;
    }
    return x + ldf(self + 0x38C);
}

RW_FUNCTION(0x646B30, "SceneMyPage::onChangeProfile", SceneMyPage_onChangeProfile);
RW_FUNCTION(0x646B64, "SceneMyPage::backScene", SceneMyPage_backScene);
RW_FUNCTION(0x646C64, "SceneMyPage::canTouchButton", SceneMyPage_canTouchButton);
RW_FUNCTION(0x646C98, "SceneMyPage::canTouchRankingModeButton", SceneMyPage_canTouchRankingModeButton);
RW_FUNCTION(0x646CD4, "SceneMyPage::gotoSceneRanking", SceneMyPage_gotoSceneRanking);
RW_FUNCTION(0x646DE8, "SceneMyPage::initFieldScale", SceneMyPage_initFieldScale);
RW_FUNCTION(0x646E44, "SceneMyPage::changeParty", SceneMyPage_changeParty);
RW_FUNCTION(0x646A30, "SceneMyPage::updateExpirationDate", SceneMyPage_updateExpirationDate);
RW_FUNCTION(0x6460F4, "SceneMyPage::getAvatarLineupStartX", SceneMyPage_getAvatarLineupStartX);

// ------------------------------------------------------------------------------------------------ more
namespace {
constexpr uint32_t M_clearMotionEvents = 0x63FA5B, M_addChangeMotionEvent = 0x63FA8D;
constexpr uint32_t ImageMap_index = 0x6469B1;           // std::map<int, Image*>::operator[]
constexpr uint32_t UIntMap_index = 0x5AF52D;            // std::map<int, unsigned>::operator[]
constexpr uint32_t DownloadManager_start = 0x4DED65;    // (dm, url, const function&, ?, ?)
constexpr uint32_t Texture2D_ctor = rwlib::cocos2d::Texture2D::ctor_base, Texture2D_initWithImage = rwlib::cocos2d::Texture2D::initWithImage, Texture2D_getContentSize = rwlib::cocos2d::Texture2D::getContentSize;
constexpr uint32_t VT_banner_36 = 0xF61370;

// queue motion `motion` for every line-up actor
void lineupMotion(uint32_t self, uint32_t motion) {
    if (int32_t(ld32(self + 0x2E4) - ld32(self + OFF_ACTORS_B)) < 1) return;
    int32_t i = 0;
    do {
        call(M_addChangeMotionEvent, {self, ld32(ld32(self + OFF_ACTORS_B) + uint32_t(i) * 4), motion});
        i++;
    } while (i < int32_t(ld32(self + 0x2E4) - ld32(self + OFF_ACTORS_B)) >> 2);
}
}  // namespace

// the line-up avatars idle (stopped), walk (slow scroll) or run (fast scroll)
void SceneMyPage_updateScrollAvatarsMotion(uint32_t self, float speed, uint32_t holding) {
    float a = std::fabs(speed);
    if (!(a >= 1.0f || a != a) && !holding) {
        st32(self + 0x3D0, 0x1B);
        lineupMotion(self, 0x1B);
        return;
    }
    float slow = ldf(self + 0x3B4);
    bool run = a >= slow || a != a || slow != slow;
    uint32_t motion = run ? 0x18 : 0x1C;
    if (ld32(self + 0x3D0) == motion) return;
    call(M_clearMotionEvents, {self});
    st32(self + 0x3D0, motion);
    lineupMotion(self, motion);
}

// A shop banner image: from the cache (texture scaled to the slide height), else download it ($_36).
// Returns 1 when a download started.
uint32_t SceneMyPage_loadShopBanner(uint32_t self, uint32_t sprite, int32_t id, uint32_t url) {
    const uint32_t end = self + 0x3D8;
    uint32_t n = ld32(self + OFF_IMAGES);
    for (; n != end; n = treeNext(n))
        if (int32_t(ld32(n + 0x10)) == id) break;
    if (n == end) {
        Frame f;
        uint32_t key = f.alloc(4);
        st32(key, uint32_t(id));
        st32(call(ImageMap_index, {self + OFF_IMAGES, key}), 0);
        uint32_t dm = call(DownloadManager_getInstance, {});
        uint32_t fo = newFunction(f, VT_banner_36, {self, uint32_t(id), sprite});
        uint32_t handle = call(DownloadManager_start, {dm, url, fo, 0, 0});
        st32(call(UIntMap_index, {self + OFF_DOWNLOADS, key}), handle);
        functionDestroy(fo);
        return 1;
    }
    uint32_t image = ld32(n + 0x14);
    if (!image) return 0;
    uint32_t tex = call(fn::operator_new, {0x40});
    call(Texture2D_ctor, {tex});
    if (call(Texture2D_initWithImage, {tex, image}) == 1) {
        uint32_t slide = ld32(self + 0x400);
        float h = ldf(vcall(slide, node::getContentSize, {slide}) + 4);
        Frame f;
        uint32_t size = f.alloc(8);
        call(Texture2D_getContentSize, {size, tex});
        vcall(sprite, node::setScale, {sprite, fb(h / ldf(size + 4))});
        vcall(sprite, 0x27C, {sprite, tex});   // setTexture
    }
    call(smain::lib::Ref_release, {tex});
    return 0;
}

// The avatar linkage notice popup; OK ($_34 {popup, layer, shadow, done}) closes it and runs `done`
void SceneMyPage_openLinkageAvatarPopup(uint32_t, uint32_t done) {
    constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, SceneManager_getPopupLayer2 = 0x613E71;
    constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_createOpenPopupAnimation = 0x45BC05;
    constexpr uint32_t FunctionVV_move = 0x47F905;
    constexpr uint32_t VT_34 = 0xF613C8, VT_35 = 0xF6139C;
    constexpr uint32_t S_JSON = 0xE038C5, S_TXT_WORDING = 0xDF862F, S_BUTTON_OK = 0xDF8688, S_TXT_OK = 0xDF869F;
    call(AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(SceneManager_getPopupLayer2, {call(SceneManager_getInstance, {})});
    uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    uint32_t op = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(wl::Node_runAction, {shadow, call(fn::FadeTo_create, {fb(0.1f), op})});
    vcall(layer, node::addChild, {layer, shadow});
    uint32_t popup = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_JSON});
    vcall(shadow, node::addChild, {shadow, popup});
    Frame f;
    {
        uint32_t v = f.alloc(8);
        call(wl::Vec2_ctorXY, {v, fb(0.5f), fb(0.5f)});
        vcall(popup, node::setAnchorPoint, {popup, v});
        call(wl::Vec2_dtor, {v});
    }
    vcall(popup, node::setPosition, {popup, vcall(shadow, node::getPosition, {shadow})});
    titleLabel(popup, S_TXT_WORDING, 11, 104700016);
    uint32_t cap = f.alloc(0x24);   // {popup, layer, shadow, pad, function<void()> done}
    st32(cap, popup);
    st32(cap + 4, layer);
    st32(cap + 8, shadow);
    call(fn::function_vv_copy, {cap + 0x10, done});
    uint32_t ok = findNode(popup, S_BUTTON_OK, 9);
    titleLabel(ok, S_TXT_OK, 6, 100100001);
    {
        uint32_t tmp = f.alloc(0x24);
        st32(tmp, ld32(cap));
        st32(tmp + 4, ld32(cap + 4));
        st32(tmp + 8, ld32(cap + 8));
        functionCopy(tmp + 0x10, cap + 0x10);
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x30});
        if (heap) {
            st32(heap, VT_34);
            st32(heap + 8, ld32(tmp));
            st32(heap + 0xC, ld32(tmp + 4));
            st32(heap + 0x10, ld32(tmp + 8));
            call(FunctionVV_move, {heap + 0x18, tmp + 0x10});
        }
        st32(fo + 16, heap);
        call(Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        functionDestroy(tmp + 0x10);
    }
    {
        Frame g;
        uint32_t fo = newFunction(g, VT_35, {});
        call(wl::Node_runAction, {popup, call(SceneUtil_createOpenPopupAnimation, {popup, fo})});
        functionDestroy(fo);
    }
    functionDestroy(cap + 0x10);
}

RW_FUNCTION(0x646428, "SceneMyPage::openLinkageAvatarPopup", SceneMyPage_openLinkageAvatarPopup);
RW_FUNCTION(0x63FB08, "SceneMyPage::updateScrollAvatarsMotion", SceneMyPage_updateScrollAvatarsMotion);
RW_FUNCTION(0x6467FC, "SceneMyPage::loadShopBanner", SceneMyPage_loadShopBanner);
