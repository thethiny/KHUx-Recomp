// SceneLight: construction, init, loading, textures, the command loop and the small commands.
#include "SceneLight_lib.hpp"

using namespace slight;

// ------------------------------------------------------------------------------------------------ lifetime
SceneLight::Addr SceneLight::construct(Addr self) {
    call(smain::lib::Layer_ctor, {self});
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->nameLabel = 0;
    s->speech = 0;
    s->speechFix = 0;
    s->sequence = 0;
    s->sequenceCtl = 0;
    s->waitingPage = 0;
    s->textShown = 0;
    s->touchFlag = 0;
    s->spritesCap = 0;
    s->spritesEnd = 0;
    s->sprites = 0;
    call(wl::MEMSET, {self + 0x29F4, 0x14, 0});   // avatarIcon + the page map's first four words
    rw::set<float>(s, 0x2A08, 1.0f);               // the page map's max_load_factor
    rw::set<uint32_t>(s, 0x2A0C, 0);
    rw::set<uint32_t>(s, 0x2A10, 0);
    rw::set<uint32_t>(s, 0x2A14, 0);
    s->avatarSlot = -1;
    s->skipButton = 0;
    // TTFConfig ttfSpeech / ttfName: fontFilePath "", fontSize 12, glyphs, customGlyphs, distanceField,
    // outlineSize, the two bools after it
    strInit(self + OFF_TTF_SPEECH, S_EMPTY, 0);
    rw::set<uint32_t>(s, 0x2A2C, 12);
    rw::set<uint32_t>(s, 0x2A30, 0);
    rw::set<uint32_t>(s, 0x2A34, 0);
    rw::set<uint8_t>(s, 0x2A38, 0);
    rw::set<uint32_t>(s, 0x2A3C, 0);
    rw::set<uint8_t>(s, 0x2A41, 0);
    rw::set<uint8_t>(s, 0x2A40, 0);
    strInit(self + OFF_TTF_NAME, S_EMPTY, 0);
    rw::set<uint32_t>(s, 0x2A50, 12);
    rw::set<uint32_t>(s, 0x2A54, 0);
    rw::set<uint32_t>(s, 0x2A58, 0);
    rw::set<uint8_t>(s, 0x2A5C, 0);
    rw::set<uint32_t>(s, 0x2A60, 0);
    rw::set<uint8_t>(s, 0x2A64, 0);
    rw::set<uint8_t>(s, 0x2A65, 0);
    // std::map<int, Label*> labels: empty (begin = end node)
    rw::set<uint32_t>(s, 0x2A6C, 0);
    rw::set<uint32_t>(s, 0x2A70, 0);
    rw::set<uint32_t>(s, 0x2A68, self + 0x2A6C);
    return self;
}

SceneLight::Addr SceneLight::destroy(Addr self) {
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    call(smain::lib::Node_unscheduleUpdate, {self});
    if (s->avatarIcon) call(smain::lib::Ref_release, {s->avatarIcon});
    if (s->nameLabel) call(smain::lib::Ref_release, {s->nameLabel});
    if (s->speech) call(smain::lib::Ref_release, {s->speech});
    if (s->speechFix) call(smain::lib::Ref_release, {s->speechFix});
    call(LabelMap_destroy, {self + OFF_LABELS, rw::get<uint32_t>(s, 0x2A6C)});
    strFree(self + OFF_TTF_NAME);
    strFree(self + OFF_TTF_SPEECH);
    call(ExpressionVector_baseDtor, {self + OFF_EXPRESSIONS});
    call(Pages_dtor, {self + OFF_PAGES});
    release(s->sequenceCtl);
    call(SpriteVector_dtor, {self + 0x288});
    return call(smain::lib::Layer_dtor, {self});
}

SceneLight::Addr SceneLight::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    construct(self);
    if (vcall(self, VS_init, {self}) == 1) {
        call(smain::lib::Ref_autorelease, {self});
        return self;
    }
    vcall(self, 4, {self});
    return 0;
}

SceneLight::Addr SceneLight::createScene() {
    uint32_t scene = call(Scene_create, {});
    uint32_t layer = create();
    vcall(scene, VS_addChild, {scene, layer});
    return scene;
}

