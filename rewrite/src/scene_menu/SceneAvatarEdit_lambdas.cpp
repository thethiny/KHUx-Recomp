// SceneAvatarEdit::init's control lambdas: Create / Save, the gender pair, the part list's taps (wear a part, an
// accessory), the "my coordinate" swap and the finish callback.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace avataredit {
// SceneAvatarEdit
constexpr uint32_t AE_COORD = 0x2E4, AE_TAB = 0x2E8, AE_GENDER = 0x2EC, AE_SWAPPED = 0x2ED;
constexpr uint32_t AE_PARTS = 0x2F0;         // AvatarPartsScrollView*
constexpr uint32_t AE_WORN = 0x30C;          // int*: the parts worn, by slot
constexpr uint32_t AE_PRESET = 0x314;        // the default coordinate written on a swap
// AvatarPartsScrollView
constexpr uint32_t PV_BUSY = 0x66C, PV_ACCESSORY_MODE = 0x680, PV_ONE_ACCESSORY = 0x690;
constexpr uint32_t PV_IDS = 0x628;           // vector<int>: the listed part ids
// callees
constexpr uint32_t SAE_openCreateCheckPopup = 0x709669, SAE_saveMyCoordinate = 0x70A505;
constexpr uint32_t SAE_changeGender = 0x707CF9, SAE_setAccessory = 0x708FE9;   // (this, id, bool, bool)
constexpr uint32_t SAE_changeMyCordinate = 0x705C25, SAE_updateTabNew = 0x7076B1;
constexpr uint32_t SAE_createAccessoryIDs = 0x707C61;   // (sret vector<int>, this)
constexpr uint32_t SAE_finalize = 0x70A71D;             // (-, function<void(bool)> by value)
constexpr uint32_t AvatarPartsScrollView_setSelectedIDs = 0x554F05;
constexpr uint32_t IntVector_dtor = 0x478961;
constexpr uint32_t AvatarParts_create = 0x778DE1, SafeBuffer_read = 0x45A269;
constexpr uint32_t Vector_throwOutOfRange = 0x47CCE1;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;            // (dst, n, value)
constexpr uint32_t AVATARPARTS_KIND = 0x88;
constexpr uint32_t VT_Saved = 0xF6AC74;                 // $_4's save answer {this}
constexpr uint32_t AE_LOCKED = 0x2E0, AE_TABS = 0x2BC;   // a byte; vector<ImageButton*>
constexpr uint32_t AE_ROOT = 0x298, AE_FILTER = 0x341, AE_FILTER_EDIT = 0x34B;   // the filter: ten bytes each
constexpr uint32_t SAE_changeTab = 0x705CF5, SAE_selectCoordinateButton = 0x707051;   // sub_707050(buttons, index)
constexpr uint32_t SAE_createFilterdAccessoryIDs = 0x707951;     // (sret vector<int>, this)
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5, BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t ImageButton_setIsSelected = 0x5318FD;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t EaseSineOut_create = 0x860D09, SM_closeCurrentPopup = 0x6216A1, AEABI_MEMCPY = 0x40E164;
constexpr uint32_t S_TxtNoAvatar = 0xE0681F;       // "Txt_NoAvatar"
constexpr uint32_t VT_FilterClosedEnd = 0xF6A828;  // $_31's end {layer, shadow, -}
constexpr uint32_t TOUCH_ENDED = 2;

bool busy(uint32_t me) { return ld8(ld32(me + AE_PARTS) + PV_BUSY) != 0; }

