// StageDirection: life cycle, the stage start fade, the stage name panel, the finish / colosseum titles, the
// "all in 1 turn" colosseum challenge and the lambda bodies (see StageDirection.hpp; cocos_start is in
// StageDirection_start.cpp).
#include "StageDirection.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using namespace sdir;

namespace {
// lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_LAMBDA_INIT_ENTER = 0xF6EB70;       // init: onEnter callback [this]
constexpr uint32_t VT_LAMBDA_INIT_EXIT = 0xF6EB44;        // init: onExit callback [this]
constexpr uint32_t VT_LAMBDA_STAGE_START = 0xF6EB18;      // stage_start: [this, arg] after the fade
constexpr uint32_t VT_LAMBDA_FINISH_SE = 0xF6E9E4;        // cocos_finish $_9: sound
constexpr uint32_t VT_LAMBDA_FINISH_HOLD = 0xF6E9B8;      // cocos_finish $_10: [this] titleHold = 1
constexpr uint32_t VT_LAMBDA_FINISH_END = 0xF6E98C;       // cocos_finish $_11: nothing
constexpr uint32_t VT_LAMBDA_CLEAR_SE = 0xF6E960;         // cocos_clear_colosseum $_12: sound
constexpr uint32_t VT_LAMBDA_CLEAR_END = 0xF6E934;        // cocos_clear_colosseum $_13: nothing
constexpr uint32_t VT_LAMBDA_LOSE_SE = 0xF6E908;          // cocos_lose_colosseum $_14: sound
constexpr uint32_t VT_LAMBDA_LOSE_END = 0xF6E8DC;         // cocos_lose_colosseum $_15: nothing
constexpr uint32_t VT_LAMBDA_ALL1TURN_SE = 0xF6E8B0;      // all1turn_start $_16: sound

constexpr uint32_t SE_FINISH = 509000280;                 // audio resource ids
constexpr uint32_t SE_COLOSSEUM_CLEAR = 509000560;
constexpr uint32_t SE_ALL1TURN = 509000570;
constexpr uint32_t SE_COLOSSEUM_LOSE = 509000580;

constexpr uint32_t STR_ActMapColosseumAnim_json = 0xE07808;   // "cocostudio/publish/ActMapColosseumAnim.ExportJson"
constexpr uint32_t STR_ActMapTargetTitleAnim_json = 0xE02D90; // "cocostudio/publish/ActMapTargetTitleAnim.ExportJson"
constexpr uint32_t STR_ActMapTargetTitleAnim = 0xE0785E;
constexpr uint32_t STR_ActMapColosseumAnim = 0xE07881;
constexpr uint32_t STR_FieldName_Panel = 0xE0783A;        // "img/start_title/FieldName_Panel.png"
constexpr uint32_t STR_FONT = 0xDF9F4D;                   // "cocostudio/publish/DFPSoGei-W5.TTF"
constexpr uint32_t STR_Colosseum_All1Turn = 0xE078DF;
constexpr uint32_t STR_Colosseum_RoundNum = 0xE078BA;     // "img/colosseum/Colosseum_RoundNum.png"
constexpr uint32_t STR_Colosseum01_plist = 0xE078F2, STR_Colosseum02_plist = 0xE07912;
constexpr uint32_t STR_Colosseum03_plist = 0xE07932;
constexpr uint32_t ROUND_DIGIT_BONES = 0xF6E898;          // const char*[2] "Round_NumL1", "Round_NumL0"
constexpr uint32_t ROUND_DIGIT_EFFECT_BONES = 0xF6E8A0;   // const char*[2] "Round_NumL1_Eff01", "Round_NumL0_Eff01"

constexpr uint32_t Story_vector_copy = 0x4B5BC9;          // vector<hole::network::api::Stage::Story>(const&)
constexpr uint32_t Story_vector_dtor = 0x4B58EF;          // __vector_base<Story>::~__vector_base
constexpr uint32_t IntVector_assign = 0x47D5BD;           // vector<int>::assign(int*, int*)
constexpr uint32_t IntVector_dtor = 0x478961;             // __vector_base<int>::~__vector_base
constexpr uint32_t STORY_SIZE = 24;                       // Story: id, two ints, vector<int>

constexpr uint32_t API_STAGE = 0x13C;                     // APIManager: the stage start data (+0x30 stage id)
constexpr uint32_t API_STORIES = 0x124;                   // APIManager: vector<Story>*
constexpr uint32_t API_COLOSSEUM_STAGE = 0x24C;           // APIManager: int* the colosseum stage id
constexpr uint32_t API_COLOSSEUM_USER = 0x260;            // APIManager: the user's colosseum data (+0x28 class)
constexpr uint32_t TTF_CONFIG_SIZE = 36;

uint32_t api() { return call(APIManager_getInstance, {}); }

// ArmatureDataManager::addArmatureFileInfo(literal)
void addArmatureFile(uint32_t path, uint32_t len) {
    uint32_t mgr = call(ArmatureDataManager_getInstance, {});
    Frame f;
    uint32_t s = literal(f, path, len);
    call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
    strFree(s);
}

// audio::Manager::getInstance()->getLayer(0)->playSE(id, 0, 0)
void playSE(uint32_t id) {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    call(audio_Layer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
}

// CallFunc::create(std::function built on the stack from a lambda), the std::function destroyed after
uint32_t callFunc(uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fn = newFunction(f, vtable, captures);
    uint32_t action = call(CallFunc_create, {fn});
    functionDestroy(fn);
    return action;
}

// visible size (w, h)
Size visible() {
    Frame f;
    uint32_t out = f.alloc(8);
    call(fn::Director_getVisibleSize, {out, director()});
    return Size{ldf(out), ldf(out + 4)};
}

// a cocos2d::TTFConfig on the guest stack: the font file, size, no glyphs, no distance field / outline / styles
uint32_t ttfConfig(Frame &f, int32_t size) {
    uint32_t cfg = f.alloc(TTF_CONFIG_SIZE);
    strInit(cfg, STR_FONT, 0x22);
    st32(cfg + 12, uint32_t(size));
    st32(cfg + 16, 0);
    st32(cfg + 20, 0);
    st8(cfg + 24, 0);
    st32(cfg + 28, 0);
    st8(cfg + 32, 0);
    st8(cfg + 33, 0);
    return cfg;
}

// The colosseum result title: ActMapColosseumAnim (movement `index`) centred at z 9, then delay1, the sound
// lambda, delay2, the closing lambda and the caller's CallFunc.
void colosseumResult(StageDirection *self, uint32_t done, uint32_t index, float delay1, float delay2,
                     uint32_t vtSound, uint32_t vtEnd) {
    const uint32_t me = rw::guest_addr(self);
    Size vs = visible();
    {
        Frame f;
        uint32_t name = literal(f, STR_ActMapColosseumAnim, 0x13);
        self->titleArmature = call(Armature_create, {name});
        strFree(name);
    }
    uint32_t arm = self->titleArmature;
    float x = vs.width * 0.5f, y = vs.height * 0.5f;
    vcall(arm, node::setPositionXY, {arm, fbits(x), fbits(y)});
    uint32_t anim = vcall(self->titleArmature, VT_GET_ANIMATION, {self->titleArmature});
    vcall(anim, VT_ANIM_PLAY_WITH_INDEX, {anim, index, 0xFFFFFFFFu, 0xFFFFFFFFu});
    vcall(me, node::addChildZ, {me, self->titleArmature, 9});
    uint32_t d1 = call(DelayTime_create, {fbits(delay1)});
    uint32_t d2 = call(DelayTime_create, {fbits(delay2)});
    uint32_t sound = callFunc(vtSound, {});
    uint32_t target = self->titleArmature;
    Frame f;
    uint32_t fn = newFunction(f, vtEnd, {});
    uint32_t end = call(CallFunc_create, {fn});
    call(Node_runAction, {target, call(Sequence_create, {d1, sound, d2, end, done, 0})});
    functionDestroy(fn);
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
uint32_t StageDirection::create() {
    uint32_t obj = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<StageDirection>(obj));
    if (vcall(obj, VT_INIT, {obj}) == 1) {
        call(Ref_autorelease, {obj});
        return obj;
    }
    vcall(obj, 4, {obj});    // deleting destructor
    return 0;
}
RW_FUNCTION(0x5FC8CC, "StageDirection::create", StageDirection::create);

StageDirection *StageDirection::construct(StageDirection *self) {
    call(Node_ctor, {rw::guest_addr(self)});
    self->vtable = VTABLE;
    self->kind = 0;
    self->field_278 = 0;
    self->titleArmature = 0;
    self->titleBackArmature = 0;
    self->field_288 = 0;
    self->titleHold = 0;
    self->endTitle = 0;
    self->field_2A8 = 0;
    self->field_2AC = 0;
    self->field_2B4 = -1;
    self->field_2BC = 1;
    return self;
}
RW_FUNCTION(0x767A10, "StageDirection::StageDirection", StageDirection::construct);

StageDirection *StageDirection::destruct(StageDirection *self) {
    call(Node_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x767A78, "StageDirection::~StageDirection", StageDirection::destruct);

void StageDirection::destructDelete(StageDirection *self) {
    call(Node_dtor, {rw::guest_addr(self)});
    call(fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x767A80, "StageDirection::~StageDirection (deleting)", StageDirection::destructDelete);

// state 0; the Node's onEnter / onExit callbacks become lambdas of this direction.
bool StageDirection::init(StageDirection *self) {
    const uint32_t me = rw::guest_addr(self);
    self->state = 0;
    {
        Frame f;
        uint32_t fn = newFunction(f, VT_LAMBDA_INIT_ENTER, {me});
        call(function_vv_assign, {rw::guest_addr(&self->onEnterCallback), fn});
        functionDestroy(fn);
    }
    {
        Frame f;
        uint32_t fn = newFunction(f, VT_LAMBDA_INIT_EXIT, {me});
        call(function_vv_assign, {rw::guest_addr(&self->onExitCallback), fn});
        functionDestroy(fn);
    }
    return true;
}
RW_FUNCTION(0x767A8C, "StageDirection::init", StageDirection::init);

void StageDirection::preloadResources(StageDirection *self) {
    if (self->kind == 1) addArmatureFile(STR_ActMapColosseumAnim_json, 0x31);
    else if (self->kind == 0) addArmatureFile(STR_ActMapTargetTitleAnim_json, 0x33);
}
RW_FUNCTION(0x767B78, "StageDirection::preloadResources", StageDirection::preloadResources);

void StageDirection::exec_stage_title(StageDirection *self, int32_t arg) { stage_start(self, arg); }
RW_FUNCTION(0x767C78, "StageDirection::exec_stage_title", StageDirection::exec_stage_title);

// A screen-sized black LayerColor (z 100) fading out in 0.5 s, then the stage_start lambda.
void StageDirection::stage_start(StageDirection *self, int32_t arg) {
    const uint32_t me = rw::guest_addr(self);
    self->stageArg = arg;
    Frame f;
    uint32_t vs = f.alloc(8);
    call(fn::Director_getVisibleSize, {vs, director()});
    uint32_t layer = call(LayerColor_create, {COLOR4B_BLACK});
    self->fadeLayer = layer;
    uint32_t size = f.alloc(8);
    call(Size_ctor, {size, ld32(vs), ld32(vs + 4)});
    vcall(layer, node::setContentSize, {layer, size});
    vcall(self->fadeLayer, VT_SET_OPACITY, {self->fadeLayer, 255});
    uint32_t fade = call(FadeTo_create, {fbits(0.5f), 0});
    uint32_t target = self->fadeLayer;
    uint32_t fn = newFunction(f, VT_LAMBDA_STAGE_START, {me, uint32_t(arg)});
    uint32_t done = call(CallFunc_create, {fn});
    call(Node_runAction, {target, call(Sequence_create, {fade, done, 0})});
    functionDestroy(fn);
    vcall(me, node::addChildZ, {me, self->fadeLayer, 100});
}
RW_FUNCTION(0x767C80, "StageDirection::stage_start", StageDirection::stage_start);

// The stage name panel (z 11): slides in from below after 0.5 s, stays 4 s and slides out; its label is the
// stage name (master::Stage) of the stage being played. Also copies the matching story of the API's story list
// (the copy is not used).
void StageDirection::stage_name_start(StageDirection *self, int32_t) {
    const uint32_t me = rw::guest_addr(self);
    Frame f;
    self->namePanel = call(Sprite_create, {});
    uint32_t cache = call(Director_getTextureCache, {director()});
    uint32_t path = literal(f, STR_FieldName_Panel, 0x23);
    uint32_t texture = call(TextureCache_addImage, {cache, path});
    strFree(path);
    uint32_t size = f.alloc(8);
    call(Texture2D_getContentSize, {size, texture});
    vcall(self->namePanel, VT_SPRITE_SET_TEXTURE, {self->namePanel, texture});
    uint32_t panel = self->namePanel;
    uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, 0, 0, ld32(size), ld32(size + 4)});
    vcall(panel, VT_SPRITE_SET_TEXTURE_RECT, {panel, rect});
    call(Vec2_dtor, {rect});
    vcall(self->namePanel, node::setVisible, {self->namePanel, 1});
    float x = ldf(size) * 0.5f;
    float below = ldf(size + 4) * -0.5f;
    vcall(self->namePanel, node::setPositionXY, {self->namePanel, fbits(x), fbits(below)});
    uint32_t wait = call(DelayTime_create, {fbits(0.5f)});
    uint32_t p = f.alloc(8);
    call(Vec2_ctor_xy, {p, fbits(ldf(size) * 0.5f), fbits(ldf(size + 4) * 0.5f)});
    uint32_t moveIn = call(MoveTo_create, {fbits(0.2f), p});
    call(Vec2_dtor, {p});
    uint32_t easeIn = call(EaseSineOut_create, {moveIn});
    uint32_t hold = call(DelayTime_create, {fbits(4.0f)});
    uint32_t q = f.alloc(8);
    call(Vec2_ctor_xy, {q, fbits(ldf(size) * 0.5f), fbits(ldf(size + 4) * -0.5f)});
    uint32_t moveOut = call(MoveTo_create, {fbits(0.2f), q});
    call(Vec2_dtor, {q});
    uint32_t easeOut = call(EaseSineIn_create, {moveOut});
    call(Node_runAction, {self->namePanel, call(Sequence_create, {wait, easeIn, hold, easeOut, 0})});
    vcall(me, node::addChildZ, {me, self->namePanel, 11});

    // the story of the stage being played (copied, unused)
    uint32_t stageId = ld32(ld32(api() + API_STAGE) + 0x30);
    uint32_t stories = f.alloc(12);
    call(Story_vector_copy, {stories, ld32(api() + API_STORIES)});
    uint32_t story = f.alloc(STORY_SIZE);
    st32(story + 12, 0);
    st32(story + 16, 0);
    st32(story + 20, 0);
    uint32_t begin = ld32(stories), end = ld32(stories + 4);
    if (end != begin) {
        uint32_t count = uint32_t(int32_t(end - begin) >> 3) * 0xAAAAAAABu;
        uint32_t s = begin;
        for (uint32_t k = 0; k < count; k++, s += STORY_SIZE) {
            if (ld32(s) != stageId) continue;
            st32(story, ld32(s));
            st32(story + 4, ld32(s + 4));
            st32(story + 8, ld32(s + 8));
            if (story != s) call(IntVector_assign, {story + 12, ld32(s + 12), ld32(s + 16)});
            break;
        }
    }

    uint32_t row = masterRow(f, master_Stage_create, stageId);
    uint32_t cfg = ttfConfig(f, 22);
    uint32_t name = safeBytes(f, ld32(row), 8, 0x81);
    uint32_t text = newStr(f, name);
    uint32_t label = call(Label_createWithTTF, {cfg, text, 0, 0});
    strFree(text);
    uint32_t pos = f.alloc(8);
    call(Vec2_ctor_xy, {pos, fbits(ldf(size) * 0.5f), fbits(ldf(size + 4) * 0.5f)});
    vcall(label, node::setPosition, {label, pos});
    call(Vec2_dtor, {pos});
    vcall(label, node::setColor, {label, COLOR3B_WHITE});
    vcall(self->namePanel, node::addChildZ, {self->namePanel, label, 10});
    strFree(cfg);
    sharedRelease(row);
    call(IntVector_dtor, {story + 12});
    call(Story_vector_dtor, {stories});
}
RW_FUNCTION(0x767DD0, "StageDirection::stage_name_start", StageDirection::stage_name_start);

// A target title armature (movement index `movement`) at (x, y), z `z`, stored in *armatureOut.
void StageDirection::cocos_sub_start(StageDirection *self, uint32_t armatureOut, int32_t movement, int32_t z,
                                     float x, float y) {
    const uint32_t me = rw::guest_addr(self);
    {
        Frame f;
        uint32_t name = literal(f, STR_ActMapTargetTitleAnim, 0x15);
        st32(armatureOut, call(Armature_create, {name}));
        strFree(name);
    }
    uint32_t arm = ld32(armatureOut);
    vcall(arm, node::setPositionXY, {arm, fbits(x), fbits(y)});
    uint32_t anim = vcall(ld32(armatureOut), VT_GET_ANIMATION, {ld32(armatureOut)});
    vcall(anim, VT_ANIM_PLAY_WITH_INDEX, {anim, uint32_t(movement), 0xFFFFFFFFu, 0xFFFFFFFFu});
    vcall(me, node::addChildZ, {me, ld32(armatureOut), uint32_t(z)});
}
RW_FUNCTION(0x768200, "StageDirection::cocos_sub_start", StageDirection::cocos_sub_start);

// The quest finish title (quests only): target title movement 2 (z 12), then after 0.5 s titleHold = 1, the
// sound, 3 s, and the caller's CallFunc.
void StageDirection::cocos_finish(StageDirection *self, uint32_t done) {
    const uint32_t me = rw::guest_addr(self);
    Size vs = visible();
    if (self->kind != 0) return;
    float x = vs.width * 0.5f;
    float y = vs.height * 0.5f + -32.0f;
    cocos_sub_start(self, rw::guest_addr(&self->titleArmature), 2, 12, x, y);
    uint32_t sound = callFunc(VT_LAMBDA_FINISH_SE, {});
    uint32_t target = self->titleArmature;
    uint32_t wait = call(DelayTime_create, {fbits(0.5f)});
    Frame f;
    uint32_t holdFn = newFunction(f, VT_LAMBDA_FINISH_HOLD, {me});
    uint32_t hold = call(CallFunc_create, {holdFn});
    uint32_t wait2 = call(DelayTime_create, {fbits(3.0f)});
    uint32_t endFn = newFunction(f, VT_LAMBDA_FINISH_END, {});
    uint32_t end = call(CallFunc_create, {endFn});
    call(Node_runAction, {target, call(Sequence_create, {wait, hold, sound, wait2, end, done, 0})});
    functionDestroy(endFn);
    functionDestroy(holdFn);
}
RW_FUNCTION(0x7691BC, "StageDirection::cocos_finish", StageDirection::cocos_finish);

void StageDirection::cocos_clear_colosseum(StageDirection *self, uint32_t done) {
    colosseumResult(self, done, 3, 1.25f, 1.75f, VT_LAMBDA_CLEAR_SE, VT_LAMBDA_CLEAR_END);
}
RW_FUNCTION(0x7693A4, "StageDirection::cocos_clear_colosseum", StageDirection::cocos_clear_colosseum);

void StageDirection::cocos_lose_colosseum(StageDirection *self, uint32_t done) {
    colosseumResult(self, done, 2, 0.5f, 2.5f, VT_LAMBDA_LOSE_SE, VT_LAMBDA_LOSE_END);
}
RW_FUNCTION(0x769598, "StageDirection::cocos_lose_colosseum", StageDirection::cocos_lose_colosseum);

// The colosseum "all in 1 turn" title: ActMapColosseumAnim movement 1 with the round number of the user's class
// (two digits from Colosseum_RoundNum.png, digit + glow effect bones), three particle systems, then after 0.5 s
// the sound and, (movement length - 31) / 60 s later, the caller's CallFunc.
void StageDirection::all1turn_start(StageDirection *self, uint32_t done) {
    const uint32_t me = rw::guest_addr(self);
    Frame f;
    Size vs = visible();
    {
        Frame g;
        uint32_t name = literal(g, STR_ActMapColosseumAnim, 0x13);
        self->titleArmature = call(Armature_create, {name});
        strFree(name);
    }
    uint32_t arm = self->titleArmature;
    vcall(arm, node::setPositionXY, {arm, fbits(vs.width * 0.5f), fbits(vs.height * 0.5f)});
    uint32_t anim = vcall(self->titleArmature, VT_GET_ANIMATION, {self->titleArmature});
    vcall(anim, VT_ANIM_PLAY_WITH_INDEX, {anim, 1, 0xFFFFFFFFu, 0xFFFFFFFFu});
    uint32_t data = vcall(anim, VT_ANIM_GET_ANIMATION_DATA, {anim});
    uint32_t movement;
    {
        Frame g;
        uint32_t name = literal(g, STR_Colosseum_All1Turn, 0x12);
        movement = call(AnimationData_getMovement, {data, name});
        strFree(name);
    }
    int32_t duration = int32_t(ld32(movement + 0x20));
    uint32_t cache = call(Director_getTextureCache, {director()});
    uint32_t texture;
    {
        Frame g;
        uint32_t path = literal(g, STR_Colosseum_RoundNum, 0x24);
        texture = call(TextureCache_addImage, {cache, path});
        strFree(path);
    }
    uint32_t texSize = f.alloc(8);
    call(Texture2D_getContentSize, {texSize, texture});

    // the round of the user's colosseum class
    uint32_t stage = masterRow(f, master_ColosseumStage_create, ld32(ld32(api() + API_COLOSSEUM_STAGE)));
    uint32_t user = ld32(api() + API_COLOSSEUM_USER);
    int32_t id = safe32(ld32(stage), 0x1C);
    int32_t round = 99;
    for (;;) {
        if (id > safe32(ld32(stage), 0x20)) break;
        Frame g;
        uint32_t row = masterRow(g, master_Colosseum_create, uint32_t(id));
        bool found = false;
        if (int32_t(ld32(user + 0x28)) == safe32(ld32(row), 8)) {
            round = safe32(ld32(row), 0x58);
            found = true;
        }
        sharedRelease(row);
        id++;
        if (found) break;
    }

    // two digits (tens, ones): a sprite of the digit on the bone and an additive copy on its effect bone; a
    // leading digit of a round below its place is hidden
    int32_t rest = round, place = 10;
    for (uint32_t k = 0; k < 2; k++) {
        uint32_t bone, effect;
        {
            Frame g;
            uint32_t a = self->titleArmature;
            uint32_t n = newStr(g, ld32(ROUND_DIGIT_BONES + 4 * k));
            bone = vcall(a, VT_ARMATURE_GET_BONE, {a, n});
            strFree(n);
        }
        {
            Frame g;
            uint32_t a = self->titleArmature;
            uint32_t n = newStr(g, ld32(ROUND_DIGIT_EFFECT_BONES + 4 * k));
            effect = vcall(a, VT_ARMATURE_GET_BONE, {a, n});
            strFree(n);
        }
        if (!bone || !effect) continue;
        int32_t digit = rest / place;
        if (round < place) {
            uint32_t dm = vcall(bone, VT_BONE_GET_DISPLAY_MANAGER, {bone});
            vcall(dm, VT_DISPLAY_SET_VISIBLE, {dm, 0});
            dm = vcall(effect, VT_BONE_GET_DISPLAY_MANAGER, {effect});
            vcall(dm, VT_DISPLAY_SET_VISIBLE, {dm, 0});
        } else {
            Frame g;
            uint32_t sprite = call(Sprite_create, {});
            vcall(sprite, VT_SPRITE_SET_TEXTURE, {sprite, texture});
            float w = ldf(texSize) / 10.0f;
            float fd = float(digit);
            uint32_t rect = g.alloc(16);
            call(Rect_ctor, {rect, fbits(fd * w), 0, fbits(w), ld32(texSize + 4)});
            vcall(sprite, VT_SPRITE_SET_TEXTURE_RECT, {sprite, rect});
            call(Vec2_dtor, {rect});
            call(Bone_addDisplay, {bone, sprite, 0});
            uint32_t glow = call(Sprite_create, {});
            vcall(glow, VT_SPRITE_SET_TEXTURE, {glow, texture});
            float w2 = ldf(texSize) / 10.0f;
            uint32_t rect2 = g.alloc(16);
            call(Rect_ctor, {rect2, fbits(fd * w2), 0, fbits(w2), ld32(texSize + 4)});
            vcall(glow, VT_SPRITE_SET_TEXTURE_RECT, {glow, rect2});
            call(Vec2_dtor, {rect2});
            vcall(glow, VT_SPRITE_SET_BLEND_FUNC, {glow, BLENDFUNC_ADDITIVE});
            call(Bone_addDisplay, {effect, glow, 0});
        }
        rest = rest % place;
        place = place / 10;
    }

    uint32_t particles[3];
    const uint32_t plists[3] = {STR_Colosseum01_plist, STR_Colosseum02_plist, STR_Colosseum03_plist};
    for (int k = 0; k < 3; k++) {
        Frame g;
        uint32_t s = literal(g, plists[k], 0x1F);
        particles[k] = call(ParticleSystemQuad_create, {s});
        strFree(s);
    }
    for (uint32_t p : particles) vcall(p, VT_PARTICLE_SET_AUTO_REMOVE, {p, 0});
    for (uint32_t p : particles) vcall(me, node::addChildZ, {me, p, 9});
    vcall(me, node::addChildZ, {me, self->titleArmature, 9});
    uint32_t wait = call(DelayTime_create, {fbits(0.5f)});
    uint32_t wait2 = call(DelayTime_create, {fbits(float(duration - 31) / 60.0f)});
    uint32_t sound = callFunc(VT_LAMBDA_ALL1TURN_SE, {});
    call(Node_runAction, {self->titleArmature, call(Sequence_create, {wait, sound, wait2, done, 0})});
    sharedRelease(stage);
}
RW_FUNCTION(0x769788, "StageDirection::all1turn_start", StageDirection::all1turn_start);

// ------------------------------------------------------------------------------------------- small state
StageDirection *StageDirection::to_start(StageDirection *self) {
    self->endTitle = 1;
    self->state = 4;
    return self;
}
RW_FUNCTION(0x769EE0, "StageDirection::to_start", StageDirection::to_start);

// The fade layer turns white, transparent, visible, and fades in after 1.5 s (1.5 s).
void StageDirection::to_end(StageDirection *self) {
    uint32_t wait = call(DelayTime_create, {fbits(1.5f)});
    uint32_t fade = call(FadeTo_create, {fbits(1.5f), 255});
    vcall(self->fadeLayer, node::setColor, {self->fadeLayer, COLOR3B_WHITE});
    vcall(self->fadeLayer, VT_SET_OPACITY, {self->fadeLayer, 0});
    vcall(self->fadeLayer, node::setVisible, {self->fadeLayer, 1});
    call(Node_runAction, {self->fadeLayer, call(Sequence_create, {wait, fade, 0})});
}
RW_FUNCTION(0x769EF4, "StageDirection::to_end", StageDirection::to_end);

void StageDirection::update(StageDirection *, float) {}
RW_FUNCTION(0x769F7C, "StageDirection::update", StageDirection::update);

uint8_t StageDirection::isEndTitle(StageDirection *self) { return self->endTitle; }
RW_FUNCTION(0x769F7E, "StageDirection::isEndTitle", StageDirection::isEndTitle);

bool StageDirection::isTitleHold(StageDirection *self) { return self->titleHold == 1; }
RW_FUNCTION(0x769F86, "StageDirection::isTitleHold", StageDirection::isTitleHold);

StageDirection *StageDirection::TitleHoldReset(StageDirection *self) {
    self->titleHold = 2;
    return self;
}
RW_FUNCTION(0x769F98, "StageDirection::TitleHoldReset", StageDirection::TitleHoldReset);

bool StageDirection::turn_and_face_boss(StageDirection *, uint32_t) { return true; }
RW_FUNCTION(0x769FA2, "StageDirection::turn_and_face_boss", StageDirection::turn_and_face_boss);

// ------------------------------------------------------------------------------------------- lambda bodies
namespace {
// cocos_finish $_9 / cocos_clear_colosseum $_12 / cocos_lose_colosseum $_14 / all1turn_start $_16: a sound
void finishSoundLambda() { playSE(SE_FINISH); }
RW_FUNCTION(0x76A558, "StageDirection::cocos_finish $_9 __func::operator()", finishSoundLambda);
void clearSoundLambda() { playSE(SE_COLOSSEUM_CLEAR); }
RW_FUNCTION(0x76A324, "StageDirection::cocos_clear_colosseum $_12 __func::operator()", clearSoundLambda);
void loseSoundLambda() { playSE(SE_COLOSSEUM_LOSE); }
RW_FUNCTION(0x76A190, "StageDirection::cocos_lose_colosseum $_14 __func::operator()", loseSoundLambda);
void all1turnSoundLambda() { playSE(SE_ALL1TURN); }
RW_FUNCTION(0x769FFC, "StageDirection::all1turn_start $_16 __func::operator()", all1turnSoundLambda);

// cocos_finish $_10: the title holds
void finishHoldLambda(uint32_t func) { rw::host_ptr<StageDirection>(ld32(func + 4))->titleHold = 1; }
RW_FUNCTION(0x76A4C2, "StageDirection::cocos_finish $_10 __func::operator()", finishHoldLambda);

// cocos_finish $_11, cocos_clear_colosseum $_13, cocos_lose_colosseum $_15: nothing
void finishEndLambda() {}
RW_FUNCTION(0x76A42E, "StageDirection::cocos_finish $_11 __func::operator()", finishEndLambda);
void clearEndLambda() {}
RW_FUNCTION(0x76A29A, "StageDirection::cocos_clear_colosseum $_13 __func::operator()", clearEndLambda);
void loseEndLambda() {}
RW_FUNCTION(0x76A106, "StageDirection::cocos_lose_colosseum $_15 __func::operator()", loseEndLambda);

// cocos_start $_8: the title armature is removed (cleanup) and the opening ends
void startEndLambda(uint32_t func) {
    StageDirection *self = rw::host_ptr<StageDirection>(ld32(func + 4));
    const uint32_t me = rw::guest_addr(self);
    vcall(me, node::removeChild, {me, self->titleArmature, 1});
    self->endTitle = 1;
    self->state = 4;
}
RW_FUNCTION(0x76A66E, "StageDirection::cocos_start $_8 __func::operator()", startEndLambda);
}  // namespace
