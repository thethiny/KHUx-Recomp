// StartDeckEditDialog: the lambda bodies (operator() of the $_N lambdas and their nested lambdas).
// Lambda arguments arrive by reference (std::function forwards them): a pointer to the value.
#include "StartDeckEditDialog_lib.hpp"

using namespace sded;

namespace {
// nested lambda vtables
constexpr uint32_t VT_open_2_retry = 0xF59D54;       // open::$_2::{lambda(bool)#1}
constexpr uint32_t VT_layout_7_costOver = 0xF59C4C;  // initLayout::$_7::{lambda(bool)#1}
constexpr uint32_t VT_layout_7_tut35 = 0xF59BF4;     // initLayout::$_7::{lambda()#1}
constexpr uint32_t VT_layout_7_tut107 = 0xF59C20;    // initLayout::$_7::{lambda()#1}::{lambda()#1}
constexpr uint32_t VT_layout_8_costOver = 0xF59B9C;  // initLayout::$_8::{lambda(bool)#1}
constexpr uint32_t VT_deck_9_touch = 0xF59B18;       // initDeckEdit::$_9::{lambda(Ref*, TouchEventType)#1}
constexpr uint32_t VT_deck_9_maxed = 0xF59B44;       // initDeckEdit::$_9::{lambda()#1}
constexpr uint32_t VT_support_15_moved = 0xF59A10;   // initSupport::$_15::{lambda()#1}
constexpr uint32_t VT_LAYER_FACTORY = 0xF7E130;      // __func<Layer*(*)(), ...> + 8
constexpr uint32_t createSceneLayer_SceneDeckEdit = 0x57AFCD;
constexpr uint32_t createSceneLayer_SceneRaidList = 0x57A735;
constexpr uint32_t createSceneLayer_SceneAdventureSelect = 0x579EB1;

constexpr uint32_t S_DECK_JSON = 0xDFFA0A;           // cocostudio/publish/StartDeckEdit2_Deck.json
constexpr uint32_t S_PANEL_KB_JSON = 0xDFFA36;       // cocostudio/publish/StartDeckEdit2_Panel_KB.json
constexpr uint32_t S_BUTTON_CHANGE = 0xDFF9F1, S_TXT_CHANGE = 0xDFF9FF, S_TXT_PRICE_LABEL = 0xDFFA66;
constexpr uint32_t S_TXT_MAXBURST = 0xDF8F15, S_TXT_MAXBURST_LABEL = 0xDF8F22, S_FMT_D_D = 0xDF8F35;

constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_executeRaidList = 0x4CC009;
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D;
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t AudioPlayGameMainSEOneShot = 0x5C1A79;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create;
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;
constexpr uint32_t Widget_isBright = rwlib::cocos2d::ui::Widget::isBright;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t FunctionWidgetInt_call = 0x54A065;    // std::function<Widget*(int)>::operator()
constexpr uint32_t FunctionVB_call = 0x474139;           // std::function<void(bool)>::operator()
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t Misc_create = 0x786D5D;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t StageUtil_setupRaidGlobalVariables = 0x764269;
constexpr uint32_t SceneRaidBattle_create = 0x57A425;
constexpr uint32_t SceneRaidBattle_setRaidStage = 0x608C6D;
constexpr uint32_t DIVSI3 = 0xCD5E8D;
constexpr uint32_t IN_OUT_TIME = 0xDEC3D0;               // SceneUtil::IN_OUT_TIME (float)
constexpr uint32_t VS_removeChild = 0x12C, VS_getPositionY = 0x70, VS_isVisible = 0xB4;

// Uninitialised words of the original frame passed to KeyBladeDataManager::getKeyBladeData: the padding word of
// the by-value UserKeyblade::Keyblade argument and the bytes of the struct the original never clears. Read at
// entry (frame = the original sp), before this rewrite's own calls can reach that stack area.
struct StaleKeyblade {
    uint32_t argPad, gap14, gap4C, gap54;
    StaleKeyblade(uint32_t frameSize, uint32_t structAt) {
        uint32_t sp = rw::cpu->r[13] - frameSize;
        argPad = rw::load<uint32_t>(sp + 4);
        gap14 = rw::load<uint32_t>(sp + structAt + 0x14);
        gap4C = rw::load<uint32_t>(sp + structAt + 0x4C);
        gap54 = rw::load<uint32_t>(sp + structAt + 0x54);
    }
};
// KeyBladeDataManager::getKeyBladeData(out, id, 0, UserKeyblade::Keyblade{}) as the original builds the argument
void getKeyBladeData(uint32_t out, uint32_t kdm, uint32_t lo, uint32_t hi, const StaleKeyblade &st) {
    uint32_t k[22] = {};
    k[5] = st.gap14;
    k[19] = st.gap4C & 0xFFFFFF00u;
    k[21] = st.gap54;
    call(KeyBladeDataManager_getKeyBladeData,
         {out, kdm, lo, hi, 0, st.argPad, k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9], k[10], k[11],
          k[12], k[13], k[14], k[15], k[16], k[17], k[18], k[19], k[20], k[21]});
}

