// ScenePartyRequest (Party > Find Party / Create Party): the search API call, the entry list and the generic popup
// (a cocostudio layout with OK / Cancel; OK runs the "ok" callback through the scene's +0x2E8 function, the popup's
// closing runs "closed").
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t Scene_create = 0x8A5F37, ScenePartyRequest_create = 0x657A81;
constexpr uint32_t APIManager_executePartySearchActive = 0x4CF0DD;
constexpr uint32_t function_vb_copy = 0x45C84D, function_vb_copy2 = 0x47BC69;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_copy2 = 0x47F905, function_vv_swap = 0x4EE86D;
constexpr uint32_t function_vi_swap = 0x5428E1;
constexpr uint32_t PartyScrollView_create = 0x51CB05, PartyScrollView_setEnabledTapItem = 0x51F0C1;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, EaseSineOut_create = 0x860D09;
constexpr uint32_t GOT_POPUP_FADE = 0xFD65C0;
constexpr uint32_t VT_SearchDone = 0xF6914C;               // callPartySearchAPI's callback {answer} (heap 0x20)
constexpr uint32_t VT_EntryTapped = 0xF68A6C;              // runInEntryLayout's row callback {this}
constexpr uint32_t VT_PopupClose = 0xF67F98;               // openPopUp: {Close, shadow, root, layer, ok} (heap 0x30)
constexpr uint32_t VT_PopupOk = 0xF67F40;                  // {OK, Close, this}
constexpr uint32_t VT_PopupCancel = 0xF67F14;              // {}
constexpr uint32_t VT_PopupCanClose = 0xF67EE8;            // {Close}
constexpr uint32_t VT_PopupClosed = 0xF67E90;              // {OK, Close, shadow, root, layer, closed} (heap 0x38)
constexpr uint32_t S_ScrollArea = 0xDF9B59, S_ButtonClose = 0xDF8692, S_TxtOK = 0xDF869F, S_ButtonOK = 0xDF8688;
constexpr uint32_t S_TxtCancel = 0xDF86A6;
constexpr uint32_t V_setPosition = 0x4C, V_getPosition = 0x54, V_setAnchorPoint = 0x9C, V_setContentSize = 0xA8;
constexpr uint32_t V_getContentSize = 0xAC, V_addChild = 0xF0;
// ScenePartyRequest
constexpr uint32_t F_ROOT = 0x290, F_ENTRY_VIEW = 0x2D0, F_POPUP_OK = 0x2E8;
constexpr uint32_t VIEW_ON_TAP = 0x650;

void shadowText(uint32_t root, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(root, name, len);
    setTextId(text, id);
    SceneUtil::setShadow(text);
}
// the search condition popups: a title and labelled check boxes; the current value checked (and not tappable)
constexpr uint32_t CheckBox_setSelectedState = 0x8382E7, SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t S_TxtTitle = 0xDFF142;
constexpr uint32_t S_TxtSearch[7] = {0xE05D4F, 0xE05D5C, 0xE05D69, 0xE05D76, 0xE05E05, 0xE05E12, 0xE05EF8};
constexpr uint32_t S_CheckBox[7] = {0xE05D83, 0xE05D95, 0xE05DA7, 0xE05DB9, 0xE05E1F, 0xE05E31, 0xE05F05};
constexpr uint32_t V_setVisible = 0xB0, V_setTouchEnabled = 0x29C;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;

struct SearchPopup {
    uint32_t value, pending, pendingFrom;      // the condition field; pending = it (or the other way round)
    uint32_t json, okVt, select;               // layout, OK lambda {this}, the check helper (boxes, value)
    int32_t title;
    int32_t labels[7];
    uint32_t count;
    int8_t boxFor[8];                          // value -> box index (0-based), -1: none
    uint32_t vts[7];                           // the check boxes' touch lambdas {this, boxes...}
};

