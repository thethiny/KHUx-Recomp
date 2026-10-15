// The std::function __func thunks of the SceneNotice lambdas (first generated from the lifted code by a scratch
// tool): clone (copy the captures into a new / given block), destroy (destroy the captured objects),
// target / target_type (typeinfo checks). The lambda bodies are in w3_7/SceneNotice.cpp.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
constexpr uint32_t function_node_copy = 0x474D99;   // std::function<void(Node*)>::function(const function&)
}  // namespace

// SceneNotice::openNextPage $_9: __func::~__func
static uint32_t t_73463C(uint32_t self) {
    st32(self, 0xf6d2f4u);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x73463C, "SceneNotice::openNextPage $_9 __func::~__func", t_73463C);

// SceneNotice::openNextPage $_9: __func::~__func (deleting)
static void t_734678(uint32_t self) {
    st32(self, 0xf6d2f4u);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734678, "SceneNotice::openNextPage $_9 __func::~__func (deleting)", t_734678);

// SceneNotice::openNextPage $_9: __func::__clone()
static uint32_t t_7346B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf6d2f4u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x7346B8, "SceneNotice::openNextPage $_9 __func::__clone()", t_7346B8);

// SceneNotice::openNextPage $_9: __func::__clone(__base*)
static void t_734710(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d2f4u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x734710, "SceneNotice::openNextPage $_9 __func::__clone(__base*)", t_734710);

// SceneNotice::openNextPage $_9: __func::destroy
static void t_734750(uint32_t self) {
    functionDestroy(self + 16);
}
RW_FUNCTION(0x734750, "SceneNotice::openNextPage $_9 __func::destroy", t_734750);

// SceneNotice::openNextPage $_9: __func::destroy_deallocate
static void t_734770(uint32_t self) {
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734770, "SceneNotice::openNextPage $_9 __func::destroy_deallocate", t_734770);

// SceneNotice::openNextPage $_9: __func::target
static uint32_t t_734834(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9a510u ? self + 8 : 0; }
RW_FUNCTION(0x734834, "SceneNotice::openNextPage $_9 __func::target", t_734834);

// SceneNotice::openNextPage $_9: __func::target_type
static uint32_t t_734858() { return 0xfa6860u; }
RW_FUNCTION(0x734858, "SceneNotice::openNextPage $_9 __func::target_type", t_734858);

// SceneNotice::openWebViewInfoPopup $_8: __func::~__func
static uint32_t t_73486C(uint32_t self) {
    st32(self, 0xf6d320u);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x73486C, "SceneNotice::openWebViewInfoPopup $_8 __func::~__func", t_73486C);

// SceneNotice::openWebViewInfoPopup $_8: __func::~__func (deleting)
static void t_7348A8(uint32_t self) {
    st32(self, 0xf6d320u);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7348A8, "SceneNotice::openWebViewInfoPopup $_8 __func::~__func (deleting)", t_7348A8);

// SceneNotice::openWebViewInfoPopup $_8: __func::destroy
static void t_734988(uint32_t self) {
    functionDestroy(self + 16);
}
RW_FUNCTION(0x734988, "SceneNotice::openWebViewInfoPopup $_8 __func::destroy", t_734988);

// SceneNotice::openWebViewInfoPopup $_8: __func::destroy_deallocate
static void t_7349A8(uint32_t self) {
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7349A8, "SceneNotice::openWebViewInfoPopup $_8 __func::destroy_deallocate", t_7349A8);

// SceneNotice::openWebViewInfoPopup $_8: __func::target
static uint32_t t_734B30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9a610u ? self + 8 : 0; }
RW_FUNCTION(0x734B30, "SceneNotice::openWebViewInfoPopup $_8 __func::target", t_734B30);

