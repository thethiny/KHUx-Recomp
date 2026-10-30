// AvatarInfoDialog::createLayout's page updater ($_8, the slide view's function<Widget*(Widget*, int, bool)>; its
// __func::operator() forwards the captures, the widget to reuse and the page): fills page 0 of Player Information
// (deck status, keyblade and medals on a revolver, Lux, weekly ranking, id, avatar, comment, Facebook share) or
// page 1 (records, munny, Mogu points, last login, time zones, play style). A missing widget comes from the slide
// view's page factory; each page swaps in its own text layer (AvatarInfo_Txt_ver120 / AvatarInfo_Txt2).
//
// The rewrite keeps the original frame (0x18B8 bytes with the pushed registers): getKeyBladeData / getMedalData
// take a struct by value on the stack, and for an empty medal slot of another player the original pushes a medal
// buffer it never wrote (stale frame bytes) into the deck's medal list.
#include "AvatarInfoDialog.hpp"

using namespace scene;
using aid::sbRead;

namespace {
namespace c {
constexpr uint32_t MEMCPY = 0x40E164, MEMSET = 0x40E7A0;    // imports (memset: dst, n, c)
constexpr uint32_t FunctionWidgetInt_call = 0x54A065;      // std::function<Widget*(int)>::operator()(int)
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t TextAtlas_setString = rwlib::cocos2d::ui::TextAtlas::setString;
constexpr uint32_t Label_setAdditionalKerning = 0x88697D;  // (Label*, float)
constexpr uint32_t Text_setTextHorizontalAlignment = 0x83A65F;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t SceneUtil_setMyPageTeamStatus = 0x46874D;  // (node, User&, UserDetail&, Party&, ImageDownloader*, bool)
constexpr uint32_t SceneUtil_setMyPageDeckStatus = 0x469869;  // (node, UserPoint&, int, int)
constexpr uint32_t SceneUtil_calcDeckStatus = 0x469CB5;   // (shared_ptr<UserKeyBladeData> by value, datas&, int*, medals*)
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D;
constexpr uint32_t KeyBladeDataManager_getKeyBladeData = 0x76B659;   // (out, mgr, u64 id, uint, Keyblade by value)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;         // (out, mgr, u64 id, int, Medal by value)
constexpr uint32_t MedalDatas_pushBackSlow = 0x487A39;    // vector<shared_ptr<UserMedalData>>::__push_back_slow_path
constexpr uint32_t Medals_pushBackSlow = 0x4D4E59;        // vector<UserMedal::Medal>::__push_back_slow_path
constexpr uint32_t Medals_dtor = 0x4C513D;                // ~__vector_base<UserMedal::Medal>
constexpr uint32_t MedalDatas_dtor = 0x46B119;            // ~__vector_base<shared_ptr<UserMedalData>>
constexpr uint32_t DeckRevolverView_create = 0x52C471;    // (shared_ptr<UserKeyBladeData>, shared_ptr<UserDetail>) by value
constexpr uint32_t DeckRevolverView_setKeyBladeData = 0x52CC21;
constexpr uint32_t DeckRevolverView_setMedalDatas = 0x52D135;
constexpr uint32_t DeckRevolverView_setOpenMedalInfoPopupEnabled = 0x52D5A5;
constexpr uint32_t MedalInfoFunction_swap = 0x4876D9;     // function<void(MedalInfoButtonType, bool, u64)>::swap
constexpr uint32_t MyRecordScrollView_create = 0x519801;
constexpr uint32_t MyRecordScrollView_resetScroll = 0x51B075;
constexpr uint32_t Timezones_lowerBound = 0x54B0D1;       // std::__lower_bound over set<APIManager::Timezone>
constexpr uint32_t TimezoneText = 0x54A0E5;               // $_8's time zone label lambda (below)
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t createTimeAgoString = 0x492BE9;        // hole::text::createTimeAgoString(string out, long)
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;       // (Style out, const UserAvatar::Coordinate&, int)
constexpr uint32_t Avatar_create = 0x5C5883;              // (const Style&, bool)
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t Style_dtor = 0x45A13D;                 // Avatar::Style's destructor (folded with ~SafeBuffer)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;        // (sm, bool) -> bool
constexpr uint32_t SceneUtil_shareScreenShot = 0x47A559;             // (node, function<void(RenderTexture*, const string&)>)
constexpr uint32_t SceneUtil_openFBSharePhotoPopup = 0x4799D9;
constexpr uint32_t SceneUtil_openPartyInfoRequestPopup = 0x478D59;   // (u64 party, bool, function<void(bool)>)
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t OtherClosedAssign = 0x54AB91;                     // (function<void(bool)>&, $_73's captures)
constexpr uint32_t FunctionVB_copy = 0x45C84D;
constexpr uint32_t openPopUpJoinRequest = 0x53B38D;                  // (dialog, function<void(bool)> by value)
}  // namespace c

// guest string literals
constexpr uint32_t S_TxtLayer1 = 0xDFE653, S_TxtLayer2 = 0xDFE65D, S_Txt1Json = 0xDFE667, S_Txt2Json = 0xDFE695;
constexpr uint32_t S_CloseButton = 0xDF8D2F, S_User = 0xDFE6BD, S_PartyButtonArea = 0xDFE6C2,
                   S_ImageButtonParty = 0xDFE6D4;
constexpr uint32_t S_RevolverView = 0xDF8F3B, S_AreaRevolver = 0xDF8F48, S_AtlasNumL = 0xDFD2A4,
                   S_AtlasLux = 0xDFE6E6, S_TxtWeekly = 0xDFE6F5, S_BaseLuxWeekly = 0xDFE700, S_BaseLux = 0xDFE710,
                   S_WeeklyTotal = 0xDFE719, S_WeeklyRank = 0xDFE733, S_TxtPlace = 0xDFE74C, S_TxtId = 0xDFE756,
                   S_TxtIdLabel = 0xDFE75D, S_Avatar = 0xDF8F56, S_AvatarNode = 0xDFE76A, S_CommentLabel = 0xDFE1CC,
                   S_FbShare = 0xDFE775, S_TxtFbShare = 0xDFE785;
constexpr uint32_t S_ScrollArea = 0xDF9B59, S_RecordScrollView = 0xDFE792, S_TxtMoney = 0xDFE7A3,
                   S_TxtMoneyLabel = 0xDFE7AD, S_TxtMoguPoint = 0xDFE7BD, S_TxtMoguPointLabel = 0xDFE7CB,
                   S_TxtLastLogin = 0xDFE7DF, S_TxtLastLoginLabel = 0xDFDC12, S_TxtTimezone = 0xDFE7ED,
                   S_TxtTimezone1 = 0xDFE7FA, S_TxtTimezone2 = 0xDFE80F, S_TxtTimezone3 = 0xDFE824,
                   S_TxtStyle = 0xDFE548, S_TxtStyleLabel = 0xDFE839;
constexpr uint32_t S_FmtD = 0xDF86DE, S_FmtLlu = 0xDF8A23;   // "%d", "%llu"
constexpr uint32_t ATLASNUM_KERNING_PIXEL = 0xDEC3DC;       // SceneUtil::ATLASNUM_KERNING_PIXEL (float)

// the lambdas' __func vtables
constexpr uint32_t VT_CLOSE = 0xF58678;          // Close_Button touch: (dialog, view)
constexpr uint32_t VT_PARTY = 0xF5859C;          // ImageButton_party touch (heap, 0x28 bytes)
constexpr uint32_t VT_MEDAL_INFO = 0xF58570;     // the revolver's medal info button (no captures)
constexpr uint32_t VT_LUX_GAUGE = 0xF58544;      // calcLuxGaugePer callback: (layer, Lux atlas)
constexpr uint32_t VT_FB_SHARE = 0xF584C0;       // Button_FB_Share touch: (dialog)

// texts
constexpr int32_t TXT_MONEY = 100300001, TXT_MOGU = 103400001, TXT_LAST_LOGIN = 102100008, TXT_TIMEZONE = 102400001,
                  TXT_ZONE_1 = 102100013, TXT_STYLE = 102100005, TXT_STYLE_1 = 102400004, TXT_WEEKLY = 102100025,
                  TXT_WEEKLY_LUX = 100200001, TXT_ID = 102100007, TXT_FB_SHARE = 104900010;

// the updater's captures: the slide view, the dialog, the slide view, the five party buttons, the dialog, the
// slide view, then the shown player's 0x30-byte friend record (its avatar coordinate)
constexpr uint32_t CAP_VIEW = 0x00, CAP_DIALOG = 0x04, CAP_VIEW2 = 0x08, CAP_BUTTONS = 0x0C, CAP_RECORD = 0x28;
constexpr uint32_t VIEW_PAGE_FACTORY = 0x500;    // BaseSlideView's function<Widget*(int)>

// AvatarInfoDialog fields (see AvatarInfoDialog_layout.cpp)
constexpr uint32_t F_IS_OTHER_USER = 0x2C8, F_COMMENT = 0x2D8, F_PARTY_ROLE = 0x2F0, F_TIMEZONES = 0x2F4,
                   F_SKILLS = 0x298;
constexpr uint32_t F_USER = 0x300, F_POINT = 0x308, F_DETAIL = 0x310;   // shared_ptr<SafeBuffer>s
constexpr uint32_t F_MEDALS = 0x318;            // shared_ptr<SafeBuffer>: the shown player's user medals
constexpr uint32_t F_KEYBLADE_ID = 0x33C, F_MEDAL_COUNT = 0x340, F_MEDAL_IDS = 0x348;   // the deck record
constexpr uint32_t F_WEEKLY_LUX = 0x388, F_WEEKLY_RANK = 0x390, F_PARTY = 0x3A8;
constexpr uint32_t F_RECORD_VIEW = 0x4B0, F_REVOLVER = 0x4B4, F_IMAGE_DOWNLOADER = 0x4C4;
// DeckRevolverView: the skills to show (vector*), whether the deck is the viewer's own
constexpr uint32_t RV_SKILLS = 0x318, RV_IS_OWN = 0x314, RV_MEDAL_INFO = 0x270;
constexpr uint32_t PROFILE_LAST_LOGIN = 0x2C0;   // the API cache's profile: the last login Date
constexpr uint32_t MEDAL_SIZE = 0x48, KEYBLADE_SIZE = 0x58;
// cocos2d::Node slots not in scene::node
constexpr uint32_t NODE_GET_LOCAL_Z_ORDER = 0x18, NODE_SET_SCALE_XY = 0x48, TEXT_ATLAS_GET_RENDERER = 0x2D8;

constexpr uint32_t FRAME = 0x18B8;

struct Page {
    uint32_t sp;         // the original frame
    uint32_t cap;        // the lambda's captures
    uint32_t me;         // the dialog
    uint32_t widget;     // the page
    uint32_t layer;      // its text layer
};

void addRef(uint32_t ctrl) { if (ctrl) call(fn::shared_add, {ctrl}); }
void release(uint32_t ctrl) { if (ctrl) call(fn::shared_release, {ctrl}); }
// a shared_ptr copy at dst (a by-value argument or a local)
void copyShared(uint32_t dst, uint32_t obj, uint32_t ctrl) {
    st32(dst, obj);
    st32(dst + 4, ctrl);
    addRef(ctrl);
}

// node->addChild(child, z, name) with a temporary name
void addChildNamed(uint32_t node, uint32_t child, uint32_t z, uint32_t name, uint32_t len) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    vcall(node, node::addChildZName, {node, child, z, s});
    strFree(s);
}
// text shows the characters of the std::string str (through a copy), then str is destroyed
void setTextFromString(uint32_t text, uint32_t str) {
    setTextC(text, strData(str));
    strFree(str);
}
// text shows a printf of one or two words
void setTextFormat(uint32_t text, std::initializer_list<uint32_t> fmtAndArgs) {
    Frame f;
    setTextC(text, stringBuffer(f, fmtAndArgs));
}