// AppState::getTutorialCloseFunc(id) destroyed at once; true when there was one
bool takeTutorialCloseFunc(uint32_t id) {
    Frame f;
    uint32_t close = f.alloc(24);
    call(AppState_getTutorialCloseFunc, {close, call(AppState_getInstance, {}), id});
    uint32_t p = ld32(close + 16);
    if (!p) return false;
    if (p == close) vcall(p, 0x10, {p});
    else vcall(p, 0x14, {p});
    return true;
}
void execTutorialClose(uint32_t id) {
    call(AppState_execTutorialCloseFunc, {call(AppState_getInstance, {}), id});
}

// openCostOverDeckPopup(callback) (its `this` is never used: the callers leave the vtable address in r0)
void costOverPopup(uint32_t vtable, uint32_t self) {
    Frame f;
    uint32_t fo = inlineFn(f, vtable, {self});
    call(0x57818D, {vtable, fo});    // StartDeckEditDialog::openCostOverDeckPopup
    functionDestroy(fo);
}

// The keyblade's "max burst" price state: change button bright / enabled unless already at max
void deckChangeButton(uint32_t panel, uint32_t button, uint32_t kbData, uint32_t exp) {
    int32_t cost = scene::safe32(ld32(kbData + 0x20), 0xE0);
    int32_t level = int32_t(call(DIVSI3, {exp, 10000}));
    uint32_t enabled = level == cost ? 0 : 1;
    call(Widget_setBright, {button, enabled});
    uint32_t txt = find(panel, S_TXT_CHANGE, 10);
    call(SceneUtil_setButtonEnabled, {button, txt, level == cost ? 0u : 1u, 0});
}
}  // namespace

struct StartDeckEditDialogLambdas {
    // open::$_2 (bool ok): the support list arrived - build the dialog, or the error dialog
    static void supportListLoaded(uint32_t fn, uint32_t okRef) {
        uint8_t ok = rw::load<uint8_t>(okRef);
        uint32_t self = ld32(fn + 4);
        call(SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            StartDeckEditDialog::initLayout(self);
            return;
        }
        if (!rw::load<uint8_t>(api() + 0x14)) return;
        uint32_t a = api();
        Frame f;
        uint32_t fo = inlineFn(f, VT_open_2_retry, {});
        call(APIManager_openErrorDialog, {a, fo, 1});
        functionDestroy(fo);
    }

    // initLayout::$_3: the open animation is done - deck paging on, the pages built
    static void openAnimationDone(uint32_t fn) {
        uint32_t self = ld32(fn + 4);
        uint32_t slide = S(self)->deckSlide;
        vcall(slide, VS_slide31C, {slide, 1});
        call(0x525EB5, {S(self)->deckSlide, 1});   // BaseSlideView::updateLayout
    }

    // initLayout::$_4 {self, support Button_Close}: the dialog's close button
    static void closeTouched(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        uint32_t self = ld32(fn + 4);
        if (S(self)->supportOpen) {
            uint32_t sc = ld32(fn + 8);
            vcall(sc, VS_setTouchEnabled, {sc, 1});
            call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
            sc = ld32(fn + 8);
            vcall(sc, VS_setTouchEnabled, {sc, 0});
        }
        call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
    }

    // initLayout::$_6 (bool): the dialog popup is closed
    static void popupClosed(uint32_t fn, uint32_t okRef) {
        uint32_t self = ld32(fn + 4);
        if (rw::load<uint8_t>(okRef)) call(AudioPlayGameMainSEOneShot, {0x1E56B9FEu});
        else call(AudioPlaySystemSECancel, {});
        StartDeckEditDialog::close(self);
    }

