// The std::function __func thunks of the DataDownloader lambdas (generated from the lifted code by a scratch tool and
// checked against it): clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "Common.hpp"

using namespace w36;

// DataDownloader::openErrorDialog $_1: __func::~__func (deleting)
static void t_7771AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7771AA, "DataDownloader::openErrorDialog $_1 __func::~__func (deleting)", t_7771AA);

// DataDownloader::openErrorDialog $_1: __func::__clone()
static uint32_t t_7771B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ec4cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7771B4, "DataDownloader::openErrorDialog $_1 __func::__clone()", t_7771B4);

// DataDownloader::openErrorDialog $_1: __func::__clone(__base*)
static void t_7771E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ec4cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7771E0, "DataDownloader::openErrorDialog $_1 __func::__clone(__base*)", t_7771E0);

// DataDownloader::openErrorDialog $_1: __func::destroy
static void t_777200(uint32_t self) {
}
RW_FUNCTION(0x777200, "DataDownloader::openErrorDialog $_1 __func::destroy", t_777200);

// DataDownloader::openErrorDialog $_1: __func::destroy_deallocate
static void t_777202(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x777202, "DataDownloader::openErrorDialog $_1 __func::destroy_deallocate", t_777202);

// DataDownloader::openErrorDialog $_1: __func::target
static uint32_t t_77726C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea8250u ? self + 4 : 0; }
RW_FUNCTION(0x77726C, "DataDownloader::openErrorDialog $_1 __func::target", t_77726C);