void titleLabelId(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

uint32_t checkValue(uint32_t boxes, uint32_t value, const SearchPopup &p);

uint32_t openSearchPopup(uint32_t self, const SearchPopup &p) {
    st32(self + p.pending, ld32(self + p.pendingFrom));
    uint32_t root;
    {
        Frame f;
        const uint32_t ok = newFunction(f, p.okVt, {self});
        const uint32_t none = newEmptyFunction(f);
        root = call(0x6D7DBD, {self, p.json, ok, none});
        functionDestroy(none);
        functionDestroy(ok);
    }
    vcall(root, V_setVisible, {root, 1});
    titleLabelId(root, S_TxtTitle, 9, p.title);
    for (uint32_t k = 0; k < p.count; k++) titleLabelId(root, S_TxtSearch[k], 0xC, p.labels[k]);
    uint32_t box[7];
    for (uint32_t k = 0; k < p.count; k++) box[k] = findNode(root, S_CheckBox[k], 0x11);
    {
        Frame f;
        const uint32_t arr = f.alloc(4 * (p.count + 1));
        st32(arr, self);
        for (uint32_t k = 0; k < p.count; k++) st32(arr + 4 + 4 * k, box[k]);
        if (p.select) call(p.select, {arr, ld32(self + p.value)});
        else checkValue(arr, ld32(self + p.value), p);      // inlined in openPopUpJoin
    }
    const uint32_t v = ld32(self + p.value);
    if (v < 8 && p.boxFor[v] >= 0) {
        const uint32_t b = box[p.boxFor[v]];
        call(CheckBox_setSelectedState, {b, 1});
        vcall(b, V_setTouchEnabled, {b, 0});
    }
    for (uint32_t k = 0; k < p.count; k++) {
        Frame f;
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapObject(p.vts[k], 8 + 4 * p.count);
        if (obj) {
            st32(obj + 4, self);
            for (uint32_t j = 0; j < p.count; j++) st32(obj + 8 + 4 * j, box[j]);
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {box[k], fo});
        functionDestroy(fo);
    }
    return 0;
}

const SearchPopup PLAY_STYLE{0x298, 0x29C, 0x298, 0xE05D19, 0xF68A40, 0x6D8539, 102400003,
                             {102400006, 102400005, 102400004, 103800011}, 4, {3, 2, 1, 0, -1, -1, -1, -1},
                             {0xF68A14, 0xF689E8, 0xF689BC, 0xF68990}};
const SearchPopup RANK{0x2A0, 0x2A4, 0x2A0, 0xE05DCB, 0xF68964, 0x6D8EA9, 103800012,
                       {103800013, 103800014, 103800015, 103800016, 103800017, 103800011}, 6,
                       {5, 0, 1, 2, 3, 4, -1, -1}, {0xF68938, 0xF6890C, 0xF688E0, 0xF688B4, 0xF68888, 0xF6885C}};
const SearchPopup MEMBER{0x2B0, 0x2B4, 0x2B0, 0xE05E82, 0xF68780, 0x6D9B39, 103800023,
                         {103800024, 103800025, 103800026, 103800011}, 4, {3, 0, 1, 2, -1, -1, -1, -1},
                         {0xF68754, 0xF68728, 0xF686FC, 0xF686D0}};
const SearchPopup TIME_ZONE{0x2BC, 0x2B8, 0x2BC, 0xE05EBF, 0xF686A4, 0x6DA5F1, 102400001,
                            {102100013, 102100014, 102100015, 102100016, 102100017, 102100018, 103800011}, 7,
                            {-1, 0, 1, 2, 3, 4, 5, 6},
                            {0xF68678, 0xF6864C, 0xF68620, 0xF685F4, 0xF685C8, 0xF6859C, 0xF68570}};
const SearchPopup LEVEL{0x2C0, 0x2C4, 0x2C0, 0xE05F17, 0xF68544, 0x6DAE49, 103800042,
                        {103830001, 103830002, 103830003, 103830004, 103800011}, 5, {4, 0, 1, 2, 3, -1, -1, -1},
                        {0xF68518, 0xF684EC, 0xF684C0, 0xF68494, 0xF68468}};
const SearchPopup JOIN{0x2A8, 0x2AC, 0x2A8, 0xE05E43, 0xF68830, 0, 103800019, {103800020, 103800021, 103800011}, 3,
                       {2, -1, 1, 0, -1, -1, -1, -1}, {0xF68804, 0xF687D8, 0xF687AC}};
const SearchPopup LUX{0x2C8, 0x2CC, 0x2C8, 0xE05F4D, 0xF6843C, 0x6DB7DD, 103800063,
                      {103830005, 103830006, 103830007, 103830008, 103830009, 103800011}, 6,
                      {5, 4, 3, 2, 1, 0, -1, -1}, {0xF68410, 0xF683E4, 0xF683B8, 0xF6838C, 0xF68360, 0xF68334}};
// the check helper (boxes = {scene, box1..}): a new value is kept as the pending choice; every box tappable and
// unchecked, then the value's box not tappable
uint32_t checkValue(uint32_t boxes, uint32_t value, const SearchPopup &p) {
    const uint32_t pending = ld32(boxes) + p.pending;
    if (ld32(pending) == value) return pending;
    st32(pending, value);
    for (uint32_t k = 0; k < p.count; k++) {
        const uint32_t b = ld32(boxes + 4 + 4 * k);
        vcall(b, V_setTouchEnabled, {b, 1});
    }
    uint32_t r = 0;
    for (uint32_t k = 0; k < p.count; k++) r = call(CheckBox_setSelectedState, {ld32(boxes + 4 + 4 * k), 0});
    if (value < 8 && p.boxFor[value] >= 0) {
        const uint32_t b = ld32(boxes + 4 + 4 * uint32_t(p.boxFor[value]));
        r = vcall(b, V_setTouchEnabled, {b, 0});
    }
    return r;
}
}  // namespace

namespace party_request {

uint32_t openPopUpJoin(uint32_t self) { return openSearchPopup(self, JOIN); }
RW_FUNCTION(0x6D8F9C, "ScenePartyRequest::openPopUpJoin", party_request::openPopUpJoin);

// the approval check boxes: "anyone" (0), "approval" (2), "invitation" (3)
uint32_t joinTouched(uint32_t self, uint32_t typeRef, uint32_t value) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    call(AudioPlaySystemSEDecide, {});
    return checkValue(self + 4, value, JOIN);
}
uint32_t joinAnyone(uint32_t self, uint32_t, uint32_t typeRef) { return joinTouched(self, typeRef, 0); }
RW_FUNCTION(0x6E169A, "ScenePartyRequest::openPopUpJoin box 3 operator()", party_request::joinAnyone);
uint32_t joinApproval(uint32_t self, uint32_t, uint32_t typeRef) { return joinTouched(self, typeRef, 2); }
RW_FUNCTION(0x6E17BE, "ScenePartyRequest::openPopUpJoin box 2 operator()", party_request::joinApproval);
uint32_t joinInvitation(uint32_t self, uint32_t, uint32_t typeRef) { return joinTouched(self, typeRef, 3); }
RW_FUNCTION(0x6E18DE, "ScenePartyRequest::openPopUpJoin box 1 operator()", party_request::joinInvitation);

