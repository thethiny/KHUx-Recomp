// Avatar Board pieces: SphereUI's texture loaders and master-data getters, free_board / select_incentive,
// SceneSphereBoard's createScene / changeSceneNext / back lambda, SceneSphereBoardSelect's medal lambdas,
// ConnectingSphere::getSphereOpenlist and LuxBoard's sprite lambda.
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t Director_getInstance = 0x8EE519, Director_getTextureCache = 0x8EFE69;
constexpr uint32_t TextureCache_addImage = 0x917DA9, StringBuffer_ctor = 0x49140D, SafeBuffer_read = 0x45A269;
constexpr uint32_t Skill_create = 0x78BDE9, Medal_create = 0x785871, Keyblade_create = 0x783F9D;
constexpr uint32_t Material_create = 0x7852E5;
constexpr uint32_t SphereUI_selectIncentiveSub = 0x795751;
constexpr uint32_t Scene_create = 0x8A5F37, SceneSphereBoard_create = 0x6BCE79;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5, SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, SceneUtil_openMedalInfoPopup = 0x466EF5;
constexpr uint32_t map_sphere_find = 0x503479, vector_int_copy = 0x4AF707;
constexpr uint32_t S_FmtMedal = 0xDF90C5, S_FmtMaterial = 0xDF9101, S_FmtKeyblade = 0xDF9213;
constexpr uint32_t S_FmtSkill = 0xE079ED, S_FmtAvatarSet = 0xDF92BE, S_FmtAvatar = 0xDF92D9;
constexpr uint32_t S_FmtComposeLwf = 0xE052F7, S_Empty = 0xDF8A1A;
constexpr uint32_t VT_BoardOut = 0xF66AF8, VT_BoardBack = 0xF66B24;
constexpr uint32_t V_addChild = 0xF0, V_setTag304 = 0x130;

uint32_t loadTexture(uint32_t fmt, uint32_t id) {
    const uint32_t cache = call(Director_getTextureCache, {call(Director_getInstance, {})});
    Frame f;
    const uint32_t buf = f.alloc(0x404);
    call(StringBuffer_ctor, {buf, fmt, id});
    const uint32_t s = newStr(f, buf);
    const uint32_t tex = call(TextureCache_addImage, {cache, s});
    strFree(s);
    return tex;
}
// a master row's word at `off` (read flag off)
uint32_t masterWord(uint32_t create, uint32_t id, uint32_t off) {
    Frame f;
    const uint32_t sp = f.alloc(8), v = f.alloc(4);
    call(create, {sp, id});
    call(SafeBuffer_read, {ld32(sp), v, ld32(ld32(sp) + 0xC) + off, 4, off});
    const uint32_t r = ld32(v);
    if (ld32(sp + 4)) call(fn::shared_release, {ld32(sp + 4)});
    return r;
}
// a master row's name (0x41 bytes at `off`) into out
uint32_t masterName(uint32_t out, uint32_t create, uint32_t id, uint32_t off) {
    Frame f;
    const uint32_t sp = f.alloc(8);
    call(create, {sp, id});
    call(SafeBuffer_read, {ld32(sp), out, ld32(ld32(sp) + 0xC) + off, 0x41, off});
    if (ld32(sp + 4)) call(fn::shared_release, {ld32(sp + 4)});
    return 0;
}
}  // namespace

