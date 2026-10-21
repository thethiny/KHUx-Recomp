// SceneAvatarEdit (the avatar editor): the coordinate (outfit) switch and the panels it refreshes - the gender
// switch and the per-tab "new" marks, the accessory count - the equipped keyblade for the preview, the parts of a
// tab filtered by gender, a MyCoordinate slot's number plate, and leaving the editor (backScene, its $_40: back to
// MyPage; finalize: the "new parts" marks are cleared and saved).
#include "../scene_menu/SceneAvatarEdit.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;
using Addr = SceneAvatarEdit::Addr;

namespace {
namespace g {
constexpr uint32_t function_vb_call = 0x474139;                 // std::function<void(bool)>::operator()(bool)
constexpr uint32_t function_vb_assign = 0x490109;               // std::function<void(bool)>::operator=(const&)
constexpr uint32_t AppState_setNewAvatarPartsIDs = 0x4EBB4D;    // (state, const vector<int>* or null)
constexpr uint32_t AppState_setNewAvatarPartsSetIDs = 0x4EBCC9;
constexpr uint32_t AppState_save = 0x4E8DC9;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;  // (base, function<void()> by value)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;  // (function<void()> by value)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t GOT_FuncLayerPtr_vt = 0xFD6074;              // &vtable of __func<Layer*(*)(), ...>
constexpr uint32_t GOT_createMyPage = 0xFD6078;                 // &createSceneLayer<SceneMyPage>
constexpr uint32_t BattleUtil_getCurrentKeybladeIndex = 0x496F4D;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;             // (Style out, const Coordinate&, int keyblade)
constexpr uint32_t AvatarActor_setStyle = 0x740635;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t AvatarParts_create = 0x778DE1;               // master::AvatarParts::create(shared_ptr out, id)
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;           // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t ImageView_loadTexture = 0x839731;
}  // namespace g

constexpr uint32_t S_Txt_Accessorie_Cost = 0xE06944, S_Txt_Num_Label = 0xDFD8DC, S_Switch_M = 0xE06932,
                   S_Switch_L = 0xE0693B, S_tab_d = 0xE06801, S_Icon_New = 0xDFDD8A, S_Box_Num = 0xE02AEE,
                   S_BoxNumPng = 0xE069B2;                     // "img/ui/Av_Box_Num%d.png"
constexpr int32_t TXT_ACCESSORY_COST = 102500018, TXT_ACCESSORY_COUNT = 102500023;
constexpr uint32_t ACCESSORY_MAX = 5;
constexpr uint32_t TABS = 7;
constexpr uint32_t VT_BackDone = 0xF6A5C0;                     // backScene $_40 (no captures)
constexpr uint32_t SM_RETURN_TO_MYPAGE = 0x4CC;                // SceneManager byte
constexpr uint32_t SM_ON_MYPAGE = 0x548;                       // SceneManager std::function<void(bool)>
constexpr uint32_t API_USER_KEYBLADES = 0xEC;                  // APIManager shared_ptr<SafeBuffer> (0x58-byte records)
constexpr uint32_t KEYBLADE_RECORD = 0x58, KEYBLADE_ID = 0xC;
constexpr uint32_t PARTS_GENDER = 0x8C;                        // master::AvatarParts: 1 male, 2 female
constexpr uint32_t STYLE_SIZE = 0x24;
constexpr uint32_t V_SET_VISIBLE = 0xB0;

Addr self_addr(SceneAvatarEdit *self) { return rw::guest_addr(self); }
}  // namespace

// finalize(function<void(bool)> done): no parts are "new" any more; saved; done(true)
void SceneAvatarEdit::finalize(SceneAvatarEdit *, Addr done) {
    call(g::AppState_setNewAvatarPartsIDs, {call(fn::AppState_getInstance, {}), 0});
    call(g::AppState_setNewAvatarPartsSetIDs, {call(fn::AppState_getInstance, {}), 0});
    call(g::AppState_save, {call(fn::AppState_getInstance, {})});
    call(g::function_vb_call, {done, 1});
}

