// PartyInfoDialog lambdas whose std::function thunks the owner heuristic attributed to RaidBossLwfDialog (they sit
// right before RaidBossLwfDialog in the library): the lambda bodies (operator()) of PartyInfoDialog::open $_0,
// createLayout $_1..$_4, openRequest $_5 (+ its inner lambdas) and createRequestLayout $_6 / $_7, and their
// __func thunks (clone / destroy / target / target_type), generated from the lifted code like
// scene_social/thunks/PartyInfoDialog_thunks.cpp (which holds the dialog's other lambdas).
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

constexpr uint32_t PartyInfoDialog_close = 0x596081;
constexpr uint32_t PartyInfoDialog_createLayout = 0x592CA1;          // (dialog, bool)
constexpr uint32_t PartyInfoDialog_createRequestLayout = 0x593C49;   // (dialog, bool)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (manager, bool)
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;        // (manager, bool)
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;             // (api, const function<void(bool)>&, bool)
constexpr uint32_t SceneUtil_createFadeOutAnimation = 0x45BCE5;      // (node, std::function<void()> by value)
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t Widget_isTouchEnabled = 0x826665;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_runAction = 0x894AED;

constexpr uint32_t APIMANAGER_ERROR = 20;            // uint8: the last request failed with an error to show
constexpr uint32_t DIALOG_SHADOW = 628;              // PartyInfoDialog: the shadow node it was opened on
constexpr uint32_t DIALOG_ROOT = 632;                // PartyInfoDialog: the dialog's root node
constexpr uint32_t NODE_REMOVE_CHILD = 300;          // cocos2d::Node::removeChild(Node*, bool) (byte offset)
constexpr uint32_t WIDGET_SET_TOUCH_ENABLED = 668;   // ui::Widget::setTouchEnabled(bool) (byte offset)

// __func vtables (+8) of the lambdas built inside the bodies
constexpr uint32_t VT_openRequest5_bool = 0xF5AEDC;  // openRequest $_5 {lambda(bool)} (error dialog closed)
constexpr uint32_t VT_openRequest5_void = 0xF5AEB0;  // openRequest $_5 {lambda()} (fade out finished)
constexpr uint32_t VT_open0_bool = 0xF5AFE4;         // open $_0 {lambda(bool)}

// The request's error, if any, as the error dialog (with an empty callback lambda); then whether to retry.
bool showErrorAndCheckRetry(uint32_t errorLambdaVtable) {
    uint32_t api = call(sfn::APIManager_getInstance, {});
    if (ld8(api + APIMANAGER_ERROR)) {
        uint32_t a = call(sfn::APIManager_getInstance, {});
        withLambda(errorLambdaVtable, {}, [&](Addr fn) { call(APIManager_openErrorDialog, {a, fn, 1}); });
    }
    return call(APIManager_needsRetry, {call(sfn::APIManager_getInstance, {})}) != 0;
}
}  // namespace

