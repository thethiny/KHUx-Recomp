// IMedalSortFilter: the medal sort / filter mix-in of the medal list screens (deck edit, fusion, evolve, sell,
// medal list). It keeps the screen's medals and the filtered + sorted view of them, builds the "Sort" button
// (label = the current sort key, "Filter on" marker) and opens the sort / filter popup.
// The state (AppState::MedalSortFilterState) comes from the implementing scene: virtual slot 2 returns it.
#pragma once

#include "rw/rw.hpp"
#include "core/Guest.hpp"
#include "core/AppStateTypes.hpp"
#include "MedalDataManager.hpp"
#include "FuncThunks.hpp"

struct IMedalSortFilter {
    rw::Field<uint32_t> vtable;   // +0x00 slot 2: getSortFilterState() -> shared_ptr<MedalSortFilterState> (sret)
    guest::Vector medals;         // +0x04 vector<shared_ptr<UserMedalData>>: every medal of the screen
    guest::Vector filtered;       // +0x10 the filtered, sorted view

    static constexpr uint32_t VTABLE = 0xF8B298;     // _ZTV16IMedalSortFilter + 8
    static constexpr uint32_t VSLOT_GET_STATE = 2;

    static IMedalSortFilter *destruct(IMedalSortFilter *self);
    static void destructDelete(IMedalSortFilter *self);
    // Rebuild `filtered` from `medals` with the state's filters, then sort it by the state's sort key.
    static void runSortFilter(IMedalSortFilter *self, int32_t sceneType);
    // Set up the "Sort" button of `root`: its labels, a touch handler opening the popup, the "Filter on" marker.
    static void registSortButton(IMedalSortFilter *self, uint32_t root, int32_t sceneType, uint32_t callback);
    // DeckMedalView::ShowType for the state's sort key (`defaultType` when the key has none).
    static int32_t getShowType(uint32_t self, const guest::SharedPtr *state, uint32_t equipped);

    // ---- lambdas
    // registSortButton: the sort key label (TextData) for the state's sort index
    static void setSortLabel(const uint32_t *label, const guest::SharedPtr *state);
    // runSortFilter's std::sort comparator: medal a before medal b?
    static uint32_t compareMedals(const uint32_t *comparator, const guest::SharedPtr *a, const guest::SharedPtr *b);
};
RW_LAYOUT(IMedalSortFilter, medals, 4);
RW_LAYOUT(IMedalSortFilter, filtered, 0x10);
RW_SIZE(IMedalSortFilter, 0x1C);
