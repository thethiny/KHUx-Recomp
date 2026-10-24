// SceneTutorialDownload's popups and callbacks: the download question (Opening_DL: "Download" starts the video and
// the DataDownloader), the Jewel popup after the download (Opening_Jewel: Collect sends tutorial progress 3, then
// the name input), and the video events (the video ends -> the popup may close it; skipped -> BGM).
#include "Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;   // (node, const string& name, text id)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t MapUintUint_at = 0x5681A5;
constexpr uint32_t function_vb_copy = 0x45C84D, function_vb_copy2 = 0x47BC69;   // std::function<void(bool)>(const&)
constexpr uint32_t function_vv_call = 0x470E69;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_setEnabledTouchEffect = 0x60CF65;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioStopGameMainBGM = 0x5C0F59;
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance, Director_getVisibleOrigin = rwlib::cocos2d::Director::getVisibleOrigin;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t Label_createWithTTF = rwlib::cocos2d::Label::createWithTTF__RKNS_10_ttfConfigERKNSt3__112basic_stringIcNS4_11char_traitsIcEENS4_9allocatorIcEEEENS_14TextHAlignmentEi, Label_setAlignment = rwlib::cocos2d::Label::setAlignment;
constexpr uint32_t DataDownloader_init = 0x776B25;
constexpr uint32_t GOT_NAME_REGISTER_FACTORY = 0xFD69A0;
constexpr uint32_t VT_DownloadTouch = 0xF6D7F0;            // $_6 {button, -, win, layer, shadow, root, answer}
constexpr uint32_t VT_DownloadClosed = 0xF6D81C;           // $_6's close end {layer, shadow, root, answer}
constexpr uint32_t VT_JewelTouch = 0xF6D740;               // $_8 {button, -, win, layer, shadow, root, answer}
constexpr uint32_t VT_JewelSent = 0xF6D76C;                // $_8's API callback (same captures)
constexpr uint32_t VT_JewelClosed = 0xF6D798;              // its close end {layer, shadow, root, answer}
constexpr uint32_t VT_ErrorClosed = 0xF6D7C4;              // {}
constexpr uint32_t VT_ToNameRegister = 0xF6D874;           // finalize $_9's lambda {}
constexpr uint32_t VT_Downloaded = 0xF6D8CC;               // init $_3's DataDownloader callback {scene, scene}
constexpr uint32_t S_Win = 0xE04096, S_TxtHeading = 0xE006CA, S_Wording1 = 0xDFF14C, S_Wording2 = 0xDF863B;
constexpr uint32_t S_Wording = 0xDF862F, S_DlJson = 0xE072A9, S_ButtonDL = 0xE072CC, S_TxtDL = 0xE072D6;
constexpr uint32_t S_JewelJson = 0xE072DD, S_ButtonGet = 0xE07303, S_TxtGet = 0xE0730E;
constexpr uint32_t S_Font = 0xDF9F4D, S_TutorialMovie = 0xE007CF;
constexpr uint32_t V_setPosition = 0x4C, V_setContentSize = 0xA8, V_setVisible = 0xB0, V_addChild = 0xF0;
constexpr uint32_t V_setTouchEnabled = 0x29C, V_videoSetFileName = 0x31C, V_videoPlay = 0x32C;
constexpr uint32_t V_videoStop = 0x330, V_videoSeekStart = 0x334;
constexpr uint32_t LABEL_V_ALIGNMENT = 0x3E0;
constexpr uint32_t API_MISC = 0x1B0, API_RETRY_DIALOG = 0x14;
constexpr uint32_t MISC_DOWNLOAD_JEWELS = 116;
constexpr uint32_t PHASE_DOWNLOADED = 3;
// SceneTutorialDownload
constexpr uint32_t F_VIDEO = 0x28C, F_DOWNLOADER = 0x290, F_LABEL = 0x2C8, F_POPUP_CLOSED = 0x2CD;
constexpr uint32_t F_VIDEO_STOPPABLE = 0x2D0;