// $_3 (Create) {this}: not busy -> the decide sound and the "create this avatar?" popup
void createTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (busy(me)) return;
    call(fn::AudioPlaySystemSEDecide, {});
    call(SAE_openCreateCheckPopup, {me});
}
// $_4 (Save) {this}: not busy -> the decide sound and the coordinate saved (its answer {this})
void saveTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (busy(me)) return;
    call(fn::AudioPlaySystemSEDecide, {});
    Frame fr;
    const uint32_t done = newFunction(fr, VT_Saved, {me});
    call(SAE_saveMyCoordinate, {me, done});
    functionDestroy(done);
}
// $_5 / $_6 (female / male) {this}: not busy and not that gender yet -> the decide sound, the gender changed
void genderTouched(uint32_t f, uint32_t typeRef, uint32_t gender) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (busy(me) || (ld8(me + AE_GENDER) != 0) == (gender != 0)) return;
    call(fn::AudioPlaySystemSEDecide, {});
    call(SAE_changeGender, {me, gender});
}
void femaleTouched(uint32_t f, uint32_t, uint32_t typeRef) { genderTouched(f, typeRef, 1); }
void maleTouched(uint32_t f, uint32_t, uint32_t typeRef) { genderTouched(f, typeRef, 0); }

// the part list's id `index`
uint32_t listedId(uint32_t parts, uint32_t index) {
    const uint32_t ids = parts + PV_IDS;
    if (((ld32(ids + 4) - ld32(ids)) >> 2) <= index) call(Vector_throwOutOfRange, {ids});
    return ld32(ld32(ids) + 4 * index);
}
// the list's ticks redrawn from the accessories worn
void showAccessoryTicks(uint32_t me) {
    const uint32_t parts = ld32(me + AE_PARTS);
    Frame fr;
    const uint32_t ids = fr.alloc(12);
    call(SAE_createAccessoryIDs, {ids, me});
    call(AvatarPartsScrollView_setSelectedIDs, {parts, ids});
    call(IntVector_dtor, {ids});
}

// $_7 (a part tapped) {this}(index): the decide sound; the part worn by its kind (2 a body set: in one-accessory
// mode the accessories off; 3..6 a slot; 7 an accessory, the ticks redrawn); the coordinate and the tab's marks
void partTapped(uint32_t f, uint32_t indexRef) {
    const uint32_t index = ld32(indexRef);
    const uint32_t me = ld32(f + 4);
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t id = listedId(ld32(me + AE_PARTS), index);
    Frame fr;
    const uint32_t row = fr.alloc(8), kind = fr.alloc(4);
    call(AvatarParts_create, {row, id});
    const uint32_t sb = ld32(row);
    call(SafeBuffer_read, {sb, kind, ld32(sb + 0xC) + AVATARPARTS_KIND, 4, AVATARPARTS_KIND});
    const uint32_t worn = me + AE_WORN;
    switch (ld32(kind)) {
    case 2:
        if (ld8(ld32(me + AE_PARTS) + PV_ONE_ACCESSORY)) {
            for (uint32_t i = 7; i != 12; i++) st32(ld32(worn) + 4 * i, 0);
            call(SAE_setAccessory, {me, id, 0, 0});
            st32(ld32(worn) + 0x14, id);
        } else {
            call(SAE_setAccessory, {me, id, 0, 1});
        }
        break;
    case 3: st32(ld32(worn) + 8, id); break;
    case 4: st32(ld32(worn) + 0x10, id); break;
    case 5: st32(ld32(worn) + 0x18, id); break;
    case 6: st32(ld32(worn) + 0xC, id); break;
    case 7:
        call(SAE_setAccessory, {me, id, 0, 0});
        showAccessoryTicks(me);
        break;
    default: break;
    }
    call(SAE_changeMyCordinate, {me, ld32(me + AE_COORD)});
    call(SAE_updateTabNew, {me, ld32(me + AE_TAB)});
    sharedRelease(row);
}
// $_8 (an accessory ticked off) {this}(index): in accessory mode -> the decide sound, the accessory taken off, the
// ticks redrawn, the coordinate
void accessoryTapped(uint32_t f, uint32_t indexRef) {
    const uint32_t me = ld32(f + 4);
    if (!ld8(ld32(me + AE_PARTS) + PV_ACCESSORY_MODE)) return;
    const uint32_t index = ld32(indexRef);
    call(fn::AudioPlaySystemSEDecide, {});
    call(SAE_setAccessory, {me, listedId(ld32(me + AE_PARTS), index), 1, 0});
    showAccessoryTicks(me);
    call(SAE_changeMyCordinate, {me, ld32(me + AE_COORD)});
}
// the default coordinate of the avatar's gender written ({gender, 5 part ids, 5 accessories: the first set})
void writePreset(uint32_t me) {
    const bool female = ld8(me + AE_GENDER) != 0;
    const uint32_t p = me + AE_PRESET;
    st32(p, female ? 2 : 1);
    st32(p + 0x10, female ? 11 : 1);
    st32(p + 0x14, female ? 30006 : 30001);
    st32(p + 0xC, female ? 20019 : 20001);
    st32(p + 4, female ? 41002 : 40001);
    st32(p + 8, female ? 51001 : 50001);
    call(AEABI_MEMSET, {p + 0x18, 0x14, 0});
    st32(p + 0x18, female ? 109011 : 109001);
}