namespace PartyInfoDialogLambdas {

// createRequestLayout $_7 (touch; captures two widgets): on touch end close the popup and stop both widgets taking touches
void requestCloseTouch(uint32_t self, uint32_t, uint32_t typeRef) {
    if (rw::load<int32_t>(typeRef) != 2) return;
    call(SceneManager_closeCurrentPopup, {call(sfn::SceneManager_getInstance, {}), 0});
    uint32_t a = ld32(self + 4);
    vcall(a, WIDGET_SET_TOUCH_ENABLED, {a, 0});
    uint32_t b = ld32(self + 8);
    vcall(b, WIDGET_SET_TOUCH_ENABLED, {b, 0});
}

// lambdas with an empty body (createRequestLayout $_6, openRequest $_5 {lambda(bool)}, createLayout $_1,
// open $_0 {lambda(bool)})
void nothing(uint32_t) {}

// openRequest $_5 (the request finished; captures the dialog): success rebuilds the request layout, an error shows
// the error dialog, and without a retry the dialog fades out
void openRequestDone(uint32_t self, uint32_t okRef) {
    uint8_t ok = ld8(okRef);
    uint32_t dialog = ld32(self + 4);
    call(SceneManager_enableLoadingIndicator, {call(sfn::SceneManager_getInstance, {}), 0});
    if (ok) {
        call(PartyInfoDialog_createRequestLayout, {dialog, 0});
        return;
    }
    if (showErrorAndCheckRetry(VT_openRequest5_bool)) return;
    uint32_t root = ld32(dialog + DIALOG_ROOT);
    withLambda(VT_openRequest5_void, {dialog}, [&](Addr fn) {
        uint32_t action = call(SceneUtil_createFadeOutAnimation, {root, fn});
        call(Node_runAction, {root, action});
    });
}

// openRequest $_5 {lambda()} (the fade out finished): take the dialog off its shadow node
void openRequestFadedOut(uint32_t self) {
    uint32_t dialog = ld32(self + 4);
    if (!ld32(dialog + DIALOG_SHADOW)) return;
    call(Ref_retain, {dialog});
    call(Ref_autorelease, {dialog});
    uint32_t root = ld32(dialog + DIALOG_ROOT);
    uint32_t shadow = ld32(dialog + DIALOG_SHADOW);
    vcall(shadow, NODE_REMOVE_CHILD, {shadow, root, 1});
    rw::save<uint32_t>(dialog + DIALOG_SHADOW, 0);
}

// createLayout $_4 (back key; captures the dialog): cancel sound, refresh the header, close
void layoutBack(uint32_t self) {
    call(AudioPlaySystemSECancel, {});
    uint32_t dialog = ld32(self + 4);
    call(SceneManager_updateHeader, {call(sfn::SceneManager_getInstance, {})});
    call(PartyInfoDialog_close, {dialog});
}

// createLayout $_3 (captures a widget): whether it takes touches
uint32_t layoutTouchEnabled(uint32_t self) { return call(Widget_isTouchEnabled, {ld32(self + 4)}); }

// createLayout $_2 (touch): on touch end close the popup
void layoutCloseTouch(uint32_t, uint32_t, uint32_t typeRef) {
    if (rw::load<int32_t>(typeRef) != 2) return;
    call(SceneManager_closeCurrentPopup, {call(sfn::SceneManager_getInstance, {}), 0});
}

// open $_0 (the party request finished; captures the dialog): success builds the layout, an error shows the error
// dialog, and without a retry the dialog closes
void openDone(uint32_t self, uint32_t okRef) {
    uint8_t ok = ld8(okRef);
    uint32_t dialog = ld32(self + 4);
    call(SceneManager_enableLoadingIndicator, {call(sfn::SceneManager_getInstance, {}), 0});
    if (ok) {
        call(PartyInfoDialog_createLayout, {dialog, 0});
        return;
    }
    if (showErrorAndCheckRetry(VT_open0_bool)) return;
    call(PartyInfoDialog_close, {dialog});
}

}  // namespace PartyInfoDialogLambdas

using namespace PartyInfoDialogLambdas;
RW_FUNCTION(0x596AC2, "PartyInfoDialog::createRequestLayout $_7 operator()", requestCloseTouch);
RW_FUNCTION(0x596B82, "PartyInfoDialog::createRequestLayout $_6 operator()", nothing);
RW_FUNCTION(0x596C20, "PartyInfoDialog::openRequest $_5 operator()", openRequestDone);
RW_FUNCTION(0x596DE6, "PartyInfoDialog::openRequest $_5 {lambda()} operator()", openRequestFadedOut);
RW_FUNCTION(0x596EA6, "PartyInfoDialog::openRequest $_5 {lambda(bool)} operator()", nothing);
RW_FUNCTION(0x596F3A, "PartyInfoDialog::createLayout $_4 operator()", layoutBack);
RW_FUNCTION(0x596FEA, "PartyInfoDialog::createLayout $_3 operator()", layoutTouchEnabled);
RW_FUNCTION(0x59707A, "PartyInfoDialog::createLayout $_2 operator()", layoutCloseTouch);
RW_FUNCTION(0x597116, "PartyInfoDialog::createLayout $_1 operator()", nothing);
RW_FUNCTION(0x5971B4, "PartyInfoDialog::open $_0 operator()", openDone);
RW_FUNCTION(0x597316, "PartyInfoDialog::open $_0 {lambda(bool)} operator()", nothing);

// ------------------------------------------------------------------------------------------- __func thunks
// PartyInfoDialog::createRequestLayout $_7: __func::__clone(__base*)
static void t_596A94(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ae2cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596A94, "PartyInfoDialog::createRequestLayout $_7 __func::__clone(__base*)", t_596A94);

// PartyInfoDialog::createRequestLayout $_7: __func::destroy
static void t_596AB8(uint32_t self) {
}
RW_FUNCTION(0x596AB8, "PartyInfoDialog::createRequestLayout $_7 __func::destroy", t_596AB8);

