// SceneMyPage: the ranking mode (zoomed-out field with the members lined up) and the camera zoom.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FadeOut_create = rwlib::cocos2d::FadeOut::create;
constexpr uint32_t FunctionVV_swap = 0x4EE86D;
constexpr uint32_t String_compareC = 0x47CBD5, String_assign = 0x498A1D;
constexpr uint32_t SceneManager_getFaceImageUrl = 0x621D0D;
constexpr uint32_t ImageDownloader_loadImage = 0x5BD0CB, ImageDownloader_addFacebookID = 0x5BD9A1;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t M_zoomInCamera = 0x6436F5, M_setCameraPosition = 0x640851, M_changeRankingHeader = 0x643C9D;
constexpr uint32_t M_resetAvatarAction = 0x643605, M_changeRankingSort = 0x645141;
constexpr uint32_t M_setAvatarProfileImageVisible = 0x641EF5;
constexpr uint32_t VT_zoomIn_23 = 0xF61580, VT_zoomOut_24 = 0xF61554, VT_mode_25 = 0xF61528, VT_profile_22 = 0xF615AC;
constexpr uint32_t S_FUKIDASHI = 0xE03781, S_AVATAR_STATUS = 0xE037FA, S_AVATAR_BALOON = 0xE037CC;
constexpr uint32_t S_FB_PHOTO = 0xDF8E5A, S_FB_PHOTO_IMG = 0xDF8E6E;

uint32_t cstrOf(uint32_t s) { return strIsLong(s) ? ld32(s + 8) : s + 1; }

// zoom the field (and the centre widget) to the scale at `scaleOff`, then `done` (a default lambda when empty)
void zoomCamera(uint32_t self, uint32_t done, uint32_t scaleOff, uint32_t vt) {
    if (!ld32(done + 16)) {
        Frame f;
        uint32_t fo = newFunction(f, vt, {});
        call(FunctionVV_swap, {fo, done});
        functionDestroy(fo);
    }
    float scale = ldf(self + scaleOff);
    stf(self + 0x370, scale / bf(0x4026353Fu));
    stf(self + 0x374, scale);
    call(smain::lib::Node_stopAllActions, {ld32(self + 0x298)});
    {
        uint32_t panel = ld32(self + 0x298);
        uint32_t delay = call(wl::DelayTime_create, {fb(0.2f)});
        uint32_t ease = call(EaseSineOut_create, {call(fn::ScaleTo_create, {fb(1.2f), ld32(self + scaleOff)})});
        uint32_t cf = call(wl::CallFunc_create, {done});
        call(wl::Node_runAction, {panel, call(wl::Sequence_create, {delay, ease, cf, 0})});
    }
    uint32_t center = ld32(self + 0x294);
    uint32_t delay = call(wl::DelayTime_create, {fb(0.2f)});
    uint32_t ease = call(EaseSineOut_create, {call(fn::ScaleTo_create, {fb(1.2f), ld32(self + scaleOff)})});
    call(wl::Node_runAction, {center, call(wl::Sequence_create, {delay, ease, 0})});
}
}  // namespace

// the actor stops; its comment balloon hides
void SceneMyPage_resetAvatarAction(uint32_t, uint32_t actor) {
    call(smain::lib::Node_stopAllActions, {actor});
    uint32_t status = findNode(actor, S_AVATAR_STATUS, 13);
    uint32_t image = findNode(status, S_AVATAR_BALOON, 13);
    vcall(image, node::setVisible, {image, 0});
}

void SceneMyPage_zoomInCamera(uint32_t self, uint32_t done) { zoomCamera(self, done, 0x36C, VT_zoomIn_23); }
void SceneMyPage_zoomOutCamera(uint32_t self, uint32_t done) { zoomCamera(self, done, OFF_SCALE_BASE, VT_zoomOut_24); }

