// SceneDownload: the data download the title opens when the client's master / resource revisions differ from the
// server's (AppState::needsUpdateData) or event resources are missing - a progress label over a DataDownloader -
// and the title menu's "download the data again" confirmation that removes the local data first.
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t SceneDownload_ctor = 0x67F961, SceneDownload_create = 0x67F36D, SceneDownload_finalize = 0x6C8221;
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create, Layer_init = rwlib::cocos2d::Layer::init, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t DataDownloader_init = 0x776B25, DataDownloader_update = 0x776C99;
constexpr uint32_t DataDownloader_getNumberOfDownloadedResources = 0x77719D;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance, Director_getVisibleOrigin = rwlib::cocos2d::Director::getVisibleOrigin;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;
constexpr uint32_t Label_createWithTTF = rwlib::cocos2d::Label::createWithTTF__RKNS_10_ttfConfigERKNSt3__112basic_stringIcNS4_11char_traitsIcEENS4_9allocatorIcEEEENS_14TextHAlignmentEi, Vec2_ctor_ = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor_ = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t FunctionV_assign = 0x4EB9CD;            // std::function<void()>::operator=(function&&)
constexpr uint32_t FunctionLayer_assign = 0x612E29;        // std::function<cocos2d::Layer*()>::operator=
constexpr uint32_t FunctionB_assign = 0x490109;            // std::function<void(bool)>::operator=
constexpr uint32_t ResourceDataManager_getInstance = 0x7728AD, ResourceDataManager_mount = 0x773355;
constexpr uint32_t ResourceDataManager_remove = 0x7736DD;
constexpr uint32_t MasterDataManager_getInstance = 0x76C091, MasterDataManager_remove = 0x76CD4D;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_setAvatarBoardPos[4] = {0x4EABA7, 0x4EABB9, 0x4EABCB, 0x4EABDD};   // normal, raid,
                                                                                               // colosseum, stage
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99, APIManager_executeUserGet = 0x4C6081;
constexpr uint32_t APIManager_executeRankingParadeGet = 0x4D3181, SceneNotice_callPrepareAPI = 0x733AD9;
constexpr uint32_t SceneUtil_freeTutorialConvertData = 0x47250D;
constexpr uint32_t PopupDialog_create = 0x4F3C15, PopupDialog_open = 0x4F420D;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t StringBuffer_ctor = 0x49140D, GOT_NOTICE_FACTORY = 0xFD693C;
constexpr uint32_t S_Font = 0xDF9F4D, S_Empty = 0xDF8A1A;
constexpr uint32_t VT_DownloadDone = 0xF672B4;             // init: DataDownloader's callback {}
constexpr uint32_t VT_Fn210 = 0xF67288, VT_Fn228 = 0xF6725C;   // init: the two layer callbacks {this}
constexpr uint32_t VT_UserGot = 0xF671AC;                  // finalize: executeUserGet's answer {}
constexpr uint32_t VT_ConvertFreed = 0xF671D8, VT_Prepared = 0xF67204, VT_ParadeGot = 0xF67230;
constexpr uint32_t VT_RemoveDataOk = 0xF64180;             // the title's "download again" dialog answer {}
constexpr uint32_t VT_ToTitle = 0xF641AC;                  // ... the layer after removing the data {}
constexpr uint32_t VT_AfterDownload = 0xF641D8;            // ... SceneDownload's next layer {}
constexpr uint32_t V_init = 0x248, V_release = 4, V_addChild = 0xF0, V_setPosition = 0x4C, V_setString = 0x2A8;
// SceneDownload (a Layer): +0x210 / +0x228 callbacks, +0x288 the DataDownloader (state +0x288, total +0x298,
// done +0x29C, resources +0x2A0), +0x2C0 the label, +0x2C4 finalized, +0x2C8 the next layer's factory
constexpr uint32_t F_FN_210 = 0x210, F_FN_228 = 0x228, F_DOWNLOADER = 0x288, F_STATE = 0x288;
constexpr uint32_t F_TOTAL = 0x298, F_DONE = 0x29C, F_COUNT = 0x2A0, F_LABEL = 0x2C0, F_FINALIZED = 0x2C4;
constexpr uint32_t F_NEXT = 0x2C8;
constexpr uint32_t SM_RETRY_FLAG = 0x4CC, SM_RETRY = 0x548;

float f32(uint32_t bits) { float f; std::memcpy(&f, &bits, 4); return f; }
uint64_t dbits(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }
// done * 100 / total as the game computes it (single precision), widened to double for the format
uint64_t percent(uint32_t done, uint32_t total) {
    if (!total) return 0;
    const float p = (float(done) * 100.0f) / float(total);
    return dbits(double(p));
}

