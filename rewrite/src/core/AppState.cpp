#include "AppState.hpp"

namespace {

// ------------------------------------------------------------------------------------------ guest functions
// libc / libc++ / runtime
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;        // __aeabi_memset(dst, n, c)
constexpr uint32_t AEABI_MEMCPY = 0x40E164;        // __aeabi_memcpy(dst, src, n)
constexpr uint32_t AEABI_MEMMOVE = 0x40EF50;       // __aeabi_memmove(dst, src, n)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t ATOI = 0x410A14;
constexpr uint32_t FOPEN = 0x414158;
constexpr uint32_t FSEEK = 0x4146D4;
constexpr uint32_t FTELL = 0x4146E0;
constexpr uint32_t FCLOSE = 0x414170;
constexpr uint32_t PTHREAD_SELF = 0x414704;
constexpr uint32_t DIVSI3 = 0xCD5E8D;              // __divsi3
constexpr uint32_t MODSI3 = 0xCD6009;              // __modsi3
constexpr uint32_t STRING_INIT = 0x47CD93;         // std::string::__init(const char*, size_t) (sub_47CD92)
constexpr uint32_t MUTEX_LOCK = 0xC8DE15;          // std::mutex::lock (sub_C8DE14)
constexpr uint32_t MUTEX_UNLOCK = 0xC8DE59;        // std::mutex::unlock (sub_C8DE58)
constexpr uint32_t MUTEX_DTOR = 0xC8DDFD;          // std::mutex::~mutex (sub_C8DDFC)
constexpr uint32_t COND_WAIT = 0xC60735;           // std::condition_variable::wait(unique_lock&) (sub_C60734)
constexpr uint32_t COND_NOTIFY = 0xC60725;         // std::condition_variable::notify_all (sub_C60724)
constexpr uint32_t COND_DTOR = 0xC60701;           // std::condition_variable::~condition_variable (sub_C60700)
constexpr uint32_t VECTOR_INT_ASSIGN = 0x47D5BD;   // std::vector<int>::assign(int*, int*)
constexpr uint32_t VECTOR_U64_ASSIGN = 0x4D7461;   // std::vector<unsigned long long>::assign(u64*, u64*)
constexpr uint32_t SORT_INT = 0x4EC65D;            // std::__sort<__less<int>, int*>(first, last, less&)
constexpr uint32_t SORT_U64 = 0x4ECC81;            // std::__sort<__less<u64>, u64*>(first, last, less&)
constexpr uint32_t VECTOR_INT_PUSH_SLOW = 0x4D6CC1;  // std::vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t VECTOR_U64_PUSH_SLOW = 0x4ECB6D;  // std::vector<u64>::__push_back_slow_path(const u64&)
constexpr uint32_t VECTOR_UINT_PUSH_SLOW = 0x4ED6F1; // std::vector<unsigned>::__push_back_slow_path(unsigned&&)
constexpr uint32_t VECTOR_INT_DTOR = 0x478961;     // std::__vector_base<int>::~__vector_base
constexpr uint32_t VECTOR_U64_DTOR = 0x468499;     // std::__vector_base<u64>::~__vector_base
constexpr uint32_t UMAP_UU_AT = 0x4EAC69;          // unordered_map<unsigned, unsigned>::operator[](const unsigned&)
constexpr uint32_t UMAP_UU_ERASE = 0x4ED82D;       // ...::__erase_unique(const unsigned&)
constexpr uint32_t UMAP_UU_FIND = 0x4B52F3;        // ...::find(const unsigned&) -> node or null
constexpr uint32_t UMAP_UU_CLEAR = 0x4EE2A5;       // ...::clear()
constexpr uint32_t UMAP_UU_DTOR = 0x4B4EC9;        // ...::~__hash_table
constexpr uint32_t SET_INT_DESTROY = 0x48D9B5;     // std::__tree<int>::destroy(node)
constexpr uint32_t FUNCTION_COPY = 0x4683CD;       // std::function<void()>::function(const function&)
constexpr uint32_t FUNCTION_CALL = 0x470E69;       // std::function<void()>::operator()
constexpr uint32_t FUNCTION_ASSIGN = 0x4EB9CD;     // std::function<void()>::operator=(const function&)
// SaveBin (another helper's module)
constexpr uint32_t SAVEBIN_DESERIALIZE = 0x4F0FE9; // SaveBin::deserialize(const shared_ptr<bg::Data>&) -> bool
constexpr uint32_t SAVEBIN_SERIALIZE = 0x4F0E95;   // SaveBin::serialize() -> shared_ptr<bg::Data> (sret)
constexpr uint32_t SAVEBIN_CLEAR = 0x4F0E8D;       // SaveBin::clear()
constexpr uint32_t SAVEBIN_DTOR = 0x4DAB71;        // the SaveBin's unordered_map<string, shared_ptr<Data>> dtor
constexpr uint32_t SAVEBIN_GET_DATA = 0x4F0DED;    // SaveBin::get(const char*) -> shared_ptr<bg::Data> (sret)
constexpr uint32_t SAVEBIN_SET_DATA = 0x4F0CE1;    // SaveBin::set(const char*, const shared_ptr<bg::Data>&)
constexpr uint32_t SAVEBIN_GET_U32 = 0x4D8E8D;     // SaveBin::get<unsigned>(key, T&) -> bool
constexpr uint32_t SAVEBIN_GET_S32 = 0x4D8DDD;     // SaveBin::get<int>
constexpr uint32_t SAVEBIN_GET_BOOL = 0x4E86F5;    // SaveBin::get<bool>
constexpr uint32_t SAVEBIN_GET_VEC_INT = 0x4E7829; // SaveBin::get<vector<int>>
constexpr uint32_t SAVEBIN_GET_VEC_U64 = 0x4E8651; // SaveBin::get<vector<unsigned long long>>
constexpr uint32_t SAVEBIN_GET_UMAP = 0x4E78CD;    // SaveBin::get<unordered_map<unsigned, unsigned>>
constexpr uint32_t SAVEBIN_SET_U32 = 0x4D8881;     // SaveBin::set<unsigned>(key, const T&)
constexpr uint32_t SAVEBIN_SET_S32 = 0x4D8AB5;     // SaveBin::set<int>
constexpr uint32_t SAVEBIN_SET_BOOL = 0x4E956D;    // SaveBin::set<bool>
constexpr uint32_t SAVEBIN_SET_U8 = 0x4E00FD;      // SaveBin::set<unsigned char>
constexpr uint32_t SAVEBIN_SET_VEC_INT = 0x4E933D; // SaveBin::set<vector<int>>
constexpr uint32_t SAVEBIN_SET_VEC_U64 = 0x4EA321; // SaveBin::set<vector<unsigned long long>>
constexpr uint32_t SAVEBIN_SET_UMAP = 0x4E9455;    // SaveBin::set<unordered_map<unsigned, unsigned>>
// AppState nested types (AppStateTypes.*) and their make_shared
constexpr uint32_t VERSION_CTOR = 0x4EC09D;        // AppState::Version::Version(const char*)
constexpr uint32_t VERSION_GT = 0x4EAEDD;          // AppState::Version::operator>(const Version&)
constexpr uint32_t MEDAL_SFS_LOAD = 0x4E7971, MEDAL_SFS_SAVE = 0x4E9689;
constexpr uint32_t AVATAR_BFS_LOAD = 0x4E7ECD, AVATAR_BFS_SAVE = 0x4E9DD5;
constexpr uint32_t ADVENTURE_SFS_LOAD = 0x4E8211, ADVENTURE_SFS_SAVE = 0x4EA119;
constexpr uint32_t OPTION_LOAD = 0x4E8419, OPTION_SAVE = 0x4E9BE5;
constexpr uint32_t MAKE_PERSONAL_INFO = 0x4EE7F9;        // make_shared<PersonalInfo>()
constexpr uint32_t MAKE_PERSONAL_INFO_DATA = 0x4EE49D;   // make_shared<PersonalInfo>(shared_ptr<bg::Data>&)
constexpr uint32_t MAKE_BRIDGE_INFO = 0x4EE71D;          // make_shared<BRIDGEInfo>()
constexpr uint32_t MAKE_LOGIN_INFO = 0x4EE641;           // make_shared<LoginInfo>()
constexpr uint32_t MAKE_MEDAL_SFS = 0x4EE1AD;            // make_shared<MedalSortFilterState>(const char*&)
constexpr uint32_t MAKE_AVATAR_BFS = 0x4EE0BD;           // make_shared<AvatarBoardFilterState>(const char*&)
constexpr uint32_t MAKE_ADVENTURE_SFS = 0x4EDFCD;        // make_shared<AdventureSelectFilterState>(const char*&)
constexpr uint32_t MAKE_KEYBLADE_SFS = 0x4EDC89;         // make_shared<KeyBladeSortFilterState>()
constexpr uint32_t MAKE_KEYBLADE_SFS_DATA = 0x4EDF55;    // make_shared<KeyBladeSortFilterState>(shared_ptr<Data>&)
constexpr uint32_t MAKE_SKILL_SFS = 0x4EDB89;            // make_shared<SkillSortFilterState>()
constexpr uint32_t MAKE_SKILL_SFS_DATA = 0x4EDEDD;       // make_shared<SkillSortFilterState>(shared_ptr<Data>&)
constexpr uint32_t MAKE_OPTION_STATE = 0x4EDE7D;         // make_shared<OptionState>()
// game
constexpr uint32_t APIMANAGER_GET_INSTANCE = 0x4BA8C9;
constexpr uint32_t MDM_GET_INSTANCE = 0x76C091;          // MasterDataManager::getInstance
constexpr uint32_t MDM_GET_IDS = 0x76D091;               // MasterDataManager::getIDs(out set<int>, mgr, table)
constexpr uint32_t RDM_GET_INSTANCE = 0x7728AD;          // ResourceDataManager::getInstance
constexpr uint32_t MISC_CREATE = 0x786D5D;               // master::Misc::create(out, id)
constexpr uint32_t EVRESOURCE_CREATE = 0x792D95;         // master::EvResource::create(out, id)
constexpr uint32_t STAGE_STATE_CREATE = 0x4EE969;        // StageState::create(out, bool)
constexpr uint32_t DATE_SET_NOW = 0x492739;              // Date::setNow()
constexpr uint32_t DATE_INTERVAL_SINCE_NOW = 0x4922D5;   // Date::calculateIntervalSinceNow() -> seconds
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;         // StringBuffer::StringBuffer(fmt, ...)
constexpr uint32_t STRINGBUFFER_FCONCAT = 0x491571;      // StringBuffer::fconcat(fmt, ...)
constexpr uint32_t STRINGBUFFER_CONCAT = 0x4914C5;       // StringBuffer::concat(const char*)
constexpr uint32_t TEXTDATA_GET = 0x4928AD;              // TextData::get(out shared_ptr, Type, id)
constexpr uint32_t DISP_TUTORIAL_CHECK = 0x472D75;       // SceneUtil::dispTutorialCheck(tutorial_ids_t)
constexpr uint32_t PURCHASE_GET_INSTANCE = 0x4D7A71;
constexpr uint32_t PURCHASE_DESERIALIZE = 0x4D8BD1;      // PurchaseManager::deserialize(const shared_ptr<Data>&)
constexpr uint32_t PURCHASE_SERIALIZE = 0x4D8709;        // PurchaseManager::serialize() (sret)
constexpr uint32_t LOAD_DATA_FROM_FILE = 0x4E87A9;       // LoadDataFromFile() -> shared_ptr<bg::Data> (sret)
constexpr uint32_t SAVE_DATA_TO_FILE = 0x4EA439;         // SaveDataToFile(const shared_ptr<bg::Data>&)
constexpr uint32_t UUID_CREATE = 0x4E0719;               // UUID::create() -> shared_ptr<bg::Data> (sret)
constexpr uint32_t MD5_CALCULATE = 0x4E0B29;             // md5::calculate(path, std::string& out, unsigned* size)
constexpr uint32_t VFS_READ_DATA = 0x8DD46D;             // bg::vfs::readData(path) -> shared_ptr<bg::Data> (sret)
constexpr uint32_t SAFEBUFFER_SERIALIZE = 0x45A555;      // SafeBuffer::serialize() (sret)
constexpr uint32_t AUDIO_GET_INSTANCE = 0x5BECFD;        // audio::Manager::getInstance
constexpr uint32_t AUDIO_GET_LAYER = 0x5BF33D;           // audio::Manager::getLayer(out shared_ptr, mgr, int)
constexpr uint32_t AUDIO_SET_VOLUME = 0x5BF825;          // audio::Layer::setVolume(float, int)
// cocos2d
constexpr uint32_t USERDEFAULT_GET_INSTANCE = 0x901A25;
constexpr uint32_t USERDEFAULT_GET_DATA = 0x901805;      // UserDefault::getDataForKey(out Data, ud, key)
constexpr uint32_t USERDEFAULT_SET_DATA = 0x901989;      // UserDefault::setDataForKey(ud, key, const Data&)
constexpr uint32_t USERDEFAULT_FLUSH = 0x90129D;
constexpr uint32_t DATA_CTOR = 0x8ED815, DATA_DTOR = 0x8ED89B;
constexpr uint32_t DATA_IS_NULL = 0x8ED8EF, DATA_GET_BYTES = 0x8ED90B, DATA_GET_SIZE = 0x8ED90F;
constexpr uint32_t DATA_FAST_SET = 0x8ED913;             // cocos2d::Data::fastSet(bytes, size)
constexpr uint32_t DIRECTOR_GET_INSTANCE = 0x8EE519;
constexpr uint32_t DIRECTOR_GET_SCHEDULER = 0x8F0CF1;    // Director::getScheduler(int)
constexpr uint32_t SCHEDULER_SCHEDULE_UPDATE = 0x4E7495; // Scheduler::scheduleUpdate<AppState>(target, priority, paused)
constexpr uint32_t SCHEDULER_UNSCHEDULE_UPDATE = 0x8FE9E5;
constexpr uint32_t FILEUTILS_GET_INSTANCE = 0xA3F44D;
constexpr uint32_t FILEUTILS_VSLOT_PATH = 19;            // FileUtils virtual at +0x4C: a path (sret std::string)

// ------------------------------------------------------------------------------------------ guest data
constexpr uint32_t DESTRUCTOR = 0x4E753D;                // AppState::~AppState (for __cxa_atexit)
constexpr uint32_t BG_DATA_EMPLACE_VTABLE = 0xF7E1C4;    // __shared_ptr_emplace<bg::Data> vtable + 8
constexpr uint32_t BG_DATA_VTABLE = 0xF7E1EC;            // bg::Data vtable + 8
constexpr uint32_t SAVE_FORMAT_VERSION = 0xE1E9A0;       // const unsigned (0x780D108B): the save "version" value
constexpr uint32_t FILTER_STATE_NAMES = 0xFEF010;        // const char*[7]: deckmedal_state, mixedmedal_state,
                                                         // evolvemedal_state, sellmedal_state, medallist_state,
                                                         // avatarboardfilter_state, adventure_select_filter_state
constexpr uint32_t ONE_F = 0x3F800000;                   // 1.0f (unordered_map max_load_factor)
constexpr uint32_t SCHEDULER_PRIORITY = 0x7FFFFFFF;
constexpr int32_t MISC_AP_RECOVERY = 101;                // master::Misc 101: minutes per AP point (int at 4)
constexpr uint32_t TABLE_EV_RESOURCE = 0x12;             // MasterDataManager::Table evResource
constexpr int32_t SORT_DEFAULT_TEXT = 100100003;         // TextData type 2: the default sort index
// string literals
constexpr uint32_t STR_INFO_VERSION = 0xDFBAE0, STR_RB = 0xDFC3A6, STR_DATA = 0xDFC324;
constexpr uint32_t STR_HEX_BYTE = 0xDFC3A1;              // "%02x"
constexpr uint32_t STR_DASH = 0xDFC8FD;                  // "-"
constexpr uint32_t STR_SESSION_FMT = 0xDFC8FF;           // "%.*s"
constexpr uint32_t STR_VERSION = 0xDFCD4C, STR_PERSONAL = 0xDFCD43, STR_MASTER_REVISION = 0xDFCD33;
constexpr uint32_t STR_MASTER_FILE_REVISIONS = 0xDFCD1D, STR_RESOURCE_REVISION = 0xDFCD0B;
constexpr uint32_t STR_RESOURCE_SIZE = 0xDFCCFD, STR_EVENT_RESOURCES = 0xDFCCED;
constexpr uint32_t STR_POS_NORMAL = 0xDFCCD5, STR_POS_RAID = 0xDFCCBF, STR_POS_COLOSSEUM = 0xDFCCA4;
constexpr uint32_t STR_POS_STAGE = 0xDFCC8D;
constexpr uint32_t STR_POWERUP_KEYBLADE = 0xDFCBEE, STR_EQUIP_KEYBLADE = 0xDFCBDA, STR_SELL_SKILL = 0xDFCBCA;
constexpr uint32_t STR_PURCHASE = 0xDFCBB4, STR_TRACKING = 0xDFCBA4;
constexpr uint32_t STR_NEW_USERMEDAL = 0xDFCB92, STR_WILL_LOCK = 0xDFCB7A, STR_WILL_UNLOCK = 0xDFCB60;
constexpr uint32_t STR_STAGE_OLD_SELECTS = 0xDFCB4E, STR_TUTORIAL_STAGE_START = 0xDFCB39;
constexpr uint32_t STR_NEW_AVATAR_PARTS = 0xDFCB24, STR_NEW_AVATAR_PARTS_SET = 0xDFCB0B;
constexpr uint32_t STR_NEW_AVATAR_BOARD = 0xDFCAF6, STR_POP_BENEFIT_STONE = 0xDFCADF;

// APIManager + 0x8C: the user status SafeBuffer (AP at 0x34, max AP at 0x3C, AP update Date at 0x40);
// APIManager + 0x44: the version info (master revision at +0x30, resource revision at +0x2C)
constexpr uint32_t API_USER_STATUS = 0x8C, API_VERSION_INFO = 0x44;
constexpr uint32_t STATUS_AP = 0x34, STATUS_AP_MAX = 0x3C, STATUS_AP_DATE = 0x40;

// ------------------------------------------------------------------------------------------ helpers
uint32_t addr(const void *p) { return rw::guest_addr(p); }
void releaseIf(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }
void releaseAt(uint32_t ctrlAddr) { releaseIf(rw::load<uint32_t>(ctrlAddr)); }

// member = std::move(*tmp) (tmp: a shared_ptr in the guest frame), releasing the member's old control block
void moveShared(guest::SharedPtr &member, uint32_t tmp) {
    uint32_t obj = rw::load<uint32_t>(tmp), ctrl = rw::load<uint32_t>(tmp + 4);
    rw::save<uint32_t>(tmp, 0);
    rw::save<uint32_t>(tmp + 4, 0);
    member.obj = obj;
    uint32_t old = member.ctrl;
    member.ctrl = ctrl;
    releaseIf(old);
}

// A SafeBuffer row / record behind a guest pointer.
const SafeBuffer *buffer(uint32_t obj) { return rw::host_ptr<SafeBuffer>(obj); }

// Decode n bytes of a SafeBuffer's data at `index` into guest memory (the original's frame buffers).
void readTo(uint32_t sb, uint32_t dst, uint32_t index, uint32_t n) {
    const SafeBuffer *b = buffer(sb);
    SafeBuffer::read(b, rw::host_ptr<uint8_t>(dst), b->data.get() + index, n, index);
}
// Encode n bytes from guest memory into a SafeBuffer's data at `index`.
void writeFrom(uint32_t sb, uint32_t src, uint32_t index, uint32_t n) {
    SafeBuffer *b = rw::host_ptr<SafeBuffer>(sb);
    SafeBuffer::write(b, b->data.get() + index, rw::host_ptr<uint8_t>(src), n, index);
}

// libc++'s __destruct_at_end(begin) as compiled: end += ~(size - 1) & ~(end - size - begin) (= begin).
void clearVector(guest::Vector &v, uint32_t elemSize) {
    uint32_t end = v.end, begin = v.begin;
    if (end == begin) return;
    v.end = end + (~(elemSize - 1) & ~(end - elemSize - begin));
}

// vector::assign(other) unless it is the same vector, then std::sort (sortFn) / or clear when other is null.
void assignVector(guest::Vector &v, const guest::Vector *other, uint32_t assignFn, uint32_t sortFn,
                  uint32_t elemSize) {
    if (other) {
        if (&v != other) rw::call(assignFn, {addr(&v), other->begin, other->end});
        if (sortFn) {
            guest::Frame f(8);   // the (empty) __less comparator
            rw::call(sortFn, {v.begin, v.end, f.addr(0)});
        }
    } else {
        clearVector(v, elemSize);
    }
}

// push_back as inlined: construct at end when there is room, else __push_back_slow_path(value in the frame).
void pushBack32(guest::Vector &v, uint32_t value, uint32_t slowFn) {
    uint32_t end = v.end;
    if (end != v.cap) {
        uint32_t newEnd = 0;
        if (end != 0) {
            rw::save<uint32_t>(end, value);
            newEnd = v.end;
        }
        v.end = newEnd + 4;
    } else {
        guest::Frame f(8);
        rw::save<uint32_t>(f.addr(0), value);
        rw::call(slowFn, {addr(&v), f.addr(0)});
    }
}
void pushBack64(guest::Vector &v, uint64_t value) {
    uint32_t end = v.end;
    if (end != v.cap) {
        uint32_t newEnd = 0;
        if (end != 0) {
            rw::save<uint32_t>(end, uint32_t(value));
            rw::save<uint32_t>(end + 4, uint32_t(value >> 32));
            newEnd = v.end;
        }
        v.end = newEnd + 8;
    } else {
        guest::Frame f(8);
        rw::save<uint64_t>(f.addr(0), value);
        rw::call(VECTOR_U64_PUSH_SLOW, {addr(&v), f.addr(0)});
    }
}

// std::find over a vector<int> / vector<u64>: the address of the first match, or end
uint32_t find32(const guest::Vector &v, uint32_t value) {
    uint32_t p = v.begin, end = v.end;
    while (p != end && rw::load<uint32_t>(p) != value) p += 4;
    return p;
}
uint32_t find64(const guest::Vector &v, uint64_t value) {
    uint32_t p = v.begin, end = v.end;
    while (p != end && rw::load<uint64_t>(p) != value) p += 8;
    return p;
}

// vector::erase(pos) as compiled: move the tail down, then shrink end to begin + (pos index + tail count).
void eraseAt(guest::Vector &v, uint32_t pos, uint32_t elemSize, uint32_t shift) {
    uint32_t begin = v.begin, end = v.end;
    uint32_t offset = pos - begin;
    uint32_t p = begin + (offset & ~(elemSize - 1));
    uint32_t tail = end - (p + elemSize);
    rw::call(AEABI_MEMMOVE, {p, p + elemSize, tail});
    uint32_t newEnd = begin + (((offset >> shift) + (tail >> shift)) << shift);
    uint32_t cur = v.end;
    if (cur != newEnd) v.end = cur + (~(elemSize - 1) & ~(cur - elemSize - newEnd));
}

// ~std::function on a function object in guest memory
void destroyFunction(uint32_t impl, uint32_t buffer) {
    if (impl == buffer) rw::vcall(rw::host_ptr<void>(buffer), 4, {buffer});   // destroy()
    else if (impl) rw::vcall(rw::host_ptr<void>(impl), 5, {impl});           // destroy_deallocate()
}

uint32_t audioLayerVolume(uint32_t frameSlot, uint32_t layerId, uint32_t volumeBits) {
    uint32_t mgr = rw::call(AUDIO_GET_INSTANCE, {});
    rw::call(AUDIO_GET_LAYER, {frameSlot, mgr, layerId});
    rw::call(AUDIO_SET_VOLUME, {rw::load<uint32_t>(frameSlot), volumeBits, 0});
    releaseAt(frameSlot + 4);
    return 0;
}

}  // namespace

