// SceneManager: the per-frame update (API status, scene change state machine), the header contents (level, Lux
// gauge, jewels, AP with its recovery timers, avatar icon, ticker) and the touch effects.
//
// The functions reproduce the original frame (Frame::allocExact with the original push + sub size) and call with
// the original sp (rw::call_exact), so callee frames and stack temporaries land where the original put them.
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
// APIManager
constexpr uint32_t API_STATUS = 0x00;               // int: 0 ok, 2..5 errors that send back to the title
constexpr uint32_t API_BUSY = 0x04;                 // u8: requests in flight
constexpr uint32_t API_ERROR = 0x14;                // u8: an error to show
constexpr uint32_t API_FACEBOOK = 0x44;             // user (link flags at +0x35, bit 1 Facebook)
constexpr uint32_t API_REMOTE_NOTIFICATION = 0x74;  // RemoteNotification*
constexpr uint32_t API_USER_STATUS = 0x8C;          // SafeBuffer: Lux points (u64) +8, AP +0x3C
constexpr uint32_t API_USER_DATA = 0x94;            // shared_ptr<UserData>: Lux rank +8, avatar id +0x3C
constexpr uint32_t API_USER_POINT = 0xA4;           // shared_ptr<UserPoint>: paid +0, free +4 jewels
constexpr uint32_t API_USER_AVATARS = 0xC4;         // shared_ptr (SafeBuffer of 48-byte coordinates, id +0)
constexpr uint32_t API_RAIDS = 0x154;               // vector begin / end
constexpr uint32_t SB_DATA = 0x0C, SB_COUNT = 0x20;
constexpr uint32_t AVATAR_SIZE = 48;

// SceneManager fields not in the layout yet
constexpr uint32_t SM_SCENE_STATE = 0x318;          // int: 0 waiting for APIs, 1 error dialog, 2 change scene,
                                                    // 3 fading out, 4 running, 5 fading in
constexpr uint32_t SM_TITLE_CURRENT = 0x3B0;        // TitleMain / Sub1 / Sub2 shown (0x3B0, 0x3B8, 0x3C0)
constexpr uint32_t SM_FIELD_3FC = 0x3FC;            // zeroed when a scene shows no header
constexpr uint32_t SM_TICKER_START = 0x344, SM_TICKER_LEFT = 0x348, SM_TICKER_END = 0x34C;  // floats
constexpr uint32_t SM_AP_SHOWN = 0x570, SM_AP_MAX_SHOWN = 0x574, SM_AP_SECONDS = 0x578;
constexpr uint32_t SM_PUSH_TOKEN = 0x57C;           // std::string: device token to register

constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t Director_setDefaultScheduler = 0x8EECC1;
constexpr uint32_t Sprite_createWithFile = 0x8A637D;
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t Node_getNumberOfRunningActions = 0x894B4F;
constexpr uint32_t RepeatForever_create = 0x86815D;
constexpr uint32_t FadeIn_create = 0x86B115, FadeOut_create = 0x86B25D;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t Label_setAdditionalKerning = 0x88697D;
constexpr uint32_t ATLASNUM_KERNING_PIXEL = 0xDEC3DC;  // SceneUtil::ATLASNUM_KERNING_PIXEL (float)
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41;
constexpr uint32_t SceneUtil_calcLuxGaugePer = 0x46EE0D;
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t AppState_getCurrentAP = 0x4EA5E9;     // (this, long* seconds to the next AP) -> AP
constexpr uint32_t AppState_getFullChargeApSec = 0x4EA729;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;
constexpr uint32_t AvatarIcon_create = 0x5C9DB5;
constexpr uint32_t ExpressionVector_allocate = 0x487635, ExpressionVector_dtor = 0x46BE0D;
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t String_assign = 0x498A1D;
constexpr uint32_t RemoteNotification_needsGetToken = 0x4E2611, RemoteNotification_getToken = 0x4E262D;
constexpr uint32_t APIManager_executeSystemPushRegist = 0x4D21B1;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_reallocate = 0x4C399D;
constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_isConnecting = 0x4E56B1;
constexpr uint32_t LoadingIndicator_hide = 0x509D69;
constexpr uint32_t LWFResourceCache_shared = 0x7EFF59, LWFResourceCache_unloadAll = 0x7F0ED1;
constexpr uint32_t AudioClearGameMainBGMStack = 0x5C19B5;
constexpr uint32_t AudioManager_getInstance = 0x5BECFD, AudioManager_unloadAll = 0x5BF139;
constexpr uint32_t RESTClient_getInstance = 0x497AE9, RESTClient_cancelAll = 0x498E8D;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_clear = 0x7ACC95;
constexpr uint32_t DeckMedalViewManager_getInstance = 0x527131, DeckMedalViewManager_update = 0x527209;
constexpr uint32_t SceneManager_moveToTitleForcibly = 0x60CF7D, SceneManager_moveToTitleIfPossible = 0x60D1B1;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_callPrevChangeNextSceneAPIs = 0x60E78D;
constexpr uint32_t SceneManager_createNextScene = 0x60E895;
constexpr uint32_t SceneManager_createHeader = 0x60EA65, SceneManager_initGlobalMenu = 0x6102D5;
constexpr uint32_t SceneManager_createTitleHeader = 0x610DA1;
constexpr uint32_t SceneManager_chatDataUpdate = 0x60E111;
constexpr uint32_t SceneManager_updateHeader = 0x611169, SceneManager_updateHeaderAp = 0x60D9CD;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;

