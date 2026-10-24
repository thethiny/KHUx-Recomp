// ColosseumScrollView: a list of the colosseum's stages (Colosseum_MainPanel rows: the stage's rank, its round
// "cleared/rounds", its AP and a check button). No code in 1.0.1 creates it (the colosseum screens use
// AdvStageScrollView); kept as written. A BaseScrollView<int> over indexes into the colosseum info's stages
// (APIManager +0x24C, Colosseum::Info: id, then vector<Colosseum::Stage> at +8).
#include "../w3_9/BaseScrollView.hpp"
#include "RowTouch.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t strlen = 0x40E08C;
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1, IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t BaseScrollViewInt_init = 0x4FA8A1;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t StageVector_copy = 0x54E489, StageVector_dtor = 0x4B3A95;   // vector<Colosseum::Stage>
constexpr uint32_t Colosseum_create = 0x77C4E9, ColosseumStage_create = 0x77CF95;   // master::
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addSceneGraph = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t FunctionBTE_swap = 0x483E4D, FunctionVTE_swap = 0x4F3699;
// this class
constexpr uint32_t getLayoutPanel = 0x54DA95, updateSelectItems = 0x54E4F9, assignEnded = 0x54E7C1;
constexpr uint32_t setSelectItem = 0x54E861, touchEndedBody = 0x54EB19;
}  // namespace g

constexpr uint32_t SIZE = 0x670;
constexpr uint32_t VTABLE = 0xF875C8, VTABLE2 = 0xF87920, VTABLE3 = 0xF87934;
// fields
constexpr uint32_t F_FUNCTION = 0x638;          // std::function (callable pointer at +0x648)
constexpr uint32_t F_PANEL = 0x650;             // ui::Widget* row template (retained)
constexpr uint32_t F_TOUCH = 0x654;             // rows take touches
constexpr uint32_t F_SELECTED = 0x664;          // std::vector<int>
constexpr uint32_t API_COLOSSEUM = 0x24C;       // APIManager: Colosseum::Info*
// lambda vtables
constexpr uint32_t VT_BEGAN = 0xF58A14, VT_MOVED = 0xF589E8, VT_ENDED = 0xF58990, VT_TAP_DONE = 0xF589BC;
constexpr uint32_t VT_CHECK = 0xF58A40;         // the check button {row, this, id}
// strings
constexpr uint32_t S_MainPanel = 0xDFE8D4, S_Panel = 0xDFD420, S_TxtRank = 0xDFE900, S_TxtRankLabel = 0xDFE909;
constexpr uint32_t S_TxtRound = 0xDFE918, S_TxtRoundLabel = 0xDFE922, S_TxtAP = 0xDF93C8, S_TxtAPLabel = 0xDFE936;
constexpr uint32_t S_D = 0xDF86DE, S_DSlashD = 0xDF8F35, S_NoRounds = 0xDFE932;
constexpr uint32_t S_Check = 0xDFDC5C, S_PartyBut15 = 0xDFE943;
constexpr int32_t TEXT_RANK = 103300007, TEXT_ROUND = 103300008, TEXT_AP = 103200007;

const rowtouch::View VIEW = {F_SELECTED, 0x655, 0x658, 0x65C, 0x660, VT_ENDED, VT_TAP_DONE,
                             g::setSelectItem, g::updateSelectItems, g::touchEndedBody};

int32_t sbInt(uint32_t sb, uint32_t offset) {
    Frame f;
    const uint32_t v = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + offset, 4, offset});
    return int32_t(ld32(v));
}
// a Text: its shadow, then a string
void shadowText(uint32_t text, uint32_t cstr) {
    call(g::SceneUtil_setShadow, {text});
    setTextC(text, cstr);
}
void shadowTextId(uint32_t text, int32_t id) {
    call(g::SceneUtil_setShadow, {text});
    setTextId(text, id);
}
}  // namespace

