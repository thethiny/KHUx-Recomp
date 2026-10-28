// AvatarInfoDialog::openPopUpPlayStyle: the play style popup of the Player Information page (button C). Six play
// time check boxes (up to three of the time zones 1..6; the player's set at +0x2F4 is copied into the vector +0x444
// being edited) and three exclusive style check boxes (+0x440, from the player's style +0x2F0: 3 A, 2 B, 1 C).
// sub_539240 syncs the time boxes with the vector: checked when chosen, touchable while fewer than three are.
#include "../scene_menu/AvatarInfoDialog.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t FunctionVB_copy = 0x45C84D;              // std::function<void(bool)>(const&)
constexpr uint32_t FunctionVB_move = 0x47BC69;              // std::function<void(bool)>(&&)
constexpr uint32_t SceneManager_openPopup = 0x61524D;       // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t TimezoneSet_lowerBound = 0x54B0D1;       // std::__lower_bound(root, end node, const key&, comp)
constexpr uint32_t TimezoneVector_pushBackSlow = 0x544D15;  // vector<Timezone>::__push_back_slow_path(vec, &&)
constexpr uint32_t TimezoneSort = 0x54D231;                 // std::__sort(first, last, comp)
constexpr uint32_t CheckBox_addEventListener = rwlib::cocos2d::ui::CheckBox::addEventListener;    // (box, const function<void(Ref*, EventType)>&)
constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState;    // (box, bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t FunctionVB_call = 0x474139;              // std::function<void(bool)>::operator()(bool)
constexpr uint32_t TimezoneSet_insertUnique = 0x54CE39;     // (pair out, tree, const Timezone&)
constexpr uint32_t TimezoneSet_assignMulti = 0x54CC0D;      // (tree, first, last)
constexpr uint32_t TimezoneSet_destroy = 0x54CD73;          // (tree, node)
constexpr uint32_t ZoneRemove = 0x5442B1;                   // sub_5442B0(&dialog, zone): the zone erased from +0x444
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (sm, bool)
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;        // (sm, bool) -> bool
constexpr uint32_t APIManager_getInstance = 0x4BA8C9, APIManager_openErrorDialog = 0x4C3C8D;   // (api, function<void()>, bool)
constexpr uint32_t APIManager_executeUserPlaystyleUpdate = 0x4C8741; // (api, set<Timezone>&, style, function<void(bool)>)
}  // namespace g

constexpr uint32_t S_Json = 0xDFE423, S_TxtPlayTime = 0xDFE450, S_TxtSelect = 0xDFE4B7, S_TxtSetting = 0xDFE4C2,
                   S_TxtNumLavel = 0xDFE4CE, S_TxtStyle = 0xDFE548, S_TxtOK = 0xDF869F, S_ButtonOk = 0xDF9564,
                   S_TxtCancel = 0xDF86A6, S_ButtonClose = 0xDF8692;
constexpr uint32_t S_TxtPlayTimeX[6] = {0xDFE45D, 0xDFE46C, 0xDFE47B, 0xDFE48A, 0xDFE499, 0xDFE4A8};   // _A.._F
constexpr uint32_t S_ButtonPlayTimeX[6] = {0xDFE4DC, 0xDFE4EE, 0xDFE500, 0xDFE512, 0xDFE524, 0xDFE536};
constexpr uint32_t S_TxtStyleX[3] = {0xDFE552, 0xDFE55E, 0xDFE56A};
constexpr uint32_t S_ButtonStyleX[3] = {0xDFE576, 0xDFE585, 0xDFE594};
constexpr int32_t TXT_PLAY_TIME = 102400001, TXT_PLAY_TIME_A = 102100013, TXT_SELECT = 102400002,
                  TXT_STYLE = 102400003, TXT_OK = 100100001, TXT_CANCEL = 100100002;
constexpr int32_t TXT_STYLE_X[3] = {102400006, 102400005, 102400004};
constexpr uint32_t ZONES = 6, MAX_ZONES = 3;

constexpr uint32_t D_STYLE = 0x2F0;                    // AvatarInfoDialog: the player's play style
constexpr uint32_t D_ZONES = 0x2F4;                    // std::set<Timezone> the player's play times
constexpr uint32_t D_EDIT_STYLE = 0x440;               // the popup's style
constexpr uint32_t D_EDIT_ZONES = 0x444;               // std::vector<Timezone> the popup's play times

