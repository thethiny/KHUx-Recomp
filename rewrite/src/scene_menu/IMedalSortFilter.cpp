// IMedalSortFilter: filtering and sorting of the medal lists, the "Sort" button.
// openMedalSortPopup (10870 bytes, a ~5400-instruction popup builder) is not rewritten (too large); its lambdas
// are in IMedalSortFilter_popup.cpp. The std::sort internals instantiated for the comparator (sub_67403C __sort,
// sub_674A04 __sort3, sub_674ADA __sort4, sub_674B7A __sort5, sub_674C44 __insertion_sort_3) are libc++ code.
#include "IMedalSortFilter.hpp"

#include "../scene/SceneUi.hpp"

using appstate::MedalSortFilterState;
using UserMedalData = MedalDataManager::UserMedalData;

namespace {
constexpr uint32_t VECTOR_BASE_DTOR = 0x46B119;        // __vector_base<shared_ptr<UserMedalData>>::~__vector_base
constexpr uint32_t PUSH_BACK_SLOW = 0x487A39;          // vector<shared_ptr<UserMedalData>>::__push_back_slow_path
constexpr uint32_t SORT = 0x67403D;                    // std::__sort<comparator&, shared_ptr<UserMedalData>*>
constexpr uint32_t MEDAL_DATA_GET_INSTANCE = 0x76AD69; // MedalDataManager::getInstance
constexpr uint32_t IS_FAVORITE_MEDAL = 0x76B249;       // MedalDataManager::isFavoriteMedal(shared_ptr)
constexpr uint32_t SKILL_DATA_GET_INSTANCE = 0x76BA0D; // SkillDataManager::getInstance
constexpr uint32_t SKILL_DATA_GET = 0x76BA59;          // SkillDataManager::getData(u64, UserSkill::Skill, bool)
constexpr uint32_t SET_SHADOW = 0x45BD15;              // SceneUtil::setShadow(Text*)
constexpr uint32_t WIDGET_ADD_TOUCH = 0x826CCF;        // Widget::addTouchEventListener(std::function)
constexpr uint32_t FUNCTION_VB_COPY = 0x45C84D;        // std::function<void(bool)>(const function&)
constexpr uint32_t FUNCTION_VB_MOVE = 0x47BC69;        // std::function<void(bool)>(function&&)
constexpr uint32_t SHOW_TYPE_TABLE = 0xE6E270;         // int[10]: DeckMedalView::ShowType per sort index - 1

constexpr uint32_t STR_BUTTON_SORT = 0xE03DC6;         // "Button_Sort"
constexpr uint32_t STR_TXT_SORT = 0xE041FE;            // "Txt_Sort"
constexpr uint32_t STR_TXT_SORT_LABEL = 0xE04207;      // "Txt_Sort_Label"
constexpr uint32_t STR_TXT_FILTER_ON = 0xE04216;       // "Txt_Filter_On"
constexpr uint32_t VT_SORT_BUTTON_TOUCH = 0xF63260;    // __func<registSortButton::$_52, void(Ref*, TouchEventType)>

// master::Medal data positions
constexpr uint32_t MEDAL_SORT_ID = 12;
constexpr uint32_t MEDAL_TYPE = 1108;
constexpr uint32_t MEDAL_ATTRIBUTE = 1112;
constexpr uint32_t MEDAL_DARKLIGHT = 1116;
constexpr uint32_t MEDAL_RARE = 1136;
constexpr uint32_t MEDAL_SKILL_SLOT = 1140;
constexpr uint32_t MEDAL_COST = 1156;

int32_t medalRow32(uint32_t data, uint32_t index) {
    return rw::host_ptr<SafeBuffer>(rw::host_ptr<UserMedalData>(data)->medal.obj)->get32(index);
}
const MedalSortFilterState *stateOf(uint32_t sharedPtr) {
    return rw::host_ptr<MedalSortFilterState>(rw::load<uint32_t>(sharedPtr));
}
// any filter toggle on (bytes 3 .. 22)?
bool anyFilter(const MedalSortFilterState *s) {
    const uint8_t *b = reinterpret_cast<const uint8_t *>(s);
    for (uint32_t k = 3; k <= 22; k++)
        if (b[k]) return true;
    return false;
}
// MedalDataManager::isFavoriteMedal(getInstance(), copy of the shared_ptr): the copy is made after getInstance
uint32_t isFavorite(uint32_t obj, uint32_t ctrl) {
    scene::Frame f;
    uint32_t inst = rw::call(MEDAL_DATA_GET_INSTANCE, {});
    uint32_t copy = f.alloc(8);
    rw::save<uint32_t>(copy, obj);
    rw::save<uint32_t>(copy + 4, ctrl);
    if (ctrl) guest::addRef(ctrl);
    uint32_t r = rw::call(IS_FAVORITE_MEDAL, {inst, copy});
    if (rw::load<uint32_t>(copy + 4)) guest::release(rw::load<uint32_t>(copy + 4));
    return r;
}
// vector<shared_ptr>::push_back(copy)
void pushBack(guest::Vector &v, uint32_t sharedPtr) {
    uint32_t end = v.end;
    if (end != v.cap) {
        if (end) {
            uint32_t obj = rw::load<uint32_t>(sharedPtr), ctrl = rw::load<uint32_t>(sharedPtr + 4);
            rw::save<uint32_t>(end, obj);
            rw::save<uint32_t>(end + 4, ctrl);
            if (ctrl) guest::addRef(ctrl);
        }
        v.end = v.end + 8;
    } else {
        rw::call(PUSH_BACK_SLOW, {rw::guest_addr(&v), sharedPtr});
    }
}
}  // namespace

