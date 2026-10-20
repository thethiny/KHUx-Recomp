// Quests > Coliseum: SceneAdventureSelect::initColosseum (ticker, the Start button and its lambda $_33 {this,
// the Colosseum reply shared_ptr}, the Ranking / Tier Rewards / Coliseum Board buttons of
// AdventureTop2_Colosseum_Button - each runs the scene out ($_36 / $_37 / $_38) then changes scene: the ranking
// (after its API), the reward list (after its API), the colosseum sphere board), the description of a cleared
// tier, the stage info), colosseumInAnimation, openTutorial25 (the skip tutorial once there are 2+ rounds), the
// stage info lambda $_46 of changeStageInfo (title, round info, its in animation, the campaign banner), and
// AdvStageScrollView::setColosseumInfo's page lambda $_7 (the round's enemies from stage/cl_stg%05d_%02d.bin on
// PlateEnemy1..6, sub_572E04) and the $_8 clone / destroy.
#include "../scene/SceneUi.hpp"

#include <cstring>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269, searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15, SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39, SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t FunctionVV_copy = 0x4683CD, FunctionVV_call = 0x470E69, FunctionVB_assign = 0x490109;
constexpr uint32_t FunctionWI_call = 0x54A065;     // std::function<Widget*(int)>::operator()
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t SphereBoard_getValidCnt = 0x6BB4FD;
constexpr uint32_t SceneRanking_callColosseumRankingAPI = 0x6FBBE1;
constexpr uint32_t APIManager_executeColosseumRewardList = 0x4D1469;
constexpr uint32_t changeStageInfo = 0x723089, colosseumInAnimation = 0x724869, openTutorial25 = 0x724909;
constexpr uint32_t createWidget = 0x723955, backButtonRunInAnimation = 0x726015;
constexpr uint32_t runRightInAnimation = 0x726115, scrollRunInAnimation = 0x726285, runInfoInAnimation = 0x72ABDD;
constexpr uint32_t buttonListener = 0x724769;       // sub_724768(button, function): the button's touch lambda
constexpr uint32_t setColosseumTitle = 0x56F429, setColosseumInfo = 0x56D65D;
constexpr uint32_t CampaignAnimationView_show = 0x598D59;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t FileUtils_getInstance = 0xA3F44D, Data_getBytes = 0x8ED90B, Data_dtor = 0x8ED89B;
constexpr uint32_t StageManager_int32LE = 0x75992B, Enemy_create = 0x77F7DD;
constexpr uint32_t VecInt_dtor = 0x478961;
constexpr uint32_t plate = 0x572E05;
}  // namespace g

