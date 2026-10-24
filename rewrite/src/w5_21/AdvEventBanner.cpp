// Quests > Events: AdvEventBannerScrollView, the list of running events (a BaseScrollView<int> over indexes into
// APIManager+0x12C, the 56-byte event records of the 101 reply). A cell (AdventureTop2_EV_Banner) shows the event's
// banner (downloaded once per index, the textures kept in a map<int, Texture2D*>), the lock mask / icon of a
// locked event, the "n days left" plate inside 100 days (updated every second) or the unlock plate, and an
// ImageButton whose lambda opens the event (locked: the "locked" callback when the player may not enter). Rows
// take touches through began / moved / ended lambdas; a tap plays the tap animation and selects the item.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff, Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE, ImageView_loadTextureTex = rwlib::cocos2d::ui::ImageView::loadTexture__PNS_9Texture2DE;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction, Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace, Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Image_ctor = rwlib::cocos2d::Image::ctor_base, Image_initWithImageData = rwlib::cocos2d::Image::initWithImageData;
constexpr uint32_t Texture2D_ctor = rwlib::cocos2d::Texture2D::ctor_base, Texture2D_initWithImage = rwlib::cocos2d::Texture2D::initWithImage;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addSceneGraph = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t FunctionBTE_swap = 0x483E4D, FunctionVTE_swap = 0x4F3699;
constexpr uint32_t FunctionVV_call = 0x470E69, FunctionVI_call = 0x4F321D;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1, StringInit = 0x47CD93;
constexpr uint32_t VectorInt_copy = 0x4AF707, VectorInt_dtor = 0x478961, VectorInt_pushSlow = 0x4D6CC1;
constexpr uint32_t VectorInt_assign = 0x47D5BD, SortInt = 0x4EC65D;
constexpr uint32_t VectorStory_copy = 0x4B5BC9, VectorStory_dtor = 0x4B58EF;
constexpr uint32_t TextureMap_findEqualKey = 0x588C03, TextureMap_insertNodeAt = 0x588C4F, TextureMap_destroy = 0x588C7F;
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1, IScrollProtocol_update = 0x4F5CFD;
constexpr uint32_t IScrollProtocol_getItemPixel = 0x4F6751, IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t IScrollProtocol_setScrollType = 0x4F70B5, BaseScrollViewInt_init = 0x4FA8A1;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5, createTimeLeftString = 0x492B75;
constexpr uint32_t DownloadManager_getInstance = 0x4DECB1, DownloadManager_start = 0x4DED65;
constexpr uint32_t aeabi_fixsfsi = 0xCD7240, aeabi_idiv = 0xCD5E8D;
// this class
constexpr uint32_t ctor = 0x586825, loadEventBanner = 0x586A21, textureAt = 0x586C05, setEventBanner = 0x586C85;
constexpr uint32_t updateExpirationDate = 0x587395, setEventBannerTime = 0x58745D, getLayoutPanel = 0x587969;
constexpr uint32_t updateSelectItems = 0x587B71, assignTouchEnded = 0x587E39, setSelectItem = 0x587ED9;
constexpr uint32_t touchEndedBody = 0x588191;
}  // namespace g

constexpr uint32_t GOT_VT = 0xFD66C0, GOT_VT_BASE_INT = 0xFD6598, GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t GOT_ROW_SPACING = 0xFD659C, GOT_PLACEHOLDER = 0xFD66C4;   // &float, &const char* (banner path)
constexpr uint32_t SIZE = 0x6A0, VT2_OFFSET = 0x360, VT3_OFFSET = 0x374;
// IScrollProtocol
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518, F_ROWS = 0x51C, F_ROW_COUNT = 0x528, F_COLUMNS = 0x538;
constexpr uint32_t F_SCROLL_Y = 0x540, F_FIRST = 0x544;
constexpr uint32_t F_IDS = 0x628;                         // BaseScrollView<int>: event indexes
// AdvEventBannerScrollView
constexpr uint32_t F_ON_SELECT = 0x638;                   // std::function<void(int)>
constexpr uint32_t F_ON_LOCKED = 0x650;                   // std::function<void()>
constexpr uint32_t F_PANEL = 0x668, F_TAP = 0x66C, F_PRESSED = 0x66D, F_PRESS_INDEX = 0x670, F_TAPPED = 0x674;
constexpr uint32_t F_PRESS_Y = 0x678, F_TIMER = 0x67C, F_HAS_LIMIT = 0x680, F_SELECTED = 0x684;
constexpr uint32_t F_TEXTURES = 0x690;                    // map<int, Texture2D*>: begin, root (end node), size
// APIManager: vector of 56-byte event records {vector<Story>, int, byte, string url, 16 bytes (dates: end at
// +0x28), int, byte state (+0x34: 1 locked, 2 ...)}
constexpr uint32_t API_EVENTS = 0x12C, API_FLAGS_OBJ = 0x44;
constexpr uint32_t EV_SIZE = 56, EV_URL = 0x14, EV_END = 0x28, EV_STATE = 0x34;
constexpr int32_t LIMIT_SECONDS = 8639999;               // < 100 days: show the time left
// lambdas
constexpr uint32_t VT_Load = 0xF5A48C;                    // loadEventBanner $_0 {this, index, image}
constexpr uint32_t VT_Open = 0xF5A460;                    // setEventBanner $_1 {widget, record copy, this, index}
constexpr uint32_t VT_Began = 0xF5A434, VT_Moved = 0xF5A408, VT_Ended = 0xF5A3B0, VT_TapDone = 0xF5A3DC;
constexpr uint32_t V_deleting = 0x04, V_setLocalZOrder = 0x0C, V_isVisible = 0xB4, V_init = 0x248;
constexpr uint32_t V_changeRow = 0x340, V_setTouchEnabled = 0x29C;

