// The login bonus (SceneLoginBonus, shown after login once a server day): create, destructor, init, the in / out
// animations of the bonus "memory" pages and the stamp board, skipEvent, openLoginBonus (the day's stamp: an LWF
// stamp on the continuous or the total panel, with its sound), the total-bonus stamp helper (sub_6FA580),
// make_getIncentive_id, loadIncentiveTexture and the two incentive name builders; initLoginBonus is in w5_20.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t Director_getInstance = 0x8EE519, Director_getTextureCache = 0x8EFE69;
constexpr uint32_t TextureCache_addImage = 0x917DA9;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t StringAssign = 0x48D811;                // std::string::assign(const char*, size_t)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;         // (const char* path, void*)
constexpr uint32_t LWFMovie_GotoAndPlay = 0x815DCF;
constexpr uint32_t AudioManager_getInstance = 0x5BECFD, AudioManager_getLayer = 0x5BF33D;   // (out, mgr, int)
constexpr uint32_t AudioLayer_playSE = 0x5BF785;           // (layer, ResourceID, int, int)
constexpr uint32_t VectorLoginBonus_dtor = 0x6F6429;
constexpr uint32_t Medal_create = 0x785871, Material_create = 0x7852E5, Keyblade_create = 0x783F9D;
constexpr uint32_t AvatarParts_create = 0x778DE1;
constexpr uint32_t SceneUtil_createLeftIn = 0x45B5C5, SceneUtil_createRightOut = 0x45BA3D;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t LoginBonus_init = 0x6F649D, LoginBonus_initMemoryBonus = 0x6F87E1, LoginBonus_stampTotal = 0x6FA581;
constexpr uint32_t GOT_VT_LoginBonus = 0xFD6958;
constexpr uint32_t Layer_init = 0x88A8A1;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t SM_hideHeader = 0x61154D, SM_removeBackground = 0x611691;
constexpr uint32_t VectorInt_copy = 0x4AF707, VectorInt_dtor = 0x478961;
constexpr uint32_t LoginBonus_create = 0x784A79, LoginBonus_getLoginBonusIds = 0x784A5D;
constexpr uint32_t VectorLoginBonus_pushSlowConst = 0x6FB495, VectorLoginBonus_pushSlowMove = 0x6FB315;
constexpr uint32_t TreeInt_destroy = 0x48D9B5;
constexpr uint32_t AudioManager_load = 0x5BEEF5;
constexpr uint32_t LoginBonus_initLoginBonus = 0x6F710D;
}  // namespace g

constexpr uint32_t SIZE = 0x2E0;
constexpr uint32_t F_INOUT = 0x288, F_ROOT = 0x28C;
constexpr uint32_t F_BONUS_A = 0x290, F_BONUS_B = 0x29C, F_BONUS_C = 0x2A8, F_BONUS_D = 0x2B4;   // vector<shared_ptr<LoginBonus>>
constexpr uint32_t F_MEMORY_SHOWN = 0x2C0, F_MEMORY_COUNT = 0x2C4;   // the welcome "memory" pages
constexpr uint32_t F_TOTAL_DAYS = 0x2C8, F_CONTINUOUS_DAYS = 0x2CC;
constexpr uint32_t F_FLAG_2D8 = 0x2D8, F_CAN_SKIP = 0x2D9, F_TAP_ANIMATION = 0x2DC;
constexpr uint32_t VT_Skipped = 0xF69F64;      // skipEvent's run-out {this}
constexpr uint32_t VT_CenterIn = 0xF69FE8, VT_MemoryIn = 0xF6A014, VT_MemoryOut = 0xF69FBC;   // {this}
constexpr uint32_t VT_RunOutDone = 0xF69F90;   // {std::function<void()>} (heap 0x20)
constexpr uint32_t VT_StampDone = 0xF69F0C;    // openLoginBonus {area point, total panel, this}
constexpr uint32_t VT_TotalStampDone = 0xF6A0C4;   // sub_6FA580 {this}
constexpr uint32_t S_CenterUI = 0xE02FBA, S_BonusMemory = 0xE06326, S_Total = 0xE0646E, S_PanelTotal = 0xE06474;
constexpr uint32_t S_AreaPoint = 0xDF9FB4, S_PanelCont = 0xE06408, S_FmtPanelCont = 0xE06413;
constexpr uint32_t S_StampLwf = 0xE06420;      // "lwf/loginbonus/LoginBonus_Stamp/LoginBonus_Stamp.lwf"
constexpr uint32_t S_IncentiveIcon = 0xE064CB; // "img/incentive/IncentiveIcon_%02d.png"
constexpr uint32_t SE_STAMP = 509100630;
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228;
constexpr uint32_t F_TOTAL_NEXT = 0x2D0, F_TOTAL_NEXT_DAY = 0x2D4;
constexpr uint32_t VT_OnEnter = 0xF6A098, VT_OnExit = 0xF6A06C, VT_CenterButton = 0xF6A040;   // {this}
constexpr uint32_t S_SceneJson = 0xE062AD;     // "cocostudio/publish/LBonusScene.json"
constexpr uint32_t S_BG = 0xE04BF3, S_BgLwf = 0xE062D1, S_TapJson = 0xDFFCF2, S_TapAnimation = 0xDFFB2B;
constexpr uint32_t S_MemoryJson = 0xE06300, S_Mog = 0xE06332, S_WelcomeLwf = 0xE06336, S_ShopBuyBut = 0xE00327;
constexpr uint32_t S_CenterButton = 0xDFFB40;
constexpr uint32_t API_LOGIN_BONUS = 0x6C;     // shared_ptr<response>: continue id, -, total id, total days,
                                               // vector<int> specials (+0x10)