// SceneAdventureSelect fields
constexpr uint32_t F_TYPE = 0x28C, F_INOUT = 0x288, F_STAGE = 0x298, F_HEADER_LOCK = 0x29E, F_CAN_TAP = 0x325;
constexpr uint32_t F_DESCRIPTION = 0x370, F_INFO_PANEL = 0x398, F_COLOSSEUM = 0x3A0, F_BUTTONS = 0x3D4;
constexpr uint32_t F_TAP_FLAG = 0x3FC, F_PARENT = 0x2A8;
constexpr uint32_t SM_MODE = 0x28C, SM_KIND = 0x290, SM_CALLBACK_FLAG = 0x4CC, SM_CALLBACK = 0x548;
constexpr uint32_t API_COLOSSEUM = 0x24C;     // shared_ptr: {.., +4 state, +8 vector<Stage> (16 bytes each)}
// lambda vtables
constexpr uint32_t VT_Start = 0xF6C63C;      // $_33 {this, colosseum shared_ptr}
constexpr uint32_t VT_Ranking = 0xF6C5B8, VT_Reward = 0xF6C560, VT_Board = 0xF6C508;   // {this}
constexpr uint32_t VT_RankingOut = 0xF6C5E4, VT_RewardOut = 0xF6C58C, VT_BoardOut = 0xF6C534;   // {}
constexpr uint32_t VT_RankingScene = 0xF6C610;   // {}: SceneRanking's layer
constexpr uint32_t VT_InfoIn = 0xF6C2A0;         // $_46's runInfoInAnimation lambda {this}
constexpr uint32_t VT_Colosseum8 = 0xF59540;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074;   // __func<Layer*(*)()> vtable
constexpr uint32_t GOT_CREATE_BOARD = 0xFD68A0, GOT_CREATE_REWARD = 0xFD698C;
constexpr uint32_t GOT_KEYBLADE_TYPE_PASS = 0xFD6698;
constexpr uint32_t S_ColosseumInfo = 0xDFF8CF, S_ButtonStart = 0xE07021, S_TxtStart = 0xE0702E;
constexpr uint32_t S_QuestStart = 0xE06EF1, S_But10 = 0xE06F08, S_ButtonJson = 0xE07038;
constexpr uint32_t S_ButtonRanking = 0xE0706F, S_ButtonReward = 0xE0707E, S_ButtonBoard = 0xE0708C;
constexpr uint32_t S_TxtDescription = 0xE00690, S_TargetJson = 0xDFF810, S_StageBin = 0xDFF5AD;
constexpr uint32_t S_PlateEnemy = 0xDFF847, S_KeyBladeType = 0xDF8A58, S_TxtNameLabel = 0xDFDBB8;
constexpr uint32_t S_TxtLV = 0xDF8DE1, S_TxtLVLabel = 0xDF8DE8, S_FmtInt = 0xDF86DE;
constexpr uint32_t V_setTouchEnabled = 0x29C;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline uint32_t findC(uint32_t root, uint32_t name) { return findNode(root, name, host_strlen(name)); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void titleLabel3(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(g::SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}
// a short std::string built in place (the original stores the bytes inline)
uint32_t shortStr(Frame &f, const char *text) {
    const uint32_t s = f.alloc(12);
    const uint32_t n = uint32_t(std::strlen(text));
    st8(s, uint8_t(n * 2));
    for (uint32_t k = 0; k < n; ++k) st8(s + 1 + k, uint8_t(text[k]));
    st8(s + 1 + n, 0);
    return s;
}
// a scene button with its "Txt" title
uint32_t sceneButton(uint32_t widget, uint32_t name, uint32_t len, int32_t textId) {
    const uint32_t button = find(widget, name, len);
    Frame f;
    const uint32_t txt = shortStr(f, "Txt");
    call(g::SceneUtil_settingTitleLabel, {button, txt, uint32_t(textId)});
    strFree(txt);
    return button;
}
// the button's touch lambda {this}
void hookButton(uint32_t button, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(g::buttonListener, {button, fo});
    functionDestroy(fo);
}
}  // namespace

struct Colosseum {
    static void initColosseum(uint32_t self);
    static void colosseumInAnimation(uint32_t self);
    static void openTutorial25(uint32_t self, uint32_t done);
    static void cloneStartTo(uint32_t fo, uint32_t dst);
    static void sceneOut(uint32_t fo, uint32_t vt, bool clearTap);
    static void rankingOut(uint32_t fo) { sceneOut(fo, VT_RankingOut, false); }
    static void rewardOut(uint32_t fo) { sceneOut(fo, VT_RewardOut, false); }
    static void boardOut(uint32_t fo) { sceneOut(fo, VT_BoardOut, true); }
    static void boardScene(uint32_t fo);
    static void rewardScene(uint32_t fo);
    static void rankingScene(uint32_t fo);
    static void stageInfo(uint32_t fo);
    static void destroyPage(uint32_t fo);
    static uint32_t page(uint32_t fo, uint32_t widgetRef, uint32_t indexRef);
    static void plate(uint32_t widget, uint32_t index, uint32_t enemy, uint32_t level, uint32_t show);
    static uint32_t clone8(uint32_t fo);
    static void destroy8(uint32_t fo);
};

void Colosseum::initColosseum(uint32_t self) {
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t td = textData(f, 100620021);
        const uint32_t text = textDataStr(td);
        const uint32_t s = newStr(f, text, host_strlen(text));
        call(g::SceneManager_changeTickerMessage, {sm, s});
        strFree(s);
        sharedRelease(td);
    }
    st32(sceneManager() + SM_MODE, 5);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t info = find(ld32(self + F_COLOSSEUM), S_ColosseumInfo, 0xD);
    const uint32_t start = find(info, S_ButtonStart, 0xC);
    titleLabel3(start, S_TxtStart, 9, 102900003);
    if (!find(start, S_QuestStart, 0x16)) {
        Frame f;
        const uint32_t s = newStr(f, S_But10, 5);
        const uint32_t button = call(g::ImageButton_create, {s, 0});
        strFree(s);
        call(g::ImageButton_setTargetImageView, {button, start});
        {
            const uint32_t name = newStr(f, S_QuestStart, 0x16);
            vcall(start, node::addChildZName, {start, button, 0, name});
            strFree(name);
        }
        const uint32_t ctl = ld32(api + API_COLOSSEUM + 4);
        const uint32_t obj = ld32(api + API_COLOSSEUM);
        addRef(ctl);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, fo);
        st32(fo, VT_Start);
        st32(fo + 4, self);
        st32(fo + 8, obj);
        st32(fo + 12, ctl);
        addRef(ctl);
        st32(fo + 4, self);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        release(ctl);
    }
    if (!ld32(self + F_BUTTONS + 8)) {
        call(g::createWidget, {self + F_BUTTONS, ld32(self + F_PARENT), S_ButtonJson});
        visible(ld32(self + F_BUTTONS + 8), 1);
        hookButton(sceneButton(ld32(self + F_BUTTONS + 8), S_ButtonRanking, 0xE, 103300021), VT_Ranking, self);
        hookButton(sceneButton(ld32(self + F_BUTTONS + 8), S_ButtonReward, 0xD, 103300022), VT_Reward, self);
        const uint32_t board = sceneButton(ld32(self + F_BUTTONS + 8), S_ButtonBoard, 0x16, 103300023);
        if (call(g::SphereBoard_getValidCnt, {3})) hookButton(board, VT_Board, self);
        else visible(board, 0);
    }
    uint32_t col = ld32(api + API_COLOSSEUM);
    if (col) {
        bool check = true;
        if (ld32(col + 4) == 2) {
            titleLabel3(ld32(self + F_DESCRIPTION), S_TxtDescription, 0xF, 102900024);
            visible(ld32(self + F_DESCRIPTION), 1);
            col = ld32(api + API_COLOSSEUM);
            check = col != 0;
        }
        if (check && ld32(col + 0xC) != ld32(col + 8)) call(g::changeStageInfo, {self});
    }
    call(g::colosseumInAnimation, {self});
    Frame f;
    const uint32_t done = newEmptyFunction(f);
    call(g::openTutorial25, {self, done});
    functionDestroy(done);
}

