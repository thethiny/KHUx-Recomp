// SceneAvatarEdit, the rest: the "new" marks of the tabs, the accessory lists (filtered by the ten category
// filters, which map categories 2 and 4 to each other's filter bytes - as the original does), the accessory filter
// popup, wearing an accessory (conflicts and the five slots), the gender switch (which writes a stale stack word
// into the default coordinate's id in the original - left out here), the save popup and the save itself, and the
// debug parts list (never called in the release).
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t SafeBuffer_read = 0x45A269, SafeBuffer_dtor = 0x45A13D, AvatarParts_create = 0x778DE1;
constexpr uint32_t shared_addRef = 0xC8DBB5;
constexpr uint32_t AvatarParts_getAvatarPartsIds = 0x778DC5, tree_int_destroy = 0x48D9B5;
constexpr uint32_t vector_int_push_slow = 0x4D6CC1;      // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t vector_int_deallocate = 0x47D6B1, vector_int_dtor = 0x478961;
constexpr uint32_t vector_parts_dtor = 0x709625;          // __vector_base<shared_ptr<AvatarParts const>>
constexpr uint32_t ThrowOutOfRange = 0x47CCE1, aeabi_memmove = 0x40EF50, libc_memcpy = 0x40E164;
constexpr uint32_t AppState_getNewAvatarPartsIDs = 0x4EBB45, AppState_getNewAvatarPartsSetIDs = 0x4EBCBF;
constexpr uint32_t SAE_updateTabNew = 0x7076B1, SAE_createFilterdAccessoryIDs = 0x707951;
constexpr uint32_t SAE_genderFiliteringIDs = 0x707B4D, SAE_changeMyCordinate = 0x705C25, SAE_changeTab = 0x705CF5;
constexpr uint32_t SAE_generateAccessories = 0x708DA1, SAE_updateCost = 0x707491, SAE_getEquipKeybladeID = 0x70715D;
constexpr uint32_t AvatarPartsScrollView_setSelectedID = 0x554EF1;
constexpr uint32_t AvatarSent = 0x70B291;                 // the avatar-sent callback (sub_70B290), called directly
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t CheckBox_addEventListener = rwlib::cocos2d::ui::CheckBox::addEventListener, CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance, ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t AvatarActor_create = 0x73FD21, AvatarActor_setStyle = 0x740635, AvatarUtil_makeStyle = 0x497105;
constexpr uint32_t Avatar_preloadResources = 0x5C7ED1;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_updateHeaderAvatarIcon = 0x611725;
constexpr uint32_t APIManager_executeUserAvatarUpdate = 0x4C8B55, APIManager_executeUserGet = 0x4C6081;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t function_vb_copy = 0x45C84D, function_vb_move = 0x47BC69, function_vb_call = 0x474139;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t VT_FilterClose = 0xF6AA64;     // {}
constexpr uint32_t VT_FilterOk = 0xF6AA38;        // {this}
constexpr uint32_t VT_Filter01 = 0xF6AA0C;        // {this}; Filter02..10 follow at -0x2C each
constexpr uint32_t VT_FilterCanClose = 0xF6A854;  // {Close}
constexpr uint32_t VT_FilterClosed = 0xF6A7FC;    // {Close, OK, window, layer, shadow, root} (heap 0x1C)
constexpr uint32_t VT_SaveTouch = 0xF6A644;       // {window, layer, shadow, root} (heap 0x14)
constexpr uint32_t VT_SaveCloseEnd = 0xF6A670;    // {layer, shadow, root}
constexpr uint32_t VT_Saved = 0xF6A5EC;           // {pad, answer} (heap 0x20)
constexpr uint32_t VT_SaveError = 0xF6A618;       // {}
constexpr uint32_t VT_AvatarSent = 0xF6A6C8;      // {byte}
constexpr uint32_t S_FilterJson = 0xE0682C, S_Win = 0xE04096, S_CloseButton = 0xDF8D2F, S_ButtonOK = 0xDF8688;
constexpr uint32_t S_TxtOK = 0xDF869F, S_Filter01 = 0xE06856, S_TxtFilter01 = 0xE0685F, FILTER_STRIDE = 0x16;
constexpr uint32_t S_SaveJson = 0xE06988, S_Win04 = 0xE06981, S_TxtWording = 0xDF862F;
constexpr uint32_t S_TapArmatureFile = 0xDFFCF2, S_TapArea = 0xE00317, S_TapArmature = 0xDFFB2B;
constexpr uint32_t S_Animation1 = 0xDFD0AF, S_Empty = 0xDF8A1A, S_CenterButton = 0xDFFB40, S_Avatar = 0xDF8F56;
constexpr uint32_t S_TabFmt = 0xE06801, S_IconNew = 0xDFDD8A;
constexpr uint32_t V_getLocalZOrder = 0x18, V_setScale = 0x40, V_setPosition = 0x4C, V_getPositionX = 0x68;
constexpr uint32_t V_getPositionY = 0x70, V_setVisible = 0xB0, V_addChild = 0xF0, V_addChildZ = 0xF4;
constexpr uint32_t V_addChildZName = 0xFC, V_getParent = 0x11C, V_removeFromParent = 0x124;
constexpr uint32_t V_getAnimation = 0x298, V_playAnimation = 0x68;
// SceneAvatarEdit
constexpr uint32_t F_ROOT = 0x28C, F_NEW_MALE = 0x2C8, F_NEW_FEMALE = 0x2D4, F_COORD_INDEX = 0x2E4, F_TAB = 0x2E8;
constexpr uint32_t F_MALE = 0x2EC, F_SHOW_KEYBLADE = 0x2ED, F_SCROLL = 0x2F0, F_PART_IDS = 0x2F4;
constexpr uint32_t F_COORDS = 0x300, F_CURRENT = 0x30C, F_DEFAULT = 0x310, F_FILTER = 0x341, F_FILTER_EDIT = 0x34B;
// Coordinate: +0x14 the set (a parts set id), +0x1C the five accessory slots
constexpr uint32_t C_SET = 0x14, C_ACCESSORY = 0x1C, COORD_SIZE = 48, COORD_TEMP_ID = 7;
// AvatarParts (SafeBuffer): +0 id, +0x88 kind, +0x8C gender, +0x94 debug only, +0x98 category, +0x9C unique,
// +0xA0 default, +0xA4 set only, +0xA8 the set ids
constexpr uint32_t P_KIND = 0x88, P_GENDER = 0x8C, P_HIDDEN = 0x94, P_CATEGORY = 0x98, P_UNIQUE = 0x9C;
constexpr uint32_t P_DEFAULT = 0xA0, P_SET_ONLY = 0xA4, P_SETS = 0xA8;
constexpr uint32_t ACCESSORY_R0 = 0x30C;                  // generateAccessories' r0 (a leftover, unread)
constexpr uint32_t API_ERROR_SHOWN = 0x14;
constexpr uint32_t PHASE_AVATAR = 5;

