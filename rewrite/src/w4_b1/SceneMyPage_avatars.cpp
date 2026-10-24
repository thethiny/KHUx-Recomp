// SceneMyPage: avatar actors on the field - queued motion changes, placing, moving and their walking AI.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t ActorVector_clear = 0x485BA3, ActorVector_pushBack = 0x6425D5;
constexpr uint32_t MotionVector_pushSlow = 0x64C431;      // vector<Avatar::Motion>::__push_back_slow_path(&&)
constexpr uint32_t UIntVector_copy = 0x641E91;
constexpr uint32_t Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t FadeOut_create = rwlib::cocos2d::FadeOut::create, FadeIn_create = rwlib::cocos2d::FadeIn::create, Spawn_create = rwlib::cocos2d::Spawn::create;
constexpr uint32_t Text_setTextHorizontalAlignment = rwlib::cocos2d::ui::Text::setTextHorizontalAlignment;
constexpr uint32_t MemberIndex_find = 0x490275;          // unordered_map<uint64, int>::find
constexpr uint32_t M_addChangeMotionEvent = 0x63FA8D, M_moveAvatar = 0x6411CD, M_setCameraPosition = 0x640851;
constexpr uint32_t M_setAvatarProfileImageVisible = 0x641EF5;
constexpr uint32_t VT_action_17 = 0xF616B4, VT_action_18 = 0xF61688, VT_action_19 = 0xF6165C;
constexpr uint32_t S_FUKIDASHI = 0xE03781, S_TXT_COMMENT = 0xDFE1CC;
// Avatar::Motion values used here
constexpr uint32_t MOTION_RUN = 0x18, MOTION_IDLE = 0x1B, MOTION_WALK = 0x1C;

int32_t randomInt(int32_t lo, int32_t hi) {
    return int32_t(call(Random_generateUniformInt, {call(Random_getInstance, {}), uint32_t(lo), uint32_t(hi), 0}));
}
uint32_t vecCount(uint32_t vec, uint32_t elemShift) { return uint32_t(int32_t(ld32(vec + 4) - ld32(vec)) >> elemShift); }

// actor->setLocalZOrder(field height - y)
void zOrderByY(uint32_t self, uint32_t actor) {
    uint32_t panel = ld32(self + 0x298);
    float h = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
    float y = bf(vcall(actor, node::getPositionY, {actor}));
    vcall(actor, node::setLocalZOrder, {actor, uint32_t(scene::f2i(h - y))});
}

// vector<bool> bit
uint32_t bitWord(uint32_t vec, uint32_t i) { return ld32(vec) + ((i >> 3) & 0x1FFFFFFCu); }
}  // namespace

void SceneMyPage_clearMotionEvents(uint32_t self) {
    call(ActorVector_clear, {self + OFF_ACTORS});
    uint32_t e = ld32(self + 0x35C), b = ld32(self + OFF_MOTIONS);
    if (e != b) st32(self + 0x35C, e + (~3u & ~((e - 4) - b)));
}

// queue a motion change for the actor (applied one per frame in update)
void SceneMyPage_addChangeMotionEvent(uint32_t self, uint32_t actor, uint32_t motion) {
    Frame f;
    uint32_t value = f.alloc(4);
    st32(value, motion);
    call(ActorVector_pushBack, {self + OFF_ACTORS, actor});
    uint32_t end = ld32(self + 0x35C);
    if (end != ld32(self + 0x360)) {
        uint32_t e = 0;
        if (end) {
            st32(end, motion);
            e = ld32(self + 0x35C);
        }
        st32(self + 0x35C, e + 4);
    } else {
        call(MotionVector_pushSlow, {self + OFF_MOTIONS, value});
    }
}

// place the actor at a field point (divided by the field scale), z order by height
void SceneMyPage_setAvatrPosition(uint32_t self, uint32_t actor, uint32_t pos) {
    uint32_t panel = ld32(self + 0x298);
    float x = ldf(pos) / bf(vcall(panel, node::getScale, {panel}));
    panel = ld32(self + 0x298);
    float y = ldf(pos + 4) / bf(vcall(panel, node::getScale, {panel}));
    {
        Frame f;
        uint32_t v = f.alloc(8);
        call(wl::Vec2_ctorXY, {v, fb(x), fb(y)});
        vcall(actor, node::setPosition, {actor, v});
        call(wl::Vec2_dtor, {v});
    }
    zOrderByY(self, actor);
}