void Colosseum::colosseumInAnimation(uint32_t self) {
    call(g::runRightInAnimation, {self + F_BUTTONS});
    Frame f;
    const uint32_t done = newEmptyFunction(f);
    call(g::scrollRunInAnimation, {self, done});
    functionDestroy(done);
    call(g::backButtonRunInAnimation, {self});
}

// The "skip rounds" tutorial (25) once the tier has two or more rounds, `done` after it; else `done` now
void Colosseum::openTutorial25(uint32_t self, uint32_t done) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    if (ld32(self + F_TYPE) == 5) {
        const uint32_t col = ld32(api + API_COLOSSEUM);
        if (((ld32(col + 0xC) - ld32(col + 8)) >> 4) >= 2) {
            Frame f;
            const uint32_t copy = f.alloc(20);
            call(g::FunctionVV_copy, {copy, done});
            const uint32_t out = f.alloc(20);
            call(g::SceneUtil_openTutorialPopup, {out, 25, copy, 0, 0, 0, 0});
            functionDestroy(out);
            functionDestroy(copy);
            return;
        }
    }
    if (ld32(done + 16)) call(g::FunctionVV_call, {done});
}

// $_33 {this, colosseum shared_ptr} __clone(p)
void Colosseum::cloneStartTo(uint32_t fo, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_Start);
    st32(dst + 4, ld32(fo + 4));
    st32(dst + 8, ld32(fo + 8));
    st32(dst + 0xC, ld32(fo + 0xC));
    addRef(ld32(dst + 0xC));
    st32(dst + 4, ld32(fo + 4));
}

