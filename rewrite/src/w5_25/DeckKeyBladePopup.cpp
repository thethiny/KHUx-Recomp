// SceneDeckEdit::openKeyBladePopup(keybladeId, keyblade): the keyblade strengthen result (DeckEdit_KBStrength_
// Result): the level-up LWF, OK ($_27), the name, the ability ticker, the medal cost (the deck's cost / the new
// maximum, an arrow when it grew), the keyblade image, the revolver with its five medal slots (DeckMedalView each:
// slot magnifications; an up arrow per magnification that grew; a slot opened by this level gets the KeyEvo LWFs
// - "_root" enterFrame and "medal_blank_slot" load handlers - and the first such slot the keyblade glow), then the
// open animation and SceneManager::openPopup with $_31 (can close) / $_32 (closed).
//
// The original frame (0x2208 bytes) is kept: getKeyBladeData and DeckMedalView::setData take stack arguments.
// `keyblade` is the strengthened keyblade (UserKeyBladeData: +0x20 its master::Keyblade row); keybladeId the id
// whose master data is the level before.
#include "../scene/SceneUi.hpp"
#include "../w3_6/Common.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneManager_getPopupLayer2 = 0x613E71;
constexpr uint32_t SceneManager_openPopup = 0x61524D;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_runTicker = 0x465AB9;              // (Node* text, float, float, float)
constexpr uint32_t SceneUtil_setParameterUpEffect = 0x465995;
constexpr uint32_t SceneUtil_setNormalColor = 0x468669;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D;
constexpr uint32_t KeyBladeDataManager_getKeyBladeData = 0x76B659;   // (sret, mgr, u64, unsigned id, Keyblade)
constexpr uint32_t KeyBladeDataManager_getKeyBladeImagePath = 0x76B809;   // (mgr, shared_ptr by value)
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance, ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t DeckMedalView_create = 0x52BA71;            // (bool)
constexpr uint32_t DeckMedalView_setData = 0x52BEE5;           // see the call
constexpr uint32_t DeckMedalView_setIsShowDarklightMagnification = 0x52BFDD;
constexpr uint32_t DeckRevolverView_settingSlotOpenLevels = 0x52D435;   // (shared_ptr by value, vector<int>&)
constexpr uint32_t VectorInt_pushBackSlow = 0x47D709, VectorBaseInt_dtor = 0x478961;
constexpr uint32_t ResultArrow = 0x666F85;                     // sub_666F84: the result arrow animation
constexpr uint32_t divsi3 = 0xCD5E8D;
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t GOT_CAROUSEL_NAME = 0xFD6604, GOT_BUTTON_NAME = 0xFD6608;   // CarouselDeck::DECK_STYLE_RES_*
}  // namespace g

constexpr uint32_t FRAME = 0x2208;
// strings
constexpr uint32_t S_ResultJson = 0xE04063;   // "cocostudio/publish/DeckEdit_KBStrength_Result.json"
constexpr uint32_t S_Win = 0xE04096, S_Area_Point = 0xDF9FB4;
constexpr uint32_t S_LevelUpLwf = 0xE0409A;   // "lwf/result/KeyEvo_LevelUp/KeyEvo_LevelUp.lwf"
constexpr uint32_t S_Button_OK = 0xDF8688, S_Txt_OK = 0xDF869F, S_KeyBlade_Name_Label = 0xDF880D;
constexpr uint32_t S_FmtS = 0xDF8821;          // "%s"
constexpr uint32_t S_Area_Ticker_Ability = 0xDF8B15, S_Txt_Ticker_Ability = 0xDF8B29;
constexpr uint32_t S_BurstCost = 0xDF8B3C, S_AllNum_Label = 0xE040C7, S_Num_Label = 0xDF8A89;
constexpr uint32_t S_FmtD = 0xDF86DE, S_FmtDSlash = 0xE040D4;   // "%d", "%d/"
constexpr uint32_t S_KeyBlade_Dummy = 0xDF8832;
constexpr uint32_t S_ArrowJson = 0xE040D8;    // ".../DeckEdit_KBStrength_ResultArrow_Animation.ExportJson"
constexpr uint32_t S_Area_Arrow = 0xE04120, S_Area_Arrow2 = 0xE0412B;
constexpr uint32_t S_Revolver = 0xDFE17A, S_Revolver_Center = 0xDFE183, S_Area_N = 0xDFE172;   // "Area_%d"
constexpr uint32_t S_Dummy_Mag = 0xDFDDB9, S_Txt_Mag_Label = 0xDFDDCD, S_Dummy_Mag2 = 0xDFDD63;
constexpr uint32_t S_MedalLwf = 0xE044C4;     // "lwf/keyblade/KeyEvo/KeyEvo_Medal_01B/KeyEvo_Medal_01B.lwf"
constexpr uint32_t S_EffLwf = 0xE04137;       // "lwf/keyblade/KeyEvo/KeyEvo_eff/KeyEvo_eff.lwf"
constexpr uint32_t S_MedalBlankSlot = 0xE044FE;   // "medal_blank_slot"
constexpr int32_t TXT_OK = 100100001, TXT_BURST_COST = 101900007;
constexpr uint32_t MAGNIFICATION_GLOW = 0xE6E298;   // int[3]: by slot attribute 1..3