namespace sphere_small {

// the 5 x 9 board sprites (from +0x370) tagged 1000 + 10 row + column
uint32_t freeBoard(uint32_t self) {
    uint32_t base = 1000;
    for (uint32_t row = 0; row < 5; row++, base += 10)
        for (uint32_t col = 0; col < 9; col++) {
            const uint32_t s = ld32(self + 0x370 + 4 * (9 * row + col));
            vcall(s, V_setTag304, {s, base + col, 1});
        }
    return base;
}
RW_FUNCTION(0x793AD0, "SphereUI::free_board", sphere_small::freeBoard);

uint32_t selectIncentive(uint32_t self) {
    Frame f;
    const uint32_t a = f.alloc(4), b = f.alloc(4);
    return call(SphereUI_selectIncentiveSub, {self, a, b});
}
RW_FUNCTION(0x7958F0, "SphereUI::select_incentive", sphere_small::selectIncentive);

uint32_t loadMedalTexture(uint32_t, uint32_t id) { return loadTexture(S_FmtMedal, id); }
RW_FUNCTION(0x7962E4, "SphereUI::loadMedalTexture", sphere_small::loadMedalTexture);
uint32_t loadMaterialTexture(uint32_t, uint32_t id) { return loadTexture(S_FmtMaterial, id); }
RW_FUNCTION(0x7963B0, "SphereUI::loadMaterialTexture", sphere_small::loadMaterialTexture);
uint32_t loadKeyBladeTexture(uint32_t, uint32_t id) { return loadTexture(S_FmtKeyblade, id); }
RW_FUNCTION(0x79647C, "SphereUI::loadKeyBladeTexture", sphere_small::loadKeyBladeTexture);
uint32_t loadSkillTexture(uint32_t, uint32_t id) { return loadTexture(S_FmtSkill, id); }
RW_FUNCTION(0x796548, "SphereUI::loadSkillTexture", sphere_small::loadSkillTexture);
uint32_t loadAvatarSetTexture(uint32_t, uint32_t id) { return loadTexture(S_FmtAvatarSet, id); }
RW_FUNCTION(0x796614, "SphereUI::loadAvatarSetTexture", sphere_small::loadAvatarSetTexture);
uint32_t loadAvatarTexture(uint32_t, uint32_t id) { return loadTexture(S_FmtAvatar, id); }
RW_FUNCTION(0x7966E0, "SphereUI::loadAvatarTexture", sphere_small::loadAvatarTexture);

uint32_t getSkillImage(uint32_t, uint32_t id) { return masterWord(Skill_create, id, 8); }
RW_FUNCTION(0x797F3C, "SphereUI::get_skill_image", sphere_small::getSkillImage);
uint32_t getSkillName(uint32_t out, uint32_t, uint32_t id) { return masterName(out, Skill_create, id, 0xC); }
RW_FUNCTION(0x79802C, "SphereUI::get_skill_name", sphere_small::getSkillName);
uint32_t getMedalName(uint32_t out, uint32_t, uint32_t id) { return masterName(out, Medal_create, id, 0x10); }
RW_FUNCTION(0x7980AC, "SphereUI::get_medal_name", sphere_small::getMedalName);
uint32_t getMedalImage(uint32_t, uint32_t id) { return masterWord(Medal_create, id, 4); }
RW_FUNCTION(0x79812C, "SphereUI::get_medal_image", sphere_small::getMedalImage);
uint32_t getKeybladeName(uint32_t out, uint32_t, uint32_t id) { return masterName(out, Keyblade_create, id, 4); }
RW_FUNCTION(0x7981A4, "SphereUI::get_keyblade_name", sphere_small::getKeybladeName);
uint32_t getKeybladeImage(uint32_t, uint32_t id) { return masterWord(Keyblade_create, id, 0xCC); }
RW_FUNCTION(0x798224, "SphereUI::get_keyblade_image", sphere_small::getKeybladeImage);
uint32_t getMaterialName(uint32_t out, uint32_t, uint32_t id) { return masterName(out, Material_create, id, 0xC); }
RW_FUNCTION(0x7982A0, "SphereUI::get_material_name", sphere_small::getMaterialName);
// materials have no image id: the row is looked up and 0 returned
uint32_t getMaterialImage(uint32_t, uint32_t id) {
    Frame f;
    const uint32_t sp = f.alloc(8);
    call(Material_create, {sp, id});
    if (ld32(sp + 4)) call(fn::shared_release, {ld32(sp + 4)});
    return 0;
}
RW_FUNCTION(0x798320, "SphereUI::get_material_image", sphere_small::getMaterialImage);

uint32_t boardCreateScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneSphereBoard_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x6BE1B4, "SceneSphereBoard::createScene", sphere_small::boardCreateScene);

