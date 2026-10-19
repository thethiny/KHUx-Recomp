// PresentBoxScrollView: the Presents list (see PresentBoxScrollView.hpp). popUpGetPresent / popUpAllGetPresent /
// removeItem and the row lambdas' bodies are not rewritten.
#include "PresentBoxScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = PresentBoxScrollView;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_ctor = 0x8DF027;                    // Size(float, float)
constexpr uint32_t VectorAt_throw = 0x47CCE1;               // vector::at's out_of_range (does not return)
constexpr uint32_t PresentMap_insertHint = 0x5105FD;        // __tree<ull, Present>::__insert_unique(hint, const pair&)
constexpr uint32_t PresentMap_at = 0x50DC75;                // map<ull, Present>::operator[](const ull&)
constexpr uint32_t PresentMap_destroy = 0x4B5381;           // __tree<ull, Present>::destroy(node)
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;   // (string& name, string& image, type, id, bool)
constexpr uint32_t ImageView_loadTexture = 0x839731;        // (image, const string&, TextureResType)
constexpr uint32_t Date_makeLocalDate = 0x4927CD;           // (Date out, const Date*)
constexpr uint32_t TextData_format = 0x492929;              // (string out, Type, id, ...)
constexpr uint32_t ImageButton_create = 0x531239;           // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;   // (button, std::function by value)
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t TouchBoolFunction_swap = 0x483E4D;
constexpr uint32_t TouchVoidFunction_swap = 0x4F3699;
constexpr uint32_t TouchVoidFunction_assignPair = 0x50DE49; // sub_50DE48: listener fn = lambda {this, row, this, row}
}  // namespace g

constexpr uint32_t S_PanelJson = 0xDFD60F;        // "cocostudio/publish/PresentBOX_Panel_ver120.json"
constexpr uint32_t S_Gift = 0xDFD63F;
constexpr uint32_t S_TxtGiftNameLabel = 0xDFD644;
constexpr uint32_t S_PercentSS = 0xDF8BBA;        // "%s%s"
constexpr uint32_t S_TxtGiftNameNumLabel = 0xDFD658;
constexpr uint32_t S_TimesD = 0xDFD670;           // "×%d"
constexpr uint32_t S_TxtReception = 0xDFD675;
constexpr uint32_t S_TxtReceptionDateLabel = 0xDFD683;
constexpr uint32_t S_TxtStorage = 0xDFD69C;
constexpr uint32_t S_TxtStorageDateLabel = 0xDFD6A8;
constexpr uint32_t S_TxtGiftDetailLabel = 0xDFD6BF;
constexpr uint32_t S_Button = 0xDFD6D5;
constexpr uint32_t S_ImageButton = 0xDFD6DC;
constexpr uint32_t S_But02 = 0xDFD6E8;
constexpr uint32_t S_TxtReceive = 0xDFD6EE;

constexpr int32_t TXT_COUNT = 103500014;          // "x%d" after the name
constexpr int32_t TXT_RECEPTION = 103500010;
constexpr int32_t TXT_STORAGE = 103500011;
constexpr int32_t TXT_RECEIVE = 103500004;
constexpr int32_t TXT_DATE = 103501005;           // year, month, day, hour, minute

constexpr uint32_t ITEM_KEYBLADE = 13;            // shown at 0.3 scale

constexpr uint32_t APIMANAGER_PRESENTS = 0x104;   // map<unsigned long long, UserPresent::Present>*
// UserPresent::Present: 7 words (+8 item type, +0xC item id, +0x10 count, +0x14 storage limit Date, +0x1B flags
// (bit 1: has a storage limit)), +0x1C detail text (std::string), +0x28 received Date (9 bytes)
constexpr uint32_t PRESENT_TYPE = 0x08, PRESENT_ITEM = 0x0C, PRESENT_COUNT = 0x10, PRESENT_LIMIT = 0x14,
                   PRESENT_FLAGS = 0x1B, PRESENT_TEXT = 0x1C, PRESENT_RECEIVED = 0x28;
