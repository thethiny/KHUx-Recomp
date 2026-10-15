#include "Stage.hpp"

namespace hole::network::api {

namespace {
using stage::fn::vector_int_copy;
using stage::fn::vector_int_base_dtor;
constexpr uint32_t STORY_PUSH_SLOW = 0x4B5C8D;        // std::vector<Stage::Story>::__push_back_slow_path(const Story&)
constexpr uint32_t STORY_VECTOR_COPY = 0x4B5BC9;      // std::vector<Stage::Story>::vector(const vector&)
constexpr uint32_t STORY_VECTOR_BASE_DTOR = 0x4B58EF; // std::__vector_base<Stage::Story>::~__vector_base
constexpr uint32_t EVENT_PUSH_SLOW = 0x4B5EA9;        // std::vector<Stage::Event>::__push_back_slow_path
constexpr uint32_t EVENT_VECTOR_BASE_DTOR = 0x4B5E59; // std::__vector_base<Stage::Event>::~__vector_base
constexpr uint32_t STAGE_SHARED_PTR_CTOR = 0x4A3CA1;  // std::shared_ptr<Stage>::shared_ptr(Stage*)
constexpr uint32_t S160310_SHARED_PTR_CTOR = 0x4A3EA1;   // std::shared_ptr<Stage160310>::shared_ptr(...)
constexpr uint32_t SEVENT_PUSH_SLOW = 0x4B5925;       // std::vector<StageEvent::Event>::__push_back_slow_path
constexpr uint32_t SEVENT_VECTOR_BASE_DTOR = 0x4B58A1;   // std::__vector_base<StageEvent::Event>::~__vector_base
constexpr uint32_t SEVENT_SHARED_PTR_CTOR = 0x4A4175; // std::shared_ptr<StageEvent>::shared_ptr(StageEvent*)

bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}

// Append the Story local to `list` (push_back of a const Story&: in place when there is room)
void pushStory(StdVector *list, Stage::Story *local) {
    uint32_t end = list->end;
    if (end == list->capEnd) {
        rw::call(STORY_PUSH_SLOW, {rw::guest_addr(list), rw::guest_addr(local)});
        return;
    }
    uint32_t base = 0;
    if (end) {
        raid::copyWords(end, rw::guest_addr(local), 3);
        rw::call(vector_int_copy, {end + 12, rw::guest_addr(&local->clearMissionIds)});
        base = list->end;
    }
    list->end = base + sizeof(Stage::Story);
}

// A list of stories through the Story local (Stage::parse, Stage160310::parse, Stage::Event::init)
bool readStories(StdVector *list, const JsonValue &array, Stage::Story *local) {
    uint32_t n = array.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &entry = array.At(k);
        if (!entry.IsObject()) return false;
        local->clearMissionIds.begin = 0;
        local->clearMissionIds.end = 0;
        local->clearMissionIds.capEnd = 0;
        if (!Stage::Story::init(local, &entry)) {
            rw::call(vector_int_base_dtor, {rw::guest_addr(&local->clearMissionIds)});
            return false;
        }
        pushStory(list, local);
        rw::call(vector_int_base_dtor, {rw::guest_addr(&local->clearMissionIds)});
    }
    return true;
}

// The Event local before init: no stories, an empty bannerUrl, the endTime flag bit 1 cleared
void resetEvent(Stage::Event *local) {
    local->stories.begin = 0;
    local->stories.end = 0;
    local->stories.capEnd = 0;
    local->bannerUrl.clear();
    const uint8_t *flags = local->endTime.bytes + 7;
    rw::store8(flags, uint8_t(*flags & 0xFD));
}
// The Event part of a copy constructor into fresh memory at dst
void copyEvent(uint32_t dst, Stage::Event *src) {
    Stage::Event *d = rw::host_ptr<Stage::Event>(dst);
    rw::call(STORY_VECTOR_COPY, {dst, rw::guest_addr(&src->stories)});
    d->showIcon = src->showIcon.get();
    d->eventStageId = src->eventStageId.get();
    raid::copyString(&d->bannerUrl, &src->bannerUrl);
    raid::copyWords(dst + 32, rw::guest_addr(src) + 32, 4);     // eventType, status, endTime
}
// The Event local's destructor
void destroyEvent(Stage::Event *local) {
    raid::freeString(&local->bannerUrl);
    rw::call(STORY_VECTOR_BASE_DTOR, {rw::guest_addr(&local->stories)});
}
}  // namespace

