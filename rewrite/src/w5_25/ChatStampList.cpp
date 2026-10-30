// Chat > stickers: ChatStampListScrollView, a BaseStageScrollView over sticker indices 1..n. Each cell is a clone of
// the Chat_Stamp_On panel: the sticker image (TextureCache, scaled to the master::Stamp size, placed by its type)
// on an ImageButton, the animated cursor on the selected sticker, and the touch lambda $_3 {row, this, index}.
// ChatStamp records (28 bytes): +0 id, +4 a byte, +8 the image path (std::string), +0x14, +0x18 the stamp id.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t BaseStageScrollView_ctor = 0x55DAE5, BaseStageScrollView_dtor = 0x59BADD;
constexpr uint32_t BaseStageScrollView_init = 0x59BD95, BaseStageScrollView_setScrollType = 0x59D529;
constexpr uint32_t BaseStageScrollView_setItemIDs = 0x59D351;     // (this, vector<int> by value)
constexpr uint32_t ChatStampVector_dtor = 0x55B939;               // ~__vector_base<ChatStamp>
constexpr uint32_t ChatStampVector_assign = 0x55C7E9;             // vector<ChatStamp>::assign(first, last)
constexpr uint32_t VectorInt_pushBackSlow = 0x47D709;             // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t VectorInt_copy = 0x4AF707, VectorBaseInt_dtor = 0x478961;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff, Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance, ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__v;
constexpr uint32_t Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache, TextureCache_addImage = rwlib::cocos2d::TextureCache::addImage;
constexpr uint32_t Texture2D_getContentSize = rwlib::cocos2d::Texture2D::getContentSize;
constexpr uint32_t ImageButton_create = 0x531239;                // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;         // (button, node, float)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1; // (button, function by value)
constexpr uint32_t Stamp_create = 0x7919E5;                      // master::Stamp::create(shared_ptr out, id)
constexpr uint32_t floatsisf = 0xCD6D68, divsf3 = 0xCD6FAC;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
}  // namespace g

constexpr uint32_t VTABLE = 0xF89360;     // _ZTV23ChatStampListScrollView
constexpr uint32_t SIZE = 0x668;
constexpr uint32_t VEC2_ANCHOR_MIDDLE_LEFT = 0xFEF5E8;

// fields
constexpr uint32_t F_VIEW_W = 0x57C, F_VIEW_H = 0x580;           // float
constexpr uint32_t F_5A4 = 0x5A4, F_5A8 = 0x5A8, F_5AC = 0x5AC, F_5B0 = 0x5B0;   // scroll tuning (floats)
constexpr uint32_t F_5C0 = 0x5C0, F_5C4 = 0x5C4, F_5D0 = 0x5D0, F_5F0 = 0x5F0, F_HALF_H = 0x5F4;
constexpr uint32_t F_SELECTED = 0x63C;                           // index of the selected sticker, -1 none
constexpr uint32_t F_ON_SELECT = 0x640;                          // std::function
constexpr uint32_t F_PANEL = 0x658;                              // the cell template (retained)
constexpr uint32_t F_STAMPS = 0x65C;                             // vector<ChatStamp>
constexpr uint32_t STAMP_SIZE = 28, STAMP_PATH = 8, STAMP_ID = 0x18;
// master::Stamp
constexpr uint32_t ROW_TYPE = 0xC, ROW_W = 0x14, ROW_H = 0x18;
constexpr uint32_t TYPE_CENTERED = 3;

constexpr uint32_t V_DELETE = 0x04, V_SET_TOUCH_ENABLED = 0x31C, V_CREATE_ROW_CHANGE = 0x340;
constexpr uint32_t V_SET_TEXTURE = 0x27C, V_SET_TEXTURE_RECT = 0x284, V_SCALE_XY = 0x48;
constexpr uint32_t V_ARMATURE_GET_ANIMATION = 0x298, V_ANIMATION_PLAY_WITH_INDEX = 0x70;

constexpr uint32_t VT_StampTouch = 0xF58BCC;   // changeRow $_3 {row, this, index}

constexpr uint32_t S_CursorJson = 0xDFEE50;    // "cocostudio/publish/Cursor_Anim_Chat.ExportJson"
constexpr uint32_t S_StampOnJson = 0xDFEE7F;   // "cocostudio/publish/Chat_Stamp_On.json"
constexpr uint32_t S_Stamp_On = 0xDFEEA5, S_mainPanel = 0xDFD1FC, S_imageButton = 0xDF8F6D;
constexpr uint32_t S_stampImage = 0xDFEEAE, S_stampCursor = 0xDFEEB9, S_Cursor_Anim_Chat = 0xDFEEC5;
constexpr uint32_t S_Empty = 0xDF8A1A;

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }
// parent->addChild(child, z, name literal)
void addChildNamed(uint32_t parent, uint32_t child, uint32_t z, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    vcall(parent, node::addChildZName, {parent, child, z, s});
    strFree(s);
}
void setVtables(uint32_t self) {
    st32(self, VTABLE + 8);
    st32(self + 0x280, VTABLE + 0x360);
    st32(self + 0x350, VTABLE + 0x374);
}

