// StartDeckEditDialog: building the dialog (initLayout and the parts it sets up).
#include "StartDeckEditDialog_lib.hpp"

using namespace sded;

namespace {
constexpr uint32_t AudioManager_getInstance = 0x5BECFD;
constexpr uint32_t AudioManager_getLayer = 0x5BF33D;
constexpr uint32_t AudioLayer_playSE = 0x5BF785;
constexpr uint32_t SE_POPUP_OPEN = 0x1E56B954;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;

// a touch listener lambda {self, extra} on a widget
void addTouch2(uint32_t widget, uint32_t vtable, uint32_t self, uint32_t extra) {
    Frame f;
    uint32_t fo = inlineFn(f, vtable, {self, extra});
    call(scene::fn::Widget_addTouchEventListener, {widget, fo});
    functionDestroy(fo);
}

// a widget used as a button: touch on, visible, opaque
void enableButton(uint32_t w) {
    vcall(w, VS_setTouchEnabled, {w, 1});
    vcall(w, VS_setVisible, {w, 1});
    vcall(w, VS_setOpacity, {w, 0xFF});
}

// Upper bound in a sorted int list: the first element > value
uint32_t upperBound(uint32_t first, int32_t count, int32_t value) {
    while (count != 0) {
        int32_t half = count / 2;
        uint32_t mid = first + uint32_t(half) * 4;
        if (int32_t(ld32(mid)) > value) {
            count = half;
        } else {
            count = count - 1 - half;
            first = mid + 4;
        }
    }
    return first;
}
}  // namespace

// The info panel, the deck pager, the buttons (close / OK / skip) and the open animation; tutorial 106.
void StartDeckEditDialog::initLayout(Addr self) {
    Self *s = S(self);
    {
        Frame f;
        uint32_t layer = f.alloc(8);
        call(AudioManager_getLayer, {layer, call(AudioManager_getInstance, {}), 0});
        call(AudioLayer_playSE, {ld32(layer), SE_POPUP_OPEN, 0, 0});
        release(ld32(layer + 4));
    }
    initInfo(self);
    initDeckEdit(self);
    uint32_t plus = find(s->root, S_PLUS, 4);
    vcall(plus, VS_setVisible, {plus, 0});
    uint32_t support = find(s->root, S_SUPPORT, 7);
    vcall(support, VS_setVisible, {support, 0});
    uint32_t supportClose = find(support, S_BUTTON_CLOSE, 12);
    vcall(s->root, VS_setVisible, {s->root, 1});
    {
        uint32_t root = s->root;
        Frame f;
        uint32_t fo = inlineFn(f, VT_layout_3, {self});
        uint32_t anim = call(SceneUtil_createOpenPopupAnimation, {root, fo});
        call(wl::Node_runAction, {root, call(EaseSineOut_create, {anim})});
        functionDestroy(fo);
    }
    s->closeButton = find(s->root, S_BUTTON_CLOSE, 12);
    addTouch2(s->closeButton, VT_layout_4, self, supportClose);
    {
        uint32_t sm = sceneManager();
        Frame f;
        uint32_t canClose = inlineFn(f, VT_layout_5, {self});
        uint32_t onClose = inlineFn(f, VT_layout_6, {self});
        call(SceneManager_openPopup, {sm, canClose, onClose, 1});
        functionDestroy(onClose);
        functionDestroy(canClose);
    }
    uint32_t button = find(s->root, S_BUTTON, 6);
    uint32_t ok = find(button, S_BUTTON_OK, 9);
    enableButton(ok);
    addTouch2(ok, VT_layout_7, self, supportClose);
    uint32_t skip = find(s->root, S_SKIP, 4);
    uint32_t skipButton = find(skip, S_BUTTON_SKIP, 11);
    enableButton(skipButton);
    addTouch2(skipButton, VT_layout_8, self, supportClose);
    updateDeckLayout(self);
    uint32_t announce = find(s->root, S_ANNOUNCE, 8);
    if (announce) vcall(announce, VS_setVisible, {announce, 0});
    Frame f;
    uint32_t empty = newEmptyFunction(f);
    openTutorial106(self, empty);
    functionDestroy(empty);
}
RW_FUNCTION(0x573A6C, "StartDeckEditDialog::initLayout", StartDeckEditDialog::initLayout);

