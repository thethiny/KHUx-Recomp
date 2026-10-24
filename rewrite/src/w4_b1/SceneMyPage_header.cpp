// SceneMyPage::changeRankingHeader: the right-hand header panels slide in (ranking mode) or out (back to the
// field), the Chirishi NPC walks in or out, and `done` runs when the last panel has moved.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t FunctionVV_copy = 0x4683CD;           // std::function<void()>::function(const function&)
constexpr uint32_t FunctionVV_move = 0x47F905;           // std::function<void()>::function(function&&)
constexpr uint32_t SceneUtil_createRightInAnimation = 0x45B8BD;    // (node, float, float, std::function)
constexpr uint32_t SceneUtil_createRightOutAnimation = 0x45BA3D;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t Npc_create = 0x5CC965;
constexpr uint32_t Audio_getInstance = 0x5BECFD, Audio_getLayer = 0x5BF33D, AudioLayer_playSE = 0x5BF785;
constexpr uint32_t SE_JEWEL = 0x1E56BAE4;
constexpr uint32_t BuildPanelDone = 0x646F3D;            // sub_646F3C below ($_56)
constexpr uint32_t M_resetAvatarAction = 0x643605, M_addChangeMotionEvent = 0x63FA8D, M_moveAvatar = 0x6411CD;
constexpr uint32_t VT_56 = 0xF60D40, VT_57 = 0xF60D14, VT_58 = 0xF60CE8, VT_59 = 0xF60CBC, VT_60 = 0xF60C90;
constexpr uint32_t VT_61 = 0xF60C64, VT_62 = 0xF60C38, VT_63 = 0xF60C0C, VT_64 = 0xF60BE0, VT_65 = 0xF60BB4;
constexpr uint32_t VT_66 = 0xF60B88, VT_67 = 0xF60B5C, VT_68 = 0xF60B30, VT_69 = 0xF60B04;
constexpr uint32_t S_RANK_SOLO1 = 0xE036E9, S_RANK_SOLO2 = 0xE036F4, S_JEWEL_GET_BIG = 0xE036A4;
constexpr uint32_t S_CHIRISHI_BALOON = 0xE03A98, S_TXT_NAME = 0xDFDA10;
constexpr int32_t TXT_CHIRISHI = 101000022;
constexpr uint32_t NODE_setScale = 0x40, NODE_setPositionXY = 0x5C, NODE_setPositionVec = 0x4C, NODE_addChild = 0xF0;
constexpr uint32_t WIDGET_setScaleY = 0x30;
constexpr uint32_t MOTION_WALK = 0x1C;

// node runs EaseSineOut(SceneUtil::createRight{In,Out}Animation(node, 0, 0, fo)); fo is destroyed after
void slide(uint32_t node, uint32_t anim, uint32_t fo) {
    uint32_t a = call(anim, {node, 0, 0, fo});
    call(wl::Node_runAction, {node, call(EaseSineOut_create, {a})});
    functionDestroy(fo);
}
void slideIn(uint32_t node) {
    Frame f;
    slide(node, SceneUtil_createRightInAnimation, newEmptyFunction(f));
}
void slideInFn(uint32_t node, uint32_t fo) { slide(node, SceneUtil_createRightInAnimation, fo); }
// slide `node` out with an inline lambda {vt, a, b}
void slideOut2(uint32_t node, uint32_t vt, uint32_t a, uint32_t b) {
    Frame f;
    slide(node, SceneUtil_createRightOutAnimation, newFunction(f, vt, {a, b}));
}

// a heap __func (0x30 bytes): {vt, -, a, b, self, -, std::function moved from tmp}; stored into `out`
void heapLambda(uint32_t out, uint32_t vt, uint32_t a, uint32_t b, uint32_t self, uint32_t tmp) {
    st32(out + 16, 0);
    uint32_t p = call(fn::operator_new, {0x30});
    if (p) {
        st32(p, vt);
        st32(p + 0xC, b);
        st32(p + 8, a);
        st32(p + 0x10, self);
        call(FunctionVV_move, {p + 0x18, tmp});
        st32(p + 0x10, self);
    }
    st32(out + 16, p);
}