// ---------------------------------------------------------------------------------------------- Stage::Story
uint32_t Stage::Story::init(Story *self, const JsonValue *json) {
    GuestFrame frame(0x38);                   // original frame; the pushed int local at +0x1c
    if (!json->IsObject()) return 0;
    if (!readInt(*json, "stageId", self->stageId)) return 0;
    if (!readInt(*json, "useAp", self->useAp)) return 0;
    if (!readInt(*json, "score", self->score)) return 0;
    const JsonValue &ids = (*json)["clearMissionIds"];
    if (!ids.IsArray()) return 0;
    uint32_t n = ids.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &v = ids.At(k);
        if (!v.IsInt()) return 0;
        stage::pushInt(&self->clearMissionIds, frame.addr(0x1C), v.GetUint());
    }
    return 1;
}
RW_FUNCTION(0x4A34B4, "hole::network::api::Stage::Story::init", Stage::Story::init);

Stage::Story *Stage::Story::destroy(Story *self) {
    rw::call(vector_int_base_dtor, {rw::guest_addr(&self->clearMissionIds)});
    return self;
}
RW_FUNCTION(0x764254, "hole::network::api::Stage::Story::~Story", Stage::Story::destroy);

// ---------------------------------------------------------------------------------------------- Stage::Event
uint32_t Stage::Event::init(Event *self, const JsonValue *json) {
    GuestFrame frame(0x50);                   // original frame; the Story local at +0x20
    if (!json->IsObject()) return 0;
    const JsonValue &stages = (*json)["stages"];
    if (!stages.IsArray()) return 0;
    if (!readStories(&self->stories, stages, frame.at<Story>(0x20))) return 0;
    if (!readInt(*json, "eventStageId", self->eventStageId)) return 0;
    const JsonValue &icon = (*json)["showIcon"];
    if (!icon.IsInt()) return 0;
    self->showIcon = uint8_t(icon.GetUint() != 0 ? 1 : 0);
    const JsonValue &banner = (*json)["bannerUrl"];
    if (!banner.IsString()) return 0;
    self->bannerUrl.assign(banner.GetString());
    const JsonValue &type = (*json)["eventType"];
    if (!type.IsInt() || type.GetUint() - 1 > 2) return 0;
    self->eventType = type.GetInt();
    const JsonValue &status = (*json)["status"];
    if (!status.IsInt() || status.GetUint() - 1 > 1) return 0;
    self->status = status.GetInt();
    const JsonValue &end = (*json)["endTime"];
    if (!end.IsString()) return 0;
    return rw::call(guest::Date_init, {rw::guest_addr(&self->endTime), end.GetString(), 0});
}
RW_FUNCTION(0x4A360C, "hole::network::api::Stage::Event::init", Stage::Event::init);

// ---------------------------------------------------------------------------------------------- Stage::Colosseum
uint32_t Stage::Colosseum::init(Colosseum *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    if (!readInt(*json, "status", self->status)) return 0;
    const JsonValue &message = (*json)["message"];
    if (!message.IsString()) return 0;
    self->message.assign(message.GetString());
    return 1;
}
RW_FUNCTION(0x4A3854, "hole::network::api::Stage::Colosseum::init", Stage::Colosseum::init);