uint32_t sbRead(uint32_t sb, uint32_t index, uint32_t n = 4) {
    Frame f;
    const uint32_t v = f.alloc(n < 4 ? 4 : n);
    call(SafeBuffer_read, {sb, v, ld32(sb + 0xC) + index, n, index});
    return ld32(v);
}

// AvatarParts row `id`: field at `index`
uint32_t partsField(uint32_t id, uint32_t index) {
    Frame f;
    const uint32_t row = masterRow(f, AvatarParts_create, id);
    const uint32_t v = sbRead(ld32(row), index);
    sharedRelease(row);
    return v;
}

// vector<int>::push_back(value) as libc++ inlines it
void pushInt(uint32_t vec, uint32_t valueSlot) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(vector_int_push_slow, {vec, valueSlot});
        return;
    }
    uint32_t base = 0;
    if (end) {
        st32(end, ld32(valueSlot));
        base = ld32(vec + 4);
    }
    st32(vec + 4, base + 4);
}

void labelOf(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

void runEasedOpen(uint32_t node) {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(Node_runAction, {node, call(EaseSineOut_create, {call(SceneUtil_createOpenPopupAnimation, {node, none})})});
    functionDestroy(none);
}

void okDialog(int32_t id) {
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t none = newEmptyFunction(f);
    call(SceneUtil_openOkDialog4Line, {s, none, 0});
    functionDestroy(none);
    strFree(s);
    sharedRelease(td);
}

// clear every accessory slot of `coord` that holds one of `parts` (vector<shared_ptr<AvatarParts>>)
void clearSlotsOf(uint32_t coordRef, uint32_t parts) {
    for (uint32_t slot = 0; slot != 5; slot++) {
        for (uint32_t k = 0; k < (ld32(parts + 4) - ld32(parts)) >> 3; k++) {
            const uint32_t worn = ld32(ld32(coordRef) + C_ACCESSORY + 4 * slot);
            const uint32_t id = sbRead(ld32(ld32(parts) + 8 * k), 0);
            if (worn == id) st32(ld32(coordRef) + C_ACCESSORY + 4 * slot, 0);
        }
    }
}
}  // namespace