// SceneNotice::openWebViewInfoPopup $_8: __func::target_type
static uint32_t t_734B54() { return 0xfa6874u; }
RW_FUNCTION(0x734B54, "SceneNotice::openWebViewInfoPopup $_8 __func::target_type", t_734B54);

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1}: __func::~__func
static uint32_t t_734B68(uint32_t self) {
    st32(self, 0xf6d34cu);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x734B68, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::~__func", t_734B68);

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1}: __func::~__func (deleting)
static void t_734BA4(uint32_t self) {
    st32(self, 0xf6d34cu);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734BA4, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::~__func (deleting)", t_734BA4);

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1}: __func::destroy
static void t_734C84(uint32_t self) {
    functionDestroy(self + 8);
}
RW_FUNCTION(0x734C84, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::destroy", t_734C84);

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1}: __func::destroy_deallocate
static void t_734CA4(uint32_t self) {
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734CA4, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::destroy_deallocate", t_734CA4);

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1}: __func::target
static uint32_t t_734D0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9a780u ? self + 8 : 0; }
RW_FUNCTION(0x734D0C, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::target", t_734D0C);

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1}: __func::target_type
static uint32_t t_734D2C() { return 0xfa6888u; }
RW_FUNCTION(0x734D2C, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::target_type", t_734D2C);

// SceneNotice::openWebViewInfoPopup $_7: __func::~__func (deleting)
static void t_734D40(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734D40, "SceneNotice::openWebViewInfoPopup $_7 __func::~__func (deleting)", t_734D40);

// SceneNotice::openWebViewInfoPopup $_7: __func::__clone()
static uint32_t t_734D48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d378u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x734D48, "SceneNotice::openWebViewInfoPopup $_7 __func::__clone()", t_734D48);

// SceneNotice::openWebViewInfoPopup $_7: __func::__clone(__base*)
static void t_734D74(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d378u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x734D74, "SceneNotice::openWebViewInfoPopup $_7 __func::__clone(__base*)", t_734D74);

// SceneNotice::openWebViewInfoPopup $_7: __func::destroy
static void t_734D94(uint32_t self) {
}
RW_FUNCTION(0x734D94, "SceneNotice::openWebViewInfoPopup $_7 __func::destroy", t_734D94);

// SceneNotice::openWebViewInfoPopup $_7: __func::destroy_deallocate
static void t_734D96(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734D96, "SceneNotice::openWebViewInfoPopup $_7 __func::destroy_deallocate", t_734D96);

// SceneNotice::openWebViewInfoPopup $_7: __func::target
static uint32_t t_734DA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9a8f0u ? self + 4 : 0; }
RW_FUNCTION(0x734DA8, "SceneNotice::openWebViewInfoPopup $_7 __func::target", t_734DA8);

// SceneNotice::openWebViewInfoPopup $_7: __func::target_type
static uint32_t t_734DC8() { return 0xfa689cu; }
RW_FUNCTION(0x734DC8, "SceneNotice::openWebViewInfoPopup $_7 __func::target_type", t_734DC8);

// SceneNotice::openWebViewInfoPopup $_6: __func::~__func
static uint32_t t_734DDC(uint32_t self) {
    st32(self, 0xf6d3a4u);
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x734DDC, "SceneNotice::openWebViewInfoPopup $_6 __func::~__func", t_734DDC);

// SceneNotice::openWebViewInfoPopup $_6: __func::~__func (deleting)
static void t_734E0C(uint32_t self) {
    st32(self, 0xf6d3a4u);
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734E0C, "SceneNotice::openWebViewInfoPopup $_6 __func::~__func (deleting)", t_734E0C);

// SceneNotice::openWebViewInfoPopup $_6: __func::__clone(__base*)
static void t_734EAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d3a4u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
        uint32_t v3 = ld8(self + 12);
        if (v3 & 1) {
            uint32_t v4 = ld32(self + 16);
            uint32_t v5 = ld32(self + 20);
            call(0x47cd93, {p + 12, v5, v4});   // sub_47CD92
            return;
        }
        uint32_t v6 = ld32(self + 12 + 0);
        uint32_t v7 = ld32(self + 12 + 4);
        uint32_t v8 = ld32(self + 12 + 8);
        st32(p + 12 + 0, v6);
        st32(p + 12 + 4, v7);
        st32(p + 12 + 8, v8);
        return;
    }
}
RW_FUNCTION(0x734EAC, "SceneNotice::openWebViewInfoPopup $_6 __func::__clone(__base*)", t_734EAC);

// SceneNotice::openWebViewInfoPopup $_6: __func::destroy
static void t_734F00(uint32_t self) {
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x734F00, "SceneNotice::openWebViewInfoPopup $_6 __func::destroy", t_734F00);

// SceneNotice::openWebViewInfoPopup $_6: __func::destroy_deallocate
static void t_734F12(uint32_t self) {
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x734F12, "SceneNotice::openWebViewInfoPopup $_6 __func::destroy_deallocate", t_734F12);

// SceneNotice::openWebViewInfoPopup $_6: __func::target
static uint32_t t_7350B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9aa50u ? self + 4 : 0; }
RW_FUNCTION(0x7350B0, "SceneNotice::openWebViewInfoPopup $_6 __func::target", t_7350B0);

