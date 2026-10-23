// Batch #8 chat leftovers: ChatBlockListScrollView (the blocked players list: a BaseScrollView<int> of
// Chat_Post_BlockPanel rows with the player's avatar, name, release button and block date), ChatScrollView::
// getMessageId, ChatStampListScrollView::setAllTouchEnabled and ChatEditBoxMessageSend::editBoxEditingDidEnd.
//
// ChatBlockListScrollView (0x668): BaseScrollView<int>, +0x638 std::function, +0x650 ui::Widget* row template
// (retained), +0x654 std::vector<ChatBlock>, +0x660 std::shared_ptr<AvatarIconCache>.
// ChatBlock (0x1C): +0 int user id, +4 bool released, +6 Date, +0x10 std::string.
#include "../scene_social/ChatScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Ref_retain = 0x8FD0B5, Ref_release = 0x8FD0BD, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Widget_create = 0x825875, Widget_clone = 0x8272B9;
constexpr uint32_t Size_ctor = 0x8DF027, Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t VectorAt_throw = 0x47CCE1;
constexpr uint32_t BlockVector_dtor = 0x55A989, BlockVector_assign = 0x55CD61;
constexpr uint32_t IntVector_pushBackSlow = 0x47D709, IntVector_base_dtor = 0x478961;
constexpr uint32_t AvatarIconCache_makeShared = 0x509899;
constexpr uint32_t AvatarMap_find = 0x490275, AvatarIconCache_find = 0x5CA9A9, AvatarIconCache_add = 0x5CAA57;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105, AvatarIcon_clone = 0x5CA5C1, SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t Text_setTextAreaSize = 0x83A605;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Date_makeLocalDate = 0x4927CD, TextData_format = 0x492929;
constexpr uint32_t EditBox_getText = 0x85520D, SceneUtil_editBoxEditingDidEnd = 0x45C61D;
}  // namespace g

constexpr uint32_t VTABLE = 0xF88FDC, VTABLE2 = 0xF89334, VTABLE3 = 0xF89348;
constexpr uint32_t SIZE = 0x668, BLOCK_SIZE = 0x1C;
constexpr uint32_t F_FUNCTION = 0x638, F_ROW = 0x650, F_BLOCKS = 0x654, F_ICONS = 0x660;

constexpr uint32_t S_BlockJson = 0xDFEE23, S_MainPanel = 0xDFD1FC, S_Avatar = 0xDF8F56;
constexpr uint32_t S_TxtName = 0xDFDA10, S_AvatarUser = 0xDFED1D, S_TxtRelease = 0xDF9CD9;
constexpr uint32_t S_ButtonRelease = 0xDF9CCA, S_HalfContentImageButton = 0xDFEDFB, S_HalfContentBut = 0xDFEE13;
constexpr uint32_t S_TxtDate = 0xDFEACC;

constexpr int32_t TXT_UNKNOWN_USER = 104200024, TXT_RELEASED = 102800001, TXT_RELEASE = 104200004;
constexpr int32_t TXT_DATE = 104201027;      // year, month, day
constexpr uint32_t VT_Release = 0xF58BF8;    // the release button {this, block words, date half, name} (heap 0x24)
constexpr uint32_t API_AVATARS = 0x1FC;
constexpr uint32_t AVATAR_ENTRY = 0x128, AVATAR_COORDINATE = 0xE8, AVATAR_NAME = 0xC;

constexpr uint32_t V_IS_REPLACE_ITEM = 0x340, V_GET_VIRTUAL_RENDERER = 0x2D8, V_GET_TEXTURE = 0x280;
constexpr uint32_t V_SET_SCALE_X = 0x28, V_GET_LOCAL_Z = 0x18, V_SET_TOUCH_ENABLED = 0x29C;

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }

void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}

void sizeText(uint32_t text, float w, float h) {
    Frame f;
    const uint32_t size = f.alloc(8);
    call(g::Size_ctor, {size, fbits(w), fbits(h)});
    call(g::Text_setTextAreaSize, {text, size});
    const uint32_t size2 = f.alloc(8);
    call(g::Size_ctor, {size2, fbits(w), fbits(h)});
    vcall(text, node::setContentSize, {text, size2});
}

BaseScrollView *destroy(BaseScrollView *self) {
    const uint32_t me = self->self();
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    if (ld32(me + F_ROW)) call(g::Ref_release, {ld32(me + F_ROW)});
    if (ld32(me + F_ICONS + 4)) call(fn::shared_release, {ld32(me + F_ICONS + 4)});
    call(g::BlockVector_dtor, {me + F_BLOCKS});
    functionDestroy(me + F_FUNCTION);
    BaseScrollView::destroyInt(self);
    return self;
}

