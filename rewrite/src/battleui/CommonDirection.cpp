// CommonDirection: skill / material panels, boss direction markers and the full-screen fade layer of a battle.
#include "CommonDirection.hpp"

#include <cmath>

namespace {
using scene::Frame;
using scene::call;
using scene::ld32;
using scene::ldf;

// ---- library / game functions (Thumb addresses as the lifted code calls them)
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Node_dtor = 0x8927ED;
constexpr uint32_t Node_create = 0x89372D;
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Node_getNumberOfRunningActions = 0x894B4F;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t function_assign = 0x4EB9CD;          // std::function<void()>::operator=(const function&)
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_load = 0x5BEEF5;       // (manager, ResourceID)
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;   // (shared_ptr<Layer> out, manager, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;       // (layer, ResourceID, int, int)
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;          // cocostudio::Armature::create(const string&)
constexpr uint32_t ArmatureWS_create = 0x7652FD;
constexpr uint32_t Bone_addDisplay = 0x7A2657;          // (bone, node, index)
constexpr uint32_t DisplayManager_addDisplay = 0x7AAA5D;
constexpr uint32_t DisplayManager_removeDisplay = 0x7AAE09;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t Director_getVisibleSize = 0x8F0675;
constexpr uint32_t Director_getVisibleOrigin = 0x8F06A1;
constexpr uint32_t FadeIn_create = 0x86B115;
constexpr uint32_t FadeOut_create = 0x86B25D;
constexpr uint32_t FadeTo_create = 0x86B3E1;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t Sequence_create = 0x867999;          // (action, ..., nullptr)
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t MoveTo_create = 0x86941D;            // (duration, const Vec2&)
constexpr uint32_t CallFunc_create = 0x866B39;          // (const std::function<void()>&)
constexpr uint32_t LayerColor_create = 0x88B539;        // (const Color4B&)
constexpr uint32_t Label_createWithTTF = 0x883AE5;      // (TTFConfig&, const string&, hAlign, maxLineWidth)
constexpr uint32_t Label_setDimensions = 0x883A8D;
constexpr uint32_t Label_setAlignment = 0x884CB9;       // (label, hAlign, vAlign)
constexpr uint32_t Sprite_create = 0x8A637D;            // (const string&)
constexpr uint32_t Vec2_distance = 0x8E4EF5;
constexpr uint32_t AppGetGrameFrameWidth = 0x799B31;
constexpr uint32_t AppGetGrameFrameHeight = 0x799D21;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t StageState_cmpSubMission14 = 0x4EFD55;
constexpr uint32_t master_Material_create = 0x7852E5;   // (shared_ptr out, id)
constexpr uint32_t master_Skill_create = 0x78BDE9;
constexpr uint32_t StringBuffer_ctor = 0x49140D;        // (buffer, fmt, ...)
constexpr uint32_t StageManager_MarkingMapTransportGate = 0x75C465;
constexpr uint32_t libm_atan2 = 0x42287C;               // atan2(double, double) (import)
constexpr uint32_t libm_sinf = 0x41A9E0;
constexpr uint32_t libm_cosf = 0x41C8AC;

// GOT entries of cocos2d colour constants
constexpr uint32_t GOT_Color3B_WHITE = 0xFD6060;
constexpr uint32_t GOT_Color3B_BLACK = 0xFD60CC;
constexpr uint32_t GOT_Color4B_BLACK = 0xFD65C8;

// strings
constexpr uint32_t S_EMPTY = 0xDF8A1A;                  // ""
constexpr uint32_t S_ACTION_MAP_ANIMATION = 0xE01C44;   // "ActionMapAnimation" (18)
constexpr uint32_t S_ACTION_MAP_JSON = 0xE02831;        // "cocostudio/publish/ActionMapAnimation.ExportJson" (48)
constexpr uint32_t S_FONT = 0xDF9F4D;                   // "cocostudio/publish/DFPSoGei-W5.TTF" (34)
constexpr uint32_t S_MATERIAL_PANEL_TEXT = 0xE077ED;    // "MaterialPanelText" (17)
constexpr uint32_t S_MATERIAL_ICON = 0xDF9101;          // "img/material/material_%d.png"
constexpr uint32_t S_MATERIAL = 0xE077FF;               // "Material" (8)
constexpr uint32_t S_SKILL_PANEL_TEXT = 0xE077E0;       // "SkillPanel03" (12)

constexpr uint32_t SE_PANEL = 0x1E56BAF8;               // audio ResourceID of the panel sound

// std::function<void()> lambda vtables (__func vtable + 8)
constexpr uint32_t VT_BATTLE_START = 0xF6E644;          // battle_start $_12: fadeLayer->setVisible(false)
constexpr uint32_t VT_HEAL = 0xF6E670;                  // heal $_11: fadeLayer->setVisible(false)
constexpr uint32_t VT_SHOW_MARKER = 0xF6E69C;           // show_boss_marker $_10: marker->setVisible(true)
constexpr uint32_t VT_HIDE_MARKER = 0xF6E6C8;           // hide_boss_marker $_9: marker->setVisible(false)
constexpr uint32_t VT_MATERIAL_HIDE = 0xF6E6F4;         // materialPanel_in $_8
constexpr uint32_t VT_MATERIAL_OUT = 0xF6E720;          // materialPanel_in $_7
constexpr uint32_t VT_MATERIAL_HOLD = 0xF6E74C;         // materialPanel_in $_6
constexpr uint32_t VT_PANEL_HOLD = 0xF6E778;            // panel_update $_5 (heap: 4 captures)
constexpr uint32_t VT_HOLD_OUT = 0xF6E7A4;              // skilPanel_hold $_4
constexpr uint32_t VT_OUT_DONE = 0xF6E7D0;              // skilPanel_out_common $_3
constexpr uint32_t VT_IN_DONE = 0xF6E7FC;               // skilPanel_in $_2
constexpr uint32_t VT_STOP_UPDATE = 0xF6E828;           // init $_1
constexpr uint32_t VT_START_UPDATE = 0xF6E854;          // init $_0

// cocos2d::Node / Armature / Label virtual slots (byte offsets)
constexpr uint32_t V_SET_SCALE = 0x40;
constexpr uint32_t V_SET_POSITION = 0x4C;
constexpr uint32_t V_GET_POSITION = 0x54;
constexpr uint32_t V_SET_POSITION_XY = 0x5C;
constexpr uint32_t V_SET_CONTENT_SIZE = 0xA8;
constexpr uint32_t V_SET_VISIBLE = 0xB0;
constexpr uint32_t V_SET_ROTATION = 0xB8;
constexpr uint32_t V_ADD_CHILD = 0xF0;
constexpr uint32_t V_ADD_CHILD_Z = 0xF4;
constexpr uint32_t V_INIT = 0x248;
constexpr uint32_t V_SET_OPACITY = 0x214;
constexpr uint32_t V_SET_COLOR = 0x22C;
constexpr uint32_t V_BONE_ADD_CHILD = 0x160;            // cocostudio::Bone virtual taking the display node
constexpr uint32_t V_ARMATURE_GET_BONE = 0x27C;         // Armature::getBone(const string&)
constexpr uint32_t V_ARMATURE_GET_ANIMATION = 0x298;
constexpr uint32_t V_BONE_GET_DISPLAY_MANAGER = 0x29C;
constexpr uint32_t V_LABEL_ENABLE_SHADOW = 0x2B4;       // (label, const Color4B&, const Size&, blur)
constexpr uint32_t V_ANIMATION_PLAY_WITH_INDEX = 0x70;  // ArmatureAnimation::playWithIndex(index, durationTo, loop)
constexpr uint32_t LABEL_VALIGN = 0x3E0;                // cocos2d::Label vertical alignment field

constexpr uint32_t NONE = 0xFFFFFFFFu;

uint32_t addr(const void *p) { return rw::guest_addr(p); }
uint32_t fb(float f) { return rw::fbits(f); }

void setVisible(uint32_t node, uint32_t visible) { scene::vcall(node, V_SET_VISIBLE, {node, visible}); }
void setOpacity(uint32_t node, uint32_t opacity) { scene::vcall(node, V_SET_OPACITY, {node, opacity}); }

// armature->getAnimation()->playWithIndex(index, -1, -1)
void playIndex(uint32_t armature, uint32_t index) {
    uint32_t anim = scene::vcall(armature, V_ARMATURE_GET_ANIMATION, {armature});
    scene::vcall(anim, V_ANIMATION_PLAY_WITH_INDEX, {anim, index, NONE, NONE});
}

// CallFunc::create(std::function holding an inline lambda {captures}); the temporary is destroyed after
uint32_t callFunc(uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fn = scene::newFunction(f, vtable, captures);
    uint32_t action = call(CallFunc_create, {fn});
    scene::functionDestroy(fn);
    return action;
}

uint32_t sequence2(uint32_t a, uint32_t b) { return call(Sequence_create, {a, b, 0}); }

void runSequence2(uint32_t node, uint32_t a, uint32_t b) { call(Node_runAction, {node, sequence2(a, b)}); }

uint32_t armatureCreate(uint32_t create) {
    Frame f;
    uint32_t s = scene::newStr(f, S_ACTION_MAP_ANIMATION, 18);
    uint32_t armature = call(create, {s});
    scene::strFree(s);
    return armature;
}

// armature->getBone(name)
uint32_t getBone(uint32_t armature, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = scene::newStr(f, name, len);
    uint32_t bone = scene::vcall(armature, V_ARMATURE_GET_BONE, {armature, s});
    scene::strFree(s);
    return bone;
}

// Label::createWithTTF(self->fontFile config, text) with text = the printf of `fmt` (args)
uint32_t labelFromBuffer(CommonDirection *self, uint32_t text) {
    Frame f;
    uint32_t s = scene::newStr(f, text);
    uint32_t label = call(Label_createWithTTF, {addr(self->fontFile), s, 0, 0});
    scene::strFree(s);
    return label;
}

// the panel label style: white, black half-transparent shadow offset (2, -2), blur 6
void labelShadow(uint32_t label) {
    Frame f;
    uint32_t color = f.alloc(4);
    rw::frame_ptr<uint8_t>(color)[0] = 0;
    rw::frame_ptr<uint8_t>(color)[1] = 0;
    rw::frame_ptr<uint8_t>(color)[2] = 0;
    rw::frame_ptr<uint8_t>(color)[3] = 0x80;
    uint32_t size = scene::newVec2(f, 2.0f, -2.0f);
    scene::vcall(label, V_LABEL_ENABLE_SHADOW, {label, color, size, 6});
}

void setPosition(uint32_t node, float x, float y) {
    Frame f;
    scene::vcall(node, V_SET_POSITION, {node, scene::newVec2(f, x, y)});
}

// A 65-byte name from a master row (SafeBuffer data position 12) run through StringBuffer (printf with the name
// as the format, as the original does), into a guest buffer.
uint32_t rowName(Frame &f, uint32_t row) {
    uint32_t name = scene::safeBytes(f, row, 12, 0x41);
    uint32_t buf = f.alloc(scene::STRING_BUFFER_SIZE);
    call(StringBuffer_ctor, {buf, name});
    return buf;
}

// a 32-bit value at data position `index` of a master row
int32_t rowInt(uint32_t row, uint32_t index) { return scene::safe32(row, index); }

uint32_t visibleSize(Frame &f) {
    uint32_t out = f.alloc(8);
    call(Director_getVisibleSize, {out, call(Director_getInstance, {})});
    return out;
}
uint32_t visibleOrigin(Frame &f) {
    uint32_t out = f.alloc(8);
    call(Director_getVisibleOrigin, {out, call(Director_getInstance, {})});
    return out;
}

// the fade layer's colour / opacity / visibility, then fade to 204 and back to 0 before `done`
void flash(CommonDirection *self, uint32_t colorGot, uint32_t doneVtable) {
    uint32_t in = call(EaseSineOut_create, {call(FadeTo_create, {fb(0.25f), 204})});
    uint32_t out = call(EaseSineOut_create, {call(FadeTo_create, {fb(0.75f), 0})});
    uint32_t layer = self->fadeLayer.addr;
    scene::vcall(layer, V_SET_COLOR, {layer, ld32(colorGot)});
    layer = self->fadeLayer.addr;
    setOpacity(layer, 0);
    setVisible(self->fadeLayer.addr, 1);
    layer = self->fadeLayer.addr;
    uint32_t done = callFunc(doneVtable, {addr(self)});
    call(Node_runAction, {layer, call(Sequence_create, {in, out, done, 0})});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t CommonDirection::create() {
    uint32_t obj = guest::newObject(0x388);
    construct(rw::host_ptr<CommonDirection>(obj));
    if (scene::vcall(obj, V_INIT, {obj}) == 1) {
        call(Ref_autorelease, {obj});
        return obj;
    }
    scene::vcall(obj, 4, {obj});                         // deleting destructor
    return 0;
}
RW_FUNCTION(0x5FC920, "CommonDirection::create", CommonDirection::create);

CommonDirection *CommonDirection::construct(CommonDirection *self) {
    call(Node_ctor, {addr(self)});
    rw::set<uint32_t>(self, 0, VTABLE);
    self->mode = 0;
    rw::set<uint32_t>(self, 0x27C, 0);                   // clock (stored as an integer 0)
    scene::strInit(addr(self->fontFile), S_EMPTY, 0);
    self->fontSize = 12;
    self->glyphs = 0;
    self->customGlyphs = 0;
    rw::set<uint8_t>(self, 0x298, 0);
    self->outlineSize = 0;
    rw::set<uint8_t>(self, 0x2A0, 0);
    rw::set<uint8_t>(self, 0x2A1, 0);
    self->bossMarkerArrow = nullptr;
    self->bossMarker = nullptr;
    self->materialId = -1;
    self->warningLevel = -1;
    self->warningOn = 1;
    self->markerHidden = 0;
    return self;
}
RW_FUNCTION(0x7649A8, "CommonDirection::CommonDirection", CommonDirection::construct);

CommonDirection *CommonDirection::destruct(CommonDirection *self) {
    rw::set<uint32_t>(self, 0, VTABLE);
    if (self->panelLayer) call(Ref_release, {self->panelLayer.addr});
    if (self->markerLayer) call(Ref_release, {self->markerLayer.addr});
    scene::strFree(addr(self->fontFile));
    call(Node_dtor, {addr(self)});
    return self;
}
RW_FUNCTION(0x764A60, "CommonDirection::~CommonDirection", CommonDirection::destruct);

void CommonDirection::destructDelete(CommonDirection *self) {
    guest::deleteObject(addr(destruct(self)));
}
RW_FUNCTION(0x764AE8, "CommonDirection::~CommonDirection (deleting)", CommonDirection::destructDelete);

uint32_t CommonDirection::init(CommonDirection *self) {
    self->flag_378 = 0;
    self->materialId = -1;
    uint32_t n = call(Node_create, {});
    self->panelLayer = rw::Ptr<uint8_t>{n};
    call(Ref_retain, {n});
    n = call(Node_create, {});
    self->markerLayer = rw::Ptr<uint8_t>{n};
    call(Ref_retain, {n});
    call(audio_Manager_load, {call(audio_Manager_getInstance, {}), SE_PANEL});
    {
        Frame f;
        uint32_t fn = scene::newFunction(f, VT_START_UPDATE, {addr(self)});
        call(function_assign, {addr(self->startUpdate), fn});
        scene::functionDestroy(fn);
    }
    {
        Frame f;
        uint32_t fn = scene::newFunction(f, VT_STOP_UPDATE, {addr(self)});
        call(function_assign, {addr(self->stopUpdate), fn});
        scene::functionDestroy(fn);
    }
    return 1;
}
RW_FUNCTION(0x764AF4, "CommonDirection::init", CommonDirection::init);

// Load the panel armatures: six skill panels (front / back), the material panel, the target-select marker and
// the two boss markers; set the label font.
void CommonDirection::preloadResources(CommonDirection *self) {
    // The original moves a stack TTFConfig into fontFile..italicsBold; its distanceField bool is copied as a whole
    // word, so the three padding bytes are the stale stack bytes at that slot (entry sp - 0x24), read before any
    // call reuses the stack.
    uint32_t staleWord = rw::load<uint32_t>(rw::cpu->r[13] - 0x24);
    {
        uint32_t mgr = call(ArmatureDataManager_getInstance, {});
        Frame f;
        uint32_t s = scene::newStr(f, S_ACTION_MAP_JSON, 0x30);
        call(ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        scene::strFree(s);
    }
    {
        Frame f;
        uint32_t s = scene::newStr(f, S_FONT, 0x22);
        scene::strMoveAssign(addr(self->fontFile), s);
        self->distanceField = staleWord & 0xFFFFFF00u;
        self->customGlyphs = 0;
        self->glyphs = 0;
        self->fontSize = 18;
        self->outlineSize = 0;
        self->italicsBold = 0;
    }
    Frame f;
    uint32_t size = visibleSize(f);
    for (uint32_t k = 0; k < 11; k++) rw::store32(&self->skillQueue[0] + k, 0);   // memset(+0x348, 0, 0x2C)
    for (uint32_t k = 0; k < 6; k++) {
        self->skillPanel[k] = rw::Ptr<uint8_t>{armatureCreate(Armature_create)};
        self->skillPanelBack[k] = rw::Ptr<uint8_t>{armatureCreate(Armature_create)};
        setVisible(self->skillPanel[k].addr, 0);
        setVisible(self->skillPanelBack[k].addr, 0);
        uint32_t layer = self->panelLayer.addr;
        scene::vcall(layer, V_ADD_CHILD_Z, {layer, self->skillPanel[k].addr, 11});
        layer = self->panelLayer.addr;
        scene::vcall(layer, V_ADD_CHILD_Z, {layer, self->skillPanelBack[k].addr, 12});
        self->panelSkillId[k] = 0;
        self->panelState[k] = -1;
        self->panelOrder[k] = -1;
    }
    self->materialPanel = rw::Ptr<uint8_t>{armatureCreate(Armature_create)};
    setVisible(self->materialPanel.addr, 0);
    uint32_t layer = self->panelLayer.addr;
    scene::vcall(layer, V_ADD_CHILD_Z, {layer, self->materialPanel.addr, 11});
    self->selectTarget = rw::Ptr<uint8_t>{armatureCreate(Armature_create)};
    setVisible(self->selectTarget.addr, 0);
    layer = self->panelLayer.addr;
    scene::vcall(layer, V_ADD_CHILD_Z, {layer, self->selectTarget.addr, 12});
    float w = ldf(size), h = ldf(size + 4);
    uint32_t target = self->selectTarget.addr;
    scene::vcall(target, V_SET_POSITION_XY, {target, fb(w * 0.5f), fb(h + -128.0f)});
    gene_boss_marker(self, &self->bossMarkerArrow.addr, 12);
    gene_boss_marker(self, &self->bossMarker.addr, 11);
    setPosition(self->markerLayer.addr, w * 0.5f, h * 0.5f);
    if (self->mode == 2) {
        uint32_t m = self->markerLayer.addr;
        scene::vcall(m, V_SET_SCALE, {m, fb(0.5f)});
    }
}
RW_FUNCTION(0x764C1C, "CommonDirection::preloadResources", CommonDirection::preloadResources);

// Create (once) and show a boss marker armature playing `animation`, fading in at the centre of the screen.
void CommonDirection::gene_boss_marker(CommonDirection *self, const uint32_t *marker, int32_t animation) {
    if (self->mode != 0) return;
    Frame f;
    uint32_t origin = visibleOrigin(f);
    uint32_t size = visibleSize(f);
    if (*marker == 0) {
        uint32_t m = armatureCreate(ArmatureWS_create);
        rw::store32(marker, m);
        scene::vcall(addr(self), V_ADD_CHILD, {addr(self), *marker});
    }
    call(Node_stopAllActions, {*marker});
    setOpacity(*marker, 0);
    setVisible(*marker, 1);
    playIndex(*marker, uint32_t(animation));
    uint32_t m = *marker;
    call(Node_runAction, {m, call(Sequence_create, {call(FadeIn_create, {fb(0.5f)}), 0})});
    float x = ldf(origin) + ldf(size) * 0.5f;
    float y = ldf(origin + 4) + ldf(size + 4) * 0.5f;
    setPosition(*marker, x, y);
}
RW_FUNCTION(0x765010, "CommonDirection::gene_boss_marker", CommonDirection::gene_boss_marker);

void CommonDirection::req_select_target_disp(CommonDirection *self) {
    setVisible(self->selectTarget.addr, 1);
    int32_t mode = self->mode;
    uint32_t target = self->selectTarget.addr;
    uint32_t anim = scene::vcall(target, V_ARMATURE_GET_ANIMATION, {target});
    scene::vcall(anim, V_ANIMATION_PLAY_WITH_INDEX, {anim, mode == 2 ? 22u : 21u, NONE, NONE});
}
RW_FUNCTION(0x76543A, "CommonDirection::req_select_target_disp", CommonDirection::req_select_target_disp);

void CommonDirection::req_select_target_hide(CommonDirection *self) { setVisible(self->selectTarget.addr, 0); }
RW_FUNCTION(0x76547C, "CommonDirection::req_select_target_hide", CommonDirection::req_select_target_hide);

// ------------------------------------------------------------------------------------------------ material panel
void CommonDirection::req_materialPanel_in(CommonDirection *self, int32_t materialId) {
    materialPanel_in(self, materialId);
}
RW_FUNCTION(0x765490, "CommonDirection::req_materialPanel_in", CommonDirection::req_materialPanel_in);

// Show the dropped material's name and icon in the material panel, then play it out (hold 2/3 s, out 2/3 s).
void CommonDirection::materialPanel_in(CommonDirection *self, int32_t materialId) {
    if (self->materialId == materialId) return;
    self->materialId = materialId;
    setVisible(self->materialPanel.addr, 1);
    playIndex(self->materialPanel.addr, 7);
    uint32_t panel = self->materialPanel.addr;
    uint32_t setXY = ld32(ld32(panel) + V_SET_POSITION_XY);
    float x = rw::callf(AppGetGrameFrameWidth, {}) + -154.0f;
    float y = rw::callf(AppGetGrameFrameHeight, {}) + -125.0f;
    call(setXY, {panel, fb(x), fb(y)});

    Frame f;
    uint32_t material = scene::masterRow(f, master_Material_create, uint32_t(materialId));
    uint32_t row = ld32(material);
    uint32_t label;
    {
        Frame g;
        label = labelFromBuffer(self, rowName(g, row));
    }
    setPosition(label, 100.0f, 0.0f);
    {
        Frame g;
        scene::vcall(label, V_SET_CONTENT_SIZE, {label, scene::newVec2(g, 218.0f, 21.0f)});
    }
    call(Label_setDimensions, {label, 0xDA, 0});
    call(Label_setAlignment, {label, 0, ld32(label + LABEL_VALIGN)});
    scene::vcall(label, V_SET_COLOR, {label, ld32(GOT_Color3B_WHITE)});
    labelShadow(label);
    uint32_t bone = getBone(self->materialPanel.addr, S_MATERIAL_PANEL_TEXT, 0x11);
    if (bone) {
        uint32_t dm = scene::vcall(bone, V_BONE_GET_DISPLAY_MANAGER, {bone});
        scene::vcall(bone, V_BONE_ADD_CHILD, {bone, label});
        call(DisplayManager_removeDisplay, {dm, 0});
        call(Bone_addDisplay, {bone, label, 0});
    }
    uint32_t icon;
    {
        Frame g;
        uint32_t buf = scene::stringBuffer(g, {S_MATERIAL_ICON, uint32_t(rowInt(ld32(material), 0))});
        uint32_t s = scene::newStr(g, buf);
        icon = call(Sprite_create, {s});
        scene::strFree(s);
    }
    bone = getBone(self->materialPanel.addr, S_MATERIAL, 8);
    if (bone) {
        uint32_t dm = scene::vcall(bone, V_BONE_GET_DISPLAY_MANAGER, {bone});
        setPosition(icon, 12.0f, 12.0f);
        scene::vcall(bone, V_BONE_ADD_CHILD, {bone, icon});
        call(DisplayManager_removeDisplay, {dm, 0});
        call(DisplayManager_addDisplay, {dm, icon, 0});
    }
    uint32_t delay1 = call(DelayTime_create, {fb(0.6666667f)});
    uint32_t hold = callFunc(VT_MATERIAL_HOLD, {addr(self)});
    uint32_t delay2 = call(DelayTime_create, {fb(0.6666667f)});
    uint32_t out = callFunc(VT_MATERIAL_OUT, {addr(self)});
    uint32_t delay3 = call(DelayTime_create, {fb(0.33333334f)});
    uint32_t hide = callFunc(VT_MATERIAL_HIDE, {addr(self)});
    call(Node_stopAllActions, {self->materialPanel.addr});
    panel = self->materialPanel.addr;
    uint32_t seq = call(Sequence_create, {delay1, hold, delay2, out, delay3, hide, 0});
    call(Node_runAction, {panel, seq});
    scene::sharedRelease(material);
}
RW_FUNCTION(0x765498, "CommonDirection::materialPanel_in", CommonDirection::materialPanel_in);

// ------------------------------------------------------------------------------------------------ skill panels
// Queue a skill panel (at most 10 waiting) and play the panel sound.
void CommonDirection::req_skilPanel_in(CommonDirection *self, int32_t skillId) {
    int32_t count = self->skillQueueCount;
    if (count > 9) return;
    self->skillQueue[count] = skillId;
    self->skillQueueCount = int32_t(uint32_t(self->skillQueueCount.get()) + 1);
    Frame f;
    uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    call(audio_Layer_playSE, {ld32(layer), SE_PANEL, 0, 0});
    scene::sharedRelease(layer);
}
RW_FUNCTION(0x765A18, "CommonDirection::req_skilPanel_in", CommonDirection::req_skilPanel_in);

void CommonDirection::req_skilPanel_out(CommonDirection *self, int32_t skillId) { skilPanel_out(self, skillId); }
RW_FUNCTION(0x765AB8, "CommonDirection::req_skilPanel_out", CommonDirection::req_skilPanel_out);

// Play out every occupied slot showing `skillId`.
void CommonDirection::skilPanel_out(CommonDirection *self, int32_t skillId) {
    for (int32_t k = 0; k < 6; k++) {
        if (self->panelState[k] != -1 && self->panelSkillId[k] == skillId) skilPanel_out_common(self, k);
    }
}
RW_FUNCTION(0x765AC0, "CommonDirection::skilPanel_out", CommonDirection::skilPanel_out);

// Play out every slot that is shown and not already going out.
void CommonDirection::req_skilPanel_all(CommonDirection *self) {
    for (int32_t k = 0; k < 6; k++) {
        int32_t state = self->panelState[k];
        if (state != -1 && state != 2) skilPanel_out_common(self, k);
    }
}
RW_FUNCTION(0x765AFC, "CommonDirection::req_skilPanel_all", CommonDirection::req_skilPanel_all);

void CommonDirection::skilPanel_out_all(CommonDirection *self) {
    for (int32_t k = 0; k < 6; k++) {
        int32_t state = self->panelState[k];
        if (state != -1 && state != 2) skilPanel_out_common(self, k);
    }
}
RW_FUNCTION(0x765B2C, "CommonDirection::skilPanel_out_all", CommonDirection::skilPanel_out_all);

// Show `skillId` in a free slot, on the row below the last occupied one.
void CommonDirection::skilPanel_in(CommonDirection *self, int32_t skillId) {
    call(StageState_cmpSubMission14, {ld32(call(AppState_getInstance, {}) + 32), uint32_t(skillId)});
    int32_t row = 0;
    for (int32_t k = 5; k >= 0; k--) {
        if (self->panelOrder[k] != -1) {
            row = k + 1;
            break;
        }
    }
    if (row > 5) return;
    int32_t slot = 0;
    while (self->panelState[slot] != -1) {
        if (++slot >= 6) return;
    }
    self->panelOrder[row] = slot;
    Frame f;
    uint32_t skill = scene::masterRow(f, master_Skill_create, uint32_t(skillId));
    if (ld32(skill)) {
        setVisible(self->skillPanel[slot].addr, 1);
        playIndex(self->skillPanel[slot].addr, 0);
        uint32_t panel = self->skillPanel[slot].addr;
        uint32_t setXY = ld32(ld32(panel) + V_SET_POSITION_XY);
        float top = rw::callf(AppGetGrameFrameHeight, {}) + -160.0f;
        float offset = float(int32_t(uint32_t(row) << 5));
        call(setXY, {panel, 0, fb(top - offset)});
        setVisible(self->skillPanelBack[slot].addr, 1);
        playIndex(self->skillPanelBack[slot].addr, 3);
        uint32_t back = self->skillPanelBack[slot].addr;
        setXY = ld32(ld32(back) + V_SET_POSITION_XY);
        float y = (rw::callf(AppGetGrameFrameHeight, {}) + -160.0f) - offset;
        call(setXY, {back, 0, fb(y)});
        self->panelSkillId[slot] = skillId;
        self->panelState[slot] = 1;
        uint32_t label;
        {
            Frame g;
            label = labelFromBuffer(self, rowName(g, ld32(skill)));
        }
        setPosition(label, 128.0f, 0.0f);
        scene::vcall(label, V_SET_COLOR, {label, ld32(GOT_Color3B_WHITE)});
        labelShadow(label);
        uint32_t bone = getBone(self->skillPanel[slot].addr, S_SKILL_PANEL_TEXT, 0xC);
        if (bone) call(Bone_addDisplay, {bone, label, 0});
        uint32_t done = callFunc(VT_IN_DONE, {addr(self), uint32_t(slot), uint32_t(skillId)});
        uint32_t delay = call(DelayTime_create, {fb(0.6666667f)});
        call(Node_stopAllActions, {self->skillPanel[slot].addr});
        runSequence2(self->skillPanel[slot].addr, delay, done);
    }
    scene::sharedRelease(skill);
}
RW_FUNCTION(0x765B5C, "CommonDirection::skilPanel_in", CommonDirection::skilPanel_in);

// Play the slot's out animation; the slot is freed 1/6 s later.
void CommonDirection::skilPanel_out_common(CommonDirection *self, int32_t slot) {
    playIndex(self->skillPanel[slot].addr, 2);
    self->panelState[slot] = 2;
    uint32_t done = callFunc(VT_OUT_DONE, {addr(self), uint32_t(slot)});
    uint32_t delay = call(DelayTime_create, {fb(0.16666667f)});
    call(Node_stopAllActions, {self->skillPanel[slot].addr});
    runSequence2(self->skillPanel[slot].addr, delay, done);
}
RW_FUNCTION(0x765F30, "CommonDirection::skilPanel_out_common", CommonDirection::skilPanel_out_common);

// Hold the slot for `seconds` (negative: play out now) unless the skill is of type 3 (held until replaced).
void CommonDirection::skilPanel_hold(CommonDirection *self, int32_t slot, float seconds) {
    int32_t skillId = self->panelSkillId[slot];
    Frame f;
    uint32_t skill = scene::masterRow(f, master_Skill_create, uint32_t(skillId));
    int32_t type = rowInt(ld32(skill), 0xF0);
    bool hold = seconds >= 0.0f || std::isnan(seconds);     // __aeabi_fcmpge | __unordsf2
    if (!hold && type != 3) {
        skilPanel_out_common(self, slot);
    } else {
        playIndex(self->skillPanel[slot].addr, 1);
        setVisible(self->skillPanelBack[slot].addr, 0);
        self->panelState[slot] = 4;
        if (type != 3) {
            uint32_t delay = call(DelayTime_create, {fb(seconds)});
            uint32_t done = callFunc(VT_HOLD_OUT, {addr(self), uint32_t(slot), uint32_t(skillId)});
            call(Node_stopAllActions, {self->skillPanel[slot].addr});
            runSequence2(self->skillPanel[slot].addr, delay, done);
        }
    }
    scene::sharedRelease(skill);
}
RW_FUNCTION(0x766048, "CommonDirection::skilPanel_hold", CommonDirection::skilPanel_hold);

// Start the next queued skill panel when a slot is free; when all six are busy and none is going out, push out
// the oldest held panel (not of type 3).
void CommonDirection::exec_que(CommonDirection *self) {
    if (self->skillQueueCount == 0) return;
    bool freeSlot = false;
    for (int32_t k = 0; k < 6; k++) {
        if (self->panelState[k] == -1) {
            freeSlot = true;
            break;
        }
    }
    if (!freeSlot) {
        for (int32_t k = 0; k < 6; k++) {
            if (self->panelState[k] == 2) return;
        }
        for (int32_t k = 0; k < 6; k++) {
            uint32_t slotBase = addr(self) + uint32_t(self->panelOrder[k].get()) * 4;   // may be row -1
            if (rw::load<int32_t>(slotBase + 0x300) != 4) continue;
            int32_t skillId = rw::load<int32_t>(slotBase + 0x2E8);
            Frame f;
            uint32_t skill = scene::masterRow(f, master_Skill_create, uint32_t(skillId));
            if (rowInt(ld32(skill), 0xF0) != 3) {
                skilPanel_out(self, skillId);
                scene::sharedRelease(skill);
                return;
            }
            scene::sharedRelease(skill);
        }
        return;
    }
    skilPanel_in(self, self->skillQueue[0]);
    int32_t count = self->skillQueueCount;
    if (count >= 1) {
        for (int32_t k = 1;; k++) {
            // (k may reach 10: queue[9] takes the word after the queue, skillQueueCount)
            self->skillQueue[k - 1] = rw::load<int32_t>(addr(self) + 0x348 + uint32_t(k) * 4);
            count = self->skillQueueCount;
            if (!(k < count)) break;
        }
    }
    self->skillQueueCount = int32_t(uint32_t(count) - 1);
}
RW_FUNCTION(0x7661E8, "CommonDirection::exec_que", CommonDirection::exec_que);

// Move held panels up a row when the row above became free, then advance the clock.
void CommonDirection::panel_update(CommonDirection *self, float dt) {
    exec_que(self);
    for (int32_t row = 1; row < 6; row++) {
        if (self->panelOrder[row - 1] != -1) continue;
        int32_t slot = self->panelOrder[row];
        if (slot == -1) continue;
        uint32_t stateAddr = addr(self) + uint32_t(slot) * 4 + 0x300;
        if (rw::load<int32_t>(stateAddr) != 4) continue;
        rw::store32(rw::host_ptr<uint32_t>(stateAddr), 3);
        self->panelOrder[row - 1] = self->panelOrder[row].get();
        self->panelOrder[row] = -1;
        uint32_t panel = self->skillPanel[slot].addr;
        uint32_t pos = scene::vcall(panel, V_GET_POSITION, {panel});
        Frame f;
        uint32_t to = scene::newVec2(f, ldf(pos), ldf(pos + 4) + 32.0f);
        float holdStart = self->panelHoldStart[slot];
        float now = self->clock;
        uint32_t move = call(MoveTo_create, {fb(0.16666667f), to});
        float remaining = 2.0f - (now - holdStart);
        // the lambda {this, slot, row, remaining} does not fit the inline buffer: heap __func
        uint32_t fn = f.alloc(24);
        uint32_t heap = guest::newObject(0x14);
        if (heap) {
            rw::save<uint32_t>(heap, VT_PANEL_HOLD);
            rw::save<uint32_t>(heap + 4, addr(self));
            rw::save<uint32_t>(heap + 8, uint32_t(slot));
            rw::save<uint32_t>(heap + 12, uint32_t(row));
            rw::save<float>(heap + 16, remaining);
        }
        scene::st32(fn + 16, heap);
        uint32_t done = call(CallFunc_create, {fn});
        scene::functionDestroy(fn);
        call(Node_stopAllActions, {self->skillPanel[slot].addr});
        runSequence2(self->skillPanel[slot].addr, move, done);
    }
    self->clock = self->clock + dt;
}
RW_FUNCTION(0x766320, "CommonDirection::panel_update", CommonDirection::panel_update);

// ------------------------------------------------------------------------------------------------ boss markers
void CommonDirection::hide_boss_marker(CommonDirection *, const uint32_t *marker) {
    if (*marker == 0) return;
    call(Node_stopAllActions, {*marker});
    uint32_t m = *marker;
    uint32_t fade = call(FadeOut_create, {fb(0.5f)});
    uint32_t done = callFunc(VT_HIDE_MARKER, {*marker});
    call(Node_runAction, {m, sequence2(fade, done)});
}
RW_FUNCTION(0x7664F4, "CommonDirection::hide_boss_marker", CommonDirection::hide_boss_marker);

void CommonDirection::show_boss_marker(CommonDirection *, const uint32_t *marker) {
    if (*marker == 0) return;
    call(Node_stopAllActions, {*marker});
    uint32_t m = *marker;
    uint32_t fade = call(FadeIn_create, {fb(0.5f)});
    uint32_t done = callFunc(VT_SHOW_MARKER, {*marker});
    call(Node_runAction, {m, sequence2(fade, done)});
}
RW_FUNCTION(0x7665C4, "CommonDirection::show_boss_marker", CommonDirection::show_boss_marker);

void CommonDirection::update(CommonDirection *self, float dt) { panel_update(self, dt); }
RW_FUNCTION(0x766694, "CommonDirection::update", CommonDirection::update);

// The full-screen fade layer (black, hidden) on top of the direction (z 10).
void CommonDirection::to_start(CommonDirection *self) {
    Frame f;
    uint32_t size = visibleSize(f);
    uint32_t layer = call(LayerColor_create, {ld32(GOT_Color4B_BLACK)});
    self->fadeLayer = rw::Ptr<uint8_t>{layer};
    scene::vcall(layer, V_SET_CONTENT_SIZE, {layer, scene::newVec2(f, ldf(size), ldf(size + 4))});
    setOpacity(self->fadeLayer.addr, 0);
    setVisible(self->fadeLayer.addr, 0);
    scene::vcall(addr(self), V_ADD_CHILD_Z, {addr(self), self->fadeLayer.addr, 10});
}
RW_FUNCTION(0x76669C, "CommonDirection::to_start", CommonDirection::to_start);

// Point the boss markers at the enemy (or the transport gate when there is none) and pick the distance level
// 0..4 (0: over 4000, 4: within 1000). The markers fade out while the target is on screen. Returns true when the
// level just changed to 4. `enabled` false hides the markers.
uint32_t CommonDirection::calc_warning(CommonDirection *self, uint32_t enemy, uint32_t avatar, uint32_t stageManager,
                                       uint32_t enabled) {
    uint32_t result = 0;
    if (enabled != 1) {
        if (self->warningOn) {
            hide_boss_marker(self, &self->bossMarkerArrow.addr);
            hide_boss_marker(self, &self->bossMarker.addr);
            self->markerHidden = 0;
        }
        self->warningOn = uint8_t(enabled);
        return 0;
    }
    Frame f;
    uint32_t target = f.alloc(8);       // the point the marker aims at
    uint32_t offset = scene::newVec2(f, 0.0f, 0.0f);
    uint32_t from = f.alloc(8);
    rw::frame_ptr<uint32_t>(from)[0] = ld32(avatar + 0x270);
    rw::frame_ptr<uint32_t>(from)[1] = ld32(avatar + 0x274);
    float extra = 0.0f;
    if (enemy) {
        rw::frame_ptr<uint32_t>(target)[0] = ld32(enemy + 0x270);
        rw::frame_ptr<uint32_t>(target)[1] = ld32(enemy + 0x274);
        uint32_t actor = ld32(enemy + 0x330);
        rw::frame_ptr<uint32_t>(offset)[0] = ld32(actor + 0x530);
        rw::frame_ptr<uint32_t>(offset)[1] = ld32(actor + 0x534);
    } else {
        uint32_t gate = call(StageManager_MarkingMapTransportGate, {stageManager, from});
        rw::frame_ptr<uint32_t>(target)[0] = ld32(gate);
        rw::frame_ptr<uint32_t>(target)[1] = ld32(gate + 4);
        extra = float(ld32(gate + 12));
    }
    float distance = extra + rw::callf(Vec2_distance, {from, target});
    if (self->bossMarker) {
        setVisible(self->bossMarkerArrow.addr, 1);
        setVisible(self->bossMarker.addr, 1);
        uint32_t origin = visibleOrigin(f);
        (void)origin;
        uint32_t size = visibleSize(f);
        float halfW = ldf(size) * 0.5f, halfH = ldf(size + 4) * 0.5f;
        float radiusX = halfW + -192.0f;
        float radiusY = halfH + -128.0f;
        uint32_t centre = ld32(addr(self) + 0x384) + 0x94;
        float cx = ldf(centre), cy = ldf(centre + 4);
        float tx = ldf(target) - ldf(offset);           // Vec2::subtract
        float ty = ldf(target + 4) - ldf(offset + 4);
        float dx = tx - cx;
        float dy = cy - ty;
        double a = double(dx * (radiusY / radiusX));
        double b = double(dy);
        uint64_t ab = 0, bb = 0;
        std::memcpy(&ab, &a, 8);
        std::memcpy(&bb, &b, 8);
        uint64_t r = rw::call64(libm_atan2, {uint32_t(ab), uint32_t(ab >> 32), uint32_t(bb), uint32_t(bb >> 32)});
        double angleD;
        std::memcpy(&angleD, &r, 8);
        float angle = float(angleD);
        float degrees = float(double(angle) * 180.0 / 3.14159265358979311600);
        uint32_t marker = self->bossMarker.addr;
        scene::vcall(marker, V_SET_ROTATION, {marker, fb(degrees)});
        float s = rw::callf(libm_sinf, {fb(angle)});
        float c = rw::callf(libm_cosf, {fb(angle)});
        float edgeX = s * radiusX;
        float edgeY = c * radiusY;
        float edge2 = edgeX * edgeX + edgeY * edgeY;
        float nearX = dx - s * 96.0f;
        float nearY = dy - c * 96.0f;
        float near2 = nearX * nearX + nearY * nearY;
        uint32_t delay = call(DelayTime_create, {fb(0.25f)});
        uint32_t showA = call(FadeTo_create, {fb(0.125f), 255});
        uint32_t showB = call(FadeTo_create, {fb(0.125f), 255});
        uint32_t hideA = call(FadeTo_create, {fb(0.125f), 0});
        uint32_t hideB = call(FadeTo_create, {fb(0.125f), 0});
        float px, py;
        if (near2 > edge2 || std::isnan(near2) || std::isnan(edge2)) {
            // the target is off screen: show the markers at the screen edge
            if (call(Node_getNumberOfRunningActions, {self->bossMarker.addr}) == 0 && self->markerHidden == 0) {
                call(Node_stopAllActions, {self->bossMarker.addr});
                setOpacity(self->bossMarker.addr, 0);
                runSequence2(self->bossMarker.addr, showA, delay);
                call(Node_stopAllActions, {self->bossMarkerArrow.addr});
                setOpacity(self->bossMarkerArrow.addr, 0);
                runSequence2(self->bossMarkerArrow.addr, showB, delay);
                self->markerHidden = 1;
            }
            px = edgeX + ldf(size) * 0.5f;
            py = edgeY + ldf(size + 4) * 0.5f;
        } else {
            if (call(Node_getNumberOfRunningActions, {self->bossMarker.addr}) == 0 && self->markerHidden != 0) {
                call(Node_stopAllActions, {self->bossMarker.addr});
                setOpacity(self->bossMarker.addr, 255);
                runSequence2(self->bossMarker.addr, hideA, delay);
                call(Node_stopAllActions, {self->bossMarkerArrow.addr});
                setOpacity(self->bossMarkerArrow.addr, 255);
                runSequence2(self->bossMarkerArrow.addr, hideB, delay);
                self->markerHidden = 0;
            }
            px = nearX + ldf(size) * 0.5f;
            py = nearY + ldf(size + 4) * 0.5f;
        }
        uint32_t pos = scene::newVec2(f, px, py);
        uint32_t arrow = self->bossMarkerArrow.addr;
        scene::vcall(arrow, V_SET_POSITION, {arrow, pos});
        marker = self->bossMarker.addr;
        scene::vcall(marker, V_SET_POSITION, {marker, pos});
        int32_t level;
        if (distance > 4000.0f) level = 0;
        else if (distance > 3000.0f) level = 1;
        else if (distance > 2000.0f) level = 2;
        else if (distance > 1000.0f) level = 3;
        else level = 4;
        if (level != self->warningLevel) {
            playIndex(self->bossMarkerArrow.addr, uint32_t(level + 12));
            self->warningLevel = level;
            result = level == 4 ? 1 : 0;
        }
        rw::set<float>(self->bossMarker.get(), 0x2C4, 1.0f);
        rw::set<float>(self->bossMarkerArrow.get(), 0x2C4, 1.0f);
    }
    self->warningOn = uint8_t(enabled);
    return result;
}
RW_FUNCTION(0x76675C, "CommonDirection::calc_warning", CommonDirection::calc_warning);

void CommonDirection::HideBossMarkerAll(CommonDirection *self) {
    if (self->bossMarkerArrow) {
        call(Node_stopAllActions, {self->bossMarkerArrow.addr});
        setOpacity(self->bossMarkerArrow.addr, 0);
    }
    if (self->bossMarker) {
        call(Node_stopAllActions, {self->bossMarker.addr});
        setOpacity(self->bossMarker.addr, 0);
    }
    self->markerHidden = 0;
}
RW_FUNCTION(0x766DAC, "CommonDirection::HideBossMarkerAll", CommonDirection::HideBossMarkerAll);

// ------------------------------------------------------------------------------------------------ fade layer
// A white flash (heal).
void CommonDirection::heal(CommonDirection *self) { flash(self, GOT_Color3B_WHITE, VT_HEAL); }
RW_FUNCTION(0x766DF8, "CommonDirection::heal", CommonDirection::heal);

// A black flash (battle start).
void CommonDirection::battle_start(CommonDirection *self) { flash(self, GOT_Color3B_BLACK, VT_BATTLE_START); }
RW_FUNCTION(0x766F18, "CommonDirection::battle_start", CommonDirection::battle_start);

// Black out the screen, then run `callFunc` after 0.25 s. Returns runAction's result.
uint32_t CommonDirection::black_out(CommonDirection *self, uint32_t callFunc) {
    uint32_t delay = call(DelayTime_create, {fb(0.25f)});
    setVisible(self->fadeLayer.addr, 1);
    uint32_t layer = self->fadeLayer.addr;
    scene::vcall(layer, V_SET_COLOR, {layer, ld32(GOT_Color3B_BLACK)});
    setOpacity(self->fadeLayer.addr, 255);
    return call(Node_runAction, {self->fadeLayer.addr, sequence2(delay, callFunc)});
}
RW_FUNCTION(0x767038, "CommonDirection::black_out", CommonDirection::black_out);