constexpr uint32_t S_Json = 0xDFFE83, S_Image3 = 0xDFFE5D, S_Mask = 0xDF99A1, S_IconLock = 0xDFDD93;
constexpr uint32_t S_ImageButtonCheck = 0xDFDC5C, S_Empty = 0xDF8A1A, S_Plate = 0xDFFE65, S_PlateUnlock = 0xDFFE6B;
constexpr uint32_t S_TxtUnlock = 0xDFFE78, S_TxtLimit = 0xDF9EED, S_PanelEff = 0xDFE94F;
constexpr int32_t TXT_UNLOCK = 102900040;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline uint32_t idCount(uint32_t s) { return (ld32(s + F_IDS + 4) - ld32(s + F_IDS)) >> 2; }
// the event records and their count
uint32_t events(uint32_t &count) {
    const uint32_t vec = ld32(call(fn::APIManager_getInstance, {}) + API_EVENTS);
    count = ((ld32(vec + 4) - ld32(vec)) >> 3) * 0xB6DB6DB7u;
    return vec;
}
// in-order walk of the texture map, releasing every texture; then the tree is emptied
void releaseTextures(uint32_t s) {
    const uint32_t end = s + F_TEXTURES + 4;
    uint32_t n = ld32(s + F_TEXTURES);
    while (n != end) {
        if (ld32(n + 0x14)) call(g::Ref_release, {ld32(n + 0x14)});
        if (ld32(n + 4)) {
            n = ld32(n + 4);
            while (ld32(n)) n = ld32(n);
        } else {
            uint32_t c = n;
            for (;;) {
                n = ld32(c + 8);
                if (ld32(n) == c) break;
                c = n;
            }
        }
    }
    call(g::TextureMap_destroy, {s + F_TEXTURES, ld32(end)});
    st32(s + F_TEXTURES + 8, 0);
    st32(s + F_TEXTURES, end);
    st32(end, 0);
}
// the lock mask / icon: visible when the event is locked
void lockMarks(uint32_t widget, uint32_t record) {
    const uint32_t mask = find(widget, S_Mask, 4);
    const uint32_t lock = find(widget, S_IconLock, 9);
    vcall(mask, node::setVisible, {mask, ld8(record + EV_STATE) == 1 ? 1u : 0u});
    vcall(lock, node::setVisible, {lock, ld8(record + EV_STATE) == 1 ? 1u : 0u});
}
// the time-left plate inside 100 days, else the unlock plate (state 2)
void timePlates(uint32_t s, uint32_t widget, uint32_t record, uint32_t plate, uint32_t unlock) {
    visible(plate, 0);
    visible(unlock, 0);
    Frame f;
    const uint32_t date = f.alloc(8);
    st32(date, ld32(record + EV_END));
    st32(date + 4, ld32(record + EV_END + 4));
    const int32_t secs = int32_t(call(g::Date_calculateIntervalSinceNow, {date}));
    if (secs <= LIMIT_SECONDS) {
        st8(s + F_HAS_LIMIT, 1);
        visible(plate, 1);
        call(g::updateExpirationDate, {s, find(plate, S_TxtLimit, 9), uint32_t(secs)});
    } else {
        visible(unlock, ld8(record + EV_STATE) == 2 ? 1u : 0u);
    }
    (void)widget;
}
// a short / long std::string copy (the libc++ copy constructor)
void stringCopy(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        call(g::StringInit, {dst, ld32(src + 8), ld32(src + 4)});
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}
// $_1's captures from `src` (a lambda object or the record-shaped local) into `dst`: widget, the record (vector
// and string deep-copied), this, index
void copyOpenLambda(uint32_t dst, uint32_t src) {
    st32(dst + 4, ld32(src + 4));
    call(g::VectorStory_copy, {dst + 8, src + 8});
    st8(dst + 0x18, ld8(src + 0x18));
    st32(dst + 0x14, ld32(src + 0x14));
    stringCopy(dst + 0x1C, src + 0x1C);
    for (uint32_t k = 0x28; k < 0x38; k += 4) st32(dst + k, ld32(src + k));
    st8(dst + 0x3C, ld8(src + 0x3C));
    st32(dst + 0x38, ld32(src + 0x38));
    st32(dst + 0x40, ld32(src + 0x40));
    st32(dst + 0x44, ld32(src + 0x44));
}
}  // namespace