uint32_t init(BaseScrollView *self);

uint32_t create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    BaseScrollView *self = rw::host_ptr<BaseScrollView>(me);
    BaseScrollView::constructInt(self);
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    st32(me + F_FUNCTION + 16, 0);
    for (uint32_t k = 0; k < 0x14; k += 4) st32(me + F_BLOCKS + k, 0);
    if (init(self) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

// 735 x 154 rows (offset half a row, 10 down): the invisible template holding Chat_Post_BlockPanel ("mainPanel"),
// the avatar icons drawn from its Avatar texture at (-85, 130), scale 1.6
uint32_t init(BaseScrollView *self) {
    const uint32_t me = self->self();
    self->itemWidth = 735.0f;
    self->itemHeight = 154.0f;
    IScrollProtocol::setEnabledScrollBar(self, 1);
    self->offsetX = self->itemWidth * 0.5f;
    self->offsetY = 10.0f;
    st32(me + F_ROW, call(g::Widget_create, {}));
    const uint32_t row = ld32(me + F_ROW);
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fbits(float(self->itemWidth)), fbits(float(self->itemHeight))});
        vcall(row, node::setContentSize, {row, size});
    }
    vcall(ld32(me + F_ROW), node::setAnchorPoint, {ld32(me + F_ROW), VEC2_ANCHOR_MIDDLE});
    call(g::Ref_retain, {ld32(me + F_ROW)});
    vcall(ld32(me + F_ROW), node::setVisible, {ld32(me + F_ROW), 0});
    const uint32_t reader = call(fn::GUIReader_getInstance, {});
    const uint32_t panel = call(fn::GUIReader_widgetFromJsonFile, {reader, S_BlockJson});
    {
        const uint32_t template_ = ld32(me + F_ROW);
        Frame f;
        const uint32_t name = newStr(f, S_MainPanel, 9);
        vcall(template_, node::addChildZName, {template_, panel, 0, name});
        strFree(name);
    }
    const uint32_t avatar = findNode(panel, S_Avatar, 6);
    const uint32_t renderer = vcall(avatar, V_GET_VIRTUAL_RENDERER, {avatar});
    Frame f;
    const uint32_t texture = f.alloc(4), pos = f.alloc(8), s1 = f.alloc(4), s2 = f.alloc(4), made = f.alloc(8);
    st32(texture, vcall(renderer, V_GET_TEXTURE, {renderer}));
    call(g::Vec2_ctor, {pos, fbits(-85.0f), fbits(130.0f)});
    stf(s1, 1.6f);
    stf(s2, 1.6f);
    call(g::AvatarIconCache_makeShared, {made, texture, pos, s1, s2});
    const uint32_t old = ld32(me + F_ICONS + 4);
    st32(me + F_ICONS, ld32(made));
    st32(me + F_ICONS + 4, ld32(made + 4));
    if (old) call(fn::shared_release, {old});
    call(g::Vec2_dtor, {pos});
    return BaseScrollView::initInt(self);
}

uint32_t createRow(BaseScrollView *self, int32_t index, int32_t arg) {
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {self->self() + 0x628});
    const uint32_t row = call(g::Widget_clone, {ld32(self->self() + F_ROW)});
    vcall(self->self(), V_IS_REPLACE_ITEM, {self->self(), row, uint32_t(index), uint32_t(arg)});
    return row;
}

