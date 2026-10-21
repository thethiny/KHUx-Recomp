// Shop > Sell (materials): SceneSellOutMaterialItem's frame - the scene (create, D0, init with the owned materials
// sorted by their sort key, the money total, the list's animations, back to the Shop and its lambda) - and its
// MaterialScrollView (two columns of EquipSell_Block cells: icon, name, stock, price; the selected cell's mark and
// the touch lambdas that pick it). Also the material sort: std::__sort over the user material ids (libc++, 8-byte
// elements), its __sort3 and the comparator (sub_68B318: the materials' master rows, field +8). The sell flow itself
// (openSellDetailPopup, split in two by IDA, and openSellCheckDialog with their lambdas) is not part of this file.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_init = 0x88A8A1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_release = 0x8FD0BD, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Size_ctor = 0x8DF027, Rect_ctor = 0x8DF18B, Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Widget_create = 0x825875, Widget_clone = 0x8272B9;
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t ImageView_loadTextureTex = 0x8398FD;     // (image, Texture2D*)
constexpr uint32_t Node_runAction = 0x894AED, Node_convertToNodeSpace = 0x89599D;
constexpr uint32_t Touch_getLocation = 0x900E57;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t Director_getInstance = 0x8EE519, Director_getTextureCache = 0x8EFE69;
constexpr uint32_t TextureCache_addImage = 0x917DA9;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionBTE_swap = 0x483E4D;            // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVTE_swap = 0x4F3699;            // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t FunctionVI_swap = 0x5428E1;             // std::function<void(int)>::swap
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t VectorULL_copy = 0x468431, VectorULL_dtor = 0x468499;
constexpr uint32_t VectorULL_pushBackSlow = 0x4ECB6D;      // vector<ull>::__push_back_slow_path(const ull&)
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1, IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9, IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t BaseScrollViewULL_init = 0x4FC969, BaseScrollViewULL_setItemIDs = 0x4FD231;
constexpr uint32_t MaterialScrollView_create = 0x501045, MaterialScrollView_setEnabledTapItem = 0x501D8D;
constexpr uint32_t MaterialScrollView_getLayoutPanel = 0x501331, MaterialScrollView_loadTexture = 0x501AD1;
constexpr uint32_t MaterialScrollView_touchEnded = 0x501E5D;        // the $_2 lambda's call operator
constexpr uint32_t Material_create = 0x7852E5;             // master::Material::create(shared_ptr out, id)
constexpr uint32_t SceneUtil_backBtnSetPosY = 0x45AFB5;
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;       // (string out, unsigned long long)
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;    // (function<void(bool)> by value)
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B, SM_setBackButtonType = 0x613E27, SM_showHeader = 0x6131ED;
constexpr uint32_t SM_changeTickerMessage = 0x610B39, SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_isOpenedPopup = 0x621519, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t Scene_init = 0x686265, Scene_updateListField = 0x6869B1, Scene_runInAnimation = 0x686A9D, Scene_runOutAnimation = 0x686C41;
constexpr uint32_t aeabi_fixsfsi = 0xCD7240, aeabi_idiv = 0xCD5E8D;  // the original's soft-float / division helpers
constexpr uint32_t SortMaterials = 0x68AED9, MaterialLess = 0x68B319, MaterialSort3 = 0x68B495;
constexpr uint32_t MaterialSort4 = 0x68B557, MaterialSort5 = 0x68B5EF, MaterialInsertionIncomplete = 0x68B6B1;
// GOT
constexpr uint32_t GOT_VT_MaterialScrollView = 0xFD65AC, GOT_VT_BaseScrollView_ull = 0xFD65A4;
constexpr uint32_t GOT_VT_Scene = 0xFD691C;
constexpr uint32_t GOT_ROW_SPACING = 0xFD659C;             // &float added to the cell height (row step)
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_SHOP_TOP = 0xFD66F8;
}  // namespace g

// ------------------------------------------------------------------------------------------------ MaterialScrollView
constexpr uint32_t SCROLL_SIZE = 0x668;
constexpr uint32_t VT2_OFFSET = 0x360, VT3_OFFSET = 0x374;
constexpr uint32_t F_ROWS = 0x51C;            // IScrollProtocol's row widgets
constexpr uint32_t F_COLUMNS = 0x538, F_SCROLL_Y = 0x540;
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518;
constexpr uint32_t F_ITEM_IDS = 0x628;        // BaseScrollView<ull> item ids (user material ids)
constexpr uint32_t F_ON_TAP = 0x638;          // std::function<void(int)>
constexpr uint32_t F_PANEL = 0x650;           // the cell template (EquipSellBlock)
constexpr uint32_t F_TAP = 0x654;             // cells take touches
constexpr uint32_t F_PRESSED = 0x655;         // a cell is pressed (cleared on move-out / end)
constexpr uint32_t F_PRESS_INDEX = 0x658, F_TAPPED = 0x65C, F_PRESS_Y = 0x660, F_SELECTED = 0x664;
constexpr uint32_t VT_TouchBegan = 0xF55E14;  // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_TouchMoved = 0xF55DE8;  // $_1 {this, index, this, index, this, index} (heap 0x1C)
constexpr uint32_t VT_TouchEnded = 0xF55DBC;  // $_2 {this, index, this, index} (heap 0x14)
constexpr uint32_t S_EquipSellBlockJson = 0xDFD0C5, S_EquipSellBlock = 0xDFD0ED;
constexpr uint32_t S_MaterialSellIcon = 0xDFD0FC, S_TxtData1 = 0xDFD10D /* "Txt_data1 " */, S_TxtData2 = 0xDFD118;
constexpr uint32_t S_TxtMaterialName = 0xDFD122, S_TxtMaterialStock = 0xDFD134, S_PanelMask = 0xDFD14B;
constexpr uint32_t S_TxtMoneyFix = 0xDFD155, S_Dashes = 0xDFD163, S_PanelOn = 0xDFD167;
constexpr uint32_t S_FmtInt = 0xDF86DE, S_MaterialPng = 0xDF9101;   // "%d", "img/material/material_%d.png"
constexpr int32_t TXT_STOCK = 100300004;
constexpr uint32_t API_USER_MATERIALS = 0xE4;   // SafeBuffer: {u64 id, material id, count} records (16 bytes)
constexpr uint32_t ROW_NAME = 0xC, ROW_SORT = 8, ROW_PRICE = 0xD0;
constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_changeRow = 0x340, V_getParent = 0x11C;
constexpr uint32_t V_removeChild = 0x12C, V_setTouchEnabled = 0x29C;