// ------------------------------------------------------------------------------------------------ init
uint32_t SceneLight::init(Addr self) {
    // init's frame (0x190 bytes) leaves three bytes of each TTFConfig's distanceFieldEnabled word uninitialised:
    // they are copied into the object as they are
    const uint32_t frame = rw::cpu->r[13] - 0x190;
    const uint32_t staleSpeech = rw::load<uint32_t>(frame + 0x104) & 0xFFFFFF00u;
    const uint32_t staleName = rw::load<uint32_t>(frame + 0xE0) & 0xFFFFFF00u;

    if (call(smain::lib::Layer_init, {self}) != 1) return 0;
    Self *s = S(self);
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_0, {self});
        call(smain::lib::FunctionVV_assign, {self + 0x210, fo});   // onEnter callback
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_1, {self});
        call(smain::lib::FunctionVV_assign, {self + 0x228, fo});   // onExit callback
        functionDestroy(fo);
    }
    call(SceneManager_setEnabledTouchEffect, {call(SceneManager_getInstance, {}), 0});
    {
        uint32_t sm = call(SceneManager_getInstance, {});
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(SceneManager_hideInfoHeader, {sm, empty});
        functionDestroy(empty);
    }
    call(SceneManager_removeBackground, {call(SceneManager_getInstance, {})});

    // the slot sprites: hidden children at z = slot + 2
    for (int32_t i = 0; i != SLOT_COUNT; i++) {
        st8(boundAddr(self, i), 0);
        st8(flipAddr(self, i), 0);
        uint32_t sp = call(Sprite_create, {});
        vcall(self, VS_addChildZ, {self, sp, uint32_t(i + 2)});
        setVisible(sp, 0);
        call(SpriteVector_pushBack, {self + 0x288, sp});
    }

    Frame f;
    // slot 5014: a black sprite over the visible area
    uint32_t black = call(Sprite_create, {});
    {
        uint32_t color = f.alloc(4);
        call(Color3B_ctor, {color, 0, 0, 0});
        vcall(black, VS_setColor, {black, color});
    }
    setOpacity(black, 0xFF);
    uint32_t origin = f.alloc(8);
    call(Director_getVisibleOrigin, {origin, call(Director_getInstance, {})});
    uint32_t vsize = f.alloc(8);
    call(Director_getVisibleSize, {vsize, call(Director_getInstance, {})});
    const float ox = ldf(origin), oy = ldf(origin + 4), vw = ldf(vsize), vh = ldf(vsize + 4);
    {
        float top = oy + vh;
        float right = ox + vw;
        uint32_t rect = f.alloc(16);
        call(smain::lib::Rect_ctor, {rect, fb(ox), fb(oy), fb(right), fb(top)});
        vcall(black, VS_setTextureRect, {black, rect});
        call(wl::Vec2_dtor, {rect});
    }
    {
        float cx = ox + vw * 0.5f;
        float cy = oy + vh * 0.5f;
        withVec2(cx, cy, [&](uint32_t v) { return vcall(black, VS_setPositionV, {black, v}); });
    }
    vcall(self, VS_addChild, {self, black});
    setVisible(black, 0);
    call(SpriteVector_pushBack, {self + 0x288, black});

    // the letterbox bars above and below the 640-high play area
    uint32_t bars = f.alloc(8);
    call(Size_copy, {bars, vsize});
    const float barH = ldf(bars + 4) * 0.5f + -320.0f;
    {
        uint32_t top = call(Sprite_create, {});
        uint32_t rect = f.alloc(16);
        call(smain::lib::Rect_ctor, {rect, 0, 0, fb(vw), fb(barH)});
        vcall(top, VS_setTextureRect, {top, rect});
        call(wl::Vec2_dtor, {rect});
        withVec2(0.0f, 720.0f, [&](uint32_t v) { return vcall(top, VS_setPositionV, {top, v}); });
        vcall(top, VS_setColor, {top, Color3B_BLACK});
        vcall(top, VS_setAnchorPoint, {top, ANCHOR_TOP_LEFT});
        vcall(self, VS_addChildZ, {self, top, 0xFA});
    }
    {
        uint32_t bottom = call(Sprite_create, {});
        float h = ldf(bars + 4) * 0.5f + -320.0f;
        uint32_t rect = f.alloc(16);
        call(smain::lib::Rect_ctor, {rect, 0, 0, fb(ldf(vsize)), fb(h)});
        vcall(bottom, VS_setTextureRect, {bottom, rect});
        call(wl::Vec2_dtor, {rect});
        vcall(bottom, VS_setColor, {bottom, Color3B_BLACK});
        vcall(bottom, VS_setAnchorPoint, {bottom, ANCHOR_TOP_LEFT});
        float y = ldf(bars + 4) * 0.5f + -320.0f;
        withVec2(0.0f, y, [&](uint32_t v) { return vcall(bottom, VS_setPositionV, {bottom, v}); });
        vcall(self, VS_addChildZ, {self, bottom, 0xFA});
    }
    s->command = 0;
    s->wait = -1.0f;
    bindTexture(self, SLOT_WINDOW + 1, 99998, 0);
    bindTexture(self, SLOT_CURSOR, 99997, 0);

    // the two fonts (TTFConfig temporaries moved into the object)
    struct Ttf { uint32_t at, path, len, stale; };
    const Ttf fonts[2] = {{OFF_TTF_SPEECH, S_FONT_SPEECH, 45, staleSpeech}, {OFF_TTF_NAME, S_FONT_NAME, 34, staleName}};
    for (const Ttf &t : fonts) {
        Frame g;
        uint32_t path = newStr(g, t.path, t.len);
        uint32_t cfg = self + t.at;
        strMoveAssign(cfg, path);
        st32(cfg + 0x18, t.stale);      // distanceFieldEnabled = false (+ the stale bytes)
        st32(cfg + 0x14, 0);            // customGlyphs
        st32(cfg + 0x10, 0);            // glyphs
        st32(cfg + 0x0C, 30);           // fontSize
        st32(cfg + 0x1C, 0);            // outlineSize
        rw::save<uint16_t>(cfg + 0x20, 0);
    }
    rw::set<uint32_t>(s, 0x2A3C, 0);
    rw::set<uint32_t>(s, 0x2A60, 0);

    s->nameLabel = call(Label_create, {});
    call(smain::lib::Ref_retain, {s->nameLabel});
    vcall(s->nameLabel, VS_setTTFConfig, {s->nameLabel, self + OFF_TTF_SPEECH});
    s->speech = call(SpeechDisp_create, {});
    call(smain::lib::Ref_retain, {s->speech});
    vcall(s->speech, VS_setTTFConfig, {s->speech, self + OFF_TTF_SPEECH});
    s->speechFix = call(SpeechDisp_create, {});
    call(smain::lib::Ref_retain, {s->speechFix});
    vcall(s->speechFix, VS_setTTFConfig, {s->speechFix, self + OFF_TTF_NAME});
    s->textShown = 0;
    call(Pages_clear, {self + OFF_PAGES});

    // the skip button (FlashSkipAnimation armature) at the top left of the play area
    {
        uint32_t adm = call(ArmatureDataManager_getInstance, {});
        Frame g;
        uint32_t json = newStr(g, S_SKIP_JSON, 48);
        call(ArmatureDataManager_addArmatureFileInfo, {adm, json});
        strFree(json);
        uint32_t name = newStr(g, S_SKIP, 18);
        s->skipButton = call(Armature_create, {name});
        strFree(name);
    }
    {
        uint32_t arm = s->skipButton;
        float y = ldf(bars + 4) * 0.5f + -320.0f + 640.0f + -32.0f;
        withVec2(96.0f, y, [&](uint32_t v) { return vcall(arm, VS_setPositionV, {arm, v}); });
        uint32_t anim = vcall(s->skipButton, VS_getAnimation, {s->skipButton});
        vcall(anim, VS_Animation_playWithIndex, {anim, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
        vcall(self, VS_addChildZTag, {self, s->skipButton, 200, 0x2564813});
    }

    // touches (not swallowed): began $_2, moved $_3, ended $_4, cancelled $_5
    uint32_t dispatcher = ld32(call(Director_getInstance, {}) + smain::lib::Director_eventDispatcher);
    uint32_t listener = call(smain::lib::EventListenerTouchOneByOne_create, {});
    call(smain::lib::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    {
        uint32_t fo = f.alloc(24);
        struct Cb { uint32_t vt, slot, swap; bool capture; };
        const Cb cbs[4] = {{VT_init_2, 0x50, wl::FunctionTouchB_swap, true},
                           {VT_init_3, 0x68, wl::FunctionTouchV_swap, false},
                           {VT_init_4, 0x80, wl::FunctionTouchV_swap, true},
                           {VT_init_5, 0x98, wl::FunctionTouchV_swap, true}};
        for (const Cb &c : cbs) {
            st32(fo + 16, fo);
            st32(fo, c.vt);
            if (c.capture) st32(fo + 4, self);
            call(c.swap, {fo, listener + c.slot});
            functionDestroy(fo);
        }
    }
    call(smain::lib::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self});
    call(wl::Vec2_dtor, {origin});
    return 1;
}

// ------------------------------------------------------------------------------------------------ loading
// the command sequence img/light/SEQ/<id>.l (or 1010201 when missing) into `sequence`
void SceneLight::load(Addr self, int32_t id) {
    Frame f;
    uint32_t buf = f.alloc(0x404);
    st32(buf + 0x400, 0);
    st8(buf, 0);
    call(StringBuffer_fset, {buf, S_SEQ, uint32_t(id)});
    uint32_t data = f.alloc(8);
    call(bg_vfs_readData, {data, buf});
    if (!ld32(data)) {
        call(StringBuffer_fset, {buf, S_SEQ, uint32_t(SEQ_DEFAULT)});
        uint32_t tmp = f.alloc(8);
        call(bg_vfs_readData, {tmp, buf});
        uint32_t obj = ld32(tmp), ctl = ld32(tmp + 4);
        st32(tmp, 0);
        st32(tmp + 4, 0);
        st32(data, obj);
        uint32_t old = ld32(data + 4);
        st32(data + 4, ctl);
        if (old) {
            release(old);
            release(ld32(tmp + 4));
        }
    }
    uint32_t ctl = ld32(data + 4);
    uint32_t obj = ld32(data);
    addRef(ctl);
    Self *s = S(self);
    s->sequence = obj;
    uint32_t old = s->sequenceCtl;
    s->sequenceCtl = ctl;
    release(old);
    release(ld32(data + 4));
}

uint32_t SceneLight::making(Addr self, int32_t a, int32_t sceneId, uint32_t flag) {
    Self *s = S(self);
    s->sceneId = sceneId;
    s->sceneFlag = uint8_t(flag);
    load(self, a);
    s->command = ld32(s->sequence + 4);
    s->wait = 0.0f;
    execCommandCheckAvatar(self);
    return 1;
}

// TextureCache::addImage(format(path, arg)) (`this` is not used)
static uint32_t addImage(uint32_t fmt, uint32_t arg) {
    uint32_t cache = call(Director_getTextureCache, {call(Director_getInstance, {})});
    Frame f;
    uint32_t buf = stringBuffer(f, {fmt, arg});
    uint32_t path = newStr(f, buf);
    uint32_t tex = call(TextureCache_addImage, {cache, path});
    strFree(path);
    return tex;
}
uint32_t SceneLight::loadTexture(Addr, Addr name) { return addImage(S_IMG, name); }
uint32_t SceneLight::loadTextureBG(Addr, int32_t id) { return addImage(S_IMG_BG, uint32_t(id)); }
uint32_t SceneLight::loadTextureNPC(Addr, int32_t id) { return addImage(S_IMG_NPC, uint32_t(id)); }
uint32_t SceneLight::loadTextureNARRATION(Addr, int32_t id) { return addImage(S_IMG_NARRATION, uint32_t(id)); }

// Bind texture `texture` to slot `slot`'s sprite (texture size rect). Slots 5003/5004: backgrounds, 5005..5007:
// narration window parts, 5008..5013: characters (40000+: the player's avatar icon, expression by id).
// Returns the sprite, 0 for 5001 / 5002 and unknown slots.
uint32_t SceneLight::bindTexture(Addr self, int32_t slot, int32_t texture, uint32_t fade) {
    if (uint32_t(slot - SLOT_BGM) < 2) return 0;
    uint32_t kind = uint32_t(slot - SLOT_BG);
    uint32_t spAddr = ld32(self + 0x288) + uint32_t(slot) * 4;
    if (kind > 10) return 0;
    uint32_t sp = ld32(spAddr);
    uint32_t bit = 1u << kind;
    Self *s = S(self);
    uint32_t tex;
    if (bit & 0x7E0) {
        if (texture >= AVATAR_TEXTURE + 10) {
            // the player's avatar icon on this character slot
            if (s->avatarIcon) vcall(s->avatarIcon, VS_removeFromParentAndCleanup, {s->avatarIcon, 1});
            uint32_t idx = uint32_t(texture - 40011);
            uint32_t expression = idx <= 4 ? ld32(T_EXPRESSION + idx * 4) : 1;
            uint32_t duration = fade ? fb(0.25f) : 0;
            call(AvatarIcon_changeExpression, {s->avatarIcon, expression, duration});
            vcall(sp, VS_addChild, {sp, s->avatarIcon});
            Frame f;
            uint32_t rect = f.alloc(16);
            call(smain::lib::Rect_ctor, {rect, 0, 0, 0, 0});
            vcall(sp, VS_setTextureRect, {sp, rect});
            call(wl::Vec2_dtor, {rect});
            s->avatarSlot = slot;
            return sp;
        }
        // an NPC picture: the avatar icon leaves this slot (loadTextureNPC gets the slot / -1 in r0)
        uint32_t r0 = uint32_t(s->avatarSlot);
        if (s->avatarSlot == slot) {
            vcall(s->avatarIcon, VS_removeFromParentAndCleanup, {s->avatarIcon, 1});
            s->avatarSlot = -1;
            r0 = 0xFFFFFFFFu;
        }
        tex = call(0x6C1535, {r0, uint32_t(texture)});          // SceneLight::loadTextureNPC
    } else if (bit & 0x1C) {
        tex = call(0x6C1601, {0, uint32_t(texture)});           // SceneLight::loadTextureNARRATION
    } else if (bit & 3) {
        tex = call(0x6C1469, {0, uint32_t(texture)});           // SceneLight::loadTextureBG
    } else {
        return 0;
    }
    if (!tex) return sp;
    Frame f;
    uint32_t size = f.alloc(8);
    call(Texture2D_getContentSize, {size, tex});
    call(Sprite_setFlippedY, {sp, 0});
    vcall(sp, VS_setTexture, {sp, tex});
    uint32_t rect = f.alloc(16);
    call(smain::lib::Rect_ctor, {rect, 0, 0, ld32(size), ld32(size + 4)});
    vcall(sp, VS_setTextureRect, {sp, rect});
    call(wl::Vec2_dtor, {rect});
    return sp;
}

// hide the name and both speech labels, drop their queued pages
void SceneLight::removeText(Addr self) {
    Self *s = S(self);
    s->waitingPage = 0;
    s->textShown = 0;
    call(Pages_erase, {self + OFF_PAGES, self + OFF_SPEECH});
    call(Pages_erase, {self + OFF_PAGES, self + OFF_SPEECH_FIX});
    if (s->nameLabel) vcall(s->nameLabel, VS_removeFromParentAndCleanup, {s->nameLabel, 0});
    if (s->speech) vcall(s->speech, VS_removeFromParentAndCleanup, {s->speech, 0});
    if (s->speechFix) vcall(s->speechFix, VS_removeFromParentAndCleanup, {s->speechFix, 0});
}

// vector<Expression>::push_back as inlined (the end pointer is re-read after a construct at a null end)
static void pushExpression(uint32_t vec, uint32_t valueAddr, uint32_t slow) {
    uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t e = 0;
        if (end) {
            st32(end, ld32(valueAddr));
            e = ld32(vec + 4);
        }
        st32(vec + 4, e + 4);
    } else {
        call(slow, {vec, valueAddr});
    }
}

