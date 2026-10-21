// SceneDeckEdit, reached by the deck edit driver (a medal info popup's buttons, a medal dropped on a deck slot):
// onTapMedalInfoButton and its Level Up lambda $_36, updateDeckView's $_33 (a deck medal's info buttons) and $_34.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t IInOut_runOutAnimation = 0x6F24B5;
constexpr uint32_t IInOut_canRunOut = 0x51022D;            // std::function<bool()>::operator()
constexpr uint32_t MiniMedal_setDisableMedalID = 0x4FD425;  // (mini, u64 id, bool disable, bool relayout)
constexpr uint32_t SM_ChainOfTutorialAdventureSelect = 0x6219E1;
constexpr uint32_t AppState_addClearTutorialPopupIDs = 0x4EB7A9;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;      // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t VT_ToLevelUp = 0xF63628;   // onTapMedalInfoButton $_36 {this} (button type 0)
constexpr uint32_t VT_ToEvolve = 0xF635D0;    // onTapMedalInfoButton $_37 {} (button type 1)
constexpr uint32_t VT_LevelUpLayer = 0xF63654;   // $_36's layer factory {}
constexpr uint32_t TUTORIAL_DECK_DROP = 37;
constexpr uint32_t SceneDeckEdit_onTapMedalInfoButton = 0x667DC1;
// SceneDeckEdit
constexpr uint32_t F_INOUT = 0x288, F_MINI = 0x390, F_LEAVING = 0x3A0, F_BACK_ANIM = 0x3A8;
// SceneManager: the medal the next screen opens on
constexpr uint32_t SM_NEXT_MEDAL = 0x2A0;
}  // namespace

namespace deck_edit_leftovers {

// a medal info popup button: type 0 / 1 remember the medal and leave the screen (its lambda opens the next one);
// any other type does nothing here
uint32_t onTapMedalInfoButton(uint32_t self, uint32_t type, uint32_t, uint32_t, uint32_t idLo, uint32_t idHi) {
    if (type != 0 && type != 1) return 0;
    const uint32_t sm = sceneManager();
    st32(sm + SM_NEXT_MEDAL + 4, idHi);
    st32(sm + SM_NEXT_MEDAL, idLo);
    if (call(IInOut_canRunOut, {self + F_BACK_ANIM}) != 1) return 0;
    Frame f;
    const uint32_t then = type == 0 ? newFunction(f, VT_ToLevelUp, {self}) : newFunction(f, VT_ToEvolve, {});
    call(IInOut_runOutAnimation, {self + F_INOUT, then});
    functionDestroy(then);
    return 0;
}
RW_FUNCTION(0x667DC0, "SceneDeckEdit::onTapMedalInfoButton", deck_edit_leftovers::onTapMedalInfoButton);

// $_36: (after the out animation) the Level Up screen
uint32_t toLevelUp(uint32_t self) {
    st8(ld32(self + 4) + F_LEAVING, 1);
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t factory = newFunction(f, VT_LevelUpLayer, {});
    call(SM_changeSceneLayer, {sm, factory, 1, 0, 0});
    functionDestroy(factory);
    return 0;
}
RW_FUNCTION(0x677C50, "SceneDeckEdit::onTapMedalInfoButton::$_36 operator() (to Level Up)", deck_edit_leftovers::toLevelUp);

// updateDeckView $_33: a deck medal's info popup button (the medal id comes as an int)
uint32_t deckMedalInfoButton(uint32_t self, uint32_t typeRef, uint32_t, uint32_t idRef) {
    const int32_t id = int32_t(ld32(idRef));
    const uint32_t hi = uint32_t(id >> 31);
    return call(SceneDeckEdit_onTapMedalInfoButton, {ld32(self + 4), ld32(typeRef), 0, hi, uint32_t(id), hi});
}
RW_FUNCTION(0x67802A, "SceneDeckEdit::updateDeckView::$_33 operator() (info button)", deck_edit_leftovers::deckMedalInfoButton);

// updateDeckView $_34 (a medal set into a slot): greyed out in the panel, the drop tutorial step done
uint32_t medalDropped(uint32_t self, uint32_t idRef) {
    call(MiniMedal_setDisableMedalID, {ld32(ld32(self + 4) + F_MINI), 1, ld32(idRef), ld32(idRef + 4), 0, 1});
    call(SM_ChainOfTutorialAdventureSelect, {sceneManager(), TUTORIAL_DECK_DROP});
    return call(AppState_addClearTutorialPopupIDs, {call(fn::AppState_getInstance, {}), TUTORIAL_DECK_DROP});
}
RW_FUNCTION(0x677F5E, "SceneDeckEdit::updateDeckView::$_34 operator() (medal set)", deck_edit_leftovers::medalDropped);

}  // namespace deck_edit_leftovers