uint32_t changeSceneNext(uint32_t self, uint32_t) {
    Frame f;
    const uint32_t fo = newFunction(f, VT_BoardOut, {});
    call(IInOutAnimation_runOutAnimation, {self + 0x288, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6BF08C, "SceneSphereBoard::changeSceneNext", sphere_small::changeSceneNext);

uint32_t boardBack(uint32_t) {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, VT_BoardBack, {});
    call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6C02AC, "SceneSphereBoard out animation done lambda (sub_6C02AC)", sphere_small::boardBack);

// SceneSphereBoardSelect {node, scene}: the composed medal's large LWF (mixture/medal/compose/<id>/...) made by
// the node's virtual +0x2F0 (its result dropped)
uint32_t composeLwf(uint32_t self, uint32_t valueRef) {
    const uint32_t value = ld32(valueRef);
    const uint32_t node = ld32(self + 4);
    const uint32_t make = ld32(ld32(node) + 0x2F0);
    const uint32_t sb = ld32(ld32(ld32(self + 8) + 0x2A0) + 0x58);
    Frame f;
    const uint32_t id = f.alloc(4);
    call(SafeBuffer_read, {sb, id, ld32(sb + 0xC) + 4, 4, 4});
    const uint32_t buf = f.alloc(0x404);
    call(StringBuffer_ctor, {buf, S_FmtComposeLwf, ld32(id)});
    const uint32_t none = newEmptyFunction(f);
    const uint32_t out = f.alloc(8);
    call(make, {out, node, buf, value, S_Empty, none});
    if (ld32(out + 4)) call(fn::shared_release, {ld32(out + 4)});
    functionDestroy(none);
    return 0;
}
RW_FUNCTION(0x6B32D0, "SceneSphereBoardSelect compose medal LWF lambda (sub_6B32D0)", sphere_small::composeLwf);

// {medal shared_ptr}: the medal's info popup (user data byte +0x54 picks its mode)
uint32_t medalInfo(uint32_t self) {
    call(AudioPlaySystemSEDecide, {});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t medal = f.alloc(8);
    st32(medal, ld32(self + 4));
    st32(medal + 4, ld32(self + 8));
    if (ld32(medal + 4)) call(fn::shared_add, {ld32(medal + 4)});
    const uint32_t sb = ld32(api + 0x94);
    const uint32_t flag = f.alloc(4);
    call(SafeBuffer_read, {sb, flag, ld32(sb + 0xC) + 0x54, 1, 0x54});
    const uint32_t a = newEmptyFunction(f), b = newEmptyFunction(f);
    call(SceneUtil_openMedalInfoPopup, {medal, ld8(flag) ? 1u : 0u, 0, 0, a, 0, b, 0});
    functionDestroy(b);
    functionDestroy(a);
    if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
    return 0;
}
RW_FUNCTION(0x6B357C, "SceneSphereBoardSelect medal info lambda (sub_6B357C)", sphere_small::medalInfo);

// the spheres a board opens: the user's sphere `id` (APIManager +0xB4 map, its control block +0xB8), its open
// list copied into out
uint32_t getSphereOpenlist(uint32_t out, uint32_t, uint32_t id) {
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, id);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + 0xB8), map = ld32(api + 0xB4);
    if (ctrl) call(fn::shared_add, {ctrl});
    const uint32_t it = call(map_sphere_find, {map, key});
    call(vector_int_copy, {out, it + 0x28});
    if (ctrl) call(fn::shared_release, {ctrl});
    return 0;
}
RW_FUNCTION(0x798E30, "ConnectingSphere::getSphereOpenlist", sphere_small::getSphereOpenlist);

// LuxBoard {board}: when shown and ready, the current sprite (+0x278 + 4 * index +0x378) added to the layer
// +0x290 unless it already has a parent
uint32_t luxSprite(uint32_t captures) {
    const uint32_t board = ld32(captures + 4);
    uint32_t r = ld8(board + 0x3F9);
    if (!r) return r;
    r = ld8(board + 0x3F8);
    if (!r) return r;
    const uint32_t sprite = ld32(board + 4 * ld32(board + 0x378) + 0x278);
    r = vcall(sprite, 0x11C, {sprite});
    if (r) return r;
    const uint32_t layer = ld32(board + 0x290);
    return vcall(layer, V_addChild, {layer, sprite});
}
RW_FUNCTION(0x5D2E18, "LuxBoard sprite lambda (sub_5D2E18)", sphere_small::luxSprite);

}  // namespace sphere_small
