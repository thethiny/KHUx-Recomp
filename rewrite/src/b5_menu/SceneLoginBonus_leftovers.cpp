// SceneLoginBonus leftovers: createScene, the texture loaders of the reward images, the memory bonus panel
// (initMemoryBonus: the day's loginBonus row - title, text, reward name / image / count) and its stamp animation
// (openMemoryBonus, then $_ sub_6FA97C plays the panel's armature).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t SceneLoginBonus_create = 0x6F6329;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance, Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache;
constexpr uint32_t TextureCache_addImage = rwlib::cocos2d::TextureCache::addImage;      // (cache, const string&)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__PNS_9Texture2DE;      // (view, Texture2D*)
constexpr uint32_t make_getIncentive_name = 0x6F90E5;
constexpr uint32_t loadIncentiveTexture = 0x6FA0D5;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;        // (path, bool)
constexpr uint32_t Movie_GotoAndPlay = rwlib::LWF::Movie::GotoAndPlay__i;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create, CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;     // (shared_ptr out, manager, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;         // (layer, id, int, int)
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;
constexpr uint32_t shared_add = 0xC8DBB5, shared_release = 0xC8DBD1;
}  // namespace g

constexpr uint32_t V_addChild = 0xF0;
constexpr uint32_t V_setVisible = 0xB0;

uint32_t createScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(g::SceneLoginBonus_create, {});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}

// a texture from the cache by a printf path
uint32_t loadTexture(uint32_t fmt, uint32_t id) {
    const uint32_t cache = call(g::Director_getTextureCache, {call(g::Director_getInstance, {})});
    Frame f;
    const uint32_t path = newStr(f, stringBuffer(f, {fmt, id}));
    const uint32_t tex = call(g::TextureCache_addImage, {cache, path});
    strFree(path);
    return tex;
}
uint32_t loadAvatarSetTexture(uint32_t, uint32_t id) { return loadTexture(0xDF92BE, id); }   // "img/avatarParts/ai%03d.png"
uint32_t loadAvatarTexture(uint32_t, uint32_t id) { return loadTexture(0xDF92D9, id); }      // "img/avatarParts/ai%d.png"
uint32_t loadMedalTexture(uint32_t, uint32_t id) { return loadTexture(0xE06393, id); }       // "img/medal/Medal_S_%d.png"
uint32_t loadKeyBladeTexture(uint32_t, uint32_t id) { return loadTexture(0xE063AC, id); }    // "img/KeyBlade/KeyBlade_%06d.png"
uint32_t loadMaterialTexture(uint32_t, uint32_t id) { return loadTexture(0xE063CB, id); }    // "img/material/material_%d.png"

// loginBonus fields
constexpr uint32_t LB_NAME = 8, LB_TEXT = 0x109, LB_TYPE = 0x920, LB_ID = 0x924, LB_COUNT = 0x934;

uint32_t readInt(uint32_t row, uint32_t offset, uint32_t dst) {
    call(g::SafeBuffer_read, {row, dst, ld32(row + 0xC) + offset, 4, offset});
    return ld32(dst);
}
void readText(uint32_t label, uint32_t row, uint32_t offset, uint32_t size) {
    Frame f;
    const uint32_t buf = f.alloc((size + 7) & ~7u);
    call(g::SafeBuffer_read, {row, buf, ld32(row + 0xC) + offset, size, offset});
    const uint32_t s = newStr(f, buf);
    call(g::Text_setString, {label, s});
    strFree(s);
}

// The memory bonus panel ("BonusMemory") for the current entry (+0x2C0) of the bonus list (+0x2B4)
uint32_t initMemoryBonus(uint32_t self) {
    const uint32_t list = ld32(self + 0x2B4), index = ld32(self + 0x2C0);
    if ((ld32(self + 0x2B8) - list) >> 3 <= index) call(g::THROW_OUT_OF_RANGE, {});
    const uint32_t row = ld32(list + 8 * index), ctrl = ld32(list + 8 * index + 4);
    if (ctrl) call(g::shared_add, {ctrl});
    const uint32_t panel = findNode(ld32(self + 0x28C), 0xE06326, 11);   // "BonusMemory"
    {
        const uint32_t title = findNode(panel, 0xE0636F, 6);             // "Title1"
        call(g::SceneUtil_setShadow, {title});
        readText(title, row, LB_NAME, 0x101);
    }
    {
        const uint32_t text = findNode(panel, 0xE06480, 16);             // "Txt_Memory_Label"
        call(g::SceneUtil_setShadow, {text});
        readText(text, row, LB_TEXT, 0x401);
    }
    Frame f;
    const uint32_t imageId = f.alloc(4), word = f.alloc(4);
    st32(imageId, 0);
    {
        const uint32_t get = findNode(panel, 0xE06491, 20);              // "Txt_Memory_Get_Label"
        call(g::SceneUtil_setShadow, {get});
        const uint32_t type = readInt(row, LB_TYPE, word);
        const uint32_t id = readInt(row, LB_ID, word);
        const uint32_t count = readInt(row, LB_COUNT, word);
        Frame t;
        const uint32_t name = t.alloc(12);
        call(g::make_getIncentive_name, {name, imageId, type, id, count, imageId});
        call(g::Text_setString, {get, name});
        strFree(name);
    }
    const uint32_t image = findNode(panel, 0xE04C9C, 6);                 // "Memory"
    const uint32_t texId = ld32(imageId);
    if (texId) {
        uint32_t tex = 0;
        bool shown = true;
        switch (int32_t(readInt(row, LB_TYPE, word))) {
        case 2: case 4: case 6: case 7: case 11: case 15: case 16: case 17: case 26:
            tex = call(g::loadIncentiveTexture, {0, texId}); break;
        case 3: tex = loadMedalTexture(0, texId); break;
        case 5: tex = loadMaterialTexture(0, texId); break;
        case 13: tex = loadKeyBladeTexture(0, texId); break;
        case 18: tex = loadAvatarSetTexture(0, texId); break;
        case 19: case 20: tex = loadAvatarTexture(0, texId); break;
        default: shown = false; break;
        }
        if (shown) {
            call(g::ImageView_loadTexture, {image, tex});
            vcall(image, 0x48, {image, rw::fbits(1.0f), rw::fbits(1.0f)});   // setScale(1, 1)
        }
    }
    {
        const uint32_t num = findNode(panel, 0xE04CA3, 19);              // "Txt_MemoryNum_Label"
        const uint32_t count = readInt(row, LB_COUNT, word);
        Frame t;
        const uint32_t s = newStr(t, stringBuffer(t, {0xDFD670, count}));
        call(g::Text_setString, {num, s});
        strFree(s);
    }
    if (ctrl) call(g::shared_release, {ctrl});
    return 0x9CC;   // r0 at the return: the frame size (add sp, r0)
}

