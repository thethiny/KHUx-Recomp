// Medal List > long press > Level Up > Optimize > Level Up: the fusion itself - SceneMixedMedal::runMixed (the
// materials' master ids into the scene's list, their user ids into a set, the lock update, then the enhance API in
// its $_24 callback) and openRareWarnPopup (a caution panel per reason: rare material, skill overwrite / mix, guilt
// change). The result popup and its lambdas are in MixedResult.cpp.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_callUpdateLockMedalAPI = 0x473DF5;  // (function<void(bool)> by value)
constexpr uint32_t ULLSet_copy = 0x4740A9, ULLSet_move = 0x6A5035, ULLSet_dtor = 0x482619;
constexpr uint32_t ULLSet_insertUnique = 0x482649;               // (pair out, set, const ull&)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t VectorInt_pushBackSlow = 0x47D709;            // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t VectorS8_pushBackSlow = 0x6A5CF5;             // vector<signed char>::__push_back_slow_path
constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_executeUserMedalEnhance = 0x4CA119;  // (api, ull base, set&, bool skill mix, fn&)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_getPopupLayer = 0x613E69, SM_openPopup = 0x61524D;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create, Node_runAction = rwlib::cocos2d::Node::runAction;
}  // namespace g

// SceneMixedMedal
constexpr uint32_t F_MATERIALS = 0x310;        // vector<ull>: the chosen materials' user medal ids
constexpr uint32_t F_BASE = 0x31C;             // the base medal's UserMedalData
constexpr uint32_t F_WARN_RARE = 0x33C, F_WARN_SKILL = 0x33D, F_WARN_GUILT = 0x33E;
constexpr uint32_t F_SKILL_MIX = 0x380;        // skill mix (overwrite) chosen
constexpr uint32_t F_MATERIAL_IDS = 0x394;     // vector<int>: the materials' master ids
constexpr uint32_t VT_Enhance = 0xF658BC;      // runMixed $_24 {this, set<ull>, this, this} (heap 0x24)
constexpr uint32_t VT_EnhanceDone = 0xF658E8;  // its API callback {this, this}
constexpr uint32_t VT_EnhanceError = 0xF65A1C; // the error dialog's callback {}
// the rare caution popup
constexpr uint32_t S_RareJson = 0xE05046;      // "cocostudio/publish/PopupNormal_MedalMix_RareCaution.json"
constexpr uint32_t S_PanelJson = 0xE0509E;     // "cocostudio/publish/PopupNormal_MedalMix_RareCaution_Panel.json"
constexpr uint32_t S_Window = 0xE0507F, S_ButtonOK = 0xDF8688, S_ButtonClose = 0xDF8692, S_Wording3 = 0xE0014B;
constexpr uint32_t S_AlertArea = 0xE05093, S_Wording1 = 0xDFF14C, S_Star1 = 0xE050DD, S_Wording2 = 0xDF863B;
constexpr uint32_t S_TxtOK = 0xDF869F, S_TxtCancel = 0xDF86A6;
constexpr uint32_t VT_RareOK = 0xF65C00, VT_RareCancel = 0xF65BD4;   // {}
constexpr uint32_t VT_RareOpen = 0xF65BA8;     // openPopup fn<bool()> {close button}
constexpr uint32_t VT_RareClose = 0xF65B50;    // openPopup fn<void(bool)> {ok, close, window, layer, shadow, popup,
                                               //   callback} (heap 0x38)
constexpr uint32_t V_setTouchEnabledW = 0x29C;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void shadow(uint32_t text) { SceneUtil::setShadow(text); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
void shadowTextId(uint32_t text, int32_t id) {
    shadow(text);
    setTextId(text, id);
}
// vector<signed char>::push_back on a stack vector {begin, end, cap}
void pushS8(uint32_t vec, int8_t v) {
    Frame f;
    const uint32_t tmp = f.alloc(4);
    st8(tmp, uint8_t(v));
    const uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t at = 0;
        if (end) {
            st8(end, uint8_t(v));
            at = ld32(vec + 4);
        }
        st32(vec + 4, at + 1);
    } else {
        call(g::VectorS8_pushBackSlow, {vec, tmp});
    }
}
}  // namespace

