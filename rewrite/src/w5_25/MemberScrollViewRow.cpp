// MemberScrollView::changeRow(row, index, ...): fills a PartyTop_JoinPanel row. The row's user (a SupportUser record,
// 0x124 bytes) comes from the list's source: 0 the party members, 1 the second user list (both: user id -> record
// index map at +0x24 of the users SafeBuffer), 2 the invitations sent and 3 the applications received (an
// Offer / Applicant vector whose entry gives the user id and the request date). For a member that is not in the
// list any more only the buttons and the title panel are updated. Otherwise: the avatar icon from the list's
// AvatarIconCache (moved into the row in place of the last one, 75 points up over the plain Avatar the first
// time), its touch button ($_0), name, Lux tier, avatar level, the medal (image, level, touch button $_1), the last
// login / request / application date; then the buttons of the mode (Invitation $_2 in 1, Check $_3 in 0, Cancel
// Invitation $_4 in 2, Approval $_5 and Delete $_6 in 3), the title panel and the Leader / Sub marks.
//
// The original frame (0x1D44 bytes below its five pushed registers) is kept: the touch lambdas $_0 / $_1 copy 0x128 bytes of the 0x124-byte record,
// so their last word is the stack word above it, never written here.
#include "../core/Guest.hpp"
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;          // vector::at's throw (this)
constexpr uint32_t HashULLInt_find = 0x490275;          // unordered_map<ull, int>::find(const ull&) -> node (+0x10 value)
constexpr uint32_t OfferVector_copy = 0x5232D1, OfferVector_dtor = 0x4B3E5D;
constexpr uint32_t ApplicantVector_copy = 0x523325, ApplicantVector_dtor = 0x4B4453;
constexpr uint32_t AvatarIconCache_find = 0x5CA9A9, AvatarIconCache_add = 0x5CAA57;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;      // (Style* out, const Coordinate&, int)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t Vec2_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17;
constexpr uint32_t ImageButton_create = 0x531239;       // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;        // (button, Node*, float)
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;   // (button, ImageView*)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41;
constexpr uint32_t SceneUtil_updateUserTitlePanel = 0x46ED85;   // (Node*, const SupportUser&)
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t String_assignN = 0x48D811;           // std::string::assign(const char*, size_t)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;      // (sret, mgr, ull id, int, Medal by value)
constexpr uint32_t MedalDataManager_getMedalSmallImagePath = 0x76B17D;   // (mgr, shared_ptr by value) -> char*
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5, Date_makeLocalDate = 0x4927CD;
constexpr uint32_t createTimeAgoString = 0x492BE9;      // (string out, long seconds)
constexpr uint32_t TextData_format = 0x492929;          // (string out, type, id, ...)
constexpr uint32_t memcpy = 0x40E164;
constexpr uint32_t GOT_ImageButtonScale = 0xFD65F4;     // &float: the medal button's target scale
}  // namespace g

constexpr uint32_t S_Plate_Join = 0xDFDBAD, S_Avatar = 0xDF8F56, S_Avatar_Icon_Obj = 0xDF8F5D,
                   S_ImageButton_Avatar = 0xDFD9F4, S_Empty = 0xDF8A1A;
constexpr uint32_t S_Txt_Name_Label = 0xDFDBB8, S_Txt_Lux_Label = 0xDFDBC7, S_Txt_AvatarLV = 0xDFDBD5,
                   S_AtlasLabel_AvatarLV_Label = 0xDFDB93;
constexpr uint32_t S_Medal = 0xDFD05F, S_Txt_MedalLV_Label = 0xDFDBE2, S_d = 0xDF86DE,
                   S_ImageButton_Medal = 0xDFDBF4, S_Txt_MedalLV = 0xDFDC06;
constexpr uint32_t S_Txt_RequestDate_Label = 0xDFDA9D, S_Txt_LastLogin_Label = 0xDFDC12,
                   S_Txt_Reception_Label = 0xDFDAB3, S_s_s = 0xDFDAC7;
constexpr uint32_t S_But01 = 0xDFD027, S_Txt = 0xDF94D1, S_Leader = 0xDFDC86, S_Sub = 0xDFDC8D;

