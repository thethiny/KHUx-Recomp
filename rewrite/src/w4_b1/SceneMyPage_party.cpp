// SceneMyPage: the party members / the player's avatar on the field (initPartyMembers).
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t ActorVector_clear = 0x485BA3, ActorVector_pushBack = 0x6425D5;
constexpr uint32_t U64Vector_pushSlow = 0x4ECB6D;        // vector<uint64>::__push_back_slow_path(const&)
constexpr uint32_t UIntVector_pushSlow = 0x4ED6F1;       // vector<unsigned>::__push_back_slow_path(unsigned&&)
constexpr uint32_t Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t M_createPartyAvatar = 0x642395, M_setAvatrPosition = 0x6410D5, M_runAvatrAction = 0x6412E9;
constexpr uint32_t S_AVATAR_LLU = 0xE03794;              // avatar_%llu
constexpr uint32_t MEMBER_SIZE = 0x128;

// vector<uint64>::push_back(const&) as inlined (end == capacity: the slow path; a null end becomes 8)
void pushId(uint32_t vec, uint32_t valueAddr) {
    uint32_t end = ld32(vec + 4);
    if (end != ld32(vec + 8)) {
        uint32_t e = 0;
        if (end) {
            st32(end, ld32(valueAddr));
            st32(end + 4, ld32(valueAddr + 4));
            e = ld32(vec + 4);
        }
        st32(vec + 4, e + 8);
    } else {
        call(U64Vector_pushSlow, {vec, valueAddr});
    }
}

// vector<unsigned>::push_back as inlined (end >= capacity: the slow path)
void pushIndex(uint32_t vec, uint32_t valueAddr) {
    uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t e = 0;
        if (end) {
            st32(end, ld32(valueAddr));
            e = ld32(vec + 4);
        }
        st32(vec + 4, e + 4);
    } else {
        call(UIntVector_pushSlow, {vec, valueAddr});
    }
}

// avatar actor added to the field layer, named avatar_<id>
void addActor(uint32_t self, uint32_t actor, uint32_t lo, uint32_t hi) {
    uint32_t layer = ld32(self + 0x2A4);
    Frame f;
    uint32_t buf = stringBuffer(f, {S_AVATAR_LLU, lo, hi});
    uint32_t name = newStr(f, buf);
    vcall(layer, node::addChildZName, {layer, actor, 0, name});
    strFree(name);
}

// a random move point for the actor: pushed onto the route, the actor placed there; returns the point
uint32_t placeAtRandomPoint(uint32_t self, uint32_t actor, uint32_t slot) {
    uint32_t points = self + OFF_POINTS;
    uint32_t n = uint32_t(int32_t(ld32(points + 4) - ld32(points)) >> 3);
    uint32_t idx = call(Random_generateUniformInt, {call(Random_getInstance, {}), 0, n - 1, 0});
    st32(slot, idx);
    pushIndex(self + OFF_ROUTE, slot);
    uint32_t begin = ld32(points);
    if (!(uint32_t(int32_t(ld32(points + 4) - begin) >> 3) > idx)) {
        call(wl::VECTOR_AT_THROW, {points});
        return idx;
    }
    Frame f;
    uint32_t pos = f.alloc(8);
    call(fn::Vec2_ctor_copy, {pos, begin + idx * 8});
    call(M_setAvatrPosition, {self, actor, pos});
    call(wl::Vec2_dtor, {pos});
    return idx;
}
}  // namespace