// the retry callback cleared (byte +0x4CC set, an empty function<void(bool)> at +0x548)
void clearRetry() {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    st8(sm + SM_RETRY_FLAG, 1);
    call(FunctionB_assign, {sm + SM_RETRY, none});
    functionDestroy(none);
}
void setLabelText(uint32_t label, uint32_t text) {
    Frame f;
    const uint32_t s = newStr(f, text);
    vcall(label, V_setString, {label, s});
    strFree(s);
}
}  // namespace

namespace scene_download {

uint32_t create(uint32_t) {
    const uint32_t layer = call(fn::operator_new, {0x2E0});
    call(SceneDownload_ctor, {layer});
    if (vcall(layer, V_init, {layer}) == 1) {
        call(Ref_autorelease, {layer});
        return layer;
    }
    vcall(layer, V_release, {layer});
    return 0;
}
RW_FUNCTION(0x67F36C, "SceneDownload::create", scene_download::create);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneDownload_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x6C7F3C, "SceneDownload::createScene", scene_download::createScene);

// the downloader started, a 36-point label 20 pixels above the screen centre, the two callbacks
uint32_t init(uint32_t self) {
    if (call(Layer_init, {self}) != 1) return 0;
    {
        Frame f;
        const uint32_t done = newFunction(f, VT_DownloadDone, {});
        call(DataDownloader_init, {self + F_DOWNLOADER, done});
        functionDestroy(done);
    }
    Frame f;
    const uint32_t origin = f.alloc(8), size = f.alloc(8);
    call(Director_getVisibleOrigin, {origin, call(Director_getInstance, {})});
    call(Director_getVisibleSize, {size, call(Director_getInstance, {})});
    // TTFConfig: the font, size 36, no glyphs / custom glyphs, no distance field, no outline, no italics / bold
    const uint32_t config = f.alloc(0x24);
    strInit(config, S_Font, 0x22);
    st32(config + 0xC, 36);
    st32(config + 0x10, 0);
    st32(config + 0x14, 0);
    st8(config + 0x18, 0);
    st32(config + 0x1C, 0);
    st8(config + 0x20, 0);
    st8(config + 0x21, 0);
    const uint32_t text = newStr(f, S_Empty, 0);
    uint32_t label = call(Label_createWithTTF, {config, text, 0, 0});
    st32(self + F_LABEL, label);
    strFree(text);
    label = ld32(self + F_LABEL);
    const float x = f32(ld32(origin)) + f32(ld32(size)) * 0.5f;
    const float y = (f32(ld32(origin + 4)) + f32(ld32(size + 4)) * 0.5f) + -20.0f;
    {
        Frame g;
        const uint32_t pos = g.alloc(8);
        call(Vec2_ctor_, {pos, fbits(x), fbits(y)});
        vcall(label, V_setPosition, {label, pos});
        call(Vec2_dtor_, {pos});
    }
    vcall(self, V_addChild, {self, ld32(self + F_LABEL)});
    {
        Frame g;
        const uint32_t fo = newFunction(g, VT_Fn210, {self});
        call(FunctionV_assign, {self + F_FN_210, fo});
        functionDestroy(fo);
    }
    {
        Frame g;
        const uint32_t fo = newFunction(g, VT_Fn228, {self});
        call(FunctionV_assign, {self + F_FN_228, fo});
        functionDestroy(fo);
    }
    strFree(config);
    call(Vec2_dtor_, {origin});
    return 1;
}
RW_FUNCTION(0x6C7F60, "SceneDownload::init", scene_download::init);

// once: when the API is idle (state 0 or 5) the resources are mounted, then the next layer - the given factory,
// or the user data again and the notices
uint32_t finalize(uint32_t self) {
    if (ld8(self + F_FINALIZED)) return 0;
    st8(self + F_FINALIZED, 1);
    const uint32_t apiState = ld32(call(fn::APIManager_getInstance, {}));
    if (apiState != 0 && apiState != 5) return 0;
    call(ResourceDataManager_mount, {call(ResourceDataManager_getInstance, {})});
    const uint32_t next = ld32(self + F_NEXT + 16);
    const uint32_t sm = sceneManager();
    if (next) {
        call(SceneManager_changeSceneLayer, {sm, self + F_NEXT, 1, 1, 1});
        return 0;
    }
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        st8(sm + SM_RETRY_FLAG, 1);
        call(FunctionB_assign, {sm + SM_RETRY, none});
        functionDestroy(none);
    }
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_UserGot, {});
        call(APIManager_executeUserGet, {api, fo});
        functionDestroy(fo);
    }
    smain::changeSceneLayer(ld32(GOT_NOTICE_FACTORY));
    return 0;
}
RW_FUNCTION(0x6C8220, "SceneDownload::finalize", scene_download::finalize);

