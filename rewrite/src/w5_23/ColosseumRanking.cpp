// Coliseum > Ranking / Tier Rewards. RankingScrollView's rows (getLayoutPanel, createRow, changeRow -> the Lux /
// party / colosseum layer) with the colosseum row (initColosseumLayer, IDA split it in chunks: rank image or text,
// name, points, tier and its round count, the season date, title panel, the cursor on the player's own row) and
// the avatar icon (initAvatarLayer: the cached icon replacing the template's, its button and lambda);
// RankingRewardScrollView::initColosseumRewardLayout (a tier's rewards, the cleared mask); SceneRanking::
// callColosseumRankingAPI and its two chained lambdas; SceneRankingReward::backScene's two inner lambdas.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269, SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5, SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_numberToAtlasString = 0x466C41;
constexpr uint32_t SceneUtil_updateUserTitlePanel = 0x46974D, SceneUtil_setItemImageData = 0x46C71D;
constexpr uint32_t SceneUtil_setAdditionalKerning = 0x466BA9;
constexpr uint32_t aeabi_memset = 0x40E7A0, StringInit = 0x47CD93, ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t Date_makeLocalDate = 0x4927CD, TextData_format = 0x492929;
constexpr uint32_t UserAssign = 0x55204D;          // ColosseumRanking::User::operator=
constexpr uint32_t ColosseumStage_create = 0x77CF95, Colosseum_create = 0x77C4E9;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create, Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE, TextAtlas_setString = rwlib::cocos2d::ui::TextAtlas::setString;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t AvatarIconCache_find = 0x5CA9A9, AvatarIconCache_add = 0x5CAA57, AvatarUtil_makeStyle = 0x497105;
constexpr uint32_t VecInt_copy = 0x4AF707, VecInt_dtor = 0x478961, SortInt = 0x4EC65D, IntArray4_at = 0x508555;
constexpr uint32_t floatsisf = 0xCD6D68;
constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69, FunctionVB_call = 0x474139;
constexpr uint32_t API_rankingRewardCheck = 0x4D32C5, API_rankingColosseum = 0x4D2EF9;
constexpr uint32_t API_rankingColosseumOwn = 0x4D303D, API_stageGet160310 = 0x4CADB9, API_raidGet = 0x4CBEC5;
constexpr uint32_t setIncentiveItem = 0x55E4D1;
constexpr uint32_t getLayoutPanel = 0x54FAA1, initPartyLayer = 0x54FDF5, initLuxLayer = 0x550B21;
constexpr uint32_t initColosseumLayer = 0x55149D, initAvatarLayer = 0x5520D1;
}  // namespace g