// Party: up to `columns` members (always the player) as actors at random points, walking; solo: the player
void SceneMyPage_initPartyMembers(uint32_t self) {
    Frame f;
    {
        // the player's id
        uint32_t id[2];
        uint32_t sb = ld32(call(APIManager_getInstance, {}) + 0x114);
        id[0] = uint32_t(scene::safe32(sb, 0));
        id[1] = uint32_t(scene::safe32(sb, 4));
        st32(self + 0x3A4, id[1]);
        st32(self + 0x3A0, id[0]);
    }
    {
        uint32_t api = call(APIManager_getInstance, {});
        uint32_t ctl = ld32(api + 0x200), obj = ld32(api + 0x1FC);
        addRef(ctl);
        st32(self + 0x2C0, obj);
        uint32_t old = ld32(self + 0x2C4);
        st32(self + 0x2C4, ctl);
        release(old);
    }
    if (ld8(self + OFF_HAS_PARTY)) {
        uint32_t limit = ld32(self + OFF_COLUMNS);
        uint32_t members = ld32(ld32(self + 0x2C0) + 0x20);
        call(ActorVector_clear, {self + OFF_ACTORS_A});
        if (!(limit < members)) limit = members;
        uint32_t limit1 = limit ? limit - 1 : 0;
        if (!ld32(ld32(self + 0x2C0) + 0x20)) return;
        uint32_t member = f.alloc(MEMBER_SIZE);
        uint32_t slot = f.alloc(4);
        uint32_t shown = 0;
        uint32_t off = 0;
        for (uint32_t i = 0; i < ld32(ld32(self + 0x2C0) + 0x20); i++, off += MEMBER_SIZE) {
            uint32_t sb = ld32(self + 0x2C0);
            st8(member + 0x11F, ld8(member + 0x11F) & 0xFD);
            call(SafeBuffer_read, {sb, member, ld32(sb + 0xC) + off, MEMBER_SIZE, off});
            pushId(self + 0x390, member);
            uint32_t lo = ld32(member), hi = ld32(member + 4);
            bool isMe = lo == ld32(self + 0x3A0) && hi == ld32(self + 0x3A4);
            if (!(limit1 > shown) && !isMe) continue;
            uint32_t actor = call(M_createPartyAvatar, {self, 0, lo, hi});
            if (isMe) st32(self + 0x2BC, actor);
            else shown++;
            uint32_t before = uint32_t(int32_t(ld32(self + 0x2D8) - ld32(self + 0x2D4)) >> 2);
            addActor(self, actor, lo, hi);
            call(ActorVector_pushBack, {self + OFF_ACTORS_A, actor});
            pushId(self + OFF_IDS, member);
            uint32_t idx = placeAtRandomPoint(self, actor, slot);
            if (isMe) st32(self + 0x2EC, idx);
            else call(M_runAvatrAction, {self, actor, before});
        }
        return;
    }
    if (call(SceneUtil_isLoggedInFacebook, {}) == 1) {
        uint32_t api = call(APIManager_getInstance, {});
        uint32_t ctl = ld32(api + 0x330), obj = ld32(api + 0x32C);
        addRef(ctl);
        st32(self + 0x2C0, obj);
        uint32_t old = ld32(self + 0x2C4);
        st32(self + 0x2C4, ctl);
        release(old);
    }
    uint32_t lo = ld32(self + 0x3A0), hi = ld32(self + 0x3A4);
    uint32_t actor = call(M_createPartyAvatar, {self, 0, lo, hi});
    st32(self + 0x2BC, actor);
    addActor(self, actor, ld32(self + 0x3A0), ld32(self + 0x3A4));
    call(ActorVector_pushBack, {self + OFF_ACTORS_A, ld32(self + 0x2BC)});
    pushId(self + OFF_IDS, self + 0x3A0);
    uint32_t slot = f.alloc(4);
    uint32_t idx = placeAtRandomPoint(self, ld32(self + 0x2BC), slot);
    st32(self + 0x2EC, idx);
}

RW_FUNCTION(0x63DDF0, "SceneMyPage::initPartyMembers", SceneMyPage_initPartyMembers);

// ------------------------------------------------------------------------------------------------ update
namespace {
constexpr uint32_t ImageDownloader_update = 0x5BD4D9;
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t ActorVector_erase = 0x63F9FD;         // cocos2d::Vector<AvatarActor*>::erase(iterator)
constexpr uint32_t BaseSlideView_changeNextPage = 0x526245;
constexpr uint32_t M_changeWalkingMode = 0x63F465, M_clearMotionEvents = 0x63FA5B;
constexpr uint32_t M_addChangeMotionEvent = 0x63FA8D, M_updateScrollSortAvatars = 0x63FC49;
constexpr uint32_t M_updateScrollAvatarsMotion = 0x63FB09;
inline bool isnanf_(float v) { return v != v; }
}  // namespace