// The avatar icon: the expressions the sequence uses (up to the first 4001), then the icon is built
void SceneLight::execCommandCheckAvatar(Addr self) {
    Self *s = S(self);
    const uint32_t vec = self + OFF_EXPRESSIONS;
    // clear()
    uint32_t begin = ld32(vec), end = ld32(vec + 4);
    if (end != begin) st32(vec + 4, end + (~3u & ~((end - 4) - begin)));
    Frame f;
    uint32_t value = f.alloc(4);
    st32(value, 1);
    pushExpression(vec, value, ExpressionVector_pushSlowMove);
    uint32_t cmd = s->command;
    if (cmd) {
        int32_t op;
        do {
            const uint32_t c = cmd;
            cmd += 0x14;
            int32_t texture = int32_t(ld32(c + 0xC));
            op = int32_t(ld32(c));
            if (op == OP_BIND && texture >= 40011) {
                uint32_t idx = uint32_t(texture - 40011);
                uint32_t expression = idx <= 4 ? ld32(T_EXPRESSION + idx * 4) : 1;
                st32(value, expression);
                uint32_t b = ld32(vec), e = ld32(vec + 4);
                bool found = false;
                if (e != b) {
                    uint32_t n = uint32_t(int32_t(e - b) >> 2);
                    for (uint32_t i = 0; i < n; i++) found |= ld32(b + i * 4) == expression;
                }
                if (!found) pushExpression(vec, value, ExpressionVector_pushSlowCopy);
            }
        } while (op != OP_FINISH);
    }
    uint32_t style = f.alloc(0x28);
    uint32_t r0 = call(AvatarUtil_getCurrentStyle, {style, 1});
    uint32_t tex = call(0x6C1535, {r0, 40006});        // SceneLight::loadTextureNPC (r0: getCurrentStyle's result)
    uint32_t size = f.alloc(8);
    call(Texture2D_getContentSize, {size, tex});
    float x = ldf(size) * 0.5f + -4.0f;
    float y = ldf(size + 4) * 0.5f + -110.0f;
    s->avatarIcon = withVec2(-x, y, [&](uint32_t pos) {
        return call(AvatarIcon_create, {style, tex, pos, fb(3.9f), fb(3.9f), vec});
    });
    call(smain::lib::Ref_retain, {s->avatarIcon});
    uint32_t icon = s->avatarIcon;
    withVec2(20.0f, 236.0f, [&](uint32_t v) { return vcall(icon, VS_setPositionV, {icon, v}); });
    call(SafeBuffer_dtor, {style});
}

