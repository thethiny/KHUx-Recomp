// IMedalSortFilter::openMedalSortPopup: the medal sort / filter popup. Ten sort keys (one selected, touch off),
// ascending / descending, twenty filters with a "Release" (clear) button, Sort / Filter tabs, OK and Close.
// Every widget gets a heap lambda holding the state (shared_ptr<MedalSortFilterState>); the popup's close
// handler gets a copy of the state bytes as they were on opening, all the widgets and the caller's callback.
// MedalSortFilterState: +0 mode (0 sort, 1 filter), +1 sort key, +2 ascending, +3..+22 the filters.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
constexpr uint32_t SetShadow = 0x45BD15;                  // SceneUtil::setShadow(Text*)
constexpr uint32_t SetButtonEnabled = 0x468625;           // SceneUtil::setButtonEnabled(Button*, Text*, bool, bool)
constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState;
constexpr uint32_t CheckBox_addEventListener = rwlib::cocos2d::ui::CheckBox::addEventListener;  // (box, std::function<void(Ref*, EventType)>)
constexpr uint32_t ImageButton_create = 0x531239;         // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;  // (button, Node*, float scale)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t S_PopupJson = 0xE04224;                // "cocostudio/publish/PopupNormal_SortMedal_ver120.json"
constexpr uint32_t S_Empty = 0xDF8A1A;
constexpr uint32_t S_Txt = 0xDF94D1;

// the sort keys in the order of their lambdas (and of each lambda's captures); state +1 picks one (6 unused)
struct SortKey {
    uint32_t name;
    int32_t text;
    uint32_t vt;
    uint32_t key;
};
constexpr SortKey SORT_KEYS[10] = {
    {0xE04271, 102700001, 0xF63234, 0},     // Sort_Attribute
    {0xE04280, 102700003, 0xF63208, 1},     // Sort_Rarity
    {0xE0428C, 102700002, 0xF631DC, 2},     // Sort_Lv
    {0xE04294, 102700004, 0xF631B0, 3},     // Sort_Cost
    {0xE0429E, 102700005, 0xF63184, 4},     // Sort_Attack
    {0xE042AA, 102700006, 0xF63158, 5},     // Sort_Defense
    {0xE042B7, 102700007, 0xF6312C, 7},     // Sort_Get
    {0xE042C0, 102700009, 0xF63100, 8},     // Sort_Price
    {0xE042CB, 102700014, 0xF630D4, 9},     // Sort_Skill
    {0xE042D6, 102700015, 0xF630A8, 10},    // Sort_Ability
};
constexpr uint32_t SORT_PRICE = 7;          // the one sort key the close handler does not get
// the filters in the order of their state bytes (+3..+22) and lambdas; text 0 = no "Txt" label
struct Filter {
    uint32_t name;
    int32_t text;
    uint32_t vt;
};
constexpr Filter FILTERS[20] = {
    {0xDF9D98, 0, 0xF63024},            // Filter_Power
    {0xDF9DA5, 0, 0xF62FF8},            // Filter_Speed
    {0xDF9DB2, 0, 0xF62FCC},            // Filter_Magic
    {0xE042F5, 102800016, 0xF62FA0},    // Filter_Uplight
    {0xE04304, 102800017, 0xF62F74},    // Filter_Reverse
    {0xE04313, 0, 0xF62F48},            // Filter_RareStarSet1
    {0xE04327, 0, 0xF62F1C},            // Filter_RareStarSet2
    {0xE0433B, 0, 0xF62EF0},            // Filter_RareStarSet3
    {0xE0434F, 0, 0xF62EC4},            // Filter_RareStarSet4
    {0xE04363, 0, 0xF62E98},            // Filter_RareStarSet5
    {0xE04377, 0, 0xF62E6C},            // Filter_RareStarSet6
    {0xE0438B, 102800007, 0xF62E40},    // Filter_General
    {0xE0439A, 102800009, 0xF62E14},    // Filter_Exp
    {0xE043A5, 102800010, 0xF62DE8},    // Filter_limit
    {0xE043B2, 102800008, 0xF62DBC},    // Filter_Cost
    {0xE043BE, 102800012, 0xF62D90},    // Filter_Evo
    {0xE043C9, 102800011, 0xF62D64},    // Filter_Price
    {0xE043D6, 102800013, 0xF62D38},    // Filter_Favorite
    {0xE043E6, 102800014, 0xF62D0C},    // Filter_Skill_Set
    {0xE043F7, 102800015, 0xF62CE0},    // Filter_Skill_NotSet
};
constexpr uint32_t S_DummySort = 0xE04259, S_DummyFilter = 0xE04264, S_SortUp = 0xE042E3, S_SortDown = 0xE042EB;
constexpr uint32_t S_ButtonRelease = 0xDF9CCA, S_TxtRelease = 0xDF9CD9, S_ButtonOK = 0xDF8688, S_TxtOK = 0xDF869F;
constexpr uint32_t S_CloseButton = 0xDF8D2F, S_SwitchSort = 0xE0440B, S_SwitchFilter = 0xE04417;
constexpr uint32_t S_HitLeft = 0xE04425, S_HitRight = 0xE04433, S_TxtSort = 0xE041FE, S_TxtFilter = 0xE04442;