void SceneMyPage_update(uint32_t self, float dt) {
    call(ImageDownloader_update, {ld32(self + OFF_DOWNLOADER)});
    // every 10 s (not in ranking mode): the walking mode changes
    if (!ld8(self + 0x2F0)) {
        float t = ldf(self + 0x2F4) + dt;
        stf(self + 0x2F4, t);
        if (!(t <= 10.0f || isnanf_(t))) {
            st32(self + 0x2F4, 0);
            call(M_changeWalkingMode, {self});
        }
    }
    // one queued motion change per frame
    {
        uint32_t actors = ld32(self + OFF_ACTORS);
        if (int32_t(ld32(self + 0x350) - actors) >= 1) {
            uint32_t motions = ld32(self + OFF_MOTIONS);
            if (ld32(self + 0x35C) == motions) {
                call(wl::VECTOR_AT_THROW, {self + OFF_MOTIONS});
                return;
            }
            uint32_t actor = ld32(actors);
            if (actor) {
                uint32_t motion = ld32(motions);
                uint32_t avatar = ld32(actor + 0x288);
                if (ld32(avatar + 0x524) != motion) {
                    call(Avatar_changeMotion, {avatar, motion});
                    actors = ld32(self + OFF_ACTORS);
                }
            }
            call(ActorVector_erase, {self + OFF_ACTORS, actors});
            uint32_t end = ld32(self + 0x35C);
            uint32_t b = ld32(self + OFF_MOTIONS);
            uint32_t n = end - (b + 4);
            call(wl::MEMMOVE, {b, b + 4, n});
            uint32_t newEnd = b + (n & ~3u);
            uint32_t e = ld32(self + 0x35C);
            if (e != newEnd) st32(self + 0x35C, e + (~3u & ~((e - 4) - newEnd)));
        }
    }
    // z order by height on the field
    if (int32_t(ld32(self + 0x2D8) - ld32(self + OFF_ACTORS_A)) >= 1) {
        int32_t i = 0;
        do {
            uint32_t actor = ld32(ld32(self + OFF_ACTORS_A) + uint32_t(i) * 4);
            uint32_t panel = ld32(self + 0x298);
            float h = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
            float y = bf(vcall(actor, node::getPositionY, {actor}));
            vcall(actor, node::setLocalZOrder, {actor, uint32_t(scene::f2i(h - y))});
            i++;
        } while (i < int32_t(ld32(self + 0x2D8) - ld32(self + OFF_ACTORS_A)) >> 2);
    }
    // the banners slide on by themselves
    {
        float period = ldf(self + 0x3F8);
        if (!(period <= 0.0f || isnanf_(period))) {
            uint32_t slide = ld32(self + 0x400);
            if (!ld8(slide + 0x5C0) && !ld8(slide + 0x5C1)) {
                float acc = ldf(self + 0x3FC);
                if (acc >= period || isnanf_(acc) || isnanf_(period)) {
                    st32(self + 0x3FC, 0);
                    call(BaseSlideView_changeNextPage, {slide});
                } else {
                    stf(self + 0x3FC, acc + dt);
                }
            }
        }
    }
    // the field's horizontal shake back to rest
    uint32_t layer = ld32(self + 0x2A4);
    if (layer && !ld8(self + 0x3CC) && !ld8(self + 0x2F0)) {
        float v = ldf(self + 0x3C4);
        if (!(v == 0.0f)) {
            bool keep;
            if (!(v <= 0.0f || isnanf_(v))) {
                if (!ld8(self + 0x3AB)) {
                    v = v + ldf(self + 0x3C8) * bf(0xBE4CCCCDu);
                    stf(self + 0x3C4, v);
                }
                keep = v >= 0.0f || isnanf_(v);
            } else {
                if (!ld8(self + 0x3AB)) {
                    v = v + ldf(self + 0x3C8) * bf(0x3E4CCCCDu);
                    stf(self + 0x3C4, v);
                }
                keep = v <= 0.0f || isnanf_(v);
            }
            if (!keep) {
                st32(self + 0x3C4, 0);
                v = 0.0f;
            }
        }
        if (ld8(self + 0x3BC))
            vcall(layer, node::setPositionX, {layer, fb(ldf(self + 0x3C0) + v)});
    }
    // the scroll of the avatar line (ranking mode)
    float s = ldf(self + 0x3AC);
    if (s == 0.0f) return;
    const uint32_t dirAt = self + 0x3AB;
    if (ld8(dirAt)) {
        float slow = ldf(self + 0x3B8);
        bool positive = !(s <= 0.0f || isnanf_(s));
        bool reset;
        if (positive) {
            s = s - slow;
            stf(self + 0x3AC, s);
            reset = !(s >= 0.0f || isnanf_(s));
        } else {
            s = s + slow;
            stf(self + 0x3AC, s);
            reset = !(s <= 0.0f || isnanf_(s));
        }
        if (reset) st32(self + 0x3AC, 0);
        else if (!(s == 0.0f)) return;
        if (ld32(self + 0x3D0) == 0x1C) return;
        call(M_clearMotionEvents, {self});
        if (int32_t(ld32(self + 0x2E4) - ld32(self + OFF_ACTORS_B)) >= 1) {
            int32_t i = 0;
            do {
                call(M_addChangeMotionEvent, {self, ld32(ld32(self + OFF_ACTORS_B) + uint32_t(i) * 4), 0x1C});
                i++;
            } while (i < int32_t(ld32(self + 0x2E4) - ld32(self + OFF_ACTORS_B)) >> 2);
        }
        st32(self + 0x3D0, 0x1C);
        return;
    }
    float fast = ldf(self + 0x3B0);
    float step;
    if (!(s <= 0.0f || isnanf_(s))) {
        step = (fast + fast < s) ? fast : s * 0.5f;
    } else {
        float lim = fast * -2.0f;
        step = (s >= lim || isnanf_(s) || isnanf_(lim)) ? s * 0.5f : -fast;
    }
    float s2 = s + step / -3.5f;
    bool small = std::fabs(s2) < 2.0f;
    st32(self + 0x3AC, small ? 0u : fb(s2));
    float moved = bf(call(M_updateScrollSortAvatars, {self, small ? 0u : fb(step)}));
    call(M_updateScrollAvatarsMotion, {self, fb(moved), ld8(dirAt)});
}

RW_FUNCTION(0x63EF40, "SceneMyPage::update", SceneMyPage_update);
