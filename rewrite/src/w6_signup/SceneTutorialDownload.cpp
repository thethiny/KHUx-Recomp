// SceneTutorialDownload: the first resource download of a new account. A video plays while the DataDownloader
// fetches the resources (the label shows the percentage, then "n / total" with the percentage per file); when done
// the resources are mounted and the Jewel popup (300 Jewels for downloading) leads on.
#include "../scene/SceneUi.hpp"

#include <cstring>

using namespace scene;

namespace {
constexpr uint32_t Layer_ctor = 0x88A7D1, Scene_create = 0x8A5F37, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_create = 0x89372D;
constexpr uint32_t function_vv_assign = 0x4EB9CD;
constexpr uint32_t Director_getInstance = 0x8EE519, Director_getVisibleSize = 0x8F0675;
constexpr uint32_t Size_ctor = 0x8DF027, Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t VideoPlayer_create = 0x5A358D;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t SM_setEnabledTouchEffect = 0x60CF65;
constexpr uint32_t ResourceDataManager_getInstance = 0x7728AD, ResourceDataManager_mount = 0x773355;
constexpr uint32_t DataDownloader_ctor = 0x776927, DataDownloader_update = 0x776C99;
constexpr uint32_t DataDownloader_getNumberOfDownloadedResources = 0x77719D;
constexpr uint32_t SceneTutorialDownload_ctor = 0x67F86D, SceneTutorialDownload_create = 0x67F5C5;
constexpr uint32_t SceneTutorialDownload_openDownloadPopup = 0x73A485;   // (garbage this, function<void(bool)>)
constexpr uint32_t SceneTutorialDownload_openJewelPopup = 0x73ABBD;      // (garbage this, function<void(bool)>)
constexpr uint32_t SceneTutorialDownload_finalize = 0x73AAB5;
constexpr uint32_t GOT_VT_SceneTutorialDownload = 0xFD690C;
constexpr uint32_t VT_OnEnter = 0xF6D950, VT_OnExit = 0xF6D924;   // {this}
constexpr uint32_t VT_VideoEvent = 0xF6D8F8;               // {this, this, this}
constexpr uint32_t VT_DownloadAnswer = 0xF6D8A0;           // {this, this}
constexpr uint32_t VT_JewelClosed = 0xF6D848;              // {this}
constexpr uint32_t V_release = 0x04, V_setPosition = 0x4C, V_setPositionX = 0x64, V_setContentSize = 0xA8;
constexpr uint32_t V_setVisible = 0xB0, V_addChild = 0xF0, V_removeFromParent = 0x124, V_init = 0x248;
constexpr uint32_t V_textSetString = 0x2A8, V_videoAddEventListener = 0x358;
constexpr uint32_t SIZE = 0x2D8;
constexpr uint32_t EPILOGUE_R0 = 0x86C;
// SceneTutorialDownload
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_NODE = 0x288, F_VIDEO = 0x28C;
constexpr uint32_t F_DOWNLOADER = 0x290;                   // DataDownloader: +0 state, +0x10 total, +0x14 done,
constexpr uint32_t F_STATE = 0x290, F_TOTAL = 0x2A0, F_DONE = 0x2A4, F_FILES = 0x2A8;   // +0x18 files
constexpr uint32_t F_LABEL = 0x2C8, F_FINALIZED = 0x2CC, F_POPUP_CLOSED = 0x2CD, F_COMPLETE = 0x2CE, F_2D0 = 0x2D0;

void assign(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}

// done * 100 / total as the double printf gets (0 without a total)
uint64_t percent(uint32_t done, uint32_t total) {
    if (!total) return 0;
    const float pct = (float(done) * 100.0f) / float(total);
    uint64_t bits;
    const double d = double(pct);
    std::memcpy(&bits, &d, 8);
    return bits;
}

void setLabel(uint32_t label, uint32_t cstr) {
    Frame f;
    const uint32_t s = newStr(f, cstr);
    vcall(label, V_textSetString, {label, s});
    strFree(s);
}
}  // namespace