// ------------------------------------------------------------------------------------------------ the scene
constexpr uint32_t SIZE = 0x298;
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228;
constexpr uint32_t F_ROOT = 0x288, F_SCROLL = 0x28C, F_DETAIL = 0x290, F_CAN_BACK = 0x294;
constexpr uint32_t VT_OnEnter = 0xF64D0C, VT_OnExit = 0xF64CE0, VT_BackKey = 0xF64CB4;   // {this}
constexpr uint32_t VT_SwallowBegan = 0xF64C88;   // the swallowing listener's began {}
constexpr uint32_t VT_BackButton = 0xF64C5C;     // {this}
constexpr uint32_t VT_MaterialTap = 0xF64C30;    // {this} void(int)
constexpr uint32_t VT_RunOutDone = 0xF64C04;     // runOutAnimation {std::function<void()>} (heap 0x20)
constexpr uint32_t VT_Back = 0xF64944;           // backScene {}: sub_688C2C
constexpr uint32_t S_SceneJson = 0xE04813;       // "cocostudio/publish/EquipSellScene.json"
constexpr uint32_t S_ButtonBack = 0xE03DBA, S_TxtMoneyTotal = 0xE03D94, S_Panel1 = 0xE02A0D;
constexpr uint32_t S_ScrollArea = 0xE0483A, S_TxtNone = 0xE04850, S_TxtMoneyTotalLavel = 0xE03DA4;
constexpr uint32_t S_CenterUI = 0xE02FBA, S_LeftUI = 0xDF85F0;
constexpr int32_t TXT_TICKER = 100620007, TXT_MONEY_TOTAL = 100300003, TXT_NONE = 101100011;
constexpr uint32_t API_MONEY = 0x8C;             // SafeBuffer: the user's money (+0)
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548, SM_HEADER_308 = 0x308;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
void sbRead(uint32_t sb, uint32_t dst, uint32_t offset, uint32_t len) {
    rw::call(g::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, len, offset});
}
// node->runAction(EaseSineOut(make(node, 0, 0, done)))
using MakeAnim = SceneUtil::Addr (*)(SceneUtil::Addr, float, float, SceneUtil::Addr);
void runEased(MakeAnim make, uint32_t node, uint32_t done) {
    const uint32_t action = make(node, 0.0f, 0.0f, done);
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
void runEasedEmpty(MakeAnim make, uint32_t node) {
    Frame f;
    const uint32_t done = newEmptyFunction(f);
    runEased(make, node, done);
    functionDestroy(done);
}
inline uint64_t ld64(uint32_t a) { return (uint64_t(ld32(a + 4)) << 32) | ld32(a); }

// libc++'s std::__sort over 8-byte elements with a comparator object (passed to every helper, last; to the
// comparator first), ported as is so equal keys end up in the same places
struct Sort8 {
    uint32_t comp;
    bool less(uint32_t a, uint32_t b) const { return call(g::MaterialLess, {comp, a, b}) == 1; }
    static void swap(uint32_t a, uint32_t b) {
        const uint32_t a0 = ld32(a), a1 = ld32(a + 4);
        st32(a, ld32(b));
        st32(a + 4, ld32(b + 4));
        st32(b, a0);
        st32(b + 4, a1);
    }
    static void copy(uint32_t dst, uint32_t src) {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
    }
};
}  // namespace

struct MaterialScrollView {
    static uint32_t create();
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static uint32_t getLayoutPanel(uint32_t self, uint32_t, uint32_t, uint32_t);
    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t arg);
    static uint32_t changeRow(uint32_t self, uint32_t widget, uint32_t index, uint32_t arg);
    static uint32_t loadTexture(uint32_t self, uint32_t material);
    static uint32_t registItemTouchEvent(uint32_t self, uint32_t widget, uint32_t index);
    static uint32_t assignTouchEnded(uint32_t dst, uint32_t lambda);
    static void touchEnded(uint32_t closure, uint32_t touch);
    static void touchMoved(uint32_t functor, uint32_t touchRef);
};

struct SceneSellOutMaterialItem {
    static uint32_t create();
    static void destroyDelete(uint32_t self);
    static uint32_t init(uint32_t self);
    static void updateListField(uint32_t self);
    static void runInAnimation(uint32_t self);
    static void runOutAnimation(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static uint32_t materialLess(uint32_t comp, uint32_t a, uint32_t b);
    static uint32_t sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t comp);
    static void sort(uint32_t first, uint32_t last, uint32_t comp);
};

