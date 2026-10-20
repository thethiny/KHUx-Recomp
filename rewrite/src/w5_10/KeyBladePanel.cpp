// Equipment > Keyblade LV UP: SceneDeckEdit::openKeyBladePanel (the strength popup over the deck editor, with the
// keyblade tutorial on first use), setKeybladeArrowVisible and updateKeyBladePanel (the shown keyblade's name,
// ability ticker, burst level, image, and its four level-up materials with owned / required counts).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t ThrowOutOfRangeSlide = 0x664B11, ThrowOutOfRangeKeyblades = 0x664B15;
constexpr uint32_t DIVSI3 = 0xCD5E8D;
constexpr uint32_t BaseSlideView_setSlideEnabled = 0x526111;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;   // (sret function, id, function, Widget*, Widget*, Widget*, bool)
constexpr uint32_t SceneUtil_runTicker = 0x465AB9;            // (node, float, float, float)
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;     // (Button*, Text*, bool, bool)
constexpr uint32_t SceneUtil_setNormalColor = 0x468669, SceneUtil_setWarningColor = 0x4686A9;
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_isClearTutorialPopupIDs = 0x4EB815;
constexpr uint32_t AppState_setTutorialCloseFunc = 0x4EB8D9;  // (sret function, appState, function, id)
constexpr uint32_t SM_openPopup = 0x61524D;                // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D, KeyBladeDataManager_getKeyBladeData = 0x76B659;
constexpr uint32_t KeyBladeDataManager_getKeyBladeImagePath = 0x76B809;   // (mgr, shared_ptr by value) -> const char*
constexpr uint32_t Material_create = 0x7852E5;
constexpr uint32_t setKeybladeArrowVisible = 0x663DD9, updateKeyBladePanel = 0x663ED9;
}  // namespace g

// SceneDeckEdit fields
constexpr uint32_t LEFT = 0x2EC, RIGHT = 0x2F0, KBSTRENGTH = 0x2F8, BUTTON_MEDAL = 0x310, BUTTON_KB = 0x318;
constexpr uint32_t KEYBLADES = 0x324, OPEN_KB_PANEL = 0x37D, SLIDE = 0x38C;
constexpr uint32_t KEYBLADE_SIZE = 0x58, KEYBLADES_DIV = 0xBA2E8BA3u;
constexpr uint32_t SLIDE_IDS = 0x578, SLIDE_INDEX = 0x5AC;
constexpr uint32_t VT_PowerUpCheck = 0xF639F0;   // function<bool()> {close button}
constexpr uint32_t VT_PowerUpPopup = 0xF63998;   // function<void(bool)> {this, panel, power up, close, scale} (heap 0x18)
// strings
constexpr uint32_t S_WinUnder = 0xE03EF1, S_CloseButton = 0xDF8D2F, S_PowerUpButton = 0xE03F9D, S_LeftArrow = 0xDFF5DA;
constexpr uint32_t S_KBNameLabel = 0xDF880D, S_FmtS = 0xDF8821, S_AreaTicker = 0xE03FB8, S_AbilityLabel = 0xDF8824;
constexpr uint32_t S_TxtMaxBurst = 0xDF8F15, S_TxtMaxBurstLabel = 0xDF8F22, S_FmtCount = 0xDF8F35;
constexpr uint32_t S_KBDummy = 0xDF8832, S_TxtPowerUp = 0xE03FAC, S_AreaMaterial = 0xE03EFB, S_MaterialFmt = 0xE03F3C;
constexpr uint32_t S_MaterialIcon = 0xE03FC4, S_MaterialImageFmt = 0xDF9101, S_MaterialNameLabel = 0xE03FD2;
constexpr uint32_t S_MaterialNowLabel = 0xE03FE6, S_FmtD = 0xDF86DE, S_MaterialSlash = 0xE03FF8;
constexpr uint32_t S_MaterialMaxLabel = 0xE04007, S_TxtAlert = 0xE04019;
constexpr int32_t TXT_MAX_BURST = 101900007, TXT_ALERT = 101900006;
constexpr int32_t TUTORIAL_KEYBLADE = 114, TUTORIAL_POPUP_KEYBLADE = 105;
// SceneManager / APIManager
constexpr uint32_t SM_OPEN_KB_PANEL = 0x2E0;
constexpr uint32_t API_MATERIALS = 0xE4;      // SafeBuffer: 16-byte records (+8 material id, +0xC count), count at +0x20
// master::Keyblade row (the UserKeyBladeData's +0x20)
constexpr uint32_t KB_NAME = 4, KB_ABILITY = 0x45, KB_MAX_BURST = 0xE0, KB_NEXT = 0x13C, KB_MATERIAL_COUNT = 0x140;
constexpr uint32_t KB_MATERIAL_IDS = 0x144, KB_MATERIAL_NEEDS = 0x154;