struct AdvEventBanner {
    static uint32_t destruct(uint32_t s);
    static uint32_t create();
    static uint32_t construct(uint32_t s);
    static void clearLoadEventBanner(uint32_t s);
    static uint32_t loadEventBanner(uint32_t s, uint32_t image, uint32_t index, uint32_t url);
    static uint32_t textureAt(uint32_t map, uint32_t key);
    static void setEventBanner(uint32_t s, uint32_t widget, uint32_t index);
    static void updateExpirationDate(uint32_t s, uint32_t text, uint32_t secs);
    static void setEventBannerTime(uint32_t s, uint32_t widget, uint32_t index);
    static uint32_t init(uint32_t s);
    static void update(uint32_t s, float dt);
    static uint32_t getLayoutPanel(uint32_t s, uint32_t);
    static uint32_t createRow(uint32_t s, uint32_t index, uint32_t column);
    static uint32_t changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t);
    static void updateSelectItems(uint32_t s);
    static void registItemTouchEvent(uint32_t s, uint32_t, uint32_t index);
    static void assignTouchEnded(uint32_t dst, uint32_t caps);
    static void setSelectItem(uint32_t s, uint32_t id);
    static uint32_t searchSelect(uint32_t s, uint32_t id);
    static void touchEndedBody(uint32_t caps, uint32_t touch);
    static void tapDone(uint32_t fo);
    static void touchMoved(uint32_t fo, uint32_t touchRef);
    static uint32_t touchBegan(uint32_t fo, uint32_t touchRef);
    static uint32_t cloneOpen(uint32_t fo);
    static void cloneOpenTo(uint32_t fo, uint32_t dst);
    static void open(uint32_t fo, uint32_t, uint32_t typeRef);
    static uint32_t loaded(uint32_t fo, uint32_t dataRef);
};

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t AdvEventBanner::destruct(uint32_t s) {
    const uint32_t vt = ld32(GOT_VT);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    if (ld32(s + F_PANEL)) vcall(ld32(s + F_PANEL), V_deleting, {ld32(s + F_PANEL)});
    st32(s + F_PANEL, 0);
    releaseTextures(s);
    call(g::TextureMap_destroy, {s + F_TEXTURES, 0});
    call(g::VectorInt_dtor, {s + F_SELECTED});
    functionDestroy(s + F_ON_LOCKED);
    functionDestroy(s + F_ON_SELECT);
    const uint32_t bvt = ld32(GOT_VT_BASE_INT);
    st32(s, bvt + 8);
    st32(s + 0x280, bvt + VT2_OFFSET);
    st32(s + 0x350, bvt + VT3_OFFSET);
    call(g::VectorInt_dtor, {s + F_IDS});
    call(g::IScrollProtocol_dtor, {s});
    return s;
}

uint32_t AdvEventBanner::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::ctor, {me});
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t AdvEventBanner::construct(uint32_t s) {
    auto *base = rw::host_ptr<BaseScrollView>(s);
    call(g::Layout_ctor, {s});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    const uint32_t vt = ld32(GOT_VT);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    st32(s + F_ON_SELECT + 0x10, 0);
    st32(s + F_ON_LOCKED + 0x10, 0);
    st8(s + F_TAP, 0);
    st8(s + F_PRESSED, 0);
    st32(s + F_PRESS_INDEX, 0);
    st8(s + F_TAPPED, 0);
    st32(s + F_PRESS_Y, 0);
    st32(s + F_TIMER, 0);
    st8(s + F_HAS_LIMIT, 0);
    st32(s + F_SELECTED, 0);
    st32(s + F_SELECTED + 4, 0);
    st32(s + F_SELECTED + 8, 0);
    st32(s + F_TEXTURES + 4, 0);
    st32(s + F_TEXTURES + 8, 0);
    st32(s + F_TEXTURES, s + F_TEXTURES + 4);
    return s;
}

void AdvEventBanner::clearLoadEventBanner(uint32_t s) { releaseTextures(s); }

// The banner of event `index`: the cached texture, else the placeholder and a download (once per index)
uint32_t AdvEventBanner::loadEventBanner(uint32_t s, uint32_t image, uint32_t index, uint32_t url) {
    const uint32_t end = s + F_TEXTURES + 4;
    uint32_t n = ld32(s + F_TEXTURES);
    while (n != end) {
        if (ld32(n + 0x10) == index) {
            if (ld32(n + 0x14)) {
                call(g::ImageView_loadTextureTex, {image, ld32(n + 0x14)});
            } else {
                Frame f;
                const uint32_t path = ld32(ld32(GOT_PLACEHOLDER));
                const uint32_t str = newStr(f, path, host_strlen(path));
                call(g::ImageView_loadTexture, {image, str, 0});
                strFree(str);
            }
            return 0;
        }
        if (ld32(n + 4)) {
            n = ld32(n + 4);
            while (ld32(n)) n = ld32(n);
        } else {
            uint32_t c = n;
            for (;;) {
                n = ld32(c + 8);
                if (ld32(n) == c) break;
                c = n;
            }
        }
    }
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, index);
    st32(call(g::textureAt, {s + F_TEXTURES, key}), 0);
    {
        const uint32_t path = ld32(ld32(GOT_PLACEHOLDER));
        const uint32_t str = newStr(f, path, host_strlen(path));
        call(g::ImageView_loadTexture, {image, str, 0});
        strFree(str);
    }
    const uint32_t dm = call(g::DownloadManager_getInstance, {});
    const uint32_t fo = newFunction(f, VT_Load, {s, index, image});
    call(g::DownloadManager_start, {dm, url, fo, 0, 0});
    functionDestroy(fo);
    return 1;
}

