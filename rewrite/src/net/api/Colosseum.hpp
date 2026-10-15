// Colosseum responses: the colosseum overview (Colosseum), the start of a colosseum run (ColosseumStart) and
// its result (ColosseumClear).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

// The colosseum overview: response["colosseumInfo"], the stage list response["colosseumStages"] (skipped when
// the caller only wants the info) and the player's state response["userColosseum"].
struct Colosseum {
    // {"colosseumStageId", "status"}
    struct Info {
        rw::Field<int32_t> colosseumStageId;   // +0
        rw::Field<int32_t> status;             // +4

        // 1 when both are there (fields set so far stay), else 0
        static uint32_t init(Info *self, const JsonValue *json);
    };

    // one colosseum stage: {"rank", "stageCount", "stageId", "useAp"}
    struct Stage {
        rw::Field<int32_t> rank;               // +0
        rw::Field<int32_t> stageCount;         // +4
        rw::Field<int32_t> stageId;            // +8
        rw::Field<int32_t> useAp;              // +12

        static uint32_t init(Stage *self, const JsonValue *json);
    };

    // the player's colosseum progress: {"userRank", "userStageCount", "userStageTotalCount"}
    struct UserState {
        rw::Field<int32_t> userRank;            // +0
        rw::Field<int32_t> userStageCount;      // +4
        rw::Field<int32_t> userStageTotalCount; // +8

        static uint32_t init(UserState *self, const JsonValue *json);
    };

    Info info;                 // +0  (not initialised before parsing)
    StdVector stages;          // +8  std::vector<Stage>
    UserState user;            // +20

    // response -> *out; null when a part is missing. infoOnly != 0 skips "colosseumStages".
    static uint32_t parse(SharedPtr *out, const JsonValue *response, uint32_t infoOnly);
};
RW_SIZE(Colosseum::Info, 8);
RW_SIZE(Colosseum::Stage, 16);
RW_SIZE(Colosseum::UserState, 12);
RW_LAYOUT(Colosseum, stages, 8);
RW_LAYOUT(Colosseum, user, 20);
RW_SIZE(Colosseum, 32);

// The start of a colosseum run: the treasures of the run (a SafeBuffer of StageStart::Treasure records, 24
// bytes each), the run's request data and the active campaign ids.
struct ColosseumStart : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F3CC;     // ColosseumStart vtable (+8)
    static constexpr uint32_t TREASURE = 24;          // StageStart::Treasure record size (SafeBuffer stride)

    // response["startColosseumData"]: {"rank", "stageCount", "stageId", "userKeybladeId"}
    struct RequestData {
        rw::Field<int32_t> rank;               // +0
        rw::Field<int32_t> stageCount;         // +4
        rw::Field<int32_t> stageId;            // +8
        const uint8_t pad_12[4];
        rw::Field<uint64_t> userKeybladeId;    // +16

        static uint32_t init(RequestData *self, const JsonValue *json);
    };

    rw::Field<uint32_t> treasureCount;         // +32 records in the SafeBuffer
    const uint8_t pad_36[4];
    RequestData request;                       // +40
    StdVector campaigns;                       // +64 std::vector<int>

    // {"userTreasures": [...], "startColosseumData": {...}, "campaigns": [int...]} -> *out
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static ColosseumStart *destroy(ColosseumStart *self);       // ~ColosseumStart (D2)
    static uint32_t destroyDelete(ColosseumStart *self);        // ~ColosseumStart + operator delete (D0)
};
RW_SIZE(ColosseumStart::RequestData, 24);
RW_LAYOUT(ColosseumStart, treasureCount, 32);
RW_LAYOUT(ColosseumStart, request, 40);
RW_LAYOUT(ColosseumStart, campaigns, 64);
RW_SIZE(ColosseumStart, 80);

// The result of a colosseum run: the cleared colosseum ranks and the player's updated items.
struct ColosseumClear {
    // one entry of response["clearColosseums"]: {"rank", "stageCount"}
    struct ClearColosseum {
        rw::Field<int32_t> rank;               // +0
        rw::Field<int32_t> stageCount;         // +4

        static uint32_t init(ClearColosseum *self, const JsonValue *json);
    };

    StdVector clears;                          // +0  std::vector<ClearColosseum>
    SharedPtr userMaterial;                    // +12
    SharedPtr userMedal;                       // +20
    SharedPtr userSkill;                       // +28
    SharedPtr userTitle;                       // +36
    SharedPtr userKeyblade;                    // +44
    SharedPtr stageClearProperty[5];           // +52 StageClear::Property (five shared_ptrs, filled by its init)

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static ColosseumClear *destroy(ColosseumClear *self);       // ~ColosseumClear (D2)
};
RW_LAYOUT(ColosseumClear, userMaterial, 12);
RW_LAYOUT(ColosseumClear, userKeyblade, 44);
RW_LAYOUT(ColosseumClear, stageClearProperty, 52);
RW_SIZE(ColosseumClear, 92);

}  // namespace hole::network::api
