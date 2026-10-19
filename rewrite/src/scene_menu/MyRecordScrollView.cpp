// MyRecordScrollView: create / init / destructor, the record list built in setContentSize from the player's
// record counters, the one-row-per-frame update and the scroll bar. Its lambdas' std::function slots are in
// scene_main/MyRecordScrollView_thunks.cpp.
#include "MyRecordScrollView.hpp"

using namespace scene;

namespace {
constexpr uint32_t VT_MyRecordScrollView = 0xF84754;      // _ZTV18MyRecordScrollView + 8
constexpr uint32_t VT2 = 0xF84A98, VT3 = 0xF84AAC;        // the +0x280 and +0x350 bases

// ---- callees
constexpr uint32_t Memset = 0x40E7A0;                     // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t Layout_ctor = 0x828671, Layout_dtor = 0x8289C5;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Widget_setContentSize = 0x825C75;
constexpr uint32_t Size_ctorWH = 0x8DF027, Size_copy = 0x8DF02D;
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t ScrollView_create = 0x856EA1;          // (const Size&, Node* container)
constexpr uint32_t ScrollView_getContentOffset = 0x8574B1;   // (Vec2 out, view)
constexpr uint32_t ScrollView_setContentOffset = 0x857661;   // (view, const Vec2&, bool animated)
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t function_vv_assign = 0x4EB9CD;         // std::function<void()>::operator=(const function&)
constexpr uint32_t MyRecordScrollBar_create = 0x51B111;   // (ScrollBarType, const Size&)
constexpr uint32_t MyRecordScrollBar_updateLayout = 0x51B1D5;
constexpr uint32_t MyRecordScrollBar_setScrollPosition = 0x519F8D;   // (bar, float)
constexpr uint32_t SceneUtil_setCascadeOpacityEnabled = 0x45B16D;
// std::vector helpers of libc++ (out of line in the game)
constexpr uint32_t VectorInt_allocate = 0x47D6DF;         // vector<int>::allocate(n)
constexpr uint32_t VectorInt_copy = 0x4AF707;             // vector<int>(const vector&)
constexpr uint32_t VectorInt_destroy = 0x478961;          // __vector_base<int>::~__vector_base
constexpr uint32_t VectorU64_allocate = 0x48D7E9;
constexpr uint32_t VectorU64_copy = 0x468431;
constexpr uint32_t VectorU64_destroy = 0x468499;
constexpr uint32_t VectorVectorInt_allocate = 0x51B939;
constexpr uint32_t VectorVectorInt_destroy = 0x51B045;
constexpr uint32_t VectorVectorU64_allocate = 0x51B909;
constexpr uint32_t VectorVectorU64_destroy = 0x51B015;
constexpr uint32_t VectorRecord_destroy = 0x51976D;       // __vector_base<RecordData>::~__vector_base
constexpr uint32_t VectorRecord_pushBackSlow = 0x51B78D;  // __push_back_slow_path(const RecordData&)

// ---- data
constexpr uint32_t ANCHOR_BOTTOM_LEFT = 0x1006958;        // cocos2d::Vec2::ANCHOR_BOTTOM_LEFT
constexpr uint32_t VEC2_ZERO = 0x1006950;                 // cocos2d::Vec2::ZERO
constexpr uint32_t APIManager_profile = 0x294;            // shared_ptr: the player's profile with the record counters

// virtual slots
constexpr uint32_t VSLOT_release = 0x04;                  // Ref: the deleting destructor
constexpr uint32_t VSLOT_setClippingEnabled = 0x31C;      // ui::Layout
constexpr uint32_t VSLOT_setTouchEnabled = 0x29C;         // ui::Widget
constexpr uint32_t VSLOT_setDirection = 0x300;            // extension::ScrollView
constexpr uint32_t VSLOT_removeChild = 0x12C;

// init's lambdas (std::function __func vtables + 8), both [view]; stored at +0x210 and +0x228
constexpr uint32_t VT_Init0 = 0xF5696C, VT_Init1 = 0xF56940;
constexpr uint32_t OFF_FUNCTION_0 = 0x210, OFF_FUNCTION_1 = 0x228;

constexpr float BAR_FADE = 0.15f, BAR_HIDE_DELAY = 1.0f;

constexpr uint32_t JSON_Category = 0xDFD868, JSON_Panel = 0xDFD892;
struct Lit { uint32_t addr, len; };
constexpr Lit S_Txt_Category_Label{0xDFD8B9, 18}, S_Txt_Panel_Label{0xDFD8CC, 15}, S_Txt_Num_Label{0xDFD8DC, 13};
constexpr uint32_t FMT_U64 = 0xDF8A23;                    // "%llu"

inline uint32_t find(uint32_t root, Lit name) { return findNode(root, name.addr, name.len); }
inline float height(uint32_t node) { return ldf(vcall(node, node::getContentSize, {node}) + 4); }

// ---- the record list: four categories, each with its rows (text ids and the profile's u64 counters)
constexpr int32_t CATEGORIES[4] = {105300001, 105300002, 105300003, 105300004};
constexpr int32_t ROWS_0[29] = {105310001, 105310003, 105310004, 105310005, 105310006, 105310007, 105310008,
                                105310009, 105310014, 105310015, 105310016, 105310017, 105310018, 105310019,
                                105310020, 105310021, 105310022, 105310023, 105310024, 105310025, 105310026,
                                105310027, 105310030, 105310031, 105310032, 105310033, 105310034, 105310035,
                                105310036};
constexpr int32_t ROWS_1[13] = {105320001, 105320018, 105320002, 105320003, 105320004, 105320005, 105320006,
                                105320007, 105320008, 105320009, 105320010, 105320016, 105320017};
constexpr int32_t ROWS_2[2] = {105330002, 105330003};
constexpr int32_t ROWS_3[3] = {105340005, 105340002, 105340004};
// where each row's counter is in the profile (byte offsets of u64 values)
constexpr uint32_t VALUES_0[29] = {0x88, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xE0, 0xE8, 0xF0, 0xF8,
                                   0x100, 0x108, 0x110, 0x118, 0x120, 0x128, 0x130, 0x138, 0x140, 0x148,
                                   0x158, 0x160, 0x168, 0x170, 0x178, 0x180, 0x188};
constexpr uint32_t VALUES_1[13] = {0x190, 0x198, 0x1A0, 0x1A8, 0x1B0, 0x1B8, 0x1C0, 0x1C8, 0x1D0, 0x1D8, 0x1E0,
                                   0x1F0, 0x1F8};
constexpr uint32_t VALUES_2[2] = {0x200, 0x208};
constexpr uint32_t VALUES_3[3] = {0x210, 0x218, 0x228};

// a std::vector<int> (12 bytes at v) filled like the inlined initializer-list constructor
void makeIntVector(uint32_t v, const int32_t *items, uint32_t n) {
    st32(v, 0);
    st32(v + 4, 0);
    st32(v + 8, 0);
    call(VectorInt_allocate, {v, n});
    for (uint32_t i = 0; i < n; i++) {
        const uint32_t end = ld32(v + 4);
        if (end) st32(end, uint32_t(items[i]));
        st32(v + 4, (end ? end : 0) + 4);
    }
}
// the same for a std::vector<unsigned long long> of the profile's counters
void makeU64Vector(uint32_t v, uint32_t profile, const uint32_t *offsets, uint32_t n) {
    st32(v, 0);
    st32(v + 4, 0);
    st32(v + 8, 0);
    call(VectorU64_allocate, {v, n});
    for (uint32_t i = 0; i < n; i++) {
        const uint32_t end = ld32(v + 4);
        if (end) {
            st32(end, ld32(profile + offsets[i]));
            st32(end + 4, ld32(profile + offsets[i] + 4));
        }
        st32(v + 4, (end ? end : 0) + 8);
    }
}
// a vector of the four vectors at `parts` (copied, then the parts destroyed last to first)
void makeNested(uint32_t v, uint32_t parts, uint32_t allocate, uint32_t copy, uint32_t destroy) {
    st32(v, 0);
    st32(v + 4, 0);
    st32(v + 8, 0);
    call(allocate, {v, 4});
    for (uint32_t i = 0; i < 4; i++) {
        const uint32_t end = ld32(v + 4);
        if (end) call(copy, {end, parts + 12 * i});
        st32(v + 4, (end ? end : 0) + 12);
    }
    for (int32_t i = 3; i >= 0; i--) call(destroy, {parts + 12 * uint32_t(i)});
}

// records.push_back(record) (record: a RecordData on the guest stack); the inlined copy stores the words in a
// different order for the header and the row push
void pushRecord(MyRecordScrollView *self, uint32_t record, bool header) {
    const uint32_t end = self->recordsEnd;
    if (end == self->recordsCap) {
        call(VectorRecord_pushBackSlow, {rw::guest_addr(self) + 0x514, record});
        return;
    }
    if (end) {
        static constexpr uint32_t HEADER_ORDER[6] = {0xC, 0x0, 0x4, 0x8, 0x14, 0x10};
        static constexpr uint32_t ROW_ORDER[6] = {0xC, 0x8, 0x4, 0x0, 0x14, 0x10};
        for (uint32_t k : header ? HEADER_ORDER : ROW_ORDER) st32(end + k, ld32(record + k));
    }
    self->recordsEnd = (end ? end : 0) + 0x18;
}
}  // namespace