// RaidInfo / ColosseumInfo by stage type
void StartDeckEditDialog::initInfo(Addr self) {
    Self *s = S(self);
    uint32_t raidInfo = find(s->root, S_RAID_INFO, 8);
    uint32_t colInfo = find(s->root, S_COLOSSEUM_INFO, 13);
    int32_t type = s->stageType;
    if (uint32_t(type) > 4) return;
    auto addPanel = [&](uint32_t json) {
        uint32_t panel = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), json});
        Frame f;
        uint32_t name = newStr(f, S_RAID_PANEL, 9);
        vcall(raidInfo, VS_addChildZName, {raidInfo, panel, 0, name});
        strFree(name);
        return panel;
    };
    switch (type) {
    case 0:
    case 1: {
        vcall(raidInfo, VS_removeAllChildren, {raidInfo});
        addPanel(S_RAID_INFO_JSON);
        uint32_t stage = ld32(sceneManager() + SM_STAGE_ID);
        call(AdvStageScrollView_setBossInfo, {raidInfo, stage, uint32_t(s->stageType.get())});
        vcall(colInfo, VS_setVisible, {colInfo, 0});
        break;
    }
    case 2: {
        vcall(raidInfo, VS_setVisible, {raidInfo, 0});
        vcall(colInfo, VS_setVisible, {colInfo, 1});
        uint32_t list = ld32(api() + API_COLOSSEUM);
        uint32_t begin = ld32(list + 8), end = ld32(list + 0xC);
        uint32_t idx = uint32_t(s->stageIndex.get());
        if (!(uint32_t(int32_t(end - begin) >> 4) > idx)) {
            call(wl::VECTOR_AT_THROW, {list + 8});
            return;
        }
        call(AdvStageScrollView_setColosseumInfo, {colInfo, begin + idx * 16});
        break;
    }
    case 3: {
        vcall(raidInfo, VS_removeAllChildren, {raidInfo});
        vcall(colInfo, VS_setVisible, {colInfo, 0});
        uint32_t panel = addPanel(S_RAID_INFO2_JSON);
        call(AdvStageScrollView_setRaidBossInfo, {panel, ld32(api() + API_RAID_INFO)});
        break;
    }
    case 4: {
        vcall(raidInfo, VS_removeAllChildren, {raidInfo});
        vcall(colInfo, VS_setVisible, {colInfo, 0});
        uint32_t panel = addPanel(S_RAID_INFO2_JSON);
        uint32_t a = api();
        uint32_t begin = ld32(a + API_RAID_LIST), end = ld32(a + API_RAID_LIST + 4);
        uint32_t count = uint32_t(int32_t(end - begin) >> 4) * 0xEEEEEEEFu;
        uint32_t idx = uint32_t(s->stageIndex.get());
        if (!(count > idx)) {
            call(wl::VECTOR_AT_THROW, {a + API_RAID_LIST});
            return;
        }
        call(AdvStageScrollView_setRaidBossInfo, {panel, begin + idx * 0xF0});
        break;
    }
    }
}
RW_FUNCTION(0x57412C, "StartDeckEditDialog::initInfo", StartDeckEditDialog::initInfo);