uint32_t misc(uint32_t id) {
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, id);
    return ld32(call(MapUintUint_at, {ld32(call(fn::APIManager_getInstance, {}) + API_MISC), key}));
}
void titleLabelId(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

// Opening_DL / Opening_Jewel: shadow (fading in) on popup layer 2, the popup on the shadow, centred; "Win" with
// the window inside
struct Popup {
    uint32_t layer, shadow, root, win;
};
Popup openPopup(uint32_t json) {
    Popup p;
    call(fn::AudioPlaySystemSEPopUp, {});
    p.layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    p.shadow = addShadow(p.layer, true);
    p.root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
    centerOnMargins(p.root);
    vcall(p.shadow, V_addChild, {p.shadow, p.root});
    p.win = findNode(p.root, S_Win, 3);
    return p;
}

// the button's heap lambda {button, -, win, layer, shadow, root, answer} (the answer copied in again)
void touchWithClosure(uint32_t button, uint32_t vt, const Popup &p, uint32_t answerCopy) {
    Frame f;
    const uint32_t copy = f.alloc(20);
    functionCopy(copy, answerCopy);
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(vt, 0x38);
    if (obj) {
        st32(obj + 8, button);
        st32(obj + 0x10, p.win);
        st32(obj + 0x14, p.layer);
        st32(obj + 0x18, p.shadow);
        st32(obj + 0x1C, p.root);
        call(function_vb_copy2, {obj + 0x20, copy});
        st32(obj + 8, button);
    }
    st32(fo + 16, obj);
    call(fn::Widget_addTouchEventListener, {button, fo});
    functionDestroy(fo);
    functionDestroy(copy);
}

void openAnimation(uint32_t win) {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    const uint32_t open = call(SceneUtil_createOpenPopupAnimation, {win, none});
    call(Node_runAction, {win, call(EaseSineOut_create, {open})});
    functionDestroy(none);
}

// the popup closes: the window's close animation (then {layer, shadow, root, answer}), the shadow fades out
void closeWith(uint32_t self, uint32_t vt) {
    const uint32_t win = ld32(self + 0x10);
    const uint32_t layer = ld32(self + 0x14), shadow = ld32(self + 0x18), root = ld32(self + 0x1C);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, self + 0x20});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(vt, 0x30);
    if (obj) {
        st32(obj + 8, layer);
        st32(obj + 0xC, shadow);
        st32(obj + 0x10, root);
        call(function_vb_copy2, {obj + 0x18, copy});
    }
    st32(fo + 16, obj);
    call(Node_runAction, {win, call(SceneUtil_createClosePopupAnimation, {win, fo})});
    functionDestroy(fo);
    functionDestroy(copy);
    const uint32_t shade = ld32(self + 0x18);
    call(Node_runAction, {shade, call(FadeTo_create, {0x3DCCCCCD, 0})});
}
}  // namespace

namespace tutorial_download {

uint32_t openDownloadPopup(uint32_t, uint32_t answer) {
    const Popup p = openPopup(S_DlJson);
    Frame f;
    const uint32_t answerCopy = f.alloc(20);
    call(function_vb_copy, {answerCopy, answer});
    titleLabelId(p.win, S_TxtHeading, 0xB, 109160001);
    {
        const uint32_t text = findNode(p.root, S_Wording1, 0xC);
        SceneUtil::setShadow(text);
        Frame g;
        const uint32_t td = textData(g, 109160007);
        const uint32_t fmt = textDataStr(td);
        setTextC(text, stringBuffer(g, {fmt, misc(MISC_DOWNLOAD_JEWELS)}));
        sharedRelease(td);
    }
    titleLabelId(p.win, S_Wording2, 0xC, 109160003);
    const uint32_t button = findNode(p.root, S_ButtonDL, 9);
    touchWithClosure(button, VT_DownloadTouch, p, answerCopy);
    titleLabelId(button, S_TxtDL, 6, 109030004);
    openAnimation(p.win);
    functionDestroy(answerCopy);
    return 0;
}
RW_FUNCTION(0x73A484, "SceneTutorialDownload::openDownloadPopup", tutorial_download::openDownloadPopup);

uint32_t openJewelPopup(uint32_t, uint32_t answer) {
    const Popup p = openPopup(S_JewelJson);
    Frame f;
    const uint32_t answerCopy = f.alloc(20);
    call(function_vb_copy, {answerCopy, answer});
    titleLabelId(p.win, S_TxtHeading, 0xB, 109160004);
    {
        const uint32_t text = findNode(p.root, S_Wording, 0xB);
        SceneUtil::setShadow(text);
        Frame g;
        const uint32_t td = textData(g, 109160005);
        const uint32_t fmt = textDataStr(td);
        setTextC(text, stringBuffer(g, {fmt, misc(MISC_DOWNLOAD_JEWELS)}));
        sharedRelease(td);
    }
    const uint32_t button = findNode(p.root, S_ButtonGet, 0xA);
    touchWithClosure(button, VT_JewelTouch, p, answerCopy);
    titleLabelId(button, S_TxtGet, 7, 103500004);
    openAnimation(p.win);
    functionDestroy(answerCopy);
    return 0;
}
RW_FUNCTION(0x73ABBC, "SceneTutorialDownload::openJewelPopup", tutorial_download::openJewelPopup);

// $_6: Download - the button off, the popup closes (then the answer)
uint32_t downloadTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
    closeWith(self, VT_DownloadClosed);
    return 0;
}
RW_FUNCTION(0x73BFEC, "SceneTutorialDownload::openDownloadPopup::$_6 operator()", tutorial_download::downloadTouched);