// Fill `row` with block itemIDs[index]: the player's avatar (mirrored) and name, Release / Released, the release
// button's lambda (carrying the block), the block date. 1 when the row has no Txt_Name.
uint32_t changeRow(BaseScrollView *self, uint32_t row, int32_t index, int32_t) {
    const uint32_t me = self->self();
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {me + 0x628});
    const int32_t id = int32_t(ld32(self->itemIDs.begin + 4 * uint32_t(index)));
    if (!id) return 0;
    const uint32_t name = findNode(row, S_TxtName, 8);
    if (!name) return 1;
    setVisible(row, 1);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t avatarCtrl = ld32(api + API_AVATARS + 4);
    const uint32_t avatars = ld32(api + API_AVATARS);
    if (avatarCtrl) call(fn::shared_add, {avatarCtrl});
    const uint32_t begin = ld32(me + F_BLOCKS);
    if ((ld32(me + F_BLOCKS + 4) - begin) / BLOCK_SIZE <= uint32_t(id - 1)) call(g::VectorAt_throw, {me + F_BLOCKS});
    Frame f;
    const uint32_t src = begin + BLOCK_SIZE * uint32_t(id - 1);
    const uint32_t block = f.alloc(BLOCK_SIZE);
    for (uint32_t k = 0; k < 0xC; k += 4) st32(block + k, ld32(src + k));
    rw::save<uint16_t>(block + 0xC, rw::load<uint16_t>(src + 0xC));
    copyString(block + 0x10, src + 0x10);
    SceneUtil::setShadow(name);
    const uint32_t avatar = findNode(row, S_Avatar, 6);
    const uint32_t key = f.alloc(8);
    st32(key, ld32(block));
    st32(key + 4, uint32_t(int32_t(ld32(block)) >> 31));
    const uint32_t node = call(g::AvatarMap_find, {avatars + 0x24, key});
    if (node && int32_t(ld32(node + 0x10)) >= 0) {
        const uint32_t entry = safeBytes(f, avatars, AVATAR_ENTRY * ld32(node + 0x10), AVATAR_ENTRY);
        uint32_t icon = call(g::AvatarIconCache_find, {ld32(me + F_ICONS), 0, ld32(entry), ld32(entry + 4)});
        if (!icon) {
            const uint32_t cache = ld32(me + F_ICONS);
            const uint32_t style = f.alloc(0x40);
            call(g::AvatarUtil_makeStyle, {style, entry + AVATAR_COORDINATE, 0});
            icon = call(g::AvatarIconCache_add, {cache, 0, ld32(entry), ld32(entry + 4), style});
            call(g::SafeBuffer_dtor, {style});
        }
        const uint32_t clone = call(g::AvatarIcon_clone, {icon});
        vcall(clone, V_SET_SCALE_X, {clone, fbits(-1.0f)});
        vcall(clone, node::setPosition, {clone, vcall(avatar, node::getPosition, {avatar})});
        const uint32_t parent = vcall(avatar, node::getParent, {avatar});
        const uint32_t old = findNode(parent, S_AvatarUser, 10);
        if (old) vcall(old, node::removeFromParentAndCleanup, {old, 1});
        {
            const uint32_t z = vcall(avatar, V_GET_LOCAL_Z, {avatar});
            Frame h;
            const uint32_t tag = newStr(h, S_AvatarUser, 10);
            vcall(parent, node::addChildZName, {parent, clone, z, tag});
            strFree(tag);
        }
        setVisible(avatar, 0);
        setTextC(name, entry + AVATAR_NAME);
        sizeText(name, 150.0f, 20.0f);
    } else {
        const uint32_t parent = vcall(avatar, node::getParent, {avatar});
        const uint32_t old = findNode(parent, S_AvatarUser, 10);
        if (old) vcall(old, node::removeFromParentAndCleanup, {old, 1});
        setVisible(avatar, 0);
        setTextId(name, TXT_UNKNOWN_USER);
        sizeText(name, 150.0f, 40.0f);
    }
    const uint32_t release = findNode(row, S_TxtRelease, 11);
    SceneUtil::setShadow(release);
    setTextId(release, ld8(block + 4) ? TXT_RELEASED : TXT_RELEASE);
    const uint32_t button = findNode(row, S_ButtonRelease, 14);
    uint32_t imageButton = findNode(row, S_HalfContentImageButton, 0x17);
    if (!imageButton) {
        {
            Frame h;
            const uint32_t file = newStr(h, S_HalfContentBut, 0xF);
            imageButton = call(g::ImageButton_create, {file, 0});
            strFree(file);
        }
        call(g::ImageButton_setTargetView, {imageButton, button, fbits(1.0f)});
        Frame h;
        const uint32_t tag = newStr(h, S_HalfContentImageButton, 0x17);
        vcall(button, node::addChildZName, {button, imageButton, 0, tag});
        strFree(tag);
    }
    {
        Frame h;
        const uint32_t text = h.alloc(12);
        copyString(text, block + 0x10);
        const uint32_t fo = h.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x24});
        if (heap) {
            st32(heap, VT_Release);
            for (uint32_t k = 0; k < 0xC; k += 4) st32(heap + 8 + k, ld32(block + k));
            rw::save<uint16_t>(heap + 0x14, rw::load<uint16_t>(block + 0xC));
            for (uint32_t k = 0; k < 12; k += 4) {
                st32(heap + 0x18 + k, ld32(text + k));
                st32(text + k, 0);
            }
            st32(heap + 4, me);
        }
        st32(fo + 16, heap);
        call(g::ImageButton_addTouchEventListener, {imageButton, fo});
        functionDestroy(fo);
        strFree(text);
    }
    const uint32_t date = findNode(row, S_TxtDate, 8);
    SceneUtil::setShadow(date);
    const uint32_t local = f.alloc(8);
    call(g::Date_makeLocalDate, {local, block + 6});
    {
        const uint32_t md = rw::load<uint16_t>(local + 2);
        Frame h;
        const uint32_t s = h.alloc(12);
        call(g::TextData_format, {s, 0, uint32_t(TXT_DATE), rw::load<uint16_t>(local), md & 0xFF, md >> 8});
        setTextC(date, strData(s));
        strFree(s);
    }
    strFree(block + 0x10);
    if (avatarCtrl) call(fn::shared_release, {avatarCtrl});
    return 0;
}