// The keyblade deck pager: one page per owned keyblade (the equipped one first shown), arrows when more than one.
void StartDeckEditDialog::initDeckEdit(Addr self) {
    Self *s = S(self);
    uint32_t kb = find(s->root, S_KB, 2);
    vcall(kb, VS_removeAllChildren, {kb});
    uint32_t deck = find(s->root, S_DECK, 4);
    uint32_t base = find(deck, S_BASE, 4);
    Frame f;
    uint32_t size1 = f.alloc(8);
    call(Size_copy, {size1, vcall(base, VS_getContentSize, {base})});
    vcall(deck, VS_removeAllChildren, {deck});
    s->deckSlide = call(BaseSlideView_create, {});
    uint32_t slide = s->deckSlide;
    vcall(slide, VS_slide31C, {slide, 1});
    uint32_t b1 = find(s->root, S_BASE, 4);
    uint32_t b2 = find(b1, S_BASE, 4);
    uint32_t size2 = f.alloc(8);
    call(Size_copy, {size2, vcall(b2, VS_getContentSize, {b2})});
    st32(size2, ld32(size1));
    slide = s->deckSlide;
    vcall(slide, VS_setContentSize, {slide, size2});
    slide = s->deckSlide;
    call(BaseSlideView_setScreenWidth, {slide, ld32(vcall(b2, VS_getContentSize, {b2}))});
    uint32_t a = api();
    uint32_t selected = 0;
    uint32_t counter = f.alloc(4);
    st32(counter, 0);
    uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t list = ld32(a + API_KEYBLADES);
    if (ld32(list + 0x20)) {
        uint32_t off = 0, k = 0;
        uint32_t next;
        do {
            uint32_t equipped = safe8(list, off + 0x4C);
            uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(wl::IntVector_pushBackSlow, {ids, counter});
            } else {
                uint32_t v = 0;
                if (end) {
                    st32(end, k);
                    v = ld32(ids + 4);
                }
                st32(ids + 4, v + 4);
            }
            next = k + 1;
            st32(counter, next);
            if (equipped) selected = k;
            off += 0x58;
            list = ld32(a + API_KEYBLADES);
            k = next;
        } while (next < ld32(list + 0x20));
    }
    {
        Frame g;
        uint32_t fo = inlineFn(g, VT_deck_9, {self});
        call(FunctionWidgetInt_swap, {fo, s->deckSlide + 0x500});
        functionDestroy(fo);
    }
    {
        Frame g;
        uint32_t fo = inlineFn(g, VT_deck_10, {self});
        call(FunctionWidgetW_swap, {fo, s->deckSlide + 0x518});
        functionDestroy(fo);
    }
    call(BaseSlideView_setIndex, {s->deckSlide, selected});
    {
        slide = s->deckSlide;
        Frame g;
        uint32_t copy = g.alloc(12);
        call(IntVector_copy, {copy, ids});
        call(BaseSlideView_setItemIDs, {slide, copy, 0});
        call(wl::IntVector_baseDtor, {copy});
    }
    call(BaseSlideView_setSlideEnabled, {s->deckSlide, 1});
    vcall(deck, VS_addChild, {deck, s->deckSlide});
    uint32_t count = uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2);
    s->leftUI = find(s->root, S_LEFT_UI, 6);
    s->rightUI = find(s->root, S_RIGHT_UI, 7);
    if (count >= 2) {
        const uint32_t sides[2] = {self + 0x2B4, self + 0x2B8};
        const uint32_t vts[2] = {VT_deck_11, VT_deck_12};
        for (int side = 0; side < 2; side++) {
            uint32_t ui = ld32(sides[side]);
            uint32_t target = ld32(ld32(vcall(ui, VS_getChildren, {ui})));
            uint32_t btn;
            {
                Frame g;
                uint32_t name = newStr(g, S_EMPTY, 0);
                btn = call(ImageButton_create, {name, 0});
                strFree(name);
            }
            call(ImageButton_setTargetView, {btn, target, fb(1.0f)});
            {
                Frame g;
                uint32_t anchor = g.alloc(8);
                call(wl::Vec2_ctorXY, {anchor, fb(-0.5f), fb(-0.5f)});
                call(ImageButton_setTargetAnchorPoint, {btn, anchor});
                call(wl::Vec2_dtor, {anchor});
            }
            vcall(target, VS_addChild, {target, btn});
            Frame g;
            uint32_t fo = inlineFn(g, vts[side], {self});
            call(ImageButton_addTouchEventListener, {btn, fo});
            functionDestroy(fo);
        }
    }
    uint32_t show = count > 1 ? 1 : 0;
    vcall(s->leftUI, VS_setVisible, {s->leftUI, show});
    vcall(s->rightUI, VS_setVisible, {s->rightUI, show});
    call(wl::IntVector_baseDtor, {ids});
}
RW_FUNCTION(0x57452C, "StartDeckEditDialog::initDeckEdit", StartDeckEditDialog::initDeckEdit);