struct ColosseumScrollView {
    static uint32_t create();
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static uint32_t getLayoutPanel(uint32_t self, uint32_t unused);
    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t flag);
    static uint32_t changeRow(uint32_t self, uint32_t widget, uint32_t index, uint32_t flag);
    static void updateSelectItems(uint32_t self) { rowtouch::updateSelectItems(VIEW, self); }
    static void registItemTouchEvent(uint32_t self, uint32_t widget, uint32_t index);
    static void setSelectItem(uint32_t self, uint32_t id) { rowtouch::setSelectItem(VIEW, self, id); }
    static uint32_t searchSelect(uint32_t self, uint32_t id) { return rowtouch::searchSelect(VIEW, self, id); }
    static void assignEnded(uint32_t dst, uint32_t caps) { rowtouch::assignEnded(VIEW, dst, caps); }
    static void touchEndedBody(uint32_t caps, uint32_t touch) { rowtouch::touchEndedBody(VIEW, caps, touch); }
    static void tapDone(uint32_t fo) { rowtouch::tapDone(VIEW, fo); }
    static void touchMoved(uint32_t fo, uint32_t touchRef) { rowtouch::touchMoved(VIEW, fo, touchRef); }
    static uint32_t touchBegan(uint32_t fo, uint32_t touchRef) { return rowtouch::touchBegan(VIEW, fo, touchRef); }
};

uint32_t ColosseumScrollView::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(me);
    if (vcall(me, 0x248, {me}) == 1) {                      // init
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

uint32_t ColosseumScrollView::construct(uint32_t self) {
    auto *base = rw::host_ptr<BaseScrollView>(self);
    call(g::Layout_ctor, {self});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    st32(self, VTABLE);
    st32(self + 0x280, VTABLE2);
    st32(self + 0x350, VTABLE3);
    st32(self + F_FUNCTION + 0x10, 0);
    st8(self + F_TOUCH, 0);
    st8(self + 0x655, 0);
    st32(self + 0x658, 0);
    st8(self + 0x65C, 0);
    for (uint32_t k : {0x66Cu, 0x668u, 0x664u, 0x660u}) st32(self + k, 0);
    return self;
}

// the row template goes through its deleting destructor (not release)
uint32_t ColosseumScrollView::destruct(uint32_t self) {
    st32(self, VTABLE);
    st32(self + 0x280, VTABLE2);
    st32(self + 0x350, VTABLE3);
    if (const uint32_t panel = ld32(self + F_PANEL)) vcall(panel, 0x04, {panel});
    st32(self + F_PANEL, 0);
    call(g::VectorBaseInt_dtor, {self + F_SELECTED});
    functionDestroy(self + F_FUNCTION);
    st32(self, BaseScrollView::VTABLE_INT);
    st32(self + 0x280, BaseScrollView::VTABLE2_INT);
    st32(self + 0x350, BaseScrollView::VTABLE3_INT);
    call(g::VectorBaseInt_dtor, {self + 0x628});
    call(g::IScrollProtocol_dtor, {self});
    return self;
}

// 650 x 100 rows from Colosseum_MainPanel
uint32_t ColosseumScrollView::init(uint32_t self) {
    stf(self + 0x514, 650.0f);
    stf(self + 0x518, 100.0f);
    call(g::IScrollProtocol_setEnabledScrollBar, {self, 1});
    stf(self + 0x570, ldf(self + 0x514) * 0.5f);
    const uint32_t panel = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_MainPanel});
    st32(self + F_PANEL, panel);
    call(g::Ref_retain, {panel});
    vcall(ld32(self + F_PANEL), 0x29C, {ld32(self + F_PANEL), 0});   // setTouchEnabled(false)
    return call(g::BaseScrollViewInt_init, {self});
}

uint32_t ColosseumScrollView::getLayoutPanel(uint32_t self, uint32_t) {
    const uint32_t w = call(g::Widget_create, {});
    const uint32_t clone = call(g::Widget_clone, {ld32(self + F_PANEL)});
    vcall(w, node::setAnchorPoint, {w, VEC2_ANCHOR_MIDDLE});
    vcall(w, node::addChild, {w, clone});
    Frame f;
    const uint32_t size = f.alloc(8);
    call(g::Size_ctor, {size, ld32(self + 0x514), ld32(self + 0x518)});
    vcall(w, node::setContentSize, {w, size});
    vcall(w, node::setPositionX, {w, fbits(ldf(self + 0x514) * 0.5f)});
    return w;
}