    // initLayout::$_7 {self, support Button_Close}: OK - first opens the support list (tutorials 106 / 35 / 112),
    // then starts the quest (tutorials 108 / 112 closed); a deck over the cost limit opens the cost popup instead
    static void okTouched(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        uint32_t self = ld32(fn + 4);
        Self *s = S(self);
        if (s->closing) return;
        if (s->notColosseum) {
            call(AudioPlaySystemSEDecide, {});
            if (StartDeckEditDialog::isCostOverCurrentKeyblade(self) == 1) {
                costOverPopup(VT_layout_7_costOver, self);
                return;
            }
            StartDeckEditDialog::initSupport(self);
            StartDeckEditDialog::supportScrollRunInAnimation(self);
            s->notColosseum = 0;
            StartDeckEditDialog::updateDeckLayout(self);
            if (!takeTutorialCloseFunc(106)) return;
            execTutorialClose(106);
            {
                Frame f;
                uint32_t fo = inlineFn(f, VT_layout_7_tut35, {self});
                uint32_t popup = f.alloc(24);
                call(SceneUtil_openTutorialPopup, {popup, 35, fo, 0, 0, 0, 0});
                functionDestroy(popup);
                functionDestroy(fo);
            }
            Frame f;
            uint32_t empty = newEmptyFunction(f);
            StartDeckEditDialog::openTutorial112(self, empty);
            functionDestroy(empty);
            return;
        }
        if (StartDeckEditDialog::isCostOverCurrentKeyblade(self) == 1) {
            costOverPopup(VT_layout_7_costOver, self);
            return;
        }
        vcall(s->closeButton, VS_setTouchEnabled, {s->closeButton, 1});
        if (s->supportOpen) {
            uint32_t sc = ld32(fn + 8);
            vcall(sc, VS_setTouchEnabled, {sc, 1});
            call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
            sc = ld32(fn + 8);
            vcall(sc, VS_setTouchEnabled, {sc, 0});
        }
        call(SceneManager_closeCurrentPopup, {sceneManager(), 1});
        vcall(s->closeButton, VS_setTouchEnabled, {s->closeButton, 0});
        s->useSkipTicket = 0;
        StartDeckEditDialog::startAdventure(self);
        if (takeTutorialCloseFunc(108)) execTutorialClose(108);
        if (takeTutorialCloseFunc(112)) execTutorialClose(112);
    }

    // initLayout::$_7::{lambda()#1}: tutorial 35 closed - tutorial 107
    static void tutorial35Closed(uint32_t fn) {
        uint32_t self = ld32(fn + 4);
        Frame f;
        uint32_t fo = inlineFn(f, VT_layout_7_tut107, {self});
        StartDeckEditDialog::openTutorial107(self, fo);
        functionDestroy(fo);
    }

    // ...::{lambda()#1}: tutorial 107 closed - tutorial 108
    static void tutorial107Closed(uint32_t fn) {
        uint32_t self = ld32(fn + 4);
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        StartDeckEditDialog::openTutorial108(self, empty);
        functionDestroy(empty);
    }

    // initLayout::$_8 {self, support Button_Close}: skip ticket - start the quest with the ticket
    static void skipTouched(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        uint32_t self = ld32(fn + 4);
        Self *s = S(self);
        if (s->closing || s->notColosseum) return;
        if (StartDeckEditDialog::isCostOverCurrentKeyblade(self) == 1) {
            costOverPopup(VT_layout_8_costOver, self);
            return;
        }
        vcall(s->closeButton, VS_setTouchEnabled, {s->closeButton, 1});
        if (s->supportOpen) {
            uint32_t sc = ld32(fn + 8);
            vcall(sc, VS_setTouchEnabled, {sc, 1});
            call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
            sc = ld32(fn + 8);
            vcall(sc, VS_setTouchEnabled, {sc, 0});
        }
        call(SceneManager_closeCurrentPopup, {sceneManager(), 1});
        vcall(s->closeButton, VS_setTouchEnabled, {s->closeButton, 0});
        s->useSkipTicket = 1;
        StartDeckEditDialog::startAdventure(self);
    }