// OK button text / title by stage type; the skip-ticket button (story / event quests with tickets left) with the
// ticket count and the three sub-mission marks: a mission counts when it is in the cleared mission list.
void StartDeckEditDialog::updateDeckLayout(Addr self) {
    Self *s = S(self);
    uint32_t base = find(s->root, S_BASE, 4);
    uint32_t button = find(s->root, S_BUTTON, 6);
    uint32_t ok = find(button, S_BUTTON_OK, 9);
    uint32_t okText = find(ok, S_TXT_OK, 6);
    call(SceneUtil_setShadow, {okText});
    int32_t title, text;
    if (s->stageType == 2) {
        title = 103000006;
        text = 103000010;
    } else if (s->notColosseum) {
        title = 103000006;
        text = 103000009;
    } else {
        title = 103000007;
        text = 103000010;
    }
    titleLabel(base, S_TXT, 3, title);
    setTextId(okText, text);
    uint32_t a = api();
    uint32_t skip = find(s->root, S_SKIP, 4);
    uint32_t skipButton = find(skip, S_BUTTON_SKIP, 11);
    uint32_t skipText = find(skip, S_TXT_SKIP, 8);
    bool show = false;
    if (s->stageType != 2 && !s->notColosseum) {
        uint32_t status = ld32(a + API_USER_STATUS);
        if (scene::safe32(status, 0x54) >= 1 && uint32_t(s->stageType.get()) <= 1) show = true;
    }
    if (!show) {
        vcall(skip, VS_setVisible, {skip, 0});
        call(SceneUtil_setButtonEnabled, {skipButton, skipText, 0, 0});
        return;
    }
    vcall(skip, VS_setVisible, {skip, 1});
    titleLabel(skip, S_TXT_SKIP, 8, 102900037);
    uint32_t num = find(skip, S_TXT_NUM, 7);
    call(SceneUtil_setShadow, {num});
    {
        Frame f;
        uint32_t td = textData(f, 102900038);
        uint32_t fmt = textDataStr(td);
        uint32_t status = ld32(a + API_USER_STATUS);
        int32_t tickets = scene::safe32(status, 0x54);
        if (tickets <= 9999) tickets = scene::safe32(ld32(a + API_USER_STATUS), 0x54);
        else tickets = 9999;
        uint32_t buf = stringBuffer(f, {fmt, uint32_t(tickets)});
        uint32_t str = newStr(f, buf);
        call(Text_setString, {num, str});
        strFree(str);
        sharedRelease(td);
    }
    uint32_t sub1 = 0, sub2 = 0, sub3 = 0;
    if (s->stageType != 0) {
        // event quest: the cleared missions of this stage in the event list
        uint32_t events = ld32(api() + API_EVENTS);
        Frame f;
        uint32_t stage = f.alloc(0x18);     // {3 ints, vector<int> missions}
        st32(stage + 0xC, 0);
        st32(stage + 0x10, 0);
        st32(stage + 0x14, 0);
        uint32_t eb = ld32(events), ee = ld32(events + 4);
        uint32_t mb = 0, me = 0;
        if (int32_t(ee - eb) >= 1) {
            uint32_t n = uint32_t(int32_t(ee - eb) >> 3) * 0xB6DB6DB7u;   // / 56
            int32_t i = 0;
            do {
                if (!(n > uint32_t(i))) {
                    call(wl::VECTOR_AT_THROW, {events});
                    return;
                }
                uint32_t e = eb + uint32_t(i) * 0x38;
                uint32_t sb = ld32(e), se = ld32(e + 4);
                if (int32_t(se - sb) >= 1) {
                    uint32_t m = uint32_t(int32_t(se - sb) >> 3) * 0xAAAAAAABu;   // / 24
                    uint32_t off = 0;
                    int32_t j = 0;
                    do {
                        if (!(m > uint32_t(j))) {
                            call(wl::VECTOR_AT_THROW, {e});
                            return;
                        }
                        uint32_t id = ld32(sb + off);
                        if (id == ld32(sceneManager() + SM_STAGE_ID)) {
                            uint32_t b = ld32(e);
                            uint32_t m2 = uint32_t(int32_t(ld32(e + 4) - b) >> 3) * 0xAAAAAAABu;
                            if (!(m2 > uint32_t(j))) {
                                call(wl::VECTOR_AT_THROW, {e});
                                return;
                            }
                            uint32_t src = b + off;
                            st32(stage, ld32(src));
                            st32(stage + 4, ld32(src + 4));
                            st32(stage + 8, ld32(src + 8));
                            if (stage - b != off)
                                call(wl::IntVector_assign, {stage + 0xC, ld32(src + 0xC), ld32(src + 0x10)});
                        }
                        sb = ld32(e);
                        m = uint32_t(int32_t(ld32(e + 4) - sb) >> 3) * 0xAAAAAAABu;
                        off += 0x18;
                        j++;
                    } while (j < int32_t(m));
                    eb = ld32(events);
                    ee = ld32(events + 4);
                }
                n = uint32_t(int32_t(ee - eb) >> 3) * 0xB6DB6DB7u;
                i++;
            } while (i < int32_t(n));
            mb = ld32(stage + 0xC);
            me = ld32(stage + 0x10);
        }
        int32_t count = int32_t(me - mb) >> 2;
        uint32_t it = upperBound(mb, count, 0);
        if (it != me) sub1 = int32_t(ld32(it)) < 2 ? 1 : 0;
        it = upperBound(mb, count, 1);
        if (it != me) sub2 = int32_t(ld32(it)) < 3 ? 1 : 0;
        it = upperBound(mb, count, 2);
        if (it != me) sub3 = int32_t(ld32(it)) < 4 ? 1 : 0;
        call(wl::IntVector_baseDtor, {stage + 0xC});
    } else {
        uint32_t story = call(StageUtil_findStory, {ld32(sceneManager() + SM_STAGE_ID), 1});
        uint32_t mb = ld32(story + 0xC), me = ld32(story + 0x10);
        int32_t count = int32_t(me - mb) >> 2;
        uint32_t it = upperBound(mb, count, 0);
        if (it != me) sub1 = int32_t(ld32(it)) >= 2 ? 0 : 1;
        it = upperBound(mb, count, 1);
        if (it != me) sub2 = int32_t(ld32(it)) < 3 ? 1 : 0;
        it = upperBound(mb, count, 2);
        if (it != me) sub3 = int32_t(ld32(it)) < 4 ? 1 : 0;
    }
    const uint32_t bases[3] = {S_SUBCLEAR1_BASE, S_SUBCLEAR2_BASE, S_SUBCLEAR3_BASE};
    const uint32_t flags[3] = {sub1, sub2, sub3};
    for (int k = 0; k < 3; k++) {
        uint32_t b = find(skip, bases[k], 14);
        uint32_t mark = find(b, S_SUBCLEAR, 8);
        vcall(mark, VS_setVisible, {mark, flags[k]});
    }
    call(SceneUtil_setButtonEnabled, {skipButton, skipText, sub2 & sub3 & sub1, 0});
}
RW_FUNCTION(0x574C24, "StartDeckEditDialog::updateDeckLayout", StartDeckEditDialog::updateDeckLayout);