// RankingScrollView
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518, F_IDS = 0x628, F_TYPE = 0x650, F_SEASON = 0x654;
constexpr uint32_t F_TEMPLATE = 0x658, F_ICONS = 0x65C;
constexpr uint32_t V_changeRow = 0x340;
constexpr uint32_t API_RANKING_WEEK = 0x2E4, API_RANKING_NOW = 0x2DC;    // shared_ptr: list of 48-byte users at +0x14
constexpr uint32_t API_COLOSSEUM = 0x24C, API_ME = 0x74, API_CLEARED_TIERS = 0x254;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t VT_Avatar = 0xF58A6C;          // initAvatarLayer's button lambda {-, this, row, user id}
constexpr uint32_t VT_Check = 0xF6A3B0, VT_Ranking = 0xF6A3DC, VT_Own = 0xF6A408;   // {function<void(bool)>}
constexpr uint32_t VT_StageGet = 0xF6ADD4, VT_RaidGet = 0xF6AE00;      // {bool}
constexpr uint32_t S_AtlasNumL = 0xDFD2A4, S_Panel = 0xDFD420, S_PanelEff = 0xDFE94F, S_MyName = 0xDF8DD9;
constexpr uint32_t S_RankNum = 0xDFEA2A, S_TxtNumLabel = 0xDFD8DC, S_RankNumPng = 0xDFEA33, S_RankLuxPanel = 0xDFEA90;
constexpr uint32_t S_TxtGrade = 0xDFEA9F, S_TxtGradeLabel = 0xDFEAA9, S_TxtGradeNum = 0xDFEAB9, S_TxtDate = 0xDFEACC;
constexpr uint32_t S_AnimeCursor = 0xDFEA70, S_CursorAnim = 0xDFEAD5, S_FmtInt = 0xDF86DE, S_FmtFrac = 0xDF8F35;
constexpr uint32_t S_Avatar = 0xDF8F56, S_AvatarIconObj = 0xDF8F5D, S_ImageButtonAvatar = 0xDFD9F4, S_Empty = 0xDF8A1A;
constexpr uint32_t S_Area = 0xDF9378, S_ItemPanelD = 0xDFD4B4, S_TxtIncentiveD = 0xDFEC76, S_IncentiveFmt = 0xDFEC8C;
constexpr uint32_t S_PanelMask = 0xDFD14B, S_ClearMark = 0xDFF0CD;
constexpr uint32_t V_getParent = 0x11C, V_removeFromParent = 0x124, V_getLocalZOrder = 0x18, V_setPositionXY = 0x5C;
constexpr uint32_t V_setOpacity = 0x214, V_setScaleXY = 0x48, V_getAnimation = 0x298, V_playWithIndex = 0x70;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline uint32_t findC(uint32_t root, uint32_t name) { return findNode(root, name, host_strlen(name)); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void shadow(uint32_t t) { call(g::SceneUtil_setShadow, {t}); }
inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
uint32_t read(Frame &f, uint32_t sb, uint32_t off, uint32_t len) {
    const uint32_t v = f.alloc((len + 3) & ~3u);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, len, off});
    return v;
}
uint32_t read32(Frame &f, uint32_t sb, uint32_t off) { return ld32(read(f, sb, off, 4)); }
void titleLabelS(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(g::SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

// std::string copy constructor
void stringCopy(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        call(g::StringInit, {dst, ld32(src + 8), ld32(src + 4)});
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}
// a heap __func {vt, std::function<void(bool)> moved from `tmp`} held by a fresh std::function
uint32_t heapCallback(Frame &f, uint32_t vt, uint32_t tmp) {
    const uint32_t fo = newEmptyFunction(f);
    const uint32_t heap = call(fn::operator_new, {0x20});
    if (heap) {
        st32(heap, vt);
        call(g::FunctionVB_move, {heap + 8, tmp});
    }
    st32(fo + 16, heap);
    return fo;
}
}  // namespace

struct ColosseumRanking {
    static uint32_t getLayoutPanel(uint32_t s, uint32_t);
    static uint32_t createRow(uint32_t s, uint32_t index, uint32_t arg);
    static uint32_t changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t arg);
    static void initColosseumLayer(uint32_t s, uint32_t widget, uint32_t index, uint32_t arg);
    static void initAvatarLayer(uint32_t s, uint32_t widget, uint32_t lo, uint32_t hi, uint32_t coord);
    static void initColosseumRewardLayout(uint32_t, uint32_t widget, uint32_t id, uint32_t);
    static void callColosseumRankingAPI(uint32_t done);
    static void checked(uint32_t fo, uint32_t ok);
    static void ranked(uint32_t fo, uint32_t ok);
    static void rewardBackStage(uint32_t fo, uint32_t ok);
    static void rewardBackRaid(uint32_t fo, uint32_t ok);
};

// ------------------------------------------------------------------------------------------------ RankingScrollView
uint32_t ColosseumRanking::getLayoutPanel(uint32_t s, uint32_t) {
    const uint32_t panel = call(g::Widget_create, {});
    const uint32_t cell = call(g::Widget_clone, {ld32(s + F_TEMPLATE)});
    vcall(panel, node::setAnchorPoint, {panel, ld32(GOT_ANCHOR_MIDDLE)});
    vcall(panel, node::addChild, {panel, cell});
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, ld32(s + F_ITEM_W), ld32(s + F_ITEM_H)});
        vcall(panel, node::setContentSize, {panel, size});
    }
    vcall(panel, node::setPositionX, {panel, fbits(ldf(s + F_ITEM_W) * 0.5f)});
    return panel;
}

