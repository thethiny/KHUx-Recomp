// SceneUtil: MyPage's status panels (the deck's HP / attack / defense; the player and party panel).
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t UserPoint_hp = 0x38;                   // SafeBuffer index
// User / UserDetail SafeBuffer indexes
constexpr uint32_t User_id = 0x00;                        // u64
constexpr uint32_t User_name = 0x14;                      // char[0x21]
constexpr uint32_t UserDetail_rank = 0x08;
constexpr uint32_t UserDetail_partyId = 0x28;             // u64, 0: in no party
constexpr uint32_t UserDetail_unionId = 0x30;
constexpr uint32_t UserDetail_crowns = 0x38;              // shown as stars (5 each, beyond the first 5) + crowns
constexpr uint32_t Party_name = 0x10;                     // std::string
constexpr uint32_t APIManager_user = 0x74;                // shared_ptr<User>: the player
constexpr uint32_t APIManager_facebook = 0x294;           // -> object holding the Facebook id string at +0x2C8
constexpr uint32_t Facebook_id = 0x2C8;

// ---- callees
constexpr uint32_t SceneUtil_createUnionImagePath = 0x46968D;   // (string out, int id, bool, bool)
constexpr uint32_t SceneUtil_updateUserTitlePanel = 0x46974D;   // (Node*, const shared_ptr<UserDetail>&)
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t String_notEqualC = 0x47CBD5;           // operator!=(const string&, const char*)
constexpr uint32_t FacebookManager_getInstance = 0x4E5585;
constexpr uint32_t FacebookManager_getProfileImageUrl = 0x4E5783;   // (mgr, const string& id, const function&)
constexpr uint32_t ImageDownloader_loadImage = 0x5BD0CB;  // (dl, ImageView*, const char* url, const char* key)
constexpr uint32_t SceneManager_getFaceImageUrl = 0x621D0D;   // (string out, mgr, const string& id)
constexpr uint32_t Sprite_create = 0x8A64DD;
constexpr uint32_t Rect_ctor = 0x8DF18B;                  // (rect, x, y, w, h)
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Director_getTextureCache = 0x8EFE69;
constexpr uint32_t Texture2D_getContentSize = 0x914ACB;   // (Size out, texture)
constexpr uint32_t TextureCache_addImage = 0x917DA9;      // (cache, const string& path)

// virtual slots
constexpr uint32_t VSLOT_removeAllChildrenWithCleanup = 0x13C;
constexpr uint32_t VSLOT_setTexture = 0x27C, VSLOT_setTextureRect = 0x284;   // cocos2d::Sprite

// the Facebook photo lambda (getProfileImageUrl's callback): [id string, downloader, image]
constexpr uint32_t VT_FacebookPhoto = 0xF54B0C;
constexpr uint32_t FUNC_FacebookPhoto_SIZE = 0x18;

constexpr int32_t TEXT_ATTACK = 100200007, TEXT_DEFENSE = 100200008;
constexpr int32_t TEXT_PARTY = 102100024, TEXT_NO_PARTY = 102100023, TEXT_LV = 100200001;
constexpr float STAR_X = 12.0f, STAR_Y = 20.0f, STAR_STEP = 19.0f;
constexpr float CROWN_XY = 21.0f, CROWN_STEP = 38.0f;

struct Lit { uint32_t addr, len; };
constexpr uint32_t FMT_Int = 0xDF86DE;                    // "%d"
constexpr Lit S_Txt_HP_Label{0xDF8EBF, 12}, S_Txt_Power_Label{0xDF8ECC, 15}, S_Txt_Power{0xDF8EDC, 9};
constexpr Lit S_Txt_Defence_Label{0xDF8EE6, 17}, S_Txt_Defence{0xDF8EF8, 11};
constexpr Lit S_Union_Symbol{0xDF8D8E, 12}, S_Base_User{0xDF8D9B, 9}, S_Txt_Party{0xDF8DC4, 9};
constexpr Lit S_Party_Name{0xDF8DCE, 10}, S_My_Name{0xDF8DD9, 7}, S_Txt_LV{0xDF8DE1, 6}, S_Txt_LV_Label{0xDF8DE8, 12};
constexpr Lit S_Area_Crown_Star41{0xDF8DF5, 17}, S_Area_Crown41{0xDF8E2C, 12};
constexpr Lit S_FaceBook_PhotoPanel{0xDF8E5A, 19}, S_FaceBook_Photo{0xDF8E6E, 14};
constexpr Lit IMG_Plate10{0xDF8DA5, 30}, IMG_Crown_Star{0xDF8E07, 29}, IMG_Crown{0xDF8E39, 24};
constexpr Lit IMG_PhotoPanel03{0xDF8E7D, 32}, IMG_PhotoPanel04{0xDF8E9E, 32};
constexpr uint32_t FMT_Star = 0xDF8E25, FMT_Crown = 0xDF8E52;   // "star%d", "crown%d"
constexpr uint32_t S_EMPTY = 0xDF8A1A;                    // ""