// ------------------------------------------------------------------------------------------------ the loop
void SceneLight::update(Addr self, float dt) {
    Self *s = S(self);
    float t = s->wait;
    bool waiting;
    if (t <= 0.0f || t != t) {
        waiting = t < 0.0f;
    } else {
        s->wait = t - dt;
        waiting = float(s->wait) > 0.0f;
    }
    if (waiting) return;
    if (s->waitingPage) {
        s->waitingPage = 0;
        s->wait = -1.0f;
        if (s->windowType) {
            uint32_t deque = call(Pages_index, {self + OFF_PAGES, self + OFF_SPEECH});
            if (ld32(deque + 0x14)) {
                runSpeech(self, s->speech);
                return;
            }
        } else {
            uint32_t deque = call(Pages_index, {self + OFF_PAGES, self + OFF_SPEECH_FIX});
            if (ld32(deque + 0x14)) {
                runSpeechFix(self, s->speechFix);
                return;
            }
        }
    } else if (s->currentOp == OP_TEXT) {
        uint32_t key = s->windowType ? self + OFF_SPEECH : self + OFF_SPEECH_FIX;
        uint32_t deque = call(Pages_index, {self + OFF_PAGES, key});
        if (ld32(deque + 0x14)) {
            call(Deque_popFront, {deque});
            if (ld32(deque + 0x14)) {
                s->waitingPage = 1;
                priExecCommand(self, 0.0f);
                s->wait = -1.0f;
            } else {
                call(Pages_erase, {self + OFF_PAGES, key});
                s->wait = 0.0f;
            }
            return;
        }
    }
    do {
        execCommand(self, 0.0f);
    } while (float(s->wait) == 0.0f);
}

