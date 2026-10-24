// The login bonus scene's lambdas: initLoginBonus $_8 (the stamp sheet LWF: show the stamp of today's count on
// the movie and its partner) and its clone, skipEvent $_7 (leave once: to the union ranking when the API has
// ranking results to show, else to MyPage / the interrupted quest) and runInAnimation $_4 (open the bonus popup).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t LWF_Movie_GotoAndStop = rwlib::LWF::Movie::GotoAndStop;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneUtil_moveToMyPageOrResumeStage = 0x46F161;
constexpr uint32_t SceneLoginBonus_openLoginBonus = 0x6FA1A1;     // (scene, function<void()>)
constexpr uint32_t GOT_FuncLayerPtr_vt = 0xFD6074;   // &vtable of __func<Layer*(*)(), ..., Layer*()>
constexpr uint32_t GOT_createUnionRanking = 0xFD695C;   // &createSceneLayer<SceneUnionRanking>
}  // namespace g

constexpr uint32_t VT_StampSheet = 0xF69F38;   // initLoginBonus $_8 {shared_ptr<bonus>}
constexpr uint32_t BONUS_DAY = 0x98;           // the login bonus record: days logged in
constexpr uint32_t MOVIE_ATTACHED = 0xAD;      // LWF::Movie: byte, +8 its partner movie
constexpr uint32_t F_LEAVING = 0x2D8;          // SceneLoginBonus: leaving already started
constexpr uint32_t API_UNION_RANKING = 0x2EC;  // vector* of union ranking results to show

struct LoginBonusLambdas {
    // $_8 (LWF::Movie*) {bonus}: stop the sheet and its partner on today's stamp
    static void stampSheet(uint32_t functor, uint32_t movieRef) {
        uint32_t movie = ld32(movieRef);
        if (!ld8(movie + MOVIE_ATTACHED)) return;
        call(g::LWF_Movie_GotoAndStop, {movie, ld32(ld32(functor + 4) + BONUS_DAY) - 1});
        call(g::LWF_Movie_GotoAndStop, {ld32(movie + 8), ld32(ld32(functor + 4) + BONUS_DAY) - 1});
    }

    // $_8 __clone(__base*): the shared_ptr copied (one more owner)
    static void cloneStampSheet(uint32_t self, uint32_t dst) {
        if (!dst) return;
        st32(dst, VT_StampSheet);
        st32(dst + 4, ld32(self + 4));
        uint32_t ctrl = ld32(self + 8);
        st32(dst + 8, ctrl);
        if (ctrl) call(fn::shared_add, {ctrl});
    }

    // skipEvent $_7 {scene}
    static void leave(uint32_t functor) {
        uint32_t scene = ld32(functor + 4);
        if (ld8(scene + F_LEAVING)) return;
        st8(scene + F_LEAVING, 1);
        uint32_t ranking = ld32(call(fn::APIManager_getInstance, {}) + API_UNION_RANKING);
        if (ld32(ranking + 4) == ld32(ranking)) {
            call(g::SceneUtil_moveToMyPageOrResumeStage, {});
            return;
        }
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t layer = newFunction(f, ld32(g::GOT_FuncLayerPtr_vt) + 8, {ld32(g::GOT_createUnionRanking)});
        call(g::SceneManager_changeSceneLayer, {mgr, layer, 1, 1, 1});
        functionDestroy(layer);
    }

    // runInAnimation $_4 {scene}: the bonus popup, no callback
    static void openBonus(uint32_t functor) {
        uint32_t scene = ld32(functor + 4);
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(g::SceneLoginBonus_openLoginBonus, {scene, none});
        functionDestroy(none);
    }
};
}  // namespace

RW_FUNCTION(0x6FAC28, "SceneLoginBonus::initLoginBonus $_8 __func::operator()(LWF::Movie*) (sub_6FAC28)",
            LoginBonusLambdas::stampSheet);
RW_FUNCTION(0x6FABD4, "SceneLoginBonus::initLoginBonus $_8 __func::__clone(__base*) (sub_6FABD4)",
            LoginBonusLambdas::cloneStampSheet);
RW_FUNCTION(0x6FACEC, "SceneLoginBonus::skipEvent $_7 __func::operator() (sub_6FACEC)", LoginBonusLambdas::leave);
RW_FUNCTION(0x6FB144, "SceneLoginBonus::runInAnimation $_4 __func::operator() (sub_6FB144)", LoginBonusLambdas::openBonus);