constexpr int32_t TXT_AVATAR_LV = 100200001, TXT_MEDAL_LV = 100200002;
constexpr int32_t TXT_LUX[6] = {103800011, 103830005, 103830006, 103830007, 103830008, 103830009};
constexpr int32_t TXT_LAST_LOGIN = 102100008, TXT_REQUESTED = 103800051, TXT_APPLIED = 103800050,
                  TXT_DATE_FORMAT = 103801063;

// a mode button: Button_*, its ImageButton_*, the label and the touch lambda ({row, this, id} on the heap)
struct ModeButton {
    uint32_t name, nameLen, image, imageLen;
    int32_t text;
    uint32_t vt;
    uint32_t mode;
};
constexpr ModeButton BUTTONS[5] = {
    {0xDFDC26, 17, 0xDFDC38, 22, 103800033, 0xF56EEC, 1},   // Button_Invitation $_2
    {0xDFDC4F, 12, 0xDFDC5C, 17, 103800053, 0xF56E94, 0},   // Button_Check $_3
    {0xDFDC6E, 23, 0xDFDB23, 28, 103800037, 0xF56E3C, 2},   // Button_CancelInvitation $_4
    {0xDFDAF1, 15, 0xDFDB40, 20, 103800030, 0xF56DE4, 3},   // Button_Approval $_5
    {0xDFDB01, 13, 0xDFDB55, 18, 103800031, 0xF56D8C, 3},   // Button_Delete $_6
};
constexpr uint32_t VT_AvatarTouch = 0xF56FC8;   // $_0 0x140 {+8 row, this, ull id, record (0x128 bytes)}
constexpr uint32_t VT_MedalTouch = 0xF56F44;    // $_1 0x138 {+8 row, this, record (0x128 bytes)}

// MemberScrollView
constexpr uint32_t F_ITEM_IDS = 0x628, F_TYPE = 0x6F0, F_ICONS = 0x708;
// APIManager: the users of modes 0 / 1, the invitation / application lists of modes 2 / 3, the party
constexpr uint32_t API_MEMBERS = 0x1FC, API_USERS2 = 0x134, API_INVITES = 0x230, API_APPLICANTS = 0x220,
                   API_PARTY = 0x84;
constexpr uint32_t USERS_INDEX = 0x24;         // unordered_map<ull, int> in the users SafeBuffer
constexpr uint32_t LIST_USERS = 0xC;           // PartyInviteList / PartyApplication: shared_ptr<users> after the vector
constexpr uint32_t ENTRY_SIZE = 0x38, ENTRY_DATE = 0x20;
constexpr uint32_t PARTY_LEADER = 0x58, PARTY_SUBS = 0x60;
// SupportUser
constexpr uint32_t REC_SIZE = 0x124, REC_READ = 0x128, REC_LEVEL = 0x08, REC_NAME = 0x0C, REC_MEDAL = 0x48,
                   REC_MEDAL_LV = 0x54, REC_COORDINATE = 0xE8, REC_LOGIN = 0x118, REC_LUX = 0x120;
constexpr uint32_t MEDAL_SIZE = 0x48;
// the original frame and the places of the record and its copies in it
constexpr uint32_t FRAME = 0x1D58, AT_RECORD = 0x19C0, AT_KEY = 0x1AE8, AT_AVATAR_COPY = 0x1AE8,
                   AT_MEDAL_COPY = 0x1C10;
constexpr float AVATAR_RAISE = 75.0f;

constexpr uint32_t V_SET_VISIBLE = 0xB0, V_GET_PARENT = 0x11C, V_REMOVE_FROM_PARENT = 0x124,
                   V_ADD_CHILD_Z_NAME = 0xFC, V_GET_LOCAL_Z_ORDER = 0x18;

