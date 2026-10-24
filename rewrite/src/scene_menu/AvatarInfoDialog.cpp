// AvatarInfoDialog: construction / destruction, open (the profile request) and close. createLayout (the page,
// 7 KB) and the sub-popups (name, message, play style, party actions) are not rewritten yet.
#include "AvatarInfoDialog.hpp"

using namespace scene;

namespace {
constexpr uint32_t VT_AvatarInfoDialog = 0xF86938;        // _ZTV16AvatarInfoDialog + 8
constexpr uint32_t VT_EditBoxDelegate = 0xF86BC0;         // _ZTV16AvatarInfoDialog + 0x290 (the +0x270 base)

// ---- callees
constexpr uint32_t Memset = 0x40E7A0;                     // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base, Node_dtor = rwlib::cocos2d::Node::dtor_base, Node_init = rwlib::cocos2d::Node::init;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t function_vb_assign = 0x490109;         // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t function_bv_assign = 0x53327D;         // std::function<bool()>::operator=(const function&)
constexpr uint32_t function_vb_call = 0x474139;           // std::function<void(bool)>::operator()(bool)
constexpr uint32_t AudioManager_getInstance = 0x5BECFD;
constexpr uint32_t AudioManager_load = 0x5BEEF5;          // (mgr, ResourceID)
constexpr uint32_t ImageDownloader_ctor = 0x5BCF7D, ImageDownloader_dtor = 0x5BCFA9;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneUtil_getLinkPlatformType = 0x47AAAD;
constexpr uint32_t APIManager_executeUserProfileGet = 0x4C7F65;   // (api, u64 userId, PlatformType, const function&)
constexpr uint32_t VectorSkill_destroy = 0x46BE37;        // __vector_base<UserSkill::Skill>::~__vector_base
constexpr uint32_t VectorU64_destroy = 0x468499;
constexpr uint32_t VectorTimezone_destroy = 0x5329D5;     // __vector_base<APIManager::Timezone>::~__vector_base
constexpr uint32_t TreeTimezone_destroy = 0x54CD73;       // __tree<APIManager::Timezone>::destroy(node)

// ---- data
constexpr uint32_t APIManager_user = 0x74;                // shared_ptr<User>: the player
constexpr uint32_t APIManager_userSetting = 0x84;         // shared_ptr: the player's settings (byte 0x11)
constexpr uint32_t User_id = 0;                           // u64 SafeBuffer index
constexpr uint32_t UserSetting_flag = 0x11;
constexpr uint32_t IMAGE_CACHE_SECONDS = 3600;            // ImageDownloader +0x18
constexpr uint32_t SE_DIALOG = 0x1E56B9EA;                // the sound loaded for the dialog
constexpr uint32_t SHADOW_Z = 100;
constexpr float SHADOW_FADE = 0.1f;

// virtual slots
constexpr uint32_t VSLOT_release = 0x04;                  // Ref: the deleting destructor
constexpr uint32_t VSLOT_init = 0x248;
constexpr uint32_t VSLOT_setTouchEnabled = 0x29C;         // ui::Widget

// lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_ProfileLoaded = 0xF58938;           // open::$_ [dialog, flag]: /user/profile answered
constexpr uint32_t VT_CloseDone = 0xF583E4;               // close::$_12 [dialog]

constexpr uint32_t S_EMPTY = 0xDF8A1A;                    // ""
constexpr uint32_t JSON_AvatarInfoScene = 0xDFE1DE;       // "cocostudio/publish/AvatarInfoScene_A_ver120.json"
struct Lit { uint32_t addr, len; };
constexpr Lit S_UnderUI{0xDFE20F, 7}, S_Button_A{0xDFE217, 8}, S_Button_B{0xDFE220, 8}, S_Button_C{0xDFE229, 8};
constexpr Lit S_Button_D{0xDFE232, 8};

inline uint32_t find(uint32_t root, Lit name) { return findNode(root, name.addr, name.len); }
inline void stringFree(uint32_t s) {
    if (ld8(s) & 1) call(fn::operator_delete, {ld32(s + 8)});
}
// the root layout, added to the shadow and hidden until the profile arrives
void loadLayout(AvatarInfoDialog *self) {
    Frame f;
    const uint32_t name = newStr(f, JSON_AvatarInfoScene, 0x30);
    self->root = SceneUtil::createNodeWithSceneFile(name);
    strFree(name);
    vcall(self->shadow, node::addChild, {self->shadow, self->root});
    vcall(self->root, node::setVisible, {self->root, 0});
}
}  // namespace

AvatarInfoDialog::Addr AvatarInfoDialog::create() {
    const uint32_t addr = call(fn::operator_new, {SIZE});
    call(Memset, {addr, SIZE, 0});
    construct(rw::host_ptr<AvatarInfoDialog>(addr));
    if (vcall(addr, VSLOT_init, {addr}) == 1) {
        call(Ref_autorelease, {addr});
        return addr;
    }
    vcall(addr, VSLOT_release, {addr});
    return 0;
}

