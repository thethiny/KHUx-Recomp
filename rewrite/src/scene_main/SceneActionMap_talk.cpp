// SceneActionMap: the talk mode (NPC talk windows and their line queues), the interlude after a stage (the clear
// request and the move to the result scene) and the map tutorial hook.
#include "SceneActionMap.hpp"

using namespace smain;
using rw::guest_addr;

namespace {

namespace g {
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t TextData_format = 0x492929;            // (std::string out, Type, int id, ...)
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t string_find_char = 0x601CD1;           // std::string::find(char, pos)
constexpr uint32_t string_substr_init = 0x47CD4D;         // std::string(const string&, pos, n, alloc)
constexpr uint32_t SpeechDeque_push_back = 0x601D05;      // deque<string>::push_back(string&&)
constexpr uint32_t SpeechDeque_dtor = 0x601D95;           // ~__deque_base
constexpr uint32_t SpeechMap_insert = 0x603C09;           // __hash_table::__insert_unique(pair<SpeechDisp*, deque>&&)
constexpr uint32_t SpeechMap_at = 0x601971;
constexpr uint32_t SpeechMap_clear = 0x605285;
constexpr uint32_t MapNonPlayableCharacter_get_talk = 0x5DC299;
constexpr uint32_t MapNonPlayableCharacter_show_talkicon = 0x5DC287;
constexpr uint32_t SceneActionMap_makeTalkWindow = 0x60136D;
constexpr uint32_t SceneActionMap_updateFieldMode = 0x5FE9F9;
constexpr uint32_t StageActorManager_update = 0x74FF25;
constexpr uint32_t StageFieldManager_terminateCollectPrize = 0x7575F1;
constexpr uint32_t StageManager_finish = 0x75945D;
constexpr uint32_t StageUI_getMedalField = 0x75E65D;
constexpr uint32_t StageUI_createTutorialWidget = 0x760EC9;
constexpr uint32_t StageUI_setTutorialWidgetOnPropagetion = 0x761539;
constexpr uint32_t SceneUtil_dispTutorialCheck = 0x472D75;
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815;
constexpr uint32_t AppState_addClearTutorialPopupIDs = 0x4EB7A9;
constexpr uint32_t TutorialMisc_create = 0x777BB5;
constexpr uint32_t APIManager_executeStageClear = 0x4CB771;
constexpr uint32_t APIManager_executeColosseumClear = 0x4D1955;
constexpr uint32_t APIManager_executeColosseumRetire = 0x4D17A5;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t createSceneLayer_SceneBattleResult = 0x601365;
constexpr uint32_t createSceneLayer_SceneAdventureSelect = 0x579EB1;
constexpr uint32_t function_vb_assign = 0x490109;          // std::function<void(bool)>::operator=
constexpr uint32_t function_vv_assign = 0x4EB9CD;
constexpr uint32_t function_b_call = 0x51022D;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;      // (shared_ptr<Layer> out, manager, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;          // (layer, ResourceID, int, int)
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t ScaleTo_create = 0x86A8CD;              // (duration, scale)
constexpr uint32_t ScaleTo_create_xy = 0x86A981;           // (duration, sx, sy)
constexpr uint32_t EaseBackIn_create = 0x8615C9;
constexpr uint32_t FadeTo_create = 0x86B3E1;
constexpr uint32_t EaseExponentialIn_create = 0x8608F9;
constexpr uint32_t Spawn_createWithTwoActions = 0x86838D;
constexpr uint32_t Sequence_create = 0x867999;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t shared_release = 0xC8DBD1;
constexpr uint32_t SE_TALK_CLOSE = 0x1E5841B6;             // audio::ResourceID
}  // namespace g

constexpr uint32_t VT_changedMapTutorial_47 = 0xF5E200;
constexpr uint32_t VT_changedMapTutorial_48 = 0xF5E1D4;
constexpr uint32_t VT_updateTalkMode_35 = 0xF5E4EC;
constexpr uint32_t VT_updateTalkMode_34 = 0xF5E518;
constexpr uint32_t VT_updateIntrludeMode_33 = 0xF5E544;
constexpr uint32_t VT_updateIntrludeMode_32 = 0xF5E570;
constexpr uint32_t VT_LayerFactory = 0xF7E130;             // __func<Layer*(*)()> + 8

// a 4-byte field of a TutorialMisc record (shared_ptr released after the read)
int32_t tutorialMisc(uint32_t id, uint32_t offset) {
    Frame f;
    uint32_t sp = f.alloc(8);
    call(g::TutorialMisc_create, {sp, id});
    uint32_t out = f.alloc(8);
    uint32_t m = ld32(sp);
    call(g::SafeBuffer_read, {m, out, ld32(m + 0xC) + offset, 4, offset});
    if (ld32(sp + 4)) call(g::shared_release, {ld32(sp + 4)});
    return int32_t(ld32(out));
}

// SceneManager::changeSceneLayer(createSceneLayer<T>, fade in, fade out, fade type 1)
void changeScene(uint32_t factory) {
    uint32_t sm = call(g::SceneManager_getInstance, {});
    Frame f;
    uint32_t fn = newFunction(f, VT_LayerFactory, {factory});
    call(g::SceneManager_changeSceneLayer, {sm, fn, 1, 1, 1});
    functionDestroy(fn);
}

// SceneManager +0x4CC = 1 and its result callback (+0x548) cleared
void clearSceneManagerCallback() {
    uint32_t sm = call(g::SceneManager_getInstance, {});
    Frame f;
    uint32_t empty = newEmptyFunction(f);
    st8(sm + 0x4CC, 1);
    call(g::function_vb_assign, {sm + 0x548, empty});
    functionDestroy(empty);
}

}  // namespace

