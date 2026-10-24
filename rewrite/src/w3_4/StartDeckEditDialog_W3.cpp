// StartDeckEditDialog lambdas that sit next to SceneRaidList in the binary (owners.py attributes them to
// SceneRaidList): the startStory callbacks and supportScrollRunOutAnimation $_21 / $_22, a lambda capturing
// [node, Vec2 position] (its __func: vtable +0, node +4, Vec2 +8; 16 bytes) that hides the node and moves it back.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
constexpr uint32_t start_movie = 0x6C4A55;                       // Layer* start_movie(int id, bool)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t callAdventureSelectAPI = 0x71D261;            // SceneAdventureSelect::callAdventureSelectAPI(function<void(bool)>)
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;     // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t createSceneLayer_AdventureSelect = 0x579EB1;  // createSceneLayer<SceneAdventureSelect>
constexpr uint32_t VT_FuncPtrLayer = 0xF7E130;                   // __func<Layer*(*)(), ..., Layer*()> + 8
constexpr uint32_t SM_STORY_STARTED = 0x30A;                     // SceneManager byte set when the story starts
constexpr uint32_t Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;                         // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;                         // cocos2d::Vec2::~Vec2()
constexpr uint32_t LAMBDA_SIZE = 16;
constexpr uint32_t CAP_NODE = 4, CAP_POS = 8;

// __func<supportScrollRunOutAnimation $_N> slots, VTABLE = its vtable + 8
template <uint32_t VTABLE> struct RunOutLambda {
    static uint32_t destructor(uint32_t self) {
        st32(self, VTABLE);
        call(Vec2_dtor, {self + CAP_POS});
        return self;
    }
    static void deletingDestructor(uint32_t self) {
        destructor(self);
        call(fn::operator_delete, {self});
    }
    // __clone(): the node is stored before and again after the Vec2 copy (as compiled)
    static uint32_t clone(uint32_t self) {
        uint32_t copy = call(fn::operator_new, {LAMBDA_SIZE});
        if (copy) {
            st32(copy, VTABLE);
            st32(copy + CAP_NODE, ld32(self + CAP_NODE));
            call(Vec2_copy, {copy + CAP_POS, self + CAP_POS});
            st32(copy + CAP_NODE, ld32(self + CAP_NODE));
        }
        return copy;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        st32(dst, VTABLE);
        st32(dst + CAP_NODE, ld32(self + CAP_NODE));
        call(Vec2_copy, {dst + CAP_POS, self + CAP_POS});
        st32(dst + CAP_NODE, ld32(self + CAP_NODE));
    }
    static void destroy(uint32_t self) { call(Vec2_dtor, {self + CAP_POS}); }
    static void destroyDeallocate(uint32_t self) {
        call(Vec2_dtor, {self + CAP_POS});
        call(fn::operator_delete, {self});
    }
};
using RunOut22 = RunOutLambda<0xF598B0>;
using RunOut21 = RunOutLambda<0xF598DC>;
}  // namespace

// startStory $_24: the story's opening movie layer.
static uint32_t startStoryMovie(uint32_t /*func*/) { return call(start_movie, {0, 0}); }
RW_FUNCTION(0x57A81E, "StartDeckEditDialog::startStory $_24 operator()", startStoryMovie);

// startStory $_23: unless a request must be retried, refresh the adventure data and change to the adventure
// selection; marks the story as started.
static void startStoryAdventure(uint32_t /*func*/) {
    if (call(APIManager_needsRetry, {call(APIManager_getInstance, {})})) return;
    {
        Frame f;
        uint32_t none = newEmptyFunction(f);   // std::function<void(bool)>()
        call(callAdventureSelectAPI, {none});
        functionDestroy(none);
    }
    uint32_t sm = call(SceneManager_getInstance, {});
    {
        Frame f;
        uint32_t factory = newFunction(f, VT_FuncPtrLayer, {createSceneLayer_AdventureSelect});
        call(SceneManager_changeSceneLayer, {sm, factory, 1, 1, 1});
        functionDestroy(factory);
    }
    st8(call(SceneManager_getInstance, {}) + SM_STORY_STARTED, 1);
}
RW_FUNCTION(0x57A8B4, "StartDeckEditDialog::startStory $_23 operator()", startStoryAdventure);

// supportScrollRunOutAnimation $_22 / $_21 thunks
RW_FUNCTION(0x57A9E4, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::~__func", RunOut22::destructor);
RW_FUNCTION(0x57AA10, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::~__func (deleting)", RunOut22::deletingDestructor);
RW_FUNCTION(0x57AA40, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::__clone()", RunOut22::clone);
RW_FUNCTION(0x57AA98, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::__clone(__base*)", RunOut22::cloneInto);
RW_FUNCTION(0x57AAD4, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::destroy", RunOut22::destroy);
RW_FUNCTION(0x57AADE, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::destroy_deallocate", RunOut22::destroyDeallocate);
RW_FUNCTION(0x57AB4C, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::~__func", RunOut21::destructor);
RW_FUNCTION(0x57AB78, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::~__func (deleting)", RunOut21::deletingDestructor);

// supportScrollRunOutAnimation $_22: hide the node and put it back at the captured position.
static void runOutReset(uint32_t func) {
    uint32_t node = ld32(func + CAP_NODE);
    vcall(node, node::setVisible, {node, 0});
    node = ld32(func + CAP_NODE);
    vcall(node, node::setPosition, {node, func + CAP_POS});
}
RW_FUNCTION(0x57AAF4, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 operator()", runOutReset);
