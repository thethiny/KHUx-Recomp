// The ranking list rows of SceneRanking (RankingScrollView::changeRow picks the layer by the list's type):
// initPartyLayer (a party: icon, name, rank image or "nth" text, Lux, the touch lambda $_0 holding a copy of the
// party, the cursor on the player's own party) and initLuxLayer (a player: avatar, name, points, rank, Lux, the
// cursor on the player's own row). Rows come from the API's lists of the season shown (+0x654). Also
// RankingRewardScrollView::setIncentiveItemBalloonRemove (drops the reward balloon from popup layer 3).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41, SceneUtil_getMoneyStr = 0x461DC5;
constexpr uint32_t SceneUtil_updateUserTitlePanel = 0x46974D;
constexpr uint32_t SceneUtil_createPartyRankImagePath = 0x46F08D, SceneUtil_numberToPlaceString = 0x473689;
constexpr uint32_t aeabi_memset = 0x40E7A0, MEMCPY = 0x40E164, ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t StringBuffer_ctor = 0x49140D, TextData_format = 0x492929;
constexpr uint32_t String_assign = 0x498A1D;               // std::string::operator=(const string&)
constexpr uint32_t U64Vector_assign = 0x4D7461;            // vector<unsigned long long>::assign(first, last)
constexpr uint32_t U64Vector_dtor = 0x468499;
constexpr uint32_t Party_copy = 0x4B2E63;                  // hole::network::api::Party(const Party&)
constexpr uint32_t LuxUser_assign = 0x5525B9;              // LuxRanking::User::operator=
constexpr uint32_t Armature_create = 0x79F3C7, Ref_retain = 0x8FD0B5;
constexpr uint32_t ImageView_loadTexture = 0x839731, TextAtlas_setString = 0x83AED7;
constexpr uint32_t Vec2_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t SceneManager_getPopupLayer3 = 0x613E79;
constexpr uint32_t initAvatarLayer = 0x5520D1;             // RankingScrollView::initAvatarLayer(row, id, coordinate)
}  // namespace g

constexpr uint32_t F_SEASON = 0x654;                       // RankingScrollView: the list shown
constexpr uint32_t API_PARTY_WEEK = 0x2D4, API_PARTY_NOW = 0x2CC;   // Party lists (vector at +4, 0x78 each)
constexpr uint32_t API_LUX_WEEK = 0x2C4, API_LUX_NOW = 0x2BC;       // shared_ptr: users at +0x14 (0x20 each)
constexpr uint32_t API_MY_PARTY = 0x84, API_ME = 0x74;     // SafeBuffers: own party (id at +8), own user (id at +0)
constexpr uint32_t PARTY = 0x78, LUX_USER = 0x20;
constexpr uint32_t VT_PartyTouch = 0xF58AC4;               // initPartyLayer $_0 {this, row, Party, rank, Lux}
constexpr uint32_t V_getAnimation = 0x298, V_playWithIndex = 0x70, V_removeChildByName = 0x134;
constexpr int32_t TXT_PLACE = 100221005, TXT_LUX = 100200001, TXT_RANK = 104100003;
constexpr uint32_t S_Panel = 0xDFD420, S_ImageButtonParty = 0xDFEA13, S_ImageButtonAvatar = 0xDFD9F4,
                   S_TxtNameLabel = 0xDFDBB8, S_Icon = 0xDFEA25, S_RankNum = 0xDFEA2A, S_TxtNumLabel = 0xDFD8DC,
                   S_RankNumPng = 0xDFEA33, S_TxtLUX = 0xDFEA5A, S_TxtLUXLabel = 0xDFEA62,
                   S_AnimeCursor = 0xDFEA70, S_CursorParty = 0xDFEA7C, S_CursorLux = 0xDFEAD5,
                   S_PanelEff = 0xDFE94F, S_RankLuxPanel = 0xDFEA90, S_MyName = 0xDF8DD9,
                   S_AtlasNumL = 0xDFD2A4, S_RankGradePanel = 0xDFEAEA, S_Empty = 0xDF8A1A,
                   S_BalloonPanel = 0xDFEFB5;

inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
uint32_t read(Frame &f, uint32_t sb, uint32_t off, uint32_t len) {
    const uint32_t v = f.alloc((len + 3) & ~3u);
    call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, len, off});
    return v;
}