// A row (non-party rows tighten the points atlas), filled by changeRow with the entry at ids[index]
uint32_t ColosseumRanking::createRow(uint32_t s, uint32_t index, uint32_t arg) {
    const uint32_t ids = ld32(s + F_IDS);
    if (((ld32(s + F_IDS + 4) - ids) >> 2) <= index) call(g::ThrowOutOfRange, {s + F_IDS});
    const uint32_t panel = call(g::getLayoutPanel, {s});
    if (ld32(s + F_TYPE) != 1) call(g::SceneUtil_setAdditionalKerning, {find(panel, S_AtlasNumL, 0xF), uint32_t(-21)});
    vcall(s, V_changeRow, {s, panel, ld32(ids + 4 * index), arg});
    return panel;
}

uint32_t ColosseumRanking::changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
    const uint32_t ids = ld32(s + F_IDS);
    if (((ld32(s + F_IDS + 4) - ids) >> 2) <= index) call(g::ThrowOutOfRange, {s + F_IDS});
    if (!find(widget, S_Panel, 5)) return 1;
    switch (ld32(s + F_TYPE)) {
    case 0: call(g::initLuxLayer, {s, widget, ld32(ids + 4 * index)}); break;
    case 2: call(g::initColosseumLayer, {s, widget, ld32(ids + 4 * index)}); break;
    case 1: call(g::initPartyLayer, {s, widget, ld32(ids + 4 * index)}); break;
    }
    visible(find(widget, S_PanelEff, 9), 0);
    return 0;
}