// a page's text layer from its cocostudio file, added to the widget under its name
uint32_t loadLayer(uint32_t widget, uint32_t json, uint32_t name) {
    const uint32_t layer = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    addChildNamed(widget, layer, 0, name, 9);
    return layer;
}

// ------------------------------------------------------------------------------------------------ deck (page 0)
// vector<shared_ptr<UserMedalData>>::push_back(const&)
void pushMedalData(uint32_t vec, uint32_t value) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(c::MedalDatas_pushBackSlow, {vec, value});
        return;
    }
    if (end) {
        st32(end, ld32(value));
        const uint32_t ctrl = ld32(value + 4);
        st32(end + 4, ctrl);
        addRef(ctrl);
    }
    st32(vec + 4, ld32(vec + 4) + 8);
}
// vector<UserMedal::Medal>::push_back(const&)
void pushMedal(uint32_t vec, uint32_t medal) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(c::Medals_pushBackSlow, {vec, medal});
        return;
    }
    uint32_t at = 0;
    if (end) {
        call(c::MEMCPY, {end, medal, MEDAL_SIZE});
        at = end;
    }
    st32(vec + 4, at + MEDAL_SIZE);
}

// KeyBladeDataManager::getKeyBladeData(0, id, UserKeyblade::Keyblade{}) into out: the struct is zeroed in buf
// (its padding left as it was) and copied to the stack arguments
void keybladeData(uint32_t sp, uint32_t buf, uint32_t out, uint32_t id) {
    const uint32_t manager = call(c::KeyBladeDataManager_getInstance, {});
    st32(buf + 0x50, 0);
    call(c::MEMSET, {buf, 0x14, 0});
    call(c::MEMSET, {buf + 0x18, 0x35, 0});
    copyWords(sp + 8, buf, KEYBLADE_SIZE);
    rw::call_exact(c::KeyBladeDataManager_getKeyBladeData, {out, manager, 0, 0, id});
}
// UserMedal::Medal{} in buf (its padding left as it was)
void zeroMedal(uint32_t buf) {
    call(c::MEMSET, {buf, 0x2C, 0});
    call(c::MEMSET, {buf + 0x30, 0x18, 0});
}
// MedalDataManager::getMedalData(id, 0, medal) into out
void medalData(uint32_t sp, uint32_t manager, uint32_t out, uint32_t idLo, uint32_t idHi, uint32_t medal) {
    copyWords(sp + 8, medal, MEDAL_SIZE);
    rw::call_exact(c::MedalDataManager_getMedalData, {out, manager, idLo, idHi, 0});
}

