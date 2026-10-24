// Equipment > Keyblade: lambdas of SceneDeckEdit::openKeyBladePopup. $_27 (Equip released): the keyblade's medal
// slot count; the current deck grows to it with empty medal slots (and the medal view gets an empty medal per new
// slot), then the deck data, the slide view, the popup and the keyblade panel are refreshed. $_32 (the popup
// closed after a strengthen): close animation, the shadow fades, and the user's new title popup; its UlvE_ removes
// the popup and re-enables the panel (and advances the keyblade tutorial). sub_666F84 builds the result arrow
// animation. The __clone thunks copy their captured words.
#include "../scene/SceneUi.hpp"
#include "../scene_main/FunctionThunks.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D;
constexpr uint32_t KeyBladeDataManager_getKeyBladeData = 0x76B659;   // (sret, mgr, u64, unsigned, Keyblade by value)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;         // (sret, mgr, u64, int, Medal by value)
constexpr uint32_t MedalVector_pushBackSlow = 0x487A39;     // vector<shared_ptr<UserMedalData>>::__push_back_slow_path
constexpr uint32_t SceneDeckEdit_updateDeckData = 0x667EF9, SceneDeckEdit_updateKeyBladePanel = 0x663ED9;
constexpr uint32_t BaseSlideView_updateLayout = 0x525EB5;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneManager_ChainOfTutorialAdventureSelect = 0x6219E1;
constexpr uint32_t AppState_addClearTutorialPopupIDs = 0x4EB7A9;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_openUserTitlePopup = 0x470725;   // (const shared_ptr<UserTitle>&, int, function<void()>)
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
}  // namespace g

constexpr uint32_t TOUCH_ENDED = 2;
// SceneDeckEdit
constexpr uint32_t DE_DECKS = 0x324;        // vector<deck> (0x58 bytes: +0x10 slot count, +0x18 medal ids)
constexpr uint32_t DE_MEDAL_VIEW = 0x380;   // +0x2C0 vector<shared_ptr<UserMedalData>> (one per slot)
constexpr uint32_t DE_SLIDE = 0x38C;        // BaseSlideView: +0x578 vector<int> deck indices, +0x5AC current page
constexpr uint32_t DE_PANEL = 0x314;
constexpr uint32_t DECK_SIZE = 0x58, DECK_SLOTS = 0x10, DECK_MEDALS = 0x18;
constexpr uint32_t KB_ROW = 0x20, KB_SLOTS = 0xDC;   // UserKeyBladeData: master row (SafeBuffer), medal slots
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C;
constexpr uint32_t API_NEW_TITLE = 0x188;   // shared_ptr<UserTitle>
constexpr int32_t TUTORIAL_KEYBLADE = 500, TUTORIAL_ID = 42;

constexpr uint32_t VT_ClosedRemove = 0xF6375C;   // $_100 UlvE_ {parent, shadow, popup, scene}
constexpr uint32_t VT_TitleClosed = 0xF63730;    // $_32's title popup lambda {scene}
constexpr uint32_t S_ResultArrow = 0xE04585;     // "DeckEdit_KBStrength_ResultArrow_Animation"
constexpr uint32_t S_UP_Anime = 0xE045AF;