namespace avatar_edit_rest {

// the "new" mark of tab `tab` (and, for the set and hair tabs, the ones after it): shown while one of the tab's
// parts is in the app's list of new parts; the mark's bit kept per gender
uint32_t updateTabNew(uint32_t self, uint32_t tab) {
    Frame f;
    const uint32_t ids = f.alloc(12);
    st32(ids, 0); st32(ids + 4, 0); st32(ids + 8, 0);
    {
        const uint32_t tmp = f.alloc(12);
        if (tab == 2) call(SAE_createFilterdAccessoryIDs, {tmp, self});
        else call(SAE_genderFiliteringIDs, {tmp, self, tab});
        call(vector_int_deallocate, {ids});
        st32(ids, ld32(tmp)); st32(ids + 4, ld32(tmp + 4)); st32(ids + 8, ld32(tmp + 8));
        st32(tmp + 8, 0); st32(tmp + 4, 0); st32(tmp, 0);
        call(vector_int_dtor, {tmp});
    }
    const uint32_t name = newStr(f, stringBuffer(f, {S_TabFmt, tab + 1}));
    const uint32_t tabNode = SceneUtil::searchNodeByName(ld32(self + F_ROOT), name);
    const uint32_t icon = findNode(tabNode, S_IconNew, 8);
    if (icon) {
        const uint32_t app = call(fn::AppState_getInstance, {});
        const uint32_t fresh = call(tab ? AppState_getNewAvatarPartsIDs : AppState_getNewAvatarPartsSetIDs, {app});
        uint32_t gender = 1, found = 0;
        for (uint32_t i = 0; i < (ld32(ids + 4) - ld32(ids)) >> 2; i++) {
            const uint32_t id = ld32(ld32(ids) + 4 * i);
            if (!i) gender = partsField(id, P_GENDER);
            uint32_t p = ld32(fresh);
            const uint32_t q = ld32(fresh + 4);
            while (p != q && ld32(p) != id) p += 4;
            if (p != q) { found = 1; break; }
        }
        vcall(icon, V_setVisible, {icon, found});
        const uint32_t word = ld32(self + (gender == 1 ? F_NEW_MALE : F_NEW_FEMALE)) + 4 * (tab >> 5);
        const uint32_t bit = 1u << (tab & 0x1F);
        st32(word, found == 1 ? (ld32(word) | bit) : (ld32(word) & ~bit));
    }
    if (tab == 1) call(SAE_updateTabNew, {self, 2});
    else if (tab == 0) call(SAE_updateTabNew, {self, 1});
    strFree(name);
    call(vector_int_dtor, {ids});
    return 0;
}
RW_FUNCTION(0x7076B0, "SceneAvatarEdit::updateTabNew", avatar_edit_rest::updateTabNew);

// the accessories of the current gender whose category filter is on
uint32_t createFilterdAccessoryIDs(uint32_t out, uint32_t self) {
    // category 1..10 -> the filter byte it reads (2 and 4 crossed)
    static const uint8_t FILTER_OF[10] = {0, 2, 3, 1, 4, 5, 6, 7, 8, 9};
    const uint32_t tabs = ld32(self + F_PART_IDS);
    const uint32_t gender = ld8(self + F_MALE) ? 1 : 2;
    if (((ld32(self + F_PART_IDS + 4) - tabs) >> 2) * 0xAAAAAAABu <= 2) call(ThrowOutOfRange, {self + F_PART_IDS});
    st32(out, 0); st32(out + 4, 0); st32(out + 8, 0);
    const uint32_t end = ld32(tabs + 0x1C);
    for (uint32_t p = ld32(tabs + 0x18); p != end; p += 4) {
        Frame f;
        const uint32_t slot = f.alloc(4);
        const uint32_t id = ld32(p);
        st32(slot, id);
        const uint32_t row = masterRow(f, AvatarParts_create, id);
        if (sbRead(ld32(row), P_GENDER) == gender) {
            const uint32_t category = sbRead(ld32(row), P_CATEGORY);
            if (category >= 1 && category <= 10 && ld8(self + F_FILTER + FILTER_OF[category - 1])) pushInt(out, slot);
        }
        sharedRelease(row);
    }
    return 0;
}
RW_FUNCTION(0x707950, "SceneAvatarEdit::createFilterdAccessoryIDs", avatar_edit_rest::createFilterdAccessoryIDs);

// the accessories worn by the current coordinate
uint32_t createAccessoryIDs(uint32_t out, uint32_t self) {
    st32(out, 0); st32(out + 4, 0); st32(out + 8, 0);
    for (uint32_t k = 0; k < 5; k++) {
        Frame f;
        const uint32_t slot = f.alloc(4);
        const uint32_t id = ld32(ld32(self + F_CURRENT) + C_ACCESSORY + 4 * k);
        st32(slot, id);
        if (id) pushInt(out, slot);
    }
    return 0;
}
RW_FUNCTION(0x707C60, "SceneAvatarEdit::createAccessoryIDs", avatar_edit_rest::createAccessoryIDs);

// the gender switch: the current coordinate's parts (+4..+0x2C) and the default coordinate's swap places. The
// original also stores an uninitialised stack word into the default coordinate's id (+0x310); not copied here.
uint32_t changeGender(uint32_t self, uint32_t male) {
    st8(self + F_MALE, uint8_t(male));
    const uint32_t c = ld32(self + F_CURRENT);
    uint32_t saved[11];
    for (uint32_t k = 0; k < 11; k++) saved[k] = ld32(c + 4 + 4 * k);
    st32(c + 4, ld32(self + F_DEFAULT + 4));
    st32(ld32(self + F_CURRENT) + 0x14, ld32(self + F_DEFAULT + 0x14));
    st32(ld32(self + F_CURRENT) + 0x18, ld32(self + F_DEFAULT + 0x18));
    st32(ld32(self + F_CURRENT) + 0x10, ld32(self + F_DEFAULT + 0x10));
    st32(ld32(self + F_CURRENT) + 0x08, ld32(self + F_DEFAULT + 0x08));
    st32(ld32(self + F_CURRENT) + 0x0C, ld32(self + F_DEFAULT + 0x0C));
    for (uint32_t k = 0; k < 5; k++) st32(ld32(self + F_CURRENT) + 0x1C + 4 * k, ld32(self + F_DEFAULT + 0x1C + 4 * k));
    st32(self + F_DEFAULT + 0x0C, saved[2]);
    st32(self + F_DEFAULT + 0x08, saved[1]);
    st32(self + F_DEFAULT + 0x04, saved[0]);
    st32(self + F_DEFAULT + 0x1C, saved[6]);
    st32(self + F_DEFAULT + 0x18, saved[5]);
    st32(self + F_DEFAULT + 0x14, saved[4]);
    st32(self + F_DEFAULT + 0x10, saved[3]);
    st32(self + F_DEFAULT + 0x2C, saved[10]);
    st32(self + F_DEFAULT + 0x28, saved[9]);
    st32(self + F_DEFAULT + 0x24, saved[8]);
    st32(self + F_DEFAULT + 0x20, saved[7]);
    call(SAE_changeMyCordinate, {self, ld32(self + F_COORD_INDEX)});
    call(SAE_changeTab, {self, ld32(self + F_TAB)});
    return 0;
}
RW_FUNCTION(0x707CF8, "SceneAvatarEdit::changeGender", avatar_edit_rest::changeGender);

// the accessory filter popup: ten category check boxes (edited on a copy of the filters), OK applies them
uint32_t openAccessoriesFilterPopup(uint32_t self) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = addShadow(layer, true);
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_FilterJson});
    centerOnMargins(root);
    vcall(shadow, V_addChild, {shadow, root});
    const uint32_t win = findNode(root, S_Win, 3);
    const uint32_t close = findNode(root, S_CloseButton, 0xC);
    const uint32_t ok = findNode(root, S_ButtonOK, 9);
    addTouch(close, VT_FilterClose, {});
    addTouch(ok, VT_FilterOk, {self});
    labelOf(ok, S_TxtOK, 6, 100100001);
    runEasedOpen(win);
    call(libc_memcpy, {self + F_FILTER_EDIT, self + F_FILTER, 10});
    for (uint32_t k = 0; k < 10; k++) {
        const uint32_t box = findNode(root, S_Filter01 + FILTER_STRIDE * k, 8);
        {
            Frame f;
            const uint32_t fo = newFunction(f, VT_Filter01 - 0x2C * k, {self});
            call(CheckBox_addEventListener, {box, fo});
            functionDestroy(fo);
        }
        labelOf(box, S_TxtFilter01 + FILTER_STRIDE * k, 0xC, 102510001 + int32_t(k));
        call(CheckBox_setSelectedState, {box, ld8(self + F_FILTER_EDIT + k)});
    }
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t canClose = newFunction(f, VT_FilterCanClose, {close});
    const uint32_t closed = f.alloc(20);
    const uint32_t obj = heapObject(VT_FilterClosed, 0x1C);
    if (obj) {
        const uint32_t words[6] = {close, ok, win, layer, shadow, root};
        for (uint32_t k = 0; k < 6; k++) st32(obj + 4 + 4 * k, words[k]);
    }
    st32(closed + 16, obj);
    call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
    functionDestroy(closed);
    functionDestroy(canClose);
    return 0;
}
RW_FUNCTION(0x707E40, "SceneAvatarEdit::openAccessoriesFilterPopup", avatar_edit_rest::openAccessoriesFilterPopup);