// Into the ranking mode: the balloons hide, the camera zooms in on the line-up, the header changes ($_25); the
// party members are sorted for the mode, other avatars fade out. Returns 1 when the mode changed.
uint32_t SceneMyPage_changeRankingMode(uint32_t self) {
    if (!ld8(self + 0x2F9)) return 0;
    if (!ld8(self + 0x2F0)) return 0;
    call(smain::lib::Node_stopAllActions, {ld32(self + 0x29C)});
    call(smain::lib::Node_stopAllActions, {ld32(self + 0x2A4)});
    st32(self + 0x388, 0);
    const uint32_t shown = self + OFF_BALOON_SHOWN;
    uint32_t n = ld32(shown + 4);
    for (uint32_t i = 0; i < n; i++) {
        if (!(ld32(ld32(shown) + ((i >> 3) & 0x1FFFFFFCu)) & (1u << (i & 31)))) continue;
        uint32_t layer = ld32(self + 0x2A4);
        Frame f;
        uint32_t buf = stringBuffer(f, {S_FUKIDASHI, i});
        uint32_t name = newStr(f, buf);
        uint32_t balloon = SceneUtil::searchNodeByName(layer, name);
        strFree(name);
        vcall(balloon, node::setVisible, {balloon, 0});
        n = ld32(shown + 4);
    }
    st8(self + 0x2F0, 0);
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(M_zoomInCamera, {self, empty});
        functionDestroy(empty);
    }
    {
        uint32_t panel = ld32(self + 0x298);
        float x = ldf(vcall(panel, node::getContentSize, {panel})) * 0.5f;
        Frame f;
        uint32_t v = f.alloc(8);
        call(wl::Vec2_ctorXY, {v, fb(x), fb(80.0f)});
        call(M_setCameraPosition, {self, v, fb(1.2f)});
        call(wl::Vec2_dtor, {v});
    }
    st32(self + 0x38C, 0);
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_mode_25, {});
        call(M_changeRankingHeader, {self, 1, fo});
        functionDestroy(fo);
    }
    const uint32_t actors = self + OFF_ACTORS_A;
    if (ld8(self + OFF_HAS_PARTY) || ld32(self + 0x28C) == 1) {
        if (int32_t(ld32(actors + 4) - ld32(actors)) >= 1) {
            int32_t i = 0;
            do {
                uint32_t at = ld32(actors) + uint32_t(i) * 4;
                call(M_resetAvatarAction, {at, ld32(at)});
                i++;
            } while (i < int32_t(ld32(actors + 4) - ld32(actors)) >> 2);
        }
        call(M_changeRankingSort, {self, ld8(self + 0x3A8)});
    } else {
        if (int32_t(ld32(actors + 4) - ld32(actors)) >= 1) {
            int32_t i = 0;
            do {
                uint32_t begin = ld32(actors);
                uint32_t actor = ld32(begin + uint32_t(i) * 4);
                if (ld32(self + 0x2BC) != actor) {
                    call(M_resetAvatarAction, {begin, actor});
                    call(M_setAvatarProfileImageVisible, {self, actor, 0});
                    uint32_t avatar = ld32(actor + 0x288);
                    call(wl::Node_runAction, {avatar, call(FadeOut_create, {fb(0.6f)})});
                }
                i++;
            } while (i < int32_t(ld32(actors + 4) - ld32(actors)) >> 2);
        }
    }
    return 1;
}

