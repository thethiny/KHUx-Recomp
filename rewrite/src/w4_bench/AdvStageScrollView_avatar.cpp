// AdvStageScrollView::settingAvatarFrame: the support partner's avatar (and Facebook photo) on a story row.
#include "AdvStageScrollView.hpp"

using namespace scene;

namespace {
namespace lib {
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_getFaceImageUrl = 0x621D0D;      // (string out, manager, const string& facebookId)
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t SceneUtil_changePartnerAvatarPanel = 0x46B15D; // (Widget*, SupportUser by value, AvatarIconCache*)
constexpr uint32_t Stage_create = 0x78E4E9;
constexpr uint32_t SafeBuffer_read = 0x45A269;                   // (sb, dst, src, n, index)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t ExpressionVector_allocate = 0x487635;         // vector<Avatar::Expression>::allocate(n)
constexpr uint32_t ExpressionVector_dtor = 0x46BE0D;
constexpr uint32_t AvatarIconCache_ctor = 0x5CA741;              // (cache, Texture2D*, const Vec2&, float, float, const vector<Expression>&)
constexpr uint32_t string_compare = 0x47CBD5;                    // std::string::compare(const char*) (sub_47CBD4)
constexpr uint32_t ImageDownloader_loadImage = 0x5BD0CB;         // (downloader, ImageView*, url, name)
constexpr uint32_t ImageDownloader_addFacebookID = 0x5BD9A1;     // (downloader, const char* id, function<void(bool, const char*)>)
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;
}  // namespace lib

constexpr uint32_t VT_settingAvatarFrame_17 = 0xF593B4;          // __func of the lambda (view, ImageView*, std::string)

constexpr uint32_t S_AVATAR = 0xDF8F56;                          // "Avatar"
constexpr uint32_t S_BALL = 0xDFF3DD;                            // "Ball"
constexpr uint32_t S_FACEBOOK_PHOTO_PANEL = 0xDF8E5A;            // "FaceBook_PhotoPanel"
constexpr uint32_t S_FACEBOOK_PHOTO = 0xDF8E6E;                  // "FaceBook_Photo"
constexpr uint32_t S_EMPTY = 0xDF8A1A;                           // ""

constexpr uint32_t API_USER_ID = 116;          // APIManager + 116: SafeBuffer* (+0: the user's 64-bit id)

constexpr uint32_t VS_GET_RENDERER = 0x2D8;    // ui widget slot: its renderer (Sprite / Scale9Sprite)
constexpr uint32_t VS_GET_TEXTURE = 0x280;     // the renderer's Texture2D
constexpr uint32_t VS_SET_ENABLED = 0x29C;

// PartyMemberList (a SafeBuffer of 296-byte SupportUser records): +32 count, +56 loaded flag, +60 std::vector of
// 152-byte member records (+120: the stage the member supports, +136: Facebook id std::string)
constexpr uint32_t PML_COUNT = 32, PML_LOADED = 56, PML_MEMBERS = 60;
constexpr uint32_t MEMBER_SIZE = 152, MEMBER_STAGE = 120, MEMBER_FACEBOOK_ID = 136;
constexpr uint32_t SUPPORT_USER_SIZE = 296;

// the original frame (5 pushed registers + 0x344): its locals at their sp offsets
constexpr uint32_t FRAME = 20 + 0x1FC + 0x148;
constexpr uint32_t L_SUPPORT_USER = 0x210;     // SafeBuffer::read of one SupportUser (296 bytes)
constexpr uint32_t L_FUNCTION = 0x1F8;         // std::function<void(bool, const char*)>
constexpr uint32_t L_EXPRESSION = 0x1F4;       // the one Avatar::Expression of the initializer list
constexpr uint32_t L_USER_ID = 0x1E8;          // the user's id (8 bytes)
constexpr uint32_t L_STAGE_NO = 0x1E4;
constexpr uint32_t L_STR_AVATAR0 = 0x1D8;
constexpr uint32_t L_VEC2 = 0x1D0;
constexpr uint32_t L_EXPRESSIONS = 0x1C4;      // std::vector<Avatar::Expression>
constexpr uint32_t L_STAGE = 0x1BC;            // shared_ptr<master::Stage>
constexpr uint32_t L_STR_BALL = 0x1B0, L_STR_AVATAR = 0x1A4, L_STR_PHOTO_PANEL = 0x198, L_STR_PHOTO = 0x18C;
constexpr uint32_t L_FACEBOOK_ID = 0x180, L_URL = 0x174;
constexpr uint32_t L_CAPTURES = 0x160;         // lambda captures: view, ImageView*, std::string (at +8)

uint32_t findAt(const guest::Frame &f, uint32_t off, uint32_t root, uint32_t name, uint32_t len) {
    uint32_t s = f.addr(off);
    strInit(s, name, len);
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    strFree(s);
    return node;
}
uint32_t cstrOf(uint32_t s) { return strIsLong(s) ? ld32(s + 8) : s + 1; }
}  // namespace