uint32_t downloadClosedClone(uint32_t self) {
    const uint32_t c = heapObject(VT_DownloadClosed, 0x30);
    if (c) {
        st32(c + 8, ld32(self + 8));
        st32(c + 0xC, ld32(self + 0xC));
        st32(c + 0x10, ld32(self + 0x10));
        call(function_vb_copy, {c + 0x18, self + 0x18});
    }
    return c;
}
RW_FUNCTION(0x73C204, "SceneTutorialDownload::openDownloadPopup::$_6 close end __clone()",
            tutorial_download::downloadClosedClone);

// $_8: Collect - the button off, the reward claimed with the tutorial progress (phase 3)
uint32_t jewelTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
    call(SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t button = ld32(self + 8);
    const Popup p{ld32(self + 0x14), ld32(self + 0x18), ld32(self + 0x1C), ld32(self + 0x10)};
    Frame f;
    const uint32_t answerCopy = f.alloc(20);
    call(function_vb_copy, {answerCopy, self + 0x20});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t copy = f.alloc(20);
    functionCopy(copy, answerCopy);
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapObject(VT_JewelSent, 0x38);
    if (obj) {
        st32(obj + 8, button);
        st32(obj + 0x10, p.win);
        st32(obj + 0x14, p.layer);
        st32(obj + 0x18, p.shadow);
        st32(obj + 0x1C, p.root);
        call(function_vb_copy2, {obj + 0x20, copy});
        st32(obj + 8, button);
    }
    st32(fo + 16, obj);
    call(APIManager_executeTutorialProgress, {api, PHASE_DOWNLOADED, 0, 0, 0, 0, 0, 0, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    functionDestroy(answerCopy);
    return 0;
}
RW_FUNCTION(0x73B650, "SceneTutorialDownload::openJewelPopup::$_8 operator()", tutorial_download::jewelTouched);

// the reward claimed: the popup closes, or the error dialog and the button back on
uint32_t jewelSent(uint32_t self, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        closeWith(self, VT_JewelClosed);
        return 0;
    }
    if (ld8(call(fn::APIManager_getInstance, {}) + API_RETRY_DIALOG)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_ErrorClosed, {});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    if (!call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})}))
        vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 1});
    return 0;
}
RW_FUNCTION(0x73BA00, "SceneTutorialDownload::openJewelPopup::$_8 sent operator()", tutorial_download::jewelSent);