// Show / hide the Facebook photo of a member's status balloon (loading it the first time it is shown)
void SceneMyPage_setAvatarProfileImageVisible(uint32_t self, uint32_t actor, uint32_t visible) {
    Frame f;
    uint32_t fbId = f.alloc(12);
    st32(fbId, 0);
    st32(fbId + 4, 0);
    st32(fbId + 8, 0);
    uint32_t found = 0;
    {
        const uint32_t actors = self + OFF_ACTORS_A;
        uint32_t begin = ld32(actors);
        int32_t bytes = int32_t(ld32(actors + 4) - begin);
        if (bytes >= 1) {
            for (int32_t i = 0; i < (bytes >> 2); i++) {
                if (ld32(begin + uint32_t(i) * 4) != actor) continue;
                // the member's facebook id (party members list, else the friends list)
                uint32_t ids = self + OFF_IDS;
                if (!(uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 3) > uint32_t(i))) {
                    call(wl::VECTOR_AT_THROW, {ids});
                    return;
                }
                uint32_t idAt = ld32(ids) + uint32_t(i) * 8;
                uint32_t members = ld32(self + 0x2C0);
                if (!members) members = ld32(call(APIManager_getInstance, {}) + 0x32C);
                uint32_t key = f.alloc(8);
                st32(key, ld32(idAt));
                st32(key + 4, ld32(idAt + 4));
                uint32_t node = call(0x490275, {members + 0x24, key});
                if (node && int32_t(ld32(node + 0x10)) >= 0) {
                    uint32_t k = ld32(node + 0x10);
                    uint32_t list = members + 0x3C;
                    uint32_t count = uint32_t(int32_t(ld32(list + 4) - ld32(list)) >> 3) * 0x286BCA1Bu;
                    if (!(count > k)) {
                        call(wl::VECTOR_AT_THROW, {list});
                        return;
                    }
                    uint32_t str = ld32(list) + k * 0x98 + 0x88;
                    if (call(String_compareC, {str, S_EMPTY})) {
                        found = 1;
                        call(String_assign, {fbId, str});
                    }
                }
                break;
            }
        }
    }
    uint32_t status = findNode(actor, S_AVATAR_STATUS, 13);
    uint32_t panel = findNode(status, S_FB_PHOTO, 19);
    uint32_t loggedIn = call(SceneUtil_isLoggedInFacebook, {});
    uint32_t show = loggedIn & (found & visible);
    uint32_t cur = vcall(panel, node::isVisible, {panel});
    if (!(cur | (show ^ 1))) {
        uint32_t photo = findNode(panel, S_FB_PHOTO_IMG, 14);
        if (call(String_compareC, {fbId, S_EMPTY})) {
            uint32_t url = f.alloc(12);
            call(SceneManager_getFaceImageUrl, {url, call(SceneManager_getInstance, {}), fbId});
            uint32_t downloader = ld32(self + OFF_DOWNLOADER);
            if (call(String_compareC, {url, S_EMPTY})) {
                call(ImageDownloader_loadImage, {downloader, photo, cstrOf(url), cstrOf(fbId)});
            } else {
                // $_22 {this, photo, facebook id} sets the photo once downloaded
                uint32_t cap = f.alloc(0x14);
                st32(cap, self);
                st32(cap + 4, photo);
                uint32_t idText;
                if (strIsLong(fbId)) {
                    idText = ld32(fbId + 8);
                    call(fn::string_init, {cap + 8, ld32(fbId + 8), ld32(fbId + 4)});
                } else {
                    for (uint32_t k = 0; k < 12; k += 4) st32(cap + 8 + k, ld32(fbId + k));
                    idText = fbId + 1;
                }
                uint32_t fo = f.alloc(24);
                st32(fo + 16, 0);
                uint32_t heap = call(fn::operator_new, {0x18});
                if (heap) {
                    st32(heap, VT_profile_22);
                    st32(heap + 8, photo);
                    st32(heap + 4, self);
                    st32(heap + 0x14, ld32(cap + 0x10));
                    st32(heap + 0x10, ld32(cap + 0xC));
                    st32(heap + 0xC, ld32(cap + 8));
                    st32(cap + 8, 0);
                    st32(cap + 0xC, 0);
                    st32(cap + 0x10, 0);
                }
                st32(fo + 16, heap);
                call(ImageDownloader_addFacebookID, {downloader, idText, fo});
                functionDestroy(fo);
                strFree(cap + 8);
            }
            strFree(url);
        }
    }
    vcall(panel, node::setVisible, {panel, show});
    strFree(fbId);
}