constexpr uint32_t STR_Tap = 0xE0361F, STR_Tap1 = 0xE0360B, STR_TickerLabel = 0xE03047;
constexpr uint32_t STR_AvatarIcon = 0xE03290, STR_Avatar = 0xDF8F56;
constexpr uint32_t STR_AtlasLabel_NumL = 0xDFD2A4, STR_AtlasLabel_LUX = 0xDFE6E6, STR_AtlasLabel_MS = 0xE032BD;
constexpr uint32_t STR_Alerts = 0xE0311C;
constexpr uint32_t STR_Txt_Wording1 = 0xDFF14C, STR_Txt_Wording2 = 0xDF863B, STR_Txt_Time_Label = 0xDFF589;
constexpr uint32_t STR_02d02d = 0xE0329B, STR_02d02d02d = 0xE032A5;
constexpr uint32_t STR_AtlasLabel_AP = 0xDFD382, STR_AtlasLabel_AP_Max = 0xE02FDB, STR_Tip_AP = 0xE032B6;
constexpr uint32_t STR_l = 0xE032B4;
constexpr int32_t TEXT_AP_RECOVERY = 102600051;     // "%d:%02d" style text of the AP popup timer
constexpr int32_t TEXT_TITLE_ERROR_1 = 109010001, TEXT_TITLE_ERROR_2 = 109010002;
constexpr int32_t TEXT_TITLE_ERROR_3 = 109030001, TEXT_TITLE_ERROR_4 = 109030003;

// lambdas (vtable of the __func + 8)
constexpr uint32_t VT_TouchEffect_Remove = 0xF60450;     // generateTouchEffect $_24 [sprite]
constexpr uint32_t VT_TouchMoveEffect_Remove = 0xF60424; // generateTouchMoveEffect $_25 [sprite]
constexpr uint32_t VT_Header_LuxGauge = 0xF602F0;        // updateHeader $_30 [this, luxLabel] (float, u64)
constexpr uint32_t VT_Update_Token = 0xF60818;           // update $_7 [this] (const char* token)
constexpr uint32_t VT_Update_TitleError = 0xF607EC;      // update $_8