// walk the actor to `pos` (facing the direction), then `done`; the camera follows the player in ranking mode
void SceneMyPage_moveAvatar(uint32_t self, uint32_t actor, uint32_t pos, uint32_t done, float duration) {
    Frame f;
    uint32_t target = f.alloc(8);
    call(fn::Vec2_ctor_copy, {target, pos});
    uint32_t avatar = ld32(actor + 0x288);
    float x = bf(vcall(actor, node::getPositionX, {actor}));
    float tx = ldf(target);
    bool left = x >= tx || x != x || tx != tx;
    vcall(avatar, node::setScaleX, {avatar, left ? fb(1.0f) : fb(-1.0f)});
    uint32_t move = call(wl::MoveTo_create, {fb(duration), target});
    uint32_t cf = call(wl::CallFunc_create, {done});
    call(wl::Node_runAction, {actor, call(wl::Sequence_create, {move, cf, 0})});
    if (ld32(self + 0x2BC) == actor && ld8(self + 0x2F0)) call(M_setCameraPosition, {self, target, fb(duration)});
    call(wl::Vec2_dtor, {target});
}

// The walking AI of a party member / the player: pick a neighbour point, then walk (60 %), run (30 %) or say the
// member's comment in a free balloon (10 %); some members teleport through the doors (points 1 / 5). $_19 runs
// the next action when this one ends.
void SceneMyPage_runAvatrAction(uint32_t self, uint32_t actor, uint32_t index) {
    {
        uint32_t id = ld32(self + OFF_IDS) + index * 8;
        if (!(ld32(id) | ld32(id + 4))) return;
    }
    zOrderByY(self, actor);
    const bool isMe = ld32(self + 0x2BC) == actor;
    uint32_t curAt;
    if (isMe) {
        curAt = self + 0x2EC;
    } else {
        if (!(vecCount(self + OFF_ROUTE, 2) > index)) {
            call(wl::VECTOR_AT_THROW, {self + OFF_ROUTE});
            return;
        }
        curAt = ld32(self + OFF_ROUTE) + index * 4;
    }
    const uint32_t cur = ld32(curAt);
    {
        uint32_t routes = self + OFF_ROUTES;
        uint32_t n = uint32_t(int32_t(ld32(routes + 4) - ld32(routes)) >> 2) * 0xAAAAAAABu;
        if (!(n > cur)) {
            call(wl::VECTOR_AT_THROW, {routes});
            return;
        }
    }
    Frame f;
    uint32_t nexts = f.alloc(12);
    call(UIntVector_copy, {nexts, ld32(self + OFF_ROUTES) + cur * 12});
    int32_t r = randomInt(0, int32_t(vecCount(nexts, 2)) - 1);
    if (!(vecCount(nexts, 2) > uint32_t(r))) {
        call(wl::VECTOR_AT_THROW, {nexts});
        return;
    }
    uint32_t target = ld32(ld32(nexts) + uint32_t(r) * 4);
    if (!(vecCount(self + OFF_POINTS, 3) > target)) {
        call(wl::VECTOR_AT_THROW, {self + OFF_POINTS});
        return;
    }
    uint32_t dest = f.alloc(8);
    call(fn::Vec2_ctor_copy, {dest, ld32(self + OFF_POINTS) + target * 8});
    stf(dest, ldf(dest) + float(randomInt(0, 30) - 15));
    stf(dest + 4, ldf(dest + 4) - float(randomInt(0, 10)));
    int32_t roll = randomInt(0, 99);
    uint32_t kind = roll < 60 ? 0 : roll < 90 ? 1 : roll < 100 ? 2 : 0;

    uint32_t action = 0;
    bool teleport = false;
    if (!isMe) {
        uint32_t avatar = ld32(actor + 0x288);
        uint32_t opacity = vcall(avatar, node::getOpacity, {avatar});
        if (roll <= 9 && opacity == 0xFF) {
            // through a door: to point 5 (from 0..2) or 1 (from 4..6)
            uint32_t points = self + OFF_POINTS;
            if (cur <= 2) {
                if (!(vecCount(points, 3) > 5)) {
                    call(wl::VECTOR_AT_THROW, {points});
                    return;
                }
                st32(dest, ld32(ld32(points) + 0x28));
                st32(dest + 4, ld32(ld32(points) + 0x2C));
                target = 5;
                teleport = true;
            } else if (cur - 4 <= 2) {
                if (!(vecCount(points, 3) > 1)) {
                    call(wl::VECTOR_AT_THROW, {points});
                    return;
                }
                st32(dest, ld32(ld32(points) + 8));
                st32(dest + 4, ld32(ld32(points) + 0xC));
                target = 1;
                teleport = true;
            }
        }
    }
    if (teleport || kind == 3) {
        // fade out / in, then $_18 puts the actor at the destination
        call(M_addChangeMotionEvent, {self, actor, MOTION_IDLE});
        call(wl::DelayTime_create, {fb(1.2f)});
        st32(ld32(self + OFF_ROUTE) + index * 4, target);
        uint32_t pos = f.alloc(8);
        call(fn::Vec2_ctor_copy, {pos, dest});
        call(M_setAvatarProfileImageVisible, {self, actor, 0});
        uint32_t avatar = ld32(actor + 0x288);
        uint32_t out = call(FadeOut_create, {fb(0.6f)});
        call(wl::Node_runAction, {avatar, call(wl::Sequence_create, {out, call(FadeIn_create, {fb(0.6f)}), 0})});
        uint32_t d1 = call(wl::DelayTime_create, {fb(0.6f)});
        uint32_t d2 = call(wl::DelayTime_create, {fb(0.6f)});
        uint32_t posCopy = f.alloc(8);
        call(fn::Vec2_ctor_copy, {posCopy, pos});
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_action_18);
            st32(heap + 4, self);
            st32(heap + 8, actor);
            call(fn::Vec2_ctor_copy, {heap + 0xC, posCopy});
        }
        st32(fo + 16, heap);
        uint32_t cf = call(wl::CallFunc_create, {fo});
        uint32_t spawn = call(Spawn_create, {d2, cf, 0});
        action = call(wl::Sequence_create, {d1, spawn, 0});
        functionDestroy(fo);
        call(wl::Vec2_dtor, {posCopy});
        call(wl::Vec2_dtor, {pos});
    } else if (kind == 0 || kind == 1) {
        // walk (4.5 s) / run (2.3 s) to the destination
        if (isMe) st32(self + 0x2EC, target);
        else st32(ld32(self + OFF_ROUTE) + index * 4, target);
        float duration = kind == 0 ? 4.5f : 2.3f;
        call(M_addChangeMotionEvent, {self, actor, kind == 0 ? MOTION_WALK : MOTION_RUN});
        uint32_t pos = f.alloc(8);
        call(fn::Vec2_ctor_copy, {pos, dest});
        Frame g;
        uint32_t empty = newEmptyFunction(g);
        call(M_moveAvatar, {self, actor, pos, empty, fb(duration)});
        functionDestroy(empty);
        call(wl::Vec2_dtor, {pos});
        action = call(wl::DelayTime_create, {fb(duration)});
    } else {
        // the comment in a free balloon over the actor (5 s), when its point is free and it is not fading
        const uint32_t shown = self + OFF_BALOON_SHOWN;
        int32_t free = -1;
        uint32_t n = ld32(shown + 4);
        for (uint32_t i = 0; i < n; i++) {
            if (!(ld32(bitWord(shown, i)) & (1u << (i & 31)))) {
                free = int32_t(i);
                break;
            }
        }
        const uint32_t occupied = self + OFF_VEC_328;
        if (!(ld32(occupied + 4) > cur)) {
            call(wl::VECTOR_AT_THROW, {occupied});
            return;
        }
        const uint32_t mask = 1u << (cur & 31);
        const uint32_t wordOff = (cur >> 5) * 4;
        bool busy = free < 0 || (ld32(ld32(occupied) + wordOff) & mask);
        if (!busy) {
            uint32_t avatar = ld32(actor + 0x288);
            busy = vcall(avatar, node::getOpacity, {avatar}) != 0xFF;
        }
        if (busy) {
            call(M_addChangeMotionEvent, {self, actor, MOTION_IDLE});
            action = call(wl::DelayTime_create, {fb(3.0f)});
        } else {
            uint32_t balloon;
            {
                Frame g;
                uint32_t buf = stringBuffer(g, {S_FUKIDASHI, uint32_t(free)});
                uint32_t name = newStr(g, buf);
                balloon = SceneUtil::searchNodeByName(ld32(self + 0x2A4), name);
                strFree(name);
            }
            uint32_t label = findNode(balloon, S_TXT_COMMENT, 17);
            call(Text_setTextHorizontalAlignment, {label, 3});
            if ((ld8(self + OFF_HAS_PARTY) || ld32(self + 0x28C) == 1) && ld32(self + 0x2BC) != actor) {
                // a party member's comment
                uint32_t members = ld32(self + 0x2C0);
                if (members) {
                    Frame g;
                    uint32_t key = g.alloc(8);
                    uint32_t idAt = ld32(self + OFF_IDS) + index * 8;
                    st32(key, ld32(idAt));
                    st32(key + 4, ld32(idAt + 4));
                    uint32_t node = call(MemberIndex_find, {members + 0x24, key});
                    uint32_t i = node ? ld32(node + 0x10) : 0xFFFFFFFFu;
                    uint32_t list = members + 0x3C;
                    uint32_t count = uint32_t(int32_t(ld32(list + 4) - ld32(list)) >> 3) * 0x286BCA1Bu;
                    if (!(count > i)) {
                        call(wl::VECTOR_AT_THROW, {list});
                        return;
                    }
                    uint32_t entry = ld32(list) + i * 0x98;
                    uint32_t copy = g.alloc(0x94);
                    call(wl::MEMCPY, {copy, entry, 0x7C});
                    uint32_t comment = copy + 0x7C, other = copy + 0x88;
                    if (strIsLong(entry + 0x7C)) call(fn::string_init, {comment, ld32(entry + 0x84), ld32(entry + 0x80)});
                    else for (uint32_t k = 0; k < 12; k += 4) st32(comment + k, ld32(entry + 0x7C + k));
                    if (strIsLong(entry + 0x88)) call(fn::string_init, {other, ld32(entry + 0x90), ld32(entry + 0x8C)});
                    else for (uint32_t k = 0; k < 12; k += 4) st32(other + k, ld32(entry + 0x88 + k));
                    const bool commentLong = strIsLong(comment);
                    uint32_t text = commentLong ? ld32(comment + 8) : comment + 1;
                    uint32_t str = newStr(g, text);
                    call(fn::Text_setString, {label, str});
                    strFree(str);
                    strFree(other);
                    if (commentLong) call(fn::operator_delete, {ld32(comment + 8)});
                }
            } else {
                // the player's own comment (user detail +0x3C, 257 bytes)
                uint32_t api = call(APIManager_getInstance, {});
                uint32_t detail = ld32(api + 0x74), ctl = ld32(api + 0x78);
                addRef(ctl);
                Frame g;
                uint32_t buf = g.alloc(0x104);
                call(SafeBuffer_read, {detail, buf, ld32(detail + 0xC) + 0x3C, 0x101, 0x3C});
                uint32_t str = newStr(g, buf);
                call(fn::Text_setString, {label, str});
                strFree(str);
                release(ctl);
            }
            float scale = ldf(self + OFF_SCALE) / ldf(self + OFF_SCALE_BASE);
            float x = bf(vcall(actor, node::getPositionX, {actor}));
            float y = bf(vcall(actor, node::getPositionY, {actor}));
            float up = scale * 150.0f;
            float px = x + scale * -130.0f;
            float py = up + y;
            {
                Frame g;
                uint32_t v = g.alloc(8);
                call(wl::Vec2_ctorXY, {v, fb(px), fb(py)});
                vcall(balloon, node::setPosition, {balloon, v});
                call(wl::Vec2_dtor, {v});
            }
            vcall(balloon, node::setVisible, {balloon, 1});
            {
                uint32_t w = bitWord(shown, uint32_t(free));
                st32(w, ld32(w) | (1u << (uint32_t(free) & 31)));
            }
            call(M_addChangeMotionEvent, {self, actor, MOTION_IDLE});
            uint32_t wait = call(wl::DelayTime_create, {fb(5.0f)});
            uint32_t fo = f.alloc(24);
            st32(fo + 16, 0);
            uint32_t heap = call(fn::operator_new, {0x14});
            if (heap) {
                st32(heap, VT_action_17);
                st32(heap + 4, balloon);
                st32(heap + 8, self);
                st32(heap + 0xC, cur);
                st32(heap + 0x10, uint32_t(free));
            }
            st32(fo + 16, heap);
            uint32_t cf = call(wl::CallFunc_create, {fo});
            action = call(wl::Sequence_create, {wait, cf, 0});
            functionDestroy(fo);
            uint32_t w = ld32(occupied) + wordOff;
            st32(w, ld32(w) | mask);
        }
    }
    {
        Frame g;
        uint32_t fo = newFunction(g, VT_action_19, {self, actor, index});
        uint32_t cf = call(wl::CallFunc_create, {fo});
        call(wl::Node_runAction, {actor, call(wl::Sequence_create, {action, cf, 0})});
        functionDestroy(fo);
    }
    call(wl::Vec2_dtor, {dest});
    call(UIntVector_baseDtor, {nexts});
}

