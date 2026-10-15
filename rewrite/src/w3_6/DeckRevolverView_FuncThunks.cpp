// The std::function __func thunks of the DeckRevolverView lambdas (generated from the lifted code by a scratch tool and
// checked against it): clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "Common.hpp"

using namespace w36;

// DeckRevolverView::setMedalData $_7: __func::~__func
static uint32_t t_52E10C(uint32_t self) {
    st32(self, 0xf57180u);
    uint32_t v1 = ld32(self + 32);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x52E10C, "DeckRevolverView::setMedalData $_7 __func::~__func", t_52E10C);

// DeckRevolverView::setMedalData $_7: __func::~__func (deleting)
static void t_52E138(uint32_t self) {
    st32(self, 0xf57180u);
    uint32_t v1 = ld32(self + 32);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E138, "DeckRevolverView::setMedalData $_7 __func::~__func (deleting)", t_52E138);

// DeckRevolverView::setMedalData $_7: __func::__clone()
static uint32_t t_52E168(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf57180u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        st32(v1 + 8 + 12, v5);
        uint32_t v6 = ld32(self + 24);
        st32(v1 + 24, v6);
        uint32_t v7 = ld32(self + 28);
        st32(v1 + 28, v7);
        uint32_t v8 = ld32(self + 32);
        st32(v1 + 32, v8);
        if (v8) {
            call(0xc8dbb5, {v8});   // sub_C8DBB4
        }
        uint32_t v9 = ld32(self + 40);
        uint32_t v10 = ld32(self + 44);
        st32(v1 + 40, v9);
        st32(v1 + 44, v10);
    }
    return v1;
}
RW_FUNCTION(0x52E168, "DeckRevolverView::setMedalData $_7 __func::__clone()", t_52E168);

// DeckRevolverView::setMedalData $_7: __func::__clone(__base*)
static void t_52E1C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf57180u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        uint32_t v5 = ld32(self + 24);
        st32(p + 24, v5);
        uint32_t v6 = ld32(self + 28);
        st32(p + 28, v6);
        uint32_t v7 = ld32(self + 32);
        st32(p + 32, v7);
        if (v7) {
            call(0xc8dbb5, {v7});   // sub_C8DBB4
        }
        uint32_t v8 = ld32(self + 40);
        uint32_t v9 = ld32(self + 44);
        st32(p + 40, v8);
        st32(p + 44, v9);
    }
}
RW_FUNCTION(0x52E1C0, "DeckRevolverView::setMedalData $_7 __func::__clone(__base*)", t_52E1C0);

// DeckRevolverView::setMedalData $_7: __func::destroy
static void t_52E20C(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x52E20C, "DeckRevolverView::setMedalData $_7 __func::destroy", t_52E20C);

// DeckRevolverView::setMedalData $_7: __func::destroy_deallocate
static void t_52E21A(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E21A, "DeckRevolverView::setMedalData $_7 __func::destroy_deallocate", t_52E21A);

// DeckRevolverView::setMedalData $_7: __func::target
static uint32_t t_52E448(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe27300u ? self + 8 : 0; }
RW_FUNCTION(0x52E448, "DeckRevolverView::setMedalData $_7 __func::target", t_52E448);

// DeckRevolverView::setMedalData $_7: __func::target_type
static uint32_t t_52E46C() { return 0xf861f8u; }
RW_FUNCTION(0x52E46C, "DeckRevolverView::setMedalData $_7 __func::target_type", t_52E46C);

// DeckRevolverView::init $_6: __func::~__func (deleting)
static void t_52E640(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E640, "DeckRevolverView::init $_6 __func::~__func (deleting)", t_52E640);

// DeckRevolverView::init $_6: __func::__clone()
static uint32_t t_52E648(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf571acu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x52E648, "DeckRevolverView::init $_6 __func::__clone()", t_52E648);

// DeckRevolverView::init $_6: __func::__clone(__base*)
static void t_52E674(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf571acu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x52E674, "DeckRevolverView::init $_6 __func::__clone(__base*)", t_52E674);

// DeckRevolverView::init $_6: __func::destroy
static void t_52E694(uint32_t self) {
}
RW_FUNCTION(0x52E694, "DeckRevolverView::init $_6 __func::destroy", t_52E694);

