// StageDirection::cocos_start: the opening title of a quest (see StageDirection.hpp).
//  - quest (kind 0): two target title armatures (movements 0 and 1), the "target" label (TextData 103100020 with
//    the target enemy's name) on the Target_Label bone, and the target enemy's model zooming in (move / scale /
//    fade), then the lambdas $_3, $_4, $_5 run along its sequence;
//  - colosseum (kind 1): ActMapColosseumAnim with the user's class number (Class_Num1/0 bones, two digits of
//    Colosseum_ClassNum.png), the round number (Round_NumL2/L1/L0, three digits of Colosseum_RoundNum.png) and the
//    colosseum stage's value (Round_NumS0/1/2, small digits); after the sequence the lambda $_8 ends the opening.
// Any other kind does nothing. sub_768F60 is this function's epilogue (stack guard check, frame 0x66C), not a
// function of its own.
#include "StageDirection.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using namespace sdir;

namespace {
constexpr uint32_t VT_LAMBDA_START_3 = 0xF6EAEC;          // cocos_start $_3 (no capture)
constexpr uint32_t VT_LAMBDA_START_4 = 0xF6EAC0;          // cocos_start $_4 [this]
constexpr uint32_t VT_LAMBDA_START_5 = 0xF6EA94;          // cocos_start $_5 [this]
constexpr uint32_t VT_LAMBDA_START_6 = 0xF6EA68;          // cocos_start $_6 (no capture; created, unused)
constexpr uint32_t VT_LAMBDA_START_7 = 0xF6EA3C;          // cocos_start $_7 (no capture)
constexpr uint32_t VT_LAMBDA_START_8 = 0xF6EA10;          // cocos_start $_8 [this]: the opening ends

constexpr uint32_t STR_ActMapColosseumAnim = 0xE07881;
constexpr uint32_t STR_FONT = 0xDF9F4D;                   // "cocostudio/publish/DFPSoGei-W5.TTF"
constexpr uint32_t STR_Target_Label = 0xE07874;
constexpr uint32_t STR_Colosseum_ClassNum = 0xE07895;     // "img/colosseum/Colosseum_ClassNum.png"
constexpr uint32_t STR_Colosseum_RoundNum = 0xE078BA;     // "img/colosseum/Colosseum_RoundNum.png"
constexpr uint32_t CLASS_DIGIT_BONES = 0xF6E878;          // const char*[2] "Class_Num1", "Class_Num0"
constexpr uint32_t ROUND_DIGIT_BONES = 0xF6E880;          // const char*[3] "Round_NumL2", "Round_NumL1", "Round_NumL0"
constexpr uint32_t SMALL_DIGIT_BONES = 0xF6E88C;          // const char*[3] "Round_NumS0", "Round_NumS1", "Round_NumS2"
constexpr int32_t TEXT_TARGET = 103100020;                // TextData: the target label format ("%s")
constexpr uint32_t API_COLOSSEUM_STAGE = 0x24C;           // APIManager: int* the colosseum stage id
constexpr uint32_t API_COLOSSEUM_USER = 0x260;            // APIManager: the user's colosseum data
constexpr uint32_t USER_CLASS = 0x28, USER_ROUND = 0x2C;
constexpr uint32_t ENEMY_NAME = 4, ENEMY_NAME_SIZE = 0x41;    // master::Enemy: name
constexpr uint32_t ENEMY_MODEL = 0x248;                   // master::Enemy: model id (Enemy::create)
constexpr uint32_t ENEMY_OFFSET = 0x530;                  // Enemy: Vec2 offset of the model (y used)
constexpr uint32_t ENEMY_NODE = 0x29C;                    // Enemy: its node (update unscheduled)
constexpr uint32_t TTF_CONFIG_SIZE = 36;

uint32_t api() { return call(APIManager_getInstance, {}); }

Size visible() {
    Frame f;
    uint32_t out = f.alloc(8);
    call(fn::Director_getVisibleSize, {out, director()});
    return Size{ldf(out), ldf(out + 4)};
}

uint32_t callFunc(uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fn = newFunction(f, vtable, captures);
    uint32_t action = call(CallFunc_create, {fn});
    functionDestroy(fn);
    return action;
}

// armature->getBone(name) for a C string (table entry)
uint32_t bone(uint32_t armature, uint32_t name) {
    Frame f;
    uint32_t s = newStr(f, name);
    uint32_t b = vcall(armature, VT_ARMATURE_GET_BONE, {armature, s});
    strFree(s);
    return b;
}

// hide a bone's displays
void hideBone(uint32_t b) {
    uint32_t dm = vcall(b, VT_BONE_GET_DISPLAY_MANAGER, {b});
    vcall(dm, VT_DISPLAY_SET_VISIBLE, {dm, 0});
}

// a Sprite showing digit `digit` of a 10-digit strip texture (width texW, height at texHeightAddr)
uint32_t digitSprite(uint32_t texture, uint32_t texSize, int32_t digit) {
    Frame f;
    uint32_t sprite = call(Sprite_create, {});
    vcall(sprite, VT_SPRITE_SET_TEXTURE, {sprite, texture});
    uint32_t setRect = ld32(ld32(sprite) + VT_SPRITE_SET_TEXTURE_RECT);
    float w = ldf(texSize) / 10.0f;
    float x = float(digit) * w;
    uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, fbits(x), 0, fbits(w), ld32(texSize + 4)});
    call(setRect, {sprite, rect});
    call(Vec2_dtor, {rect});
    return sprite;
}