// another player's deck: each medal slot's data, from the medal in the shown player's medal list with the slot's
// user medal id (an empty slot: the data for id 0 and the never written medal buffer)
void otherDeckMedals(const Page &p, uint32_t datas, uint32_t medals) {
    const uint32_t sp = p.sp, me = p.me;
    for (uint32_t i = 0; i < ld32(me + F_MEDAL_COUNT); i++) {
        const uint32_t id = me + F_MEDAL_IDS + 8 * i;
        if (!(ld32(id) | ld32(id + 4))) {
            const uint32_t manager = call(c::MedalDataManager_getInstance, {});
            const uint32_t lo = ld32(id), hi = ld32(id + 4);
            zeroMedal(sp + 0x17B0);
            medalData(sp, manager, sp + 0x560, lo, hi, sp + 0x17B0);
            pushMedalData(datas, sp + 0x560);
            pushMedal(medals, sp + 0x1768);
            release(ld32(sp + 0x564));
            continue;
        }
        const uint32_t medal = sp + 0x1720;
        uint32_t offset = 0;
        for (uint32_t n = 0; n < ld32(ld32(me + F_MEDALS) + 0x20); n++, offset += MEDAL_SIZE) {
            sbRead(ld32(me + F_MEDALS), medal, offset, MEDAL_SIZE);
            if (ld32(medal + 8) != ld32(id) || ld32(medal + 12) != ld32(id + 4)) continue;
            const uint32_t manager = call(c::MedalDataManager_getInstance, {});
            medalData(sp, manager, sp + 0x558, 0, 0, medal);
            pushMedalData(datas, sp + 0x558);
            pushMedal(medals, medal);
            release(ld32(sp + 0x55C));
            break;
        }
    }
}

