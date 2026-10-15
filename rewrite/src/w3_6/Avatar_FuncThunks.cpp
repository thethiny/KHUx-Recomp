// The std::function __func thunks of the Avatar lambdas (generated from the lifted code by a scratch tool and
// checked against it): clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "Common.hpp"

using namespace w36;

// FlashNode $_3 (shared): __func::~__func
static uint32_t t_5C54B0(uint32_t self) {
    return self;
}
RW_FUNCTION(0x5C54B0, "FlashNode $_3 (shared) __func::~__func", t_5C54B0);

// Avatar::addEventListeners $_12: __func::~__func (deleting)
static void t_5C830C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C830C, "Avatar::addEventListeners $_12 __func::~__func (deleting)", t_5C830C);

// Avatar::addEventListeners $_12: __func::__clone()
static uint32_t t_5C8314(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ce30u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C8314, "Avatar::addEventListeners $_12 __func::__clone()", t_5C8314);

// Avatar::addEventListeners $_12: __func::__clone(__base*)
static void t_5C8340(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ce30u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8340, "Avatar::addEventListeners $_12 __func::__clone(__base*)", t_5C8340);

// Avatar::addEventListeners $_12: __func::destroy
static void t_5C8360(uint32_t self) {
}
RW_FUNCTION(0x5C8360, "Avatar::addEventListeners $_12 __func::destroy", t_5C8360);

// Avatar::addEventListeners $_12: __func::destroy_deallocate
static void t_5C8362(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8362, "Avatar::addEventListeners $_12 __func::destroy_deallocate", t_5C8362);

// Avatar::addEventListeners $_12: __func::target
static uint32_t t_5C8444(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe499d0u ? self + 4 : 0; }
RW_FUNCTION(0x5C8444, "Avatar::addEventListeners $_12 __func::target", t_5C8444);

// Avatar::addEventListeners $_12: __func::target_type
static uint32_t t_5C8464() { return 0xf8f3d8u; }
RW_FUNCTION(0x5C8464, "Avatar::addEventListeners $_12 __func::target_type", t_5C8464);

// Avatar::addEventListeners $_11: __func::~__func (deleting)
static void t_5C8478(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8478, "Avatar::addEventListeners $_11 __func::~__func (deleting)", t_5C8478);

// Avatar::addEventListeners $_11: __func::__clone()
static uint32_t t_5C8480(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ce5cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C8480, "Avatar::addEventListeners $_11 __func::__clone()", t_5C8480);

// Avatar::addEventListeners $_11: __func::__clone(__base*)
static void t_5C84AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ce5cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C84AC, "Avatar::addEventListeners $_11 __func::__clone(__base*)", t_5C84AC);

// Avatar::addEventListeners $_11: __func::destroy
static void t_5C84CC(uint32_t self) {
}
RW_FUNCTION(0x5C84CC, "Avatar::addEventListeners $_11 __func::destroy", t_5C84CC);

// Avatar::addEventListeners $_11: __func::destroy_deallocate
static void t_5C84CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C84CE, "Avatar::addEventListeners $_11 __func::destroy_deallocate", t_5C84CE);

// Avatar::addEventListeners $_11: __func::target
static uint32_t t_5C8500(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49a70u ? self + 4 : 0; }
RW_FUNCTION(0x5C8500, "Avatar::addEventListeners $_11 __func::target", t_5C8500);

// Avatar::addEventListeners $_11: __func::target_type
static uint32_t t_5C8520() { return 0xf8f3ecu; }
RW_FUNCTION(0x5C8520, "Avatar::addEventListeners $_11 __func::target_type", t_5C8520);

// Avatar::addEventListeners $_10: __func::~__func (deleting)
static void t_5C8534(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8534, "Avatar::addEventListeners $_10 __func::~__func (deleting)", t_5C8534);

// Avatar::addEventListeners $_10: __func::__clone()
static uint32_t t_5C853C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ce88u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C853C, "Avatar::addEventListeners $_10 __func::__clone()", t_5C853C);

// Avatar::addEventListeners $_10: __func::__clone(__base*)
static void t_5C8568(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ce88u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8568, "Avatar::addEventListeners $_10 __func::__clone(__base*)", t_5C8568);

// Avatar::addEventListeners $_10: __func::destroy
static void t_5C8588(uint32_t self) {
}
RW_FUNCTION(0x5C8588, "Avatar::addEventListeners $_10 __func::destroy", t_5C8588);