// SceneNotice::openWebViewInfoPopup $_6: __func::target_type
static uint32_t t_7350D0() { return 0xfa68b0u; }
RW_FUNCTION(0x7350D0, "SceneNotice::openWebViewInfoPopup $_6 __func::target_type", t_7350D0);

// SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1}: __func::~__func (deleting)
static void t_7350E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7350E4, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} __func::~__func (deleting)", t_7350E4);

// SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1}: __func::__clone()
static uint32_t t_7350EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d3d0u);
    }
    return v1;
}
RW_FUNCTION(0x7350EC, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} __func::__clone()", t_7350EC);

// SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1}: __func::__clone(__base*)
static void t_735110(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d3d0u);
    }
}
RW_FUNCTION(0x735110, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} __func::__clone(__base*)", t_735110);

// SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1}: __func::destroy
static void t_73512C(uint32_t self) {
}
RW_FUNCTION(0x73512C, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} __func::destroy", t_73512C);

// SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1}: __func::destroy_deallocate
static void t_73512E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73512E, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} __func::destroy_deallocate", t_73512E);

// SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1}: __func::target
static uint32_t t_735180(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9abe0u ? self + 4 : 0; }
RW_FUNCTION(0x735180, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} __func::target", t_735180);

// SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1}: __func::target_type
static uint32_t t_7351A0() { return 0xfa68c4u; }
RW_FUNCTION(0x7351A0, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} __func::target_type", t_7351A0);

// SceneNotice::openWebViewInfoPopup $_5: __func::~__func (deleting)
static void t_7351B4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7351B4, "SceneNotice::openWebViewInfoPopup $_5 __func::~__func (deleting)", t_7351B4);

// SceneNotice::openWebViewInfoPopup $_5: __func::__clone()
static uint32_t t_7351BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d3fcu);
    }
    return v1;
}
RW_FUNCTION(0x7351BC, "SceneNotice::openWebViewInfoPopup $_5 __func::__clone()", t_7351BC);

// SceneNotice::openWebViewInfoPopup $_5: __func::__clone(__base*)
static void t_7351E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d3fcu);
    }
}
RW_FUNCTION(0x7351E0, "SceneNotice::openWebViewInfoPopup $_5 __func::__clone(__base*)", t_7351E0);

// SceneNotice::openWebViewInfoPopup $_5: __func::destroy
static void t_7351FC(uint32_t self) {
}
RW_FUNCTION(0x7351FC, "SceneNotice::openWebViewInfoPopup $_5 __func::destroy", t_7351FC);

// SceneNotice::openWebViewInfoPopup $_5: __func::destroy_deallocate
static void t_7351FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7351FE, "SceneNotice::openWebViewInfoPopup $_5 __func::destroy_deallocate", t_7351FE);

// SceneNotice::openWebViewInfoPopup $_5: __func::target
static uint32_t t_73521C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9ad80u ? self + 4 : 0; }
RW_FUNCTION(0x73521C, "SceneNotice::openWebViewInfoPopup $_5 __func::target", t_73521C);

// SceneNotice::openWebViewInfoPopup $_5: __func::target_type
static uint32_t t_73523C() { return 0xfa68d8u; }
RW_FUNCTION(0x73523C, "SceneNotice::openWebViewInfoPopup $_5 __func::target_type", t_73523C);

// SceneNotice::init $_4: __func::~__func (deleting)
static void t_735250(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x735250, "SceneNotice::init $_4 __func::~__func (deleting)", t_735250);

// SceneNotice::init $_4: __func::__clone()
static uint32_t t_735258(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d428u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x735258, "SceneNotice::init $_4 __func::__clone()", t_735258);

// SceneNotice::init $_4: __func::__clone(__base*)
static void t_735284(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d428u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x735284, "SceneNotice::init $_4 __func::__clone(__base*)", t_735284);

// SceneNotice::init $_4: __func::destroy
static void t_7352A4(uint32_t self) {
}
RW_FUNCTION(0x7352A4, "SceneNotice::init $_4 __func::destroy", t_7352A4);

// SceneNotice::init $_4: __func::destroy_deallocate
static void t_7352A6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7352A6, "SceneNotice::init $_4 __func::destroy_deallocate", t_7352A6);

// SceneNotice::init $_4: __func::target
static uint32_t t_735340(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9ae70u ? self + 4 : 0; }
RW_FUNCTION(0x735340, "SceneNotice::init $_4 __func::target", t_735340);