// DeckRevolverView::init $_6: __func::destroy_deallocate
static void t_52E696(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E696, "DeckRevolverView::init $_6 __func::destroy_deallocate", t_52E696);

// DeckRevolverView::init $_6: __func::target
static uint32_t t_52E6A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe27430u ? self + 4 : 0; }
RW_FUNCTION(0x52E6A8, "DeckRevolverView::init $_6 __func::target", t_52E6A8);

// DeckRevolverView::init $_6: __func::target_type
static uint32_t t_52E6C8() { return 0xf8620cu; }
RW_FUNCTION(0x52E6C8, "DeckRevolverView::init $_6 __func::target_type", t_52E6C8);

// DeckRevolverView::init $_5: __func::~__func (deleting)
static void t_52E6DC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E6DC, "DeckRevolverView::init $_5 __func::~__func (deleting)", t_52E6DC);

// DeckRevolverView::init $_5: __func::__clone()
static uint32_t t_52E6E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf571d8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x52E6E4, "DeckRevolverView::init $_5 __func::__clone()", t_52E6E4);

// DeckRevolverView::init $_5: __func::__clone(__base*)
static void t_52E710(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf571d8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x52E710, "DeckRevolverView::init $_5 __func::__clone(__base*)", t_52E710);

// DeckRevolverView::init $_5: __func::destroy
static void t_52E730(uint32_t self) {
}
RW_FUNCTION(0x52E730, "DeckRevolverView::init $_5 __func::destroy", t_52E730);

// DeckRevolverView::init $_5: __func::destroy_deallocate
static void t_52E732(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E732, "DeckRevolverView::init $_5 __func::destroy_deallocate", t_52E732);

// DeckRevolverView::init $_5: __func::target
static uint32_t t_52E744(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe27580u ? self + 4 : 0; }
RW_FUNCTION(0x52E744, "DeckRevolverView::init $_5 __func::target", t_52E744);

// DeckRevolverView::init $_5: __func::target_type
static uint32_t t_52E764() { return 0xf86220u; }
RW_FUNCTION(0x52E764, "DeckRevolverView::init $_5 __func::target_type", t_52E764);

// DeckRevolverView::init $_4: __func::~__func (deleting)
static void t_52E778(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E778, "DeckRevolverView::init $_4 __func::~__func (deleting)", t_52E778);

// DeckRevolverView::init $_4: __func::__clone()
static uint32_t t_52E780(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf57204u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x52E780, "DeckRevolverView::init $_4 __func::__clone()", t_52E780);

// DeckRevolverView::init $_4: __func::__clone(__base*)
static void t_52E7AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf57204u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x52E7AC, "DeckRevolverView::init $_4 __func::__clone(__base*)", t_52E7AC);

// DeckRevolverView::init $_4: __func::destroy
static void t_52E7CC(uint32_t self) {
}
RW_FUNCTION(0x52E7CC, "DeckRevolverView::init $_4 __func::destroy", t_52E7CC);

// DeckRevolverView::init $_4: __func::destroy_deallocate
static void t_52E7CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E7CE, "DeckRevolverView::init $_4 __func::destroy_deallocate", t_52E7CE);

// DeckRevolverView::init $_4: __func::target
static uint32_t t_52E92C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe276f0u ? self + 4 : 0; }
RW_FUNCTION(0x52E92C, "DeckRevolverView::init $_4 __func::target", t_52E92C);

// DeckRevolverView::init $_4: __func::target_type
static uint32_t t_52E94C() { return 0xf86234u; }
RW_FUNCTION(0x52E94C, "DeckRevolverView::init $_4 __func::target_type", t_52E94C);

// DeckRevolverView::init $_4 UlvE_: __func::~__func (deleting)
static void t_52E960(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E960, "DeckRevolverView::init $_4 UlvE_ __func::~__func (deleting)", t_52E960);

// DeckRevolverView::init $_4 UlvE_: __func::__clone()
static uint32_t t_52E968(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf57230u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x52E968, "DeckRevolverView::init $_4 UlvE_ __func::__clone()", t_52E968);