// ------------------------------------------------------------------------------------------ singleton
AppState *AppState::getInstance() {
    if (guest::staticInitBegin(INSTANCE_GUARD)) {
        rw::call(0x4E6775, {INSTANCE});   // AppState::AppState()
        rw::call(guest::CXA_ATEXIT, {DESTRUCTOR, INSTANCE, guest::DSO_HANDLE});
        guest::staticInitDone(INSTANCE_GUARD);
    }
    return rw::host_ptr<AppState>(INSTANCE);
}
// first call constructs (files, scheduler, atexit): not shadow-run
RW_FUNCTION_NOVERIFY(0x4E66DC, "AppState::getInstance", AppState::getInstance);

void AppState::setMainThread() {
    rw::save<uint8_t>(MAIN_THREAD_ID_SET, 1);
    rw::save<uint32_t>(MAIN_THREAD_ID, rw::call(PTHREAD_SELF, {}));
}
RW_FUNCTION_NOVERIFY(0x4E6BD4, "AppState::setMainThread", AppState::setMainThread);

uint32_t AppState::isMainThread() {
    uint32_t self = rw::call(PTHREAD_SELF, {});
    return self == rw::load<uint32_t>(MAIN_THREAD_ID) ? 1 : 0;
}
RW_FUNCTION_NOVERIFY(0x4E77F8, "AppState::isMainThread", AppState::isMainThread);

