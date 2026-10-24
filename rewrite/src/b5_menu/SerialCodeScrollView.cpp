// SerialCodeScrollView: the serial code list (Other menu > Serial code > list): one Information2_SerialPanel
// row per serialcodeReward id - the campaign name, up to six reward items (Ranking_IconPanel icons, "name x n")
// and the code's end date. A BaseScrollView<int>.
#include "../w3_9/BaseScrollView.hpp"

using namespace scene;

namespace {
struct SerialCodeScrollView : BaseScrollView {
    // +0x638 std::function (callable at +0x648), +0x650 the row template (an empty widget with the panel)
    const uint8_t members[0x658 - 0x634];

    static constexpr uint32_t SIZE = 0x658;
    static constexpr uint32_t VTABLE = 0xF88198, VTABLE2 = 0xF884F0, VTABLE3 = 0xF88504;
    static constexpr uint32_t FUNCTION = 0x638;
    static constexpr uint32_t PANEL = 0x650;
};
using Self = SerialCodeScrollView;
RW_SIZE(SerialCodeScrollView, SerialCodeScrollView::SIZE);

namespace g {
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create;
constexpr uint32_t Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;  // (string& image, string& name, type, id, bool)
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (buffer, dst, src, size, offset)
constexpr uint32_t SerialcodeReward_create = 0x79006D;     // master::SerialcodeReward::create(shared_ptr out, id)
constexpr uint32_t ArrayInt6_at = 0x55673D;                // std::array<int, 6>::at
constexpr uint32_t RankingRewardScrollView_setIncentiveItem = 0x55E4D1;
constexpr uint32_t Date_makeLocalDate = 0x4927CD;          // (Date out, const Time&)
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t shared_release = 0xC8DBD1;
}  // namespace g

constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t S_SERIAL_PANEL_JSON = 0xDFEBFC;   // "cocostudio/publish/Information2_SerialPanel.json"
constexpr uint32_t S_ICON_PANEL_JSON = 0xDFEC2D;     // "cocostudio/publish/Ranking_IconPanel.json"
constexpr uint32_t S_AREA = 0xDF9378;                // "Area" (4)
constexpr uint32_t S_ITEM_PANEL = 0xDFD4B4;          // "ItemPanel%d"
constexpr uint32_t S_SERIAL_PANEL = 0xDFEC57;        // "SerialPanel" (11)
constexpr uint32_t S_CAMPAIGN_LABEL = 0xDFEC63;      // "Txt_Campaign_Label" (18)
constexpr uint32_t S_INCENTIVE_LABEL = 0xDFEC76;     // "Txt_Incentive%d_Label"
constexpr uint32_t S_ITEM_COUNT = 0xDFEC8C;          // "%s x %d"
constexpr uint32_t S_DATE_LABEL = 0xDFEC95;          // "Txt_Date_Label" (14)
constexpr uint32_t S_DATE = 0xDFECA4;                // "%s: %4d/%02d/%02d %02d:%02d"
constexpr int32_t TXT_DATE = 104000006;

// master::SerialcodeReward fields
constexpr uint32_t R_NAME = 4, R_DATE = 270, R_COUNT = 284, R_TYPES = 288, R_IDS = 312, R_RARITIES = 360,
                   R_AMOUNTS = 408;

constexpr uint32_t V_deleting = 0x04;
constexpr uint32_t V_setPosition = 0x4C;
constexpr uint32_t V_setAnchorPoint = 0x9C;
constexpr uint32_t V_setContentSize = 0xA8;
constexpr uint32_t V_setVisible = 0xB0;
constexpr uint32_t V_addChild = 0xF0;
constexpr uint32_t V_addChildZName = 0xFC;
constexpr uint32_t V_init = 0x248;
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;

inline uint32_t idCount(uint32_t me) { return (ld32(me + 0x62C) - ld32(me + 0x628)) >> 2; }
inline uint32_t fb(float f) { return rw::fbits(f); }

// a node by a printf-made name ("ItemPanel%d", n)
uint32_t findNodeF(uint32_t root, uint32_t fmt, uint32_t n) {
    Frame f;
    uint32_t buf = stringBuffer(f, {fmt, n});
    uint32_t s = newStr(f, buf);
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    strFree(s);
    return node;
}
// SafeBuffer::read of `size` bytes at `offset` of the row into dst
void readField(uint32_t row, uint32_t dst, uint32_t offset, uint32_t size) {
    call(g::SafeBuffer_read, {row, dst, ld32(row + 0xC) + offset, size, offset});
}
// the copy constructor of std::string, as inlined: a long string through __init, a short one word by word
void strCopy(uint32_t dst, uint32_t src) {
    if (strIsLong(src)) {
        strInit(dst, ld32(src + 8), ld32(src + 4));
        return;
    }
    st32(dst, ld32(src));
    st32(dst + 4, ld32(src + 4));
    st32(dst + 8, ld32(src + 8));
}
}  // namespace