// openPopUp's pieces: the close animation of {shadow, root, layer} (sub_6DD70C) and of {OK, Close, shadow, root,
// layer} with the buttons off (sub_6DE004), the scene's OK function, the popup closed with "no"
uint32_t closeAnimation(uint32_t captures) {
    const uint32_t shadow = ld32(captures);
    call(Node_runAction, {shadow, call(FadeTo_create, {0x3E19999A, 0})});
    const uint32_t root = ld32(captures + 4);
    Frame f;
    const uint32_t fo = newFunction(f, 0xF67EBC, {ld32(captures + 8), ld32(captures), root});
    const uint32_t close = call(0x45BC61, {root, fo});
    call(Node_runAction, {root, call(EaseSineOut_create, {close})});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6DD70C, "ScenePartyRequest::openPopUp close animation (sub_6DD70C)", party_request::closeAnimation);

uint32_t closeButtonsAndAnimate(uint32_t captures) {
    vcall(ld32(captures), V_setTouchEnabled, {ld32(captures), 0});
    vcall(ld32(captures + 4), V_setTouchEnabled, {ld32(captures + 4), 0});
    const uint32_t shadow = ld32(captures + 8);
    call(Node_runAction, {shadow, call(FadeTo_create, {0x3E19999A, 0})});
    const uint32_t root = ld32(captures + 0xC);
    Frame f;
    const uint32_t fo = newFunction(f, 0xF67FF0, {ld32(captures + 0x10), ld32(captures + 8), root});
    const uint32_t close = call(0x45BC61, {root, fo});
    call(Node_runAction, {root, call(EaseSineOut_create, {close})});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6DE004, "ScenePartyRequest::openPopUp close with the buttons off (sub_6DE004)", party_request::closeButtonsAndAnimate);

// the scene's OK function {Close, shadow, root, layer, ok}: the popup closed, then ok
uint32_t popupOk(uint32_t self) {
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 1});
    call(0x6216A1, {sceneManager(), 1});
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
    call(0x6DD70D, {self + 0xC});
    if (!ld32(self + 0x28)) return 0;
    return call(0x470E69, {self + 0x18});
}
RW_FUNCTION(0x6DDD9C, "ScenePartyRequest::openPopUp OK function operator()", party_request::popupOk);

// closed with "no" {OK, Close, shadow, root, layer, closed}: the buttons off, the close animation, closed
uint32_t popupClosed(uint32_t self, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    if (ok) return ok;
    call(0x5C0D81, {});
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
    vcall(ld32(self + 0xC), V_setTouchEnabled, {ld32(self + 0xC), 0});
    call(0x6DD70D, {self + 0x10});
    if (!ld32(self + 0x30)) return 0;
    return call(0x470E69, {self + 0x20});
}
RW_FUNCTION(0x6DD688, "ScenePartyRequest::openPopUp closed operator()", party_request::popupClosed);

// OK button {OK, Close, scene}: the buttons off, no taps for a moment, then the scene's OK function
uint32_t okTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t scene = ld32(self + 0xC);
    call(AudioPlaySystemSEDecide, {});
    vcall(ld32(self + 4), V_setTouchEnabled, {ld32(self + 4), 0});
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
    const uint32_t sm = sceneManager();
    call(0x61FB25, {sm, ld32(sceneManager() + 0x300)});
    const uint32_t delay = call(0x86852D, {ld32(sceneManager() + 0x300)});
    Frame f;
    const uint32_t fo = newFunction(f, 0xF67F6C, {scene});
    const uint32_t then = call(0x866B39, {fo});
    call(Node_runAction, {scene, call(0x867999, {delay, then, 0})});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6DDA4C, "ScenePartyRequest::openPopUp OK operator()", party_request::okTouched);

uint32_t checkPlayStyle(uint32_t boxes, uint32_t value) { return checkValue(boxes, value, PLAY_STYLE); }
RW_FUNCTION(0x6D8538, "ScenePartyRequest::openPopUpPlayStyle check (sub_6D8538)", party_request::checkPlayStyle);
uint32_t checkRank(uint32_t boxes, uint32_t value) { return checkValue(boxes, value, RANK); }
RW_FUNCTION(0x6D8EA8, "ScenePartyRequest::openPopUpRank check (sub_6D8EA8)", party_request::checkRank);
uint32_t checkMember(uint32_t boxes, uint32_t value) { return checkValue(boxes, value, MEMBER); }
RW_FUNCTION(0x6D9B38, "ScenePartyRequest::openPopUpMember check (sub_6D9B38)", party_request::checkMember);
uint32_t checkTimeZone(uint32_t boxes, uint32_t value) { return checkValue(boxes, value, TIME_ZONE); }
RW_FUNCTION(0x6DA5F0, "ScenePartyRequest::openPopUpTimeZone check (sub_6DA5F0)", party_request::checkTimeZone);
uint32_t checkLevel(uint32_t boxes, uint32_t value) { return checkValue(boxes, value, LEVEL); }
RW_FUNCTION(0x6DAE48, "ScenePartyRequest::openPopUpLevel check (sub_6DAE48)", party_request::checkLevel);
uint32_t checkLux(uint32_t boxes, uint32_t value) { return checkValue(boxes, value, LUX); }
RW_FUNCTION(0x6DB7DC, "ScenePartyRequest::openPopUpLux check (sub_6DB7DC)", party_request::checkLux);