// PartyInfoDialog::createRequestLayout $_7: __func::destroy_deallocate
static void t_596ABA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596ABA, "PartyInfoDialog::createRequestLayout $_7 __func::destroy_deallocate", t_596ABA);

// PartyInfoDialog::createRequestLayout $_7: __func::target
static uint32_t t_596AFC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f1c0u ? self + 4 : 0; }
RW_FUNCTION(0x596AFC, "PartyInfoDialog::createRequestLayout $_7 __func::target", t_596AFC);

// PartyInfoDialog::createRequestLayout $_7: __func::target_type
static uint32_t t_596B1C() { return 0xf8c2ecu; }
RW_FUNCTION(0x596B1C, "PartyInfoDialog::createRequestLayout $_7 __func::target_type", t_596B1C);

// PartyInfoDialog::createRequestLayout $_6: __func::~__func (deleting)
static void t_596B30(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596B30, "PartyInfoDialog::createRequestLayout $_6 __func::~__func (deleting)", t_596B30);

// PartyInfoDialog::createRequestLayout $_6: __func::__clone()
static uint32_t t_596B38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ae58u);
    }
    return v1;
}
RW_FUNCTION(0x596B38, "PartyInfoDialog::createRequestLayout $_6 __func::__clone()", t_596B38);

// PartyInfoDialog::createRequestLayout $_6: __func::__clone(__base*)
static void t_596B5C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ae58u);
    }
}
RW_FUNCTION(0x596B5C, "PartyInfoDialog::createRequestLayout $_6 __func::__clone(__base*)", t_596B5C);

// PartyInfoDialog::createRequestLayout $_6: __func::destroy
static void t_596B78(uint32_t self) {
}
RW_FUNCTION(0x596B78, "PartyInfoDialog::createRequestLayout $_6 __func::destroy", t_596B78);

// PartyInfoDialog::createRequestLayout $_6: __func::destroy_deallocate
static void t_596B7A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596B7A, "PartyInfoDialog::createRequestLayout $_6 __func::destroy_deallocate", t_596B7A);

// PartyInfoDialog::createRequestLayout $_6: __func::target
static uint32_t t_596B84(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f260u ? self + 4 : 0; }
RW_FUNCTION(0x596B84, "PartyInfoDialog::createRequestLayout $_6 __func::target", t_596B84);

// PartyInfoDialog::createRequestLayout $_6: __func::target_type
static uint32_t t_596BA4() { return 0xf8c300u; }
RW_FUNCTION(0x596BA4, "PartyInfoDialog::createRequestLayout $_6 __func::target_type", t_596BA4);

// PartyInfoDialog::openRequest $_5: __func::~__func (deleting)
static void t_596BB8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596BB8, "PartyInfoDialog::openRequest $_5 __func::~__func (deleting)", t_596BB8);

// PartyInfoDialog::openRequest $_5: __func::__clone()
static uint32_t t_596BC0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5ae84u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596BC0, "PartyInfoDialog::openRequest $_5 __func::__clone()", t_596BC0);

// PartyInfoDialog::openRequest $_5: __func::__clone(__base*)
static void t_596BF0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ae84u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596BF0, "PartyInfoDialog::openRequest $_5 __func::__clone(__base*)", t_596BF0);

// PartyInfoDialog::openRequest $_5: __func::destroy
static void t_596C14(uint32_t self) {
}
RW_FUNCTION(0x596C14, "PartyInfoDialog::openRequest $_5 __func::destroy", t_596C14);

// PartyInfoDialog::openRequest $_5: __func::destroy_deallocate
static void t_596C16(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596C16, "PartyInfoDialog::openRequest $_5 __func::destroy_deallocate", t_596C16);

// PartyInfoDialog::openRequest $_5: __func::target
static uint32_t t_596D54(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f310u ? self + 4 : 0; }
RW_FUNCTION(0x596D54, "PartyInfoDialog::openRequest $_5 __func::target", t_596D54);

// PartyInfoDialog::openRequest $_5: __func::target_type
static uint32_t t_596D74() { return 0xf8c314u; }
RW_FUNCTION(0x596D74, "PartyInfoDialog::openRequest $_5 __func::target_type", t_596D74);

// PartyInfoDialog::openRequest $_5 UlvE_: __func::~__func (deleting)
static void t_596D88(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596D88, "PartyInfoDialog::openRequest $_5 UlvE_ __func::~__func (deleting)", t_596D88);