namespace {
Self *construct(Self *self) {
    uint32_t me = self->self();
    call(g::Layout_ctor, {me});
    BaseScrollView::initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = Self::VTABLE;
    self->vtable2 = Self::VTABLE2;
    self->vtable3 = Self::VTABLE3;
    st32(me + Self::FUNCTION + 16, 0);
    return self;
}

uint32_t create() {
    uint32_t p = call(fn::operator_new, {Self::SIZE});
    construct(rw::host_ptr<Self>(p));
    if (vcall(p, V_init, {p}) == 1) {
        call(g::Ref_autorelease, {p});
        return p;
    }
    vcall(p, V_deleting, {p});
    return 0;
}

Self *destroy(Self *self) {
    uint32_t me = self->self();
    self->vtable = Self::VTABLE;
    self->vtable2 = Self::VTABLE2;
    self->vtable3 = Self::VTABLE3;
    if (uint32_t panel = ld32(me + Self::PANEL)) call(g::Ref_release, {panel});
    functionDestroy(me + Self::FUNCTION);
    self->vtable = BaseScrollView::VTABLE_INT;
    self->vtable2 = BaseScrollView::VTABLE2_INT;
    self->vtable3 = BaseScrollView::VTABLE3_INT;
    call(g::VectorBaseInt_dtor, {me + 0x628});
    IScrollProtocol::destroy(self);
    return self;
}

// rows of 908 x 186: the template is an empty centred widget with the serial panel, its "Area" holding six
// icon panels ("ItemPanel1".."6", 88 pixels apart, hidden until filled)
uint32_t init(Self *self) {
    uint32_t me = self->self();
    self->itemWidth = 908.0f;
    self->itemHeight = 186.0f;
    IScrollProtocol::setEnabledScrollBar(self, 1);
    self->offsetX = self->itemWidth * 0.5f;
    uint32_t panel = call(g::Widget_create, {});
    st32(me + Self::PANEL, panel);
    {
        Frame f;
        uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fb(self->itemWidth), fb(self->itemHeight)});
        vcall(panel, V_setContentSize, {panel, size});
    }
    panel = ld32(me + Self::PANEL);
    vcall(panel, V_setAnchorPoint, {panel, ld32(GOT_ANCHOR_MIDDLE)});
    call(g::Ref_retain, {ld32(me + Self::PANEL)});
    panel = ld32(me + Self::PANEL);
    vcall(panel, V_widgetSetTouchEnabled, {panel, 0});
    uint32_t serial = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_SERIAL_PANEL_JSON});
    panel = ld32(me + Self::PANEL);
    vcall(panel, V_addChild, {panel, serial});
    uint32_t area = findNode(ld32(me + Self::PANEL), S_AREA, 4);
    uint32_t icon = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_ICON_PANEL_JSON});
    for (int32_t i = 0; i < 6; i++) {
        uint32_t clone = call(g::Widget_clone, {icon});
        {
            Frame f;
            uint32_t pos = f.alloc(8);
            call(g::Vec2_ctor, {pos, fb(float(i) * 88.0f), fb(18.0f)});
            vcall(clone, V_setPosition, {clone, pos});
            call(g::Vec2_dtor, {pos});
        }
        vcall(clone, V_setVisible, {clone, 0});
        uint32_t add = ld32(ld32(area) + V_addChildZName);
        Frame f;
        uint32_t buf = stringBuffer(f, {S_ITEM_PANEL, uint32_t(i + 1)});
        uint32_t name = newStr(f, buf);
        call(add, {area, clone, 0, name});
        strFree(name);
    }
    return BaseScrollView::initInt(self);
}

uint32_t createRow(Self *self, uint32_t index, int32_t arg) {
    uint32_t me = self->self();
    if (idCount(me) <= index) call(g::THROW_OUT_OF_RANGE, {});
    uint32_t row = call(g::Widget_clone, {ld32(me + Self::PANEL)});
    vcall(me, IScrollProtocol::V_isReplaceItem, {me, row, index, uint32_t(arg)});
    return row;
}

