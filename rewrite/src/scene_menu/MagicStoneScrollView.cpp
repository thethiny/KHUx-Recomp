// MagicStoneScrollView: the Jewel pack list of the Buy Jewels dialog (see MagicStoneScrollView.hpp).
#include "MagicStoneScrollView.hpp"

#include "../master/Shop.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = MagicStoneScrollView;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;                 // cocos2d::ui::Layout::Layout()
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;          // std::__vector_base<int>::~__vector_base
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;             // vector::at out of range (throws)
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Size_ctor = 0x8DF027;                   // Size(out, float w, float h)
constexpr uint32_t Text_setFontName = 0x83A499;            // (text, const string&)
constexpr uint32_t Text_setFontSize = 0x83A35D;            // (text, int)
constexpr uint32_t Shop_create = 0x790951;                 // master::Shop::create(shared_ptr out, int id)
constexpr uint32_t ImageButton_create = 0x531239;          // (std::string file, bool)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;   // (button, std::function by value)
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;
// std::unordered_map<std::string, iap::ProductInfo>
constexpr uint32_t ProductMap_dtor = 0x4DABC7;             // __hash_table::~__hash_table
constexpr uint32_t ProductMap_clear = 0x4FBB73;            // __hash_table::clear
constexpr uint32_t ProductMap_insert = 0x4FB9A1;           // __insert_unique(pair<iterator,bool>* out, table, const value&)
constexpr uint32_t ProductMap_find = 0x4DA18F;             // find(table, const string&) -> node (0: end)
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;      // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;      // std::function<void(Touch*, Event*)>::swap
}  // namespace g

// strings
constexpr uint32_t S_ItemBaseJson = 0xDFCF6B;    // "cocostudio/publish/ShopBuyItem_ItemBase_ver120.json"
constexpr uint32_t S_ItemBase = 0xDFCF9F;        // "ItemBase"
constexpr uint32_t S_Empty = 0xDF8A1A;           // ""
constexpr uint32_t S_Price = 0xDFCFA8;           // "Price"
constexpr uint32_t S_Gain = 0xDFCFAE;            // "Gain"
constexpr uint32_t S_ItemImage = 0xDFCFB3;       // "ItemImage"
constexpr uint32_t S_ItemImagePath = 0xDFCFBD;   // "img/shop/Shop_BuyItem_%02d.png"
constexpr uint32_t S_MagicStone = 0xDFCFDC;      // "MagicStone"
constexpr uint32_t S_NumLabel = 0xDF8A89;        // "Num_Label"
constexpr uint32_t S_PriceFont = 0xDFCFE7;       // "微软雅黑" (Microsoft YaHei, 12 bytes)
constexpr uint32_t S_Balloon = 0xDFCFF4;         // "BuyItem_Balloon"
constexpr uint32_t S_TxtBuy = 0xDFD004;          // "Txt_Buy"
constexpr uint32_t S_BuyButton = 0xDFD00C;       // "Buy_Button"
constexpr uint32_t S_BuyImageButton = 0xDFD017;  // "Buy_ImageButton"
constexpr uint32_t S_But01 = 0xDFD027;           // "But01"
constexpr uint32_t S_SaleBalloon = 0xDFD02D;     // "BuyItem_Balloon_Sale"
constexpr uint32_t S_SkipBalloon = 0xDFD042;     // "SkipTicket_Balloon"

// text ids
constexpr int32_t TXT_JEWEL_COUNT = 102600028;   // "%d" Jewels of the pack
constexpr int32_t TXT_BONUS = 102600029;         // the bonus balloon
constexpr int32_t TXT_BUY = 102600023;
constexpr int32_t TXT_SKIP_TICKETS = 102600059;
constexpr int32_t MISC_PURCHASE_LIMIT = 118;     // master::Misc row: Jewel count checked by hasMagicStone

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_BuyTouch = 0xF55C04;       // changeRow $_0 {this, shop id}: the Buy button
constexpr uint32_t VT_TouchBegan = 0xF55BD8;     // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_TouchMoved = 0xF55BAC;     // $_1 {this, index, $_2 {this, index, this, index}} (heap, 28 bytes)
constexpr uint32_t VT_TouchEnded = 0xF55B54;     // $_2 {this, index, this, index} (heap, 20 bytes)

// APIManager: Ret (functionFlags at +0x34) and the benefits already used (vector of 12-byte entries, id first)
constexpr uint32_t API_RET = 0x44, RET_FUNCTION_FLAGS = 0x34, API_USED_BENEFITS = 0xAC;
constexpr uint32_t FLAG_IGNORE_APPLE_STATUS = 4;
// ImageButton: the object it reports to
constexpr uint32_t IMAGEBUTTON_DELEGATE = 0x2B0;

