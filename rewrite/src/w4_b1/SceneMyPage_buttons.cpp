// SceneMyPage: the left and right menu buttons and the info icons.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t Vec2Vector_pushSlowCopy = 0x64A065;   // vector<Vec2>::__push_back_slow_path(const Vec2&)
constexpr uint32_t VT_left_46 = 0xF61134, VT_left_47 = 0xF610B0, VT_left_48 = 0xF61058;
constexpr uint32_t S_LEFT_UI = 0xDF85F0, S_CENTER_UI = 0xE02FBA, S_AREA_RELIEF = 0xE03991, S_BUTTON_RELIEF = 0xE039A4;
constexpr uint32_t S_TXT = 0xDF94D1, S_AREA_COLOSSEUM = 0xE039B6, S_BUTTON_COLOSSEUM = 0xE039CC;
constexpr uint32_t S_AREA_EV = 0xE039DD, S_BUTTON_EV = 0xE039EC, S_BUTTON_EQUIP = 0xDFF9C0, S_TXT_EQUIP = 0xDFF9CD;
constexpr uint32_t S_BUTTON_BOARD = 0xE039F6, S_TXT_BOARD = 0xE03A05, S_BUTTON_WORLD = 0xE03A11;
constexpr uint32_t S_TXT_WORLD = 0xE03A1E;

// vector<Vec2>::push_back(const Vec2&) as inlined (end == capacity: the slow path; a null end becomes 8)
void pushVec2(uint32_t vec, uint32_t value) {
    uint32_t end = ld32(vec + 4);
    if (end != ld32(vec + 8)) {
        uint32_t e = 0;
        if (end) {
            call(fn::Vec2_ctor_copy, {end, value});
            e = ld32(vec + 4);
        }
        st32(vec + 4, e + 8);
    } else {
        call(Vec2Vector_pushSlowCopy, {vec, value});
    }
}

void shadowLabel(uint32_t button, uint32_t name, uint32_t len, int32_t textId) {
    uint32_t text = findNode(button, name, len);
    call(SceneUtil_setShadow, {text});
    setTextId(text, textId);
}
}  // namespace

// The left column: relief / colosseum / event buttons (hidden, not touchable), equip, board, world
void SceneMyPage_initLeftButtons(uint32_t self) {
    uint32_t left = findNode(ld32(self + 0x290), S_LEFT_UI, 6);
    uint32_t center = findNode(left, S_CENTER_UI, 8);
    {
        float y = bf(vcall(center, node::getPositionY, {center}));
        float m = bf(call(SceneUtil_getSideHeightMargin, {}));
        vcall(center, node::setPositionY, {center, fb(y - m)});
    }
    uint32_t relief = findNode(left, S_AREA_RELIEF, 18);
    uint32_t reliefButton = findNode(relief, S_BUTTON_RELIEF, 17);
    titleLabel(reliefButton, S_TXT, 3, 101000032);
    uint32_t colosseum = findNode(left, S_AREA_COLOSSEUM, 21);
    uint32_t colosseumButton = findNode(colosseum, S_BUTTON_COLOSSEUM, 16);
    titleLabel(colosseumButton, S_TXT, 3, 101000035);
    uint32_t event = findNode(left, S_AREA_EV, 14);
    uint32_t eventButton = findNode(event, S_BUTTON_EV, 9);
    titleLabel(eventButton, S_TXT, 3, 101000034);
    // (their positions are collected into a local vector that is not used)
    Frame f;
    uint32_t positions = f.alloc(12);
    st32(positions, 0);
    st32(positions + 4, 0);
    st32(positions + 8, 0);
    pushVec2(positions, vcall(event, node::getPosition, {event}));
    pushVec2(positions, vcall(colosseum, node::getPosition, {colosseum}));
    pushVec2(positions, vcall(relief, node::getPosition, {relief}));
    vcall(event, node::setVisible, {event, 0});
    vcall(eventButton, 0x29C, {eventButton, 0});            // setTouchEnabled
    vcall(colosseum, node::setVisible, {colosseum, 0});
    vcall(colosseumButton, 0x29C, {colosseumButton, 0});
    vcall(relief, node::setVisible, {relief, 0});
    vcall(reliefButton, 0x29C, {reliefButton, 0});

    uint32_t equip = findNode(left, S_BUTTON_EQUIP, 12);
    addTouchLambda(equip, VT_left_46, {self});
    shadowLabel(equip, S_TXT_EQUIP, 9, 100100006);
    uint32_t board = findNode(left, S_BUTTON_BOARD, 14);
    addTouchLambda(board, VT_left_47, {self});
    shadowLabel(board, S_TXT_BOARD, 11, 100100038);
    uint32_t world = findNode(left, S_BUTTON_WORLD, 12);
    addTouchLambda(world, VT_left_48, {self});
    shadowLabel(world, S_TXT_WORLD, 9, 100100008);
    call(Vec2Vector_baseDtor, {positions});
}