namespace scene_tutorial_download {

uint32_t construct(uint32_t self) {
    call(Layer_ctor, {self});
    st32(self, ld32(GOT_VT_SceneTutorialDownload) + 8);
    st32(self + F_NODE, 0);
    st32(self + F_VIDEO, 0);
    call(DataDownloader_ctor, {self + F_DOWNLOADER});
    st8(self + F_2D0, 0);
    st32(self + F_FINALIZED, 0);
    st32(self + F_LABEL, 0);
    return self;
}
RW_FUNCTION(0x67F86C, "SceneTutorialDownload::SceneTutorialDownload", scene_tutorial_download::construct);

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(SceneTutorialDownload_ctor, {self});
    if (vcall(self, V_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x67F5C4, "SceneTutorialDownload::create", scene_tutorial_download::create);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneTutorialDownload_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x73A1A8, "SceneTutorialDownload::createScene", scene_tutorial_download::createScene);

// the video (960x540, centred) and the download question
uint32_t init(uint32_t self) {
    assign(self + F_ON_ENTER, VT_OnEnter, self);
    assign(self + F_ON_EXIT, VT_OnExit, self);
    const uint32_t node = call(Node_create, {0x51});
    st32(self + F_NODE, node);
    vcall(node, V_setPositionX, {node, call(SceneUtil_getSideMargin, {})});
    vcall(self, V_addChild, {self, ld32(self + F_NODE)});
    st32(self + F_VIDEO, call(VideoPlayer_create, {0xA3}));
    Frame f;
    const uint32_t visible = f.alloc(8), size = f.alloc(8);
    call(Director_getVisibleSize, {visible, call(Director_getInstance, {})});
    call(Size_ctor, {size, fbits(960.0f), fbits(540.0f)});
    {
        const uint32_t video = ld32(self + F_VIDEO);
        Frame g;
        const uint32_t pos = g.alloc(8);
        call(Vec2_ctor, {pos, fbits(ldf(visible) * 0.5f), fbits(ldf(visible + 4) * 0.5f)});
        vcall(video, V_setPosition, {video, pos});
        call(Vec2_dtor, {pos});
    }
    vcall(ld32(self + F_VIDEO), V_setContentSize, {ld32(self + F_VIDEO), size});
    {
        const uint32_t video = ld32(self + F_VIDEO);
        Frame g;
        const uint32_t fo = newFunction(g, VT_VideoEvent, {self, self, self});
        vcall(video, V_videoAddEventListener, {video, fo});
        functionDestroy(fo);
    }
    vcall(self, V_addChild, {self, ld32(self + F_VIDEO)});
    {
        Frame g;
        const uint32_t fo = newFunction(g, VT_DownloadAnswer, {self, self});
        call(SceneTutorialDownload_openDownloadPopup, {fo + 4, fo});
        functionDestroy(fo);
    }
    return 1;
}
RW_FUNCTION(0x73A238, "SceneTutorialDownload::init", scene_tutorial_download::init);

// once: touch effects back; when the download ended (state 0 or 5) the video goes, the label hides, the resources
// are mounted and the Jewel popup opens
uint32_t finalize(uint32_t self) {
    if (ld8(self + F_FINALIZED)) return 0;
    st8(self + F_FINALIZED, 1);
    call(SM_setEnabledTouchEffect, {sceneManager(), 1});
    const uint32_t state = ld32(call(fn::APIManager_getInstance, {}));
    if (state != 0 && state != 5) return 0;
    if (const uint32_t video = ld32(self + F_VIDEO)) {
        vcall(video, V_removeFromParent, {video});
        st32(self + F_VIDEO, 0);
    }
    if (const uint32_t label = ld32(self + F_LABEL)) vcall(label, V_setVisible, {label, 0});
    call(ResourceDataManager_mount, {call(ResourceDataManager_getInstance, {})});
    Frame f;
    const uint32_t fo = newFunction(f, VT_JewelClosed, {self});
    call(SceneTutorialDownload_openJewelPopup, {VT_JewelClosed, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x73AAB4, "SceneTutorialDownload::finalize", scene_tutorial_download::finalize);

// the download state in the label: 1 = the list (percent), 2 / 3 = the files ("n / total", percent), 4 = done
uint32_t update(uint32_t self, uint32_t dt) {
    call(DataDownloader_update, {self + F_DOWNLOADER, dt});
    const uint32_t state = ld32(self + F_STATE);
    uint32_t extra = 2;
    switch (state) {
    case 1:
        if (ld32(self + F_LABEL)) {
            const uint32_t label = ld32(self + F_LABEL);
            const uint32_t done = ld32(self + F_DONE), total = ld32(self + F_TOTAL);
            Frame f;
            const uint32_t td = textData(f, 109130026);
            const uint32_t fmt = textDataStr(td);
            const uint64_t pct = percent(done, total);
            const uint32_t buf = stringBuffer(f, {fmt, uint32_t(pct), uint32_t(pct >> 32)});
            sharedRelease(td);
            setLabel(label, buf);
        }
        break;
    case 2:
        extra = 1;
        [[fallthrough]];
    case 3:
        if (ld32(self + F_LABEL)) {
            const uint32_t label = ld32(self + F_LABEL);
            const uint32_t done = ld32(self + F_DONE), total = ld32(self + F_TOTAL);
            Frame f;
            const uint32_t td = textData(f, 109130022);
            const uint32_t fmt = textDataStr(td);
            const uint32_t n = 2 * call(DataDownloader_getNumberOfDownloadedResources, {self + F_DOWNLOADER}) + extra;
            const uint32_t files = 2 * ld32(self + F_FILES);
            const uint64_t pct = percent(done, total);
            const uint32_t buf = stringBuffer(f, {fmt, n, files, uint32_t(pct), uint32_t(pct >> 32)});
            sharedRelease(td);
            setLabel(label, buf);
        }
        break;
    case 4:
        if (const uint32_t label = ld32(self + F_LABEL)) {
            Frame f;
            const uint32_t td = textData(f, 109130025);
            setLabel(label, textDataStr(td));
            sharedRelease(td);
        }
        st8(self + F_COMPLETE, 1);
        if (ld8(self + F_POPUP_CLOSED)) call(SceneTutorialDownload_finalize, {self});
        break;
    default:
        break;
    }
    return EPILOGUE_R0;
}
RW_FUNCTION(0x73B190, "SceneTutorialDownload::update", scene_tutorial_download::update);

}  // namespace scene_tutorial_download
