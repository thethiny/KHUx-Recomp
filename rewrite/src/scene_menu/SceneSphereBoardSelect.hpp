// SceneSphereBoardSelect: the sphere board (avatar board) selection screen. A horizontally scrolling list of the
// boards of the current board type (normal / colosseum / stage / raid coin boards), the incentive totals of the
// board in the centre, filter / back buttons and the "buy this board" dialog. Selecting an owned board opens it
// in SceneSphereBoard.
//
// The scene is a cocos2d::Layer with a second base, IInOutAnimation, at +0x288 (runInAnimation /
// runOutAnimationMain are reached through its thunks). All guest objects are 32-bit guest addresses.
//
// The header also holds the std::function __func thunk templates shared by SceneSphereBoardSelect and
// SceneSphereBoard (namespace sphere_board): every lambda the two scenes hand to cocos2d / SceneManager becomes a
// libc++ __func whose virtual slots (deleting dtor, clone, clone(dst), destroy, destroy_deallocate, operator(),
// target, target_type) are rewritten here.
#pragma once

#include "core/Guest.hpp"
#include "scene/SceneUi.hpp"

struct SceneSphereBoardSelect {
    const uint8_t layer[0x288];                     // cocos2d::Layer (Node std::function members at +0x210 / +0x228)
    rw::Field<uint32_t> inOutVtable;          // +0x288 IInOutAnimation base
    const uint8_t pad_28C[4];
    rw::Field<uint32_t> numLabel;             // +0x290 Txt_Open/Txt_Num_Label: incentives owned on the board
    rw::Field<uint32_t> totalLabel;           // +0x294 Txt_Open/Txt_Total_Label: incentives on the board
    rw::Field<uint32_t> boardNameLabel;       // +0x298 Txt_SphereBoard_Label
    rw::Field<uint32_t> coinLabel;            // +0x29C Txt_Coin_Label: coins of the board type
    rw::Field<uint32_t> filterNumLabel;       // +0x2A0 Bar_Filter/Txt_Num_Label: boards shown
    rw::Field<uint32_t> filterTotalLabel;     // +0x2A4 Bar_Filter/Txt_Total_Label: shown + filtered out
    rw::Field<uint32_t> filterText;           // +0x2A8 Button_Filter/Txt_Filter
    rw::Field<int32_t> avatarBoardPos;        // +0x2AC board id the list starts on (AppState)
    rw::Field<int32_t> boardType;             // +0x2B0 1 normal, 3 colosseum, 4 stage, 5 raid
    rw::Field<uint32_t> scrollView;           // +0x2B4 SphereScrollView*
    rw::Field<uint32_t> root;                 // +0x2B8 the cocostudio scene node
    guest::Vector spheres;                    // +0x2BC vector<shared_ptr<const master::Sphere>> shown in the list
    rw::Field<uint32_t> boardIdsBegin;        // +0x2C8 std::set<int> of all board ids: begin node
    rw::Field<uint32_t> boardIdsRoot;         // +0x2CC root (its address is the end node)
    rw::Field<uint32_t> boardIdsSize;         // +0x2D0
    rw::Field<int32_t> incentiveCount[9];     // +0x2D4 incentives on the centred board per get_type kind
    rw::Field<int32_t> ownedIncentiveCount[9];// +0x2F8 incentives already opened per kind
    rw::Field<int32_t> selectedId;            // +0x31C board id in the centre (-1 none, -100 between boards)
    rw::Field<uint8_t> flag320;               // +0x320
    rw::Field<uint8_t> flag321;               // +0x321
    rw::Field<uint8_t> changingScene;         // +0x322 a board was chosen: taps are ignored
    const uint8_t pad_323[1];
    rw::Field<uint32_t> field_324;            // +0x324
    rw::Field<uint32_t> leftArrowButton;      // +0x328 ImageButton over LeftUI_Arrow
    rw::Field<uint32_t> rightArrowButton;     // +0x32C ImageButton over RightUI
    rw::Field<uint8_t> scrolling;             // +0x330 the list is moving (no taps on boards)
    rw::Field<uint8_t> canBack;               // +0x331 the back button / key works
    const uint8_t pad_332[6];

    static constexpr uint32_t SIZE = 0x338;
    static constexpr uint32_t VTABLE_START = 0xF9E2F0;           // _ZTV22SceneSphereBoardSelect
    static constexpr uint32_t VTABLE = VTABLE_START + 8;
    static constexpr uint32_t INOUT_VTABLE = VTABLE_START + 0x318;
    static constexpr uint32_t INOUT_OFFSET = 0x288;               // IInOutAnimation base (thunks: this - 0x288)

    using Addr = uint32_t;

