// AvatarPartsScrollView: construction, cells and touch handling (the cell contents are in
// AvatarPartsScrollView_Row.cpp).
#include "AvatarPartsScrollView.hpp"

#include "../scene_social/SocialCommon.hpp"

using namespace social;
using Self = AvatarPartsScrollView;

namespace {
namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Layout_ctor = 0x828671;                 // cocos2d::ui::Layout::Layout()
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;        // IScrollProtocol::~IScrollProtocol
constexpr uint32_t IScrollProtocol_update = 0x4F5CFD;      // IScrollProtocol::update(float)
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;   // (self, bool)
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t BaseScrollView_int_init = 0x4FA8A1;     // BaseScrollView<int>::init()
constexpr uint32_t vector_int_dtor = 0x478961;             // std::__vector_base<int>::~__vector_base
constexpr uint32_t vector_int_assign = 0x47D5BD;           // std::vector<int>::assign(int*, int*)
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Armature_create = 0x79F3C7;             // cocostudio::Armature::create(const string&)
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t throwOutOfRange = 0x47CCE1;             // std::vector::at out of range (throws)
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t function_touchBool_swap = 0x483E4D;     // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t function_touchVoid_swap = 0x4F3699;     // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t function_vi_call = 0x4F321D;            // std::function<void(int)>::operator()(int)
constexpr uint32_t Touch_getLocation = 0x900E57;           // (Vec2* out, Touch*)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;     // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t Rect_ctor = 0x8DF18B;                   // Rect(x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t AvatarParts_create = 0x778DE1;          // master::AvatarParts::create(shared_ptr out, id)
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_removeNewAvatarPartsIDs = 0x4EBC55;
constexpr uint32_t AppState_removeNewAvatarPartsSetIDs = 0x4EBDD1;
constexpr uint32_t divsi3 = 0xCD5E8D;                      // __divsi3
}  // namespace g

constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;           // GOT slot: &cocos2d::Vec2::ANCHOR_MIDDLE
constexpr uint32_t ITEM_MARGIN = 0xE1F834;                 // IScrollProtocol::ITEM_MARGIN (float)
constexpr uint32_t S_PanelJson = 0xDFEAFB;                 // "cocostudio/publish/AvatarEdit_Panel.json"
constexpr uint32_t S_mainPanel = 0xDFD1FC;
constexpr uint32_t S_Panel = 0xDFD420;
constexpr uint32_t S_CursorJson = 0xDFEB24;                // "cocostudio/publish/Cursor_Anim_AvatarEdit.ExportJson"
constexpr uint32_t S_Cursor = 0xDFEB59;                    // "Cursor_Anim_AvatarEdit"
constexpr uint32_t S_Animation1 = 0xDFD0AF;
constexpr uint32_t S_Area_Point = 0xDF9FB4;
constexpr uint32_t S_SelectedAnimation = 0xDFD24C;

// lambdas of registItemTouchEvent (std::function vtables, __func + 8)
constexpr Addr VT_touchBegan_L0 = 0xF58BA0;   // [this, index]: onTouchBegan
constexpr Addr VT_touchMoved_L1 = 0xF58B74;   // [this, index, $_2 = {this, index, this}]: onTouchMoved
constexpr Addr VT_touchEnded_L2 = 0xF58B1C;   // {this, index, this}: onTouchEnded / onTouchCancelled
constexpr Addr VT_tapDone_L3 = 0xF58B48;      // [this]: after the tap animation

constexpr uint32_t SLOT_init = 0x248;          // vtable byte offset of init() (vtable + 0x248)
constexpr uint32_t SLOT_release = 0x04;
constexpr uint32_t SLOT_changeRow = 0x340;     // AvatarPartsScrollView::changeRow (virtual)
constexpr uint32_t SLOT_getAnimation = 0x298;  // cocostudio::Armature::getAnimation
constexpr uint32_t SLOT_play = 0x68;           // ArmatureAnimation::play(name, durationTo, loop)