struct ChatStampList {
    static uint32_t destruct(uint32_t self) {
        setVtables(self);
        if (ld32(self + F_PANEL)) call(g::Ref_release, {ld32(self + F_PANEL)});
        call(g::ChatStampVector_dtor, {self + F_STAMPS});
        functionDestroy(self + F_ON_SELECT);
        call(g::BaseStageScrollView_dtor, {self});
        return self;
    }

    static uint32_t create() {
        uint32_t self = call(fn::operator_new, {SIZE});
        call(g::BaseStageScrollView_ctor, {self});
        setVtables(self);
        st32(self + F_SELECTED, 0xFFFFFFFF);
        st32(self + F_ON_SELECT + 16, 0);
        st32(self + F_STAMPS, 0);
        st32(self + F_STAMPS + 4, 0);
        st32(self + F_STAMPS + 8, 0);
        if (init(self) == 1) {
            call(g::Ref_autorelease, {self});
            return self;
        }
        vcall(self, V_DELETE, {self});
        return 0;
    }

    static uint32_t init(uint32_t self) {
        stf(self + F_VIEW_W, 200.0f);
        stf(self + F_VIEW_H, 218.0f);
        vcall(self, V_SET_TOUCH_ENABLED, {self, 1});
        st8(self + F_5D0, 0);
        st32(self + F_5C0, 0);
        st32(self + F_5C4, 0);
        st32(self + F_5F0, 0);
        stf(self + F_HALF_H, ldf(self + F_VIEW_H) * 0.5f);
        // the cell template
        uint32_t panel = call(g::Widget_create, {});
        st32(self + F_PANEL, panel);
        {
            Frame f;
            uint32_t size = f.alloc(8);
            call(g::Size_ctor, {size, ld32(self + F_VIEW_W), ld32(self + F_VIEW_H)});
            vcall(panel, node::setContentSize, {panel, size});
        }
        panel = ld32(self + F_PANEL);
        vcall(panel, node::setAnchorPoint, {panel, VEC2_ANCHOR_MIDDLE_LEFT});
        call(g::Ref_retain, {ld32(self + F_PANEL)});
        panel = ld32(self + F_PANEL);
        vcall(panel, node::setVisible, {panel, 0});
        {
            uint32_t mgr = call(g::ArmatureDataManager_getInstance, {});
            Frame f;
            uint32_t s = newStr(f, S_CursorJson, 0x2E);
            call(g::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
            strFree(s);
        }
        uint32_t stampPanel = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_StampOnJson});
        uint32_t stampOn = findNode(stampPanel, S_Stamp_On, 8);
        vcall(stampPanel, node::removeChild, {stampPanel, stampOn, 1});
        setVisible(stampOn, 0);
        addChildNamed(stampPanel, stampOn, 10, S_Stamp_On, 8);
        addChildNamed(ld32(self + F_PANEL), stampPanel, 0, S_mainPanel, 9);
        call(g::BaseStageScrollView_setScrollType, {self, 1});
        call(g::Node_scheduleUpdate, {self});
        uint32_t ok = call(g::BaseStageScrollView_init, {self});
        stf(self + F_5A4, 1.0f);
        stf(self + F_5A8, 10.0f);
        stf(self + F_5AC, 2.0f);
        stf(self + F_5B0, 0.2f);
        vcall(self, V_SET_TOUCH_ENABLED, {self, 1});
        return ok;
    }

    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t arg) {
        uint32_t row = call(g::Widget_clone, {ld32(self + F_PANEL)});
        vcall(self, V_CREATE_ROW_CHANGE, {self, row, index, arg});
        return row;
    }

    // a cell: sticker `index` (1-based; 0 = none)
    static uint32_t changeRow(uint32_t self, uint32_t row, uint32_t index, uint32_t) {
        if (!index) return 0;
        uint32_t stampOn = findNode(row, S_Stamp_On, 8);
        if (!stampOn) return 1;
        setVisible(row, 1);
        uint32_t main = findNode(row, S_mainPanel, 9);
        Frame f;
        uint32_t pos = f.alloc(8);              // the sticker position, kept to the end
        call(g::Vec2_copy, {pos, vcall(stampOn, node::getPosition, {stampOn})});
        stf(pos, 100.0f);
        stf(pos + 4, 110.0f);
        vcall(stampOn, node::setPosition, {stampOn, pos});
        uint32_t button = findNode(main, S_imageButton, 11);
        if (!button) {
            {
                Frame t;
                uint32_t name = newStr(t, S_Empty, 0);
                button = call(g::ImageButton_create, {name, 0});
                strFree(name);
            }
            call(g::ImageButton_setTargetView, {button, main, fbits(1.0f)});
            addChildNamed(main, button, 0, S_imageButton, 11);
        }
        uint32_t image = findNode(button, S_stampImage, 10);
        if (!image) {
            image = call(g::Sprite_create, {});
            addChildNamed(button, image, 1, S_stampImage, 10);
        }
        uint32_t stamps = ld32(self + F_STAMPS);
        if ((ld32(self + F_STAMPS + 4) - stamps) / STAMP_SIZE <= index - 1)
            call(g::ThrowOutOfRange, {self + F_STAMPS});
        uint32_t stamp = stamps + STAMP_SIZE * (index - 1);
        uint32_t path = f.alloc(12);           // copy of the record's image path
        if (strIsLong(stamp + STAMP_PATH)) {
            strInit(path, ld32(stamp + STAMP_PATH + 8), ld32(stamp + STAMP_PATH + 4));
        } else {
            for (uint32_t k = 0; k < 12; k += 4) st32(path + k, ld32(stamp + STAMP_PATH + k));
        }
        uint32_t stampId = ld32(stamp + STAMP_ID);
        uint32_t texture = call(g::TextureCache_addImage, {call(g::Director_getTextureCache, {director()}), path});
        uint32_t texSize = f.alloc(8);
        call(g::Texture2D_getContentSize, {texSize, texture});
        vcall(image, V_SET_TEXTURE, {image, texture});
        {
            Frame t;
            uint32_t rect = t.alloc(16);
            call(g::Rect_ctor, {rect, 0, 0, ld32(texSize), ld32(texSize + 4)});
            vcall(image, V_SET_TEXTURE_RECT, {image, rect});
            call(g::Vec2_dtor, {rect});
        }
        setVisible(image, 1);
        uint32_t row2 = masterRow(f, g::Stamp_create, stampId);
        int32_t w = safe32(ld32(row2), ROW_W);
        int32_t h = safe32(ld32(row2), ROW_H);
        uint32_t sx = call(g::divsf3, {call(g::floatsisf, {uint32_t(w)}), ld32(texSize)});
        uint32_t sy = call(g::divsf3, {call(g::floatsisf, {uint32_t(h)}), ld32(texSize + 4)});
        vcall(image, V_SCALE_XY, {image, sx, sy});
        if (safe32(ld32(row2), ROW_TYPE) == int32_t(TYPE_CENTERED))
            vcall(image, node::setPositionXY, {image, fbits(100.0f), fbits(100.0f)});
        else
            vcall(image, node::setPositionXY, {image, fbits(100.0f), fbits(110.0f)});
        uint32_t cursor = findNode(button, S_stampCursor, 11);
        stf(pos, 100.0f);
        stf(pos + 4, 110.0f);
        if (cursor) {
            vcall(cursor, node::setPosition, {cursor, pos});
            setVisible(cursor, ld32(self + F_SELECTED) == index - 1 ? 1 : 0);
        } else {
            uint32_t armature;
            {
                Frame t;
                uint32_t name = newStr(t, S_Cursor_Anim_Chat, 0x10);
                armature = call(g::Armature_create, {name});
                strFree(name);
            }
            call(g::Ref_retain, {armature});
            uint32_t anim = vcall(armature, V_ARMATURE_GET_ANIMATION, {armature});
            vcall(anim, V_ANIMATION_PLAY_WITH_INDEX, {anim, 0, 0xFFFFFFFF, 0xFFFFFFFF});
            setVisible(armature, 0);
            vcall(armature, node::setPosition, {armature, pos});
            vcall(armature, node::setScale, {armature, fbits(0.9f)});
            addChildNamed(button, armature, 10, S_stampCursor, 11);
        }
        {
            Frame t;
            uint32_t touch = newFunction(t, VT_StampTouch, {row, self, index});
            call(g::ImageButton_addTouchEventListener, {button, touch});
            functionDestroy(touch);
        }
        sharedRelease(row2);
        strFree(path);
        call(g::Vec2_dtor, {pos});
        return 0;
    }

    // the stickers; the list shows indices 1..n
    static void setStampList(uint32_t self, uint32_t stamps) {
        if (self + F_STAMPS != stamps) call(g::ChatStampVector_assign, {self + F_STAMPS, ld32(stamps), ld32(stamps + 4)});
        Frame f;
        uint32_t tmp = f.alloc(4), ids = f.alloc(12);
        st32(ids, 0);
        st32(ids + 4, 0);
        st32(ids + 8, 0);
        if (ld32(stamps + 4) != ld32(stamps)) {
            for (uint32_t i = 1;; i++) {
                *rw::frame_ptr<uint32_t>(tmp) = i;
                uint32_t end = ld32(ids + 4);
                if (end >= ld32(ids + 8)) {
                    call(g::VectorInt_pushBackSlow, {ids, tmp});
                } else {
                    uint32_t next = 0;
                    if (end) {
                        st32(end, i);
                        next = ld32(ids + 4);
                    }
                    st32(ids + 4, next + 4);
                }
                if (i >= (ld32(stamps + 4) - ld32(stamps)) / STAMP_SIZE) break;
            }
        }
        uint32_t copy = f.alloc(12);
        call(g::VectorInt_copy, {copy, ids});
        call(g::BaseStageScrollView_setItemIDs, {self, copy});
        call(g::VectorBaseInt_dtor, {copy});
        call(g::VectorBaseInt_dtor, {ids});
    }
};
}  // namespace

