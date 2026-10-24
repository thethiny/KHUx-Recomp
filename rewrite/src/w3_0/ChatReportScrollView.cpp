// ChatReportScrollView: the list of report reasons in the chat's "report post" popup. A BaseScrollView<int>
// (IScrollProtocol, a cocos2d::ui::Layout) whose items are reason numbers 1..n; each row shows the reason text
// (TextData) and an ImageButton; a tap reports the reason through onSelect (std::function<void(int)> at +0x638,
// set by SceneManager's chat popup).
#include "../scene/SceneCommon.hpp"
#include "../scene_main/IScrollProtocol.hpp"

using namespace scene;

namespace {
constexpr uint32_t SIZE = 0x658;
constexpr uint32_t VTABLE_BASE = 0xF88C48;           // _ZTV20ChatReportScrollView (+8, +0x360, +0x374)
constexpr uint32_t BASE_SCROLL_VIEW_VTABLE = 0xF811D0;  // _ZTV14BaseScrollViewIiE
constexpr uint32_t ITEM_IDS = 0x628;                 // BaseScrollView<int>: std::vector<int> item ids
constexpr uint32_t ON_SELECT = 0x638;                // std::function<void(int)> (callable pointer at +0x648)
constexpr uint32_t FIELD_650 = 0x650;
constexpr uint32_t PANEL_TEMPLATE = 0x654;           // ui::Widget* row template (retained)

constexpr uint32_t BASE_SCROLL_VIEW_CTOR = 0x55DBF5;        // BaseScrollView<int>::BaseScrollView()
constexpr uint32_t BASE_SCROLL_VIEW_INIT = 0x4FA8A1;        // BaseScrollView<int>::init()
constexpr uint32_t BASE_SCROLL_VIEW_SET_ITEM_IDS = 0x478929;  // BaseScrollView<int>::setItemIDs(const vector<int>&)
constexpr uint32_t ISCROLL_DTOR = 0x4F57E1;                 // IScrollProtocol::~IScrollProtocol()
constexpr uint32_t ISCROLL_UPDATE = 0x4F5CFD;               // IScrollProtocol::update(float)
constexpr uint32_t ISCROLL_SET_ENABLED_SCROLL_BAR = 0x4F6FE9;
constexpr uint32_t INT_VECTOR_DTOR = 0x478961;              // std::__vector_base<int>::~__vector_base
constexpr uint32_t INT_VECTOR_PUSH_BACK_SLOW = 0x47D709;    // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t VECTOR_THROW_OUT_OF_RANGE = 0x47CCE1;
constexpr uint32_t REF_RETAIN = rwlib::cocos2d::Ref::retain, REF_RELEASE = rwlib::cocos2d::Ref::release, REF_AUTORELEASE = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t WIDGET_CREATE = rwlib::cocos2d::ui::Widget::create;                // ui::Widget::create()
constexpr uint32_t WIDGET_CLONE = rwlib::cocos2d::ui::Widget::clone;                 // ui::Widget::clone()
constexpr uint32_t SIZE_CTOR = rwlib::cocos2d::Size::ctor_base__ff;                    // cocos2d::Size::Size(float, float)
constexpr uint32_t GUIREADER_GET_INSTANCE = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIREADER_WIDGET_FROM_JSON = rwlib::cocostudio::GUIReader::widgetFromJsonFile;   // GUIReader::widgetFromJsonFile(const char*)
constexpr uint32_t SEARCH_NODE_BY_NAME = 0x45A9F5;          // SceneUtil::searchNodeByName(Node*, const string&)
constexpr uint32_t SET_SHADOW = 0x45BD15;                   // SceneUtil::setShadow(ui::Text*)
constexpr uint32_t TEXT_SET_STRING = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t IMAGE_BUTTON_CREATE = 0x531239;          // ImageButton::create(std::string, bool)
constexpr uint32_t IMAGE_BUTTON_SET_TARGET_VIEW = 0x5318C1; // ImageButton::setTargetView(Node*, float)
constexpr uint32_t IMAGE_BUTTON_ADD_TOUCH_LISTENER = 0x5315B1;  // ImageButton::addTouchEventListener(std::function)
constexpr uint32_t STRLEN = 0x40E08C;

constexpr uint32_t ANCHOR_MIDDLE = 0xFEF5C0;                // cocos2d::Vec2::ANCHOR_MIDDLE
constexpr uint32_t REASON_TEXT_IDS = 0xE30CCC;              // int[]: TextData id of reason n at [n - 1]
constexpr uint32_t TOUCH_LAMBDA_VTABLE = 0xF58C24;          // __func<changeRow::$_..> + 8: captures {this, id}
constexpr uint32_t STR_PANEL_JSON = 0xDFEDC1;               // "cocostudio/publish/Chat_Post_ReportPanel.json"
constexpr uint32_t STR_MAIN_PANEL = 0xDFD1FC;               // "mainPanel"
constexpr uint32_t STR_TXT_NUM_LABEL = 0xDFD8DC;            // "Txt_Num_Label"
constexpr uint32_t STR_HALF_CONTENT = 0xDFEDEF;             // "HalfContent"
constexpr uint32_t STR_HALF_CONTENT_IMAGE_BUTTON = 0xDFEDFB;  // "HalfContent_ImageButton"
constexpr uint32_t STR_HALF_CONTENT_BUT = 0xDFEE13;         // "HalfContent_But"

constexpr uint32_t VSLOT_DELETING_DTOR = 0x4;
constexpr uint32_t VSLOT_CHANGE_ROW = 0x340;                // (widget, index, int)

uint32_t addr(const void *p) { return rw::guest_addr(p); }

void setVtables(uint32_t self, uint32_t vtable) {
    st32(self, vtable + 8);
    st32(self + 0x280, vtable + 0x360);
    st32(self + 0x350, vtable + 0x374);
}

uint32_t searchNode(uint32_t root, uint32_t name, uint32_t length) {
    Frame f;
    uint32_t s = f.alloc(12);
    strInit(s, name, length);
    uint32_t node = call(SEARCH_NODE_BY_NAME, {root, s});
    strFree(s);
    return node;
}
}  // namespace

