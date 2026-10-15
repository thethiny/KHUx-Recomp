// SceneUtil: small popup openers, text checks, screenshots, the medal count label.
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t PartyInfoDialog_create = 0x592995;
constexpr uint32_t PartyInfoDialog_open = 0x592A39;            // (dlg, parent, u64 party, function<void(bool)>, bool)
constexpr uint32_t PartyInfoDialog_openRequest = 0x5939E1;
constexpr uint32_t PartyInfoDialog_openAlertRequest = 0x594B85;   // (dlg, parent, function<void(bool)>, bool)
constexpr uint32_t AvatarInfoDialog_create = 0x532A31;
constexpr uint32_t AvatarInfoDialog_open = 0x532E95;           // (dlg, parent, u64, function<void(bool)>, bool, function<bool()>)
constexpr uint32_t FunctionBool_copy = 0x46A3B5;               // std::function<bool()>(const function&)
constexpr uint32_t FunctionNode_copy = 0x474D99;               // std::function<void(Node*)>(const function&)
constexpr uint32_t VT_AvatarInfo_CanClose = 0xF54AB4;          // $_35
constexpr uint32_t VT_AvatarInfo_OnClose = 0xF54A88;           // $_36: [dialog]

constexpr uint32_t StringSubstr = 0x47CD4D;                    // std::string(const string&, pos, n)
constexpr uint32_t StringCompareC = 0x47CBD5;                  // string::compare(const char*)
constexpr uint32_t StringVector_pushBackSlowCopy = 0x48D891;   // vector<string>::__push_back_slow_path(const string&)
constexpr uint32_t StringVector_dtor = 0x47363F;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t STR_NewLine = 0xDF9E14, STR_Space = 0xDF9E16, STR_WideSpace = 0xDF9E18, STR_Tab = 0xDF9E1C;

constexpr uint32_t RenderTexture_create = 0x8A3FFD;            // (w, h, PixelFormat, depthStencilFormat)
constexpr uint32_t Node_visit = 0x8944C9;
constexpr uint32_t Renderer_render = 0x913611;
constexpr uint32_t RenderTexture_beginWithClear = 0x274;       // vtable slots
constexpr uint32_t RenderTexture_end = 0x280;
constexpr uint32_t Director_renderer = 0x114;

constexpr uint32_t SetCountLabel = 0x473CAD;                   // static helper (node, name, text, warning)
constexpr uint32_t STR_Txt_MedalGet = 0xDF9A4C, STR_Txt_MedalGet_All_Label = 0xDF9A59;
constexpr uint32_t STR_Txt_MedalGet_Slash = 0xDF9A70, STR_Slash = 0xDF8C66;
constexpr uint32_t STR_Txt_MedalGet_Num_Label = 0xDF9A83, STR_d = 0xDF86DE;
}  // namespace

// ------------------------------------------------------------------------------------------------ popups

void SceneUtil::openPartyInfoPopup(uint64_t party, uint32_t flag, Addr callback) {
    uint32_t dialog = call(PartyInfoDialog_create, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, callback});
    call(PartyInfoDialog_open, {dialog, layer, uint32_t(party), uint32_t(party >> 32), cb, flag});
    functionDestroy(cb);
    call(fn::AudioPlaySystemSEPopUp, {});
}
RW_FUNCTION(0x46A3E8, "SceneUtil::openPartyInfoPopup", SceneUtil::openPartyInfoPopup);

void SceneUtil::openPartyInfoRequestPopup(uint64_t party, uint32_t flag, Addr callback) {
    uint32_t dialog = call(PartyInfoDialog_create, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, callback});
    call(PartyInfoDialog_openRequest, {dialog, layer, uint32_t(party), uint32_t(party >> 32), cb, flag});
    functionDestroy(cb);
    call(fn::AudioPlaySystemSEPopUp, {});
}
RW_FUNCTION(0x478D58, "SceneUtil::openPartyInfoRequestPopup", SceneUtil::openPartyInfoRequestPopup);

void SceneUtil::openPartyInfoJoinAlertPopup(uint32_t flag, Addr callback) {
    uint32_t dialog = call(PartyInfoDialog_create, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, callback});
    call(PartyInfoDialog_openAlertRequest, {dialog, layer, cb, flag});
    functionDestroy(cb);
    call(fn::AudioPlaySystemSEPopUp, {});
}
RW_FUNCTION(0x478E1C, "SceneUtil::openPartyInfoJoinAlertPopup", SceneUtil::openPartyInfoJoinAlertPopup);