// wear (or with `remove` take off) accessory `partsId`; `isSet`: a whole set (its members, the set id kept at
// +0x14). A unique accessory clashing with the set shows "cannot combine"; more parts than free slots "no room".
uint32_t setAccessory(uint32_t self, uint32_t partsId, uint32_t remove, uint32_t isSet) {
    const uint32_t coordRef = self + F_CURRENT;
    uint32_t saved[5];
    for (uint32_t k = 0; k < 5; k++) saved[k] = ld32(ld32(coordRef) + C_ACCESSORY + 4 * k);
    Frame f;
    const uint32_t parts = f.alloc(12);
    st32(parts, 0); st32(parts + 4, 0); st32(parts + 8, 0);
    call(SAE_generateAccessories, {ACCESSORY_R0, partsId, parts, isSet});
    if (remove == 1) {
        clearSlotsOf(coordRef, parts);
        call(SAE_updateCost, {self});
        call(vector_parts_dtor, {parts});
        return 0;
    }
    if (isSet) {
        Frame g;
        const uint32_t old = g.alloc(12);
        st32(old, 0); st32(old + 4, 0); st32(old + 8, 0);
        const uint32_t c = ld32(coordRef);
        call(SAE_generateAccessories, {c, ld32(c + C_SET), old, 1});
        clearSlotsOf(coordRef, old);
        call(vector_parts_dtor, {old});
    }
    for (uint32_t slot = 0; slot < 5; slot++) {
        const uint32_t at = ld32(coordRef) + 4 * slot;
        const uint32_t worn = ld32(at + C_ACCESSORY);
        if (!worn) continue;
        Frame g;
        const uint32_t wornParts = g.alloc(12);
        st32(wornParts, 0); st32(wornParts + 4, 0); st32(wornParts + 8, 0);
        call(SAE_generateAccessories, {at, worn, wornParts, 0});
        bool refused = false;
        for (uint32_t a = 0; a < (ld32(wornParts + 4) - ld32(wornParts)) >> 3 && !refused; a++) {
            const uint32_t aSb = ld32(ld32(wornParts) + 8 * a);
            const uint32_t aCtrl = ld32(ld32(wornParts) + 8 * a + 4);
            if (aCtrl) call(shared_addRef, {aCtrl});
            for (uint32_t b = 0; b < (ld32(parts + 4) - ld32(parts)) >> 3; b++) {
                const uint32_t bSb = ld32(ld32(parts) + 8 * b);
                const uint32_t bCtrl = ld32(ld32(parts) + 8 * b + 4);
                if (bCtrl) call(shared_addRef, {bCtrl});
                const uint32_t newCategory = sbRead(bSb, P_CATEGORY);
                const uint32_t wornCategory = sbRead(aSb, P_CATEGORY);
                bool stop = false;
                if (newCategory == wornCategory) {
                    if (sbRead(aSb, P_UNIQUE) != 1 && sbRead(aSb, P_SET_ONLY) == 1 &&
                        ld32(ld32(coordRef) + C_SET) == sbRead(aSb, P_SETS, 0x14)) {
                        okDialog(102500024);
                        stop = true;
                        for (uint32_t k = 0; k < 5; k++) st32(ld32(coordRef) + C_ACCESSORY + 4 * k, saved[k]);
                    } else {
                        clearSlotsOf(coordRef, wornParts);
                    }
                }
                if (bCtrl) call(fn::shared_release, {bCtrl});
                if (stop) { refused = true; break; }
            }
            if (aCtrl) call(fn::shared_release, {aCtrl});
        }
        call(vector_parts_dtor, {wornParts});
        if (refused) {
            call(vector_parts_dtor, {parts});
            return 0;
        }
    }
    uint32_t empty = 0;
    for (uint32_t k = 0; k < 5; k++)
        if (!ld32(ld32(coordRef) + C_ACCESSORY + 4 * k)) empty++;
    if (((ld32(parts + 4) - ld32(parts)) >> 3) <= empty) {
        uint32_t next = 0;
        for (uint32_t k = 0; k < 5; k++) {
            if (ld32(ld32(coordRef) + C_ACCESSORY + 4 * k)) continue;
            if (((ld32(parts + 4) - ld32(parts)) >> 3) <= next) break;
            const uint32_t id = sbRead(ld32(ld32(parts) + 8 * next), 0);
            st32(ld32(coordRef) + C_ACCESSORY + 4 * k, id);
            next++;
        }
        call(SAE_updateCost, {self});
        if (isSet == 1) st32(ld32(coordRef) + C_SET, partsId);
    } else {
        okDialog(102500016);
        for (uint32_t k = 0; k < 5; k++) st32(ld32(coordRef) + C_ACCESSORY + 4 * k, saved[k]);
        call(AvatarPartsScrollView_setSelectedID, {ld32(self + F_SCROLL), ld32(ld32(coordRef) + C_SET)});
    }
    call(vector_parts_dtor, {parts});
    return 0;
}
RW_FUNCTION(0x708FE8, "SceneAvatarEdit::setAccessory", avatar_edit_rest::setAccessory);