// ------------------------------------------------------------------------------------------------ back to walking
namespace {
constexpr uint32_t M_zoomOutCamera = 0x643839;
constexpr uint32_t FadeIn_create = rwlib::cocos2d::FadeIn::create;
constexpr uint32_t VT_walk_26 = 0xF614FC, VT_walk_27 = 0xF614D0;
constexpr uint32_t S_AVATAR_LLU = 0xE03794;

// two std::strings equal (as inlined: the sizes, then memcmp / a byte loop)
bool strEqual(uint32_t a, uint32_t b) {
    uint32_t la = strIsLong(a) ? ld32(a + 4) : uint32_t(ld8(a) >> 1);
    uint32_t lb = strIsLong(b) ? ld32(b + 4) : uint32_t(ld8(b) >> 1);
    if (la != lb) return false;
    uint32_t pa = cstrOf(a), pb = cstrOf(b);
    for (uint32_t i = 0; i < la; i++)
        if (ld8(pa + i) != ld8(pb + i)) return false;
    return true;
}
// "avatar_<id>" as a std::string on the frame
uint32_t avatarName(Frame &f, uint32_t idAt) {
    uint32_t buf = stringBuffer(f, {S_AVATAR_LLU, ld32(idAt), ld32(idAt + 4)});
    return newStr(f, buf);
}
uint32_t nodeName(Frame &f, uint32_t node) {
    uint32_t out = f.alloc(12);
    call(ld32(ld32(node) + 0x150), {out, node});   // Node::getName (by value)
    return out;
}
}  // namespace

// Back from the ranking mode to walking: zoom out, header back ($_26); the party members (and the player) fade
// back in (the player found by its node name; if missing the first actor becomes the player, $_27)
void SceneMyPage_changeWalkingMode(uint32_t self) {
    if (!ld8(self + 0x2F9) || ld8(self + 0x2F0)) return;
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(M_zoomOutCamera, {self, empty});
        functionDestroy(empty);
    }
    call(M_resetAvatarAction, {0xAF, ld32(self + 0x2BC)});
    if (ld32(self + 0x32C)) {
        for (uint32_t i = 0; i < ld32(self + 0x32C); i++) {
            uint32_t w = ld32(self + OFF_VEC_328) + ((i >> 3) & 0x1FFFFFFCu);
            st32(w, ld32(w) & ~(1u << (i & 31)));
        }
    }
    call(smain::lib::Node_stopAllActions, {ld32(self + 0x29C)});
    call(smain::lib::Node_stopAllActions, {ld32(self + 0x2A4)});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_walk_26, {self});
        call(M_changeRankingHeader, {self, 0, fo});
        functionDestroy(fo);
    }
    if (ld8(self + OFF_HAS_PARTY) || ld32(self + 0x28C) == 1) {
        Frame f;
        uint32_t myName = avatarName(f, self + 0x3A0);
        const uint32_t actors = self + OFF_ACTORS_A;
        bool foundMe = false;
        if (int32_t(ld32(actors + 4) - ld32(actors)) >= 1) {
            int32_t i = 0;
            do {
                uint32_t actor = ld32(ld32(actors) + uint32_t(i) * 4);
                bool isMe;
                {
                    Frame g;
                    uint32_t name = nodeName(g, actor);
                    isMe = strEqual(name, myName);
                    strFree(name);
                }
                if (isMe) {
                    st32(self + 0x2BC, actor);
                    foundMe = true;
                    call(M_setAvatarProfileImageVisible, {self, actor, 1});
                    uint32_t avatar = ld32(actor + 0x288);
                    call(wl::Node_runAction, {avatar, call(FadeIn_create, {fb(0.6f)})});
                } else if (ld32(self + 0x394) != ld32(self + OFF_IDS2)) {
                    uint32_t j = 0;
                    do {
                        Frame g;
                        uint32_t idName = avatarName(g, ld32(self + OFF_IDS2) + j * 8);
                        uint32_t name = nodeName(g, actor);
                        bool same = strEqual(idName, name);
                        strFree(name);
                        if (same) {
                            call(M_setAvatarProfileImageVisible, {self, actor, 1});
                            uint32_t avatar = ld32(actor + 0x288);
                            call(wl::Node_runAction, {avatar, call(FadeIn_create, {fb(0.6f)})});
                        }
                        strFree(idName);
                        j++;
                    } while (j < uint32_t(int32_t(ld32(self + 0x394) - ld32(self + OFF_IDS2)) >> 3));
                }
                i++;
            } while (i < int32_t(ld32(actors + 4) - ld32(actors)) >> 2);
        }
        if (!foundMe) {
            // the first actor stands for the player (its id becomes the player's), fading out and back ($_27)
            uint32_t first = ld32(ld32(actors));
            uint32_t ids = ld32(self + OFF_IDS);
            st32(ids, ld32(self + 0x3A0));
            st32(ids + 4, ld32(self + 0x3A4));
            st32(self + 0x2BC, first);
            call(M_setAvatarProfileImageVisible, {self, first, 0});
            uint32_t avatar = ld32(first + 0x288);
            uint32_t out = call(FadeOut_create, {fb(0.6f)});
            Frame g;
            uint32_t fo = newFunction(g, VT_walk_27, {self, first});
            uint32_t cf = call(wl::CallFunc_create, {fo});
            call(wl::Node_runAction, {avatar, call(wl::Sequence_create, {out, cf, 0})});
            functionDestroy(fo);
        }
        strFree(myName);
    }
    uint32_t me = ld32(self + 0x2BC);
    call(M_setCameraPosition, {self, vcall(me, node::getPosition, {me}), fb(1.2f)});
}

