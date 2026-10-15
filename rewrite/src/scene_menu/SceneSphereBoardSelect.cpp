// SceneSphereBoardSelect: life cycle, animations, scene changes and the board / incentive helpers.
#include "SceneSphereBoardSelect.hpp"

using namespace scene;
using Self = SceneSphereBoardSelect;

namespace {
constexpr uint32_t Layer_ctor = 0x88A7D1;                // cocos2d::Layer::Layer
constexpr uint32_t Layer_dtor = 0x88A88D;                // cocos2d::Layer::~Layer
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Scene_create = 0x8A5F37;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t IntSet_destroy = 0x48D9B5;            // std::__tree<int>::destroy(node)
constexpr uint32_t SphereVector_dtor = 0x50248D;         // __vector_base<shared_ptr<const master::Sphere>> dtor
constexpr uint32_t IntVector_copy = 0x4AF707;            // std::vector<int>::vector(const vector&)
constexpr uint32_t IntVector_dtor = 0x478961;            // std::__vector_base<int>::~__vector_base
constexpr uint32_t InOut_runOutAnimation = 0x6F24B5;     // IInOutAnimation::runOutAnimation(function<void()>)
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneManager_isOpenedPopup = 0x621519;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;
constexpr uint32_t Function_vb_assign = 0x490109;        // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SceneUtil_createTopInAnimation = 0x45B03D;
constexpr uint32_t SceneUtil_createBottomInAnimation = 0x45B2F9;
constexpr uint32_t SceneUtil_createTopOutAnimation = 0x45B1DD;
constexpr uint32_t SceneUtil_createBottomOutAnimation = 0x45B469;
constexpr uint32_t AppState_getAvatarBoardFilterState = 0x4EB189;   // (shared_ptr out, AppState*)
constexpr uint32_t master_Sphere_create = 0x78CF21;       // (shared_ptr out, id)
constexpr uint32_t master_Sphere_getSphereBoardIds = 0x78CF05;      // (std::set<int> out)
constexpr uint32_t master_SphereMasu_create = 0x78DD09;
constexpr uint32_t master_AvatarParts_create = 0x778DE1;
constexpr uint32_t UserSphereMap_find = 0x503479;        // map<int, UserSphere::Sphere>::find(const int&)
constexpr uint32_t SceneSphereBoard_create = 0x6BCE79;
constexpr uint32_t SceneSphereBoard_BoardMaking = 0x6BF385;

constexpr uint32_t VSLOT_INIT = 0x250;                    // from the vtable start: init()
constexpr uint32_t VSLOT_DELETE = 0x04;                   // deleting destructor (through the object)

constexpr uint32_t SCENEMANAGER_MODE = 0x28C;             // SceneManager: the menu the scene was opened from
constexpr uint32_t SCENEMANAGER_FLAG_308 = 0x308;
constexpr uint32_t SCENEMANAGER_FLAG_4CC = 0x4CC;
constexpr uint32_t SCENEMANAGER_CALLBACK_548 = 0x548;     // std::function<void(bool)>
constexpr uint32_t APIMANAGER_USER_SPHERES = 0xB4;        // shared_ptr<map<int, UserSphere::Sphere>>
constexpr uint32_t SCROLL_LOCKED = 0x324;                 // SphereScrollView: byte, no more scrolling
constexpr uint32_t SCROLL_SPHERES = 0x3C4;                // SphereScrollView: vector<shared_ptr<Sphere>> begin
constexpr uint32_t SCROLL_SPHERES_END = 0x3C8;
constexpr uint32_t SCROLL_COUNT = 0x3B4;                  // SphereScrollView: number of boards
constexpr uint32_t SCROLL_INDEX_BASE = 0x31C;             // SphereScrollView: first item index
constexpr uint32_t VSLOT_SCROLL_FIND_ITEM = 0x274;        // SphereScrollView virtual: (float x, int* idx, float*)

constexpr uint32_t VT_ChangeSceneNext = 0xF66784;         // changeSceneNext $_3 [boardId]
constexpr uint32_t VT_RunOutDone = 0xF665F8;              // runOutAnimationMain $_11 [std::function<void()>]
constexpr uint32_t VT_BackToMyPage = 0xF665CC;            // backScene $_14
constexpr uint32_t VT_BackToAdventure = 0xF665A0;         // backScene $_15

constexpr uint32_t STR_SelectArea = 0xE0561E;
constexpr uint32_t STR_CenterUI = 0xE02FBA;

// in-order successor of a libc++ red-black tree node (left +0, right +4, parent +8)
uint32_t treeNext(uint32_t node) {
    uint32_t r = ld32(node + 4);
    if (r) {
        node = r;
        while (ld32(node)) node = ld32(node);
        return node;
    }
    for (;;) {
        uint32_t parent = ld32(node + 8);
        if (ld32(parent) == node) return parent;
        node = parent;
    }
}

// node->runAction(EaseSineOut::create(anim(node, 0, 0, std::function<void()>())))
void runEasedAnimation(uint32_t node, uint32_t animFn, uint32_t done) {
    uint32_t action = call(animFn, {node, 0, 0, done});
    call(fn::Node_runAction, {node, call(EaseSineOut_create, {action})});
}
void runEasedAnimation(uint32_t node, uint32_t animFn) {
    Frame f;
    uint32_t none = newEmptyFunction(f);
    runEasedAnimation(node, animFn, none);
    functionDestroy(none);
}

// the incentive kind of a sphere square (master::SphereMasu kind) and its parameter -> the counter index
// get_type / sum_Incentive / update use; -1 for kinds without a counter
int32_t incentiveType(uint32_t kind, int32_t param) {
    switch (kind) {
    case 0: return 2;
    case 3: return param != 0 ? 2 : 1;
    case 5: return 4;
    case 7: return 6;
    case 12: return 5;
    case 13: return 3;
    case 18: case 19: case 20: case 23: return 0;
    case 21: return 7;
    case 22: return 8;
    default: return -1;
    }
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
SceneSphereBoardSelect::Addr SceneSphereBoardSelect::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->inOutVtable = INOUT_VTABLE;
    s->avatarBoardPos = 0;
    s->boardType = 1;
    s->boardIdsRoot = 0;
    s->boardIdsSize = 0;
    s->spheres.cap = 0;
    s->spheres.end = 0;
    s->spheres.begin = 0;
    s->root = 0;
    s->boardIdsBegin = self + 0x2CC;        // empty set: begin = end node
    s->selectedId = -1;
    s->flag321 = 0;
    s->flag320 = 0;
    s->changingScene = 0;
    rw::set<uint16_t>(s, 0x330, 0);         // scrolling and canBack
    s->rightArrowButton = 0;
    s->leftArrowButton = 0;
    s->field_324 = 0;
    if (call(ld32(VTABLE_START + VSLOT_INIT), {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, VSLOT_DELETE, {self});
    return 0;
}
RW_FUNCTION(0x636E28, "SceneSphereBoardSelect::create", SceneSphereBoardSelect::create);

SceneSphereBoardSelect::Addr SceneSphereBoardSelect::createScene() {
    uint32_t sceneObj = call(Scene_create, {});
    uint32_t layer = create();
    vcall(sceneObj, node::addChild, {sceneObj, layer});
    return sceneObj;
}
RW_FUNCTION(0x6B640C, "SceneSphereBoardSelect::createScene", SceneSphereBoardSelect::createScene);

SceneSphereBoardSelect::Addr SceneSphereBoardSelect::destroy(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->inOutVtable = INOUT_VTABLE;
    call(IntSet_destroy, {self + 0x2C8, s->boardIdsRoot});
    call(SphereVector_dtor, {self + 0x2BC});
    call(Layer_dtor, {self});
    return self;
}
RW_FUNCTION(0x6B6430, "SceneSphereBoardSelect::~SceneSphereBoardSelect", SceneSphereBoardSelect::destroy);

void SceneSphereBoardSelect::destroyThunk(Addr inOut) { destroy(inOut - INOUT_OFFSET); }
RW_FUNCTION(0x6B6484, "SceneSphereBoardSelect::~SceneSphereBoardSelect (thunk)", SceneSphereBoardSelect::destroyThunk);

void SceneSphereBoardSelect::destroyDelete(Addr self) {
    destroy(self);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6B6494, "SceneSphereBoardSelect::~SceneSphereBoardSelect (deleting)", SceneSphereBoardSelect::destroyDelete);

void SceneSphereBoardSelect::destroyDeleteThunk(Addr inOut) {
    uint32_t self = inOut - INOUT_OFFSET;
    destroy(self);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6B64A0, "SceneSphereBoardSelect::~SceneSphereBoardSelect (deleting thunk)", SceneSphereBoardSelect::destroyDeleteThunk);

// ------------------------------------------------------------------------------------------------ animations
// The list slides in from the top, the centre panel from the bottom.
void SceneSphereBoardSelect::runInAnimation(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    uint32_t area = findNode(s->root, STR_SelectArea, 10);
    runEasedAnimation(area, SceneUtil_createTopInAnimation);
    uint32_t center = findNode(s->root, STR_CenterUI, 8);
    runEasedAnimation(center, SceneUtil_createBottomInAnimation);
}
RW_FUNCTION(0x6B9D60, "SceneSphereBoardSelect::runInAnimation", SceneSphereBoardSelect::runInAnimation);

void SceneSphereBoardSelect::runInAnimationThunk(Addr inOut) { runInAnimation(inOut - INOUT_OFFSET); }
RW_FUNCTION(0x6B9F00, "SceneSphereBoardSelect::runInAnimation (thunk)", SceneSphereBoardSelect::runInAnimationThunk);

// Slide out; `done` (std::function<void()>) runs when the centre panel is gone.
void SceneSphereBoardSelect::runOutAnimationMain(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    call(SceneManager_setCanTouchHeader, {sceneManager(), 0});
    uint32_t area = findNode(s->root, STR_SelectArea, 10);
    runEasedAnimation(area, SceneUtil_createTopOutAnimation);
    uint32_t center = findNode(s->root, STR_CenterUI, 8);
    Frame f;
    uint32_t captured = f.alloc(20);
    call(fn::function_vv_copy, {captured, done});
    uint32_t onDone = f.alloc(20);
    heapFunction(onDone, VT_RunOutDone, 0x20, 8, fn::function_vv_move, captured);
    runEasedAnimation(center, SceneUtil_createBottomOutAnimation, onDone);
    functionDestroy(onDone);
    functionDestroy(captured);
}
RW_FUNCTION(0x6B9F10, "SceneSphereBoardSelect::runOutAnimationMain", SceneSphereBoardSelect::runOutAnimationMain);

void SceneSphereBoardSelect::runOutAnimationMainThunk(Addr inOut, Addr done) {
    runOutAnimationMain(inOut - INOUT_OFFSET, done);
}
RW_FUNCTION(0x6BA128, "SceneSphereBoardSelect::runOutAnimationMain (thunk)", SceneSphereBoardSelect::runOutAnimationMainThunk);

// ------------------------------------------------------------------------------------------------ scene changes
// A board was chosen: lock the list and slide out, then open the board (SceneSphereBoard) - $_3.
void SceneSphereBoardSelect::changeSceneNext(Addr self, int32_t boardId) {
    Self *s = rw::host_ptr<Self>(self);
    s->changingScene = 1;
    st8(s->scrollView + SCROLL_LOCKED, 1);
    Frame f;
    uint32_t next = newFunction(f, VT_ChangeSceneNext, {uint32_t(boardId)});
    call(InOut_runOutAnimation, {self + INOUT_OFFSET, next});
    functionDestroy(next);
}
RW_FUNCTION(0x6B8310, "SceneSphereBoardSelect::changeSceneNext", SceneSphereBoardSelect::changeSceneNext);

// Back: close an open popup, else return to My Page (opened from there, mode 1) or to the adventure select.
void SceneSphereBoardSelect::backScene(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->canBack == 0) return;
    uint32_t opened = call(SceneManager_isOpenedPopup, {sceneManager()});
    uint32_t mgr = sceneManager();
    if (opened == 1) {
        call(SceneManager_closeCurrentPopup, {mgr, 0});
        return;
    }
    Frame f;
    if (int32_t(ld32(mgr + SCENEMANAGER_MODE)) == 1) {
        uint32_t back = newFunction(f, VT_BackToMyPage, {});
        call(InOut_runOutAnimation, {self + INOUT_OFFSET, back});
        functionDestroy(back);
        return;
    }
    s->canBack = 0;
    st8(sceneManager() + SCENEMANAGER_FLAG_308, 0);
    uint32_t m = sceneManager();
    uint32_t none = newEmptyFunction(f);
    st8(m + SCENEMANAGER_FLAG_4CC, 1);
    call(Function_vb_assign, {m + SCENEMANAGER_CALLBACK_548, none});
    functionDestroy(none);
    uint32_t none2 = newEmptyFunction(f);
    call(SceneAdventureSelect_callAdventureSelectAPI, {none2});
    functionDestroy(none2);
    uint32_t back = newFunction(f, VT_BackToAdventure, {});
    call(InOut_runOutAnimation, {self + INOUT_OFFSET, back});
    functionDestroy(back);
}
RW_FUNCTION(0x6BB63C, "SceneSphereBoardSelect::backScene", SceneSphereBoardSelect::backScene);

// ------------------------------------------------------------------------------------------------ list helpers
// The list index under the centre of the scroll view (wrapping around lists of 6+ boards), -1 when empty.
int32_t SceneSphereBoardSelect::calc_index(Addr self) {
    uint32_t scroll = rw::host_ptr<Self>(self)->scrollView;
    if (ld32(scroll + SCROLL_SPHERES_END) == ld32(scroll + SCROLL_SPHERES)) return -1;
    float x = (ldf(scroll + 0x318) + ldf(scroll + 0x3AC)) + ldf(scroll + 0x3B0) * 0.5f;
    Frame f;
    uint32_t out = f.alloc(8);                           // [+0] float, [+4] index
    vcall(scroll, VSLOT_SCROLL_FIND_ITEM, {scroll, fbits(x), out + 4, out});
    scroll = rw::host_ptr<Self>(self)->scrollView;
    int32_t index = int32_t(ld32(out + 4) + ld32(scroll + SCROLL_INDEX_BASE));
    int32_t count = int32_t(ld32(scroll + SCROLL_COUNT));
    if (count >= 6) {
        while (index < 0) {
            index = int32_t(uint32_t(count) + uint32_t(index));
            count = int32_t(ld32(scroll + SCROLL_COUNT));
        }
        return int32_t(call(0xCD6009, {uint32_t(index), uint32_t(count)}));   // __modsi3
    }
    if (index >= count) {
        index = int32_t(call(0xCD6009, {uint32_t(index), uint32_t(count)})); // __modsi3
    } else if (index <= -2) {
        index = int32_t(uint32_t(count) + uint32_t(index));
    }
    if (index < 0) return 0;
    count = int32_t(ld32(scroll + SCROLL_COUNT));
    if (index >= count) return count - 1;
    return index;
}
RW_FUNCTION(0x6B83C4, "SceneSphereBoardSelect::calc_index", SceneSphereBoardSelect::calc_index);

int32_t SceneSphereBoardSelect::get_type(Addr, int32_t kind, int32_t param) {
    if (uint32_t(kind) > 23) return -1;
    return incentiveType(uint32_t(kind), param);
}
RW_FUNCTION(0x6B90FC, "SceneSphereBoardSelect::get_type", SceneSphereBoardSelect::get_type);

// The gender of the avatar parts a board's squares give (1 / 2), 3 when they mix, 0 when there are none.
int32_t SceneSphereBoardSelect::getAvatarBoardGender(Addr, int32_t masuId) {
    Frame f;
    uint32_t masu = f.alloc(8);
    call(master_SphereMasu_create, {masu, uint32_t(masuId)});
    uint32_t parts = f.alloc(8);
    int32_t gender = 0;
    int32_t result = 0;
    bool mixed = false;
    for (uint32_t i = 0; i < 22; i++) {
        uint32_t kind = uint32_t(safe32(ld32(masu), 0x64 + 4 * i));
        if (kind > 23 || !((1u << kind) & 0x9C0000)) continue;   // avatar part squares: kinds 18, 19, 20, 23
        int32_t partsId = safe32(ld32(masu), 0x11C + 4 * i);
        call(master_AvatarParts_create, {parts, uint32_t(partsId)});
        if (gender != 0 && gender != safe32(ld32(parts), 0x8C)) {
            sharedRelease(parts);
            mixed = true;
            break;
        }
        gender = safe32(ld32(parts), 0x8C);
        sharedRelease(parts);
    }
    result = mixed ? 3 : gender;
    sharedRelease(masu);
    return result;
}
RW_FUNCTION(0x6B91A4, "SceneSphereBoardSelect::getAvatarBoardGender", SceneSphereBoardSelect::getAvatarBoardGender);

// 1: a square of `kind` (-1: any avatar part kind) is still closed on the board (its index + 1 is not in the
// user's opened list).
uint32_t SceneSphereBoardSelect::getAvatarBoardSpaceCheck(Addr, Addr userSphere, int32_t masuId, int32_t kind) {
    Frame f;
    uint32_t masu = f.alloc(8);
    call(master_SphereMasu_create, {masu, uint32_t(masuId)});
    uint32_t opened = f.alloc(12);
    call(IntVector_copy, {opened, userSphere + 0x14});
    int32_t bytes = int32_t(ld32(opened + 4) - ld32(opened));
    int32_t count = bytes >> 2;
    uint32_t result = 0;
    for (uint32_t i = 0; i < 22; i++) {
        int32_t k = safe32(ld32(masu), 0x64 + 4 * i);
        if (k != kind) {
            if (kind != -1) continue;
            uint32_t d = uint32_t(k) - 18;
            if (d > 5 || !((0x27u >> (d & 0xFF)) & 1)) continue;
        }
        if (bytes < 1) {
            result = 1;
            break;
        }
        bool found = false;
        for (int32_t j = 0; j < count;) {
            int32_t v = int32_t(ld32(ld32(opened) + 4 * uint32_t(j)));
            j++;
            if (v == int32_t(i + 1)) {
                found = true;
                break;
            }
        }
        if (!found) {
            result = 1;
            break;
        }
    }
    call(IntVector_dtor, {opened});
    sharedRelease(masu);
    return result;
}
RW_FUNCTION(0x6B92E0, "SceneSphereBoardSelect::getAvatarBoardSpaceCheck", SceneSphereBoardSelect::getAvatarBoardSpaceCheck);

// Count the incentive of square `index` (1-based) of the board's squares: incentiveCount[type] += 1.
void SceneSphereBoardSelect::sum_Incentive(Addr self, Addr masuPtr, int32_t index) {
    uint32_t i = uint32_t(index) - 1;
    if (i > 0x3E5) return;
    // the original decodes the 23 kinds / parameters into its frame and indexes them without a bound: keep the
    // frame where it was so an out-of-range index reads the same stack words
    Frame f;
    uint32_t sp = f.allocExact(0x14 + 0xCC);
    const SafeBuffer *masu = safe(ld32(masuPtr));
    SafeBuffer::read(masu, rw::host_ptr<uint8_t>(sp + 0x6C), masu->data.get() + 0x64, 0x5C, 0x64);
    uint32_t kind = ld32(sp + 0x6C + 4 * i);
    SafeBuffer::read(masu, rw::host_ptr<uint8_t>(sp + 0x10), masu->data.get() + 0xC0, 0x5C, 0xC0);
    int32_t type = kind > 23 ? -1 : incentiveType(kind, int32_t(ld32(sp + 0x10 + 4 * i)));
    uint32_t counter = self + 0x2D4 + 4 * uint32_t(type);      // type -1 bumps the word before the counters
    st32(counter, ld32(counter) + 1);
}
RW_FUNCTION(0x6B9408, "SceneSphereBoardSelect::sum_Incentive", SceneSphereBoardSelect::sum_Incentive);

// Boards of `type` the user owns.
int32_t SceneSphereBoardSelect::getSphereBoardValidCnt(Addr, int32_t type) {
    uint32_t mapSlot = call(fn::APIManager_getInstance, {}) + APIMANAGER_USER_SPHERES;
    if (ld32(mapSlot) == 0) return 0;
    // the id set lives in the original frame (its nodes point back at the end node)
    Frame f;
    uint32_t sp = f.allocExact(0x14 + 0x44);
    uint32_t ids = sp + 0x2C;
    call(master_Sphere_getSphereBoardIds, {ids});
    int32_t count = 0;
    uint32_t sphere = sp + 0x24;
    uint32_t key = sp + 0x20;
    for (uint32_t node = ld32(ids); node != ids + 4; node = treeNext(node)) {
        call(master_Sphere_create, {sphere, ld32(node + 0x10)});
        int32_t kind = safe32(ld32(sphere), 0x98);
        uint32_t map = ld32(mapSlot);
        st32(key, uint32_t(safe32(ld32(sphere), 0)));
        bool typeOk = kind == type;
        uint32_t it = call(UserSphereMap_find, {map, key});
        bool owned = it != ld32(mapSlot) + 4;
        if (typeOk && owned) count++;
        sharedRelease(sphere);
    }
    call(IntSet_destroy, {ids, ld32(ids + 4)});
    return count;
}
RW_FUNCTION(0x6BB4FC, "SceneSphereBoardSelect::getSphereBoardValidCnt", SceneSphereBoardSelect::getSphereBoardValidCnt);

// ------------------------------------------------------------------------------------------------ filter
namespace {
// UserSphere::Sphere passed by value: 5 words and the opened square list (vector<int>), copied to the stack
uint32_t copyUserSphere(Frame &f, uint32_t userSphere) {
    uint32_t tmp = f.alloc(0x20);
    for (uint32_t k = 0; k < 5; k++) st32(tmp + 4 * k, ld32(userSphere + 4 * k));
    call(IntVector_copy, {tmp + 0x14, userSphere + 0x14});
    return tmp;
}
// getAvatarBoardSpaceCheck(copy of userSphere, masu of the board, kind)
uint32_t spaceCheck(uint32_t sphere, uint32_t userSphere, int32_t kind) {
    Frame f;
    uint32_t tmp = copyUserSphere(f, userSphere);
    int32_t masuId = safe32(ld32(sphere), 0x94);
    uint32_t r = Self::getAvatarBoardSpaceCheck(0, tmp, masuId, kind);
    call(IntVector_dtor, {tmp + 0x14});
    return r;
}
}  // namespace

// The avatar board filter (AppState): byte 0 on, 1 hide owned, 2 hide not owned, 3 / 4 male / female parts,
// 5 not completed, 6..9 squares of kinds 21, 12, 22, 3 still closed. Returns 1 when the board is filtered out.
uint32_t SceneSphereBoardSelect::avaterBoardDispCheck(Addr, Addr sphere, Addr userSphere) {
    Frame f;
    uint32_t state = f.alloc(8);
    call(AppState_getAvatarBoardFilterState, {state, call(fn::AppState_getInstance, {})});
    uint32_t filter = ld32(state);
    uint32_t r6 = 0;
    int32_t gender;
    uint32_t f2, f3, f4;
    if (ld8(filter) == 0) goto done;
    gender = getAvatarBoardGender(0, safe32(ld32(sphere), 0x94));
    if (ld8(filter + 1) != 0) {
        if (int32_t(ld32(userSphere + 0xC)) != 3) goto L8600;
        if (gender == 0 || gender == 3) goto zero;
        f4 = ld8(filter + 4);
        f3 = ld8(filter + 3);
        if (f3 == 0) {
            if (f4 == 0) goto zero;
            if (gender != 2) goto one;
            if (ld8(filter + 4) != 0) goto zero;
            goto one;
        }
        if (f4 != 0) goto zero;
        if (gender != 1) goto one;
        if (ld8(filter + 4) != 0) goto one;
        goto zero;
    }
    // filter[1] == 0
    f2 = ld8(filter + 2);
    if (f2 != 0) {
        r6 = 1;
        if (int32_t(ld32(userSphere + 0xC)) == 3) goto done;
        goto L860C;
    }
    // filter[1] == 0 && filter[2] == 0
    if (gender == 0 || gender == 3) goto zero;
    f4 = ld8(filter + 4);
    f3 = ld8(filter + 3);
    if (f3 == 0) {
        if (f4 == 0) goto zero;
        goto L88B4;
    }
    {
        uint32_t r3 = f4 != 0 ? 1 : 0;
        if (gender == 1) goto zero;
        if (r3 != 0) goto zero;
        goto L88B4;
    }
L88B4: {
        uint32_t r2 = 1;
        r6 = gender != 2 ? 1 : 0;
        if (ld8(filter + 4) != 0) r2 = 0;
        r6 |= r2;
        goto done;
    }
L8600:
    r6 = 1;
    if (ld8(filter + 2) == 0) goto done;
L860C:
    if (gender != 0) {
        f4 = ld8(filter + 4);
        f3 = ld8(filter + 3);
        r6 = 1;
        if (f3 == 0) {
            if (gender == 1 && f4 != 0) goto done;
        } else {
            if (gender == 2 && f4 == 0) goto done;
        }
    }
    // not completed / closed squares
    {
        uint32_t f5 = ld8(filter + 5), f7 = ld8(filter + 7);
        bool checkAny = f7 != 0 ? f5 != 0 : f5 != 0;
        if (checkAny) {
            if (spaceCheck(sphere, userSphere, -1) != 0) goto zero;
        } else if (f7 == 0) {
            if (ld8(filter + 8) == 0 && ld8(filter + 6) == 0 && ld8(filter + 9) == 0) goto zero;
        }
    }
    if (ld8(filter + 6) != 0 && spaceCheck(sphere, userSphere, 0x15) != 0) goto zero;
    if (ld8(filter + 7) != 0 && spaceCheck(sphere, userSphere, 0xC) != 0) goto zero;
    if (ld8(filter + 8) != 0 && spaceCheck(sphere, userSphere, 0x16) != 0) goto zero;
    r6 = 1;
    if (ld8(filter + 9) == 0) goto done;
    r6 = spaceCheck(sphere, userSphere, 3) ^ 1;
    goto done;
one:
    r6 = 1;
    goto done;
zero:
    r6 = 0;
done:
    sharedRelease(state);
    return r6;
}
RW_FUNCTION(0x6B8550, "SceneSphereBoardSelect::avaterBoardDispCheck", SceneSphereBoardSelect::avaterBoardDispCheck);