AppState *AppState::construct(AppState *self) {
    // the original's frame (0xAC bytes): md5 size at 0x38, md5 string at 0x3C, file size at 0x48, path string at
    // 0x4C, LoginInfo temp at 0x58, BRIDGEInfo temp at 0x60, PersonalInfo temp at 0x68, version data at 0x70,
    // Version at 0x78, md5 copy at 0x88, Date at 0x30
    guest::Frame f(0xAC);
    const uint32_t s = addr(self);
    self->versionValid = 0;
    rw::save<uint32_t>(s + 0xB8, 0);
    rw::save<uint32_t>(s + 0xB4, 0);
    rw::save<uint32_t>(s + 0xB0, 0);
    rw::save<uint32_t>(s + 0xAC, 0);
    rw::call(AEABI_MEMSET, {s + 0x10, 0x99, 0});      // shared_ptrs .. popBenefitStone
    self->trackingEvents.maxLoadFactor = ONE_F;
    self->eventResources.size = 0;
    self->eventResources.first = 0;
    self->eventResources.bucketCount = 0;
    self->eventResources.buckets = 0;
    self->eventResources.maxLoadFactor = ONE_F;
    self->tutorialCloseFunc.impl = 0;
    rw::call(AEABI_MEMSET, {s + 0xD4, 0x12, 0});      // avatar board positions, isStartTutorialStage
    rw::call(AEABI_MEMSET, {s + 0x100, 0x65, 0});     // tutorialCloseId, ID lists, saveRequested
    self->saveMutex = 0;
    self->saveDone = 0;
    if (rw::load<uint8_t>(MAIN_THREAD_ID_SET) == 0) setMainThread();

    // the version from the bundled text "info/version"
    rw::call(VFS_READ_DATA, {f.addr(0x70), STR_INFO_VERSION});
    uint32_t versionData = rw::load<uint32_t>(f.addr(0x70));
    if (versionData) {
        uint32_t v = rw::call(VERSION_CTOR, {f.addr(0x78), rw::load<uint32_t>(versionData + 4)});
        self->versionMajor = rw::load<int32_t>(f.addr(0x78));
        self->versionMinor = rw::load<int32_t>(f.addr(0x7C));
        self->versionPatch = rw::load<int32_t>(f.addr(0x80));
        self->versionValid = rw::load<uint8_t>(v + 12);
    }
    rw::call(MAKE_PERSONAL_INFO, {f.addr(0x68)});
    moveShared(self->personalInfo, f.addr(0x68));
    rw::call(MAKE_BRIDGE_INFO, {f.addr(0x60)});
    moveShared(self->bridgeInfo, f.addr(0x60));
    load(self);
    rw::call(MAKE_LOGIN_INFO, {f.addr(0x58)});
    moveShared(self->loginInfo, f.addr(0x58));

    // LoginInfo: size, md5 and date of the file FileUtils' path names (when the md5 covers the whole file)
    uint32_t fu = rw::call(FILEUTILS_GET_INSTANCE, {});
    rw::vcall(rw::host_ptr<void>(fu), FILEUTILS_VSLOT_PATH, {f.addr(0x4C), fu});
    auto *path = f.at<guest::String>(0x4C);
    if (path->size() != 0) {
        uint32_t file = rw::call(FOPEN, {path->data(), STR_RB});
        if (file) {
            rw::call(FSEEK, {file, 0, 2});
            rw::save<uint32_t>(f.addr(0x48), rw::call(FTELL, {file}));
            rw::call(FCLOSE, {file});
            rw::save<uint32_t>(f.addr(0x3C), 0);
            rw::save<uint32_t>(f.addr(0x40), 0);
            rw::save<uint32_t>(f.addr(0x44), 0);
            auto *md5 = f.at<guest::String>(0x3C);
            if (rw::call(MD5_CALCULATE, {path->data(), f.addr(0x3C), f.addr(0x38)}) &&
                rw::load<uint32_t>(f.addr(0x38)) == rw::load<uint32_t>(f.addr(0x48))) {
                rw::call(AEABI_MEMCPY, {f.addr(0x88), md5->data(), 0x20});
                rw::store8(f.at<uint8_t>(0x37), uint8_t(*f.at<uint8_t>(0x37) & 0xFD));
                rw::call(DATE_SET_NOW, {f.addr(0x30)});
                writeFrom(self->loginInfo.obj, f.addr(0x48), 0, 4);
                writeFrom(self->loginInfo.obj, f.addr(0x88), 4, 0x20);
                writeFrom(self->loginInfo.obj, f.addr(0x30), 0x24, 8);
            }
            if (md5->isLong()) guest::deleteObject(rw::load<uint32_t>(f.addr(0x44)));
        }
    }
    if (path->isLong()) guest::deleteObject(rw::load<uint32_t>(f.addr(0x54)));

    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    uint32_t scheduler = rw::call(DIRECTOR_GET_SCHEDULER, {director, SCHEDULER_PRIORITY});
    rw::call(SCHEDULER_SCHEDULE_UPDATE, {scheduler, s, 0, 0});
    releaseAt(f.addr(0x74));
    return self;
}
RW_FUNCTION_NOVERIFY(0x4E6774, "AppState::AppState", AppState::construct);

AppState *AppState::destruct(AppState *self) {
    const uint32_t s = addr(self);
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    uint32_t scheduler = rw::call(DIRECTOR_GET_SCHEDULER, {director, SCHEDULER_PRIORITY});
    rw::call(SCHEDULER_UNSCHEDULE_UPDATE, {scheduler, s});
    rw::call(COND_DTOR, {addr(&self->saveDone)});
    rw::call(MUTEX_DTOR, {addr(&self->saveMutex)});
    rw::call(VECTOR_INT_DTOR, {addr(&self->newAvatarBoardIDs)});
    rw::call(VECTOR_INT_DTOR, {addr(&self->newAvatarPartsSetIDs)});
    rw::call(VECTOR_INT_DTOR, {addr(&self->newAvatarPartsIDs)});
    rw::call(VECTOR_INT_DTOR, {addr(&self->clearTutorialPopupIDs)});
    rw::call(VECTOR_INT_DTOR, {addr(&self->clearStageOldSelects)});
    rw::call(VECTOR_U64_DTOR, {addr(&self->willUnlockUserMedalIDs)});
    rw::call(VECTOR_U64_DTOR, {addr(&self->willLockUserMedalIDs)});
    rw::call(VECTOR_U64_DTOR, {addr(&self->newUserMedalIDs)});
    destroyFunction(self->tutorialCloseFunc.impl, addr(&self->tutorialCloseFunc));
    rw::call(UMAP_UU_DTOR, {addr(&self->eventResources)});
    rw::call(SAVEBIN_DTOR, {addr(&self->trackingEvents)});
    rw::call(VECTOR_INT_DTOR, {addr(&self->masterFileRevisions)});
    // shared_ptr members, last to first
    releaseIf(self->optionState.ctrl);
    releaseIf(self->adventureSelectFilter.ctrl);
    releaseIf(self->avatarBoardFilter.ctrl);
    releaseIf(self->sellSkillSortFilter.ctrl);
    releaseIf(self->selectKeyBladeSortFilter.ctrl);
    releaseIf(self->powerUpKeyBladeSortFilter.ctrl);
    for (int k = 5; k >= 0; k--) releaseIf(self->medalSortFilter[k].ctrl);
    releaseIf(self->loginInfo.ctrl);
    releaseIf(self->stageState.ctrl);
    releaseIf(self->bridgeInfo.ctrl);
    releaseIf(self->personalInfo.ctrl);
    return self;
}
RW_FUNCTION_NOVERIFY(0x4E753C, "AppState::~AppState", AppState::destruct);