// show the front page of `disp`'s queue; the next page follows when the speech ends ($_10)
void SceneLight::runSpeech(Addr self, Addr disp) {
    Frame f;
    uint32_t key = f.alloc(4);
    st32(key, disp);
    uint32_t deque = call(Pages_index, {self + OFF_PAGES, key});
    Self *s = S(self);
    if (!ld32(deque + 0x14)) {
        s->wait = 0.0f;
        return;
    }
    uint32_t start = ld32(deque + 0x10);
    uint32_t page = ld32(ld32(deque + 4) + (start / 341) * 4) + (start % 341) * 12;
    vcall(disp, VS_setString, {disp, page});
    if (!s->textShown) {
        s->textShown = 1;
        call(SpeechDisp_runSpeechSet, {disp, 0});
        call(SpeechDisp_toEndSpeech, {disp});
    }
    uint32_t fo = newFunction(f, VT_speech_10, {self, disp});
    uint32_t cf = call(wl::CallFunc_create, {fo});
    call(SpeechDisp_runSpeechSet, {disp, cf});
    functionDestroy(fo);
}

// show the front page at once and drop it; the following page waits for the tap
void SceneLight::runSpeechFix(Addr self, Addr disp) {
    Frame f;
    uint32_t key = f.alloc(4);
    st32(key, disp);
    uint32_t deque = call(Pages_index, {self + OFF_PAGES, key});
    Self *s = S(self);
    if (!ld32(deque + 0x14)) {
        s->wait = 0.0f;
        return;
    }
    uint32_t start = ld32(deque + 0x10);
    uint32_t page = ld32(ld32(deque + 4) + (start / 341) * 4) + (start % 341) * 12;
    vcall(disp, VS_setString, {disp, page});
    if (!s->textShown) s->textShown = 1;
    deque = call(Pages_index, {self + OFF_PAGES, key});
    call(Deque_popFront, {deque});
    if (ld32(deque + 0x14)) {
        s->waitingPage = 1;
        priExecCommand(self, 0.0f);
        s->wait = -1.0f;
    } else {
        call(Pages_erase, {self + OFF_PAGES, key});
        s->wait = 0.0f;
    }
}

