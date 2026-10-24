// SceneUtil: scene flow helpers - back to My Page (or the interrupted quest), the My Page API chain, the
// Android back key, medal locks, boss type icons.
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t APIManager_userInfo = 0x84;             // shared_ptr<...> (SafeBuffer; party flag byte at 0x10)
constexpr uint32_t APIManager_userMedals = 0xD4;
constexpr uint32_t APIManager_campaignStages = 0xEC;       // SafeBuffer*: 88-byte records, count at +0x20

// SceneManager fields cleared / set when going back to My Page (the "where to go next" state)
constexpr uint32_t SM_nextA = 0x2B0, SM_nextB = 0x2B4, SM_nextC = 0x2C0, SM_nextD = 0x2C4, SM_nextE = 0x2C8,
                   SM_nextF = 0x2D0, SM_nextG = 0x2D4, SM_nextH = 0x2D8, SM_nextI = 0x2DC;
constexpr uint32_t SM_loadingBlocked = 0x4CC, SM_onFinishLoading = 0x548;
constexpr uint32_t StartDeckEditDialog_mode = 0x2D4;

constexpr uint32_t StartDeckEditDialog_makeShared = 0x485625;
constexpr uint32_t StartDeckEditDialog_startAdventure = 0x576FC5;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;   // (mgr, function<Layer*()>*, bool, bool, FadeType)
constexpr uint32_t VT_LayerFactory = 0xF7E130;             // __func<Layer*(*)(), ...>
constexpr uint32_t CreateSceneLayer_MyPage = 0x46FDBD;     // createSceneLayer<SceneMyPage>
constexpr uint32_t FunctionB_assign = 0x490109;
constexpr uint32_t PrepareMypage_next = 0x471201;          // the lambda's body: sphere check, then the next API
constexpr uint32_t APIManager_executePartyMemberList = 0x4C7AA5;
constexpr uint32_t VT_PrepareMypage_66 = 0xF54668;

constexpr uint32_t EventListenerKeyboard_create = rwlib::cocos2d::EventListenerKeyboard::create;
constexpr uint32_t KeyFunction_swap = 0x47C949;            // std::function<void(KeyCode, Event*)>::swap
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t VT_BackKey = 0xF53BC0;                  // addBackKeyCallback $_122 (heap)
constexpr uint32_t Listener_onKeyReleased = 0x68;

constexpr uint32_t AppState_getWillLockUserMedalIDs = 0x4EB347;
constexpr uint32_t AppState_getWillUnLockUserMedalIDs = 0x4EB507;
constexpr uint32_t MedalIndex_find = 0x490275;
constexpr uint32_t U64Set_insertUnique = 0x482649;         // unordered_set<u64>::__insert_unique (sret pair)
constexpr uint32_t U64Set_copy = 0x4740A9;
constexpr uint32_t U64Set_dtor = 0x482619;
constexpr uint32_t FunctionB_call = 0x474139;              // std::function<void(bool)>::operator()(bool)
constexpr uint32_t APIManager_executeUserMedalLock = 0x4C91F5;
constexpr uint32_t VT_LockMedal_76 = 0xF543D4;

constexpr uint32_t RaidEnemy_create = 0x787745, Enemy_create = 0x77F7DD;
constexpr uint32_t FileUtils_getInstance = rwlib::cocos2d::FileUtils::getInstance;
constexpr uint32_t FileUtils_getDataFromFile = 0x10;       // vtable slot (sret Data, utils, const string&)
constexpr uint32_t Data_getBytes = rwlib::cocos2d::Data::getBytes, Data_dtor = rwlib::cocos2d::Data::dtor_base;
constexpr uint32_t StageManager_getInteger32bitFromLittleEndianBytes = 0x75992B;
constexpr uint32_t STR_KeyBladeTypePng = 0xDF905F;         // "cocostudio/publish/KeyBlade_type_%02d.png"
constexpr uint32_t STR_MappoiBin = 0xDF9089;               // "stage/mappoi_stg%05d.bin"

