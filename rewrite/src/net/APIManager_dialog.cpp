// APIManager::openErrorDialog: after a failed request, show the error (a PopupDialog with errorMessage, or the
// maintenance web page when the reply flagged it). Both popups get a std::function wrapping a lambda that
// captures {this, the caller's callback, retry flag}; the lambda's code is the __func vtable given here.
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t SCENE_MANAGER_GET_INSTANCE = 0x60C26D;  // SceneManager::getInstance
constexpr uint32_t SCENE_MANAGER_POPUP_LAYER3 = 0x613E79;  // SceneManager::getPopupLayer3
constexpr uint32_t POPUP_DIALOG_CREATE = 0x4F3C15;         // PopupDialog::create(ButtonType, WindowType)
constexpr uint32_t POPUP_DIALOG_OPEN = 0x4F420D;           // PopupDialog::open(Node*, const string&, function<void(bool)>)
constexpr uint32_t OPEN_WEBVIEW_INFO_POPUP = 0x474701;     // SceneUtil::openWebViewInfoPopup(layer, title, url, func, string)
constexpr uint32_t AUDIO_PLAY_POPUP_SE = 0x5C0E09;         // AudioPlaySystemSEPopUp()
constexpr uint32_t STRING_FROM_CHARS = 0x47CD93;           // std::string(const char*, size_t)
constexpr uint32_t DIALOG_LAMBDA_VTABLE = 0xF55658;        // __func<openErrorDialog lambda #1>
constexpr uint32_t WEBVIEW_LAMBDA_VTABLE = 0xF55684;       // __func<openErrorDialog lambda #2>
constexpr uint32_t S_EMPTY = 0xDF8A1A;                     // ""

constexpr uint32_t RET_SHOW_WEBVIEW = 5;                   // Ret: u8, the error page is a web view
constexpr uint32_t RET_URL = 0x14;                         // Ret: std::string, its URL

// libc++ std::function copy constructor as inlined: empty, clone into the destination buffer (__clone(dst),
// slot 3) or a heap clone (__clone(), slot 2). fnSlot = the destination's __f_ word.
void copyFunction(const Frame &F, uint32_t src, uint32_t dst, uint32_t dstF) {
    uint32_t f = load32(src + 16);
    if (f == 0) {
        rw::save<uint32_t>(dstF, 0);
    } else if (f == src) {
        rw::save<uint32_t>(dstF, dst);
        F.vcall(f, 3, {f, dst});
    } else {
        rw::save<uint32_t>(dstF, F.vcall(f, 2, {f}));
    }
}

// A lambda {APIManager* self, std::function<void(bool)> callback, bool flag} at closure (function at +8, flag
// at +0x20) wrapped in a heap __func (std::function's callable): returns it (or null when new failed).
uint32_t wrapLambda(const Frame &F, uint32_t closure, uint32_t vtable) {
    uint32_t p = F.call(OPERATOR_NEW, {0x30});
    if (p) {
        rw::save<uint32_t>(p, vtable);
        rw::save<uint32_t>(p + 8, load32(closure));
        copyFunction(F, closure + 8, p + 0x10, p + 0x20);
        rw::store8(rw::host_ptr<uint8_t>(p + 0x28), load8(closure + 0x20));
    }
    return p;
}

}  // namespace

uint32_t APIManager::openErrorDialog(APIManager *self, uint32_t cb, uint32_t retry) {
    Frame F(0x108);
    if (!self->failed) return 0;
    // the lambda captures, kept on the stack: {self, callback copy, retry}
    const uint32_t captures = F.at(0xC0);
    F.setWord(0xC0, addr(self));
    F.call(FN_BOOL_COPY, {captures + 8, cb});
    rw::store8(rw::host_ptr<uint8_t>(captures + 0x20), uint8_t(retry));
    uint32_t scenes = F.call(SCENE_MANAGER_GET_INSTANCE, {});
    uint32_t layer = F.call(SCENE_MANAGER_POPUP_LAYER3, {scenes});

    uint32_t shown;
    uint32_t ret = self->ret.ptr;
    if (!ret || !load8(ret + RET_SHOW_WEBVIEW)) {
        uint32_t dialog = F.call(POPUP_DIALOG_CREATE, {0, 0});
        // the lambda (by value) at sp+0x40, its std::function wrapper at sp+0x68
        const uint32_t lambda = F.at(0x40), fn = F.at(0x68);
        F.setWord(0x40, addr(self));
        copyFunction(F, captures + 8, lambda + 8, lambda + 0x18);
        rw::store8(rw::host_ptr<uint8_t>(lambda + 0x20), load8(captures + 0x20));
        F.setWord(0x78, 0);
        F.setWord(0x78, wrapLambda(F, lambda, DIALOG_LAMBDA_VTABLE));
        F.call(POPUP_DIALOG_OPEN, {dialog, layer, addr(&self->errorMessage), fn});
        destroyFunction(F, fn);
        destroyFunction(F, lambda + 8);
        shown = 1;
        F.call(AUDIO_PLAY_POPUP_SE, {});
    } else {
        const uint32_t lambda = F.at(0x80), fn = F.at(0xA8);
        F.setWord(0x80, addr(self));
        copyFunction(F, captures + 8, lambda + 8, lambda + 0x18);
        rw::store8(rw::host_ptr<uint8_t>(lambda + 0x20), load8(captures + 0x20));
        F.setWord(0xB8, 0);
        F.setWord(0xB8, wrapLambda(F, lambda, WEBVIEW_LAMBDA_VTABLE));
        F.call(STRING_FROM_CHARS, {F.at(0x34), S_EMPTY, 0});
        shown = F.call(OPEN_WEBVIEW_INFO_POPUP, {layer, addr(&self->errorMessage), ret + RET_URL, fn, F.at(0x34)});
        if (load8(F.at(0x34)) & 1) F.call(OPERATOR_DELETE, {F.word(0x3C)});
        destroyFunction(F, fn);
        destroyFunction(F, lambda + 8);
    }
    destroyFunction(F, captures + 8);
    return shown;
}
RW_FUNCTION_NOVERIFY(0x4C3C8C, "APIManager::openErrorDialog", APIManager::openErrorDialog);