// the "next" cursor of the coming command: its own (4006) or the default one
uint32_t SceneLight::priExecCommand(Addr self, float) {
    uint32_t cmd = S(self)->command;
    if (!cmd) return self;
    if (int32_t(ld32(cmd)) == OP_BUTTON)
        return call(0x6C2A39, {self, ld32(cmd + 4), cmd, ld32(cmd + 0x10)});   // SceneLight::cmd_button
    return call(0x6C2A39, {self, uint32_t(SLOT_CURSOR), cmd, 0});
}

// run the next command: {op, a, waitMs, c, d}
void SceneLight::execCommand(Addr self, float) {
    Self *s = S(self);
    uint32_t cmd = s->command;
    if (!cmd) {
        s->wait = -1.0f;
        return;
    }
    s->command = cmd + 4;
    int32_t op = int32_t(ld32(cmd));
    s->currentOp = op;
    s->command = cmd + 8;
    uint32_t a = ld32(cmd + 4);
    s->command = cmd + 0xC;
    s->wait = msToSec(int32_t(ld32(cmd + 8)));
    s->command = cmd + 0x10;
    uint32_t c = ld32(cmd + 0xC);
    s->command = cmd + 0x14;
    uint32_t idx = uint32_t(op - OP_INIT);
    if (idx > 6) return;
    uint32_t d = ld32(cmd + 0x10);
    static const uint32_t handlers[7] = {0x6C2BD1, 0x6C4DED, 0x6C2C5D, 0x6C2F41, 0x6C2F91, 0x6C3765, 0x6C2A39};
    call(handlers[idx], {self, a, c, d});   // cmd_init, next_scene, cmd_bind, cmd_prt, cmd_text, cmd_move, cmd_button
}

// ------------------------------------------------------------------------------------------------ commands
// show the "next" cursor (slot) at the window's corner, jumping in place every `ms`
void SceneLight::cmdButton(Addr self, int32_t slot, uint32_t, int32_t ms) {
    uint32_t window = sprite(self, SLOT_WINDOW);
    uint32_t cursor = sprite(self, slot);
    Frame f;
    Size2 vs = visibleSize(f);
    Size2 cursorSize = contentSize(f, cursor);
    Size2 windowSize = contentSize(f, window);
    vcall(cursor, VS_setPositionX, {cursor, fb(vs.w * 0.5f + 362.0f)});
    float y = vs.h * 0.5f;
    if (S(self)->windowType) y = (y + -320.0f) + cursorSize.h * 0.5f + 18.0f;
    else y = y + windowSize.h * -0.5f;
    vcall(cursor, VS_setPositionY, {cursor, fb(y)});
    uint32_t jump = withVec2(0.0f, 0.0f, [&](uint32_t v) {
        return call(JumpBy_create, {fb(msToSec(ms)), v, fb(8.0f), 1});
    });
    setVisible(cursor, 1);
    call(wl::Node_runAction, {cursor, call(RepeatForever_create, {jump})});
}