uint32_t ld16_(uint32_t a) { return rw::load<uint16_t>(a); }
void release(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}
void setVisible(uint32_t node, uint32_t v) { vcall(node, V_SET_VISIBLE, {node, v}); }
// parent->addChild(child, z, literal name)
void addNamed(uint32_t parent, uint32_t child, uint32_t z, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    vcall(parent, V_ADD_CHILD_Z_NAME, {parent, child, z, s});
    strFree(s);
}
// ImageButton::create(std::string(literal), false)
uint32_t newImageButton(uint32_t text, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, text, len);
    uint32_t button = call(g::ImageButton_create, {s, 0});
    strFree(s);
    return button;
}
// users->read(the record at index), 0x128 bytes, then its 0x124 into the record
void readRecord(uint32_t users, int32_t index, uint32_t tmp, uint32_t record) {
    const SafeBuffer *s = safe(users);
    int32_t at = index * int32_t(REC_READ);
    SafeBuffer::read(s, rw::host_ptr<uint8_t>(tmp), s->data.get() + at, REC_READ, uint32_t(at));
    call(g::memcpy, {record, tmp, REC_SIZE});
}
int32_t findIndex(uint32_t users, uint32_t key) {
    uint32_t node = call(g::HashULLInt_find, {users + USERS_INDEX, key});
    return node ? int32_t(ld32(node + 0x10)) : -1;
}
// text->setString(TextData `id` " " str), or the date variants
void setTwoParts(uint32_t text, uint32_t first, uint32_t second) {
    Frame f;
    uint32_t buf = stringBuffer(f, {S_s_s, first, second});
    setTextC(text, buf);
}

struct MemberScrollViewRow {
    static uint32_t changeRow(uint32_t self, uint32_t row, uint32_t index) {
        guest::Frame frame(FRAME);
        const uint32_t record = frame.addr(AT_RECORD), key = frame.addr(AT_KEY);
        uint32_t idsBegin = ld32(self + F_ITEM_IDS), idsEnd = ld32(self + F_ITEM_IDS + 4);
        if (!((idsEnd - idsBegin) >> 3 > index)) call(g::ThrowOutOfRange, {self + F_ITEM_IDS});
        const uint32_t id = idsBegin + 8 * index;
        if (!findNode(row, S_Plate_Join, 10)) return 1;

        const uint32_t mode = ld32(self + F_TYPE);
        Frame f;
        uint32_t date = f.alloc(8);
        bool skipToButtons = false;
        if (mode == 0 || mode == 1) {
            uint32_t api = call(fn::APIManager_getInstance, {});
            uint32_t at = mode == 0 ? API_MEMBERS : API_USERS2;
            uint32_t users = ld32(api + at), ctrl = ld32(api + at + 4);
            if (ctrl) call(fn::shared_add, {ctrl});
            st32(key, ld32(id));
            st32(key + 4, ld32(id + 4));
            int32_t i = findIndex(users, key);
            readRecord(users, i, frame.addr(mode == 0 ? 0x1898 : 0x1770), record);
            st32(date, ld32(record + REC_LOGIN));
            st32(date + 4, ld32(record + REC_LOGIN + 4));
            release(ctrl);
            skipToButtons = i == -1;
        } else if (mode == 2 || mode == 3) {
            uint32_t api = call(fn::APIManager_getInstance, {});
            uint32_t at = mode == 2 ? API_INVITES : API_APPLICANTS;
            uint32_t list = ld32(api + at), ctrl = ld32(api + at + 4);
            if (ctrl) call(fn::shared_add, {ctrl});
            uint32_t entries = f.alloc(12);
            call(mode == 2 ? g::OfferVector_copy : g::ApplicantVector_copy, {entries, list});
            uint32_t users = ld32(list + LIST_USERS), usersCtrl = ld32(list + LIST_USERS + 4);
            if (usersCtrl) call(fn::shared_add, {usersCtrl});
            auto count = [&]() { return ((ld32(entries + 4) - ld32(entries)) >> 3) * 0xB6DB6DB7u; };
            uint32_t n = ld32(id);
            if (!(count() > n)) call(g::ThrowOutOfRange, {entries});
            uint32_t entry = ld32(entries) + ENTRY_SIZE * n;
            st32(key, ld32(entry));
            st32(key + 4, ld32(entry + 4));
            readRecord(users, findIndex(users, key), frame.addr(mode == 2 ? 0x1648 : 0x1520), record);
            if (!(count() > n)) call(g::ThrowOutOfRange, {entries});
            entry = ld32(entries) + ENTRY_SIZE * n;
            st32(date, ld32(entry + ENTRY_DATE));
            st32(date + 4, ld32(entry + ENTRY_DATE + 4));
            release(usersCtrl);
            call(mode == 2 ? g::OfferVector_dtor : g::ApplicantVector_dtor, {entries});
            release(ctrl);
        }
        if (!skipToButtons) user(self, row, record, id, date, mode);
        buttons(self, row, id, mode);
        call(g::SceneUtil_updateUserTitlePanel, {row, record});
        marks(row, record);
        return 0;
    }