RW_FUNCTION(0x55B874, "ChatStampListScrollView::~ChatStampListScrollView", ChatStampList::destruct);
RW_FUNCTION(0x55B9D8, "ChatStampListScrollView::create", ChatStampList::create);
RW_FUNCTION(0x55BA84, "ChatStampListScrollView::init", ChatStampList::init);
RW_FUNCTION(0x55BDA2, "ChatStampListScrollView::createRow", ChatStampList::createRow);
RW_FUNCTION(0x55BDE0, "ChatStampListScrollView::changeRow", ChatStampList::changeRow);
RW_FUNCTION(0x55C4D4, "ChatStampListScrollView::setStampList", ChatStampList::setStampList);

// ------------------------------------------------------------------------------------------------ changeRow $_3
namespace {
constexpr uint32_t SM_STAMP_BLOCKED = 0x2FC;           // SceneManager: byte, the stamp list ignores taps
constexpr uint32_t F_ROWS = 0x590;                     // std::vector<Widget*> the rows
constexpr uint32_t S_StampCursor = 0xDFEEB9;           // "stampCursor"
constexpr char STAMP_ON[] = "Stamp_On";                // built inline as a short string
constexpr uint32_t FunctionVB_call = 0x474139;

// a row's child `name` hidden (when there is one)
void hideChild(uint32_t row, uint32_t nameSlot) {
    const uint32_t node = SceneUtil::searchNodeByName(row, nameSlot);
    strFree(nameSlot);
    if (node) vcall(node, node::setVisible, {node, 0});
}
}  // namespace

