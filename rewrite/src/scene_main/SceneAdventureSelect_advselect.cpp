// SceneAdventureSelect::initAdvSelect: the adventure select panel (Adv_Select) - four quest buttons (story,
// events, colosseum, raid) unlocked by the user's rank against master::Misc thresholds 123 / 122 / 106.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {

// master::Misc row `id`: its value (index 4)
struct MiscValue {
    int32_t value;
    uint32_t ctrl;
};
MiscValue miscValue(Frame &f, uint32_t id) {
    uint32_t row = masterRow(f, fn::Misc_create, id);
    return MiscValue{safe32(ld32(row), 4), ld32(row + 4)};
}

// "Rank %d" style lock text: TextData 102850009 formatted with master::Misc `miscId`'s value into `text`.
void setLockText(uint32_t text, uint32_t miscId) {
    SceneUtil::setShadow(text);
    Frame f;
    uint32_t fmt = textData(f, 102850009);
    uint32_t fmtText = textDataStr(fmt);
    MiscValue misc = miscValue(f, miscId);
    uint32_t buf = stringBuffer(f, {fmtText, uint32_t(misc.value)});
    setTextC(text, buf);
    if (misc.ctrl) call(fn::shared_release, {misc.ctrl});
    sharedRelease(fmt);
}

// widget->addTouchEventListener(heap lambda {vtable, captures})
void addHeapTouch(uint32_t widget, uint32_t vtable, uint32_t size, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fo = f.alloc(20);
    heapLambda(fo, size, vtable, captures);
    call(fn::Widget_addTouchEventListener, {widget, fo});
    functionDestroy(fo);
}

}  // namespace