AvatarInfoDialog *AvatarInfoDialog::construct(AvatarInfoDialog *self) {
    const uint32_t a = rw::guest_addr(self);
    call(Node_ctor, {a});
    self->vtable = VT_AvatarInfoDialog;
    self->vtable2 = VT_EditBoxDelegate;
    for (uint32_t off : {0x274u, 0x278u, 0x27Cu, 0x290u, 0x2B8u}) st32(a + off, 0);
    st8(a + 0x2A4, 0);
    for (uint32_t off : {0x2A0u, 0x29Cu, 0x298u, 0x2C8u, 0x2C4u, 0x2C0u}) st32(a + off, 0);
    strInit(a + 0x2CC, S_EMPTY, 0);
    strInit(a + 0x2D8, S_EMPTY, 0);
    for (uint32_t off : {0x2E4u, 0x2E8u, 0x2ECu, 0x2F8u, 0x2FCu}) st32(a + off, 0);
    st32(a + 0x2F4, a + 0x2F8);           // the empty std::map: begin = its end node
    for (uint32_t off : {0x3C0u, 0x3BCu, 0x3B8u, 0x3D4u, 0x3D0u, 0x3CCu, 0x3E8u, 0x3ECu, 0x3F0u}) st32(a + off, 0);
    call(Memset, {a + 0x300, 0x30, 0});
    st8(a + 0x3FB, ld8(a + 0x3FB) & 0xFD);
    st8(a + 0x403, ld8(a + 0x403) & 0xFD);
    for (uint32_t off : {0x410u, 0x40Cu, 0x408u, 0x468u}) st32(a + off, 0);
    call(Memset, {a + 0x418, 0x28, 0});
    for (uint32_t off : {0x450u, 0x44Cu, 0x448u, 0x444u}) st32(a + off, 0);
    strInit(a + 0x470, S_EMPTY, 0);
    for (uint32_t off : {0x490u, 0x4A8u, 0x4C4u}) st32(a + off, 0);
    call(Memset, {a + 0x4B0, 0x11, 0});
    return self;
}

uint32_t AvatarInfoDialog::init(AvatarInfoDialog *self) {
    if (call(Node_init, {rw::guest_addr(self)}) != 1) return 0;
    call(AudioManager_load, {call(AudioManager_getInstance, {}), SE_DIALOG});
    const uint32_t downloader = call(fn::operator_new, {0x34});
    call(ImageDownloader_ctor, {downloader});
    self->imageDownloader = downloader;
    st32(downloader + 0x18, IMAGE_CACHE_SECONDS);
    return 1;
}

AvatarInfoDialog *AvatarInfoDialog::destruct(AvatarInfoDialog *self) {
    const uint32_t a = rw::guest_addr(self);
    self->vtable = VT_AvatarInfoDialog;
    self->vtable2 = VT_EditBoxDelegate;
    for (uint32_t off : {0x4B0u, 0x4B4u}) {
        if (ld32(a + off)) {
            call(Ref_release, {ld32(a + off)});
            st32(a + off, 0);
        }
    }
    if (self->imageDownloader) {
        const uint32_t downloader = self->imageDownloader;
        call(fn::operator_delete, {call(ImageDownloader_dtor, {downloader})});
    }
    self->imageDownloader = 0;
    functionDestroy(a + 0x498);
    functionDestroy(a + 0x480);
    stringFree(a + 0x470);
    functionDestroy(a + 0x458);
    call(VectorTimezone_destroy, {a + 0x444});
    stringFree(a + 0x424);
    stringFree(a + 0x418);
    stringFree(a + 0x408);
    call(VectorU64_destroy, {a + 0x3E8});
    stringFree(a + 0x3CC);
    stringFree(a + 0x3B8);
    for (uint32_t ctl = 0x32C; ctl >= 0x304; ctl -= 8) sharedRelease(a + ctl - 4);
    call(TreeTimezone_destroy, {a + 0x2F4, ld32(a + 0x2F8)});
    stringFree(a + 0x2D8);
    stringFree(a + 0x2CC);
    functionDestroy(a + 0x2A8);
    call(VectorSkill_destroy, {a + 0x298});
    functionDestroy(a + 0x280);
    call(Node_dtor, {a});
    return self;
}