// ------------------------------------------------------------------------------------------ load / save
void AppState::load(AppState *self) {
    // the original's frame (0x14C bytes); offsets below are from its sp
    guest::Frame f(0x14C);
    const uint32_t s = addr(self);
    const uint32_t cocosData = f.addr(0x120), data = f.addr(0x128), sb = f.addr(0x10C);
    rw::save<uint32_t>(data, 0);
    rw::save<uint32_t>(data + 4, 0);
    // the save blob: UserDefault "data" first
    uint32_t ud = rw::call(USERDEFAULT_GET_INSTANCE, {});
    rw::call(USERDEFAULT_GET_DATA, {cocosData, ud, STR_DATA});
    if (rw::call(DATA_IS_NULL, {cocosData}) == 0) {
        uint32_t bytes = rw::call(DATA_GET_BYTES, {cocosData});
        uint32_t size = rw::call(DATA_GET_SIZE, {cocosData});
        // make_shared<bg::Data>(bytes, size), inlined (a failed new leaves the object pointer at 0x10)
        uint32_t block = guest::newObject(0x38);
        uint32_t obj = 0x10;
        if (block) {
            rw::save<uint32_t>(block + 4, 0);
            rw::save<uint32_t>(block + 8, 0);
            rw::save<uint32_t>(block, BG_DATA_EMPLACE_VTABLE);
            rw::save<uint32_t>(block + 0x30, 0);
            rw::save<uint32_t>(block + 0x10, BG_DATA_VTABLE);
            rw::save<uint32_t>(block + 0x14, bytes);
            rw::save<uint32_t>(block + 0x18, size);
            obj = block + 0x10;
        }
        rw::save<uint32_t>(data, obj);
        uint32_t old = rw::load<uint32_t>(data + 4);
        rw::save<uint32_t>(data + 4, block);
        releaseIf(old);
    }

    uint32_t loaded = 0;      // a pass read the save completely
    uint32_t fromFile = 0;    // the second pass (the save file) ran
    for (;;) {
        if (rw::load<uint32_t>(data) != 0) {
            // SaveBin sb (an empty unordered_map)
            for (uint32_t k = 0; k < 16; k += 4) rw::save<uint32_t>(sb + k, 0);
            rw::save<uint32_t>(sb + 16, ONE_F);
            if (rw::call(SAVEBIN_DESERIALIZE, {sb, data}) != 0 &&
                rw::call(SAVEBIN_GET_U32, {sb, STR_VERSION, f.addr(0x108)}) == 1) {
                rw::call(SAVEBIN_GET_DATA, {f.addr(0x100), sb, STR_PERSONAL});
                if (rw::load<uint32_t>(f.addr(0x100)) != 0) {
                    rw::call(MAKE_PERSONAL_INFO_DATA, {f.addr(0xF8), f.addr(0x100)});
                    moveShared(self->personalInfo, f.addr(0xF8));
                    if (rw::call(SAVEBIN_GET_S32, {sb, STR_MASTER_REVISION, s + 0x90}) != 1 ||
                        rw::call(SAVEBIN_GET_VEC_INT, {sb, STR_MASTER_FILE_REVISIONS, s + 0x94}) == 0)
                        self->masterRevision = 0;
                    if (rw::call(SAVEBIN_GET_S32, {sb, STR_RESOURCE_REVISION, s + 0xA0}) != 1 ||
                        rw::call(SAVEBIN_GET_U32, {sb, STR_RESOURCE_SIZE, s + 0xA4}) == 0) {
                        self->resourceRevision = 0;
                        self->resourceSize = 0;
                    }
                    if (rw::call(SAVEBIN_GET_UMAP, {sb, STR_EVENT_RESOURCES, s + 0xC0}) == 0)
                        rw::call(UMAP_UU_CLEAR, {s + 0xC0});
                    if (rw::call(SAVEBIN_GET_S32, {sb, STR_POS_NORMAL, s + 0xD4}) == 0) self->avatarBoardPosNormal = 0;
                    if (rw::call(SAVEBIN_GET_S32, {sb, STR_POS_RAID, s + 0xD8}) == 0) self->avatarBoardPosRaid = 0;
                    if (rw::call(SAVEBIN_GET_S32, {sb, STR_POS_COLOSSEUM, s + 0xDC}) == 0)
                        self->avatarBoardPosColosseum = 0;
                    if (rw::call(SAVEBIN_GET_S32, {sb, STR_POS_STAGE, s + 0xE0}) == 0) self->avatarBoardPosStage = 0;

                    // sort / filter states, named by FILTER_STATE_NAMES
                    for (uint32_t k = 0; k < 5; k++) {
                        uint32_t tmp = f.addr(0xF0 - 8 * k);
                        rw::call(MAKE_MEDAL_SFS, {tmp, FILTER_STATE_NAMES + 4 * k});
                        moveShared(self->medalSortFilter[k], tmp);
                        rw::call(MEDAL_SFS_LOAD, {self->medalSortFilter[k].obj, sb});
                    }
                    rw::call(MAKE_AVATAR_BFS, {f.addr(0xC8), FILTER_STATE_NAMES + 0x14});
                    moveShared(self->avatarBoardFilter, f.addr(0xC8));
                    rw::call(AVATAR_BFS_LOAD, {self->avatarBoardFilter.obj, sb});
                    rw::call(MAKE_ADVENTURE_SFS, {f.addr(0xC0), FILTER_STATE_NAMES + 0x18});
                    moveShared(self->adventureSelectFilter, f.addr(0xC0));
                    rw::call(ADVENTURE_SFS_LOAD, {self->adventureSelectFilter.obj, sb});

                    rw::call(SAVEBIN_GET_DATA, {f.addr(0xB8), sb, STR_POWERUP_KEYBLADE});
                    if (rw::load<uint32_t>(f.addr(0xB8))) {
                        rw::call(MAKE_KEYBLADE_SFS_DATA, {f.addr(0xB0), f.addr(0xB8)});
                        moveShared(self->powerUpKeyBladeSortFilter, f.addr(0xB0));
                    }
                    rw::call(SAVEBIN_GET_DATA, {f.addr(0xA8), sb, STR_EQUIP_KEYBLADE});
                    if (rw::load<uint32_t>(f.addr(0xA8))) {
                        rw::call(MAKE_KEYBLADE_SFS_DATA, {f.addr(0xA0), f.addr(0xA8)});
                        moveShared(self->selectKeyBladeSortFilter, f.addr(0xA0));
                    }
                    rw::call(SAVEBIN_GET_DATA, {f.addr(0x98), sb, STR_SELL_SKILL});
                    if (rw::load<uint32_t>(f.addr(0x98))) {
                        rw::call(MAKE_SKILL_SFS_DATA, {f.addr(0x90), f.addr(0x98)});
                        moveShared(self->sellSkillSortFilter, f.addr(0x90));
                    }
                    rw::call(SAVEBIN_GET_DATA, {f.addr(0x88), sb, STR_PURCHASE});
                    if (rw::load<uint32_t>(f.addr(0x88))) {
                        uint32_t pm = rw::call(PURCHASE_GET_INSTANCE, {});
                        rw::call(PURCHASE_DESERIALIZE, {pm, f.addr(0x88)});
                    }
                    rw::call(SAVEBIN_GET_DATA, {f.addr(0x80), sb, STR_TRACKING});
                    if (rw::load<uint32_t>(f.addr(0x80)))
                        rw::call(SAVEBIN_DESERIALIZE, {s + 0xAC, f.addr(0x80)});

                    // options, applied to the audio layers (BGM: all layers, then SE on 0 and on 10000)
                    rw::call(MAKE_OPTION_STATE, {f.addr(0x78)});
                    moveShared(self->optionState, f.addr(0x78));
                    rw::call(OPTION_LOAD, {self->optionState.obj, sb});
                    audioLayerVolume(f.addr(0x130), 0x7FFFFFFF, rw::load<uint32_t>(self->optionState.obj));
                    audioLayerVolume(f.addr(0x138), 0, rw::load<uint32_t>(self->optionState.obj + 4));
                    audioLayerVolume(f.addr(0x140), 10000, rw::load<uint32_t>(self->optionState.obj + 4));

                    rw::call(SAVEBIN_GET_VEC_U64, {sb, STR_NEW_USERMEDAL, s + 0x104});
                    rw::call(SAVEBIN_GET_VEC_U64, {sb, STR_WILL_LOCK, s + 0x110});
                    rw::call(SAVEBIN_GET_VEC_U64, {sb, STR_WILL_UNLOCK, s + 0x11C});
                    rw::call(SAVEBIN_GET_VEC_INT, {sb, STR_STAGE_OLD_SELECTS, s + 0x128});
                    rw::call(SAVEBIN_GET_BOOL, {sb, STR_TUTORIAL_STAGE_START, s + 0xE4});
                    rw::call(SAVEBIN_GET_VEC_INT, {sb, STR_NEW_AVATAR_PARTS, s + 0x140});
                    rw::call(SAVEBIN_GET_VEC_INT, {sb, STR_NEW_AVATAR_PARTS_SET, s + 0x14C});
                    rw::call(SAVEBIN_GET_VEC_INT, {sb, STR_NEW_AVATAR_BOARD, s + 0x158});
                    rw::call(SAVEBIN_GET_BOOL, {sb, STR_POP_BENEFIT_STONE, s + 0xA8});
                    releaseAt(f.addr(0x84));
                    releaseAt(f.addr(0x8C));
                    releaseAt(f.addr(0x9C));
                    releaseAt(f.addr(0xAC));
                    loaded = 1;
                    releaseAt(f.addr(0xBC));
                }
                releaseAt(f.addr(0x104));
            }
            rw::call(SAVEBIN_DTOR, {sb});
        }
        if (fromFile != 0 || loaded != 0) break;
        // second try: the save file
        rw::call(LOAD_DATA_FROM_FILE, {f.addr(0x70)});
        uint32_t obj = rw::load<uint32_t>(f.addr(0x70)), ctrl = rw::load<uint32_t>(f.addr(0x74));
        rw::save<uint32_t>(f.addr(0x70), 0);
        rw::save<uint32_t>(f.addr(0x74), 0);
        rw::save<uint32_t>(data, obj);
        uint32_t old = rw::load<uint32_t>(data + 4);
        rw::save<uint32_t>(data + 4, ctrl);
        fromFile = 1;
        releaseIf(old);
    }
    if (!loaded) reset(self);
    rw::call(DATA_DTOR, {cocosData});
    releaseAt(data + 4);
}
RW_FUNCTION_NOVERIFY(0x4E6C0C, "AppState::load", AppState::load);