// The map tutorial (popup 103): when the stage's map (+0x60) and position (+0x148) match TutorialMisc 10061 and
// the popup was not shown yet, a tutorial widget asks for a tap on the marked area; the explore manager then
// continues with executingTutorial1006.
void SceneActionMap::changedMapTutorial(SceneActionMap *self) {
    uint32_t a = guest_addr(self);
    if (call(g::SceneUtil_dispTutorialCheck, {0x67}) != 1) return;
    int32_t map = rw::load<int32_t>(self->stageManager + 0x60);
    if (map != tutorialMisc(0x274D, 0x10)) return;
    int32_t pos = rw::load<int32_t>(self->stageManager + 0x148);
    if (pos != tutorialMisc(0x274D, 0xC)) return;
    if (call(g::AppState_isClearTutorialPopupIDs, {call(g::AppState_getInstance, {}), 0x67})) return;
    uint32_t widget = call(g::StageUI_createTutorialWidget, {self->stageUI, 0x67});
    if (widget == 0) return;
    {
        Frame f;
        uint32_t fn = newFunction(f, VT_changedMapTutorial_47, {a});
        call(g::StageUI_setTutorialWidgetOnPropagetion, {self->stageUI, fn});
        functionDestroy(fn);
    }
    vcall(a, node::addChildZ, {a, widget, 0x14});
    call(g::AppState_addClearTutorialPopupIDs, {call(g::AppState_getInstance, {}), 0x67});
    {
        Frame f;
        uint32_t explore = self->exploreManager;
        uint32_t fn = newFunction(f, VT_changedMapTutorial_48, {a});
        call(g::function_vv_assign, {explore + 0x28, fn});
        functionDestroy(fn);
    }
    lockUI(self, 1);
}