// std::map<int, Texture2D*>::operator[](const int&)
uint32_t AdvEventBanner::textureAt(uint32_t map, uint32_t key) {
    Frame f;
    const uint32_t parent = f.alloc(4);
    const uint32_t slot = call(g::TextureMap_findEqualKey, {map, parent, key});
    uint32_t node = ld32(slot);
    if (!node) {
        node = call(fn::operator_new, {0x18});
        st32(node + 0x10, ld32(key));
        st32(node + 0x14, 0);
        call(g::TextureMap_insertNodeAt, {map, ld32(parent), slot, node});
    }
    return node + 0x14;
}

// A cell for event `index`: lock marks, the open button and its lambda, the banner, the plates
void AdvEventBanner::setEventBanner(uint32_t s, uint32_t widget, uint32_t index) {
    uint32_t count;
    const uint32_t vec = events(count);
    if (int32_t(count) < int32_t(index)) return;
    const uint32_t begin = ld32(vec);
    if (count <= index) call(g::ThrowOutOfRange, {vec});
    const uint32_t record = begin + EV_SIZE * index;
    lockMarks(widget, record);
    const uint32_t image = find(widget, S_Image3, 7);
    uint32_t button = find(widget, S_ImageButtonCheck, 0x11);
    if (!button) {
        Frame f;
        const uint32_t empty = newStr(f, S_Empty, 0);
        button = call(g::ImageButton_create, {empty, 0});
        strFree(empty);
        call(g::ImageButton_setTargetView, {button, image, fbits(1.0f)});
        const uint32_t name = newStr(f, S_ImageButtonCheck, 0x11);
        vcall(image, node::addChildZName, {image, button, 0, name});
        strFree(name);
    }
    {
        // the lambda {widget, the record, this, index}, built on the stack, then moved to the heap __func
        Frame f;
        const uint32_t local = f.alloc(0x48);
        st32(local + 4, widget);
        call(g::VectorStory_copy, {local + 8, record});
        st8(local + 0x18, ld8(record + 0x10));
        st32(local + 0x14, ld32(record + 0xC));
        stringCopy(local + 0x1C, record + EV_URL);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(local + 0x28 + k, ld32(record + 0x20 + k));
        st8(local + 0x3C, ld8(record + EV_STATE));
        st32(local + 0x38, ld32(record + 0x30));
        st32(local + 0x40, s);
        st32(local + 0x44, index);
        const uint32_t fo = newEmptyFunction(f);
        const uint32_t heap = call(fn::operator_new, {0x48});
        if (heap) {
            st32(heap, VT_Open);
            copyOpenLambda(heap, local);
        }
        st32(fo + 16, heap);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        strFree(local + 0x1C);
        call(g::VectorStory_dtor, {local + 8});
    }
    {
        const uint32_t img = find(widget, S_Image3, 7);
        const uint32_t url = (ld8(record + EV_URL) & 1) ? ld32(record + EV_URL + 8) : record + EV_URL + 1;
        call(g::loadEventBanner, {s, img, index, url});
    }
    const uint32_t plate = find(widget, S_Plate, 5);
    const uint32_t unlock = find(widget, S_PlateUnlock, 0xC);
    {
        Frame f;
        const uint32_t name = newStr(f, S_TxtUnlock, 0xA);
        call(g::SceneUtil_settingTitleLabel, {unlock, name, uint32_t(TXT_UNLOCK)});
        strFree(name);
    }
    timePlates(s, widget, record, plate, unlock);
}

void AdvEventBanner::updateExpirationDate(uint32_t, uint32_t text, uint32_t secs) {
    Frame f;
    const uint32_t left = f.alloc(12);
    call(g::createTimeLeftString, {left, secs});
    setTextC(text, (ld8(left) & 1) ? ld32(left + 8) : left + 1);
    strFree(left);
}

// The per-second refresh of a cell's plates (and lock marks)
void AdvEventBanner::setEventBannerTime(uint32_t s, uint32_t widget, uint32_t index) {
    const uint32_t vec = ld32(call(fn::APIManager_getInstance, {}) + API_EVENTS);
    const uint32_t begin = ld32(vec);
    const int32_t bytes = int32_t(ld32(vec + 4) - begin);
    const uint32_t count = uint32_t(bytes >> 3) * 0xB6DB6DB7u;
    if (bytes <= 55) {
        visible(find(widget, S_Plate, 5), 0);
        return;
    }
    if (!(int32_t(count) > int32_t(index))) return;
    if (count <= index) call(g::ThrowOutOfRange, {vec});
    const uint32_t record = begin + EV_SIZE * index;
    lockMarks(widget, record);
    const uint32_t plate = find(widget, S_Plate, 5);
    const uint32_t unlock = find(widget, S_PlateUnlock, 0xC);
    timePlates(s, widget, record, plate, unlock);
}