void AppState::reset(AppState *self) {
    guest::Frame f(0x9C);
    const uint32_t s = addr(self);
    // a new UUID into PersonalInfo
    rw::call(UUID_CREATE, {f.addr(0x68)});
    rw::call(AEABI_MEMCPY, {f.addr(0x88), rw::load<uint32_t>(rw::load<uint32_t>(f.addr(0x68)) + 4), 0x10});
    writeFrom(self->personalInfo.obj, f.addr(0x88), 0, 0x10);
    self->stageState.obj = 0;
    uint32_t old = self->stageState.ctrl;
    self->stageState.ctrl = 0;
    releaseIf(old);
    clearVector(self->newUserMedalIDs, 8);
    clearVector(self->willLockUserMedalIDs, 8);
    clearVector(self->willUnlockUserMedalIDs, 8);
    clearVector(self->clearStageOldSelects, 4);
    clearVector(self->clearTutorialPopupIDs, 4);
    {
        // newAvatarPartsIDs: the same clear, compiled differently (two steps)
        guest::Vector &v = self->newAvatarPartsIDs;
        uint32_t end = v.end, begin = v.begin;
        if (end != begin) {
            uint32_t n = (end - 4 - begin) >> 2;
            uint32_t notN = ~n;
            uint32_t e1 = end + (notN << 2);
            v.end = e1;
            if (e1 != begin) {
                uint32_t r5 = end + ((0xFFFFFFFEu - n) << 2);
                uint32_t r2 = ~((r5 - begin) >> 2);
                v.end = end + ((notN + r2) << 2);
            }
        }
    }
    clearVector(self->newAvatarPartsSetIDs, 4);
    clearVector(self->newAvatarBoardIDs, 4);
    rw::call(SAVEBIN_CLEAR, {s + 0xAC});

    // fresh sort / filter states with their defaults
    for (uint32_t k = 0; k < 5; k++) {
        uint32_t tmp = f.addr(0x60 - 8 * k);
        rw::call(MAKE_MEDAL_SFS, {tmp, FILTER_STATE_NAMES + 4 * k});
        moveShared(self->medalSortFilter[k], tmp);
        if (k == 0) {
            rw::set<uint8_t>(rw::host_ptr<void>(self->medalSortFilter[0].obj), 0xE, 1);   // isAttack
        } else if (k == 1) {
            for (uint32_t b = 0xE; b <= 0x12; b++)   // isAttack .. isEvolve
                rw::set<uint8_t>(rw::host_ptr<void>(self->medalSortFilter[1].obj), b, 1);
        }
    }
    rw::call(MAKE_AVATAR_BFS, {f.addr(0x38), FILTER_STATE_NAMES + 0x14});
    moveShared(self->avatarBoardFilter, f.addr(0x38));
    rw::call(MAKE_ADVENTURE_SFS, {f.addr(0x30), FILTER_STATE_NAMES + 0x18});
    moveShared(self->adventureSelectFilter, f.addr(0x30));
    rw::call(MAKE_KEYBLADE_SFS, {f.addr(0x28)});
    moveShared(self->powerUpKeyBladeSortFilter, f.addr(0x28));
    rw::call(MAKE_KEYBLADE_SFS, {f.addr(0x20)});
    moveShared(self->selectKeyBladeSortFilter, f.addr(0x20));
    rw::call(MAKE_SKILL_SFS, {f.addr(0x18)});
    moveShared(self->sellSkillSortFilter, f.addr(0x18));
    rw::call(MAKE_OPTION_STATE, {f.addr(0x10)});
    moveShared(self->optionState, f.addr(0x10));
    // default options: full volumes, all notifications on
    void *opt = rw::host_ptr<void>(self->optionState.obj);
    rw::set<uint32_t>(opt, 0, ONE_F);
    rw::set<uint32_t>(opt, 4, ONE_F);
    rw::set<uint8_t>(opt, 9, 1);
    rw::set<uint8_t>(opt, 8, 1);
    rw::set<uint8_t>(opt, 10, 1);
    audioLayerVolume(f.addr(0x80), 0x7FFFFFFF, rw::load<uint32_t>(self->optionState.obj));
    audioLayerVolume(f.addr(0x78), 0, rw::load<uint32_t>(self->optionState.obj + 4));
    audioLayerVolume(f.addr(0x70), 10000, rw::load<uint32_t>(self->optionState.obj + 4));
    save(self);
    releaseAt(f.addr(0x6C));
}
RW_FUNCTION_NOVERIFY(0x4E892C, "AppState::reset", AppState::reset);

void AppState::save(AppState *self) {
    if (isMainThread() == 1) {
        saveMain(self);
        return;
    }
    // std::unique_lock<std::mutex> lock(saveMutex); saveRequested = true; saveDone.wait(lock);
    guest::Frame f(0x14);
    const uint32_t lock = f.addr(8);
    rw::save<uint32_t>(lock, addr(&self->saveMutex));
    rw::save<uint8_t>(lock + 4, 1);
    rw::call(MUTEX_LOCK, {addr(&self->saveMutex)});
    self->saveRequested = 1;
    rw::call(COND_WAIT, {addr(&self->saveDone), lock});
    if (rw::load<uint8_t>(lock + 4)) rw::call(MUTEX_UNLOCK, {rw::load<uint32_t>(lock)});
}
RW_FUNCTION_NOVERIFY(0x4E8DC8, "AppState::save", AppState::save);

void AppState::update(AppState *self, float) {
    if (self->saveRequested == 0) return;
    saveMain(self);
    uint32_t mutex = addr(&self->saveMutex);
    rw::call(MUTEX_LOCK, {mutex});
    self->saveRequested = 0;
    rw::call(COND_NOTIFY, {addr(&self->saveDone)});
    rw::call(MUTEX_UNLOCK, {mutex});
}
RW_FUNCTION_NOVERIFY(0x4E8E4C, "AppState::update", AppState::update);

void AppState::saveMain(AppState *self) {
    guest::Frame f(0x60);
    const uint32_t s = addr(self);
    const uint32_t sb = f.addr(0x48);
    for (uint32_t k = 0; k < 16; k += 4) rw::save<uint32_t>(sb + k, 0);
    rw::save<uint32_t>(sb + 16, ONE_F);
    rw::call(SAVEBIN_SET_U32, {sb, STR_VERSION, SAVE_FORMAT_VERSION});
    rw::call(SAFEBUFFER_SERIALIZE, {f.addr(0x40), self->personalInfo.obj});
    rw::call(SAVEBIN_SET_DATA, {sb, STR_PERSONAL, f.addr(0x40)});
    releaseAt(f.addr(0x44));
    rw::call(SAVEBIN_SET_S32, {sb, STR_MASTER_REVISION, s + 0x90});
    rw::call(SAVEBIN_SET_VEC_INT, {sb, STR_MASTER_FILE_REVISIONS, s + 0x94});
    rw::call(SAVEBIN_SET_S32, {sb, STR_RESOURCE_REVISION, s + 0xA0});
    rw::call(SAVEBIN_SET_U32, {sb, STR_RESOURCE_SIZE, s + 0xA4});
    rw::call(SAVEBIN_SET_UMAP, {sb, STR_EVENT_RESOURCES, s + 0xC0});
    rw::call(SAVEBIN_SET_VEC_INT, {sb, STR_STAGE_OLD_SELECTS, s + 0x128});
    rw::call(SAVEBIN_SET_BOOL, {sb, STR_TUTORIAL_STAGE_START, s + 0xE4});
    rw::call(SAVEBIN_SET_S32, {sb, STR_POS_NORMAL, s + 0xD4});
    rw::call(SAVEBIN_SET_S32, {sb, STR_POS_RAID, s + 0xD8});
    rw::call(SAVEBIN_SET_S32, {sb, STR_POS_COLOSSEUM, s + 0xDC});
    rw::call(SAVEBIN_SET_S32, {sb, STR_POS_STAGE, s + 0xE0});
    for (int k = 0; k < 6; k++)
        if (self->medalSortFilter[k].obj) rw::call(MEDAL_SFS_SAVE, {self->medalSortFilter[k].obj, sb});
    struct Blob { const guest::SharedPtr &state; uint32_t tmp, key; };
    const Blob blobs[] = {{self->powerUpKeyBladeSortFilter, 0x38, STR_POWERUP_KEYBLADE},
                          {self->selectKeyBladeSortFilter, 0x30, STR_EQUIP_KEYBLADE},
                          {self->sellSkillSortFilter, 0x28, STR_SELL_SKILL}};
    for (const Blob &b : blobs) {
        if (!b.state.obj) continue;
        rw::call(SAFEBUFFER_SERIALIZE, {f.addr(b.tmp), b.state.obj});
        rw::call(SAVEBIN_SET_DATA, {sb, b.key, f.addr(b.tmp)});
        releaseAt(f.addr(b.tmp + 4));
    }
    if (self->optionState.obj) rw::call(OPTION_SAVE, {self->optionState.obj, sb});
    if (self->avatarBoardFilter.obj) rw::call(AVATAR_BFS_SAVE, {self->avatarBoardFilter.obj, sb});
    if (self->adventureSelectFilter.obj) rw::call(ADVENTURE_SFS_SAVE, {self->adventureSelectFilter.obj, sb});
    rw::call(SAVEBIN_SET_VEC_U64, {sb, STR_NEW_USERMEDAL, s + 0x104});
    rw::call(SAVEBIN_SET_VEC_U64, {sb, STR_WILL_LOCK, s + 0x110});
    rw::call(SAVEBIN_SET_VEC_U64, {sb, STR_WILL_UNLOCK, s + 0x11C});
    rw::call(SAVEBIN_SET_VEC_INT, {sb, STR_NEW_AVATAR_PARTS, s + 0x140});
    rw::call(SAVEBIN_SET_VEC_INT, {sb, STR_NEW_AVATAR_PARTS_SET, s + 0x14C});
    rw::call(SAVEBIN_SET_VEC_INT, {sb, STR_NEW_AVATAR_BOARD, s + 0x158});
    uint32_t pm = rw::call(PURCHASE_GET_INSTANCE, {});
    rw::call(PURCHASE_SERIALIZE, {f.addr(0x20), pm});
    rw::call(SAVEBIN_SET_DATA, {sb, STR_PURCHASE, f.addr(0x20)});
    rw::call(SAVEBIN_SERIALIZE, {f.addr(0x18), s + 0xAC});
    rw::call(SAVEBIN_SET_DATA, {sb, STR_TRACKING, f.addr(0x18)});
    releaseAt(f.addr(0x1C));
    rw::call(SAVEBIN_SET_BOOL, {sb, STR_POP_BENEFIT_STONE, s + 0xA8});
    // the blob: UserDefault "data" (flushed) and the save file
    rw::call(SAVEBIN_SERIALIZE, {f.addr(0x10), sb});
    uint32_t blob = rw::load<uint32_t>(f.addr(0x10));
    if (blob) {
        const uint32_t cocosData = f.addr(8);
        rw::call(DATA_CTOR, {cocosData});
        blob = rw::load<uint32_t>(f.addr(0x10));
        rw::call(DATA_FAST_SET, {cocosData, rw::load<uint32_t>(blob + 4), rw::load<uint32_t>(blob + 8)});
        uint32_t ud = rw::call(USERDEFAULT_GET_INSTANCE, {});
        rw::call(USERDEFAULT_SET_DATA, {ud, STR_DATA, cocosData});
        ud = rw::call(USERDEFAULT_GET_INSTANCE, {});
        rw::call(USERDEFAULT_FLUSH, {ud});
        rw::call(DATA_FAST_SET, {cocosData, 0, 0});   // the bytes belong to the blob
        rw::call(SAVE_DATA_TO_FILE, {f.addr(0x10)});
        rw::call(DATA_DTOR, {cocosData});
    }
    releaseAt(f.addr(0x14));
    releaseAt(f.addr(0x24));
    rw::call(SAVEBIN_DTOR, {sb});
}
RW_FUNCTION_NOVERIFY(0x4E8E8C, "AppState::saveMain", AppState::saveMain);

// ------------------------------------------------------------------------------------------ AP
// AP now (stored AP + one per recovery interval since the stored date, capped at max AP); *secondsToNext =
// seconds until the next point (0 when full or the date lies in the future).
int32_t AppState::getCurrentAP(const AppState *, const int32_t *secondsToNext) {
    guest::Frame f(0x34);
    const SafeBuffer *status = buffer(rw::load<uint32_t>(rw::call(APIMANAGER_GET_INSTANCE, {}) + API_USER_STATUS));
    int32_t ap = status->get32(STATUS_AP);
    status = buffer(rw::load<uint32_t>(rw::call(APIMANAGER_GET_INSTANCE, {}) + API_USER_STATUS));
    int32_t maxAp = status->get32(STATUS_AP_MAX);
    uint32_t statusAddr = rw::load<uint32_t>(rw::call(APIMANAGER_GET_INSTANCE, {}) + API_USER_STATUS);
    const uint32_t date = f.addr(0x24);
    readTo(statusAddr, date, STATUS_AP_DATE, 8);
    int32_t interval = int32_t(rw::call(DATE_INTERVAL_SINCE_NOW, {date}));
    uint32_t elapsed = 0u - uint32_t(interval);
    int32_t result = ap;
    int32_t toNext = 0;
    if (ap < maxAp && interval <= 0) {
        rw::call(MISC_CREATE, {f.addr(0x1C), uint32_t(MISC_AP_RECOVERY)});
        int32_t minutes = buffer(rw::load<uint32_t>(f.addr(0x1C)))->get32(4);
        uint32_t period = 60u * uint32_t(minutes);
        releaseAt(f.addr(0x20));
        uint32_t points = rw::call(DIVSI3, {elapsed, period});
        uint32_t now = points + uint32_t(ap);
        if (now >= uint32_t(maxAp)) {
            result = maxAp;
        } else {
            uint32_t rem = rw::call(MODSI3, {elapsed, period});
            toNext = int32_t(period - rem);
            result = int32_t(now);
        }
    }
    if (secondsToNext) rw::store32(secondsToNext, uint32_t(toNext));
    return result;
}
RW_FUNCTION_NOVERIFY(0x4EA5E8, "AppState::getCurrentAP", AppState::getCurrentAP);