// the downloader ticked; the label shows its progress: 1 the master data (percent), 2 / 3 resource n of m
// (percent), 4 done (then finalize). Returns 0x86C (left in r0).
uint32_t update(uint32_t self, uint32_t dt) {
    call(DataDownloader_update, {self + F_DOWNLOADER, dt});
    const uint32_t state = ld32(self + F_STATE);
    const uint32_t label = ld32(self + F_LABEL);
    if (state == 1) {
        const uint32_t done = ld32(self + F_DONE), total = ld32(self + F_TOTAL);
        Frame f;
        const uint32_t td = textData(f, 109130026);
        const uint64_t p = percent(done, total);
        const uint32_t buf = f.alloc(0x404);
        call(StringBuffer_ctor, {buf, textDataStr(td), uint32_t(p), uint32_t(p >> 32)});
        sharedRelease(td);
        setLabelText(label, buf);
    } else if (state == 2 || state == 3) {
        const uint32_t done = ld32(self + F_DONE), total = ld32(self + F_TOTAL);
        Frame f;
        const uint32_t td = textData(f, 109130022);
        const uint32_t n = 2 * call(DataDownloader_getNumberOfDownloadedResources, {self + F_DOWNLOADER}) +
                           (state == 2 ? 1 : 2);
        const uint32_t of = 2 * ld32(self + F_COUNT);
        const uint64_t p = percent(done, total);
        const uint32_t buf = f.alloc(0x404);
        call(StringBuffer_ctor, {buf, textDataStr(td), n, of, uint32_t(p), uint32_t(p >> 32)});
        sharedRelease(td);
        setLabelText(label, buf);
    } else if (state == 4) {
        Frame f;
        const uint32_t td = textData(f, 109130025);
        setLabelText(label, textDataStr(td));
        sharedRelease(td);
        call(SceneDownload_finalize, {self});
    }
    return 0x86C;
}
RW_FUNCTION(0x6C83B4, "SceneDownload::update", scene_download::update);

// finalize's answers: the tutorial conversion data freed, the notices prepared, the parade ranking read
uint32_t convertDataFree(uint32_t, uint32_t okRef) {
    if (!ld8(okRef)) return 0;
    Frame f;
    const uint32_t fo = newFunction(f, VT_ConvertFreed, {});
    call(SceneUtil_freeTutorialConvertData, {fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6C87BC, "SceneDownload::finalize user got (sub_6C87BC)", scene_download::convertDataFree);

uint32_t prepareNotice(uint32_t) {
    clearRetry();
    Frame f;
    const uint32_t fo = newFunction(f, VT_Prepared, {});
    call(SceneNotice_callPrepareAPI, {fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6C88E0, "SceneDownload::finalize convert data freed (sub_6C88E0)", scene_download::prepareNotice);

uint32_t paradeGet(uint32_t, uint32_t okRef) {
    if (!ld8(okRef)) return 0;
    clearRetry();
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_ParadeGot, {});
    call(APIManager_executeRankingParadeGet, {api, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x6C8A40, "SceneDownload::finalize notice prepared (sub_6C8A40)", scene_download::paradeGet);

// the title menu's "download the data again": a confirmation dialog
uint32_t removeDataTouched(uint32_t, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t dlg = call(PopupDialog_create, {1, 0});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    Frame f;
    const uint32_t td = textData(f, 109100006);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t fo = newFunction(f, VT_RemoveDataOk, {});
    call(PopupDialog_open, {dlg, layer, s, fo});
    functionDestroy(fo);
    strFree(s);
    sharedRelease(td);
    call(AudioPlaySystemSEPopUp, {});
    return 0;
}
RW_FUNCTION(0x67FC1C, "SceneTitle download-again button operator() (sub_67FC1C)", scene_download::removeDataTouched);

// ... confirmed: master and resource data removed, the avatar board positions reset, the title again
uint32_t removeData(uint32_t, uint32_t okRef) {
    if (!ld8(okRef)) return 0;
    call(MasterDataManager_remove, {call(MasterDataManager_getInstance, {})});
    call(ResourceDataManager_remove, {call(ResourceDataManager_getInstance, {})});
    for (uint32_t k = 0; k < 4; k++) call(AppState_setAvatarBoardPos[k], {call(AppState_getInstance, {}), 0});
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, VT_ToTitle, {});
    call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x67FDD4, "SceneTitle download-again confirmed (sub_67FDD4)", scene_download::removeData);

// the download layer whose next layer is the title's own
uint32_t downloadLayer(uint32_t) {
    const uint32_t layer = call(SceneDownload_create, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_AfterDownload, {});
    call(FunctionLayer_assign, {layer + F_NEXT, fo});
    functionDestroy(fo);
    return layer;
}
RW_FUNCTION(0x67FF48, "SceneTitle download layer (sub_67FF48)", scene_download::downloadLayer);

}  // namespace scene_download