constexpr uint32_t V_getScale = 0x44, V_isVisible = 0xB4, V_widgetSetTouchEnabled = 0x29C, V_setClipping = 0x31C;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
void sbRead(uint32_t sb, uint32_t dst, uint32_t offset, uint32_t len) {
    rw::call(g::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, len, offset});
}
void setTextFormat(uint32_t text, std::initializer_list<uint32_t> fmtAndArgs) {
    Frame f;
    const uint32_t buf = stringBuffer(f, fmtAndArgs);
    setTextC(text, buf);
}
}  // namespace

struct KeyBladePanel {
    static void openKeyBladePanel(uint32_t self, uint32_t noAnimation);
    static void setKeybladeArrowVisible(uint32_t self, uint32_t visible);
    static void updateKeyBladePanel(uint32_t self);
};

// The keyblade strength popup (not already open): slide off, the panel's buttons, the popup with its close check,
// the content, the open animation (unless `noAnimation`), and the keyblade tutorial the first time.
void KeyBladePanel::openKeyBladePanel(uint32_t me, uint32_t noAnimation) {
    {
        const uint32_t panel = ld32(me + KBSTRENGTH);
        if (vcall(panel, V_isVisible, {panel})) return;
    }
    call(g::BaseSlideView_setSlideEnabled, {ld32(me + SLIDE), 0});
    const uint32_t winUnder = findNode(ld32(me + KBSTRENGTH), S_WinUnder, 9);
    const uint32_t scale = vcall(winUnder, V_getScale, {winUnder});
    vcall(winUnder, V_widgetSetTouchEnabled, {winUnder, 1});
    call(g::setKeybladeArrowVisible, {me, 0});
    const uint32_t close = findNode(ld32(me + KBSTRENGTH), S_CloseButton, 0xC);
    const uint32_t powerUp = findNode(ld32(me + KBSTRENGTH), S_PowerUpButton, 0xE);
    {
        const uint32_t m = sceneManager();
        Frame f;
        const uint32_t check = newFunction(f, VT_PowerUpCheck, {close});
        const uint32_t result = f.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, VT_PowerUpPopup);
            st32(obj + 4, me);
            st32(obj + 8, winUnder);
            st32(obj + 12, powerUp);
            st32(obj + 16, close);
            st32(obj + 20, scale);
        }
        st32(result + 16, obj);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(check);
    }
    vcall(ld32(me + BUTTON_MEDAL), V_widgetSetTouchEnabled, {ld32(me + BUTTON_MEDAL), 0});
    vcall(ld32(me + BUTTON_KB), V_widgetSetTouchEnabled, {ld32(me + BUTTON_KB), 0});
    vcall(powerUp, V_widgetSetTouchEnabled, {powerUp, 1});
    vcall(close, V_widgetSetTouchEnabled, {close, 1});
    call(g::updateKeyBladePanel, {me});
    if (!noAnimation) {
        call(g::AudioPlaySystemSEPopUp, {});
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(g::Node_runAction, {winUnder, call(g::SceneUtil_createOpenPopupAnimation, {winUnder, none})});
        functionDestroy(none);
    }
    st8(sceneManager() + SM_OPEN_KB_PANEL, 1);
    st8(me + OPEN_KB_PANEL, 1);
    setVisible(ld32(me + KBSTRENGTH), 1);
    if (call(g::AppState_isClearTutorialPopupIDs, {call(g::AppState_getInstance, {}), TUTORIAL_POPUP_KEYBLADE}) == 1) {
        const uint32_t app = call(g::AppState_getInstance, {});
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        const uint32_t closeFn = f.alloc(20), result = f.alloc(20);
        rw::call(g::SceneUtil_openTutorialPopup, {closeFn, uint32_t(TUTORIAL_KEYBLADE), none, powerUp, 0, 0, 0});
        rw::call(g::AppState_setTutorialCloseFunc, {result, app, closeFn, uint32_t(TUTORIAL_KEYBLADE)});
        functionDestroy(result);
        functionDestroy(closeFn);
        functionDestroy(none);
    }
}

// The left / right keyblade arrows (their first child), shown only with more than one keyblade.
void KeyBladePanel::setKeybladeArrowVisible(uint32_t me, uint32_t visible) {
    const uint32_t count = uint32_t(int32_t(ld32(me + KEYBLADES + 4) - ld32(me + KEYBLADES)) >> 3) * KEYBLADES_DIV;
    const uint32_t shown = (count > 1 && (visible & 0xFF)) ? 1 : 0;
    const uint32_t arrow = findNode(ld32(me + LEFT), S_LeftArrow, 0xA);
    setVisible(ld32(ld32(vcall(arrow, node::getChildren, {arrow}))), shown);
    const uint32_t right = ld32(me + RIGHT);
    setVisible(ld32(ld32(vcall(right, node::getChildren, {right}))), shown);
}