// finalize $_4: the Jewel popup answered - its inner lambda runs at once (to the name input)
uint32_t jewelAnswered(uint32_t) {
    Frame f;
    const uint32_t fo = newFunction(f, VT_ToNameRegister, {});
    call(function_vv_call, {fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x73C3C0, "SceneTutorialDownload::finalize::$_4 operator()", tutorial_download::jewelAnswered);

uint32_t toNameRegister(uint32_t) {
    smain::changeSceneLayer(ld32(GOT_NAME_REGISTER_FACTORY), 1);
    return 0;
}
RW_FUNCTION(0x73C4D8, "SceneTutorialDownload::finalize::$_9 lambda operator() (to the name)", tutorial_download::toNameRegister);

// init $_2: the video events - 0 (playing) stops the video when allowed; 3 (completed) marks the popup closed and
// returns to the main BGM
uint32_t videoEvent(uint32_t self, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type == 3) {
        st8(ld32(self + 4) + F_POPUP_CLOSED, 1);
        return call(AudioReplaceGameMainBGM, {501000910, 500});
    }
    if (type != 0) return self;
    if (!ld8(ld32(self + 8) + F_VIDEO_STOPPABLE)) return self;
    const uint32_t slot = ld32(self + 0xC) + F_VIDEO;
    const uint32_t video = ld32(slot);
    if (!video) return 0;
    vcall(video, V_setTouchEnabled, {video, 0});
    vcall(ld32(slot), V_setVisible, {ld32(slot), 0});
    return vcall(ld32(slot), V_videoStop, {ld32(slot)});
}
RW_FUNCTION(0x73CAF0, "SceneTutorialDownload::init::$_2 operator() (video event)", tutorial_download::videoEvent);

// init $_3: the download answer - the touch effect off, the BGM out, the progress label (bottom right), the
// tutorial movie, then the DataDownloader
uint32_t downloadAnswered(uint32_t self, uint32_t okRef) {
    if (!ld8(okRef)) return 0;
    const uint32_t scene = ld32(self + 4);
    call(SM_setEnabledTouchEffect, {sceneManager(), 0});
    call(AudioStopGameMainBGM, {50});
    Frame f;
    const uint32_t origin = f.alloc(8), visible = f.alloc(8);
    call(Director_getVisibleOrigin, {origin, call(Director_getInstance, {})});
    call(Director_getVisibleSize, {visible, call(Director_getInstance, {})});
    // TTFConfig {font file, size 27, glyphs 0, custom glyphs 0, distance field off, outline 0, ...}
    const uint32_t config = f.alloc(0x24);
    strInit(config, S_Font, 0x22);
    st32(config + 0xC, 27);
    st32(config + 0x10, 0);
    st32(config + 0x14, 0);
    st8(config + 0x18, 0);
    st32(config + 0x1C, 0);
    st8(config + 0x20, 0);
    st8(config + 0x21, 0);
    const uint32_t text = f.alloc(12);
    st8(text, 0);
    st8(text + 1, 0);
    const uint32_t label = call(Label_createWithTTF, {config, text, 0, 0});
    st32(scene + F_LABEL, label);
    strFree(text);
    call(Label_setAlignment, {ld32(scene + F_LABEL), 2, ld32(ld32(scene + F_LABEL) + LABEL_V_ALIGNMENT)});
    {
        const float x = float(double(ldf(origin)) + double(ldf(visible)) * 0.7);
        const float y = ldf(origin + 4) + 20.0f;
        const uint32_t l = ld32(scene + F_LABEL);
        Frame g;
        const uint32_t pos = g.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits(y)});
        vcall(l, V_setPosition, {l, pos});
        call(Vec2_dtor, {pos});
    }
    {
        const uint32_t l = ld32(scene + F_LABEL);
        Frame g;
        const uint32_t size = g.alloc(8);
        call(Size_ctor, {size, ld32(visible), fbits(27.0f)});
        vcall(l, V_setContentSize, {l, size});
    }
    vcall(scene, V_addChild, {scene, ld32(scene + F_LABEL)});
    const uint32_t other = ld32(self + 8);
    if (const uint32_t video = ld32(other + F_VIDEO)) {
        vcall(video, V_setVisible, {video, 1});
        vcall(ld32(other + F_VIDEO), V_setTouchEnabled, {ld32(other + F_VIDEO), 1});
        vcall(ld32(other + F_VIDEO), V_videoSeekStart, {ld32(other + F_VIDEO)});
    }
    {
        const uint32_t video = ld32(scene + F_VIDEO);
        Frame g;
        const uint32_t file = newStr(g, S_TutorialMovie, 0x12);
        vcall(video, V_videoSetFileName, {video, file});
        strFree(file);
    }
    vcall(ld32(scene + F_VIDEO), V_videoPlay, {ld32(scene + F_VIDEO)});
    {
        Frame g;
        const uint32_t fo = newFunction(g, VT_Downloaded, {scene, ld32(self + 8)});
        call(DataDownloader_init, {scene + F_DOWNLOADER, fo});
        functionDestroy(fo);
    }
    strFree(config);
    call(Vec2_dtor, {origin});
    return 0;
}
RW_FUNCTION(0x73C628, "SceneTutorialDownload::init::$_3 operator() (download answer)", tutorial_download::downloadAnswered);

}  // namespace tutorial_download
