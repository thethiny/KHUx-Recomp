// DataDownloader: downloads what the server says is out of date - the master data (when the master revision
// changed), the resource archive (resource revision) and the event resources - one after the other, reporting
// progress and opening a retry dialog on errors. The owner polls update() every frame; `callback(true)` reports
// that the downloader is waiting for the user (busy API / error dialog).
#include "Common.hpp"

using namespace w36;

struct DataDownloader {
    rw::Field<uint32_t> state;             // +0x00 0 idle, 1 master, 2 resources, 3 processing resources, 4 done
    rw::Field<uint8_t> waiting;            // +0x04 a dialog / busy API holds the download
    const uint8_t pad_05[3];
    rw::Field<uint32_t> pending;           // +0x08 bit 0 master, bit 1 resources, bit 2 event resources
    rw::Field<uint8_t> eventResources;     // +0x0C the resource download failed on the event resources
    const uint8_t pad_0d[3];
    rw::Field<uint32_t> progressBytes;     // +0x10 (getProgress out parameter)
    rw::Field<uint32_t> progress;          // +0x14
    rw::Field<uint32_t> totalResources;    // +0x18
    rw::Field<uint32_t> downloaded;        // +0x1C resources finished before the current batch
    const uint8_t callback[0x14];          // +0x20 std::function<void(bool)> (callable at +0x30)

    static constexpr uint32_t CALLBACK = 0x20;
    static constexpr uint32_t STATE_DONE = 4;

    static DataDownloader *construct(DataDownloader *self);
    static DataDownloader *destruct(DataDownloader *self);
    static void downloadResource(DataDownloader *self);
    static uint32_t downloadEventResource(DataDownloader *self);
    static uint32_t init(DataDownloader *self, uint32_t callback);
    static void update(DataDownloader *self, float dt);
    static uint32_t openErrorDialog(DataDownloader *self);
    static uint32_t getNumberOfDownloadedResources(const DataDownloader *self);
};
RW_LAYOUT(DataDownloader, pending, 8);
RW_LAYOUT(DataDownloader, downloaded, 0x1C);
RW_SIZE(DataDownloader, 0x34);

namespace {
using rw::guest_addr;
constexpr uint32_t Device_setKeepScreenOn = rwlib::cocos2d::Device::setKeepScreenOn;
constexpr uint32_t FUNC_VB_CALL = 0x474139;            // std::function<void(bool)>::operator()(bool)
constexpr uint32_t FUNC_VB_ASSIGN = 0x490109;          // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_isBusy = 0x4C3C51;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;     // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executeSystemMaster = 0x4CA721;   // (api, int revision, const function<void(bool)>&)
constexpr uint32_t APIManager_executeSystemResource = 0x4CA8CD; // (api, int revision, const function<void(bool)>&)
constexpr uint32_t APIManager_executeSystemResourceEv = 0x4CAA79;   // (api, vector<unsigned> by value, const function&)
constexpr uint32_t API_SYSTEM = 0x44;                  // APIManager: the system (version) response
constexpr uint32_t API_MAINTENANCE = 0x04;             // APIManager byte
constexpr uint32_t API_ERROR = 0x14;                   // APIManager byte: the last request failed
constexpr uint32_t SYSTEM_RESOURCE_REVISION = 0x2C;
constexpr uint32_t SYSTEM_MASTER_REVISION = 0x30;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_getMasterRevision = 0x4EAB49;
constexpr uint32_t AppState_getResourceRevision = 0x4EAB9B;
constexpr uint32_t AppState_enumerateEventResourceIDsToDownload = 0x4EAD05;   // (vector<unsigned>* out, state)
constexpr uint32_t VECTOR_UINT_COPY = 0x641E91;        // vector<unsigned>(const vector&)
constexpr uint32_t VECTOR_UINT_DTOR = 0x485ABB;
constexpr uint32_t MDM_getInstance = 0x76C091;
constexpr uint32_t MDM_getProgress = 0x76D695;         // (mdm, unsigned* bytes)
constexpr uint32_t MDM_isDownloading = 0x76D5E1;
constexpr uint32_t MDM_startNextDownload = 0x76D40D;
constexpr uint32_t MDM_ERROR = 0x04;                   // MasterDataManager: error kind (0 none)
constexpr uint32_t RDM_getInstance = 0x7728AD;
constexpr uint32_t RDM_getProgress = 0x774715;
constexpr uint32_t RDM_isDownloading = 0x7746F5;
constexpr uint32_t RDM_isProcessing = 0x774711;
constexpr uint32_t RDM_startNextDownload = 0x773C39;
constexpr uint32_t RDM_ERROR = 0x00;                   // ResourceDataManager: error kind (0 none)
constexpr uint32_t RDM_FINISHED = 0x14;                // ResourceDataManager: resources finished
constexpr uint32_t PopupDialog_create = 0x4F3C15;      // (ButtonType, WindowType)
constexpr uint32_t PopupDialog_open = 0x4F420D;        // (dialog, Node*, const string&, function<void(bool)>)
constexpr uint32_t SceneManager_getPopupLayer3 = 0x613E79;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t TextData_get = 0x4928AD;            // (shared_ptr out, Type, id)
constexpr uint32_t STRING_ASSIGN = 0x48D811;           // std::string::assign(const char*, size_t)
constexpr uint32_t VT_API_ERROR_RETRY = 0xF6EC78;      // update() lambda [this]
constexpr uint32_t VT_ERROR_RETRY = 0xF6EC4C;          // openErrorDialog $_1 [this]
constexpr int32_t TEXT_NETWORK_ERROR = 105000001;
constexpr int32_t TEXT_DOWNLOAD_ERROR = 105000002;
constexpr int32_t TEXT_STORAGE_ERROR = 105000009;

uint32_t api() { return call(APIManager_getInstance, {}); }
uint32_t appState() { return call(AppState_getInstance, {}); }
void keepScreenOn(uint32_t on) { call(Device_setKeepScreenOn, {on}); }
void runCallback(DataDownloader *self, uint32_t value) { call(FUNC_VB_CALL, {guest_addr(self) + DataDownloader::CALLBACK, value}); }
// the number of event resources still to download
uint32_t eventResourceCount() {
    Frame f;
    uint32_t ids = f.alloc(12);
    call(AppState_enumerateEventResourceIDsToDownload, {ids, appState()});
    uint32_t n = uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2);
    call(VECTOR_UINT_DTOR, {ids});
    return n;
}
}  // namespace