// SceneNotice::init $_4: __func::target_type
static uint32_t t_735360() { return 0xfa68ecu; }
RW_FUNCTION(0x735360, "SceneNotice::init $_4 __func::target_type", t_735360);

// SceneNotice::init $_4 UlvE_: __func::~__func (deleting)
static void t_735374(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x735374, "SceneNotice::init $_4 UlvE_ __func::~__func (deleting)", t_735374);

// SceneNotice::init $_4 UlvE_: __func::__clone()
static uint32_t t_73537C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d454u);
    }
    return v1;
}
RW_FUNCTION(0x73537C, "SceneNotice::init $_4 UlvE_ __func::__clone()", t_73537C);

// SceneNotice::init $_4 UlvE_: __func::__clone(__base*)
static void t_7353A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d454u);
    }
}
RW_FUNCTION(0x7353A0, "SceneNotice::init $_4 UlvE_ __func::__clone(__base*)", t_7353A0);

// SceneNotice::init $_4 UlvE_: __func::destroy
static void t_7353BC(uint32_t self) {
}
RW_FUNCTION(0x7353BC, "SceneNotice::init $_4 UlvE_ __func::destroy", t_7353BC);

// SceneNotice::init $_4 UlvE_: __func::destroy_deallocate
static void t_7353BE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7353BE, "SceneNotice::init $_4 UlvE_ __func::destroy_deallocate", t_7353BE);

// SceneNotice::init $_4 UlvE_: __func::target
static uint32_t t_7355B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9aef0u ? self + 4 : 0; }
RW_FUNCTION(0x7355B0, "SceneNotice::init $_4 UlvE_ __func::target", t_7355B0);

// SceneNotice::init $_4 UlvE_: __func::target_type
static uint32_t t_7355D0() { return 0xfa6900u; }
RW_FUNCTION(0x7355D0, "SceneNotice::init $_4 UlvE_ __func::target_type", t_7355D0);

// SceneNotice::init $_3: __func::~__func (deleting)
static void t_7355EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7355EC, "SceneNotice::init $_3 __func::~__func (deleting)", t_7355EC);

// SceneNotice::init $_3: __func::__clone()
static uint32_t t_7355F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d480u);
    }
    return v1;
}
RW_FUNCTION(0x7355F4, "SceneNotice::init $_3 __func::__clone()", t_7355F4);

// SceneNotice::init $_3: __func::__clone(__base*)
static void t_735618(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d480u);
    }
}
RW_FUNCTION(0x735618, "SceneNotice::init $_3 __func::__clone(__base*)", t_735618);

// SceneNotice::init $_3: __func::destroy
static void t_735634(uint32_t self) {
}
RW_FUNCTION(0x735634, "SceneNotice::init $_3 __func::destroy", t_735634);

// SceneNotice::init $_3: __func::destroy_deallocate
static void t_735636(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x735636, "SceneNotice::init $_3 __func::destroy_deallocate", t_735636);

// SceneNotice::init $_3: __func::target
static uint32_t t_7356D0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9af70u ? self + 4 : 0; }
RW_FUNCTION(0x7356D0, "SceneNotice::init $_3 __func::target", t_7356D0);

// SceneNotice::init $_3: __func::target_type
static uint32_t t_7356F0() { return 0xfa6914u; }
RW_FUNCTION(0x7356F0, "SceneNotice::init $_3 __func::target_type", t_7356F0);

// SceneNotice::init $_3 UlbE_ UlbE_: __func::~__func (deleting)
static void t_735704(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x735704, "SceneNotice::init $_3 UlbE_ UlbE_ __func::~__func (deleting)", t_735704);

// SceneNotice::init $_3 UlbE_ UlbE_: __func::__clone()
static uint32_t t_73570C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d4acu);
    }
    return v1;
}
RW_FUNCTION(0x73570C, "SceneNotice::init $_3 UlbE_ UlbE_ __func::__clone()", t_73570C);

// SceneNotice::init $_3 UlbE_ UlbE_: __func::__clone(__base*)
static void t_735730(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d4acu);
    }
}
RW_FUNCTION(0x735730, "SceneNotice::init $_3 UlbE_ UlbE_ __func::__clone(__base*)", t_735730);

// SceneNotice::init $_3 UlbE_ UlbE_: __func::destroy
static void t_73574C(uint32_t self) {
}
RW_FUNCTION(0x73574C, "SceneNotice::init $_3 UlbE_ UlbE_ __func::destroy", t_73574C);