constexpr uint32_t VT_Up = 0xF6307C, VT_Down = 0xF63050;      // heap 0x18 {-, up, down, state}
constexpr uint32_t VT_Release = 0xF62CB4;                    // heap 0x68 {20 filters, release, its text, state, scene}
constexpr uint32_t VT_Ok = 0xF62C88, VT_Close = 0xF62C5C;     // {}
constexpr uint32_t VT_TabSort = 0xF62C30, VT_TabFilter = 0xF62C04;   // heap 0x24 {tab, state, tabs, release, dummies}
constexpr uint32_t VT_CanClose = 0xF62BD8;                   // {close button}
constexpr uint32_t VT_OnClose = 0xF62B80;                    // heap 0xE8: the captures below, 8 bytes further
constexpr uint32_t VT_RemoveNodes = 0xF62BAC;                // the close animation's end {shadow, popup, layer}
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AppState_save = 0x4E8DC9;
constexpr uint32_t function_vb_call = 0x474139;             // std::function<void(bool)>::operator()
constexpr uint32_t IMAGE_BUTTON_ENABLED = 0x272;             // ImageButton: takes touches (byte)
constexpr int32_t TXT_SORT_TAB = 100100010, TXT_FILTER_TAB = 100100035, TXT_OK = 100100001;
constexpr int32_t TXT_UP = 102700011, TXT_DOWN = 102700012, TXT_RELEASE = 102800001;

// The close handler's captures as the original builds them (copied twice on the stack, then moved to the heap):
// +0 state, +8 state bytes 1..22 as on opening, +0x20 the widgets, +0xB8 the callback (std::function<void(bool)>),
// +0xD0 scene type, +0xD4 the state again (the copy taken first).
constexpr uint32_t C_BYTES = 8, C_WIDGETS = 0x20, C_FN = 0xB8, C_SCENE = 0xD0, C_STATE2 = 0xD4, C_SIZE = 0xDC;
constexpr uint32_t N_BYTES = 22, N_WIDGETS = 37;

uint32_t find(uint32_t root, uint32_t name) { return findNode(root, name, host_strlen(name)); }
void addRef(uint32_t ctrl) {
    if (ctrl) call(fn::shared_add, {ctrl});
}
void releaseRef(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}
void setVisible(uint32_t n, uint32_t on) { vcall(n, node::setVisible, {n, on}); }
void select(uint32_t box) {
    call(CheckBox_setSelectedState, {box, 1});
    vcall(box, V_setTouchEnabled, {box, 0});
}
// a std::function temporary whose callable is the heap object `size` bytes with vtable `vt`, filled by `fill`
template <class F> void heapListener(uint32_t add, uint32_t widget, uint32_t vt, uint32_t size, F fill) {
    Frame f;
    const uint32_t fo = f.alloc(24);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, vt);
        fill(heap);
    }
    st32(fo + 16, heap);
    call(add, {widget, fo});
    functionDestroy(fo);
}
uint32_t imageButton() {
    Frame f;
    const uint32_t s = newStr(f, S_Empty, 0);
    const uint32_t b = call(ImageButton_create, {s, 0});
    strFree(s);
    return b;
}
}  // namespace

