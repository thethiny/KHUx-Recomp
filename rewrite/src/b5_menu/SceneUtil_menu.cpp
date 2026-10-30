// SceneUtil popups of the menus: the album challenge list.
#include "../scene/FilterPopup.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SM_getInstance = 0x60C26D, SM_getPopupLayer = 0x613E69, SM_openPopup = 0x61524D;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t VectorInt_pushBackSlow = 0x47D709;      // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t VectorInt_dtor = 0x478961;
constexpr uint32_t AlbumChallengeScrollView_create = 0x5BAD8D;
constexpr uint32_t BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t FunctionBool_swap = 0x47D0F1;           // std::function<bool()>::swap
constexpr uint32_t shared_release = 0xC8DBD1;
}  // namespace g

constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214, V_setTouchEnabled = 0x29C;
constexpr uint32_t V_setContentSize = 0xA8, V_getContentSize = 0xAC;

// The album challenge list (Album_Challenge_ver112) on popup layer 1: "achieved" text, the two challenge
// groups (ids 1, 2) in an AlbumChallengeScrollView, Update and Back (both off until the open animation ends,
// $_ {update, back}); the list's back handler and the popup's check share $_ {back}; the result lambda
// {back, update, shadow, widget, layer} closes it
uint32_t openAlbumChallengeListPopup() {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {call(g::SM_getInstance, {})});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.15f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), 0xDF9DBF});
    vcall(shadow, node::addChild, {shadow, widget});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    const uint32_t achieved = findNode(widget, 0xDF9DEE, 12);           // "Txt_Achieved"
    call(g::SceneUtil_setShadow, {achieved});
    {
        Frame f;
        const uint32_t text = f.alloc(8);
        call(g::TextData_get, {text, 0, 103600005});
        const uint32_t s = newStr(f, ld32(ld32(text) + 4));
        call(g::Text_setString, {achieved, s});
        strFree(s);
        if (ld32(text + 4)) call(g::shared_release, {ld32(text + 4)});
    }
    Frame f;
    const uint32_t ids = f.alloc(12), item = f.alloc(4), item2 = f.alloc(4);
    st32(item, 1);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    call(g::VectorInt_pushBackSlow, {ids, item});
    st32(item2, 2);
    if (ld32(ids + 4) >= ld32(ids + 8)) {
        call(g::VectorInt_pushBackSlow, {ids, item2});
    } else {
        const uint32_t end = ld32(ids + 4);
        if (end) st32(end, 2);
        st32(ids + 4, (end ? end : 0) + 4);
    }
    const uint32_t area = findNode(widget, 0xDF9B59, 11);               // "Scroll_Area"
    const uint32_t view = call(g::AlbumChallengeScrollView_create, {});
    vcall(view, V_setContentSize, {view, vcall(area, V_getContentSize, {area})});
    call(g::BaseScrollViewInt_setItemIDs, {view, ids});
    vcall(area, node::addChild, {area, view});
    const uint32_t update = findNode(widget, 0xDF9DFB, 13);             // "Button_Update"
    vcall(update, V_setTouchEnabled, {update, 0});
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF53CC8, {achieved});
        call(g::Widget_addTouchEventListener, {update, fo});
        functionDestroy(fo);
    }
    const uint32_t back = findNode(widget, 0xDF9E09, 10);               // "BackButton"
    vcall(back, V_setTouchEnabled, {back, 0});
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF53C70, {update, back});
        call(g::Node_runAction, {widget, call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF53C44, {back});
        call(g::FunctionBool_swap, {fo, view + 0x668});
        functionDestroy(fo);
    }
    {
        const uint32_t m = call(g::SM_getInstance, {});
        Frame t;
        const uint32_t check = newFunction(t, 0xF53C44, {back});
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, 0xF53BEC);
            st32(obj + 4, back);
            st32(obj + 8, update);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, widget);
            st32(obj + 0x14, layer);
        }
        st32(result + 16, obj);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(check);
    }
    call(g::VectorInt_dtor, {ids});
    return 1;
}
// SceneUtil::shareScreenShot(node, done): with the publish_actions permission and a Facebook login, the node is
// rendered (touch effects off meanwhile) and saved as drawScreenShot.png in the writable path, then done(texture,
// path) - or the "save failed" dialog; without the permission the access token is refreshed first ($_ {node,
// done})
void shareScreenShot(uint32_t node, uint32_t done) {
    constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
    constexpr uint32_t VectorString_pushBackSlow = 0x484465, VectorString_copy = 0x47A9A5;
    constexpr uint32_t VectorString_dtor = 0x47363F;
    constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_hasPermissions = 0x4E56C9;
    constexpr uint32_t FacebookManager_refreshCurrentAccessToken = 0x4E573D;
    constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
    constexpr uint32_t FileUtils_getInstance = rwlib::cocos2d::FileUtils::getInstance;
    constexpr uint32_t String_append = 0x47AC89;            // (string&, const char*, size_t) -> string&
    constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance, Director_getTextureCache = rwlib::cocos2d::Director::getTextureCache;
    constexpr uint32_t TextureCache_removeTextureForKey = rwlib::cocos2d::TextureCache::removeTextureForKey;
    constexpr uint32_t SM_setEnabledTouchEffect = 0x60CF65;
    constexpr uint32_t SceneUtil_generateScreenShotTexture = 0x47A449;
    constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release;
    constexpr uint32_t RenderTexture_saveToFile = rwlib::cocos2d::RenderTexture::saveToFile;  // (rt, const string&, Format, bool)
    constexpr uint32_t FunctionShare_call = 0x47AA2D;        // function<void(RenderTexture*, const string&)>()
    constexpr uint32_t FunctionShare_copy = 0x47A9F9, FunctionShare_copyHeap = 0x47B5C5;
    constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;
    constexpr uint32_t S_FILE = 0xDF9F8D;                    // "drawScreenShot.png"

    call(AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t perms = f.alloc(12), perm = f.alloc(12);
    st32(perms, 0);
    st32(perms + 4, 0);
    st32(perms + 8, 0);
    strInit(perm, 0xDF9F7D, 15);                             // "publish_actions"
    if (ld32(perms + 4) >= ld32(perms + 8)) {
        call(VectorString_pushBackSlow, {perms, perm});
    } else {
        const uint32_t end = ld32(perms + 4);
        if (end) {
            st32(end, ld32(perm));
            st32(end + 4, ld32(perm + 4));
            st32(end + 8, ld32(perm + 8));
            st32(perm, 0);
            st32(perm + 4, 0);
            st32(perm + 8, 0);
        }
        st32(perms + 4, (end ? end : 0) + 12);
    }
    strFree(perm);
    uint32_t allowed;
    {
        const uint32_t fbm = call(FacebookManager_getInstance, {});
        Frame t;
        const uint32_t copy = t.alloc(12);
        call(VectorString_copy, {copy, perms});
        allowed = call(FacebookManager_hasPermissions, {fbm, copy});
        call(VectorString_dtor, {copy});
    }
    if (allowed) {
        if (call(SceneUtil_isLoggedInFacebook, {}) == 1) {
            const uint32_t path = f.alloc(12);
            {
                const uint32_t fu = call(FileUtils_getInstance, {});
                Frame t;
                const uint32_t writable = t.alloc(12);
                vcall(fu, 0x44, {writable, fu});
                const uint32_t r = call(String_append, {writable, S_FILE, 18});
                st32(path, ld32(r));
                st32(path + 4, ld32(r + 4));
                st32(path + 8, ld32(r + 8));
                st32(r, 0);
                st32(r + 4, 0);
                st32(r + 8, 0);
                strFree(writable);
            }
            call(TextureCache_removeTextureForKey, {call(Director_getTextureCache, {call(Director_getInstance, {})}), path});
            call(SM_setEnabledTouchEffect, {call(g::SM_getInstance, {}), 0});
            const uint32_t rt = call(SceneUtil_generateScreenShotTexture, {node});
            call(SM_setEnabledTouchEffect, {call(g::SM_getInstance, {}), 1});
            call(Ref_retain, {rt});
            uint32_t saved;
            {
                Frame t;
                const uint32_t name = newStr(t, S_FILE, 18);
                saved = call(RenderTexture_saveToFile, {rt, name, 2, 1});
                strFree(name);
            }
            if (saved == 1) {
                call(FunctionShare_call, {done, rt, path});
            } else {
                Frame t;
                const uint32_t text = t.alloc(8);
                call(g::TextData_get, {text, 0, 104900018});
                const uint32_t msg = newStr(t, ld32(ld32(text) + 4));
                const uint32_t fo = newEmptyFunction(t);
                call(SceneUtil_openOkDialog4Line, {msg, fo, 0});
                functionDestroy(fo);
                strFree(msg);
                if (ld32(text + 4)) call(g::shared_release, {ld32(text + 4)});
            }
            call(Ref_release, {rt});
            strFree(path);
        }
    } else {
        const uint32_t a = f.alloc(20);
        call(FunctionShare_copy, {a, done});
        const uint32_t fbm = call(FacebookManager_getInstance, {});
        const uint32_t b = f.alloc(20);
        functionCopy(b, a);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x28});
        if (obj) {
            st32(obj, 0xF5392C);
            st32(obj + 8, node);
            call(FunctionShare_copyHeap, {obj + 0x10, b});
            st32(obj + 8, node);
        }
        st32(fo + 16, obj);
        call(FacebookManager_refreshCurrentAccessToken, {fbm, fo});
        functionDestroy(fo);
        functionDestroy(b);
        functionDestroy(a);
    }
    call(VectorString_dtor, {perms});
}
// SceneUtil::openFBSharePhotoPopup(&editing, &editNode, &editLabel, &text, delegate, message, screenshot, done):
// the Facebook share popup: title, the comment limit (misc 810), the comment field (a Text over a hidden edit
// box, limited), the screenshot, OK ($_ {message, edit box}) and Cancel; the result lambda {ok, cancel, widget,
// layer, shadow, done} closes it
uint32_t openFBSharePhotoPopup(uint32_t editing, uint32_t editNode, uint32_t editLabel, uint32_t text,
                               uint32_t delegate, uint32_t message, uint32_t screenshot, uint32_t done) {
    constexpr uint32_t SM_getPopupLayer2 = 0x613E71;
    constexpr uint32_t MasterMisc_create = 0x786D5D;
    constexpr uint32_t SafeBuffer_read = 0x45A269;
    constexpr uint32_t TextField_setMaxLengthEnabled = rwlib::cocos2d::ui::TextField::setMaxLengthEnabled, TextField_setMaxLength = rwlib::cocos2d::ui::TextField::setMaxLength;
    constexpr uint32_t SceneUtil_textFieldToUiText = 0x45C465, SceneUtil_textFieldToEditBox = 0x45C0A1;
    constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69;
    constexpr uint32_t S_CALLIGRAPHY = 0xDF9F1F, S_SOGEI = 0xDF9F4D, S_EMPTY = 0xDF8A1A;

    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(SM_getPopupLayer2, {call(g::SM_getInstance, {})});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), 0xDF9EB0});
    vcall(shadow, node::addChild, {shadow, widget});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    titleLabel(widget, 0xDF9EE3, 9, 104900011);                         // "Txt_Share"
    uint32_t limit;
    {
        Frame f;
        const uint32_t misc = f.alloc(8), word = f.alloc(4);
        call(MasterMisc_create, {misc, 810});
        const uint32_t row = ld32(misc);
        call(SafeBuffer_read, {row, word, ld32(row + 0xC) + 4, 4, 4});
        limit = ld32(word);
        if (ld32(misc + 4)) call(g::shared_release, {ld32(misc + 4)});
    }
    {
        const uint32_t label = findNode(widget, 0xDF9EED, 9);           // "Txt_Limit"
        call(g::SceneUtil_setShadow, {label});
        Frame f;
        const uint32_t t = f.alloc(8);
        call(g::TextData_get, {t, 0, 104900013});
        const uint32_t s = newStr(f, stringBuffer(f, {ld32(ld32(t) + 4), limit}));
        call(g::Text_setString, {label, s});
        strFree(s);
        if (ld32(t + 4)) call(g::shared_release, {ld32(t + 4)});
    }
    titleLabel(widget, 0xDF9EF7, 11, 104900012);                        // "Txt_Comment"
    titleLabel(widget, 0xDF9F03, 9, 104900014);                         // "Txt_Image"
    const uint32_t field = findNode(widget, 0xDF9F0D, 17);              // "TextField_Comment"
    call(TextField_setMaxLengthEnabled, {field, 1});
    call(TextField_setMaxLength, {field, limit});
    st32(text, call(SceneUtil_textFieldToUiText, {field, S_EMPTY, S_CALLIGRAPHY}));
    st8(editing, 0);
    uint32_t edit;
    {
        Frame f;
        const uint32_t placeholder = f.alloc(12);
        vcall(field, 0x150, {placeholder, field});
        const uint32_t size = vcall(field, V_getContentSize, {field});
        edit = rw::call(SceneUtil_textFieldToEditBox, {editNode, editLabel, field, placeholder, S_EMPTY, S_SOGEI, size,
                                                       delegate, editing, 0, 0});
        strFree(placeholder);
    }
    vcall(edit, node::setVisible, {edit, 0});
    {
        const uint32_t screen = findNode(widget, 0xDF9F70, 12);         // "Share_Screen"
        vcall(screen, node::addChild, {screen, screenshot});
    }
    const uint32_t ok = findNode(widget, 0xDF8688, 9);                  // "Button_OK"
    const uint32_t cancel = findNode(widget, 0xDF8692, 12);             // "Button_Close"
    Frame f;
    const uint32_t a = f.alloc(20);
    call(FunctionVB_copy, {a, done});
    const uint32_t b = f.alloc(20);
    functionCopy(b, a);
    titleLabel(ok, 0xDF869F, 6, 100100001);                             // "Txt_OK"
    {
        Frame t;
        const uint32_t msg = t.alloc(12);
        if (strIsLong(message)) {
            strInit(msg, ld32(message + 8), ld32(message + 4));
        } else {
            st32(msg, ld32(message));
            st32(msg + 4, ld32(message + 4));
            st32(msg + 8, ld32(message + 8));
        }
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x14});
        if (obj) {
            st32(obj, 0xF53A60);
            if (strIsLong(msg)) {
                strInit(obj + 4, ld32(msg + 8), ld32(msg + 4));
            } else {
                st32(obj + 4, ld32(msg));
                st32(obj + 8, ld32(msg + 4));
                st32(obj + 0xC, ld32(msg + 8));
            }
            st32(obj + 0x10, edit);
        }
        st32(fo + 16, obj);
        call(g::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        strFree(msg);
    }
    titleLabel(cancel, 0xDF86A6, 10, 100100002);                        // "Txt_Cancel"
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF53A34, {});
        call(g::Widget_addTouchEventListener, {cancel, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t m = call(g::SM_getInstance, {});
        Frame t;
        const uint32_t check = newFunction(t, 0xF53A08, {ok});
        const uint32_t c = t.alloc(20);
        functionCopy(c, b);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x38});
        if (obj) {
            st32(obj, 0xF539B0);
            st32(obj + 8, ok);
            st32(obj + 0xC, cancel);
            st32(obj + 0x10, widget);
            st32(obj + 0x14, layer);
            st32(obj + 0x18, shadow);
            call(FunctionVB_move, {obj + 0x20, c});
        }
        st32(result + 16, obj);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(c);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF53984, {});
        call(g::Node_runAction, {widget, call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    functionDestroy(b);
    functionDestroy(a);
    return 0x62C;   // r0 at the return: the frame size (add sp, r0)
}
// SceneUtil::openAdventureSelectFilterPopup(done): the quest list filter (AdventureTop2_Filter_ver106) on popup
// layer 1: the texts, seven check boxes set from the AppState filter (story 1/2, challenge 1/2, power, speed,
// magic) - each with a listener holding the filter state twice - and Release (bright when the filter is on, its
// listener holding everything); every control stays off until the open animation ends ($_ {ok, release, close,
// boxes}); the result lambda (heap 0x68) keeps the controls, the state, the original filter bytes, done and the
// popup nodes. The shared_ptr copies are made and released in the original's order.
uint32_t openAdventureSelectFilterPopup(uint32_t done) {
    constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_getAdventureSelectFilterState = 0x4EB1A1;
    constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState, Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;
    constexpr uint32_t SetTextEnabledColor = 0x4744F5;      // sub_4744F4(text, bool, bool)
    constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69;
    auto addRef = [](uint32_t c) { if (c) call(0xC8DBB5, {c}); };
    auto releaseRef = [](uint32_t c) { if (c) call(0xC8DBD1, {c}); };

    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {call(g::SM_getInstance, {})});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.15f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), 0xDF9CE5});
    vcall(shadow, node::addChild, {shadow, widget});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    Frame f;
    const uint32_t state = f.alloc(8);
    call(AppState_getAdventureSelectFilterState, {state, call(AppState_getInstance, {})});
    uint8_t saved[8];
    for (uint32_t k = 0; k < 8; k++) saved[k] = ld8(ld32(state) + k);
    auto label = [&](uint32_t name, uint32_t len, int32_t id) {
        const uint32_t text = findNode(widget, name, len);
        call(g::SceneUtil_setShadow, {text});
        Frame t;
        const uint32_t td = t.alloc(8);
        call(g::TextData_get, {td, 0, uint32_t(id)});
        const uint32_t s = newStr(t, ld32(ld32(td) + 4));
        call(g::Text_setString, {text, s});
        strFree(s);
        releaseRef(ld32(td + 4));
    };
    label(0xDF9D19, 10, 102900032);                                     // "Txt_Title4"
    label(0xDF9D24, 14, 102900028);                                     // "Txt_Challenge1"
    label(0xDF9D33, 14, 102900029);                                     // "Txt_Challenge2"
    label(0xDF9D42, 10, 102900030);                                     // "Txt_Story1"
    label(0xDF9D4D, 10, 102900031);                                     // "Txt_Story2"
    const uint32_t ch1 = findNode(widget, 0xDF9D58, 17), ch2 = findNode(widget, 0xDF9D6A, 17);
    const uint32_t st1 = findNode(widget, 0xDF9D7C, 13), st2 = findNode(widget, 0xDF9D8A, 13);
    const uint32_t pow = findNode(widget, 0xDF9D98, 12), spd = findNode(widget, 0xDF9DA5, 12);
    const uint32_t mag = findNode(widget, 0xDF9DB2, 12);
    const uint32_t releaseButton = findNode(widget, 0xDF9CCA, 14);      // "Button_Release"
    titleLabel(releaseButton, 0xDF9CD9, 11, 102800001);
    const uint32_t releaseText = findNode(releaseButton, 0xDF9CD9, 11); // "Txt_Release"

    const uint32_t obj = ld32(state);
    const uint32_t c1 = ld32(state + 4);                                // copy 1
    addRef(c1);
    call(CheckBox_setSelectedState, {st1, ld8(obj + 3) ? 1u : 0u});
    call(CheckBox_setSelectedState, {st2, ld8(obj + 4) ? 1u : 0u});
    call(CheckBox_setSelectedState, {ch1, ld8(obj + 1) ? 1u : 0u});
    call(CheckBox_setSelectedState, {ch2, ld8(obj + 2) ? 1u : 0u});
    call(CheckBox_setSelectedState, {pow, ld8(obj + 5) ? 1u : 0u});
    call(CheckBox_setSelectedState, {spd, ld8(obj + 6) ? 1u : 0u});
    call(CheckBox_setSelectedState, {mag, ld8(obj + 7) ? 1u : 0u});
    const uint32_t obj2 = ld32(state);
    const uint32_t c2 = ld32(state + 4);                                // copy 2 (and copy 3)
    uint32_t c3 = 0;
    if (c2) {
        addRef(c2);
        c3 = ld32(state + 4);
        addRef(c3);
    }
    call(Widget_setBright, {releaseButton, ld8(ld32(state)) ? 1u : 0u});

    // Release's listener {state, release, its text, story1, state, story2, challenge 1/2, power, speed, magic}
    {
        const uint32_t so = ld32(state), sc = ld32(state + 4);
        addRef(sc);
        addRef(c1);
        Frame t;
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x38});
        uint32_t keep1 = c1, keep2 = sc;
        if (h) {
            const uint32_t words[] = {0xF53F30, so, sc, releaseButton, releaseText, st1, obj, c1, st2, ch1, ch2, pow, spd, mag};
            for (uint32_t k = 0; k < 14; k++) st32(h + 4 * k, words[k]);
            keep1 = keep2 = 0;
        }
        st32(fo + 16, h);
        call(g::Widget_addTouchEventListener, {releaseButton, fo});
        functionDestroy(fo);
        releaseRef(keep1);
        releaseRef(keep2);
    }
    // the check boxes' listeners {state, state, release, its text}
    static constexpr uint32_t BOX_VT[7] = {0xF53F04, 0xF53ED8, 0xF53EAC, 0xF53E80, 0xF53E54, 0xF53E28, 0xF53DFC};
    const uint32_t boxes[7] = {st1, st2, ch1, ch2, pow, spd, mag};
    for (uint32_t i = 0; i < 7; i++) {
        const uint32_t so = ld32(state), sc = ld32(state + 4);
        addRef(sc);
        addRef(c2);
        Frame t;
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x1C});
        uint32_t keep1 = c2, keep2 = sc;
        if (h) {
            const uint32_t words[] = {BOX_VT[i], so, sc, obj2, c2, releaseButton, releaseText};
            for (uint32_t k = 0; k < 7; k++) st32(h + 4 * k, words[k]);
            keep1 = keep2 = 0;
        }
        st32(fo + 16, h);
        call(g::Widget_addTouchEventListener, {boxes[i], fo});
        functionDestroy(fo);
        releaseRef(keep1);
        releaseRef(keep2);
    }
    for (uint32_t b : boxes) vcall(b, V_setTouchEnabled, {b, 0});
    const uint32_t ok = findNode(widget, 0xDF8688, 9);                  // "Button_OK"
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF53DD0, {});
        call(g::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
    }
    titleLabel(ok, 0xDF869F, 6, 100100001);                             // "Txt_OK"
    vcall(ok, V_setTouchEnabled, {ok, 0});
    vcall(releaseButton, V_setTouchEnabled, {releaseButton, 0});
    const uint8_t filtered = ld8(ld32(state));
    call(Widget_setBright, {releaseButton, filtered ? 1u : 0u});
    vcall(releaseButton, V_setTouchEnabled, {releaseButton, filtered ? 1u : 0u});
    call(SetTextEnabledColor, {releaseText, filtered ? 1u : 0u, 0});
    const uint32_t close = findNode(widget, 0xDF8D2F, 12);              // "Close_Button"
    vcall(close, V_setTouchEnabled, {close, 0});

    // the closure: the controls, the state (copied), the original filter bytes, done, the popup nodes
    const uint32_t closureCtrl = ld32(state + 4);
    addRef(closureCtrl);
    const uint32_t doneCopy = f.alloc(20);
    call(FunctionVB_copy, {doneCopy, done});
    {
        Frame t;
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x2C});
        if (h) {
            const uint32_t words[] = {0xF53D78, ok, releaseButton, close, st1, st2, ch1, ch2, pow, spd, mag};
            for (uint32_t k = 0; k < 11; k++) st32(h + 4 * k, words[k]);
        }
        st32(fo + 16, h);
        call(g::Node_runAction, {widget, call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    {
        const uint32_t m = call(g::SM_getInstance, {});
        Frame t;
        const uint32_t check = newFunction(t, 0xF53D4C, {close});
        const uint32_t copyCtrl = closureCtrl;                          // the closure's state copied again
        addRef(copyCtrl);
        const uint32_t doneTmp = t.alloc(20);
        functionCopy(doneTmp, doneCopy);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x68});
        uint32_t leftCtrl = copyCtrl;
        if (h) {
            st32(h, 0xF53CF4);
            const uint32_t words[] = {close, releaseButton, ok, st1, st2, ch1, ch2, pow, spd, mag};
            for (uint32_t k = 0; k < 10; k++) st32(h + 8 + 4 * k, words[k]);
            st32(h + 0x30, ld32(state));
            st32(h + 0x34, copyCtrl);
            leftCtrl = 0;
            for (uint32_t k = 0; k < 8; k++) st8(h + 0x38 + k, saved[k]);
            call(FunctionVB_move, {h + 0x40, doneTmp});
            st32(h + 0x58, shadow);
            st32(h + 0x5C, widget);
            st32(h + 0x60, layer);
        }
        st32(result + 16, h);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(doneTmp);
        releaseRef(leftCtrl);
        functionDestroy(check);
    }
    functionDestroy(doneCopy);
    releaseRef(closureCtrl);
    if (c2) releaseRef(c3);
    releaseRef(c2);
    releaseRef(c1);
    releaseRef(ld32(state + 4));
    return 1;
}