IMedalSortFilter *IMedalSortFilter::destruct(IMedalSortFilter *self) {
    self->vtable = VTABLE;
    rw::call(VECTOR_BASE_DTOR, {rw::guest_addr(&self->filtered)});
    rw::call(VECTOR_BASE_DTOR, {rw::guest_addr(&self->medals)});
    return self;
}
RW_FUNCTION(0x57B27C, "IMedalSortFilter::~IMedalSortFilter", IMedalSortFilter::destruct);

void IMedalSortFilter::destructDelete(IMedalSortFilter *self) {
    destruct(self);
    guest::deleteObject(rw::guest_addr(self));
}
RW_FUNCTION(0x57B2B0, "IMedalSortFilter::~IMedalSortFilter (deleting)", IMedalSortFilter::destructDelete);

void IMedalSortFilter::runSortFilter(IMedalSortFilter *self, int32_t) {
    scene::Frame f;
    uint32_t state = f.alloc(8);
    rw::vcall(self, VSLOT_GET_STATE, {state, rw::guest_addr(self)});
    // clear the filtered view
    uint32_t begin = self->filtered.begin;
    for (uint32_t end = self->filtered.end; end != begin;) {
        self->filtered.end = end - 8;
        uint32_t ctrl = rw::load<uint32_t>(end - 4);
        if (ctrl) guest::release(ctrl);
        end = self->filtered.end;
    }
    uint32_t sortEnd = begin;
    if (self->medals.end != self->medals.begin) {
        uint32_t medal = f.alloc(8);   // the copy of medals[i]
        for (uint32_t i = 0;; i++) {
            uint32_t src = self->medals.begin + 8 * i;
            uint32_t obj = rw::load<uint32_t>(src), ctrl = rw::load<uint32_t>(src + 4);
            rw::save<uint32_t>(medal, obj);
            rw::save<uint32_t>(medal + 4, ctrl);
            if (ctrl) guest::addRef(ctrl);
            const MedalSortFilterState *s = stateOf(state);
            const UserMedalData *d = rw::host_ptr<UserMedalData>(obj);
            bool keep;
            if (!anyFilter(s)) {
                keep = true;
            } else {
                int32_t attribute = medalRow32(obj, MEDAL_ATTRIBUTE);
                int32_t darklight = medalRow32(obj, MEDAL_DARKLIGHT);
                int32_t rare = medalRow32(obj, MEDAL_RARE);
                int32_t type = medalRow32(obj, MEDAL_TYPE);
                // attribute (power / speed / magic)
                bool attrOk = (!s->isPower && !s->isSpeed && !s->isMagic) || (attribute == 1 && s->isPower) ||
                              (attribute == 2 && s->isSpeed) || (attribute == 3 && s->isMagic);
                // light / dark
                bool lightOk = (!s->isLight && !s->isDark) || (darklight == 1 && s->isLight) ||
                               (darklight == 2 && s->isDark);
                // rarity 1..6
                const uint8_t *b = reinterpret_cast<const uint8_t *>(s);
                bool anyRare = false, rareOk = false;
                for (int32_t k = 1; k <= 6; k++) {
                    if (b[7 + k]) anyRare = true;
                    if (rare == k && b[7 + k]) rareOk = true;
                }
                rareOk = rareOk || !anyRare;
                // medal kind
                bool typeMatch = (type == 1 && s->isAttack) || (type == 2 && s->isLargeExp) ||
                                 ((type == 8 || type == 4) && s->isPowerUp) || (type == 5 && s->isCostMinus) ||
                                 (type == 6 && s->isEvolve) || (type == 3 && s->isLargeMoney);
                // (the original asks for the favourite state once before the kind test, result unused)
                if (!typeMatch && s->isFavorite) isFavorite(obj, ctrl);
                s = stateOf(state);
                bool anyKind = false;
                for (uint32_t k = 14; k <= 22; k++)
                    if (b[k]) anyKind = true;
                bool kindOk;
                if (!anyKind || typeMatch) {
                    kindOk = true;
                } else {
                    kindOk = false;
                    bool asked = false, favourite = false;
                    if (s->isFavorite) {
                        asked = true;
                        favourite = isFavorite(obj, ctrl) != 0;
                    }
                    if (favourite) {
                        kindOk = true;
                    } else {
                        s = stateOf(state);
                        if (s->isSetSkill && int32_t(d->skillCount) > 0) kindOk = true;
                        else if (s->isNotSetSkill) kindOk = d->skillCount == 0;
                    }
                    (void)asked;
                }
                keep = attrOk && lightOk && rareOk && kindOk;
            }
            if (keep) pushBack(self->filtered, medal);
            if (ctrl) guest::release(ctrl);
            if (i + 1 >= ((self->medals.end - self->medals.begin) >> 3)) break;
        }
        sortEnd = self->filtered.end;
    }
    // std::sort(filtered, [&state](a, b) { ... })
    uint32_t comparator = f.alloc(8);
    rw::save<uint32_t>(comparator, state);
    rw::call(SORT, {self->filtered.begin, sortEnd, comparator});
    if (rw::load<uint32_t>(state + 4)) guest::release(rw::load<uint32_t>(state + 4));
}
RW_FUNCTION(0x66204C, "IMedalSortFilter::runSortFilter", IMedalSortFilter::runSortFilter);