int32_t idiv(int32_t a, int32_t b) { return int32_t(call(g::divsi3, {uint32_t(a), uint32_t(b)})); }

// The item index under a cell: index + maxColumn * (rows scrolled) - 1 (rows = scroll offset / row height).
int32_t itemIndexAt(const Self *s, int32_t index) {
    float rowHeight = s->itemHeight + ldf(ITEM_MARGIN);
    int32_t offset = f2i(s->scrollOffset);
    int32_t rows = idiv(offset, f2i(rowHeight));
    return int32_t(uint32_t(index) + uint32_t(s->maxColumn) * uint32_t(rows) - 1);
}

uint32_t itemCount(const Self *s) { return (s->itemIDs.end - s->itemIDs.begin) >> 2; }

// Is the touch inside the node's own rect (0, 0, content size)?
bool touchInside(Addr node, Addr touch) {
    Frame f;
    Addr loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    Addr local = f.alloc(8);
    call(g::Node_convertToNodeSpace, {local, node, loc});
    call(sfn::Vec2_dtor, {loc});
    float w = ldf(vcall(node, node::getContentSize, {node}));
    float h = ldf(vcall(node, node::getContentSize, {node}) + 4);
    Addr rect = f.alloc(16);
    call(g::Rect_ctor, {rect, 0, 0, fbits(w), fbits(h)});
    bool inside = call(g::Rect_containsPoint, {rect, local}) != 0;
    call(sfn::Vec2_dtor, {rect});
    call(sfn::Vec2_dtor, {local});
    return inside;
}

}  // namespace

// The touch end lambda's body ($_2: {view, index, view}): a tap without much vertical movement on an unselected
// part plays the tap animation, whose end ($_3) reports the selection.
struct AvatarPartsTouch {
    static void touchEnded(Addr capture, Addr touch);
    static void touchEndedCall(Addr func, const uint32_t *touch);
    static void tapDone(Addr func);
    static void touchMoved(Addr func, const uint32_t *touch);
    static uint32_t touchBegan(Addr func, const uint32_t *touch);
    static void assignTouchEnded(Addr dst, Addr capture);
};

void AvatarPartsTouch::touchEnded(Addr capture, Addr touch) {
    Self *s = rw::host_ptr<Self>(ld32(capture));
    int32_t index = itemIndexAt(s, int32_t(ld32(capture + 4)));
    s->touchIndex = index;
    if (itemCount(s) <= uint32_t(index) || !s->touching) {
        s->touching = 0;
        return;
    }
    Frame f;
    Addr loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    float dy = ldf(loc + 4) - s->touchStartY;
    float ady = std::fabs(dy);
    if (ady >= 20.0f || ady != ady) {
        call(sfn::Vec2_dtor, {loc});
        s->touching = 0;
        return;
    }
    if (itemCount(s) <= uint32_t(s->touchIndex)) {
        call(g::throwOutOfRange, {});
        return;
    }
    int32_t id = int32_t(ld32(s->itemIDs.begin + 4 * uint32_t(s->touchIndex)));
    if (id == 0) {
        call(sfn::Vec2_dtor, {loc});
        s->touching = 0;
        return;
    }
    int32_t selected = s->selectedID;
    call(sfn::Vec2_dtor, {loc});
    if (id == selected) {
        s->touching = 0;
        return;
    }
    s->tapping = 1;
    Addr cell = ld32(s->cells + 4 * ld32(capture + 4));
    withLambda(VT_tapDone_L3, {ld32(capture + 8)}, [&](Addr fn) {
        Addr anim = call(g::SceneUtil_createTapAnimation, {cell, fn});
        call(sfn::Node_runAction, {cell, anim});
    });
}
RW_FUNCTION(0x554FEC, "AvatarPartsScrollView::registItemTouchEvent()::$_2::operator()", AvatarPartsTouch::touchEnded);

void AvatarPartsTouch::touchEndedCall(Addr func, const uint32_t *touch) { touchEnded(func + 4, *touch); }
RW_FUNCTION(0x554FAA, "AvatarPartsScrollView::registItemTouchEvent()::$_2 __func::operator()",
            AvatarPartsTouch::touchEndedCall);

