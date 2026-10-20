// Medal List > long press > Evolve: SceneEvolveMedal's frame - create, destructor, the material medal cells
// (createMaterialMedal: a DeckMedalView with the owned count), the four panels' in / out animations, backScene and
// its lambda (back to the Medal List, the Equipment screen or the medal's detail), and the run-out lambda's clone /
// destroy, plus the clone of an init lambda that holds an unordered_set of medal ids. init, initEvolutLayer and
// initBurstInfo (~15 KB of code) stay lifted for now.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t Color3B_ctor = 0x9062E7;
constexpr uint32_t DeckMedalView_create = 0x52BA71, DeckMedalView_setData = 0x52BEE5;
constexpr uint32_t DeckMedalViewManager_getInstance = 0x527131;
constexpr uint32_t DeckMedalViewManager_addDeckMedalView = 0x527559, DeckMedalViewManager_removeDeckMedalView = 0x5275CD;
constexpr uint32_t VectorView_pushSlow = 0x5306D5, VectorView_dtor = 0x52C439;
constexpr uint32_t VectorULL_dtor = 0x468499;
constexpr uint32_t UnorderedSetULL_copy = 0x4740A9;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t SceneUtil_setNeedMedalColor = 0x468689;
constexpr uint32_t SceneUtil_createLeftIn = 0x45B5C5, SceneUtil_createLeftOut = 0x45B735;
constexpr uint32_t SceneUtil_createBottomIn = 0x45B2F9, SceneUtil_createBottomOut = 0x45B469;
constexpr uint32_t SceneUtil_createRightIn = 0x45B8BD, SceneUtil_createRightOut = 0x45BA3D;
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D, SM_isOpenedPopup = 0x621519, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t GOT_VT_Evolve = 0xFD68EC;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_MEDAL_LIST = 0xFD60BC, GOT_CREATE_DECK_EDIT = 0xFD66B0;
}  // namespace g

constexpr uint32_t SIZE = 0x2E8;
constexpr uint32_t F_INOUT = 0x288;
constexpr uint32_t F_LEFT = 0x290, F_UNDER = 0x294, F_CENTER_RIGHT = 0x298, F_CENTER_LEFT = 0x29C;
constexpr uint32_t F_MEDAL = 0x2A0;          // shared_ptr<UserMedalData>
constexpr uint32_t F_IDS_A = 0x2A8, F_IDS_B = 0x2B4;   // vector<ull>
constexpr uint32_t F_VIEWS = 0x2C0;          // vector<DeckMedalView*> (registered with the manager)
constexpr uint32_t F_FROM = 0x2CC;           // where the screen was opened from (0 deck edit, 1 list, 2 detail)
constexpr uint32_t F_DETAIL_ARG = 0x2D0, F_FROM_MEDAL = 0x2D8;   // u64 medal id
constexpr uint32_t F_CAN_BACK = 0x2E3;
constexpr uint32_t VT_RunOutDone = 0xF66204;  // runOutAnimationMain {std::function<void()>} (heap 0x20)
constexpr uint32_t VT_Back = 0xF66154;        // backScene {this}: sub_6B1370
constexpr uint32_t VT_DetailLayer = 0xF66180; // function<Layer*()> {detail argument}
constexpr uint32_t VT_IdSetLambda = 0xF662B4; // init lambda {3 flags, unordered_set<ull>, 3 words} (heap 0x28)
constexpr uint32_t S_EvoMedalJson = 0xE054B2; // "cocostudio/publish/MedalInfo_Evo_Medal.json"
constexpr uint32_t S_MedalS = 0xDF8F79, S_medalView = 0xDFD055, S_Possession = 0xE054DE, S_Label9 = 0xE054E9;
constexpr uint32_t S_NumLabel = 0xDF8A89, S_FmtInt = 0xDF86DE;
constexpr int32_t TXT_POSSESSION = 101400002;
constexpr uint32_t SM_HEADER_308 = 0x308, SM_DETAIL_MEDAL = 0x2A0;
constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_setBright = 0x238;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void runEased(uint32_t make, uint32_t node, uint32_t done) {
    const uint32_t action = call(make, {node, 0, 0, done});
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
void runEasedEmpty(uint32_t make, uint32_t node) {
    Frame f;
    const uint32_t done = newEmptyFunction(f);
    runEased(make, node, done);
    functionDestroy(done);
}
void changeScene(uint32_t factoryGot) {
    const uint32_t m = sceneManager();
    Frame f;
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(factoryGot)});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
    functionDestroy(layer);
}
}  // namespace