// Shows the avatar of the party member supporting the row's stage (1 when there is one). `party` points to the
// shared_ptr<PartyMemberList> argument; `unused` is the trailing unsigned argument.
static uint32_t settingAvatarFrame(AdvStageScrollView *self, uint32_t widget, int32_t index, uint32_t party,
                                   uint32_t) {
    uint32_t me = rw::guest_addr(self);
    guest::Frame f(FRAME);
    uint8_t *sp = f.at<uint8_t>(0);
    auto w32 = [&](uint32_t off, uint32_t v) { std::memcpy(sp + off, &v, 4); };
    auto r32 = [&](uint32_t off) { uint32_t v; std::memcpy(&v, sp + off, 4); return v; };

    if (!self->avatarIconCache) {
        uint32_t avatar = findAt(f, L_STR_AVATAR0, widget, S_AVATAR, 6);
        uint32_t renderer = vcall(avatar, VS_GET_RENDERER, {avatar});
        uint32_t texture = vcall(renderer, VS_GET_TEXTURE, {renderer});
        uint32_t cache = call(guest::OPERATOR_NEW, {0x60});
        call(lib::Vec2_ctor, {f.addr(L_VEC2), rw::fbits(-54.0f), rw::fbits(80.0f)});
        w32(L_EXPRESSION, 1);
        uint32_t vec = f.addr(L_EXPRESSIONS);
        w32(L_EXPRESSIONS, 0);
        w32(L_EXPRESSIONS + 4, 0);
        w32(L_EXPRESSIONS + 8, 0);
        call(lib::ExpressionVector_allocate, {vec, 1});
        // push_back the initializer list's single element
        uint32_t end = r32(L_EXPRESSIONS + 4);
        uint32_t e = 0;
        if (end) {
            st32(end, r32(L_EXPRESSION));
            e = r32(L_EXPRESSIONS + 4);
        }
        w32(L_EXPRESSIONS + 4, e + 4);
        rw::call_exact(lib::AvatarIconCache_ctor,
                       {cache, texture, f.addr(L_VEC2), rw::fbits(-0.91f), rw::fbits(0.91f), vec});
        self->avatarIconCache = cache;
        call(lib::ExpressionVector_dtor, {vec});
        call(lib::Vec2_dtor, {f.addr(L_VEC2)});
    }

    call(lib::Stage_create, {f.addr(L_STAGE), uint32_t(index)});
    uint32_t stageRow = r32(L_STAGE);
    rw::call_exact(lib::SafeBuffer_read, {stageRow, f.addr(L_STAGE_NO), ld32(stageRow + 12) + 4, 4, 4});
    uint32_t stageNo = r32(L_STAGE_NO);
    uint32_t userSb = ld32(call(lib::APIManager_getInstance, {}) + API_USER_ID);
    rw::call_exact(lib::SafeBuffer_read, {userSb, f.addr(L_USER_ID), ld32(userSb + 12), 8, 0});
    uint32_t userLo = r32(L_USER_ID), userHi = r32(L_USER_ID + 4);
    uint32_t ball = findAt(f, L_STR_BALL, widget, S_BALL, 4);
    uint32_t avatar = findAt(f, L_STR_AVATAR, widget, S_AVATAR, 6);
    vcall(avatar, VS_SET_ENABLED, {avatar, 1});
    uint32_t photoPanel = findAt(f, L_STR_PHOTO_PANEL, widget, S_FACEBOOK_PHOTO_PANEL, 0x13);
    uint32_t photo = findAt(f, L_STR_PHOTO, widget, S_FACEBOOK_PHOTO, 0xE);

    uint32_t result = 0;
    uint32_t list = ld32(party);
    if (list && ld8(list + PML_LOADED) && ld32(list + PML_COUNT)) {
        uint32_t memberOff = 0, recordIndex = 0;
        for (uint32_t k = 0;; k++) {
            uint32_t begin = ld32(list + PML_MEMBERS);
            uint32_t n = (uint32_t(int32_t(ld32(list + PML_MEMBERS + 4) - begin) >> 3)) * 0x286BCA1Bu;   // / 152
            if (n <= k) call(lib::THROW_OUT_OF_RANGE, {list + PML_MEMBERS});
            uint32_t member = begin + memberOff;
            if (ld32(member + MEMBER_STAGE) == stageNo) {
                sp[L_SUPPORT_USER + 287] &= 0xFD;   // (a bit of the buffer cleared before the read overwrites it)
                rw::call_exact(lib::SafeBuffer_read,
                               {list, f.addr(L_SUPPORT_USER), ld32(list + 12) + recordIndex, SUPPORT_USER_SIZE,
                                recordIndex});
                uint32_t idLo = r32(L_SUPPORT_USER), idHi = r32(L_SUPPORT_USER + 4);
                if ((idLo ^ userLo) | (idHi ^ userHi)) {
                    // someone else: SupportUser by value (8 bytes in r2:r3, the other 288 at [sp]), then the cache
                    w32(288, self->avatarIconCache);
                    std::memmove(sp, sp + L_SUPPORT_USER + 8, 288);
                    rw::call_exact(lib::SceneUtil_changePartnerAvatarPanel,
                                   {ball, f.addr(L_SUPPORT_USER + 8 + 288), idLo, idHi});
                    vcall(ball, node::setVisible, {ball, 1});
                    result = 1;
                    if (call(lib::SceneUtil_isLoggedInFacebook, {}) != 1) break;
                    // the member's Facebook id: its profile photo
                    uint32_t fb = f.addr(L_FACEBOOK_ID);
                    uint32_t src = member + MEMBER_FACEBOOK_ID;
                    if (ld8(src) & 1) {
                        strInit(fb, ld32(src + 8), ld32(src + 4));
                    } else {
                        w32(L_FACEBOOK_ID, ld32(src));
                        w32(L_FACEBOOK_ID + 4, ld32(src + 4));
                        w32(L_FACEBOOK_ID + 8, ld32(src + 8));
                    }
                    if (call(lib::string_compare, {fb, S_EMPTY}) != 0) {
                        vcall(photoPanel, node::setVisible, {photoPanel, 1});
                        uint32_t url = f.addr(L_URL);
                        call(lib::SceneManager_getFaceImageUrl, {url, call(lib::SceneManager_getInstance, {}), fb});
                        uint32_t urlSet = call(lib::string_compare, {url, S_EMPTY});
                        uint32_t downloader = self->imageDownloader;
                        if (urlSet) {
                            call(lib::ImageDownloader_loadImage, {downloader, photo, cstrOf(url), cstrOf(fb)});
                        } else {
                            // ask for the URL: a lambda (view, photo, copy of the id) loads it when it arrives
                            uint32_t caps = f.addr(L_CAPTURES);
                            uint32_t idStr = caps + 8;
                            uint32_t id;
                            w32(L_CAPTURES, me);
                            w32(L_CAPTURES + 4, photo);
                            if (strIsLong(fb)) {
                                id = ld32(fb + 8);
                                strInit(idStr, ld32(fb + 8), ld32(fb + 4));
                            } else {
                                w32(L_CAPTURES + 8, r32(L_FACEBOOK_ID));
                                w32(L_CAPTURES + 12, r32(L_FACEBOOK_ID + 4));
                                w32(L_CAPTURES + 16, r32(L_FACEBOOK_ID + 8));
                                id = fb + 1;
                            }
                            uint32_t fn = f.addr(L_FUNCTION);
                            w32(L_FUNCTION + 16, 0);
                            uint32_t heap = call(guest::OPERATOR_NEW, {0x18});
                            if (heap) {
                                st32(heap, VT_settingAvatarFrame_17);
                                st32(heap + 8, photo);
                                st32(heap + 4, me);
                                st32(heap + 20, r32(L_CAPTURES + 16));
                                st32(heap + 16, r32(L_CAPTURES + 12));
                                st32(heap + 12, r32(L_CAPTURES + 8));
                                w32(L_CAPTURES + 8, 0);
                                w32(L_CAPTURES + 12, 0);
                                w32(L_CAPTURES + 16, 0);
                            }
                            w32(L_FUNCTION + 16, heap);
                            call(lib::ImageDownloader_addFacebookID, {downloader, id, fn});
                            functionDestroy(fn);
                            strFree(idStr);
                        }
                        strFree(url);
                    }
                    strFree(fb);
                    break;
                }
                list = ld32(party);
            }
            recordIndex += SUPPORT_USER_SIZE;
            memberOff += MEMBER_SIZE;
            if (!(k + 1 < ld32(list + PML_COUNT))) break;
        }
    }
    uint32_t ctrl = r32(L_STAGE + 4);
    if (ctrl) call(guest::SHARED_RELEASE, {ctrl});
    return result;
}

RW_FUNCTION(0x56FC1C, "AdvStageScrollView::settingAvatarFrame", settingAvatarFrame);