struct DeckKeyBladeLambdas {
    // $_27 (Ref*, TouchEventType) {keyblade id, scene}
    static void equip(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        uint32_t scene = ld32(functor + 8);
        call(g::AudioPlaySystemSEDecide, {});
        uint32_t kbm = call(g::KeyBladeDataManager_getInstance, {});
        uint32_t keybladeId = ld32(functor + 4);
        // the original frame: both data managers take their record by value from fixed stack positions, records
        // whose unset bytes are stale stack
        guest::Frame frame(0x168);
        uint32_t sp = frame.addr();
        uint32_t record = sp + 0xF0;
        *rw::frame_ptr<uint32_t>(sp + 0x140) = 0;
        call(g::aeabi_memset, {record, 0x14, 0});
        call(g::aeabi_memset, {record + 0x18, 0x35, 0});
        *rw::frame_ptr<uint32_t>(sp) = keybladeId;
        for (uint32_t k = 0; k < 0x58; k += 4) *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(record + k);
        uint32_t kb = sp + 0x9C;     // shared_ptr<UserKeyBladeData>
        rw::call_exact(g::KeyBladeDataManager_getKeyBladeData, {kb, kbm, 0, 0});
        uint32_t slide = ld32(scene + DE_SLIDE);
        uint32_t pages = slide + 0x578;
        uint32_t page = ld32(slide + 0x5AC);
        if ((ld32(pages + 4) - ld32(pages)) / 4 <= page) call(g::ThrowOutOfRange, {pages});
        uint32_t deckIndex = ld32(ld32(pages) + 4 * page);
        uint32_t decks = scene + DE_DECKS;
        if ((ld32(decks + 4) - ld32(decks)) / DECK_SIZE <= deckIndex) call(g::ThrowOutOfRange, {decks});
        uint32_t slots = safe32(ld32(ld32(kb) + KB_ROW), KB_SLOTS);
        uint32_t view = ld32(scene + DE_MEDAL_VIEW);
        uint32_t medals = view + 0x2C0;
        uint32_t deck = ld32(decks) + DECK_SIZE * deckIndex;
        uint32_t old = ld32(deck + DECK_SLOTS);
        uint32_t added = slots - old;
        if (slots != old) {
            for (uint32_t i = 0; i < added; i++) {
                uint32_t at = deck + 8 * (old + i);
                st32(at + DECK_MEDALS, 0);
                st32(at + DECK_MEDALS + 4, 0);
                uint32_t mm = call(g::MedalDataManager_getInstance, {});
                uint32_t mrec = sp + 0xA8;
                call(g::aeabi_memset, {mrec, 0x2C, 0});
                call(g::aeabi_memset, {mrec + 0x30, 0x18, 0});
                *rw::frame_ptr<uint32_t>(sp) = 0;
                for (uint32_t k = 0; k < 0x48; k += 4) *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(mrec + k);
                uint32_t medal = sp + 0x94;
                rw::call_exact(g::MedalDataManager_getMedalData, {medal, mm, 0, 0});
                uint32_t end = ld32(medals + 4);
                if (end == ld32(medals + 8)) {
                    call(g::MedalVector_pushBackSlow, {medals, medal});
                } else {
                    if (end) {
                        st32(end, ld32(medal));
                        st32(end + 4, ld32(medal + 4));
                        if (ld32(medal + 4)) call(fn::shared_add, {ld32(medal + 4)});
                    }
                    st32(medals + 4, ld32(medals + 4) + 8);
                }
                if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
            }
            st32(deck + DECK_SLOTS, old + added);
        }
        call(g::SceneDeckEdit_updateDeckData, {scene});
        call(g::BaseSlideView_updateLayout, {ld32(scene + DE_SLIDE), 1});
        call(g::SceneManager_closeCurrentPopup, {sceneManager(), 1});
        call(g::SceneDeckEdit_updateKeyBladePanel, {scene});
        if (ld32(kb + 4)) call(fn::shared_release, {ld32(kb + 4)});
    }