// changeRow $_3 {row, view, stamp number}(sender, type): a stamp tapped -> the other rows' marks and cursors off,
// its cursor toggled, the stamp chosen (number - 1) and the view told whether the cursor shows
static void stampTouched(uint32_t functor, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t view = ld32(functor + 8);
    if (ld8(sceneManager() + SM_STAMP_BLOCKED)) return;
    const uint32_t tapped = ld32(functor + 4);
    if (vcall(tapped, node::isVisible, {tapped}) != 1) return;
    for (uint32_t i = 0; int32_t(i) < int32_t(ld32(view + F_ROWS + 4) - ld32(view + F_ROWS)) >> 2; i++) {
        const uint32_t row = ld32(ld32(view + F_ROWS) + 4 * i);
        if (row == ld32(functor + 4)) continue;
        Frame f;
        const uint32_t on = newShortStr(f, STAMP_ON);
        hideChild(row, on);
        const uint32_t cursor = f.alloc(12);
        strInitC(cursor, S_StampCursor);
        hideChild(row, cursor);
    }
    uint32_t cursor;
    {
        Frame f;
        const uint32_t name = f.alloc(12);
        strInitC(name, S_StampCursor);
        cursor = SceneUtil::searchNodeByName(ld32(functor + 4), name);
        strFree(name);
    }
    vcall(cursor, node::setVisible, {cursor, vcall(cursor, node::isVisible, {cursor}) ^ 1u});
    st32(view + F_SELECTED, ld32(functor + 0xC) - 1);
    if (ld32(view + F_ON_SELECT + 16)) call(FunctionVB_call, {view + F_ON_SELECT, vcall(cursor, node::isVisible, {cursor})});
}
RW_FUNCTION(0x55CAC8, "ChatStampListScrollView::changeRow $_3::operator()", stampTouched);
