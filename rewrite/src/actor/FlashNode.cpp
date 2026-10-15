#include "FlashNode.hpp"

#include "FunctionThunks.hpp"

namespace {
namespace g {
constexpr uint32_t vec4_ctor = 0x8E6913;             // cocos2d::Vec4::Vec4()
constexpr uint32_t vec4_dtor = 0x8E6A69;             // cocos2d::Vec4::~Vec4()
constexpr uint32_t color3b_ctor = 0x9062E7;          // cocos2d::Color3B::Color3B(GLubyte, GLubyte, GLubyte)
constexpr uint32_t node_stopAllActions = 0x893815;   // cocos2d::Node::stopAllActions()
constexpr uint32_t node_scheduleUpdate = 0x894BA7;   // cocos2d::Node::scheduleUpdate()
constexpr uint32_t node_unscheduleUpdate = 0x894CB3; // cocos2d::Node::unscheduleUpdate()
constexpr uint32_t lwf_setColorTransformMovie = 0x80D995;  // LWF::LWF::SetColorTransformMovie(const string&, const ColorTransform*)
constexpr uint32_t lwf_searchMovieInstance = 0x7FE1B9;     // LWF::LWF::SearchMovieInstance(const string&) const
constexpr uint32_t delayTime_create = 0x86852D;      // cocos2d::DelayTime::create(float)
constexpr uint32_t callFunc_create = 0x866B39;       // cocos2d::CallFunc::create(const std::function<void()>&)
constexpr uint32_t sequence_create = 0x867999;       // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t fnVoid_swap = 0x4EE86D;           // std::function<void()>::swap(function&)
constexpr uint32_t fnVoid_assign = 0x4EB9CD;         // std::function<void()>::operator=(const function&)
constexpr uint32_t fnCStr_swap = 0x4E2961;           // std::function<void(const char*)>::swap(function&)
constexpr uint32_t fnInt_swap = 0x5428E1;            // std::function<void(int)>::swap(function&)
constexpr uint32_t vectorInt_dtor = 0x478961;        // std::__vector_base<int>::~__vector_base()
constexpr uint32_t dequeActFlag_dtor = 0x5C37D9;     // std::__deque_base<FlashNode::ActFlag>::~__deque_base()
constexpr uint32_t dequeInt_dtor = 0x5C3805;         // std::__deque_base<int>::~__deque_base()
constexpr uint32_t commandParser_dtor = 0x5C3831;    // CommandParser::~CommandParser()
constexpr uint32_t dequeInt_popFront = 0x5F151D;     // std::deque<int>::pop_front()
constexpr uint32_t dequeActFlag_popFront = 0x5F14E9; // std::deque<FlashNode::ActFlag>::pop_front()
constexpr uint32_t dequeInt_pushBack = 0x5C4BBD;     // std::deque<int>::push_back(const int&)
constexpr uint32_t dequeActFlag_pushBack = 0x5C40C9; // std::deque<FlashNode::ActFlag>::push_back(ActFlag&&)
constexpr uint32_t vectorInt_pushBackSlow = 0x4D6CC1;  // std::vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t convertSENameToResourceID = 0x5C2299;  // audio::convertSENameToResourceID(const char*)
constexpr uint32_t atoi_ = 0x410A14;                 // atoi
constexpr uint32_t replaceGameMainBGM = 0x5C0FA1;    // AudioReplaceGameMainBGM(audio::ResourceID, int)
constexpr uint32_t str_char = 0xDFCDC1;              // "char": the character movie of an LWF
constexpr uint32_t str_effectJson = 0xE00D35;        // "cocostudio/publish/ActMapBuffDeBuff_EffectAnim.ExportJson" (57)
constexpr uint32_t str_effectAnim = 0xE00D6F;        // "ActMapBuffDeBuff_EffectAnim" (27)
constexpr uint32_t statusDelay = 0xE49430;           // float[4]: how long each status effect plays before looping
// lambdas (std::function __func vtables)
constexpr uint32_t lambda1 = 0xF5CC6C;               // $_1 sound command
constexpr uint32_t lambda2 = 0xF5CC40;               // $_2 SE command
constexpr uint32_t lambda3 = 0xF5CC14;               // $_3 BGM command
constexpr uint32_t lambda4 = 0xF5CBE8;               // $_4 knockback command
constexpr uint32_t lambda5 = 0xF5CBBC;               // $_5 ActFlag 1
constexpr uint32_t lambda6 = 0xF5CB90;               // $_6 ActFlag 4
constexpr uint32_t lambda7 = 0xF5CB64;               // $_7 ActFlag 6
constexpr uint32_t lambda8 = 0xF5CB38;               // $_8 Node on-enter: scheduleUpdate
constexpr uint32_t lambda9 = 0xF5CB0C;               // $_9 Node on-exit: unscheduleUpdate
constexpr uint32_t reflectionLambda = 0xF5CC98;      // reflection_status $_0: set_loops
}  // namespace g

constexpr uint32_t NODE_UPDATE = 113;                // cocos2d::Node::update(float)

using actor::fb;
uint32_t ga(const void *p) { return rw::guest_addr(p); }
uint32_t at(const FlashNode *self, uint32_t off) { return ga(self) + off; }

// member = lambda{this} through a temporary std::function swapped in (the inlined assignment from a functor)
void assignBySwap(FlashNode *self, uint32_t swapFn, uint32_t vtable, uint32_t member, uint32_t tmp) {
    fnthunk::makeLocal(tmp, vtable, {ga(self)});
    rw::call(swapFn, {tmp, member});
    fnthunk::destroy(tmp);
}
// member = std::function(lambda{this}) (operator=(const function&) of a temporary)
void assignByCopy(FlashNode *self, uint32_t vtable, uint32_t member, uint32_t tmp) {
    fnthunk::makeLocal(tmp, vtable, {ga(self)});
    rw::call(g::fnVoid_assign, {member, tmp});
    fnthunk::destroy(tmp);
}
// LWFNode::setColor(Color3B(r, g, b)) then setOpacity(a) (the tint of the character)
void tint(FlashNode *self, uint32_t r, uint32_t gr, uint32_t b, uint32_t a) {
    uint32_t node = self->lwfNode;
    uint32_t setColor = actor::ld(actor::ld(node) + FlashNode::NODE_SET_COLOR * 4);
    bt::GuestFrame f(8);
    rw::call(g::color3b_ctor, {f.addr(0), r, gr, b});
    rw::call(setColor, {node, f.addr(0)});
    actor::vcallAt(self->lwfNode, FlashNode::NODE_SET_OPACITY, {self->lwfNode, a});
}
// SetColorTransformMovie("char", {mult, add}) on the node's LWF
void colorTransform(FlashNode *self, const float (&ct)[8]) {
    uint32_t lwf = actor::ld(self->lwfNode + FlashNode::LWF_NODE_LWF);
    bt::GuestFrame f(48);
    for (int k = 0; k < 8; k++) rw::save<float>(f.addr(16 + 4 * k), ct[k]);
    actor::makeString(f.addr(0), g::str_char, 4);
    rw::call(g::lwf_setColorTransformMovie, {lwf, f.addr(0), f.addr(16)});
    actor::freeString(f.addr(0));
}
void pushActFlag(uint32_t node, uint32_t flag) {
    bt::GuestFrame f(8);
    rw::save<uint32_t>(f.addr(0), flag);
    rw::call(g::dequeActFlag_pushBack, {node + 1248, f.addr(0)});
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
FlashNode *FlashNode::construct(FlashNode *self, uint32_t noCommands) {
    rw::call(actor_fn::node_ctor, {ga(self)});
    self->vtable = VTABLE;
    self->field_270 = 0;
    self->statusTinted = 0;
    self->field_272 = 0;
    self->field_274 = 0;
    rw::call(g::vec4_ctor, {ga(&self->vec4_278)});
    self->shownStatus = NONE;
    self->status = NONE;
    self->freezeOnStatus = 0;
    rw::set<uint32_t>(self, 660, 0);
    rw::set<uint32_t>(self, 664, 0);
    self->lwfNode = 0;
    for (uint32_t k = 0; k < 23; k++) rw::set<uint32_t>(self, 672 + 24 * k + 16, 0);   // CommandParser functions
    rw::call(actor_fn::aeabi_memset, {at(self, 1224), 0x39, 0});    // deques, pending sounds
    rw::call(actor_fn::aeabi_memset, {at(self, 1284), 0x14, 0});    // hit effect ids, armature, status bits
    if (noCommands != 0) return self;

    bt::GuestFrame f(0x28);
    uint32_t tmp = f.addr(0);
    assignBySwap(self, g::fnVoid_swap, g::lambda1, at(self, CMD_SOUND), tmp);
    assignBySwap(self, g::fnCStr_swap, g::lambda2, at(self, CMD_SE), tmp);
    assignBySwap(self, g::fnCStr_swap, g::lambda3, at(self, CMD_BGM), tmp);
    assignBySwap(self, g::fnInt_swap, g::lambda4, at(self, CMD_KNOCKBACK), tmp);
    assignBySwap(self, g::fnVoid_swap, g::lambda5, at(self, CMD_HIT), tmp);
    assignBySwap(self, g::fnVoid_swap, g::lambda6, at(self, CMD_FLAG4), tmp);
    assignBySwap(self, g::fnVoid_swap, g::lambda7, at(self, CMD_FLAG6), tmp);

    uint32_t adm = rw::call(actor_fn::adm_getInstance, {});
    actor::makeString(f.addr(0), g::str_effectJson, 0x39);
    rw::call(actor_fn::adm_addArmatureFileInfo, {adm, f.addr(0)});
    actor::freeString(f.addr(0));
    actor::makeString(f.addr(0), g::str_effectAnim, 0x1B);
    self->effectArmature = rw::call(actor_fn::armature_create, {f.addr(0)});
    actor::freeString(f.addr(0));
    actor::vcallAt(self->effectArmature, node_slot::SET_VISIBLE, {self->effectArmature, 0});
    actor::vcallAt(self->effectArmature, node_slot::SET_POSITION_XY, {self->effectArmature, 0, 0});
    rw::vcall(self, node_slot::ADD_CHILD_Z, {ga(self), self->effectArmature, 250});
    self->status = NONE;
    self->shownStatus = NONE;
    assignByCopy(self, g::lambda8, ga(&self->nodeOnEnter), tmp);
    assignByCopy(self, g::lambda9, ga(&self->nodeOnExit), tmp);
    return self;
}
RW_FUNCTION(0x5C32F4, "FlashNode::FlashNode(bool)", FlashNode::construct);

FlashNode *FlashNode::destruct(FlashNode *self) {
    self->vtable = VTABLE;
    rw::call(g::vectorInt_dtor, {ga(&self->hitEffectIds)});
    rw::call(g::dequeActFlag_dtor, {ga(&self->actFlags)});
    rw::call(g::dequeInt_dtor, {ga(&self->knockbacks)});
    rw::call(g::commandParser_dtor, {at(self, 672)});
    rw::call(g::vec4_dtor, {ga(&self->vec4_278)});
    rw::call(actor_fn::node_dtor, {ga(self)});
    return self;
}
RW_FUNCTION(0x5C3DEC, "FlashNode::~FlashNode", FlashNode::destruct);

void FlashNode::destructDelete(FlashNode *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5C3E4C, "FlashNode::~FlashNode (deleting)", FlashNode::destructDelete);

// ------------------------------------------------------------------------------------------- colour
void FlashNode::set_node_color_multi(FlashNode *self, float r, float g, float b, float a) {
    uint32_t node = self->lwfNode;
    uint32_t setColor = actor::ld(actor::ld(node) + NODE_SET_COLOR * 4);
    uint32_t cr = bt::f2uiz(r * 255.0f), cg = bt::f2uiz(g * 255.0f), cb = bt::f2uiz(b * 255.0f);
    bt::GuestFrame f(8);
    rw::call(g::color3b_ctor, {f.addr(0), cr, cg, cb});
    rw::call(setColor, {node, f.addr(0)});
    uint32_t node2 = self->lwfNode;
    uint32_t setOpacity = actor::ld(actor::ld(node2) + NODE_SET_OPACITY * 4);
    rw::call(setOpacity, {node2, bt::f2uiz(a * 255.0f)});
}
RW_FUNCTION(0x5C2B18, "FlashNode::set_node_color_multi", FlashNode::set_node_color_multi);

void FlashNode::set_node_color_add(FlashNode *self, float r, float g, float b, float a) {
    const float ct[8] = {1.0f, 1.0f, 1.0f, 1.0f, r, g, b, a};
    colorTransform(self, ct);
}
RW_FUNCTION(0x5C2BF4, "FlashNode::set_node_color_add", FlashNode::set_node_color_add);

void FlashNode::set_node_color_multi_add(FlashNode *self, float mr, float mg, float mb, float ma, float ar, float ag,
                                         float ab, float aa) {
    const float ct[8] = {mr, mg, mb, ma, ar, ag, ab, aa};
    colorTransform(self, ct);
}
RW_FUNCTION(0x5C2CA8, "FlashNode::set_node_color_multi_add", FlashNode::set_node_color_multi_add);

// ------------------------------------------------------------------------------------------- bad status
// Show a bad status (0..3; deep poison shows as poison), or clear the display (> 3).
void FlashNode::set_bad_status(FlashNode *self, uint32_t s) {
    if (s > 3) {
        self->statusShown = 1;
        self->statusTinted = 0;
        rw::call(g::node_stopAllActions, {self->effectArmature});
        actor::vcallAt(self->effectArmature, node_slot::SET_VISIBLE, {self->effectArmature, 0});
        uint32_t node = self->lwfNode;
        if (node) {
            tint(self, 0xFF, 0xFF, 0xFF, 255);
            set_node_color_add(self, 0.0f, 0.0f, 0.0f, 0.0f);
            node = self->lwfNode;
        }
        self->status = NONE;
        self->shownStatus = NONE;
        self->statusBits = 0;
        rw::call(g::node_scheduleUpdate, {node});
    } else if (uint32_t(int32_t(self->status)) != s) {
        self->statusTinted = 1;
        self->status = int32_t(s == 3 ? 0 : s);
        self->statusShown = 0;
    }
}
RW_FUNCTION(0x5C2D60, "FlashNode::set_bad_status", FlashNode::set_bad_status);

// Pick the status to show from the active bits (deep poison / poison first, then sleep, then paralysis) and loop
// its effect, or hide the effect when no bit is set.
void FlashNode::set_loops(FlashNode *self) {
    uint32_t bits = self->statusBits;
    int32_t shown;
    if (bits & (BIT_POISON | BIT_DEEP_POISON)) {
        shown = POISON;
        self->status = shown;
    } else if (bits & BIT_SLEEP) {
        shown = SLEEP;
        self->status = shown;
    } else if (bits & BIT_PARALYSIS) {
        shown = PARALYSIS;
        self->status = shown;
    } else {
        self->status = NONE;
        self->shownStatus = NONE;
        self->statusBits = 0;
        shown = -1;
    }
    if (shown >= 0) {
        self->shownStatus = shown;
        if (bits != 0) {
            uint32_t anim = actor::vcallAt(self->effectArmature, cs_slot::ARMATURE_GET_ANIMATION, {self->effectArmature});
            actor::vcallAt(anim, cs_slot::ANIM_PLAY_WITH_INDEX, {anim, uint32_t(shown * 2) | 1, 0xFFFFFFFFu, 0xFFFFFFFFu});
            uint32_t y = shown == PARALYSIS ? rw::get<uint32_t>(self, 660) : rw::get<uint32_t>(self, 664);
            actor::vcallAt(self->effectArmature, node_slot::SET_POSITION_Y, {self->effectArmature, y});
            change_sub(self);
            return;
        }
    }
    self->statusTinted = 0;
    rw::call(g::node_stopAllActions, {self->effectArmature});
    actor::vcallAt(self->effectArmature, node_slot::SET_VISIBLE, {self->effectArmature, 0});
    change_sub(self);
}
RW_FUNCTION(0x5C2E84, "FlashNode::set_loops", FlashNode::set_loops);

// Apply the tint of the shown status to the LWF node (purple poison, grey sleep, yellow paralysis); sleep and
// paralysis also freeze the animation when freezeOnStatus is set.
void FlashNode::change_sub(FlashNode *self) {
    uint32_t node = self->lwfNode;
    if (!node) return;
    if (self->statusTinted == 0) {
        rw::call(g::node_scheduleUpdate, {node});
        return;
    }
    switch (uint32_t(int32_t(self->status))) {
    case 0:
    case 3:
        tint(self, 0xA4, 0x42, 0xFF, 255);
        set_node_color_add(self, 0.0f, 0.0f, 0.0f, 0.0f);
        break;
    case 1:
        tint(self, 0xFF, 0xFF, 0xFF, 255);
        set_node_color_add(self, rw::bitsf(0x3E9C0000), rw::bitsf(0x3E9C0000), rw::bitsf(0x3E9C0000), 1.0f);
        break;
    case 2:
        tint(self, 0xFF, 0xFF, 0xFF, 255);
        set_node_color_add(self, rw::bitsf(0x3ED00000), rw::bitsf(0x3EFF0000), rw::bitsf(0x3D180000), 1.0f);
        break;
    default:
        tint(self, 0xFF, 0xFF, 0xFF, 255);
        set_node_color_add(self, 0.0f, 0.0f, 0.0f, 0.0f);
        rw::call(g::node_scheduleUpdate, {self->lwfNode});
        break;
    }
    if (self->freezeOnStatus != 0 && (rw::get<uint8_t>(self, 1300) & 6) != 0) {
        actor::vcallAt(self->lwfNode, NODE_UPDATE, {self->lwfNode, 0});
        rw::call(g::node_unscheduleUpdate, {self->lwfNode});
        return;
    }
    rw::call(g::node_scheduleUpdate, {self->lwfNode});
}
RW_FUNCTION(0x5C2F48, "FlashNode::change_sub", FlashNode::change_sub);

// Bring the effect armature in line with the status to show: play its intro once, then (via set_loops) loop.
void FlashNode::reflection_status(FlashNode *self, uint32_t force) {
    int32_t s = self->status;
    if (self->shownStatus == s) {
        if (force == 1) set_loops(self);
        self->statusShown = 1;
        return;
    }
    if (self->statusShown != 0 && force == 0) {
        self->statusShown = 1;
        return;
    }
    if (s > 3) return;
    uint32_t arm = self->effectArmature;
    rw::call(g::node_stopAllActions, {arm});
    actor::vcallAt(self->effectArmature, node_slot::SET_VISIBLE, {self->effectArmature, 1});
    uint32_t anim = actor::vcallAt(self->effectArmature, cs_slot::ARMATURE_GET_ANIMATION, {self->effectArmature});
    actor::vcallAt(anim, cs_slot::ANIM_PLAY_WITH_INDEX,
                   {anim, uint32_t(int32_t(self->status)) << 1, 0xFFFFFFFFu, 0xFFFFFFFFu});
    uint32_t y = self->status == PARALYSIS ? rw::get<uint32_t>(self, 660) : rw::get<uint32_t>(self, 664);
    actor::vcallAt(self->effectArmature, node_slot::SET_POSITION_Y, {self->effectArmature, y});
    uint32_t delay = rw::call(g::delayTime_create, {actor::ld(g::statusDelay + uint32_t(int32_t(self->status)) * 4)});
    self->shownStatus = self->status;
    uint32_t target = self->effectArmature;
    bt::GuestFrame f(24);
    fnthunk::makeLocal(f.addr(0), g::reflectionLambda, {ga(self)});
    uint32_t done = rw::call(g::callFunc_create, {f.addr(0)});
    uint32_t seq = rw::call(g::sequence_create, {delay, done, 0});
    rw::call(actor_fn::node_runAction, {target, seq});
    fnthunk::destroy(f.addr(0));
    change_sub(self);
    self->statusShown = 1;
}
RW_FUNCTION(0x5C3170, "FlashNode::reflection_status", FlashNode::reflection_status);

namespace {
// The UpdateStatus* pattern: set the bit (and show this status unless it is already the one shown), or clear it
// and fall back to the next active status (or clear the display).
uint32_t updateStatus(FlashNode *self, uint32_t on, uint32_t bit, int32_t showWhenSet, int32_t skipIfStatus,
                      uint32_t sleepTest, int32_t sleepShow, uint32_t otherMask, int32_t otherShow) {
    uint32_t bits = self->statusBits;
    if (on == 1) {
        self->statusBits = bits | bit;
        if (self->status != skipIfStatus) {
            self->statusTinted = 1;
            self->status = showWhenSet;
            self->statusShown = 0;
        }
        return 1;
    }
    if ((bits & bit) == 0) return 0;
    uint32_t rest = bits & ~bit;
    self->statusBits = rest;
    if (rest == 0) {
        FlashNode::set_bad_status(self, 4);
        return 0;
    }
    if (bits & sleepTest) {
        self->status = sleepShow;
        self->shownStatus = sleepShow;
    } else if (bits & otherMask) {
        self->status = otherShow;
        self->shownStatus = otherShow;
    }
    FlashNode::reflection_status(self, 1);
    return 0;
}
}  // namespace

uint32_t FlashNode::UpdateStatusDeepPoison(FlashNode *self, uint32_t on) {
    return updateStatus(self, on, BIT_DEEP_POISON, POISON, DEEP_POISON, BIT_SLEEP, SLEEP, ~0xCu, PARALYSIS);
}
RW_FUNCTION(0x5C3BCC, "FlashNode::UpdateStatusDeepPoison", FlashNode::UpdateStatusDeepPoison);

uint32_t FlashNode::UpdateStatusPoison(FlashNode *self, uint32_t on) {
    return updateStatus(self, on, BIT_POISON, POISON, POISON, BIT_SLEEP, SLEEP, ~5u, PARALYSIS);
}
RW_FUNCTION(0x5C3C54, "FlashNode::UpdateStatusPoison", FlashNode::UpdateStatusPoison);

uint32_t FlashNode::UpdateStatusSleep(FlashNode *self, uint32_t on) {
    return updateStatus(self, on, BIT_SLEEP, SLEEP, SLEEP, BIT_POISON, POISON, ~6u, PARALYSIS);
}
RW_FUNCTION(0x5C3CD8, "FlashNode::UpdateStatusSleep", FlashNode::UpdateStatusSleep);

uint32_t FlashNode::UpdateStatusParalysis(FlashNode *self, uint32_t on) {
    return updateStatus(self, on, BIT_PARALYSIS, PARALYSIS, PARALYSIS, BIT_POISON, POISON, ~6u, SLEEP);
}
RW_FUNCTION(0x5C3D60, "FlashNode::UpdateStatusParalysis", FlashNode::UpdateStatusParalysis);

// ------------------------------------------------------------------------------------------- update / queries
void FlashNode::update(FlashNode *self, float dt) { rw::vcall(self, VSLOT_UPDATE_SOUND, {ga(self), fb(dt)}); }
RW_FUNCTION(0x5C3AF6, "FlashNode::update", FlashNode::update);

// Play the sound effect / switch the BGM the movie asked for since the last frame.
void FlashNode::updateSound(FlashNode *self, float) {
    if (self->soundPending == 0) return;
    self->soundPending = 0;
    uint32_t se = self->pendingSE;
    if (se) {
        bt::GuestFrame f(8);
        uint32_t audio = rw::call(actor_fn::audio_getInstance, {});
        rw::call(actor_fn::audio_getLayer, {f.addr(0), audio, 0});
        rw::call(actor_fn::layer_playSE, {f.word(0), se, 0, 0});
        bt::releaseShared(f.word(4));
        self->pendingSE = 0;
    }
    uint32_t bgm = self->pendingBGM;
    if (bgm) {
        rw::call(g::replaceGameMainBGM, {bgm, 500});
        self->pendingBGM = 0;
    }
}
RW_FUNCTION(0x5C3B04, "FlashNode::updateSound", FlashNode::updateSound);

int32_t FlashNode::getHitEffectID(const FlashNode *) { return 2; }
RW_FUNCTION(0x5C3BC8, "FlashNode::getHitEffectID", FlashNode::getHitEffectID);

// The "char" movie is playing.
uint32_t FlashNode::isPlaying(const FlashNode *self) {
    uint32_t lwf = actor::ld(self->lwfNode + LWF_NODE_LWF);
    bt::GuestFrame f(16);
    actor::makeString(f.addr(0), g::str_char, 4);
    uint32_t movie = rw::call(g::lwf_searchMovieInstance, {lwf, f.addr(0)});
    uint8_t playing = actor::ld8(movie + 173);
    actor::freeString(f.addr(0));
    return playing ? 1 : 0;
}
RW_FUNCTION(0x5EE498, "FlashNode::isPlaying", FlashNode::isPlaying);

// Pop the oldest queued knockback (0 when none).
int32_t FlashNode::knockbackShift(FlashNode *self) {
    if (self->knockbacks.size == 0) return 0;
    int32_t v = int32_t(self->knockbacks.front());
    rw::call(g::dequeInt_popFront, {ga(&self->knockbacks)});
    return v;
}
RW_FUNCTION(0x5EFB18, "FlashNode::knockbackShift", FlashNode::knockbackShift);

// Pop the oldest queued action flag (0 when none).
int32_t FlashNode::flagShift(FlashNode *self) {
    if (self->actFlags.size == 0) return 0;
    int32_t v = int32_t(self->actFlags.front());
    rw::call(g::dequeActFlag_popFront, {ga(&self->actFlags)});
    return v;
}
RW_FUNCTION(0x5EFD38, "FlashNode::flagShift", FlashNode::flagShift);

void FlashNode::pushBackHitEffectID(FlashNode *self, int32_t id) {
    uint32_t end = self->hitEffectIds.end;
    if (end == self->hitEffectIds.cap) {
        bt::GuestFrame f(8);
        rw::save<int32_t>(f.addr(0), id);
        rw::call(g::vectorInt_pushBackSlow, {ga(&self->hitEffectIds), f.addr(0)});
        return;
    }
    uint32_t base = 0;
    if (end) {
        rw::store32(rw::host_ptr<void>(end), uint32_t(id));
        base = self->hitEffectIds.end;
    }
    self->hitEffectIds.end = base + 4;
}
RW_FUNCTION(0x74A26C, "FlashNode::pushBackHitEffectID", FlashNode::pushBackHitEffectID);

// ------------------------------------------------------------------------------------------- lambdas
void FlashNode::cmdSound(uint32_t lambda) { rw::store8(rw::host_ptr<uint8_t>(actor::ld(lambda + 4) + 1280), 1); }

namespace {
void soundCommand(uint32_t lambda, const uint32_t *name, uint32_t field) {
    uint32_t self = actor::ld(lambda + 4);
    uint32_t str = *name;
    uint32_t id = rw::call(g::convertSENameToResourceID, {str});
    rw::save<uint32_t>(self + field, id);
    if (id == 0) rw::save<uint32_t>(self + field, rw::call(g::atoi_, {str}));
}
}  // namespace

void FlashNode::cmdSE(uint32_t lambda, const uint32_t *name) { soundCommand(lambda, name, 1272); }
void FlashNode::cmdBGM(uint32_t lambda, const uint32_t *name) { soundCommand(lambda, name, 1276); }

// Queue a knockback value and its action flag (3 for value 1, else 2).
void FlashNode::cmdKnockback(uint32_t lambda, const int32_t *value) {
    int32_t v = *value;
    uint32_t self = actor::ld(lambda + 4);
    bt::GuestFrame f(8);
    rw::save<int32_t>(f.addr(0), v);
    rw::call(g::dequeInt_pushBack, {self + 1224, f.addr(0)});
    pushActFlag(self, v == 1 ? 3 : 2);
}
void FlashNode::cmdHit(uint32_t lambda) { pushActFlag(actor::ld(lambda + 4), 1); }
void FlashNode::cmdFlag4(uint32_t lambda) { pushActFlag(actor::ld(lambda + 4), 4); }
void FlashNode::cmdFlag6(uint32_t lambda) { pushActFlag(actor::ld(lambda + 4), 6); }
void FlashNode::onEnterLambda(uint32_t lambda) { rw::call(g::node_scheduleUpdate, {actor::ld(lambda + 4)}); }
void FlashNode::onExitLambda(uint32_t lambda) { rw::call(g::node_unscheduleUpdate, {actor::ld(lambda + 4)}); }
void FlashNode::reflectionDoneLambda(uint32_t lambda) { set_loops(rw::host_ptr<FlashNode>(actor::ld(lambda + 4))); }

RW_FUNCTION(0x5C55CA, "FlashNode::FlashNode $_1 (sound command)", FlashNode::cmdSound);
RW_FUNCTION(0x5C5512, "FlashNode::FlashNode $_2 (SE command)", FlashNode::cmdSE);
RW_FUNCTION(0x5C5454, "FlashNode::FlashNode $_3 (BGM command)", FlashNode::cmdBGM);
RW_FUNCTION(0x5C4B20, "FlashNode::FlashNode $_4 (knockback command)", FlashNode::cmdKnockback);
RW_FUNCTION(0x5C4A40, "FlashNode::FlashNode $_5 (ActFlag 1)", FlashNode::cmdHit);
RW_FUNCTION(0x5C4960, "FlashNode::FlashNode $_6 (ActFlag 4)", FlashNode::cmdFlag4);
RW_FUNCTION(0x5C4048, "FlashNode::FlashNode $_7 (ActFlag 6)", FlashNode::cmdFlag6);
RW_FUNCTION(0x5C3FAA, "FlashNode::FlashNode $_8 (on enter: scheduleUpdate)", FlashNode::onEnterLambda);
RW_FUNCTION(0x5C3F0E, "FlashNode::FlashNode $_9 (on exit: unscheduleUpdate)", FlashNode::onExitLambda);
RW_FUNCTION(0x5C56C2, "FlashNode::reflection_status $_0 (effect done: set_loops)", FlashNode::reflectionDoneLambda);

// std::function thunks of the lambdas
RW_FUNCTION(0x5C556C, "FlashNode::FlashNode $_1 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C5574, "FlashNode::FlashNode $_1 __func::__clone()", (fnthunk::cloneWords<g::lambda1, 8, 1>));
RW_FUNCTION(0x5C55A0, "FlashNode::FlashNode $_1 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda1>);
RW_FUNCTION(0x5C55C0, "FlashNode::FlashNode $_1 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C55C2, "FlashNode::FlashNode $_1 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C55D8, "FlashNode::FlashNode $_1 __func::target", fnthunk::target<0xe49820>);
RW_FUNCTION(0x5C55F8, "FlashNode::FlashNode $_1 __func::target_type", fnthunk::targetType<0xf8f118>);
RW_FUNCTION(0x5C54B2, "FlashNode::FlashNode $_2 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C54BC, "FlashNode::FlashNode $_2 __func::__clone()", (fnthunk::cloneWords<g::lambda2, 8, 1>));
RW_FUNCTION(0x5C54E8, "FlashNode::FlashNode $_2 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda2>);
RW_FUNCTION(0x5C5508, "FlashNode::FlashNode $_2 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C550A, "FlashNode::FlashNode $_2 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C5538, "FlashNode::FlashNode $_2 __func::target", fnthunk::target<0xe497b0>);
RW_FUNCTION(0x5C5558, "FlashNode::FlashNode $_2 __func::target_type", fnthunk::targetType<0xf8f104>);
RW_FUNCTION(0x5C53F4, "FlashNode::FlashNode $_3 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C53FC, "FlashNode::FlashNode $_3 __func::__clone()", (fnthunk::cloneWords<g::lambda3, 8, 1>));
RW_FUNCTION(0x5C5428, "FlashNode::FlashNode $_3 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda3>);
RW_FUNCTION(0x5C5448, "FlashNode::FlashNode $_3 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C544A, "FlashNode::FlashNode $_3 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C547C, "FlashNode::FlashNode $_3 __func::target", fnthunk::target<0xe49740>);
RW_FUNCTION(0x5C549C, "FlashNode::FlashNode $_3 __func::target_type", fnthunk::targetType<0xf8f0f0>);
RW_FUNCTION(0x5C4AC0, "FlashNode::FlashNode $_4 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C4AC8, "FlashNode::FlashNode $_4 __func::__clone()", (fnthunk::cloneWords<g::lambda4, 8, 1>));
RW_FUNCTION(0x5C4AF4, "FlashNode::FlashNode $_4 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda4>);
RW_FUNCTION(0x5C4B14, "FlashNode::FlashNode $_4 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C4B16, "FlashNode::FlashNode $_4 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C4B88, "FlashNode::FlashNode $_4 __func::target", fnthunk::target<0xe496d0>);
RW_FUNCTION(0x5C4BA8, "FlashNode::FlashNode $_4 __func::target_type", fnthunk::targetType<0xf8f0dc>);
RW_FUNCTION(0x5C49E0, "FlashNode::FlashNode $_5 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C49E8, "FlashNode::FlashNode $_5 __func::__clone()", (fnthunk::cloneWords<g::lambda5, 8, 1>));
RW_FUNCTION(0x5C4A14, "FlashNode::FlashNode $_5 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda5>);
RW_FUNCTION(0x5C4A34, "FlashNode::FlashNode $_5 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C4A36, "FlashNode::FlashNode $_5 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C4A8C, "FlashNode::FlashNode $_5 __func::target", fnthunk::target<0xe49660>);
RW_FUNCTION(0x5C4AAC, "FlashNode::FlashNode $_5 __func::target_type", fnthunk::targetType<0xf8f0c8>);
RW_FUNCTION(0x5C4900, "FlashNode::FlashNode $_6 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C4908, "FlashNode::FlashNode $_6 __func::__clone()", (fnthunk::cloneWords<g::lambda6, 8, 1>));
RW_FUNCTION(0x5C4934, "FlashNode::FlashNode $_6 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda6>);
RW_FUNCTION(0x5C4954, "FlashNode::FlashNode $_6 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C4956, "FlashNode::FlashNode $_6 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C49AC, "FlashNode::FlashNode $_6 __func::target", fnthunk::target<0xe495f0>);
RW_FUNCTION(0x5C49CC, "FlashNode::FlashNode $_6 __func::target_type", fnthunk::targetType<0xf8f0b4>);
RW_FUNCTION(0x5C3FE8, "FlashNode::FlashNode $_7 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C3FF0, "FlashNode::FlashNode $_7 __func::__clone()", (fnthunk::cloneWords<g::lambda7, 8, 1>));
RW_FUNCTION(0x5C401C, "FlashNode::FlashNode $_7 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda7>);
RW_FUNCTION(0x5C403C, "FlashNode::FlashNode $_7 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C403E, "FlashNode::FlashNode $_7 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C4094, "FlashNode::FlashNode $_7 __func::target", fnthunk::target<0xe49580>);
RW_FUNCTION(0x5C40B4, "FlashNode::FlashNode $_7 __func::target_type", fnthunk::targetType<0xf8f0a0>);
RW_FUNCTION(0x5C3F4C, "FlashNode::FlashNode $_8 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C3F54, "FlashNode::FlashNode $_8 __func::__clone()", (fnthunk::cloneWords<g::lambda8, 8, 1>));
RW_FUNCTION(0x5C3F80, "FlashNode::FlashNode $_8 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda8>);
RW_FUNCTION(0x5C3FA0, "FlashNode::FlashNode $_8 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C3FA2, "FlashNode::FlashNode $_8 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C3FB4, "FlashNode::FlashNode $_8 __func::target", fnthunk::target<0xe49510>);
RW_FUNCTION(0x5C3FD4, "FlashNode::FlashNode $_8 __func::target_type", fnthunk::targetType<0xf8f08c>);
RW_FUNCTION(0x5C3EB0, "FlashNode::FlashNode $_9 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C3EB8, "FlashNode::FlashNode $_9 __func::__clone()", (fnthunk::cloneWords<g::lambda9, 8, 1>));
RW_FUNCTION(0x5C3EE4, "FlashNode::FlashNode $_9 __func::__clone(__base*)", fnthunk::cloneInto1<g::lambda9>);
RW_FUNCTION(0x5C3F04, "FlashNode::FlashNode $_9 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C3F06, "FlashNode::FlashNode $_9 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C3F18, "FlashNode::FlashNode $_9 __func::target", fnthunk::target<0xe494a0>);
RW_FUNCTION(0x5C3F38, "FlashNode::FlashNode $_9 __func::target_type", fnthunk::targetType<0xf8f078>);
RW_FUNCTION(0x5C5664, "FlashNode::reflection_status $_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5C566C, "FlashNode::reflection_status $_0 __func::__clone()", (fnthunk::cloneWords<g::reflectionLambda, 8, 1>));
RW_FUNCTION(0x5C5698, "FlashNode::reflection_status $_0 __func::__clone(__base*)", fnthunk::cloneInto1<g::reflectionLambda>);
RW_FUNCTION(0x5C56B8, "FlashNode::reflection_status $_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5C56BA, "FlashNode::reflection_status $_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5C56CC, "FlashNode::reflection_status $_0 __func::target", fnthunk::target<0xe498a0>);
RW_FUNCTION(0x5C56EC, "FlashNode::reflection_status $_0 __func::target_type", fnthunk::targetType<0xf8f12c>);