void IMedalSortFilter::registSortButton(IMedalSortFilter *self, uint32_t root, int32_t sceneType, uint32_t callback) {
    using namespace scene;
    Frame f;
    uint32_t state = f.alloc(8);
    rw::vcall(self, VSLOT_GET_STATE, {state, rw::guest_addr(self)});
    uint32_t button = findNode(root, STR_BUTTON_SORT, 11);
    titleLabel(button, STR_TXT_SORT, 8, 100100010);
    uint32_t label = findNode(root, STR_TXT_SORT_LABEL, 14);
    call(SET_SHADOW, {label});
    uint32_t filterOn = findNode(root, STR_TXT_FILTER_ON, 13);
    call(SET_SHADOW, {filterOn});
    setTextId(filterOn, 100100012);
    {
        // the sort key label, from a copy of the state
        uint32_t labelRef = f.alloc(4);
        st32(labelRef, label);
        uint32_t copy = f.alloc(8);
        uint32_t ctrl = ld32(state + 4);
        st32(copy, ld32(state));
        st32(copy + 4, ctrl);
        if (ctrl) guest::addRef(ctrl);
        setSortLabel(rw::host_ptr<uint32_t>(labelRef), rw::host_ptr<guest::SharedPtr>(copy));
        if (ctrl) guest::release(ctrl);
    }
    // the touch lambda: [self, sceneType, filterOn, label, state, callback]
    uint32_t lambda = f.alloc(0x30);
    st32(lambda, rw::guest_addr(self));
    st32(lambda + 4, uint32_t(sceneType));
    st32(lambda + 8, filterOn);
    st32(lambda + 12, label);
    st32(lambda + 16, ld32(state));
    st32(lambda + 20, ld32(state + 4));
    if (ld32(state + 4)) guest::addRef(ld32(state + 4));
    call(FUNCTION_VB_COPY, {lambda + 0x18, callback});
    if (button) {
        uint32_t copy = f.alloc(0x30);
        for (uint32_t k = 0; k < 16; k += 4) st32(copy + k, ld32(lambda + k));
        st32(copy + 16, ld32(lambda + 16));
        st32(copy + 20, ld32(lambda + 20));
        if (ld32(copy + 20)) guest::addRef(ld32(copy + 20));
        functionCopy(copy + 0x18, lambda + 0x18);
        uint32_t touch = f.alloc(20);
        st32(touch + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x38});
        if (heap) {
            st32(heap, VT_SORT_BUTTON_TOUCH);
            st32(heap + 0x14, ld32(copy + 12));
            st32(heap + 0x10, ld32(copy + 8));
            st32(heap + 0x0C, ld32(copy + 4));
            st32(heap + 0x08, ld32(copy));
            st32(heap + 0x18, ld32(copy + 16));
            st32(heap + 0x1C, ld32(copy + 20));
            st32(copy + 16, 0);
            st32(copy + 20, 0);
            call(FUNCTION_VB_MOVE, {heap + 0x20, copy + 0x18});
        }
        st32(touch + 16, heap);
        call(WIDGET_ADD_TOUCH, {button, touch});
        functionDestroy(touch);
        functionDestroy(copy + 0x18);
        if (ld32(copy + 20)) guest::release(ld32(copy + 20));
    }
    vcall(filterOn, node::setVisible, {filterOn, anyFilter(stateOf(state)) ? 1u : 0u});
    functionDestroy(lambda + 0x18);
    if (ld32(lambda + 20)) guest::release(ld32(lambda + 20));
    if (ld32(state + 4)) guest::release(ld32(state + 4));
}
RW_FUNCTION(0x664DA4, "IMedalSortFilter::registSortButton", IMedalSortFilter::registSortButton);