// Avatar::addEventListeners $_10: __func::destroy_deallocate
static void t_5C858A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C858A, "Avatar::addEventListeners $_10 __func::destroy_deallocate", t_5C858A);

// Avatar::addEventListeners $_10: __func::target
static uint32_t t_5C85AC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49b10u ? self + 4 : 0; }
RW_FUNCTION(0x5C85AC, "Avatar::addEventListeners $_10 __func::target", t_5C85AC);

// Avatar::addEventListeners $_10: __func::target_type
static uint32_t t_5C85CC() { return 0xf8f400u; }
RW_FUNCTION(0x5C85CC, "Avatar::addEventListeners $_10 __func::target_type", t_5C85CC);

// Avatar::addEventListeners $_9: __func::~__func (deleting)
static void t_5C85E0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C85E0, "Avatar::addEventListeners $_9 __func::~__func (deleting)", t_5C85E0);

// Avatar::addEventListeners $_9: __func::__clone()
static uint32_t t_5C85E8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ceb4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C85E8, "Avatar::addEventListeners $_9 __func::__clone()", t_5C85E8);

// Avatar::addEventListeners $_9: __func::__clone(__base*)
static void t_5C8614(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ceb4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8614, "Avatar::addEventListeners $_9 __func::__clone(__base*)", t_5C8614);

// Avatar::addEventListeners $_9: __func::destroy
static void t_5C8634(uint32_t self) {
}
RW_FUNCTION(0x5C8634, "Avatar::addEventListeners $_9 __func::destroy", t_5C8634);

// Avatar::addEventListeners $_9: __func::destroy_deallocate
static void t_5C8636(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8636, "Avatar::addEventListeners $_9 __func::destroy_deallocate", t_5C8636);

// Avatar::addEventListeners $_9: __func::target
static uint32_t t_5C867C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49bb0u ? self + 4 : 0; }
RW_FUNCTION(0x5C867C, "Avatar::addEventListeners $_9 __func::target", t_5C867C);

// Avatar::addEventListeners $_9: __func::target_type
static uint32_t t_5C869C() { return 0xf8f414u; }
RW_FUNCTION(0x5C869C, "Avatar::addEventListeners $_9 __func::target_type", t_5C869C);

// Avatar::updateNode $_8: __func::~__func (deleting)
static void t_5C8848(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8848, "Avatar::updateNode $_8 __func::~__func (deleting)", t_5C8848);

// Avatar::updateNode $_8: __func::__clone()
static uint32_t t_5C8850(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5cee0u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 16, v5);
        st32(v1 + 12, v4);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C8850, "Avatar::updateNode $_8 __func::__clone()", t_5C8850);

// Avatar::updateNode $_8: __func::__clone(__base*)
static void t_5C8884(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5cee0u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8884, "Avatar::updateNode $_8 __func::__clone(__base*)", t_5C8884);

// Avatar::updateNode $_8: __func::destroy
static void t_5C88B0(uint32_t self) {
}
RW_FUNCTION(0x5C88B0, "Avatar::updateNode $_8 __func::destroy", t_5C88B0);

// Avatar::updateNode $_8: __func::destroy_deallocate
static void t_5C88B2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C88B2, "Avatar::updateNode $_8 __func::destroy_deallocate", t_5C88B2);

// Avatar::updateNode $_8: __func::target
static uint32_t t_5C8AE0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49c50u ? self + 4 : 0; }
RW_FUNCTION(0x5C8AE0, "Avatar::updateNode $_8 __func::target", t_5C8AE0);

// Avatar::updateNode $_8: __func::target_type
static uint32_t t_5C8B00() { return 0xf8f428u; }
RW_FUNCTION(0x5C8B00, "Avatar::updateNode $_8 __func::target_type", t_5C8B00);

// Avatar::updateNode $_7: __func::~__func (deleting)
static void t_5C8B46(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8B46, "Avatar::updateNode $_7 __func::~__func (deleting)", t_5C8B46);

// Avatar::updateNode $_7: __func::__clone()
static uint32_t t_5C8B50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5cf0cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 16, v5);
        st32(v1 + 12, v4);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C8B50, "Avatar::updateNode $_7 __func::__clone()", t_5C8B50);

// Avatar::updateNode $_7: __func::__clone(__base*)
static void t_5C8B84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5cf0cu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8B84, "Avatar::updateNode $_7 __func::__clone(__base*)", t_5C8B84);