// node virtual slots (byte offsets)
constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_changeRow = 0x340;

inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
// searchNodeByName(root, literal) then SceneUtil::setShadow on it
inline uint32_t shadowed(uint32_t root, uint32_t name, uint32_t len) {
    uint32_t text = findNode(root, name, len);
    SceneUtil::setShadow(text);
    return text;
}
// text->setString(StringBuffer(TextData `id`, value))
void setTextIdNumber(uint32_t text, int32_t id, int32_t value) {
    Frame f;
    uint32_t td = textData(f, id);
    uint32_t buf = stringBuffer(f, {textDataStr(td), uint32_t(value)});
    setTextC(text, buf);
    sharedRelease(td);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self::Addr Self::create() {
    uint32_t p = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(p));
    if (vcall(p, V_init, {p}) == 1) {
        call(g::Ref_autorelease, {p});
        return p;
    }
    vcall(p, V_deleting, {p});
    return 0;
}
RW_FUNCTION(0x4FA568, "MagicStoneScrollView::create()", MagicStoneScrollView::create);

Self *Self::construct(Self *self) {
    uint32_t me = self->self();
    call(g::Layout_ctor, {me});
    initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    rw::set<uint32_t>(self, 0x648, 0);    // both std::functions are empty
    rw::set<uint32_t>(self, 0x660, 0);
    self->itemTouchEnabled = 0;
    self->field_66D = 0;
    self->field_670 = 0;
    self->field_674 = 0;
    call(g::aeabi_memset, {me + 0x678, 0x14, 0});   // field_678 and the product map
    self->productMaxLoad = 1.0f;
    return self;
}
RW_FUNCTION(0x4FA5BC, "MagicStoneScrollView::MagicStoneScrollView()", MagicStoneScrollView::construct);

Self *Self::destroy(Self *self) {
    uint32_t me = self->self();
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    if (self->itemTemplate) call(g::Ref_release, {self->itemTemplate});
    call(g::ProductMap_dtor, {me + OFF_PRODUCTS});
    functionDestroy(me + 0x650);
    functionDestroy(me + 0x638);
    self->vtable = VTABLE_INT;
    self->vtable2 = VTABLE2_INT;
    self->vtable3 = VTABLE3_INT;
    call(g::VectorBaseInt_dtor, {rw::guest_addr(&self->itemIDs)});
    IScrollProtocol::destroy(self);
    return self;
}
RW_FUNCTION(0x4FA3AC, "MagicStoneScrollView::~MagicStoneScrollView()", MagicStoneScrollView::destroy);

// 916 x 145 rows; the row template is the ItemBase card of ShopBuyItem_ItemBase_ver120.json moved into a Widget
uint32_t Self::init(Self *self) {
    self->itemWidth = 916.0f;
    self->itemHeight = 145.0f;
    IScrollProtocol::setEnabledScrollBar(self, 1);
    self->offsetX = float(self->itemWidth) * 0.5f;
    uint32_t panel = call(g::Widget_create, {});
    self->itemTemplate = panel;
    {
        Frame f;
        uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, rw::fbits(float(self->itemWidth)), rw::fbits(float(self->itemHeight))});
        vcall(panel, node::setContentSize, {panel, size});
    }
    vcall(self->itemTemplate, node::setAnchorPoint, {self->itemTemplate, VEC2_ANCHOR_MIDDLE});
    call(g::Ref_retain, {self->itemTemplate});
    uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_ItemBaseJson});
    uint32_t item = findNode(root, S_ItemBase, 8);
    uint32_t parent = vcall(item, node::getParent, {item});
    vcall(parent, node::removeChild, {parent, item, 1});
    vcall(self->itemTemplate, node::addChild, {self->itemTemplate, item});
    return BaseScrollView::initInt(self);
}
RW_FUNCTION(0x4FA730, "MagicStoneScrollView::init()", MagicStoneScrollView::init);

// ------------------------------------------------------------------------------------------------ rows
Self::Addr Self::createRow(Self *self, uint32_t index, int32_t arg) {
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= index) call(g::ThrowOutOfRange, {});
    uint32_t row = call(g::Widget_clone, {self->itemTemplate});
    uint32_t me = self->self();
    vcall(me, V_changeRow, {me, row, index, uint32_t(arg)});
    return row;
}
RW_FUNCTION(0x4FA8DC, "MagicStoneScrollView::createRow(int, int)", MagicStoneScrollView::createRow);