RW_FUNCTION(0x63FA5A, "SceneMyPage::clearMotionEvents", SceneMyPage_clearMotionEvents);
RW_FUNCTION(0x63FA8C, "SceneMyPage::addChangeMotionEvent", SceneMyPage_addChangeMotionEvent);
RW_FUNCTION(0x6410D4, "SceneMyPage::setAvatrPosition", SceneMyPage_setAvatrPosition);
RW_FUNCTION(0x6411CC, "SceneMyPage::moveAvatar", SceneMyPage_moveAvatar);
RW_FUNCTION(0x6412E8, "SceneMyPage::runAvatrAction", SceneMyPage_runAvatrAction);

// ------------------------------------------------------------------------------------------------ creation
namespace {
constexpr uint32_t AvatarActor_create = 0x73FD21, AvatarActor_setStyle = 0x740635;
constexpr uint32_t Avatar_preloadResources = 0x5C7ED1;
constexpr uint32_t Node_create = rwlib::cocos2d::Node::create;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;      // (Style* out, const Coordinate&, keyblade)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t BattleUtil_getCurrentKeybladeIndex = 0x496F4D;
constexpr uint32_t M_updateStyle = 0x6428FD, M_addAvatarBaloon = 0x64263D, M_updateFukidashi = 0x642C39;
constexpr uint32_t VT_create_21 = 0xF615D8;
constexpr uint32_t ANCHOR_BOTTOM_LEFT = 0x1006958;
constexpr uint32_t S_AVATAR_LLU = 0xE03794, S_BALOON_JSON = 0xE037A0, S_AVATAR_BALOON = 0xE037CC;
constexpr uint32_t S_BALLOON_BLUE = 0xE037DA, S_TXT_NAME = 0xDFDA10, S_TXT_WORDING = 0xDF862F;
constexpr uint32_t S_FB_PHOTO = 0xDF8E5A, S_AVATAR_STATUS = 0xE037FA, S_RIGHT_UI = 0xDF85F7;
constexpr uint32_t S_BALOON_PRESENT = 0xE038B3;

// actor->setName("avatar_<id>")
void nameActor(uint32_t actor, uint32_t lo, uint32_t hi) {
    Frame f;
    uint32_t buf = stringBuffer(f, {S_AVATAR_LLU, lo, hi});
    uint32_t name = newStr(f, buf);
    vcall(actor, node::setName, {actor, name});
    strFree(name);
}
}  // namespace