namespace medal_sort_popup {

// the popup's text helper (sub_66DDDC): shadowed text from the text table
uint32_t setText(uint32_t text, int32_t id) {
    call(SetShadow, {text});
    setTextId(text, id);
    return 0;
}
RW_FUNCTION(0x66DDDC, "IMedalSortFilter::openMedalSortPopup text helper (sub_66DDDC)", medal_sort_popup::setText);

// returns what the original leaves in r0: the frame size its epilogue loads
uint32_t openMedalSortPopup(uint32_t /*medals*/, uint32_t state, uint32_t sceneType, uint32_t callback) {
    constexpr uint32_t EPILOGUE_R0 = 0xB0C;
    Frame f;
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PopupJson});
    vcall(root, node::setAnchorPoint, {root, VEC2_ANCHOR_MIDDLE});
    {
        const float px = bitsf(vcall(root, node::getPositionX, {root}));
        const float w = ldf(vcall(root, node::getContentSize, {root}));
        const float side = SceneUtil::getSideMargin();
        const float py = bitsf(vcall(root, node::getPositionY, {root}));
        const float h = ldf(vcall(root, node::getContentSize, {root}) + 4);
        const float x = side + (px + w * 0.5f);
        const float yHalf = py + h * 0.5f;
        const float y = SceneUtil::getSideHeightMargin() + yHalf;
        Frame g;
        vcall(root, node::setPosition, {root, newVec2(g, x, y)});
    }
    vcall(shadow, node::addChild, {shadow, root});
    const uint32_t dummySort = find(root, S_DummySort);
    setVisible(dummySort, 1);
    const uint32_t dummyFilter = find(root, S_DummyFilter);
    setVisible(dummyFilter, 0);

    // sort keys
    uint32_t sortBox[10];
    for (uint32_t i = 0; i < 10; i++) {
        sortBox[i] = find(root, SORT_KEYS[i].name);
        setText(find(sortBox[i], S_Txt), SORT_KEYS[i].text);
    }
    // a copy of the state kept for the filters and the close handler (referenced twice, released twice at the end)
    const uint32_t heldObj = ld32(state), heldCtrl = ld32(state + 4);
    addRef(heldCtrl);
    for (uint32_t i = 0; i < 10; i++) {
        const uint32_t obj = ld32(state), ctrl = ld32(state + 4);
        addRef(ctrl);
        heapListener(fn::Widget_addTouchEventListener, sortBox[i], SORT_KEYS[i].vt, 0x3C, [&](uint32_t h) {
            for (uint32_t k = 0; k < 10; k++) st32(h + 8 + 4 * k, sortBox[k]);
            st32(h + 0x30, sortBox[i]);
            st32(h + 0x34, obj);
            st32(h + 0x38, ctrl);
        });
    }
    {
        const uint32_t key = ld8(ld32(state) + 1);
        for (uint32_t i = 0; i < 10; i++)
            if (SORT_KEYS[i].key == key) select(sortBox[i]);
    }
    const uint32_t up = find(root, S_SortUp);
    setText(find(up, S_Txt), TXT_UP);
    const uint32_t down = find(root, S_SortDown);
    setText(find(down, S_Txt), TXT_DOWN);
    for (uint32_t which = 0; which < 2; which++) {
        const uint32_t obj = ld32(state), ctrl = ld32(state + 4);
        addRef(ctrl);
        heapListener(fn::Widget_addTouchEventListener, which ? down : up, which ? VT_Down : VT_Up, 0x18,
                     [&](uint32_t h) {
                         st32(h + 8, up);
                         st32(h + 0xC, down);
                         st32(h + 0x10, obj);
                         st32(h + 0x14, ctrl);
                     });
    }
    select(ld8(ld32(state) + 2) ? up : down);

    // filters
    uint32_t box[20];
    for (uint32_t i = 0; i < 20; i++) {
        box[i] = find(root, FILTERS[i].name);
        if (FILTERS[i].text) setText(find(box[i], S_Txt), FILTERS[i].text);
    }
    const uint32_t releaseButton = find(root, S_ButtonRelease);
    const uint32_t releaseText = find(releaseButton, S_TxtRelease);
    setText(releaseText, TXT_RELEASE);
    setVisible(releaseButton, 0);
    addRef(heldCtrl);
    for (uint32_t i = 0; i < 20; i++) {
        const uint32_t obj = ld32(state), ctrl = ld32(state + 4);
        addRef(ctrl);
        addRef(heldCtrl);
        heapListener(CheckBox_addEventListener, box[i], FILTERS[i].vt, 0x20, [&](uint32_t h) {
            st32(h + 4, obj);
            st32(h + 8, ctrl);
            st32(h + 0xC, sceneType);
            st32(h + 0x10, heldObj);
            st32(h + 0x14, heldCtrl);
            st32(h + 0x18, releaseButton);
            st32(h + 0x1C, releaseText);
        });
    }
    {
        const uint32_t obj = ld32(state), ctrl = ld32(state + 4);
        addRef(ctrl);
        heapListener(fn::Widget_addTouchEventListener, releaseButton, VT_Release, 0x68, [&](uint32_t h) {
            for (uint32_t k = 0; k < 20; k++) st32(h + 4 + 4 * k, box[k]);
            st32(h + 0x54, releaseButton);
            st32(h + 0x58, releaseText);
            st32(h + 0x5C, obj);
            st32(h + 0x60, ctrl);
            st32(h + 0x64, sceneType);
        });
    }
    for (uint32_t i = 0; i < 20; i++)
        call(CheckBox_setSelectedState, {box[i], ld8(ld32(state) + 3 + i) != 0 ? 1u : 0u});
    {
        uint32_t any = 0;
        for (uint32_t i = 0; i < 20; i++) any |= ld8(heldObj + 3 + i);
        call(SetButtonEnabled, {releaseButton, releaseText, any ? 1u : 0u, 0});
    }

    // tabs, OK, Close
    const uint32_t ok = find(root, S_ButtonOK);
    const uint32_t close = find(root, S_CloseButton);
    const uint32_t tabSort = find(root, S_SwitchSort);
    const uint32_t tabFilter = find(root, S_SwitchFilter);
    const uint32_t hitLeft = find(root, S_HitLeft);
    const uint32_t hitRight = find(root, S_HitRight);
    setVisible(hitLeft, 1);
    const uint32_t leftButton = imageButton();
    call(ImageButton_setTargetView, {leftButton, hitLeft, fbits(1.0f)});
    vcall(hitLeft, node::addChild, {hitLeft, leftButton});
    setVisible(tabSort, 1);
    setText(find(tabSort, S_TxtSort), TXT_SORT_TAB);
    setText(find(tabSort, S_TxtFilter), TXT_FILTER_TAB);
    setVisible(hitRight, 1);
    const uint32_t rightButton = imageButton();
    call(ImageButton_setTargetView, {rightButton, hitRight, fbits(1.0f)});
    vcall(hitRight, node::addChild, {hitRight, rightButton});
    setVisible(tabFilter, 0);
    setText(find(tabFilter, S_TxtSort), TXT_SORT_TAB);
    setText(find(tabFilter, S_TxtFilter), TXT_FILTER_TAB);

    // the close handler's captures: first copy
    const uint32_t c1 = f.alloc(C_SIZE);
    {
        const uint32_t widgets[N_WIDGETS] = {
            box[0], box[1], box[2], box[3], box[4], box[5], box[6], box[7], box[8], box[9],
            box[10], box[11], box[12], box[13], box[14], box[15], box[16], box[17], box[18], box[19],
            sortBox[0], sortBox[1], sortBox[2], sortBox[3], sortBox[4], sortBox[5], sortBox[6], sortBox[8],
            sortBox[9], ok, close, leftButton, rightButton, releaseButton, root, shadow, layer};
        static_assert(SORT_PRICE == 7, "sortBox[7] is left out");
        for (uint32_t k = 0; k < N_WIDGETS; k++) st32(c1 + C_WIDGETS + 4 * k, widgets[k]);
    }
    call(fn::function_vb_copy, {c1 + C_FN, callback});
    st32(c1 + C_SCENE, sceneType);
    st32(c1 + C_STATE2, heldObj);
    st32(c1 + C_STATE2 + 4, heldCtrl);
    addRef(heldCtrl);
    // second copy, with the state and its bytes
    const uint32_t c2 = f.alloc(C_SIZE);
    st32(c2, ld32(state));
    st32(c2 + 4, ld32(state + 4));
    addRef(ld32(c2 + 4));
    for (uint32_t k = 0; k < N_BYTES; k++) st8(c2 + C_BYTES + k, ld8(ld32(state) + 1 + k));
    copyWords(c2 + C_WIDGETS, c1 + C_WIDGETS, N_WIDGETS * 4);
    functionCopy(c2 + C_FN, c1 + C_FN);
    st32(c2 + C_SCENE, ld32(c1 + C_SCENE));
    st32(c2 + C_STATE2, ld32(c1 + C_STATE2));
    st32(c2 + C_STATE2 + 4, ld32(c1 + C_STATE2 + 4));
    addRef(ld32(c2 + C_STATE2 + 4));

    setText(find(ok, S_TxtOK), TXT_OK);
    setVisible(ok, 1);
    addTouch(ok, VT_Ok, {});
    setVisible(close, 1);
    addTouch(close, VT_Close, {});
    // the tabs (on the image buttons over the hit areas)
    const uint32_t tabObj = ld32(state), tabCtrl = ld32(state + 4);
    if (tabCtrl) {
        call(fn::shared_add, {tabCtrl});
        call(fn::shared_add, {tabCtrl});
    }
    const auto tabCaptures = [&](uint32_t h, uint32_t tab) {
        st32(h + 0x20, dummyFilter);
        st32(h + 4, tab);
        st32(h + 8, tabObj);
        st32(h + 0xC, tabCtrl);
        st32(h + 0x10, tabSort);
        st32(h + 0x14, tabFilter);
        st32(h + 0x18, releaseButton);
        st32(h + 0x1C, dummySort);
    };
    heapListener(ImageButton_addTouchEventListener, leftButton, VT_TabSort, 0x24,
                 [&](uint32_t h) { tabCaptures(h, tabSort); });
    addRef(tabCtrl);
    heapListener(ImageButton_addTouchEventListener, rightButton, VT_TabFilter, 0x24,
                 [&](uint32_t h) { tabCaptures(h, tabFilter); });
    {
        const uint8_t mode = ld8(tabObj);
        st8(tabObj, mode);
        if (mode == 1) {
            setVisible(tabSort, 0);
            setVisible(tabFilter, 1);
            setVisible(releaseButton, 1);
            setVisible(dummySort, 0);
            setVisible(dummyFilter, 1);
        } else if (mode == 0) {
            setVisible(tabSort, 1);
            setVisible(tabFilter, 0);
            setVisible(releaseButton, 0);
            setVisible(dummySort, 1);
            setVisible(dummyFilter, 0);
        }
    }
    {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(fn::Node_runAction, {root, SceneUtil::createOpenPopupAnimation(root, none)});
        functionDestroy(none);
    }
    const uint32_t sm = sceneManager();
    Frame g;
    const uint32_t canClose = newFunction(g, VT_CanClose, {close});
    // third copy, moved into the heap handler
    const uint32_t c3 = g.alloc(C_SIZE);
    st32(c3, ld32(c2));
    st32(c3 + 4, ld32(c2 + 4));
    addRef(ld32(c3 + 4));
    for (uint32_t k = 0; k < N_BYTES; k++) st8(c3 + C_BYTES + k, ld8(c2 + C_BYTES + k));
    copyWords(c3 + C_WIDGETS, c2 + C_WIDGETS, N_WIDGETS * 4);
    functionCopy(c3 + C_FN, c2 + C_FN);
    st32(c3 + C_SCENE, ld32(c2 + C_SCENE));
    st32(c3 + C_STATE2, ld32(c2 + C_STATE2));
    st32(c3 + C_STATE2 + 4, ld32(c2 + C_STATE2 + 4));
    addRef(ld32(c3 + C_STATE2 + 4));
    const uint32_t onClose = g.alloc(24);
    st32(onClose + 16, 0);
    const uint32_t handler = call(fn::operator_new, {0xE8});
    if (handler) {
        st32(handler, VT_OnClose);
        st32(handler + 8, ld32(c3));
        st32(handler + 0xC, ld32(c3 + 4));
        st32(c3, 0);
        st32(c3 + 4, 0);
        for (uint32_t k = 0; k < N_BYTES; k++) st8(handler + 8 + C_BYTES + k, ld8(c3 + C_BYTES + k));
        copyWords(handler + 8 + C_WIDGETS, c3 + C_WIDGETS, N_WIDGETS * 4);
        call(fn::function_vb_move, {handler + 8 + C_FN, c3 + C_FN});
        st32(handler + 8 + C_STATE2, ld32(c3 + C_STATE2));
        st32(handler + 8 + C_STATE2 + 4, ld32(c3 + C_STATE2 + 4));
        st32(c3 + C_STATE2, 0);
        st32(c3 + C_STATE2 + 4, 0);
        st32(handler + 8 + C_SCENE, ld32(c3 + C_SCENE));
    }
    st32(onClose + 16, handler);
    call(fn::SceneManager_openPopup, {sm, canClose, onClose, 1});
    functionDestroy(onClose);
    releaseRef(ld32(c3 + C_STATE2 + 4));
    functionDestroy(c3 + C_FN);
    releaseRef(ld32(c3 + 4));
    functionDestroy(canClose);
    releaseRef(tabCtrl);
    releaseRef(ld32(c2 + C_STATE2 + 4));
    functionDestroy(c2 + C_FN);
    releaseRef(ld32(c2 + 4));
    releaseRef(ld32(c1 + C_STATE2 + 4));
    functionDestroy(c1 + C_FN);
    if (heldCtrl) {
        call(fn::shared_release, {heldCtrl});
        call(fn::shared_release, {heldCtrl});
    }
    return EPILOGUE_R0;
}
RW_FUNCTION(0x669D50, "IMedalSortFilter::openMedalSortPopup", medal_sort_popup::openMedalSortPopup);

