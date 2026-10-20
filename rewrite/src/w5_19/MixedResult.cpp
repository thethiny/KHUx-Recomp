// The fusion's result: SceneMixedMedal::openResultPopup - the level up sound, the shadow and MedalMixResultScene on
// the popup layer, the new medal's details (updateMedalInfo against the old base medal), its mix circle, one result
// line per improvement (sub_6A8908: level, EXP, attack / defense and their bonuses, cost, burst level, burst, skill
// mix) stacked in the result area (five or more: the medal image moves down and shrinks), and the open animation
// whose callback ($_28) holds the lines and the popup's nodes.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_updateMedalInfo = 0x462195, SceneUtil_updateMixCircle = 0x465C85;
constexpr uint32_t WidgetVector_dtor = 0x485A8F, WidgetVector_pushBack = 0x4F6B5D;   // cocos2d::Vector<Widget*>
constexpr uint32_t WidgetVector_copy = 0x6A8799, WidgetStdVector_deallocate = 0x6A2E65;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AudioManager_getInstance = 0x5BECFD, AudioManager_getLayer = 0x5BF33D, AudioLayer_playSE = 0x5BF785;
constexpr uint32_t SM_getPopupLayer = 0x613E69;
constexpr uint32_t Mixed_getGuiltFactorValueRange = 0x69A309;
constexpr uint32_t Mixed_resultLine = 0x6A8909;          // sub_6A8908(context, kind): one result line widget
constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
constexpr uint32_t Vec2_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17;
constexpr uint32_t aeabi_uidiv = 0xCD5DF9;
constexpr uint32_t memcpy = 0x40E164;
constexpr uint32_t GOT_VEC2_ZERO = 0xFD605C;
}  // namespace g

constexpr uint32_t F_BASE = 0x31C;            // the base medal's UserMedalData (before the fusion)
constexpr uint32_t F_FLAG_340 = 0x340, F_SKILL_MIX = 0x380;
constexpr uint32_t SE_LEVEL_UP = 509000340;
constexpr uint32_t S_ResultJson = 0xE0513C;   // "cocostudio/publish/MedalMixResultScene_ver120.json" (50)
constexpr uint32_t S_MedalInfo = 0xDF8D25, S_AreaMixCircle = 0xDF8B8A, S_AreaGuiltIcon = 0xDF8B99;
constexpr uint32_t S_AreaGuiltPlate = 0xDF8BA9, S_ResultArea = 0xE05334, S_DummyMedal = 0xDF8C16;
constexpr uint32_t S_CloseButton = 0xDF8D2F;
constexpr uint32_t VT_Result = 0xF65680;      // $_28 (heap 0x58)
constexpr uint32_t V_getPosition = 0x54;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
uint32_t medalRead(Frame &f, uint32_t medal, uint32_t off, uint32_t len) {
    const uint32_t dst = f.alloc((len + 3) & ~3u);
    const uint32_t sb = ld32(medal + 0x58);
    rw::call(g::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + off, len, off});
    return dst;
}
// a long std::string built in place: operator new(cap + 1), memcpy of the literal, terminator
uint32_t heapString(Frame &f, uint32_t cap1, uint32_t text, uint32_t len) {
    const uint32_t s = f.alloc(12);
    const uint32_t p = call(fn::operator_new, {cap1 - 1});
    st32(s, cap1);
    st32(s + 4, len);
    st32(s + 8, p);
    call(g::memcpy, {p, text, len});
    st8(p + len, 0);
    return s;
}
}  // namespace

struct MixedResult {
    static void openResultPopup(uint32_t self, uint32_t medalRef);
};

