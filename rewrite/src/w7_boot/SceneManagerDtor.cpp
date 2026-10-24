// SceneManager::~SceneManager (never runs while the game is up: the manager lives until the process ends).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t GOT_SceneManager_vtable = 0xFD6894;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release, map_string_destroy = 0x622035;
constexpr uint32_t vector_fn_b_dtor = 0x60CA61, vector_fn_bool_dtor = 0x60CAAB;   // vector<function<void(bool)>>,
                                                                                 // vector<function<bool()>>
constexpr uint32_t vector_block_dtor = 0x4DB601, vector_string_dtor = 0x47363F, vector_int_dtor = 0x478961;
constexpr uint32_t vector_message_dtor = 0x4DBBBD, Vec2_dtor_ = rwlib::cocos2d::Vec2::dtor_base, Layer_dtor = rwlib::cocos2d::Layer::dtor_base;

void freeBuffer(uint32_t p) { if (p) call(fn::operator_delete, {p}); }
void freeLongString(uint32_t s) { if (ld8(s) & 1) call(fn::operator_delete, {ld32(s + 8)}); }
}  // namespace

namespace scene_manager_dtor {

uint32_t dtor(uint32_t self) {
    st32(self, ld32(GOT_SceneManager_vtable) + 8);
    if (ld32(self + 0x4C8)) call(Ref_release, {ld32(self + 0x4C8)});
    if (ld32(self + 0x3E4)) call(Ref_release, {ld32(self + 0x3E4)});
    call(map_string_destroy, {self + 0x5CC, ld32(self + 0x5D0)});
    functionDestroy(self + 0x5B0);
    freeBuffer(ld32(self + 0x5A4));
    call(vector_fn_b_dtor, {self + 0x598});
    call(vector_fn_bool_dtor, {self + 0x58C});
    freeLongString(self + 0x57C);
    freeLongString(self + 0x560);
    for (uint32_t fnAt : {0x548u, 0x530u, 0x518u, 0x500u, 0x4E8u, 0x4D0u, 0x480u}) functionDestroy(self + fnAt);
    freeBuffer(ld32(self + 0x460));
    call(vector_block_dtor, {self + 0x454});
    call(vector_string_dtor, {self + 0x448});
    call(vector_int_dtor, {self + 0x43C});
    call(vector_message_dtor, {self + 0x430});
    functionDestroy(self + 0x3C8);
    functionDestroy(self + 0x370);
    freeLongString(self + 0x350);
    call(Vec2_dtor_, {self + 0x32C});
    call(Layer_dtor, {self});
    return self;
}
RW_FUNCTION(0x60C5E0, "SceneManager::~SceneManager", scene_manager_dtor::dtor);

}  // namespace scene_manager_dtor