// master::Keyblade row positions
constexpr uint32_t KB_ID = 0, KB_NAME = 4, KB_ABILITY = 0x45, KB_STYLE = 0xD0, KB_SLOTS = 0xDC, KB_COST = 0xE0;
constexpr uint32_t KB_SLOT_MAG = 0xE8, KB_SLOT_ATTR = 0xFC, KB_SLOT_MAG2 = 0x110, KB_SLOT_MAG3 = 0x124;  // int[5]
constexpr uint32_t UKB_ROW = 0x20;
// SceneDeckEdit
constexpr uint32_t DE_PANEL = 0x314, DE_DECKS = 0x324, DE_SLIDE = 0x38C;
constexpr uint32_t DECK_SIZE = 0x58, DECK_COST = 0x40;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C, V_SET_CLIPPING = 0x31C;
constexpr uint32_t LWFNODE_LWF = 0x3E0;
constexpr uint32_t VIEW_FLAG_3B8 = 0x3B8;

// __func vtables (+8)
constexpr uint32_t VT_Equip = 0xF637B4;       // $_27 {keyblade id, scene}
constexpr uint32_t VT_EnterFrame = 0xF63F18;  // the medal LWF's "_root" enterFrame handler {view, lwf, area}
constexpr uint32_t VT_BlankLoad = 0xF63EEC;   // its "medal_blank_slot" load handler {lwf, glow}
constexpr uint32_t VT_CanClose = 0xF63788;    // $_31 bool() {ok}
constexpr uint32_t VT_Closed = 0xF63704;      // $_32 {ok, win, layer, shadow, popup, scene}

void setVisible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
uint32_t row(uint32_t keyblade) { return ld32(ld32(keyblade) + UKB_ROW); }
void shadowed(uint32_t text) { call(g::SceneUtil_setShadow, {text}); }

// a std::string at `s` from a short literal (as the compiler builds them in place)
void shortStr(uint32_t s, const char *text) {
    uint32_t n = uint32_t(std::strlen(text));
    st8(s, uint8_t(n * 2));
    for (uint32_t k = 0; k < n; k++) st8(s + 1 + k, uint8_t(text[k]));
    st8(s + 1 + n, 0);
}
// text->setString(StringBuffer(fmt, arg))
void setFormatted(uint32_t text, uint32_t fmt, uint32_t arg) {
    Frame f;
    uint32_t buf = stringBuffer(f, {fmt, arg});
    setTextC(text, buf);
}
// node centred in `parent`: position (parent size / 2)
void centreIn(uint32_t node, uint32_t parent) {
    float w = ldf(vcall(parent, node::getContentSize, {parent})) * 0.5f;
    float h = ldf(vcall(parent, node::getContentSize, {parent}) + 4) * 0.5f;
    Frame f;
    uint32_t v = f.alloc(8);
    call(g::Vec2_ctor, {v, fbits(w), fbits(h)});
    vcall(node, node::setPosition, {node, v});
    call(g::Vec2_dtor, {v});
}
// a magnification that grew: the label's parent a bit larger, and the result arrow in `arrowArea`
void magnificationUp(uint32_t label, uint32_t arrowArea) {
    call(g::SceneUtil_setParameterUpEffect, {label});
    uint32_t parent = vcall(label, node::getParent, {label});
    vcall(parent, node::setScale, {parent, fbits(1.2f)});
    vcall(arrowArea, node::addChild, {arrowArea, call(g::ResultArrow, {})});
}

