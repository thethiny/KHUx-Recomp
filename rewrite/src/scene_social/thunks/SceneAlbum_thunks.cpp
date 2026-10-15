// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneAlbum lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneAlbum::backScene $_7: __func::~__func (deleting)
static void t_6CE690(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CE690, "SceneAlbum::backScene $_7 __func::~__func (deleting)", t_6CE690);

// SceneAlbum::backScene $_7: __func::__clone()
static uint32_t t_6CE698(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6772cu);
    }
    return v1;
}
RW_FUNCTION(0x6CE698, "SceneAlbum::backScene $_7 __func::__clone()", t_6CE698);

// SceneAlbum::backScene $_7: __func::__clone(__base*)
static void t_6CE6BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6772cu);
    }
}
RW_FUNCTION(0x6CE6BC, "SceneAlbum::backScene $_7 __func::__clone(__base*)", t_6CE6BC);

// SceneAlbum::backScene $_7: __func::destroy
static void t_6CE6D8(uint32_t self) {
}
RW_FUNCTION(0x6CE6D8, "SceneAlbum::backScene $_7 __func::destroy", t_6CE6D8);

// SceneAlbum::backScene $_7: __func::destroy_deallocate
static void t_6CE6DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CE6DA, "SceneAlbum::backScene $_7 __func::destroy_deallocate", t_6CE6DA);

// SceneAlbum::backScene $_7: __func::target
static uint32_t t_6CE804(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e6b0u ? self + 4 : 0; }
RW_FUNCTION(0x6CE804, "SceneAlbum::backScene $_7 __func::target", t_6CE804);

// SceneAlbum::backScene $_7: __func::target_type
static uint32_t t_6CE824() { return 0xfa010cu; }
RW_FUNCTION(0x6CE824, "SceneAlbum::backScene $_7 __func::target_type", t_6CE824);

// SceneAlbum::openAlbumMedalPopup $_6: __func::~__func (deleting)
static void t_6CE838(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CE838, "SceneAlbum::openAlbumMedalPopup $_6 __func::~__func (deleting)", t_6CE838);

// SceneAlbum::openAlbumMedalPopup $_6: __func::__clone()
static uint32_t t_6CE840(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67758u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6CE840, "SceneAlbum::openAlbumMedalPopup $_6 __func::__clone()", t_6CE840);

// SceneAlbum::openAlbumMedalPopup $_6: __func::__clone(__base*)
static void t_6CE870(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67758u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CE870, "SceneAlbum::openAlbumMedalPopup $_6 __func::__clone(__base*)", t_6CE870);

// SceneAlbum::openAlbumMedalPopup $_6: __func::destroy
static void t_6CE898(uint32_t self) {
}
RW_FUNCTION(0x6CE898, "SceneAlbum::openAlbumMedalPopup $_6 __func::destroy", t_6CE898);

// SceneAlbum::openAlbumMedalPopup $_6: __func::destroy_deallocate
static void t_6CE89A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CE89A, "SceneAlbum::openAlbumMedalPopup $_6 __func::destroy_deallocate", t_6CE89A);

// SceneAlbum::openAlbumMedalPopup $_6: __func::target
static uint32_t t_6CE964(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e760u ? self + 4 : 0; }
RW_FUNCTION(0x6CE964, "SceneAlbum::openAlbumMedalPopup $_6 __func::target", t_6CE964);

// SceneAlbum::openAlbumMedalPopup $_6: __func::target_type
static uint32_t t_6CE984() { return 0xfa0120u; }
RW_FUNCTION(0x6CE984, "SceneAlbum::openAlbumMedalPopup $_6 __func::target_type", t_6CE984);

// SceneAlbum::openAlbumMedalPopup $_8 UlvE_: __func::~__func (deleting)
static void t_6CE998(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CE998, "SceneAlbum::openAlbumMedalPopup $_8 UlvE_ __func::~__func (deleting)", t_6CE998);

// SceneAlbum::openAlbumMedalPopup $_8 UlvE_: __func::__clone()
static uint32_t t_6CE9A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67784u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6CE9A0, "SceneAlbum::openAlbumMedalPopup $_8 UlvE_ __func::__clone()", t_6CE9A0);

// SceneAlbum::openAlbumMedalPopup $_8 UlvE_: __func::__clone(__base*)
static void t_6CE9D0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67784u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CE9D0, "SceneAlbum::openAlbumMedalPopup $_8 UlvE_ __func::__clone(__base*)", t_6CE9D0);

