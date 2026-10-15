// SceneAdventureSelect: life cycle, per-frame updates and small helpers.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

// ------------------------------------------------------------------------------------------------ life cycle
SceneAdventureSelect::Addr SceneAdventureSelect::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    construct(self);
    if (vcall(self, vslot::init, {self}) == 1) {
        call(lib::Ref_autorelease, {self});
        return self;
    }
    vcall(self, vslot::deleteSelf, {self});
    return 0;
}
RW_FUNCTION(0x579EB8, "SceneAdventureSelect::create", SceneAdventureSelect::create);

// The constructor: the Layer base, both vtables, every field cleared (flag_325 starts at 1), the Vec2 members
// constructed in declaration order.
SceneAdventureSelect::Addr SceneAdventureSelect::construct(Addr self) {
    call(lib::Layer_ctor, {self});
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->inOutVtable = INOUT_VTABLE;
    s->adventureType = 0;
    s->raidInfoActive = 0;
    s->flag_2A0 = 0;
    rw::set<uint32_t>(s, 0x29C, 0);              // flag_29C..flag_29F
    s->field_298 = 0;
    call(lib::MEMSET, {self + 0x2A4, 0x18, 0});  // root .. buttonFilter
    call(lib::Vec2_ctor, {self + 0x2BC});        // announce
    s->announce.node = 0;
    s->field_2C8 = 0;
    s->leftUiBack = 0;
    call(lib::Vec2_ctor, {self + 0x2D0});        // leftUiBack position
    s->scrollMoving = 0;
    s->scrollLeft = 0;
    s->scrollInfoOut = 0;
    s->scrollTime = 0.0f;
    rw::set<uint32_t>(s, 0x2E0, 0);              // scrollSpeed
    s->flag_2E4 = 0;
    call(lib::Vec2_ctor, {self + 0x2E8});        // cursor
    s->cursor.node = 0;
    call(lib::Vec2_ctor, {self + 0x2F4});        // cursorArrow
    s->rightArrow = 0;
    s->leftArrow = 0;
    s->stageScrollView = 0;
    s->cursorArrow.node = 0;
    call(lib::Vec2_ctor, {self + 0x30C});
    call(lib::Vec2_ctor, {self + 0x314});        // left arrow position
    call(lib::Vec2_ctor, {self + 0x31C});        // right arrow position
    s->flag_324 = 0;
    s->flag_325 = 1;
    s->eventIndex = 0;
    s->field_32C = 0;
    call(lib::Vec2_ctor, {self + 0x330});        // eventSelect
    s->eventSelect.node = 0;
    call(lib::Vec2_ctor, {self + 0x33C});        // eventSelectLeftUI
    s->eventSelectLeftUI.node = 0;
    s->eventTimerOn = 0;
    call(lib::MEMSET, {self + 0x34C, 0x28, 0});  // bannerNames .. deferment
    for (uint32_t panel = 0x374; panel < 0x3F8; panel += 12) {   // advSelect .. filterText
        call(lib::Vec2_ctor, {self + panel});
        if (panel != 0x3EC) rw::set<uint32_t>(s, panel + 8, 0);
    }
    rw::set<uint16_t>(s, 0x3FC, 0);              // flag_3FC, flag_3FD
    s->filterText.node = 0;
    s->cursorAnimAP = 0;
    return self;
}
RW_FUNCTION(0x579F0C, "SceneAdventureSelect::SceneAdventureSelect", SceneAdventureSelect::construct);

SceneAdventureSelect::Addr SceneAdventureSelect::createScene() {
    uint32_t sceneObj = call(lib::Scene_create, {});
    uint32_t layer = create();
    vcall(sceneObj, node::addChild, {sceneObj, layer});
    return sceneObj;
}
RW_FUNCTION(0x71D378, "SceneAdventureSelect::createScene", SceneAdventureSelect::createScene);

SceneAdventureSelect::Addr SceneAdventureSelect::destroy(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->inOutVtable = INOUT_VTABLE;
    for (uint32_t panel = 0x3EC; panel >= 0x374; panel -= 12) call(lib::Vec2_dtor, {self + panel});
    call(lib::DateVector_dtor, {self + 0x358});
    call(lib::StringVector_dtor, {self + 0x34C});
    call(lib::Vec2_dtor, {self + 0x33C});
    call(lib::Vec2_dtor, {self + 0x330});
    call(lib::Vec2_dtor, {self + 0x31C});
    call(lib::Vec2_dtor, {self + 0x314});
    call(lib::Vec2_dtor, {self + 0x30C});
    call(lib::Vec2_dtor, {self + 0x2F4});
    call(lib::Vec2_dtor, {self + 0x2E8});
    call(lib::Vec2_dtor, {self + 0x2D0});
    call(lib::Vec2_dtor, {self + 0x2BC});
    call(lib::Layer_dtor, {self});
    return self;
}
RW_FUNCTION(0x71D39C, "SceneAdventureSelect::~SceneAdventureSelect", SceneAdventureSelect::destroy);

void SceneAdventureSelect::destroyDelete(Addr self) {
    destroy(self);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D4C4, "SceneAdventureSelect::~SceneAdventureSelect (deleting)", SceneAdventureSelect::destroyDelete);