// "Save this coordinate?": the coordinate on an avatar, a tap anywhere (the tap animation) saves
uint32_t openSavePopup(uint32_t self) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = addShadow(layer, true);
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_SaveJson});
    centerOnMargins(root);
    vcall(shadow, V_addChild, {shadow, root});
    const uint32_t win = findNode(root, S_Win04, 6);
    labelOf(win, S_TxtWording, 0xB, 102500026);
    {
        const uint32_t mgr = call(ArmatureDataManager_getInstance, {});
        Frame f;
        const uint32_t s = newStr(f, S_TapArmatureFile, 0x32);
        call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        strFree(s);
    }
    const uint32_t area = findNode(root, S_TapArea, 0xF);
    uint32_t armature;
    {
        Frame f;
        const uint32_t s = newStr(f, S_TapArmature, 0x14);
        armature = call(Armature_create, {s});
        strFree(s);
    }
    vcall(area, V_addChild, {area, armature});
    {
        const uint32_t anim = vcall(armature, V_getAnimation, {armature});
        Frame f;
        const uint32_t s = newStr(f, S_Animation1, 0xA);
        vcall(anim, V_playAnimation, {anim, s, 0xFFFFFFFF, 0xFFFFFFFF});
        strFree(s);
    }
    uint32_t button;
    {
        Frame f;
        const uint32_t s = newStr(f, S_Empty, 0);
        button = call(ImageButton_create, {s, 0});
        strFree(s);
    }
    call(ImageButton_setTargetView, {button, root, fbits(1.0f)});
    {
        Frame f;
        const uint32_t s = newStr(f, S_CenterButton, 0xC);
        vcall(root, V_addChildZName, {root, button, 0, s});
        strFree(s);
    }
    {
        Frame f;
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapObject(VT_SaveTouch, 0x14);
        if (obj) {
            st32(obj + 4, win);
            st32(obj + 8, layer);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, root);
        }
        st32(fo + 16, obj);
        call(ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    const uint32_t spot = findNode(win, S_Avatar, 6);
    const uint32_t actor = call(AvatarActor_create, {0, 1});
    const uint32_t kb = call(SAE_getEquipKeybladeID, {self, ld8(self + F_SHOW_KEYBLADE)});
    const uint32_t coords = ld32(self + F_COORDS);
    const uint32_t index = ld32(self + F_COORD_INDEX);
    if (((ld32(self + F_COORDS + 4) - coords) >> 4) * 0xAAAAAAABu <= index) return call(ThrowOutOfRange, {self + F_COORDS});
    {
        Frame f;
        const uint32_t style = f.alloc(0x40);
        call(AvatarUtil_makeStyle, {style, coords + COORD_SIZE * index, kb});
        call(AvatarActor_setStyle, {actor, style});
        call(SafeBuffer_dtor, {style});
    }
    vcall(actor, V_setVisible, {actor, 1});
    vcall(actor, V_setScale, {actor, 0x40000000});
    {
        const float x = bitsf(vcall(spot, V_getPositionX, {spot}));
        const float y1 = bitsf(vcall(spot, V_getPositionY, {spot}));
        const float y2 = bitsf(vcall(spot, V_getPositionY, {spot}));
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits((y1 + y2 * -0.5f) + -50.0f)});
        vcall(actor, V_setPosition, {actor, pos});
        call(Vec2_dtor, {pos});
    }
    call(Avatar_preloadResources, {ld32(actor + 0x288)});
    {
        const uint32_t parent = vcall(spot, V_getParent, {spot});
        vcall(parent, V_addChildZ, {parent, actor, vcall(spot, V_getLocalZOrder, {spot})});
    }
    vcall(spot, V_removeFromParent, {spot});
    runEasedOpen(win);
    return 0;
}
RW_FUNCTION(0x709DF0, "SceneAvatarEdit::openSavePopup", avatar_edit_rest::openSavePopup);