    // initDeckEdit::$_9 (int deck): a deck page - the deck layout and the keyblade panel with its max burst button
    static uint32_t makeDeckPage(uint32_t fn, uint32_t deckRef) {
        StaleKeyblade stale(0x5F0, 0x520);
        uint32_t deck = ld32(deckRef);
        uint32_t self = ld32(fn + 4);
        uint32_t page = call(Widget_create, {});
        uint32_t deckW = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_DECK_JSON});
        uint32_t panel = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_PANEL_KB_JSON});
        uint32_t base = find(deckW, S_BASE, 4);
        vcall(page, VS_addChild, {page, deckW});
        vcall(base, VS_addChild, {base, panel});
        uint32_t kb[22];
        safeWords(ld32(api() + API_KEYBLADES), deck * 0x58, kb, 22);
        uint32_t kdm = call(KeyBladeDataManager_getInstance, {});
        Frame f;
        uint32_t kbData = f.alloc(8);
        getKeyBladeData(kbData, kdm, kb[0], kb[1], stale);
        uint32_t button = find(panel, S_BUTTON_CHANGE, 13);
        deckChangeButton(panel, button, ld32(kbData), kb[16]);
        {
            Frame g;
            uint32_t fo = g.alloc(24);
            st32(fo + 16, 0);
            uint32_t heap = call(fn::operator_new, {0x14});
            if (heap) {
                st32(heap, VT_deck_9_touch);
                st32(heap + 4, button);
                st32(heap + 8, self);
                st32(heap + 0xC, self);
                st32(heap + 0x10, panel);
            }
            st32(fo + 16, heap);
            call(scene::fn::Widget_addTouchEventListener, {button, fo});
            functionDestroy(fo);
        }
        titleLabel(button, S_TXT_CHANGE, 10, 103000011);
        uint32_t price = find(panel, S_TXT_PRICE_LABEL, 15);
        call(SceneUtil_setShadow, {price});
        {
            Frame g;
            uint32_t td = textData(g, 103000015);
            uint32_t fmt = textDataStr(td);
            uint32_t misc = masterRow(g, Misc_create, 120);
            int32_t value = scene::safe32(ld32(misc), 4);
            uint32_t buf = stringBuffer(g, {fmt, uint32_t(value)});
            uint32_t str = newStr(g, buf);
            call(Text_setString, {price, str});
            strFree(str);
            release(ld32(misc + 4));
            release(ld32(td + 4));
        }
        release(ld32(kbData + 4));
        return page;
    }

    // initDeckEdit::$_9::{lambda(Ref*, TouchEventType)#1} {button, self, self, panel}: max burst - the popup
    static void changeTouched(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        uint32_t self = ld32(fn + 8);
        if (call(Widget_isBright, {ld32(fn + 4)}) != 1) return;
        if (S(self)->closing) return;
        call(AudioPlaySystemSEDecide, {});
        Frame f;
        uint32_t fo = inlineFn(f, VT_deck_9_maxed, {ld32(fn + 0xC), ld32(fn + 0x10)});
        call(0x577551, {self, fo});   // StartDeckEditDialog::openAbilityMaxPopup
        functionDestroy(fo);
    }

    // initDeckEdit::$_9::{lambda()#1} {self, panel}: the burst is maxed - refresh the panel
    static void burstMaxed(uint32_t fn) {
        StaleKeyblade stale(0x5B4, 0x4E0);
        uint32_t self = ld32(fn + 4);
        uint32_t a = api();
        uint32_t slide = S(self)->deckSlide;
        uint32_t ids = ld32(slide + 0x578), idsEnd = ld32(slide + 0x57C);
        uint32_t idx = ld32(slide + 0x5AC);
        if (!(uint32_t(int32_t(idsEnd - ids) >> 2) > idx)) {
            call(wl::VECTOR_AT_THROW, {slide + 0x578});
            return;
        }
        uint32_t kb[22];
        safeWords(ld32(a + API_KEYBLADES), ld32(ids + idx * 4) * 0x58, kb, 22);
        uint32_t kdm = call(KeyBladeDataManager_getInstance, {});
        Frame f;
        uint32_t kbData = f.alloc(8);
        getKeyBladeData(kbData, kdm, kb[0], kb[1], stale);
        uint32_t panel = ld32(fn + 8);
        uint32_t max = find(panel, S_TXT_MAXBURST, 12);
        call(SceneUtil_setShadow, {max});
        setTextId(max, 101900007);
        panel = ld32(fn + 8);
        uint32_t label = find(panel, S_TXT_MAXBURST_LABEL, 18);
        call(SceneUtil_setShadow, {label});
        {
            int32_t cost = scene::safe32(ld32(ld32(kbData) + 0x20), 0xE0);
            uint32_t level = call(DIVSI3, {kb[16], 10000});
            Frame g;
            uint32_t buf = stringBuffer(g, {S_FMT_D_D, level, uint32_t(cost)});
            uint32_t str = newStr(g, buf);
            call(Text_setString, {label, str});
            strFree(str);
        }
        panel = ld32(fn + 8);
        uint32_t button = find(panel, S_BUTTON_CHANGE, 13);
        call(Widget_setBright, {button, 0});
        panel = ld32(fn + 8);
        uint32_t txt = find(panel, S_TXT_CHANGE, 10);
        call(SceneUtil_setButtonEnabled, {button, txt, 0, 0});
        release(ld32(kbData + 4));
    }

    // initDeckEdit::$_10 (Widget* page, int deck, bool): reuse / create the page, update it and its button
    static uint32_t updateDeckPage(uint32_t fn, uint32_t pageRef, uint32_t deckRef, uint32_t) {
        StaleKeyblade stale(0x170, 0xA0);
        uint32_t deck = ld32(deckRef);
        uint32_t page = ld32(pageRef);
        uint32_t self = ld32(fn + 4);
        if (!page) {
            page = call(FunctionWidgetInt_call, {S(self)->deckSlide + 0x500, deck});
            call(Ref_retain, {page});
        }
        StartDeckEditDialog::updateDeckView(self, page, int32_t(deck), 0);
        uint32_t kb[22];
        safeWords(ld32(api() + API_KEYBLADES), deck * 0x58, kb, 22);
        uint32_t kdm = call(KeyBladeDataManager_getInstance, {});
        Frame f;
        uint32_t kbData = f.alloc(8);
        getKeyBladeData(kbData, kdm, kb[0], kb[1], stale);
        uint32_t button = find(page, S_BUTTON_CHANGE, 13);
        if (button) deckChangeButton(page, button, ld32(kbData), kb[16]);
        release(ld32(kbData + 4));
        return page;
    }

    // initDeckEdit::$_11 / $_12: the pager arrows
    static void prevTouched(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 0) return;
        uint32_t self = ld32(fn + 4);
        uint32_t left = S(self)->leftUI;
        if (vcall(left, VS_isVisible, {left}) != 1) return;
        call(AudioPlaySystemSEDecide, {});
        call(0x5263ED, {S(self)->deckSlide});   // BaseSlideView::changePreviousPage
    }
    static void nextTouched(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 0) return;
        uint32_t self = ld32(fn + 4);
        uint32_t right = S(self)->rightUI;
        if (vcall(right, VS_isVisible, {right}) != 1) return;
        call(AudioPlaySystemSEDecide, {});
        call(0x526245, {S(self)->deckSlide});   // BaseSlideView::changeNextPage
    }

    // initSupport::$_15 {plus child, its x, self, self} (int index): a support picked - it becomes the support,
    // the "Plus" panel slides right and comes back with it ({lambda()#1}); the tutorial 107 close func runs
    static void supportPicked(uint32_t fn, uint32_t indexRef) {
        uint32_t index = ld32(indexRef);
        uint32_t self2 = ld32(fn + 0x10);
        call(AudioPlaySystemSEDecide, {});
        uint32_t id[2];
        safeWords(ld32(api() + API_SUPPORTS), index * 0x128, id, 2);
        uint32_t sm = sceneManager();
        st32(sm + SM_SUPPORT_ID + 4, id[1]);
        st32(sm + SM_SUPPORT_ID, id[0]);
        call(Node_stopAllActions, {ld32(fn + 4)});
        uint32_t node = ld32(fn + 4);
        uint32_t x = ld32(fn + 8);
        uint32_t cap = ld32(fn + 0xC);
        float nx = bf(x) + ldf(vcall(node, VS_getContentSize, {node}));
        uint32_t n2 = ld32(fn + 4);
        float y = bf(vcall(n2, VS_getPositionY, {n2}));
        {
            Frame f;
            uint32_t pos = f.alloc(8);
            call(wl::Vec2_ctorXY, {pos, fb(nx), fb(y)});
            uint32_t move = call(wl::MoveTo_create, {fb(0.1f), pos});
            uint32_t fo = f.alloc(24);
            st32(fo + 16, 0);
            uint32_t heap = call(fn::operator_new, {0x14});
            if (heap) {
                st32(heap, VT_support_15_moved);
                st32(heap + 4, node);
                st32(heap + 8, x);
                st32(heap + 0xC, cap);
                st32(heap + 0x10, index);
            }
            st32(fo + 16, heap);
            uint32_t cf = call(wl::CallFunc_create, {fo});
            runSequence2(node, move, cf);
            functionDestroy(fo);
            call(wl::Vec2_dtor, {pos});
        }
        if (!takeTutorialCloseFunc(107)) return;
        if (!rw::load<uint8_t>(ld32(self2 + 0x2C0) + 0x684)) execTutorialClose(107);
    }

    // initSupport::$_15::{lambda()#1} {node, x, self, index}: slide back, show the picked support
    static void supportPanelBack(uint32_t fn) {
        uint32_t node = ld32(fn + 4);
        uint32_t x = ld32(fn + 8);
        float y = bf(vcall(node, VS_getPositionY, {node}));
        {
            Frame f;
            uint32_t pos = f.alloc(8);
            call(wl::Vec2_ctorXY, {pos, x, fb(y)});
            call(wl::Node_runAction, {node, call(wl::MoveTo_create, {fb(0.1f), pos})});
            call(wl::Vec2_dtor, {pos});
        }
        StartDeckEditDialog::showSupportUser(fn + 0xC, int32_t(ld32(fn + 0x10)));
    }

    // initSupport::$_16 (int): the list was scrolled - the tutorial 107 close func runs
    static void supportScrolled(uint32_t fn, uint32_t) {
        uint32_t self = ld32(fn + 4);
        if (!takeTutorialCloseFunc(107)) return;
        if (!rw::load<uint8_t>(ld32(self + 0x2C0) + 0x684)) execTutorialClose(107);
    }

    // initSupport::$_17 {self, support Button_Close}: back to the deck page (after the tutorial)
    static void supportCloseTouched(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        uint32_t self = ld32(fn + 4);
        if (int32_t(call(SceneUtil_getNexTutorialphase, {})) < 995) return;
        call(AudioPlaySystemSECancel, {});
        Self *s = S(self);
        uint32_t panel = find(ld32(s->deckSlide + 0x588), S_PANEL_KB, 8);
        vcall(panel, VS_setVisible, {panel, 1});
        call(BaseSlideView_setSlideEnabled, {s->deckSlide, 1});
        vcall(s->leftUI, VS_setVisible, {s->leftUI, 1});
        vcall(s->rightUI, VS_setVisible, {s->rightUI, 1});
        call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
        uint32_t close = ld32(fn + 8);
        vcall(close, VS_setTouchEnabled, {close, 0});
        s->notColosseum = s->stageType == 2 ? 0 : 1;
        StartDeckEditDialog::updateDeckLayout(self);
    }

    // close::$_18: the close animation is done - take the dialog off; to the deck edit scene when asked;
    // the open callback (started) and fn_280
    static void closed(uint32_t fn) {
        uint32_t self = ld32(fn + 4);
        Self *s = S(self);
        if (!s->parent) return;
        call(Ref_retain, {self});
        call(rwlib::cocos2d::Ref::autorelease, {self});   // Ref::autorelease
        uint32_t p = s->parent;
        vcall(p, VS_removeChild, {p, s->shadow, 1});
        s->parent = 0;
        if (s->field_2D0) {
            uint32_t sm = sceneManager();
            Frame f;
            uint32_t fo = inlineFn(f, VT_LAYER_FACTORY, {createSceneLayer_SceneDeckEdit});
            call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
            functionDestroy(fo);
        }
        if (rw::get<uint32_t>(s, 0x2A8)) call(FunctionVB_call, {self + 0x298, s->started});
        if (rw::get<uint32_t>(s, 0x290)) call(wl::FunctionVV_call, {self + 0x280});
    }

    // supportScrollRunOutAnimation::$_21 {support, Vec2 pos, self}: hidden and put back, the list removed
    static void supportOut(uint32_t fn) {
        uint32_t node = ld32(fn + 4);
        uint32_t self = ld32(fn + 0x10);
        vcall(node, VS_setVisible, {node, 0});
        node = ld32(fn + 4);
        vcall(node, VS_setPosition, {node, fn + 8});
        uint32_t scroll = ld32(self + 0x2C0);
        vcall(scroll, 0x124, {scroll});   // removeFromParent
        st32(self + 0x2C0, 0);
    }

    // startRaid::$_25 (bool): the raid request finished - back to the raid list or the adventure select
    static void raidStartResult(uint32_t, uint32_t) {
        if (call(APIManager_needsRetry, {api()})) return;
        if (ld32(sceneManager() + 0x28C) == 8) {
            {
                uint32_t a = api();
                Frame f;
                uint32_t empty = newEmptyFunction(f);
                call(APIManager_executeRaidList, {a, empty});
                functionDestroy(empty);
            }
            uint32_t sm = sceneManager();
            Frame f;
            uint32_t fo = inlineFn(f, VT_LAYER_FACTORY, {createSceneLayer_SceneRaidList});
            call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
            functionDestroy(fo);
            return;
        }
        {
            Frame f;
            uint32_t empty = newEmptyFunction(f);
            call(SceneAdventureSelect_callAdventureSelectAPI, {empty});
            functionDestroy(empty);
        }
        {
            uint32_t sm = sceneManager();
            Frame f;
            uint32_t fo = inlineFn(f, VT_LAYER_FACTORY, {createSceneLayer_SceneAdventureSelect});
            call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
            functionDestroy(fo);
        }
        st8(sceneManager() + 0x30A, 1);
    }

    // startRaid::$_26: the raid battle scene layer (mode 6 for the player's own raid, else 8)
    static uint32_t raidBattleLayer(uint32_t) {
        uint32_t a = api();
        uint32_t a2 = api();
        uint32_t raid = ld32(a + 0x178);
        uint32_t lo = ld32(raid + 0x60), hi = ld32(raid + 0x64);
        uint32_t own[2];
        safeWords(ld32(a2 + 0x74), 0, own, 2);
        uint32_t mode = ((own[0] ^ lo) | (own[1] ^ hi)) ? 8 : 6;
        st32(sceneManager() + 0x28C, mode);
        Frame f;
        uint32_t info = f.alloc(0xA8);
        uint32_t sb = ld32(a + 0x178);
        call(SafeBuffer_read, {sb, info, ld32(sb + 0xC), 0xA8, 0});
        call(StageUtil_setupRaidGlobalVariables, {info});
        uint32_t layer = call(SceneRaidBattle_create, {});
        uint32_t v2E4 = ld32(sceneManager() + 0x2E4);
        uint32_t v2E8 = ld32(sceneManager() + 0x2E8);
        uint32_t v2EC = rw::load<uint8_t>(sceneManager() + 0x2EC);
        call(SceneRaidBattle_setRaidStage, {layer, v2E4, v2E8, v2EC, 0, 0, 0});
        return layer;
    }

    // openCostOverDeckPopup::$_35 (bool) {shadow, popup, layer, done}: close the popup, then done(flag)
    static void costOverClose(uint32_t cap, uint32_t flag) {
        uint32_t shadow = ld32(cap);
        call(wl::Node_runAction, {shadow, call(FadeTo_create, {ld32(IN_OUT_TIME), 0})});
        uint32_t popup = ld32(cap + 4);
        Frame f;
        uint32_t tmp = f.alloc(0x2C);           // {layer, shadow, popup, function<void(bool)>, bool}
        st32(tmp, ld32(cap + 8));
        st32(tmp + 4, ld32(cap));
        st32(tmp + 8, popup);
        call(scene::fn::function_vb_copy, {tmp + 0x10, cap + 0x10});
        st8(tmp + 0x28, uint8_t(flag));
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x38});
        if (heap) {
            st32(heap, 0xF59598);   // $_35::{lambda()#1}
            st32(heap + 8, ld32(tmp));
            st32(heap + 0xC, ld32(tmp + 4));
            st32(heap + 0x10, ld32(tmp + 8));
            functionCopy(heap + 0x18, tmp + 0x10);
            st8(heap + 0x30, rw::load<uint8_t>(tmp + 0x28));
        }
        st32(fo + 16, heap);
        uint32_t anim = call(SceneUtil_createClosePopupAnimation, {popup, fo});
        call(wl::Node_runAction, {popup, anim});
        functionDestroy(fo);
        functionDestroy(tmp + 0x10);
    }

    // $_35::{lambda()#1}: shadow and popup off, done(flag)
    static void costOverClosed(uint32_t fn) {
        uint32_t layer = ld32(fn + 8);
        vcall(layer, VS_removeChild, {layer, ld32(fn + 0xC), 1});
        layer = ld32(fn + 8);
        vcall(layer, VS_removeChild, {layer, ld32(fn + 0x10), 1});
        call(FunctionVB_call, {fn + 0x18, rw::load<uint8_t>(fn + 0x30)});
    }

    // $_36 (equip) / $_37 (close) {equip, close, $_35 captures}
    static void costOverEquip(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        uint32_t b = ld32(fn + 8);
        vcall(b, VS_setTouchEnabled, {b, 0});
        b = ld32(fn + 0xC);
        vcall(b, VS_setTouchEnabled, {b, 0});
        call(AudioPlaySystemSEDecide, {});
        costOverClose(fn + 0x10, 1);
    }
    static void costOverCancel(uint32_t fn, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        uint32_t b = ld32(fn + 8);
        vcall(b, VS_setTouchEnabled, {b, 0});
        b = ld32(fn + 0xC);
        vcall(b, VS_setTouchEnabled, {b, 0});
        call(AudioPlaySystemSECancel, {});
        costOverClose(fn + 0x10, 0);
    }
};

