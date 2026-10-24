// SceneDrawRaise: life cycle, the comment edit box, isRareMedal and the std::vector<DropMedalLayer> helpers.
#include "SceneDrawRaise.hpp"

using namespace scene;
using Self = SceneDrawRaise;

namespace {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;                // cocos2d::Layer::Layer
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;                // cocos2d::Layer::~Layer
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;              // __aeabi_memset(dest, n, c)
constexpr uint32_t AEABI_MEMCPY = 0x40E164;              // __aeabi_memcpy(dest, src, n)
constexpr uint32_t STRLEN = 0x40E08C;                    // strlen
constexpr uint32_t String_compare = 0x47CBD5;            // std::string::compare(const char*)
constexpr uint32_t EditBox_getText = rwlib::cocos2d::extension::EditBox::getText;           // cocos2d::extension::EditBox::getText
constexpr uint32_t SceneUtil_editBoxEditingDidEnd = 0x45C61D;
constexpr uint32_t Vector_throwLengthError = 0x47B66D;   // __vector_base_common::__throw_length_error

constexpr uint32_t VSLOT_INIT = 0x250;                   // from the vtable start: init()
constexpr uint32_t VSLOT_DELETE = 0x04;                  // deleting destructor (through the object)
constexpr uint32_t VSLOT_GET_NAME = 0x150;               // Node::getName (std::string sret)

constexpr uint32_t STR_TextField_Comment = 0xDF9F0D;

constexpr uint32_t MEDAL_RARE = 0x470;                   // master::Medal SafeBuffer: rarity (stars)
constexpr uint32_t MEDAL_FLAG_454 = 0x454;               // master::Medal SafeBuffer: 1 = counts as rare
constexpr uint32_t USERMEDAL_MASTER = 0x58;              // UserMedalData: shared_ptr<master::Medal> (row = SafeBuffer)
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
SceneDrawRaise::Addr SceneDrawRaise::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->editBoxDelegateVtable = DELEGATE_VTABLE;
    call(AEABI_MEMSET, {self + 0x290, 0x14, 0});     // field_290 and the empty dropMedals vector
    s->field_2A4 = 0.1f;
    s->field_2A8 = 0.1f;
    s->flag_2AD = 0;
    s->flag_2AC = 0;
    s->flag_2AE = 0;
    s->flag_2BC = 0;
    s->commentText = 0;
    s->editBoxLabel = 0;
    s->editBoxLayer = 0;
    if (call(ld32(VTABLE_START + VSLOT_INIT), {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, VSLOT_DELETE, {self});
    return 0;
}
RW_FUNCTION(0x68DEFC, "SceneDrawRaise::create", SceneDrawRaise::create);

SceneDrawRaise::Addr SceneDrawRaise::createSceneLayer() { return create(); }
RW_FUNCTION(0x68DEF4, "createSceneLayer<SceneDrawRaise>", SceneDrawRaise::createSceneLayer);

SceneDrawRaise::Addr SceneDrawRaise::createScene() {
    uint32_t sceneObj = call(Scene_create, {});
    uint32_t layer = create();
    vcall(sceneObj, node::addChild, {sceneObj, layer});
    return sceneObj;
}
RW_FUNCTION(0x68E9BC, "SceneDrawRaise::createScene", SceneDrawRaise::createScene);

SceneDrawRaise::Addr SceneDrawRaise::destroy(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->editBoxDelegateVtable = DELEGATE_VTABLE;
    dropMedalsDestroy(self + 0x294);
    call(Layer_dtor, {self});
    return self;
}
RW_FUNCTION(0x68E9E0, "SceneDrawRaise::~SceneDrawRaise", SceneDrawRaise::destroy);

void SceneDrawRaise::destroyThunk(Addr delegate) {
    uint32_t self = delegate - DELEGATE_OFFSET;
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->editBoxDelegateVtable = DELEGATE_VTABLE;
    dropMedalsDestroy(self + 0x294);
    call(Layer_dtor, {self});
}
RW_FUNCTION(0x68EA54, "SceneDrawRaise::~SceneDrawRaise (thunk)", SceneDrawRaise::destroyThunk);

void SceneDrawRaise::destroyDelete(Addr self) {
    destroy(self);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x68EA94, "SceneDrawRaise::~SceneDrawRaise (deleting)", SceneDrawRaise::destroyDelete);

void SceneDrawRaise::destroyDeleteThunk(Addr delegate) {
    uint32_t self = delegate - DELEGATE_OFFSET;
    Self *s = rw::host_ptr<Self>(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->editBoxDelegateVtable = DELEGATE_VTABLE;
    dropMedalsDestroy(self + 0x294);
    // operator delete gets what ~Layer returned (this)
    call(fn::operator_delete, {call(Layer_dtor, {self})});
}
RW_FUNCTION(0x68EADC, "SceneDrawRaise::~SceneDrawRaise (deleting thunk)", SceneDrawRaise::destroyDeleteThunk);

void SceneDrawRaise::update(Addr, float) {}
RW_FUNCTION(0x68FC7C, "SceneDrawRaise::update", SceneDrawRaise::update);

// ------------------------------------------------------------------------------------------------ edit box
// The edit box text changed: only the comment box is looked at (its text is read and dropped).
void SceneDrawRaise::editBoxTextChanged(Addr, Addr editBox, Addr) {
    Frame f;
    uint32_t name = f.alloc(12);
    vcall(editBox, VSLOT_GET_NAME, {name, editBox});
    if (call(String_compare, {name, STR_TextField_Comment}) == 0) call(EditBox_getText, {editBox});
    strFree(name);
}
RW_FUNCTION(0x68FC80, "SceneDrawRaise::editBoxTextChanged", SceneDrawRaise::editBoxTextChanged);

// the delegate thunk forwards `this` unadjusted (editBoxTextChanged never uses it)
void SceneDrawRaise::editBoxTextChangedThunk(Addr delegate, Addr editBox, Addr text) {
    editBoxTextChanged(delegate, editBox, text);
}
RW_FUNCTION(0x68FD1C, "SceneDrawRaise::editBoxTextChanged (thunk)", SceneDrawRaise::editBoxTextChangedThunk);

// Editing finished: the comment box copies its text into commentText, then the edit box layer / label are
// updated by SceneUtil::editBoxEditingDidEnd.
void SceneDrawRaise::editBoxEditingDidEnd(Addr self, Addr editBox) {
    Self *s = rw::host_ptr<Self>(self);
    Frame f;
    uint32_t name = f.alloc(12);
    vcall(editBox, VSLOT_GET_NAME, {name, editBox});
    if (call(String_compare, {name, STR_TextField_Comment}) == 0) {
        uint32_t text = call(EditBox_getText, {editBox});
        uint32_t str = f.alloc(12);
        strInit(str, text, call(STRLEN, {text}));
        uint32_t label = s->commentText;
        if (label) {
            uint32_t chars = strData(str);
            uint32_t copy = f.alloc(12);
            strInit(copy, chars, call(STRLEN, {chars}));
            call(fn::Text_setString, {label, copy});
            strFree(copy);
        }
        strFree(str);
    }
    uint32_t label = s->editBoxLabel;
    uint32_t text = call(EditBox_getText, {editBox});
    call(SceneUtil_editBoxEditingDidEnd, {self + 0x2B0, label, text});
    strFree(name);
}
RW_FUNCTION(0x68FD24, "SceneDrawRaise::editBoxEditingDidEnd", SceneDrawRaise::editBoxEditingDidEnd);

void SceneDrawRaise::editBoxEditingDidEndThunk(Addr delegate, Addr editBox) {
    editBoxEditingDidEnd(delegate - DELEGATE_OFFSET, editBox);
}
RW_FUNCTION(0x68FEA0, "SceneDrawRaise::editBoxEditingDidEnd (thunk)", SceneDrawRaise::editBoxEditingDidEndThunk);

void SceneDrawRaise::editBoxEditingDidBegin(Addr, Addr) {}
RW_FUNCTION(0x6928CC, "SceneDrawRaise::editBoxEditingDidBegin", SceneDrawRaise::editBoxEditingDidBegin);
RW_FUNCTION(0x6928D0, "SceneDrawRaise::editBoxEditingDidBegin (thunk)", SceneDrawRaise::editBoxEditingDidBegin);

void SceneDrawRaise::editBoxReturn(Addr, Addr) {}
RW_FUNCTION(0x6928CE, "SceneDrawRaise::editBoxReturn", SceneDrawRaise::editBoxReturn);
RW_FUNCTION(0x6928D2, "SceneDrawRaise::editBoxReturn (thunk)", SceneDrawRaise::editBoxReturn);

// ------------------------------------------------------------------------------------------------ medals
// A medal counts as rare from 3 stars when its master row flags it.
uint32_t SceneDrawRaise::isRareMedal(Addr, Addr medal) {
    uint32_t row = ld32(ld32(medal) + USERMEDAL_MASTER);
    if (safe32(row, MEDAL_RARE) < 3) return 0;
    return safe32(row, MEDAL_FLAG_454) == 1 ? 1 : 0;
}
RW_FUNCTION(0x69284C, "SceneDrawRaise::isRareMedal", SceneDrawRaise::isRareMedal);

// ------------------------------------------------------------------------------------------------ vector<DropMedalLayer>
SceneDrawRaise::Addr SceneDrawRaise::dropMedalsDestroy(Addr vec) {
    uint32_t begin = ld32(vec);
    if (begin) {
        uint32_t end = ld32(vec + 4);
        if (end != begin) st32(vec + 4, end + (~(end - 16 - begin) & 0xFFFFFFF0u));
        call(fn::operator_delete, {begin});
    }
    return vec;
}
RW_FUNCTION(0x68EA24, "std::__vector_base<SceneDrawRaise::DropMedalLayer>::~__vector_base",
            SceneDrawRaise::dropMedalsDestroy);

SceneDrawRaise::Addr SceneDrawRaise::dropMedalsSplitBufferDestroy(Addr buf) {
    uint32_t begin = ld32(buf + 4), end = ld32(buf + 8);
    if (end != begin) st32(buf + 8, end + (~(end - 16 - begin) & 0xFFFFFFF0u));
    if (ld32(buf)) call(fn::operator_delete, {ld32(buf)});
    return buf;
}
RW_FUNCTION(0x694330, "std::__split_buffer<SceneDrawRaise::DropMedalLayer>::~__split_buffer",
            SceneDrawRaise::dropMedalsSplitBufferDestroy);

// push_back when the vector is full: grow (double, at least size + 1), copy the new element, move the old ones in
// front of it (memcpy), swap the buffers through a __split_buffer and free the old storage.
void SceneDrawRaise::dropMedalsPushBackSlow(Addr vec, Addr value) {
    uint32_t oldBegin = ld32(vec);
    uint32_t size = uint32_t(int32_t(ld32(vec + 4) - oldBegin) >> 4);
    uint32_t want = size + 1;
    if (want >> 28) {
        call(Vector_throwLengthError, {vec});
        return;
    }
    uint32_t capBytes = ld32(vec + 8) - oldBegin;
    uint32_t cap;
    if (uint32_t(int32_t(capBytes) >> 4) >= 0x7FFFFFFu) {
        cap = 0xFFFFFFFu;
    } else {
        cap = uint32_t(int32_t(capBytes) >> 3);
        if (cap < want) cap = want;
    }
    uint32_t buffer = cap ? call(fn::operator_new, {cap << 4}) : 0;
    uint32_t slot = buffer + (size << 4);
    uint32_t capEnd = buffer + (cap << 4);
    if (slot) {
        uint32_t w0 = ld32(value), w1 = ld32(value + 4), w2 = ld32(value + 8), w3 = ld32(value + 12);
        st32(slot, w0);
        st32(slot + 4, w1);
        st32(slot + 8, w2);
        st32(slot + 12, w3);
    }
    uint32_t begin = ld32(vec), end = ld32(vec + 4);
    uint32_t newBegin = buffer + ((size - ((end - begin) >> 4)) << 4);
    call(AEABI_MEMCPY, {newBegin, begin, end - begin});
    st32(vec, newBegin);
    st32(vec + 4, slot + 16);
    uint32_t oldCap = ld32(vec + 8);
    st32(vec + 8, capEnd);
    // the split buffer now holds the old storage: first = begin = old begin, end = old end, cap = old cap
    Frame f;
    uint32_t split = f.alloc(16);
    st32(split, begin);
    st32(split + 4, begin);
    st32(split + 8, end);
    st32(split + 12, oldCap);
    dropMedalsSplitBufferDestroy(split);
}
RW_FUNCTION(0x69421C, "std::vector<SceneDrawRaise::DropMedalLayer>::__push_back_slow_path",
            SceneDrawRaise::dropMedalsPushBackSlow);