// Fill a card with the pack `index`: Jewels, bonus, image, store price, Buy button (enabled when the player may
// still buy Jewels), the sale balloon (a benefit not used yet) and the skip ticket balloon. Returns 1 when the
// widget has no ItemBase card.
uint32_t Self::changeRow(Self *self, Addr widget, uint32_t index, int32_t /*arg*/) {
    uint32_t me = self->self();
    uint32_t ids = self->itemIDs.begin;
    if (uint32_t(int32_t(self->itemIDs.end - ids) >> 2) <= index) call(g::ThrowOutOfRange, {});
    if (!findNode(widget, S_ItemBase, 8)) return 1;

    Frame f;
    uint32_t price = newStr(f, S_Empty, 0);
    uint32_t idAddr = ids + 4 * index;
    uint32_t shop = masterRow(f, g::Shop_create, ld32(idAddr));
    uint32_t row = ld32(shop);
    int32_t bonus = safe32(row, master::Shop::BONUS);
    int32_t number = safe32(row, master::Shop::NUMBER);
    // the store price of the product (stays empty when the store does not know it)
    {
        Frame t;
        uint32_t productId = safeBytes(t, row, master::Shop::PRODUCT_ID, 0x81);
        uint32_t key = newStr(t, productId);
        uint32_t node = call(g::ProductMap_find, {me + OFF_PRODUCTS, key});
        strFree(key);
        if (node) {
            uint32_t s = node + 0x20;   // ProductInfo's price string
            uint32_t text = strIsLong(s) ? ld32(s + 8) : s + 1;
            call(fn::string_assign, {price, text, host_strlen(text)});
        }
    }
    shadowed(widget, S_Price, 5);
    shadowed(widget, S_Gain, 4);
    uint32_t image = findNode(widget, S_ItemImage, 9);
    {
        Frame t;
        uint32_t path = stringBuffer(t, {S_ItemImagePath, uint32_t(safe32(row, master::Shop::THUMB_ID))});
        uint32_t s = newStr(t, path);
        call(fn::ImageView_loadTexture, {image, s, 0});
        strFree(s);
    }
    uint32_t name = findNode(widget, S_MagicStone, 10);
    {
        Frame t;
        setTextC(name, safeBytes(t, row, master::Shop::EXPLANATION, 0x81));
    }
    SceneUtil::setShadow(name);
    uint32_t count = findNode(widget, S_NumLabel, 9);
    setTextIdNumber(count, TXT_JEWEL_COUNT, number);
    SceneUtil::setShadow(count);
    uint32_t priceText = findNode(widget, S_Price, 5);
    {
        Frame t;
        uint32_t font = newStr(t, S_PriceFont, 12);
        call(g::Text_setFontName, {priceText, font});
        strFree(font);
    }
    call(g::Text_setFontSize, {priceText, 24});
    setTextC(priceText, strData(price));
    SceneUtil::setShadow(priceText);
    uint32_t balloon = findNode(widget, S_Balloon, 15);
    if (bonus < 1) {
        setVisible(balloon, 0);
    } else {
        setVisible(balloon, 1);
        uint32_t gain = findNode(balloon, S_Gain, 4);
        setTextIdNumber(gain, TXT_BONUS, bonus);
        SceneUtil::setShadow(gain);
    }
    uint32_t buyText = findNode(widget, S_TxtBuy, 7);
    setTextId(buyText, TXT_BUY);
    SceneUtil::setShadow(buyText);
    uint32_t buyImage = findNode(widget, S_BuyButton, 10);
    uint32_t button = findNode(widget, S_BuyImageButton, 15);
    if (!button) {
        Frame t;
        uint32_t file = newStr(t, S_But01, 5);
        button = call(g::ImageButton_create, {file, 0});
        strFree(file);
        call(g::ImageButton_setTargetImageView, {button, buyImage});
        uint32_t s = newStr(t, S_BuyImageButton, 15);
        vcall(buyImage, node::addChildZName, {buyImage, button, 0, s});
        strFree(s);
    }
    {
        Frame t;
        uint32_t cb = newFunction(t, VT_BuyTouch, {me, ld32(idAddr)});
        call(g::ImageButton_addTouchEventListener, {button, cb});
        functionDestroy(cb);
    }
    st32(button + IMAGEBUTTON_DELEGATE, me);

    // sale balloon: the pack's benefit, unless already used (or the Apple status hides it)
    uint32_t sale = findNode(widget, S_SaleBalloon, 20);
    uint32_t saleGain = shadowed(sale, S_Gain, 4);
    {
        Frame t;
        setTextC(saleGain, safeBytes(t, row, master::Shop::BENEFIT_EXPLANATION, 0x81));
    }
    int32_t benefit = safe32(row, master::Shop::BENEFIT_ID);
    bool offered = true;
    uint32_t ret = ld32(call(fn::APIManager_getInstance, {}) + API_RET);
    if (!ret || !(ld8(ret + RET_FUNCTION_FLAGS) & FLAG_IGNORE_APPLE_STATUS))
        offered = safe32(row, master::Shop::APPLE_STATUS) == 0;
    uint32_t showSale = 0;
    if (benefit && offered) {
        uint32_t used = ld32(call(fn::APIManager_getInstance, {}) + API_USED_BENEFITS);
        uint32_t b = ld32(used), e = ld32(used + 4);
        showSale = 1;
        for (uint32_t p = b; p != e; p += 12)
            if (int32_t(ld32(p)) == benefit) { showSale = 0; break; }
    }
    setVisible(sale, showSale);

    uint32_t skip = findNode(widget, S_SkipBalloon, 18);
    if (safe32(row, master::Shop::STAGE_SKIP_TICKET) < 1 || vcall(sale, node::isVisible, {sale})) {
        setVisible(skip, 0);
    } else {
        setVisible(skip, 1);
        uint32_t gain = shadowed(skip, S_Gain, 4);
        setTextIdNumber(gain, TXT_SKIP_TICKETS, safe32(row, master::Shop::STAGE_SKIP_TICKET));
    }

    uint32_t limit;
    {
        Frame t;
        uint32_t misc = masterRow(t, fn::Misc_create, MISC_PURCHASE_LIMIT);
        limit = uint32_t(safe32(ld32(misc), 4));
        sharedRelease(misc);
    }
    SceneUtil::setButtonEnabled(button, buyText, SceneUtil::hasMagicStone(int32_t(limit)) ^ 1, 0);
    sharedRelease(shop);
    strFree(price);
    return 0;
}
RW_FUNCTION(0x4FA934, "MagicStoneScrollView::changeRow(Widget*, int, int)", MagicStoneScrollView::changeRow);

