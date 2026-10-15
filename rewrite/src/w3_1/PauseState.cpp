// PauseState: an empty cocos2d::Node whose lifetime pauses the game. The first live PauseState pauses the audio
// layer and every scheduler target (remembering the paused set and the default scheduler); the last one to go away
// restores them. A global reference count guarded by a mutex tracks the live instances.
#include "core/Guest.hpp"

namespace pause_state_w31 {

constexpr uint32_t VTABLE = 0xF95BB8;                  // _ZTV10PauseState + 8
constexpr uint32_t SIZE = 0x270;

// the global pause bookkeeping (anonymous statics of PauseState.cpp)
struct PauseGlobals {
    const uint8_t mutex[4];                 // +0x00 std::mutex
    rw::Field<int32_t> liveCount;           // +0x04 PauseState instances alive
    rw::Field<uint32_t> savedScheduler;     // +0x08 Director::setDefaultScheduler's previous value
    const uint8_t pad_0c[0x18];
    // +0x24 std::set<void*> of the paused scheduler targets
    rw::Field<uint32_t> pausedBeginNode;    // +0x24
    rw::Field<uint32_t> pausedRoot;         // +0x28 (end node's left child)
    rw::Field<uint32_t> pausedSize;         // +0x2C
};
RW_LAYOUT(PauseGlobals, pausedBeginNode, 0x24);
constexpr uint32_t GLOBALS = 0x1003E50;
inline PauseGlobals &globals() { return *rw::host_ptr<PauseGlobals>(GLOBALS); }

constexpr uint32_t MUTEX_LOCK = 0xC8DE15;              // std::mutex::lock (sub_C8DE14)
constexpr uint32_t MUTEX_UNLOCK = 0xC8DE59;            // std::mutex::unlock (sub_C8DE58)
constexpr uint32_t AUDIO_MANAGER_GET_INSTANCE = 0x5BECFD;   // audio::Manager::getInstance()
constexpr uint32_t AUDIO_MANAGER_GET_LAYER = 0x5BF33D;      // audio::Manager::getLayer(shared_ptr<Layer>* out, mgr, int)
constexpr uint32_t AUDIO_LAYER_PAUSE = 0x5BF6EB;            // audio::Layer::pause(int)
constexpr uint32_t AUDIO_LAYER_RESUME = 0x5BF737;           // audio::Layer::resume(int)
constexpr uint32_t DIRECTOR_GET_INSTANCE = 0x8EE519;
constexpr uint32_t DIRECTOR_GET_SCHEDULER = 0x8F0CF1;       // Director::getScheduler(int) const
constexpr uint32_t DIRECTOR_SET_DEFAULT_SCHEDULER = 0x8EECC1;   // Director::setDefaultScheduler(int) -> previous
constexpr uint32_t SCHEDULER_PAUSE_ALL_TARGETS = 0x8FF74D;  // Scheduler::pauseAllTargets() -> std::set<void*> (sret)
constexpr uint32_t SCHEDULER_RESUME_TARGETS = 0x8FF845;     // Scheduler::resumeTargets(const std::set<void*>&)
constexpr uint32_t SET_MOVE_ASSIGN = 0x5EDE11;              // std::__tree<void*>::__move_assign(tree&, true_type)
constexpr uint32_t SET_DESTROY = 0x5EDDE5;                  // std::__tree<void*>::destroy(node)
constexpr uint32_t NODE_CTOR = 0x892531;                    // cocos2d::Node::Node()
constexpr uint32_t NODE_DTOR = 0x8927ED;                    // cocos2d::Node::~Node()
constexpr uint32_t REF_AUTORELEASE = 0x8FD0D5;
constexpr uint32_t SCHEDULER_ALL = 0x80000000u;             // getScheduler argument: the scheduler of every priority
constexpr uint32_t SCHEDULER_PAUSED = 0x7FFFFFFF;           // setDefaultScheduler argument while paused

// the audio layer 0 (shared_ptr), action applied, reference dropped
inline void audioLayer(uint32_t action) {
    guest::Frame f(8);
    uint32_t mgr = rw::call(AUDIO_MANAGER_GET_INSTANCE, {});
    rw::call(AUDIO_MANAGER_GET_LAYER, {f.addr(), mgr, 0});
    rw::call(action, {rw::load<uint32_t>(f.addr()), 0});
    uint32_t ctrl = rw::load<uint32_t>(f.addr(4));
    if (ctrl) guest::release(ctrl);
}

struct PauseState {
    // PauseState::create(): pause everything when this is the first live instance, then a new autoreleased node
    static uint32_t create() {
        PauseGlobals &g = globals();
        rw::call(MUTEX_LOCK, {GLOBALS});
        int32_t count = g.liveCount;
        if (count == 0) {
            audioLayer(AUDIO_LAYER_PAUSE);
            uint32_t scheduler = rw::call(DIRECTOR_GET_SCHEDULER, {rw::call(DIRECTOR_GET_INSTANCE, {}), SCHEDULER_ALL});
            {
                guest::Frame f(12);   // the std::set<void*> pauseAllTargets returns
                rw::call(SCHEDULER_PAUSE_ALL_TARGETS, {f.addr(), scheduler});
                rw::call(SET_MOVE_ASSIGN, {GLOBALS + 0x24, f.addr()});
                rw::call(SET_DESTROY, {f.addr(), rw::load<uint32_t>(f.addr(4))});
            }
            g.savedScheduler = rw::call(DIRECTOR_SET_DEFAULT_SCHEDULER, {rw::call(DIRECTOR_GET_INSTANCE, {}), SCHEDULER_PAUSED});
            count = g.liveCount;
        }
        g.liveCount = int32_t(uint32_t(count) + 1);
        uint32_t self = guest::newObject(SIZE);
        rw::call(NODE_CTOR, {self});
        rw::save<uint32_t>(self, VTABLE);
        rw::call(REF_AUTORELEASE, {self});
        rw::call(MUTEX_UNLOCK, {GLOBALS});
        return self;
    }