// An actor for user `id`: its style, the status balloon, the comment balloon, and a touch area ($_21)
uint32_t SceneMyPage_createPartyAvatar(uint32_t self, uint32_t, uint32_t lo, uint32_t hi) {
    uint32_t actor = call(AvatarActor_create, {0});
    call(M_updateStyle, {self, actor, lo, hi});
    uint32_t avatar = ld32(actor + 0x288);
    vcall(avatar, node::setCascadeOpacityEnabled, {avatar, 1});
    call(Avatar_preloadResources, {ld32(actor + 0x288)});
    vcall(actor, node::setVisible, {actor, 1});
    vcall(actor, node::setScale, {actor, fb(ldf(self + OFF_SCALE) / ldf(self + OFF_SCALE_BASE))});
    avatar = ld32(actor + 0x288);
    st8(avatar + 0x564, 0);
    call(M_addAvatarBaloon, {avatar, actor});   // (r0 still holds the avatar: `this` is not used)
    call(M_updateFukidashi, {self, actor, lo, hi, 0});
    Frame f;
    uint32_t area = call(Node_create, {});
    {
        uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fb(100.0f), fb(200.0f)});
        vcall(area, node::setContentSize, {area, size});
    }
    vcall(area, node::setAnchorPoint, {area, ANCHOR_BOTTOM_LEFT});
    vcall(area, node::setPositionX, {area, fb(ldf(vcall(area, node::getContentSize, {area})) * -0.5f)});
    uint32_t empty = newStr(f, S_EMPTY, 0);
    uint32_t button = call(ImageButton_create, {empty, 0});
    strFree(empty);
    call(ImageButton_setTargetView, {button, area, fb(1.0f)});
    vcall(area, node::addChild, {area, button});
    vcall(actor, node::addChild, {actor, area});
    uint32_t fo = newFunction(f, VT_create_21, {self, actor});
    call(ImageButton_addTouchEventListener, {button, fo});
    functionDestroy(fo);
    return actor;
}