uint32_t cx(uint32_t fn, std::initializer_list<uint32_t> args) { return uint32_t(rw::call_exact(fn, args)); }
uint32_t vx(uint32_t obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return cx(ld32(ld32(obj) + offset), args);
}
uint32_t api() { return cx(fn::APIManager_getInstance, {}); }
void addRef(uint32_t ctl) { if (ctl) cx(fn::shared_add, {ctl}); }
void release(uint32_t ctl) { if (ctl) cx(fn::shared_release, {ctl}); }
void destroyFn(uint32_t f) {
    uint32_t p = ld32(f + 16);
    if (p == f) cx(ld32(ld32(p) + 0x10), {p});
    else if (p) cx(ld32(ld32(p) + 0x14), {p});
}
void freeStr(uint32_t s) {
    if (ld8(s) & 1) cx(fn::operator_delete, {ld32(s + 8)});
}
// SceneUtil::searchNodeByName(root, std::string(text)) with the name built at `tmp`
uint32_t search(uint32_t root, uint32_t tmp, uint32_t text, uint32_t len) {
    cx(fn::string_init, {tmp, text, len});
    uint32_t node = cx(SceneUtil_searchNodeByName, {root, tmp});
    freeStr(tmp);
    return node;
}
uint32_t readSafe(uint32_t sb, uint32_t dst, uint32_t index, uint32_t n) {
    cx(SafeBuffer_read, {sb, dst, ld32(sb + SB_DATA) + index, n, index});
    return ld32(dst);
}
// text->setString(std::string(cstr)) through a temporary at `tmp`
void setTextFromBuffer(uint32_t text, uint32_t tmp, uint32_t cstr) {
    cx(fn::string_init, {tmp, cstr, host_strlen(cstr)});
    cx(fn::Text_setString, {text, tmp});
    freeStr(tmp);
}
// atlas->setString(SceneUtil::numberToAtlasString(n)) through a temporary at `tmp`
void setAtlasNumber(uint32_t atlas, uint32_t tmp, int32_t n) {
    cx(SceneUtil_numberToAtlasString, {tmp, uint32_t(n)});
    cx(TextAtlas_setString, {atlas, tmp});
    freeStr(tmp);
}
uint32_t setDefaultScheduler(uint32_t value) {
    return cx(Director_setDefaultScheduler, {cx(fn::Director_getInstance, {}), value});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ touch effects

// A tap ring at the touch (converted to this layer's parent space), scaled in then faded out and removed.
void SceneManager::generateTouchEffect(SceneManager *self, Addr position) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x60);
    const uint32_t m = rw::guest_addr(self);
    uint32_t scheduler = setDefaultScheduler(0x7FFFFFFF);
    const uint32_t name = base + 0x1C;
    cx(fn::string_init, {name, STR_Tap, 18});
    uint32_t sprite = cx(Sprite_createWithFile, {name});
    freeStr(name);
    float x = ldf(position), y = ldf(position + 4);
    uint32_t parent = vx(m, node::getParent, {m});
    float parentY = bitsf(vx(parent, node::getPositionY, {parent}));
    const uint32_t pos = base + 0x14;
    stf(pos, x);
    stf(pos + 4, y - parentY);
    vx(sprite, node::setPosition, {sprite, pos});
    vx(sprite, 0x48, {sprite, fbits(0.8f), fbits(0.8f)});           // setScale(x, y)
    uint32_t layer = self->touchEffectLayer;
    vx(layer, node::addChild, {layer, sprite});
    const uint32_t remove = base + 0x28;
    st32(remove + 16, remove);
    st32(remove, VT_TouchEffect_Remove);
    st32(remove + 4, sprite);
    uint32_t done = cx(fn::CallFunc_create, {remove});
    destroyFn(remove);
    uint32_t grow = cx(fn::ScaleTo_create, {fbits(0.08f), fbits(1.0f)});
    uint32_t widen = cx(fn::ScaleTo_create, {fbits(0.3f), fbits(1.5f)});
    uint32_t fade = cx(fn::FadeTo_create, {fbits(0.3f), 0});
    uint32_t out = cx(fn::Spawn_create, {widen, fade, 0});
    cx(fn::Node_runAction, {sprite, cx(fn::Sequence_create, {grow, out, done, 0})});
    setDefaultScheduler(scheduler);
}
RW_FUNCTION(0x61257C, "SceneManager::generateTouchEffect", SceneManager::generateTouchEffect);

// A smaller trail sprite while the touch moves: shrinks and fades out, then is removed.
void SceneManager::generateTouchMoveEffect(SceneManager *self, Addr position) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x58);
    uint32_t scheduler = setDefaultScheduler(0x7FFFFFFF);
    const uint32_t name = base + 0x14;
    cx(fn::string_init, {name, STR_Tap1, 19});
    uint32_t sprite = cx(Sprite_createWithFile, {name});
    freeStr(name);
    vx(sprite, node::setPosition, {sprite, position});
    vx(sprite, 0x48, {sprite, fbits(1.2f), fbits(1.2f)});
    uint32_t layer = self->touchEffectLayer;
    vx(layer, node::addChild, {layer, sprite});
    const uint32_t remove = base + 0x20;
    st32(remove + 16, remove);
    st32(remove, VT_TouchMoveEffect_Remove);
    st32(remove + 4, sprite);
    uint32_t done = cx(fn::CallFunc_create, {remove});
    destroyFn(remove);
    uint32_t shrink = cx(fn::ScaleTo_create, {fbits(0.3f), fbits(0.0f)});
    uint32_t fade = cx(fn::FadeTo_create, {fbits(0.3f), 0});
    uint32_t out = cx(fn::Spawn_create, {shrink, fade, 0});
    cx(fn::Node_runAction, {sprite, cx(fn::Sequence_create, {out, done, 0})});
    setDefaultScheduler(scheduler);
}
RW_FUNCTION(0x612764, "SceneManager::generateTouchMoveEffect", SceneManager::generateTouchMoveEffect);

// ------------------------------------------------------------------------------------------------ header