int32_t IMedalSortFilter::getShowType(uint32_t, const guest::SharedPtr *state, uint32_t equipped) {
    int32_t result = equipped ? 0 : 4;
    uint32_t k = uint32_t(rw::host_ptr<MedalSortFilterState>(state->obj)->sortIndex) - 1u;
    if (k <= 9 && ((0x29Fu >> (k & 0xFFFF)) & 1)) return rw::load<int32_t>(SHOW_TYPE_TABLE + 4 * k);
    return result;
}
RW_FUNCTION(0x66527C, "IMedalSortFilter::getShowType", IMedalSortFilter::getShowType);

// ------------------------------------------------------------------------------------------ lambdas
void IMedalSortFilter::setSortLabel(const uint32_t *label, const guest::SharedPtr *state) {
    static const int32_t TEXT_IDS[11] = {102700001, 102700003, 102700002, 102700004, 102700005, 102700006, 0,
                                         102700007, 102700009, 102700014, 102700115};
    uint8_t index = rw::host_ptr<MedalSortFilterState>(state->obj)->sortIndex;
    if (index > 10 || index == 6) return;
    uint32_t text = *label;
    scene::setTextId(text, TEXT_IDS[index]);
}
RW_FUNCTION(0x6698C0, "IMedalSortFilter::registSortButton sort label lambda", IMedalSortFilter::setSortLabel);