// backScene: once (the editor may be left by the back key and the button), the out animation then $_40
void SceneAvatarEdit::backScene(SceneAvatarEdit *self) {
    if (!self->isTutorialEdit) return;
    self->isTutorialEdit = 0;
    Frame f;
    uint32_t done = newFunction(f, VT_BackDone, {});
    call(g::IInOutAnimation_runOutAnimation, {self_addr(self) + 0x288, done});
    functionDestroy(done);
}

namespace {
// backScene $_40: back to MyPage, its APIs requested again
void backDone(uint32_t) {
    uint32_t mgr = sceneManager();
    st8(mgr + SM_RETURN_TO_MYPAGE, 1);
    {
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(g::function_vb_assign, {mgr + SM_ON_MYPAGE, none});
        functionDestroy(none);
    }
    {
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(g::SceneUtil_callPrepareMypageAPIs, {none});
        functionDestroy(none);
    }
    mgr = sceneManager();
    Frame f;
    uint32_t layer = newFunction(f, ld32(g::GOT_FuncLayerPtr_vt) + 8, {ld32(g::GOT_createMyPage)});
    call(g::SceneManager_changeSceneLayer, {mgr, layer, 1, 1, 1});
    functionDestroy(layer);
}
}  // namespace

// the keyblade the preview holds: the current deck's (or the first one), only when editing and asked for
int32_t SceneAvatarEdit::getEquipKeybladeID(SceneAvatarEdit *self, uint32_t show) {
    if (self->isCreate || show != 1) return 0;
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t sb = ld32(api + API_USER_KEYBLADES), ctrl = ld32(api + API_USER_KEYBLADES + 4);
    if (ctrl) call(fn::shared_add, {ctrl});
    int32_t k = int32_t(call(g::BattleUtil_getCurrentKeybladeIndex, {}));
    if (k == -1) k = 0;
    int32_t id = safe32(sb, KEYBLADE_RECORD * uint32_t(k) + KEYBLADE_ID);
    if (ctrl) call(fn::shared_release, {ctrl});
    return id;
}

// the coordinate (MyCoordinate slot) `index`: the preview's style, the gender switch and the accessory count
void SceneAvatarEdit::changeMyCordinate(SceneAvatarEdit *self, int32_t index) {
    self->coordinateIndex = index;
    const int32_t keyblade = getEquipKeybladeID(self, self->showKeyblade);
    const uint32_t begin = self->coordinates.begin, n = (self->coordinates.end - begin) / sizeof(Coordinate);
    if (!(n > uint32_t(self->coordinateIndex))) call(g::ThrowOutOfRange, {self_addr(self) + 0x300});
    self->currentCoordinate = begin + sizeof(Coordinate) * uint32_t(self->coordinateIndex);
    Frame f;
    uint32_t style = f.alloc(STYLE_SIZE);
    const uint32_t actor = self->avatarActor;
    call(g::AvatarUtil_makeStyle, {style, self->currentCoordinate, uint32_t(keyblade)});
    call(g::AvatarActor_setStyle, {actor, style});
    call(g::SafeBuffer_dtor, {style});
    updateGenderPanel(self);
    updateCost(self);
}

// the accessory tab's count: "n/5"
void SceneAvatarEdit::updateCost(SceneAvatarEdit *self) {
    const uint32_t c = self->currentCoordinate;
    uint32_t used = 0;
    for (uint32_t k = 0; k < ACCESSORY_MAX; k++) used += ld32(c + 0x1C + 4 * k) != 0;
    titleLabel(self->tabAccessory, S_Txt_Accessorie_Cost, 0x13, TXT_ACCESSORY_COST);
    uint32_t text = findNode(self->tabAccessory, S_Txt_Num_Label, 0xD);
    SceneUtil::setShadow(text);
    Frame f;
    uint32_t fmt = textData(f, TXT_ACCESSORY_COUNT);
    setTextC(text, stringBuffer(f, {textDataStr(fmt), used, ACCESSORY_MAX}));
    sharedRelease(fmt);
}