struct ChatReportScrollView {
    static uint32_t destruct(uint32_t self) {
        setVtables(self, VTABLE_BASE);
        uint32_t panel = ld32(self + PANEL_TEMPLATE);
        if (panel) call(REF_RELEASE, {panel});
        functionDestroy(self + ON_SELECT);
        setVtables(self, BASE_SCROLL_VIEW_VTABLE);
        call(INT_VECTOR_DTOR, {self + ITEM_IDS});
        call(ISCROLL_DTOR, {self});
        return self;
    }
    static void deletingDestruct(uint32_t self) {
        destruct(self);
        call(fn::operator_delete, {self});
    }

    static uint32_t create() {
        uint32_t self = call(fn::operator_new, {SIZE});
        call(BASE_SCROLL_VIEW_CTOR, {self});
        setVtables(self, VTABLE_BASE);
        st32(self + ON_SELECT + 16, 0);
        st32(self + FIELD_650, 0);
        if (init(self) == 1) {
            call(REF_AUTORELEASE, {self});
            return self;
        }
        vcall(self, VSLOT_DELETING_DTOR, {self});
        return 0;
    }

    // rows 476 x 70 with a scroll bar; the row template is Chat_Post_ReportPanel.json in a Widget
    static uint32_t init(uint32_t self) {
        auto *view = rw::host_ptr<IScrollProtocol>(self);
        view->itemWidth = 476.0f;
        view->itemHeight = 70.0f;
        call(ISCROLL_SET_ENABLED_SCROLL_BAR, {self, 1});
        view->offsetX = float(view->itemWidth) * 0.5f;
        view->offsetY = -10.0f;
        uint32_t panel = call(WIDGET_CREATE, {});
        st32(self + PANEL_TEMPLATE, panel);
        uint32_t setContentSize = ld32(ld32(panel) + node::setContentSize);
        Frame f;
        uint32_t size = f.alloc(8);
        call(SIZE_CTOR, {size, fbits(float(view->itemWidth)), fbits(float(view->itemHeight))});
        call(setContentSize, {panel, size});
        panel = ld32(self + PANEL_TEMPLATE);
        vcall(panel, node::setAnchorPoint, {panel, ANCHOR_MIDDLE});
        call(REF_RETAIN, {ld32(self + PANEL_TEMPLATE)});
        panel = ld32(self + PANEL_TEMPLATE);
        vcall(panel, node::setVisible, {panel, 0});
        uint32_t content = call(GUIREADER_WIDGET_FROM_JSON, {call(GUIREADER_GET_INSTANCE, {}), STR_PANEL_JSON});
        panel = ld32(self + PANEL_TEMPLATE);
        uint32_t addChild = ld32(ld32(panel) + node::addChildZName);
        uint32_t name = f.alloc(12);
        strInit(name, STR_MAIN_PANEL, 9);
        call(addChild, {panel, content, 0, name});
        strFree(name);
        return call(BASE_SCROLL_VIEW_INIT, {self});
    }

    static void update(uint32_t self, float dt) { call(ISCROLL_UPDATE, {self, fbits(dt)}); }