// ================================================================================================ MaterialScrollView
uint32_t MaterialScrollView::create() {
    const uint32_t me = call(fn::operator_new, {SCROLL_SIZE});
    construct(me);
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t MaterialScrollView::construct(uint32_t s) {
    auto *base = rw::host_ptr<BaseScrollView>(s);
    call(g::Layout_ctor, {s});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    const uint32_t vt = ld32(g::GOT_VT_MaterialScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    st32(s + F_ON_TAP + 0x10, 0);
    st8(s + F_TAP, 0);
    st8(s + F_PRESSED, 0);
    st32(s + F_PRESS_INDEX, 0);
    st8(s + F_TAPPED, 0);
    st32(s + F_PRESS_Y, 0);
    st32(s + F_SELECTED, 0xFFFFFFFFu);
    return s;
}

uint32_t MaterialScrollView::destruct(uint32_t s) {
    const uint32_t vt = ld32(g::GOT_VT_MaterialScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    if (ld32(s + F_PANEL)) call(g::Ref_release, {ld32(s + F_PANEL)});
    functionDestroy(s + F_ON_TAP);
    const uint32_t bvt = ld32(g::GOT_VT_BaseScrollView_ull);
    st32(s, bvt + 8);
    st32(s + 0x280, bvt + VT2_OFFSET);
    st32(s + 0x350, bvt + VT3_OFFSET);
    call(g::VectorULL_dtor, {s + F_ITEM_IDS});
    call(g::IScrollProtocol_dtor, {s});
    return s;
}

// Cells 453x128 in the scroll view (the scene sets two columns), with the scroll bar; the cell template is the
// EquipSellBlock node of EquipSell_Block.json, taken out of its layout.
uint32_t MaterialScrollView::init(uint32_t s) {
    stf(s + F_ITEM_W, 453.0f);
    stf(s + F_ITEM_H, 128.0f);
    stf(s + 0x570, 4.0f);
    stf(s + 0x574, -4.0f);
    stf(s + 0x57C, 8.0f);
    call(g::IScrollProtocol_setEnabledScrollBar, {s, 1});
    const uint32_t layout =
        call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_EquipSellBlockJson});
    st32(s + F_PANEL, findNode(layout, S_EquipSellBlock, 0xE));
    call(g::Ref_retain, {ld32(s + F_PANEL)});
    {
        const uint32_t panel = ld32(s + F_PANEL);
        const uint32_t parent = vcall(panel, V_getParent, {panel});
        vcall(parent, V_removeChild, {parent, panel, 1});
    }
    vcall(ld32(s + F_PANEL), V_setTouchEnabled, {ld32(s + F_PANEL), 0});
    return call(g::BaseScrollViewULL_init, {s});
}

// A cell: an empty widget of the cell size holding a clone of the template.
uint32_t MaterialScrollView::getLayoutPanel(uint32_t s, uint32_t, uint32_t, uint32_t) {
    const uint32_t panel = call(g::Widget_create, {});
    const uint32_t cell = call(g::Widget_clone, {ld32(s + F_PANEL)});
    vcall(panel, node::setAnchorPoint, {panel, VEC2_ANCHOR_MIDDLE});
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

uint32_t MaterialScrollView::createRow(uint32_t s, uint32_t index, uint32_t arg) {
    const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 3);
    if (count <= index) {
        call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint32_t panel = call(g::MaterialScrollView_getLayoutPanel, {s});
    vcall(s, V_changeRow, {s, panel, index, arg});
    return panel;
}

// Fill a cell with user material `index`: icon, name, stock, price ("---" and the mask when it cannot be sold)
// and the selection mark.
uint32_t MaterialScrollView::changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
    const uint32_t ids = ld32(s + F_ITEM_IDS);
    if (!(uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ids) >> 3) > index)) {
        call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint64_t id = ld64(ids + 8 * index);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t materialId = 0, count = 0, selected = 0;
    Frame f;
    {
        const uint32_t rec = f.alloc(16);
        uint32_t sb = ld32(api + API_USER_MATERIALS);
        if (ld32(sb + 0x20)) {
            for (uint32_t i = 0, at = 0;; at += 16) {
                sbRead(sb, rec, at, 16);
                i++;
                if (ld64(rec) == id) {
                    materialId = ld32(rec + 8);
                    const uint32_t stock = ld32(rec + 0xC);
                    const uint32_t copy = f.alloc(12);
                    call(g::VectorULL_copy, {copy, s + F_ITEM_IDS});
                    const int32_t sel = int32_t(ld32(s + F_SELECTED));
                    uint32_t mark = 0;
                    if (sel >= 0) {
                        const int32_t n = int32_t(ld32(copy + 4) - ld32(copy)) >> 3;
                        if (sel < n) {
                            if (uint32_t(n) <= uint32_t(sel)) call(g::ThrowOutOfRange, {copy});
                            mark = ld64(rec) == ld64(ld32(copy) + 8 * uint32_t(sel)) ? 1 : 0;
                        }
                    }
                    call(g::VectorULL_dtor, {copy});
                    selected = mark;
                    count = stock;
                    break;
                }
                sb = ld32(api + API_USER_MATERIALS);
                if (i >= ld32(sb + 0x20)) break;
            }
        }
    }
    const uint32_t material = f.alloc(8);
    call(g::Material_create, {material, materialId});
    const uint32_t icon = findNode(widget, S_MaterialSellIcon, 0x10);
    uint32_t result = 1;
    if (icon) {
        {
            Frame t;
            const uint32_t copy = t.alloc(8);
            st32(copy, ld32(material));
            const uint32_t ctl = ld32(material + 4);
            st32(copy + 4, ctl);
            if (ctl) call(fn::shared_add, {ctl});
            const uint32_t texture = call(g::MaterialScrollView_loadTexture, {s, copy});
            call(g::ImageView_loadTextureTex, {icon, texture});
            releaseIf(ctl);
        }
        SceneUtil::setShadow(findNode(widget, S_TxtData1, 0xA));
        {
            const uint32_t text = findNode(widget, S_TxtData2, 9);
            SceneUtil::setShadow(text);
            result = 0;
            setTextId(text, TXT_STOCK);
        }
        {
            const uint32_t text = findNode(widget, S_TxtMaterialName, 0x11);
            SceneUtil::setShadow(text);
            Frame t;
            const uint32_t name = t.alloc(0x44);
            sbRead(ld32(material), name, ROW_NAME, 0x41);
            setTextC(text, stringBuffer(t, {name}));
        }
        {
            const uint32_t text = findNode(widget, S_TxtMaterialStock, 0x16);
            SceneUtil::setShadow(text);
            Frame t;
            setTextC(text, stringBuffer(t, {S_FmtInt, count}));
        }
        const uint32_t mask = findNode(widget, S_PanelMask, 9);
        {
            const uint32_t text = findNode(widget, S_TxtMoneyFix, 0xD);
            SceneUtil::setShadow(text);
            Frame t;
            const uint32_t price = t.alloc(4);
            sbRead(ld32(material), price, ROW_PRICE, 4);
            if (int32_t(ld32(price)) < 1) {
                const uint32_t str = newStr(t, S_Dashes, 3);
                call(g::Text_setString, {text, str});
                strFree(str);
                setVisible(mask, 1);
            } else {
                setTextC(text, stringBuffer(t, {S_FmtInt, ld32(price)}));
                setVisible(mask, 0);
            }
        }
        setVisible(findNode(widget, S_PanelOn, 8), selected);
    }
    releaseIf(ld32(material + 4));
    return result;
}