// openAdventureSelectFilterPopup's lambdas. The state: 0 on, 1 / 2 challenge, 3 / 4 story, 5 power, 6 speed, 7 magic.
namespace advfilter {
constexpr uint32_t ON = 0, CH1 = 1, CH2 = 2, ST1 = 3, ST2 = 4, POW = 5, SPD = 6, MAG = 7;
constexpr filter::Flags ANY = {ST1, ST2, CH1, CH2, POW, SPD, MAG};
constexpr filter::Flags ALL = {ON, CH1, CH2, ST1, ST2, POW, SPD, MAG};
constexpr uint32_t VT_ClosedEnd = 0xF53D20;   // $_116's end {layer, shadow, popup}

// $_105..$_111 (a check box)
void st1Touched(uint32_t f, uint32_t, uint32_t typeRef) { filter::boxTouched(f, typeRef, ST1, ANY); }
void st2Touched(uint32_t f, uint32_t, uint32_t typeRef) { filter::boxTouched(f, typeRef, ST2, ANY); }
void ch1Touched(uint32_t f, uint32_t, uint32_t typeRef) { filter::boxTouched(f, typeRef, CH1, ANY); }
void ch2Touched(uint32_t f, uint32_t, uint32_t typeRef) { filter::boxTouched(f, typeRef, CH2, ANY); }
void powTouched(uint32_t f, uint32_t, uint32_t typeRef) { filter::boxTouched(f, typeRef, POW, ANY); }
void spdTouched(uint32_t f, uint32_t, uint32_t typeRef) { filter::boxTouched(f, typeRef, SPD, ANY); }
void magTouched(uint32_t f, uint32_t, uint32_t typeRef) { filter::boxTouched(f, typeRef, MAG, ANY); }
// $_103 (Release) {state, -, release, text, story1, state, -, story2, challenge 1 / 2, power, speed, magic}
void releaseTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    filter::releaseTouched(f, typeRef, ALL, {ST1, ST2, CH1, CH2, POW, SPD, MAG});
}
// $_116 (closed) {-, 10 controls +8, state +0x30, -, the snapshot +0x38, done function<void(bool)> +0x40, shadow,
// popup, layer}(ok): the controls off; OK -> saved; Close -> the snapshot put back; done(ok); the popup closes
void closed(uint32_t f, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    filter::controlsOff(f, 10);
    if (ok)
        call(filter::AppState_save, {call(fn::AppState_getInstance, {})});
    else
        filter::restore(f + 0x30, f + 0x38, ALL);
    call(fn::function_vb_call, {f + 0x40, ok != 0});
    filter::close(ld32(f + 0x58), ld32(f + 0x5C), ld32(f + 0x60), VT_ClosedEnd);
}
}  // namespace advfilter
}  // namespace