MyRecordScrollView::Addr MyRecordScrollView::create() {
    const uint32_t addr = call(fn::operator_new, {SIZE});
    call(Layout_ctor, {addr});
    auto *self = rw::host_ptr<MyRecordScrollView>(addr);
    self->vtable = VT_MyRecordScrollView;
    self->vtable2 = VT2;
    self->vtable3 = VT3;
    st32(addr + 0x508, 0);
    st32(addr + 0x50C, 0);
    self->building = 0;
    st32(addr + 0x52C, 0);
    self->scrollBarShown = 0;
    call(Memset, {addr + 0x514, 0x15, 0});
    if (init(self) == 1) {
        call(Ref_autorelease, {addr});
        return addr;
    }
    vcall(addr, VSLOT_release, {addr});
    return 0;
}

uint32_t MyRecordScrollView::init(MyRecordScrollView *self) {
    const uint32_t addr = rw::guest_addr(self);
    vcall(addr, VSLOT_setClippingEnabled, {addr, 1});
    self->categoryTemplate = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), JSON_Category});
    call(Ref_retain, {self->categoryTemplate});
    vcall(self->categoryTemplate, VSLOT_setTouchEnabled, {self->categoryTemplate, 0});
    self->panelTemplate = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), JSON_Panel});
    call(Ref_retain, {self->panelTemplate});
    vcall(self->panelTemplate, VSLOT_setTouchEnabled, {self->panelTemplate, 0});
    Frame f;
    const uint32_t first = newFunction(f, VT_Init0, {addr});
    call(function_vv_assign, {addr + OFF_FUNCTION_0, first});
    functionDestroy(first);
    const uint32_t second = newFunction(f, VT_Init1, {addr});
    call(function_vv_assign, {addr + OFF_FUNCTION_1, second});
    functionDestroy(second);
    return 1;
}