// reset: no slot flipped; window, background layers and the black sprite back to their z orders
void SceneLight::cmdInit(Addr self) {
    call(wl::MEMSET, {self + OFF_FLIPPED, uint32_t(SLOT_COUNT), 0});
    uint32_t black = sprite(self, SLOT_BLACK);
    uint32_t picture = sprite(self, SLOT_PICTURE);
    uint32_t bg = sprite(self, SLOT_BG);
    vcall(self, VS_reorderChild, {self, sprite(self, SLOT_WINDOW), 0x66});
    vcall(self, VS_reorderChild, {self, bg, 1});
    vcall(self, VS_reorderChild, {self, picture, 100});
    vcall(self, VS_reorderChild, {self, black, 101});
}

// bind a texture to a slot (flip 1 / 2 sets / clears the slot's flip). A character slot already shown fades its
// copy (slot + 3) in over it ($_7 then rebinds the slot); slot 5002 plays a sound, 5001 changes the BGM.
void SceneLight::cmdBind(Addr self, int32_t slot, int32_t texture, int32_t flip) {
    uint32_t sp = sprite(self, slot);
    if (flip == 2) st8(flipAddr(self, slot), 0);
    else if (flip == 1) st8(flipAddr(self, slot), 1);
    Self *s = S(self);
    if (slot >= SLOT_CHARA && ld8(boundAddr(self, slot))) {
        Frame f;
        uint32_t pos = f.alloc(8);
        call(Vec2_copy, {pos, vcall(sp, VS_getPositionV, {sp})});
        int32_t copy = slot + 3;
        st8(flipAddr(self, copy), ld8(flipAddr(self, slot)));
        uint32_t c = bindTexture(self, copy, texture, ld8(boundAddr(self, slot)));
        vcall(c, VS_setPositionV, {c, pos});
        setVisible(c, 1);
        call(Sprite_setFlippedX, {c, ld8(flipAddr(self, copy))});
        if (s->avatarSlot == copy) {
            float sx = ld8(flipAddr(self, copy)) ? -1.0f : 1.0f;
            vcall(s->avatarIcon, VS_scaleX, {s->avatarIcon, fb(sx)});
            setOpacity(c, 0xFF);
        } else {
            setOpacity(c, 0);
        }
        s->wait = -1.0f;
        uint32_t fade = call(FadeTo_create, {fb(0.25f), 0xFF});
        uint32_t fo = f.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_bind_7);
            st32(heap + 4, self);
            st32(heap + 8, uint32_t(slot));
            st32(heap + 0xC, uint32_t(texture));
            st32(heap + 0x10, uint32_t(flip));
        }
        st32(fo + 16, heap);
        uint32_t cf = call(wl::CallFunc_create, {fo});
        call(wl::Node_runAction, {c, call(wl::Sequence_create, {fade, cf, 0})});
        functionDestroy(fo);
        call(wl::Vec2_dtor, {pos});
    } else {
        uint32_t c = bindTexture(self, slot, texture, 0);
        if (c) {
            call(Sprite_setFlippedX, {c, ld8(flipAddr(self, slot))});
            if (s->avatarSlot == slot) {
                float sx = ld8(flipAddr(self, slot)) ? -1.0f : 1.0f;
                vcall(s->avatarIcon, VS_scaleX, {s->avatarIcon, fb(sx)});
            }
        } else if (slot == SLOT_SE) {
            playSE(uint32_t(texture));
        } else if (slot == SLOT_BGM) {
            call(AudioReplaceGameMainBGM, {uint32_t(texture), 500});
        }
    }
    st8(boundAddr(self, slot), 1);
}

// z order of a slot (a character's copy above it)
void SceneLight::cmdPrt(Addr self, int32_t slot, int32_t z, int32_t) {
    vcall(self, VS_reorderChild, {self, sprite(self, slot), uint32_t(z)});
    if (slot >= SLOT_CHARA)
        vcall(self, VS_reorderChild, {self, ld32(ld32(self + 0x288) + uint32_t(slot) * 4 + 0xC), uint32_t(z + 1)});
}

// to the next scene layer ($_6 makes it: scene id + 1)
void SceneLight::nextScene(Addr self) {
    Self *s = S(self);
    s->command = 0;
    s->wait = -1.0f;
    call(smain::lib::Node_unscheduleUpdate, {self});
    int32_t next = s->sceneId + 1;
    s->sceneId = next;
    uint32_t flag = s->sceneFlag;
    uint32_t sm = call(SceneManager_getInstance, {});
    Frame f;
    uint32_t fo = newFunction(f, VT_nextScene_6, {self, uint32_t(next), flag});
    call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
}

