// SceneManager: the tutorial checks / chains run from the global menu, the no-tap screen, the scene run-out
// animation and the face image url cache.
//
// Most functions here reproduce the original frame (Frame::allocExact with the original push + sub size) and
// call with the original sp (rw::call_exact): openTutorialPopup (TutorialMask) reads a stale stack word relative
// to its entry sp, tutorial_menu hands openTutorialPopup a lambda with an uninitialised capture word, and
// isTutorial18 passes KeyBladeDataManager::getKeyBladeData a by-value Keyblade with uninitialised bytes.
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t APIManager_userData = 0x94;      // shared_ptr<UserData> (SafeBuffer)
constexpr uint32_t APIManager_userPoint = 0xA4;     // shared_ptr<UserPoint> (SafeBuffer: paid 0, free 4)
constexpr uint32_t APIManager_userMedals = 0xD4;    // shared_ptr<UserMedal> (SafeBuffer of 72-byte medals)
constexpr uint32_t APIManager_userMaterials = 0xE4; // shared_ptr (SafeBuffer of 16-byte records: id +8, count +12)
constexpr uint32_t APIManager_userKeyblades = 0xEC; // shared_ptr<UserKeyblade> (SafeBuffer of 88-byte keyblades)
constexpr uint32_t SB_DATA = 0x0C, SB_COUNT = 0x20; // SafeBuffer: data pointer, record count (lists)
constexpr uint32_t UserMedal_index = 0x24;          // unordered_map<u64 medal id, int slot>
constexpr uint32_t MEDAL_SIZE = 72, KEYBLADE_SIZE = 88, MATERIAL_SIZE = 16;
constexpr uint32_t UserKeyBladeData_row = 0x20;     // shared_ptr<master::Keyblade> (SafeBuffer)
// master::Keyblade: level-up material count 0x13C, max level 0x140, material ids 0x144[4], amounts 0x154[4]
constexpr uint32_t KB_MATERIAL_KINDS = 0x13C, KB_MAX_LEVEL = 0x140, KB_MATERIAL_IDS = 0x144, KB_MATERIAL_NEED = 0x154;
constexpr uint32_t KEYBLADE_ID = 0x18;              // AvatarUtil::getCurrentKeyblade result: user keyblade id (u64)

constexpr uint32_t SafeBuffer_read = 0x45A269;      // (this, dst, src, n, index)
constexpr uint32_t AvatarUtil_getCurrentKeyblade = 0x4976FD;
constexpr uint32_t MedalIndex_find = 0x490275;
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D;
constexpr uint32_t KeyBladeDataManager_getKeyBladeData = 0x76B659; // (out, this, u64 id, uint, Keyblade by value)
constexpr uint32_t Memset = 0x40E7A0;               // __aeabi_memset(dst, n, value)
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815;
constexpr uint32_t AppState_resetClearTutorialPopupIDs = 0x4EB8B5;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t SceneUtil_updateTutorialPhase = 0x472BA1;
constexpr uint32_t SceneUtil_CompleteTutorialPhase = 0x472A75;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t DynamicCast = 0xC9A171;          // __dynamic_cast(src, srcType, dstType, hint)
constexpr uint32_t TI_Layer = 0xFBE720, TI_IInOutAnimation = 0xF98B74;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t FaceMap_destroy = 0x622035;      // __tree::destroy(node)
constexpr uint32_t FaceMap_index = 0x621C89;        // map<string, string>::operator[]
constexpr uint32_t String_assign = 0x498A1D;
constexpr uint32_t SceneManager_D2 = 0x60C5E1;

constexpr uint32_t STR_CenterUI = 0xE02FBA, STR_MenuButton = 0xE02E00, STR_Empty = 0xDF8A1A;
constexpr uint32_t MISC_TUTORIAL_SHOP_JEWELS = 403;
constexpr uint32_t FACE_URL_LIMIT = 200;            // the cache is emptied when it holds more
constexpr uint32_t NO_TAP_SHADOW = 0x4B8;           // SceneManager: the shadow sprite of openNoTapScreen