    // the avatar icon, name, Lux tier, levels, medal and dates
    static void user(uint32_t self, uint32_t row, uint32_t record, uint32_t id, uint32_t date, uint32_t mode) {
        uint32_t avatar = findNode(row, S_Avatar, 6);
        uint32_t current = findNode(row, S_Avatar_Icon_Obj, 15);
        const uint32_t lo = ld32(record), hi = ld32(record + 4);
        uint32_t icon = call(g::AvatarIconCache_find, {ld32(self + F_ICONS), record, lo, hi});
        auto addIcon = [&]() {
            Frame f;
            uint32_t style = f.alloc(32);
            call(g::AvatarUtil_makeStyle, {style, record + REC_COORDINATE, 0});
            uint32_t made;
            {
                Frame args;
                args.alloc(8);
                made = uint32_t(rw::call_exact(g::AvatarIconCache_add, {ld32(self + F_ICONS), 0, lo, hi, style}));
            }
            call(g::SafeBuffer_dtor, {style});
            return made;
        };
        if (current) {
            if (!icon) icon = addIcon();
            if (icon != current) {
                vcall(icon, node::setPosition, {icon, vcall(current, node::getPosition, {current})});
                uint32_t parent = vcall(current, V_GET_PARENT, {current});
                vcall(icon, V_REMOVE_FROM_PARENT, {icon});
                addNamed(parent, icon, vcall(current, V_GET_LOCAL_Z_ORDER, {current}), S_Avatar_Icon_Obj, 15);
                vcall(current, V_REMOVE_FROM_PARENT, {current});
            }
        } else {
            if (!icon) icon = addIcon();
            vcall(icon, node::setPosition, {icon, vcall(avatar, node::getPosition, {avatar})});
            Frame f;
            uint32_t pos = f.alloc(8);
            call(g::Vec2_copy, {pos, vcall(icon, node::getPosition, {icon})});
            stf(pos + 4, ldf(pos + 4) + AVATAR_RAISE);
            vcall(icon, node::setPosition, {icon, pos});
            vcall(icon, V_REMOVE_FROM_PARENT, {icon});
            uint32_t parent = vcall(avatar, V_GET_PARENT, {avatar});
            addNamed(parent, icon, vcall(avatar, V_GET_LOCAL_Z_ORDER, {avatar}), S_Avatar_Icon_Obj, 15);
            vcall(avatar, node::setOpacity, {avatar, 0});
            call(g::Vec2_dtor, {pos});
        }
        uint32_t avatarButton = findNode(avatar, S_ImageButton_Avatar, 18);
        if (!avatarButton) {
            avatarButton = newImageButton(S_Empty, 0);
            call(g::ImageButton_setTargetView, {avatarButton, avatar, fbits(1.0f)});
            addNamed(avatar, avatarButton, 10, S_ImageButton_Avatar, 18);
        }
        {
            const uint32_t idLo = ld32(id), idHi = ld32(id + 4);
            const uint32_t copy = record - AT_RECORD + AT_AVATAR_COPY;
            call(g::memcpy, {copy, record, REC_READ});
            Frame f;
            uint32_t cb = newEmptyFunction(f);
            uint32_t heap = call(fn::operator_new, {0x140});
            if (heap) {
                st32(heap, VT_AvatarTouch);
                st32(heap + 8, row);
                st32(heap + 0xC, self);
                st32(heap + 0x10, idLo);
                st32(heap + 0x14, idHi);
                call(g::memcpy, {heap + 0x18, copy, REC_READ});
            }
            st32(cb + 16, heap);
            call(g::ImageButton_addTouchEventListener, {avatarButton, cb});
            functionDestroy(cb);
        }
        uint32_t name = findNode(row, S_Txt_Name_Label, 14);
        call(g::SceneUtil_setShadow, {name});
        setTextC(name, record + REC_NAME);
        uint32_t lux = findNode(row, S_Txt_Lux_Label, 13);
        call(g::SceneUtil_setShadow, {lux});
        {
            Frame f;
            uint32_t tier = newStr(f, S_Empty, 0);
            uint32_t t = ld32(record + REC_LUX);
            if (t <= 5) {
                uint32_t text = textData(f, TXT_LUX[t]);
                uint32_t s = textDataStr(text);
                call(g::String_assignN, {tier, s, host_strlen(s)});
                sharedRelease(text);
            }
            setTextC(lux, strData(tier));
            titleLabel(row, S_Txt_AvatarLV, 12, TXT_AVATAR_LV);
            uint32_t level = findNode(row, S_AtlasLabel_AvatarLV_Label, 25);
            {
                Frame g2;
                uint32_t s = g2.alloc(12);
                call(g::SceneUtil_numberToAtlasString, {s, ld32(record + REC_LEVEL)});
                call(g::TextAtlas_setString, {level, s});
                strFree(s);
            }
            uint32_t medalData = f.alloc(8);
            {
                uint32_t mgr = call(g::MedalDataManager_getInstance, {});
                Frame args;
                uint32_t sp = args.alloc(8 + MEDAL_SIZE);
                *rw::frame_ptr<uint32_t>(sp) = 0;
                for (uint32_t k = 0; k < MEDAL_SIZE; k += 4)
                    *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(record + REC_MEDAL + k);
                rw::call_exact(g::MedalDataManager_getMedalData, {medalData, mgr, 0, 0});
            }
            uint32_t medal = findNode(row, S_Medal, 5);
            {
                uint32_t mgr = call(g::MedalDataManager_getInstance, {});
                Frame g2;
                uint32_t copy = g2.alloc(8);
                st32(copy, ld32(medalData));
                st32(copy + 4, ld32(medalData + 4));
                if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
                uint32_t path = call(g::MedalDataManager_getMedalSmallImagePath, {mgr, copy});
                uint32_t s = newStr(g2, path);
                call(fn::ImageView_loadTexture, {medal, s, 0});
                strFree(s);
                sharedRelease(copy);
            }
            uint32_t medalLevel = findNode(row, S_Txt_MedalLV_Label, 17);
            call(g::SceneUtil_setShadow, {medalLevel});
            {
                Frame g2;
                setTextC(medalLevel, stringBuffer(g2, {S_d, ld32(record + REC_MEDAL_LV)}));
            }
            uint32_t medalButton = findNode(row, S_ImageButton_Medal, 17);
            if (!medalButton) {
                medalButton = newImageButton(S_Empty, 0);
                call(g::ImageButton_setTargetView, {medalButton, medal, ld32(ld32(g::GOT_ImageButtonScale))});
                addNamed(medal, medalButton, 0, S_ImageButton_Medal, 17);
            }
            {
                const uint32_t copy = record - AT_RECORD + AT_MEDAL_COPY;
                call(g::memcpy, {copy, record, REC_READ});
                Frame g2;
                uint32_t cb = newEmptyFunction(g2);
                uint32_t heap = call(fn::operator_new, {0x138});
                if (heap) {
                    st32(heap, VT_MedalTouch);
                    st32(heap + 8, row);
                    st32(heap + 0xC, self);
                    call(g::memcpy, {heap + 0x10, copy, REC_READ});
                }
                st32(cb + 16, heap);
                call(g::ImageButton_addTouchEventListener, {medalButton, cb});
                functionDestroy(cb);
            }
            titleLabel(row, S_Txt_MedalLV, 11, TXT_MEDAL_LV);
            dates(row, date, mode);
            sharedRelease(medalData);
            strFree(tier);
        }
    }