// The texture of a material's icon ("img/material/material_<id>.png") from the texture cache.
uint32_t MaterialScrollView::loadTexture(uint32_t, uint32_t material) {
    const uint32_t cache = call(g::Director_getTextureCache, {call(g::Director_getInstance, {})});
    Frame f;
    const uint32_t id = f.alloc(4);
    sbRead(ld32(material), id, 0, 4);
    const uint32_t buf = stringBuffer(f, {S_MaterialPng, ld32(id)});
    const uint32_t name = newStr(f, buf);
    const uint32_t texture = call(g::TextureCache_addImage, {cache, name});
    strFree(name);
    return texture;
}

// Touch listener for a cell: began / moved / ended / cancelled lambdas capturing (this, index); the listener is
// registered on the scroll view itself.
uint32_t MaterialScrollView::registItemTouchEvent(uint32_t s, uint32_t, uint32_t index) {
    if (!ld8(s + F_TAP)) return 0;
    const uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    const uint32_t began = newFunction(f, VT_TouchBegan, {s, index});
    call(g::FunctionBTE_swap, {began, listener + 0x50});
    functionDestroy(began);
    const uint32_t captures = f.alloc(16);
    st32(captures, s);
    st32(captures + 4, index);
    st32(captures + 8, s);
    st32(captures + 12, index);
    const uint32_t moved = f.alloc(20);
    st32(moved + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, VT_TouchMoved);
        for (uint32_t w = 4; w < 0x1C; w += 8) {
            st32(obj + w, s);
            st32(obj + w + 4, index);
        }
    }
    st32(moved + 16, obj);
    call(g::FunctionVTE_swap, {moved, listener + 0x68});
    functionDestroy(moved);
    assignTouchEnded(listener + 0x80, captures);
    assignTouchEnded(listener + 0x98, captures);
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, s});
    return 0;
}

// std::function<void(Touch*, Event*)>::operator=($_2&&): a heap __func with the four captures, swapped in
uint32_t MaterialScrollView::assignTouchEnded(uint32_t dst, uint32_t lambda) {
    Frame f;
    const uint32_t tmp = f.alloc(20);
    st32(tmp + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, VT_TouchEnded);
        for (uint32_t w = 0; w < 16; w += 4) st32(obj + 4 + w, ld32(lambda + w));
    }
    st32(tmp + 16, obj);
    call(g::FunctionVTE_swap, {tmp, dst});
    functionDestroy(tmp);
    return 0;
}

