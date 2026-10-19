// SceneSphereBoardSelect: the board list (initSphereBoardList), the centred board's counters (update) and backScene's
// return to MyPage ($_14). The list's lambda bodies ($_4 ... $_10) are not rewritten.
#include "SceneSphereBoardSelect.hpp"
#include "SphereScrollView.hpp"

using namespace scene;
using Self = SceneSphereBoardSelect;

namespace {
namespace g {
constexpr uint32_t IntSet_destroy = 0x48D9B5;              // std::__tree<int>::destroy(node)
constexpr uint32_t IntSet_insertUnique = 0x6BCBD5;         // __tree<int>::__insert_unique(int&&) -> pair (sret)
constexpr uint32_t IntSet_insertHint = 0x6BCA05;           // __tree<int>::__insert_unique(const_iterator, const int&)
constexpr uint32_t IntVector_copy = 0x4AF707;              // std::vector<int>::vector(const vector&)
constexpr uint32_t IntVector_base_dtor = 0x478961;
constexpr uint32_t SphereVector_copy = 0x6B8929;           // vector<shared_ptr<const Sphere>>::vector(const vector&)
constexpr uint32_t SphereVector_base_dtor = 0x50248D;
constexpr uint32_t SphereVector_pushBackSlow = 0x6BC885;   // __push_back_slow_path(const shared_ptr&)
constexpr uint32_t master_Sphere_create = 0x78CF21;         // (shared_ptr out, id)
constexpr uint32_t master_Sphere_getSphereBoardIds = 0x78CF05;      // (std::set<int> out)
constexpr uint32_t master_SphereArray_create = 0x78D675;
constexpr uint32_t master_SphereMasu_create = 0x78DD09;
constexpr uint32_t UserSphereMap_find = 0x503479;           // map<int, UserSphere::Sphere>::find(const int&) const
constexpr uint32_t AppState_getNewAvatarBoardIDs = 0x4EBE3B;        // const vector<int>&
constexpr uint32_t AppState_getAvatarBoardFilterState = 0x4EB189;   // (shared_ptr out, AppState*)
constexpr uint32_t AppState_getAvatarBoardPosNormal = 0x4EABB3;
constexpr uint32_t AppState_getAvatarBoardPosRaid = 0x4EABC5;
constexpr uint32_t AppState_getAvatarBoardPosColosseum = 0x4EABD7;
constexpr uint32_t AppState_getAvatarBoardPosStage = 0x4EABE9;
constexpr uint32_t AppState_setAvatarBoardPosNormal = 0x4EABA7;
constexpr uint32_t AppState_setAvatarBoardPosRaid = 0x4EABB9;
constexpr uint32_t AppState_setAvatarBoardPosColosseum = 0x4EABCB;
constexpr uint32_t AppState_setAvatarBoardPosStage = 0x4EABDD;
constexpr uint32_t Function_vi_swap = 0x5428E1;             // std::function<void(int)>::swap
constexpr uint32_t Function_vf_swap = 0x5E40BD;             // std::function<void(float)>::swap
constexpr uint32_t Function_viw_swap = 0x6BBF45;            // std::function<void(int, ui::Widget*)>::swap
constexpr uint32_t Function_vv_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t Function_vb_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t ImageButton_create = 0x531239;           // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;    // (button, Node*, float scale)
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;   // (button, std::function by value)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;       // (shared_ptr out, manager, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;           // (layer, ResourceID, int, int)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;     // (function<void(bool)> by value)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;       // (mgr, const function<Layer*()>&, bool, bool, FadeType)
}  // namespace g

// GOT slots (read at run time: the loader fills them)
constexpr uint32_t GOT_FUNC_LAYER_FACTORY_VTABLE = 0xFD6074;   // &_ZTV __func<Layer*(*)(), ..., Layer*()> (vptr + 8)
constexpr uint32_t GOT_CREATE_MYPAGE = 0xFD6078;               // &createSceneLayer<SceneMyPage>

// strings
constexpr uint32_t S_TxtCoinNameLabel = 0xE0560A;
constexpr uint32_t S_TxtCoinLabel = 0xE04769;
constexpr uint32_t S_SelectArea = 0xE0561E;
constexpr uint32_t S_LeftUIArrow = 0xE05629;
constexpr uint32_t S_RightUI = 0xDF85F7;
constexpr uint32_t S_Empty = 0xDF8A1A;
constexpr uint32_t S_PercentD = 0xDF86DE;

// texts
constexpr int32_t TXT_COIN_NORMAL = 103400001, TXT_COIN_COLOSSEUM = 103400002, TXT_COIN_STAGE = 103400003,
                  TXT_COIN_RAID = 103400027;
constexpr int32_t TXT_FILTER_ON = 100100012, TXT_FILTER_OFF = 100100035;
constexpr uint32_t SE_SELECT = 509000170;

// board types (boardType / master::Sphere +0x98)
constexpr int32_t TYPE_NORMAL = 1, TYPE_COLOSSEUM = 3, TYPE_STAGE = 4, TYPE_RAID = 5;

// APIManager
constexpr uint32_t API_USER_POINT = 0x8C;          // SafeBuffer*: coins at +0x18 (normal), +0x48 stage, +0x4C raid,
                                                   // +0x50 colosseum
constexpr uint32_t API_USER_SPHERES = 0xB4;        // map<int, UserSphere::Sphere>* (+0xB8: its control block)
// SceneManager
constexpr uint32_t SM_HAS_NEW_BOARD = 0x2F7;       // byte
constexpr uint32_t SM_BOARD_TYPE = 0x2F8;
constexpr uint32_t SM_FLAG_4CC = 0x4CC;
constexpr uint32_t SM_CALLBACK_548 = 0x548;        // std::function<void(bool)>
// SphereScrollView
constexpr uint32_t SCROLL_BOARDS = 0x3B4;          // boards on the wheel
constexpr uint32_t SCROLL_SPHERES = 0x3C4;
constexpr uint32_t SCROLL_ON_SCROLL = 0x2A0, SCROLL_ON_STOP = 0x2E8, SCROLL_ON_TAP = 0x300;

// initSphereBoardList's lambdas (__func vtables + 8)
constexpr uint32_t VT_OnScrollStop = 0xF66758;     // $_4 {this}
constexpr uint32_t VT_OnScrollMove = 0xF6672C;     // $_5 {this}
constexpr uint32_t VT_OnTapBoard = 0xF666D4;       // $_6 {this, user spheres (shared_ptr), new ids}, on the heap
constexpr uint32_t VT_OnEnter = 0xF666A8;          // $_7 {this}
constexpr uint32_t VT_OnExit = 0xF6667C;           // $_8 {this}
constexpr uint32_t VT_OnTapLeftArrow = 0xF66650;   // $_9 {arrow node, this}
constexpr uint32_t VT_OnTapRightArrow = 0xF66624;  // $_10 {arrow node, this}

// map<int, UserSphere::Sphere> node: key at +0x10, the value (5 words and the opened squares) at +0x14
constexpr uint32_t USER_SPHERE_WORDS = 0x14;
constexpr uint32_t USER_SPHERE_OPENED = 0x28;      // std::vector<int> of 1-based squares

constexpr uint32_t MASU_SQUARES = 23;              // master::SphereMasu: kinds at +0x64, parameters at +0xC0

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

void setNumber(uint32_t text, int32_t value) {
    Frame f;
    setTextC(text, stringBuffer(f, {S_PercentD, uint32_t(value)}));
}

// a shared_ptr copy on the stack (control block shared)
uint32_t sharedCopy(Frame &f, uint32_t sp) {
    uint32_t copy = f.alloc(8);
    st32(copy, ld32(sp));
    uint32_t ctrl = ld32(sp + 4);
    st32(copy + 4, ctrl);
    if (ctrl) call(fn::shared_add, {ctrl});
    return copy;
}

// An ImageButton over the first child of `arrow` (a transparent hit area), shown when there is more than one board.
uint32_t arrowButton(Self *self, uint32_t arrow, uint32_t vtable) {
    const uint32_t me = rw::guest_addr(self);
    const uint32_t children = vcall(arrow, node::getChildren, {arrow});
    const uint32_t target = ld32(ld32(children));
    uint32_t button;
    {
        Frame f;
        uint32_t file = newStr(f, S_Empty, 0);
        button = call(g::ImageButton_create, {file, 0});
        strFree(file);
    }
    call(g::ImageButton_setTargetView, {button, target, fbits(1.0f)});
    {
        Frame f;
        uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctor, {anchor, fbits(-0.5f), fbits(-0.5f)});
        call(g::ImageButton_setTargetAnchorPoint, {button, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(target, node::addChild, {target, button});
    const int32_t boards = int32_t(ld32(self->scrollView + SCROLL_BOARDS));
    vcall(arrow, node::setVisible, {arrow, boards > 1 ? 1u : 0u});
    {
        Frame f;
        uint32_t cb = newFunction(f, vtable, {arrow, me});
        call(g::ImageButton_addTouchEventListener, {button, cb});
        functionDestroy(cb);
    }
    return button;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ the list
// Fill the coin counter of the board type, create the wheel (SelectArea), collect the board ids, put the boards of
// this type the user owns on the wheel (those the avatar board filter hides only counted for the normal type), the
// item shown first being the remembered board, and wire the wheel, scene and arrow callbacks.
void Self::initSphereBoardList(Addr me) {
    Self *self = rw::host_ptr<Self>(me);
    const uint32_t coinName = findNode(self->root, S_TxtCoinNameLabel, 19);
    SceneUtil::setShadow(coinName);
    self->coinLabel = findNode(self->root, S_TxtCoinLabel, 14);
    SceneUtil::setShadow(self->coinLabel);
    // (the original also walks APIManager's vector<int> at +0x308 with at(), whose bound checks never fail)
    const uint32_t api = call(fn::APIManager_getInstance, {});
    int32_t coins = 0;
    if (ld32(api + API_USER_POINT)) {
        const uint32_t app = call(fn::AppState_getInstance, {});
        int32_t text;
        uint32_t coinField;
        switch (self->boardType) {
        case TYPE_COLOSSEUM:
            self->avatarBoardPos = int32_t(call(g::AppState_getAvatarBoardPosColosseum, {app}));
            coinField = 0x50;
            text = TXT_COIN_COLOSSEUM;
            break;
        case TYPE_STAGE:
            self->avatarBoardPos = int32_t(call(g::AppState_getAvatarBoardPosStage, {app}));
            coinField = 0x48;
            text = TXT_COIN_STAGE;
            break;
        case TYPE_RAID:
            self->avatarBoardPos = int32_t(call(g::AppState_getAvatarBoardPosRaid, {app}));
            coinField = 0x4C;
            text = TXT_COIN_RAID;
            break;
        default:
            self->avatarBoardPos = int32_t(call(g::AppState_getAvatarBoardPosNormal, {app}));
            coinField = 0x18;
            text = TXT_COIN_NORMAL;
            break;
        }
        coins = safe32(ld32(api + API_USER_POINT), coinField);
        setTextId(coinName, text);
    }
    setNumber(self->coinLabel, coins);

    const uint32_t area = findNode(self->root, S_SelectArea, 10);
    self->scrollView = SphereScrollView::create(4);
    vcall(area, node::addChild, {area, self->scrollView});
    SphereScrollView *scroll = rw::host_ptr<SphereScrollView>(self->scrollView);

    Frame f;                                       // the new ids and the board id set live to the end
    const uint32_t newIds = f.alloc(12);
    call(g::IntVector_copy, {newIds, call(g::AppState_getNewAvatarBoardIDs, {call(fn::AppState_getInstance, {})})});
    SphereScrollView::setSphereBoardNewId(scroll, newIds);
    st8(call(fn::SceneManager_getInstance, {}) + SM_HAS_NEW_BOARD, ld32(newIds + 4) != ld32(newIds) ? 1 : 0);
    st32(call(fn::SceneManager_getInstance, {}) + SM_BOARD_TYPE, uint32_t(self->boardType));
    st32(self->scrollView + SCROLL_BOARDS, 0);

    // this->boardIds = every board id of the master table
    call(g::IntSet_destroy, {me + 0x2C8, self->boardIdsRoot});
    self->boardIdsSize = 0;
    self->boardIdsBegin = me + 0x2CC;
    self->boardIdsRoot = 0;
    const uint32_t ids = f.alloc(12);
    call(g::master_Sphere_getSphereBoardIds, {ids});
    for (uint32_t node = ld32(ids); node != ids + 4; node = treeNext(node)) {
        Frame h;
        const uint32_t sphere = masterRow(h, g::master_Sphere_create, ld32(node + 0x10));
        const uint32_t key = h.alloc(4);
        st32(key, uint32_t(safe32(ld32(sphere), 0)));
        const uint32_t result = h.alloc(8);
        call(g::IntSet_insertUnique, {result, me + 0x2C8, key});
        sharedRelease(sphere);
    }

    if (ld32(api + API_USER_SPHERES)) {
        while (self->spheres.end != self->spheres.begin) {
            const uint32_t last = self->spheres.end - 8;
            self->spheres.end = last;
            const uint32_t ctrl = ld32(last + 4);
            if (ctrl) call(fn::shared_release, {ctrl});
        }
        Frame h;
        const uint32_t local = h.alloc(12);        // a copy of the id set
        st32(local + 4, 0);
        st32(local + 8, 0);
        st32(local, local + 4);
        int32_t filtered = 0;                      // owned boards of the type the filter hides
        int32_t start = 0;                         // wheel item of the remembered board
        if (self->boardIdsBegin != me + 0x2CC) {
            for (uint32_t node = self->boardIdsBegin; node != me + 0x2CC; node = treeNext(node))
                call(g::IntSet_insertHint, {local, local + 4, node + 0x10});
            for (uint32_t node = ld32(local); node != local + 4; node = treeNext(node)) {
                const uint32_t map = ld32(api + API_USER_SPHERES);
                const uint32_t it = call(g::UserSphereMap_find, {map, node + 0x10});
                if (it == map + 4) continue;
                Frame k;
                const uint32_t sphere = masterRow(k, g::master_Sphere_create, ld32(node + 0x10));
                const int32_t kind = safe32(ld32(sphere), 0x98);
                bool hidden = false;
                if (uint32_t(kind - 1) <= 1) {
                    const uint32_t sphereCopy = sharedCopy(k, sphere);
                    const uint32_t user = k.alloc(0x20);
                    for (uint32_t w = 0; w < 0x14; w += 4) st32(user + w, ld32(it + USER_SPHERE_WORDS + w));
                    call(g::IntVector_copy, {user + 0x14, it + USER_SPHERE_OPENED});
                    const uint32_t check = avaterBoardDispCheck(me, sphereCopy, user);
                    call(g::IntVector_base_dtor, {user + 0x14});
                    sharedRelease(sphereCopy);
                    if (check == 1) {
                        if (self->boardType == TYPE_NORMAL) filtered++;
                        hidden = true;
                    }
                }
                if (!hidden && uint32_t(kind - 1) <= 4) {
                    static constexpr int32_t TYPE_OF_KIND[5] = {TYPE_NORMAL, TYPE_NORMAL, TYPE_COLOSSEUM, TYPE_STAGE,
                                                                TYPE_RAID};
                    if (self->boardType == TYPE_OF_KIND[kind - 1]) {
                        const uint32_t end = self->spheres.end;
                        if (end == self->spheres.cap) {
                            call(g::SphereVector_pushBackSlow, {me + 0x2BC, sphere});
                        } else {
                            if (end) {
                                st32(end, ld32(sphere));
                                const uint32_t ctrl = ld32(sphere + 4);
                                st32(end + 4, ctrl);
                                if (ctrl) call(fn::shared_add, {ctrl});
                            }
                            self->spheres.end = end + 8;
                        }
                        const int32_t count = int32_t(ld32(self->scrollView + SCROLL_BOARDS));
                        st32(self->scrollView + SCROLL_BOARDS, uint32_t(count + 1));
                        if (self->avatarBoardPos == int32_t(ld32(node + 0x10))) start = count;
                    }
                }
                sharedRelease(sphere);
            }
        }
        const int32_t boards = int32_t(ld32(self->scrollView + SCROLL_BOARDS));
        setNumber(self->filterNumLabel, boards);
        setNumber(self->filterTotalLabel, int32_t(ld32(self->scrollView + SCROLL_BOARDS)) + filtered);
        {
            Frame k;
            const uint32_t copy = k.alloc(12);
            call(g::SphereVector_copy, {copy, me + 0x2BC});
            SphereScrollView::setSphereBoard(scroll, copy, start);
            call(g::SphereVector_base_dtor, {copy});
        }
        call(g::IntSet_destroy, {local, ld32(local + 4)});
    }

    // the wheel's callbacks
    {
        Frame h;
        uint32_t cb = newFunction(h, VT_OnScrollStop, {me});
        call(g::Function_vi_swap, {cb, self->scrollView + SCROLL_ON_STOP});
        functionDestroy(cb);
    }
    {
        Frame h;
        uint32_t cb = newFunction(h, VT_OnScrollMove, {me});
        call(g::Function_vf_swap, {cb, self->scrollView + SCROLL_ON_SCROLL});
        functionDestroy(cb);
    }
    {
        // $_6 {this, user spheres, new ids}: the captures are copied twice on the stack, then moved into the heap
        Frame h;
        const uint32_t first = h.alloc(0x18);
        st32(first, me);
        st32(first + 4, ld32(api + API_USER_SPHERES));
        st32(first + 8, ld32(api + API_USER_SPHERES + 4));
        if (ld32(first + 8)) call(fn::shared_add, {ld32(first + 8)});
        call(g::IntVector_copy, {first + 0xC, newIds});
        const uint32_t second = h.alloc(0x18);
        st32(second, me);
        st32(second + 4, ld32(first + 4));
        st32(second + 8, ld32(first + 8));
        if (ld32(second + 8)) call(fn::shared_add, {ld32(second + 8)});
        for (uint32_t w = 0; w < 12; w += 4) {
            st32(second + 0xC + w, ld32(first + 0xC + w));
            st32(first + 0xC + w, 0);
        }
        const uint32_t cb = h.alloc(20);
        st32(cb + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x1C});
        if (heap) {
            st32(heap, VT_OnTapBoard);
            st32(heap + 4, me);
            st32(heap + 8, ld32(second + 4));
            st32(heap + 0xC, ld32(second + 8));
            if (ld32(heap + 0xC)) call(fn::shared_add, {ld32(heap + 0xC)});
            for (uint32_t w = 0; w < 12; w += 4) {
                st32(heap + 0x10 + w, ld32(second + 0xC + w));
                st32(second + 0xC + w, 0);
            }
        }
        st32(cb + 16, heap);
        call(g::Function_viw_swap, {cb, self->scrollView + SCROLL_ON_TAP});
        functionDestroy(cb);
        call(g::IntVector_base_dtor, {second + 0xC});
        if (ld32(second + 8)) call(fn::shared_release, {ld32(second + 8)});
        call(g::IntVector_base_dtor, {first + 0xC});
        if (ld32(first + 8)) call(fn::shared_release, {ld32(first + 8)});
    }
    // onEnter / onExit
    {
        Frame h;
        uint32_t cb = newFunction(h, VT_OnEnter, {me});
        call(g::Function_vv_assign, {me + 0x210, cb});
        functionDestroy(cb);
    }
    {
        Frame h;
        uint32_t cb = newFunction(h, VT_OnExit, {me});
        call(g::Function_vv_assign, {me + 0x228, cb});
        functionDestroy(cb);
    }
    if (self->boardType == TYPE_NORMAL) {
        Frame h;
        const uint32_t state = h.alloc(8);
        call(g::AppState_getAvatarBoardFilterState, {state, call(fn::AppState_getInstance, {})});
        setTextId(self->filterText, ld8(ld32(state)) ? TXT_FILTER_ON : TXT_FILTER_OFF);
        sharedRelease(state);
    }
    const uint32_t left = findNode(self->root, S_LeftUIArrow, 12);
    const uint32_t right = findNode(self->root, S_RightUI, 7);
    self->leftArrowButton = arrowButton(self, left, VT_OnTapLeftArrow);
    self->rightArrowButton = arrowButton(self, right, VT_OnTapRightArrow);
    call(g::IntSet_destroy, {ids, ld32(ids + 4)});
    call(g::IntVector_base_dtor, {newIds});
}
RW_FUNCTION(0x6B72C8, "SceneSphereBoardSelect::initSphereBoardList", SceneSphereBoardSelect::initSphereBoardList);

// ------------------------------------------------------------------------------------------------ per frame
// When the centred board changes: play the select sound, remember it (AppState, by board type), count the
// incentives on its squares and those the user opened, and show the board's name. Between two boards the counters
// read 0 and the name is empty.
void Self::update(Addr me, float) {
    Self *self = rw::host_ptr<Self>(me);
    const int32_t item = calc_index(me);
    if (item < 0) {
        if (item == -100 || self->selectedId == item) return;
        self->selectedId = item;
        setNumber(self->totalLabel, 0);
        setNumber(self->numLabel, 0);
        Frame f;
        setTextC(self->boardNameLabel, stringBuffer(f, {S_Empty}));
        return;
    }
    const uint32_t listed = ld32(ld32(self->scrollView + SCROLL_SPHERES) + 8 * uint32_t(item));
    const int32_t id = safe32(listed, 0);
    if (self->selectedId == id) return;
    {
        Frame f;
        const uint32_t layer = f.alloc(8);
        call(g::audio_Manager_getLayer, {layer, call(g::audio_Manager_getInstance, {}), 0});
        call(g::audio_Layer_playSE, {ld32(layer), SE_SELECT, 0, 0});
        sharedRelease(layer);
    }
    const uint32_t app = call(fn::AppState_getInstance, {});
    switch (self->boardType) {
    case TYPE_COLOSSEUM: call(g::AppState_setAvatarBoardPosColosseum, {app, uint32_t(id)}); break;
    case TYPE_STAGE: call(g::AppState_setAvatarBoardPosStage, {app, uint32_t(id)}); break;
    case TYPE_RAID: call(g::AppState_setAvatarBoardPosRaid, {app, uint32_t(id)}); break;
    default: call(g::AppState_setAvatarBoardPosNormal, {app, uint32_t(id)}); break;
    }
    self->selectedId = id;
    Frame f;
    const uint32_t sphere = masterRow(f, g::master_Sphere_create, uint32_t(id));
    const uint32_t array = masterRow(f, g::master_SphereArray_create, uint32_t(safe32(ld32(sphere), 0x90)));
    const uint32_t masu = masterRow(f, g::master_SphereMasu_create, uint32_t(safe32(ld32(sphere), 0x94)));
    for (uint32_t k = 0; k < 9; k++) {
        self->ownedIncentiveCount[k] = 0;
        self->incentiveCount[k] = 0;
    }
    // the incentives of the board's squares, five SphereArray lists of nine
    for (uint32_t k = 0; k < 9; k++) {
        for (uint32_t list : {0x94u, 0x70u, 0x4Cu, 0x28u, 0x04u}) {
            Frame h;
            const uint32_t copy = sharedCopy(h, masu);
            sum_Incentive(me, copy, safe32(ld32(array), list + 4 * k));
            sharedRelease(copy);
        }
    }
    int32_t total = 0;
    for (uint32_t k = 0; k < 9; k++) total += self->incentiveCount[k];
    setNumber(self->totalLabel, total);
    // the squares the user opened
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_USER_SPHERES + 4);
    const uint32_t map = ld32(api + API_USER_SPHERES);
    if (ctrl) call(fn::shared_add, {ctrl});
    if (map) {
        const uint32_t key = f.alloc(4);
        st32(key, uint32_t(id));
        const uint32_t it = call(g::UserSphereMap_find, {map, key});
        if (it != map + 4) {
            Frame h;
            const uint32_t opened = h.alloc(12);
            call(g::IntVector_copy, {opened, it + USER_SPHERE_OPENED});
            const uint32_t count = (ld32(opened + 4) - ld32(opened)) >> 2;
            for (uint32_t k = 0; k < count; k++) {
                const uint32_t square = ld32(ld32(opened) + 4 * k) - 1;
                if (square > 0x3E5) continue;
                // the original reads the 23 kinds / parameters into stack arrays and indexes them up to 997: past
                // square 23 it reads other stack words; the user data never holds such squares
                if (square >= MASU_SQUARES) continue;
                const uint32_t kind = uint32_t(safe32(ld32(masu), 0x64 + 4 * square));
                const int32_t param = safe32(ld32(masu), 0xC0 + 4 * square);
                const int32_t type = kind > 23 ? -1 : incentiveType(kind, param);
                const uint32_t counter = me + 0x2F8 + 4 * uint32_t(type);   // type -1 bumps incentiveCount[8]
                st32(counter, ld32(counter) + 1);
            }
            call(g::IntVector_base_dtor, {opened});
        }
    }
    int32_t owned = 0;
    for (uint32_t k = 0; k < 9; k++) owned += self->ownedIncentiveCount[k];
    setNumber(self->numLabel, owned);
    {
        Frame h;
        const uint32_t name = safeBytes(h, ld32(sphere), 4, 0x81);
        setTextC(self->boardNameLabel, stringBuffer(h, {name}));
    }
    if (ctrl) call(fn::shared_release, {ctrl});
    sharedRelease(masu);
    sharedRelease(array);
    sharedRelease(sphere);
}
RW_FUNCTION(0x6B9550, "SceneSphereBoardSelect::update", SceneSphereBoardSelect::update);

// ------------------------------------------------------------------------------------------------ back
// backScene $_14: prepare MyPage's requests and change to MyPage (fade).
void Self::backToMyPage(Addr) {
    Frame f;
    const uint32_t sm = call(fn::SceneManager_getInstance, {});
    const uint32_t none = newEmptyFunction(f);
    st8(sm + SM_FLAG_4CC, 1);
    call(g::Function_vb_assign, {sm + SM_CALLBACK_548, none});
    functionDestroy(none);
    const uint32_t done = newEmptyFunction(f);
    call(g::SceneUtil_callPrepareMypageAPIs, {done});
    functionDestroy(done);
    const uint32_t m = call(fn::SceneManager_getInstance, {});
    const uint32_t layer = newFunction(f, ld32(GOT_FUNC_LAYER_FACTORY_VTABLE) + 8, {ld32(GOT_CREATE_MYPAGE)});
    call(g::SceneManager_changeSceneLayer, {m, layer, 1, 1, 1});
    functionDestroy(layer);
}
RW_FUNCTION(0x6BB958, "SceneSphereBoardSelect::backScene $_14 __func::operator()", SceneSphereBoardSelect::backToMyPage);
