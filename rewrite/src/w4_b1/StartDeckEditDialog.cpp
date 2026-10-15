// StartDeckEditDialog: life cycle, open / close, the start requests and small helpers.
#include "StartDeckEditDialog_lib.hpp"

using namespace sded;

// ------------------------------------------------------------------------------------------------ life cycle
StartDeckEditDialog::Addr StartDeckEditDialog::construct(Addr self) {
    call(Node_ctor, {self});
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    rw::set<uint32_t>(s, 0x290, 0);
    rw::set<uint32_t>(s, 0x2A8, 0);
    rw::set<uint32_t>(s, 0x2C0, 0);
    rw::set<uint16_t>(s, 0x27C, 0);
    rw::set<uint32_t>(s, 0x278, 0);
    rw::set<uint32_t>(s, 0x274, 0);
    rw::set<uint32_t>(s, 0x270, 0);
    rw::set<uint16_t>(s, 0x2BC, 0);
    rw::set<uint32_t>(s, 0x2B8, 0);
    rw::set<uint32_t>(s, 0x2B4, 0);
    rw::set<uint32_t>(s, 0x2B0, 0);
    call(wl::Vec2_ctor, {self + 0x2C4});
    s->avatarIconCache = 0;
    s->useSkipTicket = 0;
    s->field_2D0 = 0;
    call(wl::MEMSET, {self + 0x2D8, 0x14, 0});
    return self;
}
RW_FUNCTION(0x485700, "StartDeckEditDialog::StartDeckEditDialog", StartDeckEditDialog::construct);

StartDeckEditDialog::Addr StartDeckEditDialog::destroy(Addr self) {
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    uint32_t cache = s->avatarIconCache;
    if (cache) call(fn::operator_delete, {call(AvatarIconCache_dtor, {cache})});
    s->avatarIconCache = 0;
    call(CampaignVector_baseDtor, {self + 0x2DC});
    call(wl::Vec2_dtor, {self + 0x2C4});
    functionDestroy(A(s->onClose));
    functionDestroy(A(s->fn_280));
    call(Node_dtor, {self});
    return self;
}
RW_FUNCTION(0x5734F8, "StartDeckEditDialog::~StartDeckEditDialog", StartDeckEditDialog::destroy);

StartDeckEditDialog::Addr StartDeckEditDialog::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    construct(self);
    if (vcall(self, VS_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, vslot::release, {self});
    return 0;
}
RW_FUNCTION(0x5735DE, "StartDeckEditDialog::create", StartDeckEditDialog::create);

// onEnter / onExit callbacks ($_0, $_1)
uint32_t StartDeckEditDialog::init(Addr self) {
    if (call(Node_init, {self}) != 1) return 0;
    {
        Frame f;
        uint32_t fo = inlineFn(f, VT_init_0, {self});
        call(FunctionVV_assign, {self + 0x210, fo});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = inlineFn(f, VT_init_1, {self});
        call(FunctionVV_assign, {self + 0x228, fo});
        functionDestroy(fo);
    }
    return 1;
}
RW_FUNCTION(0x573634, "StartDeckEditDialog::init", StartDeckEditDialog::init);

// Raids: keep the boss info of RaidPanel up to date
void StartDeckEditDialog::update(Addr self, float) {
    Self *s = S(self);
    if (uint32_t(s->stageType.get() - 3) > 1) return;
    uint32_t panel = find(s->root, S_RAID_PANEL, 9);
    if (!panel) return;
    int32_t type = s->stageType;
    if (type == 3) {
        call(AdvStageScrollView_setRaidBossInfo, {panel, ld32(api() + API_RAID_INFO)});
        type = s->stageType;
    }
    if (type != 4) return;
    uint32_t a = api();
    uint32_t begin = ld32(a + API_RAID_LIST), end = ld32(a + API_RAID_LIST + 4);
    uint32_t count = uint32_t(int32_t(end - begin) >> 4) * 0xEEEEEEEFu;   // / 240
    uint32_t idx = uint32_t(s->stageIndex.get());
    if (!(count > idx)) {
        call(wl::VECTOR_AT_THROW, {a + API_RAID_LIST});
        return;
    }
    call(AdvStageScrollView_setRaidBossInfo, {panel, begin + idx * 0xF0});
}
RW_FUNCTION(0x57373C, "StartDeckEditDialog::update", StartDeckEditDialog::update);