RW_FUNCTION(0x63CA18, "SceneMyPage::initLeftButtons", SceneMyPage_initLeftButtons);

// ------------------------------------------------------------------------------------------------ right side
namespace {
constexpr uint32_t VT_right_49 = 0xF61000, VT_right_50 = 0xF60FA8, VT_right_51 = 0xF60F50, VT_right_52 = 0xF60DF0;
constexpr uint32_t VT_right_53 = 0xF60DC4;
constexpr uint32_t S_RIGHT_UI = 0xDF85F7, S_BUTTON_PRESENT = 0xE03A28, S_TXT_BOX = 0xE03A3A, S_BUTTON_SHOP = 0xE03A42;
constexpr uint32_t S_TXT_SHOP = 0xE03A4E, S_BUTTON_INVITE = 0xE03A57, S_MY_SWITCH = 0xE03A65;
constexpr uint32_t S_FACEBOOK_BUTTON = 0xE005A0, S_TXT_FACEBOOK = 0xE00808, S_SWITCH_SOLO = 0xE03A6F;
constexpr uint32_t S_SWITCH_PARTY = 0xE03A7B, S_SWITCH_FACEBOOK = 0xE03A88;
constexpr uint32_t UseHash_find = 0x47ABFB;               // __hash_table<...>::find(const key&)
constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_isLoggedIn = 0x4E56C1;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
}  // namespace