// PartyInfoDialog::openRequest $_5 UlvE_: __func::__clone()
static uint32_t t_596D90(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5aeb0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596D90, "PartyInfoDialog::openRequest $_5 UlvE_ __func::__clone()", t_596D90);

// PartyInfoDialog::openRequest $_5 UlvE_: __func::__clone(__base*)
static void t_596DBC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aeb0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596DBC, "PartyInfoDialog::openRequest $_5 UlvE_ __func::__clone(__base*)", t_596DBC);

// PartyInfoDialog::openRequest $_5 UlvE_: __func::destroy
static void t_596DDC(uint32_t self) {
}
RW_FUNCTION(0x596DDC, "PartyInfoDialog::openRequest $_5 UlvE_ __func::destroy", t_596DDC);

// PartyInfoDialog::openRequest $_5 UlvE_: __func::destroy_deallocate
static void t_596DDE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596DDE, "PartyInfoDialog::openRequest $_5 UlvE_ __func::destroy_deallocate", t_596DDE);

// PartyInfoDialog::openRequest $_5 UlvE_: __func::target
static uint32_t t_596E20(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f3f0u ? self + 4 : 0; }
RW_FUNCTION(0x596E20, "PartyInfoDialog::openRequest $_5 UlvE_ __func::target", t_596E20);

// PartyInfoDialog::openRequest $_5 UlvE_: __func::target_type
static uint32_t t_596E40() { return 0xf8c328u; }
RW_FUNCTION(0x596E40, "PartyInfoDialog::openRequest $_5 UlvE_ __func::target_type", t_596E40);

// PartyInfoDialog::openRequest $_5 UlbE_: __func::~__func (deleting)
static void t_596E54(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596E54, "PartyInfoDialog::openRequest $_5 UlbE_ __func::~__func (deleting)", t_596E54);

// PartyInfoDialog::openRequest $_5 UlbE_: __func::__clone()
static uint32_t t_596E5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5aedcu);
    }
    return v1;
}
RW_FUNCTION(0x596E5C, "PartyInfoDialog::openRequest $_5 UlbE_ __func::__clone()", t_596E5C);

// PartyInfoDialog::openRequest $_5 UlbE_: __func::__clone(__base*)
static void t_596E80(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aedcu);
    }
}
RW_FUNCTION(0x596E80, "PartyInfoDialog::openRequest $_5 UlbE_ __func::__clone(__base*)", t_596E80);

// PartyInfoDialog::openRequest $_5 UlbE_: __func::destroy
static void t_596E9C(uint32_t self) {
}
RW_FUNCTION(0x596E9C, "PartyInfoDialog::openRequest $_5 UlbE_ __func::destroy", t_596E9C);

// PartyInfoDialog::openRequest $_5 UlbE_: __func::destroy_deallocate
static void t_596E9E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596E9E, "PartyInfoDialog::openRequest $_5 UlbE_ __func::destroy_deallocate", t_596E9E);

// PartyInfoDialog::openRequest $_5 UlbE_: __func::target
static uint32_t t_596EA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f4e0u ? self + 4 : 0; }
RW_FUNCTION(0x596EA8, "PartyInfoDialog::openRequest $_5 UlbE_ __func::target", t_596EA8);

// PartyInfoDialog::openRequest $_5 UlbE_: __func::target_type
static uint32_t t_596EC8() { return 0xf8c33cu; }
RW_FUNCTION(0x596EC8, "PartyInfoDialog::openRequest $_5 UlbE_ __func::target_type", t_596EC8);

// PartyInfoDialog::createLayout $_4: __func::~__func (deleting)
static void t_596EDC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596EDC, "PartyInfoDialog::createLayout $_4 __func::~__func (deleting)", t_596EDC);

// PartyInfoDialog::createLayout $_4: __func::__clone()
static uint32_t t_596EE4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5af08u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596EE4, "PartyInfoDialog::createLayout $_4 __func::__clone()", t_596EE4);

// PartyInfoDialog::createLayout $_4: __func::__clone(__base*)
static void t_596F10(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5af08u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596F10, "PartyInfoDialog::createLayout $_4 __func::__clone(__base*)", t_596F10);

// PartyInfoDialog::createLayout $_4: __func::destroy
static void t_596F30(uint32_t self) {
}
RW_FUNCTION(0x596F30, "PartyInfoDialog::createLayout $_4 __func::destroy", t_596F30);