struct DeckKeyBladePopup {
    static void open(uint32_t self, uint32_t keybladeId, uint32_t keyblade) {
        guest::Frame frame(FRAME);
        const uint32_t sp = frame.addr();
        uint32_t layer = call(g::SceneManager_getPopupLayer2, {sceneManager()});
        uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
        uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
        uint32_t panel = ld32(self + DE_PANEL);
        vcall(panel, V_SET_TOUCH_ENABLED, {panel, 0});
        uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_ResultJson});
        vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_MIDDLE});
        {
            float px = bitsf(vcall(popup, node::getPositionX, {popup}));
            float w = ldf(vcall(popup, node::getContentSize, {popup}));
            float margin = bitsf(call(g::SceneUtil_getSideMargin, {}));
            float py = bitsf(vcall(popup, node::getPositionY, {popup}));
            uint32_t size = vcall(popup, node::getContentSize, {popup});
            float x = margin + (px + w * 0.5f);
            float y0 = py + ldf(size + 4) * 0.5f;
            float y = bitsf(call(g::SceneUtil_getSideHeightMargin, {})) + y0;
            Frame f;
            uint32_t v = f.alloc(8);
            call(g::Vec2_ctor, {v, fbits(x), fbits(y)});
            vcall(popup, node::setPosition, {popup, v});
            call(g::Vec2_dtor, {v});
        }
        vcall(shadow, node::addChild, {shadow, popup});
        uint32_t win = findNode(popup, S_Win, 3);
        const int32_t id = safe32(row(keyblade), KB_ID);
        uint32_t point = findNode(popup, S_Area_Point, 10);
        vcall(point, node::addChild, {point, call(g::AudioLWFNode_create, {S_LevelUpLwf, 0})});
        uint32_t ok = findNode(popup, S_Button_OK, 9);
        {
            Frame f;
            uint32_t touch = newFunction(f, VT_Equip, {uint32_t(id), self});
            call(g::Widget_addTouchEventListener, {ok, touch});
            functionDestroy(touch);
        }
        {
            uint32_t text = findNode(ok, S_Txt_OK, 6);
            shadowed(text);
            Frame f;
            uint32_t td = textData(f, TXT_OK);
            setTextC(text, textDataStr(td));
            sharedRelease(td);
        }
        {
            uint32_t name = findNode(popup, S_KeyBlade_Name_Label, 19);
            shadowed(name);
            Frame f;
            setFormatted(name, S_FmtS, safeBytes(f, row(keyblade), KB_NAME, 0x41));
        }
        uint32_t tickerArea = findNode(popup, S_Area_Ticker_Ability, 19);
        uint32_t ticker = findNode(popup, S_Txt_Ticker_Ability, 18);
        shadowed(ticker);
        {
            Frame f;
            setFormatted(ticker, S_FmtS, safeBytes(f, row(keyblade), KB_ABILITY, 0x81));
        }
        vcall(tickerArea, V_SET_CLIPPING, {tickerArea, 1});
        {
            uint32_t areaW = ld32(vcall(tickerArea, node::getContentSize, {tickerArea}));
            float x = bitsf(vcall(ticker, node::getPositionX, {ticker}));
            float end = x + ldf(vcall(tickerArea, node::getContentSize, {tickerArea}));
            uint32_t x2 = vcall(ticker, node::getPositionX, {ticker});
            call(g::SceneUtil_runTicker, {ticker, areaW, fbits(end), x2});
        }
        uint32_t cost = findNode(popup, S_BurstCost, 9);
        shadowed(cost);
        {
            Frame f;
            uint32_t td = textData(f, TXT_BURST_COST);
            setTextC(cost, textDataStr(td));
            sharedRelease(td);
        }
        uint32_t allNum = findNode(cost, S_AllNum_Label, 12);
        shadowed(allNum);
        uint32_t num = findNode(cost, S_Num_Label, 9);
        shadowed(num);
        // the current deck's medal cost
        uint32_t slide = ld32(self + DE_SLIDE);
        uint32_t pages = slide + 0x578, page = ld32(slide + 0x5AC);
        if ((ld32(pages + 4) - ld32(pages)) / 4 <= page) call(g::ThrowOutOfRange, {pages});
        uint32_t deckIndex = ld32(ld32(pages) + 4 * page);
        uint32_t decks = self + DE_DECKS;
        if ((ld32(decks + 4) - ld32(decks)) / DECK_SIZE <= deckIndex) call(g::ThrowOutOfRange, {decks});
        int32_t deckCost = int32_t(ld32(ld32(decks) + DECK_SIZE * deckIndex + DECK_COST));
        setFormatted(allNum, S_FmtD, uint32_t(safe32(row(keyblade), KB_COST)));
        setFormatted(num, S_FmtDSlash, call(g::divsi3, {uint32_t(deckCost), 10000}));
        // the keyblade image
        uint32_t dummy = findNode(popup, S_KeyBlade_Dummy, 14);
        {
            uint32_t mgr = call(g::KeyBladeDataManager_getInstance, {});
            Frame f;
            uint32_t copy = f.alloc(8);
            st32(copy, ld32(keyblade));
            st32(copy + 4, ld32(keyblade + 4));
            if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
            uint32_t path = newStr(f, call(g::KeyBladeDataManager_getKeyBladeImagePath, {mgr, copy}));
            call(fn::ImageView_loadTexture, {dummy, path, 0});
            strFree(path);
            if (ld32(copy + 4)) call(fn::shared_release, {ld32(copy + 4)});
        }
        {
            uint32_t adm = call(g::ArmatureDataManager_getInstance, {});
            Frame f;
            uint32_t s = newStr(f, S_ArrowJson, 0x47);
            call(g::ArmatureDataManager_addArmatureFileInfo, {adm, s});
            strFree(s);
        }
        // the keyblade's data before the strengthen
        const uint32_t before = sp + 0x1FC;          // shared_ptr<UserKeyBladeData>
        {
            uint32_t mgr = call(g::KeyBladeDataManager_getInstance, {});
            uint32_t record = sp + 0x1050;
            *rw::frame_ptr<uint32_t>(sp + 0x10A0) = 0;
            call(g::aeabi_memset, {record, 0x14, 0});
            call(g::aeabi_memset, {record + 0x18, 0x35, 0});
            *rw::frame_ptr<uint32_t>(sp) = keybladeId;
            for (uint32_t k = 0; k < 0x58; k += 4) *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(record + k);
            rw::call_exact(g::KeyBladeDataManager_getKeyBladeData, {before, mgr, 0, 0});
        }
        const int32_t slotsBefore = safe32(row(before), KB_SLOTS);
        const int32_t costBefore = safe32(row(before), KB_COST);
        if (costBefore < safe32(row(keyblade), KB_COST)) {
            uint32_t arrow = findNode(popup, S_Area_Arrow, 10);
            vcall(arrow, node::addChild, {arrow, call(g::ResultArrow, {})});
            call(g::SceneUtil_setParameterUpEffect, {allNum});
        } else {
            call(g::SceneUtil_setNormalColor, {allNum});
        }
        // the revolver
        uint32_t revolver = findNode(popup, S_Revolver, 8);
        uint32_t centre = findNode(popup, S_Revolver_Center, 15);
        {
            int32_t style = safe32(row(keyblade), KB_STYLE);
            Frame f;
            uint32_t buf = stringBuffer(f, {ld32(ld32(g::GOT_CAROUSEL_NAME)), uint32_t(style)});
            uint32_t s = newStr(f, buf);
            call(fn::ImageView_loadTexture, {revolver, s, 0});
            strFree(s);
            Frame t;
            uint32_t buf2 = stringBuffer(t, {ld32(ld32(g::GOT_BUTTON_NAME)), uint32_t(style)});
            uint32_t s2 = newStr(t, buf2);
            call(fn::ImageView_loadTexture, {centre, s2, 0});
            strFree(s2);
        }
        // the levels at which each slot opens
        const uint32_t levels = sp + 0x1B4;          // vector<int>
        st32(levels, 0);
        st32(levels + 4, 0);
        st32(levels + 8, 0);
        for (int32_t k = 0; k <= 4; k++) {
            *rw::frame_ptr<uint32_t>(sp + 0x1B0) = 0;
            uint32_t end = ld32(levels + 4);
            if (end >= ld32(levels + 8)) {
                call(g::VectorInt_pushBackSlow, {levels, sp + 0x1B0});
            } else {
                if (end) st32(end, 0);
                st32(levels + 4, ld32(levels + 4) + 4);
            }
        }
        {
            uint32_t copy = sp + 0x1A8;
            st32(copy, ld32(keyblade));
            st32(copy + 4, ld32(keyblade + 4));
            if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
            call(g::DeckRevolverView_settingSlotOpenLevels, {copy, levels});
            if (ld32(copy + 4)) call(fn::shared_release, {ld32(copy + 4)});
        }
        const int32_t slots = safe32(row(keyblade), KB_SLOTS);
        bool glowShown = false;
        for (int32_t i = 0; i < 5; i++) {
            uint32_t area;
            {
                Frame f;
                uint32_t buf = stringBuffer(f, {S_Area_N, uint32_t(i + 1)});
                uint32_t s = newStr(f, buf);
                area = SceneUtil::searchNodeByName(revolver, s);
                strFree(s);
            }
            uint32_t view = call(g::DeckMedalView_create, {0});
            st8(view + VIEW_FLAG_3B8, 1);
            vcall(area, node::addChild, {area, view});
            uint32_t mag = findNode(view, S_Dummy_Mag, 9);
            uint32_t magLabel = findNode(mag, S_Txt_Mag_Label, 13);
            uint32_t mag2 = findNode(view, S_Dummy_Mag2, 10);
            uint32_t mag2Label = findNode(mag2, S_Txt_Mag_Label, 13);
            uint32_t arrow = findNode(view, S_Area_Arrow, 10);
            uint32_t arrow2 = findNode(view, S_Area_Arrow2, 11);
            const uint32_t level = ld32(ld32(levels) + 4 * i);
            const uint32_t medal = sp + 0x14C, detail = sp + 0x144, emptyMedal = sp + 0x134, emptyDetail = sp + 0x12C;
            if (i >= slots) {
                // a closed slot
                for (uint32_t k = 0; k < 8; k += 4) {
                    *rw::frame_ptr<uint32_t>(emptyMedal + k) = 0;
                    *rw::frame_ptr<uint32_t>(emptyDetail + k) = 0;
                }
                uint32_t *a = rw::frame_ptr<uint32_t>(sp);
                a[0] = 1; a[1] = 1000; a[2] = 1000; a[3] = 0; a[4] = level; a[5] = 0; a[6] = emptyDetail;
                rw::call_exact(g::DeckMedalView_setData, {view, emptyMedal, 0xFFFFFFFF, 1});
                if (ld32(emptyDetail + 4)) call(fn::shared_release, {ld32(emptyDetail + 4)});
                if (ld32(emptyMedal + 4)) call(fn::shared_release, {ld32(emptyMedal + 4)});
                continue;
            }
            Frame f;
            const int32_t attr = safe32(row(keyblade), KB_SLOT_ATTR + 4 * i);
            *rw::frame_ptr<uint32_t>(medal) = 0;
            *rw::frame_ptr<uint32_t>(medal + 4) = 0;
            const int32_t magnification = safe32(row(keyblade), KB_SLOT_MAG + 4 * i);
            const int32_t magnification2 = safe32(row(keyblade), KB_SLOT_MAG2 + 4 * i);
            const int32_t magnification3 = safe32(row(keyblade), KB_SLOT_MAG3 + 4 * i);
            *rw::frame_ptr<uint32_t>(detail) = 0;
            *rw::frame_ptr<uint32_t>(detail + 4) = 0;
            uint32_t *a = rw::frame_ptr<uint32_t>(sp);
            a[0] = uint32_t(attr); a[1] = uint32_t(magnification2); a[2] = uint32_t(magnification3); a[3] = 0;
            a[4] = level; a[5] = 0; a[6] = detail;
            rw::call_exact(g::DeckMedalView_setData, {view, medal, uint32_t(i), uint32_t(magnification)});
            if (ld32(detail + 4)) call(fn::shared_release, {ld32(detail + 4)});
            if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
            if (slotsBefore > i) {
                // a slot that was open: arrows where its magnifications grew
                if (safe32(row(before), KB_SLOT_MAG2 + 4 * i) < safe32(row(keyblade), KB_SLOT_MAG2 + 4 * i))
                    magnificationUp(magLabel, arrow);
                if (safe32(row(before), KB_SLOT_MAG3 + 4 * i) < safe32(row(keyblade), KB_SLOT_MAG3 + 4 * i))
                    magnificationUp(mag2Label, arrow2);
                continue;
            }
            // a slot opened by this level
            call(g::DeckMedalView_setIsShowDarklightMagnification, {view, 0});
            uint32_t lwfNode = call(g::AudioLWFNode_create, {S_MedalLwf, 0});
            centreIn(lwfNode, area);
            vcall(area, node::addChild, {area, lwfNode});
            uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
            {
                Frame t;
                uint32_t name = t.alloc(12);
                shortStr(name, "_root");
                w36::addMovieHandler(lwf, name, "enterFrame", VT_EnterFrame, {view, lwfNode, area});
                strFree(name);
            }
            uint32_t glow = uint32_t(attr - 1) < 3 ? ld32(MAGNIFICATION_GLOW + 4 * uint32_t(attr - 1)) : 0;
            {
                Frame t;
                uint32_t name = t.alloc(12);              // "medal_blank_slot" (16 chars: a heap string)
                uint32_t chars = call(fn::operator_new, {0x20});
                st32(name + 8, chars);
                st32(name, 0x21);
                st32(name + 4, 16);
                for (uint32_t k = 0; k < 16; k += 4) st32(chars + k, ld32(S_MedalBlankSlot + k));
                st8(chars + 16, 0);
                w36::addMovieHandler(ld32(lwfNode + LWFNODE_LWF), name, "load", VT_BlankLoad, {lwfNode, glow});
                strFree(name);
            }
            if (!glowShown) {
                uint32_t eff = call(g::AudioLWFNode_create, {S_EffLwf, 0});
                centreIn(eff, dummy);
                vcall(dummy, node::addChild, {dummy, eff});
                call(g::SceneUtil_setParameterUpEffect, {magLabel});
                if (attr != 3) call(g::SceneUtil_setParameterUpEffect, {mag2Label});
            }
            glowShown = true;
        }
        {
            Frame f;
            uint32_t opened = newEmptyFunction(f);
            call(fn::Node_runAction, {win, call(g::SceneUtil_createOpenPopupAnimation, {win, opened})});
            functionDestroy(opened);
        }
        uint32_t mgr = sceneManager();
        {
            Frame f;
            uint32_t canClose = newFunction(f, VT_CanClose, {ok});
            uint32_t closed = newEmptyFunction(f);
            uint32_t heap = call(fn::operator_new, {0x1C});
            if (heap) {
                st32(heap, VT_Closed);
                st32(heap + 4, ok);
                st32(heap + 8, win);
                st32(heap + 0xC, layer);
                st32(heap + 0x10, shadow);
                st32(heap + 0x14, popup);
                st32(heap + 0x18, self);
            }
            st32(closed + 16, heap);
            call(g::SceneManager_openPopup, {mgr, canClose, closed, 0});
            functionDestroy(closed);
            functionDestroy(canClose);
        }
        call(g::VectorBaseInt_dtor, {levels});
        if (ld32(before + 4)) call(fn::shared_release, {ld32(before + 4)});
    }
};