void SceneAdventureSelect::initAdvSelect(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->buttonFilter) setVisible(s->buttonFilter, 0);
    {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t sp = textData(f, 100620048);
        uint32_t msg = newStr(f, textDataStr(sp));
        call(lib::SceneManager_changeTickerMessage, {mgr, msg});
        strFree(msg);
        sharedRelease(sp);
    }
    call(fn::APIManager_getInstance, {});
    if (s->stageScrollView) call(lib::BaseStageScrollView_setEnabledScroll, {s->stageScrollView, 0});
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t benefits = ld32(call(fn::APIManager_getInstance, {}) + API_BENEFITS);

    // the unlock thresholds against the user's rank
    Frame f;
    int32_t rank = safe32(ld32(api + API_USER), USER_RANK);
    MiscValue raidRank = miscValue(f, 106);
    if (raidRank.ctrl) call(fn::shared_release, {raidRank.ctrl});
    int32_t rank2 = safe32(ld32(api + API_USER), USER_RANK);
    MiscValue eventRank = miscValue(f, 123);
    if (eventRank.ctrl) call(fn::shared_release, {eventRank.ctrl});
    int32_t rank3 = safe32(ld32(api + API_USER), USER_RANK);
    MiscValue colosseumRank = miscValue(f, 122);
    uint32_t colosseumOpen;
    if (rank3 >= colosseumRank.value) {
        if (colosseumRank.ctrl) call(fn::shared_release, {colosseumRank.ctrl});
        colosseumOpen = 1;
    } else if (ld32(benefits + 4) != ld32(benefits) && SceneUtil::isBenefits() == 1) {
        int32_t phase = SceneUtil::getNexTutorialphase();
        if (colosseumRank.ctrl) call(fn::shared_release, {colosseumRank.ctrl});
        colosseumOpen = phase > 994 ? 1 : 0;
    } else {
        colosseumOpen = 0;
        if (colosseumRank.ctrl) call(fn::shared_release, {colosseumRank.ctrl});
    }
    uint32_t eventOpen = rank2 >= eventRank.value ? 1 : 0;
    uint32_t raidOpen = rank >= raidRank.value ? 1 : 0;

    uint32_t panel = s->advSelect.node;
    uint32_t quest1 = findNode(panel, STR_QuestBut1, 9);
    uint32_t quest2 = findNode(s->advSelect.node, STR_QuestBut2, 9);
    uint32_t quest3 = findNode(s->advSelect.node, STR_QuestBut3, 9);
    uint32_t quest4 = findNode(s->advSelect.node, STR_QuestBut4, 9);

    // 1: story
    vcall(quest1, vslot::widgetSetTouchEnabled, {quest1, 1});
    addHeapTouch(quest1, VT_initAdvSelect_13, 0x18, {self, quest1, quest2, quest3, quest4});

    // 2: events
    vcall(quest2, vslot::widgetSetTouchEnabled, {quest2, 1});
    addHeapTouch(quest2, VT_initAdvSelect_14, 0x1C, {self, eventOpen, quest1, quest2, quest3, quest4});
    call(lib::Widget_setBright, {quest2, eventOpen});
    uint32_t lock2 = findNode(s->advSelect.node, STR_Title2_Lock, 0xB);
    uint32_t title2 = findNode(s->advSelect.node, STR_Title2, 6);
    uint32_t panel2 = findNode(s->advSelect.node, STR_Panel2, 6);
    uint32_t icon2 = findNode(s->advSelect.node, STR_Title2_Lock_Icon, 0x10);
    setVisible(panel2, 0);
    uint32_t text2 = findNode(s->advSelect.node, STR_Txt_Title2, 0xA);
    if (rank2 >= eventRank.value) {
        setVisible(title2, 1);
        setVisible(lock2, 0);
        setVisible(icon2, 0);
        setVisible(text2, 0);
    } else {
        setVisible(title2, 0);
        setVisible(lock2, 1);
        setVisible(icon2, 1);
        setVisible(text2, 1);
        setLockText(text2, 123);
    }

    // 3: colosseum
    vcall(quest3, vslot::widgetSetTouchEnabled, {quest3, 1});
    addHeapTouch(quest3, VT_initAdvSelect_15, 0x1C, {self, colosseumOpen, quest1, quest2, quest3, quest4});
    call(lib::Widget_setBright, {quest3, colosseumOpen});
    uint32_t lock3 = findNode(s->advSelect.node, STR_Title3_Lock, 0xB);
    uint32_t title3 = findNode(s->advSelect.node, STR_Title3, 6);
    uint32_t panel3 = findNode(s->advSelect.node, STR_Panel3, 6);
    uint32_t text3 = findNode(s->advSelect.node, STR_Txt_Title3, 0xA);
    uint32_t icon3 = findNode(s->advSelect.node, STR_Title3_Lock_Icon, 0x10);
    if (colosseumOpen == 1) {
        {
            Frame g;
            uint32_t name = newStr(g, STR_Txt_Panel3, 0xA);
            SceneUtil::settingTitleLabel(s->advSelect.node, name, 102850010);
            strFree(name);
        }
        setVisible(lock3, 0);
        setVisible(icon3, 0);
        setVisible(title3, 1);
        setVisible(text3, 0);
        setVisible(panel3, ld32(benefits + 4) != ld32(benefits) ? 1 : 0);
    } else {
        setVisible(title3, 0);
        setVisible(lock3, 1);
        setVisible(icon3, 1);
        setVisible(panel3, 0);
        setVisible(text3, 1);
        setLockText(text3, 122);
    }

    // 4: raid
    vcall(quest4, vslot::widgetSetTouchEnabled, {quest4, 1});
    addHeapTouch(quest4, VT_initAdvSelect_16, 0x1C, {self, raidOpen, quest1, quest2, quest3, quest4});
    uint32_t lock4 = findNode(s->advSelect.node, STR_Title4_Lock, 0xB);
    uint32_t title4 = findNode(s->advSelect.node, STR_Title4, 6);
    uint32_t text4 = findNode(s->advSelect.node, STR_Txt_Title4, 0xA);
    uint32_t icon4 = findNode(s->advSelect.node, STR_Title4_Lock_Icon, 0x10);
    call(lib::Widget_setBright, {quest4, raidOpen});
    if (rank >= raidRank.value) {
        {
            Frame g;
            uint32_t name = newStr(g, STR_Txt_Title4, 0xA);
            SceneUtil::settingTitleLabel(s->advSelect.node, name, 102850008);
            strFree(name);
        }
        setVisible(title4, 1);
        setVisible(lock4, 0);
        setVisible(icon4, 0);
        setVisible(text4, 0);
    } else {
        setVisible(title4, 0);
        setVisible(lock4, 1);
        setVisible(icon4, 1);
        setVisible(text4, 1);
        setLockText(text4, 106);
    }

    changeStageInfo(self);
    uint32_t cb = newFunction(f, VT_initAdvSelect_17, {self});
    uint32_t result = f.alloc(20);
    SceneUtil::openTutorialPopup(result, 24, cb, 0, 0, 0, 0);
    functionDestroy(result);
    functionDestroy(cb);
}
RW_FUNCTION(0x721F50, "SceneAdventureSelect::initAdvSelect", SceneAdventureSelect::initAdvSelect);