// $_36 / $_37 / $_38 {this}: once (no header lock, taps allowed) - decide SE, header off, the description hidden,
// the scene runs out and then changes
void Colosseum::sceneOut(uint32_t fo, uint32_t vt, bool clearTap) {
    const uint32_t self = ld32(fo + 4);
    if (ld8(self + F_HEADER_LOCK) || !ld8(self + F_CAN_TAP)) return;
    st8(self + F_CAN_TAP, 0);
    call(g::AudioPlaySystemSEDecide, {});
    call(g::SceneManager_setCanTouchHeader, {sceneManager(), 0});
    if (ld32(self + F_DESCRIPTION)) visible(ld32(self + F_DESCRIPTION), 0);
    if (clearTap) st8(self + F_TAP_FLAG, 0);
    Frame f;
    const uint32_t done = newFunction(f, vt, {});
    call(g::IInOutAnimation_runOutAnimation, {self + F_INOUT, done});
    functionDestroy(done);
}

namespace {
// SceneManager::changeSceneLayer(a function-pointer layer factory, a, b, fade)
void changeLayer(uint32_t create, uint32_t a, uint32_t b, uint32_t fade) {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(create)});
    call(g::SceneManager_changeSceneLayer, {sm, fo, a, b, fade});
    functionDestroy(fo);
}
// SceneManager's back callback cleared (the flag set first)
void clearSceneCallback() {
    const uint32_t sm = sceneManager();
    st8(sm + SM_CALLBACK_FLAG, 1);
    Frame f;
    const uint32_t empty = newEmptyFunction(f);
    call(g::FunctionVB_assign, {sm + SM_CALLBACK, empty});
    functionDestroy(empty);
}
}  // namespace

void Colosseum::boardScene(uint32_t) {
    st32(sceneManager() + SM_MODE, 5);
    changeLayer(GOT_CREATE_BOARD, 1, 1, 1);
}

void Colosseum::rewardScene(uint32_t) {
    clearSceneCallback();
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t empty = newEmptyFunction(f);
        call(g::APIManager_executeColosseumRewardList, {api, empty});
        functionDestroy(empty);
    }
    st32(sceneManager() + SM_KIND, 3);
    changeLayer(GOT_CREATE_REWARD, 1, 0, 0);
}

void Colosseum::rankingScene(uint32_t) {
    clearSceneCallback();
    {
        Frame f;
        const uint32_t empty = newEmptyFunction(f);
        call(g::SceneRanking_callColosseumRankingAPI, {empty});
        functionDestroy(empty);
    }
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, VT_RankingScene, {});
    call(g::SceneManager_changeSceneLayer, {sm, fo, 1, 0, 0});
    functionDestroy(fo);
}

// changeStageInfo $_46 {this, campaign banner}: the selected round's title and info, the info panel's in
// animation, the campaign banner
void Colosseum::stageInfo(uint32_t fo) {
    const uint32_t self = ld32(fo + 4);
    if (ld32(self + F_STAGE) == 0xFFFFFFFFu) return;
    uint32_t title;
    {
        Frame f;
        const uint32_t s = shortStr(f, "Title");
        title = call(g::searchNodeByName, {ld32(self + F_COLOSSEUM), s});
        strFree(s);
    }
    const uint32_t info = find(ld32(self + F_COLOSSEUM), S_ColosseumInfo, 0xD);
    const uint32_t col = ld32(call(fn::APIManager_getInstance, {}) + API_COLOSSEUM);
    if (!col) return;
    const uint32_t stages = col + 8;
    const uint32_t n = (ld32(stages + 4) - ld32(stages)) >> 4;
    if (!n) return;
    const uint32_t at = ld32(self + F_STAGE);
    if (n <= at) call(g::ThrowOutOfRange, {stages});
    Frame f;
    const uint32_t stage = f.alloc(16);
    for (uint32_t k = 0; k < 16; k += 4) st32(stage + k, ld32(ld32(stages) + 16 * at + k));
    call(g::setColosseumTitle, {title, ld32(self + F_STAGE)});
    call(g::setColosseumInfo, {info, stage});
    {
        const uint32_t done = newFunction(f, VT_InfoIn, {self});
        call(g::runInfoInAnimation, {self + F_INFO_PANEL, done});
        functionDestroy(done);
    }
    if (ld32(fo + 8)) call(g::CampaignAnimationView_show, {ld32(fo + 8)});
}

// AdvStageScrollView::setColosseumInfo $_7: destroy_deallocate (its three vector<int>)
void Colosseum::destroyPage(uint32_t fo) {
    call(g::VecInt_dtor, {fo + 0x40});
    call(g::VecInt_dtor, {fo + 0x34});
    call(g::VecInt_dtor, {fo + 0x28});
    call(fn::operator_delete, {fo});
}