// SceneNotice::init $_3 UlbE_ UlbE_: __func::destroy_deallocate
static void t_73574E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73574E, "SceneNotice::init $_3 UlbE_ UlbE_ __func::destroy_deallocate", t_73574E);

// SceneNotice::init $_3 UlbE_ UlbE_: __func::target
static uint32_t t_735760(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b000u ? self + 4 : 0; }
RW_FUNCTION(0x735760, "SceneNotice::init $_3 UlbE_ UlbE_ __func::target", t_735760);

// SceneNotice::init $_3 UlbE_ UlbE_: __func::target_type
static uint32_t t_735780() { return 0xfa6928u; }
RW_FUNCTION(0x735780, "SceneNotice::init $_3 UlbE_ UlbE_ __func::target_type", t_735780);

// SceneNotice::init $_2: __func::~__func (deleting)
static void t_735794(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x735794, "SceneNotice::init $_2 __func::~__func (deleting)", t_735794);

// SceneNotice::init $_2: __func::__clone()
static uint32_t t_73579C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d4d8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x73579C, "SceneNotice::init $_2 __func::__clone()", t_73579C);

// SceneNotice::init $_2: __func::__clone(__base*)
static void t_7357C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d4d8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7357C8, "SceneNotice::init $_2 __func::__clone(__base*)", t_7357C8);

// SceneNotice::init $_2: __func::destroy
static void t_7357E8(uint32_t self) {
}
RW_FUNCTION(0x7357E8, "SceneNotice::init $_2 __func::destroy", t_7357E8);

// SceneNotice::init $_2: __func::destroy_deallocate
static void t_7357EA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7357EA, "SceneNotice::init $_2 __func::destroy_deallocate", t_7357EA);

// SceneNotice::init $_2: __func::target
static uint32_t t_735808(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b090u ? self + 4 : 0; }
RW_FUNCTION(0x735808, "SceneNotice::init $_2 __func::target", t_735808);

// SceneNotice::init $_2: __func::target_type
static uint32_t t_735828() { return 0xfa693cu; }
RW_FUNCTION(0x735828, "SceneNotice::init $_2 __func::target_type", t_735828);

// SceneNotice::init $_1: __func::~__func (deleting)
static void t_73583C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73583C, "SceneNotice::init $_1 __func::~__func (deleting)", t_73583C);

// SceneNotice::init $_1: __func::__clone()
static uint32_t t_735844(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d504u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x735844, "SceneNotice::init $_1 __func::__clone()", t_735844);

// SceneNotice::init $_1: __func::__clone(__base*)
static void t_735870(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d504u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x735870, "SceneNotice::init $_1 __func::__clone(__base*)", t_735870);

// SceneNotice::init $_1: __func::destroy
static void t_735890(uint32_t self) {
}
RW_FUNCTION(0x735890, "SceneNotice::init $_1 __func::destroy", t_735890);

// SceneNotice::init $_1: __func::destroy_deallocate
static void t_735892(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x735892, "SceneNotice::init $_1 __func::destroy_deallocate", t_735892);

// SceneNotice::init $_1: __func::target
static uint32_t t_7358B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b100u ? self + 4 : 0; }
RW_FUNCTION(0x7358B0, "SceneNotice::init $_1 __func::target", t_7358B0);

// SceneNotice::init $_1: __func::target_type
static uint32_t t_7358D0() { return 0xfa6950u; }
RW_FUNCTION(0x7358D0, "SceneNotice::init $_1 __func::target_type", t_7358D0);

// SceneNotice::callPrepareAPI $_0: __func::~__func (deleting)
static void t_7358E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7358E4, "SceneNotice::callPrepareAPI $_0 __func::~__func (deleting)", t_7358E4);

// SceneNotice::callPrepareAPI $_0: __func::__clone()
static uint32_t t_7358EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d530u);
    }
    return v1;
}
RW_FUNCTION(0x7358EC, "SceneNotice::callPrepareAPI $_0 __func::__clone()", t_7358EC);

// SceneNotice::callPrepareAPI $_0: __func::__clone(__base*)
static void t_735910(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d530u);
    }
}
RW_FUNCTION(0x735910, "SceneNotice::callPrepareAPI $_0 __func::__clone(__base*)", t_735910);

// SceneNotice::callPrepareAPI $_0: __func::destroy
static void t_73592C(uint32_t self) {
}
RW_FUNCTION(0x73592C, "SceneNotice::callPrepareAPI $_0 __func::destroy", t_73592C);