// Set the header ticker text; a text wider than the ticker area scrolls forever (from the right edge to the end
// of the text, then jumps back). Without a header the text is kept for the next one.
void SceneManager::changeTickerMessage(SceneManager *self, Addr message) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x60);
    const uint32_t m = rw::guest_addr(self);
    uint32_t header = self->header;
    if (!header) {
        cx(String_assign, {m + 0x350, message});
        return;
    }
    uint32_t label = search(header, base + 0x3C, STR_TickerLabel, 12);
    cx(CustomTextView_setXml, {label, message});
    stf(m + SM_TICKER_END, ldf(m + SM_TICKER_LEFT) - ldf(label + 0x2BC));
    cx(Node_stopAllActions, {label});
    float start = ldf(m + SM_TICKER_START), left = ldf(m + SM_TICKER_LEFT);
    float width = ldf(label + 0x2BC);
    float duration = (width / 1136.0f) * 17.0f;
    if (!((start - left) < width)) {                 // fits (or NaN): no scrolling
        vx(label, node::setPositionX, {label, fbits(left)});
        return;
    }
    vx(label, node::setPositionX, {label, fbits(start)});
    float y = bitsf(vx(label, node::getPositionY, {label}));
    const uint32_t p1 = base + 0x34, p2 = base + 0x2C, p3 = base + 0x24;
    stf(p1, ldf(m + SM_TICKER_LEFT));
    stf(p1 + 4, y);
    uint32_t enter = cx(fn::MoveTo_create, {fbits(1.0f), p1});
    uint32_t wait = cx(fn::DelayTime_create, {fbits(1.0f)});
    float endX = ldf(m + SM_TICKER_END);
    y = bitsf(vx(label, node::getPositionY, {label}));
    stf(p2, endX);
    stf(p2 + 4, y);
    uint32_t scroll = cx(fn::MoveTo_create, {fbits(duration), p2});
    float startX = ldf(m + SM_TICKER_START);
    y = bitsf(vx(label, node::getPositionY, {label}));
    stf(p3, startX);
    stf(p3 + 4, y);
    uint32_t back = cx(fn::MoveTo_create, {0, p3});
    uint32_t seq = cx(fn::Sequence_create, {enter, wait, scroll, back, 0});
    cx(fn::Node_runAction, {label, cx(RepeatForever_create, {seq})});
}
RW_FUNCTION(0x610B38, "SceneManager::changeTickerMessage", SceneManager::changeTickerMessage);

// Replace the "AvatarIcon" of `node` (default: the header) with an icon of avatar `avatarId` (-1: the user's
// current avatar; an unknown id shows the first coordinate), placed over the "Avatar" image.
void SceneManager::updateHeaderAvatarIcon(SceneManager *self, Addr node, int32_t avatarId) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x108);
    uint32_t root = node ? node : uint32_t(self->header);
    if (!root) return;
    uint32_t oldIcon = search(root, base + 0x7C, STR_AvatarIcon, 10);
    if (oldIcon) vx(oldIcon, node::removeFromParent, {oldIcon});
    uint32_t avatar = search(root, base + 0x70, STR_Avatar, 6);
    uint32_t a = api();
    uint32_t dataCtl = ld32(a + API_USER_DATA + 4), data = ld32(a + API_USER_DATA);
    addRef(dataCtl);
    uint32_t current = readSafe(data, base + 0x88, 0x3C, 4);
    if (avatarId == -1) avatarId = int32_t(current);
    uint32_t b = api();
    uint32_t avatarsCtl = ld32(b + API_USER_AVATARS + 4), avatars = ld32(b + API_USER_AVATARS);
    addRef(avatarsCtl);
    uint32_t index = 0;
    if (ld32(avatars + SB_COUNT)) {
        for (uint32_t i = 0, off = 0;; off += AVATAR_SIZE) {
            uint32_t id = readSafe(avatars, base + 0xC0, off, AVATAR_SIZE);
            if (id == uint32_t(avatarId)) index = i;
            i++;
            if (id == uint32_t(avatarId) || i >= ld32(avatars + SB_COUNT)) break;
        }
    }
    const uint32_t coordinate = base + 0x90, style = base + 0x50, offset = base + 0x48;
    const uint32_t expression = base + 0x8C, expressions = base + 0x3C;
    readSafe(avatars, coordinate, AVATAR_SIZE * index, AVATAR_SIZE);
    cx(AvatarUtil_makeStyle, {style, coordinate, 0});
    uint32_t renderer = vx(avatar, 0x2D8, {avatar});                 // ImageView::getVirtualRenderer
    uint32_t texture = vx(renderer, 0x280, {renderer});              // Sprite::getTexture
    stf(offset, -115.0f);
    stf(offset + 4, 125.0f);
    // std::vector<Avatar::Expression>{1}
    st32(expression, 1);
    st32(expressions, 0);
    st32(expressions + 4, 0);
    st32(expressions + 8, 0);
    cx(ExpressionVector_allocate, {expressions, 1});
    uint32_t end = ld32(expressions + 4);
    uint32_t next = 0;
    if (end) {
        st32(end, ld32(expression));
        next = ld32(expressions + 4);
    }
    st32(expressions + 4, next + 4);
    uint32_t icon = cx(AvatarIcon_create, {style, texture, offset, fbits(-1.5f), fbits(1.5f), expressions});
    cx(ExpressionVector_dtor, {expressions});
    cx(SafeBuffer_dtor, {style});
    uint32_t setPosition = ld32(ld32(icon) + node::setPosition);
    cx(setPosition, {icon, vx(avatar, node::getPosition, {avatar})});
    uint32_t parent = vx(avatar, node::getParent, {avatar});
    uint32_t addChild = ld32(ld32(parent) + node::addChildZName);
    uint32_t z = vx(avatar, 0x18, {avatar});                         // getLocalZOrder
    const uint32_t name = base + 0x30;
    cx(fn::string_init, {name, STR_AvatarIcon, 10});
    cx(addChild, {parent, icon, z, name});
    freeStr(name);
    release(avatarsCtl);
    release(dataCtl);
}
RW_FUNCTION(0x611724, "SceneManager::updateHeaderAvatarIcon", SceneManager::updateHeaderAvatarIcon);