// $_9 (the coordinate swap) {this}(sender, type): the decide sound, swapped flipped, the coordinate redrawn, the
// default coordinate written
void swapTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    call(fn::AudioPlaySystemSEDecide, {});
    st8(me + AE_SWAPPED, ld8(me + AE_SWAPPED) ^ 1);
    call(SAE_changeMyCordinate, {me, ld32(me + AE_COORD)});
    writePreset(me);
}
// $_10 (finished) {}(done function<void(bool)> by value): finalize with a copy of it
void finished(uint32_t, uint32_t done) {
    Frame fr;
    const uint32_t moved = fr.alloc(24), copy = fr.alloc(24);
    call(fn::function_vb_move, {moved, done});
    functionCopy(copy, moved);
    call(SAE_finalize, {ld32(copy + 16), copy});   // r0: the clone (finalize does not read it)
    functionDestroy(copy);
    functionDestroy(moved);
}

// $_17 (a tab) {this, index}(sender, type): not busy and another tab -> the decide sound, only that tab selected
// (and changed to)
void tabTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (busy(me) || ld32(me + AE_TAB) == ld32(f + 8)) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t tabs = me + AE_TABS;
    if (int32_t(ld32(tabs + 4) - ld32(tabs)) < 1) return;
    for (int32_t k = 0; k < int32_t(ld32(tabs + 4) - ld32(tabs)) >> 2; k++) {
        const int32_t index = int32_t(ld32(f + 8));
        call(ImageButton_setIsSelected, {ld32(ld32(tabs) + 4 * uint32_t(k)), k == index});
        if (k == index) call(SAE_changeTab, {me, ld32(f + 8)});
    }
}
// $_15 (my coordinate) {this, buttons +8}(sender, type): not locked and not busy -> the decide sound, the next
// coordinate (after 5: the first) selected and shown, the tab redrawn, the default coordinate written
void coordinateTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (ld8(me + AE_LOCKED) || busy(me)) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const int32_t coord = int32_t(ld32(me + AE_COORD));
    const uint32_t next = coord > 4 ? 0 : uint32_t(coord + 1);
    call(SAE_selectCoordinateButton, {f + 8, next});
    call(SAE_changeMyCordinate, {me, next});
    call(SAE_changeTab, {me, ld32(me + AE_TAB)});
    writePreset(me);
}
// $_19 (the accessory filter's OK) {this}(sender, type): the decide sound, the edited filter kept, the list
// refiltered from the top, "no accessories" shown when it is empty, the popup closed
void filterOk(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    call(fn::AudioPlaySystemSEDecide, {});
    call(AEABI_MEMCPY, {me + AE_FILTER, me + AE_FILTER_EDIT, 10});
    call(IScrollProtocol_setIndex, {ld32(me + AE_PARTS), 0});
    const uint32_t parts = ld32(me + AE_PARTS);
    uint32_t label;
    {
        Frame fr;
        const uint32_t ids = fr.alloc(12);
        call(SAE_createFilterdAccessoryIDs, {ids, me});
        call(BaseScrollViewInt_setItemIDs, {parts, ids});
        call(IntVector_dtor, {ids});
        const uint32_t name = fr.alloc(12);   // "Txt_NoAvatar": the literal's inline long string
        const uint32_t data = call(fn::operator_new, {0x10});
        st32(name + 8, data);
        st32(name, 0x11);
        st32(name + 4, 12);
        call(AEABI_MEMCPY, {data, S_TxtNoAvatar, 12});
        st8(data + 12, 0);
        label = call(SceneUtil_searchNodeByName, {ld32(me + AE_ROOT), name});
        strFree(name);
    }
    const uint32_t listed = ld32(me + AE_PARTS) + PV_IDS;
    vcall(label, node::setVisible, {label, ld32(listed + 4) == ld32(listed)});
    call(SM_closeCurrentPopup, {sceneManager(), 1});
}
// $_31 (the accessory filter closed) {OK, Close, popup, layer, shadow, -}(ok): Close -> the cancel sound; both
// buttons off, the popup shrinks (eased) and the shadow fades
void filterClosed(uint32_t f, uint32_t okRef) {
    if (!ld8(okRef)) call(fn::AudioPlaySystemSECancel, {});
    vcall(ld32(f + 4), node::setTouchEnabled, {ld32(f + 4), 0});
    vcall(ld32(f + 8), node::setTouchEnabled, {ld32(f + 8), 0});
    const uint32_t popup = ld32(f + 0xC);
    {
        Frame fr;
        const uint32_t end = newFunction(fr, VT_FilterClosedEnd, {ld32(f + 0x10), ld32(f + 0x14), ld32(f + 0x18)});
        const uint32_t anim = call(SceneUtil_createClosePopupAnimation, {popup, end});
        call(fn::Node_runAction, {popup, call(EaseSineOut_create, {anim})});
        functionDestroy(end);
    }
    call(fn::Node_runAction, {ld32(f + 0x14), call(fn::FadeTo_create, {fbits(0.1f), 0})});
}
}  // namespace avataredit
}  // namespace