// lambdas (vtable of the __func + 8)
constexpr uint32_t VT_Tutorial12_Complete = 0xF602C4;  // Tutorial12Exec $_31
constexpr uint32_t VT_Shop_Next = 0xF5EFB0;            // ChainOfTutorialShop $_120
constexpr uint32_t VT_Tutorial15_Next = 0xF60298;      // Tutorial15Exec $_32
constexpr uint32_t VT_DeckEdit_Next = 0xF5EFDC;        // ChainOfTutorialDeckEdit $_119
constexpr uint32_t VT_Menu_Next = 0xF60214;            // tutorial_menu $_33 [centerUI, ?, menuButton]
constexpr uint32_t VT_Adventure_Next220 = 0xF5F0B8;    // ChainOfTutorialAdventureSelect $_116
constexpr uint32_t VT_Adventure_Next = 0xF5F034;       // $_117
constexpr uint32_t VT_Sphere_Next = 0xF5F008;          // ChainOfTutorialSphere $_118
constexpr uint32_t VT_NoTap_Close = 0xF5F4AC;          // openNoTapScreen $_102 [this]
constexpr uint32_t VT_RunOut_Done = 0xF5F0E4;          // callCurrentSceneRunOutAnimation $_115 [callback] (heap)

uint32_t cx(uint32_t fn, std::initializer_list<uint32_t> args) { return uint32_t(rw::call_exact(fn, args)); }
uint32_t mgrAddr(SceneManager *self) { return rw::guest_addr(self); }
uint32_t api() { return cx(fn::APIManager_getInstance, {}); }
bool tutorialPopupCleared(uint32_t id) {
    return cx(AppState_isClearTutorialPopupIDs, {cx(fn::AppState_getInstance, {}), id}) != 0;
}
void addRef(uint32_t ctl) { if (ctl) cx(fn::shared_add, {ctl}); }
void release(uint32_t ctl) { if (ctl) cx(fn::shared_release, {ctl}); }
// the inline std::function destructor with the original call
void destroyFn(uint32_t f) {
    uint32_t p = ld32(f + 16);
    if (p == f) cx(ld32(ld32(p) + 0x10), {p});
    else if (p) cx(ld32(ld32(p) + 0x14), {p});
}
uint32_t readSafe32(uint32_t sb, uint32_t dst, uint32_t index) {
    cx(SafeBuffer_read, {sb, dst, ld32(sb + SB_DATA) + index, 4, index});
    return ld32(dst);
}
// an inline lambda without captures: [vtable] .. __f_ at +16
void inlineLambda(uint32_t f, uint32_t vtable) {
    st32(f + 16, f);
    st32(f, vtable);
}
// openTutorialPopup(result, id, callback, nullptr, nullptr, nullptr, false), then destroy result and callback
void openTutorial(uint32_t result, uint32_t id, uint32_t callback) {
    cx(SceneUtil_openTutorialPopup, {result, id, callback, 0, 0, 0, 0});
    destroyFn(result);
    destroyFn(callback);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ checks

// Tutorial 12 (the shop) applies while the medal box holds at most 3 medals and the user has 300+ jewels.
bool SceneManager::isTutorial12(SceneManager *) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x38);
    uint32_t a = api();
    uint32_t medalsCtl = ld32(a + APIManager_userMedals + 4), medals = ld32(a + APIManager_userMedals);
    addRef(medalsCtl);
    bool result = false;
    if (ld32(medals + SB_COUNT) <= 3) {
        uint32_t b = api();
        uint32_t pointCtl = ld32(b + APIManager_userPoint + 4), point = ld32(b + APIManager_userPoint);
        addRef(pointCtl);
        uint32_t paid = readSafe32(point, base + 0x1C, 0);
        uint32_t free = readSafe32(point, base + 0x18, 4);
        result = int32_t(free + paid) > 299;
        release(pointCtl);
    }
    release(medalsCtl);
    return result;
}
RW_FUNCTION(0x6132DC, "SceneManager::isTutorial12", SceneManager::isTutorial12);