// ------------------------------------------------------------------------------------------------ touch
void Self::assignTouchEnded(Addr function, Addr captures) {
    Frame f;
    uint32_t c0 = ld32(captures), c1 = ld32(captures + 4), c2 = ld32(captures + 8), c3 = ld32(captures + 12);
    uint32_t fo = f.alloc(24);
    st32(fo + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x14});
    if (heap) {
        st32(heap, VT_TouchEnded);
        st32(heap + 4, c0);
        st32(heap + 8, c1);
        st32(heap + 12, c2);
        st32(heap + 16, c3);
    }
    st32(fo + 16, heap);
    call(g::FunctionTouchVoid_swap, {fo, function});
    functionDestroy(fo);
}
RW_FUNCTION(0x4FB8A0, "MagicStoneScrollView::registItemTouchEvent: function<void(Touch*,Event*)> = $_2",
            MagicStoneScrollView::assignTouchEnded);

void Self::registItemTouchEvent(Self *self, Addr /*widget*/, int32_t index) {
    if (!self->itemTouchEnabled) return;
    uint32_t me = self->self();
    uint32_t idx = uint32_t(index);
    Frame f;
    uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    {
        // $_0 {this, index} -> onTouchBegan
        Frame t;
        uint32_t fo = newFunction(t, VT_TouchBegan, {me, idx});
        call(g::FunctionTouchBool_swap, {fo, listener + 0x50});
        functionDestroy(fo);
    }
    uint32_t caps = f.alloc(16);
    st32(caps, me);
    st32(caps + 4, idx);
    st32(caps + 8, me);
    st32(caps + 12, idx);
    {
        // $_1 {this, index, $_2} -> onTouchMoved
        Frame t;
        uint32_t fo = t.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x1C});
        if (heap) {
            st32(heap, VT_TouchMoved);
            for (uint32_t k = 0; k < 3; k++) {
                st32(heap + 4 + 8 * k, me);
                st32(heap + 8 + 8 * k, idx);
            }
        }
        st32(fo + 16, heap);
        call(g::FunctionTouchVoid_swap, {fo, listener + 0x68});
        functionDestroy(fo);
    }
    // $_2 -> onTouchEnded and onTouchCancelled
    assignTouchEnded(listener + 0x80, caps);
    assignTouchEnded(listener + 0x98, caps);
    call(g::EventDispatcher_addListenerWithSceneGraphPriority, {dispatcher, listener, me});
}
RW_FUNCTION(0x4FB758, "MagicStoneScrollView::registItemTouchEvent(Widget*, int)", MagicStoneScrollView::registItemTouchEvent);

void Self::setProductInfoMap(Self *self, Addr products) {
    uint32_t map = self->self() + OFF_PRODUCTS;
    call(g::ProductMap_clear, {map});
    if (!products) return;
    Frame f;
    uint32_t result = f.alloc(8);
    for (uint32_t node = ld32(products + 8); node; node = ld32(node))
        call(g::ProductMap_insert, {result, map, node + 8});
}
RW_FUNCTION(0x4FB934, "MagicStoneScrollView::setProductInfoMap(const unordered_map<string, ProductInfo>*)",
            MagicStoneScrollView::setProductInfoMap);