// ------------------------------------------------------------------------------------------------ lambdas
// A sort key: the other keys back to unselected and touchable, this one not touchable, the key stored.
// Returns what the original leaves in r0: the touch type, or the state.
template <uint32_t KEY> uint32_t sortKeyTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    call(AudioPlaySystemSEDecide, {});
    for (uint32_t k = 0; k < 10; k++) {
        const uint32_t box = ld32(self + 8 + 4 * k);
        call(CheckBox_setSelectedState, {box, 0});
        vcall(box, V_setTouchEnabled, {box, 1});
    }
    const uint32_t own = ld32(self + 0x30);
    vcall(own, V_setTouchEnabled, {own, 0});
    const uint32_t state = ld32(self + 0x34);
    st8(state + 1, uint8_t(KEY));
    return state;
}
RW_FUNCTION(0x673834, "openMedalSortPopup Sort_Attribute lambda operator()", medal_sort_popup::sortKeyTouched<0>);
RW_FUNCTION(0x67358C, "openMedalSortPopup Sort_Rarity lambda operator()", medal_sort_popup::sortKeyTouched<1>);
RW_FUNCTION(0x6732E0, "openMedalSortPopup Sort_Lv lambda operator()", medal_sort_popup::sortKeyTouched<2>);
RW_FUNCTION(0x673034, "openMedalSortPopup Sort_Cost lambda operator()", medal_sort_popup::sortKeyTouched<3>);
RW_FUNCTION(0x672D88, "openMedalSortPopup Sort_Attack lambda operator()", medal_sort_popup::sortKeyTouched<4>);
RW_FUNCTION(0x672ADC, "openMedalSortPopup Sort_Defense lambda operator()", medal_sort_popup::sortKeyTouched<5>);
RW_FUNCTION(0x672830, "openMedalSortPopup Sort_Get lambda operator()", medal_sort_popup::sortKeyTouched<7>);
RW_FUNCTION(0x672584, "openMedalSortPopup Sort_Price lambda operator()", medal_sort_popup::sortKeyTouched<8>);
RW_FUNCTION(0x6722D8, "openMedalSortPopup Sort_Skill lambda operator()", medal_sort_popup::sortKeyTouched<9>);
RW_FUNCTION(0x67202C, "openMedalSortPopup Sort_Ability lambda operator()", medal_sort_popup::sortKeyTouched<10>);