struct SceneEvolveMedal {
    static uint32_t create();
    static uint32_t destruct(uint32_t self);
    static uint32_t createMaterialMedal(uint32_t self, uint32_t medal, uint32_t count, uint32_t enough);
    static void runInAnimation(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static uint32_t cloneRunOutDone(uint32_t functor);
    static void destroyRunOutDone(uint32_t functor);
    static uint32_t cloneIdSetLambda(uint32_t functor);
};

uint32_t SceneEvolveMedal::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    const uint32_t vt = ld32(g::GOT_VT_Evolve);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    call(g::aeabi_memset, {me + 0x28C, 0x48, 0});
    st32(me + 0x2E0, 0);
    st32(me + 0x2DC, 0);
    st32(me + 0x2D8, 0);
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t SceneEvolveMedal::destruct(uint32_t me) {
    const uint32_t vt = ld32(g::GOT_VT_Evolve);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    uint32_t views = ld32(me + F_VIEWS);
    if (ld32(me + F_VIEWS + 4) != views) {
        for (uint32_t i = 0;;) {
            const uint32_t view = ld32(views + 4 * i);
            call(g::DeckMedalViewManager_removeDeckMedalView, {call(g::DeckMedalViewManager_getInstance, {}), view});
            views = ld32(me + F_VIEWS);
            if (!(++i < (ld32(me + F_VIEWS + 4) - views) >> 2)) break;
        }
    }
    call(g::VectorView_dtor, {me + F_VIEWS});
    call(g::VectorULL_dtor, {me + F_IDS_B});
    call(g::VectorULL_dtor, {me + F_IDS_A});
    releaseIf(ld32(me + F_MEDAL + 4));
    call(g::Layer_dtor, {me});
    return me;
}

// A material medal cell: the medal (a registered DeckMedalView), "Possession" with the owned count; greyed out when
// there are not enough.
uint32_t SceneEvolveMedal::createMaterialMedal(uint32_t me, uint32_t medal, uint32_t count, uint32_t enough) {
    const uint32_t widget = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_EvoMedalJson});
    const uint32_t medalS = findNode(widget, S_MedalS, 7);
    Frame f;
    const uint32_t viewSlot = f.alloc(4);
    const uint32_t view = call(g::DeckMedalView_create, {1});
    st32(viewSlot, view);
    {
        const uint32_t copy = f.alloc(8), detail = f.alloc(8);
        st32(copy, ld32(medal));
        st32(copy + 4, ld32(medal + 4));
        if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
        st32(detail, 0);
        st32(detail + 4, 0);
        rw::call(g::DeckMedalView_setData, {view, copy, 0xFFFFFFFFu, 4, 1, 10000, 10000, 0, 0, 0, detail});
        releaseIf(ld32(detail + 4));
        releaseIf(ld32(copy + 4));
    }
    {
        const uint32_t name = newStr(f, S_medalView, 9);
        vcall(medalS, node::addChildZName, {medalS, view, 0, name});
        strFree(name);
    }
    vcall(medalS, node::setContentSize, {medalS, vcall(view, node::getContentSize, {view})});
    call(g::DeckMedalViewManager_addDeckMedalView, {call(g::DeckMedalViewManager_getInstance, {}), view});
    {
        const uint32_t end = ld32(me + F_VIEWS + 4);
        if (end == ld32(me + F_VIEWS + 8)) {
            call(g::VectorView_pushSlow, {me + F_VIEWS, viewSlot});
        } else {
            uint32_t n = 0;
            if (end) {
                st32(end, view);
                n = ld32(me + F_VIEWS + 4);
            }
            st32(me + F_VIEWS + 4, n + 4);
        }
    }
    const uint32_t possession = findNode(widget, S_Possession, 0xA);
    vcall(possession, node::setLocalZOrder, {possession, 2});
    const uint32_t label = findNode(possession, S_Label9, 7);
    SceneUtil::setShadow(label);
    setTextId(label, TXT_POSSESSION);
    const uint32_t num = findNode(possession, S_NumLabel, 9);
    SceneUtil::setShadow(num);
    {
        Frame t;
        setTextC(num, stringBuffer(t, {S_FmtInt, count}));
    }
    if (!(enough & 0xFF)) {
        vcall(view, V_setBright, {view, 1});
        const uint32_t grey = f.alloc(4);
        call(g::Color3B_ctor, {grey, 0x64, 0x64, 0x64});
        vcall(view, node::setColor, {view, grey});
        call(g::SceneUtil_setNeedMedalColor, {label});
        call(g::SceneUtil_setNeedMedalColor, {num});
    }
    return widget;
}

// ------------------------------------------------------------------------------------------------ animations
void SceneEvolveMedal::runInAnimation(uint32_t me) {
    runEasedEmpty(g::SceneUtil_createLeftIn, ld32(me + F_LEFT));
    runEasedEmpty(g::SceneUtil_createBottomIn, ld32(me + F_UNDER));
    runEasedEmpty(g::SceneUtil_createRightIn, ld32(me + F_CENTER_RIGHT));
    runEasedEmpty(g::SceneUtil_createLeftIn, ld32(me + F_CENTER_LEFT));
}