// Seconds until AP is full (0 when it is, or without user status / misc row, or a future date).
int32_t AppState::getFullChargeApSec(const AppState *) {
    guest::Frame f(0x3C);
    uint32_t statusSlot = rw::call(APIMANAGER_GET_INSTANCE, {}) + API_USER_STATUS;
    if (rw::load<uint32_t>(statusSlot) == 0) return 0;
    rw::call(MISC_CREATE, {f.addr(0x24), uint32_t(MISC_AP_RECOVERY)});
    int32_t result = 0;
    uint32_t misc = rw::load<uint32_t>(f.addr(0x24));
    if (misc) {
        int32_t ap = buffer(rw::load<uint32_t>(statusSlot))->get32(STATUS_AP);
        int32_t maxAp = buffer(rw::load<uint32_t>(statusSlot))->get32(STATUS_AP_MAX);
        const uint32_t date = f.addr(0x1C);
        readTo(rw::load<uint32_t>(statusSlot), date, STATUS_AP_DATE, 8);
        int32_t interval = int32_t(rw::call(DATE_INTERVAL_SINCE_NOW, {date}));
        uint32_t elapsed = 0u - uint32_t(interval);
        if (ap < maxAp && interval < 1) {
            uint32_t period = 60u * uint32_t(buffer(rw::load<uint32_t>(f.addr(0x24)))->get32(4));
            uint32_t now = rw::call(DIVSI3, {elapsed, period}) + uint32_t(ap);
            if (uint32_t(maxAp) > now) {
                uint32_t rem = rw::call(MODSI3, {elapsed, period});
                result = int32_t((uint32_t(maxAp) - 1 - now) * period + period - rem);
            }
        }
    }
    releaseAt(f.addr(0x28));
    return result;
}
RW_FUNCTION_NOVERIFY(0x4EA728, "AppState::getFullChargeApSec", AppState::getFullChargeApSec);

// ------------------------------------------------------------------------------------------ stage state
// (StageState::create builds a SafeBuffer, whose key is seeded from the clock)
void AppState::createStageState(AppState *self, uint32_t playable) {
    guest::Frame f(0x10);
    rw::call(STAGE_STATE_CREATE, {f.addr(4), playable});
    moveShared(self->stageState, f.addr(4));
}
RW_FUNCTION_NOVERIFY(0x4EA860, "AppState::createStageState", AppState::createStageState);

void AppState::destroyStageState(AppState *self) {
    self->stageState.obj = 0;
    uint32_t old = self->stageState.ctrl;
    self->stageState.ctrl = 0;
    releaseIf(old);
}
RW_FUNCTION(0x4EA8C8, "AppState::destroyStageState", AppState::destroyStageState);

// ------------------------------------------------------------------------------------------ IDs as text
// "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" from the 16 UUID bytes in PersonalInfo
void AppState::getUUIDString(guest::String *out, const AppState *self) {
    guest::Frame f(0x42C);
    const uint32_t uuid = f.addr(0x418), text = f.addr(0x14);   // text: a StringBuffer (1024 chars + length)
    readTo(self->personalInfo.obj, uuid, 0, 0x10);
    rw::save<uint32_t>(text + 0x400, 0);
    rw::save<uint8_t>(text, 0);
    for (uint32_t k = 0; k < 16; k++) {
        rw::call(STRINGBUFFER_FCONCAT, {text, STR_HEX_BYTE, rw::load<uint8_t>(uuid + k)});
        if (k <= 9 && ((1u << k) & 0x2A8u)) rw::call(STRINGBUFFER_CONCAT, {text, STR_DASH});   // after bytes 3, 5, 7, 9
    }
    uint32_t n = rw::call(STRLEN, {text});
    rw::call(STRING_INIT, {addr(out), text, n});
}
RW_FUNCTION(0x4EA8E0, "AppState::getUUIDString", AppState::getUUIDString);

// The 32-character native session ID kept in BRIDGEInfo
void AppState::getNativeSessionIDString(guest::String *out, const AppState *self) {
    guest::Frame f(0x434);
    const uint32_t id = f.addr(0x410), text = f.addr(0xC);
    readTo(self->bridgeInfo.obj, id, 0, 0x20);
    rw::call(STRINGBUFFER_CTOR, {text, STR_SESSION_FMT, 0x20, id});
    uint32_t n = rw::call(STRLEN, {text});
    rw::call(STRING_INIT, {addr(out), text, n});
}
RW_FUNCTION(0x4EAA30, "AppState::getNativeSessionIDString", AppState::getNativeSessionIDString);

void AppState::setNativeSessionID(AppState *self, uint32_t id) {
    guest::Frame f(0x34);
    const uint32_t buf = f.addr(0x10);
    readTo(self->bridgeInfo.obj, buf, 0, 0x20);   // read, then overwritten whole
    rw::call(AEABI_MEMCPY, {buf, id, 0x20});
    writeFrom(self->bridgeInfo.obj, buf, 0, 0x20);
}
RW_FUNCTION(0x4EAAC4, "AppState::setNativeSessionID", AppState::setNativeSessionID);

// ------------------------------------------------------------------------------------------ revisions
RW_FUNCTION(0x4EAB48, "AppState::getMasterRevision", AppState::getMasterRevision);
RW_FUNCTION(0x4EAB4E, "AppState::getMasterFileRevisions", AppState::getMasterFileRevisions);

// (every setter that persists calls save(): files, so not shadow-run)
void AppState::setMasterRevisions(AppState *self, int32_t revision, const guest::Vector *fileRevisions) {
    self->masterRevision = revision;
    assignVector(self->masterFileRevisions, fileRevisions, VECTOR_INT_ASSIGN, 0, 4);
    save(self);
}
RW_FUNCTION_NOVERIFY(0x4EAB52, "AppState::setMasterRevisions", AppState::setMasterRevisions);

RW_FUNCTION(0x4EAB9A, "AppState::getResourceRevision", AppState::getResourceRevision);
RW_FUNCTION(0x4EABA0, "AppState::getResourceSize", AppState::getResourceSize);

void AppState::setAvatarBoardPosNormal(AppState *self, int32_t pos) { self->avatarBoardPosNormal = pos; save(self); }
RW_FUNCTION_NOVERIFY(0x4EABA6, "AppState::setAvatarBoardPosNormal", AppState::setAvatarBoardPosNormal);
RW_FUNCTION(0x4EABB2, "AppState::getAvatarBoardPosNormal", AppState::getAvatarBoardPosNormal);
void AppState::setAvatarBoardPosRaid(AppState *self, int32_t pos) { self->avatarBoardPosRaid = pos; save(self); }
RW_FUNCTION_NOVERIFY(0x4EABB8, "AppState::setAvatarBoardPosRaid", AppState::setAvatarBoardPosRaid);
RW_FUNCTION(0x4EABC4, "AppState::getAvatarBoardPosRaid", AppState::getAvatarBoardPosRaid);
void AppState::setAvatarBoardPosColosseum(AppState *self, int32_t pos) {
    self->avatarBoardPosColosseum = pos;
    save(self);
}
RW_FUNCTION_NOVERIFY(0x4EABCA, "AppState::setAvatarBoardPosColosseum", AppState::setAvatarBoardPosColosseum);
RW_FUNCTION(0x4EABD6, "AppState::getAvatarBoardPosColosseum", AppState::getAvatarBoardPosColosseum);
void AppState::setAvatarBoardPosStage(AppState *self, int32_t pos) { self->avatarBoardPosStage = pos; save(self); }
RW_FUNCTION_NOVERIFY(0x4EABDC, "AppState::setAvatarBoardPosStage", AppState::setAvatarBoardPosStage);
RW_FUNCTION(0x4EABE8, "AppState::getAvatarBoardPosStage", AppState::getAvatarBoardPosStage);

void AppState::setResourceRevisionAndSize(AppState *self, int32_t revision, uint32_t size) {
    self->resourceRevision = revision;
    self->resourceSize = size;
    save(self);
}
RW_FUNCTION_NOVERIFY(0x4EABEE, "AppState::setResourceRevisionAndSize", AppState::setResourceRevisionAndSize);

// ------------------------------------------------------------------------------------------ event resources
// size 0 removes the id
void AppState::setEventResourceIDAndSize(AppState *self, uint32_t id, uint32_t size) {
    guest::Frame f(0xC);
    rw::save<uint32_t>(f.addr(4), id);
    if (size != 0) rw::save<uint32_t>(rw::call(UMAP_UU_AT, {addr(&self->eventResources), f.addr(4)}), size);
    else rw::call(UMAP_UU_ERASE, {addr(&self->eventResources), f.addr(4)});
    save(self);
}
RW_FUNCTION_NOVERIFY(0x4EAC00, "AppState::setEventResourceIDAndSize", AppState::setEventResourceIDAndSize);

RW_FUNCTION(0x4EACF4, "AppState::getEventResources", AppState::getEventResources);
RW_FUNCTION(0x4EACF8, "AppState::isStartTutorialStage", AppState::isStartTutorialStageGet);

void AppState::setIsStartTutorialStage(AppState *self, uint32_t value) { self->isStartTutorialStage = uint8_t(value); }
RW_FUNCTION(0x4EACFE, "AppState::setIsStartTutorialStage", AppState::setIsStartTutorialStage);