// Tutorial 15 (deck edit) applies when the user owns one of the tutorial medals and a keyblade is equipped.
bool SceneManager::isTutorial15(SceneManager *) {
    Frame fr;
    const uint32_t base = fr.allocExact(0xD8);
    const uint32_t keyblade = base + 0x18, medal = base + 0x70;
    uint32_t a = api();
    uint32_t medalsCtl = ld32(a + APIManager_userMedals + 4), medals = ld32(a + APIManager_userMedals);
    addRef(medalsCtl);
    uint32_t count = ld32(medals + SB_COUNT);
    bool result = false;
    if (count) {
        uint32_t flag = 0;
        for (uint32_t i = 0, off = 0; i < count; i++, off += MEDAL_SIZE) {
            cx(SafeBuffer_read, {medals, medal, ld32(medals + SB_DATA) + off, MEDAL_SIZE, off});
            uint32_t id = ld32(medal + 8);
            uint32_t k = id - 90021;
            if (k < 0x19 && ((0x150D435u >> k) & 1)) flag = 1;
            else flag |= id == 90046 ? 1 : 0;
        }
        if (flag & 1) {
            cx(AvatarUtil_getCurrentKeyblade, {keyblade});
            result = (ld32(keyblade + KEYBLADE_ID) | ld32(keyblade + KEYBLADE_ID + 4)) != 0;
        }
    }
    release(medalsCtl);
    return result;
}
RW_FUNCTION(0x6133E0, "SceneManager::isTutorial15", SceneManager::isTutorial15);

// False while the current keyblade's medal (looked up in the medal box) is medal 33043.
bool SceneManager::isTutorial36(SceneManager *) {
    Frame fr;
    const uint32_t base = fr.allocExact(0xD0);
    const uint32_t key = base + 8, medal = base + 0x10, keyblade = base + 0x58;
    uint32_t a = api();
    cx(AvatarUtil_getCurrentKeyblade, {keyblade});
    uint32_t medals = ld32(a + APIManager_userMedals);
    st32(key + 4, ld32(keyblade + KEYBLADE_ID + 4));
    st32(key, ld32(keyblade + KEYBLADE_ID));
    uint32_t node = cx(MedalIndex_find, {medals + UserMedal_index, key});
    uint32_t off = node ? MEDAL_SIZE * ld32(node + 0x10) : uint32_t(-int32_t(MEDAL_SIZE));
    cx(SafeBuffer_read, {medals, medal, ld32(medals + SB_DATA) + off, MEDAL_SIZE, off});
    return ld32(medal + 8) != 33043;
}
RW_FUNCTION(0x6134F4, "SceneManager::isTutorial36", SceneManager::isTutorial36);

