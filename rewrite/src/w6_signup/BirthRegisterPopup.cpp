// PopupRegister::openBirthRegisterPopup: the birthday popup of a new account. The age rules (misc 900-904), the
// input panel (year / month plates with their pull-down lists), the check panel ("born in <year>"? Redo /
// Register), and the touch callbacks that tie them together; the done callback runs once the birthday is sent.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;   // (node, const function<void()>&)
constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
constexpr uint32_t MapUintUint_at = 0x5681A5;
constexpr uint32_t clock_now = 0xC60645, clock_to_time_t = 0xC6067D, libc_localtime = 0x4101B0;
constexpr uint32_t VectorInt_pushSlowCopy = 0x4D6CC1, VectorInt_pushSlowMove = 0x47D709;
constexpr uint32_t VectorInt_dtor = 0x478961;
constexpr uint32_t BirthRegisterScrollView_create = 0x5622BD;
constexpr uint32_t BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9, IScrollProtocol_setIndexPrev = 0x4F731D;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_copy2 = 0x47F905;   // std::function<void()>(const&)
constexpr uint32_t function_vi_swap = 0x5428E1;            // std::function<void(int)>::swap
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;  // (node, const function<void()>&)
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D;   // (api, phase, a, b, stack 4 words, callback)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;           // (api, const function<void(bool)>&, bool)
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t VT_Sent = 0xF58F3C;                     // $_8 {5 words, done callback at +0x20} (0x38)
constexpr uint32_t VT_Closed = 0xF58F68;                   // $_8's close-animation end {layer, shadow, root, done}
constexpr uint32_t VT_ErrorClosed = 0xF58F94;              // {}
constexpr uint32_t API_RETRY_DIALOG = 0x14;
constexpr uint32_t PHASE_BIRTHDAY = 1;
constexpr uint32_t V_addChild = 0xF0, V_setVisible = 0xB0, V_setContentSize = 0xA8, V_getContentSize = 0xAC;
constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214;
constexpr uint32_t VT_YearClose = 0xF591FC, VT_MonthClose = 0xF59120, VT_Redo = 0xF58FC0;   // {}
constexpr uint32_t VT_YearFall = 0xF59178, VT_YearPicked = 0xF5914C;      // heap, 7 captures
constexpr uint32_t VT_MonthFall = 0xF5909C, VT_MonthPicked = 0xF59070;    // heap, 7 captures
constexpr uint32_t VT_Register = 0xF58FEC;                 // heap, 6 captures
constexpr uint32_t VT_Confirm = 0xF58F10;                  // heap 0x48: 9 words + the done callback at +0x30
constexpr uint32_t S_Json = 0xDFF109, S_Base = 0xDFF134, S_TxtTitle = 0xDFF142, S_Wording1 = 0xDFF14C;
constexpr uint32_t S_Wording2 = 0xDF863B, S_Age1 = 0xDFF159, S_Rule = 0xDFF162, S_Age2 = 0xDFF16B;
constexpr uint32_t S_Age3 = 0xDFF174, S_Age4 = 0xDFF17D, S_Age5 = 0xDFF186, S_Input = 0xDFF18F;
constexpr uint32_t S_YearLabel = 0xDFF19E, S_IntStr = 0xDFF1AD, S_MonthLabel = 0xDFF1B2, S_TxtRegister = 0xDFF1C2;
constexpr uint32_t S_Check = 0xDFF1CF, S_BirthLabel = 0xDFF1DE, S_TxtRedo = 0xDFF1EE, S_Year = 0xDFF1F7;
constexpr uint32_t S_Month = 0xDFF205, S_PlateYear = 0xDFF214, S_ButtonFall = 0xDFF222, S_PlateMonth = 0xDFF22E;
constexpr uint32_t S_ButtonRegister = 0xDFF23D, S_ButtonRedo = 0xDFF24D, S_ScrollArea = 0xDF9B59;
constexpr uint32_t S_CloseButton = 0xDF8D2F;
constexpr uint32_t API_MISC = 0x1B0;
constexpr uint32_t SCROLL_PICKED = 0x638, SCROLL_SELECTED = 0x650;   // BirthRegisterScrollView
constexpr uint32_t DEFAULT_YEAR = 1995, YEAR_ROW = 82;
constexpr uint32_t EPILOGUE_R0 = 0x2DDC;