// DeckRevolverView::init $_4 UlvE_: __func::__clone(__base*)
static void t_52E994(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf57230u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x52E994, "DeckRevolverView::init $_4 UlvE_ __func::__clone(__base*)", t_52E994);

// DeckRevolverView::init $_4 UlvE_: __func::destroy
static void t_52E9B4(uint32_t self) {
}
RW_FUNCTION(0x52E9B4, "DeckRevolverView::init $_4 UlvE_ __func::destroy", t_52E9B4);

// DeckRevolverView::init $_4 UlvE_: __func::destroy_deallocate
static void t_52E9B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E9B6, "DeckRevolverView::init $_4 UlvE_ __func::destroy_deallocate", t_52E9B6);

// DeckRevolverView::init $_4 UlvE_: __func::target
static uint32_t t_52E9C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe27860u ? self + 4 : 0; }
RW_FUNCTION(0x52E9C8, "DeckRevolverView::init $_4 UlvE_ __func::target", t_52E9C8);

// DeckRevolverView::init $_4 UlvE_: __func::target_type
static uint32_t t_52E9E8() { return 0xf86248u; }
RW_FUNCTION(0x52E9E8, "DeckRevolverView::init $_4 UlvE_ __func::target_type", t_52E9E8);

// DeckRevolverView::init $_3: __func::~__func (deleting)
static void t_52E9FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52E9FC, "DeckRevolverView::init $_3 __func::~__func (deleting)", t_52E9FC);

// DeckRevolverView::init $_3: __func::__clone()
static uint32_t t_52EA04(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5725cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x52EA04, "DeckRevolverView::init $_3 __func::__clone()", t_52EA04);

// DeckRevolverView::init $_3: __func::__clone(__base*)
static void t_52EA30(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5725cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x52EA30, "DeckRevolverView::init $_3 __func::__clone(__base*)", t_52EA30);

// DeckRevolverView::init $_3: __func::destroy
static void t_52EA50(uint32_t self) {
}
RW_FUNCTION(0x52EA50, "DeckRevolverView::init $_3 __func::destroy", t_52EA50);

// DeckRevolverView::init $_3: __func::destroy_deallocate
static void t_52EA52(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52EA52, "DeckRevolverView::init $_3 __func::destroy_deallocate", t_52EA52);

// DeckRevolverView::init $_3: __func::target
static uint32_t t_52F2FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe279f0u ? self + 4 : 0; }
RW_FUNCTION(0x52F2FC, "DeckRevolverView::init $_3 __func::target", t_52F2FC);

// DeckRevolverView::init $_3: __func::target_type
static uint32_t t_52F31C() { return 0xf8625cu; }
RW_FUNCTION(0x52F31C, "DeckRevolverView::init $_3 __func::target_type", t_52F31C);

// DeckRevolverView::init $_3 UlvE2_: __func::~__func (deleting)
static void t_52F330(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52F330, "DeckRevolverView::init $_3 UlvE2_ __func::~__func (deleting)", t_52F330);

// DeckRevolverView::init $_3 UlvE2_: __func::__clone()
static uint32_t t_52F338(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf57288u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x52F338, "DeckRevolverView::init $_3 UlvE2_ __func::__clone()", t_52F338);

// DeckRevolverView::init $_3 UlvE2_: __func::__clone(__base*)
static void t_52F364(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf57288u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x52F364, "DeckRevolverView::init $_3 UlvE2_ __func::__clone(__base*)", t_52F364);

// DeckRevolverView::init $_3 UlvE2_: __func::destroy
static void t_52F384(uint32_t self) {
}
RW_FUNCTION(0x52F384, "DeckRevolverView::init $_3 UlvE2_ __func::destroy", t_52F384);

// DeckRevolverView::init $_3 UlvE2_: __func::destroy_deallocate
static void t_52F386(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52F386, "DeckRevolverView::init $_3 UlvE2_ __func::destroy_deallocate", t_52F386);

// DeckRevolverView::init $_3 UlvE2_: __func::target
static uint32_t t_52F468(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe27b60u ? self + 4 : 0; }
RW_FUNCTION(0x52F468, "DeckRevolverView::init $_3 UlvE2_ __func::target", t_52F468);