// the status balloon over an actor (name / wording, hidden Facebook photo), named avatar_status
void SceneMyPage_addAvatarBaloon(uint32_t, uint32_t actor) {
    uint32_t w = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_BALOON_JSON});
    {
        Frame f;
        uint32_t v = f.alloc(8);
        call(wl::Vec2_ctorXY, {v, fb(-55.0f), fb(150.0f)});
        vcall(w, node::setPosition, {w, v});
        call(wl::Vec2_dtor, {v});
    }
    vcall(w, node::setScale, {w, 0x3F333333u});   // 0.7
    vcall(w, node::setVisible, {w, 1});
    uint32_t image = findNode(w, S_AVATAR_BALOON, 13);
    {
        Frame f;
        uint32_t path = newStr(f, S_BALLOON_BLUE, 31);
        call(ImageView_loadTexture, {image, path, 0});
        strFree(path);
    }
    vcall(image, node::setVisible, {image, 0});
    call(SceneUtil_setShadow, {findNode(w, S_TXT_NAME, 8)});
    call(SceneUtil_setShadow, {findNode(w, S_TXT_WORDING, 11)});
    uint32_t photo = findNode(w, S_FB_PHOTO, 19);
    vcall(photo, node::setVisible, {photo, 0});
    Frame f;
    uint32_t name = newStr(f, S_AVATAR_STATUS, 13);
    vcall(actor, node::addChildZName, {actor, w, 0, name});
    strFree(name);
}