constexpr uint32_t V_SET_VISIBLE = 0xB0, V_GET_OPACITY = 0x20C, V_SET_OPACITY = 0x214, V_ADD_CHILD = 0xF0;
constexpr uint32_t V_SET_ANCHOR = 0x9C, V_SET_POSITION = 0x4C, V_GET_X = 0x68, V_GET_Y = 0x70, V_GET_SIZE = 0xAC;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C;        // ui::Widget

constexpr uint32_t VT_ZoneTouch[6] = {0xF580A0, 0xF58074, 0xF58048, 0xF5801C, 0xF57FF0, 0xF57FC4};
constexpr uint32_t VT_StyleTouch[3] = {0xF57F98, 0xF57F6C, 0xF57F40};
constexpr uint32_t VT_OkTouch = 0xF57EBC;              // Button_Ok {ok, dialog, view, layer, shadow, onClosed} (0x38)
constexpr uint32_t VT_CancelTouch = 0xF57E90;          // Button_Close (no captures)
constexpr uint32_t VT_CanClose = 0xF57E64;             // openPopup's function<bool()> {close}
constexpr uint32_t VT_Closed = 0xF57E0C;               // openPopup's result {ok, -, view, layer, shadow, onClosed}
constexpr uint32_t VT_Closing = 0xF57E38;              // the close animation's end {layer, shadow, view}
constexpr uint32_t VT_Updated = 0xF57EE8;              // executeUserPlaystyleUpdate's result {view, layer, shadow, -, onClosed}
constexpr uint32_t VT_UpdateError = 0xF57F14;          // the error dialog's callback (no captures)
constexpr uint32_t API_SHOW_ERROR = 0x14;              // APIManager: uint8, the error dialog is wanted
constexpr uint32_t G_SHADOW_FADE = 0xFD65C0;           // GOT: the popup shadow's fade time (float)

// text->setString(TextData `id`) then its shadow / the shadow first
void textShadow(uint32_t text, int32_t id) {
    setTextId(text, id);
    SceneUtil::setShadow(text);
}
void shadowText(uint32_t text, int32_t id) {
    SceneUtil::setShadow(text);
    setTextId(text, id);
}

void addCheckListener(uint32_t box, uint32_t vt, std::initializer_list<uint32_t> caps, uint32_t size) {
    Frame f;
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {size});
    if (obj) {
        st32(obj, vt);
        uint32_t k = 4;
        for (uint32_t w : caps) { st32(obj + k, w); k += 4; }
    }
    st32(fo + 16, obj);
    call(g::CheckBox_addEventListener, {box, fo});
    functionDestroy(fo);
}

// vector<Timezone>::push_back as compiled inline
void pushZone(uint32_t vec, uint32_t zone, uint32_t tmp) {
    const uint32_t end = ld32(vec + 4);
    if (end >= ld32(vec + 8)) {
        st32(tmp, zone);
        call(g::TimezoneVector_pushBackSlow, {vec, tmp});
        return;
    }
    uint32_t next = 0;
    if (end) {
        st32(end, zone);
        next = ld32(vec + 4);
    }
    st32(vec + 4, next + 4);
}

// first element not less than `key` in the sorted [first, first + n)
uint32_t lowerBound(uint32_t first, uint32_t n, int32_t key) {
    while (n) {
        const uint32_t half = uint32_t(int32_t(n) / 2);
        const uint32_t mid = first + 4 * half;
        if (int32_t(ld32(mid)) <= key - 1) {
            first = mid + 4;
            n = n - 1 - half;
        } else {
            n = half;
        }
    }
    return first;
}

struct AvatarInfoPlayStyle {
    // sub_539240 {dialog, time boxes A..F}
    static void syncZones(uint32_t ctx) {
        const uint32_t dialog = ld32(ctx);
        const uint32_t vec = dialog + D_EDIT_ZONES;
        {
            Frame f;
            const uint32_t comp = f.alloc(4);
            call(g::TimezoneSort, {ld32(vec), ld32(vec + 4), comp});
        }
        const uint32_t count = (ld32(vec + 4) - ld32(vec)) >> 2;
        for (uint32_t k = 1; k <= ZONES; k++) {
            const uint32_t it = lowerBound(ld32(vec), (ld32(vec + 4) - ld32(vec)) >> 2, int32_t(k));
            const bool chosen = it != ld32(vec + 4) && int32_t(ld32(it)) < int32_t(k) + 1;
            const uint32_t box = ld32(ctx + 4 * k);
            vcall(box, V_SET_TOUCH_ENABLED, {box, (count < MAX_ZONES || chosen) ? 1u : 0u});
            call(g::CheckBox_setSelectedState, {box, chosen ? 1u : 0u});
        }
    }