// SceneNotice::callPrepareAPI $_0: __func::destroy_deallocate
static void t_73592E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73592E, "SceneNotice::callPrepareAPI $_0 __func::destroy_deallocate", t_73592E);

// SceneNotice::callPrepareAPI $_0: __func::target
static uint32_t t_735A08(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b190u ? self + 4 : 0; }
RW_FUNCTION(0x735A08, "SceneNotice::callPrepareAPI $_0 __func::target", t_735A08);

// SceneNotice::callPrepareAPI $_0: __func::target_type
static uint32_t t_735A28() { return 0xfa6964u; }
RW_FUNCTION(0x735A28, "SceneNotice::callPrepareAPI $_0 __func::target_type", t_735A28);

// SceneNotice::openWebViewInfoPopup $_8 {shadow +8, panel +12, function<void(Node*)> +16, popupLayer +40}:
// __func::__clone()
static uint32_t t_7348E8(uint32_t self) {
    uint32_t copy = call(fn::operator_new, {0x30});
    if (copy) {
        st32(copy, 0xf6d320u);
        uint32_t shadow = ld32(self + 8), panel = ld32(self + 12);
        st32(copy + 8, shadow);
        st32(copy + 12, panel);
        call(function_node_copy, {copy + 16, self + 16});
        st32(copy + 40, ld32(self + 40));
    }
    return copy;
}
RW_FUNCTION(0x7348E8, "SceneNotice::openWebViewInfoPopup $_8 __func::__clone()", t_7348E8);

// SceneNotice::openWebViewInfoPopup $_8: __func::__clone(__base*)
static void t_734944(uint32_t self, uint32_t p) {
    if (!p) return;
    st32(p, 0xf6d320u);
    uint32_t shadow = ld32(self + 8), panel = ld32(self + 12);
    st32(p + 8, shadow);
    st32(p + 12, panel);
    call(function_node_copy, {p + 16, self + 16});
    st32(p + 40, ld32(self + 40));
}
RW_FUNCTION(0x734944, "SceneNotice::openWebViewInfoPopup $_8 __func::__clone(__base*)", t_734944);

// copy $_8::{lambda()#1}'s plain captures {panel +0x20, popupLayer +0x24, shadow +0x28}
static void noticeCopyRemoveCaptures(uint32_t dst, uint32_t src) {
    uint32_t panel = ld32(src + 0x20), layer = ld32(src + 0x24), shadow = ld32(src + 0x28);
    st32(dst + 0x20, panel);
    st32(dst + 0x24, layer);
    st32(dst + 0x28, shadow);
}

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} {function<void(Node*)> +8, panel, popupLayer, shadow}:
// __func::__clone()
static uint32_t t_734BE4(uint32_t self) {
    uint32_t copy = call(fn::operator_new, {0x30});
    if (copy) {
        st32(copy, 0xf6d34cu);
        call(function_node_copy, {copy + 8, self + 8});
        noticeCopyRemoveCaptures(copy, self);
    }
    return copy;
}
RW_FUNCTION(0x734BE4, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::__clone()", t_734BE4);

// SceneNotice::openWebViewInfoPopup $_8::{lambda()#1}: __func::__clone(__base*)
static void t_734C40(uint32_t self, uint32_t p) {
    if (!p) return;
    st32(p, 0xf6d34cu);
    call(function_node_copy, {p + 8, self + 8});
    noticeCopyRemoveCaptures(p, self);
}
RW_FUNCTION(0x734C40, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} __func::__clone(__base*)", t_734C40);

// SceneNotice::openWebViewInfoPopup $_6 {this +4, scrollArea +8, std::string url +12}: __func::__clone()
static uint32_t t_734E40(uint32_t self) {
    uint32_t copy = call(fn::operator_new, {0x18});
    if (copy) {
        st32(copy, 0xf6d3a4u);
        uint32_t scene = ld32(self + 4), scroll = ld32(self + 8);
        st32(copy + 8, scroll);
        st32(copy + 4, scene);
        if (ld8(self + 12) & 1) {
            call(fn::string_init, {copy + 12, ld32(self + 20), ld32(self + 16)});
        } else {
            uint32_t w0 = ld32(self + 12), w1 = ld32(self + 16), w2 = ld32(self + 20);
            st32(copy + 12, w0);
            st32(copy + 16, w1);
            st32(copy + 20, w2);
        }
    }
    return copy;
}
RW_FUNCTION(0x734E40, "SceneNotice::openWebViewInfoPopup $_6 __func::__clone()", t_734E40);