DataDownloader *DataDownloader::construct(DataDownloader *self) {
    self->state = 0;
    self->waiting = 0;
    self->eventResources = 0;
    rw::set<uint32_t>(self, 0x30, 0);
    self->progressBytes = 0;
    self->progress = 0;
    return self;
}
RW_FUNCTION(0x776926, "DataDownloader::DataDownloader", DataDownloader::construct);

DataDownloader *DataDownloader::destruct(DataDownloader *self) {
    keepScreenOn(0);
    functionDestroy(guest_addr(self) + CALLBACK);
    return self;
}
RW_FUNCTION(0x776936, "DataDownloader::~DataDownloader", DataDownloader::destruct);

void DataDownloader::downloadResource(DataDownloader *self) {
    self->state = 2;
    self->eventResources = 0;
    self->progressBytes = 0;
    self->progress = 0;
    uint32_t a = api();
    uint32_t revision = call(AppState_getResourceRevision, {appState()});
    withEmptyFunction([&](uint32_t fo) { call(APIManager_executeSystemResource, {a, revision, fo}); });
}
RW_FUNCTION(0x776990, "DataDownloader::downloadResource", DataDownloader::downloadResource);

// Start downloading the event resources; 0 when there are none.
uint32_t DataDownloader::downloadEventResource(DataDownloader *self) {
    Frame f;
    uint32_t ids = f.alloc(12);
    call(AppState_enumerateEventResourceIDsToDownload, {ids, appState()});
    uint32_t started = 0;
    if (ld32(ids) != ld32(ids + 4)) {
        self->state = 2;
        self->pending = self->pending | 6;
        self->eventResources = 0;
        self->progressBytes = 0;
        self->progress = 0;
        uint32_t a = api();
        uint32_t copy = f.alloc(12);
        call(VECTOR_UINT_COPY, {copy, ids});
        withEmptyFunction([&](uint32_t fo) { call(APIManager_executeSystemResourceEv, {a, copy, fo}); });
        call(VECTOR_UINT_DTOR, {copy});
        started = 1;
    }
    call(VECTOR_UINT_DTOR, {ids});
    return started;
}
RW_FUNCTION(0x776A38, "DataDownloader::downloadEventResource", DataDownloader::downloadEventResource);