    // the popup's close {view, layer, shadow, -, onClosed}(result): the view shrinks (a lambda {layer, shadow,
    // view} ends the close), the shadow fades out, then onClosed(result) when there is one (sub_543260)
    static void close(uint32_t ctx, uint32_t result) {
        const uint32_t view = ld32(ctx);
        {
            Frame f;
            const uint32_t fo = newFunction(f, VT_Closing, {ld32(ctx + 4), ld32(ctx + 8), view});
            call(fn::Node_runAction, {view, SceneUtil::createClosePopupAnimation(view, fo)});
            functionDestroy(fo);
        }
        call(fn::Node_runAction, {ld32(ctx + 8), call(fn::FadeTo_create, {ld32(ld32(G_SHADOW_FADE)), 0})});
        if (ld32(ctx + 0x20)) call(g::FunctionVB_call, {ctx + 0x10, result & 0xFF});
    }

    // a time box {dialog, dialog, boxes A..F, dialog}(sender, type): checked -> the zone added while fewer than
    // three are chosen; unchecked -> removed; then the boxes synced ($_26..$_31)
    template <uint32_t Zone> static void zoneTouch(uint32_t self, uint32_t, uint32_t typep) {
        const uint32_t type = ld32(typep);
        const uint32_t dialog = ld32(self + 4);
        call(g::AudioPlaySystemSEDecide, {});
        if (type) {
            call(g::ZoneRemove, {self + 0x24, Zone});
        } else {
            const uint32_t vec = dialog + D_EDIT_ZONES;
            if (((ld32(vec + 4) - ld32(vec)) >> 2) <= MAX_ZONES - 1) {
                Frame f;
                pushZone(vec, Zone, f.alloc(4));
            }
        }
        syncZones(self + 8);
    }

    // a style box {dialog, box 3, dialog, box 2, box 1}(sender, type): checked -> the style chosen and the three
    // boxes show it; unchecked -> no style ($_33..$_35)
    template <uint32_t Style> static void styleTouch(uint32_t self, uint32_t, uint32_t typep) {
        const uint32_t edit = ld32(self + 4) + D_EDIT_STYLE;
        const uint32_t type = ld32(typep);
        call(g::AudioPlaySystemSEDecide, {});
        if (type) {
            st32(edit, 0);
            return;
        }
        st32(edit, Style);
        const uint32_t dialog = ld32(self + 12);
        call(g::CheckBox_setSelectedState, {ld32(self + 8), ld32(dialog + D_EDIT_STYLE) == 3 ? 1u : 0u});
        call(g::CheckBox_setSelectedState, {ld32(self + 16), ld32(dialog + D_EDIT_STYLE) == 2 ? 1u : 0u});
        call(g::CheckBox_setSelectedState, {ld32(self + 20), ld32(dialog + D_EDIT_STYLE) == 1 ? 1u : 0u});
    }

