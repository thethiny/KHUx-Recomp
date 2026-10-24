// ChatScrollDelView: the chat message list of the delete / report screen. The same layout as ChatScrollView (a
// BaseScrollView<int> of Chat_Panel rows, 922 x 180, the messages copied in, an AvatarIconCache) with its own
// vtables; changeRow lays a message out without the date, the New marks and the balloon button.
#include "../scene_social/ChatScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using View = ChatScrollView;

namespace {
namespace g {
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff, Size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t VectorAt_throw = 0x47CCE1;
constexpr uint32_t MessageVector_dtor = 0x5568AD, MessageVector_assign = 0x55D309;
constexpr uint32_t IntVector_pushBackSlow = 0x47D709, IntVector_base_dtor = 0x478961;
constexpr uint32_t AvatarIconCache_makeShared = 0x509899;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t AvatarMap_find = 0x490275, AvatarIconCache_find = 0x5CA9A9, AvatarIconCache_add = 0x5CAA57;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105, AvatarIcon_clone = 0x5CA5C1, SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t Text_setTextAreaSize = rwlib::cocos2d::ui::Text::setTextAreaSize, Widget_getCustomSize = rwlib::cocos2d::ui::Widget::getCustomSize;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache, TextureCache_addImage = rwlib::cocos2d::TextureCache::addImage;
constexpr uint32_t Texture2D_getContentSize = rwlib::cocos2d::Texture2D::getContentSize;
constexpr uint32_t master_Stamp_create = 0x7919E5;
constexpr uint32_t ToString = 0xC95451, String_substr = 0x47CD4D, String_append = 0x47AC89, String_assign = 0x48D811;
constexpr uint32_t Search = 0x49A42B, CharEqualNoCase = 0x49A419;
constexpr uint32_t GetTextLineCount = 0x55852D;
}  // namespace g

constexpr uint32_t VTABLE = 0xF888C4, VTABLE2 = 0xF88C1C, VTABLE3 = 0xF88C30;
constexpr uint32_t SIZE = 0x650, MESSAGE_SIZE = 0x40;

constexpr uint32_t S_ChatPanelJson = 0xDFECC1, S_MainPanel = 0xDFD1FC, S_Avatar = 0xDF8F56;
constexpr uint32_t S_AreaAvatar = 0xDFECE4, S_AreaBalloon = 0xDFECF0, S_AreaStamp = 0xDFECFD, S_AreaDate = 0xDFED08;
constexpr uint32_t S_TxtNameLabel = 0xDFDBB8, S_AvatarUser = 0xDFED1D, S_Balloon = 0xDFED28, S_Stamp = 0xDFED30;
constexpr uint32_t S_Medal = 0xDFD05F, S_MedalPath = 0xDFED36, S_Talks = 0xDFED49, S_StampPath = 0xDFED50;
constexpr uint32_t S_Png = 0xDFED65, S_TxtChatLabel = 0xDFED6A, S_BalloonB1 = 0xDFED79, S_BalloonA1 = 0xDFED96;

constexpr int32_t TXT_UNKNOWN_USER = 104200024;
constexpr uint32_t API_AVATARS = 0x1FC, API_USER = 0x84;
constexpr uint32_t AVATAR_ENTRY = 0x128, AVATAR_COORDINATE = 0xE8, AVATAR_NAME = 0xC;

constexpr uint32_t V_INIT = 0x248, V_IS_REPLACE_ITEM = 0x340;
constexpr uint32_t V_GET_VIRTUAL_RENDERER = 0x2D8, V_GET_TEXTURE = 0x280;
constexpr uint32_t V_WIDGET_SET_SIZE = 0x2B0, V_SET_SCALE_X = 0x28, V_SET_SCALE_XY = 0x48, V_GET_LOCAL_Z = 0x18;

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

// the medal / talk stamp image: `path` + id + ".png", sized to the master Stamp size
void loadStampImage(uint32_t image, uint32_t path, int32_t id) {
    Frame f;
    const uint32_t num = f.alloc(12);
    call(g::ToString, {num, uint32_t(id)});
    call(g::String_append, {path, strData(num), strSize(num)});
    strFree(num);
    call(g::String_append, {path, S_Png, 4});
    call(g::ImageView_loadTexture, {image, path, 0});
    const uint32_t texture =
        call(g::TextureCache_addImage, {call(g::Director_getTextureCache, {call(fn::Director_getInstance, {})}), path});
    const uint32_t texSize = f.alloc(8);
    call(g::Texture2D_getContentSize, {texSize, texture});
    const uint32_t unused = f.alloc(12);
    call(g::ToString, {unused, uint32_t(id)});
    const uint32_t stamp = masterRow(f, g::master_Stamp_create, uint32_t(id));
    const int32_t w = safe32(ld32(stamp), 0x14);
    const int32_t h = safe32(ld32(stamp), 0x18);
    const float tw = ldf(texSize), th = ldf(texSize + 4);
    vcall(image, node::setContentSize, {image, texSize});
    const float sy = float(h) / th;
    const float sx = float(w) / tw;
    vcall(image, V_SET_SCALE_XY, {image, fbits(sx), fbits(sy)});
    sharedRelease(stamp);
    strFree(unused);
}

View *destroy(View *self) {
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    if (self->rowTemplate) call(g::Ref_release, {self->rowTemplate});
    if (self->iconCache.ctrl) call(fn::shared_release, {self->iconCache.ctrl});
    call(g::MessageVector_dtor, {self->self() + 0x638});
    BaseScrollView::destroyInt(self);
    return self;
}

uint32_t init(View *self);

uint32_t create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    View *self = rw::host_ptr<View>(me);
    BaseScrollView::constructInt(self);
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    for (uint32_t k = 0; k < 0x14; k += 4) st32(me + 0x638 + k, 0);
    if (init(self) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

// 922 x 180 rows (offset a quarter row): the invisible template holding Chat_Panel ("mainPanel"), the avatar icons
// drawn from the panel's Avatar texture at (-80, 130), scale 1.6
uint32_t init(View *self) {
    self->itemWidth = 922.0f;
    self->itemHeight = 180.0f;
    IScrollProtocol::setEnabledScrollBar(self, 1);
    self->offsetX = self->itemWidth * 0.25f;
    self->offsetY = 0.0f;
    self->rowTemplate = call(g::Widget_create, {});
    const uint32_t row = self->rowTemplate;
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fbits(float(self->itemWidth)), fbits(float(self->itemHeight))});
        vcall(row, node::setContentSize, {row, size});
    }
    vcall(self->rowTemplate, node::setAnchorPoint, {self->rowTemplate, VEC2_ANCHOR_MIDDLE});
    call(g::Ref_retain, {self->rowTemplate});
    vcall(self->rowTemplate, node::setVisible, {self->rowTemplate, 0});
    const uint32_t reader = call(fn::GUIReader_getInstance, {});
    const uint32_t panel = call(fn::GUIReader_widgetFromJsonFile, {reader, S_ChatPanelJson});
    {
        Frame f;
        const uint32_t name = newStr(f, S_MainPanel, 9);
        vcall(self->rowTemplate, node::addChildZName, {self->rowTemplate, panel, 0, name});
        strFree(name);
    }
    const uint32_t avatar = findNode(panel, S_Avatar, 6);
    const uint32_t renderer = vcall(avatar, V_GET_VIRTUAL_RENDERER, {avatar});
    Frame f;
    const uint32_t texture = f.alloc(4), pos = f.alloc(8), s1 = f.alloc(4), s2 = f.alloc(4), made = f.alloc(8);
    st32(texture, vcall(renderer, V_GET_TEXTURE, {renderer}));
    call(g::Vec2_ctor, {pos, fbits(-80.0f), fbits(130.0f)});
    stf(s1, 1.6f);
    stf(s2, 1.6f);
    call(g::AvatarIconCache_makeShared, {made, texture, pos, s1, s2});
    const uint32_t old = self->iconCache.ctrl;
    self->iconCache.obj = ld32(made);
    self->iconCache.ctrl = ld32(made + 4);
    if (old) call(fn::shared_release, {old});
    call(g::Vec2_dtor, {pos});
    return BaseScrollView::initInt(self);
}