// $_3: the tap animation ended: the touched part becomes the selection (or is toggled in multi select) and the
// "new" marks of the part (and of a set's parts) are cleared.
void AvatarPartsTouch::tapDone(Addr func) {
    Self *s = rw::host_ptr<Self>(ld32(func + 4));
    if (s->touching) {
        uint32_t idx = uint32_t(s->touchIndex);
        if (itemCount(s) <= idx) {
            call(g::throwOutOfRange, {});
            return;
        }
        int32_t id = int32_t(ld32(s->itemIDs.begin + 4 * idx));
        Frame f;
        Addr row = f.alloc(8);
        call(g::AvatarParts_create, {row, uint32_t(id)});
        const SafeBuffer *part = rw::host_ptr<SafeBuffer>(ld32(row));
        if (s->isSet) {
            call(g::AppState_removeNewAvatarPartsIDs, {call(g::AppState_getInstance, {}), uint32_t(id)});
            call(g::AppState_removeNewAvatarPartsSetIDs, {call(g::AppState_getInstance, {}), uint32_t(id)});
            int32_t count = part->get32(0xA4);
            for (int32_t k = 0; k < count; k++) {
                int32_t member = part->get32(0xA8 + 4 * uint32_t(k));
                call(g::AppState_removeNewAvatarPartsIDs, {call(g::AppState_getInstance, {}), uint32_t(member)});
            }
        } else {
            call(g::AppState_removeNewAvatarPartsIDs, {call(g::AppState_getInstance, {}), uint32_t(id)});
            int32_t count = part->get32(0xA4);
            for (int32_t k = 0; k < count; k++) {
                int32_t member = part->get32(0xA8 + 4 * uint32_t(k));
                Frame g2;
                Addr mrow = g2.alloc(8);
                call(g::AvatarParts_create, {mrow, uint32_t(member)});
                const SafeBuffer *m = rw::host_ptr<SafeBuffer>(ld32(mrow));
                // a set member that belongs to this part's set
                if (m->get32(0x9C) != 1 && m->get32(0xA4) == 1 && m->get32(0xA8) == id)
                    call(g::AppState_removeNewAvatarPartsIDs, {call(g::AppState_getInstance, {}), uint32_t(member)});
                sharedRelease(mrow);
            }
        }
        bool toggled = false;
        if (s->selectedIDsEnabled) {
            for (uint32_t p = s->selectedIDs.begin; p != s->selectedIDs.end; p += 4)
                if (int32_t(ld32(p)) == id) { toggled = true; break; }
        } else if (!s->isSet) {
            int32_t old = s->selectedID;
            s->selectedID = id;
            toggled = old == id;
        }
        Addr me = rw::guest_addr(s);
        if (toggled) {
            if (ld32(me + 0x660)) call(g::function_vi_call, {me + 0x650, uint32_t(s->touchIndex)});
        } else {
            if (ld32(me + 0x648)) call(g::function_vi_call, {me + 0x638, uint32_t(s->touchIndex)});
        }
        call(g::IScrollProtocol_updateLayout, {me, 0});
        sharedRelease(row);
    }
    s->tapping = 0;
    s->touching = 0;
}
RW_FUNCTION(0x55523C, "AvatarPartsScrollView::registItemTouchEvent()::$_3 __func::operator()", AvatarPartsTouch::tapDone);