// the coordinates sent (the temporary one, id 7, dropped first) as the selected slot; `answer` on the reply
uint32_t saveMyCoordinate(uint32_t self, uint32_t answer) {
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, answer});
    const uint32_t begin = ld32(self + F_COORDS);
    const uint32_t end = ld32(self + F_COORDS + 4);
    const uint32_t count = ((end - begin) >> 4) * 0xAAAAAAABu;
    for (uint32_t i = 0; i < count; i++) {
        const uint32_t pos = begin + COORD_SIZE * i;
        if (ld32(pos) != COORD_TEMP_ID) continue;
        const uint32_t n = end - (pos + COORD_SIZE);
        call(aeabi_memmove, {pos, pos + COORD_SIZE, n});
        if (end != pos + n) st32(self + F_COORDS + 4, pos + n);
        break;
    }
    call(SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t slot = ld32(self + F_COORD_INDEX) + 1;
    const uint32_t inner = f.alloc(20);
    functionCopy(inner, copy);
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(VT_Saved, 0x20);
    if (obj) call(function_vb_move, {obj + 8, inner});
    st32(fo + 16, obj);
    call(APIManager_executeUserAvatarUpdate, {api, self + F_COORDS, slot, fo});
    functionDestroy(fo);
    functionDestroy(inner);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x70A504, "SceneAvatarEdit::saveMyCoordinate", avatar_edit_rest::saveMyCoordinate);

