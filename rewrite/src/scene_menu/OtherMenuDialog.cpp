// OtherMenuDialog: create, open (build the button page) and close. The buttons' touch lambdas, the popups they
// open (legal, serial codes, user code, Facebook, movie) and the edit box callbacks are not rewritten yet.
#include "OtherMenuDialog.hpp"

using namespace scene;

namespace {
constexpr uint32_t VT_OtherMenuDialog = 0xF8D198;         // _ZTV15OtherMenuDialog + 8
constexpr uint32_t VT_EditBoxDelegate = 0xF8D420;         // _ZTV15OtherMenuDialog + 0x290 (the +0x270 base)

// ---- callees
constexpr uint32_t Memset = 0x40E7A0;                     // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Size_copy = 0x8DF02D;                  // cocos2d::Size::Size(const Size&)
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t EaseOut_create = 0x8606A5;             // (action, float rate)
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t OtherMenuDialog_updateFacebookButton = 0x59F819;

// ---- data
constexpr uint32_t SceneUtil_IN_OUT_TIME = 0xDEC3D0;      // float: the shadow's fade-out time
constexpr uint32_t APIManager_ret = 0x44;                 // Ret*: the last response's header
constexpr uint32_t Ret_functionFlags = 0x34;              // which optional features the server enables
// functionFlags bits (the low byte, then the second byte)
constexpr uint8_t FEATURE_SERIAL_CODE = 0x02, FEATURE_USER_CODE = 0x08, FEATURE_TRANSFER = 0x10;
constexpr uint8_t FEATURE_SUPPORT = 0x20;
constexpr uint8_t FEATURE2_FACEBOOK = 0x02;               // functionFlags bit 9

constexpr uint32_t VSLOT_setTouchEnabled = 0x29C;         // ui::Widget
constexpr float SHADOW_FADE_IN = 0.1f;
constexpr float OPEN_EASE_RATE = 8.0f;

// ---- the buttons' touch lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_Back = 0xF5C37C;                    // $_0  []
constexpr uint32_t VT_Notice = 0xF5C2F8;                  // $_1  []
constexpr uint32_t VT_PlayerInfo = 0xF5C2A0;              // $_3  []
constexpr uint32_t VT_Album = 0xF5C274;                   // $_4  [bool = false]
constexpr uint32_t VT_Party = 0xF5C248;                   // $_5  [0, button]
constexpr uint32_t VT_UnionTransfer = 0xF5C1F0;           // $_6  [bool = false]
constexpr uint32_t VT_SerialCode = 0xF5C1C4;              // $_7  [dialog]
constexpr uint32_t VT_Support = 0xF5C16C;                 // $_8  []
constexpr uint32_t VT_Legal = 0xF5C140;                   // $_9  [dialog]
constexpr uint32_t VT_Transfer = 0xF5C0E8;                // $_10 []
constexpr uint32_t VT_Help = 0xF5C0BC;                    // $_11 []
constexpr uint32_t VT_Option = 0xF5C090;                  // $_12 []
constexpr uint32_t VT_Credit = 0xF5C064;                  // $_13 []
constexpr uint32_t VT_UserCode = 0xF5C038;                // $_14 [dialog]
constexpr uint32_t VT_Facebook = 0xF5BF88;                // $_15 [button, (unset), dialog]
constexpr uint32_t VT_CanClose = 0xF5BF5C;                // $_16 [back button]: SceneManager asks to close
constexpr uint32_t VT_Closed = 0xF5BF30;                  // $_17 [dialog]
constexpr uint32_t VT_CloseDone = 0xF5BC9C;               // close::$_22 [dialog]: the close animation ended

// ---- text table ids
constexpr int32_t TEXT_NOTICES = 101000002, TEXT_PLAYER_INFO = 101000003, TEXT_ALBUM = 101000005;
constexpr int32_t TEXT_PARTY = 101000004, TEXT_UNION_TRANSFER = 101000016, TEXT_SERIAL_CODES = 101000009;
constexpr int32_t TEXT_SUPPORT = 101000012, TEXT_LEGAL = 101000013, TEXT_TRANSFER = 101000011;
constexpr int32_t TEXT_HELP = 101000007, TEXT_SETTINGS = 101000008, TEXT_CREDITS = 101000014;
constexpr int32_t TEXT_USER_CODE = 104700001;

// ---- strings (address, length)
struct Lit { uint32_t addr, len; };
constexpr uint32_t JSON_MenuDialog = 0xE004C0;            // "cocostudio/publish/MenuDialog_ver120.json"
constexpr Lit S_BackButton{0xDF9E09, 10}, S_NoticeButton{0xE004EA, 12}, S_PlayerInfoButton{0xE004F7, 16};
constexpr Lit S_AlbumButton{0xE00508, 11}, S_ItemButton{0xE00514, 10}, S_UnionTransfers{0xE0051F, 14};
constexpr Lit S_SerialCodeButton{0xE0052E, 16}, S_SupportButton{0xE0053F, 13}, S_LicenseButton{0xE0054D, 13};
constexpr Lit S_TransferButton{0xE0055B, 14}, S_HelpButton{0xE0056A, 10}, S_OptionButton{0xE00575, 12};
constexpr Lit S_CreditButton{0xE00582, 12}, S_UserButton{0xE0058F, 10}, S_Movie{0xE0059A, 5};
constexpr Lit S_FaceBookButton{0xE005A0, 14};
constexpr Lit S_Txt{0xDF94D1, 3};

inline uint32_t find(uint32_t root, Lit name) { return findNode(root, name.addr, name.len); }
inline void setVisible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }

// the server's optional features (Ret::functionFlags); no response yet: none
bool featureOn(uint32_t byte, uint8_t bit) {
    const uint32_t ret = ld32(call(fn::APIManager_getInstance, {}) + APIManager_ret);
    return ret && (ld8(ret + Ret_functionFlags + byte) & bit);
}

// a button with a capture-less touch lambda and its label
uint32_t setUpButton(uint32_t root, Lit name, uint32_t vtable, int32_t textId) {
    const uint32_t button = find(root, name);
    addTouch(button, vtable, {});
    OtherMenuDialog::setButtonLabel(button, textId);
    return button;
}

// a touch lambda whose only capture is a bool (false)
void addTouchFlag(uint32_t button, uint32_t vtable) {
    Frame f;
    const uint32_t cb = newFunction(f, vtable, {});
    st8(cb + 4, 0);
    call(fn::Widget_addTouchEventListener, {button, cb});
    functionDestroy(cb);
}
}  // namespace

OtherMenuDialog::Addr OtherMenuDialog::create() {
    const uint32_t addr = call(fn::operator_new, {SIZE});
    call(Memset, {addr, SIZE, 0});
    call(Node_ctor, {addr});
    auto *self = rw::host_ptr<OtherMenuDialog>(addr);
    self->vtable = VT_OtherMenuDialog;
    self->vtable2 = VT_EditBoxDelegate;
    call(Memset, {addr + 0x280, 0x1D, 0});
    self->parent = 0;
    call(Ref_autorelease, {addr});
    return addr;
}

void OtherMenuDialog::setButtonLabel(Addr button, int32_t textId) {
    const uint32_t text = find(button, S_Txt);
    SceneUtil::setShadow(text);
    setTextId(text, textId);
}

