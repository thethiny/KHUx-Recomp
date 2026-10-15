// AppDelegate: the cocos2d::Application of the game (start-up, background / foreground), and AppConfig, the
// key/value settings read from json/app_config_android.json, then json/app_config_common.json.
#include "FuncThunks.hpp"

namespace appdelegate {

namespace {
constexpr uint32_t APPLICATION_CTOR = 0xA3E72D;        // cocos2d::Application::Application()
constexpr uint32_t APPLICATION_DTOR = 0xA3E751;        // cocos2d::Application::~Application()
constexpr uint32_t CONFIGURATION_GET_INSTANCE = 0x8E861D;
constexpr uint32_t CONFIGURATION_SET_VALUE = 0x8E8737;  // (config, const string& key, const Value&)
constexpr uint32_t VALUE_CTOR_CSTR = 0x901C39;          // cocos2d::Value::Value(const char*)
constexpr uint32_t VALUE_DTOR = 0x901FBD;
constexpr uint32_t STRING_INIT = 0x47CD93;              // sub_47CD92: std::string::__init(this, const char*, size)
constexpr uint32_t STRING_ASSIGN = 0x48D811;            // sub_48D810: std::string::assign(const char*, size)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t DIRECTOR_GET_INSTANCE = 0x8EE519;
constexpr uint32_t DIRECTOR_ADD_SCHEDULER = 0x8EEC15;     // (director, int)
constexpr uint32_t DIRECTOR_REMOVE_SCHEDULER = 0x8F0C49;  // (director, int)
constexpr uint32_t DIRECTOR_SET_OPENGL_VIEW = 0x8EF96F;
constexpr uint32_t DIRECTOR_RUN_WITH_SCENE = 0x8F06CD;
constexpr uint32_t GLVIEW_CREATE = 0xA3F2AD;              // cocos2d::GLView::create(const string&)
constexpr uint32_t APP_SET_TEXTURE_CACHE_LIMIT = 0x799F05;
constexpr uint32_t APP_GET_FRAME_WIDTH = 0x799B31;        // AppGetGrameFrameWidth()
constexpr uint32_t APP_GET_FRAME_HEIGHT = 0x799D21;       // AppGetGrameFrameHeight()
constexpr uint32_t AUDIO_SYSTEM_INIT = 0x5C0BA1;
constexpr uint32_t AUDIO_MANAGER_GET_INSTANCE = 0x5BECFD;
constexpr uint32_t AUDIO_MANAGER_RELEASE = 0x5BEEC1;
constexpr uint32_t AUDIO_MANAGER_SUSPEND = 0x5BEEE5;
constexpr uint32_t AUDIO_MANAGER_RESUME = 0x5BEEED;
constexpr uint32_t APPSTATE_SET_MAIN_THREAD = 0x4E6BD5;
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;
constexpr uint32_t APPSTATE_GET_OPTION_STATE = 0x4EB16D;  // (shared_ptr out, appState)
constexpr uint32_t APPSTATE_GET_FULL_CHARGE_AP_SEC = 0x4EA729;
constexpr uint32_t PURCHASE_MANAGER_GET_INSTANCE = 0x4D7A71;
constexpr uint32_t STORE_MANAGER_GET_INSTANCE = 0x4E0DD5;  // iap::StoreManager::getInstance
constexpr uint32_t STORE_MANAGER_REFRESH_RECEIPT = 0x4E1521;
constexpr uint32_t SCENE_MANAGER_CREATE_SCENE = 0x60C249;
constexpr uint32_t LOCAL_NOTIFICATION_CANCEL = 0x4E0D5D;
constexpr uint32_t LOCAL_NOTIFICATION_SHOW = 0x4E0C59;    // (string*, seconds, id)
constexpr uint32_t TEXTDATA_GET = 0x4928AD;
constexpr uint32_t DISPATCH_CUSTOM_EVENT = 0x8F3F69;      // EventDispatcher::dispatchCustomEvent(name, data)
constexpr uint32_t TEXTURE_SIZE_IN_PIXELS = 0x914B07;     // Texture2D::getContentSizeInPixels() -> const Size&
constexpr uint32_t VFS_READ_DATA = 0x8DD46D;
constexpr uint32_t JSON_STACK_CTOR = 0x49A659;            // rapidjson::internal::Stack(allocator, capacity)
constexpr uint32_t JSON_STACK_DTOR = 0x49A5FF;
constexpr uint32_t JSON_PARSE_STREAM = 0x49BD61;          // GenericDocument::ParseStream<0>(GenericStringStream&)
constexpr uint32_t JSON_MEMBER = 0x4B1BB5;                // GenericValue::operator[](const char*)

constexpr uint32_t GOT_APPDELEGATE_VTABLE = 0xFD7078;
constexpr uint32_t STR_GL_PROJECTION_KEY = 0xE09DF7;     // "cocos2d.x.gl.projection"
constexpr uint32_t STR_2D = 0xE09E0F;                    // "2d"
constexpr uint32_t STR_MY_GAME = 0xE09E12;               // "My Game"
constexpr uint32_t STR_WILL_ENTER_FOREGROUND = 0xE09E1A; // "applicationWillEnterForeground"
constexpr uint32_t STR_APP_CONFIG_ANDROID = 0xE09DDA;    // "json/app_config_android.json"
constexpr uint32_t STR_APP_CONFIG_COMMON = 0xE09DBE;     // "json/app_config_common.json"

constexpr uint32_t DIRECTOR_OPENGL_VIEW = 0x9C;           // Director::_openGLView
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;      // Director::_eventDispatcher
constexpr uint32_t DIRECTOR_VSLOT_SET_ANIMATION_INTERVAL = 0xC;   // vtable byte offset (double interval)
constexpr uint32_t DIRECTOR_VSLOT_STOP_ANIMATION = 0x10;
constexpr uint32_t DIRECTOR_VSLOT_START_ANIMATION = 0x14;
constexpr uint32_t GLVIEW_VSLOT_SET_DESIGN_RESOLUTION = 0x30;    // (w, h, ResolutionPolicy)
constexpr int32_t NOTIFICATION_AP_FULL_TEXT = 105200001;

// libc++ std::string destructor on a guest string object
void destroyString(uint32_t s) {
    if (sys::ld8(s) & 1) guest::deleteObject(sys::ld32(s + 8));
}
}  // namespace

uint32_t construct(uint32_t self) {
    guest::Frame f(0x30);
    const uint32_t key = f.addr(4), value = f.addr(0x10);
    rw::call(APPLICATION_CTOR, {self});
    sys::st32(self, sys::ld32(GOT_APPDELEGATE_VTABLE) + 8);
    uint32_t config = rw::call(CONFIGURATION_GET_INSTANCE, {});
    rw::call(STRING_INIT, {key, STR_GL_PROJECTION_KEY, 23});
    rw::call(VALUE_CTOR_CSTR, {value, STR_2D});
    rw::call(CONFIGURATION_SET_VALUE, {config, key, value});
    rw::call(VALUE_DTOR, {value});
    destroyString(key);
    return self;
}
RW_FUNCTION(0x79A210, "AppDelegate::AppDelegate", construct);

uint32_t destruct(uint32_t self) {
    sys::st32(self, sys::ld32(GOT_APPDELEGATE_VTABLE) + 8);
    uint32_t audio = rw::call(AUDIO_MANAGER_GET_INSTANCE, {});
    rw::call(AUDIO_MANAGER_RELEASE, {audio});
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    rw::call(DIRECTOR_REMOVE_SCHEDULER, {director, 0x7FFFFFFF});
    rw::call(APPLICATION_DTOR, {self});
    return self;
}
RW_FUNCTION(0x79A2F8, "AppDelegate::~AppDelegate", destruct);

void deletingDestruct(uint32_t self) { guest::deleteObject(destruct(self)); }
RW_FUNCTION(0x79A34C, "AppDelegate::~AppDelegate (deleting)", deletingDestruct);

uint32_t applicationDidFinishLaunching(uint32_t) {
    guest::Frame f(0x20);
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    rw::call(APP_SET_TEXTURE_CACHE_LIMIT, {0});
    uint32_t view = sys::ld32(director + DIRECTOR_OPENGL_VIEW);
    if (!view) {
        const uint32_t name = f.addr(0xC);
        rw::call(STRING_INIT, {name, STR_MY_GAME, 7});
        view = rw::call(GLVIEW_CREATE, {name});
        destroyString(name);
        rw::call(DIRECTOR_SET_OPENGL_VIEW, {director, view});
    }
    uint32_t setDesign = sys::ld32(sys::ld32(view) + GLVIEW_VSLOT_SET_DESIGN_RESOLUTION);
    uint32_t w = rw::call(APP_GET_FRAME_WIDTH, {});
    uint32_t h = rw::call(APP_GET_FRAME_HEIGHT, {});
    rw::call(setDesign, {view, w, h, 2});   // ResolutionPolicy::FIXED_WIDTH
    // setAnimationInterval(1.0 / 60) (the double in r2:r3; r1 still holds the function address)
    uint32_t setInterval = sys::ld32(sys::ld32(director) + DIRECTOR_VSLOT_SET_ANIMATION_INTERVAL);
    rw::call(setInterval, {director, setInterval, 0x11111111, 0x3F911111});
    rw::call(DIRECTOR_ADD_SCHEDULER, {director, 0x7FFFFFFF});
    rw::call(AUDIO_SYSTEM_INIT, {});
    rw::call(APPSTATE_SET_MAIN_THREAD, {});
    rw::call(APPSTATE_GET_INSTANCE, {});
    uint32_t purchase = rw::call(PURCHASE_MANAGER_GET_INSTANCE, {});
    if (sys::ld32(purchase + 12)) {
        uint32_t store = rw::call(STORE_MANAGER_GET_INSTANCE, {});
        rw::call(STORE_MANAGER_REFRESH_RECEIPT, {store});
    }
    uint32_t scene = rw::call(SCENE_MANAGER_CREATE_SCENE, {});
    rw::call(DIRECTOR_RUN_WITH_SCENE, {director, scene});
    rw::call(LOCAL_NOTIFICATION_CANCEL, {0});
    return 1;
}
RW_FUNCTION(0x79A358, "AppDelegate::applicationDidFinishLaunching", applicationDidFinishLaunching);

// Pause, and schedule the "AP full" notification when the option is on.
void applicationDidEnterBackground(uint32_t) {
    guest::Frame f(0x28);
    const uint32_t text = f.addr(4), message = f.addr(0xC), option = f.addr(0x18);
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    sys::vcallOff(director, DIRECTOR_VSLOT_STOP_ANIMATION, {director});
    uint32_t audio = rw::call(AUDIO_MANAGER_GET_INSTANCE, {});
    rw::call(AUDIO_MANAGER_SUSPEND, {audio});
    rw::call(LOCAL_NOTIFICATION_CANCEL, {0});
    uint32_t state = rw::call(APPSTATE_GET_INSTANCE, {});
    rw::call(APPSTATE_GET_OPTION_STATE, {option, state});
    uint8_t notifyApFull = sys::ld8(sys::ld32(option) + 8);
    if (uint32_t ctrl = sys::ld32(option + 4)) guest::release(ctrl);
    if (!notifyApFull) return;
    state = rw::call(APPSTATE_GET_INSTANCE, {});
    int32_t seconds = int32_t(rw::call(APPSTATE_GET_FULL_CHARGE_AP_SEC, {state}));
    if (seconds < 1) return;
    rw::call(TEXTDATA_GET, {text, 0, uint32_t(NOTIFICATION_AP_FULL_TEXT)});
    uint32_t chars = sys::ld32(sys::ld32(text) + 4);
    uint32_t n = rw::call(STRLEN, {chars});
    rw::call(STRING_INIT, {message, chars, n});
    rw::call(LOCAL_NOTIFICATION_SHOW, {message, uint32_t(seconds), 0});
    destroyString(message);
    if (uint32_t ctrl = sys::ld32(text + 4)) guest::release(ctrl);
}
RW_FUNCTION(0x79A488, "AppDelegate::applicationDidEnterBackground", applicationDidEnterBackground);

void applicationWillEnterForeground(uint32_t) {
    guest::Frame f(0x18);
    const uint32_t name = f.addr(4);
    uint32_t director = rw::call(DIRECTOR_GET_INSTANCE, {});
    sys::vcallOff(director, DIRECTOR_VSLOT_START_ANIMATION, {director});
    uint32_t audio = rw::call(AUDIO_MANAGER_GET_INSTANCE, {});
    rw::call(AUDIO_MANAGER_RESUME, {audio});
    rw::call(LOCAL_NOTIFICATION_CANCEL, {0});
    director = rw::call(DIRECTOR_GET_INSTANCE, {});
    uint32_t dispatcher = sys::ld32(director + DIRECTOR_EVENT_DISPATCHER);
    rw::call(STRING_INIT, {name, STR_WILL_ENTER_FOREGROUND, 30});
    rw::call(DISPATCH_CUSTOM_EVENT, {dispatcher, name, 0});
    destroyString(name);
}
RW_FUNCTION(0x79A584, "AppDelegate::applicationWillEnterForeground", applicationWillEnterForeground);

// ------------------------------------------------------------------------------------------------ AppConfig
// sub_79A03C: the string value of `key` in the JSON object file `path` into *out; true when found.
uint32_t readConfigValue(uint32_t path, uint32_t key, uint32_t out) {
    guest::Frame f(0x60);
    const uint32_t text = f.addr(0xC), data = f.addr(0x18), stream = f.addr(0x20), doc = f.addr(0x28);
    const uint32_t docStack = f.addr(0x38);
    rw::call(VFS_READ_DATA, {data, path});
    uint32_t found = 2;
    uint32_t blob = sys::ld32(data);
    if (blob) {
        rw::call(STRING_INIT, {text, sys::ld32(blob + 4), sys::ld32(blob + 8)});
        // GenericDocument doc (value type null, a 1 KB parse stack, no parse error)
        sys::st32(doc + 12, 0);
        rw::call(JSON_STACK_CTOR, {docStack, 0, 1024});
        sys::st32(doc + 0x28, 0);
        sys::st32(doc + 0x2C, 0);
        uint32_t chars = (sys::ld8(text) & 1) ? sys::ld32(text + 8) : text + 1;
        sys::st32(stream, chars);
        sys::st32(stream + 4, chars);
        rw::call(JSON_PARSE_STREAM, {doc, stream});
        found = 2;
        if (sys::ld32(doc + 0x28) == 0 && sys::ld32(doc + 12) == 3) {   // parsed, and an object
            uint32_t v = rw::call(JSON_MEMBER, {doc, key});
            if (sys::ld8(v + 14) & 0x10) {   // a string
                uint32_t s = sys::ld32(v);
                uint32_t n = rw::call(STRLEN, {s});
                found = 1;
                rw::call(STRING_ASSIGN, {out, s, n});
            }
        }
        rw::call(JSON_STACK_DTOR, {docStack});
        destroyString(text);
    }
    if (uint32_t ctrl = sys::ld32(data + 4)) guest::release(ctrl);
    return found == 1 ? 1 : 0;
}
RW_FUNCTION(0x79A03C, "AppConfig::get (read one config file, sub_79A03C)", readConfigValue);

// AppConfig::get(const char* key) -> std::string (sret): "" when neither file has it.
void appConfigGet(uint32_t out, uint32_t key) {
    sys::st32(out, 0);
    sys::st32(out + 4, 0);
    sys::st32(out + 8, 0);
    if (!readConfigValue(STR_APP_CONFIG_ANDROID, key, out)) readConfigValue(STR_APP_CONFIG_COMMON, key, out);
}
RW_FUNCTION(0x799FD0, "AppConfig::get", appConfigGet);

// ------------------------------------------------------------------------------------------------ texture cache
// AppSetTextureCacheLimit(unsigned reserved): the texture cache may hold 32 MB minus `reserved`. The first time
// (no limit set yet) it also installs the cost function ($_0 below: a texture costs its pixel count).
void appSetTextureCacheLimit(uint32_t reserved) {
    constexpr uint32_t DIRECTOR_GET_TEXTURE_CACHE = 0x8EFE69;    // Director::getTextureCache()
    constexpr uint32_t TEXTURE_CACHE_GET_LIMIT = 0x918219;       // TextureCache::getLimit()
    constexpr uint32_t TEXTURE_CACHE_SET_LIMIT = 0x91822D;       // TextureCache::setLimit(unsigned)
    constexpr uint32_t TEXTURE_CACHE_SET_LIMIT_COST = 0x91821D;  // setLimit(unsigned, const function<unsigned(..)>&)
    constexpr uint32_t COST_LAMBDA_VTABLE = 0xF6EEEC;            // __func<AppSetTextureCacheLimit::$_0> + 8
    uint32_t limit = 0x2000000u - reserved;
    uint32_t cache = rw::call(DIRECTOR_GET_TEXTURE_CACHE, {rw::call(DIRECTOR_GET_INSTANCE, {})});
    if (rw::call(TEXTURE_CACHE_GET_LIMIT, {cache})) {
        rw::call(TEXTURE_CACHE_SET_LIMIT, {cache, limit});
        return;
    }
    guest::Frame f(0x40);
    uint32_t cost = f.addr(8);   // std::function holding $_0 in place
    sysfn::makeLocal(cost, COST_LAMBDA_VTABLE);
    rw::call(TEXTURE_CACHE_SET_LIMIT_COST, {cache, limit, cost});
    sysfn::destroy(cost);
}
RW_FUNCTION(0x799F04, "AppSetTextureCacheLimit(unsigned)", appSetTextureCacheLimit);

// ------------------------------------------------------------------------------------------------ lambdas
// AppSetTextureCacheLimit(unsigned)::$_0 (in this file's translation unit): a texture's size in pixels
// (width * height) for the texture cache's limit.
uint32_t textureCacheCost(uint32_t, const uint32_t *texture) {
    uint32_t size = rw::call(TEXTURE_SIZE_IN_PIXELS, {*texture});
    return sys::f2u(sys::ldf(size) * sys::ldf(size + 4));
}
RW_FUNCTION(0x79A1C2, "AppSetTextureCacheLimit::$_0 (texture pixel count)", textureCacheCost);
RW_FUNCTION(0x79A16C, "AppSetTextureCacheLimit::$_0 __func::~__func", sysfn::nothing);
RW_FUNCTION(0x79A16E, "AppSetTextureCacheLimit::$_0 __func::~__func (deleting)", sysfn::deleteThis);
RW_FUNCTION(0x79A1B8, "AppSetTextureCacheLimit::$_0 __func::destroy", sysfn::nothing);
RW_FUNCTION(0x79A1BA, "AppSetTextureCacheLimit::$_0 __func::destroy_deallocate", sysfn::deleteThis);
RW_FUNCTION(0x79A178, "AppSetTextureCacheLimit::$_0 __func::__clone()", (sysfn::clone<0xF6EEEC, 8>));
RW_FUNCTION(0x79A19C, "AppSetTextureCacheLimit::$_0 __func::__clone(__base*)", (sysfn::cloneInto<0xF6EEEC>));
RW_FUNCTION(0x79A1DC, "AppSetTextureCacheLimit::$_0 __func::target", sysfn::target<0xEACA20>);
RW_FUNCTION(0x79A1FC, "AppSetTextureCacheLimit::$_0 __func::target_type", sysfn::targetType<0xFAC898>);

}  // namespace appdelegate