// $_7's second half: the slot itself gets the texture, its copy is hidden
void SceneLight::cmdRebind(Addr self, int32_t slot, int32_t texture, int32_t flip) {
    uint32_t sp = bindTexture(self, slot, texture, 0);
    uint32_t f;
    if (flip == 1) {
        f = 1;
        st8(flipAddr(self, slot), 1);
    } else if (flip == 2) {
        f = 0;
        st8(flipAddr(self, slot), 0);
    } else {
        f = ld8(flipAddr(self, slot)) ? 1 : 0;
    }
    call(Sprite_setFlippedX, {sp, f});
    Self *s = S(self);
    if (s->avatarSlot == slot) {
        float sx = ld8(flipAddr(self, slot)) ? -1.0f : 1.0f;
        vcall(s->avatarIcon, VS_scaleX, {s->avatarIcon, fb(sx)});
    }
    setVisible(ld32(ld32(self + 0x288) + uint32_t(slot) * 4 + 0xC), 0);
    setVisible(sp, 1);
    s->wait = 0.0f;
}

// place the window tail (slot 5006) for the window type; it ends up hidden either way
void SceneLight::setHige(Addr self) {
    Frame f;
    Size2 vs = visibleSize(f);
    Size2 window = contentSize(f, sprite(self, SLOT_WINDOW));
    Self *s = S(self);
    int32_t type = s->windowType;
    uint32_t hige = sprite(self, SLOT_HIGE);
    if (type) {
        float x = vs.w * 0.5f + float(rw::load<int16_t>(T_HIGE_X + uint32_t(type) * 2));
        float y = window.h + 45.0f;
        vcall(hige, VS_setPositionXY, {hige, fb(x), fb(y)});
        vcall(hige, VS_setScaleXY, {hige, 0, fb(1.0f)});
        uint32_t flipped = s->windowType >= 4 ? 1 : 0;
        st8(flipAddr(self, SLOT_HIGE), uint8_t(flipped));
        call(Sprite_setFlippedX, {hige, flipped});
    } else {
        setVisible(hige, 0);
    }
    setVisible(hige, 0);
}

RW_FUNCTION(0x6C6360, "SceneLight::SceneLight", SceneLight::construct);
RW_FUNCTION(0x6C1244, "SceneLight::~SceneLight", SceneLight::destroy);
RW_FUNCTION(0x6C11EC, "SceneLight::create", SceneLight::create);
RW_FUNCTION(0x6C11C8, "SceneLight::createScene", SceneLight::createScene);
RW_FUNCTION(0x6C1964, "SceneLight::init", SceneLight::init);
RW_FUNCTION(0x6C10B8, "SceneLight::load", SceneLight::load);
RW_FUNCTION(0x6C22C8, "SceneLight::making", SceneLight::making);
RW_FUNCTION(0x6C139C, "SceneLight::loadTexture", SceneLight::loadTexture);
RW_FUNCTION(0x6C1468, "SceneLight::loadTextureBG", SceneLight::loadTextureBG);
RW_FUNCTION(0x6C1534, "SceneLight::loadTextureNPC", SceneLight::loadTextureNPC);
RW_FUNCTION(0x6C1600, "SceneLight::loadTextureNARRATION", SceneLight::loadTextureNARRATION);
RW_FUNCTION(0x6C16CC, "SceneLight::bind_texture_common", SceneLight::bindTexture);
RW_FUNCTION(0x6C18E0, "SceneLight::removeText", SceneLight::removeText);
RW_FUNCTION(0x6C2308, "SceneLight::exec_commandCheckAvater", SceneLight::execCommandCheckAvatar);
RW_FUNCTION(0x6C2590, "SceneLight::update", SceneLight::update);
RW_FUNCTION(0x6C26D8, "SceneLight::runSpeech", SceneLight::runSpeech);
RW_FUNCTION(0x6C2818, "SceneLight::runSpeechFix", SceneLight::runSpeechFix);
RW_FUNCTION(0x6C2920, "SceneLight::pri_exec_command", SceneLight::priExecCommand);
RW_FUNCTION(0x6C294C, "SceneLight::exec_command", SceneLight::execCommand);
RW_FUNCTION(0x6C2A38, "SceneLight::cmd_button", SceneLight::cmdButton);
RW_FUNCTION(0x6C2BD0, "SceneLight::cmd_init", SceneLight::cmdInit);
RW_FUNCTION(0x6C2C5C, "SceneLight::cmd_bind", SceneLight::cmdBind);
RW_FUNCTION(0x6C2F40, "SceneLight::cmd_prt", SceneLight::cmdPrt);
RW_FUNCTION(0x6C4DEC, "SceneLight::next_scene", SceneLight::nextScene);
RW_FUNCTION(0x6C4ED0, "SceneLight::cmd_rebind", SceneLight::cmdRebind);
RW_FUNCTION(0x6C4F78, "SceneLight::set_hige", SceneLight::setHige);