// Opens over `parent` for the player `userId` (0: the viewer). Returns false when already open.
uint32_t AvatarInfoDialog::open(AvatarInfoDialog *self, Addr parent, uint32_t idLo, uint32_t idHi, Addr onClose,
                                uint32_t flag, Addr canClose) {
    if (self->parent) return 0;
    const uint32_t a = rw::guest_addr(self);
    self->parent = parent;
    call(function_vb_assign, {a + 0x280, onClose});
    call(function_bv_assign, {a + 0x2A8, canClose});
    functionDestroy(a + 0x458);
    st32(a + 0x468, 0);

    self->shadow = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(self->shadow, node::getOpacity, {self->shadow});
    vcall(self->shadow, node::setOpacity, {self->shadow, 0});
    const uint32_t shadow = self->shadow;
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(SHADOW_FADE), opacity})});
    vcall(self->shadow, node::addChild, {self->shadow, a});
    vcall(self->parent, node::addChildZ, {self->parent, self->shadow, SHADOW_Z});

    // another player: remember whom (the viewer's own id: not another player)
    if (idLo | idHi) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t me = ld32(api + APIManager_user);
        const uint32_t meCtl = ld32(api + APIManager_user + 4);
        if (meCtl) call(fn::shared_add, {meCtl});
        const uint32_t api2 = call(fn::APIManager_getInstance, {});
        const uint32_t settingCtl = ld32(api2 + APIManager_userSetting + 4);
        const uint32_t setting = ld32(api2 + APIManager_userSetting);
        if (settingCtl) call(fn::shared_add, {settingCtl});
        const uint32_t other = (uint32_t(safe32(me, User_id)) ^ idLo) | (uint32_t(safe32(me, User_id + 4)) ^ idHi);
        self->isOtherUser = other ? 1 : 0;
        if (other) {
            self->userIdHi = idHi;
            self->userIdLo = idLo;
        }
        if (setting) {
            Frame f;
            self->viewerFlag = ld8(safeBytes(f, setting, UserSetting_flag, 1));
        }
        if (settingCtl) call(fn::shared_release, {settingCtl});
        if (meCtl) call(fn::shared_release, {meCtl});
    }

    uint32_t requestLo, requestHi;
    if (self->isOtherUser) {
        requestLo = self->userIdLo;
        requestHi = self->userIdHi;
        loadLayout(self);
    } else {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t me = ld32(api + APIManager_user);
        const uint32_t meCtl = ld32(api + APIManager_user + 4);
        if (meCtl) call(fn::shared_add, {meCtl});
        requestLo = uint32_t(safe32(me, User_id));
        requestHi = uint32_t(safe32(me, User_id + 4));
        loadLayout(self);
        if (meCtl) call(fn::shared_release, {meCtl});
    }

    call(SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t platform = call(SceneUtil_getLinkPlatformType, {});
    Frame f;
    const uint32_t loaded = newFunction(f, VT_ProfileLoaded, {a, flag});
    call(APIManager_executeUserProfileGet, {api, loaded, requestLo, requestHi, platform, loaded});
    functionDestroy(loaded);
    return 1;
}

// tells the open sub-popup's callback it was cancelled, then animates out; $_12 removes the dialog
void AvatarInfoDialog::close(AvatarInfoDialog *self) {
    const uint32_t a = rw::guest_addr(self);
    if (ld32(a + 0x468)) {
        call(function_vb_call, {a + 0x458, 0});
        functionDestroy(a + 0x458);
        st32(a + 0x468, 0);
    }
    if (const uint32_t root = self->root) {
        Frame f;
        const uint32_t done = newFunction(f, VT_CloseDone, {a});
        call(fn::Node_runAction, {root, SceneUtil::createClosePopupAnimation(root, done)});
        functionDestroy(done);
    }
    if (const uint32_t shadow = self->shadow)
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(SHADOW_FADE), 0})});
}

// the four buttons at the bottom (titles, name / message, play style, outfits)
void AvatarInfoDialog::setTouchEnabledUnderUIButtons(AvatarInfoDialog *self, uint32_t enabled) {
    const uint32_t under = find(self->root, S_UnderUI);
    for (Lit name : {S_Button_A, S_Button_B, S_Button_C, S_Button_D}) {
        const uint32_t button = find(under, name);
        vcall(button, VSLOT_setTouchEnabled, {button, enabled});
    }
}

RW_FUNCTION(0x532A30, "AvatarInfoDialog::create", AvatarInfoDialog::create);
RW_FUNCTION(0x532A9C, "AvatarInfoDialog::AvatarInfoDialog", AvatarInfoDialog::construct);
RW_FUNCTION(0x532E3C, "AvatarInfoDialog::init", AvatarInfoDialog::init);
RW_FUNCTION(0x53259C, "AvatarInfoDialog::~AvatarInfoDialog", AvatarInfoDialog::destruct);
RW_FUNCTION(0x532E94, "AvatarInfoDialog::open", AvatarInfoDialog::open);
RW_FUNCTION(0x534F50, "AvatarInfoDialog::close", AvatarInfoDialog::close);
RW_FUNCTION(0x53C6CC, "AvatarInfoDialog::setTouchEnabledUnderUIButtons", AvatarInfoDialog::setTouchEnabledUnderUIButtons);