// The $_2 lambda (touch ended / cancelled) {this, index, this, index}: the item under the touch (row `index`,
// from the scroll offset); a press that did not move 10 points selects it - row marks, on-tap(item) - and the
// list is laid out again.
void MaterialScrollView::touchEnded(uint32_t closure, uint32_t touch) {
    const uint32_t s = ld32(closure);
    const float step = ldf(s + F_ITEM_H) + ldf(ld32(g::GOT_ROW_SPACING));
    const uint32_t scrolled = call(g::aeabi_fixsfsi, {ld32(s + F_SCROLL_Y)});
    const uint32_t rowStep = call(g::aeabi_fixsfsi, {fbits(step)});
    const uint32_t rows = call(g::aeabi_idiv, {scrolled, rowStep});
    const uint32_t item = ld32(closure + 4) + ld32(s + F_COLUMNS) * rows - 1;
    st32(s + F_PRESS_INDEX, item);
    const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 3);
    if (!(count > item) || !ld8(s + F_PRESSED)) {
        st8(s + F_PRESSED, 0);
        return;
    }
    Frame f;
    const uint32_t loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    const float moved = std::fabs(ldf(loc + 4) - ldf(s + F_PRESS_Y));
    if (!(moved < 10.0f)) {
        call(g::Vec2_dtor, {loc});
        st8(s + F_PRESSED, 0);
        return;
    }
    const uint32_t ids = ld32(s + F_ITEM_IDS);
    const uint32_t at = ld32(s + F_PRESS_INDEX);
    if (!(uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ids) >> 3) > at)) {
        call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        return;
    }
    const uint64_t id = ld64(ids + 8 * at);
    call(g::Vec2_dtor, {loc});
    if (!id) {
        st8(s + F_PRESSED, 0);
        return;
    }
    st8(s + F_TAPPED, 1);
    st32(s + F_SELECTED, ld32(s + F_PRESS_INDEX));
    {
        const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(closure + 4));
        vcall(row, node::setLocalZOrder, {row, 1});
    }
    const uint32_t s2 = ld32(closure + 8);
    {
        const uint32_t row = ld32(ld32(s2 + F_ROWS) + 4 * ld32(closure + 0xC));
        vcall(row, node::setLocalZOrder, {row, 0});
    }
    if (ld32(s2 + F_ON_TAP + 0x10) && ld8(s2 + F_PRESSED))
        call(g::FunctionVI_call, {s2 + F_ON_TAP, ld32(s2 + F_PRESS_INDEX)});
    st8(s2 + F_TAPPED, 0);
    st8(s2 + F_PRESSED, 0);
    call(g::IScrollProtocol_updateLayout, {s, 0});
}

// The $_1 lambda (touch moved) __func::operator() {this, index, then the $_2 captures}: a touch that leaves its
// cell ends the press.
void MaterialScrollView::touchMoved(uint32_t functor, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(functor + 4);
    if (!ld8(s + F_PRESSED)) return;
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(functor + 8));
    Frame f;
    const uint32_t loc = f.alloc(8), local = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {local, row, loc});
    call(g::Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
    rw::call(g::Rect_ctor, {rect, 0, 0, w, h});
    if (!call(g::Rect_containsPoint, {rect, local})) {
        st8(s + F_PRESSED, 0);
        call(g::MaterialScrollView_touchEnded, {functor + 0xC, touch});
    }
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {local});
}