// ---- the opened slot's LWF handlers
constexpr uint32_t DeckMedalView_setIsShowDarklightMagnification = 0x52BFDD;
constexpr uint32_t SceneUtil_createFadeOutAnimation = 0x45BCE5;
constexpr uint32_t VT_GlowFaded = 0xF63F44;       // the fade's end {area, lwf}
constexpr uint32_t V_LWF_LOAD_MOVIE = 0x2F0;      // the LWF node's vslot 0x2F0: (shared_ptr out, node, path, slot, name, done)
constexpr uint32_t MOVIE_FRAMES = 0x98, MOVIE_FRAME = 0x9C;
constexpr uint32_t S_BlankSlotFmt = 0xE0450F;     // "lwf/keyblade/KeyEvo/medal_blank_slot%d/medal_blank_slot%d.lwf"
constexpr uint32_t S_MedalGlowLwf = 0xE0454D;     // "lwf/keyblade/KeyEvo/KeyEvo_Medal_02/KeyEvo_Medal_02.lwf"
constexpr uint32_t S_EmptyName = 0xDF8A1A;        // ""

// "medal_blank_slot" load {lwf node, slot}(slot index): the slot's blank LWF loaded into the node
void blankSlotLoad(uint32_t self, uint32_t slotp) {
    const uint32_t index = ld32(slotp);
    const uint32_t node = ld32(self + 4), slot = ld32(self + 8);
    const uint32_t load = ld32(ld32(node) + V_LWF_LOAD_MOVIE);
    Frame f;
    const uint32_t path = stringBuffer(f, {S_BlankSlotFmt, slot, slot});
    const uint32_t none = newEmptyFunction(f);
    const uint32_t out = f.alloc(8);
    call(load, {out, node, path, index, S_EmptyName, none});
    sharedRelease(out);
    functionDestroy(none);
}