// SceneAlbum::openAlbumMedalPopup $_8 UlvE_: __func::destroy
static void t_6CE9F8(uint32_t self) {
}
RW_FUNCTION(0x6CE9F8, "SceneAlbum::openAlbumMedalPopup $_8 UlvE_ __func::destroy", t_6CE9F8);

// SceneAlbum::openAlbumMedalPopup $_8 UlvE_: __func::destroy_deallocate
static void t_6CE9FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CE9FA, "SceneAlbum::openAlbumMedalPopup $_8 UlvE_ __func::destroy_deallocate", t_6CE9FA);

// SceneAlbum::openAlbumMedalPopup $_8 UlvE_: __func::target
static uint32_t t_6CEA2C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e800u ? self + 4 : 0; }
RW_FUNCTION(0x6CEA2C, "SceneAlbum::openAlbumMedalPopup $_8 UlvE_ __func::target", t_6CEA2C);

// SceneAlbum::openAlbumMedalPopup $_8 UlvE_: __func::target_type
static uint32_t t_6CEA4C() { return 0xfa0134u; }
RW_FUNCTION(0x6CEA4C, "SceneAlbum::openAlbumMedalPopup $_8 UlvE_ __func::target_type", t_6CEA4C);

// SceneAlbum::runOutAnimationMain $_5: __func::~__func
static uint32_t t_6CEA60(uint32_t self) {
    st32(self, 0xf677b0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6CEA60, "SceneAlbum::runOutAnimationMain $_5 __func::~__func", t_6CEA60);

// SceneAlbum::runOutAnimationMain $_5: __func::~__func (deleting)
static void t_6CEA9C(uint32_t self) {
    st32(self, 0xf677b0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CEA9C, "SceneAlbum::runOutAnimationMain $_5 __func::~__func (deleting)", t_6CEA9C);

// SceneAlbum::runOutAnimationMain $_5: __func::__clone()
static uint32_t t_6CEADC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf677b0u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6CEADC, "SceneAlbum::runOutAnimationMain $_5 __func::__clone()", t_6CEADC);

// SceneAlbum::runOutAnimationMain $_5: __func::__clone(__base*)
static void t_6CEB2C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf677b0u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6CEB2C, "SceneAlbum::runOutAnimationMain $_5 __func::__clone(__base*)", t_6CEB2C);

// SceneAlbum::runOutAnimationMain $_5: __func::destroy
static void t_6CEB60(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6CEB60, "SceneAlbum::runOutAnimationMain $_5 __func::destroy", t_6CEB60);

// SceneAlbum::runOutAnimationMain $_5: __func::destroy_deallocate
static void t_6CEB80(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CEB80, "SceneAlbum::runOutAnimationMain $_5 __func::destroy_deallocate", t_6CEB80);

// SceneAlbum::runOutAnimationMain $_5: __func::target
static uint32_t t_6CEBB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e8b0u ? self + 8 : 0; }
RW_FUNCTION(0x6CEBB8, "SceneAlbum::runOutAnimationMain $_5 __func::target", t_6CEBB8);

// SceneAlbum::runOutAnimationMain $_5: __func::target_type
static uint32_t t_6CEBD8() { return 0xfa0148u; }
RW_FUNCTION(0x6CEBD8, "SceneAlbum::runOutAnimationMain $_5 __func::target_type", t_6CEBD8);

// SceneAlbum::init $_4: __func::~__func (deleting)
static void t_6CEBEC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CEBEC, "SceneAlbum::init $_4 __func::~__func (deleting)", t_6CEBEC);

// SceneAlbum::init $_4: __func::__clone()
static uint32_t t_6CEBF4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf677dcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CEBF4, "SceneAlbum::init $_4 __func::__clone()", t_6CEBF4);

// SceneAlbum::init $_4: __func::__clone(__base*)
static void t_6CEC20(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf677dcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CEC20, "SceneAlbum::init $_4 __func::__clone(__base*)", t_6CEC20);

// SceneAlbum::init $_4: __func::destroy
static void t_6CEC40(uint32_t self) {
}
RW_FUNCTION(0x6CEC40, "SceneAlbum::init $_4 __func::destroy", t_6CEC40);

// SceneAlbum::init $_4: __func::destroy_deallocate
static void t_6CEC42(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CEC42, "SceneAlbum::init $_4 __func::destroy_deallocate", t_6CEC42);

// SceneAlbum::init $_4: __func::target
static uint32_t t_6CEC58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e950u ? self + 4 : 0; }
RW_FUNCTION(0x6CEC58, "SceneAlbum::init $_4 __func::target", t_6CEC58);