void clear(uint32_t mgr, uint32_t off) { st32(mgr + off, 0); }
}  // namespace

// Back to My Page: reset the "next destination", pick the campaign stage to show, then either resume an
// interrupted quest (resumption status 2 / 3 / other) or load My Page.
void SceneUtil::moveToMyPageOrResumeStage() {
    clear(sceneManager(), SM_nextE);
    uint32_t mgr = sceneManager();
    clear(mgr, SM_nextD);
    clear(mgr, SM_nextC);
    mgr = sceneManager();
    clear(mgr, SM_nextB);
    clear(mgr, SM_nextA);
    clear(sceneManager(), SM_nextH);
    clear(sceneManager(), SM_nextI);
    uint32_t list = call(fn::APIManager_getInstance, {}) + APIManager_campaignStages;
    if (ld32(ld32(list) + 0x20)) {
        Frame f;
        uint32_t record = f.alloc(0x58);
        uint32_t offset = 0;
        for (uint32_t k = 0; k < ld32(ld32(list) + 0x20); k++, offset += 0x58) {
            uint32_t sb = ld32(list);
            SafeBuffer::read(safe(sb), rw::host_ptr<uint8_t>(record), safe(sb)->data.get() + offset, 0x58, offset);
            if (ld8(record + 0x4C)) {
                uint32_t a = ld32(record), b = ld32(record + 4);
                uint32_t m = sceneManager();
                st32(m + SM_nextB, b);
                st32(m + SM_nextA, a);
            }
        }
    }
    int32_t status = getStageResumptionStatus();
    int32_t mode;
    if (status == 3) mode = 2;
    else if (status == 2) mode = 3;
    else mode = 0;
    if (status) {
        clear(sceneManager(), SM_nextE);
        mgr = sceneManager();
        clear(mgr, SM_nextG);
        clear(mgr, SM_nextF);
        mgr = sceneManager();
        clear(mgr, SM_nextD);
        clear(mgr, SM_nextC);
        mgr = sceneManager();
        clear(mgr, SM_nextB);
        clear(mgr, SM_nextA);
        clear(sceneManager(), SM_nextH);
        clear(sceneManager(), SM_nextI);
        Frame f;
        uint32_t dialog = f.alloc(8);
        call(StartDeckEditDialog_makeShared, {dialog});
        st32(ld32(dialog) + StartDeckEditDialog_mode, uint32_t(mode));
        call(StartDeckEditDialog_startAdventure, {ld32(dialog)});
        sharedRelease(dialog);
        return;
    }
    Frame f;
    uint32_t none = newEmptyFunction(f);
    callPrepareMypageAPIs(none);
    functionDestroy(none);
    uint32_t m = sceneManager();
    uint32_t factory = newFunction(f, VT_LayerFactory, {CreateSceneLayer_MyPage});
    call(SceneManager_changeSceneLayer, {m, factory, 1, 1, 1});
    functionDestroy(factory);
}
RW_FUNCTION(0x46F160, "SceneUtil::moveToMyPageOrResumeStage", SceneUtil::moveToMyPageOrResumeStage);