// Opens the dialog on `parent` (once): the shadow fades in, the scene is loaded hidden, and the support list is
// requested ($_2 builds the dialog when it arrives).
uint32_t StartDeckEditDialog::open(Addr self, Addr parent, int32_t type, int32_t index, Addr callback) {
    Self *s = S(self);
    if (s->parent) return 0;
    s->parent = parent;
    s->started = 0;
    call(FunctionVB_assign, {self + 0x298, callback});
    s->shadow = call(SceneUtil_createShadowSprite, {});
    uint32_t opacity = vcall(s->shadow, VS_getOpacity, {s->shadow});
    vcall(s->shadow, VS_setOpacity, {s->shadow, 0});
    uint32_t sh = s->shadow;
    call(wl::Node_runAction, {sh, call(FadeTo_create, {fb(0.1f), opacity})});
    vcall(s->shadow, VS_addChild, {s->shadow, self});
    uint32_t p = s->parent;
    vcall(p, VS_addChild, {p, s->shadow});
    s->stageType = type;
    s->notColosseum = type != 2 ? 1 : 0;
    if (type == 2 || type == 4) s->stageIndex = index;
    {
        Frame f;
        uint32_t name = newStr(f, S_SCENE_JSON, 0x32);
        s->root = call(SceneUtil_createNodeWithSceneFile, {name});
        strFree(name);
    }
    uint32_t root = s->root;
    sh = s->shadow;
    uint32_t z = vcall(sh, VS_getLocalZOrder, {sh});
    vcall(sh, VS_addChildZ, {sh, root, z + 1});
    s->field_2D0 = 0;
    vcall(s->root, VS_setVisible, {s->root, 0});
    call(SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    uint32_t a = api();
    Frame f;
    uint32_t fo = inlineFn(f, VT_open_2, {self});
    call(APIManager_executeStageSupportList, {a, fo});
    functionDestroy(fo);
    return 1;
}
RW_FUNCTION(0x573850, "StartDeckEditDialog::open", StartDeckEditDialog::open);

void StartDeckEditDialog::close(Addr self) {
    Self *s = S(self);
    s->closing = 1;
    uint32_t slide = s->deckSlide;
    if (slide) vcall(slide, VS_slide31C, {slide, 1});
    uint32_t root = s->root;
    {
        Frame f;
        uint32_t fo = inlineFn(f, VT_close_18, {self});
        uint32_t anim = call(SceneUtil_createClosePopupAnimation, {root, fo});
        call(wl::Node_runAction, {root, anim});
        functionDestroy(fo);
    }
    uint32_t shadow = s->shadow;
    call(wl::Node_runAction, {shadow, call(FadeTo_create, {fb(0.1f), 0})});
}
RW_FUNCTION(0x57674C, "StartDeckEditDialog::close", StartDeckEditDialog::close);

// Draws a keyblade deck page; the page shown in the pager is also the keyblade the quest starts with.
void StartDeckEditDialog::updateDeckView(Addr self, Addr widget, int32_t deck, uint32_t) {
    uint32_t a = api();
    uint32_t w[22];
    safeWords(ld32(a + API_KEYBLADES), uint32_t(deck) * 0x58, w, 22);
    Self *s = S(self);
    uint32_t slide = s->deckSlide;
    uint32_t ids = ld32(slide + 0x578), idsEnd = ld32(slide + 0x57C);
    uint32_t idx = ld32(slide + 0x5AC);
    if (!(uint32_t(int32_t(idsEnd - ids) >> 2) > idx)) {
        call(wl::VECTOR_AT_THROW, {slide + 0x578});
        return;
    }
    if (int32_t(ld32(ids + idx * 4)) == deck) {
        uint32_t sm = sceneManager();
        st32(sm + SM_KEYBLADE_ID + 4, w[1]);
        st32(sm + SM_KEYBLADE_ID, w[0]);
    }
    Frame f;
    uint32_t fo = inlineFn(f, VT_deckView_13, {});
    call(SceneUtil_updateDeckView, {widget, 0, w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10],
                                    w[11], w[12], w[13], w[14], w[15], w[16], w[17], w[18], w[19], w[20], w[21], fo});
    functionDestroy(fo);
}
RW_FUNCTION(0x575EEC, "StartDeckEditDialog::updateDeckView", StartDeckEditDialog::updateDeckView);