// ================================================================================================ the scene
uint32_t SceneSellOutMaterialItem::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    st32(me, ld32(g::GOT_VT_Scene) + 8);
    st32(me + F_DETAIL, 0);
    st8(me + F_CAN_BACK, 0);
    if (call(g::Scene_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

void SceneSellOutMaterialItem::destroyDelete(uint32_t self) {
    st32(self, ld32(g::GOT_VT_Scene) + 8);
    call(g::Layer_dtor, {self});
    call(fn::operator_delete, {self});
}

uint32_t SceneSellOutMaterialItem::init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    Frame f;
    for (const auto &[member, vtable] : {std::pair{F_ON_ENTER, VT_OnEnter}, std::pair{F_ON_EXIT, VT_OnExit}}) {
        const uint32_t fo = newFunction(f, vtable, {me});
        call(g::FunctionVV_assign, {me + member, fo});
        functionDestroy(fo);
    }
    st8(me + F_CAN_BACK, 1);
    {
        const uint32_t m = sceneManager();
        const uint32_t fo = newFunction(f, VT_BackKey, {me});
        call(g::SM_setOnBackKeyCallback, {m, fo});
        functionDestroy(fo);
    }
    call(g::SM_setBackButtonType, {sceneManager(), 0});
    call(g::SM_showHeader, {sceneManager()});
    {
        const uint32_t m = sceneManager();
        const uint32_t td = textData(f, TXT_TICKER);
        const uint32_t s = newStr(f, textDataStr(td));
        call(g::SM_changeTickerMessage, {m, s});
        strFree(s);
        sharedRelease(td);
    }
    {
        const uint32_t s = newStr(f, S_SceneJson, 0x26);
        st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
        strFree(s);
    }
    vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
    {
        // a touch-swallowing listener (never registered)
        const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
        call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
        const uint32_t fo = newFunction(f, VT_SwallowBegan, {});
        call(g::FunctionBTE_swap, {fo, listener + 0x50});
        functionDestroy(fo);
    }
    {
        const uint32_t back = findNode(ld32(me + F_ROOT), S_ButtonBack, 0xB);
        call(g::SceneUtil_backBtnSetPosY, {back});
        addTouch(back, VT_BackButton, {me});
    }
    // the materials the user has (count >= 1), sorted
    const uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    {
        const uint32_t rec = f.alloc(16);
        uint32_t sb = ld32(api + API_USER_MATERIALS);
        if (ld32(sb + 0x20)) {
            for (uint32_t i = 0, at = 0; i < ld32(ld32(api + API_USER_MATERIALS) + 0x20); i++, at += 16) {
                sbRead(sb, rec, at, 16);
                if (int32_t(ld32(rec + 0xC)) >= 1) {
                    const uint32_t end = ld32(ids + 4);
                    if (end == ld32(ids + 8)) {
                        call(g::VectorULL_pushBackSlow, {ids, rec});
                    } else {
                        uint32_t n = 0;
                        if (end) {
                            st32(end, ld32(rec));
                            st32(end + 4, ld32(rec + 4));
                            n = ld32(ids + 4);
                        }
                        st32(ids + 4, n + 8);
                    }
                }
                sb = ld32(api + API_USER_MATERIALS);
            }
        }
    }
    {
        const uint32_t comp = f.alloc(4);
        st32(comp, api + API_USER_MATERIALS);
        call(g::SortMaterials, {ld32(ids), ld32(ids + 4), comp});
    }
    {
        const uint32_t text = findNode(ld32(me + F_ROOT), S_TxtMoneyTotal, 0xF);
        SceneUtil::setShadow(text);
        setTextId(text, TXT_MONEY_TOTAL);
    }
    const uint32_t panel = findNode(ld32(me + F_ROOT), S_Panel1, 7);
    vcall(panel, V_setTouchEnabled, {panel, 0});
    const uint32_t area = findNode(panel, S_ScrollArea, 0x15);
    st32(me + F_SCROLL, call(g::MaterialScrollView_create, {}));
    call(g::IScrollProtocol_setMaxColumn, {ld32(me + F_SCROLL), 2});
    call(g::MaterialScrollView_setEnabledTapItem, {ld32(me + F_SCROLL), 1});
    {
        const uint32_t scroll = ld32(me + F_SCROLL);
        vcall(scroll, node::setContentSize, {scroll, vcall(area, node::getContentSize, {area})});
    }
    call(g::BaseScrollViewULL_setItemIDs, {ld32(me + F_SCROLL), ids});
    {
        const uint32_t fo = newFunction(f, VT_MaterialTap, {me});
        call(g::FunctionVI_swap, {fo, ld32(me + F_SCROLL) + F_ON_TAP});
        functionDestroy(fo);
    }
    vcall(area, node::addChild, {area, ld32(me + F_SCROLL)});
    setVisible(findNode(ld32(me + F_ROOT), S_TxtNone, 8), 0);
    if (ld32(ids + 4) == ld32(ids)) {
        const uint32_t none = findNode(ld32(me + F_ROOT), S_TxtNone, 8);
        SceneUtil::setShadow(none);
        setVisible(none, 1);
        setTextId(none, TXT_NONE);
    }
    call(g::Scene_updateListField, {me});
    call(g::Scene_runInAnimation, {me});
    call(g::VectorULL_dtor, {ids});
    return 1;
}

// The user's money.
void SceneSellOutMaterialItem::updateListField(uint32_t me) {
    const uint32_t text = findNode(ld32(me + F_ROOT), S_TxtMoneyTotalLavel, 0x15);
    SceneUtil::setShadow(text);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t money = f.alloc(4);
    sbRead(ld32(api + API_MONEY), money, 0, 4);
    const uint32_t str = f.alloc(12);
    const int32_t value = int32_t(ld32(money));
    call(g::SceneUtil_getMoneyStr, {str, 0, uint32_t(value), uint32_t(value >> 31)});
    call(g::Text_setString, {text, str});
    strFree(str);
}

void SceneSellOutMaterialItem::runInAnimation(uint32_t me) {
    runEasedEmpty(SceneUtil::createLeftInAnimation, findNode(ld32(me + F_ROOT), S_CenterUI, 8));
    runEasedEmpty(SceneUtil::createLeftInAnimation, findNode(ld32(me + F_ROOT), S_LeftUI, 6));
}

// Slide both panels out; `done` runs when the centre one is gone.
void SceneSellOutMaterialItem::runOutAnimation(uint32_t me, uint32_t done) {
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    const uint32_t center = findNode(ld32(me + F_ROOT), S_CenterUI, 8);
    {
        Frame f;
        const uint32_t captured = f.alloc(20);
        call(fn::function_vv_copy, {captured, done});
        const uint32_t onDone = f.alloc(20);
        heapFunction(onDone, VT_RunOutDone, 0x20, 8, fn::function_vv_move, captured);
        runEased(SceneUtil::createLeftOutAnimation, center, onDone);
        functionDestroy(onDone);
        functionDestroy(captured);
    }
    runEasedEmpty(SceneUtil::createLeftOutAnimation, findNode(ld32(me + F_ROOT), S_LeftUI, 6));
}

// Back: an open popup closes first; otherwise the list stops taking taps, the Shop data is requested and the
// panels slide out (then the Shop).
void SceneSellOutMaterialItem::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    if (call(g::SM_isOpenedPopup, {sceneManager()}) == 1) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 0});
        return;
    }
    st8(me + F_CAN_BACK, 0);
    st8(sceneManager() + SM_HEADER_308, 0);
    call(g::MaterialScrollView_setEnabledTapItem, {ld32(me + F_SCROLL), 0});
    {
        const uint32_t scroll = ld32(me + F_SCROLL);
        functionDestroy(scroll + F_ON_TAP);
        st32(scroll + F_ON_TAP + 0x10, 0);
    }
    Frame f;
    {
        const uint32_t m = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    }
    {
        const uint32_t none = newEmptyFunction(f);
        call(g::SceneShopTop_callShopAPI, {none});
        functionDestroy(none);
    }
    const uint32_t fo = newFunction(f, VT_Back, {});
    call(g::Scene_runOutAnimation, {me, fo});
    functionDestroy(fo);
}

// backScene's lambda: to the Shop.
void SceneSellOutMaterialItem::backDone(uint32_t) {
    const uint32_t m = sceneManager();
    Frame f;
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(g::GOT_CREATE_SHOP_TOP)});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
    functionDestroy(layer);
}