// The right column: present box, shop, invite, the Facebook button and the solo / party switch
void SceneMyPage_initRightButtons(uint32_t self) {
    uint32_t right = findNode(ld32(self + 0x290), S_RIGHT_UI, 7);
    uint32_t center = findNode(right, S_CENTER_UI, 8);
    {
        float y = bf(vcall(center, node::getPositionY, {center}));
        float m = bf(call(SceneUtil_getSideHeightMargin, {}));
        vcall(center, node::setPositionY, {center, fb(y - m)});
    }
    uint32_t present = findNode(right, S_BUTTON_PRESENT, 17);
    addTouchLambda(present, VT_right_49, {self});
    titleLabel(present, S_TXT_BOX, 7, 101000028);
    uint32_t shop = findNode(right, S_BUTTON_SHOP, 11);
    addTouchLambda(shop, VT_right_50, {self});
    shadowLabel(shop, S_TXT_SHOP, 8, 100100007);
    uint32_t invite = findNode(right, S_BUTTON_INVITE, 13);
    addTouchLambda(invite, VT_right_51, {self});
    uint32_t mySwitch = findNode(right, S_MY_SWITCH, 9);
    uint32_t facebook = findNode(right, S_FACEBOOK_BUTTON, 14);
    vcall(facebook, node::setVisible, {facebook, 0});
    titleLabel(facebook, S_TXT_FACEBOOK, 12, 101000038);

    // the Facebook link button: shown when the user may link and is not linked / logged in
    uint32_t linked = 0;
    uint32_t uses = ld32(call(APIManager_getInstance, {}) + 0x334);
    if (uses) {
        Frame f;
        uint32_t key = f.alloc(4);
        st8(key + 3, 1);
        linked = call(UseHash_find, {uses, key + 3}) ? 1 : 0;
    }
    uint32_t user = ld32(call(APIManager_getInstance, {}) + 0x44);
    if (user && (ld8(user + 0x35) & 2) &&
        !(linked == 1 && call(FacebookManager_isLoggedIn, {call(FacebookManager_getInstance, {})}))) {
        vcall(facebook, node::setVisible, {facebook, 1});
        addTouchLambda(facebook, VT_right_52, {self, facebook});
    }
    if (call(SceneUtil_isLoggedInFacebook, {}) == 1) {
        vcall(mySwitch, node::setVisible, {mySwitch, 1});
        vcall(invite, node::setVisible, {invite, 1});
        Frame f;
        uint32_t empty = newStr(f, S_EMPTY, 0);
        uint32_t button = call(ImageButton_create, {empty, 1});
        strFree(empty);
        call(ImageButton_setTargetView, {button, mySwitch, fb(1.0f)});
        vcall(mySwitch, node::addChild, {mySwitch, button});
        uint32_t solo = findNode(mySwitch, S_SWITCH_SOLO, 11);
        uint32_t party = findNode(mySwitch, S_SWITCH_PARTY, 12);
        uint32_t fbSwitch = findNode(mySwitch, S_SWITCH_FACEBOOK, 15);
        uint32_t shown;
        if (ld8(self + OFF_HAS_PARTY)) {
            vcall(solo, node::setVisible, {solo, 0});
            vcall(party, node::setVisible, {party, 1});
            shown = party;
        } else {
            vcall(solo, node::setVisible, {solo, 1});
            vcall(party, node::setVisible, {party, 0});
            shown = solo;
        }
        uint32_t fo = newFunction(f, VT_right_53, {self, shown, fbSwitch});
        call(ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        vcall(shown, node::setLocalZOrder, {shown, 2});
        vcall(fbSwitch, node::setLocalZOrder, {fbSwitch, 1});
    } else {
        vcall(invite, node::setVisible, {invite, 0});
        vcall(mySwitch, node::setVisible, {mySwitch, 0});
    }
}

RW_FUNCTION(0x63D338, "SceneMyPage::initRightButtons", SceneMyPage_initRightButtons);

// ------------------------------------------------------------------------------------------------ info icons
namespace {
constexpr uint32_t S_BALOON_EQUIP = 0xE0388B, S_BALOON_BOARD = 0xE03898, S_BALOON_SHOP = 0xE038A7;
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_getNewAvatarBoardIDs = 0x4EBE3B;
constexpr uint32_t IntVector_copy = 0x4AF707;
constexpr uint32_t Sphere_create = 0x78CF21;              // master::Sphere::create(shared_ptr out, id)
constexpr uint32_t M_updatePresentBoxIcon = 0x6462F9;
}  // namespace

// "new" balloons: equip (a notice of types 11..16), board (a new board sphere of kind 1 / 2), shop (off)
void SceneMyPage_updateInfoIcon(uint32_t self) {
    uint32_t left = findNode(ld32(self + 0x290), S_LEFT_UI, 6);
    call(APIManager_getInstance, {});
    uint32_t api = call(APIManager_getInstance, {});
    uint32_t b = ld32(api + 0x2FC), e = ld32(api + 0x300);
    uint32_t equipNew = 0;
    if (e != b) {
        uint32_t n = uint32_t(int32_t(e - b) >> 2);
        for (uint32_t i = 0; i < n; i++) equipNew |= uint32_t(ld32(b + i * 4) - 11) < 6 ? 1 : 0;
    }
    uint32_t equip = findNode(left, S_BALOON_EQUIP, 12);
    vcall(equip, node::setVisible, {equip, equipNew & 1});

    Frame f;
    uint32_t ids = f.alloc(12);
    call(IntVector_copy, {ids, call(AppState_getNewAvatarBoardIDs, {call(AppState_getInstance, {})})});
    uint32_t boardNew = 0;
    if (ld32(ids + 4) != ld32(ids)) {
        for (uint32_t i = 0; i < uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2); i++) {
            Frame g;
            uint32_t sphere = g.alloc(8);
            call(Sphere_create, {sphere, ld32(ld32(ids) + i * 4)});
            if (ld32(sphere)) {
                int32_t kind = scene::safe32(ld32(sphere), 0x98);
                if (uint32_t(kind - 1) < 2) boardNew = 1;
            }
            release(ld32(sphere + 4));
        }
    }
    uint32_t board = findNode(left, S_BALOON_BOARD, 14);
    vcall(board, node::setVisible, {board, boardNew & 1});
    uint32_t right = findNode(ld32(self + 0x290), S_RIGHT_UI, 7);
    uint32_t shop = findNode(right, S_BALOON_SHOP, 11);
    vcall(shop, node::setVisible, {shop, 0});
    call(M_updatePresentBoxIcon, {self, 0});
    call(IntVector_baseDtor, {ids});
}

RW_FUNCTION(0x63DB18, "SceneMyPage::updateInfoIcon", SceneMyPage_updateInfoIcon);