// A colosseum ranking row for entry `index` of the season's list (this week or now)
void ColosseumRanking::initColosseumLayer(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
    Frame f;
    const uint32_t user = f.alloc(0x30);
    call(g::aeabi_memset, {user, 0x18, 0});
    st8(user + 0x2B, 0);
    const uint32_t season = ld32(s + F_SEASON);
    if (season == 2 || season == 0) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t at = season == 2 ? API_RANKING_WEEK : API_RANKING_NOW;
        const uint32_t ctl = ld32(api + at + 4), obj = ld32(api + at);
        addRef(ctl);
        const uint32_t list = obj + 0x14;
        if (((ld32(list + 4) - ld32(list)) >> 4) * 0xAAAAAAABu <= index) call(g::ThrowOutOfRange, {list});
        call(g::UserAssign, {user, ld32(list) + 48 * index});
        release(ctl);
    }
    const uint32_t a = ld32(user), b = ld32(user + 8), c = ld32(user + 0x10);
    {
        const uint32_t id = read(f, a, 0, 8);
        const uint32_t lo = ld32(id), hi = ld32(id + 4);
        const uint32_t coord = read(f, c, 0, 0x30);
        call(g::initAvatarLayer, {s, widget, lo, hi, coord});
    }
    {
        const uint32_t t = find(widget, S_MyName, 7);
        shadow(t);
        setTextC(t, read(f, a, 0x14, 0x21));
    }
    {
        const uint32_t atlas = find(widget, S_AtlasNumL, 0xF);
        const uint32_t points = read32(f, b, 8);
        Frame g2;
        const uint32_t str = g2.alloc(12);
        call(g::SceneUtil_numberToAtlasString, {str, points});
        call(g::TextAtlas_setString, {atlas, str});
        strFree(str);
    }
    const uint32_t rankImage = find(widget, S_RankNum, 8);
    const uint32_t rankText = find(widget, S_TxtNumLabel, 0xD);
    visible(rankImage, 0);
    visible(rankText, 0);
    const int32_t rank = int32_t(ld32(user + 0x2C));
    if (rank > 10) {
        visible(rankText, 1);
        shadow(rankText);
        Frame g2;
        const uint32_t td = textData(g2, 104100003);
        setTextC(rankText, stringBuffer(g2, {textDataStr(td), uint32_t(rank)}));
        sharedRelease(td);
    } else {
        visible(rankImage, 1);
        Frame g2;
        const uint32_t path = stringBuffer(g2, {S_RankNumPng, uint32_t(rank)});
        const uint32_t str = newStr(g2, path, host_strlen(path));
        call(g::ImageView_loadTexture, {rankImage, str, 0});
        strFree(str);
    }
    visible(find(widget, S_RankLuxPanel, 0xE), 0);
    uint32_t rounds = 0;
    titleLabelS(widget, S_TxtGrade, 9, 103300007);
    {
        const uint32_t t = find(widget, S_TxtGradeLabel, 0xF);
        shadow(t);
        Frame g2;
        setTextC(t, stringBuffer(g2, {S_FmtInt, ld32(user + 0x18)}));
    }
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t stage = f.alloc(8);
        call(g::ColosseumStage_create, {stage, ld32(ld32(api + API_COLOSSEUM))});
        int32_t tier = int32_t(read32(f, ld32(stage), 0x1C));
        const uint32_t col = f.alloc(8);
        for (;; ++tier) {
            const int32_t last = int32_t(read32(f, ld32(stage), 0x20));
            if (tier > last) break;
            call(g::Colosseum_create, {col, uint32_t(tier)});
            if (read32(f, ld32(col), 8) == ld32(user + 0x18)) rounds = read32(f, ld32(col), 0x10);
            release(ld32(col + 4));
        }
        {
            const uint32_t t = find(widget, S_TxtGradeNum, 0x12);
            shadow(t);
            Frame g2;
            setTextC(t, stringBuffer(g2, {S_FmtFrac, ld32(user + 0x1C), rounds}));
        }
        {
            const uint32_t t = find(widget, S_TxtDate, 8);
            shadow(t);
            Frame g2;
            const uint32_t date = g2.alloc(8);
            call(g::Date_makeLocalDate, {date, user + 0x24});
            const uint32_t y = ld32(date) & 0xFFFF, md = (ld32(date) >> 16) & 0xFFFF, hm = ld32(date + 4) & 0xFFFF;
            const uint32_t sec = ld8(date + 6);
            const uint32_t out = g2.alloc(12);
            call(g::TextData_format, {out, 0, 103901058, y, md & 0xFF, md >> 8, hm & 0xFF, hm >> 8, sec});
            setTextC(t, strData(out));
            strFree(out);
        }
        call(g::SceneUtil_updateUserTitlePanel, {widget, user + 8});
        const uint32_t api2 = call(fn::APIManager_getInstance, {});
        uint32_t cursor = find(widget, S_AnimeCursor, 0xB);
        bool own = false;
        if (ld32(api2 + API_ME)) {
            const uint32_t id = read(f, a, 0, 8);
            const uint32_t lo = ld32(id), hi = ld32(id + 4);
            const uint32_t me = read(f, ld32(api2 + API_ME), 0, 8);
            own = lo == ld32(me) && hi == ld32(me + 4);
        }
        if (own) {
            if (!cursor) {
                Frame g2;
                const uint32_t name = newStr(g2, S_CursorAnim, 0x14);
                const uint32_t arm = call(g::Armature_create, {name});
                strFree(name);
                call(g::Ref_retain, {arm});
                const uint32_t anim = vcall(arm, V_getAnimation, {arm});
                vcall(anim, V_playWithIndex, {anim, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
                const uint32_t key = newStr(g2, S_AnimeCursor, 0xB);
                vcall(widget, node::addChildZName, {widget, arm, 10, key});
                strFree(key);
                cursor = find(widget, S_AnimeCursor, 0xB);
                const uint32_t eff = find(widget, S_PanelEff, 9);
                const uint32_t pos = g2.alloc(8);
                call(g::Vec2_copy, {pos, vcall(eff, node::getPosition, {eff})});
                stf(pos, ldf(pos) + 12.0f);
                stf(pos + 4, ldf(pos + 4) + 1.0f);
                vcall(cursor, node::setPosition, {cursor, pos});
                call(g::Vec2_dtor, {pos});
            }
            visible(cursor, 1);
        } else if (cursor) {
            visible(cursor, 0);
        }
        release(ld32(stage + 4));
    }
    release(ld32(user + 0x14));
    release(ld32(user + 0xC));
    release(ld32(user + 4));
}