constexpr uint32_t PRESENT_SIZE = 0x34;

// __func vtables (+8)
constexpr uint32_t VT_Receive = 0xF56444;         // changeRow's lambda (0x50 bytes)
constexpr uint32_t VT_TouchBegan = 0xF56418;      // registItemTouchEvent $_0 {this, row}
constexpr uint32_t VT_TouchMoved = 0xF563EC;      // $_1 {this, row} x3, on the heap

constexpr uint32_t V_INIT = 0x248;
constexpr uint32_t V_CHANGE_ROW = 0x340;
constexpr uint32_t V_SET_SCALE_XY = 0x48;

// std::string copy constructor, inline
void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        strInit(dst, ld32(src + 8), ld32(src + 4));
    } else {
        for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
    }
}

void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// text->setString(TextData::format(TXT_DATE, local date of `date`))
void setDateText(uint32_t text, uint32_t date) {
    Frame f;
    const uint32_t local = f.alloc(12);
    call(g::Date_makeLocalDate, {local, date});
    const uint32_t ymd = ld32(local);
    const uint32_t hm = rw::load<uint16_t>(local + 4);
    const uint32_t formatted = f.alloc(12);
    call(g::TextData_format, {formatted, 0, uint32_t(TXT_DATE), ymd & 0xFFFF, (ymd >> 16) & 0xFF, ymd >> 24,
                              hm & 0xFF, hm >> 8});
    setTextC(text, strData(formatted));
    strFree(formatted);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self::Addr Self::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(me));
    if (vcall(me, V_INIT, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}
RW_FUNCTION(0x50CC14, "PresentBoxScrollView::create", PresentBoxScrollView::create);

Self *Self::construct(Self *self) {
    const uint32_t me = self->self();
    call(g::Layout_ctor, {me});
    initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    for (uint32_t fnAt : {0x638u, 0x650u, 0x668u, 0x680u, 0x698u}) st32(me + fnAt + 0x10, 0);
    self->touchEnabled = 0;
    self->field_6B5 = 0;
    self->field_6B8 = 0;
    self->field_6BC = 0;
    self->field_6C0 = 0;
    return self;
}
RW_FUNCTION(0x50CC68, "PresentBoxScrollView::PresentBoxScrollView", PresentBoxScrollView::construct);

Self *Self::destroy(Self *self) {
    const uint32_t me = self->self();
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    if (self->panel) vcall(self->panel, 0x04, {self->panel});
    self->panel = 0;
    for (uint32_t fnAt : {0x698u, 0x680u, 0x668u, 0x650u, 0x638u}) functionDestroy(me + fnAt);
    destroyUll(self);
    return self;
}
RW_FUNCTION(0x50CA88, "PresentBoxScrollView::~PresentBoxScrollView", PresentBoxScrollView::destroy);

// 925 x 148 rows from PresentBOX_Panel (centred horizontally), with a scroll bar.
uint32_t Self::init(Self *self) {
    self->itemWidth = 925.0f;
    self->itemHeight = 148.0f;
    setEnabledScrollBar(self, 1);
    const uint32_t reader = call(fn::GUIReader_getInstance, {});
    self->panel = call(fn::GUIReader_widgetFromJsonFile, {reader, S_PanelJson});
    call(g::Ref_retain, {self->panel});
    vcall(self->panel, 0x29C, {self->panel, 0});                  // ui::Widget::setTouchEnabled(false)
    const uint32_t panel = self->panel;
    const uint32_t size = vcall(panel, node::getContentSize, {panel});
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(g::Vec2_ctor, {pos, fbits(ldf(size) * 0.5f), 0});
    vcall(panel, node::setPosition, {panel, pos});
    call(g::Vec2_dtor, {pos});
    return initUll(self);
}
RW_FUNCTION(0x50CDD8, "PresentBoxScrollView::init", PresentBoxScrollView::init);

// A row: an item-sized widget holding a clone of the panel.
Self::Addr Self::getLayoutPanel(Self *self) {
    const uint32_t row = call(g::Widget_create, {});
    const uint32_t panel = call(g::Widget_clone, {self->panel});
    vcall(row, node::setAnchorPoint, {row, VEC2_ANCHOR_MIDDLE});
    vcall(row, node::addChild, {row, panel});
    Frame f;
    const uint32_t size = f.alloc(8);
    call(g::Size_ctor, {size, fbits(float(self->itemWidth)), fbits(float(self->itemHeight))});
    vcall(row, node::setContentSize, {row, size});
    vcall(row, node::setPositionX, {row, fbits(self->itemWidth * 0.5f)});
    return row;
}
RW_FUNCTION(0x50CECC, "PresentBoxScrollView::getLayoutPanel", PresentBoxScrollView::getLayoutPanel);

Self::Addr Self::createRow(Self *self, int32_t index, int32_t arg) {
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 3) <= uint32_t(index))
        call(g::VectorAt_throw, {self->self() + 0x628});
    const uint32_t row = getLayoutPanel(self);
    vcall(self->self(), V_CHANGE_ROW, {self->self(), row, uint32_t(index), uint32_t(arg)});
    return row;
}
RW_FUNCTION(0x50CF94, "PresentBoxScrollView::createRow", PresentBoxScrollView::createRow);