    // $_32 {panel, popup, parent, shadow, popupRoot, scene}
    static void closed(uint32_t functor) {
        uint32_t panel = ld32(functor + 4);
        vcall(panel, V_SET_TOUCH_ENABLED, {panel, 0});
        uint32_t popup = ld32(functor + 8);
        uint32_t scene = ld32(functor + 0x18);
        {
            Frame f;
            uint32_t remove = newEmptyFunction(f);
            uint32_t heap = call(fn::operator_new, {0x14});
            if (heap) {
                st32(heap, VT_ClosedRemove);
                st32(heap + 4, ld32(functor + 0xC));
                st32(heap + 8, ld32(functor + 0x10));
                st32(heap + 0xC, ld32(functor + 0x14));
                st32(heap + 0x10, scene);
            }
            st32(remove + 16, heap);
            call(fn::Node_runAction, {popup, call(g::SceneUtil_createClosePopupAnimation, {popup, remove})});
            functionDestroy(remove);
        }
        uint32_t shadow = ld32(functor + 0x10);
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), 0})});
        uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        uint32_t titleClosed = newFunction(f, VT_TitleClosed, {scene});
        call(g::SceneUtil_openUserTitlePopup, {api + API_NEW_TITLE, 0, titleClosed});
        functionDestroy(titleClosed);
    }

    // $_100 UlvE_ {parent, shadow, popup, scene}
    static void removePopup(uint32_t functor) {
        uint32_t parent = ld32(functor + 4);
        uint32_t scene = ld32(functor + 0x10);
        vcall(parent, node::removeChild, {parent, ld32(functor + 8), 1});
        parent = ld32(functor + 4);
        vcall(parent, node::removeChild, {parent, ld32(functor + 0xC), 1});
        uint32_t panel = ld32(scene + DE_PANEL);
        vcall(panel, V_SET_TOUCH_ENABLED, {panel, 1});
        if (int32_t(call(g::SceneUtil_getNexTutorialphase, {})) != TUTORIAL_KEYBLADE) return;
        call(g::SceneManager_ChainOfTutorialAdventureSelect, {sceneManager(), TUTORIAL_ID});
        call(g::AppState_addClearTutorialPopupIDs, {call(fn::AppState_getInstance, {}), TUTORIAL_ID});
    }

    // sub_666F84: the strengthen result arrow, playing UP_Anime
    static uint32_t resultArrow() {
        uint32_t armature;
        {
            Frame f;
            uint32_t name = newStr(f, S_ResultArrow, 0x29);
            armature = call(g::Armature_create, {name});
            strFree(name);
        }
        uint32_t anim = vcall(armature, 0x298, {armature});
        Frame f;
        uint32_t name = newStr(f, S_UP_Anime, 8);
        vcall(anim, 0x68, {anim, name, 1, 1});
        strFree(name);
        return armature;
    }
};

using thunk::cap;
using Clone32 = thunk::Lambda<0xF63704, 0x1C, 0, 0, 4, cap(4, 4), cap(8, 4), cap(0xC, 4), cap(0x10, 4), cap(0x14, 4),
                              cap(0x18, 4)>;
using Clone100 = thunk::Lambda<VT_ClosedRemove, 0x14, 0, 0, 4, cap(4, 4), cap(8, 4), cap(0xC, 4), cap(0x10, 4)>;
using Clone27 = thunk::Lambda<0xF637B4, 0xC, 0, 0, 4, cap(4, 4), cap(8, 4)>;
}  // namespace

RW_FUNCTION(0x67853C, "SceneDeckEdit::openKeyBladePopup $_27 __func::operator() (sub_67853C)", DeckKeyBladeLambdas::equip);
RW_FUNCTION(0x6780F0, "SceneDeckEdit::openKeyBladePopup $_32 __func::operator() (sub_6780F0)", DeckKeyBladeLambdas::closed);
RW_FUNCTION(0x67839A, "SceneDeckEdit::openKeyBladePopup $_100 UlvE_ __func::operator() (sub_67839A)",
            DeckKeyBladeLambdas::removePopup);
RW_FUNCTION(0x666F84, "SceneDeckEdit strengthen result arrow (sub_666F84)", DeckKeyBladeLambdas::resultArrow);
RW_FUNCTION(0x678080, "SceneDeckEdit::openKeyBladePopup $_32 __func::__clone() (sub_678080)", Clone32::clone);
RW_FUNCTION(0x678330, "SceneDeckEdit::openKeyBladePopup $_100 UlvE_ __func::__clone() (sub_678330)", Clone100::clone);
RW_FUNCTION(0x67850C, "SceneDeckEdit::openKeyBladePopup $_27 __func::__clone(__base*) (sub_67850C)", Clone27::cloneInto);