// the save answer: the header's avatar icon redrawn and the answer with true; failed: the error dialog
uint32_t saved(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        call(SM_updateHeaderAvatarIcon, {sceneManager(), 0, 0xFFFFFFFF});
        call(function_vb_call, {functor + 8, 1});
    } else if (ld8(call(fn::APIManager_getInstance, {}) + API_ERROR_SHOWN)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_SaveError, {});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    return 0;
}
RW_FUNCTION(0x70AAF0, "SceneAvatarEdit::saveMyCoordinate answer operator()(bool) (sub_70AAF0)", avatar_edit_rest::saved);

// the save popup tapped: it closes (the window's animation, the shadow fading out)
uint32_t saveTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t win = ld32(functor + 4);
    Frame f;
    const uint32_t done = newFunction(f, VT_SaveCloseEnd, {ld32(functor + 8), ld32(functor + 0xC), ld32(functor + 0x10)});
    call(Node_runAction, {win, call(EaseSineOut_create, {call(SceneUtil_createClosePopupAnimation, {win, done})})});
    functionDestroy(done);
    const uint32_t shadow = ld32(functor + 0xC);
    call(Node_runAction, {shadow, call(FadeTo_create, {0x3DCCCCCD, 0})});
    return 0;
}
RW_FUNCTION(0x70AD04, "SceneAvatarEdit::openSavePopup touch operator() (sub_70AD04)", avatar_edit_rest::saveTouched);