uint32_t IMedalSortFilter::compareMedals(const uint32_t *comparator, const guest::SharedPtr *a,
                                         const guest::SharedPtr *b) {
    uint32_t ma = a->obj, mb = b->obj;
    int32_t sortA = medalRow32(ma, MEDAL_SORT_ID);
    int32_t sortB = medalRow32(mb, MEDAL_SORT_ID);
    const MedalSortFilterState *s = stateOf(*comparator);
    const UserMedalData *da = rw::host_ptr<UserMedalData>(ma);
    const UserMedalData *db = rw::host_ptr<UserMedalData>(mb);
    uint8_t key = s->sortIndex;
    auto ordered = [&](int32_t va, int32_t vb) -> uint32_t {   // a before b: by value, ties by sort id
        if (va == vb) return sortA < sortB;
        return stateOf(*comparator)->isAsc ? (va < vb) : (va > vb);
    };
    switch (key) {
    case 0: return ordered(medalRow32(ma, MEDAL_ATTRIBUTE), medalRow32(mb, MEDAL_ATTRIBUTE));
    case 1: return ordered(medalRow32(ma, MEDAL_RARE), medalRow32(mb, MEDAL_RARE));
    case 2: return ordered(da->level, db->level);
    case 3: {
        int32_t ca = int32_t(uint32_t(da->upperCost) + uint32_t(medalRow32(ma, MEDAL_COST)));
        int32_t cb = int32_t(uint32_t(db->upperCost) + uint32_t(medalRow32(mb, MEDAL_COST)));
        return ordered(ca, cb);
    }
    case 4: return ordered(da->attack, db->attack);
    case 5: return ordered(da->defense, db->defense);
    case 6: return ordered(medalRow32(ma, MEDAL_SKILL_SLOT), medalRow32(mb, MEDAL_SKILL_SLOT));
    case 7: return ordered(da->getDatetime, db->getDatetime);
    case 8: return ordered(da->sellPrice, db->sellPrice);
    case 9: {
        // by the first set skill: id, then level; medals without a skill go last
        scene::Frame f;
        uint32_t skillA = 0, ctrlA = 0, dataA = 0, skillB = 0, ctrlB = 0, dataB = 0;
        if (int32_t(da->skillCount) >= 1) {
            uint32_t inst = rw::call(SKILL_DATA_GET_INSTANCE, {});
            uint32_t out = f.alloc(8);
            rw::call(SKILL_DATA_GET, {out, inst, da->userSkillIds[0], da->userSkillIds[1], 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0});
            dataA = rw::load<uint32_t>(out);
            ctrlA = rw::load<uint32_t>(out + 4);
            skillA = uint32_t(rw::host_ptr<SafeBuffer>(rw::load<uint32_t>(dataA + 0x20))->get32(0));
        }
        if (int32_t(db->skillCount) >= 1) {
            uint32_t inst = rw::call(SKILL_DATA_GET_INSTANCE, {});
            uint32_t out = f.alloc(8);
            rw::call(SKILL_DATA_GET, {out, inst, db->userSkillIds[0], db->userSkillIds[1], 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0});
            dataB = rw::load<uint32_t>(out);
            ctrlB = rw::load<uint32_t>(out + 4);
            skillB = uint32_t(rw::host_ptr<SafeBuffer>(rw::load<uint32_t>(dataB + 0x20))->get32(0));
        }
        int32_t ia = int32_t(skillA), ib = int32_t(skillB);
        uint32_t r;
        if (ia == ib) {
            if (ia != 0) {
                int32_t la = rw::host_ptr<SafeBuffer>(dataA)->get32(8);
                int32_t lb = rw::host_ptr<SafeBuffer>(dataB)->get32(8);
                r = la != lb ? (la > lb) : (sortA < sortB);
            } else {
                r = sortA < sortB;
            }
        } else if (ia == 0 && ib > 0) {
            r = 0;
        } else if (ia >= 1 && ib == 0) {
            r = 1;
        } else {
            r = stateOf(*comparator)->isAsc ? (ia < ib) : (ia > ib);
        }
        if (ctrlB) guest::release(ctrlB);
        if (ctrlA) guest::release(ctrlA);
        return r;
    }
    case 10: {
        // by attribute (always ascending), then sort id, then burst / guilt
        int32_t aa = medalRow32(ma, MEDAL_ATTRIBUTE), ab = medalRow32(mb, MEDAL_ATTRIBUTE);
        if (aa != ab) return aa < ab;
        if (sortA != sortB) return sortA < sortB;
        uint32_t gb = db->guiltFactor, ga = da->guiltFactor;
        bool eitherZero = ga == 0 || gb == 0;
        int32_t vb = gb == 0 ? db->burstUpperNumber : int32_t(uint32_t(db->burstUpperNumber) + 1u);
        if (!eitherZero) vb = int32_t(gb);
        int32_t va = ga == 0 ? da->burstUpperNumber : int32_t(uint32_t(da->burstUpperNumber) + 1u);
        if (!eitherZero) va = int32_t(ga);
        return stateOf(*comparator)->isAsc ? (va < vb) : (va > vb);
    }
    default: return 0;
    }
}
RW_FUNCTION(0x674500, "IMedalSortFilter::runSortFilter comparator", IMedalSortFilter::compareMedals);