// the actor's look: a party member's (from the member list) or the player's (current coordinate + keyblade)
void SceneMyPage_updateStyle(uint32_t self, uint32_t actor, uint32_t lo, uint32_t hi) {
    if ((ld8(self + OFF_HAS_PARTY) || ld32(self + 0x28C) == 1) && ld32(self + 0x2C0)) {
        nameActor(actor, lo, hi);
        Frame f;
        uint32_t key = f.alloc(8);
        st32(key, lo);
        st32(key + 4, hi);
        uint32_t node = call(0x490275, {ld32(self + 0x2C0) + 0x24, key});   // unordered_map<uint64,int>::find
        if (!node || int32_t(ld32(node + 0x10)) < 0) {
            vcall(actor, node::setVisible, {actor, 0});
            return;
        }
        uint32_t i = ld32(node + 0x10);
        vcall(actor, node::setVisible, {actor, 1});
        uint32_t member = f.alloc(0x128);
        st8(member + 0x11F, ld8(member + 0x11F) & 0xFD);
        uint32_t sb = ld32(self + 0x2C0);
        call(SafeBuffer_read, {sb, member, ld32(sb + 0xC) + i * 0x128, 0x128, i * 0x128});
        uint32_t style = f.alloc(0x24);
        call(AvatarUtil_makeStyle, {style, member + 0xE8, ld32(member + 0x40)});
        call(AvatarActor_setStyle, {actor, style});
        call(SafeBuffer_dtor, {style});
        return;
    }
    if (lo != ld32(self + 0x3A0) || hi != ld32(self + 0x3A4)) return;
    uint32_t api = call(APIManager_getInstance, {});
    int32_t idx = int32_t(call(BattleUtil_getCurrentKeybladeIndex, {}));
    uint32_t kbs = ld32(api + 0xEC);
    uint32_t keyblade = uint32_t(scene::safe32(kbs, idx == -1 ? 0xC : uint32_t(idx) * 0x58 + 0xC));
    uint32_t coordinateId = uint32_t(scene::safe32(ld32(call(APIManager_getInstance, {}) + 0x94), 0x3C));
    uint32_t listAt = call(APIManager_getInstance, {}) + 0xC4;
    if (!ld32(ld32(listAt) + 0x20)) return;
    Frame f;
    uint32_t coord = f.alloc(0x30);
    for (uint32_t i = 0, off = 0; i < ld32(ld32(listAt) + 0x20); i++, off += 0x30) {
        uint32_t list = ld32(listAt);
        call(SafeBuffer_read, {list, coord, ld32(list + 0xC) + off, 0x30, off});
        if (uint32_t(scene::safe32(ld32(listAt), off)) != coordinateId) continue;
        uint32_t style = f.alloc(0x24);
        call(AvatarUtil_makeStyle, {style, coord, keyblade});
        call(AvatarActor_setStyle, {actor, style});
        call(SafeBuffer_dtor, {style});
        nameActor(actor, lo, hi);
        return;
    }
}