    static Addr create();
    static Addr createScene();
    static Addr destroy(Addr self);                     // ~SceneSphereBoardSelect (D2)
    static void destroyThunk(Addr inOut);               // non-virtual thunk from the IInOutAnimation base (D1)
    static void destroyDelete(Addr self);               // deleting destructor (D0)
    static void destroyDeleteThunk(Addr inOut);
    static uint32_t init(Addr self);
    static void initSphereBoardList(Addr self);
    static void changeSceneNext(Addr self, int32_t boardId);
    static int32_t calc_index(Addr self);
    // 1: the board is hidden by the avatar board filter (sphere: shared_ptr<master::Sphere>*, userSphere:
    // UserSphere::Sphere* - both passed by value, owned by the caller)
    static uint32_t avaterBoardDispCheck(Addr self, Addr sphere, Addr userSphere);
    static void setSphereBoardList(Addr self);
    static int32_t get_type(Addr self, int32_t kind, int32_t param);
    static int32_t getAvatarBoardGender(Addr self, int32_t masuId);
    static uint32_t getAvatarBoardSpaceCheck(Addr self, Addr userSphere, int32_t masuId, int32_t kind);
    static void sum_Incentive(Addr self, Addr masu, int32_t index);
    static void update(Addr self, float dt);
    static void runInAnimation(Addr self);
    static void runInAnimationThunk(Addr inOut);
    static void runOutAnimationMain(Addr self, Addr done);
    static void runOutAnimationMainThunk(Addr inOut, Addr done);
    static void openBoardBuyDialog(Addr self, Addr callback, int32_t boardId, int32_t price);
    static int32_t getSphereBoardValidCnt(Addr self, int32_t type);
    static void backScene(Addr self);

    // lambda bodies (std::function operator())
    static void onBackKey(Addr fn);                                   // init $_0
    static void onTapBack(Addr fn, Addr ref, Addr type);              // init $_1
    static void onTapFilter(Addr fn, Addr ref, Addr type);            // init $_2
    static void onFilterClosed(Addr fn);                              // init $_2 inner
    static void openNextScene(Addr fn);                               // changeSceneNext $_3
    static Addr createNextLayer(Addr fn);                             // changeSceneNext $_3 inner
    static void onScrollStop(Addr fn, Addr index);                    // initSphereBoardList $_4
    static void onScrollMove(Addr fn, Addr dt);                       // initSphereBoardList $_5
    static void onTapBoard(Addr fn, Addr index, Addr widget);         // initSphereBoardList $_6
    static void onBuyResult(Addr fn, Addr ok, Addr jewel);            // initSphereBoardList $_6 inner
    static void onEnter(Addr fn);                                     // initSphereBoardList $_7
    static void onExit(Addr fn);                                      // initSphereBoardList $_8
    static void onTapRightArrow(Addr fn, Addr ref, Addr type);        // initSphereBoardList $_9
    static void onTapLeftArrow(Addr fn, Addr ref, Addr type);         // initSphereBoardList $_10
    static void runOutDone(Addr fn);                                  // runOutAnimationMain $_11
    static void onBuyClosed(Addr fn, Addr ok);                        // openBoardBuyDialog $_13 inner
    static void onBuyCancel(Addr fn, Addr ok);                        // openBoardBuyDialog $_12 inner
    static void removePopup(Addr fn);                                 // ... cancel's close animation end
    static uint32_t isPopupTouchEnabled(Addr fn);                     // ... popup back-key check
    static void onTapBuyClose(Addr fn, Addr ref, Addr type);          // ... Button_Close
    static void onTapBuyOk(Addr fn, Addr ref, Addr type);             // ... Button_OK
    static void onBuyApiDone(Addr fn, Addr ok);                       // ... executeUserSphereBuy result
    static void onBuyErrorClosed(Addr fn, Addr ok);                   // ... error dialog closed
    static void backToMyPage(Addr fn);                                // backScene $_14
    static void backToAdventure(Addr fn);                             // backScene $_15
};
RW_LAYOUT(SceneSphereBoardSelect, inOutVtable, 0x288);
RW_LAYOUT(SceneSphereBoardSelect, numLabel, 0x290);
RW_LAYOUT(SceneSphereBoardSelect, avatarBoardPos, 0x2AC);
RW_LAYOUT(SceneSphereBoardSelect, spheres, 0x2BC);
RW_LAYOUT(SceneSphereBoardSelect, boardIdsBegin, 0x2C8);
RW_LAYOUT(SceneSphereBoardSelect, incentiveCount, 0x2D4);
RW_LAYOUT(SceneSphereBoardSelect, ownedIncentiveCount, 0x2F8);
RW_LAYOUT(SceneSphereBoardSelect, selectedId, 0x31C);
RW_LAYOUT(SceneSphereBoardSelect, changingScene, 0x322);
RW_LAYOUT(SceneSphereBoardSelect, leftArrowButton, 0x328);
RW_LAYOUT(SceneSphereBoardSelect, scrolling, 0x330);
RW_LAYOUT(SceneSphereBoardSelect, canBack, 0x331);
RW_SIZE(SceneSphereBoardSelect, SceneSphereBoardSelect::SIZE);