// Tutorial 18 (keyblade level up) applies when some owned keyblade has a level-up material list whose every
// material the user owns in the needed amount.
bool SceneManager::isTutorial18(SceneManager *) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x1A0);
    const uint32_t material = base + 0x90, data = base + 0xA0, maxLevel = base + 0xA8, kinds = base + 0xAC;
    const uint32_t need = base + 0xB0, ids = base + 0xC0, arg = base + 0xD0, record = base + 0x128;
    uint32_t a = api();
    const uint32_t keyblades = a + APIManager_userKeyblades;
    uint32_t list = ld32(keyblades);
    uint32_t found = 0;
    if (ld32(list + SB_COUNT) == 0) return false;
    for (uint32_t i = 0;;) {
        uint32_t off = KEYBLADE_SIZE * i;
        cx(SafeBuffer_read, {list, record, ld32(list + SB_DATA) + off, KEYBLADE_SIZE, off});
        uint32_t manager = cx(KeyBladeDataManager_getInstance, {});
        uint32_t idLo = ld32(record), idHi = ld32(record + 4);
        // the by-value Keyblade argument: built at base+0xD0 (partly zeroed, the rest stale) and copied to sp+8
        st32(base + 0x120, 0);
        cx(Memset, {arg, 0x14, 0});
        cx(Memset, {arg + 0x18, 0x35, 0});
        for (uint32_t k = 0; k < KEYBLADE_SIZE; k += 4) st32(base + 8 + k, ld32(arg + k));
        cx(KeyBladeDataManager_getKeyBladeData, {data, manager, idLo, idHi, 0});
        uint32_t row = ld32(ld32(data) + UserKeyBladeData_row);
        cx(SafeBuffer_read, {row, kinds, ld32(row + SB_DATA) + KB_MATERIAL_KINDS, 4, KB_MATERIAL_KINDS});
        uint32_t done;
        if (ld32(kinds) == 0) {
            done = 0;
        } else {
            int32_t j = 0;
            do {
                row = ld32(ld32(data) + UserKeyBladeData_row);
                cx(SafeBuffer_read, {row, maxLevel, ld32(row + SB_DATA) + KB_MAX_LEVEL, 4, KB_MAX_LEVEL});
                if (int32_t(ld32(maxLevel)) > j) {
                    row = ld32(ld32(data) + UserKeyBladeData_row);
                    cx(SafeBuffer_read, {row, ids, ld32(row + SB_DATA) + KB_MATERIAL_IDS, 16, KB_MATERIAL_IDS});
                    uint32_t id = ld32(ids + 4 * uint32_t(j));
                    uint32_t b = api();
                    const uint32_t materials = b + APIManager_userMaterials;
                    uint32_t mats = ld32(materials);
                    int32_t owned = 0;
                    for (uint32_t m = 0, moff = 0; m < ld32(mats + SB_COUNT); moff += MATERIAL_SIZE) {
                        cx(SafeBuffer_read, {mats, material, ld32(mats + SB_DATA) + moff, MATERIAL_SIZE, moff});
                        m++;
                        if (ld32(material + 8) == id) {
                            owned = int32_t(ld32(material + 12));
                            break;
                        }
                        mats = ld32(materials);
                    }
                    row = ld32(ld32(data) + UserKeyBladeData_row);
                    cx(SafeBuffer_read, {row, need, ld32(row + SB_DATA) + KB_MATERIAL_NEED, 16, KB_MATERIAL_NEED});
                    done = 0;
                    if (owned < int32_t(ld32(need + 4 * uint32_t(j)))) break;
                }
                j++;
                done = 1;
            } while (j < 4);
            found |= done;
        }
        release(ld32(data + 4));
        i++;
        if (done == 1) break;
        list = ld32(keyblades);
        if (i >= ld32(list + SB_COUNT)) break;
    }
    return (found & 1) != 0;
}
RW_FUNCTION(0x61358C, "SceneManager::isTutorial18", SceneManager::isTutorial18);

// ------------------------------------------------------------------------------------------------ chains

// The shop tutorial (12) when it applies; otherwise complete phase 220 directly (or, after tutorial 36, the
// current phase).
void SceneManager::Tutorial12Exec(SceneManager *, Addr callback) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x58);
    bool shop = isTutorial12(getInstance());
    SceneManager *mgr = getInstance();
    if (shop) {
        ChainOfTutorialShop(mgr, 12);
        return;
    }
    if (isTutorial36(mgr)) {
        const uint32_t done = base + 0x20;
        inlineLambda(done, VT_Tutorial12_Complete);
        cx(SceneUtil_CompleteTutorialPhase, {done, 0, 1});
        destroyFn(done);
        return;
    }
    const uint32_t copy = base + 8;
    cx(fn::function_vv_copy, {copy, callback});
    cx(SceneUtil_updateTutorialPhase, {220, copy, 0, 1});
    destroyFn(copy);
    cx(AppState_resetClearTutorialPopupIDs, {cx(fn::AppState_getInstance, {})});
}
RW_FUNCTION(0x61379C, "SceneManager::Tutorial12Exec", SceneManager::Tutorial12Exec);