    // Button_Ok {-, ok, dialog, view, layer, shadow, -, onClosed}(sender, type): ended -> the button disabled, the
    // edited style and the chosen zones (as a set) become the player's and go to the server ($_38)
    static void okTouch(uint32_t self, uint32_t, uint32_t typep) {
        if (ld32(typep) != 2) return;
        const uint32_t dialog = ld32(self + 12);
        call(g::AudioPlaySystemSEDecide, {});
        const uint32_t ok = ld32(self + 8);
        vcall(ok, V_SET_TOUCH_ENABLED, {ok, 0});
        st32(dialog + D_STYLE, ld32(dialog + D_EDIT_STYLE));
        Frame f;
        const uint32_t tree = f.alloc(12);       // std::set<Timezone>: begin node, root (the end node's left), size
        st32(tree + 4, 0);
        st32(tree + 8, 0);
        st32(tree, tree + 4);
        const uint32_t vec = dialog + D_EDIT_ZONES;
        {
            const uint32_t key = f.alloc(4), result = f.alloc(8);
            for (uint32_t z = 1; z <= ZONES; z++) {
                const uint32_t it = lowerBound(ld32(vec), (ld32(vec + 4) - ld32(vec)) >> 2, int32_t(z));
                if (it == ld32(vec + 4) || int32_t(ld32(it)) > int32_t(z)) continue;
                st32(key, z);
                call(g::TimezoneSet_insertUnique, {result, tree, key});
            }
        }
        const uint32_t zones = dialog + D_ZONES;
        if (zones != tree) call(g::TimezoneSet_assignMulti, {zones, ld32(tree), tree + 4});
        const uint32_t closer = f.alloc(0x28);   // {view, layer, shadow, -, onClosed}
        for (uint32_t k = 0; k < 12; k += 4) st32(closer + k, ld32(self + 16 + k));
        call(g::FunctionVB_copy, {closer + 0x10, self + 0x20});
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(g::APIManager_getInstance, {});
        const uint32_t style = ld32(dialog + D_STYLE);
        const uint32_t copy = f.alloc(0x28);
        for (uint32_t k = 0; k < 12; k += 4) st32(copy + k, ld32(closer + k));
        functionCopy(copy + 0x10, closer + 0x10);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, VT_Updated);
            for (uint32_t k = 0; k < 12; k += 4) st32(obj + 8 + k, ld32(copy + k));
            call(g::FunctionVB_move, {obj + 0x18, copy + 0x10});
        }
        st32(fo + 16, obj);
        call(g::APIManager_executeUserPlaystyleUpdate, {api, zones, style, fo});
        functionDestroy(fo);
        functionDestroy(copy + 0x10);
        functionDestroy(closer + 0x10);
        call(g::TimezoneSet_destroy, {tree, ld32(tree + 4)});
    }

    // the play style update's result {-, view, layer, shadow, -, onClosed}(ok): the loading indicator off; ok ->
    // the popup closes with true; else the API error dialog when one is wanted ($_38 lambda(bool))
    static void updated(uint32_t self, uint32_t okp) {
        const uint32_t ok = ld8(okp);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            call(g::SceneManager_closeCurrentPopup, {sceneManager(), 1});
            close(self + 8, 1);
            return;
        }
        if (!ld8(call(g::APIManager_getInstance, {}) + API_SHOW_ERROR)) return;
        const uint32_t api = call(g::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_UpdateError, {});
        call(g::APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }

    // openPopup's result {-, ok, -, view, layer, shadow, -, onClosed}(closed by OK): closed otherwise (the back key,
    // Button_Close) -> the cancel sound, the OK button disabled and the popup closes with false ($_41)
    static void closed(uint32_t self, uint32_t byOkp) {
        if (ld8(byOkp)) return;
        call(g::AudioPlaySystemSECancel, {});
        const uint32_t ok = ld32(self + 8);
        vcall(ok, V_SET_TOUCH_ENABLED, {ok, 0});
        close(self + 0x10, 0);
    }

    static void open(uint32_t dialog, uint32_t onClosed) {
        call(fn::AudioPlaySystemSEPopUp, {});
        st32(dialog + D_EDIT_STYLE, ld32(dialog + D_STYLE));
        const uint32_t vec = dialog + D_EDIT_ZONES;
        if (ld32(vec + 4) != ld32(vec)) st32(vec + 4, ld32(vec));
        Frame f;
        {
            const uint32_t key = f.alloc(4), comp = f.alloc(4), tmp = f.alloc(4);
            const uint32_t endNode = dialog + D_ZONES + 4;
            for (uint32_t k = 1; k <= ZONES; k++) {
                st32(key, k);
                const uint32_t node = call(g::TimezoneSet_lowerBound, {ld32(dialog + D_ZONES), endNode, key, comp});
                if (node != endNode && int32_t(ld32(node + 0x10)) <= int32_t(k)) pushZone(vec, k, tmp);
            }
        }
        call(g::AudioPlaySystemSEDecide, {});
        const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
        const uint32_t shadow = SceneUtil::createShadowSprite();
        const uint32_t opacity = vcall(shadow, V_GET_OPACITY, {shadow});
        vcall(shadow, V_SET_OPACITY, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.15f), opacity})});
        vcall(layer, V_ADD_CHILD, {layer, shadow});
        const uint32_t view = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
        vcall(view, V_SET_ANCHOR, {view, VEC2_ANCHOR_MIDDLE});
        {
            const float px = bitsf(vcall(view, V_GET_X, {view}));
            const float w = ldf(vcall(view, V_GET_SIZE, {view}));
            const float mx = SceneUtil::getSideMargin();
            const float py = bitsf(vcall(view, V_GET_Y, {view}));
            const float y = py + ldf(vcall(view, V_GET_SIZE, {view}) + 4) * 0.5f;
            const float my = SceneUtil::getSideHeightMargin();
            const uint32_t pos = f.alloc(8);
            call(g::Vec2_ctor, {pos, fbits(mx + (px + w * 0.5f)), fbits(my + y)});
            vcall(view, V_SET_POSITION, {view, pos});
            call(g::Vec2_dtor, {pos});
        }
        vcall(shadow, V_ADD_CHILD, {shadow, view});

        uint32_t texts[7];
        texts[0] = findNode(view, S_TxtPlayTime, 0xC);
        for (uint32_t k = 0; k < ZONES; k++) texts[k + 1] = findNode(view, S_TxtPlayTimeX[k], 0xE);
        for (uint32_t t : texts) SceneUtil::setShadow(t);
        setTextId(texts[0], TXT_PLAY_TIME);
        for (uint32_t k = 0; k < ZONES; k++) setTextId(texts[k + 1], TXT_PLAY_TIME_A + int32_t(k));
        shadowText(findNode(view, S_TxtSelect, 0xA), TXT_SELECT);
        for (const auto &[name, len] : {std::pair{S_TxtSetting, 0xBu}, std::pair{S_TxtNumLavel, 0xDu}}) {
            const uint32_t node = findNode(view, name, len);
            if (node) vcall(node, V_SET_VISIBLE, {node, 0});
        }
        uint32_t boxes[6];
        for (uint32_t k = 0; k < ZONES; k++) boxes[k] = findNode(view, S_ButtonPlayTimeX[k], 0x11);
        const uint32_t ctx = f.alloc(0x1C);
        st32(ctx, dialog);
        for (uint32_t k = 0; k < ZONES; k++) st32(ctx + 4 + 4 * k, boxes[k]);
        for (uint32_t k = 0; k < ZONES; k++)
            addCheckListener(boxes[k], VT_ZoneTouch[k],
                             {dialog, dialog, boxes[0], boxes[1], boxes[2], boxes[3], boxes[4], boxes[5], dialog}, 0x28);
        syncZones(ctx);

        shadowText(findNode(view, S_TxtStyle, 9), TXT_STYLE);
        for (uint32_t k = 0; k < 3; k++) shadowText(findNode(view, S_TxtStyleX[k], 0xB), TXT_STYLE_X[k]);
        uint32_t styles[3];
        for (uint32_t k = 0; k < 3; k++) styles[k] = findNode(view, S_ButtonStyleX[k], 0xE);
        for (uint32_t k = 0; k < 3; k++)
            addCheckListener(styles[k], VT_StyleTouch[k], {dialog, styles[0], dialog, styles[1], styles[2]}, 0x18);
        const uint32_t style = ld32(dialog + D_EDIT_STYLE);
        call(g::CheckBox_setSelectedState, {styles[0], style == 3 ? 1u : 0u});
        call(g::CheckBox_setSelectedState, {styles[1], style == 2 ? 1u : 0u});
        call(g::CheckBox_setSelectedState, {styles[2], style == 1 ? 1u : 0u});

        // the close lambda {view, layer, shadow, -, onClosed}
        const uint32_t closer = f.alloc(0x28);
        st32(closer, view);
        st32(closer + 4, layer);
        st32(closer + 8, shadow);
        call(g::FunctionVB_copy, {closer + 0x10, onClosed});
        textShadow(findNode(view, S_TxtOK, 6), TXT_OK);
        const uint32_t ok = findNode(view, S_ButtonOk, 9);
        vcall(ok, V_SET_TOUCH_ENABLED, {ok, 1});
        {
            Frame t;
            const uint32_t copy = t.alloc(0x30);   // {ok, dialog, view, layer, shadow, -, onClosed}
            st32(copy, ok);
            st32(copy + 4, dialog);
            for (uint32_t k = 0; k < 3; k++) st32(copy + 8 + 4 * k, ld32(closer + 4 * k));
            functionCopy(copy + 0x18, closer + 0x10);
            const uint32_t fo = t.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x38});
            if (obj) {
                st32(obj, VT_OkTouch);
                st32(obj + 0xC, dialog);
                st32(obj + 8, ok);
                for (uint32_t k = 0; k < 3; k++) st32(obj + 0x10 + 4 * k, ld32(copy + 8 + 4 * k));
                call(g::FunctionVB_move, {obj + 0x20, copy + 0x18});
            }
            st32(fo + 16, obj);
            call(fn::Widget_addTouchEventListener, {ok, fo});
            functionDestroy(fo);
            functionDestroy(copy + 0x18);
        }
        textShadow(findNode(view, S_TxtCancel, 0xA), TXT_CANCEL);
        const uint32_t close = findNode(view, S_ButtonClose, 0xC);
        addTouch(close, VT_CancelTouch, {});
        // the result lambda {ok, view, layer, shadow, onClosed}
        const uint32_t result = f.alloc(0x28);
        st32(result, ok);
        for (uint32_t k = 0; k < 3; k++) st32(result + 4 + 4 * k, ld32(closer + 4 * k));
        functionCopy(result + 0x10, closer + 0x10);
        {
            const uint32_t sm = sceneManager();
            Frame t;
            const uint32_t canClose = newFunction(t, VT_CanClose, {close});
            const uint32_t copy = t.alloc(0x28);
            for (uint32_t k = 0; k < 4; k++) st32(copy + 4 * k, ld32(result + 4 * k));
            functionCopy(copy + 0x10, result + 0x10);
            const uint32_t closed = t.alloc(20);
            st32(closed + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x38});
            if (obj) {
                st32(obj, VT_Closed);
                st32(obj + 8, ld32(copy));
                for (uint32_t k = 0; k < 3; k++) st32(obj + 0x10 + 4 * k, ld32(copy + 4 + 4 * k));
                call(g::FunctionVB_move, {obj + 0x20, copy + 0x10});
                st32(obj + 8, ld32(copy));
            }
            st32(closed + 16, obj);
            call(g::SceneManager_openPopup, {sm, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(copy + 0x10);
            functionDestroy(canClose);
        }
        {
            Frame t;
            const uint32_t none = newEmptyFunction(t);
            call(fn::Node_runAction, {view, SceneUtil::createOpenPopupAnimation(view, none)});
            functionDestroy(none);
        }
        functionDestroy(result + 0x10);
        functionDestroy(closer + 0x10);
    }
};
}  // namespace

