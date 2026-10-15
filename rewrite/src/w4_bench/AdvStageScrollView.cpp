// AdvStageScrollView: life cycle, rows, touch handling and the selection list.
#include "AdvStageScrollView.hpp"

#include "w3_0/ImageDownloader.hpp"

using namespace scene;

namespace {
namespace lib {
constexpr uint32_t BaseStageScrollView_ctor = 0x55DAE5;
constexpr uint32_t BaseStageScrollView_dtor = 0x59BADD;
constexpr uint32_t BaseStageScrollView_init = 0x59BD95;
constexpr uint32_t BaseStageScrollView_setScrollType = 0x59D529;
constexpr uint32_t BaseStageScrollView_getItemIDs = 0x59D341;    // (vector<int> out, view)
constexpr uint32_t ImageDownloader_ctor = 0x5BCF7D;
constexpr uint32_t ImageDownloader_dtor = 0x5BCFA9;
constexpr uint32_t AvatarIconCache_dtor = 0x5CA8FD;
constexpr uint32_t WidgetVector_dtor = 0x485B79;                 // std::__vector_base<ui::Widget*>::~__vector_base
constexpr uint32_t IntVector_dtor = 0x478961;                    // std::__vector_base<int>::~__vector_base
constexpr uint32_t IntVector_pushBackSlow = 0x4D6CC1;            // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t IntVector_assign = 0x47D5BD;                  // vector<int>::assign(int* first, int* last)
constexpr uint32_t sortInts = 0x4EC65D;                          // std::__sort<__less<int>&, int*>(first, last, comp)
constexpr uint32_t MEMSET = 0x40E7A0;                            // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t Stage_create = 0x78E4E9;                      // master::Stage::create(shared_ptr out, id)
constexpr uint32_t setOverStageFrame = 0x56A2CD;                 // AdvStageScrollView::setOverStageFrame (static)
constexpr uint32_t setAdvemtureStageFrame = 0x56FAB5;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;            // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;            // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t string_plus = 0x470EAD;                       // std::operator+(string out, const string&, const string&)
}  // namespace lib

// vtables (the value stored at +0)
constexpr uint32_t VT_AdvStageScrollView = 0xF8A760;
constexpr uint32_t VT_AdvStageScrollView_640 = 0xF8AAB8;
constexpr uint32_t VT_AdvStageScrollView_848 = 0xF8AACC;
constexpr uint32_t VT_registItemTouchEvent_18 = 0xF59388;    // onTouchBegan lambda (this, row index)
constexpr uint32_t VT_registItemTouchEvent_19 = 0xF5935C;    // onTouchMoved lambda (this, row widget)
constexpr uint32_t VT_registItemTouchEvent_20 = 0xF59330;    // onTouchEnded / onTouchCancelled lambda (this)

// virtual slots (byte offsets)
constexpr uint32_t VS_DELETE = 0x04;
constexpr uint32_t VS_NODE_INIT = 0x248;
constexpr uint32_t VS_31C = 0x31C;          // a BaseStageScrollView virtual taking a bool (called with false)
constexpr uint32_t VS_CHANGE_ROW = 0x340;   // AdvStageScrollView::changeRow(widget, index, b)

// strings
constexpr uint32_t JSON_COLOSSEUM_PANEL = 0xDFF758;   // "cocostudio/publish/AdventureTop2_Colosseum_Panel.json"
constexpr uint32_t JSON_EV_PANEL = 0xDFF729;          // "cocostudio/publish/AdventureTop2_EV_Panel.json"
constexpr uint32_t JSON_ADV_PANEL = 0xDFF6F2;         // "cocostudio/publish/AdventureTop2_Adv_Panel_ver120.json"
constexpr uint32_t S_CURSOR = 0xDFF78E;               // "Cursor"
constexpr uint32_t S_FACEBOOK_PHOTO_PANEL = 0xDF8E5A; // "FaceBook_PhotoPanel"
constexpr uint32_t S_TXT_ROUND = 0xDFE918;            // "Txt_Round"
constexpr uint32_t S_TXT_EV_GRADE_LABEL = 0xDFF3E2;   // "Txt_EV_Grade_Label"
constexpr uint32_t S_STORY = 0xDFF3CF;                // "Story"

constexpr uint32_t API_CURRENT_STAGE = 292;           // APIManager + 292: SafeBuffer* (+12: the current stage id)

// libc++'s inline vector<T>::clear(): end = begin, written the way the compiler does it
uint32_t clearedEnd(uint32_t begin, uint32_t end) { return end + (~(end - 4 - begin) & ~3u); }

void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t AdvStageScrollView::create() {
    uint32_t obj = call(guest::OPERATOR_NEW, {SIZE});
    call(lib::BaseStageScrollView_ctor, {obj});
    AdvStageScrollView *self = rw::host_ptr<AdvStageScrollView>(obj);
    self->vtable = VT_AdvStageScrollView;
    self->vtable2 = VT_AdvStageScrollView_640;
    self->vtable3 = VT_AdvStageScrollView_848;
    self->onSelect_f = 0;
    self->callback2.f = 0;
    self->stageType = 0;
    self->touching = 0;
    self->raidBossLwf = 0;
    self->partyIconsShown = 0;
    call(lib::MEMSET, {obj + 1656, 0x1A, 0});
    self->eventNewStage = 0;                 // strh: +1692 and +1693
    self->raidBossShown = 0;
    self->newStageLwf = 0;
    self->field_694 = 0;
    self->imageDownloader = 0;
    self->rowWidgets.cap = 0;
    self->rowWidgets.end = 0;
    self->rowWidgets.begin = 0;
    if (vcall(obj, VS_NODE_INIT, {obj}) == 1) {
        call(lib::Ref_autorelease, {obj});
        return obj;
    }
    vcall(obj, VS_DELETE, {obj});
    return 0;
}

AdvStageScrollView *AdvStageScrollView::destruct(AdvStageScrollView *self) {
    self->vtable = VT_AdvStageScrollView;
    self->vtable2 = VT_AdvStageScrollView_640;
    self->vtable3 = VT_AdvStageScrollView_848;
    if (self->avatarIconCache) call(guest::OPERATOR_DELETE, {call(lib::AvatarIconCache_dtor, {self->avatarIconCache})});
    self->avatarIconCache = 0;
    if (self->imageDownloader) call(guest::OPERATOR_DELETE, {call(lib::ImageDownloader_dtor, {self->imageDownloader})});
    self->imageDownloader = 0;
    uint32_t me = rw::guest_addr(self);
    call(lib::WidgetVector_dtor, {me + 1704});
    call(lib::IntVector_dtor, {me + 1664});
    functionDestroy(me + 1624);
    functionDestroy(me + ON_SELECT);
    call(lib::BaseStageScrollView_dtor, {me});
    return self;
}

uint32_t AdvStageScrollView::init(AdvStageScrollView *self) {
    uint32_t me = rw::guest_addr(self);
    uint32_t dl = call(guest::OPERATOR_NEW, {0x34});
    call(lib::ImageDownloader_ctor, {dl});
    self->imageDownloader = dl;
    rw::host_ptr<ImageDownloader>(dl)->defaultTimeout = 3600;
    rw::set<float>(self, 1404, 168.0f);
    rw::set<float>(self, 1408, 179.0f);
    rw::set<float>(self, 1528, 40.0f);
    rw::set<float>(self, 1532, 40.0f);
    rw::set<float>(self, 1524, 24.0f);
    vcall(me, VS_31C, {me, 0});
    rw::set<uint8_t>(self, 1488, 0);
    rw::set<uint32_t>(self, 1472, 0);
    rw::set<uint32_t>(self, 1476, 0);
    if (self->itemIDs.end != self->itemIDs.begin) self->itemIDs.end = clearedEnd(self->itemIDs.begin, self->itemIDs.end);
    call(lib::BaseStageScrollView_setScrollType, {me, 3});
    call(lib::Node_scheduleUpdate, {me});
    uint32_t ok = call(lib::BaseStageScrollView_init, {me});
    rw::set<float>(self, 1444, 1.0f);
    rw::set<float>(self, 1448, 10.0f);
    rw::set<float>(self, 1452, 2.0f);
    rw::set<float>(self, 1456, 0.2f);
    vcall(me, VS_31C, {me, 0});
    return ok;
}

// ------------------------------------------------------------------------------------------------ rows
// A row panel for the current stage type. For an unknown type the original never assigns the panel and uses
// whatever its r7 held (the caller's register).
uint32_t AdvStageScrollView::createRow(AdvStageScrollView *self, int32_t a, int32_t b) {
    uint32_t row = rw::cpu->r[7];
    int32_t type = self->stageType;
    uint32_t json = 0;
    if (type == 2) json = JSON_COLOSSEUM_PANEL;
    else if (type == 1) json = JSON_EV_PANEL;
    else if (type == 0) json = JSON_ADV_PANEL;
    if (json) row = call(lib::GUIReader_widgetFromJsonFile, {call(lib::GUIReader_getInstance, {}), json});
    uint32_t cursor = findNode(row, S_CURSOR, 6);
    setVisible(cursor, 0);
    uint32_t photo = findNode(row, S_FACEBOOK_PHOTO_PANEL, 0x13);
    if (photo) setVisible(photo, 0);
    uint32_t me = rw::guest_addr(self);
    vcall(me, VS_CHANGE_ROW, {me, row, uint32_t(a), uint32_t(b)});
    return row;
}

// Fills a row: 1 when the row is not a panel of the current type (its marker node is missing).
uint32_t AdvStageScrollView::changeRow(AdvStageScrollView *self, Addr widget, int32_t index, int32_t) {
    int32_t type = self->stageType;
    uint32_t marker;
    if (type == 2) marker = findNode(widget, S_TXT_ROUND, 9);
    else if (type == 1) marker = findNode(widget, S_TXT_EV_GRADE_LABEL, 0x12);
    else if (type == 0) marker = findNode(widget, S_STORY, 5);
    else marker = 1;   // other types: fill without looking
    if (!marker) return 1;
    call(lib::setAdvemtureStageFrame, {rw::guest_addr(self), widget, uint32_t(index)});
    return 0;
}

// The row past the last item: the "next stage" frame for the story list, hidden otherwise.
void AdvStageScrollView::changeOverRow(AdvStageScrollView *self, Addr widget, int32_t index) {
    int32_t type = self->stageType;
    if (index < 0 || type == 2 || type == 1) {
        setVisible(widget, 0);
        return;
    }
    if (type != 0) return;
    setVisible(widget, 1);
    uint32_t stageId = ld32(ld32(call(lib::APIManager_getInstance, {}) + API_CURRENT_STAGE) + 12);
    uint32_t value = 0;
    if (stageId) {
        Frame f;
        uint32_t row = masterRow(f, lib::Stage_create, stageId);
        uint32_t begin = self->itemIDs.begin, end = self->itemIDs.end;
        int32_t n = int32_t(end - begin) >> 2;
        value = uint32_t(safe32(ld32(row), 4)) + uint32_t(index - n);
        sharedRelease(row);
    }
    call(lib::setOverStageFrame, {widget, value, value});
}

// ------------------------------------------------------------------------------------------------ touch
void AdvStageScrollView::registItemTouchEvent(AdvStageScrollView *self, Addr widget, Addr index) {
    uint32_t me = rw::guest_addr(self);
    uint32_t listener = call(lib::EventListenerTouchOneByOne_create, {});
    call(lib::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    {
        Frame f;
        uint32_t fn = newFunction(f, VT_registItemTouchEvent_18, {me, index});
        call(lib::FunctionTouchBool_swap, {fn, listener + 80});      // onTouchBegan
        functionDestroy(fn);
    }
    {
        Frame f;
        uint32_t fn = newFunction(f, VT_registItemTouchEvent_19, {me, widget});
        call(lib::FunctionTouchVoid_swap, {fn, listener + 104});     // onTouchMoved
        functionDestroy(fn);
    }
    Frame f;
    uint32_t pme = f.alloc(4);
    st32(pme, me);
    setTouchEndedCallback(listener + 128, rw::host_ptr<uint32_t>(pme));   // onTouchEnded
    setTouchEndedCallback(listener + 152, rw::host_ptr<uint32_t>(pme));   // onTouchCancelled
    uint32_t dispatcher = ld32(call(lib::Director_getInstance, {}) + 132);
    call(lib::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, widget});
}

// sub_57085C: *fn = lambda $_20 capturing *self (std::function<void(Touch*, Event*)> built and swapped in)
void AdvStageScrollView::setTouchEndedCallback(Addr fn, const uint32_t *self) {
    Frame f;
    uint32_t tmp = newFunction(f, VT_registItemTouchEvent_20, {*self});
    call(lib::FunctionTouchVoid_swap, {tmp, fn});
    functionDestroy(tmp);
}

// ------------------------------------------------------------------------------------------------ selection
// Toggles `item` in the sorted selection list (the list is cleared first, so it ends up holding just `item`, or
// nothing when it was there).
void AdvStageScrollView::setSelectItem(AdvStageScrollView *self, int32_t item) {
    uint32_t me = rw::guest_addr(self);
    uint32_t vec = me + 1664;
    Frame f;
    uint32_t pItem = f.alloc(4);
    st32(pItem, uint32_t(item));
    uint32_t comp = f.alloc(4);
    uint32_t ids = f.alloc(12);
    if (self->selectedItems.end != self->selectedItems.begin)
        self->selectedItems.end = clearedEnd(self->selectedItems.begin, self->selectedItems.end);
    call(lib::BaseStageScrollView_getItemIDs, {ids, me});   // unused copy of the item ids
    call(lib::sortInts, {self->selectedItems.begin, self->selectedItems.end, comp});
    uint32_t begin = self->selectedItems.begin;
    uint32_t end = self->selectedItems.end;
    // std::lower_bound(begin, end, item)
    uint32_t first = begin;
    int32_t len = int32_t(end - begin) >> 2;
    while (len != 0) {
        int32_t half = len / 2;
        uint32_t mid = first + uint32_t(half) * 4;
        if (int32_t(ld32(mid)) < item) {
            len = len - 1 - half;
            first = mid + 4;
        } else {
            len = half;
        }
    }
    if (first == end || int32_t(ld32(first)) > item) {
        // not selected: push_back and sort
        uint32_t newEnd;
        if (end == self->selectedItems.cap) {
            call(lib::IntVector_pushBackSlow, {vec, pItem});
            newEnd = self->selectedItems.end;
        } else {
            uint32_t e = 0;
            if (end) {
                st32(end, uint32_t(item));
                e = self->selectedItems.end;
            }
            newEnd = e + 4;
            self->selectedItems.end = newEnd;
        }
        call(lib::sortInts, {self->selectedItems.begin, newEnd, comp});
    } else {
        // selected: keep every other item (a local vector), then assign it back
        uint32_t keep = f.alloc(12);
        st32(keep, 0);
        st32(keep + 4, 0);
        st32(keep + 8, 0);
        uint32_t p = begin;
        if (p != end) {
            do {
                uint32_t v = ld32(p);
                if (int32_t(v) != item) {
                    uint32_t kEnd = ld32(keep + 4);
                    if (kEnd == ld32(keep + 8)) {
                        call(lib::IntVector_pushBackSlow, {keep, p});
                    } else {
                        uint32_t e = 0;
                        if (kEnd) {
                            st32(kEnd, v);
                            e = ld32(keep + 4);
                        }
                        st32(keep + 4, e + 4);
                    }
                }
                p += 4;
            } while (p != self->selectedItems.end);
            uint32_t b = self->selectedItems.begin;
            if (b != p) self->selectedItems.end = p - ((p - 4 - b) & ~3u) - 4;
        }
        call(lib::IntVector_assign, {vec, ld32(keep), ld32(keep + 4)});
        call(lib::IntVector_dtor, {keep});
    }
    call(lib::IntVector_dtor, {ids});
}

uint32_t AdvStageScrollView::removeSelectItemAll(AdvStageScrollView *self) {
    uint32_t begin = self->selectedItems.begin, end = self->selectedItems.end;
    if (end == begin) return begin;
    uint32_t e = clearedEnd(begin, end);
    self->selectedItems.end = e;
    return e;
}

// ------------------------------------------------------------------------------------------------ text
// The atlas-label text of a point value: each decimal digit d becomes the letter 'a' + d (most significant first);
// with `hasSecond`, "<second>k<point>" ('k' is the separator). Negative values index outside the digit table, as
// in the original.
void AdvStageScrollView::pointToString(Addr out, int32_t point, int32_t second, uint32_t hasSecond) {
    constexpr uint32_t LETTERS = 0xDF959C;   // "a b c ...k ": eleven one-letter strings
    constexpr uint32_t EMPTY = 0xDF8A1A;     // ""
    Frame f;
    uint32_t digits = f.alloc(11 * 12);
    for (uint32_t k = 0; k < 11; k++) strInit(digits + 12 * k, LETTERS + 2 * k, 1);
    strInit(out, EMPTY, 0);
    uint32_t tmp = f.alloc(12);
    // out = digits[v % 10] + out for every digit of v
    auto prependDigits = [&](int32_t v) {
        int32_t prev;
        do {
            prev = v;
            call(lib::string_plus, {tmp, digits + uint32_t(12 * (v % 10)), out});
            strMoveAssign(out, tmp);
            v = v / 10;
        } while (uint32_t(prev + 9) > 18);
    };
    if (hasSecond) {
        prependDigits(second);
        call(lib::string_plus, {tmp, digits + 12 * 10, out});
        strMoveAssign(out, tmp);
    }
    prependDigits(point);
    for (uint32_t k = 0; k < 11; k++) strFree(digits + 12 * (10 - k));
}

RW_FUNCTION(0x506B60, "AdvStageScrollView::pointToString", AdvStageScrollView::pointToString);
RW_FUNCTION(0x7249FC, "AdvStageScrollView::create", AdvStageScrollView::create);
RW_FUNCTION(0x568B68, "AdvStageScrollView::~AdvStageScrollView", AdvStageScrollView::destruct);
RW_FUNCTION(0x570248, "AdvStageScrollView::init", AdvStageScrollView::init);
RW_FUNCTION(0x570364, "AdvStageScrollView::createRow", AdvStageScrollView::createRow);
RW_FUNCTION(0x5704E4, "AdvStageScrollView::changeRow", AdvStageScrollView::changeRow);
RW_FUNCTION(0x570650, "AdvStageScrollView::changeOverRow", AdvStageScrollView::changeOverRow);
RW_FUNCTION(0x57074C, "AdvStageScrollView::registItemTouchEvent", AdvStageScrollView::registItemTouchEvent);
RW_FUNCTION(0x57085C, "AdvStageScrollView::registItemTouchEvent (set the touch ended callback)",
            AdvStageScrollView::setTouchEndedCallback);
RW_FUNCTION(0x5708D0, "AdvStageScrollView::setSelectItem", AdvStageScrollView::setSelectItem);
RW_FUNCTION(0x570A6C, "AdvStageScrollView::removeSelectItemAll", AdvStageScrollView::removeSelectItemAll);