// ------------------------------------------------------------------------------------------ sort button lambdas
// $_52 (the "Sort" button touch, a 0x38-byte __func): [filterOn... see registSortButton] =
//   +8 self, +0xC sceneType, +0x10 filterOn text, +0x14 sort label, +0x18 state, +0x20 callback
// $_52's inner lambda (popup closed, void(bool changed)): +8 filterOn, +0xC self, +0x10 sceneType, +0x14 label,
//   +0x18 state, +0x20 callback
namespace {
constexpr uint32_t VT_POPUP_CLOSED = 0xF6328C;          // __func<$_52::operator()::lambda(bool), void(bool)>
constexpr uint32_t SORT_BUTTON_NAME = 0xE69D60;         // type_info names
constexpr uint32_t POPUP_CLOSED_NAME = 0xE69EA0;
constexpr uint32_t SORT_BUTTON_TYPEINFO = 0xF9AE94;
constexpr uint32_t POPUP_CLOSED_TYPEINFO = 0xF9AEA8;
constexpr uint32_t AUDIO_SE_DECIDE = 0x5C0CF9;          // AudioPlaySystemSEDecide()
constexpr uint32_t OPEN_SORT_POPUP = 0x669D51;          // IMedalSortFilter::openMedalSortPopup
constexpr uint32_t RUN_SORT_FILTER = 0x66204D;
constexpr uint32_t FUNCTION_VB_CALL = 0x474139;         // std::function<void(bool)>::operator()
constexpr uint32_t TOUCH_ENDED = 2;

using SortButtonFunc = scene_menu::Words4SharedFnFunc<VT_SORT_BUTTON_TOUCH>;
using PopupClosedFunc = scene_menu::Words4SharedFnFunc<VT_POPUP_CLOSED>;

// Sort button released: open the sort / filter popup; when it closes, refresh the list and the button.
void sortButtonTouched(uint32_t self, const uint32_t *, const int32_t *type) {
    using namespace scene;
    if (*type != int32_t(TOUCH_ENDED)) return;
    uint32_t filter = ld32(self + 8);
    call(AUDIO_SE_DECIDE, {});
    Frame f;
    uint32_t state = f.alloc(8);
    vcall(filter, 8, {state, filter});
    uint32_t sceneType = ld32(self + 0xC);
    // [filterOn, self, sceneType, label, state, callback]
    uint32_t lambda = f.alloc(0x30);
    st32(lambda, ld32(self + 0x10));
    st32(lambda + 4, filter);
    st32(lambda + 8, sceneType);
    st32(lambda + 12, ld32(self + 0x14));
    st32(lambda + 16, ld32(self + 0x18));
    uint32_t ctrl = ld32(self + 0x1C);
    st32(lambda + 20, ctrl);
    if (ctrl) guest::addRef(ctrl);
    call(scene_menu::FUNCTION_VB_COPY, {lambda + 0x18, self + 0x20});
    uint32_t closed = f.alloc(20);
    st32(closed + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x38});
    uint32_t r0 = heap;
    if (heap) {
        st32(heap, VT_POPUP_CLOSED);
        st32(heap + 0x14, ld32(lambda + 12));
        st32(heap + 0x10, ld32(lambda + 8));
        st32(heap + 0x0C, ld32(lambda + 4));
        st32(heap + 0x08, ld32(lambda));
        st32(heap + 0x18, ld32(lambda + 16));
        st32(heap + 0x1C, ld32(lambda + 20));
        st32(lambda + 16, 0);
        st32(lambda + 20, 0);
        r0 = call(FUNCTION_VB_MOVE, {heap + 0x20, lambda + 0x18});
    }
    st32(closed + 16, heap);
    // openMedalSortPopup ignores its vector argument: r0 is whatever the previous call left
    call(OPEN_SORT_POPUP, {r0, state, sceneType, closed});
    scene_menu::destroyFunctionAt(closed);
    scene_menu::destroyFunctionAt(lambda + 0x18);
    scene_menu::releaseAt(lambda + 20);
    scene_menu::releaseAt(state + 4);
}