void SceneUtil::openAvatarInfoPopup(uint64_t user, uint32_t flag, Addr callback, Addr canClose) {
    uint32_t dialog = call(AvatarInfoDialog_create, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, callback});
    uint32_t check = f.alloc(20);
    call(FunctionBool_copy, {check, canClose});
    call(AvatarInfoDialog_open, {dialog, layer, uint32_t(user), uint32_t(user >> 32), cb, flag, check});
    functionDestroy(check);
    functionDestroy(cb);
    uint32_t mgr = sceneManager();
    uint32_t onCanClose = newFunction(f, VT_AvatarInfo_CanClose, {});
    uint32_t onClose = newFunction(f, VT_AvatarInfo_OnClose, {dialog});
    call(fn::SceneManager_openPopup, {mgr, onCanClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(onCanClose);
}
RW_FUNCTION(0x46A228, "SceneUtil::openAvatarInfoPopup", SceneUtil::openAvatarInfoPopup);

// openWebViewInfoPopup on popup layer 2
void SceneUtil::openWebViewInfoPopupDefault(Addr title, Addr url, Addr onClose, Addr extra) {
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    Frame f;
    uint32_t cb = f.alloc(20);
    call(FunctionNode_copy, {cb, onClose});
    call(0x474701 /* SceneUtil::openWebViewInfoPopup(Layer*, ...) */, {layer, title, url, cb, extra});
    functionDestroy(cb);
}
RW_FUNCTION(0x47463C, "SceneUtil::openWebViewInfoPopup", SceneUtil::openWebViewInfoPopupDefault);

// ------------------------------------------------------------------------------------------------ misc

// Does the text contain anything besides spaces (" ", U+3000), tabs and new lines? (UTF-8 aware.)
bool SceneUtil::splitSpaceCheck(Addr text) {
    Frame f;
    uint32_t chars = f.alloc(12);                      // std::vector<std::string>: one entry per character
    st32(chars, 0); st32(chars + 4, 0); st32(chars + 8, 0);
    uint32_t size = strSize(text);
    bool result = false;
    if (size) {
        uint32_t piece = f.alloc(12);
        uint32_t pos = 0;
        do {
            uint32_t data = (ld8(text) & 1) ? ld32(text + 8) : text + 1;
            int8_t lead = int8_t(ld8(data + pos));
            uint32_t b = uint8_t(lead);
            uint32_t len = 1;
            if (lead < 0) {
                if ((b >> 5) < 7) len = 2;
                else if ((b >> 4) < 0xF) len = 3;
                else if ((b >> 3) < 0x1F) len = 4;
                else if ((b >> 2) < 0x3F) len = 5;
                else if ((b >> 1) < 0x7F) len = 6;
                else len = 3;
            }
            call(StringSubstr, {piece, text, pos, len, text});
            uint32_t end = ld32(chars + 4);
            if (end == ld32(chars + 8)) {
                call(StringVector_pushBackSlowCopy, {chars, piece});
            } else {
                if (end) {
                    if (strIsLong(piece)) strInit(end, ld32(piece + 8), ld32(piece + 4));
                    else for (uint32_t w = 0; w < 12; w += 4) st32(end + w, ld32(piece + w));
                }
                st32(chars + 4, ld32(chars + 4) + 12);
            }
            strFree(piece);
            pos += len;
        } while (pos < size);
        uint32_t begin = ld32(chars);
        if (ld32(chars + 4) != begin) {
            auto count = [&]() { return ((ld32(chars + 4) - ld32(chars)) >> 2) * 0xAAAAAAABu; };
            for (uint32_t k = 0; k < count(); k++) {
                uint32_t entry = ld32(chars) + 12 * k;
                if (!call(StringCompareC, {entry, STR_NewLine})) continue;
                if (count() <= k) call(ThrowOutOfRange, {chars});
                if (!call(StringCompareC, {ld32(chars) + 12 * k, STR_Space})) continue;
                if (count() <= k) call(ThrowOutOfRange, {chars});
                if (!call(StringCompareC, {ld32(chars) + 12 * k, STR_WideSpace})) continue;
                if (count() <= k) call(ThrowOutOfRange, {chars});
                if (call(StringCompareC, {ld32(chars) + 12 * k, STR_Tab})) {
                    result = true;
                    break;
                }
            }
        }
    }
    call(StringVector_dtor, {chars});
    return result;
}
RW_FUNCTION(0x47898C, "SceneUtil::splitSpaceCheck", SceneUtil::splitSpaceCheck);

// Render `node` into a screen-sized RenderTexture.
SceneUtil::Addr SceneUtil::generateScreenShotTexture(Addr node) {
    Size size = visibleSize();
    uint32_t texture = call(RenderTexture_create, {uint32_t(f2i(size.width)), uint32_t(f2i(size.height)), 2, 0x88F0});
    {
        Frame f;
        vcall(texture, node::setPosition, {texture, newVec2(f, size.width * 0.5f, size.height * 0.5f)});
    }
    vcall(texture, RenderTexture_beginWithClear, {texture, 0, 0, 0, 0});
    call(Node_visit, {node});
    vcall(texture, RenderTexture_end, {texture});
    call(Renderer_render, {ld32(director() + Director_renderer)});
    return texture;
}
RW_FUNCTION_NOVERIFY(0x47A448, "SceneUtil::generateScreenShotTexture", SceneUtil::generateScreenShotTexture);

// "Medals: <count> / <box size>" (the count highlighted when over the box size).
void SceneUtil::updateMedalPossessionLabel(Addr node) {
    titleLabel(node, STR_Txt_MedalGet, 12, 101200021);
    uint32_t api = call(fn::APIManager_getInstance, {});
    int32_t box = safe32(ld32(api + 0x94), 0x34);
    {
        Frame f;
        uint32_t n = newStr(f, STR_Txt_MedalGet_All_Label, 22);
        uint32_t t = newStr(f, stringBuffer(f, {STR_d, uint32_t(box)}));
        call(SetCountLabel, {node, n, t, 0});
        strFree(t);
        strFree(n);
    }
    {
        Frame f;
        uint32_t n = newStr(f, STR_Txt_MedalGet_Slash, 18);
        uint32_t t = newStr(f, STR_Slash, 1);
        call(SetCountLabel, {node, n, t, 0});
        strFree(t);
        strFree(n);
    }
    int32_t count = int32_t(ld32(ld32(call(fn::APIManager_getInstance, {}) + 0xD4) + 0x20));
    {
        Frame f;
        uint32_t n = newStr(f, STR_Txt_MedalGet_Num_Label, 22);
        uint32_t t = newStr(f, stringBuffer(f, {STR_d, uint32_t(count)}));
        call(SetCountLabel, {node, n, t, count > box ? 1u : 0u});
        strFree(t);
        strFree(n);
    }
}
RW_FUNCTION(0x473A18, "SceneUtil::updateMedalPossessionLabel", SceneUtil::updateMedalPossessionLabel);