uint32_t loadTexture(uint32_t path) {
    uint32_t cache = call(Director_getTextureCache, {director()});
    Frame f;
    uint32_t s = literal(f, path, 0x24);
    uint32_t texture = call(TextureCache_addImage, {cache, s});
    strFree(s);
    return texture;
}

// ------------------------------------------------------------------------------------------- colosseum
void colosseumStart(StageDirection *self, Size vs) {
    const uint32_t me = rw::guest_addr(self);
    Frame f;
    {
        Frame g;
        uint32_t name = literal(g, STR_ActMapColosseumAnim, 0x13);
        self->titleArmature = call(Armature_create, {name});
        strFree(name);
    }
    uint32_t arm = self->titleArmature;
    vcall(arm, node::setPositionXY, {arm, fbits(vs.width * 0.5f), fbits(vs.height * 0.5f)});
    uint32_t anim = vcall(self->titleArmature, VT_GET_ANIMATION, {self->titleArmature});
    vcall(anim, VT_ANIM_PLAY_WITH_INDEX, {anim, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
    int32_t value = 0;     // the colosseum entry's value for the user's class
    uint32_t user = ld32(api() + API_COLOSSEUM_USER);
    int32_t userClass = int32_t(ld32(user + USER_CLASS));
    int32_t userRound = int32_t(ld32(user + USER_ROUND));
    uint32_t stage = masterRow(f, master_ColosseumStage_create, ld32(ld32(api() + API_COLOSSEUM_STAGE)));
    int32_t id = safe32(ld32(stage), 0x1C);
    for (;;) {
        if (id > safe32(ld32(stage), 0x20)) break;
        Frame g;
        uint32_t row = masterRow(g, master_Colosseum_create, uint32_t(id));
        bool found = false;
        if (userClass == safe32(ld32(row), 8)) {
            value = safe32(ld32(row), 0x10);
            found = true;
        }
        sharedRelease(row);
        id++;
        if (found) break;
    }

    // the class number: two digits; a leading digit above the number is hidden
    uint32_t texture = loadTexture(STR_Colosseum_ClassNum);
    uint32_t texSize = f.alloc(8);
    call(Texture2D_getContentSize, {texSize, texture});
    int32_t place = 10, rest = userClass;
    for (uint32_t k = 0; k <= 1;) {
        uint32_t b = bone(self->titleArmature, ld32(CLASS_DIGIT_BONES + 4 * k));
        if (b) {
            if (userClass < place) {
                uint32_t dm = vcall(b, VT_BONE_GET_DISPLAY_MANAGER, {b});
                place = place / 10;
                k++;
                vcall(dm, VT_DISPLAY_SET_VISIBLE, {dm, 0});
                continue;
            }
            int32_t digit = rest / place;
            rest = rest % place;
            int32_t next = place / 10;
            call(Bone_addDisplay, {b, digitSprite(texture, texSize, digit), 0});
            place = next;
        }
        k++;
        if (k > 1 || place <= 0) break;
    }

    // the round number: three digits
    texture = loadTexture(STR_Colosseum_RoundNum);
    uint32_t roundSize = f.alloc(8);
    call(Texture2D_getContentSize, {roundSize, texture});
    call(Size_assign, {texSize, roundSize});
    place = 100;
    rest = userRound;
    for (uint32_t k = 0; k < 3; k++) {
        uint32_t b = bone(self->titleArmature, ld32(ROUND_DIGIT_BONES + 4 * k));
        if (!b) continue;
        int32_t digit = rest / place;
        if (userRound < place) {
            hideBone(b);
        } else {
            call(Bone_addDisplay, {b, digitSprite(texture, texSize, digit), 0});
        }
        rest = rest % place;
        place = place / 10;
    }

    // the stage value: up to three small digits, the unused bones hidden
    place = 100;
    rest = value;
    uint32_t k = 0;
    bool cleanup = true;
    for (;;) {
        uint32_t b = bone(self->titleArmature, ld32(SMALL_DIGIT_BONES + 4 * k));
        if (b) {
            if (value < place) {
                place = place / 10;
                if (k < 3) continue;      // the same bone with the next place
                cleanup = false;
                break;
            }
            if (place < 1) break;
            int32_t digit = rest / place;
            rest = rest % place;
            place = place / 10;
            uint32_t sprite = digitSprite(texture, texSize, digit);
            vcall(sprite, node::setPositionXY, {sprite, fbits(6.0f), fbits(6.0f)});
            call(Bone_addDisplay, {b, sprite, 0});
        }
        k++;
        if (k > 2 || place < 1) break;
    }
    if (cleanup) {
        for (; k <= 2; k++) {
            uint32_t b = bone(self->titleArmature, ld32(SMALL_DIGIT_BONES + 4 * k));
            if (b) hideBone(b);
        }
    }

    vcall(me, node::addChildZ, {me, self->titleArmature, 12});
    uint32_t wait = call(DelayTime_create, {fbits(0.5f)});
    uint32_t wait2 = call(DelayTime_create, {fbits(1.5f)});
    uint32_t wait3 = call(DelayTime_create, {0x3F955555u});   // 1.1666666
    callFunc(VT_LAMBDA_START_6, {});
    uint32_t step = callFunc(VT_LAMBDA_START_7, {});
    uint32_t target = self->titleArmature;
    uint32_t endFn = newFunction(f, VT_LAMBDA_START_8, {me});
    uint32_t end = call(CallFunc_create, {endFn});
    call(Node_runAction, {target, call(Sequence_create, {wait, step, wait2, wait3, end, 0})});
    functionDestroy(endFn);
    sharedRelease(stage);
}

// ------------------------------------------------------------------------------------------- quest
void questStart(StageDirection *self, int32_t enemyId, Size vs) {
    const uint32_t me = rw::guest_addr(self);
    Frame f;
    StageDirection::cocos_sub_start(self, rw::guest_addr(&self->titleArmature), 0, 12, vs.width * 0.5f,
                                    vs.height * 0.5f + -32.0f);
    StageDirection::cocos_sub_start(self, rw::guest_addr(&self->titleBackArmature), 1, 10, vs.width * 0.5f,
                                    vs.height * 0.5f + -32.0f);
    uint32_t enemy = masterRow(f, master_Enemy_create, uint32_t(enemyId));

    // the target label: TextData 103100020 formatted with the enemy's name
    uint32_t cfg = f.alloc(TTF_CONFIG_SIZE);
    strInit(cfg, STR_FONT, 0x22);
    st32(cfg + 12, 40);
    st32(cfg + 16, 0);
    st32(cfg + 20, 0);
    st8(cfg + 24, 0);
    st32(cfg + 28, 0);
    st8(cfg + 32, 0);
    st8(cfg + 33, 0);
    uint32_t label;
    {
        Frame g;
        uint32_t text = g.alloc(8);
        call(TextData_get, {text, 0, uint32_t(TEXT_TARGET)});
        uint32_t format = ld32(ld32(text) + 4);
        uint32_t name = safeBytes(g, ld32(enemy), ENEMY_NAME, ENEMY_NAME_SIZE);
        uint32_t buf = stringBuffer(g, {format, name});
        uint32_t s = newStr(g, buf);
        label = call(Label_createWithTTF, {cfg, s, 0, 0});
        strFree(s);
        sharedRelease(text);
    }
    vcall(label, node::setColor, {label, COLOR3B_WHITE});
    {
        Frame g;
        uint32_t outline = g.alloc(4);
        call(Color4B_ctor, {outline, 0x13, 0x13, 0x13, 0xFF});
        vcall(label, VT_LABEL_ENABLE_OUTLINE, {label, outline, 6});
    }
    call(Label_setAdditionalKerning, {label, fbits(-8.0f)});
    uint32_t b;
    {
        Frame g;
        uint32_t arm = self->titleArmature;
        uint32_t s = literal(g, STR_Target_Label, 0xC);
        b = vcall(arm, VT_ARMATURE_GET_BONE, {arm, s});
        strFree(s);
    }
    if (b) call(Bone_addDisplay, {b, label, 0});

    // the target enemy's model
    uint32_t model;
    {
        Frame g;
        uint32_t row = masterRow(g, master_Enemy_create, uint32_t(enemyId));
        model = call(Enemy_create, {uint32_t(safe32(ld32(row), ENEMY_MODEL))});
        sharedRelease(row);
    }
    self->targetEnemy = model;
    uint32_t offset = f.alloc(8);
    call(Vec2_ctor_copy, {offset, model + ENEMY_OFFSET});
    float x = vs.width * 0.5f;
    float y = ((vs.height * 0.5f + -32.0f) + 80.0f) + ldf(offset + 4) * -3.0f;
    vcall(self->targetEnemy, node::setPositionXY, {self->targetEnemy, fbits(x), fbits(y)});
    vcall(self->targetEnemy, node::setScale, {self->targetEnemy, fbits(3.0f)});
    vcall(self->targetEnemy, VT_SET_OPACITY, {self->targetEnemy, 255});
    float tx = vs.width * 0.5f;
    float ty = ((vs.height * 0.5f + -32.0f) + 80.0f) - ldf(offset + 4);
    const uint32_t third = 0x3EAAAAABu;     // 1/3 s
    uint32_t move;
    {
        Frame g;
        uint32_t p = g.alloc(8);
        call(Vec2_ctor_xy, {p, fbits(tx), fbits(ty)});
        move = call(MoveTo_create, {third, p});
        call(Vec2_dtor, {p});
    }
    uint32_t scale = call(ScaleTo_create, {third, fbits(1.0f)});
    uint32_t fade = call(FadeTo_create, {third, 255});
    uint32_t easeMove = call(EaseSineOut_create, {move});
    uint32_t easeScale = call(EaseSineOut_create, {scale});
    uint32_t easeFade = call(EaseSineOut_create, {fade});
    uint32_t zoom = call(Spawn_create, {easeMove, easeFade, easeScale, 0});
    uint32_t waitA = call(DelayTime_create, {fbits(0.5f)});
    uint32_t waitB = call(DelayTime_create, {0x3F7BBBBCu});     // 0.98333335
    uint32_t waitC = call(DelayTime_create, {0x3F044444u});     // 0.51666665
    uint32_t step3 = callFunc(VT_LAMBDA_START_3, {});
    uint32_t target = self->targetEnemy;
    uint32_t fn4 = newFunction(f, VT_LAMBDA_START_4, {me});
    uint32_t step4 = call(CallFunc_create, {fn4});
    uint32_t fadeOut = call(EaseCubicActionIn_create, {call(FadeTo_create, {fbits(0.5f), 0})});
    uint32_t fn5 = newFunction(f, VT_LAMBDA_START_5, {me});
    uint32_t step5 = call(CallFunc_create, {fn5});
    call(Node_runAction,
         {target, call(Sequence_create, {step3, zoom, step4, waitA, waitB, fadeOut, waitC, step5, 0})});
    functionDestroy(fn5);
    functionDestroy(fn4);
    vcall(me, node::addChildZ, {me, self->targetEnemy, 11});
    call(Node_unscheduleUpdate, {ld32(self->targetEnemy + ENEMY_NODE)});
    call(Vec2_dtor, {offset});
    strFree(cfg);
    sharedRelease(enemy);
}
}  // namespace

void StageDirection::cocos_start(StageDirection *self, int32_t enemyId) {
    Size vs = visible();
    if (self->kind == 1) colosseumStart(self, vs);
    else if (self->kind == 0) questStart(self, enemyId, vs);
}
RW_FUNCTION(0x7682D0, "StageDirection::cocos_start", StageDirection::cocos_start);