// ------------------------------------------------------------------------------------------------ rows
// Fill `row` with present `index`: copied out of a copy of the user's present map, its image, name ("name x n"
// from two up), "x n", the received date, the storage limit (red) when it has one, the detail text, and the Receive
// button (made once: an ImageButton over "Button") whose lambda carries the present. 1 when the row has no gift
// image (nothing filled), else 0.
uint32_t Self::changeRow(Self *self, Addr row, int32_t index, int32_t) {
    const uint32_t me = self->self();
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 3) <= uint32_t(index))
        call(g::VectorAt_throw, {me + 0x628});
    const uint32_t idSlot = self->itemIDs.begin + 8 * uint32_t(index);
    Frame f;
    // a copy of the user's present map
    const uint32_t userMap = ld32(call(fn::APIManager_getInstance, {}) + APIMANAGER_PRESENTS);
    const uint32_t local = f.alloc(12);
    st32(local + 4, 0);
    st32(local + 8, 0);
    st32(local, local + 4);
    for (uint32_t node = ld32(userMap); node != userMap + 4;) {
        call(g::PresentMap_insertHint, {local, local + 4, node + 0x10});
        uint32_t r = ld32(node + 4);
        if (r) {
            node = r;
            while (ld32(node)) node = ld32(node);
        } else {
            uint32_t parent;
            while (ld32(parent = ld32(node + 8)) != node) node = parent;
            node = parent;
        }
    }
    // the present (copied)
    const uint32_t entry = call(g::PresentMap_at, {local, idSlot});
    const uint32_t present = f.alloc(PRESENT_SIZE);
    for (uint32_t k = 0; k < PRESENT_TEXT; k += 4) st32(present + k, ld32(entry + k));
    copyString(present + PRESENT_TEXT, entry + PRESENT_TEXT);
    for (uint32_t k = 0; k < 9; k++) st8(present + PRESENT_RECEIVED + k, ld8(entry + PRESENT_RECEIVED + k));
    const uint32_t name = f.alloc(12), image = f.alloc(12);
    for (uint32_t k = 0; k < 12; k += 4) {
        st32(name + k, 0);
        st32(image + k, 0);
    }
    call(g::SceneUtil_setItemImageData, {name, image, ld32(present + PRESENT_TYPE), ld32(present + PRESENT_ITEM), 0});
    const int32_t count = int32_t(ld32(present + PRESENT_COUNT));
    uint32_t result = 0;
    const uint32_t gift = findNode(row, S_Gift, 4);
    if (!gift) {
        result = 1;
    } else {
        call(g::ImageView_loadTexture, {gift, image, 0});
        const float scale = ld32(present + PRESENT_TYPE) == ITEM_KEYBLADE ? 0.3f : 1.0f;
        vcall(gift, V_SET_SCALE_XY, {gift, fbits(scale), fbits(scale)});
        // name
        const uint32_t nameLabel = findNode(row, S_TxtGiftNameLabel, 19);
        SceneUtil::setShadow(nameLabel);
        if (count < 2) {
            call(fn::Text_setString, {nameLabel, name});
        } else {
            Frame h;
            const uint32_t td = textData(h, TXT_COUNT);
            const uint32_t times = stringBuffer(h, {textDataStr(td), uint32_t(count)});
            const uint32_t text = stringBuffer(h, {S_PercentSS, strData(name), times});
            setTextC(nameLabel, text);
            sharedRelease(td);
        }
        const uint32_t numLabel = findNode(row, S_TxtGiftNameNumLabel, 23);
        SceneUtil::setShadow(numLabel);
        {
            Frame h;
            setTextC(numLabel, stringBuffer(h, {S_TimesD, uint32_t(count)}));
        }
        // received
        const uint32_t reception = findNode(row, S_TxtReception, 13);
        const uint32_t receptionDate = findNode(row, S_TxtReceptionDateLabel, 24);
        SceneUtil::setShadow(reception);
        SceneUtil::setShadow(receptionDate);
        setTextId(reception, TXT_RECEPTION);
        setDateText(receptionDate, present + PRESENT_RECEIVED);
        // storage limit
        const uint32_t storage = findNode(row, S_TxtStorage, 11);
        const uint32_t storageDate = findNode(row, S_TxtStorageDateLabel, 22);
        if (ld8(present + PRESENT_FLAGS) & 2) {
            SceneUtil::setShadow(storage);
            SceneUtil::setShadow(storageDate);
            setTextId(storage, TXT_STORAGE);
            setDateText(storageDate, present + PRESENT_LIMIT);
            for (uint32_t text : {storage, storageDate}) {
                Frame h;
                const uint32_t color = h.alloc(4);
                call(fn::Color3B_ctor, {color, 0xFF, 0x5E, 0x5E});
                vcall(text, node::setColor, {text, color});
            }
            setVisible(storage, 1);
            setVisible(storageDate, 1);
        } else {
            setVisible(storage, 0);
            setVisible(storageDate, 0);
        }
        const uint32_t detail = findNode(row, S_TxtGiftDetailLabel, 21);
        SceneUtil::setShadow(detail);
        call(fn::Text_setString, {detail, present + PRESENT_TEXT});
        // the Receive button
        const uint32_t buttonImage = findNode(row, S_Button, 6);
        uint32_t button = findNode(row, S_ImageButton, 11);
        if (!button) {
            {
                Frame h;
                const uint32_t file = newStr(h, S_But02, 5);
                button = call(g::ImageButton_create, {file, 0});
                strFree(file);
            }
            call(g::ImageButton_setTargetImageView, {button, buttonImage});
            {
                Frame h;
                const uint32_t tag = newStr(h, S_ImageButton, 11);
                vcall(buttonImage, node::addChildZName, {buttonImage, button, 0, tag});
                strFree(tag);
            }
            const uint32_t receive = findNode(row, S_TxtReceive, 11);
            setTextId(receive, TXT_RECEIVE);
            SceneUtil::setShadow(receive);
        }
        // the lambda {row, this, present words, text, received date, id}: built on the stack, copied to the heap
        const uint32_t lambda = f.alloc(0x48);
        st32(lambda, row);
        st32(lambda + 4, me);
        for (uint32_t k = 0; k < PRESENT_TEXT; k += 4) st32(lambda + 8 + k, ld32(present + k));
        copyString(lambda + 0x24, present + PRESENT_TEXT);
        for (uint32_t k = 0; k < 9; k++) st8(lambda + 0x30 + k, ld8(present + PRESENT_RECEIVED + k));
        st32(lambda + 0x40, ld32(idSlot));
        st32(lambda + 0x44, ld32(idSlot + 4));
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x50});
        if (heap) {
            st32(heap, VT_Receive);
            st32(heap + 8, row);
            st32(heap + 0xC, me);
            for (uint32_t k = 0; k < PRESENT_TEXT; k += 4) st32(heap + 0x10 + k, ld32(lambda + 8 + k));
            copyString(heap + 0x2C, lambda + 0x24);
            for (uint32_t k = 0; k < 9; k++) st8(heap + 0x38 + k, ld8(lambda + 0x30 + k));
            st32(heap + 0x48, ld32(lambda + 0x40));
            st32(heap + 0x4C, ld32(lambda + 0x44));
        }
        st32(fo + 16, heap);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        strFree(lambda + 0x24);
    }
    strFree(name);
    strFree(image);
    strFree(present + PRESENT_TEXT);
    call(g::PresentMap_destroy, {local, ld32(local + 4)});
    return result;
}
RW_FUNCTION(0x50CFEC, "PresentBoxScrollView::changeRow", PresentBoxScrollView::changeRow);