// an answer: on true the user data again
uint32_t refreshUser(uint32_t, uint32_t okRef) {
    if (!ld8(okRef)) return 0;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(APIManager_executeUserGet, {api, none});
    functionDestroy(none);
    return 0;
}
RW_FUNCTION(0x70B3D8, "SceneAvatarEdit user get operator()(bool) (sub_70B3D8)", avatar_edit_rest::refreshUser);

// the avatar progress retry: when the request retries by itself, on as if sent; else the avatar sent again
uint32_t avatarRetry(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    const uint32_t retrying = call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})});
    if (retrying) {
        call(AvatarSent, {retrying});
        return 0;
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t avatar = ld32(self + F_CURRENT);
    Frame f;
    const uint32_t sent = newFunction(f, VT_AvatarSent, {ld8(functor + 8)});
    call(APIManager_executeTutorialProgress, {api, PHASE_AVATAR, 0, 0, 0, 0, 0, avatar, sent});
    functionDestroy(sent);
    return 0;
}
RW_FUNCTION(0x70B504, "SceneAvatarEdit::openCreateCheckPopup retry operator() (sub_70B504)", avatar_edit_rest::avatarRetry);

// debug builds: every listed part sorted into the tab lists by kind (not called in the release)
uint32_t initDebugAvatarPartsList(uint32_t self) {
    // kind -> tab vector index (kind 2 male: tab 0, female: tabs 0 and 1)
    Frame f;
    const uint32_t ids = f.alloc(12);                      // std::set<int>
    call(AvatarParts_getAvatarPartsIds, {ids});
    const uint32_t header = ids + 4;
    uint32_t node = ld32(ids);
    while (node != header) {
        Frame g;
        const uint32_t slot = g.alloc(4);
        const uint32_t id = ld32(node + 0x10);
        st32(slot, id);
        const uint32_t row = masterRow(g, AvatarParts_create, id);
        const uint32_t kind = sbRead(ld32(row), P_KIND);
        const uint32_t deflt = sbRead(ld32(row), P_DEFAULT);
        if ((kind == 2 || deflt != 1) &&
            (sbRead(ld32(row), P_UNIQUE) != 1 || sbRead(ld32(row), P_HIDDEN) != 1)) {
            const uint32_t tabs = self + F_PART_IDS;
            auto tabVec = [&](uint32_t t) {
                const uint32_t b = ld32(tabs);
                if (((ld32(tabs + 4) - b) >> 2) * 0xAAAAAAABu <= t) call(ThrowOutOfRange, {tabs});
                return b + 12 * t;
            };
            switch (kind) {
            case 2:
                if (deflt == 1) {
                    if (ld32(tabs + 4) == ld32(tabs)) call(ThrowOutOfRange, {tabs});
                    pushInt(ld32(tabs), slot);
                } else {
                    if (ld32(tabs + 4) == ld32(tabs)) call(ThrowOutOfRange, {tabs});
                    pushInt(ld32(tabs), slot);
                    pushInt(tabVec(1), slot);
                }
                break;
            case 3: pushInt(tabVec(3), slot); break;
            case 4: pushInt(tabVec(5), slot); break;
            case 5: pushInt(tabVec(6), slot); break;
            case 6: pushInt(tabVec(4), slot); break;
            case 7: pushInt(tabVec(2), slot); break;
            default: break;
            }
        }
        sharedRelease(row);
        // ++iterator (tree successor)
        uint32_t right = ld32(node + 4);
        if (right) {
            while (ld32(right)) right = ld32(right);
            node = right;
        } else {
            uint32_t up = node;
            do {
                node = ld32(up + 8);
                const bool fromLeft = ld32(node) == up;
                up = node;
                if (fromLeft) break;
            } while (true);
        }
    }
    call(tree_int_destroy, {ids, ld32(ids + 4)});
    return 0;
}
RW_FUNCTION(0x706C8C, "SceneAvatarEdit::initDebugAvatarPartsList", avatar_edit_rest::initDebugAvatarPartsList);

}  // namespace avatar_edit_rest