// Open the shop tutorial popup `id` unless already shown; tutorial 12 needs the jewels of master Misc 403.
bool SceneManager::ChainOfTutorialShop(SceneManager *, uint32_t id) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x80);
    if (tutorialPopupCleared(id)) return false;
    if (id == 12) {
        uint32_t a = api();
        uint32_t pointCtl = ld32(a + APIManager_userPoint + 4), point = ld32(a + APIManager_userPoint);
        addRef(pointCtl);
        uint32_t paid = readSafe32(point, base + 0x24, 0);
        uint32_t free = readSafe32(point, base + 0x28, 4);
        int32_t jewels = int32_t(free + paid);
        const uint32_t misc = base + 0x1C;
        cx(fn::Misc_create, {misc, MISC_TUTORIAL_SHOP_JEWELS});
        int32_t needed = int32_t(readSafe32(ld32(misc), base + 0x2C, 4));
        release(ld32(misc + 4));
        release(pointCtl);
        if (needed > jewels) return false;
    }
    const uint32_t next = base + 0x30;
    inlineLambda(next, VT_Shop_Next);
    openTutorial(base + 0x48, id, next);
    return true;
}
RW_FUNCTION(0x6138AC, "SceneManager::ChainOfTutorialShop", SceneManager::ChainOfTutorialShop);

void SceneManager::Tutorial15Exec(SceneManager *) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x38);
    if (isTutorial15(getInstance())) {
        ChainOfTutorialDeckEdit(getInstance(), 15);
        return;
    }
    const uint32_t next = base + 8;
    inlineLambda(next, VT_Tutorial15_Next);
    cx(SceneUtil_updateTutorialPhase, {700, next, 0, 1});
    destroyFn(next);
    cx(AppState_resetClearTutorialPopupIDs, {cx(fn::AppState_getInstance, {})});
}
RW_FUNCTION(0x613A44, "SceneManager::Tutorial15Exec", SceneManager::Tutorial15Exec);

bool SceneManager::ChainOfTutorialDeckEdit(SceneManager *, uint32_t id) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x60);
    if (tutorialPopupCleared(id)) return false;
    const uint32_t next = base + 0x10;
    inlineLambda(next, VT_DeckEdit_Next);
    openTutorial(base + 0x28, id, next);
    return true;
}
RW_FUNCTION(0x613B04, "SceneManager::ChainOfTutorialDeckEdit", SceneManager::ChainOfTutorialDeckEdit);

void SceneManager::Tutorial18Exec(SceneManager *, Addr callback) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x40);
    if (isTutorial18(getInstance())) {
        ChainOfTutorialDeckEdit(getInstance(), 18);
        return;
    }
    const uint32_t copy = base + 8;
    cx(fn::function_vv_copy, {copy, callback});
    cx(SceneUtil_updateTutorialPhase, {410, copy, 0, 1});
    destroyFn(copy);
    cx(AppState_resetClearTutorialPopupIDs, {cx(fn::AppState_getInstance, {})});
}
RW_FUNCTION(0x613BE4, "SceneManager::Tutorial18Exec", SceneManager::Tutorial18Exec);

// The global menu tutorial (38) once popups 100 and 105 were shown; its lambda keeps the header's CenterUI and
// MenuButton (the capture word between them is never written: stale stack).
void SceneManager::tutorial_menu(SceneManager *self) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x78);
    uint32_t header = self->header;
    if (!header) return;
    const uint32_t name1 = base + 0x1C, name2 = base + 0x10;
    cx(fn::string_init, {name1, STR_CenterUI, 8});
    uint32_t center = cx(SceneUtil_searchNodeByName, {header, name1});
    if (ld8(name1) & 1) cx(fn::operator_delete, {ld32(name1 + 8)});
    cx(fn::string_init, {name2, STR_MenuButton, 10});
    uint32_t button = cx(SceneUtil_searchNodeByName, {center, name2});
    if (ld8(name2) & 1) cx(fn::operator_delete, {ld32(name2 + 8)});
    if (cx(AppState_isClearTutorialPopupIDs, {cx(fn::AppState_getInstance, {}), 100}) != 1) return;
    if (!tutorialPopupCleared(105)) return;
    const uint32_t next = base + 0x28;
    st32(next + 16, next);
    st32(next, VT_Menu_Next);
    st32(next + 4, center);
    st32(next + 12, button);
    openTutorial(base + 0x40, 38, next);
}
RW_FUNCTION(0x613CA0, "SceneManager::tutorial_menu", SceneManager::tutorial_menu);