uint32_t misc(uint32_t id) {
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, id);
    return ld32(call(MapUintUint_at, {ld32(call(fn::APIManager_getInstance, {}) + API_MISC), key}));
}

uint32_t shadowed(uint32_t parent, uint32_t name, uint32_t len) {
    const uint32_t text = findNode(parent, name, len);
    SceneUtil::setShadow(text);
    return text;
}

// a shadowed label from the text table, or a text-table format filled with values read after the text
uint32_t label(uint32_t parent, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = shadowed(parent, name, len);
    setTextId(text, id);
    return text;
}
template <typename Args>
uint32_t formatted(uint32_t parent, uint32_t name, uint32_t len, int32_t id, Args values) {
    const uint32_t text = shadowed(parent, name, len);
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t fmt = textDataStr(td);
    const auto v = values();
    const uint32_t buf = v.size() == 1 ? stringBuffer(f, {fmt, v[0]}) : stringBuffer(f, {fmt, v[0], v[1]});
    setTextC(text, buf);
    sharedRelease(td);
    return text;
}
struct Vals {
    uint32_t n, a[2];
    uint32_t size() const { return n; }
    uint32_t operator[](uint32_t i) const { return a[i]; }
};

// vector<int>::push_back as inlined: in place when there is room, else the slow path with the value's address
void push(uint32_t vec, uint32_t valueRef, uint32_t slow) {
    const uint32_t end = ld32(vec + 4);
    if (end != ld32(vec + 8)) {
        uint32_t next = 0;
        if (end) {
            st32(end, ld32(valueRef));
            next = ld32(vec + 4);
        }
        st32(vec + 4, next + 4);
    } else {
        call(slow, {vec, valueRef});
    }
}

// a std::function whose lambda lives on the heap ({vtable, captures})
uint32_t heapLambda(Frame &f, uint32_t vt, uint32_t size, std::initializer_list<uint32_t> captures) {
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = call(fn::operator_new, {size});
    if (obj) {
        st32(obj, vt);
        uint32_t k = 4;
        for (uint32_t c : captures) { st32(obj + k, c); k += 4; }
    }
    st32(fo + 16, obj);
    return fo;
}
void touch(uint32_t widget, uint32_t fo) {
    call(fn::Widget_addTouchEventListener, {widget, fo});
    functionDestroy(fo);
}
void onPicked(uint32_t scroll, uint32_t fo) {
    call(function_vi_swap, {fo, scroll + SCROLL_PICKED});
    functionDestroy(fo);
}

// a BirthRegisterScrollView filling the list area of `panel`
uint32_t newList(uint32_t panel, uint32_t &area) {
    area = findNode(panel, S_ScrollArea, 0xB);
    const uint32_t hidden = vcall(area, V_setVisible, {area, 0});
    const uint32_t list = call(BirthRegisterScrollView_create, {hidden});
    vcall(list, V_setContentSize, {list, vcall(area, V_getContentSize, {area})});
    return list;
}
}  // namespace

