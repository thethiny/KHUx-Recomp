// ChatScrollView::changeRow: one chat message in a row (see ChatScrollView.hpp).
#include "ChatScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = ChatScrollView;

namespace {
namespace g {
constexpr uint32_t VectorAt_throw = 0x47CCE1;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t Size_copy = 0x8DF02D;
constexpr uint32_t AvatarMap_find = 0x490275;              // unordered_map<ull, int>::find (const)
constexpr uint32_t AvatarIconCache_find = 0x5CA9A9;        // (cache, ull)
constexpr uint32_t AvatarIconCache_add = 0x5CAA57;         // (cache, ull, const Style&)
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;        // (Style out, const Coordinate&, int)
constexpr uint32_t AvatarIcon_clone = 0x5CA5C1;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t Text_setTextAreaSize = 0x83A605;
constexpr uint32_t Widget_getCustomSize = 0x8262D1;
constexpr uint32_t ImageView_loadTexture = 0x839731;       // (image, const string&, TextureResType)
constexpr uint32_t Director_getTextureCache = 0x8EFE69;
constexpr uint32_t TextureCache_addImage = 0x917DA9;
constexpr uint32_t Texture2D_getContentSize = 0x914ACB;    // (Size out, texture)
constexpr uint32_t master_Stamp_create = 0x7919E5;
constexpr uint32_t ToString = 0xC95451;                    // std::to_string(int)
constexpr uint32_t String_substr = 0x47CD4D;               // std::string(const string&, pos, n)
constexpr uint32_t String_append = 0x47AC89;               // append(const char*, n)
constexpr uint32_t String_assign = 0x48D811;
constexpr uint32_t Search = 0x49A42B;                      // std::__search(first, last, pfirst, plast, bool(*&)(char, char))
constexpr uint32_t CharEqualNoCase = 0x49A419;             // sub_49A418
constexpr uint32_t ImageButton_create = 0x531239;
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Date_makeLocalDate = 0x4927CD;
constexpr uint32_t TextData_format = 0x492929;
constexpr uint32_t GetTextLineCount = 0x55852D;
}  // namespace g

constexpr uint32_t S_AreaAvatar = 0xDFECE4, S_Avatar = 0xDF8F56, S_AreaBalloon = 0xDFECF0, S_AreaStamp = 0xDFECFD,
                   S_AreaDate = 0xDFED08, S_AreaDate2 = 0xDFED12, S_TxtNameLabel = 0xDFDBB8,
                   S_AvatarUser = 0xDFED1D, S_Balloon = 0xDFED28, S_Stamp = 0xDFED30, S_Medal = 0xDFD05F,
                   S_MedalPath = 0xDFED36, S_Talks = 0xDFED49, S_StampPath = 0xDFED50, S_Png = 0xDFED65,
                   S_TxtChatLabel = 0xDFED6A, S_BalloonB1 = 0xDFED79, S_BalloonA1 = 0xDFED96,
                   S_BalloonButton = 0xDFEDB3, S_New = 0xDFD177, S_TxtDateLabel = 0xDFEC95, S_Empty = 0xDF8A1A;

constexpr int32_t TXT_UNKNOWN_USER = 104200024;
constexpr int32_t TXT_DATE = 104201025;           // month, day, hour, minute
constexpr uint32_t VT_Balloon = 0xF58C50;         // changeRow $_0 (heap 0x50) {row, mine, message copy}

constexpr uint32_t API_AVATARS = 0x1FC;           // SafeBuffer* of UserAvatar entries (0x128 bytes; +0x24: the
                                                  // unordered_map<ull, int> user id -> entry) and its control +0x200
constexpr uint32_t API_USER = 0x84;               // SafeBuffer* (+0: the user id, 8 bytes) and its control +0x88
constexpr uint32_t AVATAR_ENTRY = 0x128;
constexpr uint32_t AVATAR_COORDINATE = 0xE8;
constexpr uint32_t AVATAR_NAME = 0xC;

constexpr uint32_t V_WIDGET_SET_SIZE = 0x2B0;     // ui::Widget::setSize (the balloon's custom size)
constexpr uint32_t V_SET_SCALE_X = 0x28;
constexpr uint32_t V_SET_SCALE_XY = 0x48;

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }

// node->setPosition(Vec2(x, node's y))
void setX(uint32_t node, float x) {
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(g::Vec2_copy, {pos, vcall(node, node::getPosition, {node})});
    stf(pos, x);
    vcall(node, node::setPosition, {node, pos});
    call(g::Vec2_dtor, {pos});
}

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

// text->setString(TextData::format(TXT_DATE, month, day, hour, minute of the local date `local`))
void setDateText(uint32_t text, uint32_t local) {
    const uint32_t md = rw::load<uint16_t>(local + 2), hm = rw::load<uint16_t>(local + 4);
    Frame f;
    const uint32_t s = f.alloc(12);
    call(g::TextData_format, {s, 0, uint32_t(TXT_DATE), md & 0xFF, md >> 8, hm & 0xFF, hm >> 8});
    setTextC(text, strData(s));
    strFree(s);
}