RW_FUNCTION(0x5376D0, "AvatarInfoDialog::openPopUpPlayStyle(function<void(bool)>)", AvatarInfoPlayStyle::open);
RW_FUNCTION(0x539240, "AvatarInfoDialog::openPopUpPlayStyle time boxes (sub_539240)", AvatarInfoPlayStyle::syncZones);
RW_FUNCTION(0x543260, "AvatarInfoDialog::openPopUpPlayStyle close (sub_543260)", AvatarInfoPlayStyle::close);
RW_FUNCTION(0x544C3C, "AvatarInfoDialog::openPopUpPlayStyle::$_26 operator() (time box A)", AvatarInfoPlayStyle::zoneTouch<1>);
RW_FUNCTION(0x544ADC, "AvatarInfoDialog::openPopUpPlayStyle::$_27 operator() (time box B)", AvatarInfoPlayStyle::zoneTouch<2>);
RW_FUNCTION(0x54497C, "AvatarInfoDialog::openPopUpPlayStyle::$_28 operator() (time box C)", AvatarInfoPlayStyle::zoneTouch<3>);
RW_FUNCTION(0x544818, "AvatarInfoDialog::openPopUpPlayStyle::$_29 operator() (time box D)", AvatarInfoPlayStyle::zoneTouch<4>);
RW_FUNCTION(0x5446B8, "AvatarInfoDialog::openPopUpPlayStyle::$_30 operator() (time box E)", AvatarInfoPlayStyle::zoneTouch<5>);
RW_FUNCTION(0x5441D8, "AvatarInfoDialog::openPopUpPlayStyle::$_31 operator() (time box F)", AvatarInfoPlayStyle::zoneTouch<6>);
RW_FUNCTION(0x5440B2, "AvatarInfoDialog::openPopUpPlayStyle::$_33 operator() (style A)", AvatarInfoPlayStyle::styleTouch<3>);
RW_FUNCTION(0x543FA6, "AvatarInfoDialog::openPopUpPlayStyle::$_34 operator() (style B)", AvatarInfoPlayStyle::styleTouch<2>);
RW_FUNCTION(0x543E9E, "AvatarInfoDialog::openPopUpPlayStyle::$_35 operator() (style C)", AvatarInfoPlayStyle::styleTouch<1>);
RW_FUNCTION(0x5436B8, "AvatarInfoDialog::openPopUpPlayStyle::$_38 operator() (Button_Ok)", AvatarInfoPlayStyle::okTouch);
RW_FUNCTION(0x543C88, "AvatarInfoDialog::openPopUpPlayStyle::$_38 lambda(bool) operator() (update result)", AvatarInfoPlayStyle::updated);
RW_FUNCTION(0x5431F4, "AvatarInfoDialog::openPopUpPlayStyle::$_41 operator() (popup closed)", AvatarInfoPlayStyle::closed);