namespace popup_register {

uint32_t openBirthRegisterPopup(uint32_t done) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x20C});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(Node_runAction, {shadow, call(FadeTo_create, {0x3DCCCCCD, opacity})});
    vcall(layer, V_addChild, {layer, shadow});
    uint32_t root;
    {
        Frame f;
        const uint32_t s = newStr(f, S_Json, 0x2A);
        root = call(SceneUtil_createNodeWithSceneFile, {s});
        strFree(s);
    }
    vcall(layer, V_addChild, {layer, root});

    // the rules
    const uint32_t base = findNode(root, S_Base, 0xD);
    label(base, S_TxtTitle, 9, 109140002);
    titleLabel(base, S_Wording1, 0xC, 109140022);
    titleLabel(base, S_Wording2, 0xC, 109140023);
    const uint32_t age1 = formatted(base, S_Age1, 8, 109140010, [] { return Vals{1, {misc(901)}}; });
    formatted(age1, S_Rule, 8, 109140013, [] { return Vals{1, {misc(903)}}; });
    const uint32_t age2 = formatted(base, S_Age2, 8, 109140011, [] {
        const uint32_t from = misc(901);
        return Vals{2, {from, misc(902) - 1}};
    });
    formatted(age2, S_Rule, 8, 109140013, [] { return Vals{1, {misc(904)}}; });
    const uint32_t age3 = formatted(base, S_Age3, 8, 109140012, [] { return Vals{1, {misc(902)}}; });
    label(age3, S_Rule, 8, 109140014);
    const uint32_t age4 = formatted(base, S_Age4, 8, 109140010, [] { return Vals{1, {misc(900)}}; });
    label(age4, S_Rule, 8, 109140015);
    const uint32_t age5 = formatted(base, S_Age5, 8, 109140012, [] { return Vals{1, {misc(900)}}; });
    label(age5, S_Rule, 8, 109140016);

    // the input panel
    const uint32_t input = findNode(root, S_Input, 0xE);
    label(input, S_Wording1, 0xC, 109140001);
    {
        const uint32_t w2 = findNode(input, S_Wording2, 0xC);
        vcall(w2, V_setVisible, {w2, 0});
    }
    const uint32_t yearLabel = shadowed(input, S_YearLabel, 0xE);
    {
        Frame f;
        const uint32_t td = textData(f, 109140003);
        setTextC(yearLabel, stringBuffer(f, {S_IntStr, DEFAULT_YEAR, textDataStr(td)}));
        sharedRelease(td);
    }
    const uint32_t monthLabel = shadowed(input, S_MonthLabel, 0xF);
    {
        Frame f;
        const uint32_t td = textData(f, 109140004);
        setTextC(monthLabel, stringBuffer(f, {S_IntStr, 1, textDataStr(td)}));
        sharedRelease(td);
    }
    label(input, S_TxtRegister, 0xC, 109140009);
    vcall(input, V_setVisible, {input, 1});

    // the check panel
    const uint32_t check = findNode(root, S_Check, 0xE);
    vcall(check, V_setVisible, {check, 0});
    label(check, S_Wording1, 0xC, 109140017);
    const uint32_t birthLabel = formatted(check, S_BirthLabel, 0xF, 109140007, [] { return Vals{1, {DEFAULT_YEAR}}; });
    label(check, S_Wording2, 0xC, 109140018);
    label(check, S_TxtRedo, 8, 109140008);
    label(check, S_TxtRegister, 0xC, 109140009);

    // the year / month lists
    const uint32_t yearNode = findNode(root, S_Year, 0xD);
    vcall(yearNode, V_setVisible, {yearNode, 0});
    const uint32_t monthNode = findNode(root, S_Month, 0xE);
    vcall(monthNode, V_setVisible, {monthNode, 0});
    Frame f;
    const uint32_t years = f.alloc(12);
    st32(years, 0);
    st32(years + 4, 0);
    st32(years + 8, 0);
    {
        const uint32_t now = f.alloc(8), t = f.alloc(4);
        call(clock_now, {now});
        st32(t, call(clock_to_time_t, {now}));
        call(libc_localtime, {t});
    }
    const uint32_t year = f.alloc(4);
    for (uint32_t i = misc(905);; i++) {
        st32(year, i);
        if (i >= misc(906)) break;
        push(years, year, VectorInt_pushSlowCopy);
    }
    {
        const uint32_t last = f.alloc(4);
        st32(last, misc(906));
        const uint32_t end = ld32(years + 4);
        if (end < ld32(years + 8)) {
            uint32_t next = 0;
            if (end) {
                st32(end, ld32(last));
                next = ld32(years + 4);
            }
            st32(years + 4, next + 4);
        } else {
            call(VectorInt_pushSlowMove, {years, last});
        }
    }
    uint32_t yearArea;
    const uint32_t yearList = newList(yearNode, yearArea);
    call(BaseScrollViewInt_setItemIDs, {yearList, years});
    st32(yearList + SCROLL_SELECTED, DEFAULT_YEAR);
    call(IScrollProtocol_setEnabledScroll, {yearList, 1});
    call(IScrollProtocol_setIndexPrev, {yearList, YEAR_ROW, 0});
    call(IScrollProtocol_setEnabledScroll, {yearList, 0});
    vcall(yearArea, V_addChild, {yearArea, yearList});

    const uint32_t months = f.alloc(12), month = f.alloc(4);
    st32(month, 1);
    st32(months, 0);
    st32(months + 4, 0);
    st32(months + 8, 0);
    for (uint32_t m = 2;; m++) {
        if (ld32(months + 4) == ld32(months + 8)) {
            call(VectorInt_pushSlowCopy, {months, month});
        } else {
            const uint32_t end = ld32(months + 4);
            uint32_t next = 0;
            if (end) {
                st32(end, m - 1);
                next = ld32(months + 4);
            }
            st32(months + 4, next + 4);
        }
        st32(month, m);
        if (m > 12) break;
    }
    uint32_t monthArea;
    const uint32_t monthList = newList(monthNode, monthArea);
    st32(monthList + SCROLL_SELECTED, 1);
    call(BaseScrollViewInt_setItemIDs, {monthList, months});
    vcall(monthArea, V_addChild, {monthArea, monthList});

    const uint32_t yearFall = findNode(findNode(input, S_PlateYear, 0xD), S_ButtonFall, 0xB);
    const uint32_t yearClose = findNode(yearNode, S_CloseButton, 0xC);
    const uint32_t monthFall = findNode(findNode(input, S_PlateMonth, 0xE), S_ButtonFall, 0xB);
    const uint32_t monthClose = findNode(monthNode, S_CloseButton, 0xC);
    const uint32_t registerButton = findNode(input, S_ButtonRegister, 0xF);
    const uint32_t redo = findNode(check, S_ButtonRedo, 0xB);
    const uint32_t confirm = findNode(check, S_ButtonRegister, 0xF);

    // the confirm closure's captures (the popup, its layer and shadow, a copy of the done callback)
    const uint32_t closure = f.alloc(0x28);
    st32(closure, redo);
    st32(closure + 4, confirm);
    st32(closure + 8, root);
    st32(closure + 0xC, layer);
    st32(closure + 0x10, shadow);
    const uint32_t doneCopy = closure + 0x14;
    call(function_vv_copy, {doneCopy, done});

    {
        Frame g;
        touch(yearClose, newFunction(g, VT_YearClose, {}));
    }
    {
        Frame g;
        touch(yearFall, heapLambda(g, VT_YearFall, 0x20,
                                   {yearArea, yearList, yearNode, monthFall, registerButton, yearFall, yearClose}));
    }
    {
        Frame g;
        onPicked(yearList, heapLambda(g, VT_YearPicked, 0x20, {yearArea, yearList, yearNode, monthFall,
                                                                registerButton, yearFall, yearLabel}));
    }
    {
        Frame g;
        touch(monthClose, newFunction(g, VT_MonthClose, {}));
    }
    {
        Frame g;
        touch(monthFall, heapLambda(g, VT_MonthFall, 0x20, {monthArea, monthList, monthNode, yearFall,
                                                             registerButton, monthFall, monthClose}));
    }
    {
        Frame g;
        onPicked(monthList, heapLambda(g, VT_MonthPicked, 0x20, {monthArea, monthList, monthNode, yearFall,
                                                                  registerButton, monthFall, monthLabel}));
    }
    {
        Frame g;
        touch(registerButton,
              heapLambda(g, VT_Register, 0x1C, {input, birthLabel, yearList, monthList, check, redo}));
    }
    {
        Frame g;
        touch(redo, newFunction(g, VT_Redo, {}));
    }
    {
        Frame g;
        const uint32_t copy = g.alloc(20);
        functionCopy(copy, doneCopy);
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = call(fn::operator_new, {0x48});
        if (obj) {
            st32(obj, VT_Confirm);
            const uint32_t words[9] = {redo, confirm, yearList, monthList, ld32(closure), ld32(closure + 4),
                                       ld32(closure + 8), ld32(closure + 0xC), ld32(closure + 0x10)};
            for (uint32_t k = 0; k < 9; k++) st32(obj + 8 + 4 * k, words[k]);
            call(function_vv_copy2, {obj + 0x30, copy});
        }
        st32(fo + 16, obj);
        touch(confirm, fo);
        functionDestroy(copy);
    }
    {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(Node_runAction, {root, call(SceneUtil_createOpenPopupAnimation, {root, none})});
        functionDestroy(none);
    }
    functionDestroy(doneCopy);
    call(VectorInt_dtor, {months});
    call(VectorInt_dtor, {years});
    return EPILOGUE_R0;
}
RW_FUNCTION(0x563344, "PopupRegister::openBirthRegisterPopup", popup_register::openBirthRegisterPopup);