// the Lux, weekly ranking, id, avatar, comment and Facebook share texts of page 0
void deckTexts(const Page &p) {
    const uint32_t sp = p.sp, me = p.me, layer = p.layer;
    const uint32_t atlas = findNode(layer, S_AtlasNumL, 15);
    const uint32_t lux = findNode(layer, S_AtlasLux, 14);
    call(c::Label_setAdditionalKerning,
         {vcall(lux, TEXT_ATLAS_GET_RENDERER, {lux}), rw::load<uint32_t>(ATLASNUM_KERNING_PIXEL)});
    const uint32_t weekly = findNode(layer, S_TxtWeekly, 10);
    const uint32_t baseWeekly = findNode(layer, S_BaseLuxWeekly, 15);
    const uint32_t weeklyLux = findNode(findNode(baseWeekly, S_BaseLux, 8), S_TxtWeekly, 10);
    const uint32_t total = findNode(layer, S_WeeklyTotal, 0x19);
    const uint32_t rank = findNode(layer, S_WeeklyRank, 0x18);
    sbRead(ld32(me + F_DETAIL), sp + 0x618, 8, 4);
    SceneUtil::numberToAtlasString(sp + 0x480, ld32(sp + 0x618));
    call(c::TextAtlas_setString, {atlas, sp + 0x480});
    strFree(sp + 0x480);
    sbRead(ld32(me + F_DETAIL), sp + 0x61C, 8, 4);
    const int32_t luxRank = int32_t(ld32(sp + 0x61C));
    sbRead(ld32(me + F_POINT), sp + 0x620, 8, 8);
    const uint32_t gauge = sp + 0x1668;
    st32(gauge + 16, gauge);
    st32(gauge, VT_LUX_GAUGE);
    st32(gauge + 8, lux);
    st32(gauge + 4, layer);
    SceneUtil::calcLuxGaugePer(luxRank, uint64_t(ld32(sp + 0x624)) << 32 | ld32(sp + 0x620), gauge);
    functionDestroy(gauge);
    setTextId(weekly, TXT_WEEKLY);
    setTextId(weeklyLux, TXT_WEEKLY_LUX);
    SceneUtil::getMoneyStr(sp + 0x44C, uint64_t(ld32(me + F_WEEKLY_LUX + 4)) << 32 | ld32(me + F_WEEKLY_LUX));
    call(fn::Text_setString, {total, sp + 0x44C});
    strFree(sp + 0x44C);
    setTextFormat(rank, {S_FmtD, ld32(me + F_WEEKLY_RANK)});
    for (uint32_t t : {weekly, weeklyLux, total, rank}) SceneUtil::setShadow(t);
    const uint32_t place = findNode(findNode(layer, S_BaseLuxWeekly, 15), S_TxtPlace, 9);
    SceneUtil::numberToPlaceString(sp + 0x410, ld32(me + F_WEEKLY_RANK));
    setTextFromString(place, sp + 0x410);
    SceneUtil::setShadow(place);

    const uint32_t idText = findNode(layer, S_TxtId, 6);
    const uint32_t idLabel = findNode(layer, S_TxtIdLabel, 12);
    setTextId(idText, TXT_ID);
    sbRead(ld32(me + F_USER), sp + 0x628, 0, 8);
    setTextFormat(idLabel, {S_FmtLlu, ld32(sp + 0x628), ld32(sp + 0x62C)});
    SceneUtil::setShadow(idText);
    SceneUtil::setShadow(idLabel);

    // the avatar, in place of the layout's (hidden) Avatar node
    const uint32_t area = findNode(layer, S_Avatar, 6);
    vcall(area, node::setVisible, {area, 0});
    const uint32_t style = sp + 0x3AC;
    call(c::AvatarUtil_makeStyle, {style, p.cap + CAP_RECORD, ld32(me + F_KEYBLADE_ID)});
    if (const uint32_t old = findNode(p.widget, S_AvatarNode, 10)) vcall(p.widget, node::removeChild, {p.widget, old, 1});
    const uint32_t avatar = call(c::Avatar_create, {style, 0});
    call(c::Avatar_changeMotion, {avatar, 0x1B});
    vcall(avatar, NODE_SET_SCALE_XY, {avatar, fbits(2.5f), fbits(2.5f)});
    vcall(avatar, node::setPosition, {avatar, vcall(area, node::getPosition, {area})});
    addChildNamed(p.widget, avatar, vcall(area, NODE_GET_LOCAL_Z_ORDER, {area}), S_AvatarNode, 10);
    call(c::Node_unscheduleUpdate, {ld32(avatar + 0x29C)});

    const uint32_t comment = findNode(layer, S_CommentLabel, 0x11);
    call(c::Text_setTextHorizontalAlignment, {comment, 3});
    setTextC(comment, strData(me + F_COMMENT));
    const uint32_t share = findNode(layer, S_FbShare, 15);
    titleLabel(share, S_TxtFbShare, 12, TXT_FB_SHARE);
    uint32_t visible = 0;
    if (SceneUtil::isLoggedInFacebook()) visible = ld8(me + F_IS_OTHER_USER) ? 0 : 1;
    vcall(share, node::setVisible, {share, visible});
    if (visible) addTouch(share, VT_FB_SHARE, {me});
    call(c::Style_dtor, {style});
}