// Sort_Up / Sort_Down: both back to unselected and touchable, then the OTHER button made touchable once more
// (the game's slip: the touched one stays touchable as well), the order stored.
template <uint32_t UP> uint32_t orderTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    call(AudioPlaySystemSEDecide, {});
    for (uint32_t k = 0; k < 2; k++) {
        const uint32_t box = ld32(self + 8 + 4 * k);
        call(CheckBox_setSelectedState, {box, 0});
        vcall(box, V_setTouchEnabled, {box, 1});
    }
    const uint32_t other = ld32(self + (UP ? 0xC : 8));
    vcall(other, V_setTouchEnabled, {other, 1});
    const uint32_t state = ld32(self + 0x10);
    st8(state + 2, uint8_t(UP));
    return state;
}
RW_FUNCTION(0x671E64, "openMedalSortPopup Sort_Up lambda operator()", medal_sort_popup::orderTouched<1>);
RW_FUNCTION(0x671CC0, "openMedalSortPopup Sort_Down lambda operator()", medal_sort_popup::orderTouched<0>);

// A filter check box: its state byte from the event (0 = selected), then "Release" enabled while any filter is
// on (read through the state copy taken on opening, the same object).
template <uint32_t BYTE> uint32_t filterChanged(uint32_t self, uint32_t, uint32_t typeRef) {
    const uint8_t on = ld32(typeRef) == 0 ? 1 : 0;
    call(AudioPlaySystemSEDecide, {});
    st8(ld32(self + 4) + BYTE, on);
    const uint32_t held = ld32(self + 0x10);
    uint32_t any = 0;
    for (uint32_t k = 3; k <= 22; k++) any |= ld8(held + k);
    return call(SetButtonEnabled, {ld32(self + 0x18), ld32(self + 0x1C), any ? 1u : 0u, 0});
}
RW_FUNCTION(0x671AD0, "openMedalSortPopup Filter_Power lambda operator()", medal_sort_popup::filterChanged<3>);
RW_FUNCTION(0x67188C, "openMedalSortPopup Filter_Speed lambda operator()", medal_sort_popup::filterChanged<4>);
RW_FUNCTION(0x671648, "openMedalSortPopup Filter_Magic lambda operator()", medal_sort_popup::filterChanged<5>);
RW_FUNCTION(0x671404, "openMedalSortPopup Filter_Uplight lambda operator()", medal_sort_popup::filterChanged<6>);
RW_FUNCTION(0x6711C0, "openMedalSortPopup Filter_Reverse lambda operator()", medal_sort_popup::filterChanged<7>);
RW_FUNCTION(0x670F7C, "openMedalSortPopup Filter_RareStarSet1 lambda operator()", medal_sort_popup::filterChanged<8>);
RW_FUNCTION(0x670D38, "openMedalSortPopup Filter_RareStarSet2 lambda operator()", medal_sort_popup::filterChanged<9>);
RW_FUNCTION(0x670AF4, "openMedalSortPopup Filter_RareStarSet3 lambda operator()", medal_sort_popup::filterChanged<10>);
RW_FUNCTION(0x6708B0, "openMedalSortPopup Filter_RareStarSet4 lambda operator()", medal_sort_popup::filterChanged<11>);
RW_FUNCTION(0x67066C, "openMedalSortPopup Filter_RareStarSet5 lambda operator()", medal_sort_popup::filterChanged<12>);
RW_FUNCTION(0x670428, "openMedalSortPopup Filter_RareStarSet6 lambda operator()", medal_sort_popup::filterChanged<13>);
RW_FUNCTION(0x6701E4, "openMedalSortPopup Filter_General lambda operator()", medal_sort_popup::filterChanged<14>);
RW_FUNCTION(0x66FFA0, "openMedalSortPopup Filter_Exp lambda operator()", medal_sort_popup::filterChanged<15>);
RW_FUNCTION(0x66FD5C, "openMedalSortPopup Filter_limit lambda operator()", medal_sort_popup::filterChanged<16>);
RW_FUNCTION(0x66FB18, "openMedalSortPopup Filter_Cost lambda operator()", medal_sort_popup::filterChanged<17>);
RW_FUNCTION(0x66F8D4, "openMedalSortPopup Filter_Evo lambda operator()", medal_sort_popup::filterChanged<18>);
RW_FUNCTION(0x66F690, "openMedalSortPopup Filter_Price lambda operator()", medal_sort_popup::filterChanged<19>);
RW_FUNCTION(0x66F44C, "openMedalSortPopup Filter_Favorite lambda operator()", medal_sort_popup::filterChanged<20>);
RW_FUNCTION(0x66F208, "openMedalSortPopup Filter_Skill_Set lambda operator()", medal_sort_popup::filterChanged<21>);
RW_FUNCTION(0x66EFC4, "openMedalSortPopup Filter_Skill_NotSet lambda operator()", medal_sort_popup::filterChanged<22>);