struct SceneMixedMedalFusion {
    static void runMixed(uint32_t self);
    static void enhance(uint32_t functor, uint32_t okRef);
    static uint32_t cloneEnhance(uint32_t functor);
    static void openRareWarnPopup(uint32_t self, uint32_t medalRef, uint32_t callback);
};

// The materials' master ids (each material's UserMedal row, +0 of its master buffer) into the scene's id list,
// their user ids into a set, the loading indicator, then the lock update API whose callback ($_24) runs the
// enhance API with that set.
void SceneMixedMedalFusion::runMixed(uint32_t me) {
    {
        const uint32_t end = ld32(me + F_MATERIAL_IDS + 4), begin = ld32(me + F_MATERIAL_IDS);
        if (end != begin) st32(me + F_MATERIAL_IDS + 4, end + (~(end - 4 - begin) & ~3u));
    }
    Frame f;
    const uint32_t set = f.alloc(20);
    auto initSet = [&] {
        for (uint32_t k = 0; k < 16; k += 4) st32(set + k, 0);
        st32(set + 16, fbits(1.0f));
    };
    const uint32_t mats = me + F_MATERIALS;
    if (ld32(mats + 4) == ld32(mats)) {
        initSet();
    } else {
        uint32_t i = 0;
        do {
            const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
            if (!(uint32_t(int32_t(ld32(mats + 4) - ld32(mats)) >> 3) > i)) {
                call(g::ThrowOutOfRange, {mats});
                return;
            }
            const uint32_t at = ld32(mats) + 8 * i;
            Frame g2;
            const uint32_t medal = g2.alloc(8), value = g2.alloc(4), id = g2.alloc(4);
            call(g::MedalDataManager_getMedalData, {medal, mdm, ld32(at), ld32(at + 4), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
            const uint32_t sb = ld32(ld32(medal) + 0x58);
            rw::call(g::SafeBuffer_read, {sb, value, ld32(sb + 0xC), 4, 0});
            st32(id, ld32(value));
            const uint32_t vec = me + F_MATERIAL_IDS;
            const uint32_t end = ld32(vec + 4);
            if (end < ld32(vec + 8)) {
                uint32_t p = 0;
                if (end) {
                    st32(end, ld32(id));
                    p = ld32(vec + 4);
                }
                st32(vec + 4, p + 4);
            } else {
                call(g::VectorInt_pushBackSlow, {vec, id});
            }
            if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
            ++i;
        } while (i < uint32_t(int32_t(ld32(mats + 4) - ld32(mats)) >> 3));
        initSet();
        for (uint32_t k = 0; k < uint32_t(int32_t(ld32(mats + 4) - ld32(mats)) >> 3); ++k) {
            Frame g2;
            const uint32_t out = g2.alloc(8);
            call(g::ULLSet_insertUnique, {out, set, ld32(mats) + 8 * k});
        }
    }
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t copy = f.alloc(20);
    call(g::ULLSet_copy, {copy, set});
    const uint32_t cb = f.alloc(20);
    st32(cb + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x24});
    if (heap) {
        st32(heap, VT_Enhance);
        st32(heap + 4, me);
        call(g::ULLSet_move, {heap + 8, copy});
        st32(heap + 0x1C, me);
        st32(heap + 0x20, me);
    }
    st32(cb + 16, heap);
    call(g::SceneUtil_callUpdateLockMedalAPI, {cb});
    functionDestroy(cb);
    call(g::ULLSet_dtor, {copy});
    call(g::ULLSet_dtor, {set});
}

// $_24 (the lock update finished): the enhance API on the base medal with the material set and the skill mix
// choice; on a failure the loading indicator goes and, when the API asks for it, its error dialog opens.
void SceneMixedMedalFusion::enhance(uint32_t functor, uint32_t okRef) {
    Frame f;
    if (ld8(okRef)) {
        const uint32_t me = ld32(functor + 4);
        const uint32_t api = call(g::APIManager_getInstance, {});
        const uint32_t base = ld32(me + F_BASE);
        const uint32_t lo = ld32(base), hi = ld32(base + 4);
        const uint32_t skillMix = ld8(me + F_SKILL_MIX);
        const uint32_t done = newFunction(f, VT_EnhanceDone, {ld32(functor + 0x1C), ld32(functor + 0x20)});
        call(g::APIManager_executeUserMedalEnhance, {api, ld32(functor + 0x20), lo, hi, functor + 8, skillMix, done});
        functionDestroy(done);
        return;
    }
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (!ld8(call(g::APIManager_getInstance, {}) + 0x14)) return;
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t cb = newFunction(f, VT_EnhanceError, {});
    call(g::APIManager_openErrorDialog, {api, cb, 1});
    functionDestroy(cb);
}

// $_24 __func::__clone(): a heap copy (the set copied)
uint32_t SceneMixedMedalFusion::cloneEnhance(uint32_t functor) {
    const uint32_t copy = call(fn::operator_new, {0x24});
    if (copy) {
        st32(copy, VT_Enhance);
        st32(copy + 4, ld32(functor + 4));
        call(g::ULLSet_copy, {copy + 8, functor + 8});
        st32(copy + 0x1C, ld32(functor + 0x1C));
        st32(copy + 0x20, ld32(functor + 0x20));
    }
    return copy;
}

// The caution before a fusion: the shadow and RareCaution popup on the popup layer (offset by the side margins),
// one panel per reason stacked in the alert area (rare material; skill overwrite or skill mix; guilt change), OK
// and Cancel, the open animation, and the popup registered with the scene manager (its close callback holds the
// widgets and the callback).
void SceneMixedMedalFusion::openRareWarnPopup(uint32_t me, uint32_t, uint32_t callback) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadowSprite = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(shadowSprite, node::getOpacity, {shadowSprite});
    vcall(shadowSprite, node::setOpacity, {shadowSprite, 0});
    call(g::Node_runAction, {shadowSprite, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadowSprite});
    const uint32_t popup = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_RareJson});
    {
        const float x = bitsf(vcall(popup, node::getPositionX, {popup}));
        const float m = bitsf(call(g::SceneUtil_getSideMargin, {}));
        vcall(popup, node::setPositionX, {popup, fbits(x + m)});
        const float y = bitsf(vcall(popup, node::getPositionY, {popup}));
        const float hm = bitsf(call(g::SceneUtil_getSideHeightMargin, {}));
        vcall(popup, node::setPositionY, {popup, fbits(y + hm)});
    }
    vcall(layer, node::addChild, {layer, popup});
    const uint32_t window = find(popup, S_Window, 19);
    const uint32_t ok = find(popup, S_ButtonOK, 9);
    const uint32_t close = find(popup, S_ButtonClose, 12);
    Frame f;
    const uint32_t cbCopy = f.alloc(20);
    call(g::FunctionVB_copy, {cbCopy, callback});
    shadowTextId(find(popup, S_Wording3, 12), 101300014);
    const uint32_t reasons = f.alloc(12);
    st32(reasons, 0);
    st32(reasons + 4, 0);
    st32(reasons + 8, 0);
    if (ld8(me + F_WARN_RARE)) pushS8(reasons, 1);
    if (ld8(me + F_WARN_SKILL)) pushS8(reasons, ld8(me + F_SKILL_MIX) ? 3 : 2);
    if (ld8(me + F_WARN_GUILT)) pushS8(reasons, 4);
    const uint32_t alert = find(popup, S_AlertArea, 10);
    for (uint32_t i = 0; i < ld32(reasons + 4) - ld32(reasons); ++i) {
        const uint32_t reason = ld32(reasons) + i;
        const uint32_t panel = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_PanelJson});
        const uint32_t w1 = find(panel, S_Wording1, 12);
        visible(w1, 0);
        const uint32_t star = find(panel, S_Star1, 5);
        visible(star, 0);
        const uint32_t w2 = find(panel, S_Wording2, 12);
        visible(w2, 0);
        const uint32_t w3 = find(panel, S_Wording3, 12);
        visible(w3, 0);
        switch (int8_t(ld8(reason))) {
        case 1:
            visible(w1, 1);
            shadowTextId(w1, 101400008);
            visible(star, 1);
            visible(w2, 1);
            shadowTextId(w2, 101400009);
            break;
        case 2: visible(w3, 1); shadowTextId(w3, 101300031); break;
        case 3: visible(w3, 1); shadowTextId(w3, 101300024); break;
        case 4: visible(w3, 1); shadowTextId(w3, 101300025); break;
        default: break;
        }
        const float h = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
        const uint32_t count = ld32(reasons + 4) - ld32(reasons);
        const float ph = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
        const float total = float(count) * ph;
        const float ah = ldf(vcall(alert, node::getContentSize, {alert}) + 4);
        const float top = total + (ah - total) * 0.5f;
        vcall(panel, node::setPositionY, {panel, fbits(top - float(int32_t(i + 1)) * h)});
        vcall(alert, node::addChild, {alert, panel});
    }
    {
        Frame g2;
        const uint32_t cb = newFunction(g2, VT_RareOK, {});
        call(g::Widget_addTouchEventListener, {ok, cb});
        functionDestroy(cb);
    }
    shadowTextId(find(ok, S_TxtOK, 6), 100100001);
    {
        Frame g2;
        const uint32_t cb = newFunction(g2, VT_RareCancel, {});
        call(g::Widget_addTouchEventListener, {close, cb});
        functionDestroy(cb);
    }
    shadowTextId(find(close, S_TxtCancel, 10), 100100002);
    {
        Frame g2;
        const uint32_t none = newEmptyFunction(g2);
        call(g::Node_runAction, {window, SceneUtil::createOpenPopupAnimation(window, none)});
        functionDestroy(none);
    }
    const uint32_t sm = sceneManager();
    const uint32_t openFn = newFunction(f, VT_RareOpen, {close});
    const uint32_t cbTmp = f.alloc(20);
    functionCopy(cbTmp, cbCopy);
    const uint32_t closeCb = f.alloc(20);
    st32(closeCb + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x38});
    if (heap) {
        st32(heap, VT_RareClose);
        st32(heap + 0x14, layer);
        st32(heap + 0x10, window);
        st32(heap + 0xC, close);
        st32(heap + 8, ok);
        st32(heap + 0x1C, popup);
        st32(heap + 0x18, shadowSprite);
        call(g::FunctionVB_move, {heap + 0x20, cbTmp});
    }
    st32(closeCb + 16, heap);
    call(g::SM_openPopup, {sm, openFn, closeCb, 1});
    functionDestroy(closeCb);
    functionDestroy(cbTmp);
    functionDestroy(openFn);
    if (const uint32_t begin = ld32(reasons)) {
        if (ld32(reasons + 4) != begin) st32(reasons + 4, begin);
        call(fn::operator_delete, {begin});
    }
    functionDestroy(cbCopy);
}

RW_FUNCTION(0x69DB88, "SceneMixedMedal::runMixed", SceneMixedMedalFusion::runMixed);
RW_FUNCTION(0x6A3E64, "SceneMixedMedal::runMixed $_24 __func::operator() (sub_6A3E64)", SceneMixedMedalFusion::enhance);
RW_FUNCTION(0x6A3DA4, "SceneMixedMedal::runMixed $_24 __func::__clone() (sub_6A3DA4)", SceneMixedMedalFusion::cloneEnhance);
RW_FUNCTION(0x69BE84, "SceneMixedMedal::openRareWarnPopup", SceneMixedMedalFusion::openRareWarnPopup);