// page 0: the deck's status, its keyblade and medals on the revolver (created once, kept by the dialog), then the
// texts
void deckPage(const Page &p) {
    const uint32_t sp = p.sp, me = p.me;
    const uint32_t medals = sp + 0x570, datas = sp + 0x57C;   // vector<UserMedal::Medal>, vector<shared_ptr<UserMedalData>>
    for (uint32_t k = 0; k < 0x18; k += 4) st32(medals + k, 0);
    const uint32_t status = guest::newArray(0xC);
    uint32_t kbObj, kbCtrl;
    if (ld8(me + F_IS_OTHER_USER)) {
        keybladeData(sp, sp + 0x17F8, sp + 0x568, ld32(me + F_KEYBLADE_ID));
        kbObj = ld32(sp + 0x568);
        kbCtrl = ld32(sp + 0x56C);
        otherDeckMedals(p, datas, medals);
        copyShared(sp + 0x550, kbObj, kbCtrl);
        call(c::SceneUtil_calcDeckStatus, {sp + 0x550, datas, status, medals});
        release(ld32(sp + 0x554));
    } else {
        keybladeData(sp, sp + 0x16C8, sp + 0x548, ld32(me + F_KEYBLADE_ID));
        kbObj = ld32(sp + 0x548);
        kbCtrl = ld32(sp + 0x54C);
        for (uint32_t i = 0; i < ld32(me + F_MEDAL_COUNT); i++) {
            const uint32_t manager = call(c::MedalDataManager_getInstance, {});
            const uint32_t id = me + F_MEDAL_IDS + 8 * i;
            const uint32_t lo = ld32(id), hi = ld32(id + 4);
            zeroMedal(sp + 0x1680);
            medalData(sp, manager, sp + 0x540, lo, hi, sp + 0x1680);
            pushMedalData(datas, sp + 0x540);
            release(ld32(sp + 0x544));
        }
        copyShared(sp + 0x538, kbObj, kbCtrl);
        call(c::SceneUtil_calcDeckStatus, {sp + 0x538, datas, status, 0});
        release(ld32(sp + 0x53C));
    }
    call(c::SceneUtil_setMyPageDeckStatus, {p.layer, me + F_POINT, ld32(status), ld32(status + 4)});
    guest::deleteArray(status);

    const bool other = ld8(me + F_IS_OTHER_USER);
    uint32_t revolver = findNode(p.widget, S_RevolverView, 12);
    if (revolver) {
        if (other) st32(revolver + RV_SKILLS, me + F_SKILLS);
        copyShared(sp + 0x524, kbObj, kbCtrl);
        call(c::DeckRevolverView_setKeyBladeData, {revolver, sp + 0x524});
        release(ld32(sp + 0x528));
        call(c::DeckRevolverView_setMedalDatas, {revolver, datas});
    } else {
        const uint32_t area = findNode(p.layer, S_AreaRevolver, 13);
        const uint32_t slot = me + F_REVOLVER;
        if (const uint32_t kept = ld32(slot)) {
            vcall(kept, node::removeFromParent, {kept});
            copyShared(sp + 0x510, kbObj, kbCtrl);
            call(c::DeckRevolverView_setKeyBladeData, {ld32(slot), sp + 0x510});
            release(ld32(sp + 0x514));
        } else {
            copyShared(sp + 0x508, kbObj, kbCtrl);
            copyShared(sp + 0x500, ld32(me + F_DETAIL), ld32(me + F_DETAIL + 4));
            st32(slot, call(c::DeckRevolverView_create, {sp + 0x508, sp + 0x500}));
            release(ld32(sp + 0x504));
            release(ld32(sp + 0x50C));
            call(c::Ref_retain, {ld32(slot)});
        }
        revolver = ld32(slot);
        if (ld8(me + F_IS_OTHER_USER)) st32(revolver + RV_SKILLS, me + F_SKILLS);
        call(c::DeckRevolverView_setMedalDatas, {revolver, datas});
        call(c::DeckRevolverView_setOpenMedalInfoPopupEnabled, {revolver, 1});
        const uint32_t info = sp + 0x1880;
        st32(info + 16, info);
        st32(info, VT_MEDAL_INFO);
        call(c::MedalInfoFunction_swap, {info, revolver + RV_MEDAL_INFO});
        functionDestroy(info);
        const float w = ldf(vcall(revolver, node::getContentSize, {revolver}));
        const float h = ldf(vcall(revolver, node::getContentSize, {revolver}) + 4);
        call(c::Vec2_ctor, {sp + 0x4F8, fbits(w * 0.5f), fbits(h * 0.5f)});
        vcall(revolver, node::setPosition, {revolver, sp + 0x4F8});
        call(c::Vec2_dtor, {sp + 0x4F8});
        addChildNamed(area, revolver, 0, S_RevolverView, 12);
    }
    st8(revolver + RV_IS_OWN, ld8(me + F_IS_OTHER_USER) ^ 1);
    deckTexts(p);
    call(c::Medals_dtor, {medals});
    call(c::MedalDatas_dtor, {datas});
    release(kbCtrl);
}