// $_7 {view, .., stage id (+0x14), tier (+0x1C), the stage data offset (+0x20), enemy count (+0x24), the round's
// enemy ids / ids / levels}: page `index` (the view's factory page plus the Colosseum_Target layout): its six
// plates cleared, then the enemies of rounds 6*index.. filled in order
uint32_t Colosseum::page(uint32_t fo, uint32_t widgetRef, uint32_t indexRef) {
    const uint32_t index = ld32(indexRef);
    uint32_t widget = ld32(widgetRef);
    if (!widget) {
        widget = call(g::FunctionWI_call, {ld32(fo + 4) + 0x500, index});
        call(g::Ref_retain, {widget});
        const uint32_t json = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_TargetJson});
        vcall(widget, node::addChild, {widget, json});
    }
    for (uint32_t i = 1; i != 7; ++i) call(g::plate, {widget, i, 0, 0, 0});
    Frame f;
    const uint32_t fu = call(g::FileUtils_getInstance, {});
    const uint32_t data = f.alloc(12);
    {
        Frame g2;
        const uint32_t path = stringBuffer(g2, {S_StageBin, ld32(fo + 0x14), ld32(fo + 0x1C)});
        const uint32_t s = newStr(g2, path, host_strlen(path));
        vcall(fu, 0x10, {data, fu, s});
        strFree(s);
    }
    const uint32_t bytes = call(g::Data_getBytes, {data});
    uint32_t slot = 1;
    int32_t n = 6 * int32_t(index);
    if (n < int32_t(ld32(fo + 0x24))) {
        const int32_t last = n + 5;
        for (;;) {
            const uint32_t v = call(g::StageManager_int32LE, {bytes + ld32(fo + 0x20) + 0x1C + 0x44 * uint32_t(n)});
            const uint32_t ids = fo + 0x28;
            if (int32_t(ld32(ids + 4) - ld32(ids)) >= 1) {
                const uint32_t want = v + 1;
                for (uint32_t k = 0; int32_t(k) < int32_t(ld32(ids + 4) - ld32(ids)) >> 2; ++k) {
                    if (((ld32(ids + 4) - ld32(ids)) >> 2) <= k) call(g::ThrowOutOfRange, {ids});
                    if (ld32(ld32(ids) + 4 * k) != want) continue;
                    if (((ld32(fo + 0x38) - ld32(fo + 0x34)) >> 2) <= k) call(g::ThrowOutOfRange, {fo + 0x34});
                    if (((ld32(fo + 0x44) - ld32(fo + 0x40)) >> 2) <= k) call(g::ThrowOutOfRange, {fo + 0x40});
                    call(g::plate, {widget, slot, ld32(ld32(fo + 0x34) + 4 * k), ld32(ld32(fo + 0x40) + 4 * k), 1});
                    ++slot;
                }
            }
            if (n >= last) break;
            ++n;
            if (!(n < int32_t(ld32(fo + 0x24)))) break;
        }
    }
    call(g::Data_dtor, {data});
    return widget;
}

// sub_572E04: enemy plate `index` shown or hidden; shown: the enemy's keyblade type icon, name, "Lv" and level
void Colosseum::plate(uint32_t widget, uint32_t index, uint32_t enemy, uint32_t level, uint32_t show) {
    uint32_t node;
    {
        Frame f;
        node = findC(widget, stringBuffer(f, {S_PlateEnemy, index}));
    }
    if (!node) return;
    visible(node, show);
    if (show != 1) return;
    Frame f;
    const uint32_t enemyPtr = f.alloc(8);
    call(g::Enemy_create, {enemyPtr, enemy});
    const uint32_t row = ld32(enemyPtr);
    {
        const uint32_t icon = find(node, S_KeyBladeType, 0xD);
        const uint32_t fmt = ld32(ld32(GOT_KEYBLADE_TYPE_PASS));
        const uint32_t v = f.alloc(4);
        rw::call(g::SafeBuffer_read, {row, v, ld32(row + 0xC) + 0x278, 4, 0x278});
        Frame g2;
        const uint32_t path = stringBuffer(g2, {fmt, ld32(v)});
        const uint32_t s = newStr(g2, path, host_strlen(path));
        call(g::ImageView_loadTexture, {icon, s, 0});
        strFree(s);
    }
    {
        const uint32_t t = find(node, S_TxtNameLabel, 0xE);
        call(g::SceneUtil_setShadow, {t});
        Frame g2;
        const uint32_t raw = g2.alloc(0x44);
        rw::call(g::SafeBuffer_read, {row, raw, ld32(row + 0xC) + 4, 0x41, 4});
        setTextC(t, raw);
    }
    {
        const uint32_t t = find(node, S_TxtLV, 6);
        call(g::SceneUtil_setShadow, {t});
        setTextId(t, 100200002);
    }
    {
        const uint32_t t = find(node, S_TxtLVLabel, 0xC);
        call(g::SceneUtil_setShadow, {t});
        Frame g2;
        setTextC(t, stringBuffer(g2, {S_FmtInt, level}));
    }
    release(ld32(enemyPtr + 4));
}