// Refresh the header: level, AP, Lux gauge, jewels, chat, raid alert button.
void SceneManager::updateHeader(SceneManager *self) {
    Frame fr;
    const uint32_t base = fr.allocExact(0xC0);
    const uint32_t m = rw::guest_addr(self);
    uint32_t header = self->header;
    if (!header) return;
    const uint32_t value = base + 0x78;
    uint32_t level = search(header, base + 0x6C, STR_AtlasLabel_NumL, 15);
    uint32_t a = api();
    const uint32_t userData = a + API_USER_DATA;
    setAtlasNumber(level, base + 0x60, int32_t(readSafe(ld32(userData), value, 8, 4)));
    cx(SceneManager_updateHeaderAp, {m});
    uint32_t lux = search(self->header, base + 0x54, STR_AtlasLabel_LUX, 14);
    uint32_t kerning = rw::load<uint32_t>(ATLASNUM_KERNING_PIXEL);
    cx(Label_setAdditionalKerning, {vx(lux, 0x2D8, {lux}), kerning});
    uint32_t b = api();
    int32_t rank = int32_t(readSafe(ld32(userData), value, 8, 4));
    readSafe(ld32(b + API_USER_STATUS), value, 8, 8);
    const uint32_t gauge = base + 0x88;
    st32(gauge + 16, gauge);
    st32(gauge, VT_Header_LuxGauge);
    st32(gauge + 8, lux);
    st32(gauge + 4, m);
    // (int rank, u64 points in r2:r3, function by value at [sp]); r1 holds `this` in the original
    cx(SceneUtil_calcLuxGaugePer, {uint32_t(rank), m, ld32(value), ld32(value + 4), gauge});
    destroyFn(gauge);
    uint32_t c = api();
    uint32_t pointCtl = ld32(c + API_USER_POINT + 4), point = ld32(c + API_USER_POINT);
    addRef(pointCtl);
    uint32_t jewels = search(self->header, base + 0x48, STR_AtlasLabel_MS, 13);
    cx(Label_setAdditionalKerning, {vx(jewels, 0x2D8, {jewels}), kerning});
    uint32_t paid = readSafe(point, base + 0x80, 0, 4);
    uint32_t free = readSafe(point, base + 0x84, 4, 4);
    setAtlasNumber(jewels, base + 0x3C, int32_t(free + paid));
    cx(SceneManager_chatDataUpdate, {m, 0});
    uint32_t alerts = search(self->header, base + 0x30, STR_Alerts, 6);
    uint32_t d = api();
    uint32_t setVisible = ld32(ld32(alerts) + node::setVisible);
    uint32_t visible = 0;
    if (ld32(d + API_RAIDS) != ld32(d + API_RAIDS + 4)) visible = self->raidAlertsShown ? 1 : 0;
    cx(setVisible, {alerts, visible});
    self->raidAlertsShown = 1;
    release(pointCtl);
}
RW_FUNCTION(0x611168, "SceneManager::updateHeader", SceneManager::updateHeader);