void StartDeckEditDialog::startAdventure(Addr self) {
    Self *s = S(self);
    s->started = 1;
    switch (s->stageType.get()) {
    case 0:
    case 1: startStory(self); break;
    case 2: startColiseum(self); break;
    case 3:
    case 4: startRaid(self); break;
    default: break;
    }
}
RW_FUNCTION(0x576FC4, "StartDeckEditDialog::startAdventure", StartDeckEditDialog::startAdventure);

namespace {
// SceneManager +0x4CC = 1 and its +0x548 std::function<void(bool)> = a captureless lambda
void setSceneResultCallback(uint32_t vtable) {
    uint32_t sm = sceneManager();
    Frame f;
    uint32_t fo = inlineFn(f, vtable, {});
    st8(sm + 0x4CC, 1);
    call(FunctionVB_assign, {sm + 0x548, fo});
    functionDestroy(fo);
}
// hideHeader(empty callback), then changeSceneLayer(lambda, true, true, fade 1)
void hideHeaderAndChange(uint32_t vtable, std::initializer_list<uint32_t> caps) {
    {
        uint32_t sm = sceneManager();
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(SceneManager_hideHeader, {sm, empty});
        functionDestroy(empty);
    }
    uint32_t sm = sceneManager();
    Frame f;
    uint32_t fo = inlineFn(f, vtable, caps);
    call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
}
}  // namespace

void StartDeckEditDialog::startStory(Addr self) {
    st32(sceneManager() + 0x30C, 0);
    setSceneResultCallback(VT_story_23);
    uint32_t a = api();
    uint32_t stage = ld32(sceneManager() + SM_STAGE_ID);
    uint32_t sm = sceneManager();
    uint32_t supHi = ld32(sm + SM_SUPPORT_ID + 4), supLo = ld32(sm + SM_SUPPORT_ID);
    sm = sceneManager();
    uint32_t kbLo = ld32(sm + SM_KEYBLADE_ID), kbHi = ld32(sm + SM_KEYBLADE_ID + 4);
    uint32_t flag = S(self)->useSkipTicket;
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(APIManager_executeStageStart, {a, stage, supLo, supHi, kbLo, kbHi, flag, empty});
        functionDestroy(empty);
    }
    if (ld32(sceneManager() + 0x28C) == 0) st32(sceneManager() + 0x28C, 1);
    hideHeaderAndChange(VT_story_24, {self});
}
RW_FUNCTION(0x577010, "StartDeckEditDialog::startStory", StartDeckEditDialog::startStory);

void StartDeckEditDialog::startColiseum(Addr self) {
    st32(sceneManager() + 0x30C, 2);
    setSceneResultCallback(VT_coliseum_27);
    uint32_t a = api();
    uint32_t v2D8 = ld32(sceneManager() + 0x2D8);
    uint32_t v2DC = ld32(sceneManager() + 0x2DC);
    uint32_t stage = ld32(sceneManager() + SM_STAGE_ID);
    uint32_t sm = sceneManager();
    uint32_t kbLo = ld32(sm + SM_KEYBLADE_ID), kbHi = ld32(sm + SM_KEYBLADE_ID + 4);
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(APIManager_executeColosseumStart, {a, v2D8, v2DC, stage, kbLo, kbHi, empty});
        functionDestroy(empty);
    }
    st32(sceneManager() + 0x28C, 5);
    hideHeaderAndChange(VT_coliseum_28, {self});
}
RW_FUNCTION(0x5771E8, "StartDeckEditDialog::startColiseum", StartDeckEditDialog::startColiseum);