// `node` (at its current x) slides out; $vt holds {node-or-self, x, self, copy of done}
void slideOutWithDone(uint32_t node, uint32_t vt, uint32_t a, uint32_t self, uint32_t done) {
    float x = bf(vcall(node, node::getPositionX, {node}));
    Frame f;
    uint32_t tmp = f.alloc(20);
    functionCopy(tmp, done);
    uint32_t out = f.alloc(20);
    heapLambda(out, vt, a, fb(x), self, tmp);
    slide(node, SceneUtil_createRightOutAnimation, out);
    functionDestroy(tmp);
}

// `node` slides in; $_56 {self, copy of done} runs at the end
void slideInWithDone(uint32_t node, uint32_t self, uint32_t done) {
    Frame f;
    uint32_t lam = f.alloc(8 + 20);   // {self, -, std::function copy of done}
    st32(lam, self);
    functionCopy(lam + 8, done);
    st32(lam, self);
    uint32_t out = f.alloc(20);
    call(BuildPanelDone, {out, lam});
    slideInFn(node, out);
    functionDestroy(lam + 8);
}

// the Chirishi NPC walks out of the field (balloon hidden) and $_69 runs
void npcWalkOut(uint32_t self) {
    uint32_t npc = ld32(self + 0x2FC);
    if (!npc) return;
    uint32_t balloon = ld32(self + 0x300);
    vcall(balloon, node::setVisible, {balloon, 0});
    float s = ldf(self + OFF_SCALE) / ldf(self + OFF_SCALE_BASE);
    npc = ld32(self + 0x2FC);
    vcall(npc, node::setScaleX, {npc, fb(s)});
    Frame f;
    uint32_t pos = f.alloc(8);
    call(wl::Vec2_ctorXY, {pos, 0, fb(70.0f)});
    uint32_t move = call(wl::MoveTo_create, {fb(0.6f), pos});
    uint32_t fo = newFunction(f, VT_69, {self});
    uint32_t cf = call(wl::CallFunc_create, {fo});
    call(wl::Node_runAction, {npc, call(wl::Sequence_create, {move, cf, 0})});
    functionDestroy(fo);
    call(wl::Vec2_dtor, {pos});
}