// The APIs My Page needs before it opens (party members, then a chain run by the lambda), with `callback` at
// the end. The original nests the callback through several lambdas: every level copies both captured
// callbacks, which is reproduced here (clone order and count decide the heap layout).
void SceneUtil::callPrepareMypageAPIs(Addr callback) {
    Frame f;
    uint32_t a[9], b[8];
    for (int k = 0; k < 9; k++) a[k] = f.alloc(24);
    for (int k = 0; k < 7; k++) b[k] = f.alloc(24);
    uint32_t last = f.alloc(48);                       // the innermost lambda: callbacks at +0 and +24
    a[8] = last;
    b[7] = last + 24;
    call(fn::function_vb_copy, {a[0], callback});
    functionCopy(a[1], a[0]);
    call(fn::function_vb_copy, {b[0], callback});
    for (int k = 1; k <= 7; k++) {
        functionCopy(a[k + 1], a[k]);
        functionCopy(b[k], b[k - 1]);
    }
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t infoCtl = ld32(api + APIManager_userInfo + 4);
    uint32_t info = ld32(api + APIManager_userInfo);
    if (infoCtl) call(fn::shared_add, {infoCtl});
    bool inParty = false;
    if (info) {
        // one byte at 0x10 (the original decodes it with SafeBuffer::read into its frame)
        const SafeBuffer *sb = safe(info);
        inParty = uint8_t(sb->data[0x10] ^ SafeBuffer::mask(sb->key, 0x10)) != 0;
    }
    if (inParty) {
        uint32_t mgr = sceneManager();
        uint32_t empty = newEmptyFunction(f);
        st8(mgr + SM_loadingBlocked, 1);
        call(FunctionB_assign, {mgr + SM_onFinishLoading, empty});
        functionDestroy(empty);
        uint32_t api2 = call(fn::APIManager_getInstance, {});
        uint32_t pair = f.alloc(48);
        functionCopy(pair, a[8]);
        functionCopy(pair + 24, b[7]);
        uint32_t done = f.alloc(20);
        st32(done + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x38});
        if (heap) {
            st32(heap, VT_PrepareMypage_66);
            call(fn::function_vb_move, {heap + 8, pair});
            call(fn::function_vb_move, {heap + 0x20, pair + 24});
        }
        st32(done + 16, heap);
        call(APIManager_executePartyMemberList, {api2, 1, done, 1});
        functionDestroy(done);
        functionDestroy(pair + 24);
        functionDestroy(pair);
    } else {
        call(PrepareMypage_next, {last, 1});
    }
    if (infoCtl) call(fn::shared_release, {infoCtl});
    for (int k = 7; k >= 0; k--) {
        functionDestroy(b[k]);
        functionDestroy(a[k + 1]);
    }
    functionDestroy(a[0]);
}
RW_FUNCTION(0x46F37C, "SceneUtil::callPrepareMypageAPIs", SceneUtil::callPrepareMypageAPIs);

// Run `callback` when the Android back key is released while `node` is in the scene.
void SceneUtil::addBackKeyCallback(Addr node, Addr callback) {
    uint32_t listener = call(EventListenerKeyboard_create, {});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    uint32_t captured = f.alloc(24);
    functionCopy(captured, cb);
    uint32_t onKey = f.alloc(20);
    st32(onKey + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x20});
    if (heap) {
        st32(heap, VT_BackKey);
        functionCopy(heap + 8, captured);
    }
    st32(onKey + 16, heap);
    call(KeyFunction_swap, {onKey, listener + Listener_onKeyReleased});
    functionDestroy(onKey);
    functionDestroy(captured);
    functionDestroy(cb);
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {ld32(director() + 0x84), listener, node});
}
RW_FUNCTION(0x478BA8, "SceneUtil::addBackKeyCallback", SceneUtil::addBackKeyCallback);