// The AP display: the AP popup's recovery countdown (when open), the header's next-AP / full-charge timers, the
// AP / max AP numbers (an "l" suffix when over the max) and the gauge width (101 px when full).
void SceneManager::updateHeaderAp(SceneManager *self) {
    Frame fr;
    const uint32_t base = fr.allocExact(0xD18);
    const uint32_t m = rw::guest_addr(self);
    if (getInstance()->apRecoveryText) {
        const uint32_t seconds = base + 0xEC;
        cx(AppState_getCurrentAP, {cx(fn::AppState_getInstance, {}), seconds});
        if (ld32(m + SM_AP_SECONDS) != ld32(seconds)) {
            uint32_t text = getInstance()->apRecoveryText;
            const uint32_t format = base + 0xD8, buffer = base + 0x8FC;
            cx(fn::TextData_get, {format, 0, uint32_t(TEXT_AP_RECOVERY)});
            int32_t s = int32_t(ld32(seconds));
            cx(fn::StringBuffer_ctor, {buffer, ld32(ld32(format) + 4), uint32_t(s / 60), uint32_t(s % 60)});
            setTextFromBuffer(text, base + 0xE0, buffer);
            release(ld32(format + 4));
            st32(m + SM_AP_SECONDS, ld32(seconds));
            return;
        }
    }
    uint32_t header = self->header;
    if (!header) return;
    const uint32_t seconds = base + 0xD4;
    uint32_t ap = cx(AppState_getCurrentAP, {cx(fn::AppState_getInstance, {}), seconds});
    if (ld32(m + SM_AP_SECONDS) != ld32(seconds)) {
        uint32_t wording1 = search(self->header, base + 0xC8, STR_Txt_Wording1, 12);
        uint32_t next = search(wording1, base + 0xBC, STR_Txt_Time_Label, 14);
        uint32_t wording2 = search(self->header, base + 0xB0, STR_Txt_Wording2, 12);
        uint32_t full = search(wording2, base + 0xA4, STR_Txt_Time_Label, 14);
        int32_t fullSec = int32_t(cx(AppState_getFullChargeApSec, {cx(fn::AppState_getInstance, {})}));
        int32_t s = int32_t(ld32(seconds));
        const uint32_t buffer1 = base + 0x4F8, buffer2 = base + 0xF4;
        cx(fn::StringBuffer_ctor, {buffer1, STR_02d02d, uint32_t(s / 60), uint32_t(s % 60)});
        setTextFromBuffer(next, base + 0x98, buffer1);
        int32_t sec = fullSec % 60;
        int32_t hours = fullSec / 3600;
        int32_t minutes = (fullSec % 3600) / 60;
        cx(fn::StringBuffer_ctor, {buffer2, STR_02d02d02d, uint32_t(hours), uint32_t(minutes), uint32_t(sec)});
        setTextFromBuffer(full, base + 0x8C, buffer2);
        st32(m + SM_AP_SECONDS, ld32(seconds));
    }
    const uint32_t maxValue = base + 0xF0;
    if (ld32(m + SM_AP_SHOWN) == ap) {
        uint32_t shownMax = ld32(m + SM_AP_MAX_SHOWN);
        if (shownMax == readSafe(ld32(api() + API_USER_STATUS), maxValue, 0x3C, 4)) return;
    }
    st32(m + SM_AP_SHOWN, ap);
    uint32_t maxAp = readSafe(ld32(api() + API_USER_STATUS), maxValue, 0x3C, 4);
    st32(m + SM_AP_MAX_SHOWN, maxAp);
    int32_t shown = int32_t(ld32(m + SM_AP_SHOWN));
    uint32_t label = search(self->header, base + 0x80, STR_AtlasLabel_AP, 13);
    if (int32_t(maxAp) < shown) {
        const uint32_t number = base + 0x68, text = base + 0x74;
        cx(SceneUtil_numberToAtlasString, {number, ld32(m + SM_AP_MAX_SHOWN)});
        uint32_t r = cx(fn::string_append, {number, STR_l, 1});
        st32(text, ld32(r));
        st32(text + 4, ld32(r + 4));
        st32(text + 8, ld32(r + 8));
        st32(r, 0);
        st32(r + 4, 0);
        st32(r + 8, 0);
        cx(TextAtlas_setString, {label, text});
        freeStr(text);
        freeStr(number);
    } else {
        setAtlasNumber(label, base + 0x5C, int32_t(ld32(m + SM_AP_SHOWN)));
    }
    uint32_t maxLabel = search(self->header, base + 0x50, STR_AtlasLabel_AP_Max, 17);
    setAtlasNumber(maxLabel, base + 0x44, int32_t(ld32(m + SM_AP_MAX_SHOWN)));
    uint32_t tip = search(self->header, base + 0x38, STR_Tip_AP, 6);
    float ratio = float(int32_t(ld32(m + SM_AP_SHOWN))) / float(int32_t(ld32(m + SM_AP_MAX_SHOWN)));
    float width = ratio < 1.0f ? ratio * 101.0f : 101.0f;
    uint32_t setContentSize = ld32(ld32(tip) + node::setContentSize);
    uint32_t size = vx(tip, node::getContentSize, {tip});
    const uint32_t newSize = base + 0x30;
    cx(Size_ctor, {newSize, fbits(width), ld32(size + 4)});
    cx(setContentSize, {tip, newSize});
}
RW_FUNCTION(0x60D9CC, "SceneManager::updateHeaderAp", SceneManager::updateHeaderAp);