// changeRow's lambda __func::__clone(): a heap copy (the text copied, the rest word for word).
Self::Addr Self::cloneReceiveLambda(Addr functor) {
    const uint32_t p = call(fn::operator_new, {0x50});
    if (!p) return 0;
    st32(p, VT_Receive);
    st32(p + 0xC, ld32(functor + 0xC));
    st32(p + 8, ld32(functor + 8));
    for (uint32_t k = 0x10; k < 0x2C; k += 4) st32(p + k, ld32(functor + k));
    copyString(p + 0x2C, functor + 0x2C);
    for (uint32_t k = 0; k < 9; k++) st8(p + 0x38 + k, ld8(functor + 0x38 + k));
    st32(p + 0x48, ld32(functor + 0x48));
    st32(p + 0x4C, ld32(functor + 0x4C));
    return p;
}
RW_FUNCTION(0x50FED8, "PresentBoxScrollView::changeRow $_0 __func::__clone()", PresentBoxScrollView::cloneReceiveLambda);

// Touch listeners for a row (only while touchEnabled): began $_0, moved $_1, ended / cancelled through sub_50DE48.
void Self::registItemTouchEvent(Self *self, Addr row, int32_t index) {
    (void)row;
    if (!self->touchEnabled) return;
    const uint32_t me = self->self();
    const uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    {
        const uint32_t cb = newFunction(f, VT_TouchBegan, {me, uint32_t(index)});
        call(g::TouchBoolFunction_swap, {cb, listener + 0x50});
        functionDestroy(cb);
    }
    const uint32_t pair = f.alloc(16);
    st32(pair, me);
    st32(pair + 4, uint32_t(index));
    st32(pair + 8, me);
    st32(pair + 12, uint32_t(index));
    {
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x1C});
        if (heap) {
            st32(heap, VT_TouchMoved);
            for (uint32_t k = 4; k < 0x1C; k += 8) {
                st32(heap + k, me);
                st32(heap + k + 4, uint32_t(index));
            }
        }
        st32(fo + 16, heap);
        call(g::TouchVoidFunction_swap, {fo, listener + 0x68});
        functionDestroy(fo);
    }
    call(g::TouchVoidFunction_assignPair, {listener + 0x80, pair});
    call(g::TouchVoidFunction_assignPair, {listener + 0x98, pair});
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, me});
}
RW_FUNCTION(0x50DD00, "PresentBoxScrollView::registItemTouchEvent", PresentBoxScrollView::registItemTouchEvent);