bool SceneManager::ChainOfTutorialAdventureSelect(SceneManager *, uint32_t id) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x90);
    if (tutorialPopupCleared(id)) return false;
    if ((id == 37 || id == 42) && cx(0x472B31, {}) == 220) {   // SceneUtil::getNexTutorialphase
        const uint32_t next = base + 0x40;
        inlineLambda(next, VT_Adventure_Next220);
        openTutorial(base + 0x58, id, next);
    } else {
        const uint32_t next = base + 0x10;
        inlineLambda(next, VT_Adventure_Next);
        openTutorial(base + 0x28, id, next);
    }
    return true;
}
RW_FUNCTION(0x6219E0, "SceneManager::ChainOfTutorialAdventureSelect", SceneManager::ChainOfTutorialAdventureSelect);

// The avatar board tutorial (16) from Lux rank 2 on; true unless the popup was already shown.
bool SceneManager::ChainOfTutorialSphere(SceneManager *, uint32_t id) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x60);
    if (tutorialPopupCleared(id)) return false;
    uint32_t userData = ld32(api() + APIManager_userData);
    if (int32_t(readSafe32(userData, base + 0x14, 8)) < 2) return true;
    const uint32_t next = base + 0x18;
    inlineLambda(next, VT_Sphere_Next);
    openTutorial(base + 0x30, 16, next);
    return true;
}
RW_FUNCTION(0x621B30, "SceneManager::ChainOfTutorialSphere", SceneManager::ChainOfTutorialSphere);

// ------------------------------------------------------------------------------------------------ misc

// Block touches: an invisible shadow sprite on popup layer 3, removed after `delay` seconds.
void SceneManager::openNoTapScreen(SceneManager *self, float delay) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x50);
    uint32_t m = mgrAddr(self);
    uint32_t layer = getInstance()->popupLayer3;
    uint32_t shadow = cx(SceneUtil_createShadowSprite, {});
    st32(m + NO_TAP_SHADOW, shadow);
    cx(ld32(ld32(shadow) + node::setOpacity), {shadow, 0});
    cx(ld32(ld32(layer) + node::addChild), {layer, ld32(m + NO_TAP_SHADOW)});
    uint32_t wait = cx(fn::DelayTime_create, {fbits(delay)});
    const uint32_t close = base + 0x18;
    st32(close + 16, close);
    st32(close, VT_NoTap_Close);
    st32(close + 4, m);
    uint32_t done = cx(fn::CallFunc_create, {close});
    cx(fn::Node_runAction, {layer, cx(fn::Sequence_create, {wait, done, 0})});
    destroyFn(close);
}
RW_FUNCTION(0x61FB24, "SceneManager::openNoTapScreen", SceneManager::openNoTapScreen);

// Run the current scene layer's out animation (when it is an IInOutAnimation) and then `callback`; otherwise
// call `callback` right away.
void SceneManager::callCurrentSceneRunOutAnimation(SceneManager *self, Addr callback) {
    Frame fr;
    const uint32_t base = fr.allocExact(0x58);
    uint32_t layer = self->sceneLayer;
    uint32_t anim = layer ? cx(DynamicCast, {layer, TI_Layer, TI_IInOutAnimation, uint32_t(-2)}) : 0;
    if (!anim) {
        if (ld32(callback + 16)) cx(fn::function_vv_call, {callback});
        return;
    }
    const uint32_t copy = base + 8, done = base + 0x20;
    cx(fn::function_vv_copy, {copy, callback});
    st32(done + 16, 0);
    uint32_t heap = cx(fn::operator_new, {0x20});
    if (heap) {
        st32(heap, VT_RunOut_Done);
        uint32_t dst = heap + 8, p = ld32(copy + 16);
        if (!p) {
            st32(heap + 0x18, 0);
        } else if (p == copy) {
            st32(heap + 0x18, dst);
            cx(ld32(ld32(p) + 0x0C), {p, dst});
        } else {
            st32(heap + 0x18, cx(ld32(ld32(p) + 0x08), {p}));
        }
    }
    st32(done + 16, heap);
    cx(IInOutAnimation_runOutAnimation, {anim, done});
    destroyFn(done);
    destroyFn(copy);
}
RW_FUNCTION(0x62186C, "SceneManager::callCurrentSceneRunOutAnimation", SceneManager::callCurrentSceneRunOutAnimation);