void StartDeckEditDialog::startRaid(Addr) {
    st32(sceneManager() + 0x30C, 3);
    setSceneResultCallback(VT_raid_25);
    uint32_t a = api();
    uint32_t sm = sceneManager();
    uint32_t v2D4 = ld32(sm + 0x2D4), v2D0 = ld32(sm + 0x2D0);
    sm = sceneManager();
    uint32_t supLo = ld32(sm + SM_SUPPORT_ID), supHi = ld32(sm + SM_SUPPORT_ID + 4);
    sm = sceneManager();
    uint32_t kbLo = ld32(sm + SM_KEYBLADE_ID), kbHi = ld32(sm + SM_KEYBLADE_ID + 4);
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        // (uint64 raid, uint64 support, uint64 keyblade, callback); r1 holds a leftover (the keyblade id low word)
        call(APIManager_executeRaidStart, {a, kbLo, v2D0, v2D4, supLo, supHi, kbLo, kbHi, empty});
        functionDestroy(empty);
    }
    hideHeaderAndChange(VT_raid_26, {});
}
RW_FUNCTION(0x5773A4, "StartDeckEditDialog::startRaid", StartDeckEditDialog::startRaid);

// ------------------------------------------------------------------------------------------------ tutorials
namespace {
// AppState::setTutorialCloseFunc(SceneUtil::openTutorialPopup(id, done, w1, w2, w3, false), id)
void tutorialPopup(uint32_t id, uint32_t done, uint32_t w1, uint32_t w2) {
    uint32_t app = call(AppState_getInstance, {});
    Frame f;
    uint32_t copy = f.alloc(24);
    if (done) {
        call(FunctionVV_copy, {copy, done});
    } else {
        st32(copy + 16, 0);
    }
    uint32_t popup = f.alloc(24), prev = f.alloc(24);
    call(SceneUtil_openTutorialPopup, {popup, id, copy, w1, w2, 0, 0});
    call(AppState_setTutorialCloseFunc, {prev, app, popup, id});
    functionDestroy(prev);
    functionDestroy(popup);
    functionDestroy(copy);
}
uint32_t okButton(uint32_t self) {
    uint32_t button = find(S(self)->root, S_BUTTON, 6);
    return find(button, S_BUTTON_OK, 9);
}
uint32_t supportArea(uint32_t self) {
    uint32_t support = find(S(self)->root, S_SUPPORT, 7);
    return find(support, S_AREA, 4);
}
}  // namespace

void StartDeckEditDialog::openTutorial106(Addr self, Addr done) {
    uint32_t ok = okButton(self);
    tutorialPopup(106, done, ok, 0);
}
RW_FUNCTION(0x5757A0, "StartDeckEditDialog::openTutorial106", StartDeckEditDialog::openTutorial106);

void StartDeckEditDialog::openTutorial107(Addr self, Addr done) {
    uint32_t area = supportArea(self);
    tutorialPopup(107, done, area, 0);
}
RW_FUNCTION(0x575948, "StartDeckEditDialog::openTutorial107", StartDeckEditDialog::openTutorial107);

// (the callback is not used: the popup gets an empty one)
void StartDeckEditDialog::openTutorial108(Addr self, Addr) {
    uint32_t ok = okButton(self);
    tutorialPopup(108, 0, ok, 0);
}
RW_FUNCTION(0x575AF0, "StartDeckEditDialog::openTutorial108", StartDeckEditDialog::openTutorial108);

void StartDeckEditDialog::openTutorial112(Addr self, Addr done) {
    uint32_t ok = okButton(self);
    uint32_t area = supportArea(self);
    tutorialPopup(112, done, ok, area);
}
RW_FUNCTION(0x575C88, "StartDeckEditDialog::openTutorial112", StartDeckEditDialog::openTutorial112);