uint32_t openPopUpPlayStyle(uint32_t self) { return openSearchPopup(self, PLAY_STYLE); }
RW_FUNCTION(0x6D7774, "ScenePartyRequest::openPopUpPlayStyle", party_request::openPopUpPlayStyle);
uint32_t openPopUpRank(uint32_t self) { return openSearchPopup(self, RANK); }
RW_FUNCTION(0x6D85EC, "ScenePartyRequest::openPopUpRank", party_request::openPopUpRank);
uint32_t openPopUpMember(uint32_t self) { return openSearchPopup(self, MEMBER); }
RW_FUNCTION(0x6D94EC, "ScenePartyRequest::openPopUpMember", party_request::openPopUpMember);
uint32_t openPopUpTimeZone(uint32_t self) { return openSearchPopup(self, TIME_ZONE); }
RW_FUNCTION(0x6D9BF0, "ScenePartyRequest::openPopUpTimeZone", party_request::openPopUpTimeZone);
uint32_t openPopUpLevel(uint32_t self) { return openSearchPopup(self, LEVEL); }
RW_FUNCTION(0x6DA704, "ScenePartyRequest::openPopUpLevel", party_request::openPopUpLevel);
uint32_t openPopUpLux(uint32_t self) { return openSearchPopup(self, LUX); }
RW_FUNCTION(0x6DAF20, "ScenePartyRequest::openPopUpLux", party_request::openPopUpLux);

uint32_t callPartySearchAPI(uint32_t frequency, uint32_t rank, uint32_t agree, uint32_t scale, uint32_t done) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, done});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(VT_SearchDone, 0x20);
    if (obj) call(function_vb_copy2, {obj + 8, copy});
    st32(fo + 16, obj);
    call(APIManager_executePartySearchActive, {api, frequency, rank, agree, scale, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x6D5F0C, "ScenePartyRequest::callPartySearchAPI", party_request::callPartySearchAPI);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(ScenePartyRequest_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x6D602C, "ScenePartyRequest::createScene", party_request::createScene);

// the party list of the entry layout (one column, rows not tappable; a row callback)
uint32_t runInEntryLayout(uint32_t self) {
    const uint32_t area = findNode(ld32(self + F_ROOT), S_ScrollArea, 0xB);
    st32(self + F_ENTRY_VIEW, call(PartyScrollView_create, {0}));
    call(IScrollProtocol_setMaxColumn, {ld32(self + F_ENTRY_VIEW), 1});
    call(PartyScrollView_setEnabledTapItem, {ld32(self + F_ENTRY_VIEW), 0});
    {
        const uint32_t view = ld32(self + F_ENTRY_VIEW);
        vcall(view, V_setContentSize, {view, vcall(area, V_getContentSize, {area})});
    }
    {
        const uint32_t view = ld32(self + F_ENTRY_VIEW);
        Frame f;
        const uint32_t fo = newFunction(f, VT_EntryTapped, {self});
        call(function_vi_swap, {fo, view + VIEW_ON_TAP});
        functionDestroy(fo);
    }
    vcall(area, V_addChild, {area, ld32(self + F_ENTRY_VIEW)});
    return 0;
}
RW_FUNCTION(0x6D7450, "ScenePartyRequest::runInEntryLayout", party_request::runInEntryLayout);

// the generic popup: `json` on a shadow (popup layer), OK / Cancel; returns the popup
uint32_t openPopUp(uint32_t self, uint32_t json, uint32_t ok, uint32_t closed) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x20C});
    const uint32_t opacity = vcall(shadow, 0x20C, {shadow});
    vcall(shadow, 0x214, {shadow, 0});
    call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), opacity})});
    vcall(layer, V_addChild, {layer, shadow});
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    vcall(shadow, V_addChild, {shadow, root});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(root, V_setAnchorPoint, {root, anchor});
        call(Vec2_dtor, {anchor});
    }
    vcall(root, V_setPosition, {root, vcall(shadow, V_getPosition, {shadow})});
    const uint32_t close = findNode(root, S_ButtonClose, 0xC);
    Frame f;
    {
        // the scene's OK function: {Close, shadow, root, layer, ok}
        const uint32_t okCopy = f.alloc(20);
        call(function_vv_copy, {okCopy, ok});
        const uint32_t okCopy2 = f.alloc(20);
        call(function_vv_copy2, {okCopy2, okCopy});
        Frame g;
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(VT_PopupClose, 0x30);
        if (obj) {
            st32(obj + 8, close);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, root);
            st32(obj + 0x14, layer);
            call(function_vv_copy2, {obj + 0x18, okCopy2});
        }
        st32(fo + 16, obj);
        call(function_vv_swap, {fo, self + F_POPUP_OK});
        functionDestroy(fo);
        functionDestroy(okCopy2);
        functionDestroy(okCopy);
    }
    shadowText(root, S_TxtOK, 6, 100100001);
    const uint32_t okButton = findNode(root, S_ButtonOK, 9);
    addTouch(okButton, VT_PopupOk, {okButton, close, self});
    shadowText(root, S_TxtCancel, 0xA, 100100002);
    addTouch(close, VT_PopupCancel, {});
    {
        const uint32_t closedCopy = f.alloc(20);
        call(function_vv_copy, {closedCopy, closed});
        const uint32_t sm = sceneManager();
        Frame g;
        const uint32_t canClose = newFunction(g, VT_PopupCanClose, {close});
        const uint32_t copy = g.alloc(20);
        functionCopy(copy, closedCopy);
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(VT_PopupClosed, 0x38);
        if (obj) {
            const uint32_t words[5] = {okButton, close, shadow, root, layer};
            for (uint32_t k = 0; k < 5; k++) st32(obj + 8 + 4 * k, words[k]);
            call(function_vv_copy2, {obj + 0x20, copy});
        }
        st32(fo + 16, obj);
        call(fn::SceneManager_openPopup, {sm, canClose, fo, 1});
        functionDestroy(fo);
        functionDestroy(copy);
        functionDestroy(canClose);
        Frame h;
        const uint32_t none = newEmptyFunction(h);
        const uint32_t open = call(SceneUtil_createOpenPopupAnimation, {root, none});
        call(Node_runAction, {root, call(EaseSineOut_create, {open})});
        functionDestroy(none);
        functionDestroy(closedCopy);
    }
    return root;
}
RW_FUNCTION(0x6D7DBC, "ScenePartyRequest::openPopUp", party_request::openPopUp);