// $_1 (onTouchMoved): a touch that leaves its cell's panel is no tap any more (handled as a touch end).
void AvatarPartsTouch::touchMoved(Addr func, const uint32_t *touchArg) {
    Addr touch = *touchArg;
    Self *s = rw::host_ptr<Self>(ld32(func + 4));
    if (!s->touching) return;
    Addr cell = ld32(s->cells + 4 * ld32(func + 8));
    Addr panel = search(cell, S_mainPanel, 9);
    if (!panel) return;
    Frame f;
    Addr loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    Addr local = f.alloc(8);
    call(g::Node_convertToNodeSpace, {local, panel, loc});
    call(sfn::Vec2_dtor, {loc});
    float w = ldf(vcall(panel, node::getContentSize, {panel}));
    float h = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
    Addr rect = f.alloc(16);
    call(g::Rect_ctor, {rect, 0, 0, fbits(w), fbits(h)});
    if (!call(g::Rect_containsPoint, {rect, local})) {
        s->touching = 0;
        AvatarPartsTouch::touchEnded(func + 12, touch);
    }
    call(sfn::Vec2_dtor, {rect});
    call(sfn::Vec2_dtor, {local});
}
RW_FUNCTION(0x555564, "AvatarPartsScrollView::registItemTouchEvent()::$_1 __func::operator()",
            AvatarPartsTouch::touchMoved);

// $_0 (onTouchBegan): claims touches inside the list and inside a cell's panel; remembers the item and the y.
uint32_t AvatarPartsTouch::touchBegan(Addr func, const uint32_t *touchArg) {
    Addr touch = *touchArg;
    Self *s = rw::host_ptr<Self>(ld32(func + 4));
    if (s->tapping) return 0;
    Addr me = rw::guest_addr(s);
    uint32_t result = 0;
    Frame f;
    Addr loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    Addr local = f.alloc(8);
    call(g::Node_convertToNodeSpace, {local, me, loc});
    call(sfn::Vec2_dtor, {loc});
    float w = ldf(vcall(me, node::getContentSize, {me}));
    float h = ldf(vcall(me, node::getContentSize, {me}) + 4);
    Addr rect = f.alloc(16);
    call(g::Rect_ctor, {rect, 0, 0, fbits(w), fbits(h)});
    if (call(g::Rect_containsPoint, {rect, local})) {
        Addr start = f.alloc(8);
        call(g::Touch_getLocation, {start, touch});
        s->touchStartY = ldf(start + 4);
        call(sfn::Vec2_dtor, {start});
        Addr cell = ld32(s->cells + 4 * ld32(func + 8));
        Addr panel = search(cell, S_mainPanel, 9);
        if (panel) {
            Addr ploc = f.alloc(8);
            call(g::Touch_getLocation, {ploc, touch});
            Addr plocal = f.alloc(8);
            call(g::Node_convertToNodeSpace, {plocal, panel, ploc});
            call(sfn::Vec2_dtor, {ploc});
            float pw = ldf(vcall(panel, node::getContentSize, {panel}));
            float ph = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
            Addr prect = f.alloc(16);
            call(g::Rect_ctor, {prect, 0, 0, fbits(pw), fbits(ph)});
            if (call(g::Rect_containsPoint, {prect, plocal})) {
                int32_t index = itemIndexAt(s, int32_t(ld32(func + 8)));
                s->touchIndex = index;
                uint32_t count = itemCount(s);
                if (count <= uint32_t(index)) s->touchIndex = int32_t(count - 1);
                s->touching = 1;
                result = 1;
            }
            call(sfn::Vec2_dtor, {prect});
            call(sfn::Vec2_dtor, {plocal});
        }
    }
    call(sfn::Vec2_dtor, {rect});
    call(sfn::Vec2_dtor, {local});
    return result;
}
RW_FUNCTION(0x55576C, "AvatarPartsScrollView::registItemTouchEvent()::$_0 __func::operator()",
            AvatarPartsTouch::touchBegan);