// the gender switch (M / L) and each tab's "new" mark for that gender
void SceneAvatarEdit::updateGenderPanel(SceneAvatarEdit *self) {
    uint32_t male = findNode(self->root, S_Switch_M, 8);
    uint32_t female = findNode(self->root, S_Switch_L, 8);
    const uint32_t isMale = ld32(self->currentCoordinate + 4) == 1;
    vcall(male, V_SET_VISIBLE, {male, isMale});
    vcall(female, V_SET_VISIBLE, {female, !isMale});
    self->isMale = uint8_t(isMale);
    for (uint32_t k = 0; k < TABS; k++) {
        Frame f;
        uint32_t name = newStr(f, stringBuffer(f, {S_tab_d, k + 1}));
        uint32_t tab = SceneUtil::searchNodeByName(self->root, name);
        const uint32_t bits = self->isMale ? self_addr(self) + 0x2C8 : self_addr(self) + 0x2D4;   // vector<bool>
        if (!(ld32(bits + 4) > k)) call(g::ThrowOutOfRange, {bits});
        const uint32_t on = (ld32(ld32(bits) + 4 * (k >> 5)) & (1u << (k & 31))) != 0;
        uint32_t icon = findNode(tab, S_Icon_New, 8);
        if (icon) vcall(icon, V_SET_VISIBLE, {icon, on});
        strFree(name);
    }
}

// genderFiliteringIDs(tab) -> vector<int>: the tab's part ids for the current gender
void SceneAvatarEdit::genderFiliteringIDs(Addr out, SceneAvatarEdit *self, int32_t tab) {
    const uint32_t lists = self->partIdsByTab.begin;
    const uint32_t gender = self->isMale ? 1 : 2;
    if (!((self->partIdsByTab.end - lists) / 12 > uint32_t(tab))) call(g::ThrowOutOfRange, {self_addr(self) + 0x2F4});
    st32(out, 0);
    st32(out + 4, 0);
    st32(out + 8, 0);
    const uint32_t list = lists + 12 * uint32_t(tab);
    const uint32_t end = ld32(list + 4);
    for (uint32_t p = ld32(list); p != end; p += 4) {
        Frame f;
        uint32_t id = f.alloc(4);
        st32(id, ld32(p));
        uint32_t row = masterRow(f, g::AvatarParts_create, ld32(id));
        if (uint32_t(safe32(ld32(row), PARTS_GENDER)) == gender) {
            const uint32_t e = ld32(out + 4);
            if (e == ld32(out + 8)) call(g::VectorInt_pushBackSlow, {out, id});
            else {
                if (e) st32(e, ld32(id));
                st32(out + 4, ld32(out + 4) + 4);
            }
        }
        sharedRelease(row);
    }
}

namespace {
// a MyCoordinate slot button: its number plate (img/ui/Av_Box_Num<n>.png)
void slotNumber(uint32_t buttonRef, int32_t index) {
    uint32_t plate = findNode(ld32(buttonRef), S_Box_Num, 7);
    Frame f;
    uint32_t path = newStr(f, stringBuffer(f, {S_BoxNumPng, uint32_t(index + 1)}));
    call(g::ImageView_loadTexture, {plate, path, 0});
    strFree(path);
}

}  // namespace