uint32_t createRow(View *self, int32_t index, int32_t arg) {
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {self->self() + 0x628});
    const uint32_t row = call(g::Widget_clone, {self->rowTemplate});
    vcall(self->self(), V_IS_REPLACE_ITEM, {self->self(), row, uint32_t(index), uint32_t(arg)});
    return row;
}

// Fill `row` with message itemIDs[index]: the avatar (left for others, mirrored right for the player), name, the
// balloon text (sized to its lines) or stamp / medal; the date area hidden. 1 when the row has no Area_Avatar.
uint32_t changeRow(View *self, uint32_t row, int32_t index, int32_t) {
    const uint32_t me = self->self();
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {me + 0x628});
    const int32_t id = int32_t(ld32(self->itemIDs.begin + 4 * uint32_t(index)));
    if (!id) return 0;
    const uint32_t areaAvatar = findNode(row, S_AreaAvatar, 11);
    if (!areaAvatar) return 1;
    setVisible(row, 1);
    uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t avatarCtrl = ld32(api + API_AVATARS + 4);
    const uint32_t avatars = ld32(api + API_AVATARS);
    if (avatarCtrl) call(fn::shared_add, {avatarCtrl});
    if (uint32_t(int32_t(self->messages.end - self->messages.begin) >> 6) <= uint32_t(id - 1))
        call(g::VectorAt_throw, {me + 0x638});
    Frame f;
    const uint32_t src = self->messages.begin + MESSAGE_SIZE * uint32_t(id - 1);
    const uint32_t msg = f.alloc(MESSAGE_SIZE);
    for (uint32_t k = 0; k < 0x10; k += 4) st32(msg + k, ld32(src + k));
    st8(msg + 0x10, ld8(src + 0x10));
    copyString(msg + 0x14, src + 0x14);
    call(g::MEMCPY, {msg + 0x20, src + 0x20, 12});
    copyString(msg + 0x2C, src + 0x2C);
    st8(msg + 0x38, ld8(src + 0x38));
    const bool isStamp = ld8(msg + 8) != 0;
    const uint32_t avatar = findNode(row, S_Avatar, 6);
    const uint32_t areaBalloon = findNode(row, S_AreaBalloon, 12);
    const uint32_t areaStamp = findNode(row, S_AreaStamp, 10);
    const uint32_t areaDate = findNode(row, S_AreaDate, 9);
    const uint32_t name = findNode(areaAvatar, S_TxtNameLabel, 14);
    SceneUtil::setShadow(name);
    api = call(fn::APIManager_getInstance, {});
    const uint32_t userCtrl = ld32(api + API_USER + 4);
    const uint32_t user = ld32(api + API_USER);
    if (userCtrl) call(fn::shared_add, {userCtrl});
    const uint32_t userLo = uint32_t(safe32(user, 0)), userHi = uint32_t(safe32(user, 4));
    const bool mine = ld32(msg) == userLo && ld32(msg + 4) == userHi;
    // the areas placed for the sender: one position read from the avatar area, reused
    float iconScale;
    {
        Frame h;
        const uint32_t pos = h.alloc(8);
        const float x[4] = {mine ? 778.0f : 274.0f, mine ? 258.0f : 404.0f, mine ? 0.0f : 804.0f,
                            mine ? 490.0f : 414.0f};
        const uint32_t nodes[4] = {areaAvatar, areaBalloon, areaDate, areaStamp};
        call(g::Vec2_copy, {pos, vcall(areaAvatar, node::getPosition, {areaAvatar})});
        for (uint32_t k = 0; k < 4; k++) {
            if (k) {
                const uint32_t p = vcall(nodes[k], node::getPosition, {nodes[k]});
                st32(pos, ld32(p));
                st32(pos + 4, ld32(p + 4));
            }
            stf(pos, x[k]);
            vcall(nodes[k], node::setPosition, {nodes[k], pos});
        }
        call(g::Vec2_dtor, {pos});
        iconScale = mine ? 1.0f : -1.0f;
    }
    // the sender's avatar icon
    const uint32_t key = f.alloc(8);
    st32(key, ld32(msg));
    st32(key + 4, ld32(msg + 4));
    const uint32_t node = call(g::AvatarMap_find, {avatars + 0x24, key});
    if (node && int32_t(ld32(node + 0x10)) >= 0) {
        const uint32_t entry = safeBytes(f, avatars, AVATAR_ENTRY * ld32(node + 0x10), AVATAR_ENTRY);
        uint32_t icon = call(g::AvatarIconCache_find, {self->iconCache.obj, 0, ld32(entry), ld32(entry + 4)});
        if (!icon) {
            const uint32_t cache = self->iconCache.obj;
            const uint32_t style = f.alloc(0x40);
            call(g::AvatarUtil_makeStyle, {style, entry + AVATAR_COORDINATE, 0});
            icon = call(g::AvatarIconCache_add, {cache, 0, ld32(entry), ld32(entry + 4), style});
            call(g::SafeBuffer_dtor, {style});
        }
        const uint32_t clone = call(g::AvatarIcon_clone, {icon});
        vcall(clone, V_SET_SCALE_X, {clone, fbits(iconScale)});
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_copy, {pos, vcall(avatar, node::getPosition, {avatar})});
        vcall(clone, node::setPosition, {clone, pos});
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
        call(g::Vec2_dtor, {pos});
    } else {
        const uint32_t parent = vcall(avatar, node::getParent, {avatar});
        const uint32_t old = findNode(parent, S_AvatarUser, 10);
        if (old) vcall(old, node::removeFromParentAndCleanup, {old, 1});
        setVisible(avatar, 0);
        setTextId(name, TXT_UNKNOWN_USER);
        sizeText(name, 150.0f, 40.0f);
    }
    const uint32_t balloon = findNode(row, S_Balloon, 7);
    const uint32_t stamp = findNode(areaStamp, S_Stamp, 5);
    const uint32_t medal = findNode(areaStamp, S_Medal, 5);
    setVisible(medal, 0);
    if (isStamp) {
        setVisible(areaStamp, 1);
        setVisible(areaBalloon, 0);
        setVisible(medal, 0);
        setVisible(stamp, 0);
        const uint32_t sub = f.alloc(12);
        call(g::String_substr, {sub, msg + 0x14, 7, 0xFFFFFFFFu});
        const uint32_t path = newStr(f, S_MedalPath, 0x12);
        const uint32_t subLen = strSize(sub);
        const uint32_t subData = strData(sub);
        bool talk = false;
        if (subLen >= 6) {
            const uint32_t comp = f.alloc(4);
            st32(comp, g::CharEqualNoCase);
            const uint32_t end = subData + subLen;
            const uint32_t at = call(g::Search, {subData, end, S_Talks, S_Talks + 6, comp});
            talk = at != end && at - subData != 0xFFFFFFFFu;
        }
        const int32_t stampId = int32_t(ld32(msg + 0xC));
        if (talk) {
            call(g::String_assign, {path, S_StampPath, 0x14});
            setVisible(stamp, 1);
            loadStampImage(stamp, path, stampId);
        } else {
            setVisible(medal, 1);
            loadStampImage(medal, path, stampId);
        }
        strFree(path);
        strFree(sub);
    } else {
        const uint32_t label = findNode(areaBalloon, S_TxtChatLabel, 14);
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_copy, {pos, vcall(label, node::getPosition, {label})});
        {
            Frame h;
            const uint32_t file = newStr(h, mine ? S_BalloonA1 : S_BalloonB1, 0x1C);
            call(g::ImageView_loadTexture, {balloon, file, 0});
            strFree(file);
        }
        stf(pos, mine ? 15.0f : 40.0f);
        setVisible(areaStamp, 0);
        setVisible(areaBalloon, 1);
        const int32_t lines = int32_t(call(g::GetTextLineCount, {msg + 0x14, 36, 3}));
        setTextC(label, strData(msg + 0x14));
        stf(pos + 4, float(lines - 1) * 26.0f + 66.0f);
        vcall(label, node::setPosition, {label, pos});
        sizeText(label, 470.0f, float(26 * lines));
        const uint32_t size = f.alloc(8);
        call(g::Size_copy, {size, call(g::Widget_getCustomSize, {balloon})});
        stf(size + 4, float(26 * (lines - 1)) + 72.0f);
        vcall(balloon, V_WIDGET_SET_SIZE, {balloon, size});
        call(g::Vec2_dtor, {pos});
    }
    setVisible(areaDate, 0);
    if (userCtrl) call(fn::shared_release, {userCtrl});
    strFree(msg + 0x2C);
    strFree(msg + 0x14);
    if (avatarCtrl) call(fn::shared_release, {avatarCtrl});
    return 0;
}