// ------------------------------------------------------------------------------------------------ __func thunks
namespace sphere_board {

using scene::ld32;
using scene::st32;

// The guest libc++ std::function<..>::operator() / copy for the signatures the scenes use.
constexpr uint32_t FUNCTION_VBB_CALL = 0x48B159;   // std::function<void(bool,bool)>::operator()(bool, bool)
constexpr uint32_t FUNCTION_VBB_COPY = 0x45CD55;   // std::function<void(bool,bool)>::function(const function&)

// libc++ std::function destructor, inline: destroy() when stored in place, destroy_deallocate() on the heap.
inline void destroyFunction(uint32_t f) { scene::functionDestroy(f); }

// __func<lambda> whose captures are SIZE - 4 bytes of plain words copied by clone; the ~__func is trivial.
// NAME: the type name pointer target() compares, TI: the typeinfo target_type() returns.
template <uint32_t VT, uint32_t SIZE, uint32_t WORDS, uint32_t NAME, uint32_t TI>
struct PodFunc {
    // deleting ~__func and destroy_deallocate(): nothing to destroy, free the heap object
    static void deleteThis(uint32_t self) { guest::deleteObject(self); }
    // __clone(): heap copy
    static uint32_t clone(uint32_t self) {
        uint32_t p = guest::newObject(SIZE);
        if (p) {
            st32(p, VT);
            for (uint32_t k = 0; k < WORDS; k++) st32(p + 4 + 4 * k, ld32(self + 4 + 4 * k));
        }
        return p;
    }
    // __clone(dst): placement copy
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        st32(dst, VT);
        for (uint32_t k = 0; k < WORDS; k++) st32(dst + 4 + 4 * k, ld32(self + 4 + 4 * k));
    }
    // destroy(): the lambda has a trivial destructor
    static void destroy(uint32_t) {}
    // target(type_info): the stored lambda when the type matches (by name pointer)
    static uint32_t target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == NAME ? self + 4 : 0; }
    static uint32_t targetType() { return TI; }
};

// __func<lambda> holding one std::function (at +8, 0x20 bytes in all; COPY = its copy constructor):
// runOutAnimationMain's completion lambdas (std::function<void()>) and the buy dialog's result forwarder
// (std::function<void(bool,bool)>).
template <uint32_t VT, uint32_t COPY, uint32_t NAME, uint32_t TI>
struct FnFunc {
    static constexpr uint32_t FN = 8;
    static uint32_t dtor(uint32_t self) {
        st32(self, VT);
        destroyFunction(self + FN);
        return self;
    }
    static void deleteThis(uint32_t self) {
        dtor(self);
        guest::deleteObject(self);
    }
    static uint32_t clone(uint32_t self) {
        uint32_t p = guest::newObject(0x20);
        if (p) {
            st32(p, VT);
            scene::call(COPY, {p + FN, self + FN});
        }
        return p;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        st32(dst, VT);
        scene::call(COPY, {dst + FN, self + FN});
    }
    static void destroy(uint32_t self) { destroyFunction(self + FN); }
    static void destroyDeallocate(uint32_t self) {
        destroyFunction(self + FN);
        guest::deleteObject(self);
    }
    // operator() of a completion lambda: callback() when it is set
    static void callVoid(uint32_t self) {
        if (ld32(self + FN + 16)) scene::call(scene::fn::function_vv_call, {self + FN});
    }
    static uint32_t target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == NAME ? self + FN : 0; }
    static uint32_t targetType() { return TI; }
};

// __func<lambda> capturing PRE plain words (at +8), a std::function<void(bool,bool)> (at +0x18) and POST plain
// words after it (at +0x30): the buy dialog's lambdas. SIZE = total bytes.
template <uint32_t VT, uint32_t SIZE, uint32_t PRE, uint32_t POST, uint32_t NAME, uint32_t TI>
struct BoolFnFunc {
    static constexpr uint32_t FN = 0x18;
    static uint32_t dtor(uint32_t self) {
        st32(self, VT);
        destroyFunction(self + FN);
        return self;
    }
    static void deleteThis(uint32_t self) {
        dtor(self);
        guest::deleteObject(self);
    }
    static void copyInto(uint32_t self, uint32_t dst) {
        st32(dst, VT);
        for (uint32_t k = 0; k < PRE; k++) st32(dst + 8 + 4 * k, ld32(self + 8 + 4 * k));
        scene::call(FUNCTION_VBB_COPY, {dst + FN, self + FN});
        for (uint32_t k = 0; k < POST; k++) st32(dst + 0x30 + 4 * k, ld32(self + 0x30 + 4 * k));
    }
    static uint32_t clone(uint32_t self) {
        uint32_t p = guest::newObject(SIZE);
        if (p) copyInto(self, p);
        return p;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (dst) copyInto(self, dst);
    }
    static void destroy(uint32_t self) { destroyFunction(self + FN); }
    static void destroyDeallocate(uint32_t self) {
        destroyFunction(self + FN);
        guest::deleteObject(self);
    }
    static uint32_t target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == NAME ? self + 8 : 0; }
    static uint32_t targetType() { return TI; }
};

}  // namespace sphere_board