// the "new" balloon of the present box (count of presents: a refreshed list or the my page info)
void SceneMyPage_updatePresentBoxIcon(uint32_t self, uint32_t refreshed) {
    uint32_t api = call(APIManager_getInstance, {});
    uint32_t countAt = refreshed == 1 ? ld32(api + 0x104) + 0xC : ld32(api + 0x314) + 0x18;
    int32_t count = int32_t(ld32(countAt));
    uint32_t right = findNode(ld32(self + 0x290), S_RIGHT_UI, 7);
    uint32_t balloon = findNode(right, S_BALOON_PRESENT, 17);
    vcall(balloon, node::setVisible, {balloon, count > 0 ? 1u : 0u});
}

RW_FUNCTION(0x642394, "SceneMyPage::createPartyAvatar", SceneMyPage_createPartyAvatar);
RW_FUNCTION(0x64263C, "SceneMyPage::addAvatarBaloon", SceneMyPage_addAvatarBaloon);
RW_FUNCTION(0x6428FC, "SceneMyPage::updateStyle", SceneMyPage_updateStyle);
RW_FUNCTION(0x6462F8, "SceneMyPage::updatePresentBoxIcon", SceneMyPage_updatePresentBoxIcon);

// ------------------------------------------------------------------------------------------------ status balloon
namespace {
constexpr uint32_t String_compareC = 0x47CBD5;          // std::string::compare(const char*)
constexpr uint32_t SceneManager_getFaceImageUrl = 0x621D0D;   // (string* out, mgr, const string& id)
constexpr uint32_t ImageDownloader_loadImage = 0x5BD0CB;      // (dl, ImageView*, url, id)
constexpr uint32_t ImageDownloader_addFacebookID = 0x5BD9A1;  // (dl, id, function<void(bool, const char*)>)
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;          // (string* out, uint64)
constexpr uint32_t VT_fukidashi_32 = 0xF61420;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t S_FMT_D = 0xDF86DE, S_BALLOON_RED = 0xE03810, S_FB_PHOTO_IMG = 0xDF8E6E;
constexpr uint32_t S_FB_DEFAULT = 0xE0382F, S_FB_PANEL1 = 0xE03849, S_FB_PANEL2 = 0xE0386A;

void setTextStr(uint32_t text, uint32_t cstr) {
    Frame f;
    uint32_t s = newStr(f, cstr);
    call(fn::Text_setString, {text, s});
    strFree(s);
}
// a copy of the std::string at `src` into `dst` (long: a new buffer; short: the three words)
void copyStr(uint32_t dst, uint32_t src) {
    if (strIsLong(src)) call(fn::string_init, {dst, ld32(src + 8), ld32(src + 4)});
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}
uint32_t cstrOf(uint32_t s) { return strIsLong(s) ? ld32(s + 8) : s + 1; }
}  // namespace