// Work out what is out of date (from the system response) and start the first download; 0 = no system
// response yet.
uint32_t DataDownloader::init(DataDownloader *self, uint32_t callback) {
    self->state = STATE_DONE;
    uint32_t a = api();
    if (ld32(a + API_SYSTEM) == 0) return 0;
    self->pending = 0;
    self->totalResources = 0;
    self->downloaded = 0;
    uint32_t sys = ld32(a + API_SYSTEM);
    bool masterCurrent = true;
    if (sys) {
        uint32_t serverRevision = ld32(sys + SYSTEM_MASTER_REVISION);
        masterCurrent = serverRevision == call(AppState_getMasterRevision, {appState()});
    }
    if (masterCurrent) self->totalResources = self->totalResources + eventResourceCount();
    else self->pending = self->pending | 1;
    sys = ld32(a + API_SYSTEM);
    if (sys) {
        uint32_t serverRevision = ld32(sys + SYSTEM_RESOURCE_REVISION);
        if (serverRevision != call(AppState_getResourceRevision, {appState()})) {
            self->pending = self->pending | 2;
            self->totalResources = self->totalResources + 1;
        }
    }
    uint32_t pending = self->pending;
    if (pending & 1) {
        self->state = 1;
        uint32_t a2 = api();
        uint32_t revision = call(AppState_getMasterRevision, {appState()});
        withEmptyFunction([&](uint32_t fo) { call(APIManager_executeSystemMaster, {a2, revision, fo}); });
    } else if (pending & 2) {
        downloadResource(self);
    } else {
        downloadEventResource(self);
    }
    call(FUNC_VB_ASSIGN, {guest_addr(self) + CALLBACK, callback});
    if (self->state != STATE_DONE) keepScreenOn(1);
    return 1;
}
RW_FUNCTION(0x776B24, "DataDownloader::init", DataDownloader::init);

void DataDownloader::update(DataDownloader *self, float) {
    const uint32_t me = guest_addr(self);
    if ((self->state | 4) == 4) return;
    uint32_t pending = self->pending;
    if (pending & 1) self->progress = call(MDM_getProgress, {call(MDM_getInstance, {}), me + 0x10});
    else if (pending & 2) self->progress = call(RDM_getProgress, {call(RDM_getInstance, {}), me + 0x10});
    if (rw::load<uint8_t>(api() + API_MAINTENANCE)) return;
    if (call(APIManager_isBusy, {api()}) == 1) {
        if (!self->waiting) {
            self->waiting = 1;
            runCallback(self, 1);
        }
        return;
    }
    if (rw::load<uint8_t>(api() + API_ERROR)) {
        if (self->waiting) return;
        keepScreenOn(0);
        uint32_t a = api();
        withLambda(VT_API_ERROR_RETRY, {me}, [&](uint32_t fo) {
            self->waiting = uint8_t(call(APIManager_openErrorDialog, {a, fo, 0}));
        });
        if (self->waiting) runCallback(self, 1);
        return;
    }
    pending = self->pending;
    bool error = false;
    if (pending & 1) {
        if (call(MDM_isDownloading, {call(MDM_getInstance, {})})) return;
        if (ld32(call(MDM_getInstance, {}) + MDM_ERROR) != 0) {
            error = true;
        } else {
            self->pending = self->pending & ~1u;
            self->totalResources = self->totalResources + eventResourceCount();
            if (rw::load<uint8_t>(me + 8) & 2) {
                downloadResource(self);
                return;
            }
            if (downloadEventResource(self) != 0) return;
            pending = self->pending;
        }
    }
    if (!error) {
        if (pending & 2) {
            if (call(RDM_isDownloading, {call(RDM_getInstance, {})}) == 1) {
                self->state = 2;
                return;
            }
            if (call(RDM_isProcessing, {call(RDM_getInstance, {})}) == 1) {
                self->state = 3;
                return;
            }
            if (ld32(call(RDM_getInstance, {}) + RDM_ERROR) != 0) {
                error = true;
            } else {
                self->pending = self->pending & ~2u;
                self->downloaded = self->downloaded + ld32(call(RDM_getInstance, {}) + RDM_FINISHED);
                if (!(rw::load<uint8_t>(me + 8) & 4) && downloadEventResource(self) != 0) return;
            }
        }
        if (!error) {
            keepScreenOn(0);
            self->state = STATE_DONE;
            return;
        }
    }
    if (!self->waiting) self->waiting = uint8_t(openErrorDialog(self));
}
RW_FUNCTION(0x776C98, "DataDownloader::update", DataDownloader::update);