// fills a row from the serialcodeReward entry of its id; 1 when the widget is not a serial row
uint32_t changeRow(Self *self, uint32_t widget, uint32_t index, int32_t) {
    uint32_t me = self->self();
    uint32_t ids = self->itemIDs.begin;
    if (idCount(me) <= index) call(g::THROW_OUT_OF_RANGE, {});
    if (!findNode(widget, S_SERIAL_PANEL, 11)) return 1;

    Frame f;
    uint32_t reward = f.alloc(8);
    call(g::SerialcodeReward_create, {reward, ld32(ids + 4 * index)});
    uint32_t row = ld32(reward);
    {
        uint32_t label = findNode(widget, S_CAMPAIGN_LABEL, 18);
        call(g::SceneUtil_setShadow, {label});
        Frame g2;
        uint32_t name = g2.alloc(0x120);
        readField(row, name, R_NAME, 0x101);
        uint32_t s = newStr(g2, name);
        call(g::Text_setString, {label, s});
        strFree(s);
    }
    uint32_t area = findNode(widget, S_AREA, 4);
    int32_t count;
    {
        Frame g2;
        uint32_t v = g2.alloc(4);
        readField(row, v, R_COUNT, 4);
        count = int32_t(ld32(v));
    }
    for (int32_t i = 0; i < 6; i++) {
        uint32_t item = findNodeF(area, S_ITEM_PANEL, uint32_t(i + 1));
        uint32_t label = findNodeF(widget, S_INCENTIVE_LABEL, uint32_t(i + 1));
        if (i >= count) {
            vcall(item, V_setVisible, {item, 0});
            vcall(label, V_setVisible, {label, 0});
            continue;
        }
        Frame g2;
        uint32_t arr = g2.alloc(0x18);
        auto field = [&](uint32_t offset) {
            readField(row, arr, offset, 0x18);
            return ld32(call(g::ArrayInt6_at, {arr, uint32_t(i)}));
        };
        uint32_t type = field(R_TYPES);
        uint32_t id = field(R_IDS);
        uint32_t rarity = field(R_RARITIES);
        // setItemImageData's two strings: the item's name (shown "name x amount") and its image
        uint32_t name = g2.alloc(12), image = g2.alloc(12);
        for (uint32_t k = 0; k < 12; k += 4) {
            st32(name + k, 0);
            st32(image + k, 0);
        }
        call(g::SceneUtil_setItemImageData, {name, image, type, id, 0});
        vcall(item, V_setVisible, {item, 1});
        uint32_t imageCopy = g2.alloc(12), nameCopy = g2.alloc(12);
        strCopy(imageCopy, image);
        strCopy(nameCopy, name);
        uint32_t amount = field(R_AMOUNTS);
        call(g::RankingRewardScrollView_setIncentiveItem,
             {item, type, rarity, id, imageCopy, nameCopy, amount, uint32_t(i + 4), 0});
        strFree(nameCopy);
        strFree(imageCopy);
        uint32_t text = strData(name);
        amount = field(R_AMOUNTS);
        {
            Frame g3;
            uint32_t buf = stringBuffer(g3, {S_ITEM_COUNT, text, amount});
            uint32_t s = newStr(g3, buf);
            call(g::Text_setString, {label, s});
            strFree(s);
        }
        vcall(label, V_setVisible, {label, 1});
        call(g::SceneUtil_setShadow, {label});
        strFree(name);
        strFree(image);
    }
    uint32_t date = findNode(widget, S_DATE_LABEL, 14);
    call(g::SceneUtil_setShadow, {date});
    {
        Frame g2;
        uint32_t time = g2.alloc(8), local = g2.alloc(8), text = g2.alloc(8);
        readField(row, time, R_DATE, 8);
        call(g::Date_makeLocalDate, {local, time});
        call(g::TextData_get, {text, 0, uint32_t(TXT_DATE)});
        uint32_t ymd = ld32(local + 2), hm = ld32(local + 4);
        uint32_t buf = stringBuffer(g2, {S_DATE, ld32(ld32(text) + 4), uint32_t(rw::load<uint16_t>(local)),
                                         ymd & 0xFF, (ymd >> 8) & 0xFF, hm & 0xFF, (hm >> 8) & 0xFF});
        uint32_t s = newStr(g2, buf);
        call(g::Text_setString, {date, s});
        strFree(s);
        if (uint32_t ctrl = ld32(text + 4)) call(g::shared_release, {ctrl});
    }
    if (uint32_t ctrl = ld32(reward + 4)) call(g::shared_release, {ctrl});
    return 0;
}
}  // namespace

RW_FUNCTION(0x555BE8, "SerialCodeScrollView::SerialCodeScrollView()", construct);
RW_FUNCTION(0x555B94, "SerialCodeScrollView::create()", create);
RW_FUNCTION(0x555A34, "SerialCodeScrollView::~SerialCodeScrollView()", destroy);
RW_FUNCTION(0x555D1C, "SerialCodeScrollView::init()", init);
RW_FUNCTION(0x555FA0, "SerialCodeScrollView::createRow(int, int)", createRow);
RW_FUNCTION(0x555FF8, "SerialCodeScrollView::changeRow(Widget*, int, int)", changeRow);