// "Txt_LUX" / its value (Lux as a money string)
void luxTexts(uint32_t row, int32_t lux) {
    {
        const uint32_t t = findNode(row, S_TxtLUX, 7);
        SceneUtil::setShadow(t);
        setTextId(t, TXT_LUX);
    }
    const uint32_t t = findNode(row, S_TxtLUXLabel, 0xD);
    SceneUtil::setShadow(t);
    Frame f;
    const uint32_t s = f.alloc(12);
    call(g::SceneUtil_getMoneyStr, {s, 0, uint32_t(lux), uint32_t(lux >> 31)});
    call(fn::Text_setString, {t, s});
    strFree(s);
}

// the cursor on the player's own row: made once (an armature named "animeCursor" next to Panel_Eff)
void ownCursor(uint32_t row, uint32_t cursor, bool own, uint32_t anim, uint32_t animLen, float dy) {
    if (!own) {
        if (cursor) visible(cursor, 0);
        return;
    }
    if (!cursor) {
        Frame f;
        const uint32_t name = newStr(f, anim, animLen);
        const uint32_t arm = call(g::Armature_create, {name});
        strFree(name);
        call(g::Ref_retain, {arm});
        const uint32_t a = vcall(arm, V_getAnimation, {arm});
        vcall(a, V_playWithIndex, {a, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
        const uint32_t key = newStr(f, S_AnimeCursor, 0xB);
        vcall(row, node::addChildZName, {row, arm, 10, key});
        strFree(key);
        cursor = findNode(row, S_AnimeCursor, 0xB);
        const uint32_t eff = findNode(row, S_PanelEff, 9);
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_copy, {pos, vcall(eff, node::getPosition, {eff})});
        stf(pos, ldf(pos) + 12.0f);
        stf(pos + 4, ldf(pos + 4) + dy);
        vcall(cursor, node::setPosition, {cursor, pos});
        call(g::Vec2_dtor, {pos});
    }
    visible(cursor, 1);
}

// Party::operator= as compiled inline (into a zero-initialised record)
void partyAssign(uint32_t r, uint32_t e) {
    for (uint32_t k = 0; k < 0x10; k += 4) st32(r + k, ld32(e + k));
    call(g::String_assign, {r + 0x10, e + 0x10});
    st32(r + 0x1C, ld32(e + 0x1C));
    st32(r + 0x20, ld32(e + 0x20));
    call(g::String_assign, {r + 0x24, e + 0x24});
    for (uint32_t k = 0x30; k < 0x40; k += 4) st32(r + k, ld32(e + k));
    if (r != e) call(g::U64Vector_assign, {r + 0x40, ld32(e + 0x40), ld32(e + 0x44)});
    call(g::MEMCPY, {r + 0x4C, e + 0x4C, 0x14});
    call(g::String_assign, {r + 0x60, e + 0x60});
    st32(r + 0x70, ld32(e + 0x70));
    st32(r + 0x74, ld32(e + 0x74));
}
// move a std::string (3 words) from `src` to `dst`, leaving `src` empty
void strMove(uint32_t dst, uint32_t src) {
    for (uint32_t k = 0; k < 12; k += 4) {
        st32(dst + k, ld32(src + k));
    }
    st32(src + 8, 0);
    st32(src + 4, 0);
    st32(src, 0);
}

struct RankingLayers {
    static void initPartyLayer(uint32_t self, uint32_t row, uint32_t index, uint32_t) {
        Frame f;
        const uint32_t r = f.alloc(PARTY);
        for (uint32_t off : {0x14u, 0x18u, 0x10u, 0x2Cu, 0x28u, 0x24u, 0x40u, 0x44u, 0x48u}) st32(r + off, 0);
        st8(r + 0x53, 0);
        st8(r + 0x5B, 0);
        for (uint32_t off : {0x60u, 0x64u, 0x68u}) st32(r + off, 0);
        const uint32_t season = ld32(self + F_SEASON);
        if (season == 2 || season == 0) {
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t list = ld32(api + (season == 2 ? API_PARTY_WEEK : API_PARTY_NOW)) + 4;
            if ((((ld32(list + 4) - ld32(list)) >> 3) * 0xEEEEEEEFu) <= index) call(g::ThrowOutOfRange, {list});
            partyAssign(r, ld32(list) + PARTY * index);
        }
        const uint32_t panel = findNode(row, S_Panel, 5);
        uint32_t button = findNode(row, S_ImageButtonParty, 0x11);
        if (!button) {
            {
                Frame t;
                const uint32_t empty = newStr(t, S_Empty, 0);
                button = call(g::ImageButton_create, {empty, 0});
                strFree(empty);
            }
            call(g::ImageButton_setTargetView, {button, panel, fbits(1.0f)});
            Frame t;
            const uint32_t name = newStr(t, S_ImageButtonAvatar, 0x12);
            vcall(panel, node::addChildZName, {panel, button, 0, name});
            strFree(name);
        }
        {
            // $_0 {this, row, Party, rank, Lux}: built on the stack, moved into the heap __func
            const uint32_t lambda = f.alloc(0x80);
            st32(lambda, self);
            st32(lambda + 4, row);
            call(g::Party_copy, {lambda + 8, r});
            st32(lambda + 0x7C, ld32(r + 0x74));
            st32(lambda + 0x78, ld32(r + 0x70));
            Frame t;
            const uint32_t fo = t.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x88});
            if (obj) {
                const uint32_t p = lambda + 8;
                st32(obj, VT_PartyTouch);
                st32(obj + 0xC, row);
                st32(obj + 8, self);
                for (uint32_t k = 0; k < 0x10; k += 4) st32(obj + 0x10 + k, ld32(p + k));
                strMove(obj + 0x20, p + 0x10);
                st32(obj + 0x30, ld32(p + 0x20));
                st32(obj + 0x2C, ld32(p + 0x1C));
                strMove(obj + 0x34, p + 0x24);
                for (uint32_t k = 0; k < 0x10; k += 4) st32(obj + 0x40 + k, ld32(p + 0x30 + k));
                for (uint32_t k = 0; k < 12; k += 4) st32(obj + 0x50 + k, ld32(p + 0x40 + k));
                st32(p + 0x40, 0);
                st32(p + 0x44, 0);
                st32(p + 0x48, 0);
                call(g::MEMCPY, {obj + 0x5C, p + 0x4C, 0x14});
                strMove(obj + 0x70, p + 0x60);
                st32(obj + 0x84, ld32(lambda + 0x7C));
                st32(obj + 0x80, ld32(lambda + 0x78));
            }
            st32(fo + 16, obj);
            call(g::ImageButton_addTouchEventListener, {button, fo});
            functionDestroy(fo);
            strFree(lambda + 0x68);
            call(g::U64Vector_dtor, {lambda + 0x48});
            strFree(lambda + 0x2C);
            strFree(lambda + 0x18);
        }
        {
            const uint32_t t = findNode(row, S_TxtNameLabel, 0xE);
            SceneUtil::setShadow(t);
            setTextC(t, strData(r + 0x10));
        }
        {
            const uint32_t icon = findNode(row, S_Icon, 4);
            Frame t;
            const uint32_t path = t.alloc(12);
            call(g::SceneUtil_createPartyRankImagePath, {path, ld32(r + 0xC)});
            const uint32_t s = newStr(t, strData(path));
            call(g::ImageView_loadTexture, {icon, s, 0});
            strFree(s);
            strFree(path);
        }
        const uint32_t rankImage = findNode(row, S_RankNum, 8);
        const uint32_t rankText = findNode(row, S_TxtNumLabel, 0xD);
        visible(rankImage, 0);
        visible(rankText, 0);
        const int32_t rank = int32_t(ld32(r + 0x70));
        if (rank <= 10) {
            visible(rankImage, 1);
            Frame t;
            const uint32_t png = stringBuffer(t, {S_RankNumPng, uint32_t(rank)});
            const uint32_t s = newStr(t, png);
            call(g::ImageView_loadTexture, {rankImage, s, 0});
            strFree(s);
        } else {
            Frame t;
            const uint32_t place = t.alloc(12);
            call(g::SceneUtil_numberToPlaceString, {place, 9});
            const uint32_t copy = newStr(t, strData(place));
            strFree(place);
            visible(rankText, 1);
            SceneUtil::setShadow(rankText);
            const uint32_t out = t.alloc(12);
            call(g::TextData_format, {out, 0, uint32_t(TXT_PLACE), uint32_t(rank)});
            setTextC(rankText, strData(out));
            strFree(out);
            strFree(copy);
        }
        luxTexts(row, int32_t(ld32(r + 0x74)));
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cursor = findNode(row, S_AnimeCursor, 0xB);
        bool own = false;
        if (const uint32_t mine = ld32(api + API_MY_PARTY)) {
            const uint32_t lo = ld32(r), hi = ld32(r + 4);
            const uint32_t id = read(f, mine, 8, 8);
            own = lo == ld32(id) && hi == ld32(id + 4);
        }
        ownCursor(row, cursor, own, S_CursorParty, 0x13, 6.0f);
        strFree(r + 0x60);
        call(g::U64Vector_dtor, {r + 0x40});
        strFree(r + 0x24);
        strFree(r + 0x10);
    }

    static void initLuxLayer(uint32_t self, uint32_t row, uint32_t index, uint32_t) {
        Frame f;
        const uint32_t user = f.alloc(LUX_USER);
        call(g::aeabi_memset, {user, 0x18, 0});
        const uint32_t season = ld32(self + F_SEASON);
        if (season == 3 || season == 1) {
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t at = season == 3 ? API_LUX_WEEK : API_LUX_NOW;
            const uint32_t ctl = ld32(api + at + 4), obj = ld32(api + at);
            if (ctl) call(fn::shared_add, {ctl});
            const uint32_t list = obj + 0x14;
            if (((ld32(list + 4) - ld32(list)) >> 5) <= index) call(g::ThrowOutOfRange, {list});
            call(g::LuxUser_assign, {user, ld32(list) + LUX_USER * index});
            if (ctl) call(fn::shared_release, {ctl});
        }
        const uint32_t a = ld32(user), b = ld32(user + 8), c = ld32(user + 0x10);
        {
            const uint32_t id = read(f, a, 0, 8);
            const uint32_t lo = ld32(id), hi = ld32(id + 4);
            const uint32_t coord = read(f, c, 0, 0x30);
            call(g::initAvatarLayer, {self, row, lo, hi, coord});
        }
        visible(findNode(row, S_RankLuxPanel, 0xE), 1);
        {
            const uint32_t t = findNode(row, S_MyName, 7);
            SceneUtil::setShadow(t);
            setTextC(t, read(f, a, 0x14, 0x21));
        }
        {
            const uint32_t atlas = findNode(row, S_AtlasNumL, 0xF);
            const uint32_t points = ld32(read(f, b, 8, 4));
            Frame t;
            const uint32_t s = t.alloc(12);
            call(g::SceneUtil_numberToAtlasString, {s, points});
            call(g::TextAtlas_setString, {atlas, s});
            strFree(s);
        }
        const uint32_t rankImage = findNode(row, S_RankNum, 8);
        const uint32_t rankText = findNode(row, S_TxtNumLabel, 0xD);
        visible(rankImage, 0);
        visible(rankText, 0);
        const int32_t rank = int32_t(ld32(user + 0x1C));
        if (rank > 10) {
            visible(rankText, 1);
            SceneUtil::setShadow(rankText);
            Frame t;
            const uint32_t td = textData(t, TXT_RANK);
            setTextC(rankText, stringBuffer(t, {textDataStr(td), uint32_t(rank)}));
            sharedRelease(td);
        } else {
            visible(rankImage, 1);
            Frame t;
            const uint32_t png = stringBuffer(t, {S_RankNumPng, uint32_t(rank)});
            const uint32_t s = newStr(t, png);
            call(g::ImageView_loadTexture, {rankImage, s, 0});
            strFree(s);
        }
        visible(findNode(row, S_RankGradePanel, 0x10), 0);
        luxTexts(row, int32_t(ld32(user + 0x18)));
        call(g::SceneUtil_updateUserTitlePanel, {row, user + 8});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cursor = findNode(row, S_AnimeCursor, 0xB);
        bool own = false;
        if (ld32(api + API_ME)) {
            const uint32_t id = read(f, a, 0, 8);
            const uint32_t lo = ld32(id), hi = ld32(id + 4);
            const uint32_t me = read(f, ld32(api + API_ME), 0, 8);
            own = lo == ld32(me) && hi == ld32(me + 4);
        }
        ownCursor(row, cursor, own, S_CursorLux, 0x14, 1.0f);
        for (uint32_t off : {0x14u, 0xCu, 4u})
            if (const uint32_t ctl = ld32(user + off)) call(fn::shared_release, {ctl});
    }

    // RankingRewardScrollView::setIncentiveItemBalloonRemove
    static void balloonRemove(uint32_t) {
        const uint32_t layer = call(g::SceneManager_getPopupLayer3, {sceneManager()});
        Frame f;
        const uint32_t name = newStr(f, S_BalloonPanel, 0xC);
        vcall(layer, V_removeChildByName, {layer, name, 1});
        strFree(name);
    }
};
}  // namespace

RW_FUNCTION(0x54FDF4, "RankingScrollView::initPartyLayer", RankingLayers::initPartyLayer);
RW_FUNCTION(0x550B20, "RankingScrollView::initLuxLayer", RankingLayers::initLuxLayer);
RW_FUNCTION(0x55FE3C, "RankingRewardScrollView::setIncentiveItemBalloonRemove", RankingLayers::balloonRemove);