// The row's avatar icon: the cached one (made from the coordinate when missing) takes the template icon's place
// (or, without one, the "Avatar" image's, which is hidden), then the avatar button and its lambda
void ColosseumRanking::initAvatarLayer(uint32_t s, uint32_t widget, uint32_t lo, uint32_t hi, uint32_t coord) {
    const uint32_t avatar = find(widget, S_Avatar, 6);
    const uint32_t iconObj = find(widget, S_AvatarIconObj, 0xF);
    uint32_t icon = call(g::AvatarIconCache_find, {ld32(s + F_ICONS), 0, lo, hi});
    auto ensureIcon = [&] {
        if (icon) return;
        Frame f;
        const uint32_t style = f.alloc(0x40);
        call(g::AvatarUtil_makeStyle, {style, coord, 0});
        icon = call(g::AvatarIconCache_add, {ld32(s + F_ICONS), 0, lo, hi, style});
        call(g::SafeBuffer_dtor, {style});
    };
    if (iconObj) {
        ensureIcon();
        if (icon != iconObj) {
            vcall(icon, node::setPosition, {icon, vcall(iconObj, node::getPosition, {iconObj})});
            const uint32_t parent = vcall(iconObj, V_getParent, {iconObj});
            vcall(icon, V_removeFromParent, {icon});
            const uint32_t z = vcall(iconObj, V_getLocalZOrder, {iconObj});
            Frame f;
            const uint32_t name = newStr(f, S_AvatarIconObj, 0xF);
            vcall(parent, node::addChildZName, {parent, icon, z, name});
            strFree(name);
            vcall(iconObj, V_removeFromParent, {iconObj});
        }
    } else {
        ensureIcon();
        vcall(icon, V_setPositionXY, {icon, fbits(200.0f), fbits(79.5f)});
        vcall(icon, V_removeFromParent, {icon});
        const uint32_t parent = vcall(avatar, V_getParent, {avatar});
        const uint32_t z = vcall(avatar, V_getLocalZOrder, {avatar});
        {
            Frame f;
            const uint32_t name = newStr(f, S_AvatarIconObj, 0xF);
            vcall(parent, node::addChildZName, {parent, icon, z, name});
            strFree(name);
        }
        vcall(avatar, V_setOpacity, {avatar, 0});
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_copy, {pos, vcall(avatar, node::getPosition, {avatar})});
        stf(pos + 4, 49.5f);
        vcall(avatar, node::setPosition, {avatar, pos});
        vcall(avatar, V_setScaleXY, {avatar, fbits(1.0f), fbits(0.7f)});
        call(g::Vec2_dtor, {pos});
    }
    uint32_t button = find(widget, S_ImageButtonAvatar, 0x12);
    if (!button) {
        Frame f;
        const uint32_t empty = newStr(f, S_Empty, 0);
        button = call(g::ImageButton_create, {empty, 0});
        strFree(empty);
        call(g::ImageButton_setTargetView, {button, avatar, fbits(1.0f)});
        const uint32_t name = newStr(f, S_ImageButtonAvatar, 0x12);
        vcall(avatar, node::addChildZName, {avatar, button, 10, name});
        strFree(name);
    }
    Frame f;
    const uint32_t fo = newEmptyFunction(f);
    const uint32_t heap = call(fn::operator_new, {0x18});
    if (heap) {
        st32(heap, VT_Avatar);
        st32(heap + 8, s);
        st32(heap + 0xC, widget);
        st32(heap + 0x10, lo);
        st32(heap + 0x14, hi);
    }
    st32(fo + 16, heap);
    call(g::ImageButton_addTouchEventListener, {button, fo});
    functionDestroy(fo);
}