// Send the pending medal lock / unlock changes (only medals the user still owns), then `callback(ok)`.
void SceneUtil::callUpdateLockMedalAPI(Addr callback) {
    uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, callback});
    uint32_t lock = call(AppState_getWillLockUserMedalIDs, {call(fn::AppState_getInstance, {})});
    uint32_t unlock = call(AppState_getWillUnLockUserMedalIDs, {call(fn::AppState_getInstance, {})});
    uint32_t medalsRef = api + APIManager_userMedals;
    if (!ld32(medalsRef) || (ld32(lock + 4) == ld32(lock) && ld32(unlock + 4) == ld32(unlock))) {
        call(FunctionB_call, {callback, 1});
        functionDestroy(cb);
        return;
    }
    // unordered_set<u64>: 4 words then max_load_factor 1.0f
    uint32_t lockSet = f.alloc(20), unlockSet = f.alloc(20), key = f.alloc(8), result = f.alloc(8);
    auto collect = [&](uint32_t set, uint32_t ids) {
        for (uint32_t k = 0; k < 16; k += 4) st32(set + k, 0);
        st32(set + 16, fbits(1.0f));
        uint32_t end = ld32(ids + 4);
        for (uint32_t it = ld32(ids); it != end; it += 8) {
            st32(key, ld32(it));
            st32(key + 4, ld32(it + 4));
            uint32_t node = call(MedalIndex_find, {ld32(medalsRef) + 0x24, key});
            if (node && int32_t(ld32(node + 0x10)) >= 0) call(U64Set_insertUnique, {result, set, it});
        }
    };
    collect(lockSet, lock);
    collect(unlockSet, unlock);
    uint32_t api2 = call(fn::APIManager_getInstance, {});
    uint32_t lockArg = f.alloc(20), unlockArg = f.alloc(20);
    call(U64Set_copy, {lockArg, lockSet});
    call(U64Set_copy, {unlockArg, unlockSet});
    uint32_t captured = f.alloc(24);
    functionCopy(captured, cb);
    uint32_t done = f.alloc(20);
    heapFunction(done, VT_LockMedal_76, 0x20, 8, fn::function_vb_move, captured);
    call(APIManager_executeUserMedalLock, {api2, lockArg, unlockArg, done});
    functionDestroy(done);
    functionDestroy(captured);
    call(U64Set_dtor, {unlockArg});
    call(U64Set_dtor, {lockArg});
    call(U64Set_dtor, {unlockSet});
    call(U64Set_dtor, {lockSet});
    functionDestroy(cb);
}
RW_FUNCTION(0x473DF4, "SceneUtil::callUpdateLockMedalAPI", SceneUtil::callUpdateLockMedalAPI);

// The attribute icon of a boss: a raid boss's from its RaidEnemy row, else the stage's boss enemy (read from
// the stage's map point file).
void SceneUtil::setBossTypeImage(Addr image, int32_t id, uint32_t raid) {
    Frame f;
    if (raid == 1) {
        uint32_t row = masterRow(f, RaidEnemy_create, uint32_t(id));
        int32_t type = safe32(ld32(row), 0x88);
        {
            Frame g;
            uint32_t s = newStr(g, stringBuffer(g, {STR_KeyBladeTypePng, uint32_t(type)}));
            call(fn::ImageView_loadTexture, {image, s, 0});
            strFree(s);
        }
        sharedRelease(row);
        return;
    }
    uint32_t utils = call(FileUtils_getInstance, {});
    uint32_t data = f.alloc(16);
    {
        Frame g;
        uint32_t s = newStr(g, stringBuffer(g, {STR_MappoiBin, uint32_t(id)}));
        vcall(utils, FileUtils_getDataFromFile, {data, utils, s});
        strFree(s);
    }
    int32_t enemy = int32_t(call(StageManager_getInteger32bitFromLittleEndianBytes, {call(Data_getBytes, {data}) + 40}));
    if (!enemy)
        enemy = int32_t(call(StageManager_getInteger32bitFromLittleEndianBytes, {call(Data_getBytes, {data}) + 24}));
    uint32_t row = masterRow(f, Enemy_create, uint32_t(enemy));
    int32_t type = safe32(ld32(row), 0x278);
    {
        Frame g;
        uint32_t s = newStr(g, stringBuffer(g, {STR_KeyBladeTypePng, uint32_t(type)}));
        call(fn::ImageView_loadTexture, {image, s, 0});
        strFree(s);
    }
    sharedRelease(row);
    call(Data_dtor, {data});
}
RW_FUNCTION(0x46C4C4, "SceneUtil::setBossTypeImage", SceneUtil::setBossTypeImage);