RW_FUNCTION(0x476E80, "SceneUtil::openAdventureSelectFilterPopup(function<void(bool)>)", openAdventureSelectFilterPopup);
RW_FUNCTION(0x4799D8, "SceneUtil::openFBSharePhotoPopup(...)", openFBSharePhotoPopup);
RW_FUNCTION(0x47A558, "SceneUtil::shareScreenShot(Node*, function<void(RenderTexture*, const string&)>)", shareScreenShot);
RW_FUNCTION(0x4783C4, "SceneUtil::openAlbumChallengeListPopup()", openAlbumChallengeListPopup);
RW_FUNCTION(0x47F024, "SceneUtil::openAdventureSelectFilterPopup $_105::operator() (story 1)", advfilter::st1Touched);
RW_FUNCTION(0x47EDEC, "SceneUtil::openAdventureSelectFilterPopup $_106::operator() (story 2)", advfilter::st2Touched);
RW_FUNCTION(0x47EBB4, "SceneUtil::openAdventureSelectFilterPopup $_107::operator() (challenge 1)", advfilter::ch1Touched);
RW_FUNCTION(0x47E97C, "SceneUtil::openAdventureSelectFilterPopup $_108::operator() (challenge 2)", advfilter::ch2Touched);
RW_FUNCTION(0x47E744, "SceneUtil::openAdventureSelectFilterPopup $_109::operator() (power)", advfilter::powTouched);
RW_FUNCTION(0x47E50C, "SceneUtil::openAdventureSelectFilterPopup $_110::operator() (speed)", advfilter::spdTouched);
RW_FUNCTION(0x47E2D4, "SceneUtil::openAdventureSelectFilterPopup $_111::operator() (magic)", advfilter::magTouched);
RW_FUNCTION(0x47F28C, "SceneUtil::openAdventureSelectFilterPopup $_103::operator() (Release)", advfilter::releaseTouched);
RW_FUNCTION(0x47DB14, "SceneUtil::openAdventureSelectFilterPopup $_116::operator()(bool) (closed)", advfilter::closed);