RW_FUNCTION(0x70A71C, "SceneAvatarEdit::finalize(function<void(bool)>)", SceneAvatarEdit::finalize);
RW_FUNCTION(0x70A750, "SceneAvatarEdit::backScene", SceneAvatarEdit::backScene);
RW_FUNCTION(0x70A850, "SceneAvatarEdit::backScene()::$_40 __func::operator()", backDone);
RW_FUNCTION(0x70715C, "SceneAvatarEdit::getEquipKeybladeID(bool)", SceneAvatarEdit::getEquipKeybladeID);
RW_FUNCTION(0x705C24, "SceneAvatarEdit::changeMyCordinate(int)", SceneAvatarEdit::changeMyCordinate);
RW_FUNCTION(0x707490, "SceneAvatarEdit::updateCost", SceneAvatarEdit::updateCost);
RW_FUNCTION(0x707228, "SceneAvatarEdit::updateGenderPanel", SceneAvatarEdit::updateGenderPanel);
RW_FUNCTION(0x707B4C, "SceneAvatarEdit::genderFiliteringIDs(int)", SceneAvatarEdit::genderFiliteringIDs);
RW_FUNCTION(0x707050, "SceneAvatarEdit::initMyCordinateButton slot number plate (sub_707050)", slotNumber);

// ------------------------------------------------------------------------------------------------ tabs
namespace {
namespace tb {
constexpr uint32_t ImageButton_create = 0x531239;            // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageButton_setSelectEnabled = 0x5318F1;
constexpr uint32_t ImageButton_setIsSelected = 0x5318FD;
constexpr uint32_t ImageButtonVector_pushBack = 0x707649;    // cocos2d::Vector<ImageButton*>::pushBack
constexpr uint32_t S_Button_Filter = 0xE055B1, S_Txt_Filter = 0xE04442, S_Av_Make_Win_But = 0xE06807,
                   S_Txt_Tab = 0xE06817;
constexpr int32_t TXT_FILTER = 100100035;
constexpr uint32_t TAB_TEXTS = 0xE8C1B0;                     // int[7]: the tab titles (102500001..7)
constexpr uint32_t VT_Filter = 0xF6AABC;                     // initTabButton filter button {this}
constexpr uint32_t VT_Tab = 0xF6AA90;                        // tab button {this, index}
constexpr uint32_t SAE_TAB_BUTTONS = 0x2BC;                  // Vector<ImageButton*>
constexpr uint32_t V_ADD_CHILD = 0xF0;
}  // namespace tb
}  // namespace

// the accessory filter button (the accessory panel starts hidden) and the seven tab buttons: an ImageButton over
// each "tab<n>" image, selected for the current tab, its "new" mark and title
void SceneAvatarEdit::initTabButton(SceneAvatarEdit *self) {
    const uint32_t me = self_addr(self);
    vcall(self->tabAccessory, V_SET_VISIBLE, {self->tabAccessory, 0});
    self->filterButton = findNode(self->tabAccessory, tb::S_Button_Filter, 0xD);
    addTouch(self->filterButton, tb::VT_Filter, {me});
    titleLabel(self->filterButton, tb::S_Txt_Filter, 0xA, tb::TXT_FILTER);
    for (uint32_t k = 0; k < TABS; k++) {
        Frame f;
        uint32_t name = newStr(f, stringBuffer(f, {S_tab_d, k + 1}));
        uint32_t tab = SceneUtil::searchNodeByName(self->root, name);
        uint32_t button;
        {
            Frame t;
            uint32_t file = newStr(t, tb::S_Av_Make_Win_But, 0xF);
            button = call(tb::ImageButton_create, {file, 0});
            strFree(file);
        }
        call(tb::ImageButton_setTargetImageView, {button, tab});
        vcall(tab, tb::V_ADD_CHILD, {tab, button});
        {
            Frame t;
            uint32_t cb = newFunction(t, tb::VT_Tab, {me, k});
            call(tb::ImageButton_addTouchEventListener, {button, cb});
            functionDestroy(cb);
        }
        call(tb::ImageButton_setSelectEnabled, {button, 1});
        if (k == uint32_t(int32_t(self->tabIndex))) call(tb::ImageButton_setIsSelected, {button, 1});
        call(tb::ImageButtonVector_pushBack, {me + tb::SAE_TAB_BUTTONS, button});
        const uint32_t bits = self->isMale ? me + 0x2C8 : me + 0x2D4;
        if (!(ld32(bits + 4) > k)) call(g::ThrowOutOfRange, {bits});
        const uint32_t word = ld32(bits);
        uint32_t icon = findNode(tab, S_Icon_New, 8);
        if (icon) vcall(icon, V_SET_VISIBLE, {icon, (ld32(word + 4 * (k >> 5)) & (1u << (k & 31))) != 0});
        titleLabel(tab, tb::S_Txt_Tab, 7, int32_t(ld32(tb::TAB_TEXTS + 4 * k)));
        strFree(name);
    }
}
RW_FUNCTION(0x7054AC, "SceneAvatarEdit::initTabButton", SceneAvatarEdit::initTabButton);