// the jewel panel slides in, the leader walks to the front and the Chirishi NPC (created once) walks in with
// her balloon
void jewelPanelIn(uint32_t self, uint32_t done) {
    const uint32_t jewel = findNode(ld32(self + 0x290), S_JEWEL_GET_BIG, 12);
    vcall(jewel, node::setVisible, {jewel, 1});
    slideInWithDone(jewel, self, done);
    {
        Frame f;
        uint32_t layer = f.alloc(8);
        call(Audio_getLayer, {layer, call(Audio_getInstance, {}), 0});
        call(AudioLayer_playSE, {ld32(layer), SE_JEWEL, 0, 0});
        sharedRelease(layer);
    }
    call(M_resetAvatarAction, {0, ld32(self + 0x2BC)});
    call(M_addChangeMotionEvent, {self, ld32(self + 0x2BC), MOTION_WALK});
    {
        uint32_t leader = ld32(self + 0x2BC);
        Frame f;
        uint32_t pos = f.alloc(8);
        call(wl::Vec2_ctorXY, {pos, fb(540.0f), fb(70.0f)});
        uint32_t fo = newFunction(f, VT_57, {self});
        call(M_moveAvatar, {self, leader, pos, fo, fb(0.6f)});
        functionDestroy(fo);
        call(wl::Vec2_dtor, {pos});
    }
    uint32_t npc = ld32(self + 0x2FC);
    if (npc) {
        call(smain::lib::Node_stopAllActions, {npc});
    } else {
        npc = call(Npc_create, {999});
        st32(self + 0x2FC, npc);
        uint32_t layer = ld32(self + 0x2A4);
        vcall(layer, NODE_addChild, {layer, npc});
    }
    npc = ld32(self + 0x2FC);
    vcall(npc, NODE_setScale, {npc, fb(ldf(self + OFF_SCALE) / ldf(self + OFF_SCALE_BASE))});
    npc = ld32(self + 0x2FC);
    vcall(npc, NODE_setPositionXY, {npc, 0, fb(65.0f)});
    float sx = (-0.0f - ldf(self + OFF_SCALE)) / ldf(self + OFF_SCALE_BASE);
    npc = ld32(self + 0x2FC);
    vcall(npc, node::setScaleX, {npc, fb(sx)});
    uint32_t balloon = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_CHIRISHI_BALOON});
    st32(self + 0x300, balloon);
    {
        Frame f;
        uint32_t pos = f.alloc(8);
        call(wl::Vec2_ctorXY, {pos, fb(50.0f), fb(100.0f)});
        vcall(balloon, NODE_setPositionVec, {balloon, pos});
        call(wl::Vec2_dtor, {pos});
    }
    uint32_t name = findNode(ld32(self + 0x300), S_TXT_NAME, 8);
    setTextId(name, TXT_CHIRISHI);
    balloon = ld32(self + 0x300);
    vcall(balloon, WIDGET_setScaleY, {balloon, fb(0.6f)});
    balloon = ld32(self + 0x300);
    vcall(balloon, node::setScaleX, {balloon, 0xBF19999Au});
    balloon = ld32(self + 0x300);
    vcall(balloon, node::setVisible, {balloon, 0});
    npc = ld32(self + 0x2FC);
    vcall(npc, NODE_addChild, {npc, ld32(self + 0x300)});
    npc = ld32(self + 0x2FC);
    Frame f;
    uint32_t pos = f.alloc(8);
    call(wl::Vec2_ctorXY, {pos, fb(420.0f), fb(65.0f)});
    uint32_t move = call(wl::MoveTo_create, {fb(0.6f), pos});
    uint32_t fo = newFunction(f, VT_58, {self});
    uint32_t cf = call(wl::CallFunc_create, {fo});
    call(wl::Node_runAction, {npc, call(wl::Sequence_create, {move, cf, 0})});
    functionDestroy(fo);
    call(wl::Vec2_dtor, {pos});
}

uint32_t solo2(uint32_t self) { return findNode(ld32(self + 0x290), S_RANK_SOLO2, 10); }
uint32_t solo1(uint32_t self) { return findNode(ld32(self + 0x290), S_RANK_SOLO1, 10); }
uint32_t rankingFlag() {
    uint32_t api = call(APIManager_getInstance, {});
    return sbRead(ld32(api + 0x84), 0x13, 1);
}
}  // namespace

// Builds the std::function of changeRankingHeader's $_56 (heap: self + a moved std::function) into `out`
uint32_t SceneMyPage_buildPanelDone(uint32_t out, uint32_t lam) {
    st32(out + 16, 0);
    uint32_t p = call(fn::operator_new, {0x28});
    if (p) {
        st32(p, VT_56);
        st32(p + 8, ld32(lam));
        call(FunctionVV_move, {p + 0x10, lam + 8});
        st32(p + 8, ld32(lam));
    }
    st32(out + 16, p);
    return out;
}