// $_9: Register on the check panel - the buttons off, the birthday sent with the tutorial progress (phase 1)
uint32_t confirmTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t redo = ld32(self + 8), confirm = ld32(self + 0xC);
    vcall(redo, V_setTouchEnabled, {redo, 0});
    vcall(confirm, V_setTouchEnabled, {confirm, 0});
    call(SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t month = ld32(ld32(self + 0x14) + SCROLL_SELECTED);
    const uint32_t year = ld32(ld32(self + 0x10) + SCROLL_SELECTED);
    Frame f;
    uint32_t words[5];
    for (uint32_t k = 0; k < 5; k++) words[k] = ld32(self + 0x18 + 4 * k);
    const uint32_t doneCopy = f.alloc(20);
    call(function_vv_copy, {doneCopy, self + 0x30});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = call(fn::operator_new, {0x38});
    if (obj) {
        st32(obj, VT_Sent);
        for (uint32_t k = 0; k < 5; k++) st32(obj + 8 + 4 * k, words[k]);
        call(function_vv_copy2, {obj + 0x20, doneCopy});
    }
    st32(fo + 16, obj);
    call(APIManager_executeTutorialProgress, {api, PHASE_BIRTHDAY, year, month, 0, 0, 0, 0, fo});
    functionDestroy(fo);
    functionDestroy(doneCopy);
    return 0;
}
RW_FUNCTION(0x566848, "PopupRegister::openBirthRegisterPopup::$_9 operator() (register)", popup_register::confirmTouched);