// The evResource ids with a resource (field 8 != 0) whose date (field 12, when set) has come, that are not
// downloaded yet (not in eventResources). Uses the clock: not shadow-run.
void AppState::enumerateEventResourceIDsToDownload(guest::Vector *out, AppState *self) {
    guest::Frame f(0x44);
    out->begin = 0;
    out->end = 0;
    out->cap = 0;
    const uint32_t ids = f.addr(0x30);   // std::set<int>: begin node, root (its address = end node), size
    uint32_t mgr = rw::call(MDM_GET_INSTANCE, {});
    rw::call(MDM_GET_IDS, {ids, mgr, TABLE_EV_RESOURCE});
    const uint32_t endNode = ids + 4;
    uint32_t node = rw::load<uint32_t>(ids);
    while (node != endNode) {
        uint32_t id = rw::load<uint32_t>(node + 16);
        rw::call(EVRESOURCE_CREATE, {f.addr(0x28), id});
        uint32_t row = rw::load<uint32_t>(f.addr(0x28));
        readTo(row, f.addr(0x3C), 8, 4);
        if (rw::load<uint32_t>(f.addr(0x3C)) != 0) {
            const uint32_t date = f.addr(0x20);
            rw::store8(f.at<uint8_t>(0x27), uint8_t(*f.at<uint8_t>(0x27) & 0xFD));
            readTo(rw::load<uint32_t>(f.addr(0x28)), date, 0xC, 8);
            if ((rw::load<uint8_t>(date + 7) & 2) && int32_t(rw::call(DATE_INTERVAL_SINCE_NOW, {date})) >= 0) {
                rw::save<uint32_t>(f.addr(0x1C), rw::load<uint32_t>(node + 16));
                if (rw::call(UMAP_UU_FIND, {addr(&self->eventResources), f.addr(0x1C)}) == 0) {
                    uint32_t value = rw::load<uint32_t>(node + 16);
                    rw::save<uint32_t>(f.addr(0x18), value);
                    uint32_t end = out->end;
                    if (end < out->cap) {
                        uint32_t base = 0;
                        if (end != 0) {
                            rw::save<uint32_t>(end, value);
                            base = out->end;
                        }
                        out->end = base + 4;
                    } else {
                        rw::call(VECTOR_UINT_PUSH_SLOW, {addr(out), f.addr(0x18)});
                    }
                }
            }
        }
        releaseAt(f.addr(0x2C));
        // ++iterator (in-order successor)
        uint32_t right = rw::load<uint32_t>(node + 4);
        if (right) {
            node = right;
            while (rw::load<uint32_t>(node)) node = rw::load<uint32_t>(node);
        } else {
            uint32_t child;
            do {
                child = node;
                node = rw::load<uint32_t>(child + 8);
            } while (rw::load<uint32_t>(node) != child);
        }
    }
    rw::call(SET_INT_DESTROY, {ids, rw::load<uint32_t>(ids + 4)});
}
RW_FUNCTION_NOVERIFY(0x4EAD04, "AppState::enumerateEventResourceIDsToDownload",
                     AppState::enumerateEventResourceIDsToDownload);

// The server's master / resource revisions differ from ours.
uint32_t AppState::needsUpdateData(AppState *self) {
    rw::call(MDM_GET_INSTANCE, {});
    rw::call(RDM_GET_INSTANCE, {});
    uint32_t info = rw::load<uint32_t>(rw::call(APIMANAGER_GET_INSTANCE, {}) + API_VERSION_INFO);
    if (info == 0) return 0;
    if (rw::load<int32_t>(info + 0x30) != self->masterRevision) return 1;
    if (rw::load<int32_t>(info + 0x2C) != self->resourceRevision) return 1;
    return 0;
}
RW_FUNCTION(0x4EAE5C, "AppState::needsUpdateData", AppState::needsUpdateData);

// The given "major.minor.patch" is newer than the app's version.
uint32_t AppState::needsUpdateApp(AppState *self, uint32_t version) {
    guest::Frame f(0x18);
    rw::call(VERSION_CTOR, {f.addr(4), version});
    return rw::call(VERSION_GT, {f.addr(4), addr(self)});
}
RW_FUNCTION(0x4EAE90, "AppState::needsUpdateApp", AppState::needsUpdateApp);

// ------------------------------------------------------------------------------------------ sort / filter states
// A fresh keyblade / skill sort state: sort index = the default text value (2/100100003), ascending flag 1.
// (make_shared builds a SafeBuffer, keyed from the clock: not shadow-run)
static void createSortFilterState(guest::SharedPtr *out, uint32_t makeShared) {
    guest::Frame f(0x1C);
    rw::call(makeShared, {addr(out)});
    rw::call(TEXTDATA_GET, {f.addr(0x10), 2, uint32_t(SORT_DEFAULT_TEXT)});
    uint32_t value = rw::call(ATOI, {rw::load<uint32_t>(rw::load<uint32_t>(f.addr(0x10)) + 4)});
    releaseAt(f.addr(0x14));
    uint32_t state = out->obj;
    rw::save<uint32_t>(f.addr(0xC), value);
    writeFrom(state, f.addr(0xC), 0, 4);
    rw::save<uint8_t>(f.addr(0xB), 1);
    writeFrom(state, f.addr(0xB), 4, 1);
}

void AppState::createKeyBladeSortFilterState(guest::SharedPtr *out, AppState *) {
    createSortFilterState(out, MAKE_KEYBLADE_SFS);
}
RW_FUNCTION_NOVERIFY(0x4EAF24, "AppState::createKeyBladeSortFilterState", AppState::createKeyBladeSortFilterState);

void AppState::createSkillSortFilterState(guest::SharedPtr *out, AppState *) {
    createSortFilterState(out, MAKE_SKILL_SFS);
}
RW_FUNCTION_NOVERIFY(0x4EAFDC, "AppState::createSkillSortFilterState", AppState::createSkillSortFilterState);

// shared_ptr getters (copy out, add a reference)
void AppState::copyShared(guest::SharedPtr *out, const guest::SharedPtr &member) {
    out->obj = member.obj;
    uint32_t ctrl = member.ctrl;
    out->ctrl = ctrl;
    if (ctrl) guest::addRef(ctrl);
}

#define APPSTATE_SHARED_GETTER(name, member)                                                   static void AppState_##name(guest::SharedPtr *out, AppState *self) { AppState::copyShared(out, self->member); }

APPSTATE_SHARED_GETTER(getDeckMedalSortFilterState, medalSortFilter[0])
RW_FUNCTION(0x4EB094, "AppState::getDeckMedalSortFilterState", AppState_getDeckMedalSortFilterState);
APPSTATE_SHARED_GETTER(getMixedMedalSortFilterState, medalSortFilter[1])
RW_FUNCTION(0x4EB0AC, "AppState::getMixedMedalSortFilterState", AppState_getMixedMedalSortFilterState);
APPSTATE_SHARED_GETTER(getEvolveMedalSortFilterState, medalSortFilter[2])
RW_FUNCTION(0x4EB0C4, "AppState::getEvolveMedalSortFilterState", AppState_getEvolveMedalSortFilterState);
APPSTATE_SHARED_GETTER(getSellMedalSortFilterState, medalSortFilter[3])
RW_FUNCTION(0x4EB0DC, "AppState::getSellMedalSortFilterState", AppState_getSellMedalSortFilterState);
APPSTATE_SHARED_GETTER(getMedalListSortFilterState, medalSortFilter[4])
RW_FUNCTION(0x4EB0F4, "AppState::getMedalListSortFilterState", AppState_getMedalListSortFilterState);
APPSTATE_SHARED_GETTER(getEquipSkillMedalSortFilterState, medalSortFilter[5])
RW_FUNCTION(0x4EB10C, "AppState::getEquipSkillMedalSortFilterState", AppState_getEquipSkillMedalSortFilterState);
APPSTATE_SHARED_GETTER(getPowerUpKeyBladeSortFilterState, powerUpKeyBladeSortFilter)
RW_FUNCTION(0x4EB124, "AppState::getPowerUpKeyBladeSortFilterState", AppState_getPowerUpKeyBladeSortFilterState);
APPSTATE_SHARED_GETTER(getSelectKeyBladeSortFilterState, selectKeyBladeSortFilter)
RW_FUNCTION(0x4EB13C, "AppState::getSelectKeyBladeSortFilterState", AppState_getSelectKeyBladeSortFilterState);
APPSTATE_SHARED_GETTER(getSellSkillSortFilterState, sellSkillSortFilter)
RW_FUNCTION(0x4EB154, "AppState::getSellSkillSortFilterState", AppState_getSellSkillSortFilterState);
APPSTATE_SHARED_GETTER(getOptionState, optionState)
RW_FUNCTION(0x4EB16C, "AppState::getOptionState", AppState_getOptionState);
APPSTATE_SHARED_GETTER(getAvatarBoardFilterState, avatarBoardFilter)
RW_FUNCTION(0x4EB188, "AppState::getAvatarBoardFilterState", AppState_getAvatarBoardFilterState);
APPSTATE_SHARED_GETTER(getAdventureSelectFilterState, adventureSelectFilter)
RW_FUNCTION(0x4EB1A0, "AppState::getAdventureSelectFilterState", AppState_getAdventureSelectFilterState);

// ------------------------------------------------------------------------------------------ user medal ID lists
RW_FUNCTION(0x4EB1BC, "AppState::getNewUserMedalIDs", AppState::getNewUserMedalIDs);
RW_FUNCTION(0x4EB346, "AppState::getWillLockUserMedalIDs", AppState::getWillLockUserMedalIDs);
RW_FUNCTION(0x4EB506, "AppState::getWillUnLockUserMedalIDs", AppState::getWillUnLockUserMedalIDs);

namespace {
// set (sorted copy, or clear when null), then save
void setMedalIDs(AppState *self, guest::Vector &v, const guest::Vector *ids) {
    assignVector(v, ids, VECTOR_U64_ASSIGN, SORT_U64, 8);
    AppState::save(self);
}
// append unless present, then save
void addMedalIDUnique(AppState *self, guest::Vector &v, uint64_t id) {
    if (find64(v, id) != v.end) return;
    pushBack64(v, id);
    AppState::save(self);
}
// erase the first match, then save (nothing when absent)
void removeMedalID(AppState *self, guest::Vector &v, uint64_t id) {
    uint32_t pos = find64(v, id);
    if (pos == v.end) return;
    eraseAt(v, pos, 8, 3);
    AppState::save(self);
}

void setNewUserMedalIDs(AppState *self, const guest::Vector *ids) { setMedalIDs(self, self->newUserMedalIDs, ids); }
void setWillLockUserMedalIDs(AppState *self, const guest::Vector *ids) {
    setMedalIDs(self, self->willLockUserMedalIDs, ids);
}
void setWillUnlockUserMedalIDs(AppState *self, const guest::Vector *ids) {
    setMedalIDs(self, self->willUnlockUserMedalIDs, ids);
}
// (no uniqueness check, no save)
void addNewUserMedalID(AppState *self, uint64_t id) { pushBack64(self->newUserMedalIDs, id); }
void addWillLockUserMedalID(AppState *self, uint64_t id) { addMedalIDUnique(self, self->willLockUserMedalIDs, id); }
void addWillUnLockUserMedalID(AppState *self, uint64_t id) {
    addMedalIDUnique(self, self->willUnlockUserMedalIDs, id);
}
void removeNewUserMedalID(AppState *self, uint64_t id) { removeMedalID(self, self->newUserMedalIDs, id); }
void removeWillLockUserMedalID(AppState *self, uint64_t id) { removeMedalID(self, self->willLockUserMedalIDs, id); }
void removeWillUnLockUserMedalID(AppState *self, uint64_t id) {
    removeMedalID(self, self->willUnlockUserMedalIDs, id);
}
}  // namespace

RW_FUNCTION_NOVERIFY(0x4EB1C4, "AppState::setNewUserMedalIDs", setNewUserMedalIDs);
RW_FUNCTION(0x4EB258, "AppState::addNewUserMedalID", addNewUserMedalID);
RW_FUNCTION_NOVERIFY(0x4EB2C4, "AppState::removeNewUserMedalID", removeNewUserMedalID);
RW_FUNCTION_NOVERIFY(0x4EB350, "AppState::setWillLockUserMedalIDs", setWillLockUserMedalIDs);
RW_FUNCTION_NOVERIFY(0x4EB3E4, "AppState::addWillLockUserMedalID", addWillLockUserMedalID);
RW_FUNCTION_NOVERIFY(0x4EB484, "AppState::removeWillLockUserMedalID", removeWillLockUserMedalID);
RW_FUNCTION_NOVERIFY(0x4EB510, "AppState::setWillUnlockUserMedalIDs", setWillUnlockUserMedalIDs);
RW_FUNCTION_NOVERIFY(0x4EB5A4, "AppState::addWillUnLockUserMedalID", addWillUnLockUserMedalID);
RW_FUNCTION_NOVERIFY(0x4EB644, "AppState::removeWillUnLockUserMedalID", removeWillUnLockUserMedalID);