// The status balloon of a party member's actor: name, the ranking value of the current mode, red (the player) /
// blue balloon, and the Facebook photo (downloaded, or requested by id - $_32 sets it later)
void SceneMyPage_updateFukidashi(uint32_t self, uint32_t actor, uint32_t lo, uint32_t hi, uint32_t show) {
    const uint32_t members = ld32(self + 0x2C0);
    if (!members) return;
    Frame f;
    uint32_t key = f.alloc(8);
    st32(key, lo);
    st32(key + 4, hi);
    uint32_t node = call(0x490275, {members + 0x24, key});
    if (!node) return;
    const int32_t i = int32_t(ld32(node + 0x10));
    if (i < 0) return;
    uint32_t member = f.alloc(0x128);
    st8(member + 0x11F, 0);
    call(SafeBuffer_read, {members, member, ld32(members + 0xC) + uint32_t(i) * 0x128, 0x128, uint32_t(i) * 0x128});
    uint32_t list = ld32(self + 0x2C0) + 0x3C;
    const uint32_t listBegin = ld32(list);
    uint32_t count = uint32_t(int32_t(ld32(list + 4) - listBegin) >> 3) * 0x286BCA1Bu;
    if (!(count > uint32_t(i))) {
        call(wl::VECTOR_AT_THROW, {list});
        return;
    }
    const uint32_t entry = listBegin + uint32_t(i) * 0x98;
    uint32_t status = findNode(actor, S_AVATAR_STATUS, 13);
    uint32_t name = findNode(status, S_TXT_NAME, 8);
    const bool isMe = lo == ld32(self + 0x3A0) && hi == ld32(self + 0x3A4);
    if (isMe) {
        uint32_t detail = ld32(call(APIManager_getInstance, {}) + 0x74);
        uint32_t buf = f.alloc(0x24);
        call(SafeBuffer_read, {detail, buf, ld32(detail + 0xC) + 0x14, 0x21, 0x14});
        setTextStr(name, buf);
    } else {
        setTextStr(name, member + 0xC);
    }
    uint32_t wording = findNode(status, S_TXT_WORDING, 11);
    switch (ld8(self + 0x3A8)) {
    case 0: {
        Frame g;
        uint32_t td = textData(g, 101000039);
        uint32_t buf = stringBuffer(g, {textDataStr(td), ld32(entry + 0x78)});
        setTextStr(wording, buf);
        release(ld32(td + 4));
        break;
    }
    case 1: {
        Frame g;
        uint32_t buf = stringBuffer(g, {S_FMT_D, ld32(entry + 0x44) + ld32(entry + 0x48)});
        setTextStr(wording, buf);
        break;
    }
    case 2:
    case 3: {
        Frame g;
        uint32_t money = g.alloc(12);
        call(SceneUtil_getMoneyStr, {money, listBegin, ld32(entry + 0x58), ld32(entry + 0x5C)});
        call(fn::Text_setString, {wording, money});
        strFree(money);
        break;
    }
    default:
        break;
    }
    uint32_t image = findNode(status, S_AVATAR_BALOON, 13);
    if (show == 1) vcall(image, node::setVisible, {image, 1});
    {
        Frame g;
        uint32_t path = isMe ? newStr(g, S_BALLOON_RED, 30) : newStr(g, S_BALLOON_BLUE, 31);
        call(ImageView_loadTexture, {image, path, 0});
        strFree(path);
    }
    uint32_t panel = findNode(status, S_FB_PHOTO, 19);
    uint32_t visible = 0;
    if (call(SceneUtil_isLoggedInFacebook, {}) == 1) {
        uint32_t photo = findNode(panel, S_FB_PHOTO_IMG, 14);
        uint32_t fbId = f.alloc(12);
        copyStr(fbId, entry + 0x88);
        bool linked = false;
        if (call(String_compareC, {fbId, S_EMPTY})) {
            linked = true;
            uint32_t url = f.alloc(12);
            call(SceneManager_getFaceImageUrl, {url, call(SceneManager_getInstance, {}), fbId});
            if (call(String_compareC, {url, S_EMPTY})) {
                call(ImageDownloader_loadImage, {ld32(self + OFF_DOWNLOADER), photo, cstrOf(url), cstrOf(fbId)});
            } else {
                uint32_t actorName = f.alloc(12);
                call(ld32(ld32(actor) + 0x150), {actorName, actor});   // getName (by value)
                {
                    Frame g;
                    uint32_t path = newStr(g, S_FB_DEFAULT, 25);
                    call(ImageView_loadTexture, {photo, path, 0});
                    strFree(path);
                }
                uint32_t downloader = ld32(self + OFF_DOWNLOADER);
                // $_32 {id, this, the facebook id} (the string moved into the lambda)
                uint32_t cap = f.alloc(0x18);
                st32(cap, lo);
                st32(cap + 4, hi);
                st32(cap + 8, self);
                copyStr(cap + 0xC, fbId);
                uint32_t idText = cstrOf(fbId);
                uint32_t fo = f.alloc(24);
                st32(fo + 16, 0);
                uint32_t heap = call(fn::operator_new, {0x20});
                if (heap) {
                    st32(heap, VT_fukidashi_32);
                    for (uint32_t k = 0; k < 0x18; k += 4) st32(heap + 8 + k, ld32(cap + k));
                    st32(cap + 0xC, 0);
                    st32(cap + 0x10, 0);
                    st32(cap + 0x14, 0);
                }
                st32(fo + 16, heap);
                call(ImageDownloader_addFacebookID, {downloader, idText, fo});
                functionDestroy(fo);
                strFree(cap + 0xC);
                strFree(actorName);
            }
            strFree(url);
        }
        strFree(fbId);
        if (linked) {
            Frame g;
            uint32_t path = isMe ? newStr(g, S_FB_PANEL1, 32) : newStr(g, S_FB_PANEL2, 32);
            call(ImageView_loadTexture, {panel, path, 0});
            visible = 1;
            strFree(path);
        }
    }
    vcall(panel, node::setVisible, {panel, visible});
}

RW_FUNCTION(0x642C38, "SceneMyPage::updateFukidashi", SceneMyPage_updateFukidashi);