// ------------------------------------------------------------------------------------------------ MyCoordinate
namespace {
namespace mc {
constexpr uint32_t S_MyCoordinate = 0xE067E6, S_Button_Switch = 0xE067F3;
constexpr uint32_t CoordinateVector_pushBackSlow = 0x70CB95;   // vector<Coordinate>::__push_back_slow_path(const&)
constexpr uint32_t SceneManager_updateHeaderAvatarIcon = 0x611725;   // (mgr, ?, bool)
constexpr uint32_t aeabi_memset = 0x40E7A0;                    // (dst, n, c)
constexpr uint32_t VT_Switch = 0xF6AAE8;                       // "Button_Switch" {this, MyCoordinate}
constexpr uint32_t API_USER_DETAIL = 0x94, UD_EQUIP_COORDINATE = 0x3C;
constexpr uint32_t API_COORDINATES = 0xC4;                     // shared_ptr<SafeBuffer>: 0x30-byte Coordinates
constexpr uint32_t SB_COUNT = 0x20, COORDINATE = 0x30, MAX_SLOTS = 6;
constexpr uint32_t FRAME = 0xE8, AT_DEFAULT = 0x9C, AT_RECORD = 0x6C;   // the original frame (the default's id word
                                                                         // is never set: the stale word is pushed)
}  // namespace mc

// vector<Coordinate>::push_back(const&) as compiled inline
void pushCoordinate(uint32_t vec, uint32_t c) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(mc::CoordinateVector_pushBackSlow, {vec, c});
        return;
    }
    uint32_t base = 0;
    if (end) {
        for (uint32_t k = 0; k < mc::COORDINATE; k += 4) st32(end + k, ld32(c + k));
        base = ld32(vec + 4);
    }
    st32(vec + 4, base + mc::COORDINATE);
}
}  // namespace

// the MyCoordinate panel: hidden for the first avatar (a default coordinate is the only one); else the saved
// coordinates 1..6 (only those the server sent), the equipped one selected (7 = none yet: 1, and the header icon is refreshed), its number plate
// and the switch button
void SceneAvatarEdit::initMyCordinateButton(SceneAvatarEdit *self) {
    guest::Frame frame(mc::FRAME);
    const uint32_t me = self_addr(self);
    uint32_t panel = findNode(self->root, mc::S_MyCoordinate, 0xC);
    vcall(panel, V_SET_VISIBLE, {panel, self->isCreate ? 0u : 1u});
    const uint32_t coords = me + 0x300;
    if (self->isCreate) {
        const uint32_t c = frame.addr(mc::AT_DEFAULT);
        st32(c + 4, 1);                                   // male
        st32(c + 0x14, 1);
        st32(c + 0x18, 30001);
        st32(c + 0x10, 20001);
        st32(c + 8, 40001);
        st32(c + 0xC, 50001);
        call(mc::aeabi_memset, {c + 0x1C, 0x14, 0});
        st32(c + 0x1C, 109001);
        pushCoordinate(coords, c);
        return;
    }
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t equipped = uint32_t(safe32(ld32(api + mc::API_USER_DETAIL), mc::UD_EQUIP_COORDINATE));
    if (equipped == 7) {
        equipped = 1;
        call(mc::SceneManager_updateHeaderAvatarIcon, {sceneManager(), 0, 1});
    }
    api = call(fn::APIManager_getInstance, {});
    const uint32_t record = frame.addr(mc::AT_RECORD);
    for (uint32_t slot = 1; slot <= mc::MAX_SLOTS; slot++) {
        for (uint32_t k = 0; k < ld32(ld32(api + mc::API_COORDINATES) + mc::SB_COUNT); k++) {
            const SafeBuffer *s = safe(ld32(api + mc::API_COORDINATES));
            SafeBuffer::read(s, rw::host_ptr<uint8_t>(record), s->data.get() + mc::COORDINATE * k, mc::COORDINATE,
                             mc::COORDINATE * k);
            if (ld32(record) == slot) {
                pushCoordinate(coords, record);
                break;
            }
        }
    }
    for (uint32_t k = 0, n = (ld32(coords + 4) - ld32(coords)) / mc::COORDINATE; k < n; k++)
        if (ld32(ld32(coords) + mc::COORDINATE * k) == equipped) self->coordinateIndex = int32_t(k);
    {
        Frame f;
        uint32_t ref = f.alloc(4);
        st32(ref, panel);
        call(0x707051, {ref, uint32_t(self->coordinateIndex)});   // the slot's number plate
    }
    uint32_t button = findNode(panel, mc::S_Button_Switch, 0xD);
    addTouch(button, mc::VT_Switch, {me, panel});
}
RW_FUNCTION(0x7058C8, "SceneAvatarEdit::initMyCordinateButton", SceneAvatarEdit::initMyCordinateButton);