void MixedResult::openResultPopup(uint32_t me, uint32_t medalRef) {
    Frame f;
    {
        const uint32_t mgr = call(g::AudioManager_getInstance, {});
        const uint32_t layer = f.alloc(8);
        call(g::AudioManager_getLayer, {layer, mgr, 0});
        call(g::AudioLayer_playSE, {ld32(layer), SE_LEVEL_UP, 0, 0});
        release(ld32(layer + 4));
    }
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = SceneUtil::createShadowSprite();
    {
        const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
    }
    uint32_t root;
    {
        Frame g2;
        const uint32_t s = newStr(g2, S_ResultJson, 50);
        root = SceneUtil::createNodeWithSceneFile(s);
        strFree(s);
    }
    vcall(layer, node::addChild, {layer, root});
    const uint32_t rootPos = f.alloc(8);
    call(g::Vec2_copy, {rootPos, vcall(root, V_getPosition, {root})});
    const uint32_t info = find(root, S_MedalInfo, 9);
    {
        const uint32_t inner = find(info, S_MedalInfo, 9);
        vcall(inner, node::setAnchorPoint, {inner, ld32(g::GOT_VEC2_ZERO)});
    }
    const uint32_t api1 = call(g::APIManager_getInstance, {});
    const uint32_t api2 = call(g::APIManager_getInstance, {});
    const uint32_t medalObj = ld32(medalRef), medalCtl = ld32(medalRef + 4);
    {
        const uint32_t copy = f.alloc(8);
        st32(copy, medalObj);
        st32(copy + 4, medalCtl);
        addRef(medalCtl);
        Frame g2;
        const uint32_t ownedByte = g2.alloc(4);
        const uint32_t sb = ld32(api1 + 0x94);
        rw::call(g::SafeBuffer_read, {sb, ownedByte, ld32(sb + 0xC) + 0x54, 1, 0x54});
        const uint32_t owned = ld8(ownedByte);
        const uint32_t prev = f.alloc(8);
        st32(prev, ld32(me + F_BASE));
        st32(prev + 4, ld32(me + F_BASE + 4));
        addRef(ld32(prev + 4));
        const uint32_t skillUp = ld8(ld32(api2 + 0x190) + 0xA0);
        call(g::SceneUtil_updateMedalInfo, {root, copy, owned ? 1u : 0u, 0, prev, skillUp});
        release(ld32(prev + 4));
        release(ld32(copy + 4));
    }
    const int32_t maxLevel = int32_t(ld32(medalRead(f, medalObj, 0x47C, 4)));
    const uint32_t atMax = maxLevel > int32_t(ld32(medalObj + 8)) ? 0 : 1;
    const uint32_t range = f.alloc(8);
    st32(range, 0);
    st32(range + 4, 0);
    {
        const uint32_t guilt = ld32(medalRead(f, medalObj, 0x478, 4));
        call(g::Mixed_getGuiltFactorValueRange, {guilt, range});
    }
    const uint32_t guiltShown = call(g::aeabi_uidiv, {ld32(range + 4), 100});
    const uint32_t circle = find(info, S_AreaMixCircle, 14);
    const uint32_t guiltIcon = find(info, S_AreaGuiltIcon, 15);
    const uint32_t guiltPlate = find(info, S_AreaGuiltPlate, 16);
    {
        const uint32_t copy = f.alloc(8);
        st32(copy, medalObj);
        st32(copy + 4, medalCtl);
        addRef(medalCtl);
        Frame g2;
        const uint32_t ownedByte = g2.alloc(4);
        const uint32_t sb = ld32(api1 + 0x94);
        rw::call(g::SafeBuffer_read, {sb, ownedByte, ld32(sb + 0xC) + 0x54, 1, 0x54});
        call(g::SceneUtil_updateMixCircle,
             {circle, guiltIcon, guiltPlate, copy, ld8(ownedByte) ? 1u : 0u, 0, 1, ld8(me + F_FLAG_340), 0, 0, 0});
        release(ld32(copy + 4));
    }
    // the result lines' context: {root, this | this, medal, the API's +0x190 shared_ptr, medal, root, root, layer,
    // shadow, this, at max level}
    const uint32_t ctx = f.alloc(0x40);
    const uint32_t apiObj = ld32(api2 + 0x190), apiCtl = ld32(api2 + 0x194);
    addRef(medalCtl);
    addRef(apiCtl);
    st32(ctx, root);
    st32(ctx + 4, me);
    st32(ctx + 8, me);
    st32(ctx + 0xC, medalObj);
    st32(ctx + 0x10, medalCtl);
    addRef(medalCtl);
    st32(ctx + 0x14, apiObj);
    st32(ctx + 0x18, apiCtl);
    addRef(apiCtl);
    st32(ctx + 0x1C, medalObj);
    st32(ctx + 0x20, medalCtl);
    addRef(medalCtl);
    st32(ctx + 0x24, root);
    st32(ctx + 0x28, root);
    st32(ctx + 0x2C, layer);
    st32(ctx + 0x30, shadow);
    st32(ctx + 0x34, me);
    st32(ctx + 0x38, atMax);
    uint32_t area;
    {
        Frame g2;
        const uint32_t s = heapString(g2, 0x21, S_ResultArea, 20);
        area = SceneUtil::searchNodeByName(root, s);
        strFree(s);
    }
    vcall(area, node::setVisible, {area, 0});
    const uint32_t lines = f.alloc(12);
    st32(lines, 0);
    st32(lines + 4, 0);
    st32(lines + 8, 0);
    auto addLine = [&](uint32_t kind) {
        const uint32_t line = call(g::Mixed_resultLine, {ctx + 8, kind});
        const float h = ldf(vcall(line, node::getContentSize, {line}) + 4);
        const uint32_t count = uint32_t(int32_t(ld32(lines + 4) - ld32(lines)) >> 2);
        vcall(line, node::setPositionY, {line, fbits(h * float(int32_t(count)))});
        vcall(area, node::addChild, {area, line});
        call(g::WidgetVector_pushBack, {lines, line});
    };
    {
        const uint32_t oldM = ld32(me + F_BASE), newM = ld32(ctx + 0x1C);
        auto o = [&](uint32_t off) { return int32_t(ld32(ld32(me + F_BASE) + off)); };
        auto n = [&](uint32_t off) { return int32_t(ld32(newM + off)); };
        (void)oldM;
        if (ld8(me + F_SKILL_MIX)) addLine(9);
        if (uint32_t(o(0x54)) < uint32_t(n(0x54))) addLine(8);
        if (o(0x50) < n(0x50) || (o(0x54) == 0 && n(0x54) != 0)) addLine(7);
        if (o(0x28) < n(0x28)) addLine(6);
        if (o(0x30) < n(0x30)) addLine(5);
        if (o(0x2C) < n(0x2C)) addLine(4);
        if (o(0x10) - o(0x30) < n(0x10) - n(0x30)) addLine(3);
        if (o(0xC) - o(0x2C) < n(0xC) - n(0x2C)) addLine(2);
        if (o(0x20) < n(0x20)) addLine(1);
        if (o(0x8) < n(0x8)) addLine(0);
    }
    if (int32_t(ld32(lines + 4) - ld32(lines)) >= 0x11) {
        uint32_t dummy;
        {
            Frame g2;
            const uint32_t s = heapString(g2, 0x11, S_DummyMedal, 11);
            dummy = SceneUtil::searchNodeByName(ld32(ctx), s);
            strFree(s);
        }
        const float extra = float(int32_t((int32_t(ld32(lines + 4) - ld32(lines)) >> 2) - 4)) / 6.0f;
        const float shrink = extra * 0.5f;
        const float y = bitsf(vcall(dummy, node::getPositionY, {dummy}));
        vcall(dummy, node::setPositionY, {dummy, fbits(extra * 100.0f + y)});
        const float scale = bitsf(vcall(dummy, node::getScale, {dummy}));
        vcall(dummy, node::setScale, {dummy, fbits(scale - shrink)});
    }
    // $_28's captures, copied as the original's nested lambdas do: {this, area, lines, root, root, layer, shadow,
    // this, at max level, root, circle, guilt icon, guilt plate, medal, the API's +0x94 shared_ptr, guilt range}
    const uint32_t linesA = f.alloc(12), linesB = f.alloc(12), linesC = f.alloc(12);
    call(g::WidgetVector_copy, {linesA, lines});
    call(g::WidgetVector_dtor, {lines});
    call(g::WidgetVector_copy, {linesB, linesA});
    const uint32_t sbObj = ld32(api1 + 0x94), sbCtl = ld32(api1 + 0x98);
    addRef(medalCtl);
    addRef(sbCtl);
    {
        const uint32_t close = find(root, S_CloseButton, 12);
        vcall(close, node::setVisible, {close, 0});
    }
    call(g::WidgetVector_copy, {linesC, linesB});
    addRef(medalCtl);
    addRef(sbCtl);
    const uint32_t cb = f.alloc(20);
    st32(cb + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x58});
    if (heap) {
        st32(heap, VT_Result);
        st32(heap + 4, me);
        st32(heap + 8, area);
        st32(heap + 0xC, 0);
        st32(heap + 0x10, 0);
        st32(heap + 0x14, 0);
        call(g::WidgetStdVector_deallocate, {heap + 0xC});
        for (uint32_t k = 0; k < 12; k += 4) {
            st32(heap + 0xC + k, ld32(linesC + k));
            st32(linesC + k, 0);
        }
        st32(heap + 0x18, root);
        st32(heap + 0x1C, root);
        st32(heap + 0x20, layer);
        st32(heap + 0x24, shadow);
        st32(heap + 0x28, me);
        st32(heap + 0x2C, atMax);
        st32(heap + 0x30, root);
        st32(heap + 0x34, circle);
        st32(heap + 0x38, guiltIcon);
        st32(heap + 0x3C, guiltPlate);
        st32(heap + 0x40, medalObj);
        st32(heap + 0x44, medalCtl);
        st32(heap + 0x48, sbObj);
        st32(heap + 0x4C, sbCtl);
        addRef(sbCtl);
        st8(heap + 0x54, 0);
        st32(heap + 0x50, guiltShown);
    } else {
        release(medalCtl);   // the moved copy is released with its holder
    }
    st32(cb + 16, heap);
    call(g::Node_runAction, {root, SceneUtil::createOpenPopupAnimation(root, cb)});
    functionDestroy(cb);
    release(sbCtl);
    call(g::WidgetVector_dtor, {linesC});
    release(sbCtl);
    release(medalCtl);
    call(g::WidgetVector_dtor, {linesB});
    call(g::WidgetVector_dtor, {linesA});
    release(medalCtl);
    release(apiCtl);
    release(medalCtl);
    release(apiCtl);
    release(medalCtl);
    call(g::Vec2_dtor, {rootPos});
}

RW_FUNCTION(0x69DE34, "SceneMixedMedal::openResultPopup", MixedResult::openResultPopup);