// The talk text `id` (TextData type 1, id taken modulo 200000000) split at form feeds into the talk window's line
// queue; the window shows the first line.
void SceneActionMap::setTalkText(SceneActionMap *self, int32_t id) {
    uint32_t a = guest_addr(self);
    Frame f;
    uint32_t text = f.alloc(12);
    int32_t key = id % 200000000;
    call(g::TextData_format, {text, 1, uint32_t(key + 200000000)});
    uint32_t lines = f.alloc(0x18);
    call(g::aeabi_memset, {lines, 0x18, 0});
    uint32_t pos = 0, end;
    do {
        end = call(g::string_find_char, {text, 0xC, pos});
        uint32_t n = end == 0xFFFFFFFFu ? end : end - pos;
        Frame g2;
        uint32_t line = g2.alloc(12);
        call(g::string_substr_init, {line, text, pos, n, text});
        call(g::SpeechDeque_push_back, {lines, line});
        strFree(line);
        pos = end + 1;
    } while (end != 0xFFFFFFFFu);
    // pair<SpeechDisp* const, deque<string>> (the deque moved out of `lines`)
    uint32_t pair = f.alloc(0x1C);
    st32(pair, self->talkWindow);
    for (uint32_t k = 0; k < 4; k++) st32(pair + 4 + 4 * k, ld32(lines + 4 * k));
    for (uint32_t k = 0; k < 4; k++) st32(lines + 4 * k, 0);
    st32(pair + 0x14, ld32(lines + 0x10));
    st32(pair + 0x18, ld32(lines + 0x14));
    st32(lines + 0x10, 0);
    st32(lines + 0x14, 0);
    uint32_t result = f.alloc(8);
    call(g::SpeechMap_insert, {result, a + 0x37C, pair});
    call(g::SpeechDeque_dtor, {pair + 4});
    uint32_t q = call(g::SpeechMap_at, {a + 0x37C, a + 0x378});
    if (ld32(q + 0x14)) {
        // the front string: block start / 341 (341 strings of 12 bytes per block), slot start % 341
        uint32_t start = ld32(q + 0x10);
        uint32_t line = ld32(ld32(q + 4) + (start / 341) * 4) + (start % 341) * 12;
        uint32_t window = self->talkWindow;
        vcall(window, 0x2A8, {window, line});
    }
    call(g::SpeechDeque_dtor, {lines});
    strFree(text);
}

// Talk mode (mode 5), driven by the stage's talk state (StageManager +0x1C): 0 open the talker's window, 1-5
// follow the talker with the camera, 6 close the window (SE, scale / fade out), 8 back to the field.
void SceneActionMap::updateTalkMode(SceneActionMap *self, float dt) {
    uint32_t a = guest_addr(self);
    uint32_t stage = self->stageManager;
    switch (ld32(stage + 0x1C)) {
    case 0: {
        if (ld32(self->fieldManager + 0xD4) == 0) break;
        call(g::SpeechMap_clear, {a + 0x37C});
        uint32_t talk = call(g::MapNonPlayableCharacter_get_talk, {ld32(self->fieldManager + 0xD4)});
        uint32_t state = call(g::SceneActionMap_makeTalkWindow, {a, talk});
        st32(self->stageManager + 0x1C, state);
        call(g::MapNonPlayableCharacter_show_talkicon, {ld32(self->fieldManager + 0xD4), 0});
        break;
    }
    case 1: case 2: case 3: case 4: case 5:
        updateTalkCameraPosition(self, dt, 0.5f);
        break;
    case 6: {
        st32(stage + 0x1C, 7);
        {
            Frame f;
            uint32_t layer = f.alloc(8);
            call(g::audio_Manager_getLayer, {layer, call(g::audio_Manager_getInstance, {}), 0});
            call(g::audio_Layer_playSE, {ld32(layer), g::SE_TALK_CLOSE, 0, 0});
            if (ld32(layer + 4)) call(g::shared_release, {ld32(layer + 4)});
        }
        uint32_t layer = rw::get<uint32_t>(self, 0x36C);
        vcall(layer, node::removeChild, {layer, rw::get<uint32_t>(self, 0x374), 1});
        layer = rw::get<uint32_t>(self, 0x36C);
        vcall(layer, node::removeChild, {layer, self->talkWindow, 1});
        uint32_t delay = call(g::DelayTime_create, {0x3E088889});                     // 0.1333 s
        uint32_t shrink = call(g::EaseBackIn_create, {call(g::ScaleTo_create, {0x3E888889, fbits(0.5f)})});
        uint32_t fade = call(g::EaseExponentialIn_create, {call(g::FadeTo_create, {0x3E888889, 0})});
        uint32_t done;
        {
            Frame f;
            uint32_t fn = newFunction(f, VT_updateTalkMode_34, {a});
            done = call(g::CallFunc_create, {fn});
            functionDestroy(fn);
        }
        uint32_t spawn = call(g::Spawn_createWithTwoActions, {shrink, fade});
        uint32_t node = rw::get<uint32_t>(self, 0x36C);
        call(g::Node_runAction, {node, call(g::Sequence_create, {delay, spawn, done, 0})});
        uint32_t squash = call(g::ScaleTo_create_xy, {0x3E088889, fbits(0.0f), fbits(1.0f)});
        uint32_t removed;
        {
            Frame f;
            uint32_t fn = newFunction(f, VT_updateTalkMode_35, {a});
            removed = call(g::CallFunc_create, {fn});
            functionDestroy(fn);
        }
        uint32_t frame = rw::get<uint32_t>(self, 0x370);
        call(g::Node_runAction, {frame, call(g::Sequence_create, {squash, removed, 0})});
        break;
    }
    case 8:
        st32(stage + 0x18, 2);
        vcall(a, node::removeChild, {a, rw::get<uint32_t>(self, 0x36C), 1});
        call(g::MapNonPlayableCharacter_show_talkicon, {ld32(self->fieldManager + 0xD4), 1});
        break;
    default:
        break;
    }
    call(g::StageActorManager_update, {self->actorManager, fbits(dt)});
}