    // PauseState::PauseState(): a plain Node with the PauseState vtable (returns this)
    static uint32_t construct(uint32_t self) {
        uint32_t node = rw::call(NODE_CTOR, {self});
        rw::save<uint32_t>(node, VTABLE);
        return node;
    }

    // ~PauseState(): the last live instance resumes the scheduler targets and the audio
    static uint32_t destruct(uint32_t self) {
        PauseGlobals &g = globals();
        rw::save<uint32_t>(self, VTABLE);
        rw::call(MUTEX_LOCK, {GLOBALS});
        int32_t count = int32_t(uint32_t(int32_t(g.liveCount)) - 1);
        g.liveCount = count;
        if (count == 0) {
            rw::call(DIRECTOR_SET_DEFAULT_SCHEDULER, {rw::call(DIRECTOR_GET_INSTANCE, {}), g.savedScheduler});
            uint32_t scheduler = rw::call(DIRECTOR_GET_SCHEDULER, {rw::call(DIRECTOR_GET_INSTANCE, {}), SCHEDULER_ALL});
            rw::call(SCHEDULER_RESUME_TARGETS, {scheduler, GLOBALS + 0x24});
            // paused.clear()
            rw::call(SET_DESTROY, {GLOBALS + 0x24, g.pausedRoot});
            g.pausedBeginNode = GLOBALS + 0x28;
            g.pausedRoot = 0;
            g.pausedSize = 0;
            audioLayer(AUDIO_LAYER_RESUME);
        }
        rw::call(MUTEX_UNLOCK, {GLOBALS});
        rw::call(NODE_DTOR, {self});
        return self;
    }

    // deleting destructor: the pointer freed is the one ~PauseState returned
    static void destructDelete(uint32_t self) { guest::deleteObject(destruct(self)); }
};

}  // namespace pause_state_w31

RW_FUNCTION(0x5EDB30, "PauseState::create()", pause_state_w31::PauseState::create);
RW_FUNCTION(0x5EDC80, "PauseState::PauseState()", pause_state_w31::PauseState::construct);
RW_FUNCTION(0x5EDCA0, "PauseState::~PauseState()", pause_state_w31::PauseState::destruct);
RW_FUNCTION(0x5EDDD8, "PauseState::~PauseState() (deleting)", pause_state_w31::PauseState::destructDelete);

// IStreamReader: the abstract stream reader interface; its destructors only restore the vtable.
namespace istream_reader_w31 {
constexpr uint32_t VTABLE = 0xFD59E0;   // _ZTV13IStreamReader + 8
inline uint32_t destruct(uint32_t self) {
    rw::save<uint32_t>(self, VTABLE);
    return self;   // r0 untouched
}
inline uint32_t destructDelete(uint32_t self) {
    rw::save<uint32_t>(self, VTABLE);
    guest::deleteObject(self);
    return self;   // r0 restored to this after operator delete
}
}  // namespace istream_reader_w31

RW_FUNCTION(0xCDB7A0, "IStreamReader::~IStreamReader()", istream_reader_w31::destruct);
RW_FUNCTION(0xCDB7F8, "IStreamReader::~IStreamReader() (deleting)", istream_reader_w31::destructDelete);