MyRecordScrollView *MyRecordScrollView::destruct(MyRecordScrollView *self) {
    self->vtable = VT_MyRecordScrollView;
    self->vtable2 = VT2;
    self->vtable3 = VT3;
    if (self->categoryTemplate) vcall(self->categoryTemplate, VSLOT_release, {self->categoryTemplate});
    self->categoryTemplate = 0;
    if (self->panelTemplate) vcall(self->panelTemplate, VSLOT_release, {self->panelTemplate});
    self->panelTemplate = 0;
    const uint32_t addr = rw::guest_addr(self);
    call(VectorRecord_destroy, {addr + 0x514});
    call(Layout_dtor, {addr});
    return self;
}

// adds the next row while building; then keeps the scroll bar in step with the list
void MyRecordScrollView::update(MyRecordScrollView *self, float) {
    if (self->building) {
        const uint32_t count = (self->recordsEnd - self->recordsBegin) / 0x18;
        uint32_t index = self->built;
        if (index < count) {
            const auto *record = rw::host_ptr<RecordData>(self->recordsBegin + 0x18 * index);
            if (record->isCategory) {
                const uint32_t row = call(Widget_clone, {self->categoryTemplate});
                const uint32_t label = find(row, S_Txt_Category_Label);
                setTextId(label, record->textId);
                SceneUtil::setShadow(label);
                vcall(self->scrollView, node::addChild, {self->scrollView, row});
                self->nextY = self->nextY - height(self->categoryTemplate);
                vcall(row, node::setPositionY, {row, fbits(float(self->nextY))});
            } else {
                const uint32_t row = call(Widget_clone, {self->panelTemplate});
                const uint32_t label = find(row, S_Txt_Panel_Label);
                SceneUtil::setShadow(label);
                setTextId(label, record->textId);
                const uint32_t number = find(row, S_Txt_Num_Label);
                SceneUtil::setShadow(number);
                const uint64_t value = record->value;
                Frame f;
                setTextC(number, stringBuffer(f, {FMT_U64, uint32_t(value), uint32_t(value >> 32)}));
                vcall(self->scrollView, node::addChild, {self->scrollView, row});
                self->nextY = self->nextY - height(self->panelTemplate);
                vcall(row, node::setPositionY, {row, fbits(float(self->nextY))});
            }
            index = self->built + 1;
            self->built = index;
        }
        if (index == (self->recordsEnd - self->recordsBegin) / 0x18) {
            self->building = 0;
            self->built = 0;
        }
    }
    if (!self->scrollView || !self->scrollBarEnabled) return;

    float position;
    {
        Frame f;
        const uint32_t offset = f.alloc(8);
        call(ScrollView_getContentOffset, {offset, self->scrollView});
        const float y = ldf(offset + 4);
        call(Vec2_dtor, {offset});
        position = self->contentHeight + y;
    }
    call(MyRecordScrollBar_setScrollPosition, {self->scrollBar, fbits(position)});
    // the bar fades out a second after the list stops, and back in when it moves
    if (self->scrollBarShown) {
        if (self->lastScrollPosition - position != 0.0f) {
            self->lastScrollPosition = position;
            return;
        }
        call(Node_stopAllActions, {self->scrollBar});
        const uint32_t bar = self->scrollBar;
        const uint32_t delay = call(DelayTime_create, {fbits(BAR_HIDE_DELAY)});
        const uint32_t fade = call(fn::FadeTo_create, {fbits(BAR_FADE), 0});
        call(fn::Node_runAction, {bar, call(fn::Sequence_create, {delay, fade, 0})});
        self->scrollBarShown = 0;
    }
    if (self->lastScrollPosition - position != 0.0f) {
        call(Node_stopAllActions, {self->scrollBar});
        const uint32_t bar = self->scrollBar;
        call(fn::Node_runAction, {bar, call(fn::FadeTo_create, {fbits(BAR_FADE), 255})});
        self->scrollBarShown = 1;
    }
    self->lastScrollPosition = position;
}