// $_8: the birthday sent - the popup closes (then the done callback), or the error dialog and the buttons back on
uint32_t birthdaySent(uint32_t self, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    const uint32_t redo = ld32(self + 8);
    if (ok) {
        vcall(redo, V_setTouchEnabled, {redo, 1});
        call(SM_closeCurrentPopup, {sceneManager(), 1});
        vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
        const uint32_t root = ld32(self + 0x10), layer = ld32(self + 0x14), shadow = ld32(self + 0x18);
        Frame f;
        const uint32_t doneCopy = f.alloc(20);
        call(function_vv_copy, {doneCopy, self + 0x20});
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, VT_Closed);
            st32(obj + 8, layer);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, root);
            call(function_vv_copy2, {obj + 0x18, doneCopy});
        }
        st32(fo + 16, obj);
        call(Node_runAction, {root, call(SceneUtil_createClosePopupAnimation, {root, fo})});
        functionDestroy(fo);
        functionDestroy(doneCopy);
        const uint32_t shade = ld32(self + 0x18);
        call(Node_runAction, {shade, call(FadeTo_create, {0x3DCCCCCD, 0})});
        return 0;
    }
    if (ld8(call(fn::APIManager_getInstance, {}) + API_RETRY_DIALOG)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_ErrorClosed, {});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 1});
    vcall(ld32(self + 0xC), V_setTouchEnabled, {ld32(self + 0xC), 1});
    return 0;
}
RW_FUNCTION(0x566B70, "PopupRegister::openBirthRegisterPopup::$_8 operator() (sent)", popup_register::birthdaySent);

}  // namespace popup_register