    static void dates(uint32_t row, uint32_t date, uint32_t mode) {
        Frame f;
        uint32_t ago = newStr(f, S_Empty, 0);
        {
            int32_t seconds = int32_t(call(g::Date_calculateIntervalSinceNow, {date}));
            Frame g2;
            uint32_t tmp = g2.alloc(12);
            call(g::createTimeAgoString, {tmp, uint32_t(-seconds)});
            uint32_t s = strData(tmp);
            call(g::String_assignN, {ago, s, host_strlen(s)});
            strFree(tmp);
        }
        uint32_t request = findNode(row, S_Txt_RequestDate_Label, 21);
        call(g::SceneUtil_setShadow, {request});
        uint32_t login = findNode(row, S_Txt_LastLogin_Label, 19);
        call(g::SceneUtil_setShadow, {login});
        uint32_t reception = findNode(row, S_Txt_Reception_Label, 19);
        call(g::SceneUtil_setShadow, {reception});
        uint32_t local = f.alloc(8);
        call(g::Date_makeLocalDate, {local, date});
        if (mode < 2) {
            setVisible(request, 0);
            setVisible(login, 1);
            setVisible(reception, 0);
            Frame g2;
            uint32_t text = textData(g2, TXT_LAST_LOGIN);
            setTwoParts(login, textDataStr(text), strData(ago));
            sharedRelease(text);
        } else if (mode == 2 || mode == 3) {
            setVisible(request, mode == 2);
            setVisible(login, 0);
            setVisible(reception, mode == 3);
            Frame g2;
            uint32_t text = textData(g2, mode == 2 ? TXT_REQUESTED : TXT_APPLIED);
            uint32_t label = textDataStr(text);
            uint32_t formatted = g2.alloc(12);
            {
                Frame args;
                args.alloc(16);
                uint32_t md = ld16_(local + 2), hm = ld16_(local + 4);
                rw::call_exact(g::TextData_format, {formatted, 0, uint32_t(TXT_DATE_FORMAT), ld16_(local), md & 0xFF,
                                                    md >> 8, hm & 0xFF, hm >> 8});
            }
            setTwoParts(mode == 2 ? request : reception, label, strData(formatted));
            strFree(formatted);
            sharedRelease(text);
        }
        strFree(ago);
    }