// listener->onTouchEnded = $_2 (std::function<void(Touch*, Event*)>::operator=(lambda&&), outlined)
void AvatarPartsTouch::assignTouchEnded(Addr dst, Addr capture) {
    withLambda(VT_touchEnded_L2, {ld32(capture), ld32(capture + 4), ld32(capture + 8)},
               [&](Addr fn) { call(g::function_touchVoid_swap, {fn, dst}); });
}
RW_FUNCTION(0x554E78, "AvatarPartsScrollView::registItemTouchEvent()::assignTouchEnded",
            AvatarPartsTouch::assignTouchEnded);

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::construct(Self *self) {
    Addr me = rw::guest_addr(self);
    call(g::Layout_ctor, {me});
    // IScrollProtocol
    self->scrollRate = 0.1f;
    self->scrollDecay = 0.3f;
    self->scroll504[0] = 0;
    self->scroll504[1] = 0;
    self->scroll504[2] = 0;
    self->scroll510 = 200.0f;
    call(g::aeabi_memset, {me + 0x514, 0x24, 0});
    self->maxColumn = 1;
    self->scroll53C = 1;
    rw::set<uint32_t>(self, 0x590, 0);
    rw::set<uint8_t>(self, 0x594, 0);
    rw::set<uint32_t>(self, 0x5C4, 0);
    rw::set<uint32_t>(self, 0x5D8, 0);
    rw::set<uint32_t>(self, 0x5F0, 0);
    rw::set<uint32_t>(self, 0x608, 0);
    rw::set<uint32_t>(self, 0x620, 0);
    call(g::aeabi_memset, {me + 0x540, 0x12, 0});
    call(g::aeabi_memset, {me + 0x554, 0x12, 0});
    call(g::aeabi_memset, {me + 0x568, 0x25, 0});
    call(g::aeabi_memset, {me + 0x598, 0x2A, 0});
    // BaseScrollView<int>
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    // AvatarPartsScrollView
    rw::set<uint32_t>(self, 0, VTABLE);
    rw::set<uint32_t>(self, 0x280, VTABLE2);
    rw::set<uint32_t>(self, 0x350, VTABLE3);
    rw::set<uint32_t>(self, 0x648, 0);       // onSelect empty
    rw::set<uint32_t>(self, 0x660, 0);       // onToggle empty
    self->touching = 0;
    self->touchIndex = 0;
    self->tapping = 0;
    self->touchStartY = 0.0f;
    self->selectedID = 0;
    self->selectedIDsEnabled = 0;
    rw::set<uint32_t>(self, 0x690, 0);       // isSet .. showSelection
    self->selectedIDs.cap = 0;
    self->selectedIDs.end = 0;
    self->selectedIDs.begin = 0;
    return self;
}
RW_FUNCTION(0x5536CC, "AvatarPartsScrollView::AvatarPartsScrollView", AvatarPartsScrollView::construct);

Self *Self::destroy(Self *self) {
    Addr me = rw::guest_addr(self);
    rw::set<uint32_t>(self, 0, VTABLE);
    rw::set<uint32_t>(self, 0x280, VTABLE2);
    rw::set<uint32_t>(self, 0x350, VTABLE3);
    if (self->panelTemplate) call(g::Ref_release, {self->panelTemplate});
    call(g::vector_int_dtor, {me + 0x684});
    for (uint32_t fo : {me + 0x650u, me + 0x638u}) {
        uint32_t p = ld32(fo + 16);
        if (p == fo) vcall(p, 0x10, {p});
        else if (p) vcall(p, 0x14, {p});
    }
    // back to BaseScrollView<int>
    rw::set<uint32_t>(self, 0, 0xF811D8);
    rw::set<uint32_t>(self, 0x280, 0xF811D0 + 0x360);
    rw::set<uint32_t>(self, 0x350, 0xF811D0 + 0x374);
    call(g::vector_int_dtor, {me + 0x628});
    call(g::IScrollProtocol_dtor, {me});
    return self;
}
RW_FUNCTION(0x5534BC, "AvatarPartsScrollView::~AvatarPartsScrollView", AvatarPartsScrollView::destroy);

Self *Self::destroyThunk640(Addr base) { return rw::host_ptr<Self>(call(0x5534BD, {base - 0x280})); }
RW_FUNCTION(0x553624, "non-virtual thunk (-0x280) to AvatarPartsScrollView::~AvatarPartsScrollView",
            AvatarPartsScrollView::destroyThunk640);
Self *Self::destroyThunk848(Addr base) { return rw::host_ptr<Self>(call(0x5534BD, {base - 0x350})); }
RW_FUNCTION(0x553634, "non-virtual thunk (-0x350) to AvatarPartsScrollView::~AvatarPartsScrollView",
            AvatarPartsScrollView::destroyThunk848);