// the network retry from the party top: the retry callback cleared, the search with no conditions, then the scene
uint32_t retryToRequest(uint32_t) {
    const uint32_t sm = sceneManager();
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        st8(sm + 0x4CC, 1);
        call(0x490109, {sm + 0x548, none});
        functionDestroy(none);
    }
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        callPartySearchAPI(0, 0, 0, 0, none);
        functionDestroy(none);
    }
    smain::changeSceneLayer(ld32(0xFD68D0));
    return 0;
}
RW_FUNCTION(0x657910, "SceneParty: to ScenePartyRequest lambda operator() (sub_657910)", party_request::retryToRequest);

// a back button {scene}: the scene's out animation
uint32_t backTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t scene = ld32(self + 4);
    call(AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t fo = newFunction(f, 0xF6215C, {});
    call(0x6F24B5, {scene + 0x288, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x657BD8, "SceneParty back button operator() (sub_657BD8)", party_request::backTouched);

// the comment edit box: its text copied to the preview label (+0x30C), then the usual end handling
uint32_t editBoxEditingDidEnd(uint32_t self, uint32_t box) {
    if (ld32(self + 0x30C)) {
        Frame f;
        const uint32_t text = call(0x85520D, {box});
        const uint32_t s1 = newStr(f, text);
        const uint32_t label = ld32(self + 0x30C);
        const uint32_t c = strIsLong(s1) ? ld32(s1 + 8) : s1 + 1;
        const uint32_t s2 = newStr(f, c);
        call(0x83A2DD, {label, s2});
        strFree(s2);
        strFree(s1);
    }
    const uint32_t label = ld32(self + 0x304);
    call(0x45C61D, {self + 0x300, label, call(0x85520D, {box})});
    return 0;
}
RW_FUNCTION(0x6DD154, "ScenePartyRequest::editBoxEditingDidEnd", party_request::editBoxEditingDidEnd);

// the party name was rejected: a message, the scene manager's flag +0x309 set
uint32_t nameNgDialog(uint32_t) {
    st8(sceneManager() + 0x309, 1);
    Frame f;
    const uint32_t td = textData(f, 103810020);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t fo = newFunction(f, 0xF68124, {});
    call(0x45C775, {s, fo, 0});
    functionDestroy(fo);
    strFree(s);
    sharedRelease(td);
    return 0;
}
RW_FUNCTION(0x6DE8F4, "ScenePartyRequest name NG dialog (sub_6DE8F4)", party_request::nameNgDialog);

// Cancel {scene, OK, Close}: the buttons off, no taps for a moment, then the scene's cancel
uint32_t cancelTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t scene = ld32(self + 4);
    call(AudioPlaySystemSEDecide, {});
    st8(scene + 0x308, 1);
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
    vcall(ld32(self + 0xC), V_setTouchEnabled, {ld32(self + 0xC), 0});
    const uint32_t sm = sceneManager();
    call(0x61FB25, {sm, ld32(sceneManager() + 0x300)});
    const uint32_t delay = call(0x86852D, {ld32(sceneManager() + 0x300)});
    Frame f;
    const uint32_t fo = newFunction(f, 0xF68258, {scene});
    const uint32_t then = call(0x866B39, {fo});
    call(Node_runAction, {scene, call(0x867999, {delay, then, 0})});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6DF0E4, "ScenePartyRequest create popup Cancel operator() (sub_6DF0E4)", party_request::cancelTouched);

// a condition popup's OK {scene}: the pending choice kept, its row button selected and labelled (unset: the row's
// title, not selected)
struct ConditionRow {
    uint32_t value, pending, button, label;
    uint32_t first, count;                     // values first..first+count-1 are set
    int32_t ids[7];
    int32_t none;
};
uint32_t applyCondition(uint32_t self, const ConditionRow &c) {
    const uint32_t scene = ld32(self + 4);
    st32(scene + c.value, ld32(scene + c.pending));
    const uint32_t button = findNode(ld32(scene + F_ROOT), c.button, 0x14);
    const uint32_t label = findNode(ld32(scene + F_ROOT), c.label, 0x12);
    const uint32_t v = ld32(scene + c.value);
    const bool set = v >= c.first && v < c.first + c.count;
    call(0x5318FD, {button, set ? 1u : 0u});
    setTextId(label, set ? c.ids[v - c.first] : c.none);
    return 0;
}
constexpr uint32_t S_Button3 = 0xE05FE9, S_Label3 = 0xE05FFE, S_Button4 = 0xE05FC1, S_Label4 = 0xE05FD6;
constexpr uint32_t S_Button2 = 0xE06011, S_Label2 = 0xE06026;
const ConditionRow ROW_LUX{0x2C8, 0x2CC, S_Button4, S_Label4, 1, 5,
                           {103830009, 103830008, 103830007, 103830006, 103830005}, 103800043};
const ConditionRow ROW_LEVEL{0x2C0, 0x2C4, S_Button3, S_Label3, 1, 4,
                             {103830001, 103830002, 103830003, 103830004}, 103800042};
// time zone 7 ("any") is not selected; no other value reaches it
const ConditionRow ROW_TIME_ZONE{0x2BC, 0x2B8, S_Button2, S_Label2, 1, 6,
                                 {102100013, 102100014, 102100015, 102100016, 102100017, 102100018}, 102400001};
const ConditionRow ROW_MEMBER{0x2B0, 0x2B4, S_Button4, S_Label4, 1, 3, {103800024, 103800025, 103800026}, 103800023};
const ConditionRow ROW_JOIN{0x2A8, 0x2AC, S_Button3, S_Label3, 2, 2, {103800021, 103800020}, 103800019};

uint32_t applyLux(uint32_t self) { return applyCondition(self, ROW_LUX); }
RW_FUNCTION(0x6DFF78, "ScenePartyRequest::openPopUpLux OK (sub_6DFF78)", party_request::applyLux);
uint32_t applyLevel(uint32_t self) { return applyCondition(self, ROW_LEVEL); }
RW_FUNCTION(0x6E0608, "ScenePartyRequest::openPopUpLevel OK (sub_6E0608)", party_request::applyLevel);
uint32_t applyTimeZone(uint32_t self) { return applyCondition(self, ROW_TIME_ZONE); }
RW_FUNCTION(0x6E0E44, "ScenePartyRequest::openPopUpTimeZone OK (sub_6E0E44)", party_request::applyTimeZone);
uint32_t applyMember(uint32_t self) { return applyCondition(self, ROW_MEMBER); }
RW_FUNCTION(0x6E1408, "ScenePartyRequest::openPopUpMember OK (sub_6E1408)", party_request::applyMember);
uint32_t applyJoin(uint32_t self) { return applyCondition(self, ROW_JOIN); }
RW_FUNCTION(0x6E19EC, "ScenePartyRequest::openPopUpJoin OK (sub_6E19EC)", party_request::applyJoin);

// the player ID search popup's OK {edit box, scene, OK, Close, ID button, shadow, root, layer}: a valid number opens
// the player's info; otherwise a message and the buttons back on
void idSearchError(int32_t id, uint32_t vt) {
    st8(sceneManager() + 0x309, 1);
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t fo = newFunction(f, vt, {});
    call(0x45C775, {s, fo, 0});
    functionDestroy(fo);
    strFree(s);
    sharedRelease(td);
}
uint32_t idSearchOk(uint32_t self) {
    const uint32_t scene = ld32(self + 8);
    const uint32_t box = ld32(self + 4);
    const uint32_t text = vcall(box, 0x2AC, {box});
    Frame f;
    const uint32_t id = f.alloc(12);
    if (strIsLong(text)) strInit(id, ld32(text + 8), ld32(text + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(id + k, ld32(text + k));
    if (!call(0x47898D, {id})) idSearchError(103830011, 0xF68308);
    else if (!call(0x47CBD5, {id, 0xDF8A1A})) idSearchError(103830011, 0xF682DC);
    else {
        const uint32_t end = f.alloc(4);
        const uint64_t v = rw::call64(0x413F90, {strData(id), end, 10});
        if (!v || ld8(ld32(end))) idSearchError(103830021, 0xF682B0);
        else {
            vcall(ld32(self + 0x14), V_setTouchEnabled, {ld32(self + 0x14), 1});
            call(0x6216A1, {sceneManager(), 1});
            call(0x6DEDA5, {self + 0x18});
            Frame g;
            const uint32_t a = newEmptyFunction(g);
            const uint32_t b = newEmptyFunction(g);
            call(0x46A229, {uint32_t(v), uint32_t(v >> 32), 1, b, a});
            functionDestroy(a);
            functionDestroy(b);
        }
    }
    st8(scene + 0x308, 0);
    vcall(ld32(self + 0xC), V_setTouchEnabled, {ld32(self + 0xC), 1});
    vcall(ld32(self + 0x10), V_setTouchEnabled, {ld32(self + 0x10), 1});
    strFree(id);
    return 0;
}
RW_FUNCTION(0x6DF358, "ScenePartyRequest player ID search OK (sub_6DF358)", party_request::idSearchOk);

// no taps for a moment, then lambda `vt` {scene} on the scene
void delayedOnScene(uint32_t scene, uint32_t vt) {
    const uint32_t sm = sceneManager();
    call(0x61FB25, {sm, ld32(sceneManager() + 0x300)});
    const uint32_t delay = call(0x86852D, {ld32(sceneManager() + 0x300)});
    Frame f;
    const uint32_t fo = newFunction(f, vt, {scene});
    const uint32_t then = call(0x866B39, {fo});
    call(Node_runAction, {scene, call(0x867999, {delay, then, 0})});
    functionDestroy(fo);
}
// the user's level (word 0 of the user data SafeBuffer) is 13 or more
bool levelAtLeast13(uint32_t sb) {
    Frame f;
    const uint32_t v = f.alloc(4);
    call(0x45A269, {sb, v, ld32(sb + 0xC), 4, 0});
    return int32_t(ld32(v)) >= 13;
}

// the create popup's OK {comment box, user data, -, scene, OK, Close}: from level 13 the comment is checked (empty:
// a message after a moment; only spaces / bad: a message) and kept at +0x2D8; then the buttons off and the
// scene's create step after a moment
uint32_t createOkTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t scene = ld32(self + 0x10);
    call(AudioPlaySystemSEDecide, {});
    if (ld32(self + 4)) {
        if (!ld32(self + 8)) goto off;
        if (levelAtLeast13(ld32(self + 8))) {
            const uint32_t box = ld32(self + 4);
            const uint32_t text = vcall(box, 0x2AC, {box});
            if (!strSize(text)) {
                delayedOnScene(scene, 0xF680F8);
                return 0;
            }
            if (!call(0x47898D, {text})) {
                idSearchError(103810020, 0xF680CC);
                return 0;
            }
        }
    }
    if (ld32(self + 8) && levelAtLeast13(ld32(self + 8))) {
        const uint32_t box = ld32(self + 4);
        call(0x498A1D, {scene + 0x2D8, vcall(box, 0x2AC, {box})});
    }
off:
    st8(scene + 0x308, 1);
    vcall(ld32(self + 0x14), V_setTouchEnabled, {ld32(self + 0x14), 0});
    vcall(ld32(self + 0x18), V_setTouchEnabled, {ld32(self + 0x18), 0});
    delayedOnScene(scene, 0xF680A0);
    return 0;
}
RW_FUNCTION(0x6DE418, "ScenePartyRequest create popup OK operator() (sub_6DE418)", party_request::createOkTouched);

// the join request popup (Find Party > a party > Request): the general layout, or from level 13 the one with a
// comment edit box (its text kept at +0x2D8); OK / Cancel as in openPopUp. Returns 0x77C (left in r0).
uint32_t openPopUpRequest(uint32_t self, uint32_t ok, uint32_t closed) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x83});
    const uint32_t opacity = vcall(shadow, 0x20C, {shadow});
    vcall(shadow, 0x214, {shadow, 0});
    call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), opacity})});
    vcall(layer, V_addChild, {layer, shadow});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + 0x1D8), sb = ld32(api + 0x1D4);
    if (ctrl) call(fn::shared_add, {ctrl});
    auto level = [&] {
        Frame g;
        const uint32_t v = g.alloc(4);
        call(0x45A269, {sb, v, ld32(sb + 0xC), 4, 0});
        return int32_t(ld32(v));
    };
    Frame f;
    const uint32_t json = f.alloc(12);
    for (uint32_t k = 0; k < 12; k += 4) st32(json + k, 0);
    if (sb && level() > 12) call(0x48D811, {json, 0xDFE611, 0x35});
    else call(0x48D811, {json, 0xDFE5DF, 0x31});
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), strData(json)});
    vcall(shadow, V_addChild, {shadow, root});
    {
        Frame g;
        const uint32_t anchor = g.alloc(8);
        call(Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(root, V_setAnchorPoint, {root, anchor});
        call(Vec2_dtor, {anchor});
    }
    vcall(root, V_setPosition, {root, vcall(shadow, V_getPosition, {shadow})});
    const uint32_t twoLine = findNode(root, 0xDFE647, 0xB);
    SceneUtil::setShadow(twoLine);
    const uint32_t comment = findNode(root, 0xDF9EF7, 0xB);
    SceneUtil::setShadow(comment);
    const uint32_t field = findNode(root, 0xDF9F0D, 0x11);
    const uint32_t limit = findNode(root, 0xDFE418, 0xA);
    if (limit && level() >= 13) {
        // "up to %d characters" (misc 808)
        SceneUtil::setShadow(limit);
        Frame g;
        const uint32_t td = textData(g, 100100037);
        const uint32_t misc = g.alloc(8);
        call(0x786D5D, {misc, 808});
        const uint32_t n = g.alloc(4);
        call(0x45A269, {ld32(misc), n, ld32(ld32(misc) + 0xC) + 4, 4, 4});
        const uint32_t buf = g.alloc(0x404);
        call(0x49140D, {buf, textDataStr(td), ld32(n)});
        setTextC(limit, buf);
        sharedRelease(misc);
        sharedRelease(td);
    }
    const bool approval = ld8(self + 0x294) != 0;
    setTextId(twoLine, approval ? 103830013 : 103810002);
    setTextId(comment, 103800044);
    if (sb && level() <= 12) {
        const uint32_t label = findNode(root, 0xDFE1CC, 0x11);
        setTextId(label, approval ? 103830014 : 103810025);
        call(0x498A1D, {self + 0x2D8, call(0x83A33B, {label})});
    }
    st8(self + 0x308, 0);
    const uint32_t extra = f.alloc(4);
    st32(extra, 0);
    uint32_t editBox = 0;
    if (field) {
        Frame g;
        const uint32_t text = g.alloc(12);
        for (uint32_t k = 0; k < 12; k += 4) st32(text + k, 0);
        {
            Frame h;
            const uint32_t td = textData(h, approval ? 103830014 : 103810025);
            call(0x48D811, {text, textDataStr(td), host_strlen(textDataStr(td))});
            sharedRelease(td);
        }
        st32(self + 0x30C, call(0x45C465, {field, strData(text), 0xDF9F1F}));
        const uint32_t holder = g.alloc(12);
        call(ld32(ld32(field) + 0x150), {holder, field});
        const uint32_t size = g.alloc(8);
        call(0x8DF027, {size, fbits(400.0f), fbits(136.0f)});
        const uint32_t misc = g.alloc(8);
        call(0x786D5D, {misc, 808});
        const uint32_t n = g.alloc(4);
        call(0x45A269, {ld32(misc), n, ld32(ld32(misc) + 0xC) + 4, 4, 4});
        editBox = call(0x45C0A1, {self + 0x300, self + 0x304, field, holder, strData(text), 0xDF9F1F, size,
                                  self + 0x288, self + 0x308, extra, ld32(n)});
        sharedRelease(misc);
        strFree(holder);
        vcall(editBox, V_setVisible, {editBox, 0});
        strFree(text);
        if (ld32(extra) && sb && level() <= 12) vcall(ld32(extra), 0x128, {ld32(extra), 1});
    }
    const uint32_t okButton = findNode(root, S_ButtonOK, 9);
    const uint32_t close = findNode(root, S_ButtonClose, 0xC);
    {
        // the scene's OK function: {Close, OK, Close, shadow, root, layer, ok}
        const uint32_t okCopy = f.alloc(20);
        call(function_vv_copy, {okCopy, ok});
        const uint32_t okCopy2 = f.alloc(20);
        call(function_vv_copy2, {okCopy2, okCopy});
        Frame g;
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(0xF68150, 0x38);
        if (obj) {
            const uint32_t words[6] = {close, okButton, close, shadow, root, layer};
            for (uint32_t k = 0; k < 6; k++) st32(obj + 8 + 4 * k, words[k]);
            call(function_vv_copy2, {obj + 0x20, okCopy2});
        }
        st32(fo + 16, obj);
        call(function_vv_swap, {fo, self + F_POPUP_OK});
        functionDestroy(fo);
        functionDestroy(okCopy2);
        functionDestroy(okCopy);
    }
    shadowText(root, S_TxtOK, 6, 100100001);
    {
        if (ctrl) call(fn::shared_add, {ctrl});
        Frame g;
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(0xF68074, 0x1C);
        if (obj) {
            const uint32_t words[6] = {editBox, sb, ctrl, self, okButton, close};
            for (uint32_t k = 0; k < 6; k++) st32(obj + 4 + 4 * k, words[k]);
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {okButton, fo});
        functionDestroy(fo);
        if (!obj && ctrl) call(fn::shared_release, {ctrl});
    }
    shadowText(root, S_TxtCancel, 0xA, 100100002);
    addTouch(close, 0xF68048, {});
    {
        const uint32_t closedCopy = f.alloc(20);
        call(function_vv_copy, {closedCopy, closed});
        const uint32_t sm = sceneManager();
        Frame g;
        const uint32_t canClose = newFunction(g, 0xF6801C, {close});
        const uint32_t copy = g.alloc(20);
        functionCopy(copy, closedCopy);
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = heapObject(0xF67FC4, 0x38);
        if (obj) {
            const uint32_t words[6] = {self, okButton, close, shadow, root, layer};
            for (uint32_t k = 0; k < 6; k++) st32(obj + 8 + 4 * k, words[k]);
            call(function_vv_copy2, {obj + 0x20, copy});
        }
        st32(fo + 16, obj);
        call(fn::SceneManager_openPopup, {sm, canClose, fo, 1});
        functionDestroy(fo);
        functionDestroy(copy);
        functionDestroy(canClose);
        Frame h;
        const uint32_t none = newEmptyFunction(h);
        const uint32_t open = call(SceneUtil_createOpenPopupAnimation, {root, none});
        call(Node_runAction, {root, call(EaseSineOut_create, {open})});
        functionDestroy(none);
        functionDestroy(closedCopy);
    }
    strFree(json);
    if (ctrl) call(fn::shared_release, {ctrl});
    return 0x77C;
}
RW_FUNCTION(0x6DC0D0, "ScenePartyRequest::openPopUpRequest", party_request::openPopUpRequest);

}  // namespace party_request