// "Release": every filter unselected, the button disabled, the filter bytes cleared
uint32_t releaseTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    call(AudioPlaySystemSEDecide, {});
    for (uint32_t k = 0; k < 20; k++) call(CheckBox_setSelectedState, {ld32(self + 4 + 4 * k), 0});
    call(SetButtonEnabled, {ld32(self + 0x54), ld32(self + 0x58), 0, 0});
    for (uint32_t k = 3; k <= 22; k++) st8(ld32(self + 0x5C) + k, 0);
    return ld32(self + 0x5C);
}
RW_FUNCTION(0x66ECF0, "openMedalSortPopup Button_Release lambda operator()", medal_sort_popup::releaseTouched);

// The Sort / Filter tab areas: unless that tab already shows, the mode stored and the tabs, "Release" and the
// two pages switched.
template <uint32_t MODE> uint32_t tabTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    const uint32_t tab = ld32(self + 4);
    const uint32_t shown = vcall(tab, node::isVisible, {tab});
    if (shown) return shown;
    call(AudioPlaySystemSEDecide, {});
    st8(ld32(self + 8), uint8_t(MODE));
    // sort tab, filter tab, Release, sort page, filter page
    const uint32_t vis[5] = {MODE ? 0u : 1u, MODE, MODE, MODE ? 0u : 1u, MODE};
    uint32_t r = 0;
    for (uint32_t k = 0; k < 5; k++) {
        const uint32_t n = ld32(self + 0x10 + 4 * k);
        r = vcall(n, node::setVisible, {n, vis[k]});
    }
    return r;
}
RW_FUNCTION(0x66E9FC, "openMedalSortPopup Sort tab lambda operator()", medal_sort_popup::tabTouched<0>);
RW_FUNCTION(0x66E834, "openMedalSortPopup Filter tab lambda operator()", medal_sort_popup::tabTouched<1>);