// ------------------------------------------------------------------------------------------------ the list
// Cells 800x172 with the scroll bar; the cell template is AdventureTop2_EV_Banner, untouchable
uint32_t AdvEventBanner::init(uint32_t s) {
    st8(s + F_HAS_LIMIT, 0);
    stf(s + F_ITEM_W, 800.0f);
    stf(s + F_ITEM_H, 172.0f);
    call(g::IScrollProtocol_setEnabledScrollBar, {s, 1});
    call(g::IScrollProtocol_setScrollType, {s, 0});
    stf(s + 0x570, ldf(s + F_ITEM_W) * 0.5f);
    stf(s + 0x574, -15.0f);
    stf(s + 0x57C, 65.0f);
    st32(s + F_PANEL, call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json}));
    call(g::Ref_retain, {ld32(s + F_PANEL)});
    vcall(ld32(s + F_PANEL), V_setTouchEnabled, {ld32(s + F_PANEL), 0});
    return call(g::BaseScrollViewInt_init, {s});
}

// Every second while some cell shows a time limit: refresh the visible cells' plates
void AdvEventBanner::update(uint32_t s, float dt) {
    if (ld8(s + F_HAS_LIMIT)) {
        const float t = ldf(s + F_TIMER) + dt;
        stf(s + F_TIMER, t);
        if (t >= 1.0f) {
            stf(s + F_TIMER, t + -1.0f);
            call(g::IScrollProtocol_getItemPixel, {s});
            if (ldf(s + F_ROW_COUNT) > 0.0f) {
                const uint32_t first = ld32(s + F_FIRST);
                for (uint32_t i = 0;;) {
                    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * i);
                    const uint32_t shown = vcall(row, V_isVisible, {row});
                    const int32_t at = int32_t(first + i);
                    if (at >= 1 && shown == 1 && uint32_t(at - 1) < idCount(s))
                        call(g::setEventBannerTime, {s, row, ld32(ld32(s + F_IDS) + 4 * first - 4 + 4 * i)});
                    ++i;
                    if (!(float(int32_t(i)) < ldf(s + F_ROW_COUNT))) break;
                }
            }
        }
    }
    call(g::IScrollProtocol_update, {s, fbits(dt)});
}

uint32_t AdvEventBanner::getLayoutPanel(uint32_t s, uint32_t) {
    const uint32_t panel = call(g::Widget_create, {});
    const uint32_t cell = call(g::Widget_clone, {ld32(s + F_PANEL)});
    vcall(panel, node::setAnchorPoint, {panel, ld32(GOT_ANCHOR_MIDDLE)});
    vcall(panel, node::addChild, {panel, cell});
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, ld32(s + F_ITEM_W), ld32(s + F_ITEM_H)});
        vcall(panel, node::setContentSize, {panel, size});
    }
    vcall(panel, node::setPositionX, {panel, fbits(ldf(s + F_ITEM_W) * 0.5f)});
    return panel;
}

uint32_t AdvEventBanner::createRow(uint32_t s, uint32_t index, uint32_t column) {
    if (idCount(s) <= index) call(g::ThrowOutOfRange, {s + F_IDS});
    const uint32_t panel = call(g::getLayoutPanel, {s});
    vcall(s, V_changeRow, {s, panel, index, column});
    return panel;
}

uint32_t AdvEventBanner::changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
    const uint32_t ids = ld32(s + F_IDS);
    if (idCount(s) <= index) call(g::ThrowOutOfRange, {s + F_IDS});
    if (!find(widget, S_Image3, 7)) return 1;
    call(g::setEventBanner, {s, widget, ld32(ids + 4 * index)});
    call(g::updateSelectItems, {s});
    return 0;
}

// The selected cell's effect panel
void AdvEventBanner::updateSelectItems(uint32_t s) {
    if (ld32(s + F_IDS + 4) == ld32(s + F_IDS)) return;
    if (!(ldf(s + F_ROW_COUNT) > 0.0f)) return;
    for (uint32_t i = 0;;) {
        uint32_t on = 0;
        const uint32_t sel = ld32(s + F_SELECTED), selEnd = ld32(s + F_SELECTED + 4);
        if (selEnd != sel) {
            const uint32_t id = ld32(ld32(s + F_IDS) + 4 * (ld32(s + F_FIRST) + i) - 4);
            const uint32_t n = (selEnd - sel) >> 2;
            for (uint32_t k = 0; k < n;) {
                const uint32_t v = ld32(sel + 4 * k++);
                on = 1;
                if (v == id) break;
                on = 0;
            }
        }
        const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * i);
        const uint32_t eff = find(row, S_PanelEff, 9);
        if (eff) visible(eff, on);
        ++i;
        if (!(float(int32_t(i)) < ldf(s + F_ROW_COUNT))) break;
    }
}