// The popup closed: show "Filter on" when something changed, re-run the filter, update the sort label and
// call the screen's callback.
void popupClosed(uint32_t self, const uint8_t *changed) {
    using namespace scene;
    uint8_t c = *changed;
    uint32_t filterOn = ld32(self + 8);
    uint32_t filter = ld32(self + 0xC);
    vcall(filterOn, node::setVisible, {filterOn, c ? 1u : 0u});
    call(RUN_SORT_FILTER, {filter});
    Frame f;
    uint32_t copy = f.alloc(8);
    st32(copy, ld32(self + 0x18));
    uint32_t ctrl = ld32(self + 0x1C);
    st32(copy + 4, ctrl);
    if (ctrl) guest::addRef(ctrl);
    IMedalSortFilter::setSortLabel(rw::host_ptr<uint32_t>(self + 0x14), rw::host_ptr<guest::SharedPtr>(copy));
    if (ctrl) guest::release(ctrl);
    if (ld32(self + 0x30)) call(FUNCTION_VB_CALL, {self + 0x20, c ? 1u : 0u});
}

uint32_t sortButtonTarget(uint32_t self, uint32_t ti) {
    return scene_menu::funcTargetAt(self, ti, SORT_BUTTON_NAME, 8);
}
uint32_t sortButtonTargetType() { return SORT_BUTTON_TYPEINFO; }
uint32_t popupClosedTarget(uint32_t self, uint32_t ti) {
    return scene_menu::funcTargetAt(self, ti, POPUP_CLOSED_NAME, 8);
}
uint32_t popupClosedTargetType() { return POPUP_CLOSED_TYPEINFO; }
}  // namespace

RW_FUNCTION(0x6739BC, "IMedalSortFilter::registSortButton::$_52 __func::~__func", SortButtonFunc::destroyFunc);
RW_FUNCTION(0x673A04, "IMedalSortFilter::registSortButton::$_52 __func::~__func (deleting)", SortButtonFunc::deleteFunc);
RW_FUNCTION(0x673A50, "IMedalSortFilter::registSortButton::$_52 __func::__clone()", SortButtonFunc::clone);
RW_FUNCTION(0x673AD0, "IMedalSortFilter::registSortButton::$_52 __func::__clone(dst)", SortButtonFunc::cloneInto);
RW_FUNCTION(0x673B38, "IMedalSortFilter::registSortButton::$_52 __func::destroy", SortButtonFunc::destroy);
RW_FUNCTION(0x673B66, "IMedalSortFilter::registSortButton::$_52 __func::destroy_deallocate", SortButtonFunc::destroyDeallocate);
RW_FUNCTION(0x673B9C, "IMedalSortFilter::registSortButton::$_52 (sort button touched)", sortButtonTouched);
RW_FUNCTION(0x673D24, "IMedalSortFilter::registSortButton::$_52 __func::target", sortButtonTarget);
RW_FUNCTION(0x673D48, "IMedalSortFilter::registSortButton::$_52 __func::target_type", sortButtonTargetType);
RW_FUNCTION(0x673D5C, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::~__func", PopupClosedFunc::destroyFunc);
RW_FUNCTION(0x673DA4, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::~__func (deleting)", PopupClosedFunc::deleteFunc);
RW_FUNCTION(0x673DF0, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::__clone()", PopupClosedFunc::clone);
RW_FUNCTION(0x673E70, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::__clone(dst)", PopupClosedFunc::cloneInto);
RW_FUNCTION(0x673ED8, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::destroy", PopupClosedFunc::destroy);
RW_FUNCTION(0x673F06, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::destroy_deallocate", PopupClosedFunc::destroyDeallocate);
RW_FUNCTION(0x673F3C, "IMedalSortFilter::registSortButton::$_52::lambda(bool) (popup closed)", popupClosed);
RW_FUNCTION(0x674004, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::target", popupClosedTarget);
RW_FUNCTION(0x674028, "IMedalSortFilter::registSortButton::$_52::lambda(bool) __func::target_type", popupClosedTargetType);