// The shown keyblade's strength panel.
void KeyBladePanel::updateKeyBladePanel(uint32_t me) {
    const uint32_t winUnder = findNode(ld32(me + KBSTRENGTH), S_WinUnder, 9);
    findNode(ld32(me + KBSTRENGTH), S_CloseButton, 0xC);
    const uint32_t powerUp = findNode(ld32(me + KBSTRENGTH), S_PowerUpButton, 0xE);
    // the keyblade of the slide's page
    const uint32_t slide = ld32(me + SLIDE);
    const uint32_t ids = ld32(slide + SLIDE_IDS);
    const uint32_t index = ld32(slide + SLIDE_INDEX);
    if (!(uint32_t(int32_t(ld32(slide + SLIDE_IDS + 4) - ids) >> 2) > index)) {
        call(g::ThrowOutOfRangeSlide, {me + KEYBLADES, slide + SLIDE_IDS});
        return;
    }
    const uint32_t keyblades = ld32(me + KEYBLADES);
    const uint32_t kbIndex = ld32(ids + 4 * index);
    if (!(uint32_t(int32_t(ld32(me + KEYBLADES + 4) - keyblades) >> 3) * KEYBLADES_DIV > kbIndex)) {
        call(g::ThrowOutOfRangeKeyblades, {});
        return;
    }
    const uint32_t kb = keyblades + KEYBLADE_SIZE * kbIndex;
    const uint32_t mgr = call(g::KeyBladeDataManager_getInstance, {});
    Frame f;
    const uint32_t data = f.alloc(8);
    call(g::KeyBladeDataManager_getKeyBladeData, {data, mgr, ld32(kb), ld32(kb + 4), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    const auto row = [&]() { return ld32(ld32(data) + 0x20); };
    {
        const uint32_t name = findNode(winUnder, S_KBNameLabel, 0x13);
        SceneUtil::setShadow(name);
        Frame t;
        const uint32_t buf = t.alloc(0x44);
        sbRead(row(), buf, KB_NAME, 0x41);
        setTextFormat(name, {S_FmtS, buf});
    }
    const uint32_t ticker = findNode(winUnder, S_AreaTicker, 0xB);
    {
        const uint32_t ability = findNode(winUnder, S_AbilityLabel, 0xD);
        SceneUtil::setShadow(ability);
        {
            Frame t;
            const uint32_t buf = t.alloc(0x84);
            sbRead(row(), buf, KB_ABILITY, 0x81);
            setTextFormat(ability, {S_FmtS, buf});
        }
        vcall(ticker, V_setClipping, {ticker, 1});
        const uint32_t width = ld32(vcall(ticker, node::getContentSize, {ticker}));
        const float x = bitsf(vcall(ability, node::getPositionX, {ability}));
        const float width2 = ldf(vcall(ticker, node::getContentSize, {ticker}));
        const uint32_t x2 = vcall(ability, node::getPositionX, {ability});
        call(g::SceneUtil_runTicker, {ability, width, fbits(x + width2), x2});
    }
    {
        const uint32_t text = findNode(winUnder, S_TxtMaxBurst, 0xC);
        SceneUtil::setShadow(text);
        setTextId(text, TXT_MAX_BURST);
    }
    {
        const uint32_t label = findNode(winUnder, S_TxtMaxBurstLabel, 0x12);
        SceneUtil::setShadow(label);
        const uint32_t level = ld32(kb + 0x40);
        Frame t;
        const uint32_t max = t.alloc(4);
        sbRead(row(), max, KB_MAX_BURST, 4);
        const uint32_t shown = call(g::DIVSI3, {level, 10000});
        setTextFormat(label, {S_FmtCount, shown, ld32(max)});
    }
    {
        const uint32_t image = findNode(winUnder, S_KBDummy, 0xE);
        const uint32_t m = call(g::KeyBladeDataManager_getInstance, {});
        Frame t;
        const uint32_t copy = t.alloc(8);
        st32(copy, ld32(data));
        st32(copy + 4, ld32(data + 4));
        addRefIf(ld32(copy + 4));
        const uint32_t path = call(g::KeyBladeDataManager_getKeyBladeImagePath, {m, copy});
        const uint32_t s = newStr(t, path);
        call(g::ImageView_loadTexture, {image, s, 0});
        strFree(s);
        releaseIf(ld32(copy + 4));
    }
    const uint32_t powerUpText = findNode(powerUp, S_TxtPowerUp, 0xB);
    const uint32_t area = findNode(winUnder, S_AreaMaterial, 0xD);
    uint32_t enough = 1;
    for (int32_t k = 0; k <= 3;) {
        uint32_t material;
        {
            Frame t;
            const uint32_t buf = stringBuffer(t, {S_MaterialFmt, uint32_t(k + 1)});
            const uint32_t s = newStr(t, buf);
            material = SceneUtil::searchNodeByName(area, s);
            strFree(s);
        }
        Frame t;
        const uint32_t count = t.alloc(4);
        sbRead(row(), count, KB_MATERIAL_COUNT, 4);
        if (!(int32_t(ld32(count)) > k)) {
            setVisible(material, 0);
            k++;
            continue;
        }
        setVisible(material, 1);
        const uint32_t ids = t.alloc(0x10);
        sbRead(row(), ids, KB_MATERIAL_IDS, 0x10);
        const uint32_t mat = t.alloc(8);
        call(g::Material_create, {mat, ld32(ids + 4 * uint32_t(k))});
        {
            const uint32_t icon = findNode(material, S_MaterialIcon, 0xD);
            const uint32_t imageId = t.alloc(4);
            sbRead(ld32(mat), imageId, 0, 4);
            Frame p;
            const uint32_t buf = stringBuffer(p, {S_MaterialImageFmt, ld32(imageId)});
            const uint32_t s = newStr(p, buf);
            call(g::ImageView_loadTexture, {icon, s, 0});
            strFree(s);
        }
        {
            const uint32_t label = findNode(material, S_MaterialNameLabel, 0x13);
            SceneUtil::setShadow(label);
            Frame p;
            const uint32_t buf = p.alloc(0x44);
            sbRead(ld32(mat), buf, 0xC, 0x41);
            setTextFormat(label, {S_FmtS, buf});
        }
        {
            const uint32_t now = findNode(material, S_MaterialNowLabel, 0x11);
            SceneUtil::setShadow(now);
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t ctl = ld32(api + API_MATERIALS + 4), owned = ld32(api + API_MATERIALS);
            addRefIf(ctl);
            uint32_t have = 0;
            if (ld32(owned + 0x20)) {
                Frame p;
                const uint32_t rec = p.alloc(0x10), wanted = p.alloc(0x10);
                for (uint32_t j = 0, at = 0;; at += 0x10) {
                    sbRead(owned, rec, at, 0x10);
                    const uint32_t id = ld32(rec + 8);
                    sbRead(row(), wanted, KB_MATERIAL_IDS, 0x10);
                    j++;
                    if (id == ld32(wanted + 4 * uint32_t(k))) {
                        have = ld32(rec + 12);
                        break;
                    }
                    if (j >= ld32(owned + 0x20)) break;
                }
            }
            setTextFormat(now, {S_FmtD, have});
            {
                Frame p;
                const uint32_t needs = p.alloc(0x10);
                sbRead(row(), needs, KB_MATERIAL_NEEDS, 0x10);
                if (int32_t(have) >= int32_t(ld32(needs + 4 * uint32_t(k)))) {
                    call(g::SceneUtil_setNormalColor, {now});
                } else {
                    enough = 0;
                    call(g::SceneUtil_setWarningColor, {now});
                }
            }
            SceneUtil::setShadow(findNode(material, S_MaterialSlash, 0xE));
            const uint32_t max = findNode(material, S_MaterialMaxLabel, 0x11);
            SceneUtil::setShadow(max);
            {
                Frame p;
                const uint32_t needs = p.alloc(0x10);
                sbRead(row(), needs, KB_MATERIAL_NEEDS, 0x10);
                setTextFormat(max, {S_FmtD, ld32(needs + 4 * uint32_t(k))});
            }
            releaseIf(ctl);
        }
        releaseIf(ld32(mat + 4));
        k++;
    }
    uint32_t next;
    {
        Frame t;
        const uint32_t v = t.alloc(4);
        sbRead(row(), v, KB_NEXT, 4);
        next = ld32(v);
    }
    const uint32_t alert = findNode(winUnder, S_TxtAlert, 9);
    SceneUtil::setShadow(alert);
    setTextId(alert, TXT_ALERT);
    setVisible(alert, next ? 0 : 1);
    if (enough && next) call(g::SceneUtil_setButtonEnabled, {powerUp, powerUpText, 1, 0});
    else call(g::SceneUtil_setButtonEnabled, {powerUp, powerUpText, 0, 0});
    releaseIf(ld32(data + 4));
}

RW_FUNCTION(0x662E2C, "SceneDeckEdit::openKeyBladePanel", KeyBladePanel::openKeyBladePanel);
RW_FUNCTION(0x663DD8, "SceneDeckEdit::setKeybladeArrowVisible", KeyBladePanel::setKeybladeArrowVisible);
RW_FUNCTION(0x663ED8, "SceneDeckEdit::updateKeyBladePanel", KeyBladePanel::updateKeyBladePanel);