// ------------------------------------------------------------------------------------------------ records (page 1)
void recordsPage(const Page &p) {
    const uint32_t sp = p.sp, me = p.me, layer = p.layer;
    if (const uint32_t old = findNode(p.widget, S_AvatarNode, 10)) vcall(p.widget, node::removeChild, {p.widget, old, 1});
    // the records view (created once, kept by the dialog)
    const uint32_t scroll = findNode(layer, S_ScrollArea, 11);
    if (!findNode(scroll, S_RecordScrollView, 16)) {
        const uint32_t slot = me + F_RECORD_VIEW;
        if (const uint32_t kept = ld32(slot)) {
            vcall(kept, node::removeFromParent, {kept});
            addChildNamed(scroll, ld32(slot), 0, S_RecordScrollView, 16);
            call(c::MyRecordScrollView_resetScroll, {ld32(slot)});
        } else {
            st32(slot, call(c::MyRecordScrollView_create, {}));
            call(c::Ref_retain, {ld32(slot)});
            const uint32_t view = ld32(slot);
            vcall(view, node::setContentSize, {view, vcall(scroll, node::getContentSize, {scroll})});
            addChildNamed(scroll, ld32(slot), 0, S_RecordScrollView, 16);
        }
    }

    const uint32_t money = findNode(layer, S_TxtMoney, 9);
    setTextId(money, TXT_MONEY);
    SceneUtil::setShadow(money);
    const uint32_t moneyLabel = findNode(layer, S_TxtMoneyLabel, 15);
    sbRead(ld32(me + F_POINT), sp + 0x1880, 0, 4);
    SceneUtil::getMoneyStr(sp + 0x2E4, uint64_t(int64_t(int32_t(ld32(sp + 0x1880)))));
    setTextFromString(moneyLabel, sp + 0x2E4);
    SceneUtil::setShadow(moneyLabel);
    const uint32_t mogu = findNode(layer, S_TxtMoguPoint, 13);
    setTextId(mogu, TXT_MOGU);
    SceneUtil::setShadow(mogu);
    const uint32_t moguLabel = findNode(layer, S_TxtMoguPointLabel, 0x13);
    sbRead(ld32(me + F_POINT), sp + 0x1880, 0x18, 4);
    setTextFormat(moguLabel, {S_FmtD, ld32(sp + 0x1880)});
    SceneUtil::setShadow(moguLabel);
    const uint32_t lastLogin = findNode(layer, S_TxtLastLogin, 13);
    setTextId(lastLogin, TXT_LAST_LOGIN);
    SceneUtil::setShadow(lastLogin);
    // the shown player's profile (a copy of the API cache's shared_ptr): the last login
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t profileCtrl = ld32(api + 0x298), profile = ld32(api + 0x294);
    addRef(profileCtrl);
    const uint32_t lastLoginLabel = findNode(layer, S_TxtLastLoginLabel, 0x13);
    SceneUtil::setShadow(lastLoginLabel);
    const uint32_t since = call(c::Date_calculateIntervalSinceNow, {profile + PROFILE_LAST_LOGIN});
    call(c::createTimeAgoString, {sp + 0x268, 0u - since});
    setTextFromString(lastLoginLabel, sp + 0x268);

    // the party's time zones, one label each from the first on (zone 1 always goes to the first label)
    const uint32_t zone = findNode(layer, S_TxtTimezone, 12);
    setTextId(zone, TXT_TIMEZONE);
    SceneUtil::setShadow(zone);
    const uint32_t labels = sp + 0x1F4;   // the three label Texts, captured by the label lambda
    const uint32_t zone1 = findNode(layer, S_TxtTimezone1, 0x14);
    const uint32_t zone2 = findNode(layer, S_TxtTimezone2, 0x14);
    const uint32_t zone3 = findNode(layer, S_TxtTimezone3, 0x14);
    for (uint32_t t : {zone1, zone2, zone3}) setTextC(t, aid::S_Empty);
    st32(labels, zone1);
    st32(labels + 4, zone2);
    st32(labels + 8, zone3);
    const uint32_t keys[] = {sp + 0x1F0, sp + 0x1D8, sp + 0x1C0, sp + 0x1A8, sp + 0x190, sp + 0x178};
    uint32_t shown = 0;
    for (uint32_t z = 1; z <= 6; z++) {
        st32(keys[z - 1], z);
        const uint32_t end = me + F_TIMEZONES + 4;
        const uint32_t n = call(c::Timezones_lowerBound, {ld32(me + F_TIMEZONES), end, keys[z - 1], sp + 0x1880});
        if (n == end || int32_t(ld32(n + 0x10)) > int32_t(z)) continue;
        Frame f;
        const uint32_t td = textData(f, TXT_ZONE_1 + int32_t(z) - 1);
        const uint32_t s = newStr(f, textDataStr(td));
        if (z == 1) setTextC(zone1, strData(s));
        else call(c::TimezoneText, {labels, s, shown});
        strFree(s);
        sharedRelease(td);
        shown++;
    }
    for (uint32_t t : {zone1, zone2, zone3}) SceneUtil::setShadow(t);

    const uint32_t style = findNode(layer, S_TxtStyle, 9);
    setTextId(style, TXT_STYLE);
    SceneUtil::setShadow(style);
    const uint32_t styleLabel = findNode(layer, S_TxtStyleLabel, 15);
    const uint32_t role = ld32(me + F_PARTY_ROLE);
    if (role == 0) setTextFormat(styleLabel, {aid::S_Empty});
    else if (role <= 3) setTextId(styleLabel, TXT_STYLE_1 + int32_t(role) - 1);
    SceneUtil::setShadow(styleLabel);
    release(profileCtrl);
}


// ------------------------------------------------------------------------------------------------ the page lambdas
constexpr uint32_t D_PENDING_POPUP = 0x458;     // AvatarInfoDialog: std::function<void(bool)> of an open sub-popup
constexpr uint32_t VSLOT_SLIDE_TOUCH = 0x31C;   // BaseSlideView: (bool)
constexpr uint32_t VT_SHARE_DONE = 0xF584EC;    // shareScreenShot's result {dialog}
constexpr uint32_t VT_SHARE_POSTED = 0xF58518;  // openFBSharePhotoPopup's callback (no captures)
constexpr uint32_t VT_PARTY_INFO_DONE = 0xF585C8;   // openPartyInfoRequestPopup's result (heap 0x24)
constexpr uint32_t VT_JOIN_PAYMENT_DONE = 0xF585F4, VT_JOIN_PAYMENT_ERROR = 0xF58620;
constexpr uint32_t S_TipLux = 0xDFE849;         // "Tip_LUX"