// setColosseumInfo $_7's $_8 {flag byte, std::function<void()>}: __clone() / destroy_deallocate()
uint32_t Colosseum::clone8(uint32_t fo) {
    const uint32_t p = call(fn::operator_new, {0x28});
    if (p) {
        st32(p, VT_Colosseum8);
        st8(p + 8, ld8(fo + 8));
        call(g::FunctionVV_copy, {p + 0x10, fo + 0x10});
        st8(p + 8, ld8(fo + 8));
    }
    return p;
}
void Colosseum::destroy8(uint32_t fo) {
    functionDestroy(fo + 0x10);
    call(fn::operator_delete, {fo});
}

RW_FUNCTION(0x723F78, "SceneAdventureSelect::initColosseum", Colosseum::initColosseum);
RW_FUNCTION(0x724868, "SceneAdventureSelect::colosseumInAnimation", Colosseum::colosseumInAnimation);
RW_FUNCTION(0x724908, "SceneAdventureSelect::openTutorial25", Colosseum::openTutorial25);
RW_FUNCTION(0x72D0B0, "SceneAdventureSelect::initColosseum $_33 __func::__clone(p) (sub_72D0B0)", Colosseum::cloneStartTo);
RW_FUNCTION(0x72CCC0, "SceneAdventureSelect::initColosseum $_36 __func::operator() (sub_72CCC0)", Colosseum::rankingOut);
RW_FUNCTION(0x72CE2C, "SceneAdventureSelect::initColosseum $_36 inner __func::operator() (sub_72CE2C)", Colosseum::rankingScene);
RW_FUNCTION(0x72C984, "SceneAdventureSelect::initColosseum $_37 __func::operator() (sub_72C984)", Colosseum::rewardOut);
RW_FUNCTION(0x72CAF0, "SceneAdventureSelect::initColosseum $_37 inner __func::operator() (sub_72CAF0)", Colosseum::rewardScene);
RW_FUNCTION(0x72C6C8, "SceneAdventureSelect::initColosseum $_38 __func::operator() (sub_72C6C8)", Colosseum::boardOut);
RW_FUNCTION(0x72C838, "SceneAdventureSelect::initColosseum $_38 inner __func::operator() (sub_72C838)", Colosseum::boardScene);
RW_FUNCTION(0x72AE30, "SceneAdventureSelect::changeStageInfo $_46 __func::operator() (sub_72AE30)", Colosseum::stageInfo);
RW_FUNCTION(0x572B6A, "AdvStageScrollView::setColosseumInfo $_7 __func::destroy_deallocate() (sub_572B6A)", Colosseum::destroyPage);
RW_FUNCTION(0x572B94, "AdvStageScrollView::setColosseumInfo $_7 __func::operator() (sub_572B94)", Colosseum::page);
RW_FUNCTION(0x572E04, "AdvStageScrollView colosseum enemy plate (sub_572E04)", Colosseum::plate);
RW_FUNCTION(0x573340, "AdvStageScrollView::setColosseumInfo $_8 __func::__clone() (sub_573340)", Colosseum::clone8);
RW_FUNCTION(0x5733F8, "AdvStageScrollView::setColosseumInfo $_8 __func::destroy_deallocate() (sub_5733F8)", Colosseum::destroy8);