// ------------------------------------------------------------------------------------------------ line-up scroll
namespace {
constexpr uint32_t ActorVector_erase = 0x63F9FD, ActorVector_pushBack = 0x6425D5;
constexpr uint32_t ActorStdVector_insert = 0x64A375;     // std::vector<AvatarActor*>::insert(iterator, const&)
constexpr uint32_t M_updateStyle = 0x6428FD, M_updateFukidashi = 0x642C39, M_getAvatarLineupStartX = 0x6460F5;
constexpr uint32_t M_clearMotionEvents = 0x63FA5B, M_addChangeMotionEvent = 0x63FA8D;
inline bool nanf_(float v) { return v != v; }
int32_t floorIndex(float pos) { return scene::f2i(std::floor(pos / -60.0f)); }

// actor shows member `j` of the line-up (its id in the actor ids, style, status balloon, row height)
void showLineupMember(uint32_t self, uint32_t actor, uint32_t j, uint32_t lo, uint32_t hi, bool visibleFirst) {
    (void)visibleFirst;
    const uint32_t actors = self + OFF_ACTORS_A;
    uint32_t begin = ld32(actors);
    int32_t bytes = int32_t(ld32(actors + 4) - begin);
    if (bytes >= 1) {
        int32_t k = 0;
        do {
            if (ld32(begin + uint32_t(k) * 4) == actor) {
                uint32_t at = ld32(self + OFF_IDS) + uint32_t(k) * 8;
                st32(at, lo);
                st32(at + 4, hi);
                break;
            }
            k++;
        } while (k < (bytes >> 2));
    }
}
float rowY(int32_t j) { return float(j % 2) * -10.0f + 70.0f; }
}  // namespace