// ------------------------------------------------------------------------------------------------ changeTab
namespace {
namespace ct {
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t BaseScrollViewInt_setItemIDs = 0x478929;     // (view, const vector<int>&)
constexpr uint32_t AvatarPartsScrollView_setSelectedIDsEnabled = 0x554F31;
constexpr uint32_t AvatarPartsScrollView_setSelectedIDs = 0x554F05;
constexpr uint32_t AvatarPartsScrollView_setSelectedID = 0x554EF1;
constexpr uint32_t SceneAvatarEdit_createFilterdAccessoryIDs = 0x707951;   // (vector<int> out, this)
constexpr uint32_t SceneAvatarEdit_createAccessoryIDs = 0x707C61;
constexpr uint32_t VectorInt_copy = 0x4AF707, VectorInt_dtor = 0x478961;
constexpr uint32_t S_Txt_NoAvatar = 0xE0681F;
constexpr int32_t TXT_NO_AVATAR = 102500020;
// AvatarPartsScrollView
constexpr uint32_t SV_ITEM_IDS = 0x628;                          // vector<int>
constexpr uint32_t SV_FLAG_A = 0x690, SV_FLAG_B = 0x691, SV_IS_MALE = 0x692, SV_FLAG_C = 0x693;
constexpr uint32_t TAB_ACCESSORY = 2;
}  // namespace ct
}  // namespace