// toRanking: the ranking panels slide in (party: the party panels; ranking kind 1: the ranking ones; otherwise
// the solo panels and the jewel panel with the NPC). Back: they slide out and the NPC walks away.
uint32_t SceneMyPage_changeRankingHeader(uint32_t self, uint32_t toRanking, uint32_t doneArg) {
    Frame frame;
    const uint32_t done = frame.alloc(20);
    call(FunctionVV_copy, {done, doneArg});
    st8(self + 0x2F3, 1);
    const bool party = ld8(self + OFF_HAS_PARTY) != 0;
    if (toRanking == 1) {
        if (party) {
            uint32_t a = ld32(self + 0x2B8);
            vcall(a, node::setVisible, {a, 1});
            slideIn(ld32(self + 0x2B8));
            uint32_t b = ld32(self + 0x2B4);
            vcall(b, node::setVisible, {b, 1});
            slideInWithDone(ld32(self + 0x2B4), self, done);
        } else if (ld32(self + 0x28C) == 1) {
            uint32_t a = ld32(self + 0x2AC);
            vcall(a, node::setPositionX, {a, ld32(self + OFF_RANK_X_OUT)});
            a = ld32(self + 0x2AC);
            vcall(a, node::setVisible, {a, 1});
            slideIn(ld32(self + 0x2AC));
            uint32_t s2 = solo2(self);
            vcall(s2, node::setPositionX, {s2, ld32(self + OFF_RANK_X)});
            vcall(s2, node::setVisible, {s2, 1});
            slideIn(s2);
            uint32_t b = ld32(self + 0x2B4);
            vcall(b, node::setVisible, {b, 1});
            slideInWithDone(ld32(self + 0x2B4), self, done);
        } else if (rankingFlag()) {
            uint32_t a = ld32(self + 0x2AC);
            vcall(a, node::setPositionX, {a, ld32(self + OFF_SOLO_X)});
            a = ld32(self + 0x2AC);
            vcall(a, node::setVisible, {a, 1});
            slideIn(ld32(self + 0x2AC));
            uint32_t s2 = solo2(self);
            vcall(s2, node::setPositionX, {s2, ld32(self + OFF_RANK_X)});
            vcall(s2, node::setVisible, {s2, 1});
            slideIn(s2);
            jewelPanelIn(self, done);
        } else {
            uint32_t a = ld32(self + 0x2B0);
            vcall(a, node::setVisible, {a, 1});
            slideIn(ld32(self + 0x2B0));
            uint32_t s1 = solo1(self);
            vcall(s1, node::setVisible, {s1, 1});
            slideIn(s1);
            jewelPanelIn(self, done);
        }
    } else if (party) {
        uint32_t b = ld32(self + 0x2B4);
        float x = bf(vcall(b, node::getPositionX, {b}));
        slideOut2(ld32(self + 0x2B4), VT_59, self, fb(x));
        slideOutWithDone(ld32(self + 0x2B8), VT_60, self, self, done);
        npcWalkOut(self);
    } else if (ld32(self + 0x28C) == 1) {
        uint32_t b = ld32(self + 0x2B4);
        float x = bf(vcall(b, node::getPositionX, {b}));
        slideOut2(ld32(self + 0x2B4), VT_61, self, fb(x));
        uint32_t a = ld32(self + 0x2AC);
        float x2 = bf(vcall(a, node::getPositionX, {a}));
        slideOut2(ld32(self + 0x2AC), VT_62, self, fb(x2));
        uint32_t s2 = solo2(self);
        slideOutWithDone(s2, VT_63, s2, self, done);
        npcWalkOut(self);
    } else {
        uint32_t api84 = call(APIManager_getInstance, {}) + 0x84;
        if (sbRead(ld32(api84), 0x13, 1)) {
            uint32_t a = ld32(self + 0x2AC);
            float x = bf(vcall(a, node::getPositionX, {a}));
            slideOut2(ld32(self + 0x2AC), VT_64, self, fb(x));
        } else {
            uint32_t a = ld32(self + 0x2B0);
            float x = bf(vcall(a, node::getPositionX, {a}));
            slideOut2(ld32(self + 0x2B0), VT_65, self, fb(x));
        }
        if (sbRead(ld32(api84), 0x13, 1)) {
            uint32_t s2 = solo2(self);
            float x = bf(vcall(s2, node::getPositionX, {s2}));
            slideOut2(s2, VT_66, s2, fb(x));
        } else {
            uint32_t s1 = solo1(self);
            float x = bf(vcall(s1, node::getPositionX, {s1}));
            slideOut2(s1, VT_67, s1, fb(x));
        }
        uint32_t jewel = findNode(ld32(self + 0x290), S_JEWEL_GET_BIG, 12);
        slideOutWithDone(jewel, VT_68, jewel, self, done);
        npcWalkOut(self);
    }
    functionDestroy(done);
    return 0;
}

RW_FUNCTION(0x643C9C, "SceneMyPage::changeRankingHeader", SceneMyPage_changeRankingHeader);
RW_FUNCTION(0x646F3C, "sub_646F3C (changeRankingHeader $_56 std::function)", SceneMyPage_buildPanelDone);