constexpr uint32_t ROW_TYPE = 4, ROW_DAY = 0x90C;
constexpr uint32_t V_runInAnimation = 0x300, V_getLocalZOrder = 0x1C, V_setPositionXY = 0x5C, V_slot14 = 0x14;
constexpr uint32_t V_deleting = 0x04, V_isVisible = 0xB4;

void runEased(uint32_t make, uint32_t node, uint32_t done) {
    const uint32_t action = call(make, {node, 0, 0, done});
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
// the stamp LWF's play time (its frame count / 60 s, at least 0.1 s), then `done` (a CallFunc)
void playStamp(uint32_t lwf, uint32_t done) {
    const uint32_t movie = ld32(ld32(lwf + 0x3E0) + 0x18);
    call(g::LWFMovie_GotoAndPlay, {movie, ld32(movie + 0x98)});
    const uint32_t frames = ld32(ld32(ld32(lwf + 0x3E0) + 0x18) + 0x98);
    const float seconds = float(int32_t(frames)) / 60.0f;
    const float delay = seconds < 0.1f ? 0.1f : seconds;
    const uint32_t wait = call(g::DelayTime_create, {fbits(delay)});
    const uint32_t then = call(g::CallFunc_create, {done});
    call(g::Node_runAction, {lwf, call(g::Sequence_create, {wait, then, 0})});
}
void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// vector<shared_ptr<const LoginBonus>>: clear (pop back, releasing each), push a copy, push an empty pointer
void clearBonuses(uint32_t vec) {
    for (uint32_t end = ld32(vec + 4), begin = ld32(vec); end != begin;) {
        end -= 8;
        st32(vec + 4, end);
        const uint32_t ctl = ld32(end + 4);
        if (ctl) {
            releaseIf(ctl);
            end = ld32(vec + 4);
        }
    }
}
void pushBonus(uint32_t vec, uint32_t sp) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(g::VectorLoginBonus_pushSlowConst, {vec, sp});
        return;
    }
    if (end) {
        st32(end, ld32(sp));
        st32(end + 4, ld32(sp + 4));
        if (ld32(sp + 4)) call(fn::shared_add, {ld32(sp + 4)});
    }
    st32(vec + 4, ld32(vec + 4) + 8);
}
void pushEmptyBonus(uint32_t vec) {
    const uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t n = 0;
        if (end) {
            st32(end, 0);
            st32(end + 4, 0);
            n = ld32(vec + 4);
        }
        st32(vec + 4, n + 8);
        return;
    }
    Frame f;
    const uint32_t tmp = f.alloc(8);
    st32(tmp, 0);
    st32(tmp + 4, 0);
    call(g::VectorLoginBonus_pushSlowMove, {vec, tmp});
    releaseIf(ld32(tmp + 4));
}
uint32_t rowInt(uint32_t sb, uint32_t offset) {
    Frame f;
    const uint32_t v = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + offset, 4, offset});
    return ld32(v);
}
void playStampSound() {
    const uint32_t mgr = call(g::AudioManager_getInstance, {});
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(g::AudioManager_getLayer, {layer, mgr, 0});
    call(g::AudioLayer_playSE, {ld32(layer), SE_STAMP, 0, 0});
    if (ld32(layer + 4)) call(fn::shared_release, {ld32(layer + 4)});
}
}  // namespace