void OtherMenuDialog::open(OtherMenuDialog *self, Addr parent) {
    const uint32_t addr = rw::guest_addr(self);
    self->parent = parent;
    call(fn::AudioPlaySystemSEPopUp, {});

    // the shadow fades in from transparent to its own opacity
    self->shadow = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(self->shadow, node::getOpacity, {self->shadow});
    vcall(self->shadow, node::setOpacity, {self->shadow, 0});
    call(fn::Node_runAction, {self->shadow, call(fn::FadeTo_create, {fbits(SHADOW_FADE_IN), opacity})});

    // the layout, centred on the shadow, drops in from the top
    self->root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), JSON_MenuDialog});
    {
        Frame f;
        const uint32_t shadowSize = f.alloc(8), rootSize = f.alloc(8), position = f.alloc(8);
        call(Size_copy, {shadowSize, vcall(self->shadow, node::getContentSize, {self->shadow})});
        call(Size_copy, {rootSize, vcall(self->root, node::getContentSize, {self->root})});
        const uint32_t root = self->root;
        const float x = (ldf(shadowSize) - ldf(rootSize)) * 0.5f;
        const float y = (ldf(shadowSize + 4) - ldf(rootSize + 4)) * 0.5f;
        call(Vec2_ctorXY, {position, fbits(x), fbits(y)});
        vcall(root, node::setPosition, {root, position});
        call(Vec2_dtor, {position});
    }
    {
        Frame f;
        const uint32_t root = self->root;
        const uint32_t done = newEmptyFunction(f);
        const uint32_t in = SceneUtil::createTopInAnimation(root, 0.0f, 0.0f, done);
        call(fn::Node_runAction, {root, call(EaseOut_create, {in, fbits(OPEN_EASE_RATE)})});
        functionDestroy(done);
    }
    vcall(self->shadow, node::addChild, {self->shadow, self->root});
    vcall(self->parent, node::addChild, {self->parent, self->shadow});
    vcall(self->parent, node::addChild, {self->parent, addr});
    call(fn::APIManager_getInstance, {});

    const uint32_t back = find(self->root, S_BackButton);
    addTouch(back, VT_Back, {});
    setUpButton(self->root, S_NoticeButton, VT_Notice, TEXT_NOTICES);
    setUpButton(self->root, S_PlayerInfoButton, VT_PlayerInfo, TEXT_PLAYER_INFO);

    const uint32_t album = find(self->root, S_AlbumButton);
    addTouchFlag(album, VT_Album);
    setButtonLabel(album, TEXT_ALBUM);

    const uint32_t party = find(self->root, S_ItemButton);
    {
        Frame f;
        const uint32_t cb = newFunction(f, VT_Party, {0, party});
        call(fn::Widget_addTouchEventListener, {party, cb});
        functionDestroy(cb);
    }
    setButtonLabel(party, TEXT_PARTY);

    const uint32_t unionTransfer = find(self->root, S_UnionTransfers);
    addTouchFlag(unionTransfer, VT_UnionTransfer);
    setButtonLabel(unionTransfer, TEXT_UNION_TRANSFER);

    // the optional buttons are hidden unless the server enables their feature
    const uint32_t serial = find(self->root, S_SerialCodeButton);
    addTouch(serial, VT_SerialCode, {addr});
    setButtonLabel(serial, TEXT_SERIAL_CODES);
    if (!featureOn(0, FEATURE_SERIAL_CODE)) setVisible(serial, 0);

    const uint32_t support = setUpButton(self->root, S_SupportButton, VT_Support, TEXT_SUPPORT);
    if (!featureOn(0, FEATURE_SUPPORT)) setVisible(support, 0);

    const uint32_t legal = find(self->root, S_LicenseButton);
    addTouch(legal, VT_Legal, {addr});
    setButtonLabel(legal, TEXT_LEGAL);

    // account transfer: shown when enabled, and not to a Facebook-linked account
    const uint32_t transfer = setUpButton(self->root, S_TransferButton, VT_Transfer, TEXT_TRANSFER);
    if (!featureOn(0, FEATURE_TRANSFER) || featureOn(1, FEATURE2_FACEBOOK)) setVisible(transfer, 0);

    setUpButton(self->root, S_HelpButton, VT_Help, TEXT_HELP);
    setUpButton(self->root, S_OptionButton, VT_Option, TEXT_SETTINGS);
    setUpButton(self->root, S_CreditButton, VT_Credit, TEXT_CREDITS);

    const uint32_t userCode = find(self->root, S_UserButton);
    addTouch(userCode, VT_UserCode, {addr});
    setButtonLabel(userCode, TEXT_USER_CODE);
    if (!featureOn(0, FEATURE_USER_CODE)) setVisible(userCode, 0);

    const uint32_t movie = find(self->root, S_Movie);
    setVisible(movie, 0);
    vcall(movie, VSLOT_setTouchEnabled, {movie, 0});

    const uint32_t facebook = find(self->root, S_FaceBookButton);
    if (featureOn(1, FEATURE2_FACEBOOK)) {
        setVisible(facebook, 1);
        call(OtherMenuDialog_updateFacebookButton, {addr});
        Frame f;
        const uint32_t cb = f.alloc(20);
        st32(cb, VT_Facebook);
        st32(cb + 4, facebook);
        st32(cb + 12, addr);              // the word at +8 is never set by the original either
        st32(cb + 16, cb);
        call(fn::Widget_addTouchEventListener, {facebook, cb});
        functionDestroy(cb);
    } else {
        setVisible(facebook, 0);
    }

    // SceneManager's popup stack: BACK asks $_16 (via the back button) whether it may close, $_17 closes it
    const uint32_t manager = sceneManager();
    Frame f;
    const uint32_t canClose = newFunction(f, VT_CanClose, {back});
    const uint32_t closed = newFunction(f, VT_Closed, {addr});
    call(fn::SceneManager_openPopup, {manager, canClose, closed, 1});
    functionDestroy(closed);
    functionDestroy(canClose);
}

// slides the layout out to the top and fades the shadow; $_22 removes the dialog when the slide ends
void OtherMenuDialog::close(OtherMenuDialog *self) {
    if (!self->parent) return;
    const uint32_t root = self->root;
    {
        Frame f;
        const uint32_t done = newFunction(f, VT_CloseDone, {rw::guest_addr(self)});
        const uint32_t out = SceneUtil::createTopOutAnimation(root, 0.0f, 0.0f, done);
        call(fn::Node_runAction, {root, call(EaseSineOut_create, {out})});
        functionDestroy(done);
    }
    const uint32_t shadow = self->shadow;
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {rw::load<uint32_t>(SceneUtil_IN_OUT_TIME), 0})});
}

RW_FUNCTION(0x59E84C, "OtherMenuDialog::create", OtherMenuDialog::create);
RW_FUNCTION(0x59E8E0, "OtherMenuDialog::open", OtherMenuDialog::open);
RW_FUNCTION(0x59FDB8, "OtherMenuDialog::close", OtherMenuDialog::close);
RW_FUNCTION(0x59F710, "OtherMenuDialog::setButtonLabel (sub_59F710)", OtherMenuDialog::setButtonLabel);