// PartyInfoDialog::createLayout $_4: __func::destroy_deallocate
static void t_596F32(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596F32, "PartyInfoDialog::createLayout $_4 __func::destroy_deallocate", t_596F32);

// PartyInfoDialog::createLayout $_4: __func::target
static uint32_t t_596F58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f5a0u ? self + 4 : 0; }
RW_FUNCTION(0x596F58, "PartyInfoDialog::createLayout $_4 __func::target", t_596F58);

// PartyInfoDialog::createLayout $_4: __func::target_type
static uint32_t t_596F78() { return 0xf8c350u; }
RW_FUNCTION(0x596F78, "PartyInfoDialog::createLayout $_4 __func::target_type", t_596F78);

// PartyInfoDialog::createLayout $_3: __func::~__func (deleting)
static void t_596F8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596F8C, "PartyInfoDialog::createLayout $_3 __func::~__func (deleting)", t_596F8C);

// PartyInfoDialog::createLayout $_3: __func::__clone()
static uint32_t t_596F94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5af34u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596F94, "PartyInfoDialog::createLayout $_3 __func::__clone()", t_596F94);

// PartyInfoDialog::createLayout $_3: __func::__clone(__base*)
static void t_596FC0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5af34u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596FC0, "PartyInfoDialog::createLayout $_3 __func::__clone(__base*)", t_596FC0);

// PartyInfoDialog::createLayout $_3: __func::destroy
static void t_596FE0(uint32_t self) {
}
RW_FUNCTION(0x596FE0, "PartyInfoDialog::createLayout $_3 __func::destroy", t_596FE0);

// PartyInfoDialog::createLayout $_3: __func::destroy_deallocate
static void t_596FE2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596FE2, "PartyInfoDialog::createLayout $_3 __func::destroy_deallocate", t_596FE2);

// PartyInfoDialog::createLayout $_3: __func::target
static uint32_t t_596FF4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f630u ? self + 4 : 0; }
RW_FUNCTION(0x596FF4, "PartyInfoDialog::createLayout $_3 __func::target", t_596FF4);

// PartyInfoDialog::createLayout $_3: __func::target_type
static uint32_t t_597014() { return 0xf8c364u; }
RW_FUNCTION(0x597014, "PartyInfoDialog::createLayout $_3 __func::target_type", t_597014);

// PartyInfoDialog::createLayout $_2: __func::~__func (deleting)
static void t_597028(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x597028, "PartyInfoDialog::createLayout $_2 __func::~__func (deleting)", t_597028);

// PartyInfoDialog::createLayout $_2: __func::__clone()
static uint32_t t_597030(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5af60u);
    }
    return v1;
}
RW_FUNCTION(0x597030, "PartyInfoDialog::createLayout $_2 __func::__clone()", t_597030);

// PartyInfoDialog::createLayout $_2: __func::__clone(__base*)
static void t_597054(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5af60u);
    }
}
RW_FUNCTION(0x597054, "PartyInfoDialog::createLayout $_2 __func::__clone(__base*)", t_597054);

// PartyInfoDialog::createLayout $_2: __func::destroy
static void t_597070(uint32_t self) {
}
RW_FUNCTION(0x597070, "PartyInfoDialog::createLayout $_2 __func::destroy", t_597070);

// PartyInfoDialog::createLayout $_2: __func::destroy_deallocate
static void t_597072(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x597072, "PartyInfoDialog::createLayout $_2 __func::destroy_deallocate", t_597072);

// PartyInfoDialog::createLayout $_2: __func::target
static uint32_t t_597090(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f6f0u ? self + 4 : 0; }
RW_FUNCTION(0x597090, "PartyInfoDialog::createLayout $_2 __func::target", t_597090);

// PartyInfoDialog::createLayout $_2: __func::target_type
static uint32_t t_5970B0() { return 0xf8c378u; }
RW_FUNCTION(0x5970B0, "PartyInfoDialog::createLayout $_2 __func::target_type", t_5970B0);

// PartyInfoDialog::createLayout $_1: __func::~__func (deleting)
static void t_5970C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5970C4, "PartyInfoDialog::createLayout $_1 __func::~__func (deleting)", t_5970C4);

// PartyInfoDialog::createLayout $_1: __func::__clone()
static uint32_t t_5970CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5af8cu);
    }
    return v1;
}
RW_FUNCTION(0x5970CC, "PartyInfoDialog::createLayout $_1 __func::__clone()", t_5970CC);

