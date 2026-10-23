// SceneSellOutMaterialItem leftovers: createScene, four lambdas (a delayed CallFunc, the back button's sphere check
// and layer change, the quest list API call, the battle result after a sale) and the sort helpers of the two
// material orders over vector<ull>: std::__sort4 / __sort5 of the first comparator (sub_6899D8) and
// std::__insertion_sort_incomplete of both (the second's __sort3 .. __sort5 are in w5_13/MaterialSale.cpp).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Scene_create = 0x8A5F37, SceneSellOutMaterialItem_create = 0x6861AD;
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t SM_changeSceneLayer = 0x612B99, SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t FunctionVB_assign = 0x490109;                 // std::function<void(bool)>::operator=(function&&)
constexpr uint32_t APIManager_executeUserSphereCheck = 0x4CCBC1, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;
constexpr uint32_t SceneBattleResult_startResult = 0x683265;
// order A (sub_6899D8): __sort3 sub_689B54; order B (sub_68B318): __sort3 / 4 / 5 sub_68B494 / 68B556 / 68B5EE
constexpr uint32_t LessA = 0x6899D9, Sort3A = 0x689B55, Sort4A = 0x689C17, Sort5A = 0x689CAF;
constexpr uint32_t LessB = 0x68B319, Sort3B = 0x68B495, Sort4B = 0x68B557, Sort5B = 0x68B5EF;
}  // namespace g

constexpr uint32_t VT_Delayed = 0xF64810;      // {2 bytes}
constexpr uint32_t VT_SphereChecked = 0xF64868, VT_BackLayer = 0xF6483C;
constexpr uint32_t VT_QuestList = 0xF64708, VT_ResultRetry = 0xF648C0;
constexpr uint32_t SM_SPHERE_FLAG = 0x4CC, SM_SPHERE_DONE = 0x548;

inline void st16(uint32_t a, uint16_t v) { rw::save<uint16_t>(a, v); }

uint32_t createScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(g::SceneSellOutMaterialItem_create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}

// {-, node}: after 0.1 s the CallFunc {2 bytes}
uint32_t delayed(uint32_t self) {
    const uint32_t node = ld32(self + 8);
    const uint32_t delay = call(g::DelayTime_create, {fbits(0.1f)});
    Frame f;
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, fo);
    st32(fo, VT_Delayed);
    st16(fo + 4, 0);
    const uint32_t cf = call(g::CallFunc_create, {fo});
    call(fn::Node_runAction, {node, call(g::Sequence_create, {delay, cf, 0})});
    functionDestroy(fo);
    return 0;
}

// the back button: no sphere callback pending, check the spheres (type 3), back to the previous layer
uint32_t backButton() {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    st8(sm + SM_SPHERE_FLAG, 1);
    call(g::FunctionVB_assign, {sm + SM_SPHERE_DONE, none});
    functionDestroy(none);
    const uint32_t a = call(fn::APIManager_getInstance, {});
    {
        Frame t;
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, fo);
        st32(fo, VT_SphereChecked);
        st16(fo + 4, 0);
        call(g::APIManager_executeUserSphereCheck, {a, 3, fo});
        functionDestroy(fo);
    }
    const uint32_t sm2 = sceneManager();
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_BackLayer, {});
        call(g::SM_changeSceneLayer, {sm2, fo, 1, 1, 1});
        functionDestroy(fo);
    }
    return 0;
}

// (bool ok): the quest list
uint32_t toQuestList(uint32_t, uint32_t okp) {
    if (!ld8(okp)) return 0;
    Frame f;
    const uint32_t fo = newFunction(f, VT_QuestList, {});
    call(g::SceneAdventureSelect_callAdventureSelectAPI, {fo});
    functionDestroy(fo);
    return 0;
}

// {result scene}(bool ok): the battle result goes on, or the retry dialog
uint32_t saleAnswered(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    const uint32_t scene = ld32(self + 4);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        call(g::SceneBattleResult_startResult, {scene});
        return 0;
    }
    if (!ld8(call(fn::APIManager_getInstance, {}) + 0x14)) return 0;
    const uint32_t a = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_ResultRetry, {scene});
    call(g::APIManager_openErrorDialog, {a, fo, 1});
    functionDestroy(fo);
    return 0;
}