// The stamp: "Area_Point" emptied (its first child removed) and the LoginBonus_Stamp LWF played over it with the
// stamp SE; when it ends ($_ {self}, at least 0.1 s) the panel's armature plays
constexpr uint32_t VT_STAMPED = 0xF69EE0;
void openMemoryBonus(uint32_t self) {
    const uint32_t panel = findNode(ld32(self + 0x28C), 0xE06326, 11);   // "BonusMemory"
    const uint32_t area = findNode(panel, 0xDF9FB4, 10);                 // "Area_Point"
    if (int32_t(vcall(area, 0x114, {area})) >= 1) vcall(area, 0x13C, {area, 1});   // children: removeAllChildren
    const uint32_t lwf = call(g::AudioLWFNode_create, {0xE06420, 0});    // "lwf/loginbonus/LoginBonus_Stamp/..."
    vcall(area, V_addChild, {area, lwf});
    const uint32_t lwfObj = ld32(lwf + 0x3E0);
    call(g::Movie_GotoAndPlay, {ld32(lwfObj + 0x18), ld32(ld32(lwfObj + 0x18) + 0x98)});
    float t = float(int32_t(ld32(ld32(lwfObj + 0x18) + 0x98))) / 60.0f;
    if (t < 0.1f) t = 0.1f;
    const uint32_t delay = call(g::DelayTime_create, {rw::fbits(t)});
    {
        Frame f;
        const uint32_t fo = newFunction(f, VT_STAMPED, {self});
        const uint32_t done = call(g::CallFunc_create, {fo});
        call(g::Node_runAction, {lwf, call(g::Sequence_create, {delay, done, 0})});
        functionDestroy(fo);
    }
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(g::audio_Manager_getLayer, {layer, call(g::audio_Manager_getInstance, {}), 0});
    call(g::audio_Layer_playSE, {ld32(layer), 509100630, 0, 0});
    if (ld32(layer + 4)) call(g::shared_release, {ld32(layer + 4)});
}

// sub_6FA97C, the stamp ended {self}: the panel's armature (+0x2DC) plays "Animation1" and shows (+0x2D9 set)
void stamped(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    st8(self + 0x2D9, 1);
    const uint32_t armature = ld32(self + 0x2DC);
    const uint32_t anim = vcall(armature, 0x298, {armature});
    const uint32_t play = ld32(ld32(anim) + 0x68);
    Frame f;
    const uint32_t name = f.alloc(12);
    st8(name, 20);
    call(g::MEMCPY, {name + 1, 0xDFD0AF, 10});                        // "Animation1"
    st8(name + 11, 0);
    call(play, {anim, name, 0xFFFFFFFF, 0xFFFFFFFF});
    strFree(name);
    const uint32_t a = ld32(self + 0x2DC);
    vcall(a, V_setVisible, {a, 1});
}
}  // namespace

RW_FUNCTION(0x6F6304, "SceneLoginBonus::createScene()", createScene);
RW_FUNCTION(0x6F9CD8, "SceneLoginBonus::loadAvatarSetTexture(int)", loadAvatarSetTexture);
RW_FUNCTION(0x6F9DA4, "SceneLoginBonus::loadAvatarTexture(int)", loadAvatarTexture);
RW_FUNCTION(0x6F9E70, "SceneLoginBonus::loadMedalTexture(int)", loadMedalTexture);
RW_FUNCTION(0x6F9F3C, "SceneLoginBonus::loadKeyBladeTexture(int)", loadKeyBladeTexture);
RW_FUNCTION(0x6FA008, "SceneLoginBonus::loadMaterialTexture(int)", loadMaterialTexture);
RW_FUNCTION(0x6F87E0, "SceneLoginBonus::initMemoryBonus()", initMemoryBonus);
RW_FUNCTION(0x6FA6F4, "SceneLoginBonus::openMemoryBonus(function<void()>)", openMemoryBonus);
RW_FUNCTION(0x6FA97C, "SceneLoginBonus memory bonus stamped (sub_6FA97C)", stamped);