// the messages copied in, item ids 1..n
void setMessages(View *self, uint32_t messages) {
    const uint32_t me = self->self();
    if (me + 0x638 != messages) call(g::MessageVector_assign, {me + 0x638, ld32(messages), ld32(messages + 4)});
    Frame f;
    const uint32_t ids = f.alloc(12), value = f.alloc(4);
    for (uint32_t k = 0; k < 12; k += 4) st32(ids + k, 0);
    const uint32_t count = (ld32(messages + 4) - ld32(messages)) >> 6;
    for (uint32_t k = 1; count && k <= count; k++) {
        st32(value, k);
        const uint32_t end = ld32(ids + 4);
        if (end >= ld32(ids + 8)) {
            call(g::IntVector_pushBackSlow, {ids, value});
        } else {
            if (end) st32(end, k);
            st32(ids + 4, end + 4);
        }
        if (k >= (ld32(messages + 4) - ld32(messages)) >> 6) break;
    }
    BaseScrollView::setItemIDsInt(self, ids);
    call(g::IntVector_base_dtor, {ids});
}
}  // namespace

RW_FUNCTION(0x558A94, "ChatScrollDelView::~ChatScrollDelView", destroy);
RW_FUNCTION(0x558BE0, "ChatScrollDelView::create", create);
RW_FUNCTION(0x558C74, "ChatScrollDelView::init", init);
RW_FUNCTION(0x558ECC, "ChatScrollDelView::createRow", createRow);
RW_FUNCTION(0x558F28, "ChatScrollDelView::changeRow", changeRow);
RW_FUNCTION(0x559FE4, "ChatScrollDelView::setMessages", setMessages);