// The popup closes (OK = true): OK saves the state and calls back with "any filter on"; Close puts back the
// bytes from the opening. Either way the widgets stop taking touches and the popup animates out.
uint32_t popupClosed(uint32_t self, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    if (ok) {
        call(AppState_save, {call(fn::AppState_getInstance, {})});
    } else {
        const uint32_t state = ld32(self + 8);
        for (uint32_t k = 0; k < N_BYTES; k++) st8(state + 1 + k, ld8(self + 0x10 + k));
    }
    for (uint32_t off = 0x28; off <= 0xA0; off += 4) {
        const uint32_t w = ld32(self + off);
        vcall(w, V_setTouchEnabled, {w, 0});
    }
    st8(ld32(self + 0xA4) + IMAGE_BUTTON_ENABLED, 0);
    st8(ld32(self + 0xA8) + IMAGE_BUTTON_ENABLED, 0);
    const uint32_t release = ld32(self + 0xAC);
    vcall(release, V_setTouchEnabled, {release, 0});
    const uint32_t root = ld32(self + 0xB0), shadow = ld32(self + 0xB4), layer = ld32(self + 0xB8);
    {
        Frame f;
        const uint32_t done = newFunction(f, VT_RemoveNodes, {shadow, root, layer});
        call(fn::Node_runAction, {root, SceneUtil::createClosePopupAnimation(root, done)});
        functionDestroy(done);
    }
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), 0})});
    if (ok && ld32(self + 0xD0)) {
        const uint32_t state = ld32(self + 0xDC);
        uint32_t any = 0;
        for (uint32_t k = 3; k <= 22; k++) any |= ld8(state + k);
        call(function_vb_call, {self + 0xC0, any ? 1u : 0u});
    }
    return 0;
}
RW_FUNCTION(0x66E1A8, "openMedalSortPopup close handler lambda operator()", medal_sort_popup::popupClosed);