// ------------------------------------------------------------------------------------------------ cost
// The deck cost of the shown keyblade (SceneUtil::calcDeckStatus) is over the player's cost limit
uint32_t StartDeckEditDialog::isCostOverCurrentKeyblade(Addr self) {
    // stale words of the original frame (entry sp - 0x1C0) passed in uninitialised struct padding
    uint32_t frame = rw::cpu->r[13] - 0x1C0;
    uint32_t kbArgPad = rw::load<uint32_t>(frame + 4);
    uint32_t kbPad14 = rw::load<uint32_t>(frame + 0x104);
    uint32_t kbPad4C = rw::load<uint32_t>(frame + 0x13C);
    uint32_t kbPad54 = rw::load<uint32_t>(frame + 0x144);
    uint32_t medalPad2C = rw::load<uint32_t>(frame + 0xD4);
    Self *s = S(self);
    uint32_t a = api();
    uint32_t slide = s->deckSlide;
    uint32_t ids = ld32(slide + 0x578), idsEnd = ld32(slide + 0x57C);
    uint32_t idx = ld32(slide + 0x5AC);
    if (!(uint32_t(int32_t(idsEnd - ids) >> 2) > idx)) {
        call(wl::VECTOR_AT_THROW, {slide + 0x578});
        return 0;
    }
    uint32_t kb[22];
    safeWords(ld32(a + API_KEYBLADES), ld32(ids + idx * 4) * 0x58, kb, 22);
    uint32_t kdm = call(KeyBladeDataManager_getInstance, {});
    // UserKeyblade::Keyblade{} (bytes 0x14..0x17, 0x4D..0x4F and 0x54..0x57 are not cleared)
    uint32_t k[22] = {};
    k[5] = kbPad14;
    k[19] = kbPad4C & 0xFFFFFF00u;
    k[21] = kbPad54;
    Frame f;
    uint32_t kbData = f.alloc(8);
    call(KeyBladeDataManager_getKeyBladeData,
         {kbData, kdm, kb[0], kb[1], 0, kbArgPad, k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9], k[10],
          k[11], k[12], k[13], k[14], k[15], k[16], k[17], k[18], k[19], k[20], k[21]});
    uint32_t medals = f.alloc(12);
    st32(medals, 0);
    st32(medals + 4, 0);
    st32(medals + 8, 0);
    uint32_t count = kb[4];
    for (uint32_t n = 0; n < count; n++) {
        uint32_t mdm = call(MedalDataManager_getInstance, {});
        uint32_t lo = kb[6 + n * 2], hi = kb[7 + n * 2];
        uint32_t m[18] = {};
        m[11] = medalPad2C;
        uint32_t medal = f.alloc(8);
        call(MedalDataManager_getMedalData, {medal, mdm, lo, hi, 0, kbArgPad, m[0], m[1], m[2], m[3], m[4], m[5], m[6],
                                             m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15], m[16], m[17]});
        uint32_t end = ld32(medals + 4);
        if (end == ld32(medals + 8)) {
            call(MedalVector_pushBackSlow, {medals, medal});
        } else {
            if (end) {
                st32(end, ld32(medal));
                uint32_t ctrl = ld32(medal + 4);
                st32(end + 4, ctrl);
                addRef(ctrl);
            }
            st32(medals + 4, ld32(medals + 4) + 8);
        }
        release(ld32(medal + 4));
    }
    uint32_t status = call(wl::NEW_ARRAY, {0xC});
    uint32_t copy = f.alloc(8);
    st32(copy, ld32(kbData));
    st32(copy + 4, ld32(kbData + 4));
    addRef(ld32(copy + 4));
    call(SceneUtil_calcDeckStatus, {copy, medals, status, 0});
    release(ld32(copy + 4));
    int32_t cost = int32_t(ld32(status + 8));
    call(wl::DELETE_ARRAY, {status});
    int32_t limit = scene::safe32(ld32(api() + API_USER_DETAIL), 0x1C);
    call(MedalVector_baseDtor, {medals});
    release(ld32(kbData + 4));
    return limit < cost ? 1 : 0;
}
RW_FUNCTION(0x577F3C, "StartDeckEditDialog::isCostOverCurrentKeyblade", StartDeckEditDialog::isCostOverCurrentKeyblade);