inline uint32_t find(uint32_t root, Lit name) { return findNode(root, name.addr, name.len); }
inline void setVisible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void loadTexture(uint32_t image, Lit path) {
    Frame f;
    const uint32_t s = newStr(f, path.addr, path.len);
    call(fn::ImageView_loadTexture, {image, s, 0});
    strFree(s);
}
// a temporary copy of a std::string, as libc++ inlines it (a long string is re-allocated)
uint32_t strCopy(Frame &f, uint32_t src) {
    const uint32_t s = f.alloc(12);
    if (strIsLong(src)) {
        strInit(s, ld32(src + 8), ld32(src + 4));
    } else {
        st32(s, ld32(src));
        st32(s + 4, ld32(src + 4));
        st32(s + 8, ld32(src + 8));
    }
    return s;
}

// `count` sprites of `texturePath` added to `area` as children named fmt % i (tag i), at (x, y) stepping right.
// The crowns get their position before their texture, the stars after.
void addIcons(uint32_t area, Lit texturePath, int32_t count, float x, float y, float step, uint32_t fmt,
              bool positionFirst) {
    vcall(area, VSLOT_removeAllChildrenWithCleanup, {area, 1});
    setVisible(area, 1);
    Frame f;
    const uint32_t position = f.alloc(8);
    call(Vec2_copy, {position, vcall(area, node::getPosition, {area})});
    stf(position, x);
    stf(position + 4, y);
    const uint32_t cache = call(Director_getTextureCache, {director()});
    uint32_t texture;
    {
        Frame g;
        const uint32_t s = newStr(g, texturePath.addr, texturePath.len);
        texture = call(TextureCache_addImage, {cache, s});
        strFree(s);
    }
    const uint32_t size = f.alloc(8);
    call(Texture2D_getContentSize, {size, texture});
    for (int32_t i = 0; i < count; i++) {
        const uint32_t sprite = call(Sprite_create, {});
        if (positionFirst) vcall(sprite, node::setPosition, {sprite, position});
        vcall(sprite, VSLOT_setTexture, {sprite, texture});
        {
            Frame g;
            const uint32_t rect = g.alloc(16);
            call(Rect_ctor, {rect, 0, 0, rw::load<uint32_t>(size), rw::load<uint32_t>(size + 4)});
            vcall(sprite, VSLOT_setTextureRect, {sprite, rect});
            call(Vec2_dtor, {rect});
        }
        if (!positionFirst) vcall(sprite, node::setPosition, {sprite, position});
        setVisible(sprite, 1);
        Frame g;
        const uint32_t name = newStr(g, stringBuffer(g, {fmt, uint32_t(i)}));
        vcall(area, node::addChildZName, {area, sprite, uint32_t(i), name});
        strFree(name);
        stf(position, ldf(position) + step);
    }
    call(Vec2_dtor, {position});
}

// the Facebook photo of the player (when logged in to Facebook); returns whether the panel is shown
bool showFacebookPhoto(uint32_t panel, uint32_t api, uint32_t downloader, bool otherUser) {
    Frame f;
    const uint32_t photo = find(panel, S_FaceBook_Photo);
    const uint32_t facebookId = ld32(api + APIManager_facebook) + Facebook_id;
    if (!call(String_notEqualC, {facebookId, S_EMPTY})) return false;
    const uint32_t id = strCopy(f, facebookId);
    const uint32_t url = f.alloc(12);
    call(SceneManager_getFaceImageUrl, {url, sceneManager(), id});
    if (call(String_notEqualC, {url, S_EMPTY})) {
        // a known URL: download it now
        call(ImageDownloader_loadImage, {downloader, photo, strData(url), strData(id)});
    } else {
        // ask Facebook for the URL; the lambda downloads it
        const uint32_t idCopy = strCopy(f, id);
        const uint32_t manager = call(FacebookManager_getInstance, {});
        const uint32_t captured = strCopy(f, idCopy);
        const uint32_t callback = f.alloc(20);
        st32(callback + 16, 0);
        const uint32_t heap = call(fn::operator_new, {FUNC_FacebookPhoto_SIZE});
        if (heap) {
            st32(heap, VT_FacebookPhoto);
            st32(heap + 0xC, ld32(captured + 8));
            st32(heap + 8, ld32(captured + 4));
            st32(heap + 4, ld32(captured));
            st32(captured, 0);
            st32(captured + 4, 0);
            st32(captured + 8, 0);
            st32(heap + 0x14, photo);
            st32(heap + 0x10, downloader);
        }
        st32(callback + 16, heap);
        call(FacebookManager_getProfileImageUrl, {manager, id, callback});
        functionDestroy(callback);
        strFree(captured);
        strFree(idCopy);
    }
    strFree(url);
    strFree(id);
    loadTexture(panel, otherUser ? IMG_PhotoPanel04 : IMG_PhotoPanel03);
    return true;
}

