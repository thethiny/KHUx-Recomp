// SceneShopTop: the bodies (operator()) of the scene's lambdas. `fn` is the libc++ __func object: vtable at +0,
// the captures from +4. Touch callbacks receive (Ref**, TouchEventType*) by pointer.
#include "SceneShopTop.hpp"

using namespace scene;
using namespace shop_top;
using Self = SceneShopTop;

namespace {
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t Node_unscheduleUpdate = rwlib::cocos2d::Node::unscheduleUpdate;
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;    // (bgm id, fade ms)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t DrawScrollView_setCanTouch = 0x5B13AD;
constexpr uint32_t Function_vb_call = 0x474139;           // std::function<void(bool)>::operator()
constexpr uint32_t Function_vb_assign = 0x490109;         // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;   // (mgr, function<Layer*()>, bool, bool, FadeType)
constexpr uint32_t SceneManager_hideHeader = 0x61154D;    // (mgr, function<void()>)
constexpr uint32_t SceneManager_hideHeaderRight = 0x613091;
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;
constexpr uint32_t SceneUtil_openMedalMaxUpDialog = 0x45E799;  // (function<void(bool,bool)>)
constexpr uint32_t SceneUtil_isBenefits = 0x4733C5;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;     // (out, id, function<void()>, w1, w2, w3, bool)
constexpr uint32_t BuyMagicStoneDialog_open = 0x588CAD;        // (const function<void()>&, bool)
constexpr uint32_t BuyMagicStoneDialog_openDiscount = 0x589005;   // (const function<void()>&, const function<void()>&)
constexpr uint32_t InOut_runOutAnimation = 0x6F24B5;
constexpr uint32_t ImageButton_setIsDisabled = 0x5318E1;
constexpr uint32_t Text_enableShadow = rwlib::cocos2d::ui::Text::enableShadow;          // (text, const Color4B&, const Size&, int blur)
constexpr uint32_t Text_enableOutline = rwlib::cocos2d::ui::Text::enableOutline;         // (text, const Color4B&, int size)
constexpr uint32_t Label_setAdditionalKerning = rwlib::cocos2d::Label::setAdditionalKerning;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                  // cocos2d::Size::Size(float, float)
constexpr uint32_t Color4B_ctor = rwlib::cocos2d::Color4B::ctor_base;               // (out, r, g, b, a)
constexpr uint32_t SafeBuffer_read = 0x45A269;            // (sb, dst, src, n, index)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;  // (out, mgr, uint64 uid, int, UserMedal::Medal)
constexpr uint32_t TutorialMisc_create = 0x777BB5;        // master::TutorialMisc::create(shared_ptr out, id)
constexpr uint32_t IntVector_pushBackSlow = 0x4D6CC1;     // std::vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t IntVector_dtor = 0x478961;             // std::__vector_base<int>::~__vector_base
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;               // __aeabi_memset(dest, n, c)

constexpr uint32_t VT_LayerFactoryPtr = 0xF7E130;         // __func<Layer*(*)()>: [vtable, function pointer]
constexpr uint32_t CreateLayer_MyPage = 0x46FDBD;         // createSceneLayer<SceneMyPage>
constexpr uint32_t CreateLayer_SellOutMaterialItem = 0x68D865;
constexpr uint32_t CreateLayer_DrawRaise = 0x68DEF5;
constexpr uint32_t COLOR4B_BLACK = 0x1007EBC;             // cocos2d::Color4B::BLACK
constexpr uint32_t COLOR3B_WHITE = 0x1007E88;             // cocos2d::Color3B::WHITE

constexpr uint32_t SCENEMANAGER_FLAG_4CC = 0x4CC;
constexpr uint32_t SCENEMANAGER_CALLBACK_548 = 0x548;     // std::function<void(bool)>
constexpr uint32_t APIMANAGER_USER_MEDALS = 0xD4;         // SafeBuffer* of UserMedal::Medal records (72 bytes)
constexpr uint32_t USERMEDAL_SIZE = 72;
constexpr uint32_t USERMEDALDATA_MASTER = 0x58;           // UserMedalData: master::Medal row (SafeBuffer*)
constexpr uint32_t MEDAL_RARITY = 0x470;                  // master::Medal: rarity
constexpr uint32_t MEDAL_FUSION_FLAG = 0x454;             // master::Medal: 1 = may be fused
constexpr uint32_t VSLOT_TEXT_GET_RENDERER = 0x2D8;       // ui::Text: the cocos2d::Label it draws with
constexpr uint32_t TOUCH_ENDED = 2;
constexpr int32_t TUTORIAL_MISC_FUSION = 7;               // master::TutorialMisc row: minimum rarity at +0xC
constexpr uint32_t TUTORIAL_FUSION = 29;
constexpr int32_t TEXT_JEWEL = 102600057;
constexpr int32_t BGM_SHOP = 501000920;
constexpr uint32_t STR_Txt_Jewel = 0xE04AD9;

uint32_t scrollOf(uint32_t scene) { return rw::host_ptr<Self>(scene)->scrollView; }
bool tapEnded(uint32_t fn, uint32_t type) {
    return ld32(type) == TOUCH_ENDED && rw::host_ptr<Self>(ld32(fn + 4))->canTouch != 0;
}
// SceneManager::changeSceneLayer(createSceneLayer<T>, ...)
void changeScene(uint32_t createLayer, uint32_t a, uint32_t b, uint32_t fade) {
    uint32_t mgr = sceneManager();
    Frame f;
    uint32_t factory = newFunction(f, VT_LayerFactoryPtr, {createLayer});
    call(SceneManager_changeSceneLayer, {mgr, factory, a, b, fade});
    functionDestroy(factory);
}
// BuyMagicStoneDialog::open(std::function<void()>(), false)
void openBuyMagicStone() {
    Frame f;
    uint32_t none = newEmptyFunction(f);
    call(BuyMagicStoneDialog_open, {none, 0});
    functionDestroy(none);
}
// scene->runOutAnimation(lambda without captures)
void runOut(uint32_t scene, uint32_t vtable) {
    Frame f;
    uint32_t next = newFunction(f, vtable, {});
    call(InOut_runOutAnimation, {scene + Self::INOUT_OFFSET, next});
    functionDestroy(next);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ callShopAPI
// $_0: the captured callback(true) when the request succeeded.
void SceneShopTop::onGachaListResult(Addr fn, Addr ok) {
    if (ld8(ok) == 0) return;
    if (ld32(fn + 0x20) == 0) return;
    call(Function_vb_call, {fn + 0x10, 1});
}
RW_FUNCTION(0x68E7A0, "SceneShopTop::callShopAPI::$_0::operator()", SceneShopTop::onGachaListResult);

// ------------------------------------------------------------------------------------------------ init
void SceneShopTop::onEnterScene(Addr fn) {
    call(Node_scheduleUpdate, {ld32(fn + 4)});
    call(AudioReplaceGameMainBGM, {uint32_t(BGM_SHOP), 500});
}
RW_FUNCTION(0x68E608, "SceneShopTop::init::$_1::operator()", SceneShopTop::onEnterScene);

void SceneShopTop::onExitScene(Addr fn) { call(Node_unscheduleUpdate, {ld32(fn + 4)}); }
RW_FUNCTION(0x68E56A, "SceneShopTop::init::$_2::operator()", SceneShopTop::onExitScene);

void SceneShopTop::onBackKey(Addr fn) { backScene(ld32(fn + 4)); }
RW_FUNCTION(0x68E4CE, "SceneShopTop::init::$_3::operator()", SceneShopTop::onBackKey);

uint32_t SceneShopTop::onTouchBegan(Addr, Addr, Addr) { return 1; }
RW_FUNCTION(0x68E436, "SceneShopTop::init::$_4::operator()", SceneShopTop::onTouchBegan);

// ------------------------------------------------------------------------------------------------ runInAnimation
// $_5: when checkDuplicateMedals is set, look through the owned medals for a second copy of a medal that can be
// fused (fusion flag, rarity at least TutorialMisc 7's value) and show the fusion tutorial (29) for it; otherwise
// (or when nothing was found) unlock the list and the buttons. The frame is the original's (0x188 bytes): the
// UserMedal::Medal passed by value to getMedalData is built from two cleared areas around 4 bytes the original
// never writes.
void SceneShopTop::onInAnimationDone(Addr fn) {
    uint32_t self = ld32(fn + 4);
    Self *s = rw::host_ptr<Self>(self);
    if (s->checkDuplicateMedals == 0) {
        call(DrawScrollView_setCanTouch, {s->scrollView, 1});
        s->canTouch = 1;
        return;
    }
    guest::Frame frame(0x188);
    const uint32_t sp = frame.addr();
    const uint32_t misc = sp + 0x80;        // shared_ptr<master::TutorialMisc>
    const uint32_t medal = sp + 0x88;       // shared_ptr<UserMedalData>
    const uint32_t seen = sp + 0x90;        // std::vector<int>: fusable medal ids met so far
    const uint32_t minRarity = sp + 0x9C, rarity = sp + 0xA0, fusable = sp + 0xA4;
    const uint32_t onClose = sp + 0xA8;     // std::function<void()> $_5 inner #2
    const uint32_t popupResult = sp + 0xC0; // openTutorialPopup's returned std::function
    const uint32_t medalArg = sp + 0xD8;    // UserMedal::Medal{} (cleared: +0x00..0x2C, +0x30..0x48)
    const uint32_t record = sp + 0x120;     // the owned medal's record: uid (8 bytes), medal id at +8
    st32(seen, 0);
    st32(seen + 4, 0);
    st32(seen + 8, 0);
    uint32_t list = call(fn::APIManager_getInstance, {}) + APIMANAGER_USER_MEDALS;
    uint32_t sb = ld32(list);
    bool shown = false;
    if (ld32(sb + 0x20) != 0) {
        for (uint32_t i = 0;;) {
            call(SafeBuffer_read, {sb, record, ld32(sb + 0xC) + USERMEDAL_SIZE * i, USERMEDAL_SIZE,
                                   USERMEDAL_SIZE * i});
            uint32_t mgr = call(MedalDataManager_getInstance, {});
            uint32_t uidLo = ld32(record), uidHi = ld32(record + 4);
            call(AEABI_MEMSET, {medalArg, 0x2C, 0});
            call(AEABI_MEMSET, {medalArg + 0x30, 0x18, 0});
            // stack arguments: int 0 at sp, the Medal (72 bytes) at sp + 8
            *rw::frame_ptr<uint32_t>(sp) = 0;
            for (uint32_t k = 0; k < USERMEDAL_SIZE; k += 4)
                *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(medalArg + k);
            rw::call_exact(MedalDataManager_getMedalData, {medal, mgr, uidLo, uidHi});
            call(TutorialMisc_create, {misc, uint32_t(TUTORIAL_MISC_FUSION)});
            int32_t need = 0;
            uint32_t miscRow = ld32(misc);
            if (miscRow) {
                call(SafeBuffer_read, {miscRow, minRarity, ld32(miscRow + 0xC) + 0xC, 4, 0xC});
                need = int32_t(ld32(minRarity));
            }
            uint32_t row = ld32(ld32(medal) + USERMEDALDATA_MASTER);
            call(SafeBuffer_read, {row, rarity, ld32(row + 0xC) + MEDAL_RARITY, 4, MEDAL_RARITY});
            bool found = false;
            if (need <= int32_t(ld32(rarity))) {
                row = ld32(ld32(medal) + USERMEDALDATA_MASTER);
                call(SafeBuffer_read, {row, fusable, ld32(row + 0xC) + MEDAL_FUSION_FLAG, 4, MEDAL_FUSION_FLAG});
                if (ld32(fusable) == 1) {
                    uint32_t id = ld32(record + 8);
                    uint32_t end = ld32(seen + 4), it = ld32(seen);
                    while (it != end && ld32(it) != id) it += 4;
                    if (it != end) {
                        // a second copy: the fusion tutorial, closing it unlocks the screen
                        st32(onClose, VT_EnableTouch);
                        st32(onClose + 4, self);
                        st32(onClose + 16, onClose);
                        call(SceneUtil_openTutorialPopup, {popupResult, TUTORIAL_FUSION, onClose, 0, 0, 0, 0});
                        functionDestroy(popupResult);
                        functionDestroy(onClose);
                        found = true;
                        shown = true;
                    } else if (end == ld32(seen + 8)) {
                        call(IntVector_pushBackSlow, {seen, record + 8});
                    } else {
                        uint32_t next = 0;
                        if (end) {
                            rw::save<uint32_t>(end, ld32(record + 8));
                            next = ld32(seen + 4);
                        }
                        st32(seen + 4, next + 4);
                    }
                }
            }
            if (ld32(misc + 4)) call(fn::shared_release, {ld32(misc + 4)});
            if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
            i++;
            if (found) break;
            sb = ld32(list);
            if (i >= ld32(sb + 0x20)) break;
        }
    }
    if (!shown) {
        call(DrawScrollView_setCanTouch, {s->scrollView, 1});
        s->canTouch = 1;
    }
    call(IntVector_dtor, {seen});
}
RW_FUNCTION(0x68E028, "SceneShopTop::runInAnimation::$_5::operator()", SceneShopTop::onInAnimationDone);

// $_5 inner #2: the tutorial popup closed.
void SceneShopTop::enableTouch(Addr fn) {
    uint32_t self = ld32(fn + 4);
    call(DrawScrollView_setCanTouch, {scrollOf(self), 1});
    rw::host_ptr<Self>(self)->canTouch = 1;
}
RW_FUNCTION(0x68E390, "SceneShopTop::runInAnimation::$_5::{lambda()#2}::operator()", SceneShopTop::enableTouch);

// ------------------------------------------------------------------------------------------------ initShopTop
// $_6: a draw was chosen in the list: hide the header and slide out to SceneDrawRaise.
void SceneShopTop::onOpenDraw(Addr fn) {
    uint32_t self = ld32(fn + 4);
    uint32_t mgr = sceneManager();
    {
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(SceneManager_hideHeader, {mgr, none});
        functionDestroy(none);
    }
    runOut(self, VT_OpenDrawRaise);
}
RW_FUNCTION(0x68DCBC, "SceneShopTop::initShopTop::$_6::operator()", SceneShopTop::onOpenDraw);

void SceneShopTop::openDrawRaise(Addr) { changeScene(CreateLayer_DrawRaise, 1, 1, 1); }
RW_FUNCTION(0x68DE14, "SceneShopTop::initShopTop::$_6::{lambda()#1}::operator()", SceneShopTop::openDrawRaise);

// ------------------------------------------------------------------------------------------------ bottom buttons
// $_7: the transparent button over the jewel icon.
void SceneShopTop::onTapJewelDummy(Addr fn, Addr, Addr type) {
    if (!tapEnded(fn, type)) return;
    uint32_t self = ld32(fn + 4);
    st8(scrollOf(self) + SCROLL_TAPPED_JEWEL, 1);
    st8(scrollOf(self) + SCROLL_BUSY, 1);
}
RW_FUNCTION(0x68DBF8, "SceneShopTop::initShopTopBottomButton::$_7::operator()", SceneShopTop::onTapJewelDummy);

// $_8: the jewel button: the discount offer dialog ($_14 when it closes, $_9 refreshes the button).
void SceneShopTop::onTapJewel(Addr fn, Addr, Addr type) {
    if (ld32(type) != TOUCH_ENDED) return;
    uint32_t self = ld32(fn + 4);
    Self *s = rw::host_ptr<Self>(self);
    if (s->canTouch == 0) return;
    if (ld8(s->scrollView + SCROLL_BUSY) != 0) return;
    call(AudioPlaySystemSEDecide, {});
    s->canTouch = 0;
    st8(s->scrollView + SCROLL_BUSY, 1);
    Frame f;
    uint32_t onClosed = newFunction(f, VT_DiscountClosed, {ld32(fn + 8)});
    uint32_t onRefresh = newFunction(f, VT_RefreshJewel, {ld32(fn + 12), ld32(fn + 16), ld32(fn + 20)});
    call(BuyMagicStoneDialog_openDiscount, {onClosed, onRefresh});
    functionDestroy(onRefresh);
    functionDestroy(onClosed);
}
RW_FUNCTION(0x68D8DC, "SceneShopTop::initShopTopBottomButton::$_8::operator()", SceneShopTop::onTapJewel);

void SceneShopTop::refreshJewelButtonCall(Addr fn) { refreshJewelButton(fn + 4); }
RW_FUNCTION(0x68DAAA, "SceneShopTop::initShopTopBottomButton::$_9::operator()", SceneShopTop::refreshJewelButtonCall);

// $_9 body: the jewel label and button state; greyed out while the jewel benefits are active.
void SceneShopTop::refreshJewelButton(Addr captures) {
    uint32_t self = ld32(captures + 8);
    titleLabel(ld32(captures), STR_Txt_Jewel, 9, TEXT_JEWEL);
    uint32_t text = findNode(ld32(captures), STR_Txt_Jewel, 9);
    Frame f;
    uint32_t noOffset = f.alloc(8);
    call(Size_ctor, {noOffset, 0, 0});
    call(Text_enableShadow, {text, COLOR4B_BLACK, noOffset, 0});
    uint32_t label = vcall(text, VSLOT_TEXT_GET_RENDERER, {text});
    call(Label_setAdditionalKerning, {label, fbits(-2.0f)});
    uint32_t benefits = call(SceneUtil_isBenefits, {});
    st8(ld32(captures + 4) + IMAGEBUTTON_ENABLED, uint8_t(benefits ^ 1));
    call(ImageButton_setIsDisabled, {ld32(captures + 4), benefits});
    uint32_t outline = f.alloc(4);
    uint32_t shade;
    if (benefits == 1) {
        uint32_t grey = f.alloc(4);
        call(fn::Color3B_ctor, {grey, 0xA7, 0xA7, 0xA7});
        vcall(text, node::setColor, {text, grey});
        shade = 0x47;
    } else {
        vcall(text, node::setColor, {text, COLOR3B_WHITE});
        shade = 0;
    }
    call(Color4B_ctor, {outline, shade, shade, shade, 0x80});
    call(Text_enableOutline, {text, outline, 2});
    rw::host_ptr<Self>(self)->canTouch = 1;
    st8(scrollOf(self) + SCROLL_BUSY, 0);
}
RW_FUNCTION(0x68CE10, "SceneShopTop::initShopTopBottomButton::$_9 (body)", SceneShopTop::refreshJewelButton);

// $_10: sell materials: hide the header's right side and slide out to SceneSellOutMaterialItem.
void SceneShopTop::onTapSell(Addr fn, Addr, Addr type) {
    if (!tapEnded(fn, type)) return;
    uint32_t self = ld32(fn + 4);
    call(AudioPlaySystemSEDecide, {});
    call(SceneManager_hideHeaderRight, {sceneManager()});
    runOut(self, VT_OpenSellOut);
}
RW_FUNCTION(0x68D644, "SceneShopTop::initShopTopBottomButton::$_10::operator()", SceneShopTop::onTapSell);

void SceneShopTop::openSellOut(Addr) { changeScene(CreateLayer_SellOutMaterialItem, 1, 0, 0); }
RW_FUNCTION(0x68D788, "SceneShopTop::initShopTopBottomButton::$_10::{lambda()#1}::operator()", SceneShopTop::openSellOut);

// $_11: buy one: the jewel shop.
void SceneShopTop::onTapBuy1(Addr fn, Addr, Addr type) {
    if (!tapEnded(fn, type)) return;
    call(AudioPlaySystemSEDecide, {});
    openBuyMagicStone();
}
RW_FUNCTION(0x68D514, "SceneShopTop::initShopTopBottomButton::$_11::operator()", SceneShopTop::onTapBuy1);

// $_12: buy three: the medal box upgrade dialog first.
void SceneShopTop::onTapBuy3(Addr fn, Addr, Addr type) {
    if (!tapEnded(fn, type)) return;
    call(AudioPlaySystemSEDecide, {});
    Frame f;
    uint32_t onClosed = newFunction(f, VT_MedalMaxUpClosed, {});
    call(SceneUtil_openMedalMaxUpDialog, {onClosed});
    functionDestroy(onClosed);
}
RW_FUNCTION(0x68D2B4, "SceneShopTop::initShopTopBottomButton::$_12::operator()", SceneShopTop::onTapBuy3);

// $_12 inner (bool, bool): not cancelled and asked to buy: the jewel shop.
void SceneShopTop::onMedalMaxUpClosed(Addr, Addr cancel, Addr buy) {
    if (ld8(cancel) != 0 || ld8(buy) == 0) return;
    openBuyMagicStone();
}
RW_FUNCTION(0x68D3F0, "SceneShopTop::initShopTopBottomButton::$_12::{lambda(bool,bool)#1}::operator()", SceneShopTop::onMedalMaxUpClosed);

// $_14: the discount dialog closed.
void SceneShopTop::onDiscountClosed(Addr fn) {
    uint32_t self = ld32(fn + 4);
    rw::host_ptr<Self>(self)->canTouch = 1;
    st8(scrollOf(self) + SCROLL_BUSY, 0);
}
RW_FUNCTION(0x68DB48, "SceneShopTop::initShopTopBottomButton::$_14::operator()", SceneShopTop::onDiscountClosed);

// ------------------------------------------------------------------------------------------------ backScene
// $_13: back to My Page (its APIs first).
void SceneShopTop::backToMyPage(Addr) {
    uint32_t mgr = sceneManager();
    rw::save<uint8_t>(mgr + SCENEMANAGER_FLAG_4CC, 1);
    Frame f;
    uint32_t none = newEmptyFunction(f);
    call(Function_vb_assign, {mgr + SCENEMANAGER_CALLBACK_548, none});
    functionDestroy(none);
    uint32_t none2 = newEmptyFunction(f);
    call(SceneUtil_callPrepareMypageAPIs, {none2});
    functionDestroy(none2);
    changeScene(CreateLayer_MyPage, 1, 1, 1);
}
RW_FUNCTION(0x68D100, "SceneShopTop::backScene::$_13::operator()", SceneShopTop::backToMyPage);