// Avatar::updateNode $_7: __func::destroy
static void t_5C8BB0(uint32_t self) {
}
RW_FUNCTION(0x5C8BB0, "Avatar::updateNode $_7 __func::destroy", t_5C8BB0);

// Avatar::updateNode $_7: __func::destroy_deallocate
static void t_5C8BB2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8BB2, "Avatar::updateNode $_7 __func::destroy_deallocate", t_5C8BB2);

// Avatar::updateNode $_7: __func::target
static uint32_t t_5C8E3C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49cf0u ? self + 4 : 0; }
RW_FUNCTION(0x5C8E3C, "Avatar::updateNode $_7 __func::target", t_5C8E3C);

// Avatar::updateNode $_7: __func::target_type
static uint32_t t_5C8E5C() { return 0xf8f43cu; }
RW_FUNCTION(0x5C8E5C, "Avatar::updateNode $_7 __func::target_type", t_5C8E5C);

// Avatar $_6: __func::~__func (deleting)
static void t_5C8E70(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8E70, "Avatar $_6 __func::~__func (deleting)", t_5C8E70);

// Avatar $_6: __func::__clone()
static uint32_t t_5C8E78(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5cf38u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C8E78, "Avatar $_6 __func::__clone()", t_5C8E78);

// Avatar $_6: __func::__clone(__base*)
static void t_5C8EA4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5cf38u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8EA4, "Avatar $_6 __func::__clone(__base*)", t_5C8EA4);

// Avatar $_6: __func::destroy
static void t_5C8EC4(uint32_t self) {
}
RW_FUNCTION(0x5C8EC4, "Avatar $_6 __func::destroy", t_5C8EC4);

// Avatar $_6: __func::destroy_deallocate
static void t_5C8EC6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8EC6, "Avatar $_6 __func::destroy_deallocate", t_5C8EC6);

// Avatar $_6: __func::target
static uint32_t t_5C8EDC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49d70u ? self + 4 : 0; }
RW_FUNCTION(0x5C8EDC, "Avatar $_6 __func::target", t_5C8EDC);

// Avatar $_6: __func::target_type
static uint32_t t_5C8EFC() { return 0xf8f450u; }
RW_FUNCTION(0x5C8EFC, "Avatar $_6 __func::target_type", t_5C8EFC);

// Avatar $_5: __func::~__func (deleting)
static void t_5C8F10(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8F10, "Avatar $_5 __func::~__func (deleting)", t_5C8F10);

// Avatar $_5: __func::__clone()
static uint32_t t_5C8F18(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5cf64u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C8F18, "Avatar $_5 __func::__clone()", t_5C8F18);

// Avatar $_5: __func::__clone(__base*)
static void t_5C8F44(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5cf64u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8F44, "Avatar $_5 __func::__clone(__base*)", t_5C8F44);

// Avatar $_5: __func::destroy
static void t_5C8F64(uint32_t self) {
}
RW_FUNCTION(0x5C8F64, "Avatar $_5 __func::destroy", t_5C8F64);

// Avatar $_5: __func::destroy_deallocate
static void t_5C8F66(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8F66, "Avatar $_5 __func::destroy_deallocate", t_5C8F66);

// Avatar $_5: __func::target
static uint32_t t_5C8F80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49de0u ? self + 4 : 0; }
RW_FUNCTION(0x5C8F80, "Avatar $_5 __func::target", t_5C8F80);

// Avatar $_5: __func::target_type
static uint32_t t_5C8FA0() { return 0xf8f464u; }
RW_FUNCTION(0x5C8FA0, "Avatar $_5 __func::target_type", t_5C8FA0);

// Avatar $_4: __func::~__func (deleting)
static void t_5C8FB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C8FB4, "Avatar $_4 __func::~__func (deleting)", t_5C8FB4);

// Avatar $_4: __func::__clone()
static uint32_t t_5C8FBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5cf90u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C8FBC, "Avatar $_4 __func::__clone()", t_5C8FBC);

// Avatar $_4: __func::__clone(__base*)
static void t_5C8FE8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5cf90u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C8FE8, "Avatar $_4 __func::__clone(__base*)", t_5C8FE8);

// Avatar $_4: __func::destroy
static void t_5C9008(uint32_t self) {
}
RW_FUNCTION(0x5C9008, "Avatar $_4 __func::destroy", t_5C9008);