// Touch listener for a cell: began / moved / ended / cancelled lambdas capturing (this, index), registered on the
// scroll view itself
void AdvEventBanner::registItemTouchEvent(uint32_t s, uint32_t, uint32_t index) {
    if (!ld8(s + F_TAP)) return;
    const uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    {
        const uint32_t began = newFunction(f, VT_Began, {s, index});
        call(g::FunctionBTE_swap, {began, listener + 0x50});
        functionDestroy(began);
    }
    const uint32_t caps = f.alloc(16);
    st32(caps, s);
    st32(caps + 4, index);
    st32(caps + 8, s);
    st32(caps + 12, index);
    {
        const uint32_t moved = newEmptyFunction(f);
        const uint32_t obj = call(fn::operator_new, {0x1C});
        if (obj) {
            st32(obj, VT_Moved);
            for (uint32_t w = 4; w < 0x1C; w += 8) {
                st32(obj + w, s);
                st32(obj + w + 4, index);
            }
        }
        st32(moved + 16, obj);
        call(g::FunctionVTE_swap, {moved, listener + 0x68});
        functionDestroy(moved);
    }
    call(g::assignTouchEnded, {listener + 0x80, caps});
    call(g::assignTouchEnded, {listener + 0x98, caps});
    call(g::EventDispatcher_addSceneGraph, {dispatcher, listener, s});
}

// std::function<void(Touch*, Event*)> = the ended lambda {this, index, this, index} (heap 0x14)
void AdvEventBanner::assignTouchEnded(uint32_t dst, uint32_t caps) {
    Frame f;
    const uint32_t tmp = newEmptyFunction(f);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, VT_Ended);
        for (uint32_t w = 0; w < 16; w += 4) st32(obj + 4 + w, ld32(caps + w));
    }
    st32(tmp + 16, obj);
    call(g::FunctionVTE_swap, {tmp, dst});
    functionDestroy(tmp);
}

// Select event `id`: the selection is cleared first (one at a time), then `id` added (or, if it were there,
// removed); the cells' effect panels follow
void AdvEventBanner::setSelectItem(uint32_t s, uint32_t id) {
    Frame f;
    const uint32_t value = f.alloc(4);
    st32(value, id);
    const uint32_t vec = s + F_SELECTED;
    if (ld32(vec + 4) != ld32(vec)) {
        const uint32_t end = ld32(vec + 4);
        st32(vec + 4, end + ~((end - 4 - ld32(vec)) | 3u));
    }
    const uint32_t copy = f.alloc(12);
    call(g::VectorInt_copy, {copy, s + F_IDS});
    const uint32_t less = f.alloc(4);
    call(g::SortInt, {ld32(vec), ld32(vec + 4), less});
    const uint32_t begin = ld32(vec), end = ld32(vec + 4);
    // lower_bound
    uint32_t it = begin;
    for (int32_t len = int32_t(end - begin) >> 2; len;) {
        const int32_t half = len / 2;
        if (int32_t(ld32(it + 4 * half)) < int32_t(id)) {
            it += 4 * half + 4;
            len = len - 1 - half;
        } else {
            len = half;
        }
    }
    if (it == end || int32_t(ld32(it)) > int32_t(id)) {
        if (end == ld32(vec + 8)) {
            call(g::VectorInt_pushSlow, {vec, value});
        } else {
            if (end) st32(end, id);
            st32(vec + 4, ld32(vec + 4) + 4);
        }
        call(g::SortInt, {ld32(vec), ld32(vec + 4), less});
    } else {
        const uint32_t tmp = f.alloc(12);
        st32(tmp, 0);
        st32(tmp + 4, 0);
        st32(tmp + 8, 0);
        uint32_t p = begin;
        if (p != end) {
            do {
                if (ld32(p) != id) {
                    const uint32_t te = ld32(tmp + 4);
                    if (te == ld32(tmp + 8)) {
                        call(g::VectorInt_pushSlow, {tmp, p});
                    } else {
                        if (te) st32(te, ld32(p));
                        st32(tmp + 4, ld32(tmp + 4) + 4);
                    }
                }
                p += 4;
            } while (p != ld32(vec + 4));
            if (ld32(vec) != p) st32(vec + 4, p - ((p - ld32(vec) - 4) & ~3u) - 4);
        }
        call(g::VectorInt_assign, {vec, ld32(tmp), ld32(tmp + 4)});
        call(g::VectorInt_dtor, {tmp});
    }
    call(g::updateSelectItems, {s});
    call(g::VectorInt_dtor, {copy});
}

uint32_t AdvEventBanner::searchSelect(uint32_t s, uint32_t id) {
    uint32_t it = ld32(s + F_SELECTED);
    const uint32_t end = ld32(s + F_SELECTED + 4);
    for (int32_t len = int32_t(end - it) >> 2; len;) {
        const int32_t half = len / 2;
        if (int32_t(ld32(it + 4 * half)) < int32_t(id)) {
            it += 4 * half + 4;
            len = len - 1 - half;
        } else {
            len = half;
        }
    }
    if (it == end) return 0;
    return int32_t(ld32(it)) <= int32_t(id) ? 1 : 0;
}