    static uint32_t getLayoutPanel(uint32_t self, int32_t) { return call(WIDGET_CLONE, {ld32(self + PANEL_TEMPLATE)}); }

    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t arg) {
        uint32_t count = uint32_t(int32_t(ld32(self + ITEM_IDS + 4) - ld32(self + ITEM_IDS)) >> 2);
        if (count <= index) call(VECTOR_THROW_OUT_OF_RANGE, {self + ITEM_IDS});
        uint32_t row = call(WIDGET_CLONE, {ld32(self + PANEL_TEMPLATE)});
        vcall(self, VSLOT_CHANGE_ROW, {self, row, index, arg});
        return row;
    }

    // Fill a row with reason itemIDs[index]: its text, and a button that reports it.
    static uint32_t changeRow(uint32_t self, uint32_t row, uint32_t index, int32_t) {
        uint32_t begin = ld32(self + ITEM_IDS);
        if (uint32_t(int32_t(ld32(self + ITEM_IDS + 4) - begin) >> 2) <= index)
            call(VECTOR_THROW_OUT_OF_RANGE, {self + ITEM_IDS});
        const uint32_t item = begin + index * 4;
        if (ld32(item) == 0) return 0;
        uint32_t label = searchNode(row, STR_TXT_NUM_LABEL, 13);
        if (!label) return 0;
        vcall(row, node::setVisible, {row, 1});
        call(SET_SHADOW, {label});
        Frame f;
        uint32_t text = f.alloc(8), str = f.alloc(12);
        call(fn::TextData_get, {text, 0, ld32(REASON_TEXT_IDS + ld32(item) * 4)});
        uint32_t chars = ld32(ld32(text) + 4);
        strInit(str, chars, call(STRLEN, {chars}));
        call(TEXT_SET_STRING, {label, str});
        strFree(str);
        sharedRelease(text);
        uint32_t halfContent = searchNode(row, STR_HALF_CONTENT, 11);
        uint32_t button = searchNode(row, STR_HALF_CONTENT_IMAGE_BUTTON, 23);
        if (!button) {
            uint32_t s = f.alloc(12);
            strInit(s, STR_HALF_CONTENT_BUT, 15);
            button = call(IMAGE_BUTTON_CREATE, {s, 0});
            strFree(s);
            call(IMAGE_BUTTON_SET_TARGET_VIEW, {button, halfContent, fbits(1.0f)});
            uint32_t addChild = ld32(ld32(halfContent) + node::addChildZName);
            uint32_t name = f.alloc(12);
            strInit(name, STR_HALF_CONTENT_IMAGE_BUTTON, 23);
            call(addChild, {halfContent, button, 0, name});
            strFree(name);
        }
        uint32_t listener = newFunction(f, TOUCH_LAMBDA_VTABLE, {self, ld32(item)});
        call(IMAGE_BUTTON_ADD_TOUCH_LISTENER, {button, listener});
        functionDestroy(listener);
        return 0;
    }

    static void registItemTouchEvent(uint32_t, uint32_t, int32_t) {}

    // the items are the reasons 1..count
    static void setIds(uint32_t self, int32_t count) {
        Frame f;
        uint32_t value = f.alloc(4), ids = f.alloc(12);
        st32(ids, 0);
        st32(ids + 4, 0);
        st32(ids + 8, 0);
        for (int32_t n = 1; n <= count; n++) {
            st32(value, uint32_t(n));
            uint32_t end = ld32(ids + 4);
            if (end < ld32(ids + 8)) {
                uint32_t next = 0;
                if (end) {
                    st32(end, uint32_t(n));
                    next = ld32(ids + 4);
                }
                st32(ids + 4, next + 4);
            } else {
                call(INT_VECTOR_PUSH_BACK_SLOW, {ids, value});
            }
        }
        call(BASE_SCROLL_VIEW_SET_ITEM_IDS, {self, ids});
        call(INT_VECTOR_DTOR, {ids});
    }
};

RW_FUNCTION(0x55A0AC, "ChatReportScrollView::~ChatReportScrollView", ChatReportScrollView::destruct);
RW_FUNCTION(0x55A1DC, "ChatReportScrollView::~ChatReportScrollView (deleting)", ChatReportScrollView::deletingDestruct);
RW_FUNCTION(0x55A210, "ChatReportScrollView::create", ChatReportScrollView::create);
RW_FUNCTION(0x55A2A0, "ChatReportScrollView::init", ChatReportScrollView::init);
RW_FUNCTION(0x55A408, "ChatReportScrollView::update", ChatReportScrollView::update);
RW_FUNCTION(0x55A410, "ChatReportScrollView::getLayoutPanel", ChatReportScrollView::getLayoutPanel);
RW_FUNCTION(0x55A420, "ChatReportScrollView::createRow", ChatReportScrollView::createRow);
RW_FUNCTION(0x55A47C, "ChatReportScrollView::changeRow", ChatReportScrollView::changeRow);
RW_FUNCTION(0x55A7A4, "ChatReportScrollView::registItemTouchEvent", ChatReportScrollView::registItemTouchEvent);
RW_FUNCTION(0x55A7A8, "ChatReportScrollView::setIds", ChatReportScrollView::setIds);