// The retry dialog for the failed download (message by error kind); 1 = opened.
uint32_t DataDownloader::openErrorDialog(DataDownloader *self) {
    const uint32_t me = guest_addr(self);
    Frame f;
    uint32_t message = f.alloc(12);
    st32(message, 0);
    st32(message + 4, 0);
    st32(message + 8, 0);
    int32_t textId = 0;
    bool open = true;
    if (self->pending & 1) {
        switch (ld32(call(MDM_getInstance, {}) + MDM_ERROR)) {
        case 0: open = false; break;
        case 1: textId = TEXT_NETWORK_ERROR; break;
        case 2: textId = TEXT_STORAGE_ERROR; break;
        case 3: textId = TEXT_DOWNLOAD_ERROR; break;
        default: break;
        }
    } else if (self->pending & 2) {
        switch (ld32(call(RDM_getInstance, {}) + RDM_ERROR)) {
        case 0: open = false; break;
        case 1: textId = TEXT_NETWORK_ERROR; break;
        case 2: textId = TEXT_STORAGE_ERROR; break;
        case 3:
            self->eventResources = 1;
            textId = TEXT_STORAGE_ERROR;
            break;
        default: break;
        }
    }
    if (textId) {
        uint32_t text = f.alloc(8);
        call(TextData_get, {text, 0, uint32_t(textId)});
        uint32_t chars = ld32(ld32(text) + 4);
        call(STRING_ASSIGN, {message, chars, host_strlen(chars)});
        sharedRelease(text);
    }
    uint32_t result = 0;
    if (open) {
        keepScreenOn(0);
        result = 1;
        runCallback(self, 1);
        uint32_t dialog = call(PopupDialog_create, {0, 0});
        uint32_t layer = call(SceneManager_getPopupLayer3, {call(sfn::SceneManager_getInstance, {})});
        withLambda(VT_ERROR_RETRY, {me}, [&](uint32_t fo) { call(PopupDialog_open, {dialog, layer, message, fo}); });
        call(AudioPlaySystemSEPopUp, {});
    }
    strFree(message);
    return result;
}
RW_FUNCTION(0x776F24, "DataDownloader::openErrorDialog", DataDownloader::openErrorDialog);

uint32_t DataDownloader::getNumberOfDownloadedResources(const DataDownloader *self) {
    uint32_t before = self->downloaded;
    return ld32(call(RDM_getInstance, {}) + RDM_FINISHED) + before;
}
RW_FUNCTION(0x77719C, "DataDownloader::getNumberOfDownloadedResources", DataDownloader::getNumberOfDownloadedResources);

// openErrorDialog $_1 (the dialog's button): resume the download that failed.
static void errorRetryLambda(uint32_t func, const uint8_t *) {
    const uint32_t me = ld32(func + 4);
    DataDownloader *self = rw::host_ptr<DataDownloader>(me);
    runCallback(self, 0);
    self->waiting = 0;
    keepScreenOn(1);
    uint32_t pending = self->pending;
    if (pending & 1) {
        call(MDM_startNextDownload, {call(MDM_getInstance, {})});
        return;
    }
    if (!(pending & 2)) return;
    if (!self->eventResources) {
        call(RDM_startNextDownload, {call(RDM_getInstance, {})});
        return;
    }
    if (pending & 4) DataDownloader::downloadEventResource(self);
    else DataDownloader::downloadResource(self);
}
RW_FUNCTION(0x77720A, "DataDownloader::openErrorDialog $_1 __func::operator()", errorRetryLambda);

// std::__vector_base<{std::string, std::string, int}>::~__vector_base (28-byte elements, destroyed from the back)
static uint32_t stringPairVectorDtor(uint32_t vec) {
    uint32_t begin = ld32(vec);
    if (begin) {
        for (uint32_t end = ld32(vec + 4); end != begin; end = ld32(vec + 4)) {
            uint32_t item = end - 0x1C;
            st32(vec + 4, item);
            strFree(item + 0xC);
            strFree(item);
        }
        call(fn::operator_delete, {ld32(vec)});
    }
    return vec;
}
RW_FUNCTION(0x7762BC, "std::__vector_base<{string, string, int}>::~__vector_base", stringPairVectorDtor);