struct SceneLoginBonus {
    static uint32_t create();
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static void runInAnimation(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void skipEvent(uint32_t self);
    static void openLoginBonus(uint32_t self);
    static void stampTotal(uint32_t closure);
    static void getIncentiveId(uint32_t self, uint32_t type, uint32_t id, uint32_t out);
    static void getIncentiveName(uint32_t out, uint32_t self, uint32_t type, uint32_t id, uint32_t count,
                                 uint32_t idOut);
    static void getIncentiveNextName(uint32_t out, uint32_t self, uint32_t type, uint32_t id, uint32_t a5,
                                     uint32_t a6);
    static uint32_t loadIncentiveTexture(uint32_t self, uint32_t number);
};

uint32_t SceneLoginBonus::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    const uint32_t vt = ld32(g::GOT_VT_LoginBonus);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    st8(me + F_FLAG_2D8, 0);
    st8(me + F_CAN_SKIP, 0);
    st32(me + F_TAP_ANIMATION, 0);
    call(g::aeabi_memset, {me + F_ROOT, 0x34, 0});
    if (call(g::LoginBonus_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t SceneLoginBonus::destruct(uint32_t me) {
    const uint32_t vt = ld32(g::GOT_VT_LoginBonus);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    call(g::VectorLoginBonus_dtor, {me + F_BONUS_D});
    call(g::VectorLoginBonus_dtor, {me + F_BONUS_C});
    call(g::VectorLoginBonus_dtor, {me + F_BONUS_B});
    call(g::VectorLoginBonus_dtor, {me + F_BONUS_A});
    call(g::Layer_dtor, {me});
    return me;
}

// The scene: the LBonusScene layout over its LWF background, the tap prompt armature, the welcome "memory" page,
// the Mog with its welcome LWF and the full-screen tap button; then the bonus rows - the special (welcome) bonuses,
// today's continuous row, the next total bonus to reach - from the login bonus response and the master table.
uint32_t SceneLoginBonus::init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    Frame f;
    for (const auto &[member, vtable] : {std::pair{F_ON_ENTER, VT_OnEnter}, std::pair{F_ON_EXIT, VT_OnExit}}) {
        const uint32_t fo = newFunction(f, vtable, {me});
        call(g::FunctionVV_assign, {me + member, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t m = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        call(g::SM_hideHeader, {m, none});
        functionDestroy(none);
    }
    call(g::SM_removeBackground, {sceneManager()});
    {
        const uint32_t s = newStr(f, S_SceneJson, 0x23);
        st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
        strFree(s);
    }
    vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
    {
        const uint32_t bg = findNode(ld32(me + F_ROOT), S_BG, 2);
        if (bg) vcall(bg, node::setVisible, {bg, 0});
        const uint32_t lwf = call(g::AudioLWFNode_create, {S_BgLwf, 0});
        const uint32_t v = f.alloc(8);
        call(g::Vec2_ctorXY, {v, fbits(0.5f), fbits(0.5f)});
        vcall(lwf, node::setAnchorPoint, {lwf, v});
        call(g::Vec2_dtor, {v});
        call(g::Vec2_ctorXY, {v, fbits(480.0f), fbits(320.0f)});
        vcall(lwf, node::setPosition, {lwf, v});
        call(g::Vec2_dtor, {v});
        vcall(ld32(me + F_ROOT), node::addChildZ, {ld32(me + F_ROOT), lwf, 0});
    }
    {
        const uint32_t mgr = call(g::ArmatureDataManager_getInstance, {});
        const uint32_t json = newStr(f, S_TapJson, 0x32);
        call(g::ArmatureDataManager_addArmatureFileInfo, {mgr, json});
        strFree(json);
        const uint32_t name = newStr(f, S_TapAnimation, 0x14);
        st32(me + F_TAP_ANIMATION, call(g::Armature_create, {name}));
        strFree(name);
        const uint32_t root = ld32(me + F_ROOT), arm = ld32(me + F_TAP_ANIMATION);
        const uint32_t z = vcall(root, V_getLocalZOrder, {root});
        vcall(root, node::addChildZ, {root, arm, z + 10});
        vcall(ld32(me + F_TAP_ANIMATION), V_setPositionXY, {ld32(me + F_TAP_ANIMATION), fbits(480.0f), fbits(30.0f)});
        vcall(ld32(me + F_TAP_ANIMATION), node::setVisible, {ld32(me + F_TAP_ANIMATION), 0});
    }
    {
        const uint32_t center = findNode(ld32(me + F_ROOT), S_CenterUI, 8);
        const uint32_t memory =
            call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_MemoryJson});
        const uint32_t root = ld32(me + F_ROOT);
        const uint32_t z = vcall(center, V_getLocalZOrder, {center});
        const uint32_t name = newStr(f, S_BonusMemory, 0xB);
        vcall(root, node::addChildZName, {root, memory, z, name});
        strFree(name);
        vcall(memory, node::setPosition, {memory, vcall(center, node::getPosition, {center})});
    }
    {
        const uint32_t mog = findNode(ld32(me + F_ROOT), S_Mog, 3);
        vcall(mog, V_slot14, {mog, 2});
        const uint32_t area = findNode(mog, S_AreaPoint, 0xA);
        const uint32_t lwf = call(g::AudioLWFNode_create, {S_WelcomeLwf, 0});
        vcall(area, node::addChild, {area, lwf});
    }
    {
        const uint32_t name = newStr(f, S_ShopBuyBut, 0xB);
        const uint32_t button = call(g::ImageButton_create, {name, 0});
        strFree(name);
        call(g::ImageButton_setTargetView, {button, ld32(me + F_ROOT), fbits(1.0f)});
        const uint32_t root = ld32(me + F_ROOT);
        const uint32_t cname = newStr(f, S_CenterButton, 0xC);
        vcall(root, node::addChildZName, {root, button, 0, cname});
        strFree(cname);
        const uint32_t fo = newFunction(f, VT_CenterButton, {me});
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    call(g::aeabi_memset, {me + F_MEMORY_SHOWN, 0x14, 0});
    st32(me + F_TOTAL_NEXT_DAY, 10000000);
    clearBonuses(me + F_BONUS_A);
    clearBonuses(me + F_BONUS_B);
    clearBonuses(me + F_BONUS_C);
    clearBonuses(me + F_BONUS_D);

    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t respCtl = ld32(api + API_LOGIN_BONUS + 4), resp = ld32(api + API_LOGIN_BONUS);
    if (respCtl) call(fn::shared_add, {respCtl});
    const uint32_t tmp = f.alloc(8);
    if (resp) {
        const uint32_t specials = f.alloc(12);
        call(g::VectorInt_copy, {specials, resp + 0x10});
        for (uint32_t it = ld32(specials); it != ld32(specials + 4); it += 4) {
            call(g::LoginBonus_create, {tmp, ld32(it)});
            pushBonus(me + F_BONUS_D, tmp);
            st32(me + F_MEMORY_COUNT, ld32(me + F_MEMORY_COUNT) + 1);
            releaseIf(ld32(tmp + 4));
        }
        if (ld32(resp)) {
            call(g::LoginBonus_create, {tmp, ld32(resp)});
            st32(me + F_TOTAL_DAYS, rowInt(ld32(tmp), ROW_DAY));
            releaseIf(ld32(tmp + 4));
        }
        const uint32_t totalId = ld32(resp + 8), totalDays = ld32(resp + 0xC);
        if (totalId) {
            call(g::LoginBonus_create, {tmp, totalId});
            pushBonus(me + F_BONUS_B, tmp);
            releaseIf(ld32(tmp + 4));
        } else {
            pushEmptyBonus(me + F_BONUS_B);
        }
        st32(me + F_TOTAL_NEXT, totalDays);
        call(g::VectorInt_dtor, {specials});
    } else {
        pushEmptyBonus(me + F_BONUS_B);
    }
    // the master rows: the continuous days (type 1) and the next total bonus after the user's total (type 2)
    const uint32_t ids = f.alloc(12);
    call(g::LoginBonus_getLoginBonusIds, {ids});
    for (uint32_t node = ld32(ids); node != ids + 4;) {
        call(g::LoginBonus_create, {tmp, ld32(node + 0x10)});
        const uint32_t row = ld32(tmp);
        if (row) {
            if (rowInt(row, ROW_TYPE) == 1) {
                pushBonus(me + F_BONUS_A, tmp);
                st32(me + F_CONTINUOUS_DAYS, ld32(me + F_CONTINUOUS_DAYS) + 1);
            } else if (rowInt(ld32(tmp), ROW_TYPE) == 2) {
                const int32_t reached = int32_t(ld32(me + F_TOTAL_NEXT));
                if (reached < int32_t(rowInt(ld32(tmp), ROW_DAY)) &&
                    int32_t(rowInt(ld32(tmp), ROW_DAY)) < int32_t(ld32(me + F_TOTAL_NEXT_DAY))) {
                    st32(me + F_TOTAL_NEXT_DAY, rowInt(ld32(tmp), ROW_DAY));
                    clearBonuses(me + F_BONUS_C);
                    pushBonus(me + F_BONUS_C, tmp);
                }
            }
        }
        releaseIf(ld32(tmp + 4));
        if (ld32(node + 4)) {
            node = ld32(node + 4);
            while (ld32(node)) node = ld32(node);
        } else {
            uint32_t child;
            do {
                child = node;
                node = ld32(node + 8);
            } while (ld32(node) != child);
        }
    }
    if (ld32(me + F_BONUS_C + 4) == ld32(me + F_BONUS_C)) pushEmptyBonus(me + F_BONUS_C);
    call(g::AudioManager_load, {call(g::AudioManager_getInstance, {}), SE_STAMP});
    call(g::LoginBonus_initLoginBonus, {me});
    vcall(me, V_runInAnimation, {me});
    call(g::TreeInt_destroy, {ids, ld32(ids + 4)});
    releaseIf(respCtl);
    return 1;
}

// The stamp board slides in; while welcome "memory" pages are left, the next page slides in instead (at the
// board's place).
void SceneLoginBonus::runInAnimation(uint32_t me) {
    const uint32_t center = findNode(ld32(me + F_ROOT), S_CenterUI, 8);
    const uint32_t memory = findNode(ld32(me + F_ROOT), S_BonusMemory, 0xB);
    Frame f;
    if (int32_t(ld32(me + F_MEMORY_COUNT)) <= int32_t(ld32(me + F_MEMORY_SHOWN))) {
        vcall(center, node::setVisible, {center, 1});
        vcall(memory, node::setVisible, {memory, 0});
        const uint32_t fo = newFunction(f, VT_CenterIn, {me});
        runEased(g::SceneUtil_createLeftIn, center, fo);
        functionDestroy(fo);
        return;
    }
    vcall(center, node::setVisible, {center, 0});
    vcall(memory, node::setPosition, {memory, vcall(center, node::getPosition, {center})});
    call(g::LoginBonus_initMemoryBonus, {me});
    const uint32_t fo = newFunction(f, VT_MemoryIn, {me});
    runEased(g::SceneUtil_createLeftIn, memory, fo);
    functionDestroy(fo);
}

// The board slides out (then `done`); while memory pages are left, the shown page slides out and the next follows.
void SceneLoginBonus::runOutAnimationMain(uint32_t me, uint32_t done) {
    Frame f;
    if (int32_t(ld32(me + F_MEMORY_COUNT)) <= int32_t(ld32(me + F_MEMORY_SHOWN))) {
        const uint32_t center = findNode(ld32(me + F_ROOT), S_CenterUI, 8);
        const uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        const uint32_t fo = f.alloc(20);
        heapFunction(fo, VT_RunOutDone, 0x20, 8, fn::function_vv_move, copy);
        runEased(g::SceneUtil_createRightOut, center, fo);
        functionDestroy(fo);
        functionDestroy(copy);
        return;
    }
    st32(me + F_MEMORY_SHOWN, ld32(me + F_MEMORY_SHOWN) + 1);
    const uint32_t memory = findNode(ld32(me + F_ROOT), S_BonusMemory, 0xB);
    const uint32_t fo = newFunction(f, VT_MemoryOut, {me});
    runEased(g::SceneUtil_createRightOut, memory, fo);
    functionDestroy(fo);
}

// A tap skips the stamp event: the tap prompt hides and the board runs out.
void SceneLoginBonus::skipEvent(uint32_t me) {
    st8(me + F_CAN_SKIP, 0);
    vcall(ld32(me + F_TAP_ANIMATION), node::setVisible, {ld32(me + F_TAP_ANIMATION), 0});
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_Skipped, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// Today's stamp: on the continuous panel of the day (Panel_Cont / Panel_Cont<n>) when the total day is within the
// continuous run, else on the total panel (sub_6FA580).
void SceneLoginBonus::openLoginBonus(uint32_t me) {
    const uint32_t center = findNode(ld32(me + F_ROOT), S_CenterUI, 8);
    findNode(center, S_Total, 5);
    const uint32_t panelTotal = findNode(center, S_PanelTotal, 0xB);
    const uint32_t areaTotal = findNode(panelTotal, S_AreaPoint, 0xA);
    Frame f;
    const uint32_t closure = f.alloc(12);
    st32(closure, areaTotal);
    st32(closure + 4, panelTotal);
    st32(closure + 8, me);
    const int32_t day = int32_t(ld32(me + F_TOTAL_DAYS));
    if (!day || day > int32_t(ld32(me + F_CONTINUOUS_DAYS))) {
        call(g::LoginBonus_stampTotal, {closure});
        return;
    }
    const uint32_t name = newStr(f, S_PanelCont, 0xA);
    if (int32_t(ld32(me + F_TOTAL_DAYS)) >= 2) {
        const uint32_t buf = stringBuffer(f, {S_FmtPanelCont, ld32(me + F_TOTAL_DAYS)});
        call(g::StringAssign, {name, buf, call(0x40E08C, {buf})});
    }
    uint32_t panel;
    {
        Frame t;
        const uint32_t s = newStr(t, strData(name));
        panel = SceneUtil::searchNodeByName(center, s);
        strFree(s);
    }
    const uint32_t area = findNode(panel, S_AreaPoint, 0xA);
    const uint32_t lwf = call(g::AudioLWFNode_create, {S_StampLwf, 0});
    vcall(area, node::addChild, {area, lwf});
    playStampSound();
    const uint32_t done = newFunction(f, VT_StampDone, {areaTotal, panelTotal, me});
    playStamp(lwf, done);
    functionDestroy(done);
    strFree(name);
}

// The total-bonus stamp (closure {area point, total panel, this}): the stamp LWF, its sound when the panel shows.
void SceneLoginBonus::stampTotal(uint32_t closure) {
    const uint32_t me = ld32(closure + 8);
    const uint32_t lwf = call(g::AudioLWFNode_create, {S_StampLwf, 0});
    vcall(ld32(closure), node::addChild, {ld32(closure), lwf});
    if (vcall(ld32(closure + 4), V_isVisible, {ld32(closure + 4)}) == 1) playStampSound();
    Frame f;
    const uint32_t done = newFunction(f, VT_TotalStampDone, {me});
    playStamp(lwf, done);
    functionDestroy(done);
}

// The id an incentive shows: its own type for the plain kinds; the medal's / material's / keyblade's / avatar
// part's id field; 0 otherwise.
void SceneLoginBonus::getIncentiveId(uint32_t, uint32_t type, uint32_t id, uint32_t out) {
    uint32_t create = 0, offset = 0;
    switch (type) {
    case 2: case 4: case 6: case 7: case 11: case 15: case 16: case 17: case 26:
        st32(out, type);
        return;
    case 3: create = g::Medal_create; offset = 4; break;
    case 5: create = g::Material_create; offset = 0; break;
    case 13: create = g::Keyblade_create; offset = 0xCC; break;
    case 18: case 19: case 20: create = g::AvatarParts_create; offset = 0; break;
    default:
        st32(out, 0);
        return;
    }
    Frame f;
    const uint32_t row = f.alloc(8), value = f.alloc(4);
    call(create, {row, id});
    const uint32_t sb = ld32(row);
    rw::call(g::SafeBuffer_read, {sb, value, ld32(sb + 0xC) + offset, 4, offset});
    st32(out, ld32(value));
    if (ld32(row + 4)) call(fn::shared_release, {ld32(row + 4)});
}

// ------------------------------------------------------------------------------------------------ incentive texts
namespace {
constexpr uint32_t TextData_format = 0x492929;      // (string out, Type, id, ...)
constexpr uint32_t String_shrink = 0x48D705;        // the target string's buffer reset before a move
constexpr uint32_t Title_create = 0x78FA8D;
constexpr uint32_t INCENTIVE_TEXT = 0xE8A330;       // int[]: the text id of each plain incentive type
constexpr uint32_t S_Empty = 0xDF8A1A, S_FmtTitle = 0xE064A6;   // "", "%s:%s"
constexpr int32_t TXT_GET_FORMAT = 104500004, TXT_NEXT_FORMAT = 104501006, TXT_TITLE = 103400029;

// the plain kinds of make_getIncentive_name (make_getIncentiveNext_name has no 14)
bool plainIncentive(uint32_t type, bool next) {
    switch (type) {
    case 2: case 4: case 6: case 11: case 15: case 16: case 17: case 26: return true;
    case 14: return !next;
    default: return false;
    }
}
// a master row's name (len bytes at offset) through StringBuffer (the name is used as the format)
uint32_t rowName(Frame &f, uint32_t sb, uint32_t offset, uint32_t len) {
    const uint32_t raw = f.alloc((len + 4) & ~3u);
    rw::call(g::SafeBuffer_read, {sb, raw, ld32(sb + 0xC) + offset, len, offset});
    return stringBuffer(f, {raw});
}
void assignString(uint32_t dst, uint32_t text) { call(g::StringAssign, {dst, text, host_strlen(text)}); }
// dst = std::move(tmp): dst is cleared and its buffer reset, then takes tmp's three words
void moveString(uint32_t dst, uint32_t tmp) {
    if (ld8(dst) & 1) {
        st8(ld32(dst + 8), 0);
        st32(dst + 4, 0);
    } else {
        st8(dst + 1, 0);
        st8(dst, 0);
    }
    call(String_shrink, {dst, 0});
    for (uint32_t w = 0; w < 12; w += 4) st32(dst + w, ld32(tmp + w));
    for (uint32_t w = 0; w < 12; w += 4) st32(tmp + w, 0);
}
}  // namespace

// "Got <name> x<count>" for an incentive: the text of a plain kind, or the medal's / material's / keyblade's /
// avatar part's name, or the title ("Title:<name>"); *out gets the id make_getIncentive_id would give.
void SceneLoginBonus::getIncentiveName(uint32_t out, uint32_t, uint32_t type, uint32_t id, uint32_t count,
                                       uint32_t idOut) {
    st32(out, 0);
    st32(out + 4, 0);
    st32(out + 8, 0);
    Frame f;
    if (plainIncentive(type, false)) {
        st32(idOut, type);
        const uint32_t fmt = textData(f, TXT_GET_FORMAT);
        const uint32_t name = textData(f, int32_t(ld32(INCENTIVE_TEXT + 4 * type)));
        assignString(out, stringBuffer(f, {textDataStr(fmt), textDataStr(name), count}));
        sharedRelease(name);
        sharedRelease(fmt);
        return;
    }
    const uint32_t row = f.alloc(8);
    uint32_t idOffset = 0, nameOffset = 0, nameLen = 0x41;
    switch (type) {
    case 3: call(g::Medal_create, {row, id}); idOffset = 4; nameOffset = 0x10; break;
    case 5: call(g::Material_create, {row, id}); idOffset = 0; nameOffset = 0xC; break;
    case 13: call(g::Keyblade_create, {row, id}); idOffset = 0xCC; nameOffset = 4; break;
    case 18: case 19: case 20: call(g::AvatarParts_create, {row, id}); idOffset = 0; nameOffset = 4; nameLen = 0x81; break;
    case 7: {
        st32(idOut, 7);
        call(Title_create, {row, id});
        const uint32_t fmt = textData(f, TXT_GET_FORMAT);
        const uint32_t label = textData(f, TXT_TITLE);
        const uint32_t raw = f.alloc(0x44);
        rw::call(g::SafeBuffer_read, {ld32(row), raw, ld32(ld32(row) + 0xC) + 8, 0x41, 8});
        const uint32_t title = stringBuffer(f, {S_FmtTitle, textDataStr(label), raw});
        assignString(out, stringBuffer(f, {textDataStr(fmt), title, count}));
        sharedRelease(label);
        sharedRelease(fmt);
        releaseIf(ld32(row + 4));
        return;
    }
    default:
        assignString(out, stringBuffer(f, {S_Empty, count, id}));
        return;
    }
    st32(idOut, rowInt(ld32(row), idOffset));
    const uint32_t fmt = textData(f, TXT_GET_FORMAT);
    const uint32_t name = rowName(f, ld32(row), nameOffset, nameLen);
    assignString(out, stringBuffer(f, {textDataStr(fmt), name, count}));
    sharedRelease(fmt);
    releaseIf(ld32(row + 4));
}

// The next total bonus's text: TextData::format(104501006, a6, name, a5) with the incentive's name as above.
void SceneLoginBonus::getIncentiveNextName(uint32_t out, uint32_t, uint32_t type, uint32_t id, uint32_t a5,
                                           uint32_t a6) {
    st32(out, 0);
    st32(out + 4, 0);
    st32(out + 8, 0);
    Frame f;
    const uint32_t tmp = f.alloc(12);
    auto format = [&](uint32_t name) {
        rw::call(TextData_format, {tmp, 0, uint32_t(TXT_NEXT_FORMAT), a6, name, a5});
        moveString(out, tmp);
    };
    if (plainIncentive(type, true)) {
        const uint32_t name = textData(f, int32_t(ld32(INCENTIVE_TEXT + 4 * type)));
        format(textDataStr(name));
        sharedRelease(name);
        return;
    }
    const uint32_t row = f.alloc(8);
    uint32_t nameOffset = 0, nameLen = 0x41;
    switch (type) {
    case 3: call(g::Medal_create, {row, id}); nameOffset = 0x10; break;
    case 5: call(g::Material_create, {row, id}); nameOffset = 0xC; break;
    case 13: call(g::Keyblade_create, {row, id}); nameOffset = 4; break;
    case 18: case 19: case 20: call(g::AvatarParts_create, {row, id}); nameOffset = 4; nameLen = 0x81; break;
    case 7: {
        call(Title_create, {row, id});
        const uint32_t label = textData(f, TXT_TITLE);
        const uint32_t raw = f.alloc(0x44);
        rw::call(g::SafeBuffer_read, {ld32(row), raw, ld32(ld32(row) + 0xC) + 8, 0x41, 8});
        format(stringBuffer(f, {S_FmtTitle, textDataStr(label), raw}));
        sharedRelease(label);
        releaseIf(ld32(row + 4));
        return;
    }
    default:
        assignString(out, stringBuffer(f, {S_Empty}));
        return;
    }
    format(rowName(f, ld32(row), nameOffset, nameLen));
    releaseIf(ld32(row + 4));
}

// "img/incentive/IncentiveIcon_<nn>.png" from the texture cache
uint32_t SceneLoginBonus::loadIncentiveTexture(uint32_t, uint32_t number) {
    const uint32_t cache = call(g::Director_getTextureCache, {call(g::Director_getInstance, {})});
    Frame f;
    const uint32_t name = newStr(f, stringBuffer(f, {S_IncentiveIcon, number}));
    const uint32_t texture = call(g::TextureCache_addImage, {cache, name});
    strFree(name);
    return texture;
}

RW_FUNCTION(0x6F6328, "SceneLoginBonus::create", SceneLoginBonus::create);
RW_FUNCTION(0x6F63C4, "SceneLoginBonus::~SceneLoginBonus", SceneLoginBonus::destruct);
RW_FUNCTION(0x6F649C, "SceneLoginBonus::init", SceneLoginBonus::init);
RW_FUNCTION(0x6F85BC, "SceneLoginBonus::runInAnimation", SceneLoginBonus::runInAnimation);
RW_FUNCTION(0x6F8DEC, "SceneLoginBonus::runOutAnimationMain", SceneLoginBonus::runOutAnimationMain);
RW_FUNCTION(0x6F902C, "SceneLoginBonus::skipEvent", SceneLoginBonus::skipEvent);
RW_FUNCTION(0x6FA1A0, "SceneLoginBonus::openLoginBonus", SceneLoginBonus::openLoginBonus);
RW_FUNCTION(0x6FA580, "SceneLoginBonus::openLoginBonus total stamp (sub_6FA580)", SceneLoginBonus::stampTotal);
RW_FUNCTION(0x6F9B50, "SceneLoginBonus::make_getIncentive_id", SceneLoginBonus::getIncentiveId);
RW_FUNCTION(0x6FA0D4, "SceneLoginBonus::loadIncentiveTexture", SceneLoginBonus::loadIncentiveTexture);
RW_FUNCTION(0x6F90E4, "SceneLoginBonus::make_getIncentive_name", SceneLoginBonus::getIncentiveName);
RW_FUNCTION(0x6F9688, "SceneLoginBonus::make_getIncentiveNext_name", SceneLoginBonus::getIncentiveNextName);