// DeckRevolverView::init $_3 UlvE2_: __func::target_type
static uint32_t t_52F488() { return 0xf86270u; }
RW_FUNCTION(0x52F488, "DeckRevolverView::init $_3 UlvE2_ __func::target_type", t_52F488);

// DeckRevolverView::init $_3 UlvE1_: __func::~__func (deleting)
static void t_52F49C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52F49C, "DeckRevolverView::init $_3 UlvE1_ __func::~__func (deleting)", t_52F49C);

// DeckRevolverView::init $_3 UlvE1_: __func::__clone()
static uint32_t t_52F4A4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf572b4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x52F4A4, "DeckRevolverView::init $_3 UlvE1_ __func::__clone()", t_52F4A4);

// DeckRevolverView::init $_3 UlvE1_: __func::__clone(__base*)
static void t_52F4D4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf572b4u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x52F4D4, "DeckRevolverView::init $_3 UlvE1_ __func::__clone(__base*)", t_52F4D4);

// DeckRevolverView::init $_3 UlvE1_: __func::destroy
static void t_52F4FC(uint32_t self) {
}
RW_FUNCTION(0x52F4FC, "DeckRevolverView::init $_3 UlvE1_ __func::destroy", t_52F4FC);

// DeckRevolverView::init $_3 UlvE1_: __func::destroy_deallocate
static void t_52F4FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52F4FE, "DeckRevolverView::init $_3 UlvE1_ __func::destroy_deallocate", t_52F4FE);

// DeckRevolverView::init $_3 UlvE1_: __func::target
static uint32_t t_52F720(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe27cf0u ? self + 4 : 0; }
RW_FUNCTION(0x52F720, "DeckRevolverView::init $_3 UlvE1_ __func::target", t_52F720);

// DeckRevolverView::init $_3 UlvE1_: __func::target_type
static uint32_t t_52F740() { return 0xf86284u; }
RW_FUNCTION(0x52F740, "DeckRevolverView::init $_3 UlvE1_ __func::target_type", t_52F740);

// DeckRevolverView::init $_3 UlvE0_: __func::~__func
static uint32_t t_52F754(uint32_t self) {
    st32(self, 0xf572e0u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x52F754, "DeckRevolverView::init $_3 UlvE0_ __func::~__func", t_52F754);

// DeckRevolverView::init $_3 UlvE0_: __func::~__func (deleting)
static void t_52F780(uint32_t self) {
    st32(self, 0xf572e0u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52F780, "DeckRevolverView::init $_3 UlvE0_ __func::~__func (deleting)", t_52F780);

// DeckRevolverView::init $_3 UlvE0_: __func::__clone()
static uint32_t t_52F7B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf572e0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t m6 = v2;
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
            uint32_t v5 = ld32(self + 4);
            m6 = v5;
        }
        st32(v1 + 4, m6);
    }
    return v1;
}
RW_FUNCTION(0x52F7B0, "DeckRevolverView::init $_3 UlvE0_ __func::__clone()", t_52F7B0);

// DeckRevolverView::init $_3 UlvE0_: __func::__clone(__base*)
static void t_52F7F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf572e0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 4);
        st32(p + 4, v4);
    }
}
RW_FUNCTION(0x52F7F8, "DeckRevolverView::init $_3 UlvE0_ __func::__clone(__base*)", t_52F7F8);

// DeckRevolverView::init $_3 UlvE0_: __func::destroy
static void t_52F834(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x52F834, "DeckRevolverView::init $_3 UlvE0_ __func::destroy", t_52F834);

// DeckRevolverView::init $_3 UlvE0_: __func::destroy_deallocate
static void t_52F842(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x52F842, "DeckRevolverView::init $_3 UlvE0_ __func::destroy_deallocate", t_52F842);

// DeckRevolverView::init $_3 UlvE0_: __func::target
static uint32_t t_52F88C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe27eb0u ? self + 4 : 0; }
RW_FUNCTION(0x52F88C, "DeckRevolverView::init $_3 UlvE0_ __func::target", t_52F88C);

// DeckRevolverView::init $_3 UlvE0_: __func::target_type
static uint32_t t_52F8AC() { return 0xf86298u; }
RW_FUNCTION(0x52F8AC, "DeckRevolverView::init $_3 UlvE0_ __func::target_type", t_52F8AC);