// ---------------------------------------------------------------------------------------------- Stage
uint32_t Stage::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0xb8: the Story local at +0x58, the Event local at +0x70
    GuestFrame frame(0xB8);
    Story *story = frame.at<Story>(0x58);
    Event *event = frame.at<Event>(0x70);

    Stage *self = newObject<Stage>(sizeof(Stage));
    rw::call(guest::memset_, {rw::guest_addr(self), 0x18, 0});        // stories, events
    rw::store8(self->raid.info.timeLeft.bytes + 7, 0);                 // Date flag bytes
    rw::store8(self->raid.info.feverTime.bytes + 7, 0);
    self->raid.discoverer.name.clear();
    self->raid.mvp.name.clear();
    self->colosseum.message.clear();

    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &stories = (*response)["stories"];
        if (!stories.IsArray()) return false;
        if (!readStories(&self->stories, stories, story)) return false;
        const JsonValue &events = (*response)["events"];
        if (!events.IsArray()) return false;
        uint32_t n = events.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = events.At(k);
            if (!entry.IsObject()) return false;
            resetEvent(event);
            if (!Event::init(event, &entry)) {
                destroyEvent(event);
                return false;
            }
            uint32_t end = self->events.end;
            if (end == self->events.capEnd) {
                rw::call(EVENT_PUSH_SLOW, {rw::guest_addr(&self->events), rw::guest_addr(event)});
            } else {
                uint32_t base = 0;
                if (end) {
                    copyEvent(end, event);
                    base = self->events.end;
                }
                self->events.end = base + sizeof(Event);
            }
            destroyEvent(event);
        }
        if (Raid::init(&self->raid, response, 1) != 1) return false;
        const JsonValue &next = (*response)["newStageId"];
        if (!next.IsInt()) return false;
        self->newStageId = next.GetInt();
        const JsonValue &colosseum = (*response)["colosseum"];
        if (!colosseum.IsObject()) return false;
        return Colosseum::init(&self->colosseum, &colosseum) != 0;
    };

    if (!parseAll()) {
        raid::freeString(&self->colosseum.message);
        raid::freeString(&self->raid.mvp.name);
        raid::freeString(&self->raid.discoverer.name);
        rw::call(EVENT_VECTOR_BASE_DTOR, {rw::guest_addr(&self->events)});
        rw::call(STORY_VECTOR_BASE_DTOR, {rw::guest_addr(&self->stories)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(STAGE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A38CC, "hole::network::api::Stage::parse", Stage::parse);

// ---------------------------------------------------------------------------------------------- Stage160310
uint32_t Stage160310::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x58);                   // original frame; the Story local at +0x28
    Stage::Story *story = frame.at<Stage::Story>(0x28);

    Stage160310 *self = newObject<Stage160310>(sizeof(Stage160310));
    self->stories.begin = 0;
    self->stories.end = 0;
    self->stories.capEnd = 0;
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &stories = (*response)["stories"];
        if (!stories.IsArray()) return false;
        if (!readStories(&self->stories, stories, story)) return false;
        const JsonValue &next = (*response)["newStageId"];
        if (!next.IsUint()) return false;
        self->newStageId = next.GetUint();
        return true;
    };
    if (!parseAll()) {
        rw::call(STORY_VECTOR_BASE_DTOR, {rw::guest_addr(self)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(S160310_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A3D3C, "hole::network::api::Stage160310::parse", Stage160310::parse);

// ---------------------------------------------------------------------------------------------- StageEvent
uint32_t StageEvent::Event::init(Event *self, const JsonValue *json) {
    if (Stage::Event::init(self, json) != 1) return 0;
    if (!readInt(*json, "newStageId", self->newStageId)) return 0;
    const JsonValue &lock = (*json)["isLock"];
    if (!lock.IsUint()) return 0;
    self->isLock = uint8_t(lock.GetUint());
    return 1;
}
RW_FUNCTION(0x4A3EF8, "hole::network::api::StageEvent::Event::init", StageEvent::Event::init);

uint32_t StageEvent::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x80);                   // original frame; the Event local at +0x30
    Event *event = frame.at<Event>(0x30);

    StageEvent *self = newObject<StageEvent>(sizeof(StageEvent));
    self->events.begin = 0;
    self->events.end = 0;
    self->events.capEnd = 0;
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &events = (*response)["events"];
        if (!events.IsArray()) return false;
        uint32_t n = events.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = events.At(k);
            if (!entry.IsObject()) return false;
            resetEvent(event);
            if (!Event::init(event, &entry)) {
                destroyEvent(event);
                return false;
            }
            uint32_t end = self->events.end;
            if (end == self->events.capEnd) {
                rw::call(SEVENT_PUSH_SLOW, {rw::guest_addr(&self->events), rw::guest_addr(event)});
            } else {
                uint32_t base = 0;
                if (end) {
                    copyEvent(end, event);
                    Event *dst = rw::host_ptr<Event>(end);
                    dst->isLock = event->isLock.get();
                    dst->newStageId = event->newStageId.get();
                    base = self->events.end;
                }
                self->events.end = base + sizeof(Event);
            }
            destroyEvent(event);
        }
        const JsonValue &status = (*response)["raidStatus"];
        if (!status.IsUint()) return false;
        self->raidStatus = status.GetUint();
        return true;
    };
    if (!parseAll()) {
        // operator delete gets what the vector destructor returns (this)
        guestDelete(rw::call(SEVENT_VECTOR_BASE_DTOR, {rw::guest_addr(self)}));
        self = nullptr;
    }
    rw::call(SEVENT_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A3F60, "hole::network::api::StageEvent::parse", StageEvent::parse);

}  // namespace hole::network::api