uint32_t ColosseumScrollView::createRow(uint32_t self, uint32_t index, uint32_t flag) {
    if ((ld32(self + 0x62C) - ld32(self + 0x628)) >> 2 <= index) call(g::ThrowOutOfRange, {});
    const uint32_t row = call(g::getLayoutPanel, {self, 0x628});
    vcall(self, 0x340, {self, row, index, flag});             // changeRow
    return row;
}

// one stage: its rank (the stage entry's first word), "cleared/rounds" (the rounds of the master::Colosseum row
// with that rank, searched between the ColosseumStage row's first and last colosseum; 10 when none matches, a dash
// for 0) and its AP; the check button selects it
uint32_t ColosseumScrollView::changeRow(uint32_t self, uint32_t widget, uint32_t index, uint32_t) {
    const uint32_t ids = ld32(self + 0x628);
    if ((ld32(self + 0x62C) - ids) >> 2 <= index) call(g::ThrowOutOfRange, {});
    if (!findNode(widget, S_Panel, 5)) return 1;
    const uint32_t info = ld32(call(fn::APIManager_getInstance, {}) + API_COLOSSEUM);
    const uint32_t colosseumId = ld32(info);
    Frame f;
    const uint32_t stages = f.alloc(12);
    call(g::StageVector_copy, {stages, ld32(call(fn::APIManager_getInstance, {}) + API_COLOSSEUM) + 8});
    const uint32_t idAt = ids + 4 * index;
    if ((ld32(stages + 4) - ld32(stages)) >> 4 <= ld32(idAt)) call(g::ThrowOutOfRange, {stages});
    const uint32_t stage = ld32(stages) + 16 * ld32(idAt);
    const int32_t ap = int32_t(ld32(stage + 0xC));
    const int32_t cleared = int32_t(ld32(stage + 4));
    const int32_t rank = int32_t(ld32(stage));
    const uint32_t row = masterRow(f, g::ColosseumStage_create, colosseumId);
    int32_t rounds = 10;
    for (int32_t id = sbInt(ld32(row), 0x1C); id <= sbInt(ld32(row), 0x20); id++) {
        Frame g2;
        const uint32_t c = masterRow(g2, g::Colosseum_create, uint32_t(id));
        bool found = false;
        if (rank == sbInt(ld32(c), 8)) {
            rounds = sbInt(ld32(c), 0x10);
            found = true;
        }
        sharedRelease(c);
        if (found) break;
    }
    shadowTextId(findNode(widget, S_TxtRank, 8), TEXT_RANK);
    {
        Frame g2;
        const uint32_t text = findNode(widget, S_TxtRankLabel, 0xE);
        call(g::SceneUtil_setShadow, {text});
        setTextC(text, stringBuffer(g2, {S_D, uint32_t(rank)}));
    }
    shadowTextId(findNode(widget, S_TxtRound, 9), TEXT_ROUND);
    {
        Frame g2;
        const uint32_t text = findNode(widget, S_TxtRoundLabel, 0xF);
        call(g::SceneUtil_setShadow, {text});
        if (rounds) {
            setTextC(text, stringBuffer(g2, {S_DSlashD, uint32_t(cleared), uint32_t(rounds)}));
        } else {
            const uint32_t s = newStr(g2, S_NoRounds, 3);
            call(fn::Text_setString, {text, s});
            strFree(s);
        }
    }
    shadowTextId(findNode(widget, S_TxtAP, 6), TEXT_AP);
    {
        Frame g2;
        const uint32_t text = findNode(widget, S_TxtAPLabel, 0xC);
        call(g::SceneUtil_setShadow, {text});
        setTextC(text, stringBuffer(g2, {S_D, uint32_t(ap)}));
    }
    const uint32_t panel = findNode(widget, S_Panel, 5);
    uint32_t button = findNode(widget, S_Check, 0x11);
    if (!button) {
        Frame g2;
        const uint32_t s = newStr(g2, S_PartyBut15, 0xB);
        button = call(g::ImageButton_create, {s, 0});
        strFree(s);
        call(g::ImageButton_setTargetView, {button, panel, fbits(1.0f)});
        const uint32_t name = newStr(g2, S_Check, 0x11);
        vcall(panel, node::addChildZName, {panel, button, 0, name});
        strFree(name);
    }
    {
        Frame g2;
        const uint32_t fo = newFunction(g2, VT_CHECK, {widget, self, ld32(idAt)});
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    call(g::updateSelectItems, {self});
    sharedRelease(row);
    call(g::StageVector_dtor, {stages});
    return 0;
}

// rows take touches through their own listener (when enabled): began {this, index}, moved (heap, the ended
// captures after it), ended / cancelled
void ColosseumScrollView::registItemTouchEvent(uint32_t self, uint32_t, uint32_t index) {
    if (!ld8(self + F_TOUCH)) return;
    const uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    {
        const uint32_t began = newFunction(f, VT_BEGAN, {self, index});
        call(g::FunctionBTE_swap, {began, listener + 0x50});
        functionDestroy(began);
    }
    const uint32_t caps = f.alloc(16);
    st32(caps, self);
    st32(caps + 4, index);
    st32(caps + 8, self);
    st32(caps + 12, index);
    {
        const uint32_t moved = newEmptyFunction(f);
        const uint32_t obj = call(fn::operator_new, {0x1C});
        if (obj) {
            st32(obj, VT_MOVED);
            for (uint32_t w = 4; w < 0x1C; w += 8) {
                st32(obj + w, self);
                st32(obj + w + 4, index);
            }
        }
        st32(moved + 16, obj);
        call(g::FunctionVTE_swap, {moved, listener + 0x68});
        functionDestroy(moved);
    }
    call(g::assignEnded, {listener + 0x80, caps});
    call(g::assignEnded, {listener + 0x98, caps});
    call(g::EventDispatcher_addSceneGraph, {dispatcher, listener, self});
}

RW_FUNCTION(0x54D740, "ColosseumScrollView::~ColosseumScrollView", ColosseumScrollView::destruct);
RW_FUNCTION(0x54D858, "ColosseumScrollView::create", ColosseumScrollView::create);
RW_FUNCTION(0x54D8AC, "ColosseumScrollView::ColosseumScrollView", ColosseumScrollView::construct);
RW_FUNCTION(0x54DA1C, "ColosseumScrollView::init", ColosseumScrollView::init);
RW_FUNCTION(0x54DA94, "ColosseumScrollView::getLayoutPanel", ColosseumScrollView::getLayoutPanel);
RW_FUNCTION(0x54DB5C, "ColosseumScrollView::createRow", ColosseumScrollView::createRow);
RW_FUNCTION(0x54DBB4, "ColosseumScrollView::changeRow", ColosseumScrollView::changeRow);
RW_FUNCTION(0x54E4F8, "ColosseumScrollView::updateSelectItems", ColosseumScrollView::updateSelectItems);
RW_FUNCTION(0x54E678, "ColosseumScrollView::registItemTouchEvent", ColosseumScrollView::registItemTouchEvent);
RW_FUNCTION(0x54E7C0, "ColosseumScrollView ended lambda function::operator= (sub_54E7C0)",
            ColosseumScrollView::assignEnded);
RW_FUNCTION(0x54E860, "ColosseumScrollView::setSelectItem", ColosseumScrollView::setSelectItem);
RW_FUNCTION(0x54EA18, "ColosseumScrollView::searchSelect", ColosseumScrollView::searchSelect);
RW_FUNCTION(0x54EB18, "ColosseumScrollView ended lambda body (sub_54EB18)", ColosseumScrollView::touchEndedBody);
RW_FUNCTION(0x54ED40, "ColosseumScrollView tap-done lambda __func::operator() (sub_54ED40)",
            ColosseumScrollView::tapDone);
RW_FUNCTION(0x54EE50, "ColosseumScrollView moved lambda __func::operator() (sub_54EE50)",
            ColosseumScrollView::touchMoved);
RW_FUNCTION(0x54EFF4, "ColosseumScrollView began lambda __func::operator() (sub_54EFF4)",
            ColosseumScrollView::touchBegan);
