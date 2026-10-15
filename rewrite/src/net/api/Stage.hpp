// Stage (the quest map: story stages, events, the current raid, the colosseum), Stage160310 and StageEvent (later
// server versions of the same lists).
#pragma once

#include "Raid.hpp"
#include "StageCommon.hpp"

namespace hole::network::api {

struct Stage {
    // A cleared / reachable story stage.
    struct Story {
        rw::Field<int32_t> stageId;           // +0
        rw::Field<int32_t> useAp;             // +4
        rw::Field<int32_t> score;             // +8
        StdVector clearMissionIds;            // +12 std::vector<int>

        // 1 when all fields are there (clearMissionIds: every entry an int), else 0
        static uint32_t init(Story *self, const JsonValue *json);
        static Story *destroy(Story *self);   // D2
    };
    // An event with its stages.
    struct Event {
        StdVector stories;                    // +0  std::vector<Story> ("stages")
        rw::Field<int32_t> eventStageId;      // +12
        rw::Field<uint8_t> showIcon;          // +16
        const uint8_t pad_17[3];
        StdString bannerUrl;                  // +20
        rw::Field<int32_t> eventType;         // +32 1..3
        rw::Field<int32_t> status;            // +36 1..2
        DateValue endTime;                    // +40

        // returns Date::init's result for endTime once everything else is in (0 before that)
        static uint32_t init(Event *self, const JsonValue *json);
    };
    struct Colosseum {
        rw::Field<int32_t> status;            // +0
        StdString message;                    // +4

        static uint32_t init(Colosseum *self, const JsonValue *json);
    };

    StdVector stories;                        // +0   std::vector<Story>
    StdVector events;                         // +12  std::vector<Event>
    Raid raid;                                // +24  Raid::init(json, 1)
    rw::Field<int32_t> newStageId;            // +344
    Colosseum colosseum;                      // +348

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(Stage::Story, clearMissionIds, 12);
RW_SIZE(Stage::Story, 24);
RW_LAYOUT(Stage::Event, bannerUrl, 20);
RW_LAYOUT(Stage::Event, endTime, 40);
RW_SIZE(Stage::Event, 48);
RW_SIZE(Stage::Colosseum, 16);
RW_LAYOUT(Stage, raid, 24);
RW_LAYOUT(Stage, newStageId, 344);
RW_LAYOUT(Stage, colosseum, 348);
RW_SIZE(Stage, 0x170);

struct Stage160310 {
    StdVector stories;                        // +0 std::vector<Stage::Story>
    rw::Field<uint32_t> newStageId;           // +12 (unsigned in the JSON)

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(Stage160310, 16);

struct StageEvent {
    struct Event : Stage::Event {
        rw::Field<int32_t> newStageId;        // +48
        rw::Field<uint8_t> isLock;            // +52 (unsigned in the JSON)
        const uint8_t pad_53[3];

        static uint32_t init(Event *self, const JsonValue *json);
    };

    StdVector events;                         // +0 std::vector<StageEvent::Event>
    rw::Field<uint32_t> raidStatus;           // +12 (unsigned in the JSON)

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(StageEvent::Event, newStageId, 48);
RW_SIZE(StageEvent::Event, 56);
RW_SIZE(StageEvent, 16);

}  // namespace hole::network::api