// Scroll the ranking line-up by `delta` (clamped with a rubber band on the field shake), recycling the actors that
// leave one side for the members entering on the other; returns the scroll applied (0 at the ends)
float SceneMyPage_updateScrollSortAvatars(uint32_t self, float delta) {
    if (!ld8(self + 0x3A9) || ld8(self + 0x2F0)) return 0.0f;
    if (!ld8(self + OFF_HAS_PARTY) && ld32(self + 0x28C) != 1) return 0.0f;
    const bool still = delta == 0.0f || nanf_(delta);
    const uint32_t absBits = fb(delta) & 0x7FFFFFFFu;
    const float pos = ldf(self + 0x388);
    float newPos = pos + delta;
    float facing;
    if (still) {
        facing = ldf(self + 0x380);
    } else {
        float s = ldf(self + 0x3AC);
        if (delta < 0.0f || nanf_(delta)) {
            if (!(s <= 0.0f || nanf_(s))) st32(self + 0x3AC, 0xBDCCCCCDu);   // -0.1
            facing = 1.0f;
        } else {
            if (!(s >= 0.0f || nanf_(s))) st32(self + 0x3AC, 0x3DCCCCCDu);   // 0.1
            facing = -1.0f;
        }
        stf(self + 0x380, facing);
    }
    int32_t over = int32_t(uint32_t(int32_t(ld32(self + 0x394) - ld32(self + OFF_IDS2)) >> 3) - ld32(self + OFF_COLUMNS));
    const int32_t maxIdx = over >= 0 ? over + 1 : 0;
    int32_t oldIdx = 0;
    {
        int32_t i = floorIndex(pos);
        if (i >= 1) oldIdx = maxIdx < i ? maxIdx : i;
    }
    const float limit = float(maxIdx) * 60.0f;
    float result;
    float base;
    if (!(newPos >= -0.0f - limit || nanf_(newPos))) {
        // past the end: the field shakes left
        if (!(delta >= 0.0f || nanf_(delta))) {
            float shake = ldf(self + 0x3C4) + delta * 0.5f;
            stf(self + 0x3C4, shake);
            float lim = -0.0f - ldf(self + 0x3C8);
            if (!(shake >= lim || nanf_(shake) || nanf_(lim))) {
                stf(self + 0x3C4, lim);
                st32(self + 0x3AC, 0xBDCCCCCDu);
            }
        }
        base = 1.0f - limit;
        stf(self + 0x388, base);
        result = 0.0f;
    } else if (newPos < 0.0f || nanf_(newPos)) {
        base = pos;
        result = delta;
    } else {
        // before the start: the field shakes right
        if (!(delta <= 0.0f || nanf_(delta))) {
            float shake = ldf(self + 0x3C4) + delta * 0.5f;
            stf(self + 0x3C4, shake);
            float lim = ldf(self + 0x3C8);
            if (!(lim >= shake || nanf_(lim) || nanf_(shake))) {
                stf(self + 0x3C4, lim);
                st32(self + 0x3AC, 0x3DCCCCCDu);
            }
        }
        base = -1.0f;
        st32(self + 0x388, 0xBF800000u);
        result = 0.0f;
    }
    const float finalPos = result + base;
    stf(self + 0x388, finalPos);
    int32_t newIdx = 0;
    bool clampedOrLow = true;
    {
        int32_t i = floorIndex(finalPos);
        if (i >= 1) {
            clampedOrLow = maxIdx < i;
            newIdx = maxIdx < i ? maxIdx : i;
        }
    }
    const int32_t diff = newIdx - oldIdx;
    const int32_t steps = diff < 0 ? -diff : diff;
    const uint32_t lineup = self + OFF_ACTORS_B, ids2 = self + OFF_IDS2;
    if (newIdx > oldIdx) {
        for (int32_t k = 0; k < steps; k++) {
            // the first actor moves to the end and shows the next member
            uint32_t first = ld32(ld32(lineup));
            call(ActorVector_erase, {lineup, ld32(lineup)});
            call(ActorVector_pushBack, {lineup, first});
            uint32_t j = uint32_t(oldIdx + k) + uint32_t(int32_t(ld32(lineup + 4) - ld32(lineup)) >> 2);
            uint32_t lo = 0, hi = 0;
            if (j < uint32_t(int32_t(ld32(ids2 + 4) - ld32(ids2)) >> 3)) {
                uint32_t at = ld32(ids2) + j * 8;
                lo = ld32(at);
                hi = ld32(at + 4);
                vcall(first, node::setVisible, {first, 1});
            } else {
                vcall(first, node::setVisible, {first, 0});
            }
            showLineupMember(self, first, j, lo, hi, true);
            call(M_updateStyle, {self, first, lo, hi});
            call(M_updateFukidashi, {self, first, lo, hi, 1});
            vcall(first, node::setPositionY, {first, fb(rowY(int32_t(j)))});
        }
    } else if (newIdx < oldIdx) {
        for (int32_t k = 0; k < steps; k++) {
            // the last actor moves to the front and shows the previous member
            uint32_t b = ld32(lineup), e = ld32(lineup + 4);
            uint32_t last = ld32(b + ((e - b) & ~3u) - 4);
            call(ActorVector_erase, {lineup, e - 4});
            Frame f;
            uint32_t slot = f.alloc(4);
            st32(slot, last);
            call(ActorStdVector_insert, {lineup, ld32(lineup), slot});
            call(smain::lib::Ref_retain, {ld32(slot)});
            uint32_t j = uint32_t(newIdx + steps - 1 - k);
            if (!(uint32_t(int32_t(ld32(ids2 + 4) - ld32(ids2)) >> 3) > j)) {
                call(wl::VECTOR_AT_THROW, {ids2});
                return 0.0f;
            }
            uint32_t at = ld32(ids2) + j * 8;
            uint32_t lo = ld32(at), hi = ld32(at + 4);
            call(M_updateStyle, {self, last, lo, hi});
            call(M_updateFukidashi, {self, last, lo, hi, 1});
            showLineupMember(self, last, j, lo, hi, false);
            vcall(last, node::setVisible, {last, 1});
            vcall(last, node::setPositionY, {last, fb(rowY(int32_t(j)))});
        }
    }
    int32_t rem = scene::f2i(ldf(self + 0x388)) % 60;
    int32_t offset;
    if (clampedOrLow) {
        offset = 0;
        if (maxIdx != 0 && newIdx >= 0) offset = maxIdx > newIdx ? rem : -60;
    } else {
        offset = rem;
    }
    if (bf(absBits) < 0.0f || nanf_(bf(absBits))) {
        call(M_clearMotionEvents, {self});
        st32(self + 0x3D0, 0x1B);
        if (int32_t(ld32(lineup + 4) - ld32(lineup)) >= 1) {
            int32_t i = 0;
            do {
                call(M_addChangeMotionEvent, {self, ld32(ld32(lineup) + uint32_t(i) * 4), 0x1B});
                i++;
            } while (i < int32_t(ld32(lineup + 4) - ld32(lineup)) >> 2);
        }
    }
    const float startX = bf(call(M_getAvatarLineupStartX, {self}));
    if (int32_t(ld32(lineup + 4) - ld32(lineup)) >= 1) {
        const float off = float(offset);
        int32_t i = 0;
        do {
            uint32_t actor = ld32(ld32(lineup) + uint32_t(i) * 4);
            float x = off + (startX + float(i) * 60.0f);
            vcall(actor, node::setPositionX, {actor, fb(x)});
            uint32_t avatar = ld32(actor + 0x288);
            vcall(avatar, node::setScaleX, {avatar, fb(facing)});
            i++;
        } while (i < int32_t(ld32(lineup + 4) - ld32(lineup)) >> 2);
    }
    return result;
}

RW_FUNCTION(0x63FC48, "SceneMyPage::updateScrollSortAvatars", SceneMyPage_updateScrollSortAvatars);
RW_FUNCTION(0x63F464, "SceneMyPage::changeWalkingMode", SceneMyPage_changeWalkingMode);
RW_FUNCTION(0x643604, "SceneMyPage::resetAvatarAction", SceneMyPage_resetAvatarAction);
RW_FUNCTION(0x6436F4, "SceneMyPage::zoomInCamera", SceneMyPage_zoomInCamera);
RW_FUNCTION(0x643838, "SceneMyPage::zoomOutCamera", SceneMyPage_zoomOutCamera);
RW_FUNCTION(0x64397C, "SceneMyPage::changeRankingMode", SceneMyPage_changeRankingMode);
RW_FUNCTION(0x641EF4, "SceneMyPage::setAvatarProfileImageVisible", SceneMyPage_setAvatarProfileImageVisible);