// The panels slide out; `done` runs when the right centre panel is gone.
void SceneEvolveMedal::runOutAnimationMain(uint32_t me, uint32_t done) {
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    runEasedEmpty(g::SceneUtil_createLeftOut, ld32(me + F_LEFT));
    runEasedEmpty(g::SceneUtil_createBottomOut, ld32(me + F_UNDER));
    {
        const uint32_t node = ld32(me + F_CENTER_RIGHT);
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        const uint32_t onDone = f.alloc(20);
        heapFunction(onDone, VT_RunOutDone, 0x20, 8, fn::function_vv_move, copy);
        runEased(g::SceneUtil_createRightOut, node, onDone);
        functionDestroy(onDone);
        functionDestroy(copy);
    }
    runEasedEmpty(g::SceneUtil_createLeftOut, ld32(me + F_CENTER_LEFT));
}

// ------------------------------------------------------------------------------------------------ back
void SceneEvolveMedal::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    if (call(g::SM_isOpenedPopup, {sceneManager()}) == 1) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 0});
        return;
    }
    st8(me + F_CAN_BACK, 0);
    st8(sceneManager() + SM_HEADER_308, 0);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Back, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// backScene's lambda {this}: back to where the screen was opened from - the Equipment screen (0), the Medal List
// (1) or the medal's detail (2: when the medal still exists, else the Medal List).
void SceneEvolveMedal::backDone(uint32_t functor) {
    const uint32_t me = ld32(functor + 4);
    const uint32_t from = ld32(me + F_FROM);
    if (from == 2) {
        const uint32_t mgr = call(g::MedalDataManager_getInstance, {});
        Frame f;
        const uint32_t medal = f.alloc(8);
        call(g::MedalDataManager_getMedalData, {medal, mgr, ld32(me + F_FROM_MEDAL), ld32(me + F_FROM_MEDAL + 4), 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
        const uint32_t data = ld32(medal);
        if (ld32(data) | ld32(data + 4)) {
            const uint32_t lo = ld32(me + F_FROM_MEDAL), hi = ld32(me + F_FROM_MEDAL + 4);
            const uint32_t sm = sceneManager();
            st32(sm + SM_DETAIL_MEDAL + 4, hi);
            st32(sm + SM_DETAIL_MEDAL, lo);
            const uint32_t arg = ld32(me + F_DETAIL_ARG);
            const uint32_t m = sceneManager();
            const uint32_t layer = newFunction(f, VT_DetailLayer, {arg});
            rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
            functionDestroy(layer);
        } else {
            changeScene(g::GOT_CREATE_MEDAL_LIST);
        }
        releaseIf(ld32(medal + 4));
    } else if (from == 1) {
        changeScene(g::GOT_CREATE_MEDAL_LIST);
    } else if (from == 0) {
        changeScene(g::GOT_CREATE_DECK_EDIT);
    }
}

// the run-out lambda's __func::__clone() / destroy_deallocate()
uint32_t SceneEvolveMedal::cloneRunOutDone(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, VT_RunOutDone);
        call(fn::function_vv_copy, {obj + 8, functor + 8});
    }
    return obj;
}
void SceneEvolveMedal::destroyRunOutDone(uint32_t functor) {
    functionDestroy(functor + 8);
    call(fn::operator_delete, {functor});
}

// __func::__clone() of an init lambda {3 flag bytes, unordered_set<ull>, 3 words}
uint32_t SceneEvolveMedal::cloneIdSetLambda(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, VT_IdSetLambda);
        st8(obj + 5, ld8(functor + 5));
        st8(obj + 4, ld8(functor + 4));
        st8(obj + 6, ld8(functor + 6));
        call(g::UnorderedSetULL_copy, {obj + 8, functor + 8});
        for (uint32_t w = 0x1C; w < 0x28; w += 4) st32(obj + w, ld32(functor + w));
    }
    return obj;
}

RW_FUNCTION(0x65E1F8, "SceneEvolveMedal::create", SceneEvolveMedal::create);
RW_FUNCTION(0x6AA324, "SceneEvolveMedal::~SceneEvolveMedal", SceneEvolveMedal::destruct);
RW_FUNCTION(0x6AD680, "SceneEvolveMedal::createMaterialMedal", SceneEvolveMedal::createMaterialMedal);
RW_FUNCTION(0x6AF164, "SceneEvolveMedal::runInAnimation", SceneEvolveMedal::runInAnimation);
RW_FUNCTION(0x6AF314, "SceneEvolveMedal::runOutAnimationMain", SceneEvolveMedal::runOutAnimationMain);
RW_FUNCTION(0x6AFD24, "SceneEvolveMedal::backScene", SceneEvolveMedal::backScene);
RW_FUNCTION(0x6B1370, "SceneEvolveMedal::backScene lambda __func::operator() (sub_6B1370)", SceneEvolveMedal::backDone);
RW_FUNCTION(0x6B19C8, "SceneEvolveMedal run-out lambda __func::__clone() (sub_6B19C8)", SceneEvolveMedal::cloneRunOutDone);
RW_FUNCTION(0x6B1A6C, "SceneEvolveMedal run-out lambda __func::destroy_deallocate() (sub_6B1A6C)",
            SceneEvolveMedal::destroyRunOutDone);
RW_FUNCTION(0x6B1ECC, "SceneEvolveMedal::init id-set lambda __func::__clone() (sub_6B1ECC)",
            SceneEvolveMedal::cloneIdSetLambda);