// PartyInfoDialog::createLayout $_1: __func::__clone(__base*)
static void t_5970F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5af8cu);
    }
}
RW_FUNCTION(0x5970F0, "PartyInfoDialog::createLayout $_1 __func::__clone(__base*)", t_5970F0);

// PartyInfoDialog::createLayout $_1: __func::destroy
static void t_59710C(uint32_t self) {
}
RW_FUNCTION(0x59710C, "PartyInfoDialog::createLayout $_1 __func::destroy", t_59710C);

// PartyInfoDialog::createLayout $_1: __func::destroy_deallocate
static void t_59710E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59710E, "PartyInfoDialog::createLayout $_1 __func::destroy_deallocate", t_59710E);

// PartyInfoDialog::createLayout $_1: __func::target
static uint32_t t_597118(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f780u ? self + 4 : 0; }
RW_FUNCTION(0x597118, "PartyInfoDialog::createLayout $_1 __func::target", t_597118);

// PartyInfoDialog::createLayout $_1: __func::target_type
static uint32_t t_597138() { return 0xf8c38cu; }
RW_FUNCTION(0x597138, "PartyInfoDialog::createLayout $_1 __func::target_type", t_597138);

// PartyInfoDialog::open $_0: __func::~__func (deleting)
static void t_59714C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59714C, "PartyInfoDialog::open $_0 __func::~__func (deleting)", t_59714C);

// PartyInfoDialog::open $_0: __func::__clone()
static uint32_t t_597154(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5afb8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x597154, "PartyInfoDialog::open $_0 __func::__clone()", t_597154);

// PartyInfoDialog::open $_0: __func::__clone(__base*)
static void t_597184(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5afb8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x597184, "PartyInfoDialog::open $_0 __func::__clone(__base*)", t_597184);

// PartyInfoDialog::open $_0: __func::destroy
static void t_5971A8(uint32_t self) {
}
RW_FUNCTION(0x5971A8, "PartyInfoDialog::open $_0 __func::destroy", t_5971A8);

// PartyInfoDialog::open $_0: __func::destroy_deallocate
static void t_5971AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5971AA, "PartyInfoDialog::open $_0 __func::destroy_deallocate", t_5971AA);

// PartyInfoDialog::open $_0: __func::target
static uint32_t t_597290(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f830u ? self + 4 : 0; }
RW_FUNCTION(0x597290, "PartyInfoDialog::open $_0 __func::target", t_597290);

// PartyInfoDialog::open $_0: __func::target_type
static uint32_t t_5972B0() { return 0xf8c3a0u; }
RW_FUNCTION(0x5972B0, "PartyInfoDialog::open $_0 __func::target_type", t_5972B0);

// PartyInfoDialog::open $_0 UlbE_: __func::~__func (deleting)
static void t_5972C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5972C4, "PartyInfoDialog::open $_0 UlbE_ __func::~__func (deleting)", t_5972C4);

// PartyInfoDialog::open $_0 UlbE_: __func::__clone()
static uint32_t t_5972CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5afe4u);
    }
    return v1;
}
RW_FUNCTION(0x5972CC, "PartyInfoDialog::open $_0 UlbE_ __func::__clone()", t_5972CC);

// PartyInfoDialog::open $_0 UlbE_: __func::__clone(__base*)
static void t_5972F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5afe4u);
    }
}
RW_FUNCTION(0x5972F0, "PartyInfoDialog::open $_0 UlbE_ __func::__clone(__base*)", t_5972F0);

// PartyInfoDialog::open $_0 UlbE_: __func::destroy
static void t_59730C(uint32_t self) {
}
RW_FUNCTION(0x59730C, "PartyInfoDialog::open $_0 UlbE_ __func::destroy", t_59730C);

// PartyInfoDialog::open $_0 UlbE_: __func::destroy_deallocate
static void t_59730E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59730E, "PartyInfoDialog::open $_0 UlbE_ __func::destroy_deallocate", t_59730E);

// PartyInfoDialog::open $_0 UlbE_: __func::target
static uint32_t t_597318(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f910u ? self + 4 : 0; }
RW_FUNCTION(0x597318, "PartyInfoDialog::open $_0 UlbE_ __func::target", t_597318);

// PartyInfoDialog::open $_0 UlbE_: __func::target_type
static uint32_t t_597338() { return 0xf8c3b4u; }
RW_FUNCTION(0x597338, "PartyInfoDialog::open $_0 UlbE_ __func::target_type", t_597338);