// ------------------------------------------------------------------------------------------------ API
// APIManager::executeCampaignGet with a lambda ($_0) that holds a copy of `callback` (std::function<void(bool)>).
void SceneAdventureSelect::callAdventureSelectAPI(Addr callback) {
    uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    uint32_t copy = f.alloc(20);
    call(fn::function_vb_copy, {copy, callback});
    uint32_t onDone = f.alloc(20);
    heapFunction(onDone, VT_callAdventureSelectAPI_0, 0x28, 0x10, fn::function_vb_move, copy);
    call(lib::APIManager_executeCampaignGet, {api, onDone});
    functionDestroy(onDone);
    functionDestroy(copy);
}
RW_FUNCTION(0x71D260, "SceneAdventureSelect::callAdventureSelectAPI", SceneAdventureSelect::callAdventureSelectAPI);

// ------------------------------------------------------------------------------------------------ update
// Every frame: the raid boss's time left, the event's time left, then the held-arrow scroll of the stage list
// (after 0.3 s the info panel slides out and the list migrates with a growing step).
void SceneAdventureSelect::update(Addr self, float dt) {
    Self *s = rw::host_ptr<Self>(self);
    updateStageRaidChack(self, dt);
    updateEventTimeChack(self, dt);
    if (!s->scrollMoving) return;
    float held = s->scrollTime;
    if (held < 0.3f) {
        s->scrollTime = held + dt;
        return;
    }
    if (!s->scrollInfoOut) {
        s->scrollInfoOut = 1;
        infoOutAnimation(self);
    }
    int32_t step = s->scrollSpeed;
    if (step <= 4) s->scrollSpeed = ++step;
    uint32_t moving = s->scrollLeft ? call(lib::BaseStageScrollView_migrationLeft, {s->stageScrollView, uint32_t(step)})
                                    : call(lib::BaseStageScrollView_migrationRight, {s->stageScrollView, uint32_t(step)});
    s->scrollMoving = uint8_t(moving);
}
RW_FUNCTION(0x71F754, "SceneAdventureSelect::update", SceneAdventureSelect::update);

// The raid panel's RaidInfo: refresh the boss's remaining time.
void SceneAdventureSelect::updateStageRaidChack(Addr self, float) {
    Self *s = rw::host_ptr<Self>(self);
    if (!s->raidInfoActive) return;
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t raidNode = s->raid.node;
    if (!raidNode) return;
    uint32_t info = findNode(raidNode, STR_RaidInfo, 8);
    call(lib::AdvStageScrollView_updateRaidBossInfoTime, {info, ld32(api + API_RAID_INFO)});
}
RW_FUNCTION(0x71F80C, "SceneAdventureSelect::updateStageRaidChack", SceneAdventureSelect::updateStageRaidChack);

// The event panel's Txt_Limit: counted down, then refreshed every second (the last hour) or every 59.9 s.
void SceneAdventureSelect::updateEventTimeChack(Addr self, float dt) {
    Self *s = rw::host_ptr<Self>(self);
    if (!s->eventTimerOn) return;
    float left = s->eventTimer - dt;
    s->eventTimer = left;
    if (left > 0.0f) return;
    uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_STAGE_EVENTS);
    uint32_t index = uint32_t(s->eventIndex);
    uint32_t begin = ld32(events);
    if (eventCount(events) <= index) call(lib::THROW_OUT_OF_RANGE, {events});
    int32_t seconds = int32_t(call(lib::Date_calculateIntervalSinceNow, {begin + index * EVENT_SIZE + EVENT_END_DATE}));
    uint32_t limit = findNode(s->event.node, STR_Txt_Limit, 9);
    Frame f;
    uint32_t text = f.alloc(12);
    call(lib::createTimeLeftString, {text, uint32_t(seconds)});
    setTextC(limit, strData(text));
    strFree(text);
    s->eventTimer = seconds < 3601 ? 1.0f : 59.9f;
}
RW_FUNCTION(0x71F8CC, "SceneAdventureSelect::updateEventTimeChack", SceneAdventureSelect::updateEventTimeChack);

// ------------------------------------------------------------------------------------------------ small helpers
uint32_t SceneAdventureSelect::isScroll(Addr self) {
    uint32_t view = rw::host_ptr<Self>(self)->stageScrollView;
    if (!view) return 0;
    return call(lib::BaseStageScrollView_isScroll, {view});
}
RW_FUNCTION(0x727C14, "SceneAdventureSelect::isScroll", SceneAdventureSelect::isScroll);

// The colosseum tutorial popup (24) was seen.
uint32_t SceneAdventureSelect::openColosseumCheck(Addr) {
    call(fn::APIManager_getInstance, {});
    uint32_t app = call(fn::AppState_getInstance, {});
    return call(lib::AppState_isClearTutorialPopupIDs, {app, 24});
}
RW_FUNCTION(0x727114, "SceneAdventureSelect::openColosseumCheck", SceneAdventureSelect::openColosseumCheck);

void SceneAdventureSelect::eventBannerInAnimation(Addr self) {
    call(lib::AdventurePanel_runLeftInAnimation, {self + 0x330});   // eventSelect
    call(lib::AdventurePanel_runLeftInAnimation, {self + 0x33C});   // eventSelectLeftUI
    backButtonRunInAnimation(self);
}
RW_FUNCTION(0x723CEC, "SceneAdventureSelect::eventBannerInAnimation", SceneAdventureSelect::eventBannerInAnimation);