// the medal / talk stamp image of a stamp message: loaded, sized to the master Stamp size
void loadStampImage(uint32_t image, uint32_t path, int32_t id) {
    Frame f;
    const uint32_t num = f.alloc(12);
    call(g::ToString, {num, uint32_t(id)});
    call(g::String_append, {path, strData(num), (ld8(num) & 1) ? ld32(num + 4) : uint32_t(ld8(num) >> 1)});
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
}  // namespace

// Fill `row` with message itemIDs[index]: avatar (left for others, mirrored right for the player), name, balloon
// text (sized to its lines) or stamp / medal, the button over it, the date and New marks. 1 when the row has no
// Area_Avatar (replace it), else 0.
uint32_t Self::changeRow(Self *self, Addr row, int32_t index, int32_t) {
    const uint32_t me = self->self();
    if (uint32_t(int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2) <= uint32_t(index))
        call(g::VectorAt_throw, {me + 0x628});
    const int32_t id = int32_t(ld32(self->itemIDs.begin + 4 * uint32_t(index)));
    if (!id) return 0;
    const uint32_t areaAvatar = findNode(row, S_AreaAvatar, 11);
    if (!areaAvatar) return 1;
    if (uint32_t(int32_t(self->messages.end - self->messages.begin) >> 6) <= uint32_t(id - 1))
        call(g::VectorAt_throw, {me + 0x638});
    Frame f;
    // the message (copied)
    const uint32_t src = self->messages.begin + MESSAGE_SIZE * uint32_t(id - 1);
    const uint32_t msg = f.alloc(MESSAGE_SIZE);
    for (uint32_t k = 0; k < 0x10; k += 4) st32(msg + k, ld32(src + k));
    st8(msg + 0x10, ld8(src + 0x10));
    copyString(msg + 0x14, src + 0x14);
    call(g::MEMCPY, {msg + 0x20, src + 0x20, 12});
    copyString(msg + 0x2C, src + 0x2C);
    st8(msg + 0x38, ld8(src + 0x38));
    const bool isStamp = ld8(msg + 8) != 0;
    setVisible(row, 1);
    uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t avatarCtrl = ld32(api + API_AVATARS + 4);
    const uint32_t avatars = ld32(api + API_AVATARS);
    if (avatarCtrl) call(fn::shared_add, {avatarCtrl});
    const uint32_t avatar = findNode(row, S_Avatar, 6);
    const uint32_t areaBalloon = findNode(row, S_AreaBalloon, 12);
    const uint32_t areaStamp = findNode(row, S_AreaStamp, 10);
    const uint32_t areaDate = findNode(row, S_AreaDate, 9);
    const uint32_t areaDate2 = findNode(row, S_AreaDate2, 10);
    setVisible(areaDate, 1);
    setVisible(areaDate2, 0);
    const uint32_t name = findNode(areaAvatar, S_TxtNameLabel, 14);
    setVisible(name, 1);
    SceneUtil::setShadow(name);
    api = call(fn::APIManager_getInstance, {});
    const uint32_t userCtrl = ld32(api + API_USER + 4);
    const uint32_t user = ld32(api + API_USER);
    if (userCtrl) call(fn::shared_add, {userCtrl});
    const uint32_t userLo = uint32_t(safe32(user, 0)), userHi = uint32_t(safe32(user, 4));
    const bool mine = ld32(msg) == userLo && ld32(msg + 4) == userHi;
    float iconScale;
    if (mine) {
        setX(areaAvatar, 778.0f);
        setX(areaBalloon, 258.0f);
        setX(areaDate, 0.0f);
        setX(areaDate2, 0.0f);
        setX(areaStamp, 490.0f);
        iconScale = 1.0f;
    } else {
        setX(areaAvatar, 16.0f);
        setX(areaBalloon, 146.0f);
        setX(areaDate, 546.0f);
        setX(areaDate2, 74.0f);
        setX(areaStamp, 156.0f);
        iconScale = -1.0f;
    }
    // the sender's avatar icon
    const uint32_t key = f.alloc(8);
    st32(key, ld32(msg));
    st32(key + 4, ld32(msg + 4));
    const uint32_t node = call(g::AvatarMap_find, {avatars + 0x24, key});
    if (node && int32_t(ld32(node + 0x10)) >= 0) {
        const uint32_t entry = safeBytes(f, avatars, AVATAR_ENTRY * ld32(node + 0x10), AVATAR_ENTRY);
        const uint32_t cache = self->iconCache.obj;
        uint32_t icon = call(g::AvatarIconCache_find, {cache, 0, ld32(entry), ld32(entry + 4)});
        if (!icon) {
            const uint32_t style = f.alloc(0x40);
            call(g::AvatarUtil_makeStyle, {style, entry + AVATAR_COORDINATE, 0});
            icon = call(g::AvatarIconCache_add, {self->iconCache.obj, 0, ld32(entry), ld32(entry + 4), style});
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
            const uint32_t z = vcall(avatar, 0x18, {avatar});   // getLocalZOrder
            Frame h;
            const uint32_t tag = newStr(h, S_AvatarUser, 10);
            vcall(parent, node::addChildZName, {parent, clone, z, tag});
            strFree(tag);
        }
        setVisible(avatar, 0);
        setTextC(name, entry + AVATAR_NAME);
        sizeText(name, 150.0f, 20.0f);
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
    if (isStamp) {
        setVisible(areaStamp, 1);
        setVisible(areaBalloon, 0);
        setVisible(medal, 0);
        setVisible(stamp, 0);
        const uint32_t sub = f.alloc(12);
        call(g::String_substr, {sub, msg + 0x14, 7, 0xFFFFFFFFu});
        const uint32_t path = newStr(f, S_MedalPath, 0x12);
        const uint32_t subLen = (ld8(sub) & 1) ? ld32(sub + 4) : uint32_t(ld8(sub) >> 1);
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
    // the button over the balloon / stamp, its lambda carrying the message
    uint32_t button = findNode(row, S_BalloonButton, 13);
    if (button) {
        call(g::ImageButton_setTargetView, {button, isStamp ? areaStamp : balloon, fbits(1.0f)});
    } else {
        {
            Frame h;
            const uint32_t file = newStr(h, S_Empty, 0);
            button = call(g::ImageButton_create, {file, 0});
            strFree(file);
        }
        call(g::ImageButton_setTargetView, {button, isStamp ? areaStamp : balloon, fbits(1.0f)});
        Frame h;
        const uint32_t tag = newStr(h, S_BalloonButton, 13);
        vcall(balloon, node::addChildZName, {balloon, button, 0, tag});
        strFree(tag);
    }
    {
        Frame h;
        const uint32_t text1 = h.alloc(12), text2 = h.alloc(12);
        copyString(text1, msg + 0x14);
        copyString(text2, msg + 0x2C);
        const uint32_t fo = h.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x50});
        if (heap) {
            st32(heap, VT_Balloon);
            st8(heap + 0xC, mine ? 1 : 0);
            st32(heap + 8, row);
            for (uint32_t k = 0; k < 0x10; k += 4) st32(heap + 0x10 + k, ld32(msg + k));
            st8(heap + 0x20, ld8(msg + 0x10));
            for (uint32_t k = 0; k < 12; k += 4) {
                st32(heap + 0x24 + k, ld32(text1 + k));
                st32(text1 + k, 0);
            }
            call(g::MEMCPY, {heap + 0x30, msg + 0x20, 12});
            for (uint32_t k = 0; k < 12; k += 4) {
                st32(heap + 0x3C + k, ld32(text2 + k));
                st32(text2 + k, 0);
            }
            st8(heap + 0x48, ld8(msg + 0x38));
        }
        st32(fo + 16, heap);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        strFree(text2);
        strFree(text1);
    }
    // the date and New marks
    uint32_t newMark = findNode(areaDate, S_New, 3);
    setVisible(newMark, ld8(msg + 0x10));
    const uint32_t dateLabel = findNode(areaDate, S_TxtDateLabel, 14);
    SceneUtil::setShadow(dateLabel);
    const uint32_t local = f.alloc(8);
    call(g::Date_makeLocalDate, {local, msg + 0x20});
    setDateText(dateLabel, local);
    if (isStamp) {
        newMark = findNode(areaDate2, S_New, 3);
        setVisible(newMark, ld8(msg + 0x10));
        const uint32_t dateLabel2 = findNode(areaDate2, S_TxtDateLabel, 14);
        SceneUtil::setShadow(dateLabel2);
        setDateText(dateLabel2, local);
        setVisible(areaDate, 0);
        setVisible(areaDate2, 1);
    } else {
        const int32_t lines = int32_t(call(g::GetTextLineCount, {msg + 0x14, 36, 3}));
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_copy, {pos, vcall(areaDate, node::getPosition, {areaDate})});
        stf(pos + 4, float(lines - 1) * 26.0f + 8.0f);
        vcall(areaDate, node::setPosition, {areaDate, pos});
        call(g::Vec2_dtor, {pos});
    }
    {
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_copy, {pos, vcall(newMark, node::getPosition, {newMark})});
        stf(pos, mine ? 223.0f : 153.0f);
        vcall(newMark, node::setPosition, {newMark, pos});
        const uint32_t newMark2 = findNode(areaDate2, S_New, 3);
        const uint32_t p2 = vcall(newMark2, node::getPosition, {newMark2});
        st32(pos, ld32(p2));
        st32(pos + 4, ld32(p2 + 4));
        stf(pos, mine ? 457.0f : 387.0f);
        vcall(newMark2, node::setPosition, {newMark2, pos});
        call(g::Vec2_dtor, {pos});
    }
    if (userCtrl) call(fn::shared_release, {userCtrl});
    if (avatarCtrl) call(fn::shared_release, {avatarCtrl});
    strFree(msg + 0x2C);
    strFree(msg + 0x14);
    return 0;
}
RW_FUNCTION(0x556CB8, "ChatScrollView::changeRow", ChatScrollView::changeRow);