// ---------------------------------------------------------------- sorts over ull (8-byte values)
void swap8(uint32_t a, uint32_t b) {
    const uint32_t a0 = ld32(a), a1 = ld32(a + 4);
    st32(a, ld32(b));
    st32(a + 4, ld32(b + 4));
    st32(b, a0);
    st32(b + 4, a1);
}
uint32_t sort4A(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t comp) {
    uint32_t r = call(g::Sort3A, {x1, x2, x3, comp});
    if (!call(g::LessA, {comp, x4, x3})) return r;
    swap8(x3, x4);
    if (!call(g::LessA, {comp, x3, x2})) return r + 1;
    swap8(x2, x3);
    if (!call(g::LessA, {comp, x2, x1})) return r + 2;
    swap8(x1, x2);
    return r + 3;
}
uint32_t sort5A(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t comp) {
    uint32_t r = sort4A(x1, x2, x3, x4, comp);
    if (!call(g::LessA, {comp, x5, x4})) return r;
    swap8(x4, x5);
    if (!call(g::LessA, {comp, x4, x3})) return r + 1;
    swap8(x3, x4);
    if (!call(g::LessA, {comp, x3, x2})) return r + 2;
    swap8(x2, x3);
    if (!call(g::LessA, {comp, x2, x1})) return r + 3;
    swap8(x1, x2);
    return r + 4;
}
struct Order {
    uint32_t less, sort3, sort4, sort5;
};
uint32_t insertionSortIncomplete(const Order &o, uint32_t first, uint32_t last, uint32_t comp) {
    switch ((last - first) >> 3) {
    case 0:
    case 1: return 1;
    case 2:
        if (call(o.less, {comp, last - 8, first})) swap8(first, last - 8);
        return 1;
    case 3: call(o.sort3, {first, first + 8, last - 8, comp}); return 1;
    case 4: call(o.sort4, {first, first + 8, first + 16, last - 8, comp}); return 1;
    case 5: call(o.sort5, {first, first + 8, first + 16, first + 24, last - 8, comp}); return 1;
    default: break;
    }
    uint32_t j = first + 16;
    call(o.sort3, {first, first + 8, j, comp});
    if (first + 24 == last) return 1;
    Frame f;
    const uint32_t t = f.alloc(8);
    uint32_t count = 0;
    for (uint32_t i = first + 24;;) {
        if (call(o.less, {comp, i, j})) {
            st32(t, ld32(i));
            st32(t + 4, ld32(i + 4));
            uint32_t k = j, dest;
            for (;;) {
                st32(k + 8, ld32(k));
                st32(k + 12, ld32(k + 4));
                if (k == first) {
                    dest = first;
                    break;
                }
                if (!call(o.less, {comp, t, k - 8})) {
                    dest = k;
                    break;
                }
                k -= 8;
            }
            st32(dest, ld32(t));
            st32(dest + 4, ld32(t + 4));
            if (++count == 8) return i + 8 == last ? 1 : 0;
        }
        j = i;
        i += 8;
        if (i == last) return 1;
    }
}
uint32_t insertionA(uint32_t first, uint32_t last, uint32_t comp) {
    return insertionSortIncomplete({g::LessA, g::Sort3A, g::Sort4A, g::Sort5A}, first, last, comp);
}
uint32_t insertionB(uint32_t first, uint32_t last, uint32_t comp) {
    return insertionSortIncomplete({g::LessB, g::Sort3B, g::Sort4B, g::Sort5B}, first, last, comp);
}
}  // namespace

RW_FUNCTION(0x686188, "SceneSellOutMaterialItem::createScene()", createScene);
RW_FUNCTION(0x6850F8, "SceneSellOutMaterialItem delayed CallFunc (sub_6850F8)", delayed);
RW_FUNCTION(0x68588C, "SceneSellOutMaterialItem back button (sub_68588C)", backButton);
RW_FUNCTION(0x685AF0, "SceneSellOutMaterialItem quest list (sub_685AF0)", toQuestList);
RW_FUNCTION(0x685DF0, "SceneSellOutMaterialItem sale answered (sub_685DF0)", saleAnswered);
RW_FUNCTION(0x689C16, "std::__sort4 (material order A) (sub_689C16)", sort4A);
RW_FUNCTION(0x689CAE, "std::__sort5 (material order A) (sub_689CAE)", sort5A);
RW_FUNCTION(0x689D70, "std::__insertion_sort_incomplete (material order A) (sub_689D70)", insertionA);
RW_FUNCTION(0x68B6B0, "std::__insertion_sort_incomplete (material order B) (sub_68B6B0)", insertionB);