// ------------------------------------------------------------------------------------------------ the sort
// The comparator {&APIManager's user materials}: the materials of user material ids a and b (master rows found
// through the user records), compared by the row's sort key (+8).
uint32_t SceneSellOutMaterialItem::materialLess(uint32_t comp, uint32_t a, uint32_t b) {
    Frame f;
    const uint32_t rec = f.alloc(16), tmp = f.alloc(8), key = f.alloc(4);
    uint32_t rowA = 0, ctlA = 0, rowB = 0, ctlB = 0;
    uint32_t sb = ld32(ld32(comp));
    if (ld32(sb + 0x20)) {
        for (uint32_t i = 0, at = 0;; at += 16) {
            sbRead(sb, rec, at, 16);
            if (ld64(rec) == ld64(a)) {
                call(g::Material_create, {tmp, ld32(rec + 8)});
                rowA = ld32(tmp);
                const uint32_t c = ld32(tmp + 4);
                releaseIf(ctlA);
                ctlA = c;
            }
            if (ld64(rec) == ld64(b)) {
                call(g::Material_create, {tmp, ld32(rec + 8)});
                rowB = ld32(tmp);
                const uint32_t c = ld32(tmp + 4);
                releaseIf(ctlB);
                ctlB = c;
            }
            i++;
            sb = ld32(ld32(comp));
            if (!(i < ld32(sb + 0x20))) break;
        }
    }
    sbRead(rowA, key, ROW_SORT, 4);
    const int32_t ka = int32_t(ld32(key));
    sbRead(rowB, key, ROW_SORT, 4);
    const int32_t kb = int32_t(ld32(key));
    releaseIf(ctlB);
    releaseIf(ctlA);
    return ka < kb ? 1 : 0;
}

// std::__sort3: returns the swaps made
uint32_t SceneSellOutMaterialItem::sort3(uint32_t x, uint32_t y, uint32_t z, uint32_t comp) {
    const Sort8 o{comp};
    const bool yx = o.less(y, x);
    const bool zy = o.less(z, y);
    if (!yx) {
        if (!zy) return 0;
        Sort8::swap(y, z);
        if (!o.less(y, x)) return 1;
        Sort8::swap(x, y);
        return 2;
    }
    if (zy) {
        Sort8::swap(x, z);
        return 1;
    }
    Sort8::swap(x, y);
    if (!o.less(z, y)) return 1;
    Sort8::swap(y, z);
    return 2;
}

