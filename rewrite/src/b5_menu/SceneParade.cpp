// SceneParade: the Union ranking parade (a SceneFlashDrama): the top five rankers' avatars marching in the
// parade_ranking LWF over the Parade_Top layout (name and Lux per place, the player's Union emblem, the
// congratulations LWF), and the union ranking LWF texture callbacks.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_dtor = 0x88A88D;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;
constexpr uint32_t SafeBuffer_ctor = 0x459F21, SafeBuffer_dtor = 0x45A13D, SafeBuffer_assign = 0x45A2C9;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t SceneFlashDrama_init = 0x6F34BD;      // (self, bool)
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;         // std::function<void()>::operator=(function&&)
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t VectorRanker_copy = 0x735FB1;          // vector<UnionRanking::Ranker>(const vector&)
constexpr uint32_t VectorRanker_dtor = 0x4AE8D9;
constexpr uint32_t Ranker_getTemporaryBuffer = 0x7367FD;  // std::get_temporary_buffer<Ranker>(pair out, n)
constexpr uint32_t Ranker_stableSort = 0x736859;         // __stable_sort(first, last, comp, n, buf, size)
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;
constexpr uint32_t VectorStyle_range = 0x736605;          // vector<Avatar::Style>(first, last)
constexpr uint32_t VectorStyle_dtor = 0x736655;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t Director_getVisibleSize = 0x8F0675, Director_getVisibleOrigin = 0x8F06A1;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_copy = 0x8E4C03, Vec2_dtor = 0x8E4C17;
constexpr uint32_t SceneUtil_createUnionImagePath = 0x46968D;   // (string out, union, int, bool)
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;     // (string out, long long)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t ImageView_loadTexture = 0x839731;      // (view, const string&, TextureResType)
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t FlashDrama_ctor = 0x6F2565;
constexpr uint32_t Avatar_addInstance = 0x5C7385, Avatar_setInstanceName = 0x5C7359;
constexpr uint32_t Avatar_changeStyle = 0x5C5F15, Avatar_changeLWF = 0x5C7575;
constexpr uint32_t SceneManager_getInstance = 0x60C26D, SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;
constexpr uint32_t shared_release = 0xC8DBD1;
}  // namespace g

constexpr uint32_t GOT_VT_SCENE_PARADE = 0xFD6994, GOT_VT_LAYER_BASE = 0xFD694C, GOT_VT_STYLE = 0xFD6138;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_PARADE = 0xFD699C, GOT_CREATE_MYPAGE = 0xFD6078;
constexpr uint32_t SIZE = 0x368;
// members
constexpr uint32_t WIDTH = 0x288, HEIGHT = 0x28C, DRAMA = 0x290, SCALE = 0x294, FN_END = 0x2A8, WIDGET = 0x2C4;
constexpr uint32_t STYLES = 0x2C8, STYLE_SIZE = 0x20, RANKER_SIZE = 0x70;

constexpr uint32_t V_deleting = 0x04, V_setScale = 0x40, V_getScale = 0x44, V_setPosition = 0x4C;
constexpr uint32_t V_getPosition = 0x54, V_setAnchorPoint = 0x9C, V_addChild = 0xF0, V_addChildZ = 0xF4;
constexpr uint32_t V_addChildZName = 0xFC, V_removeChild = 0x12C, V_init = 0x248, V_setTouchEnabled = 0x29C;
constexpr uint32_t V_lwfReplaceTexture = 0x2F0;