void Self::destroyDelete(Self *self) { call(fn::operator_delete, {call(0x5534BD, {rw::guest_addr(self)})}); }
RW_FUNCTION(0x553644, "AvatarPartsScrollView::~AvatarPartsScrollView (deleting)", AvatarPartsScrollView::destroyDelete);
void Self::destroyDeleteThunk640(Addr base) { call(fn::operator_delete, {call(0x5534BD, {base - 0x280})}); }
RW_FUNCTION(0x553650, "non-virtual thunk (-0x280) to AvatarPartsScrollView::~AvatarPartsScrollView (deleting)",
            AvatarPartsScrollView::destroyDeleteThunk640);
void Self::destroyDeleteThunk848(Addr base) { call(fn::operator_delete, {call(0x5534BD, {base - 0x350})}); }
RW_FUNCTION(0x553664, "non-virtual thunk (-0x350) to AvatarPartsScrollView::~AvatarPartsScrollView (deleting)",
            AvatarPartsScrollView::destroyDeleteThunk848);

Addr Self::create() {
    Addr view = call(fn::operator_new, {SIZE});
    call(0x5536CD, {view});                                   // AvatarPartsScrollView::AvatarPartsScrollView
    if (vcall(view, SLOT_init, {view}) == 1) {
        call(sfn::Ref_autorelease, {view});
        return view;
    }
    vcall(view, SLOT_release, {view});
    return 0;
}
RW_FUNCTION(0x553678, "AvatarPartsScrollView::create", AvatarPartsScrollView::create);

uint32_t Self::init(Self *self) {
    Addr me = rw::guest_addr(self);
    self->itemWidth = 360.0f;
    self->itemHeight = 122.0f;
    call(g::IScrollProtocol_setEnabledScrollBar, {me, 1});
    self->scrollBar570 = 2.0f;
    self->scrollBar574 = -9.0f;
    self->scrollBar57C = 2.0f;
    call(g::IScrollProtocol_setMaxColumn, {me, 2});
    Addr panel = call(g::Widget_create, {});
    self->panelTemplate = panel;
    {
        uint32_t setSize = ld32(ld32(panel) + node::setContentSize);
        Frame f;
        Addr size = f.alloc(8);
        call(sfn::Size_ctor, {size, rw::fbits(self->itemWidth), rw::fbits(self->itemHeight)});
        call(setSize, {panel, size});
    }
    vcall(self->panelTemplate, node::setAnchorPoint, {self->panelTemplate, ld32(GOT_ANCHOR_MIDDLE)});
    call(sfn::Ref_retain, {self->panelTemplate});
    Addr json = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_PanelJson});
    Addr tmpl = self->panelTemplate;
    uint32_t addChildZName = ld32(ld32(tmpl) + node::addChildZName);
    withStr(S_mainPanel, 9, [&](Addr s) { call(addChildZName, {tmpl, json, 0, s}); });
    Addr inner = search(json, S_Panel, 5);
    vcall(inner, node::setOpacity, {inner, 255});
    Addr mgr = call(g::ArmatureDataManager_getInstance, {});
    withStr(S_CursorJson, 52, [&](Addr s) { call(g::ArmatureDataManager_addArmatureFileInfo, {mgr, s}); });
    return call(g::BaseScrollView_int_init, {me});
}
RW_FUNCTION(0x55384C, "AvatarPartsScrollView::init", AvatarPartsScrollView::init);

void Self::update(Self *self, float dt) { call(g::IScrollProtocol_update, {rw::guest_addr(self), rw::fbits(dt)}); }
RW_FUNCTION(0x553A7C, "AvatarPartsScrollView::update", AvatarPartsScrollView::update);

Addr Self::getLayoutPanel(Self *self, int32_t row) {
    (void)row;
    return call(g::Widget_clone, {self->panelTemplate});
}
RW_FUNCTION(0x553A84, "AvatarPartsScrollView::getLayoutPanel", AvatarPartsScrollView::getLayoutPanel);