// Close_Button {dialog, view}(sender, type): ended -> the pending sub-popup callback dropped, the buttons under
// the page locked, the slide view's touch on, the current popup closed ($_8 Close_Button lambda)
void closeTouch(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t dialog = ld32(self + 4);
    functionDestroy(dialog + D_PENDING_POPUP);
    st32(dialog + D_PENDING_POPUP + 16, 0);
    call(c::AudioPlaySystemSECancel, {});
    AvatarInfoDialog::setTouchEnabledUnderUIButtons(rw::host_ptr<AvatarInfoDialog>(dialog), 0);
    const uint32_t view = ld32(self + 8);
    vcall(view, VSLOT_SLIDE_TOUCH, {view, 1});
    call(c::SceneManager_closeCurrentPopup, {sceneManager(), 0});
}

// Button_FB_Share {dialog}(sender, type): ended -> a screenshot, then the Facebook photo popup ($_8 lambda #3)
void shareTouch(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t dialog = ld32(self + 4);
    call(c::AudioPlaySystemSEDecide, {});
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t done = newFunction(f, VT_SHARE_DONE, {dialog});
    call(c::SceneUtil_shareScreenShot, {sm, done});
    functionDestroy(done);
}
// the screenshot {dialog}(RenderTexture*, file name): the Facebook photo popup over the dialog's edit state
void shareDone(uint32_t self, uint32_t texturep, uint32_t name) {
    const uint32_t texture = ld32(texturep);
    const uint32_t dialog = ld32(self + 4);
    Frame f;
    const uint32_t posted = newFunction(f, VT_SHARE_POSTED, {});
    call(c::SceneUtil_openFBSharePhotoPopup, {dialog + 0x4C0, dialog + 0x4B8, dialog + 0x4BC, dialog + 0x450,
                                              dialog + 0x270, name, texture, posted});
    functionDestroy(posted);
}

// calcLuxGaugePer's callback {layer, Lux atlas}(rate, Lux): the gauge (Tip_LUX) rate * 100 wide, the atlas the Lux
void luxGauge(uint32_t self, uint32_t ratep, uint32_t luxp) {
    const uint32_t lux = ld32(luxp);
    const float rate = ldf(ratep);
    const uint32_t layer = ld32(self + 4);
    Frame f;
    const uint32_t name = f.alloc(12);       // the short string "Tip_LUX", built inline
    st8(name, 7 << 1);
    call(c::MEMCPY, {name + 1, S_TipLux, 7});
    st8(name + 8, 0);
    const uint32_t tip = SceneUtil::searchNodeByName(layer, name);
    strFree(name);
    const float h = ldf(vcall(tip, node::getContentSize, {tip}) + 4);
    const uint32_t size = f.alloc(8);
    call(c::Size_ctor, {size, fbits(rate * 100.0f), fbits(h)});
    vcall(tip, node::setContentSize, {tip, size});
    const uint32_t atlas = ld32(self + 8);
    const uint32_t s = f.alloc(12);
    SceneUtil::numberToAtlasString(s, lux);
    call(c::TextAtlas_setString, {atlas, s});
    strFree(s);
}

// ImageButton_party {dialog, dialog, buttons, dialog, view}(sender, type): ended -> the shown player's party info
// popup (its id from the profile), the answer to a heap lambda over the same captures
void partyTouch(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t dialog = ld32(self + 4);
    call(c::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t party = f.alloc(8);
    sbRead(ld32(dialog + F_DETAIL), party, 0x28, 8);
    uint32_t w[8];
    for (uint32_t k = 0; k < 8; k++) w[k] = ld32(self + 8 + 4 * k);
    const uint32_t fo = f.alloc(20);
    heapLambda(fo, 0x24, VT_PARTY_INFO_DONE, {w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7]});
    call(c::SceneUtil_openPartyInfoRequestPopup, {ld32(party), ld32(party + 4), 1, fo});
    functionDestroy(fo);
}

// the party info popup's join request {dialog, $_73's captures}(-): the "closed" callback parked in the dialog;
// the join request popup now, or first the payment info
void partyInfoDone(uint32_t self, uint32_t) {
    const uint32_t dialog = ld32(self + 4);
    call(c::OtherClosedAssign, {dialog + aid::D_ON_POPUP_CLOSED, self + 8});
    const uint32_t api = call(aid::APIManager_getInstance, {});
    const uint32_t count = ld32(api + aid::API_PAYMENT + 4), payment = ld32(api + aid::API_PAYMENT);
    addRef(count);
    if (payment) {
        Frame f;
        const uint32_t cb = f.alloc(20);
        call(c::FunctionVB_copy, {cb, dialog + aid::D_ON_POPUP_CLOSED});
        call(c::openPopUpJoinRequest, {dialog, cb});
        functionDestroy(cb);
    } else {
        aid::requestPaymentInfo(dialog, VT_JOIN_PAYMENT_DONE);
    }
    release(count);
}

// its payment info result {dialog}(ok): the loading indicator off; the join request popup, or the error dialog
void joinPaymentDone(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    const uint32_t dialog = ld32(self + 4);
    call(aid::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        aid::apiErrorDialog(VT_JOIN_PAYMENT_ERROR);
        return;
    }
    Frame f;
    const uint32_t cb = f.alloc(20);
    call(c::FunctionVB_copy, {cb, dialog + aid::D_ON_POPUP_CLOSED});
    call(c::openPopUpJoinRequest, {dialog, cb});
    functionDestroy(cb);
}