// faceImageUrls[key] = url; the map is emptied first once it holds more than 200 entries.
void SceneManager::addFaceImageUrl(SceneManager *self, Addr key, Addr url) {
    uint32_t m = mgrAddr(self);
    uint32_t map = m + 0x5CC;
    if (ld32(m + 0x5D4) > FACE_URL_LIMIT) {
        call(FaceMap_destroy, {map, ld32(m + 0x5D0)});
        st32(m + 0x5D4, 0);
        st32(map, m + 0x5D0);
        st32(m + 0x5D0, 0);
    }
    call(String_assign, {call(FaceMap_index, {map, key}), url});
}
RW_FUNCTION(0x621C28, "SceneManager::addFaceImageUrl", SceneManager::addFaceImageUrl);

// The cached url of `key` (a copy), or "".
void SceneManager::getFaceImageUrl(Addr result, SceneManager *self, Addr key) {
    uint32_t m = mgrAddr(self);
    uint32_t node = call(0x621D6D, {m + 0x5CC, key});   // __tree::find
    if (node == m + 0x5D0) {
        call(fn::string_init, {result, STR_Empty, 0});
        return;
    }
    uint32_t value = node + 0x1C;
    if (ld8(value) & 1) {
        call(fn::string_init, {result, ld32(value + 8), ld32(value + 4)});
    } else {
        st32(result, ld32(value));
        st32(result + 4, ld32(value + 4));
        st32(result + 8, ld32(value + 8));
    }
}
RW_FUNCTION(0x621D0C, "SceneManager::getFaceImageUrl", SceneManager::getFaceImageUrl);

// deleting destructor
void SceneManager::destroyDelete(SceneManager *self) {
    call(SceneManager_D2, {mgrAddr(self)});
    call(fn::operator_delete, {mgrAddr(self)});
}
RW_FUNCTION(0x60CAF4, "SceneManager::~SceneManager(D0)", SceneManager::destroyDelete);

// ------------------------------------------------------------------------------------------------ the chains' steps
namespace {
namespace chain {
constexpr uint32_t GOT_CREATE_ADVENTURE = 0xFD669C, GOT_CREATE_DECK_EDIT = 0xFD66B0;
constexpr uint32_t GOT_CREATE_SHOP_TOP = 0xFD66F8, GOT_CREATE_SPHERE_SELECT = 0xFD68A0;
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;            // (function<void(bool)> by value)
constexpr uint32_t APIManager_executeStageGet160310 = 0x4CADB9, APIManager_executeRaidGet = 0x4CBEC5;
constexpr uint32_t VT_AdvStages = 0xF5F060, VT_AdvRaid = 0xF5F08C;   // {flag 0}
constexpr uint32_t SM_TUTORIAL_FLAG = 0x30A;

// the tutorial's scene: changeSceneLayer(createSceneLayer<T>, true, true, fade) and the flag at +0x30A cleared
void toScene(uint32_t createGot) {
    changeSceneLayerTo(createGot, 1, 1, 1);
    st8(sceneManager() + SM_TUTORIAL_FLAG, 0);
}
}  // namespace chain

// ChainOfTutorialShop $_120: the shop (its API first, with the payment callback dropped)
void shopStep(uint32_t) {
    apiBusyDropPayment();
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(chain::SceneShopTop_callShopAPI, {none});
        functionDestroy(none);
    }
    chain::toScene(chain::GOT_CREATE_SHOP_TOP);
}
// ChainOfTutorialDeckEdit $_119 / ChainOfTutorialSphere $_118: the deck screen / the Avatar Board list
void deckEditStep(uint32_t) {
    apiBusyDropPayment();
    chain::toScene(chain::GOT_CREATE_DECK_EDIT);
}
void sphereStep(uint32_t) {
    apiBusyDropPayment();
    chain::toScene(chain::GOT_CREATE_SPHERE_SELECT);
}
// ChainOfTutorialAdventureSelect $_117: the 2016-03-10 stages, then (lambda(bool)) the raid; the adventure screen
void adventureStep(uint32_t) {
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t next = newFunction(f, chain::VT_AdvStages, {0});
        call(chain::APIManager_executeStageGet160310, {api, next});
        functionDestroy(next);
    }
    chain::toScene(chain::GOT_CREATE_ADVENTURE);
}
void adventureStagesLoaded(uint32_t, uint32_t okRef) {
    if (!ld8(okRef)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t next = newFunction(f, chain::VT_AdvRaid, {0});
    call(chain::APIManager_executeRaidGet, {api, next});
    functionDestroy(next);
}
// ChainOfTutorialAdventureSelect $_116: the adventure screen
void adventureSceneStep(uint32_t) { chain::toScene(chain::GOT_CREATE_ADVENTURE); }
}  // namespace