// The interlude (mode 3): the field keeps running while the stage was tapped; when the wait (+0x360) runs out
// the stage finishes, the clear (or colosseum clear / retire) request goes out and the scene moves on to the
// battle result (or back to the adventure select after a retire).
void SceneActionMap::updateIntrludeMode(SceneActionMap *self, float dt) {
    uint32_t a = guest_addr(self);
    if (self->intrludeTapped) call(g::SceneActionMap_updateFieldMode, {a, fbits(dt)});
    float t = rw::get<float>(self, 0x360) - dt;
    rw::set<float>(self, 0x360, t);
    if (!(t <= 0.0f)) return;
    if (self->ready == 0) return;
    self->ready = 0;
    call(g::StageFieldManager_terminateCollectPrize, {self->fieldManager, ld32(self->actorManager + 0x20)});
    uint32_t stage = self->stageManager;
    call(g::StageManager_finish, {stage, call(g::StageUI_getMedalField, {self->stageUI})});
    uint32_t kind = ld32(self->stageManager + 0x14);
    if (kind == 1) {
        clearSceneManagerCallback();
        uint32_t ok = call(g::function_b_call, {a + 0x340});
        uint32_t api = call(g::APIManager_getInstance, {});
        if (ok == 1) {
            {
                Frame f;
                uint32_t empty = newEmptyFunction(f);
                call(g::APIManager_executeColosseumClear, {api, empty});
                functionDestroy(empty);
            }
            changeScene(g::createSceneLayer_SceneBattleResult);
        } else {
            {
                Frame f;
                uint32_t fn = f.alloc(0x18);
                st32(fn, VT_updateIntrludeMode_33);
                st32(fn + 0x10, fn);
                call(g::APIManager_executeColosseumRetire, {api, fn});
                functionDestroy(fn);
            }
            changeScene(g::createSceneLayer_SceneAdventureSelect);
            st8(call(g::SceneManager_getInstance, {}) + 0x30A, 1);
        }
    } else if (kind == 0) {
        clearSceneManagerCallback();
        uint32_t api = call(g::APIManager_getInstance, {});
        {
            Frame f;
            uint32_t fn = f.alloc(0x18);
            st32(fn, VT_updateIntrludeMode_32);
            st32(fn + 0x10, fn);
            call(g::APIManager_executeStageClear, {api, fn});
            functionDestroy(fn);
        }
        changeScene(g::createSceneLayer_SceneBattleResult);
    }
}

RW_FUNCTION(0x601EEC, "SceneActionMap::changedMapTutorial", SceneActionMap::changedMapTutorial);
RW_FUNCTION(0x601A14, "SceneActionMap::setTalkText", SceneActionMap::setTalkText);
RW_FUNCTION(0x5FF254, "SceneActionMap::updateTalkMode", SceneActionMap::updateTalkMode);
RW_FUNCTION(0x5FEEA4, "SceneActionMap::updateIntrludeMode", SceneActionMap::updateIntrludeMode);