// the close animation's end: the popup off the shadow, the shadow off the layer
uint32_t removeNodes(uint32_t self) {
    const uint32_t shadow = ld32(self + 4);
    vcall(shadow, node::removeChild, {shadow, ld32(self + 8), 1});
    const uint32_t layer = ld32(self + 0xC);
    return vcall(layer, node::removeChild, {layer, shadow, 1});
}
RW_FUNCTION(0x66E612, "openMedalSortPopup close animation end lambda operator()", medal_sort_popup::removeNodes);

// ---- __clone() of the heap lambdas (copies, the state referenced once more)
uint32_t cloneOrder(uint32_t self, uint32_t vt) {
    const uint32_t c = call(fn::operator_new, {0x18});
    if (c) {
        st32(c, vt);
        copyWords(c + 4, self + 4, 0xC);
        st32(c + 0x10, ld32(self + 0x10));
        st32(c + 0x14, ld32(self + 0x14));
        addRef(ld32(c + 0x14));
    }
    return c;
}
uint32_t cloneUp(uint32_t self) { return cloneOrder(self, VT_Up); }
uint32_t cloneDown(uint32_t self) { return cloneOrder(self, VT_Down); }
RW_FUNCTION(0x671DB4, "openMedalSortPopup Sort_Up lambda __clone()", medal_sort_popup::cloneUp);
RW_FUNCTION(0x671C10, "openMedalSortPopup Sort_Down lambda __clone()", medal_sort_popup::cloneDown);

uint32_t cloneRelease(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0x68});
    if (c) {
        st32(c, VT_Release);
        copyWords(c + 4, self + 4, 0x58);
        st32(c + 0x5C, ld32(self + 0x5C));
        st32(c + 0x60, ld32(self + 0x60));
        addRef(ld32(c + 0x60));
        st32(c + 0x64, ld32(self + 0x64));
    }
    return c;
}
RW_FUNCTION(0x66EC38, "openMedalSortPopup Button_Release lambda __clone()", medal_sort_popup::cloneRelease);

uint32_t cloneClosed(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0xE8});
    if (c) {
        st32(c, VT_OnClose);
        st32(c + 8, ld32(self + 8));
        st32(c + 0xC, ld32(self + 0xC));
        addRef(ld32(c + 0xC));
        for (uint32_t k = 0; k < N_BYTES; k++) st8(c + 0x10 + k, ld8(self + 0x10 + k));
        copyWords(c + 0x28, self + 0x28, N_WIDGETS * 4);
        call(fn::function_vb_copy, {c + 0xC0, self + 0xC0});
        st32(c + 0xD8, ld32(self + 0xD8));
        st32(c + 0xDC, ld32(self + 0xDC));
        st32(c + 0xE0, ld32(self + 0xE0));
        addRef(ld32(c + 0xE0));
    }
    return c;
}
RW_FUNCTION(0x66DFD0, "openMedalSortPopup close handler lambda __clone()", medal_sort_popup::cloneClosed);

}  // namespace medal_sort_popup