// ------------------------------------------------------------------------------------------ int ID lists
RW_FUNCTION(0x4EB6C6, "AppState::getClearStageOldSelects", AppState::getClearStageOldSelects);
RW_FUNCTION(0x4EB716, "AppState::getClearTutorialPopupIDs", AppState::getClearTutorialPopupIDs);
RW_FUNCTION(0x4EBB44, "AppState::getNewAvatarPartsIDs", AppState::getNewAvatarPartsIDs);
RW_FUNCTION(0x4EBCBE, "AppState::getNewAvatarPartsSetIDs", AppState::getNewAvatarPartsSetIDs);
RW_FUNCTION(0x4EBE3A, "AppState::getNewAvatarBoardIDs", AppState::getNewAvatarBoardIDs);

namespace {
// copy (no sort), save
void setClearStageOldSelects(AppState *self, const guest::Vector *ids) {
    assignVector(self->clearStageOldSelects, ids, VECTOR_INT_ASSIGN, 0, 4);
    AppState::save(self);
}
// sorted copy, no save
void setClearTutorialPopupIDs(AppState *self, const guest::Vector *ids) {
    assignVector(self->clearTutorialPopupIDs, ids, VECTOR_INT_ASSIGN, SORT_INT, 4);
}
// append (no uniqueness check, no save)
void addClearTutorialPopupIDs(AppState *self, int32_t id) {
    pushBack32(self->clearTutorialPopupIDs, uint32_t(id), VECTOR_INT_PUSH_SLOW);
}

// avatar parts / parts set / board: sorted copy + save; append + save; erase without save
void setAvatarIDs(AppState *self, guest::Vector &v, const guest::Vector *ids) {
    assignVector(v, ids, VECTOR_INT_ASSIGN, SORT_INT, 4);
    AppState::save(self);
}
void setNewAvatarPartsIDs(AppState *self, const guest::Vector *ids) { setAvatarIDs(self, self->newAvatarPartsIDs, ids); }
void setNewAvatarPartsSetIDs(AppState *self, const guest::Vector *ids) {
    setAvatarIDs(self, self->newAvatarPartsSetIDs, ids);
}
void setNewAvatarBoardIDs(AppState *self, const guest::Vector *ids) { setAvatarIDs(self, self->newAvatarBoardIDs, ids); }
void addNewAvatarPartsIDs(AppState *self, int32_t id) {
    pushBack32(self->newAvatarPartsIDs, uint32_t(id), VECTOR_INT_PUSH_SLOW);
    AppState::save(self);
}
void addNewAvatarPartsSetIDs(AppState *self, int32_t id) {
    pushBack32(self->newAvatarPartsSetIDs, uint32_t(id), VECTOR_INT_PUSH_SLOW);
    AppState::save(self);
}
// The duplicate check looks in newAvatarPartsIDs, not in the board list (game bug, kept).
void addNewAvatarBoardIDs(AppState *self, int32_t id) {
    if (find32(self->newAvatarPartsIDs, uint32_t(id)) != self->newAvatarPartsIDs.end) return;
    pushBack32(self->newAvatarBoardIDs, uint32_t(id), VECTOR_INT_PUSH_SLOW);
    AppState::save(self);
}
void removeIntID(guest::Vector &v, int32_t id) {
    uint32_t pos = find32(v, uint32_t(id));
    if (pos != v.end) eraseAt(v, pos, 4, 2);
}
void removeNewAvatarPartsIDs(AppState *self, int32_t id) { removeIntID(self->newAvatarPartsIDs, id); }
void removeNewAvatarPartsSetIDs(AppState *self, int32_t id) { removeIntID(self->newAvatarPartsSetIDs, id); }
void removeNewAvatarBoardIDs(AppState *self, int32_t id) { removeIntID(self->newAvatarBoardIDs, id); }
}  // namespace

RW_FUNCTION_NOVERIFY(0x4EB6CE, "AppState::setClearStageOldSelects", setClearStageOldSelects);
RW_FUNCTION(0x4EB720, "AppState::setClearTutorialPopupIDs", setClearTutorialPopupIDs);
RW_FUNCTION(0x4EB7A8, "AppState::addClearTutorialPopupIDs", addClearTutorialPopupIDs);
RW_FUNCTION_NOVERIFY(0x4EBB4C, "AppState::setNewAvatarPartsIDs", setNewAvatarPartsIDs);
RW_FUNCTION_NOVERIFY(0x4EBBDC, "AppState::addNewAvatarPartsIDs", addNewAvatarPartsIDs);
RW_FUNCTION(0x4EBC54, "AppState::removeNewAvatarPartsIDs", removeNewAvatarPartsIDs);
RW_FUNCTION_NOVERIFY(0x4EBCC8, "AppState::setNewAvatarPartsSetIDs", setNewAvatarPartsSetIDs);
RW_FUNCTION_NOVERIFY(0x4EBD58, "AppState::addNewAvatarPartsSetIDs", addNewAvatarPartsSetIDs);
RW_FUNCTION(0x4EBDD0, "AppState::removeNewAvatarPartsSetIDs", removeNewAvatarPartsSetIDs);
RW_FUNCTION_NOVERIFY(0x4EBE44, "AppState::setNewAvatarBoardIDs", setNewAvatarBoardIDs);
RW_FUNCTION_NOVERIFY(0x4EBED4, "AppState::addNewAvatarBoardIDs", addNewAvatarBoardIDs);
RW_FUNCTION(0x4EBF6C, "AppState::removeNewAvatarBoardIDs", removeNewAvatarBoardIDs);

// ------------------------------------------------------------------------------------------ tutorial popups
// Already shown: ids 40 / 41 never count; else in clearTutorialPopupIDs, or not shown by dispTutorialCheck.
// (works on the singleton, whatever `self` is)
uint32_t AppState::isClearTutorialPopupIDs(AppState *, int32_t id) {
    if ((uint32_t(id) & ~1u) == 0x28) return 0;
    rw::call(0x4E66DD, {});   // AppState::getInstance() (result unused: the instance address is a constant)
    const AppState *app = rw::host_ptr<AppState>(INSTANCE);
    if (find32(app->clearTutorialPopupIDs, uint32_t(id)) != app->clearTutorialPopupIDs.end) return 1;
    return rw::call(DISP_TUTORIAL_CHECK, {uint32_t(id)}) ^ 1;
}
RW_FUNCTION(0x4EB814, "AppState::isClearTutorialPopupIDs", AppState::isClearTutorialPopupIDs);

uint32_t AppState::isClearTutorialPopupIDsInPhase(AppState *, int32_t id) {
    rw::call(0x4E66DD, {});   // AppState::getInstance()
    const AppState *app = rw::host_ptr<AppState>(INSTANCE);
    return find32(app->clearTutorialPopupIDs, uint32_t(id)) != app->clearTutorialPopupIDs.end ? 1 : 0;
}
RW_FUNCTION(0x4EB870, "AppState::isClearTutorialPopupIDsInPhase", AppState::isClearTutorialPopupIDsInPhase);

void AppState::resetClearTutorialPopupIDs(AppState *self) { clearVector(self->clearTutorialPopupIDs, 4); }
RW_FUNCTION(0x4EB8B4, "AppState::resetClearTutorialPopupIDs", AppState::resetClearTutorialPopupIDs);

// Register the close function of tutorial popup `id` (unless that popup was already shown: then out = empty).
// The previous function runs first. out = a copy of the new function.
// (runs arbitrary callbacks: not shadow-run)
void AppState::setTutorialCloseFunc(GuestFunction *out, AppState *self, GuestFunction *func, int32_t id) {
    guest::Frame f(0x3C);
    rw::call(0x4E66DD, {});   // AppState::getInstance()
    if (isClearTutorialPopupIDs(rw::host_ptr<AppState>(INSTANCE), id) == 1) {
        out->impl = 0;
        return;
    }
    const uint32_t tmp = f.addr(0x18);
    const uint32_t closeFunc = addr(&self->tutorialCloseFunc);
    rw::call(FUNCTION_COPY, {tmp, closeFunc});
    if (rw::load<uint32_t>(tmp + 16)) rw::call(FUNCTION_CALL, {tmp});
    rw::call(FUNCTION_ASSIGN, {closeFunc, addr(func)});
    self->tutorialCloseId = id;
    rw::call(FUNCTION_COPY, {addr(out), closeFunc});
    destroyFunction(rw::load<uint32_t>(tmp + 16), tmp);
}
RW_FUNCTION_NOVERIFY(0x4EB8D8, "AppState::setTutorialCloseFunc", AppState::setTutorialCloseFunc);

// The close function when it belongs to `id` (or id 0), else an empty function.
void AppState::getTutorialCloseFunc(GuestFunction *out, AppState *self, int32_t id) {
    if (id == 0 || self->tutorialCloseId == id) rw::call(FUNCTION_COPY, {addr(out), addr(&self->tutorialCloseFunc)});
    else out->impl = 0;
}
RW_FUNCTION(0x4EBA3C, "AppState::getTutorialCloseFunc", AppState::getTutorialCloseFunc);

// Take the close function of `id` (or any, for 0), clear it, and run it.
void AppState::execTutorialCloseFunc(AppState *self, int32_t id) {
    if (id != 0 && self->tutorialCloseId != id) return;
    guest::Frame f(0x34);
    const uint32_t tmp = f.addr(0x10);
    const uint32_t closeFunc = addr(&self->tutorialCloseFunc);
    rw::call(FUNCTION_COPY, {tmp, closeFunc});
    destroyFunction(self->tutorialCloseFunc.impl, closeFunc);
    self->tutorialCloseFunc.impl = 0;
    self->tutorialCloseId = 0;
    uint32_t impl = rw::load<uint32_t>(tmp + 16);
    if (impl) {
        rw::call(FUNCTION_CALL, {tmp});
        destroyFunction(rw::load<uint32_t>(tmp + 16), tmp);
    }
}
RW_FUNCTION_NOVERIFY(0x4EBA5C, "AppState::execTutorialCloseFunc", AppState::execTutorialCloseFunc);

// ------------------------------------------------------------------------------------------ misc flags
void AppState::setPopBenefitStone(AppState *self, uint32_t value) {
    self->popBenefitStone = uint8_t(value);
    save(self);
}
RW_FUNCTION_NOVERIFY(0x4EBFD6, "AppState::setPopBenefitStone", AppState::setPopBenefitStone);

// A tracking event was recorded (its key exists in trackingEvents).
uint32_t AppState::checkTrackingEvent(AppState *self, uint32_t name) {
    guest::Frame f(0x10);
    rw::call(SAVEBIN_GET_DATA, {f.addr(4), addr(&self->trackingEvents), name});
    uint32_t obj = rw::load<uint32_t>(f.addr(4));
    releaseAt(f.addr(8));
    return obj != 0 ? 1 : 0;
}
RW_FUNCTION(0x4EBFE4, "AppState::checkTrackingEvent", AppState::checkTrackingEvent);

void AppState::saveTrackingEvent(AppState *self, uint32_t name) {
    guest::Frame f(0x10);
    rw::save<uint8_t>(f.addr(0xB), 1);
    rw::call(SAVEBIN_SET_U8, {addr(&self->trackingEvents), name, f.addr(0xB)});
    save(self);
}
RW_FUNCTION_NOVERIFY(0x4EC044, "AppState::saveTrackingEvent", AppState::saveTrackingEvent);