// ------------------------------------------------------------------------------------------------ rewards
// A tier's reward row: its name and grade titles, up to four rewards (icon, its item panel, "<name> x<count>"),
// and the cleared mask / mark when the tier is in the player's cleared tiers
void ColosseumRanking::initColosseumRewardLayout(uint32_t, uint32_t widget, uint32_t id, uint32_t) {
    Frame f;
    const uint32_t cleared = f.alloc(12);
    call(g::VecInt_copy, {cleared, call(fn::APIManager_getInstance, {}) + API_CLEARED_TIERS});
    const uint32_t colPtr = f.alloc(8);
    call(g::Colosseum_create, {colPtr, id});
    const uint32_t col = ld32(colPtr);
    {
        const uint32_t t = find(widget, S_TxtGradeLabel, 0xF);
        shadow(t);
        Frame g2;
        const uint32_t td = textData(g2, 103300015);
        const uint32_t fmt = textDataStr(td);
        const uint32_t grade = read32(g2, col, 8);
        setTextC(t, stringBuffer(g2, {fmt, grade}));
        sharedRelease(td);
    }
    {
        const uint32_t t = find(widget, S_TxtGrade, 9);
        shadow(t);
        setTextId(t, 103300016);
    }
    const uint32_t area = find(widget, S_Area, 4);
    int32_t x = 0;
    for (uint32_t k = 0; k < 4; ++k, x += 88) {
        Frame g2;
        const uint32_t item = findC(area, stringBuffer(g2, {S_ItemPanelD, k + 1}));
        const uint32_t txt = findC(widget, stringBuffer(g2, {S_TxtIncentiveD, k + 1}));
        {
            const uint32_t pos = g2.alloc(8);
            call(g::Vec2_ctor, {pos, call(g::floatsisf, {uint32_t(x)}), 0});
            vcall(item, node::setPosition, {item, pos});
            call(g::Vec2_dtor, {pos});
        }
        const uint32_t count = read32(g2, col, 0x78);
        if (!(int32_t(count) > int32_t(k))) {
            visible(item, 0);
            visible(txt, 0);
            continue;
        }
        visible(item, 1);
        visible(txt, 1);
        const uint32_t strA = g2.alloc(12), strB = g2.alloc(12);
        for (uint32_t w = 0; w < 12; w += 4) {
            st32(strA + w, 0);
            st32(strB + w, 0);
        }
        uint32_t sub = 0;
        const uint32_t arr = g2.alloc(16);
        auto at = [&](uint32_t off) {
            rw::call(g::SafeBuffer_read, {col, arr, ld32(col + 0xC) + off, 0x10, off});
            return ld32(call(g::IntArray4_at, {arr, k}));
        };
        if (at(0x9C) != 1) sub = at(0xAC);
        {
            const uint32_t type = at(0x7C);
            const uint32_t itemId = at(0x8C);
            rw::call(g::SceneUtil_setItemImageData, {strA, strB, type, itemId, 0});
        }
        const uint32_t type = at(0x7C);
        const uint32_t itemId = at(0x8C);
        const uint32_t argA = g2.alloc(12), argB = g2.alloc(12);
        stringCopy(argA, strB);
        stringCopy(argB, strA);
        const uint32_t amount = at(0xCC);
        rw::call(g::setIncentiveItem, {item, type, sub, itemId, argA, argB, amount, k + 6, 0});
        strFree(argB);
        strFree(argA);
        const uint32_t name = strData(strA);
        const uint32_t n2 = at(0xCC);
        setTextC(txt, stringBuffer(g2, {S_IncentiveFmt, name, n2}));
        shadow(txt);
        strFree(strA);
        strFree(strB);
    }
    {
        const uint32_t less = f.alloc(4);
        call(g::SortInt, {ld32(cleared), ld32(cleared + 4), less});
    }
    uint32_t it = ld32(cleared);
    const uint32_t end = ld32(cleared + 4);
    const int32_t grade = int32_t(read32(f, col, 8));
    for (int32_t len = int32_t(end - it) >> 2; len;) {
        const int32_t half = len / 2;
        if (int32_t(ld32(it + 4 * half)) < grade) {
            it += 4 * half + 4;
            len = len - 1 - half;
        } else {
            len = half;
        }
    }
    uint32_t done = 0;
    if (it != end) done = grade >= int32_t(ld32(it)) ? 1 : 0;
    const uint32_t mask = find(widget, S_PanelMask, 9);
    const uint32_t mark = find(widget, S_ClearMark, 9);
    visible(mask, done);
    visible(mark, done);
    release(ld32(colPtr + 4));
    call(g::VecInt_dtor, {cleared});
}