RW_FUNCTION(0x70E254, "SceneAvatarEdit::init $_3::operator() (Create)", avataredit::createTouched);
RW_FUNCTION(0x70E060, "SceneAvatarEdit::init $_4::operator() (Save)", avataredit::saveTouched);
RW_FUNCTION(0x70DF94, "SceneAvatarEdit::init $_5::operator() (female)", avataredit::femaleTouched);
RW_FUNCTION(0x70DEC8, "SceneAvatarEdit::init $_6::operator() (male)", avataredit::maleTouched);
RW_FUNCTION(0x70DAE4, "SceneAvatarEdit::init $_7::operator()(int) (part tapped)", avataredit::partTapped);
RW_FUNCTION(0x70D988, "SceneAvatarEdit::init $_8::operator()(int) (accessory tapped)", avataredit::accessoryTapped);
RW_FUNCTION(0x70D818, "SceneAvatarEdit::init $_9::operator() (coordinate swap)", avataredit::swapTouched);
RW_FUNCTION(0x70D69C, "SceneAvatarEdit::init $_10::operator()(function<void(bool)>)", avataredit::finished);
RW_FUNCTION(0x70C87C, "SceneAvatarEdit::initTabButton $_17::operator() (a tab)", avataredit::tabTouched);
RW_FUNCTION(0x70CA44, "SceneAvatarEdit::initMyCordinateButton $_15::operator()", avataredit::coordinateTouched);
RW_FUNCTION(0x70C4B8, "SceneAvatarEdit::openAccessoriesFilterPopup $_19::operator() (OK)", avataredit::filterOk);
RW_FUNCTION(0x70BAC8, "SceneAvatarEdit::openAccessoriesFilterPopup $_31::operator()(bool) (closed)", avataredit::filterClosed);