// tab `tab`: its "new" mark, which coordinate field it edits (the part selected in the list), the accessory panel
// and filter button, the list's items (accessories: the filtered ids, several selected) and "no avatar parts"
void SceneAvatarEdit::changeTab(SceneAvatarEdit *self, uint32_t tab) {
    const uint32_t me = self_addr(self);
    self->tabIndex = int32_t(tab);
    const uint8_t male = self->isMale;
    Frame f;
    uint32_t name = newStr(f, stringBuffer(f, {S_tab_d, tab + 1}));
    const uint32_t bits = male ? me + 0x2C8 : me + 0x2D4;
    uint32_t node = SceneUtil::searchNodeByName(self->root, name);
    if (!(ld32(bits + 4) > tab)) call(g::ThrowOutOfRange, {bits});
    const uint32_t word = ld32(bits);
    uint32_t icon = findNode(node, S_Icon_New, 8);
    if (icon) vcall(icon, V_SET_VISIBLE, {icon, (ld32(word + 4 * (tab >> 5)) & (1u << (tab & 31))) != 0});
    const uint32_t c = self->currentCoordinate;
    const uint32_t sv = self->scrollView;
    uint32_t selected = 0, filter = 0;
    switch (tab) {
    case 0:
        selected = ld32(c + 0x14);
        st8(sv + ct::SV_FLAG_B, 1);
        st8(sv + ct::SV_FLAG_C, 0);
        break;
    case 2:
        filter = 1;
        st8(sv + ct::SV_FLAG_B, 1);
        st8(sv + ct::SV_FLAG_C, 1);
        break;
    case 1: case 3: case 5:
        selected = ld32(c + (tab == 3 ? 8 : tab == 5 ? 0x10 : 0x14));
        st8(sv + ct::SV_FLAG_B, 1);
        st8(sv + ct::SV_FLAG_C, 1);
        break;
    case 4: case 6:
        selected = ld32(c + (tab == 4 ? 0xC : 0x18));
        st8(sv + ct::SV_FLAG_B, 0);
        st8(sv + ct::SV_FLAG_C, 1);
        break;
    default:
        break;
    }
    vcall(self->tabAccessory, V_SET_VISIBLE, {self->tabAccessory, 1});
    vcall(self->filterButton, V_SET_VISIBLE, {self->filterButton, filter});
    st8(self->scrollView + ct::SV_IS_MALE, self->isMale);
    call(ct::IScrollProtocol_setIndex, {self->scrollView, 0});
    st8(self->scrollView + ct::SV_FLAG_A, tab == 0);
    if (tab == ct::TAB_ACCESSORY) {
        call(ct::IScrollProtocol_setIndex, {self->scrollView, 0});
        uint32_t items = f.alloc(12), picked = f.alloc(12);
        call(ct::SceneAvatarEdit_createFilterdAccessoryIDs, {items, me});
        call(ct::BaseScrollViewInt_setItemIDs, {self->scrollView, items});
        call(ct::AvatarPartsScrollView_setSelectedIDsEnabled, {self->scrollView, 1});
        const uint32_t view = self->scrollView;
        call(ct::SceneAvatarEdit_createAccessoryIDs, {picked, me});
        call(ct::AvatarPartsScrollView_setSelectedIDs, {view, picked});
        call(ct::VectorInt_dtor, {picked});
        call(ct::VectorInt_dtor, {items});
    } else {
        uint32_t items = f.alloc(12);
        genderFiliteringIDs(items, self, int32_t(tab));
        call(ct::BaseScrollViewInt_setItemIDs, {self->scrollView, items});
        call(ct::AvatarPartsScrollView_setSelectedIDsEnabled, {self->scrollView, 0});
        if (tab) {
            call(ct::AvatarPartsScrollView_setSelectedID, {self->scrollView, selected});
            uint32_t ids = f.alloc(12);
            call(ct::VectorInt_copy, {ids, self->scrollView + ct::SV_ITEM_IDS});
            for (uint32_t k = 0; k < (ld32(ids + 4) - ld32(ids)) / 4; k++)
                if (selected == ld32(ld32(ids) + 4 * k)) call(ct::IScrollProtocol_setIndex, {self->scrollView, k});
            call(ct::VectorInt_dtor, {ids});
        } else {
            call(ct::AvatarPartsScrollView_setSelectedID, {self->scrollView, 0});
        }
        call(ct::VectorInt_dtor, {items});
    }
    call(ct::IScrollProtocol_updateLayout, {self->scrollView, 0});
    uint32_t text = findNode(self->centerUILeft, ct::S_Txt_NoAvatar, 0xC);
    SceneUtil::setShadow(text);
    setTextId(text, ct::TXT_NO_AVATAR);
    const uint32_t list = self->scrollView + ct::SV_ITEM_IDS;
    vcall(text, V_SET_VISIBLE, {text, ld32(list + 4) == ld32(list)});
    strFree(name);
}
RW_FUNCTION(0x705CF4, "SceneAvatarEdit::changeTab(int)", SceneAvatarEdit::changeTab);