// a new size rebuilds the list: the scroll view, then the records (rows are added by update)
void MyRecordScrollView::setContentSize(MyRecordScrollView *self, Addr size) {
    const uint32_t addr = rw::guest_addr(self);
    {
        Frame f;
        const uint32_t old = f.alloc(8);
        call(Size_copy, {old, vcall(addr, node::getContentSize, {addr})});
        call(Widget_setContentSize, {addr, size});
        if (ldf(old) == ldf(size) && ldf(old + 4) == ldf(size + 4)) return;
    }
    {
        Frame f;
        const uint32_t copy = f.alloc(8);
        call(Size_copy, {copy, size});
        self->scrollView = call(ScrollView_create, {copy, 0});
    }
    const uint32_t view = self->scrollView;
    vcall(view, node::setAnchorPoint, {view, ANCHOR_BOTTOM_LEFT});
    vcall(self->scrollView, node::setPosition, {self->scrollView, VEC2_ZERO});
    vcall(self->scrollView, VSLOT_setDirection, {self->scrollView, 1});
    vcall(addr, node::addChild, {addr, self->scrollView});
    self->contentHeight = 0.0f;

    Frame f;
    const uint32_t categories = f.alloc(12);
    makeIntVector(categories, CATEGORIES, 4);

    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t profileCtl = ld32(api + APIManager_profile + 4);
    const uint32_t profile = ld32(api + APIManager_profile);
    if (profileCtl) call(fn::shared_add, {profileCtl});

    // the rows' text ids and values, per category
    const uint32_t intParts = f.alloc(48);
    makeIntVector(intParts, ROWS_0, 29);
    makeIntVector(intParts + 12, ROWS_1, 13);
    makeIntVector(intParts + 24, ROWS_2, 2);
    makeIntVector(intParts + 36, ROWS_3, 3);
    const uint32_t rowIds = f.alloc(12);
    makeNested(rowIds, intParts, VectorVectorInt_allocate, VectorInt_copy, VectorInt_destroy);
    const uint32_t u64Parts = f.alloc(48);
    makeU64Vector(u64Parts, profile, VALUES_0, 29);
    makeU64Vector(u64Parts + 12, profile, VALUES_1, 13);
    makeU64Vector(u64Parts + 24, profile, VALUES_2, 2);
    makeU64Vector(u64Parts + 36, profile, VALUES_3, 3);
    const uint32_t rowValues = f.alloc(12);
    makeNested(rowValues, u64Parts, VectorVectorU64_allocate, VectorU64_copy, VectorU64_destroy);

    const uint32_t record = f.alloc(0x18);
    for (uint32_t k = 0; k < 0x18; k += 4) st32(record + k, 0);
    const uint32_t groups = (ld32(rowIds + 4) - ld32(rowIds)) / 12;
    for (uint32_t c = 0; c < groups; c++) {
        self->contentHeight = height(self->categoryTemplate) + self->contentHeight;
        st32(record, ld32(ld32(categories) + 4 * c));
        st32(record + 8, 0);
        st32(record + 12, 0);
        st8(record + 0x10, 1);
        pushRecord(self, record, true);
        const uint32_t ids = ld32(rowIds) + 12 * c;
        const uint32_t values = ld32(rowValues) + 12 * c;
        for (uint32_t i = 0; i < (ld32(ids + 4) - ld32(ids)) / 4; i++) {
            self->contentHeight = height(self->panelTemplate) + self->contentHeight;
            st32(record, ld32(ld32(ids) + 4 * i));
            st32(record + 8, ld32(ld32(values) + 8 * i));
            st32(record + 12, ld32(ld32(values) + 8 * i + 4));
            st8(record + 0x10, 0);
            pushRecord(self, record, false);
        }
    }

    self->nextY = self->contentHeight;
    self->building = 1;
    self->built = 0;
    {
        Frame g;
        const uint32_t contentSize = g.alloc(8);
        const uint32_t sv = self->scrollView;
        call(Size_ctorWH, {contentSize, rw::load<uint32_t>(size), fbits(float(self->contentHeight))});
        vcall(sv, node::setContentSize, {sv, contentSize});
        const uint32_t offset = g.alloc(8);
        call(Vec2_ctorXY, {offset, 0, fbits(ldf(size + 4) - self->contentHeight)});
        call(ScrollView_setContentOffset, {self->scrollView, offset, 0});
        call(Vec2_dtor, {offset});
    }
    setEnabledScrollBar(self, 1);
    call(VectorVectorU64_destroy, {rowValues});
    call(VectorVectorInt_destroy, {rowIds});
    if (profileCtl) call(fn::shared_release, {profileCtl});
    call(VectorInt_destroy, {categories});
}

