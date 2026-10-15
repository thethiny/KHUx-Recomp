// Campaign::initCampaign: before a stage starts, the running event campaigns (EvCampaign master rows of the
// campaign IDs the server listed) that apply to the stage are shown in a CampaignAnimationView added to `parent`.
// Which campaigns apply depends on the stage type: a filter lambda per type gets the stage's key (world / event /
// colosseum / raid id) and the campaign row.
#include "Common.hpp"

using namespace w36;

struct Campaign {
    static void initCampaign(uint32_t parent, uint32_t stageType, uint32_t index);
};

namespace {
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;        // std::__vector_base_common::__throw_out_of_range (vector::at)
constexpr uint32_t FUNC_FILTER_MOVE_ASSIGN = 0x474425;   // function<bool(int, shared_ptr<const EvCampaign>&)>::operator=(&&)
constexpr uint32_t FUNC_FILTER_CALL = 0x474475;          // function<bool(int, shared_ptr<const EvCampaign>&)>::operator()
constexpr uint32_t EvCampaign_create = 0x781DD9;         // master::EvCampaign::create(shared_ptr out, int id)
constexpr uint32_t VECTOR_PUSH_SLOW = 0x599229;          // vector<shared_ptr<const EvCampaign>>::__push_back_slow_path
constexpr uint32_t VECTOR_DTOR = 0x573591;               // __vector_base<shared_ptr<const EvCampaign>>::~__vector_base
constexpr uint32_t CampaignAnimationView_create = 0x598AF1;   // (Node* parent, vector<shared_ptr<const EvCampaign>>&)
constexpr uint32_t SCENE_MANAGER_WORLD = 0x2C8;          // SceneManager: the selected world / story id
constexpr uint32_t API_EVENT_LIST = 0x24C;               // APIManager: event list (16-byte entries at +8.., id at +8)
constexpr uint32_t API_COLOSSEUM = 0x14C;                // APIManager: colosseum info (id at +0x2C)
constexpr uint32_t API_RAIDS = 0x154;                    // APIManager: vector of 240-byte raid entries (id at +0x2C)
constexpr uint32_t API_CAMPAIGN_IDS = 0x2FC;             // APIManager: vector<int> of running campaign ids
// the filter lambdas (std::function __func vtables, no captures)
constexpr uint32_t VT_FILTER[5] = {0xF5B220, 0xF5B1F4, 0xF5B1C8, 0xF5B19C, 0xF5B170};
}  // namespace

void Campaign::initCampaign(uint32_t parent, uint32_t stageType, uint32_t index) {
    Frame f;
    const uint32_t filter = f.alloc(24);       // std::function<bool(int, shared_ptr<const EvCampaign>&)>
    st32(filter + 16, 0);
    if (stageType > 4) return;
    uint32_t key = 0;
    switch (stageType) {
    case 0:
    case 1:
        key = ld32(call(SceneManager_getInstance, {}) + SCENE_MANAGER_WORLD);
        break;
    case 2: {
        uint32_t list = ld32(call(APIManager_getInstance, {}) + API_EVENT_LIST);
        uint32_t begin = ld32(list + 8);
        if (uint32_t(int32_t(ld32(list + 0xC) - begin) >> 4) <= index) call(THROW_OUT_OF_RANGE, {list + 8});
        key = ld32(begin + 16 * index + 8);
        break;
    }
    case 3:
        key = ld32(ld32(call(APIManager_getInstance, {}) + API_COLOSSEUM) + 0x2C);
        break;
    case 4: {
        uint32_t a = call(APIManager_getInstance, {});
        uint32_t begin = ld32(a + API_RAIDS);
        uint32_t count = uint32_t(int32_t(ld32(a + API_RAIDS + 4) - begin) >> 4) * 0xEEEEEEEFu;   // / 240
        if (count <= index) call(THROW_OUT_OF_RANGE, {a + API_RAIDS});
        key = ld32(begin + 240 * index + 0x2C);
        break;
    }
    }
    uint32_t lambda = newFunction(f, VT_FILTER[stageType], {});
    call(FUNC_FILTER_MOVE_ASSIGN, {filter, lambda});
    functionDestroy(lambda);
    if (ld32(filter + 16) == 0) return;

    const uint32_t campaigns = f.alloc(12);    // vector<shared_ptr<const EvCampaign>>
    st32(campaigns, 0);
    st32(campaigns + 4, 0);
    st32(campaigns + 8, 0);
    const uint32_t row = f.alloc(8);           // shared_ptr<const EvCampaign>
    const uint32_t a = call(APIManager_getInstance, {});
    const uint32_t ids = a + API_CAMPAIGN_IDS;
    if (ld32(ids + 4) != ld32(ids)) {
        uint32_t k = 0;
        do {
            call(EvCampaign_create, {row, ld32(ld32(ids) + 4 * k)});
            if (ld32(row) && call(FUNC_FILTER_CALL, {filter, key, row}) == 1) {
                uint32_t end = ld32(campaigns + 4);
                if (end == ld32(campaigns + 8)) {
                    call(VECTOR_PUSH_SLOW, {campaigns, row});
                } else {
                    if (end) {
                        st32(end, ld32(row));
                        uint32_t ctrl = ld32(row + 4);
                        st32(end + 4, ctrl);
                        if (ctrl) call(fn::shared_add, {ctrl});
                    }
                    st32(campaigns + 4, ld32(campaigns + 4) + 8);
                }
            }
            sharedRelease(row);
            k++;
        } while (k < uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2));
    }
    uint32_t view = call(CampaignAnimationView_create, {parent, campaigns});
    vcall(parent, node::addChild, {parent, view});
    call(VECTOR_DTOR, {campaigns});
    functionDestroy(filter);
}
RW_FUNCTION(0x598F30, "Campaign::initCampaign", Campaign::initCampaign);