// ------------------------------------------------------------------------------------------------ update

// Every frame: react to the API status (push token registration, errors that send back to the title), then run
// the scene change state machine (wait for APIs -> change scene and fade out -> show the new layer -> fade in).
void SceneManager::update(SceneManager *self, float dt) {
    Frame fr;
    const uint32_t base = fr.allocExact(0xF8);
    const uint32_t m = rw::guest_addr(self);
    const uint32_t state = m + SM_SCENE_STATE;
    uint32_t status = ld32(api() + API_STATUS);
    switch (status) {
    case 0: {
        if (ld32(state) != 2) break;
        uint32_t notification = ld32(api() + API_REMOTE_NOTIFICATION);
        if (notification && cx(RemoteNotification_needsGetToken, {notification}) == 1) {
            const uint32_t onToken = base + 0xC0;
            st32(onToken + 16, onToken);
            st32(onToken, VT_Update_Token);
            st32(onToken + 4, m);
            cx(RemoteNotification_getToken, {onToken});
            destroyFn(onToken);
        }
        if (self->abortOnNextScene) break;
        const uint32_t token = m + SM_PUSH_TOKEN;
        bool isLong = ld8(token) & 1;
        if ((isLong ? ld32(token + 4) : uint32_t(ld8(token) >> 1)) == 0) break;
        uint32_t a = api();
        uint32_t text = (ld8(token) & 1) ? ld32(token + 8) : token + 1;
        const uint32_t done = base + 0xA8;
        st32(done + 16, 0);
        cx(APIManager_executeSystemPushRegist, {a, text, done});
        destroyFn(done);
        if (ld8(token) & 1) {
            st8(ld32(token + 8), 0);
            st32(token + 4, 0);
        } else {
            st8(token + 1, 0);
            st8(token, 0);
        }
        st32(state, 0);
        break;
    }
    case 1:
        if (!self->abortOnNextScene && ld32(state) == 2) return;
        break;
    case 2:
    case 5: {
        const uint32_t empty = status == 2 ? base + 0x90 : base + 0x48;
        st32(empty + 16, 0);
        if (status == 2) cx(SceneManager_moveToTitleForcibly, {m, uint32_t(TEXT_TITLE_ERROR_1), empty});
        else cx(SceneManager_moveToTitleIfPossible, {m, uint32_t(TEXT_TITLE_ERROR_2), empty});
        destroyFn(empty);
        break;
    }
    case 3: {
        const uint32_t onClose = base + 0x78;
        st32(onClose + 16, onClose);
        st32(onClose, VT_Update_TitleError);
        cx(SceneManager_moveToTitleForcibly, {m, uint32_t(TEXT_TITLE_ERROR_3), onClose});
        destroyFn(onClose);
        break;
    }
    case 4: {
        const uint32_t empty = base + 0x60;
        st32(empty + 16, 0);
        cx(SceneManager_moveToTitleIfPossible, {m, uint32_t(TEXT_TITLE_ERROR_4), empty});
        destroyFn(empty);
        break;
    }
    default:
        break;
    }

    switch (ld32(state)) {
    case 0:   // waiting for the scene's APIs
        cx(SceneManager_enableLoadingIndicator, {m, 1});
        if (ld8(api() + API_ERROR)) {
            uint32_t indicator = self->loadingIndicator;
            if (vx(indicator, node::getParent, {indicator})) cx(LoadingIndicator_hide, {self->loadingIndicator});
            uint32_t a = api();
            if (cx(APIManager_openErrorDialog, {a, m + 0x548, 1}) == 1) st32(state, 1);
            return;
        }
        if (ld8(api() + API_BUSY)) return;
        {
            uint32_t user = ld32(api() + API_FACEBOOK);
            if (user && (ld8(user + 0x35) & 2) &&
                cx(FacebookManager_isConnecting, {cx(FacebookManager_getInstance, {})}))
                return;
        }
        if (self->flag4CD) {
            cx(SceneManager_callPrevChangeNextSceneAPIs, {m});
            self->flag4CD = 0;
        } else {
            uint32_t indicator = self->loadingIndicator;
            if (vx(indicator, node::getParent, {indicator})) cx(LoadingIndicator_hide, {self->loadingIndicator});
            st32(state, 2);
        }
        return;
    case 1:   // error dialog open
        if (ld8(api() + API_ERROR)) return;
        st32(state, ld8(api() + API_BUSY) ? 0 : 2);
        return;
    case 2: { // change the scene
        destroyFn(m + 0x548);
        st32(m + 0x548 + 16, 0);
        destroyFn(m + 0x530);
        st32(m + 0x530 + 16, 0);
        if (self->sceneLayer) {
            cx(Ref_release, {self->sceneLayer});
            self->sceneLayer = 0;
        }
        cx(APIManager_reallocate, {api()});
        if (self->flag31C) {
            cx(LWFResourceCache_unloadAll, {cx(LWFResourceCache_shared, {})});
            cx(AudioClearGameMainBGMStack, {});
            cx(AudioManager_unloadAll, {cx(AudioManager_getInstance, {})});
            cx(RESTClient_cancelAll, {cx(RESTClient_getInstance, {})});
            cx(ArmatureDataManager_clear, {cx(ArmatureDataManager_getInstance, {})});
        }
        self->loadingBlocked = 0;
        self->touchEffectEnabled = 1;
        uint32_t touchLayer = self->touchEffectLayer;
        vx(touchLayer, node::setVisible, {touchLayer, 1});
        self->flag588 = int32_t(cx(SceneUtil_getNexTutorialphase, {})) > 709 ? 1 : 0;
        cx(SceneManager_createNextScene, {m});
        if (self->headerRequested) {
            if (!self->header) {
                self->header = cx(SceneManager_createHeader, {m});
                cx(SceneManager_initGlobalMenu, {m});
                cx(SceneManager_changeTickerMessage, {m, m + 0x350});
                vx(m, node::addChildZ, {m, self->header, 4});
            }
        } else {
            st32(m + SM_FIELD_3FC, 0);
        }
        if (self->titleHeaderShown && !self->titleHeader) {
            st32(m + SM_TITLE_CURRENT, self->titleMain);
            st32(m + SM_TITLE_CURRENT + 8, self->titleSub1);
            st32(m + SM_TITLE_CURRENT + 0x10, self->titleSub2);
            cx(SceneManager_createTitleHeader, {m, 0x3C0});   // the title ids are read from the fields
            uint32_t title = self->titleHeader;
            const uint32_t none = base + 0x30;
            st32(none + 16, 0);
            uint32_t in = cx(SceneUtil_createLeftInAnimation, {title, 0, 0, none});
            cx(fn::Node_runAction, {title, cx(EaseSineOut_create, {in})});
            destroyFn(none);
            vx(m, node::addChildZ, {m, self->titleHeader, 4});
        }
        cx(SceneManager_updateHeader, {m});
        uint32_t fade = self->fadeLayer;
        cx(fn::Node_runAction, {fade, cx(FadeOut_create, {fbits(0.15f)})});
        st32(state, 3);
        return;
    }
    case 3:   // fading out
        if (cx(Node_getNumberOfRunningActions, {self->fadeLayer})) return;
        st32(state, 4);
        vx(self->fadeLayer, node::setVisible, {self->fadeLayer, 1});
        vx(m, node::removeChild, {m, self->fadeLayer, 1});
        return;
    case 4:   // running; a queued scene starts with a fade in
        if (ld32(m + 0x3C8 + 16)) {
            uint32_t fullScreen = self->flag31D;
            uint32_t fade = self->fadeLayer;
            uint32_t setContentSize = ld32(ld32(fade) + node::setContentSize);
            uint32_t width = ld32(vx(fade, node::getContentSize, {fade}));
            float height = bitsf(cx(fn::AppGetGrameFrameHeight, {}));
            uint32_t size;
            if (fullScreen) {
                size = base + 0x28;
            } else {
                uint32_t header = self->header;
                height = height - ldf(vx(header, node::getContentSize, {header}) + 4);
                size = base + 0x20;
            }
            cx(Size_ctor, {size, width, fbits(height)});
            cx(setContentSize, {fade, size});
            vx(self->fadeLayer, node::setVisible, {self->fadeLayer, self->flag31E});
            vx(m, node::addChildZ, {m, self->fadeLayer, 11});
            uint32_t layer = self->fadeLayer;
            cx(fn::Node_runAction, {layer, cx(FadeIn_create, {fbits(0.15f)})});
            st32(state, 5);
        }
        cx(SceneManager_updateHeaderAp, {m});
        cx(DeckMedalViewManager_update, {cx(DeckMedalViewManager_getInstance, {}), fbits(dt)});
        cx(SceneManager_chatDataUpdate, {m, fbits(dt)});
        return;
    case 5:   // fading in
        if (cx(Node_getNumberOfRunningActions, {self->fadeLayer})) return;
        vx(m, node::removeChild, {m, self->sceneLayer, 1});
        if (self->loadingBlocked) {
            st32(state, 0);
            return;
        }
        st32(state, 2);
        if (self->flag4CD) {
            cx(SceneManager_callPrevChangeNextSceneAPIs, {m});
            self->flag4CD = 0;
            if (self->loadingBlocked) st32(state, 0);
        }
        return;
    default:
        return;
    }
}
RW_FUNCTION(0x60D27C, "SceneManager::update", SceneManager::update);