    static void buttons(uint32_t self, uint32_t row, uint32_t id, uint32_t mode) {
        for (const ModeButton &b : BUTTONS) {
            uint32_t button = findNode(row, b.name, b.nameLen);
            setVisible(button, mode == b.mode);
            uint32_t image = findNode(row, b.image, b.imageLen);
            if (!image) {
                image = newImageButton(S_But01, 5);
                call(g::ImageButton_setTargetImageView, {image, button});
                addNamed(button, image, 0, b.image, b.imageLen);
                titleLabel(button, S_Txt, 3, b.text);
            }
            const uint32_t idLo = ld32(id), idHi = ld32(id + 4);
            Frame f;
            uint32_t cb = newEmptyFunction(f);
            uint32_t heap = call(fn::operator_new, {0x18});
            if (heap) {
                st32(heap, b.vt);
                st32(heap + 8, row);
                st32(heap + 0xC, self);
                st32(heap + 0x10, idLo);
                st32(heap + 0x14, idHi);
            }
            st32(cb + 16, heap);
            call(g::ImageButton_addTouchEventListener, {image, cb});
            functionDestroy(cb);
        }
    }

    // Leader: the party leader; Sub: one of the sub-leaders
    static void marks(uint32_t row, uint32_t record) {
        uint32_t party = ld32(call(fn::APIManager_getInstance, {}) + API_PARTY);
        uint32_t leader = findNode(row, S_Leader, 6);
        uint32_t sub = findNode(row, S_Sub, 3);
        const uint32_t lo = ld32(record), hi = ld32(record + 4);
        uint32_t isLeader = 0, isSub = 0;
        if (ld32(party + PARTY_LEADER) == lo && ld32(party + PARTY_LEADER + 4) == hi) {
            isLeader = 1;
        } else {
            for (uint32_t p = ld32(party + PARTY_SUBS); p != ld32(party + PARTY_SUBS + 4); p += 8)
                if (ld32(p) == lo && ld32(p + 4) == hi) {
                    isSub = 1;
                    break;
                }
        }
        setVisible(leader, isLeader);
        setVisible(sub, isSub);
    }
};
}  // namespace

RW_FUNCTION(0x520EB4, "MemberScrollView::changeRow(ui::Widget*, int, int)", MemberScrollViewRow::changeRow);