// "_root" enterFrame {medal view, lwf, area}(movie): at the movie's last frame the medal view shows its darklight
// magnification and the LWF fades out (then the glow LWF)
void slotEnterFrame(uint32_t self, uint32_t moviep) {
    const uint32_t movie = ld32(moviep);
    if (ld32(movie + MOVIE_FRAME) != ld32(movie + MOVIE_FRAMES)) return;
    call(DeckMedalView_setIsShowDarklightMagnification, {ld32(self + 4), 1});
    const uint32_t lwf = ld32(self + 8);
    Frame f;
    const uint32_t done = newFunction(f, VT_GlowFaded, {ld32(self + 0xC), lwf});
    call(fn::Node_runAction, {lwf, call(SceneUtil_createFadeOutAnimation, {lwf, done})});
    functionDestroy(done);
}

// the fade's end {area, lwf}: the faded LWF removed, the medal glow LWF centred on the area's TYpe node
void glowFaded(uint32_t self) {
    const uint32_t area = ld32(self + 4);
    vcall(area, node::removeChild, {area, ld32(self + 8), 1});
    const uint32_t glow = call(g::AudioLWFNode_create, {S_MedalGlowLwf, 0});
    Frame f;
    const uint32_t name = f.alloc(12);        // the short string "TYpe", built inline
    st8(name, 4 << 1);
    st8(name + 1, 'T');
    st8(name + 2, 'Y');
    st8(name + 3, 'p');
    st8(name + 4, 'e');
    st8(name + 5, 0);
    const uint32_t type = SceneUtil::searchNodeByName(ld32(self + 4), name);
    strFree(name);
    const uint32_t setPosition = ld32(ld32(glow) + node::setPosition);
    const float w = ldf(vcall(type, node::getContentSize, {type}));
    const float h = ldf(vcall(type, node::getContentSize, {type}) + 4);
    const uint32_t pos = f.alloc(8);
    call(g::Vec2_ctor, {pos, fbits(w * 0.5f), fbits(h * 0.5f)});
    call(setPosition, {glow, pos});
    call(g::Vec2_dtor, {pos});
    vcall(type, node::addChild, {type, glow});
}
}  // namespace

RW_FUNCTION(0x665584, "SceneDeckEdit::openKeyBladePopup(int, shared_ptr<UserKeyBladeData>)", DeckKeyBladePopup::open);
RW_FUNCTION(0x67CB70, "SceneDeckEdit::openKeyBladePopup::$_29 medal_blank_slot load (sub_67CB70)", blankSlotLoad);
RW_FUNCTION(0x67CCF0, "SceneDeckEdit::openKeyBladePopup::$_29 _root enterFrame (sub_67CCF0)", slotEnterFrame);
RW_FUNCTION(0x67CE5C, "SceneDeckEdit::openKeyBladePopup::$_29 glow fade end (sub_67CE5C)", glowFaded);