// ------------------------------------------------------------------------------------------------ the updater
uint32_t pageUpdater(uint32_t cap, uint32_t widget, uint32_t page) {
    Frame f;
    Page p{f.allocExact(FRAME), cap, ld32(cap + CAP_DIALOG), widget, 0};
    if (!p.widget) {
        p.widget = call(c::FunctionWidgetInt_call, {ld32(cap + CAP_VIEW) + VIEW_PAGE_FACTORY, page});
        call(c::Ref_retain, {p.widget});
    }
    const uint32_t layer1 = findNode(p.widget, S_TxtLayer1, 9);
    const uint32_t layer2 = findNode(p.widget, S_TxtLayer2, 9);
    uint32_t first = 0;
    if (page == 0) {
        p.layer = layer1;
        if (!p.layer) {
            p.layer = loadLayer(p.widget, S_Txt1Json, S_TxtLayer1);
            if (const uint32_t loaded = findNode(p.widget, S_TxtLayer1, 9))
                SceneUtil::setAdditionalKerningForAtlasLabel(findNode(loaded, S_AtlasNumL, 15), -21);
        }
        if (layer2) vcall(layer2, node::removeFromParent, {layer2});
        first = 1;
    } else if (page == 1) {
        p.layer = layer2 ? layer2 : loadLayer(p.widget, S_Txt2Json, S_TxtLayer2);
        if (layer1) vcall(layer1, node::removeFromParent, {layer1});
    } else {
        p.layer = 0xFFE25211u;   // only pages 0 and 1 exist: the original leaves a literal's offset here
    }
    addTouch(findNode(p.layer, S_CloseButton, 12), VT_CLOSE, {p.me, ld32(cap + CAP_VIEW2)});

    // the team status, and for another player in a party the party button
    const uint32_t user = findNode(p.widget, S_User, 4);
    rw::call_exact(c::SceneUtil_setMyPageTeamStatus, {user, p.me + F_USER, p.me + F_DETAIL, p.me + F_PARTY,
                                                       ld32(p.me + F_IMAGE_DOWNLOADER), first});
    if (ld8(p.me + F_IS_OTHER_USER)) {
        sbRead(ld32(p.me + F_DETAIL), p.sp + 0x1880, 0x28, 8);
        if (ld32(p.sp + 0x1880) | ld32(p.sp + 0x1884)) {
            uint32_t buttons[7];
            for (uint32_t k = 0; k < 7; k++) buttons[k] = ld32(cap + CAP_BUTTONS + 4 * k);
            const uint32_t area = findNode(user, S_PartyButtonArea, 0x11);
            uint32_t button = findNode(user, S_ImageButtonParty, 0x11);
            if (!button) {
                {
                    Frame t;
                    const uint32_t empty = newStr(t, aid::S_Empty, 0);
                    button = call(aid::ImageButton_create, {empty, 0});
                    strFree(empty);
                }
                call(aid::ImageButton_setTargetView, {button, area, fbits(1.0f)});
                addChildNamed(area, button, 0, S_ImageButtonParty, 0x11);
            }
            const uint32_t fo = p.sp + 0x1850;
            heapLambda(fo, 0x28, VT_PARTY, {p.me, p.me, buttons[0], buttons[1], buttons[2], buttons[3],
                                                 buttons[4], buttons[5], buttons[6]});
            call(aid::ImageButton_addTouchEventListener, {button, fo});
            functionDestroy(fo);
        }
    }
    if (page == 1) recordsPage(p);
    else if (page == 0) deckPage(p);
    return p.widget;
}

// $_8's time zone label: label `index` of the three captured Texts shows the string
void timezoneText(uint32_t labels, uint32_t str, uint32_t index) {
    if (index > 2) return;
    setTextC(ld32(labels + 4 * index), strData(str));
}

}  // namespace

RW_FUNCTION(0x547310, "AvatarInfoDialog::createLayout::$_8 page updater (sub_547310)", pageUpdater);
RW_FUNCTION(0x54A0E4, "AvatarInfoDialog::createLayout::$_8 time zone label (sub_54A0E4)", timezoneText);
RW_FUNCTION(0x54B036, "AvatarInfoDialog::createLayout::$_8 Close_Button lambda (sub_54B036)", closeTouch);
RW_FUNCTION(0x54A294, "AvatarInfoDialog::createLayout::$_8 Button_FB_Share lambda (sub_54A294)", shareTouch);
RW_FUNCTION(0x54A3D0, "AvatarInfoDialog::createLayout::$_8 screenshot lambda (sub_54A3D0)", shareDone);
RW_FUNCTION(0x54A5B8, "AvatarInfoDialog::createLayout::$_8 Lux gauge lambda(float, u64) (sub_54A5B8)", luxGauge);
RW_FUNCTION(0x54A820, "AvatarInfoDialog::createLayout::$_8 ImageButton_party lambda (sub_54A820)", partyTouch);
RW_FUNCTION(0x54A9E0, "AvatarInfoDialog::createLayout::$_8 party info lambda(bool) (sub_54A9E0)", partyInfoDone);
RW_FUNCTION(0x54AC78, "AvatarInfoDialog::createLayout::$_8 lambda(bool)::lambda(bool) (payment info, sub_54AC78)", joinPaymentDone);