// Avatar $_4: __func::destroy_deallocate
static void t_5C900A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C900A, "Avatar $_4 __func::destroy_deallocate", t_5C900A);

// Avatar $_4: __func::target
static uint32_t t_5C9020(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49e50u ? self + 4 : 0; }
RW_FUNCTION(0x5C9020, "Avatar $_4 __func::target", t_5C9020);

// Avatar $_4: __func::target_type
static uint32_t t_5C9040() { return 0xf8f478u; }
RW_FUNCTION(0x5C9040, "Avatar $_4 __func::target_type", t_5C9040);

// Avatar $_3: __func::~__func (deleting)
static void t_5C9054(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C9054, "Avatar $_3 __func::~__func (deleting)", t_5C9054);

// Avatar $_3: __func::__clone()
static uint32_t t_5C905C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5cfbcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C905C, "Avatar $_3 __func::__clone()", t_5C905C);

// Avatar $_3: __func::__clone(__base*)
static void t_5C9088(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5cfbcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C9088, "Avatar $_3 __func::__clone(__base*)", t_5C9088);

// Avatar $_3: __func::destroy
static void t_5C90A8(uint32_t self) {
}
RW_FUNCTION(0x5C90A8, "Avatar $_3 __func::destroy", t_5C90A8);

// Avatar $_3: __func::destroy_deallocate
static void t_5C90AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C90AA, "Avatar $_3 __func::destroy_deallocate", t_5C90AA);

// Avatar $_3: __func::target
static uint32_t t_5C9118(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49ec0u ? self + 4 : 0; }
RW_FUNCTION(0x5C9118, "Avatar $_3 __func::target", t_5C9118);

// Avatar $_3: __func::target_type
static uint32_t t_5C9138() { return 0xf8f48cu; }
RW_FUNCTION(0x5C9138, "Avatar $_3 __func::target_type", t_5C9138);

// Avatar $_2: __func::~__func (deleting)
static void t_5C91F4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C91F4, "Avatar $_2 __func::~__func (deleting)", t_5C91F4);

// Avatar $_2: __func::__clone()
static uint32_t t_5C91FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5cfe8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C91FC, "Avatar $_2 __func::__clone()", t_5C91FC);

// Avatar $_2: __func::__clone(__base*)
static void t_5C9228(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5cfe8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C9228, "Avatar $_2 __func::__clone(__base*)", t_5C9228);

// Avatar $_2: __func::destroy
static void t_5C9248(uint32_t self) {
}
RW_FUNCTION(0x5C9248, "Avatar $_2 __func::destroy", t_5C9248);

// Avatar $_2: __func::destroy_deallocate
static void t_5C924A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C924A, "Avatar $_2 __func::destroy_deallocate", t_5C924A);

// Avatar $_2: __func::target
static uint32_t t_5C9264(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49f30u ? self + 4 : 0; }
RW_FUNCTION(0x5C9264, "Avatar $_2 __func::target", t_5C9264);

// Avatar $_2: __func::target_type
static uint32_t t_5C9284() { return 0xf8f4a0u; }
RW_FUNCTION(0x5C9284, "Avatar $_2 __func::target_type", t_5C9284);

// Avatar $_1: __func::~__func (deleting)
static void t_5C9298(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C9298, "Avatar $_1 __func::~__func (deleting)", t_5C9298);

// Avatar $_1: __func::__clone()
static uint32_t t_5C92A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d014u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5C92A0, "Avatar $_1 __func::__clone()", t_5C92A0);

// Avatar $_1: __func::__clone(__base*)
static void t_5C92CC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d014u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5C92CC, "Avatar $_1 __func::__clone(__base*)", t_5C92CC);

// Avatar $_1: __func::destroy
static void t_5C92EC(uint32_t self) {
}
RW_FUNCTION(0x5C92EC, "Avatar $_1 __func::destroy", t_5C92EC);

// Avatar $_1: __func::destroy_deallocate
static void t_5C92EE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5C92EE, "Avatar $_1 __func::destroy_deallocate", t_5C92EE);

// Avatar $_1: __func::target
static uint32_t t_5C930C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe49fa0u ? self + 4 : 0; }
RW_FUNCTION(0x5C930C, "Avatar $_1 __func::target", t_5C930C);

// Avatar $_1: __func::target_type
static uint32_t t_5C932C() { return 0xf8f4b4u; }
RW_FUNCTION(0x5C932C, "Avatar $_1 __func::target_type", t_5C932C);