// the blocks copied in, item ids 1..n
void setBlockList(BaseScrollView *self, uint32_t blocks) {
    const uint32_t me = self->self();
    if (me + F_BLOCKS != blocks) call(g::BlockVector_assign, {me + F_BLOCKS, ld32(blocks), ld32(blocks + 4)});
    Frame f;
    const uint32_t ids = f.alloc(12), value = f.alloc(4);
    for (uint32_t k = 0; k < 12; k += 4) st32(ids + k, 0);
    if (ld32(blocks + 4) != ld32(blocks)) {
        for (uint32_t k = 1;; k++) {
            st32(value, k);
            const uint32_t end = ld32(ids + 4);
            if (end >= ld32(ids + 8)) {
                call(g::IntVector_pushBackSlow, {ids, value});
            } else {
                if (end) st32(end, k);
                st32(ids + 4, end + 4);
            }
            if (k >= (ld32(blocks + 4) - ld32(blocks)) / BLOCK_SIZE) break;
        }
    }
    BaseScrollView::setItemIDsInt(self, ids);
    call(g::IntVector_base_dtor, {ids});
}

// ChatScrollView::getMessageId(index): the message number of message `index`, the last one's when past the end
// (out of range throws; none: 0)
uint32_t getMessageId(ChatScrollView *self, uint32_t index) {
    const uint32_t begin = self->messages.begin;
    const int32_t bytes = int32_t(self->messages.end - begin);
    if (bytes < 0x40) return 0;
    const uint32_t count = uint32_t(bytes >> 6);
    if (int32_t(count - 1) <= int32_t(index)) {
        if (count) return ld32(begin + ((count - 1) << 6) + 0x28);
    } else if (count > index) {
        return ld32(begin + (index << 6) + 0x28);
    }
    call(g::VectorAt_throw, {self->self() + 0x638});
    return 0;
}

// ChatStampListScrollView::setAllTouchEnabled(bool): every stamp button (vector at +0x590)
uint32_t setAllTouchEnabled(uint32_t self, uint32_t enabled) {
    uint32_t r = self;
    const uint32_t vec = self + 0x590;
    if (int32_t(ld32(vec + 4) - ld32(vec)) >= 1) {
        int32_t k = 0;
        do {
            const uint32_t button = ld32(ld32(vec) + 4 * uint32_t(k));
            vcall(button, V_SET_TOUCH_ENABLED, {button, enabled});
            r = uint32_t(int32_t(ld32(vec + 4) - ld32(vec)) >> 2);
            k++;
        } while (k < int32_t(r));
    }
    return r;
}

// ChatEditBoxMessageSend::editBoxEditingDidEnd(box): the text shown in the label (+0x2A8 node, +0x2AC label), then
// the done callback (+0x290) when set
uint32_t editBoxEditingDidEnd(uint32_t self, uint32_t box) {
    const uint32_t label = ld32(self + 0x2AC);
    const uint32_t text = call(g::EditBox_getText, {box});
    call(g::SceneUtil_editBoxEditingDidEnd, {self + 0x2A8, label, text});
    if (!ld32(self + 0x2A0)) return 0;
    return call(fn::function_vv_call, {self + 0x290});
}
}  // namespace

RW_FUNCTION(0x55A84C, "ChatBlockListScrollView::~ChatBlockListScrollView", destroy);
RW_FUNCTION(0x55AA28, "ChatBlockListScrollView::create", create);
RW_FUNCTION(0x55AAC4, "ChatBlockListScrollView::init", init);
RW_FUNCTION(0x55AD1C, "ChatBlockListScrollView::createRow", createRow);
RW_FUNCTION(0x55AD74, "ChatBlockListScrollView::changeRow", changeRow);
RW_FUNCTION(0x55B7A0, "ChatBlockListScrollView::setBlockList", setBlockList);
RW_FUNCTION(0x558A34, "ChatScrollView::getMessageId", getMessageId);
RW_FUNCTION(0x55C5C0, "ChatStampListScrollView::setAllTouchEnabled", setAllTouchEnabled);
RW_FUNCTION(0x55C75E, "ChatEditBoxMessageSend::editBoxEditingDidEnd", editBoxEditingDidEnd);