namespace {
namespace albumchallenge {
constexpr uint32_t SM_SHOW_CLEARED = 0x30B;     // SceneManager byte: the challenge list shows the cleared ones
constexpr uint32_t VT_ClosedEnd = 0xF53C18;     // $_121's end {layer, shadow, popup}
constexpr int32_t TXT_SHOW_CLEARED = 103600005, TXT_HIDE_CLEARED = 103600006;

// $_121 (closed) {button, button, shadow, popup, layer}(ok): both buttons off, Close -> the cancel sound; the shadow
// fades, the popup shrinks
void closed(uint32_t f, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    vcall(ld32(f + 4), node::setTouchEnabled, {ld32(f + 4), 0});
    vcall(ld32(f + 8), node::setTouchEnabled, {ld32(f + 8), 0});
    if (!ok) call(fn::AudioPlaySystemSECancel, {});
    call(fn::Node_runAction, {ld32(f + 0xC), call(fn::FadeTo_create, {fbits(0.15f), 0})});
    const uint32_t popup = ld32(f + 0x10);
    Frame fr;
    const uint32_t end = newFunction(fr, VT_ClosedEnd, {ld32(f + 0x14), ld32(f + 0xC), popup});
    // r0 holds the end's vtable here (the animation does not read it)
    call(fn::Node_runAction, {popup, call(0x45BC61, {VT_ClosedEnd, end})});   // SceneUtil::createClosePopupAnimation
    functionDestroy(end);
}
// $_118 (show / hide cleared) {label}(sender, type): the decide sound, the setting flipped, the label to match
void toggleTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t shown = ld8(sceneManager() + SM_SHOW_CLEARED);
    const uint32_t flag = sceneManager() + SM_SHOW_CLEARED;
    st8(flag, shown ? 0 : 1);
    setTextId(ld32(f + 4), shown ? TXT_HIDE_CLEARED : TXT_SHOW_CLEARED);
}
}  // namespace albumchallenge
}  // namespace

RW_FUNCTION(0x47CE60, "SceneUtil::openAlbumChallengeListPopup $_121::operator()(bool) (closed)", albumchallenge::closed);
RW_FUNCTION(0x47D444, "SceneUtil::openAlbumChallengeListPopup $_118::operator() (show cleared)", albumchallenge::toggleTouched);