// std::__sort (libc++: insertion sort up to 30 elements, median of 3 / 5 pivots, the equal-pivot partition, the
// insertion-sort shortcut after a swap-free partition, recursion into the smaller side)
void SceneSellOutMaterialItem::sort(uint32_t first, uint32_t last, uint32_t comp) {
    const Sort8 o{comp};
    constexpr uint32_t E = 8;
    for (;;) {
    restart:
        const uint32_t len = uint32_t(int32_t(last - first) >> 3);
        const uint32_t lm1 = last - E;
        switch (len) {
        case 0:
        case 1:
            return;
        case 2:
            if (o.less(lm1, first)) Sort8::swap(first, lm1);
            return;
        case 3:
            call(g::MaterialSort3, {first, first + E, lm1, comp});
            return;
        case 4:
            call(g::MaterialSort4, {first, first + E, first + 2 * E, lm1, comp});
            return;
        case 5:
            rw::call(g::MaterialSort5, {first, first + E, first + 2 * E, first + 3 * E, lm1, comp});
            return;
        }
        if (len <= 30) {
            call(g::MaterialSort3, {first, first + E, first + 2 * E, comp});
            Frame f;
            const uint32_t t = f.alloc(8);
            for (uint32_t j = first + 2 * E, i = j + E; i != last; j = i, i += E) {
                if (!o.less(i, j)) continue;
                Sort8::copy(t, i);
                uint32_t k = j, hole = i;
                for (;;) {
                    Sort8::copy(hole, k);
                    hole = k;
                    if (hole == first) break;
                    k -= E;
                    if (!o.less(t, k)) break;
                }
                Sort8::copy(hole, t);
            }
            return;
        }
        uint32_t m = first + E * (len / 2);
        uint32_t swaps;
        if (len >= 1000) {
            const uint32_t delta = len / 4;
            swaps = rw::call(g::MaterialSort5, {first, first + E * delta, m, m + E * delta, lm1, comp});
        } else {
            swaps = call(g::MaterialSort3, {first, m, lm1, comp});
        }
        uint32_t i = first, j = lm1;
        if (!o.less(i, m)) {
            bool found = false;
            while (j - E != i) {
                j -= E;
                if (o.less(j, m)) {
                    Sort8::swap(i, j);
                    swaps++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                i = first + E;
                j = lm1;
                if (!o.less(first, j)) {
                    for (;;) {
                        if (i == j) return;
                        if (o.less(first, i)) {
                            Sort8::swap(i, j);
                            swaps++;
                            i += E;
                            break;
                        }
                        i += E;
                    }
                }
                if (i == j) return;
                for (;;) {
                    while (!o.less(first, i)) i += E;
                    do j -= E;
                    while (o.less(first, j));
                    if (i >= j) break;
                    Sort8::swap(i, j);
                    swaps++;
                    i += E;
                }
                first = i;
                goto restart;
            }
        }
        i += E;
        if (i < j) {
            for (;;) {
                while (o.less(i, m)) i += E;
                do j -= E;
                while (!o.less(j, m));
                if (i > j) break;
                Sort8::swap(i, j);
                swaps++;
                if (m == i) m = j;
                i += E;
            }
        }
        if (i != m && o.less(m, i)) {
            Sort8::swap(i, m);
            swaps++;
        }
        if (swaps == 0) {
            const bool fs = call(g::MaterialInsertionIncomplete, {first, i, comp}) != 0;
            if (call(g::MaterialInsertionIncomplete, {i + E, last, comp}) == 1) {
                if (fs) return;
                last = i;
                continue;
            }
            if (fs) {
                first = i + E;
                continue;
            }
        }
        if (int32_t(i - first) < int32_t(last - i)) {
            call(g::SortMaterials, {first, i, comp});
            first = i + E;
        } else {
            call(g::SortMaterials, {i + E, last, comp});
            last = i;
        }
    }
}

RW_FUNCTION(0x500EE4, "MaterialScrollView::~MaterialScrollView", MaterialScrollView::destruct);
RW_FUNCTION(0x501044, "MaterialScrollView::create", MaterialScrollView::create);
RW_FUNCTION(0x501098, "MaterialScrollView::MaterialScrollView", MaterialScrollView::construct);
RW_FUNCTION(0x5011FC, "MaterialScrollView::init", MaterialScrollView::init);
RW_FUNCTION(0x501330, "MaterialScrollView::getLayoutPanel", MaterialScrollView::getLayoutPanel);
RW_FUNCTION(0x5013F8, "MaterialScrollView::createRow", MaterialScrollView::createRow);
RW_FUNCTION(0x501450, "MaterialScrollView::changeRow", MaterialScrollView::changeRow);
RW_FUNCTION(0x501AD0, "MaterialScrollView::loadTexture", MaterialScrollView::loadTexture);
RW_FUNCTION(0x501BB0, "MaterialScrollView::registItemTouchEvent", MaterialScrollView::registItemTouchEvent);
RW_FUNCTION(0x501CF8, "MaterialScrollView::registItemTouchEvent $_2 std::function::operator=(&&) (sub_501CF8)",
            MaterialScrollView::assignTouchEnded);
RW_FUNCTION(0x501E5C, "MaterialScrollView::registItemTouchEvent $_2 call operator (sub_501E5C)",
            MaterialScrollView::touchEnded);
RW_FUNCTION(0x502090, "MaterialScrollView::registItemTouchEvent $_1 __func::operator() (sub_502090)",
            MaterialScrollView::touchMoved);
RW_FUNCTION(0x6861AC, "SceneSellOutMaterialItem::create", SceneSellOutMaterialItem::create);
RW_FUNCTION(0x686240, "SceneSellOutMaterialItem::~SceneSellOutMaterialItem (deleting)",
            SceneSellOutMaterialItem::destroyDelete);
RW_FUNCTION(0x686264, "SceneSellOutMaterialItem::init", SceneSellOutMaterialItem::init);
RW_FUNCTION(0x6869B0, "SceneSellOutMaterialItem::updateListField", SceneSellOutMaterialItem::updateListField);
RW_FUNCTION(0x686A9C, "SceneSellOutMaterialItem::runInAnimation", SceneSellOutMaterialItem::runInAnimation);
RW_FUNCTION(0x686C40, "SceneSellOutMaterialItem::runOutAnimation", SceneSellOutMaterialItem::runOutAnimation);
RW_FUNCTION(0x688A60, "SceneSellOutMaterialItem::backScene", SceneSellOutMaterialItem::backScene);
RW_FUNCTION(0x688C2C, "SceneSellOutMaterialItem::backScene lambda __func::operator() (sub_688C2C)",
            SceneSellOutMaterialItem::backDone);
RW_FUNCTION(0x68B318, "SceneSellOutMaterialItem material sort comparator (sub_68B318)",
            SceneSellOutMaterialItem::materialLess);
RW_FUNCTION(0x68B494, "std::__sort3<material comparator&, ull*> (sub_68B494)", SceneSellOutMaterialItem::sort3);

// std::__sort4 / std::__sort5 of the material sort (sub_68B556 / sub_68B5EE): the swaps made
static uint32_t materialSort4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t comp) {
    const Sort8 o{comp};
    const uint32_t r = call(g::MaterialSort3, {x1, x2, x3, comp});
    if (!o.less(x4, x3)) return r;
    Sort8::swap(x3, x4);
    if (!o.less(x3, x2)) return r + 1;
    Sort8::swap(x2, x3);
    if (!o.less(x2, x1)) return r + 2;
    Sort8::swap(x1, x2);
    return r + 3;
}
static uint32_t materialSort5(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t comp) {
    const Sort8 o{comp};
    const uint32_t r = call(g::MaterialSort4, {x1, x2, x3, x4, comp});
    if (!o.less(x5, x4)) return r;
    Sort8::swap(x4, x5);
    if (!o.less(x4, x3)) return r + 1;
    Sort8::swap(x3, x4);
    if (!o.less(x3, x2)) return r + 2;
    Sort8::swap(x2, x3);
    if (!o.less(x2, x1)) return r + 3;
    Sort8::swap(x1, x2);
    return r + 4;
}
RW_FUNCTION(0x68B556, "std::__sort4<material comparator&, ull*> (sub_68B556)", materialSort4);
RW_FUNCTION(0x68B5EE, "std::__sort5<material comparator&, ull*> (sub_68B5EE)", materialSort5);
RW_FUNCTION(0x68AED8, "std::__sort<material comparator&, ull*> (sub_68AED8)", SceneSellOutMaterialItem::sort);