RW_FUNCTION(0x6220D0, "SceneManager::ChainOfTutorialShop $_120::operator()", shopStep);
RW_FUNCTION(0x622284, "SceneManager::ChainOfTutorialDeckEdit $_119::operator()", deckEditStep);
RW_FUNCTION(0x62240C, "SceneManager::ChainOfTutorialSphere $_118::operator()", sphereStep);
RW_FUNCTION(0x622594, "SceneManager::ChainOfTutorialAdventureSelect $_117::operator()", adventureStep);
RW_FUNCTION(0x622728, "SceneManager::ChainOfTutorialAdventureSelect $_117 lambda(bool)::operator()", adventureStagesLoaded);
RW_FUNCTION(0x6228E8, "SceneManager::ChainOfTutorialAdventureSelect $_116::operator()", adventureSceneStep);

// ------------------------------------------------------------------------------------------------ tutorial_menu's steps
namespace {
namespace tmenu {
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t VT_MenuNext = 0xF60240;      // $_33's popup callback {a, b}
constexpr uint32_t VT_MenuDone = 0xF6026C;      // $_125's popup callback {}
constexpr uint32_t S_ButtonD = 0xE03266;        // "Button_%d"
constexpr uint32_t SM_MENU_TUTORIAL = 0x588;    // SceneManager: byte, past phase 709
constexpr uint32_t SM_TUTORIAL_FLAG = 0x30A;
constexpr int32_t PHASE_MENU = 709;
constexpr uint32_t TUTORIAL_MENU = 119, TUTORIAL_MENU_2 = 120;

void finish() {
    const uint32_t sm = sceneManager();
    st8(sm + SM_MENU_TUTORIAL, int32_t(call(SceneUtil_getNexTutorialphase, {})) > PHASE_MENU ? 1 : 0);
    st8(sceneManager() + SM_TUTORIAL_FLAG, 0);
}
}  // namespace tmenu

// tutorial_menu $_33 {-, a, b, target}: tutorial 119 over the target (once)
void menuTutorial(uint32_t f) {
    if (call(tmenu::AppState_isClearTutorialPopupIDs, {call(fn::AppState_getInstance, {}), tmenu::TUTORIAL_MENU})) return;
    Frame fr;
    const uint32_t next = newFunction(fr, tmenu::VT_MenuNext, {ld32(f + 4), ld32(f + 8)});
    openTutorialWithClose(tmenu::TUTORIAL_MENU, next, ld32(f + 0xC));
    functionDestroy(next);
    tmenu::finish();
}
// tutorial_menu $_125 {menu}: tutorial 120 over the menu's "Button_2"
void menuTutorial2(uint32_t f) {
    uint32_t target;
    {
        Frame t;
        const uint32_t name = newStr(t, stringBuffer(t, {tmenu::S_ButtonD, 2}));
        target = SceneUtil::searchNodeByName(ld32(f + 4), name);
        strFree(name);
    }
    Frame fr;
    const uint32_t done = newFunction(fr, tmenu::VT_MenuDone, {});
    openTutorialWithClose(tmenu::TUTORIAL_MENU_2, done, target);
    functionDestroy(done);
    tmenu::finish();
}
}  // namespace

RW_FUNCTION(0x633B68, "SceneManager::tutorial_menu $_33::operator()", menuTutorial);
RW_FUNCTION(0x633D5C, "SceneManager::tutorial_menu $_125::operator()", menuTutorial2);