void MyRecordScrollView::setEnabledScrollBar(MyRecordScrollView *self, uint32_t enabled) {
    if (self->scrollBarEnabled == enabled) return;
    self->scrollBarEnabled = uint8_t(enabled);
    const uint32_t addr = rw::guest_addr(self);
    if (!enabled) {
        vcall(addr, VSLOT_removeChild, {addr, self->scrollBar, 1});
        self->scrollBar = 0;
        return;
    }
    self->scrollBar = call(MyRecordScrollBar_create, {0, vcall(addr, node::getContentSize, {addr})});
    const uint32_t track = ld32(self->scrollBar + 0x288);
    vcall(track, node::setVisible, {track, 0});
    vcall(self->scrollBar, node::setOpacity, {self->scrollBar, 0});
    const uint32_t bar = self->scrollBar;
    const float width = ldf(vcall(addr, node::getContentSize, {addr}));
    const float barWidth = ldf(vcall(self->scrollBar, node::getContentSize, {self->scrollBar}));
    vcall(bar, node::setPositionX, {bar, fbits(width - barWidth)});
    call(SceneUtil_setCascadeOpacityEnabled, {self->scrollBar, 1});
    vcall(addr, node::addChildZ, {addr, self->scrollBar, 0x7FFFFFFF});
    st32(self->scrollBar + 0x27C, ld32(addr + 0x508));   // the bar's content height
    call(MyRecordScrollBar_updateLayout, {self->scrollBar});
}

RW_FUNCTION(0x519800, "MyRecordScrollView::create", MyRecordScrollView::create);
RW_FUNCTION(0x5198B8, "MyRecordScrollView::init", MyRecordScrollView::init);
RW_FUNCTION(0x5196EC, "MyRecordScrollView::~MyRecordScrollView", MyRecordScrollView::destruct);
RW_FUNCTION(0x519A2C, "MyRecordScrollView::update", MyRecordScrollView::update);
RW_FUNCTION(0x51A090, "MyRecordScrollView::setContentSize", MyRecordScrollView::setContentSize);
RW_FUNCTION(0x51AF34, "MyRecordScrollView::setEnabledScrollBar", MyRecordScrollView::setEnabledScrollBar);
