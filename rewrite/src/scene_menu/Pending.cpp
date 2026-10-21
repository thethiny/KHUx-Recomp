// Functions declared by wave-2 headers whose rewrites were not written yet (their agents were stopped): each one
// forwards to the lifted original so callers in rewritten code link. Not registered, so nothing is replaced.
#include "SceneAvatarEdit.hpp"
#include "SceneSphereBoardSelect.hpp"
#include "../battleui/CarouselDeck.hpp"
#include "../scene_main/SceneAdventureSelect.hpp"
#include "../w3_7/SceneMixedMedal.hpp"
#include "../w3_9/RaidSubdueScrollView.hpp"

using rw::guest_addr;

void SceneAvatarEdit::settingDefaultCoordinate(SceneAvatarEdit *self, Coordinate *coordinate, uint32_t male) {
    rw::call(0x7061A1, {guest_addr(self), guest_addr(coordinate), male});
}
void CarouselDeck::setDeckMedal(CarouselDeck *self, uint32_t unused) {
    rw::call(0x5D17A1, {guest_addr(self), unused});
}

// wave 3 (stopped by the usage limit)
void SceneAdventureSelect::initEvent(Addr self) { rw::call(0x721839, {self}); }
void SceneAdventureSelect::initEventBanner(Addr self, uint32_t flag) { rw::call(0x720A5D, {self, flag}); }
void SceneMixedMedal::finalize(SceneMixedMedal *self) { rw::call(0x69F56D, {guest_addr(self)}); }
// sub_5B6140: std::function<void(Touch*,Event*)> = registItemTouchEvent()::$_2
void RaidSubdueScrollView::assignTouchEnded(Addr function, Addr captures) { rw::call(0x5B6141, {function, captures}); }