// SceneAlbum::init $_4: __func::target_type
static uint32_t t_6CEC78() { return 0xfa015cu; }
RW_FUNCTION(0x6CEC78, "SceneAlbum::init $_4 __func::target_type", t_6CEC78);

// SceneAlbum::init $_2: __func::~__func (deleting)
static void t_6CF480(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CF480, "SceneAlbum::init $_2 __func::~__func (deleting)", t_6CF480);

// SceneAlbum::init $_2: __func::__clone()
static uint32_t t_6CF488(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67808u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CF488, "SceneAlbum::init $_2 __func::__clone()", t_6CF488);

// SceneAlbum::init $_2: __func::__clone(__base*)
static void t_6CF4B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67808u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CF4B4, "SceneAlbum::init $_2 __func::__clone(__base*)", t_6CF4B4);

// SceneAlbum::init $_2: __func::destroy
static void t_6CF4D4(uint32_t self) {
}
RW_FUNCTION(0x6CF4D4, "SceneAlbum::init $_2 __func::destroy", t_6CF4D4);

// SceneAlbum::init $_2: __func::destroy_deallocate
static void t_6CF4D6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CF4D6, "SceneAlbum::init $_2 __func::destroy_deallocate", t_6CF4D6);

// SceneAlbum::init $_2: __func::target
static uint32_t t_6CF4E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e9c0u ? self + 4 : 0; }
RW_FUNCTION(0x6CF4E8, "SceneAlbum::init $_2 __func::target", t_6CF4E8);

// SceneAlbum::init $_2: __func::target_type
static uint32_t t_6CF508() { return 0xfa0170u; }
RW_FUNCTION(0x6CF508, "SceneAlbum::init $_2 __func::target_type", t_6CF508);

// SceneAlbum::init $_1: __func::~__func (deleting)
static void t_6CF51C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CF51C, "SceneAlbum::init $_1 __func::~__func (deleting)", t_6CF51C);

// SceneAlbum::init $_1: __func::__clone()
static uint32_t t_6CF524(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67834u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CF524, "SceneAlbum::init $_1 __func::__clone()", t_6CF524);

// SceneAlbum::init $_1: __func::__clone(__base*)
static void t_6CF550(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67834u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CF550, "SceneAlbum::init $_1 __func::__clone(__base*)", t_6CF550);

// SceneAlbum::init $_1: __func::destroy
static void t_6CF570(uint32_t self) {
}
RW_FUNCTION(0x6CF570, "SceneAlbum::init $_1 __func::destroy", t_6CF570);

// SceneAlbum::init $_1: __func::destroy_deallocate
static void t_6CF572(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CF572, "SceneAlbum::init $_1 __func::destroy_deallocate", t_6CF572);

// SceneAlbum::init $_1: __func::target
static uint32_t t_6CF584(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ea30u ? self + 4 : 0; }
RW_FUNCTION(0x6CF584, "SceneAlbum::init $_1 __func::target", t_6CF584);

// SceneAlbum::init $_1: __func::target_type
static uint32_t t_6CF5A4() { return 0xfa0184u; }
RW_FUNCTION(0x6CF5A4, "SceneAlbum::init $_1 __func::target_type", t_6CF5A4);

// SceneAlbum::init $_0: __func::~__func (deleting)
static void t_6CF5B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CF5B8, "SceneAlbum::init $_0 __func::~__func (deleting)", t_6CF5B8);

// SceneAlbum::init $_0: __func::__clone()
static uint32_t t_6CF5C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67860u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CF5C0, "SceneAlbum::init $_0 __func::__clone()", t_6CF5C0);

// SceneAlbum::init $_0: __func::__clone(__base*)
static void t_6CF5EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67860u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CF5EC, "SceneAlbum::init $_0 __func::__clone(__base*)", t_6CF5EC);

// SceneAlbum::init $_0: __func::destroy
static void t_6CF60C(uint32_t self) {
}
RW_FUNCTION(0x6CF60C, "SceneAlbum::init $_0 __func::destroy", t_6CF60C);

// SceneAlbum::init $_0: __func::destroy_deallocate
static void t_6CF60E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CF60E, "SceneAlbum::init $_0 __func::destroy_deallocate", t_6CF60E);

// SceneAlbum::init $_0: __func::target
static uint32_t t_6CF620(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7eaa0u ? self + 4 : 0; }
RW_FUNCTION(0x6CF620, "SceneAlbum::init $_0 __func::target", t_6CF620);

// SceneAlbum::init $_0: __func::target_type
static uint32_t t_6CF640() { return 0xfa0198u; }
RW_FUNCTION(0x6CF640, "SceneAlbum::init $_0 __func::target_type", t_6CF640);