// ------------------------------------------------------------------------------------------------ API chains
// SceneRanking::callColosseumRankingAPI(done): the reward check, then (checked) the ranking, then (ranked) the
// player's own ranking, then done
void ColosseumRanking::callColosseumRankingAPI(uint32_t done) {
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(g::FunctionVB_copy, {copy, done});
    st32(sceneManager() + 0x290, 2);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t tmp = f.alloc(20);
    functionCopy(tmp, copy);
    const uint32_t fo = heapCallback(f, VT_Check, tmp);
    call(g::API_rankingRewardCheck, {api, 3, fo});
    functionDestroy(fo);
    functionDestroy(tmp);
    functionDestroy(copy);
}

namespace {
// a chained API lambda {function<void(bool)> done}: on success the next request with done moved along, else done
void chain(uint32_t fo, uint32_t ok, uint32_t vt, uint32_t request) {
    const uint32_t done = fo + 8;
    if (ld8(ok)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t tmp = f.alloc(20);
        call(g::FunctionVB_copy, {tmp, done});
        const uint32_t next = heapCallback(f, vt, tmp);
        call(request, {api, next});
        functionDestroy(next);
        functionDestroy(tmp);
    } else if (ld32(done + 16)) {
        call(g::FunctionVB_call, {done, 0});
    }
}
// SceneRankingReward::backScene's inner lambdas: on success refresh the stages / raids
void refresh(uint32_t ok, uint32_t vt, uint32_t request) {
    if (!ld8(ok)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, vt, {});
    st8(fo + 4, 0);
    call(request, {api, fo});
    functionDestroy(fo);
}
}  // namespace

void ColosseumRanking::checked(uint32_t fo, uint32_t ok) { chain(fo, ok, VT_Ranking, g::API_rankingColosseum); }
void ColosseumRanking::ranked(uint32_t fo, uint32_t ok) { chain(fo, ok, VT_Own, g::API_rankingColosseumOwn); }
void ColosseumRanking::rewardBackStage(uint32_t, uint32_t ok) { refresh(ok, VT_StageGet, g::API_stageGet160310); }
void ColosseumRanking::rewardBackRaid(uint32_t, uint32_t ok) { refresh(ok, VT_RaidGet, g::API_raidGet); }

RW_FUNCTION(0x54FAA0, "RankingScrollView::getLayoutPanel", ColosseumRanking::getLayoutPanel);
RW_FUNCTION(0x54FB68, "RankingScrollView::createRow", ColosseumRanking::createRow);
RW_FUNCTION(0x54FC6C, "RankingScrollView::changeRow", ColosseumRanking::changeRow);
RW_FUNCTION(0x55149C, "RankingScrollView::initColosseumLayer", ColosseumRanking::initColosseumLayer);
RW_FUNCTION(0x5520D0, "RankingScrollView::initAvatarLayer", ColosseumRanking::initAvatarLayer);
RW_FUNCTION(0x560018, "RankingRewardScrollView::initColosseumRewardLayout", ColosseumRanking::initColosseumRewardLayout);
RW_FUNCTION(0x6FBBE0, "SceneRanking::callColosseumRankingAPI", ColosseumRanking::callColosseumRankingAPI);
RW_FUNCTION(0x702878, "SceneRanking::callColosseumRankingAPI $_5 __func::operator() (sub_702878)", ColosseumRanking::checked);
RW_FUNCTION(0x702B18, "SceneRanking::callColosseumRankingAPI $_5 inner __func::operator() (sub_702B18)", ColosseumRanking::ranked);
RW_FUNCTION(0x710B3C, "SceneRankingReward::backScene $_14 stage lambda __func::operator() (sub_710B3C)", ColosseumRanking::rewardBackStage);
RW_FUNCTION(0x710C74, "SceneRankingReward::backScene $_14 raid lambda __func::operator() (sub_710C74)", ColosseumRanking::rewardBackRaid);