uint32_t create() {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(g::AEABI_MEMSET, {self, SIZE, 0});
    call(g::Layer_ctor, {self});
    stf(self + WIDTH, 960.0f);
    stf(self + HEIGHT, 540.0f);
    st32(self + DRAMA, 0);
    st32(self + 0x2A0, 0);
    st32(self + FN_END + 16, 0);
    st8(self + 0x2C0, 0);
    st32(self, ld32(GOT_VT_SCENE_PARADE) + 8);
    st32(self + WIDGET, 0);
    for (uint32_t k = 0; k < 5; k++) {
        const uint32_t sb = self + STYLES + STYLE_SIZE * k;
        call(g::SafeBuffer_ctor, {sb, 0x30, 1});
        st32(sb, ld32(GOT_VT_STYLE) + 8);
    }
    if (vcall(self, V_init, {self}) == 1) {
        call(g::Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_deleting, {self});
    return 0;
}

uint32_t destroy(uint32_t self) {
    st32(self, ld32(GOT_VT_SCENE_PARADE) + 8);
    for (int32_t k = 4; k >= 0; k--) call(g::SafeBuffer_dtor, {self + STYLES + STYLE_SIZE * uint32_t(k)});
    st32(self, ld32(GOT_VT_LAYER_BASE) + 8);
    functionDestroy(self + FN_END);
    call(g::Layer_dtor, {self});
    return self;
}
void destroyDelete(uint32_t self) {
    destroy(self);
    call(fn::operator_delete, {self});
}

// place `index` (0 = 1st .. 4 = 5th): the ranker's name and Lux; the avatar style kept for the parade
void registAvatar(uint32_t self, int32_t index, uint32_t user, uint32_t style, int32_t lux) {
    static constexpr uint32_t PLACES[5] = {0xE071A5, 0xE071A9, 0xE071AD, 0xE071B1, 0xE071B5};   // "1st".."5th"
    const uint32_t i = uint32_t(index) & 0xFF;
    if (i > 4) return;
    const uint32_t place = findNode(ld32(self + WIDGET), PLACES[i], 3);
    const uint32_t name = findNode(place, 0xDFDBB8, 14);                 // "Txt_Name_Label"
    call(g::SceneUtil_setShadow, {name});
    {
        Frame f;
        const uint32_t raw = f.alloc(0x28);
        const uint32_t row = ld32(user);
        call(g::SafeBuffer_read, {row, raw, ld32(row + 0xC) + 0x14, 0x21, 0x14});
        const uint32_t s = newStr(f, raw);
        call(g::Text_setString, {name, s});
        strFree(s);
    }
    titleLabel(place, 0xDFEA5A, 7, 100200001);                            // "Txt_LUX"
    const uint32_t luxLabel = findNode(place, 0xDFEA62, 13);              // "Txt_LUX_Label"
    call(g::SceneUtil_setShadow, {luxLabel});
    {
        Frame f;
        const uint32_t s = f.alloc(12);
        call(g::SceneUtil_getMoneyStr, {s, uint32_t(lux), uint32_t(lux >> 31)});
        call(g::Text_setString, {luxLabel, s});
        strFree(s);
    }
    call(g::SafeBuffer_assign, {self + uint32_t(index) * STYLE_SIZE + STYLES, style});
}

// the parade drama: a FlashDrama scaled like the scene, at the screen's left edge and the scene's height, one
// avatar instance per style ("my_avatar1"..), the parade_ranking LWF; added as "paradeLwfView"
void loadLWF(uint32_t self, uint32_t styles) {
    Frame f;
    const uint32_t origin = f.alloc(8), size = f.alloc(8);
    call(g::Director_getVisibleOrigin, {origin, call(g::Director_getInstance, {})});
    call(g::Director_getVisibleSize, {size, call(g::Director_getInstance, {})});
    const uint32_t drama = call(fn::operator_new, {0x608});
    call(g::FlashDrama_ctor, {drama});
    call(g::Ref_autorelease, {drama});
    vcall(drama, V_setScale, {drama, ld32(self + SCALE)});
    const float y = (ldf(origin + 4) + ldf(size + 4) * 0.5f) + (ldf(self + SCALE) * ldf(self + HEIGHT)) * 0.5f;
    {
        const uint32_t setPos = ld32(ld32(drama) + V_setPosition);
        Frame t;
        const uint32_t pos = t.alloc(8);
        call(g::Vec2_ctor, {pos, ld32(origin), rw::fbits(y)});
        call(setPos, {drama, pos});
        call(g::Vec2_dtor, {pos});
    }
    for (uint32_t i = 0; i < (ld32(styles + 4) - ld32(styles)) >> 5;) {
        const uint32_t style = ld32(styles) + 32 * i;
        if (i) {
            i++;
            Frame t;
            const uint32_t buf = stringBuffer(t, {0xE0716C, i});           // "my_avatar%d"
            call(g::Avatar_addInstance, {drama, buf, style});
        } else {
            call(g::Avatar_setInstanceName, {drama, 0xE07161});            // "my_avatar1"
            i = 1;
            call(g::Avatar_changeStyle, {drama, style});
        }
    }
    call(g::Avatar_changeLWF, {drama, 0xE07178});                        // ".../parade_ranking.lwf"
    st32(self + DRAMA, drama);
    const uint32_t add = ld32(ld32(self) + V_addChildZName);
    Frame t;
    const uint32_t name = newStr(t, 0xE0710E, 13);                       // "paradeLwfView"
    call(add, {self, drama, 100, name});
    strFree(name);
    call(g::Vec2_dtor, {origin});
}

constexpr uint32_t VT_END = 0xF6D55C;
// the scene: the end callback, Parade_Top (no touch), the rankers sorted and registered, the parade, the layout
// centred over it (z 1000), the player's Union emblem and the congratulations LWF moved by the screen's height
uint32_t init(uint32_t self) {
    if (call(g::SceneFlashDrama_init, {self, 0}) != 1) return 0;
    {
        Frame f;
        const uint32_t fo = newFunction(f, VT_END, {});
        call(g::FunctionVV_assign, {self + FN_END, fo});
        functionDestroy(fo);
    }
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), 0xE070EB});
    st32(self + WIDGET, widget);
    vcall(widget, V_setTouchEnabled, {widget, 0});
    const uint32_t api = call(g::APIManager_getInstance, {});
    Frame f;
    const uint32_t word = f.alloc(8), rankers = f.alloc(12), raw = f.alloc(0x30), style = f.alloc(0x20);
    {
        const uint32_t user = ld32(api + 0x94);
        call(g::SafeBuffer_read, {user, word, ld32(user + 0xC) + 0x30, 4, 0x30});
    }
    call(g::VectorRanker_copy, {rankers, ld32(call(g::APIManager_getInstance, {}) + 0x2EC) + 0xC});
    {
        const uint32_t first = ld32(rankers), last = ld32(rankers + 4);
        const uint32_t n = ((last - first) >> 4) * 0xB6DB6DB7u;
        uint32_t buf = 0, bufSize = 0;
        Frame t;
        const uint32_t pair = t.alloc(8), comp = t.alloc(8);
        if (int32_t(last - first) >= 1) {
            call(g::Ranker_getTemporaryBuffer, {pair, n});
            buf = ld32(pair);
            bufSize = ld32(pair + 4);
        }
        call(g::Ranker_stableSort, {first, last, comp, n, buf, bufSize});
        if (buf) call(fn::operator_delete, {buf});
    }
    for (uint32_t i = 0; ld32(rankers + 4) != ld32(rankers) && i < ((ld32(rankers + 4) - ld32(rankers)) >> 4) * 0xB6DB6DB7u; i++) {
        const uint32_t ranker = ld32(rankers) + RANKER_SIZE * i;
        const uint32_t sb = ld32(ranker + 8);
        call(g::SafeBuffer_read, {sb, raw, ld32(sb + 0xC), 0x30, 0});
        call(g::AvatarUtil_makeStyle, {style, raw, ld32(ranker + 0x1C)});
        registAvatar(self, int32_t(int8_t(i)), ranker, style, int32_t(ld32(ranker + 0x6C)));
        call(g::SafeBuffer_dtor, {style});
    }
    {
        Frame t;
        const uint32_t v = t.alloc(12);
        call(g::VectorStyle_range, {v, self + STYLES, self + STYLES + 5 * STYLE_SIZE});
        loadLWF(self, v);
        call(g::VectorStyle_dtor, {v});
    }
    findNode(self, 0xE0710E, 13);                                        // "paradeLwfView" (unused)
    {
        const uint32_t w = ld32(self + WIDGET);
        const uint32_t d = ld32(self + DRAMA);
        const uint32_t setScale = ld32(ld32(w) + V_setScale);
        call(setScale, {w, vcall(d, V_getScale, {d})});
    }
    {
        const uint32_t w = ld32(self + WIDGET);
        vcall(w, V_setAnchorPoint, {w, ld32(GOT_ANCHOR_MIDDLE)});
    }
    const uint32_t visible = f.alloc(8);
    call(g::Director_getVisibleSize, {visible, call(g::Director_getInstance, {})});
    {
        const float x = ldf(visible) * 0.5f, y = ldf(visible + 4) * 0.5f;
        const uint32_t w = ld32(self + WIDGET);
        const uint32_t setPos = ld32(ld32(w) + V_setPosition);
        Frame t;
        const uint32_t pos = t.alloc(8);
        call(g::Vec2_ctor, {pos, rw::fbits(x), rw::fbits(y)});
        call(setPos, {w, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(self, V_addChildZ, {self, ld32(self + WIDGET), 1000});
    const float shift = ldf(visible + 4) + -640.0f;
    const uint32_t emblem = findNode(ld32(self + WIDGET), 0xE0711C, 7);  // "Union_L"
    {
        const uint32_t user = ld32(api + 0x94);
        Frame t;
        const uint32_t uw = t.alloc(4), path = t.alloc(12);
        call(g::SafeBuffer_read, {user, uw, ld32(user + 0xC) + 0x30, 4, 0x30});
        call(g::SceneUtil_createUnionImagePath, {path, ld32(uw), 1, 0});
        const uint32_t s = newStr(t, strData(path));
        call(g::ImageView_loadTexture, {emblem, s, 0});
        strFree(s);
        strFree(path);
    }
    const uint32_t emblemPos = f.alloc(8);
    call(g::Vec2_copy, {emblemPos, vcall(emblem, V_getPosition, {emblem})});
    {
        const float y = ldf(emblemPos + 4) - shift;
        const uint32_t setPos = ld32(ld32(emblem) + V_setPosition);
        Frame t;
        const uint32_t pos = t.alloc(8);
        call(g::Vec2_ctor, {pos, ld32(emblemPos), rw::fbits(y)});
        call(setPos, {emblem, pos});
        call(g::Vec2_dtor, {pos});
    }
    const uint32_t area = findNode(ld32(self + WIDGET), 0xDF9FB4, 10);   // "Area_Point"
    const uint32_t areaPos = f.alloc(8);
    call(g::Vec2_copy, {areaPos, vcall(area, V_getPosition, {area})});
    {
        const float y = shift + ldf(areaPos + 4);
        const uint32_t setPos = ld32(ld32(area) + V_setPosition);
        Frame t;
        const uint32_t pos = t.alloc(8);
        call(g::Vec2_ctor, {pos, ld32(areaPos), rw::fbits(y)});
        call(setPos, {area, pos});
        call(g::Vec2_dtor, {pos});
    }
    const uint32_t congrats = call(g::AudioLWFNode_create, {0xE07124, 0});
    vcall(area, V_addChild, {area, congrats});
    call(g::Vec2_dtor, {areaPos});
    call(g::Vec2_dtor, {emblemPos});
    call(g::VectorRanker_dtor, {rankers});
    return 1;
}

// a scene layer change through createSceneLayer<T> (a function pointer in a std::function)
void changeLayer(uint32_t gotCreate) {
    const uint32_t mgr = call(g::SceneManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(gotCreate)});
    call(g::SceneManager_changeSceneLayer, {mgr, fo, 1, 1, 1});
    functionDestroy(fo);
}
// sub_738F50, a button {parent, child}: on release the child is removed and the parade starts
void paradeButton(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t parent = ld32(functor + 4);
    vcall(parent, V_removeChild, {parent, ld32(functor + 8), 1});
    changeLayer(GOT_CREATE_PARADE);
}
// sub_739204, the parade's end: the MyPage requests again and back to MyPage
void paradeEnd(uint32_t) {
    {
        Frame f;
        const uint32_t fo = newEmptyFunction(f);
        call(g::SceneUtil_callPrepareMypageAPIs, {fo});
        functionDestroy(fo);
    }
    changeLayer(GOT_CREATE_MYPAGE);
}

// the union ranking LWF's texture replacement: (lwf, path, texture) with an empty name and callback
void replaceTexture(uint32_t lwf, uint32_t path, uint32_t texture) {
    const uint32_t fn = ld32(ld32(lwf) + V_lwfReplaceTexture);
    Frame f;
    const uint32_t out = f.alloc(8);
    const uint32_t fo = newEmptyFunction(f);
    call(fn, {out, lwf, path, texture, 0xDF8A1A, fo});
    if (ld32(out + 4)) call(g::shared_release, {ld32(out + 4)});
    functionDestroy(fo);
}
// sub_7395A8 {.., scene, fixed, lwf, .., union id (u64), slot}: the union's fixed emblem LWF (or union_fix10
// when neither the scene's flag +0x298 nor `fixed` is set); the texture is kept in the scene's slot list
void unionFixTexture(uint32_t functor, uint32_t textureRef) {
    const uint32_t texture = ld32(textureRef);
    const uint32_t scene = ld32(functor + 8);
    if (ld8(scene + 0x298) || ld32(functor + 0xC)) {
        const uint32_t lwf = ld32(functor + 0x10);
        const uint32_t lo = ld32(functor + 0x18), hi = ld32(functor + 0x1C);
        Frame f;
        const uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
        call(fn::StringBuffer_ctor, {buf, 0xE07203, lo, hi, lo, hi});
        replaceTexture(lwf, buf, texture);
    } else {
        replaceTexture(ld32(functor + 0x10), 0xE07234, texture);
    }
    st32(ld32(scene + 0x28C) + 4 * ld32(functor + 0x20), texture);
}
// sub_7397D8 {lwf, union}: the union's ranking LWF texture
void unionTexture(uint32_t functor, uint32_t textureRef) {
    const uint32_t texture = ld32(textureRef);
    const uint32_t lwf = ld32(functor + 4), id = ld32(functor + 8);
    Frame f;
    const uint32_t buf = stringBuffer(f, {0xE07261, id, id});           // "lwf/unionranking/union%d/union%d.lwf"
    replaceTexture(lwf, buf, texture);
}
}  // namespace

RW_FUNCTION(0x739060, "SceneParade::create()", create);
RW_FUNCTION(0x736690, "SceneParade::~SceneParade()", destroy);
RW_FUNCTION(0x736708, "SceneParade::~SceneParade() (deleting)", destroyDelete);
RW_FUNCTION(0x735A3C, "SceneParade::init()", init);
RW_FUNCTION(0x736004, "SceneParade::registAvatar(signed char, const shared_ptr<User>&, const Avatar::Style&, int)", registAvatar);
RW_FUNCTION(0x7363EC, "SceneParade::loadLWF(const vector<Avatar::Style>&)", loadLWF);
RW_FUNCTION(0x738F50, "SceneParade start button (sub_738F50)", paradeButton);
RW_FUNCTION(0x739204, "SceneParade end (sub_739204)", paradeEnd);
RW_FUNCTION(0x7395A8, "union ranking fixed emblem texture (sub_7395A8)", unionFixTexture);
RW_FUNCTION(0x7397D8, "union ranking emblem texture (sub_7397D8)", unionTexture);