// A cell: a clone of the panel template with the selection cursor animation at its "Area_Point".
Addr Self::createRow(Self *self, uint32_t index, int32_t column) {
    if (itemCount(self) <= index) {
        call(g::throwOutOfRange, {});
        return 0;
    }
    Addr cell = call(g::Widget_clone, {self->panelTemplate});
    Addr cursor = withStr(S_Cursor, 22, [&](Addr s) { return call(g::Armature_create, {s}); });
    Addr anim = vcall(cursor, SLOT_getAnimation, {cursor});
    uint32_t play = ld32(ld32(anim) + SLOT_play);
    withStr(S_Animation1, 10, [&](Addr s) { call(play, {anim, s, 0xFFFFFFFFu, 0xFFFFFFFFu}); });
    Addr point = search(cell, S_Area_Point, 10);
    uint32_t addChildZName = ld32(ld32(point) + node::addChildZName);
    withStr(S_SelectedAnimation, 17, [&](Addr s) { call(addChildZName, {point, cursor, 0, s}); });
    Addr me = rw::guest_addr(self);
    vcall(me, SLOT_changeRow, {me, cell, index, uint32_t(column)});
    return cell;
}
RW_FUNCTION(0x553A94, "AvatarPartsScrollView::createRow", AvatarPartsScrollView::createRow);

// Touch handling of a cell: began ($_0), moved ($_1), ended and cancelled ($_2), swallowing nothing.
void Self::registItemTouchEvent(Self *self, Addr widget, int32_t index) {
    (void)widget;
    Addr me = rw::guest_addr(self);
    Addr dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    Addr listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    withLambda(VT_touchBegan_L0, {me, uint32_t(index)},
               [&](Addr f) { call(g::function_touchBool_swap, {f, listener + 0x50}); });
    {
        Frame f;
        Addr fo = f.alloc(20);
        st32(fo + 16, 0);
        Addr heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_touchMoved_L1);
            st32(heap + 4, me);
            st32(heap + 8, uint32_t(index));
            st32(heap + 12, me);
            st32(heap + 16, uint32_t(index));
            st32(heap + 20, me);
        }
        st32(fo + 16, heap);
        call(g::function_touchVoid_swap, {fo, listener + 0x68});
        functionDestroy(fo);
    }
    Frame f;
    Addr capture = f.alloc(12);
    st32(capture, me);
    st32(capture + 4, uint32_t(index));
    st32(capture + 8, me);
    AvatarPartsTouch::assignTouchEnded(listener + 0x80, capture);
    AvatarPartsTouch::assignTouchEnded(listener + 0x98, capture);
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, me});
}
RW_FUNCTION(0x554D38, "AvatarPartsScrollView::registItemTouchEvent", AvatarPartsScrollView::registItemTouchEvent);

void Self::setSelectedID(Self *self, int32_t id) {
    self->selectedID = id;
    call(g::IScrollProtocol_updateLayout, {rw::guest_addr(self), 0});
}
RW_FUNCTION(0x554EF0, "AvatarPartsScrollView::setSelectedID", AvatarPartsScrollView::setSelectedID);

void Self::setSelectedIDs(Self *self, Addr ids) {
    Addr me = rw::guest_addr(self);
    if (me + 0x684 != ids) call(g::vector_int_assign, {me + 0x684, ld32(ids), ld32(ids + 4)});
    call(g::IScrollProtocol_updateLayout, {me, 0});
}
RW_FUNCTION(0x554F04, "AvatarPartsScrollView::setSelectedIDs", AvatarPartsScrollView::setSelectedIDs);

void Self::setSelectedIDsEnabled(Self *self, uint32_t enabled) {
    self->selectedIDsEnabled = uint8_t(enabled);
    call(g::IScrollProtocol_updateLayout, {rw::guest_addr(self), 0});
}
RW_FUNCTION(0x554F30, "AvatarPartsScrollView::setSelectedIDsEnabled", AvatarPartsScrollView::setSelectedIDsEnabled);