// ------------------------------------------------------------------------------------------------ lambdas
// The ended lambda's body {this, index, this, index}: the item under the touch (row `index` plus the scrolled
// rows); a press that stayed within 10 points is a tap: the row comes to the front and plays the tap animation
void AdvEventBanner::touchEndedBody(uint32_t caps, uint32_t touch) {
    const uint32_t s = ld32(caps);
    const float step = ldf(s + F_ITEM_H) + ldf(ld32(GOT_ROW_SPACING));
    const uint32_t scrolled = call(g::aeabi_fixsfsi, {ld32(s + F_SCROLL_Y)});
    const uint32_t rowStep = call(g::aeabi_fixsfsi, {fbits(step)});
    const uint32_t rows = call(g::aeabi_idiv, {scrolled, rowStep});
    const uint32_t item = ld32(caps + 4) + ld32(s + F_COLUMNS) * rows - 1;
    st32(s + F_PRESS_INDEX, item);
    if (!(idCount(s) > item) || !ld8(s + F_PRESSED)) {
        st8(s + F_PRESSED, 0);
        return;
    }
    Frame f;
    const uint32_t loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    const float pressY = ldf(s + F_PRESS_Y), y = ldf(loc + 4);
    call(g::Vec2_dtor, {loc});
    if (!(std::fabs(y - pressY) < 10.0f)) {
        st8(s + F_PRESSED, 0);
        return;
    }
    st8(s + F_TAPPED, 1);
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(caps + 4));
    vcall(row, V_setLocalZOrder, {row, 1});
    const uint32_t fo = newFunction(f, VT_TapDone, {ld32(caps + 8), ld32(caps + 12)});
    call(g::Node_runAction, {row, call(g::SceneUtil_createTapAnimation, {row, fo})});
    functionDestroy(fo);
}

// the tap animation ended {this, index}: the row goes back, the item is selected
void AdvEventBanner::tapDone(uint32_t fo) {
    const uint32_t s = ld32(fo + 4);
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(fo + 8));
    vcall(row, V_setLocalZOrder, {row, 0});
    const uint32_t at = ld32(s + F_PRESS_INDEX);
    if (idCount(s) <= at) call(g::ThrowOutOfRange, {s + F_IDS});
    call(g::setSelectItem, {s, ld32(ld32(s + F_IDS) + 4 * at)});
    st8(s + F_TAPPED, 0);
    st8(s + F_PRESSED, 0);
}

namespace {
// the touch inside row `index` (its node space against its content size)
bool insideRow(uint32_t s, uint32_t index, uint32_t touch) {
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * index);
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, row, loc});
    call(g::Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, w, h});
    const bool in = call(g::Rect_containsPoint, {rect, p}) != 0;
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {p});
    return in;
}
}  // namespace

// moved {this, index, then the ended captures}: leaving the row ends the press
void AdvEventBanner::touchMoved(uint32_t fo, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(fo + 4);
    if (!ld8(s + F_PRESSED)) return;
    if (!insideRow(s, ld32(fo + 8), touch)) {
        st8(s + F_PRESSED, 0);
        call(g::touchEndedBody, {fo + 0xC, touch});
    }
}

// began {this, index}: remember the y; a touch inside the row starts a press
uint32_t AdvEventBanner::touchBegan(uint32_t fo, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(fo + 4);
    if (ld8(s + F_TAPPED)) return 0;
    {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        st32(s + F_PRESS_Y, ld32(loc + 4));
        call(g::Vec2_dtor, {loc});
    }
    if (insideRow(s, ld32(fo + 8), touch)) {
        st8(s + F_PRESSED, 1);
        return 1;
    }
    return 0;
}

uint32_t AdvEventBanner::cloneOpen(uint32_t fo) {
    const uint32_t p = call(fn::operator_new, {0x48});
    if (p) {
        st32(p, VT_Open);
        copyOpenLambda(p, fo);
    }
    return p;
}
void AdvEventBanner::cloneOpenTo(uint32_t fo, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_Open);
    copyOpenLambda(dst, fo);
}

// the banner button released on a shown cell: a locked event calls the locked callback (when the player's flags
// allow it), any other the select callback; then the item is selected
void AdvEventBanner::open(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t s = ld32(fo + 0x40);
    const uint32_t widget = ld32(fo + 4);
    if (vcall(widget, V_isVisible, {widget}) != 1) return;
    if (ld8(fo + 0x3C) == 1) {
        if (ld32(s + F_ON_LOCKED + 0x10)) {
            const uint32_t obj = ld32(call(fn::APIManager_getInstance, {}) + API_FLAGS_OBJ);
            if (obj && (ld8(obj + 0x34) & 4)) call(g::FunctionVV_call, {s + F_ON_LOCKED});
        }
    } else if (ld32(s + F_ON_SELECT + 0x10)) {
        call(g::FunctionVI_call, {s + F_ON_SELECT, ld32(fo + 0x44)});
    }
    call(g::setSelectItem, {s, ld32(fo + 0x44)});
}