// a shadowed Text named `name` showing the number `value`
void setNumber(uint32_t root, Lit name, int32_t value) {
    const uint32_t text = find(root, name);
    SceneUtil::setShadow(text);
    Frame f;
    setTextC(text, stringBuffer(f, {FMT_Int, uint32_t(value)}));
}
// a shadowed Text named `name` showing the TextData text `textId`
void setLabel(uint32_t root, Lit name, int32_t textId) {
    const uint32_t text = find(root, name);
    SceneUtil::setShadow(text);
    setTextId(text, textId);
}
}  // namespace

void SceneUtil::setMyPageTeamStatus(Addr root, Addr user, Addr detail, Addr party, Addr downloader,
                                    uint32_t showCrowns) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t me = ld32(api + APIManager_user);
    const uint32_t meCtl = ld32(api + APIManager_user + 4);
    if (meCtl) call(fn::shared_add, {meCtl});
    const uint32_t api2 = call(fn::APIManager_getInstance, {});
    const uint32_t detailSb = ld32(detail);

    // the union emblem
    {
        const uint32_t emblem = find(root, S_Union_Symbol);
        const int32_t unionId = safe32(detailSb, UserDetail_unionId);
        Frame f;
        const uint32_t path = f.alloc(12);
        call(SceneUtil_createUnionImagePath, {path, uint32_t(unionId), 0, 0});
        const uint32_t s = newStr(f, strData(path));
        call(fn::ImageView_loadTexture, {emblem, s, 0});
        strFree(s);
        strFree(path);
    }
    // another player's panel gets the other plate
    const uint32_t userSb = ld32(user);
    const bool otherUser = safe32(me, User_id) != safe32(userSb, User_id) ||
                           safe32(me, User_id + 4) != safe32(userSb, User_id + 4);
    if (otherUser) loadTexture(find(root, S_Base_User), IMG_Plate10);

    titleLabel(root, S_Txt_Party.addr, S_Txt_Party.len, TEXT_PARTY);
    {
        const uint32_t name = find(root, S_Party_Name);
        if (safe32(detailSb, UserDetail_partyId) | safe32(detailSb, UserDetail_partyId + 4))
            setTextC(name, strData(party + Party_name));
        else
            setTextId(name, TEXT_NO_PARTY);
        SceneUtil::setShadow(name);
    }
    {
        const uint32_t name = find(root, S_My_Name);
        Frame f;
        setTextC(name, safeBytes(f, userSb, User_name, 0x21));
        SceneUtil::setShadow(name);
    }
    {
        const uint32_t lv = find(root, S_Txt_LV);
        const uint32_t rank = find(root, S_Txt_LV_Label);
        setTextId(lv, TEXT_LV);
        Frame f;
        setTextC(rank, stringBuffer(f, {FMT_Int, uint32_t(safe32(detailSb, UserDetail_rank))}));
        SceneUtil::setShadow(lv);
        SceneUtil::setShadow(rank);
    }

    // crowns: a star for each full 5 beyond the first, then the rest as crowns (5 for a multiple of 5)
    const int32_t crowns = safe32(detailSb, UserDetail_crowns);
    if (crowns >= 1 && showCrowns == 1) {
        if (crowns >= 6) {
            const uint32_t stars = find(root, S_Area_Crown_Star41);
            addIcons(stars, IMG_Crown_Star, (crowns - 1) / 5, STAR_X, STAR_Y, STAR_STEP, FMT_Star, false);
        }
        const uint32_t area = find(root, S_Area_Crown41);
        const int32_t rest = crowns % 5;
        addIcons(area, IMG_Crown, rest ? rest : 5, CROWN_XY, CROWN_XY, CROWN_STEP, FMT_Crown, true);
    } else {
        const uint32_t stars = find(root, S_Area_Crown_Star41);
        const uint32_t area = find(root, S_Area_Crown41);
        setVisible(stars, 0);
        setVisible(area, 0);
    }

    call(SceneUtil_updateUserTitlePanel, {root, detail});
    const uint32_t panel = find(root, S_FaceBook_PhotoPanel);
    bool shown = false;
    if (call(SceneUtil_isLoggedInFacebook, {}) == 1) shown = showFacebookPhoto(panel, api2, downloader, otherUser);
    setVisible(panel, shown ? 1 : 0);
    if (meCtl) call(fn::shared_release, {meCtl});
}

void SceneUtil::setMyPageDeckStatus(Addr root, Addr userPoint, int32_t power, int32_t defense) {
    {
        const uint32_t text = find(root, S_Txt_HP_Label);
        SceneUtil::setShadow(text);
        const int32_t hp = safe32(ld32(userPoint), UserPoint_hp);
        Frame f;
        setTextC(text, stringBuffer(f, {FMT_Int, uint32_t(hp)}));
    }
    setNumber(root, S_Txt_Power_Label, power);
    setLabel(root, S_Txt_Power, TEXT_ATTACK);
    setNumber(root, S_Txt_Defence_Label, defense);
    setLabel(root, S_Txt_Defence, TEXT_DEFENSE);
}

RW_FUNCTION(0x46874C, "SceneUtil::setMyPageTeamStatus", SceneUtil::setMyPageTeamStatus);
RW_FUNCTION(0x469868, "SceneUtil::setMyPageDeckStatus", SceneUtil::setMyPageDeckStatus);