using L = StartDeckEditDialogLambdas;
RW_FUNCTION(0x57D694, "StartDeckEditDialog::open::$_2::operator()", L::supportListLoaded);
RW_FUNCTION(0x57D5DA, "StartDeckEditDialog::initLayout::$_3::operator()", L::openAnimationDone);
RW_FUNCTION(0x57D4F8, "StartDeckEditDialog::initLayout::$_4::operator()", L::closeTouched);
RW_FUNCTION(0x57D394, "StartDeckEditDialog::initLayout::$_6::operator()", L::popupClosed);
RW_FUNCTION(0x57CD24, "StartDeckEditDialog::initLayout::$_7::operator()", L::okTouched);
RW_FUNCTION(0x57D0A8, "StartDeckEditDialog::initLayout::$_7::{lambda()#1}::operator()", L::tutorial35Closed);
RW_FUNCTION(0x57D1D0, "StartDeckEditDialog::initLayout::$_7::{lambda()#1}::{lambda()#1}::operator()", L::tutorial107Closed);
RW_FUNCTION(0x57CA94, "StartDeckEditDialog::initLayout::$_8::operator()", L::skipTouched);
RW_FUNCTION(0x57BF00, "StartDeckEditDialog::initDeckEdit::$_9::operator()", L::makeDeckPage);
RW_FUNCTION(0x57C48C, "StartDeckEditDialog::initDeckEdit::$_9::{lambda(Ref*,TouchEventType)#1}::operator()", L::changeTouched);
RW_FUNCTION(0x57C5F4, "StartDeckEditDialog::initDeckEdit::$_9::{lambda()#1}::operator()", L::burstMaxed);
RW_FUNCTION(0x57BC40, "StartDeckEditDialog::initDeckEdit::$_10::operator()", L::updateDeckPage);
RW_FUNCTION(0x57BB7E, "StartDeckEditDialog::initDeckEdit::$_11::operator()", L::prevTouched);
RW_FUNCTION(0x57BABE, "StartDeckEditDialog::initDeckEdit::$_12::operator()", L::nextTouched);
RW_FUNCTION(0x57B65C, "StartDeckEditDialog::initSupport::$_15::operator()", L::supportPicked);
RW_FUNCTION(0x57B8E4, "StartDeckEditDialog::initSupport::$_15::{lambda()#1}::operator()", L::supportPanelBack);
RW_FUNCTION(0x57B530, "StartDeckEditDialog::initSupport::$_16::operator()", L::supportScrolled);
RW_FUNCTION(0x57B350, "StartDeckEditDialog::initSupport::$_17::operator()", L::supportCloseTouched);
RW_FUNCTION(0x57AE74, "StartDeckEditDialog::close::$_18::operator()", L::closed);
RW_FUNCTION(0x57AC5C, "StartDeckEditDialog::supportScrollRunOutAnimation::$_21::operator()", L::supportOut);
RW_FUNCTION(0x57A568, "StartDeckEditDialog::startRaid::$_25::operator()", L::raidStartResult);
RW_FUNCTION(0x57A2F0, "StartDeckEditDialog::startRaid::$_26::operator()", L::raidBattleLayer);
RW_FUNCTION(0x5789B4, "StartDeckEditDialog::openCostOverDeckPopup::$_35::operator()", L::costOverClose);
RW_FUNCTION(0x578CC4, "StartDeckEditDialog::openCostOverDeckPopup::$_35::{lambda()#1}::operator()", L::costOverClosed);
RW_FUNCTION(0x578EA8, "StartDeckEditDialog::openCostOverDeckPopup::$_36::operator()", L::costOverEquip);
RW_FUNCTION(0x57893C, "StartDeckEditDialog::openCostOverDeckPopup::$_37::operator()", L::costOverCancel);