// the banner downloaded {this, index, image}: an Image, then a retained Texture2D kept in the map and shown
uint32_t AdvEventBanner::loaded(uint32_t fo, uint32_t dataRef) {
    if (!ld32(dataRef)) return 0;
    const uint32_t s = ld32(fo + 4);
    const uint32_t image = call(fn::operator_new, {0xC4});
    call(g::Image_ctor, {image});
    uint32_t result = 0;
    const uint32_t data = ld32(dataRef);
    if (call(g::Image_initWithImageData, {image, ld32(data + 4), ld32(data + 8)}) == 1) {
        const uint32_t tex = call(fn::operator_new, {0x40});
        call(g::Texture2D_ctor, {tex});
        if (call(g::Texture2D_initWithImage, {tex, image}) == 1) {
            call(g::Ref_retain, {tex});
            st32(call(g::textureAt, {s + F_TEXTURES, fo + 8}), tex);
            result = 1;
            if (ld32(fo + 0xC)) {
                call(g::ImageView_loadTextureTex, {ld32(fo + 0xC), tex});
                visible(ld32(fo + 0xC), 1);
            }
        }
        call(g::Ref_release, {tex});
    }
    call(g::Ref_release, {image});
    return result;
}

RW_FUNCTION(0x586588, "AdvEventBannerScrollView::~AdvEventBannerScrollView", AdvEventBanner::destruct);
RW_FUNCTION(0x5867D0, "AdvEventBannerScrollView::create", AdvEventBanner::create);
RW_FUNCTION(0x586824, "AdvEventBannerScrollView::AdvEventBannerScrollView", AdvEventBanner::construct);
RW_FUNCTION(0x5869BC, "AdvEventBannerScrollView::clearLoadEventBanner", AdvEventBanner::clearLoadEventBanner);
RW_FUNCTION(0x586A20, "AdvEventBannerScrollView::loadEventBanner", AdvEventBanner::loadEventBanner);
RW_FUNCTION(0x586C04, "std::map<int, Texture2D*>::operator[] (AdvEventBannerScrollView)", AdvEventBanner::textureAt);
RW_FUNCTION(0x586C84, "AdvEventBannerScrollView::setEventBanner", AdvEventBanner::setEventBanner);
RW_FUNCTION(0x587394, "AdvEventBannerScrollView::updateExpirationDate", AdvEventBanner::updateExpirationDate);
RW_FUNCTION(0x58745C, "AdvEventBannerScrollView::setEventBannerTime", AdvEventBanner::setEventBannerTime);
RW_FUNCTION(0x587790, "AdvEventBannerScrollView::init", AdvEventBanner::init);
RW_FUNCTION(0x58783C, "AdvEventBannerScrollView::update", AdvEventBanner::update);
RW_FUNCTION(0x587968, "AdvEventBannerScrollView::getLayoutPanel", AdvEventBanner::getLayoutPanel);
RW_FUNCTION(0x587A30, "AdvEventBannerScrollView::createRow", AdvEventBanner::createRow);
RW_FUNCTION(0x587A88, "AdvEventBannerScrollView::changeRow", AdvEventBanner::changeRow);
RW_FUNCTION(0x587B70, "AdvEventBannerScrollView::updateSelectItems", AdvEventBanner::updateSelectItems);
RW_FUNCTION(0x587CF0, "AdvEventBannerScrollView::registItemTouchEvent", AdvEventBanner::registItemTouchEvent);
RW_FUNCTION(0x587E38, "AdvEventBannerScrollView ended lambda function::operator= (sub_587E38)", AdvEventBanner::assignTouchEnded);
RW_FUNCTION(0x587ED8, "AdvEventBannerScrollView::setSelectItem", AdvEventBanner::setSelectItem);
RW_FUNCTION(0x588090, "AdvEventBannerScrollView::searchSelect", AdvEventBanner::searchSelect);
RW_FUNCTION(0x588190, "AdvEventBannerScrollView ended lambda body (sub_588190)", AdvEventBanner::touchEndedBody);
RW_FUNCTION(0x5883B8, "AdvEventBannerScrollView tap-done lambda __func::operator() (sub_5883B8)", AdvEventBanner::tapDone);
RW_FUNCTION(0x5884C8, "AdvEventBannerScrollView moved lambda __func::operator() (sub_5884C8)", AdvEventBanner::touchMoved);
RW_FUNCTION(0x58866C, "AdvEventBannerScrollView began lambda __func::operator() (sub_58866C)", AdvEventBanner::touchBegan);
RW_FUNCTION(0x588844, "AdvEventBannerScrollView::setEventBanner $_1 __func::__clone() (sub_588844)", AdvEventBanner::cloneOpen);
RW_FUNCTION(0x588900, "AdvEventBannerScrollView::setEventBanner $_1 __func::__clone(p) (sub_588900)", AdvEventBanner::cloneOpenTo);
RW_FUNCTION(0x5889EC, "AdvEventBannerScrollView::setEventBanner $_1 __func::operator() (sub_5889EC)", AdvEventBanner::open);
RW_FUNCTION(0x588AFE, "AdvEventBannerScrollView::loadEventBanner $_0 __func::operator() (sub_588AFE)", AdvEventBanner::loaded);
